#include "user.h"
#include "stdafx.h"

#if (PACK_EDITION>=3)
#if (CRYSTAL_EDITION == 1)

#define MAX_BOTVIPSHOP	10

struct BotVipShopBodyItems
{
	int num;
	BYTE level;
	BYTE opt;
	bool Enabled;
};

struct botVipShopStruct
{
	int Class;
	BYTE OnlyVip;
	char Name[11];
	BYTE Map;
	BYTE X;
	BYTE Y;
	BYTE Dir;
	bool Enabled;
	int index;
	BotVipShopBodyItems body[9];
};

class ObjBotVipShop
{
public:
	bool Enabled;
	void Read(char * FilePath);
	void MakeBot();
	BOOL TradeOpen(int index,int nindex);
	botVipShopStruct bot[MAX_BOTVIPSHOP];
private:
	int GetBotIndex(int aIndex);
};
extern ObjBotVipShop BotVipShop;

#endif
#endif