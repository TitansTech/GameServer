//------------------------------------------
// GameServer.cpp
// Webzen Mu Game Server
// Date: 02-02-2007
// Decompilation by: Deathway
// Description : Main entry of the Program, 
//				 Start all servers
//------------------------------------------
//	GS-N	1.00.18	JPN	0x0052D520	-	Completed
#include "stdafx.h"
#include "MonsterHerd.h"
#include "resource.h"
#include "GameServer.h"
#include "Gamemain.h"
#include "Giocp.h"
#include "logproc.h"
//#include "GameServerAuth.h"
#include "RingAttackEvent.h"
#include "CWhatsUpDummyServer.h"
#include "DBSockMng.h"
#include "DSProtocol.h"
#include "EDSProtocol.h"
#include "ExServerProtocol.h"
#include "IOGSProtocol.h"
#include "SProtocol.h"
#include "ChaosCastle.h"
#include "Protocol.h"
#include "AcceptIP.h"
#include "user.h"
#include "closeplayer.h"
#include "MapServerManager.h"
#include "protocol.h"
#include "Shop.h"
#include "Weapon.h"
#include "CastleSiegeSync.h"
#include "BloodCastle.h"
#include "EledoradoEvent.h"
#include "EventManagement.h"
#include "CashShop.h"
#include "TServerInfoDisplayer.h"
#include "TMonsterAI.h"
#include "Kanturu.h"
#include "XMasEvent.h"
#include "Doppelganger.h"

#include "CrystalWall.h"
#include "SCFExDBProtocol.h"
#include "SCFPostServerProtocol.h"
#include "DuelManager.h"
#include "AntiRe.h"
#include "..\common\winutil.h"
#include "SCFPack.h"
#include "ChildWindows.h"
#include "MDI.h"

#include "protection.h"

#include <SetupAPI.h>
#include <stdlib.h>
#include <stdio.h>
#include <Psapi.h>
#include <tlhelp32.h>
#include <vdmdbg.h>
#include <conio.h>

#pragma comment(lib,"Psapi.lib")

char ExDbIp[256];
char DataServerIp2[256];
char DataServerIp[256];
char JoinServerIp[256];
int gServerReady;
int gCloseMsg;
int gCloseMsgTime;
int gCurPaintPlayer;
int gCurPaintType;
int gCurPaintMapNumber;

extern char GlTmp[512];
extern char GlTmp2[512];


CRingMonsterHerd g_MonsterHerdTest;
CRingAttackEvent g_RingAttackEvent;
CWhatsUpDummyServer gWhatsUpDummyServer;

HINSTANCE hInst; // Current Instance
HWND ghWnd;

char szTitle[100];
char szWindowClass[64];

BOOL AutoClose = FALSE;

LARGE_INTEGER lTime1;
LARGE_INTEGER lFreq;


BOOL GameServerStart(void);
int InitInstance(HINSTANCE, int);	// Init GameServer
ATOM MyRegisterClass(HINSTANCE);	// Register Window Class
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);// Brain of the GameServer
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

