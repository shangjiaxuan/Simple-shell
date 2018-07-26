//For dynamic linking on different platforms

#pragma once

#ifdef _WIN32

//for quick working with native strings without converting things
//it is best to use simple cout for now because wcout itself has problems
//with certain characters in windows console
//May be fixed in the future (or maybe use a combination of utf-8 and OEMCP)
#ifdef _UNICODE
#define mcout std::wcout
#else
#define mcout std::cout
#endif

//macros for importing and exporting dll
#ifdef _WINDLL
#define DLL_PORT extern "C" __declspec(dllexport)
#endif
#ifndef _WINDLL
#define DLL_PORT extern "C" __declspec(dllimport)
#endif
#endif
