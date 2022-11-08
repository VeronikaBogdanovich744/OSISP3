// InjectionImport.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <wtypes.h>

typedef struct
{
	DWORD pid;
	char srcString[20] ;
	char resString[20];

} loadLibraryInfo;

void loadLibToProcess(DWORD pid, const char* libName);
void callReplaceString(DWORD pid, char* oldStr, char* newStr);
int main()
{
	DWORD pid;
	std::cout << "Process Id: ";
	std::cin >> pid;
	char oldString[] = "Old strivcvcng";
	char newString[] = "Inj string";

	loadLibToProcess(pid, "MemoryReplaceDll.dll");
	callReplaceString(pid, oldString, newString);

	std::cin.get();
}

void loadLibToProcess(DWORD pid,const char* libName) {
	//libName = dll name

	//parameter
	int libNameLength = strlen(libName);
	int libNameSize = libNameLength * sizeof(char);
	
	HANDLE hprocess, hThread;
	hprocess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, DWORD(pid));
	if (hprocess == NULL) {
		return;
	}
	PCSTR virtPath;
	virtPath = (PCSTR)VirtualAllocEx(hprocess, NULL, libNameSize, MEM_COMMIT, PAGE_READWRITE);
	WriteProcessMemory(hprocess, (LPVOID)virtPath, libName, libNameSize, NULL);
	
	auto threadFunc = GetProcAddress(LoadLibrary(L"Kernel32.dll"), "LoadLibraryA");

	hThread = CreateRemoteThread(hprocess, NULL, 0, (PTHREAD_START_ROUTINE)threadFunc, (LPVOID)virtPath, 0, NULL);
	WaitForSingleObject(hThread, INFINITE);

	if (virtPath != NULL)
		VirtualFreeEx(hprocess, (LPVOID)virtPath, 0, MEM_RELEASE);
	if (hThread != NULL)
		CloseHandle(hThread);
	if (hprocess != NULL)
		CloseHandle(hprocess);
}

void callReplaceString(DWORD pid, char* oldStr, char* newStr) {
	loadLibraryInfo info;
	info.pid = pid;	
	strcpy_s(info.srcString, oldStr);
	strcpy_s(info.resString, newStr);

	HANDLE hprocess, hThread;
	hprocess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, DWORD(pid));
	if (hprocess == NULL) {
		return;
	}
	PCSTR virtPath = (PCSTR)VirtualAllocEx(hprocess, NULL, sizeof(info), MEM_COMMIT, PAGE_READWRITE);
	WriteProcessMemory(hprocess, (LPVOID)virtPath, &info, sizeof(info), NULL);
	
	auto threadFunc =
		GetProcAddress(LoadLibraryA("MemoryReplaceDll.dll"), "InjectorVirtualMemoryReplace");

	hThread = CreateRemoteThread(hprocess, NULL,//дескриптор потока не может быть унаследован
		0, //размер стека в байтах - 0 = по умолчанию 
		(PTHREAD_START_ROUTINE)threadFunc,//Указатель на определяемую приложением функцию, которая должна выполняться потоком
		(LPVOID)virtPath,//Указатель на переменную, которая будет передана функции потока
		0,//Поток запускается сразу после создания
		NULL);
	WaitForSingleObject(hThread, INFINITE);//Ожидает, пока указанный объект не перейдет в сигнальное состояние или пока не истечет время ожидания.

	if (virtPath != NULL)
		VirtualFreeEx(hprocess, (LPVOID)virtPath, 0, MEM_RELEASE);
	if (hThread != NULL)
		CloseHandle(hThread);
	if (hprocess != NULL)
		CloseHandle(hprocess);
}
