#include <iostream>

/*
1）联合体就是一个结构
2）联合体的所有成员相对于基地址的偏移量为0
3）此结构空间要大到总够容纳最“宽”的成员
4）并且，其对其方式要适合于联合体中所有类型的成员
*/
union MyUnion 
{
	struct { int x, y; }s;
	int x, y;
};

int main(int argc, char**argv)
{
	int a = sizeof(int);
	int b = sizeof(double);
	int c = sizeof(long long);

	union MyUnion data;
	data.x = 1;
	data.y = 2;

	data.s.x = data.x*data.x;
	data.s.y = data.y + data.y;

	std::cout << data.x << " , " << data.y << " , " << data.s.x << " , " << data.s.y << std::endl;


}


