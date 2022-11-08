
#include <iostream>
#include <Windows.h>


typedef void(__stdcall* MYPROC)(DWORD, char* , char*);

int main()
{
    std::cout << "Dynamic import" << std::endl;

    HINSTANCE hInstance = LoadLibrary(L"MemoryReplaceDll.dll");//функция возвращает дескриптор библиотеки DLL 

    if (!hInstance) {
        std::cout << "MemoryReplaceDll.dll not found";
        return 1;
    }

    MYPROC VirtualMemoryReplace = (MYPROC)GetProcAddress(hInstance, "VirtualMemoryReplace");

    int pid = GetCurrentProcessId();
    char replacedString[] = "Old string";
    char newString[] = "New string";

    char test1[] = "Old string";
    std::string test2 = "Old string";
    std::string test3 = " ___Old string___";

    std::cout << test1 << ' ' << test2 << ' ' << test3 << std::endl;
    
    VirtualMemoryReplace(pid, replacedString, newString);

    std::cout << test1 << ' ' << test2 << ' ' << test3 << std::endl;
    FreeLibrary(hInstance);
    std::cin.get();
}