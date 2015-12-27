
#define WM_GM_IOGS_CLIENT_MSG_PROC		0x440


struct PMSG_IOGSCOMMAND
{
	PBMSG_HEAD h;
	int CmdNum;
};

extern wsJoinServerCli wsIOGSServerCli;

void IOGSClientMsgProc(WPARAM wParam, LPARAM lParam);
BOOL IOGSConnect(char* ConnectServer,int Port ,DWORD wMsg);
void IOGSLogin();
void IOGSProtocolCore(BYTE protoNum, LPBYTE aRecv, int aLen);