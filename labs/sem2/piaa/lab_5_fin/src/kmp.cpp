#include "kmp.h"

std::vector<int> kmp(const std::string& image, const std::string& input_str, int multipler, int* counter)
{
	size_t i = 0, j = 0, inp_size = input_str.size();
	std::vector<int> ident_points;
	std::vector<int> vals = prefix_vec(image, counter);
	while (i < input_str.size() * multipler)
	{
		while (j > 0 && !cmp_iter(image[j], input_str[i%inp_size], counter))
			j = vals[j - 1];
		if (cmp_iter(image[j], input_str[i%inp_size], counter))
			j++;
		if (j == image.size())
		{
			ident_points.push_back(i - j + 1);
			j = vals[j - 1];
		}
		i++;
	}
	return ident_points;
}

std::vector<int> prefix_vec(const std::string& image, int* counter)
{
	int j = 0;
	std::vector<int> values;
	size_t im_size = image.size();
	values.push_back(0);
	for (size_t i = 1; i < im_size; ++i)
	{
		j = values[i - 1];
		while ((j > 0) && !cmp_iter(image[i], image[j], counter))
		{
			j = values[j - 1];
		}
		if (cmp_iter(image[i], image[j], counter))
		{
			++j;
		}
		values.push_back(j);
	}
	return values;
}
