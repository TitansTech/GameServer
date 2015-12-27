// ------------------------------
// Decompiled by Deathway
// Date : 2007-03-09
// ------------------------------
// GS-N 0.99.60T 0x0048C960
//	GS-N	1.00.18	JPN	0x004A7AD0	-	Completed

#include "stdafx.h"
#include "CWhatsUpDummyServer.h"
#include "GameServer.h"

WhatsUpDummyServerProc CWhatsUpDummyServer::m_lpOldProc=0;

CWhatsUpDummyServer::CWhatsUpDummyServer()
{
	WSADATA wsaVer;
	this->m_sckDUMMY = INVALID_SOCKET;	// INVALID_SOCKET
	this->m_hParentWnd = NULL;
	CWhatsUpDummyServer::m_lpOldProc=0;
	WSAStartup(WINSOCK_REQUESTED_VERSION, &wsaVer);
}
	

CWhatsUpDummyServer::~CWhatsUpDummyServer()
{
	if ( this->m_sckDUMMY != INVALID_SOCKET ) // INVALID_SOCKET
	{
		closesocket(this->m_sckDUMMY );
	}
}


int CWhatsUpDummyServer::Start(HWND hWnd, WORD wPort)
{
	sockaddr_in sockinfo;
	int iRetVal;

	if ( hWnd == 0 )
	{
		return 0;
	}

	if ( !IsWindow(hWnd) )
	{
		return 0;
	}

	this->m_hParentWnd = hWnd;

	this->m_sckDUMMY=socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
	
	if ( this->m_sckDUMMY == INVALID_SOCKET ) // INVALID_SOCKET
	{
		return 0;
	}

	sockinfo.sin_family =AF_INET;
	sockinfo.sin_addr.S_un.S_addr=0;
	sockinfo.sin_port=htons(wPort);
	
	iRetVal=0;
	iRetVal=bind(this->m_sckDUMMY , (sockaddr*)&sockinfo,16);

	if (iRetVal == -1)
	{
		closesocket(this->m_sckDUMMY);
		this->m_sckDUMMY = INVALID_SOCKET;
		return 0;
	}

	iRetVal=listen(this->m_sckDUMMY, 2147483647 );

	if (iRetVal == -1 )
	{
		closesocket(this->m_sckDUMMY);
		this->m_sckDUMMY = INVALID_SOCKET;
		return 0;
	}

	iRetVal=WSAAsyncSelect(this->m_sckDUMMY , hWnd, WM_GM_CONNECT_SERVER_MSG, FD_ACCEPT);

	if (iRetVal == -1 )
	{
		closesocket(this->m_sckDUMMY);
		this->m_sckDUMMY = INVALID_SOCKET;
		return 0;
	}

#ifdef _WIN64
	CWhatsUpDummyServer::m_lpOldProc=(WhatsUpDummyServerProc)SetWindowLong(hWnd, GWLP_WNDPROC, (LONG)ParentWndProc  );
#else
	CWhatsUpDummyServer::m_lpOldProc=(WNDPROC)SetWindowLong(hWnd, GWL_WNDPROC, (LONG)ParentWndProc  );
#endif
	return 1;
}


LRESULT __cdecl CWhatsUpDummyServer::ParentWndProc(HWND hWnd,
													UINT iMessage,
													WPARAM wParam,
													LPARAM lParam)
{
	switch ( iMessage )
	{
		case WM_GM_CONNECT_SERVER_MSG:
			switch ( lParam & 0xFFFF & 0xFFFF )
			{
				case FD_ACCEPT:
					{
						sockaddr_in sockinfo;
						int isockinfolen=16;
						SOCKET sckTemp;
						int iRetVal;

						sckTemp = accept(wParam & 0xFFFF & 0xFFFF, (sockaddr *)&sockinfo, &isockinfolen);

						if ( sckTemp == INVALID_SOCKET )
							return FALSE;

						iRetVal = WSAAsyncSelect(sckTemp, hWnd, WM_GM_CONNECT_SERVER_MSG, FD_READ|FD_CLOSE);

						if ( iRetVal == -1 )
						{
							closesocket(sckTemp);
						}
						return FALSE;
					}
					break;
				case FD_READ:
					{
						char cBUFFER[256];
						int iRecvBytes = recv(wParam & 0xFFFF & 0xFFFF, cBUFFER, sizeof(cBUFFER), 0);

						if ( iRecvBytes <= 0 )
						{
							closesocket(wParam & 0xFFFF & 0xFFFF);
						}
						return FALSE;
					}
					break;
				case FD_CLOSE:
					closesocket(wParam & 0xFFFF & 0xFFFF);
					return FALSE;
				default:
					return FALSE;
			}
			break;
		default:
#ifdef _WIN64
			return CallWindowProcA((WNDPROC)CWhatsUpDummyServer::m_lpOldProc, hWnd, iMessage, wParam, lParam);
#else
			return CallWindowProcA(CWhatsUpDummyServer::m_lpOldProc, hWnd, iMessage, wParam, lParam);
#endif
	}
}