#pragma once
#include "Header.h"

template<typename type>
class Array_2D {
public:
	Array_2D() { throw std::runtime_error("Array_2D: cannot initialize without two integers!"); }

	Array_2D(const int& row, const int& col);
	~Array_2D();
	type* get(int i, int j);
	void write(type* pos, type val);
	//	virtual void init() {};
	type** map;
protected:
	type* data;
};

#define CharNum 256

class CharTree;
class index_list;
struct node;
struct item;

struct item {
	~item() {
		if (next_item) {
			//			next_item->~item();
			delete next_item;
			next_item = nullptr;
		}
	}
	int index_number;
	item* next_item{ nullptr };
};


class index_list {
public:
	item* head;
	void add(int index);
	void del(int index);
	void print_index(std::ostream& ost);
	void load_index(std::ifstream& ifs);
	operator bool() {
		return head;
	}
	friend node;
private:
	index_list() {
		head = nullptr;
	}
	~index_list() {
		delete head;
		head = nullptr;
	}
};

struct node {				//字符节点
	node();
	~node();
	node** next;
	index_list list;
	//	item* head;
		//以后考虑改写代码好看些试试
	node*& operator [] (size_t elem) {
		return next[elem];
	}
};


class CharTree {			//字符树的头和操作
public:
	//	CharTree() = default;
	//	~CharTree();
	const node head;
	node* locate(std::string token);
	index_list& access(std::string token);
	node* add_token(std::string token);
	bool one_succ(node** list);
	bool del_token(std::string token);
	void save(std::ofstream& ofs);
	void save_loop(node* current, std::ofstream& ofs);
	void load(std::ifstream& ifs);
	void load_loop(node* current, std::ifstream& ifs);
	void load_loop_start(std::ifstream& ifs);
	//	void load_item_list(node* current, std::ifstream& ifs);

	bool node_found;
	std::string find_node(node* target);
	void find_node_loop(node* target, node* current, std::string& token);

	void print_tokens(std::ostream& ost);
	void print_tokens_loop(node* current, std::string& token, std::ostream& ost);
};


