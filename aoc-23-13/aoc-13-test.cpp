#include <gtest/gtest.h>

#include <optional>

#include "pex.h"


using namespace pex;

using Row = std::vector<bool>;
using Map = std::vector<Row>;
using Maps = std::vector<Map>;

Maps parseInput(const std::string& input) {
	auto lines = pSplit(input, '\n');
	Maps maps;
	Map curmap;
	for (auto& line : lines) {		
		if (line.empty()) {
			maps.emplace_back(curmap);
			curmap = {};
		}
		else {
			Row row;
			for (auto& cell : line) {
				row.emplace_back(cell == '#' ? true : false);
			}
			curmap.emplace_back(row);
		}
	}
	if (!curmap.empty()) {
		maps.emplace_back(curmap);
	}
	return maps;
}

bool rowsMatch(const Map& map, int rowA, int rowB) {
	assert(map[rowA].size() == map[rowB].size());
	for (int col = 0; col < map[rowA].size(); col++) {
		if (map[rowA][col] != map[rowB][col]) {
			return false;
		}
	}
	return true;
}

bool colsMatch(const Map& map, int colA, int colB) {
	for (int row = 0; row < map.size(); row++) {
		if (map[row][colA] != map[row][colB]) {
			return false;
		}
	}
	return true;
}

std::vector<int> findReflection(const Map& map, const int64_t bound, const std::function<bool(const Map&, int, int)> matchingFn) {
	std::vector<int> result;
	for (int slice = 1; slice < bound; slice++) {
		bool doRowsMatch = true;
			for (int rowA = slice - 1, rowB = slice;
				rowA >= 0 && rowB < bound;
				rowA--, rowB++) {
			if (!matchingFn(map, rowA, rowB)) {
				doRowsMatch = false;
				break;
			}
		}
		if (doRowsMatch) {
			result.emplace_back(slice);
		}
	}
	return result;
}

// finding a vertical reflection for me means finding the horizontal line across which it reflects vertically,
// the opposite of what the puzzle talks about
std::vector<int> findVerticalReflection(const Map& map) {
	return findReflection(map, std::ssize(map), rowsMatch);
}

std::vector<int> findHorizontalReflection(const Map& map) {
	return findReflection(map, std::ssize(map[0]), colsMatch);
}

std::optional<int> findReflectionValue(const Map& map) {
	auto result = findVerticalReflection(map);
	if( !result.empty()) {
		return result[0] * 100;
	}
	auto resultH = findHorizontalReflection(map);
	if (!resultH.empty()) {
		return resultH[0];
	}
	return std::nullopt;
}

int doTheThingX(const std::string& input, const std::function<std::optional<int>(const Map& map)> evaluatorFn) {
	auto maps = parseInput(input);
	const int sum = pAccumulate(maps, 0, [&evaluatorFn](const int sum, const Map& map) {
		return sum + evaluatorFn(map).value();
		});
	return sum;
}

int findSmudgedReflectionValue(const Map& map) {
	std::vector<int> oldHorizontal = findHorizontalReflection(map);
	assert(oldHorizontal.size() <= 1);
	std::vector<int> oldVertical = findVerticalReflection(map);
	assert(oldVertical.size() <= 1);
	auto oldReflectionValue = findReflectionValue(map);
	for (int y = 0; y < map.size(); y++) {
		for (int x = 0; x < map[y].size(); x++) {
			Map mapCopy = map;
			mapCopy[y][x] = !mapCopy[y][x];
			const auto newHorizontal = findHorizontalReflection(mapCopy);
			const auto newVertical = findVerticalReflection(mapCopy);
			if (!newHorizontal.empty()) {
				if (oldHorizontal.empty()) {
					return newHorizontal[0];
				}
				for (const auto& horizontal : newHorizontal) {
					if (horizontal != oldHorizontal[0]) {
						return horizontal;
					}
				}
			}
			if (!newVertical.empty()) {
				if (oldVertical.empty()) {
					return newVertical[0]*100;
				}
				for (const auto& vertical : newVertical) {
					if (vertical != oldVertical[0]) {
						return vertical*100;
					}
				}
			}
		}
	}
	assert(false);
	return 0;
}

int doTheThing(const std::string& input) {
	return doTheThingX(input, findReflectionValue);
}

int doTheThing2(const std::string& input) {
	return doTheThingX(input, findSmudgedReflectionValue);
}

