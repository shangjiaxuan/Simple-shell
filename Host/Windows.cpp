//#include "../../../../../../Program Files (x86)/Windows Kits/10/Include/10.0.16299.0/um/winnls.h"
#ifdef _WIN32

#include "WinPlatform.h"
#include "Library.h"

using namespace std;
namespace WIN {


	///////////////////////////////////////////////////////////////
	//Windows method for loading shared library
	void call(const LPCWSTR library, const LPCSTR function) {
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
		}
		FreeLibrary(hInst);
	}

	template <class  type>
	void call(const LPCWSTR library, const LPCSTR function, type& pass) {
		HINSTANCE hInst = LoadLibrary(library);
		if (!hInst) {
			std::cerr << "Error!: Cannot load " << library << " for access!" << std::endl;
			throw std::runtime_error("call: Library loading failed!");
		}
		typedef void(*_func)(type&);
		_func func = _func(GetProcAddress(hInst, function));
		try {
			func(pass);
			FreeLibrary(hInst);
		}
		catch (...) {
			std::cerr << "call: Unknown error\n";
			FreeLibrary(hInst);
		}
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


	//////////////////////////////////////////////////////
	//Windows UTF-16 path
	void Change_directory(std::wstring dir) {
		try {
			experimental::filesystem::current_path(dir);
		}
		catch (experimental::filesystem::filesystem_error& e) {
			cerr << e.what() << '\n' << endl;
			throw runtime_error("Error: Cannot chage into that working directory!\n");
		}
	}


	//////////////////////////////////////////////////////


	//########################################################
	//Windows I/O APIs wrapped in Functions
	std::vector<std::wstring> Get_input() {
		std::vector<std::wstring> arguments;
		while (std::cin.peek() != '\n') {
			arguments.push_back(parse_input());
		}
		cin.get();
		cin.get();
		return arguments;
	}
	//It seems that vectors are not good containers in this case and it seems that it needs a 
	//structure that incorporate strings and pointers to next string (only one direction needed)

	bool change(char a) {
		if (a == '\a' || a == '\b' || a == '\f' || a == '\n' || a == '\r' || a == '\t' || a == '\'' || a == '\"') {
			return true;
		}//'\\'will be dealt with else where. cannot afford to use it too much in cmd shell 
		//numbers meaning characters are ignored
		return false;
	}


	std::wstring parse_input() {
		std::string input;
		//	std::cin >> input;
		char c;
		c = cin.peek();
		if (c == ' ') {
			do {
				cin.get(c);
			} while (c == ' ');
			cin.putback(c);
		}
		//	if(c != '\"') {
		//		cin >> input;
		//	} else 
		while (true) {
			if (c == '\"') {
				cin.get();
				cin.get(c);
				while (c != '\"') {
					//				if (c == '\\') {
					//					char a;
					//					cin.get(a);
					//					if (a == '\'' || a == '\"') {
					//						input += a;
					//						cin.get(c);
					//						continue;
					//					}
					//					if(a=='\n') {
					//						goto stop;
					//					}
					//					cin.putback(a);
					//				}
					if (c == '\n') {
						break;
					}
					input += c;
					cin.get(c);
				}
			}
			else if (c == '\n') {
				cin.putback(c);
				break;
			}
			else {
				cin.get(c);
				input += c;
			}
			c = cin.peek();
			if (c == ' ') {
				do {
					cin.get(c);
				} while (c == ' ');
				cin.putback(c);
				break;
			}
		}
		//	stop:
		std::wstring rtn = String_input2Wstring_input(input);
		//	wchar_t temp= std::cin.get();
		//	if (temp!=L'\"'&&temp!=L'\\') {
		//		std::cin.putback(temp);
		//	}
		//	std::cin >> rtn;
		return rtn;
	}

	LPWSTR MBC2utf16(const LPSTR multiByteStr, const LPWSTR unicodeStr, DWORD size) {
		if (MultiByteToWideChar(CP_OEMCP, MB_ERR_INVALID_CHARS, multiByteStr, -1, unicodeStr, size) == 0) {
			throw std::runtime_error("Error converting to UTF16");
		}
		return unicodeStr;
	}

	DWORD Get_Needed_Unicode_size(const LPSTR multiByteStr) {
		DWORD rtn = MultiByteToWideChar(CP_OEMCP, MB_ERR_INVALID_CHARS, multiByteStr, -1, NULL, 0);
		return rtn;
	}

	std::wstring String_input2Wstring_input(std::string& str) {
		DWORD size = Get_Needed_Unicode_size(const_cast<LPSTR>(str.c_str()));
		if (size >= 8192) { throw std::runtime_error("Input too long!"); }
		wchar_t temp[8192];												//the size of console input limit
		MBC2utf16(const_cast<LPSTR>(str.c_str()), temp, size);
		std::wstring rtn = temp;
		rtn.resize(size - 1);
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
			std::cout << FALSE;
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

	////////////////////////////////////
	//Windows method for handling errors
	void Handle_Error(std::exception& e) {
		TCHAR a[100];
		MessageBox(NULL, MBC2utf16(const_cast<LPSTR>(e.what()), a, 100), NULL, MB_OK);
	}

}
#endif
