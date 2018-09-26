﻿#include <iostream>
#include <filesystem>
#include <fstream>
#include "Windows.h"
#include "winnt.h"

namespace fs = std::experimental::filesystem;

using namespace std;

int main() {
	string p;
	std::fstream is;
	getline(cin, p);
	is.clear();
	is.open(p, ios::binary | ios::in);
	if(is) {
		is.seekg(0x3C);
		char c[2];
		unsigned char ch[2];
		is.read(c, 2);
		ch[0] = c[0];
		ch[1] = c[1];
		WORD trans[2];
		trans[0] = ch[0];
		trans[1] = ch[1];
		WORD pos1 = trans[0] + (trans[1] << 8);
		//			cout << pos1 << endl;
		//following should be "PE\0\0"...etc
		//see docs on https://msdn.microsoft.com/en-us/magazine/ms809762.aspx
		is.seekg(pos1 + 0x5c);
		is.read(c, 2);
		ch[0] = c[0];
		ch[1] = c[1];
		trans[0] = ch[0];
		trans[1] = ch[1];
		WORD subsystem = trans[0] + (trans[1] << 8);
		switch(subsystem) {
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
}
