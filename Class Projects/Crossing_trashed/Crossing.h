#pragma once

#include "Header.h"

//用三个数表现路口本身状态作为输入
//1:只出不入
//0：可入可出
//-1：只入不出
//短整型就好
//只用两位，作位运算不好维护
//虽然char类可能好？

typedef short status;
//方便重载带有循环的++，--运算符

//定义路口
typedef class Crossing {
public:
	Crossing() :size{ 0 }, data{ nullptr }, current(0) { throw std::runtime_error("Crossing: Improper usage!"); }
	Crossing(const int& s) : size{ s }, data{ new status[size] }, current { 0 } { init(*this); }
	~Crossing() {
		delete[] data;
		data = nullptr;									//不加这一句操作系统会在程序结束时再次释放，导致访问错误
	}
	const int& size;
	status* data;
	int current;
	static void init(Crossing&);						//初始化各个路口
	status*& next(status*& st);
	status*& prev(status*& st);
};





//template<typename type>
//void delete_2D(type** map) {
//	delete[] **map;
//	delete[] *map;
//	delete[] map;
//}


