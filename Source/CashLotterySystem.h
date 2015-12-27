#ifndef CASHLOTTERYSYSTEM_H__
#define CASHLOTTERYSYSTEM_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "TRandomPoolMgr.h"
#include "..\common\zzzitem.h"

#define MAX_LOTTERY_TYPES			5
#define MAX_LOTTERY_ITEM_CATEGORY	20
#define MAX_LOTTERY_ITEM_COUNT		130


struct LOTTERY_ITEM_OPTION_RATE
{
	int iSkillOptionRate;	// 0
	int iLuckOptionRate;	// 4
	int iAddOptionRate;	// 8
	int iExOptionRate;	// C
};


struct LOTTERY_ITEMLIST
{
	WORD wItemIndex;	// 0
	BYTE btItemType;	// 2
	BYTE btItemLevelMin;	// 3
	BYTE btItemLevelMax;	// 4
	BYTE btSkillOption;	// 5
	BYTE btLuckOption;	// 6
	BYTE btAddOption;	// 7
	BYTE btExOption;	// 8
};



class CCashLotterySystem
{

public:

	CCashLotterySystem();
	virtual ~CCashLotterySystem();

	void Initialize();
	void Load(LPSTR pchFilename);
	void SetitemDropRate(int iCategory, int iLotteryTYPE, int iDropRate, int iSkillOptionRate, int iLuckOptionRate, int iAddOptionRate, int iExOptionRate);
	BOOL InsertItem(int iItemCategory, int iItemType, int iItemIndex,int iItemLevelMin, int iItemLevelMax, int iSkillOption, int iLuckOption, int iAddOption, int iExOption);
	int GetItem(CItem* lpItem, BYTE LotteryTYPE);
	void GetRequireInvenSize(int* piHeight, int* piWidth);

private:

	TRandomPoolMgr CategoryRandomPool[MAX_LOTTERY_TYPES];	// 4
	TRandomPoolMgr AddOptionRandomPool[MAX_LOTTERY_TYPES];	// 1C
	TRandomPoolMgr ExOptionRandomPool[MAX_LOTTERY_TYPES];	// 34
	BYTE iItemMaxHeight;	// 4C
	BYTE iItemMaxWidth;	// 50
	short LotteryItemListCount[MAX_LOTTERY_ITEM_CATEGORY];	// 54
	LOTTERY_ITEM_OPTION_RATE LotteryItemOptionRate[MAX_LOTTERY_ITEM_CATEGORY];	// A4
	LOTTERY_ITEMLIST LotteryItemList[MAX_LOTTERY_ITEM_CATEGORY][MAX_LOTTERY_ITEM_COUNT];	// 1E4
};


extern CCashLotterySystem g_CashLotterySystem;

#endif