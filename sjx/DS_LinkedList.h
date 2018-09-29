#pragma once

#include "Header.h"
#include "DS_CharTree.h"

template<typename content_type>
struct linked_list_node {
	~linked_list_node() {
		if(next_item) {
			//			next_item->~linked_list_node();
			delete next_item;
			next_item = nullptr;
		}
	}

	content_type index_number;
	linked_list_node* next_item{nullptr};
};

template<typename content_type>
class linked_list {
public:
	linked_list_node<content_type>* head;

	void add(int index) {
		linked_list_node<content_type>* current = head;
		linked_list_node<content_type>* traceback = nullptr;
		while(current) {
			if(index < current->index_number) {
				if(!traceback) {
					head = new linked_list_node<content_type>;
					head->index_number = index;
					head->next_item = current;
					return;
				}
				linked_list_node<content_type>* added = new linked_list_node<content_type>;
				added->next_item = current;
				traceback->next_item = added;
				added->index_number = index;
				return;
			}
			if(index == current->index_number) {
				throw std::runtime_error("linked_list::add: index exists!");
			}
			if(!current->next_item) {
				current->next_item = new linked_list_node<content_type>;
				current->next_item->index_number = index;
				return;
			}
			traceback = current;
			current = current->next_item;
		}
		head = new linked_list_node<content_type>;
		head->index_number = index;
	};

	void del(int index) {
		linked_list_node<content_type>* current = head;
		linked_list_node<content_type>* traceback = nullptr;
		while(current) {
			if(index == current->index_number) {
				if(current == head) {
					current->next_item = nullptr;
					head = nullptr;
				} else {
					traceback->next_item = current->next_item;
					current->next_item = nullptr;
				}
				delete current;
				//			current = nullptr;
				return;
			}
			if(index < current->index_number || !current->next_item) {
				throw std::runtime_error("linked_list::del: no such index exists!");
			}
			traceback = current;
			current = current->next_item;
		}
		throw std::runtime_error("linked_list::del: list is empty!");
	};

	void print_index(std::ostream& ost) const {
		linked_list_node<content_type>* current = head;
		while(current) {
			ost << current->index_number << ' ';
			current = current->next_item;
		}
	};

	void load_index(std::ifstream& ifs) {
		char c;
		linked_list_node<content_type>* current = head;
		linked_list_node<content_type>* traceback = nullptr;
		ifs.get(c);
		if(c != '{') {
			ifs.putback(c);
			return;
		}
		int index;
		while(ifs.peek() != '}') {
			if(ifs >> index) {
				traceback = current;
				current = new linked_list_node<content_type>;
				if(traceback) {
					traceback->next_item = current;
				} else {
					head = current;
				}
			} else {
				throw std::runtime_error("linked_list::load_index: not a number");
			}
			current->index_number = index;
			ifs.get();
		}
		ifs.get();
	};

	operator bool() const {
		return head;
	}

	friend CharTree_node<linked_list<content_type>>;
private:
	linked_list() {
		head = nullptr;
	}

	~linked_list() {
		delete head;
		head = nullptr;
	}
};
