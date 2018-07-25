#include "Header.h"
#include "SwapEnc.h"
#include "version.h"

using namespace std;

namespace enc {

	void SwapEnc::name_ver() {
		cout << "Simple encryptor & decryptor\n";
		cout << "Version " << STRING(VERSION) << '\n' << endl;
	}
	void SwapEnc::prompt() {
		cout << "Please specify files for encryption or decryption.\n";
		cout << "You may drag and drop, and then press \"enter\" if you like.\n" << endl;
	}
	void __fastcall SwapEnc::enc(string a) {
		if(a=="") {
			throw std::runtime_error("Specified file empty!");
		}
		ifs.open(a, ios::binary);
		if (ifs.fail()) {
			throw runtime_error("Error: Cannot open specified file!");
		}
		string b;
		if (a.substr(a.length() - 4, 4) == ".enc") {
			b = a.substr(0, a.length() - 4);
		}
		else {
			b = a + ".enc";
		}
		ofs.open(b, ios::binary);
		char cur;
		ifs.get(cur);
		while (!ifs.eof()) {
			cur = (cur & 0x0f) << 4 | (cur & 0xf0) >> 4;
			ofs.put(cur);
			ifs.get(cur);
		}
		ifs.close();
		ofs.close();
	}

	void __fastcall SwapEnc::run_time() {
		string a;
		if(cin.peek()=='\n') {
			cout << "Please specify at least one file!\n" << endl;
		}
		while (true) {
			char c = cin.peek();
			if (c == '\n') {
				break;
			}
			if (c == ' ') {
				do {
					cin.get(c);
				} while (c == ' ');
				cin.putback(c);
			}
			else if (c != '\"') {
				cin >> a;
				enc(a);
			}
			else {
				a = "";
				cin.get();
				while (true) {
					cin.get(c);
					if (c != '\"'&&c != '\\') {
						a += c;
					}
					else if (c == '\"') {
						break;
					}
					else if (c == '\\') {
						char t = cin.peek();
						if (t == '\\') {
							cin.get();
						}
						else {
							a += c;
						}
					}
				}
				cout << "Encrypting the file \"" << a <<"\""<< endl;
				enc(a);
			}
		}
		Exit();
	}

	void SwapEnc::command_line(int argc, char** argv) {
		int i = 1;
		string a;
		while (i < argc) {
			a = argv[i];
//			int pos{ 0 };
//			while(pos<a.length()){
//				pos=a.find("\\\\", pos);
//				if (pos != string::npos) {
//					a.erase(pos, 2);
//					pos++;
//				}else{break;}
//			}
			enc(a);
			i++;
		}
	}

	void SwapEnc::Exit() {
		char c;
		cout << "Continue?[Y/n]\t";
		cin.clear();
		cin.get();
		cin.get(c);
		switch (c) {
		case 'Y': case 'y': case '\n':
			cout << endl;
			prompt();
			break;
		case 'N': case 'n':
			on = false;
			break;
		default:
			throw runtime_error("Unkown specifier!");
		}
	}
}

#ifndef _WINDLL

int main(int argc, char* argv[]) {
	SwapEnc This;
	if(argc <= 1) {
		This.name_ver();
		This.prompt();
		while(true) {
			try {
				This.run_time();
				This.Exit();
			} catch(exception& e) {
				cerr << e.what() << endl;
			}
		}
	}
	if(argc > 1) {
		try {
			This.command_line(argc, argv);
		} catch(exception& e) {
			cerr << e.what() << endl;
		}
	}
	//	system("pause");
	return 0;
}

#endif

#ifdef _WINDLL 
void SwapEnc() {
	enc::SwapEnc This;
	enc::SwapEnc::name_ver();
	enc::SwapEnc::prompt();
	while (true) {
		try {
			This.run_time();
			if(!This.on) {
				return;
			}
		}
		catch (exception& e) {
			cerr << e.what() << endl;
		}
	}
}
#endif
