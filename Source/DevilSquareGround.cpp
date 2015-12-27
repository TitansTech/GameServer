#include "stdafx.h"
#include "DevilSquareGround.h"
#include "DevilSquare.h"
#include "GameMain.h"
#include "Logproc.h"
#include "..\common\winutil.h"
#include "CrywolfSync.h"
#include "Crywolf.h"
#include "VIPSystem.h"
#include "RProtocol.h"
// GS-N 0.99.60T - 0x00457190 
// GS-N	1.00.18	JPN	0x004667C0	-	Completed


bool DevilSquareScoreSort(LPOBJ const & lhs,LPOBJ const & rhs)
{
	if ( lhs->m_nEventScore == rhs->m_nEventScore )
	{
		if ( lhs->Level < rhs->Level)
		{
			return true;
		}

		return false;
	}

	if ( lhs->m_nEventScore > rhs->m_nEventScore)
	{
		return true;
	}

	return false;
}



CDevilSquareGround::CDevilSquareGround()
{
	this->Init(0);
}




CDevilSquareGround::~CDevilSquareGround()
{
	this->m_BonusScoreTable[CLASS_WIZARD][0] = 0;
	this->m_BonusScoreTable[CLASS_WIZARD][1] = 0;
	this->m_BonusScoreTable[CLASS_WIZARD][2] = 0;
	this->m_BonusScoreTable[CLASS_WIZARD][3] = 170;
	this->m_BonusScoreTable[CLASS_WIZARD][4] = 170;
	this->m_BonusScoreTable[CLASS_WIZARD][5] = 170;
	this->m_BonusScoreTable[CLASS_WIZARD][6] = 170;
	
	this->m_BonusScoreTable[CLASS_KNIGHT][0] = 20;
	this->m_BonusScoreTable[CLASS_KNIGHT][1] = 90;
	this->m_BonusScoreTable[CLASS_KNIGHT][2] = 120;
	this->m_BonusScoreTable[CLASS_KNIGHT][3] = 400;
	this->m_BonusScoreTable[CLASS_KNIGHT][4] = 400;
	this->m_BonusScoreTable[CLASS_KNIGHT][5] = 400;
	this->m_BonusScoreTable[CLASS_KNIGHT][6] = 400;

	this->m_BonusScoreTable[CLASS_ELF][0] = 10;
	this->m_BonusScoreTable[CLASS_ELF][1] = 10;
	this->m_BonusScoreTable[CLASS_ELF][2] = 10;
	this->m_BonusScoreTable[CLASS_ELF][3] = 200;
	this->m_BonusScoreTable[CLASS_ELF][4] = 200;
	this->m_BonusScoreTable[CLASS_ELF][5] = 200;
	this->m_BonusScoreTable[CLASS_ELF][6] = 200;

	this->m_BonusScoreTable[CLASS_MAGICGLADIATOR][0] = 0;
	this->m_BonusScoreTable[CLASS_MAGICGLADIATOR][1] = 0;
	this->m_BonusScoreTable[CLASS_MAGICGLADIATOR][2] = 0;
	this->m_BonusScoreTable[CLASS_MAGICGLADIATOR][3] = 0;
	this->m_BonusScoreTable[CLASS_MAGICGLADIATOR][4] = 0;
	this->m_BonusScoreTable[CLASS_MAGICGLADIATOR][5] = 0;
	this->m_BonusScoreTable[CLASS_MAGICGLADIATOR][6] = 0;

	this->m_BonusScoreTable[CLASS_DARKLORD][0] = 0;
	this->m_BonusScoreTable[CLASS_DARKLORD][1] = 0;
	this->m_BonusScoreTable[CLASS_DARKLORD][2] = 0;
	this->m_BonusScoreTable[CLASS_DARKLORD][3] = 0;
	this->m_BonusScoreTable[CLASS_DARKLORD][4] = 0;
	this->m_BonusScoreTable[CLASS_DARKLORD][5] = 0;
	this->m_BonusScoreTable[CLASS_DARKLORD][6] = 0;

	this->m_BonusScoreTable[CLASS_SUMMONER][0] = 0;
	this->m_BonusScoreTable[CLASS_SUMMONER][1] = 0;
	this->m_BonusScoreTable[CLASS_SUMMONER][2] = 0;
	this->m_BonusScoreTable[CLASS_SUMMONER][3] = 0;
	this->m_BonusScoreTable[CLASS_SUMMONER][4] = 0;
	this->m_BonusScoreTable[CLASS_SUMMONER][5] = 0;
	this->m_BonusScoreTable[CLASS_SUMMONER][6] = 0;

	this->m_BonusScoreTable[CLASS_RAGEFIGHTER][0] = 0;
	this->m_BonusScoreTable[CLASS_RAGEFIGHTER][1] = 0;
	this->m_BonusScoreTable[CLASS_RAGEFIGHTER][2] = 0;
	this->m_BonusScoreTable[CLASS_RAGEFIGHTER][3] = 0;
	this->m_BonusScoreTable[CLASS_RAGEFIGHTER][4] = 0;
	this->m_BonusScoreTable[CLASS_RAGEFIGHTER][5] = 0;
	this->m_BonusScoreTable[CLASS_RAGEFIGHTER][6] = 0;
}




