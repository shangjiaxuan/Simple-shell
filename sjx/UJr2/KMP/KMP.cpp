#include "KMP.h"

using namespace std;

namespace UJr2_funcs {
	int KMP(int argc, char* argv[]) {
		while(kmp::on) {
			try {
				kmp::interaction();
			} catch(exception& e) {
				cerr << '\n' << e.what() << '\n' << endl;
			}
		}
		return 0;
	}

	namespace kmp {
		bool on{true};

		void interaction() {
			string target;
			string pattern;
			cout << "KMP algorithm\n" << endl;
			cout << "Please note that only ASCII characters are properly supported here\n" << endl;
			cout << "Please specify the target string." << endl;
			getline(cin, target);
			if(target.empty()) {
				throw runtime_error("interaction: target found to be empty!");
			}
			cout << '\n' << "Please specify the pattern string." << endl;
			getline(cin, pattern);
			if(pattern.empty()) {
				throw runtime_error("interaction: pattern found to be empty!");
			}
			if(target.size() < pattern.size()) {
				throw runtime_error("interaction: target cannot be shorter than pattern!");
			}
			KMP This(pattern.c_str(), target.c_str());
			long long plc = This.find();
			if(plc != -1) {
				cout << "\nThe pattern is found starting at the " << This.find() + 1 << "th character\n" << endl;
			} else {
				cout << "\nNo pattern string found!" << '\n' << endl;
			}
			string s;
			while(true) {
				cout << "Continue? [Y/n]\t";
				getline(cin, s);
				if(s == "N" || s == "n") {
					kmp::on = false;
					cout << endl;
					break;
				}
				if(s == "Y" || s == "y" || s.empty()) {
					kmp::on = true;
					cin.clear();
					cout << endl;
					break;
				} else {
					cout << "Unknown option." << endl;
				}
			}
		}

		void KMP::init(const char* pat, const char* tar) {
			Pattern(pat);
			Target(tar);
			MakeNext();
		}

		KMP::KMP(const char* pat, const char* tar) {
			init(pat, tar);
		}

		void KMP::Pattern(const char* w) {
			pattern_size = strlen(w);
			char* temp_pattern = new char[pattern_size];
			for(long long i = 0; i < pattern_size; i++) {
				temp_pattern[i] = w[i];
			}
			pattern = temp_pattern;
		}

		void KMP::Target(const char* t) {
			target_size = strlen(t);
			char* temp_target = new char[target_size];
			for(long long i = 0; i < target_size; i++) {
				temp_target[i] = t[i];
			}
			target = temp_target;
		}

		void KMP::MakeNext() {
			long long* temp_next = new long long[pattern_size];
			temp_next[0] = -1;
			long long This = 0;
			long long Head = -1;
			while(This < pattern_size - 1) {
				//计算next[i+1]
				while(Head >= 0 && pattern[This] != pattern[Head]) {
					Head = temp_next[Head];
				}
				This++;
				Head++;
				if(pattern[This] != pattern[Head]) {
					temp_next[This] = Head;
				} else {
					temp_next[This] = temp_next[Head];
				}
			}
			next = temp_next;
		}

		long long KMP::find(long long start) const {
			long long tgplc = start;
			long long ptplc = 0;
			while(tgplc < target_size && ptplc < pattern_size) {
				if(pattern[ptplc] == target[tgplc] || ptplc == -1) {
					tgplc++;
					ptplc++;
				} else {
					ptplc = next[ptplc];
				}
			}
			if(ptplc == pattern_size) {
				return tgplc - pattern_size;
			}
			return -1;
		}

		long long KMP::find_nth(long long n) const {
			if(n == 0) {
				throw std::out_of_range("find(unsigned): has to take positive arguments!");
			}
			long long place{0};
			for(long long i = 0; i < n; i++) {
				place = find(place + pattern_size);
				if(place == -1) {
					break;
				}
			}
			return place;
		}

		long long KMP::find() const {
			return find(0);
		}
	}
}
