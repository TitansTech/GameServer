#include "stdafx.h"
#include "GameServer.h"
#include "LogProc.h"
#include "GameMain.h"
#include "SkyEvent.h"
#include "TNotice.h"
#include "..\include\readscript.h"
#include "gObjMonster.h"
//#include "GameServerAuth.h"
#include "MapServerManager.h"
#include "DSProtocol.h"
#include "Event.h"
#include "..\common\winutil.h"
#include "DSProtocol.h"


#if (PACK_EDITION>=2)

cSkyEvent SkyEvent;


BOOL SKYEVENT_MAP_RANGE(int Map, BYTE X)
{
	if (Map != 10)
	{
		return FALSE;
	}else
	{
		if(X >= 130)
			return TRUE;
	}
	return FALSE;
}


void cSkyEvent::ReadINI(char * FilePath)
{
	char sBuf[255]={0};
	char sBuf2[255]={0};
	char sBuf3[255]={0};
	
	this->Start		= 0;
	this->Enabled		= GetPrivateProfileInt("Common", "SCFSkyEventEnabled",0, FilePath) ;
		
	for(int iLevel = 0; iLevel < MAX_SKY_EVENT_LEVEL; iLevel++)
	{		
		this->PlayerCount[iLevel] = 0;
		wsprintf(sBuf,"Level %d",iLevel+1);

		this->Level_Min[iLevel]			= GetPrivateProfileInt(sBuf, "SCFSkyEventMinLevelReq", 1, FilePath);
		this->Level_Max[iLevel]			= GetPrivateProfileInt(sBuf, "SCFSkyEventMaxLevelReq", 1000, FilePath);
		this->ItemWinType[iLevel]		= GetPrivateProfileInt(sBuf, "SCFSkyEventItemType", 14, FilePath);
		this->ItemWinIndex[iLevel]		= GetPrivateProfileInt(sBuf, "SCFSkyEventItemIndex", 14, FilePath);
		this->ItemWinLevel[iLevel]		= GetPrivateProfileInt(sBuf, "SCFSkyEventItemLevel", 0, FilePath);
		this->ItemWinDur[iLevel]		= GetPrivateProfileInt(sBuf, "SCFSkyEventItemOpt", 0, FilePath);
		this->ItemWinLuck[iLevel]		= GetPrivateProfileInt(sBuf, "SCFSkyEventItemLuck", 0, FilePath);
		this->ItemWinSkill[iLevel]		= GetPrivateProfileInt(sBuf, "SCFSkyEventItemSkill", 0, FilePath);
		this->ItemWinOpt[iLevel]		= GetPrivateProfileInt(sBuf, "SCFSkyEventItemDur", 1, FilePath);
		this->ItemWinExc[iLevel]		= GetPrivateProfileInt(sBuf, "SCFSkyEventItemIsExc", 0, FilePath);
	}
	for(int iStage = 0 ; iStage < MAX_SKY_EVENT_STAGES; iStage++)
	{
		wsprintf(sBuf2,"SCFSkyEventExtraExpStage%d",iStage+1);
		wsprintf(sBuf3,"SCFSkyEventMinutesStage%d",iStage+1);
		this->StageMinutes[iStage]		= GetPrivateProfileInt("Common", sBuf3, 0, FilePath);
		for(int iLevel = 0; iLevel < MAX_SKY_EVENT_LEVEL; iLevel++)
		{
			wsprintf(sBuf,"Level %d",iLevel+1);
			this->ExtraExp[iLevel][iStage] = GetPrivateProfileInt(sBuf, sBuf2, (10*(iStage+1)), FilePath);
		}
	}
	LogAddCTD(3,"[SCF SkyEvent] - %s file is Loaded",FilePath);
}

int cSkyEvent::GetWinner(BYTE iLevel)
{
	int MaxKillMob = 0;
	int WinUser = -1;
	//if(this->ActiveLevel[iLevel] == TRUE)
	//{
		for(int iUser = 0; iUser < MAX_SKY_EVENT_SUB_LEVEL; iUser++)
		{
			if(this->m_UserData[iLevel][iUser].m_Index > -1)
			{
				int PlayerKillMobs = 0;
				for(int iStage = 0;iStage < MAX_SKY_EVENT_STAGES; iStage++)
				{
					PlayerKillMobs += (this->m_UserData[iLevel][iUser].KillCount[iStage] * (iStage+1));
				}
				if(PlayerKillMobs > MaxKillMob)
				{
					MaxKillMob = PlayerKillMobs;
					WinUser = iUser;
				}
			}			
		}
	//}
	return WinUser;
}

