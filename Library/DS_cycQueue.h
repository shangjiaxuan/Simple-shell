#pragma once

#include "Header.h"

//may add a Linked_Cyclic_Queue class to dynamically allocate Cyclic_Queues with given fixed size
//This will require a linked list of cyclic queues
template<typename type>
class Cyclic_Queue {
public:
	Cyclic_Queue(size_t queue_size);

	Cyclic_Queue(const Cyclic_Queue& source);
	Cyclic_Queue(Cyclic_Queue&& source) noexcept;
	Cyclic_Queue& operator=(const Cyclic_Queue& source);
	Cyclic_Queue& operator=(Cyclic_Queue&& source) noexcept;
	~Cyclic_Queue();

	type* data{nullptr}; // [MAX_LENGTH];
	size_t total_size{0};
	size_t start{0};
	size_t end{0};
	bool empty{true};

	void push_back(type item);
	type pop_front();
	type& front();
	size_t size() const;
	operator bool() const;

	type& operator[](size_t i);

	bool exist(type& compared);

	void print(std::ostream& ost);

private:
	void destroy();
	static void copy(Cyclic_Queue& destination, const Cyclic_Queue& source);
	static void move(Cyclic_Queue&& destination, Cyclic_Queue&& source) noexcept;
};

#include "DS_cycQueue.ipp"
