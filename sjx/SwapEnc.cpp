#include "SwapEnc.h"
#include "../Lib/Pathman.h"

using namespace std;

namespace enc {

	void SwapEnc::name_ver() {
		cout << "Simple encryptor & decryptor\n";
		cout << "Version " << STRING(ENC_VERSION) << '\n' << endl;
	}

	void SwapEnc::prompt() {
		cout << "Please specify files for encryption or decryption.\n";
		cout << "You may drag and drop one by one, and then press \"enter\" if you like.\n" << endl;
	}

	inline void __fastcall SwapEnc::enc(fs::path input, fs::path output) {
		if(input.empty()) {
			throw std::runtime_error("Specified file empty!");
		}
		ifs.open(input, ios::binary);
		if(ifs.fail()) {
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
		while(!ifs.eof()) {
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
		if(c == ' ') {
			do {
				ist.get(c);
			}
			while(c == ' ');
			ist.putback(c);
		}
		if(c == '\n') {
			return "";
		}
		if(c != '\"') {
			ist >> a;
			return a;
		}
		a = "";
		ist.get();
		while(true) {
			ist.get(c);
			if(c != '\"' && c != '\\') {
				a += c;
			} else if(c == '\"') {
				break;
			} else if(c == '\\') {
				const char t = ist.peek();
				if(t == '\\') {
					ist.get();
				} else {
					a += c;
				}
			}
		}
		//			cout << "Encrypting the file \"" << a << "\"" << endl;
		return a;
	}

	std::string SwapEnc::parse_name(const std::string& ori) {
		string rtn;
		if(ori.substr(ori.length() - 4, 4) == ".enc") {
			rtn = ori.substr(0, ori.length() - 4);
		} else {
			rtn = ori + ".enc";
		}
		return rtn;
	}

	inline void __fastcall SwapEnc::encrypt_loop(std::istream& ist) {
		while(true) {
			const string name = parse_pathnames(ist);
			if(name.empty()) {
				break;
			}
			fs::path input = name;
			if(is_directory(input)) {
				fs::path output = parse_name(input.string());
				pathman::iopath_recursive_iterator it{input, output};
				//				cout << "Iterator constructed!\n";
				while(it != pathman::iopath_recursive_iterator::end()) {
					//					cout << "Current input path: " << it.cur_ipath << '\n';
					//					cout << "Current output path: " << it.cur_opath << '\n';
					if(is_directory(it.cur_ipath)) {
						create_directory(it.cur_opath); //parse_name(it.cur_opath.string()));
					} else {
						enc(it.cur_ipath, parse_name(it.cur_opath.string()));
					}
					++it;
				}
			}
				//			if (fs::is_regular_file(input)) {
			else {
				const fs::path output = parse_name(input.string());
				enc(input, output);
			}
		}
	}

	void SwapEnc::run_time() {
		if(cin.peek() == '\n') {
			cout << "Please specify at least one file!\n" << endl;
		} else {
			encrypt_loop(cin);
		}
		Exit();
	}

	void SwapEnc::command_line(int argc, char** argv) {
		int i = 1;
		string a;
		while(true) {
			a += argv[i];
			i++;
			if(i >= argc) {
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
		istringstream iss{a};
		encrypt_loop(iss);
	}

	void SwapEnc::Exit() {
		char c;
		cout << "\nContinue?[Y/n]\t";
		cin.clear();
		cin.get();
		cin.get(c);
		switch(c) {
			case 'Y': case 'y': case '\n': case EOF:
				cout << endl;
				break;
			case 'N': case 'n':
				on = false;
				break;
			default:
				do {
					cin.get(c);
				}
				while(c != '\n' && c != EOF);
				throw runtime_error("Unkown specifier!");
		}
	}
}
