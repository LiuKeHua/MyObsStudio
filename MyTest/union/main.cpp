#include <iostream>

/*
1�����������һ���ṹ
2������������г�Ա����ڻ���ַ��ƫ����Ϊ0
3���˽ṹ�ռ�Ҫ���ܹ���������ĳ�Ա
4�����ң�����䷽ʽҪ�ʺ������������������͵ĳ�Ա
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


