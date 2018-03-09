#pragma once

#include "Header.h"

bool start;
int arg_number;											//for incorporating multiple use arguments
void prompt();											//for working with prompts
void manual();

#ifdef _WIN32
void after_start_selector(std::vector<std::wstring>);	//select the function to call if no original argument is found
void Launch(const std::wstring& str);

void Calledcalculator(std::istream& ist);

const std::wstring Exit{ L"exit" };
const std::wstring Cd{ L"cd" };
const std::wstring Calculator{ L"calculator" };
const std::wstring Man{ L"man" };
const std::wstring Exec{ L".exe" };
#endif
