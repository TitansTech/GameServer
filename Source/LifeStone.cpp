// LifeStone.cpp: implementation of the CLifeStone class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LifeStone.h"
#include "..\common\winutil.h"

#if (GS_CASTLE==1)
#include "CastleSiege.h"
#include "DSProtocol.h"
#include "GameMain.h"
#include "TNotice.h"
#include "TUnion.h"
#include "LifeStone.h"
#include "Mercenary.h"
#include "DBSockMng.h"
#include "LogToFile.h"
#include "logproc.h"
#include "..\include\readscript.h"

CLifeStone g_CsNPC_LifeStone;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLifeStone::CLifeStone()
{

}

CLifeStone::~CLifeStone()
{

}

struct PMSG_LIFESTONE_STATE_STRUCT
{
	PBMSG_HEAD2 h;	// C1:B9:XX	[1-4]
	BYTE NumberH;	// 5
	BYTE NumberL;	// 6
	BYTE State;		// 7
};

//----- (0045F6C0) --------------------------------------------------------
void CLifeStone::SendLifeStoneState(int iIndex)
{
	LPOBJ lpObj = &gObj[iIndex];

	PMSG_LIFESTONE_STATE_STRUCT pSend;
	PHeadSubSetB((LPBYTE)&pSend, 0xB9, 1, sizeof(PMSG_LIFESTONE_STATE_STRUCT));
	pSend.NumberH	= SET_NUMBERH(iIndex);
	pSend.NumberL	= SET_NUMBERL(iIndex);
	pSend.State		= lpObj->m_btCreationState;

	::MsgSendV2(lpObj, (PBYTE)&pSend, pSend.h.size);
}

