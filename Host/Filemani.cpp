#include "Header.h"

#include "Filemani.h"

using namespace std;

namespace fileman {
	void Change_directory(nstring dir) {
		try {
			std::experimental::filesystem::current_path(dir);
		} catch(std::experimental::filesystem::filesystem_error& e) {
			std::cerr << e.what() << '\n' << std::endl;
			throw std::runtime_error("Error: Cannot chage into that working directory!\n");
		}
	}

	bool isexecutable(nstring& str) {
#ifdef _WIN32
		if (str.back() == _T('"') && str.length() >= 6 && str.substr(str.length() - 5, 4) != Exec) { return false; }
		if (str.back() != _T('"') && str.length() >= 4 && str.substr(str.length() - 4, 4) != Exec) { return false; }
		//see if the file starts with a "MZ"(4D5A)
		ifstream ifs;
		ifs.open(str);
		char c;
		ifs.get(c);
		if(c == 0x4D) {
			ifs.get(c);
			if(c == 0x5A) {
				ifs.close();
				return true;
			}
		}
		ifs.close();
#endif
		return false;
	}

	bool isshelllink(nstring& str) {
		if (str.length() < 4) return false;
		if (str.back() == _T('"') && str.length() >= 6 && str.substr(str.length() - 5, 4) != Link) { return false; }
		if (str.back() != _T('"') && str.length() >= 4 && str.substr(str.length() - 4, 4) != Link) { return false; }
		return true;
	}

}
