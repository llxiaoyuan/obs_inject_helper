#include <iostream>
#include "my_print.h"
#include "AllocFree.h"
#include "my_print_alloc.h"

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

int main()
{
	HWND hWnd = FindWindow(TEXT("Notepad"), TEXT("无标题 - 记事本"));
	DWORD ProcessId;
	DWORD ThreadId = GetWindowThreadProcessId(hWnd, &ProcessId);

	wchar_t inject_path_w[MAX_PATH] = L"inject-helper64.exe";
	wchar_t hook_dll_w[MAX_PATH] = LR"(C:\Users\li\source\repos\obs_inject_helper_dll\x64\Release\obs_inject_helper_dll.dll)";
	unsigned long anti_cheat = 1;
	
	wchar_t* command_line_w = my_wsprintf_alloc(LR"("%s" "%s" %lu %lu)", inject_path_w, hook_dll_w, anti_cheat, anti_cheat ? ThreadId : ProcessId);
	if (command_line_w)
	{
		STARTUPINFO StartupInfo = { 0 };
		StartupInfo.cb = sizeof(StartupInfo);
		PROCESS_INFORMATION ProcessInformation = { 0 };
		BOOL success = CreateProcessW(inject_path_w, command_line_w, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &StartupInfo, &ProcessInformation);
		if (success) {
			WaitForSingleObject(ProcessInformation.hProcess, INFINITE);
			CloseHandle(ProcessInformation.hProcess);
			CloseHandle(ProcessInformation.hThread);
		}
		Free(command_line_w);
	}
	return 0;

}
