#include "pch.h"

#include <algorithm>
#include <deque>
#include <format>
#include <functional>

using namespace std;

string_view getSampleInput() {
	return ">>><<><>><<<>><>>><<<>>><<<><<<>><>><<>>";
}

string_view getPuzzleInput() {
	return "><<<<><<>>><<<>>><<><<>>>><<><<>><><<<<>>>><<>>><<<>>>><<<<><<<<>>>><>>>><>>><<><>><<>><<<<>><<><<><>><<><>>><<<>><>>><<>><<>>><<<><>>><<>>><<<<>><<<<><<<><<>>><<<>>><>>><<<>>><<<<>>>><<<>>><<<>><<<<>><<<>>><<<>>><<>><<>><<<>><>><<<><<>><<<>>>><>>>><>><<>>>><<<<><<<<><<<<><<>>>><<<>>>><<<><<>>><<<><><<>><<>>><<>><>>><<>><<>>>><<<<><<<>>><<>>>><<>>>><<>><>>><<<<>>><<<>><<<<><>>><>><<<<>>><<>>><><<<>>><<<><<<>><<<<>>>><<<><<<><<>>>><<<>>><<><<>>>><<<<>><<>><<<<>>>><<<><<>>><<<<>>>><<<<>>>><<<<>><><>><<>>>><>>>><<<<>><>>><<<<>>><><<><<<>>>><><<<>><<>>><<<>>><<<>><<<><<>><<><<>>>><<>>><<<<><<<<><>>>><<>><<>>><<<>>>><<<<>>>><<<>>>><<>><>>>><><<<>>>><<<<><>>><>>><<>>>><>>><<<<><<>>><<<<><<>>><<>><<<>><<<<>>><<>>><<<>>><>>><<<<><>>><><<<<>>><>>><<<<><><<<<>>>><<>>><<<<>>><<<<>>>><>>>><<><<<<>>><<<<>><<<<>><<>><<<<>>>><<<>>>><<<<>>><>>>><<<<>><<<<>><<>>>><<<><<<>>>><<><<<<>>>><><<<<>><>><<>>>><<<<>>><<>>><<>>><<<>>>><>>><<<<>>>><>>><<>><<><<>><<><<><<<>>><>><<<>><<<><<<<><<<<>><<>><<<<>>>><<<<><<<<>>>><<><<><<<<>><<<>>><>>><<<<><><>><<>>><<><>>><<<>><<<<><<<<>>><<<>>>><<>><<><<<<>><<<<>>><<>><<>><<<<><>>>><><<>>>><>>><<<><>><<>>>><>>><<<<><<<<>>><<<>>><<<>>>><<<<><<<<>>>><<><<<<>>><<<>><<>>>><<>>><<>><<><<<<>>><<>><<><<<<>>><<<><<<<>>><<>>>><<<<>><>>>><<<<>>><<>>><<><<>><<<>>><<<<>>>><<<<>><<><<>><<<<>>>><><<<><><<<><<<><<<<>><>><<<>>><><<<><<<<><<<>><>>><<>><>>><<>><<>><><>>>><<<><>>><<>>>><<<>><<<>><<<<>>><<<<>>><<<<>>>><>><<<>><<>><<<><<>>>><>><<<><<<<>>><<><<<<>><<>>><<<<>>>><<<>>>><>>><<<>>><<<>>><<>><<<<>>>><<<<><><<<<>>>><<<>><<<<><<<<>>><<<<>>>><<>>><<<<>>>><<<<><<<><>>>><<>>>><<<<>><<<>>>><<<<>><<>>>><>><>>>><<<>><<>>>><<<>>>><<>><<<>>>><<<>><>><><<><>><>><<<>><<<>>><<>>><<<<>>><<<><<>>><<<<><>>>><<><>>>><<><<<>>>><><<<<>>><<<<>><<>>>><<<><<<>><<><>>><<>>>><<><>>>><<<<><>><<>><<<>>><>><<<>><<<<>>><<<<>><>><<><<<<><<<>>><><>>><<>>>><<><<<<>>><<>>><<<><<>>><><<<><>>><<><<<>>><<>>><<><<><<<<>>><<><<<<>><<<<>><>>>><<<<><>>>><<<<>>>><<>><<<<>>>><<<<><<<>>><<<<>>><<<<><<<><<>><>>>><>>><<<>>>><<<<>>><<<>><<<<>>>><<<>>><<<<>>><<<<>>><<<>>><<>><<<>><<<<>><<<<>>>><<>><<>><>>><<<<>>><<<<>>><<<<><<>>><><<<<>><<>>>><<><<>>><>><<<><>>><<<<><<>><<>>><><<>>><<<<><<<<><>>>><<>>><<>>>><<<>>><<>>><<<<>>><<>>>><>><><<<<>>>><>>><<<><<<<>>><<><<>>>><<<<>>>><>><>><<<>>>><>><><<>>><<<<><<<>>><<>>><><<>>>><<>>>><<<><<<>>><<<><<<>>><<<><<>>><<<<>><<<>><>>><>><<<<>>>><><<<<>>>><><<<<>>>><<<>>>><<>><<<>>><<<<>>>><<<>><<><>>>><<<>>><<<<><<<>>><<<>>><<>><<>>>><<>>><>><<<>>>><<>>>><<<><<>>><<<>><><<>>><<<<>>>><<<><<>>><>><>>>><><>>><><<<<>><<<><>>>><>><<<>><>>><<<><<><><<>><<<>>>><<<<>>>><>>>><<<><<<>><>>>><<>>>><<<>>><<><<<<><<<<>><<<>><<>>><<><><<<<>>><>>>><<<><>><<><>><>>>><><>>><<><<>><<<>>>><<>><<><<<><><<<>>>><<<<>><<<<>>><<<<>>>><>>><<<<><<>>>><><<>>><<<<>><<<>><<<>>>><><<<>><<<<><<<><<>>>><<<>>>><<<<>>><<<>>>><<<<>><<<>>><<<>>><>>>><<<<><<<<>><>>>><<>><<<<>>><<<<><<<<><<<<><<<<>>><<<>><<<>><<>>>><>><<<<><<><<>>><<>>>><<<><>><<<<><<<>>>><<<<><>><<<>>>><<<<>>>><<<>><<<><<<>>>><<>>>><<<>><><<<<>>>><<>>>><<<>>><>>>><<<<>><<<<>><>>><<<<>>><>>>><<<<>>><<>>><<<>>><<<>>><<<<>>><<<<>>>><<<<>><<<<>><>>><<<<>>><<<>>><<>><<<<><<>>><>>>><<<>>><<>>><<><<<>>>><<<>>><<>>>><<>>><><<<<>><>>>><>>><>>>><<<>><<>><<<<><<<><>><<<<>>>><<<><<>><<<<>><<>><<>><<>>><<<<>>><<<<><<<>>><>><>>>><<<>>><>>>><<<<>><<<<>><>>>><<>>><<<<>>><<>>><<<<><>>>><>><><<<<>><>>><<<<>><<<<>>>><<<>>>><<<<>><<<<><<<<>>><>>><<<<>><<<><<<><><<<>>><<>>>><<<<><<<<>>><<<<><<>>><<<>>><<<<>>>><<<>>><<<<><<><<>>>><>>>><<<<>>><<<<>>>><<>>>><<<>><<<<>><<>>>><<<>>>><<<<>>><<<<>>><<<><<><<<<>>>><<><<><>>>><<<>>><<<<>>><>><>>>><<<<>><<<>>>><<<>><>>><<>><>><<<<>>>><><<<>>>><<>>>><<<<><<<>>><<<<>><<<<>>>><<<><<<>><<>><<<>>>><<<>><>>>><<>>>><<<>><>><>>>><<<>>><<<<>>>><>><<>><><>>><<<<><<>><>><>>><<>><>><<<><<>>>><<>>>><<<>>>><<>>>><><<>>>><<<>><<<<>><<<<><<<<>>>><>>>><><>>><<<<>>>><<<>><<<>>><<<<>>><<><<<>>>><>>><>>><<<>>><<<><>>><<<><<<>>>><<>>><<<<>><<<<>><<<>><<<<>>><<<<>>>><<<<>>>><>>>><><<<<>><<<<>><<<<>>><<<<>>>><>>><<>><>>><>>>><>>><<<><>><>>><><>>><>><>>>><<<<>><<<>>>><<<>>><<<<>><<<<>>><>><><<<<>><<>><<<><<>><<<<>>><<<>><>><>><<><<<>>>><<<<>><<<<><<<<><<<>>>><<><<<<>><<<>>><<><<><>>><<>><<>>>><<<>><<>><>><<>><<<>>>><>>>><><>>>><<<<>>>><<>>>><<>>>><<<<><<>><><<<<>>><<>>>><<<<>>><>>>><<<>>>><<<>><<>>><<>>><<<>>><<<<>>><<><<<<><<<<>>><<>>><<<<>>><<<>>>><<<<>><<<<>>>><<<<>>>><<><>><<>>><<>><<<>>>><>>><>><<<<>>><>><<<<>>>><<<<>><><<<><>>>><><<>>><<<<>><<<<>>><>><>>><<<>>><><<<<>>><>>><<<<>>><>>><<<>><>>>><><<<<>>>><<<<>><<<><<<>><<<>>><<<>>><<>>><>>><<<<>><>>><>>><<<>><<>>><<><<<<>><>>>><>>>><<<>><><>>><<<><>><<<>>>><<>>><<<<>>>><<>>><<<<>><<<<>>>><<<<>>>><<>>><>>>><<<<>>>><<<>>>><<<<>>><<>><><>>><<<>>>><<>>>><>>><<<<>>><<>>><<>>><<<>><>>><>>><<><<<><<<><>><><>>>><<<>>>><<<<>>>><>>><<<>>><<<<>><<<<><<>>><<>>>><<<<>>><<<<>>>><<>><<<<>>><<<<>><<<>>><<>>>><<>>>><<<<>>><<>><>><>><>><<<>>>><<<<>>>><<<>>>><<<<>>>><<<<><<<>><<<>><<<><<>>><<<>><>><<>>><<<><<><<<>>><<<<>><><<>>><<>>>><<><<<>>>><><<<>>>><<<<>><<<>>>><<>>><<<<>>><<<<>>><<<<>>>><<>>>><<><<<<>><<><<>>>><<><<>>><<<>>>><<>>><<<<>>><<<>>><<>>>><<<>>>><<<<>>><<<<>>><<<<>>><<><<>>><<<<>><<<<>>><<<><<<>>>><<<<>>>><<><><<>>><<<<><<<>>>><<<>>>><<<<>>><>>><<>>>><>><<<<>>><<<<>>>><<>><<<<>>><<<>>>><<<>>>><<<<>><>>>><<<<>>>><<<>><>>><<<<><<<>>><<<<>>>><<<<>>><<>>>><<>>>><<<>>><<<<>><>>><<<<>><<<<>>>><>><<<>>>><<><<>>>><<<>>><<><>>>><<<>>><<<>>>><<<<><<<>><<<<><<<>>>><<<<>>>><<<>>>><<<>>><>>>><<<><>>><<<<><<<><>><>><>><<<<>>><<><<<>>><<>><<<<>>><<<<>>><<<<>>><<><<<<>>>><<>>><<<>><<<>><>><<<>>>><>>>><<<<>><<><<<>><<<<>><<>>><<<>>>><<<><><<<>>><><<<<>>><<>><<>><>><<<<>><<<<><>>>><><>>>><<<><<><<<>>><>>><<<>>><<<<><<<<><><<><<>><<>><<<<>>><>>>><<<<>><<><>><>>><>>>><<<>>><<><>><<<<><>>>><>>>><>>>><<>>>><<<>>><<>>><<<>>><>>>><<>><<>><<<<>>>><<<>>><<>><>><<<>>>><>>><<<>>><<<>>><<<<>>>><<><>><<<>>><<<>>>><>>><<><>><<<<>><<<<>>>><<<<>>>><<<>><<<<><>>>><<><><<>><>><><>>>><<<>><<<<><<<<><><><>>><>><<<<>>>><<>>>><>>>><<>><>>>><<<<>><<<<>>><><<<>><>>>><>>>><<>>>><<<<>>>><><<<<><><<<<><<<<><<<>><<<>>>><<<<>>>><>>>><<<<>><<>>>><<<<>>>><>>><<<>>>><<><>>><<<<>>>><<><<><<<>><>><<><<>><<<<>>>><>>>><<>>><<<<>>><>>>><<<>>><<<>><<<><<<><>>><>><>><<<>>>><<<<><>>>><<<<><<<<><<>>><<<>>><<<>><>>><<<>><<<>><<><<<<>><<<>>>><<<>>>><>>><<<>>>><>>>><<<><<>><>><<<<><>>>><<<<>><>>>><<<><<<<><>>><<<<>>>><<<<>>>><<<>><>><<<>>><<<><<<<>><<><>>><<>>>><>><><<<<>>>><<<<>>><<>><<><<<<><<<<>>><<<>><<><<><>>>><>><<<<><<<><<<<>><<<<>><<<>><<><<<<><<<>>>><<><<<<>><<<<>>>><>>><<<<><<<>>><>>>><>>>><<<>>>><<>>>><<>>><<>>>><<>><<<<>>><<<>>><<<<>><<><<>>>><>><<>>>><<<<><<<>>>><<<>>>><<<>>>><<<><<<>><>>><<<<>>><<>><<<><<>>>><<<><<>>>><<<<>><<<<>><>><><<<>><<>>>><<<>><<<<>>><<<><<<<><<<>>><<<><<<<>>>><<>>>><>><<<<>>><><<<>>><<>>>><<<<><<>><<>>><>><<<>>>><<<<>>>><<>>>><<<<><><<<<>><><>><>><<>>><<>>><<><<<><><<<><><<>>><<<<>>>><<<>>><<<>><<<<>><<<>>><<<<><>>>><<>>>><<<<><<<>><<<>>>><<>><<>>>><<<>><<><<<>>>><>>><<<>>>><<>><<<<>>><<>><<<<><<>>><<<<>>>><>>>><<<>>><>><>>>><>>>><<>>><>>>><>>><<>>><<>>><<<>>><<<<>><>>>><<><<>>><<<>>>><<<<>>><<<><<<>>><<<>>><<>><>>><><<<><<>>><<>><<<>><<>>>><<>>>><>>>><<<>>><<<>>>><<<<>>>><><<<<>><<<>><<><<<<>><<<<><>>><>><>>><>>>><<>>><>>><<<<><<<<><<<>><>>>><>><<>>><><<<>>>><<<>>>><>>>><<<<>>><<<<>>>><<<>>><<<><>>>><><<<>><>>>><<><<<><<<>>><<>>><>>><<<<>>>><<><<<<>>>><<>>><<<<>>>><<<><<<<><>>><<<><>>><<<<>>><<<<><><<<<>><<<>><<<<><<<>><>>><>><<>>>><<>>><<<<>>><>>><<><<<<>>><>><<<<>>><><><<>>>><<<>>>><<>>>><<<<><>>>><<<<>><<>>>><>><<<>>><<>><>>><<<><<<>><<<><<>>><>>><<><<>><<<<><<>>>><<>>>><<<>>><<<<>><<<>><<<>>><<<>><<<<>>><<<<><><<<><><<<>><><>>>><>>>><<<><<<><<<<><<<>>>><<>><>>>><<<<>>>><>><<<<>>><>>>><<<><<<>>>><<<>>><><<<>>>><<<>>><<<<>>><>>><<<<><<<>><<<><><<<>><<><>>>><<>><<>>><<>><<<><>>>><<>><<<>><<>><>>><<<><<<<>>><<>>>><<>>><<<<>>><>>>><<<<>>><<<<>>><<>>><<>>>><<<<><>><<<>>><<<>>><>><<>>><><>><<>>><<<<>><<<<><<<<>>><<<<>>><<>>>><<<<>>>><>>><<<<><<>>>><<>>><>><<<<><<<<>>><<>>><<<<>>><<<><<>><<<<>>><>>>><<<>>>><<>><>><<<><<<>>>><<>>><<<<>>>><<<<>><<<<>>>><<<>>>><<>>><<<<>>><<<>>>><<>>><><>>>><<<<>>><<<<>>><<<>>>><<<<>>>><>>>><<>>>><><<>><>>>><<<<>>><<<<><<<>><>><<>>>><<>><>>><<>><<>>><<>>><<>>>><<<<>><<<<>><<<<>>>><<<<>>>><<>><<<><<>>><<>>><<<<>>><>>>><<><>><><>>><<<>><<<<>>>><<>><<<><<<>>><<<<>>><<>>>><<>><<<>>><<>>>><<>><<>>><<<<>><<<<>><<<><<<>>>><<<>>><<<<>><>><>>><>>>><>>>><<>>>><>>><<<>>><<><>>>><<<<>>><<<>><<<<>><<><>>>><<<<><<><>>><<<<>><<<<>>><<<><<><<<>><>>>><<<<>><<<<>>>><<<>>><><<>>>><<><><<>>><<<<>><<<<>>><>>>><<<>><<<<>>>><<<>>><<<<>>>><<><<<>>><<>>><<<><><<<<><><<>><<<<>>>><<<<>><<<<><<>>>><>><>><<>>>><>>>><<<<>><><<<>><<<>>>><<><<>><<<<>><<><>>>><<<<>><<>><<<>>><>>>><<<<><<<<>><<<><<>>>><<<><<<>>>><<>>>><<<<><>>><<<>>>><<<<>>>><>><>>>><<<>><<>>>><<<>>>><>>>><<<<>>>><<<<>>><<<><<><<<<>><>>>><<<>>><<>><<<<>>><<<<>><>><>>>><>>>><<>><>>>><<<<>>><>>><<<<>><<><<<>>><<>>>><<<<>>>><><><<<>>><<<<><<<<>><<><<<<>><<<>>><<><<<<>>>><<<<>><>><>><><><<><>><<<<>>><<><<<>>><<><<>>>><<<><<<<><<<<><<>>><>>><<>>><<>>>><<><<<<>>>><><<><<>>><><<<<>><>>><><<<<>>><<<<>><<<>>>><<<<><<<>><<<>><<><><<<>><<<>>>><<<><<<><>>><<<<>>><<>><<<>>><<<>>><<<><<><<<<>>><<<><>>>><<><<<>>><>>><<<>>><<<>>>><<<>>><<<>>>><<<><<>>><<>><<<>>>><<<<>><<<<><><<<<>><<>><<<>>><<<>><<<>>><<<<>><<>><>><<<><<>>><<<>>><>>><<<>>><<<>><<<>>>><<<<>><<>><<>><><<<>><<<<>>><<<>>><<<<><>>><<<<>><<<<>>>><<<>>><<<<>><<>>><<>>><<<>>><<<>><<>>>><><<>>>><><<>>><><<<<>>>><<<<>><>><<<<>>><<<>><<<><<>>><<<<><>>><>>><<>>>><<><<<>><<<>>><<<>>><<<>><<>><<><<<<><<<<>>><<<><<<<><<<<><<<><<<><>>><<>>>><>>>><>>><<><<<<><<<>>><<<>>><<><<<<>>>><<<<>>>><<>>>><<>>>><<<<><<>><>><<<><><><>><<<>><<<<>>>><<<<>><<<<>>><<><<<<>><<><>>><<<>><<<><<<<>><><<<<><<<>>><>>><<>>><<>>>><><<>><>>>><<<<><<<<><<><<<>><<<>>>><<>><<>>>><<<<>><<<>><<<><<>><<>>><<>>><<>>><<>>>><<>>><<<<>>><<><<<<>><>><<<><<<>>><<>>>><<<>><>>><<><<<<>>>><>>>><<><>>><<<<><<<><>>>><<<<>>>><<<><>><<<<><<<>>>><<<><<<<>>><<<>><><>><>>>><<<<>>>><>><<<<>>><<>><<<<><<>>>><<<<>><>>><>><<<>>>><>><<<><><<>>>><<<>><<>><>><<><<>><>>><>>><<><><<>>>><<>>>><>>><<>>>><>><";
}

