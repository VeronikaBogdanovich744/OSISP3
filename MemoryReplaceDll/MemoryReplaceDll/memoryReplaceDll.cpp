//#define VirtualMemoryReplace_EXPORTS

#include "pch.h" 
#include "MemoryReplaceDll.h";
#include <string>

void RegionReplace(HANDLE hProcess, MEMORY_BASIC_INFORMATION* memInfo,
	char* oldstr, const char* newstr, int oldstrSize, int newstrSize);
char* SearchString(char* buffer, int bufferSize, const char* str, int strlen);

void __stdcall InjectorVirtualMemoryReplace(loadLibraryInfo* info) {
	VirtualMemoryReplace(info->pid, info->oldStr, info->newStr);
}

void __stdcall VirtualMemoryReplace(DWORD pid, char* oldstr, char* newstr) {
	int oldstrSize = strlen(oldstr);
	int newstrSize = strlen(newstr);

	if (!(newstrSize || oldstrSize)) return;
	
	std::string newString(newstr);
	while (newstrSize < oldstrSize) {
		newString.push_back(' ');
		newstrSize = newString.length();
	}

	SYSTEM_INFO sysInfo; 
	GetSystemInfo(&sysInfo); //инфорамция о системе и процессорах

	HANDLE hProcess = OpenProcess(PROCESS_VM_WRITE | PROCESS_VM_READ | PROCESS_QUERY_INFORMATION, //PROCESS_QUERY_INFORMATION - для получении информации о процессе
		false, //дескриптор не наследуем
		pid); 
	char* currAddress = (char*)sysInfo.lpMinimumApplicationAddress; //Указатель на наименьший адрес памяти,
	//доступный приложениям и библиотекам динамической компоновки (DLL)

	while (currAddress < sysInfo.lpMaximumApplicationAddress) {
		MEMORY_BASIC_INFORMATION memInfo; //Содержит информацию о диапазоне страниц в виртуальном адресном пространстве процесса
		VirtualQueryEx(hProcess, currAddress, &memInfo, sizeof(memInfo));//Извлекает информацию о диапазоне страниц в виртуальном адресном пространстве указанного процесса
		if (memInfo.State == MEM_COMMIT //Указывает зафиксированные страницы, для которых выделено физическое хранилище либо в памяти, либо в файле подкачки на диске
			&& memInfo.AllocationProtect == PAGE_READWRITE) {
			RegionReplace(hProcess, &memInfo, oldstr, newString.c_str(), oldstrSize, newstrSize);
		}
		currAddress += memInfo.RegionSize;
	}

	CloseHandle(hProcess);
}

void RegionReplace(HANDLE hProcess, MEMORY_BASIC_INFORMATION* memInfo,
	char* oldstr,const char* newstr, int oldstrSize, int newstrSize) {

	SIZE_T bytes;
	char* buffer = new char[memInfo->RegionSize];

	oldstr[0]++; //нельзя заменять строку, в которой находится информация какую строку необходимо искать
	ReadProcessMemory(hProcess, memInfo->BaseAddress, buffer, memInfo->RegionSize, &bytes);
	oldstr[0]--; 

	char* curBuffer = buffer;
	char* strpos = SearchString(curBuffer, memInfo->RegionSize, oldstr, oldstrSize);
	while (strpos) {
		
		WriteProcessMemory(hProcess, (char*)memInfo->BaseAddress + (strpos - buffer), newstr, oldstrSize, nullptr);
		curBuffer = strpos + oldstrSize;
		strpos = SearchString(curBuffer, memInfo->RegionSize - (curBuffer - buffer), oldstr, oldstrSize);
	}
	delete[] buffer;
}

char* SearchString(char* buffer, int bufferSize, const char* str, int strlen) {
	
	for (int i = 0; i < bufferSize - strlen; i++) {
		if (!strncmp(buffer + i, str, strlen)) {
			return buffer + i;
		}
	}
	return nullptr;
}