#pragma once

#include "../MyLibrary/Header.h"

#include "Host.h"			//for cmdline struct
#include "Filemani.h"

#ifdef _WIN32
class PELaunch :
public fileman::BinaryFileReader {
public:
	//for launching other executables
	void Launch() const;

	//read the PE file to determine subsystem
	//0: console
	//1: GUI
	//2: Other
	//3: failed to open file
	static int non_console(const fs::path& p);

	//read the beginning of DOS header and see if magic number
	//"MZ" exists
	static bool DOS_magic_number(const fs::path& p);

	PELaunch(fs::path exe_path);
	PELaunch(const cmdline<nchar>& cmd);
	PELaunch(const PELaunch& source);
	PELaunch& operator=(const PELaunch& source);
	PELaunch(PELaunch&& source) noexcept;
	PELaunch& operator=(PELaunch&& source) noexcept;

	//note that every time an instance goes out of scope,
	//the strings stored in memory are deleted
	~PELaunch() {
		destroy();
	}

	void destroy();
	static void copy(PELaunch& destination, const PELaunch& source);
	static void move(PELaunch& destination, PELaunch& source) noexcept;

private:
	//	path to the application
	LPCTSTR lpApplicationName{};
	//	Command line (complete commandline with module name)
	LPTSTR lpCommandLine{};
	//	Process handle not inheritable as default
	//	LPSECURITY_ATTRIBUTES lpProcessAttributes;
	//	Thread handle not inheritable as default
	//	LPSECURITY_ATTRIBUTES lpThreadAttributes;
	//	handle inheritance may be used for running other exes in the shell
	//	so no default false assumed here 
	BOOL bInheritHandles{};
	//	Creation flags e.g. CREATE_NEW_CONSOLE
	DWORD dwCreationFlags{};
	//	currently always NULL for now
	LPVOID lpEnvironment{};
	//	Use file location's directory
	LPCTSTR lpCurrentDirectory{};
	//	Pointer to STARTUPINFO structure
	//	LPSTARTUPINFO		  lpStartupInfo;
	//	Pointer to PROCESS_INFORMATION structure
	//	LPPROCESS_INFORMATION lpProcessInformation;
#endif


};
