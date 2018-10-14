//////////////////////////////////////////////////////////////////////
//For debugging certain functions directly by compiling into exe

#include "UJr2/UJr2.h"
#include "SwapEnc/SwapEnc.h"
#include "calculator/Calculator.h"

using namespace std;

#ifndef _WINDLL
int main(int argc, char* argv[]) {
	UJr2(argc, argv);
	system("pause");
	return 0;
}
#endif
