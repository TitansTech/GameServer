// ------------------------------
// Decompiled by Deathway
// Date : 2007-05-09
// ------------------------------
// GS-N 0.99.60T 0x005004D0 - Completed
/* Disorder with same effect in CBloodCastle::LevelUp
		GCLevelUpMsgSend(gObj[iIndex].Index, gObj[iIndex].Level, gObj[iIndex].LevelUpPoint, 
			(int)((float)gObj[iIndex].unk104 + gObj[iIndex].fMaxLife), (int)((float)gObj[iIndex].unk108 + gObj[iIndex].fMaxMana),
			gObj[iIndex].iMaxBP + gObj[iIndex].unkE8, AddPoint, MaxAddPoint);
   Problem with global variables, there is a Zero ( 4 BYTES ) space
*/
// GS-N	1.00.18	0x005303F0	-	Completed


#include "stdafx.h"
#include "BloodCastle.h"
#include "ChaosCastle.h"
#include "GameMain.h"
#include "GameServer.h"
////#include "GameServerAuth.h"
#include "logproc.h"
#include "..\include\readscript.h"
#include "DSProtocol.h"
#include "Protocol.h"
#include "TNotice.h"
#include "..\common\winutil.h"
#include "ObjUseSkill.h"
#include "ChaosBox.h"
#include "CastleSiegeSync.h"
#include "DSProtocol.h"
#include "RProtocol.h"
#include "CrywolfSync.h"
#include "Crywolf.h"
#include "HitAndUp.h"
#include "VIPSystem.h"
#include "QuestS5Info.h"
#include "MuItemShop.h"

CBloodCastle g_BloodCastle;

#include "LogToFile.h"
extern CLogToFile CHAOS_LOG;

static const struct BLOOD_ZONE
{
	BYTE btStartX;
	BYTE btStartY;
	BYTE btEndX;
	BYTE btEndY;

} g_btCastleEntranceMapXY[MAX_BLOOD_CASTLE_LEVEL] = {

	13, 15, 15, 23,	// Blood Castle 1
	13, 15, 15, 23,	// Blood Castle 2
	13, 15, 15, 23,	// Blood Castle 3
	13, 15, 15, 23,	// Blood Castle 4
	13, 15, 15, 23,	// Blood Castle 5
	13, 15, 15, 23,	// Blood Castle 6
	13, 15, 15, 23,	// Blood Castle 7
	13, 15, 15, 23	// Blood Castle 8

}, g_btCastleBridgeMapXY[MAX_BLOOD_CASTLE_LEVEL] = {

	13, 70, 15, 75,	// Bridge of Blood Castle 1
	13, 70, 15, 75,	// Bridge of Blood Castle 2
	13, 70, 15, 75,	// Bridge of Blood Castle 3
	13, 70, 15, 75,	// Bridge of Blood Castle 4
	13, 70, 15, 75,	// Bridge of Blood Castle 5
	13, 70, 15, 75,	// Bridge of Blood Castle 6
	13, 70, 15, 75,	// Bridge of Blood Castle 7
	13, 70, 15, 75	// Bridge of Blood Castle 8

}, g_btCastleDoorMapXY[MAX_BLOOD_CASTLE_LEVEL][3]= {

	// Blood Castle 1
	13, 76, 15, 79,	// Door Itself
	11, 80, 25, 89,	// Zone Beginh Door
	 8, 80, 10, 83,	// Altar 

	// Blood Castle 2
	13, 76, 15, 79,	// Door Itself
	11, 80, 25, 89,	// Zone Beginh Door
	 8, 80, 10, 83,	// Altar 

	// Blood Castle 3
	13, 76, 15, 79,	// Door Itself
	11, 80, 25, 89,	// Zone Beginh Door
	 8, 80, 10, 83,	// Altar 

	// Blood Castle 4
	13, 76, 15, 79,	// Door Itself
	11, 80, 25, 89,	// Zone Beginh Door
	 8, 80, 10, 83,	// Altar 

	// Blood Castle 5
	13, 76, 15, 79,	// Door Itself
	11, 80, 25, 89,	// Zone Beginh Door
	 8, 80, 10, 83,	// Altar 

	// Blood Castle 6
	13, 76, 15, 79,	// Door Itself
	11, 80, 25, 89,	// Zone Beginh Door
	 8, 80, 10, 83,	// Altar 

	// Blood Castle 7
	13, 76, 15, 79,	// Door Itself
	11, 80, 25, 89,	// Zone Beginh Door
	 8, 80, 10, 83,	// Altar 
	// Blood Castle 8
	13, 76, 15, 79,	// Door Itself
	11, 80, 25, 89,	// Zone Beginh Door
	 8, 80, 10, 83	// Altar 
};

static const struct ST_REWARD_ZEN
{
	int NormalCharacter;
	int SpecialCharacter;

} g_iQuestWinExpendZEN[MAX_BLOOD_CASTLE_LEVEL] = {

	20000,	10000,
	50000,	25000,
	100000,	50000,
	150000,	80000,
	200000,	100000,
	250000,	120000,
	250000,	120000,
	250000,	120000

};


BOOL BC_MAP_RANGE(int Map)
{
	if (Map == MAP_INDEX_BLOODCASTLE8)
	{
		return TRUE;
	}
	if ((Map < MAP_INDEX_BLOODCASTLE1) || (Map > MAP_INDEX_BLOODCASTLE7))
	{
		return FALSE;
	}
	return TRUE;
}


/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/


CBloodCastle::CBloodCastle()
{
	this->m_bBC_EVENT_ENABLE = false;
	this->m_iBC_TIME_MIN_OPEN = 10;
	this->m_iBC_NORMAL_ITEM_DROP = 100;
	this->m_iBC_EXCEL_ITEM_DROP = 1200;
	this->m_iBC_MONSTER_REGEN = 0;

	for ( int i =0; i<MAX_BLOOD_CASTLE_LEVEL ; i++ )
	{
		this->OpenDoorCheck[i] = false;
		this->m_BridgeData[i].m_iBC_STATE = BC_STATE_NONE;
		
		if(i != 7)
			this->m_BridgeData[i].m_iMapNumber = i + MAP_INDEX_BLOODCASTLE1;
		else
			this->m_BridgeData[i].m_iMapNumber = i + 45;

		//this->m_BridgeData[i].m_iBridgeIndex = i;
		this->m_BridgeData[i].m_iBC_REMAIN_MSEC = -1;
		this->m_BridgeData[i].m_iBC_TICK_COUNT = -1;
		this->m_BridgeData[i].m_nSaintStatuePosNum = -1;
		this->m_BridgeData[i].m_nCastleDoorPosNum = -1;
		this->m_BridgeData[i].m_iAngelKingPosNum = -1;
		InitializeCriticalSection(&this->m_BridgeData[i].m_critUserData);
		this->ClearBridgeData(i);
	}
}




CBloodCastle::~CBloodCastle()
{
	for ( int i =0; i<MAX_BLOOD_CASTLE_LEVEL ; i++ )
	{
		DeleteCriticalSection(&this->m_BridgeData[i].m_critUserData);
	}
}




void CBloodCastle::Init(bool bEVENT_ENABLE)
{
	this->m_bBC_EVENT_ENABLE = bEVENT_ENABLE;

	for ( int i=0; i<MAX_BLOOD_CASTLE_LEVEL ; i++ )
	{
		this->OpenDoorCheck[i] = false;
		for ( int n=0;n<MAX_BLOOD_CASTLE_BOSS_MONSTER;n++)
		{
			this->m_BridgeData[i].m_iAngelKingPosNum = -1;
			this->m_BridgeData[i].m_nBossMonsterPosNum[n] = -1;
		}
	}

	for ( int n =0; n<gMSetBase.m_Count;n++)
	{
		if ( BC_MAP_RANGE(gMSetBase.m_Mp[n].m_MapNumber ) != FALSE )
		{
			WORD btMonsterType = gMSetBase.m_Mp[n].m_Type;
			BYTE btMapNumber = gMSetBase.m_Mp[n].m_MapNumber;
			BYTE btBloodCastleIndex = btMapNumber - MAP_INDEX_BLOODCASTLE1;

			if(btMapNumber == MAP_INDEX_BLOODCASTLE8)
			{
				btBloodCastleIndex = 7;
			}
			
			if ( btMonsterType == 232 )
			{
				this->m_BridgeData[btBloodCastleIndex/*gMSetBase.m_Mp[n].m_MapNumber - MAP_INDEX_BLOODCASTLE1*/].m_iAngelKingPosNum = n;
				continue;
			}
			
			if ( BC_STATUE_RANGE(btMonsterType - 132) != FALSE )
			{
				this->m_BridgeData[btBloodCastleIndex].m_nSaintStatuePosNum = n;
				continue;
			}
			
			if ( btMonsterType == 131 )
			{
				this->m_BridgeData[btBloodCastleIndex].m_nCastleDoorPosNum = n;
				continue;
			}
			
			if ( btMonsterType == 89 || btMonsterType == 95 || btMonsterType == 112 || btMonsterType == 118 || btMonsterType == 124 || btMonsterType == 130 || btMonsterType == 143 || btMonsterType == 433)
			{
				for ( int l=0;l<MAX_BLOOD_CASTLE_BOSS_MONSTER;l++)
				{
					if ( this->m_BridgeData[btBloodCastleIndex].m_nBossMonsterPosNum[l] == -1 )
					{
						this->m_BridgeData[btBloodCastleIndex].m_nBossMonsterPosNum[l] = n;
						break;
					}
				}
			}
		}

	}

	for ( int i=0;i<MAX_BLOOD_CASTLE_LEVEL;i++)
	{
		this->SetState(i, BC_STATE_CLOSED);
	}
}


void CBloodCastle::Load(char* filename)
{
	int Token;
	int type;
	int BridgeCount;
	int iBridgeNum;
	SMDFile=fopen(filename, "r");

	if (SMDFile == 0)
	{
		MsgBox("[Blood Castle] Info file Load Fail [%s]", filename);
		return;
	}

	type=-1;
	BridgeCount=-1;

	while ( true )
	{
		Token=GetToken();

		if ( Token == 2 )
		{
			break;
		}

		type=TokenNumber;

		while ( true )
		{
			if ( type == 0 )
			{
				Token=GetToken();

				if (strcmp("end", &TokenString[0]) == 0)
				{
					break;
				}

				this->m_iBC_TIME_MIN_OPEN = TokenNumber;

				Token = GetToken();
				this->m_iBC_TIME_MIN_PLAY =TokenNumber;

				Token = GetToken();
				this->m_iBC_TIME_MIN_REST = TokenNumber;
			}
			else if ( type == 1 )
			{
				Token = GetToken();

				if (strcmp("end", &TokenString[0]) == 0)
				{
					break;
				}

				this->m_iBC_NORMAL_ITEM_DROP = TokenNumber;

				Token = GetToken();
				this->m_iBC_EXCEL_ITEM_DROP = TokenNumber;

				Token = GetToken();
				this->m_iBC_MONSTER_REGEN = TokenNumber;
			}
			else if ( type == 2 )
			{
				Token = GetToken();

				if (strcmp("end", &TokenString[0]) == 0)
				{
					break;
				}
			
				iBridgeNum = TokenNumber;

				if ( BC_BRIDGE_RANGE(iBridgeNum) != FALSE )
				{
					Token = GetToken();
					this->m_BridgeData[iBridgeNum].m_iCastleStatueHealth = TokenNumber;

					Token = GetToken();
					this->m_BridgeData[iBridgeNum].m_iCastleDoorHealth = TokenNumber;
				}
				else
				{
					Token = GetToken();
					Token = GetToken();
				}
			}
		}
	}

	fclose(SMDFile);

	LogAdd("%s file load!", filename);
}




void CBloodCastle::LoadItemDropRate()
{
	//Blood Castle Event
	g_BloodCastle.Load(ReadConfig.ConnDataFiles[14]);
	g_bBloodCastle = GetPrivateProfileInt("GameServerInfo","BloodCastleEvent", 0, ReadConfig.ConnDataFiles[0]);
	g_iBloodCastle_StartHour = GetPrivateProfileInt("GameServerInfo","BloodCastleStartHour", 1, ReadConfig.ConnDataFiles[0]);
	g_iAngelKingsPaperDropRate = GetPrivateProfileInt("GameServerInfo","AngelKingsPaperDropRate", 0, ReadConfig.ConnDataFiles[0]);
	g_iBloodBoneDropRate = GetPrivateProfileInt("GameServerInfo","BloodBoneDropRate", 0, ReadConfig.ConnDataFiles[0]);
	g_iNpcAngelKingRemainTime = GetPrivateProfileInt("GameServerInfo", "SCFBCMissionReciveTime",900, ReadConfig.ConnDataFiles[0]) ;
	g_iBCDropChaosGemItemType = GetPrivateProfileInt("GameServerInfo", "SCFBCRewardItemType",12, ReadConfig.ConnDataFiles[0]) ;
	g_iBCDropChaosGemItemIndex = GetPrivateProfileInt("GameServerInfo", "SCFBCRewardItemID",15, ReadConfig.ConnDataFiles[0]) ;
	g_iBCDropChaosGemItemLevel = GetPrivateProfileInt("GameServerInfo", "SCFBCRewardItemLevel",0, ReadConfig.ConnDataFiles[0]) ;
	g_iBCDropChaosGemItemDur = GetPrivateProfileInt("GameServerInfo", "SCFBCRewardItemDur",0, ReadConfig.ConnDataFiles[0]) ;
	g_iBCDropChaosGemItemSkill = GetPrivateProfileInt("GameServerInfo", "SCFBCRewardItemSkill",0, ReadConfig.ConnDataFiles[0]) ;
	g_iBCDropChaosGemItemLuck = GetPrivateProfileInt("GameServerInfo", "SCFBCRewardItemLuck",0, ReadConfig.ConnDataFiles[0]) ;
	g_iBCDropChaosGemItemOpt = GetPrivateProfileInt("GameServerInfo", "SCFBCRewardItemOpt",0, ReadConfig.ConnDataFiles[0]) ;
	g_iBCDropChaosGemItemExc = GetPrivateProfileInt("GameServerInfo", "SCFBCRewardItemExc",0, ReadConfig.ConnDataFiles[0]) ;
}




void CBloodCastle::CheckSync(int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}

	tm * today;
	time_t ltime;

	time(&ltime);
	today = localtime(&ltime);

	if ( BC_TIME_RANGE(g_iBloodCastle_StartHour-1) == FALSE )
	{
		g_iBloodCastle_StartHour = 1;
	}

	switch ( g_iBloodCastle_StartHour )
	{
		case 2:
			
			if ( (today->tm_hour%2) == 0 )
			{
				if ( ( today->tm_min*60 + today->tm_sec ) >= 1800 )
				{
					this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC = (9000 - (today->tm_min*60 + today->tm_sec)) * 1000;
				}
				else
				{
					this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC = (1800 - (today->tm_min*60 + today->tm_sec)) * 1000;
				}
			}
			else
			{
				this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC = (5400 - (today->tm_min*60 + today->tm_sec)) * 1000;
			}

			if ( this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC < 0 )
			{
				this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC += 3600000;
			}

			this->m_BridgeData[iBridgeIndex].m_iBC_TICK_COUNT = GetTickCount();
			break;

		default:

			if ( ( today->tm_min*60 + today->tm_sec ) >= 1800 )
			{
				this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC = (5400 - (today->tm_min*60 + today->tm_sec)) * 1000;
			}
			else
			{
				this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC = (1800 - (today->tm_min*60 + today->tm_sec)) * 1000;
			}

			if ( this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC < 0 )
			{
				this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC += 3600000;
			}
			this->m_BridgeData[iBridgeIndex].m_iBC_TICK_COUNT = GetTickCount();

			break;

	}

	LogAddTD("[Blood Castle] (%d) Sync Open Time. [%d] min remain",
		iBridgeIndex+1, this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC / 60000);


}






void CBloodCastle::ClearBridgeData(int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}

	this->OpenDoorCheck[iBridgeIndex] = false;
	this->m_BridgeData[iBridgeIndex].m_iTOTAL_EXP = 0;
	this->m_BridgeData[iBridgeIndex].m_iBC_MONSTER_MAX_COUNT = 0;
	this->m_BridgeData[iBridgeIndex].m_iBC_MONSTER_KILL_COUNT = 0;
	this->m_BridgeData[iBridgeIndex].m_iBC_MONSTER_SUCCESS_MSG_COUNT = 0;
	this->m_BridgeData[iBridgeIndex].m_iBC_BOSS_MONSTER_MAX_COUNT = 0;
	this->m_BridgeData[iBridgeIndex].m_iBC_BOSS_MONSTER_KILL_COUNT = 0;
	this->m_BridgeData[iBridgeIndex].m_iBC_BOSS_MONSTER_SUCCESS_MSG_COUNT = 0;
	this->m_BridgeData[iBridgeIndex].m_btBC_QUEST_ITEM_NUMBER = 0;
	this->m_BridgeData[iBridgeIndex].m_dwBC_TICK_DOOR_OPEN = -1;
	this->m_BridgeData[iBridgeIndex].m_nBC_QUESTITEM_SERIAL = -1;
	this->m_BridgeData[iBridgeIndex].m_iBC_QUEST_ITEM_USER_INDEX = -1;
	this->m_BridgeData[iBridgeIndex].m_iMISSION_SUCCESS = -1;
	this->m_BridgeData[iBridgeIndex].m_iBC_NOTIFY_COUNT = -1;
	this->m_BridgeData[iBridgeIndex].m_bCASTLE_DOOR_LIVE = TRUE;
	this->m_BridgeData[iBridgeIndex].m_bBC_REWARDED = false;
	this->m_BridgeData[iBridgeIndex].m_bBC_MSG_BEFORE_ENTER = false;
	this->m_BridgeData[iBridgeIndex].m_bBC_MSG_BEFORE_PLAY = false;
	this->m_BridgeData[iBridgeIndex].m_bBC_MSG_BEFORE_END = false;
	this->m_BridgeData[iBridgeIndex].m_bBC_MSG_BEFORE_QUIT = false;
	this->m_BridgeData[iBridgeIndex].m_bBC_CAN_ENTER = false;
	this->m_BridgeData[iBridgeIndex].m_bBC_CAN_PARTY = false;
	this->m_BridgeData[iBridgeIndex].m_bBC_PLAY_START = false;
	this->m_BridgeData[iBridgeIndex].m_bBC_MONSTER_KILL_COMPLETE = false;
	this->m_BridgeData[iBridgeIndex].m_bBC_BOSS_MONSTER_KILL_COMPLETE = false;
	this->m_BridgeData[iBridgeIndex].m_bBC_DOOR_TERMINATE_COMPLETE = false;
	this->m_BridgeData[iBridgeIndex].m_iExtraEXP_Kill_Door_Index = -10;
	this->m_BridgeData[iBridgeIndex].m_iExtraEXP_Kill_Door_Party = -10;
	this->m_BridgeData[iBridgeIndex].m_iExtraEXP_Kill_Statue_Index = -10;
	this->m_BridgeData[iBridgeIndex].m_iExtraEXP_Kill_Statue_Party = -10;
	this->m_BridgeData[iBridgeIndex].m_iExtraEXP_Win_Quest_Index = -10;
	this->m_BridgeData[iBridgeIndex].m_iExtraEXP_Win_Quest_Party = -10;

	memset(this->m_BridgeData[iBridgeIndex].m_szKill_Door_CharName , 0, sizeof(this->m_BridgeData[iBridgeIndex].m_szKill_Door_CharName));
	memset(this->m_BridgeData[iBridgeIndex].m_szKill_Status_CharName, 0, sizeof(this->m_BridgeData[iBridgeIndex].m_szKill_Status_CharName));
	memset(this->m_BridgeData[iBridgeIndex].m_szWin_Quest_CharName, 0, sizeof(this->m_BridgeData[iBridgeIndex].m_szWin_Quest_CharName));
	memset(this->m_BridgeData[iBridgeIndex].m_szKill_Door_AccountID, 0, sizeof(this->m_BridgeData[iBridgeIndex].m_szKill_Door_AccountID));
	memset(this->m_BridgeData[iBridgeIndex].m_szKill_Status_AccountID, 0, sizeof(this->m_BridgeData[iBridgeIndex].m_szKill_Status_AccountID));
	memset(this->m_BridgeData[iBridgeIndex].m_szWin_Quest_AccountID, 0, sizeof(this->m_BridgeData[iBridgeIndex].m_szWin_Quest_AccountID));
	

	for ( int i=0;i<MAX_BLOOD_CASTLE_SUB_BRIDGE;i++)
	{
		if ( this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex != -1 )
		{
			gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_iBloodCastleEXP = 0;
			gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleIndex = -1;
			gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleSubIndex = -1;
			gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_bBloodCastleComplete = false;
		}

		this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iEXP = 0;
		this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iScore = 0;
		this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex = -1;
		this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState= 0;
		this->m_BridgeData[iBridgeIndex].m_UserData[i].m_bSendQuitMsg = false;
		this->m_BridgeData[iBridgeIndex].m_UserData[i].m_bLiveWhenDoorBreak = false;
		this->m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX = -1;
	}
}





