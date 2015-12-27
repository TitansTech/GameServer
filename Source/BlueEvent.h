#include "stdafx.h"

#if (PACK_EDITION>=1)

struct EventMob
{
	short Number;
	short ID;
	short Pos;
};

class cBlueEvent
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
	int Minutes;
	EventMob Monster[256];
	short MonsterCount;
	short ExtraItemDropPer;
};

void cBlueEvent__InsideTrigger(void * lpParam);

extern cBlueEvent BlueEvent;

#endif