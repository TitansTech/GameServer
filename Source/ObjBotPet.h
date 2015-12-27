#include "stdafx.h"

#if (PACK_EDITION>=3)

#define MAX_BOTS	50

struct bodyItems
{
	int num;
	int level;
	int opt;
	bool Enabled;
};

struct botStruct
{
	int Class;
	int OnlyVip;
	int VipCost;
	int MaxLife;
	int MaxMana;
	int BotLifes;
	BYTE Version;
	bool Enabled;
	bodyItems item[9];
	int skill[40];
	int skillcount;		
	int LvlUpDefense;
	int LvlUpPower;
	int LvlUpLife;
	int LvlUpMana;
	int LvlUpExp;
	int LvlUpMaxLevel;
};

struct SDHP_RECVBOT
{
	PBMSG_HEAD h;	// C1:FA:20
	BYTE SubCode;
	char Name[11];
	int Level;
	int Defense;
	int Power;
	int Lives1;
	int Lives2;
};

class ObjBotPet
{
public:
	bool Command(int aIndex,char * cmd, int opt1,char * Name);
	void OpenTrade(int index, int nindex);
	BYTE IsInTrade(int aIndex);
	void CancelTrade(int aIndex);
	void OKTrade(int aIndex);
	void Read(char * FilePath);
	void ReadCfg(char * FilePath);
	int Make(int PlayerIndex, int botNum, char * botName);
	void Get(int PlayerIndex);
	void CalcStats(int petIndex);
	void AddExp(int petIndex,unsigned long iAddExp);
	void ChangeSkill(int botNum, int Skill);
	void Close(int aIndex);
	void DieClose(int aIndex);
	void SendInfo(int aIndex);
	void gProcessAutoRecuperation(LPOBJ lpObj);
	BOOL BotDecreaseArrow(LPOBJ lpObj);

private:
	BOOL Enabled;
	botStruct bot[MAX_BOTS];
	BOOL BotTradeItemInventoryPutTest(int aIndex);
	BOOL BotTradeitemInventoryPut(int aIndex);
	int gObjMagicAdd(LPOBJ lpObj, BYTE Type, BYTE Index, BYTE Level, WORD & SkillNumber);
	int gObjMagicAddCheck(LPOBJ lpObj, BYTE Type, BYTE Index, BYTE Level, WORD & SkillNumber);

	int RelifeBot1;
	int RelifeBot2;
};

extern ObjBotPet botPet;

#endif