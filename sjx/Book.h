#pragma once

#include "../Lib/myDataStructures.h"
#include "Booklist.h"

namespace UJr2_funcs {
	namespace book {

		class UI;

		class BookIndex : public CharTree<char, sorted_index_list<int>> {
		protected:
			void print_useful_content(node* current, std::ostream& ost, const std::string& token) override {
				if(current->data.front() >= 0) {
					ost << token << ":\t";
					ost << current->data;
					ost << std::endl;
				}
			}
		};

		//管理所有书目的链表
		//貌似应该再加一个保存到文件的功能，就叫“index.dat”和“booklist.dat”罢
		//这样就不需要非token的vector了（手动输入一次，保存）
		//最好加上取消非token的功能
		//但这样就要写一个遍历的函数，查找所有含有这个新token的书目，添加
		//最好把书和next指针分开封装，这样可以让CharTree的节点单链表也含有书名信息，方便访问（不查找，不一定要用find(index).name）
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
			//用于find函数的返回值
			//这个函数不安全，放在private里面
			//	void add(volume* previous, int index, std::string name, bool index_type);
			//reassigns a default index_number and returns the volume before the index_number
			void reindex(list::found original, volume* new_book);
			static void To_standard(std::string& bookname);
			//为了加速，让非token对应的CharTree节点的index头字符串对应一本编号
			//“-1”的书
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
