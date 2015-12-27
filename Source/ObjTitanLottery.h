
#if (PACK_EDITION>=3)

#define MAX_TITANLOTTERY_NUMS		4

struct cObjTitanLottery_LotteryInfo
{
	BYTE	Type;
	int		MinReward;
	int		MaxReward;
	BYTE	AdditionalReward;
};

struct SDHP_RECVLOTTERYINFO
{
	PBMSG_HEAD h;
	
	int WinnersCount;
	int Reward;
	int NextReward;
};

struct SDHP_SENDLOTTERYBUYTICKET
{
	PBMSG_HEAD h;
	BYTE SubType;						//1
	
	char AccountID[11];
	char Name[11];
	BYTE Nums[MAX_TITANLOTTERY_NUMS];
	int  ToReward;
};

struct SDHP_SENDLOTTERYINFO
{
	PBMSG_HEAD h;
	BYTE SubType;						//0

	BYTE Nums[MAX_TITANLOTTERY_NUMS];

	cObjTitanLottery_LotteryInfo info;
	botReward_Data_Struct ExReward;
};

class cObjTitanLottery
{
public:

	void Read(char * FilePath);
	void StartEvent();
	bool ExistNumber(BYTE Number);
	void BuyTicket(int aIndex, BYTE Num1, BYTE Num2, BYTE Num3, BYTE Num4);
	void SendBuyTicket(int aIndex, BYTE Num1, BYTE Num2, BYTE Num3, BYTE Num4);
	void ShowInfo(SDHP_RECVLOTTERYINFO * aRecv);

	BYTE	Enabled;

	cObjTitanLottery_LotteryInfo	Info;
	botReward_Data_Struct			ExReward;

	int								MaxNumber;
	bool							Start;

	BYTE							Nums[MAX_TITANLOTTERY_NUMS];

	BYTE							Taxes;
	int								TypeCost;
	int								Cost;
	BYTE							MaxTicketBuy;
};

extern cObjTitanLottery ObjTitanLottery;
void cObjTitanLottery__StartEvent(void * pvoid);

#endif