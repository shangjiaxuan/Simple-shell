#pragma once

#include "UJr2.h"
#include "../Library/myDataStructures.h"


namespace UJr2_funcs {
	namespace crossriver {
		class state {
		public:
#define ITEM_NO 4
			state() {
				data = '\0';
				layer = 0;
			}
			char data;
			unsigned layer;
			void set(int location, bool state);
			void invert(int location);
			bool access(int location) const;
			friend std::ostream& operator<<(std::ostream& ost, const state& source);
			bool operator==(state st) const;
			bool operator!=(state st) const {
				return !(*this == st);
			}
		};

		class cross_river {
		public:
#define MAX_LENGTH 16
			state status;
			FixedStack<state> visited{ MAX_LENGTH };
			FixedStack<state> traceback{ MAX_LENGTH };
			FixedStack<state> stack{ MAX_LENGTH };
			unsigned cur_layer{ 0 };
			bool stash();
			void go_back();
			bool is_safe() const;
			static bool is_safe(state state);
			void cross_verbose(std::ostream& ost);
			bool done() const;
			void add_record();
			void del_record();
			void print_stack(std::ostream& ost) const;
			void print_path(std::ostream& ost) const;
			void interaction();
		};
	}
}
