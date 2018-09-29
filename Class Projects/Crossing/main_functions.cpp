#include "main.h"

using namespace std;

void name_ver() {
	cout << "\nCrossing \n";
	cout << "Version 0.0.0.0\n" << endl;
}

void prompt() {
	cout << "This program can calculate the smallest possible number of route groups needed \nfor a crossing with given number and state of roads to function.\n" << endl;
	cout << "The roads may be two-ways or one-way roads according to your input.\n" << endl;
	cout << endl;
}

void function() {
	Crossing::Roads current;
	Crossing::relations rel{ current };
	rel.find_relations();
	rel.print_rel();
	Colors color{ rel };
	color.find_groups();
	color.print_result();
}

void Exit() {
	char c = cin.get();
	cout << "Continue?[Y/n]\t";
	cin.clear();
	c = cin.get();				//cin的输入很奇怪，貌似自带一个'\n'
	switch (c) {
	case 'n': case 'N':
		exit(0);
	case '\n': case 'y': case 'Y':
		break;
	default:
		throw runtime_error("Exit(): Unkown choice encountered!");
	}
}
