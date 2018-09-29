#include "BinSort.h"

using namespace std;


//实际上可以一边输入一边生成，但这里用了先输入后排序的方法
BinSort::BinSort() {
	string cmd;
	cout << "Please specify the indexes you wish to sort:\n";
	getline(cin, cmd);
	istringstream iss0(cmd);
	int num;
	if (iss0 >> num) {
		istringstream iss1(cmd);
		istringstream iss2(cmd);
		init(iss1);
		sort(iss2);
	}
	else {
		cmd = parse_path(cmd);
		ifstream ifs;
		ifs.open(cmd);
		if (!ifs) {
			throw runtime_error("BinSort::BinSort(): Cannot load specified indexes!");
		}
		init(ifs);
		ifs.close();
		ifs.open(cmd);
		if (!ifs) {
			throw runtime_error("BinSort::BinSort(): Cannot reopen secified file!");
		}
		sort(ifs);
		ifs.close();
	}
	cout << endl;
	cout << "Done sorting the indexes.!\n" <<endl;
}

void BinSort::init(istream& ist) {
	int temp;
	while (ist >> temp) {
		size++;
	}
	data = new node[size];
};

void BinSort::sort(istream& ist) {
	int num;
	size_t i{ 0 };
	while (ist >> num) {
		data[i].data = num;
		add(i);
		i++;
	}
}

void BinSort::add(size_t added) {
	long long root=find_parent_node(this->root, data[added].data);
	if (root < 0) {
		return;
	}
	if (data[added].data < data[root].data) {
		unbalance unb = add_left(root, added);
		if (unb.subtree<0) {
			return;
		}
		else {
			parse_unbalance(unb);
		}
	}
	else if (data[added].data >= data[root].data) {
		unbalance unb = add_right(root, added);
		if (unb.subtree<0) {
			return;
		}
		else {
			parse_unbalance(unb);
		}
	}
}

long long BinSort::find_parent_node(long long root, int index) {
	size_t trace;
	if (start) {
		start = false;
		return -1;
	}
	while (root >= 0) {
		if (index < data[root].data) {
			trace = root;
			root = data[root].lchild;
			continue;
		}
		if (index >= data[root].data) {
			trace = root;
			root = data[root].rchild;
			continue;
		}
	}
	return trace;
}

BinSort::unbalance BinSort::add_left(size_t subtree, size_t added) {
	data[subtree].lchild = added;
	data[added].parent = subtree;
	data[added].depth = 1;
	return find_smallest_unbalance(subtree,false);

}

BinSort::unbalance BinSort::add_right(size_t subtree, size_t added) {
	data[subtree].rchild = added;
	data[added].parent = subtree;
	data[added].depth = 1;
	return find_smallest_unbalance(subtree,true);
}

BinSort::unbalance BinSort::find_smallest_unbalance(long long start,bool current) {
	unbalance rtn{current,false,start};
	while (start>=0) {
		set_depth(start);
		if (data[start].lchild < 0) {
			if (data[start].rchild < 0) {
				break;
			}
			else if (data[data[start].rchild].depth==1) {
				rtn.child = true;
				if (data[data[start].parent].rchild == start) {
					rtn.current = true;
				}
				else {
					rtn.current = false;
				}
				start = data[start].parent;
				continue;
			}
			else {
				rtn.child = rtn.current;
				rtn.current = true;
				rtn.subtree = start;
				return rtn;
			}
		}
		if (data[start].rchild < 0) {
			if (data[start].lchild < 0) {
				break;
			}
			else if (data[data[start].lchild].depth == 1) {
				rtn.current = false;
				if (data[data[start].parent].rchild == start) {
					rtn.current = true;
				}
				else {
					rtn.current = false;
				}
				start = data[start].parent;
				continue;
			}
			else {
				rtn.child = rtn.current;
				rtn.current = false;
				rtn.subtree = start;
				return rtn;
			}
		}
		if (data[data[start].rchild].depth == data[data[start].lchild].depth) {
			break;
		}
		if (data[data[start].rchild].depth + 1 == data[data[start].lchild].depth) {
			rtn.child = rtn.current;
			if (data[data[start].parent].rchild == start) {
				rtn.current = true;
			}
			else {
				rtn.current = false;
			}
			start = data[start].parent;
			continue;
		}
		if (data[data[start].lchild].depth + 1 == data[data[start].rchild].depth) {
			rtn.child = rtn.current;
			if (data[data[start].parent].rchild == start) {
				rtn.current = true;
			}
			else {
				rtn.current = false;
			}
			start = data[start].parent;
			continue;
		}
		else {
			rtn.subtree = start;
			return rtn;
		}
	}
	return { false,false,-1 };
}

