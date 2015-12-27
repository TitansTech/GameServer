#include "stdafx.h"
#include "GameServer.h"
#include "LogProc.h"
#include "GameMain.h"
#include "TNotice.h"
#include "..\include\readscript.h"
#include "gObjMonster.h"
#include "MapServerManager.h"
#include "DSProtocol.h"
#include "Event.h"
#include "ImperialGuardian.h"
#include "ExServerProtocol.h"
#include "SCFExDB_Defines.h"
#include "..\common\winutil.h"

#include "LogToFile.h"
extern CLogToFile ANTI_HACK_LOG;

cImperialGuardian g_ImperialGuardian;
BYTE cImperialGuardian__Map[MAX_IG_EVENT_DAYS]	= {72,69,70,71,69,70,71};//0 its sunday;
BYTE cImperialGuardian__Stages[MAX_IG_EVENT_DAYS]= {4, 3, 3, 3, 3, 3, 3};//0 its sunday;

static const struct IG_GATES
{
	int stage1Start;
	int stage1Finish;
	int stage2Start;
	int stage2Finish;
	int stage3Start;
	int stage3Finish;
	int stage4Start;

} cImperialGuardian_Gates[MAX_IG_EVENT_DAYS] = {

	322,323,324,325,326,327,328,
	307,308,309,310,311,-1,-1,
	312,313,314,315,316,-1,-1,
	317,318,319,320,321,-1,-1,
	307,308,309,310,311,-1,-1,
	312,313,314,315,316,-1,-1,
	317,318,319,320,321,-1,-1
};

static const struct IGUARDIAN_ZONE
{
	BYTE btStartX;
	BYTE btStartY;
	BYTE btEndX;
	BYTE btEndY;

} g_iGuardianDoorMapXY[MAX_IG_EVENT_STAGES][10]= 
{
	//Map 69
	219, 28, 243, 55,  // Zone 1-A
	216, 55, 234, 85,  // Zone 1-B
	209, 78, 215, 83,  // Zone 1-C	216,83 => 215, 83 [DaRKav]

	166, 15, 194, 38,  // Zone 2-A
	152, 23, 166, 53,  // Zone 2-B
	152, 53, 156, 59,  // Zone 2-C

	172, 79, 187, 102, // Zone 3-A
	-1,-1,-1,-1,
	-1,-1,-1,-1,
	-1,-1,-1,-1,

	//Map 70
	51, 53, 75, 74,	   // Zone 1-A
	19, 62, 50, 67,	   // Zone 1-B
	13, 64, 19, 66,	   // Zone 1-C

	26, 93, 48, 117,   // Zone 2-A
	38, 117, 55, 154,  // Zone 2-B
	53, 154, 57, 160,  // Zone 2-C

	84, 104, 107, 118, // Zone 3-A
	-1,-1,-1,-1,
	-1,-1,-1,-1,
	-1,-1,-1,-1,

	//Map 71
	136, 180, 146, 204,// Zone 1-A
	111, 185, 119, 200,// Zone 1-B
	83, 193, 89, 197,  // Zone 1-C

	212, 134, 232, 144,// Zone 2-A
	170, 160, 220, 193,// Zone 2-B
	222, 193, 224, 200,// Zone 2-C

	157, 217, 176, 239,// Zone 3-A
	-1,-1,-1,-1,
	-1,-1,-1,-1,
	-1,-1,-1,-1,

	//Map 72
	66, 59, 81, 78,	   // Zone 1-A
	42, 66, 50, 90,    // Zone 1-B
	29, 90, 32, 96,    // Zone 1-C

	30, 176, 40, 185,  // Zone 2-A
	52, 189, 60, 193,  // Zone 2-B
	65, 160, 72, 166,  // Zone 2-C

	156, 127, 197, 139,// Zone 3-A
	197, 131, 226, 159,// Zone 3-B
	220, 159, 226, 166,// Zone 3-C
	
	180, 20, 214, 32   // Zone 4-A
};

BOOL IMPERIALGUARDIAN_MAP_RANGE(int Map)
{
	if ((Map < MAP_INDEX_EMPIREGUARDIAN1) || (Map > MAP_INDEX_EMPIREGUARDIAN4))
	{
		return FALSE;
	}
	return TRUE;
}

void cImperialGuardian::ReadConfigs(char * FilePath)
{
	this->Enabled = GetPrivateProfileInt("Common", "SCFImperialGuardianEventEnabled",0, FilePath) ;
	this->DifMaxLvl = GetPrivateProfileInt("Common", "SCFImperialGuardianMaxLevelDifficulty",ReadConfig.Max_Normal_Level,FilePath);
	this->SaveInDB =  GetPrivateProfileInt("Events", "SCFImperialGuardianSaveInDB",0,RANKINGINI);

	if(this->DifMaxLvl == 0)
		this->DifMaxLvl = ReadConfig.Max_Normal_Level;

	InitializeCriticalSection(&mobKill);

	LogAddTD("[SCF Imperial Guardian] - %s file is Loaded",FilePath);
}

