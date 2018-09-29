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

