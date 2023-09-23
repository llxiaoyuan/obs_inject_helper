#pragma once
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif
	typedef void* (*__Alloc)(size_t BufferSize);
	typedef void (*__Free)(void* Buffer);
	typedef void* (*__ReAlloc)(void* Buffer, size_t NewBufferSize);
	typedef void* (*__ReAllocExpand)(void* OldBuffer, size_t NewBufferSize, size_t OldBufferSize);

	void* Alloc(size_t BufferSize);
	void Free(void* Buffer);
	void* ReAlloc(void* OldBuffer, size_t NewBufferSize);
	void* ReAllocExpand(void* OldBuffer, size_t NewBufferSize, size_t OldBufferSize);
#ifdef __cplusplus
}
#endif