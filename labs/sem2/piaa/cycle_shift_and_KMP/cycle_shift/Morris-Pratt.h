#pragma once
#include <iostream>
#include <string>
#include <vector>

std::vector<int> cycle_shift(std::string image, std::string input_str);
std::vector<int> prefix_vec(std::string image);
std::vector<size_t> in_points_vec(std::vector<size_t> ident_points, std::vector<size_t> ident_val, std::string image, std::string input_str);

/*int cycle_shift(std::vector<size_t> ident_points, std::vector<size_t> ident_val, std::string input_str, std::string image)
{
	int pos = -1;
	std::string temp_pref;
	std::string temp_postf;
	temp_pref = image;
	for (int i = image.size() - 1; i > 0; i--)
	{
		ident_val.clear(); ident_points.clear();
		temp_postf.insert(temp_postf.begin(), temp_pref.back());
		temp_pref.pop_back();
		ident_val = prefix_vec(ident_val, temp_pref);
		ident_points = in_points_vec(ident_points, ident_val, temp_pref, input_str);
		for (auto elems : ident_points)
		{
			if (elems + temp_pref.size() == input_str.size())
			{
				ident_val.clear(); ident_points.clear();
				ident_val = prefix_vec(ident_val, temp_postf);
				ident_points = in_points_vec(ident_points, ident_val, temp_postf, input_str);
				if (ident_points.size() != 0 && ident_points[0] == 0)
				{
					//std::cout << "yes" << std::endl;
					pos = elems;
					i = 0;
				}
			}
		}
	}
	return pos;
}*/

std::vector<int> prefix_vec(std::string image)
{
	int j = 0;
	std::vector<int> values;
	values.push_back(0);
	for (int i = 1; i < image.size(); ++i)
	{
		j = values[i - 1];
		while ((j > 0) && (image[i] != image[j]))
		{
			j = values[j - 1];
		}
		if (image[i] == image[j])
		{
			++j;
		}
		values.push_back(j);
	}
	return values;
}

std::vector<int> cycle_shift(std::string image, std::string input_str)
{
	int i = 0, j = 0;
	int im_sz = image.size();
	std::vector<int> points;
	std::vector<int> values = prefix_vec(input_str);
	for (i = 0; i < (im_sz * 2); i++)
	{
		while (j > 0 && image[i%im_sz] != input_str[j])
			j = values[j - 1];
		if (image[i%im_sz] == input_str[j])
			j++;
		if (j >= input_str.size())
		{
			points.push_back(i - j + 1);
			//j = ident_val[j - 1];
		}
	}
	return points;
}


std::vector<size_t> in_points_vec(std::vector<size_t> ident_points, std::vector<size_t> ident_val, std::string image, std::string input_str)
{
	size_t i = 0, j = 0;
	while (i < input_str.size())
	{
		while (j > 0 && image[j] != input_str[i])
			j = ident_val[j - 1];
		if (image[j] == input_str[i])
			j++;
		if (j >= image.size())
		{
			ident_points.push_back(i - j + 1);
			j = ident_val[j - 1];
		}
		i++;
	}
	return ident_points;
}