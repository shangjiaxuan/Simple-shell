#include "LoadLibrary.h"

HINSTANCE try_load_library(const nchar* library) {
	HINSTANCE hInstance = LoadLibrary(library);
	if (!hInstance) {
		std::cerr << "Error!: Cannot load \"";
#ifdef _MBCS
		std::cerr << library;
#endif
#ifdef _UNICODE
		std::cerr << convert::UTF16_2mbcs(library);
#endif
		std::cerr << "\" for access!" << std::endl;
		throw std::runtime_error("call: Library loading failed!");
	}
	return hInstance;
}

//must pass by reference in the current code because cmdline has a default destructor and 
//this conflicts with SEH error handling
template<>
int call<int, cmdline<char>&>(const nchar* library, const char* function, cmdline<char>& cmd) {  // NOLINT(readability-inconsistent-declaration-parameter-name)
	HINSTANCE hInst = try_load_library(library);
	int val;
	typedef int(*_func)(int, char**);
	const _func func = _func(GetProcAddress(hInst, function));
	if (!func) {
		FreeLibrary(hInst);
		throw runtime_error("call: Cannot find function in library!");
	}
	__try {
		val = func(int(cmd.argc), cmd.argv);
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {
		FreeLibrary(hInst);
		std::cin.get();
		throw runtime_error("SEH error executing function as \"void func(int argc, char* argv[])\"!\n");
	}
	FreeLibrary(hInst);
	std::cin.get();
	return val;
}

