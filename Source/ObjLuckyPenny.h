
#define LUCKYPENNY_REWARDITEMS_COUNT		50

struct LuckyPennyItems
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

class cLuckyPenny
{
public:
	void RegisterOpt(int aIndex);
	void RegisterSend(int aIndex);
	void Exchange(int aIndex,BYTE Count);
	void ReadBag(char * FilePath);
	void Read(char * FilePath);
private:
	bool GetReward(int aIndex,BYTE Count);
	BOOL Enabled;
	LuckyPennyItems items[3][LUCKYPENNY_REWARDITEMS_COUNT];
	int iCount[3];
};

struct PMSG_LUCKYCOINSCOUNT //C1:BF:0B
{
	PBMSG_HEAD h;
	BYTE subcode;
	DWORD Count;
};

extern cLuckyPenny LuckyPenny;