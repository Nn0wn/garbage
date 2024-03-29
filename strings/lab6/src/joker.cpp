#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <map>
#include "bohr.h"


int main()
{
	std::string text, image;
	char joker;
	std::getline(std::cin, text);
	std::getline(std::cin, image);
	std::cin >> joker;
	std::stringstream str_stream(image);
	std::vector<int> count(text.size(), 0);
	std::vector<std::string> pattern;
	bohr_ini();
	std::vector<int> lenght = divider(str_stream, joker, pattern);
	find_all_pos_joker(text, count, lenght);
	for (size_t i = 0; i < text.size() - image.size() + 1; i++)
	{
		if (count[i] == pattern.size())
			std::cout << i + 1 << std::endl;
	}
	system("pause");
    return 0;
}