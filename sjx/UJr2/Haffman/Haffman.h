#pragma once

#include "../UJr2.h"

namespace UJr2_funcs {
	namespace haffman {
		struct node {
			node() {
				lchild = -1;
				rchild = -1;
				parent = -1;
				//		leaf = true;
			}
			int lchild;
			int rchild;
			int parent;
			size_t weight{ 0 };
			//	bool leaf;
			void print(std::ostream& ost) const;
		};

		class Haffman {
#define MAX_CHAR 256
#define NUM_NODE 511
		public:
			static void prompt() {
				std::cout << "Simple program for simulating Haffman encoding.\n";
				std::cout << "v0.0.0.0\n" << std::endl;
			}
			Haffman() {
				for (int i = 0; i < NUM_NODE; i++) {
					tree[i] = node();
				}
				init();
			}
			node tree[NUM_NODE];
			unsigned short root{};

			unsigned short find_smallest(short range) const;
			void init();
			void init_tree();
			void print_tree() const;

			void encode(std::istream& input, std::ostream& output) const;
			void decode(std::istream& input, std::ostream& output) const;
			void encode_char(unsigned char ch, std::ostream& output) const;

			void UI();
			static std::string parse_path(const std::string& line);
			void encode() const;
			void decode() const;
			void print_code() const;
			bool on{ true };
		};
		extern bool on;
	}
}