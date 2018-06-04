#include <iostream>
#include <string>
#include <vector>
#include <limits>

#include "kmp.h"


int main(int argc, char* argv[])
{
	std::string input_str;
	std::string image;
	std::getline(std::cin, image);
	std::getline(std::cin, input_str);
	std::vector<int> ident_points;
	ident_points = kmp(image, input_str);
	print_vec(ident_points);
	system("pause");
	return 0;
}
