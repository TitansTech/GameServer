// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)
#define AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#define _WIN32_WINNT 0x500

// Windows Header Files:
#include <afx.h> //
#include <windows.h>
#include <winbase.h>
#include <winsock2.h>
#include "COMMCTRL.h"

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <time.h>
#include <math.h>
#include <process.h>

#include <map>
#include <string>
#include <ios>
#include <algorithm>
#include <vector>
#include <set>

// Local Header Files
#include "MuMsg.h"

// TODO: reference additional headers your program requires here
#pragma comment( lib , "wsock32.lib" )
#pragma comment( lib , "ws2_32.lib" )
#pragma comment( lib , "COMCTL32.lib" )

#include "ReadConfig.h"
#include "Version.h"
#include "DSGN.h"


#if (WL_PROTECT==1)
#include "Extra\\WinLicenseSDK.h"
//#pragma comment (lib,"Extra\\WinLicenseSDK.lib")
#ifdef _WIN64
#pragma comment( lib , "wldap32.lib" )
#pragma comment (lib,"Extra\\curllib_x64.lib")
#else
#pragma comment (lib,"Extra\\curllib.lib")
#endif
#endif
#pragma warning (disable:4100 4213 4310 4244 4245 4389 4701 4706)
#pragma warning (disable:4189)	// Disable Local variable is initialised but not referenced
#pragma warning (disable:4702)	// Disable Unreachable Code
#pragma warning (disable:4786)	// Disable Warning of Large Debuf Strings ( truncated to 255 len )
#pragma warning (disable:4102 4805 4646 4018 4532 4258 4482 4172 4554 4996 4312 4101 4309 4018 4076 4098 4309 4305 4311 4312 4267 4995 4700 4733 4793 4099)

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)
