////////////////////////////////////////
//File for all the program-wide macros

#pragma once

//change the number bellow to change the in-app version number
//other compile-constant string macros can go here too

#define _STRING(str) #str
#define STRING(str) _STRING(str)

#define HOST_VERSION 0.0.0.0
#define DLL_VERSION 0.0.1.0
#define ENC_VERSION 0.0.1.0
#define CALC_VERSION 0.0.0.0
#define UJR2_VERSION 0.0.0.0

//For specifying macros for partial cross-compiling

#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
//native strings and chars
#ifdef _UNICODE
#define nstring wstring
#define nistream wistream
#define nostream wostream
#define nisstream wistringstream
#define nosstream wostringstream
#define nifstream wifstream
#define nofstream wofstream
#define nchar wchar_t
#define stringcpy wcscpy
#define stringlen wcslen
#define stringcmp wcscmp
#endif

#ifdef _MBCS
#define nstring string
#define nistream istream
#define nostream ostream
#define nisstream istringstream
#define nosstream ostringstream
#define nifstream ifstream
#define nofstream ofstream
#define nchar char
#define stringcpy strcpy
#define stringlen strlen
#define stringcmp strcmp
#endif

#endif

//macros for importing and exporting dynammic library
#ifdef _WINDLL
#define DLL_PORT extern "C" __declspec(dllexport)
#endif
#ifndef _WINDLL
#define DLL_PORT extern "C" __declspec(dllimport)
#endif
