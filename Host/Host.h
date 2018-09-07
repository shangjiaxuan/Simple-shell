#pragma once

#include "Header.h"
#include "Parser.h"			//needs to pass commands on to the parser

class UI {
	public:
		void loop(int argc, char* argv[]);
	private:
		static void prompt();					//prompt shown with each input of command
												//shows the path of current working directory
		bool go_to_beginning = false;			//a switch for showing the welcome message again

		//following are functions for parsing the direct input into strict commands
		nstring parse_input();					//for pasring a single argument
		std::vector<nstring> Get_input();		//for making vector of parsed arguments
												//and then pass on to the command parser
		bool change(const char& a);				//certain characters are supported on console
												//with the character "\" (eg '\a')
												//!!!currently not incorporated
};

