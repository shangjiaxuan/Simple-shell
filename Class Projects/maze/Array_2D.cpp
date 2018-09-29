#include "Array_2D.h"
#include "Stack.h"

using namespace std;

template<typename type>
Array_2D<type>::Array_2D(const size_t& row, const size_t& col) {
	width = col;
	length = row;
	this->data = new type[row*col]();
	this->map = new type*[row];
	for (size_t i = 0; i < row; i++) {
		this->map[i] = this->data + i * col * sizeof(type);
	}
}

template<typename type>
Array_2D<type>::~Array_2D() {
	delete this->map;
	map = nullptr;
	delete this->data;
	data = nullptr;
}


template class Array_2D<char>;
template class Array_2D<Stack<Maze::step>*>;

