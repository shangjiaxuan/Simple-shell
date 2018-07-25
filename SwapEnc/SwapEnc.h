#pragma once

#include "Header.h"
#include "Platform.h"

namespace enc {
	class SwapEnc {
	public:
		bool on{ true };
		static void name_ver();
		static void prompt();
		void __fastcall run_time();
		void command_line(int argc, char** argv);
	private:
		std::ifstream ifs;
		std::ofstream ofs;
		void Exit();
		void __fastcall enc(std::string a);
	};
}

DLL void SwapEnc();
