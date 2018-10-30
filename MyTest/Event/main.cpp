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

//第二个线程
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
			FALSE, //automatically resets，False为自动reset
			TRUE, //flag for initial state
			NULL); 
		ResetEvent(signal_init);

		HANDLE thread1 = CreateThread(NULL, 0, FirstThread, NULL, 0, nullptr);
		HANDLE thread2 = CreateThread(NULL, 0, SecondThread, NULL, 0, nullptr);

 
		HANDLE hThreadHandle[2];//
		hThreadHandle[0] = thread1;
		hThreadHandle[1] = thread2;

		//等待线程返回
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

		WaitForSingleObject(hEvent, -1); //此处正常退出，因为前面的初始值是TRUE
		SetEvent(hEvent); //设置事件
		WaitForSingleObject(hEvent, -1); //此处正常退出，因为前面的SetEvent
		WaitForSingleObject(hEvent, 2000); //此处会超时退出

		CloseHandle(hEvent);
	}

	{
		HANDLE hEvent = CreateEvent(NULL,
			TRUE, //manual-reset 
			TRUE, //flag for initial state
			NULL);

		WaitForSingleObject(hEvent, -1); //此处正常退出，因为前面的初始值是TRUE
		WaitForSingleObject(hEvent, -1); //此处正常退出，因为需要手动复位
		ResetEvent(hEvent); //重置事件
		WaitForSingleObject(hEvent, 2000); //此处会超时退出
		SetEvent(hEvent);
		WaitForSingleObject(hEvent, -1); //此处正常退出，因为前面的SetEvent

		CloseHandle(hEvent);
	}

}