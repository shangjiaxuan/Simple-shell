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
			cin.get();
		}
		FreeLibrary(hInst);
		cin.get();
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
		return arguments;
	}
	//It seems that vectors are not good containers in this case and it seems that it needs a 
	//structure that incorporate strings and pointers to next wstring2string (only one direction needed)

	bool change(const char& a) {
		if (a == '\a' || a == '\b' || a == '\f' || a == '\n' || a == '\r' || a == '\t' || a == '\'' || a == '\"') {
			return true;
		}//'\\'will be dealt with else where. cannot afford to use it too much in cmd shell 
		//numbers meaning characters are ignored
		return false;
	}


	std::wstring parse_input() {
		convert This;
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
//				cin.putback(c);
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
		std::wstring rtn = This.string2wstring(input);
		//	wchar_t temp= std::cin.get();
		//	if (temp!=L'\"'&&temp!=L'\\') {
		//		std::cin.putback(temp);
		//	}
		//	std::cin >> rtn;
		return rtn;
	}

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
		convert This;
		string temp = e.what();
		MessageBox(NULL, This.string2wstring(temp).c_str(), NULL, MB_OK);
	}
}
#endif
