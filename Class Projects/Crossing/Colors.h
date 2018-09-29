#pragma once

#include "Crossing.h"

class Colors {
public:
	Colors() {
		rel = nullptr;
		group = nullptr;
		color_num = 0;
		throw std::runtime_error("UpColors: Please specify a relation to use this class!");
	}
	Colors(Crossing::relations& rela) {
		color_num = 0;
		rel = &rela;
		group = new int[Crossing::possi]();					//用0初始化数组
//		for (int i = 0; i < Crossing::possi; i++) {
//			group[i] = 0;
//		}
	}
	~Colors() {
		delete[] group;
		group = nullptr;
	}
	void find_groups();
	void print_result();
protected:
	Crossing::relations* rel;
	int color_num;
	int* group;									//和struct route对应的分组数组
	bool evaluate(int cur, int opp);			//看某个元素能否加入目前的组
	void assign(int add);						//给某个元素分配一个组
};
