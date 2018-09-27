#include "Header.h"

#include "Parser.h"

#include "Names.h"			//required for the command names
#include "Filemani.h"		//required for file management functions
#include "Launch.h"


#ifdef _WIN32
#include "WinPlatform.h"	//required for calling functions from dlls
#endif

using namespace std;

namespace parser {
	unsigned cur_arg = 0;

	void manual() {
		cout << "I'm sorry but there's no manual page at the moment." << endl;
	}

	//It seems that vectors are not good containers in this case and it seems that it needs a
	//structure that incorporate strings and pointers to next wstring2string (only one direction needed)
	void after_start_selector(std::vector<nstring> arg) {
		if (arg[cur_arg] == Exit) {
			exit(0);
		}
		if (arg[cur_arg] == Cd) {
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
			call<void, istream>(sjxDLL, "calculator", &cin);
			cout << endl;
			//		go_to_beginning = true;
		}
		else if (arg[cur_arg] == SwapEnc) {
			cur_arg++;
			call<void, void>(sjxDLL, "SwapEnc", nullptr);
			//		go_to_beginning = true;
		}
		else if (arg[cur_arg] == Man) {
			cur_arg++;
			manual();
		}
		else if (fs::exists(arg[cur_arg])) {
			//launching executables may include lnks in the future
			if (fileman::isexecutable(arg[cur_arg])) {
				ncmdline cmd;
				get_formatted_nchar(cmd, arg, cur_arg, arg.size() - 1);
				const PELaunch info = PELaunch(cmd);
				info.Launch();
			}
#ifdef _WIN32
			else if (fileman::isshelllink(arg[cur_arg])) {
				Lnk_Info info = get_LnkInfo(arg[cur_arg]);
				//currently lnks are only supported in this way
				//may add a link parser in the future
				if (info != Lnk_Info())
					after_start_selector(vector<nstring>{info.target_path});
				fs::directory_iterator a;
			}
#endif
			else {
				cout << "I'm sorry, but we currently do not support opening files with default programs yet." << endl;
				cur_arg++;
			}
		}
		else {
			cout << "Sorry, but we cannot find the specified program " << convert::wstring2string(arg[cur_arg]) << endl;
			cur_arg++;
			//because console apps in windows does not support unicode console I/O very well, need some work to fix this
		}
	}

	void get_formatted_char(cmdline& output, std::vector<nstring> cmd, size_t start, size_t end) {
		output.argc = end - start + 1;
		output.argv = new char*[output.argc];
		size_t arg_size;
		for (size_t i = 0; i < output.argc; i++) {
			arg_size = cmd[i + start].size() + 1;
			output.argv[i] = new char[arg_size];
			strcpy_s(output.argv[i], arg_size, convert::wstring2string(cmd[i + start]).c_str());
		}
	}

	void get_formatted_nchar(ncmdline& output, std::vector<nstring> cmd, size_t start, size_t end) {
		output.argc = end - start + 1;
		output.argv = new nchar*[output.argc];
		size_t arg_size;
		for (size_t i = 0; i < output.argc; i++) {
			arg_size = cmd[i + start].size() + 1;
			output.argv[i] = new nchar[arg_size];
			stringcpy(output.argv[i], arg_size, cmd[i + start].c_str());
		}
	}

}
