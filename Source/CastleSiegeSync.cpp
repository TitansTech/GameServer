#include "stdafx.h"
#include "CastleSiegeSync.h"
#include "TUnion.h"
#include "protocol.h"
#include "DSProtocol.h"
#include "MapServerManager.h"
#include "logproc.h"

// GS-N 0.99.60T 0x0051B7C0 - Completed
//	GS-N	1.00.18	JPN	0x0054D150	-	Completed



CCastleSiegeSync::CCastleSiegeSync()
{
	this->Clear();
}



CCastleSiegeSync::~CCastleSiegeSync()
{
	return;
}
	


void CCastleSiegeSync::Clear()
{
	this->m_iCurCastleState = -1;
	this->m_iCurTaxRateChaos = 0;
	this->m_iCurTaxRateStore  = 0;
	this->m_lCastleTributeMoney = 0;
	this->m_iCsTributeMoneyTimer = 0;
	memset(this->m_szCastleOwnerGuild, 0, sizeof(this->m_szCastleOwnerGuild));
}





void CCastleSiegeSync::SetCastleOwnerGuild(char * lpszGuildName)
{
	if ( lpszGuildName == NULL )
	{
		DebugLog("%s NULL",__FUNCTION__);
		return;
	}

	memset(this->m_szCastleOwnerGuild, 0, sizeof(this->m_szCastleOwnerGuild));
	memcpy(this->m_szCastleOwnerGuild, lpszGuildName, sizeof(this->m_szCastleOwnerGuild)/2);
	DebugLog("%s Setting to %s vs %s",__FUNCTION__,lpszGuildName,this->m_szCastleOwnerGuild);
}




int CCastleSiegeSync::GetTaxRateChaos(int iIndex)
{
	//DebugLog("%s START",__FUNCTION__);
	int iCurTaxRateChaos = this->m_iCurTaxRateChaos;

	if ( this->CheckCastleOwnerMember(iIndex) == TRUE || this->CheckCastleOwnerUnionMember(iIndex) == TRUE )
	{
		iCurTaxRateChaos = 0;
	}

	//DebugLog("%s END",__FUNCTION__);
	return iCurTaxRateChaos;
}




int CCastleSiegeSync::GetTaxRateStore(int iIndex)
{
	//DebugLog("%s START",__FUNCTION__);
	int iCurTaxRateStore = this->m_iCurTaxRateStore;

	if ( this->CheckCastleOwnerMember(iIndex) == TRUE || this->CheckCastleOwnerUnionMember(iIndex) == TRUE )
	{
		iCurTaxRateStore = 0;
	}

	//DebugLog("%s END",__FUNCTION__);
	return iCurTaxRateStore;
}




int CCastleSiegeSync::GetTaxHuntZone(int iIndex, BOOL bCheckOwnerGuild)
{
	//DebugLog("%s START",__FUNCTION__);
	int iCurTaxHuntZone = this->m_iCurTaxHuntZone;

	if ( bCheckOwnerGuild == TRUE )
	{
		if ( this->CheckCastleOwnerMember(iIndex) == TRUE || this->CheckCastleOwnerUnionMember(iIndex) == TRUE )
		{
			iCurTaxHuntZone = 0;
		}
	}

	//DebugLog("%s END",__FUNCTION__);
	return iCurTaxHuntZone;
}




void CCastleSiegeSync::AddTributeMoney(int iMoney)
{
	if ( iMoney <= 0 )
	{
		//DebugLog("%s END",__FUNCTION__);
		return;
	}

	if ( (this->m_lCastleTributeMoney + iMoney) > MAX_TRIBUTE_MONEY )
	{
		//DebugLog("%s END",__FUNCTION__);
		return;
	}

	//DebugLog("%s START",__FUNCTION__);
	if ( this->m_lCastleTributeMoney < 0 )
	{
		InterlockedExchange((LPLONG)&this->m_lCastleTributeMoney, 0);
	}

	InterlockedExchangeAdd((LPLONG)&this->m_lCastleTributeMoney, iMoney);
	//DebugLog("%s END",__FUNCTION__);
}




void CCastleSiegeSync::ResetTributeMoney()
{
	//DebugLog("%s START",__FUNCTION__);
	InterlockedExchange((LPLONG)&this->m_lCastleTributeMoney, 0);
	//DebugLog("%s END",__FUNCTION__);
}




