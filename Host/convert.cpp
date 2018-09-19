#include "Header.h"

///////////////////////////////////////////////////
//Code specific to Windows platform

///////////////////////////////////////////////////
//Following code referenced mostly from MSDN forum

#ifdef _WIN32
#include "WinPlatform.h"
using namespace std;

//namespace WIN {
wstring convert::string2wstring(string& str) {
	return MBC2utf16(const_cast<LPSTR>(str.c_str()));
}

string convert::wstring2string(wstring& wst) {
	return UTF16_2mbcs(const_cast<LPWSTR>(wst.c_str()));
}

DWORD convert::UNC_size(const LPSTR& multiByteStr) {
	return MultiByteToWideChar(CP_OEMCP, MB_ERR_INVALID_CHARS, multiByteStr, -1, nullptr, 0);
}

DWORD convert::MBCS_size(const LPWSTR& unicodeStr) {
	return WideCharToMultiByte(CP_OEMCP, NULL, unicodeStr, -1, nullptr, 0, nullptr, nullptr);
}

wstring convert::MBC2utf16(const LPSTR& multiByteStr) {
	const DWORD size = UNC_size(multiByteStr);
	// ReSharper disable once CppLocalVariableMayBeConst
	LPWSTR unicodeStr = new WCHAR[size];
	if(MultiByteToWideChar(CP_OEMCP, MB_ERR_INVALID_CHARS, multiByteStr, -1, unicodeStr, size) == 0) {
		throw runtime_error("Error converting to UTF16");
	}
	wstring rtn = unicodeStr;
	delete[] unicodeStr;
	return rtn;
}

///////////////////////////////////////////////////////////////////////////////////////
//Need some work to integrate this. Non-ASCII characters cannot be displayed on console
//with wcout right now and needs to converted to MBCS
string convert::UTF16_2mbcs(const LPWSTR& unicodeStr) {
	const DWORD size = MBCS_size(unicodeStr);
	// ReSharper disable once CppLocalVariableMayBeConst
	LPSTR multiByteStr = new CHAR[size];
	char default_char = '?';
	BOOL use_default = true;
	if(WideCharToMultiByte(CP_OEMCP, NULL, unicodeStr, -1, multiByteStr, size, &default_char, &use_default) == 0) {
		throw runtime_error("Error converting to MBCS");
	}
	string rtn = multiByteStr;
	delete[] multiByteStr;
	return rtn;
}

/////////////////////////////////////////////////////////////////////////////////////////
//}

#endif
