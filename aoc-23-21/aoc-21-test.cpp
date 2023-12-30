#include <gtest/gtest.h>

#include "pex.h"

#include <array>
#include <queue>
#include <limits>

using namespace pex;


// fills within existing 1-border in a map where 0 = empty and 1 = fill
// must start in empty square
template<typename IntType>
void floodFill(
	Grid<IntType>& map,
	const Vec2& startXY,
    const int64_t distance) {
	
	std::queue<std::pair<Vec2, int64_t>> fillPoints;  // position, steps
	fillPoints.push({ startXY, 0 });
	for (; !fillPoints.empty();) {
		cauto[nextSpot, steps] = fillPoints.front();
		fillPoints.pop();
		if (inBounds(map, nextSpot)) {
			if (map[nextSpot.y][nextSpot.x] == -1) {
				assert(steps < std::numeric_limits<IntType>::max());
				map[nextSpot.y][nextSpot.x] = static_cast<IntType>(steps);
				cauto x = nextSpot.x;
				cauto y = nextSpot.y;
				if (steps < distance) {
					fillPoints.push({ { x - 1, y }, steps + 1 });
					fillPoints.push({ { x + 1, y }, steps + 1 });
					fillPoints.push({ { x, y - 1 }, steps + 1 });
					fillPoints.push({ { x, y + 1 }, steps + 1 });
				}
			}
		}
	}
}


Grid<int16_t> floodFillSubGrid(const Grid<char>& grid, const Vec2& xy, int64_t distance) {
	auto map = gridTransform<int16_t>(grid, [](cauto cell) {
		cauto retval =
			cell == '#' ? -2 :
			cell == 'S' ? -1 :
			cell == '.' ? -1 : 0;
		assert(retval != 0);
		return retval; });
	// you can go 32 steps and come back 32
	// or you can oscillate
	// there will be a "ring" at 64
	// there will be a "ring" at 62 (walk 63, come back 1)
	// there will be a "ring" at 60 (walk 62, come back 2)
	// there will be a "ring" at 58 (walk 61, come back 3)
	// etc
	// so the problem can be, without retracing steps, what are the spots at ring 0, 2, etc
	// I think we can floodfill incrementally and just count even squares when done
	floodFill(map, xy, distance);
	return map;
}

// odd takes 0 or 1
int countLandingPoints(const Grid<int16_t>& map, int odd) {
	int sum = 0;
	for (int y = 0; y < gridHeight(map); y++) {
		for (int x = 0; x < gridWidth(map); x++) {
			if (map[y][x] >= 0 && map[y][x] % 2 == odd) {
				++sum;
			}
		}
	}
	return sum;

	// couldn't figure out this one's template badness. It was supposed to *save* time, damn it
	//return gridReduce(map, [](const int16_t cell)->int64_t { return (cell % 2 == 0)?1:0; });
}

int64_t validLandingPointsForSubgrid(const Grid<char>& grid, const Vec2& xy, int64_t distance) {
	auto map = floodFillSubGrid(grid, xy, distance);
	
	return countLandingPoints(map, distance % 2);
}

enum class CompassDir {
	NW,
	N,
	NE,
	E,
	SE,
	S,
	SW,
	W,

	Front = NW,
	Back = W
};

static Vec2 compassOffsets[] = {{-1,-1}, {0,-1}, {1,-1}, {1,0}, {1,1}, {0,1}, {-1,1}, {-1,0}};

bool isCardinal(const CompassDir direction) {
	return static_cast<int>(direction) % 2 == 1;
}

constexpr int64_t maxDistance = 300;

using MemoArray = std::vector<std::vector<int64_t>>;

