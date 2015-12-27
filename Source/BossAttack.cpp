#include "stdafx.h"
#include "BossAttack.h"
#include "GameServer.h"
#include "LogProc.h"
#include "GameMain.h"
#include "TNotice.h"
#include "..\include\readscript.h"
#include "gObjMonster.h"
//#include "GameServerAuth.h"
#include "Event.h"
#include "..\common\winutil.h"

#if (PACK_EDITION>=3)

cBossAttack BossAttack;

void cBossAttack::StartEvent()
{
	if((this->Start == 0) && (this->Enabled == 1))
		_beginthread( cBossAttack__InsideTrigger, 0, NULL  );
}

void cBossAttack::Init(char * FilePath)
{
	this->Start					= 0;
	this->Enabled				= GetPrivateProfileInt("Common", "SCFBossAttackEnabled",0, FilePath) ;
	this->Minutes				= GetPrivateProfileInt("Common", "SCFBossAttackMinutes",5, FilePath) ;
	LogAddTD("[SCF BossAttack] - %s file is Loaded",FilePath);
}

void cBossAttack__InsideTrigger(void * lpParam)
{
	int Seconds=0;
	BossAttack.BossDie = 0;
	BossAttack.Start = 1;

	LogAddTD("[BossAttack] Event Start");
	AllSendServerMsg(lMsg.Get(MSGGET(14, 4)));
	BossAttack.SetMonsters();
	while(true)
	{
		if(Seconds < (BossAttack.Minutes * 60))
		{
			Sleep(1000);
			if(BossAttack.BossDie == 1)
				break;
		}else
		{
			break;
		}
		Seconds++;
	}

	BossAttack.ClearMonsters();
	LogAddTD("[BossAttack] Event End");
	AllSendServerMsg(lMsg.Get(MSGGET(14, 5)));

	BossAttack.Start = 0;
	_endthread();
}

void cBossAttack::ClearMonsters()
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
	this->BossID = 0;
}

void cBossAttack::SetMonsters()
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
				gObjSetMonster(MobID, gMSetBase.m_Mp[this->Monster[i].Pos].m_Type,"cBossAttack::SetMonsters");

				if(this->Monster[i].Boss == 1)
					this->BossID = this->Monster[i].ID;
			}
		}
	}
}

void cBossAttack::ReadMonsters(char * FilePath)
{
	if(this->Enabled == 1)
	{
		int Token;
		int MonsterNumber;
		int Map;
		int X;
		int Y;
		int Count;
		this->MonsterCount = 0;
		this->BossID = 0;

		for (int iM = 0; iM < 256; iM++)
		{
			this->Monster[iM].ID = -1;
			this->Monster[iM].Pos = -1;
			this->Monster[iM].Number = -1;
			this->Monster[iM].Boss = -1;
		}

		SMDFile = fopen(FilePath, "r");

		if ( SMDFile == NULL )
		{
			MsgBox("BossAttack data load error %s", FilePath);
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
					MonsterNumber = TokenNumber;

					Token = GetToken();
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
							int MobPos = gObjMonsterAdd(MonsterNumber,Map,X,Y);
							if(MobPos >= 0)
							{
								this->Monster[this->MonsterCount].ID = -1;
								this->Monster[this->MonsterCount].Pos = MobPos;
								this->Monster[this->MonsterCount].Number = MonsterNumber;
								this->Monster[this->MonsterCount].Boss = 0;
								this->MonsterCount++;
							}
						} else {
							LogAddTD("[BossEvent][Overflow] - Cant add monster to Map:%d:, X:%d, Y:%d, Mob:%d, Boss:%d",
								Map,X,Y,MonsterNumber,0);
						}
					}
				}
			}
			
			iType = GetToken();
			iType = TokenNumber;
			if ( iType == 2 )
			{
				while(true)
				{
					Token = GetToken();
					if ( strcmp("end", TokenString) == 0 )
					{
						break;
					}
					MonsterNumber = TokenNumber;

					Token = GetToken();
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
							int MobPos = gObjMonsterAdd(MonsterNumber,Map,X,Y);
							if(MobPos >= 0)
							{
								this->Monster[this->MonsterCount].ID = -1;
								this->Monster[this->MonsterCount].Pos = MobPos;
								this->Monster[this->MonsterCount].Number = MonsterNumber;
								this->Monster[this->MonsterCount].Boss = 1;
								this->MonsterCount++;
							}
						} else {
							LogAddTD("[BossEvent][Overflow] - Cant add monster to Map:%d:, X:%d, Y:%d, Mob:%d, Boss:%d",
								Map,X,Y,MonsterNumber,1);
						}
					}
				}
			}
			break;
		}
			
		LogAddTD("[BossAttack] - %s file is Loaded",FilePath);
		fclose(SMDFile);
	}
}

#endif