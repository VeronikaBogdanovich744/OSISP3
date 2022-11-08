// dllmain.cpp : Определяет точку входа для приложения DLL.
#include "pch.h"
#include "MemoryReplaceDll.h"
#include <string>

BOOL APIENTRY DllMain( HMODULE hModule, // handle(базовый адрес библиотеки DLL)
                       DWORD  ul_reason_for_call, // reason for calling function
                       LPVOID lpReserved // reserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH: //первая загрузка DLL каким-либо потоком
    {
   /* char replacedString[] = "Hello world";
    char newString[] = "trolled!";
    size_t oldstrSize = strlen(replacedString);
    size_t newstrSize = strlen(newString);
    VirtualMemoryReplace(GetCurrentProcessId(), replacedString, newString);*/
    }
    case DLL_THREAD_ATTACH: //подключение нового потока
    case DLL_THREAD_DETACH: //упорядоченное завершение потока
    case DLL_PROCESS_DETACH: //упорядоченное завершение процесса
        break;
    }
    return TRUE;
}