BOOL cImperialGuardian::DoorAttack(LPOBJ mObj)
{
	if(IMPERIALGUARDIAN_MAP_RANGE(mObj->MapNumber) == TRUE)
	{
		int MobId = mObj->Class;

		if (this->WaitPeriod == TRUE)
			return FALSE;

		if(MobId == 526)
			return FALSE;

		if(MobId == 524 || MobId == 525 || MobId == 527 || MobId == 528)
		{
			int stage = this->OnStage;
			int day = this->ActiveDay;

			if(this->Start == FALSE)
				return FALSE;

			if(this->MonsterStageKillCount[day][stage] >= this->MonsterStageCount[day][stage])
				return TRUE;

			if ((this->KillDoor == 0) || 
				(this->KillDoor == 3) || 
				(this->KillDoor == 6) || 
				(this->KillDoor == 9))
				return TRUE;

			return FALSE;
			//if((this->KillDoor == 0) || (this->KillDoor == 3) || (this->KillDoor == 6) || (this->KillDoor == 9))
			//	return TRUE;
			//else
			//{
			//	int day = this->ActiveDay;
			//	int stage = this->OnStage;
			//	if(MonsterStageKillCount[day][stage] >= MonsterStageCount[day][stage])
			//	{
			//		return TRUE;
			//	}else
			//	{
			//		return FALSE;
			//	}
			//}
		}else
		{
			return TRUE;
		}
	}else
	{
		return TRUE;
	}
}
BOOL cImperialGuardian::gObjMoveGateCheck(LPOBJ lpObj, int Gate)
{
	if(IMPERIALGUARDIAN_MAP_RANGE(lpObj->MapNumber) == TRUE)
	{
		if(this->Enabled == 1 && this->Occuped == 1)
		{
			if(Gate == cImperialGuardian_Gates[this->ActiveDay].stage1Finish)
			{
				if(this->Stage >= 1)
				{
					if(this->OnStage<2)
					{
						this->OnStage = 1;
						this->WaitPeriod = TRUE;
					}
					goto Continue;
				}else
				{
					goto Error;
				}
			}
			else if(Gate == cImperialGuardian_Gates[this->ActiveDay].stage2Finish)
			{
				if(this->Stage >= 2)
				{
					if(this->OnStage<3)
					{
						this->OnStage = 2;
						this->WaitPeriod = TRUE;
					}
					goto Continue;
				}else
				{
					goto Error;
				}
			}
			else if(Gate == cImperialGuardian_Gates[this->ActiveDay].stage3Finish)
			{
				if(this->Stage >= 3)
				{
					if(this->OnStage<4)
					{
						this->OnStage = 3;
						this->WaitPeriod = TRUE;
					}
					goto Continue;
				}else
				{
					goto Error;
				}
			}else
			{
				return TRUE;
			}
		}else
		{
			return TRUE;
		}
	}else
	{
		return TRUE;
	}

Error:
	//YOU NEED TO KILL ALL MOBS BEFORE PROCEED TO NEXT STAGE!
	g_ImperialGuardian.GCServerMsgStringSend("You must kill all monsters before continue!",1);
	return FALSE;

Continue:
	
	LogAddC(12, "[Imperial Guardian] (Day:%d) State -> Moving To Stage %d PartyNumber: %d, User: %s", this->ActiveDay,(this->Stage+1),this->m_UserData.PartyIndex,lpObj->Name);
	return TRUE;
}

BYTE cImperialGuardian::CheckOverlapPaperMark(int iIndex)
{
	for ( int x = 0;x<ReadConfig.MAIN_INVENTORY_SIZE(iIndex,false);x++ )
	{
		if ( gObj[iIndex].pInventory[x].IsItem() == TRUE )
		{
			if ( gObj[iIndex].pInventory[x].m_Type == ITEMGET(14,101) )
			{
				int iITEM_DUR = gObj[iIndex].pInventory[x].m_Durability;

				if ( IMPERIAL_MARK_RANGE(iITEM_DUR) != FALSE )
				{
					return x;
				}
			}
		}
	}

	return -1;
}

void cImperialGuardian::SetMonsters(BYTE iStage)
{
	if(this->Enabled == 1 && this->Occuped == 1)
	{
		this->MonsterStageCount[this->ActiveDay][iStage] = 0;

		for(int MobCount=0; MobCount < MAX_IG_EVENT_STAGE_MOB_COUNT ;MobCount++)
		{	
			if (this->m_MonsterData[this->ActiveDay][iStage][MobCount].Pos < 0)
			{
				continue;
			}

			int MobID = gObjAddMonster(gMSetBase.m_Mp[this->m_MonsterData[this->ActiveDay][iStage][MobCount].Pos].m_MapNumber);
			if(MobID >= 0)
			{
				short MobNumber = gMSetBase.m_Mp[this->m_MonsterData[this->ActiveDay][iStage][MobCount].Pos].m_Type;

				this->m_MonsterData[this->ActiveDay][iStage][MobCount].ID = MobID;
				BOOL retSetPos = gObjSetPosMonster(MobID, this->m_MonsterData[this->ActiveDay][iStage][MobCount].Pos);
				if (retSetPos == FALSE)
				{
					LogAddTD("[Imperial Guardian][retSetPos] - Cant add monster Pos:%d,Number:%d [%d,%d,%d]",
						this->m_MonsterData[this->ActiveDay][iStage][MobCount].Pos,
						this->m_MonsterData[this->ActiveDay][iStage][MobCount].Number,
						gMSetBase.m_Mp[this->m_MonsterData[this->ActiveDay][iStage][MobCount].Pos].m_MapNumber,
						gMSetBase.m_Mp[this->m_MonsterData[this->ActiveDay][iStage][MobCount].Pos].m_X,
						gMSetBase.m_Mp[this->m_MonsterData[this->ActiveDay][iStage][MobCount].Pos].m_Y
					);

					continue;
				}

				BOOL retSetMon = gObjSetMonster(MobID, MobNumber,"cImperialGuardian::SetMonsters");
				if (retSetMon == FALSE)
				{
					LogAddTD("[Imperial Guardian][retSetMon] - Cant add monster Pos:%d,Number:%d [%d,%d,%d]",
						this->m_MonsterData[this->ActiveDay][iStage][MobCount].Pos,
						this->m_MonsterData[this->ActiveDay][iStage][MobCount].Number,
						gMSetBase.m_Mp[this->m_MonsterData[this->ActiveDay][iStage][MobCount].Pos].m_MapNumber,
						gMSetBase.m_Mp[this->m_MonsterData[this->ActiveDay][iStage][MobCount].Pos].m_X,
						gMSetBase.m_Mp[this->m_MonsterData[this->ActiveDay][iStage][MobCount].Pos].m_Y
					);

					continue;
				}

				//if((MobID >= 100 && MobID <= 106) || (MobID == 523))
				//{
				//	//gObj[MobID].m_ViewSkillState |= 0x10000000;
				//	//GCSkillInfoSend(&gObj[MobID], 1, 0x1B);
				//	gObj[MobID].isInvisible = 1;
				//}
				this->SetMonsterDifficulty(MobID);
				if (MobNumber == 523 || //Trap
					MobNumber == 524 || 
					MobNumber == 525 || 
					MobNumber == 526 || //Statue
					MobNumber == 527 || 
					MobNumber == 528)
				{
				}
				else
				{
					this->MonsterStageCount[this->ActiveDay][iStage]++;
				}
				LogAddTD("[Imperial Guardian][OK] - Add monster Pos:%d, Number:%d [%d,%d,%d] (%d)",
					this->m_MonsterData[this->ActiveDay][iStage][MobCount].Pos,
					this->m_MonsterData[this->ActiveDay][iStage][MobCount].Number,
					gMSetBase.m_Mp[this->m_MonsterData[this->ActiveDay][iStage][MobCount].Pos].m_MapNumber,
					gMSetBase.m_Mp[this->m_MonsterData[this->ActiveDay][iStage][MobCount].Pos].m_X,
					gMSetBase.m_Mp[this->m_MonsterData[this->ActiveDay][iStage][MobCount].Pos].m_Y,
					this->MonsterStageCount[this->ActiveDay][iStage]
				);
			} else {
				LogAddTD("[Imperial Guardian][MobID] - Cant add monster Pos:%d,Number:%d [%d,%d,%d]",
					this->m_MonsterData[this->ActiveDay][iStage][MobCount].Pos,
					this->m_MonsterData[this->ActiveDay][iStage][MobCount].Number,
					gMSetBase.m_Mp[this->m_MonsterData[this->ActiveDay][iStage][MobCount].Pos].m_MapNumber,
					gMSetBase.m_Mp[this->m_MonsterData[this->ActiveDay][iStage][MobCount].Pos].m_X,
					gMSetBase.m_Mp[this->m_MonsterData[this->ActiveDay][iStage][MobCount].Pos].m_Y
				);
			}
		}
	}
}