int64_t validLandingPointsForSubgrids(MemoArray& memoArray, const Grid<char>& grid, const CompassDir incomingDirection, int64_t distance) {
	if (distance <= 0) {
		return 0;
	}
	cauto choppedDistance = std::min(maxDistance-1, distance);
	if (memoArray[static_cast<int>(incomingDirection)][choppedDistance]>=0) {
		return memoArray[static_cast<int>(incomingDirection)][choppedDistance];
	}
	cauto startCell =
		(incomingDirection == CompassDir::NW) ? Vec2{ 0,0 } :
		(incomingDirection == CompassDir::N) ? Vec2{ gridWidth(grid) / 2, 0 } :
		(incomingDirection == CompassDir::NE) ? Vec2{ gridWidth(grid) - 1, 0 } :
		(incomingDirection == CompassDir::E) ? Vec2{ gridWidth(grid) - 1, gridHeight(grid) / 2 } :
		(incomingDirection == CompassDir::SE) ? Vec2{ gridWidth(grid), gridHeight(grid) } :
		(incomingDirection == CompassDir::S) ? Vec2{ gridWidth(grid) / 2, gridHeight(grid) - 1 } :
		(incomingDirection == CompassDir::SW) ? Vec2{ 0, gridHeight(grid) - 1 } :
		(incomingDirection == CompassDir::W) ? Vec2{ 0, gridHeight(grid) / 2 } :
		Vec2{ -1,-1 };
	//assert(Vec2{ -1,-1 } != startCell);


	// be really easy to have off-by-one errors here. To make sure we don't imagine 3x3 subgrids
	// It's 5 steps from off map to opposite corner; 4 steps from off map to orthogonal. 
	// Imagine 5x5 : it's 9 and 7 (height+width-1 and height+width/2)
	int64_t result = validLandingPointsForSubgrid(grid, startCell, distance);

	if (incomingDirection == CompassDir::NW) {
		// diagonals have to branch three ways
		result += validLandingPointsForSubgrids(memoArray, grid, CompassDir::W, distance - gridWidth(grid) - gridHeight(grid) / 2);
		result += validLandingPointsForSubgrids(memoArray, grid, CompassDir::NW, distance - gridWidth(grid) - gridHeight(grid) - 1);
		result += validLandingPointsForSubgrids(memoArray, grid, CompassDir::N, distance - gridWidth(grid) / 2 - gridHeight(grid));
	}
	else if (incomingDirection == CompassDir::NE) {
		// diagonals have to branch three ways
		result += validLandingPointsForSubgrids(memoArray, grid, CompassDir::E, distance - gridWidth(grid) - gridHeight(grid) / 2);
		result += validLandingPointsForSubgrids(memoArray, grid, CompassDir::NE, distance - gridWidth(grid) - gridHeight(grid) - 1);
		result += validLandingPointsForSubgrids(memoArray, grid, CompassDir::N, distance - gridWidth(grid) / 2 - gridHeight(grid));
	}
	else if (incomingDirection == CompassDir::SE) {
		// diagonals have to branch three ways
		result += validLandingPointsForSubgrids(memoArray, grid, CompassDir::E, distance - gridWidth(grid) - gridHeight(grid) / 2);
		result += validLandingPointsForSubgrids(memoArray, grid, CompassDir::SE, distance - gridWidth(grid) - gridHeight(grid) - 1);
		result += validLandingPointsForSubgrids(memoArray, grid, CompassDir::S, distance - gridWidth(grid) / 2 - gridHeight(grid));
	}
	else if (incomingDirection == CompassDir::SW) {
		// diagonals have to branch three ways
		result += validLandingPointsForSubgrids(memoArray, grid, CompassDir::W, distance - gridWidth(grid) - gridHeight(grid) / 2);
		result += validLandingPointsForSubgrids(memoArray, grid, CompassDir::SW, distance - gridWidth(grid) - gridHeight(grid) - 1);
		result += validLandingPointsForSubgrids(memoArray, grid, CompassDir::S, distance - gridWidth(grid) / 2 - gridHeight(grid));
	}
	else {
		// cardinal directions are all straight shots
		result += validLandingPointsForSubgrids(memoArray, grid, incomingDirection, distance - gridWidth(grid));
	}

	memoArray[static_cast<int>(incomingDirection)][choppedDistance] = result;
	return result;
}

