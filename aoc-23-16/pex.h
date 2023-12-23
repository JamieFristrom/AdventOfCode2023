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

	template<typename Container>
	bool pAnyOf(const Container& c) {
		return std::any_of(c.begin(), c.end());
	}

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

using Map = std::vector<std::vector<char>>;

// turn a typical AoC map input into a 2d vector array
Map parseMap(const std::string& input) {
	const auto rows = pex::pSplit(input, '\n');
	Map map;
	for (const auto& row : rows) {
		std::vector<char> newRow;
		for (int x = 0; x < row.size(); x++) {
			newRow.emplace_back(row[x]);
		}
		map.emplace_back(newRow);
	}
	return map;
}

Map createEmptyMap(int64_t width, int64_t height) {
	Map result(height);
	for (auto& row : result) {
		row.resize(width);
	}
	return result;
}


bool inBounds(const Map& map, const Vec2& point) {
	return((point.x >= 0)
		&& (point.y >= 0)
		&& (point.y < std::ssize(map))
		&& (point.x < std::ssize(map[point.y])));
}

int64_t mapAccumulate(const Map& map, const std::function<int64_t(const int64_t acc, const char cell)>& predicate ) {
	return pex::pAccumulate(map, 0ll, [&predicate](const int64_t acc, const auto& row) {
		return pex::pAccumulate(row, acc, predicate);
		});
}

#pragma once
