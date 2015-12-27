#include "stdafx.h"

class cChildWindow
{
public:
	BOOL Init(char * Class, char * Name, HINSTANCE hInstance,WNDPROC WndProcEx);
	HWND hwnd;

private:
	char ClassName[64];
	char WindowName[64];

	BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);
	ATOM MyRegisterClass(HINSTANCE hInstance, WNDPROC WndProcEx);
};

LRESULT CALLBACK WndProc2(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProc3(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

#if(PROXY_SHOW == 1)
LRESULT CALLBACK WndProc4(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void WriteProxyPacket(int aIndex, unsigned char * buf, int Size, BYTE Type);

extern int Proxy_aIndex;
extern BYTE Proxy_ShowRecv;
extern BYTE Proxy_ShowSend;
extern cChildWindow childProxy;

#endif
extern cChildWindow childLog;
extern cChildWindow childEventLog;


#define IDC_MAIN_STATUS                     10622
#define IDC_MAIN_TOOL                       10633
#define IDC_MAIN_EDIT                       10655

class cStatusBar
{
public:
	void Create(HWND parent);
	int AddText(char * msg);
	void ModText(int index,char * msg);
	void AutoSize(HWND parent);
	HWND hStatus;
	int MsgCount;
	int Msg[10];
};

extern cStatusBar statusBar;
extern cStatusBar statusParent;