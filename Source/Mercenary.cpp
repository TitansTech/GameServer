#include "stdafx.h"
#include "Mercenary.h"
#include "protocol.h"
#include "GameMain.h"
#include "LogProc.h"
#include "CastleSiege.h"
// GS-N 0.99.60T 0x0051A200
// GS-N	1.00.18	JPN	0x0054BB70	-	Completed

#if (GS_CASTLE==1)
CMercenary g_CsNPC_Mercenary;


CMercenary::CMercenary()
{
	this->m_iMercenaryCount = 0;
}

CMercenary::~CMercenary()
{
	return;
}


//----- (00563760) --------------------------------------------------------
BOOL CMercenary::CreateMercenary(int iIndex, int iMercenaryTypeIndex, BYTE cTX, BYTE cTY)
{
#if (GS_CASTLE==1)
	LPOBJ lpObj = &gObj[iIndex];
	int iMonsterIndex=-1;

	BYTE btMapAttr = MapC[lpObj->MapNumber].GetAttr(cTX, cTY);
	if ( lpObj->MapNumber != MAP_INDEX_CASTLESIEGE )
	{
		GCServerMsgStringSend(lMsg.Get(MSGGET(6, 91)), lpObj->m_Index, 1);
		return FALSE;
	}

	if ( g_CastleSiege.GetCastleState() != 7 )
	{
		GCServerMsgStringSend(lMsg.Get(MSGGET(6, 94)), lpObj->m_Index, 1);
		return FALSE;
	}

	if ( iMercenaryTypeIndex != 286 && iMercenaryTypeIndex != 287 )
		goto SKIP_CHK;

	if ( lpObj->m_btCsJoinSide != 1 )
	{
		GCServerMsgStringSend(lMsg.Get(MSGGET(6, 92)), lpObj->m_Index, 1);
		return FALSE;
	}
	
	if ( lpObj->GuildStatus != GUILD_MASTER && lpObj->GuildStatus != GUILD_ASSISTANT )
	{
		GCServerMsgStringSend(lMsg.Get(MSGGET(6, 93)), lpObj->m_Index, 1);
		return FALSE;
	}

SKIP_CHK:
	if ( this->m_iMercenaryCount > 100 )
	{
		GCServerMsgStringSend(lMsg.Get(MSGGET(6, 95)), lpObj->m_Index, 1);
		return FALSE;
	}

	iMonsterIndex = gObjAddMonster(lpObj->MapNumber);
	if ( iMonsterIndex < 0 )
	{
		GCServerMsgStringSend(lMsg.Get(MSGGET(6, 97)), lpObj->m_Index, 1);
		return FALSE;
	}

	LPMONSTER_ATTRIBUTE lpMATTR = gMAttr.GetAttr(iMercenaryTypeIndex);
	if ( !lpMATTR )
	{
		gObjDel(iMonsterIndex);
		LogAddTD("[CastleSiege] [%s][%s]  Mercenary Vanished (NextMap) - lpMATTR == NULL (SummonIndex:%d)",
			lpObj->AccountID, lpObj->Name, iMonsterIndex);
		return FALSE;
	}

	gObjSetMonster(iMonsterIndex, iMercenaryTypeIndex,"CMercenary::CreateMercenary");

	gObj[iMonsterIndex].Live = TRUE;
	gObj[iMonsterIndex].Life = lpMATTR->m_Hp;
	gObj[iMonsterIndex].MaxLife = lpMATTR->m_Hp;
	gObj[iMonsterIndex].m_PosNum = -1;
	gObj[iMonsterIndex].X = cTX;
	gObj[iMonsterIndex].Y = cTY;
	gObj[iMonsterIndex].MTX = cTX;
	gObj[iMonsterIndex].MTY = cTY;
	gObj[iMonsterIndex].TX = cTX;
	gObj[iMonsterIndex].TY = cTY;
	gObj[iMonsterIndex].m_OldX = cTX;
	gObj[iMonsterIndex].m_OldY = cTY;
	gObj[iMonsterIndex].StartX = cTX;
	gObj[iMonsterIndex].StartY = cTY;
	gObj[iMonsterIndex].MapNumber = lpObj->MapNumber;
	gObj[iMonsterIndex].m_MoveRange = 0;
	gObj[iMonsterIndex].Level = lpMATTR->m_Level;		//*(_WORD *)(lpMATTR + 28) lpMATTR->m_Hp
	gObj[iMonsterIndex].Type = OBJ_MONSTER;				//2
	gObj[iMonsterIndex].MaxRegenTime = 1000;
	gObj[iMonsterIndex].Dir = 1;
	gObj[iMonsterIndex].RegenTime = GetTickCount();
	gObj[iMonsterIndex].m_Attribute = 0;
	gObj[iMonsterIndex].DieRegen = 0;
	gObj[iMonsterIndex].m_btCsNpcType = 2;
	gObj[iMonsterIndex].m_btCsJoinSide = 1;

	GCServerMsgStringSend(lMsg.Get(MSGGET(6, 96)), lpObj->m_Index, 1);

	this->m_iMercenaryCount += 1;

	if ( lpObj->lpGuild )
		LogAddTD("[CastleSiege] Mercenary is summoned [%d] - [%d][%d] [%s][%s][%d] - (Guild : %s)", gObj[iMonsterIndex].Class,gObj[iMonsterIndex].X,gObj[iMonsterIndex].Y,lpObj->AccountID,lpObj->Name,lpObj->MapNumber,lpObj->lpGuild->Name);
	else
		LogAddTD("[CastleSiege] Mercenary is summoned [%d] - [%d][%d] [%s][%s][%d]", gObj[iMonsterIndex].Class,gObj[iMonsterIndex].X,gObj[iMonsterIndex].Y,lpObj->AccountID,lpObj->Name,lpObj->MapNumber);
#endif
	return TRUE;
}

