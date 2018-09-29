﻿#include "Header.h"

#include "Host.h"			//for cmdline struct

#ifdef _WIN32

using namespace std;

////////////////////////////////////////////////////////////////////////////////////
//following code adapted form sources like the one bellow
//https://stackoverflow.com/questions/8696653/dynamically-load-a-function-from-a-dll

///////////////////////////////////////////////////////////////
//For loading dynamic library
template<typename rtn, typename passed>
typename std::enable_if<!std::is_same<rtn, void>::value, rtn>::type
call(const nchar* library, const char* function, passed* pass) {
	HINSTANCE const hInst = LoadLibrary(library);
	if(!hInst) {
		std::cerr << "Error!: Cannot load " << library << " for access!" << std::endl;
		throw std::runtime_error("call: Library loading failed!");
	}
	rtn val;
	if(!pass) {
		typedef rtn (*_func)();
		const _func func = _func(GetProcAddress(hInst, function));
		if(!func) {
			FreeLibrary(hInst);
			throw runtime_error("call: Cannot find function in library!");
		}
		__try {
			val = func();
		} __except(EXCEPTION_EXECUTE_HANDLER) {
			FreeLibrary(hInst);
			std::cin.get();
			throw runtime_error("SEH error executing function as \"rtn func()\"!\n");
		}
	} else {
		//probably implement a method here to see if the function can
		//take multipule arguments stored in a inherited class, and somehow
		//make the following code automatically adjust to the member variables
		//or maybe jst pass a void pointer, but non-pure-c structures may cause
		//problems here
		typedef rtn (*_func)(const passed&);
		const _func func = _func(GetProcAddress(hInst, function));
		if(!func) {
			FreeLibrary(hInst);
			throw runtime_error("call: Cannot find function in library!");
		}
		__try {
			val = func(*pass);
		} __except(EXCEPTION_EXECUTE_HANDLER) {
			FreeLibrary(hInst);
			std::cin.get();
			throw runtime_error("SEH error executing function as \"rtn func(passed)\"!\n");
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
	HINSTANCE const hInst = LoadLibrary(library);
	if(!hInst) {
		std::cerr << "Error!: Cannot load " << library << " for access!" << std::endl;
		throw std::runtime_error("call: Library loading failed!");
	}
	if(!pass) {
		typedef void (*_func)();
		const _func func = _func(GetProcAddress(hInst, function));
		if(!func) {
			FreeLibrary(hInst);
			throw runtime_error("call: Cannot find function in library!");
		}
		__try {
			func();
		} __except(EXCEPTION_EXECUTE_HANDLER) {
			FreeLibrary(hInst);
			std::cin.get();
			throw runtime_error("SEH error executing function as \"void func()\"!\n");
		}
	} else {
		if(std::is_same<passed, void>::value) {
			FreeLibrary(hInst);
			throw std::runtime_error(
				"ISO C++ does not allow indirection on operand of type void*\ntry using other method of sending information");
		}
		typedef void (*_func)(const passed&);
		const _func func = _func(GetProcAddress(hInst, function));
		if(!func) {
			FreeLibrary(hInst);
			throw runtime_error("call: Cannot find function in library!");
		}
		__try {
			func(*pass);
		} __except(EXCEPTION_EXECUTE_HANDLER) {
			FreeLibrary(hInst);
			std::cin.get();
			throw runtime_error("SEH error executing function as \"void func(passed pass)\"!\n");
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
	HINSTANCE const hInst = LoadLibrary(library);
	if(!hInst) {
		std::cerr << "Error!: Cannot load " << library << " for access!" << std::endl;
		throw std::runtime_error("call: Library loading failed!");
	}
	typedef void (*_func)();
	const _func func = _func(GetProcAddress(hInst, function));
	if(!func) {
		FreeLibrary(hInst);
		throw runtime_error("call: Cannot find function in library!");
	}
	__try {
		func();
	} __except(EXCEPTION_EXECUTE_HANDLER) {
		FreeLibrary(hInst);
		std::cin.get();
		throw runtime_error("SEH error executing function as \"void func()\"!\n");
	}
	FreeLibrary(hInst);
	std::cin.get();
}

//currently only support the same character set for argv
template<>
inline void call<void, cmdline<char>>(const nchar* library, const char* function, cmdline<char>* cmd) {
	HINSTANCE const hInst = LoadLibrary(library);
	if(!hInst) {
		std::cerr << "Error!: Cannot load " << library << " for access!" << std::endl;
		throw std::runtime_error("call: Library loading failed!");
	}
	typedef void (*_func)(int, char**);
	const _func func = _func(GetProcAddress(hInst, function));
	if(!func) {
		FreeLibrary(hInst);
		throw runtime_error("call: Cannot find function in library!");
	}
	__try {
		func(cmd->argc, cmd->argv);
	} __except(EXCEPTION_EXECUTE_HANDLER) {
		FreeLibrary(hInst);
		std::cin.get();
		throw runtime_error("SEH error executing function as \"void func(int argc, char* argv[])\"!\n");
	}
	FreeLibrary(hInst);
	std::cin.get();
}

template void call<void, std::istream>(const nchar* library, const char* function, std::istream* pass);
template void call<void, void>(const nchar* library, const char* function, void* pass);


//////////////////////////////////////


#endif
