//defines the entry point for executable and the main function for use when compiled as DLL

#include "SwapEnc.h"
#include "Calculator.h"

using namespace std;

#ifndef _WINDLL

int main() {
	SwapEnc();
	system("pause");
	return 0;
}

#endif
