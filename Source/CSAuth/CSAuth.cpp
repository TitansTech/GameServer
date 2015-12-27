// CSAuth.cpp : Defines the entry point for the DLL application.
//
#include "stdafx.h"
#include "../stdafx.h"
#include "CSAuth.h"

unsigned char g_szAuthTable[8][5];

char szAuthIndexFilename[256];

unsigned long g_AuthIndex = -1;

int _LoadAuthTable(char* filename)
{
	int n = 0;
	unsigned char szAuthTable[40] = "";

	//memset(szAuthTable, 0, sizeof(szAuthTable));
	FILE * fp = fopen(filename, "rb");

	if ( fp == NULL )
	{
		return 0;
	}

	for ( n = 0; n < 8 ; n++ )
	{
		if ( fread(&szAuthTable[n], 1, 5, fp) < 5 )
		{
			break;
		}

		if ( fp->_flag == 16 )
		{
			break;
		}

		if ( szAuthTable[n] >= 32 )
		{
			break;
		}
	}

	fclose(fp);
	
	memset(g_szAuthTable,0, sizeof(g_szAuthTable));
	memcpy(g_szAuthTable, szAuthTable, sizeof(g_szAuthTable));

	return n;
}


int _LoadAuthIndex(char * filename)
{
	strcpy(szAuthIndexFilename, filename);

	FILE * fp = fopen(filename, "rt");

	if ( fp == NULL )
	{
		return 0;
	}

	fscanf(fp, "%lu", &g_AuthIndex);
	fclose(fp);
	
	if ( g_AuthIndex == -1 )
	{
		return 0;
	}

	return 1;
}

CCSAuth::CCSAuth()
{ 
	this->Init();
}

CCSAuth::~CCSAuth()
{
	return;
}

BOOL CCSAuth::IsAuth()
{
	return this->m_bIsAuth;
}

void CCSAuth::np_srandom(int iSeed)
{
	this->m_dwRandomSeed = iSeed;
}


void CCSAuth::Init()
{
	this->np_srandom(time(NULL));
	this->m_dwAuthDword = 0;
	this->m_bIsAuth = TRUE;
	memset(this->unk10, 0, 40);
	this->unkC = -1;
	this->unk3C = 0;
	memset(this->unk40, 0, 48);
	this->m_dwLastError = 0;
}

int CCSAuth::np_random()
{
	return this->m_dwRandomSeed;
}
	
DWORD CCSAuth::GetAuthDword()
{
	this->m_bIsAuth = FALSE;
	this->m_dwAuthDword = ((this->np_random() ^ this->np_random()) & 0xFFF0FFFF) | this->unkC << 18 ;
	return this->m_dwAuthDword;
}

DWORD CCSAuth::PPGetLastError()
{
	return this->m_dwLastError;
}