const std::string sampleData = 
R"(#.##..##.
..#.##.#.
##......#
##......#
..#.##.#.
..##..##.
#.#.##.#.

#...##..#
#....#..#
..##..###
#####.##.
#####.##.
..##..###
#....#..#
)";

TEST(HelloTest, twoEqualRows_findVerticalReflection_middle) {
	Map map = Map{ {true,false,false,true,true},
		{true,false,false,true,true} };

	ASSERT_EQ(1, findVerticalReflection(map)[0]);
}


TEST(HelloTest, fiveRows_findVerticalReflection_slice2) {
	Map map = Map{ {true,false},
		{false,false},
		{false,false},
		{true,false},
		{false,true} };

	ASSERT_EQ(2, findVerticalReflection(map)[0]);
}

TEST(HelloTest, findReflectionTest) {
	Map map = Map{ {true,false,false,true,true},
		{true,false,false,true,true} };

	ASSERT_EQ(2, findHorizontalReflection(map)[0]);
}

TEST(HelloTest, ParseTest) {
	const std::string parseTestInput = 
R"(#.#
.#.

.#
#.
)";

	auto result = parseInput(parseTestInput);

	Maps expected = { {{true,false,true},{false,true,false}},
		{{false,true},{true,false}} };
	ASSERT_EQ(expected, result);
}

TEST(HelloTest, evaluateSampleMaps) {
	auto maps = parseInput(sampleData);
	ASSERT_EQ(5, findReflectionValue(maps[0]));
	ASSERT_EQ(400, findReflectionValue(maps[1]));
}

TEST(HelloTest, sampleInput_doTheThing_405) {
	ASSERT_EQ(405, doTheThing(sampleData));
}

TEST(HelloTest, sampleInput_doTheThing2_400) {
	ASSERT_EQ(400, doTheThing2(sampleData));
}

const std::string puzzleInput =
R"(.#.####
##..#.#
##..#.#
.#.####
..#..#.
####.#.
#.#.#.#
.#..#.#
##.##..
#.#..#.
#.#....

.##.....#....#.
.##.....#....#.
..#.#..##....##
...###...#..#..
#..##.....##...
.#.#...#.####.#
#.#.....######.
##..###...##...
#..#..##..##..#
.##.#.#........
#..#..##..##..#
#####.#.##.###.
.#....##.#..#.#
..##.#...#..#..
#..####..#..#..

.##......##
.##......##
.#.#.##.#.#
#.########.
#.##....##.
#.######.#.
####....###
##.#.##.#.#
###......##
...######..
####.##.###

....#.###
#.#..####
##.......
##.......
#.#..####
....#.###
#..##.#.#
#.##...#.
#.##..#.#
####..#..
#.#.###.#
#.#..##.#
####..#..
#.##..#.#
#.##...#.

##.####.#
.###.####
###...###
###...###
.###.####
#..####.#
..###..#.
##..#..#.
####...#.
#....#.#.
...#.....
#....#.##
#.#.#.##.
#.#.#.##.
#....#.##
...#.....
#....#.#.

.##..#..#..#..#
#....##.##.##.#
......#..######
......#..######
#....##.##.##.#
.##..#..#..#..#
######..#.#.###
.#..#.....#.#..
#...#..#.#.##.#
.###....#..###.
.###....#..###.
#...#..#.#.##.#
.#..#.....#.#..
######..#.#.###
.#...#..#..#..#

.######.#.#..#.
#..##....##..##
#.#.#.###.#..#.
#.#.#.###.#..#.
#..##....##..##
.######...#..#.
#.#.#..#..####.
#.#...#########
.....###.##..##
.#.#....###..##
.#...#...#....#

#.##.#......#.#
#.##.#......#.#
..##.#.......#.
.##.##.##.##...
#.#.##...#.####
##...#.##.#.##.
.#..#.##.#.#..#
.#..#.##.#.#..#
##...#.##.#.##.
#...##...#.####
.##.##.##.##...
..##.#.......#.
#.##.#......#.#

..#..#..#..#.
##.#.####.#.#
####......###
#####.##.####
..#.#.##.#.#.
####..##..###
...#......#..
##.###..###..
...#..##..#..
##.##.##.##.#
##.###..###.#
####......###
..#.##..##.#.
##..........#
##.##....##.#

