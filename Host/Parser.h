/////////////////////////////////////////////////
//The core command parser
//Used for parsing commands
//call the correct functions for furthur parsing
//or find the function hierarchically directly
//if the function is considered miscellaneous

#pragma once

#include "Header.h"

#include "Host.h"
#include "Names.h"			//required for the command names

namespace parser {
	//select the function to call
	void after_start_selector(cmdline<nchar> cmd);

	//secondary commands' parsers go here

	//for calling out the manual page
	//manuals will be encased in dlls in the future
	//this function will call the corresponding man()
	//functions in the dlls.
	static void manual();

	//this function allocates space, add needs to be deallocated elsewhere
	void get_formatted_char(cmdline<char>&, std::vector<nstring>, size_t, size_t);
#ifdef _UNICODE
	void get_formatted_nchar(cmdline<nchar>&, std::vector<nstring>, size_t, size_t);
#endif
	//parse native char cmdline struct into nchar* string
	//may add other versions in the future (different charset)
	size_t ncmdline2nchar(const cmdline<nchar>& input, nchar* output, size_t max_output_size);
	//add a nchar* string to cmdline parser here (needed when shell link support is addressed)
	cmdline<nchar> nchar2ncmdline(const nchar* input);


	//the argument number that parser is currently parsing
	extern unsigned cur_arg;
};

//following are functions that only stands alone currently
