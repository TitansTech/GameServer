// Kanturu.cpp: implementation of the CKanturu class.
//	GS-N	1.00.18	JPN	0x00581CD0	-	Completed
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Kanturu.h"
#include "KanturuMonsterMng.h"
#include "KanturuBattleUserMng.h"
#include "KanturuUtil.h"
#include "Gamemain.h"
#include "LogProc.h"
#include "..\include\ReadScript.h"
#include "LogToFile.h"

#if (GS_CASTLE==0)

extern CLogToFile ANTI_HACK_LOG;
static CKanturuUtil KANTURU_UTIL;
CKanturu g_Kanturu;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CKanturu::CKanturu()
{
	this->m_bKanturuEnable = FALSE;
	this->m_iKanturuState = 0;
	this->m_StateInfoCount = 0;
	this->m_bFileDataLoad = FALSE;
	this->m_bEnableCheckMoonStone = FALSE;
	this->m_iKanturuBattleCounter = 0;
	this->m_iKanturuBattleDate = 0;
}

CKanturu::~CKanturu()
{
	return;
}



void CKanturu::ResetAllData()
{
	this->m_StateInfoCount = 0;

	for ( int iCount=0;iCount<MAX_KANTURU_STATE_INFO;iCount++)
	{
		this->m_StateInfo[iCount].ResetTimeInfo();
	}
}



BOOL CKanturu::LoadData(LPSTR lpszFileName)
{
	this->m_bFileDataLoad = FALSE;

	if ( !lpszFileName || !strcmp(lpszFileName, ""))
	{
		MsgBox("[Kanturu] - File load error : File Name Error");
		return FALSE;
	}

	try
	{

		SMDFile = fopen(lpszFileName, "r");

		if ( SMDFile == NULL )
		{
			MsgBox("[Kanturu] - Can't Open %s ", lpszFileName);
			return FALSE;
		}

		this->ResetAllData();

		enum SMDToken Token;
		int iType = -1;
		int iState = 0;
		int iCondition = 0;
		int iValue = 0;

		while ( true )
		{
			Token = (SMDToken)GetToken();
			
			if ( Token ==  END )
				break;
			
			iType = TokenNumber;

			while ( true )
			{
				if ( iType == 0 )
				{
					iState = 0;
					iCondition = 0;
					iValue = 0;

					Token = (SMDToken)GetToken();
					
					if ( !strcmp("end", TokenString))
						break;

					iState = TokenNumber;

					Token = (SMDToken)GetToken();
					iCondition = TokenNumber;

					Token = (SMDToken)GetToken();
					iValue = TokenNumber;

					if ( this->m_StateInfoCount < 0 || this->m_StateInfoCount >= MAX_KANTURU_STATE_INFO )
					{
						MsgBox("[Kanturu] - Exceed Max State Time (%d)", this->m_StateInfoCount);
						break;
					}

					this->m_StateInfo[this->m_StateInfoCount].SetStateInfo(iState);
					this->m_StateInfo[this->m_StateInfoCount].SetCondition(iCondition);

					if ( iCondition == 1 )
						iValue *= 1000;

					this->m_StateInfo[this->m_StateInfoCount].SetValue(iValue);

					this->m_StateInfoCount++;
				}
				else if ( iType == 10 )
				{
					BOOL bEnableCheckMoonStone = FALSE;

					Token = (SMDToken)GetToken();
					
					if ( !strcmp("end", TokenString))
						break;

					bEnableCheckMoonStone = TokenNumber;
					this->SetEnableCheckMoonStone(bEnableCheckMoonStone);
				}
				else
				{
					Token = (SMDToken)GetToken();
					
					if ( !strcmp("end", TokenString))
						break;
				}
			}
		}

		fclose(SMDFile);
		LogAddTD("[Kanturu] - %s file is Loaded", lpszFileName);
		
		this->m_bFileDataLoad = TRUE;

		// Load Other Resources from Kanturu.dat
		this->m_BattleStanby.LoadData(ReadConfig.ConnDataFiles[16]);
		this->m_BattleOfMaya.LoadData(ReadConfig.ConnDataFiles[16]);
		this->m_BattleOfNightmare.LoadData(ReadConfig.ConnDataFiles[16]);
		this->m_TowerOfRefinement.LoadData(ReadConfig.ConnDataFiles[16]);
	}
	catch ( DWORD )
	{
		MsgBox("[Kanturu] - Loading Exception Error (%s) File. ", lpszFileName);
	}

	return this->m_bFileDataLoad;
}




