#include "stdafx.h"
#include "GameServer.h"
//#include "GameServerAuth.h"
#include "Shop.h"
#include "LogProc.h"
#include "..\include\readscript.h"
#include "GameMain.h"
#include "..\common\winutil.h"
#include "..\common\WzMemScript.h"
#include "PCShop.h"
// GS-N 0.99.60T 0x004EF0A0
//	GS-N	1.00.18	JPN	0x00518EA0	-	Completed

CShop ShopC[MAX_SHOP];


CShop::CShop()
{
	return;
}


CShop::~CShop()
{
	return;
}

void CShop::Init()
{
	this->SendItemDataLen = 0;
	this->ItemCount = 0;
	memset(this->ShopInventoryMap, 0, sizeof(this->ShopInventoryMap));
}

BOOL CShop::InsertItem(int type, int index, int level, int dur, int op1, int op2 ,int op3, int Exc)
{
	int itemp;
	int width;
	int height;
	int x;
	int y;
	int blank;

	blank = -1;
	itemp = type * MAX_SUBTYPE_ITEMS + index;
	
	if ( itemp < 0 )
	{
		MsgBox(lMsg.Get(MSGGET(2, 53)), __FILE__, __LINE__, type, index);
		return false;
	}
	
	ItemGetSize(itemp, width, height);

	if ( width < 0 || height < 0 )
	{
		// Error in getting item size in shop %s %d
		MsgBox(lMsg.Get(MSGGET(2, 54)), __FILE__, __LINE__);
		return FALSE;
	}

	for ( y=0;y<15;y++)
	{
		for ( x=0;x<8;x++)
		{
			if ( this->ShopInventoryMap[x + y*8] == 0 )
			{
				blank = this->InentoryMapCheck(x, y, width, height);

				if ( blank >= 0 )
				{
					goto skiploop;
				}
			}
		}
	}

	if ( blank < 0 )
	{
		MsgBox("error-L2 : %s %d", __FILE__, __LINE__);
		return FALSE;
	}

skiploop:

	this->m_item[blank].m_Level = level;

	if ( dur == 0 )
	{
		dur = ItemGetDurability(ITEMGET(type, index), level, 0, 0);
	}

	this->m_item[blank].m_Durability = dur;
	this->m_item[blank].Convert(itemp, op1, op2, op3, Exc, 0, 0, CURRENT_DB_VERSION);
	this->m_item[blank].Value();
	this->SendItemData[this->SendItemDataLen] = blank;
	this->SendItemDataLen++;
	ItemByteConvert((LPBYTE)&this->SendItemData[this->SendItemDataLen], this->m_item[blank]);
	this->SendItemDataLen += 7+5;
	this->ItemCount++;

	return TRUE;
}

int CShop::InentoryMapCheck(int sx, int sy, int width, int height)
{
	int x;
	int y;
	int blank = 0;

	if ( (sx+width) > 8 )
		return -1;

	if ( (sy+height) > 15 )
		return -1;

	for(y=0;y<height;y++)
	{
		for(x=0;x<width;x++)
		{
			if ( this->ShopInventoryMap[( sy + y) * 8 + (sx + x)] )
			{
				blank++;
				break;
			}
		}
	}

	if ( blank == 0 )
	{
		for(y=0;y<height;y++)
		{
			for(x=0;x<width;x++)
			{
				this->ShopInventoryMap[( sy + y) * 8 + (sx + x)] = 1;
			}
		}

		return (sx + sy*8);
	}

	return -1;
}

BOOL CShop::LoadShopItem(char* filename )
{
	int Token;
	int type;
	int index;
	int level;
	int dur;
	int op1;
	int op2;
	int op3;
	int exc;

	this->Init();

	SMDFile = fopen(filename, "r");

	if ( SMDFile == NULL )
	{
		MsgBox("Shop data load error %s", filename);
		return FALSE;
	}

	while ( true )
	{
		Token = GetToken();

		if ( Token == 2 )
		{
			break;
		}

		if ( Token == 1 )
		{
			type = TokenNumber;

			Token = GetToken();
			index = TokenNumber;

			Token = GetToken();
			level = TokenNumber;

			Token = GetToken();
			dur = TokenNumber;

			Token = GetToken();
			op1 = TokenNumber;

			Token = GetToken();
			op2 = TokenNumber;

			Token = GetToken();
			op3 = TokenNumber;

			Token = GetToken();
			exc = TokenNumber;

			if (this->InsertItem(type, index, level, dur, op1, op2, op3, exc) == FALSE )
			{
				MsgBox("error-L3 : %s %s %d", filename, __FILE__, __LINE__);
			}
		}
	}

	fclose(SMDFile);
	return true;
}


