#include "Maze.h"

using namespace std;

void Maze_map::print_map_value(std::ostream & ost) {
	unsigned a;
	for (size_t i = 0; i<length; i++) {
		for (size_t j = 0; j<width; j++) {
			a = map[i][j];
			ost << a << '\t';
		}
		ost << '\n';
	}
	ost << endl;
}

namespace Maze {

	bool Maze::on{ true };

	Maze::Maze() {
		string line;
		prompt();
		getline(cin, line);
		istringstream iss{ line };
		size_t width;
		size_t length;
		if (iss >> length >> width) {
			maze = new Maze_map{ length,width };
			maze->init();
			route_map = new Route_map{ length,width };
			Queue = new Cyclic_Queue<step>{ 4 * length*width };
			Route = new Stack<step>{ length*width };
			load_maze(cin);
		}
		else {
			line = parse_path(line);
			ifstream ifs;
			ifs.open(line);
			if (!ifs) {
				throw runtime_error("Maze::Maze(): cannot open specified file!");
			}
			scan_maze_input(ifs);
			ifs.close();
			ifs.open(line);
			if (!ifs) {
				throw runtime_error("Maze::Maze(): cannot open specified file!");
			}
			load_maze(ifs);
			return;
		}
	}

	void Maze::Interaction() {
		std::cout << "The specified maze is:\n";
		maze->print_map_value(std::cout);
		solve_maze();
		print_result(cout);
		conti();
	}

	void Maze::name_ver() {
		cout << "Maze\n";
		cout << "v0.0.0.0\n";
		cout << "Simple breadth-first maze solver.\n" << endl;
	}

	void Maze::solve_maze() {
		find_route_verbose(cout);
	}

	void Maze::print_result(ostream& ost) {
		ost << "The result is:\n";
		Route->print(ost);
		ost << "Row:\t" << end_row << '\n';
		ost << "Column:\t" << end_col << '\n';
		ost << "Done!\n";
		ost << endl;
	}

	void Maze::start() {
		current.row = start_row;
		current.col = start_col;
		current.direction = null;
		set_passed(current);
//		Route->push_back(current);
	}

	void Maze::look_around() {
		current.direction = up;
		do {
			if (direction_okay() && Queue_okay()) {
				step temp = current;
				temp.walk();
				set_passed(temp);
				Route->copy_to(*(route_map->map[temp.row][temp.col]));
				route_map->map[temp.row][temp.col]->push_back(current);
//				route_map->map[temp.row][temp.col]->push_back(temp);
				Queue->push_back(temp);
			}
			current.next_direction();
		} while (current.direction != up);
	}

	void Maze::pop_front() {
		current = Queue->pop_front();
		route_map->map[current.row][current.col]->copy_to(*Route);
	}

	void Maze::walk() {
//		parse_route(current);
		current.walk();
//		set_passed();
	}

	void Maze::parse_route(step added) {
		size_t size = Route->size();
		size_t end{ 0 };
		if (size != 0) {
			bool go_back{ false };
			for (size_t i = size - 1; i >= 0; i--) {
				if ((*Route)[i].row == added.row && (*Route)[i].col == added.col) {
					go_back = true;
					break;
				}
				if (i == 0) {
					break;
				}
			}
			if (go_back) {
				step temp = Route->pop_back();
				while (temp.row != added.row || temp.col != added.col) {
					temp = Route->pop_back();
				}
			}
		}
		Route->push_back(added);
	}

	void Maze::set_passed(const step& added) {
		maze->map[added.row][added.col] = 2;
	}

	void Maze::reset_passed(const step& deleted) {
		maze->map[deleted.row][deleted.col] = 0;
	}

	//true only if the location headed is a okay path and not covered before
	bool Maze::direction_okay() {
		if (current.direction == up) {
			if (current.row == 0) {
				return false;
			}
			return !(maze->map[current.row - 1][current.col]);
		}
		if (current.direction == down) {
			if (current.row == maze->length - 1) {
				return false;
			}
			return !(maze->map[current.row + 1][current.col]);
		}
		if (current.direction == left) {
			if (current.col == 0) {
				return false;
			}
			return !(maze->map[current.row][current.col - 1]);
		}
		if (current.direction == right) {
			if (current.col == maze->width - 1) {
				return false;
			}
			return !(maze->map[current.row][current.col + 1]);
		}
		throw runtime_error("Maze::direction_okay(): Unknown direction!");
	}

	bool Maze::Queue_okay() {
		size_t i = Queue->start;
		step next = current.peek();
		while (true) {
			if (Queue->data[i].peek() == next) {
				return false;
			}
			if (i == Queue->end) {
				break;
			}
			i++;
			if (i >= Queue->total_size) {
				i -= Queue->total_size;
			}
		}
		return true;
	}

