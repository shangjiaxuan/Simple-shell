#pragma once

#include "Header.h"

template<typename type>
class Cyclic_Queue {
public:
	Cyclic_Queue(size_t queue_size) {
		data = new type[queue_size]();
		total_size = queue_size;
	}

	Cyclic_Queue(const Cyclic_Queue& source) {
		copy(*this, source);
	}

	Cyclic_Queue(Cyclic_Queue&& source) noexcept {
		move(*this, source);
	}

	Cyclic_Queue& operator=(const Cyclic_Queue& source) {
		copy(*this, source);
		return *this;
	}

	Cyclic_Queue& operator=(Cyclic_Queue&& source) noexcept {
		move(*this, source);
		return *this;
	}

	~Cyclic_Queue() {
		destroy();
	}

	type* data{nullptr}; // [MAX_LENGTH];
	size_t total_size{0};
	size_t start{0};
	size_t end{0};
	bool empty{true};

	void push_back(type item) {
		if(empty) {
			start = 0;
			end = 0;
			data[0] = item;
			empty = false;
			return;
		}
		end++;
		if(end >= total_size) {
			end -= total_size;
		}
		if(end == start) {
			throw std::runtime_error("Cyclic_Queue::push_back: queue overflow!");
		}
		data[end] = item;
	}

	type pop_front() {
		if(empty) {
			throw std::runtime_error("Cyclic_Queue::pop_front: Cyclic_Queue is empty!");
		}
		type rtn = data[start];
		data[start] = {};
		if(start == end) {
			empty = true;
			start = 0;
			end = 0;
			return rtn;
		}
		start++;
		if(start >= total_size) {
			start -= total_size;
		}
		return rtn;
	}

	type& front() {
		if(empty) {
			throw std::runtime_error("Cyclic_Queue::front: empty queue cannot have front!");
		}
		return data[start];
	}

	size_t size() const {
		if(empty) {
			return 0;
		}
		if(end + 1 >= start) {
			return end - start + 1;
		} else {
			return total_size + end - start + 1;
		}
	}

	operator bool() const {
		return !empty;
	}

	type& operator[](size_t i) {
		if(i >= size()) {
			throw std::out_of_range("Cyclic_Queue::operator[]: index too large!");
		}
		size_t temp = start + i;
		if(temp < total_size) {
			return data[temp];
		} else {
			return data[temp - total_size];
		}
	}

	bool exist(type& compared) {
		size_t i = start;
		do {
			if(data[i] == compared) {
				return true;
			}
			i++;
			if(i >= total_size) {
				i -= total_size;
			}
		}
		while(i != end);
		return false;
	};

	void print(std::ostream& ost) {
		const size_t queue_size = size();
		for(size_t i = 0; i < queue_size; i++) {
			ost << data[i];
		}
		ost << std::endl;
	}

private:
	void destroy() {
		delete[] data;
		data = nullptr;
		empty = true;
		start = 0;
		end = 0;
		total_size = 0;
	}

	static void copy(Cyclic_Queue& destination, const Cyclic_Queue& source) {
		destination.destroy();
		destination.data = new type[source.total_size]();
		destination.total_size = source.total_size;
		destination.empty = source.empty;
		destination.start = source.start;
		destination.end = source.end;
		size_t i = source.start;
		while(i != source.end) {
			destination.data[i] = source.data[i];
			i++;
			if(i >= source.total_size) i -= source.total_size;
		}
	}

	static void move(Cyclic_Queue&& destination, Cyclic_Queue&& source) noexcept {
		if((&destination) == (&source)) {
			return;
		}
		destination.destroy();
		destination.empty = source.empty;
		destination.data = source.data;
		destination.total_size = source.total_size;
		destination.start = source.start;
		destination.end = source.end;
		source.empty = true;
		source.data = nullptr;
		source.total_size = 0;
		source.start = 0;
		source.end = 0;
	}
};
