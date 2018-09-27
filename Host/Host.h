
#pragma once

#include "Header.h"

class UI {
public:

	void loop(int argc, char* argv[]) const;

	//for making vector of parsed arguments
	//and then pass on to the command parser
	static std::vector<nstring> Get_input(std::istream& input_stream);

private:

	//prompt shown with each input of command
	//shows the path of current working directory
	static void prompt();

	//a switch for showing the welcome message again
	bool go_to_beginning = false;

	//following are functions for parsing the direct input into strict commands

	//for pasring a single argument
	static nstring parse_input(std::istream& input_stream);

	//certain characters not on the keyboard are supported on console
	//with the character "\" (eg '\a')
	//!!!currently not incorporated
	static bool change(const char& a);
};

//a struct for passing commandline arguments
#ifdef _UNICODE
struct cmdline {
	size_t argc = 0;
	char** argv = nullptr;
	cmdline() = default;
	cmdline(const cmdline&) = default;
	cmdline(cmdline&&) = default;
	cmdline& operator=(const cmdline&) = default;
	cmdline& operator=(cmdline&&) = default;
	~cmdline() {
		for(size_t i = 0; i < argc; i++) {
			delete[] argv[i];
			argv[i] = nullptr;
		}
		delete[] argv;
	}
};
#endif

typedef struct ncmdline {
	size_t argc = 0;
	nchar** argv = nullptr;
	ncmdline() = default;
	ncmdline(const ncmdline&) = default;
	ncmdline(ncmdline&&) = default;
	ncmdline& operator=(const ncmdline&) = default;
	ncmdline& operator=(ncmdline&&) = default;

	~ncmdline() {
		for(size_t i = 0; i < argc; i++) {
			delete[] argv[i];
			argv[i] = nullptr;
		}
		delete[] argv;
	}
} ncmdline;
