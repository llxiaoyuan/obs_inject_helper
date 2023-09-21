#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#define WIN32_LEAN_AND_MEAN
#define WIN32_NO_STATUS
#define NOMINMAX
#include <Windows.h>
#include <winternl.h>

/*

1. most effective - use LdrAddRefDll - this do exactly what you need in shortest way. you need use ntdll.lib (or ntdllp.lib) for linking.

LdrAddRefDll(0, (HMODULE)&__ImageBase);

2. not nice and effective but work

WCHAR sz[MAX_PATH];
if (GetModuleFileName((HMODULE)&__ImageBase, sz, RTL_NUMBER_OF(sz)))
{
    LoadLibrary(sz);
}

*/

//DWORD WINAPI ThreadFunction(LPVOID lpThreadParameter)
//{
//    AllocConsole();
//    freopen("CONOUT$", "w", stdout);
//    printf("ThreadFunction\n");
//    return 0;
//}

typedef NTSTATUS(NTAPI* __LdrAddRefDll)(_In_ ULONG Flags, _In_ PVOID DllHandle);
HMODULE g_hModule;

DWORD WINAPI Function(LPVOID lpParameter)
{
    AllocConsole();
    freopen("CONOUT$", "w", stdout);

    printf("%p\n", g_hModule);

    __LdrAddRefDll _LdrAddRefDll = (__LdrAddRefDll)GetProcAddress(GetModuleHandle(TEXT("ntdll.dll")), "LdrAddRefDll");
    _LdrAddRefDll(0, g_hModule);

    printf("Function\n");
    return 0;
}

extern "C" __declspec(dllexport) LRESULT CALLBACK dummy_debug_proc(int code, WPARAM wparam, LPARAM lparam)
{
    static BOOL hooking = TRUE;
    MSG* msg = (MSG*)lparam;
    if (hooking && msg->message == (WM_USER + 432)) {
        UnhookWindowsHookEx((HHOOK)msg->lParam);
        hooking = FALSE;
        Function(NULL);
    }
    return CallNextHookEx(0, code, wparam, lparam);
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH: {
        //HANDLE hThread = CreateThread(NULL, 0, ThreadFunction, NULL, 0, NULL);
        //if (hThread != NULL) {
        //    CloseHandle(hThread);
        //}
        g_hModule = hModule;
    } break;
    case DLL_THREAD_ATTACH: {
    } break;
    case DLL_THREAD_DETACH: {
    } break;
    case DLL_PROCESS_DETACH: {
    } break;
    }
    return TRUE;
}

