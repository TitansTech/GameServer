#include "stdafx.h"

#if !defined(AFX_SWAMPEVENT_H__F6667979_5AD8_4879_AA04_5C25C47DFADE__INCLUDED_)
#define AFX_SWAMPEVENT_H__F6667979_5AD8_4879_AA04_5C25C47DFADE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#if (PACK_EDITION>=3)
#if (GS_CASTLE==1)
//#include "user.h"
#define SWAMP_MOBSTOKILLCOUNT		255
#define SWAMP_ROOMS					4
#define SWAMP_MAXGROUPS				255
#define SWAMP_STANBYSECONDS			30

struct SwampRoomSettings
{
	BYTE Groups;
	short BossID;
	char Territory[20];
	BYTE Active;
};

struct SWAMP_EVENTMOBDATA
{
	short Number;
	BYTE Room;
	BYTE Group;
	BYTE XF;
	BYTE YF;
	BYTE XT;
	BYTE YT;
	BYTE Count;
};

class cSwampEvent
{
public:
	cSwampEvent()
	{
		InitializeCriticalSection(&this->m_critEventData);
	};

	/*~cSwampEvent()
	{
		DeleteCriticalSection(&this->m_critEventData);
	};*/

	void ReadConfigs(char * FilePath);
	void ReadMonsters(char * FilePath);
	void SetMonsters(int iAssultType, int iGroup);
	void ClearAllMonsters();
	void MonsterDie(LPOBJ lpMon, int pIndex);
	void StartEvent();
	void GCServerMsgStringSend(LPSTR szMsg, BYTE type);
	void Timer(int Seconds, BYTE State);
	bool GateMove(int aIndex, int Gate);
	void Clean();
	void WarpOutside();
	void SendMessageToAllGS(char * Msg);

//Vars:
	BOOL Enabled;
	BOOL Start;

	BYTE OnlyMastersEnter;
	BYTE SwampMapEnterOnlyWhenStarted;
	short SwampIfWonKeepOpenMapMinutes;

	BYTE MedusaDropItemCount;

	BYTE CurrentRoom;
	BYTE CurrentGroup;
	BYTE MobsStageGroupCount;
	BYTE MobsKilled;

	short SwampWarpGate;
	short SwampGateRangeStart;
	short SwampGateRangeEnd;

	BYTE GroupTimer[SWAMP_MAXGROUPS];
	SwampRoomSettings pRoom[SWAMP_ROOMS];

//private:
	void DataSendInside(LPBYTE pMsg, int size);

	std::map<int, std::vector<SWAMP_EVENTMOBDATA> > m_swampMonsterInfo;
	CRITICAL_SECTION m_critEventData;
};

void cSwampEvent__InsideTrigger(void * lpParam);
#else
class cSwampEvent
{
public:
	void ReadConfigs(char * FilePath);
	void SendMessageToAllGS(char * Msg);
	void Clean();

//Vars:
	BOOL Enabled;
	BOOL Start;

	BYTE OnlyMastersEnter;
	BYTE SwampMapEnterOnlyWhenStarted;
	short SwampIfWonKeepOpenMapMinutes;

	BYTE MedusaDropItemCount;

	BYTE CurrentRoom;
	BYTE CurrentGroup;
	BYTE MobsStageGroupCount;
	BYTE MobsKilled;

	short SwampWarpGate;
	short SwampGateRangeStart;
	short SwampGateRangeEnd;
};
#endif	//END IF GSCS

extern cSwampEvent g_Swamp;

#endif
#endif // !defined(AFX_SWAMPEVENT_H__F6667979_5AD8_4879_AA04_5C25C47DFADE__INCLUDED_)