using vecu8 = vector<uint8_t>;
using RockShape = vector<vecu8>;

auto rockShapes = vector<RockShape>{
RockShape{
	vecu8{ 1,1,1,1 },
},
RockShape{
	vecu8{ 0,1,0 },
	vecu8{ 1,1,1 },
	vecu8{ 0,1,0 }
},
RockShape{
	vecu8{1,1,1},
	vecu8{0,0,1},
	vecu8{0,0,1}
},
RockShape{
	vecu8{1},
	vecu8{1},
	vecu8{1},
	vecu8{1},
},
RockShape{
	vecu8{ 1,1 },
	vecu8{ 1,1 }
} };

//deque < array<uint8_t>(7)> = { 0, 0, 0, 0, 0, 0, 0 };


void simulate() {

}
//
//
//std::strspan get_sample_input() -> & 'static str {
//">>><<><>><<<>><>>><<<>>><<<><<<>><>><<>>"
//}
//
//fn get_puzzle_input() -> & 'static str {
//"><<<<><<>>><<<>>><<><<>>>><<><<>><><<<<>>>><<>>><<<>>>><<<<><<<<>>>><>>>><>>><<><>><<>><<<<>><<><<><>><<><>>><<<>><>>><<>><<>>><<<><>>><<>>><<<<>><<<<><<<><<>>><<<>>><>>><<<>>><<<<>>>><<<>>><<<>><<<<>><<<>>><<<>>><<>><<>><<<>><>><<<><<>><<<>>>><>>>><>><<>>>><<<<><<<<><<<<><<>>>><<<>>>><<<><<>>><<<><><<>><<>>><<>><>>><<>><<>>>><<<<><<<>>><<>>>><<>>>><<>><>>><<<<>>><<<>><<<<><>>><>><<<<>>><<>>><><<<>>><<<><<<>><<<<>>>><<<><<<><<>>>><<<>>><<><<>>>><<<<>><<>><<<<>>>><<<><<>>><<<<>>>><<<<>>>><<<<>><><>><<>>>><>>>><<<<>><>>><<<<>>><><<><<<>>>><><<<>><<>>><<<>>><<<>><<<><<>><<><<>>>><<>>><<<<><<<<><>>>><<>><<>>><<<>>>><<<<>>>><<<>>>><<>><>>>><><<<>>>><<<<><>>><>>><<>>>><>>><<<<><<>>><<<<><<>>><<>><<<>><<<<>>><<>>><<<>>><>>><<<<><>>><><<<<>>><>>><<<<><><<<<>>>><<>>><<<<>>><<<<>>>><>>>><<><<<<>>><<<<>><<<<>><<>><<<<>>>><<<>>>><<<<>>><>>>><<<<>><<<<>><<>>>><<<><<<>>>><<><<<<>>>><><<<<>><>><<>>>><<<<>>><<>>><<>>><<<>>>><>>><<<<>>>><>>><<>><<><<>><<><<><<<>>><>><<<>><<<><<<<><<<<>><<>><<<<>>>><<<<><<<<>>>><<><<><<<<>><<<>>><>>><<<<><><>><<>>><<><>>><<<>><<<<><<<<>>><<<>>>><<>><<><<<<>><<<<>>><<>><<>><<<<><>>>><><<>>>><>>><<<><>><<>>>><>>><<<<><<<<>>><<<>>><<<>>>><<<<><<<<>>>><<><<<<>>><<<>><<>>>><<>>><<>><<><<<<>>><<>><<><<<<>>><<<><<<<>>><<>>>><<<<>><>>>><<<<>>><<>>><<><<>><<<>>><<<<>>>><<<<>><<><<>><<<<>>>><><<<><><<<><<<><<<<>><>><<<>>><><<<><<<<><<<>><>>><<>><>>><<>><<>><><>>>><<<><>>><<>>>><<<>><<<>><<<<>>><<<<>>><<<<>>>><>><<<>><<>><<<><<>>>><>><<<><<<<>>><<><<<<>><<>>><<<<>>>><<<>>>><>>><<<>>><<<>>><<>><<<<>>>><<<<><><<<<>>>><<<>><<<<><<<<>>><<<<>>>><<>>><<<<>>>><<<<><<<><>>>><<>>>><<<<>><<<>>>><<<<>><<>>>><>><>>>><<<>><<>>>><<<>>>><<>><<<>>>><<<>><>><><<><>><>><<<>><<<>>><<>>><<<<>>><<<><<>>><<<<><>>>><<><>>>><<><<<>>>><><<<<>>><<<<>><<>>>><<<><<<>><<><>>><<>>>><<><>>>><<<<><>><<>><<<>>><>><<<>><<<<>>><<<<>><>><<><<<<><<<>>><><>>><<>>>><<><<<<>>><<>>><<<><<>>><><<<><>>><<><<<>>><<>>><<><<><<<<>>><<><<<<>><<<<>><>>>><<<<><>>>><<<<>>>><<>><<<<>>>><<<<><<<>>><<<<>>><<<<><<<><<>><>>>><>>><<<>>>><<<<>>><<<>><<<<>>>><<<>>><<<<>>><<<<>>><<<>>><<>><<<>><<<<>><<<<>>>><<>><<>><>>><<<<>>><<<<>>><<<<><<>>><><<<<>><<>>>><<><<>>><>><<<><>>><<<<><<>><<>>><><<>>><<<<><<<<><>>>><<>>><<>>>><<<>>><<>>><<<<>>><<>>>><>><><<<<>>>><>>><<<><<<<>>><<><<>>>><<<<>>>><>><>><<<>>>><>><><<>>><<<<><<<>>><<>>><><<>>>><<>>>><<<><<<>>><<<><<<>>><<<><<>>><<<<>><<<>><>>><>><<<<>>>><><<<<>>>><><<<<>>>><<<>>>><<>><<<>>><<<<>>>><<<>><<><>>>><<<>>><<<<><<<>>><<<>>><<>><<>>>><<>>><>><<<>>>><<>>>><<<><<>>><<<>><><<>>><<<<>>>><<<><<>>><>><>>>><><>>><><<<<>><<<><>>>><>><<<>><>>><<<><<><><<>><<<>>>><<<<>>>><>>>><<<><<<>><>>>><<>>>><<<>>><<><<<<><<<<>><<<>><<>>><<><><<<<>>><>>>><<<><>><<><>><>>>><><>>><<><<>><<<>>>><<>><<><<<><><<<>>>><<<<>><<<<>>><<<<>>>><>>><<<<><<>>>><><<>>><<<<>><<<>><<<>>>><><<<>><<<<><<<><<>>>><<<>>>><<<<>>><<<>>>><<<<>><<<>>><<<>>><>>>><<<<><<<<>><>>>><<>><<<<>>><<<<><<<<><<<<><<<<>>><<<>><<<>><<>>>><>><<<<><<><<>>><<>>>><<<><>><<<<><<<>>>><<<<><>><<<>>>><<<<>>>><<<>><<<><<<>>>><<>>>><<<>><><<<<>>>><<>>>><<<>>><>>>><<<<>><<<<>><>>><<<<>>><>>>><<<<>>><<>>><<<>>><<<>>><<<<>>><<<<>>>><<<<>><<<<>><>>><<<<>>><<<>>><<>><<<<><<>>><>>>><<<>>><<>>><<><<<>>>><<<>>><<>>>><<>>><><<<<>><>>>><>>><>>>><<<>><<>><<<<><<<><>><<<<>>>><<<><<>><<<<>><<>><<>><<>>><<<<>>><<<<><<<>>><>><>>>><<<>>><>>>><<<<>><<<<>><>>>><<>>><<<<>>><<>>><<<<><>>>><>><><<<<>><>>><<<<>><<<<>>>><<<>>>><<<<>><<<<><<<<>>><>>><<<<>><<<><<<><><<<>>><<>>>><<<<><<<<>>><<<<><<>>><<<>>><<<<>>>><<<>>><<<<><<><<>>>><>>>><<<<>>><<<<>>>><<>>>><<<>><<<<>><<>>>><<<>>>><<<<>>><<<<>>><<<><<><<<<>>>><<><<><>>>><<<>>><<<<>>><>><>>>><<<<>><<<>>>><<<>><>>><<>><>><<<<>>>><><<<>>>><<>>>><<<<><<<>>><<<<>><<<<>>>><<<><<<>><<>><<<>>>><<<>><>>>><<>>>><<<>><>><>>>><<<>>><<<<>>>><>><<>><><>>><<<<><<>><>><>>><<>><>><<<><<>>>><<>>>><<<>>>><<>>>><><<>>>><<<>><<<<>><<<<><<<<>>>><>>>><><>>><<<<>>>><<<>><<<>>><<<<>>><<><<<>>>><>>><>>><<<>>><<<><>>><<<><<<>>>><<>>><<<<>><<<<>><<<>><<<<>>><<<<>>>><<<<>>>><>>>><><<<<>><<<<>><<<<>>><<<<>>>><>>><<>><>>><>>>><>>><<<><>><>>><><>>><>><>>>><<<<>><<<>>>><<<>>><<<<>><<<<>>><>><><<<<>><<>><<<><<>><<<<>>><<<>><>><>><<><<<>>>><<<<>><<<<><<<<><<<>>>><<><<<<>><<<>>><<><<><>>><<>><<>>>><<<>><<>><>><<>><<<>>>><>>>><><>>>><<<<>>>><<>>>><<>>>><<<<><<>><><<<<>>><<>>>><<<<>>><>>>><<<>>>><<<>><<>>><<>>><<<>>><<<<>>><<><<<<><<<<>>><<>>><<<<>>><<<>>>><<<<>><<<<>>>><<<<>>>><<><>><<>>><<>><<<>>>><>>><>><<<<>>><>><<<<>>>><<<<>><><<<><>>>><><<>>><<<<>><<<<>>><>><>>><<<>>><><<<<>>><>>><<<<>>><>>><<<>><>>>><><<<<>>>><<<<>><<<><<<>><<<>>><<<>>><<>>><>>><<<<>><>>><>>><<<>><<>>><<><<<<>><>>>><>>>><<<>><><>>><<<><>><<<>>>><<>>><<<<>>>><<>>><<<<>><<<<>>>><<<<>>>><<>>><>>>><<<<>>>><<<>>>><<<<>>><<>><><>>><<<>>>><<>>>><>>><<<<>>><<>>><<>>><<<>><>>><>>><<><<<><<<><>><><>>>><<<>>>><<<<>>>><>>><<<>>><<<<>><<<<><<>>><<>>>><<<<>>><<<<>>>><<>><<<<>>><<<<>><<<>>><<>>>><<>>>><<<<>>><<>><>><>><>><<<>>>><<<<>>>><<<>>>><<<<>>>><<<<><<<>><<<>><<<><<>>><<<>><>><<>>><<<><<><<<>>><<<<>><><<>>><<>>>><<><<<>>>><><<<>>>><<<<>><<<>>>><<>>><<<<>>><<<<>>><<<<>>>><<>>>><<><<<<>><<><<>>>><<><<>>><<<>>>><<>>><<<<>>><<<>>><<>>>><<<>>>><<<<>>><<<<>>><<<<>>><<><<>>><<<<>><<<<>>><<<><<<>>>><<<<>>>><<><><<>>><<<<><<<>>>><<<>>>><<<<>>><>>><<>>>><>><<<<>>><<<<>>>><<>><<<<>>><<<>>>><<<>>>><<<<>><>>>><<<<>>>><<<>><>>><<<<><<<>>><<<<>>>><<<<>>><<>>>><<>>>><<<>>><<<<>><>>><<<<>><<<<>>>><>><<<>>>><<><<>>>><<<>>><<><>>>><<<>>><<<>>>><<<<><<<>><<<<><<<>>>><<<<>>>><<<>>>><<<>>><>>>><<<><>>><<<<><<<><>><>><>><<<<>>><<><<<>>><<>><<<<>>><<<<>>><<<<>>><<><<<<>>>><<>>><<<>><<<>><>><<<>>>><>>>><<<<>><<><<<>><<<<>><<>>><<<>>>><<<><><<<>>><><<<<>>><<>><<>><>><<<<>><<<<><>>>><><>>>><<<><<><<<>>><>>><<<>>><<<<><<<<><><<><<>><<>><<<<>>><>>>><<<<>><<><>><>>><>>>><<<>>><<><>><<<<><>>>><>>>><>>>><<>>>><<<>>><<>>><<<>>><>>>><<>><<>><<<<>>>><<<>>><<>><>><<<>>>><>>><<<>>><<<>>><<<<>>>><<><>><<<>>><<<>>>><>>><<><>><<<<>><<<<>>>><<<<>>>><<<>><<<<><>>>><<><><<>><>><><>>>><<<>><<<<><<<<><><><>>><>><<<<>>>><<>>>><>>>><<>><>>>><<<<>><<<<>>><><<<>><>>>><>>>><<>>>><<<<>>>><><<<<><><<<<><<<<><<<>><<<>>>><<<<>>>><>>>><<<<>><<>>>><<<<>>>><>>><<<>>>><<><>>><<<<>>>><<><<><<<>><>><<><<>><<<<>>>><>>>><<>>><<<<>>><>>>><<<>>><<<>><<<><<<><>>><>><>><<<>>>><<<<><>>>><<<<><<<<><<>>><<<>>><<<>><>>><<<>><<<>><<><<<<>><<<>>>><<<>>>><>>><<<>>>><>>>><<<><<>><>><<<<><>>>><<<<>><>>>><<<><<<<><>>><<<<>>>><<<<>>>><<<>><>><<<>>><<<><<<<>><<><>>><<>>>><>><><<<<>>>><<<<>>><<>><<><<<<><<<<>>><<<>><<><<><>>>><>><<<<><<<><<<<>><<<<>><<<>><<><<<<><<<>>>><<><<<<>><<<<>>>><>>><<<<><<<>>><>>>><>>>><<<>>>><<>>>><<>>><<>>>><<>><<<<>>><<<>>><<<<>><<><<>>>><>><<>>>><<<<><<<>>>><<<>>>><<<>>>><<<><<<>><>>><<<<>>><<>><<<><<>>>><<<><<>>>><<<<>><<<<>><>><><<<>><<>>>><<<>><<<<>>><<<><<<<><<<>>><<<><<<<>>>><<>>>><>><<<<>>><><<<>>><<>>>><<<<><<>><<>>><>><<<>>>><<<<>>>><<>>>><<<<><><<<<>><><>><>><<>>><<>>><<><<<><><<<><><<>>><<<<>>>><<<>>><<<>><<<<>><<<>>><<<<><>>>><<>>>><<<<><<<>><<<>>>><<>><<>>>><<<>><<><<<>>>><>>><<<>>>><<>><<<<>>><<>><<<<><<>>><<<<>>>><>>>><<<>>><>><>>>><>>>><<>>><>>>><>>><<>>><<>>><<<>>><<<<>><>>>><<><<>>><<<>>>><<<<>>><<<><<<>>><<<>>><<>><>>><><<<><<>>><<>><<<>><<>>>><<>>>><>>>><<<>>><<<>>>><<<<>>>><><<<<>><<<>><<><<<<>><<<<><>>><>><>>><>>>><<>>><>>><<<<><<<<><<<>><>>>><>><<>>><><<<>>>><<<>>>><>>>><<<<>>><<<<>>>><<<>>><<<><>>>><><<<>><>>>><<><<<><<<>>><<>>><>>><<<<>>>><<><<<<>>>><<>>><<<<>>>><<<><<<<><>>><<<><>>><<<<>>><<<<><><<<<>><<<>><<<<><<<>><>>><>><<>>>><<>>><<<<>>><>>><<><<<<>>><>><<<<>>><><><<>>>><<<>>>><<>>>><<<<><>>>><<<<>><<>>>><>><<<>>><<>><>>><<<><<<>><<<><<>>><>>><<><<>><<<<><<>>>><<>>>><<<>>><<<<>><<<>><<<>>><<<>><<<<>>><<<<><><<<><><<<>><><>>>><>>>><<<><<<><<<<><<<>>>><<>><>>>><<<<>>>><>><<<<>>><>>>><<<><<<>>>><<<>>><><<<>>>><<<>>><<<<>>><>>><<<<><<<>><<<><><<<>><<><>>>><<>><<>>><<>><<<><>>>><<>><<<>><<>><>>><<<><<<<>>><<>>>><<>>><<<<>>><>>>><<<<>>><<<<>>><<>>><<>>>><<<<><>><<<>>><<<>>><>><<>>><><>><<>>><<<<>><<<<><<<<>>><<<<>>><<>>>><<<<>>>><>>><<<<><<>>>><<>>><>><<<<><<<<>>><<>>><<<<>>><<<><<>><<<<>>><>>>><<<>>>><<>><>><<<><<<>>>><<>>><<<<>>>><<<<>><<<<>>>><<<>>>><<>>><<<<>>><<<>>>><<>>><><>>>><<<<>>><<<<>>><<<>>>><<<<>>>><>>>><<>>>><><<>><>>>><<<<>>><<<<><<<>><>><<>>>><<>><>>><<>><<>>><<>>><<>>>><<<<>><<<<>><<<<>>>><<<<>>>><<>><<<><<>>><<>>><<<<>>><>>>><<><>><><>>><<<>><<<<>>>><<>><<<><<<>>><<<<>>><<>>>><<>><<<>>><<>>>><<>><<>>><<<<>><<<<>><<<><<<>>>><<<>>><<<<>><>><>>><>>>><>>>><<>>>><>>><<<>>><<><>>>><<<<>>><<<>><<<<>><<><>>>><<<<><<><>>><<<<>><<<<>>><<<><<><<<>><>>>><<<<>><<<<>>>><<<>>><><<>>>><<><><<>>><<<<>><<<<>>><>>>><<<>><<<<>>>><<<>>><<<<>>>><<><<<>>><<>>><<<><><<<<><><<>><<<<>>>><<<<>><<<<><<>>>><>><>><<>>>><>>>><<<<>><><<<>><<<>>>><<><<>><<<<>><<><>>>><<<<>><<>><<<>>><>>>><<<<><<<<>><<<><<>>>><<<><<<>>>><<>>>><<<<><>>><<<>>>><<<<>>>><>><>>>><<<>><<>>>><<<>>>><>>>><<<<>>>><<<<>>><<<><<><<<<>><>>>><<<>>><<>><<<<>>><<<<>><>><>>>><>>>><<>><>>>><<<<>>><>>><<<<>><<><<<>>><<>>>><<<<>>>><><><<<>>><<<<><<<<>><<><<<<>><<<>>><<><<<<>>>><<<<>><>><>><><><<><>><<<<>>><<><<<>>><<><<>>>><<<><<<<><<<<><<>>><>>><<>>><<>>>><<><<<<>>>><><<><<>>><><<<<>><>>><><<<<>>><<<<>><<<>>>><<<<><<<>><<<>><<><><<<>><<<>>>><<<><<<><>>><<<<>>><<>><<<>>><<<>>><<<><<><<<<>>><<<><>>>><<><<<>>><>>><<<>>><<<>>>><<<>>><<<>>>><<<><<>>><<>><<<>>>><<<<>><<<<><><<<<>><<>><<<>>><<<>><<<>>><<<<>><<>><>><<<><<>>><<<>>><>>><<<>>><<<>><<<>>>><<<<>><<>><<>><><<<>><<<<>>><<<>>><<<<><>>><<<<>><<<<>>>><<<>>><<<<>><<>>><<>>><<<>>><<<>><<>>>><><<>>>><><<>>><><<<<>>>><<<<>><>><<<<>>><<<>><<<><<>>><<<<><>>><>>><<>>>><<><<<>><<<>>><<<>>><<<>><<>><<><<<<><<<<>>><<<><<<<><<<<><<<><<<><>>><<>>>><>>>><>>><<><<<<><<<>>><<<>>><<><<<<>>>><<<<>>>><<>>>><<>>>><<<<><<>><>><<<><><><>><<<>><<<<>>>><<<<>><<<<>>><<><<<<>><<><>>><<<>><<<><<<<>><><<<<><<<>>><>>><<>>><<>>>><><<>><>>>><<<<><<<<><<><<<>><<<>>>><<>><<>>>><<<<>><<<>><<<><<>><<>>><<>>><<>>><<>>>><<>>><<<<>>><<><<<<>><>><<<><<<>>><<>>>><<<>><>>><<><<<<>>>><>>>><<><>>><<<<><<<><>>>><<<<>>>><<<><>><<<<><<<>>>><<<><<<<>>><<<>><><>><>>>><<<<>>>><>><<<<>>><<>><<<<><<>>>><<<<>><>>><>><<<>>>><>><<<><><<>>>><<<>><<>><>><<><<>><>>><>>><<><><<>>>><<>>>><>>><<>>>><>><"
//}

