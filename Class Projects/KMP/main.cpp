#define _CRT_SECURE_NO_WARNINGS
#include "Header.h"

#include "KMP.h"

using namespace std;

bool on{ true };
KMP This;

void interaction() {
	string target;
	string pattern;
	cout << "KMP algorithm\n" << endl;
	cout << "Please note that only ASCII characters are properly supported here\n" << endl;
	cout << "Please specify the target string." << endl;
	getline(cin,target);
	if (target.size() == 0) {
		throw runtime_error("interaction: target found to be empty!");
	}
	cout << '\n' << "Please specify the pattern string." << endl;
	getline(cin,pattern);
	if(pattern.size()==0) {
		throw runtime_error("interaction: pattern found to be empty!");
	}
	if(target.size()<pattern.size()) {
		throw runtime_error("interaction: target cannot be shorter than pattern!");
	}
	This.init(pattern.c_str(), target.c_str());
	long long plc = This.find();
	if (plc != -1) {
		cout << "\nThe pattern is found at character number " << This.find() + 1 << '\n' << endl;
	}
	else {
		cout << "\nNo pattern string found!" << '\n' << endl;
	}
	string s;
	while (true) {
		cout << "Continue? [Y/n]\t";
		getline(cin, s);
		if (s == "N" || s == "n") {
			on = false;
			cout << endl;
			break;
		}
		if (s == "Y" || s == "y") {
			on = true;
			cin.clear();
			cout << endl;
			break;
		}
		if(s.size() == 0) {
			on = true;
			cin.clear();
			cout << endl;
			break;
		}
		else {
			cout << "Unkown option." << endl;
		}
	}
}

int main(int argc, char* argv[]) {
	while (on) {
		try {
			interaction();
		}
		catch (exception& e) {
			cerr << '\n' << e.what() << '\n' << endl;
		}
	}
	return 0;
}
