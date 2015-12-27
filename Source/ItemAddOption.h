// ItemAddOption.h: interface for the CItemAddOption class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ITEMADDOPTION_H__96FEC421_39C3_40DD_9D47_EE1D86DD8A21__INCLUDED_)
#define AFX_ITEMADDOPTION_H__96FEC421_39C3_40DD_9D47_EE1D86DD8A21__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAX_ITEM_ADD_OPTION	100

#include "user.h"

#define ADD_OPTION_SPEED					1
#define ADD_OPTION_ATTACK_DAMAGE			2
#define ADD_OPTION_DEFENSE					3
#define ADD_OPTION_LIFE						4
#define ADD_OPTION_MANA						5
#define ADD_OPTION_EXPERIENCE_PERCENT		6
#define ADD_OPTION_DROP_RATE_PERCENT		7

#define ADD_OPTION_REMOVE_EXTRA_EXPERIENCE	8

#define ADD_OPTION_STRENGTH					9
#define ADD_OPTION_DEXTERITY				10
#define ADD_OPTION_VITALITY					11
#define ADD_OPTION_ENERGY					12
#define ADD_OPTION_LEADERSHIP				13

#define ADD_OPTION_ATTACKPOWER_PERCENT		51
#define ADD_OPTION_DMG_ABSORB_PERCENT		52

#define ADD_OPTION_EXPERIENCE_RATE			60
#define ADD_OPTION_DROP_RATE_1000_PERCENT	61

#define ADD_OPTION_INCREASE_DMG				64

#define ADD_OPTION_INCREASE_AG_PERCENT		66
#define ADD_OPTION_INCREASE_SD_PERCENT		67

typedef struct ITEMEFFECT
{
	WORD wOptionType;	// 0
	WORD wEffectType1;	// 2
	WORD wEffectType2;	// 4
	int iItemNumber;	// 8
	int iItemType;	// C
	int iItemIndex;	// 10
	int iEffectValue1;	// 14
	int iEffectValue2;	// 18
	int iEffectValidTime;	// 1C

} ITEMEFFECT, * LPITEMEFFECT;

class CItemAddOption
{

public:

	CItemAddOption();
	virtual ~CItemAddOption();

	void Load(PCHAR chFileName);
	void Initialize();
	LPITEMEFFECT GetCurrentEffectType(LPOBJ lpObj,WORD wOptionType);
	bool SetItemLastEffectStats(LPOBJ lpObj, WORD wOptionType);
	bool SetItemLastEffectLifeMana(LPOBJ lpObj, WORD wOptionType);
	bool SetItemLastEffectOther(LPOBJ lpObj, WORD wOptionType);
	void SendItemUse(LPOBJ lpObj, int iItemNumber, int iOptionType, int iEffectType, int iEffectValue, int iValidTime);
	bool SetItemEffect(LPOBJ lpObj, int iItemNumber, int iItemUseTime);
	bool ClearItemEffect(LPOBJ lpObj, int iEffectOption, WORD wOptionType);
	bool SearchItemEffectType(int iItemNumber, int* iEffectType1, int* iEffectType2);
	bool ApplyInventoryItemsStats(LPOBJ lpObj);
	bool ApplyInventoryItems(LPOBJ lpObj);
	LPITEMEFFECT _SearchItemEffect(int iItemNumber);
	int GetItemEffect(int Item);

//private:

	bool _SetItemEffect(LPOBJ lpObj, int iEffectType, int iEffectValue);
	void ItemShowEffect(LPOBJ lpObj,int Enabled, WORD wOptionType);
	bool _ClearItemEffect(LPOBJ lpObj, int iEffectOption, int iOptionType, int iEffectType, int iEffectValue);
	
//private:

	ITEMEFFECT m_ItemAddOption[MAX_ITEM_ADD_OPTION];
};

extern CItemAddOption g_ItemAddOption;

#endif // !defined(AFX_ITEMADDOPTION_H__96FEC421_39C3_40DD_9D47_EE1D86DD8A21__INCLUDED_)
