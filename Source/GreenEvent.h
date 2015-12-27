#include "stdafx.h"

#if (PACK_EDITION>=1)

class cGreenEvent
{
public:
	void Init(char * FilePath);
	void StartEvent();
	BOOL Enabled;
	BOOL Start;
	short Minutes;
	short ExcDropRate;
	short ExcItemDropSkill;
	short ExcItemDropLuck;
	short NormalItemDropSkill;
	short NormalItemDropLuck;
};
void cGreenEvent__InsideTrigger(void * lpParam);
extern cGreenEvent GreenEvent;

#endif