void cImperialGuardian::ReadMonsters(char * FilePath)
{
	//this->Enabled = 1;
	if(this->Enabled == 1)
	{
		int Token;
		int iStage;
		int Trap;
		int MobNum;
		int X;
		int Y;
		int Count;
		int Dir;

		SMDFile = fopen(FilePath, "r");

		if ( SMDFile == NULL )
		{
			MsgBox("Imperial Guardian data load error %s", FilePath);
			return;
		}

		for(int Stages = 0; Stages < MAX_IG_EVENT_STAGES; Stages++)
		{
			for(int Level = 0; Level < MAX_IG_EVENT_DAYS; Level++)
			{
				this->MonsterStageCount[Level][Stages] = 0;
			}
		}

		for(int iL = 0; iL < MAX_IG_EVENT_DAYS; iL++)
		{
			for (int iS = 0; iS < MAX_IG_EVENT_STAGES; iS++)
			{
				for (int iM = 0; iM <MAX_IG_EVENT_STAGE_MOB_COUNT; iM++)
				{
					this->m_MonsterData[iL][iS][iM].ID = -1;
					this->m_MonsterData[iL][iS][iM].Pos = -1;
					this->m_MonsterData[iL][iS][iM].Number = -1;
				}
			}
		}

		for(int iDay = 0; iDay < MAX_IG_EVENT_DAYS; iDay++)
		{
			int iType = GetToken();
			while(true)
			{
				Token = GetToken();
				if ( strcmp("end", TokenString) == 0 )
				{
					break;
				}
				iStage = TokenNumber;

				Token = GetToken();
				Trap = TokenNumber;

				Token = GetToken();
				MobNum = TokenNumber;

				Token = GetToken();
				X = TokenNumber;

				Token = GetToken();
				Y = TokenNumber;

				Token = GetToken();
				Count = TokenNumber;

				Token = GetToken();
				Dir = TokenNumber;

				int Map = cImperialGuardian__Map[iDay];

				if (g_MapServerManager.CheckMapCanMove(Map) == FALSE )
				{
					LogAddCTD(3,"[Imperial Guardian] No need to load monster base file!");
					continue;
				}

				for(int i=0;i<Count;i++)
				{
					int MobCount = this->MonsterStageCount[iDay][iStage];

					if (iStage < MAX_IG_EVENT_STAGES &&
						MobCount < MAX_IG_EVENT_STAGE_MOB_COUNT)
					{
						int MobPos = gObjMonsterAdd(MobNum,Map,X,Y);
						if(MobPos >= 0)
						{
							if(Trap != 1)
							{
								//if(Trap == 0)
								this->MonsterStageCount[iDay][iStage]++;

								this->m_MonsterData[iDay][iStage][MobCount].ID = -1;
								this->m_MonsterData[iDay][iStage][MobCount].Pos = MobPos;
								this->m_MonsterData[iDay][iStage][MobCount].Number = MobNum;
								gMSetBase.m_Mp[MobPos].m_Dir = Dir;
							} else {
								gMSetBase.m_Mp[MobPos].m_Dir = Dir;

								int MobID = gObjAddMonster(gMSetBase.m_Mp[MobPos].m_MapNumber);
								if(MobID >= 0)
								{
									gObjSetPosMonster(MobID, MobPos);
									gObjSetMonster(MobID, gMSetBase.m_Mp[MobPos].m_Type,"cImperialGuardian::SetTraps");
								} else {
									LogAddTD("[Imperial Guardian][Trap] - Cant add Trap to Day:%d, Stage:%d, [ID:%d:,Map:%d,X:%d,Y:%d]",
										iDay,iStage,MobNum,Map,X,Y);
								}
							}
						} else {
							LogAddTD("[Imperial Guardian][MobPos] - Cant add monster to Day:%d, Stage:%d, Count:%d [ID:%d:,Map:%d,X:%d,Y:%d]",
								iDay,iStage,MobCount,MobNum,Map,X,Y);
						}
					} else {
						LogAddTD("[Imperial Guardian][Overflow] - Cant add monster to Day:%d, Stage:%d, Count:%d [ID:%d:,Map:%d,X:%d,Y:%d]",
							iDay,iStage,MobCount,MobNum,Map,X,Y);
					}
				}
			}
		}
			
		fclose(SMDFile);
		LogAddC(11, "[Imperial Guardian] - %s file is Loaded",FilePath);
	}
}

BOOL cImperialGuardian::MonsterAttack(LPOBJ lpMon)
{
	if(IMPERIALGUARDIAN_MAP_RANGE(lpMon->MapNumber) == TRUE)
	{
		if (this->WaitPeriod == FALSE)
		{
			return TRUE;
		}

		return FALSE;
	} else {
		return TRUE;
	}
}

void cImperialGuardian::MonsterDieRemove(LPOBJ lpMon)
{
	if(IMPERIALGUARDIAN_MAP_RANGE(lpMon->MapNumber) == TRUE)
	{
		int day = this->ActiveDay;
		int stage = this->OnStage;

		int MobId = lpMon->Class;

		if( (MobId >= 100 && MobId <= 106) || 
			(MobId == 523) )
		{
			//Do Nothing
		}else
		{
			LogAddTD("[Imperial Guardian][MobDie] Stage:%d-%d => Monster:%d, Map:%d [%d,%d] Count:%d/%d",
				day, stage,
				MobId, lpMon->MapNumber, lpMon->X, lpMon->Y,
				MonsterStageKillCount[day][stage], MonsterStageCount[day][stage]
			);

			lpMon->m_Attribute = 60;	//Mark Mob to be deleted
			//gObjDel(lpMon->m_Index);
		}
	}
}

