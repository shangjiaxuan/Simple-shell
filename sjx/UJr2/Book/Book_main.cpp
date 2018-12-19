#include "BookUI.h"

namespace UJr2_funcs {
	book::UI This;
	int Book(int argc, char* argv[]) {
		try {
			This.UI_main();
		} catch(std::exception& e) {
			std::cerr << e.what() << std::endl;
			std::cin.clear();
		}
		return 0;
	}
}
