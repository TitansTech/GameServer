#include "stdafx.h"
#include "GameServer.h"
#include "LogProc.h"
#include "GameMain.h"
#include "User.h"
#include "TNotice.h"
#include "..\common\winutil.h"
#include "..\include\readscript.h"
#include "Experiments.h"


/*

Must be all declared.

void CheckRemoteDebuggerPresent()
{
	BOOL pbIsPresent = FALSE;

	CheckRemoteDebuggerPresent(GetCurrentProcess(), &pbIsPresent);
	if (pbIsPresent) {
		exit(0);
	}
}


void GetTickCountDebug()
{
	DWORD li, li2;

	li = GetTickCount();
	li2 = GetTickCount();
	
	if ((li2-li) > 0x10) {
		exit(0);
	} 

}

void GetHardwareDebug()
{
	HANDLE hnd;
	CONTEXT ctx;
	int status;

	hnd = GetCurrentThread();
	status = GetThreadContext(hnd, &ctx);
	if (status == 0) printf("Error in GetThreadContext() %d\n", GetLastError());
	

	if ((ctx.Dr0 != 0x00) || (ctx.Dr1 != 0x00) || (ctx.Dr2 != 0x00) || (ctx.Dr3 != 0x00) || (ctx.Dr6 != 0x00)
		|| (ctx.Dr7 != 0x00))
	{
		exit(0);
	} 
}

void IsDebuggerPresentRuntime()
{
	if (IsDebuggerPresent()) {
		exit(0);
	} 
}

void IsDebuggerPresentRuntimePIB()
{
	HANDLE hnd;
	HMODULE hmod;
	FARPROC _NtQueryInformationProcess;
	PROCESS_BASIC_INFORMATION pPIB;
	NTSTATUS status;
	PULONG bytesWritten;

	hmod = LoadLibrary(L"Ntdll.dll");
	_NtQueryInformationProcess = GetProcAddress(hmod, "NtQueryInformationProcess");

	hnd = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, GetCurrentProcessId());
	status = (_NtQueryInformationProcess) (hnd, ProcessBasicInformation, &pPIB, sizeof(PROCESS_BASIC_INFORMATION), &bytesWritten);

	if (status == 0 ) {
		if (pPIB.PebBaseAddress->BeingDebugged == 1) {
			exit(0);
		} 
	}

	return;
}

void IsDebuggerPresentRuntimeSetCheck()
{
	HANDLE hnd;
	HMODULE hmod;
	FARPROC _NtQueryInformationProcess;
	PROCESS_BASIC_INFORMATION pPIB;
	NTSTATUS status;
	PULONG bytesWritten;
	int retVal;

	hmod = LoadLibrary(L"Ntdll.dll");
	_NtQueryInformationProcess = GetProcAddress(hmod, "NtQueryInformationProcess");

	hnd = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, GetCurrentProcessId());
	status = (_NtQueryInformationProcess) (hnd, ProcessBasicInformation, &pPIB, sizeof(PROCESS_BASIC_INFORMATION), &bytesWritten);

	if (status == 0) {
		pPIB.PebBaseAddress->BeingDebugged = 0x90;
	}

	retVal = IsDebuggerPresent();
	if (retVal != 0x90 ) {
		exit(0);
	} 
}

void IsDebuggerPresentNTGlobalFlag()
{
	HANDLE hnd;
	HMODULE hmod;
	FARPROC _NtQueryInformationProcess;
	PROCESS_BASIC_INFORMATION pPIB;
	NTSTATUS status;
	PULONG bytesWritten;
	const char *value;

	hmod = LoadLibrary(L"Ntdll.dll");
	_NtQueryInformationProcess = GetProcAddress(hmod, "NtQueryInformationProcess");

	hnd = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, GetCurrentProcessId());
	status = (_NtQueryInformationProcess) (hnd, ProcessBasicInformation, &pPIB, sizeof(PROCESS_BASIC_INFORMATION), &bytesWritten);

	value = (pPIB.PebBaseAddress);
	value = value+0x68;
	printf("FLAG DWORD: %08X\n", *value);
	
	if (*value == 0x70) {
		exit(0);
	} 
	return;
}

*/