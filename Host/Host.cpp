#include "Header.h"
#include "Host.h"

#include "../Calculator/Token.h"

#include <iostream>
#include <string>
#include <stdio.h>
#include <vector>
#include <filesystem>
#include <experimental/filesystem>

#include "testing.h"
#include "Platform.h"

using namespace std;

typedef void(*void_ist_ptr)(istream&);

void prompt() {
	experimental::filesystem::path p = experimental::filesystem::current_path();
	cout << p << ">";
}

int main(int argc, char* argv[]) {
	if (argc <= 1) {
		start = true;
	host_beginning:
		cout << "**************************************************************************************************\n";
		cout << "My mini program collection v0.0.0.0\n";
		cout << "Which program do you wish to use?\n\n";
		cout << "Use \"-h\" or \"man\" for help or manual page.\n" << endl;
		if (!start) { getchar(); }
		start = false;
		wchar_t c{ 0 };
	host_prompt:
		prompt();
		arg_number = 0;
		c = cin.peek();
		if (c == L'\n') {
			getchar();
			cout << endl;
			goto host_prompt;
		}
		vector<wstring> argumentlist=Get_input();
		after_start_selector(argumentlist);
	}
	goto host_beginning;
}

void after_start_selector(vector<wstring> arg) {
	if(arg[0].compare(Exit)==0) {
		exit(0);
	}
	else if(arg[0].compare(Cd) == 0) {
		Change_directory(arg[1]);
	}
	else if (arg[0].compare(Calculator)==0) {
		Calledcalculator(cin);
		cout << endl;
	}
	else if (arg[0].compare(Man)==0) {
		manual();
	}
	else if (arg[0].length() > 4 && arg[0].substr(arg[0].length()-4,4).compare(Exec)==0) {
		Launch(arg[0]);
	}else{
		cout << "\nSorry, but we cannot find the specified program ";
		wcout << arg[0] << endl;
	}
}


void manual() {
	cout << "\nI'm sorry but there's no manual page at the moment.\n" << endl;
}

void Calledcalculator(istream& ist) {
	HINSTANCE hInst = LoadLibrary(L"Calculator.dll");
	if (!hInst) {
		cout << "Error!" << endl;
	}
	const void_ist_ptr calculator = void_ist_ptr(GetProcAddress(hInst, "calculator"));
	Calc::Token_stream ts;
	try {
		calculator(ist);
		FreeLibrary(hInst);
	}
	catch (...) {
		cerr << "unknown error\n";
		FreeLibrary(hInst);
	}
}

