#include "user.h"

#define MAX_ILLUSION_TEMPLE_LEVEL		6

#define ILLUSIONTEMPLE_TEAM_ORANGE		1
#define ILLUSIONTEMPLE_TEAM_BLUE		2

#define ILLUSIONTEMPLE_ENEMY_PKLEVEL	3			//5
#define	ILLUSIONTEMPLE_MAX_KILLPOINT	255
#define	ILLUSIONTEMPLE_MAX_PLAYERS		5

#define	IL_ORDER_OF_PROTECT				0xd2	//210
#define	IL_ORDER_OF_FREEZE				0xd3	//211
#define	IL_ORDER_OF_TRACKING			0xd4	//212
#define	IL_ORDER_OF_SHIELD				0xd5	//213

BOOL IT_MAP_RANGE(BYTE Map);

struct PMSG_ILLUSION_DIESTATUE
{
	PBMSG_HEAD h;	// C1:17
	BYTE NumberH;	// 4
	BYTE NumberL;	// 5
	WORD Skill;	// 7
	BYTE KillerNumberH;	// 8
	BYTE KillerNumberL;	// 9
};

struct PMSG_ILLUSION_RADAR {
	unsigned short	aIndex;
	unsigned char	unk;
	unsigned char	X;
	unsigned char	Y;
	unsigned char	unk2;
};

struct PMSG_ILLUSION_EVENTSTATE {
	PBMSG_HEAD2 h;
	BYTE TimerHI;
	BYTE TimerLO;
	short aHeroIndex;
	BYTE HeroX;
	BYTE HeroY;
	BYTE RedPoints;
	BYTE BluePoints;
	BYTE RadarColor;
	BYTE PlayersCount;
	PMSG_ILLUSION_RADAR Radar[ILLUSIONTEMPLE_MAX_PLAYERS*2];
};

struct PMSG_ILLUSION_EXP_DATA {
	char Name[10];
	BYTE ChangeUP;
	BYTE Color;

	BYTE Class;
	BYTE Tmp1;
	BYTE Tmp2;
	BYTE Tmp3;

	//BYTE unknown[2];
	int	Experience;
};

struct PMSG_ILLUSION_EXP {
	PBMSG_HEAD h;
	BYTE subcode;
	BYTE Points1;
	BYTE Points2;
	BYTE pCount;
};

//PMSG_ILLUSION_EXPERIENCEDATA ExData[ILLUSIONTEMPLE_MAX_PLAYERS*2];
	//unsigned char winPoints;
	//unsigned char losPoints;
	//unsigned char RedPoints;
	//unsigned char BluePoints;

struct PMSG_STATE_ILLUSIONTEMPLE
{
	PBMSG_HEAD h;	// C1:BF 1,2,3
	BYTE subcode;	// 4
	BYTE State;	// 5
	BYTE Type;	// 6
};

struct PMSG_KILLPOINT_ILLUSIONTEMPLE
{
	PBMSG_HEAD h;	// C1:BF 1,2,3
	BYTE subcode;	// 4
	BYTE Points;	// 5
};

struct PMSG_ILLUSION_STATUE_BREAK {
	PBMSG_HEAD2		h;		//1,2,3,4
	unsigned short	aIndex;	//5,6
	unsigned char	unk1;
	unsigned char	unk2;
	unsigned char	unk3;
	unsigned char	unk4;
	unsigned char	unk5;
	unsigned char	unk6;
	unsigned char	unk7;
	unsigned char	unk8;
	unsigned char	unk9;
	unsigned char	unk10;
};

struct PMSG_ILLUSION_ACTIVESKILL {
	PBMSG_HEAD2		h;		//1,2,3,4
	unsigned char	useTime;	//5
	unsigned char	SkillEx;
	unsigned char	SkillId;	//6,7
	unsigned char	unk1;
	unsigned short	aOwnerIndex;	//8,9
	unsigned short	aRecvrIndex;	//10,11
};

struct PMSG_ILLUSION_DEACTIVESKILL {
	PBMSG_HEAD2		h;
	unsigned char	SkillEx;
	unsigned char	SkillId;
	//unsigned char	unk1;
	unsigned short	aOwnerIndex;
};

struct ILLUSIONTEMPLE_PLAYER
{
	short Index;
	BYTE Team;
	BYTE kill_points;
	float total_score;
	unsigned long Exp_Current;
	LPSTR Name;
	int FernirTime;
};

typedef struct IllusionTemple_Room
{
	BOOL Start;

	BYTE TeamBlue_Score;
	BYTE TeamOrange_Score;
	BYTE TeamBlue_Count;
	BYTE TeamOrange_Count;

	ILLUSIONTEMPLE_PLAYER Player[ILLUSIONTEMPLE_MAX_PLAYERS*2];

	short Hero;
	BYTE Hero_X;
	BYTE Hero_Y;

	BYTE DroppedBall;

	short StatueNum;
	BYTE Statue_X;
	BYTE Statue_Y;
}ILLUSIONTEMPLE_ROOM;


