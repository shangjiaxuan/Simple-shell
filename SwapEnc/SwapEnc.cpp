#include "Header.h"
#include "SwapEnc.h"
#include "version.h"
#include "Paths.h"

using namespace std;

namespace enc {

	void SwapEnc::name_ver() {
		cout << "Simple encryptor & decryptor\n";
		cout << "Version " << STRING(VERSION) << '\n' << endl;
	}
	void SwapEnc::prompt() {
		cout << "Please specify files for encryption or decryption.\n";
		cout << "You may drag and drop one by one, and then press \"enter\" if you like.\n" << endl;
	}

	void __fastcall SwapEnc::enc(fs::path input, fs::path output) {
		if(input=="") {
			throw std::runtime_error("Specified file empty!");
		}
		ifs.open(input, ios::binary);
		if (ifs.fail()) {
			throw runtime_error("Error: Cannot open specified file!");
		}
		// string b;
		// if (input.string().substr(input.string.length() - 4, 4) == ".enc") {
		// 	b = input.string.substr(0, input.string.length() - 4);
		// }
		// else {
		// 	b = input + ".enc";
		// }
		ofs.open(output, ios::binary);
		cout << "Processing the file \"" << input << "\"" << endl;
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

	std::string SwapEnc::parse_pathnames(std::istream& ist) {
		string a;
		char c = ist.peek();
		if (c == ' ') {
			do {
				ist.get(c);
			} while (c == ' ');
			ist.putback(c);
		}
		if (c == '\n') {
			return "";
		}
		if (c != '\"') {
			ist >> a;
			return a;
		}
		else {
			a = "";
			ist.get();
			while (true) {
				ist.get(c);
				if (c != '\"'&&c != '\\') {
					a += c;
				}
				else if (c == '\"') {
					break;
				}
				else if (c == '\\') {
					char t = ist.peek();
					if (t == '\\') {
						ist.get();
					}
					else {
						a += c;
					}
				}
			}
//			cout << "Encrypting the file \"" << a << "\"" << endl;
			return a;
		}
	}

	std::string parse_name(std::string ori) {
		string rtn;
		if (ori.substr(ori.length() - 4, 4) == ".enc") {
			rtn = ori.substr(0, ori.length() - 4);
		}
		else {
			rtn = ori + ".enc";
		}
		return rtn;
	}

	fs::path parse_output_path(fs::path input, fs::path cur_output, int depth) {
		if(depth==0) {
			return parse_name(input.string());
		}
		else {

		}
	}

	void __fastcall SwapEnc::encrypt_loop(std::istream& ist) {
		while (true) {
			string name = parse_pathnames(ist);
			if(name=="") {
				break;
			}
			fs::path input = name;
			if(fs::is_directory(input)) {
				iopath_recursive_iterator it{ input };
//				cout << "Iterator constructed!\n";
				while(it!=it.end()) {
//					cout << "Current input path: " << it.cur_ipath << '\n';
//					cout << "Current output path: " << it.cur_opath << '\n';
					if(fs::is_directory(it.cur_ipath)) {
						fs::create_directory(it.cur_opath);
					}
					else {
						enc(it.cur_ipath, it.cur_opath);
					}
					++it;
				}
			}
//			if (fs::is_regular_file(input)) {
			else {
				fs::path output = parse_name(input.string());
				enc(input,output);
			}
		}
	}

	void __fastcall SwapEnc::run_time() {
		if(cin.peek()=='\n') {
			cout << "Please specify at least one file!\n" << endl;
		}
		else {
			encrypt_loop(cin);
		}
		Exit();
	}

	void SwapEnc::command_line(int argc, char** argv) {
		int i = 1;
		string a = "";
		while (true) {
			a += argv[i];
			i++;
			if(i>=argc) {
				break;
			}
			a += ' ';
//			int pos{ 0 };
//			while(pos<a.length()){
//				pos=a.find("\\\\", pos);
//				if (pos != string::npos) {
//					a.erase(pos, 2);
//					pos++;
//				}else{break;}
//			}
		}
		istringstream iss{ a };
		encrypt_loop(iss);
	}

	void SwapEnc::Exit() {
		char c;
		cout << "\nContinue?[Y/n]\t";
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


//int main() function for the program to run independently (eg. debug purpose)
#ifndef _WINDLL
int main(int argc, char* argv[]) {
	enc::SwapEnc This;
	if(argc <= 1) {
		This.name_ver();
		while(This.on) {
			try {
				This.prompt();
				This.run_time();
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


//The function for export in extern "C"
#ifdef _WINDLL 
void SwapEnc() {
	enc::SwapEnc This;
	enc::SwapEnc::name_ver();
	while (true) {
		try {
			enc::SwapEnc::prompt();
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

