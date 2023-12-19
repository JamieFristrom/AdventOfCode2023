#include <gtest/gtest.h>

#include <algorithm>
#include <format>
#include <iterator>
#include <queue>
#include <optional>
#include <regex>
#include <variant>

#include "pex.h"

using namespace pex;


enum class Cell: int8_t {
    Space,
    CubeRock,
    RoundRock,
    Invalid
};

using Map = std::vector<std::vector<Cell>>;

Map parseMap(const std::string& input) {
    const auto rows = pSplit(input, '\n');
    Map map;
    for( const auto& row: rows) {
        std::vector<Cell> newRow;
        for(int x=0;x<row.size();x++) {
            newRow.emplace_back(row[x]=='.'?Cell::Space:
                                row[x]=='#'?Cell::CubeRock:
                                row[x]=='O'?Cell::RoundRock:Cell::Invalid);
            assert(newRow.back()!=Cell::Invalid);
        }
        map.emplace_back(newRow);
    }
    return map;
}

bool inBounds(const Map& map, const Vec2& point) {
    return((point.x>=0)
        &&(point.y>=0)
        &&(point.y<std::ssize(map))
        &&(point.x<std::ssize(map[point.y])));
}


Map slideRoundRocks(const Map& oldMap, const Vec2& delta, const Vec2& start, const Vec2& end) {
    Map newMap = oldMap;
    int64_t yLoopDelta = sgn(end.y-start.y);
    int64_t xLoopDelta = sgn(end.x-start.x);
    assert(xLoopDelta!=0);
    assert(yLoopDelta!=0);
    for(auto y=start.y;y!=end.y;y+=yLoopDelta) {
        for(auto x=start.x;x!=end.x;x+=xLoopDelta) {
            if(oldMap[y][x]==Cell::RoundRock) {
                newMap[y][x] = Cell::Space;
                for (auto newV2 = Vec2{ x,y } + delta;; newV2 += delta) {
                    if(!inBounds(newMap,newV2) || (newMap[newV2.y][newV2.x]!=Cell::Space)) {
                        // hit an obstruction
                        auto lastV2 = newV2 - delta;
                        newMap[lastV2.y][lastV2.x] = Cell::RoundRock;
                        break;
                    }
                }
            }
        }
    }
    return newMap;
}

Map slideRoundRocksUp(const Map& oldMap) {
    return slideRoundRocks(oldMap, Vec2{ 0,-1 }, Vec2{ 0,0 }, 
        Vec2{ std::ssize(oldMap[0]),std::ssize(oldMap) });
}

Map slideRoundRocksDown(const Map& oldMap) {
    return slideRoundRocks(oldMap, Vec2{ 0,1 }, 
        Vec2{ std::ssize(oldMap[0]) - 1,std::ssize(oldMap) - 1 }, Vec2{ -1,-1 });
}

Map slideRoundRocksLeft(const Map& oldMap) {
    return slideRoundRocks(oldMap, Vec2{ -1,0 }, Vec2{ 0,0 },
        Vec2{ std::ssize(oldMap[0]),std::ssize(oldMap) });
}

Map slideRoundRocksRight(const Map& oldMap) {
    return slideRoundRocks(oldMap, Vec2{ 1,0 },
        Vec2{ std::ssize(oldMap[0]) - 1,std::ssize(oldMap) - 1 }, Vec2{ -1,-1 });
}

//Map slideRoundRocksLeft(const Map& oldMap) {
//    return slideRoundRocks(oldMap, Vec2{ 0,1 }, Vec2{ 0,0 },
//        Vec2{ std::ssize(oldMap[0]),std::ssize(oldMap) });
//}
//Map slideRoundRocksDown(const Map& oldMap) {
//    Map newMap;
//    for(int y=oldMap.size();y>=0;y--) {
//        newMap.emplace_back(std::vector<Cell>(oldMap[y].size()));
//        for(int x=0;x<oldMap[y].size();x++) {
//            if(oldMap[y][x]==Cell::RoundRock) {
//                for(int newY=y-1;;newY--) {
//                    if((newY<0) || (newMap[newY][x]!=Cell::Space)) {
//                        // hit an obstruction
//                        newMap[newY+1][x] = Cell::RoundRock;
//                        break;
//                    }
//                }
//            }
//            else {
//                newMap[y][x] = oldMap[y][x];
//            }
//        }
//    }
//    return newMap;
//}

