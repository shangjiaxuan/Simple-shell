#include "Header.h"
#include "Platform.h"

using namespace std;

void Change_directory(nstring dir) {
	try {
		experimental::filesystem::current_path(dir);
	}
	catch (experimental::filesystem::filesystem_error& e) {
		cerr << e.what() << '\n' << endl;
		throw runtime_error("Error: Cannot chage into that working directory!\n");
	}
}