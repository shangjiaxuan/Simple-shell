////////////////////////////////////////
//File for all the program-wide macros

#pragma once

//change the number bellow to change the in-app version number
//other compile-constant string macros can go here too

#define _STRING(str) #str
#define STRING(str) _STRING(str)

#define VERSION 0.0.0.0

//For specifying macros for partial cross-compiling

#ifdef _WIN32
//native strings and chars
#ifdef _UNICODE
#define nstring wstring
#define nchar wchar_t
#define stringcpy wcscpy_s
#define stringlen wcslen
#define stringcmp wcscmp
#endif

#ifdef _MBCS
#define nstring string
#define nchar char
#define stringcpy strcpy_s
#define stringlen strlen
#define stringcmp strcmp
#endif

#endif
