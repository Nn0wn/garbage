#pragma once


/*struct Bohr {
	map<char, int> nextV; //nextV[i] — номер вершины, в которую мы придем по символу с номером i в алфавите
	vector<int> pat_num; //pat_num — номер строки-образца, обозначаемого этой вершиной
	int suff_link; //suff_link - суффиксная ссылка
	map<char, int> auto_move; //auto_move - запоминание перехода автомата
	int parent; //parent - вершина-отец в дереве
	int suff_flink; //suff_flink - "хорошая" суф. ссылка
	bool flag; //flag — бит, указывающий на то, является ли наша вершина исходной строкой
	char symb; //символ на ребре от parent к этой вершине
};

Bohr makeBohr(int p, char c);
void initBohr();
void addstrBohr(const string& s, vector<string>& pattern);
int getAuto_move(int v, char ch);
int getSuff_link(int v);
int getSuff_flink(int v);
void checkForStandart(int v, int i, const vector<string>& pattern, map<int, vector<int>>& answ);
void checkForDjoker(int v, int i, vector<int>& count, const vector<int>& lenght);
void findAllPosForStandart(const string& s, const vector<string>& pattern, map<int, vector<int>>& answ);
void findAllPosForDjoker(const string& s, vector<int>& count, const vector<int>& lenght);
vector<int> spliter(stringstream& str_pat, char razd, vector<string>& pattern);

vector<Bohr> bohr;

Bohr makeBohr(int p, char c) { //Функции создания новой вершины и инициализации бора
	Bohr v;
	v.nextV[c] = v.auto_move[c] = -1;
	v.flag = false;
	v.suff_link = -1; //изначально - суф. ссылки нет
	v.parent = p; //передаем номер отца и символ на ребре в бор
	v.symb = c;
	v.suff_flink = -1;
	return v;
}

void initBohr() {
	bohr.clear();
	bohr.push_back(makeBohr(0, '$'));
}

void addstrBohr(const string& s, vector<string>& pattern) { //Процедура добавление строки-образца в бор
	int num = 0;
	for (size_t i = 0; i < s.length(); ++i) {
		char ch = s[i];
		if (bohr[num].nextV.find(ch) == bohr[num].nextV.end() || bohr[num].nextV[ch] == -1) {
			bohr.push_back(makeBohr(num, ch));
			bohr[num].nextV[ch] = bohr.size() - 1;
		}
		num = bohr[num].nextV[ch];
	}
	bohr[num].flag = true;
	pattern.push_back(s);
	bohr[num].pat_num.push_back(pattern.size() - 1);
}

int getSuff_link(int v) {
	if (bohr[v].suff_link == -1) { //если еще не считали
		if (v == 0 || bohr[v].parent == 0) //если v - корень или предок v - корень
			bohr[v].suff_link = 0;
		else
			bohr[v].suff_link = getAuto_move(getSuff_link(bohr[v].parent), bohr[v].symb);
	}
	return bohr[v].suff_link;
}

int getAuto_move(int v, char ch) {
	if (bohr[v].auto_move.find(ch) == bohr[v].auto_move.end() || bohr[v].auto_move[ch] == -1) {
		if (bohr[v].nextV.find(ch) != bohr[v].nextV.end() && bohr[v].nextV[ch] != -1)
			bohr[v].auto_move[ch] = bohr[v].nextV[ch];
		else {
			if (v == 0)
				bohr[v].auto_move[ch] = 0;
			else
				bohr[v].auto_move[ch] = getAuto_move(getSuff_link(v), ch);
		}
	}
	return bohr[v].auto_move[ch];
}

int getSuff_flink(int v) {
	if (bohr[v].suff_flink == -1) {
		int u = getSuff_link(v);
		if (u == 0)
			bohr[v].suff_flink = 0;
		else
			bohr[v].suff_flink = (bohr[u].flag) ? u : getSuff_flink(u);
	}
	return bohr[v].suff_flink;
}

void checkForStandart(int v, int i, const vector<string>& pattern, map<int, vector<int>>& answ) {
	for (int u = v; u != 0; u = getSuff_flink(u)) {
		if (bohr[u].flag) {
			answ[i - pattern[bohr[u].pat_num[0]].size() + 1].push_back(bohr[u].pat_num[0] + 1);
		}
		// cout<<i - pattern[bohr[u].pat_num[0]].size() + 1<< " "<< bohr[u].pat_num[0] + 1 << endl;
	}
}

void checkForDjoker(int v, int i, vector<int>& count, const vector<int>& lenght) {
	for (int u = v; u != 0; u = getSuff_flink(u)) {
		if (bohr[u].flag) {
			for (const auto& it : bohr[u].pat_num) {
				if ((size_t)(i - lenght[it])<count.size()) {
					count[i - lenght[it]]++;
				}
			}
		}
	}
}

void findAllPosForStandart(const string& s, const vector<string>& pattern, map<int, vector<int>>& answ) {
	int u = 0;
	for (size_t i = 0; i < s.size(); i++) {
		u = getAuto_move(u, s[i]);
		checkForStandart(u, i + 1, pattern, answ);

	}
}

void findAllPosForDjoker(const string& s, vector<int>& count, const vector<int>& lenght) {
	int u = 0;
	for (size_t i = 0; i < s.size(); i++) {
		u = getAuto_move(u, s[i]);
		checkForDjoker(u, i + 1, count, lenght);
	}
}

vector<int> spliter(stringstream& str_pat, char razd, vector<string>& pattern) {
	vector<int> lenght;
	int len = 0;
	string tmp;
	while (getline(str_pat, tmp, razd)) {
		if (tmp.size()>0) {
			len += tmp.size();
			lenght.push_back(len);
			addstrBohr(tmp, pattern);
		}
		len++;
	}
	return lenght;
}

void printDj(const vector<int>& count, int stopper, int pattern_size) {
	for (int i = 0; i<stopper; i++) {
		if (count[i] == pattern_size)
			cout << i + 1 << "\n";
	}
}*/