struct v2 {
public:
	int64_t x = 0;
	int64_t y = 0;

	const bool operator==(const v2& a) const { return x == a.x && y == a.y; }
};

using RockFlavor = int;

struct Rock {
public:
	RockFlavor flavor = 0;
	v2 pos;  // lower left corner is origin
	int64_t tickCameToRest=0;

	int64_t rightBound() { return pos.x + ssize(rockShapes[flavor][0]); }
	int64_t topBound() { return pos.y + ssize(rockShapes[flavor]); }
};

bool rocksIntersect(Rock rockA, Rock rockB) {
	const auto aLeft = rockA.pos.x;
	const auto aRight = rockA.rightBound();
	const auto bLeft = rockB.pos.x;
	const auto bRight = rockB.rightBound();
	if (aRight <= bLeft) {
		return false;
	}
	if (aLeft >= bRight) {
		return false;
	}
	const auto aTop = rockA.topBound();
	const auto aBottom = rockA.pos.y;
	const auto bTop = rockB.topBound();
	const auto bBottom = rockB.pos.y;
	if (aTop <= bBottom) {
		return false;
	}
	if (aBottom >= bTop) {
		return false;
	}
	for (int y = 0; y < ssize(rockShapes[rockA.flavor]); y++) {
		for (int x = 0; x < ssize(rockShapes[rockA.flavor][y]); x++) {
			if (rockShapes[rockA.flavor][y][x]) {
				auto relativeY = y + rockA.pos.y - rockB.pos.y;
				auto relativeX = x + rockA.pos.x - rockB.pos.x;
				if (relativeY < 0 || relativeX < 0) {
					continue;
				}
				if (relativeY >= ssize(rockShapes[rockB.flavor])) {
					continue;
				}
				if (relativeX >= ssize(rockShapes[rockB.flavor][relativeY])) {
					continue;
				}
				if (rockShapes[rockB.flavor][relativeY][relativeX]) {
					return true;
				}
			}
		}

	}
	return false;
}

