#include "user.h"
#include "stdafx.h"

#if (PACK_EDITION>=3)

#define MAX_BOTWARPER		10

struct BotWarperBodyItems
{
	int num;
	BYTE level;
	BYTE opt;
	bool Enabled;
};

struct botWarperStruct
{
	int Class;
	BYTE OnlyVip;
	int Zen;
	int MinLevel;
	int MinReset;
	int VipMoney;
	char Name[11];
	BYTE Map;
	BYTE X;
	BYTE Y;
	BYTE Dir;
	bool Enabled;
	int index;
	BotWarperBodyItems body[9];

	BYTE Warp_Map;
	BYTE Warp_X;
	BYTE Warp_Y;
};

class ObjBotWarper
{
public:
	bool Enabled;
	void Read(char * FilePath);
	void MakeBot();
	BOOL TradeOpen(int index,int nindex);
	botWarperStruct bot[MAX_BOTWARPER];
private:
	int GetBotIndex(int aIndex);
};
extern ObjBotWarper BotWarper;

#endif