#pragma once
#include "Header.h"

#include "Names.h"								//currently "isexecutable" function relies on this

namespace fileman {
	//function for seeing if a file is an executable
	//nearly means ".exe" on windows
	//may add the feature that checks on registry
	//and gets other programs to open other files
	bool isexecutable(nstring& str);

	//function for changing the working directory
	void Change_directory(nstring dir);

};
