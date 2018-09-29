#pragma once

#define CharNum 256

template<typename type>
struct CharTree_node {
	//字符节点
	CharTree_node() {
		next = nullptr;
	};

	~CharTree_node() {
		if(next) {
			for(int i = 0; i < CharNum; i++) {
				if(next[i]) {
					delete next[i];
					next[i] = nullptr;
				}
			}
			delete[] next;
			next = nullptr;
		}
	};

	CharTree_node<type>** next;
	type list;

	CharTree_node<type>*& operator [](size_t elem) {
		return next[elem];
	}
};

//class of a CharTree
//This structure maps its contents into a structure for fast(?) string reading and access
//structure overview:
//maps char to its corresponding place in a pointer array to the next node
//node content: 256 pointers to the next node and a pointer to the corresponding data
//pubilc:
//	start of the tree:
//	const CharTree_node<content_type> head;
//	Locates node for given string:
//	CharTree_node<content_type>* locate(std::string token);
//	Access a given content for string:
//	content_type& access(std::string token);
//	returns a pointer to node for adding content to given string(creates if string not exists in tree):
//	CharTree_node<content_type>* add_token(std::string token);
//	Delete given node's content, removes empty branch at the same time:
//	bool del_token(std::string token)
//	Save the tree into a given filestream:
//	void save(std::ofstream& ofs)
//	Loads saved data from given file stream:
//	void load(std::ifstream& ifs)
//	Recursively searches the corresponding string for a given node in tree:
//	std::string find_node(CharTree_node<content_type>* target)
//	Prints the strings and values stored in the tree:
//	void print_tokens(std::ostream& ost) 
//protected:
//	Helper function to see if there's only one child on the given node(for deallocationg empty branches):
//	bool one_succ(CharTree_node<content_type>** list);
//	Helper function for recursive saving:
//	void save_loop(CharTree_node<content_type>* current, std::ofstream& ofs);
//	Helper function for recursive loading:
//	void load_loop(CharTree_node<content_type>* current, std::ifstream& ifs);
//	Helper function for starting the loop (head is const because empty strings are not considered, but needs a const cast to start loop):
//	void load_loop_start(std::ifstream& ifs);
//	Variable for internal signaling if a node is found currenly existing:
//	bool node_found;
//	Helper function for recursive searching:
//	void find_node_loop(CharTree_node<content_type>* target, CharTree_node<content_type>* current, std::string& token);
//	Helper function for recursive print:
//	void print_tokens_loop(CharTree_node<content_type>* current, std::string& token, std::ostream& ost);
template<typename content_type>
class CharTree {
	//字符树的头和操作
public:
	const CharTree_node<content_type> head;

	CharTree_node<content_type>* locate(std::string token) {
		const size_t size = token.size();
		CharTree_node<content_type>* current = const_cast<CharTree_node<content_type>*>(&head);
		if(size == 0) {
			return const_cast<CharTree_node<content_type>*>(&head);
		}
		for(int i = 0; i < size; i++) {
			unsigned char loc = token[i];
			if(current->next[loc]) {
				current = current->next[loc];
			} else {
				return nullptr;
			}
		}
		return current;
	};

	//按引用传递，不然链表会被析构函数消除
	content_type& access(std::string token) {
		CharTree_node<content_type>* node = locate(token);
		if(!node) {
			throw std::runtime_error("CharTree::access: no such token exists!");
		}
		return node->list;
		//	return locate(token)->head;
	}

	CharTree_node<content_type>* add_token(std::string token) {
		const size_t size = token.size();
		CharTree_node<content_type>* current = const_cast<CharTree_node<content_type>*>(&head);
		for(int i = 0; i < size; i++) {
			unsigned char loc = token[i];
			if(!current->next) {
				current->next = new CharTree_node<content_type>*[CharNum];
				for(int j = 0; j < CharNum; j++) {
					current->next[j] = nullptr;
				}
			}
			if(current->next[loc] != nullptr) {
				current = current->next[loc];
			} else {
				current->next[loc] = new CharTree_node<content_type>;
				current = current->next[loc];
			}
		}
		return current;
	}