//----- (00562F60) --------------------------------------------------------
void CLifeStone::LifeStoneAct(int iIndex)
{
	if( ::gObjIsConnected(iIndex) == FALSE )
		return;

	LPOBJ lpObj = &gObj[iIndex];

	lpObj->m_iCreatedActivationTime += 1;
	BYTE stoneStatus = lpObj->m_btCreationState;

	if ( lpObj->m_iCreatedActivationTime >= 60 )
		lpObj->m_btCreationState = 5;
    else
		lpObj->m_btCreationState = lpObj->m_iCreatedActivationTime / 12;

    if ( stoneStatus != lpObj->m_btCreationState )
		this->SendLifeStoneState(iIndex);

	if ( lpObj->m_btCreationState < 5 )
		return;

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
					if( gObj[tObjNum].m_btCsJoinSide == 1 )
					{
						if( (abs(lpObj->X - gObj[tObjNum].X) <= 3) && (abs(lpObj->Y - gObj[tObjNum].Y) <= 3) )
						{
							BOOL bIsMaxLife = FALSE;
							BOOL bIsMaxMana = FALSE;
							BOOL bIsMaxBP	= FALSE;

							if( gObj[tObjNum].Life < (gObj[tObjNum].Life+gObj[tObjNum].AddLife) )
							{
								gObj[tObjNum].Life = gObj[tObjNum].Life + ((gObj[tObjNum].AddLife+gObj[tObjNum].MaxLife) * (lpObj->m_btCsNpcRgLevel+1)) / 100;

								if( gObj[tObjNum].Life > (gObj[tObjNum].Life+gObj[tObjNum].AddLife) )
								{
									gObj[tObjNum].Life = gObj[tObjNum].Life+gObj[tObjNum].AddLife;
								}
								bIsMaxLife = TRUE;
							}

							if( gObj[tObjNum].Mana < (gObj[tObjNum].Mana+gObj[tObjNum].AddMana) )
							{
								gObj[tObjNum].Mana = gObj[tObjNum].Mana + ((gObj[tObjNum].AddMana+gObj[tObjNum].MaxMana) * (lpObj->m_btCsNpcRgLevel+1)) / 100;

								if( gObj[tObjNum].Mana > (gObj[tObjNum].Mana+gObj[tObjNum].AddMana) )
								{
									gObj[tObjNum].Mana = gObj[tObjNum].Mana+gObj[tObjNum].AddMana;
								}
								bIsMaxLife = TRUE;
							}

							if( gObj[tObjNum].BP < (gObj[tObjNum].BP+gObj[tObjNum].AddBP) )
							{
								gObj[tObjNum].BP = gObj[tObjNum].BP + ((gObj[tObjNum].AddBP+gObj[tObjNum].MaxBP) * (lpObj->m_btCsNpcRgLevel+1)) / 100;

								if( gObj[tObjNum].BP > (gObj[tObjNum].BP+gObj[tObjNum].AddBP) )
								{
									gObj[tObjNum].BP = gObj[tObjNum].BP+gObj[tObjNum].AddBP;
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

//----- (00562770) --------------------------------------------------------
BOOL CLifeStone::CreateLifeStone(int iIndex)
{
	LPOBJ lpObj = &gObj[iIndex];
	int iMonsterIndex=-1;

	if ( g_CastleSiege.GetCastleState() != 7 )
	{
		//(98175472, 1248)	(4, 224)
		//(98175472, 1180)	(4, 156)	=> BASE: 1024
		//(98175472, 1500)	(5, 220)	=> 1500-1024 = 476 (220 = 476 - 256)
		GCServerMsgStringSend(lMsg.Get(MSGGET(6, 109)), lpObj->m_Index, 1);
		return FALSE;
	}

	if (lpObj->GuildStatus != GUILD_MASTER)
	{
		return FALSE;
	}

	if (lpObj->m_btCsJoinSide < 2 )
	{
		GCServerMsgStringSend(lMsg.Get(MSGGET(6, 105)), lpObj->m_Index, 1);
		return FALSE;
	}

	if (lpObj->lpGuild == NULL)
		return FALSE;

	if (!lpObj->lpGuild->back)
		return FALSE;

	if (lpObj->lpGuild->back->Number > 0)
	{
		GCServerMsgStringSend(lMsg.Get(MSGGET(6, 106)), lpObj->m_Index, 1);
		return FALSE;
	}

	if ( (lpObj->X > 150 && lpObj->X < 210) && (lpObj->Y > 175 && lpObj->Y < 230) )
	{
		GCServerMsgStringSend(lMsg.Get(MSGGET(6, 108)), lpObj->m_Index, 1);
		return FALSE;
	}

	BYTE btMapAttr = MapC[lpObj->MapNumber].GetAttr(lpObj->X, lpObj->Y);
	if ( lpObj->MapNumber != MAP_INDEX_CASTLESIEGE )
	{
		GCServerMsgStringSend(lMsg.Get(MSGGET(6, 91)), lpObj->m_Index, 1);
		return FALSE;
	}

	iMonsterIndex = gObjAddMonster(lpObj->MapNumber);
	if ( iMonsterIndex < 0 )
	{
		GCServerMsgStringSend(lMsg.Get(MSGGET(6, 111)), lpObj->m_Index, 1);
		return FALSE;
	}

	LPMONSTER_ATTRIBUTE lpMATTR = gMAttr.GetAttr(278);
	if ( !lpMATTR )
	{
		gObjDel(iMonsterIndex);
		LogAddTD("[CastleSiege] [%s][%s] LifeStone Vanished (NextMap) - lpMATTR == NULL (SummonIndex:%d)",
			lpObj->AccountID, lpObj->Name, iMonsterIndex);
		return FALSE;
	}
	
	gObjSetMonster(iMonsterIndex, 278,"CLifeStone::CreateLifeStone");

	gObj[iMonsterIndex].Live = TRUE;
	gObj[iMonsterIndex].Life = lpMATTR->m_Hp;
	gObj[iMonsterIndex].MaxLife = lpMATTR->m_Hp;
	gObj[iMonsterIndex].m_PosNum = -1;
	gObj[iMonsterIndex].X = lpObj->X;
	gObj[iMonsterIndex].Y = lpObj->Y;
	gObj[iMonsterIndex].MTX = lpObj->X;
	gObj[iMonsterIndex].MTY = lpObj->Y;
	gObj[iMonsterIndex].TX = lpObj->X;
	gObj[iMonsterIndex].TY = lpObj->Y;
	gObj[iMonsterIndex].m_OldX = lpObj->X;
	gObj[iMonsterIndex].m_OldY = lpObj->Y;
	gObj[iMonsterIndex].StartX = lpObj->X;
	gObj[iMonsterIndex].StartY = lpObj->Y;
	gObj[iMonsterIndex].MapNumber = lpObj->MapNumber;
	gObj[iMonsterIndex].m_MoveRange = 0;
	gObj[iMonsterIndex].Level = lpMATTR->m_Level;	//*(_WORD *)(lpMATTR + 28)
	gObj[iMonsterIndex].Type = OBJ_MONSTER;		//2
	gObj[iMonsterIndex].MaxRegenTime = 1000;
	gObj[iMonsterIndex].Dir = 1;
	gObj[iMonsterIndex].RegenTime = GetTickCount();
	gObj[iMonsterIndex].m_Attribute = 0;
	gObj[iMonsterIndex].DieRegen = 0;
	gObj[iMonsterIndex].m_btCsNpcType = 3;
	gObj[iMonsterIndex].m_btCsJoinSide = lpObj->m_btCsJoinSide;
	gObj[iMonsterIndex].lpGuild = lpObj->lpGuild;
	gObj[iMonsterIndex].m_btCreationState = 0;
	lpObj->lpGuild->back->Number = gObj[iMonsterIndex].m_Index;

	GCServerMsgStringSend(lMsg.Get(MSGGET(6, 105)), lpObj->m_Index, 1);
	LogAddTD("[CastleSiege] LifeStone is created - [%s][%s] (Map:%d)(X:%d, Y:%d)",
		lpObj->AccountID,lpObj->Name,lpObj->MapNumber,lpObj->X,lpObj->Y);

	lpObj->m_btLifeStoneCount += 1;
	return TRUE;
}


//----- (00562DC0) --------------------------------------------------------
int CLifeStone::DeleteLifeStone(int iIndex)
{
	if ( OBJMAX_RANGE(iIndex) == FALSE )
	{
		return false;
	}

	LPOBJ lpObj = &gObj[iIndex];

    if ( lpObj->lpGuild )
    {
		LogAddTD("[CastleSiege] LifeStone is broken - [%s]", lpObj->lpGuild->Name);
		lpObj->lpGuild->back->Number = 0;
    }
	return TRUE;
}

//----- (00562E60) --------------------------------------------------------
int CLifeStone::SetReSpawnUserXY(int iUserIndex)
{
	bool value=false;
	__try
	{
		if ( g_CastleSiege.GetCastleState() != 7 )
			return FALSE;

		if ( OBJMAX_RANGE(iUserIndex) == FALSE )
			return FALSE;

		if ( !gObjIsConnected(iUserIndex) )
			return FALSE;

		LPOBJ lpObj = &gObj[iUserIndex];
		LogAddTD("[CastleSiege] Looking for LifeStone : [%s][%s] (IX:%d)",lpObj->AccountID,lpObj->Name,lpObj->m_Index);

		if( lpObj->MapNumber != MAP_INDEX_CASTLESIEGE )
			return FALSE;

		if ( lpObj->lpGuild == NULL )
			return FALSE;

		if ( !lpObj->lpGuild->back )
			return FALSE;

		if ( !lpObj->lpGuild->back->Number )
			return FALSE;

		int LifeStoneIndex = lpObj->lpGuild->back->Number;

		LogAddTD("[CastleSiege] Looking for LifeStone ID: [%s][%s] (ID:%d)",lpObj->AccountID,lpObj->Name,lpObj->lpGuild->back->Number);

		if ( !OBJMAX_RANGE(LifeStoneIndex) )
			return FALSE;

		if( gObj[LifeStoneIndex].Class != 278 )
			return FALSE;

		if ( gObj[LifeStoneIndex].Live )
		{
			lpObj->MapNumber = 30;
			lpObj->X = gObj[LifeStoneIndex].X;
			lpObj->Y = gObj[LifeStoneIndex].Y;

			value = true;
			return TRUE;
		}

		return FALSE;
	}__finally
	{
		if(value == false)
			return FALSE;
		else 
			return TRUE;
	}
}
#endif