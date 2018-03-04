#include <iostream>
#include <stdexcept>

#include "Token_stream.h"
#include "Memory.h"

using namespace std;

namespace Calc {
	void Token_stream::putback(Token t) {
		if(full) {
			throw runtime_error("putback() into a full buffer");
		}
		buffer = t;
		full = true;
	}

	Token Token_stream::get() {
		if(full) {
			full = false;
			return buffer;
		}
		//处理换行，缓存上一个token
		char ch;
		cin.get(ch);
		if(ch == '\n') {
			cin.putback(print);
		} else {
			if(ch != ' ') {
				last_char = {ch};
			}
			cin.putback(ch);
		}
		//处理换行,缓存上一个token
		cin >> ch;
		switch(ch) {
			case quit:
			case print:
			case '(': case ')':
			case '+': case '-':
			case '*': case '/': case '%':
			case '=':
				return Token{ch};
			case '.':
			case '0': case '1': case '2': case '3': case '4':
			case '5': case '6': case '7': case '8': case '9':
				{
					cin.putback(ch);
					double val;
					cin >> val;
					return Token{number,val};
				}
			default:
				if(isalpha(ch)) {
					string s{ch};
					while(cin.get(ch) && (isalpha(ch) > 0 || isdigit(ch) > 0 || ch == '_')) {
						s += ch;
						if(s == squareroot) {
							return Token{root2};
						}
						if(s == declkey) {
							return Token{let};
						}
					}
					cin.putback(ch);
					if(isdecl) {
						return Token{name,s};
					} else {
						char test{0};
						cin.get(test);
						//去空格
						while(test == ' ') {
							cin.get(test);
						}
						//去空格
						//传出赋值变量
						if(test == '=') {
							return Token{assign,s};
						}
							//传出赋值变量
							//传出读取内容
						else {
							cin.putback(test);
							return Token{access,s};
						}
						//传出读取内容
					}
				}
				throw runtime_error("Bad token");
		}
	}


	void Token_stream::ignore(char c)
	// 略过接下来的输入，直到给定的结束符c为止（c一般来说是';'）
	{
		// 如果缓冲区里存的就是结束符，清空缓冲区即可
		if(full && (c == buffer.kind)) {
			full = false;
			return;
		}

		full = false; // 清空缓冲区

		// 搜索结束符
		char ch{0}; // 0代表空字符，是不可能用键盘输入的
		while(cin.get(ch)) {
			if(ch == c || ch == '\n') {
				return;
			}
		}
	}
}
