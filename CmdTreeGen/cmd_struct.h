#pragma once

#include "../MyLibrary/DS_CharTree.h"
#include <tchar.h>

#define NCharTree(type) Basic_Type_Tree<nchar, type>

inline void calc_num_of_data(int& content, void* passed, const std::basic_string<nchar>& token) {
	(*(size_t*)passed)++;
}

class helper_tree: public NCharTree(int) {
public:
	void make_static_list(std::ostream& ost);
	size_t num_of_node;
};



