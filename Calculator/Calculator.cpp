#include <iostream>
#include <stdexcept>
#include <cmath>

#include "Token.h"
#include "Token_stream.h"
#include "Calculator.h"
#include "Variable.h"
#include "Memory.h"

using namespace std;

namespace Calc {

	void clean_up_mess(Token_stream& ts)
	{
		ts.ignore(print);
	}

	bool isdecl{ 0 };
	void calculate(Token_stream& ts) {
		const string prompt{ "> " };
		const string result{ "= " };
		while (cin) {
			try {
				cout << prompt;
				Token t = ts.get();
				if (t.kind == quit) {
					return;
				}
				while (t.kind == print) {
					t = ts.get();
				}
				ts.putback(t);

				cout << result << statement(ts) << '\n';
			}
			catch (exception& e) {
				cerr << e.what() << '\n';
				clean_up_mess(ts);
			}
		}
	}

	double declaration(Token_stream& ts)
	{
		Token t = ts.get();
		if (t.kind != name) {
			throw runtime_error("declare: name expected in declaration");
		}
		string var_name = t.name;
		isdecl = false;
		Token t2 = ts.get();
		if (t2.kind != '=') {
			throw runtime_error("declare: '=' missing in declaration of " + var_name);
		}
		double d = expression(ts);
		define_name(var_name, d);
		return d;
	}

	double assignment(Token_stream &ts) {
		Token t = ts.get();
		string var_name = t.name;
		double value = expression(ts);
		assign_name(var_name, value);
		return value;
	}

	double statement(Token_stream& ts)
	{
		Token t = ts.get();
		switch (t.kind) {
		case let:
			isdecl = true;
			return declaration(ts);
		case assign: {
			ts.putback(t);
			return assignment(ts);
		}
		case access: {
			ts.putback(t);
			return expression(ts);
		}
		default:
			ts.putback(t);
			return expression(ts);
		}
	}

	double expression(Token_stream&ts)
	{
		double left = term(ts);
		while (true) {
			Token t = ts.get();
			switch (t.kind) {
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

	double term(Token_stream& ts)
	{
		double left = primary(ts);
		while (true) {
			Token t = ts.get();
			switch (t.kind) {
			case '*':
				left *= primary(ts);
				break;
			case '/':
			{
				double d = primary(ts);
				if (d == 0) {
					throw runtime_error("divide by zero");
				}
				left /= d;
				break;
			}
			case '%':
			{
				double d = primary(ts);
				if (d == 0) {
					throw runtime_error("divide by zero");
				}
				left = fmod(left, d);
				break;
			}
			default:
				ts.putback(t);
				return left;
			}
		}
	}


	double primary(Token_stream& ts)
	{
		Token t = ts.get();
		switch (t.kind) {
		case '(':
		{
			double d = expression(ts);
			t = ts.get();
			if (t.kind != ')') {
				throw runtime_error("')' expected");
			}
			return d;
		}
		//开平方
		case root2: {
			double e = primary(ts);
			if (e<0) {
				throw runtime_error("Cannot take the square root of a negative number in the real domain.");
			}
			return sqrt(e);
		}
					//开平方
		case number:
			return t.value;
		case '-':
			return -primary(ts);
		case '+':
			return primary(ts);
		case name:
			return get_value(t.name);
		case access:
			return get_value(t.name);
		default:
			throw runtime_error("primary expected");
		}
	}
}