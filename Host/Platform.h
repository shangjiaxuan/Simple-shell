#pragma once

#ifdef WIN32

//native strings
#ifdef _UNICODE
#define nstring std::wstring
#define nchar wchar_t
#endif

#ifdef _MBCS
#define nstring std::string
#define nchar char
#endif

#endif



#ifdef _WIN32
#include "WinPlatform.h"
#endif
