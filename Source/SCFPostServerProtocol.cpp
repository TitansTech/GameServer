#include "stdafx.h"
#include "..\common\MyWinsockBase.h"
#include "wsJoinServerCli.h"
#include "LogToFile.h"
#include "GameServer.h"
#include "GameMain.h"
#include "DBSockMng.h"
#include "SCFPostServerProtocol.h"
#include "logproc.h"
#include "GMMng.h"
#include "..\common\winutil.h"

wsJoinServerCli wsSCFPSCli;
extern CLogToFile POST_LOG;

void SCFPSClientMsgProc(WPARAM wParam, LPARAM lParam)
{
	switch ( lParam & 0xFFFF & 0xFFFF )
	{
		case 1:
			wsSCFPSCli.DataRecv();
			break;

		case 2:
			wsSCFPSCli.FDWRITE_MsgDataSend();
			break;

		case 32:
			LogAddC(2, "SCFPostServer Close");
			wsSCFPSCli.Close();
			break;
	}
}


BOOL SCFPSServerConnect(char* ConnectServer,int SCFPSPort ,DWORD wMsg)
{
	wsSCFPSCli.SetProtocolCore(SCFPSServerProtocolCore);
	int result = wsSCFPSCli.Connect(ConnectServer, SCFPSPort, wMsg);

	if ( result == 0 )
	{
		LogAdd("Error: Cant Connect To SCF Post Server (%s:%d)", ConnectServer, SCFPSPort);
		return FALSE;
	}

	SCFPostServerLogin();
	LogAddC(2, "SCFPostServer Online");
	return TRUE;
}

void SCFPSServerProtocolCore(BYTE protoNum, LPBYTE aRecv, int aLen)
{	
	switch ( protoNum )
	{
		case 0x00:
			SCFPostServerLoginResult((SDHP_RESULT *)aRecv);
			break;	
		case 0x01:
			SCFPostServerRecvPost((SDHP_POSTINFO *)aRecv);
			break;
		case 0x02:
			SCFAllServerMsgRecv((SDHP_ALLSVRMSGSENDINFO *)aRecv);
			break;
	}
}

void SCFPostServerLogin()
{
	SDHP_SERVERINFO pInfo;

	PHeadSetB((LPBYTE)&pInfo, 0x00, sizeof(pInfo));
	pInfo.Port = GameServerPort;
	pInfo.Type = 0x01;
	pInfo.ServerCode = gGameServerCode;
	strcpy(pInfo.ServerName, szServerName);

	wsSCFPSCli.DataSend((char*)&pInfo, pInfo.h.size);
}

void SCFPostServerLoginResult(SDHP_RESULT * lpMsg)
{
	if ( lpMsg->Result == false )
	{
		LogAddTD("SCF PostServer Login Fail: %d", lpMsg->Result);

		return;
	}	
}

void SCFPostServerRecvPost(SDHP_POSTINFO * lpMsg)
{
	ServerMsgSendPost(ReadConfig.CmdPostColor ,lpMsg->Name," [POST] %s",lpMsg->Chat);
}

void SCFPostSend(int aIndex, char * chat)
{
	int Len = strlen(chat);
	if(Len > 60)
		Len = 60;
	SDHP_POSTINFO pResult={0};
	PHeadSetB((LPBYTE)&pResult, 0x01, sizeof(pResult));
	strncpy(pResult.Chat,chat,Len);
	strcpy(pResult.Name,gObj[aIndex].Name);
	pResult.ServerCode = gGameServerCode;
	wsSCFPSCli.DataSend((char*)&pResult, pResult.h.size);	
}

void SCFAllServerMsgRecv(SDHP_ALLSVRMSGSENDINFO * lpMsg)
{
	AllSendServerMsg(lpMsg->Chat);
}

void SCFAllServerMsgSend(char * chat)
{
	int Len = strlen(chat);
	if(Len > 60)
		Len = 60;
	SDHP_ALLSVRMSGSENDINFO pResult={0};
	PHeadSetB((LPBYTE)&pResult, 0x02, sizeof(pResult));
	strncpy(pResult.Chat,chat,Len);
	pResult.ServerCode = gGameServerCode;
	wsSCFPSCli.DataSend((char*)&pResult, pResult.h.size);
}