#pragma once
#include "../MyLibrary/Header.h"
#include "Names.h"		//for isexecutable ... functions 

namespace fileman {
	//function for seeing if a file is an executable
	//nearly means ".exe" on windows
	//may add the feature that checks on registry
	//and gets other programs to open other files
	bool isexecutable(std::nstring& str);
	bool isshelllink(std::nstring& str);

	//function for changing the working directory
	void Change_directory(const std::nstring& dir);

	class BinaryFileReader : public std::ifstream {
	public:
		BinaryFileReader& read_WORD(WORD* buffer, std::streamsize num);
		BinaryFileReader& read_DWORD(DWORD* buffer, std::streamsize num);
		BinaryFileReader& read_LONG(LONG* buffer, std::streamsize num);
	};

	template<typename type>
	bool BinRead(type* buffer, std::streamsize num, std::istream& source) {
		if(!source) return false;
		source.read(reinterpret_cast<char*>(buffer), num * sizeof type);
		if(!source) return false;
		return true;
	}


};
