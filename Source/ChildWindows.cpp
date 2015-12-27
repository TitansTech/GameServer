#include "stdafx.h"
#include "GameServer.h"
#include "LogProc.h"
#include "GameMain.h"
#include "User.h"
#include "Resource.h"
#include "..\common\winutil.h"
#include "..\include\readscript.h"
#include "ChildWindows.h"
#include "TServerInfoDisplayer.h"
#include "DBSockMng.h"
#include "SCFExDBProtocol.h"
#include "ExServerProtocol.h"
#include "IOGSProtocol.h"
#include "SCFPostServerProtocol.h"
#include "BloodCastle.h"
#include "DevilSquare.h"
#include "ChaosCastle.h"
#include "Kanturu.h"
#include "BlueEvent.h"
#include "BossAttack.h"
#include "DoppelGanger.h"
#include "GreenEvent.h"
#include "HappyHour.h"
#include "HitAndUp.h"
#include "IllusionTemple.h"
#include "ImperialGuardian.h"
#include "MossShop.h"
#include "Raklion.h"
#include "SkyEvent.h"
#include "SummerEvent.h"
#include "SwampEvent.h"
#include "XMasEvent.h"
#include "EledoradoEvent.h"
#include "Crywolf.h"
#include "CrywolfSync.h"
#include "CastleSiege.h"
#include "CastleDeepEvent.h"
#include "MDI.h"
#include "ChildLog.h"


cChildWindow childLog;
cChildWindow childEventLog;
CHILDLOG linkLog;
CHILDLOG eventChildLog;



#if(PROXY_SHOW == 1)
cChildWindow childProxy;
#endif

cStatusBar statusParent;

extern BOOL AutoClose;

void LinksChecker()
{
	BYTE LinkDown=0;
	
	if(cDBSMng.wsCDBS[0].GetSocket() == INVALID_SOCKET)
	{
		linkLog.LogAddTD("[OFFLINE] DataServer 1");
		LinkDown++;
	}

	if(cDBSMng.wsCDBS[1].GetSocket() == INVALID_SOCKET)
	{
		linkLog.LogAddTD("[OFFLINE] DataServer 2");
		LinkDown++;
	}

	if(wsJServerCli.GetSocket() == INVALID_SOCKET)
	{
		for ( int n=OBJ_STARTUSERINDEX; n< OBJMAX; n++)
		{
			if ( gObj[n].Connected > PLAYER_EMPTY )
			{
				if ( gObj[n].Type == OBJ_USER )
				{
					CloseClient(n);
				}
			}
		}
		linkLog.LogAddTD("[OFFLINE] JoinServer");
		wsJServerCli.Close();
		wsJServerCli.CreateSocket(ghWnd);
		GMJoinServerConnect(JoinServerIp, WM_GM_JOIN_CLIENT_MSG_PROC);
		LinkDown++;
	}

	if(wsExDbCli.GetSocket() == INVALID_SOCKET)
	{
		linkLog.LogAddTD("[OFFLINE] ExDB");
	}

	if(ReadConfig.SCFPSON == 1)
	{
		if ( wsSCFPSCli.GetSocket()  == INVALID_SOCKET)
		{
			linkLog.LogAddTD("[OFFLINE] PostServer");
		}
	}

	if(wsExServerCli.GetSocket() == INVALID_SOCKET)
	{
		linkLog.LogAddTD("[OFFLINE] Extra Server");
	}

	if(ReadConfig.SCFIOGSON == 1)
	{
		if ( wsIOGSServerCli.GetSocket()  == INVALID_SOCKET)
		{
			linkLog.LogAddTD("[OFFLINE] IOGS");
		}
	}

	if(ReadConfig.SCFESON == FALSE)
	{
		if(IsEventChipServerConnected == FALSE)
		{
			linkLog.LogAddTD("[OFFLINE] EventServer");
			if ((EventChipServerConnect!=0) && (IsEventChipServerConnected==0))
			{
				wsEvenChipServerCli.Close();
				wsEvenChipServerCli.CreateSocket(ghWnd);
				if (GMEventChipServerConnect(gEventChipServerIp, WM_GM_EVENTCHIP_CLIENT_MSG_PROC) == 0)
				{
					goto RSCheck;
				}
				IsEventChipServerConnected=1;
			}
		}
	}
RSCheck:
	if(ReadConfig.SCFRSON == FALSE)
	{
		if(IsDevilSquareEventConnected == FALSE)
		{
			linkLog.LogAddTD("[OFFLINE] RankingServer");
			
			if ((DevilSquareEventConnect==0) && (IsDevilSquareEventConnected==0))
			{
				wsRServerCli.Close();
				wsRServerCli.CreateSocket(ghWnd);
				if (GMRankingServerConnect(gDevilSquareEventServerIp, WM_GM_RANKING_CLIENT_MSG_PROC) == 0)
				{
					goto LastCheck;
				}
				IsDevilSquareEventConnected=1;
			}				
		}
	}
LastCheck:
	if(AutoClose == TRUE && LinkDown == 0)
	{
		AutoClose = FALSE;
		HWND pWnd = FindWindow(NULL, "Close");

		if(pWnd != NULL)
		{
			SendMessage(pWnd,WM_COMMAND,IDNO,0);
			//DestroyWindow(pWnd);
		}
	}
}


