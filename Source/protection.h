#ifndef PROTECTION_H
#define PROTECTION_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#if (MEM_PROTECT==1)

extern int imageBaseOfGS;
extern BYTE replacementPart;
extern int replacementRange;
extern BYTE executedParentSearch;
extern BYTE processesFound;
extern int removeFunctionsArray[255];
extern BYTE jmpLength;
extern int functionMangleApiMain;
extern int functionMangleInit;

DWORD WINAPI GetParentPIDById( DWORD crtpid );
DWORD GetProcessByFileName(char* name);
void CheckProcessByFileName(LPCTSTR ProcessName);
ULONG_PTR GetParentProcessId();

#endif
#endif