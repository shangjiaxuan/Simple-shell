/*

Simple calculator

The grammar for input is:

        Calculation:
          Statement
          Print
          Quit
          Calculation Statement

        Statement:
          Declaration
          Expression

        Print:
          ;

        Quit:
          q

        Expression:
          Term
          Expression + Term
          Expression - Term

        Term:
          Primary
          Term * Primary
          Term / Primary
          Term % Primary

        Primary:
          Number
          ( Expression )
          - Primary
          + Primary

        Number:
          floating-point-literal

*/

#include <iostream>

using namespace std;

#include "Token_stream.h"
#include "Calculator.h"

int main() {
	Calc::Token_stream ts;

	try {
		Calc::calculate(ts);
		return 0;
	} catch(...) {
		cerr << "unknown error\n";
		return 2;
	}
}