void CDevilSquareGround::Init(int Index)
{
	this->m_dwObjCount = 0;
	this->m_iIndex = Index;
	memset(this->m_DevilSquareMonsterInfo, 0, sizeof(this->m_DevilSquareMonsterInfo));
	memset(this->m_DevilSquareBossMonsterInfo, 0, sizeof(this->m_DevilSquareBossMonsterInfo));

	for (int i = 0; i < MAX_ST_DEVILSQUARE_MONSTER ; i++)
	{
		this->m_DevilSquareMonsterInfo[i].m_Type = -1;
	}
	for (int i = 0; i < MAX_ST_DEVILSQUARE_MONSTER ; i++)
	{
		this->m_DevilSquareBossMonsterInfo[i].m_Type = -1;
	}
}




void CDevilSquareGround::Clear()
{
	this->m_dwObjCount = 0;
}





void CDevilSquareGround::Set(WORD type, int starttime, int endtime)
{
	for (int i=0;i<MAX_ST_DEVILSQUARE_MONSTER;i++)
	{
		if ( this->m_DevilSquareMonsterInfo[i].m_Type == -1  )
		{
			this->m_DevilSquareMonsterInfo[i].m_Type = type;
			this->m_DevilSquareMonsterInfo[i].m_RegenStartTime = starttime;
			this->m_DevilSquareMonsterInfo[i].m_RegenEndTime = endtime;
			this->m_DevilSquareMonsterInfo[i].m_iDevilSquareNumber = this->m_iIndex;

			LogAddTD("[DevilSquare][%d][Mob] Loaded ID:%d S:%d E:%d", 
				this->m_DevilSquareMonsterInfo[i].m_iDevilSquareNumber,
				this->m_DevilSquareMonsterInfo[i].m_Type,
				this->m_DevilSquareMonsterInfo[i].m_RegenStartTime,
				this->m_DevilSquareMonsterInfo[i].m_iDevilSquareNumber
			);

			return;
		}
	}
}





void CDevilSquareGround::SetBoss(WORD type, int starttime, int x, int y, int tx, int ty)
{
	for (int i=0;i<MAX_ST_DEVILSQUARE_MONSTER;i++)
	{
		if ( this->m_DevilSquareBossMonsterInfo[i].m_Type == -1  )
		{
			this->m_DevilSquareBossMonsterInfo[i].m_Type = type;
			this->m_DevilSquareBossMonsterInfo[i].m_RegenStartTime = starttime;
			this->m_DevilSquareBossMonsterInfo[i].X = x;
			this->m_DevilSquareBossMonsterInfo[i].Y = y;
			this->m_DevilSquareBossMonsterInfo[i].TX = tx;
			this->m_DevilSquareBossMonsterInfo[i].TY = ty;
			this->m_DevilSquareBossMonsterInfo[i].m_iDevilSquareNumber = this->m_iIndex;

			LogAddTD("[DevilSquare][%d][Boss] Loaded ID:%d S:%d XY:%d-%d/%d-%d", 
				this->m_DevilSquareBossMonsterInfo[i].m_iDevilSquareNumber,
				this->m_DevilSquareBossMonsterInfo[i].m_Type,
				this->m_DevilSquareBossMonsterInfo[i].m_RegenStartTime,
				this->m_DevilSquareBossMonsterInfo[i].X, this->m_DevilSquareBossMonsterInfo[i].TX,
				this->m_DevilSquareBossMonsterInfo[i].Y, this->m_DevilSquareBossMonsterInfo[i].TY
			);
			
			return;
		}
	}
}





