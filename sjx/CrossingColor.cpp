#include "Crossing.h"
#include "CrossingColor.h"

using namespace std;

namespace UJr2_funcs {
	namespace crossing {
		void Color::find_groups() {
			cout << "\nFinding solutions...\n";
			cout << "Please wait..." << endl;
			for(int i = 0; i < crossing::possi; i++) {
				assign(i);
			}
			cout << "Done!\n" << endl;
		}

		bool Color::evaluate(int cur, int opp) const {
			//	cout << "Evaluating " << opp << " against group " << cur << endl;
			for(int i = 0; i < crossing::possi; i++) {
				//		cout << "evaluating against " << i << endl;
				if(this->group[i] == cur && !this->rel->map[opp][i]) {
					//			cout << "Not okay!" << endl;
					return false;
				}
			}
			//	cout << "okay!" << endl;
			return true;
		}

		void Color::assign(int add) {
			for(int i = 1; i <= this->color_num; i++) {
				if(evaluate(i, add)) {
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

		void Color::print_result() {
			cout << "At least " << this->color_num <<
			" groups needed for the grouping! The folowing is one possibility:" << endl;
			for(int i = 1; i <= color_num; i++) {
				//各个组的编号
				cout << "Group No. " << i << ":\t"; //用1st啥的太麻烦了，英语语法还要写个函数...
				for(int cur = 0; cur < crossing::possi; cur++) {
					if(this->group[cur] == i) {
						cout << this->rel->This->routes[cur].from << "->" << this->rel->This->routes[cur].to << '\t';
					}
				}
				cout << endl;
			}
			cout << endl;
		}
	}
}
