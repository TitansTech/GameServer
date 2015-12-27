
#include "user.h"

typedef struct RaklionMonsters
{
	short Number;
	short Pos;
	short ID;
	BYTE Stage;
}RaklionMobs;

class RaklionClass
{
public:
	RaklionClass()
	{
		InitializeCriticalSection(&this->m_critEventData);
	};

	/*~RaklionClass()
	{
		DeleteCriticalSection(&this->m_critEventData);
	};*/

	void ReadINI(char * FilePath);
	void EventStart();
	BOOL WarpCheck(int PlayerID,int Map, int TargetMap);
	void MonsterDiePoint(LPOBJ lpObj, LPOBJ lmObj);
	BOOL MonsterDieRemove(LPOBJ lpObj, LPOBJ lmObj);
	void ReadMonsters(LPSTR FileName);
	void SetMonsters(int Stage);
	void ClearAllMonsters();
	void ClearStageMonsters(int Stage);
	void ExitPlayer();
	int PlayersCount();
	
//Vars:
	BYTE Start;
	BYTE Enabled;
	BYTE Stage;
	BYTE EnableWarp;
	short MobCount;
	short EggCount;
	BYTE BossCount;
	BYTE BossKills;
	short EggKills;

	BYTE SelupanDropItems;

private:

	short GateIN;
	short GateOUT;
	short MobLoad;
	short MaxPlayers;
	RaklionMobs RalikionMob[256];

	CRITICAL_SECTION m_critEventData;
};

extern RaklionClass Raklion;