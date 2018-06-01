#include "bohr.h"

int get_move(int vertex, char ch);

std::vector<bohr_vertex> bohr;

bohr_vertex create_vertex(int i_parent, char letter)
{
	bohr_vertex elem;
	elem.next[letter] = -1;
	elem.move[letter] = -1;
	elem.parent = i_parent;
	elem.symb = letter;
	elem.word = false;
	elem.suff_link = -1;
	elem.good_suff_link = -1;
	return elem;
}

void bohr_ini()
{
	bohr.clear();
	bohr.push_back(create_vertex(0, '#'));
}

void add_string(const std::string input_str, std::vector<std::string> pattern)
{
	int num = 0;
	for (size_t i = 0; i<input_str.length(); i++)
	{
		char ch = input_str[i];
		if (bohr[num].next.find(ch) == bohr[num].next.end() || bohr[num].next[ch] == -1)
		{
			bohr.push_back(create_vertex(num, ch));
			bohr[num].next[ch] = bohr.size() - 1;
		}
		num = bohr[num].next[ch];
	}
	bohr[num].word = true;
	pattern.push_back(input_str);
	bohr[num].pattern_num.push_back(pattern.size() - 1);
}

bool check_string(const std::string& input_str)
{
	int num = 0;
	for (size_t i = 0; i<input_str.length(); i++)
	{
		char ch = input_str[i] - 'A';
		if (bohr[num].next[ch] == -1)
		{
			return false;
		}
		num = bohr[num].next[ch];
	}
	return true;
}

int get_suff_link(int vertex)
{
	if (bohr[vertex].suff_link == -1)
	{
		if (vertex == 0 || bohr[vertex].parent == 0)
			bohr[vertex].suff_link = 0;
		else
			bohr[vertex].suff_link = get_move(get_suff_link(bohr[vertex].parent), bohr[vertex].symb);
	}
	return bohr[vertex].suff_link;
}

int get_move(int vertex, char ch)
{
	if (bohr[vertex].move.find(ch) == bohr[vertex].move.end() || bohr[vertex].move[ch] == -1)
	{
		if (bohr[vertex].next.find(ch) != bohr[vertex].next.end() && bohr[vertex].next[ch] != -1)
			bohr[vertex].move[ch] = bohr[vertex].next[ch];
		else
		{
			if (vertex == 0)
			{
				bohr[vertex].move[ch] = 0;
			}
			else
			{
				bohr[vertex].move[ch] = get_move(get_suff_link(vertex), ch);
			}
		}
	}
	return bohr[vertex].move[ch];
}

int get_good_suff_link(int vertex)
{
	if (bohr[vertex].good_suff_link == -1)
	{
		int good_link = get_suff_link(vertex);
		if (good_link == 0)
			bohr[vertex].good_suff_link = 0;
		else
			bohr[vertex].good_suff_link = (bohr[good_link].word) ? good_link : get_good_suff_link(good_link);
	}
	return bohr[vertex].good_suff_link;
}


void check_good_suff_stand(int vertex, int i, const std::vector<std::string>& pattern, std::map<int, std::vector<int >> &vals)
{
	for (int u = vertex; u != 0; u = get_good_suff_link(u))
	{
		if (bohr[u].word)
		{
			vals[i - pattern[bohr[u].pattern_num[0]].size() + 1].push_back(bohr[u].pattern_num[0] + 1);
			//pos_str val;
			//val.pos = i - pattern[bohr[u].pattern_num[0]].size() + 1;
			//val.num_str = bohr[u].pattern_num[0] + 1;
			//vals.push_back(val);
		}
	}
}


void find_all_pos_stand(const std::string& input_str, const std::vector<std::string>& pattern, std::map<int, std::vector<int >> &vals)
{
	int u = 0;
	for (size_t i = 0; i<input_str.size(); i++)
	{
		u = get_move(u, input_str[i]);
		check_good_suff_stand(u, i + 1, pattern, vals);
	}
}

void check_good_suff_joker(int vertex, int i, std::vector<int>& count, const std::vector<int>& lenght)
{
	for (int u = vertex; u != 0; u = get_good_suff_link(u))
	{
		if (bohr[u].word)
		{
			for (const auto& it : bohr[u].pattern_num)
			{
				if ((size_t)(i - lenght[it])<count.size())
				{
					count[i - lenght[it]]++;
				}
			}
		}
	}
}

void find_all_pos_joker(const std::string& input_str, std::vector<int>& count, const std::vector<int>& lenght)
{
	int u = 0;
	for (size_t i = 0; i < input_str.size(); i++)
	{
		u = get_move(u, input_str[i]);
		check_good_suff_joker(u, i + 1, count, lenght);
	}
}

std::vector<int> divider(std::stringstream& str_stream, char spl_ch, const std::vector<std::string> pattern)
{
	std::vector<int> lenght;
	int len = 0;
	std::string tmp;
	while (getline(str_stream, tmp, spl_ch))
	{
		if (tmp.size()>0)
		{
			len += tmp.size();
			lenght.push_back(len);
			add_string(tmp, pattern);
		}
		len++;
	}
	return lenght;
}