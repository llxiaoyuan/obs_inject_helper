#define _CRT_SECURE_NO_WARNINGS

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#include <stdio.h>

extern "C"
__declspec(dllexport) LRESULT CALLBACK dummy_debug_proc(int code, WPARAM wparam, LPARAM lparam)
{
    static BOOL hooking = TRUE;
    MSG* msg = (MSG*)lparam;
    if (hooking && msg->message == (WM_USER + 432)) {
        UnhookWindowsHookEx((HHOOK)msg->lParam);
        hooking = FALSE;
        AllocConsole();
        freopen("CONOUT$", "w", stdout);
        printf("dummy_debug_proc\n");
    }
    return CallNextHookEx(0, code, wparam, lparam);
}

DWORD WINAPI ThreadFunction(LPVOID lpThreadParameter)
{
    AllocConsole();
    freopen("CONOUT$", "w", stdout);
    printf("ThreadFunction\n");
    return 0;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH: {
        HANDLE hThread = CreateThread(NULL, 0, ThreadFunction, NULL, 0, NULL);
        if (hThread != NULL) {
            CloseHandle(hThread);
        }
    } break;
    case DLL_THREAD_ATTACH: {} break;
    case DLL_THREAD_DETACH: {} break;
    case DLL_PROCESS_DETACH: {} break;
    }
    return TRUE;
}

