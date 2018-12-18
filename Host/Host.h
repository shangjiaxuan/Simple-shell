#pragma once

#include "../MyLibrary/Header.h"

template<typename type>
struct cmdline;

class UI {
public:

	void loop(int argc, char* argv[]) const;

	//for making vector of parsed arguments
	//and then pass on to the command parser
	static cmdline<nchar> Get_input(std::istream& input_stream);

private:

	//prompt shown with each input of command
	//shows the path of current working directory
	static void prompt();

	//a switch for showing the welcome message again
	bool go_to_beginning = false;

	//following are functions for parsing the direct input into strict commands

	//for pasring a single argument
	static nchar* parse_input(std::istream& input_stream);

	//certain characters not on the keyboard are supported on console
	//with the character "\" (eg '\a')
	//!!!currently not incorporated
	static bool change(const char& a);
};


//a struct for passing commandline arguments
template<typename type>
struct cmdline {
	size_t argc = 0;
	type** argv = nullptr;
	cmdline() = default;

	cmdline(const cmdline& source) {
		copy(*this, source);
	}

	cmdline(cmdline&& source) noexcept {
		move(*this, source);
	}

	cmdline& operator=(const cmdline& source) {
		copy(*this, source);
		return *this;
	}

	cmdline& operator=(cmdline&& source) noexcept {
		move(*this, source);
		return *this;
	}

	~cmdline() {
		destroy();
	}

	void destroy() {
		for (size_t i = 0; i < argc; i++) {
			delete[] argv[i];
			argv[i] = nullptr;
		}
		delete[] argv;
		argv = nullptr;
		argc = 0;
	}

	static void copy(cmdline& destination, const cmdline& source) {
		destination.argc = source.argc;
		destination.argv = new type*[source.argc];
		for (size_t i = 0; i < source.argc; i++) {
			const size_t arg_length = stringlen(source.argv[i]);
			destination.argv[i] = new type[arg_length + 1];
			stringcpy(destination.argv[i], source.argv[i]);
		}
	}

	static void move(cmdline& destination, cmdline& source) {
		if((&destination)==(&source)) return;
		destination.argc = source.argc;
		destination.argv = source.argv;
		source.argc = 0;
		source.argv = nullptr;
	}
};


template<>
struct cmdline<char> {
	size_t argc = 0;
	char** argv = nullptr;
	cmdline() = default;

	cmdline(const cmdline& source) {
		copy(*this, source);
	}

	cmdline(cmdline&& source) noexcept {
		move(*this, source);
	}

	cmdline& operator=(const cmdline& source) {
		copy(*this, source);
		return *this;
	}

	cmdline& operator=(cmdline&& source) noexcept {
		move(*this, source);
		return *this;
	}

	~cmdline() {
		destroy();
	}

	void destroy() {
		for (size_t i = 0; i < argc; i++) {
			delete[] argv[i];
			argv[i] = nullptr;
		}
		delete[] argv;
		argv = nullptr;
		argc = 0;
	}

	static void copy(cmdline& destination, const cmdline& source) {
		destination.argc = source.argc;
		destination.argv = new char*[source.argc];
		for (size_t i = 0; i < source.argc; i++) {
			const size_t arg_length = strlen(source.argv[i]);
			destination.argv[i] = new char[arg_length + 1];
			strcpy(destination.argv[i], source.argv[i]);
		}
	}

	static void move(cmdline& destination, cmdline& source) {
		if ((&destination) == (&source)) return;
		destination.argc = source.argc;
		destination.argv = source.argv;
		source.argc = 0;
		source.argv = nullptr;
	}
};
