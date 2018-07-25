//For dynamic linking on different platforms

#pragma once

#ifdef _WIN32
#ifdef _WINDLL
#define DLL extern "C" __declspec(dllexport)
#endif
#ifndef _WINDLL
#define DLL extern "C" __declspec(dllimport)
#endif
#endif
