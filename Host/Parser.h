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
#include "Host.h"


class parser {
public:
	//select the function to call
	void after_start_selector(std::vector<nstring>);

	//secondary commands' parsers go here

	//for calling out the manual page
	//manuals will be encased in dlls in the future
	//this function will call the corresponding man()
	//functions in the dlls.
	static void manual();

private:
	//the argument number that parser is currently parsing
	unsigned cur_arg = 0;
};

//following are functions that only stands alone currently


