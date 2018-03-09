#include "Header.h"

using namespace std;

typedef void(*void_ist_ptr)(istream&);

int main() {
	HINSTANCE hInst = LoadLibrary("Calculator.dll");
	if (!hInst){
		cout << "Error!" << endl;
	}
	const void_ist_ptr calculator = void_ist_ptr(GetProcAddress(hInst, "calculator"));
	try {
		calculator(cin);
		FreeLibrary(hInst);
		return 0;
	} catch(...) {
		cerr << "unknown error\n";
		FreeLibrary(hInst);
		return 2;
	}
}
