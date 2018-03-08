/* 

Simple calculator

The grammar for input is:

Calculation:
Statement
Print
Quit
Calculation Statement

Statement:
Declaration						//"let a=10"
Expression

End:
';'

Print:
'\n'

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
Variable
sqrtPrimary
(Expression)
- Primary
+ Primary

Number:
floating-point-literal

*/

#include <iostream>
#include <stdexcept>
#include <cmath>

#include "Token.h"
#include "Calculator.h"
#include "Variable.h"

#include <vector>

using namespace std;

void calculator(istream& ist) {
	const string result{ "= " };
	cout << "\nSimple Calculator\n";
	cout << "Version 0.0.0.0\n" << endl;;
	Calc::Token_stream ts;
	ts.init(ist);
	while(true) {
		try {
			Calc::init(ts);
			Calc::Token temp;
			double answer;
			do {
				answer = Calc::statement(ts);
				if(on == false) { return; }
				cout << result << answer << '\n';
				temp = ts.peek();
			}while(temp.kind == Calc::end);
		}
		catch (exception& e) {
			cerr << e.what() << '\n';
			Calc::clean_up_mess(ts);
		}
	}
}

namespace Calc {
	bool isdecl{ false };
	void init(Token_stream& ts) {
		const string prompt{ "> " };
	calculator_start:
		ts.get();										//actively wait for input, solves the while(cin) problem with starting
		cout << prompt;
		Token t = ts.peek();
		switch (t.kind) {
		case quit: return;
		case print:
				cout << "Please input something!\n";
				goto calculator_start;
		case end: do {
			t = ts.get();
		} while (t.kind == end);
		ts.putback(t); break;
		default: {}
		}
	}

	double statement(Token_stream& ts) {
		const Token t = ts.get();
		switch (t.kind) {
		case print:
			throw runtime_error("Broken link");
		case quit:
			on = false;
			return 0;
		case let:
			isdecl = true;
			return declaration(ts);
		case assign:
			ts.putback(t);
			return assignment(ts);
		case access:
			ts.putback(t);
			return expression(ts);
		default:
			ts.putback(t);
			return expression(ts);
		}
	}

	double declaration(Token_stream& ts) {
		const Token t = ts.get();
		isdecl = false;
		if(t.kind != name) {
			ts.putback(print);
			throw runtime_error("declare: name expected in declaration.");
		}
		const string var_name = t.name;

		const Token t2 = ts.get();
		if(t2.kind != '=') {
			throw runtime_error("declare: '=' missing in declaration of " + var_name);
		}
		double d = expression(ts);
		define_name(var_name, d);
		return d;
	}

	double assignment(Token_stream& ts) {
		const Token t = ts.get();
		const string var_name = t.name;
		double value = expression(ts);
		assign_name(var_name, value);
		return value;
	}

	double expression(Token_stream& ts) {
		double left = term(ts);
		Token t;
		while(true) {
			t = ts.get();
			switch(t.kind) {
				case '+':
					left += term(ts);
					break;
				case '-':
					left -= term(ts);
					break;
				default:
					ts.putback(t);
					return left;
			}
		}
	}

	double term(Token_stream& ts) {
		double left = primary(ts);
		Token t = ts.get();
		if (t.kind == end) { return left; }
		while (cin) {																	//cin still used!!!
			switch (t.kind) {
			case '*':
				left *= primary(ts);
				break;
			case '/': {
				double d = primary(ts);
				if (d == 0) {
					throw runtime_error("divide by zero");
				}
				left /= d;
				break;
			}
			case '%': {
				double m = primary(ts);
				if (m == 0) {
					throw runtime_error("divide by zero");
				}
				left = fmod(left, m);
				break;
			}
			case name:{
				return left*t.value;
			}
			default:
				ts.putback(t);
				return left;
			}
			t = ts.get();
		}throw runtime_error("Term: Unknown term.");
	}


	double primary(Token_stream& ts) {
		Token t = ts.get();
		switch(t.kind) {
			case '(':
				{
					double d = expression(ts);
					t = ts.get();
					if(t.kind != ')') {
						throw runtime_error("')' expected");
					}
					return d;
				}
				//开平方
			case root2:
				{
					double e = primary(ts);
					if(e < 0) {
						throw runtime_error(
							"Cannot take the square root of a negative number in the real domain."
						);
					}
					return sqrt(e);
				}
				//开平方
			case number:
				return t.value;
			case access:
				return get_value(t.name);
			default:
				throw runtime_error("primary expected");
		}
	}

	void clean_up_mess(Token_stream& ts) {
		ts.ignore(print);
	}
}
