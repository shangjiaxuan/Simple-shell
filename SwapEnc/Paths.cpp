#include "Paths.h"

using namespace std;

void iopath_recursive_iterator::pop() {
	if(depth==0) {
		throw range_error("iopath_recursive_iterator::pop(): cannot pop over the root directory for iteration");
	}
	depth--;
	cur_ipath = cur_ipath.parent_path();
	cur_opath = cur_opath.parent_path();
}

iopath_recursive_iterator iopath_recursive_iterator::end() {
	return iopath_recursive_iterator();
}

iopath_recursive_iterator& iopath_recursive_iterator::operator++() {
	if (temp != fs::end(temp)) {
		cur_ipath = temp->path();
		cur_opath = cur_oroot / cur_ipath.filename();
		++temp;
		if (fs::is_directory(cur_ipath)) {
			stack.push(temp);
			cur_oroot = cur_opath;
			temp = fs::directory_iterator(cur_ipath);
			depth++;
		}
		return *this;
	}
	else {
		if(stack.empty()) {
			iroot = fs::path();
			cur_ipath = fs::path();
			return *this;
		}
		else {
			temp = stack.top();
			stack.pop();
			cur_oroot = cur_oroot.parent_path();
			depth--;
			cur_ipath = temp->path();
			cur_opath = cur_oroot / cur_ipath.filename();
			++temp;
			if (fs::is_directory(cur_ipath)) {
				stack.push(temp);
				cur_oroot = cur_opath;
				temp = fs::directory_iterator(cur_ipath);
				depth++;
			}
			return *this;
		}
	}
}