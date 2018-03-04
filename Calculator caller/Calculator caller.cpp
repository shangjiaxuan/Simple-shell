// Calculator caller.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include "../Calculator/Token_stream.h"
#include "../Calculator/Calculator.h"

using namespace std;

int main() {
	HINSTANCE hInst = LoadLibrary(L"Calc.dll");

	Calc::Token_stream ts;
	try {
		calculate(ts);
		return 0;
	} catch(...) {
		cerr << "unknown error\n";
		return 2;
	}
}
