#include "../Library/Header.h"

#include "CalcVariable.h"

using namespace std;

namespace Calc {

	map<string, double> var_table;

	double get_value(const string& var) {
		try {
			return var_table.at(var);
		} catch(out_of_range&) {
			throw runtime_error("get: undefined variable " + var);
		}
	}

	void set_value(const string& var, const double val) {
		try {
			var_table.at(var) = val;
		} catch(out_of_range&) {
			throw runtime_error("set: undefined variable " + var);
		}
	}

	bool is_declared(const string& var) {
		return var_table.count(var);
	}

	double define_name(const string& var, const double val) {
		if(is_declared(var)) {
			throw runtime_error("variable " + var + " is already declared");
		}
		var_table[var] = val;
		return val;
	}

	double assign_name(const string& var, const double val) {
		try {
			var_table.at(var) = val;
			return val;
		} catch(out_of_range&) {
			throw runtime_error("assign: undefined variable");
		}
	}

}
