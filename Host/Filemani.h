#pragma once

#include "Names.h"

void Change_directory(nstring);

class fileman {
	
};
inline bool isexecutable(nstring& str) {
	if (str.length() > 4 && str.substr(str.length() - 4, 4) == Exec) { return true; }
#ifdef _UNICODE
	if (str.back() == L'"' && str.length() > 5 && str.substr(str.length() - 5, 4) == Exec) { return true; }
#endif
#ifdef _MBCS
	if (str.back() == '"' && str.length() > 5 && str.substr(str.length() - 5, 4) == Exec) { return true; }
#endif
	return false;
}
