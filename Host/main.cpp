#include "Header.h"

#include "Host.h"

int main(int argc, char* argv[]) {
	std::cout << argc;
	UI This;
	This.loop(argc, argv);
	return 0;
}
