#include "stack.h"

cross_river This;

int main(int argc, const char * argv[]) {
	try {
		This.interaction();
	}catch(std::exception& e) {
		std::cerr << e.what() << '\n' << std::endl;
	}
	system("pause");
    return 0;
}
