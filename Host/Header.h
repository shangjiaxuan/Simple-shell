#pragma once

#define EXEC

#include<stdio.h>
#include<tchar.h>
#include<Windows.h>
#include<iostream>
#include <vector>

bool start;
int arg_number;	//for incorporating multiple use arguments
void prompt();	//for working with prompts
void after_start_selector(std::vector<std::wstring>);	//select the function to call if no original argument is found

void manual();

void Calledcalculator(std::istream& ist);

#ifdef _WIN32
void Launch(const std::wstring& str) {
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

#endif