void EventsChecker()
{
	for ( int i =0; i< MAX_BLOOD_CASTLE_LEVEL ; i++ )
	{
		if(g_BloodCastle.GetCurrentState(i) == BC_STATE_PLAYING)
		{
			eventChildLog.LogAddTD("BloodCastle");
			break;
		}
	}

#if (GS_CASTLE==1)
	if(g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE)
	{
		eventChildLog.LogAddTD("CastleSiege");
	}
	
	if(g_CastleDeepEvent.m_iEVENT_STATE == CASTLE_STATE_PLAYING)
	{
		eventChildLog.LogAddTD("CastleDeepEvent");
	}
#endif
	
	for ( int i =0; i< MAX_CHAOSCASTLE_LEVEL ; i++ )
	{
		if(g_ChaosCastle.GetCurrentState(i) == CC_STATE_PLAYING)
		{
			eventChildLog.LogAddTD("ChaosCastle");
			break;
		}
	}

#if (GS_CASTLE==0)
	if(g_CrywolfSync.GetCrywolfState() >= CRYWOLF_STATE_NOTIFY_1 && g_CrywolfSync.GetCrywolfState() <= CRYWOLF_STATE_END  )
	{
		eventChildLog.LogAddTD("Crywolf");
	}
#endif
#if (GS_CASTLE==1)
	if(g_Crywolf.GetCrywolfState() >= CRYWOLF_STATE_NOTIFY_1 && g_Crywolf.GetCrywolfState() <= CRYWOLF_STATE_END  )
	{
		eventChildLog.LogAddTD("Crywolf");
	}
#endif

	if(g_DevilSquare.GetState() == DevilSquare_PLAYING)
	{
		eventChildLog.LogAddTD("DevilSquare");
	}

	if(DragonEvent->GetState() > 0)
	{
		eventChildLog.LogAddTD("DragonEvent");
	}

	if(gEledoradoEvent.GetEventState() > 0)
	{
		eventChildLog.LogAddTD("EldoradoEvent");
	}

#if (GS_CASTLE==0)
	if(g_Kanturu.GetKanturuState() >= KANTURU_STATE_BATTLE_OF_MAYA && g_Kanturu.GetKanturuState() < KANTURU_STATE_END)
	{
		eventChildLog.LogAddTD("Kanturu");
	}
#endif

#if (PACK_EDITION>=1)
	if(BlueEvent.Start == 1)
	{
		eventChildLog.LogAddTD("BlueEvent");
	}
#endif

#if (PACK_EDITION>=3)
	if(BossAttack.Start == 1)
	{
		eventChildLog.LogAddTD("BossAttack");
	}
#endif

#if (PACK_EDITION>=1)
	if(GreenEvent.Start == 1)
	{
		eventChildLog.LogAddTD("GreenEvent");
	}

	if(HappyHour.Start == 1)
	{
		eventChildLog.LogAddTD("HappyHour");
	}
#endif

#if (PACK_EDITION>=2)
	if(HitAndUp.Start == 1)
	{
		eventChildLog.LogAddTD("HitAndUp");
	}
#endif
	if(IllusionTemple.EventStart == 1)
	{
		eventChildLog.LogAddTD("IllusionTemple");
	}

	if(g_ImperialGuardian.Occuped == 1)
	{
		eventChildLog.LogAddTD("ImperialGuardian");
	}

	if(MossShop.Start == 1)
	{
		eventChildLog.LogAddTD("MossMerchant");
	}

	if(Raklion.Start == 1)
	{
		eventChildLog.LogAddTD("Raklion");
	}

#if (PACK_EDITION>=2)
	if(SkyEvent.Start == 1)
	{
		eventChildLog.LogAddTD("SkyEvent");
	}
#endif

#if (PACK_EDITION>=1)
	if(SummerEvent.Start == 1)
	{
		eventChildLog.LogAddTD("SummerEvent");
	}
#endif

#if (GS_CASTLE==1)
#if (PACK_EDITION>=3)
	if(g_Swamp.Start == 1)
	{
		eventChildLog.LogAddTD("SwampEvent");
	}
#endif
#endif

#if (PACK_EDITION>=2)
	if(XMasEvent.Start == 1)
	{
		eventChildLog.LogAddTD("XMasEvent");
	}
#endif
}


