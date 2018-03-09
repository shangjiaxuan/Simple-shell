#ifdef _WIN32

#include "WinPlatform.h"
#include "Library.h"

//########################################################
//Windows I/O APIs wrapped in Functions
std::vector<std::wstring> Get_input() {
	std::vector<std::wstring> arguments;
	while(std::cin.peek()!='\n'&& std::cin) {
		arguments.push_back(parse_input());
	}
	return arguments;
}

std::wstring parse_input() {
	std::string input;
	std::cin >> input;
	std::wstring rtn = String_input2Wstring_input(input);
	//	wchar_t temp= std::cin.get();
	//	if (temp!=L'\"'&&temp!=L'\\') {
	//		std::cin.putback(temp);
	//	}
	//	std::cin >> rtn;
	return rtn;
}

void Convert2Unicode(const char* multiByteStr, wchar_t* unicodeStr, DWORD size) {
	if(MultiByteToWideChar(GetACP(), MB_ERR_INVALID_CHARS, multiByteStr, -1, unicodeStr, size)==0) {
		throw std::runtime_error("Error converting to UTF16");
	}
}

DWORD Get_Needed_Unicode_size(const char* multiByteStr) {
	DWORD rtn = MultiByteToWideChar(GetACP(), MB_ERR_INVALID_CHARS, multiByteStr, -1, nullptr, 0);
	return rtn;
}

std::wstring String_input2Wstring_input(std::string& str) {
	DWORD size = Get_Needed_Unicode_size(str.c_str());
	if (size >= 8192) { throw std::runtime_error("Input too long!"); }
	wchar_t temp[8192];												//the size of console input limit
	Convert2Unicode(str.c_str(), temp, size);
	std::wstring rtn = temp;
	rtn.resize(size-1);
	return rtn;
}

///////////////////////////////////////////////////////////////////////////////////////
//Need some work to integrate this. Non-ASCII characters cannot be displayed on console
//with wcout right now and needs to converted to MBCS
LPSTR UnicodeToMByte(LPCWSTR unicodeStr, LPSTR multiByteStr, DWORD size)
{
	DWORD minSize = WideCharToMultiByte(
		CP_OEMCP, WC_ERR_INVALID_CHARS,
		unicodeStr,
		-1,
		NULL,
		0,
		NULL,
		FALSE);
	if (size < minSize)
	{
		std::cout<< FALSE;
	}
	// Convert string from Unicode to multi-byte.
	WideCharToMultiByte(CP_OEMCP, NULL, unicodeStr, -1, multiByteStr, size, NULL, FALSE);
	return multiByteStr;
}
/////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////
//Windows API for launching exectutables
void Launch(const std::wstring& str) {
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	const LPWSTR cmd = LPWSTR(str.c_str());

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	// Start the child process.
	if (!CreateProcessW(NULL,   // No module name (use command line)
		cmd,			 // Command line
		NULL,           // Process handle not inheritable
		NULL,           // Thread handle not inheritable
		FALSE,          // Set handle inheritance to FALSE
		0,              // No creation flags
		NULL,           // Use parent's environment block
		NULL,           // Use parent's starting directory
		&si,            // Pointer to STARTUPINFO structure
		&pi)           // Pointer to PROCESS_INFORMATION structure
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


///////////////////////////////////////////////////
//Windows method for loading shared library
void Calledcalculator(std::istream& ist) {
	HINSTANCE hInst = LoadLibrary(L"Calculator.dll");
	if (!hInst) {
		std::cout << "Error!: Cannot load \"Calculator.dll\" for access!" << std::endl;
		throw std::runtime_error("Calculator: Library loading failed!");
	}
	const void_ist_ptr calculator = void_ist_ptr(GetProcAddress(hInst, "calculator"));
	try {
		calculator(ist);
		FreeLibrary(hInst);
	}
	catch (...) {
		std::cerr << "Calculator: Unknown error\n";
		FreeLibrary(hInst);
	}
}

#endif