void CDevilSquareGround::SetBonus(int index, int zen, int exp)
{
	this->m_Bonus[index][0] = exp;
	this->m_Bonus[index][1] = zen;
}



void CDevilSquareGround::IncObjCount()
{
	InterlockedIncrement((LPLONG)&this->m_dwObjCount);
}




long  CDevilSquareGround::GetObjCount()
{
	return this->m_dwObjCount;
}





void CDevilSquareGround::RegenBossMonster(int currtime)
{
	for ( int i=0;i<MAX_ST_DEVILSQUARE_MONSTER;i++)
	{
		if ( this->m_DevilSquareBossMonsterInfo[i].m_Type != -1 )
		{
			if ( currtime == this->m_DevilSquareBossMonsterInfo[i].m_RegenStartTime )
			{
				int iMapNumber = (this->m_DevilSquareBossMonsterInfo[i].m_iDevilSquareNumber >= DEVIL_SQUARE_GROUND_1&& this->m_DevilSquareBossMonsterInfo[i].m_iDevilSquareNumber <= DEVIL_SQUARE_GROUND_4)?MAP_INDEX_DEVILSQUARE:(this->m_DevilSquareBossMonsterInfo[i].m_iDevilSquareNumber>=DEVIL_SQUARE_GROUND_5  && this->m_DevilSquareBossMonsterInfo[i].m_iDevilSquareNumber <= DEVIL_SQUARE_GROUND_7 )?MAP_INDEX_DEVILSQUARE2:-1;

				int iMonIndex = gObjAddMonster(iMapNumber);

				if ( iMonIndex >= 0 )
				{
					gObjSetMonster(iMonIndex, this->m_DevilSquareBossMonsterInfo[i].m_Type,"CDevilSquareGround::RegenBossMonster");
					gObj[iMonIndex].MapNumber = iMapNumber;
					gMSetBase.GetBoxPosition(gObj[iMonIndex].MapNumber,
						this->m_DevilSquareBossMonsterInfo[i].X, this->m_DevilSquareBossMonsterInfo[i].Y,
						this->m_DevilSquareBossMonsterInfo[i].TX, this->m_DevilSquareBossMonsterInfo[i].TY,
						gObj[iMonIndex].X, gObj[iMonIndex].Y);
					int map = gObj[iMonIndex].MapNumber;
					int x = gObj[iMonIndex].X;
					int y = gObj[iMonIndex].Y;

					LogAddTD("[DevilSquare][%d][Boss] ID:%d Location: %d [%d,%d] create ", 
						this->m_DevilSquareBossMonsterInfo[i].m_iDevilSquareNumber,
						this->m_DevilSquareBossMonsterInfo[i].m_Type,
						map, x, y
					);

					gObj[iMonIndex].TX = gObj[iMonIndex].X;
					gObj[iMonIndex].TY = gObj[iMonIndex].Y;
					gObj[iMonIndex].MTX = gObj[iMonIndex].X;
					gObj[iMonIndex].MTY = gObj[iMonIndex].Y;
					gObj[iMonIndex].StartX = gObj[iMonIndex].X;
					gObj[iMonIndex].StartY = gObj[iMonIndex].Y;
						//gObj[iMonIndex].m_OldX = gObj[iMonIndex].X;
						//gObj[iMonIndex].m_OldY = gObj[iMonIndex].Y;
					
					gObj[iMonIndex].m_bDevilSquareIndex = this->m_iIndex;
					gObj[iMonIndex].DieRegen = FALSE;
					gObj[iMonIndex].m_PosNum = -1;
					gObj[iMonIndex].Live = TRUE;
					gObj[iMonIndex].m_State = 1;
				}
			}
		}
	}
}






