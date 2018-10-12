#include "Haffman.h"

using namespace std;

namespace UJr2_funcs {
	int Haffman(int argc, char* argv[]) {
		haffman::Haffman::prompt();
		do {
			try {
				haffman::Haffman This;
				while (This.on) {
					try {
						This.UI();
					}
					catch (exception& e) {
						cerr << e.what() << '\n' << endl;
					}
				}
			}
			catch (exception& e) {
				cerr << e.what() << '\n' << endl;
			}
		} while (haffman::on);
		return 0;
	}
	namespace haffman {
		bool on{ true };
		void Haffman::init() {
			cout << "Please specify the file you wish to use as sample:\n";
			string path;
			getline(cin, path);
			path = parse_path(path);
			ifstream ifs;
			ifs.open(path, ios:;binary);
			if (!ifs) {
				throw runtime_error("Haffman::init(): Cannot open specified file!");
			}
			cout << endl;
			cout << "Reading file to initialize frequency list...\n";
			char ch;
			ifs.get(ch);
			while (!ifs.eof()) {
				const unsigned char temp = ch;
				tree[temp].weight++;
				ifs.get(ch);
			}
			cout << endl;
			cout << "Done initializing list, initializing Haffman tree...\n";
			init_tree();
			cout << endl;
			cout << "Done!\n";
			bool root_found{ false };
			for (int i = 0; i < NUM_NODE; i++) {
				if (tree[i].parent < 0) {
					if (root_found) {
						throw runtime_error("Haffman::init(): Multiple roots found!");
					}
					root = i;
					root_found = true;
				}
			}
			if (!root_found) {
				throw runtime_error("Haffman::init(): Root not found!");
			}
			cout << endl;
			return;
		}

		void Haffman::init_tree() {
			for (unsigned short added_node = MAX_CHAR; added_node < NUM_NODE; added_node++) {
				unsigned short found = find_smallest(added_node);
				tree[added_node].rchild = found;
				tree[found].parent = added_node;
				found = find_smallest(added_node);
				tree[added_node].lchild = found;
				tree[found].parent = added_node;
				tree[added_node].weight = tree[tree[added_node].lchild].weight + tree[tree[added_node].rchild].weight;
				//		tree[added_node].leaf = false;
			}
		}

		unsigned short Haffman::find_smallest(short range) const {
			int j = 0;
			while (tree[j].parent >= 0) {
				j++;
			}
			unsigned short rtn = j;
			int i = 0;
			while (i < range) {
				if (tree[i].parent < 0) {
					if (tree[i].weight < tree[rtn].weight) {
						rtn = i;
					}
				}
				i++;
			}
			return  rtn;
		}

		void Haffman::print_tree() const {
			for (int i = 0; i < NUM_NODE; i++) {
				cout << "Index:\t\t" << i << '\n';
				tree[i].print(cout);
			}
		}

		void node::print(std::ostream& ost) const {
			ost << "Weight:\t\t" << weight << "\n";
			ost << "Parent:\t\t" << parent << "\n";
			ost << "Left child:\t" << lchild << "\n";
			ost << "Right child:\t" << rchild << "\n";
			ost << endl;
		}

		void Haffman::encode(std::istream& input, std::ostream& output) const {
			char c;
			input.get(c);
			while (!input.eof()) {
				const unsigned char temp = c;
				encode_char(temp, output);
				input.get(c);
			}
		}

		void Haffman::decode(std::istream& input, std::ostream& output) const {
			char c;
			short current = root;
			do {
				while (!input.eof()) {
					input.get(c);
					if (c == '0') {
						if (tree[current].lchild < 0) {
							if (current >= MAX_CHAR) {
								throw runtime_error("Haffman::decode(std::istream&, std::ostream&): Decode ended with intermediate node!");
							}
							output << char(current);
							current = tree[root].lchild;
							break;
						}
						current = tree[current].lchild;
					}
					else if (c == '1') {
						if (tree[current].rchild < 0) {
							if (current >= MAX_CHAR) {
								throw runtime_error("Haffman::decode(std::istream&, std::ostream&): Decode ended with intermediate node!");
							}
							output << char(current);
							current = tree[root].rchild;
							break;
						}
						current = tree[current].rchild;
					}
					else {
						throw runtime_error("Haffman::decode(std::istream&, std::ostream&): Unknown signal state!");
					}
				}
			} while (!input.eof());
		}

		void Haffman::encode_char(unsigned char ch, std::ostream& output) const {
			short current = ch;
			short parent = tree[ch].parent;
			stack<bool> encoding;
			while (parent >= 0) {
				if (tree[parent].lchild == current) {
					encoding.push(false);
					current = parent;
				}
				else if (tree[parent].rchild == current) {
					encoding.push(true);
					current = parent;
				}
				else {
					throw runtime_error("Haffman::decode_char(unsigned char, std::ostream&): parent has no record of child!");
				}
				parent = tree[current].parent;
			}
			while (!encoding.empty()) {
				output << encoding.top();
				encoding.pop();
			}
		}

		void Haffman::UI() {
			cout << "Please input a command...\n";
			string cmd;
			char c = 0;
			cin >> cmd;
			if (cmd == "exit") {
				haffman::on = false;
				on = false;
			}
			else if (cmd == "re-encode") {
				while (c != '\n') {
					cin.get(c);
				}
				on = false;
			}
			else if (cmd == "encode") {
				while (c != '\n') {
					cin.get(c);
				}
				cout << endl;
				encode();
			}
			else if (cmd == "decode") {
				while (c != '\n') {
					cin.get(c);
				}
				cout << endl;
				decode();
			}
			else if (cmd == "print") {
				while (c != '\n') {
					cin.get(c);
				}
				print_code();
			}
			else {
				throw runtime_error("Haffman::UI(): Unknown command!");
			}
			cout << endl;
		}

		void Haffman::encode() const {
			cout << "Please specify the input file:\n";
			string cmd;
			getline(cin, cmd);
			cmd = parse_path(cmd);
			ifstream ifs;
			ifs.open(cmd, ios::binary);
			if (!ifs) {
				throw runtime_error("Haffman::encode(): Cannot open file for input!");
			}
			cout << endl;
			cout << "Please specify output file:\n";
			getline(cin, cmd);
			cmd = parse_path(cmd);
			ofstream ofs;
			ofs.open(cmd, ios::binary);
			if (!ofs) {
				throw runtime_error("Haffman::encode(): Cannot open file for output!");
			}
			encode(ifs, ofs);
			cout << endl;
			cout << "Done!\n" << endl;
			ifs.close();
			ofs.close();
		}

		void Haffman::decode() const {
			cout << "Please specify the input file:\n";
			string cmd;
			getline(cin, cmd);
			cmd = parse_path(cmd);
			ifstream ifs;
			ifs.open(cmd, ios::binary);
			if (!ifs) {
				throw runtime_error("Haffman::decode(): Cannot open file for input!");
			}
			cout << endl;
			cout << "Please specify output file:\n";
			getline(cin, cmd);
			cmd = parse_path(cmd);
			ofstream ofs;
			ofs.open(cmd, ios::binary);
			if (!ofs) {
				throw runtime_error("Haffman::decode(): Cannot open file for output!");
			}
			decode(ifs, ofs);
			cout << endl;
			cout << "Done!\n" << endl;
			ifs.close();
			ofs.close();
		}

		string Haffman::parse_path(const string& line) {
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

		void Haffman::print_code() const {
			cout << "The current tree is:\n" << endl;
			print_tree();
		}
	}
}
