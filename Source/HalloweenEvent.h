#ifndef HALLOWEENDAY_H
#define	HALLOWEENDAY_H

#if _MSC_VER > 1000
#pragma once
#endif

#include "stdafx.h"

#if (PACK_EDITION>=2)

class cHalloweenEvent
{
public:
	void Init(char * FilePath);
	void StartEvent();
	void ViewPort();
	void WarpPlayersRandomLocation();
	BOOL Enabled;
	bool Start;
	bool CheckMap(BYTE Map);
	int Minutes;
	BYTE WarpRandom;
	//BYTE Map;
	BYTE Maps[MAX_MAP_NUMBER];
};
void cHalloweenEvent__InsideTrigger(void * lpParam);
extern cHalloweenEvent HalloweenEvent;

#endif
#endif