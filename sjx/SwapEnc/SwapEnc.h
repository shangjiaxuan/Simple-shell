#pragma once

#include "../Library/Header.h"

#include "../sjx_Exports.h"

namespace enc {
	class SwapEnc {
	public:
		bool on{true};
		static void name_ver();
		static void prompt();
		void run_time();
		void command_line(int argc, char** argv);
		static std::string parse_name(const std::string& ori);
	private:
		std::ifstream ifs;
		std::ofstream ofs;
		void Exit();
		static std::string __fastcall parse_pathnames(std::istream& ist);
		void __fastcall encrypt_loop(std::istream& ist);
		void __fastcall enc(fs::path input, fs::path output);
	};
}

#ifndef _WINDLL
void SwapEnc();
#endif