void CKanturu::LoadKanturuMapAttr(LPSTR lpszFileName, BYTE btLevel)
{
	if ( !lpszFileName || btLevel > MAX_KANTURU_MAP_LEVEL )
		return;

	switch ( btLevel )
	{
		case KANTURU_MAP_CLOSE:
			this->m_KanturuMap[btLevel].LoadMapAttr(lpszFileName, MAP_INDEX_KANTURU_BOSS);
			break;
		case KANTURU_MAP_OPEN:
			this->m_KanturuMap[btLevel].LoadMapAttr(lpszFileName, MAP_INDEX_KANTURU_BOSS);
			break;
	}
}



void CKanturu::SetKanturuMapAttr(BYTE btLevel)
{
	MapClass & KanturuMap = this->m_KanturuMap[btLevel];
	memcpy(MapC[MAP_INDEX_KANTURU_BOSS].m_attrbuf, KanturuMap.m_attrbuf, 256*256);

	LogAddC(2, "[Kanturu][Map Attr Change] Map(%d) State(%d) DetailState(%d)",
		btLevel, this->GetKanturuState(), this->GetKanturuDetailState());

}



void CKanturu::Run()
{
	if ( !this->IsKanturuEnable() )
		return;

	this->CheckStateTime();
	this->CheckUserOnKanturuBossMap();

	switch ( this->m_iKanturuState )
	{
		case KANTURU_STATE_NONE:
			this->ProcState_NONE();
			break;
		case KANTURU_STATE_BATTLE_STANTBY:
			this->ProcState_BATTLE_STANDBY();
			break;
		case KANTURU_STATE_BATTLE_OF_MAYA:
			this->ProcState_BATTLE_OF_MAYA();
			break;
		case KANTURU_STATE_BATTLE_OF_NIGHTMARE:
			this->ProcState_BATTLE_OF_NIGHTMARE();
			break;
		case KANTURU_STATE_TOWER_OF_REFINEMENT:
			this->ProcState_TOWER_OF_REFINEMENT();
			break;
		case KANTURU_STATE_END:
			this->ProcState_END();
			break;
	}
}


void CKanturu::SetState(int iKanturuState)
{
	if ( this->m_StateInfo[iKanturuState].GetCondition() > 0 )
	{
		this->m_StateInfo[iKanturuState].SetAppliedTime();
	}

	g_KanturuMonsterMng.ResetRegenMonsterObjData();
	this->SetKanturuMapAttr(0);

	switch ( iKanturuState )
	{
		case KANTURU_STATE_NONE:
			this->SetState_NONE();
			break;
		case KANTURU_STATE_BATTLE_STANTBY:
			this->SetState_BATTLE_STANDBY();
			break;
		case KANTURU_STATE_BATTLE_OF_MAYA:
			this->SetState_BATTLE_OF_MAYA();
			break;
		case KANTURU_STATE_BATTLE_OF_NIGHTMARE:
			this->SetState_BATTLE_OF_NIGHTMARE();
			break;
		case KANTURU_STATE_TOWER_OF_REFINEMENT:
			this->SetState_TOWER_OF_REFINEMENT();
			break;
		case KANTURU_STATE_END:
			this->SetState_END();
			break;
	}
}


void CKanturu::SetNextState(int iCurrentState)
{
	int iNextState = iCurrentState + 1;

	if ( iNextState >= MAX_KANTURU_STATE_INFO )
	{
		iNextState = KANTURU_STATE_NONE;
	}

	this->SetState(iNextState);
}


