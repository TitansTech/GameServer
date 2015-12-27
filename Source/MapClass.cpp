// ------------------------------
// Decompiled by Deathway
// Date : 2007-03-09
// ------------------------------
#include "stdafx.h"
#include "MapClass.h"
#include "LogProc.h"
#include "GameMain.h"
#include "user.h"
#include "Protocol.h"
#include "BloodCastle.h"
#include "DevilSquare.h"
#include "ChaosCastle.h"
#include "IllusionTemple.h"
#include "ImperialGuardian.h"
#include "DoppelGanger.h"
#include "protection.h"

// GS-N 0.99.60T 0x00478170 - Completed
//	GS-N	1.00.18	JPN	0x0048F140	-	Completed


MapClass::MapClass()
{
	this->m_Weather =0;
	this->m_WeatherTimer=GetTickCount();
	this->m_NextWeatherTimer=(rand()%10000)+10000;
	this->init();
	
	SetRect(&this->gRegenRect[MAP_INDEX_LORENCIA], 130, 116, 151, 137);
	SetRect(&this->gRegenRect[MAP_INDEX_DUNGEON], 106, 236, 112, 243);
	SetRect(&this->gRegenRect[MAP_INDEX_DEVIAS], 197, 35, 218, 50);
	SetRect(&this->gRegenRect[MAP_INDEX_NORIA], 174, 101, 187, 125);
	SetRect(&this->gRegenRect[MAP_INDEX_LOSTTOWER], 201, 70, 213, 81);
	SetRect(&this->gRegenRect[MAP_INDEX_RESERVED], 89, 135, 90, 136);
	SetRect(&this->gRegenRect[MAP_INDEX_BATTLESOCCER], 89, 135, 90, 136);
	SetRect(&this->gRegenRect[MAP_INDEX_ATLANS], 14, 11, 27, 23);
	SetRect(&this->gRegenRect[MAP_INDEX_TARKAN], 187, 54, 203, 69);

	SetRect(&this->gRegenRect[MAP_INDEX_AIDA], 82, 8, 87, 14);
	SetRect(&this->gRegenRect[MAP_INDEX_CRYWOLF_FIRSTZONE], 133, 41, 140, 44);
	SetRect(&this->gRegenRect[MAP_INDEX_BARRACKS], 30, 75, 33, 78);
	SetRect(&this->gRegenRect[MAP_INDEX_ELBELAND], 50, 220, 56, 226);
	SetRect(&this->gRegenRect[MAP_INDEX_SWAMP], 135, 105, 145, 115);
	SetRect(&this->gRegenRect[MAP_INDEX_RAKLION], 220, 210, 223, 212);
	SetRect(&this->gRegenRect[MAP_INDEX_VULCANUS], 110, 120, 125, 135);	
	SetRect(&this->gRegenRect[MAP_INDEX_KARLUTAN1], 124, 123, 127, 125);
	SetRect(&this->gRegenRect[MAP_INDEX_KARLUTAN2], 162, 16, 163, 17);
	this->m_ItemCount=0;
}





MapClass::~MapClass()
{
	this->free();
}



BOOL gObjItsInSafeZone(int PlayerID)
{
	unsigned char btMapAttribute = MapC[gObj[PlayerID].MapNumber].GetAttr(gObj[PlayerID].X,gObj[PlayerID].Y);
	if( ((btMapAttribute & 1) == 1) && 
		(DS_MAP_RANGE(PlayerID) == FALSE) && 
		(BC_MAP_RANGE(PlayerID) == FALSE) &&
		(IT_MAP_RANGE(PlayerID) == FALSE) &&
#if (PACK_EDITION>=3)
		(DG_MAP_RANGE(PlayerID) == FALSE) &&
#endif
#if (PACK_EDITION>=2)
		(IMPERIALGUARDIAN_MAP_RANGE(PlayerID) == FALSE) &&
#endif
		(CC_MAP_RANGE(PlayerID) == FALSE))//If safezone
	{
		return 1;
	}else{
		return 0;
	}
}

