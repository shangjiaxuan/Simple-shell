#pragma once

#include "Header.h"

extern bool on;

struct index_block {
	int* data;
	size_t start;
	size_t block_size;
	int max=std::numeric_limits<int>::lowest();
	int& operator[](size_t i) {
		if (!(i<block_size)) {
			throw std::runtime_error("int& index_block::operator[](size_t): index too large!");
		}
		return data[start + i];
	}
};

class search {
public:
	search();
	~search();
	bool conti{ true };
	static void name_ver();
	size_t block_num;
	index_block* index;
	int* data;
	void print(std::ostream& ost);
	size_t search_index(int code);
	long long search_block(size_t block, int code);
	struct coordinate{
		size_t block;
		long long place;
	};
	coordinate search_code(int code);
	void print_result(std::ostream& ost, const coordinate& cor);
	void UI();
	void search_cmd();
};

std::string parse_path(std::string line);
