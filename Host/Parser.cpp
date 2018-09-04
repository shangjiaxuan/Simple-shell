#include "Parser.h"

using namespace std;

std::vector<nstring> Get_input() {
	std::vector<nstring> arguments;
	while (std::cin.peek() != '\n') {
		arguments.push_back(parse_input());
	}
	cin.get();
	return arguments;
}


//It seems that vectors are not good containers in this case and it seems that it needs a 
//structure that incorporate strings and pointers to next wstring2string (only one direction needed)

bool change(const char& a) {
	if (a == '\a' || a == '\b' || a == '\f' || a == '\n' || a == '\r' || a == '\t' || a == '\'' || a == '\"') {
		return true;
	}//'\\'will be dealt with else where. cannot afford to use it too much in cmd shell 
	 //numbers meaning characters are ignored
	return false;
}

void after_start_selector(std::vector<nstring> arg) {
	convert This;
	if (arg[arg_number] == Exit) {
		exit(0);
	}
	else if (arg[arg_number] == Cd) {
		try {
			arg_number++;
			if (arg.size() <= arg_number) {
				cout << "\ncd: no target specified!" << endl;
				return;
			}
			Change_directory(arg[arg_number]);
		}
		catch (exception& e) {
			//			Handle_Error(e);
			cout << e.what();
		}
		arg_number++;
	}
	else if (arg[arg_number] == Calculator) {
		arg_number++;
		cout << endl;
		cin.clear();
		call<istream>(calcDLL, "calculator", cin);
		cout << endl;
//		go_to_beginning = true;
	}
	else if (arg[arg_number] == SwapEnc) {
		arg_number++;
		call(encDLL, "SwapEnc");
//		go_to_beginning = true;
	}
	else if (arg[arg_number] == Man) {
		arg_number++;
		manual();
	}//launching executables may include lnks in the future
	else if (isexecutable(arg[arg_number])) {
		Launch(arg[arg_number]);
		arg_number++;
	}
	else {
		cout << "Sorry, but we cannot find the specified program " << This.wstring2string(arg[arg_number]) << endl;
		arg_number++;
		//because console apps in windows does not support unicode console I/O very well, need some work to fix this
	}
}




