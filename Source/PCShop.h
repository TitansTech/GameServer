#include "stdafx.h"


#if (PACK_EDITION>=2)

#include "user.h"

#define MAX_ITEM_IN_SHOP 120
#define PCPOINTS_MAX_VALUE 30000

struct PMSG_PCSHOPITEMCOUNT
{
	PWMSG_HEAD h;	// C2:31
	//BYTE Type;	// 4
	BYTE HICount;	// 5
	BYTE LOCount;	// 5
};

struct PMSG_PCSHOPINFO
{
	PBMSG_HEAD h;	// C1:D0
	BYTE Type;	// 4
	BYTE unk;	// 5
};

struct PMSG_PCSHOPPOINTINFO
{
	PBMSG_HEAD h;	// C1:D0
	BYTE Type;		// 4
	WORD Points;
	WORD MaxPoints;
};


struct PCShopItemConfig
{	
	int Type;	
	int Index;
	int Skill;
	int Level;
	int Luck;
	int Opt;
	int Dur;
	int Exc;

	int NewOpt1;
	int NewOpt2;
	int NewOpt3;
	int NewOpt4;
	int NewOpt5;

	int Position;
	int Cost;
};

struct PMSG_PCSHOPITEMSINFO
{	
	BYTE Position;
	BYTE Index;	
	BYTE Opts;
	BYTE Dur;
	BYTE unk1;
	BYTE Exc;
	BYTE Type16;
	BYTE NewOpt1;
	BYTE NewOpt2;
	BYTE NewOpt3;
	BYTE NewOpt4;
	BYTE NewOpt5;
	BYTE unk2;
};

class CPCShop
{
public:
	void ShopLoad();
	void SendData(LPOBJ lpObj);
	void BuyItem(LPOBJ lpObj,BYTE Position);
	bool LoadShopIni(char* filename );
	bool LoadShopItem(char* filename );
	void SendPoints(LPOBJ lpObj);
	void MonsterDie(LPOBJ lpObj, LPOBJ lmObj);

private:
	int InventoryMapCheck(int type,int index);
	int SearchIndex(BYTE Position);

//Vars:
	BYTE MonsterAddPoints[MAX_MONSTER_TYPE];
	BYTE SendByte[4680];
	BYTE PC_Shop[0x78];
	int PCShopPacketSize;
	int ItemsCount;
	PCShopItemConfig ShopInventoryMap[MAX_ITEM_IN_SHOP];
	CItem m_item[MAX_ITEM_IN_SHOP];	// 80
};

extern CPCShop PCShop;

#endif