// SwapEnc.cpp : Defines the entry point for the console application.
//

#include <string>
#include <iostream>
#include <fstream>

using namespace std;

ifstream ifs;
ofstream ofs;

void name_ver() {
	cout << "Simple encryptor & decryptor\n";
	cout << "Version 0.0.0.0\n" << endl;
}

void prompt() {
	cout << "Please specify files for encryption or decryption.\n";
	cout << "You may drag and drop, and then press \"enter\" if you like.\n" << endl;
}

void enc(string a) {
	ifs.open(a, ios::binary);
	if(ifs.fail()) {
		throw runtime_error("Error: Cannot open specified file!");
	}
	string b;
	if (a.substr(a.length() - 4, 4) == ".enc") {
		b = a.substr(0, a.length() - 4);
	}else {
		b = a + ".enc";
	}
	ofs.open(b, ios::binary);
	char cur;
	ifs.get(cur);
	while(!ifs.eof()) {
		cur = (cur & 0x0f) << 4 | (cur & 0xf0) >> 4;
		ofs.put(cur);
		ifs.get(cur);
	}
	ifs.close();
	ofs.close();
}

void run_time(){
	string a;
	while(true) {
		char c = cin.peek();
		if(c=='\n') {
			break;
		}
		if(c==' ') {
			do {
			cin.get(c);
			} while (c == ' ');
			cin.putback(c);
		}
		else if (c != '\"') {
			cin >> a;
			enc(a);
		}else {
			a = "";
			cin.get();
			while(true) {
				cin.get(c);
				if (c != '\"'&&c != '\\') {
					a += c;
				}
				else if(c=='\"'){
					break;
				}
				else if(c=='\\') {
					char t = cin.get();
					if (t== '\\') {
						cin.get();
					}
					else {
						cin.putback(t);
						a += c;
					}
				}
			}
//			cout << a << endl;
			enc(a);
		}
	}
}

void command_line(int argc, char** argv) {
	int i = 1;
	string a;
	while (i < argc) {
		a = argv[i];
//		int pos{ 0 };
//		while(pos<a.length()){
//			pos=a.find("\\\\", pos);
//			if (pos != string::npos) {
//				a.erase(pos, 2);
//				pos++;
//			}else{break;}
//		}
		enc(a);
		i++;
	}
}

void Exit() {
	char c;
	cout << "Continue?[Y/n]\t";
	cin.clear();
	cin.get();
	cin.get(c);
	switch (c) {
	case 'Y': case 'y': case '\n':
		cout << endl;
		name_ver();
		break;
	case 'N': case 'n':
		exit(0);
	default:
		throw runtime_error("Unkown specifier!");
	}
}

int main(int argc, char* argv[]) {
	if(argc<=1) {
		name_ver();
		prompt();
		while(true) {
			try {
				run_time();
				Exit();
			}
			catch (exception& e) {
				cerr << e.what() << endl;
			}
		}
	}
	if(argc>1) {
		try {
			command_line(argc,argv);
		}
		catch (exception& e) {
			cerr << e.what() << endl;
		}
	}
//	system("pause");
	return 0;
}
