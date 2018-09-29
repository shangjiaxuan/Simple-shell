#include "Booklist.h"

using namespace std;

list::found list::find(int index) {
	return find(head, index);
/*	static int default_index;
	int cur_index{ 0 };
	if (!head) {
		return { 'N', nullptr , nullptr , "" , 0 };
	}
	if (index < head->index_number) {
		return { 'S' , nullptr, head->volume_next, "", 0 };
	}
	if (index == head->index_number) {
		volume* temp = head;
		while (temp->volume_next != nullptr) {
			if (temp->index_number == cur_index) {
				cur_index++;
			}
			else {
				default_index = cur_index;
			}
			temp = temp->volume_next;
		}
		
		return { 'P' , head, nullptr, head->name, index };
	}
	volume* current = head;
	volume* traceback = nullptr;
	while (index > current->index_number) {
		if(cur_index==current->index_number) {
			cur_index++;
		}else {
			default_index = cur_index;
		}
		if (current->volume_next == nullptr) {
			default_index = cur_index + 1;
			return { 'M' , current, nullptr, "", default_index };
		}
		traceback = current;
		current = current->volume_next;
	}
	if (index == current->index_number) {
		volume* temp = head;
		while (temp->volume_next != nullptr) {
			if (temp->index_number == cur_index) {
				cur_index++;
			}
			else {
				default_index = cur_index;
			}
			temp = temp->volume_next;
		}
		default_index = cur_index + 1;
		return { 'P' , current, traceback, current->name, default_index };
	}
	volume* temp = head;
	while (temp->volume_next != nullptr) {
		if (temp->index_number == cur_index) {
			cur_index++;
		}
		else {
			default_index = cur_index;
		}
		temp = temp->volume_next;
	}
	default_index = cur_index + 1;
	return { 'L' , nullptr, traceback->volume_next, "", default_index };*/
}

list::found list::find(volume* start, int index) {
	static int default_index;
	int cur_index{ 0 };
	if (!start) {
		return { 'N', nullptr , nullptr , "" , 0 };
	}
	if (index < start->index_number) {
		return { 'S' , nullptr, start->volume_next, "", 0 };
	}
	if (start==head&&index == head->index_number) {
		volume* temp = head;
		while (temp->volume_next != nullptr) {
			if (temp->index_number == cur_index) {
				cur_index++;
			}
			else {
				default_index = cur_index;
			}
			temp = temp->volume_next;
		}

		return { 'P' , head, nullptr, head->name, index };
	}
	volume* current = start;
	volume* traceback = nullptr;
	while (index > current->index_number) {
		if (cur_index == current->index_number) {
			cur_index++;
		}
		else {
			default_index = cur_index;
		}
		if (current->volume_next == nullptr) {
			default_index = cur_index + 1;
			return { 'M' , current, nullptr, "", default_index };
		}
		traceback = current;
		current = current->volume_next;
	}
	if (index == current->index_number) {
		volume* temp = head;
		while (temp->volume_next != nullptr) {
			if (temp->index_number == cur_index) {
				cur_index++;
			}
			else {
				default_index = cur_index;
			}
			temp = temp->volume_next;
		}
		default_index = cur_index + 1;
		return { 'P' , current, traceback, current->name, default_index };
	}
	volume* temp = head;
	while (temp->volume_next != nullptr) {
		if (temp->index_number == cur_index) {
			cur_index++;
		}
		else {
			default_index = cur_index;
		}
		temp = temp->volume_next;
	}
	default_index = cur_index + 1;
	return { 'L' , nullptr, traceback->volume_next, "", default_index };
}

list::found list::find(std::string name) {
	if (!head) {
		return { 'N', nullptr , nullptr, "", 0 };
	}
	static int index;
	int cur_index = 0;
	volume* current = head;
	volume* traceback = nullptr;
	static volume* _default;
	if (current->name != name) {
		if(head->index_number>0) {
			_default = nullptr;
			index = 0;
		}
		while (current->volume_next) {
			if (current->index_number == cur_index) {
				cur_index++;
			}
			else {
				_default = traceback;
				index = cur_index;
			}
			traceback = current;
			current = current->volume_next;
			if (current->name == name) {
				break;
			}
		}
	}
	volume* temp = current;
	volume* temp_default = traceback;
	while (temp->volume_next != nullptr) {
		if (temp->index_number == cur_index) {
			cur_index++;
		}
		else {
			_default = temp_default;
			index = cur_index;
		}
		temp_default = temp;
		temp = temp->volume_next;
	}
	_default = temp;
	index = cur_index + 1;
	if (current->name == name) {
		return { 'P' , current, traceback, name, index };
	}
	return { 'X' , _default , nullptr, name, index };
}


int list::add(std::string name) {
	return add(-1, name);
}

int list::add(int index, std::string name) {
	found found;
	bool fixed;
	if (index<0) {
		fixed = false;
		found = find(name);
		index = found.default_index;
	}
	else {
		fixed = true;
		found = find(index);
	}
	switch(found.status) {
	case 'P':
	{
		if (found.rtn->fixed_index) {
			throw runtime_error("list::add: the specified index (or book name if no index specified) exists!");
		}
		volume* added = new volume;
		added->name = name;
		added->index_number = index;
		added->fixed_index = fixed;
		index = reindex(found, added);
		break;
	}
	case 'N':
		add_head(index, name, fixed);
		break;
	case 'S':
		change_head(index, name, fixed);
		break;
	case 'L': case 'M':
		add(found.rtn, index, name, fixed);
		break;
	case 'X':
	{
		volume* added = new volume;
		added->index_number = index;
		added->fixed_index = fixed;
		added->name = name;
		added->volume_next = found.rtn->volume_next;
		found.rtn->volume_next = added;
		break;
	}
	default:
		throw runtime_error("list::add: unknown status!");
	}
	return index;
}


