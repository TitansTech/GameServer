#include "stdafx.h"
#include "GameServer.h"
#include "LogProc.h"
#include "GameMain.h"
#include "User.h"
#include "Resource.h"
#include "MDI.h"
#include "..\common\winutil.h"
#include "..\include\readscript.h"
#include "ChildWindows.h"
#include "protection.h"
#include "AntiRE.h"

LRESULT CALLBACK ParentMDI(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);// Brain of the GameServer
LRESULT CALLBACK WndProcLog(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
extern LARGE_INTEGER lTime1;
extern LARGE_INTEGER lFreq;


namespace {
  const int IDC_MDI_CLIENT=500;
}
#define IDM_FIRSTCHILD                  5500

extern void InitAll(LPSTR lpCmdLine);

MDI mdi;

BOOL MDI::Init(char * Class, char * Name, HINSTANCE hInstance,LPSTR lpCmdLine)
{
#if (MEM_PROTECT==1)
	#if (MEM_PROTECT_DEBUG==1)
		ReplaceBytes(removeFunctionsArray[12],replacementPart,jmpLength);		//Call to MDI::Init
	#else
		//ReplaceBytes(removeFunctionsArray[12],replacementPart,jmpLength);		//Call to MDI::Init
		ReplaceBytes(removeFunctionsArray[0] ,replacementPart,800);				//APIINIT
	#endif
#endif

	this->ChildLog = NULL;
	this->hInstance = hInstance;
	this->lpCmdLine = lpCmdLine;
	strcpy(ClassName,Class);
	strcpy(WindowName,Name);
	
	// Check if error ocurs
	this->MyRegisterClass(hInstance);
	InitCommonControls();
	if (!this->InitInstance(hInstance, SW_SHOWNORMAL))
	{
		return FALSE;
	} else 
	{
#if (MEM_PROTECT==1)
		#if (MEM_PROTECT_DEBUG==1)
			ReplaceBytes(removeFunctionsArray[14],replacementPart,jmpLength);						//Call to MDI::InitInstance
			DestroyFunction(removeFunctionsArray[14] + removeFunctionsArray[15] ,replacementPart);	//MDI::InitInstance
		#else
			DestroyFunction(removeFunctionsArray[14] ,replacementPart);								//MDI::InitInstance
		#endif
#endif
	}

	ShowWindow(this->hwnd, SW_SHOWMAXIMIZED);
	UpdateWindow(this->hwnd);

	//PrintDestroyFunction(removeFunctionsArray[2] + removeFunctionsArray[3],500);

	InitAll(this->lpCmdLine);
#if (MEM_PROTECT==1)
	#if (MEM_PROTECT_DEBUG==1)
		ReplaceBytes(removeFunctionsArray[2],replacementPart,jmpLength);						//Call to InitAll
		DestroyFunction(removeFunctionsArray[2] + removeFunctionsArray[3],replacementPart);		//Destroy InitAll
	#else
		//ReplaceBytes(removeFunctionsArray[2],replacementPart,removeFunctionsArray[3]-removeFunctionsArray[2]-jmpLength);
		DestroyFunction(removeFunctionsArray[2],replacementPart);								//Destroy InitAll
	#endif
#endif
	
	//SetTimer(this->hwnd, WM_FUNCTIMER, 2000, NULL);
	HACCEL hAccel=LoadAccelerators(hInstance,(LPCSTR)IDA_GAMESERVER);// Load Accelerators;
	HWND hMdiClient=GetDlgItem(this->hwnd,IDC_MDI_CLIENT);
	MSG msg;
	
	LARGE_INTEGER lTime2;
	long diffTicks;
	float diffSecs;
	QueryPerformanceCounter(&lTime2);
	diffTicks=lTime2.QuadPart-lTime1.QuadPart;	//ticks passed
	diffSecs=(diffTicks/lFreq.QuadPart);		//secs passed.
	
	char szTemp2[256];
	wsprintf(szTemp2,"[%s] %s - %s",VersionName,Creators,EMails);
	statusParent.AddText(szTemp2);
	wsprintf(szTemp2,"[%s] %s",GAMESERVER_VERSION,ProjectName);				
	statusParent.AddText(szTemp2);
	wsprintf(szTemp2,"GameServer Loaded in %d seconds",(int)diffSecs);
	statusParent.AddText(szTemp2);

#if (MEM_PROTECT==1)
	//Finish it
	//ErasePEHeaderFromMemory();

	#if (MEM_PROTECT_DEBUG==1)
		ReplaceBytes(removeFunctionsArray[0] + removeFunctionsArray[1],replacementPart,functionMangleApiMain);	//APIINIT
		ReplaceBytes(removeFunctionsArray[12]+ removeFunctionsArray[3],replacementPart,500);					//MDI::Init
		DestroyFunction(removeFunctionsArray[12] + removeFunctionsArray[13],replacementPart);					//MDI::Init
	#else
		//ReplaceBytes(removeFunctionsArray[0] ,replacementPart,removeFunctionsArray[1]-removeFunctionsArray[0]-functionMangleApiMain);	//APIINIT
		ReplaceBytes(removeFunctionsArray[12],replacementPart,250);												//MDI::Init
	#endif
#endif

	while (GetMessage(&msg,0,0,0)>0)
	{
		if (!TranslateMDISysAccel(hMdiClient,&msg)&&		//mdi specific keyboard msgs
			!TranslateAccelerator(this->hwnd,hAccel,&msg))	//all other keyboard msgs
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return static_cast<int>(msg.wParam);

	return TRUE;
}

ATOM MDI::MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)ParentMDI;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_NORMAL_V1);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= (LPCSTR)IDR_MENU;//NULL;
	wcex.lpszClassName	= this->ClassName;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_ICON_MINIMIZED);

	return RegisterClassEx(&wcex);
}

