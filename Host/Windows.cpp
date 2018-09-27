#include "Header.h"
#include "Host.h"			//for cmdline struct
#include "Launch.h"			//for PELaunch class

///////////////////////////////////////////////////
//Code specific to Windows platform

//#include <functional>
#ifdef _WIN32
#include "WinPlatform.h"

using namespace std;

///////////////////////////////////////////////////
//Following code referenced mostly from MSDN forum

///////////////////////////////////////////////////////////////
//For loading dynamic library
template<typename rtn, typename passed>
typename std::enable_if<!std::is_same<rtn, void>::value, rtn>::type
call(const nchar* library, const char* function, passed* pass) {
	const HINSTANCE hInst = LoadLibrary(library);
	if(!hInst) {
		std::cerr << "Error!: Cannot load " << library << " for access!" << std::endl;
		throw std::runtime_error("call: Library loading failed!");
	}
	rtn val;
	if(!pass) {
		typedef rtn (*_func)();
		const _func func = _func(GetProcAddress(hInst, function));
		try {
			val = func();
		} catch(...) {
			std::cerr << "call: Unknown error\n";
			FreeLibrary(hInst);
			std::cin.get();
		}
	} else {
		//probably implement a method here to see if the function can
		//take multipule arguments stored in a inherited class, and somehow
		//make the following code automatically adjust to the member variables
		//or maybe jst pass a void pointer, but non-pure-c structures may cause
		//problems here
		typedef rtn (*_func)(const passed&);
		const _func func = _func(GetProcAddress(hInst, function));
		try {
			val = func(*pass);
		} catch(...) {
			std::cerr << "call: Unknown error\n";
			FreeLibrary(hInst);
			std::cin.get();
		}
	}
	FreeLibrary(hInst);
	std::cin.get();
	return val;
}

template<typename rtn, typename passed>
typename std::enable_if<std::is_same<rtn, void>::value, void>::type
call(const nchar* library,
	const char* function,
	typename std::enable_if<!std::is_same<passed, void>::value, passed>::type* pass) {

	const HINSTANCE hInst = LoadLibrary(library);
	if(!hInst) {
		std::cerr << "Error!: Cannot load " << library << " for access!" << std::endl;
		throw std::runtime_error("call: Library loading failed!");
	}
	if(!pass) {
		typedef void (*_func)();
		const _func func = _func(GetProcAddress(hInst, function));
		try {
			func();
		} catch(...) {
			std::cerr << "call: Unknown error\n";
			FreeLibrary(hInst);
			std::cin.get();
		}
	} else {
		if(std::is_same<passed, void>::value) {
			FreeLibrary(hInst);
			throw std::runtime_error(
				"ISO C++ does not allow indirection on operand of type void*\ntry using other method of sending information");
		}
		typedef void (*_func)(const passed&);
		const _func func = _func(GetProcAddress(hInst, function));
		try {
			func(*pass);
		} catch(...) {
			std::cerr << "call: Unknown error\n";
			FreeLibrary(hInst);
			std::cin.get();
		}
	}
	FreeLibrary(hInst);
	std::cin.get();
}

template<typename rtn, typename passed>
typename std::enable_if<std::is_same<rtn, void>::value, void>::type
call(const nchar* library,
	const char* function,
	typename std::enable_if<std::is_same<passed, void>::value, passed>::type* pass) {
	if(pass) {
		throw std::runtime_error(
			"ISO C++ does not allow indirection on operand of type void*\ntry using other method of sending information");
	}
	const HINSTANCE hInst = LoadLibrary(library);
	if(!hInst) {
		std::cerr << "Error!: Cannot load " << library << " for access!" << std::endl;
		throw std::runtime_error("call: Library loading failed!");
	}
	typedef void (*_func)();
	const _func func = _func(GetProcAddress(hInst, function));
	try {
		func();
	} catch(...) {
		std::cerr << "call: Unknown error\n";
		FreeLibrary(hInst);
		std::cin.get();
	}
	FreeLibrary(hInst);
	std::cin.get();
}

//currently only support the same character set for argv
template<>
inline void call<void, cmdline>(const nchar* library, const char* function, cmdline* cmd) {
	const HINSTANCE hInst = LoadLibrary(library);
	if(!hInst) {
		std::cerr << "Error!: Cannot load " << library << " for access!" << std::endl;
		throw std::runtime_error("call: Library loading failed!");
	}
	typedef void (*_func)(size_t, char**);
	const _func func = _func(GetProcAddress(hInst, function));
	try {
		func(cmd->argc, cmd->argv);
	} catch(...) {
		std::cerr << "call: Unknown error\n";
		FreeLibrary(hInst);
		std::cin.get();
	}
	FreeLibrary(hInst);
	std::cin.get();
}

template void call<void, std::istream>(const nchar* library, const char* function, std::istream* pass);
template void call<void, void>(const nchar* library, const char* function, void* pass);


//////////////////////////////////////


#endif
