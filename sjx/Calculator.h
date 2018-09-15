#pragma once

#include "Header.h"

#include "CalcToken.h"

namespace Calc {

	void init(Token_stream&);			//initializes for each round of input

	double statement(Token_stream&);
	double declaration(Token_stream&);
	double assignment(Token_stream&);
	double expression(Token_stream&);
	double term(Token_stream&);
	double primary(Token_stream&);

	void clean_up_mess(Token_stream&);
}

#ifndef _WINDLL
void calculator(std::istream&);
#endif

//DLL_PORT Calc::message calculate(Calc::Token_stream&);
//
//constexpr char quit{ 'q' };
//constexpr char value{ 'v' };
//constexpr char error{ 'e' };		//Could expand this to catch all the exceptions.

extern bool on;	//quiting switch