#include <CORE_TWO.h>

#include <array>

using namespace CORE_TWO;

int main() {
	std::array<std::string, 3> scoords{
		"11110100011111101110101010110011001000100110101110111110010111101010010001001100010000000101101011101011110101000010011110011000101100111001110100001000111110100100111101000110001011000010011011010010101110111000E-215",
		"-11110100011111101110101010110011001000100110101110111110010111101010010001001100010000000101101011101011110101000010011110011000101100111001110100001000111110100100111101000110001011000010011011010010101110111000E-215",
		"-10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000E1"
	};
	std::array<Expr, 3> ptc{
		BigFloat(scoords[0], 2),
		BigFloat(scoords[1], 2),
		BigFloat(scoords[2], 2)
	};
	
	//normalize input
	{
		Expr len{0};
		for(auto & x : ptc) {
			len += x*x;
		}
		len = sqrt(len);
		for(auto & x : ptc) {
			x /= len;
		}
	}
	if (-ptc[1] > ptc[0]) {
		std::cout << "Working" << std::endl;
	}
	return 0;
};