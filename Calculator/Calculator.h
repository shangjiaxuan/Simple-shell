#pragma once

#include "Platform.h"

DLL void calculate(Calc::Token_stream& ts);

namespace Calc {
	void clean_up_mess(Token_stream& ts);

	double statement(Token_stream& ts);
	double declaration(Token_stream& ts);
	double expression(Token_stream& ts);
	double term(Token_stream& ts);
	double primary(Token_stream& ts);
}
