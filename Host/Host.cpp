#include "Header.h"
#include "Host.h"

//#include "testing.h"
#include "Platform.h"

using namespace std;

void prompt() {
	experimental::filesystem::path p = experimental::filesystem::current_path();
	cout << p << ">";
}

int main(int argc, char* argv[]) {
	if (argc > 1) {}
	host_beginning:
		go_to_beginning = false;
		cout << "********************************************************************************\n";
		cout << "My mini program collection v0.0.0.0\n";
		cout << "Which program do you wish to use?\n\n";
		cout << "Use \"-h\" or \"man\" for help or manual page.\n" << endl;
		while (cin) {
			prompt();
			arg_number = 0;
			vector<wstring> argumentlist = Get_input();
			if (argumentlist.size()!=0) {
				after_start_selector(argumentlist);
			}
			cout << endl;
			getchar();
			if (go_to_beginning) { goto host_beginning; }
		}
	}

void after_start_selector(vector<wstring> arg) {
	if(arg[0].compare(Exit)==0) {
		exit(0);
	}
	else if(arg[0].compare(Cd) == 0) {
		try {
			Change_directory(arg[1]);
		}
		catch(exception& e) {
			cout << e.what();
		}
	}
	else if (arg[0].compare(Calculator)==0) {
		Calledcalculator(cin);
		cout << endl;
		go_to_beginning = true;
	}
	else if (arg[0].compare(Man)==0) {
		manual();
	}//launching executables may include lnks in the future
	else if (arg[0].length() > 4 && arg[0].substr(arg[0].length()-4,4).compare(Exec)==0) {
		Launch(arg[0]);
	}else{
		cout << "Sorry, but we cannot find the specified program " << endl;
		wcout << arg[0] << endl;
		//because console apps in windows does not support unicode console I/O very well, need some work to fix this
	}
}


void manual() {
	cout << "I'm sorry but there's no manual page at the moment." << endl;
}


void Change_directory(std::wstring dir) {
	try {
		experimental::filesystem::current_path(dir);
	}catch(experimental::filesystem::filesystem_error) {
		throw runtime_error("Error: Cannot chage into that working directory!\n");
	}

}