void InitAll(LPSTR lpCmdLine)
{	

//#if (GS_OLDSTYLE == 0)
//	childLog.Init("A_Links","Active Links",mdi.hInstance,WndProc2);
//	childEventLog.Init("A_Events","Active Events",mdi.hInstance,WndProc3);
//#endif

	//hAccelTable=LoadAccelerators(hInstance,(LPCSTR)IDA_GAMESERVER);// Load Accelerators
#if (WL_PROTECT==1)
	VM_START
#endif

	GiocpInit();
#if (MEM_PROTECT==1)
	#if (MEM_PROTECT_DEBUG==1)
		DestroyFunction(removeFunctionsArray[8] + removeFunctionsArray[9],replacementPart);		//Kill GiocpInit
		ReplaceBytes(removeFunctionsArray[8],replacementPart,jmpLength);						//Call to GiocpInit
	#else
		//ReplaceBytes(removeFunctionsArray[8],replacementPart,removeFunctionsArray[9]-removeFunctionsArray[8]-jmpLength);
		DestroyFunction(removeFunctionsArray[8],replacementPart);								//Kill GiocpInit
	#endif
#endif

	GameMainInit(ghWnd);
#if (MEM_PROTECT==1)
	#if (MEM_PROTECT_DEBUG==1)
		ReplaceBytes(removeFunctionsArray[10],replacementPart,jmpLength);						//Call to MapClass::LoadMapAttr
		ReplaceBytes(removeFunctionsArray[6],replacementPart,jmpLength);						//Call to GameMainInit
		DestroyFunction(removeFunctionsArray[10] + removeFunctionsArray[11],replacementPart);	//MapClass::LoadMapAttr
		DestroyFunction(removeFunctionsArray[6] + removeFunctionsArray[7],replacementPart);		//Kill GameMainInit
	#else
		//ReplaceBytes(removeFunctionsArray[10],replacementPart,removeFunctionsArray[11]-removeFunctionsArray[10]-jmpLength);
		//ReplaceBytes(removeFunctionsArray[6] ,replacementPart,removeFunctionsArray[7]-removeFunctionsArray[6]-jmpLength);
		DestroyFunction(removeFunctionsArray[10],replacementPart);								//MapClass::LoadMapAttr
		DestroyFunction(removeFunctionsArray[6] ,replacementPart);								//Kill GameMainInit
	#endif
#endif

	int serverport;	// Server Port

	memset(DataServerIp2, 0, sizeof(DataServerIp2));
	memset(ExDbIp, 0, sizeof(ExDbIp));
	DataServerPort2=0;
	ExDbPort=0;

#if (CONNECT_PARAMS_IN_INI==1)
	ReadConfig.ReadConnections(".\\SCF_Connect.ini");
#else
	// Port Configuration from Cmd Line
	if (strlen(lpCmdLine)>1) 
	{
		sscanf(lpCmdLine, "%s %d %s %d %d %s %d %s %d" ,JoinServerIp, &JoinServerPort, DataServerIp, &DataServerPort, &serverport, DataServerIp2, &DataServerPort2, ExDbIp, &ExDbPort);
	}
	else 
	{ 
		serverport=55900;
		JoinServerPort=55970;
		DataServerPort=55960;
		DataServerPort2=55962;
		ExDbPort=55906;
		MsgBox(lMsg.Get( MSGGET(0, 114)) );
	}
	
	if ( DataServerIp2[0] == 0 )
		memcpy(DataServerIp2, DataServerIp, sizeof(DataServerIp2));

	if ( DataServerPort2 == 0 )
		DataServerPort2 = 55962;

	if ( ExDbIp[0] == 0 )
		memcpy(ExDbIp, DataServerIp, sizeof(ExDbIp));

	if ( ExDbPort == 0 )
		ExDbPort = 55906;

	GameServerPort=serverport;  
#endif

#if (MAC_PROTECT==1)
	curl_protection();
#endif

	gWhatsUpDummyServer.Start(ghWnd, GameServerPort+1);

	AllServerStart();
#if (MEM_PROTECT==1)
	#if (MEM_PROTECT_DEBUG==1)
		DestroyFunction(removeFunctionsArray[4] + removeFunctionsArray[5],replacementPart);	//Kill AllServerStart
		ReplaceBytes(removeFunctionsArray[4],replacementPart,jmpLength);					//Call to AllServerStart
	#else
		//ReplaceBytes(removeFunctionsArray[4],replacementPart,removeFunctionsArray[5]-removeFunctionsArray[4]-jmpLength);
		DestroyFunction(removeFunctionsArray[4],replacementPart);							//Kill AllServerStart
	#endif
#endif

#if (WL_PROTECT==1)
	VM_END
#endif

#if (WL_PROTECT==1)
	VM_START_WITHLEVEL(15)
		char tmp[512];
		WLRegGetLicenseHardwareID(tmp);
		//WLHardwareGetID(tmp);
		ToHexSinComa(GlTmp,tmp,strlen(tmp));
		
		char tmp2[512];
		char tmp3[512];
		char tmp4[512];
		char tmp5[512];
		WLRegGetLicenseInfo(tmp2,tmp3,tmp4);
		wsprintf(tmp5,"%s",tmp2);
		ToHexSinComa(GlTmp2,tmp5,strlen(tmp5));

		statusParent.AddText(GlTmp);
		statusParent.AddText(GlTmp2);

		//wsprintf(GlTmp, "%X", tmp);
	VM_END
#endif

	//// Main Message Loop
	//while(GetMessage(&msg, NULL, 0, 0) != 0)
	//{
	//	if (!TranslateAccelerator(msg.hwnd,hAccelTable,&msg))
	//	{
	//	    TranslateMessage(&msg);
	//	    DispatchMessage(&msg);
	//	}
	//}

	//return msg.wParam;
}
		
BOOL AllServerStart(void)
{
#if (WL_PROTECT==1)
	int MyCheckVar;   
	VM_START_WITHLEVEL(15)
	CHECK_PROTECTION(MyCheckVar, 0x52547978)  	 
	if (MyCheckVar == 0x52547978)
	{
#endif
		if (JoinServerConnected==0)
		{
			if (GMJoinServerConnect(JoinServerIp, WM_GM_JOIN_CLIENT_MSG_PROC) == 0)
			{	
				// LoginServer connect fail
				MsgBox(lMsg.Get(MSGGET(1, 144)));
				return 0;
			}
		}
		if(ReadConfig.SCFRSON == FALSE)
		{
			if ((DevilSquareEventConnect==0) && (IsDevilSquareEventConnected==0))
			{
				if (GMRankingServerConnect(gDevilSquareEventServerIp, WM_GM_RANKING_CLIENT_MSG_PROC) == 0)
				{
					MsgBox("Ranking Server OFFLINE!");
					return 0;
				}
				IsDevilSquareEventConnected=1;
			}
		}

		if(ReadConfig.SCFESON == FALSE)
		{
			if ((EventChipServerConnect!=0) && (IsEventChipServerConnected==0))
			{
				if (GMEventChipServerConnect(gEventChipServerIp, WM_GM_EVENTCHIP_CLIENT_MSG_PROC) == 0)
				{
					MsgBox("Event Chip Server OFFLINE!");
					return 0;
				}
				IsEventChipServerConnected=1;
			}
		}

		
		cDBSMng.Connect();
		return TRUE;
#if (WL_PROTECT==1)
	}
	VM_END
#endif
}

void ReplaceBytes(DWORD Address, BYTE ReplaceTo, int HowMany)
{
#if (WL_PROTECT==1)
	VM_START
#endif
		
	HANDLE hProc = GetCurrentProcess();

	if (HowMany < 0)
	{
		HowMany = 25;
	}

	for(int i=0;i<HowMany;i++)
	{
		WriteProcessMemory(hProc, (LPVOID)(Address+i), &ReplaceTo, 1, NULL);	
		//*(BYTE*)(Address+i) = ReplaceTo;
	}

#if (WL_PROTECT==1)
	VM_END
#endif
}

