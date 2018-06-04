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
	if (image.size() == input_str.size())
	{
		ident_points = kmp(input_str, image, 2);
		print_vec(ident_points);
	}
	system("pause");
	return 0;
}