BOOL MapNumberCheck(int map)
{
	if ( map < 0 || map > MAX_MAP_NUMBER-1 )
	{
		return FALSE;
	}
	return TRUE;
}




void MapClass::GetLevelPos( short level, short& ox, short& oy)
{

	int x=0;
	int y=0;
	int levelpos = 0;
	int loopcount = MAX_MAP_NUMBER;
	BYTE attr=0;
	int w=0;
	int h=0;

	while ( loopcount-- != 0 )
	{
		w = this->gRegenRect[levelpos].right - this->gRegenRect[levelpos].left;
		h = this->gRegenRect[levelpos].bottom - this->gRegenRect[levelpos].top;

		__try
		{
			x = (short)(this->gRegenRect[levelpos].left + (rand()%w));
			y = (short)(this->gRegenRect[levelpos].top  + (rand()%h));
		}
		__except(w=1, h=1,1)
		{
		}

		attr = this->GetAttr(x, y);

		if ( (attr & 4) != 4  && (attr & 8) != 8 ) 
		{
			ox = x;
			oy = y;
			return;
		}
	}
	
	LogAdd( lMsg.Get( MSGGET(1, 204 )) , __FILE__, __LINE__);
	x = this->gRegenRect[levelpos].left;
	y = this->gRegenRect[levelpos].top;
}






void MapClass::GetRandomLengthPos(short& x, short& y, int length)
{
	int lc = 10;	// loopcount
	int px;
	int py;
	BYTE attr;
	int maxlen;

	if ( length < 1 )
	{
		length = 1;
	}

	maxlen = length*2;

	while ( lc-- != 0 )
	{
		px = (x-length)+(rand()%maxlen);
		py = (y-length)+(rand()%maxlen);

		attr = this->GetAttr(px, py);

		if ( (attr & 4) != 4  && (attr & 8) != 8 )
		{
			x = px;
			y = py;
			return;
		}
	}
}




void MapClass::GetMapPos(short Map, short& ox, short& oy)
{
	short OriginalMap = Map;
	if ( Map != MAP_INDEX_TARKAN )
	{
		if ( Map == MAP_INDEX_AIDA )
			Map = MAP_INDEX_AIDA;
		else if ( Map == MAP_INDEX_CRYWOLF_FIRSTZONE )
			Map = MAP_INDEX_CRYWOLF_FIRSTZONE;
		else if ( Map == MAP_INDEX_BARRACKS )
			Map = MAP_INDEX_BARRACKS;
		else if ( Map == MAP_INDEX_ELBELAND )
			Map = MAP_INDEX_ELBELAND;
		else if ( Map == MAP_INDEX_RAKLION )
			Map = MAP_INDEX_RAKLION;
		else if ( Map == MAP_INDEX_SWAMP )
			Map = MAP_INDEX_SWAMP;
		else if ( Map == MAP_INDEX_VULCANUS )
			Map = MAP_INDEX_VULCANUS;
		else if ( Map > MAP_INDEX_LOSTTOWER && Map != MAP_INDEX_ATLANS)
			Map = MAP_INDEX_LORENCIA;
	}

	int x=0;
	int y=0;
	int levelpos = Map;
	int loopcount=50;
	BYTE attr=0;
	int w=0;
	int h=0;

	while ( loopcount-- != 0 )
	{
		w = this->gRegenRect[levelpos].right - this->gRegenRect[levelpos].left;
		h = this->gRegenRect[levelpos].bottom - this->gRegenRect[levelpos].top;

		__try
		{
			x = (short)(this->gRegenRect[levelpos].left + (rand()%w));
			y = (short)(this->gRegenRect[levelpos].top  + (rand()%h));
		}
		__except(w=1,h=1,1)
		{
		}
		attr = this->GetAttr(x, y);

		if ( (attr & 4) != 4  && (attr & 8) != 8 )
		{
			ox = x;
			oy = y;
			return;
		}
	}				

	if(OriginalMap != MAP_INDEX_BATTLESOCCER)
		LogAdd( lMsg.Get( MSGGET(1, 204 )) , __FILE__, __LINE__);

	x = this->gRegenRect[levelpos].left;
	y = this->gRegenRect[levelpos].top;
}