.#.#....#.#......
...##..##.......#
#..######..#.##..
.###....###.##...
.#.#.##.#.#..#...
..###..###.......
###..##..####.###
#..##..##..##.#..
....#.##....#...#
###.#..#.###.#.#.
#.#......#.##..#.
#.#......#.##..#.
###.#..#.###.#.#.

#.##..##...##..
##.#.#.####..##
##..#..#.#.##.#
##.###.##......
####...####..##
##.#.#.####..##
##.#.#.####..##
####...####..##
##.###.##......
##..#..#.#.##.#
##.#.#.####..##
#.##..##...##..
#...#.#########
.##.###.#.#..#.
......####.####

##..##.
.##....
.##....
##..##.
#.###.#
#.##..#
.######
.######
#.##..#
#.###.#
.#..##.

....#..
##.#.##
..#.#..
....###
##.#.##
##.####
###....
##.##..
....#..
.....##
#####..
##.....
....#.#
####.##
##..#..

.#.##.#.....#.#.#
###..###.#####...
###..###.#####...
.#.##.#.....#.#.#
.#....#..#.###...
#.#..#.#####..##.
#.####.##......#.
##.##.##.##.##..#
###...##.##....##
#.####.#####.####
##.##.##.##......
#########.#.####.
###..####..###...

#..####..#..#..##
###.##.####.#.##.
..######.....#.##
###.##.#####...#.
#.#....#.#.#....#
##.####.###.#.#..
###....###.#.#.##
###.##.####.#####
...#..#....##...#
...#..#....##...#
###.##.####.#####
###....###.#.#.##
##.####.###.###..
#.#....#.#.#....#
###.##.#####...#.

####..##.
####...##
.....###.
...######
######...
....#....
####..#.#
####..#.#
....#....
######...
...####.#
.....###.
####...##

#...#..#.
#...#..#.
####....#
#..######
...######
###......
...##..##
###.####.
#.#....#.

.####..
##.#.#.
##.#.#.
.####..
#..#..#
.....##
.#.#..#
..#.#.#
..#.#.#
.#.#..#
.....##
#..#..#
.#.##..

#.#.#....##..
.####.#.####.
.####.#..###.
#.#.#....##..
..#...###....
..#...###....
#.#.#....##..
.####.#..###.
.####.#.####.
#.#.#....##..
##.#.####...#
..#..#..#.##.
##.#######..#
.##..####...#
###..##....##
.######.##...
.#..#.##..##.

......#
##...#.
.#....#
.#....#
.#...#.
......#
#..###.
...#..#
#.#..##
#.#...#
#..##.#
#..##.#
#.#...#
#.#..##
...#..#
#..###.
......#

##..#.#....
####..#.##.
#.#.#..####
..#....#..#
..#....#..#
#.#.#..####
####..#.##.
##..#.#....
####...###.
#....######
#...###...#
..#####..#.
..##.###.#.
..##.###.#.
..###.#..#.

..........#.###
..######...##.#
.#......#......
#.##..##.#.....
##.####.##.#.##
#...##...###..#
#...##...###..#
##.####.##.#.##
#.##..##.#..#..

..#.......##.#.
#.#.#...####..#
.#..##.#.#.#.##
.#..##.#.#.#.##
#.#.#...####..#
..#.......##.#.
..#..#####....#
.#.#.#......###
.#.###......###

...#.##.#
....#..#.
#..##..##
...######
.#.#....#
.#.#....#
...######
#..##..##
....#..#.
#..#.##.#
###..##..

.#.####.#....
#.#....#.#...
###....##.###
#.##..##.#.##
.#.#..#.#.###
#.######.#...
#.##..##.####

##.#.##
#......
#...#.#
####.##
.#.#..#
...####
...####
.#.#..#
####.##
#...#.#
#......
##.#.##
.#..#..
#..#..#
#.....#

#.########.#.
..##....##..#
..##....##..#
#.########.#.
.##..##..##.#
#.##....##.#.
.#.....#..#.#
...#.##.#....
#..........#.
.#...##...#..
.##.#..#.##..

.##..#..#.##.#.
#..#.##...##...
#######..####..
#######..####..
#..#.##...##...
.##..#..#.##.#.
.#.....#.#....#
......###.#..##
.......#.###.#.
####.##..#..#.#
....#.#.##.#..#
######.######..
....#..##.###..

##.#....#..
##.#...##..
.###.##.#.#
##.####....
...##....#.
..#....#.#.
.....##..##
.#..####...
.#..####...
.....##..##
..#....#.#.
...##....#.
##.####....
.###.##.#.#
##.#...##..

