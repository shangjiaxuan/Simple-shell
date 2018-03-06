#pragma once

#include "Platform.h"

DLL void calculate(Calc::Token_stream&);

namespace Calc {
	double statement(Token_stream&);
	double declaration(Token_stream&);
	double assignment(Token_stream&);
	double expression(Token_stream&);
	double term(Token_stream&);
	double primary(Token_stream&);

	void clean_up_mess(Token_stream&);
}