//int64_t doTheThing(const std::string& stuff, const Vec2& xy, int distance) {
//	auto grid = parseGrid<char>(stuff);
//	assert(grid[xy.y][xy.x] == 'S');
//	grid[xy.y][xy.x] = '.';
//	return validLandingPointsForSubgrid(grid, xy, distance);
//}

int64_t doTheThingBigSlowMap(const std::string& stuff, const Vec2& xy, int64_t distance) {
	auto grid = parseGrid<char>(stuff);
	assert(grid[xy.y][xy.x] == 'S');
	grid[xy.y][xy.x] = '.';
	// how wide might our grid need to be?
	cauto neededWidthMultiple = ((distance - gridWidth(grid)/2) / gridWidth(grid) + 1) * 2 + 1;
	cauto neededHeightMultiple = ((distance - gridHeight(grid)/2) / gridHeight(grid) + 1) * 2 + 1;
	auto bigGrid = createGrid<char>(neededWidthMultiple * gridWidth(grid), neededHeightMultiple * gridHeight(grid));
	for (auto row = 0; row < gridHeight(bigGrid); ++row) {
		for (auto col = 0; col < gridWidth(bigGrid); ++col) {
			bigGrid[row][col] = grid[row % gridHeight(grid)][col % gridWidth(grid)];
		}
	}

//	printGrid(std::cout, bigGrid, "");
	return validLandingPointsForSubgrid(bigGrid, Vec2{ gridWidth(bigGrid)/2, gridHeight(bigGrid)/2 }, distance);
}

int64_t doTheThing(const std::string& stuff, const Vec2& xy, int distance) {
	return doTheThingBigSlowMap(stuff, xy, distance);
}
struct CondensedMap {
	std::array<int64_t, static_cast<int>(CompassDir::Back) + 1> dists;
	bool visited=false;
	int totalValidSpots = -1;
};

using OverMap = Grid<CondensedMap>;

int64_t mapGetDist(const OverMap& overMap, const Vec2& v2, const CompassDir& dir) {
	return overMap[v2.y][v2.x].dists[static_cast<int>(dir)];
}

