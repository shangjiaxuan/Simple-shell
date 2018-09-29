#include "CharTree.h"
#include "Booklist.h"

using namespace std;

node::node() {
	next = nullptr;
//	head = nullptr;
}

node::~node() {
	if(next) {
		for(int i = 0; i < CharNum; i++) {
			if(next[i]) {
				delete next[i];
				next[i] = nullptr;
			}
		}
		//就是怕栈溢出。。。递归调用。。。
		delete[] next;
		next = nullptr;
	}
//	if(head) {
//		delete head;
//		head = nullptr;
//	}
}

node* CharTree::locate(std::string token) {
	const size_t size = token.size();
	node* current = const_cast<node*>(&head);
	unsigned char loc;
	if(size==0) {
		return const_cast<node*>(&head);
	}
	for(int i=0; i<size; i++) {
		loc = token[i];
		if(current->next[loc]) {
			current = current->next[loc];
		}else {
			return nullptr;
		}
	}
	return current;
}

//按引用传递，不然链表会被析构函数消除
index_list& CharTree::access(std::string token) {
	node* node = locate(token);
	if(!node) {
		throw runtime_error("CharTree::access: no such token exists!");
	}
	return node->list;
//	return locate(token)->head;
}

node* CharTree::add_token(std::string token) {
	const size_t size = token.size();
	node* current = const_cast<node*>(&head);
	unsigned char loc;
	for (int i = 0; i < size; i++) {
		loc = token[i];
		if(!current->next) {
			current->next = new node*[CharNum];
			for (int j = 0; j < CharNum; j++) {
				current->next[j] = nullptr;
			}
		}
		if(current->next[loc]!=nullptr) {
			current = current->next[loc];
		} else {
			current->next[loc] = new node;
			current = current->next[loc];
		}
	}
//	if(!current->head) {
//		current->head = new item;
//		current->head->next_item = nullptr;
//		current->head->index_number = -2;
//	}
	return current;
}

bool CharTree::del_token(std::string token) {
	size_t size = token.size();
	node* current = const_cast<node*>(&head);
	//see if the character is the only one and the branch can be removed
	bool* remove = new bool[size]();
	unsigned char loc;
	for (int i = 0; i<size; i++) {
		loc = token[i];
		if (current->next[loc]) {
			if(one_succ(current->next)) {
				remove[i] = true;
			}
			current = current->next[loc];
		}
		else {
			return false;
		}
	}
	//找出只有这个节点的开头
	size--;
	while(remove[size]) {
		size--;
	}
	std::string common = token.substr(0, size);
	node* temp = locate(common);
	loc = token[size];
	if(temp->next) {
		delete (temp->next[loc]);
		temp->next[loc] = nullptr;
	}
	delete[] remove;
//	remove = nullptr;
	return true;
}

bool CharTree::one_succ(node** list) {
	bool seen{ false };
	for(int i=0; i<CharNum; i++) {
		if(list[i]) {
			if(seen) {
				return false;
			}
			seen = true;
		}
	}
	if(!seen) {
		throw std::runtime_error("CharTree::one_succ: No successor found!");
	}
	return true;
}


void CharTree::save(ofstream& ofs) {
	node* current = const_cast<node*>(&head);
	save_loop(current, ofs);
}

void CharTree::save_loop(node* current, ofstream& ofs) {
	char char_buffer;
	if (current->list.head) {
		if (current->list.head->index_number<0) {
			return;
		}
		ofs << '{';
		current->list.print_index(ofs);
		ofs << '}';
	}
	if (!current->next) {
		return;
	}
	for(unsigned i = 0; i < CharNum; i++) {
		if(current->next[i]) {
			char_buffer = i;
			ofs << char_buffer;
			save_loop(current->next[i], ofs);
			ofs << '\t';
		}
	}
}

void CharTree::load(ifstream& ifs) {
	node* start = const_cast<node*>(&head);
	if (!start->next) {
		start->next = new node*[CharNum];
		for (int i = 0; i < CharNum; i++) {
			start->next[i] = nullptr;
		}
//		start->head = nullptr;
	}
	load_loop_start(ifs);
}

void CharTree::load_loop_start(std::ifstream& ifs) {
//	while(!ifs.eof()) {
		load_loop(const_cast<node*>(&head), ifs);
//	}
}

