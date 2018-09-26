#include "Header.h"

using namespace std;

BinaryFileReader& BinaryFileReader::read_WORD(WORD* buffer, std::streamsize num){
read(reinterpret_cast<char*>(buffer), num * sizeof WORD);
	return *this;
}

BinaryFileReader& BinaryFileReader::read_DWORD(DWORD* buffer, std::streamsize num) {
	read(reinterpret_cast<char*>(buffer), num * sizeof DWORD);
	return *this;
};
BinaryFileReader& BinaryFileReader::read_LONG(LONG* buffer, std::streamsize num) {
	read(reinterpret_cast<char*>(buffer), num * sizeof LONG);
	return *this;
};

