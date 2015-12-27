#include "stdafx.h"
#include "SummerEvent.h"
#include "GameServer.h"
#include "LogProc.h"
#include "GameMain.h"
#include "TNotice.h"
#include "..\include\readscript.h"
#include "gObjMonster.h"
//#include "GameServerAuth.h"
#include "Event.h"
#include "CashShop.h"
#include "..\common\winutil.h"

#if (PACK_EDITION>=1)

cSummerEvent SummerEvent;

void cSummerEvent::StartEvent()
{
	#if (WL_PROTECT==1)
		VM_START_WITHLEVEL(2)
		g_CashShop.SystemProcessesScan();
		VM_END
	#endif

	if((this->Start == 0) && (this->Enabled == 1))
		_beginthread( cSummerEvent__InsideTrigger, 0, NULL  );
}

void cSummerEvent::Init(char * FilePath)
{
	this->Start					= 0;
	this->Enabled				= GetPrivateProfileInt("Common", "SCFSummerEventEnabled",0, FilePath) ;
	this->Minutes				= GetPrivateProfileInt("Common", "SCFSummerEventMinutes",5, FilePath) ;
	LogAddTD("[SCF SummerEvent] - %s file is Loaded",FilePath);
}

void cSummerEvent__InsideTrigger(void * lpParam)
{
	SummerEvent.Start = 1;

	LogAddTD("[SummerEvent] Event Start");
	AllSendServerMsg(lMsg.Get(MSGGET(14, 135)));
	SummerEvent.SetMonsters();
	Sleep(SummerEvent.Minutes * 60000);
	SummerEvent.ClearMonsters();
	LogAddTD("[SummerEvent] Event End");
	AllSendServerMsg(lMsg.Get(MSGGET(14, 136)));

	SummerEvent.Start = 0;
	_endthread();
}

void cSummerEvent::ClearMonsters()
{
	if(this->Enabled == 1 && this->Start == 1)
	{
		for(int i=0;i<this->MonsterCount;i++)
		{
			if (this->Monster[i].ID >= 0)
			{
				if (this->Monster[i].Number = gObj[this->Monster[i].ID].Class &&
					gMSetBase.m_Mp[this->Monster[i].Pos].m_MapNumber == gObj[this->Monster[i].ID].MapNumber)
				{
					gObjDel(this->Monster[i].ID);
				}

				this->Monster[i].ID = -1;
			}
		}
	}
}

void cSummerEvent::SetMonsters()
{
	if(this->Enabled == 1 && this->Start == 1)
	{
		for(int i=0; i<this->MonsterCount;i++)
		{			
			int MobID = gObjAddMonster(gMSetBase.m_Mp[this->Monster[i].Pos].m_MapNumber);
			if(MobID >= 0)
			{
				this->Monster[i].ID = MobID;
				gObjSetPosMonster(MobID, this->Monster[i].Pos);
				gObjSetMonster(MobID, gMSetBase.m_Mp[this->Monster[i].Pos].m_Type,"cSummerEvent::SetMonsters");
			}
		}
	}
}

void cSummerEvent::ReadMonsters(char * FilePath)
{
	if(this->Enabled == 1)
	{
		int Token;
		int Map;
		int X;
		int Y;
		int Count;
		this->MonsterCount = 0;

		for (int iM = 0; iM < 256; iM++)
		{
			this->Monster[iM].ID = -1;
			this->Monster[iM].Pos = -1;
			this->Monster[iM].Number = -1;
		}

		SMDFile = fopen(FilePath, "r");

		if ( SMDFile == NULL )
		{
			MsgBox("SummerEvent data load error %s", FilePath);
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
					Map = TokenNumber;

					Token = GetToken();
					X = TokenNumber;

					Token = GetToken();
					Y = TokenNumber;

					Token = GetToken();
					Count = TokenNumber;

					for(int i=0;i<Count;i++)
					{
						if (this->MonsterCount < 256)
						{
							int MobPos = gObjMonsterAdd(463,Map,X,Y);
							if(MobPos >= 0)
							{
								this->Monster[this->MonsterCount].ID = -1;
								this->Monster[this->MonsterCount].Pos = MobPos;
								this->Monster[this->MonsterCount].Number = 463;
								this->MonsterCount++;
							}
						} else {
							LogAddTD("[SummerEvent][Overflow] - Cant add monster to Map:%d:, X:%d, Y:%d",
								Map,X,Y);
						}
					}
				}
			}
			break;
		}
			
		fclose(SMDFile);
		LogAddTD("[SummerEvent] - %s file is Loaded",FilePath);
	}
}

#endif