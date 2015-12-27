#include "user.h"
#include "stdafx.h"

#if (PACK_EDITION>=3)

#define MAX_BOTBUFFER	30
#define MAX_BOTBUFFERSKILLS	5

struct BotBufferBodyItems
{
	int num;
	int level;
	int opt;
	bool Enabled;
};

struct botBufferSkill
{
	WORD skill;
	int time;
};

struct botBufferStruct
{
	int Class;
	int OnlyVip;
	char Name[11];
	BYTE Map;
	BYTE X;
	BYTE Y;
	BYTE Dir;
	bool Enabled;
	int index;
	WORD MaxLevel;
	BotBufferBodyItems body[9];
	botBufferSkill skill[MAX_BOTBUFFERSKILLS];
	BYTE SkillCount;
};

class ObjBotBuffer
{
public:
	bool Enabled;
	void Read(char * FilePath);
	void MakeBot();
	BOOL TradeOpen(int index,int nindex);
	botBufferStruct bot[MAX_BOTBUFFER];
private:
	int GetBotIndex(int aIndex);
};
extern ObjBotBuffer BotBuff;

#endif