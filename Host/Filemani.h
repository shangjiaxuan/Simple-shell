#pragma once
#include "Header.h"

#include "Names.h"								//currently "isexecutable" function relies on this

namespace fileman {
	bool isexecutable(nstring& str);			//function for seeing if a file is an executable
												//nearly means ".exe" on windows
												//may add the feature that checks on registry
												//and gets other programs to open other files
	void Change_directory(nstring dir);			//function for changing the working directory
};