void MapClass::LoadMapAttr(char * filename, int MapNumber)
{
	if (this->AttrLoad(filename) == FALSE )
	{
		MsgBox( lMsg.Get( MSGGET ( 2, 52 )));
	}

	this->thisMapNumber = MapNumber;
}




BYTE MapClass::GetWeather()
{
	BYTE weather = this->m_Weather *16;
	weather |= this->m_WeatherVariation ;
	return weather;
}




void MapClass::SetWeather(BYTE a_weather, BYTE a_variation)
{
	LPOBJ lpObj;
	BYTE weather;
	int n;

	this->m_Weather = a_weather;
	this->m_WeatherVariation = a_variation;
	weather = this->m_Weather * 16;	// like << 4
	weather |= this->m_WeatherVariation;
	
	for ( n=OBJ_STARTUSERINDEX ; n<OBJMAX ; n++ )
	{
		lpObj = &gObj[n];

		if ( lpObj->Connected > PLAYER_CONNECTED && lpObj->Live != 0 && lpObj->MapNumber == this->thisMapNumber )
		{
			CGWeatherSend(n, weather);
		}
	}
}





void MapClass::WeatherVariationProcess()
{
	LPOBJ lpObj;
	BYTE weather;

	if ( (GetTickCount() - this->m_WeatherTimer) > this->m_NextWeatherTimer)
	{
		this->m_WeatherTimer = GetTickCount();
		this->m_NextWeatherTimer = (rand()%10000) + 10000;
		this->m_WeatherVariation = rand()%10;
		this->m_Weather = rand()%3;

		weather = this->m_Weather*16;
		weather |= this->m_WeatherVariation;

		for (int n=OBJ_STARTUSERINDEX ; n<OBJMAX ; n++ )
		{
			lpObj = &gObj[n];

			if ( lpObj->Connected > PLAYER_CONNECTED && lpObj->Live != 0 && lpObj->MapNumber == this->thisMapNumber )
			{
				CGWeatherSend(n, weather);
			}
		}
	}
}





void MapClass::SaveItemInfo()
{
	return;	// Here goes a Macro
}





void MapClass::ItemInit()
{
	return; // Here goes a Macro
}





int MapClass::MonsterItemDrop(int type, int level, float dur,  int x, int y, BYTE Option1, BYTE Option2, BYTE Option3, BYTE NOption, BYTE SOption,  int aIndex, DWORD number, BYTE ItemEffectEx, BYTE ItemSocket1, BYTE ItemSocket2, BYTE ItemSocket3, BYTE ItemSocket4, BYTE ItemSocket5)
{
	int count;
	int counttot=0;
	BYTE attr = this->GetAttr(x, y);

	if ( (attr & 4) == 4  || (attr & 8) == 8 )
	{
		return -1;
	}
	count = this->m_ItemCount;

	while ( true )
	{
		if ( this->m_cItem[count].IsItem() == FALSE )
		{
			this->m_cItem[count].CreateItem(type, level, x, y, dur, Option1, Option2, Option3, NOption, SOption, number, ItemEffectEx);
			this->m_cItem[count].m_UserIndex = aIndex;
			this->m_ItemCount++;

			if ( this->m_ItemCount > MAX_MAPITEM-1 )
			{
				this->m_ItemCount = 0;
			}

			return count;

		}

		count++;

		if ( count > MAX_MAPITEM-1 )
		{
			count = 0;
		}

		counttot++;

		if ( counttot > MAX_MAPITEM-1 )
		{
			break;
		}
	}

	LogAdd(lMsg.Get(MSGGET(1, 205)), __FILE__, __LINE__ );

	return -1;
}







