#pragma once

template<typename tree_type, typename content_type>
void Basic_Type_Tree<tree_type, content_type>::node::destroy() {
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

template<typename tree_type, typename content_type> 
typename Basic_Type_Tree<tree_type, content_type>::node*&
Basic_Type_Tree<tree_type, content_type>::node::operator[](size_t elem) {
	return next[elem];
}

template<typename tree_type, typename content_type>
Basic_Type_Tree<tree_type, content_type>::
Basic_Type_Tree(const Basic_Type_Tree& source) {
	Basic_Type_Tree<tree_type, content_type>::copy(*this, source);
}

template<typename tree_type, typename content_type>
Basic_Type_Tree<tree_type, content_type>&
Basic_Type_Tree<tree_type, content_type>::
operator=(const Basic_Type_Tree& source) {
	copy(*this, source);
	return *this;
}

template<typename tree_type, typename content_type>
Basic_Type_Tree<tree_type, content_type>::
Basic_Type_Tree(Basic_Type_Tree&& source) noexcept {
	Basic_Type_Tree<tree_type, content_type>::move(*this, source);
}

template<typename tree_type, typename content_type>
Basic_Type_Tree<tree_type, content_type>&
Basic_Type_Tree<tree_type, content_type>::
operator=(Basic_Type_Tree&& source) noexcept {
	Basic_Type_Tree<tree_type, content_type>::move(*this, source);
	return *this;
}

template<typename tree_type, typename content_type> 
Basic_Type_Tree<tree_type, content_type>::
~Basic_Type_Tree() {
	head->destroy();
	delete head;
	node_found = false;
}

template<typename tree_type, typename content_type> 
typename Basic_Type_Tree<tree_type, content_type>::node* 
Basic_Type_Tree<tree_type, content_type>::
locate(std::basic_string<tree_type> token) const {
	const size_t size = token.size();
	node* current = head;
	if (size == 0) {
		return head;
	}
	for (size_t i = 0; i < size; i++) {
		size_t loc = size_t(token[i]);
		if (current->next[loc]) {
			current = current->next[loc];
		}
		else {
			return nullptr;
		}
	}
	return current;
}

template<typename tree_type, typename content_type> 
content_type& Basic_Type_Tree<tree_type, content_type>::
access(std::basic_string<tree_type> token) {
	node* node = locate(token);
	if (!node) {
		throw std::runtime_error("CharTree::access: no such token exists!");
	}
	return node->data;
	//	return locate(token)->head;
}

template<typename tree_type, typename content_type> 
typename Basic_Type_Tree<tree_type, content_type>::node*
Basic_Type_Tree<tree_type, content_type>::
add_token(std::basic_string<tree_type> token) {
	if(token.empty()) return nullptr;
	const size_t size = token.size();
	if(!head) head = new node;
	node* current = head;
	if(size == 1) return head->next[size_t(token[0])];
	for(size_t i = 0; i < size; i++) {
		size_t loc = size_t(token[i]);
		if(!current->next) {
			current->next = new node*[find_MAX<tree_type>()];
			for(size_t j = 0; j < find_MAX<tree_type>(); j++) {
				current->next[j] = nullptr;
			}
		}
		if((current->next[loc] != nullptr)) {
			current = current->next[loc];
		} else {
			current->next[loc] = new node;
			current = current->next[loc];
		}
	}
	return current;
}

template<typename tree_type, typename content_type> 
bool 
Basic_Type_Tree<tree_type, content_type>::
del_token(std::basic_string<tree_type> token) {size_t size = token.size();
	node* current = head;
	//see if the character is the only one and the branch can be removed
	bool* remove = new bool[size]();
	size_t loc;
	for (size_t i = 0; i < size; i++) {
		loc = size_t(token[i]);
		if (current->next[loc]) {
			if (one_successor(current->next)) {
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
	if (size == 0) {
		head->destroy();
		delete[] remove;
		return true;
	}
	while (remove[size]) {
		size--;
		if (size == 0) {
			head->destroy();
			delete[] remove;
			return true;
		}
	}
	const std::basic_string<tree_type> common = token.substr(0, size);
	node* temp = locate(common);
	loc = size_t(token[size]);
	if (temp->next) {
		if (temp->next[loc]) {
			temp->next[loc]->destroy();
			delete (temp->next[loc]);
			temp->next[loc] = nullptr;
		}
	}
	delete[] remove;
	//	remove = nullptr;
	return true;
}

template<typename tree_type, typename content_type>
void
Basic_Type_Tree<tree_type, content_type>::
save(std::ofstream& ofs) {
	save_loop(head, ofs);
}

template<typename tree_type, typename content_type>
void 
Basic_Type_Tree<tree_type, content_type>::
load(std::ifstream& ifs) {
	node* start = head;
	if (!start->next) {
		start->next = new node*[find_MAX<tree_type>()];
		for (size_t i = 0; i < find_MAX<tree_type>(); i++) {
			start->next[i] = nullptr;
		}
		//		start->head = nullptr;
	}
	load_loop(head, ifs);
}

template<typename tree_type, typename content_type> 
std::basic_string<tree_type>
Basic_Type_Tree<tree_type, content_type>::
find_node(node* target) {
	std::basic_string<tree_type> token;
	node_found = false;
	find_node_loop(target, head, token);
	return token;
}

template<typename tree_type, typename content_type>
void 
Basic_Type_Tree<tree_type, content_type>::
print_tokens(std::ostream& passed) {
	std::basic_string<tree_type> token;
	root_first_recurse_loop(head, token, &passed, print_one_token);
}

template<typename tree_type, typename content_type>
bool 
Basic_Type_Tree<tree_type, content_type>::
one_successor(node** list) {
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

template<typename tree_type, typename content_type> 
void 
Basic_Type_Tree<tree_type, content_type>::
save_loop(node* current, std::ofstream& ofs) {
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

template<typename tree_type, typename content_type> 
void 
Basic_Type_Tree<tree_type, content_type>::
load_loop(node* current, std::ifstream& ifs) {
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
}

template<typename tree_type, typename content_type> 
void 
Basic_Type_Tree<tree_type, content_type>::
find_node_loop(node* target, node* current, std::basic_string<tree_type>& token) {
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

template<typename tree_type, typename content_type>
void
Basic_Type_Tree<tree_type, content_type>::
root_first_recurse_loop(node* current, std::basic_string<tree_type>& token, void* passed, visit_func function) {
	if(current->data && function) {
		function(current->data, passed, token);
	}
	for(unsigned i = 0; i < find_MAX<tree_type>(); i++) {
		if(!current->next) {
			return;
		}
		if(current->next[i]) {
			const unsigned char c = i;
			const char ch = c;
			token.push_back(ch);
			root_first_recurse_loop(current->next[i], token, passed, function);
			token.pop_back();
		}
	}
}

template<typename tree_type, typename content_type> 
void 
Basic_Type_Tree<tree_type, content_type>::
copy(Basic_Type_Tree& destination, const Basic_Type_Tree& source) {
	if(destination.head) {
		destination.head->destroy();
	}
	destination.node_found = source.node_found;
	destination.head = new node;
	node* cur_dest = destination.head;
	node* cur_source = source.head;
	copy_node_loop(cur_dest, cur_source);
}

template<typename tree_type, typename content_type> 
void 
Basic_Type_Tree<tree_type, content_type>::
copy_node_loop(node* destination, node* source) {
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

template<typename tree_type, typename content_type>
void 
Basic_Type_Tree<tree_type, content_type>::
move(Basic_Type_Tree&& destination, Basic_Type_Tree&& source) noexcept {
	if ((&destination) == (&source)) {
		return;
	}
	destination.head = source.head;
	destination.node_found = source.node_found;
	source.head = nullptr;
	destination.head = nullptr;
}

/*
template <typename type>
class CharTree : public Basic_Type_Tree<char, type> {
	void visit_node(typename Basic_Type_Tree<char, type>::node* current, std::ostream& ost, const std::string& token) override {
		ost << token << ":\t";
		ost << current->data;
		ost << std::endl;
	}
};

template <typename type>
class WCharTree : public Basic_Type_Tree<wchar_t, type> {
	void visit_node(typename Basic_Type_Tree<wchar_t, type>::node* current, std::ostream& ost, const std::wstring& token) override {
#ifdef _WIN32
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
#endif
	}
};
*/

