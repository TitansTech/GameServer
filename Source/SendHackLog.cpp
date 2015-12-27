// ------------------------------
// Decompiled by Deathway
// Date : 2007-05-09
// ------------------------------
// GS-N 0.99.60T 0x453512 Status : Completed
//	GS-N	1.00.18	JPN	0x00462870	-	Completed
#include "stdafx.h"
#include "SendHackLog.h"
#include "..\common\winutil.h"
#include "GameMain.h"
#include "user.h"



CSendHackLog gSendHackLog;


CSendHackLog::CSendHackLog()
{
	return;
}


CSendHackLog::~CSendHackLog()
{
	return;
}


void CSendHackLog::SendSet(char * ip, int port)
{
	this->m_UdpSocCHackLog.CreateSocket();
	this->m_UdpSocCHackLog.SendSet(ip, port);
}




struct PMSG_HACK_LOG
{
	PBMSG_HEAD h;	// C1:01
	WORD wServer;	// 4
	char AccountID[11];	// 6
	char Name[11];	// 11
	BYTE btType;	// 1C
	char szPacketContext[50];	// 1D
};


void CSendHackLog::Send(int aIndex, BYTE type, LPSTR Context)
{
	PMSG_HACK_LOG Msg;
	
	PHeadSetB((LPBYTE)&Msg, 0x01, sizeof(Msg) );
	Msg.wServer = gGameServerCode;
	memcpy(Msg.AccountID, gObj[aIndex].AccountID , MAX_ACCOUNT_LEN );
	memcpy(Msg.Name, gObj[aIndex].Name, MAX_ACCOUNT_LEN );
	Msg.AccountID[MAX_ACCOUNT_LEN] = 0;
	Msg.Name[MAX_ACCOUNT_LEN] = 0;
	Msg.btType = type;
	memcpy(Msg.szPacketContext, Context, sizeof(Msg.szPacketContext) );

	this->m_UdpSocCHackLog.SendData((LPBYTE)&Msg, sizeof(Msg) );
}


BOOL CSendHackLog::SendData(LPBYTE SendData, DWORD nSendDataLen)
{
	return this->m_UdpSocCHackLog.SendData(SendData, nSendDataLen);
}
