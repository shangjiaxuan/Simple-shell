#include "Calculator.h"

using namespace std;

#ifdef _WINDLL
void calculator(istream& ist) {
	const string result{ "= " };
	cout << "Simple Calculator\n";
	cout << "Version " << STRING(VERSION) << '\n' << endl;
	Calc::Token_stream ts;
	ts.init(ist);
	while (true) {
		try {
			init(ts);
			if (on == false) {
				return;
			}
			Calc::Token temp;
			double answer;
			do {
				answer = statement(ts);
				if (on == false) { return; }
				cout << result << answer << '\n';
				temp = ts.peek();
			} while (temp.kind == Calc::end);
		}
		catch (exception& e) {
			cerr << e.what() << '\n';
			Calc::clean_up_mess(ts);
		}
		ts.get();
	}
}
#endif

#ifndef _WINDLL
int main() {
	const string result{ "= " };
	cout << "Simple Calculator\n";
	cout << "Version " << STRING(VERSION) << '\n' << endl;
	Calc::Token_stream ts;
	ts.init(cin);
	while (true) {
		try {
			init(ts);
			if (on == false) {
				return 0;
			}
			Calc::Token temp;
			double answer;
			do {
				answer = statement(ts);
				if (on == false) { return 0; }
				cout << result << answer << '\n';
				temp = ts.peek();
			} while (temp.kind == Calc::end);
		}
		catch (exception& e) {
			cerr << e.what() << '\n';
			Calc::clean_up_mess(ts);
		}
		ts.get();
	}
	return 0;
}
#endif