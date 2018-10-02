#include "BinSort.h"

using namespace std;

namespace UJr2_funcs {
	int BinSort(int argc, char* argv[]) {
		binsort::BinSort::name_ver();
		try {
			binsort::BinSort This;
			while (binsort::on) {
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
		return 0;
	}
	namespace binsort {
		bool on{ true };

		void BinSort::search() const {
			int index;
			cin >> index;
			search_verbose(index);
		}

		void BinSort::search_verbose(int index) const {
			long long place = root;
			while (place >= 0) {
				if (data[place].data == index) {
					cout << "Index found!\n";
					cout << endl;
					return;
				}
				else if (data[place].data > index) {
					cout << "Index entry " << data[place].data << " larger than given index " << index << "! going to left child...\n";
					place = data[place].lchild;
				}
				else if (data[place].data < index) {
					cout << "Index entry " << data[place].data << " smaller than given index " << index << "! going to right child...\n";
					place = data[place].rchild;
				}
			}
			cout << "No Index found!\n";
			cout << endl;
		}

		void BinSort::print() const {
			for (size_t i = 0; i < size; i++) {
				cout << "Data:\t" << data[i].data << '\n';
				cout << "Depth:\t" << data[i].depth << '\n';
				cout << "Parent:\t" << data[i].parent << '\n';
				cout << "lChild:\t" << data[i].lchild << '\n';
				cout << "rChild:\t" << data[i].rchild << '\n';
				cout << endl;
			}
		}

		void BinSort::UI() const {
			cout << ">";
			if (isdigit(cin.peek())) {
				search();
			}
			else {
				string cmd;
				cin >> cmd;
				if (cmd == "exit") {
					on = false;
				}
				else if (cmd == "search") {
					search();
				}
				else if (cmd == "print") {
					cout << endl;
					cout << "The following is the memory of the items:\n" << endl;
					print();
				}
			}
			char c;
			do {
				cin.get(c);
			} while (c != '\n');
		}

		void BinSort::name_ver() {
			cout << "Simple binary sort program\n";
			cout << "v0.0.0.0\n";
			cout << endl;
		}

		std::string parse_path(std::string line) {
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