void cImperialGuardian::MonsterDiePoint(LPOBJ lpMon)
{
	if(IMPERIALGUARDIAN_MAP_RANGE(lpMon->MapNumber) == TRUE)
	{
		int day = this->ActiveDay;
		int stage = this->OnStage;

		int MobId = lpMon->Class;

		//Help Mob counter work properly
		EnterCriticalSection(&mobKill);

		if ((MobId >= 100 && MobId <= 106) || 
			(MobId == 523 || //Trap
			 MobId == 524 || 
			 MobId == 525 || 
			 MobId == 526 || //Statue
			 MobId == 527 || 
			 MobId == 528))
		{
		}else
		{
			if (MonsterStageKillCount[day][stage] < MonsterStageCount[day][stage])
				this->MonsterStageKillCount[day][stage]++;
			
			LogAddC(12, "[Imperial Guardian] (Day:%d) State -> Monster Killed (%d/%d)(%d:%d,%d,%d) In Stage: %d PartyNumber: %d, PartyLeader:(%d) %s", 
				this->ActiveDay,
				this->MonsterStageKillCount[day][stage], MonsterStageCount[day][stage], 
				lpMon->Class, lpMon->MapNumber, lpMon->X, lpMon->Y, 
				this->Stage,this->m_UserData.PartyIndex,this->m_UserData.PartyLeaderUserNumber,gObj[this->m_UserData.PartyLeaderUserNumber].Name
			);

			if(MonsterStageKillCount[day][stage] >= MonsterStageCount[day][stage])
			{
				LogAddC(12, "[Imperial Guardian] (Day:%d) State -> ALL Monsters Killed In Stage: %d PartyNumber: %d, PartyLeader:(%d) %s", this->ActiveDay,this->Stage,this->m_UserData.PartyIndex,this->m_UserData.PartyLeaderUserNumber,gObj[this->m_UserData.PartyLeaderUserNumber].Name);
				this->Stage++;

				if (cImperialGuardian__Stages[day] > this->Stage)
				{
					this->SetMonsters(this->Stage);
				}
			}
		}

		if(MobId == 526)
		{
			LogAddC(12, "[Imperial Guardian] (Day:%d) State -> Broken Statue (%d/%d)(%d:%d,%d,%d) In Stage: %d PartyNumber: %d, PartyLeader:(%d) %s", 
				this->ActiveDay,
				this->MonsterStageKillCount[day][stage], MonsterStageCount[day][stage], 
				lpMon->Class, lpMon->MapNumber, lpMon->X, lpMon->Y, 
				this->Stage,this->m_UserData.PartyIndex,this->m_UserData.PartyLeaderUserNumber,gObj[this->m_UserData.PartyLeaderUserNumber].Name
			);
		}

		if(MobId == 524 || MobId == 525 || MobId == 527 || MobId == 528)
		{
			LogAddC(12, "[Imperial Guardian] (Day:%d) State -> Broken Door (%d)(%d:%d,%d,%d) In Stage: %d PartyNumber: %d, PartyLeader:(%d) %s", 
				this->ActiveDay,
				this->KillDoor, lpMon->Class, lpMon->MapNumber, lpMon->X, lpMon->Y, 
				this->Stage,this->m_UserData.PartyIndex,this->m_UserData.PartyLeaderUserNumber,gObj[this->m_UserData.PartyLeaderUserNumber].Name
			);
				
			//this->ReleaseCastleDoor(KillDoor);
			this->DoorBlocked[KillDoor] = false;
			this->KillDoor++;
		}
		LeaveCriticalSection(&mobKill);

		lpMon->DieRegen = FALSE;
	}
}

bool cImperialGuardian::CanWalk(BYTE TX, BYTE TY)
{
	int Gate = this->KillDoor;
	if((this->ActiveDay != 0) && (Gate > 6))
		return true;

	int Index = cImperialGuardian__Map[this->ActiveDay] - MAP_INDEX_EMPIREGUARDIAN1;
	if(TX > ::g_iGuardianDoorMapXY[Index][Gate].btStartX && TX <= ::g_iGuardianDoorMapXY[Index][Gate].btEndX)
	{
		if(TY > ::g_iGuardianDoorMapXY[Index][Gate].btStartY && TY <= ::g_iGuardianDoorMapXY[Index][Gate].btEndY)
		{
			if(this->DoorBlocked[Gate]==true)
			{
				return false;
			}
		}
	}
	return true;
}

BYTE cImperialGuardian::IsPartyAlive()
{
	// 0 = No Party
	// 1 = Some ppl left or are not inside map
	// 2 = OK
	int pIndex = this->m_UserData.PartyIndex;

	if(gParty.IsParty(pIndex) == FALSE)
	{
		return 0;
	}

	if(gObjIsConnected(this->m_UserData.PartyLeaderUserNumber) == TRUE)
	{
		if(gParty.Isleader(pIndex,this->m_UserData.PartyLeaderUserNumber,this->m_UserData.PartyLeaderDBNumber) == FALSE)
		{
			return 0;
		}
	}else
	{
		return 0;
	}

	int Active = 0;
	for(int i = 0; i < MAX_USER_IN_PARTY; i++)
	{
		int pId = gParty.m_PartyS[pIndex].Number[i];

		if ( pId < 0 || pId > (OBJMAX-1) )
		{
			
		}else
		{
			if(gObjIsConnected(gParty.m_PartyS[pIndex].Number[i]) == TRUE)
			{
				if(gObj[pId].MapNumber == cImperialGuardian__Map[this->ActiveDay])
				{
					Active++;
				}
			}
		}
	}
	if(this->m_UserData.PartyCount > Active)
	{
		return 1;
	}
	return 2;
}

