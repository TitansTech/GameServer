// ------------------------------
// Decompiled by Deathway
// Date : 2007-03-09
// ------------------------------
//------------------------------------------
// WinUtil.h	: Status Exactly as GameServer
//------------------------------------------
#ifndef __WINUTIL_H
#define __WINUTIL_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\source\protocol.h"
#include "..\source\user.h"

//------------------------------------------
// WinUtil.cpp Functions
//------------------------------------------
int roundInt(int d);
double roundDbl(double d);
void ToHexSinComa(char * out, char * data, int c_len);
void BuxConvert(char* buf, int size);
void BuxConvert2(char* buf, int size);
BOOL StringReplace(char * str,int maxSize,char Replace,char ReplaceTo);
BOOL SQLSyntexCheck(char* SQLString);
BOOL SQLSyntexCheckConvert(char* SQLString);
BOOL SpaceSyntexCheck(char* string);
BOOL StrHangulCheck(char* str);
void FileSaveString(char* filename, char* string);
BOOL IsFile(char* filename);
void GetTodayString(char* szDate);
void PHeadSetB(LPBYTE lpBuf, BYTE head, int size);
void PHeadSubSetB(LPBYTE lpBuf, BYTE head, BYTE sub, int size);
void PHeadSetW( LPBYTE lpBuf, BYTE head, int size) ;
void PHeadSubSetW(LPBYTE lpBuf, BYTE head, BYTE sub, int size);
void PHeadSetBE(LPBYTE lpBuf, BYTE head,int size);
void PHeadSubSetBE(LPBYTE lpBuf, BYTE head,BYTE sub, int size);
int RandomNum(int Start, int End, bool UseSRand);

#if (WL_PROTECT==1)

#if (MAC_PROTECT_OLD==1)
void CheckMemoryTeaser_MAC_Comparator(int param);
void WinUtil_Teaser();
#endif
void Teaser__InsideTrigger(void * lpParam);
#if (MAC_PROTECT==1)
void curl_protection();
#endif
#endif

class char_ID
{

public:

	char_ID(LPSTR szName)	// line : 44
	{
		memset(this->Name, 0, sizeof(this->Name));

		if ( szName != NULL )
		{
			memcpy(this->Name, szName, MAX_ACCOUNT_LEN);
		}
	}	// line : 49

	LPSTR GetBuffer()	// line : 52
	{
		return this->Name;
	}	// line : 54

	int  GetLength()	// line : 63
	{
		return strlen(this->Name);
	}	// line : 65

private:

	char Name[MAX_ACCOUNT_LEN+1];	// 0

};





#endif