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
			cur_opath = output_root;//parse_name(input_root.string());
			cur_oroot = cur_opath;
			temp = fs::directory_iterator(iroot);
		}

		unsigned depth{ 0 };
		fs::path cur_ipath;
		fs::path cur_opath;

		void pop();
		static iopath_recursive_iterator end();
		iopath_recursive_iterator& operator++();

		bool operator==(const iopath_recursive_iterator& tar) const {
			return((cur_ipath == tar.cur_ipath) && (iroot == tar.iroot));
		}

		bool operator!=(const iopath_recursive_iterator& tar) const {
			return !(*this == tar);
		}
	};
}