BOOL MapClass::ItemDrop(int type, int level, float dur,  int x, int y, BYTE Option1, BYTE Option2, BYTE Option3, BYTE NOption, BYTE SOption, DWORD number, int aIndex, int PetLevel, int PetExp, BYTE ItemEffectEx, BYTE ItemSocket1, BYTE ItemSocket2, BYTE ItemSocket3, BYTE ItemSocket4, BYTE ItemSocket5)
{
	int count;
	int counttot = 0;
	BYTE attr = this->GetAttr(x, y);

	if ( (attr&4) == 4 || (attr&8) == 8 )
	{
		return FALSE;
	}

	count = this->m_ItemCount;

	while ( true )
	{
		if ( this->m_cItem[count].IsItem() == FALSE )
		{
			this->m_cItem[count].DropCreateItem(type, level, x, y, dur, Option1, Option2, Option3, NOption, SOption, number, PetLevel, PetExp, ItemEffectEx,ItemSocket1,ItemSocket2,ItemSocket3,ItemSocket4,ItemSocket5);
			this->m_cItem[count].m_UserIndex = aIndex;
			this->m_ItemCount++;

			if ( this->m_ItemCount > MAX_MAPITEM-1 )
			{
				this->m_ItemCount = 0;
			}

			if ( IT_MAP_RANGE(this->thisMapNumber) != FALSE )
			{

			}

			if ( BC_MAP_RANGE(this->thisMapNumber) != FALSE )
			{
				if ( type == ITEMGET(13,19) )
				{
					if ( ((level<0)?FALSE:(level>2)?FALSE:TRUE) != FALSE )
					{
						int BCRest = MAP_INDEX_BLOODCASTLE1;
						if(this->thisMapNumber == MAP_INDEX_BLOODCASTLE8)
							BCRest = 45;
						if ( g_BloodCastle.m_BridgeData[this->thisMapNumber-BCRest].m_nBC_QUESTITEM_SERIAL != -1 )
						{
							if ( number == g_BloodCastle.m_BridgeData[this->thisMapNumber-BCRest].m_nBC_QUESTITEM_SERIAL )
							{
								if ( g_BloodCastle.m_BridgeData[this->thisMapNumber-BCRest].m_iBC_QUEST_ITEM_USER_INDEX != -1 )
								{
									LogAddTD("[Blood Castle] (%d) (Account:%s, Name:%s) Dropped Angel King's Weapon (%d)",
										this->thisMapNumber-(BCRest-1), gObj[g_BloodCastle.m_BridgeData[this->thisMapNumber-BCRest].m_iBC_QUEST_ITEM_USER_INDEX ].AccountID,
										gObj[g_BloodCastle.m_BridgeData[this->thisMapNumber-BCRest].m_iBC_QUEST_ITEM_USER_INDEX].Name,
										g_BloodCastle.m_BridgeData[this->thisMapNumber-BCRest].m_btBC_QUEST_ITEM_NUMBER);
									
									g_BloodCastle.m_BridgeData[this->thisMapNumber-BCRest].m_iBC_QUEST_ITEM_USER_INDEX = -1;
									g_BloodCastle.m_BridgeData[this->thisMapNumber-BCRest].m_btBC_QUEST_ITEM_NUMBER = 0;

									
								}
							}
						}
					}
				}
			}

			return TRUE;
		}

		count++;

		if ( count > MAX_MAPITEM-1 )
		{
			count = 0;
		}

		counttot++;

		if ( counttot > MAX_MAPITEM-1 )
		{
			return FALSE;
		}
	}

	LogAdd(lMsg.Get(MSGGET(1, 205)), __FILE__, __LINE__ );

	return FALSE;
}








