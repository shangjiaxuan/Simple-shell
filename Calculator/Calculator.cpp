#include <iostream>
#include <stdexcept>
#include <cmath>

#include "Token.h"
#include "Calculator.h"
#include "Variable.h"

using namespace std;

void calculator(Calc::Token_stream& ts) {
	const string prompt{ "> " };
	const string result{ "= " };
	while(cin) {
		try {
			cout << prompt;
			Calc::Token t = ts.get();
			if (t.kind == Calc::quit) {
				return;
			}
			while (t.kind == Calc::print|| t.kind == Calc::end) {
				if(t.kind== Calc::print) {
					cout << "Please inputt something\n" << prompt;
				}
				t = ts.get();
			}
			ts.putback(t);
			double ans = statement(ts);
			if (on=false){return;}
			cout << result << ans << '\n';
		}
		catch (exception& e) {
			cerr << e.what() << '\n';
			clean_up_mess(ts);
		}
	}
}

Calc::message calculate(Calc::Token_stream& ts) {
	const string prompt{ "> " };
	const string result{ "= " };
	char ch;
	while (cin||cin.get(ch),ch=='\n') {
		try {
			cout << prompt;
			Calc::Token t = ts.get();
			if (t.kind == Calc::quit) {
				return{ quit };
			}
			while (t.kind == Calc::print) {
				t = ts.get();
			}
			ts.putback(t);
			return { value ,statement(ts) };
		}catch (exception& e) {
			cerr << e.what() << '\n';
			return { error };
		}
	}
}






namespace Calc {

	bool isdecl{false};

	double statement(Token_stream& ts) {
		Token t = ts.get();
		switch (t.kind) {
		case print:
			throw runtime_error("Please input something");
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
		case number:
			ts.putback(t);
			return expression(ts);
		default:
			throw runtime_error("statement: Unknown statement.");
		}
	}

	double declaration(Token_stream& ts) {
		isdecl = false;
		Token t = ts.get();
		if(t.kind != name) {
			ts.putback(print);
			throw runtime_error("declare: name expected in declaration.");
		}
		string var_name = t.name;

		Token t2 = ts.get();
		if(t2.kind != '=') {
			throw runtime_error("declare: '=' missing in declaration of " + var_name);
		}
		double d = expression(ts);
		define_name(var_name, d);
		return d;
	}

	double assignment(Token_stream& ts) {
		Token t = ts.get();
		string var_name = t.name;
		double value = expression(ts);
		assign_name(var_name, value);
		return value;
	}

	double expression(Token_stream& ts) {
		double left = term(ts);
		while(true) {
			Token t = ts.get();
			if (t.kind == print) { goto Label; }
			switch(t.kind) {
				case '+':
					left += term(ts);
					break;
				case '-':
					left -= term(ts);
					break;
				default:
					ts.putback(t);
					Label: return left;
			}
		}
	}

	double term(Token_stream& ts) {
		double left = primary(ts);
		Token t=ts.get();
		if (t.kind == end) { return left; }
		while (cin) {
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
			default:
				ts.putback(t);
				return left;
			}
			t = ts.get();
		};
	}


	double primary(Token_stream& ts) {
		Token t = ts.get();
		switch(t.kind) {
			case print: throw runtime_error("???");
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
			case name:
				return get_value(t.name);
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
