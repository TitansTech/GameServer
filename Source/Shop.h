#ifndef ___SHOP_H
#define ___SHOP_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\common\zzzitem.h"

#define MAX_ITEM_IN_SHOP 120

class CShop
{

public:

	CShop();
	virtual ~CShop();

	void Init();
	BOOL LoadShopItem(char* filename);
	BOOL LoadMossShopItem(char* filename );
	int InsertItem(int type, int index, int level, int dur, int op1,  int op2, int op3, int Exc);
	int InentoryMapCheck(int sx, int sy, int width,  int height);


public:

	BYTE ShopInventoryMap[MAX_ITEM_IN_SHOP];	// 4
	int ItemCount;	// 7C
	CItem m_item[MAX_ITEM_IN_SHOP];	// 80
	BYTE SendItemData[MAX_ITEM_IN_SHOP*(MAX_ITEM_INFO+1)];	// 5300
	int SendItemDataLen;	// 56C0

	CItem moss_item[5][MAX_ITEM_IN_SHOP];	// 80
	int moss_ItemCount[5];	// 7C
};

BOOL ShopDataLoad();
extern CShop ShopC[MAX_SHOP];

#endif