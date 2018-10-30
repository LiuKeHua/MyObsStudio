#include <iostream>
#include <windows.h>


CRITICAL_SECTION       cs;

static int n_AddValue = 0;
static HANDLE                  signal_init = NULL;

#define EVENT_HOOK_READY      L"CaptureHook_HookReady"

DWORD FirstThread(LPVOID lParam)
{
	while (true)
	{
		DWORD ret = WaitForSingleObject(signal_init, INFINITE);

		std::cout << "HelloWorld" << std::endl;
	}
	
	return 0;

}

//�ڶ����߳�
DWORD SecondThread(LPVOID lParam)
{
	
	while (true)
	{
		SetEvent(signal_init);

		Sleep(1000);
	}
	
	return 0;
}


int main(int argc, char**argv)
{
	int nRetCode = 0;	
	{
		signal_init = CreateEvent(NULL,
			FALSE, //automatically resets��FalseΪ�Զ�reset
			TRUE, //flag for initial state
			NULL); 
		ResetEvent(signal_init);

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



void MyEventTest()
{
	{
		HANDLE hEvent = CreateEvent(NULL,
			FALSE, //automatically resets
			TRUE, //flag for initial state
			NULL);

		WaitForSingleObject(hEvent, -1); //�˴������˳�����Ϊǰ��ĳ�ʼֵ��TRUE
		SetEvent(hEvent); //�����¼�
		WaitForSingleObject(hEvent, -1); //�˴������˳�����Ϊǰ���SetEvent
		WaitForSingleObject(hEvent, 2000); //�˴��ᳬʱ�˳�

		CloseHandle(hEvent);
	}

	{
		HANDLE hEvent = CreateEvent(NULL,
			TRUE, //manual-reset 
			TRUE, //flag for initial state
			NULL);

		WaitForSingleObject(hEvent, -1); //�˴������˳�����Ϊǰ��ĳ�ʼֵ��TRUE
		WaitForSingleObject(hEvent, -1); //�˴������˳�����Ϊ��Ҫ�ֶ���λ
		ResetEvent(hEvent); //�����¼�
		WaitForSingleObject(hEvent, 2000); //�˴��ᳬʱ�˳�
		SetEvent(hEvent);
		WaitForSingleObject(hEvent, -1); //�˴������˳�����Ϊǰ���SetEvent

		CloseHandle(hEvent);
	}

}