void CBloodCastle::SetState(int iBridgeIndex, int iBC_STATE)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}

	if ( iBC_STATE < BC_STATE_NONE || iBC_STATE > BC_STATE_PLAYEND )
	{
		return;
	}

	this->m_BridgeData[iBridgeIndex].m_iBC_STATE = iBC_STATE;

	switch ( this->m_BridgeData[iBridgeIndex].m_iBC_STATE )
	{
		case BC_STATE_NONE:
			this->SetState_None(iBridgeIndex);
			break;

		case BC_STATE_CLOSED:
			this->SetState_Closed(iBridgeIndex);
			break;

		case BC_STATE_PLAYING:
			this->SetState_Playing(iBridgeIndex);
			break;

		case BC_STATE_PLAYEND:
			this->SetState_PlayEnd(iBridgeIndex);
			break;
	}
}





void CBloodCastle::Run()
{
	if ( this->m_bBC_EVENT_ENABLE != false )
	{
		for (int iBridgeIndex=0;iBridgeIndex<MAX_BLOOD_CASTLE_LEVEL;iBridgeIndex++)
		{
			switch ( this->m_BridgeData[iBridgeIndex].m_iBC_STATE )
			{
				case BC_STATE_NONE:
					this->ProcState_None(iBridgeIndex);
					break;

				case BC_STATE_CLOSED:
					this->ProcState_Closed(iBridgeIndex);
					break;

				case BC_STATE_PLAYING:
					this->ProcState_Playing(iBridgeIndex);
					break;

				case BC_STATE_PLAYEND:
					this->ProcState_PlayEnd(iBridgeIndex);
					break;
			}
		}
	}

}







	
void CBloodCastle::ProcState_None(int iBridgeIndex)
{
	return;
}




void CBloodCastle::ProcState_Closed(int iBridgeIndex)
{
	int iTICK_MSEC = GetTickCount() - this->m_BridgeData[iBridgeIndex].m_iBC_TICK_COUNT;

	if ( iTICK_MSEC >= 1000 )
	{
		this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC -= iTICK_MSEC;
		this->m_BridgeData[iBridgeIndex].m_iBC_TICK_COUNT = GetTickCount();

		if ( g_bBloodCastle != FALSE )
		{
			if ( this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC <= ( this->m_iBC_TIME_MIN_OPEN * 60 * 1000 ) && this->m_BridgeData[iBridgeIndex].m_bBC_CAN_ENTER == false)
			{
				this->m_BridgeData[iBridgeIndex].m_bBC_CAN_ENTER = true;
				this->m_BridgeData[iBridgeIndex].m_bBC_CAN_PARTY = true;
			}

			if ( this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC <= ( this->m_iBC_TIME_MIN_OPEN * 60 * 1000 ) && this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC > 0 && (this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC/60000) !=this->m_BridgeData[iBridgeIndex].m_iBC_NOTIFY_COUNT)
			{
				this->m_BridgeData[iBridgeIndex].m_iBC_NOTIFY_COUNT = this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC / 60000;

				if ( iBridgeIndex == 0 )
				{
					PMSG_NOTICE pNotice;
					pNotice.type = 0;	// 3
					pNotice.btCount = 0;	// 4
					pNotice.wDelay = 0;	// 6	
					pNotice.dwColor = 0;	// 8
					pNotice.btSpeed = 0;	// C

					TNotice::MakeNoticeMsgEx(&pNotice, 0, lMsg.Get( MSGGET(4, 136)), this->m_BridgeData[iBridgeIndex].m_iBC_NOTIFY_COUNT+1);
					this->SendAllUserAnyMsg((BYTE *)&pNotice, pNotice.h.size);
				}
			}

			if ( this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC <= 30000 && this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC > 0 && this->m_BridgeData[iBridgeIndex].m_bBC_MSG_BEFORE_ENTER == false )
			{
				this->m_BridgeData[iBridgeIndex].m_bBC_MSG_BEFORE_ENTER = true;

				if ( iBridgeIndex == 0 )
				{
					PMSG_SET_DEVILSQUARE pMsg;

					PHeadSetB((LPBYTE)&pMsg, 0x92, sizeof(pMsg));
					pMsg.Type = 3;

					for (int i= OBJ_STARTUSERINDEX;i<OBJMAX;i++)
					{
						if ( gObj[i].Connected == PLAYER_PLAYING && gObj[i].Type == OBJ_USER)
						{
							if ( BC_MAP_RANGE(gObj[i].MapNumber) == FALSE )
							{
								if ( CC_MAP_RANGE(gObj[i].MapNumber) == FALSE )
								{
									DataSend(i, (UCHAR*)&pMsg, pMsg.h.size);
								}
							}
						}
					}
				}
			}
		}
	}

	if ( this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC <= 0 )
	{
		if ( g_bBloodCastle != FALSE )
		{
			this->SetState(iBridgeIndex, BC_STATE_PLAYING);
		}
		else
		{
			this->SetState(iBridgeIndex, BC_STATE_CLOSED);
		}
	}
}






void CBloodCastle::ProcState_Playing(int iBridgeIndex)
{
	int iTICK_MSEC = GetTickCount() - this->m_BridgeData[iBridgeIndex].m_iBC_TICK_COUNT;

	if ( iTICK_MSEC >= 1000 )
	{
		this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC -= iTICK_MSEC;
		this->m_BridgeData[iBridgeIndex].m_iBC_TICK_COUNT = GetTickCount();

		if ( this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC <= ((this->m_iBC_TIME_MIN_PLAY*60-30)*1000) )
		{
			if ( this->m_BridgeData[iBridgeIndex].m_bBC_MSG_BEFORE_PLAY == false )
			{
				this->m_BridgeData[iBridgeIndex].m_bBC_MSG_BEFORE_PLAY = true;
				
				PMSG_SET_DEVILSQUARE pMsg;
				PHeadSetB((LPBYTE)&pMsg, 0x92, sizeof(pMsg));
				pMsg.Type = 4;
				this->SendBridgeAnyMsg((BYTE*)&pMsg, sizeof(pMsg), iBridgeIndex);
			}
		}

		if ( this->m_BridgeData[iBridgeIndex].m_bBC_MONSTER_KILL_COMPLETE != false	)
		{
			if ( this->m_BridgeData[iBridgeIndex].m_dwBC_TICK_DOOR_OPEN != -1 )
			{
				if ( GetTickCount() > this->m_BridgeData[iBridgeIndex].m_dwBC_TICK_DOOR_OPEN )
				{
					this->ReleaseCastleBridge(iBridgeIndex);
					this->SendCastleBridgeBlockInfo(iBridgeIndex, 0);
					LogAddTD("[Blood Castle] (%d) Bridge Change Bridge Attribute -> Open", iBridgeIndex+1);
					this->m_BridgeData[iBridgeIndex].m_dwBC_TICK_DOOR_OPEN = -1; // Prevent multiple openings
				}
			}
		}

		// Set Play Quest
		if ( this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC <= ((this->m_iBC_TIME_MIN_PLAY*60-60)*1000) && this->m_BridgeData[iBridgeIndex].m_bBC_PLAY_START == false )
		{
			PMSG_NOTICE pNotice;
			pNotice.type = 0;	// 3
			pNotice.btCount = 0;	// 4
			pNotice.wDelay = 0;	// 6	
			pNotice.dwColor = 0;	// 8
			pNotice.btSpeed = 0;	// C

			this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC = (this->m_iBC_TIME_MIN_PLAY*60)*1000;
			this->m_BridgeData[iBridgeIndex].m_bBC_CAN_PARTY = false;
			TNotice::MakeNoticeMsgEx((TNotice*)&pNotice, 0, lMsg.Get(MSGGET(4, 137)), iBridgeIndex+1);
			this->SendBridgeAnyMsg( (LPBYTE)&pNotice, pNotice.h.size, iBridgeIndex);
			this->ReleaseCastleEntrance(iBridgeIndex);
			this->SendCastleEntranceBlockInfo(iBridgeIndex, 0);
			this->m_BridgeData[iBridgeIndex].m_bBC_PLAY_START = true;
			this->SetMonster(iBridgeIndex);
			this->SendNoticeState(iBridgeIndex, false);

			LogAddTD("[Blood Castle] (%d) Blood Castle Quest Start", iBridgeIndex+1);
		}

		if ( this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC <= 30000 && this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC > 0 && this->m_BridgeData[iBridgeIndex].m_bBC_MSG_BEFORE_END == false)	// Set counter to kick
		{
			this->m_BridgeData[iBridgeIndex].m_bBC_MSG_BEFORE_END = true;
			PMSG_SET_DEVILSQUARE pMsg;
			PHeadSetB((LPBYTE)&pMsg, 0x92, sizeof(pMsg));
			pMsg.Type = 5;
			this->SendBridgeAnyMsg((BYTE *)&pMsg, sizeof(pMsg), iBridgeIndex);
		}

		if ( this->CheckEveryUserDie(iBridgeIndex) != false )
		{
			PMSG_NOTICE pNotice;
			pNotice.type = 0;	// 3
			pNotice.btCount = 0;	// 4
			pNotice.wDelay = 0;	// 6	
			pNotice.dwColor = 0;	// 8
			pNotice.btSpeed = 0;	// C

			TNotice::MakeNoticeMsg(&pNotice, 0, lMsg.Get(MSGGET(4, 138)));
			this->SendBridgeAnyMsg( (LPBYTE)&pNotice, pNotice.h.size, iBridgeIndex);

			LogAddTD("[Blood Castle] (%d) Blood Castle Quest Fail Result -> Destroy Castle Door [%s][%s]",
				iBridgeIndex+1, this->m_BridgeData[iBridgeIndex].m_szKill_Door_AccountID,
				this->m_BridgeData[iBridgeIndex].m_szKill_Door_CharName);

			LogAddTD("[Blood Castle] (%d) Blood Castle Quest Fail Result -> Destroy Saint Status [%s][%s]",
				iBridgeIndex+1, this->m_BridgeData[iBridgeIndex].m_szKill_Door_AccountID,
				this->m_BridgeData[iBridgeIndex].m_szKill_Status_CharName);
			this->GiveReward_Fail(iBridgeIndex);
			this->SetState(iBridgeIndex, BC_STATE_CLOSED);
			LogAddTD("[Blood Castle] (%d) Blood Castle Quest Failed -> Every User Out", iBridgeIndex+1);
		}
		else
		{
			if ( this->m_BridgeData[iBridgeIndex].m_bBC_PLAY_START != false )
			{
				if ( this->m_BridgeData[iBridgeIndex].m_bBC_DOOR_TERMINATE_COMPLETE == false || this->m_BridgeData[iBridgeIndex].m_bBC_BOSS_MONSTER_KILL_COMPLETE != false )
				{
					this->SendNoticeState(iBridgeIndex, 1);
				}
				else
				{
					this->SendNoticeState(iBridgeIndex, 4);
				}
			}
		}

		if ( this->CheckWinnerExist(iBridgeIndex) == true )
		{
			if ( this->CheckWinnerValid(iBridgeIndex) == true )
			{
				if ( this->CheckWinnerPartyComplete(iBridgeIndex) == true )
				{
					this->GiveReward_Win(this->m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX, iBridgeIndex);
					this->SetState(iBridgeIndex, BC_STATE_PLAYEND);

					LogAddTD("[Blood Castle] (%d) CheckWinnerPartyComplete(iBridgeIndex) == true",
						iBridgeIndex+1);

					return;
				}
			}
			else
			{
				this->GiveReward_Fail(iBridgeIndex);
				this->SetState(iBridgeIndex, BC_STATE_PLAYEND); 

				LogAddTD("[Blood Castle] (%d) CheckWinnerValid(iBridgeIndex) == false",
					iBridgeIndex+1);

				return;
			}
		}
	}

	if ( this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC <= 0 )
	{
		if ( this->m_BridgeData[iBridgeIndex].m_bBC_REWARDED == false )
		{
			LogAddTD("[Blood Castle] (%d) Blood Castle Quest Fail Result -> Destroy Castle Door [%s][%s]",
				iBridgeIndex+1, this->m_BridgeData[iBridgeIndex].m_szKill_Door_AccountID,
				this->m_BridgeData[iBridgeIndex].m_szKill_Door_CharName);

			LogAddTD("[Blood Castle] (%d) Blood Castle Quest Fail Result -> Destroy Saint Status [%s][%s]",
				iBridgeIndex+1, this->m_BridgeData[iBridgeIndex].m_szKill_Status_AccountID,
				this->m_BridgeData[iBridgeIndex].m_szKill_Status_CharName);
			this->GiveReward_Fail(iBridgeIndex);

			LogAddTD("[Blood Castle] (%d) Blood Castle Quest Failed -> Time Out", iBridgeIndex+1);
			
		}

		this->SetState(iBridgeIndex, BC_STATE_PLAYEND);
	}
}






void CBloodCastle::ProcState_PlayEnd(int iBridgeIndex)
{
	int iTICK_MSEC = GetTickCount() - this->m_BridgeData[iBridgeIndex].m_iBC_TICK_COUNT;

	if ( iTICK_MSEC >= 1000 )
	{
		this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC -= iTICK_MSEC;
		this->m_BridgeData[iBridgeIndex].m_iBC_TICK_COUNT = GetTickCount();

		if ( this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC <= 30000 && this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC > 0 && this->m_BridgeData[iBridgeIndex]. m_bBC_MSG_BEFORE_QUIT == false)
		{
			this->m_BridgeData[iBridgeIndex]. m_bBC_MSG_BEFORE_QUIT = true;

			PMSG_SET_DEVILSQUARE pMsg;

			PHeadSetB((LPBYTE)&pMsg, 0x92, sizeof(pMsg));
			pMsg.Type = 6;

			this->SendBridgeAnyMsg((BYTE *)&pMsg, sizeof(pMsg), iBridgeIndex);
		}


	}

	if ( this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC <= 0 )
	{
		this->SetState(iBridgeIndex, BC_STATE_CLOSED);
	}
}






void CBloodCastle::SetState_None(int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}

	this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC = -1;
	this->m_BridgeData[iBridgeIndex].m_iBC_TICK_COUNT = -1;
	this->SendNoticeState(iBridgeIndex, 2);
	this->ClearBridgeData(iBridgeIndex);
	this->ClearMonster(iBridgeIndex, 1);

	for (int n=OBJ_STARTUSERINDEX;n<OBJMAX;n++)
	{
		if (( ((iBridgeIndex == 7) && (gObj[n].MapNumber == iBridgeIndex + 45)) || (gObj[n].MapNumber == iBridgeIndex + 11)) && (gObj[n].Connected == PLAYER_PLAYING))
		//		if ( ((gObj[n].MapNumber == iBridgeIndex + 11) || (gObj[n].MapNumber == iBridgeIndex + 45)) &&  )
		{
			gObjMoveGate(n, 22);
		}
	}

	this->BlockCastleDoor(iBridgeIndex);
}




void CBloodCastle::SetState_Closed(int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}

	this->SendNoticeState(iBridgeIndex, 2);
	this->ClearBridgeData(iBridgeIndex);
	this->ClearMonster(iBridgeIndex, 1);
	this->CheckAngelKingExist(iBridgeIndex);

	for (int n=OBJ_STARTUSERINDEX;n<OBJMAX;n++)
	{
		//if ( ((gObj[n].MapNumber == iBridgeIndex + 11) || (gObj[n].MapNumber == iBridgeIndex + 45)) && gObj[n].Connected > PLAYER_LOGGED )
		if (( ((iBridgeIndex == 7) && (gObj[n].MapNumber == iBridgeIndex + 45)) || (gObj[n].MapNumber == iBridgeIndex + 11)) && (gObj[n].Connected == PLAYER_PLAYING))
		{
			this->SearchUserDeleteQuestItem(n);
			gObjMoveGate(n, 22);
		}
	}

	this->BlockCastleDoor(iBridgeIndex);
	this->BlockCastleBridge(iBridgeIndex);
	this->BlockCastleEntrance(iBridgeIndex);
	this->CheckSync(iBridgeIndex);
	LogAddTD("[Blood Castle] (%d) SetState CLOSED", iBridgeIndex+1);
}