int64_t doTheThing2(const std::string& stuff, const Vec2& xy, const int64_t distance) {
	// a flood fill within a flood fill
	auto grid = parseGrid<char>(stuff);
	assert(grid[xy.y][xy.x] == 'S');
	grid[xy.y][xy.x] = '.';

	// using "OVerMap" here 
	std::vector<CondensedMap> memoArray(8);

	// will recursion be fast enough, if we memoize? It'll go ~200k calls deep...
	// let's try it
	// nope
	//int64_t sum = validLandingPointsForSubgrid(grid, xy, distance);

	//for(cauto direction: enum_range<CompassDirection>(CompassDirection::Front, CompassDirection::Back)) {
	//	assert(gridWidth(grid) == gridHeight(grid));
	//	assert(gridWidth(grid) % 2 == 1);
	//	cauto distanceTravelledSoFar =
	//		isCardinal(direction) ? gridWidth(grid) / 2 : manhattanDistance(xy, Vec2(0, 0));
	//	sum += validLandingPointsForSubgrids(memoArray, grid, direction, distance-distanceTravelledSoFar);
	//}

	// idea 2, floodfill of floodfills
	int64_t overMapMaxWidth = 2 * distance / gridWidth(grid) + 1;
	int64_t overMapMaxHeight = 2 * distance / gridHeight(grid) + 1;
	OverMap overMap(overMapMaxHeight);
	for (auto& row : overMap) {
		row = std::vector<CondensedMap>(overMapMaxWidth, { 0 });
	}

	// overmap, innermap
	// go out in a spiral
	Vec2 subGridCursor = xy;
	const Vec2 overMapStart = Vec2(overMapMaxWidth / 2, overMapMaxHeight / 2);

	Vec2 overMapCursor = Vec2(overMapMaxWidth / 2, overMapMaxHeight / 2);
	cauto mid = gridWidth(grid) / 2;
	cauto max = gridWidth(grid) - 1;

	CompassDir travelDir = CompassDir::N;
	int emptyCells = 0;
	int64_t sum = 0;
	for (;;) {
		// calculate starting point from position relative to start
		Vec2 thisXY;
		CompassDir heading;
		int64_t remainingDistance;
		if (overMapCursor.x < overMapStart.x) {
			if (overMapCursor.y < overMapStart.y) {
				thisXY = Vec2(max, max);
				heading = CompassDir::NW;
				remainingDistance = mapGetDist(overMap, overMapCursor + Vec2{1, 1}, heading);
			}
			else if (overMapCursor.y > overMapStart.y) {
				thisXY = Vec2(max, 0);
				heading = CompassDir::W;
				remainingDistance = mapGetDist(overMap, overMapCursor + Vec2{ 1, 0 }, heading);
			}
			else {
				thisXY = Vec2(max, mid);
				heading = CompassDir::SW;
				remainingDistance = mapGetDist(overMap, overMapCursor + Vec2{ 1, -1 }, heading);
			}
		}
		else if (overMapCursor.x > overMapStart.x) {
			if (overMapCursor.y < overMapStart.y) {
				thisXY = Vec2(0, max);
				heading = CompassDir::NE;
				remainingDistance = mapGetDist(overMap, overMapCursor + Vec2{ -1, 1 }, heading);
			}
			else if (overMapCursor.y > overMapStart.y) {
				thisXY = Vec2(0, 0);
				heading = CompassDir::E;
				remainingDistance = mapGetDist(overMap, overMapCursor + Vec2{ -1, 0 }, heading);
			}
			else {
				thisXY = Vec2(0, mid);
				heading = CompassDir::SE;
				remainingDistance = mapGetDist(overMap, overMapCursor + Vec2{ -1, -1 }, heading);
			}
		}
		else /*if (overMapCursor.x == overMapStart.x)*/ {
			if (overMapCursor.y < overMapStart.y) {
				thisXY = Vec2(mid, max);
				heading = CompassDir::N;
				remainingDistance = mapGetDist(overMap, overMapCursor + Vec2{ 0, 1 }, heading);
			}
			else if (overMapCursor.y > overMapStart.y) {
				thisXY = Vec2(mid, 0);
				heading = CompassDir::S;
				remainingDistance = mapGetDist(overMap, overMapCursor + Vec2{ 0, -1 }, heading);
			}
			else {
				thisXY = Vec2(mid, mid);
				remainingDistance = distance;
			}
		}
		if (remainingDistance <= 0) {
			// how do we know when we're done, anyway
			emptyCells++;
		}
		else {

			cauto choppedDistance = std::min(maxDistance - 1, remainingDistance);

			CondensedMap overMapEntry;
			if((remainingDistance==maxDistance-1)&& (memoArray[static_cast<int>(heading)].visited)) {
				overMapEntry = memoArray[static_cast<int>(heading)];
			}
			else {
				auto map = floodFillSubGrid(grid, thisXY, remainingDistance);

				assert(gridWidth(grid) == gridHeight(grid));


				overMapEntry.dists[(int)CompassDir::NW] = map[0][0];
				overMapEntry.dists[(int)CompassDir::N] = map[mid][0];
				overMapEntry.dists[(int)CompassDir::NE] = map[max][0];
				overMapEntry.dists[(int)CompassDir::E] = map[max][mid];
				overMapEntry.dists[(int)CompassDir::SE] = map[max][max];
				overMapEntry.dists[(int)CompassDir::S] = map[mid][max];
				overMapEntry.dists[(int)CompassDir::SW] = map[0][max];
				overMapEntry.dists[(int)CompassDir::W] = map[0][mid];

				overMapEntry.visited = true;
				overMapEntry.totalValidSpots = countLandingPoints(map, distance % 2);
				if (remainingDistance == maxDistance - 1) {
					memoArray[(int)heading] = overMapEntry;
				}
			}
			overMap[overMapCursor.y][overMapCursor.x] = overMapEntry;
			sum += overMapEntry.totalValidSpots;
		}
		// spiral algorithm; turn right if you can, otherwise go straight
		cauto clockwiseRotation = (static_cast<int>(travelDir) + 2) % 8;
		const Vec2 clockwise = compassOffsets[clockwiseRotation];
		cauto clockwiseStep = overMapCursor + clockwise;
		if (!overMap[clockwiseStep.y][clockwiseStep.x].visited) {
			overMapCursor = clockwiseStep;
			travelDir = static_cast<CompassDir>(clockwiseRotation);
		}
		else {
			overMapCursor = overMapCursor + compassOffsets[static_cast<int>(travelDir)];
		}
	}

	return sum;
}

