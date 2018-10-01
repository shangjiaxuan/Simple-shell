#pragma once

#include "Header.h"

template<typename content_type>
class linked_list {
public:
	struct node {
		void destroy() {
			if(next) {
				next->destroy();
				delete next;
				next = nullptr;
			}
		}

		content_type data{};
		node* next{nullptr};
	};

	linked_list() {
		head = nullptr;
		tail = nullptr;
		current_iterated_object = nullptr;
	}

	linked_list(const linked_list& source) {
		linked_list<content_type>::copy(*this, source);
	}

	linked_list& operator=(linked_list const& source) {
		copy(*this, source);
		return *this;
	}

	linked_list(linked_list&& source) noexcept {
		move(*this, source);
	}

	linked_list& operator=(linked_list&& source) noexcept {
		move(*this, source);
		return *this;
	}

	virtual ~linked_list() {
		destroy();
	}

	void destroy() {
		if(head) {
			head->destroy();
			delete head;
			head = nullptr;
			current_iterated_object = nullptr;
		}
	}

	operator bool() const {
		return head;
	}

	bool empty() const {
		return !head;
	}

	friend std::ostream& operator<<(std::ostream& ost, const linked_list& list) {
		node* current = list.head;
		while(current) {
			ost << current->data << ' ';
			current = current->next;
		}
		return ost;
	}

	friend std::istream& operator>>(std::istream& ist, linked_list& list) {
		content_type temp;
		node* current = list.tail;
		if(!current) {
			ist.peek();
			if((ist >> temp) && ist) {
				list.head = new node;
				current = list.head;
				list.current_iterated_object = list.head;
				goto assignment;
			} else {
				//clears the input fail flag if ist>>temp fails
				ist.clear();
				return ist;
			}
		}
		ist.peek();
		while((ist >> temp) && ist) {
			current->next = new node;
			current = current->next;
		assignment:
			current->data = temp;
			ist.peek();
		}
		list.tail = current;
		//clears the input fail flag if ist>>temp fails
		ist.clear();
		return ist;
	}

	void push_front(content_type item) {
		node* temp = new node;
		temp->next = head;
		temp->data = item;
		head = temp;
		if(!current_iterated_object) {
			current_iterated_object = head;
		}
	}

	void push_back(content_type item) {
		tail->next = new node;
		tail = tail->next;
		tail->next = nullptr;
		tail->data = item;
	}

	content_type& current() {
		if(!iteration_ended_value) return current_iterated_object->data;
		return tail->data;
	}

	content_type& front() {
		return head->data;
	}

	content_type& back() {
		return tail->data;
	}

	content_type& operator++() {
		if(current_iterated_object->next) {
			current_iterated_object = current_iterated_object->next;
			return current_iterated_object->data;
		} else {
			iteration_ended_value = true;
			return current_iterated_object->data;
		}
	}

	content_type& peek_iteration() {
		if(current_iterated_object->next) {
			return current_iterated_object->next->data;
		} else {
			iteration_ended_value = true;
			return current_iterated_object->data;
		}
	}

	void start_iteration() {
		iteration_ended_value = false;
		current_iterated_object = head;
	}

	bool iteration_ended() const {
		return iteration_ended_value;
	}

protected:
	node* head{nullptr};
	//not completely implemented yet
	node* tail{nullptr};

	void insert(node* pre, content_type item) {
		if(!pre) {
			throw std::runtime_error{"Linked list insert: predecessor is nullptr"};
		}
		node* temp = pre->next;
		pre->next = new node;
		pre->next->next = temp;
		pre->next->data = item;
	}

	void del_base(node* pre_start, node* end_del) {
		node* temp = end_del->next;
		end_del->next = nullptr;
		pre_start->destroy();
		pre_start->next = temp;
	}

private:
	//for iteration purpose
	node* current_iterated_object;
	bool iteration_ended_value = false;

	//base functions
	virtual void copy(linked_list& destination, const linked_list& source) {
		if(!source.head) {
			destination.head = nullptr;
			destination.tail = nullptr;
			return;
		}
		node* current_source = source.head;
		destination.head = new node;
		node* current_dest = destination.head;
		while(current_source) {
			current_dest->data = current_source->data;
			if(current_source->next) {
				current_dest->next = new node;
				current_dest = current_dest->next;
			}
			current_source = current_source->next;
		}
		current_dest->next = nullptr;
		destination.current_iterated_object = destination.head;
	}

	virtual void move(linked_list&& destination, linked_list&& source) noexcept {
		if((&destination) == (&source)) {
			return;
		}
		if(destination.head) {
			destination.head->destroy();
		}
		destination.head = source.head;
		destination.tail = source.tail;
		destination.current_iterated_object = source.current_iterated_object;
		destination.iteration_ended_value = source.iteration_ended_value;
		source.head = nullptr;
		source.tail = nullptr;
		source.current_iterated_object = nullptr;
		source.iteration_ended_value = true;
	}
};

template<typename index_type>
class sorted_index_list : public linked_list<index_type> {
public:
	void add(index_type index) {
		if(empty()) {
			push_front(index);
			return;
		}
		if(back() < index) {
			push_back(index);
			return;
		}
		node* pre = find_pre(head, index);
		if((pre->next->data) == index) {
			throw std::runtime_error{"sorted_index_list: add: index exists!"};
		}
		insert(pre, index);
	}

	void del(index_type index) {
		if(empty()) {
			throw std::range_error{"sorted_index_list: del: list is empty!"};
		}
		node* pre_start = find_pre(head, index);
		if(pre_start->next->data == index) {
			node* end_del = pre_start;
			while(end_del->next && (end_del->next->data == index)) {
				end_del = end_del->next;
			}
			if(!(end_del->next)) {
				tail = pre_start;
			}
			del_base(pre_start, end_del);
			return;
		}
		throw std::runtime_error{"sorted_index_list: del: index not found!"};
	}

private:
	static node* find_pre(node* start, index_type index) {
		if(!start) {
			throw std::runtime_error{"sorted_index_list: find_pre: start of search is nullptr!"};
		}
		node* current = start;
		while(current->next && (current->next->data < index)) {
			current = current->next;
		}
		return current;
	}
};
