#include "Book.h"

using namespace std;

namespace UJr2_funcs {
	namespace book {
		void Book::init_nontoken() {
			const size_t size = default_non_tokens.size();
			for(size_t i = 0; i < size; i++) {
				index.add_token(default_non_tokens[i])->data.add(-1);
			}
		}

		void Book::reindex(list::found original, volume* new_book) {
			const int new_index = booklist.reindex(original, new_book);
			if(original.name.empty()) {
				throw runtime_error("Book::reindex: Empty bookname!");
			}
			vector<string> tokens = get_tokens(original.name);
			const size_t size = tokens.size();
			for(size_t i = 0; i < size; i++) {
				CharTree<sorted_index_list<int>>::node* token_loc = index.locate(tokens[i]);
				token_loc->data.add(new_index);
			}
		}

		void Book::add(istream& ist) {
			string start;
			int index;
			while(!ist.eof()) {
				ist >> start;
				istringstream convert(start);
				if(convert >> index) {
					char id;
					string bookname;
					ist >> id;
					char c;
					ist.get(c);
					while(c == ' ') {
						ist.get(c);
					}
					ist.putback(c);
					getline(ist, bookname);
					switch(id) {
						case 't':
							add(index, bookname);
							break;
						case 'f':
							add(bookname);
							break;
						default:
							throw runtime_error("Book::add: unknown fixed status!");
					}
				} else {
					string following;
					getline(ist, following);
					start += following;
					add(start);
				}
			}
		}

		void Book::add(std::string name) {
			if(name.empty()) {
				throw runtime_error("Book::add: no bookname specified!");
			}
			To_standard(name);
			const int index = booklist.add(name);
			add_book_tree(name, index);
		}

		void Book::add(int added_index, std::string name) {
			if(name.empty()) {
				throw runtime_error("Book::add: no bookname specified!");
			}
			To_standard(name);
			const list::found found = booklist.find(added_index);
			const int new_index = booklist.add(added_index, name);
			if(found.status == 'P') {
				vector<string> tokens = get_tokens(found.name);
				const size_t size = tokens.size();
				for(size_t i = 0; i < size; i++) {
					CharTree<sorted_index_list<int>>::node* token = index.locate(tokens[i]);
					token->data.del(added_index);
					token->data.add(new_index);
				}
			}
			add_book_tree(name, added_index);
		}

		bool Book::del(int deleted_index) {
			try {
				const string name = booklist.del(deleted_index);
				del_book_tree(name, deleted_index);
			} catch(exception& e) {
				cerr << e.what() << endl;
				return false;
			}
			return true;
			/*	list::found temp = booklist.find(deleted_index);
			switch(temp.status) {
			case 'P':

			if (temp.traceback) {
			temp.traceback = temp.rtn->volume_next;
			temp.rtn->volume_next = nullptr;
			delete temp.rtn;
			}
			else {
			delete temp.rtn;
			booklist.head = nullptr;
			}
			{string name = temp.name;
			vector<string> tokens = get_tokens(name);
			int size = tokens.size();
			for (int i = 0; i<size; i++) {
			index.del_token(tokens[i]);
			}
			}
			return true;
			case 'N': case 'L': case 'M': case 'S':
			return false;
			default:
			throw runtime_error("del(int): Unknown status!");
			}
			*/
		}

		bool Book::del(std::string name) {
			To_standard(name);
			try {
				const int index = booklist.del(name);
				del_book_tree(name, index);
			} catch(exception& e) {
				cerr << e.what() << endl;
				return false;
			}
			return true;
		}


		//有大写又有小写的规范很复杂，要考虑词的性质和长度，还有“I”一类的词，干脆英文全大写（尽管完全和词相关，和位置关系不大（除了第一个词））
		void Book::To_standard(std::string& bookname) {
			const size_t size = bookname.size();
			for(size_t i = 0; i < size; i++) {
				if(bookname[i] > 96 && bookname[i] < 123) {
					bookname[i] -= 32;
				}
			}
		}

		std::vector<std::string> Book::get_tokens(const std::string& bookname) {
			vector<string> rtn;
			string current;
			istringstream str(bookname);
			while(str >> current) {
				if(istoken(current)) {
					rtn.push_back(current);
				}
			}
			return rtn;
		}

