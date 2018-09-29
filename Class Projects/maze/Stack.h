#pragma once

#include "Header.h"
#include "step.h"

template<typename type>
class Stack {
	//#define MAX_LENGTH 10000
public:
	Stack(size_t queue_size) {
		data = new type[queue_size];
		total_size = queue_size;
	}
	~Stack() {
		delete[] data;
		data = nullptr;
	}
	type* data{ nullptr };// [MAX_LENGTH];
	size_t total_size;
	size_t end{ 0 };
	bool empty{ true };
	void push_back(type item);
	type pop_back();
	type back() {
		if (empty) {
			throw std::runtime_error("Cyclic_Queue::front: empty queue cannot have front!");
		}
		return data[end];
	};
	size_t size() {
		if (empty) {
			return 0;
		}
		return end + 1;
	};
	operator bool() {
		return !empty;
	}

	type& operator[](size_t i) {
		if (i >= size()) {
			throw std::out_of_range("Cyclic_Queue::operator[]: index too large!");
		}
		return data[i];
	}
	bool exist(type& compared);
	void print(std::ostream& ost);

	void copy_to(Stack& target);
};

