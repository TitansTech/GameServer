#ifdef _WIN64

// This function will erase the current images
// PE header from memory preventing a successful image
// if dumped
inline void ErasePEHeaderFromMemory()
{
	DWORD OldProtect = 0;
	
	// Get base address of module
	char *pBaseAddr = (char*)GetModuleHandle(NULL);

	// Change memory protection
	VirtualProtect(pBaseAddr, 4096, // Assume x86 page size
			PAGE_READWRITE, &OldProtect);

	// Erase the header
	ZeroMemory(pBaseAddr, 4096);
}

#else

#include <Windows.h>
#include <TlHelp32.h>
#include <tchar.h>

#if defined( _MSC_VER ) && (_MSC_VER > 1200)
	#pragma once
#endif


#pragma pack(1)

/* @bet0x: Fix for this crap code*/

#define NTSTATUS ULONG

// Grabbed this definition of MSDN and modified one pointer
// http://msdn.microsoft.com/en-us/library/ms684280(VS.85).aspx
typedef struct _PROCESS_BASIC_INFORMATION {
	PVOID Reserved1;
	void* PebBaseAddress;
	PVOID Reserved2[2];
	ULONG_PTR UniqueProcessId;
	ULONG_PTR ParentProcessId;
} PROCESS_BASIC_INFORMATION;

// Taken from: http://msdn.microsoft.com/en-us/library/
// aa380518(VS.85).aspx
typedef struct _LSA_UNICODE_STRING {
	USHORT Length;
	USHORT MaximumLength;
	PWSTR Buffer;
} LSA_UNICODE_STRING, *PLSA_UNICODE_STRING, 
UNICODE_STRING, *PUNICODE_STRING;


typedef struct _OBJECT_TYPE_INFORMATION {
	UNICODE_STRING TypeName;
	ULONG TotalNumberOfHandles;
	ULONG TotalNumberOfObjects;
}OBJECT_TYPE_INFORMATION, *POBJECT_TYPE_INFORMATION;

// Returned by the ObjectAllTypeInformation class
// passed to NtQueryObject
typedef struct _OBJECT_ALL_INFORMATION {
	ULONG NumberOfObjects;
	OBJECT_TYPE_INFORMATION ObjectTypeInformation[1];
}OBJECT_ALL_INFORMATION, *POBJECT_ALL_INFORMATION;

#pragma pack()

// CheckCloseHandle will call CloseHandle on an invalid
// DWORD aligned value and if a debugger is running an exception
// will occur and the function will return true otherwise it'll
// return false
inline bool CheckDbgPresentCloseHandle()
{
	HANDLE Handle = (HANDLE)0x8000;
	__try
	{
		CloseHandle(Handle);
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		return true;
	}
	
	return false;
}

// This function will erase the current images
// PE header from memory preventing a successful image
// if dumped
inline void ErasePEHeaderFromMemory()
{
	DWORD OldProtect = 0;
	
	// Get base address of module
	char *pBaseAddr = (char*)GetModuleHandle(NULL);

	// Change memory protection
	VirtualProtect(pBaseAddr, 4096, // Assume x86 page size
			PAGE_READWRITE, &OldProtect);

	// Erase the header
	ZeroMemory(pBaseAddr, 4096);
}

// This function uses the toolhelp32 api to enumerate all running processes
// on the computer and does a comparison of the process name against the 
// ProcessName parameter. The function will return 0 on failure.
inline DWORD GetProcessIdFromName(LPCTSTR ProcessName)
{
	PROCESSENTRY32 pe32;
	HANDLE hSnapshot = NULL;
	ZeroMemory(&pe32, sizeof(PROCESSENTRY32));

	// We want a snapshot of processes
	hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	// Check for a valid handle, in this case we need to check for
	// INVALID_HANDLE_VALUE instead of NULL
	if(hSnapshot == INVALID_HANDLE_VALUE)
		return 0;

	// Now we can enumerate the running process, also 
	// we can't forget to set the PROCESSENTRY32.dwSize member
	// otherwise the following functions will fail
	pe32.dwSize =  sizeof(PROCESSENTRY32);
	
	if(Process32First(hSnapshot, &pe32) == FALSE)
	{
		// Cleanup the mess
		CloseHandle(hSnapshot);
		return 0;
	}

	// Do our first comparison
	if(_tcsicmp(pe32.szExeFile, ProcessName) == FALSE)
	{
		// Cleanup the mess
		CloseHandle(hSnapshot);
		return pe32.th32ProcessID;
	}

	// Most likely it won't match on the first try so 
	// we loop through the rest of the entries until
	// we find the matching entry or not one at all
	while (Process32Next(hSnapshot, &pe32))
	{
		if(_tcsicmp(pe32.szExeFile, ProcessName) == 0)
		{
			// Cleanup the mess
			CloseHandle(hSnapshot);
			return pe32.th32ProcessID;
		}
	}

	// If we made it this far there wasn't a match
	// so we'll return 0
	CloseHandle(hSnapshot);
	return 0;
}