ATOM cChildWindow::MyRegisterClass(HINSTANCE hInstance, WNDPROC WndProcEx)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProcEx;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_NORMAL_V1);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= this->ClassName;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_ICON_MINIMIZED);

	return RegisterClassEx(&wcex);
}
 
BOOL cChildWindow::InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;
	hInst=hInstance;

    hWnd = CreateWindowEx(
        WS_EX_MDICHILD,
        this->ClassName,
        this->WindowName,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, 250,400,
        mdi.hwndParent, NULL, hInstance, NULL);
	
	if (hWnd==0) 
	{
		return FALSE;
	}

	this->hwnd=hWnd;

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

#define WM_FUNCTIMER			0x333FF
#define WM_EVENTFUNCTIMER		0x333FE

BOOL cChildWindow::Init(char * Class, char * Name, HINSTANCE hInstance, WNDPROC WndProcEx)
{
	if(this->hwnd == NULL)
	{
		strcpy(ClassName,Class);
		strcpy(WindowName,Name);
		
		// Check if error ocurs
		this->MyRegisterClass(hInstance,WndProcEx);
		if (!this->InitInstance(hInstance, SW_SHOWNORMAL))
		{
			return FALSE;
		}

		SetTimer(this->hwnd, WM_EVENTFUNCTIMER, 600000, NULL);
		SetTimer(this->hwnd, WM_FUNCTIMER, 2000, NULL);
		return TRUE;
	}
	return FALSE;
}


LRESULT CALLBACK WndProc2(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	INT wmId, wmEvent;
	
	switch (message) 
	{
	  case WM_CREATE:
		{
			linkLog.LogInit(TRUE,"LINKCHECKER_Log");
			return 0;
		}break;
	case WM_SIZE:
		{
			return DefMDIChildProc(hWnd,message,wParam,lParam);
		}break;
		
		case WM_TIMER:
			switch ( wParam )
			{
				case WM_FUNCTIMER:
					{
						LinksChecker();
						RECT rect;
						HDC hdc = GetDC(childLog.hwnd);
						GetClientRect(childLog.hwnd, &rect);
						FillRect(hdc, &rect, (HBRUSH)GetStockObject(0));
						ReleaseDC(childLog.hwnd, hdc);
						linkLog.LogTextPaint(childLog.hwnd);
					}
					break;
			}break;

		case WM_DESTROY:
		{
			childLog.hwnd = NULL;
		}break;
	default:
		return DefMDIChildProc(hWnd,message,wParam,lParam);
	}
	return 0;
}


