#include "stdafx.h"

#if (PACK_EDITION>=1)

class cHappyHour
{
public:
	void Init(char * FilePath);
	void StartEvent();
	BOOL Enabled;
	BOOL Start;
	short ExtraExp;
	short ExtraDrop;
	short Minutes;
};
void cHappyHour__InsideTrigger(void * lpParam);
extern cHappyHour HappyHour;

#endif