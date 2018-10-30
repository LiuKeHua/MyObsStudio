
#include <iostream>
#include <windows.h>
using namespace std;

#define BUF_SIZE 1025
wchar_t szName[] = L"NameOfMappingObject";    // �����ڴ������

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
	// ���������ļ����
	HANDLE hMapFile = CreateFileMapping(
		INVALID_HANDLE_VALUE,    // �����ļ����
		NULL,                    // Ĭ�ϰ�ȫ����
		PAGE_READWRITE,          // �ɶ���д
		0,                       // ��λ�ļ���С
		BUF_SIZE,                // ��λ�ļ���С
		szName                   // �����ڴ�����
	);


	Node * pNode =(Node*) MapViewOfFile(
		hMapFile,            // �����ڴ�ľ��
		FILE_MAP_ALL_ACCESS, // �ɶ�д���
		0,
		0,
		BUF_SIZE
	);


	while (1)
	{
		signal_ready = OpenEventW(GC_EVENT_FLAGS, false, EVENT_HOOK_READY);

		if (signal_ready)
		{
			std::cout << pNode->test1 << " , " << pNode->test2 << " , " << pNode->test3 << std::endl;
			Sleep(500);
		}

	}

	UnmapViewOfFile(pNode);
	CloseHandle(hMapFile);
	return 0;
}
