// ------------------------------
// Decompiled by Deathway
// Date : 2007-03-09
// ------------------------------
// One of My old successful Decompilation
// GS-N 0.99.60T : Status : Completed
//	GS-N	1.00.18	JPN	0x004A37F0	-	Completed

#include "stdafx.h"
#include "logproc.h"
#include "logtofile.h"
#include "GameMain.h"
#include "GameServer.h"



int m_cline;	// Current Line of Log
static FILE* logfp;	// FILE Variable for LOG
static char m_szLogFileName[100];	// Log Name

char LogText[LOG_TEXT_LINE][LOG_TEXT_LENGTH];
short LogTextLength[LOG_TEXT_LINE];
BYTE LogTextViewType[LOG_TEXT_LINE];

// Functions Pointers to Log Variables
void (*LogAdd)(char* szLog,...);
void (*LogAddC)(BYTE, char*, ...);
void (*LogAddCTD)(BYTE, char*, ...);
void (*LogAddTD)(char* szLog, ...);
void (*LogAddHeadHex)(char*, char*, char*, int);
void (*LogAddL)(char* szLog, ...);
void (*LogTextPaint)(HWND);

CRITICAL_SECTION LogCritical;	// Critical Section For LOG

// Log Set Internals
int LogMDay;
int LogMonth;
int LogMYear;


//std::vector<char *>	vec3_origin;

int gLogOutType = 1; // THIS IS NOT THE PLACE OF TTHIS VARIABLE


CLogToFile DEBUG_LOG("DEBUG_LOG", ".\\DEBUG_LOG", 1);

void DebugLog2(LPSTR szLog, ...)
{
#if (DSGN_COMPILE == 1)
	char szBuffer[512] = "";
	char dbgBuffer[512] = "";
	va_list pArguments;

	va_start(pArguments, szLog);
	vsprintf(szBuffer, szLog, pArguments);
	va_end(pArguments);

	//wsprintf(dbgBuffer, "%s \n", szBuffer);
	//OutputDebugString(dbgBuffer);

	DEBUG_LOG.Output("[Debug] %s", szBuffer);
#endif
}

void DebugLog(LPSTR szLog, ...)
{
#if (ShowWinDbgDebugInfo==1)
	char szBuffer[512] = "";
	char dbgBuffer[512] = "";
	va_list pArguments;

	va_start(pArguments, szLog);
	vsprintf(szBuffer, szLog, pArguments);
	va_end(pArguments);

	wsprintf(dbgBuffer, "%s \n", szBuffer);
	OutputDebugString(dbgBuffer);

	DEBUG_LOG.Output("[Debug] %s", szBuffer);
#endif
		//char szBuffer[1024]="";
		//va_list pArguments;
		//tm * today;
		//time_t ltime;
		//char tmpbuf[1024];
		//
		//
		//time(&ltime);
		//today=localtime(&ltime);
		//wsprintf(tmpbuf, "%.8s ", asctime(today)+11 );

		//va_start( pArguments,szLog );
		//vsprintf(szBuffer, szLog, pArguments);
		//va_end(pArguments);

		//strcat(tmpbuf, szBuffer);
		//OutputDebugString(tmpbuf);
}

void LogInit(int logprint)
{
	__try
	{
		int n;
		InitializeCriticalSection(&LogCritical);

		// Create yes no Log
		if (logprint!=0)		
		{

			LogAdd=LogAddFunc;
			LogAddC=LogAddFuncColor;
			LogAddCTD=LogAddTimeColorDateFunc;
			LogAddTD=LogAddTimeDateFunc;
			LogAddHeadHex=LogAddStrHexFunc;
			LogTextPaint=LogTextPaintProc;
			LogAddL=LogAddLocalFunc;

			for (n=0;n<LOG_TEXT_LINE;n++)
			{
				memset(&LogText[n],0, sizeof(LogText[0]));
				LogTextLength[n]=0;
				LogTextViewType[n]=0;
			}
			if (gLogOutType==0)
			{
				return;
			}
			else
			{
				LogDataSet();
			}
		}
		else
		{

			LogAdd=LogAddFuncVoid;
			LogAddC=LogAddFuncColorVoid;
			LogAddCTD=LogAddTimeColorDateFuncVoid;
			LogAddHeadHex=LogAddHeadHexFuncVoid;
			LogTextPaint=LogTextPaintProcVoid;
			LogAddTD=LogAddTimeDateVoid;
			LogAddL=LogAddLocalFuncVoid;
			
		}
	}__except( EXCEPTION_ACCESS_VIOLATION == GetExceptionCode() )
	{
	}
}


