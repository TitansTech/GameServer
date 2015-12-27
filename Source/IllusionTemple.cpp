#include "stdafx.h"
#include "DevilSquare.h"
#include "GameServer.h"
#include "LogProc.h"
#include "GameMain.h"
#include "TNotice.h"
#include "..\include\readscript.h"
#include "gObjMonster.h"
//#include "GameServerAuth.h"
#include "..\common\winutil.h"
#include "DSProtocol.h"
#include "ObjUseSkill.h"
#include "IllusionTemple.h"
#include "protocol.h"

#include "LogToFile.h"
extern CLogToFile ANTI_HACK_LOG;

CIllusionTemple IllusionTemple;

struct PMSG_KILLPLAYER
{
	PBMSG_HEAD h;	// C1:16
	BYTE NumberH;	// 3
	BYTE NumberL;	// 4
	BYTE ExpH;	// 5
	BYTE ExpL;	// 6
	BYTE DamageH;	// 7
	BYTE DamageL;	// 8
};

BOOL IT_MAP_RANGE(BYTE Map)
{	
	if ((Map < MAP_INDEX_ILLUSIONTEMPLE1) || (Map > MAP_INDEX_ILLUSIONTEMPLE6))
	{
		return FALSE;
	}
	return TRUE;
}

CIllusionTemple::CIllusionTemple()
{
	this->Running = false;
}

void CIllusionTemple::Init( char * FilePath )
{
	char Tmp[512] = {0};

	Enabled 		= GetPrivateProfileInt("Common", "SCFIllusionEnabled", 0, FilePath);
	if(this->Running == false)
	{
		GetPrivateProfileString	("Common","SCFIllusionStartHours","1.0",Tmp,100,FilePath);
		Time_Start = atof(Tmp);
	}

	//MinInsidePlayers		= GetPrivateProfileInt("Common", "SCFIllusionMinStartPlayersOnEachTeam", 0, FilePath);
	AutoWinScoreRequired	= GetPrivateProfileInt("Common", "SCFIllusionRaceToScore", 7, FilePath);

	startSkillPoints		= GetPrivateProfileInt("Common", "SFCStartSkillPointsAwarded", 5, FilePath);
	pointsPerMobKill		= GetPrivateProfileInt("Common", "SFCSkillPointsPerMobKill", 2, FilePath);
	pointsPerPlayerKill		= GetPrivateProfileInt("Common", "SFCSkillPointsPerPlayerKill", 5, FilePath);

	ExtraRankExp			= GetPrivateProfileInt("Common", "SCFIllusionExtraRankExp", 5000, FilePath);

	GetPrivateProfileString	("Common","SCFIllusionMobKillRankExp","0.1",Tmp,100,FilePath);
	MobKillRankExp			= atof(Tmp);
	GetPrivateProfileString	("Common","SCFIllusionPlayerKillRankExp","1.0",Tmp,100,FilePath);
	PlayerKillRankExp		= atof(Tmp);
	GetPrivateProfileString	("Common","SCFIllusionScoreBallRankExp","5.0",Tmp,100,FilePath);
	ScoreBallRankExp		= atof(Tmp);
	GetPrivateProfileString	("Common","SCFIllusionPickUpBallRankExp","0.25",Tmp,100,FilePath);
	PickUpBallRankExp		= atof(Tmp);
	GetPrivateProfileString	("Common","SCFIllusionStatueBreakRankExp","0.5",Tmp,100,FilePath);
	StatueBreakRankExp		= atof(Tmp);

	BoxItemDropRate			= GetPrivateProfileInt("Common", "SCFIllusionBoxItemDropRate", 80, FilePath);

	CheckTwoPlayerReq		= GetPrivateProfileInt("Common", "SCFIllusionCheckTwoPlayerReq", 0, FilePath);

	ExpForBallDropped		= GetPrivateProfileInt("Common", "SCFIllusionGetExpForEachDroppedBall", 5000, FilePath);
	ExpForKill				= GetPrivateProfileInt("Common", "SCFIllusionIsGetExpForKillOtherTeamMembers", 1, FilePath);

	IllusionStartCountdown	= GetPrivateProfileInt("Common", "SCFIllusionStartCountdownMin", 5, FilePath);
	IllusionSyncHrMin		= GetPrivateProfileInt("Common", "SCFIllusionSyncHrMin", 30, FilePath);

	IllusionEventMinLength	= GetPrivateProfileInt("Common", "SCFIllusionTempleEventMinLength", 15, FilePath);

	Reward_ItemType[0]		= GetPrivateProfileInt("Reward", "SCFIllusionRewardItemType", 12, FilePath);
	Reward_ItemIndex[0]		= GetPrivateProfileInt("Reward", "SCFIllusionRewardItemIndex", 15, FilePath);
	Reward_Itemlvl[0]		= GetPrivateProfileInt("Reward", "SCFIllusionRewardItemLevel", 0, FilePath);
	Reward_ItemOpt[0]		= GetPrivateProfileInt("Reward", "SCFIllusionRewardItemOpt", 0, FilePath);
	Reward_ItemLuck[0]		= GetPrivateProfileInt("Reward", "SCFIllusionRewardItemLuck", 0, FilePath);
	Reward_ItemSkill[0]		= GetPrivateProfileInt("Reward", "SCFIllusionRewardItemSkill", 0, FilePath);
	Reward_ItemDur[0]		= GetPrivateProfileInt("Reward", "SCFIllusionRewardItemDur", 255, FilePath);
	Reward_ItemIsExc[0]		= GetPrivateProfileInt("Reward", "SCFIllusionRewardItemIsExc", 0, FilePath);

	IsExtraReward					 = GetPrivateProfileInt("ExtraReward", "SCFIllusionIsExtraReward", 1, FilePath);
	IsExtraRewardMinPlayerReq		 = GetPrivateProfileInt("ExtraReward", "SCFIllusionExtraRewardMinPlayerReq", 4, FilePath);

	Reward_ItemType[1]	 = GetPrivateProfileInt("ExtraReward", "SCFIllusion_1Rank_RewardItemType", 12, FilePath);
	Reward_ItemIndex[1]  = GetPrivateProfileInt("ExtraReward", "SCFIllusion_1Rank_RewardItemIndex", 15, FilePath);
	Reward_Itemlvl[1]	 = GetPrivateProfileInt("ExtraReward", "SCFIllusion_1Rank_RewardItemLevel", 0, FilePath);
	Reward_ItemOpt[1]	 = GetPrivateProfileInt("ExtraReward", "SCFIllusion_1Rank_RewardItemOpt", 0, FilePath);
	Reward_ItemLuck[1]	 = GetPrivateProfileInt("ExtraReward", "SCFIllusion_1Rank_RewardItemLuck", 0, FilePath);
	Reward_ItemSkill[1]	 = GetPrivateProfileInt("ExtraReward", "SCFIllusion_1Rank_RewardItemSkill", 0, FilePath);
	Reward_ItemDur[1]	 = GetPrivateProfileInt("ExtraReward", "SCFIllusion_1Rank_RewardItemDur", 255, FilePath);
	Reward_ItemIsExc[1]	 = GetPrivateProfileInt("ExtraReward", "SCFIllusion_1Rank_RewardItemIsExc", 0, FilePath);

	Reward_ItemType[2]	 = GetPrivateProfileInt("ExtraReward", "SCFIllusion_2Rank_RewardItemType", 12, FilePath);
	Reward_ItemIndex[2]  = GetPrivateProfileInt("ExtraReward", "SCFIllusion_2Rank_RewardItemIndex", 15, FilePath);
	Reward_Itemlvl[2]	 = GetPrivateProfileInt("ExtraReward", "SCFIllusion_2Rank_RewardItemLevel", 0, FilePath);
	Reward_ItemOpt[2]	 = GetPrivateProfileInt("ExtraReward", "SCFIllusion_2Rank_RewardItemOpt", 0, FilePath);
	Reward_ItemLuck[2]	 = GetPrivateProfileInt("ExtraReward", "SCFIllusion_2Rank_RewardItemLuck", 0, FilePath);
	Reward_ItemSkill[2]	 = GetPrivateProfileInt("ExtraReward", "SCFIllusion_2Rank_RewardItemSkill", 0, FilePath);
	Reward_ItemDur[2]	 = GetPrivateProfileInt("ExtraReward", "SCFIllusion_2Rank_RewardItemDur", 255, FilePath);
	Reward_ItemIsExc[2]	 = GetPrivateProfileInt("ExtraReward", "SCFIllusion_2Rank_RewardItemIsExc", 0, FilePath);

	Reward_ItemType[3]	 = GetPrivateProfileInt("ExtraReward", "SCFIllusion_3Rank_RewardItemType", 12, FilePath);
	Reward_ItemIndex[3]  = GetPrivateProfileInt("ExtraReward", "SCFIllusion_3Rank_RewardItemIndex", 15, FilePath);
	Reward_Itemlvl[3]	 = GetPrivateProfileInt("ExtraReward", "SCFIllusion_3Rank_RewardItemLevel", 0, FilePath);
	Reward_ItemOpt[3]	 = GetPrivateProfileInt("ExtraReward", "SCFIllusion_3Rank_RewardItemOpt", 0, FilePath);
	Reward_ItemLuck[3]	 = GetPrivateProfileInt("ExtraReward", "SCFIllusion_3Rank_RewardItemLuck", 0, FilePath);
	Reward_ItemSkill[3]	 = GetPrivateProfileInt("ExtraReward", "SCFIllusion_3Rank_RewardItemSkill", 0, FilePath);
	Reward_ItemDur[3]	 = GetPrivateProfileInt("ExtraReward", "SCFIllusion_3Rank_RewardItemDur", 255, FilePath);
	Reward_ItemIsExc[3]	 = GetPrivateProfileInt("ExtraReward", "SCFIllusion_3Rank_RewardItemIsExc", 0, FilePath);

	IsFenrirChange = GetPrivateProfileInt("GoldenFenir", "SCFIsChangeFenrirToGold", 0, FilePath);
	FenrirChangeMinPlayerReq = GetPrivateProfileInt("GoldenFenir", "SCFFenrirToGoldMinPlayerReq", 4, FilePath);
	RedFenrirPercent = GetPrivateProfileInt("GoldenFenir", "SCFRedFenrirPercent", 100, FilePath);
	BlueFenrirPercent = GetPrivateProfileInt("GoldenFenir", "SCFBlueFenrirPercent", 100, FilePath);
	BlackFenrirPercent = GetPrivateProfileInt("GoldenFenir", "SCFBlackFenrirPercent", 100, FilePath);

	if(this->Running == false)
	{
		Time_Left=0;
		Time_Current = 0;

		if (Time_Start>24) 
		{
			Time_Start=24;
		}
		//Transfer to Seconds from hours
		Time_Start	= Time_Start * 60 * 60;

		if (Time_Start < IllusionEventMinLength*60 + 10*60)
			Time_Start = IllusionEventMinLength*60 + 10*60;

		IllusionCurrentCountdown = IllusionStartCountdown;

		//Clean Event
		if (Enabled == 1)
		{
			for(int j=0;j<MAX_ILLUSION_TEMPLE_LEVEL;j++)
			{
				SetState_Closed(j,1);
			}
			this->Running = true;
			_beginthread( CIllusionTemple__Time, 0, NULL  );
		}
	}
}

void CIllusionTemple::CheckSync(BYTE Level)
{
	tm * today;
	time_t ltime;

	time(&ltime);
	today = localtime(&ltime);

	if ( today->tm_min*60 + today->tm_sec < (IllusionSyncHrMin*60) )
	{
		Time_Current = Time_Start - ((IllusionSyncHrMin*60) - today->tm_min*60 + today->tm_sec);

		if ( (Time_Start - Time_Current)/60 < IllusionStartCountdown )
		{
			SetState_Closed(Level,0);
			IllusionCurrentCountdown = (Time_Start - Time_Current)/60;
		}

	} else {
		int nextTime = (IllusionSyncHrMin*60) + Time_Start;
		if (nextTime < today->tm_min*60 + today->tm_sec)
			nextTime = nextTime + Time_Start;

		int minOver = today->tm_min*60 + today->tm_sec + Time_Start;

		Time_Current = minOver-nextTime;

		if ( (Time_Start - Time_Current)/60 < IllusionStartCountdown )
		{
			SetState_Closed(Level,0);
			IllusionCurrentCountdown = (Time_Start - Time_Current)/60;
		}
	}

	LogAddTD("[Illusion Temple](%d) Sync Open Time. [%d] min remain. Notice: [%d] min",
		Level+1,(int)((Time_Start-Time_Current)/60),IllusionCurrentCountdown
	);
}

int CIllusionTemple::ReqRemainTime()
{
	int leftMinutes = (IllusionTemple.Time_Left/60)-4;
	
	if (leftMinutes <= 0) 
		leftMinutes = 0;

	return leftMinutes;
}

