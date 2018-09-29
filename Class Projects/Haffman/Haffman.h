#pragma once

#include "Header.h"

struct node {
	node() {
		lchild = -1;
		rchild = -1;
		parent = -1;
//		leaf = true;
	}
	int lchild;
	int rchild;
	int parent;
	size_t weight{ 0 };
//	bool leaf;
	void print(std::ostream& ost);
};

class Haffman {
#define MAX_CHAR 256
#define NUM_NODE 511
public:
	static void prompt() {
		std::cout << "Simple program for simulating Haffman encoding.\n";
		std::cout << "v0.0.0.0\n" << std::endl;
	}
	Haffman() {
		for(int i=0; i<NUM_NODE; i++) {
			tree[i] = node();
		}
		init();
	}
	node tree[NUM_NODE];
	unsigned short root;

	unsigned short find_smallest(short range);
	void init();
	void init_tree();
	void print_tree();

	void encode(std::istream& input, std::ostream& output);
	void decode(std::istream& input, std::ostream& output);
	void encode_char(unsigned char ch, std::ostream& output);

	void UI();
	std::string parse_path(std::string line);
	void encode();
	void decode();
	void print_code();
	bool conti{true};
};

extern bool on;