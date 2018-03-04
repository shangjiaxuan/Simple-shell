#pragma once
#include "Token_stream.h"

#ifdef _WIN32
#ifdef _LIB
#define DLL __declspec(dllexport)
#endif
#ifndef _LIB
#define DLL __declspec(dllimport)
#endif
#endif