##.####.#
.#.####.#
.#.####.#
##..##..#
####..###
##.####.#
#.#....#.

...#..##.
......##.
#...#####
#..#.####
...######
.#.######
##...#..#
###..####
##..#....

###..###.
####.#.#.
..#..##.#
###.#...#
......##.
..###.###
..#...###
##..#.#.#
##.......
..#......
######...
#####...#
##..##.##
###....#.
###.#.###
#####..##
#####.###

..#..###...
..#...##...
##..###.##.
#.#..###..#
.####...##.
#.###.#.###
..##.....##
#.#..##.#.#
######.#.#.
.####..#..#
.##.##.....
..#......##
..#......##

..#...##...#..#
..####..####...
.#.#.#..#.#.#..
###.#.##.#.###.
..##.####.##...
#..###..###..#.
#....####....#.
..#.######.#...
..#.######.#...
#....####....#.
#..###..###..#.
..##.####.##...
###.#.##.#.###.
.#.#.#..#.#.#..
..####..####..#
..#...##...#..#
##.##.##.##.##.

.###..#.#
..#...#.#
##.####..
#..#.####
....##...
....##...
#..#.####
#..####..
..#...#.#
.###..#.#
.###..#.#

#..##..##...##...
........#########
##....###########
#.#..#.#.#.#..#.#
.##..##..##....##
###..############
#########...##...
.#.##.#.#########
..#..#..###....##
###..#####.#..#.#
.#.##.#..######.#
..#..#..#..#..#..
..####..##..##..#
#.####.##########
##.##.##.#.####.#
..####..#..#..#..
########...####..

..#.#....#.#..##.
###.###.##.###..#
##.#......#.##..#
#....####....#..#
.##.#....#.##.##.
.#....##....#.##.
....######.......
#....#..#....#..#
##.#.#..#.#.#####
#.#...##...#.####
#.##.####.##.#..#
#####.##.########
.##..#..#..##.##.
.###......###....
#..#.#..#.#..#..#

#.#.#.#.#.####.
...########..##
..#.###.###..##
..#.#.#.###..##
#...#..#...##..
.##..##########
#..#.....##..##

#.###..##.#......
#.###..##.#......
.#.#...#....#..#.
#.#####..###.....
#####......#.###.
....#.###.#...###
....##.#.#.##..#.
#.##.#...#...##.#
..##..#.#########
#.#..###.#..###.#
#.#..###.#..#####
..##..#.#########
#.##.#...#...##.#
....##.#.#.##..#.
....#.###.#...###
#####......#.###.
#.#####..###.....

......#
....#.#
#..#.#.
.#.####
.#.####
#..#.#.
....#.#
......#
.####.#

....#.##.##..##..
####..#.##.#.##.#
.#.....#.###....#
###.####.........
#.##.#.#..###..##
##.#.####.#######
####.##.#....##..
#######.#....##..
##.#.####.#######
#.##.#.#..###..##
###.####.........

###..#...##...#..
..#.####.##.####.
##.#..#......#..#
..#..#........#..
###.#.########.#.
##.#...######...#
###.############.
##.#.#..####..#.#
##..#.#.####.#.#.
..#...#..##..#...
###.#####..#####.
#.#.###.####.###.
##..###.####.###.

##.##.####..##.##
###..####..#..#..
#.####.##..##.#..
..####..#..#...#.
##.##.##....#...#
#.#..#.####.#.#..
............#...#
#..##..#...#..##.
#......#.##...##.
.#.##.#.###.##..#
##.##.####.####.#
##.##.####.####.#
.#.##.#.###.##..#
#......#.##...##.
#..##..#...##.##.
............#...#
#.#..#.####.#.#..

.##..###.####..#.
.###...####.###..
##...#..##.#.###.
##.##.#.##.##.#.#
##.##...##.##.#.#
#.##...#..##.##..
.###....#####.#..
..####.#..#..###.
..####.#..#..###.
.###....#####.#..
#.##...#..##.##..
##.##...##.##.#.#
##.##.#.##.##.#.#

##.##..###.
##.##.#..#.
......#...#
......#...#
##.##.#..#.
##.##..###.
###....#.#.
#.#..#..#..
#.##...#.#.
..####.###.
..####.##..
#.##...#.#.
#.#..#..#..
###....#.#.
##.##..###.
##.##.#..#.
......#...#

