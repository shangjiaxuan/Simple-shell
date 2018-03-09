#pragma once

#include "Header.h"
#include<Windows.h>


#ifdef _WIN32

char* convert_string(std::string);
std::wstring String_input2Wstring_input(std::string&);
std::vector<std::wstring> Get_input();
char* UnicodeToMByte(LPCWSTR unicodeStr, LPSTR multiByteStr, DWORD size);

void Change_directory(std::wstring);

#endif