void BinSort::parse_unbalance(unbalance unb) {
	if (unb.subtree < 0) {
		return;
	}
	if (unb.current == false && unb.child == false) {
		if (data[unb.subtree].parent < 0) {
			root = data[unb.subtree].lchild;
			data[data[unb.subtree].lchild].parent = -1;
		}
		else {
			if (data[data[unb.subtree].parent].lchild == unb.subtree) {
				data[data[unb.subtree].parent].lchild= data[unb.subtree].lchild;
				data[data[unb.subtree].lchild].parent = data[unb.subtree].parent;
			}
			else if (data[data[unb.subtree].parent].rchild == unb.subtree) {
				data[data[unb.subtree].parent].rchild = data[unb.subtree].lchild;
				data[data[unb.subtree].lchild].parent = data[unb.subtree].parent;
			}
			else {
				throw runtime_error("BinSort::parse_unbalance(unbalance unb): Child-parent mismatch!");
			}
		}
		data[unb.subtree].parent = data[unb.subtree].lchild;
		data[unb.subtree].lchild = data[data[unb.subtree].parent].rchild;
		data[data[unb.subtree].parent].rchild = unb.subtree;
		set_depth(unb.subtree);
		set_depth(data[unb.subtree].parent);
		while(true) {
			unb = find_smallest_unbalance(data[unb.subtree].parent, false);
			if (unb.subtree < 0) {
				break;
			}
			parse_unbalance(unb);
		}
	}
	else if (unb.current == true && unb.child == true) {
		if (data[unb.subtree].parent < 0) {
			root = data[unb.subtree].rchild;
			data[data[unb.subtree].rchild].parent = -1;
		}
		else {
			if (data[data[unb.subtree].parent].lchild == unb.subtree) {
				data[data[unb.subtree].parent].lchild = data[unb.subtree].rchild;
				data[data[unb.subtree].rchild].parent = data[unb.subtree].parent;
			}
			else if (data[data[unb.subtree].parent].rchild == unb.subtree) {
				data[data[unb.subtree].parent].rchild = data[unb.subtree].rchild;
				data[data[unb.subtree].rchild].parent = data[unb.subtree].parent;
			}
			else {
				throw runtime_error("BinSort::parse_unbalance(unbalance unb): Child-parent mismatch!");
			}
		}
		data[unb.subtree].parent = data[unb.subtree].rchild;
		data[unb.subtree].rchild = data[data[unb.subtree].parent].lchild;
		data[data[unb.subtree].parent].lchild = unb.subtree;
		set_depth(unb.subtree);
		set_depth(data[unb.subtree].parent);
		while (true) {
			unb = find_smallest_unbalance(data[unb.subtree].parent, true);
			if (unb.subtree < 0) {
				break;
			}
			parse_unbalance(unb);
		}
	}
	else if (unb.current == false && unb.child == true) {
		if (data[unb.subtree].parent < 0) {
			root = data[data[unb.subtree].lchild].rchild;
			data[data[data[unb.subtree].lchild].rchild].parent = -1;
		}
		else {
			if (data[data[unb.subtree].parent].lchild == unb.subtree) {
				data[data[unb.subtree].parent].lchild = data[data[unb.subtree].lchild].rchild;
				data[data[data[unb.subtree].lchild].rchild].parent = data[unb.subtree].parent;
			}
			else if (data[data[unb.subtree].parent].rchild == unb.subtree) {
				data[data[unb.subtree].parent].rchild = data[data[unb.subtree].lchild].rchild;
				data[data[data[unb.subtree].lchild].rchild].parent = data[unb.subtree].parent;
			}
			else {
				throw runtime_error("BinSort::parse_unbalance(unbalance unb): Child-parent mismatch!");
			}
		}
		data[unb.subtree].parent = data[data[unb.subtree].lchild].rchild;
		data[data[unb.subtree].lchild].parent = data[data[unb.subtree].lchild].rchild;
		data[data[unb.subtree].lchild].rchild = data[data[unb.subtree].parent].lchild;
		data[data[unb.subtree].parent].lchild = data[unb.subtree].lchild;
		data[unb.subtree].lchild = data[data[unb.subtree].parent].rchild;
		data[data[unb.subtree].parent].rchild = unb.subtree;

		set_depth(unb.subtree);
		set_depth(data[data[unb.subtree].parent].lchild);
		set_depth(data[unb.subtree].parent);

		while (true) {
			unb = find_smallest_unbalance(data[data[unb.subtree].parent].lchild, false);
			if (unb.subtree < 0) {
				break;
			}
			parse_unbalance(unb);
		}
	}
	else if (unb.current == true && unb.child == false) {
		if (data[unb.subtree].parent < 0) {
			root = data[data[unb.subtree].rchild].lchild;
			data[data[data[unb.subtree].rchild].lchild].parent = -1;
		}
		else {
			if (data[data[unb.subtree].parent].lchild == unb.subtree) {
				data[data[unb.subtree].parent].lchild = data[data[unb.subtree].rchild].lchild;
				data[data[data[unb.subtree].rchild].lchild].parent = data[unb.subtree].parent;
			}
			else if (data[data[unb.subtree].parent].rchild == unb.subtree) {
				data[data[unb.subtree].parent].rchild = data[data[unb.subtree].rchild].lchild;
				data[data[data[unb.subtree].rchild].lchild].parent = data[unb.subtree].parent;
			}
			else {
				throw runtime_error("BinSort::parse_unbalance(unbalance unb): Child-parent mismatch!");
			}
		}
		data[unb.subtree].parent = data[data[unb.subtree].rchild].lchild;
		data[data[unb.subtree].rchild].parent = data[data[unb.subtree].rchild].lchild;
		data[data[unb.subtree].rchild].lchild = data[data[unb.subtree].parent].rchild;
		data[data[unb.subtree].parent].rchild = data[unb.subtree].rchild;
		data[unb.subtree].rchild = data[data[unb.subtree].parent].lchild;
		data[data[unb.subtree].parent].lchild = unb.subtree;

		set_depth(unb.subtree);
		set_depth(data[data[unb.subtree].parent].rchild);
		set_depth(data[unb.subtree].parent);

		while (true) {
			unb = find_smallest_unbalance(data[unb.subtree].parent, false);
			if (unb.subtree < 0) {
				break;
			}
			parse_unbalance(unb);
		}
	}
}

void BinSort::set_depth(size_t node) {
	if (data[node].lchild < 0) {
		if (data[node].rchild < 0) {
			data[node].depth = 1;
		}
		else {
			data[node].depth = data[data[node].rchild].depth + 1;
		}
	}
	else {
		if (data[node].rchild < 0) {
			data[node].depth = data[data[node].lchild].depth + 1;
		}
		else {
			if (data[data[node].lchild].depth >= data[data[node].rchild].depth) {
				data[node].depth = data[data[node].lchild].depth + 1;
			}
			else {
				data[node].depth = data[data[node].rchild].depth + 1;
			}
		}
	}
}