int cSkyEvent::GetPlayerSubIndex(LPOBJ lpObj, BYTE iLevel)
{
	for(int iUser = 0; iUser < MAX_SKY_EVENT_SUB_LEVEL; iUser++)
	{
		int m_Index = this->m_UserData[iLevel][iUser].m_Index;
		
		if(m_Index == lpObj->m_Index)
		{
			return iUser;
		}
	}
	return -1;
}

int cSkyEvent::GetLevel(LPOBJ lpObj)
{
	int Err = -1;
	if((this->Enabled == 1) && (this->Start == 1))
	{
		if((lpObj->MapNumber == MAP_INDEX_ICARUS) && (lpObj->X >= 130))
		{
			int X = lpObj->X;
			int Y = lpObj->Y;

			if( X >= 130 && X <= 166 )
			{
				if( Y >= 188 && Y <= 228 ) //Lvl 1
				{
					return 0;
				}else if( Y >= 128 && Y <= 170 ) //Lvl 3
				{
					return 2;
				}
			}else if( X >= 193 && X <= 228 )
			{
				if( Y >= 188 && Y <= 228 ) //Lvl 2
				{
					return 1;
				}else if( Y >= 128 && Y <= 170 ) //Lvl 4
				{
					return 3;
				}else if( Y >= 68 && Y <= 111 ) //Lvl 5
				{
					return 4;
				}
			}
		}
	}
	return Err;
}

int cSkyEvent::MonsterDie(LPOBJ lpObj)
{
	int iLevel = this->GetLevel(lpObj);
	if(iLevel > -1)
	{
		int iUser = this->GetPlayerSubIndex(lpObj,iLevel);
		if(iUser > -1)
		{
			if(this->CurrentStage > 0)
			{
				this->m_UserData[iLevel][iUser].KillCount[(this->CurrentStage-1)]++;
				return (this->ExtraExp[iLevel][(this->CurrentStage-1)]);
			}
		}else
		{
			LogAddCTD(3,"error-L3 : [Sky Event] (%d) doesn't match the user (%d) %s",
				iLevel+1, lpObj->m_Index,lpObj->Name);
		}
	}
	return 1;
}

void cSkyEvent::DropWinnerItem(int iLevel,int iUser)
{
	int iIndex = this->m_UserData[iLevel][iUser].m_Index;
	if ( OBJMAX_RANGE(iIndex) == FALSE )
	{
		return;
	}
	int iType = ::ItemGetNumberMake(this->ItemWinType[iLevel], this->ItemWinIndex[iLevel]);	// Chaos Gem 12,15
	ItemSerialCreateSend(gObj[iIndex].m_Index, gObj[iIndex].MapNumber, gObj[iIndex].X,
		gObj[iIndex].Y, iType, this->ItemWinLevel[iLevel], this->ItemWinDur[iLevel], this->ItemWinSkill[iLevel], this->ItemWinLuck[iLevel], this->ItemWinOpt[iLevel], iIndex, this->ItemWinExc[iLevel], 0);
}

void cSkyEvent::MsgStringSend(LPSTR  szMsg, BYTE type) 
{
	PMSG_NOTICE pNotice;
	pNotice.type = 0;	// 3
	pNotice.btCount = 0;	// 4
	pNotice.wDelay = 0;	// 6	
	pNotice.dwColor = 0;	// 8
	pNotice.btSpeed = 0;	// C
	
	TNotice::MakeNoticeMsg((TNotice*)&pNotice, type, szMsg);

	for ( int n = OBJ_STARTUSERINDEX ; n < OBJMAX ; n++)
	{
		if ( gObj[n].Connected == PLAYER_PLAYING )
		{
			if (( gObj[n].Type  == OBJ_USER ) && ( gObj[n].MapNumber == MAP_INDEX_ICARUS) && ( gObj[n].X >= 130))
			{
				DataSend(n, (UCHAR*)&pNotice, pNotice.h.size);
			}
		}
	}
}

