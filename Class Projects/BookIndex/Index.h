#pragma once

#define MaxTokens 1024

#include "Book.h"

using namespace std;

struct item {
	int index;
	item* next;
};

struct Token {
	string word;
	item* head;
};

class Index{
public:
	Index(int size) {
		wordlist = new Token[size];
		wordlist_size = size;
		alphabet = new Token[27];			//26个字母加其他
	};
	~Index() {
		delete[] wordlist;
		wordlist = nullptr;
		delete[] alphabet;
		alphabet = nullptr;
	};
	Token* wordlist;
	//线性表存储关键词
	Token* alphabet;
	//对26个英文字母的开始进行索引
	int wordlist_size;
	friend Book;
};

//发现理论上顺序表并不能很好地提供快速访问的功能，速度上并不会比链表快（同量级），因为都需要逐个比较查找关键词
//实际上无法发挥顺序表随机存取的优势反而只有插入删除繁复的缺点明显
//可能比较好的方法是用树作索引
//这几天先查查资料。。。


