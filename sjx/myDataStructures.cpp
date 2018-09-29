#include "myDataStructures.h"

using namespace std;

template<typename type>
Array_2D<type>::Array_2D(const int& row, const int& col) {
	this->data = new type[row * col];
	this->map = new type*[row];
	for(int i = 0; i < row; i++) {
		this->map[i] = this->data + i * col * sizeof(type);
	}
}

template<typename type>
Array_2D<type>::~Array_2D() {
	delete this->map;
	map = nullptr;
	delete this->data;
	data = nullptr;
}

template<typename type>
type* Array_2D<type>::get(int i, int j) {
	return &map[i][j];
}

template<typename type>
void Array_2D<type>::write(type* pos, type val) {
	*pos = val;
}

template class Array_2D<bool>;


/*void CharTree::load_item_list(CharTree_node* current, ifstream& ifs) {
	int index_number;
	current->list.head = new linked_list_node;
	linked_list_node* cur = current->list.head;
	char temp;
	ifs.get(temp);
	while(temp != '}') {
		ifs.putback(temp);
		ifs >> index_number;
		cur->index_number = index_number;
		ifs.get();
		ifs.get(temp);
		if(isdigit(temp)) {
			cur->next_item = new linked_list_node;
			cur = cur->next_item;
			ifs.putback(temp);
		} else if(temp=='}') {
			cur->next_item = nullptr;
			break;
		} else {
			throw  runtime_error("CharTree::load_item_list: index expected!");
		}
	}
	// break;
}*/
