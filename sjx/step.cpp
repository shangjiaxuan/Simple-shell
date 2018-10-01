#include "step.h"

using namespace std;
namespace UJr2_funcs {
	namespace Maze {
		namespace Maze {
			void step::next_direction() {
				if (direction == up) {
					direction = right;
					return;
				}
				if (direction == right) {
					direction = down;
					return;
				}
				if (direction == down) {
					direction = left;
					return;
				}
				if (direction == left) {
					direction = up;
					return;
				}
				throw runtime_error("step::next_direction(): Unknown direction!");
			}

			void step::walk() {
				if (direction == up) {
					row--;
				}
				if (direction == right) {
					col++;
				}
				if (direction == down) {
					row++;
				}
				if (direction == left) {
					col--;
				}
				direction = null;
			}

			step step::peek() {
				step rtn = *this;
				if (direction == up) {
					rtn.row--;
				}
				if (direction == right) {
					rtn.col++;
				}
				if (direction == down) {
					rtn.row++;
				}
				if (direction == left) {
					rtn.col--;
				}
				rtn.direction = null;
				return rtn;
			}


			ostream& operator<<(std::ostream& ost, const step& source) {
				ost << "Row:\t" << source.row << '\n';
				ost << "Column:\t" << source.col << '\n';
				ost << "Direction:\t";
				if (source.direction == up) {
					ost << "up\n";
				}
				else if (source.direction == right) {
					ost << "right\n";
				}
				else if (source.direction == down) {
					ost << "down\n";
				}
				else if (source.direction == left) {
					ost << "left\n";
				}
				return ost;
			}
		}
	}
}
