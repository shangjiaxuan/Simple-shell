#pragma once

#include "Header.h"
#include <Windows.h>

std::wstring parse_input();

char* convert_string(std::string);
std::wstring String_input2Wstring_input(std::string&);
std::vector<std::wstring> Get_input();
LPSTR UnicodeToMByte(LPCWSTR unicodeStr, LPSTR multiByteStr, DWORD size);
void Change_directory(std::wstring);

LPWSTR Convert2Unicode(const LPSTR multiByteStr, const LPWSTR unicodeStr, DWORD size);

void Handle_Error(std::exception&);