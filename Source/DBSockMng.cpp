// ------------------------------
// Decompiled by Deathway
// Date : 2007-03-09
// ------------------------------

// GS-N 0.99.60T Status : Completed
//	GS-N	1.00.18	JPN	0x00423E80	-	Completed

#include "stdafx.h"
#include "..\common\MyWinsockBase.h"
#include "wsJoinServerCli.h"
#include "GameServer.h"
#include "GameMain.h"
#include "DBSockMng.h"
#include "DSProtocol.h"
#include "logproc.h"
#include "SCFExDBProtocol.h"
#include "ExServerProtocol.h"
#include "SCFPostServerProtocol.h"
#include "IOGSProtocol.h"
#include "SCFPack.h"



DBSockMng cDBSMng;	// DataServer Manager

DBSockMng::DBSockMng() 
{
	this->m_ActiveServer=DS_UNINITIALIZED;
}

DBSockMng::~DBSockMng()
{

}


void DBSockMng::Connect()
{
	
	if (this->wsCDBS[0].GetSocket()  == INVALID_SOCKET )
	{
		int State;
		this->wsCDBS[0].CreateSocket(ghWnd);
		this->wsCDBS[0].SetProtocolCore(DataServerProtocolCore);
		State=this->wsCDBS[0].Connect(DataServerIp, DataServerPort, WM_GM_DATA1_CLIENT_MSG_PROC);
		if (State == 0)
		{
			LogAdd(lMsg.Get(MSGGET(1, 147)), DataServerIp, DataServerPort);
			this->wsCDBS[0].Close();
		}
		else
		{
			LogAdd(lMsg.Get(MSGGET(1, 146)), DataServerIp, DataServerPort);	
			this->m_ActiveServer=DS_0;
			DataServerLogin(DS_0);
		}
	}
	
	if (this->wsCDBS[1].GetSocket() == INVALID_SOCKET)	
	{
		int State;
		this->wsCDBS[1].CreateSocket(ghWnd);
		this->wsCDBS[1].SetProtocolCore(DataServerProtocolCore);
		State=this->wsCDBS[1].Connect(DataServerIp2, DataServerPort2, WM_GM_DATA2_CLIENT_MSG_PROC);// Two ports more than the used1
		if (State==0)
		{
			LogAdd(lMsg.Get(MSGGET(1, 147)), DataServerIp2, DataServerPort2);
			this->wsCDBS[1].Close();
		}
		else
		{
			LogAdd(lMsg.Get(MSGGET(1, 146)), DataServerIp2, DataServerPort2);
			if (this->m_ActiveServer==-1)
			{
				this->m_ActiveServer=DS_1;
			}
			DataServerLogin(DS_1);	// Second DataServer
		}
	}
	
	if ( wsExDbCli.GetSocket()  == INVALID_SOCKET)
	{
		LogAddC(2, "ExDB Server Check..");
		wsExDbCli.CreateSocket(ghWnd);
		ExDataServerConnect(ExDbIp, WM_GM_EXDATA_CLIENT_MSG_PROC);
	}
	else
	{
		if ( wsExDbCli.GetConnect() == 0 )
		{
			LogAddC(2, "ExDB Server Check..");
			ExDataServerConnect(ExDbIp, WM_GM_EXDATA_CLIENT_MSG_PROC);
		}
	}

	//SCFPostServer
	
	if(ReadConfig.SCFPSON == 1)
	{
		if ( wsSCFPSCli.GetSocket()  == INVALID_SOCKET)
		{
			LogAddC(2, "SCFPostServer Check..");
			wsSCFPSCli.CreateSocket(ghWnd);
			SCFPSServerConnect(ReadConfig.SCFPSIP,ReadConfig.SCFPSPort, WM_GM_SCFPS_CLIENT_MSG_PROC);
		}
		else
		{
			if ( wsSCFPSCli.GetConnect() == 0 )
			{
				LogAddC(2, "SCFPostServer Check..");		
				SCFPSServerConnect(ReadConfig.SCFPSIP,ReadConfig.SCFPSPort, WM_GM_SCFPS_CLIENT_MSG_PROC);
			}
		}
	}
	
	//Extra Server

	if ( wsExServerCli.GetSocket()  == INVALID_SOCKET)
	{
		LogAddC(2, "Extra Server Check..");
		wsExServerCli.CreateSocket(ghWnd);
		ExServerConnect(ReadConfig.SCFEXSIP,ReadConfig.SCFEXSPort, WM_GM_EXS_CLIENT_MSG_PROC);
	}
	else
	{
		if ( wsExServerCli.GetConnect() == 0 )
		{
			LogAddC(2, "Extra Server Check..");		
			ExServerConnect(ReadConfig.SCFEXSIP,ReadConfig.SCFEXSPort, WM_GM_EXS_CLIENT_MSG_PROC);
		}
	}
	
	//IOGS Server
	
	if(ReadConfig.SCFIOGSON == 1)
	{
		if ( wsIOGSServerCli.GetSocket()  == INVALID_SOCKET)
		{
			LogAddC(2, "Titan IOGS Check..");
			wsIOGSServerCli.CreateSocket(ghWnd);
			IOGSConnect(ReadConfig.SCFIOGSIP,ReadConfig.SCFIOGSPort, WM_GM_IOGS_CLIENT_MSG_PROC);
		}
		else
		{
			if ( wsIOGSServerCli.GetConnect() == 0 )
			{
				LogAddC(2, "Titan IOGS Check..");		
				IOGSConnect(ReadConfig.SCFIOGSIP,ReadConfig.SCFIOGSPort, WM_GM_IOGS_CLIENT_MSG_PROC);
			}
		}
	}

	////SCFPack
	//
	//
	//if ( wsSCFPackCli.GetSocket()  == INVALID_SOCKET)
	//{
	//	LogAddC(2, "SCF Pack Server Check..");
	//		wsSCFPackCli.CreateSocket(ghWnd);
	//		SCFPackServerConnect("24.232.17.133",55901, WM_GM_SCFPACK_CLIENT_MSG_PROC);
	//}
	//else
	//{
	//	if ( wsSCFPackCli.GetConnect() == 0 )
	//	{
	//		LogAddC(2, "SCF Pack Server Check..");
	//		SCFPackServerConnect("24.232.17.133",55901, WM_GM_SCFPACK_CLIENT_MSG_PROC);
	//	}
	//}
}

