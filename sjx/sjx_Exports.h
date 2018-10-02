#pragma once

#include "../Lib/Header.h"

#ifdef _WINDLL
DLL_PORT void SwapEnc();
DLL_PORT void calculator(std::istream& ist);
DLL_PORT int UJr2(int argc, char* argv[]);
#endif
