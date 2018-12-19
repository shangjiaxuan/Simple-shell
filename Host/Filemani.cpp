#include "Filemani.h"
#include "Launch.h"

using namespace std;

namespace fileman {
	void Change_directory(const nstring& dir) {
		try {
			std::experimental::filesystem::current_path(dir);
		} catch(std::experimental::filesystem::filesystem_error& e) {
			std::cerr << e.what() << '\n' << std::endl;
			throw std::runtime_error("Error: Cannot change into that working directory!\n");
		}
	}

	bool isexecutable(nstring& str) {
#ifdef _WIN32
		if(str.back() == _T('"') && str.length() >= 6 && str.substr(str.length() - 5, 4) != Exec) { return false; }
		if(str.back() != _T('"') && str.length() >= 4 && str.substr(str.length() - 4, 4) != Exec) { return false; }
		//see if the file starts with a "MZ"(4D5A)
		return PELaunch::DOS_magic_number(str);
#endif
		return false;
	}

#ifdef _WIN32
	bool isshelllink(nstring& str) {
		if(str.length() < 4) return false;
		if(str.back() == _T('"') && str.length() >= 6 && str.substr(str.length() - 5, 4) != Link) { return false; }
		if(str.back() != _T('"') && str.length() >= 4 && str.substr(str.length() - 4, 4) != Link) { return false; }
		return true;
	}
#endif

#ifdef _WIN32
	BinaryFileReader& BinaryFileReader::read_WORD(WORD* buffer, std::streamsize num) {
		read(reinterpret_cast<char*>(buffer), num * sizeof WORD);
		return *this;
	}

	BinaryFileReader& BinaryFileReader::read_DWORD(DWORD* buffer, std::streamsize num) {
		read(reinterpret_cast<char*>(buffer), num * sizeof DWORD);
		return *this;
	};

	BinaryFileReader& BinaryFileReader::read_LONG(LONG* buffer, std::streamsize num) {
		read(reinterpret_cast<char*>(buffer), num * sizeof LONG);
		return *this;
	};
#endif


}
