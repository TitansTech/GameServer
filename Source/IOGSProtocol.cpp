#include "stdafx.h"
#include "..\common\MyWinsockBase.h"
#include "wsJoinServerCli.h"
#include "GameServer.h"
#include "GameMain.h"
#include "DBSockMng.h"
#include "SCFExDBProtocol.h"
#include "SCFExDB_Defines.h"
#include "IOGSProtocol.h"
#include "logproc.h"
#include "..\common\winutil.h"

wsJoinServerCli wsIOGSServerCli;

void IOGSClientMsgProc(WPARAM wParam, LPARAM lParam)
{
	switch ( lParam & 0xFFFF & 0xFFFF )
	{
		case 1:
			wsIOGSServerCli.DataRecv();
			break;

		case 2:
			wsIOGSServerCli.FDWRITE_MsgDataSend();
			break;

		case 32:
			LogAddC(2, "Titan IOGS Close");
			wsIOGSServerCli.Close();
			break;
	}
}


BOOL IOGSConnect(char* ConnectServer,int Port ,DWORD wMsg)
{
	wsIOGSServerCli.SetProtocolCore(IOGSProtocolCore);
	int result = wsIOGSServerCli.Connect(ConnectServer, Port, wMsg);

	if ( result == 0 )
	{
		LogAdd("Error: Cant Connect To Titan IOGS (%s:%d)", ConnectServer, Port);
		return FALSE;
	}

	IOGSLogin();
	LogAddC(2, "Titan IOGS Online");
	return TRUE;
}

//Connection

void IOGSLogin()
{
	SDHP_SERVERINFO pInfo;

	PHeadSetB((LPBYTE)&pInfo, 0x00, sizeof(pInfo));
	pInfo.Port = GameServerPort;
	pInfo.Type = 0x01;
	pInfo.ServerCode = gGameServerCode;
	strcpy(pInfo.ServerName, szServerName);

	wsIOGSServerCli.DataSend((char*)&pInfo, pInfo.h.size);
}

void IOGSProtocolCore(BYTE protoNum, LPBYTE aRecv, int aLen)
{	
	switch ( protoNum )
	{
		//case 0x00:
		//	IOGSLogin((SDHP_RESULT *)aRecv);
		//	break;
		case 0x02: //Commands
		{
			PMSG_IOGSCOMMAND * lpMsg = (PMSG_IOGSCOMMAND *)aRecv;
			if(GameServerCommandCase(lpMsg->CmdNum) == false)
			{
				switch(lpMsg->CmdNum)
				{
					case 0x00:
					{
					}break;
				}
			}
		}break;
	}
}