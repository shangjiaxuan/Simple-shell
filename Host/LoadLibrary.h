#pragma once

#include "Host.h"			//for cmdline struct
#include "WinPlatform.h"

//for calling functions in dlls
// template<typename rtn, typename passed>
// rtn call(const nchar* library, const char* function, passed& pass) {
// 	return rtn{};
// };

#ifdef _WIN32

using namespace std;

DECLSPEC_NOINLINE
HINSTANCE try_load_library(const nchar* library);

////////////////////////////////////////////////////////////////////////////////////

//following code adapted form sources like the one bellow
//https://stackoverflow.com/questions/8696653/dynamically-load-a-function-from-a-dll

///////////////////////////////////////////////////////////////
//For loading dynamic library
template<typename rtn, typename passed>
typename std::enable_if<!std::is_same<rtn, void>::value && !std::is_same<passed, void>::value, rtn>::type
call(const nchar* library, const char* function, passed pass) {
	const HINSTANCE hInstance = try_load_library(library);
	rtn val;
	//probably implement a method here to see if the function can
	//take multiple arguments stored in a inherited class, and somehow
	//make the following code automatically adjust to the member variables
	//or maybe jst pass a void pointer, but non-pure-c structures may cause
	//problems here
	typedef rtn (*_func)(passed);
	const _func func = _func(GetProcAddress(hInstance, function));
	if(!func) {
		FreeLibrary(hInstance);
		throw runtime_error("call: Cannot find function in library!");
	}
	__try {
		val = func(pass);
	} __except(EXCEPTION_EXECUTE_HANDLER) {
		FreeLibrary(hInstance);
		std::cin.get();
		throw runtime_error("SEH error executing function as \"rtn func(passed)\"!\n");
	}
	FreeLibrary(hInstance);
	std::cin.get();
	return val;
}

template<typename rtn, typename passed>
typename std::enable_if<std::is_same<rtn, void>::value && !std::is_same<passed, void>::value, void>::type
call(const nchar* library,
	const char* function,
	passed pass) {
	HINSTANCE hInstance = try_load_library(library);
	typedef void (*_func)(passed);
	const _func func = _func(GetProcAddress(hInstance, function));
	if(!func) {
		FreeLibrary(hInstance);
		throw runtime_error("call: Cannot find function in library!");
	}
	__try {
		func(pass);
	} __except(EXCEPTION_EXECUTE_HANDLER) {
		FreeLibrary(hInstance);
		std::cin.get();
		throw runtime_error("SEH error executing function as \"void func(passed pass)\"!\n");
	}
	FreeLibrary(hInstance);
	std::cin.get();
}

template<typename rtn, typename passed>
typename std::enable_if<!std::is_same<rtn, void>::value && std::is_same<passed, void>::value, rtn>::type
call(const nchar* library,
	const char* function) {
	const HINSTANCE hInstance = try_load_library(library);
	rtn val;
	typedef rtn (*_func)();
	const _func func = _func(GetProcAddress(hInstance, function));
	if (!func) {
		FreeLibrary(hInstance);
		throw runtime_error("call: Cannot find function in library!");
	}
	__try {
		val = func();
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {
		FreeLibrary(hInstance);
		std::cin.get();
		throw runtime_error("SEH error executing function as \"void func(passed pass)\"!\n");
	}
	FreeLibrary(hInstance);
	std::cin.get();
	return val;
}

template<typename rtn, typename passed>
typename std::enable_if<std::is_same<rtn, void>::value && std::is_same<passed, void>::value, void>::type
call(const nchar* library,
	const char* function) {
	HINSTANCE hInstance = try_load_library(library);
	typedef void (*_func)();
	const _func func = _func(GetProcAddress(hInstance, function));
	if(!func) {
		FreeLibrary(hInstance);
		throw runtime_error("call: Cannot find function in library!");
	}
	__try {
		func();
	} __except(EXCEPTION_EXECUTE_HANDLER) {
		FreeLibrary(hInstance);
		std::cin.get();
		throw runtime_error("SEH error executing function as \"void func()\"!\n");
	}
	FreeLibrary(hInstance);
	std::cin.get();
}

template<>
int call<int, cmdline<char>>(const nchar* library, const char* function, cmdline<char> cmd);  // NOLINT(readability-inconsistent-declaration-parameter-name)
/*
template void call<void, std::istream&>(const nchar* library, const char* function, std::istream& pass);
template void call<void, void>(const nchar* library, const char* function);
*/

//////////////////////////////////////


#endif
