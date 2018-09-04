#include "Header.h"
#include "Host.h"

//#include "testing.h"
#include "Platform.h"
//#include "testing.h"
#include "version.h"
#include "Parser.h"

using namespace std;

void prompt() {
	experimental::filesystem::path p = experimental::filesystem::current_path();
	cout << p << ">";
}


unsigned int arg_number;
bool go_to_beginning;

int main(int argc, char* argv[]) {
	if (argc > 1) {}
	host_beginning:
		go_to_beginning = false;
		cout << "********************************************************************************\n";
		cout << "My mini program collection " << STRING(VERSION) << '\n';
		cout << "Which program do you wish to use?\n\n";
		cout << "Use \"-h\" or \"man\" for help or manual page.\n" << endl;
		while (cin) {
			prompt();
			arg_number = 0;
			vector<nstring> argumentlist = Get_input();
			if (argumentlist.size()!=0) {
				cin.clear();
				after_start_selector(argumentlist);
			}
			cout << endl;
//			getchar();
			if (go_to_beginning) { goto host_beginning; }
		}
	}



void manual() {
	cout << "I'm sorry but there's no manual page at the moment." << endl;
}


