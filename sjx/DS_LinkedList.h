#pragma once

#include "Header.h"

template<typename content_type>
class linked_list {
public:
	struct node {
		void destroy() {
			if (next) {
				next->destroy();
				delete next;
				next = nullptr;
			}
		}
		content_type data{};
		node* next{ nullptr };
	};

	linked_list() {
		head = nullptr;
		tail = nullptr;
	}

	linked_list(const linked_list& source) {
		copy(*this, source);
	}

	linked_list& operator=(linked_list const& source) {
		copy(*this, source);
		return *this;
	}

	linked_list(linked_list&& source) noexcept {
		head = source.head;
		tail = source.tail;
		source.head = nullptr;
		source.tail = nullptr;
	}

	linked_list& operator=(linked_list&& source) noexcept {
		head->destroy();
		head = source.head;
		tail = source.tail;
		source.head = nullptr;
		source.tail = nullptr;
		return *this;
	}

	void copy(linked_list& destination, const linked_list& source) {
		if (!source.head) {
			head = nullptr;
			tail = nullptr;
			return;
		}
		node* current_source = source.head;
		head = new node;
		node* current_dest = head;
		while (current_source) {
			current_dest->data = current_source->data;
			if(current_source->next) {
				current_dest->next = new node;
				current_dest = current_dest->next;
			}
			current_source = current_source->next;
		}
		current_dest->next = nullptr;
	}

	~linked_list() {
		if(head) {
			head->destroy();
			delete head;
			head = nullptr;
		}
	}

	//will be changed to a more direct approach
	node* back_node() const {
		if (!head) {return nullptr;}
		node* current = head;
		while(current->next) {
			current = current->next;
		}
		return current;
	}

	operator bool() const {
		return head;
	}
	bool empty() const {
		return !head;
	}

	friend std::ostream& operator<<(std::ostream& ost, const linked_list& list) {
		node* current = list.head;
		while (current) {
			ost << current->data << ' ';
			current = current->next;
		}
		return ost;
	}

	friend std::istream& operator>>(std::istream& ist, linked_list& list) {
		content_type temp;
		node* current = list.back_node();
		if(!current){
			ist.peek();
			if((ist>>temp)&&ist){ 
				list.head = new node;
				current = list.head;
				goto assignment;
			}
			else {
				//clears the input fail flag if ist>>temp fails
				ist.clear();
				return ist;
			}
		}
		ist.peek();
		while ((ist >> temp) && ist) {
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
	}
	void push_back(content_type item) {
		tail->next = new node;
		tail = tail->next;
		tail->next = nullptr;
		tail->data = item;
	}
protected:
	node* head;
	//not completely implemented yet
	node* tail;
	void insert(node* pre, content_type item) {
		if(!pre) {
			throw std::runtime_error{ "Linked list insert: predecessor is nullptr" };
		}
		node* temp = pre->next;
		pre->next = new node;
		pre->next->next = temp;
		pre->next->data = item;
	}
};

template <typename index_type>
class sorted_index_list: public linked_list<int> {
public:
	void add(index_type index) {
		if(!head) {
			push_front(index);
			return;
		}
		if(tail&&(tail->data<index)) {
			push_back(index);
			return;
		}
		node* pre = find_pre(head, index);
		if((pre->next->data)==index) {
			throw std::runtime_error{ "sorted_index_list: add: index exists!" };
		}
		insert(pre, index);
	}

	void del(index_type index) {
		if(!head) {
			throw std::range_error{ "sorted_index_list: del: list is empty!" };
		}
		node* pre_start = find_pre(head, index);
		if(pre_start->next->data==index) {
			node* end_del = pre_start;
			while(end_del->next&&(end_del->next->data==index)) {
				end_del = end_del->next;
			}
			if(!(end_del->next)) {
				tail = pre_start;
			}
			node* temp = end_del->next;
			end_del->next = nullptr;
			pre_start->destroy();
			pre_start->next = temp;
			return;
		}
		throw std::runtime_error{"sorted_index_list: del: index not found!"};
	}
private:
	static node* find_pre(node* start, index_type index) {
		if (!start) {
			throw std::runtime_error{ "sorted_index_list: find_pre: start of search is nullptr!" };
		}
		node* current = start;
		while (current->next && (current->next->data < index)) {
			current = current->next;
		}
		return current;
	}
};