void cSkyEvent::ClearAllMonsters()
{		
	for(int iStage = 0; iStage < MAX_SKY_EVENT_STAGES; iStage++)
	{
		for(int iLevel = 0; iLevel < MAX_SKY_EVENT_LEVEL; iLevel++)
		{
			for(int MobCount=0; MobCount < this->MonsterStageCount[iLevel][iStage] ;MobCount++)
			{
				if (this->m_MonsterData[iLevel][iStage][MobCount].ID >= 0)
				{
					int aIndex = this->m_MonsterData[iLevel][iStage][MobCount].ID;
					if (gObj[aIndex].Class == this->m_MonsterData[iLevel][iStage][MobCount].Number &&
						SKYEVENT_MAP_RANGE(gObj[aIndex].MapNumber,gObj[aIndex].X)
						)
					{
						gObjDel(aIndex);
					}

					this->m_MonsterData[iLevel][iStage][MobCount].ID = -1;
				}
			}
		}
	}
}

void cSkyEvent::ClearMonsters(BYTE iLevel,BYTE iStage)
{
	if(this->Enabled == 1 && this->Start == 1)
	{
		for(int MobCount=0; MobCount < this->MonsterStageCount[iLevel][iStage] ;MobCount++)
		{
			if (this->m_MonsterData[iLevel][iStage][MobCount].ID >= 0)
			{
				int aIndex = this->m_MonsterData[iLevel][iStage][MobCount].ID;
				if (gObj[aIndex].Class == this->m_MonsterData[iLevel][iStage][MobCount].Number &&
					SKYEVENT_MAP_RANGE(gObj[aIndex].MapNumber,gObj[aIndex].X)
					)
				{
					gObjDel(aIndex);
				}

				this->m_MonsterData[iLevel][iStage][MobCount].ID = -1;
			}
		}
	}
}

void cSkyEvent::SetMonsters(BYTE iLevel,BYTE iStage)
{
	if(this->Enabled == 1 && this->Start == 1)// && SkyEvent.ActiveLevel[iLevel] == TRUE)
	{
		for(int MobCount=0; MobCount < this->MonsterStageCount[iLevel][iStage] ;MobCount++)
		{			
			int MobID = gObjAddMonster(gMSetBase.m_Mp[this->m_MonsterData[iLevel][iStage][MobCount].Pos].m_MapNumber);
			if(MobID >= 0)
			{
				this->m_MonsterData[iLevel][iStage][MobCount].ID = MobID;
				gObjSetPosMonster(MobID, this->m_MonsterData[iLevel][iStage][MobCount].Pos);
				gObjSetMonster(MobID, gMSetBase.m_Mp[this->m_MonsterData[iLevel][iStage][MobCount].Pos].m_Type,"cSkyEvent::SetMonsters");
			}
		}
	}
}