BOOL MDI::InitInstance(HINSTANCE hInstance, int nCmdShow)
{	
	int desktopwidth=GetSystemMetrics(SM_CXSCREEN);
	int desktopheight=GetSystemMetrics(SM_CYSCREEN);
	HWND hWnd;
	hInst=hInstance;

    hWnd = CreateWindowEx(0,                    //extended styles
                         this->ClassName,		//name: wnd 'class'
                         this->WindowName,		//wnd title 
                         WS_OVERLAPPEDWINDOW,   //wnd style
                         desktopwidth/4,        //position:left
                         desktopheight/4,       //position: top
                         desktopwidth/2,        //width
                         desktopheight/2,       //height
                         0,                     //parent wnd handle
                         0,                     //menu handle/wnd id
                         hInstance,             //app instance
                         0);                    //user defined info
	
	if (hWnd==0) 
	{
		return FALSE;
	}

	this->hwnd=hWnd;
	ghWnd = hWnd;
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

HWND MDI::CreateMDIClient(const HWND hParent,const HINSTANCE hInst)
{
	CLIENTCREATESTRUCT ccs; 
	ccs.hWindowMenu=GetSubMenu(GetMenu(hParent),4);
	ccs.idFirstChild=IDM_FIRSTCHILD; //id of the first mdi child window

	return CreateWindowEx(	0,
							_T("mdiclient"),
							0,
							WS_VISIBLE|WS_CHILD|WS_CLIPCHILDREN,
							0,0,0,0, 
							hParent,
							reinterpret_cast<HMENU>(static_cast<INT_PTR>
							(IDC_MDI_CLIENT)), 
							hInst, 
							reinterpret_cast<void*>(&ccs)); 
}

void MDI::OnCreate(HWND hwnd,CREATESTRUCT *cs)
{
	//char MDI1Class[] = "MDI_CHILD_01";
	HWND hMdiClient= GetDlgItem(hwnd,IDC_MDI_CLIENT);
	this->hwndParent = hMdiClient;
	//this->CreateLogInst();
}

void MDI::CreateLogInst()
{
	if(this->ChildLog == NULL)
	{
		char LogClass[] = "LOGCLASS";
		this->RegisterMdiChild(WndProcLog,LogClass,this->hInstance);
		RECT rc={0,0,800,600};
		this->ChildLog = CreateMDIChildWnd(this->hwndParent,this->hInstance,WS_CLIPCHILDREN,rc,
						  IDM_FIRSTCHILD,"Logs",LogClass);
	}
}

bool MDI::RegisterMdiChild(WNDPROC wp,char * s,const HINSTANCE hInst)
{
	HCURSOR    hCursor;
	HICON      hIcon;
	WNDCLASSEX wcx={0};

	hIcon				 = (HICON)LoadImage(0,IDI_APPLICATION,IMAGE_ICON,0,0,LR_SHARED);
	hCursor				 = (HCURSOR)LoadImage(0,IDC_ARROW,IMAGE_CURSOR,0,0,LR_SHARED);
	wcx.cbSize           = sizeof(WNDCLASSEX);          
	wcx.style            = CS_HREDRAW|CS_VREDRAW;       
	wcx.lpfnWndProc      = wp;                                                  
	wcx.cbWndExtra       = sizeof(HWND);                
	wcx.hInstance        = hInst;                      
	wcx.hIcon            = LoadIcon(hInstance, (LPCTSTR)IDI_NORMAL_V1);                    
	wcx.hCursor          = hCursor;                                        
	wcx.lpszClassName    = s;     
	//wcx.lpszMenuName	 = (LPCSTR)IDR_MENU;

	//Register the wnd class with the Windows system
	if (!RegisterClassEx(&wcx))
	{
		return false;
	}
	return true;
}

HWND MDI::CreateMDIChildWnd(const HWND hParent,const HINSTANCE hInst,DWORD dwStyle,
                       const RECT& rc,const int id,char * caption, 
                       char * classname)
{
	dwStyle|=WS_CHILD|WS_VISIBLE;
	return CreateWindowEx(WS_EX_MDICHILD,               //extended styles
                      classname,            //control 'class' name
                      caption,              //control caption
                      dwStyle,                      //control style 
                      rc.left,                      //position: left
                      rc.top,                       //position: top
                      rc.right,                     //width
                      rc.bottom,                    //height
                      hParent,                      //parent window handle
                      //control's ID
                      reinterpret_cast<HMENU>(static_cast<INT_PTR>(id)),
                      hInst,                        //application instance
                      0);                           //user defined info
}

LRESULT CALLBACK WndProcLog(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	INT wmId, wmEvent;

	switch (message) 
	{
	  case WM_CREATE:
		{
			statusBar.Create(hWnd);
			statusBar.AddText("");
			statusBar.AddText("");
			char szTemp2[256];
			wsprintf(szTemp2,"[%s] %s - %s",VersionName,Creators,EMails);
			statusBar.AddText(szTemp2);
			wsprintf(szTemp2,"[%s] %s",GAMESERVER_VERSION,ProjectName);				
			statusBar.AddText(szTemp2);
			return 0;
		}break;
		case WM_SIZE:
		{
			statusBar.AutoSize(hWnd);
			return DefMDIChildProc(hWnd,message,wParam,lParam);
		}break;
		//case WM_SIZE:
		//{
		//	if (wParam==SIZE_MAXIMIZED)
		//	{
		//		SetWindowLongPtr(hWnd,GWL_EXSTYLE,0);
		//	}
		//	else
		//	{
		//		SetWindowLongPtr(hWnd,GWL_EXSTYLE,WS_EX_CLIENTEDGE);
		//	}
		//	MoveWindow(hWnd,0,0,LOWORD(lParam),HIWORD(lParam),1);

		//}break;
		case WM_DESTROY:
		{
			mdi.ChildLog = NULL;
			statusBar.hStatus = NULL;
		}break;

		default:
		 return DefMDIChildProc(hWnd,message,wParam,lParam);
	}
	return 0;
}