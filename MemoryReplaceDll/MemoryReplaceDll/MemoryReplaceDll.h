#pragma once

#include <Windows.h>

#ifdef VirtualMemoryReplace_EXPORTS
#define VirtualMemoryReplace_API __declspec(dllexport)
#else
#define VirtualMemoryReplace_API __declspec(dllimport)
#endif

typedef struct
{
    DWORD pid;
    char oldStr[20];
    char newStr[20];

} loadLibraryInfo;

extern "C" VirtualMemoryReplace_API void __stdcall InjectorVirtualMemoryReplace(loadLibraryInfo* info);
extern "C" VirtualMemoryReplace_API void __stdcall VirtualMemoryReplace(DWORD pid, char* oldstr, char* newstr);
