#pragma once

#include "Header.h"

#include "Names.h"

#include "Filemani.h"

class parser {
	public:
		void after_start_selector(std::vector<nstring>);	//select the function to call
		//secondary commands' parsers go here
	private:
		unsigned cur_arg = 0;				//the argument number that parser is currently parsing
};


//following are functions that only stands alone currently
void Launch(const nstring& str);			//for launching other executables
void manual();								//for calling out the manual page
											//manuals will be encased in a dll in the future

