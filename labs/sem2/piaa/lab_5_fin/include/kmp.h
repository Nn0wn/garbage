//#pragma once
#ifndef KMP_H
#define KMP_H
#include <vector>
#include <iostream>

#include "help_func.h"

std::vector<int> prefix_vec(const std::string& image, int* counter = nullptr);
std::vector<int> kmp(const std::string& image, const std::string& input_str, int multipler = 1, int* counter = nullptr);



#endif
