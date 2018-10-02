#pragma once

//#define CharNum 256
#include "Utilities.h"

//class of a CharTree
//This structure maps its contents into a structure for fast(?) string reading and access
//structure overview:
//maps char to its corresponding place in a pointer array to the next node
//node content: 256 pointers to the next node and a pointer to the corresponding data
//public:
//	start of the tree:
//	node<content_type> head;
//	Locates node for given string:
//	node<content_type>* locate(std::string token);
//	Access a given content for string:
//	content_type& access(std::string token);
//	returns a pointer to node for adding content to given string(creates if string not exists in tree):
//	node<content_type>* add_token(std::string token);
//	Delete given node's content, removes empty branch at the same time:
//	bool del_token(std::string token)
//	Save the tree into a given filestream:
//	void save(std::ofstream& ofs)
//	Loads saved data from given file stream:
//	void load(std::ifstream& ifs)
//	Recursively searches the corresponding string for a given node in tree:
//	std::string find_node(node<content_type>* target)
//	Prints the strings and values stored in the tree:
//	void print_tokens(std::ostream& ost) 
//protected:
//	Helper function to see if there's only one child on the given node(for deallocationg empty branches):
//	bool one_successor(node<content_type>** list);
//	Helper function for recursive saving:
//	void save_loop(node<content_type>* current, std::ofstream& ofs);
//	Helper function for recursive loading:
//	void load_loop(node<content_type>* current, std::ifstream& ifs);
//	Variable for internal signaling if a node is found currently existing:
//	bool node_found;
//	Helper function for recursive searching:
//	void find_node_loop(node<content_type>* target, node<content_type>* current, std::string& token);
//	Helper function for recursive print:
//	void print_tokens_loop(node<content_type>* current, std::string& token, std::ostream& ost);
template<typename tree_type, typename content_type>
class CharTree {
	//字符树的头和操作
public:
	struct node {
		//字符节点
		void destroy() {
			if(next) {
				for(size_t i = 0; i < find_MAX<tree_type>(); i++) {
					if(next[i]) {
						next[i]->destroy();
						delete next[i];
						next[i] = nullptr;
					}
				}
				delete[] next;
				next = nullptr;
			}
		}

		node** next{nullptr};
		content_type data{};

		node*& operator [](size_t elem) {
			return next[elem];
		}
	};

	CharTree() = default;

	CharTree(const CharTree& source) {
		CharTree<tree_type, content_type>::copy(*this, source);
	}

	CharTree& operator=(const CharTree& source) {
		copy(*this, source);
		return *this;
	}

	CharTree(CharTree&& source) noexcept {
		move(*this, source);
	}

	CharTree& operator=(CharTree&& source) noexcept {
		move(*this, source);
		return *this;
	}

	virtual ~CharTree() {
		head->destroy();
		delete head;
		node_found = false;
	}

	node* head{nullptr};

	node* locate(std::basic_string<tree_type> token) const {
		const size_t size = token.size();
		node* current = head;
		if(size == 0) {
			return head;
		}
		for(size_t i = 0; i < size; i++) {
			size_t loc = size_t(token[i]);
			if(current->next[loc]) {
				current = current->next[loc];
			} else {
				return nullptr;
			}
		}
		return current;
	};

	//按引用传递，不然链表会被析构函数消除
	content_type& access(std::basic_string<tree_type> token) {
		node* node = locate(token);
		if(!node) {
			throw std::runtime_error("CharTree::access: no such token exists!");
		}
		return node->data;
		//	return locate(token)->head;
	}

	node* add_token(std::basic_string<tree_type> token) {
		if (token.empty()) return nullptr;
		const size_t size = token.size();
		if(!head) head = new node;
		node* current = head;
		if (size == 1) return head->next[size_t(token[0])];
		for(size_t i = 0; i < size; i++) {
			size_t loc = size_t(token[i]);
			if(!current->next) {
				current->next = new node*[find_MAX<tree_type>()];
				for(size_t j = 0; j < find_MAX<tree_type>(); j++) {
					current->next[j] = nullptr;
				}
			}
			if ((current->next[loc] != nullptr)) {
				current = current->next[loc];
			}
			else {
				current->next[loc] = new node;
				current = current->next[loc];
			}
		}
		return current;
	}

	bool del_token(std::basic_string<tree_type> token) {
		size_t size = token.size();
		node* current = head;
		//see if the character is the only one and the branch can be removed
		bool* remove = new bool[size]();
		size_t loc;
		for(size_t i = 0; i < size; i++) {
			loc = size_t(token[i]);
			if(current->next[loc]) {
				if(one_successor(current->next)) {
					remove[i] = true;
				}
				current = current->next[loc];
			} else {
				return false;
			}
		}
		//找出只有这个节点的开头
		size--;
		if (size == 0) {
			head->destroy();
			delete[] remove;
			return true;
		}
		while(remove[size]) {
			size--;
			if(size==0) {
				head->destroy();
				delete[] remove;
				return true;
			}
		}
		const std::basic_string<tree_type> common = token.substr(0, size);
		node* temp = locate(common);
		loc = size_t(token[size]);
		if(temp->next) {
			if(temp->next[loc]) {
				temp->next[loc]->destroy();
				delete (temp->next[loc]);
				temp->next[loc] = nullptr;
			}
		}
		delete[] remove;
		//	remove = nullptr;
		return true;
	};