int MossItemType[5];
int MossItemIndex[5];
BOOL CShop::LoadMossShopItem(char* filename )
{
	int Token;
	int type;
	int index;
	int level;
	int dur;
	int op1;
	int op2;
	int op3;
	int exc;

	this->Init();

	SMDFile = fopen(filename, "r");

	if ( SMDFile == NULL )
	{
		MsgBox("Shop data load error %s", filename);
		return FALSE;
	}


	for(int i=0;i<5;i++)
	{
		MossItemType[i] = 13;
		MossItemIndex[i] = 71 + i;
		this->moss_ItemCount[i] = 0;
		if (this->InsertItem(MossItemType[i], MossItemIndex[i], 0, 0, 0, 0, 0, 0) == FALSE )
		{
			MsgBox("error-L3 : %s %s %d", filename, __FILE__, __LINE__);
		}
	}

	int counts=0;

	while ( true )
	{
		int iType = GetToken();
		int aIndex = counts;

		if ( counts < 5 )
		{
			while(true)
			{
				Token = GetToken();
				if ( strcmp("end", TokenString) == 0 )
				{
					break;
				}
				type = TokenNumber;

				Token = GetToken();
				index = TokenNumber;

				Token = GetToken();
				level = TokenNumber;

				Token = GetToken();
				dur = TokenNumber;

				Token = GetToken();
				op1 = TokenNumber;

				Token = GetToken();
				op2 = TokenNumber;

				Token = GetToken();
				op3 = TokenNumber;

				Token = GetToken();
				exc = TokenNumber;

				/*if(level > 0)
					level = rand()%level+1;
				if(op2 > 0)
					op2 = rand()%op2+1;
				if(exc > 0)
					exc = rand()%exc+1;*/

				this->moss_item[aIndex][this->moss_ItemCount[aIndex]].m_Level = level;

				if ( dur == 0 )
				{
					dur = ItemGetDurability(ITEMGET(type, index), level, 0, 0);
				}

				int itemp = type * MAX_SUBTYPE_ITEMS + index;

				this->moss_item[aIndex][this->moss_ItemCount[aIndex]].m_Durability = dur;
				this->moss_item[aIndex][this->moss_ItemCount[aIndex]].Convert(itemp, op1, op2, op3, exc, 0, 0, CURRENT_DB_VERSION);
				this->moss_item[aIndex][this->moss_ItemCount[aIndex]].Value();
				this->moss_ItemCount[aIndex]++;
			}
		}
		if((counts+1)>4)
			break;
		counts++;
	}

	fclose(SMDFile);
	return true;
}

BOOL ShopDataLoad()
{
#if (WL_PROTECT==1)  
	VM_START_WITHLEVEL(19)
	int MyCheckVar1;
	CHECK_PROTECTION(MyCheckVar1, 0x79325617)  
	if (MyCheckVar1 != 0x79325617)
	{	
		WLRegDisableCurrentKey(wlMarkStolenKey);
		WLRegRemoveCurrentKey();
	}
	VM_END
#endif
	for(int i = 0; i < MAX_SHOP; i++)
	{
		if(i != 21)
			ShopC[i].LoadShopItem(ReadConfig.ConnShopDataFiles[i]);
		
		if(i == 21)
			ShopC[i].LoadMossShopItem(ReadConfig.ConnShopDataFiles[i]);
		
	}
#if (PACK_EDITION>=2)
	PCShop.LoadShopIni("..\\SCFData\\PointsShops\\SCF_PCPoint.ini");
	PCShop.LoadShopItem("..\\SCFData\\PointsShops\\SCF_PCPoint.txt");
#endif
	LogAdd(lMsg.Get(MSGGET(1, 209)));

	return TRUE;
}