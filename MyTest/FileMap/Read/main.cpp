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


	Node * pNode = (Node*) MapViewOfFile(
		hMapFile,            // �����ڴ�ľ��
		FILE_MAP_ALL_ACCESS, // �ɶ�д���
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