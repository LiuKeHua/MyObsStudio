#include <iostream>
#include <windows.h>
using namespace std;

#define BUF_SIZE 1025
wchar_t szName[] = L"NameOfMappingObject";    // 共享内存的名字

HANDLE signal_ready;
#define EVENT_HOOK_READY      L"CaptureHook_HookReady"
#define GC_EVENT_FLAGS (EVENT_MODIFY_STATE | SYNCHRONIZE)
struct Node
{
	int test1;
	float test2;
	double test3;
};


int main()
{
	// 创建共享文件句柄
	HANDLE hMapFile = CreateFileMapping(
		INVALID_HANDLE_VALUE,    // 物理文件句柄
		NULL,                    // 默认安全级别
		PAGE_READWRITE,          // 可读可写
		0,                       // 高位文件大小
		BUF_SIZE,                // 地位文件大小
		szName                   // 共享内存名称
	);


	Node * pNode = (Node*) MapViewOfFile(
		hMapFile,            // 共享内存的句柄
		FILE_MAP_ALL_ACCESS, // 可读写许可
		0,
		0,
		sizeof(Node)
	);

	pNode->test1 = 0;
	pNode->test2 = 0.f;
	pNode->test3 = 0.0;

	signal_ready = CreateEventW(NULL, false, false, EVENT_HOOK_READY);

	while (1)
	{
		pNode->test1++;
		pNode->test2 += 0.1;
		pNode->test3 += 0.3;
		Sleep(500);
	}

	UnmapViewOfFile(pNode);
	CloseHandle(hMapFile);
	return 0;
}