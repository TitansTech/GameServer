
#define CHERRYBLOSSOM_REWARDITEMS_COUNT		150

struct CherryBlossomItems
{
	WORD num;
	BYTE Level;
	BYTE Opt;
	BYTE Luck;
	BYTE Skill;
	BYTE Dur;
	BYTE Exc;
	BYTE pExc;
	BYTE Anc;
};

class cCherryBlossom
{
public:
	void SendDialog(int aIndex);
	bool GetReward(int aIndex);
	void ReadItems(char * FilePath);
	void Read(char * FilePath);

	BOOL Enabled;
private:
	CherryBlossomItems items[CHERRYBLOSSOM_REWARDITEMS_COUNT];
	int iCount;
};

extern cCherryBlossom CherryBlossom;