#pragma once
#include "Crossing.h"
#include "main.h"

template<typename type>class Array_2D;
class routes;
class relations;

template<typename type>
class Array_2D{
public:
	Array_2D(const int& row, const int& col);
	Array_2D() { throw std::runtime_error("Array_2D: cannot initialize without two integers!"); }
	~Array_2D();
	type* get(int i, int j);
	void write(type* pos, type val);
	virtual void init(){};
	friend relations;
protected:
	type** map;
	type* data;
};

class relations: Array_2D<bool> {
public:
	relations(){}
	relations(const int& row, const int & col):Array_2D<bool>(row, col) {}
	~relations(){}
	void init(const routes& route);
//	bool** corresponding_route;
//	要想办法定义一个对所有route中可行可能的映射
//	可能不能用bool型继承了，要定义一个struct。
};