v2 rockBlow(const deque<Rock>& fallenRocks, RockFlavor flavor, const v2& pos, int direction) {
	v2 newPos = pos;
	newPos.x += direction;
	if (newPos.x < 0) {
		return pos;
	}
	if (newPos.x + rockShapes[flavor][0].size() > 7) {
		return pos;
	}

	Rock newRock(flavor, newPos);
	for (auto fallenRock = fallenRocks.rbegin(); fallenRock != fallenRocks.rend(); ++fallenRock) {
		if (fallenRock->pos.y < newRock.pos.y - 100) {  // hopefully that's safe
			break;
		}
		if (rocksIntersect(newRock, *fallenRock)) {
			return pos;
		}
	}
	//for (const auto& fallenRock : fallenRocks) {
	//	if (rocksIntersect(newRock, fallenRock)) {
	//		return pos;
	//	}
	//}
	return newPos;
}

std::tuple<bool, v2> rockFallStep(const deque<Rock>& fallenRocks, RockFlavor flavor, const v2& pos) {
	v2 newPos = pos; 
	newPos.y -= 1;
	Rock newRock(flavor, newPos);
	if (fallenRocks.empty()) {
		if (newPos.y < 0) {
			return { true, pos };
		}
	}
	for (auto fallenRock = fallenRocks.rbegin(); fallenRock != fallenRocks.rend(); ++fallenRock ) {
		if (fallenRock->pos.y < newRock.pos.y - 100) {  // hopefully that's safe
			break;
		}
		if (rocksIntersect(newRock, *fallenRock)) {
			return { true, pos };
		}
	}
	return { false, newPos };
}