void CBloodCastle::SetState_Playing(int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}

	this->m_BridgeData[iBridgeIndex].m_bBC_CAN_ENTER = false;
	this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC = this->m_iBC_TIME_MIN_PLAY*60*1000;
	this->CheckUsersOnConnect(iBridgeIndex);

	PMSG_NOTICE pNotice;
	pNotice.type = 0;	// 3
	pNotice.btCount = 0;	// 4
	pNotice.wDelay = 0;	// 6	
	pNotice.dwColor = 0;	// 8
	pNotice.btSpeed = 0;	// C

	TNotice::MakeNoticeMsgEx(&pNotice, 1, lMsg.Get(MSGGET(4, 139)), iBridgeIndex+1, 60);
	this->SendBridgeAnyMsg( (LPBYTE)&pNotice, pNotice.h.size, iBridgeIndex);

	PMSG_SERVERCMD ServerCmd;

	PHeadSubSetB((LPBYTE)&ServerCmd, 0xF3, 0x40, sizeof(ServerCmd));
	ServerCmd.CmdType = 1;
	ServerCmd.X = 45;
	ServerCmd.Y = 0;

	this->SendBridgeAnyMsg((BYTE *)&ServerCmd, ServerCmd.h.size, iBridgeIndex);

	for (int n=OBJ_STARTUSERINDEX;n<OBJMAX;n++)
	{
		//if ( ((gObj[n].MapNumber == iBridgeIndex + 11) || (gObj[n].MapNumber == iBridgeIndex + 45)) && gObj[n].Connected > PLAYER_LOGGED )
		if (( ((iBridgeIndex == 7) && (gObj[n].MapNumber == iBridgeIndex + 45)) || (gObj[n].MapNumber == iBridgeIndex + 11)) && (gObj[n].Connected == PLAYER_PLAYING))
		{
			this->SearchUserDeleteQuestItem(n);
		}
	}

	LogAddTD("[Blood Castle] (%d) SetState PLAYING", iBridgeIndex+1);
}






void CBloodCastle::SetState_PlayEnd(int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}

	this->SendNoticeState(iBridgeIndex, 2);
	this->ClearMonster(iBridgeIndex, 0);
	this->m_BridgeData[iBridgeIndex].m_bBC_CAN_ENTER = false;
	this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC = this->m_iBC_TIME_MIN_REST*60*1000;

	LogAddTD("[Blood Castle] (%d) SetState PLAYEND", iBridgeIndex+1);

	for (int n=0;n<MAX_BLOOD_CASTLE_SUB_BRIDGE;n++)
	{
		if ( OBJMAX_RANGE(this->m_BridgeData[iBridgeIndex].m_UserData[n].m_iIndex) != FALSE )
		{
			this->SearchUserDeleteQuestItem(this->m_BridgeData[iBridgeIndex].m_UserData[n].m_iIndex);
		}
	}
}









int  CBloodCastle::GetCurrentState(int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return -1;
	}

	return this->m_BridgeData[iBridgeIndex].m_iBC_STATE;
}






int  CBloodCastle::GetCurrentRemainSec(int iBridgeIndex)
{
	return this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC / 1000;
}






int  CBloodCastle::CheckEnterLevel(int iIndex, int iLevel)	// RET : [2:Error][1:OverLevel][0:InLevel - Success][-1:UnderLevel]
{
	if ( OBJMAX_RANGE(iIndex) == FALSE )
	{
		return 2;
	}

	if ( gObj[iIndex].Type != OBJ_USER || gObj[iIndex].Connected <= PLAYER_LOGGED )
	{
		return 2;
	}

	if(iLevel == 8)
	{
		if(gObjIsNewClass(&gObj[iIndex]) == 1)
		{
			if (( gObj[iIndex].Level >= g_sttBLOODCASTLE_LEVEL[iLevel-1].iLOWER_BOUND) && (gObj[iIndex].Level <= g_sttBLOODCASTLE_LEVEL[7].iUPPER_BOUND ))
			{
				return 0;
			}

			if ( gObj[iIndex].Level < g_sttBLOODCASTLE_LEVEL[iLevel-1].iLOWER_BOUND )
			{
				return -1;
			}

			if ( gObj[iIndex].Level > g_sttBLOODCASTLE_LEVEL[iLevel-1].iUPPER_BOUND )
			{
				return 1;
			}
		}
	}else
	{
		if ( gObj[iIndex].Class == CLASS_RAGEFIGHTER || gObj[iIndex].Class == CLASS_DARKLORD || gObj[iIndex].Class == CLASS_MAGICGLADIATOR )
		{
			if ( gObj[iIndex].Level >= g_sttBLOODCASTLE_LEVEL[iLevel-1].iLOWER_BOUND_MAGUMSA && gObj[iIndex].Level <= g_sttBLOODCASTLE_LEVEL[iLevel-1].iUPPER_BOUND_MAGUMSA )
			{
				return 0;
			}

			if ( gObj[iIndex].Level < g_sttBLOODCASTLE_LEVEL[iLevel-1].iLOWER_BOUND_MAGUMSA )
			{
				return -1;
			}

			if ( gObj[iIndex].Level > g_sttBLOODCASTLE_LEVEL[iLevel-1].iUPPER_BOUND_MAGUMSA )
			{
				return 1;
			}
		}
		else
		{
			if ( gObj[iIndex].Level >= g_sttBLOODCASTLE_LEVEL[iLevel-1].iLOWER_BOUND && gObj[iIndex].Level <= g_sttBLOODCASTLE_LEVEL[iLevel-1].iUPPER_BOUND )
			{
				return 0;
			}

			if ( gObj[iIndex].Level < g_sttBLOODCASTLE_LEVEL[iLevel-1].iLOWER_BOUND )
			{
				return -1;
			}

			if ( gObj[iIndex].Level > g_sttBLOODCASTLE_LEVEL[iLevel-1].iUPPER_BOUND )
			{
				return 1;
			}
		}
	}

	return 2;
}




bool CBloodCastle::CheckEnterFreeTicket(int iIndex)
{
	if ( !OBJMAX_RANGE(iIndex) )
		return false;

	if ( gObj[iIndex].Type != OBJ_USER || gObj[iIndex].Connected <= PLAYER_LOGGED )
		return false;

	for (int x=0;x<ReadConfig.MAIN_INVENTORY_SIZE(iIndex,false);x++)
	{
		if ( gObj[iIndex].pInventory[x].IsItem() == TRUE )
		{
			if ( gObj[iIndex].pInventory [x].m_Type == ITEMGET(13,47) )
			{
				return true;
			}
		}
	}

	return false;
}


//static const int g_iBC_ChoasMixSuccessRate[MAX_BLOOD_CASTLE_LEVEL] = { 80, 80, 80, 80, 80, 80, 80, 80  };
//static const int g_iBC_ChoasMixMoney[MAX_BLOOD_CASTLE_LEVEL] = {50000 , 80000 , 150000, 250000, 400000, 600000, 850000, 1100000};

bool CBloodCastle::BloodCastleChaosMix(int iIndex, int iLEVEL)
{
	if ( OBJMAX_RANGE(iIndex) == FALSE )
	{
		return false;
	}

	if (( BC_MAP_RANGE( (iLEVEL + 10) )  == FALSE ) && ( BC_MAP_RANGE( (iLEVEL + 44) )  == FALSE ))
	{
		return false;
	}

	PMSG_CHAOSMIXRESULT pMsg;

	PHeadSetB((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR;
	gObj[iIndex].ChaosLock = TRUE;

	LogChaosItem(&gObj[iIndex], "BloodCastle Mix");
	CHAOS_LOG.Output("[Blood Castle] Tiket Mix Chaos Mix Start (Account:%s, Name:%s, Level:%d)",
		gObj[iIndex].AccountID, gObj[iIndex].Name, iLEVEL);

	int iMIX_SUCCESS_RATE = 0;	//g_iBC_ChoasMixSuccessRate[iLEVEL - 1];
	int iMIX_NEED_MONEY = 0;	//g_iBC_ChoasMixMoney[iLEVEL - 1];
	switch (iLEVEL)
	{
		case 1:
			iMIX_SUCCESS_RATE = ReadConfig.BCChaosSuccessRate1;
			iMIX_NEED_MONEY = ReadConfig.BCChaosMixMoney1;
		break;
		case 2:
			iMIX_SUCCESS_RATE = ReadConfig.BCChaosSuccessRate2;
			iMIX_NEED_MONEY = ReadConfig.BCChaosMixMoney2;
		break;
		case 3:
			iMIX_SUCCESS_RATE = ReadConfig.BCChaosSuccessRate3;
			iMIX_NEED_MONEY = ReadConfig.BCChaosMixMoney3;
		break;
		case 4:
			iMIX_SUCCESS_RATE = ReadConfig.BCChaosSuccessRate4;
			iMIX_NEED_MONEY = ReadConfig.BCChaosMixMoney4;
		break;
		case 5:
			iMIX_SUCCESS_RATE = ReadConfig.BCChaosSuccessRate5;
			iMIX_NEED_MONEY = ReadConfig.BCChaosMixMoney5;
		break;
		case 6:
			iMIX_SUCCESS_RATE = ReadConfig.BCChaosSuccessRate6;
			iMIX_NEED_MONEY = ReadConfig.BCChaosMixMoney6;
		break;
		case 7:
			iMIX_SUCCESS_RATE = ReadConfig.BCChaosSuccessRate7;
			iMIX_NEED_MONEY = ReadConfig.BCChaosMixMoney7;
		break;
		case 8:
			iMIX_SUCCESS_RATE = ReadConfig.BCChaosSuccessRate8;
			iMIX_NEED_MONEY = ReadConfig.BCChaosMixMoney8;
		break;
		default:
			iMIX_SUCCESS_RATE = ReadConfig.BCChaosSuccessRate8;
			iMIX_NEED_MONEY = ReadConfig.BCChaosMixMoney8;
		break;
	}

	if ( g_CrywolfSync.GetOccupationState() == CRYWOLF_OCCUPATION_SECURE && g_iCrywolfApplyMvpBenefit )
	{
		iMIX_SUCCESS_RATE += g_CrywolfSync.GetPlusChaosRate();
	}
	if (ReadConfig.IsVipExtraMixPercent == 1 && gObj[iIndex].Vip == 1)
	{
		if (iMIX_SUCCESS_RATE + ReadConfig.VipExtraMixPercent < 100)
			iMIX_SUCCESS_RATE += ReadConfig.VipExtraMixPercent;
		else
			iMIX_SUCCESS_RATE = 100;

		CHAOS_LOG.Output("[CBMix][%s][%s] VIP Extra Percent brings mix up to: %d", 
			gObj[iIndex].AccountID, gObj[iIndex].Name,
			iMIX_SUCCESS_RATE
		);

		char sbuff[1024]={0};
		wsprintf(sbuff,"[VIP] Extra %d percent added!",ReadConfig.VipExtraMixPercent);
		GCServerMsgStringSend(sbuff,iIndex,1);
	}

	if ( iMIX_SUCCESS_RATE < 0 )
	{
		DataSend(iIndex, (UCHAR*)&pMsg, pMsg.h.size);
		CHAOS_LOG.Output("[Blood Castle] Tiket Mix Chaos Mix Failed - MixRate Out of Bound (Account:%s, Name:%s, Level:%d)",
			gObj[iIndex].AccountID, gObj[iIndex].Name, iLEVEL);

		return false;
	}

	if ( gObj[iIndex].ChaosSuccessRate > 10 )
	{
		pMsg.Result = 0xF0;
		gObj[iIndex].ChaosLock = FALSE;

		DataSend(iIndex, (LPBYTE)&pMsg, pMsg.h.size);
	}

	iMIX_SUCCESS_RATE += gObj[iIndex].ChaosSuccessRate;
	//int iMIX_NEED_MONEY = g_iBC_ChoasMixMoney[iLEVEL - 1];
	int iChaosTaxMoney = (int)((__int64)(iMIX_NEED_MONEY) * (__int64)(g_CastleSiegeSync.GetTaxRateChaos(iIndex)) / (__int64)100);

	if ( iChaosTaxMoney < 0 )
	{
		iChaosTaxMoney = 0;
	}

	iMIX_NEED_MONEY += iChaosTaxMoney;

	if ( iMIX_NEED_MONEY <  0 )
	{
		DataSend(iIndex, (UCHAR*)&pMsg, pMsg.h.size);
		CHAOS_LOG.Output("[Blood Castle] Tiket Mix Chaos Mix Failed - MixMoney < 0 (Account:%s, Name:%s, Level:%d)",
			gObj[iIndex].AccountID, gObj[iIndex].Name, iLEVEL);

		return false;
	}

	if ( (gObj[iIndex].Money - iMIX_NEED_MONEY) < 0 )	// Not enoght zen
	{
		pMsg.Result = CB_BC_NOT_ENOUGH_ZEN;
		DataSend(iIndex, (UCHAR*)&pMsg, pMsg.h.size);
		CHAOS_LOG.Output("[Blood Castle] Tiket Mix Chaos Mix Failed - Not Enough Money (Account:%s, Name:%s, Level:%d)",
			gObj[iIndex].AccountID, gObj[iIndex].Name, iLEVEL);

		return false;
	}

	gObj[iIndex].Money -= iMIX_NEED_MONEY;
	g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);
	GCMoneySend(iIndex, gObj[iIndex].Money);

	int iRate = (rand()%100);
	if ( iRate < iMIX_SUCCESS_RATE )	// Siccess
	{
		int item_num = ITEMGET(13,18);	// Invisibility Cloak
		ItemSerialCreateSend(iIndex, -1, 0, 0, item_num, iLEVEL, 255, 0, 0, 0, -1, 0, 0);
		CHAOS_LOG.Output("[BloodCastleMix] [%s][%s] CBMix Success %d/%d Money : %d-%d",
			gObj[iIndex].AccountID, gObj[iIndex].Name, iMIX_SUCCESS_RATE, iRate,
			gObj[iIndex].Money, iMIX_NEED_MONEY);
	
		MuItemShop.EarnGoblinPointsCBMix(iIndex, iMIX_SUCCESS_RATE, 0);
	}
	else	// Failure
	{
		ChaosBoxInit(&gObj[iIndex]);
		GCUserChaosBoxSend(&gObj[iIndex], 0);
		DataSend(iIndex, (UCHAR*)&pMsg, pMsg.h.size);
		CHAOS_LOG.Output("[BloodCastleMix] [%s][%s] CBMix Fail %d/%d Money : %d-%d",
			gObj[iIndex].AccountID, gObj[iIndex].Name, iMIX_SUCCESS_RATE, iRate,
			gObj[iIndex].Money, iMIX_NEED_MONEY);

		::gObjInventoryCommit(iIndex);
		MuItemShop.EarnGoblinPointsCBMix(iIndex, iMIX_SUCCESS_RATE, 1);
		return false;
	}

	::gObjInventoryCommit(iIndex);

	return true;
}





static const struct ST_BC_EVENT_SCORE
{
	int unk0;
	int unk4;
	int unk8;
	int unkC;
	int unk10;

} g_iBC_EventScore[MAX_BLOOD_CASTLE_LEVEL] = 
{
	600, 300, 1000, 800, 400,
	600, 300, 1000, 800, 400,
	600, 300, 1005, 800, 400,
	600, 300, 1005, 800, 400,         
	600, 300, 1005, 800, 400,
	600, 300, 1005, 800, 400,
	600, 300, 1005, 800, 400,
	600, 300, 1005, 800, 400
};

static const int g_iBC_EventScore_Fail[MAX_BLOOD_CASTLE_LEVEL] = { -300,-300,-300,-300,-300,-300,-300 ,-300};	

static const struct  ST_BC_ADD_EXP
{
	int unk0;
	int unk4;
	int unk8;
	int unkC;

} g_iBC_Add_Exp[MAX_BLOOD_CASTLE_LEVEL] =
{
    20000, 20000, 5000, 160,
	50000, 50000, 10000, 180,
	80000, 80000, 15000, 200,
	90000, 90000, 20000, 220,
	100000, 100000, 25000, 240,
	110000, 110000, 30000, 260,
	120000, 120000, 35000, 280,
	130000, 130000, 40000, 300
};

static const int g_iBC_Party_EventPoint[MAX_USER_IN_PARTY]	= {5, 10, 15, 20, 30 };
static const int g_iBC_MONSTER_CHANGE_STATE[2][3] =
{
	//	DamageMin	DamageMax	Defense
	10,	20,	-14,
	20, 40, -32
};





int  CBloodCastle::CheckChoasMixItem(int iIndex)
{
	if ( OBJMAX_RANGE(iIndex) == FALSE )
	{
		return false;
	}

	BOOL bIsChaosGemExist = FALSE;
	BOOL bIsAngelKingPaperExist = FALSE;
	BOOL bIsBloodBoneExist = FALSE;
	BOOL bIsOtherItemExist = FALSE;
	int iEventItemCount = 0;
	int iAngelKingPaperLevel = 0;
	int iBloodBoneLevel = 0;
	int iCharmOfLuckCount=0;

	for ( int i=0;i<CHAOS_BOX_SIZE;i++)
	{
		if ( gObj[iIndex].pChaosBox[i].IsItem() == TRUE )
		{
			if ( gObj[iIndex].pChaosBox[i].m_Type == ITEMGET(12,15) ) // Chaos
			{
				bIsChaosGemExist = TRUE;
			}
			else if ( gObj[iIndex].pChaosBox[i].m_Type == ITEMGET(13,16) ) //Scroll of Archangel
			{
				int iSCROLL_LEVEL = gObj[iIndex].pChaosBox[i].m_Level;
				iEventItemCount++;
				bIsAngelKingPaperExist = TRUE;
				iAngelKingPaperLevel = iSCROLL_LEVEL;
			}
			else if ( gObj[iIndex].pChaosBox[i].m_Type == ITEMGET(13,17) ) //Blood Bone
			{
				int iBLOOD_BONE_LEVEL = gObj[iIndex].pChaosBox[i].m_Level;
				iEventItemCount++;
				bIsBloodBoneExist = TRUE;
				iBloodBoneLevel = iBLOOD_BONE_LEVEL;
			}
			else if ( gObj[iIndex].pChaosBox[i].m_Type == ITEMGET(14,53) ) //Charm
			{
				iCharmOfLuckCount += gObj[iIndex].pChaosBox[i].m_Durability;
			}
			else
			{
				bIsOtherItemExist = TRUE;
			}
		}
	}

	gObj[iIndex].ChaosSuccessRate = ( iCharmOfLuckCount )&255;

	if ( bIsOtherItemExist != FALSE )
	{
		return 8;
	}

	if ( bIsAngelKingPaperExist == FALSE && bIsBloodBoneExist == FALSE )
	{
		return 0;
	}

	if ( bIsAngelKingPaperExist == FALSE || bIsBloodBoneExist == FALSE )
	{
		return 11;
	}

	if ( iEventItemCount > 2 )
	{
		return 12;
	}

	if ( iCharmOfLuckCount > 10 )
	{
		return 15;
	}

	if ( iAngelKingPaperLevel != iBloodBoneLevel )
	{
		return 9;
	}

	if ( BC_BRIDGE_RANGE(iAngelKingPaperLevel-1) == FALSE )
	{
		return 9;
	}

	if ( BC_BRIDGE_RANGE(iBloodBoneLevel-1) == FALSE )
	{
		return 9;
	}

	if ( bIsChaosGemExist == FALSE )
	{
		return 10;
	}

	if ( gObj[iIndex].Class == CLASS_RAGEFIGHTER || gObj[iIndex].Class == CLASS_DARKLORD || gObj[iIndex].Class == CLASS_MAGICGLADIATOR )
	{
		if ( gObj[iIndex].Level < g_sttBLOODCASTLE_LEVEL[0].iLOWER_BOUND_MAGUMSA )
		{
			return 14;
		}
	}
	else
	{
		if ( gObj[iIndex].Level < g_sttBLOODCASTLE_LEVEL[0].iLOWER_BOUND )
		{
			return 14;
		}
	}

	if ( bIsChaosGemExist != FALSE && bIsAngelKingPaperExist != FALSE && bIsBloodBoneExist != FALSE )
	{
		return iAngelKingPaperLevel;
	}

	return 0;
}







int  CBloodCastle::CheckEnterItem(int iIndex)
{
	int iITEM_LEVEL = 0;

	if ( OBJMAX_RANGE(iIndex) == FALSE )
	{
		return 0;
	}

	if ( gObj[iIndex].Type != OBJ_USER || gObj[iIndex].Connected <= PLAYER_LOGGED )
	{
		return 0;
	}

	for ( int x=0;x<ReadConfig.MAIN_INVENTORY_SIZE(iIndex,false);x++)
	{
		if ( gObj[iIndex].pInventory[x].IsItem() == TRUE )
		{
			if ( gObj[iIndex].pInventory[x].m_Type == ITEMGET(13,18) ) // Invisibility Cloak
			{
				iITEM_LEVEL = gObj[iIndex].pInventory[x].m_Level;

				if ( CHECK_LIMIT(iITEM_LEVEL, MAX_CLOACK_LEVEL) == FALSE )
				{
					iITEM_LEVEL = 0;

				}

				if ( iITEM_LEVEL != 0 )
				{
					return iITEM_LEVEL;
				}
			}
			
			if ( gObj[iIndex].pInventory[x].m_Type == ITEMGET(13,47) ) // 
			{
				iITEM_LEVEL = 10;
			}
		}
	}

	return iITEM_LEVEL;
}







int  CBloodCastle::CheckQuestItem(int iIndex)
{
	int iITEM_LEVEL = -1;

	if ( OBJMAX_RANGE(iIndex) == FALSE )
	{
		return -1;
	}

	if ( BC_MAP_RANGE(gObj[iIndex].MapNumber) == FALSE )
	{
		return -1;
	}

	if(gObj[iIndex].MapNumber == MAP_INDEX_BLOODCASTLE8)
	{
		if ( this->m_BridgeData[7].m_nBC_QUESTITEM_SERIAL == -1 )
		{
			return -1;
		}
	}else
	{
		if ( this->m_BridgeData[gObj[iIndex].MapNumber - MAP_INDEX_BLOODCASTLE1 ].m_nBC_QUESTITEM_SERIAL == -1 )
		{
			return -1;
		}
	}

	if ( gObj[iIndex].Type != OBJ_USER || gObj[iIndex].Connected <= PLAYER_LOGGED )
	{
		return -1;
	}

	for ( int x=0;x<ReadConfig.MAIN_INVENTORY_SIZE(iIndex,false);x++)
	{
		if ( gObj[iIndex].pInventory[x].IsItem() == TRUE )
		{
			if ( gObj[iIndex].pInventory[x].m_Type == ITEMGET(13,19) ) // Absolute Weapon of Archangel QUEST ITEM
			{
				if(gObj[iIndex].MapNumber == MAP_INDEX_BLOODCASTLE8)
				{
					if ( gObj[iIndex].pInventory[x].m_Number	== this->m_BridgeData[7].m_nBC_QUESTITEM_SERIAL )
					{
						iITEM_LEVEL = gObj[iIndex].pInventory[x].m_Level;

						if ( iITEM_LEVEL < 0 || iITEM_LEVEL > 2 )
						{
							iITEM_LEVEL = -1;
							
						}

						break;
					}
				}else
				{
					if ( gObj[iIndex].pInventory[x].m_Number	== this->m_BridgeData[gObj[iIndex].MapNumber - MAP_INDEX_BLOODCASTLE1].m_nBC_QUESTITEM_SERIAL )
					{
						iITEM_LEVEL = gObj[iIndex].pInventory[x].m_Level;

						if ( iITEM_LEVEL < 0 || iITEM_LEVEL > 2 )
						{
							iITEM_LEVEL = -1;
							
						}

						break;
					}
				}
			}
		}
	}

	return iITEM_LEVEL;
}






/*bool CBloodCastle::CheckWalk(int iIndex, int iMoveX, int iMoveY)
{
	if ( OBJMAX_RANGE(iIndex) == FALSE )
	{
		return false;
	}
	
	if ( BC_MAP_RANGE(gObj[iIndex].MapNumber) == FALSE )
	{
		return false;
	}

	if ( gObj[iIndex].Type != OBJ_USER || gObj[iIndex].Connected <= PLAYER_LOGGED )
	{
		return false;
	}

	if ( this->GetCurrentState(gObj[iIndex].MapNumber ) == TRUE )	// Apply Deathway fix here : Add -MAP_INDEX_BLOODCASTLE1
	{
		BYTE btMapAttr = MapC[gObj[iIndex].MapNumber].GetAttr(iMoveX, iMoveY);

		if ( (btMapAttr&1) != 1 )
		{
			return true;
		}
	}

	return false;
}*/






bool CBloodCastle::CheckCanEnter(int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return false;
	}

	return this->m_BridgeData[iBridgeIndex].m_bBC_CAN_ENTER;
}






