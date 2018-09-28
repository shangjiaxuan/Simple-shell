#include "Calculator.h"

using namespace std;

#ifdef _WINDLL
DLL_PORT
#endif
void calculator(std::istream& ist) {
	const string result{"= "};
	cout << "Simple Calculator\n";
	cout << "Version " << STRING(CALC_VERSION) << '\n' << endl;
	Calc::Token_stream ts;
	ts.init(ist);
	while(true) {
		try {
			init(ts);
			if(!on) {
				return;
			}
			Calc::Token temp;
			do {
				const double answer = statement(ts);
				if(!on) { return; }
				cout << result << answer << '\n';
				temp = ts.peek();
			}
			while(temp.kind == Calc::end);
		} catch(exception& e) {
			cerr << e.what() << '\n';
			clean_up_mess(ts);
		}
		ts.get();
	}
}