// This function will return the process id of csrss.exe
// and will do so in two different ways. If the OS is XP or 
// greater NtDll has a CsrGetProcessId otherwise I'll use 
// GetProcessIdFromName. Like other functions it will 
// return 0 on failure.
inline DWORD GetCsrssProcessId()
{
	// Don't forget to set dw.Size to the appropriate
	// size (either OSVERSIONINFO or OSVERSIONINFOEX)
	OSVERSIONINFO osinfo;
	ZeroMemory(&osinfo, sizeof(OSVERSIONINFO));
	osinfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	
	// Shouldn't fail
	GetVersionEx(&osinfo);

	// Visit http://msdn.microsoft.com/en-us/library/ms724833(VS.85).aspx
	// for a full table of versions however what I have set will
	// trigger on anything XP and newer including Server 2003
	if (osinfo.dwMajorVersion >= 5 && osinfo.dwMinorVersion >= 1)
	{
		// Gotta love functions pointers
		typedef DWORD (__stdcall *pCsrGetId)();
		
		// Grab the export from NtDll
		pCsrGetId CsrGetProcessId = (pCsrGetId)GetProcAddress(GetModuleHandle( TEXT("ntdll.dll") ), "CsrGetProcessId");

		if(CsrGetProcessId)
			return CsrGetProcessId();
		else
			return 0;
	}
	else
		return GetProcessIdFromName(TEXT("csrss.exe"));
}

// The function will attempt to open csrss.exe with 
// PROCESS_ALL_ACCESS rights if it fails we're 
// not being debugged however, if its successful we probably are
inline bool CanOpenCsrss()
{
	HANDLE Csrss = 0;

	// If we're being debugged and the process has
	// SeDebugPrivileges privileges then this call
	// will be successful, note that this only works
	// with PROCESS_ALL_ACCESS.
	Csrss = OpenProcess(PROCESS_ALL_ACCESS, FALSE, GetCsrssProcessId());

	if (Csrss != NULL)
	{
		CloseHandle(Csrss);
		return true;
	}
	else
		return false;
}



// Debug self is a function that uses CreateProcess
// to create an identical copy of the current process
// and debugs it


// HideThread will attempt to use
// NtSetInformationThread to hide a thread
// from the debugger, this is essentially the same
// as HideThreadFromDebugger. Passing NULL for
// hThread will cause the function to hide the thread
// the function is running in. Also, the function returns
// false on failure and true on success
inline bool HideThread(HANDLE hThread)
{
	typedef NTSTATUS (NTAPI *pNtSetInformationThread)
					(HANDLE, UINT, PVOID, ULONG);

	NTSTATUS Status;

	// Get NtSetInformationThread
	pNtSetInformationThread NtSIT = (pNtSetInformationThread)
		GetProcAddress(GetModuleHandle( TEXT("ntdll.dll") ),
						"NtSetInformationThread");
	// Shouldn't fail
	if (NtSIT == NULL)
		return false;

	// Set the thread info
	if (hThread == NULL)
		Status = NtSIT(GetCurrentThread(), 
						0x11, //ThreadHideFromDebugger
						0, 0);
	else
		Status = NtSIT(hThread, 0x11, 0, 0);

	if (Status != 0x00000000)
		return false;
	else
		return true;
}


// This function uses NtQuerySystemInformation
// to try to retrieve a handle to the current
// process's debug object handle. If the function
// is successful it'll return true which means we're
// being debugged or it'll return false if it fails
// or the process isn't being debugged
inline bool DebugObjectCheck()
{
	// Much easier in ASM but C/C++ looks so much better
	typedef NTSTATUS (WINAPI *pNtQueryInformationProcess)
		(HANDLE ,UINT ,PVOID ,ULONG , PULONG);

	HANDLE hDebugObject = NULL;
	NTSTATUS Status;

	// Get NtQueryInformationProcess
	pNtQueryInformationProcess NtQIP = (pNtQueryInformationProcess)
		GetProcAddress( 
		GetModuleHandle( TEXT("ntdll.dll") ), 
		"NtQueryInformationProcess" );

	Status = NtQIP(GetCurrentProcess(), 
					0x1e, // ProcessDebugObjectHandle
					&hDebugObject, 4, NULL);

	if (Status != 0x00000000)
		return false;

	if(hDebugObject)
		return true;
	else
		return false;
}

// CheckProcessDebugFlags will return true if 
// the EPROCESS->NoDebugInherit is == FALSE, 
// the reason we check for false is because 
// the NtQueryProcessInformation function returns the
// inverse of EPROCESS->NoDebugInherit so (!TRUE == FALSE)
inline bool CheckProcessDebugFlags()
{
	// Much easier in ASM but C/C++ looks so much better
	typedef NTSTATUS (WINAPI *pNtQueryInformationProcess)
		(HANDLE ,UINT ,PVOID ,ULONG , PULONG);

	DWORD NoDebugInherit = 0;
	NTSTATUS Status;

	// Get NtQueryInformationProcess
	pNtQueryInformationProcess NtQIP = (pNtQueryInformationProcess)
		GetProcAddress( 
		GetModuleHandle( TEXT("ntdll.dll") ), 
		"NtQueryInformationProcess" );

	Status = NtQIP(GetCurrentProcess(), 
		0x1f, // ProcessDebugFlags
		&NoDebugInherit, 4, NULL);

	if (Status != 0x00000000)
		return false;

	if(NoDebugInherit == FALSE)
		return true;
	else
		return false;
}