TEST(Aoc21Tests, TestTest) {
    ASSERT_EQ(1,1);
}



const std::string sampleInput = 
R"(...........
.....###.#.
.###.##..#.
..#.#...#..
....#.#....
.##..S####.
.##..#...#.
.......##..
.##.#.####.
.##..##.##.
...........
)";

TEST(Aoc21Tests, sampleInput_doTheThing_16) {
	ASSERT_EQ(16, doTheThing(sampleInput, { 5,5 }, 6));
}

TEST(Aoc21Tests, sampleInput6steps_doTheThing2_16) {
	ASSERT_EQ(16, doTheThing(sampleInput, { 5,5 }, 6));
}

TEST(Aoc21Tests, sampleInput10steps_doTheThing2_50) {
	ASSERT_EQ(50, doTheThing(sampleInput, { 5,5 }, 10));
}

TEST(Aoc21Tests, sampleInput50steps_doTheThing2_1594) {
	ASSERT_EQ(1594, doTheThing(sampleInput, { 5,5 }, 50));
}

TEST(Aoc21Tests, sampleInput100steps_doTheThing2_6536) {
	ASSERT_EQ(6536, doTheThing(sampleInput, { 5,5 }, 100));
}

TEST(Aoc21Tests, sampleInput500steps_doTheThing2_167004) {
	ASSERT_EQ(167004, doTheThing(sampleInput, { 5,5 }, 500));
}

TEST(Aoc21Tests, sampleInput1000steps_doTheThing2_668697) {
	ASSERT_EQ(668697, doTheThing(sampleInput, { 5,5 }, 1000));
}

// takes 60 seconds in debug with slow method
//TEST(Aoc21Tests, sampleInput5000steps_doTheThing2_16733044) {
//	ASSERT_EQ(16733044, doTheThing(sampleInput, { 5,5 }, 5000));
//}

