//////////////////////////////////////////////////////////////
//Generator for a static tree of nchar to quick search command
//It seems that there is a great increase of size of index if
//the charset is UNICODE because a lot of UNICODE characters are
//not used in commandline but is still created. (from 2mb of output
//to 31kb of output). If the commands are restricted to the alpha-
//numeric, the size can further decrease (a lot). I guess I'll
//use a macro and make some support within this project

#include "Header.h"
#include "cmd_struct.h"
#include "../MyLibrary/DS_CharTree.h"
#include <queue>
#include "../MyLibrary/DS_cycQueue.h"
#ifdef _UNICODE
#include "../Host/WinPlatform.h"
#endif

//#include "../commands/commands_uncompressed.h"

using namespace std;

helper_tree tree;

ifstream ifs;
ofstream ofs;

bool make_original_cmd() {
	ifs.open("macro_commands.h");
	ofs.open("commands_uncompressed.h");
	if(!ifs || !ofs) {
		return false;
	}
	string line;
	while(!ifs.eof() && ifs) {
		getline(ifs, line);
		istringstream iss{line};
		string preprocessor;
		string command;
		int command_number;
		iss >> preprocessor >> command >> command_number;
		if(preprocessor == "#define") {
#ifdef _UNICODE
			const nstring temp = convert::string2wstring(command);
			tree.add_token(temp);
			tree.access(temp) = command_number;
#endif
#ifdef _MBCS
			tree.add_token(command);
			tree.access(command) = command_number;
#endif
		}
	}
	tree.make_static_list(ofs);
	ifs.close();
	ofs.close();
	return true;
}

int main() {
	make_original_cmd();
	return 0;
}


//since the queue size can be known at runtime before pushing and popping, it may be better to 
//allocate a simple fixed array only once for this purpose
//note:	after using the fixed queue, heap size reduced from 35mb to 2mb when processing as unicode
//		using Visual C++, and greatly increased speed (the main operations where the push and pop)
//		operations which requires memory allocation at runtime if overflows. This is frequent if
//		the storage space required is rather large
void helper_tree::make_static_list(std::ostream& ost) {
	node* cur_node = head;
	num_of_node = 0;
	nstring token = nstring();
	root_first_recurse_loop(cur_node, token, &num_of_node, calc_num_of_data);
	Cyclic_Queue<node*> visit_queue{num_of_node* (find_MAX<nchar>() + 1) };
	Cyclic_Queue<unsigned long> index_queue{num_of_node* (find_MAX<nchar>() + 1) };
	num_of_node = 0;
	if (cur_node) num_of_node++;
	do {
		if(visit_queue) visit_queue.pop_front();
		if(cur_node) {
			index_queue.push_back(cur_node->data);
			if(cur_node->next) {
//				size_t num_of_new_nodes = 0; //number of new nodes to be pushed before the current one
				for(size_t i = 0; i < find_MAX<nchar>(); i++) {
					if((cur_node->next[i])) {
						visit_queue.push_back(cur_node->next[i]);
						index_queue.push_back(unsigned long((num_of_node) * (find_MAX<nchar>() + 1)));
						num_of_node++;
					} else {
						index_queue.push_back(0);
					}
				}
			}else {
				for (size_t i = 0; i < find_MAX<nchar>(); i++) {
					index_queue.push_back(0);
				}
			}
		}
		if(visit_queue) {
			cur_node = visit_queue.front();
		}
	}
	while(!visit_queue);
	unsigned long* full_list = new unsigned long[num_of_node * (find_MAX<nchar>() + 1)];
	for(size_t i = 0; i < num_of_node * (find_MAX<nchar>() + 1);i++) {
		full_list[i] = index_queue.front();
		index_queue.pop_front();
		if (!index_queue) break;
	}
	ost << "#pragma once\n";
	ost << endl;
	ost << "unsigned long commands[" << num_of_node * (find_MAX<nchar>() + 1) << "] {\n";
	size_t cur_index = 0;
	for(size_t i = 0; i < num_of_node; i++) {
		ost << "\t" << full_list[cur_index] <<','<< endl;
		cur_index++;
		for(size_t j = 0; j < find_SQRT_OF_MAX<nchar>(); j++) {
			ost << "\t";
			for(size_t k = 0; k < find_SQRT_OF_MAX<nchar>(); k++) {
				ost << full_list[cur_index];
				if(!(cur_index == num_of_node * (find_MAX<nchar>() + 1) - 1))ost << ", ";
				cur_index++;
			}
			ost << endl;
		}
		for(size_t l = 0; l < (find_MAX<nchar>() - (find_SQRT_OF_MAX<nchar>() * find_SQRT_OF_MAX<nchar>())); l++) {
			ost << "\t" << full_list[cur_index];
			if(!(cur_index == num_of_node * (find_MAX<nchar>() + 1) - 1))ost << ", ";
		}
	}
	ost << "};\n" << endl;
	delete[] full_list;
}
