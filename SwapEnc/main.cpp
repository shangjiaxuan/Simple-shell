//defines the entry point for executable and the main function for use when compiled as DLL

#include "SwapEnc.h"

using namespace std;

//int main() function for the program to run independently (eg. debug purpose)
#ifndef _WINDLL
int main(int argc, char* argv[]) {
	enc::SwapEnc This;
	if (argc <= 1) {
		This.name_ver();
		while (This.on) {
			try {
				This.prompt();
				This.run_time();
			}
			catch (exception& e) {
				cerr << '\n' << e.what() << '\n' << endl;
			}
		}
	}
	if (argc > 1) {
		try {
			This.command_line(argc, argv);
		}
		catch (exception& e) {
			cerr << '\n' << e.what() << '\n' << endl;
		}
	}
	//	system("pause");
	return 0;
}

#endif


//The function for export in extern "C"
#ifdef _WINDLL 
void SwapEnc() {
	enc::SwapEnc This;
	enc::SwapEnc::name_ver();
	while (true) {
		try {
			enc::SwapEnc::prompt();
			This.run_time();
			if (!This.on) {
				return;
			}
		}
		catch (exception& e) {
			cerr << '\n' << e.what() << '\n' << endl;
		}
	}
}
#endif