std::tuple<v2, int64_t> rockFall(
	const deque<Rock>& fallenRocks, 
	const std::string_view& windPattern,
	int64_t tick,
	RockFlavor flavor, 
	v2 pos ) {
	for (;;) {
		auto direction = windPattern[tick % windPattern.size()] == '<' ? -1 : +1;
		tick++;
		const auto blownPos = rockBlow(fallenRocks, flavor, pos, direction);
		const auto [cameToRest, newpos] = rockFallStep(fallenRocks, flavor, blownPos);
		if (cameToRest) {
			return { newpos, tick };
		}
		pos = newpos;
	}
}

/*
	transform that is non-destructive, operates on whole container, and is ergonomic
	and always returns vector. It would be cool if there's a way to get it to return 
*/
template<typename ReturnContainer, typename IngestContainer, typename UnaryOperation>
ReturnContainer qTransform(const IngestContainer& c, UnaryOperation unaryOp) {
	ReturnContainer newC;
	std::transform(c.begin(), c.end(), std::back_inserter(newC), unaryOp);
	return newC;
}

template<typename Container>
Container::value_type qMaxElementValue(const Container& c) {
	return *std::max_element(c.begin(), c.end());
}

template<typename Container>
auto qFindIf(const Container& c, std::function<bool(typename Container::value_type)> predicate) {
	return std::find_if(c.begin(), c.end(), predicate);
}