void cSkyEvent::ReadMonsters(char * FilePath)
{
	if(this->Enabled == 1)
	{
		int Token;
		int iLevel;
		int MobNum;
		int X;
		int Y;
		int Count;

		SMDFile = fopen(FilePath, "r");

		if ( SMDFile == NULL )
		{
			MsgBox("SkyEvent data load error %s", FilePath);
			return;
		}

		for(int Stages = 0; Stages < MAX_SKY_EVENT_STAGES; Stages++)
		{
			for(int Level = 0; Level < MAX_SKY_EVENT_LEVEL; Level++)
			{
				this->MonsterStageCount[Level][Stages] = 0;
			}
		}

		for(int iL = 0; iL < MAX_SKY_EVENT_LEVEL; iL++)
		{
			for (int iS = 0; iS < MAX_SKY_EVENT_STAGES; iS++)
			{
				for (int iM = 0; iM <MAX_SKY_EVENT_STAGE_MOB_COUNT; iM++)
				{
					this->m_MonsterData[iL][iS][iM].ID = -1;
					this->m_MonsterData[iL][iS][iM].Pos = -1;
					this->m_MonsterData[iL][iS][iM].Number = -1;
				}
			}
		}

		while ( true )
		{
			
			for(int iStages = 1; iStages < (MAX_SKY_EVENT_STAGES+1); iStages++)
			{
				int iType = GetToken();
				while(true)
				{
					Token = GetToken();
					if ( strcmp("end", TokenString) == 0 )
					{
						break;
					}
					iLevel = TokenNumber;

					Token = GetToken();
					MobNum = TokenNumber;

					Token = GetToken();
					X = TokenNumber;

					Token = GetToken();
					Y = TokenNumber;

					Token = GetToken();
					Count = TokenNumber;

					for(int i=0;i<Count;i++)
					{
						int MobCount = this->MonsterStageCount[(iLevel-1)][(iStages-1)];

						if (iLevel <= MAX_SKY_EVENT_LEVEL &&
							iStages <= MAX_SKY_EVENT_STAGES &&
							MobCount < MAX_SKY_EVENT_STAGE_MOB_COUNT)
						{
							int MobPos = gObjMonsterAdd(MobNum,10,X,Y);
							if(MobPos >= 0)
							{
								this->m_MonsterData[(iLevel-1)][(iStages-1)][MobCount].ID = -1;
								this->m_MonsterData[(iLevel-1)][(iStages-1)][MobCount].Pos = MobPos;
								this->m_MonsterData[(iLevel-1)][(iStages-1)][MobCount].Number = MobNum;
								this->MonsterStageCount[(iLevel-1)][(iStages-1)]++;
							} else {
								LogAddTD("[SkyEvent] - Cant add monster to Lvl:%d, Stage:%d, Count:%d [ID:%d:,X:%d,Y:%d] Pos:%d",
									iLevel,iStages,MobCount,MobNum,X,Y,MobPos);
							}
						} else {
							LogAddTD("[SkyEvent][Overflow] - Cant add monster to Lvl:%d, Stage:%d, Count:%d [ID:%d:,X:%d,Y:%d]",
								iLevel,iStages,MobCount,MobNum,X,Y);
						}
					}
				}
			}
			break;
		}
			
		fclose(SMDFile);
		LogAddCTD(3,"[SkyEvent] - %s file is Loaded",FilePath);
	}
}

void cSkyEvent::StartEvent()
{
	if((this->Start == 0) && (this->Enabled == 1))
		_beginthread( cSkyEvent__InsideTrigger, 0, NULL  );
}


void cSkyEvent__InsideTrigger(void * lpParam)
{
	SkyEvent.Start = 1;
	SkyEvent.NPCEnabled = TRUE;
	char sBuf[1024] = {0};
	SkyEvent.CurrentStage = 0;
	BYTE ActiveLevelsSum = 0;

	for(int i=5;i>0;i--)
	{
		wsprintf(sBuf, lMsg.Get(MSGGET(14, 50)), i);
		LogAddCTD(3, "[Sky Event] will close after %d minute(s)", i);
		AllSendServerMsg(sBuf);
		Sleep(60000);
	}

	SkyEvent.NPCEnabled = FALSE;
	LogAddCTD(3,"[Sky Event] Event entrance is closed!");
	AllSendServerMsg(lMsg.Get(MSGGET(14, 51)));
	
	for(int iStages = 0; iStages < MAX_SKY_EVENT_STAGES; iStages++)
	{
		SkyEvent.CurrentStage++;
		ActiveLevelsSum = 0;
		for(int iLevel = 0; iLevel < MAX_SKY_EVENT_LEVEL; iLevel++)
		{
			//if(SkyEvent.ActiveLevel[iLevel] == TRUE)
			//{
				int ActiveUsers = SkyEvent.CheckUsersAlive(iLevel);
				if(ActiveUsers > 0)
				{
					if(iLevel > 0 && iStages > 0)
						SkyEvent.ClearMonsters((iLevel-1),(iStages-1));	

					SkyEvent.SetMonsters(iLevel,iStages);
					ActiveLevelsSum++;
				}
				else
				{			
					if(iLevel > 0 && iStages > 0)
						SkyEvent.ClearMonsters((iLevel-1),(iStages-1));	
					SkyEvent.ClearLevelData(iLevel);
				}
			//}

			//Send Experience Information to Users
			SkyEvent.SendUserExpInfo(iLevel);
		}
		SkyEvent.MsgStringSend("[SkyEvent] Stage Experience has changed!",0x01);

		if(ActiveLevelsSum > 0)
			Sleep(SkyEvent.StageMinutes[iStages] * 60000);
		else
			break;
	}

	SkyEvent.ClearAllMonsters();

	for(int iLevel = 0; iLevel < MAX_SKY_EVENT_LEVEL; iLevel++)
	{
		int ActiveUsers = SkyEvent.CheckUsersAlive(iLevel);
		if(ActiveUsers > 0)
		{
			int WinUser = SkyEvent.GetWinner(iLevel);
			if(WinUser > -1)
			{
				SkyEvent.DropWinnerItem(iLevel,WinUser);
			}
		}
	}
	LogAddCTD(3,"[Sky Event] Event End");
	AllSendServerMsg(lMsg.Get(MSGGET(14, 52)));

	for(int i=3;i>0;i--)
	{
		wsprintf(sBuf, lMsg.Get(MSGGET(14, 53)), i*10);
		SkyEvent.MsgStringSend(sBuf,0x01);
		Sleep(10000);
	}
	
	SkyEvent.ExitPlayers();

	for(int iLevel = 0; iLevel < MAX_SKY_EVENT_LEVEL; iLevel++)
	{
		SkyEvent.ClearLevelData(iLevel);
	}

	SkyEvent.Start = 0;
	_endthread();
}


