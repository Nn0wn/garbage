#pragma once
#include <iostream>
#include <string>
#include <vector>

void print_vec(std::vector<size_t> vec)
{
	if (vec.size() != 0)
	{
		for (size_t i = 0; i < vec.size() - 1; i++)
		{
			std::cout << vec[i] << ',';
		}
		std::cout << vec.back() << std::endl;
	}
	else
		std::cout << "-1" << std::endl;
}