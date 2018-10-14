#pragma once

#include "../Library/Header.h"

#include "CalcVariable.h"

namespace Calc {
	class Token {
	public:
		char kind;
		double value;
		std::string name;

		Token()
			: kind{NULL}, value{NULL} {}

		Token(char ch)
			: kind{ch}, value{0} {}

		Token(char ch, double val)
			: kind{ch}, value{val} {}

		Token(char ch, std::string& n)
			: kind{ch}, value{NULL}, name{n} {}
	};

	constexpr char number{'6'};           //a numeric token
	constexpr char quit{'q'};             //the 'exit' token
	constexpr char print{'\n'};           //the 'print' token
	constexpr char end{';'};              //the seperator token
	constexpr char name{'N'};             //variable name declaration token
	constexpr char assign{'S'};           //token for assigning value for existing variables
	constexpr char access{'A'};           //token for accessing variable's value
	constexpr char let{'L'};              //token for declaration indicator
	constexpr char root2{'R'};            //token for taking square root
	constexpr char space{0};              //token for blank space (reserved)
	const std::string declkey{"let"};     //the declaration key
	const std::string squareroot{"sqrt"}; //the key for ttaking square root

	class Token_stream {
	public:
		Token_stream() { current = nullptr; }

		void init(std::istream&);
		Token get();
		void putback(const Token& t);
		void ignore(char c);

		Token peek(); //new one, not stable or tested with integration (testing)

		std::istream* current;

	private:
		bool full{false};
		Token buffer;

	};

}
