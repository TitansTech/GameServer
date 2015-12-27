
// ------------------------------
// Decompiled by Deathway
// Date : 2007-03-09
// ------------------------------

// GS-N 0-99-60T Status : Completed - Need Apply Fixes to prevent crashes - szTerminated String
// GS-N	1.00.18	JPN	0x004A4780	-	Completed

#include "stdafx.h"
#include "LogToFile.h"
#include "logproc.h"
#include "Gamemain.h"

void WriteTxt(LPSTR FileName,LPSTR String)
{
	FILE *f;
	f = fopen (FileName,"a+");
	char Print[255]={0};
	wsprintf(Print,"\n%s",String);
	if (fputs (Print,f) == EOF) 
	{
		LogAddTD("Error: Cant Write File: %s",FileName);
	}
	fclose (f);
}

CLogToFile::CLogToFile(const LPSTR LogFileName,const LPSTR LogDirectoryName, BOOL bWithDate)
{
	if (strcmp(LogFileName, "") == 0)
	{
		strcpy(this->m_szLogFileName , "LOGDATA");
	}
	else
	{
		strcpy(this->m_szLogFileName , LogFileName);
	}

	
	if (!strcmp(LogDirectoryName, ""))
	{
		strcpy(this->m_szLogDirectoryName , "LOG");
	}
	else
	{
		strcpy(this->m_szLogDirectoryName  , LogDirectoryName);
	}
	
	this->m_bWithDate  = bWithDate;	

	CreateDirectory(LogDirectoryName, NULL);

	InitializeCriticalSection(&this->m_critLogToFile );
	
}


CLogToFile::~CLogToFile()
{
	DeleteCriticalSection(&this->m_critLogToFile );
}


void CLogToFile::Output( LPSTR fmt, ...)
{
	bool EntCrit = false;
	__try
	{
	va_list argptr;
	int iChrWritten;
	char szLogFileName[260]; // Is StringZero Terminated

	SYSTEMTIME strSystime;
	
	EnterCriticalSection(&this->m_critLogToFile );
	EntCrit = true;

	va_start(argptr, fmt);
	iChrWritten=vsprintf(this->m_cBUFFER , fmt, argptr);
	va_end(argptr);

	GetLocalTime(&strSystime);

	wsprintf(szLogFileName, "%s\\%s %04d-%02d-%02d_%s.txt",
		&this->m_szLogDirectoryName[0] , &this->m_szLogFileName [0],
		strSystime.wYear, strSystime.wMonth, strSystime.wDay,
		szServerName);

	if ( (this->m_fLogFile = fopen(szLogFileName, "a+"))==0)	//flipe +'0
	{
		LeaveCriticalSection(&this->m_critLogToFile );
		EntCrit = false;
	}
	else
	{
		if (this->m_bWithDate ==0)
		{
			fprintf(this->m_fLogFile , "%s\n", this->m_cBUFFER);
		}
		else
		{
			fprintf(this->m_fLogFile , "%02d:%02d:%02d  %s\n", strSystime.wHour, strSystime.wMinute, strSystime.wSecond, this->m_cBUFFER);
		}
		fclose(this->m_fLogFile );

		LeaveCriticalSection(&this->m_critLogToFile );
		EntCrit = false;
	}
	}__except( EXCEPTION_ACCESS_VIOLATION == GetExceptionCode() )
	{
		if(EntCrit == true)
			LeaveCriticalSection(&this->m_critLogToFile);
	}
}

	