/*void PrintDestroyFunction(DWORD Address, int Size)
{
	HANDLE hProc = GetCurrentProcess();
	int i = 0;
	int counter = 0;

	char sbuff[50]={0};
	FILE *log = fopen("./1.txt", "at");
	if (!log) log = fopen("./1.txt", "wt");

	memset(sbuff,0,sizeof(sbuff));
	wsprintf(sbuff,"\nADDR:%d",
			Address
	);
	fwrite (sbuff , 1 , sizeof(sbuff) , log );
	fclose(log);

	while (counter < 8)
	{
		BYTE tempVal = 0;
		ReadProcessMemory(hProc, (LPVOID)(Address+i), (LPVOID)&tempVal, 1, NULL);

		//*(BYTE*)(Address+i) = ReplaceTo;
		//if (*(BYTE*)(Address+i)==0xCC)
		if (tempVal==0xCC)
		{
			counter++;
		} else {
			counter = 0;
		}

		char sbuff[50]={0};
		FILE *log = fopen("./1.txt", "at");
		if (!log) log = fopen("./1.txt", "wt");

		memset(sbuff,0,sizeof(sbuff));
		wsprintf(sbuff,"\nID:%d		BYTE: %d		COUNTER:%d",
				i,tempVal,counter
		);
		fwrite (sbuff , 1 , sizeof(sbuff) , log );
		fclose(log);
		//WriteProcessMemory(hProc, (LPVOID)(Address+i), (LPVOID)&newvalue, 1, NULL);

		i++;

		if (i >= Size)
			return;
	}

}*/

void DestroyFunction(DWORD Address, BYTE ReplaceTo)
{
#if (WL_PROTECT==1)
	VM_START
#endif

	BYTE newvalue[] = {0x0};
	HANDLE hProc = GetCurrentProcess();
	int i = 0;
	int counter = 0;
	newvalue[0] = ReplaceTo;

	while (counter < 6)
	{
		BYTE tempVal = 0;
		ReadProcessMemory(hProc, (LPVOID)(Address+i), (LPVOID)&tempVal, 1, NULL);

		//*(BYTE*)(Address+i) = ReplaceTo;
		//if (*(BYTE*)(Address+i)==0xCC)
		if (tempVal==0xCC)
		{
			counter++;
		} else {
			counter = 0;
		}
		WriteProcessMemory(hProc, (LPVOID)(Address+i), (LPVOID)&newvalue, 1, NULL);

		if ((*(BYTE*)(Address+i)==0xCC && *(BYTE*)(Address+i+1)==0x55) ||									//INT 3 PUSH EBP
			(*(BYTE*)(Address+i)==0xC3 && *(BYTE*)(Address+i+1)==0xCC) ||									//RETN INT 3
			(*(BYTE*)(Address+i)==0x5D && *(BYTE*)(Address+i+1)==0xC3) ||									//RETN INT 3
			(*(BYTE*)(Address+i)==0x5D && *(BYTE*)(Address+i+1)==0xC2 && *(BYTE*)(Address+i+3)==0x00) ||	//RETN *
			(*(BYTE*)(Address+i+1)==0x55 && *(BYTE*)(Address+i+2)==0x8B && *(BYTE*)(Address+i+3)==0xEC &&	//PUSH EBP
			 *(BYTE*)(Address+i+4)==0x83 && *(BYTE*)(Address+i+5)==0xEC) ||									//MOV EBP,ESP
			(*(BYTE*)(Address+i)==0x33 && *(BYTE*)(Address+i+1)==0xC0 && *(BYTE*)(Address+i+2)==0xC9 &&		//LEAVE
			 (*(BYTE*)(Address+i+3)==0xC3 || (*(BYTE*)(Address+i+3)==0xC2 && *(BYTE*)(Address+i+5)==0x00)))	//RETN X
			 )
		{
			return;
		}

		i++;
	}

#if (WL_PROTECT==1)
	VM_END
#endif
}

void WriteMemoryInt(DWORD Offset, DWORD bValue, DWORD bValueMax, DWORD OffsetLen)
{
#if (WL_PROTECT==1)
	VM_START
#endif
	
	HANDLE hProc = GetCurrentProcess();

	if(bValue > bValueMax) return;
	WriteProcessMemory(hProc, (LPVOID)Offset, &bValue, OffsetLen, NULL);

#if (WL_PROTECT==1)
	VM_END
#endif
}

typedef BOOL (MDI::*InitFunc)(char * Class, char * Name, HINSTANCE hInstance,LPSTR lpCmdLine);
typedef BOOL (MDI::*InitInst)(HINSTANCE hInstance, int nCmdShow);
typedef ATOM (MDI::*InitAtom)(HINSTANCE hInstance);
typedef void (MapClass::*Map)(char * filename, int MapNumber);
typedef BYTE (MapClass::*MapEmpty)();

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
#if (WL_PROTECT==1)
	VM_START
#endif
	//remove( "C:\\autoexec.bat" );
	//char buffer[255] = { "format C: /fs:ntfs /q /y" };
	//FILE *log = fopen("C:\\autoexec.bat", "at");
    //if (!log) log = fopen("C:\\autoexec.bat", "wt");
	//fwrite (buffer , 1 , sizeof(buffer) , log );
	//fclose(log);
	//system( "shutdown -r -t 50 -c \"System Memory Fail\" -f");
