
#include "stdafx.h"

struct QInfo
{
	BYTE Num;
	int WordID[9];
};

struct PMSG_QUESTS5RECV
{
	PBMSG_HEAD h;	// C1:F6
	BYTE subcode;
};

struct PMSG_QUESTS5ACCEPTRECV
{
	PBMSG_HEAD h;	// C1:F6:0A
	BYTE subcode;
	WORD Num;
	WORD Quest;
	BYTE Unk;
};

struct PMSG_QUESTS5SEND
{
	PBMSG_HEAD h;	// C1:FA:1A
	BYTE subcode;	// 3
	BYTE QuestCount;	// 4
};


struct PMSG_QUESTS5NPCSEND
{
	PBMSG_HEAD h;	// C1:FA:1A
	BYTE subcode;	// 3
	BYTE Unk1;
	BYTE Unk2;
	BYTE QuestCount;	// 4
	//BYTE Unk3;
};

struct QUESTS5INFO
{
	WORD Num;
	WORD Quest;
};



struct PMSG_QUESTS5ACCEPTRECVs
{
	PBMSG_HEAD h;	// C1:F6:0A
	BYTE subcode;
	WORD Num;
	WORD Quest;
	BYTE Unk;
	BYTE Unk1;
	BYTE Unk2;
	BYTE Unk3;
};


struct PMSG_QUESTS5REQSEND
{
	PBMSG_HEAD h;	// C1:F6:0C
	BYTE subcode;
	WORD Num;
	WORD Quest;
	BYTE ReqCount;
	BYTE RewCount;
	BYTE Unk1;
};

struct QUESTS5REQINFO
{
	BYTE Type;
	BYTE Unk1[3];
	WORD MobNum;
	BYTE ToKill[4];
	WORD Killed;
	BYTE ItemIndex;
	BYTE iOpts;
	BYTE iDurability;
	BYTE iExcellent;
	BYTE iAncient;
	BYTE ItemType;
	BYTE iReinforcement;
	BYTE Unk5[5];
};

struct QUESTS6REQINFO
{
	WORD Type;
	BYTE Unk1[2];
	WORD MobNum;
	BYTE ToKill[4];
	WORD Killed;
	WORD Unk6;
	BYTE ItemIndex;
	BYTE iOpts;
	BYTE iDurability;
	BYTE iExcellent;
	BYTE iAncient;
	BYTE ItemType;
	BYTE iReinforcement;
	BYTE Unk5[5];
};

struct QUESTS5REWINFO
{
	BYTE Type;//1 Exp // 2 Zen // 4 Items //16 Contribucion
	BYTE Unk1[3];
	WORD ItemID;
	BYTE Ammount[4];
	BYTE ItemIndex;
	BYTE iOpts;
	BYTE iDurability;
	BYTE iExcellent;
	BYTE iAncient;
	BYTE ItemType;
	BYTE iReinforcement;
	BYTE Unk3[5];
};

struct QuestS5_Req
{
	int Num;
	int QuestIndex;
	WORD Type;
	DWORD Monster;
	WORD ItemIndex;
	BYTE iLevel;
	BYTE iOpt;
	BYTE iLuck;
	BYTE iSkill;
	BYTE iDur;
	BYTE iExc;
	BYTE iAnc;
	WORD KillCount;
	BYTE RequireClass[MAX_TYPE_PLAYER];
};

struct QuestS5_Rew
{
	int Num;
	int QuestIndex;
	BYTE Type;
	int iType;
	WORD iIndex;
	BYTE iLevel;
	BYTE iOpt;
	BYTE iLuck;
	BYTE iSkill;
	BYTE iDur;
	BYTE iExc;
	BYTE iAnc;
	BYTE iCount;
};

struct QuestS5_Info
{
	int Num;
	int QuestIndex;
	WORD NPCNum;
	char Name[50];
	WORD LevelMin;
	WORD LevelMax;
	WORD ReqCount;
	WORD RewCount;
	QuestS5_Req Req[MAX_QUEST_REQ];
	QuestS5_Rew Rew[MAX_QUEST_REQ];
};

class QuestS5
{
public:
	BOOL LoadQuestInfo(char * filename);
	void SendTWindow(int aIndex);
	void MonsterDie(int aIndex,int MonsterIndex);
	void FinishEvent(int aIndex, BYTE Event, BYTE eLevel);
	void KillPlayerInChaosCastle(int aIndex, BYTE eLevel);
	void KillPlayerGensEnemy(int aIndex);
	void ProtocolCore(int aIndex,char * lpMsg);
	void RemoveGensQuest(int aIndex);
	bool UseItem(int aIndex, int ItemID);
	bool IsQuestItemActive(int aIndex, int MobNum);
private:
	void SendNPCQuest(int aIndex);
	void SendDialogReq(int aIndex,BYTE SubCode,int QuestID,int NextStatus,WORD Quest);
	void DeletePlayerQuest(int aIndex,int QuestID,bool finished);
	void DelExactQuest(int aIndex, int QuestNum, int QuestIndex);
	void DeletePlayerDayofWeekQuest(int aIndex, WORD DayofWeek);
	bool CheckWeekQuest(int aIndex,int QuestID);
	int CheckNextStatus(int PlayerID, int QuestNum, int QuestIndex);
	int GetQuestID(int QuestNum, int QuestIndex);
	int GetNextStatus(int PlayerID, int QuestID);
	int GetQuestReqID(int QuestID, int QuestNum);
	int QuestRunning(int aIndex,WORD Number,WORD Quest);
	void AddQuest(int aIndex,WORD Number,WORD Quest);
	void GiveReward(int aIndex,WORD QuestID);
	bool CheckInvFree(int aIndex,WORD QuestID);
	BOOL PlayerCheckSkill(int aIndex,int Skill);
	int PlayerCheckItemCount(int aIndex,int QuestID, int ReqID,bool Delete);
	bool CheckAllowClass(int aIndex,int QuestIndex);

	BOOL Enabled;

	QuestS5_Info quest[MAX_QUEST_COUNT];
	int Count;
};

extern QuestS5 qs5;