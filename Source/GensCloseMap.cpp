#include "stdafx.h"
#include "GameServer.h"
#include "LogProc.h"
#include "GameMain.h"
#include "TNotice.h"
#include "..\include\readscript.h"
#include "..\common\winutil.h"
#include "SCFPvPSystem.h"
#include "GensCloseMap.h"


#if (PACK_EDITION>=3)

cGensCloseMap GensCloseMap;

void cGensCloseMap::Init(char * FilePath)
{
	this->filePath				= FilePath;
	this->Start					= 0;
	this->Enabled				= GetPrivateProfileInt("CloseMap", "SCFGensCloseMapEnabled",0, FilePath) ;
	this->Minutes				= GetPrivateProfileInt("CloseMap", "SCFGensCloseMapMinutesTime",5, FilePath) ;

	memset(&this->Maps[0],0,sizeof(this->Maps));
	memset(&this->Kills[0],0,sizeof(this->Kills));

	char etiqueta[8] = {0};
    for(int n=0;n<MAX_MAP_NUMBER;n++) 
	{
		wsprintf(etiqueta,"Map%02d", n);
		this->Maps[n] = GetPrivateProfileInt("GensCloseMap", etiqueta,0, FilePath) ;
	}

	this->Maps[MAP_INDEX_LORENCIA] = 0;
	this->Maps[MAP_INDEX_DEVIAS] = 0;
	this->Maps[MAP_INDEX_NORIA] = 0;

	for(int i=11;i<24;i++)
		this->Maps[i] = 0;				//BC && CC
	
	this->Maps[MAP_INDEX_DEVILSQUARE2] = 0;

	for(int i=45;i<54;i++)
		this->Maps[i] = 0;				//IT, Elbeland, BC, CC

	for(int i=64;i<73;i++)
		this->Maps[i] = 0;				//DUEL, GOER, IMPERIAL GUARDIAN

	LogAddTD("[SCF GensCloseMap] - %s file is Loaded",FilePath);
}

bool cGensCloseMap::CheckMap2(int aIndex, BYTE Map)
{
	if(this->Enabled == FALSE)
		return false;

	if(this->Start == false)
		return false;

	if ( OBJMAX_RANGE(aIndex) == FALSE )
		return false;

	if ( gObj[aIndex].Type != OBJ_USER || gObj[aIndex].Connected <= PLAYER_LOGGED )
		return false;

	if(this->Maps[Map] == 0)
		return false;

	return true;
}

bool cGensCloseMap::CheckMap(int aIndex, BYTE Map)
{
	if(this->Enabled == FALSE)
		return false;

	if ( OBJMAX_RANGE(aIndex) == FALSE )
		return false;

	if ( gObj[aIndex].Type != OBJ_USER || gObj[aIndex].Connected <= PLAYER_LOGGED )
		return false;

	if(this->Maps[Map] == 0)
		return false;
	
	if(this->Maps[Map] == 3)
		return false;

	if((gObj[aIndex].AuthorityCode == 0x3F) || (gObj[aIndex].Authority >= 8))
		return false;

	if(this->Start == true)
	{
		if(this->Maps[Map] < 3)
		{
			char sbuf[256]={0};
			wsprintf(sbuf,"Duprian: %d - Valiant: %d",this->Kills[Map].Duprian,this->Kills[Map].Valiant);
			GCServerMsgStringSend(sbuf,aIndex, 0x00);

			return false;
		}
	}else
	{		
		if(this->Maps[Map] == gObj[aIndex].GensFam)
		{
			return false;
		}
	}
	
	if(this->Maps[Map] > 3)
	{
		if(this->Maps[Map] == gObj[aIndex].GensFam + 3)
		{
			return false;
		}
	}

	if(this->Maps[Map] == 1 || this->Maps[Map] == 4)
	{
		GCServerMsgStringSend(lMsg.Get(MSGGET(14, 151)),aIndex, 0x01);
	}else
	{
		GCServerMsgStringSend(lMsg.Get(MSGGET(14, 152)),aIndex, 0x01);
	}

	return true;
}

