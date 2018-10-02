#pragma once

#include "../Lib/Utilities.h"
#include "../Lib/DS_CharTree.h"
#include "../Host/WinPlatform.h"

typedef struct {
	unsigned int next{};
	unsigned int cmd{};
} cmd_node;

template<typename type>
struct cmd_page {
	cmd_node data[find_MAX<type>()];
};

class helper_tree: public CharTree<nchar, int> {
public:
	void make_static_list(std::ostream& ost);
	size_t num_of_node;
};