const std::string garden =
R"(...................................................................................................................................
...........##......#..........#.#....#....................#....................#...#....#...........#................##...#......#.
....................#..........#.#.....###.............................#.......#.....#...........##.##.....#..............#..#.....
.#........#......................#...#.#..#..........#..........................###......#.#.#.#................#.............##...
.....#...........#..........#.#.#....................#.......................#.#..##.......##........#....##....#.......#..#.......
....#.#......#.....#............#............#..#...#..#...................#......#......#.............#.......#..#.....#....#.....
..#...#............#....#......#......#......#..#....#.#..........#..............#..................#..#......#...........##.#..#..
.............##...............#............#..................#....#..........#........#...............................#...#.#.....
......#......#....#....##......#.....#.........#...................................#....#................#....#......#.......##....
.....#.....#...................#....................#.......#..#....#..................#..........................#.#.......#......
...#.#..#..#................#..............#.........................##.................#..........#............#.##...........#.#.
....#........###.#..##.......#.......#..##..#...................................#.....#...##.........#..................#.......#..
..............#.............#...........#...........................#....#..........#..#...#.............#....#...........#........
.#.#.......#..#.....#..#............#...........#.........#.....#....#.........................#.............#.#...#..........#....
.......#.....#..#..#...............#...#...................##.#..........................................##.#......................
.............#........##.....#.............................#.........#..#.......................#.....#.........................#..
........#..............#......#......................#..........#..............................#...........#...........#...........
.....#.....#........#..................#....#.............#....#.........#..........................##.............................
....................#.#.....#...#........#..............#.......#.......#..#.........................#.............#.........#.....
...##.#....#.......#..........#.#........##..........#............#...#....#...#.............#...............#.............#.......
...#........#.#...................##.............#.#.......#...........#..#..................................#................#....
.........#............................................##..........#...#..#.......#..............#.....#..##...##....#..#...........
.#.....#.............#........#.......#...........#........#.#....##............................................#..............#...
.......#....##...#.....###.#....#..#............#...#.........................#.................#..#.#...........#.................
.................##....#.........#............##..#..........#.........#.....##.##............#...#.......#.......#......#.#.......
..##..............#..#..#...#.......#.......#...#..........#..........#.......##....#..........##............#.#....#....#..#......
........#...................#.#..###..............#...#.#.#.#.....#.....##..#...###..............#.......#..........#.....#........
..........##.............#......#.#....................#.#...................#..........#.............#.#....................#..##.
)"
R"(.#............................#..........#.#...#.........##..#...........#.#.....#...............#.........#.....#..#..............
....................#....#...##..........................#...#....#.#.......#.#...........................#...................#..#.
..........##......###....###.#............#.................................#..#...........#.......#..#.........................#..
..#.........#......###..................................#..........................##.#.#..................#.........#.##..#.#.....
.....#...................#...#...........#..#......#...................#...#...........#...#.........#...............#.............
........#.#.#.....................................#.#.........##......#.##.....#.#...#...................#.....................#...
..###..#......#.#.#.#................#.......#.#.....................#.#..#.#...............#....................#........#.##..#..
.#........#......#.##.....#...........#.............#....#...........#................................................#............
....#.........#..#................................#...............................#............................#...................
....#...#....#..#......#.........#..........#.......#.......#..........#.........#.......#.....#.............#......#...#..........
.....#..#......................................................#...#.........#................##...........#...........#..###......
...##....#...........#..........#.........#..........#..........#....##.............#....................................#......#..
....#........#.................#...........#..........#...##...................#....##...#.....#..#..............#..#..#....#......
...#.#............#.#.......................................#..#..#..................................#...............#.#.#.........
....#............#...........#...#..........#..#......................#.#...#.....#....................#.......##..........#.......
................#...................#....#....................##.........................#........................#......#.........
..............#...............#..#.......#..#......#....#...#.................#..#....#........#..#.....#.....................#....
.......#........................#.#....#....#..#................#....#.........#..............#........#...........#...............
...#.......#................#.....#........#....##.................#...#...............##.......#...#....................#.........
........#.........................#..#...##..........................##......##....#.....#.#....#..##............................#.
..#...##................##........#.........#..#.........##................................#.............#.#.................#.....
....#.............................#.............#...............#...........#...............#....#.....#....#.#....................
..........##.............#......#..................#.........#......#....#.......#....#......#....#................................
.....#.#.#........................#..#.........#.....................#..##...#.......#......#..#.....#...#..#...#..........#.......
...................#.#..........#...............#...#.#......#....#.................#.................###.....................#....
.....................................#.......#....#................#..........................#..........#.................##...#..
....#................#..#......#....#..................#..........................................#....#....#......#.......#.......
......#..........#........#...#...#..#................#.........#.........#...#.........#.#....................#..#................
.....#................#...#.#.#....#.#............##....#...........#......##..##............#......##.......#...#...............#.
...#..........#..........................#.................................#................#..........###...##.......#............
...........................#..........#......#.....#...#.#....#.....##.........##.....#.......................##..#..#..........#..
....................#......##...#.............#.....#.#....#...........#...##..#..#............#...#...............#...............
...............#.##.....#..........#..............#......#........#...........#..#.#......................#...#..#..##.............
...................#....##...#..#..............#....................###.....#............#.........##..#.....#.##..#...............
............................#......#.##.....#....#..#...#..###............#.............#.....................##...........#.......
............#.....#.#.#.........#...#........................#......#...................#...##..#.................#........#.......
................................#.........................................#...#.......#....#.........#.........#.....#...#.........
.................................................................S.................................................................
................#.......#.............................#.#......#................#..#..#......#..........#.............#............
.............##........#.......#.#............#.......#.#.......#...#.....#...........#....#..#................#...........#.......
............................#.#....##............#.##..###..............................#....#............#..#........#............
..................#........#.........##.....#....#...##.#......................###.....#..#........#.#.......##.......#............
...................#...##..........#.#..#................##....................###...................#.....#........#.#..........#.
.................##........................#....#.#..#.....##................#...........................#......#................#.
............................#........#........#..##.....#..#.......#......#.#...#...................................#...........##.
..............#..........#............#...#...............#.........#.#...#...##..##.....#...........#...#..........#.#............
.#.............#...#.........#.....###.#...............................#.....#.....#............#..#.........#.#.#.................
....#...............#..#.....#.....#.......##....#.#........#.##.............##..#.............#.#.#...........................#...
........................#.....#..#....#......#......#..#....#...#..#..........#....#...............#.........##..#.................
....#................#.....#.#............#..................#..#................#.........#.....#.............#...................
.................#.......#.....##.##.....#..................#..#......#....#...............#............#.#.#....#.........#....#..
.......#................#..#.........#......###.....#.#........#....##......#.......#..#.#.......#.#....#.#...............#........
...#.....#.#...............#....#..............#.........#...........#............#............#..#.#..##..........................
......#...............##.............#.#...............#....................#..#..##.#.#.................#......................#..
..#.....##..#.............#...#.......#.......................#........#.#.#............#...................#...................#..
........................#..#..................#..#.#.#..............................#...#...#......#....#.#........................
...#....................#................................#...#.....#...........#........#..#........#...................##.#.....#.
......#...##...#.................#..#..#....#.....#.....#.....#...#..................#.............................#..#......#.....
....................................#..................#....#...#.........#....#......................#.............#....###..#....
.##..#..#...##..........................#..#..#...........#..............#....#.........#....#....................#......##........
.##............#.#...............##....#.....#.......#..........#......#...............#..#...#........#.................#..#......
.#..#.##.........##.#..........#...........#...#...#.....#.#..#............##.........##...#.....................#...#........#....
.......#....#.#.....#........#..................#.............##........#..........##.........#..............#.#..#...#.....#.#..#.
...#..#....#........#.......................#.......#........#....#.........#..............#...#..#.#.......##...........#.#.#..#..
................#......#......................#...#...............#.#.....#......#..........#.##.#.........#....#...#......#..#....
..............##.#..#...............#.#...........#..#......#......#..#.#..#.................#.....................#.........#.#...
..#................#..#..#..........#........#.#.........#....#...........#...#.......#.#.......#......................#..#......#.
......#............#...##..........#...#..................#.....#............##............................#...#..#........#....#..
..................#..................................###.#..........#....##......#....#.................#..........#..#............
...#..#.......#......#........................#...#......#..................#.#.#.#.#.....#...................#..#...#......##...#.
....................#.#......#.......#......#....#.......#.#.......#.............#...#..#.........................#................
.........#................#.#...........##......#..#..#..................#....#......#......#.......#.#......#.........#.....#.....
..#..........#...........#.........................#.......#..........#....#...........#.#..............................#.....##...
.....#.#..#......#....#....#.............................##.....#...#......#..#..#...#..............#..........#.................#.
.............##....##.......................#.#............#................#....#.......#...................#.....................
..#.......#.........#......................................................#..#..#..............#..#.##............................
......#.......#....#............................................#.#...................##...............##......#.......#.........#.
..............#..#..............#...........#...............................#...................................#....#......#.#....
.........#....#......#.....#.................#..............#.................................................#........##..........
......#...##..............#....#.#............#..#....#...........#.........#..........................#......#.#..........##.#....
...#....#.....#........##...#...................#.#....#...........................#...........................#..........#........
.#..........#.#.....#............#.#................#...#.#..#...............#....#..........#.........#............#.......#.#....
.........#......#.#.#.......#....#...#.#......................#........#..#...#...................#.......#.........#............#.
..........................#.....#.................................#.#.....#.................................#.#...#.###....#.......
...#.................#..#.................#..............#........#.......#.#.............#................#...#..#...........#....
....#..#....................#...............................#.#....#...........................#..#.....##..#................##..#.
....#...#...#..##........##......#...#......#................#..#.......................#....#........#...#.......#..........#.....
............#.....#................#.........#.............#...#...#.#................#...........#......#..#.......#........#.#...
......#...#...........#....#..#.#..............................#......................................................#............
................#.....#.....#....#.......#..#...#..........#.#......................#..#...#.....#...........#.............#..#....
..............#.....#.......#.#.............#.................#.........##.............#......#..............#.....................
...#......#.##...............#..........#...#....#............##................#........#.#......#.#.#............................
.#....#......................#...............###.....................#..................#..#..........#...........#.....#..........
............#..#.......#....#...........#...##..#...........##....#.............#..#.#....#............#..#.......#.#.#.....#......
......#.....##.##.#.....#.#...........#.#.#...................#..................#..#..#..#........#........................#.#....
....#.........#........................#..#....##.................#............#.......#..........................##.#.....#.......
..#..#....................#..........#..........#...........................#..............#....................#..................
..............#...#...............#...........##................#..........#........#.#....#......##.......#....#.......#...##.....
......#...............#........#...#...........#.....#......................#............#.............#..........#...........##...
..#..........#.#.#........#.#..#.#..##...........#.........................#........#.#....#.#........#.........#........#......##.
..........#.#.#...#...#.#...............................#..............#....#......##..........#.#..........#...#....#.............
.....................#............#......#.#..#....#..#..#..............#....#.#........................##......#............#.....
...................................................................................................................................
)";

