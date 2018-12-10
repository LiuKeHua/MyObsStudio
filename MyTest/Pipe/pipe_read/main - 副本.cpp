#include <iostream>

#ifdef _WIN32
#include <io.h>
#include <fcntl.h>
#include <windows.h>

#define inline __inline

#endif

using namespace std;

int main()
{
	std::cout << "Begin" << std::endl;
	SetErrorMode(SEM_FAILCRITICALERRORS);


	size_t size = sizeof(int);
	int num = 0;
	while (true)
	{
		size_t in_size = fread(&num, 1, size, stdin);
		//std::cout << "Hello: "<<num << std::endl;
		Sleep(100);
	}

	return 0;
}