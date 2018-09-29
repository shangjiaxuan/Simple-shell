//问题：一个十字路口，有单向入，有单向出。将可以不会撞车的出入方式分组，最少组数怎么分

//老师在课上直接人工枚举了路口两两之间的关系。然而这些关系的数量随路口分支的增加不是
//线性增长的，所以想在前面加一个枚举可能性的部分。

//0.一点想法：一个路口的车进入并离开后，有左和右两个参数。左面的左参数只能小于这个路口
//  的左参数只要一个路口允许的枚举完了，下一个路口就不用再从这个路口进车了。

//1.枚举各个路口的同行方式时可以考虑直接用路口枚举，避免直接用关系枚举带来的逻辑繁琐
//  但是貌似会大大增加算法的复杂度。

//2.貌似内存占用这样做会很大，还是要抽象出来

#include "Header.h"
#include "main.h"
#include "Crossing.h"
#include "Array_2D.h"

using namespace std;

int main() {
	int number;
	cout << "Please input the number of roads at the junction:" << endl;
	cin >> number;
	cout << "\nPlease input the parameters for each road. '-1' for one-way in, '1' for one-way out, and '0' for two-way road.\n";
	cout << "Note: Input in a clockwise or counterclockwise orderly manner"<< endl;
	Crossing xing(number);
	cout << "Your input is: ";
	for (int i = 0; i < xing.size; i++) {
		cout <<i<<":"<< *(xing.data + i) <<"  ";
	}
	cout << endl;
	system("pause");
//输入成功的话可以处理数据了，已经被初始化了

//可以这么想，做枚举。首先让一个路口出车到另一个确定的路口，然后枚举可以兼容的通行方式
//一个的枚举完以后，别的路口就不再枚举从这里出的车了，不然会重复
//要注意我们默认是靠右行驶的
//比较直观的存储这样的关系的是一个二维数组
	valid = 0;
	routes route = routes(number, number);
	relations relation = relations(number*number,number*number);

	return 0;
}
