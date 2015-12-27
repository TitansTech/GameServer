#include "stdafx.h"


#if (PACK_EDITION>=3)

struct BossEventMob
{
	short Number;
	short ID;
	short Pos;
	BYTE Boss;
};

class cBossAttack
{
public:
	void Init(char * FilePath);
	void ReadMonsters(char * FilePath);
	void SetMonsters();
	void ClearMonsters();
	void StartEvent();
//Vars
	BOOL Enabled;
	BOOL Start;
	short Minutes;
	BossEventMob Monster[256];
	short BossID;
	short BossDie;
	short MonsterCount;
};

void cBossAttack__InsideTrigger(void * lpParam);

extern cBossAttack BossAttack;

#endif