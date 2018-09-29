#include "Header.h"
#include "Crossing.h"
#include "main.h"

status*& Crossing::next(status*& st) {
	if(current<size-1) {				//0开始计数
		current++;
		st++;
		return st;
	}
	if(current==size-1) {
		current = 0;
		st = data;
		return st;
	}
	throw std::runtime_error("Crossing::next: current larger than size!");
}

status*& Crossing::prev(status*& st) {
	if (current>0) {
		current--;
		st--;
		return st;
	}
	if (current == 0) {
		current = size;
		st = data+size;		//从0开始计数
		return st;
	}
	throw std::runtime_error("Crossing::next: current larger than size!");
}

void Crossing::init(Crossing& xing) {
	while(xing.current<xing.size) {
		std::cin >> *(xing.data + xing.current);
		xing.current++;
	}
}