BOOL MapClass::MoneyItemDrop(int money, int x, int y)
{
	int count;
	int counttot = 0;

	BYTE attr = this->GetAttr(x, y);

	if ( (attr&4) == 4 || (attr&8) == 8 )
	{
		return FALSE;
	}

	count = this->m_ItemCount;

	while ( true )
	{
		if ( this->m_cItem[count].IsItem() == FALSE )
		{
			this->m_cItem[count].m_Type = ITEMGET(14,15);
			this->m_cItem[count].m_SellMoney = money;
			this->m_cItem[count].m_BuyMoney = money;
			this->m_cItem[count].px = x;
			this->m_cItem[count].py = y;
			this->m_cItem[count].live = 1;
			this->m_cItem[count].Give = 0;
			this->m_cItem[count].m_State = 1;
			this->m_cItem[count].m_Time = GetTickCount() + ::gZenDurationTime * 1000;
			this->m_cItem[count].m_LootTime = 0;
			this->m_ItemCount++;

			if ( this->m_ItemCount > MAX_MAPITEM-1 )
			{
				this->m_ItemCount = 0;
			}

			return TRUE;
		}

		count++;

		if ( count > MAX_MAPITEM-1 )
		{
			count = 0;
		}

		counttot++;

		if ( counttot > MAX_MAPITEM-1 )
		{
			return FALSE;
		}
	}

	return FALSE;
}





BOOL MapClass::ItemGive(int aIndex, int item_num, bool bFailNotSend)
{
	if ( ((item_num<0)?FALSE:(item_num>MAX_MAPITEM-1)?FALSE:TRUE) == FALSE ) 
	{
		LogAdd(lMsg.Get(MSGGET(1, 206)), __FILE__, __LINE__);
		return FALSE;
	}

	if ( gObj[aIndex].MapNumber != this->thisMapNumber )
	{
		LogAdd(lMsg.Get(MSGGET(1, 207)), __FILE__, __LINE__, gObj[aIndex].MapNumber, gObj[aIndex].Name);
		return FALSE;
	}

	if ( this->m_cItem[item_num].IsItem() == FALSE )
	{
		LogAdd(lMsg.Get(MSGGET(1, 208)), __FILE__, __LINE__,gObj[aIndex].Name);	// Apply Deathway Fix herw
		return FALSE;
	}

	if ( this->m_cItem[item_num].Give == true )
	{
		return FALSE;
	}

	if ( this->m_cItem[item_num].live == false )
	{
		return FALSE;
	}

	int disx = this->m_cItem[item_num].px - gObj[aIndex].X;
	int disy = this->m_cItem[item_num].py - gObj[aIndex].Y;

	if ( disx > 2 || disx < -2 )
	{
		return FALSE;
	}

	if ( disy > 2 || disy < -2 )
	{
		return FALSE;
	}

	int lootresult = 1;

	if ( gLootingTime > 0 )
	{
		if ( this->m_cItem[item_num].m_UserIndex != -1 )
		{
			if ( GetTickCount() < this->m_cItem[item_num].m_LootTime )
			{
				if ( aIndex != this->m_cItem[item_num].m_UserIndex )
				{
					lootresult = 0;

					if ( this->m_cItem[item_num].m_QuestItem == false )
					{
						if ( gObj[aIndex].PartyNumber >= 0 )
						{
							if ( gObj[aIndex].PartyNumber == gObj[this->m_cItem[item_num].m_UserIndex].PartyNumber )
							{
								if ( BC_MAP_RANGE(gObj[aIndex].MapNumber) != FALSE )
								{
									if ( this->m_cItem[item_num].m_Type == ITEMGET(12,15) ||( this->m_cItem[item_num].m_Type == ITEMGET(13,19) && ((this->m_cItem[item_num].m_Level < 0)?FALSE:(this->m_cItem[item_num].m_Level > 2)?FALSE:TRUE) != FALSE ) )
									{
										lootresult = 0;
									}
									else
									{
										lootresult = 1;
									}
								}
								else
								{
									lootresult = 1;
								}
							}
						}
					}
				}
			}
		}
	}


	if ( lootresult == 0 )
	{
		if ( bFailNotSend == false )
		{
			char szTemp[256];

			wsprintf(szTemp, lMsg.Get(MSGGET(4, 128)), gObj[aIndex].Name );
			::GCServerMsgStringSend(szTemp, aIndex, 1);
			
		}

		return FALSE;
	}
	else
	{
		this->m_cItem[item_num].m_State = 8;
		this->m_cItem[item_num].Give = true;
		this->m_cItem[item_num].live = false;

		return TRUE;
	}
}





