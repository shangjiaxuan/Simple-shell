#pragma once
#include "../Lib/Header.h"

namespace Calc {

	double get_value(const std::string& var);
	void set_value(const std::string& var, double val);
	bool is_declared(const std::string& var);
	double define_name(const std::string& var, double val);
	double assign_name(const std::string& var, double val);

	extern bool isdecl;
}