void cGensCloseMap::StartEvent()
{
	if((this->Start == false) && (this->Enabled == 1))
		_beginthread( cGensCloseMap__InsideTrigger, 0, NULL  );
}

void cGensCloseMap::AddKills(int aIndex)
{
	if(this->Enabled == FALSE)
		return;

	if(this->Start == false)
		return;

	if ( OBJMAX_RANGE(aIndex) == FALSE )
		return;

	if ( gObj[aIndex].Type != OBJ_USER || gObj[aIndex].Connected <= PLAYER_LOGGED )
		return;

	int Map = gObj[aIndex].MapNumber;

	if(this->Maps[Map] == 0)
		return;

	if(this->Maps[Map] > 3)
		return;

	if(gObj[aIndex].GensFam == 1)
		this->Kills[Map].Duprian++;
	else
		this->Kills[Map].Valiant++;

	char sbuf[256]={0};
	wsprintf(sbuf,"Duprian: %d - Valiant: %d",this->Kills[Map].Duprian,this->Kills[Map].Valiant);
	GCServerMsgStringSend(sbuf,aIndex, 1);
}

void cGensCloseMap::Finish(char * FilePath)
{
    for(int n=0;n<MAX_MAP_NUMBER;n++) 
	{
		if(this->Maps[n] != 0 && this->Maps[n] != 4 && this->Maps[n] != 5)
		{
			char etiqueta[8] = {0};
			wsprintf(etiqueta,"Map%02d", n);

			if(this->Kills[n].Duprian > this->Kills[n].Valiant)
				this->Maps[n] = 1;
			else if (this->Kills[n].Duprian < this->Kills[n].Valiant)
				this->Maps[n] = 2;
			else
				this->Maps[n] = 3;

			char sbuf[50]={0};
			itoa(this->Maps[n],sbuf,10);
			WritePrivateProfileString("GensCloseMap",etiqueta,sbuf,FilePath);
		}
	}

	GensCloseMap.Start = false;
	this->Warp();
	memset(&this->Kills[0],0,sizeof(this->Kills));
}

void cGensCloseMap::Warp()
{
	for(int iIndex=OBJ_STARTUSERINDEX;iIndex<OBJMAX;iIndex++)
	{
		if(gObj[iIndex].Connected == PLAYER_PLAYING )
		{
			if(this->CheckMap(iIndex,gObj[iIndex].MapNumber) == true)
			{		
				if(this->Maps[gObj[iIndex].MapNumber] == 1 || this->Maps[gObj[iIndex].MapNumber] == 4)
				{
					GCServerMsgStringSend(lMsg.Get(MSGGET(14, 146)),iIndex, 0x00);
				}else
				{
					GCServerMsgStringSend(lMsg.Get(MSGGET(14, 147)),iIndex, 0x00);
				}
				//Warp Lorencia
				
				GCServerMsgStringSend(lMsg.Get(MSGGET(14, 148)),iIndex, 0x01);
				gObjMoveGate(gObj[iIndex].m_Index,17);
			}else
			{
				if(this->Maps[gObj[iIndex].MapNumber] != 0 && this->Maps[gObj[iIndex].MapNumber] != 3 && this->Maps[gObj[iIndex].MapNumber] != 4 && this->Maps[gObj[iIndex].MapNumber] != 5)
				{
					GCServerMsgStringSend(lMsg.Get(MSGGET(14, 149)),iIndex, 0x00);
				}else
				{
					if(this->Maps[gObj[iIndex].MapNumber] == 3)
					{
						GCServerMsgStringSend(lMsg.Get(MSGGET(14, 150)),iIndex, 0x00);
					}
				}
			}
		}
	}
}

void cGensCloseMap__InsideTrigger(void * lpParam)
{
	GensCloseMap.Start = true;

	LogAddTD("[Gens Maps] Maps Open!!");
	AllSendServerMsg(lMsg.Get(MSGGET(14, 144)));


	Sleep(GensCloseMap.Minutes * 60000);

	GensCloseMap.Finish(GensCloseMap.filePath);

	LogAddTD("[Gens Maps] Maps Close!!");
	AllSendServerMsg(lMsg.Get(MSGGET(14, 145)));

	_endthread();
}

#endif