#include "BinSort.h"

using namespace std;

bool on{ true };

void BinSort::search() {
	int index;
	cin >> index;
	search_verbose(index);
}

void BinSort::search_verbose(int index) {
	long long place = root;
	while (place >= 0) {
		if (data[place].data == index) {
			cout << "Index found!\n";
			cout << endl;
			return;
		}
		else if(data[place].data>index) {
			cout << "Index entry " << data[place].data << " larger than given index " << index << "! going to left child...\n";
			place = data[place].lchild;
		}
		else if (data[place].data<index) {
			cout << "Index entry " << data[place].data << " smaller than given index " << index << "! going to right child...\n";
			place = data[place].rchild;
		}
	}
	cout << "No Index found!\n";
	cout << endl;
}

void BinSort::print() {
	for (size_t i = 0; i < size; i++) {
		cout << "Data:\t" << data[i].data << '\n';
		cout << "Depth:\t" << data[i].depth << '\n';
		cout << "Parent:\t" << data[i].parent << '\n';
		cout << "lChild:\t" << data[i].lchild << '\n';
		cout << "rChild:\t" << data[i].rchild << '\n';
		cout << endl;
	}
}

void BinSort::UI() {
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

