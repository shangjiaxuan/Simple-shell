/////////////////////////////////////////////////
//The core command parser
//Used for parsing commands
//call the correct functions for furthur parsing
//or find the function hierarchically directly
//if the function is considered miscellaneous

#pragma once

#include "Header.h"

#include "Names.h"			//required for the command names

#include "Filemani.h"		//required for file management functions

class parser {
	public:
		void after_start_selector(std::vector<nstring>);	//select the function to call

		//secondary commands' parsers go here
		void manual();								//for calling out the manual page
													//manuals will be encased in dlls in the future
													//this function will call the corresponding man()
													//functions in the dlls.

	private:
		unsigned cur_arg = 0;				//the argument number that parser is currently parsing
};


//following are functions that only stands alone currently
void Launch(const nstring& str);			//for launching other executables
