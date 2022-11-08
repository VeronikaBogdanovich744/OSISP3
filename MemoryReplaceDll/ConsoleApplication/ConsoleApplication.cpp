
#include <iostream>
#include "MemoryReplaceDll.h"
#include <processthreadsapi.h>



int main()
{
    std::cout << "Static import" << std::endl;

    int pid = GetCurrentProcessId();
    char replacedString[] = "Old string";
    char newString[] = "New string";

    char test1[] = "Old string";
    std::string test2 = "Old string";
    std::string test3 = " ___Old string___";

    std::cout << test1 << ' ' << test2 << ' ' << test3 << std::endl;
    VirtualMemoryReplace(pid, replacedString, newString);
    std::cout << test1 << ' ' << test2 << ' ' << test3 << std::endl;

    std::cin.get();
}
