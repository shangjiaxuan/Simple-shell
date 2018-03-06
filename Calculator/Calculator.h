#pragma once

#include "Platform.h"

namespace Calc {

	struct message {
		char kind;
		double value;
	};

	double statement(Token_stream&);
	double declaration(Token_stream&);
	double assignment(Token_stream&);
	double expression(Token_stream&);
	double term(Token_stream&);
	double primary(Token_stream&);

	void clean_up_mess(Token_stream&);
}

DLL void calculator(Calc::Token_stream&);

DLL Calc::message calculate(Calc::Token_stream&);

constexpr char quit{ 'q' };
constexpr char value{ 'v' };
constexpr char error{ 'e' };		//Could expand this to catch all the exceptions.

bool on{ true }; //quiting switch
