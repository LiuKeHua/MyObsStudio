#include <iostream>
#include <windows.h>
using namespace std;

bool create_process( wchar_t *cmd_line, HANDLE stdin_handle, HANDLE stdout_handle, HANDLE *process)
{
	PROCESS_INFORMATION pi = { 0 };

	STARTUPINFOW si = { 0 };

	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	si.hStdInput = stdin_handle;
	si.hStdOutput = stdout_handle;
	si.wShowWindow = TRUE;

	bool success = !!CreateProcessW(cmd_line, NULL, NULL, NULL, true,
		CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);


	if (success)
	{
		*process = pi.hProcess;
		CloseHandle(pi.hThread);
	}
	return success;
}

bool create_pipe(HANDLE *input, HANDLE *output)
{
	SECURITY_ATTRIBUTES sa = { 0 };

	sa.nLength = sizeof(sa);
	sa.bInheritHandle = true;

	if (!CreatePipe(input, output, &sa, 0))
	{
		return false;
	}

	return true;
}

int main()
{
	bool success = true;

	HANDLE process;
	HANDLE output;
	HANDLE input;

	if (!create_pipe(&input, &output))
	{
		return 1;
	}
	success = !!SetHandleInformation(output,HANDLE_FLAG_INHERIT, false);

	if (!success)
		return 2;

	wchar_t *cmd_line = L"F://OBS//MyObsStudio//BUILD_VS2015//Myout//Debug//bin//64bit//pipe_read.exe ";

	success = create_process(cmd_line, input, NULL, &process);


	DWORD bytes_written;

	static int num = 0;
	size_t len = sizeof(int);
	while (success)
	{
		num++;
		success = !!WriteFile(output, &num, (DWORD)len, &bytes_written, NULL);
		std::cout << num << std::endl;
		Sleep(100);
	}
	

	return 0;
}
