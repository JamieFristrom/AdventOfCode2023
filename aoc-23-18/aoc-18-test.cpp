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

// fills within existing 1-border in a map where 0 = empty and 1 = fill
// must start in empty square
template<typename IntType>
void floodFill(
	std::vector<std::vector<IntType>>& map,
	int64_t startX,
	int64_t startY) {
	std::queue<std::pair<int64_t, int64_t>> fillPoints;
	fillPoints.push({ startX,startY });
	for (;!fillPoints.empty();) {
		auto next = fillPoints.front();
		fillPoints.pop();
		int64_t x = next.first;
		int64_t y = next.second;
		if (map[y][x] == 0) {
			map[y][x] = 1;
			fillPoints.push({ x - 1, y });
			fillPoints.push({ x + 1, y });
			fillPoints.push({ x, y - 1 });
			fillPoints.push({ x, y + 1 });
		}
	}
}

void outGrid(const std::vector < std::vector<int8_t>>& grid) {
	for (const auto& row : grid) {
		for (const auto& cell : row) {
			std::cout << char(cell + '0');
		}
		std::cout << std::endl;
	}
}

struct DigInstruction {
	char dir;
	int64_t meters;
	uint32_t rgb;
};

constexpr int64_t gridRadius = 512;

struct Mesh {
	std::vector<int64_t> horizontalPoints;
	std::vector<int64_t> verticalPoints;
	std::vector<std::vector<int8_t>> grid;
	std::size_t idxForX(const int64_t x) const {
		std::size_t result = std::distance(horizontalPoints.begin(), 
			std::lower_bound(horizontalPoints.begin(), horizontalPoints.end(), x));
		assert(horizontalPoints[result] == x);  // the way I've designed this, should be true
		return result;
	}
	std::size_t idxForY(const int64_t y) const {
		std::size_t result = std::distance(verticalPoints.begin(),
			std::lower_bound(verticalPoints.begin(), verticalPoints.end(), y));
		assert(verticalPoints[result] == y);
		return result;
	}
	int64_t cellSize(const std::size_t x, const std::size_t y) const {
		int64_t left = horizontalPoints[x];
		int64_t right = horizontalPoints[x + 1];
		int64_t top = verticalPoints[y];
		int64_t bottom = verticalPoints[y + 1];
		return (right - left) * (bottom - top);
	}
};

Mesh createMesh(std::vector<DigInstruction> steps) {
	std::set<int64_t> horizontalPointSet;
	std::set<int64_t> verticalPointSet;
	Mesh mesh;
	int64_t x = 0;
	int64_t y = 0;
	for (const auto& step : steps) {
		horizontalPointSet.insert(x);
		horizontalPointSet.insert(x + 1);
		verticalPointSet.insert(y);
		verticalPointSet.insert(y + 1);
		switch (step.dir) {
		case 'U': y -= step.meters; break;
		case 'D': y += step.meters; break;
		case 'L': x -= step.meters; break;
		case 'R': x += step.meters; break;
		}
	}
	for (const auto& point : horizontalPointSet) {
		std::cout << point << " ";
	}
	std::cout << std::endl;
	for (const auto& point : verticalPointSet) {
		std::cout << point << " ";
	}
	std::cout << std::endl;
	mesh.horizontalPoints = std::vector(horizontalPointSet.begin(), horizontalPointSet.end());
	mesh.verticalPoints = std::vector(verticalPointSet.begin(), verticalPointSet.end());
	mesh.grid.resize(mesh.verticalPoints.size());
	for (auto& row : mesh.grid) {
		row.resize(mesh.horizontalPoints.size());
	}
	return mesh;
}

void digMeshBorder(Mesh& mesh, std::vector<DigInstruction> steps) {
	int64_t x = 0;
	int64_t y = 0;
	for (const auto& step : steps) {
		std::size_t xIdx = mesh.idxForX(x);
		std::size_t yIdx = mesh.idxForY(y);
		switch (step.dir) {
		case 'U':
			y -= step.meters;
			for (;mesh.verticalPoints[yIdx] > y;yIdx--) {
				mesh.grid[yIdx][xIdx] = 1;
			}
			break;
		case 'D':
			y += step.meters;
			for (; mesh.verticalPoints[yIdx] < y; yIdx++) {
				mesh.grid[yIdx][xIdx] = 1;
			}
			break;
		case 'L':
			x -= step.meters;
			for (; mesh.horizontalPoints[xIdx] > x; xIdx--) {
				mesh.grid[yIdx][xIdx] = 1;
			}
			break;
		case 'R':
			x += step.meters;
			for (; mesh.horizontalPoints[xIdx] < x; xIdx++) {
				mesh.grid[yIdx][xIdx] = 1;
			}
			break;
		}
	}
}
//struct BP {
//	enum class Dir {
//		Horiz,
//		Vert
//	};
//	enum class Side {
//		Inside,
//		Outside
//	};
//	Dir dir;
//	int x;
//	int y;
//	int len;
//	std::variant<Side, std::unique_ptr<BP>> positiveLeaf;
//	std::variant<Side, std::unique_ptr<BP>> negativeLeaf;
//
//	void refine(std::vector<DigInstruction>::iterator nextStep, bool insideRight) {
//		len = nextStep->meters;
//		int newx = x;
//		int newy = y;
//		if (nextStep->dir == 'R') {
//			newx = x + len;
//			
//		}
//		else if (nextStep->dir == 'L') {
//			newx = x - len;
//		}
//		else if (nextStep->dir == 'U') {
//			newy = y - len;
//		}
//		else if (nextStep->dir == 'D') {
//			newy = y + len;
//		}
//	}
//};