LRESULT CALLBACK WndProc3(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	INT wmId, wmEvent;
	
	switch (message) 
	{
	  case WM_CREATE:
		{
			eventChildLog.LogInit(TRUE,"EVENTCHECKER_Log");
			return 0;
		}break;
	case WM_SIZE:
		{
			return DefMDIChildProc(hWnd,message,wParam,lParam);
		}break;
		
		case WM_TIMER:
			switch ( wParam )
			{
				case WM_EVENTFUNCTIMER:
					{
						EventsChecker();
						RECT rect;
						HDC hdc = GetDC(childEventLog.hwnd);
						GetClientRect(childEventLog.hwnd, &rect);
						FillRect(hdc, &rect, (HBRUSH)GetStockObject(0));
						ReleaseDC(childEventLog.hwnd, hdc);
						eventChildLog.LogTextPaint(childEventLog.hwnd);
					}
					break;
			}break;

		case WM_DESTROY:
		{
			childEventLog.hwnd = NULL;
		}break;
	default:
		return DefMDIChildProc(hWnd,message,wParam,lParam);
	}
	return 0;
}

cStatusBar statusBar;

void cStatusBar::Create(HWND parent)
{
	this->hStatus = NULL;
	this->MsgCount = 0;
	memset(&this->Msg,0,sizeof(this->Msg));
	this->hStatus = CreateWindowEx(WS_EX_TOPMOST,  STATUSCLASSNAME, NULL,
			WS_CHILD | WS_VISIBLE | WS_DISABLED | SBARS_SIZEGRIP, 0, 0, 0, 0,
			parent, (HMENU)IDC_MAIN_STATUS, GetModuleHandle(NULL), NULL);
}

int cStatusBar::AddText(char * msg)
{
	if(this->hStatus != NULL)
	{
		if(this->MsgCount > 0)
			this->Msg[this->MsgCount] = strlen(msg)*6.5f + this->Msg[MsgCount-1];
		else
			this->Msg[this->MsgCount] = strlen(msg)*6.5f;
		this->MsgCount++;
		SendMessage( this->hStatus, SB_SETPARTS, this->MsgCount, (LPARAM) this->Msg);
		SendMessage((HWND) this->hStatus,(UINT) SB_SETTEXT,(WPARAM) this->MsgCount-1, (LPARAM) msg);
		return (this->MsgCount-1);
	}
	return -1;
}

void cStatusBar::ModText(int index,char * msg)
{
	if(this->hStatus != NULL)
	{
		if(index > 0)
		{
			int old = this->Msg[index];
			this->Msg[index] = this->Msg[index-1] + strlen(msg)*6.5f;

			for(int i=index+1;i<this->MsgCount;i++)
			{
				this->Msg[i] = this->Msg[i] - old + this->Msg[index];
			}
		}
		else
		{
			int old = this->Msg[index];
			this->Msg[index] = strlen(msg)*6.5f;
			for(int i=1;i<this->MsgCount;i++)
			{
				this->Msg[i] = this->Msg[i] - old + this->Msg[index];
			}
		}

		SendMessage( this->hStatus, SB_SETPARTS, this->MsgCount, (LPARAM) this->Msg);
		SendMessage((HWND) this->hStatus,(UINT) SB_SETTEXT,(WPARAM) index, (LPARAM) msg);
	}
}

void cStatusBar::AutoSize(HWND parent)
{
	if(this->hStatus != NULL)
	{
		HWND hTool;
		RECT rcTool;
		int iToolHeight;

		HWND hStatus;
		RECT rcStatus;
		int iStatusHeight;

		HWND hEdit;
		int iEditHeight;
		RECT rcClient;

		// Size toolbar and get height

		hTool = GetDlgItem(parent, IDC_MAIN_TOOL);
		SendMessage(hTool, TB_AUTOSIZE, 0, 0);

		GetWindowRect(hTool, &rcTool);
		iToolHeight = rcTool.bottom - rcTool.top;

		// Size status bar and get height

		hStatus = GetDlgItem(parent, IDC_MAIN_STATUS);
		SendMessage(hStatus, WM_SIZE, 0, 0);

		GetWindowRect(hStatus, &rcStatus);
		iStatusHeight = rcStatus.bottom - rcStatus.top;

		// Calculate remaining height and size edit

		GetClientRect(parent, &rcClient);

		iEditHeight = rcClient.bottom - iToolHeight - iStatusHeight;

		hEdit = GetDlgItem(parent, IDC_MAIN_EDIT);
		SetWindowPos(hEdit, NULL, 0, iToolHeight, rcClient.right, iEditHeight, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOMOVE);
	}
}

