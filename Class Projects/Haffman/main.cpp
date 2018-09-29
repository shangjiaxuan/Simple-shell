#include "Haffman.h"

using namespace std;

int main() {
	Haffman::prompt();
	do {
		try {
			Haffman This;
			while(This.conti) {
				try {
					This.UI();
				}
				catch (exception& e) {
					cerr << e.what() << '\n' << endl;
				}
			}
		}
		catch (exception& e) {
			cerr << e.what() << '\n' << endl;
		}
	} while (on);
	return 0;
}
