#include <map>

#include "Variable.h"

using namespace std;

map<string, double> var_table;

namespace Calc {

	double get_value(string var) {
		try {
			return var_table.at(var);
		} catch(out_of_range) {
			throw runtime_error("get: undefined variable " + var);
		}
	}

	void set_value(string var, double val) {
		try {
			var_table.at(var) = val;
		} catch(out_of_range) {
			throw runtime_error("set: undefined variable " + var);
		}
	}

	bool is_declared(string var) {
		if(var_table.count(var) > 0) {
			return true;
		} else { return false; }
	}

	double define_name(string var, double val) {
		if(is_declared(var)) {
			throw runtime_error("variable " + var + " is already declared");
		}
		var_table[var] = val;
		return val;
	}

	double assign_name(string var, double val) {
		try {
			var_table.at(var) = val;
			return val;
		} catch(out_of_range) {
			throw runtime_error("assign: undefined variable");
		}
	}
}
