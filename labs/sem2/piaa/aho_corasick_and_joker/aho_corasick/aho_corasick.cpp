// aho_korasik.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "bohr.h"


int main()
{
	/*initBohr();
	string text, temp;
	char razd;
	cin >> text >> temp >> razd;
	stringstream str_pat(temp);
	vector<int> count(text.size(), 0); //вектор, содержащий счётчики вхождений частей шаблона
	vector<string> pattern; //вектор для частей шаблона
	vector<int> lenght = spliter(str_pat, razd, pattern);//места, где части шаблона заканчиваются в самом шаблоне
	findAllPosForDjoker(text, count, lenght);
	printDj(count, text.size() - temp.size() + 1, pattern.size());*/
	/*std::string text, image;
	char joker;
	std::getline(std::cin, text);
	std::getline(std::cin, image);
	std::cin >> joker;
	std::stringstream str_stream(image);
	std::vector<int> count(text.size(), 0);
	std::vector<std::string> pattern;
	bohr_ini();
	std::vector<int> lenght = spliter(str_stream, joker, pattern);
	find_all_pos_joker(text, count, lenght);
	for (size_t i = 0; i < text.size() - image.size() + 1; i++)
	{
		if (count[i] == pattern.size())
			std::cout << i + 1 << std::endl;
	}*/
	size_t num_of_strs = 0;
	std::string base_string;
	std::vector<pos_str> vec;
	std::vector<std::string> pattern;
	std::getline(std::cin, base_string);
	std::cin >> num_of_strs;
	std::cin.ignore();
	bohr_ini();
	for (size_t i = 0; i < num_of_strs; i++)
	{
		std::string temp;
		std::getline(std::cin, temp);
		//pattern.push_back(temp);
		add_string(temp, pattern);
	}
	//find_all_pos(base_string);
	find_all_pos_stand(base_string, pattern, vec);
	for (auto el : vec)
	{
		std::cout << el.pos << " " << el.num_str << std::endl;
	}
	system("pause");
    return 0;
}