void LogDataSet()
{
	__try
	{
		char szTemp[250];
		
		struct tm *today;
		time_t ltime;	
		

		time(&ltime);
		today=localtime(&ltime);
		
		
		today->tm_year=today->tm_year + 1900;
		LogMYear=today->tm_year ;
		LogMonth=today->tm_mon+1;
		LogMDay=today->tm_mday;

		wsprintf(szTemp,"log\\%02d%02d%02d_%s.log",LogMYear, LogMonth, LogMDay,&szServerName);

		if (logfp != 0)
		{
			fclose(logfp);
			logfp=0;
		}

		logfp=fopen(szTemp,"a+t");
		if (logfp==0)
		{
			LogAdd("Log file create error");
		}
		strcpy(m_szLogFileName,szTemp);
	}__except( EXCEPTION_ACCESS_VIOLATION == GetExceptionCode() )
	{
	}
}




int LogDateChange()
{
	bool EntCrit = false;
	__try
	{
		char szTemp[250];

		tm* today;
		time_t ltime;
		time(&ltime);
		

		today=localtime(&ltime);
		today->tm_year =today->tm_year+1900;
		today->tm_mon = today->tm_mon +1;

		if (today->tm_year <= LogMYear)
		{
			if (today->tm_mon <= LogMonth)
			{
				if (today->tm_mday <= LogMDay)
				{
					return 0;
				}
			}
		}

		ReadConfig.ExpDay = ReadConfig.ExpDayofWeek[today->tm_wday];
		LogMYear = today->tm_year ;
		LogMonth = today->tm_mon ;
		LogMDay = today->tm_mday ;

#if (WL_PROTECT==1)
	int MyCheckVar;   
	VM_START_WITHLEVEL(5)
	CHECK_PROTECTION(MyCheckVar, 0x12746903)  	 
	if (MyCheckVar != 0x12746903)
	{
		int dow = today->tm_wday;
		int tr = rand()%6;
		if(dow == tr)
			exit(1);
	}
	VM_END
#endif

		wsprintf(szTemp, "log\\%02d%02d%02d_%s.log", LogMYear, LogMonth, LogMDay, &szServerName);
#if(GS_CASTLE_NOLOGSAVE == 0)
		EnterCriticalSection(&LogCritical);
		EntCrit = true;
		
		if (logfp!=0)
		{
			fclose(logfp);
			logfp=0;
		}
		logfp=fopen(szTemp, "a+t");
		strcpy(m_szLogFileName, szTemp);
		LeaveCriticalSection(&LogCritical);
		EntCrit = false;
#endif
		return 1;
	}__except( EXCEPTION_ACCESS_VIOLATION == GetExceptionCode() )
	{
		if(EntCrit == true)
			LeaveCriticalSection(&LogCritical);
		
		return 0;
	}
}


#if (LOG_INMEDIATLY==1)
#include "TServerInfoDisplayer.h"
extern TServerInfoDisplayer g_ServerInfoDisplayer; 
void GJNotifyMaxUserCount();
#endif


