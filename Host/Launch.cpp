#include "Launch.h"

using namespace std;

PELaunch::PELaunch(const cmdline<nchar>& cmd) {
	bInheritHandles = false;
	lpEnvironment = nullptr;
	dwCreationFlags = NULL;
	lpCurrentDirectory = nullptr;
	if(!cmd.argv || cmd.argc == 0 || !cmd.argv[0]) {
		lpCommandLine = nullptr;
		lpApplicationName = nullptr;
		return;
	}
	open(cmd.argv[0]);
	size_t required_size = stringlen(cmd.argv[0]) + 1;
	LPTSTR temp = new nchar[required_size];
	stringcpy(temp, required_size, cmd.argv[0]);
	lpApplicationName = temp;
	required_size = parser::ncmdline2nchar(cmd, nullptr, NULL);
	lpCommandLine = new nchar[required_size];
	parser::ncmdline2nchar(cmd, lpCommandLine, required_size);
}

//not const reference because exe_path.remove_filename() is called
PELaunch::PELaunch(fs::path exe_path) {
	//may not be necessary
	open(exe_path);
	//no specific info given, use default
	bInheritHandles = false;
	lpEnvironment = nullptr;
	dwCreationFlags = NULL;
	if(!*this) {
		lpCommandLine = nullptr;
		lpApplicationName = nullptr;
		lpCurrentDirectory = nullptr;
		return;
	}
	//Use path itself as commandline if given only a path without arguments
	//some apps do not work with NULL as lpCommandLine
	//command line is modifiable in UNICODE
	lpCommandLine = new nchar[exe_path.native().size() + 1];
	stringcpy(lpCommandLine, exe_path.native().size() + 1, exe_path.native().c_str());
	//a temporary pointer to modifyable data to initialize unmodifiable data
	nchar* temp = new nchar[exe_path.native().size() + 1];
	stringcpy(temp, exe_path.native().size() + 1, exe_path.native().c_str());
	lpApplicationName = temp;
	temp = new nchar[exe_path.remove_filename().native().size() + 1];
	stringcpy(temp, exe_path.native().size() + 1, exe_path.native().c_str());
	lpCurrentDirectory = temp;
}


PELaunch::PELaunch(const PELaunch& source) {
	//may not be necessary
	open(source.lpApplicationName);
	bInheritHandles = source.bInheritHandles;
	//may need fix when lpEnvironment is not always NULL
	lpEnvironment = source.lpEnvironment;
	dwCreationFlags = source.dwCreationFlags;
	size_t str_size = stringlen(source.lpCommandLine) + 1;
	lpCommandLine = new nchar[str_size];
	stringcpy(lpCommandLine, str_size, source.lpCommandLine);
	//initialize lpApplicationName
	str_size = stringlen(source.lpApplicationName) + 1;
	//create a temporary pointer to modifyable data to initialize unmodifiable data
	nchar* temp = new nchar[str_size];
	stringcpy(temp, str_size, source.lpApplicationName);
	lpApplicationName = temp;
	//initialize lpCurrentDirectory
	str_size = stringlen(source.lpCurrentDirectory) + 1;
	temp = new nchar[str_size];
	stringcpy(temp, str_size, source.lpCurrentDirectory);
	lpCurrentDirectory = temp;
}

PELaunch::PELaunch(PELaunch&& source) noexcept {
	//may not be necessary
	open(source.lpApplicationName);
	bInheritHandles = source.bInheritHandles;
	//may need fix when lpEnvironment is not always NULL
	lpEnvironment = source.lpEnvironment;
	dwCreationFlags = source.dwCreationFlags;
	size_t str_size = stringlen(source.lpCommandLine) + 1;
	lpCommandLine = new nchar[str_size];
	stringcpy(lpCommandLine, str_size, source.lpCommandLine);
	//initialize lpApplicationName
	str_size = stringlen(source.lpApplicationName) + 1;
	//create a temporary pointer to modifyable data to initialize unmodifiable data
	nchar* temp = new nchar[str_size];
	stringcpy(temp, str_size, source.lpApplicationName);
	lpApplicationName = temp;
	//initialize lpCurrentDirectory
	str_size = stringlen(source.lpCurrentDirectory) + 1;
	temp = new nchar[str_size];
	stringcpy(temp, str_size, source.lpCurrentDirectory);
	lpCurrentDirectory = temp;
}

