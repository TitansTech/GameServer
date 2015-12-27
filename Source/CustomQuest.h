//type:
/*
0 - NONE
1 - Zen Need
2 - Item Need
3 - Kill Mob Need
*/

//typeReward:
/*
0 - NONE
1 - Zen
2 - Level Up
3 - LevelUpPoint
4 - MasterPoints
5 - Item
*/


#if (PACK_EDITION>=2)

#include "user.h"

#define CQMAXCOUNT		256

struct CQItems
{
	BYTE Type;
	short Index;
	BYTE Level;
	BYTE Opt;
	BYTE Luck;
	BYTE Skill;
	BYTE Dur;
	BYTE Exc;
	BYTE Count;
};
struct CQStruct
{
	BOOL Active;
	//
	char Title[3][50];		 //[0] First Title - [1] IF_FINISH Title - [2] ELSE FINISH Title
	char Text[3][1000];		 //[0] First Text  - [1] IF_FINISH Text  - [2] ELSE FINISH Text
	//Req
	short Quest;
	short MasterLevel;
	short Monster_Kill[2]; //[0] Monster Number - [1] Monster Kill Count;
	//Req & Reward
	short Class[2];		 //[0] Req - [1] Reward
	short Level[2];		 //[0] Req - [1] Reward
	int Reset[2];		 //[0] Req - [1] Reward
	int Zen[2];			 //[0] Req - [1] Reward
	CQItems item[2];	 //[0] Req - [1] Reward
	//Reward
	BYTE LevelUpPoint;
	BYTE MasterPoint;
};

class cCQuest
{
public:
	BOOL Enabled;
	void Clear();

	void Init(char * filename);
	BOOL Command(LPOBJ lpObj,char * Command);
	void LevelUp(LPOBJ lpObj);
	void MonsterDie(LPOBJ lpObj,int MonsterIndex);
	CQStruct Quest[CQMAXCOUNT][10];
	BYTE StateCount[CQMAXCOUNT];

private:
	void ReadTxt(char * filename);
	void MailSend(LPOBJ lpObj, int QuestNr, int StateNr, BYTE TextNr);
	BOOL CheckStateComplete(LPOBJ lpObj, int QuestNr);
	void StateComplete(LPOBJ lpObj, int QuestNr);
	BOOL IsActive(LPOBJ lpObj,int Quest);

	short Count;
	char Cmd[CQMAXCOUNT][2][20]; //[0] Accept Quest Command // [1] Finish Quest Command
};

extern cCQuest customQuest;


#endif