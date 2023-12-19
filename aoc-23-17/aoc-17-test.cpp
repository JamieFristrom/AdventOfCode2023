#include <gtest/gtest.h>

#include <algorithm>
#include <queue>

#include "pex.h"

using namespace pex;

using HeatMap = Grid8;

using CostMap = Grid<int32_t>;

const std::vector<Vec2> dirs{ {0,-1}, {1,0}, {0,1}, {-1,0} };  // never eat shredded wheat

bool willBeFourInARow(const std::vector<Vec2> routeSoFar, const Vec2& nextDelta) {
    auto lastStep = routeSoFar.rbegin();
    int count = 0;
    for (auto stepIt = lastStep + 1; stepIt != routeSoFar.rend() || count>=2; ++stepIt, ++count) { // I always find it unintuitive that you increment the iterator to go in reverese
        if (*lastStep - *stepIt != nextDelta) {
            return false;
        }
        else {
            if (count >= 1) {  // 2 successes means 3 in a row; the next step will be 4. Also unintuitive
                return true;
            }
        }
        lastStep = stepIt;
    }
    return false;
}

int costSoFar(const HeatMap& heatMap, const std::vector<Vec2>& routeSoFar) {
    const auto result = pReduce(pTransform<std::vector<int>>(routeSoFar, [&heatMap](const Vec2& xy) {
        return (int)heatMap[xy.y][xy.x];
        }));
    return result;
}

// brute force recursion
// does memoizing require knowing where we've visited? that would be a ridiculous sized table
// the three step limit seems to ruin regular memoization; a best path a1-a2-a3 might not be best for
// a0-a1-a2-a3.
//  simpler problem, start upper left, go to lower left. wiggling around the 3 is necessary 
//  11311
//  11111 
int minimumHeat(const HeatMap& heatMap, Grid<int>& best, const std::vector<Vec2>& routeSoFar) {
    const auto step = routeSoFar.back();
    if ((step.y == gridHeight(heatMap) - 1) && (step.x == gridWidth(heatMap) - 1)) {
        return heatMap[step.y][step.x];
    }
    auto cost = costSoFar(heatMap, routeSoFar);
    if ( cost >= best[step.y][step.x]) {
        return INT_MAX;  // discard this line of inquiry
    }
    best[step.y][step.x] = cost;
    int minHeat = INT_MAX;
    for (const auto& dir : dirs) {
        const auto nextStep = step + dir;
        if (inBounds(heatMap, nextStep)) {
            if (!pContains(routeSoFar, nextStep)) {
                // not four in a row
                if (!willBeFourInARow(routeSoFar, dir)) {
                    // I bet this will break. Let's see.
                    const std::vector<Vec2> newRoute = pConcatenate(routeSoFar, { nextStep });
                    minHeat = std::min(minHeat, minimumHeat(heatMap, best, newRoute));
                }
            }
        }
    }

    return (minHeat==INT_MAX)? INT_MAX: minHeat + heatMap[step.y][step.x];
}

int doTheThing(const std::string& input) {
    const auto charGrid = parseGrid<char>(input);
    const Grid<int8_t> heatMap = gridTransform<int8_t>(charGrid, [](auto cell) { return cell - '0'; });
    Grid<int> best = createGrid(gridWidth(heatMap), gridHeight(heatMap), INT_MAX);
    return minimumHeat(heatMap, best, { {0,0} });
}
// I know this isn't going to work, why am I doing it
//totalCostFill(const HeatMap& heatMap) {
//    std::queue<Vec2> spaceQueue;
//    CostMap costMap = createEmptyGrid<int32_t>(ssize(heatMap[0]), ssize(heatMap));
//    spaceQueue.push(Vec2{ ssize(heatMap[0]) - 1, ssize(heatMap) - 1 });
//    for (;;) {
//        const auto curSpace = spaceQueue.front();
//        spaceQueue.pop();
//        for (const auto& dir : dirs) {
//            const auto nextSpace = curSpace + dir;
//            if (inBounds(heatMap, nextSpace)) {
//                costMap[nextSpace.y][nextSpace.x] = costMap[curSpace.y][curSpace.x]
//            }
//            costMap[nextSpace.]
//        }
//    }
//
//}

const std::string& sampleInput =
R"(2413432311323
3215453535623
3255245654254
3446585845452
4546657867536
1438598798454
4457876987766
3637877979653
4654967986887
4564679986453
1224686865563
2546548887735
4322674655533
)";

TEST(Aoc17Test, horiz3Path_willBeFourInARow_true) {
    std::vector<Vec2> path = { {0,0}, {1,0}, {2,0} };
    ASSERT_TRUE(willBeFourInARow(path, { 1,0 }));
}

TEST(Aoc17Test, vert3Path_willBeFourInARow_true) {
    std::vector<Vec2> path = { {0,1}, {0,2}, {0,3} };
    ASSERT_TRUE(willBeFourInARow(path, { 0,1 }));
}

TEST(Aoc17Test, LPath_willBeFourInARow_true) {
    std::vector<Vec2> path = { {1,1}, {0,1}, {0,2}, {0,3} };
    ASSERT_TRUE(willBeFourInARow(path, { 0,1 }));
}

TEST(Aoc17Test, shortL_willBeFourInARow_false) {
    std::vector<Vec2> path = { {0,1}, {1,1}, {2,1}, {2,2} };
    ASSERT_FALSE(willBeFourInARow(path, { 0,1 }));
}

TEST(Aoc17Test, changedDir_willBeFourInARow_false) {
    std::vector<Vec2> path = { {0,1}, {1,1}, {2,1}, {3,1} };
    ASSERT_FALSE(willBeFourInARow(path, { 0,1 }));
}

TEST(Aoc17Test, simpleGrid_minimumHeat_8) {
    Grid<int8_t> heatMap{
        {1,1,3,1,1},
        {1,1,1,1,1}
    };
    Grid<int> bestMap = createGrid(gridWidth(heatMap), gridHeight(heatMap), INT_MAX);
    ASSERT_EQ(8, minimumHeat(heatMap, bestMap, { { 0,0 } }));// , INT_MAX));
}

TEST(Aoc17Test, simpleGrid_minimumHeat_12) {
    Grid<int8_t> heatMap{
        {1,1,1,4,1},  // the best route to the 4 is straight there, but then we'd 
        {1,1,1,9,1},  // have to step on a 9. So we don't want it.
        {1,1,1,9,1}
    };
    Grid<int> bestMap = createGrid(gridWidth(heatMap), gridHeight(heatMap), INT_MAX);
    ASSERT_EQ(12, minimumHeat(heatMap, bestMap, { { 0,0 } }));// , INT_MAX));
}
TEST(Aoc17Test, sampleInput_doTheThing_102) {
    ASSERT_EQ(102, doTheThing(sampleInput));
}

