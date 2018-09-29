#pragma once

#include "Header.h"

class KMP {
public:
	KMP() = default;
	KMP(const char* pat, const char* tar);
	~KMP() {
		delete[] next;
		next = nullptr;
	}
	void init(const char* pat, const char* tar);
	long long find(long long start);
	long long find();
	long long find_nth(size_t n);
private:
	void Pattern(const char* w);
	void Target(const char* t);
	void MakeNext();
	const char* pattern;
	size_t pattern_size;
	const char* target;
	size_t target_size;
//加上字符串大小方便访问
	long long* next;
//以后可以考虑改成const.
};

inline long long KMPMatch(const char* pattern, const char* target) {
	KMP This(pattern, target);
	return This.find();
}

