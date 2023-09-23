#include "AllocFree.h"
#include <stdlib.h>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

static void* my_memcpy(void* _Dst, void const* _Src, size_t _Size)
{
    char* dp = (char*)_Dst;
    char* sp = (char*)_Src;
    for (size_t i = 0; i < _Size; i++) {
        dp[i] = sp[i];
    }
    return _Dst;
}

static void* my_memset(void* _Dst, int _Val, size_t _Size)
{
    char* q = (char*)_Dst;
    while (_Size--) {
        *q++ = _Val;
    }
    return _Dst;
}


void* Alloc(size_t BufferSize)
{
    //return VirtualAlloc(NULL, BufferSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

    //VOID* BaseAddress = NULL;
    //SIZE_T RegionSize = BufferSize;
    //NtAllocateVirtualMemory((HANDLE)-1, &BaseAddress, 0, &RegionSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    //return BaseAddress;

    //return malloc(BufferSize);

    return HeapAlloc(GetProcessHeap(), 0, BufferSize);
}

void Free(void* Buffer)
{
    //VirtualFree(Buffer, 0, MEM_RELEASE);

    //VOID* BaseAddress = Buffer;
    //SIZE_T RegionSize = 0;
    //NtFreeVirtualMemory((HANDLE)-1, &BaseAddress, &RegionSize, MEM_RELEASE);

    //free(Buffer);

    HeapFree(GetProcessHeap(), 0, Buffer);
}

void* ReAlloc(void* OldBuffer, size_t NewBufferSize)
{
    Free(OldBuffer);
    return Alloc(NewBufferSize);
}

void* ReAllocExpand(void* OldBuffer, size_t NewBufferSize, size_t OldBufferSize)
{
    void* result = NULL;
    void* NewBuffer = Alloc(NewBufferSize);
    if (NewBuffer) {
        result = my_memcpy(NewBuffer, OldBuffer, OldBufferSize);
    }
    Free(OldBuffer);
    return result;
}
