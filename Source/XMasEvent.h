#ifndef XMAS_H
#define XMAS_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "stdafx.h"

#if (PACK_EDITION>=2)

struct XMasEventMob
{
	short Number;
	short ID;
	short Pos;
};

class cXMasEvent
{
public:
	void Init(char * FilePath);
	void ReadMonsters(char * FilePath);
	void SetMonsters();
	void ClearMonsters();
	void StartEvent();
//Vars
	BYTE gXMasEvent;
	BYTE gMerryXMasNpcEvent;
	BYTE gHappyNewYearNpcEvent;
	BYTE g_bRibbonBoxEvent;

	BOOL Enabled;
	BOOL Start;

	BYTE AllowInvitation;
	BYTE AllowGifts;

	short Minutes;
	XMasEventMob Monster[256];
	short MonsterCount;
	short SantaTicketDropPer;
	short SnowManTicketDropPer;
	short GoblinTicketDropPer;

	short MaxTripsPerTicket;
	short ExtraExperience;
	short ExtraDropPercent;
	short FireworksDropRate;
	short SantaGirlRingDropRate;
	short RudolphPetDropRate;
	short SnownmanRingDropRate;

	BYTE BlessingBuffOnKill;
	BYTE BlessingBuffOnTalk;
	BYTE BlessingBuffMinutes;
	BYTE LittleBuffMinutes;

	BYTE BuffDefIncrease;
    BYTE BuffAttIncrease;
};

void cXMasEvent__InsideTrigger(void * lpParam);

extern cXMasEvent XMasEvent;

#endif
#endif
