#include "VCtrl_controls.h"

using namespace std;

namespace UJr2_funcs {
	vehicle_control::controls This;

	int Vehicle_control(int argc, char* argv[]) {
		try {
			This.load();
		} catch(...) {}
		std::cout << "Simple Vehicle control program\n";
		std::cout << "Version 0.0.0.0\n" << std::endl;
		while(!This.exit) {
			try {
				This.interaction();
			} catch(std::exception& e) {
				std::cerr << '\n' << e.what() << '\n' << std::endl;
				std::cin.clear();
				std::cin.get();
			}
		}
		return 0;
	}
}
