#include "SwapEnc.h"

using namespace std;

void SwapEnc() {
	enc::SwapEnc This;
	enc::SwapEnc::name_ver();
	while (true) {
		try {
			enc::SwapEnc::prompt();
			This.run_time();
			if (!This.on) {
				return;
			}
		}
		catch (exception& e) {
			cerr << '\n' << e.what() << '\n' << endl;
		}
	}
}
