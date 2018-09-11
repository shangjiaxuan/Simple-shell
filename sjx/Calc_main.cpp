#include "Calculator.h"

using namespace std;

#ifdef _WINDLL
DLL_PORT
#endif
void calculator(istream& ist) {
	const string result{ "= " };
	cout << "Simple Calculator\n";
	cout << "Version " << STRING(CALC_VERSION) << '\n' << endl;
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