int64_t getHighestRockY(const deque<Rock>& fallenRocks) {
	// it is possible for the last rock to fall past the highest rock, so we have to work for it
	// bet this is too slow
	const vector<int64_t> rockHeights = qTransform<vector<int64_t>>(fallenRocks, [](const Rock& rock)->int64_t { 
		return rock.pos.y + rockShapes[rock.flavor].size(); 
		});
	const auto highestRock = qMaxElementValue(rockHeights);
	return highestRock;
}

/* Each rock appears so that its left edge is two units away from the left wall and its bottom edge is three
   units above the highest rock in the room(or the floor, if there isn'c one). */
v2 getStartingPos(const deque<Rock>& fallenRocks) {
	if (fallenRocks.empty()) {
		return v2{ 2,3 };
	}
	else {
		return v2{ 2, getHighestRockY(fallenRocks) + 3 };
	}
}

int64_t doTheThing(int64_t numRocks, const std::string_view& puzzleInput) {
	deque<Rock> fallenRocks;
	vector<int64_t> loopPoints;     //how many rocks have fallen after puzzle input
	vector<int64_t> loopPointTicks; //what tick we're on after rock falls 
	int64_t tick = 0;
	for (int64_t i = 0; i < numRocks; i++) {
		const RockFlavor rockFlavor = static_cast<RockFlavor>(i % rockShapes.size());
		auto [restPos, newtick] = rockFall(fallenRocks, puzzleInput, tick, rockFlavor, getStartingPos(fallenRocks));
		fallenRocks.emplace_back(Rock{ rockFlavor, restPos, newtick });
		if( newtick / std::ssize(puzzleInput) > tick / std::ssize(puzzleInput)) {
			// search for loops
			for( int loopIdx=0; loopIdx<std::ssize(loopPoints); loopIdx++ ) {
				int64_t k = 0;
				int64_t loopPoint = loopPoints[loopIdx];
				const auto loopPointTick = loopPointTicks[loopIdx];
				const auto rockFinishedOffset = loopPointTick % std::ssize(puzzleInput);
				if (rockFinishedOffset != newtick % std::ssize(puzzleInput)) {
					std::cout << std::format("No loop at {} with {}: tick offsets don't match\n", loopPoint, tick);
					break;
				}
				for (k = 0; k < 20; k++) {
					if ((fallenRocks[loopPoint - k -1].flavor != fallenRocks[std::ssize(fallenRocks) -1 - k].flavor) ||
						(fallenRocks[loopPoint - k -1].pos.x != fallenRocks[std::ssize(fallenRocks) -1 -k].pos.x)) {
						auto oldVertOffset = fallenRocks[loopPoint - k - 1].pos.y - fallenRocks[loopPoint - k - 2].pos.y;
						auto newVertOffset = fallenRocks[std::ssize(fallenRocks) - k - 1].pos.y - fallenRocks[std::ssize(fallenRocks) - k - 2].pos.y;
						if (oldVertOffset != newVertOffset) {
							std::cout << "No loop at " << loopPoint << " with " << std::ssize(fallenRocks) << " because different at " << k << std::endl;
							break;
						}
					}
				}
				if (k == 20) {
					std::cout << "We might have found a loop! " << loopPoint << " with " << std::ssize(fallenRocks) << std::endl;
					auto foundRock = qFindIf(fallenRocks, [modpoint = 1000000000000 % std::ssize(fallenRocks)](const Rock& rock) { return rock.tickCameToRest >= modpoint; });
					std::cout << std::format("Offset rock for gigapoint: flavor {}, y {}\n", foundRock->flavor, foundRock->pos.y);
					// what's the height at 1000000000000 
					return 0;
				}
			}
			loopPoints.push_back(fallenRocks.size());
			loopPointTicks.push_back(newtick);
		}

		tick = newtick;
	}
	return getHighestRockY(fallenRocks);
}


