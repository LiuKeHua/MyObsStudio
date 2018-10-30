#include <iostream>
#include <windows.h>


CRITICAL_SECTION       cs;

static int n_AddValue = 0;

DWORD FirstThread(LPVOID lParam)
{
	for (int i = 0; i < 10; i++) 
	{
		EnterCriticalSection(&cs);//加锁 接下来的代码处理过程中不允许其他线程进行操作，除非遇到LeaveCriticalSection
		n_AddValue++;
		std::cout << "FirstThread is " << n_AddValue << std::endl;		
		LeaveCriticalSection(&cs);//解锁 到EnterCriticalSection之间代码资源已经释放了，其他线程可以进行操作   
		Sleep(50);

	}
	
	return 0;

}

//第二个线程
DWORD SecondThread(LPVOID lParam)
{	
	for (int i = 0; i < 10; i++) 
	{
		EnterCriticalSection(&cs);//加锁
		n_AddValue++;
		std::cout << "SecondThread is " << n_AddValue << std::endl;
		
		LeaveCriticalSection(&cs);//解锁
		Sleep(50);
	}	

	return 0;
}


int main(int argc, char**argv)
{
	int nRetCode = 0;	
	{
		InitializeCriticalSection(&cs);//初始化结构CRITICAL_SECTION

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