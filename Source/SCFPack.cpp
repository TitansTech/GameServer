#include "stdafx.h"
//#include "..\common\MyWinsockBase.h"
//#include "wsJoinServerCli.h"
//#include "GameServer.h"
//#include "GameMain.h"
//#include "DBSockMng.h"
//#include "SCFPack.h"
//#include "logproc.h"
//#include "..\common\winutil.h"
//#include "PCShop.h"
//#include "CustomQuest.h"
//#include "DSProtocol.h"
//#include "ObjBotPet.h"
//
//wsJoinServerCli wsSCFPackCli;
//
//void SCFPackClientMsgProc(WPARAM wParam, LPARAM lParam)
//{
//	switch ( lParam & 0xFFFF & 0xFFFF )
//	{
//		case 1:
//			wsSCFPackCli.DataRecv();
//			break;
//
//		case 2:
//			wsSCFPackCli.FDWRITE_MsgDataSend();
//			break;
//
//		case 32:
//			LogAddC(2, "SCF Pack Close");
//			wsSCFPackCli.Close();
//			break;
//	}
//}
//
//
//BOOL SCFPackServerConnect(char* ConnectServer,int SCFExDBPort ,DWORD wMsg)
//{
//	wsSCFPackCli.SetProtocolCore(SCFPackServerProtocolCore);
//	int result = wsSCFPackCli.Connect(ConnectServer, SCFExDBPort, wMsg);
//
//	if ( result == 0 )
//	{
//		LogAdd("Error: Cant Connect To SCF Pack (%s:%d)", ConnectServer, SCFExDBPort);
//		return FALSE;
//	}
//
//	SCFPackServerLogin();
//	LogAddC(2, "SCF Pack Server Online");
//	return TRUE;
//}
//
//void SCFPackServerProtocolCore(BYTE protoNum, LPBYTE aRecv, int aLen)
//{	
//	switch ( protoNum )
//	{
//		case 0x00:
//			SCFPackServerLoginResult((SDHP_RESULT *)aRecv);
//			break;
//	}
//}
//
////Connection
//
//void SCFPackServerLogin()
//{
//	char tmp [512]={0};
//	SDHP_CLIENTINFO pInfo;
//
//	PHeadSetW((LPBYTE)&pInfo, 0x00, sizeof(pInfo));  
//#if (WL_PROTECT==1)
//	WLRegGetLicenseHardwareID(pInfo.HardID);
//	WLRegGetLicenseInfo(pInfo.pName,pInfo.pCompanyName,tmp);
//#endif
//
//	wsSCFPackCli.DataSend((char*)&pInfo, sizeof(pInfo));
//}
//
//void SCFPackServerLoginResult(SDHP_RESULT * lpMsg)
//{
//	if ( lpMsg->Result == false )
//	{
//		LogAddTD("SCF Pack Server Login Fail: %d", lpMsg->Result);
//
//		return;
//	}	
//}