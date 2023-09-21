#include <iostream>
#include "my_print.h"
#include "AllocFree.h"
#include "my_print_alloc.h"

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

int wmain(int argc, wchar_t* argv[])
{
	if (argc == 1 + 3) {
		wchar_t* ClassName = argv[1];
		wchar_t* WindowName = argv[2];
		wchar_t* hook_dll_w = argv[3];

		wprintf(L"ClassName: %s\n", ClassName);
		wprintf(L"WindowName: %s\n", WindowName);
		wprintf(L"hook_dll_w: %s\n", hook_dll_w);

		HWND hWnd = FindWindowW(ClassName, WindowName);
		if (hWnd != NULL) {
			DWORD ProcessId;
			DWORD ThreadId = GetWindowThreadProcessId(hWnd, &ProcessId);

			wchar_t inject_path_w[MAX_PATH] = L"inject-helper64.exe";
			//wchar_t hook_dll_w[MAX_PATH] = LR"(C:\Users\li\source\repos\obs_inject_helper_dll\x64\Release\obs_inject_helper_dll.dll)";
			unsigned long anti_cheat = 1;

			wchar_t* command_line_w = my_wsprintf_alloc(LR"("%s" "%s" %lu %lu)", inject_path_w, hook_dll_w, anti_cheat, anti_cheat ? ThreadId : ProcessId);
			if (command_line_w) {
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
		}
	}
	else {
		wprintf(LR"(obs_inject_helper.exe "ClassName" "WindowName" "hook_dll_w"\n)");
	}
	return 0;

}