void CIllusionTemple::SelectHero(LPOBJ lpObj,int Map)
{
	int Level = -1;
	
	if (IT_MAP_RANGE(Map) == TRUE)
	{
		Level = Map - MAP_INDEX_ILLUSIONTEMPLE1;
	}

	if (Level >= 0)
	{
		if (IllusionTemple.Room[Level].Start == 1)
		{
			char sBuf[255]={0};

			if(IllusionTemple.Room[Level].Hero == -1 && IllusionTemple.Room[Level].DroppedBall != 1)
			{
				IllusionTemple.Room[Level].DroppedBall = 1;

				int result = IllusionTemple.Room[Level].StatueNum;
				IllusionTemple.StatueBreak(Level, lpObj->m_Index, gObj[result].Y);

				gObj[result].m_ViewSkillState |= 0x200000;	//ViewSkillState 2097152
				//GCSkillInfoSend(&gObj[result], 1, eVS_INVISIBLE);
					
				ItemSerialCreateSend(lpObj->m_Index, MAP_INDEX_ILLUSIONTEMPLE1+Level, gObj[result].X, gObj[result].Y, ITEMGET(14,64), 0, 255, 0, 0, 0, -1, 0, 0);
			
				LogAddTD("[Illusion Temple](%d) [%s][%s] TROW New Fireball on NPC Talk (%d,%d,%d)",
					Level+1,
					lpObj->AccountID, lpObj->Name,
					lpObj->MapNumber, lpObj->X, lpObj->Y
				);

				int pIndex = Player_GetIndex(Level,lpObj->m_Index);
				if (pIndex != -1)
				{
					Room[Level].Player[pIndex].total_score += StatueBreakRankExp;
				}
				this->SetMonster(Level);
					
				/*
				BYTE TempInventoryMap[64];
				memcpy(TempInventoryMap,lpObj->pInventoryMap,64);
				
				if(gObjTempInventoryInsertItem(lpObj,ITEMGET(14,64),TempInventoryMap) == 255)
				{
					LogAddTD("[Illusion Temple](%d) [%s][%s] Cant Register, no space in inventory!",Level+1,lpObj->AccountID,lpObj->Name);
					GCServerMsgStringSend("Cant Register, no space in inventory!",lpObj->m_Index, 0x01);
				} else {
					int result = IllusionTemple.Room[Level].StatueNum;
					gObj[result].m_ViewSkillState |= 0x200000;	//ViewSkillState 2097152
					GCSkillInfoSend(&gObj[result], 1, eVS_INVISIBLE);

					ItemSerialCreateSend(lpObj->m_Index, 235, 0, 0, ITEMGET(14,64), 0, 255, 0, 0, 0, -1, 0, 0);
					LogAddTD("[Illusion Temple](%d) [%s] Took the spark",Level+1,lpObj->Name);
					wsprintf(sBuf,lMsg.Get(MSGGET(14, 14)),lpObj->Name);
					GCServerMsgStringSendToRoom(Level,sBuf,0);
					IllusionTemple.Room[Level].Hero = lpObj->m_Index;

					int pIndex = Player_GetIndex(Level,lpObj->m_Index);
					if (pIndex != -1)
					{
						Room[Level].Player[pIndex].total_score += StatueBreakRankExp;
					}
				}*/
			}else
			{
				if(IllusionTemple.Room[Level].Hero != -1)
				{
					if (lpObj->m_Index != IllusionTemple.Room[Level].Hero)
					{
						LogAddTD("[Illusion Temple](%d) Someone took the gift before",Level+1);
						GCServerMsgStringSend(lMsg.Get(MSGGET(14, 13)),lpObj->m_Index , 0x01);
					}
				}
			}
		}else
		{
			LogAddTD("[Illusion Temple][Bug Tracer](%d) Event is not started", Level+1);
		}
	}else
	{
		LogAddTD("[Illusion Temple][Bug Tracer] (Map:%d) It's not Illusion Temple map", Map);
	}
}

void CIllusionTemple::GCServerMsgStringSendToRoom(BYTE Level, LPSTR szMsg, BYTE type) 
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
				if (gObj[n].MapNumber >= (MAP_INDEX_ILLUSIONTEMPLE1 + Level) &&
					gObj[n].MapNumber < MAP_INDEX_ILLUSIONTEMPLE1 + MAX_ILLUSION_TEMPLE_LEVEL)
					DataSend(n, (UCHAR*)&pNotice, pNotice.h.size);
			}
		}
	}
}

void CIllusionTemple::GCServerMsgStringSendToNoIllusion(LPSTR  szMsg, BYTE type) 
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
			if (( gObj[n].Type  == OBJ_USER ) && ( IT_MAP_RANGE(gObj[n].MapNumber) == FALSE))
			{
				DataSend(n, (UCHAR*)&pNotice, pNotice.h.size);
			}
		}
	}
}

void CIllusionTemple::DataSendTeamRoom(BYTE Level, BYTE Team, LPBYTE pMsg, int size)
{
	for ( int i=0;i<(ILLUSIONTEMPLE_MAX_PLAYERS*2);i++)
	{
		if(Room[Level].Player[i].Team == Team)
		{
			int PlayerID = Room[Level].Player[i].Index;
			if(PlayerID > 0)
			{
				if ( gObj[PlayerID].Connected == PLAYER_PLAYING )
				{
					if ( gObj[PlayerID].Type == OBJ_USER )
					{
						if (gObj[PlayerID].MapNumber >= (MAP_INDEX_ILLUSIONTEMPLE1 + Level) &&
							gObj[PlayerID].MapNumber < MAP_INDEX_ILLUSIONTEMPLE1 + MAX_ILLUSION_TEMPLE_LEVEL)
						{
							DataSend(PlayerID, pMsg, size);
						}
						else
							Player_Remove(Level,i);
					}else
					{
						Player_Remove(Level,i);
					}
				}else
				{
					Player_Remove(Level,i);
				}
			}
		}
	}
}

void CIllusionTemple::DataSendRoom(BYTE Level, LPBYTE pMsg, int size)
{
	for ( int i=0;i<(ILLUSIONTEMPLE_MAX_PLAYERS*2);i++)
	{
		int PlayerID = Room[Level].Player[i].Index;		
		if(PlayerID > 0)
		{
			if ( gObj[PlayerID].Connected == PLAYER_PLAYING )
			{
				if ( gObj[PlayerID].Type == OBJ_USER )
				{
					if (gObj[PlayerID].MapNumber >= (MAP_INDEX_ILLUSIONTEMPLE1 + Level) &&
						gObj[PlayerID].MapNumber < MAP_INDEX_ILLUSIONTEMPLE1 + MAX_ILLUSION_TEMPLE_LEVEL)
					{
						DataSend(PlayerID, pMsg, size);
					}
					else
						Player_Remove(Level,i);
				}else
				{
					Player_Remove(Level,i);
				}
			}else
			{
				Player_Remove(Level,i);
			}
		}
	}
}


void CIllusionTemple::StatueBreak(BYTE Level, short pIndex, BYTE y)
{
	if(OBJMAX_RANGE(pIndex) == 0)
	{
		return;
	}

	int TargetIndex = IllusionTemple.Room[Level].StatueNum;

	PMSG_ILLUSION_DIESTATUE pDieMsg;

	PHeadSetB((LPBYTE)&pDieMsg, 0x17, sizeof(pDieMsg));
	pDieMsg.NumberH = SET_NUMBERH(TargetIndex);
	pDieMsg.NumberL = SET_NUMBERL(TargetIndex);
	pDieMsg.Skill = 0x00;
	pDieMsg.KillerNumberH = SET_NUMBERH(pIndex);
	pDieMsg.KillerNumberL = SET_NUMBERL(pIndex);

	MsgSendDistNear(&gObj[pIndex], (UCHAR*)&pDieMsg, pDieMsg.h.size, 16);

	/*PMSG_ILLUSION_STATUE_BREAK pMsg;

	pMsg.h.c = 0xc1;
	pMsg.h.size = 0x10;
	pMsg.h.headcode = 0xbf;
	pMsg.h.subcode = 0x08;

	pMsg.aIndex = pIndex;

	if (y > 100)
	{
		pMsg.unk1 = 0x31;
		pMsg.unk2 = 0x31;
		pMsg.unk3 = 0x31;
		pMsg.unk4 = 0x31;
		pMsg.unk5 = 0x31;
		pMsg.unk6 = 0x31;
		pMsg.unk7 = 0;
		pMsg.unk8 = 0;
		pMsg.unk9 = 0;
		pMsg.unk10 = 0;
	} else {
		pMsg.unk1 = 0x44;
		pMsg.unk2 = 0x61;
		pMsg.unk3 = 0x52;
		pMsg.unk4 = 0x4b;
		pMsg.unk5 = 0x61;
		pMsg.unk6 = 0x76;
		pMsg.unk7 = 0;
		pMsg.unk8 = 0;
		pMsg.unk9 = 0;
		pMsg.unk10 = 0;
	}
	MsgSendDistNear(&gObj[pIndex], (UCHAR*)&pMsg, pMsg.h.size, 16);
	*/
}

void CIllusionTemple::StatueTeleport(BYTE Level)
{
	int direction=0;
	int result = IllusionTemple.Room[Level].StatueNum;
	//gObjViewportListProtocolDestroy(&gObj[result]);
	//gObjClearViewport(&gObj[result]);

	BYTE Rand = rand()%100;
	if(Rand <= 40)
	{
		IllusionTemple.Room[Level].Statue_X = 207;
		IllusionTemple.Room[Level].Statue_Y = 47;
		direction = 1;
	}else
	{
		IllusionTemple.Room[Level].Statue_X = 134;
		IllusionTemple.Room[Level].Statue_Y = 121;
		direction = 1;
	}

	gObj[result].X = IllusionTemple.Room[Level].Statue_X;
	gObj[result].Y = IllusionTemple.Room[Level].Statue_Y;
	gObj[result].TX = IllusionTemple.Room[Level].Statue_X;
	gObj[result].TY = IllusionTemple.Room[Level].Statue_Y;
	gObj[result].MTX = IllusionTemple.Room[Level].Statue_X;
	gObj[result].MTY = IllusionTemple.Room[Level].Statue_Y;
	gObj[result].m_OldX = IllusionTemple.Room[Level].Statue_X;
	gObj[result].m_OldY = IllusionTemple.Room[Level].Statue_Y;
	gObj[result].StartX = IllusionTemple.Room[Level].Statue_X;
	gObj[result].StartY = IllusionTemple.Room[Level].Statue_Y;
	gObj[result].Dir = direction;
	gObj[result].PathCount = 0;

	//Remove Invisibility
	gObj[result].m_ViewSkillState &= ~0x200000;	//ViewSkillState 2097152
	GCSkillInfoSend(&gObj[result], 0, eVS_INVISIBLE);

	IllusionTemple.Room[Level].Hero = -1;
	IllusionTemple.Room[Level].Hero_X = 0;
	IllusionTemple.Room[Level].Hero_Y = 0;

	IllusionTemple.Room[Level].DroppedBall = 0;
}

void CIllusionTemple__Time(void * lpParam)
{
	while(true)
	{
		Sleep(1000);
		char sBuf[255] = {0};
		IllusionTemple.Time_Left = IllusionTemple.Time_Start - IllusionTemple.Time_Current;
	   
		if(IllusionTemple.Time_Current == IllusionTemple.Time_Start) 
		{
			//Reset unused variables
			IllusionTemple.Time_Current = 0;

			//Give Notice = Illusion Event Closed!
			LogAddTD("[Illusion Temple] Closed, Enter When Availiable!");
			AllSendServerMsg(lMsg.Get(MSGGET(14, 11)));

			//Statue Positioning
			for (int Level=0;Level<MAX_ILLUSION_TEMPLE_LEVEL;Level++)
			{
				IllusionTemple.ClearMonster(Level);
				IllusionTemple.StatueTeleport(Level);
			}

			IllusionTemple.IllusionCurrentCountdown = IllusionTemple.IllusionStartCountdown;

			for (int Level=0;Level<MAX_ILLUSION_TEMPLE_LEVEL;Level++)
			{
				IllusionTemple.Warp_EventRoom(true,Level);
			}

			//Start Illusion Event
			IllusionTemple.Start();
		}
		else if ( (IllusionTemple.Time_Current == (IllusionTemple.Time_Start - (IllusionTemple.IllusionCurrentCountdown*60))) )
		{
			//Make sure event is clean
			if(IllusionTemple.IllusionCurrentCountdown == IllusionTemple.IllusionStartCountdown)
			{
				for(int j=0;j<MAX_ILLUSION_TEMPLE_LEVEL;j++)
				{
					IllusionTemple.SetState_Closed(j,0);
				}
			}

			wsprintf(sBuf, lMsg.Get(MSGGET(14, 10)),IllusionTemple.IllusionCurrentCountdown);
			LogAddTD("[Illusion Temple] Will Start In %d Minutes",IllusionTemple.IllusionCurrentCountdown);
			AllSendServerMsg(sBuf);

			IllusionTemple.Time_Current++;
			IllusionTemple.IllusionCurrentCountdown--;
		} 
		else
		{
			IllusionTemple.Time_Current++;
		}
	}
	_endthread();
}

