#include "Colors.h"

using namespace std;

void Colors::find_groups() {
	cout << "\nFinding solutions...\n";
	cout << "Please wait..." << endl;
	for (int i = 0; i < Crossing::possi; i++) {
		assign(i);
	}
	cout << "Done!\n" << endl;
}

bool Colors::evaluate(int cur, int opp) {
//	cout << "Evaluating " << opp << " against group " << cur << endl;
	for (int i = 0; i < Crossing::possi; i++) {
//		cout << "evaluating against " << i << endl;
		if (this->group[i] == cur && !this->rel->map[opp][i]) {
//			cout << "Not okay!" << endl;
			return false;
		}
	}
//	cout << "okay!" << endl;
	return true;
}

void Colors::assign(int add) {
	for (int i = 1; i <= this->color_num; i++) {
		if (evaluate(i, add)) {
			group[add] = i;
//			cout << "Assigned " << add << " to group " << i << endl;
			return;
		}
	}
	color_num++;
	group[add] = color_num;
//	cout << "Assigned " << add << " to group " << color_num << endl;
	//	return;
}

void Colors::print_result() {
	cout <<"At least "<< this->color_num << " groups needed for the grouping! The folowing is one possibility:" << endl;
	for (int i = 1; i <= color_num; i++) {				//各个组的编号
		cout << "Group No. " << i << ":\t";				//用1st啥的太麻烦了，英语语法还要写个函数...
		for (int cur = 0; cur < Crossing::possi; cur++) {
			if (this->group[cur] == i) {
				cout << this->rel->This->routes[cur].from << "->" << this->rel->This->routes[cur].to << '\t';
			}
		}
		cout << endl;
	}
	cout << endl;
}

