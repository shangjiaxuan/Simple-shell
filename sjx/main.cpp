//////////////////////////////////////////////////////////////////////
//For debugging certain functions directly by compiling into exe

#include "Header.h"
#include "SwapEnc.h"
#include "Calculator.h"
#include "UJr2.h"

using namespace std;

#ifndef _WINDLL
int main(int argc, char* argv[]) {
	UJr2(argc, argv);
	system("pause");
	return 0;
}
#endif
