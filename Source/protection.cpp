#include "stdafx.h"
#include "protection.h"
#include <SetupAPI.h>
#include <stdlib.h>
#include <stdio.h>
#include <Psapi.h>
#include <tlhelp32.h>
#include <vdmdbg.h>
#include <conio.h>

#if (MEM_PROTECT==1)
#pragma comment(lib,"Psapi.lib")

int removeFunctionsArray[255];
int imageBaseOfGS = 0x00400000;
BYTE replacementPart = 0xCC;
int replacementRange = 0x00400000;
BYTE executedParentSearch = 0;
BYTE processesFound = 0;

#if (MEM_PROTECT_DEBUG==1)
	int functionMangleApiMain = (0x00783B2A - 0x00783840) + (0x0078722E - 0x007870BA);
	int functionMangleInit = 0x004A4FB0 - 0x004A4F21;
#else
	int functionMangleApiMain = 0x004A0B80 - 0x004A0B29;
	int functionMangleInit = 0x004A4FB0 - 0x004A4F21;
#endif
BYTE jmpLength = 5;


ULONG_PTR GetParentProcessId()
{
#if (WL_PROTECT==1)
	VM_START
#endif

	ULONG_PTR pbi[6];
	ULONG ulSize = 0;  
	
	LONG (WINAPI *NtQueryInformationProcess)(HANDLE ProcessHandle, ULONG ProcessInformationClass, PVOID ProcessInformation, ULONG ProcessInformationLength, PULONG ReturnLength);
	*(FARPROC *)&NtQueryInformationProcess = GetProcAddress(LoadLibraryA("NTDLL.DLL"), "NtQueryInformationProcess");
	
	if(NtQueryInformationProcess){
		if(NtQueryInformationProcess(GetCurrentProcess(), 0, &pbi, sizeof(pbi), &ulSize) >= 0 && ulSize == sizeof(pbi))
		{
			executedParentSearch = 1;
			return pbi[5];
		}
	}
	return (ULONG_PTR)-1; 

#if (WL_PROTECT==1)
	VM_END
#endif
} 

void CheckProcessByFileName(LPCTSTR ProcessName)
{
#if (WL_PROTECT==1)
	VM_START
#endif

	int found = 0;

	PROCESSENTRY32 pe32;
	HANDLE hSnapshot = NULL;
	ZeroMemory(&pe32, sizeof(PROCESSENTRY32));

	// We want a snapshot of processes
	hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	// Check for a valid handle, in this case we need to check for
	// INVALID_HANDLE_VALUE instead of NULL
	if(hSnapshot == INVALID_HANDLE_VALUE)
		return;

	// Now we can enumerate the running process, also 
	// we can't forget to set the PROCESSENTRY32.dwSize member
	// otherwise the following functions will fail
	pe32.dwSize =  sizeof(PROCESSENTRY32);
	
	if(Process32First(hSnapshot, &pe32) == FALSE)
	{
		// Cleanup the mess
		CloseHandle(hSnapshot);
		return;
	}

	// Do our first comparison
	if(_tcsicmp(pe32.szExeFile, ProcessName) == FALSE)
	{
		processesFound++;
	}

	// Most likely it won't match on the first try so 
	// we loop through the rest of the entries until
	// we find the matching entry or not one at all
	while (Process32Next(hSnapshot, &pe32))
	{
		if(_tcsicmp(pe32.szExeFile, ProcessName) == 0)
		{
			processesFound++;
		}
	}

	// If we made it this far there wasn't a match
	// so we'll return 0
	CloseHandle(hSnapshot);

#if (WL_PROTECT==1)
	VM_END
#endif
}

