// GuardianStatue.cpp: implementation of the CGuardianStatue class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GuardianStatue.h"
#include "user.h"

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

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CGuardianStatue g_GuardianStatue;

CGuardianStatue::CGuardianStatue()
{

}

CGuardianStatue::~CGuardianStatue()
{

}

//----- (00562000) --------------------------------------------------------
void CGuardianStatue::GuardianStatueAct(int iIndex)
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
					if( gObj[tObjNum].m_btCsJoinSide == 1 )
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
								gObj[tObjNum].BP = gObj[tObjNum].BP + ((gObj[tObjNum].AddBP+gObj[tObjNum].MaxBP) * (lpObj->m_btCsNpcRgLevel+1)) / 100;

								if( gObj[tObjNum].BP > (gObj[tObjNum].MaxBP+gObj[tObjNum].AddBP) )
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


#endif