#if(PROXY_SHOW == 1)

CHILDLOG proxyLog;

void to_hex(char buf[], int i)
{
  if (*buf == '\0')
    return;

  to_hex(buf + 2, i+0);
  buf[1] = strtol(buf, NULL, 16);
  *buf = '\0';
}
char * to_hex2(char buf[])
{
	int c=0;
	char buf2[2000] = {0};
	for (int i = 1; buf[i] != '\0'; i += 2)
	{
		buf2[c]=buf[i];
		c++;
	}
	return (char *)buf2;
}
char * ReplaceSpace(char buf[])
{
	int c=0;
	char buf2[2000] = {0};
	for (int i = 0; buf[i] != '\0'; i ++)
	{
		if(buf[i] != ' ')
		{
			buf2[c]=buf[i];
			c++;
		}
	}
	return (char *)buf2;
}
void ProxyBytes(char * buf)
{
	char * tmp1  = ReplaceSpace(buf);
	memset(buf,0,2000);
	strcpy(buf, tmp1);
	to_hex(buf,0);
	char * tmp2  = to_hex2(buf);
	memset(buf,0,2000);
	strcpy(buf, tmp2);
}
int Proxy_aIndex = 0;
BYTE Proxy_ShowSend = 0;
BYTE Proxy_ShowRecv = 0;
short RecvFilter = -1;
short SendFilter = -1;

void ToHexSpace(unsigned char * out, unsigned char * data, int len)
{
	std::string s,t;
	char cBuff[5000];
	for(int i=0;i<len;i++)
	{
		wsprintf(cBuff,"%02X ",data[i]);
		s=cBuff;
		t+=s;
	}
	memcpy(out,t.c_str(),t.length());
}

void WriteProxyPacket(int aIndex, unsigned char * buf, int Size, BYTE Type)
{
	if(aIndex == Proxy_aIndex)
	{
		char Text2[4000]={0};
		char Text[4500]={0};
		BYTE pType;

		if(buf[0] == 0xC2 || buf[0] == 0xC4)
		{
			pType = buf[3];
		}else
		{
			pType = buf[2];
		}

		if(Type == 0)
		{
			if(Proxy_ShowRecv == 1 && (RecvFilter == -1 || RecvFilter == pType))
			{
				ToHexSpace((unsigned char *)Text, buf, Size);
				proxyLog.LogAddC(3,"[RECV][%d] %s",aIndex,Text);
			}
		}else
		{
			if(Proxy_ShowSend == 1 && (SendFilter == -1 || SendFilter == pType))
			{
				ToHexSpace((unsigned char *)Text, buf, Size);
				proxyLog.LogAddC(4,"[SEND][%d] %s",aIndex,Text);
			}
		}
	}
}

