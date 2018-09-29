#include "Crossing.h"
#include "CrossingColor.h"

using namespace std;

namespace UJr2_funcs {
	int Crossing(int argc, char* argv[]) {
		//让用户输入，并算出各种路线的关系
		crossing::name_ver();
		crossing::prompt();
		bool not_1st{ false };
		while (true) {
			try {
				if (not_1st) {
					crossing::Exit();
					if(crossing::exit) {
						return 0;
					}
					crossing::name_ver();
				}
				crossing::function();
			}
			catch (exception& e) {
				cout << '\n' << e.what() << '\n' << endl;
				cin.clear();
			}
			not_1st = true;
		}
	}
	namespace crossing {
		bool exit = false;
		//The UI functions go here
		void name_ver() {
			cout << "Crossing\n";
			cout << "Version 0.0.0.0\n" << endl;
		}

		void prompt() {
			cout <<
			"This program can calculate the smallest possible number of route groups needed \nfor a crossing with given number and state of roads to function.\n"
			<< endl;
			cout << "The roads may be two-ways or one-way roads according to your input.\n" << endl;
			cout << endl;
		}

		void function() {
			crossing::Roads current;
			crossing::relations rel{current};
			rel.find_relations();
			rel.print_rel();
			Color color{rel};
			color.find_groups();
			color.print_result();
		}

		void Exit() {
			char c = cin.get();
			cout << "Continue?[Y/n]\t";
			cin.clear();
			c = cin.get(); //cin的输入很奇怪，貌似自带一个'\n'
			switch(c) {
				case 'n': case 'N':
					//fix here
					exit = true;
					return;
				case '\n': case 'y': case 'Y':
					break;
				default:
					throw runtime_error("Exit(): Unkown choice encountered!");
			}
		}
	}
}