int CIllusionTemple::CheckTeamsSize(BYTE Level)
{
	//Check if enough player on each team side
	if((IllusionTemple.Room[Level].TeamBlue_Count == 0 ||
		IllusionTemple.Room[Level].TeamOrange_Count == 0) &&
		IllusionTemple.Room[Level].TeamOrange_Count+IllusionTemple.Room[Level].TeamBlue_Count > 0
		)
	{
		BYTE teamTransferSides = 0;
		
		if (IllusionTemple.Room[Level].TeamBlue_Count > 1)
		{
			teamTransferSides = IllusionTemple.Room[Level].TeamBlue_Count/2;

			for (int i=0; i < (ILLUSIONTEMPLE_MAX_PLAYERS*2); i++)
			{
				if (teamTransferSides > 0)
				{
					if (Room[Level].Player[i].Index > 0)
					{
						if(Room[Level].Player[i].Team == ILLUSIONTEMPLE_TEAM_BLUE)
						{
							LogAddTD("[Illusion Temple](%d) Transfer team BLUE to team ORANGE [%s][%s][%d]",
								Level+1,
								gObj[Room[Level].Player[i].Index].AccountID, gObj[Room[Level].Player[i].Index].Name,
								teamTransferSides
							);

							Room[Level].Player[i].Team = ILLUSIONTEMPLE_TEAM_ORANGE;
							teamTransferSides -= 1;

							IllusionTemple.Room[Level].TeamBlue_Count -= 1;
							IllusionTemple.Room[Level].TeamOrange_Count += 1;
						}
					}
				} else {
					break;
				}
			}
		} else if (IllusionTemple.Room[Level].TeamOrange_Count > 1) {
			teamTransferSides = IllusionTemple.Room[Level].TeamOrange_Count/2;

			for (int i=0; i < (ILLUSIONTEMPLE_MAX_PLAYERS*2); i++)
			{
				if (teamTransferSides > 0)
				{
					if (Room[Level].Player[i].Index > 0)
					{
						if(Room[Level].Player[i].Team == ILLUSIONTEMPLE_TEAM_ORANGE)
						{
							LogAddTD("[Illusion Temple](%d) Transfer team ORANGE to team BLUE [%s][%s][%d]",
								Level+1,
								gObj[Room[Level].Player[i].Index].AccountID, gObj[Room[Level].Player[i].Index].Name,
								teamTransferSides
							);

							Room[Level].Player[i].Team = ILLUSIONTEMPLE_TEAM_BLUE;
							teamTransferSides -= 1;

							IllusionTemple.Room[Level].TeamBlue_Count += 1;
							IllusionTemple.Room[Level].TeamOrange_Count -= 1;
						}
					}
				} else {
					break;
				}
			}
		}

		if (IllusionTemple.Room[Level].TeamBlue_Count + IllusionTemple.Room[Level].TeamOrange_Count <= 1)
			return -1;
	}

	if (IllusionTemple.Room[Level].TeamBlue_Count == 0 && IllusionTemple.Room[Level].TeamOrange_Count == 0)
		return 0;

	return 1;
}

void CIllusionTemple::Warp_EventRoom(bool Start, BYTE Level)
{
	int Map=0;
	int BlueX=196;
	int BlueY=126;
	int OrangeX=140;
	int OrangeY=47;
	
	int BluePartyIndex = -1;
	int BluePartyOwner = -1;

	int OrangePartyIndex = -1;
	int OrangePartyOwner = -1;

	Map=MAP_INDEX_ILLUSIONTEMPLE1+Level;

	if(Start)
	{
		int isSizedTeam = this->CheckTeamsSize(Level);
		if (CheckTwoPlayerReq == 1)
		{
			if (isSizedTeam == -1)
			{
				GCServerMsgStringSendToRoom(Level,"[Illusion Temple] FAILED! Lack of players",0);
				this->SetState_End(Level, true);
				return;
			}
		}

		if (isSizedTeam == 0)
		{
			SetState_Closed(Level,1);
			return;
		}

		LogAddTD("[Illusion Temple](%d) Orange Team: %d VS Blue Team: %d",
			Level+1,
			Room[Level].TeamOrange_Count,Room[Level].TeamBlue_Count
		);
	}

	for(int i=0;i < (ILLUSIONTEMPLE_MAX_PLAYERS*2); i++)
	{
		if (Room[Level].Player[i].Index > 0)
		{
			int pIndex = Room[Level].Player[i].Index;

			if(gObj[pIndex].MapNumber == Map)
			{
				BYTE x = rand()%1 + rand()%1 + rand()%2;
				BYTE y = rand()%2 + rand()%1 + rand()%2;

				if(Room[Level].Player[i].Team == ILLUSIONTEMPLE_TEAM_BLUE)
				{
					gObjTeleport(pIndex,Map,BlueX+x,BlueY+y);

					if(Start)
					{
						if (BluePartyIndex == -1)
						{
							BluePartyIndex = gParty.Create(pIndex, gObj[pIndex].DBNumber, gObj[pIndex].Level);
							gObj[pIndex].PartyNumber = BluePartyIndex;
							BluePartyOwner = pIndex;
						}

						if (BluePartyIndex >= 0 && BluePartyOwner != pIndex && BluePartyIndex != -1)
						{
							int iPartyPos = gParty.Add(gObj[BluePartyOwner].PartyNumber, pIndex, gObj[pIndex].DBNumber, gObj[pIndex].Level);

							gObj[pIndex].PartyNumber = BluePartyIndex;
							gParty.Paint(BluePartyIndex);
						}
							
						if (BluePartyIndex >= 0)
						{
							CGPartyListAll(BluePartyIndex);
						}
						
						LogAddTD("[Illusion Temple][BLUE](%d) Party Member: %s(%d) PartyID: %d(Owner:%d)",
							Level+1,
							gObj[pIndex].Name, pIndex,
							gObj[pIndex].PartyNumber, BluePartyOwner
						);
					}

					if(Start)
					{
						gObj[pIndex].m_Change = 405;
						gObjViewportListProtocolCreate(&gObj[pIndex]);
					}
				}else if(Room[Level].Player[i].Team == ILLUSIONTEMPLE_TEAM_ORANGE)
				{
					gObjTeleport(pIndex,Map,OrangeX+x,OrangeY+y);

					if(Start)
					{
						if (OrangePartyIndex == -1)
						{
							OrangePartyIndex = gParty.Create(pIndex, gObj[pIndex].DBNumber, gObj[pIndex].Level);
							gObj[pIndex].PartyNumber = OrangePartyIndex;
							OrangePartyOwner = pIndex;
						}

						if (OrangePartyIndex >= 0 && BluePartyOwner != pIndex && OrangePartyIndex != -1)
						{
							int iPartyPos = gParty.Add(gObj[OrangePartyOwner].PartyNumber, pIndex, gObj[pIndex].DBNumber, gObj[pIndex].Level);

							gObj[pIndex].PartyNumber = OrangePartyIndex;
							gParty.Paint(OrangePartyIndex);
						}
							
						if (OrangePartyIndex >= 0)
						{
							CGPartyListAll(OrangePartyIndex);
						}

						LogAddTD("[Illusion Temple][ORANGE](%d) Party Member: %s(%d) PartyID: %d(Owner:%d)",
							Level+1,
							gObj[pIndex].Name, pIndex,
							gObj[pIndex].PartyNumber, OrangePartyOwner
						);
					}

					if(Start)
					{
						gObj[pIndex].m_Change = 404;
						gObjViewportListProtocolCreate(&gObj[pIndex]);
					}
				}else
				{
					//TEAM ERROR: EMPTY PLAYER						
					Player_Remove(Level,i);
				}
			} else
			{
				//PLAYER IN NOT EVENT AREA: EMPTY PLAYER						
				Player_Remove(Level,i);
			}
		}
	}
}

void CIllusionTemple__TimeInside(void * lpParam)
{
	for (int Level=0;Level<MAX_ILLUSION_TEMPLE_LEVEL;Level++)
	{
		if (IllusionTemple.Room[Level].Start == 1)
		{	
			LogAddTD("[Illusion Temple](%d) SetState PLAYING",Level+1);
		}else
		{
			LogAddTD("[Illusion Temple](%d) SetState CLOSED",Level+1);
		}
	}
	while(true)
	{
		Sleep(1000);
		int RoomCount = 0;

		if(IllusionTemple.Enabled == 1) 
		{
			for (int Level=0;Level<MAX_ILLUSION_TEMPLE_LEVEL;Level++)
			{
				if (IllusionTemple.Room[Level].Start == 1)
				{	
					RoomCount++;

					if (IllusionTemple.Room[Level].TeamBlue_Score >= IllusionTemple.AutoWinScoreRequired ||
						IllusionTemple.Room[Level].TeamOrange_Score >= IllusionTemple.AutoWinScoreRequired)
					{
						IllusionTemple.Time_Inside_Current[Level] = -1;
					}

					//Check if enough player on each team side
					if (IllusionTemple.CheckTwoPlayerReq == 1)
					{
						if (IllusionTemple.Room[Level].TeamBlue_Count == 0 ||
							IllusionTemple.Room[Level].TeamOrange_Count == 0)
						{
							IllusionTemple.Time_Inside_Current[Level] = -1;
						}
					}

					if(IllusionTemple.Time_Inside_Current[Level] >=0)
					{
						IllusionTemple.SendTimer(Level,IllusionTemple.Time_Inside_Current[Level]);
						//Player Check Status
						IllusionTemple.Player_Check(Level);
					}

					if(IllusionTemple.Time_Inside_Current[Level] == -1)
					{	
						IllusionTemple.ClearMonster(Level);
						IllusionTemple.Warp_EventRoom(false,Level);
						IllusionTemple.GiveReward_Win(Level);
					}

					IllusionTemple.Time_Inside_Current[Level]--; 
					IllusionTemple.Time_Inside_Passed[Level]++;
				}
			}

			//Kill Illusion timer if they are all finished
			if (RoomCount == 0)
			{
				LogAddTD("[Illusion Temple] SetState CLOSED");
				_endthread();
				return;
			}
		}
	}
	_endthread();
}

void CIllusionTemple::Warp_WaitRoom(LPOBJ lpObj, BYTE Level)
{
	if ( lpObj->PartyNumber >= 0 )
	{
		PMSG_PARTYDELUSER pMsg;
		int pindex = gParty.GetIndex(lpObj->PartyNumber, lpObj->m_Index, lpObj->DBNumber);
		if ( pindex >= 0 )
		{
			pMsg.Number = pindex;
			CGPartyDelUser(&pMsg, lpObj->m_Index);
		}
	}

	int Count = Room[Level].TeamBlue_Count + Room[Level].TeamOrange_Count;
	int Map = MAP_INDEX_ILLUSIONTEMPLE1 + Level;

	LogAddTD("[Illusion Temple](%d) [%s] Wait Room Warp Requested ",Level+1,lpObj->Name);

	BYTE x = rand()%3 + rand()%2 + rand()%2;
	BYTE y = rand()%2 + rand()%2 + rand()%2;

	gObjTeleport(lpObj->m_Index, Map, 100+x, 129+y);
		
	Room[Level].Player[Count].Index = lpObj->m_Index;
	Room[Level].Player[Count].Name = lpObj->Name;
	Room[Level].Player[Count].Exp_Current = 0;

	BYTE starter = 0;

	if (Room[Level].TeamBlue_Count + Room[Level].TeamOrange_Count == 0)
	{
		short rndTeamRate = rand()%10000;

		if (rndTeamRate < 4000)
		{
			starter = 1;
		} else {
			starter = 0;
		}
	}

	if (Room[Level].TeamBlue_Count + starter <= Room[Level].TeamOrange_Count)
	{
		LogAddTD("[Illusion Temple](%d) [%s] Join In Team: Blue", Level+1, lpObj->Name);
		Room[Level].Player[Count].Team = ILLUSIONTEMPLE_TEAM_BLUE;
		Room[Level].TeamBlue_Count++;

	}else
	{
		LogAddTD("[Illusion Temple](%d) [%s] Join In Team: Orange",Level+1, lpObj->Name);
		Room[Level].Player[Count].Team = ILLUSIONTEMPLE_TEAM_ORANGE;
		Room[Level].TeamOrange_Count++;
	}
}

void CIllusionTemple::FenrirChange(LPOBJ lpObj, BYTE Level, int FenrirTimer)
{
	if(this->IsFenrirChange == 1)
	{
		int Count = this->Room[Level].TeamOrange_Count + this->Room[Level].TeamBlue_Count;
		if(Count >= this->FenrirChangeMinPlayerReq)
		{
			if(gObjIsConnected(lpObj->m_Index))
			{
				if(lpObj->pInventory[8].IsItem())
				{
					if(lpObj->pInventory[8].m_Type == 0x1A25)
					{
						BYTE Percent = 0;
						if(lpObj->pInventory[8].m_NewOption == 1) //Black
						{
							Percent = this->BlackFenrirPercent;
						}else if(lpObj->pInventory[8].m_NewOption == 2) //Blue
						{
							Percent = this->BlueFenrirPercent;
						}else if(lpObj->pInventory[8].m_NewOption == 0) //Red
						{
							Percent = this->RedFenrirPercent;
						}

						if(FenrirTimer < (IllusionTemple.Time_Inside_Passed[Level]/2))
						{
							GCServerMsgStringSend("To upgrade to Gold Fenrir:",lpObj->m_Index, 0x01);
							GCServerMsgStringSend("Fenrir must be used for most of the event!",lpObj->m_Index, 0x01);
							GCServerMsgStringSend("Upgrade to Gold Fenrir Cancelled!",lpObj->m_Index, 0x01);
						} else if(Percent > 0) {
							
							BYTE Rand = rand()%100;
							if(Rand <= Percent)
							{
								lpObj->pInventory[8].m_NewOption = 4;
								gObjMakePreviewCharSet(lpObj->m_Index);
								GCEquipmentChange(lpObj->m_Index,8);
								GCItemListSend(lpObj->m_Index);

								char sBuf[255]={0};
								wsprintf(sBuf,"[Illusion Temple] %s obtained Gold Fenrir!",lpObj->Name);
								GCServerMsgStringSendToRoom(Level,sBuf,0);
							} else {
								GCServerMsgStringSend("Fenrir upgrade failed!",lpObj->m_Index, 0x01);
							}
						} else {
							GCServerMsgStringSend("Fenrir upgrade failed!",lpObj->m_Index, 0x01);
						}
					}
				}
			}
		}
	}
}

