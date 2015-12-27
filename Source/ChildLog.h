#define CHILD_LOG_TEXT_LENGTH	250
#define CHILD_LOG_TEXT_LINE	25

class CHILDLOG
{
public:
	void LogInit(int logprint, char * logName);
	void LogDataSet();
	int LogDateChange();
	void LogTextAdd(BYTE type, char* msg, int len);
	void LogAdd(char* szLog, ...);
	void LogAddC(BYTE Color, char* szLog, ...);
	void LogAddL(char * szLog, ...);
	void LogAddTD(char* szLog, ...);
	void LogAddHeadHexFunc(int Type,char* data,int len);
	void LogAddHeadHex(char* str,unsigned char* data, int len);
	void LogTextClear();
	void LogTextPaint(HWND hWnd);
	int LogGetFileName();
	void LogClose();

	//Vars
private:
	char LogName[50];
	int gLogOutType;
	CRITICAL_SECTION LogCritical;	// Critical Section For LOG

	int m_cline;	// Current Line of Log
	FILE* logfp;	// FILE Variable for LOG
	char m_szLogFileName[100];	// Log Name

	char LogText[CHILD_LOG_TEXT_LINE][CHILD_LOG_TEXT_LENGTH];
	short LogTextLength[CHILD_LOG_TEXT_LINE];
	BYTE LogTextViewType[CHILD_LOG_TEXT_LINE];

	int LogMDay;
	int LogMonth;
	int LogMYear;
};