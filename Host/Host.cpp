#include "Header.h"
#include "Host.h"

//#include "testing.h"
#include "Platform.h"
#include "testing.h"
#include "../Calculator/Calculator.h"

using namespace std;

void prompt() {
	experimental::filesystem::path p = experimental::filesystem::current_path();
	cout << p << ">";
}

vector<wstring> Get_input() {
#ifdef WIN32
	return WIN::Get_input();
#endif
}

void Change_directory(wstring dir) {
#ifdef WIN32
	WIN::Change_directory(dir);
#endif
}

void call(const LPCWSTR library, const LPCSTR function) {
#ifdef WIN32
	WIN::call(library, function);
#endif
}
template <class  type>
void call(const LPCWSTR library, const LPCSTR function, type& pass) {
#ifdef WIN32
	WIN::call(library, function, pass);
#endif
};

template void call<std::istream>(const LPCWSTR, const LPCSTR, std::istream& pass);

void after_start_selector(vector<wstring> arg) {
#ifdef WIN32
	WIN::after_start_selector(arg);
#endif
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
				cin.clear();
				after_start_selector(argumentlist);
			}
			cout << endl;
			getchar();
			if (go_to_beginning) { goto host_beginning; }
		}
	}



void manual() {
	cout << "I'm sorry but there's no manual page at the moment." << endl;
}


#ifdef WIN32
  namespace WIN {
	void after_start_selector(vector<wstring> arg) {
		if (arg[arg_number] == Exit) {
			exit(0);
		}
		else if (arg[arg_number] == Cd) {
			try {
				arg_number++;
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
			call<istream>(L"Calculator.dll", "calculator", cin);
			cout << endl;
			go_to_beginning = true;
		}
		else if (arg[arg_number] == SwapEnc) {
			arg_number++;
			call(L"SwapEnc.dll", "SwapEnc");
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
			cout << "Sorry, but we cannot find the specified program " << endl;
			wcout << arg[arg_number] << endl;
			arg_number++;
			//because console apps in windows does not support unicode console I/O very well, need some work to fix this
		}
	}

}
#endif