int CIllusionTemple::Ranking_GetClass(int PlayerID)
{
	if(OBJMAX_RANGE(PlayerID) == 0)
	{
		return -1;
	}

	int dbClass = gObj[PlayerID].DbClass;	
	
	if((dbClass == DB_SOUL_MASTER) || (dbClass == DB_BLADE_KNIGHT) || (dbClass == DB_MUSE_ELF) || (dbClass == DB_BLOODY_SUMMONER))
	{
		return gObj[PlayerID].Class+8;
	}else if (gObjIsNewClass(&gObj[PlayerID]) == 1)
	{
		return gObj[PlayerID].Class+16;
	}else
	{
		return gObj[PlayerID].Class;
	}
}

BYTE CIllusionTemple::Ranking_TopTen(BYTE Level)
{
	unsigned long tempexp;
	DWORD tempPlayerID;
	LPSTR tempName;
	int tempTeam;
	int tempkill_points = 0;
	int temptotal_score = 0;

	BYTE PlayerCount = 0;

	for (int i=0;i < (ILLUSIONTEMPLE_MAX_PLAYERS*2); i++)
	{
		for (int j=0;j < (ILLUSIONTEMPLE_MAX_PLAYERS*2)-1 ; j++)
		{
			if(Room[Level].Player[j].Index > 0)
			{
				if(Room[Level].Player[j].Exp_Current < Room[Level].Player[j+1].Exp_Current)
				{
					PlayerCount += 1;

					tempPlayerID							= Room[Level].Player[j].Index;
					tempName								= Room[Level].Player[j].Name;
					tempTeam								= Room[Level].Player[j].Team;
			 		tempexp									= Room[Level].Player[j].Exp_Current;
					tempkill_points							= Room[Level].Player[j].kill_points;
					temptotal_score							= Room[Level].Player[j].total_score;

					Room[Level].Player[j].Index				= Room[Level].Player[j+1].Index;
					Room[Level].Player[j].Name				= Room[Level].Player[j+1].Name;
					Room[Level].Player[j].Team				= Room[Level].Player[j+1].Team;
			 		Room[Level].Player[j].Exp_Current		= Room[Level].Player[j+1].Exp_Current;
					Room[Level].Player[j].kill_points		= Room[Level].Player[j+1].kill_points;
					Room[Level].Player[j].total_score		= Room[Level].Player[j+1].total_score;

					Room[Level].Player[j+1].Index			= tempPlayerID;
					Room[Level].Player[j+1].Name			= tempName;
					Room[Level].Player[j+1].Team			= tempTeam;
					Room[Level].Player[j+1].Exp_Current		= tempexp;
					Room[Level].Player[j+1].kill_points		= tempkill_points;
					Room[Level].Player[j+1].total_score		= temptotal_score;
				}
			}
		}
	}

	return PlayerCount;
}

void CIllusionTemple::End_SetExp(BYTE Level)
{
	for(int i=0;i < (ILLUSIONTEMPLE_MAX_PLAYERS*2); i++)
	{
		if (Room[Level].Player[i].Index > 0)
		{			
			int PlayerID = Room[Level].Player[i].Index;
			if ( gObj[PlayerID].Connected == PLAYER_PLAYING )
			{
				Room[Level].Player[i].Exp_Current = Room[Level].Player[i].total_score * this->ExtraRankExp;

				int LevelUp = 0;
				gObjLevelUp(&gObj[PlayerID],Room[Level].Player[i].Exp_Current,gObj[PlayerID].Class,EVENT_TYPE_NONE,LevelUp);
			}
		}
	}
}

void CIllusionTemple::Start()
{
	PMSG_STATE_ILLUSIONTEMPLE pMsg;
	PHeadSubSetB((LPBYTE)&pMsg, 0xBF, 0x09, sizeof(pMsg));

	pMsg.State = 0x03;
	for(int Level=0;Level<MAX_ILLUSION_TEMPLE_LEVEL;Level++)
	{
		int PlayerCount = IllusionTemple.Room[Level].TeamOrange_Count + IllusionTemple.Room[Level].TeamBlue_Count;
		
		if (PlayerCount > 0)
		{
			EventStart = 1;
			IllusionTemple.Room[Level].Start = 1;

			GCServerMsgStringSendToRoom(Level,lMsg.Get(MSGGET(14, 12)),0);
			LogAddTD("[Illusion Temple](%d) Illusion Temple Event Start", Level + 1);

			pMsg.Type  = 1;
			DataSendRoom(Level,(BYTE*)&pMsg,sizeof(pMsg));

			//Send Starting Points if bigger than 0
			for (int i=0; i < (ILLUSIONTEMPLE_MAX_PLAYERS*2); i++)
			{
				if (Room[Level].Player[i].kill_points > 0)
				{
					int PlayerID = Room[Level].Player[i].Index;
					if(PlayerID > 0)
					{
						if ( gObj[PlayerID].Type == OBJ_USER )
						{
							PMSG_KILLPOINT_ILLUSIONTEMPLE pMsg;
							PHeadSubSetB((LPBYTE)&pMsg, 0xBF, 0x06, sizeof(pMsg));
							pMsg.Points = Room[Level].Player[i].kill_points;
							DataSend(PlayerID, (BYTE*)&pMsg, sizeof(pMsg));
						}
					}
				}
			}

			GCServerMsgStringSendToRoom(Level,"Event will start in 30 seconds.",1);
			Sleep(30000);

			//Open Gate
			pMsg.Type  = 2;
			DataSendRoom(Level,(BYTE*)&pMsg,sizeof(pMsg));
		} else 
		{
			IllusionTemple.Room[Level].Start = 0;
			LogAddTD("[Illusion Temple](%d) Illusion Temple Event Failed -> No Users Inside", Level + 1);
		}
	}

	_beginthread( CIllusionTemple__TimeInside, 0, NULL  );
}

void CIllusionTemple::Ranking_MSG(BYTE Level)
{
	End_SetExp(Level);
	BYTE PlayerCount = Ranking_TopTen(Level);

	char cBUFFER[255] = {0};
	PMSG_ILLUSION_EXP * msgStats = (PMSG_ILLUSION_EXP *)&cBUFFER[0];
	PMSG_ILLUSION_EXP_DATA * lpMsgBody = (PMSG_ILLUSION_EXP_DATA *)&cBUFFER[sizeof(PMSG_ILLUSION_EXP)];

	msgStats->h.c = 0xC1;
	msgStats->h.size = 0x00;
	msgStats->h.headcode = 0xBF;
	msgStats->subcode = 0x04;
	msgStats->Points1 = (unsigned char)Room[Level].TeamOrange_Score * 2;
	msgStats->Points2 = (unsigned char)Room[Level].TeamBlue_Score * 2;

	BYTE rankPosition = 0;
	//Make Packet
	for(int i=0;i < (ILLUSIONTEMPLE_MAX_PLAYERS*2); i++)
	{
		if (Room[Level].Player[i].Index > 0)
		{			
			int PlayerID = Room[Level].Player[i].Index;
			if ( gObj[PlayerID].Connected == PLAYER_PLAYING )
			{
				if (Room[Level].Player[i].Team == ILLUSIONTEMPLE_TEAM_ORANGE)
				{
					char tName[11]= {0};
					memcpy(tName, gObj[PlayerID].Name, MAX_ACCOUNT_LEN);
					memcpy(lpMsgBody[rankPosition].Name, tName, MAX_ACCOUNT_LEN);

					lpMsgBody[rankPosition].Class		= Ranking_GetClass(PlayerID);	//(unsigned char)gObj[PlayerID].Class;
					lpMsgBody[rankPosition].Experience	= Room[Level].Player[i].Exp_Current;

					lpMsgBody[rankPosition].ChangeUP	= 0;	//gObj[PlayerID].ChangeUP;
					lpMsgBody[rankPosition].Color		= (ILLUSIONTEMPLE_TEAM_ORANGE - 1);
					lpMsgBody[rankPosition].Tmp3		= 0;
					lpMsgBody[rankPosition].Tmp1		= 0;
					lpMsgBody[rankPosition].Tmp2		= 0;

					LogAddTD("[Illusion Temple][RANK](%d) [%d] Player:%s, %d, %d/%d, %d, %d",
						Level+1, ILLUSIONTEMPLE_TEAM_ORANGE-1, tName,
						gObj[PlayerID].ChangeUP, 
						(unsigned char)gObj[PlayerID].Class, Ranking_GetClass(PlayerID), 
						Room[Level].Player[i].Exp_Current, Room[Level].Player[i].total_score 
					);

					rankPosition++;				
				}
			}
		}
	}

	for(int i=0;i < (ILLUSIONTEMPLE_MAX_PLAYERS*2); i++)
	{
		if (Room[Level].Player[i].Index > 0)
		{			
			int PlayerID = Room[Level].Player[i].Index;
			if ( gObj[PlayerID].Connected == PLAYER_PLAYING )
			{
				if (Room[Level].Player[i].Team == ILLUSIONTEMPLE_TEAM_BLUE)
				{
					char tName[10]= {0};
					memcpy(tName, gObj[PlayerID].Name, MAX_ACCOUNT_LEN);
					memcpy(lpMsgBody[rankPosition].Name, tName, MAX_ACCOUNT_LEN);

					lpMsgBody[rankPosition].Class		= Ranking_GetClass(PlayerID);		//(unsigned char)gObj[PlayerID].Class;
					lpMsgBody[rankPosition].Experience	= Room[Level].Player[i].Exp_Current;

					lpMsgBody[rankPosition].ChangeUP	= 0;	//gObj[PlayerID].ChangeUP;
					lpMsgBody[rankPosition].Color		= (ILLUSIONTEMPLE_TEAM_BLUE-1);
					lpMsgBody[rankPosition].Tmp3		= 0;
					lpMsgBody[rankPosition].Tmp1		= 0;
					lpMsgBody[rankPosition].Tmp2		= 0;

					LogAddTD("[Illusion Temple][RANK](%d) [%d] Player:%s, %d, %d/%d, %d, %d",
						Level+1, ILLUSIONTEMPLE_TEAM_BLUE-1, tName,
						gObj[PlayerID].ChangeUP, 
						(unsigned char)gObj[PlayerID].Class, Ranking_GetClass(PlayerID), 
						Room[Level].Player[i].Exp_Current, Room[Level].Player[i].total_score 
					);
					
					rankPosition++;					
				}
			}
		}
	}

	msgStats->pCount = rankPosition;
	msgStats->h.size = rankPosition*sizeof(PMSG_ILLUSION_EXP_DATA) + sizeof(PMSG_ILLUSION_EXP);

	DataSendRoom(Level, (PBYTE)msgStats, msgStats->h.size);
}

void CIllusionTemple::SkillUse(int PlayerID, int TargetID, int Skill)
{
	if ( OBJMAX_RANGE(PlayerID) == FALSE )
	{
		return;
	}
	if ( OBJMAX_RANGE(TargetID) == FALSE )
	{
		return;
	}

	if ( gObj[PlayerID].Connected == PLAYER_PLAYING )
	{
		int Level = gObj[PlayerID].MapNumber - MAP_INDEX_ILLUSIONTEMPLE1;
		int pIndex = Player_GetIndex(Level,PlayerID);
		if(pIndex != -1)
		{			
			BYTE SkillLen = 1;	//0 if skill is in use

			switch(Skill)
			{
				case IL_ORDER_OF_PROTECT:
				{
					if(KillPointReduce(PlayerID,5) == 1)
					{
						gObj[TargetID].m_SkillIT_Time = 15;
						gObj[TargetID].m_SkillIT_Number = IL_ORDER_OF_PROTECT;

						GCSkillInfoSend(&gObj[TargetID],1,34);
						SkillEffectStart(PlayerID, TargetID, IL_ORDER_OF_PROTECT, SkillLen);
					}
				}break;
				case IL_ORDER_OF_FREEZE:
				{
					if(KillPointReduce(PlayerID,5) == 1)
					{
						gObj[TargetID].m_SkillIT_Time = 5;
						gObj[TargetID].m_SkillIT_Number = IL_ORDER_OF_FREEZE;

						GCSkillInfoSend(&gObj[TargetID],1,65);
						SkillEffectStart(PlayerID, TargetID, IL_ORDER_OF_FREEZE, SkillLen);
					}
				}break;
				case IL_ORDER_OF_TRACKING:
				{
					if(Room[Level].Hero != -1)
					{
						if(KillPointReduce(PlayerID,10) == 1)
						{
							SkillEffectStart(PlayerID, TargetID, IL_ORDER_OF_TRACKING, SkillLen);
							gObjTeleport(gObj[TargetID].m_Index ,(MAP_INDEX_ILLUSIONTEMPLE1 + Level), Room[Level].Hero_X , Room[Level].Hero_Y);
							SkillEffectEnd(TargetID, IL_ORDER_OF_TRACKING);

							gObj[TargetID].m_SkillIT_Time = 7;
							gObj[TargetID].m_SkillIT_Number = IL_ORDER_OF_PROTECT;
							SkillEffectStart(PlayerID, TargetID, IL_ORDER_OF_PROTECT, SkillLen);
						}
					}
				}break;
				case IL_ORDER_OF_SHIELD:
				{
					if(KillPointReduce(PlayerID,5) == 1)
					{
						gObj[TargetID].m_SkillIT_Time = 10;
						gObj[TargetID].m_SkillIT_Number = IL_ORDER_OF_SHIELD;

						gObj[TargetID].iShield = gObj[TargetID].iShield/2;

						SkillEffectStart(PlayerID, TargetID, IL_ORDER_OF_SHIELD, SkillLen);
						//GCMagicAttackNumberSend(&gObj[PlayerID],IL_ORDER_OF_SHIELD,TargetID,1);
						SkillEffectEnd(TargetID, IL_ORDER_OF_SHIELD);
					}
				}break;
			}
		}
	}
}