#pragma warning ( disable : 4060 )
void CKanturu::ChangeState(int iState, int DetailState)
{
	switch ( iState )
	{
	}
}
#pragma warning ( default : 4060 )

void CKanturu::SetState_NONE()
{
	LogAddC(7, "[Kanturu] State(%d) -> NONE", this->m_iKanturuState);
	this->SetKanturuState(KANTURU_STATE_NONE);
}


void CKanturu::SetState_BATTLE_STANDBY()
{
	LogAddC(7, "[Kanturu] State(%d) -> STANDBY", this->m_iKanturuState);
	g_KanturuBattleUserMng.ResetAllData();
	this->SetKanturuState(KANTURU_STATE_BATTLE_STANTBY);
	this->m_BattleStanby.SetState(1);
	this->SetKanturuTimeAttackEventInfo();
}


void CKanturu::SetState_BATTLE_OF_MAYA()
{
	LogAddC(7, "[Kanturu] State(%d) -> BATTLE_OF_MAYA", this->m_iKanturuState);
	this->SetKanturuState(KANTURU_STATE_BATTLE_OF_MAYA);
	this->m_BattleOfMaya.SetState(1);
}


void CKanturu::SetState_BATTLE_OF_NIGHTMARE()
{
	LogAddC(7, "[Kanturu] State(%d) -> BATTLE_OF_NIGHTMARE", this->m_iKanturuState);
	this->SetKanturuState(KANTURU_STATE_BATTLE_OF_NIGHTMARE);
	this->m_BattleOfNightmare.SetState(1);
}


void CKanturu::SetState_TOWER_OF_REFINEMENT()
{
	LogAddC(7, "[Kanturu] State(%d) -> TOWER_OF_REFINEMENT", this->m_iKanturuState);
	this->SetKanturuState(KANTURU_STATE_TOWER_OF_REFINEMENT);
	this->m_TowerOfRefinement.SetState(1);
	this->SetKanturuMapAttr(1);
}


void CKanturu::SetState_END()
{
	LogAddC(7, "[Kanturu] State(%d) -> END", this->m_iKanturuState);
	this->SetKanturuState(KANTURU_STATE_END);
	g_KanturuBattleUserMng.ResetAllData();
}


void CKanturu::ProcState_NONE()
{
	this->SetState(KANTURU_STATE_BATTLE_STANTBY);
}


void CKanturu::ProcState_BATTLE_STANDBY()
{
	if ( this->m_BattleStanby.GetBattleStanbyState() == 3 )
	{
		this->m_BattleStanby.SetState_NONE();
		this->SetState(KANTURU_STATE_BATTLE_OF_MAYA);
	}
	else
	{
		this->m_BattleStanby.Run();
	}
}


void CKanturu::ProcState_BATTLE_OF_MAYA()
{
	if ( this->m_BattleOfMaya.GetBattleOfMayaState() == 18 )
	{
		this->m_BattleOfMaya.SetState_NONE();

		if ( this->m_BattleOfMaya.GetSuccessValue() == 1 )
		{
			this->SetState(KANTURU_STATE_BATTLE_OF_NIGHTMARE);
		}
		else
		{
			this->SetState(KANTURU_STATE_END);
		}
	}
	else
	{
		this->m_BattleOfMaya.Run();
	}
}


void CKanturu::ProcState_BATTLE_OF_NIGHTMARE()
{
	if ( this->m_BattleOfNightmare.GetBattleOfNightmareState() == 5 )
	{
		this->m_BattleOfNightmare.SetState(0);

		if ( this->m_BattleOfNightmare.GetSuccessValue() == 1 )
		{
			this->SetState(KANTURU_STATE_TOWER_OF_REFINEMENT);
		}
		else
		{
			this->SetState(KANTURU_STATE_END);
		}
	}
	else
	{
		this->m_BattleOfNightmare.Run();
	}
}


