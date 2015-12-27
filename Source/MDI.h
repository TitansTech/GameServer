
class MDI
{
public:
	BOOL Init(char * Class, char * Name, HINSTANCE hInstance,LPSTR lpCmdLine);
	ATOM MyRegisterClass(HINSTANCE hInstance);
	BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);
	HWND CreateMDIClient(const HWND hParent,const HINSTANCE hInst);
	void OnCreate(HWND hwnd,CREATESTRUCT *cs);
	bool RegisterMdiChild(WNDPROC wp,char * s,const HINSTANCE hInst);
	HWND CreateMDIChildWnd(const HWND hParent,const HINSTANCE hInst,DWORD dwStyle,
                       const RECT& rc,const int id,char * caption, 
                       char * classname);
	void CreateLogInst();
	
	HINSTANCE hInstance;
	HWND hwnd;
	HWND hwndParent;
	HWND ChildLog;
	char ClassName[64];
	char WindowName[64];
	LPSTR lpCmdLine;
};

extern MDI mdi;