TEST(rocks, inputLength) {
	auto puzzleInput = getPuzzleInput();
  EXPECT_EQ(puzzleInput.length(), 10091);
}

TEST(rocks, partOne) {
	EXPECT_EQ(3109, doTheThing(2022, getPuzzleInput()));
}

TEST(rocks, rockNextToFloor_rockStops) {
	auto flavor = 0;
	auto rockPos = v2{ 1, 0 };
	auto previousRocks = deque<Rock>{};

	auto[stopped, newRock] = rockFallStep(previousRocks, flavor, rockPos);

	EXPECT_TRUE(stopped);
	EXPECT_EQ(newRock, rockPos);
}


TEST(rocks, rockNextToFloor_rockDrops) {
	auto flavor = 0;
	auto rockPos = v2{ 2,1 };
	auto previousRocks = deque<Rock>{};

	auto [stopped, newRock] = rockFallStep(previousRocks, flavor, rockPos);

	EXPECT_FALSE(stopped);
	EXPECT_EQ(newRock, v2(2,0 ));
}



TEST(rocks, rock3AboveRock3_rockStops) {
	auto flavor = 3;
	auto rockPos = v2{ 4,4 };
	auto previousRocks = deque<Rock>{ Rock{3, v2{4,0}} };

	auto [stopped, newRock] = rockFallStep(previousRocks, flavor, rockPos);

	EXPECT_TRUE(stopped);
	EXPECT_EQ(newRock, v2(4, 4));
	}

