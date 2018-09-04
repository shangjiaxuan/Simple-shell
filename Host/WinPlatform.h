#pragma once

#include "Platform.h"
#include <Windows.h>

//namespace WIN {
#ifdef WIN32
	class convert {
	public:
		std::wstring string2wstring(std::string&);
		std::string wstring2string(std::wstring&);
		std::wstring MBC2utf16(const LPSTR&);
		std::string UTF16_2mbcs(const LPWSTR&);
	protected:
		DWORD UNC_size(const LPSTR&);
		DWORD MBCS_size(const LPWSTR&);
	};

//void Handle_Error(std::exception&);

void call(const nchar* library, const LPCSTR function);
template <class  type>
void call(const nchar* library, const LPCSTR function, type& pass);

template void call<std::istream>(const nchar*, const LPCSTR, std::istream&);

/*
inline bool operator==(const std::string& str1, const std::string& str2) {
if (str1.compare(str2) == 0) { return true; }
return false;
}

inline bool operator==(const std::wstring& str1, const std::wstring& str2) {
if (str1.compare(str2) == 0) { return true; }
return false;
}
*/


#endif
//}