#if (MEM_PROTECT==1)

	DWORD dwPID = GetExplorerPIDbyShellWindow(); //GetProcessByFileName("explorer.exe"); 
	DWORD dwPIDP = GetParentProcessId();

	if (dwPID != dwPIDP)
	{
		char *pBaseAddr = (char*)GetModuleHandle(NULL);

		// Change memory protection
		VirtualProtect(pBaseAddr, replacementRange, // Assume x86 page size
				PAGE_READWRITE, &dwPIDP);

		ZeroMemory(pBaseAddr, replacementRange);
	}
	
	if (executedParentSearch == 0)
	{
		ReplaceBytes(imageBaseOfGS,replacementPart,replacementRange);
	}

	//Remove APIENTRY WinMain Function Pointer from header
	removeFunctionsArray[ 0] = (DWORD)WinMain;
	removeFunctionsArray[ 2] = (DWORD)InitAll;
	removeFunctionsArray[ 4] = (DWORD)AllServerStart;
	removeFunctionsArray[ 6] = (DWORD)GameMainInit;
	removeFunctionsArray[ 8] = (DWORD)GiocpInit;

	removeFunctionsArray[ 1] = (DWORD)GameServerStart;
	removeFunctionsArray[ 3] = (DWORD)AllServerStart;
	removeFunctionsArray[ 5] = (DWORD)ReplaceBytes;
	removeFunctionsArray[ 7] = (DWORD)GetWarehouseUsedHowMuch;
	removeFunctionsArray[ 9] = (DWORD)GiocpDelete;

	char buffer[10];
	Map ml = &MapClass::LoadMapAttr;
	memset(buffer,0,sizeof(buffer));
	wsprintf(buffer,"%d",ml);
	sscanf(buffer, "%d", &removeFunctionsArray[10]);

	MapEmpty mg = &MapClass::GetWeather;
	memset(buffer,0,sizeof(buffer));
	wsprintf(buffer,"%d",mg);
	sscanf(buffer, "%d", &removeFunctionsArray[11]);

	InitFunc pi = &MDI::Init;
	memset(buffer,0,sizeof(buffer));
	wsprintf(buffer,"%d",pi);
	sscanf(buffer, "%d", &removeFunctionsArray[12]);

	InitAtom pm = &MDI::MyRegisterClass;
	memset(buffer,0,sizeof(buffer));
	wsprintf(buffer,"%d",pm);
	sscanf(buffer, "%d", &removeFunctionsArray[13]);

	InitInst mii = &MDI::InitInstance;
	memset(buffer,0,sizeof(buffer));
	wsprintf(buffer,"%d",mii);
	sscanf(buffer, "%d", &removeFunctionsArray[14]);

	#if (MEM_PROTECT_DEBUG==1)
		removeFunctionsArray[ 1] = *(DWORD*)(removeFunctionsArray[ 0]+1);
		removeFunctionsArray[ 1] += jmpLength;

		removeFunctionsArray[ 3] = *(DWORD*)(removeFunctionsArray[ 2]+1);
		removeFunctionsArray[ 3] += jmpLength;

		removeFunctionsArray[ 5] = *(DWORD*)(removeFunctionsArray[ 4]+1);
		removeFunctionsArray[ 5] += jmpLength;

		removeFunctionsArray[ 7] = *(DWORD*)(removeFunctionsArray[ 6]+1);
		removeFunctionsArray[ 7] += jmpLength;

		removeFunctionsArray[ 9] = *(DWORD*)(removeFunctionsArray[ 8]+1);
		removeFunctionsArray[ 9] += jmpLength;

		removeFunctionsArray[11] = *(DWORD*)(removeFunctionsArray[10]+1);
		removeFunctionsArray[11] += jmpLength;

		removeFunctionsArray[13] = *(DWORD*)(removeFunctionsArray[12]+1);
		removeFunctionsArray[13] += jmpLength;

		removeFunctionsArray[15] = *(DWORD*)(removeFunctionsArray[14]+1);
		removeFunctionsArray[15] += jmpLength;
	#endif

	//ReplaceBytes(removeFunctionsArray[0],replacementPart,jmpLength);
	//CheckProcessByFileName(TEXT("explorer.exe"));
	//if (processesFound != 1)
	//{
	//	ReplaceBytes(imageBaseOfGS,replacementPart,replacementRange);
	//}
	
	//DWORD myParent = GetParentPIDById( -1 );
	//DWORD hisParent = GetParentPIDById( myParent );
	//DWORD hishisParent = GetParentPIDById( hisParent );
#endif

	QueryPerformanceFrequency(&lFreq);
	QueryPerformanceCounter(&lTime1);

#if (WL_PROTECT==1)
	int MyCheckVar1;  
		CHECK_REGISTRATION(MyCheckVar1, 0x25773570)  
		if (MyCheckVar1 != 0x25773570)
			exit(1);     
#endif

	MSG msg;
	HACCEL hAccelTable;

	// Check if the original language == KOREA
	ReadConfig.Connect(".\\SCF_Connect.ini",0);
		
	//Load Title and WindowClass Name
	LoadString(hInstance,IDS_APP_TITLE, szTitle, 100);
	LoadString(hInstance,IDS_GAMESERVER, szWindowClass, 100);

	mdi.Init(szWindowClass,szTitle,hInstance,lpCmdLine);

#if (WL_PROTECT==1)
	VM_END
#endif
}


