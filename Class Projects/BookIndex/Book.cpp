#include "Book.h"

Book::~Book() {

}

Book::found Book::find(int index) {
	if(!head) {
		return { 'N', nullptr };
	}
	if (head->index > index) {
		return { 'S' , head };
	}
	volume* current = head;
	while (index > current->index) {
		if (current->next == nullptr) {
			return { 'M' , current };
		}
		current = current->next;
	}
	if(current->index==index) {
		return { 'P' , current , current->name};
	}
	return { 'L' , current };
}

Book::found Book::find(std::string name) {
	if (!head) {
		return { 'N', nullptr };
	}
	volume* current = head;
	if(head->name!=name) {
		while (current->next != nullptr) {
			current = current->next;
			if (current->name == name) {
				break;
			}
		}
	}
	if (current->name == name) {
		return { 'P' , current , current->name };
	}
	return { 'X' , nullptr };
}

void Book::add(volume* previous, int index, std::string name) {
	volume* current = new volume;
	current->index = index;
	current->name = name;
	current->next = previous->next;
	previous->next = current;
}
