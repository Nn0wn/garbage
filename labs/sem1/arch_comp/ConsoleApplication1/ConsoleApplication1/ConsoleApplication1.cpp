// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include <vector>

using namespace std;

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	const int N = 80;
	//vector<char> eng_vector('A','z');
	//vector<char> rus_vector(0xC1);
	char str1[N+1];
	char str2[N+1];
	int i = 0;
	unsigned int temp = 0;
	cout << "Лабораторная работа 4\nФормирование выходной строки только из русских и латинских букв входной "
		"строки.\nАвтор: Виноградов К.А. гр. 6304\n" << endl;
	cout << "Введите строку: ";
	cin.getline(str1, N, '\n');
	_asm{
			lea esi, str1
			lea edi, str2
			let_char:
			lodsb
				cmp al, 0
				je end_char
				jg more_zero
				cmp al, 'Ё'
				je print_e_lrg
				cmp al, 'ё'
				je print_e_sm
				cmp al, 'А'
				jl let_char
				cmp al, 'Е'
				jle print_AE
				cmp al, 'Ш'
				jle print_ZHSH
				cmp al, 'Я'
				jle print_vosk
				cmp al, 'е'
				jle print_AE
				cmp al, 'ш'
				jle print_ZHSH
				cmp al, 'я'
				jle print_vosk
				jmp let_char
				more_zero:
				cmp al, 'A'
				jl let_char
				cmp al, 'F'
				jle print_AF
				cmp al, 'G'
				je print_g_lrg
				cmp al, 'Z'
				jle print_HZ
				cmp al, 'a'
				jl let_char
				cmp al, 'f'
				jle print_AF
				cmp al, 'g'
				je print_g_sm
				cmp al, 'z'
				jle print_HZ
				jmp let_char
			print_e_lrg:
				add al, 159
				stosb
				jmp let_char
			print_e_sm :
				add al, 175
				stosb
				jmp let_char
			print_AE :
				add al, 129
				stosb
				jmp let_char
			print_ZHSH :
				add al, 130
				stosb
				jmp let_char
			print_vosk :
				mov al, 33
				stosb
				jmp let_char
			print_AF :
				add al, -129
				stosb
				jmp let_char
			print_g_lrg :
				add al, -159
				stosb
				jmp let_char
			print_HZ :
				add al, -130
				stosb
				jmp let_char
			print_g_sm :
				add al, -175
				stosb
				jmp let_char
			end_char:
				stosb
	}
	cout << "Полученная строка: ";
	ofstream output;
	output.open("output.txt");
	while (str2[i] != '\0')
	{
		output << str2[i];
		cout << str2[i];
		i++;
	}
	output << endl;
	cout << endl;
	output.close();
	system("pause");
    return 0;
}

