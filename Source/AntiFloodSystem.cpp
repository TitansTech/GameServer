#include "stdafx.h"
#include "LogProc.h"
#include "User.h"
#include "LogToFile.h"
#include "..\include\readscript.h"
#include "AntiFloodSystem.h"

cAntiFlood AntiFlood;

void cAntiFlood::Init()
{
	this->Enabled			= GetPrivateProfileInt("Common", "SCFAntiFloodEnabled",0, BLISTCFGFILE) ;
	this->IsAutoAdd			= GetPrivateProfileInt("Common", "SCFAutoAddToBlackList",0, BLISTCFGFILE) ;
	this->MaxIPConnection	= GetPrivateProfileInt("Common", "SCFMaxIPConnection",20, BLISTCFGFILE) ;
	this->AutoCloseFloodCon	= GetPrivateProfileInt("Common", "SCFAutoCloseAllFloodConnection",0, BLISTCFGFILE) ;
	this->ReadBlackList(BLISTFILE);
}

void cAntiFlood::BlackListClean()
{
	for(int i=0;i<=MAXIPINBLACKLIST;i++)
	{
		memset(this->BlackListIP[i],0,sizeof(this->BlackListIP[i])-1);
	}
}

void cAntiFlood::ReadBlackList(LPSTR filename)
{
	this->BlackListClean();

	int Token;
	this->BLSize=0;

	SMDFile = fopen(filename, "r");

	if ( SMDFile == NULL )
	{
		MsgBox("Error reading file %s", filename);
		exit(1);
	}

	while ( true )
	{
		Token = GetToken();

		if ( Token == 2 )
		{
			break;
		}

		//if ( Token == 1 )
		//{
		if(this->AddToBlackList(TokenString) == false)
		{
			MsgBox("error-L3 : Max IP count in BlackList reached!");
			exit(1);
		}
		//}
	}

	fclose(SMDFile);
	LogAddC(3, "[BlackList] Loaded Successfull!");
}

int cAntiFlood::GetIPCount(char * IP)
{
	int Count = 0;

	for (int n=OBJ_STARTUSERINDEX;n<OBJMAX; n++)
	{
		if ( gObj[n].Connected >= PLAYER_CONNECTED)
		{
			if ( strcmp(&gObj[n].Ip_addr[0] , IP) == 0)
			{
				Count++;
			}
		}
	}
	return Count;
}

void cAntiFlood::AutoClose(char * IP)
{
	for (int n=OBJ_STARTUSERINDEX;n<OBJMAX; n++)
	{
		if ( gObj[n].Connected >= PLAYER_CONNECTED)
		{
			if ( strcmp(&gObj[n].Ip_addr[0] , IP) == 0)
			{
				LogAddC(2,"[AutoClose] Index: %d",n);
				CloseClient(n);
			}
		}
	}
}

bool cAntiFlood::Check(char * IP)
{
	if(this->Enabled == TRUE)
	{
		if(this->BL_IPCheck(IP) == false)
		{
			LogAddC(2,"[BlackList] Rejected IP %s",IP);
			return false;
		}
		if(this->MaxIPConnection < this->GetIPCount(IP))
		{
			if(this->IsAutoAdd == 1)
			{
				if(this->AddToBlackList(IP) == true)
				{
					WriteTxt(BLISTFILE,IP);
					LogAddC(2,"[BlackList] IP: %s Added to Black List - Flood Attempt: %d",IP,this->MaxIPConnection);
				}else
				{
					LogAddC(2,"[BlackList][BLACKLIST FULL] IP: %s Fail Add to Black List - Flood Attempt: %d",IP,this->MaxIPConnection);
				}
				if(this->AutoCloseFloodCon == 1)
				{
					this->AutoClose(IP);
				}
			}
			return false;
		}
	}
	return true;
}

bool cAntiFlood::BL_IPCheck(char * IP)
{
	for(int i=0;i<this->BLSize;i++)
		if(this->BlackListIP[i][0] != 0)
			if(!stricmp(IP,this->BlackListIP[i]))
				return false;
	return true;
}

bool cAntiFlood::AddToBlackList(LPSTR IP)
{
	if(this->BLSize >= MAXIPINBLACKLIST)
	{
		return false;
	}
	strcpy(this->BlackListIP[this->BLSize],IP);
	this->BLSize++;
	return true;
}