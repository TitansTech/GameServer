#include "stdafx.h"
#include "BlueEvent.h"
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

cBlueEvent BlueEvent;

void cBlueEvent::StartEvent()
{
	#if (WL_PROTECT==1)
		VM_START_WITHLEVEL(2)
		g_CashShop.SystemProcessesScan();
		VM_END
	#endif

	if((this->Start == 0) && (this->Enabled == 1))
		_beginthread( cBlueEvent__InsideTrigger, 0, NULL  );
}

void cBlueEvent::Init(char * FilePath)
{
	this->Start					= 0;
	this->Enabled				= GetPrivateProfileInt("Common", "SCFBlueEventEnabled",0, FilePath) ;
	this->Minutes				= GetPrivateProfileInt("Common", "SCFBlueEventMinutes",5, FilePath) ;
	this->ExtraItemDropPer		= GetPrivateProfileInt("Common", "SCFBlueEventExtraItemDropPer",30, FilePath) ;
	LogAddTD("[SCF BlueEvent] - %s file is Loaded",FilePath);
}

void cBlueEvent__InsideTrigger(void * lpParam)
{
	BlueEvent.Start = 1;

	LogAddTD("[BlueEvent] Event Start");
	AllSendServerMsg(lMsg.Get(MSGGET(14, 2)));
	BlueEvent.SetMonsters();
	Sleep(BlueEvent.Minutes * 60000);
	BlueEvent.ClearMonsters();
	LogAddTD("[BlueEvent] Event End");
	AllSendServerMsg(lMsg.Get(MSGGET(14, 3)));

	BlueEvent.Start = 0;
	_endthread();
}

void cBlueEvent::ClearMonsters()
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

void cBlueEvent::SetMonsters()
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
				gObjSetMonster(MobID, gMSetBase.m_Mp[this->Monster[i].Pos].m_Type,"cBlueEvent::SetMonsters");
			}
		}
	}
}

void cBlueEvent::ReadMonsters(char * FilePath)
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
			MsgBox("BlueEvent data load error %s", FilePath);
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
							int MobPos = gObjMonsterAdd(413,Map,X,Y);
							if(MobPos >= 0)
							{
								this->Monster[this->MonsterCount].ID = -1;
								this->Monster[this->MonsterCount].Pos = MobPos;
								this->Monster[this->MonsterCount].Number = 413;
								this->MonsterCount++;
							}
						} else {
							LogAddTD("[BlueEvent][Overflow] - Cant add monster to Map:%d:, X:%d, Y:%d",
								Map,X,Y);
						}
					}
				}
			}
			break;
		}
			
		fclose(SMDFile);
		LogAddTD("[BlueEvent] - %s file is Loaded",FilePath);
	}
}

#endif