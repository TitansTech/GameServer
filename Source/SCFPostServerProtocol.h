#include "DSProtocol.h"
#include "stdafx.h"
#include "user.h"

extern wsJoinServerCli wsSCFPSCli;

#define WM_GM_SCFPS_CLIENT_MSG_PROC		0x420

struct SDHP_POSTINFO
{
	struct PBMSG_HEAD h;
	unsigned short ServerCode;
	char Name[11];
	char Chat[60];
};

struct SDHP_ALLSVRMSGSENDINFO
{
	struct PBMSG_HEAD h;
	unsigned short ServerCode;
	char Chat[60];
};

void SCFPSClientMsgProc(WPARAM wParam, LPARAM lParam);
void SCFPSServerProtocolCore(BYTE protoNum, LPBYTE aRecv, int aLen);
BOOL SCFPSServerConnect(char* ConnectServer,int SCFPSPort ,DWORD wMsg);
void SCFPostServerLogin();
void SCFPostServerLoginResult(SDHP_RESULT * lpMsg);
void SCFPostServerRecvPost(SDHP_POSTINFO * lpMsg);
void SCFPostSend(int aIndex, char * chat);
void SCFAllServerMsgRecv(SDHP_ALLSVRMSGSENDINFO * lpMsg);
void SCFAllServerMsgSend(char * chat);