BOOL CDevilSquareGround::FindMonsterType(WORD type)
{
	for ( int i=0;i<MAX_ST_DEVILSQUARE_MONSTER;i++)
	{
		if ( this->m_DevilSquareMonsterInfo[i].m_Type == type )
		{
			return TRUE;
		}
	}

	return FALSE;
}




WORD CDevilSquareGround::GetMonsterType(int currtime)
{
	WORD sTemp[MAX_ST_DEVILSQUARE_MONSTER];
	int icount = 0;

	for ( int i=0;i<MAX_ST_DEVILSQUARE_MONSTER;i++)
	{
		if ( this->m_DevilSquareMonsterInfo[i].m_Type != -1 )
		{
			if ( currtime >= this->m_DevilSquareMonsterInfo[i].m_RegenStartTime )
			{
				if ( currtime <= this->m_DevilSquareMonsterInfo[i].m_RegenEndTime )
				{
					sTemp[icount] = this->m_DevilSquareMonsterInfo[i].m_Type;
					icount++;
				}
			}
		}
	}

	if ( icount == 0 )
	{
		return -1;
	}

	return sTemp[rand()%icount];
}






void CDevilSquareGround::ClearScore()
{
	this->m_DevilSquareRankList.clear();
}





void CDevilSquareGround::SortScore()
{
	std::sort( this->m_DevilSquareRankList.begin(), this->m_DevilSquareRankList.end(), DevilSquareScoreSort);
}





void CDevilSquareGround::InsertObj(LPOBJ lpObj)
{
	if ( lpObj->Authority != 2 )
	{
		if ( lpObj->Class >= 0 && lpObj->Class < MAX_TYPE_PLAYER)
		{
			if ( DS_LEVEL_RANGE(lpObj->m_bDevilSquareIndex) != FALSE )
			{
				lpObj->m_nEventScore +=  this->m_BonusScoreTable[lpObj->Class][lpObj->m_bDevilSquareIndex] / 100;
			}
		}

		this->m_DevilSquareRankList.push_back(lpObj);
	}
}




