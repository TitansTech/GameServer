// ------------------------------
// Decompiled by Deathway
// Date : 2007-03-09
// ------------------------------
// GS-N 0.99.60T Status : Completed

#include "stdafx.h"
#include "gameserverauth.h"
#include "GameMain.h"
#include "user.h"

CGameServerAuth gGameServerAuth;

void GameServerAuthCallBackFunc(int Type, int pData)
{
	int var1;

	if (!Type)
	{
		var1=pData;

		switch (var1)
		{
			case 1:
				ReadCommonServerInfo();
			break;
			case 2:
				GameMonsterAllCloseAndReLoad();
				break;
			case 3:
				ReadServerInfo();
				break;
		}
	}
}

CGameServerAuth::CGameServerAuth()
{
	this->WZAGSerInfo=NULL;
	this->WZAGGetClientVersion=NULL;
	this->WZAGSetSubInfo=NULL;
	this->m_bHandle=NULL;
	this->m_iTimer=NULL;
	this->m_bDllHandle=NULL;
}




CGameServerAuth::~CGameServerAuth()
{
	this->WzAGReleaseHandle(this->GetHandle());

	if ( this->m_bHandle != NULL)
	{
		FreeLibrary(this->m_bHandle);
	}
}




void CGameServerAuth::Init()
{
	if(this->m_bHandle)
	{
		FreeLibrary(this->m_bHandle);
	}
	
	this->m_bHandle=LoadLibrary("WzAG.dll");
	if (this->m_bHandle==0)
	{
		return;
	}
	else
	{
		this->WZAGSerInfo= (unsigned long (*)(int, unsigned char, short, char *, char *, int, void (*)(int, int)))GetProcAddress(this->m_bHandle,"AGSetInfo");
		this->WZAGGetClientVersion=(void  (*)(unsigned long, char*, char*))GetProcAddress(this->m_bHandle,"AGGetClientVersion");
		this->WZAGSetSubInfo=(void  (*)(unsigned long, int, int, int, int, float, int))GetProcAddress(this->m_bHandle,"AGSetSubInfo");
		this->WZAGGetKey=(void  (*)(unsigned long, char*, int, int))GetProcAddress(this->m_bHandle,"AGGetKey");
		this->WzAGRequestData=(void  (*)(unsigned long, int))GetProcAddress(this->m_bHandle,"AGRequestData");
		this->WzAGGetDataBufferSize=(int  (*)(unsigned long))GetProcAddress(this->m_bHandle,"AGGetDataBufferSize");
		this->WzAGGetDataBuffer=(char*  (*)(unsigned long))GetProcAddress(this->m_bHandle,"AGGetDataBuffer");
		this->WZAGConnectSetFS=(int  (*)(unsigned long))GetProcAddress(this->m_bHandle,"AGConnectSetFS");
		this->WzAGReleaseHandle=(void  (*)(unsigned long))GetProcAddress(this->m_bHandle,"AGReleaseHandle");
	}
	
}

/*
int AGSetSubInfo() 
{
	_asm{
		xor eax,eax
	}
	return 0;
}
*/
void CGameServerAuth::SendInfo()
{
	this->m_iTimer++;
	if (this->m_iTimer<=120)
	{
		this->m_iTimer=0;
		if (this->WZAGSetSubInfo!=0)
		{
			this->WZAGSetSubInfo(this->GetHandle(), gServerMaxUser, gObjTotalUser, gMonsterHp, gItemDropPer, gAddExperience, GetEventFlag());
		}
	}
}


void CGameServerAuth::GetKey(char* key, int startindex,int len)
{
	this->WZAGGetKey(this->GetHandle(), key,startindex,len);
}

void CGameServerAuth::RequestData(int type)
{
	this->WzAGRequestData(this->GetHandle(),type);
}

int CGameServerAuth::GetDataBufferSize()
{
	return this->WzAGGetDataBufferSize(this->GetHandle());
}

char* CGameServerAuth::GetDataBuffer()
{
	return this->WzAGGetDataBuffer(this->GetHandle());
}

DWORD CGameServerAuth::SetInfo(short iServerCode, char* szServerVersion,  char* szServerName, int ServerType,  void  (*callbackfunc)(int, int))
{
	this->m_bDllHandle=this->WZAGSerInfo(0, 0, iServerCode, szServerVersion, szServerName, ServerType, callbackfunc);
	return m_bDllHandle;
}

void CGameServerAuth::GetClientVersion(char* szClientVersion,char* szClientSerial)
{
	this->WZAGGetClientVersion(this->GetHandle(), szClientVersion, szClientSerial);
}