// The digger starts in a 1 meter cube hole in the ground. 
// They then dig the specified number of meters up (U), down (D), left (L), or right (R), 
// clearing full 1 meter cubes as they go. 
// The directions are given as seen from above, so if "up" were north, then "right" would be east, and so on. 
// Each trench is also listed with the color that the edge of the trench should be painted as an RGB hexadecimal color code.
std::vector<std::vector<int8_t>> digBorder(const std::vector<DigInstruction>& input) {
	std::vector<std::vector<int8_t>> ground(gridRadius*2);
	for (auto& row : ground) {
		row.resize(gridRadius*2);
	}
	int64_t x = gridRadius;
	int64_t y = gridRadius;
	for (const auto& instruction : input) {
		for (int64_t i = 0; i < instruction.meters; i++) {
			ground[y][x] = 1;
			switch (instruction.dir) {
			case 'U': y--; break;
			case 'D': y++; break;
			case 'L': x--; break;
			case 'R': x++; break;
			}
		}
	}
	return ground;
}

std::tuple<int64_t, int64_t, int64_t, int64_t> measureBounds(const std::vector<DigInstruction>& instructions) {
	int64_t left = 0;
	int64_t right = 0;
	int64_t top = 0;
	int64_t bottom = 0;
	int64_t x = 0; 
	int64_t y = 0;
	char lastDir = 'X';
	for (const auto& instruction : instructions) {
		left = std::min(x, left);
		right = std::max(x, right);
		top = std::min(y, top);
		bottom = std::max(y, bottom);
		assert(lastDir != instruction.dir);
		lastDir = instruction.dir;
		switch(instruction.dir) {
			case 'U': y-=instruction.meters; break;
			case 'D': y+=instruction.meters; break;
			case 'L': x-=instruction.meters; break;
			case 'R': x+=instruction.meters; break;
		}
	}
	return { left,right,top,bottom };
}

int64_t countHoles(const std::vector<std::vector<int8_t>> ground) {
	const int64_t result = pAccumulate(ground, 0ll, [](const int64_t k, const auto& row) {
		return k + pAccumulate(row, 0ll, [](const int64_t k, const auto& cell) {
			return k + cell;
			});
		});
	return result;
}

int64_t countMeshHoles(const Mesh& mesh) {
	int64_t sum = 0;
	for (std::size_t row = 0; row < mesh.verticalPoints.size(); row++) {
		for (std::size_t col = 0; col < mesh.horizontalPoints.size(); col++) {
			if (mesh.grid[row][col]) {
				sum += mesh.cellSize(col, row);
			}
		}
	}
	return sum;
}

std::vector<std::vector<int8_t>> fillBorder(const std::vector<std::vector<int8_t>>& ground, 
	const int64_t startX,
	const int64_t startY) {
	std::vector<std::vector<int8_t>> groundCopy = ground;
	floodFill(groundCopy, startX, startY);
	return groundCopy;
}

int64_t doTheThing(const std::vector<DigInstruction>& input) {
	Mesh mesh = createMesh(input);
	digMeshBorder(mesh, input);
	std::size_t meshX = mesh.idxForX(0);
	std::size_t meshY = mesh.idxForY(0);
	floodFill(mesh.grid, int64_t(meshX+1), int64_t(meshY+1));
	int64_t resultMethod1 = countMeshHoles(mesh);

	const auto ground = digBorder(input);
	const auto filledGround = fillBorder(ground, gridRadius+1, gridRadius+1);
	int64_t resultMethod2 = countHoles(filledGround);
	assert(resultMethod1 == resultMethod2);
	return resultMethod1;
}

std::vector<DigInstruction> transformInstructions(const std::vector<DigInstruction>& input) {
	std::vector<DigInstruction> newInstructions = pTransform<std::vector<DigInstruction>>(input, [](const auto& inst) {
		const auto dir = inst.rgb & 0xf;
		const auto result = DigInstruction{ 
			(dir == 0) ? 'R' :
			(dir == 1) ? 'D' :
			(dir == 2) ? 'L' :
			(dir == 3) ? 'U' : 'X',
			static_cast<int64_t>(inst.rgb >> 4),
			0 };
		assert(result.dir != 'X');
		return result;
		});
	return newInstructions;
}

int64_t doTheThing2(const std::vector<DigInstruction>& input) {
	auto newInstructions = transformInstructions(input);
	Mesh mesh = createMesh(newInstructions);
	digMeshBorder(mesh, newInstructions);
	outGrid(mesh.grid);
	std::size_t meshX = mesh.idxForX(0);
	std::size_t meshY = mesh.idxForY(0);
	floodFill(mesh.grid, int64_t(meshX + 1), int64_t(meshY + 1));

	outGrid(mesh.grid);
	int64_t resultMethod1 = countMeshHoles(mesh);
	return resultMethod1;
}

