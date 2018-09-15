#pragma once

//header for the Windows-specific functions

#ifdef _WIN32

#include <Windows.h>
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

//void Handle_Error(std::exception&);

//functions for calling functions in dlls
struct cmdline {
	size_t argc;
	nchar** argv;
};
template <typename rtn, typename  passed>
rtn call(const nchar* library, const char* function, passed* pass);




//template void call<void>(const nchar* library, const char* function, void* null);
#endif