void CIllusionTemple::SkillEffectStart(int aIndex, int aTargetIndex, unsigned char SkillId, unsigned char SkillLen) 
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	PMSG_ILLUSION_ACTIVESKILL pMsg;
	pMsg.h.c = 0xC1;
	pMsg.h.size = 0x0C;			//0x0C 
	pMsg.h.headcode = 0xBF;
	pMsg.h.subcode = 0x02;

	pMsg.unk1 = 0x00;
	pMsg.SkillEx = 0x00;
	pMsg.SkillId = SkillId;
	pMsg.useTime = SkillLen;
	pMsg.aOwnerIndex = (short)aIndex;
	pMsg.aRecvrIndex = (short)aTargetIndex;

	MsgSendDistNear(&gObj[aIndex], (UCHAR*)&pMsg, pMsg.h.size, 16);
}

void CIllusionTemple::SkillEffectEnd(int aIndex, unsigned char SkillId) 
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	PMSG_ILLUSION_DEACTIVESKILL pMsg;
	pMsg.h.c = 0xC1;
	pMsg.h.size = 0x08;				//0x08
	pMsg.h.headcode = 0xBF;
	pMsg.h.subcode = 0x07;

	pMsg.SkillEx = 0x00;
	pMsg.SkillId = SkillId;
	pMsg.aOwnerIndex = aIndex;
	
	MsgSendDistNear(&gObj[aIndex], (UCHAR*)&pMsg, pMsg.h.size, 16);
}

BYTE CIllusionTemple::ReqPlayerNumberInside(LPOBJ lpObj)
{
	if (gObjIsNewClass(lpObj) == 0)
	{
		for(int i=0;i<MAX_ILLUSION_TEMPLE_LEVEL-1;i++)
		{
			int pLevel = lpObj->Level;
			if ((pLevel >= g_sttILLUSIONTEMPLE_LEVEL[i].iLOWER_BOUND) && (pLevel <= g_sttILLUSIONTEMPLE_LEVEL[i].iUPPER_BOUND))
			{
				return Room[i].TeamBlue_Count + Room[i].TeamOrange_Count;
			}
		}
	} else { 
		return Room[5].TeamBlue_Count + Room[5].TeamOrange_Count;
	}

	return 0;
}

void CIllusionTemple::ReqEnter(LPOBJ lpObj)
{
	if ( lpObj->m_IfState.type != 96 )
	{
		ANTI_HACK_LOG.Output("[ANTI-HACK][IllusionTemple][ResultEnter] User in map(%d) [%s][%s] State(u:%d,t:%d,s:%d)",
			lpObj->MapNumber, lpObj->AccountID, lpObj->Name,
			lpObj->m_IfState.use,lpObj->m_IfState.type,lpObj->m_IfState.state);

		return;
	}

	lpObj->m_IfState.use = 0;
	lpObj->m_IfState.type = 0;

	if (gObjIsNewClass(lpObj) == 0)
	{
		for(int i=0;i<MAX_ILLUSION_TEMPLE_LEVEL-1;i++)
		{
			int pLevel = lpObj->Level;
			if ((pLevel >= g_sttILLUSIONTEMPLE_LEVEL[i].iLOWER_BOUND) && (pLevel <= g_sttILLUSIONTEMPLE_LEVEL[i].iUPPER_BOUND))
			{
				if( (Room[i].TeamBlue_Count + Room[i].TeamOrange_Count) < (ILLUSIONTEMPLE_MAX_PLAYERS*2) )
				{

					if(DeleteItemByLevel(lpObj->m_Index,ITEMGET(13,51),i+1) == 1)
					{
						lpObj->m_bEventTicketType = 0;
						Warp_WaitRoom(lpObj,i);
						return;
					} 
					

					int pos = gObjCheckItemPos(lpObj->m_Index,ITEMGET(13,61));	//New WZShop Ticket
					if(pos >= 0)
					{
						if ( lpObj->pInventory[pos].m_Durability == 1.0f )
						{
							gObjInventoryDeleteItem(lpObj->m_Index, pos);
							GCInventoryItemDeleteSend(lpObj->m_Index, pos, 1);
						}
						else if ( lpObj->pInventory[pos].m_Durability > 1.0f )
						{
							lpObj->pInventory[pos].m_Durability -= 1.0f;
							GCItemDurSend2(lpObj->m_Index, pos, lpObj->pInventory[pos].m_Durability, 0);
						}
				
						lpObj->m_bEventTicketType = 1;
						Warp_WaitRoom(lpObj,i);
						return;
					} else 
					{
						goto NoTicket;
					}
				}else
				{
					goto Full;
				}
			}
		}
		
	}else
	{ 
		if( (Room[5].TeamBlue_Count + Room[5].TeamOrange_Count) < (ILLUSIONTEMPLE_MAX_PLAYERS*2) )
		{
			if(DeleteItemByLevel(lpObj->m_Index,ITEMGET(13,51),6) == 1)
			{
				lpObj->m_bEventTicketType = 0;
				Warp_WaitRoom(lpObj,5);
				return;
			} 

			int pos = gObjCheckItemPos(lpObj->m_Index,ITEMGET(13,61));	//New WZShop Ticket
			if(pos >= 0)
			{
				if ( lpObj->pInventory[pos].m_Durability == 1.0f )
				{
					gObjInventoryDeleteItem(lpObj->m_Index, pos);
					GCInventoryItemDeleteSend(lpObj->m_Index, pos, 1);
				}
				else if ( lpObj->pInventory[pos].m_Durability > 1.0f )
				{
					lpObj->pInventory[pos].m_Durability -= 1.0f;
					GCItemDurSend2(lpObj->m_Index, pos, lpObj->pInventory[pos].m_Durability, 0);
				}

				lpObj->m_bEventTicketType = 1;
				Warp_WaitRoom(lpObj,5);
				return;
			} else  
			{
				goto NoTicket;
			}
		}else
		{
			goto Full;
		}
	}
	
Full:
	GCServerMsgStringSend(lMsg.Get(MSGGET(14, 15)),lpObj->m_Index,1);
	return;
	
NoTicket:
	GCServerMsgStringSend(lMsg.Get(MSGGET(14, 16)),lpObj->m_Index,1);
	return;
}

void CIllusionTemple::SendTimer(BYTE Level,int ctime)
{
	//====================================================================================================
	// DaRKav [Feb 28, 2008] Illusion Timer Packet Structure
	//====================================================================================================
	// Note: Blue Team, Orange Team => Different packets, contain only info on their teammates
	//====================================================================================================
	//Calculating Time
	int Map=0;
	int TIME2=0;
	int TIME1=0;
	TIME2=(int)floor((float)ctime/255);
	TIME1=ctime-(TIME2*255);

	Map = MAP_INDEX_ILLUSIONTEMPLE1 + Level;

	if(Room[Level].Hero != -1)
	{
		if (ctime == 0)
		{
			DeleteItemByID(Room[Level].Hero,ITEMGET(14,64));

			Room[Level].Hero = -1;
			Room[Level].Hero_X = 0;
			Room[Level].Hero_Y = 0;
		}

		if( gObj[Room[Level].Hero].Connected >= PLAYER_PLAYING )
		{
			if(gObj[Room[Level].Hero].MapNumber == Map)
			{
				Room[Level].Hero_X = gObj[Room[Level].Hero].X;
				Room[Level].Hero_Y = gObj[Room[Level].Hero].Y;
			}
		}
	}

	//Building Packets for players
	PMSG_ILLUSION_EVENTSTATE evState;

	//Header
	evState.h.c = 0xC1;
	evState.h.size = 0x00;
	evState.h.headcode = 0xBF;
	evState.h.subcode = 0x01;

	//Timer
	evState.TimerHI = TIME1;
	evState.TimerLO = TIME2;

	//Hero Holding Shiny Spark
	evState.aHeroIndex = Room[Level].Hero;
	evState.HeroX = (unsigned char)Room[Level].Hero_X;
	evState.HeroY = (unsigned char)Room[Level].Hero_Y;

	//Team Scores
	evState.BluePoints = (unsigned char)Room[Level].TeamBlue_Score;
	evState.RedPoints = (unsigned char)Room[Level].TeamOrange_Score;

	//================================================================================
	//Team Player Position View [ORANGE]
	//================================================================================
	evState.RadarColor = (ILLUSIONTEMPLE_TEAM_ORANGE - 1);

	int i = 0;
	for (int k = 0; k < (ILLUSIONTEMPLE_MAX_PLAYERS*2) ; k++)
	{
		if (Room[Level].Player[k].Team == ILLUSIONTEMPLE_TEAM_ORANGE &&
			Room[Level].Player[k].Index != Room[Level].Hero &&
			gObj[Room[Level].Player[k].Index].MapNumber == Map &&
			gObj[Room[Level].Player[k].Index].Connected == PLAYER_PLAYING &&
			gObj[Room[Level].Player[k].Index].Type == OBJ_USER &&
			gObj[Room[Level].Player[k].Index].X > 0 && gObj[Room[Level].Player[k].Index].X < 255 &&
			gObj[Room[Level].Player[k].Index].Y > 0 && gObj[Room[Level].Player[k].Index].Y < 255
			)
		{
			evState.Radar[i].aIndex = Room[Level].Player[k].Index;
			evState.Radar[i].X = gObj[Room[Level].Player[k].Index].X;
			evState.Radar[i].Y = gObj[Room[Level].Player[k].Index].Y;
			evState.Radar[i].unk2 = 0;
			evState.Radar[i].unk = 0;

			//Fernir timer [13,37]
			if (gObj[Room[Level].Player[k].Index].pInventory[8].IsItem() == TRUE && 
				gObj[Room[Level].Player[k].Index].pInventory[8].m_Type == ITEMGET(13,37) )
			{
				Room[Level].Player[k].FernirTime += 1;
			}

			i++;
		} else {
			evState.Radar[i].aIndex = -1;
			evState.Radar[i].X = 0;
			evState.Radar[i].Y = 0;
			evState.Radar[i].unk2 = 0;
			evState.Radar[i].unk = 0;
		}
	}

	for (int l = i; l < (ILLUSIONTEMPLE_MAX_PLAYERS*2); l++)
	{
		evState.Radar[l].aIndex = -1;
		evState.Radar[l].X = 0;
		evState.Radar[l].Y = 0;
		evState.Radar[l].unk2 = 0;
		evState.Radar[l].unk = 0;
	}

	evState.PlayersCount = i;
	evState.h.size = 14 + (evState.PlayersCount * sizeof(PMSG_ILLUSION_RADAR));
	DataSendTeamRoom(Level,ILLUSIONTEMPLE_TEAM_ORANGE,(BYTE*)&evState, evState.h.size);//sizeof(evState));

	//================================================================================
	//Team Player Position View [BLUE]
	//================================================================================
	evState.RadarColor = (ILLUSIONTEMPLE_TEAM_BLUE - 1);

	i = 0;
	for (int k = 0; k < (ILLUSIONTEMPLE_MAX_PLAYERS*2); k++)
	{
		if (Room[Level].Player[k].Team == ILLUSIONTEMPLE_TEAM_BLUE &&
			Room[Level].Player[k].Index != Room[Level].Hero &&
			gObj[Room[Level].Player[k].Index].MapNumber == Map &&
			gObj[Room[Level].Player[k].Index].Connected == PLAYER_PLAYING &&
			gObj[Room[Level].Player[k].Index].Type == OBJ_USER &&
			gObj[Room[Level].Player[k].Index].X > 0 && gObj[Room[Level].Player[k].Index].X < 255 &&
			gObj[Room[Level].Player[k].Index].Y > 0 && gObj[Room[Level].Player[k].Index].Y < 255
			)
		{
			evState.Radar[i].aIndex = Room[Level].Player[k].Index;
			evState.Radar[i].X = gObj[Room[Level].Player[k].Index].X;
			evState.Radar[i].Y = gObj[Room[Level].Player[k].Index].Y;
			evState.Radar[i].unk2 = 0;
			evState.Radar[i].unk = 0;
			
			//Fernir timer [13,37]
			if (gObj[Room[Level].Player[k].Index].pInventory[8].IsItem() == TRUE && 
				gObj[Room[Level].Player[k].Index].pInventory[8].m_Type == ITEMGET(13,37) )
			{
				Room[Level].Player[k].FernirTime += 1;
			}
			
			i++;
		} else {
			evState.Radar[i].aIndex = -1;
			evState.Radar[i].X = 0;
			evState.Radar[i].Y = 0;
			evState.Radar[i].unk2 = 0;
			evState.Radar[i].unk = 0;
		}
	}

	for (int l = i; l < (ILLUSIONTEMPLE_MAX_PLAYERS*2); l++)
	{
		evState.Radar[l].aIndex = -1;
		evState.Radar[l].X = 0;
		evState.Radar[l].Y = 0;
		evState.Radar[l].unk2 = 0;
		evState.Radar[l].unk = 0;
	}

	evState.PlayersCount = i;
	evState.h.size = 14 + (evState.PlayersCount * sizeof(PMSG_ILLUSION_RADAR));
	DataSendTeamRoom(Level,ILLUSIONTEMPLE_TEAM_BLUE,(BYTE*)&evState, evState.h.size);//sizeof(evState));
}