		bool Book::istoken(const std::string& token) {
			CharTree<sorted_index_list<int>>::node* temp = index.locate(token);
			if(!temp) {
				return true;
			}
			if(!temp->data) {
				return true;
			}
			return (temp->data.front() != -1);
		}

		void Book::load() {
			index_input.open(index_filename);
			if(!index_input) {
				return;
			}
			index.load(index_input);
			index_input.close();
			booklist_input.open(list_filename);
			if(!booklist_input) {
				return;
			}
			booklist.load(booklist_input);
			booklist_input.close();
		}


		//保存现有书目和词索引表的内容到文件
		//书目：以 '\n' 表示书目名称结束
		//字典树（按老师说法貌似是256叉树了。。。）：
		//节点数据内容：以"\0{\0" 和 "\0}\0" 包围书目编号（数字），然后每个数字后用一个空格隔开
		//节点之间的关系按存先根遍历的方法输出到文件，每次到达末端回复到父节点时输出一个空字符表示此逻辑关系
		//先输出每个节点数据（书编号链表和大括号）再输出空字符或者子节点
		//没有书号链表的不输出大括号
		void Book::save() {
			index_output.open(index_filename);
			if(!index_output) {
				throw runtime_error("Book::save: cannot open index save file for writing!");
			}
			index.save(index_output);
			index_output.close();
			booklist_output.open(list_filename);
			if(!booklist_output) {
				throw runtime_error("Book::save: cannot open booklist save file for writing!");
			}
			booklist.save(booklist_output);
			booklist_output.close();
		}

		void Book::add_book_tree(const std::string& bookname, int book_index) {
			vector<string> tokens = get_tokens(bookname);
			const size_t size = tokens.size();
			for(size_t i = 0; i < size; i++) {
				index.add_token(tokens[i])->data.add(book_index);
			}
		}

		void Book::del_book_tree(const std::string& bookname, int book_index) {
			vector<string> tokens = get_tokens(bookname);
			const size_t size = tokens.size();
			for(size_t i = 0; i < size; i++) {
				CharTree<sorted_index_list<int>>::node* node = index.locate(tokens[i]);
				if(!node->data || ((node->data.front() == node->data.back()) && node->data.front() == book_index)) {
					index.del_token(tokens[i]);
					continue;
				}
				node->data.del(book_index);
			}
		}


		void Book::print_booklist(std::ostream& ost) const {
			booklist.print(ost);
		}

		void Book::print_index(std::ostream& ost) {
			index.print_tokens(ost);
		}

		void Book::ntoken(std::string token) {
			To_standard(token);
			CharTree<sorted_index_list<int>>::node* loc = index.locate(token);
			if(loc) {
				loc->data.destroy();
				loc->data.add(-1);
				return;
			}
			index.add_token(token)->data.add(-1);
		}

		void list::print_book(int index, std::ostream& ost) const {
			const found found = find(index);
			if(found.status != 'P') {
				throw runtime_error("list::print_book: index not found!");
			}
			print_book(found.rtn, ost);
		}

		void list::print_book(const std::string& name, std::ostream& ost) const {
			const found found = find(name);
			if(found.status != 'P') {
				throw runtime_error("list::print_book: name not found!");
			}
			print_book(found.rtn, ost);
		}

		void list::print_book(volume* book, std::ostream& ost) {
			if(book->fixed_index) {
				ost << "index:\t" << book->index_number << '\n';
			}
			ost << "name:\t" << book->name << endl;
		}

		void Book::print_token(const std::string& token, std::ostream& ost) {
			//必须这么写，否则会被析构
			linked_list<int>& list = index.access(token);
			//	node* list_head = index.access(token).head;
			if(!list) {
				throw runtime_error("Book::print_token: book list with given token is empty");
			}
			ost << "Books with token \"" << token << "\":\n" << endl;
			list.start_iteration();
			volume* book = booklist.head;
			while(book && !list.iteration_ended()) {
				book = booklist.find(book, list.current()).rtn;
				if(!book) {
					break;
				}
				booklist.print_book(book, ost);
				++list;
				book = book->volume_next;
			}
		}

		void Book::print_book(const std::string& name, std::ostream& ost) const {
			booklist.print_book(name, ost);
		}

		void Book::print_book(int index, std::ostream& ost) const {
			booklist.print_book(index, ost);
		}
	}
}
