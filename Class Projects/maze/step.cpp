#include "step.h"

using namespace std;

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


	void step::print(std::ostream& ost) {
		ost << "Row:\t" << row << '\n';
		ost << "Column:\t" << col << '\n';
		ost << "Direction:\t";
		if (direction == up) {
			ost << "up\n";
		}
		else if (direction == right) {
			ost << "right\n";
		}
		else if (direction == down) {
			ost << "down\n";
		}
		else if (direction == left) {
			ost << "left\n";
		}
		ost << endl;
	}
}