void cImperialGuardian::DataSendInside(LPBYTE pMsg, int size)
{
	int pIndex = this->m_UserData.PartyIndex;

	if(gParty.IsParty(pIndex) == FALSE)
	{
		return;
	}

	if(gObjIsConnected(this->m_UserData.PartyLeaderUserNumber) == TRUE)
	{
		if(gParty.Isleader(pIndex,this->m_UserData.PartyLeaderUserNumber,this->m_UserData.PartyLeaderDBNumber) == FALSE)
		{
			return;
		}
	}else
	{
		return;
	}

	for(int i = 0; i < MAX_USER_IN_PARTY; i++)
	{
		int pId = gParty.m_PartyS[pIndex].Number[i];

		if ( pId < 0 || pId > (OBJMAX-1) )
		{
			
		}else
		{
			if(gObjIsConnected(gParty.m_PartyS[pIndex].Number[i]) == TRUE)
			{
				if(gObj[pId].MapNumber == cImperialGuardian__Map[this->ActiveDay])
				{
					::DataSend(gObj[pId].m_Index,pMsg,size);
				}
			}
		}
	}
}

void cImperialGuardian::DataSend(LPBYTE pMsg, int size)
{
	int pIndex = this->m_UserData.PartyIndex;

	if(gParty.IsParty(pIndex) == FALSE)
	{
		return;
	}

	if(gObjIsConnected(this->m_UserData.PartyLeaderUserNumber) == TRUE)
	{
		if(gParty.Isleader(pIndex,this->m_UserData.PartyLeaderUserNumber,this->m_UserData.PartyLeaderDBNumber) == FALSE)
		{
			return;
		}
	}else
	{
		return;
	}

	for(int i = 0; i < MAX_USER_IN_PARTY; i++)
	{
		int pId = gParty.m_PartyS[pIndex].Number[i];

		if ( pId < 0 || pId > (OBJMAX-1) )
		{
			
		}else
		{
			if(gObjIsConnected(gParty.m_PartyS[pIndex].Number[i]) == TRUE)
			{
				::DataSend(gObj[pId].m_Index,pMsg,size);
			}
		}
	}
}

void cImperialGuardian::GateState(bool Open)
{
	PMSG_ATTSTATE_IMPERIALGUARDIAN pMsg;
	PHeadSubSetB((LPBYTE)&pMsg, 0xBF, 0x09, sizeof(pMsg));
	pMsg.State = 1;

	if(Open == true)
	{
		LogAddC(13, "[Imperial Guardian] (Day:%d) State -> START: OPEN_GATES",this->ActiveDay);
		pMsg.Type  = 2;
	}
	else
	{
		LogAddC(13, "[Imperial Guardian] (Day:%d) State -> FINISH: CLOSE_GATES",this->ActiveDay);
		pMsg.Type  = 3;
	}

	this->DataSendInside((BYTE*)&pMsg,sizeof(pMsg));
}

void cImperialGuardian::CheckLetterItem(LPOBJ lpObj)
{
	if ( OBJMAX_RANGE(lpObj->m_Index) == FALSE )
		return;

	if (lpObj->m_IfState.type != 98)
	{
		if (lpObj->m_IfState.type != 0 || lpObj->m_IfState.state != 0)
		{
			ANTI_HACK_LOG.Output("[ANTI-HACK][Imperial][Enter] User in map(%d) [%s][%s] State(u:%d,t:%d,s:%d)",
				lpObj->MapNumber, lpObj->AccountID, lpObj->Name,
				lpObj->m_IfState.use,lpObj->m_IfState.type,lpObj->m_IfState.state);

			return;
		}
	}

	lpObj->m_IfState.use = 0;
	lpObj->m_IfState.type = 0;

	int bItemPosition = this->GetEnterItemPosition(lpObj->m_Index);

	if ( bItemPosition != -1 )
	{
		int bEnterResult = this->CheckEnter(lpObj);

		switch ( bEnterResult )
		{
			case -3:
			{
				TNotice::GCServerMsgStringSend("All Party Users must be lvl 15 or higher!",lpObj->m_Index, 0x01);
			}break;
			case -2:
			{
				TNotice::GCServerMsgStringSend("Event Only For Party!",lpObj->m_Index, 0x01);
			}break;
			case -1:
			{
				TNotice::GCServerMsgStringSend("Event Dissabled!",lpObj->m_Index, 0x01);
			}break;
			case 0:
			{
				TNotice::GCServerMsgStringSend("Room Not Availiable!",lpObj->m_Index, 0x01);
			}break;
			case 1:
			{
				TNotice::GCServerMsgStringSend("Entering Imperial Guardian Event!",lpObj->m_Index, 0x01);

				LogAddTD("[Imperial Guardian] (Account:%s, Name:%s) Delete Entrance Tiket (%d) [Serial:%d]",
					lpObj->AccountID, lpObj->Name, 
					lpObj->pInventory[bItemPosition].m_Type, lpObj->pInventory[bItemPosition].m_Number);

				if ((lpObj->pInventory[bItemPosition].m_Durability > 1.0f && lpObj->pInventory[bItemPosition].m_Type == ITEMGET(13,126)) ||
					(lpObj->pInventory[bItemPosition].m_Durability > 1.0f && lpObj->pInventory[bItemPosition].m_Type == ITEMGET(13,127)) )
				{
					lpObj->pInventory[bItemPosition].m_Durability -= 1.0f;
					GCItemDurSend2(lpObj->m_Index, bItemPosition, lpObj->pInventory[bItemPosition].m_Durability, 0);
				} else {
					gObjInventoryDeleteItem(lpObj->m_Index, bItemPosition);
					GCInventoryItemDeleteSend(lpObj->m_Index, bItemPosition, TRUE);
				}
			}break;
		}
	} else {
		TNotice::GCServerMsgStringSend("Lacking ticket to enter!",lpObj->m_Index, 0x01);
		CGAnsImperialNoItem(lpObj->m_Index);
	}
}

int cImperialGuardian::GetEnterItemPosition(int iIndex)
{
	if ( OBJMAX_RANGE(iIndex) == FALSE )
	{
		return -1;
	}

	if ( gObj[iIndex].Type != OBJ_USER || gObj[iIndex].Connected <= PLAYER_LOGGED )
	{
		return -1;
	}

	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);

	for ( int x=0;x<ReadConfig.MAIN_INVENTORY_SIZE(iIndex,false);x++)
	{
		if ( gObj[iIndex].pInventory[x].IsItem() == TRUE )
		{
			if (sysTime.wDayOfWeek == 0)
			{	
				if ( gObj[iIndex].pInventory[x].m_Type == ITEMGET(14,109) ) //Imperial Guardian Sunday Letter
				{
					return x;
				}
				if ( gObj[iIndex].pInventory[x].m_Type == ITEMGET(13,127) ) //Imperial Guardian Sunday Free
				{
					return x;
				}
			} else {
				if ( gObj[iIndex].pInventory[x].m_Type == ITEMGET(14,102) ) //Imperial Guardian Letter
				{
					return x;
				}
				if ( gObj[iIndex].pInventory[x].m_Type == ITEMGET(13,126) ) //Imperial Guardian Sunday Free
				{
					return x;
				}
			}
		}
	}

	return -1;
}

