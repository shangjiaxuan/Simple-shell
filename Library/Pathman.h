#pragma once

#include "Header.h"

namespace pathman {

	class iopath_recursive_iterator {
	private:
		fs::path iroot;
		fs::path cur_oroot;
		fs::directory_iterator temp;

		std::stack<fs::directory_iterator> stack;

	public:
		iopath_recursive_iterator() = default;

		iopath_recursive_iterator(fs::path& input_root, fs::path& output_root) {
			iroot = input_root;
			cur_ipath = input_root;
			cur_opath = output_root; //parse_name(input_root.string());
			cur_oroot = cur_opath;
			temp = fs::directory_iterator(iroot);
		}

		unsigned depth{0};
		fs::path cur_ipath;
		fs::path cur_opath;

		void pop() {
			if (depth == 0) {
				throw std::range_error("iopath_recursive_iterator::pop(): cannot pop over the root directory for iteration");
			}
			depth--;
			cur_ipath = cur_ipath.parent_path();
			cur_opath = cur_opath.parent_path();
		};
		static iopath_recursive_iterator end() {
			return iopath_recursive_iterator();
		}
		iopath_recursive_iterator& operator++() {
			//continue iterating over the current directory
			if (temp != fs::end(temp)) {
				cur_ipath = temp->path();
				cur_opath = cur_oroot / cur_ipath.filename();
				++temp;
				//start to iterate over the child directory
				if (is_directory(cur_ipath)) {
					stack.push(temp);
					cur_oroot = cur_opath;
					temp = fs::directory_iterator(cur_ipath);
					depth++;
				}
				return *this;
			}
			//stack is empty, and the iterator reaches the end, the recursive iteration must also end here
			if (stack.empty()) {
				iroot = fs::path();
				cur_ipath = fs::path();
				return *this;
			}
			//iteration over the current directory is finished, return to continue iterating over the parent directory
			temp = stack.top();
			stack.pop();
			cur_oroot = cur_oroot.parent_path();
			depth--;
			cur_ipath = temp->path();
			cur_opath = cur_oroot / cur_ipath.filename();
			++temp;
			//iterate over child directory
			if (is_directory(cur_ipath)) {
				stack.push(temp);
				cur_oroot = cur_opath;
				temp = fs::directory_iterator(cur_ipath);
				depth++;
			}
			return *this;
		};
		bool operator==(const iopath_recursive_iterator& tar) const {
			return ((cur_ipath == tar.cur_ipath) && (iroot == tar.iroot));
		}

		bool operator!=(const iopath_recursive_iterator& tar) const {
			return !(*this == tar);
		}
	};
}
