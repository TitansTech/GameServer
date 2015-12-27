#include "stdafx.h"
#include "Guardian.h"
#include "GameMain.h"
#include "logproc.h"
#include "User.h"
#include "CastleSiege.h"

// GS_N 0.99.60T 0x00519F60
//	GS-N	1.00.18	JPN	0x0054B8D0	-	Completed
#if (GS_CASTLE==1)
CGuardian g_CsNPC_Guardian;

CGuardian::CGuardian()
{
	return;
}

CGuardian::~CGuardian()
{
	return;
}

BOOL CGuardian::CreateGuardian(int iIndex, BYTE cTX, BYTE cTY)
{
#if (GS_CASTLE==1)
	LPOBJ lpObj = &gObj[iIndex];
	int iMonsterIndex=-1;
	int iMercenaryTypeIndex = 285;

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
		LogAddTD("[CastleSiege] [%s][%s] Life Guardian Vanished (NextMap) - lpMATTR == NULL (SummonIndex:%d)",
			lpObj->AccountID, lpObj->Name, iMonsterIndex);
		return FALSE;
	}

	gObjSetMonster(iMonsterIndex, iMercenaryTypeIndex,"CGuardian::CreateGuardian");

	gObj[iMonsterIndex].Live = TRUE;
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
	//*(_WORD *)(lpMATTR + 28) lpMATTR->m_Hp
	gObj[iMonsterIndex].Level = lpMATTR->m_Level;
	gObj[iMonsterIndex].Type = OBJ_MONSTER;
	gObj[iMonsterIndex].MaxRegenTime = 1000;
	gObj[iMonsterIndex].Dir = 1;
	gObj[iMonsterIndex].RegenTime = GetTickCount();
	gObj[iMonsterIndex].m_Attribute = 60;	//Once its dead = delete monster
	gObj[iMonsterIndex].DieRegen = 0;

	gObj[iMonsterIndex].m_btCsNpcType = 2;
	gObj[iMonsterIndex].m_btCsJoinSide = lpObj->m_btCsJoinSide;
	gObj[iMonsterIndex].Life = g_iNpcUpMaxHP_CSTATUE[0][0];
	gObj[iMonsterIndex].MaxLife = g_iNpcUpMaxHP_CSTATUE[0][0];
	gObj[iMonsterIndex].m_Defense = g_iNpcDefense_CSTATUE[0];
	gObj[iMonsterIndex].m_btCsNpcRgLevel = g_iNpcUpRgLevel_CSTATUE[0][0];

	GCServerMsgStringSend("Life Guardian Created!", lpObj->m_Index, 1);

	if ( lpObj->lpGuild )
		LogAddTD("[CastleSiege] Life Guardian is summoned [%d] - [%d][%d] [%s][%s][%d] - (Guild : %s)", gObj[iMonsterIndex].Class,gObj[iMonsterIndex].X,gObj[iMonsterIndex].Y,lpObj->AccountID,lpObj->Name,lpObj->MapNumber,lpObj->lpGuild->Name);
	else
		LogAddTD("[CastleSiege] Life Guardian is summoned [%d] - [%d][%d] [%s][%s][%d]", gObj[iMonsterIndex].Class,gObj[iMonsterIndex].X,gObj[iMonsterIndex].Y,lpObj->AccountID,lpObj->Name,lpObj->MapNumber);
#endif
	//MsgOutput(iIndex, lMsg.Get( MSGGET(6,104) ));

	return TRUE;
}

int CGuardian::DeleteGuardian(int iIndex)
{
	if ( OBJMAX_RANGE(iIndex) == FALSE )
	{
		return false;
	}

	LPOBJ lpObj = &gObj[iIndex];

    if ( lpObj->lpGuild )
    {
		LogAddTD("[CastleSiege] Life Guardian is broken - [%d]", lpObj->m_btCsJoinSide);
	}
	return TRUE;
}

//----- (00561AD0) --------------------------------------------------------
void CGuardian::GuardianAct(int iIndex)
{
	if( ::gObjIsConnected(iIndex) == FALSE )
		return;

	LPOBJ lpObj = &gObj[iIndex];

	if( lpObj->VPCount < 1 )
		return;

	int tObjNum = -1;

	for( int i = 0; i < MAX_VIEWPORT; i++ )
	{
		tObjNum = lpObj->VpPlayer[i].number;

		if( gObjIsConnected(tObjNum) == TRUE )
		{
			if( gObj[tObjNum].Type == OBJ_USER )
			{
				if( gObj[tObjNum].Live )
				{
					if( gObj[tObjNum].m_btCsJoinSide == lpObj->m_btCsJoinSide )
					{
						if( (abs(lpObj->X - gObj[tObjNum].X) <= 3) && (abs(lpObj->Y - gObj[tObjNum].Y) <= 3) )
						{
							BOOL bIsMaxLife = FALSE;
							BOOL bIsMaxMana = FALSE;
							BOOL bIsMaxBP	= FALSE;

							if( gObj[tObjNum].Life < (gObj[tObjNum].MaxLife+gObj[tObjNum].AddLife) )
							{
								gObj[tObjNum].Life = gObj[tObjNum].Life + ((gObj[tObjNum].AddLife+gObj[tObjNum].MaxLife) * (lpObj->m_btCsNpcRgLevel+1)) / 100;

								if( gObj[tObjNum].Life > (gObj[tObjNum].MaxLife+gObj[tObjNum].AddLife) )
								{
									gObj[tObjNum].Life = gObj[tObjNum].MaxLife+gObj[tObjNum].AddLife;
								}
								bIsMaxLife = TRUE;
							}

							if( gObj[tObjNum].Mana < (gObj[tObjNum].MaxMana+gObj[tObjNum].AddMana) )
							{
								gObj[tObjNum].Mana = gObj[tObjNum].Mana + ((gObj[tObjNum].AddMana+gObj[tObjNum].MaxMana) * (lpObj->m_btCsNpcRgLevel+1)) / 100;

								if( gObj[tObjNum].Mana > (gObj[tObjNum].MaxMana+gObj[tObjNum].AddMana) )
								{
									gObj[tObjNum].Mana = gObj[tObjNum].MaxMana+gObj[tObjNum].AddMana;
								}
								bIsMaxLife = TRUE;
							}

							if( gObj[tObjNum].BP < (gObj[tObjNum].MaxBP+gObj[tObjNum].AddBP) )
							{
								gObj[tObjNum].BP += 100;

								if( gObj[tObjNum].BP > (gObj[tObjNum].MaxBP+gObj[tObjNum].AddBP) )
								{
									gObj[tObjNum].BP = gObj[tObjNum].MaxBP+gObj[tObjNum].AddBP;
								}
								bIsMaxLife = TRUE;
							}

							if( bIsMaxLife ) 
							{
								::GCReFillSend(tObjNum, gObj[tObjNum].Life, 0xFF, 1, gObj[tObjNum].iShield);
							}
							if( bIsMaxMana || bIsMaxBP )
							{
								::GCManaSend(tObjNum, gObj[tObjNum].Mana, 0xFF, 0, gObj[tObjNum].BP);
							}
						}
					}
				}
			}
		}
	}
}

#endif