void DBSockMng::ActiveChange()
{
	{
		int Status;
		if (this->m_ActiveServer==DS_0)
		{
			this->m_ActiveServer=DS_1;
			Status=1;
		}
		else
		{
			this->m_ActiveServer=DS_0;
			Status=0;
		}
		LogAdd("Active DB Change");
	}
}

int DBSockMng::Send(char* buf, int len)
{
	if (this->m_ActiveServer >= 2)
	{
		return 0;
	}
	if (this->m_ActiveServer < 0)
	{
		return 0;
	}
	if ( this->wsCDBS[this->m_ActiveServer].DataSend((char*)buf, len) == 0)
	{
		this->ActiveChange();
		this->wsCDBS[this->m_ActiveServer].DataSend((char*)buf, len);
	}
	return 1;

}


int DBSockMng::Send(int server, char* buf, int len)
{
	if (server >= 2)
	{
		return 0;
	}
	if (server < 0)
	{
		return 0;
	}
	if (this->wsCDBS[server].DataSend(buf, len) == 0)
	{
		this->ActiveChange();
	}
	return 1;
}


int DBSockMng::MsgProc(int line, WPARAM wParam, LPARAM lParam)
{
	switch (lParam & 0xFFFF & 0xFFFF)
	{
	case 1: // Recv Data
		this->wsCDBS[line].DataRecv();
		break;
	case 2: // Send Data
		this->wsCDBS[line].FDWRITE_MsgDataSend();
		break;
	case 32: // Close
		this->wsCDBS[line].Close();
	}
	return 1;
}


			





