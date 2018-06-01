#include "help_func.h"

bool cmp_iter(const char& a, const char& b, int* counter)
{
	if (counter != nullptr)
		(*counter)++;
	return a == b;
}

std::vector<int> naive(const std::string& image, const std::string& input_str, int* counter)
{
	int j = 0;
	std::vector<int> naive_points;
	size_t im_size = image.size(), inp_size = input_str.size(), k = 0;
	for (size_t i = 0; i <= (inp_size - im_size); i++)
	{
		k = 0;
		while (cmp_iter(input_str[i + k], image[j + k], counter) && (k < im_size))
		{
			k++;
		}
		if (k == im_size)
			naive_points.push_back(i);
	}
	return naive_points;
}

void print_vec(std::vector<int>& vec)
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