int cImperialGuardian::CheckEnter(LPOBJ lpObj)
{
	/*
	return -3  =  All Party Users must be lvl 15 or higher
	return -2  =  Event Only For Party
	return -1  =  Event Dissabled
	return  0  =  Room Not Availiable
	return  1  =  Enter
	*/

	int pMaxLvl=0;
	int pMinLvl=0;

	if(this->Enabled == 0)
	{
		return -1;
	}
	if ( lpObj->PartyNumber < 0  )
	{
		return -2;
	}

	gParty.GetLevel(lpObj->PartyNumber,pMaxLvl,pMinLvl);
	
	if ( pMinLvl < 15  )
	{
		return -3;
	}
	
	if(this->Occuped == 0)
	{
		this->Difficulty = ( ( (((pMaxLvl + pMinLvl)/2) * 100) / this->DifMaxLvl) / 100.0f);
		SYSTEMTIME sysTime;
		GetLocalTime(&sysTime);
		this->ActiveDay = sysTime.wDayOfWeek;
		this->StartEvent(lpObj);
		return 1;
	}

	return 0;
}

void cImperialGuardian::Fail(int Status)
{
	if(Status == 0)
	{
		//NO PARTY
		LogAddC(13, "[Imperial Guardian] (Day:%d) State -> FAIL: NO PARTY INSIDE",this->ActiveDay);
	}else if (Status == 1)
	{
		//SOME USER DIE OR LEFT
		LogAddC(13, "[Imperial Guardian] (Day:%d) State -> FAIL: PARTY MEMBER DIE OR LEFT",this->ActiveDay);
	}else if (Status == -1)
	{
		//time exceed
		LogAddC(13, "[Imperial Guardian] (Day:%d) State -> FAIL: TIME EXCEED",this->ActiveDay);
	}
	
	PMSG_STATE_IMPERIALGUARDIAN failMsg;
	PHeadSubSetB((LPBYTE)&failMsg, 0xF7, 0x06, sizeof(failMsg));
	failMsg.State = 0;
	this->DataSend((BYTE*)&failMsg,sizeof(failMsg));
}

void cImperialGuardian::Success()
{
	LogAddC(13, "[Imperial Guardian] (Day:%d) State -> SUCCESS PartyNumber: %d, PartyLeader:(%d) %s",this->ActiveDay,this->m_UserData.PartyIndex,this->m_UserData.PartyLeaderUserNumber,gObj[this->m_UserData.PartyLeaderUserNumber].Name);
	
	PMSG_STATE_IMPERIALGUARDIAN winMsg;
	PHeadSubSetB((LPBYTE)&winMsg, 0xF7, 0x06, sizeof(winMsg));
	winMsg.State = 2;

	unsigned long Exp = gObj[this->m_UserData.PartyLeaderUserNumber].Experience - this->m_UserData.PlayerExp;
	
	WORD Tmp1 = HIWORD(Exp);
	WORD Tmp2 = LOWORD(Exp);
	winMsg.Exp[3] = HIBYTE(Tmp1);
	winMsg.Exp[4] = LOBYTE(Tmp1);
	winMsg.Exp[5] = HIBYTE(Tmp2);
	winMsg.Exp[6] = LOBYTE(Tmp2);

	//winMsg.Exp = Exp;
	this->DataSend((BYTE*)&winMsg,sizeof(winMsg));
}

void cImperialGuardian::Timer(int Seconds, BYTE tState)
{
	PMSG_TIMER_IMPERIALGUARDIAN Time;
	PHeadSubSetB((LPBYTE)&Time, 0xF7, 0x04, sizeof(Time));

	int day = this->ActiveDay;
	int stage = this->OnStage;
	int Remating = 0;
	int MonstersLeft = 0;

	if (tState == 0)
	{
		Remating = (ImperialGuardian_WaitTime * 60) - Seconds;
		Time.State = 1;
	} else {
		Remating = (ImperialGuardian_MaxTime * 60) - Seconds;
		MonstersLeft = MonsterStageCount[day][stage] - MonsterStageKillCount[day][stage];
		Time.State = 2;
	}

	int toTimer=0;
	if(Remating == 34)
	{
		toTimer = (Remating * 3.911) + 1;
	}else
	{
		toTimer = Remating * 3.911;
	}
	Time.Time1 = LOBYTE(toTimer);
	Time.Time2 = HIBYTE(toTimer);
	Time.Time3 = 0;

	Time.MonstersLeft = MonstersLeft;
	this->DataSendInside((BYTE*)&Time,sizeof(Time));
}


void cImperialGuardian::GCServerMsgStringSend(LPSTR szMsg, BYTE type) 
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
			if ( gObj[n].Type  == OBJ_USER )
			{
				if (gObj[n].MapNumber >= cImperialGuardian__Map[this->ActiveDay])
					::DataSend(n, (UCHAR*)&pNotice, pNotice.h.size);
			}
		}
	}
}

