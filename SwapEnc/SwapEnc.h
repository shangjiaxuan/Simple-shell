#pragma once

#include "Header.h"
#include "Platform.h"

namespace enc {
	class SwapEnc {
	public:
		bool on{ true };
		void prompt();
		void __fastcall run_time();
		void command_line(int argc, char** argv);
	private:
		std::ifstream ifs;
		std::ofstream ofs;
		void name_ver();
		void Exit();
		void __fastcall enc(std::string a);
	};
}

DLL void SwapEnc();
