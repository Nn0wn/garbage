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
	std::vector<int> naive_points;
	int counter1 = 0;
	int counter2 = 0;
	ident_points = kmp(image, input_str, 1, &counter1);
	std::cout << "KMP substring positions :" << std::endl;
	print_vec(ident_points);
	std::cout << "KMP counter [" << counter1 << "]\n" << std::endl;
	naive_points = naive(image, input_str, &counter2);
	std::cout << "Naive substring positions :" << std::endl;
	print_vec(naive_points);
	std::cout << "Naive counter [" << counter2 << "]" << std::endl;
	system("pause");
	return 0;
}
