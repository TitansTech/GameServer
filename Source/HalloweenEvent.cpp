#include "stdafx.h"
#include "GameServer.h"
#include "GameMain.h"
#include "LogProc.h"
#include "TNotice.h"
#include "..\include\readscript.h"
//#include "GameServerAuth.h"
#include "..\common\winutil.h"
#include "SCFPvPSystem.h"
#include "HalloweenEvent.h"

#if (PACK_EDITION>=2)

cHalloweenEvent HalloweenEvent;

void cHalloweenEvent::StartEvent()
{
	if((this->Start == false) && (this->Enabled == 1))
		_beginthread( cHalloweenEvent__InsideTrigger, 0, NULL  );
}

void cHalloweenEvent::Init(char * FilePath)
{
	this->Start					= 0;
	this->Enabled				= GetPrivateProfileInt("SCFHalloween", "SCFHalloweenEventSwitch",0, FilePath) ;
	this->Minutes				= GetPrivateProfileInt("SCFHalloween", "SCFHalloweenEventMinutesTime",5, FilePath) ;
	this->WarpRandom			= GetPrivateProfileInt("SCFHalloween", "SCFHalloweenEventWarpRandomLocation",1, FilePath) ;
	//this->Map					= GetPrivateProfileInt("Common", "SCFHalloweenEventMap",2, FilePath) ;

	g_bHalloweenDayEventOn = GetPrivateProfileInt("WZHalloween","SCFHalloweenWZOn",0, FilePath) ;
	g_iHalloweenDayEventJOLBlessDropRate = GetPrivateProfileInt("WZHalloween","SCFHalloweenWZJOLBlessDropRate",10, FilePath) ;
	g_iHalloweenDayEventJOLAngerDropRaTe = GetPrivateProfileInt("WZHalloween","SCFHalloweenWZJOLAngerDropRate",15, FilePath) ;
	g_iHalloweenDayEventJOLScreamDropRate = GetPrivateProfileInt("WZHalloween","SCFHalloweenWZJOLScreamDropRate", 20, FilePath) ;
	g_iHalloweenDayEventJOLFoodDropRate = GetPrivateProfileInt("WZHalloween","SCFHalloweenWZJOLFoodDropRate",30, FilePath) ;
	g_iHalloweenDayEventJOLDrinkDropRate = GetPrivateProfileInt("WZHalloween","SCFHalloweenWZJOLDrinkDropRate",25, FilePath) ;
	g_iHalloweenDayEventJOLPolymorphRingDropRate = GetPrivateProfileInt("WZHalloween","SCFHalloweenWZJOLPolymorphRingDropRate", 5, FilePath) ;

	char etiqueta[8] = {0};
    for(int n=0;n<MAX_MAP_NUMBER;n++) 
	{
		wsprintf(etiqueta,"Map%02d", n);
		Maps[n] = GetPrivateProfileInt("SCFHalloweenInMaps", etiqueta,0, FilePath) ;
	}

	LogAddTD("[SCF HalloweenEvent] - %s file is Loaded",FilePath);
}

bool cHalloweenEvent::CheckMap(BYTE Map)
{
	if(this->Maps[Map] == 1)
		return true;

	return false;
}

void cHalloweenEvent::ViewPort()
{
	for(int iIndex=OBJ_STARTUSERINDEX;iIndex<OBJMAX;iIndex++)
	{
		if(gObj[iIndex].Connected == PLAYER_PLAYING )
		{
			//if(gObj[iIndex].MapNumber == HalloweenEvent.Map)
			if(this->CheckMap(gObj[iIndex].MapNumber) == true)
			{
				gObjClearViewport(&gObj[iIndex]);
			}
		}
	}
}

void cHalloweenEvent::WarpPlayersRandomLocation()
{
	if (this->WarpRandom == 1)
	{
		BYTE MapsWarp[MAX_MAP_NUMBER];
		short MapsWarpX[MAX_MAP_NUMBER];
		short MapsWarpY[MAX_MAP_NUMBER];

		memset(MapsWarp, 0, MAX_MAP_NUMBER);
		memset(MapsWarpX, 0, MAX_MAP_NUMBER);
		memset(MapsWarpY, 0, MAX_MAP_NUMBER);

		for(int iIndex=OBJ_STARTUSERINDEX;iIndex<OBJMAX;iIndex++)
		{
			if(gObj[iIndex].Connected == PLAYER_PLAYING )
			{
				if(this->CheckMap(gObj[iIndex].MapNumber) == true)
				{
					int tempMap = gObj[iIndex].MapNumber;
					if (MapsWarp[tempMap] == 0)
					{
						while ( gMSetBase.GetBoxPosition(tempMap, 50, 50, 200, 200, MapsWarpX[tempMap], MapsWarpY[tempMap]) == 0 )
						{

						}
						MapsWarp[tempMap] = 1;
					} else {
						while ( gMSetBase.GetBoxPosition(tempMap, MapsWarpX[tempMap], MapsWarpY[tempMap], MapsWarpX[tempMap]+10, MapsWarpY[tempMap]+10, MapsWarpX[tempMap], MapsWarpY[tempMap]) == 0 )
						{

						}
					}

					gObjTeleport(iIndex, tempMap, MapsWarpX[tempMap], MapsWarpY[tempMap]);

				}
			}
		}

	}
}

void cHalloweenEvent__InsideTrigger(void * lpParam)
{
	HalloweenEvent.Start = true;

	LogAddTD("[Halloween PK] Event Start");
	AllSendServerMsg(lMsg.Get(MSGGET(14, 137)));
	//HalloweenEvent.ViewPort();
	HalloweenEvent.ViewPort();

	Sleep(10000);
	HalloweenEvent.WarpPlayersRandomLocation();

	//BYTE pvpMap = PvP.GetType(HalloweenEvent.Map);
	//PvP.SetType(HalloweenEvent.Map,1);

	Sleep(HalloweenEvent.Minutes * 60000);

	HalloweenEvent.Start = false;
	//PvP.SetType(HalloweenEvent.Map,pvpMap);
	HalloweenEvent.ViewPort();

	LogAddTD("[Halloween PK] Event End");
	AllSendServerMsg(lMsg.Get(MSGGET(14, 138)));

	_endthread();
}

#endif