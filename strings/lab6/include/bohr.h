//#pragma once
#ifndef BOHR_H
#define BOHR_H

struct bohr_vertex
{
	std::map<char, int> next;
	std::map<char, int> move;
	std::vector<int> pattern_num;
	int parent;
	char symb;
	bool word;
	int suff_link;
	int good_suff_link;
};


void bohr_ini();
void add_string(const std::string input_str, const std::vector<std::string> pattern);
bool check_string(const std::string& input_str);
int get_suff_link(int vertex);
int get_move(int vertex, char ch);
int get_good_suff_link(int vertex);
void check_good_suff_stand(int vertex, int i, const std::vector<std::string>& pattern, std::map<int, std::vector<int>>& vals);
void find_all_pos_stand(const std::string& input_str, const std::vector<std::string>& pattern, std::map<int, std::vector<int>>& vals);
void check_good_suff_joker(int vertex, int i, std::vector<int>& count, const std::vector<int>& lenght);
void find_all_pos_joker(const std::string& input_str, std::vector<int>& count, const std::vector<int>& lenght);
std::vector<int> divider(const std::stringstream& str_stream, char spl_ch, const std::vector<std::string> pattern);

#endif