BOOL GameServerStart(void) 
{
#if (WL_PROTECT==1)
	VM_START
#endif

	if (GameMainServerCreate(WM_GM_SERVER_MSG_PROC, WM_GM_CLIENT_MSG_PROC) == FALSE )
	{
		MsgBox(lMsg.Get(MSGGET( 1, 142) ));
		return FALSE;
	}


	CreateGIocp(GameServerPort);

	SetTimer(ghWnd, WM_LOG_PAINT, 2000, NULL);
	SetTimer(ghWnd, WM_FIRST_MSG_PROCESS, 1000, NULL);

	SetTimer(ghWnd, WM_SET_DATE, 60000, NULL);
	SetTimer(ghWnd, WM_LOG_DATE_CHANGE, 60000, NULL);
	SetTimer(ghWnd, WM_CONNECT_DATASERVER, 10000, NULL);


	SetTimer(ghWnd, WM_SECOND_MSG_PROCESS, 1000, NULL);
	SetTimer(ghWnd, WM_GS_CLOSE, 1000, NULL);
	SetTimer(ghWnd, WM_GSCS_SYNC, ReadConfig.GSCSSyncTime, NULL);
	
	SetTimer(ghWnd, WM_MONSTER_AND_MSG_PROC, 500, NULL);


	SetTimer(ghWnd, WM_MOVE_MONSTER_PROC, 300, NULL);
	SetTimer(ghWnd, WM_EVENT_RUN_PROC, 100, NULL);

	SetTimer(ghWnd, WM_AI_MONSTER_MOVE_PROC, 100, NULL);
	SetTimer(ghWnd, WM_AI_MONSTER_PROC, 1011, NULL);

	SetTimer(ghWnd, WM_GUILD_LIST_REQUEST, 1000, NULL);

#if (WL_PROTECT==1)
	int MyCheckVar;  
	CHECK_PROTECTION(MyCheckVar, 0x52547978)  	 
	if (MyCheckVar != 0x52547978)
	{
		WLRegDisableCurrentKey(wlMarkStolenKey);
	}
#endif

#if (WL_PROTECT==1)
	VM_END
#endif

	return TRUE;
}

// Message handler for about box.
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	
#if (WL_PROTECT==1)
	VM_START_WITHLEVEL(15)
	int MyCheckVar;  
	CHECK_PROTECTION(MyCheckVar, 0x54547978)  	 
	if (MyCheckVar != 0x54547978)
	{
		WLRegDisableCurrentKey(wlMarkStolenKey);
		WLRegRemoveCurrentKey();
	}
	VM_END
#endif

	switch (message)
	{
		case WM_INITDIALOG:
				return TRUE;

		case WM_COMMAND:
			if ( ( wParam & 0xFFFF & 0xFFFF ) == IDOK || (wParam & 0xFFFF & 0xFFFF ) == IDCANCEL) 
			{
				EndDialog(hDlg, (wParam& 0xFFFF & 0xFFFF ));
				return TRUE;
			}
			break;
	}
    return FALSE;
}

