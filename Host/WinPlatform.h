﻿#pragma once

//header for the Windows-specific functions

#ifdef _WIN32

#include "Header.h"

//operation class for simple conversion between UTF-16 and MBCS
class convert {
public:
	static std::wstring string2wstring(std::string&);
	static std::string wstring2string(std::wstring&);
	static std::wstring MBC2utf16(const LPSTR&);
	static std::string UTF16_2mbcs(const LPWSTR&);
protected:
	static DWORD UNC_size(const LPSTR&);
	static DWORD MBCS_size(const LPWSTR&);
};

//for calling functions in dlls
template<typename rtn, typename passed>
rtn call(const nchar* library, const char* function, passed* pass);

//template void call<void>(const nchar* library, const char* function, void* null);

struct Lnk_Info {
	Lnk_Info():
		target_attributes{WIN32_FIND_DATA{}},
		target_path{fs::path()},
		cur_dir{fs::path()},
		icon_path{fs::path()},
		icon_index{NULL},
		show_cmd{NULL},
		description{TEXT("")},
		arguments{TEXT("")} {}

	WIN32_FIND_DATA target_attributes;
	fs::path target_path;
	fs::path cur_dir;
	fs::path icon_path;
	int icon_index;
	int show_cmd;
	nstring description;
	nstring arguments;

	bool operator==(const Lnk_Info& compared) const {
		return this->target_path == compared.target_path;
	};

	bool operator!=(const Lnk_Info& compared) const {
		return !((*this) == compared);
	};
};


Lnk_Info get_LnkInfo(const fs::path& LnkFile_Path);

#endif
