#include "Parser.h"
#include "WinPlatform.h"	//required for calling functions from dlls
							//will add macros in the future to support unix-like systems
using namespace std;

void manual() {
	cout << "I'm sorry but there's no manual page at the moment." << endl;
}

//It seems that vectors are not good containers in this case and it seems that it needs a 
//structure that incorporate strings and pointers to next wstring2string (only one direction needed)

void parser::after_start_selector(std::vector<nstring> arg) {
	if (arg[cur_arg] == Exit) {
		exit(0);
	}
	else if (arg[cur_arg] == Cd) {
		try {
			cur_arg++;
			if (arg.size() <= cur_arg) {
				cout << "\ncd: no target specified!" << endl;
				return;
			}
			fileman::Change_directory(arg[cur_arg]);
		}
		catch (exception& e) {
			//			Handle_Error(e);
			cout << e.what();
		}
		cur_arg++;
	}
	else if (arg[cur_arg] == Calculator) {
		cur_arg++;
		cout << endl;
		cin.clear();
		call<istream>(calcDLL, "calculator", cin);
		cout << endl;
//		go_to_beginning = true;
	}
	else if (arg[cur_arg] == SwapEnc) {
		cur_arg++;
		call(encDLL, "SwapEnc");
//		go_to_beginning = true;
	}
	else if (arg[cur_arg] == Man) {
		cur_arg++;
		manual();
	}//launching executables may include lnks in the future
	else if (fileman::isexecutable(arg[cur_arg])) {
		Launch(arg[cur_arg]);
		cur_arg++;
	}
	else {
		cout << "Sorry, but we cannot find the specified program " << This.wstring2string(arg[cur_arg]) << endl;
		cur_arg++;
		//because console apps in windows does not support unicode console I/O very well, need some work to fix this
	}
}



