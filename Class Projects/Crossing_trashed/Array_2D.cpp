#include "Header.h"
#include "Array_2D.h"
#include "main.h"
#include "Crossing.h"

template<typename type>
Array_2D<type>::Array_2D(const int& row, const int& col) {
	this->data = new type[row*col];
	this->map = new type*[row];
	for (int i = 0; i < row; i++) {
		this->map[i] = this->data + i * col * sizeof(type);
	}
}

template<typename type>
Array_2D<type>::~Array_2D() {
	delete this->map;
	map = nullptr;
	delete this->data;
	data = nullptr;
}

template<typename type>
type* Array_2D<type>::get(int i, int j) {
	return &map[i][j];
}

template<typename type>
void Array_2D<type>::write(type* pos, type val) {
	*pos = val;
}

template class Array_2D<bool>;

//经过一下午实验，终于做成了c++中连续分配空间的二维数组。事实证明new和delete不够灵活，c++类型转换也比较麻烦
//虽然说最后看着也蛮蠢的。。。



//下面是用Crossing初始化Array_2D派生的路径类的方法，不能再写template了
//先找出所有允许的通行方式，标记
//虽然增加了判定和内存的要求，但这之前是人干的，也算是有改进吧。。。
void routes::init(const Crossing& xing) {
	int i = 0;
	while (i<number) {
		int j = 0;
		while (j<number) {
			if (xing.data[i] <= 0 && xing.data[j] >= 0) {
				valid++;
				this->map[i][j] = true;
			}else { this->map[i][j] = false; }
			j++;
		}
		i++;
	}
}

//然后用这个枚举可以同时存在的路径
//这时其实只要考虑双向都可以的统计方式，再补上是否允许的判定就好

void relations::init(const routes& route) {




}