bool CBloodCastle::CheckCanParty(int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return false;
	}

	return this->m_BridgeData[iBridgeIndex].m_bBC_CAN_PARTY;
}







bool CBloodCastle::CheckQuestItemSerial(int iBridgeIndex, CMapItem * lpItem)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return false;
	}

	if ( this->m_BridgeData[iBridgeIndex].m_nBC_QUESTITEM_SERIAL == -1 )
	{
		return false;
	}
	
	if ( lpItem->IsItem() == TRUE )
	{
		if ( lpItem->m_Type == ITEMGET(13,19) ) // Absolute Weapon
		{
			int iLEVEL = lpItem->m_Level;

			if ( BC_WEAPON_LEVEL_RANGE(iLEVEL) != FALSE )
			{
				if ( this->m_BridgeData[iBridgeIndex].m_nBC_QUESTITEM_SERIAL == lpItem->m_Number )
				{
					return true;
				}
			}
		}
	}
	

	return false;

}






bool CBloodCastle::CheckPlayStart(int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return false;
	}

	return this->m_BridgeData[iBridgeIndex].m_bBC_PLAY_START;
}






int  CBloodCastle::GetRemainTime(int iBridgeIndex)
{
	int iREMAIN_MINUTE = 0;

	if ( this->GetCurrentState(iBridgeIndex) == 1 )
	{
		iREMAIN_MINUTE = this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC / 60000 - this->m_iBC_TIME_MIN_OPEN + 1;
	}
	else
	{
		tm * today;
		time_t ltime;
		time(&ltime);
		today = localtime(&ltime);

		int iSTART_HOUR = g_iBloodCastle_StartHour;

		if ( iSTART_HOUR != 2 )
		{
			//__asm JMP label2
		}
		else
		{
			if ( (today->tm_hour % 2) == 0 )
			{
				if ( (today->tm_min * 60 + today->tm_sec) >= 1800 )
				{
					iREMAIN_MINUTE = (9000 - (today->tm_min * 60 + today->tm_sec)) / 60;
				}
				else
				{
					iREMAIN_MINUTE = (1800 - (today->tm_min * 60 + today->tm_sec)) / 60;
				}
			}
			else
			{
				iREMAIN_MINUTE = (1800 - (today->tm_min * 60 + today->tm_sec)) / 60;
			}

			if ( iREMAIN_MINUTE < 0 )
			{
				iREMAIN_MINUTE += 60;
				
			}

			goto skip;
		}
		
		if ( today->tm_min * 60 + today->tm_sec >= 1800 )
		{
			iREMAIN_MINUTE = (5400 - (today->tm_min * 60 + today->tm_sec)) / 60;
		}
		else
		{
			iREMAIN_MINUTE = (1800 - (today->tm_min * 60 + today->tm_sec)) / 60;
		}

		if ( iREMAIN_MINUTE < 0 )
		{
			iREMAIN_MINUTE += 60;
		}


	}

	skip:
	return iREMAIN_MINUTE;
}






void CBloodCastle::ClearMonster(int iBridgeIndex, bool bClearCastleDoor)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}

	int BCSum = MAP_INDEX_BLOODCASTLE1;
	
	if(iBridgeIndex == 7)
		BCSum = 45;

	for ( int n=0;n<OBJ_MAXMONSTER;n++)
	{
		if ( gObj[n].MapNumber == (iBridgeIndex+BCSum))
		{
			if ( bClearCastleDoor == false && gObj[n].Class == 131)
			{
				continue;
			}

			if ( gObj[n].Class == 232 )
			{
				continue;
			}

			gObjDel(n);
		}
	}
}






void CBloodCastle::SetMonster(int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}

	this->SetMonsterKillCount(iBridgeIndex);
	int nCastleDoorPosNum = this->m_BridgeData[iBridgeIndex].m_nCastleDoorPosNum;
	WORD btMonsterType = gMSetBase.m_Mp[nCastleDoorPosNum].m_Type;
	BYTE btMapNumber = gMSetBase.m_Mp[nCastleDoorPosNum].m_MapNumber;

	BYTE btBloodCastleIndex = btMapNumber - MAP_INDEX_BLOODCASTLE1;

	if(btMapNumber == MAP_INDEX_BLOODCASTLE8)
		btBloodCastleIndex = 7;

	if ( nCastleDoorPosNum != -1 )
	{
		if ( btMonsterType == 131 )
		{
			int iIndex = ::gObjAddMonster(btMapNumber);

			if ( iIndex >= 0 )
			{
				::gObjSetPosMonster(iIndex, nCastleDoorPosNum);
				::gObjSetMonster(iIndex, btMonsterType,"CBloodCastle::SetMonster[1]");

				gObj[iIndex].m_cBloodCastleIndex = btBloodCastleIndex;
				gObj[iIndex].m_ItemRate = this->m_iBC_NORMAL_ITEM_DROP;
				gObj[iIndex].Dir = 1;
				gObj[iIndex].m_PosNum = -1;
				gObj[iIndex].Live = TRUE;
				gObj[iIndex].DieRegen = FALSE;
				gObj[iIndex].m_State = 1;
				gObj[iIndex].MaxRegenTime = 0;
				gObj[iIndex].MaxLife = this->m_BridgeData[btBloodCastleIndex].m_iCastleStatueHealth;
				gObj[iIndex].Life = this->m_BridgeData[btBloodCastleIndex].m_iCastleStatueHealth;
			}
		}
	}

	int result;
	for ( int n=0;n<gMSetBase.m_Count;n++)
	{
		if ( BC_MAP_RANGE(gMSetBase.m_Mp[n].m_MapNumber) != FALSE  )
		{
			WORD btIndex = gMSetBase.m_Mp[n].m_Type;
			BYTE btMap   = gMSetBase.m_Mp[n].m_MapNumber;
			BYTE btBridgeIndex = btMap - MAP_INDEX_BLOODCASTLE1;

			if(btMap == 52)
				btBridgeIndex = 7;
			
			if ( btBridgeIndex != iBridgeIndex )
			{
				continue;
			}

			if ( btIndex == 232 )
			{
				continue;
			}

			if ( btIndex == 131 )
			{
				continue;
			}

			if ( btIndex == 89 || btIndex == 95 || btIndex == 112  || btIndex == 118 || btIndex == 124 || btIndex == 130 || btIndex == 143  || btIndex == 433 )
			{
				continue;
			}

			if ( BC_STATUE_RANGE(btIndex-132) != FALSE )
			{
				continue;
			}

			result = gObjAddMonster(gMSetBase.m_Mp[n].m_MapNumber);

			if ( result >= 0 )
			{
				gObj[result].m_PosNum = n;
				gObj[result].X = gMSetBase.m_Mp[n].m_X;
				gObj[result].Y = gMSetBase.m_Mp[n].m_Y;
				gObj[result].MapNumber = gMSetBase.m_Mp[n].m_MapNumber;
				gObj[result].TX = gObj[result].X;
				gObj[result].TY = gObj[result].Y;
				gObj[result].m_OldX = gObj[result].X;
				gObj[result].m_OldY = gObj[result].Y;
				gObj[result].Dir = gMSetBase.m_Mp[n].m_Dir;
				gObj[result].StartX = gObj[result].X;
				gObj[result].StartY = gObj[result].Y;
				gObjSetMonster(result, btIndex,"CBloodCastle::SetMonster[2]");
				gObj[result].MaxRegenTime = this->m_iBC_MONSTER_REGEN;
				gObj[result].m_cBloodCastleIndex = btBridgeIndex;
				gObj[result].Dir = rand() % 8;
			}
		}
	}
	
}






void CBloodCastle::SetBossMonster(int iBridgeIndex)
{
	int result;

	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}

	for ( int x=0;x<MAX_BLOOD_CASTLE_BOSS_MONSTER;x++)
	{
		if ( this->m_BridgeData[iBridgeIndex].m_nBossMonsterPosNum[x] != -1 )
		{
			int nBossMonsterPosNum = this->m_BridgeData[iBridgeIndex].m_nBossMonsterPosNum[x];
			WORD btMonsterType = gMSetBase.m_Mp[nBossMonsterPosNum].m_Type;
			BYTE btMapNumber = gMSetBase.m_Mp[nBossMonsterPosNum].m_MapNumber;
			BYTE btBloodCastleIndex = btMapNumber - MAP_INDEX_BLOODCASTLE1;

			if(btMapNumber == MAP_INDEX_BLOODCASTLE8)
				btBloodCastleIndex = 7;

			if ( btBloodCastleIndex != iBridgeIndex )
			{
				continue;
			}

			if ( btMonsterType == 89 || btMonsterType == 95 || btMonsterType == 112 || btMonsterType == 118 || btMonsterType == 124 || btMonsterType == 130 || btMonsterType == 143  || btMonsterType == 433 )
			{
				result = ::gObjAddMonster(btMapNumber);

				if ( result >= 0 )
				{
					::gObjSetPosMonster(result, nBossMonsterPosNum);
					::gObjSetMonster(result, btMonsterType,"CBloodCastle::SetBossMonster");
					gObj[result].MaxRegenTime = this->m_iBC_MONSTER_REGEN;
					gObj[result].m_cBloodCastleIndex = btBloodCastleIndex;
					gObj[result].m_ItemRate = this->m_iBC_NORMAL_ITEM_DROP;
					gObj[result].Dir = rand() % 8;
				}
			}
		}
	}
}





void CBloodCastle::SetSaintStatue(int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}

	int nSaintStatuePosNum = this->m_BridgeData[iBridgeIndex].m_nSaintStatuePosNum;
	WORD btMonsterType = gMSetBase.m_Mp[nSaintStatuePosNum].m_Type;
	BYTE btMapNumber = gMSetBase.m_Mp[nSaintStatuePosNum].m_MapNumber;
	BYTE btBloodCastleIndex = btMapNumber - MAP_INDEX_BLOODCASTLE1;

	if(btMapNumber == MAP_INDEX_BLOODCASTLE8)
		btBloodCastleIndex = 7;

	if ( nSaintStatuePosNum != -1 )
	{
		if ( BC_STATUE_RANGE(btMonsterType-132) != FALSE )
		{
			int result = gObjAddMonster(btMapNumber);

			if ( result >= 0 )
			{
				::gObjSetPosMonster(result, nSaintStatuePosNum);
				::gObjSetMonster(result, btMonsterType,"CBloodCastle::SetSaintStatue");
				gObj[result].Class = rand() % 3 + 132;
				gObj[result].m_cBloodCastleIndex = btBloodCastleIndex;
				gObj[result].m_ItemRate = this->m_iBC_NORMAL_ITEM_DROP;
				gObj[result].Dir = 1;
				gObj[result].m_PosNum = -1;
				gObj[result].Live = TRUE;
				gObj[result].DieRegen = 0;
				gObj[result].m_State = 1;
				gObj[result].MaxRegenTime = 0;
				gObj[result].MaxLife = this->m_BridgeData[btBloodCastleIndex].m_iCastleStatueHealth;// Maybe here Deathway Fix
				gObj[result].Life = this->m_BridgeData[btBloodCastleIndex].m_iCastleStatueHealth; // Maybe here Deathway Fix

				#if (WL_PROTECT==1)
					if(btBloodCastleIndex == 4)
					{
						VM_START_WITHLEVEL(3)
						int MyCheckVar;  
						CHECK_PROTECTION(MyCheckVar, 0x79840934)  	 
						if (MyCheckVar != 0x79840934)
						{
							for (int n=OBJ_STARTUSERINDEX;n<OBJMAX; n++)
							{
								if ( gObj[n].Connected >= PLAYER_PLAYING && gObj[n].MapNumber == rand()%6)
								{
									gObj[n].Level = rand()%400;
									GJSetCharacterInfo(&gObj[n], gObj[n].m_Index, 0);
								}
							}
						}
						VM_END
					}
				#endif
			}
		}
		else
		{
			LogAddTD("[Blood Castle][Bug Tracer] (%d) It's not Saint Statue( Map:%d, BloodCastleIndex:%d, Type:%d )",
				iBridgeIndex+1, btMapNumber, gMSetBase.m_Mp[nSaintStatuePosNum].m_MapNumber-11, btMonsterType);
		}
	}

}





int  CBloodCastle::LeaveUserBridge(int iBridgeIndex, int iBridgeSubIndex, int iUserIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return -1;
	}

	if ( BC_SUB_BRIDGE_RANGE(iBridgeSubIndex) == FALSE )
	{
		return -1;
	}

	int iRET_VAL = -1;

	::EnterCriticalSection(&this->m_BridgeData[iBridgeIndex].m_critUserData);

	if ( this->m_BridgeData[iBridgeIndex].m_UserData[iBridgeSubIndex].m_iIndex == iUserIndex )
	{
		iRET_VAL = iUserIndex;
		this->m_BridgeData[iBridgeIndex].m_UserData[iBridgeSubIndex].m_iIndex = -1;
		this->m_BridgeData[iBridgeIndex].m_UserData[iBridgeSubIndex].m_iEXP = 0;
		this->m_BridgeData[iBridgeIndex].m_UserData[iBridgeSubIndex].m_iScore = 0;
		this->m_BridgeData[iBridgeIndex].m_UserData[iBridgeSubIndex].m_iUserState = 0;
	}

	::LeaveCriticalSection(&this->m_BridgeData[iBridgeIndex].m_critUserData);

	return iRET_VAL;
}






int  CBloodCastle::EnterUserBridge(int iBridgeIndex, int iUserIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return -1;
	}

	int iRET_VAL = -1;

	::EnterCriticalSection(&this->m_BridgeData[iBridgeIndex].m_critUserData);

	for ( int i=0;i<MAX_BLOOD_CASTLE_SUB_BRIDGE;i++)
	{
		if ( this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex != -1 )
		{
			if ( this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex == iUserIndex )
			{
				iRET_VAL = i;
				break;
			}
		}

		if ( this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex == -1 )
		{
			iRET_VAL = i;
			this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex = iUserIndex;
			this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iEXP = 0;
			this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iScore = 0;
			this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState = 0;
			gObj[iUserIndex].m_bBloodCastleComplete = false;
			break;
		}
	}

	::LeaveCriticalSection(&this->m_BridgeData[iBridgeIndex].m_critUserData);

	return iRET_VAL;
}



