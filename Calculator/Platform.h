//For dynamic linking on different platforms

#pragma once

#ifdef _WIN32
#ifdef _LIB
#define DLL extern "C" __declspec(dllexport)
#endif
#ifndef _LIB
#define DLL extern "C" __declspec(dllimport)
#endif
#endif