void CKanturu::ProcState_TOWER_OF_REFINEMENT()
{
	if ( this->m_TowerOfRefinement.GetTowerOfRefinementState() == 5 )
	{
		this->m_TowerOfRefinement.SetState(0);
		this->SetState(KANTURU_STATE_END);
	}
	else
	{
		this->m_TowerOfRefinement.Run();
	}
}


void CKanturu::ProcState_END()
{
	this->SetState(KANTURU_STATE_NONE);
}


int CKanturu::GetKanturuDetailState()
{
	int iCurrentDetailState = KANTURU_STATE_NONE;

	switch ( this->GetKanturuState() )
	{
		case KANTURU_STATE_BATTLE_STANTBY:
			iCurrentDetailState = this->m_BattleStanby.GetBattleStanbyState();
			break;
		case KANTURU_STATE_BATTLE_OF_MAYA:
			iCurrentDetailState = this->m_BattleOfMaya.GetBattleOfMayaState();
			break;
		case KANTURU_STATE_BATTLE_OF_NIGHTMARE:
			iCurrentDetailState = this->m_BattleOfNightmare.GetBattleOfNightmareState();
			break;
		case KANTURU_STATE_TOWER_OF_REFINEMENT:
			iCurrentDetailState = this->m_TowerOfRefinement.GetTowerOfRefinementState();
			break;
	}

	return iCurrentDetailState;
}


void CKanturu::CheckStateTime()
{
	if ( this->GetKanturuState() )
	{
		int iState = this->GetKanturuState();

		if ( this->m_StateInfo[iState].GetCondition() == 1 )
		{
			if ( this->m_StateInfo[iState].IsTimeOut() == TRUE )
			{
				this->SetNextState(iState);
			}
		}
	}
}



int CKanturu::GetRemainTime()
{
	int iRemainTime = 0;

	switch ( this->GetKanturuState() )
	{
		case KANTURU_STATE_BATTLE_STANTBY:
			iRemainTime = this->m_BattleStanby.GetRemainTime();
			break;
		case KANTURU_STATE_BATTLE_OF_MAYA:
			iRemainTime = this->m_BattleOfMaya.GetRemainTime();
			break;
		case KANTURU_STATE_BATTLE_OF_NIGHTMARE:
			iRemainTime = this->m_BattleOfNightmare.GetRemainTime();
			break;
		case KANTURU_STATE_TOWER_OF_REFINEMENT:
			iRemainTime = this->m_TowerOfRefinement.GetRemainTime();
			break;
	}

	return iRemainTime;
}



void CKanturu::KanturuMonsterDieProc(int iMonIndex, int iKillerIndex)
{
	g_KanturuMonsterMng.MonsterDie(iMonIndex);	
}


void CKanturu::CheckUserOnKanturuBossMap()
{
	for ( int iCount=OBJ_STARTUSERINDEX;iCount<OBJMAX;iCount++)
	{
		if ( gObj[iCount].Connected == PLAYER_PLAYING &&
			 gObj[iCount].Type == OBJ_USER &&
			 gObj[iCount].MapNumber == MAP_INDEX_KANTURU_BOSS)
		{
			if ( gObj[iCount].m_bKanturuEntranceByNPC == 0 )
			{
				if ( (gObj[iCount].Authority&2) != 2 )
				{
					if ( gObj[iCount].RegenOk == 0 &&
						 gObj[iCount].m_State == 2 &&
						 gObj[iCount].Live == TRUE)
					{
						gObjMoveGate(iCount, 17);

						LogAddC(2, "[Kanturu][Invalid User] Invalid Kanturu Boss Map User[%s][%s]",
							gObj[iCount].AccountID, gObj[iCount].Name);
					}
				}
			}
		}
	}
}


