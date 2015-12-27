
#include "user.h"

class cMossShop
{
public:
	void Init(char * FilePath);
	void BuyItem(LPOBJ lpObj, BYTE Pos);
	void StartEvent();
	void SetMonster();
	void LoadMonster();
	void ClearMonster();

	short MobID;
	short MobPos;
	short MobNr;

	BOOL Enabled;
	BOOL Start;
	short Minutes;

private:
	short LevelReq;
	short ResetsReq;
	BOOL ResetsDecEnabled;
	BOOL OnlyForMarried;
	int ZenReq;

	short ItemEnabledReq;
	short ItemTypeReq;
	short ItemIndexReq;
	short ItemCountReq;
	short ItemLevelReq;
	short ItemOptionReq;
	short ItemLuckReq;
	short ItemSkillReq;
};

void MossShop__InsideTrigger(void * lpParam);
extern cMossShop MossShop;