void MapClass::StateSetDestroy()
{
	int CurTime = GetTickCount();

	for (int n=0;n<MAX_MAPITEM;n++)
	{
		if ( this->m_cItem[n].IsItem() != FALSE )
		{
			if ( this->m_cItem[n].m_State == 1 )
			{
				this->m_cItem[n].m_State = 2;
			}
			else if ( this->m_cItem[n].m_State == 8 )
			{
				this->m_cItem[n].Clear();
				this->m_cItem[n].live = 0;
				this->m_cItem[n].Give = 1;
			}
		}

		if ( this->m_cItem[n].IsItem() != FALSE )
		{
			if ( this->m_cItem[n].m_State != 8 )
			{
				if ( CurTime > this->m_cItem[n].m_Time )
				{
					if (this->m_cItem[n].m_Number != 0)
					{
						//LogAddTD(lMsg.Get(MSGGET(4, 76)), this->m_cItem[n].GetName(), this->m_cItem[n].m_Type, this->m_cItem[n].m_Level, this->m_cItem[n].m_Special[0] , this->m_cItem[n].m_Number);
					}
					this->m_cItem[n].m_State = 8;
				}
			}
		}
	}
}





BOOL MapClass::AttrLoad(char* filename)
{
	FILE * fp;
	unsigned char head;

	fp = fopen(filename,"rb");

	if ( fp == NULL )
	{
		return false;
	}
	
	fread(&head, sizeof(head), 1, fp);
	fread(&this->m_width, sizeof(char), 1, fp);
	fread(&this->m_height, sizeof(char), 1, fp);

	if ( this->m_width > 255 || this->m_height > 255 )
	{
		MsgBox(lMsg.Get( MSGGET(2, 51)));
		fclose(fp);
		return false;
	}

	if ( this->m_attrbuf != NULL )
	{
		GlobalFree(this->m_attrbuf);
		this->m_attrbuf = NULL;
	}

	this->m_attrbuf = (UCHAR*)GlobalAlloc(GPTR, 65536);
	fread(this->m_attrbuf, 256, 256, fp);
	fclose(fp);
	this->path->SetMapDimensions( 256, 256, (UCHAR *)this->m_attrbuf);
	return true;
}





BYTE MapClass::GetAttr(int x, int y )
{
	__try
	{

	if (this->m_attrbuf == NULL)
	{
		return 4;
	}

	if (this->thisMapNumber > MAX_MAP_NUMBER)
	{
		return 4;
	}

	if ( x < 0 )
	{
		return 4;
	}

	if ( y < 0 )
	{
		return 4;
	}

	if ( x > this->m_width -1 )
	{
		return 4;
	}

	if ( y > this->m_height -1 )
	{
		return 4;
	}

	return this->m_attrbuf[y*256 + x];

	}__except( EXCEPTION_ACCESS_VIOLATION == GetExceptionCode() )
	{
		return 4;
	}
}




