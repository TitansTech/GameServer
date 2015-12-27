#include "stdafx.h"
#include "../Source/GameMain.h"
#include "../Source/DSProtocol.h"
#include "../Source/StdAfx.h"
#include <Iphlpapi.h>
#include <Assert.h>
#pragma comment(lib, "iphlpapi.lib")
#include <rpc.h>
#include <rpcdce.h>
#pragma comment(lib, "rpcrt4.lib")

int getTotalRAM()
{
    int ret = 0;
#ifdef EXPERIMENTAL_TOOLS
    DWORD v = GetVersion();
    DWORD major =  (DWORD)(LOBYTE(LOWORD(v)));
    DWORD minor =  (DWORD)(HIBYTE(LOWORD(v)));
    DWORD build;
    if (v < 0x80000000) build = (DWORD)(HIWORD(v));
    else build = 0;

    // because compiler static links the function...
    BOOL (__stdcall*GMSEx)(LPMEMORYSTATUSEX) = 0;

    HINSTANCE hIL = LoadLibrary(L"kernel32.dll");
    GMSEx = (BOOL(__stdcall*)(LPMEMORYSTATUSEX))GetProcAddress(hIL, "GlobalMemoryStatusEx");

    if(GMSEx)
    {
        MEMORYSTATUSEX m;
        m.dwLength = sizeof(m);
        if(GMSEx(&m))
        {
                ret = (int)(m.ullTotalPhys>>20);
        }
    }
    else
    {
        MEMORYSTATUS m;
        m.dwLength = sizeof(m);
        GlobalMemoryStatus(&m);
        ret = (int)(m.dwTotalPhys>>20);
    }
#endif
    return ret;
}

int getAvailRAM()
{
    int ret = 0;
#ifdef EXPERIMENTAL_TOOLS
    DWORD v = GetVersion();
    DWORD major =  (DWORD)(LOBYTE(LOWORD(v)));
    DWORD minor =  (DWORD)(HIBYTE(LOWORD(v)));
    DWORD build;
    if (v < 0x80000000) build = (DWORD)(HIWORD(v));
    else build = 0;

    // because compiler static links the function...
    BOOL (__stdcall*GMSEx)(LPMEMORYSTATUSEX) = 0;

    HINSTANCE hIL = LoadLibrary(L"kernel32.dll");
    GMSEx = (BOOL(__stdcall*)(LPMEMORYSTATUSEX))GetProcAddress(hIL, "GlobalMemoryStatusEx");

    if(GMSEx)
    {
        MEMORYSTATUSEX m;
        m.dwLength = sizeof(m);
        if(GMSEx(&m))
        {
                ret = (int)(m.ullAvailPhys>>20);
        }
    }
    else
    {
        MEMORYSTATUS m;
        m.dwLength = sizeof(m);
        GlobalMemoryStatus(&m);
        ret = (int)(m.dwAvailPhys>>20);
    }
#endif
    return ret;
}

int getTotalMemory()
{
    int ret = 0;
#ifdef EXPERIMENTAL_TOOLS
    DWORD v = GetVersion();
    DWORD major =  (DWORD)(LOBYTE(LOWORD(v)));
    DWORD minor =  (DWORD)(HIBYTE(LOWORD(v)));
    DWORD build;
    if (v < 0x80000000) build = (DWORD)(HIWORD(v));
    else build = 0;

    // because compiler static links the function...
    BOOL (__stdcall*GMSEx)(LPMEMORYSTATUSEX) = 0;

    HINSTANCE hIL = LoadLibrary(L"kernel32.dll");
    GMSEx = (BOOL(__stdcall*)(LPMEMORYSTATUSEX))GetProcAddress(hIL, "GlobalMemoryStatusEx");

    if(GMSEx)
    {
        MEMORYSTATUSEX m;
        m.dwLength = sizeof(m);
        if(GMSEx(&m))
        {
                ret = (int)(m.ullTotalPhys>>20) + (int)(m.ullTotalVirtual>>20);
        }
    }
    else
    {
        MEMORYSTATUS m;
        m.dwLength = sizeof(m);
        GlobalMemoryStatus(&m);
        ret = (int)(m.dwTotalPhys>>20) + (int)(m.dwTotalVirtual>>20);
    }
#endif
    return ret;
}

int getAvailMemory()
{
    int ret = 0;
#ifdef EXPERIMENTAL_TOOLS
    DWORD v = GetVersion();
    DWORD major =  (DWORD)(LOBYTE(LOWORD(v)));
    DWORD minor =  (DWORD)(HIBYTE(LOWORD(v)));
    DWORD build;
    if (v < 0x80000000) build = (DWORD)(HIWORD(v));
    else build = 0;

    // because compiler static links the function...
    BOOL (__stdcall*GMSEx)(LPMEMORYSTATUSEX) = 0;

    HINSTANCE hIL = LoadLibrary(L"kernel32.dll");
    GMSEx = (BOOL(__stdcall*)(LPMEMORYSTATUSEX))GetProcAddress(hIL, "GlobalMemoryStatusEx");

    if(GMSEx)
    {
        MEMORYSTATUSEX m;
        m.dwLength = sizeof(m);
        if(GMSEx(&m))
        {
                ret = (int)(m.ullAvailPhys>>20) + (int)(m.ullAvailVirtual>>20);
        }
    }
    else
    {
        MEMORYSTATUS m;
        m.dwLength = sizeof(m);
        GlobalMemoryStatus(&m);
        ret = (int)(m.dwAvailPhys>>20) + (int)(m.dwAvailVirtual>>20);
    }
#endif
    return ret;
}