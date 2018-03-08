#include "Header.h"

#include "../Calculator/Token.h"

#include <iostream>
#include <string>
#include <stdio.h>
#include <vector>
#include <filesystem>
#include <experimental/filesystem>


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
		cout << "*************************************************************************************************\n";
		cout << "My mini program collection v0.0.0.0\n";
		cout << "Which program do you whish to use?\n\n";
		cout << "Use \"-h\" or \"man\" for help or manual page.\n"<<endl;
		if (!start) { getchar(); }
		start = false;
		char c{0};
	host_prompt:
		prompt();
		c=cin.peek();
		if (c == '\n') {
			getchar();
			cout << endl;
			goto host_prompt;
		}
		vector<string> argumentlist;
		string s;
		char ch{0};
		while(ch!='\n') {
			cin >> s;
			argumentlist.push_back(s);
			ch=cin.peek();
		}
		after_start_selector(argumentlist);
	}
	goto host_beginning;
}

void after_start_selector(vector<string> arg) {
	if (arg[0]=="calculator") {
		Calledcalculator(cin);
		cout << endl;
	}else if (arg[0]=="man") {
		manual();
	}
	else {
		cout << "\nSorry, but the program may not exist.\n" << endl;
	}
}

void manual() {
	cout << "\nI'm sorry but there's no manual page at the moment.\n" << endl;
}

void Calledcalculator(istream& ist){
	HINSTANCE hInst = LoadLibrary("Calculator.dll");
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


/*
void test(istream& ist) {
	string s;
	ist >> s;
	cout << s << '\n';
}

int main() {
	test(cin);
	system("pause");
	return 0;
}
*/