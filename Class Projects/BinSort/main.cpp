#include "BinSort.h"

using namespace std;

int main() {
	BinSort::name_ver();
	try {
		BinSort This;
		while (on) {
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
	return 0;
}

std::string parse_path(std::string line) {
	string path;
	istringstream iss(line);
	char current;
	iss.get(current);
	while (current == ' ') {
		iss.get(current);
	}
	iss.putback(current);
	while (iss) {
		iss.get(current);
		switch (current) {
		case '\"':
			iss.get(current);
			while (current != '\"') {
				path.push_back(current);
				iss.get(current);
			}
			break;
		default:
			path.push_back(current);
			iss.get(current);
		}
	}
	return path;
}

