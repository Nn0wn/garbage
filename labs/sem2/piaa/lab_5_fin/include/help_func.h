//#pragma once
#ifndef HELP_FUNC_H
#define HELP_FUNC_H
#include <iostream>
#include <string>
#include <vector>

bool cmp_iter(const char& a, const char& b, int* counter = nullptr);
std::vector<int> naive(const std::string& image, const std::string& input_str, int* counter = nullptr);
void print_vec(std::vector<int>& vec);

#endif