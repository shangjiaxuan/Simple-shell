#pragma once

#include "Book.h"

class UI {
public:
	void UI_main();
private:
	bool on{ true };
	Book base;
	void prompt();
	void name_ver();
	void parser(/*std::istream& ist*/);

	void add();
	bool add_file(std::string cmd);
	void add_manual(std::string cmd);
	void add_stream(std::istream& ist);
	//base function
	void add_string(std::string line);

	void del();
	void del_manual(std::string cmd);
	bool del_file(std::string cmd);
	void del_stream(std::istream& ist);
	//base function
	void del_string(std::string line);

	void list();
	void list_book();
	void list_book_stream(std::ostream& ost);
	void list_index();
	void list_index_stream(std::ostream& ost);

	void find();
	void find_book();
	void find_token();

	//adds a string that is not considered to be a token
	std::string parse_bookname(std::string line);
	std::string parse_path(std::string line);

	void ignore_space(std::istream& ist);
};

