#pragma once
#include "Header.h"

class Index;

struct volume {
	int index;
	bool fixed_index{ false };
	std::string name;
	volume* next;
};

class Book{
public:
	~Book();
	volume* head;
	bool add(int index, std::string name);
	//if false, the index is already assigned as fixed
	//the current book using the index is not assigned as fixed, will return true 
	//and call the corresponding functions to fix the index in the Index class
	//(what really matters)
	bool add(std::string name);
	//if false, the name's already taken.
	void del(int index);
	void del(std::string name);
	friend Index;
private:
	struct found {
		char status;
		//N: head==nullptr
		//L: the specified index is larger than all current ones, rtn is the tail pointer
		//S: the specified index is smaller than all current ones, rtn is the head pointer
		//M: the specified index is in the middle of current ones, rtn is the largest one smaller than the specified index
		//P: the specified index is found at rtn;
		//X: book name not found
		volume* rtn;
		std::string name;
	};
	found find(int index);
	found find(std::string name);
//	void sort();
	void add(volume* previous, int index, std::string name);
};