INT_PTR CALLBACK ProxyWindow(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
		case WM_CHAR:
		{
			if(wParam == VK_RETURN)
			{
				goto SendMsg;
			}
		}break;

		case WM_COMMAND:
			if (LOWORD(wParam) == ID_PROXY_OK )
			{
	SendMsg:	
				char Text[2000]={0};
				GetDlgItemText(hDlg,IDC_PROXY_EDIT1, Text,2000);

				if(!strncmp(Text,"/index ",7))
				{
					int Take = 0;
					sscanf((LPCSTR)(Text+7),"%d",&Take);

					if(gObjIsAccontConnectProxy(Take))
					{
						Proxy_aIndex = Take;
						proxyLog.LogAdd("Index changed to: %d",Proxy_aIndex);
						Proxy_ShowSend = 0;
						Proxy_ShowRecv = 0;
						SendFilter = -1;
						RecvFilter = -1;
						proxyLog.LogAdd("Show Recv Changed to: %d",Proxy_ShowRecv);
						proxyLog.LogAdd("Show Send Changed to: %d",Proxy_ShowSend);
						proxyLog.LogAdd("Recv Filter Changed to: %d",RecvFilter);
						proxyLog.LogAdd("Send Filter Changed to: %d",SendFilter);
					}else
					{
						proxyLog.LogAdd("Bad Index or its offline: %d",Proxy_aIndex);
					}
				}else if(!strncmp(Text,"/showrecv ",10))
				{
					sscanf((LPCSTR)(Text+10),"%d",&Proxy_ShowRecv);
					proxyLog.LogAdd("Show Recv Changed to: %d",Proxy_ShowRecv);
				}else if(!strncmp(Text,"/showsend ",10))
				{
					sscanf((LPCSTR)(Text+10),"%d",&Proxy_ShowSend);
					proxyLog.LogAdd("Show Send Changed to: %d",Proxy_ShowSend);
				}else if(!strncmp(Text,"/recvfilter ",12))
				{
					sscanf((LPCSTR)(Text+12),"%d",&RecvFilter);
					proxyLog.LogAdd("Recv Filter Changed to: %d",RecvFilter);
				}else if(!strncmp(Text,"/sendfilter ",12))
				{
					sscanf((LPCSTR)(Text+12),"%d",&SendFilter);
					proxyLog.LogAdd("Send Filter Changed to: %d",SendFilter);
				}else
				{
					if(Proxy_aIndex != 0)
					{
						if(gObjIsAccontConnectProxy(Proxy_aIndex))
						{
							//char Text2[2000]={0};
							//strcpy(Text2,Text);
							ProxyBytes(Text);
							int Size = 1;
							if(((BYTE)Text[0]) == 0xC1 || ((BYTE)Text[0]) == 0xC3)
							{
								Size = Text[1];
							}else
							{
								Size = (((BYTE)Text[1])*0xFF)+((BYTE)Text[2]);
							}

							
							//BYTE FinalData[Size];
							//memcpy(FinalData, Text, Size);

							::DataSend(Proxy_aIndex,(unsigned char *)Text,Size);
							//wsprintf(tmp,"[SEND][%d] %s",Proxy_aIndex,Text2);
						}else
						{
							proxyLog.LogAdd("Bad Index or its offline: %d",Proxy_aIndex);
						}
					}else
					{
						proxyLog.LogAdd("Bad Index: %d",Proxy_aIndex);
					}
				}
				return (INT_PTR)TRUE;
			}
			if (LOWORD(wParam) == IDCANCEL )
			{
				EndDialog(hDlg, LOWORD(wParam));
				return (INT_PTR)TRUE;
			}
			break;
	}
	return (INT_PTR)FALSE;
}
LRESULT CALLBACK WndProc4(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	INT wmId, wmEvent;
	
	switch (message) 
	{
	  case WM_CREATE:
		{
			proxyLog.LogInit(TRUE,"ProxyLog");
			return 0;
		}break;
	case WM_LBUTTONUP:
		{
			goto ShowProxyWin;
		}break;
	case WM_CHAR:
		{
			if(wParam == VK_RETURN)
			{
ShowProxyWin:
				DialogBox(hInst, MAKEINTRESOURCE(IDD_PROXYBOX), hWnd, ProxyWindow);
			}
		}break;
	case WM_SIZE:
		{
			return DefMDIChildProc(hWnd,message,wParam,lParam);
		}break;
		
		case WM_TIMER:
			switch ( wParam )
			{
				case WM_FUNCTIMER:
					{
						RECT rect;
						HDC hdc = GetDC(childProxy.hwnd);
						GetClientRect(childProxy.hwnd, &rect);
						FillRect(hdc, &rect, (HBRUSH)GetStockObject(0));
						ReleaseDC(childProxy.hwnd, hdc);
						proxyLog.LogTextPaint(childProxy.hwnd);
					}
					break;
			}break;
		
		//case WM_TIMER:
		//	switch ( wParam )
		//	{
		//		case WM_FUNCTIMER:
		//			EventsChecker();
		//			break;
		//	}break;
		case WM_DESTROY:
		{
			childProxy.hwnd = NULL;
		}break;
	default:
		return DefMDIChildProc(hWnd,message,wParam,lParam);
	}
	return 0;
}
#endif