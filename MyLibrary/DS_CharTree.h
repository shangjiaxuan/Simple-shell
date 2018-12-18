#pragma once

#include "Utilities.h"

//class of a CharTree
//This structure maps its contents into a structure for fast(?) string reading and access
//structure overview:
//maps char to its corresponding place in a pointer array to the next node
//node content: 256 pointers(for char type) to the next node and a pointer to the corresponding data
template<typename tree_type, typename content_type>
class Basic_Type_Tree {
public:
	//A data node of the tree
	struct node {
		void destroy();
		node** next{nullptr};
		content_type data{};
		node*& operator [](size_t elem);;
	};

	Basic_Type_Tree() = default;
	Basic_Type_Tree(const Basic_Type_Tree& source);
	Basic_Type_Tree& operator=(const Basic_Type_Tree& source);
	Basic_Type_Tree(Basic_Type_Tree&& source) noexcept;
	Basic_Type_Tree& operator=(Basic_Type_Tree&& source) noexcept;
	virtual ~Basic_Type_Tree();

	//	start of the tree:
	node* head{nullptr};

	//Locates node for given string:
	node* locate(std::basic_string<tree_type> token) const;
	//Access a given content for string:
	content_type& access(std::basic_string<tree_type> token);
	//returns a pointer to node for adding content to given string(creates if string not exists in tree):
	node* add_token(std::basic_string<tree_type> token);
	//Delete given node's content, removes empty branch at the same time, returns true if delete succeed in locating and deleting:
	bool del_token(std::basic_string<tree_type> token);

	//Save the tree into a given filestream:
	void save(std::ofstream& ofs);
	//Loads saved data from given file stream(gets node data input using the '>>' operator):
	void load(std::ifstream& ifs);

	//Recursively searches the corresponding string for a given node in tree (return empty string if not found):
	std::basic_string<tree_type> find_node(node* target);

	//Prints the strings and values stored in the tree (the function to use when going through all the nodes with info):
	void print_tokens(std::ostream& ost);

protected:
	//Helper function to see if there's only one child on the given node(for deallocationg empty branches):
	static bool one_successor(node** list);
	//Helper function for recursive saving from file:
	void save_loop(node* current, std::ofstream& ofs);
	//Helper function for recursive loading from file:
	void load_loop(node* current, std::ifstream& ifs);
	//Variable for internal signaling if a node is found currently existing:
	bool node_found{false};
	//Helper function for recursive searching:
	void find_node_loop(node* target, node* current, std::basic_string<tree_type>& token);

	//A recursive method that writes its visited output to a stream that can be extracted or used directly
	virtual void root_first_recurse(node* current, std::basic_string<tree_type>& token, std::ostream& ost);
	virtual void visit_node(node* current, std::ostream& ost, const std::basic_string<tree_type>& token) {}
private:
	virtual void copy(Basic_Type_Tree& destination, const Basic_Type_Tree& source);
	virtual void copy_node_loop(node* destination, node* source);
	virtual void move(Basic_Type_Tree&& destination, Basic_Type_Tree&& source) noexcept;
};

template<typename content_type>
class CharTree;
template<typename content_type>
class WCharTree;

#include "DS_CharTree.ipp"

