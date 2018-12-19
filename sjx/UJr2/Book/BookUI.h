#pragma once

#include "Book.h"

namespace UJr2_funcs {
	namespace book {
		class UI {
		public:
			void UI_main();
		private:
			bool on{true};
			Book base;
			static void prompt();
			static void name_ver();
			void parser(/*std::istream& ist*/);

			void add();
			bool add_file(const std::string& cmd);
			void add_manual(const std::string& cmd);
			void add_stream(std::istream& ist);
			//base function
			void add_string(const std::string& line);

			void del();
			void del_manual(const std::string& cmd);
			bool del_file(const std::string& cmd);
			void del_stream(std::istream& ist);
			//base function
			void del_string(const std::string& line);

			void list();
			void list_book() const;
			void list_book_stream(std::ostream& ost) const;
			void list_index();
			void list_index_stream(std::ostream& ost);

			void find();
			void find_book() const;
			void find_token();

			//adds a string that is not considered to be a token
			static std::string parse_bookname(const std::string& line);
			static std::string parse_path(const std::string& line);

			static void ignore_space(std::istream& ist);
		};
	}
}