TEST(Aoc21Tests, garden_doTheThing) {
	ASSERT_EQ(3773, doTheThing(garden, {65, 65}, 64));
}

//TEST(Aoc21Tests, garden_doTheThing_100) {
//	ASSERT_EQ(0, doTheThing(garden, { 65, 65 }, 100));
//}
//
//TEST(Aoc21Tests, garden_doTheThing_200) {
//	ASSERT_EQ(0, doTheThing(garden, { 65, 65 }, 200));
//}

// start with higher numbers, anything other than certain matching map widths will be funky
// 26501365 has that telling "65" at the end.
// 26501300/131 is 202300 - so we go 65 steps and then 202300 map heights/widths.
TEST(Aoc21Tests, garden_2widths_doTheThing) {
	ASSERT_EQ(0, doTheThing(garden, { 65, 65 }, 131*2+65));
}

TEST(Aoc21Tests, garden_4widths_doTheThing) {
	ASSERT_EQ(0, doTheThing(garden, { 65, 65 }, 131 * 4 +65));
}

TEST(Aoc21Tests, garden_8widths_doTheThing) {
	ASSERT_EQ(0, doTheThing(garden, { 65, 65 }, 131 * 8 +65));
}

TEST(Aoc21Tests, garden_16widths_doTheThing) {
	ASSERT_EQ(0, doTheThing(garden, { 65, 65 }, 131 * 16 +65));
}

TEST(Aoc21Tests, garden_32widths_doTheThing) {

	ASSERT_EQ(0, doTheThing(garden, { 65, 65 }, 131 * 32+65));
}

// see spread sheet https://1drv.ms/x/s!At-FVBW8gf7IhYcN_PYyfo7YivMv7w?e=4UzrxQ for final calculations
// 625628021226272 too low
// 625628021226275 too low


