// CastleCrownSwitch.cpp: implementation of the CCastleCrownSwitch class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CastleCrownSwitch.h"

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
CCastleCrownSwitch g_CastleCrownSwitch;
extern CCastleSiege g_CastleSiege;

CCastleCrownSwitch::CCastleCrownSwitch()
{

}

CCastleCrownSwitch::~CCastleCrownSwitch()
{

}

//----- (00561490) --------------------------------------------------------
void CCastleCrownSwitch::CrownSwitchPushTime(int iSwitchIndex)
{
	
	//DebugLog("%s START",__FUNCTION__);
	if ( gObjIsConnected(iSwitchIndex) )
	{
		LPOBJ lpCrownSwObj = &gObj[iSwitchIndex];

		int iSwitchUserIndex = g_CastleSiege.GetCrownSwitchUserIndex(lpCrownSwObj->Class);
		if ( gObjIsConnected(iSwitchUserIndex) )
		{
			LPOBJ lpCrownSwUserObj = &gObj[iSwitchUserIndex];

			g_CastleSiege.NotifyCrownSwitchInfo(iSwitchIndex);

			if (lpCrownSwUserObj->MapNumber != MAP_INDEX_CASTLESIEGE || 
				lpCrownSwUserObj->m_btCsJoinSide <= 1 ) /*|| 
				lpCrownSwUserObj->m_bCsGuildInvolved == false) */
			{
				GCAnsCsAccessSwitchState(lpCrownSwUserObj->m_Index,lpCrownSwObj->m_Index,-1,0);
				g_CastleSiege.ResetCrownSwitchUserIndex(lpCrownSwObj->Class);
				if ( g_CastleSiege.GetRegCrownAvailable() == 1 )
				{
					GCSkillInfoSend(lpCrownSwObj, 0, 0x14);

					g_CastleSiege.SetRegCrownAvailable(0);
					g_CastleSiege.NotifyCrownState(1);
				}
			}

			if ( abs( (lpCrownSwObj->Y - lpCrownSwUserObj->Y) ) > 3
					|| abs( (lpCrownSwObj->X - lpCrownSwUserObj->X) ) > 3 )
			{
				GCAnsCsAccessSwitchState(lpCrownSwUserObj->m_Index,lpCrownSwObj->m_Index,-1,0);
				g_CastleSiege.ResetCrownSwitchUserIndex(lpCrownSwObj->Class);
				if ( g_CastleSiege.GetRegCrownAvailable() == 1 )
				{
					GCSkillInfoSend(lpCrownSwObj, 0, 0x14);

					g_CastleSiege.SetRegCrownAvailable(0);
					g_CastleSiege.NotifyCrownState(1);
				}
				LogAddTD("[CastleSiege][Switch][%s][%s] Push Castle Crown Switch Canceled (GUILD:%s) - CS X:%d/Y:%d",lpCrownSwUserObj->AccountID, lpCrownSwUserObj->Name, lpCrownSwUserObj->GuildName, lpCrownSwUserObj->X, lpCrownSwUserObj->Y);
			} else {
				int v8 = g_CastleSiege.GetCrownSwitchUserIndex(217);
				int v9 = g_CastleSiege.GetCrownSwitchUserIndex(218);

				if (gObjIsConnected(v8) && gObjIsConnected(v9))
				{
					LPOBJ lpSwUser1Obj = &gObj[v8];
					LPOBJ lpSwUser2Obj = &gObj[v9];

					if ( lpSwUser1Obj->m_btCsJoinSide == lpSwUser2Obj->m_btCsJoinSide )
					{
						if ( g_CastleSiege.GetRegCrownAvailable() != 1 )
						{
							GCSkillInfoSend(lpCrownSwObj, 1, 0x14);

							g_CastleSiege.SetRegCrownAvailable(1);
							g_CastleSiege.NotifyCrownState(0);
						}
					}
				}
			}
		} else {
				g_CastleSiege.ResetCrownSwitchUserIndex(lpCrownSwObj->Class);
				if ( g_CastleSiege.GetRegCrownAvailable() == 1 )
				{
					g_CastleSiege.SetRegCrownAvailable(0);
					g_CastleSiege.NotifyCrownState(1);
				}
		}
	}
	
	//DebugLog("%s END",__FUNCTION__);
}

#endif