#pragma once

#include <iostream>
#include <filesystem>
#include <fstream>
#include "Windows.h"
#include "winnt.h"

namespace fs = std::experimental::filesystem;

class BinaryFileReader:
	public std::fstream {
public:
	BinaryFileReader& read_WORD(WORD* buffer, std::streamsize num);
	BinaryFileReader& read_DWORD(DWORD* buffer, std::streamsize num);
	BinaryFileReader& read_LONG(LONG* buffer, std::streamsize num);
};


