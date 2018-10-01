#include "UJr2.h"
#include "VCtrl_controls.h"

using namespace std;

//comply to the convention of argv[0] being the module name
int UJr2(int argc, char* argv[]) {
	//manual_page will be inserted to the commandline argument passed 
	//if the Host program requests a certain dll program's manual page
	//because in general, man and -h should be allowed to act differently
	//This argument is only used when there's no manual page separately
	//put in the Host program's default search directory (currently that doesn't exist)
	if(argc == 1 || strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "manual_page") == 0) {
		cout << "My(Shang Jiaxuan's) collection of programs from 2nd semester of University Junior year\n";
		cout << "Syntax: sjx Ujr2 \"Program name\" command line arguments (no argument may be a good place to start)\n";
		cout << "Following function names are allowed:\n";
		cout << "crossing\n";
		cout << "Vehicle_control\n";
		cout << "Book\n";
		cout << "KMP\n";
		cout << "maze\n";
		cout << "CrossRiver\n";
		cout << "Haffman\n";
		cout << "Block_search\n";
		cout << "BinSort\n" << endl;
		cout << "Please note that all commands are case sensitive here!\n" << endl;
		return 0;
	}
	argv++;
	argc--;
	try {
		if(strcmp(argv[1], "Crossing") == 0) {
			return UJr2_funcs::Crossing(argc, argv);
		}

		if(strcmp(argv[1], "Vehicle_control") == 0) {
			return UJr2_funcs::Vehicle_control(argc, argv);
		}
		if(strcmp(argv[1], "Book") == 0) {
			return UJr2_funcs::Book(argc, argv);
		}
		if(strcmp(argv[1], "KMP") == 0) {
			return UJr2_funcs::KMP(argc, argv);
		}
		if (strcmp(argv[1], "CrossRiver") == 0) {
			return UJr2_funcs::CrossRiver(argc, argv);
		}

		if (strcmp(argv[1], "maze") == 0) {
			return UJr2_funcs::maze(argc, argv);
		}
/*

				if (strcmp(argv[1], "Haffman") == 0) {
					return UJr2_funcs::Haffman(argc, argv);
				}
				if (strcmp(argv[1], "Block_search") == 0) {
					return UJr2_funcs::Block_search(argc, argv);
				}
				if (strcmp(argv[1], "BinSort") == 0) {
					return UJr2_funcs::BinSort(argc, argv);
				}
				*/
		else {
			cout << "UJr2 cannot find the specified program: " << argv[1] << "!\n" << endl;
			return 0;
		}
	} catch(exception& e) {
		cerr << e.what() << '\n' << endl;
		return -1;
	}
}