void CCastleSiegeSync::AdjustTributeMoney()
{
	//DebugLog("%s START",__FUNCTION__);
	if ( this->m_lCastleTributeMoney < 0 )
	{
		InterlockedExchange((LPLONG)&this->m_lCastleTributeMoney, 0);
	}

	if ( this->m_lCastleTributeMoney > MAX_TRIBUTE_MONEY )
	{
		InterlockedExchange((LPLONG)&this->m_lCastleTributeMoney, MAX_TRIBUTE_MONEY);
	}

	if ( this->m_lCastleTributeMoney <= 0 )
	{
		//DebugLog("%s END",__FUNCTION__);
		return;
	}

	if ( GetTickCount() - this->m_iCsTributeMoneyTimer > 180000 )
	{
		this->m_iCsTributeMoneyTimer = GetTickCount();
		LogAddTD("[CastleSiegeSync] AdjustTributeMoney() SRV: %d SEND MONEY:%d",
			g_MapServerManager.GetMapSvrGroup(), this->m_lCastleTributeMoney);

		GS_GDReqCastleTributeMoney(g_MapServerManager.GetMapSvrGroup(), this->m_lCastleTributeMoney);
		InterlockedExchange((LPLONG)&this->m_lCastleTributeMoney, 0);
	}
	//DebugLog("%s END",__FUNCTION__);
}





BOOL CCastleSiegeSync::CheckCastleOwnerMember(int iIndex)
{
	if ( gObjIsConnected(iIndex) == FALSE )
	{
		//DebugLog("%s Not Connected",__FUNCTION__);
		return FALSE;
	}

	if ( strcmp(this->m_szCastleOwnerGuild, "") == 0 )
	{
		//DebugLog("%s Empty Guild Name",__FUNCTION__);
		return FALSE;
	}

	if ( strcmp(gObj[iIndex].GuildName, this->m_szCastleOwnerGuild) != 0 )
	{
		//DebugLog("%s Guild Name [%s] vs [%s]",__FUNCTION__,gObj[iIndex].GuildName,this->m_szCastleOwnerGuild);
		return FALSE;
	}

	//DebugLog("%s OK",__FUNCTION__);
	return TRUE;
}





BOOL CCastleSiegeSync::CheckCastleOwnerUnionMember(int iIndex)
{
	//DebugLog("%s START",__FUNCTION__);
	if ( gObjIsConnected(iIndex) == FALSE )
	{
		return FALSE;
	}

	if ( strcmp(this->m_szCastleOwnerGuild, "") == 0 )
	{
		//DebugLog("%s END",__FUNCTION__);
		return FALSE;
	}

	_GUILD_INFO_STRUCT * lpGuildInfo = gObj[iIndex].lpGuild;
	
	if ( lpGuildInfo == NULL )
	{
		//DebugLog("%s END",__FUNCTION__);
		return FALSE;
	}
	
	TUnionInfo * pUnionInfo = UnionManager.SearchUnion(lpGuildInfo->iGuildUnion);

	if ( pUnionInfo == NULL )
	{
		//DebugLog("%s END",__FUNCTION__);
		return FALSE;
	}

	if ( strcmp( pUnionInfo->m_szMasterGuild, this->m_szCastleOwnerGuild) == 0 )
	{
		//DebugLog("%s END",__FUNCTION__);
		return TRUE;
	}

	return FALSE;
	//DebugLog("%s END",__FUNCTION__);
}





int CCastleSiegeSync::CheckOverlapCsMarks(int iIndex)
{
	for ( int x=INVENTORY_BAG_START;x<ReadConfig.MAIN_INVENTORY_SIZE(iIndex,false);x++)
	{
		if ( gObj[iIndex].pInventory[x].IsItem() == TRUE )
		{
			if ( gObj[iIndex].pInventory[x].m_Type == ITEMGET(14,21) )
			{
				if ( gObj[iIndex].pInventory[x].m_Level == 3 )
				{
					int op = gObj[iIndex].pInventory[x].m_Durability;

					if ( ITEM_DUR_RANGE(op) != FALSE)
					{
						return x;
					}
				}
			}
		}
	}

	return -1;
}


CCastleSiegeSync g_CastleSiegeSync;