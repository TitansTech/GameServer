#include "stdafx.h"

#if (PACK_EDITION>=1)

struct EventMobSummer
{
	short Number;
	short ID;
	short Pos;
};

class cSummerEvent
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
	EventMobSummer Monster[256];
	short MonsterCount;
};

void cSummerEvent__InsideTrigger(void * lpParam);

extern cSummerEvent SummerEvent;

#endif