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
		std::string parse_name(std::string ori);
	private:
		std::ifstream ifs;
		std::ofstream ofs;
		void Exit();
		void __fastcall enc(fs::path input, fs::path output);
		std::string parse_pathnames(std::istream& ist);
		void __fastcall encrypt_loop(std::istream & ist);
	};
}

DLL_PORT void SwapEnc();
