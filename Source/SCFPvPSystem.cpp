#include "stdafx.h"
#include "GameServer.h"
#include "LogProc.h"
#include "GameMain.h"
#include "..\include\readscript.h"
#include "..\common\winutil.h"
#include "SCFPvPSystem.h"
#include "HalloweenEvent.h"

PvPSystem PvP;

void PvPSystem::Read(char * FilePath)
{
	this->gNonPKServer			= GetPrivateProfileInt("PKServerSettings", "NonPKServer",0, FilePath);
	this->gPkLimitFree			= GetPrivateProfileInt("PKServerSettings", "PkLimitFree",0, FilePath);
	this->gPkCanBuy				= GetPrivateProfileInt("PKServerSettings", "PkCanBuy",0, FilePath);

	this->gPKLevelIncreaseOff	= GetPrivateProfileInt("PKPlayerSettings", "PKLevelIncreaseOff",0, FilePath);
	this->gPkTimeInMinutes		= GetPrivateProfileInt("PKPlayerSettings", "PKLevelDecreaseMinutes",180, FilePath);
	this->gPkItemDrop			= GetPrivateProfileInt("PKPlayerSettings", "PKItemDrop",0, FilePath);
	this->gPkItemDropPercent	= GetPrivateProfileInt("PKPlayerSettings", "PkItemDropPercent",0, FilePath);
	this->gPKDropMaxLevel		= GetPrivateProfileInt("PKPlayerSettings", "PKItemDropMaxLevel",0, FilePath);
	this->gPKDropMaxExc			= GetPrivateProfileInt("PKPlayerSettings", "PKItemDropMaxExc",0, FilePath);
	this->gHeroExtraDropPercent = GetPrivateProfileInt("PKPlayerSettings", "HeroExtraDropPercent",0, FilePath);

	this->gObjPlayerKiller_PKMap= GetPrivateProfileInt("PKMap", "SCFPKMap",41, FilePath) ;
	this->gObjPlayerKiller_PKX1	= GetPrivateProfileInt("PKMap", "SCFPKX1",0, FilePath) ;
	this->gObjPlayerKiller_PKY1	= GetPrivateProfileInt("PKMap", "SCFPKY1",0, FilePath) ;
	this->gObjPlayerKiller_PKX2	= GetPrivateProfileInt("PKMap", "SCFPKX2",255, FilePath) ;
	this->gObjPlayerKiller_PKY2	= GetPrivateProfileInt("PKMap", "SCFPKY2",255, FilePath) ;

	this->Enabled				= GetPrivateProfileInt("PVPSystem", "SCFPvPSystem",0, FilePath);

	if(this->Enabled == TRUE)
	{
		this->gNonPKServer = 0;

		char etiqueta[8];
	    for(int n=0;n<MAX_MAP_NUMBER;n++) 
		{
			wsprintf(etiqueta,"Map%02d", n);
			this->MapPvP[n] = GetPrivateProfileInt("MapsPvP", etiqueta,1, FilePath) ;
		}
	}
	LogAddTD("[SCF PvPSystem] - %s file is Loaded",FilePath);

	if(this->Enabled == TRUE)
		LogAddTD("[SCF PvPSystem] PVP System Enabled");
	if((this->gNonPKServer != 0)&&(this->Enabled != TRUE))
		LogAddTD("[SCF PvPSystem] NON-PK Server");
	if(this->gPkLimitFree != 0)
		LogAddTD("[SCF PvPSystem] PK Limit Free Server");
	if(this->gPkTimeInMinutes != 0)
		LogAddTD(lMsg.Get(409), this->gPkTimeInMinutes);
	if(this->gPkItemDrop != 0)
		LogAddTD(lMsg.Get(MSGGET(1, 155)));
	else
		LogAddTD(lMsg.Get(MSGGET(1, 156)));
}

BOOL PvPSystem::Check(int Map)
{
#if (PACK_EDITION>=2)
	if (HalloweenEvent.Start == true)
	{
		if(HalloweenEvent.CheckMap(Map) == true)
		{
			return FALSE;
		}
	}
#endif

	if(this->Enabled == TRUE)
	{
		if(this->MapPvP[Map] > 0)
			return FALSE;
		else
			return TRUE;
	}
	else
		return this->gNonPKServer;
}

BYTE PvPSystem::GetType(int Map)
{
	if(this->Enabled == TRUE)
	{
		return this->MapPvP[Map];
	}
	return 0xFF;
}

void PvPSystem::SetType(int Map, BYTE Type)
{
	this->MapPvP[Map] = Type;
}