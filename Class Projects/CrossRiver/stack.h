#pragma once

#include "Header.h"

class state;
class stack;

//0号位：农夫
//1号位：白菜
//2号位：羊
//3号位：狼
//某一位上的0表示在原岸，1表示在对岸
class state {
public:
#define ITEM_NO 4
	state(){
		data='\0';
		layer = 0;
	}
//	state(char c, unsigned layr){
//		data=c;
//		layer = layr;
//	}
	char data;
	unsigned layer;
	void set(int location, bool state);
	void invert(int location);
	bool access(int location);
	void print(std::ostream& ost);
	bool operator==(state st);
	bool operator!=(state st) {
		return !(*this==st);
	}
};

class stack {
public:
#define MAX_LENGTH 16
	stack() {
		for(int i=0; i<MAX_LENGTH; i++) {
			data[i] = state();
		}
	}
	state data[MAX_LENGTH];
	int filled{-1};
	void pushback(state data);
	state popback();
	state peek();
	bool exist(state state);
	void print(std::ostream& ost);
	operator bool(){
		if(filled<0){
			return false;
		}
		return true;
	}
};

class cross_river {
public:
	state status;
	stack visited;
	stack traceback;
	stack stack;
	unsigned cur_layer{ 0 };
	bool stash();
	void go_back();
	bool is_safe();
	bool is_safe(state state);
	void cross_verbose(std::ostream& ost);
	bool done();
	void add_record();
	void del_record();
	void print_stack(std::ostream& ost);
	void print_path(std::ostream& ost);
	void interaction();
};