BOOL MapClass::GetStandAttr(int x, int y)
{
	__try
	{

	if (this->m_attrbuf == NULL)
	{
		return FALSE;
	}

	if (this->thisMapNumber > MAX_MAP_NUMBER)
	{
		return FALSE;
	}

	if ( x > (this->m_width-1) )
	{
		return FALSE;
	}

	if ( y > (this->m_height-1) )
	{
		return FALSE;
	}

	BYTE attr = this->m_attrbuf[(y * 256 + x)];

	if ( (attr&2) == 2 )
	{
		return FALSE;
	}

	if ( (attr&4) == 4 )
	{
		return FALSE;
	}

	if ( (attr&8) == 8 )
	{
		return FALSE;
	}

	return TRUE;

	}__except( EXCEPTION_ACCESS_VIOLATION == GetExceptionCode() )
	{
		return FALSE;
	}
}






void MapClass::SearchStandAttr(short& x, short& y)
{
	int scount = 10;
	int tx;
	int ty;
	int tindex;
	int subx = 1;

	tx = x;
	ty = y;
	tindex = y * 256;

	if ( (tx-10) < 0 )
	{
		subx = 1;
	}

	if ( (tx+10) > 255 )
	{
		subx = -1;
	}

	while ( scount-- != 0 )
	{
		if ( ((this->m_attrbuf[tindex + tx]&4)!= 4) && ((this->m_attrbuf[tindex + tx]&2)!= 2) && ((this->m_attrbuf[tindex + tx]&8)!= 8) )
		{
			x = tx;
			y = ty;
			break;
		}

		tx += subx;
	}
}




void MapClass::SetStandAttr(int x, int y)
{
	if ( x > this->m_width-1 )
	{
		return;
	}

	if ( y > this->m_height-1 )
	{
		return;
	}

	this->m_attrbuf[y*256+x] |= 2;
}





void MapClass::ClearStandAttr(int x, int y)
{
	if ( ((x<0)?FALSE:(x>(this->m_width-1))?FALSE:TRUE) == FALSE )
	{
		return;
	}

	if ( ((y<0)?FALSE:(y>(this->m_height-1))?FALSE:TRUE) == FALSE )
	{
		return;
	}

	if ( (this->m_attrbuf[y*256+x]&2)==2 )
	{
		this->m_attrbuf[y*256+x] &= 0xFD;
	}
}






bool MapClass::PathFinding2(int sx, int sy, int tx, int ty, PATH_t* a)
{
	bool Success = this->path->FindPath(sx, sy, tx, ty, true);

	if ( Success == false )
	{
		Success = this->path->FindPath(sx, sy, tx, ty, false);
	}

	if ( Success != false )
	{
		int path = this->path->GetPath();

		if ( path > 1 )
		{
			a->PathNum = path;
			BYTE * px = this->path->GetPathX();
			BYTE * py = this->path->GetPathY();

			for (int n=0;n<a->PathNum;n++)
			{
				a->PathX[n] = px[n];
				a->PathY[n] = py[n];
			}

			a->CurrentPath = 0;
			return 1;
		}

	}

	return false;
}



int TERRAIN_INDEX_REPEAT(int x, int y);




BOOL MapClass::CheckWall(int sx1, int sy1, int sx2, int sy2)
{
	__try
	{

	int Index = TERRAIN_INDEX_REPEAT(sx1,sy1);
	int nx1;
	int ny1;
	int d1;
	int d2;
	int len1;
	int len2;
	int px1 = sx2 - sx1;
	int py1 = sy2 - sy1;

	if ( px1 < 0 )
	{
		px1 = -px1;
		nx1 = -1;
	}
	else
	{
		nx1 = 1;
	}

	if ( py1 < 0 )
	{
		py1 = -py1;
		ny1 = -256;
	}
	else
	{
		ny1 = 256;
	}

	if ( px1 > py1 )
	{
		len1 = px1;
		len2= py1;
		d1 = ny1;
		d2 = nx1;
	}
	else
	{
		len1 = py1;
		len2 = px1;
		d1 = nx1;
		d2 = ny1;
	}

	int error = 0;
	int count = 0;

	do
	{
		if ( (this->m_attrbuf[Index]&4) == 4 )
		{
			return FALSE;
		}

		error += len2;

		if ( error > len1/2 )
		{
			Index += d1;
			error -= len1;
		}
		
		Index += d2;
		count++;	
	}
	while ( count <= len1 );

	return TRUE;

	}__except( EXCEPTION_ACCESS_VIOLATION == GetExceptionCode() )
	{
		return FALSE;
	}
}




