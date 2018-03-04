#ifndef _TOKEN_H
#define _TOKEN_H

namespace Calc {
	class Token {
	public:
		char kind;
		double value;
		std::string name;
		Token() {}
		Token(char ch) : kind{ ch } {}
		Token(char ch, double val) : kind{ ch }, value{ val } {}
		Token(char ch, std::string n) : kind{ ch }, name{ n } {}
	};
	constexpr char number{ '6' };
	constexpr char quit{ 'q' };
	constexpr char print{ ';' };
	constexpr char name{ 'N' };
	constexpr char assign{ 'S' };
	constexpr char access{ 'A' };
	constexpr char let{ 'L' };
	constexpr char root2{ 'R' };
	constexpr char space{ 0 };
	const std::string declkey{ "let" };
	const std::string squareroot{ "sqrt" };
}

#endif

