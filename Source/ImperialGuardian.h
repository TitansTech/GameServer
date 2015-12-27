#include "stdafx.h"
#include "user.h"

#define IMPERIAL_MARK_COUNT	5
#define IMPERIAL_MARK_RANGE(x) ( ((x) <0)?FALSE:((x) > IMPERIAL_MARK_COUNT-1)?FALSE:TRUE)

struct ImperialGuardianEventUser
{
    short PartyIndex;
	short PartyLeaderUserNumber;
	short PartyLeaderDBNumber;
	BYTE PartyCount;
	unsigned long PlayerExp;
};

struct PMSG_ATTSTATE_IMPERIALGUARDIAN
{
	PBMSG_HEAD h;	// C1:BF
	BYTE subcode;	// 3
	BYTE State;	// 4
	BYTE Type;	// 5
};

struct PMSG_TIMER_IMPERIALGUARDIAN
{
	PBMSG_HEAD h;	// C1:F7:04
	BYTE subcode;	// 3
	BYTE State;	// 4 // (1 = Students exceed; 2 = Time Left(Monster in South); 3 = no text)
	BYTE unk1;	// 5
	BYTE unk2;	// 5
	BYTE unk3;	// 5
	BYTE unk4;	// 5
	BYTE Time1;	// 5
	BYTE Time2;	// 5
	BYTE Time3;	// 5
	BYTE MonstersLeft;
};

struct PMSG_STATUS_IMPERIALGUARDIAN
{
	PBMSG_HEAD h;	// C1:F7:02
	BYTE subcode;	// 3 
	BYTE State;	// 4 (1 = Expected Admission Time: 1 Min After the Entry;3 = Quest Failed; 4 = Jhon Success 5 = Success)
	DWORD unk1;	// 5 
	BYTE unk2;	// 5 
	BYTE Minutes; //
};

struct PMSG_STATE_IMPERIALGUARDIAN
{
	PBMSG_HEAD h;	// C1:F7:06
	BYTE subcode;	// 3 
	BYTE State;	// 4 (0 = Fail; 1 = Item Lack of quest item; 2 = Success)
	BYTE Type;	// 5 	
	char Exp[7];
	//unsigned long Exp;
};

struct ImperialGuardianEventMob
{
	short Number;
	short ID;
	short Pos;
};

BOOL IMPERIALGUARDIAN_MAP_RANGE(int Map);

const BYTE ImperialGuardian_MaxTime = 10;
const BYTE ImperialGuardian_WaitTime = 1;

#define MAX_IG_EVENT_DAYS 7
#define MAX_IG_EVENT_STAGES 4
#define MAX_IG_EVENT_STAGE_MOB_COUNT 255

class cImperialGuardian
{
public:
	void ReadConfigs(char * FilePath);
	void StartEvent(LPOBJ lpObj);
	void SetMonsters(BYTE iStage);
	void ClearAllMonsters();
	void ReadMonsters(char * FilePath);
	void MonsterDiePoint(LPOBJ lpMon);
	void MonsterDieRemove(LPOBJ lpMon);
	BOOL MonsterAttack(LPOBJ lpMon);
	int CheckEnter(LPOBJ lpObj);
	BOOL gObjMoveGateCheck(LPOBJ lpObj, int Gate);
	void GateState(bool Open);
	void DataSendInside(LPBYTE pMsg, int size);
	void DataSend(LPBYTE pMsg, int size);
	BYTE IsPartyAlive();
	BOOL WarpInside();
	void WarpOutside();
	BYTE CheckOverlapPaperMark(int iIndex);
	void CheckLetterItem(LPOBJ lpObj);
	int GetEnterItemPosition(int iIndex);
	void Fail(int Status);
	void Success();
	void Timer(int Seconds, BYTE tState);
	void GCServerMsgStringSend(LPSTR szMsg, BYTE type);
	void SetMonsterDifficulty(int MobID);
	void SendRanking();
	//void ReleaseCastleDoor(int Gate);
	//void BlockCastleDoor(int Gate);
	BOOL DoorAttack(LPOBJ mObj);
	bool DoorBlocked[10];
	bool CanWalk(BYTE TX, BYTE TY);

	ImperialGuardianEventUser m_UserData;
	ImperialGuardianEventMob m_MonsterData[MAX_IG_EVENT_DAYS][MAX_IG_EVENT_STAGES][MAX_IG_EVENT_STAGE_MOB_COUNT];
	int MonsterStageCount[MAX_IG_EVENT_DAYS][MAX_IG_EVENT_STAGES];
	//int MonsterStageTotalCount[MAX_IG_EVENT_DAYS][MAX_IG_EVENT_STAGES];
	int MonsterStageKillCount[MAX_IG_EVENT_DAYS][MAX_IG_EVENT_STAGES];
	BOOL Enabled;
	BOOL Occuped;
	BOOL Start;
	BOOL WaitPeriod;
	BYTE ActiveDay;
	BYTE Stage;
	BYTE OnStage;
	BYTE KillDoor;
	short DifMaxLvl;
	float Difficulty;

private:
	CRITICAL_SECTION mobKill;
	BYTE SaveInDB;
};

struct PMSG_ANS_IGSCORE
{
	PBMSG_HEAD h;
	int ServerCode;
	char AccountID[10];
	char Name[10];
	BYTE Day;
};

extern cImperialGuardian g_ImperialGuardian;
void cImperialGuardian__InsideTrigger(void *  lpParam);
//extern int cImperialGuardian__Map[MAX_IG_EVENT_DAYS];