void cSkyEvent::ExitPlayers()
{
	for ( int n = OBJ_STARTUSERINDEX ; n < OBJMAX ; n++)
	{
		if ( gObj[n].Connected == PLAYER_PLAYING )
		{
			if (( gObj[n].Type  == OBJ_USER ) && ( gObj[n].MapNumber == MAP_INDEX_ICARUS) && ( gObj[n].X >= 130))
			{
				this->Teleport(&gObj[n],255);
			}
		}
	}
}

void cSkyEvent::SendUserExpInfo(BYTE iLevel)
{
	for(int iUser = 0; iUser < MAX_SKY_EVENT_SUB_LEVEL; iUser++)
	{
		int m_Index = this->m_UserData[iLevel][iUser].m_Index;
		if(m_Index != -1)
		{
			if ( gObj[m_Index].Connected >= PLAYER_CONNECTED )
			{
				if (this->m_UserData[iLevel][iUser].DBNumber == gObj[m_Index].DBNumber)
				{
					if((gObj[this->m_UserData[iLevel][iUser].m_Index].MapNumber == MAP_INDEX_ICARUS) && (gObj[this->m_UserData[iLevel][iUser].m_Index].X >= 130)) 
					{
						gObjGetExtraExperienceInfoMsg(this->m_UserData[iLevel][iUser].m_Index);
					}
				}
			}
		}
	}
}

int cSkyEvent::CheckUsersAlive(BYTE iLevel)
{
	int UserCount = 0;
	for(int iUser = 0; iUser < MAX_SKY_EVENT_SUB_LEVEL; iUser++)
	{
		int m_Index = this->m_UserData[iLevel][iUser].m_Index;
		if(m_Index != -1)
		{
			if ( gObj[m_Index].Connected >= PLAYER_CONNECTED )
			{
				//if (strcmp(this->m_UserData[iLevel][iUser].Name,gObj[m_Index].Name))
				if (this->m_UserData[iLevel][iUser].DBNumber != gObj[m_Index].DBNumber)
				{
					this->RemoveUser(iLevel,iUser);
				}else
				{
					if((gObj[this->m_UserData[iLevel][iUser].m_Index].MapNumber == MAP_INDEX_ICARUS) && (gObj[this->m_UserData[iLevel][iUser].m_Index].X >= 130)) 
					{
						UserCount++;
					}
					else 
					{
						this->RemoveUser(iLevel,iUser);
					}
				}
			}else
			{
				this->RemoveUser(iLevel,iUser);
			}
		}
	}
	return UserCount;
}

void cSkyEvent::RemoveUser(BYTE iLevel,int iUser)
{
	this->m_UserData[iLevel][iUser].m_Index = -1;
	this->m_UserData[iLevel][iUser].DBNumber = -1;		
	//memset( this->m_UserData[iLevel][iUser].Name,0,MAX_ACCOUNT_LEN);
	for(int iStages = 0; iStages < MAX_SKY_EVENT_STAGES; iStages++)
	{
		this->m_UserData[iLevel][iUser].KillCount[iStages] = 0;		
	}
}