int CKanturu::CheckEnterKanturu(int iUserIndex)
{
	if ( !gObjIsConnected(iUserIndex) )
		return -1;

	if ( gObj[iUserIndex].m_IfState.type != 19 )
	{
		ANTI_HACK_LOG.Output("[ANTI-HACK][Kanturu][Entrance Fail] User in map(%d) [%s][%s] State(u:%d,t:%d,s:%d)",
			gObj[iUserIndex].MapNumber, gObj[iUserIndex].AccountID, gObj[iUserIndex].Name,
			gObj[iUserIndex].m_IfState.use,gObj[iUserIndex].m_IfState.type,gObj[iUserIndex].m_IfState.state);

		return 3;
	}

	gObj[iUserIndex].m_IfState.use = 0;
	gObj[iUserIndex].m_IfState.type = 0;

	if ( gObj[iUserIndex].MapNumber != MAP_INDEX_KANTURU2 ) 
	{
		LogAddTD("[Kanturu][Entrance Fail] Invalid Map Number(%d) [%s][%s] State(%d)",
			gObj[iUserIndex].MapNumber, gObj[iUserIndex].AccountID, gObj[iUserIndex].Name,
			this->GetKanturuState());

		return 4;
	}

	if ( this->GetKanturuState() == KANTURU_STATE_BATTLE_OF_MAYA &&
		 this->m_BattleOfMaya.GetEntrancePermit() == TRUE)
	{
		if ( g_KanturuBattleUserMng.IsOverMaxUser() == TRUE )
		{
			LogAddTD("[Kanturu][Entrance Fail] Over Max User [%s][%s] State(%d)-(%d)",
				gObj[iUserIndex].AccountID, gObj[iUserIndex].Name,
				this->GetKanturuState(), this->m_BattleOfMaya.GetBattleOfMayaState());

			return 1;
		}

		if ( this->CheckEqipmentMoonStone(iUserIndex) == FALSE )
		{
			LogAddTD("[Kanturu][Entrance Fail] Moon Stone is not exist [%s][%s] State(%d)-(%d)",
				gObj[iUserIndex].AccountID, gObj[iUserIndex].Name,
				this->GetKanturuState(), this->m_BattleOfMaya.GetBattleOfMayaState());

			return 2;
		}

		if ( (gObj[iUserIndex].pInventory[7].m_Type < ITEMGET(12,0) || gObj[iUserIndex].pInventory[7].m_Type > ITEMGET(12,6) ) &&
			 (gObj[iUserIndex].pInventory[7].m_Type < ITEMGET(12,36) || gObj[iUserIndex].pInventory[7].m_Type > ITEMGET(12,43) ) &&
			 (gObj[iUserIndex].pInventory[7].m_Type < ITEMGET(12,130) || gObj[iUserIndex].pInventory[7].m_Type > ITEMGET(12,135) ) &&
			 gObj[iUserIndex].pInventory[7].m_Type != ITEMGET(13,30) &&
			 gObj[iUserIndex].pInventory[7].m_Type != ITEMGET(12,49) &&
			 gObj[iUserIndex].pInventory[7].m_Type != ITEMGET(12,50) &&
			 gObj[iUserIndex].pInventory[8].m_Type != ITEMGET(13,3)  &&
			 gObj[iUserIndex].pInventory[8].m_Type != ITEMGET(13,37) 		
#if (CRYSTAL_EDITION == 1)
			&& (!(gObj[iUserIndex].pInventory[7].m_Type >= ITEMGET(12,200) && gObj[iUserIndex].pInventory[7].m_Type <= ITEMGET(12,254)))
#endif
			 )
		{
			LogAddTD("[Kanturu][Entrance Fail] Wing Item is not exist [%s][%s] State(%d)-(%d)",
				gObj[iUserIndex].AccountID, gObj[iUserIndex].Name,
				this->GetKanturuState(), this->m_BattleOfMaya.GetBattleOfMayaState());

			return 7;
		}

		if ( gObj[iUserIndex].pInventory[8].m_Type == ITEMGET(13,2) )
		{
			LogAddTD("[Kanturu][Entrance Fail] Uniria Item is exist [%s][%s] State(%d)-(%d)",
				gObj[iUserIndex].AccountID, gObj[iUserIndex].Name,
				this->GetKanturuState(), this->m_BattleOfMaya.GetBattleOfMayaState());

			return 5;
		}

		if ( gObj[iUserIndex].pInventory[11].m_Type == ITEMGET(13,10) ||
			gObj[iUserIndex].pInventory[10].m_Type == ITEMGET(13,10) )
		{
			LogAddTD("[Kanturu][Entrance Fail] Trasportation Ring is exist [%s][%s] State(%d)-(%d)",
				gObj[iUserIndex].AccountID, gObj[iUserIndex].Name,
				this->GetKanturuState(), this->m_BattleOfMaya.GetBattleOfMayaState());

			return 6;
		}

		if ( gObj[iUserIndex].pInventory[11].m_Type == ITEMGET(13,39) ||
			gObj[iUserIndex].pInventory[10].m_Type == ITEMGET(13,39) )
		{
			LogAddTD("[Kanturu][Entrance Fail] EliteSkelletoneChangeRing is exist [%s][%s] State(%d)-(%d)",
				gObj[iUserIndex].AccountID, gObj[iUserIndex].Name,
				this->GetKanturuState(), this->m_BattleOfMaya.GetBattleOfMayaState());

			return 6;
		}

		LogAddTD("[Kanturu][Entrance Success] [%s][%s] State(%d)-(%d)",
			gObj[iUserIndex].AccountID, gObj[iUserIndex].Name,
			this->GetKanturuState(), this->m_BattleOfMaya.GetBattleOfMayaState());

		return 0;
	}

	if ( this->GetKanturuState() == KANTURU_STATE_TOWER_OF_REFINEMENT &&
		 this->m_TowerOfRefinement.GetEntrancePermit() == TRUE )
	{
		LogAddTD("[Kanturu][Entrance Success] [%s][%s] State(%d)-(%d)",
			gObj[iUserIndex].AccountID, gObj[iUserIndex].Name,
			this->GetKanturuState(), this->m_TowerOfRefinement.GetTowerOfRefinementState());

		return 0;
	}

	LogAddTD("[Kanturu][Entrance Fail] [%s][%s] State(%d)",
		gObj[iUserIndex].AccountID, gObj[iUserIndex].Name,
		this->GetKanturuState());

	return 3;
}



