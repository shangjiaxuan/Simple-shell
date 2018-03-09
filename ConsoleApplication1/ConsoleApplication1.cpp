// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include<Windows.h>
#include<string>
#include <iostream>
#include <filesystem>
#include <istream>

using namespace std;

int main() {
	string s;
	getline(cin, s);
	experimental::filesystem::path p2 = s;
	cout << p2 << '\n';

	wstring a = p2.wstring();

	system("pause");
	Launch(a);
	return 0;
}


//windows only (CreateProcess)
void Launch(const std::wstring& str){
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	LPWSTR cmd = (LPWSTR)str.c_str();

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	// Start the child process.
	if (!CreateProcess(NULL,   // No module name (use command line)
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