	bool Maze::finished() {
		if (current.row == end_row && current.col == end_col) {
			return true;
		}
		return false;
	}

	void Maze::find_route() {
		start();
		size_t steps{ 0 };
		while (!finished() && steps < (maze->length*maze->width)) {
			look_around();
			pop_front();
			walk();
			steps++;
		}
		if (steps >= (maze->length*maze->width)) {
			throw runtime_error("Maze::find_route(): cannot find route to solve maze!");
		}
	}

	void Maze::find_route_verbose(std::ostream& ost) {
		start();
		size_t steps{ 0 };
		ost << "Current maze is:\n";
		maze->print_map_value(ost);
		while (!finished() && steps < (maze->length*maze->width)) {
			ost << "Current path is:\n";
			Route->print(ost);
			ost << "Finding directions that are okay...\n";
			look_around();
			ost << "Queue is now:\n";
			Queue->print(ost);
			ost << "Setting new current state...\n";
			pop_front();
			ost << "Current state is now:\n";
			current.print(ost);
//			ost << "Walking according to direction...\n";
//			walk();
			steps++;
			ost << "Current state is now:\n";
			current.print(ost);
			ost << "Current maze is:\n";
			maze->print_map_value(ost);
		}
		if (steps >= (maze->length*maze->width)) {
			throw runtime_error("Maze::find_route_verbose(): cannot find route to solve maze!");
		}
	}

	void Maze::set_end(size_t row, size_t col) {
		end_row = row;
		end_col = col;
	}
	void Maze::set_start(size_t row, size_t col) {
		start_row = row;
		start_col = col;
	}

	void Maze::prompt() {
		cout << "Please specify the maze you wish to solve:\n" << endl;
	}

	void Maze::load_maze(std::istream& ist) {
		//	size_t row{ 0 };
		//	size_t col{ 0 };
		string line;
		char value;
		bool ended{ false };
		bool started{ false };
		for (size_t row = 0; row < maze->length; row++) {
			//		if (row >= maze->length) {
			//			throw range_error("Maze::load_maze(std::ifstream&): too many lines for given length!");
			//		}
			getline(ist, line);
			istringstream iss{ line };
			//		col = 0;
			for (size_t col = 0; col < maze->width; col++) {
				if (line.size() != maze->width) {
					throw range_error("Maze::load_maze(std::ifstream&): input on certain line too long for given width!");
				}
				iss.get(value);
				if (value == '0') {
					maze->map[row][col] = 0;
				}
				else if (value == '1') {
					maze->map[row][col] = 1;
				}
				else if (value == '2') {
					maze->map[row][col] = 0;
					set_start(row, col);
					started = true;
				}
				else if (value == '3') {
					maze->map[row][col] = 0;
					set_end(row, col);
					ended = true;
				}
				else {
					throw range_error("Maze::load_maze(std::ifstream&): maze can only be consisted of '0's, '1's, '2's, or '3's!");
				}
			}
			//		if(col!=maze->width-1) {
			//			throw range_error("Maze::load_maze(std::istream&): wrong length of line!");
			//		}
			//		row++;
		}//while (line != "");
	//	if(row!=maze->length-1) {
	//		throw range_error("Maze::load_maze(std::istream&): wrong number of lines!");
	//	}
		if (!started || !ended) {
			throw runtime_error("Maze::load_maze(std::ifstream&): no start or end point specified!");
		}
	}

	void Maze::scan_maze_input(std::ifstream& ifs) {
		string line;
		static size_t width;
		size_t length{ 0 };
		getline(ifs, line);
		while (line != "" && !ifs.eof()) {
			length++;
			width = line.size();
			if (line.size() != width) {
				throw runtime_error("Maze::scan_maze_input(std::ifstream&): Different lenghth on different lines!");
			}
			getline(ifs, line);
		}
		maze = new Maze_map{ length,width };
		maze->init();
		route_map = new Route_map{ length,width };
		Queue = new Cyclic_Queue<step>{ 4*length*width };
		Route = new Stack<step>{ length*width };
	}

	void Maze::conti() {
		char c;
		cout << "Continue? [Y/n]\t";
		cin.get(c);
		switch (c) {
		case 'Y': case 'y': case '\n':
			cout << endl;
			return;
		case 'N': case 'n':
			on = false;
			return;
		default:
			throw runtime_error("Maze::conti(): unknown choice!");
		}
	}

	string Maze::parse_path(string line) {
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