BOOL CKanturu::CheckCanEnterKanturuBattle()
{
	if ( this->GetKanturuState() == KANTURU_STATE_BATTLE_OF_MAYA &&
		 this->m_BattleOfMaya.GetEntrancePermit() == TRUE )
	{
		return TRUE;
	}

	if ( this->GetKanturuState() == KANTURU_STATE_TOWER_OF_REFINEMENT &&
		 this->m_TowerOfRefinement.GetEntrancePermit() == TRUE &&
		 this->m_TowerOfRefinement.IsUseTowerOfRefinement() == TRUE)
	{
		return TRUE;
	}

	return FALSE;
}


BOOL CKanturu::CheckEqipmentMoonStone(int iUserIndex)
{
	if(OBJMAX_RANGE(iUserIndex) == 0)
	{
		return false;
	}

	if ( !this->GetEnableCheckMoonStone() )
		return TRUE;

	if ( gObj[iUserIndex].pInventory[10].IsItem() ||
		 gObj[iUserIndex].pInventory[11].IsItem() )	
	{
		if ( gObj[iUserIndex].pInventory[10].m_Type == ITEMGET(13,38) &&
			 gObj[iUserIndex].pInventory[10].m_Durability != 0.0f )
		{
			return TRUE;
		}

		if ( gObj[iUserIndex].pInventory[11].m_Type == ITEMGET(13,38) &&
			 gObj[iUserIndex].pInventory[11].m_Durability != 0.0f )
		{
			return TRUE;
		}
	}

	return FALSE;
}


