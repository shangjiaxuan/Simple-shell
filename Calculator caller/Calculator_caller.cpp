// Calculator caller.cpp : Defines the entry point for the console application.
//

#include "Header.h"

#include "../Calculator/Calculator.h"

using namespace std;

typedef void(*void_ts_ptr)(Calc::Token_stream&);

int main() {
	HINSTANCE hInst = LoadLibrary("Calculator.dll");
	if (!hInst){
		cout << "Error!" << endl;
	}
	const void_ts_ptr calculator = void_ts_ptr(GetProcAddress(hInst, "calculator"));

	Calc::Token_stream ts;
	try {
		calculator(ts);
		FreeLibrary(hInst);
		return 0;
	} catch(...) {
		cerr << "unknown error\n";
		FreeLibrary(hInst);
		return 2;
	}
}