std::vector<DigInstruction> sampleInput = {
{'R', 6, 0x70c710},
{'D', 5, 0x0dc571},
{'L', 2, 0x5713f0},
{'D', 2, 0xd2c081},
{'R', 2, 0x59c680},
{'D', 2, 0x411b91},
{'L', 5, 0x8ceee2},
{'U', 2, 0xcaa173},
{'L', 1, 0x1b58a2},
{'U', 2, 0xcaa171},
{'R', 2, 0x7807d2},
{'U', 3, 0xa77fa3},
{'L', 2, 0x015232},
{'U', 2, 0x7a21e3},
};

TEST(Aoc18Test, sampleInput_digBorder_38) {
	const auto ground = digBorder(sampleInput);
	const auto count = countHoles(ground);

	ASSERT_EQ(38, count);
}
TEST(Aoc18Test, sampleInput_doTheThing1_62) {
	ASSERT_EQ(62, doTheThing(sampleInput));
}


TEST(Aoc18Test, sampleInput_doTheThing2) {
	ASSERT_EQ(952408144115, doTheThing2(sampleInput));
}
TEST(Aoc18Test, TestTest) {
	ASSERT_EQ(1, 1);
}


std::vector<DigInstruction> puzzleInput = {
{'R', 9, 0x0a7ad2},
{'U', 3, 0x0b23e1},
{'R', 9, 0x6c6182},
{'U', 5, 0x949661},
{'R', 2, 0x6c6180},
{'U', 3, 0x44d691},
{'R', 7, 0x71da52},
{'U', 3, 0x03dd73},
{'R', 8, 0x0d7ce2},
{'U', 4, 0x4305d3},
{'L', 8, 0x5f36e2},
{'U', 7, 0x7dab93},
{'L', 7, 0x2e7ac2},
{'U', 6, 0x2ea391},
{'R', 4, 0x442ec2},
{'U', 8, 0x565441},
{'L', 4, 0x442ec0},
{'U', 3, 0x3bb991},
{'L', 5, 0x52e6c2},
{'D', 5, 0x03dd71},
{'L', 6, 0x567402},
{'D', 8, 0x503b81},
{'R', 6, 0x75a860},
{'D', 6, 0x7f2f71},
{'L', 2, 0x3cb9d0},
{'D', 5, 0x531401},
{'L', 4, 0x92b290},
{'U', 8, 0x531403},
{'L', 3, 0x714ed0},
{'U', 4, 0x7278b1},
{'L', 3, 0x985d92},
{'D', 10, 0x0e2ae1},
{'L', 2, 0x383f42},
{'D', 3, 0x2fc341},
{'L', 3, 0x2dbf02},
{'U', 9, 0x6ab2a1},
{'L', 4, 0x15a422},
{'U', 4, 0x4fedd1},
{'L', 3, 0x715bc2},
{'U', 3, 0x635721},
{'L', 3, 0x320aa2},
{'U', 3, 0x87e763},
{'L', 8, 0x453b52},
{'U', 4, 0x87e761},
{'R', 8, 0x41f042},
{'U', 3, 0x11ba03},
{'R', 4, 0x4c3372},
{'U', 9, 0x2e4f63},
{'R', 5, 0x0a59d2},
{'U', 5, 0x138663},
{'L', 10, 0x02e272},
{'U', 2, 0x6648c3},
{'L', 7, 0x8dab52},
{'U', 5, 0x1b16f3},
{'L', 6, 0x083552},
{'D', 7, 0x490823},
{'L', 8, 0x089522},
{'D', 3, 0x2fc343},
{'R', 8, 0x2523c2},
{'D', 5, 0x638003},
{'L', 3, 0x4b1fd2},
{'D', 4, 0x69a7a3},
{'R', 5, 0x3e5fc2},
{'D', 3, 0x44a7f3},
{'L', 5, 0x583992},
{'D', 4, 0x6df291},
{'L', 5, 0x094eb2},
{'D', 2, 0x02c911},
{'L', 7, 0x7f49f2},
{'D', 2, 0x02c913},
{'L', 2, 0x363e62},
{'D', 3, 0x2856e1},
{'R', 10, 0x0447c0},
{'D', 6, 0x0a2e21},
{'L', 10, 0x8e0b00},
{'D', 3, 0x6feb51},
{'L', 5, 0x15aa60},
{'D', 3, 0x0089e1},
{'L', 5, 0x16d9e0},
{'D', 3, 0x00e2d1},
{'L', 3, 0x79f0d2},
{'D', 5, 0x566b51},
{'R', 4, 0x6e5770},
{'D', 2, 0x3e43d1},
{'L', 4, 0x094fe0},
{'D', 4, 0x5d96f1},
{'L', 5, 0x698f20},
{'U', 4, 0x2e8521},
{'L', 3, 0x16e4b0},
{'U', 2, 0x01fd31},
{'L', 9, 0x5ec470},
{'U', 3, 0x301bd1},
{'R', 5, 0x08eb40},
{'U', 2, 0x829ba1},
{'R', 7, 0x695a40},
{'U', 3, 0x116d91},
{'L', 4, 0x608360},
{'U', 4, 0x054803},
{'L', 3, 0x4f2220},
{'U', 6, 0x054801},
{'L', 5, 0x19e1f0},
{'U', 4, 0x01f163},
{'L', 6, 0x73d2b0},
{'U', 10, 0x44d011},
{'L', 5, 0x5ab060},
{'U', 6, 0x44d013},
{'L', 6, 0x26b720},
{'U', 4, 0x10b273},
{'R', 8, 0x2337d0},
{'U', 7, 0x765673},
{'L', 8, 0x1feb20},
{'U', 4, 0x6bb9a1},
{'L', 5, 0x8b9490},
{'U', 3, 0x1b4f41},
{'L', 4, 0x26d790},
{'U', 3, 0x01f161},
{'L', 6, 0x39cf40},
{'U', 10, 0x4f5ed1},
{'L', 3, 0x209c40},
{'U', 5, 0x163fd1},
{'R', 3, 0x053302},
{'U', 2, 0x80e4d1},
{'R', 10, 0x799072},
{'U', 4, 0x247461},
{'R', 9, 0x7ec370},
{'D', 4, 0x2b79f1},
{'R', 4, 0x209c42},
{'D', 6, 0x0f69e1},
{'R', 2, 0x53cf92},
{'D', 3, 0x528351},
{'R', 4, 0x0bd6c0},
{'D', 2, 0x585001},
{'R', 7, 0x0bd6c2},
{'D', 5, 0x8dc481},
{'R', 3, 0x68ca02},
{'D', 10, 0x01cde1},
{'R', 3, 0x64c482},
{'D', 3, 0x725de3},
{'R', 4, 0x2ebf62},
{'D', 3, 0x8d71c3},
{'R', 4, 0x3815d2},
{'D', 6, 0x096853},
{'R', 4, 0x637652},
{'U', 6, 0x6eef73},
{'R', 3, 0x224b62},
{'U', 4, 0x4b71f3},
{'R', 3, 0x3320a2},
{'U', 3, 0x344e71},
{'L', 11, 0x7f2192},
{'U', 4, 0x7603b1},
{'R', 11, 0x7f2190},
{'U', 5, 0x053781},
{'R', 4, 0x68b582},
{'D', 7, 0x5d2a11},
{'R', 4, 0x68b580},
{'U', 7, 0x5846f1},
{'R', 4, 0x01efd2},
{'U', 3, 0x087781},
{'R', 4, 0x01efd0},
{'U', 10, 0x562731},
{'R', 5, 0x419bf2},
{'D', 10, 0x01cde3},
{'R', 4, 0x74bec2},
{'U', 4, 0x8c3643},
{'R', 8, 0x4bb1f2},
{'U', 3, 0x3e9233},
{'R', 7, 0x4bb1f0},
{'U', 7, 0x6dcf63},
{'R', 4, 0x038162},
{'U', 3, 0x1bbbf3},
{'R', 3, 0x565582},
{'U', 4, 0x84e503},
{'R', 7, 0x565580},
{'U', 9, 0x00f4e3},
{'R', 6, 0x018082},
{'U', 5, 0x166073},
{'R', 4, 0x6f6ba2},
{'U', 8, 0x886071},
{'R', 7, 0x3600f2},
{'U', 8, 0x23a411},
{'R', 3, 0x5ca882},
{'U', 5, 0x450111},
{'R', 9, 0x5f0b72},
{'U', 5, 0x450113},
{'L', 2, 0x67d222},
{'U', 3, 0x2ae133},
{'L', 3, 0x223062},
{'U', 5, 0x812353},
{'L', 6, 0x56e5d2},
{'U', 3, 0x893e63},
{'L', 5, 0x191772},
{'U', 3, 0x01fd53},
{'L', 5, 0x0bb5b2},
{'U', 10, 0x3d2d33},
{'L', 4, 0x5ad942},
{'D', 7, 0x85e073},
{'L', 3, 0x106702},
{'D', 6, 0x0813c3},
{'L', 7, 0x7d2d32},
{'U', 2, 0x1451c3},
{'L', 3, 0x20c442},
{'U', 5, 0x1eefe3},
{'L', 3, 0x402a12},
{'U', 2, 0x85bcf3},
{'L', 5, 0x2ba542},
{'U', 4, 0x401223},
{'R', 7, 0x2bc0b2},
{'U', 6, 0x654553},
{'R', 3, 0x92f0c2},
{'U', 5, 0x20b413},
{'R', 6, 0x290370},
{'U', 7, 0x499a53},
{'R', 3, 0x17f140},
{'D', 7, 0x3c7fe3},
{'R', 3, 0x17f142},
{'D', 2, 0x28b743},
{'R', 10, 0x3748a0},
{'D', 3, 0x56e3e1},
{'R', 3, 0x3736b0},
{'U', 6, 0x4200f1},
{'R', 3, 0x3736b2},
{'U', 6, 0x15eca1},
{'R', 3, 0x5e6560},
{'U', 2, 0x484663},
{'R', 5, 0x0c3562},
{'U', 8, 0x953733},
{'R', 2, 0x5bbd22},
{'U', 6, 0x191053},
{'R', 5, 0x5f2532},
{'U', 7, 0x948a03},
{'R', 2, 0x5c9a12},
{'U', 4, 0x2c1b31},
{'R', 8, 0x804f02},
{'U', 4, 0x3f2791},
{'R', 2, 0x804f00},
{'U', 3, 0x425791},
{'L', 10, 0x54b5e2},
{'U', 6, 0x3306f3},
{'L', 7, 0x5600f2},
{'U', 2, 0x549bf3},
{'L', 5, 0x4421c2},
{'U', 8, 0x3fffd3},
{'R', 3, 0x4421c0},
{'U', 8, 0x2eeab3},
{'R', 2, 0x196ec2},
{'U', 4, 0x8375d3},
{'R', 5, 0x08be10},
{'U', 10, 0x227fe3},
{'R', 3, 0x631ba0},
{'D', 10, 0x7267f3},
{'R', 2, 0x6f6ec0},
{'D', 8, 0x678743},
{'R', 4, 0x27e480},
{'D', 4, 0x8994b3},
{'R', 3, 0x04be02},
{'U', 3, 0x477d03},
{'R', 4, 0x1eda82},
{'D', 7, 0x6761b3},
{'R', 3, 0x3ebc32},
{'U', 11, 0x6761b1},
{'R', 2, 0x4d0a32},
{'U', 2, 0x3c95c3},
{'R', 4, 0x53ce12},
{'D', 3, 0x6e01c3},
{'R', 2, 0x219ef0},
{'D', 10, 0x501bc3},
{'R', 4, 0x75b650},
{'D', 4, 0x8b9b33},
{'R', 5, 0x156510},
{'D', 2, 0x4d6c43},
{'R', 4, 0x556360},
{'D', 6, 0x42b123},
{'R', 4, 0x1dda10},
{'D', 6, 0x47f713},
{'L', 2, 0x6e7640},
{'D', 7, 0x839fd3},
{'L', 4, 0x6e7642},
{'D', 3, 0x3de353},
{'L', 4, 0x4dc060},
{'D', 3, 0x4f1aa3},
{'L', 7, 0x98fea0},
{'U', 8, 0x201083},
{'L', 7, 0x1484c0},
{'D', 8, 0x1f6af3},
{'L', 4, 0x8442a0},
{'D', 3, 0x1f6af1},
{'R', 8, 0x2f8420},
{'D', 3, 0x6b1b33},
{'R', 2, 0x487c30},
{'D', 7, 0x466db3},
{'R', 4, 0x63c190},
{'U', 6, 0x3cdff3},
{'R', 3, 0x46bd82},
{'U', 4, 0x4faac3},
{'R', 4, 0x658042},
{'D', 4, 0x757783},
{'R', 3, 0x4e9d80},
{'D', 5, 0x525b83},
{'R', 3, 0x187360},
{'D', 5, 0x00d3b3},
{'R', 5, 0x4c84b0},
{'D', 7, 0x529d91},
{'R', 8, 0x174970},
{'D', 8, 0x529d93},
{'R', 4, 0x5b67b0},
{'D', 2, 0x285493},
{'R', 5, 0x791a70},
{'D', 3, 0x285491},
{'R', 4, 0x5197a0},
{'D', 5, 0x597071},
{'R', 5, 0x78f090},
{'D', 8, 0x546f81},
{'R', 2, 0x1eb280},
{'D', 6, 0x5d7571},
{'R', 3, 0x241f72},
{'D', 6, 0x4f2811},
{'R', 10, 0x436630},
{'D', 3, 0x23a351},
{'R', 8, 0x436632},
{'D', 3, 0x2e37b1},
{'R', 3, 0x3196e2},
{'D', 3, 0xa10313},
{'R', 9, 0x514c52},
{'D', 4, 0x4e9431},
{'R', 3, 0x6046b2},
{'D', 7, 0x96e791},
{'R', 2, 0x6046b0},
{'D', 6, 0x146a51},
{'R', 5, 0x0f4fd0},
{'D', 7, 0x915911},
{'R', 4, 0x6cce90},
{'D', 8, 0x104061},
{'R', 5, 0x2ae440},
{'D', 9, 0x6ce031},
{'R', 2, 0x97a312},
{'D', 9, 0x0df1e1},
{'R', 4, 0x53af70},
{'U', 5, 0x8483d1},
{'R', 4, 0x853c50},
{'U', 6, 0x1720d1},
{'R', 3, 0x51ce30},
{'U', 4, 0x24cb21},
{'R', 3, 0x789642},
{'U', 8, 0x768481},
{'R', 5, 0x789640},
{'U', 2, 0x450ed1},
{'R', 5, 0x7ebe70},
{'U', 3, 0x595d13},
{'L', 7, 0x3b77a2},
{'U', 8, 0x2bb771},
{'R', 7, 0x26f5f2},
{'U', 8, 0x2bb773},
{'R', 5, 0x577a02},
{'D', 3, 0x52c7d3},
{'R', 9, 0x11c420},
{'D', 7, 0x189281},
{'L', 9, 0x8b5790},
{'D', 5, 0x189283},
{'R', 6, 0x1ccbe0},
{'D', 5, 0x227163},
{'L', 5, 0x0b94d0},
{'D', 5, 0x11c833},
{'L', 2, 0x445330},
{'D', 6, 0x517361},
{'R', 4, 0x445060},
{'D', 5, 0x75c231},
{'R', 3, 0x071910},
{'D', 8, 0x169701},
{'R', 5, 0x2fbdc0},
{'D', 8, 0x327e41},
{'L', 5, 0x96e8a0},
{'D', 5, 0x4e2841},
{'L', 4, 0x5dd6d2},
{'D', 6, 0x355c43},
{'L', 8, 0x9466e2},
{'U', 7, 0x355c41},
{'L', 3, 0x4886d2},
{'U', 6, 0x905c81},
{'L', 6, 0x78da50},
{'U', 3, 0x85e9a3},
{'L', 4, 0x631060},
{'D', 5, 0x85e9a1},
{'L', 7, 0x5ed9d0},
{'D', 2, 0x83fd71},
{'L', 2, 0x34fb30},
{'D', 9, 0x895fc3},
{'L', 6, 0x7d37b0},
{'D', 5, 0x4fda83},
{'L', 2, 0x2f8562},
{'D', 5, 0x00e4c3},
{'R', 2, 0x5516f2},
{'D', 3, 0x00e4c1},
{'R', 10, 0x1d1a02},
{'D', 2, 0x201573},
{'R', 6, 0x0f5462},
{'D', 3, 0x459793},
{'L', 6, 0x138400},
{'D', 3, 0x204d01},
{'L', 6, 0x9761f0},
{'D', 5, 0x204d03},
{'L', 4, 0x0624c0},
{'D', 3, 0x408143},
{'L', 3, 0x7d37b2},
{'D', 3, 0x1597f3},
{'L', 4, 0x167030},
{'D', 8, 0x49d863},
{'L', 2, 0x94c2c0},
{'D', 2, 0x49d861},
{'L', 9, 0x1f01e0},
{'D', 4, 0x2b4dc3},
{'L', 2, 0x45c9a0},
{'D', 3, 0x310cf3},
{'L', 5, 0x66c6d0},
{'D', 3, 0x302013},
{'R', 8, 0x90fba0},
{'D', 6, 0x563f13},
{'R', 8, 0x90fba2},
{'D', 6, 0x1ec8a3},
{'R', 2, 0x4f6102},
{'D', 9, 0x7ecac1},
{'R', 4, 0x6f8d92},
{'D', 7, 0x37bfa1},
{'R', 3, 0x172422},
{'D', 3, 0x2086c3},
{'R', 3, 0x67a502},
{'D', 4, 0x9603a3},
{'L', 8, 0x3a4d72},
{'D', 5, 0x58e9c3},
{'R', 8, 0x6db970},
{'D', 4, 0x212483},
{'R', 5, 0x136650},
{'D', 11, 0x348073},
{'R', 4, 0x90fc80},
{'D', 2, 0x425823},
{'R', 3, 0x012b00},
{'D', 2, 0x97fd11},
{'R', 7, 0x64bde0},
{'D', 3, 0x454f83},
{'R', 3, 0x389c10},
{'D', 10, 0x5600c3},
{'R', 5, 0x3e8fd0},
{'D', 5, 0x583a43},
{'R', 2, 0x923702},
{'D', 7, 0x287cc3},
{'R', 7, 0x117392},
{'D', 2, 0x670023},
{'R', 3, 0x149ff0},
{'D', 9, 0x183503},
{'R', 6, 0x8f0aa0},
{'U', 5, 0x5a8393},
{'R', 4, 0x772be2},
{'U', 4, 0x7165f3},
{'R', 5, 0x291ab2},
{'D', 3, 0x3b9a43},
{'R', 3, 0x8375c2},
{'D', 4, 0x4e22a3},
{'L', 4, 0x24a6e0},
{'D', 8, 0x7cc603},
{'L', 5, 0x61b520},
{'D', 9, 0x7cc601},
{'L', 6, 0x301880},
{'D', 2, 0x0032c1},
{'L', 2, 0x32ee20},
{'D', 9, 0x1cdeb1},
{'L', 6, 0x4d2f70},
{'D', 5, 0x331db1},
{'L', 5, 0x60b9a0},
{'D', 3, 0x3f6961},
{'R', 4, 0x11e950},
{'D', 2, 0x028281},
{'R', 9, 0x2df060},
{'D', 5, 0x4c6b51},
{'R', 6, 0x5a87f0},
{'D', 4, 0x7f6c11},
{'R', 4, 0x274b50},
{'D', 3, 0x06a2d1},
{'R', 6, 0x66b940},
{'U', 6, 0x5dde91},
{'R', 3, 0x72deb0},
{'U', 4, 0x2fbbb1},
{'R', 5, 0x897602},
{'U', 2, 0x348e61},
{'R', 7, 0x441bd2},
{'U', 3, 0x7332b1},
{'R', 4, 0x5ab8a0},
{'U', 8, 0x2fe673},
{'L', 4, 0x4036c0},
{'U', 3, 0x2fe671},
{'R', 5, 0x32a270},
{'U', 11, 0x3e8741},
{'L', 5, 0x6ae282},
{'U', 4, 0x41b591},
{'R', 8, 0x37d3b2},
{'D', 8, 0x09f5a1},
{'R', 4, 0x51bf10},
{'D', 2, 0x9810a1},
{'R', 5, 0x51bf12},
{'D', 4, 0x1426e1},
{'L', 7, 0x406a12},
{'D', 3, 0x076043},
{'L', 2, 0x22fbc2},
{'D', 8, 0x2b9963},
{'R', 5, 0x233950},
{'D', 4, 0x645ad3},
{'R', 4, 0x233952},
{'D', 8, 0x1b9843},
{'R', 2, 0x22fbc0},
{'D', 4, 0x034073},
{'R', 6, 0x44b9a2},
{'U', 3, 0x13c341},
{'R', 3, 0x6181b2},
{'U', 11, 0x49de31},
{'R', 5, 0x661882},
{'U', 3, 0x2a4b71},
{'L', 4, 0x292860},
{'U', 8, 0x4b2101},
{'R', 4, 0x292862},
{'U', 3, 0x4601c1},
{'L', 5, 0x661880},
{'U', 7, 0x4ab4c1},
{'L', 8, 0x4b8900},
{'U', 5, 0x5cba01},
{'L', 4, 0x740e40},
{'U', 9, 0x4c1181},
{'R', 7, 0x6ad450},
{'U', 5, 0x1e8f13},
{'R', 10, 0x789d00},
{'U', 5, 0x56d4e3},
{'R', 9, 0x6906d0},
{'D', 4, 0x274cd3},
{'R', 2, 0x6906d2},
{'D', 6, 0x5606b3},
{'R', 4, 0x789d02},
{'D', 7, 0x0f9013},
{'R', 2, 0x941130},
{'D', 5, 0x03ece1},
{'R', 2, 0x16dad0},
{'D', 3, 0x40c361},
{'R', 3, 0x7a97f0},
{'D', 8, 0x48e781},
{'R', 2, 0x9172c2},
{'D', 4, 0x74afc1},
{'R', 4, 0x64e300},
{'D', 5, 0x1563a1},
{'R', 7, 0x643860},
{'D', 3, 0x834d81},
{'R', 4, 0x1cc3b0},
{'D', 8, 0x84faa1},
{'L', 6, 0x1455a0},
{'D', 9, 0x69a921},
{'L', 5, 0x016b10},
{'D', 3, 0x121301},
{'L', 8, 0x8896d0},
{'U', 4, 0x7e3141},
{'L', 6, 0x586070},
{'U', 6, 0x443441},
{'L', 3, 0x6b2500},
{'U', 2, 0x18af21},
{'L', 8, 0x0ba460},
{'D', 3, 0x0759a1},
{'L', 3, 0x70f710},
{'D', 3, 0x95f351},
{'L', 4, 0x3d9420},
{'D', 3, 0x6b0ba1},
{'L', 7, 0x237490},
{'D', 5, 0x4ab631},
{'L', 5, 0x650c80},
{'D', 5, 0x6d55b3},
{'L', 7, 0x09aa90},
{'U', 5, 0x5ff8d1},
{'L', 2, 0x73f7a0},
{'U', 5, 0x3c8631},
{'L', 4, 0x0070d0},
{'D', 8, 0x9c7f03},
{'L', 4, 0x238390},
{'D', 2, 0x6beb03},
{'L', 3, 0x2d0292},
{'D', 6, 0x202e03},
{'L', 3, 0x4e7522},
{'D', 7, 0x202e01},
{'L', 3, 0x261ee2},
{'D', 3, 0x316023},
{'L', 8, 0x8745e0},
{'D', 6, 0x611d13},
{'L', 3, 0x744880},
{'D', 3, 0x466c61},
{'L', 6, 0x550620},
{'D', 4, 0x2c40e1},
{'L', 3, 0x3950f0},
{'D', 3, 0x11c341},
{'L', 3, 0x03cb60},
{'D', 3, 0x658971},
{'L', 10, 0x440f70},
{'D', 2, 0x2d7601},
{'L', 4, 0x3822d0},
{'D', 5, 0x24e871},
{'L', 5, 0x110ab0},
{'D', 2, 0x4e2931},
{'L', 5, 0x8cbc82},
{'D', 5, 0x5fded1},
{'L', 4, 0x255e92},
{'D', 7, 0x101971},
{'L', 4, 0x2c5b40},
{'D', 9, 0x843b71},
{'L', 2, 0x2c5b42},
{'D', 4, 0x291691},
{'L', 5, 0x79bb32},
{'U', 7, 0x516d91},
{'L', 4, 0x703a92},
{'U', 4, 0x0b33c3},
{'L', 4, 0x362eb2},
{'U', 7, 0x223f13},
{'L', 2, 0x081312},
{'U', 7, 0x5994f3},
{'L', 7, 0x8b63c2},
{'U', 6, 0x44b181},
{'L', 7, 0x0846a2},
{'U', 7, 0x425641},
{'L', 8, 0x5a9522},
{'U', 2, 0x089c01},
{'L', 3, 0x6ea4c0},
{'D', 6, 0x73c7a3},
{'L', 8, 0x679440},
{'D', 3, 0x73c7a1},
{'L', 4, 0x12fc40},
{'U', 3, 0x7c8171},
{'L', 4, 0x5b6be2},
{'U', 5, 0x22af01},
{'L', 4, 0x983a90},
{'U', 5, 0x71f411},
{'L', 8, 0x01d330},
{'D', 3, 0x2f6621},
{'L', 3, 0x9a0dc2},
{'D', 3, 0x1a8e21},
{'L', 5, 0x300de2},
{'U', 11, 0x3fdd13},
{'L', 5, 0x457be0},
{'D', 9, 0x797403},
{'L', 4, 0x457be2},
{'D', 2, 0x254643},
{'L', 4, 0x5dbb82},
{'U', 6, 0x7bf431},
{'L', 6, 0x72ae42},
{'D', 3, 0x865831},
{'L', 3, 0x04c812},
{'D', 5, 0x6f17b3},
{'L', 3, 0x014132},
{'D', 5, 0x3d8773},
{'L', 4, 0x0869e2},
{'U', 7, 0x6007f3},
{'L', 5, 0x9534c2},
{'U', 5, 0x535c73},
{'L', 3, 0x516e42},
{'U', 4, 0x5cda51},
{'L', 2, 0x463320},
{'U', 5, 0x32cec1},
{'L', 10, 0x463322},
{'U', 6, 0x7d6161},
{'R', 7, 0x771382},
{'U', 9, 0x52f911},
{'R', 4, 0x1c72e2},
{'U', 5, 0x0b33f1},
{'R', 4, 0x73c102},
{'U', 2, 0x0b33f3},
{'R', 5, 0x1bdac2},
{'U', 7, 0x2bcb73},
{'R', 7, 0x328d92},
{'U', 4, 0x012873},
{'R', 4, 0x1e8c52},
{'U', 4, 0x500831},
{'R', 6, 0x709d42},
{'U', 8, 0x500833},
{'R', 5, 0x1511d2},
{'U', 7, 0x012871},
{'R', 3, 0x1722e2},
{'U', 2, 0x136aa3},
{'R', 5, 0x787732},
{'U', 6, 0x2d2563},
{'L', 4, 0x447100},
{'U', 2, 0x6ec813},
{'L', 6, 0x447102},
{'U', 4, 0x3f6f43},
{'L', 2, 0x4e6870},
{'U', 7, 0x5fbc43},
{'L', 2, 0x2ceaa0},
{'U', 2, 0x6f6a63},
{'L', 7, 0x704390},
{'U', 6, 0x1a14c3},
{'L', 9, 0x0e3f60},
{'U', 3, 0x849b21},
{'L', 5, 0x73a6c0},
{'D', 10, 0x1c7cc1},
{'L', 3, 0x20fc00},
{'U', 10, 0x3657e3},
{'L', 4, 0x0bbcc2},
{'U', 2, 0x22d3d3},
{'L', 5, 0x0bbcc0},
{'U', 4, 0x47ec33},
{'R', 5, 0x46a9c0},
{'U', 4, 0x8f39d3},
{'R', 9, 0x41cea0},
{'U', 5, 0x1fc343},
{'R', 3, 0x3180d2},
{'U', 3, 0x012153},
{'L', 4, 0x5b5e50},
{'U', 8, 0x11dbe3},
{'L', 7, 0x7771d0},
{'U', 6, 0x11dbe1},
{'R', 7, 0x06f310},
{'U', 6, 0x6d4b03},
{'L', 5, 0x0bed90},
{'U', 9, 0x91aac3},
{'L', 5, 0x4a8b62},
{'U', 2, 0x406ca1},
{'L', 3, 0x34e292},
{'U', 2, 0x37b021},
{'L', 3, 0x34e290},
{'U', 3, 0x593ca1},
{'L', 5, 0x437f52},
{'U', 6, 0x520cb3},
{'L', 8, 0x082890},
{'U', 3, 0x594993},
{'L', 6, 0x082892},
{'U', 6, 0x260323},
{'R', 4, 0x57a612},
{'U', 5, 0x1c63d3},
{'R', 4, 0x037fb2},
{'D', 7, 0x129b43},
{'R', 4, 0x5dd9d2},
{'U', 7, 0x057391},
{'R', 5, 0x1baaa2},
{'D', 5, 0x5218a1},
{'R', 3, 0x32ad40},
{'U', 2, 0x1ed2f1},
{'R', 6, 0x32ad42},
{'U', 6, 0x556531},
{'L', 4, 0x1baaa0},
{'U', 3, 0x01bb71},
{'L', 5, 0x35b492},
{'U', 9, 0x4d97d3},
{'L', 7, 0x431fc2},
{'D', 9, 0x7fe7f3},
{'L', 3, 0x097032},
{'U', 6, 0x4f74d1},
{'L', 7, 0x7e4702},
{'U', 6, 0x5f0441},
{'L', 3, 0x3e36b2},
{'U', 5, 0x809d11},
{'L', 5, 0x4554a2},
{'D', 10, 0x252373},
{'L', 7, 0x2741b2},
{'D', 2, 0x1e3303},
{'R', 7, 0x70c402},
{'D', 9, 0x2bc5f3},
{'L', 3, 0x9fb6f2},
{'D', 9, 0x2bc5f1},
{'L', 5, 0x611422},
{'D', 4, 0x420763},
{'L', 3, 0x092f42},
{'D', 3, 0x4861a3},
{'L', 5, 0x9bc3f2},
{'U', 5, 0x25d313},
{'L', 11, 0x32b1e0},
{'U', 5, 0x063e81},
{'R', 11, 0x52a210},
{'U', 3, 0x063e83},
{'L', 2, 0x1f9f40},
{'U', 8, 0x059533},
{'L', 3, 0x5bbba2},
{'U', 4, 0x57d773},
{'L', 7, 0x5ea432},
{'U', 4, 0x686ab3},
};

TEST(Aoc18Test, puzzleInput_doTheThing1_0) {
	ASSERT_EQ(62500, doTheThing(puzzleInput));
}


TEST(Aoc18Test, puzzleInput_doTheThing2_0) {
	ASSERT_EQ(952408144115, doTheThing2(puzzleInput));
}