#if (PACK_EDITION>=2)

#include "user.h"

class cHitAndUp
{
public:
	void Init(char * FilePath);
	void StartEvent();
	int GetAddExtraPoint(BYTE Class);

	BOOL Enabled;
	BOOL Start;
	short Minutes;
	BYTE MGExtraPoint;
	BYTE DLExtraPoint;
	BYTE RFExtraPoint;
	BYTE FirstClassExtraPoint;
	BYTE SecondClassExtraPoint;
	BYTE ThirdClassExtraPoint;
};

void cHitAndUp__InsideTrigger(void * lpParam);

extern cHitAndUp HitAndUp;

#endif