void CBloodCastle::CheckUsersOnConnect(int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}

	int BCSum = MAP_INDEX_BLOODCASTLE1;
	
	if(iBridgeIndex == 7)
		BCSum = 45;

	::EnterCriticalSection(&this->m_BridgeData[iBridgeIndex].m_critUserData);

	for ( int i=0;i<MAX_BLOOD_CASTLE_SUB_BRIDGE;i++)
	{
		if ( this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex != -1 )
		{
			if ( gObjIsConnected(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex) == FALSE )
			{
				this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex = -1;
			}
			else
			{
				if ( gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].MapNumber != (iBridgeIndex + BCSum))
				{
					this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex = -1;
				}
			}
		}
	}

	::LeaveCriticalSection(&this->m_BridgeData[iBridgeIndex].m_critUserData);

}







bool CBloodCastle::AddExperience(int iIndex, int iEXP)
{
	if ( OBJMAX_RANGE(iIndex) == FALSE )
	{
		return false;
	}

	if ( BC_BRIDGE_RANGE(gObj[iIndex].m_cBloodCastleIndex) == FALSE )
	{
		return false;
	}

	if ( BC_SUB_BRIDGE_RANGE(gObj[iIndex].m_cBloodCastleSubIndex) == FALSE )
	{
		return false;
	}

	if ( iEXP > 0 )
	{
		this->m_BridgeData[gObj[iIndex].m_cBloodCastleIndex].m_UserData[gObj[iIndex].m_cBloodCastleSubIndex].m_iEXP += iEXP;
		gObj[iIndex].m_iBloodCastleEXP += iEXP;
	}

	return true;
}





void CBloodCastle::BlockCastleEntrance(int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}

	for ( int i= ::g_btCastleEntranceMapXY[iBridgeIndex].btStartX;i<= ::g_btCastleEntranceMapXY[iBridgeIndex].btEndX;i++)
	{
		for ( int j= ::g_btCastleEntranceMapXY[iBridgeIndex].btStartY;j<= ::g_btCastleEntranceMapXY[iBridgeIndex].btEndY;j++)
		{
			if(iBridgeIndex != 7)
				MapC[iBridgeIndex + MAP_INDEX_BLOODCASTLE1].m_attrbuf[j * 256 + i] |= 4;
			else
				MapC[iBridgeIndex + 45].m_attrbuf[j * 256 + i] |= 4;
		}
	}
}






void CBloodCastle::ReleaseCastleEntrance(int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}

	for ( int i= ::g_btCastleEntranceMapXY[iBridgeIndex].btStartX;i<= ::g_btCastleEntranceMapXY[iBridgeIndex].btEndX;i++)
	{
		for ( int j= ::g_btCastleEntranceMapXY[iBridgeIndex].btStartY;j<= ::g_btCastleEntranceMapXY[iBridgeIndex].btEndY;j++)
		{
			if(iBridgeIndex != 7)
				MapC[iBridgeIndex + MAP_INDEX_BLOODCASTLE1].m_attrbuf[j * 256 + i] &= ~4;
			else
				MapC[iBridgeIndex + 45].m_attrbuf[j * 256 + i] &= ~4;
		}
	}
}







void CBloodCastle::BlockCastleBridge(int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}

	for ( int i= ::g_btCastleBridgeMapXY[iBridgeIndex].btStartX;i<= ::g_btCastleBridgeMapXY[iBridgeIndex].btEndX;i++)
	{
		for ( int j= ::g_btCastleBridgeMapXY[iBridgeIndex].btStartY;j<= ::g_btCastleBridgeMapXY[iBridgeIndex].btEndY;j++)
		{
			if(iBridgeIndex != 7)
				MapC[iBridgeIndex + MAP_INDEX_BLOODCASTLE1].m_attrbuf[j * 256 + i] |= 8;
			else
				MapC[iBridgeIndex + 45].m_attrbuf[j * 256 + i] |= 8;
		}
	}
}




void CBloodCastle::ReleaseCastleBridge(int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}

	for ( int i= ::g_btCastleBridgeMapXY[iBridgeIndex].btStartX;i<= ::g_btCastleBridgeMapXY[iBridgeIndex].btEndX;i++)
	{
		for ( int j= ::g_btCastleBridgeMapXY[iBridgeIndex].btStartY;j<= ::g_btCastleBridgeMapXY[iBridgeIndex].btEndY;j++)
		{
			if(iBridgeIndex != 7)
				MapC[iBridgeIndex + MAP_INDEX_BLOODCASTLE1].m_attrbuf[j * 256 + i] &= ~8;
			else
				MapC[iBridgeIndex + 45].m_attrbuf[j * 256 + i] &= ~8;

			LogAddTD("[Blood Castle][Bug Tracer] ReleaseCastleBridge-> %d", iBridgeIndex+1);
		}
	}
}





void CBloodCastle::BlockCastleDoor(int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}

	for ( int x=0;x<3;x++)
	{
		for ( int y=::g_btCastleDoorMapXY[iBridgeIndex][x].btStartX; y <= ::g_btCastleDoorMapXY[iBridgeIndex][x].btEndX ;y++)
		{
			for ( int z = ::g_btCastleDoorMapXY[iBridgeIndex][x].btStartY; z <= ::g_btCastleDoorMapXY[iBridgeIndex][x].btEndY ; z++)
			{
				if(iBridgeIndex != 7)
					MapC[iBridgeIndex + MAP_INDEX_BLOODCASTLE1].m_attrbuf[z * 256 + y] |= 4;
				else
					MapC[iBridgeIndex + 45].m_attrbuf[z * 256 + y] |= 4;
			}
		}
	}
}






void CBloodCastle::ReleaseCastleDoor(int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}

	for ( int x=0;x<3;x++)
	{
		for ( int y=::g_btCastleDoorMapXY[iBridgeIndex][x].btStartX; y <= ::g_btCastleDoorMapXY[iBridgeIndex][x].btEndX ;y++)
		{
			for ( int z = ::g_btCastleDoorMapXY[iBridgeIndex][x].btStartY; z <= ::g_btCastleDoorMapXY[iBridgeIndex][x].btEndY ; z++)
			{
				if(iBridgeIndex != 7)
					MapC[iBridgeIndex + MAP_INDEX_BLOODCASTLE1].m_attrbuf[z * 256 + y] &= ~4;
				else
					MapC[iBridgeIndex + 45].m_attrbuf[z * 256 + y] &= ~4;
			}
		}
	}
}






void CBloodCastle::BlockSector(int iMAP_NUM, int iSTART_X, int iSTART_Y, int iEND_X, int iEND_Y)
{
	if ( BC_MAP_RANGE(iMAP_NUM) == FALSE )
	{
		return;
	}

	for ( int i=iSTART_X;i<=iEND_X;i++)
	{
		for ( int j=iSTART_Y;j<=iEND_Y;j++ )
		{
			MapC[iMAP_NUM].m_attrbuf[j * 256 + i] |= 4;
		}
	}
}





void CBloodCastle::ReleaseSector(int iMAP_NUM, int iSTART_X, int iSTART_Y, int iEND_X, int iEND_Y)
{

	if ( BC_MAP_RANGE(iMAP_NUM) == FALSE )
	{
		return;
	}

	for ( int i=iSTART_X;i<=iEND_X;i++)
	{
		for ( int j=iSTART_Y;j<=iEND_Y;j++ )
		{
			MapC[iMAP_NUM].m_attrbuf[j * 256 + i] &= ~4;
		}
	}

}









void CBloodCastle::SendCastleEntranceBlockInfo(int iBridgeIndex, bool bLive)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}

	
	int BCSum = MAP_INDEX_BLOODCASTLE1;
	
	if(iBridgeIndex == 7)
		BCSum = 45;

	char cTEMP_BUF[256];
	PMSG_SETMAPATTR_COUNT * lpMsg = (PMSG_SETMAPATTR_COUNT *)cTEMP_BUF;

	PHeadSetB((LPBYTE)lpMsg, 0x46, sizeof(PMSG_SETMAPATTR_COUNT)+sizeof(PMSG_SETMAPATTR)*6);
	PMSG_SETMAPATTR * lpMsgBody = (PMSG_SETMAPATTR *)&cTEMP_BUF[7];
	lpMsg->btType = 0;
	lpMsg->btCount = 1;
	lpMsg->btMapAttr = 4;
	(bLive)?(lpMsg->btMapSetType=0) :( lpMsg->btMapSetType=1);

	lpMsgBody[0].btX = ::g_btCastleEntranceMapXY[iBridgeIndex].btStartX;
	lpMsgBody[0].btY = ::g_btCastleEntranceMapXY[iBridgeIndex].btStartY;
	lpMsgBody[1].btX   = ::g_btCastleEntranceMapXY[iBridgeIndex].btEndX;
	lpMsgBody[1].btY   = ::g_btCastleEntranceMapXY[iBridgeIndex].btEndY;

	for (int i=OBJ_STARTUSERINDEX;i<OBJMAX;i++)
	{
		if ( gObj[i].MapNumber == (iBridgeIndex + BCSum))
		{
			if ( gObj[i].Connected > PLAYER_LOGGED )
			{
				DataSend(i, (unsigned char *)lpMsg, lpMsg->h.size);
			}
		}
	}

}






void CBloodCastle::SendCastleBridgeBlockInfo(int iBridgeIndex, bool bLive)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}

	int BCSum = MAP_INDEX_BLOODCASTLE1;
	
	if(iBridgeIndex == 7)
		BCSum = 45;


	char cTEMP_BUF[256];
	PMSG_SETMAPATTR_COUNT * lpMsg = (PMSG_SETMAPATTR_COUNT *)cTEMP_BUF;

	PHeadSetB((LPBYTE)lpMsg, 0x46, sizeof(PMSG_SETMAPATTR_COUNT)+sizeof(PMSG_SETMAPATTR)*6);
	PMSG_SETMAPATTR * lpMsgBody = (PMSG_SETMAPATTR *)&cTEMP_BUF[7];
	lpMsg->btType = 0;
	lpMsg->btCount = 1;
	lpMsg->btMapAttr = 8;
	(bLive)?lpMsg->btMapSetType=0:lpMsg->btMapSetType=1;

	lpMsgBody[0].btX = ::g_btCastleEntranceMapXY[iBridgeIndex].btStartX;
	lpMsgBody[0].btY = ::g_btCastleEntranceMapXY[iBridgeIndex].btStartY;
	lpMsgBody[1].btX   = ::g_btCastleEntranceMapXY[iBridgeIndex].btEndX;
	lpMsgBody[1].btY   = ::g_btCastleEntranceMapXY[iBridgeIndex].btEndY;

	for ( int i=OBJ_STARTUSERINDEX;i<OBJMAX;i++)
	{
		if ( gObj[i].MapNumber == (iBridgeIndex + BCSum))
		{
			if ( gObj[i].Connected > PLAYER_LOGGED )
			{
				DataSend(i, (UCHAR *)lpMsg, lpMsg->h.size);

				LogAddTD("[Blood Castle][Bug Tracer] SendCastleBridgeBlockInfo-> Bridge:%d/Live:%d, (%d,%d,%d,%d)",
					iBridgeIndex+1, bLive, lpMsgBody[0].btX, lpMsgBody[0].btY, lpMsgBody[1].btX, lpMsgBody[1].btY);
			}
		}
	}
}





void CBloodCastle::SendCastleDoorBlockInfo(int iBridgeIndex, bool bLive)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}

	int BCSum = MAP_INDEX_BLOODCASTLE1;
	
	if(iBridgeIndex == 7)
		BCSum = 45;

	char cTEMP_BUF[256];
	PMSG_SETMAPATTR_COUNT * lpMsg = (PMSG_SETMAPATTR_COUNT *)cTEMP_BUF;

	PHeadSetB((LPBYTE)lpMsg, 0x46, sizeof(PMSG_SETMAPATTR_COUNT)+sizeof(PMSG_SETMAPATTR)*6);
	PMSG_SETMAPATTR * lpMsgBody = (PMSG_SETMAPATTR *)&cTEMP_BUF[7];
	lpMsg->btType = 0;
	lpMsg->btCount = 3;
	lpMsg->btMapAttr = 4;
	(bLive)?lpMsg->btMapSetType=0:lpMsg->btMapSetType=1;

	lpMsgBody[0].btX = ::g_btCastleDoorMapXY[iBridgeIndex][0].btStartX;
	lpMsgBody[0].btY = ::g_btCastleDoorMapXY[iBridgeIndex][0].btStartY;
	lpMsgBody[1].btX   = ::g_btCastleDoorMapXY[iBridgeIndex][0].btEndX;
	lpMsgBody[1].btY   = ::g_btCastleDoorMapXY[iBridgeIndex][0].btEndY;

	lpMsgBody[2].btX = ::g_btCastleDoorMapXY[iBridgeIndex][1].btStartX;
	lpMsgBody[2].btY = ::g_btCastleDoorMapXY[iBridgeIndex][1].btStartY;
	lpMsgBody[3].btX   = ::g_btCastleDoorMapXY[iBridgeIndex][1].btEndX;
	lpMsgBody[3].btY   = ::g_btCastleDoorMapXY[iBridgeIndex][1].btEndY;

	lpMsgBody[4].btX = ::g_btCastleDoorMapXY[iBridgeIndex][2].btStartX;
	lpMsgBody[4].btY = ::g_btCastleDoorMapXY[iBridgeIndex][2].btStartY;
	lpMsgBody[5].btX   = ::g_btCastleDoorMapXY[iBridgeIndex][2].btEndX;
	lpMsgBody[5].btY   = ::g_btCastleDoorMapXY[iBridgeIndex][2].btEndY;

	for ( int i=OBJ_STARTUSERINDEX;i<OBJMAX;i++)
	{
		if ( gObj[i].MapNumber == (iBridgeIndex + BCSum))
		{
			if ( gObj[i].Connected > PLAYER_LOGGED )
			{
				DataSend(i, (UCHAR *)lpMsg, lpMsg->h.size);
			}
		}
	}
}





void CBloodCastle::SendNoticeMessage(int iBridgeIndex, char * lpszMSG)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}

	PMSG_NOTICE pNotice;
	pNotice.type = 0;	// 3
	pNotice.btCount = 0;	// 4
	pNotice.wDelay = 0;	// 6	
	pNotice.dwColor = 0;	// 8
	pNotice.btSpeed = 0;	// C

	TNotice::MakeNoticeMsg( &pNotice, 0, lpszMSG);

	for ( int i=0;i<MAX_BLOOD_CASTLE_SUB_BRIDGE;i++)
	{
		if ( this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex != -1 )
		{
			if ( gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Connected > PLAYER_LOGGED )
			{
				if ( gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleIndex != -1 )
				{
					if ( gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleSubIndex != -1 )
					{
						DataSend(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, (UCHAR*)&pNotice, pNotice.h.size);
					}
				}
			}
		}
	}
}





void CBloodCastle::SendNoticeScore(int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}

	PMSG_NOTICE pNotice;
	pNotice.type = 0;	// 3
	pNotice.btCount = 0;	// 4
	pNotice.wDelay = 0;	// 6	
	pNotice.dwColor = 0;	// 8
	pNotice.btSpeed = 0;	// C

	for ( int i=0;i<MAX_BLOOD_CASTLE_SUB_BRIDGE;i++)
	{
		if ( this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex != -1 )
		{
			if ( gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Connected > PLAYER_LOGGED )
			{
				if ( gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleIndex != -1 )
				{
					if ( gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleSubIndex != -1 )
					{
						TNotice::MakeNoticeMsgEx(&pNotice, 0, lMsg.Get(MSGGET(4, 140)), iBridgeIndex+1, this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iEXP);
						DataSend(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, (UCHAR*)&pNotice, pNotice.h.size);
					}
				}
			}
		}
	}
}





void CBloodCastle::SendNoticeState(int iBridgeIndex, int iPlayState)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}

	PMSG_STATEBLOODCASTLE pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x9B, sizeof(PMSG_STATEBLOODCASTLE));
	pMsg.btPlayState = iPlayState;
	pMsg.wRemainSec = this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC / 1000;

	if ( iPlayState == 4 )
	{
		pMsg.wMaxKillMonster = this->m_BridgeData[iBridgeIndex].m_iBC_BOSS_MONSTER_MAX_COUNT;
		pMsg.wCurKillMonster = this->m_BridgeData[iBridgeIndex].m_iBC_BOSS_MONSTER_KILL_COUNT;
	}
	else
	{
		pMsg.wMaxKillMonster = this->m_BridgeData[iBridgeIndex].m_iBC_MONSTER_MAX_COUNT;
		pMsg.wCurKillMonster = this->m_BridgeData[iBridgeIndex].m_iBC_MONSTER_KILL_COUNT;
	}

	pMsg.wUserHaveWeapon = this->m_BridgeData[iBridgeIndex].m_iBC_QUEST_ITEM_USER_INDEX;
	pMsg.btWeaponNum = this->m_BridgeData[iBridgeIndex].m_btBC_QUEST_ITEM_NUMBER + 1;

	for ( int i=0;i<MAX_BLOOD_CASTLE_SUB_BRIDGE;i++)
	{
		if ( this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex != -1 )
		{
			if ( gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Connected > PLAYER_LOGGED )
			{
				if ( gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleIndex != -1 )
				{
					if ( gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleSubIndex != -1 )
					{
						DataSend(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, (UCHAR *)&pMsg, pMsg.h.size);
					}
				}
			}
		}
	}
}








bool CBloodCastle::CheckUserBridgeMember(int iBridgeIndex, int iIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return false;
	}

	if ( OBJMAX_RANGE(iIndex) == FALSE )
	{
		return false;
	}

	for ( int i=0;i<MAX_BLOOD_CASTLE_SUB_BRIDGE;i++)
	{
		if ( this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex == iIndex )
		{
			return true;
		}
	}

	return false;
}





int  CBloodCastle::GetAliveUserTotalEXP(int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return 0;
	}

	int iRET_EXP = 0;

	for ( int i=0;i<MAX_BLOOD_CASTLE_SUB_BRIDGE;i++)
	{
		if ( this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex != -1 )
		{
			if ( gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Connected > PLAYER_LOGGED )
			{
				if ( BC_MAP_RANGE(gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].MapNumber) != FALSE )
				{
					iRET_EXP += this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iEXP;
				}
			}
		}
	}

	return iRET_EXP;
}