struct bohr_vertex
{
	std::map<char, int> next;
	std::map<char, int> move;
	std::vector<int> pattern_num;
	int parent;
	char symb;
	//int pattern_num;
	bool word;
	int suff_link;
	int good_suff_link;
};

struct pos_str
{
	int pos;
	int num_str;
};

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

void add_string(const std::string& input_str, std::vector<std::string>& pattern)
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
	//bohr[num].pattern_num = pattern.size() - 1;
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

/*void check_good_suff(int vertex, int i)
{
	for (int u = vertex; u != 0; u = get_good_suff_link(u))
	{
		if (bohr[u].word)
		{
			std::cout << i - pattern[bohr[u].pattern_num].length() + 1 << " " << bohr[u].pattern_num + 1 << std::endl;
		}
	}
}*/

void check_good_suff_stand(int vertex, int i, std::vector<std::string>& pattern, std::vector<pos_str>& vals)
{
	for (int u = vertex; u != 0; u = get_good_suff_link(u))
	{
		if (bohr[u].word)
		{
			pos_str val;
			val.pos = i - pattern[bohr[u].pattern_num[0]].size() + 1;
			val.num_str = bohr[u].pattern_num[0] + 1;
			vals.push_back(val);
			//std::cout << i - pattern[bohr[u].pattern_num].length() + 1 << " " << bohr[u].pattern_num + 1 << std::endl;
		}
	}
}

/*void find_all_pos(const std::string input_str)
{
	int u = 0;
	for (size_t i = 0; i<input_str.length(); i++)
	{
		u = get_move(u, input_str[i] - 'a');
		check_good_suff(u, i + 1);
	}
}*/

void find_all_pos_stand(const std::string& input_str, std::vector<std::string>& pattern, std::vector<pos_str>& vals)
{
	int u = 0;
	for (size_t i = 0; i<input_str.size(); i++)
	{
		//std::vector<pos_str> temp;
		u = get_move(u, input_str[i]);
		check_good_suff_stand(u, i + 1, pattern, vals);
		//for (auto el : check_good_suff_stand(u, i + 1, pattern, vals))
			//temp.push_back(el);
		//for (auto el : temp)
			//vals.push_back(el);
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

std::vector<int> spliter(std::stringstream& str_stream, char spl_ch, std::vector<std::string>& pattern)
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