###.############.
...#.#.######.#.#
##.......##......
##.....#....#....
####....####....#
..##.##.#..#.##.#
...######..######
####.##..##..##.#
#...##..#..#..##.
..#.#.#..##..#.#.
.....#.#....#.#..
#####..#.##.#..##
...#..##....##..#
####...#.##.#...#
###.#..#....#..#.

....##..#.#..
....#...#####
....#...#.###
.##..#...#...
....###.####.
#..##.#.##...
####..#####..
#####..#.##..
####...##....
#####.#...###
#..###.####..
.##.####..###
####.###..###
....#...#....
.##...#######

...######
#.#..##..
.#.######
#...#####
..#######
#####..##
#.#.####.
.#...##..
.#..#..#.
#...#..#.
.###.##.#
..##.##.#
..##.##.#
.###.##.#
#...#..#.

##...#.
##...#.
.######
###...#
.#.#.#.
...#.#.
#..#.##
#..#.##
...#.#.
.#.#.#.
##....#
.######
##...#.
)"
R"(
##..#..##
##.#.....
##...##..
#.###....
###..#.##
#####.###
##.####..
#####.###
....#.###
..##...##
..####...
..###....
...######
#######..
###..#...

####....#####
#####...#.##.
......#.##..#
....####.####
....##...#..#
#..#.#....#..
.....#...#..#
########.....
#######.#.##.
#..##..#.#..#
#..##.....##.
.....##.#.##.
.##.#.#.#.##.
########.####
#..#...#..##.

.#####.#.##
.#####.#.##
...#......#
.#.#..###.#
###.###.#.#
###..#...##
##.#####...
##..##..#.#
..##.####.#
#.#.#..####
#.#.#..####
..##.####.#
##..##..#.#
##.#####...
###..#...##
##..###.#.#
.#.#..###.#

##......#####
#..#..#..##.#
#.#.##.#.####
##..##..##..#
..#....#...##
###########..
..######..##.
#.....#..#..#
.##....##..##
###.##.###...
#........#..#
##..##..###..
##..##..###..

#.#..#.#..#.#..
#.##.#.#..#.#.#
#.##.#.#..#.#.#
#.#..#.#..#.#..
...#...#..#...#
...##..#..#..##
..#...#....#...
.##.#.######.#.
.....#......#..
#######....####
#.#.###.##.###.
##...#......#..
#.##.#.####.#.#
#.###..#..#..##
###..##....##..
#.###..#..##.##
#.#.....##.....

...#.#..#.#......
##..#....#..####.
#####....########
##.#......#.####.
#.#...##...#.##.#
##..........####.
#.##########.##.#
.#####..#.###..##
.############..##

..#.##..#
..#.##..#
.###..#..
##.#..##.
#...#.#..
###.#..#.
##.#.####
..#...#..
##.#....#
..###.#..
#.#.###.#
.#.#####.
...#....#
...#....#
.#.#####.
#.#.###.#
..###.#.#

..#...#.#
.#....#.#
#..#####.
..#####..
...##...#
#.##.....
#.##.....
...##...#
#.#####..
#..#####.
.#....#.#
..#...#.#
.#..#.##.
.#..#.##.
..#...#.#

.#.####.##.####
##.....#..#####
#.#.#......####
##.#.#.#..#####
.#.########.##.
#.....##....##.
.#..#....##.##.
#..#.#....##.##
##.###.####.##.
##.##....#.....
#.##.##.####..#
.##..###.#.....
.##..###.#.....

......#..#.
.......#..#
.......#..#
......#....
######.####
#.##.##....
#######.##.
######.#..#
######..#..
#########..
......####.
#.##.###...
######.#.##
##..##.#...
#######....
..##...####
.......##..

.##.#####..#.
##.#..#.#....
.#.###.##.#.#
##.###.....#.
..#..#.###..#
##..#.#....##
##..#.#....##
..#..#.###..#
##.###.....#.
.#.###.##.#.#
##.#..#.#....
.##..####..#.
.##..####..#.

#...#..##.#.#..
#.#..##.####.#.
#.#..##.####.#.
#...#..##.#.#..
.#.#...#..###.#
......#.....###
....##..#.#####
##...####.#.##.
#...#...##..###
...###.##..##.#
...##..##..##.#

###..####..
###..####..
##....#....
.####.###..
###.###.#..
##.###..###
....#.#..##
#.#..#..#..
..#...#####
.#.####....
##......##.

