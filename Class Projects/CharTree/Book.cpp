#include "Book.h"

using namespace std;

void Book::init_ntoken() {
	size_t size = default_non_tokens.size();
	for (size_t i = 0; i < size; i++) {
		index.add_token(default_non_tokens[i])->list.add(-1);
	}
}

void Book::reindex(list::found original, volume* new_book) {
	int new_index = booklist.reindex(original, new_book);
	if (original.name == "") {
		throw runtime_error("Book::reindex: Empty bookname!");
	}
	vector<string> tokens = get_tokens(original.name);
	size_t size = tokens.size();
	for (size_t i = 0; i<size; i++) {
		node* token_loc = index.locate(tokens[i]);
		token_loc->list.add(new_index);
	}
}

/*
volume* Book::reindex(list::found original) {
const int oindex = original.rtn->index_number;
volume* traceback= original.rtn;
volume* cur = traceback->volume_next;
int index = original.rtn->index_number + 1;
while(cur->fixed_index&&index==cur->index_number) {
traceback = cur;
cur = cur->volume_next;
index++;
if(!cur) {
break;
}
}
original.traceback = original.rtn->volume_next;
original.rtn->index_number = index;
original.rtn->volume_next = cur;
traceback->volume_next = original.rtn;
//找出所有关键词，用关键词访问书目链表，重命名所有oindex为index
vector<string> tokens = get_tokens(original.name);
int s = tokens.size();
for(int i=0;i<s;i++) {
if(item* temp = this->index.locate(tokens[i])->head) {
while (temp->index_number != oindex) {
if (!temp->next_item) {
throw runtime_error("Book::reindex: cannot find original index in a certain token");
}
temp = temp->next_item;
}
temp->index_number = index;
}else {
throw runtime_error("Book::reindex: token not found!");
}
}
return original.traceback;
}
*/

void Book::add(istream& ist) {
	string start;
	int index;
	while (!ist.eof()) {
		ist >> start;
		istringstream convert(start);
		if (convert >> index) {
			char id;
			string bookname;
			ist >> id;
			char c;
			ist.get(c);
			while (c == ' ') {
				ist.get(c);
			}
			ist.putback(c);
			getline(ist, bookname);
			switch (id) {
			case 't':
				add(index, bookname);
				break;
			case 'f':
				add(bookname);
				break;
			default:
				throw runtime_error("Book::add: unknown fixed status!");
			}
			//			ist.ignore(numeric_limits<streamsize>::max(), '\n');
		}
		else {
			string following;
			getline(ist, following);
			start += following;
			add(start);
		}
	}
}

void Book::add(string name) {
	if(name=="") {
		throw runtime_error("Book::add: no bookname specified!");
	}
	To_standard(name);
	int index = booklist.add(name);
	add_book_tree(name, index);
}

void Book::add(int added_index, std::string name) {
	if (name == "") {
		throw runtime_error("Book::add: no bookname specified!");
	}
	To_standard(name);
	list::found found = booklist.find(added_index);
	int new_index = booklist.add(added_index, name);
	if (found.status == 'P') {
		vector<string> tokens = get_tokens(found.name);
		size_t size = tokens.size();
		for(size_t i=0; i<size; i++) {
			node* token = index.locate(tokens[i]);
			token->list.del(added_index);
			token->list.add(new_index);
		}
	}
	add_book_tree(name, added_index);
}

/*bool Book::add(std::string name) {
To_standard(name);
list::found temp = booklist.find(name);
switch (temp.status) {
case 'P':
return false;
case 'N':
booklist.add_head(name);
add_book_tree(name, 0);
return true;
case 'X':
{
if (booklist.head->index_number > 0) {
booklist.change_head(0, name);
add_book_tree(name, 0);
return true;
}
int index = 0;
volume* xconti = booklist.head;
volume* track = nullptr;
while (xconti->index_number == index&&xconti->volume_next) {
track = xconti;
xconti = xconti->volume_next;
index++;
}
if(track==nullptr) {
booklist.add_head(name);
}
else {
booklist.add(track, index, name, false);
}
{vector<string> tokens = get_tokens(name);
int size = tokens.size();
for (int i = 0; i < size; i++) {
this->index.add_token(tokens[i])->add(index);
}
}
return true;
}
default:
{
throw std::runtime_error("Book::add: Unkown found status!");
}
}
}*/

/*bool Book::add(int index, std::string name) {
To_standard(name);
list::found temp = booklist.find(index);
switch(temp.status) {
case 'P':
{
if (temp.rtn->fixed_index) {
if (temp.rtn->name == name) {
vector<string> tokens = get_tokens(name);
int size = tokens.size();
for(int i=0; i<size; i++) {
this->index.add_token(tokens[i])->add(index);
}
return true;
}
return false;
}
else {
booklist.add(reindex(temp), index, name, true);
vector<string> tokens = get_tokens(name);
int size = tokens.size();
for (int i = 0; i<size; i++) {
this->index.add_token(tokens[i])->add(index);
}
return true;
}
}
case 'N':
{
booklist.head = new volume;
booklist.head->name = name;
booklist.head->fixed_index = true;
booklist.head->index_number = index;
booklist.head->volume_next = nullptr;
}
return true;
case 'S':
{
volume* newh = new volume;
newh->name = name;
newh->index_number = index;
newh->fixed_index = true;
newh->volume_next = booklist.head->volume_next;
booklist.head = newh;
return true;
}
case 'L':
case 'M':
{
booklist.add(temp.rtn, index, name, true);
return true;
}
default:
{
throw runtime_error("add(int): Unkown found status!");
}
}
}*/

