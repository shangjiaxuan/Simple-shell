#include "CrossRiver.h"

using namespace std;
namespace UJr2_funcs {
	int CrossRiver(int argc, char* argv[]) {
		crossriver::cross_river This;
		try {
			This.interaction();
		}
		catch (std::exception& e) {
			std::cerr << e.what() << '\n' << std::endl;
		}
		system("pause");
		return 0;
	}
	namespace crossriver {
		void state::set(int location, bool state) {
			if (state) {
				data = data | (0x01 << location);
			}
			else {
				data = data & (0xff - (0x01 << location));
			}
		}

		void state::invert(int location) {
			data = data ^ (0x01 << location);
		}

		bool state::access(int location) const {
			return (data&(0x01 << location));
		}

		std::ostream& operator<<(std::ostream& ost, const state& source) {
			ost << "step " << source.layer << ":\t\t";
			for (int i = 0; i < ITEM_NO; i++) {
				ost << source.access(i) << '\t';
			}
			return ost;
		}

		bool state::operator==(state st) const {
			return  (this->data == st.data);
		}


		void cross_river::go_back() {
			status = stack.back();
			stack.pop();
			cur_layer = status.layer;
		}

		bool cross_river::is_safe(state state) {
			for (int i = 1; i < ITEM_NO - 1; i++) {
				if (state.access(i) != state.access(0) && state.access(i + 1) != state.access(0)) {
					return false;
				}
			}
			return true;
		}

		bool cross_river::is_safe() const {
			return is_safe(status);
		}

		bool cross_river::done() const {
			return (status.data == 0x0f);
		}

		//true if there exists "okay" new states
		//false if not
		bool cross_river::stash() {
			//可以考虑用static减少赋值次数，简化运算，但会提高代码维护难度，就像之前写的
			//车辆管理里的默认编号计算
			bool rtn{ false };
			state temp = status;
			cur_layer++;
			temp.layer = cur_layer;
			temp.invert(0);
			if (is_safe(temp) && (!visited.exist(temp))) {
				stack.push(temp);
				visited.push(temp);
				rtn = true;
			}
			for (int i = 1; i < ITEM_NO; i++) {
				temp.invert(i);
				if (is_safe(temp) && (!visited.exist(temp))) {
					stack.push(temp);
					visited.push(temp);
					rtn = true;
				}
				temp = status;
				temp.layer = cur_layer;
				temp.invert(0);
			}
			if (!rtn) {
				cur_layer--;
			}
			return rtn;
		}

		void cross_river::cross_verbose(std::ostream& ost) {
			cur_layer = 0;
			status.layer = cur_layer;
			add_record();
			visited.push(status);
			while (!done()) {
			label:
				ost << "Current state:\n";
				ost << status << endl;;
				print_stack(ost);
				if (!stash()) {
					ost << "No legal next state found, popping back stack!\n";
					while (true) {
						del_record();
						go_back();
						if (status.layer == traceback.back().layer + 1) {
							ost << "Popped state is a brother of the current state, visiting...\n";
							ost << "Current state:\n";
							ost << status << endl;
							add_record();
							print_stack(ost);
							break;
						}
						ost << "Current state:\n";
						ost << status << endl;
						ost << "Popped state is the brother of ancestor of the dead state, deleting record again...\n";
						print_stack(ost);
					}
					goto label;
				}
				ost << "Stashed all legal next states.\n";
				print_stack(ost);
				ost << "Popping the last child state.\n";
				go_back();
				add_record();
			}
			ost << "Done!\n";
			print_stack(ost);
		}

		void cross_river::add_record() {
			if (traceback.exist(status)) {
				return;
			}
			traceback.push(status);
		}

		void cross_river::del_record() {
			traceback.pop();
		}

		void cross_river::print_stack(std::ostream& ost) const {
			if (!stack) {
				ost << "The stack is currently empty.\n" << endl;
				return;
			}
			ost << "The following ist the current content of the stack:\n" << endl;
			ost << stack;
			ost << endl;
		}

		void cross_river::print_path(std::ostream& ost) const {
			ost << "The following is the found result for the problem:\n" << endl;
			ost << traceback;
			ost << endl;
		}

		void cross_river::interaction() {
			cross_verbose(cout);
			print_path(cout);
		}
	}
}


