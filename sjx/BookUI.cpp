#include "BookUI.h"
#include <utility>

using namespace std;
namespace UJr2_funcs {
	namespace book {
		void UI::prompt() {
			cout << "Please enter a command:......\n" << endl;
		}

		void UI::name_ver() {
			cout << "Book_Index\n";
			cout << "v0.0.0.0\n";
			cout << "Simple book control program with key-word control.\n" << endl;
		}

		void UI::UI_main() {
			name_ver();
			while (on) {
				try {
					parser();
				}
				catch (exception& e) {
					cerr << e.what() << endl;
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
				}
			}
		}


		void UI::ignore_space(std::istream& ist) {
			char c;
			ist.get(c);
			while (c == ' ') {
				ist.get(c);
			}
			ist.putback(c);
		}

		void UI::parser() {
			string cmd;
			prompt();
			cin >> cmd;
			ignore_space(cin);
			if (cmd == "exit") {
				on = false;
			}
			else if (cmd == "add") {
				add();
			}
			else if (cmd == "del") {
				del();
			}
			else if (cmd == "list") {
				list();
			}
			else if (cmd == "ntoken") {
				cin >> cmd;
				base.ntoken(cmd);
			}
			else if (cmd == "save") {
				base.save();
			}
			else if (cmd == "find") {
				find();
			}
			else {
				throw runtime_error("UI::UI_main: unknown command!");
			}
			cout << endl;
		}

		void UI::add() {
			string cmd;
			getline(cin, cmd);
			if (add_file(cmd)) {
				return;
			}
			add_manual(cmd);
		}

		bool UI::add_file(const string& cmd) {
			ifstream ifs;
			const string path = parse_path(cmd);
			ifs.open(path);
			if (ifs) {
				add_stream(ifs);
				ifs.close();
				return true;
			}
			return false;
		}

		void UI::add_manual(const string& cmd) {
			cout << "\nAdding book \"" << cmd << "\"....." << endl;
			add_string(cmd);
		}

		void UI::add_stream(istream& ist) {
			string cmd;
			while (!ist.eof()) {
				getline(ist, cmd);
				if (!cmd.empty()) {
					add_string(cmd);
				}
			}
		}

		void UI::add_string(const std::string& line) {
			//有序号的可能
			istringstream iss(line);
			int index; string name;
			if (iss >> index) {
				getline(iss, name);
				name = parse_bookname(name);
				base.add(index, name);
				return;
			}
			name = parse_bookname(line);
			base.add(name);
		}

		void UI::del() {
			string cmd;
			getline(cin, cmd);
			if (del_file(cmd)) {
				return;
			}
			del_manual(cmd);
		}

		bool UI::del_file(const std::string& cmd) {
			ifstream ifs;
			const string path = parse_path(cmd);
			ifs.open(path);
			if (ifs) {
				del_stream(ifs);
				ifs.close();
				return true;
			}
			return false;
		}

		void UI::del_manual(const std::string& cmd) {
			cout << "\nDeleting book \"" << cmd << "\"......" << endl;
			del_string(cmd);
		}

		void UI::del_stream(std::istream& ist) {
			string cmd;
			while (!ist.eof()) {
				getline(ist, cmd);
				if (!cmd.empty()) {
					del_string(cmd);
				}
			}
		}

		//为了用户操作简便，目前不支持数字开头的书名
		void UI::del_string(const std::string& line) {
			istringstream iss(line);
			int index;
			if (iss >> index) {
				base.del(index);
				return;
			}
			const string name = parse_bookname(line);
			base.del(name);
		}

		void UI::list() {
			string cmd;
			cin >> cmd;
			ignore_space(cin);
			if (cmd == "book") {
				list_book();
			}
			else if (cmd == "index") {
				list_index();
			}
			else {
				throw runtime_error("UI::list: unknown command!");
			}
		}

		void UI::list_book() const {
			char c;
			ignore_space(cin);
			cin.get(c);
			if (c == '>') {
				ofstream ofs;
				string s;
				ignore_space(cin);
				cin >> s;
				ofs.open(s);
				if (!ofs) {
					throw runtime_error("UI::list_book: cannot open file for output!");
				}
				list_book_stream(ofs);
				ofs.close();
			}
			else {
				cout << '\n';
				list_book_stream(cout);
				cout << endl;
			}
		}

		void UI::list_index() {
			char c;
			ignore_space(cin);
			cin.get(c);
			if (c == '>') {
				ofstream ofs;
				string s;
				ignore_space(cin);
				cin >> s;
				ofs.open(s);
				if (!ofs) {
					throw runtime_error("UI::list_index: cannot open file for output!");
				}
				list_index_stream(ofs);
				ofs.close();
			}
			else {
				cout << '\n';
				list_index_stream(cout);
			}
		}

		void UI::list_book_stream(std::ostream& ost) const {
			ost << "The following is the information of all the books in repository:\n" << endl;
			base.print_booklist(ost);
		}

		void UI::list_index_stream(std::ostream& ost) {
			ost << "The following are all the indexes in repository:\n" << endl;
			base.print_index(ost);
		}

		void UI::find() {
			string cmd;
			cin >> cmd;
			ignore_space(cin);
			if (cmd == "book") {
				find_book();
			}
			else if (cmd == "token") {
				find_token();
			}
			else {
				throw runtime_error("UI::list: unknown command!");
			}
		}

		void UI::find_book() const {
			string cmd;
			int index;
			ignore_space(cin);
			getline(cin, cmd);
			istringstream iss;
			if (iss >> index) {
				cout << '\n';
				base.print_book(index, cout);
			}
			else {
				string name = parse_bookname(cmd);
				Book::To_standard(name);
				cout << '\n';
				base.print_book(name, cout);
			}
		}

		void UI::find_token() {
			string token;
			cin >> token;
			Book::To_standard(token);
			char c;
			ignore_space(cin);
			cin.get(c);
			if (c == '>') {
				ignore_space(cin);
				string path;
				getline(cin, path);
				path = parse_path(path);
				ofstream ofs;
				ofs.open(path);
				base.print_token(token, ofs);
				ofs.close();
				return;
			}
			cout << '\n';
			base.print_token(token, cout);
		}


		string UI::parse_bookname(const string& line) {
			istringstream iss(line);
			string name;
			while (iss) {
				string current;
				iss >> current;
				name.append(current);
				name.push_back(' ');
			}
			name.pop_back();
			return name;
		}

		string UI::parse_path(const string& line) {
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
	}
}
