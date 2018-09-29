#pragma once

#include "Crossing.h"

namespace UJr2_funcs {
	namespace crossing {
		class Color {
		public:
			Color() {
				rel = nullptr;
				group = nullptr;
				color_num = 0;
				throw std::runtime_error("UpColors: Please specify a relation to use this class!");
			}

			Color(crossing::relations& rela) {
				color_num = 0;
				rel = &rela;
				group = new int[crossing::possi](); //用0初始化数组
				//		for (int i = 0; i < crossing::possi; i++) {
				//			group[i] = 0;
				//		}
			}

			~Color() {
				delete[] group;
				group = nullptr;
			}

			void find_groups();
			void print_result();
		protected:
			crossing::relations* rel;
			int color_num;
			int* group;                            //和struct route对应的分组数组
			bool evaluate(int cur, int opp) const; //看某个元素能否加入目前的组
			void assign(int add);                  //给某个元素分配一个组
		};
	}
}