void LogTextAdd(BYTE type, char* msg, int len)
{
	__try
	{
		if( len > LOG_TEXT_LENGTH-1 ) len = LOG_TEXT_LENGTH-1;

		m_cline++;
		if( m_cline > LOG_TEXT_LINE-1) m_cline = 0;

		LogText[m_cline][0]			= '\0';
		
		memcpy(LogText[m_cline], msg, len);

		LogText[m_cline][len]		= LogText[m_cline][len+1] = '\0';
		LogTextLength[m_cline  ]	= len;
		LogTextViewType[m_cline]	= type;	
	}__except( EXCEPTION_ACCESS_VIOLATION == GetExceptionCode() )
	{
	}
}
void LogAddFuncColorVoid(BYTE Color, char* szLog, ...)
{
	// Nothing to Add
}
void LogAddTimeColorDateFuncVoid(BYTE Color, char* szLog, ...)
{
	// Nothing to Add
}



void LogAddFuncVoid(char* szLog, ...)
{
	// Nothing to Add
}


void LogAddTimeDateVoid(char* szLog, ...)
{
	// Log Disabled
}



void LogAddFunc(char* szLog, ...)
{
	bool EntCrit = false;
	__try
	{
		char szBuffer[512]="";
		
		va_list pArguments;
		
		va_start( pArguments,  szLog);	// review
		vsprintf(szBuffer, szLog, pArguments);
		va_end(pArguments);
		LogTextAdd(0,szBuffer, strlen(szBuffer));
		if (gLogOutType==0)
		{
			return;
		}
		else
		{
			EnterCriticalSection(&LogCritical);
			EntCrit = true;
			fprintf(logfp, "%s\n", szBuffer);
			LeaveCriticalSection(&LogCritical);
			EntCrit = false;
		}
	}__except( EXCEPTION_ACCESS_VIOLATION == GetExceptionCode() )
	{
		if(EntCrit == true)
			LeaveCriticalSection(&LogCritical);
	}
}

void LogAddTimeColorDateFunc(BYTE Color, char* szLog, ...)
{
	bool EntCrit = false;
	__try
	{
		char szBuffer[1024]="";
		va_list pArguments;
		tm * today;
		time_t ltime;
		char tmpbuf[512];
		
		
		time(&ltime);
		today=localtime(&ltime);
		wsprintf(tmpbuf, "%.8s ", asctime(today)+11 );

		va_start( pArguments,szLog );
		vsprintf(szBuffer, szLog, pArguments);
		va_end(pArguments);

		strcat(tmpbuf, szBuffer);

		LogTextAdd(Color, tmpbuf, strlen(tmpbuf));

		if (gLogOutType==0)
		{
			return;
		}
		else
		{
			if (logfp==0)
			{
				LogAdd("error-L2 : file create error %s %d", __FILE__, __LINE__);
			}
			else
			{
				EnterCriticalSection(&LogCritical);
				EntCrit = true;
				fprintf(logfp, "%s\n", &tmpbuf);
				LeaveCriticalSection(&LogCritical);
				EntCrit = false;
			}
		}
	}__except( EXCEPTION_ACCESS_VIOLATION == GetExceptionCode() )
	{
		if(EntCrit == true)
			LeaveCriticalSection(&LogCritical);
	}
}


void LogAddFuncColor(BYTE Color, char* szLog, ...)
{
	bool EntCrit = false;
	__try
	{
		char szBuffer[1024]="";
		va_list pArguments;
		
		va_start(pArguments, szLog);
		vsprintf(szBuffer, szLog, pArguments);
		va_end(pArguments);
		LogTextAdd(Color, szBuffer, strlen(szBuffer));

		if (!gLogOutType)
		{
			return;
		}
		else
		{
			EnterCriticalSection(&LogCritical);
			EntCrit = true;
			fprintf(logfp, "%s\n", szBuffer);
			LeaveCriticalSection(&LogCritical);
			EntCrit = false;
		}
	}__except( EXCEPTION_ACCESS_VIOLATION == GetExceptionCode() )
	{
		if(EntCrit == true)
			LeaveCriticalSection(&LogCritical);
	}
}

