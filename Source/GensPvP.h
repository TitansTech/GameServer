
#include "stdafx.h"

struct GensRankItems
{	
	BYTE Type;
	WORD Index;
	BYTE Level;
	BYTE Opt;
	BYTE Luck;
	BYTE Skill;
	BYTE Dur;
	BYTE Exc;
	BYTE Count;
};

struct GensLevel
{
	int MinLevel;
	int MaxLevel;
	int ContKill;
	int ContKilled;
};

class GensPvP
{
public:
	void LoadCfg(char * FilePath);
	void LoadOtherCfg(char* filename );
	void ProtocolCore(int aIndex,char * lpMsg);
	bool KillPlayer(int aIndex, int tIndex);
	void Ranking(int aIndex);
//---------------------------------
	GensLevel KillLess[5];
	GensLevel KillMore[5];
	BYTE KillLessCount;
	BYTE KillMoreCount;
	GensRankItems rItem[8];

	BYTE Enabled;
	short MinLevel;
	short MinReset;
	BYTE AllowJoinInParty;
	BYTE AllowJoinInGuild;
	BYTE AllowJoinIfGuildMemberIsInOtherGens;
	BYTE AllowJoinInGuildAlliance;
	BYTE AllowJoinIfLeftLessThan3Days;
	BYTE JoinGuildIntoGensWhenYouAreGM;
	BYTE AllowLeaveGM;
	BYTE AllowPartyWithOtherGens;
	BYTE AllowDuelWithSameGensFam;
	BYTE AllowWarWithSameGensFam;
	BYTE AllowGuildAllianceWithOtherGensFam;
	BYTE WinPointsIfRivalDoesntHavePoints;

private:
	void JoinGens(int aIndex);
	void LeaveGens(int aIndex);
	void Class(int aIndex);
	bool CheckInvFree(int aIndex);
	void RewardRank(int aIndex);
};

extern GensPvP Gens;