
/////////////////////////////////////////////////
//The core command parser
//Used for parsing commands
//call the correct functions for furthur parsing
//or find the function hierarchically directly
//if the function is considered miscellaneous

#pragma once

#include "Header.h"

#include "Host.h"


namespace parser {
	//select the function to call
	void after_start_selector(std::vector<nstring>);

	//secondary commands' parsers go here

	//for calling out the manual page
	//manuals will be encased in dlls in the future
	//this function will call the corresponding man()
	//functions in the dlls.
	static void manual();

	//this function allocates space, add needs to be deallocated elsewhere
	void get_formatted_char(::cmdline& , std::vector<nstring> , size_t, size_t );
#ifdef _UNICODE
	void get_formatted_nchar(::ncmdline& , std::vector<nstring> , size_t , size_t );
#endif
	inline size_t ncmdline2nchar(const ::ncmdline& input, nchar* output, size_t max_output_size) {
		size_t rtn = 0;
		if(!output || !max_output_size) {
			size_t i = 0;
			while(true) {
				rtn += stringlen(input.argv[i]);
				i++;
				rtn++;
				if(i >= input.argc) {
					break;
				}
			}
			return rtn;
		}
		nchar* nc_ptr = output;
		size_t copied_size = 0;
		size_t arg_size;
		size_t i = 0;
		while(true) {
			arg_size = stringlen(input.argv[i]);
			copied_size += arg_size;
			if(copied_size > max_output_size) {
				return 2; //chop off in middle of copy
			}
			stringcpy(nc_ptr, arg_size + 1, input.argv[i]);
			nc_ptr += arg_size;
			i++;
			if(i >= input.argc - 1) {
				break;
			}
			copied_size++;
			if(copied_size > max_output_size) {
				return 1; //chop off at space
			}
			*nc_ptr = 0x20; //space
			nc_ptr++;
		}
		return rtn;
	}

	//the argument number that parser is currently parsing
	extern unsigned cur_arg;
};

//following are functions that only stands alone currently