void CharTree::load_loop(node* current, ifstream& ifs) {
	if(ifs.eof()) {
		return;
	}
	char temp;
	while (!ifs.eof()) {
		ifs.get(temp);
		unsigned char loc = temp;
		if (temp == '\t') {
			return;
		}
		else if (temp == '{') {
			ifs.putback(temp);
			current->list.load_index(ifs);
			continue;
		}
		else {
			if (!current->next) {
				current->next = new node*[CharNum];
				for (int i = 0; i < CharNum; i++) {
					current->next[i] = nullptr;
				}
			}
			if (!current->next[loc]) {
				current->next[loc] = new node;
			}
			load_loop(current->next[loc], ifs);
			continue;
		}
	}
}

/*void CharTree::load_item_list(node* current, ifstream& ifs) {
	int index_number;
	current->list.head = new item;
	item* cur = current->list.head;
	char temp;
	ifs.get(temp);
	while(temp != '}') {
		ifs.putback(temp);
		ifs >> index_number;
		cur->index_number = index_number;
		ifs.get();
		ifs.get(temp);
		if(isdigit(temp)) {
			cur->next_item = new item;
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

void CharTree::find_node_loop(node* target, node* current, string& token) {
	unsigned char temp;
	char char_buff;
	for(unsigned i=0; i<CharNum; i++) {
		if(!current->next) {
			return;
		}
		if (current->next[i]) {
			temp = i;
			char_buff = temp;
			token.push_back(char_buff);
			if (current->next[i] == target) {
				node_found = true;
				return;
			}
			find_node_loop(target, current->next[i], token);
			if(node_found) {
				return;
			}
			token.pop_back();
		}
	}
}

string CharTree::find_node(node* target) {
	string token;
	node_found = false;
	find_node_loop(target, const_cast<node*>(&head), token);
	return token;
}

void CharTree::print_tokens_loop(node* current, string& token, ostream& ost) {
	if (current->list) {
		if (current->list.head->index_number >= 0) {
			ost << token << ":\t";
			current->list.print_index(ost);
			ost << endl;
		}
	}
	for (unsigned i = 0; i<CharNum; i++) {
		if (!current->next) {
			return;
		}
		if (current->next[i]) {
			unsigned char c = i;
			char ch = c;
			token.push_back(ch);
			print_tokens_loop(current->next[i], token, ost);
			token.pop_back();
		}
	}
}

void CharTree::print_tokens(ostream& ost) {
	string token = "";
	print_tokens_loop(const_cast<node*>(&(this->head)), token, ost);
}


void index_list::add(int index) {
	item* current = head;
	item* traceback = nullptr;
	while(current) {
		if(index<current->index_number) {
			if(!traceback) {
				head = new item;
				head->index_number = index;
				head->next_item = current;
				return;
			}
			item* added = new item;
			added->next_item = current;
			traceback->next_item = added;
			added->index_number = index;
			return;
		}
		if(index==current->index_number) {
			throw runtime_error("index_list::add: index exists!");
		}
		if(!current->next_item) {
			current->next_item = new item;
			current->next_item->index_number = index;
			return;
		}
		traceback = current;
		current = current->next_item;
	}
	head = new item;
	head->index_number = index;
}

void index_list::del(int index) {
	item* current = head;
	item* traceback = nullptr;
	while(current) {
		if(index==current->index_number) {
			if(current==head) {
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
		if(index<current->index_number||!current->next_item) {
			throw runtime_error("index_list::del: no such index exists!");
		}
		traceback = current;
		current = current->next_item;
	}
	throw runtime_error("index_list::del: list is empty!");
}

void index_list::print_index(std::ostream& ost) {
	item* current = head;
	while(current) {
		ost << current->index_number << ' ';
		current = current->next_item;
	}
}

void index_list::load_index(std::ifstream& ifs) {
	char c;
	item* current = head;
	item* traceback = nullptr;
	ifs.get(c);
	if(c!='{') {
		ifs.putback(c);
		return;
	}
	int index;
	while (ifs.peek() != '}') {
		if (ifs >> index) {
			traceback = current;
			current = new item;
			if (traceback) {
				traceback->next_item = current;
			}
			else {
				head = current;
			}
		}
		else {
			throw runtime_error("index_list::load_index: not a number");
		}
		current->index_number = index;
		ifs.get();
	}
	ifs.get();
}

