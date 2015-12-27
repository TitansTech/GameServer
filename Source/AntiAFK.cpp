#include "stdafx.h"
#include "GameServer.h"
#include "LogProc.h"
#include "GameMain.h"
#include "User.h"
#include "TNotice.h"
#include "..\common\winutil.h"
#include "..\include\readscript.h"
//#include "GameServerAuth.h"

#if (PACK_EDITION>=3)
AntiAFKClass AntiAFK;

ANTIAFKCHARINFO::ANTIAFKCHARINFO()
{
	this->Clear();
}

void ANTIAFKCHARINFO::Clear()
{
	this->AntiAFK_Map = 0;
	this->AntiAFK_X = 0;
	this->AntiAFK_Y = 0;
	this->AntiAFK_WarnNumber = 0;
}

void AntiAFKClass::Init(char * FilePath)
{
	this->Minutes			= GetPrivateProfileInt("AntiAFK", "SCFAntiAFKMinutesCheck", 15, FilePath);
	this->Warnings			= GetPrivateProfileInt("AntiAFK", "SCFAntiAFKMaxWarnings", 3, FilePath);
	this->Enabled			= GetPrivateProfileInt("AntiAFK", "SCFAntiAFKEnabled", 1, FilePath);

	if(this->Enabled == 1)
	{
		_beginthread( AntiAFKClass__Trigger, 0, NULL  );
	}
	LogAddTD("[Anti-AFK System] Enabled: %d / Warnings: %d / Interval (min): %d",this->Enabled,this->Warnings,this->Minutes);
}

void AntiAFKClass__Trigger(void * lpParam)
{
	while(AntiAFK.Enabled)
	{
		Sleep((1000 * 60) * AntiAFK.Minutes);
		
		for ( int n = OBJ_STARTUSERINDEX ; n < OBJMAX ; n++)
		{
			if ( gObj[n].Connected == PLAYER_PLAYING )
			{
				if ( gObj[n].Type == OBJ_USER )
				{
					if(gObj[n].AFKCharClass->AntiAFK_X != 0 || gObj[n].AFKCharClass->AntiAFK_Y != 0)
					{
						if( (gObj[n].MapNumber == gObj[n].AFKCharClass->AntiAFK_Map) && ( (gObj[n].AFKCharClass->AntiAFK_X >= gObj[n].X - 5) && (gObj[n].AFKCharClass->AntiAFK_X <= gObj[n].X + 5) ) && ( (gObj[n].AFKCharClass->AntiAFK_Y >= gObj[n].Y - 5) && (gObj[n].AFKCharClass->AntiAFK_Y <= gObj[n].Y + 5) ) && (gObjItsInSafeZone(gObj[n].m_Index) == 0) )
						{
							if(gObj[n].MapNumber != MAP_INDEX_LORRENMARKET) //Exclude Loren_Market from List
							{
								if(gObj[n].AFKCharClass->AntiAFK_WarnNumber > 0)
								{
									char sBuf[512]={0};
									wsprintf(sBuf, "[Anti-AFK] Current Warning Number: %d",gObj[n].AFKCharClass->AntiAFK_WarnNumber);
									LogAddTD("[Anti-AFK][%s][%s] Current Warning Number: %d",gObj[n].AccountID,gObj[n].Name, gObj[n].AFKCharClass->AntiAFK_WarnNumber);
									GCServerMsgStringSend(sBuf,gObj[n].m_Index, 0x01);
								}
								if(gObj[n].AFKCharClass->AntiAFK_WarnNumber < AntiAFK.Warnings)
								{
									gObj[n].AFKCharClass->AntiAFK_WarnNumber++;
								}
								if(gObj[n].AFKCharClass->AntiAFK_WarnNumber >= AntiAFK.Warnings)
								{
									if (gObjMoveHackCheck(n) == TRUE)
									{
										LogAddTD("[Anti-AFK][%s][%s] Warping To Lorencia Safe Zone",gObj[n].AccountID,gObj[n].Name);
										GCServerMsgStringSend("[Anti-AFK] Warping To Lorencia Safe Zone",gObj[n].m_Index, 0x01);
										gObj[n].AFKCharClass->AntiAFK_WarnNumber = 0;
										gObjMoveGate(gObj[n].m_Index,17);
									} else {
										LogAddTD("[Anti-AFK][%s][%s] DC USER => Could Not warp to Lorencia Safe Zone",gObj[n].AccountID,gObj[n].Name);
										CloseClient(n);
									}
								}
							}
						}else
						{
							if (gObj[n].AFKCharClass->AntiAFK_WarnNumber > 0)
							{
								char sBuf[512]={0};
								gObj[n].AFKCharClass->AntiAFK_WarnNumber--;
								LogAddTD("[Anti-AFK][%s][%s] Reducing Warning Number: %d from %d",gObj[n].AccountID,gObj[n].Name,gObj[n].AFKCharClass->AntiAFK_WarnNumber,gObj[n].AFKCharClass->AntiAFK_WarnNumber+1);
								wsprintf(sBuf, "[Anti-AFK] Reducing Warning Number: %d from %d",gObj[n].AFKCharClass->AntiAFK_WarnNumber,gObj[n].AFKCharClass->AntiAFK_WarnNumber+1);
								GCServerMsgStringSend(sBuf,gObj[n].m_Index, 0x01);
							} else 
							{
								gObj[n].AFKCharClass->AntiAFK_WarnNumber = 0;
							}
						}
					}
					gObj[n].AFKCharClass->AntiAFK_Map = gObj[n].MapNumber;
					gObj[n].AFKCharClass->AntiAFK_X = gObj[n].X;
					gObj[n].AFKCharClass->AntiAFK_Y = gObj[n].Y;
				}
			}
		}
	}
}
#endif