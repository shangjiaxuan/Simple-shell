#pragma once

#include "../Library/DS_CharTree.h"

#ifdef _UNICODE
#define NCharTree WCharTree
#endif
#ifdef _MBCS
#define NCharTree CharTree
#endif
class helper_tree: public NCharTree<int> {
public:
	void make_static_list(std::ostream& ost);
	size_t num_of_node;
	void print_useful_content(node* current, std::ostream& ost, const std::basic_string<nchar>& token) override {
		num_of_node++;
	}
};



