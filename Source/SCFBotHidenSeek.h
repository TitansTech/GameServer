#if (PACK_EDITION>=3)

#define MAX_BOTHIDER_REWARD		50
#define MAX_BOTHIDER_POS		100

struct BotHiderBodyItems
{
	int num;
	int level;
	int opt;
	bool Enabled;
};

struct botHiderStruct
{
	int index;
	BYTE Class;
	BOOL OnlyVip;
	char Name[11];
	bool Enabled;
	BotHiderBodyItems body[9];
};

struct botHiderPositions
{
	BYTE Map;
	BYTE X1;
	BYTE Y1;
	BYTE X2;
	BYTE Y2;
	char LocName[100];
};

struct botHiderReward
{
	WORD num;
	BYTE Level;
	BYTE Opt;
	BYTE Luck;
	BYTE Skill;
	BYTE Dur;
	BYTE Exc;
	BYTE Anc;
};

class ObjBotHider
{
public:
	void Init(char * FilePath);
	void GetGoodPosition(BYTE mapnumber, short x1, short y1, short x2, short y2, short &mx, short &my);
	BYTE MakeBot();
	void StartEvent();
	char * GetBotName();
	char * GetBotLocName(BYTE Pos);
	BYTE GetX();
	BYTE GetY();
	void RemoveBot();
	BOOL TradeOpen(int index, int nindex);
	int Minutes;
	int RepeatMsgAfterXSecs;
	bool Start;
	bool Founded;
private:
	void Read(char * FilePath);
	void GetReward(int aIndex);
	BYTE Enabled;
	BYTE GetDir();
	botHiderStruct bot;
	botHiderPositions pos[MAX_BOTHIDER_POS];
	BYTE posCount;
	BYTE ItemCount;
	botHiderReward reward[MAX_BOTHIDER_REWARD];
};

void ObjBotHider__InsideTrigger(void * lpParam);
extern ObjBotHider BotHider;

#endif