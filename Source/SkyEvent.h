#include "stdafx.h"

#if (PACK_EDITION>=2)

#include "user.h"


/*
lvl 1

x	y
130	228
166	188


lvl 2

x	y
193	230
228	188


lvl 3

x	y
130	170
166	128


lvl 4

x	y
193	170
228	128


lvl 5

x	y
193	111
228	68
*/

#define MAX_SKY_EVENT_SUB_LEVEL			 10
#define MAX_SKY_EVENT_LEVEL				  5
#define MAX_SKY_EVENT_STAGES			  3
#define MAX_SKY_EVENT_STAGE_MOB_COUNT	255

void cSkyEvent__InsideTrigger(void * lpParam);

BOOL SKYEVENT_MAP_RANGE(int Map, BYTE X);

struct SkyEventMob
{
	short Number;
	short ID;
	short Pos;
};

struct SkyEventStruct
{
	short m_Index;
	short DBNumber;
	//char Name[11];
	short KillCount[MAX_SKY_EVENT_STAGES];
};

class cSkyEvent
{
public:
	void ReadINI(char * FilePath);
	void DropWinnerItem(int iLevel,int iUser);
	int GetWinner(BYTE iLevel);
	int GetPlayerSubIndex(LPOBJ lpObj, BYTE iLevel);
	int GetLevel(LPOBJ lpObj);
	int MonsterDie(LPOBJ lpObj);
	void ExitPlayers();
	void MsgStringSend(LPSTR  szMsg, BYTE type) ;
	void AddUser(BYTE iLevel,int iIndex);
	BOOL NpcTalk(LPOBJ lpNpc, LPOBJ lpObj);
	int CheckEnterLevel(int iIndex);
	void Teleport(LPOBJ lpObj, BYTE iLevel);
	void SetMonsters(BYTE iLevel,BYTE iStage);
	void ClearAllMonsters();
	void ClearMonsters(BYTE iLevel,BYTE iStage);
	void ReadMonsters(char * FilePath);
	void StartEvent();
	void SendUserExpInfo(BYTE iLevel);
	int CheckUsersAlive(BYTE iLevel);
	void ClearLevelData(BYTE iLevel);
	void RemoveUser(BYTE iLevel,int iUser);
	int ExtraExp[MAX_SKY_EVENT_LEVEL][MAX_SKY_EVENT_STAGES];

	BYTE CurrentStage;
	//BOOL ActiveLevel[MAX_SKY_EVENT_LEVEL];
	SkyEventStruct m_UserData[MAX_SKY_EVENT_LEVEL][MAX_SKY_EVENT_SUB_LEVEL];
	SkyEventMob m_MonsterData[MAX_SKY_EVENT_LEVEL][MAX_SKY_EVENT_STAGES][MAX_SKY_EVENT_STAGE_MOB_COUNT];

	BOOL Start;
	BOOL Enabled;
	BOOL NPCEnabled;
	BYTE StageMinutes[MAX_SKY_EVENT_STAGES];
	short MonsterStageCount[MAX_SKY_EVENT_LEVEL][MAX_SKY_EVENT_STAGES];
	short Level_Max[MAX_SKY_EVENT_LEVEL];
	short Level_Min[MAX_SKY_EVENT_LEVEL];
	short PlayerCount[MAX_SKY_EVENT_LEVEL];

	short ItemWinType[MAX_SKY_EVENT_LEVEL];
	short ItemWinIndex[MAX_SKY_EVENT_LEVEL];
	short ItemWinLevel[MAX_SKY_EVENT_LEVEL];
	short ItemWinDur[MAX_SKY_EVENT_LEVEL];
	short ItemWinLuck[MAX_SKY_EVENT_LEVEL];
	short ItemWinSkill[MAX_SKY_EVENT_LEVEL];
	short ItemWinOpt[MAX_SKY_EVENT_LEVEL];
	short ItemWinExc[MAX_SKY_EVENT_LEVEL];
};

extern cSkyEvent SkyEvent;

#endif