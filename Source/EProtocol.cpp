#include "stdafx.h"
#include "..\common\MyWinsockBase.h"
#include "wsJoinServerCli.h"
#include "GameServer.h"
#include "GameMain.h"
#include "DBSockMng.h"
#include "SCFExDBProtocol.h"
#include "logproc.h"
#include "..\common\winutil.h"
#include "event.h"
#include "EDSProtocol.h"
#include "SCFExDB_Defines.h"


void EProtocolCore(BYTE protoNum, LPBYTE aRecv)
{
	BYTE pResult[256] = {0};
	memcpy(&pResult[0],&aRecv[0],3);
	memcpy(&pResult[4],&aRecv[4],(aRecv[1]-1-3));
	pResult[1] -= 1;
	pResult[2] = aRecv[3];
	EventChipEventProtocolCore(pResult[2],(LPBYTE)pResult,pResult[2]);
}

void EProtocolSendTransform(char * aSend)
{
	char pSend [256] = {0};
	memcpy(&pSend[0],&aSend[0],3);
	memcpy(&pSend[3],&aSend[3],(aSend[1]-1));
	pSend[1] += 1;
	pSend[2] = SCFExDB_GSSend_EProtocol;
	pSend[3] = aSend[2];

	cDBSMng.Send((PCHAR)&pSend, pSend[1]);
}