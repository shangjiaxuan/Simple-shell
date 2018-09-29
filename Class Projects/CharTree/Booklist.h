#pragma once

#include "Header.h"

class Book;
class list;

//所有书目中一本书对应的链表节点
struct volume {
	volume() {
		index_number = -2;
		volume_next = nullptr;
	}
	//利用delete递归调用析构函数的特点一次性释放书目链表
	~volume() {
		if (volume_next) {
//			volume_next->~volume();
			delete volume_next;
			volume_next = nullptr;
		}
	}
	int index_number;
	bool fixed_index{ false };
	std::string name;
	volume* volume_next;
};

class list {
	list() {
		head = nullptr;
	}
	~list() {
		if(head) {
//			head->~volume();
			delete head;
//			head = nullptr;
		}
	}
	friend Book;
	struct found;
	volume* head;

	int add(std::string name);
	int add(int index, std::string name);
	void add_head(int index, std::string name, bool fixed);
	void change_head(int index, std::string name, bool fixed);

//	int default_index();
	void add(volume* previous, int index, std::string name, bool index_type);
	//return value is the reindexed default index
	int reindex(found original, volume* added);

	std::string del(int index);
	int del(std::string name);

	void print(std::ostream& ost);
	void save(std::ofstream& ofs);
	void load(std::ifstream& ifs);

	void print_book(int index, std::ostream& ost);
	void print_book(std::string name, std::ostream& ost);
	void print_book(volume* book, std::ostream& ost);

	struct found {
		//the status of the found item
		//legend:
		//N: head==nullptr
		//L: the specified index_number is larger than all current ones, rtn is the tail pointer
		//S: the specified index_number is smaller than all current ones, rtn is the head pointer
		//M: the specified index_number is in the middle of current ones, rtn is the largest one smaller than the specified index_number
		//P: the specified index_number is found at rtn;
		//X: book name string not found
		char status;
		volume* rtn;
		volume* traceback;
		std::string name;
		//the default index for new item
		int default_index;
	};
	found find(int index);
	found find(std::string name);
	found find(volume* start, int index);
	volume* find_default();
};

