#include "Header.h"

#include "Parser.h"

#include "Launch.h"
#ifdef _WIN32
#include "WinPlatform.h"	//required for calling functions from dlls
#endif

using namespace std;

void parser::manual() {
	cout << "I'm sorry but there's no manual page at the moment." << endl;
}

//It seems that vectors are not good containers in this case and it seems that it needs a
//structure that incorporate strings and pointers to next wstring2string (only one direction needed)
void parser::after_start_selector(std::vector<nstring> arg) {
	if(arg[cur_arg] == Exit) {
		exit(0);
	}
	if(arg[cur_arg] == Cd) {
		try {
			cur_arg++;
			if(arg.size() <= cur_arg) {
				cout << "\ncd: no target specified!" << endl;
				return;
			}
			fileman::Change_directory(arg[cur_arg]);
		} catch(exception& e) {
			//			Handle_Error(e);
			cout << e.what();
		}
		cur_arg++;
	} else if(arg[cur_arg] == Calculator) {
		cur_arg++;
		cout << endl;
		cin.clear();
		call<void, istream>(sjxDLL, "calculator", &cin);
		cout << endl;
		//		go_to_beginning = true;
	} else if(arg[cur_arg] == SwapEnc) {
		cur_arg++;
		call<void, void>(sjxDLL, "SwapEnc", nullptr);
		//		go_to_beginning = true;
	} else if(arg[cur_arg] == Man) {
		cur_arg++;
		manual();
	} else if(fs::exists(arg[cur_arg])) {
		//launching executables may include lnks in the future
		if(fileman::isexecutable(arg[cur_arg])) {
			nstring cmd;
			const size_t size = arg.size();
			while(true) {
				cmd += arg[cur_arg];
				cur_arg++;
				if(cur_arg >= size - 1) {
					break;
				}
				cmd += _T(" ");
			}
			fs::path p = cmd;
			const Launch_Info info = Launch_Info(p);
			Launch(info);
		} else {
			cout << "I'm sorry, but we currently do not support opening files with default programs yet." << endl;
			cur_arg++;
		}
	} else {
		cout << "Sorry, but we cannot find the specified program " << convert::wstring2string(arg[cur_arg]) << endl;
		cur_arg++;
		//because console apps in windows does not support unicode console I/O very well, need some work to fix this
	}
}


