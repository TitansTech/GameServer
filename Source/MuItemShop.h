
struct MUITEMSHOP_UPDATEINFO_SEND_2
{
	PBMSG_HEAD h;
	BYTE subcode;//3
	BYTE Unk1[9];
	BYTE Coin[8];
	BYTE CoinP[8];
	BYTE Unk2[8];
	BYTE GoblinCoin[8];
};

struct PMSG_CSHOPBUYRECV
{
	PBMSG_HEAD h;
	BYTE subcode;//3
	DWORD ID;//PACKAGE
	DWORD Category;
	DWORD OptionsID;
	WORD ItemID;
	BYTE Type;//252 = (C) // 253 = (P) // 0 =  Goblin
	BYTE Unk2;
	BYTE Unk3;
	BYTE Unk4;
};

struct PMSG_CSHOPBUYGIFTRECV
{
	PBMSG_HEAD h;
	BYTE subcode;//3
	DWORD ID;
	DWORD Category;
	DWORD OptionsID;
	WORD Unk1;
	BYTE Unk2;
	WORD ItemID;
	BYTE Type;//252 = (C) // 253 = (P) // 0 =  Goblin
	BYTE Unk3;
	BYTE Unk4;
	BYTE Unk5;
	//S6 FIX ON GIFT
	BYTE Unk6;
	//S6 FIX ON GIFT
	char Name[11];
	char Msg[200];
};

struct PMSG_CSHOPOPENSEND
{
	PBMSG_HEAD h;
	BYTE subcode;//3
	BYTE State;
};

struct PMSG_CSHOPVALUESEND
{
	PBMSG_HEAD h;
	BYTE subcode;//3
	WORD Value1;
	WORD Value2;
	WORD Value3;
};


struct PMSG_CSHOPLISTRECV
{
	PBMSG_HEAD h;
	BYTE subcode;//5
	BYTE State;
	WORD Unk1;//Zero
	BYTE List;
};


struct PMSG_CSHOPLISTPAGES_SEND
{
	PBMSG_HEAD h;
	BYTE subcode;
	WORD ItemsCount;
	WORD ItemsMax;
	WORD PageIndex;
	WORD PageCount;
};

struct PMSG_CSHOPLISTGIFTITEMS_SEND
{
	PBMSG_HEAD h;
	BYTE subcode;
	DWORD Time;
	DWORD Serial2;
	BYTE Unk1;
	BYTE Unk2;
	BYTE Unk3;
	WORD ID1;
	BYTE Unk4;
	BYTE Unk5;
	WORD ID2;
	BYTE Unk6;
	BYTE Unk7;
	BYTE Unk8;
	BYTE Unk9;
	BYTE Unk10;
	BYTE Unk11;
	BYTE Unk12;
	BYTE Unk13;
	BYTE Unk14;
	BYTE Unk15;
	BYTE Unk16;
	char Name[11];
	char Msg[200];
};

struct PMSG_CSHOPLISTITEMS_SEND
{
	PBMSG_HEAD h;
	BYTE subcode;
	DWORD Time;
	DWORD Serial2;
	BYTE Unk1;
	BYTE Unk2;
	BYTE Unk3;
	WORD ID1;
	BYTE Unk4;
	BYTE Unk5;
	WORD ID2;
	BYTE Unk6;
	BYTE Unk7;
	BYTE Unk8;
	BYTE Unk9;
	BYTE Unk10;
	BYTE Unk11;
	BYTE Unk12;
	BYTE Unk13;
	BYTE Unk14;
	BYTE Unk15;
	BYTE Unk16;
};

struct PMSG_CSHOPUSEITEMRECV
{
	//0xf3,0x99,0x02,0x00,0xa2,0xf6,0x2a
	PBMSG_HEAD h;
	BYTE subcode;//0x0B
	DWORD Time;
	DWORD Serial2;
	BYTE unk1;
	BYTE CategoryID;
};

struct PMSG_CSHOPSENDITEMTIMESTAMP
{
	PBMSG_HEAD h;
	BYTE subcode;
	WORD ItemID;
	BYTE Pos;
	BYTE Unk;
	DWORD Time;
};

#define CSHOP_PRODUCT_COUNT			512
#define CSHOP_PACKAGE_COUNT			512
#define CSHOP_CATEGORY_COUNT		20
#define CSHOP_PACKAGE_ID2_COUNT		4

struct MuItemShop_PackageID
{
	BYTE CatID;
	BYTE Sort;
	WORD PackID;
	WORD ProdID;
	WORD ItemID;
	int Value;
	WORD ID2[CSHOP_PACKAGE_ID2_COUNT];
};

