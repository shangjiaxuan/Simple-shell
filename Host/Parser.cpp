#include "Parser.h"

#include "Filemani.h"		//required for file management functions
#include "Launch.h"			//required for launching executables

#ifdef _WIN32
#include "WinPlatform.h"	//required for calling functions from dlls
#endif

using namespace std;

namespace parser {
	unsigned cur_arg = 0;

	void manual() {
		cout << "I'm sorry but there's no manual page at the moment." << endl;
	}

	/////////////////////////////////////////////////////////////////////////////////
	//As stated in the parse_input and get_input functions' notes, this function will
	//accept other form of commandline arguments in the future
	void after_start_selector(cmdline<nchar> cmd) {
		if(!(cur_arg < cmd.argc)) {
			return;
		}
		if(stringcmp(cmd.argv[cur_arg], Exit) == 0) {
			exit(0);
		}
		if(stringcmp(cmd.argv[cur_arg], Cd) == 0) {
			try {
				cur_arg++;
				if(cmd.argc <= cur_arg) {
					cout << "\ncd: no target specified!" << endl;
					return;
				}
				fileman::Change_directory(cmd.argv[cur_arg]);
			} catch(exception& e) {
				//			Handle_Error(e);
				cout << e.what();
			}
			cur_arg++;
		} else if(stringcmp(cmd.argv[cur_arg], Calculator) == 0) {
			cur_arg++;
			cout << endl;
			cin.clear();
			call<void, istream>(sjxDLL, "calculator", &cin);
			cout << endl;
			//		go_to_beginning = true;
		} else if(stringcmp(cmd.argv[cur_arg], SwapEnc) == 0) {
			cur_arg++;
			cout << endl;
			cin.clear();
			call<void, void>(sjxDLL, "SwapEnc", nullptr);
			cout << endl;
			//		go_to_beginning = true;
		} else if(stringcmp(cmd.argv[cur_arg], Man) == 0) {
			cur_arg++;
			manual();
		}
			//see if the current argument is a file
			//typically a command that start with a path should mean
			//an executable to launch with commandline or a file to open (with command?)
			//this should mean the arguments are not going to be parsed within the host
			//program anymore, thus, cur_arg generally does not need to increment
		else if(fs::exists(cmd.argv[cur_arg])) {
			cout << endl;
			cin.clear();
			//launching executables may include lnks in the future
			nstring file_path = cmd.argv[cur_arg];
			if(fileman::isexecutable(file_path)) {
				cmdline<nchar> output_cmd;
				output_cmd.argc = cmd.argc - cur_arg;
				output_cmd.argv = new nchar*[output_cmd.argc];
				for(size_t i = 0; i < output_cmd.argc; i++) {
					const size_t arg_size = stringlen(cmd.argv[i]);
					output_cmd.argv[i] = new nchar[arg_size + 1];
					stringcpy(output_cmd.argv[i], cmd.argv[i]);
				}
				const PELaunch info = PELaunch(output_cmd);
				info.Launch();
			}
#ifdef _WIN32
			else if(fileman::isshelllink(file_path)) {
				Lnk_Info info = get_LnkInfo(file_path);
				//currently lnks are only supported in this way
				//may add a link command line parser in the future
				//When adding, note that the whole line after the link file should also
				//be treated as part of the new command along with the info in the link
				if(info != Lnk_Info()) {
					const cmdline<nchar> temp = nchar2ncmdline(info.target_path.nstring().c_str());
					after_start_selector(temp);
				} else {
					cout << "Shell Link failed to open!" << endl;
				}
			}
#endif
			else {
				cout << "I'm sorry, but we currently do not support opening files with default programs yet." << endl;
				cur_arg++;
			}
		} else {
			//my extension, tries to call appropriate function in dll;
			//the function must not take arguments and must not return a value for now
			//may support argc, argv as arguments (use cmdline<char> here) and int return value
			//in the future (this function is not supported and is subject to change without warning)
#ifdef _WIN32
			bool not_found = true;
			if(cur_arg != (cmd.argc - 1)) {
				nstring str = cmd.argv[cur_arg];
				if(fs::exists(str + TEXT(".dll"))) {
					not_found = false;
					try {
						cur_arg++;
#ifdef _UNICODE
						call<void, void>(str.c_str(), convert::UTF16_2mbcs(cmd.argv[cur_arg]).c_str(), nullptr);
#endif
#ifdef _MBCS
						call<void, void>(str.c_str(), cmd.argv[cur_arg], nullptr);
#endif
						cur_arg++;
					} catch(exception& e) {
						cur_arg--;
						cout << e.what() << endl;
					}
				}
				//add other tests here(e.g. ".exe")
				if(false) {}
			}
			if(not_found) {
#ifdef _UNICODE
				cout << "Sorry, but we cannot find the specified program " << convert::UTF16_2mbcs(cmd.argv[cur_arg]) <<
				endl;
#endif
#ifdef _MBCS
				cout << "Sorry, but we cannot find the specified program " << cmd.argv[cur_arg] << endl;
#endif
				cur_arg++;
			}
#endif
			//because console apps in windows does not support unicode console I/O very well, need some work to fix this
		}
	}

