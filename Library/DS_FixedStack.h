#pragma once

#include "Header.h"

template<typename type>
class FixedStack {
	//#define MAX_LENGTH 10000
public:
	FixedStack(size_t queue_size) {
		data = new type[queue_size];
		total_size = queue_size;
	}

	FixedStack(const FixedStack& source) {
		copy(*this, source);
	}

	FixedStack(FixedStack&& source) noexcept {
		move(*this, source);
	}

	FixedStack& operator=(const FixedStack& source) {
		copy(*this, source);
		return *this;
	}

	FixedStack& operator=(FixedStack&& source) noexcept {
		move(*this, source);
		return *this;
	}

	~FixedStack() {
		delete[] data;
		data = nullptr;
	}
	type* data{ nullptr };// [MAX_LENGTH];
	size_t total_size;
	size_t end{ 0 };
	bool empty{ true };
	void push(type item) {
		if (empty) {
			end = 0;
			data[0] = item;
			empty = false;
			return;
		}
		end++;
		data[end] = item;
	}
	void pop() {
		if (empty) {
			throw std::runtime_error("Cyclic_Queue::pop_front: Cyclic_Queue is empty!");
		}
		data[end] = type{};
		if (end == 0) {
			empty = true;
		}
		end--;
	}
	type& back() {
		if (empty) {
			throw std::runtime_error("Cyclic_Queue::front: empty queue cannot have front!");
		}
		return data[end];
	};
	size_t size() const {
		if (empty) {
			return 0;
		}
		return end + 1;
	};
	operator bool() const {
		return !empty;
	}

	type& operator[](size_t i) {
		if (i >= size()) {
			throw std::out_of_range("Cyclic_Queue::operator[]: index too large!");
		}
		return data[i];
	}
	bool exist(type& compared) {
		size_t i = 0;
		while (true) {
			if (data[i] == compared) {
				return true;
			}
			i++;
			if (i >= size()) {
				break;
			}
		}
		return false;
	}

	friend std::ostream& operator<<(std::ostream& ost, const FixedStack& source) {
		const size_t queue_size = source.size();
		for (size_t i = 0; i < queue_size; i++) {
			ost << source.data[i] << '\n';
		}
		return ost;
	}

protected:
	void destroy() {
		delete[] data;
		data = nullptr;
		total_size = 0;
		empty = true;
		end = 0;
	}

	static void copy(FixedStack& destination, const FixedStack& source) {
		destination.destroy();
		destination.empty = source.empty;
		destination.data = new type[source.total_size]();
		destination.total_size = source.total_size;
		destination.end = source.end;
		for(size_t i=0; i<=source.end; i++) {
			destination.data[i] = source.data[i];
		}
	}

	static void move(FixedStack&& destination, FixedStack&& source) noexcept {
		if((&destination)==(&source)) return;
		destination.destroy();
		destination.empty = source.empty;
		destination.data = source.data;
		destination.total_size = source.total_size;
		destination.end = source.end;
		source.empty = true;
		source.data = nullptr;
		source.total_size = 0;
		source.end = 0;
	}
};