#pragma warning ( disable : 4101 )
void LogAddLocalFunc(char* szLog, ...)
{
	__try
	{
		char szBuffer[512];
		va_list pArguments;

	#if (LOCAL_LOG==1)	
		va_start(pArguments, szLog);
		vsprintf(szBuffer, szLog, pArguments);
		va_end(pArguments);

		LogTextAdd(5, szBuffer, strlen(szBuffer));


		if (!gLogOutType)
		{
			return;
		}
		else
		{
			EnterCriticalSection(&LogCritical);
			fprintf(logfp, "[LOCAL] %s\n", szBuffer);
			LeaveCriticalSection(&LogCritical);
		}

	#endif
	}__except( EXCEPTION_ACCESS_VIOLATION == GetExceptionCode() )
	{
	}
}
#pragma warning ( default : 4101 )

void LogAddLocalFuncVoid(char* szLog, ...)
{

}


void LogAddTimeDateFunc(char* szLog, ...)
{
	bool EntCrit = false;
	__try
	{
		char szBuffer[1024]="";
		va_list pArguments;
		tm * today;
		time_t ltime;
		char tmpbuf[512];
		
		
		time(&ltime);
		today=localtime(&ltime);
		wsprintf(tmpbuf, "%.8s ", asctime(today)+11 );

		va_start( pArguments,szLog );
		vsprintf(szBuffer, szLog, pArguments);
		va_end(pArguments);

		strcat(tmpbuf, szBuffer);

		LogTextAdd(0, tmpbuf, strlen(tmpbuf));

		if (gLogOutType==0)
		{
			return;
		}
		else
		{
			if (logfp==0)
			{
				LogAdd("error-L2 : file create error %s %d", __FILE__, __LINE__);
			}
			else
			{
				EnterCriticalSection(&LogCritical);
				EntCrit = true;
				fprintf(logfp, "%s\n", &tmpbuf);
				LeaveCriticalSection(&LogCritical);
				EntCrit = false;
			}
		}
	}__except( EXCEPTION_ACCESS_VIOLATION == GetExceptionCode() )
	{
		if(EntCrit == true)
			LeaveCriticalSection(&LogCritical);
	}
}


void LogAddHeadHexFuncVoid(char* sT,char* str,char* data,int len)
{
	// Log Disabled
}


void LogAddHeadHexFunc(char* sT,int Type,char* data,int len)
{
	bool EntCrit = false;
	__try
	{
		signed int n;
		
		if ( gLogOutType==0)
		{
			return;
		}
		else
		{
			if (logfp==0)
			{
				return;
			}
			else
			{
				EnterCriticalSection(&LogCritical);
				EntCrit = true;

				switch(sT[0])
				{
					case 'S':
						fprintf(logfp, "SEND %d[%d] : (", Type ,len);
					break;
					case 'R':
						fprintf(logfp, "RECV %d[%d] : (", Type ,len);
					break;
					default:
						fprintf(logfp, "UNK %d[%d] : (", Type ,len);
					break;
				}
				
				for (n=0; n<len; n++) 
				{
					fprintf(logfp, "0x%02x ", *(BYTE *)(data+n));
				}

				fprintf(logfp, ")\n");

				LeaveCriticalSection(&LogCritical);
				EntCrit = false;
			}
		}
	}__except( EXCEPTION_ACCESS_VIOLATION == GetExceptionCode() )
	{
		if(EntCrit == true)
			LeaveCriticalSection(&LogCritical);
	}
}


void LogAddStrHexFunc(char* sT,char* str,char* data, int len)
{
	bool EntCrit = false;
	__try
	{
		tm* today;
		time_t ltime;
		signed int n;

		time(&ltime);
		today=localtime(&ltime);

		if (gLogOutType==0)
		{
			return;
		}
		else
		{
			if (logfp==0)
			{
				return;
			}
			else
			{
				EnterCriticalSection(&LogCritical);
				EntCrit = true;
				fprintf(logfp, "%.08s", asctime(today)+11);
				fprintf(logfp, "[%s][%s][%d] : (", sT,str,len);

				for (n=0; n<len; n++ )
				{
					fprintf(logfp, "0x%02x ", *(BYTE *)(data+n));
				}
				fprintf(logfp, ")\n");

				LeaveCriticalSection(&LogCritical);
				EntCrit = false;
			}
		}
	}__except( EXCEPTION_ACCESS_VIOLATION == GetExceptionCode() )
	{
		if(EntCrit == true)
			LeaveCriticalSection(&LogCritical);
	}
}

