#include "my_print_alloc.h"
#include "my_print.h"
#include "AllocFree.h"
#include <stdarg.h>

char* my_sprintf_alloc(const char* Format, ...)
{
	va_list ArgList;
	va_start(ArgList, Format);
	size_t BufferCount = my_print(NULL, Format, ArgList);//no null end
	char* Buffer = (char*)Alloc((BufferCount + 1) * sizeof(char));
	if (Buffer != NULL) {
		my_print(Buffer, Format, ArgList);
		Buffer[BufferCount] = '\0';
	}
	va_end(ArgList);
	return Buffer;
}

wchar_t* my_wsprintf_alloc(const wchar_t* Format, ...)
{
	va_list ArgList;
	va_start(ArgList, Format);
	size_t BufferCount = my_wprint(NULL, Format, ArgList);//no null end
	wchar_t* Buffer = (wchar_t*)Alloc((BufferCount + 1) * sizeof(wchar_t));
	if (Buffer != NULL) {
		my_wprint(Buffer, Format, ArgList);
		Buffer[BufferCount] = '\0';
	}
	va_end(ArgList);
	return Buffer;
}