TEST(rocks, rock0AboveRock0_rockStops) {
	auto flavor = 0;
	auto rockPos = v2{ 4,1 };
	auto previousRocks = deque<Rock>{ Rock{3, v2{4,0}} };

	auto [stopped, newRock] = rockFallStep(previousRocks, flavor, rockPos);

	EXPECT_TRUE(stopped);
	EXPECT_EQ(newRock, v2(4, 1));
}

TEST(rocks, rock3IntersectionTests) {
	EXPECT_TRUE(rocksIntersect(Rock{ 3, v2{0, 0} }, Rock{ 3, v2{0, 3} }));
	EXPECT_FALSE(rocksIntersect(Rock{ 3, v2{0, 0} }, Rock{ 3, v2{0, 4}} ));
	EXPECT_TRUE(rocksIntersect(Rock{ 3, v2{0, 0} }, Rock{ 3, v2{0, -3} }));
	EXPECT_FALSE(rocksIntersect(Rock{ 3, v2{0, 0} }, Rock{ 3, v2{0, -4} }));
	EXPECT_TRUE(rocksIntersect(Rock{ 3, v2{0, 0} }, Rock{ 3, v2{0, 0} }));
	EXPECT_FALSE(rocksIntersect(Rock{ 3, v2{0, 0} }, Rock{ 3, v2{-1, 0} }));
	EXPECT_FALSE(rocksIntersect(Rock{ 3, v2{0, 0} }, Rock{ 3, v2{1, 0} }));
}

TEST(rocks, rock03IntersectionTests) {
	EXPECT_TRUE(rocksIntersect(Rock{ 0, v2{4,1} }, Rock{ 3, v2{4,0} }));
}

TEST(rocks, rock01IntersectionTests) {
	EXPECT_FALSE(rocksIntersect(Rock{ 0, v2{4,2} }, Rock{ 1, v2{2,0} }));
	EXPECT_FALSE(rocksIntersect(Rock{ 0, v2{-1,2} }, Rock{ 1, v2{2,0} }));
	EXPECT_FALSE(rocksIntersect(Rock{ 0, v2{4,0} }, Rock{ 1, v2{2,0} }));
	EXPECT_FALSE(rocksIntersect(Rock{ 0, v2{-1,0} }, Rock{ 1, v2{2,0} }));
	EXPECT_TRUE(rocksIntersect(Rock{ 0, v2{3,2} }, Rock{ 1, v2{2,0} }));
	EXPECT_TRUE(rocksIntersect(Rock{ 0, v2{0,2} }, Rock{ 1, v2{2,0} }));
	EXPECT_TRUE(rocksIntersect(Rock{ 0, v2{3,0} }, Rock{ 1, v2{2,0} }));
	EXPECT_TRUE(rocksIntersect(Rock{ 0, v2{0,0} }, Rock{ 1, v2{2,0} }));
}

TEST(rocks, rock12IntersetionTests) {
	EXPECT_FALSE(rocksIntersect(Rock{ 1, v2{0,0} }, Rock{ 2, v2{2,0} }));
	EXPECT_FALSE(rocksIntersect(Rock{ 2, v2{2,0} }, Rock{ 1, v2{0,0} }));
	EXPECT_TRUE(rocksIntersect(Rock{ 1, v2{0,0} }, Rock{ 2, v2{1,0} }));
}


TEST(rocks, zPartTwo) {
	EXPECT_EQ(0, doTheThing(1000000000, getPuzzleInput()));
}