#pragma warning ( disable : 4101 )
void CKanturu::OperateGmCommand(int iUserIndex, int iCommand)
{
	return;
	{
		int iCurrentState[3];///???
	}
}
#pragma warning ( default : 4101 )

#include "LogToFile.h"
CLogToFile KANTURU_TEST_LOG("KANTURU_TEST_LOG", ".\\KANTURU_TEST_LOG", TRUE);



void CKanturu::UserMonsterCountCheck()
{
	return;

	int iUserCount_Live = 0;
	int iUserCount_Die = 0;
	for ( int iAllUserCount=OBJ_STARTUSERINDEX;iAllUserCount<OBJMAX;iAllUserCount++)
	{
		if ( gObj[iAllUserCount].MapNumber == MAP_INDEX_KANTURU_BOSS &&
			 gObj[iAllUserCount].Type == OBJ_USER &&
			 (gObj[iAllUserCount].Authority&2) != 2 )
		{
			if ( gObj[iAllUserCount].Live == TRUE )
			{
				iUserCount_Live++;
			}
			else
			{
				iUserCount_Die++;
			}
		}
	}

	KANTURU_TEST_LOG.Output("[Kanturu][Debug] 2. All User Check:%d/(Die:%d)", iUserCount_Live, iUserCount_Die);
	KANTURU_TEST_LOG.Output("[Kanturu][Debug] 3. MonsterMng Count:%d", g_KanturuMonsterMng.GetAliveMonsterCount());

	int iMonsterCount=0;

	for ( int iAllMonsterCount=0;iAllMonsterCount<OBJ_STARTUSERINDEX;iAllMonsterCount++)
	{
		if ( gObj[iAllMonsterCount].MapNumber == MAP_INDEX_KANTURU_BOSS &&
			 gObj[iAllMonsterCount].Connected == PLAYER_PLAYING &&
			 gObj[iAllMonsterCount].Type == OBJ_MONSTER )
		{
			if ( gObj[iAllMonsterCount].Class != 105 &&
				 gObj[iAllMonsterCount].Class != 106 &&
				 gObj[iAllMonsterCount].Class != 364 )
			{
				iMonsterCount++;

				KANTURU_TEST_LOG.Output("[Kanturu][Debug] 4. Monster Info %s(%d/ObjIndex:%d) [%d]%d-%d HP:%0.1f IsAlive:%d Connected:%d State:%d ActState:Attack(%d)/Move(%d) DieRegen:%d",
					gObj[iAllMonsterCount].Name, gObj[iAllMonsterCount].Class,
					gObj[iAllMonsterCount].m_Index, gObj[iAllMonsterCount].MapNumber,
					gObj[iAllMonsterCount].X, gObj[iAllMonsterCount].Y,
					gObj[iAllMonsterCount].Life, gObj[iAllMonsterCount].Live,
					gObj[iAllMonsterCount].Connected, gObj[iAllMonsterCount].m_State,
					gObj[iAllMonsterCount].m_ActState.Attack, gObj[iAllMonsterCount].m_ActState.Move,
					gObj[iAllMonsterCount].DieRegen);
			}
		}
	}
}

void CKanturu::SetKanturuTimeAttackEventInfo()
{
	tm * today;
	time_t ltime;

	time(&ltime);
	today = localtime(&ltime);
	today->tm_year += 1900;

	int iYear = today->tm_year * 10000;
	int iMonth = ( today->tm_mon + 1 ) * 100;
	int iDay = today->tm_mday;
	int iDateInfo = iYear + iMonth + iDay;

	if ( this->m_iKanturuBattleDate < iDateInfo )
	{
		this->m_iKanturuBattleDate = iDateInfo;
		this->m_iKanturuBattleCounter = 0;
	}

	this->m_iKanturuBattleCounter++;

	LogAddTD("[Kanturu][TimeAttackEvent] Date:%d, Counter:%d",
		this->m_iKanturuBattleDate, this->m_iKanturuBattleCounter);
}

#endif