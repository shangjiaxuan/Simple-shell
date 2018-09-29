#include "Stack.h"

using namespace std;

template<typename type>
type Stack<type>::pop_back() {
	if (empty) {
		throw std::runtime_error("Cyclic_Queue::pop_front: Cyclic_Queue is empty!");
	}
	type rtn = data[end];
	data[end] = type();
	if(end==0) {
		empty = true;
		return rtn;
	}
	end--;
	return rtn;
}

template<typename type>
void Stack<type>::push_back(type item) {
	if (empty) {
		end = 0;
		data[0] = item;
		empty = false;
		return;
	}
	end++;
	data[end] = item;
}

template<typename type> 
bool Stack<type>::exist(type& compared) {
	size_t i = 0;
	while(true) {
		if (data[i] == compared) {
			return true;
		}
		i++;
		if(i>=size()) {
			break;
		}
	}
	return false;
}

template<typename type>
void Stack<type>::print(std::ostream& ost) {
	size_t queue_size = size();
	for (size_t i = 0; i<queue_size; i++) {
		data[i].print(ost);
	}
	ost << endl;
}

template<typename type> 
void Stack<type>::copy_to(Stack& target) {
	if(target.total_size!=total_size) {
		throw range_error("void Stack<type>::copy_to(Stack& target): stacks have different size!");
	}
	target.empty = empty;
	target.end = end;
	for(size_t i=0; i<total_size; i++) {
		target.data[i] = data[i];
	}
}





template class Stack<Maze::step>;

//template<typename type>
//size_t Cyclic_Queue<type>::total_size;
