#include "Maze.h"

namespace UJr2_funcs {
	int maze(int argc, char* argv[]) {
		Maze::Maze::Maze::name_ver();
		while (Maze::Maze::Maze::on) {
			try {
				Maze::Maze::Maze This;
				This.Interaction();
			}
			catch (std::exception& e) {
				std::cerr << e.what() << '\n' << std::endl;
			}
		}
		return 0;
	}
}
