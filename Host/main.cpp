#include "Header.h"

#include "Host.h"

int main(int argc, char* argv[]) {
	for(int i = 0; i < argc; i++) {
		std::cout << argv[i] << std::endl;
	}
	UI This;
	This.loop(argc, argv);
	return 0;
}
