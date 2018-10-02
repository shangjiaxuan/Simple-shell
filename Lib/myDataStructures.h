#pragma once
#include "Header.h"

#include "DS_LinkedList.h"
#include "DS_CharTree.h"
#include "DS_FixedStack.h"
#include "DS_cycQueue.h"

template<typename type>
class Array_2D {
public:
	Array_2D(const size_t& row, const size_t& col) {
		width = col;
		length = row;
		this->data = new type[row*col]();
		this->map = new type*[row];
		for (size_t i = 0; i < row; i++) {
			this->map[i] = this->data + i * col * sizeof(type);
		}
	}
	Array_2D() {
		length = 0;
		width = 0;
		throw std::runtime_error("Array_2D: cannot initialize without two integers!");
	}
	virtual ~Array_2D() {
		delete this->map;
		map = nullptr;
		delete this->data;
		data = nullptr;
	}
	//	type* get(int i, int j);
	//	void write(type* pos, type val);
	virtual void init() {};
	size_t length;
	size_t width;
	type** map{nullptr};
	//protected:
	type* data{nullptr};
};
