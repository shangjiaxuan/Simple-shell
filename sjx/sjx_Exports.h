#pragma once

#include "Header.h"

#ifdef _WINDLL
DLL_PORT void SwapEnc();
DLL_PORT void calculator(std::istream* ist);
#endif