##.##.#.#..
###.#..##..
..##.##.##.
#.#..#.#.##
#.###....#.
#..#.#...##
...##.###.#
...##.###.#
#..#.#....#
#.###....#.
#.#..#.#.##
..##.##.##.
###.#..##..
##.##.#.#..
.###.#..##.
#.####..#.#
#.####..#.#

####....####...
...#....#...#..
..##....##.....
...##..##....##
....####....#..
#...####...#...
.#...##...#.###
.#........#....
.....##........
#..........#...
.#.#.##.#.#.###
##..#..#.###.##
.##......##..##
###.#..#.###.##
#...####...#...
..##....##..###
#.#.####.#.#...

####..#..
....####.
......##.
.##...###
.##...###
......#..
....####.
####..#..
.##.##.#.
.##..#.#.
#######.#
....###.#
######..#

#.##.#..#..
#.##.##.#..
#.##.##.#..
..##....#..
..##...#.#.
#....###.##
..##...##.#
.#..#..##.#
#.##.##...#

.#.########.#..
####.......###.
#.###.##.###.##
..#........#..#
...##....##....
###..#..#..###.
##.########.###
##.########.###
###..#..#..###.

...##.....#
.##..##....
..####..##.
#..##..#.##
#..##..#.#.
###..####..
.#..#.#.###
.##..##.#.#
##....#####
..#..#....#
##....##..#
.#.##.#.###
...##...##.
##....###..
##....###..
...##...##.
.#.##.#.###

#....######.#
#.##.#..#.#.#
.####.#...##.
#######.##...
##..#####.#..
......###...#
......###...#
##..##.##.#..
#######.##...
.####.#...##.
#.##.#..#.#.#
#....######.#
..##..###.#..

..####...##..
.##..##..##..
#.#..#.#....#
##....##....#
#.#..#.##..##
#.####.#.##.#
##.##..#.##.#
########....#
...##...####.

##.....
..#.##.
###.###
.#.####
.###..#
.###..#
.#.####

#....####.###
#....####.###
.#..#...##.##
#.##.#...####
.#..##..#....
.#..#.#...#..
.####...##...
#.##.#.#.#.#.
..##...#####.
##..##.##.##.
#....###.#..#
.#..#.#..##.#
.......#.#...
.#..#.#.####.
......#..#...
##..##..##.#.
.#..#..#.###.

#.#######.#.#..
..#.#...#...#..
#......#...#.##
##..##....###..
.#.##.###.#####
###..#.#..##...
###..#.#...#...
##.#.##.##..###
.####.##..#..#.
..#####...##.##
..#####...##.##
.####.##..#..#.
##.#.##.##..###
###..#.#...#...
###..#.#..##...

##.##.##.###.#..#
##.###.#..##..##.
.###..#.##..###.#
#.#...##.#####..#
..#.#.....#..####
#...#.#..####.##.
#.#...####.#.####
..#.#.#..##.#.##.
....##..##..#####
.#..##...#.###..#
.##.#..#..#.##..#
.##.#..#..#.##..#
.#..##...#.###..#
....##..##..#####
..#.#.#..##.#.##.

####...
#.#####
..#####
####...
#....##
.###.##
.#..#..
###.###
##...##
.###.##
#.#####

..######...
.#......#..
#....#...#.
##......###
.##....##.#
...####...#
.#.#..#.#.#
.#..##..#..
#........#.
#.#.##.#.##
.#.####.#.#
.#.#..#.#..
.#.####.#..
.#.####.#..
.#.#..#.#..

##.#......#.##.
##.#......#.##.
####......#####
###..#..#..###.
...#......#...#
#...#.##.#...#.
##....##....##.
##.###..###.##.
.###......##..#
...########....
###.#....#.###.
..###....###..#
#.#.#.##.#.#.#.

.######.######.##
.#.##.#...##...#.
##....####..####.
.##..##........##
#.#..#.#.####.#.#
##.##.###....###.
.#....#.#....#.#.
.#.##.#.#....#.#.
..#..#..######..#
########..##.####
...##............
#.####.#......#.#
..#..#..######..#

.#.##......
...##......
..#..##.#..
..#.#..###.
....#...#..
#.#..##..##
...##.#.###
..#.###..#.
..#.###..#.
...##.#.###
#.#..##..##
....#...#..
..#.#..###.

