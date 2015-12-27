#include "stdafx.h"
#include "GameServer.h"
#include "LogProc.h"
#include "GameMain.h"
#include "TNotice.h"
#include "..\include\readscript.h"
//#include "GameServerAuth.h"
#include "..\common\winutil.h"
#include "GMSystem.h"

cGMSystem GMSystem;

void cGMSystem::CheckStatus(LPOBJ pObj)
{
#if (PACK_EDITION>=3)
	if(this->Enabled==1)
	{
		char sBuf[1024] = {0};

		time_t _time;
		time (&_time);
		struct tm* _tm = localtime (&_time);

		int year = _tm->tm_year + 1900;
		int month = _tm->tm_mon + 1;
		int wday = _tm->tm_wday;
		int day = _tm->tm_mday;

		for (int i=0;i < this->Count;i++)
		{
			if(strcmp(this->GMpObj[i].Name,pObj->Name)==0)
			{
				if (this->GMpObj[i].Year>=year)
				{
					if ((this->GMpObj[i].Year>year) || ((this->GMpObj[i].Month>month) || ((this->GMpObj[i].Month==month) && (this->GMpObj[i].Day>=day))))
					{
						wsprintf(sBuf, "[GMSystem][%s] Promoted to GM! Expiry: %d-%d-%d",this->GMpObj[i].Name,this->GMpObj[i].Day,this->GMpObj[i].Month,this->GMpObj[i].Year);
						LogAddTD(sBuf);
						GCServerMsgStringSend(sBuf,pObj->m_Index, 0x01);

						if(this->Announce == TRUE)
						{
							wsprintf(sBuf, "[GMSystem] %s is ONLINE",this->GMpObj[i].Name);
							AllSendServerMsg(sBuf);
						}

						GCSkillInfoSend(pObj, 1, 28);

						//Set the character to the GM
						pObj->Authority = this->GMpObj[i].CtlCode;
						pObj->AuthorityCode = 0x3F;
					}
				}
			}
		}
	}
#endif
}

BOOL cGMSystem::CheckCommand(LPOBJ pObj,int Command)
{
#if (PACK_EDITION>=3)
	if(this->Enabled==1)
	{
		char sBuf[1024] = {0};

		time_t _time;
		time (&_time);
		struct tm* _tm = localtime (&_time);

		int year = _tm->tm_year + 1900;
		int month = _tm->tm_mon + 1;
		int wday = _tm->tm_wday;
		int day = _tm->tm_mday;

		if(pObj->AuthorityCode == 0x3F)
		{
			for (int i=0;i<Count;i++)
			{
				if(strcmp(this->GMpObj[i].Name,pObj->Name)==0)
				{
					if (this->GMpObj[i].Year>=year)
					{
						if ((this->GMpObj[i].Year>year) || ((this->GMpObj[i].Month>month) || ((this->GMpObj[i].Month==month) && (this->GMpObj[i].Day>=day))))
						{
							if(this->GMpObj[i].Command[Command] == TRUE)
							{
								return TRUE;
							}else
							{
								wsprintf(sBuf, "[GMSystem][%s] You cant use that command (%d)!",this->GMpObj[i].Name,Command);
								LogAddTD(sBuf);
								GCServerMsgStringSend(sBuf,pObj->m_Index, 0x01);
								return FALSE;
							}
						}
						return FALSE;
					}
				}
			}
		}
	}
#endif
	return FALSE;
}

void cGMSystem::ReadConfig(char * FilePath)
{
#if (PACK_EDITION>=3)
	if(this->Enabled == 1)
	{
		memset(&GMpObj[0],0,sizeof(GMpObj));
		this->Count=0;
		int Token;

		SMDFile = fopen(FilePath, "r");

		if ( SMDFile == NULL )
		{
			MsgBox("GMSystem data load error %s", FilePath);
			return;
		}

		while ( true )
		{
			int iType = GetToken();
			
			if ( iType == 1 )
			{
				while(true)
				{
					Token = GetToken();
					if ( strcmp("end", TokenString) == 0 )
					{
						break;
					}
					strcpy(GMpObj[this->Count].Name,TokenString);

					Token = GetToken();
					GMpObj[this->Count].CtlCode = TokenNumber;

					Token = GetToken();
					GMpObj[this->Count].Day = TokenNumber;

					Token = GetToken();
					GMpObj[this->Count].Month = TokenNumber;

					Token = GetToken();
					GMpObj[this->Count].Year = TokenNumber;

					for(int i=0;i<GMCommands;i++)
					{
						Token = GetToken();
						GMpObj[this->Count].Command[i] = TokenNumber;
					}

					LogAddTD("[GMSystem] Loaded: %s[%d] [exp:%d-%d-%d][cmd:%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d]",
						GMpObj[this->Count].Name,GMpObj[this->Count].CtlCode,
						GMpObj[this->Count].Day,GMpObj[this->Count].Month,GMpObj[this->Count].Year,
						GMpObj[this->Count].Command[0],GMpObj[this->Count].Command[1],GMpObj[this->Count].Command[2],GMpObj[this->Count].Command[3],
						GMpObj[this->Count].Command[4],GMpObj[this->Count].Command[5],GMpObj[this->Count].Command[6],GMpObj[this->Count].Command[7],
						GMpObj[this->Count].Command[8],GMpObj[this->Count].Command[9],GMpObj[this->Count].Command[10],GMpObj[this->Count].Command[11],
						GMpObj[this->Count].Command[12],GMpObj[this->Count].Command[13],GMpObj[this->Count].Command[14],GMpObj[this->Count].Command[15],
						GMpObj[this->Count].Command[16],GMpObj[this->Count].Command[17],GMpObj[this->Count].Command[18],GMpObj[this->Count].Command[19]);

					this->Count++;
				}
			}
			break;
		}
			
		fclose(SMDFile);
		LogAddTD("[GMSystem] - %s file is Loaded",FilePath);
	}
#endif
}