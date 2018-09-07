#pragma once
#include "Header.h"

//file for declaring the constant strings, all simple commands go here

#ifdef WIN32

#ifdef _UNICODE
extern const std::wstring Exit;
extern const std::wstring Cd;
extern const std::wstring Calculator;
extern const std::wstring Man;
extern const std::wstring Exec;
extern const std::wstring SwapEnc;

extern const wchar_t* calcDLL;
extern const wchar_t* encDLL;

#endif

#ifdef _MBCS
extern const std::string Exit;
extern const std::string Cd;
extern const std::string Calculator;
extern const std::string Man;
extern const std::string Exec;
extern const std::string SwapEnc;

extern const char* calcDLL;
extern const char* encDLL;

#endif

#endif