#..#.####.#..##
#..###..###..#.
#.#.#.##.#.#.##
##..#.##.#..###
.##.##..#..##..
..#..#..#..#...
##..#....#..###
.##.#....#.##.#
...#..##..#...#
....#....#....#
.....#..#......
..#.#....#.#...
..##......##...
..##......##...
..#.#....#.#...
.....#..#......
....#....#....#

.##..#.
.##..#.
#.#.###
..##...
.#...#.
#####..
#.#.###
#.#.###
#####..
.#...#.
...#...

###..#..###
.....#...##
......#.#..
..#.#...#..
...###.####
##.##...##.
..###...###
##...#.#..#
#.##.##..#.
..#...#.#..
...#.#..#..
...#.#..#..
..#...#.#..

.....##..
....#..#.
.....##..
#..#.##.#
####....#
.##......
#..###.##

.#..##.######
#.#..##.#.##.
#.#..##.#.##.
.#..##.######
.####..#.....
.#.#.#.#.#..#
####...#...#.

#..#...
...#...
.###.#.
..#.##.
..#.#.#
.##.##.
.##.##.
..#.#.#
..#.##.
.###.#.
...#...
#..#...
...###.
#....##
.######

.##...#.#
####....#
...####.#
#..#.####
.##.##.##
#####.#.#
....#####
.##.##.#.
.##..##.#
#..#.####
.##...##.
#..#..###
#..#..###

..#...#
..#...#
.######
.##.#.#
.#.#.#.
#...##.
......#
#...#.#
#...#.#
......#
#...##.
.#.#.#.
.##...#
.######
..#...#

###.##.
..##...
.#.#..#
.#.#...
..##...
###.##.
###...#
.#.....
#...#.#
...##.#
...##.#
#...#.#
.#.....
###...#
###.##.

....#..#.
##.######
##.#....#
..###..##
..#..##.#
.....##..
##.#.##.#
..##.##.#
###..##..

###..##.##.#.
###..##.##.##
######.#.#.##
####.#..#.#.#
...#.#.##.###
##..##.#.....
....##.#..##.
........#.#..
...#....#.##.
###.####.....
...##...#.###
####..#.###..
....#..#..##.
##.####...#.#
...#.......#.

..#.#..
..#####
..#####
..#.#..
##..#..
...#.##
.######
.###..#
.####.#
.######
...#.##

.####..####.#..##
##..#..#..###.#..
#...#..#...#.#.##
.#.##..##.#.##..#
#..#.##.#..#.....
.#..#..#..#......
.#.######.#..#...
##.##..##.##.#...
.###....###.#####

..###..#.##..
..###..#.#...
...#.#..#####
#####.###.#.#
...#.#..#.#.#
#####..###.#.
##.....##.#..

....##.
.#.####
.######
.#.#..#
#.##..#
...####
.######
#...##.
##..##.
#...##.
#...##.
.######
...####

##....##.
##...##..
##.####.#
..##..##.
..##..##.
##.##.#.#
##...##..
##....##.
....##.##
..#...###
...##.##.
...#...##
#####..#.

##.#.#.
#..#.#.
..#...#
##.#...
...#...
.###...
.#.###.
.#..#..
.#..#..
.#.###.
.###...
...#...
##.#...
..#...#
#..#.#.

.###....#
#..##..##
#.#######
#.#......
##...##..
..##....#
#....##..
###......
#.#.####.
....####.
#####..##
.##......
..##....#
####.##.#
.###.##.#

.##.##.##..##.##.
.##.#.########.#.
.##.####.##.####.
#..######..######
.......##..##....
..#..##########..
....#.##.##.##.#.
#..#.##.####.##.#
####..#......#..#

#..####..##..
.#..##..#..#.
.#......#..#.
..##..##....#
...#..#......
#..#..#..##..
.#......#..#.
#........##..
...####......
.###..###..##
...#..#......
#..####..##..
..#....#....#
#........##.#
.###..###..##

#.#..##..#.#.....
#.#..##..#.#.##..
...#.##.#...#..#.
.#........####.##
#.##.##.##.#...##
..#.####.#.....#.
####.##.######.##
###.#..#.###..###
..##.##.##...#..#
..##.##.##...#..#
###.#..#.###..###
)";

TEST(HelloTest, puzzleInput_doTheThing) {
	ASSERT_EQ(36041, doTheThing(puzzleInput));
}


TEST(HelloTest, puzzleInput_doTheThing2) {
	ASSERT_EQ(0, doTheThing2(puzzleInput));
}