void CIllusionTemple::SetState_Closed(BYTE Level, BOOL ShowLog)
{
	if(ShowLog == 1)
	{
		CheckSync(Level);
		LogAddTD("[Illusion Temple](%d) SetState CLOSED", Level+1);
	}
	
	Room[Level].Start = 0;
	for(int Player=0;Player<(ILLUSIONTEMPLE_MAX_PLAYERS*2);Player++)
	{
		Player_Remove(Level,Player);
	}

	Room[Level].TeamBlue_Count = 0;
	Room[Level].TeamBlue_Score = 0;
	Room[Level].TeamOrange_Count = 0;
	Room[Level].TeamOrange_Score = 0;

	Room[Level].DroppedBall = 0;
	Room[Level].Hero = -1;
	Room[Level].Hero_X = 0;//Room[Level].Statue_X;
	Room[Level].Hero_Y = 0;//Room[Level].Statue_Y;

	Time_Inside_Current[Level] = IllusionEventMinLength*60;
	Time_Inside_Passed[Level] = 0;
}

void CIllusionTemple::RewardGiveItem(int aIndex, BYTE Num, BYTE Level)
{
	if(this->IsExtraReward == 0 && Num > 0)
	{
		return;
	}
	
	int Count = this->Room[Level].TeamOrange_Count + this->Room[Level].TeamBlue_Count;

	if(this->IsExtraRewardMinPlayerReq > Count && Num > 0)
	{
		return;
	}

	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	ItemSerialCreateSend(aIndex,gObj[aIndex].MapNumber,gObj[aIndex].X,gObj[aIndex].Y,
		ITEMGET(Reward_ItemType[Num],Reward_ItemIndex[Num]),Reward_Itemlvl[Num],Reward_ItemDur[Num],Reward_ItemSkill[Num],Reward_ItemLuck[Num],Reward_ItemOpt[Num],-1,Reward_ItemIsExc[Num],0);
}

void CIllusionTemple::GiveReward_Win(BYTE Level)
{
	//Close Gates
	PMSG_STATE_ILLUSIONTEMPLE pMsg;
	PHeadSubSetB((LPBYTE)&pMsg, 0xBF, 0x09, sizeof(pMsg));
	pMsg.State = 3;
	pMsg.Type = 3;
	DataSendRoom(Level,(BYTE*)&pMsg,sizeof(pMsg));
	Sleep(2000);

	Ranking_MSG(Level);

	Sleep(10000);
	int Map = MAP_INDEX_ILLUSIONTEMPLE1+Level;

	if (Room[Level].TeamBlue_Score > Room[Level].TeamOrange_Score)
	{
		LogAddTD("[Illusion Temple](%d) Event Complete Rewarded Team: Blue",Level+1);
		GCServerMsgStringSendToRoom(Level,lMsg.Get(MSGGET(14, 17)),0);

		for(int i=0;i < (ILLUSIONTEMPLE_MAX_PLAYERS*2); i++)
		{
			if (Room[Level].Player[i].Index > 0)
			{
				if( gObj[Room[Level].Player[i].Index].Connected == PLAYER_PLAYING &&
					gObj[Room[Level].Player[i].Index].Type == OBJ_USER )
				{
					int PlayerID = Room[Level].Player[i].Index;

					if(gObj[PlayerID].MapNumber == Map)
					{
						if(Room[Level].Player[i].Team == ILLUSIONTEMPLE_TEAM_BLUE)
						{
							LogAddTD("[Illusion Temple](%d) Event Complete Rewarded (Player: %s, Item: (%d %d))",
								Level+1,gObj[PlayerID].Name,Reward_ItemType[0],Reward_ItemIndex[0]);
							this->RewardGiveItem(PlayerID,0,Level);
							//ItemSerialCreateSend(PlayerID,gObj[PlayerID].MapNumber,gObj[PlayerID].X,gObj[PlayerID].Y,
							//					 ITEMGET(Reward_ItemType[0],Reward_ItemIndex[0]),Reward_Itemlvl[0],Reward_ItemDur[0],Reward_ItemSkill[0],Reward_ItemLuck[0],Reward_ItemOpt[0],-1,Reward_ItemIsExc[0],0);
						}
					}
				}
			}
		}
	}
	else if (Room[Level].TeamBlue_Score < Room[Level].TeamOrange_Score)
	{
		LogAddTD("[Illusion Temple](%d) Event Complete Rewarded Team: Orange",Level+1);
		GCServerMsgStringSendToRoom(Level,lMsg.Get(MSGGET(14, 18)),0);

		for(int i=0;i < (ILLUSIONTEMPLE_MAX_PLAYERS*2); i++)
		{
			if (Room[Level].Player[i].Index > 0)
			{
				if( gObj[Room[Level].Player[i].Index].Connected == PLAYER_PLAYING &&
					gObj[Room[Level].Player[i].Index].Type == OBJ_USER )
				{
					int PlayerID = Room[Level].Player[i].Index;

					if(gObj[PlayerID].MapNumber == Map)
					{
						if(Room[Level].Player[i].Team == ILLUSIONTEMPLE_TEAM_ORANGE)
						{
							LogAddTD("[Illusion Temple](%d) Event Complete Rewarded (Player: %s, Item: (%d %d))",
								Level+1,gObj[PlayerID].Name,Reward_ItemType[0],Reward_ItemIndex[0]);
							this->RewardGiveItem(PlayerID,0,Level);
							//ItemSerialCreateSend(PlayerID,gObj[PlayerID].MapNumber,gObj[PlayerID].X,gObj[PlayerID].Y,
							//					 ITEMGET(Reward_ItemType[0],Reward_ItemIndex[0]),Reward_Itemlvl[0],Reward_ItemDur[0],Reward_ItemSkill[0],Reward_ItemLuck[0],Reward_ItemOpt[0],-1,Reward_ItemIsExc[0],0);
						}
					}
				}
			}
		}
	}
	else
	{
		LogAddTD("[Illusion Temple](%d) Event finished but fail - Draw",Level+1);
		GCServerMsgStringSendToRoom(Level,lMsg.Get(MSGGET(14, 19)),0);
	}


	for (int i=0; i < (ILLUSIONTEMPLE_MAX_PLAYERS*2); i++)
	{
		if (Room[Level].TeamBlue_Score > Room[Level].TeamOrange_Score &&
			Room[Level].Player[i].Team == ILLUSIONTEMPLE_TEAM_BLUE)
		{
			FenrirChange(&gObj[Room[Level].Player[i].Index],Level,Room[Level].Player[i].FernirTime);
			break;
		}

		if (Room[Level].TeamBlue_Score < Room[Level].TeamOrange_Score &&
			Room[Level].Player[i].Team == ILLUSIONTEMPLE_TEAM_ORANGE)
		{
			FenrirChange(&gObj[Room[Level].Player[i].Index],Level,Room[Level].Player[i].FernirTime);
			break;
		}
	}

	if(Room[Level].Player[0].Index > 0)
	{
		RewardGiveItem(Room[Level].Player[0].Index,1,Level);
	}
	if(Room[Level].Player[1].Index > 0)
	{
		RewardGiveItem(Room[Level].Player[1].Index,2,Level);
	}
	if(Room[Level].Player[2].Index > 0)
	{
		RewardGiveItem(Room[Level].Player[2].Index,3,Level);
	}

	GCServerMsgStringSendToRoom(Level,"You will be warped out in 60 seconds!", 0x01);
	Sleep(60000);
	SetState_End(Level,false);
}

void CIllusionTemple::GiveExp(LPOBJ lpObj,BYTE Level,BYTE Type)
{
	int AddExp = 0;

	switch(Type)
	{
		case 0:
		{
			AddExp = (int)(this->ExpForBallDropped * lpObj->Level * (Level + 1)) ;
		}break;
	};
		
	if(AddExp > 0)
	{
		PMSG_KILLPLAYER pkillMsg;
		PHeadSetBE((LPBYTE)&pkillMsg, 0x16, sizeof(pkillMsg));

		if(AddExp > 0x0FFFF)
			AddExp = 65534;

		pkillMsg.NumberH = 0xFF;
		pkillMsg.NumberL = 0xFF;
		pkillMsg.ExpH = SET_NUMBERH(AddExp);
		pkillMsg.ExpL = SET_NUMBERL(AddExp);
		pkillMsg.DamageH = SET_NUMBERH(0x00);
		pkillMsg.DamageL = SET_NUMBERL(0x00);

		DataSend(lpObj->m_Index, (UCHAR*)&pkillMsg, pkillMsg.h.size);

		gObjSetExpPetItem(lpObj->m_Index, AddExp);
		int LevelUp = 0;
		gObjLevelUp(lpObj,AddExp,lpObj->Class,EVENT_TYPE_NONE,LevelUp);
	}
}

void CIllusionTemple::ReturnGift(LPOBJ lpObj,BYTE Level,WORD npc)
{
	char sBuf[1024]={0};
	if (Room[Level].Start==1)
	{
		BOOL Flag = 0;
		
		if(Room[Level].Hero == lpObj->m_Index )
		{
			int pIndex = Player_GetIndex(Level,lpObj->m_Index);

			if (pIndex == -1)
				return;

			if(Room[Level].Player[pIndex].Team == ILLUSIONTEMPLE_TEAM_BLUE)
			{
				if(npc == 384)
				{
					this->GiveExp(lpObj,Level,0);
					Room[Level].TeamBlue_Score++;
					LogAddTD("[Illusion Temple](%d) [Blue Team][Hero:%s] Gift returned to the statue!",Level+1,lpObj->Name);
					wsprintf(sBuf,lMsg.Get(MSGGET(14, 20)));
					Flag=1;
				}else
				{
					LogAddTD("[Illusion Temple](%d) [Blue Team][Hero:%s] Is not the blue storage!",Level+1,lpObj->Name);
					GCServerMsgStringSend(lMsg.Get(MSGGET(14, 21)),lpObj->m_Index , 0x01);
				}
			}
			else if(Room[Level].Player[pIndex].Team == ILLUSIONTEMPLE_TEAM_ORANGE)
			{
				if(npc == 383)
				{
					this->GiveExp(lpObj,Level,0);
					Room[Level].TeamOrange_Score++;
					LogAddTD("[Illusion Temple](%d) [Orange Team][Hero:%s] Gift returned to the statue!",Level+1,lpObj->Name);
					wsprintf(sBuf,lMsg.Get(MSGGET(14, 22)));
					Flag=1;
				}else
				{
					LogAddTD("[Illusion Temple](%d) [Orange Team][Hero:%s] Is not the orange storage!",Level+1,lpObj->Name);
					GCServerMsgStringSend(lMsg.Get(MSGGET(14, 23)),lpObj->m_Index , 0x01);
				}
			}
			if(Flag==1)
			{
				Room[Level].DroppedBall = 0;

				GCServerMsgStringSendToRoom(Level,sBuf,1);

				Room[Level].Player[pIndex].total_score += this->ScoreBallRankExp;

				//gObjClearViewport(&gObj[Room[Level].Hero]);
				//gObjViewportListProtocolCreate(&gObj[Room[Level].Hero]);

				Room[Level].Hero = -1;
				Room[Level].Hero_X = 0;//Room[Level].Statue_X;
				Room[Level].Hero_Y = 0;//Room[Level].Statue_Y;
				DeleteItemByID(lpObj->m_Index,ITEMGET(14,64));
				this->ClearMonster(Level);
				this->StatueTeleport(Level);
			}
		}else
		{
			LogAddTD("[Illusion Temple](%d) [%s] Is not a Hero",Level+1,lpObj->Name);
			GCServerMsgStringSend(lMsg.Get(MSGGET(14, 24)),lpObj->m_Index , 0x01);
		}
	}
}

