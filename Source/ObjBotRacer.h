#if (PACK_EDITION>=3)

#define MAX_BOTRACER		2
#define MAX_BOTRACERMAXPOS	50
#define MAX_BOTRACER_REWARD	10

struct BotRacerBodyItems
{
	int num;
	int level;
	int opt;
	bool Enabled;
};

struct botRacerPosition
{
	BYTE Map;
	BYTE X;
	BYTE Y;
	char Text[100];
};

struct botRacerStruct
{
	int Class;
	char Name[11];
	bool Enabled;
	int index;
	BotRacerBodyItems body[9];
	botRacerPosition Pos[MAX_BOTRACERMAXPOS];
};

struct botRacerReqStruct
{
	BYTE OnlyVIP;
	float Zen;
	int VipMoney;
	int MinLevel;
};

void ObjBotRacer__InsideTrigger(void * lpParam);

class ObjBotRacer
{
public:
	BOOL Enabled;
	void Init(char * FilePath);
	void Read(char * FilePath);
	bool MakeBot();
	void StartEvent();
	void CheckRaceTime(int aIndex);
	void ResetTime(int aIndex);
	void CheckStatus();
	BOOL TradeOpen(int index,int nindex);
	BYTE GetDir();
	//
	botRacerStruct bot[MAX_BOTRACER];
	int MaxTime;
	BYTE Period;
	bool Start;
	bool ForceClose;
	int RepeatMsgAfterXMins;
	BYTE CurrentPos;

	BYTE DayStart;
	BYTE DayEnd;

	botReward_Data_Struct reward[MAX_BOTRACER_REWARD];
	int RewardCount;

private:
	botRacerReqStruct req;

	int GetBotIndex(int aIndex);
	BYTE PosCount;
};
extern ObjBotRacer BotRacer;

#endif
