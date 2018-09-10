#pragma once

#define _STRING(str) #str
#define STRING(str) _STRING(str)

#define DLL_VERSION 0.0.1.0
#define ENC_VERSION 0.0.1.0
#define CALC_VERSION 0.0.0.0

//macros for importing and exporting dll
#ifdef _WINDLL
#define DLL_PORT extern "C" __declspec(dllexport)
#endif
#ifndef _WINDLL
#define DLL_PORT extern "C" __declspec(dllimport)
#endif

