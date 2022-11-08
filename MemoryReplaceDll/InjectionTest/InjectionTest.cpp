// InjectionTest.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include <Windows.h>
#include <iostream>
#include <processthreadsapi.h>

int main()
{
    int pid = GetCurrentProcessId();
    std::cout << pid << std::endl;
    char replacedString[] = "Old strivcvcng";

    char test1[] = "Old strivcvcng";
    std::string test2 = "Old strivcvcng";
    std::string test3 = " ___Old strivcvcng___";
    std::cout << test1 << ' ' << test2 << ' ' << test3 << std::endl;
    std::cin.get();
    std::cout << test1 << ' ' << test2 << ' ' << test3 << std::endl;
    std::cin.get();

}