PELaunch& PELaunch::operator=(PELaunch&& source) noexcept {
	if(this == &source) {
		return *this;
	}
	this->~PELaunch();
	//may not be necessary
	open(source.lpApplicationName);
	bInheritHandles = source.bInheritHandles;
	//may need fix when lpEnvironment is not always NULL
	lpEnvironment = source.lpEnvironment;
	dwCreationFlags = source.dwCreationFlags;
	size_t str_size = stringlen(source.lpCommandLine) + 1;
	lpCommandLine = new nchar[str_size];
	stringcpy(lpCommandLine, str_size, source.lpCommandLine);
	//initialize lpApplicationName
	str_size = stringlen(source.lpApplicationName) + 1;
	//create a temporary pointer to modifyable data to initialize unmodifiable data
	nchar* temp = new nchar[str_size];
	stringcpy(temp, str_size, source.lpApplicationName);
	lpApplicationName = temp;
	//initialize lpCurrentDirectory
	str_size = stringlen(source.lpCurrentDirectory) + 1;
	temp = new nchar[str_size];
	stringcpy(temp, str_size, source.lpCurrentDirectory);
	lpCurrentDirectory = temp;
	return *this;
}

PELaunch& PELaunch::operator=(const PELaunch& source) {
	if(this == &source) {
		return *this;
	}
	this->~PELaunch();
	//may not be necessary
	open(source.lpApplicationName);
	if(!*this) {
		throw runtime_error("failed to open file again in PElaunch::operator=(const PELaunch& source)");
	}
	bInheritHandles = source.bInheritHandles;
	//may need fix when lpEnvironment is not always NULL
	lpEnvironment = source.lpEnvironment;
	dwCreationFlags = source.dwCreationFlags;
	size_t str_size = stringlen(source.lpCommandLine) + 1;
	lpCommandLine = new nchar[str_size];
	stringcpy(lpCommandLine, str_size, source.lpCommandLine);
	//initialize lpApplicationName
	str_size = stringlen(source.lpApplicationName) + 1;
	//create a temporary pointer to modifyable data to initialize unmodifiable data
	nchar* temp = new nchar[str_size];
	stringcpy(temp, str_size, source.lpApplicationName);
	lpApplicationName = temp;
	//initialize lpCurrentDirectory
	str_size = stringlen(source.lpCurrentDirectory) + 1;
	temp = new nchar[str_size];
	stringcpy(temp, str_size, source.lpCurrentDirectory);
	lpCurrentDirectory = temp;
	return *this;
}

//////////////////////////////////////////////////////////////////
//Windows API for launching exectutables
void PELaunch::Launch() const {
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	// Start the child process.
	if(!CreateProcess(
			lpApplicationName,
			lpCommandLine,
			nullptr,
			nullptr,
			bInheritHandles,
			dwCreationFlags,
			lpEnvironment,
			lpCurrentDirectory,
			&si,
			&pi)
	) {
		printf("CreateProcess failed (%d).\n", GetLastError());
		return;
	}
	if(!non_console(lpApplicationName)) {
		// Wait until child process exits.
		WaitForSingleObject(pi.hProcess, INFINITE);
		// Close process and thread handles.
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}
}

int PELaunch::non_console(const fs::path& p) {
	ifstream ifs;
	ifs.open(p, ios::binary | ios::in);
	if (ifs) {
		ifs.seekg(0x3C);
		WORD pos1;
		fileman::BinRead(&pos1, 1, ifs);
		//			cout << pos1 << endl;
		//following should be "PE\0\0"...etc
		//see docs on https://msdn.microsoft.com/en-us/magazine/ms809762.aspx
		ifs.seekg(pos1 + 0x5c);
		WORD subsystem;
		fileman::BinRead(&subsystem, 1, ifs);
		ifs.close();
		switch (subsystem) {
		case IMAGE_SUBSYSTEM_WINDOWS_CUI:
		case IMAGE_SUBSYSTEM_OS2_CUI:
		case IMAGE_SUBSYSTEM_POSIX_CUI:
			return 0;
		case IMAGE_SUBSYSTEM_WINDOWS_GUI:
		case IMAGE_SUBSYSTEM_WINDOWS_CE_GUI:
			return 1;
		case IMAGE_SUBSYSTEM_UNKNOWN:
		case IMAGE_SUBSYSTEM_NATIVE:
		default:
			return 2;
		}
	}
	return 3;
};

bool PELaunch::DOS_magic_mumber(const fs::path& p) {
	ifstream ifs;
	ifs.open(p);
	INT16 test;
	fileman::BinRead(&test, 1, ifs);
	ifs.close();
	const INT16 i = 0x5A4D;
	return test == i;
}
