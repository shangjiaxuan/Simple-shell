#pragma once
#include "Header.h"
#include "Names.h"		//for isexecutable ... functions 

namespace fileman {
	//function for seeing if a file is an executable
	//nearly means ".exe" on windows
	//may add the feature that checks on registry
	//and gets other programs to open other files
	bool isexecutable(nstring& str);
	bool isshelllink(nstring& str);

	//function for changing the working directory
	void Change_directory(const nstring& dir);

	class BinaryFileReader : public std::ifstream {
	public:
		BinaryFileReader& read_WORD(WORD* buffer, std::streamsize num);
		BinaryFileReader& read_DWORD(DWORD* buffer, std::streamsize num);
		BinaryFileReader& read_LONG(LONG* buffer, std::streamsize num);
	};


};