void cImperialGuardian__InsideTrigger(void *  lpParam)
{
	g_ImperialGuardian.Start = FALSE;
	g_ImperialGuardian.Stage = 0;
	g_ImperialGuardian.OnStage = 0;
	BYTE pAlive = 0;
	g_ImperialGuardian.SetMonsters(g_ImperialGuardian.Stage);
	int IngameSeconds = 0;
	int WaitSeconds = 0;

	g_ImperialGuardian.GCServerMsgStringSend("Imperial Guardian will start in 60 seconds.",1);
	g_ImperialGuardian.Start = TRUE;
	g_ImperialGuardian.WaitPeriod = TRUE;
	g_ImperialGuardian.GateState(true);

	while(true)
	{
		//Check Finish
		if( cImperialGuardian__Stages[g_ImperialGuardian.ActiveDay] <= g_ImperialGuardian.Stage )
		{
			//FINISHED SUCCESS
			break;
		}

		if (g_ImperialGuardian.WaitPeriod == TRUE)
		{
			g_ImperialGuardian.Timer(WaitSeconds, 0);
			if(WaitSeconds >= (ImperialGuardian_WaitTime * 60))
			{
				g_ImperialGuardian.WaitPeriod = FALSE;
				//g_ImperialGuardian.WarpedNextStage = TRUE;	//Keep counter at 0, untill warped to next stage
			}

			IngameSeconds = 0;
			WaitSeconds++;
		} else {
			g_ImperialGuardian.Timer(IngameSeconds, 1);

			//g_ImperialGuardian.WarpedNextStage = FALSE;		//Keep counter at 0, untill warped to next stage
			if(IngameSeconds >= (ImperialGuardian_MaxTime * 60))
			{
				//time exceed
				g_ImperialGuardian.Fail(-1);
				break;
			}

			IngameSeconds++;
			WaitSeconds = 0;
		}

		//Check Party
		pAlive = g_ImperialGuardian.IsPartyAlive();
		if(pAlive != 2)
		{
			g_ImperialGuardian.Fail(pAlive);			
			break;
		}
		
		Sleep(1000);
	}

	g_ImperialGuardian.GateState(false);

	Sleep(5000);
	g_ImperialGuardian.ClearAllMonsters();

	if(pAlive != 0)
	{
		g_ImperialGuardian.GCServerMsgStringSend("Imperial Guardian will close in 30 seconds.",1);
		if(pAlive == 2)
		{
			//Success
			g_ImperialGuardian.Success();
			g_ImperialGuardian.SendRanking();
		}
		Sleep(30000);
	}
	g_ImperialGuardian.WarpOutside();
	LogAddC(13, "[Imperial Guardian] (Day:%d) State -> CLOSED",g_ImperialGuardian.ActiveDay);
	g_ImperialGuardian.Occuped = 0;
	_endthread();

}



void cImperialGuardian::SendRanking()
{
	if(this->SaveInDB == 1)
	{
		int pIndex = this->m_UserData.PartyIndex;

		if(gParty.IsParty(pIndex) == FALSE)
		{
			return;
		}

		if(gObjIsConnected(this->m_UserData.PartyLeaderUserNumber) == TRUE)
		{
			if(gParty.Isleader(pIndex,this->m_UserData.PartyLeaderUserNumber,this->m_UserData.PartyLeaderDBNumber) == FALSE)
			{
				return;
			}
		}else
		{
			return;
		}

		for(int i = 0; i < MAX_USER_IN_PARTY; i++)
		{
			int pId = gParty.m_PartyS[pIndex].Number[i];

			if ( pId < 0 || pId > (OBJMAX-1) )
			{
				
			}else
			{
				if(gObjIsConnected(gParty.m_PartyS[pIndex].Number[i]) == TRUE)
				{
					PMSG_ANS_IGSCORE pMsg = {0};
					PHeadSetB((LPBYTE)&pMsg, SCFExDB_GSSend_IGScore, sizeof(pMsg));

					memcpy(pMsg.AccountID, gObj[pId].AccountID, MAX_ACCOUNT_LEN);
					memcpy(pMsg.Name, gObj[pId].Name, MAX_ACCOUNT_LEN);
					pMsg.ServerCode = gGameServerCode;
					pMsg.Day = this->ActiveDay;

					wsExServerCli.DataSend((char*)&pMsg, pMsg.h.size);
				}
			}
		}
	}
}

void cImperialGuardian::ClearAllMonsters()
{		
	for(int iStage = 0; iStage < MAX_IG_EVENT_STAGES; iStage++)
	{
		for(int MobCount=0; MobCount < MAX_IG_EVENT_STAGE_MOB_COUNT ;MobCount++)
		{
			if (this->m_MonsterData[this->ActiveDay][iStage][MobCount].ID >= 0)
			{
				int aIndex = this->m_MonsterData[this->ActiveDay][iStage][MobCount].ID;
				
				if (gObj[aIndex].Class == this->m_MonsterData[this->ActiveDay][iStage][MobCount].Number)
				{
					if ( IMPERIALGUARDIAN_MAP_RANGE(gObj[aIndex].MapNumber) )
					{
						LogAddC(12, "[Imperial Guardian] (Day:%d) Deliting Monster: %d [%d,%d,%d] Life:%d Live:%d Attr:%d [S:%d, DR:%d]",
							this->ActiveDay,
							gObj[aIndex].Class, 
							gObj[aIndex].MapNumber, gObj[aIndex].X, gObj[aIndex].Y,
							(int)gObj[aIndex].Life, gObj[aIndex].Live, gObj[aIndex].m_Attribute,
							gObj[aIndex].m_State, gObj[aIndex].DieRegen
						);
						gObjDel(aIndex);
					}
				}

				this->m_MonsterData[this->ActiveDay][iStage][MobCount].ID = -1;
			}
		}
	}
	LogAddC(12, "[Imperial Guardian] (Day:%d) State -> FINISH: CLEAR_MONSTERS",this->ActiveDay);
}

void cImperialGuardian::StartEvent(LPOBJ lpObj)
{
	if((this->Occuped == 0) && (this->Enabled == 1))
	{
		if (( lpObj->PartyNumber >= 0  ) && (gParty.GetCount(lpObj->PartyNumber) > 0))
		{
			this->Occuped = 1;
			this->m_UserData.PartyIndex				 = lpObj->PartyNumber;
			this->m_UserData.PartyLeaderUserNumber	 = gParty.m_PartyS[lpObj->PartyNumber].Number[0];
			this->m_UserData.PartyLeaderDBNumber	 = gParty.m_PartyS[lpObj->PartyNumber].DbNumber[0];
			this->m_UserData.PlayerExp				 = lpObj->Experience;
			this->m_UserData.PartyCount				 = gParty.GetCount(lpObj->PartyNumber);
			this->KillDoor							 = 0;
			
			for(int i=0;i<10;i++)
				this->DoorBlocked[i] = true;
			
			for(int i=0;i<MAX_IG_EVENT_STAGES;i++)
			{
				this->MonsterStageKillCount[this->ActiveDay][i]=0;
			}
			if(this->WarpInside() == 0)
			{	
				LogAddC(11, "[Imperial Guardian] Error Tracer -> ENTRY_PARTY PartyNumber: %d, PartyLeader:(%d) %s",this->m_UserData.PartyIndex,this->m_UserData.PartyLeaderUserNumber,gObj[this->m_UserData.PartyLeaderUserNumber].Name);
				this->Occuped = 0;
				this->m_UserData.PartyIndex				 = 0;
				this->m_UserData.PartyLeaderUserNumber	 = 0;
				this->m_UserData.PartyLeaderDBNumber	 = 0;
				this->m_UserData.PlayerExp				 = 0;
				this->m_UserData.PartyCount				 = 0;
			}else
			{
				LogAddC(11, "[Imperial Guardian] (Day:%d) State -> StartEvent PartyNumber: %d, PartyLeader:(%d) %s", this->ActiveDay,this->m_UserData.PartyIndex,this->m_UserData.PartyLeaderUserNumber,gObj[this->m_UserData.PartyLeaderUserNumber].Name);
				LogAddC(11, "[Imperial Guardian] (Day:%d) State -> StartEvent: DIFFICULTY: %d Percent", this->ActiveDay,(int)(this->Difficulty * 100));
				
				//for(int i=0;i<10;i++)
				//	this->BlockCastleDoor(i);

				_beginthread( cImperialGuardian__InsideTrigger, 0, NULL  );
			}
		}
	}
}

