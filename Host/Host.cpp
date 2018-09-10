#include "Host.h"

#include "WinPlatform.h"	//input conversion requires including the convert class
							//will add macros here in the future to support unix-like systems

using namespace std;

void UI::loop(int argc, char* argv[]) {
	if (argc > 1) {}
	host_beginning:
//		go_to_beginning = false;
		cout << "********************************************************************************\n";
		cout << "My mini program collection " << STRING(VERSION) << '\n';
		cout << "Which program do you wish to use?\n\n";
		cout << "Use \"-h\" or \"man\" for help or manual page.\n" << endl;
		while (cin) {
			parser par;
			prompt();
			vector<nstring> argumentlist = Get_input();
			if (argumentlist.size()!=0) {
				cin.clear();
				par.after_start_selector(argumentlist);
			}
			cout << endl;
			if (go_to_beginning) { goto host_beginning; }
		}
}

void UI::prompt() {
	experimental::filesystem::path p = experimental::filesystem::current_path();
	cout << p << ">";
}


std::vector<nstring> UI::Get_input() {
	std::vector<nstring> arguments;
	while (std::cin.peek() != '\n') {
		arguments.push_back(parse_input());
	}
	cin.get();
	return arguments;
}

////////////////////////////////
//For parsing commandline input
nstring UI::parse_input() {
std::string input;
#ifdef _WIN32
//////////////////////////////////////////
//using the Windows file system convention
	convert This;
	//	std::cin >> input;
	char c;
	c = cin.peek();
	if (c == ' ') {
		do {
			cin.get(c);
		} while (c == ' ');
		cin.putback(c);
	}
	//	if(c != '\"') {
	//		cin >> input;
	//	} else 
	while (true) {
		if (c == '\"') {
			cin.get();
			cin.get(c);
			while (c != '\"') {
				//				if (c == '\\') {
				//					char a;
				//					cin.get(a);
				//					if (a == '\'' || a == '\"') {
				//						input += a;
				//						cin.get(c);
				//						continue;
				//					}
				//					if(a=='\n') {
				//						goto stop;
				//					}
				//					cin.putback(a);
				//				}
				if (c == '\n') {
					break;
				}
				input += c;
				cin.get(c);
			}
		}
		else if (c == '\n') {
			//				cin.putback(c);
			break;
		}
		else {
			cin.get(c);
			input += c;
		}
		c = cin.peek();
		if (c == ' ') {
			do {
				cin.get(c);
			} while (c == ' ');
			cin.putback(c);
			break;
		}
	}
	//	stop:
	nstring rtn;
#ifdef _UNICODE
	rtn = This.string2wstring(input);
#endif
#ifdef _MBCS
	rtn = input;
#endif
	return rtn;
#endif
/////////////////////////////////////////////////////////////
//place for parsing paths and commands in other situations
}

bool UI::change(const char& a) {
	if (a == '\a' || a == '\b' || a == '\f' || a == '\n' || a == '\r' || a == '\t' || a == '\'' || a == '\"') {
		return true;
	}//'\\'will be dealt with else where. cannot afford to use it too much in cmd shell 
	 //numbers meaning characters are ignored
	return false;
}