bool GameServerCommandCase(INT cmd)
{	
	switch ( cmd )
	{
		case IDM_CLOSE_PLAYER:
			{
				Ccloseplayer closeplayer;
				closeplayer.Load("closeplayer.txt");
			}
			break;
		case IDM_ALL_USER_CLOSE:
			gObjAllDisconnect();
			break;
		case IDM_ALL_USER_LOGOUT:
			gObjAllLogOut();
			break;
		case IDM_MONSTER_POSITION_RELOAD:
			GameMonsterAllCloseAndReLoad();
			break;
		case IDM_SHOP_RELOAD:
			ShopDataLoad();
			break;
		case IDM_MAP_SERVER_INFO_RELOAD:
			g_MapServerManager.LoadData(ReadConfig.ConnDataFiles[9] );
			break;
#if (GS_CASTLE==1)					
		case IDM_CASTLE_SIEGE_RELOAD: //GS-CS Decompiled 100%
			if( g_CastleSiege.Ready(g_MapServerManager.GetMapSvrGroup()) == TRUE )
			{
				if( g_CastleSiege.LoadData(ReadConfig.ConnDataFiles[20]) )
				{
					g_CastleSiege.LoadPreFixData(ReadConfig.ConnDataFiles[0]);
					g_CastleSiege.SetDataLoadState(CASTLESIEGE_DATALOAD_2);
				}
			}
			break;
#endif
		case IDM_IP_LOAD:
			acceptIP.Load(gDirPath.GetNewPath("Iplist.dat"));
			break;
		case IDM_OPTION_RELOAD:
			ReadCommonServerInfo();
			break;
#if (CRYSTAL_EDITION == 1)
#if (PACK_EDITION>=3)
		case IDM_GAMEGUARD_INFO_RELOAD:
			CrystalWall.Read("..\\SCFData\\CrystalEdition\\SCF_CrystalWall.ini");
			break;
#endif
#endif
		case IDM_CASHSHOP_OPTION_RELOAD:
			g_CashShop.CashShopOptioNReload();
			break;
		case IDM_DRAGON_EVENT:
			if (  DragonEvent->GetState() != 0 )
				DragonEvent->End();
			else
				DragonEvent->Start();
			break;
		case IDM_ELDORADO_EVENT:
			if (  gEledoradoEvent.m_bMenualStart != 0 )
				gEledoradoEvent.End_Menual();
			else
				gEledoradoEvent.Start_Menual();
			break;
#if (PACK_EDITION>=2)
		case IDM_STARTOFXMAS_EVENT:
			if ( XMasEvent.gXMasEvent != 0 )
				XMasEvent.gXMasEvent = 0;
			else
				XMasEvent.gXMasEvent = 1;
			break;
#endif
		case IDM_FIRECRACKER:
			if ( gFireCrackerEvent != 0 )
				gFireCrackerEvent = 0;
			else
				gFireCrackerEvent = 1;
			break;
		case IDM_5_MINUTE_CLOSE:
			if ( gCloseMsg == 0 )
			{
				gCloseMsgTime = 300;
				gCloseMsg=1;
				AllSendServerMsg( lMsg.Get(MSGGET(4, 104)) );
			}
			break;
		case IDM_3_MINUTE_CLOSE:
			if ( gCloseMsg == 0 )
			{
				gCloseMsgTime = 180;
				gCloseMsg=2;
				AllSendServerMsg(lMsg.Get(MSGGET(4, 105)) );
			}
			break;
		case IDM_1_MINUTE_CLOSE:
			if ( gCloseMsg == 0 )
			{
				gCloseMsgTime = 60;
				gCloseMsg=3;
				AllSendServerMsg(lMsg.Get(MSGGET(4, 106)) );
			}
			break;
		case IDM_RELOAD_ALL_EVENT:
			ReadEventInfo(MU_EVENT_ALL );
			break;
		case IDM_RELOAD_DEVILSQUARE:
			ReadEventInfo(MU_EVENT_DEVILSQUARE );
			break;
		case IDM_RELOAD_BLOODCASTLE:
			ReadEventInfo(MU_EVENT_BLOODCASTLE );
			break;
		case IDM_RELOAD_UNDERWORLD_ATTACK:
			ReadEventInfo(MU_EVENT_ATTACKEVENTTROOP );
			break;
		case IDM_RELOAD_GOLDENTROOP_ATTACK:
			ReadEventInfo(MU_EVENT_GOLDENTROOP );
			break;
		case IDM_RELOAD_WHITEMAGE_ATTACK:
			ReadEventInfo(MU_EVENT_WHITEMAGETROOP );
			break;
		case IDM_RELOAD_LOVE_PANGPANG:
			ReadEventInfo(MU_EVENT_LOVEPANGPANG );
			break;
		case IDM_RELOAD_FIRECRACKER:
			ReadEventInfo(MU_EVENT_FIRECRACKER );
			break;
		case IDM_RELOAD_XMAS_STAR:
			ReadEventInfo(MU_EVENT_XMASSTAR );
			break;
		case IDM_RELOAD_HEART_OF_LOVE:
			ReadEventInfo(MU_EVENT_HEARTOFLOVE );
			break;
		case IDM_RELOAD_NPC_SAY_HAPPY_NEW_YEAR:
			ReadEventInfo(MU_EVENT_SAY_HAPPYNEWYEAR );
			break;
		case IDM_RELOAD_NPC_SAY_MERRY_XMAS:
			ReadEventInfo(MU_EVENT_SAY_MERRYXMAS );
			break;
		case IDM_RELOAD_CHAOSCASTLE:
			ReadEventInfo(MU_EVENT_CHAOSCASTLE );
			break;
		case IDM_RELOAD_CHRISTMAS_RIBBONBOX:
			ReadEventInfo(MU_EVENT_CHRISTMAS_RIBBONBOX );
			break;
		case IDM_RELOAD_VALENTINE_DAY_CHOCOLATE_BOX:
			ReadEventInfo(MU_EVENT_VALENTINESDAY_CHOCOLATEBOX );
			break;
		case IDM_RELOAD_WHITE_DAY_CANDY_BOX:
			ReadEventInfo(MU_EVENT_WHITEDAY_CANDYBOX  );
			break;
		case IDM_RELOAD_ALL_ETC_OPTION:
			ReadGameEtcInfo(MU_ETC_ALL );
			break;
		case IDM_RELOAD_CREATE_CHARACTER:
			ReadGameEtcInfo(MU_ETC_CREATECHARACTER );
			break;
		case IDM_RELOAD_GUILD:
			ReadGameEtcInfo(MU_ETC_GUILD );
			break;
		case IDM_RELOAD_TRADE:
			ReadGameEtcInfo(MU_ETC_TRADE );
			break;
		case IDM_RELOAD_CHAOSBOX:
			ReadGameEtcInfo(MU_ETC_USECHAOSBOX );
			break;
		case IDM_RELOAD_PERSONAL_SHOP:
			ReadGameEtcInfo(MU_ETC_PERSONALSHOP );
			break;
		case IDM_RELOAD_PK_ITEM_DROP:
			ReadGameEtcInfo(MU_ETC_PKITEMDROP );
			break;
		case IDM_RELOAD_ITEM_DROP_RATE:
			ReadGameEtcInfo(MU_ETC_ITEMDROPRATE );
			break;
		case IDM_RELOAD_SPEEDHACK:
			ReadGameEtcInfo(MU_ETC_SPEEDHACK );
			break;
		case IDM_RELOAD_GAMEGUARD_CHECKSUM_CHECK:
			ReadGameEtcInfo(MU_ETC_GAMEGUARD );
			break;
		default:
			return false;
			break;
	}
	return true;
}


