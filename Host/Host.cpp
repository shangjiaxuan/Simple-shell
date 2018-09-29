#include "Header.h"

#include "Host.h"
#include "Parser.h"
#include <queue>

#ifdef _WIN32
#include "WinPlatform.h"	//input conversion requires including the convert class
#endif

using namespace std;

#ifdef _UNICODE
template struct cmdline<nchar>;
#endif


void UI::loop(int argc, char* argv[]) const {
	//execute this if called with command
	if(argc > 1) {
		//this variable must be moved to global scope to really work.
		//this should be considered a hit to future improvements
		bool quit_on_finish = true;
		cmdline<nchar> cmd;
		//the command passed to parser should not include the appname itself
		cmd.argc = argc - 1;
		cmd.argv = new nchar*[argc - 1];
		for(int i = 1; i < argc; i++) {
#ifdef _UNICODE
			const size_t arg_size = convert::UNC_size(argv[i]);
			cmd.argv[i - 1] = new nchar[arg_size];
			stringcpy(cmd.argv[i - 1], arg_size, convert::MBC2utf16(argv[i]).c_str());
#endif
#ifdef _MBCS
			const size_t arg_size = strlen(argv[i]);
			cmd.argv[i - 1] = new nchar[arg_size];
			stringcpy(cmd.argv[i - 1], arg_size, argv[i]);
#endif
		}
		cin.clear();
		parser::cur_arg = 0;
		parser::after_start_selector(cmd);
		cout << endl;
		if(quit_on_finish)exit(0);
	}
host_beginning:
	//go_to_beginning = false;
	cout << "********************************************************************************\n";
	cout << "My mini program collection " << STRING(VERSION) << '\n';
	cout << "Which program do you wish to use?\n\n";
	cout << "Use \"-h\" or \"man\" for help or manual page.\n" << endl;
	while(cin) {
		prompt();
		const cmdline<nchar> cmd = Get_input(cin);
		if(cmd.argc) {
			cin.clear();
			parser::cur_arg = 0;
			parser::after_start_selector(cmd);
		}
		cout << endl;
		if(go_to_beginning) { goto host_beginning; }
	}
}

void UI::prompt() {
	cout << fs::current_path() << ">";
}

//////////////////////////////////////////////////////////////////////
//vector may not be a good enough container for a long line of command
//with many arguments (argv[i] may use significant amount of resource)
//may change the function in the future to store items in a queue and then
//create a char** (or cmdline struct) in the future
cmdline<nchar> UI::Get_input(istream& input_stream) {
	queue<nchar*> arguments;
	while(input_stream.peek() != '\n' && !input_stream.eof()) {
		arguments.push(parse_input(input_stream));
	}
	cmdline<nchar> rtn;
	rtn.argc = arguments.size();
	rtn.argv = new nchar*[rtn.argc];
	for(size_t i = 0; i < rtn.argc; i++) {
		rtn.argv[i] = arguments.front();
		arguments.pop();
	}
	input_stream.get();
	return rtn;
}

///////////////////////////////////////////////////////////////////////////////////
//For parsing commandline input
//may be changed to return a nchar*(NULL terminated) instead in the future
//this function can only read from char streams and wide chars should be
//treated seperately with care
//This function only works with streams with current input console codepage for now
nchar* UI::parse_input(istream& input_stream) {
	std::string input;
#ifdef _WIN32
	//////////////////////////////////////////
	//using the Windows file system convention
	char c = input_stream.peek();
	if(c == ' ') {
		do {
			input_stream.get(c);
		}
		while(c == ' ');
		input_stream.putback(c);
	}
	while(true) {
		if(c == '\"') {
			input_stream.get();
			input_stream.get(c);
			while(c != '\"') {
				if(c == '\n' || c == EOF) {
					break;
				}
				input += c;
				input_stream.get(c);
			}
		} else if(c == '\n' || c == EOF) {
			break;
		} else {
			input_stream.get(c);
			input += c;
		}
		c = input_stream.peek();
		if(c == ' ') {
			do {
				input_stream.get(c);
			}
			while(c == ' ');
			input_stream.putback(c);
			break;
		}
	}
	nchar* rtn;
#ifdef _UNICODE
	const size_t unc_size = convert::UNC_size(input.c_str());
	rtn = new nchar[unc_size + 1];
	stringcpy(rtn, unc_size + 1, convert::string2wstring(input).c_str());
#endif
#ifdef _MBCS
	const size_t mbcs_size = strlen(input.c_str());
	rtn = new nchar[mbcs_size + 1];
	stringcpy(rtn, mbcs_size + 1, input.c_str());
#endif
	return rtn;
#endif
	/////////////////////////////////////////////////////////////
	//place for parsing paths and commands in other situations
}

bool UI::change(const char& a) {
	return (a == '\a' ||
			a == '\b' ||
			a == '\f' ||
			a == '\n' ||
			a == '\r' ||
			a == '\t' ||
			a == '\'' ||
			a == '\"');
	//'\\'will be dealt with else where. cannot afford to use it too much in cmd shell
	//numbers meaning characters are ignored
}
