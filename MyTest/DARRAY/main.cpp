#include <iostream>
#include <windows.h>
#include <util/darray.h>

struct MyNode
{
	int * p = nullptr;
	int num = 0;
	char s[10];
};


int main(int argc, char**argv)
{
	DARRAY(struct MyNode)   interleaved_packets;

	MyNode * node = new MyNode;
	da_init(interleaved_packets);
	da_insert(interleaved_packets, 0, node);



	return 0;
}