void LogTextClear()
{
	__try
	{
		int n;
		for ( n=0; n<LOG_TEXT_LINE; n++)
		{
			memset(&LogText[n], 0, LOG_TEXT_LENGTH);
			LogTextLength[n]=0;	// WORD
			LogTextViewType[n]=0;	// BYTE
		}
	}__except( EXCEPTION_ACCESS_VIOLATION == GetExceptionCode() )
	{
	}
}


void LogTextPaintProcVoid(HWND hWnd)
{
	// Log Disabled
}


void LogTextPaintProc(HWND hWnd)
{
	__try
	{
		HDC hdc;
		int total;
		int n;
		
		
		hdc=GetDC(hWnd);
		total= LOG_TEXT_LINE;

		n=m_cline;

		while(total-- != 0)
		{
			switch (LogTextViewType[n] )
			{
			case 2:
				SetTextColor(hdc, RGB(255, 0, 0) );	
				break;
			case 3:
				SetTextColor(hdc, RGB(0, 100, 0));
				break;
			case 4:
				SetTextColor(hdc, RGB(0, 0, 255));
				break;
			case 5:
				SetTextColor(hdc, RGB(155, 0, 0));
				break;
			case 6:
				SetTextColor(hdc, RGB(0, 0, 100));
				break;
			case 7:
				SetTextColor(hdc, RGB(210, 30, 150));
				break;
			case 8:
				SetTextColor(hdc, RGB(244, 164, 96));
				break;
			case 9:
				SetTextColor(hdc, RGB(255, 165, 0));
				break;
			case 10:
				SetTextColor(hdc, RGB(255, 140, 0));
				break;
			case 11:
				SetTextColor(hdc, RGB(186,85,211));
				break;
			case 12:
				SetTextColor(hdc, RGB(153,50,204));
				break;
			case 13:
				SetTextColor(hdc, RGB(148,0,211));
				break;
			default:
				SetTextColor(hdc, RGB(0, 0, 0));
				break;
			}
			if (strlen(LogText[n])>1)
			{
#if (GS_OLDSTYLE == 1)

#if (WL_PROTECT==1)
				TextOut( hdc, 0, total*15 + 130, LogText[n], strlen(LogText[n])); 
#else
				TextOut( hdc, 0, total*15 + 100, LogText[n], strlen(LogText[n])); 
#endif

#else

#if (WL_PROTECT==1)
				TextOut( hdc, 0, total*15 + 30, LogText[n], strlen(LogText[n])); 
#else
				TextOut( hdc, 0, total*15, LogText[n], strlen(LogText[n])); 
#endif

#endif
			}
			n--;
			if (n<0)
			{
				n=LOG_TEXT_LINE-1;
			}
		}
		ReleaseDC(hWnd, hdc);
	}__except( EXCEPTION_ACCESS_VIOLATION == GetExceptionCode() )
	{
	}
}

int LogGetFileName()
{
	return 0;
}

void LogClose()
{
	__try
	{
		if (logfp!=0)
		{
			fclose(logfp);
		}
		DeleteCriticalSection(&LogCritical);
	}__except( EXCEPTION_ACCESS_VIOLATION == GetExceptionCode() )
	{
	}
}



void MsgBox(char *szlog, ...)
{
	__try
	{
		char szBuffer[512]="";
		va_list pArguments;
		va_start(pArguments, szlog);
		vsprintf(szBuffer, szlog, pArguments);
		va_end(pArguments);
		MessageBox(NULL, szBuffer, "error", MB_OK|MB_APPLMODAL);
	}__except( EXCEPTION_ACCESS_VIOLATION == GetExceptionCode() )
	{
	}
}

