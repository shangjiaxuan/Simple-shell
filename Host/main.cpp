#include "Header.h"

#include "Host.h"

int main(int argc, char* argv[]) {
	if(argc > 1) {
		std::cout << "Command received!";
	}
	UI This;
	This.loop(argc, argv);
	return 0;
}