void CDevilSquareGround::SendScore()
{
	if ( this->m_DevilSquareRankList.size() < 1 )
	{
		return;
	}

	BYTE count = 1;
	int iUserCount = this->m_DevilSquareRankList.size();
	int iExp = 0;
	std::vector<OBJECTSTRUCT *>::iterator Itor;

	for (Itor = this->m_DevilSquareRankList.begin() ; Itor != this->m_DevilSquareRankList.end() ; Itor++ )
	{
		memcpy(this->m_DevilSquareScoreInfoTOP10.Score[count].Name , (*(Itor))->Name, MAX_ACCOUNT_LEN);
		this->m_DevilSquareScoreInfoTOP10.Score[count].TotalScore = (*(Itor))->m_nEventScore;

		if ( iUserCount <= 7 )//6
		{
			if ( count < 4 )	// For DS from 0 to 3
			{
				this->m_DevilSquareScoreInfoTOP10.Score[count].BonusZen = this->m_Bonus[count-1][0];
				this->m_DevilSquareScoreInfoTOP10.Score[count].BonusExp = this->m_Bonus[count-1][1];
			}
			else
			{
				this->m_DevilSquareScoreInfoTOP10.Score[count].BonusZen = this->m_Bonus[3][0];
				this->m_DevilSquareScoreInfoTOP10.Score[count].BonusExp = this->m_Bonus[3][1];
			}
		}
		else
		{
			int lc5 = count*100/iUserCount;

			if ( count == 1 )
			{
				this->m_DevilSquareScoreInfoTOP10.Score[count].BonusZen = this->m_Bonus[0][0];
				this->m_DevilSquareScoreInfoTOP10.Score[count].BonusExp = this->m_Bonus[0][1];
			}
			else if ( lc5 <= 30 )
			{
				this->m_DevilSquareScoreInfoTOP10.Score[count].BonusZen = this->m_Bonus[1][0];
				this->m_DevilSquareScoreInfoTOP10.Score[count].BonusExp = this->m_Bonus[1][1];
			}
			else if ( lc5 <= 50 )
			{
				this->m_DevilSquareScoreInfoTOP10.Score[count].BonusZen = this->m_Bonus[2][0];
				this->m_DevilSquareScoreInfoTOP10.Score[count].BonusExp = this->m_Bonus[2][1];
			}
			else
			{
				this->m_DevilSquareScoreInfoTOP10.Score[count].BonusZen = this->m_Bonus[3][0];
				this->m_DevilSquareScoreInfoTOP10.Score[count].BonusExp = this->m_Bonus[3][1];
			}
		}

		count++;

		if ( count >= 10 )
		{
			break;
		}
	}

	this->m_DevilSquareScoreInfoTOP10.Count  = count;
	int iSize = count * 24 + 5;

	PHeadSetB((LPBYTE)&this->m_DevilSquareScoreInfoTOP10, 0x93, iSize);
	count = 1;

	Itor = this->m_DevilSquareRankList.begin();

	LogAddTD("[DevilSquare] Rank [%d]", this->m_iIndex);

	for ( ; Itor != this->m_DevilSquareRankList.end(); Itor++ )
	{
		if ( (*(Itor))->Type != OBJ_USER )
			continue;

		if ( iUserCount <= 7 )//6
		{
			if ( count < 3 )
			{
				this->m_DevilSquareScoreInfoTOP10.Score[0].BonusZen = this->m_Bonus[count-1][0];
				this->m_DevilSquareScoreInfoTOP10.Score[0].BonusExp = this->m_Bonus[count-1][1];
			}
			else
			{
				this->m_DevilSquareScoreInfoTOP10.Score[0].BonusZen = this->m_Bonus[3][0];
				this->m_DevilSquareScoreInfoTOP10.Score[0].BonusExp = this->m_Bonus[3][1];
			}
		}
		else
		{
			int lc7 = count*100/iUserCount;

			if ( count ==1 )
			{
				this->m_DevilSquareScoreInfoTOP10.Score[0].BonusZen = this->m_Bonus[0][0];
				this->m_DevilSquareScoreInfoTOP10.Score[0].BonusExp = this->m_Bonus[0][1];
			}
			else if ( lc7 <= 30 )
			{
				this->m_DevilSquareScoreInfoTOP10.Score[0].BonusZen = this->m_Bonus[1][0];
				this->m_DevilSquareScoreInfoTOP10.Score[0].BonusExp = this->m_Bonus[1][1];
			}
			else if ( lc7 <= 50 )
			{
				this->m_DevilSquareScoreInfoTOP10.Score[0].BonusZen = this->m_Bonus[2][0];
				this->m_DevilSquareScoreInfoTOP10.Score[0].BonusExp = this->m_Bonus[2][1];
			}
			else
			{
				this->m_DevilSquareScoreInfoTOP10.Score[0].BonusZen = this->m_Bonus[3][0];
				this->m_DevilSquareScoreInfoTOP10.Score[0].BonusExp = this->m_Bonus[3][1];
			}
		}

		if ( g_CrywolfSync.GetOccupationState() == CRYWOLF_OCCUPATION_FAILED && g_iCrywolfApplyMvpPenalty != FALSE)
		{
#if (PACK_EDITION>=2)
			if ((VipSystem.VipIsApplyCWExpPenalty == 0)&&((*(Itor))->Vip >= 1))
			{
			} else {
				this->m_DevilSquareScoreInfoTOP10.Score[0].BonusExp =  (this->m_DevilSquareScoreInfoTOP10.Score[0].BonusExp * g_CrywolfSync.GetGettingExpPenaltyRate()) / 100;
			}
#else
			this->m_DevilSquareScoreInfoTOP10.Score[0].BonusExp =  (this->m_DevilSquareScoreInfoTOP10.Score[0].BonusExp * g_CrywolfSync.GetGettingExpPenaltyRate()) / 100;
#endif
		}

		if ( ((*(Itor))->m_wExprienceRate + (*(Itor))->MasterCharacterInfo->IncExperience) == 0 )
			iExp = 0;
		else
		{
			if ((*(Itor))->m_btDisableExpGain == 0 )
			{
				iExp = this->m_DevilSquareScoreInfoTOP10.Score[0].BonusExp;
				(*(Itor))->Experience += iExp;
			}
		}

		//(*(Itor))->Experience += this->m_DevilSquareScoreInfoTOP10.Score[0].BonusExp;
		int ret = -1;
		int LevelUp = 0;

		if ( ((*(Itor))->m_wExprienceRate + (*(Itor))->MasterCharacterInfo->IncExperience) > 0 && (*(Itor))->m_btDisableExpGain == 0 )
		{
			gObjSetExpPetItem((*(Itor))->m_Index, iExp);

			int iMAX_LEVCOUNT = 0;
			ret = iExp;

			while ( ret > 0 )
			{
				if ( ret > 0 )
				{
					ret = gObjLevelUp((*(Itor)), ret, 0, EVENT_TYPE_DEVILSQUARE, LevelUp);
				}

				iMAX_LEVCOUNT++;
				if (iMAX_LEVCOUNT > 5)
					break;
			}

			if ((ret < 0)&&(iMAX_LEVCOUNT > 1))
				ret=0;
		}

		(*(Itor))->m_nEventMoney = this->m_DevilSquareScoreInfoTOP10.Score[0].BonusZen;

		if ( (__int64)((*(Itor))->m_Index + (*(Itor))->m_nEventMoney) > (__int64)MAX_ZEN )
		{
			int Zen = MAX_ZEN - (*(Itor))->Money;
			(*(Itor))->Money += Zen;
		}
		else
		{
			(*(Itor))->Money += (*(Itor))->m_nEventMoney;
		}

		GCMoneySend((*(Itor))->m_Index, (*(Itor))->Money);

		if ( ret >= 0 )
		{
			if (  ((*(Itor))->m_wExprienceRate + (*(Itor))->MasterCharacterInfo->IncExperience) > 0 && (*(Itor))->m_btDisableExpGain == 0 )
			{
				if(LevelUp == 0)
					GCKillPlayerExpSend((*(Itor))->m_Index, (WORD)-1, iExp, 0, 0);
			}
		}

		this->SendRankingInfo((*(Itor)));

		LogAddTD("Rank :[%d] : [%s][%s][%d][%d][%d]", count, (*(Itor))->AccountID, (*(Itor))->Name, (*(Itor))->m_nEventMoney, (*(Itor))->m_nEventExp, (*(Itor))->m_nEventScore);
		memcpy(this->m_DevilSquareScoreInfoTOP10.Score[0].Name , (*(Itor))->Name, MAX_ACCOUNT_LEN);
		this->m_DevilSquareScoreInfoTOP10.Score[0].TotalScore = (*(Itor))->m_nEventScore;

		if ( ((*(Itor))->m_wExprienceRate + (*(Itor))->MasterCharacterInfo->IncExperience) == 0 || (*(Itor))->m_btDisableExpGain == 1 )
		{
			this->m_DevilSquareScoreInfoTOP10.Score[0].BonusExp = 0;
		}
		else
		{
			this->m_DevilSquareScoreInfoTOP10.Score[0].BonusExp = iExp;
		}

		this->m_DevilSquareScoreInfoTOP10.MyRank = count;
		count++;

		DataSend((*(Itor))->m_Index, (BYTE *)&this->m_DevilSquareScoreInfoTOP10 , iSize);

		(*(Itor))->m_nEventScore = 0;
		(*(Itor))->m_nEventMoney = 0;
		(*(Itor))->m_nEventExp = 0;
	}
}






void CDevilSquareGround::SendRankingInfo(LPOBJ lpObj)
{	
	if(ReadConfig.SCFRSON == FALSE)
	{
		if ( lpObj->m_nEventScore <= 0 )
		{
			return;
		}

		PMSG_ANS_EVENTUSERSCORE pMsg;

		pMsg.h.c = 0xC1;
		pMsg.h.headcode = 0x01;
		pMsg.h.size = sizeof(pMsg);
		pMsg.Score = lpObj->m_nEventScore;
		pMsg.SquareNum = lpObj->m_bDevilSquareIndex;
		pMsg.Class = lpObj->Class;
		pMsg.ServerCode = gGameServerCode;
		memcpy(pMsg.AccountID, lpObj->AccountID, MAX_ACCOUNT_LEN);
		memcpy(pMsg.GameID, lpObj->Name, MAX_ACCOUNT_LEN);

		DataSendRank((char *)&pMsg, pMsg.h.size);
	}else
	{
		DS_SendRankingInfo(lpObj->m_Index);
	}
}
