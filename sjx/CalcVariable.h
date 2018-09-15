#pragma once
#include "Header.h"

namespace Calc {

	double get_value(std::string var);
	void set_value(std::string var, double val);
	bool is_declared(std::string var);
	double define_name(std::string var, double val);
	double assign_name(std::string var, double val);

	extern bool isdecl;
}
