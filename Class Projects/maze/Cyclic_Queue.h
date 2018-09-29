#pragma once

#include "Header.h"
#include "step.h"

template<typename type>
class Cyclic_Queue {
//#define MAX_LENGTH 10000
public:
	Cyclic_Queue(size_t queue_size) {
		data = new type[queue_size]();
		total_size = queue_size;
	}
	~Cyclic_Queue() {
		delete[] data;
		data = nullptr;
	}
	type* data;// [MAX_LENGTH];
	size_t total_size;
	size_t start{ 0 };
	size_t end{ 0 };
	bool empty{ true };
	void push_back(type item);
	type pop_front();
	type front() {
		if(empty) {
			throw std::runtime_error("Cyclic_Queue::front: empty queue cannot have front!");
		}
		return data[start];
	};
	size_t size() {
		if(empty) {
			return 0;
		}
		if (end+1 >= start) {
			return end - start + 1;
		} else {
			return total_size + end - start + 1;
		}
	};
	operator bool(){
		return !empty;
	}

	type& operator[](size_t i) {
		if(i>=size()) {
			throw std::out_of_range("Cyclic_Queue::operator[]: index too large!");
		}
		size_t temp = start + i;
		if(temp<total_size) {
			return data[temp];
		}
		else {
			return data[temp - total_size];
		}
	}
	bool exist(type& compared);
	void print(std::ostream& ost);
};


