#pragma once

#include "../Library/myDataStructures.h"
#include "Booklist.h"

namespace UJr2_funcs {
	namespace book {

		class UI;

		class BookIndex : public CharTree<sorted_index_list<int>> {
		protected:
			void print_useful_content(node* current, std::ostream& ost, const std::string& token) override {
				if(current->data.front() >= 0) {
					ost << token << ":\t";
					ost << current->data;
					ost << std::endl;
				}
			}
		};


		class Book {
		public:
			Book() {
				init_nontoken();
				load();
			}

			//the linked list of books
			list booklist;
			//the index_number tree of tokens
			BookIndex index;
			//if false, the index_number is already assigned as fixed
			//the current book using the index_number is not assigned as fixed, will return true
			//and call the corresponding functions to fix the linked list in the index_number
			//(what really matters)

			friend UI;

			void add(int added_index, std::string name);
			//if false, the name's already taken.
			//this cannot pass const reference because it calls To_standard function
			void add(std::string name);
			void add(std::istream& ist);

			bool del(int deleted_index);
			bool del(std::string name);

			void ntoken(std::string token);

			const char* index_filename{"BookIndex.dat"};
			const char* list_filename{"Booklist.dat"};
			void save();
			void load();

		private:
			//不一定有用
			std::ifstream index_input;
			std::ofstream index_output;
			std::ifstream booklist_input;
			std::ofstream booklist_output;
			//	void add(volume* previous, int index, std::string name, bool index_type);
			//reassigns a default index_number and returns the volume before the index_number
			void reindex(list::found original, volume* new_book);
			static void To_standard(std::string& bookname);
			//non-tokens are stored in the tree with a -1 as booklist
			bool istoken(const std::string& token);
			//vector 本身访问速度很慢，可以考虑其他方法，比如char**，但那样又要动态分配很麻烦
			std::vector<std::string> get_tokens(const std::string& bookname);
			void init_nontoken();
			//先简单地写一个表，肯定不完整，以后用户可以调用函数添加（同时删除原有链表）
			std::vector<std::string> default_non_tokens{"THE", "AM", "IS", "ARE", "OF", "AT", "TO", "UNDER", "ABOVE"};

		protected:
			void add_book_tree(const std::string& bookname, int book_index);
			void del_book_tree(const std::string& bookname, int book_index);

			void print_booklist(std::ostream& ost) const;
			void print_index(std::ostream& ost);

			void print_book(int index, std::ostream& ost) const;
			void print_book(const std::string& name, std::ostream& ost) const;
			void print_token(const std::string& token, std::ostream& ost);
		};
	}
}
