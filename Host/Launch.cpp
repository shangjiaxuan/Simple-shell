#include "Launch.h"

#include "Parser.h"			//for converting cmdline struct

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
	LPTSTR const temp = new nchar[required_size];
	stringcpy(temp, cmd.argv[0]);
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
	lpCommandLine = new nchar[exe_path.nstring().size() + 1];
	stringcpy(lpCommandLine, exe_path.nstring().c_str());
	//a temporary pointer to modifiable data to initialize unmodifiable data
	nchar* temp = new nchar[exe_path.nstring().size() + 1];
	stringcpy(temp, exe_path.nstring().c_str());
	lpApplicationName = temp;
	temp = new nchar[exe_path.remove_filename().nstring().size() + 1];
	stringcpy(temp, exe_path.nstring().c_str());
	lpCurrentDirectory = temp;
}


PELaunch::PELaunch(const PELaunch& source) {
	copy(*this, source);
}

PELaunch::PELaunch(PELaunch&& source) noexcept {
	move(*this, source);
}

PELaunch& PELaunch::operator=(PELaunch&& source) noexcept {
	move(*this, source);
	return *this;
}

PELaunch& PELaunch::operator=(const PELaunch& source) {
	copy(*this, source);
	return *this;
}

//////////////////////////////////////////////////////////////////
//Windows API for launching executables
//adapted from answer on
//https://stackoverflow.com/questions/42531/how-do-i-call-createprocess-in-c-to-launch-a-windows-executable
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
	ifs.open(p, binary | in);
	if(ifs) {
		ifs.seekg(0x3C);
		WORD pos1;
		fileman::BinRead(&pos1, 1, ifs);
		//cout << pos1 << endl;
		//following should be "PE\0\0"...etc
		//see docs on PE file format on
		//https://msdn.microsoft.com/en-us/magazine/ms809762.aspx
		ifs.seekg(pos1 + 0x5c);
		WORD subsystem;
		fileman::BinRead(&subsystem, 1, ifs);
		ifs.close();
		switch(subsystem) {
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

bool PELaunch::DOS_magic_number(const fs::path& p) {
	ifstream ifs;
	ifs.open(p);
	INT16 test;
	fileman::BinRead(&test, 1, ifs);
	ifs.close();
	const INT16 i = 0x5A4D;
	return test == i;
}

void PELaunch::copy(PELaunch& destination, const PELaunch& source) {
	//may not be necessary
	destination.destroy();
	destination.open(source.lpApplicationName);
	if (!destination) {
		throw runtime_error("failed to open file again in PElaunch::operator=(const PELaunch& source)");
	}
	destination.bInheritHandles = source.bInheritHandles;
	//may need fix when lpEnvironment is not always NULL
	destination.lpEnvironment = source.lpEnvironment;
	destination.dwCreationFlags = source.dwCreationFlags;
	size_t str_size = stringlen(source.lpCommandLine);
	destination.lpCommandLine = new nchar[str_size + 1];
	stringcpy(destination.lpCommandLine, source.lpCommandLine);
	//initialize lpApplicationName
	str_size = stringlen(source.lpApplicationName);
	//create a temporary pointer to modifiable data to initialize unmodifiable data
	nchar* temp = new nchar[str_size + 1];
	stringcpy(temp, source.lpApplicationName);
	destination.lpApplicationName = temp;
	//initialize lpCurrentDirectory
	str_size = stringlen(source.lpCurrentDirectory);
	temp = new nchar[str_size + 1];
	stringcpy(temp, source.lpCurrentDirectory);
	destination.lpCurrentDirectory = temp;
}

void PELaunch::move(PELaunch& destination, PELaunch& source) noexcept {
	if ((&destination) == (&source)) return;
	destination.destroy();
	destination.swap(source);
	destination.bInheritHandles = source.bInheritHandles;
	destination.dwCreationFlags = source.dwCreationFlags;
	destination.lpApplicationName = source.lpApplicationName;
	destination.lpCommandLine = source.lpCommandLine;
	destination.lpCurrentDirectory = source.lpCurrentDirectory;
	destination.lpEnvironment = source.lpEnvironment;
	source.bInheritHandles = NULL;
	source.dwCreationFlags = NULL;
	source.lpApplicationName = nullptr;
	source.lpCommandLine = nullptr;
	source.lpCurrentDirectory = nullptr;
	source.lpEnvironment = nullptr;
}

void PELaunch::destroy() {
	close();
	bInheritHandles = NULL;
	dwCreationFlags = NULL;
	delete[] lpApplicationName;
	lpApplicationName = nullptr;
	delete[] lpCommandLine;
	lpCommandLine = nullptr;
	delete[] lpCurrentDirectory;
	lpCurrentDirectory = nullptr;
	free(lpEnvironment);
	lpEnvironment = nullptr;
}

