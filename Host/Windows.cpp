///////////////////////////////////////////////////
//Code specific to Windows platform

///////////////////////////////////////////////////
//Following code referenced mostly from MSDN forum

#ifdef _WIN32
#include "WinPlatform.h"
using namespace std;
///////////////////////////////////////////////////////////////
//For loading dynamic library
void call(const nchar* library, const char* function) {
	HINSTANCE hInst = LoadLibrary(library);
	if (!hInst) {
		std::cerr << "Error!: Cannot load " << library << " for access!" << std::endl;
		throw std::runtime_error("call: Library loading failed!");
	}
	typedef void(*_func)();
	_func func = _func(GetProcAddress(hInst, function));
	try {
		func();
	}
	catch (...) {
		std::cerr << "call: Unknown error\n";
		FreeLibrary(hInst);
		cin.get();
	}
	FreeLibrary(hInst);
	cin.get();
}

template <class  type>
void call(const nchar* library, const char* function, type& pass) {
	HINSTANCE hInst = LoadLibrary(library);
	if (!hInst) {
		std::cerr << "Error!: Cannot load " << library << " for access!" << std::endl;
		throw std::runtime_error("call: Library loading failed!");
	}
	typedef void(*_func)(type&);
	_func func = _func(GetProcAddress(hInst, function));
	try {
		func(pass);
	}
	catch (...) {
		std::cerr << "call: Unknown error\n";
		FreeLibrary(hInst);
		cin.get();
	}
	FreeLibrary(hInst);
	cin.get();
}

/////////////////////////////////////////////////////////
//禁止library有返回值，既是为了extern"C",也是为了避免调用混乱
/*template <class rtn, class  type>
rtn call(const LPCWSTR library, const LPCSTR function, type& pass) {
HINSTANCE hInst = LoadLibrary(library);
if (!hInst) {
std::cerr << "Error!: Cannot load " << library << " for access!" << std::endl;
throw std::runtime_error("call: Library loading failed!");
}
typedef void(*_func)(type);
_func func = _func(GetProcAddress(hInst, function));	
rtn rt;
try {
rt = func(pass);
FreeLibrary(hInst);
}
catch (...) {
std::cerr << "call: Unknown error\n";
FreeLibrary(hInst);
}
return rt;
}*/


//////////////////////////////////////////////////////////////////
//Windows API for launching exectutables
void Launch(const nstring& str) {
	STARTUPINFOW si;
	PROCESS_INFORMATION pi;

	const LPWSTR cmd = LPWSTR(str.c_str());

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	// Start the child process.
	if (!CreateProcessW(NULL,   // No module name (use command line)
		cmd,					// Command line
		NULL,					// Process handle not inheritable
		NULL,					// Thread handle not inheritable
		FALSE,					// Set handle inheritance to FALSE
		0,						// No creation flags
		NULL,				    // Use parent's environment block
		NULL,					// Use parent's starting directory
		&si,					// Pointer to STARTUPINFO structure
		&pi)					// Pointer to PROCESS_INFORMATION structure
		)
	{
		printf("CreateProcess failed (%d).\n", GetLastError());
		return;
	}

	// Wait until child process exits.
	WaitForSingleObject(pi.hProcess, INFINITE);

	// Close process and thread handles.
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
}
//////////////////////////////////////

#endif