void CBloodCastle::SearchUserDeleteQuestItem(int iIndex)
{
	if ( gObjIsConnected(iIndex) == FALSE )
	{
		return;
	}

	for ( int x=0;x<ReadConfig.INVENTORY_SIZE(iIndex,false);x++)
	{
		if ( gObj[iIndex].pInventory[x].IsItem() == TRUE )
		{
			if ( gObj[iIndex].pInventory[x].m_Type == ITEMGET(13,19) )
			{
				int iLEVEL = gObj[iIndex].pInventory[x].m_Level;

				if ( BC_WEAPON_LEVEL_RANGE(iLEVEL) != FALSE )
				{
					::gObjInventoryDeleteItem(iIndex, x);
					::GCInventoryItemDeleteSend(iIndex, x, TRUE);

					LogAddTD("[Blood Castle] Force (Account:%s, Name:%s) to Delete Angel King's Weapon (%d) [Serial:%d]",
						gObj[iIndex].AccountID, gObj[iIndex].Name, iLEVEL, gObj[iIndex].pInventory[x].m_Number);
				}
			}
		}
	}
}




void CBloodCastle::SearchUserDropQuestItem(int iIndex)
{
	if ( OBJMAX_RANGE(iIndex) == FALSE )
	{
		return;
	}

	if ( gObj[iIndex].Type != OBJ_USER || gObj[iIndex].Connected <= PLAYER_LOGGED )
	{
		return;
	}

	for ( int x=0;x<ReadConfig.INVENTORY_SIZE(iIndex,false);x++)
	{
		if ( gObj[iIndex].pInventory[x].IsItem() == TRUE )
		{
			if ( gObj[iIndex].pInventory[x].m_Type == ITEMGET(13,19) )
			{
				int iLEVEL = gObj[iIndex].pInventory[x].m_Level;

				if ( BC_WEAPON_LEVEL_RANGE(iLEVEL) != FALSE )
				{
					BYTE pMsg[6];
					pMsg[5] = x;
					pMsg[3] = gObj[iIndex].X;
					pMsg[4] = gObj[iIndex].Y;

					if(gObj[iIndex].MapNumber == MAP_INDEX_BLOODCASTLE8)
					{
						this->DropItemDirectly(7,
							gObj[iIndex].m_Index, ITEMGET(13, 19), x);
					}else
					{
						this->DropItemDirectly(gObj[iIndex].MapNumber - MAP_INDEX_BLOODCASTLE1,
							gObj[iIndex].m_Index, ITEMGET(13, 19), x);
					}

					if ( BC_MAP_RANGE(gObj[iIndex].MapNumber) != FALSE )
					{
						int iBC_INDEX = gObj[iIndex].MapNumber - MAP_INDEX_BLOODCASTLE1;
						
						if(gObj[iIndex].MapNumber == MAP_INDEX_BLOODCASTLE8)
							iBC_INDEX = 7;

						if ( this->m_BridgeData[iBC_INDEX].m_nBC_QUESTITEM_SERIAL != -1 )
						{
							if ( this->m_BridgeData[iBC_INDEX].m_nBC_QUESTITEM_SERIAL == gObj[iIndex].pInventory[x].m_Number )
							{
								this->m_BridgeData[iBC_INDEX].m_iBC_QUEST_ITEM_USER_INDEX = -1;
							}
						}

						LogAddTD("[Blood Castle] Force (Account:%s, Name:%s) to Drop Angel King's Weapon (%d) [Serial:%d-%d]",
							gObj[iIndex].AccountID, gObj[iIndex].Name, iLEVEL, this->m_BridgeData[iBC_INDEX].m_nBC_QUESTITEM_SERIAL,
							gObj[iIndex].pInventory[x].m_Number);

					}
					else
					{
						LogAddTD("[Blood Castle] Force (Account:%s, Name:%s) to Drop Angel King's Weapon (%d) [Serial:%d]",
							gObj[iIndex].AccountID, gObj[iIndex].Name, iLEVEL,
							gObj[iIndex].pInventory[x].m_Number);
					}
				}
			}
		}
	}
}





void CBloodCastle::SetUserState(int iIndex, int iState)
{
	if ( OBJMAX_RANGE(iIndex) == FALSE )
	{
		return;
	}

	int iBridgeIndex = gObj[iIndex].m_cBloodCastleIndex;
	int iBridgeSubIndex = gObj[iIndex].m_cBloodCastleSubIndex;

	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}

	if ( BC_SUB_BRIDGE_RANGE(iBridgeSubIndex) == FALSE )
	{
		return;
	}

	switch ( iState )
	{
		case 0:
			this->m_BridgeData[iBridgeIndex].m_UserData[iBridgeSubIndex].m_iUserState = 0;
			break;

		case 1:
			this->m_BridgeData[iBridgeIndex].m_UserData[iBridgeSubIndex].m_iUserState = 1;
			break;

		case 2:
			if ( gObj[iIndex].PartyNumber >= 0 )
			{
				for ( int i=0;i<MAX_BLOOD_CASTLE_SUB_BRIDGE;i++)
				{
					if ( i == iBridgeSubIndex )
					{
						continue;
					}

					if ( gObj[iIndex].PartyNumber == gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].PartyNumber )
					{
						if ( BC_MAP_RANGE( gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex ].MapNumber ) != FALSE )
						{
							this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState = 3;
						}
						else
						{
							this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState = 4;
						}
					}
				}
			}

			this->m_BridgeData[iBridgeIndex].m_UserData[iBridgeSubIndex].m_iUserState = 2;
			this->m_BridgeData[iBridgeIndex].m_iMISSION_SUCCESS = iBridgeSubIndex;
			break;

		case 3:
			this->m_BridgeData[iBridgeIndex].m_UserData[iBridgeSubIndex].m_iUserState = 3;
			break;

		case 4:
			this->m_BridgeData[iBridgeIndex].m_UserData[iBridgeSubIndex].m_iUserState = 4;
			break;
	}
}



struct ST_BC_SCORE	// size == 0x18
{
	char CharName[MAX_ACCOUNT_LEN];	// unk0
	int  iSCORE;	// unkC
	int  iEXP;	// unk10
	int  iZEN;	// unk14
	

};

#pragma pack(1)

struct GCS_BC_GIVE_REWARD	//Send[C1:93] - #error WARNING (Deathway) - Check with Packet of DevilSquareGround.h Ranking 
{
	PHEADB PHeader;
	bool bWinner;
	BYTE btType;
	ST_BC_SCORE m_stBCCharScore[MAX_BLOOD_CASTLE_SUB_BRIDGE+1];
};

#pragma pack()


// ///////////////////////////////////
// START REVIEW HERE


void CBloodCastle::GiveReward_Win(int iIndex, int iBridgeIndex)
{
	this->FixUsersPlayStateWin(iBridgeIndex);
	char szNOTIFY_MSG[256];
	int iREWARD_EXP = 0;
	int iREWARD_ZEN = 0;
	int iREWARD_SCR = 0;
	int iLEFT_TIME = this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC / 1000;
	int iALIVE_PARTYCOUNT = this->GetWinnerPartyCompleteCount(iBridgeIndex);
	int iADD_PARTYPOINT = this->GetWinnerPartyCompletePoint(iBridgeIndex);

	LogAddTD("[Blood Castle] (%d) Winner Party Point - Alive Party Member : %d, Alive Party Point : %d",
		iBridgeIndex+1, iALIVE_PARTYCOUNT, iADD_PARTYPOINT);

	if ( OBJMAX_RANGE(iIndex) == FALSE )
	{
		return;
	}

	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}
	
	if ( this->m_BridgeData[iBridgeIndex].m_bBC_REWARDED != false )
	{
		return;
	}

	if ( gObj[iIndex].Connected > PLAYER_LOGGED )
	{
		gObj[iIndex].Name[MAX_ACCOUNT_LEN] = 0;
		wsprintf(szNOTIFY_MSG, lMsg.Get(MSGGET(4, 141)), gObj[iIndex].Name);
	}
	else
	{
		wsprintf(szNOTIFY_MSG, lMsg.Get(MSGGET(4, 142)));
	}

	if ( this->m_BridgeData[iBridgeIndex].m_iMISSION_SUCCESS != -1 )
	{
		int iTOTAL_EXP = this->GetAliveUserTotalEXP(iBridgeIndex);
		LogAddTD("[Blood Castle] (%d) Quest Complete - All Alive User's Total EXP:%d",
			iBridgeIndex+1, iTOTAL_EXP);

		for ( int i=0;i<MAX_BLOOD_CASTLE_SUB_BRIDGE;i++)
		{
			if ( this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex == -1 )
			{
				continue;
			}

			if ( gObj[ this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex ].Connected < PLAYER_PLAYING )
			{
				continue;
			}

			if ( gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleIndex == -1 || gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleSubIndex == -1 ||  gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleIndex != iBridgeIndex )
			{
				continue;
			}

			PMSG_NOTICE pNotice;
			pNotice.type = 0;	// 3
			pNotice.btCount = 0;	// 4
			pNotice.wDelay = 0;	// 6	
			pNotice.dwColor = 0;	// 8
			pNotice.btSpeed = 0;	// C

			TNotice::MakeNoticeMsg(&pNotice, 10, szNOTIFY_MSG);
			TNotice::SetNoticeProperty(&pNotice, 10, _ARGB(255, 128, 149, 196), 1, 0, 20);
			TNotice::SendNoticeToUser(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, (TNotice*)&pNotice);

			int iADD_EXP = 0;

			if ( (gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].PartyNumber != -1 && gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].PartyNumber == this->m_BridgeData[iBridgeIndex].m_iExtraEXP_Kill_Door_Party) || this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex == this->m_BridgeData[iBridgeIndex].m_iExtraEXP_Kill_Door_Index)
			{
				iADD_EXP += ::g_iBC_Add_Exp[iBridgeIndex].unk0;
			}
			else
			{
				if ( gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].PartyNumber != -1 )
				{
					if ( this->m_BridgeData[iBridgeIndex].m_UserData[i].m_bLiveWhenDoorBreak != false )
					{
						iADD_EXP += ::g_iBC_Add_Exp[iBridgeIndex].unk0 / 2;
					}
				}
			}

			if ( (gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].PartyNumber != -1 && gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].PartyNumber == this->m_BridgeData[iBridgeIndex].m_iExtraEXP_Kill_Statue_Party) || this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex == this->m_BridgeData[iBridgeIndex].m_iExtraEXP_Kill_Statue_Index)
			{
				iADD_EXP += ::g_iBC_Add_Exp[iBridgeIndex].unk4;
			}

			if ( (gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].PartyNumber != -1 && gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].PartyNumber == this->m_BridgeData[iBridgeIndex].m_iExtraEXP_Win_Quest_Party) || this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex == this->m_BridgeData[iBridgeIndex].m_iExtraEXP_Win_Quest_Index)
			{
				iADD_EXP += ::g_iBC_Add_Exp[iBridgeIndex].unk8;
			}

			switch ( this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState  )
			{
				case 0:
					iADD_EXP += this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC / 1000 * ::g_iBC_Add_Exp[iBridgeIndex].unkC;
					iREWARD_EXP = this->CalcSendRewardEXP(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, iADD_EXP);
					iREWARD_ZEN = this->CalcSendRewardZEN(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, ::g_iQuestWinExpendZEN[iBridgeIndex].SpecialCharacter);
					iREWARD_SCR = ::g_iBC_EventScore[iBridgeIndex].unk0;
					this->SendRewardScore(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, iREWARD_SCR, iLEFT_TIME, 0);

					LogAddTD("[Blood Castle] (%d) Quest Complete Rewarded (Account:%s, Name:%s [State:%d], Exp:%d, ExtExp:%d, Zen:%d, Score:%d, LeftTime:%d, MapNumber:%d)",
						iBridgeIndex+1, gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].AccountID,
						gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Name,
						0, gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_iBloodCastleEXP,
						iREWARD_EXP, iREWARD_ZEN, iREWARD_SCR, iLEFT_TIME, gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].MapNumber);
					break;

				case 1:
					iADD_EXP += this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC / 1000 * ::g_iBC_Add_Exp[iBridgeIndex].unkC;
					iREWARD_EXP = this->CalcSendRewardEXP(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, iADD_EXP);
					iREWARD_ZEN = this->CalcSendRewardZEN(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, ::g_iQuestWinExpendZEN[iBridgeIndex].SpecialCharacter);
					iREWARD_SCR = ::g_iBC_EventScore[iBridgeIndex].unk4;
					this->SendRewardScore(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, iREWARD_SCR, iLEFT_TIME, 0);

					LogAddTD("[Blood Castle] (%d) Quest Complete Rewarded (Account:%s, Name:%s [State:%d], Exp:%d, ExtExp:%d, Zen:%d, Score:%d, LeftTime:%d, MapNumber:%d)",
						iBridgeIndex+1, gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].AccountID,
						gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Name,
						1, gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_iBloodCastleEXP,
						iREWARD_EXP, iREWARD_ZEN, iREWARD_SCR, iLEFT_TIME, gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].MapNumber);
					break;

				case 2:
					iADD_EXP += this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC / 1000 * ::g_iBC_Add_Exp[iBridgeIndex].unkC;
					iREWARD_EXP = this->CalcSendRewardEXP(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, iADD_EXP);
					iREWARD_ZEN = this->CalcSendRewardZEN(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, ::g_iQuestWinExpendZEN[iBridgeIndex].NormalCharacter);
					iREWARD_SCR = ::g_iBC_EventScore[iBridgeIndex].unk8;
					iREWARD_SCR += iADD_PARTYPOINT;

					if ( BC_MAP_RANGE(gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].MapNumber ) != FALSE )
					{
						this->DropChaosGem(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex);
					}


					this->SendRewardScore(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, iREWARD_SCR, iLEFT_TIME, iALIVE_PARTYCOUNT);

					LogAddTD("[Blood Castle] (%d) Quest Complete Rewarded (Account:%s, Name:%s [State:%d], Exp:%d, ExtExp:%d, Zen:%d, Score:%d, LeftTime:%d, MapNumber:%d)",
						iBridgeIndex+1, gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].AccountID,
						gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Name,
						2, gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_iBloodCastleEXP,
						iREWARD_EXP, iREWARD_ZEN, iREWARD_SCR, iLEFT_TIME, gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].MapNumber);
					break;

				case 3:
					iADD_EXP += this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC / 1000 * ::g_iBC_Add_Exp[iBridgeIndex].unkC;
					iREWARD_EXP = this->CalcSendRewardEXP(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, iADD_EXP);
					iREWARD_ZEN = this->CalcSendRewardZEN(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, ::g_iQuestWinExpendZEN[iBridgeIndex].NormalCharacter);
					iREWARD_SCR = ::g_iBC_EventScore[iBridgeIndex].unkC;
					iREWARD_SCR += iADD_PARTYPOINT;

					if ( BC_MAP_RANGE(gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].MapNumber ) != FALSE )
					{
						this->DropChaosGem(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex);
					}


					this->SendRewardScore(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, iREWARD_SCR, iLEFT_TIME, iALIVE_PARTYCOUNT);

					LogAddTD("[Blood Castle] (%d) Quest Complete Rewarded (Account:%s, Name:%s [State:%d], Exp:%d, ExtExp:%d, Zen:%d, Score:%d, LeftTime:%d, MapNumber:%d)",
						iBridgeIndex+1, gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].AccountID,
						gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Name,
						3, gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_iBloodCastleEXP,
						iREWARD_EXP, iREWARD_ZEN, iREWARD_SCR, iLEFT_TIME, gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].MapNumber);
					break;

				case 4:
					iADD_EXP += this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC / 1000 * ::g_iBC_Add_Exp[iBridgeIndex].unkC;
					iREWARD_EXP = this->CalcSendRewardEXP(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, iADD_EXP);
					iREWARD_ZEN = this->CalcSendRewardZEN(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, ::g_iQuestWinExpendZEN[iBridgeIndex].NormalCharacter);
					iREWARD_SCR = ::g_iBC_EventScore[iBridgeIndex].unk10;
					this->SendRewardScore(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, iREWARD_SCR, iLEFT_TIME, iALIVE_PARTYCOUNT);

					LogAddTD("[Blood Castle] (%d) Quest Complete Rewarded (Account:%s, Name:%s [State:%d], Exp:%d, ExtExp:%d, Zen:%d, Score:%d, LeftTime:%d, MapNumber:%d)",
						iBridgeIndex+1, gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].AccountID,
						gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Name,
						4, gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_iBloodCastleEXP,
						iREWARD_EXP, iREWARD_ZEN, iREWARD_SCR, iLEFT_TIME, gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].MapNumber);
					break;
			}

			GCS_BC_GIVE_REWARD pMsg;

			pMsg.bWinner = true;
			pMsg.btType = -1;
			memcpy(pMsg.m_stBCCharScore[0].CharName , gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Name, MAX_ACCOUNT_LEN);
			pMsg.m_stBCCharScore[0].iEXP = iREWARD_EXP;
			pMsg.m_stBCCharScore[0].iZEN = iREWARD_ZEN;
			pMsg.m_stBCCharScore[0].iSCORE = iREWARD_SCR;
			PHeadSetB((LPBYTE)&pMsg.PHeader, 0x93, sizeof(GCS_BC_GIVE_REWARD) - (sizeof(ST_BC_SCORE) * (MAX_BLOOD_CASTLE_SUB_BRIDGE -1)) );

			if(BC_MAP_RANGE(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex) == TRUE)
				qs5.FinishEvent(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex,3,iBridgeIndex+1); //FINISH BC QUEST

			DataSend(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, (UCHAR*)&pMsg, pMsg.PHeader.uSize);
		}

		this->m_BridgeData[iBridgeIndex].m_bBC_REWARDED = true;
	}
}