void cSkyEvent::ClearLevelData(BYTE iLevel)
{
	//this->ActiveLevel[iLevel] = FALSE;
	this->PlayerCount[iLevel] = 0;

	for(int iUser = 0; iUser < MAX_SKY_EVENT_SUB_LEVEL; iUser++)
	{
		this->m_UserData[iLevel][iUser].m_Index = -1;	
		this->m_UserData[iLevel][iUser].DBNumber = -1;			
		//memset( this->m_UserData[iLevel][iUser].Name,0,sizeof(MAX_ACCOUNT_LEN));
		for(int iStages = 0; iStages < MAX_SKY_EVENT_STAGES; iStages++)
		{
			this->m_UserData[iLevel][iUser].KillCount[iStages] = 0;		
		}
	}
}

void cSkyEvent::Teleport(LPOBJ lpObj, BYTE iLevel)
{
	int rNum = rand()%5;
	switch(iLevel)
	{
		case 0:
		{
			gObjTeleport(lpObj->m_Index,10,148 + rNum,209 + rNum);	
		}break;

		case 1:
		{
			gObjTeleport(lpObj->m_Index,10,210 + rNum,209 + rNum);	
		}break;

		case 2:
		{
			gObjTeleport(lpObj->m_Index,10,147 + rNum,148 + rNum);	
		}break;
		
		case 3:
		{
			gObjTeleport(lpObj->m_Index,10,211 + rNum,148 + rNum);	
		}break;
		
		case 4:
		{
			gObjTeleport(lpObj->m_Index,10,211 + rNum,89 + rNum);	
		}break;
		case 255:
		{			
			gObjMoveGate(lpObj->m_Index, 22);
		}break;
	}
}

int cSkyEvent::CheckEnterLevel(int iIndex)	// RET : [-1:Error][0-4:Success]
{
	if ( OBJMAX_RANGE(iIndex) == FALSE )
	{
		return -1;
	}

	if ( gObj[iIndex].Type != OBJ_USER || gObj[iIndex].Connected <= PLAYER_LOGGED )
	{
		return -1;
	}

	if(gObjIsNewClass(&gObj[iIndex]) == 1)
	{
		return (MAX_SKY_EVENT_LEVEL-1);
	}

	int pLevel = gObj[iIndex].Level;

	for(int iLevel = 0; iLevel < MAX_SKY_EVENT_LEVEL; iLevel++)
	{
		if(pLevel >= Level_Min[iLevel] && pLevel <= Level_Max[iLevel])
			return iLevel;
	}

	return -1;
}

BOOL cSkyEvent::NpcTalk(LPOBJ lpNpc, LPOBJ lpObj)
{
	int iIndex = lpObj->m_Index;
	if((this->Enabled == 1) && (this->Start == 1) && (this->NPCEnabled == 1))
	{
		char sBuf[255]={0};
		int iLevel = this->CheckEnterLevel(iIndex);
		if(iLevel > -1)
		{
			if(this->PlayerCount[iLevel] < MAX_SKY_EVENT_SUB_LEVEL)
			{
				if(DeleteItemByLevel(iIndex,0x1C33,iLevel+1) == 1)
				{
					lpObj->m_IfState.use = 0;
					this->AddUser(iLevel,iIndex);
					this->Teleport(&gObj[iIndex],iLevel);
					return TRUE;
				}else
				{
					wsprintf(sBuf, lMsg.Get(MSGGET(14, 54)), ItemAttribute[0x1C33].Name, iLevel+1, iLevel+1);
					ChatTargetSend(lpNpc, sBuf, lpObj->m_Index);
				}
			}else
			{
				wsprintf(sBuf, lMsg.Get(MSGGET(14, 55)), iLevel+1);
				ChatTargetSend(lpNpc, sBuf, lpObj->m_Index);
			}
		}
	}
	return FALSE;
}

void cSkyEvent::AddUser(BYTE iLevel,int iIndex)
{
	int iUser = this->PlayerCount[iLevel];
	this->m_UserData[iLevel][iUser].m_Index = iIndex;	
	this->m_UserData[iLevel][iUser].DBNumber = gObj[iIndex].DBNumber;	
	//memcpy(this->m_UserData[iLevel][iUser].Name, gObj[iIndex].Name, MAX_ACCOUNT_LEN);
	for(int iStages = 0; iStages < MAX_SKY_EVENT_STAGES; iStages++)
	{
		this->m_UserData[iLevel][iUser].KillCount[iStages] = 0;		
	}
	this->PlayerCount[iLevel]++;
}

#endif