DWORD WINAPI GetParentPIDById( DWORD crtpid )
{
#if (WL_PROTECT==1)
	VM_START
#endif

	OSVERSIONINFO  osver ;
	HINSTANCE      hInstLib ;
	HANDLE         hSnapShot ;
	BOOL           bContinue ;
	PROCESSENTRY32 procentry;

	// ToolHelp Function Pointers.
	HANDLE (WINAPI *lpfCreateToolhelp32Snapshot)(DWORD,DWORD) ;
	BOOL (WINAPI *lpfProcess32First)(HANDLE,LPPROCESSENTRY32) ;
	BOOL (WINAPI *lpfProcess32Next)(HANDLE,LPPROCESSENTRY32) ;

	// Check to see if were running under Windows95 or
	// Windows NT.
	osver.dwOSVersionInfoSize = sizeof( osver ) ;
	if( !GetVersionEx( &osver ) )
	{
		return FALSE ;
	}

	if( osver.dwPlatformId != VER_PLATFORM_WIN32_NT )
	{
	  //ShowErr();
	  return FALSE;
	}

	hInstLib = LoadLibraryA( "Kernel32.DLL" ) ;
	if( hInstLib == NULL )
	{
		//ShowErr();
        return FALSE ;
	}

	// Get procedure addresses.
	// We are linking to these functions of Kernel32
	// explicitly, because otherwise a module using
	// this code would fail to load under Windows NT,
	// which does not have the Toolhelp32
	// functions in the Kernel 32.
    lpfCreateToolhelp32Snapshot=(HANDLE(WINAPI *)(DWORD,DWORD))GetProcAddress( hInstLib,"CreateToolhelp32Snapshot" ) ;
	lpfProcess32First=(BOOL(WINAPI *)(HANDLE,LPPROCESSENTRY32))GetProcAddress( hInstLib, "Process32First" ) ;
	lpfProcess32Next=(BOOL(WINAPI *)(HANDLE,LPPROCESSENTRY32))GetProcAddress( hInstLib, "Process32Next" ) ;

	if( lpfProcess32Next == NULL ||
		lpfProcess32First == NULL ||
		lpfCreateToolhelp32Snapshot == NULL )
	{
		FreeLibrary( hInstLib ) ;
		return FALSE ;
	}

	// Get a handle to a Toolhelp snapshot of the systems
	// processes.
	hSnapShot = lpfCreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0 ) ;
	if( hSnapShot == INVALID_HANDLE_VALUE )
	{
		//ShowErr();
		FreeLibrary( hInstLib ) ;
		return FALSE ;
	}

	// Get the first process' information.
	memset((LPVOID)&procentry,0,sizeof(PROCESSENTRY32));
	procentry.dwSize = sizeof(PROCESSENTRY32) ;
	bContinue = lpfProcess32First( hSnapShot, &procentry ) ;
	DWORD pid = 0;
	// While there are processes, keep looping.

	char sbuff[256]={0};

	if (crtpid == -1)
		crtpid= GetCurrentProcessId();

	while( bContinue )
	{
		if(crtpid == procentry.th32ProcessID)
		{
			pid =  procentry.th32ParentProcessID;

			/*FILE *log = fopen("./1.txt", "at");
			if (!log) log = fopen("./1.txt", "wt");

			memset(sbuff,0,sizeof(sbuff));
			wsprintf(sbuff,"\nPROC ID:%d %s %d %d %d %d %d %d %d %d",
				procentry.th32ProcessID,
				procentry.szExeFile,
				procentry.cntThreads,
				procentry.cntUsage,
				procentry.dwFlags,
				procentry.dwSize,
				procentry.th32ModuleID,
				procentry.th32ParentProcessID,
				procentry.pcPriClassBase,
				procentry.th32DefaultHeapID
			);
			fwrite (sbuff , 1 , sizeof(sbuff) , log );
			fclose(log);*/
		}

		procentry.dwSize = sizeof(PROCESSENTRY32) ;
		bContinue = !pid && lpfProcess32Next( hSnapShot, &procentry );
	}//while ends

	// Free the library.
	FreeLibrary( hInstLib ) ;

	return pid;

#if (WL_PROTECT==1)
	VM_END
#endif
}
#endif
