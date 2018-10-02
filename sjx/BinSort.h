#pragma once

#include "UJr2.h"

namespace UJr2_funcs {
	namespace binsort {
		std::string parse_path(std::string line);

		extern bool on;

		struct node {
			int data{};
			long long lchild{ -1 };
			long long rchild{ -1 };
			long long parent{ -1 };
			size_t depth{ 1 };
		};

		class BinSort {
		public:
			BinSort();
			node* data{};
			size_t root{ 0 };
			size_t size{ 0 };

			void init(std::istream& ist);
			void sort(std::istream& ist);
			void add(size_t added);

			struct unbalance {
				bool current;
				bool child;
				long long subtree;
			};

			bool start = true;

			long long find_parent_node(long long root, int data);

			unbalance find_smallest_unbalance(long long start, bool current) const;

			unbalance add_left(size_t subtree, size_t added) const;
			unbalance add_right(size_t subtree, size_t added) const;

			void parse_unbalance(unbalance unb);

			void search_verbose(int index) const;

			void print() const;

			void set_depth(size_t node) const;


			void UI() const;
			void search() const;
			static void name_ver();
		};
	}
}