struct MuItemShop_ProductID
{
	WORD ID1;
	WORD ID2;
	int Value;
	int ItemID;
	int Dur;
	BYTE Rate;
	BYTE Quant;
	WORD Days;
	char Name[50];
};

struct MuItemShop_CategoryID
{
	BYTE ID;
	char Name[50];
};


struct SENDBUFF
{
	PBMSG_HEAD h;
	BYTE subcode;//0x00
	BYTE Unk1;
	BYTE Unk2;
	BYTE Unk3;
	BYTE Unk4;
	BYTE Unk5;
	BYTE Unk6;
	BYTE Unk7;
	BYTE Unk8;
	DWORD Time;
	BYTE BuffType;
	BYTE Unk9;
	BYTE Unk10;
	BYTE Unk11;
};

class cMuItemShop
{
public:
	void ProtocolCore(int aIndex,BYTE * aRecv);
	void OpenRequest(int aIndex);
	void SendShopValue(int aIndex);
	void Init(char * FilePath);
	void Read_Package(char * FilePath);
	void Read_Product(char * FilePath);
	void Read_Category(char * FilePath);
	void AddItem(int aIndex, DWORD m_Serial2, WORD ItemID, WORD ID1, WORD ID2, BYTE ListType);
	void AddGiftItem(int aIndex, DWORD m_Serial2, WORD ItemID, WORD ID1, WORD ID2, BYTE ListType, char * Name, char * Msg);
	void ItemSendTime(int aIndex, BYTE Pos);
	bool IsCShopItem(int aIndex, BYTE Pos);
	bool IsCShopItemByType(WORD ItemType, BYTE m_ItemOptionEx);
	bool DeleteItem(int aIndex);
	void EarnGoblinPointsCBMix(int aIndex, int points, int isFail);
	int UseBuffGetTime(int aIndex, int Slot);
	void SetBuff(int aIndex);
	
	MuItemShop_ProductID ProdID[CSHOP_PRODUCT_COUNT];

private:
	bool HaveSpace(int aIndex, BYTE ListType);
	bool HaveSpace(int aIndex, BYTE ListType, int SpacesCount);
	void ItemBuyRecv(int aIndex,PMSG_CSHOPBUYRECV* lpMsg);
	void ItemBuyGiftRecv(int aIndex,PMSG_CSHOPBUYGIFTRECV* lpMsg);
	void ListReqRecv(int aIndex,PMSG_CSHOPLISTRECV* lpMsg);
	void ItemUse(int aIndex,PMSG_CSHOPUSEITEMRECV* lpMsg);
	int GetProductID(WORD ProductID, WORD ID2, int ItemID);
	int GetPack_Index(WORD PackageID, WORD ID2, int ItemID);
	int GetProductID_Index(WORD ProductID, WORD ID2, int ItemID);
	int GetProductID_Index(int ItemID, int Dur, BYTE Rate, BYTE Quant);
	BYTE GetItemsInListCount(int aIndex, BYTE ListType);
	int ItemUse_SearchBySerial2(int aIndex, DWORD Serial2,BYTE & ListType);
	void ListSend(int aIndex,BYTE List, BYTE PageIndex);
	void ListAddItem(int aIndex, WORD ID1, WORD ID2, DWORD Serial2);
	void ListAddGiftItem(int aIndex, WORD ID1, WORD ID2, DWORD Serial2, char * Name, char * Msg);
	void DeleteFromList(int aIndex, BYTE Pos, BYTE ListType);
	double getPoints(double points);
	void SetPoints(double Val, LPBYTE Bytes);
	void SendPoints(int aIndex);
	char * GetCategoryID(BYTE Category);
	bool UseFruit(int aIndex, WORD ItemID);

	bool RemovePoints(int aIndex, BYTE Type, WORD ProdIndex);
	bool RemovePointsbyPackage(int aIndex, BYTE Type, WORD PackIndex);

	MuItemShop_PackageID PackID[CSHOP_PACKAGE_COUNT];
	MuItemShop_CategoryID CatID[CSHOP_CATEGORY_COUNT];
	int PackageCount;
	int ProductCount;
	int CategoryCount;
	BOOL Enabled;

	WORD SScript1;
	WORD SScript2;
	WORD SScript3;
	WORD SBanner1;
	WORD SBanner2;
	WORD SBanner3;

	BYTE IsAddGoblinShopPointsOnMixFail;
	BYTE OnFailAddPercentsOfSuccessAsPointsDiv;
	BYTE IsAddGoblinShopPointsOnMixSuccess;
	BYTE OnSuccessAddPercentsOfSuccessAsPointsDiv;
};

extern cMuItemShop MuItemShop;