void CIllusionTemple::SetState_End(BYTE Level, bool bReturnTicket)
{
	SetState_Closed(Level,1);
	LogAddTD("[Illusion Temple](%d) SetState END -> Moving Players",Level+1);

	int Map = MAP_INDEX_ILLUSIONTEMPLE1+Level;
	BYTE x = 125 + rand()%3;
	BYTE y = 125 - rand()%3;

	x = 125 - rand()%5;
	y = 125 + rand()%5;

	for ( int n = OBJ_STARTUSERINDEX ; n < OBJMAX ; n++)
	{
		if ( gObj[n].Connected == PLAYER_PLAYING )
		{
			if ( gObj[n].Type  == OBJ_USER )
			{
				if (gObj[n].MapNumber == Map)
				{
					if ( gObj[n].PartyNumber >= 0 )
					{
						PMSG_PARTYDELUSER pMsg;
						int pindex = gParty.GetIndex(gObj[n].PartyNumber, n, gObj[n].DBNumber);
						if ( pindex >= 0 )
						{
							pMsg.Number = pindex;
							CGPartyDelUser(&pMsg, gObj[n].m_Index);
						}
					}

					if (bReturnTicket == true && gObj[n].m_bEventTicketType == 0)
					{
						ItemSerialCreateSend(gObj[n].m_Index, 235, 0, 0, ITEMGET(13,51), (Level+1), 0, 0, 0, 0, -1, 0, 0);
						LogAddTD("[Illusion Temple](%d) Return Ticket due to lack of players [%s][%s]",
							Level+1,
							gObj[n].AccountID, gObj[n].Name
						);
					} else if (bReturnTicket == true && gObj[n].m_bEventTicketType == 1)
					{
						ItemSerialCreateSend(gObj[n].m_Index, 235, 0, 0, ITEMGET(13,61), 0, 1, 0, 0, 0, -1, 0, 0);
						LogAddTD("[Illusion Temple](%d) Return WZTicket due to lack of players [%s][%s]",
							Level+1,
							gObj[n].AccountID, gObj[n].Name
						);
					}

					gObjTeleport(n,0,x,y);
					gObj[n].m_Change = -1;
					gObjViewportListProtocolCreate(&gObj[n]);
				}
			}
		}
	}

	EventStart = 0;
}


void CIllusionTemple::Player_RemoveByID(int pIndex)
{
	int NewPartyIndex = -1;
	int NewPartyOwner = -1;

	if(OBJMAX_RANGE(pIndex) == 0)
	{
		return;
	}

	LogAddTD("[Illusion Temple][%s][%s] Removing User by ID: %d",
		gObj[pIndex].AccountID, gObj[pIndex].Name, pIndex
	);

	for (int Level = 0; Level < MAX_ILLUSION_TEMPLE_LEVEL; Level++)
	{
		for (int i = 0; i < (ILLUSIONTEMPLE_MAX_PLAYERS*2); i++)
		{
			if (pIndex == Room[Level].Player[i].Index)
			{
				BYTE teamcount = 0;
				if(Room[Level].Player[i].Team == ILLUSIONTEMPLE_TEAM_ORANGE)
				{
					Room[Level].TeamOrange_Count--;
					teamcount = Room[Level].TeamOrange_Count;
				}else if(Room[Level].Player[i].Team == ILLUSIONTEMPLE_TEAM_BLUE)
				{
					Room[Level].TeamBlue_Count--;
					teamcount = Room[Level].TeamBlue_Count;
				}

				//==========================================================================
				//Party Rebuilt
				//==========================================================================
				if ( gObj[pIndex].PartyNumber >= 0 && IllusionTemple.Room[Level].Start == 1)
				{
					//Remove player from party
					int iPartyIndex= gParty.GetIndex(gObj[pIndex].PartyNumber, gObj[pIndex].m_Index, gObj[pIndex].DBNumber);

					if ( iPartyIndex >= 0 )
					{
						PMSG_PARTYDELUSER pMsg;
						pMsg.Number = iPartyIndex;

						CGPartyDelUser(&pMsg, gObj[pIndex].m_Index);
					}

					//If party owner left, rebuild party if more than 1 user on the team
					if (iPartyIndex == 0 || teamcount >= 2 )
					{
						for(int j = 0; j < (ILLUSIONTEMPLE_MAX_PLAYERS*2); j++)
						{
							if (Room[Level].Player[j].Index > 0)
							{
								int pNewIndex = Room[Level].Player[j].Index;

								if(pNewIndex != pIndex)
								{
									if(Room[Level].Player[j].Team == Room[Level].Player[i].Team)
									{
										if (NewPartyIndex == -1)
										{
											NewPartyIndex = gParty.Create(pNewIndex, gObj[pNewIndex].DBNumber, gObj[pNewIndex].Level);
											gObj[pNewIndex].PartyNumber = NewPartyIndex;
											NewPartyOwner = pNewIndex;
										}

										if (NewPartyIndex >= 0 && NewPartyOwner != pNewIndex && NewPartyIndex != -1)
										{
											int iPartyPos = gParty.Add(gObj[NewPartyOwner].PartyNumber, pNewIndex, gObj[pNewIndex].DBNumber, gObj[pNewIndex].Level);
											gObj[pNewIndex].PartyNumber = NewPartyIndex;
											gParty.Paint(NewPartyIndex);
										}
							
										if (NewPartyIndex >= 0)
										{
											CGPartyListAll(NewPartyIndex);
										}
						
										LogAddTD("[Illusion Temple][%d](%d) Party Member: %s(%d) PartyID: %d(Owner:%d)",
											Room[Level].Player[j].Team, Level+1,
											gObj[pNewIndex].Name, pNewIndex,
											gObj[pNewIndex].PartyNumber, NewPartyOwner
										);
									}
								}
							}
						}
					}
				}

				//==========================================================================
				gObj[pIndex].m_Change = -1;
				gObjViewportListProtocolCreate(&gObj[pIndex]);

				if (Room[Level].Hero == pIndex &&
					Room[Level].Hero != -1 &&
					pIndex != 0)
				{
					int itemPos = gObjGetItemPos(pIndex,14,64);
					if (itemPos > -1)
					{
						DeleteItemByID(gObj[pIndex].m_Index,ITEMGET(14,64));

						ItemSerialCreateSend(gObj[pIndex].m_Index, MAP_INDEX_ILLUSIONTEMPLE1+Level, Room[Level].Hero_X, Room[Level].Hero_Y, ITEMGET(14,64), 0, 255, 0, 0, 0, -1, 0, 0);
						LogAddTD("[Illusion Temple][%s][%s] TROW New Fireball on User Move",
							gObj[pIndex].AccountID, gObj[pIndex].Name
						);
					} else {
						DeleteItemByID(gObj[pIndex].m_Index,ITEMGET(14,64));

						ItemSerialCreateSend(gObj[pIndex].m_Index, MAP_INDEX_ILLUSIONTEMPLE1+Level, Room[Level].Hero_X, Room[Level].Hero_Y, ITEMGET(14,64), 0, 255, 0, 0, 0, -1, 0, 0);
						LogAddTD("[Illusion Temple][%s][%s] POS ERROR: Drop NEW Fireball",
							gObj[pIndex].AccountID, gObj[pIndex].Name
						);
					}
					GCServerMsgStringSendToRoom(Level,"The fireball has been dropped!",0);

					Room[Level].Hero = -1;
					Room[Level].Hero_X = 0;
					Room[Level].Hero_Y = 0;
				}

				Room[Level].Player[i].Index = 0;
				Room[Level].Player[i].Exp_Current = 0;
				Room[Level].Player[i].Team  = 0;
				Room[Level].Player[i].kill_points = this->startSkillPoints;
				Room[Level].Player[i].total_score = 0;
				Room[Level].Player[i].FernirTime = 0;

				break;
			}
		}
	}
}

void CIllusionTemple::Player_Remove(BYTE Level, int Index)
{
	if (Room[Level].Player[Index].Index == 0)
	{
		Room[Level].Player[Index].Index = 0;
		Room[Level].Player[Index].Exp_Current = 0;
		Room[Level].Player[Index].Team = 0;
		Room[Level].Player[Index].kill_points = this->startSkillPoints;
		Room[Level].Player[Index].total_score = 0;
		Room[Level].Player[Index].FernirTime = 0;
		return;
	}

	if(OBJMAX_RANGE(Room[Level].Player[Index].Index) == 0)
	{
		return;
	}

	LogAddTD("[Illusion Temple][%s][%s] Removing User from Level:%d Team:%d",
		gObj[Room[Level].Player[Index].Index].AccountID, gObj[Room[Level].Player[Index].Index].Name,
		Level+1, Room[Level].Player[Index].Team
	);

	if( gObj[Room[Level].Player[Index].Index].Connected == PLAYER_PLAYING &&
		gObj[Room[Level].Player[Index].Index].Type == OBJ_USER )
	{
		gObj[Room[Level].Player[Index].Index].m_Change = -1;
		gObjViewportListProtocolCreate(&gObj[Room[Level].Player[Index].Index]);		
	}

	BYTE teamcount = 0;
	if(Room[Level].Player[Index].Team == ILLUSIONTEMPLE_TEAM_ORANGE)
	{
		Room[Level].TeamOrange_Count--;
		teamcount = Room[Level].TeamOrange_Count;
	}else if(Room[Level].Player[Index].Team == ILLUSIONTEMPLE_TEAM_BLUE)
	{
		Room[Level].TeamBlue_Count--;
		teamcount = Room[Level].TeamBlue_Count;
	}

	int NewPartyIndex = -1;
	int NewPartyOwner = -1;
	int pIndex = Room[Level].Player[Index].Index;
	//==========================================================================
	//Party Rebuilt
	//==========================================================================
	if ( gObj[pIndex].PartyNumber >= 0 && IllusionTemple.Room[Level].Start == 1)
	{
		//Remove player from party
		int iPartyIndex= gParty.GetIndex(gObj[pIndex].PartyNumber, gObj[pIndex].m_Index, gObj[pIndex].DBNumber);

		if ( iPartyIndex >= 0 )
		{
			PMSG_PARTYDELUSER pMsg;
			pMsg.Number = iPartyIndex;

			CGPartyDelUser(&pMsg, gObj[pIndex].m_Index);
		}

		//If party owner left, rebuild party if more than 1 user on the team
		if (iPartyIndex == 0 || teamcount >= 2 )
		{
			for(int j = 0; j < (ILLUSIONTEMPLE_MAX_PLAYERS*2); j++)
			{
				if (Room[Level].Player[j].Index > 0)
				{
					int pNewIndex = Room[Level].Player[j].Index;

					if(pNewIndex != pIndex)
					{
						if(Room[Level].Player[j].Team == Room[Level].Player[Index].Team)
						{
							if (NewPartyIndex == -1)
							{
								NewPartyIndex = gParty.Create(pNewIndex, gObj[pNewIndex].DBNumber, gObj[pNewIndex].Level);
								gObj[pNewIndex].PartyNumber = NewPartyIndex;
								NewPartyOwner = pNewIndex;
							}

							if (NewPartyIndex >= 0 && NewPartyOwner != pNewIndex && NewPartyIndex != -1)
							{
								int iPartyPos = gParty.Add(gObj[NewPartyOwner].PartyNumber, pNewIndex, gObj[pNewIndex].DBNumber, gObj[pNewIndex].Level);
								gObj[pNewIndex].PartyNumber = NewPartyIndex;
								gParty.Paint(NewPartyIndex);
							}
				
							if (NewPartyIndex >= 0)
							{
								CGPartyListAll(NewPartyIndex);
							}
			
							LogAddTD("[Illusion Temple][%d](%d) Party Member: %s(%d) PartyID: %d(Owner:%d)",
								Room[Level].Player[j].Team, Level+1,
								gObj[pNewIndex].Name, pNewIndex,
								gObj[pNewIndex].PartyNumber, NewPartyOwner
							);
						}
					}
				}
			}
		}
	}

	//==========================================================================
	if (Room[Level].Hero == Room[Level].Player[Index].Index &&
		Room[Level].Hero != -1 &&
		Room[Level].Player[Index].Index != 0)
	{
		int itemPos = gObjGetItemPos(Room[Level].Player[Index].Index,14,64);

		if (itemPos > -1)
		{
			DeleteItemByID(Room[Level].Player[Index].Index,ITEMGET(14,64));

			ItemSerialCreateSend(gObj[Room[Level].Player[Index].Index].m_Index, MAP_INDEX_ILLUSIONTEMPLE1+Level, Room[Level].Hero_X, Room[Level].Hero_Y, ITEMGET(14,64), 0, 255, 0, 0, 0, -1, 0, 0);
			LogAddTD("[Illusion Temple][%s][%s] TROW New Fireball on User Leave",
				gObj[Room[Level].Player[Index].Index].AccountID, gObj[Room[Level].Player[Index].Index].Name
			);
		} else {
			DeleteItemByID(Room[Level].Player[Index].Index,ITEMGET(14,64));

			ItemSerialCreateSend(gObj[Room[Level].Player[Index].Index].m_Index, MAP_INDEX_ILLUSIONTEMPLE1+Level, Room[Level].Hero_X, Room[Level].Hero_Y, ITEMGET(14,64), 0, 255, 0, 0, 0, -1, 0, 0);
			LogAddTD("[Illusion Temple][%s][%s] POS ERROR: New Drop Fireball on Leave",
				gObj[Room[Level].Player[Index].Index].AccountID, gObj[Room[Level].Player[Index].Index].Name
			);
		}
		GCServerMsgStringSendToRoom(Level,"The fireball has been dropped!",0);

		Room[Level].Hero = -1;
		Room[Level].Hero_X = 0;
		Room[Level].Hero_Y = 0;
	}

	Room[Level].Player[Index].Index = 0;
	Room[Level].Player[Index].Exp_Current = 0;
	Room[Level].Player[Index].Team = 0;
	Room[Level].Player[Index].kill_points = this->startSkillPoints;
	Room[Level].Player[Index].total_score = 0;
	Room[Level].Player[Index].FernirTime = 0;
}

