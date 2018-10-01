#pragma once

#include "Header.h"

namespace UJr2_funcs {
	namespace Maze {
		namespace Maze {
			constexpr char left{ 'L' };
			constexpr char right{ 'R' };
			constexpr char up{ 'U' };
			constexpr char down{ 'D' };
			constexpr char null{ 'N' };

			struct step {
				size_t row;
				size_t col;
				char direction;
				void next_direction();
				void walk();
				step peek();
				bool operator==(step& st) {
					if (this->row == st.row&&this->col == st.col&&this->direction == st.direction) {
						return true;
					}
					return false;
				}
				friend std::ostream& operator<<(std::ostream& ost, const step& step);
			};
		}
	}
}
