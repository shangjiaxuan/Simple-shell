#pragma once

#include "Header.h"
#include "Parser.h"			//needs to pass commands on to the parser

class UI {
public:

	void loop(int argc, char* argv[]) const;

private:

	//prompt shown with each input of command
	//shows the path of current working directory
	static void prompt();

	//a switch for showing the welcome message again
	bool go_to_beginning = false;

	//following are functions for parsing the direct input into strict commands

	//for pasring a single argument
	static nstring parse_input();

	//for making vector of parsed arguments
	//and then pass on to the command parser
	static std::vector<nstring> Get_input();

	//certain characters not on the keyboard are supported on console
	//with the character "\" (eg '\a')
	//!!!currently not incorporated
	static bool change(const char& a);
};
