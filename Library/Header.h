#pragma once

//file that includes all the required standard library, other files can include only this header

//make the files that only include common macros included everywhere

#include "Macros.h"

#include <tchar.h>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <filesystem>
#include <cstring>
#include <sstream>
#include <cassert>
#include <stack>
#include <stdexcept>
#include <cmath>
#include <map>

#ifdef _WIN32
#include <Windows.h>
#include <shlobj.h>
#undef max
#undef min
#endif


namespace fs = std::experimental::filesystem;
