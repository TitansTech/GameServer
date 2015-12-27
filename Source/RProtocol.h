
struct PMSG_ANS_BLOODCASTLESCORE_5TH_SCFEXDB
{
	PBMSG_HEAD2 h;	// C1:0D
	char AccountID[10];	// 3
	char GameID[10];	// D
	int ServerCode;	// 18
	int Score;	// 1C
	int Class;	// 20
	int BridgeNum;	// 24
	int iLeftTime;	// 28
	int iAlivePartyCount;	// 2C
};

struct PMSG_REQ_BLOODCASTLE_ENTERCOUNT_SCFEXDB
{
	PBMSG_HEAD2 h;	// C1:0B
	char AccountID[10];	// 3
	char GameID[10];	// D
	int ServerCode;	// 18
	int iObjIndex;	// 1C
};

struct PMSG_ANS_EVENTUSERSCORE_SCFEXDB
{
	PBMSG_HEAD2 h;	// C1:01
	char AccountID[10];	// 3
	char GameID[10];	// D
	int ServerCode;	// 18
	int Score;	// 1C
	int Class;	// 20
	int SquareNum;	// 24
};

struct PMSG_ANS_BLOODCASTLE_ENTERCOUNT_SCFEXDB
{
	PBMSG_HEAD2 h;
	char AccountID[10];	// 3
	char GameID[10];	// D
	int ServerCode;	// 18
	int iObjIndex;	// 1C
	int iLeftCount;	// 20
};

struct PMSG_ANS_CL_ENTERCOUNT_SCFEXDB
{
	PBMSG_HEAD h;	// C1:9F
	BYTE btEventType;	// 3
	BYTE btLeftEnterCount;	// 4
};

void BC_SendRewardScore(int iIndex, int iSCORE, int iLeftTime, int iAlivePartyCount);
void BC_EGReqBloodCastleEnterCount(int iIndex);
void BC_EGAnsBloodCastleEnterCount( PMSG_ANS_BLOODCASTLE_ENTERCOUNT_SCFEXDB* lpMsg);
void DS_SendRankingInfo(int iIndex);