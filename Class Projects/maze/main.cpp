#include "Maze.h"

int main() {
	Maze::Maze::name_ver();
	while(Maze::Maze::on) {
		try {
			Maze::Maze This;
			This.Interaction();
		}
		catch (std::exception& e) {
			std::cerr << e.what() << '\n' << std::endl;
		}
	}
	return 0;
}
