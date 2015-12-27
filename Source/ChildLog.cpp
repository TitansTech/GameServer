#include "stdafx.h"
#include "ChildLog.h"


void CHILDLOG::LogInit(int logprint, char * logName)
{
	strcpy(this->LogName,logName);
	CreateDirectory(logName, NULL);

	gLogOutType = logprint;
	int n;
	InitializeCriticalSection(&LogCritical);

	for (n=0;n<CHILD_LOG_TEXT_LINE;n++)
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


void CHILDLOG::LogDataSet()
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

	wsprintf(szTemp,"%s\\%02d%02d%02d_%s.log",this->LogName,LogMYear, LogMonth, LogMDay,this->LogName);

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
}




int CHILDLOG::LogDateChange()
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


	LogMYear = today->tm_year ;
	LogMonth = today->tm_mon ;
	LogMDay = today->tm_mday ;

	wsprintf(szTemp, "%s\\%02d%02d%02d_%s.log", this->LogName,LogMYear, LogMonth, LogMDay, this->LogName);

	EnterCriticalSection(&LogCritical);
	
	if (logfp!=0)
	{
		fclose(logfp);
		logfp=0;
	}
	logfp=fopen(szTemp, "a+t");
	strcpy(m_szLogFileName, szTemp);
	LeaveCriticalSection(&LogCritical);
	return 1;
}

void CHILDLOG::LogTextAdd(BYTE type, char* msg, int len)
{
	
	if (len>CHILD_LOG_TEXT_LENGTH-1)
	{
		len=CHILD_LOG_TEXT_LENGTH-1;
	}
	m_cline++;
	if (m_cline>CHILD_LOG_TEXT_LINE-1)
	{
		m_cline=0;
	}
	LogText[m_cline][0]=0;
	memcpy(&LogText[m_cline], msg, len);
	LogText[m_cline][1+len]=0;
	LogText[m_cline][len]=0;
	LogTextLength[m_cline]=len;
	LogTextViewType[m_cline]=type;	// Phew, 
}

void CHILDLOG::LogAdd(char* szLog, ...)
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


void CHILDLOG::LogAddC(BYTE Color, char* szLog, ...)
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
void CHILDLOG::LogAddL(char* szLog, ...)
{
	char szBuffer[512];
	va_list pArguments;
}
void CHILDLOG::LogAddTD(char* szLog, ...)
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


void CHILDLOG::LogAddHeadHexFunc(int Type,char* data,int len)
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
			switch(Type)
			{
			case 2:
				fprintf(logfp, "Send:[%d] : (", len);
				break;
			case 3:
				fprintf(logfp, "FDWrite:[%d] : (", len);
				break;
			case 4:
				fprintf(logfp, "Recv:[%d] : (", len);
				break;
			default:
				fprintf(logfp, "%d[%d] : (", Type ,len);
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


void CHILDLOG::LogAddHeadHex(char* str,unsigned char* data, int len)
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
			fprintf(logfp, "[%s][%d] : (", str,len);

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

void CHILDLOG::LogTextClear()
{
	int n;
	for ( n=0; n<CHILD_LOG_TEXT_LINE; n++)
	{
		memset(&LogText[n], 0, CHILD_LOG_TEXT_LENGTH);
		LogTextLength[n]=0;	// WORD
		LogTextViewType[n]=0;	// BYTE
	}
}

void CHILDLOG::LogTextPaint(HWND hWnd)
{
	HDC hdc;
	int total;
	int n;
	
	
	hdc=GetDC(hWnd);
	total= CHILD_LOG_TEXT_LINE;

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
			SetTextColor(hdc, RGB(218, 165, 32));
			break;
		default:
			SetTextColor(hdc, RGB(0, 0, 0));
			break;
		}
		if (strlen(LogText[n])>1)
		{
			TextOut( hdc, 0, total*15 + 100, LogText[n], strlen(LogText[n])); 
			SetBkMode(hdc,TRANSPARENT);
		}
		n--;
		if (n<0)
		{
			n=CHILD_LOG_TEXT_LINE-1;
		}
	}
	ReleaseDC(hWnd, hdc);
}

int CHILDLOG::LogGetFileName()
{
	return 0;
}

void CHILDLOG::LogClose()
{
	if (logfp!=0)
	{
		fclose(logfp);
	}
	DeleteCriticalSection(&LogCritical);
}