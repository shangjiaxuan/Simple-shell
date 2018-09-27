
#pragma once

//file that includes all the required standard library, other files can include only this header

//make the files that only include common macros included everywhere

#include <tchar.h>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <filesystem>
#include <cstring>

#ifdef _WIN32
#include <Windows.h>
#include <shlobj.h>
#endif

#include "Macros.h"

namespace fs = std::experimental::filesystem;