static struct _ILLUSIONTEMPLE_LEVEL
{
	short iLOWER_BOUND;	// 0
	short iUPPER_BOUND;	// 4

} g_sttILLUSIONTEMPLE_LEVEL[MAX_ILLUSION_TEMPLE_LEVEL] =
{
	// Illusion Temple 1
	220, 270, 

	// Illusion Temple 2
	271, 320,

	// Illusion Temple 3
	321, 350,

	// Illusion Temple 4
	351, 380,

	// Illusion Temple 5
	381, MAX_CHAR_LEVEL,

	// Illusion Temple 6
	1, MAX_CHAR_LEVEL
};

class CIllusionTemple
{

public:

	CIllusionTemple();
	void Init( char * FilePath );
	void Start();
	void StatueTeleport(BYTE Level);
	void SelectHero(LPOBJ lpObj,int Map);
	void DataSendRoom(BYTE Level, LPBYTE pMsg, int size);
	void DataSendTeamRoom(BYTE Level, BYTE Team, LPBYTE pMsg, int size);
	void GCServerMsgStringSendToRoom(BYTE Level, LPSTR szMsg, BYTE type);
	void GCServerMsgStringSendToNoIllusion(LPSTR  szMsg, BYTE type);
	void Warp_WaitRoom(LPOBJ lpObj, BYTE Level);
	void Player_Check(BYTE Level);
	void StatueBreak(BYTE Level, short pIndex, BYTE y);
	void Warp_EventRoom(bool Start, BYTE Level);
	int ReqRemainTime();
	void CheckSync(BYTE Level);
	int Ranking_GetClass(int PlayerID);
	BYTE Ranking_TopTen(BYTE Level);
	void End_SetExp(BYTE Level);
	void Ranking_MSG(BYTE Level);
	void Player_Remove(BYTE Level,int Index);
	void Player_RemoveByID(int pIndex);
	int  Player_GetIndex(BYTE Level, int PlayerID);
	void SkillUse(int PlayerID, int TargetID, int Skill);
	//BOOL KillPoint (BYTE Level,int Team, BOOL Add , int Points);
	BYTE ReqPlayerNumberInside(LPOBJ lpObj);
	void ReqEnter(LPOBJ lpObj);
	void SendTimer(BYTE Level,int ctime);
	void SetState_Closed(BYTE Level, BOOL ShowLog);
	void GiveReward_Win(BYTE Level);
	void ReturnGift(LPOBJ lpObj,BYTE Level,WORD npc);
	void GiveExp(LPOBJ lpObj,BYTE Level,BYTE Type);
	void SetState_End(BYTE Level, bool bReturnTicket);
	void FenrirChange(LPOBJ lpObj, BYTE Level, int FenrirTimer);
	void RewardGiveItem(int aIndex, BYTE Num, BYTE Level);
	int PlayerDieRespawn(short iIndex, short& ox, short& oy);
	void PlayerDieKillPoint(short dieIndex, short killerIndex);
	void MonsterDieKillPoint(short killerIndex);
	BYTE KillPointReduce(short pIndex, BYTE ammount);
	int CheckTeamsSize(BYTE Level);
	void SetMonster(BYTE Level);
	void ClearMonster(BYTE Level);

	void SkillEffectStart(int aIndex, int aTargetIndex, unsigned char SkillId, unsigned char SkillLen);
	void SkillEffectEnd(int aIndex, unsigned char SkillId);

	float Time_Start;
	int Time_Current;
	int Time_Left;

	short Time_Inside_Current[MAX_ILLUSION_TEMPLE_LEVEL];
	short Time_Inside_Passed[MAX_ILLUSION_TEMPLE_LEVEL];
	BOOL Enabled;
	ILLUSIONTEMPLE_ROOM Room[MAX_ILLUSION_TEMPLE_LEVEL];

//Vars:
	bool Running;

	BYTE CheckTwoPlayerReq;
	short BoxItemDropRate;

	BYTE AutoWinScoreRequired;

	BYTE startSkillPoints;
	BYTE pointsPerMobKill;
	BYTE pointsPerPlayerKill;

	int ExpForBallDropped;
	BYTE ExpForKill;

	BYTE IllusionSyncHrMin;
	BYTE IllusionStartCountdown;
	BYTE IllusionCurrentCountdown;

	BYTE IllusionEventMinLength;

	short ExtraRankExp;
	float MobKillRankExp;
	float PlayerKillRankExp;
	float ScoreBallRankExp;
	float PickUpBallRankExp;
	float StatueBreakRankExp;

	BYTE IsExtraReward;
	BYTE IsExtraRewardMinPlayerReq;
	
	BYTE Reward_ItemType[4];
	short Reward_ItemIndex[4];
	BYTE Reward_Itemlvl[4];
	BYTE Reward_ItemOpt[4];
	BYTE Reward_ItemLuck[4];
	BYTE Reward_ItemSkill[4];
	BYTE Reward_ItemDur[4];
	BYTE Reward_ItemIsExc[4];

	BYTE IsFenrirChange;
	BYTE FenrirChangeMinPlayerReq;
	BYTE RedFenrirPercent;
	BYTE BlueFenrirPercent;
	BYTE BlackFenrirPercent;

	BYTE EventStart;
};

void CIllusionTemple__TimeInside(void * lpParam);
void CIllusionTemple__Time(void * lpParam);

extern CIllusionTemple IllusionTemple;