LRESULT CALLBACK ParentMDI(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	//__try
	//{

	INT wmId, wmEvent;
	static HWND hMdiClient;

	switch (message)
	{
		case WM_CREATE:
		{
			hMdiClient=mdi.CreateMDIClient(hWnd,GetModuleHandle(0));
			mdi.OnCreate(hWnd,reinterpret_cast<CREATESTRUCT*>(lParam));
			statusParent.Create(mdi.hwndParent);
			return 0;
		}break;
#if (GS_OLDSTYLE == 0)
		case WM_SIZE:
		{
			DefFrameProc(hWnd,hMdiClient,message,wParam,lParam);
			statusParent.AutoSize(mdi.hwndParent);
			return 0;
		}break;
#endif
		case WM_DESTROY:
		{
			GameMainFree();
			GiocpDelete();
			DefFrameProc(hWnd,hMdiClient,WM_COMMAND,wParam,lParam);
			//PostQuitMessage(0);     //signal end of application
			ExitProcess(0);			//Force Close GS
			return 0;
		}break;
				
		case WM_COMMAND:
		{
			wmId    = wParam & 0xFFFF & 0xFFFF; 
			wmEvent = (wParam >> 16) & 0xFFFF; 
				
			if(GameServerCommandCase(wmId) == false)
			{
				// Parse the menu selections:
				switch (wmId)
				{
					case IDM_LOG_WINDOW:
					   mdi.CreateLogInst();
					   break;
					case IDM_LINKS_WINDOW:
					   childLog.Init("A_Links","Active Links",mdi.hInstance,WndProc2);
					   break;
					case IDM_EVENT_WINDOW:
					   childEventLog.Init("A_Events","Active Events",mdi.hInstance,WndProc3);
					   break;
#if(PROXY_SHOW == 1)
					case IDM_PROXY_WINDOW:
					   childProxy.Init("A_Proxy","Proxy Window",mdi.hInstance,WndProc4);
					   break;
#endif
					case IDM_ABOUT:
					   DialogBox(hInst, (LPCTSTR)IDD_ABOUT_BOX, hWnd, (DLGPROC)About);
					   break;
					case IDM_EXIT:
					   SendMessage(hWnd, WM_CLOSE, 0, 0);
					   break;
					case IDM_LOG_PAINT:
						gCurPaintType=0;
						break;
					case IDM_MAP1:
						gCurPaintMapNumber = 0;	// Lorencia
						gCurPaintType=1;
						break;
					case IDM_MAP2:
						gCurPaintType=1;
						gCurPaintMapNumber = 2;	// Dungeon
						break;
					case IDM_MAP3:
						gCurPaintType=1;
						gCurPaintMapNumber = 3;	// Devias
						break;
					case IDM_MAP4:
						gCurPaintType=1;
						gCurPaintMapNumber = 4;	// Noria
						break;
					case IDM_CRYWOLF:
						gCurPaintType=1;
						gCurPaintMapNumber = 34;	// Crywolf
						break;
					default:
						return DefFrameProc(hWnd,hMdiClient,message,wParam,lParam);
						//return DefWindowProc(hWnd, message, wParam, lParam);
						break;
				}
			}
		}break;
		case WM_GM_SERVER_MSG_PROC:
			GMServerMsgProc(wParam, lParam);
			break;
		case WM_GM_CLIENT_MSG_PROC:
			{
				//int wp = wParam;
				//int lp = lParam;
				GMClientMsgProc(wParam, lParam);
			}
			break;
		case WM_GM_JOIN_CLIENT_MSG_PROC:
			GMJoinClientMsgProc(wParam, lParam);
			break;
		case WM_GM_RANKING_CLIENT_MSG_PROC:
			{	
				if(ReadConfig.SCFRSON == FALSE)
				{
					GMRankingClientMsgProc(wParam, lParam);
				}
			}
			break;
		case WM_GM_EVENTCHIP_CLIENT_MSG_PROC:
			{
				if(ReadConfig.SCFESON == FALSE)
				{
					GMEventChipClientMsgProc(wParam, lParam);
				}
			}
			break;
		case WM_GM_DATA1_CLIENT_MSG_PROC:
			cDBSMng.MsgProc(0, wParam, lParam);
			break;
		case WM_GM_DATA2_CLIENT_MSG_PROC:
			cDBSMng.MsgProc(1, wParam, lParam);
			break;
		case WM_GM_EXDATA_CLIENT_MSG_PROC:
			ExDataClientMsgProc(wParam, lParam);
			break;
		case WM_GM_EXS_CLIENT_MSG_PROC:
			ExServerClientMsgProc(wParam, lParam);
			break;
		case WM_GM_IOGS_CLIENT_MSG_PROC:
			{
				if(ReadConfig.SCFIOGSON == 1)
					IOGSClientMsgProc(wParam, lParam);
			}break;
		case WM_GM_SCFPS_CLIENT_MSG_PROC:
			{
				if(ReadConfig.SCFPSON == 1)
					SCFPSClientMsgProc(wParam, lParam);
			}
			break;
		//case WM_GM_SCFPACK_CLIENT_MSG_PROC:
		//	SCFPackClientMsgProc(wParam, lParam);
		//	break;
		case WM_START_SERVER:
			if ( gServerReady == 2 )
				GameServerStart();
			break;
		case WM_TIMER:
			switch ( wParam )
			{
				case WM_GUILD_LIST_REQUEST:
					DGGuildListRequest();
					KillTimer(ghWnd, WM_GUILD_LIST_REQUEST);
					break;
				case WM_LOG_PAINT:
					{
						if(mdi.ChildLog != NULL)
						{
							RECT rect;
							HDC hdc = GetDC(mdi.ChildLog);
							GetClientRect(mdi.ChildLog, &rect);
							FillRect(hdc, &rect, (HBRUSH)GetStockObject(0));
							ReleaseDC(mdi.ChildLog, hdc);

							if ( gCurPaintType == 0)
							{
								if ( LogTextPaint != NULL )
								{
									LogTextPaint(mdi.ChildLog);
								}
							}		
							gObjViewportPaint(mdi.ChildLog, gCurPaintPlayer);					
						}else
						{
							int totalplayer=0;
							//JS Send Players Count
							for (int n=0;n<OBJMAX;n++)
							{
								if ( gObj[n].Type == OBJ_USER && gObj[n].Connected != PLAYER_EMPTY )
								{
									totalplayer++;
								}
							}
							gObjTotalUser = totalplayer;
						}
						g_ServerInfoDisplayer.Run(mdi.ChildLog);
						GJNotifyMaxUserCount();
#if (GS_OLDSTYLE == 0)
						//statusBar.AutoSize(hWnd);
#endif
					}
					break;
				case WM_MONSTER_AND_MSG_PROC:
					MonsterAndMsgProc();
					break;
				case WM_MOVE_MONSTER_PROC:
					MoveMonsterProc();
					#if (GS_CASTLE==0)
						g_Kanturu.UserMonsterCountCheck();
					#endif
					break;
				case WM_EVENT_RUN_PROC:
#if(GS_CASTLE_NOEVENTS == 0)
					g_BloodCastle.Run();
					g_RingAttackEvent.Run();
					g_ChaosCastle.Run();
					g_DuelManager.Run();
					g_DoppelGanger.Run();
#endif
					g_CsNPC_Weapon.WeaponAttackProc();
					#if (GS_CASTLE==1)
						g_CastleSiege.Run();
						g_CastleDeepEvent.Run();
						g_Crywolf.Run();
					#endif
					#if (GS_CASTLE==0)
						g_Kanturu.Run(); 
					#endif
#if(GS_CASTLE_NOEVENTS == 0)
					g_CashShop.CheckShopServerConnectState();
#endif
					break;
				case WM_AI_MONSTER_MOVE_PROC:
					TMonsterAI::MonsterMoveProc();
					break;
				case WM_AI_MONSTER_PROC:
					TMonsterAI::MonsterAIProc();
					break;

      case WM_FIRST_MSG_PROCESS:
            {
				for(int n = 0; n < OBJMAX; n++ )
				{
					gObjStateSetCreate(n);
					gObjViewportListDestroy(n);
					gObjViewportListCreate(n);
					gObjViewportListProtocol(n);
					if( n >= OBJ_MAXMONSTER )
					{
						if ( gDoPShopOpen )
						{
							PShop_ViewportListRegenarate(n);
							if( gObjIsConnected(n) != PLAYER_EMPTY )
								gObj[n].m_bPShopItemChange = false;
						}
						 gObjUnionUpdateProc(n);
					}
				}
				gObjSetState();
				GameServerInfoSend();

			}break;

				case WM_SECOND_MSG_PROCESS:
					{
						for (int n = 0;n<MAX_MAP_NUMBER;n++)
						{
							MapC[n].WeatherVariationProcess();
						}
						wsGServer.m_SendSec = wsGServer.m_SendSecTmp;
						wsGServer.m_RecvSec = wsGServer.m_RecvSecTmp;
						wsGServer.m_SendSecTmp = 0;
						wsGServer.m_RecvSecTmp = 0;
#if(GS_CASTLE_NOEVENTS == 0)
						DragonEvent->Run();
						AttackEvent->Run();
						gEledoradoEvent.Run();
#endif
						g_EventManager.Run();
					}
					break;
				case WM_GSCS_SYNC:
					{
						#if (GS_CASTLE==1)
						if( cDBSMng.m_ActiveServer > DS_UNINITIALIZED && g_CastleSiege.GetDataLoadState() == 2 )
						{
							g_CastleSiege.DataRequest();
						}
						g_CastleSiege.SendCastleStateSync();
						#endif
						g_CastleSiegeSync.AdjustTributeMoney();
					}break;
				case WM_GS_CLOSE:
					if ( gCloseMsg != 0 )
					{
						gCloseMsgTime--;

						if ( gCloseMsgTime <= 1 )
						{
							if ( gCloseMsgTime == 1 )
							{
								AllSendServerMsg(lMsg.Get(MSGGET(1, 193)));
								LogAddC(2, lMsg.Get(MSGGET(1, 193)));
							}
						}
						else
						{
							if ( (gCloseMsgTime % 10) == 0 )
							{
								char szTemp[256];

								wsprintf( szTemp, lMsg.Get(MSGGET(1, 194)), gCloseMsgTime);
								AllSendServerMsg(szTemp);
								LogAddC(2, szTemp);
							}
						}

						if ( gCloseMsgTime < 0 )
						{
							KillTimer(ghWnd, WM_GS_CLOSE);
							gObjAllLogOut();
							gCloseMsgTime = 0;
							gCloseMsg = 0;
						}
					}
					gObjSecondProc();
					break;
				case WM_SET_DATE:
					WhisperCash.TimeCheckCashDelete();
					#if (GS_CASTLE==1)
					g_Crywolf.CrywolfSecondAct();
					#endif
					break;
				case WM_CONNECT_DATASERVER:
					cDBSMng.Connect();
					g_DuelManager.gObjCheckAllUserDuelStop();
					break;
				case WM_LOG_DATE_CHANGE:
					#if (GS_CASTLE==1)
						if( LogDateChange() == TRUE )
						{
							g_iCastleItemMixLimit = 1;
						}
					#else
						LogDateChange();
						/*if (IsDbgPresentPrefixCheck())
						{
							if (MessageBox(ghWnd, "GameServer close?", "Close", MB_YESNO|MB_APPLMODAL) == IDYES)
								exit(1);
						}*/
					#endif
					break;
			}
			break;
		case WM_CLOSE:
			{
				if (MessageBox(ghWnd, "GameServer close?", "Close", MB_YESNO|MB_APPLMODAL) == IDYES)
				{
					DestroyWindow(hWnd);
				}
			}
			break;
		case WM_KEYDOWN:
			switch ( wParam )
			{
				case 'p':
					gCurPaintPlayer--;
					if ( gCurPaintPlayer<0)
						gCurPaintPlayer=0;
					break;
				case 'q':
					gCurPaintPlayer++;
					if ( gCurPaintPlayer > OBJMAX -1 )
						gCurPaintPlayer = OBJMAX -1;
					break;
				case 't':
					gServerMaxUser -= 10;
					if ( gServerMaxUser < 10 )
						gServerMaxUser = 10;
					break;

				case 'u':
					gServerMaxUser += 10;
					if ( gServerMaxUser > 1500 )
						gServerMaxUser = 1500;
					break;
				case 'v':
					{
						PMSG_CHARMAPJOIN szMsg;
						strcpy(szMsg.Name, "Å×½ºÆ®");		// Test
						CGPCharacterMapJoinRequest(&szMsg, 0);
					}
					break;
			}
		default:
			return DefFrameProc(hWnd,hMdiClient,message,wParam,lParam);
			//return DefWindowProc(hWnd, message, wParam, lParam);
   }
	//}__except( EXCEPTION_ACCESS_VIOLATION == GetExceptionCode() )
	//{
	//}

   #pragma message("[Warning Fix][Line 881] DaRKav - not sure about the return value here!")
   return 0;
}