int64_t calculateLoad(const Map& map) {
// You notice that the support beams along the north side of the platform are damaged; 
// to ensure the platform doesn't collapse, you should calculate the total load on the north support beams.
// The amount of load caused by a single rounded rock (O) is equal to the number of rows from the rock to the 
// south edge of the platform, including the row the rock is on. (Cube-shaped rocks (#) don't contribute to load.) 
    int64_t load=0;
    for(int y=0;y<map.size();y++) {
        for(int x=0;x<map[y].size();x++) {
            if(map[y][x]==Cell::RoundRock) {
                load+=map.size()-y;
            }
        }
    }
    return load;
}

void printMap(const Map& map) {
    if (false) {
        for (const auto& row : map) {
            for (const auto& cell : row) {
                std::cout << static_cast<char>(
                    (cell == Cell::Space) ? '.' :
                    (cell == Cell::CubeRock) ? '#' :
                    (cell == Cell::RoundRock) ? 'O' : 'X');
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
}

int64_t doTheThing(const std::string& input) {
    auto map = parseMap(input);
// Start by tilting the lever so all of the rocks will slide north as far as they will go:
    auto newMap = slideRoundRocksUp(map);
    return calculateLoad(newMap);
}

Map doCycle(const Map& lastmap, int64_t steps) {
    if (steps <= 0) return lastmap;

    auto newMap = slideRoundRocksUp(lastmap);
    printMap(newMap);
    std::cout << "U: " << calculateLoad(newMap) << std::endl;
    if (steps <= 1) return newMap;

    newMap = slideRoundRocksLeft(newMap);
    printMap(newMap);

    std::cout << "L:" << calculateLoad(newMap) << std::endl;
    if (steps <= 2) return newMap;

    newMap = slideRoundRocksDown(newMap);
    printMap(newMap);

    std::cout << "D:" << calculateLoad(newMap) << std::endl;
    if (steps <= 3) return newMap;

    newMap = slideRoundRocksRight(newMap);
    printMap(newMap);

    std::cout << "R:" << calculateLoad(newMap) << std::endl;
    assert(steps == 4);
    return newMap;
}

int64_t doTheThing2(const std::string& input) {
    std::vector<Map> mapHistory;
    mapHistory.emplace_back(parseMap(input));
    for(;;) {
        auto newMap = doCycle(mapHistory.back(),4);
        for (auto mapIt = mapHistory.begin(); mapIt != mapHistory.end();mapIt++) {
            if (*mapIt == newMap) {
                // found loop point
                int64_t loopSize = std::distance(mapIt, mapHistory.end());
                int64_t loopStart = std::distance(mapHistory.begin(), mapIt);
                // where do we land in future loop?
                int64_t loopMod = (1000000000 - loopStart) % loopSize;
                Map finalMap = newMap;
                for (int i = 0; i < loopMod; i++) {
                    finalMap = doCycle(finalMap, 4);
                }
                return calculateLoad(finalMap);
            }
        }
        mapHistory.emplace_back(newMap);
    }
}

const std::string sampleInput = 
R"(O....#....
O.OO#....#
.....##...
OO.#O....O
.O.....O#.
O.#..O.#.#
..O..#O..O
.......O..
#....###..
#OO..#....
)";

TEST(Aoc14Tests, sampleInput_doTheThing_136) {
    ASSERT_EQ(136, doTheThing(sampleInput));
}

TEST(Aoc14Tests, sampleInput_doTheThing2_64) {
    ASSERT_EQ(64, doTheThing2(sampleInput));
}

const std::string expectedOneCycle =
R"(.....#....
....#...O#
...OO##...
.OO#......
.....OOO#.
.O#...O#.#
....O#....
......OOOO
#...O###..
#..OO#....
)";
TEST(Aoc14Tests, sampleInput_oneCycle_expectedMap) {
    auto startingMap = parseMap(sampleInput);
    auto expectedMap = parseMap(expectedOneCycle);

    ASSERT_EQ(expectedMap, doCycle(startingMap, 4));
}

TEST(Aoc14Tests, testtest) {
    ASSERT_EQ(1,1);
}

const std::string puzzleInput = 
R"(....#..OO.O..OO..O...#....OO....#........#..#..OOO..#.....#..#.O#O.#...#......#..O......O....O#.#O.#
.O.#.#.O.O.OO...O..O...O.O.OO.....O.....#...#..O#O....OO##.O...O..#........O......O.O#.....O##O.#.O#
O.O#.....#.O#OOO..#O..#OO....###.#...#.O#....O..O...O###.......O...........O...OO.........O.....#.#.
.O.O...O#O##.#.O.#..OOOOO##......##O....O.........#.......#.#.##.O.......O.......#...O.O....#..O....
......#..#.##.OO.##..#..#OO.O#.#.O..........O.......#..O.O.O........O...O...##.O.O.#O........O.#OO..
......##.O.....#.O..O##.OO.##O#..O.OO......O.OO..O..O#.....O..O.....O#O...........#.O.##..OO..O....O
.....#.....O.O.#.##.....#.#..#.#.O#..##O#....#O..#.......#.O#OOOO...O...OO..O..O..O.#O.#..O.O..O.##.
......#......O##.#..#..O#............OOO............O.......O...O..OO.....#OOO.#O#....#...O........#
....#........O..O..O.##...O...#O...#.O.........O.O...#.OO#OO..OO.OO.....O#........#.#O..O.#........#
..O#.O#....O......O#.#.O#..#...#..O#....OO....O...OO.O..O......O.....O...O##OO........#...OO#..##.O.
.O#.O#..#..#O..O..O.##.O.O#.O.OOO.O.#O....O......OOO.#.....O.........#..#..O.....O..OO.#......OO..#.
...OOO.#OO#....#.O.#..O...O....#....O...O..O.OO#.O...O....O#.#.O...........#...#.O...#...OO#.O......
.O.#.O........#.....O..#.O#...O.#....#...O.......O.OO.OO#O..#..O..#.O...#.#....OO..O.O#..#.O.OO##...
.O...OO..#.#.OO#O....O.O.....OO...#O......OO........O.O......#.#.#O#...O..OOO#....##..O.##....#....#
...O...OO.O.#O.O..##.O.OO...#..O....O..O#.....#O.OO.......O.#........OO.........#..O.#...O#.O.......
O..#.O....O..#..............O.....##.....O.#.....#...........O.#.O...O.....##....O..O..#.O.#O##..O..
...O...O..O...O#..O#.OO#..O........#....#.O........#....#.....O...O.OO..O....#.##...O..#.O##..O..O..
..OO...O...O..O#.O#O...#.##.#...#..OO.O.#.#..O....O..#O.O.....O...#.#.......O...####.....#.......#..
.#O...O.O.......OO##...#.......#O.#..#OO...O......O...O#..#.....O#...O..O...........OOO.O...........
#....O.O.O........O.O#...#....O.O.........#..#.#.O.O..O.O......#.O.OOO.O...#O..OO..O#...O....#.O..#.
O.......#..O.......O..O..#.#O.O.....O#....O....O.O#..OO.#...O.#..O...#.O#...#..#.O.......#...O.###..
.OO#...##.O.OO#..#O#O...O..#.O.O....OOO#.O.#O.OO.#O...O...##O..##....OO#...O.#.........##..#.O....#.
......#...O.#...#O....O..O...#O.O....#O....#OOO.#....O.O.##..O..O....O........O.....#....O##.....O..
O.O...O#.#O......#O...#......O..O..#....#...#.O#.O.O..#......#....#......O.#.OO..#..O...O.....#O#.OO
#...O....O......O...#....#O.O..O.#......O#..##..#O#.#..O...O#..O..O##.OOO...#.OO..OO.#.#..##O.OO....
O.....O...#........#.O....OOOO....O............#..O...O....#..O.....#..O....O...#.......OO....O.#O..
#.O...#....#....#...#...OO...#O#..##..O...#.....OO....O..##...#..........#.....##OO.#...#.O.OO.#..O.
#O#O#.###.......OOO.#....O....##.O...#.#....O...#.O#.......#...#O..O.O..O#..#...O#.#O..OO...#OO.#.O#
.#O...O.........O...O.#O#O#.O.......##..##..##.O.#.#.O..OO.#.OO...OOO..#.O...O...#O..O.O#..#O#.OO..#
.#..#O...O..O..OO#.#.O..##.......#O.####..#O...OO#O...O........O...OO...OO..O......#..O.......O#.O..
.....#O#.#....OO.O.....O...###O...#.#........#...O..O....O##O#...###....O......O...O#..#..O...O.#.##
..O...#..#..O..#.OO..O......O#OO....O...O..#..........##..O#.....#.....#.OO#.#O...O#..O#...#.#.##...
..O...#...O###......O.O..O.#.O#..........O.......#O...#.##..O....O.O...#.#....O..O.#...#.O.#O.......
..#.#..#O#.....#......O...##OO.O...#..#O#O..##.O....O##....OO....#....#..O.#........OO..##.O.....OO.
O..#O.#..#.#...O.OO...OO...#.O#.#.O.O..#.........#....O...#.O..O#.....O......O...##.......#.##OO.O..
..O.OO..O..#.....#.O#...#.O..OO..OO...OO..O..#O#........O.#.O.#.O...O#O........O.....O#..#.....O...#
..O...OO.#.#O.OO.O##.##.O.O#O#.OO......OOOO.#........#.....O...#...#.##..#........#.OO..#.O.........
..##.#OO#..O#.#.#....OO.O..............O..#....#.#.OO.O..O..OO..O.#..O.....O....#.....##...O.#O..O..
..O.##..#..O...O....O..#........O.#..#.#.O......OO#OO.OO..OO.O###..#....O#.....#O..#....#OO#...#..O.
.O....O.....#O.#.O...O..#.#....###......O...OO###.O....#.#O..O#..##OO...#..O.........O#...O.....O...
O.OOO.O.#OO..#.......OOO..OO.#........O.O...OO......O....O#.O.O.....O......##....#......O....O...#..
..##..#...O....O..#.#..#OO..O..............O.O....#....#OO...#.#.OO##O...OO.O#O....##.OO....#..##.#O
...O....O#.#OO.O...#.O.##...#..O.#..#O.OO#.....O.O..O.....#....O#O....#.O.O..OO...O#O.O......###.O..
...O.O.#.............O.....##OOOO....O...O...#..O......#.##.#O...O#....#..O##.O....#O.O..#OOO....#..
.##OO..O...O.O.O....#.......O.#..#...OO...#..#..O.#.O.......O....O.....O.O.#.OO#O...#OO.O....#.#O...
...O.#OOO#.O.#...OOO......O..#.##OOO......O..OO.##.##....##.#..OO....O..O..............#...OOO......
........#..OOO.#.#...O.#..##.O#.O##.O.##.##.#..O#O.O#......#..#O.#....OO............#...###.OOO....#
#.O.O.....O...........OO...#OOO#..###...#.O#.#.....O..#..#.#.OO......#....O#.....#.O....#O#.OO.O..O.
O...........#..OO...OO...O..O.O##.....#....O##O.O.....O#..OO#.#O...#...OO..OO.......#O.#.#OO..#O###.
O...O.OO........#...##OO.........O.#....O.......#....#.#...#..#O.#...#..............#...#........O..
.#O....O.O....#...O##.#..O..OO#..#............#O...........O.OO#..##......O...O..#........#..#......
...............#.........#O..#....O..##.O......#....#.OO.#.#.O.O.OO....O.....OOO...........O.#..O..O
......O.#O..........O#O.#..O#.O#.......#.O...O...#.....O.....#..O...#..#..O...##O#..O#.O...#.OOO..O.
##O#...#..O#........#O.....O...#.#OO#O...O.##..OO..O....OO.O.#...O......O..#...#.O.#..##OO....O..O.#
........O......#.OO..OO.OOO.O...O.OO.....O..#..OO#O.O....O......O.#..OO..O#O..#.O#..#....O..O..#.#..
##..OOO.#OOO..O.O.O..O.#..O..O#..OOO.#..OO..OO..#.#.OOO...##..O#..O.#..#.#O....O..........#....O..OO
.......#.....O......O#O...##.....###...#O...#....#.#....O....O..O.O..O...#OO...#O..OO.....O.#..O....
O.#...O..#.....O..#..O....O#.....#..#.#..###....O.OO...#O..#O#.##..O.#...O.##O....OO.O...O#.O..O..#.
....#.O..#O.....O..OO#OOO#....#...#.#..#.#.....O....O..O....#....O#....O........O..O#.#.#.###O.....O
.....#O...#.#....O.....###...O....#.#...O#..O..##...O.O....#..O#.#...O.OO..#OO.....O...#..OOO....O..
#O...O.........#...#.OO....O.....#....##.#..#..##O.OO#O...O#O.....#..O...O...###.O#....#OO.##O.#O...
O..OOO..#O.O.O#..O..O.O.##..O..OO#O....O....#..OO#.OO.#.O...O.....O.O..##...#....#...O....O........O
...OO.O.....#..#.#....O...OO...#O......O.#..O#..O..O.......O.O....#.O....O.##.....OO#..#..O#.OO#O...
..........OO....#..#O.O..#....O#.#.....#...#...OO...#..O.###O...##.##..#.....#.#..O.O..O...###O.....
O..#...OO..O#....O...#O.....O....#....#..#O...#.O#..#..O...O#O.#O....O#O....O#..O.#.#.#..O.....O...O
.O.O...O.......OO..O...#......OO.#.#.#....#..#....#O.........#OO..O...#..O..O#.#..O..##..O.#O..#..O.
.#.O..O#..O..O#O.O...O#.....O..O..#..#...OO#...........#.O.OO...#..O.#.....#........#O.#.O....#O.O..
.O.....O..O...O..O......O#.O..O...........##..O.#.O.O.O.OOO....#.O..#O.#.##.......O#.#.....##...#O..
.O.O....#..#.#O....O.#......O.#.O#......O##..O....#.#...#.#.OO.......#OO#.##OO.....O.#O....OO.......
#O#....OO...O......O....OO#O.....O.#.O.....#.O......O..OO..O..OOO.O..#.O#..#..#.....O.O..#.O...#O.##
.......O.#O..#.#.....#.O.#..O..OOOO.#.O.O#...##.#...O.#OO...##.....#..O.O...#.....O.......#O.O#O...#
#O..#.#..OO.O.O...#.........#O.O#.......O#..#O.O##.#...O.##O.O..#.O......OOO.O....#...O.O...#......#
..O##O.O#.O...O.......O..#...##..O.#.O.OO.OO#.....O.#..............O.O#.##.O.O....#..#...O.....#..##
..............O.........#.##......#....##...#...O.O##O.O..##..O.....O#....O.#.....O..##..#..OO....##
#..#......O.O.O..O..O...#.#.#.OO..O#.O..O##OO#...#O...O.O.................#O.....#O.O#O....#......OO
..O#O#......O..#O......#.........O#O.#.O.....O.#.....O..O..O.##....#....OO....##..O.O.....O...O...O#
..O.O...O....OO......O....O..O.#O.O........OO......O#...OO##.........#.#...#.#......##O.##O....O.O#.
..O...#OO...O........O.O.....#.O..###O..#.....O.......#.....O...#O.#.....#....##..#....O.##.........
..#.#O.......O#..........#.#..#...O.....O.OO#..O...O.#....OO.O.OO....O..O.O#.....#..O.....#O..O..#O#
OO..O##..O.O...#.O..O..#....O..O....O..##.#..#O.O.......#.....O#..O.O.OOO..OOO#O##.#..#O.......#....
...O#.O..#.....OO.#O.O#O.O.O##..OO..O......O..##.....O....#.##....O..#.O....OO....O.#OO..O...#..O#..
O...OOO...#.O#..O#.O.##.#O.O..OOO..O...OO#OOO...O....#..O#...OO#.O..#.....O#.O...#..........OO#.#.##
...OO.#O#....O#...#.#......O....O#........O#...#O#...#...O.....#.OO#...##.OO.OO...#..O.O.....O..O...
.##..O..#O#O...#O.O..O#....#..#.......#....#.##.......#...##.O#OO......O#O.....O#.O.##...O.....OOOOO
.OO.#.O#....OO.#OO.#O.O.......O.#......#.#O#.....O......##..O#O#O...#....O.O...#...O#....O...#.O....
.O##.O..O...O.....O#......OO.O..##...........##..#O...#...O#O.O...O#...###..#.O.O#O.#O..#......#.OO.
O.OO#..O..OO.O...#OO.#.OO.#...#.#...O...O..O#OOO..OO..O......O............O.....O#..O...#OO.O.#O#OO#
O##O......#..#...#.##O#.O.......#O#.O.............O.......#..O...#.OOO..O..O..OO#O#.O##..#O.#....OO#
.....OO#.O..O..OOO.O..OO.........O#...#.....O.....#..#..O....#O#.O....O....OO..#...OO.O.O#...#.O....
...#....#......#...O#O....O##.OO..#.O#OO#.OO..#.##..OO..#..O#..O......O......OO#..O.O..........O....
...O...#...OOOOO......O...#...#.............O#O..OO...O.#...O.O..#O.OO..O.O.O...O.......#.....O.##..
....O.#...O..#...#...O.......#....#..O.O..#.#...#..O.....O#O.....O...O.........#OO.O.#...#.......O..
.#.O...O.#.#O.O.#..##..#O....#OO...OO.#..O#O.OO....#O#OO...#.O......#....#...O....#.....O..#.....##.
..#.O.OOO#O#.OO.O....#O.........O.OO.#.#.#...O#O........O..#...O....O.O#.O...#....#.OOO.#..##...#.O.
....#O#O#O#.#.#.#O.....#..O..##O........##.O.......O.O#.##O..O..O......#OO.O.#.OOO...O..#..O.....O..
#.O...O.#.#OO..OO.O.....O...OOO....##.O...#O....O...O.#O.##.....O#...O.OO.#.....O....O.O..O.O......O
..OO#......O.....O......O...#.........#.OO....OO......O...#....#OOO...O.O.OO.....#..O...OO....OOO#O#
.O...#.#.O...#..........#.O..O#.....O..##..O....O..#..#....O..O.O.OO#.O...O.#.O....#........###..OO.
....O.#....O...O.O..#OO##......#.#O.........OO......O#.O.#..OOO.#...OO...#O##.#.O......#...O...O....
#.O..O..#..O#.#....O...#...O#O#O...OO.OO...##....#..OO#...#...O.#.#O#..O..........#O.O..O...OOO.#...
)";

TEST(Aoc14Tests, puzzle_doTheThing) {
    ASSERT_EQ(0, doTheThing(puzzleInput));
}


TEST(Aoc14Tests, puzzle_doTheThing2) {
    ASSERT_EQ(0, doTheThing2(puzzleInput));
}