//void list::add_head(int index, std::string name) {
//	add_head(index, name, true);
//}

//void list::add_head(std::string name) {
//	add_head(default_index(), name, false);
//}

void list::add_head(int index, std::string name, bool fixed) {
	head = new volume;
	head->index_number = index;
	head->name = name;
	head->fixed_index = fixed;
	head->volume_next = nullptr;
}

//void list::change_head(int index, std::string name) {
//	change_head(index, name, true);
//}
//
//void list::change_head(std::string name) {
//	change_head(default_index(), name, false);
//}

void list::change_head(int index, std::string name, bool fixed) {
	volume* temp = head;
	head = new volume;
	head->name = name;
	head->index_number = index;
	head->volume_next = temp;
	head->fixed_index = fixed;
}

void list::add(volume* previous, int index, std::string name, bool index_type) {
	volume* current = new volume;
	current->index_number = index;
	current->fixed_index = index_type;
	current->name = name;
	current->volume_next = previous->volume_next;
	previous->volume_next = current;
}

/*
int list::default_index() {
	volume* current = head;
	int index = 0;
	while(current) {
		if(current->index_number>index) {
			return index;
		}
		current = current->volume_next;
		index++;
	}
	return index;
}
*/

volume* list::find_default() {
	volume* traceback = nullptr;
	volume* current = head;
	int index = 0;
	while (current) {
		if (current->index_number>index) {
			return traceback;
		}
		traceback = current;
		current = current->volume_next;
		index++;
	}
	return traceback;
}

int list::reindex(found original, volume* added) {
	if(original.status!='P') {
		throw runtime_error("list::reindex(found): original book not found!");
	}
	if(!original.traceback) {
		if(!original.rtn) {
			throw runtime_error("list::reindex: list cannot be empty!");
		}
		volume* prev = find_default();
		added->volume_next = original.rtn->volume_next;
		head = added;
		original.rtn->index_number = prev->index_number + 1;
		original.rtn->volume_next = prev->volume_next;
		prev->volume_next = original.rtn;
		return original.rtn->index_number;
//		if(added->index_number==0) {
//			added->volume_next = head;
//			head->index_number = 1;
//			head = added;
//			return 1;
//		}
//		else {
//			throw runtime_error("list::reindex: default index for first item is not 0!");
//		}
	}
	original.traceback->volume_next = added;
	added->volume_next = original.rtn->volume_next;
	volume* default_location = find_default();
	if(!default_location) {
		throw runtime_error("list::reindex: list cannot be empty!");
	}
	original.rtn->volume_next = default_location->volume_next;
	default_location->volume_next = original.rtn;
	original.rtn->index_number = default_location->index_number + 1;
	return original.rtn->index_number;
}

int list::del(string name) {
	volume* current = head;
	volume* traceback = nullptr;
	int index;
	while(current) {
		if(current->name==name) {
			index = current->index_number;
			if(!traceback) {
				volume* temp = head->volume_next;
				head->volume_next = nullptr;
				delete head;
				head = temp;
			}
			else {
				traceback->volume_next = current->volume_next;
				current->volume_next = nullptr;
				delete current;
//				current = nullptr;
			}
			return index;
		}
		if(!current->volume_next) {
			throw runtime_error("list::del: bookname not found!");
		}
		current = current->volume_next;
	}
	throw runtime_error("list::del list is empty!");
}

std::string list::del(int index) {
	volume* current = head;
	volume* traceback = nullptr;
	string name;
	while (current) {
		if (current->index_number == index) {
			name = current->name;
			if (!traceback) {
				volume* temp = head->volume_next;
				head->volume_next = nullptr;
				delete head;
				head = temp;
			}
			else {
				traceback->volume_next = current->volume_next;
				current->volume_next = nullptr;
				delete current;
//				current = nullptr;
			}
			return name;
		}
		if (current->index_number>index || !current->volume_next) {
			throw runtime_error("list::del: index not found!");
		}
		current = current->volume_next;
	}
	throw runtime_error("list::del list is empty!");
}

void list::save(std::ofstream& ofs) {
	print(ofs);
/*	if (head) {
		volume* current = head;
		while (true) {
			ofs << current->index_number << ' ';
			if (current->fixed_index) {
				ofs << "t ";
			}
			else {
				ofs << "f ";
			}
			ofs << current->name;
			if (!current->volume_next) {
				break;
			}
			else {
				ofs << '\n';
			}
			current = current->volume_next;
		}
	}*/
}

void list::load(std::ifstream& ifs) {
	string start;
	int index;
	while (!ifs.eof()) {
		ifs >> start;
		istringstream convert(start);
		if (convert >> index) {
			char id;
			string bookname;
			ifs >> id;
			char c;
			ifs.get(c);
			while (c == ' ') {
				ifs.get(c);
			}
			ifs.putback(c);
			getline(ifs, bookname);
			switch (id) {
			case 't':
				add(index, bookname);
				break;
			case 'f':
				add(bookname);
				break;
			default:
				throw runtime_error("list::load: unknown fixed status!");
			}
			//			ist.ignore(numeric_limits<streamsize>::max(), '\n');
		}
//		else {
//			string following;
//			getline(ifs, following);
//			start += following;
//			add(start);
//		}
	}
}

void list::print(std::ostream& ost) {
	if (head) {
		volume* current = head;
		while (true) {
			ost << current->index_number << ' ';
			if (current->fixed_index) {
				ost << "t ";
			}
			else {
				ost << "f ";
			}
			ost << current->name;
			if (!current->volume_next) {
				break;
			}
			else {
				ost << '\n';
			}
			current = current->volume_next;
		}
	}
}

