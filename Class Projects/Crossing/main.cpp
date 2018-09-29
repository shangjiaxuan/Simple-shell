//一点想法：用两个bool的数组存储每个路口可以出入的状态，用它算出要开多大的二维数组储存信息

#include "main.h"

using namespace std;

int main(){
	//让用户输入，并算出各种路线的关系
	name_ver();
	prompt();
	bool not_1st{ false };
	while(true) {
		try {
			if(not_1st) {
				Exit();
				name_ver();
			}
			function();
		}
		catch (exception& e) {
			cout << '\n' << e.what() << '\n' << endl;
			cin.clear();
		}
		not_1st = true;
	}
}

