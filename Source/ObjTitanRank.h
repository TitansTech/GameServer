
#if (PACK_EDITION>=3)

#define MAX_TITANRANK_CNT	50
#define MAX_TITANRANK_REW	3



struct cObjTitanRank_AccountPlayerINFO
{
	char AccountID[11];
	char Name[11];
};

struct cObjTitanRank_Info
{
	BYTE Enabled;
	BYTE rIndex;
	BYTE Type;
	BYTE SubType;
	BYTE Class;
	BYTE Top;

	BYTE RewCnt;

	botReward_Data_Struct	Reward[MAX_TITANRANK_REW];
};

struct SDHP_GETRANK_INFORECV
{
	PBMSG_HEAD h;
	BYTE rIndex;
	cObjTitanRank_AccountPlayerINFO info[MAX_TITANRANK_REW];
};

struct SDHP_GETRANKINFO
{
	PBMSG_HEAD h;
	BYTE rIndex;
	cObjTitanRank_Info info;
};


class cObjTitanRank
{
public:
	void Read(char * FileName);
	void StartEvent();
	void ShowInfo(BYTE * pRecv);

	cObjTitanRank_Info Tops[MAX_TITANRANK_CNT];
	char Tops_Text[MAX_TITANRANK_CNT][100];

	int Cnt;
	BYTE Enabled;
	bool Start;

	int SecsDelay;
	char StartMsg[100];

};

void cObjTitanRank__StartEvent(void * pvoid);

extern cObjTitanRank ObjTitanRank;

#endif