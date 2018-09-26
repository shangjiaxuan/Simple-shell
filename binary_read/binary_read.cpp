#include "Header.h"


using namespace std;

bool non_console(const fs::path& p) {
	BinaryFileReader reader;
	reader.open(p, ios::binary | ios::in);
	if (reader) {
		reader.seekg(0x3C);
		WORD pos1;
		reader.read_WORD(&pos1, 1);
		//			cout << pos1 << endl;
		//following should be "PE\0\0"...etc
		//see docs on https://msdn.microsoft.com/en-us/magazine/ms809762.aspx
		reader.seekg(pos1 + 0x5c);
		WORD subsystem;
		reader.read_WORD(&subsystem, 1);
		reader.clear();
		switch (subsystem) {
		case IMAGE_SUBSYSTEM_UNKNOWN:
		case IMAGE_SUBSYSTEM_NATIVE:
		case IMAGE_SUBSYSTEM_WINDOWS_GUI:
		case IMAGE_SUBSYSTEM_WINDOWS_CE_GUI:
			cout << "GUI!" << endl;
			return 1;
			break;
		case IMAGE_SUBSYSTEM_WINDOWS_CUI:
		case IMAGE_SUBSYSTEM_OS2_CUI:
		case IMAGE_SUBSYSTEM_POSIX_CUI:
			cout << "CUI!" << endl;
			return 0;
			break;
		default:
			cout << "Unknown!" << endl;
			return 2;
		}
	}
	return true;
};




int main() {
	string p;
	getline(cin, p);
	return non_console(p);
}
