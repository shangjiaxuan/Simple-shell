#pragma once

#include "../UJr2.h"

namespace UJr2_funcs {
	namespace block_search {
		extern bool on;
		struct index_block {
			int* data{nullptr};
			size_t start{0};
			size_t block_size{0};
			int max = std::numeric_limits<int>::lowest();
			int& operator[](size_t i) const {
				if (!(i < block_size)) {
					throw std::runtime_error("int& index_block::operator[](size_t): index too large!");
				}
				return data[start + i];
			}
		};

		class search {
		public:
			search();
			~search();
			bool on{ true };
			static void name_ver();
			size_t block_num;
			index_block* index;
			int* data;
			void print(std::ostream& ost) const;
			size_t search_index(int code) const;
			long long search_block(size_t block, int code) const;
			struct coordinate {
				size_t block{0};
				long long place{0};
			};
			coordinate search_code(int code) const;
			static void print_result(std::ostream& ost, const coordinate& cor);
			void UI();
			void search_cmd() const;
		};

		std::string parse_path(std::string line);
	}
}