// CheckOutputDebugString checks whether or 
// OutputDebugString causes an error to occur
// and if the error does occur then we know 
// there's no debugger, otherwise if there IS
// a debugger no error will occur
inline bool CheckOutputDebugString(LPCTSTR String)
{
	OutputDebugString(String);
	if (GetLastError() == 0)
		return true;
	else
		return false;
}

// The Int2DCheck function will check to see if a debugger
// is attached to the current process. It does this by setting up
// SEH and using the Int 2D instruction which will only cause an
// exception if there is no debugger. Also when used in OllyDBG
// it will skip a byte in the disassembly and will create
// some havoc.
inline bool Int2DCheck()
{
	__try
	{
		__asm
		{
			int 0x2d
			xor eax, eax
			add eax, 2
		}
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		return false;
	}
	
	return true;
}


// ObjectListCheck uses NtQueryObject to check the environments
// list of objects and more specifically for the number of
// debug objects. This function can cause an exception (although rarely)
// so either surround it in a try catch or __try __except block
// but that shouldn't happen unless one tinkers with the function
inline bool ObjectListCheck()
{
	typedef NTSTATUS(NTAPI *pNtQueryObject)
			(HANDLE, UINT, PVOID, ULONG, PULONG);

	POBJECT_ALL_INFORMATION pObjectAllInfo = NULL;
	void *pMemory = NULL;
	NTSTATUS Status;
	unsigned long Size = 0;

	// Get NtQueryObject
	pNtQueryObject NtQO = (pNtQueryObject)GetProcAddress( 
				GetModuleHandle( TEXT( "ntdll.dll" ) ),
				"NtQueryObject" );

	// Get the size of the list
	Status = NtQO(NULL, 3, //ObjectAllTypesInformation
						&Size, 4, &Size);

	// Allocate room for the list
	pMemory = VirtualAlloc(NULL, Size, MEM_RESERVE | MEM_COMMIT, 
					PAGE_READWRITE);

	if(pMemory == NULL)
		return false;

	// Now we can actually retrieve the list
	Status = NtQO((HANDLE)-1, 3, pMemory, Size, NULL);

	// Status != STATUS_SUCCESS
	if (Status != 0x00000000)
	{
		VirtualFree(pMemory, 0, MEM_RELEASE);
		return false;
	}

	// We have the information we need
	pObjectAllInfo = (POBJECT_ALL_INFORMATION)pMemory;

	unsigned char *pObjInfoLocation = 
		(unsigned char*)pObjectAllInfo->ObjectTypeInformation;

	ULONG NumObjects = pObjectAllInfo->NumberOfObjects;

	for(UINT i = 0; i < NumObjects; i++)
	{

		POBJECT_TYPE_INFORMATION pObjectTypeInfo =
			(POBJECT_TYPE_INFORMATION)pObjInfoLocation;

		// The debug object will always be present
		if (wcscmp(L"DebugObject", pObjectTypeInfo->TypeName.Buffer) == 0)
		{
			// Are there any objects?
			if (pObjectTypeInfo->TotalNumberOfObjects > 0)
			{
				VirtualFree(pMemory, 0, MEM_RELEASE);
				return true;
			}
			else
			{
				VirtualFree(pMemory, 0, MEM_RELEASE);
				return false;
			}
		}

		// Get the address of the current entries
		// string so we can find the end
		pObjInfoLocation = 
			(unsigned char*)pObjectTypeInfo->TypeName.Buffer;

		// Add the size
		pObjInfoLocation += 
				pObjectTypeInfo->TypeName.Length;

		// Skip the trailing null and alignment bytes
		ULONG tmp = ((ULONG)pObjInfoLocation) & -4;

		// Not pretty but it works
		pObjInfoLocation = ((unsigned char*)tmp) + 
						sizeof(unsigned long);
	}

	VirtualFree(pMemory, 0, MEM_RELEASE);
	return true; 
}


// The IsDbgPresentPrefixCheck works in at least two debuggers
// OllyDBG and VS 2008, by utilizing the way the debuggers handle
// prefixes we can determine their presence. Specifically if this code
// is ran under a debugger it will simply be stepped over;
// however, if there is no debugger SEH will fire :D
inline bool IsDbgPresentPrefixCheck()
{
	__try
	{
		__asm __emit 0xF3 // 0xF3 0x64 disassembles as PREFIX REP:
		__asm __emit 0x64
		__asm __emit 0xF1 // One byte INT 1
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		return false;
	}

	return true;
}

#endif



// This function will return the process id of Explorer.exe by using the
// GetShellWindow function and the GetWindowThreadProcessId function
inline DWORD GetExplorerPIDbyShellWindow()
{
	DWORD PID = 0;
	
	// Get the PID 
	GetWindowThreadProcessId(GetShellWindow(), &PID);

	return PID;
}