	bool del_token(std::string token) {
		size_t size = token.size();
		CharTree_node<content_type>* current = const_cast<CharTree_node<content_type>*>(&head);
		//see if the character is the only one and the branch can be removed
		bool* remove = new bool[size]();
		unsigned char loc;
		for(int i = 0; i < size; i++) {
			loc = token[i];
			if(current->next[loc]) {
				if(one_succ(current->next)) {
					remove[i] = true;
				}
				current = current->next[loc];
			} else {
				return false;
			}
		}
		//找出只有这个节点的开头
		size--;
		while(remove[size]) {
			size--;
		}
		const std::string common = token.substr(0, size);
		CharTree_node<content_type>* temp = locate(common);
		loc = token[size];
		if(temp->next) {
			delete (temp->next[loc]);
			temp->next[loc] = nullptr;
		}
		delete[] remove;
		//	remove = nullptr;
		return true;
	};

	void save(std::ofstream& ofs) {
		CharTree_node<content_type>* current = const_cast<CharTree_node<content_type>*>(&head);
		save_loop(current, ofs);
	};

	void load(std::ifstream& ifs) {
		CharTree_node<content_type>* start = const_cast<CharTree_node<content_type>*>(&head);
		if(!start->next) {
			start->next = new CharTree_node<content_type>*[CharNum];
			for(int i = 0; i < CharNum; i++) {
				start->next[i] = nullptr;
			}
			//		start->head = nullptr;
		}
		load_loop_start(ifs);
	}

	//	void load_item_list(CharTree_node* current, std::ifstream& ifs);
	std::string find_node(CharTree_node<content_type>* target) {
		std::string token;
		node_found = false;
		find_node_loop(target, const_cast<CharTree_node<content_type>*>(&head), token);
		return token;
	}

	void print_tokens(std::ostream& ost) {
		std::string token = "";
		print_tokens_loop(const_cast<CharTree_node<content_type>*>(&(this->head)), token, ost);
	}

protected:
	static bool one_succ(CharTree_node<content_type>** list) {
		bool seen{false};
		for(int i = 0; i < CharNum; i++) {
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

	void save_loop(CharTree_node<content_type>* current, std::ofstream& ofs) {
		if(current->list.head) {
			if(current->list.head->index_number < 0) {
				return;
			}
			ofs << '{';
			current->list.print_index(ofs);
			ofs << '}';
		}
		if(!current->next) {
			return;
		}
		for(unsigned i = 0; i < CharNum; i++) {
			if(current->next[i]) {
				const char char_buffer = i;
				ofs << char_buffer;
				save_loop(current->next[i], ofs);
				ofs << '\t';
			}
		}
	}

	void load_loop(CharTree_node<content_type>* current, std::ifstream& ifs) {
		if(ifs.eof()) {
			return;
		}
		char temp;
		while(!ifs.eof()) {
			ifs.get(temp);
			unsigned char loc = temp;
			if(temp == '\t') {
				return;
			} else if(temp == '{') {
				ifs.putback(temp);
				current->list.load_index(ifs);
				continue;
			} else {
				if(!current->next) {
					current->next = new CharTree_node<content_type>*[CharNum];
					for(int i = 0; i < CharNum; i++) {
						current->next[i] = nullptr;
					}
				}
				if(!current->next[loc]) {
					current->next[loc] = new CharTree_node<content_type>;
				}
				load_loop(current->next[loc], ifs);
				continue;
			}
		}
	};

	void load_loop_start(std::ifstream& ifs) {
		//	while(!ifs.eof()) {
		load_loop(const_cast<CharTree_node<content_type>*>(&head), ifs);
		//	}
	}

	bool node_found{false};

	void find_node_loop(CharTree_node<content_type>* target, CharTree_node<content_type>* current, std::string& token) {
		for(unsigned i = 0; i < CharNum; i++) {
			if(!current->next) {
				return;
			}
			if(current->next[i]) {
				const unsigned char temp = i;
				const char char_buff = temp;
				token.push_back(char_buff);
				if(current->next[i] == target) {
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

	void print_tokens_loop(CharTree_node<content_type>* current, std::string& token, std::ostream& ost) {
		if(current->list) {
			//cout<<list;
			if(current->list.head->index_number >= 0) {
				ost << token << ":\t";
				current->list.print_index(ost);
				ost << std::endl;
			}
		}
		for(unsigned i = 0; i < CharNum; i++) {
			if(!current->next) {
				return;
			}
			if(current->next[i]) {
				const unsigned char c = i;
				const char ch = c;
				token.push_back(ch);
				print_tokens_loop(current->next[i], token, ost);
				token.pop_back();
			}
		}
	}

};