	void get_formatted_char(cmdline<char>& output, std::vector<nstring> cmd, size_t start, size_t end) {
		output.argc = end - start + 1;
		output.argv = new char*[output.argc];
		for(size_t i = 0; i < output.argc; i++) {
			const size_t arg_size = cmd[i + start].size() + 1;
			output.argv[i] = new char[arg_size];
#ifdef _UNICODE
			strcpy_s(output.argv[i], arg_size, convert::wstring2string(cmd[i + start]).c_str());
#endif
#ifdef _MBCS
			strcpy_s(output.argv[i], arg_size, cmd[i + start].c_str());
#endif
		}
	}

	void get_formatted_nchar(cmdline<nchar>& output, std::vector<nstring> cmd, size_t start, size_t end) {
		output.argc = end - start + 1;
		output.argv = new nchar*[output.argc];
		for(size_t i = 0; i < output.argc; i++) {
			const size_t arg_size = cmd[i + start].size() + 1;
			output.argv[i] = new nchar[arg_size];
			stringcpy(output.argv[i], cmd[i + start].c_str());
		}
	}


	size_t ncmdline2nchar(const cmdline<nchar>& input, nchar* output, size_t max_output_size) {
		size_t rtn = 0;
		if(!output || !max_output_size) {
			size_t i = 0;
			while(true) {
				rtn += stringlen(input.argv[i]);
				i++;
				rtn++;
				if(i >= input.argc) {
					break;
				}
			}
			return rtn + 2; //2 chars for parenthesis around argv[0]
		}
		output[0] = TEXT('\"');
		nchar* nc_ptr = output + 1;
		size_t arg_size = stringlen(input.argv[0]);
		size_t copied_size = arg_size;
		if(copied_size > max_output_size) {
			return 2; //chop off in middle of copy
		}
		stringcpy(nc_ptr, input.argv[0]);
		nc_ptr += arg_size;
		copied_size += 2;
		if(copied_size > max_output_size) {
			return 1; //chop off at space
		}
		*nc_ptr = TEXT('\"'); //parenthesis
		if(input.argc == 1) {
			nc_ptr++;
			*nc_ptr = NULL;
			return rtn;
		}
		nc_ptr++;
		*nc_ptr = TEXT(' ');
		nc_ptr++;
		size_t i = 1;
		while(true) {
			arg_size = stringlen(input.argv[i]);
			copied_size += arg_size;
			if(copied_size > max_output_size) {
				return 2; //chop off in middle of copy
			}
			stringcpy(nc_ptr, input.argv[i]);
			nc_ptr += arg_size;
			i++;
			if(i >= input.argc) {
				break;
			}
			copied_size++;
			if(copied_size > max_output_size) {
				return 1; //chop off at space
			}
			*nc_ptr = 0x20; //space
			nc_ptr++;
		}
		return rtn;
	}

	//currently is simply for "A working function"
	cmdline<nchar> nchar2ncmdline(const nchar* input) {
#ifdef _UNICODE
		const string str = convert::UTF16_2mbcs(input);
#endif
#ifdef _MBCS
		const string str = input;
#endif
		istringstream iss{str};
		return UI::Get_input(iss);
	}
}
