// list.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "my_list.h"
#include <iostream>


int main()
{
	stepik::list<size_t> new_one;
	new_one.push_back(1);
	new_one.push_back(2);
	new_one.print();
	stepik::list<size_t> second_one(new_one);
	second_one.print();
	system("pause");
    return 0;
}

