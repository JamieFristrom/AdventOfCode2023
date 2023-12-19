#pragma once

// pex stands for Pure Ergonomic Extensions
// 
// So you want most of the benefits of ranges but you don't want C++ 23 or care about performance?
// 
// No promises about performance, but these are mostly analogs for things in stl
// but non-mutating (they often create a copy and operate on that, for example with pSort)
// and ergonomic in that they operate on whole containers instead of begin-end blocks

#include <algorithm>
#include <numeric>
#include <string>
#include <vector>

namespace pex {

	template<typename ReturnValue, typename IngestContainer, typename BinaryOperation>
	[[nodiscard]] ReturnValue pAccumulate(const IngestContainer& c, ReturnValue init, BinaryOperation binaryOp) {
		return std::accumulate(c.begin(), c.end(), init, binaryOp);
	}

	// pSum assumes numeric type in container. There must be a way to assert that with type traits...
	template<typename Container>
	[[nodiscard]] Container::value_type pReduce(const Container& c) {
		return std::reduce(c.begin(), c.end());
	}

	template<typename Container, typename UnaryOperation>
	[[nodiscard]] Container::value_type pReduce(const Container& c, UnaryOperation unaryOp) {
		return std::reduce(c.begin(), c.end(), unaryOp);
	}
	// we need to specify ReturnContainer because either the return element type or the 
	// returned container are likely to be different
	template<typename ReturnContainer, typename IngestContainer, typename UnaryOperation>
	[[nodiscard]] ReturnContainer pTransform(const IngestContainer& c, UnaryOperation unaryOp) {
		ReturnContainer newC;
		std::transform(c.begin(), c.end(), std::back_inserter(newC), unaryOp);
		return newC;
	}

	// not to be confused with qsort :P
	template<typename Container>
	[[nodiscard]] Container pSort(const Container& c) {
		Container newC = c;
		std::sort(newC.begin(), newC.end());
		return newC;
	}

	template<typename Container>
	typename Container::value_type pMaxElementValue(const Container& c) {
		return *std::max_element(c.begin(), c.end());
	}

	template<typename Container>
	bool pAllOf(const Container& c, const std::function<bool(typename Container::value_type)> predicate) {
		return std::all_of(c.begin(), c.end(), predicate);
	}

	// you'd proll want std::ranges::contains in C++ 23
	template<typename Container>
	bool pContains(const Container& c, const typename Container::value_type match) {
		return std::find(c.begin(), c.end(), match) != c.end();
	}

	/*template<typename Container>
	bool pAnyOf(const Container& c, ) {
		return std::any_of(c.begin(), c.end());
	}*/

	template<typename Container>
	int64_t pCount(const Container& c, const typename Container::value_type match) {
		return static_cast<int64_t>(std::count(c.begin(), c.end(), match));
	}

	template<typename Container, typename Predicate>
	int64_t pCountIf(const Container& c, Predicate predicate) {
		return static_cast<int64_t>(std::count_if(c.begin(), c.end(), predicate));
	}

	template<typename Container, typename Predicate>
	typename Container::iterator pFindIf(Container& c, Predicate predicate) {
		return std::find_if(c.begin(), c.end(), predicate);
	}

	// remember non-destructive
	//template<typename Container>
	//Container pEraseIf(const Container& c, std::function<bool(typename Container::value_type)> predicate) {
	//	Container containerCopy = c;
	//	containerCopy.erase_if(containerCopy.begin(), containerCopy.end(), predicate);
	//	return containerCopy;
	//}

	template<typename Container>
	Container pConcatenate(const Container& c1, const Container& c2) {
		Container result = c1;
		result.insert(result.end(), c2.begin(), c2.end());
		return result;
	}

	template<typename Container>
	Container pInterleave(const Container& c1, const Container& c2) {
		Container result;
		auto c1it = c1.begin();
		auto c2it = c2.begin();
		for (;;) {
			if (c1it == c1.end() && c2it == c2.end()) break;
			if (c1it != c1.end()) {
				result.emplace_back(*c1it);
				c1it++;
			}
			if (c2it != c2.end()) {
				result.emplace_back(*c2it);
				c2it++;
			}
		}
		return result;
	}

	std::vector<std::string> pSplit(const std::string& str, const char delim) {
		std::stringstream strstream(str);
		std::vector<std::string> result;
		for (std::string line; std::getline(strstream, line, delim);) {
			result.emplace_back(line);
		}
		return result;
	}

	template <typename T> int sgn(T val) {
		return (T(0) < val) - (val < T(0));
	}
}

// this is becoming a grab bag
struct Vec2 {
	int64_t x;
	int64_t y;
	Vec2 operator+(const Vec2& v2) const { return {v2.x+x,v2.y+y}; }
	void operator+=(const Vec2& v2) { x += v2.x; y += v2.y; };
	Vec2 operator-(const Vec2& v2) const { return { x - v2.x, y - v2.y }; }
	const bool operator==(const Vec2& v2) const { return x == v2.x && y == v2.y; }
};

template <typename T>
using Grid = std::vector<std::vector<T>>;

// turn a typical AoC map input into a 2d vector array
template <typename T>
Grid<T> parseGrid(const std::string& input) {
	const auto rows = pex::pSplit(input, '\n');
	Grid<T> grid;
	for (const auto& row : rows) {
		std::vector<char> newRow;
		for (int x = 0; x < row.size(); x++) {
			newRow.emplace_back(row[x]);
		}
		grid.emplace_back(newRow);
	}
	return grid;
}

template <typename T>
Grid<T> createGrid(int64_t width, int64_t height, T initVal = T(0)) {
	Grid<T> result(height);
	for (auto& row : result) {
		row.resize(width, initVal);
	}
	return result;
}

template <typename T>
bool inBounds(const Grid<T>& grid, const T x, const T y) {
	return((x >= 0)
		&& (y >= 0)
		&& (y < std::ssize(grid))
		&& (x < std::ssize(grid[y])));
}

template <typename T>
bool inBounds(const Grid<T>& grid, const Vec2& point) {
	return((point.x >= 0)
			&& (point.y >= 0)
			&& (point.y < std::ssize(grid))
			&& (point.x < std::ssize(grid[point.y])));
}

template <typename T>
int64_t gridReduce(const Grid<T>& grid, const std::function<int64_t(const char cell)>& predicate) {
	return pex::pReduce(grid, [&predicate](const int64_t acc, const auto& row) {
		return pex::pReduce(row, acc, predicate);
		});
}

// seems like there must be a way to make this deduce everything - 
// I tried playing around with -> declspec(unaryOp()) with no luck...
template<typename T2, typename T1, typename UnaryOperation>
[[nodiscard]] Grid<T2> gridTransform(const Grid<T1>& c, const UnaryOperation unaryOp) {
	Grid<T2> newGrid;
	for (const auto& row : c) {
		std::vector<T2> newRow;
		for (const auto& cell : row) {
			newRow.emplace_back(unaryOp(cell));
		}
		newGrid.emplace_back(newRow);
	}
	return newGrid;
}

template <typename T>
int64_t gridHeight(const Grid<T>& grid) {
	return grid.size();
}


template <typename T>
int64_t gridWidth(const Grid<T>& grid) {
	return grid[0].size();
}

using Grid8 = Grid<int8_t>;


#pragma once
