#include "stdafx.h"
#include "..\common\MyWinsockBase.h"
#include "wsJoinServerCli.h"
#include "GameServer.h"
#include "GameMain.h"
#include "DBSockMng.h"
#include "SCFExDBProtocol.h"
#include "logproc.h"
#include "..\common\winutil.h"
#include "DSProtocol.h"
#include "EDSProtocol.h"
#include "RProtocol.h"
#include "SCFExDB_Defines.h"


void BC_SendRewardScore(int iIndex, int iSCORE, int iLeftTime, int iAlivePartyCount)
{
	if ( OBJMAX_RANGE(iIndex) == FALSE )
	{
		return;
	}

	PMSG_ANS_BLOODCASTLESCORE_5TH_SCFEXDB pMsg;
	PHeadSubSetB((LPBYTE)&pMsg, SCFExDB_GSSend_RProtocol, 0x0D, sizeof(pMsg));

	pMsg.Score = iSCORE;
	pMsg.BridgeNum = gObj[iIndex].m_cBloodCastleIndex;
	pMsg.Class = gObj[iIndex].Class;
	pMsg.ServerCode = gGameServerCode;
	pMsg.iLeftTime = iLeftTime;
	memcpy(pMsg.AccountID, gObj[iIndex].AccountID, MAX_ACCOUNT_LEN);
	memcpy(pMsg.GameID, gObj[iIndex].Name, MAX_ACCOUNT_LEN);
	pMsg.iAlivePartyCount = iAlivePartyCount;

	cDBSMng.Send((char *)&pMsg, pMsg.h.size);
}

void BC_EGReqBloodCastleEnterCount(int iIndex)
{
	if ( !gObjIsConnected(iIndex) )
		return;

	PMSG_REQ_BLOODCASTLE_ENTERCOUNT_SCFEXDB pMsg;
	PHeadSubSetB((LPBYTE)&pMsg, SCFExDB_GSSend_RProtocol, 0x0B, sizeof(pMsg));

	memcpy(pMsg.AccountID, gObj[iIndex].AccountID, 10);
	memcpy(pMsg.GameID, gObj[iIndex].Name, 10);
	pMsg.ServerCode = gGameServerCode / 20;
	pMsg.iObjIndex = iIndex;

	cDBSMng.Send((PCHAR)&pMsg, pMsg.h.size);
}

void BC_EGAnsBloodCastleEnterCount( PMSG_ANS_BLOODCASTLE_ENTERCOUNT_SCFEXDB* lpMsg)
{
	if ( !lpMsg)
		return;

	if ( !gObjIsConnected(lpMsg->iObjIndex))
		return;

	char szAccountID[11] = {0};
	char szName[11] = {0};
	memcpy(szAccountID, lpMsg->AccountID, 10);
	memcpy(szName, lpMsg->GameID, 10);

	if ( strcmp(gObj[lpMsg->iObjIndex].AccountID, szAccountID) ||
		 strcmp(gObj[lpMsg->iObjIndex].Name, szName) )
		 return;

	PMSG_ANS_CL_ENTERCOUNT_SCFEXDB pMsgSend;

	pMsgSend.h.c = 0xC1;
	pMsgSend.h.headcode = 0x9F;
	pMsgSend.h.size = sizeof(pMsgSend);
	pMsgSend.btEventType = 2;
	pMsgSend.btLeftEnterCount = lpMsg->iLeftCount;

	DataSend(lpMsg->iObjIndex, (LPBYTE)&pMsgSend, sizeof(pMsgSend));
}

void DS_SendRankingInfo(int iIndex)
{
	if(OBJMAX_RANGE(iIndex) == 0)
	{
		return;
	}

	LPOBJ lpObj = &gObj[iIndex];

	if ( lpObj->m_nEventScore <= 0 )
	{
		return;
	}

	PMSG_ANS_EVENTUSERSCORE_SCFEXDB pMsg;
	PHeadSubSetB((LPBYTE)&pMsg, SCFExDB_GSSend_RProtocol, 0x01, sizeof(pMsg));

	pMsg.Score = lpObj->m_nEventScore;
	pMsg.SquareNum = lpObj->m_bDevilSquareIndex;
	pMsg.Class = lpObj->Class;
	pMsg.ServerCode = gGameServerCode;
	memcpy(pMsg.AccountID, lpObj->AccountID, MAX_ACCOUNT_LEN);
	memcpy(pMsg.GameID, lpObj->Name, MAX_ACCOUNT_LEN);

	cDBSMng.Send((PCHAR)&pMsg, pMsg.h.size);
}