bool Book::del(int deleted_index) {
	try {
		string name = booklist.del(deleted_index);
		del_book_tree(name, deleted_index);
	}
	catch (exception& e) {
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
		int index = booklist.del(name);
		del_book_tree(name, index);
	}
	catch (exception& e) {
		cerr << e.what() << endl;
		return  false;
	}
	return true;
	/*
	list::found temp = booklist.find(name);
	switch (temp.status) {
	case 'P':
	if(temp.traceback) {
	temp.traceback = temp.rtn->volume_next;
	}
	temp.rtn->~volume();
	{vector<string> tokens = get_tokens(name);
	int size = tokens.size();
	for(int i=0; i<size; i++) {
	index.del_token(tokens[i]);
	}
	}
	temp.rtn = nullptr;
	return true;
	case 'N': case 'X':
	return false;
	default:
	throw runtime_error("del(string): Unknown status!");
	}
	*/
}


//有大写又有小写的规范很复杂，要考虑词的性质和长度，还有“I”一类的词，干脆英文全大写（尽管完全和词相关，和位置关系不大（除了第一个词））
void Book::To_standard(std::string& bookname) {
	const size_t size = bookname.size();
	for (size_t i = 0; i<size; i++) {
		if (bookname[i]>96 && bookname[i]<123) {
			bookname[i] -= 32;
		}
	}
}

std::vector<std::string> Book::get_tokens(std::string bookname) {
	vector<string> rtn;
	string current;
	istringstream str(bookname);
	while (str >> current) {
		if (istoken(current)) {
			rtn.push_back(current);
		}
	}
	return rtn;
}

bool Book::istoken(std::string token) {
	node* temp = index.locate(token);
	if (!temp) {
		return true;
	}
	if (!temp->list) {
		return true;
	}
	if (temp->list.head->index_number == -1) {
		return false;
	}
	return true;
}

void Book::load() {
	index_input.open("index.dat");
	if (!index_input) {
		return;
	}
	index.load(index_input);
	index_input.close();
	booklist_input.open("booklist.dat");
	if (!booklist_input) {
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
	index_output.open("index.dat");
	if (!index_output) {
		throw runtime_error("Book::save: cannot open 'index.dat' for writing!");
	}
	index.save(index_output);
	index_output.close();
	booklist_output.open("booklist.dat");
	if (!booklist_output) {
		throw runtime_error("Book::save: cannot open 'booklist.dat' for writing!");
	}
	booklist.save(booklist_output);
	/*	if(booklist.head) {
	volume* current = booklist.head;
	while(true) {
	booklist_output << current->index_number << ' ';
	if(current->fixed_index) {
	booklist_output << "t ";
	}else {
	booklist_output << "f ";
	}
	booklist_output << current->name;
	if(!current->volume_next) {
	break;
	}
	else {
	booklist_output << '\n';
	}
	current = current->volume_next;
	}
	}*/
	booklist_output.close();
}

void Book::add_book_tree(std::string bookname, int book_index) {
	vector<string> tokens = get_tokens(bookname);
	size_t size = tokens.size();
	for (size_t i = 0; i<size; i++) {
		index.add_token(tokens[i])->list.add(book_index);
	}
}

void Book::del_book_tree(std::string bookname, int book_index) {
	vector<string> tokens = get_tokens(bookname);
	size_t size = tokens.size();
	for (size_t i = 0; i<size; i++) {
		node* node = index.locate(tokens[i]);
		if (!node->list || ((!node->list.head->next_item) && node->list.head->index_number == book_index)) {
			index.del_token(tokens[i]);
			continue;
		}
		node->list.del(book_index);
	}
}


void Book::print_booklist(std::ostream& ost) {
	booklist.print(ost);
}

void Book::print_index(std::ostream& ost) {
	index.print_tokens(ost);
}

void Book::ntoken(std::string token) {
	To_standard(token);
	node* loc = index.locate(token);
	if (loc) {
		delete loc->list.head;
		loc->list.head = nullptr;
		loc->list.add(-1);
		return;
	}
	index.add_token(token)->list.add(-1);
}

void list::print_book(int index, std::ostream& ost) {
	found found = find(index);
	if (found.status != 'P') {
		throw runtime_error("list::print_book: index not found!");
	}
	print_book(found.rtn, ost);
}

void list::print_book(std::string name, std::ostream& ost) {
	found found = find(name);
	if(found.status!='P') {
		throw runtime_error("list::print_book: name not found!");
	}
	print_book(found.rtn, ost);
}

void list::print_book(volume* book, std::ostream& ost) {
	if (book->fixed_index) {
		ost << "index:\t" << book->index_number << '\n';
	}
	ost << "name:\t" << book->name << endl;
}

void Book::print_token(std::string token, std::ostream& ost) {
	//必须这么写，否则会被析构
	index_list& list = index.access(token);
//	item* list_head = index.access(token).head;
	if(!list) {
		throw runtime_error("Book::print_token: book list with given token is empty");
	}
	ost << "Books with token \"" << token << "\":\n" << endl;
	item* current = list.head;
	volume* book = booklist.head;
	while(current && book) {
		book = booklist.find(book, current->index_number).rtn;
		if(!book) {
			break;
		}
		booklist.print_book(book, ost);
		current = current->next_item;
		book = book->volume_next;
	}
}

void Book::print_book(std::string name, std::ostream& ost) {
	booklist.print_book(name, ost);
}

void Book::print_book(int index, std::ostream& ost) {
	booklist.print_book(index, ost);
}

