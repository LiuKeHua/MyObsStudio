#include <iostream>
#include <windows.h>


CRITICAL_SECTION       cs;

static int n_AddValue = 0;

DWORD FirstThread(LPVOID lParam)
{
	for (int i = 0; i < 10; i++) 
	{
		EnterCriticalSection(&cs);//���� �������Ĵ��봦������в����������߳̽��в�������������LeaveCriticalSection
		n_AddValue++;
		std::cout << "FirstThread is " << n_AddValue << std::endl;		
		LeaveCriticalSection(&cs);//���� ��EnterCriticalSection֮�������Դ�Ѿ��ͷ��ˣ������߳̿��Խ��в���   
		Sleep(50);

	}
	
	return 0;

}

//�ڶ����߳�
DWORD SecondThread(LPVOID lParam)
{	
	for (int i = 0; i < 10; i++) 
	{
		EnterCriticalSection(&cs);//����
		n_AddValue++;
		std::cout << "SecondThread is " << n_AddValue << std::endl;
		
		LeaveCriticalSection(&cs);//����
		Sleep(50);
	}	

	return 0;
}


int main(int argc, char**argv)
{
	int nRetCode = 0;	
	{
		InitializeCriticalSection(&cs);//��ʼ���ṹCRITICAL_SECTION

		HANDLE thread1 = CreateThread(NULL, 0, FirstThread, NULL, 0, nullptr);
		HANDLE thread2 = CreateThread(NULL, 0, SecondThread, NULL, 0, nullptr);

 
		HANDLE hThreadHandle[2];//
		hThreadHandle[0] = thread1;
		hThreadHandle[1] = thread2;

		//�ȴ��̷߳���
		WaitForMultipleObjects(2, hThreadHandle, TRUE, INFINITE);
	}

	return nRetCode;
}