int TERRAIN_INDEX_REPEAT(int x, int y)
{
	return (BYTE)y * 256 + (BYTE)x;
}





bool MapClass::PathFinding3(int sx, int sy, int tx, int ty, PATH_t* a)
{
	bool Success = this->path->FindPath2(sx, sy, tx, ty, TRUE);

	if ( Success == false )
	{
		Success = this->path->FindPath2(sx, sy, tx, ty, FALSE);
	}

	if ( Success != false )
	{
		int lc3 = this->path->GetPath();

		if ( lc3 > 1 )
		{
			a->PathNum = lc3;
			BYTE * px = this->path->GetPathX();
			BYTE * py = this->path->GetPathY();

			for (int n=0;n<a->PathNum;n++)
			{
				a->PathX[n] = px[n];
				a->PathY[n] = py[n];
			}

			a->CurrentPath = 0;
			return 1;
		}

	}

	return false;
}


bool MapClass::PathFinding4(int sx, int sy, int tx, int ty, PATH_t *a)
{
	bool Success = this->path->FindPath3(sx, sy, tx, ty, true);

	if ( Success == false )
	{
		Success = this->path->FindPath3(sx, sy, tx, ty, false);
	}

	if ( Success != false )
	{
		int lc3 = this->path->GetPath();

		if ( lc3 > 1 )
		{
			a->PathNum = lc3;
			BYTE * px = this->path->GetPathX();
			BYTE * py = this->path->GetPathY();

			for (int n=0;n<a->PathNum;n++)
			{
				a->PathX[n] = px[n];
				a->PathY[n] = py[n];
			}

			a->CurrentPath = 0;
			return 1;
		}

	}

	return false;
}



BYTE MapClass::CheckWall2(int sx1, int sy1, int sx2, int sy2)
{
	bool catchError = true;
	__try
	{
		int Index = TERRAIN_INDEX_REPEAT(sx1,sy1);
		int nx1;
		int ny1;
		int d1;
		int d2;
		int len1;
		int len2;
		int px1 = sx2 - sx1;
		int py1 = sy2 - sy1;

		if ( px1 < 0 )
		{
			px1 = -px1;
			nx1 = -1;
		}
		else
		{
			nx1 = 1;
		}

		if ( py1 < 0 )
		{
			py1 = -py1;
			ny1 = -256;
		}
		else
		{
			ny1 = 256;
		}

		if ( px1 > py1 )
		{
			len1 = px1;
			len2= py1;
			d1 = ny1;
			d2 = nx1;
		}
		else
		{
			len1 = py1;
			len2 = px1;
			d1 = nx1;
			d2 = ny1;
		}

		int start = 0;
		BOOL error = FALSE;
		int count = 0;

		do
		{
			if ( start == 0 )
			{
				if ( (this->m_attrbuf[Index]&2) == 2 )
				{
					catchError = false;
					return 2;
				}
			}
			else
			{
				start = 1;
			}

			if ( (this->m_attrbuf[Index]&4) == 4 )
			{
				catchError = false;
				return 4;
			}

			error +=len2;

			if ( error > (len1/2) )
			{
				Index += d1;
				error -= len1;
			}

			Index += d2;
			count++;
		}
		while ( count <= len1 );
		
		catchError = false;
		return 1;

	}__except( EXCEPTION_ACCESS_VIOLATION == GetExceptionCode() )
	{
		if(catchError == true)
		{		
			LogAdd("error : %s %d", __FILE__, __LINE__ );			
		}

		return 1;
	}
	
}