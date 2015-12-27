// CastleCrown.cpp: implementation of the CCastleCrown class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CastleCrown.h"

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
CCastleCrown g_CastleCrown;
extern CCastleSiege g_CastleSiege;

CCastleCrown::CCastleCrown()
{

}

CCastleCrown::~CCastleCrown()
{

}

//----- (00560DA0) --------------------------------------------------------
void CCastleCrown::CalcAccessCrownTime(int iCrownIndex)
{
	//DebugLog("%s START",__FUNCTION__);
	LPOBJ lpCrownObj = &gObj[iCrownIndex];

	if ( lpCrownObj->Class == 216 )
	{
		int iUserIndex = g_CastleSiege.GetCrownUserIndex();

		if ( gObjIsConnected(iUserIndex) )
		{
			LPOBJ lpObj = &gObj[iUserIndex];
			
			if ( lpObj->MapNumber != MAP_INDEX_CASTLESIEGE || 
				 lpObj->m_btCsJoinSide <= 1 )/*|| 
				 lpObj->m_bCsGuildInvolved == false )*/
			{
				GCAnsCsAccessCrownState(g_CastleSiege.m_iCastleCrownAccessUser, 2);
				g_CastleSiege.ResetCrownAccessTickCount();
				g_CastleSiege.SetCrownAccessUserX(0);
				g_CastleSiege.SetCrownAccessUserY(0);
				g_CastleSiege.ResetCrownUserIndex();
			} else {
				if ( (lpObj->X != g_CastleSiege.GetCrownAccessUserX()) || 
					(lpObj->Y != g_CastleSiege.GetCrownAccessUserY()) )
				{
					GCAnsCsAccessCrownState(g_CastleSiege.m_iCastleCrownAccessUser, 2);
					g_CastleSiege.ResetCrownAccessTickCount();
					g_CastleSiege.SetCrownAccessUserX(0);
					g_CastleSiege.SetCrownAccessUserY(0);
					g_CastleSiege.ResetCrownUserIndex();
				} else {
					int iSwitchIndex1 = g_CastleSiege.GetCrownSwitchUserIndex(217);
					int iSwitchIndex2 = g_CastleSiege.GetCrownSwitchUserIndex(218);

					if ( gObjIsConnected(iSwitchIndex1) && gObjIsConnected(iSwitchIndex2) )
					{
						LPOBJ lpSw1Obj = &gObj[iUserIndex];
						LPOBJ lpSw2Obj = &gObj[iUserIndex];

						if ( ( lpSw1Obj->m_btCsJoinSide != lpObj->m_btCsJoinSide ) || 
							(lpSw2Obj->m_btCsJoinSide != lpObj->m_btCsJoinSide ) ||
							(lpSw1Obj->m_btCsJoinSide != lpSw2Obj->m_btCsJoinSide ))
						{
							GCAnsCsAccessCrownState(g_CastleSiege.m_iCastleCrownAccessUser, 2);
							g_CastleSiege.ResetCrownAccessTickCount();
							g_CastleSiege.SetCrownAccessUserX(0);
							g_CastleSiege.SetCrownAccessUserY(0);
							g_CastleSiege.ResetCrownUserIndex();
							LogAddTD("[CastleSiege][Wrong Sides][%s][%s] Register Castle Crown Canceled (GUILD:%s)", lpObj->AccountID, lpObj->Name, lpObj->GuildName);
						} else {
							lpObj->m_iAccumulatedCrownAccessTime = GetTickCount() - g_CastleSiege.GetCrownAccessTickCount();
							
							//GCAnsCsAccessCrownState(lpObj->m_Index, 0);	[Fix for the time display]

							LogAddTD("[CastleSiege][Reg. Accumulating][%s][%s] Accumulated Crown AccessTime : %d msec", lpObj->AccountID, lpObj->Name, lpObj->m_iAccumulatedCrownAccessTime);
						}
					} else {
						GCAnsCsAccessCrownState(g_CastleSiege.m_iCastleCrownAccessUser, 2);
						g_CastleSiege.ResetCrownAccessTickCount();
						g_CastleSiege.SetCrownAccessUserX(0);
						g_CastleSiege.SetCrownAccessUserY(0);
						g_CastleSiege.ResetCrownUserIndex();
					}
				}
			}
		} else {
			g_CastleSiege.ResetCrownAccessTickCount();
			g_CastleSiege.SetCrownAccessUserX(0);
			g_CastleSiege.SetCrownAccessUserY(0);
			g_CastleSiege.ResetCrownUserIndex();
		}
	}
	
	//DebugLog("%s END",__FUNCTION__);
}

#endif