int CIllusionTemple::Player_GetIndex(BYTE Level, int PlayerID)
{
	if(OBJMAX_RANGE(PlayerID) == 0)
	{
		return 0;
	}

	for ( int i=0;i<(ILLUSIONTEMPLE_MAX_PLAYERS*2);i++)
	{
		if(Room[Level].Player[i].Index == PlayerID)
		{
			if ( gObj[PlayerID].Connected == PLAYER_PLAYING &&
				 gObj[PlayerID].Type == OBJ_USER )
			{
					if (gObj[PlayerID].MapNumber >= (MAP_INDEX_ILLUSIONTEMPLE1 + Level) &&
						gObj[PlayerID].MapNumber < MAP_INDEX_ILLUSIONTEMPLE1 + MAX_ILLUSION_TEMPLE_LEVEL)
						return i;
					else
						Player_Remove(Level,i);
			}else
			{
				Player_Remove(Level,i);
			}			
		}
	}
	return -1;
}


void CIllusionTemple::Player_Check(BYTE Level)
{	
	int Map=MAP_INDEX_ILLUSIONTEMPLE1+Level;

	for(int i=0;i < (ILLUSIONTEMPLE_MAX_PLAYERS*2); i++)
	{
		int pIndex = Room[Level].Player[i].Index;
		if ( gObj[pIndex].Connected == PLAYER_PLAYING && 
			 gObj[pIndex].Type == OBJ_USER )
		{
			if (pIndex > 0)
			{
				if( gObj[pIndex].MapNumber != Map )
				{
					Player_Remove(Level,i);
				}
			}
		} else {
			if (pIndex > 0)
			{
				Player_Remove(Level,i);	
			}
		}
	}
}

BYTE CIllusionTemple::KillPointReduce(short pIndex, BYTE ammount)
{
	if(OBJMAX_RANGE(pIndex) == 0)
	{
		return 0;
	}

	int Level = -1;
	
	if ((gObj[pIndex].MapNumber >= MAP_INDEX_ILLUSIONTEMPLE1) && (gObj[pIndex].MapNumber < MAP_INDEX_ILLUSIONTEMPLE1 + MAX_ILLUSION_TEMPLE_LEVEL))
	{
		Level = gObj[pIndex].MapNumber - MAP_INDEX_ILLUSIONTEMPLE1;
	}

	if (Level == -1 || Level >= MAX_ILLUSION_TEMPLE_LEVEL)
		return 0;

	BYTE pos = Player_GetIndex(Level,pIndex);

	if (pos > -1)
	{
		Room[Level].Player[pos].kill_points -= ammount;

		PMSG_KILLPOINT_ILLUSIONTEMPLE pMsg;
		PHeadSubSetB((LPBYTE)&pMsg, 0xBF, 0x06, sizeof(pMsg));
		pMsg.Points = Room[Level].Player[pos].kill_points;
		DataSend(pIndex, (BYTE*)&pMsg, sizeof(pMsg));

		return 1;
	}
	return 0;
}

void CIllusionTemple::MonsterDieKillPoint(short killerIndex)
{
	if(OBJMAX_RANGE(killerIndex) == 0)
	{
		return;
	}

	int Level = -1;
	
	if ((gObj[killerIndex].MapNumber >= MAP_INDEX_ILLUSIONTEMPLE1) && (gObj[killerIndex].MapNumber < MAP_INDEX_ILLUSIONTEMPLE1 + MAX_ILLUSION_TEMPLE_LEVEL))
	{
		Level = gObj[killerIndex].MapNumber - MAP_INDEX_ILLUSIONTEMPLE1;
	}

	if (Level == -1 || Level >= MAX_ILLUSION_TEMPLE_LEVEL)
		return;

	BYTE killer_pos = Player_GetIndex(Level,killerIndex);
	BYTE killer_team = 0;	

	if (killer_pos > -1)
	{
		Room[Level].Player[killer_pos].kill_points = (Room[Level].Player[killer_pos].kill_points + this->pointsPerMobKill)&255;
		Room[Level].Player[killer_pos].total_score += this->MobKillRankExp;

		PMSG_KILLPOINT_ILLUSIONTEMPLE pMsg;
		PHeadSubSetB((LPBYTE)&pMsg, 0xBF, 0x06, sizeof(pMsg));
		pMsg.Points = Room[Level].Player[killer_pos].kill_points;
		DataSend(killerIndex, (BYTE*)&pMsg, sizeof(pMsg));
	}
}

void CIllusionTemple::PlayerDieKillPoint(short dieIndex, short killerIndex)
{
	if(OBJMAX_RANGE(dieIndex) == 0)
	{
		return;
	}
	if(OBJMAX_RANGE(killerIndex) == 0)
	{
		return;
	}

	int Level = -1;
	
	if ((gObj[dieIndex].MapNumber >= MAP_INDEX_ILLUSIONTEMPLE1) && (gObj[dieIndex].MapNumber < MAP_INDEX_ILLUSIONTEMPLE1 + MAX_ILLUSION_TEMPLE_LEVEL))
	{
		Level = gObj[dieIndex].MapNumber - MAP_INDEX_ILLUSIONTEMPLE1;
	}

	if (Level == -1 || Level >= MAX_ILLUSION_TEMPLE_LEVEL)
		return;

	if(this->ExpForKill == 1)
		gObjAddSingleExp(&gObj[dieIndex],&gObj[killerIndex]);

	BYTE dead_pos = Player_GetIndex(Level,dieIndex);
	BYTE dead_team = 0;	
	BYTE killer_pos = Player_GetIndex(Level,killerIndex);
	BYTE killer_team = 0;	

	if (dead_pos > -1)
		dead_team = Room[Level].Player[dead_pos].Team;
	if (killer_pos > -1)
		killer_team = Room[Level].Player[killer_pos].Team;

	if (Room[Level].Hero == dieIndex &&
		Room[Level].Hero != -1 &&
		dieIndex != 0)
	{
		DeleteItemByID(dieIndex,ITEMGET(14,64));

		ItemSerialCreateSend(gObj[dieIndex].m_Index, MAP_INDEX_ILLUSIONTEMPLE1+Level, Room[Level].Hero_X, Room[Level].Hero_Y, ITEMGET(14,64), 0, 255, 0, 0, 0, -1, 0, 0);
		LogAddTD("[Illusion Temple][%s][%s] TROW New Fireball on User Die",
			gObj[dieIndex].AccountID, gObj[dieIndex].Name
		);
		GCServerMsgStringSendToRoom(Level,"The fireball has been dropped!",0);

		Room[Level].Hero = -1;
		Room[Level].Hero_X = 0;
		Room[Level].Hero_Y = 0;
	}

	if (dead_team != killer_team && killer_team > 0)
	{
		Room[Level].Player[killer_pos].kill_points = (Room[Level].Player[killer_pos].kill_points + this->pointsPerPlayerKill)&255;
		Room[Level].Player[killer_pos].total_score += this->PlayerKillRankExp;

		PMSG_KILLPOINT_ILLUSIONTEMPLE pMsg;
		PHeadSubSetB((LPBYTE)&pMsg, 0xBF, 0x06, sizeof(pMsg));
		pMsg.Points = Room[Level].Player[killer_pos].kill_points;
		DataSend(killerIndex, (BYTE*)&pMsg, sizeof(pMsg));
	}
}

int CIllusionTemple::PlayerDieRespawn(short iIndex, short& ox, short& oy)
{
	if(OBJMAX_RANGE(iIndex) == 0)
	{
		return -1;
	}

	BYTE BlueX=196;
	BYTE BlueY=126;
	BYTE OrangeX=143;
	BYTE OrangeY=49;
	BYTE x = rand()%3 + rand()%2;
	BYTE y = rand()%2 + rand()%1;

	int Level = -1;
	
	if ((gObj[iIndex].MapNumber >= MAP_INDEX_ILLUSIONTEMPLE1) && (gObj[iIndex].MapNumber < MAP_INDEX_ILLUSIONTEMPLE1 + MAX_ILLUSION_TEMPLE_LEVEL))
	{
		Level = gObj[iIndex].MapNumber - MAP_INDEX_ILLUSIONTEMPLE1;
	} else {
		LogAddTD("[Illusion Temple][%s][%s] ERROR Player map data [%d][%d][%d]",
			gObj[iIndex].AccountID, gObj[iIndex].Name,
			gObj[iIndex].MapNumber, ox, oy
		);
		return -1;
	}

	if (Level == -1)
		return -1;

	//If some illusion skill is active - turn it off
	if (gObj[iIndex].m_SkillIT_Number > 0)
	{
		switch(gObj[iIndex].m_SkillIT_Number)
		{
			case IL_ORDER_OF_PROTECT:
			{
				GCSkillInfoSend(&gObj[iIndex],0,34);
			}break;
			case IL_ORDER_OF_FREEZE:
			{
				GCSkillInfoSend(&gObj[iIndex],0,65);
			}break;
		}

		SkillEffectEnd(gObj[iIndex].m_Index, gObj[iIndex].m_SkillIT_Number);

		gObj[iIndex].m_SkillIT_Time = 0;
		gObj[iIndex].m_SkillIT_Number = 0;
	}

	int pos = Player_GetIndex(Level,iIndex);

	if(Room[Level].Player[pos].Team == ILLUSIONTEMPLE_TEAM_BLUE)
	{
		ox = BlueX+x;
		oy = BlueY+y;

		gObj[iIndex].m_Change = 405;

		gObj[iIndex].X = BlueX+x;
		gObj[iIndex].Y = BlueY+y;

		LogAddTD("[Illusion Temple][%s][%s] TEAM BLUE User Respawn [%d] [%d][%d]-[%d][%d]",
			gObj[iIndex].AccountID, gObj[iIndex].Name,
			gObj[iIndex].MapNumber, gObj[iIndex].X, gObj[iIndex].Y,
			ox,oy
		);
	}
	else if(Room[Level].Player[pos].Team == ILLUSIONTEMPLE_TEAM_ORANGE)
	{
		ox = OrangeX+x;
		oy = OrangeY+y;

		gObj[iIndex].m_Change = 404;

		gObj[iIndex].X = OrangeX+x;
		gObj[iIndex].Y = OrangeY+y;

		LogAddTD("[Illusion Temple][%s][%s] TEAM ORANGE User Respawn [%d] [%d][%d]-[%d][%d]",
			gObj[iIndex].AccountID, gObj[iIndex].Name,
			gObj[iIndex].MapNumber, gObj[iIndex].X, gObj[iIndex].Y,
			ox,oy
		);
	}

	gObj[iIndex].m_SkillIT_Time = 15;
	gObj[iIndex].m_SkillIT_Number = IL_ORDER_OF_PROTECT;

	//GCMagicAttackNumberSend(&gObj[PlayerID],IL_ORDER_OF_PROTECT,TargetID,1);
	GCSkillInfoSend(&gObj[iIndex],1,34);
	SkillEffectStart(iIndex, iIndex, IL_ORDER_OF_PROTECT, 1);

	GCItemListSend(iIndex);

	return 1;
}

void CIllusionTemple::SetMonster(BYTE Level)
{
	BYTE Map = Level + MAP_INDEX_ILLUSIONTEMPLE1;
	if(IT_MAP_RANGE(Map) == FALSE)
		return;

	for ( int n=0;n<gMSetBase.m_Count;n++)
	{
		if ( gMSetBase.m_Mp[n].m_MapNumber == Map  )
		{
			if(gMSetBase.m_Mp[n].m_Type < 380 || gMSetBase.m_Mp[n].m_Type > 384)
			{
				WORD btIndex = gMSetBase.m_Mp[n].m_Type;
				BYTE btMap   = gMSetBase.m_Mp[n].m_MapNumber;

				int result = gObjAddMonster(gMSetBase.m_Mp[n].m_MapNumber);

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
					gObjSetMonster(result, btIndex,"CIllusionTemple::SetMonster");
					gObj[result].Dir = rand() % 8;
				}
			}
		}
	}
}

void CIllusionTemple::ClearMonster(BYTE Level)
{
	BYTE Map = Level + MAP_INDEX_ILLUSIONTEMPLE1;
	if(IT_MAP_RANGE(Map) == FALSE)
		return;

	for ( int n=0;n<OBJ_MAXMONSTER;n++)
	{
		if ( gObj[n].MapNumber == Map)
		{
			if(gObj[n].Class < 380 || gObj[n].Class > 384)
				gObjDel(n);
		}
	}
}

