#include <Windows.h>
#include <stdio.h>
#include <string>
#include <cstring>
#include <iostream>
#include <filesystem>

#include "Header.h"

#include "Platform.h"

std::wstring parse_input() {
	std::string input;
	std::cin >> input;
	std::wstring rtn = String_input2Wstring_input(input);
//	wchar_t temp= std::cin.get();
//	if (temp!=L'\"'&&temp!=L'\\') {
//		std::cin.putback(temp);
//	}
//	std::cin >> rtn;
	return rtn;
}

std::vector<std::wstring> Get_input() {
	std::vector<std::wstring> arguments;
//	while(std::cin.peek()!='\n'&& std::cin) {
		arguments.push_back(parse_input());
//	}
	return arguments;
}

void Change_directory(std::wstring dir) {
	
}

void Convert2Unicode(const char* multiByteStr, wchar_t* unicodeStr, DWORD size) {
	if(MultiByteToWideChar(GetACP(), MB_ERR_INVALID_CHARS, multiByteStr, -1, unicodeStr, size)==0) {
		throw std::runtime_error("Error converting to UTF16");
	}
}

DWORD Get_Needed_Unicode_size(const char* multiByteStr) {
	DWORD rtn = MultiByteToWideChar(GetACP(), MB_ERR_INVALID_CHARS, multiByteStr, -1, nullptr, 0);
	return rtn;
}

std::wstring String_input2Wstring_input(std::string& str) {
	DWORD size = Get_Needed_Unicode_size(str.c_str());
	if (size >= 8192) { throw std::runtime_error("Input too long!"); }
	wchar_t temp[8192];												//the size of console input limit
	Convert2Unicode(str.c_str(), temp, size);
	std::wstring rtn = temp;
	rtn.resize(size-1);
	return rtn;
}



char* UnicodeToMByte(LPCWSTR unicodeStr, LPSTR multiByteStr, DWORD size)
{
	DWORD minSize = WideCharToMultiByte(
		CP_OEMCP, WC_ERR_INVALID_CHARS,
		unicodeStr,
		-1,
		NULL,
		0,
		NULL,
		FALSE);
	if (size < minSize)
	{
		std::cout<< FALSE;
	}
	// Convert string from Unicode to multi-byte.
	WideCharToMultiByte(CP_OEMCP, NULL, unicodeStr, -1, multiByteStr, size, NULL, FALSE);
	return multiByteStr;
}