//----- (00563D30) --------------------------------------------------------
BOOL CMercenary::DeleteMercenary(int iIndex)
{
	if ( OBJMAX_RANGE(iIndex) == FALSE )
	{
		return FALSE;
	}

	this->m_iMercenaryCount--;

	LogAddTD("[CastleSiege] Mercenary is broken [%d] - [%d]", iIndex, this->m_iMercenaryCount);

	if ( this->m_iMercenaryCount < 0 )
	{
		this->m_iMercenaryCount = 0;
	}

	return TRUE;
}




int  CMercenary::SearchEnemy(LPOBJ lpObj)
{
	int tObjNum;
	BYTE distanceX = 0,distanceY = 0,attackRange = 0;

	lpObj->TargetNumber = -1;

	if ( lpObj->Class == 286 )
		attackRange = 5;
	if ( lpObj->Class == 287 )
		attackRange = 3;

	for(int i=0; i< MAX_VIEWPORT_MONSTER; i++)
	{
		tObjNum = lpObj->VpPlayer2[i].number;

		if(tObjNum >= 0)
		{
			if(gObj[tObjNum].Type==OBJ_USER)
			{
				if ((gObj[tObjNum].Live)&&(lpObj->m_btCsJoinSide != gObj[tObjNum].m_btCsJoinSide))
				{
					distanceX = lpObj->X - gObj[tObjNum].X;
					distanceY = lpObj->Y - gObj[tObjNum].Y;

					if ( lpObj->Dir == 1 )
					{
						if ( abs(distanceX) <= 2 )
						{
							if ( (lpObj->Y - attackRange) <= gObj[tObjNum].Y )
							{
								if ( lpObj->Y >= gObj[tObjNum].Y )
								{
									lpObj->TargetNumber = tObjNum;
									return TRUE;
								}
							}
						}
					}
					if ( lpObj->Dir == 3 )
					{
						if ( abs(distanceY) <= 2 )
						{
							if ( (lpObj->X - attackRange) <= gObj[tObjNum].X )
							{
								if ( lpObj->X >= gObj[tObjNum].X )
								{
									lpObj->TargetNumber = tObjNum;
									return TRUE;
								}
							}
						}
					}

				}
			}
		}
	}

	return FALSE;
}




void CMercenary::MercenaryAct(int iIndex)
{
	if ( ::gObjIsConnected(iIndex) == FALSE )
	{
		return;
	}

	LPOBJ lpObj = &gObj[iIndex];


	if ( lpObj->VPCount2 < 1 )
	{
		return;
	}

	if ( this->SearchEnemy(lpObj) != 0 && lpObj->TargetNumber >= 0)
	{
		lpObj->m_ActState.Attack = 1;
		lpObj->NextActionTime = lpObj->m_AttackSpeed;
	}
	else
	{
		lpObj->NextActionTime = lpObj->m_MoveSpeed;
	}
}

#endif