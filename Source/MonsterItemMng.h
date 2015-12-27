#ifndef MONSTERITEMMNG_H
#define MONSTERITEMMNG_H


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\common\zzzitem.h"
#include "user.h"



#define MAX_ITEM_IN_MONSTER 1000
#define MAX_LEVEL_MONSTER 150

typedef struct DropRate
{
	BYTE Type;
	BYTE Index;
	BYTE Level;
	BYTE Opt;
	BYTE Luck;
	BYTE Skill;
	BYTE Duration;
	BYTE ExcOptNum;
	WORD Rate;
	WORD VipRate;
	WORD PartyRate;
	BYTE NoTrade;
	short MobId;
	BYTE Minlvl;
	BYTE Maxlvl;
	short DropMap;
}DROPRATE;

#define MaxItemDropScript	1024

class CMonsterItemMng
{

public:

	CMonsterItemMng();
	virtual ~CMonsterItemMng();

	void Init();
	void Clear();
	BYTE InsertItem(int monsterlevel, int type, int index, int itemlevel,  int op1, int op2, int op3);
	CItem* GetItem(int monsterlevel);
	void gObjGiveItemSearch(int monsterlevel, int maxlevel);

//----------
	void ItemDropLoadScript(char * FilePath);
	void ShuffleDropScript(BYTE runs);
	BOOL SendDropConfigItem(LPOBJ lpObj, LPOBJ lpMonObj);

private:

	CItem* m_MonsterInvenItems[MAX_LEVEL_MONSTER];	// 4
	int m_iMonsterInvenItemCount[MAX_LEVEL_MONSTER];	//25C
	char MonsterName[50];	// 4B4

//-----

	DROPRATE DropRateConfig[MaxItemDropScript];
	int ItemHighestDropRate;
	int ItemDropRateCount;

};


extern CMonsterItemMng g_MonsterItemMng;

#endif