#include "KMP.h"

void KMP::init(const char* pat, const char* tar) {
	Pattern(pat);
	Target(tar);
	MakeNext();
}

KMP::KMP(const char* pat, const char* tar) {
	this->init(pat, tar);
}

void KMP::Pattern(const char* w) {
	pattern = w;
	pattern_size = strlen(w);
}

void KMP::Target(const char* t) {
	target = t;
	target_size = strlen(t);
}

void KMP::MakeNext() {
	next = new long long[pattern_size];
	next[0] = -1;
	size_t This = 0;
	long long Head = -1;
	while (This < pattern_size - 1) {			//计算next[i+1]
		while (Head >= 0 && pattern[This] != pattern[Head]) {
			Head = next[Head];
		}
		This++;
		Head++;
		if (pattern[This] != pattern[Head]) {
			next[This] = Head;
		}
		else {
			next[This] = next[Head];
		}
	}
}

long long KMP::find(long long start) {
	long long tgplc = start;
	long long ptplc = 0;
	while (tgplc < target_size && ptplc < pattern_size) {
		if (pattern[ptplc] == target[tgplc] || ptplc == -1) {
			tgplc++;
			ptplc++;
		}
		else {
			ptplc = next[ptplc];
		}
	}
	if(ptplc == pattern_size) {
		return tgplc - pattern_size;
	}
//	else{//(ptplc == 0) {
		return -1;
//	}
//	throw std::runtime_error("find: Unkown error! (Ended with intermediate pattern place)");
}

long long KMP::find_nth(size_t n) {
	if(n==0) {
		throw std::out_of_range("find(unsigned): has to take positive arguments!");
	}
	long place{ 0 };
	for(size_t i=0; i<n; i++) {
		place = find(place+pattern_size);
		if(place==-1) {
			break;
		}
	}
	return place;
}

long long KMP::find() {
	return find(0);
}