void CBloodCastle::GiveReward_Fail(int iBridgeIndex)
{
	this->FixUsersPlayStateFail(iBridgeIndex);

	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}

	int iUserWhoGotUltimateWeapon = -1;
	iUserWhoGotUltimateWeapon = this->GetWhoGotUltimateWeapon(iBridgeIndex);

	if ( iUserWhoGotUltimateWeapon != -1 )
	{
		if ( OBJMAX_RANGE(iUserWhoGotUltimateWeapon) != FALSE )
		{
			LogAddTD("[Blood Castle] (%d) (Account:%s, Name:%s) got Ultimate Weapon (%d) but Failed - Time Out",
				iBridgeIndex+1, gObj[iUserWhoGotUltimateWeapon].AccountID, gObj[iUserWhoGotUltimateWeapon].Name,
				this->m_BridgeData[iBridgeIndex].m_btBC_QUEST_ITEM_NUMBER);
		}
	}

	for ( int i=0;i<MAX_BLOOD_CASTLE_SUB_BRIDGE;i++)
	{
		if ( this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex == -1 )
		{
			continue;
		}

		if ( gObj[ this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex ].Connected < PLAYER_PLAYING )
		{
			continue;
		}

		if ( gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleIndex == -1 || gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleSubIndex == -1 )
		{
			continue;
		}	

		int iADD_EXP = 0;

		if ( (gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].PartyNumber != -1 && gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].PartyNumber == this->m_BridgeData[iBridgeIndex].m_iExtraEXP_Kill_Door_Party) || this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex == this->m_BridgeData[iBridgeIndex].m_iExtraEXP_Kill_Door_Index)
		{
			iADD_EXP += ::g_iBC_Add_Exp[iBridgeIndex].unk0;
		}
		else
		{
			if ( gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].PartyNumber != -1 )
			{
				if ( this->m_BridgeData[iBridgeIndex].m_UserData[i].m_bLiveWhenDoorBreak != false )
				{
					iADD_EXP += ::g_iBC_Add_Exp[iBridgeIndex].unk0 / 2;
				}
			}
		}

		if ( (gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].PartyNumber != -1 && gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].PartyNumber == this->m_BridgeData[iBridgeIndex].m_iExtraEXP_Kill_Statue_Party) || this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex == this->m_BridgeData[iBridgeIndex].m_iExtraEXP_Kill_Statue_Index)
		{
			iADD_EXP += ::g_iBC_Add_Exp[iBridgeIndex].unk4;
		}

		this->CalcSendRewardEXP(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, iADD_EXP);
		this->SendRewardScore(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, ::g_iBC_EventScore_Fail[iBridgeIndex], 0, 0);

		LogAddTD("[Blood Castle] (%d) Quest Fail Rewarded (Account:%s, Name:%s, Score:%d, ExtEXP:%d, LeftTime:%d)",
			iBridgeIndex+1, gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].AccountID,
			gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Name,
			::g_iBC_EventScore_Fail[iBridgeIndex], iADD_EXP, 0);

		GCS_BC_GIVE_REWARD pMsg;

		pMsg.bWinner = false;
		pMsg.btType = -1;
		memcpy(pMsg.m_stBCCharScore[0].CharName, gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Name, MAX_ACCOUNT_LEN);
		pMsg.m_stBCCharScore[0].iEXP = iADD_EXP;
		pMsg.m_stBCCharScore[0].iZEN = 0;
		pMsg.m_stBCCharScore[0].iSCORE = g_iBC_EventScore_Fail[iBridgeIndex];
		PHeadSetB((LPBYTE)&pMsg.PHeader, 0x93, sizeof(GCS_BC_GIVE_REWARD) - (sizeof(ST_BC_SCORE) * (MAX_BLOOD_CASTLE_SUB_BRIDGE -1)) );

		if ( gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleIndex != -1 && gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleSubIndex != -1 )
		{
			DataSend(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, (UCHAR*)&pMsg, pMsg.PHeader.uSize);			

			LogAddTD("[Blood Castle] (%d) Send User Quest Fail Message (Account:%s, Name:%s [State:%d], MapNumber:%d)",
				iBridgeIndex+1, gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].AccountID,
				gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Name, 
				this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState,
				gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].MapNumber);
		}	
	}
}




int  CBloodCastle::CalcSendRewardEXP(int iIndex, int iEXP)
{
	if ( iEXP <= 0 )
	{
		return 0;
	}

	int iMAX_LEVCOUNT = 0;
	int iRET_EXP = 0;
	int iCAL_EXP = iEXP;

	if ( OBJMAX_RANGE(iIndex) == FALSE )
	{
		return 0;
	}

	if ( gObj[iIndex].Connected < PLAYER_PLAYING )
	{
		return 0;
	}

	iRET_EXP = iCAL_EXP;

	if ( gObj[iIndex].Type == OBJ_USER )
	{
		if ( g_CrywolfSync.GetOccupationState() == CRYWOLF_OCCUPATION_FAILED && g_iCrywolfApplyMvpPenalty )
		{
#if (PACK_EDITION>=2)
			if ((VipSystem.VipIsApplyCWExpPenalty == 0)&&(gObj[iIndex].Vip >= 1))
			{
			} else {
				iEXP = iEXP * g_CrywolfSync.GetGettingExpPenaltyRate() / 100;
			}
#else
			iEXP = iEXP * g_CrywolfSync.GetGettingExpPenaltyRate() / 100;
#endif
		}

		if ( (gObj[iIndex].m_wExprienceRate + gObj[iIndex].MasterCharacterInfo->IncExperience) == 0 )
		{
			iEXP = 0;
			iCAL_EXP = 0;
		}

		if ( gObj[iIndex].m_btDisableExpGain == 1 )
		{
			iEXP = 0;
			iCAL_EXP = 0;
		}

		gObjSetExpPetItem(iIndex, iRET_EXP);
		while ( iCAL_EXP > 0 )
		{
			if ( iCAL_EXP > 0 )
			{
				if ( (gObj[iIndex].m_wExprienceRate + gObj[iIndex].MasterCharacterInfo->IncExperience) > 0 && gObj[iIndex].m_btDisableExpGain == 0)
				{
					int LevelUp;
					iCAL_EXP = gObjLevelUp(&gObj[iIndex], iCAL_EXP, 0, EVENT_TYPE_BLOODCASTLE,LevelUp);
				}
				else
				{
					iEXP = 0;
					iCAL_EXP = 0;
					iRET_EXP = 0;
				}

				iMAX_LEVCOUNT++;

				if ( iMAX_LEVCOUNT > 5 )
					break;
			}
		}

		PMSG_KILLPLAYER_EXT pMsg;

		PHeadSetBE((LPBYTE)&pMsg, 0x9C, sizeof(pMsg));
		pMsg.NumberH = -1;
		pMsg.NumberL = -1;
		pMsg.ExpH = SET_NUMBERHW(iEXP);
		pMsg.ExpL = SET_NUMBERLW(iEXP);
		pMsg.DamageH = 0;
		pMsg.DamageL = 0;

		DataSend(iIndex, (UCHAR*)&pMsg, pMsg.h.size);
	}

	return iRET_EXP;
}



int  CBloodCastle::CalcSendRewardZEN(int iIndex, int iZEN)
{
	if ( iZEN <= 0 )
	{
		return 0;
	}

	int iRET_ZEN = 0;

	if ( OBJMAX_RANGE(iIndex) == FALSE )
	{
		return 0;
	}

	if ( gObj[iIndex].Connected < PLAYER_PLAYING )
	{
		return 0;
	}

	__int64 tmpZen = (__int64)gObj[iIndex].Money + (__int64)iZEN;
	if (tmpZen > MAX_ZEN)
	{
		iRET_ZEN = MAX_ZEN - gObj[iIndex].Money;
		gObj[iIndex].Money += iRET_ZEN;
		return iRET_ZEN;
	}

	gObj[iIndex].Money += iZEN;
	iRET_ZEN = iZEN;
	GCMoneySend(iIndex, gObj[iIndex].Money);


	return iRET_ZEN;
}





void CBloodCastle::DropChaosGem(int iIndex)
{
	if ( OBJMAX_RANGE(iIndex) == FALSE )
	{
		return;
	}

	#if (WL_PROTECT==1)  
		VM_START_WITHLEVEL(3)
		int MyCheckVar1;
		CHECK_PROTECTION(MyCheckVar1, 0x86310437)  
		if (MyCheckVar1 != 0x86310437)
		{	
			char text[2];
			char text2[20]="ddddeee";
			memcpy(&text,text2,sizeof(text2));
			gCreateCharacter = 0;
			gItemDropPer = 1;
			gAddExperience = 1.0f;
		}
		VM_END
	#endif

	int iMaxHitUser = iIndex;
	int iType = ::ItemGetNumberMake(g_iBCDropChaosGemItemType, g_iBCDropChaosGemItemIndex);	// Chaos Gem 12,15
	
	if(gObj[iIndex].MapNumber != 52)
		ItemSerialCreateSend(gObj[iIndex].m_Index, gObj[iIndex].MapNumber + 227, gObj[iIndex].X,
			gObj[iIndex].Y, iType, g_iBCDropChaosGemItemLevel, g_iBCDropChaosGemItemDur, g_iBCDropChaosGemItemSkill, g_iBCDropChaosGemItemLuck, g_iBCDropChaosGemItemOpt, iMaxHitUser, g_iBCDropChaosGemItemExc, 0);
	else		
		ItemSerialCreateSend(gObj[iIndex].m_Index, gObj[iIndex].MapNumber, gObj[iIndex].X,
			gObj[iIndex].Y, iType, g_iBCDropChaosGemItemLevel, g_iBCDropChaosGemItemDur, g_iBCDropChaosGemItemSkill, g_iBCDropChaosGemItemLuck, g_iBCDropChaosGemItemOpt, iMaxHitUser, g_iBCDropChaosGemItemExc, 0);
}





struct PMSG_ANS_BLOODCASTLESCORE_5TH
{
	PBMSG_HEAD h;	// C1:0D
	char AccountID[10];	// 3
	char GameID[10];	// D
	int ServerCode;	// 18
	int Score;	// 1C
	int Class;	// 20
	int BridgeNum;	// 24
	int iLeftTime;	// 28
	int iAlivePartyCount;	// 2C
};





void CBloodCastle::SendRewardScore(int iIndex, int iSCORE, int iLeftTime, int iAlivePartyCount)
{
	if(ReadConfig.SCFRSON == FALSE)
	{
		if ( OBJMAX_RANGE(iIndex) == FALSE )
		{
			return;
		}

		PMSG_ANS_BLOODCASTLESCORE_5TH pMsg;

		pMsg.h.c = 0xC1;
		pMsg.h.headcode = 0x0D;
		pMsg.h.size = sizeof(pMsg);
		pMsg.Score = iSCORE;
		pMsg.BridgeNum = gObj[iIndex].m_cBloodCastleIndex;
		pMsg.Class = gObj[iIndex].Class;
		pMsg.ServerCode = gGameServerCode;
		pMsg.iLeftTime = iLeftTime;
		memcpy(pMsg.AccountID, gObj[iIndex].AccountID, MAX_ACCOUNT_LEN);
		memcpy(pMsg.GameID, gObj[iIndex].Name, MAX_ACCOUNT_LEN);
		pMsg.iAlivePartyCount = iAlivePartyCount;

		if( ::IsDevilSquareEventConnected == FALSE && ::DevilSquareEventConnect == FALSE )
		{
			wsRServerCli.Close();
			wsRServerCli.CreateSocket(ghWnd);

			if ( GMRankingServerConnect(gDevilSquareEventServerIp, WM_GM_RANKING_CLIENT_MSG_PROC) == FALSE )
			{
				::IsDevilSquareEventConnected = FALSE;
				LogAddTD("Can not connect Ranking Server");

				return;
			}

			::IsDevilSquareEventConnected = TRUE;
		}

		if ( ::DevilSquareEventConnect == FALSE && ::IsDevilSquareEventConnected != FALSE )
		{
			wsRServerCli.DataSend((char *)&pMsg, pMsg.h.size);
		}
	}else
	{
		BC_SendRewardScore(iIndex,iSCORE,iLeftTime,iAlivePartyCount);
	}
}






void CBloodCastle::SendBridgeAnyMsg(BYTE * lpMsg, int iSize, int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}

	for ( int i=0;i<MAX_BLOOD_CASTLE_SUB_BRIDGE;i++)
	{
		if ( this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex != -1 )
		{
			if ( gObj[ this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex ].Connected == PLAYER_PLAYING )
			{
				if ( gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleIndex != -1 && gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleSubIndex != -1 )
				{
					DataSend(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, lpMsg, iSize);
				}
			}
		}
	}

}





void CBloodCastle::SendAllUserAnyMsg(BYTE * lpMsg, int iSize)
{
	for ( int i=OBJ_STARTUSERINDEX;i<OBJMAX;i++)
	{
		if ( gObj[i].Connected == PLAYER_PLAYING )
		{
			if ( gObj[i].Type == OBJ_USER )
			{
				DataSend(i, lpMsg, iSize);
			}
		}
	}
}






void CBloodCastle::SetMonsterKillCount(int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}

	int iTOT_USER_COUNT = 0;
	int iLIVE_USER_COUNT = 0;
	int iKILL_USER_COUNT = 0;

	for ( int i=0;i<MAX_BLOOD_CASTLE_SUB_BRIDGE;i++)
	{
		if ( this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex != -1 )
		{
			iTOT_USER_COUNT++;

			if ( BC_MAP_RANGE(gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].MapNumber) != FALSE )
			{
				if ( gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Connected > PLAYER_LOGGED )
				{
					iLIVE_USER_COUNT++;
				}
				else
				{
					iKILL_USER_COUNT++;
				}
			}
			else
			{
				iKILL_USER_COUNT++;
			}

		}
	}

	this->m_BridgeData[iBridgeIndex].m_iBC_MONSTER_MAX_COUNT = iLIVE_USER_COUNT * 40;
	this->m_BridgeData[iBridgeIndex].m_iBC_MONSTER_KILL_COUNT = 0;

	LogAddTD("[Blood Castle] (%d) Monster Kill Count Set - Monster:%d, Current Monster:%d, USER TOT:%d, LIVE:%d, DEAD:%d",
		iBridgeIndex+1, this->m_BridgeData[iBridgeIndex].m_iBC_MONSTER_MAX_COUNT,
		this->m_BridgeData[iBridgeIndex].m_iBC_MONSTER_KILL_COUNT, 
		iTOT_USER_COUNT, iLIVE_USER_COUNT, iKILL_USER_COUNT);
}







bool CBloodCastle::CheckMonsterKillCount(int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return false;
	}

	if( this->m_BridgeData[iBridgeIndex].m_iBC_MONSTER_KILL_COUNT >= this->m_BridgeData[iBridgeIndex].m_iBC_MONSTER_MAX_COUNT )
	{
		LogAddTD("[Blood Castle][Bug Tracer] (%d) All Monster Kill Success(Max Kill Count:%d / Kill Count:%d)",
			iBridgeIndex+1, this->m_BridgeData[iBridgeIndex].m_iBC_MONSTER_MAX_COUNT,
			this->m_BridgeData[iBridgeIndex].m_iBC_MONSTER_KILL_COUNT);

		return true;
	}

	return false;
}





bool CBloodCastle::CheckMonsterKillSuccess(int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return false;
	}

	return this->m_BridgeData[iBridgeIndex].m_bBC_MONSTER_KILL_COMPLETE;
}




bool CBloodCastle::CheckBossKillCount(int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return false;
	}

	if ( this->m_BridgeData[iBridgeIndex].m_iBC_BOSS_MONSTER_KILL_COUNT >= this->m_BridgeData[iBridgeIndex].m_iBC_BOSS_MONSTER_MAX_COUNT )
	{
		LogAddTD("[Blood Castle][Bug Tracer] (%d) All Boss Monster Kill Success(Max Boss Kill Count:%d / Boss Kill Count:%d)",
			iBridgeIndex+1, this->m_BridgeData[iBridgeIndex].m_iBC_BOSS_MONSTER_MAX_COUNT,
			this->m_BridgeData[iBridgeIndex].m_iBC_BOSS_MONSTER_KILL_COUNT);

		return true;
	}

	return false;
}





bool CBloodCastle::CheckBossKillSuccess(int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return false;
	}

	return this->m_BridgeData[iBridgeIndex].m_bBC_BOSS_MONSTER_KILL_COMPLETE;
}





bool CBloodCastle::CheckEveryUserDie(int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return false;
	}


	int BCSum = MAP_INDEX_BLOODCASTLE1;
	
	if(iBridgeIndex == 7)
		BCSum = 45;

	bool bRET_VAL = true;

	for ( int i=0;i<MAX_BLOOD_CASTLE_SUB_BRIDGE;i++)
	{
		if ( this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex != -1 )
		{
			if ( OBJMAX_RANGE(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex) == FALSE )
			{
				LogAddTD("error-L3 : [Blood Castle] (%d) User Out of Bound UserIndex:%d, SubIndex:%d",
					iBridgeIndex+1, this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, i);
				
				continue;
			}

			if ( gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].MapNumber == (iBridgeIndex + BCSum) )
			{
				if ( gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Connected > PLAYER_LOGGED )
				{
					bRET_VAL = false;
				}
				else
				{
					LogAddTD("error-L3 : [Blood Castle] (%d) Connection Closed UserIndex:%d, SubIndex:%d",
						iBridgeIndex+1, this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, i);
				}
			}
			else
			{
				if ( this->m_BridgeData[iBridgeIndex].m_UserData[i].m_bSendQuitMsg == false )
				{
					this->m_BridgeData[iBridgeIndex].m_UserData[i].m_bSendQuitMsg = true;

					PMSG_STATEBLOODCASTLE pMsg;

					PHeadSetB((LPBYTE)&pMsg, 0x9B, sizeof(PMSG_STATEBLOODCASTLE));
					pMsg.btPlayState = 2;
					pMsg.wRemainSec = 0;
					pMsg.wMaxKillMonster = 0;
					pMsg.wCurKillMonster = 0;
					pMsg.wUserHaveWeapon = 0;
					pMsg.btWeaponNum = -1;

					DataSend(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, (UCHAR*)&pMsg, pMsg.h.size);
				}
			}
		}
	}

	return bRET_VAL;
}




bool CBloodCastle::CheckAngelKingExist(int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return false;
	}


	int SumBC = MAP_INDEX_BLOODCASTLE1;
	
	if(iBridgeIndex == 7)
		SumBC = 45;


	bool bRET_VAL = false;

	if ( this->m_BridgeData[iBridgeIndex].m_iAngelKingPosNum == -1 )
	{
		LogAddTD("[Blood Castle] (%d) Angel King's Monster Position Doesn't Exist",
			iBridgeIndex+1);

		return false;
	}

	for ( int i=0;i<OBJ_STARTUSERINDEX;i++)
	{
		if ( gObj[i].Connected == PLAYER_PLAYING && gObj[i].Type == OBJ_NPC )
		{
			if ( gObj[i].Class == 232 )
			{
				if (gObj[i].MapNumber == (iBridgeIndex + SumBC) )
				{
					bRET_VAL = true;
					break;
				}
			}
		}
	}

	if ( bRET_VAL == false )
	{
		int result = gObjAddMonster(iBridgeIndex+SumBC);

		if ( result >= 0 )
		{
			gObjSetPosMonster(result, this->m_BridgeData[iBridgeIndex].m_iAngelKingPosNum);
			gObjSetMonster(result, 232,"CBloodCastle::CheckAngelKingExist");
			gObj[result].m_cBloodCastleIndex = gObj[result].MapNumber - SumBC;
			bRET_VAL = true;
		}
		else
		{
			LogAddTD("[Blood Castle] (%d) Angel King - gObjAdd() failed (no space to add monster, result:%d)",
				iBridgeIndex+1, result);

			return false;
		}
	}

	return bRET_VAL;
}