	void save(std::ofstream& ofs) {
		save_loop(head, ofs);
	};

	//should be renamed to "combine (without security)"
	void load(std::ifstream& ifs) {
		node* start = head;
		if(!start->next) {
			start->next = new node*[find_MAX<tree_type>()];
			for(size_t i = 0; i < find_MAX<tree_type>(); i++) {
				start->next[i] = nullptr;
			}
			//		start->head = nullptr;
		}
		load_loop(head, ifs);
	}

	//	void load_item_list(node* current, std::ifstream& ifs);
	std::basic_string<tree_type> find_node(node* target) {
		std::basic_string<tree_type> token;
		node_found = false;
		find_node_loop(target, head, token);
		return token;
	}

	void print_tokens(std::ostream& ost) {
		std::basic_string<tree_type> token;
		print_tokens_loop(head, token, ost);
	}

protected:
	static bool one_successor(node** list) {
		bool seen{false};
		for(size_t i = 0; i < find_MAX<tree_type>(); i++) {
			if(list[i]) {
				if(seen) {
					return false;
				}
				seen = true;
			}
		}
		if(!seen) {
			throw std::runtime_error("CharTree::one_successor: No successor found!");
		}
		return true;
	}

	void save_loop(node* current, std::ofstream& ofs) {
		if(current->data) {
			ofs << '{' << current->data << '}';
		}
		if(!current->next) {
			return;
		}
		for(size_t i = 0; i < find_MAX<tree_type>(); i++) {
			if(current->next[i]) {
				const tree_type char_buffer = tree_type(i);
				ofs << char_buffer;
				save_loop(current->next[i], ofs);
				ofs << '\t';
			}
		}
	}

	void load_loop(node* current, std::ifstream& ifs) {
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
				ifs >> current->data;
				char c;
				ifs.get(c);
				while(c == ' ' || c == '\n') {
					ifs.get(c);
				}
				if(c != '}') {
					throw std::runtime_error{"CharTree: load_loop: parenthesis mismatch!"};
				}
				//do not put back '}'
				continue;
			} else {
				if(!current->next) {
					current->next = new node*[find_MAX<tree_type>()];
					for(size_t i = 0; i < find_MAX<tree_type>(); i++) {
						current->next[i] = nullptr;
					}
				}
				if(!current->next[loc]) {
					current->next[loc] = new node;
				}
				load_loop(current->next[loc], ifs);
				continue;
			}
		}
	};

	bool node_found{false};

	void find_node_loop(node* target, node* current, std::basic_string<tree_type>& token) {
		for(unsigned i = 0; i < find_MAX<tree_type>(); i++) {
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

	virtual void print_useful_content(node* current, std::ostream& ost, const std::basic_string<tree_type>& token){
		ost << token << ":\t";
		ost << current->data;
		ost << std::endl;
	}

	virtual void print_tokens_loop(node* current, std::basic_string<tree_type>& token, std::ostream& ost) {
		if(current->data) {
			print_useful_content(current, ost, token);
		}
		for(unsigned i = 0; i < find_MAX<tree_type>(); i++) {
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

private:
	virtual void copy(CharTree& destination, const CharTree& source) {
		if(destination.head) {
			destination.head->destroy();
		}
		destination.node_found = source.node_found;
		destination.head = new node;
		node* cur_dest = destination.head;
		node* cur_source = source.head;
		copy_node_loop(cur_dest, cur_source);
	}

	virtual void copy_node_loop(node* destination, node* source) {
		if(!source) return;
		destination->destroy();
		destination->data = source->data;
		if(source->next) destination->next = new node*[find_MAX<tree_type>()]();
		for(size_t i = 0; i < find_MAX<tree_type>(); i++) {
			if(source->next[i]) {
				destination->next[i] = new node;
				copy_node_loop(destination->next[i], source->next[i]);
			}
		}
	}

	virtual void move(CharTree&& destination, CharTree&& source) noexcept {
		if((&destination) == (&source)) {
			return;
		}
		destination.head = source.head;
		destination.node_found = source.node_found;
		source.head = nullptr;
		destination.head = nullptr;
	}
};

#ifdef _WIN32
inline void CharTree<wchar_t, int>::print_useful_content(node* current, std::ostream& ost, const std::wstring& token)  {
	const DWORD size = WideCharToMultiByte(GetConsoleCP(), NULL, token.c_str(), -1, nullptr, 0, nullptr, nullptr);
	LPSTR const multiByteStr = new CHAR[size];  // NOLINT(misc-misplaced-const)
	char default_char = '?';
	BOOL use_default = true;
	if (WideCharToMultiByte(GetConsoleCP(), NULL, token.c_str(), -1, multiByteStr, size, &default_char, &use_default) == 0
		) {
		throw std::runtime_error("Error calling WideCharToMultiByte converting to MBCS");
	}
	ost << multiByteStr << ":\t";
	ost << current->data;
	ost << std::endl;
	delete[] multiByteStr;
}
#endif