void cImperialGuardian::WarpOutside() 
{
	for ( int n = OBJ_STARTUSERINDEX ; n < OBJMAX ; n++)
	{
		if ( gObj[n].Connected == PLAYER_PLAYING )
		{
			if ( gObj[n].Type  == OBJ_USER )
			{
				if (gObj[n].MapNumber == cImperialGuardian__Map[this->ActiveDay])
				{
					BOOL ret = gObjMoveGate(gObj[n].m_Index, 22);

					if (ret = false)
					{
						CloseClient(n);
					}
				}
			}
		}
	}
	LogAddC(12, "[Imperial Guardian] (Day:%d) State -> FINISH: REMOVE_USERS", this->ActiveDay);
}


BOOL cImperialGuardian::WarpInside()
{
	int pIndex = this->m_UserData.PartyIndex;

	if(gParty.IsParty(pIndex) == FALSE)
	{
		return 0;
	}

	if(gObjIsConnected(this->m_UserData.PartyLeaderUserNumber) == TRUE)
	{
		if(gParty.Isleader(pIndex,this->m_UserData.PartyLeaderUserNumber,this->m_UserData.PartyLeaderDBNumber) == FALSE)
		{
			return 0;
		}
	}else
	{
		return 0;
	}

	for(int i = 0; i < MAX_USER_IN_PARTY; i++)
	{
		int pId = gParty.m_PartyS[pIndex].Number[i];

		if ( pId < 0 || pId > (OBJMAX-1) )
		{
			
		}else
		{
			if(gObjIsConnected(gParty.m_PartyS[pIndex].Number[i]) == TRUE)
			{
				gObjMoveGate(gParty.m_PartyS[pIndex].Number[i],cImperialGuardian_Gates[this->ActiveDay].stage1Start);
			}
		}
	}
	LogAddC(11, "[Imperial Guardian] (Day:%d) State -> ENTRY_PARTY PartyNumber: %d, PartyLeader:(%d) %s", this->ActiveDay,this->m_UserData.PartyIndex,this->m_UserData.PartyLeaderUserNumber,gObj[this->m_UserData.PartyLeaderUserNumber].Name);
	return 1;
}


void cImperialGuardian::SetMonsterDifficulty(int MobID)
{
	gObj[MobID].Level = gObj[MobID].Level * this->Difficulty;
	gObj[MobID].Life = gObj[MobID].Life * this->Difficulty;
	gObj[MobID].MaxLife = gObj[MobID].MaxLife * this->Difficulty;
	gObj[MobID].m_iScriptMaxLife = gObj[MobID].m_iScriptMaxLife * this->Difficulty;
	gObj[MobID].m_AttackDamageMin = gObj[MobID].m_AttackDamageMin * this->Difficulty;
	gObj[MobID].m_AttackDamageMax = gObj[MobID].m_AttackDamageMax * this->Difficulty;
	gObj[MobID].m_AttackRating = gObj[MobID].m_AttackRating * this->Difficulty;
	gObj[MobID].m_AttackSpeed = gObj[MobID].m_AttackSpeed * this->Difficulty;
	gObj[MobID].m_Defense = gObj[MobID].m_Defense * this->Difficulty;
	gObj[MobID].m_ItemRate = gObj[MobID].m_ItemRate * this->Difficulty;
	gObj[MobID].m_MoneyRate = gObj[MobID].m_MoneyRate * this->Difficulty;
}

//void cImperialGuardian::ReleaseCastleDoor(int Gate)
//{
//	if((this->ActiveDay != 0) && (Gate > 6))
//		return;
//
//	LogAddC(4, "[Imperial Guardian] (Day:%d) State -> DOOR %d TERMINATED PartyNumber: %d, PartyLeader:(%d) %s", this->ActiveDay,Gate,this->m_UserData.PartyIndex,this->m_UserData.PartyLeaderUserNumber,gObj[this->m_UserData.PartyLeaderUserNumber].Name);
//
//	int Index = cImperialGuardian__Map[this->ActiveDay] - MAP_INDEX_EMPIREGUARDIAN1;
//
//	for ( int y=::g_iGuardianDoorMapXY[Index][Gate].btStartX; y <= ::g_iGuardianDoorMapXY[Index][Gate].btEndX ;y++)
//	{
//		for ( int z = ::g_iGuardianDoorMapXY[Index][Gate].btStartY; z <= ::g_iGuardianDoorMapXY[Index][Gate].btEndY ; z++)
//		{
//			MapC[cImperialGuardian__Map[this->ActiveDay]].m_attrbuf[z * 256 + y] &= ~4;
//		}
//	}
//}
//
//void cImperialGuardian::BlockCastleDoor(int Gate)
//{
//	if((this->ActiveDay != 0) && (Gate > 6))
//		return;
//
//	int Index = cImperialGuardian__Map[this->ActiveDay] - MAP_INDEX_EMPIREGUARDIAN1;
//
//	for ( int y=::g_iGuardianDoorMapXY[Index][Gate].btStartX; y <= ::g_iGuardianDoorMapXY[Index][Gate].btEndX ;y++)
//	{
//		for ( int z = ::g_iGuardianDoorMapXY[Index][Gate].btStartY; z <= ::g_iGuardianDoorMapXY[Index][Gate].btEndY ; z++)
//		{
//			MapC[cImperialGuardian__Map[this->ActiveDay]].m_attrbuf[z * 256 + y] |= 4;
//		}
//	}
//}
//
//