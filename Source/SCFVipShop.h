#include "stdafx.h"

#if (PACK_EDITION>=3)

struct PMSG_BUYSHOPRESULT
{
	PBMSG_HEAD h;	// C1:32
	BYTE Result;	// 3
	BYTE ItemInfo[MAX_ITEM_INFO];	// 4
};

#if (CRYSTAL_EDITION == 1)
struct SDHP_SENDSVSHOP
{
	PBMSG_HEAD h;	// C1:01
	BYTE SubCode;
	BYTE mLevel;
	BYTE mOpt;
	BYTE mExc;
	BYTE mLuck;
	BYTE mSkill;
	BYTE mHarmony;
	BYTE mSock;
	BYTE mSockLevel;
	int pLevel;
	int pOpt;
	int pExc;
	int pLuck;
	int pSkill;
	int pHarmony;
	int pSock;
	int pFreeSock;
	int pSockLevel;
};

struct SDHP_OPENVSHOP
{
	PBMSG_HEAD h;	// C1:01
	BYTE SubCode;
	BYTE Open;
	int VipMoney;
};

struct SDHP_BUYSVSHOP
{
	PBMSG_HEAD h;	// C1:01
	BYTE SubCode;
	int ItemID;
	BYTE Days;
	BYTE mLevel;
	BYTE mOpt;
	BYTE mExc;
	BYTE mLuck;
	BYTE mSkill;
	BYTE mHarmony;
	BYTE mSock1;
	BYTE mSock2;
	BYTE mSock3;
	BYTE mSock4;
	BYTE mSock5;
};

struct SHOPITEM
{
	int Price_1Day;
	int Price_7Days;
	int Price_30Days;
	int Price_Forever;
	bool Enabled;
};


class SCFVipShop
{
public:
	void ProtocolCore(int aIndex, BYTE* aRecv);
	void SendInfo(int aIndex);
	void WindowState(int aIndex,BYTE State); //State 1 = Open; State 0 = Close
	void Init(char * Ini, char * FilePath);

//Vars:
	SHOPITEM item[MAX_TYPE_ITEMS*MAX_SUBTYPE_ITEMS];
	BOOL Enabled;
	BYTE IsForeverFFFE;
	int TypeCount;

private:
	void RecvBuy(int aIndex,SDHP_BUYSVSHOP * lpMsg);
	void Read(char * FilePath);

//Vars:
	BYTE MaxSockLevel;
	BYTE MaxSock;
	BYTE MaxExc;
	BYTE MaxOpt;
	BYTE MaxLevel;
	BYTE MaxLuck;
	BYTE MaxSkill;
	BYTE MaxHarmony;
	int pLevel;
	int pOpt;
	int pExc;
	int pLuck;
	int pSkill;
	int pHarmony;
	int pSocket;
	int pFreeSocket;
	int pSocketLevel;
};

extern SCFVipShop SVShop;
#endif
#endif