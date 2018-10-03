#pragma once

#include "UJr2.h"

#include "../Lib/myDataStructures.h"
#include "step.h"

namespace UJr2_funcs {
	namespace Maze {
		class Route_map {
		public:
			size_t length;
			size_t width;
			FixedStack<Maze::step>*** map;
			FixedStack<Maze::step>* data;
			Route_map() {
				length = 0;
				width = 0;
				map = nullptr;
				data = nullptr;
				throw std::runtime_error("Array_2D: cannot initialize without two integers!");
			}
			Route_map(size_t row, size_t col) {
				width = col;
				length = row;
				this->map = new FixedStack<Maze::step>**[row]();
				for (size_t i = 0; i < row; i++) {
					this->map[i] = new FixedStack<Maze::step>*[col]();
					for (size_t j = 0; j < width; j++) {
						map[i][j] = new FixedStack<Maze::step>(length*width);
					}
				}
				data = nullptr;
			}

			~Route_map() {
				for (size_t i = 0; i < length; i++) {
					for (size_t j = 0; j < width; j++) {
						delete (map[i][j]);
						map[i][j] = nullptr;
					}
					delete[] map[i];
					map[i] = nullptr;
				}
				delete[] map;
				map = nullptr;
			}
			//	~Route_map(){};
		};

		class Maze_map : public Array_2D<char> {
		public:
			Maze_map() :Array_2D<char>() {};
			Maze_map(size_t row, size_t col) : Array_2D<char>(row, col) {};
			void print_map_value(std::ostream& ost) const;
			void init() override {
				for (size_t i = 0; i < length; i++) {
					for (size_t j = 0; j < width; j++) {
						map[i][j] = 0;
					}
				}
			};
		};

		namespace Maze {
			class Maze {
			public:
				Maze();
				~Maze() {
					delete maze;
					delete route_map;
					delete Queue;
					delete Route;
				}
				void Interaction();
				static void name_ver();
				static bool on;
			private:
				Maze_map* maze{ nullptr };
				Route_map* route_map{ nullptr };
				Cyclic_Queue<step>* Queue{ nullptr };
				FixedStack<step>* Route{ nullptr };
				step current{};
				size_t end_row{};
				size_t end_col{};
				size_t start_row{};
				size_t start_col{};

				void solve_maze();
				void print_result(std::ostream& ost) const;

				void start();
				void look_around();
				void pop_front();
				void walk();
				void parse_route(step added) const;
				void set_passed(const step& added) const;
				void reset_passed(const step& deleted) const;

				bool direction_okay() const;
				bool Queue_okay();
				bool finished() const;

				void find_route();
				void find_route_verbose(std::ostream& ost);

				void set_end(size_t row, size_t col);
				void set_start(size_t row, size_t col);

				static void prompt();
				void load_maze(std::istream& ist);
				void scan_maze_input(std::ifstream& ifs);
				static void Exit();
				static std::string parse_path(const std::string& line);
			};
		}
	}
}