#pragma once

#include "Header.h"
#ifdef _WIN32
#include "WinPlatform.h"
#endif

struct Launch_Info;

//for launching other executables
void Launch(const Launch_Info& launch_info);

struct Launch_Info {
#ifdef _WIN32
	Launch_Info(fs::path exe_path) {
//		nstring filename = exe_path.filename().native();
		LPTSTR temp = new nchar[exe_path.native().size() + 1];
		stringcpy(temp, exe_path.native().size() + 1, exe_path.native().c_str());
		lpApplicationName = temp;
		lpCommandLine = new nchar[exe_path.native().size()+1];
		stringcpy(lpCommandLine, exe_path.native().size()+1, exe_path.native().c_str());
		bInheritHandles = false;
		dwCreationFlags = NULL;
		lpEnvironment = nullptr;
		temp = new TCHAR[exe_path.remove_filename().native().size()+1];
		stringcpy(temp, exe_path.native().size()+1, exe_path.native().c_str());
		lpCurrentDirectory = temp;
	}
	~Launch_Info() {
		delete[] lpCommandLine;
		delete[] lpCurrentDirectory;
	}
	//	No module name (use command line)
	LPCTSTR                lpApplicationName;
	//	Command line
	LPTSTR                 lpCommandLine;
	//	Process handle not inheritable
	//	LPSECURITY_ATTRIBUTES lpProcessAttributes;
	//	Thread handle not inheritable
	//	LPSECURITY_ATTRIBUTES lpThreadAttributes;
	//	Set handle inheritance to FALSE
	BOOL                  bInheritHandles;
	//	No creation flags
	DWORD                 dwCreationFlags;
	//	Use parent's environment block
	LPVOID                lpEnvironment;
	//	Use file location's directory
	LPCTSTR               lpCurrentDirectory;
	//	Pointer to STARTUPINFO structure
	//	LPSTARTUPINFO		  lpStartupInfo;
	//	Pointer to PROCESS_INFORMATION structure
	//	LPPROCESS_INFORMATION lpProcessInformation;
#endif


};
