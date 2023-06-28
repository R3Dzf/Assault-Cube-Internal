#include "injector.h"
#include <TlHelp32.h>

void Injector::inject()
{
	if (this->processPid == 0)
	{
		printf("[+] failed find process\n");
		Sleep(3000);
		exit(0);
	}
	GetFullPathNameA("Assault-Cube-Internal.dll", MAX_PATH, buffer, nullptr);
	hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processPid);
	allocatedMemory = VirtualAllocEx(hProc, 0, lstrlenA(buffer) + 1, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	WriteProcessMemory(hProc, allocatedMemory, buffer, lstrlenA(buffer) + 1, nullptr);
	kernel32 = GetModuleHandleA("kernel32.dll");
	loadLibrary = GetProcAddress(kernel32, "LoadLibraryA");
	CreateRemoteThread(hProc, 0, 0, LPTHREAD_START_ROUTINE(loadLibrary), allocatedMemory, 0,0);
	printf("[+] injected =)");
}

void Injector::findProcess()
{
	PROCESSENTRY32 processList{};
	processList.dwSize = sizeof(processList);
	HANDLE hProcessHandle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	do
	{
		if (processList.szExeFile == processName)
		{
			printf("[+] Pid: %d\n", processList.th32ProcessID);
			this->processPid = processList.th32ProcessID;
		}
	} while (Process32Next(hProcessHandle, &processList));
}

