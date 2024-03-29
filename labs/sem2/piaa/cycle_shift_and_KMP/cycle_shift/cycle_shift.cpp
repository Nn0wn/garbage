// cycle_shift.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "Morris-Pratt.h"
#include "help_func.h"
#include <iostream>
#include <string>
#include <vector>
#include <limits>


int main()
{
	std::string input_str;
	std::string image;
	std::getline(std::cin, image);
	std::getline(std::cin, input_str);
	std::vector<int> ident_val;
	std::vector<int> ident_points;
	if (image.size() == input_str.size())
	{
		ident_points = cycle_shift(image, input_str);
		if (ident_points.size() != 0)
			std::cout << ident_points[0] << std::endl;
		else
			std::cout << "-1" << std::endl;
	}
	system("pause");
	return 0;
}
