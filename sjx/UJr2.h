#pragma once

#include "sjx_Exports.h"

#ifndef _WINDLL
int UJr2(int argc, char* argv[]);
#endif

namespace UJr2_funcs {
	int Crossing(int argc, char* argv[]);
	int Vehicle_control(int argc, char* argv[]);
	int Book(int argc, char* argv[]);
	/*
		int KMP(int argc, char* argv[]);
		int maze(int argc, char* argv[]);
		int CrossRiver(int argc, char* argv[]);
		int Haffman(int argc, char* argv[]);
		int Block_search(int argc, char* argv[]);
		int BinSort(int argc, char* argv[]);
	*/
}
