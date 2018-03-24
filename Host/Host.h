#pragma once

#include "Header.h"

#include "Platform.h"

bool go_to_beginning;
int arg_number;											//for incorporating multiple use arguments
void prompt();											//for working with prompts
void manual();

inline bool operator==(const std::string str1, const std::string& str2) {
	if (str1.compare(str2) == 0) { return true; }
	return false;
}

inline bool operator==(const std::wstring str1, const std::wstring& str2) {
	if (str1.compare(str2) == 0) { return true; }
	return false;
}



#ifdef _WIN32
void after_start_selector(std::vector<std::wstring>);	//select the function to call if no original argument is found
void Launch(const std::wstring& str);

void Calledcalculator(std::istream& ist);
void CalledSwapEnc();

const std::wstring Exit{ L"exit" };
const std::wstring Cd{ L"cd" };
const std::wstring Calculator{ L"calculator" };
const std::wstring Man{ L"man" };
const std::wstring Exec{ L".exe" };
const std::wstring SwapEnc{ L"SwapEnc" };

inline bool isexecutable(std::wstring& str) {
	if (str.length() > 4 && str.substr(str.length() - 4, 4) == Exec) { return true; }
	if (str.back() == L'"' && str.length() > 5 && str.substr(str.length() - 5, 4) == Exec) { return true; }
	return false;
}
#endif