int  CBloodCastle::GetWhoGotUltimateWeapon(int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return -1;
	}

	int iBridgeUserIndex = -1;

	for ( int i=0;i<MAX_BLOOD_CASTLE_SUB_BRIDGE;i++)
	{
		int iIndex = this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex;

		if ( OBJMAX_RANGE(iIndex) == FALSE )
		{
			continue;
		}

		if( gObj[iIndex].Type != OBJ_USER || gObj[iIndex].Connected <= PLAYER_LOGGED )
		{
			continue;
		}

		if ( BC_MAP_RANGE( gObj[iIndex].MapNumber ) == FALSE )
		{
			continue;
		}

		for ( int x=0;x<ReadConfig.INVENTORY_SIZE(iIndex,false);x++)
		{
			if ( gObj[iIndex].pInventory[x].IsItem() == TRUE )
			{
				if ( gObj[iIndex].pInventory[x].m_Type == ITEMGET(13,19) )
				{
					int iLEVEL = gObj[iIndex].pInventory[x].m_Level;

					if ( BC_WEAPON_LEVEL_RANGE(iLEVEL) != FALSE )
					{
						if ( this->m_BridgeData[iBridgeIndex].m_nBC_QUESTITEM_SERIAL != -1 )
						{
							if ( this->m_BridgeData[iBridgeIndex].m_nBC_QUESTITEM_SERIAL == gObj[iIndex].pInventory[x].m_Number )
							{
								iBridgeUserIndex = iIndex;
								break;
							}
						}
					}
				}
			}
		}

	}

	return iBridgeUserIndex;
}






int  CBloodCastle::GetCurrentLiveUserCount(int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return -1;
	}

	int iRetLiveUserCount = 0;

	for ( int i=0;i<MAX_BLOOD_CASTLE_SUB_BRIDGE;i++)
	{
		int iIndex = this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex;

		if ( OBJMAX_RANGE(iIndex) == FALSE )
		{
			continue;
		}

		if ( BC_MAP_RANGE( gObj[iIndex].MapNumber ) == FALSE )
		{
			continue;
		}

		if( gObj[iIndex].Type != OBJ_USER  )
		{
			continue;
		}

		if ( gObj[iIndex].Connected > PLAYER_LOGGED )
		{
			iRetLiveUserCount++;
		}
	}

	return iRetLiveUserCount;
}





BOOL CBloodCastle::DropItemDirectly(int iBridgeIndex, int iIndex, int iItemType, int iItemPos)
{
	int SumBC = MAP_INDEX_BLOODCASTLE1;
	
	if (iBridgeIndex == 7)
		SumBC = 45;

	if ( BC_MAP_RANGE(iBridgeIndex+SumBC) == FALSE )
	{
		LogAddTD("[Blood Castle] [%s][%s] DropItemDirectly() failed (iBridgeIndex wrong - %d)",
			gObj[iIndex].AccountID, gObj[iIndex].Name, iBridgeIndex);

		return false;
	}

	PMSG_ITEMTHROW_RESULT pResult;

	PHeadSetB((LPBYTE)&pResult, 0x23, sizeof(pResult));
	pResult.Result = TRUE;
	pResult.Ipos = iItemPos;
	int map_num = gObj[iIndex].MapNumber;
	int type = gObj[iIndex].pInventory[iItemPos].m_Type;
	int level = gObj[iIndex].pInventory[iItemPos].m_Level;
	float dur = gObj[iIndex].pInventory[iItemPos].m_Durability;
	BYTE Option1 = gObj[iIndex].pInventory[iItemPos].m_SkillOption;
	BYTE Option2 = gObj[iIndex].pInventory[iItemPos].m_LuckOption;
	BYTE Option3 = gObj[iIndex].pInventory[iItemPos].m_Z28Option;
	BYTE NOption = gObj[iIndex].pInventory[iItemPos].m_NewOption;
	DWORD s_num = gObj[iIndex].pInventory[iItemPos].m_Number;
	BYTE ItemExOption = g_kJewelOfHarmonySystem.GetItemStrengthenOption(&gObj[iIndex].pInventory[iItemPos]);
	BYTE ItemExLevel = g_kJewelOfHarmonySystem.GetItemOptionLevel(&gObj[iIndex].pInventory[iItemPos]);

	BYTE NewOption[MAX_EXOPTION_SIZE];
	::ItemIsBufExOption(NewOption, &gObj[iIndex].pInventory[iItemPos]);
	int PetLevel = gObj[iIndex].pInventory[iItemPos].m_PetItem_Level;
	int PetExp = gObj[iIndex].pInventory[iItemPos].m_PetItem_Exp;
	BYTE SOption = gObj[iIndex].pInventory[iItemPos].m_SetOption;
	BYTE ItemEffectEx = gObj[iIndex].pInventory[iItemPos].m_ItemOptionEx;
	int item_number = gObj[iIndex].pInventory[iItemPos].m_Number;
	char szItemName[50] = "Item";
	int aAntiLootIndex = -1;

	if ( MapC[map_num].ItemDrop(type, level, dur, gObj[iIndex].X, gObj[iIndex].Y,Option1, Option2, Option3, NOption, SOption, item_number, aAntiLootIndex, PetLevel, PetExp, ItemEffectEx,0,0,0,0,0) == TRUE )
	{
		::gObjInventoryDeleteItem(iIndex, iItemPos);
		pResult.Result = TRUE;

		LogAddTD(lMsg.Get(MSGGET(1, 223)), gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].MapNumber,
			gObj[iIndex].X, gObj[iIndex].Y, s_num, szItemName, type, level, Option1, Option2, Option3, (int)dur, NewOption[0],
			NewOption[1], NewOption[2], NewOption[3], NewOption[4], NewOption[5], NewOption[6], SOption,
			ItemEffectEx>>7, ItemExOption, ItemExLevel);
	}
	else
	{
		pResult.Result = FALSE;
	}

	DataSend(iIndex, (UCHAR*)&pResult, pResult.h.size);

	if ( pResult.Result == TRUE )
	{
		if ( iItemPos < INVENTORY_BAG_START )
		{
			if ( iItemPos== 10 || iItemPos == 11 )
			{
				gObjUseSkill.SkillChangeUse(iIndex);
			}

			::gObjMakePreviewCharSet(iIndex);

			PMSG_USEREQUIPMENTCHANGED pMsg;

			PHeadSetB((LPBYTE)&pMsg, 0x25, sizeof(PMSG_USEREQUIPMENTCHANGED));
			pMsg.NumberH = SET_NUMBERH(iIndex);
			pMsg.NumberL = SET_NUMBERL(iIndex);
			ItemByteConvert(pMsg.ItemInfo, gObj[iIndex].pInventory[iItemPos]);
			pMsg.ItemInfo[I_OPTION] = iItemPos * 16; // iItemPos << 16;
			pMsg.ItemInfo[I_OPTION] |= LevelSmallConvert(iIndex, iItemPos) & 0x0F;

			DataSend(iIndex, (UCHAR*)&pMsg, pMsg.h.size);
			MsgSendV2(&gObj[iIndex], (UCHAR*)&pMsg, pMsg.h.size);
		}
	}

	return (pResult.Result);
}





bool CBloodCastle::CheckUserHaveUlimateWeapon(int iIndex)
{
	if ( OBJMAX_RANGE(iIndex ) == FALSE )
	{
		return false;
	}

	if ( gObj[iIndex].Type != OBJ_USER || gObj[iIndex].Connected <= PLAYER_LOGGED )
	{
		return false;
	}

	bool bRetVal = false;

	for ( int x=0;x<ReadConfig.INVENTORY_SIZE(iIndex,false);x++)
	{
		if ( gObj[iIndex].pInventory[x].IsItem() == TRUE )
		{
			if ( gObj[iIndex].pInventory[x].m_Type == ITEMGET(0,19) || gObj[iIndex].pInventory[x].m_Type == ITEMGET(5,10) || gObj[iIndex].pInventory[x].m_Type == ITEMGET(4,18) )
			{
				bRetVal = true;
				break;
			}
		}
	}

	return bRetVal;
}


bool CBloodCastle::CheckWinnerExist(int iBridgeIndex)
{
	if ( !BC_BRIDGE_RANGE(iBridgeIndex))
		return false;

	if ( this->m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX != -1 )
		return true;

	return false;
}



bool CBloodCastle::CheckWinnerValid(int iBridgeIndex)
{
	if ( !BC_BRIDGE_RANGE(iBridgeIndex))
	{
		LogAddTD("[Blood Castle] (%d) CBloodCastle::CheckWinnerValid() - !CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT)",
			iBridgeIndex+1);

		return false;
	}

	if ( this->m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX == -1 )
	{
		LogAddTD("[Blood Castle] (%d) CBloodCastle::CheckWinnerValid() - m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX == -1",
			iBridgeIndex+1);

		return false;
	}

	if ( !gObjIsConnected(this->m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX) )
	{
		LogAddTD("[Blood Castle] (%d) CBloodCastle::CheckWinnerValid() - !gObjIsConnected(m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX)",
			iBridgeIndex+1);

		return false;
	}

	if ( gObj[this->m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX].m_cBloodCastleIndex == -1 || gObj[this->m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX].m_cBloodCastleSubIndex == -1 || gObj[this->m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX].m_cBloodCastleIndex != iBridgeIndex )
	{
		LogAddTD("[Blood Castle] (%d) CBloodCastle::CheckWinnerValid() - (gObj[m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX].m_cBloodCastleIndex == -1) ...",
			iBridgeIndex+1);

		return false;
	}

	if ( !BC_MAP_RANGE(gObj[this->m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX].MapNumber) )
	{
		LogAddTD("[Blood Castle] (%d) CBloodCastle::CheckWinnerValid() - !CHECK_BLOODCASTLE(gObj[m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX].MapNumber)",
			iBridgeIndex+1);

		return false;
	}

	return true;
}
	



bool CBloodCastle::CheckUserWinnerParty(int iBridgeIndex, int iIndex)
{
	if ( !BC_BRIDGE_RANGE(iBridgeIndex))
		return false;

	if ( gObjIsConnected(this->m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX) == FALSE )
		return false;

	if ( gObjIsConnected(iIndex) == FALSE )
		return false;

	int iPartyIndex1 = gObj[iIndex].PartyNumber;
	int iPartyIndex2 = gObj[this->m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX].PartyNumber;

	if ( OBJMAX_RANGE(iPartyIndex1) != FALSE && iPartyIndex1 == iPartyIndex2 )
		return true;

	return false;
}



bool CBloodCastle::CheckPartyExist(int iIndex)
{
	if ( !gObjIsConnected(iIndex))
		return false;

	int iPartyIndex = gObj[iIndex].PartyNumber;
	int iUserIndex;

	if ( !OBJMAX_RANGE(iPartyIndex))
		return false;

	for ( int iPartyUserIndex =0;iPartyUserIndex<MAX_USER_IN_PARTY;iPartyUserIndex++)
	{
		iUserIndex = gParty.m_PartyS[iPartyIndex].Number[iPartyUserIndex];

		if ( gObjIsConnected(iUserIndex))
		{
			if ( BC_MAP_RANGE(gObj[iUserIndex].MapNumber) && BC_BRIDGE_RANGE(gObj[iUserIndex].m_cBloodCastleIndex) )
			{
				if ( gObj[iUserIndex].Live == 1 )
				{
					if ( gObj[iUserIndex].m_bBloodCastleComplete == false )
					{
						return true;
					}
				}
			}
		}
	}

	return false;
}


bool CBloodCastle::CheckWinnerPartyComplete(int iBridgeIndex)
{
	if ( !BC_BRIDGE_RANGE(iBridgeIndex) )
		return false;

	if ( !gObjIsConnected(this->m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX))
		return false;

	int iPartyIndex = gObj[this->m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX].PartyNumber;
	int iUserIndex;

	if ( !OBJMAX_RANGE(iPartyIndex))
		return true;	// #error why true??

	for ( int iPartyUserIndex =0;iPartyUserIndex<MAX_USER_IN_PARTY;iPartyUserIndex++)
	{
		iUserIndex = gParty.m_PartyS[iPartyIndex].Number[iPartyUserIndex];

		if ( gObjIsConnected(iUserIndex))
		{
			if ( BC_MAP_RANGE(gObj[iUserIndex].MapNumber) && BC_BRIDGE_RANGE(gObj[iUserIndex].m_cBloodCastleIndex) )
			{
				if ( gObj[iUserIndex].Live == 1 )
				{
					if ( gObj[iUserIndex].m_bBloodCastleComplete == false )
					{
						return false;
					}
				}
			}
		}
	}

	return true;
}



bool CBloodCastle::SetBridgeWinner(int iBridgeIndex, int iIndex)
{
	if ( !BC_BRIDGE_RANGE(iBridgeIndex) )
		return false;

	if ( gObjIsConnected(this->m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX) != 0 )
		return false;

	if ( !gObjIsConnected(iIndex))
		return false;

	if ( !BC_MAP_RANGE(gObj[iIndex].MapNumber))
		return false;

	this->m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX = iIndex;

	return true;
}



int CBloodCastle::GetWinnerPartyCompleteCount(int iBridgeIndex)
{
	if ( !BC_BRIDGE_RANGE(iBridgeIndex) )
		return false;

	if ( !gObjIsConnected(this->m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX))
		return false;

	int iPartyIndex = gObj[this->m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX].PartyNumber;
	
	if ( !OBJMAX_RANGE(iPartyIndex))
		return false;

	int iPartyComplete=0;
	int iUserIndex;

	for ( int iPartyUserIndex =0;iPartyUserIndex<MAX_USER_IN_PARTY;iPartyUserIndex++)
	{
		iUserIndex = gParty.m_PartyS[iPartyIndex].Number[iPartyUserIndex];

		if ( gObjIsConnected(iUserIndex))
		{
			if ( BC_MAP_RANGE(gObj[iUserIndex].MapNumber) && BC_BRIDGE_RANGE(gObj[iUserIndex].m_cBloodCastleIndex) )
			{
				if ( gObj[iUserIndex].Live == 1 )
				{
					if ( gObj[iUserIndex].m_bBloodCastleComplete == true )
					{
						iPartyComplete++;
					}
				}
			}
		}
	}

	return iPartyComplete;
}



int CBloodCastle::GetWinnerPartyCompletePoint(int iBridgeIndex)
{
	int iPartyComplete = this->GetWinnerPartyCompleteCount(iBridgeIndex);
	iPartyComplete--;

	if ( CHECK_LIMIT(iPartyComplete, MAX_USER_IN_PARTY) )
		return g_iBC_Party_EventPoint[iPartyComplete];

	return 0;
}


void CBloodCastle::ChangeMonsterState(int iBridgeIndex, int iIndex)
{
	if ( !BC_BRIDGE_RANGE(iBridgeIndex) )
		return;

	int iAddDamageMax = 0;
	int iAddDamageMin = 0;
	int iAddDefense = 0;

	if ( this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC < 300000 )
	{
		iAddDamageMin = g_iBC_MONSTER_CHANGE_STATE[1][0];
		iAddDamageMax = g_iBC_MONSTER_CHANGE_STATE[1][1];
		iAddDefense = g_iBC_MONSTER_CHANGE_STATE[1][2];
	}
	else if ( this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC < 600000 )
	{
		iAddDamageMin = g_iBC_MONSTER_CHANGE_STATE[0][0];
		iAddDamageMax = g_iBC_MONSTER_CHANGE_STATE[0][1];
		iAddDefense = g_iBC_MONSTER_CHANGE_STATE[0][2];
	}

	LPMONSTER_ATTRIBUTE lpMA = gMAttr.GetAttr(gObj[iIndex].Class);

	if ( lpMA == NULL )
		return;

	gObj[iIndex].m_AttackDamageMin = lpMA->m_DamageMin + iAddDamageMin;
	gObj[iIndex].m_AttackDamageMax = lpMA->m_DamageMax + iAddDamageMax;
	gObj[iIndex].m_Defense = lpMA->m_Defense + iAddDefense;
}




void CBloodCastle::FixUsersPlayStateWin(int iBridgeIndex)
{
	if ( !BC_BRIDGE_RANGE(iBridgeIndex) )
		return;

	if ( this->m_BridgeData[iBridgeIndex].m_iMISSION_SUCCESS != -1 )
	{
		for ( int i=0;i<MAX_BLOOD_CASTLE_SUB_BRIDGE;i++)
		{
			if ( this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex == -1 )
				continue;

			if ( gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Connected < PLAYER_PLAYING )
				continue;

			if ( gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleIndex == -1 ||
				 gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleSubIndex == -1 ||
				 gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleIndex != iBridgeIndex )
				continue;

			LPOBJ lpObj = &gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex];

			switch ( this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState )
			{
				case 0:
					if ( OBJMAX_RANGE(lpObj->PartyNumber) && lpObj->PartyNumber == gObj[this->m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX].PartyNumber )
					{
						if ( BC_MAP_RANGE(lpObj->MapNumber) && lpObj->Live == TRUE && lpObj->Life > 0.0 )
						{
							this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState = 3;
						}
						else
						{
							this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState = 4;
						}
					}
					else 
					{
						if ( !BC_MAP_RANGE(lpObj->MapNumber) || lpObj->Live == 0 || lpObj->Life <= 0.0 )
						{
							this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState = 1;
						}
					}
					break;
				case 1:
					if ( OBJMAX_RANGE(lpObj->PartyNumber) && lpObj->PartyNumber == gObj[this->m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX].PartyNumber )
					{
						this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState = 4;
					}
					break;
				case 3:
					if ( OBJMAX_RANGE(lpObj->PartyNumber) && lpObj->PartyNumber == gObj[this->m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX].PartyNumber )
					{
						if ( !BC_MAP_RANGE(lpObj->MapNumber) || lpObj->Live == 0 || lpObj->Life <= 0.0 )
						{
							this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState = 4;
						}
					}
					else 
					{
						if ( BC_MAP_RANGE(lpObj->MapNumber) && lpObj->Live == TRUE && lpObj->Life > 0.0 )
						{
							this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState = 0;
						}
						else
						{
							this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState = 1;
						}
					}
					break;
				case 4:
					if ( !OBJMAX_RANGE(lpObj->PartyNumber) || lpObj->PartyNumber != gObj[this->m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX].PartyNumber )
					{
						this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState = 1;
					}
					break;
			}

			LogAddTD("[Blood Castle] (%d) [%s][%s] FixUsersPlayStateWin() - State : %d",
				iBridgeIndex+1, lpObj->AccountID, lpObj->Name, this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState);

		}
	}
}



void CBloodCastle::FixUsersPlayStateFail(int iBridgeIndex)
{
	if ( !BC_BRIDGE_RANGE(iBridgeIndex) )
		return;

	for ( int i=0;i<MAX_BLOOD_CASTLE_SUB_BRIDGE;i++)
	{
		if ( this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex == -1 )
			continue;

		if ( gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Connected < PLAYER_PLAYING )
			continue;

		if ( gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleIndex == -1 ||
			 gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleSubIndex == -1 ||
			 gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleIndex != iBridgeIndex )
			continue;

		LPOBJ lpObj = &gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex];

		switch ( this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState )
		{
			case 3:
				if ( !BC_MAP_RANGE(lpObj->MapNumber) || lpObj->Live == 0 || lpObj->Life <= 0.0 )
				{
					this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState = 1;
				}
				else
				{
					this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState = 0;
				}
				break;
			case 4:
				this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState = 1;
				break;
		}

		LogAddTD("[Blood Castle] (%d) [%s][%s] FixUsersPlayStateFail() - State : %d",
			iBridgeIndex+1, lpObj->AccountID, lpObj->Name,
			this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState);
	}
}

