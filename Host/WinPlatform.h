#pragma once

#include "Header.h"
#include <Windows.h>

namespace WIN {
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

	std::vector<std::wstring> Get_input();
	std::wstring parse_input();

	void Change_directory(std::wstring);

	void Handle_Error(std::exception&);

	void call(const LPCWSTR library, const LPCSTR function);
	template <class  type>
	void call(const LPCWSTR library, const LPCSTR function, type& pass);

	template void call<std::istream>(const LPCWSTR, const LPCSTR, std::istream&);


}