#include <Windows.h>

int main()
{
	Sleep(3000);

	// ģ�����Ҽ�
	mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);
	mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);

	// ģ�ⰴ��'A'��
	keybd_event('A', 0, 0, 0);
	keybd_event('A', 0, KEYEVENTF_KEYUP, 0);


	for (int i = 0; i < 100; ++i)
	{
		Sleep(100);
		keybd_event(VK_SPACE, 0, 0, 0);
		keybd_event(VK_SPACE, 0, KEYEVENTF_KEYUP, 0);
	}




	// ģ�ⰴ�� CTRL + F
// 	keybd_event(VK_CONTROL, (BYTE)0, 0, 0);
// 	keybd_event('F', (BYTE)0, 0, 0);
// 
// 	keybd_event('F', (BYTE)0, KEYEVENTF_KEYUP, 0);
// 	keybd_event(VK_CONTROL, (BYTE)0, KEYEVENTF_KEYUP, 0);


	return 0;
}