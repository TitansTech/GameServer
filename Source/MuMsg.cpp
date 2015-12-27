#include "stdafx.h"
#include "GMMng.h"
#include "LogToFile.h"
#include "logproc.h"
#include "GameMain.h"
#include "protocol.h"
#include "..\common\winutil.h"
#include "giocp.h"

CMsg lMsg;

void CMsg::DataFileLoad(char * filename)
{
	unsigned char szTemp[256];
	int Index;
	int Type;
	int SubIndex;

	FILE* WTFFile;

	WTFFile = fopen(filename, "r");
	while (!feof(WTFFile))
	{
		memset(&szTemp, 0, sizeof(szTemp) );
		fscanf(WTFFile,"%d %d %[^\n]%*c",&Type,&SubIndex,szTemp);
		Index = MSGGET(Type,SubIndex);
		if(Index == 3724)
		{
			int i=0;
			i=4;
		}
		if(Index < 32767)
		{
			strcpy(this->msg[Index],(char*)szTemp);
		}
	} 
	fclose(WTFFile);
}

LPSTR CMsg::Get(int Index) 
{
	return ((char*)this->msg[Index]);
}