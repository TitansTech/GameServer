#include "DSProtocol.h"
#include "stdafx.h"
#include "user.h"

extern wsJoinServerCli wsExServerCli;

#define WM_GM_EXS_CLIENT_MSG_PROC		0x430

#define MUSHOPLIST_SIZE					720
#define MUSHOPLIST_MSG_SIZE				4995

struct SDHP_GETPLAYEROFFICIALHELPER_SAVE
{
	PWMSG_HEAD h;
	char AccountID[11];
	char Name[11];
	short aIndex;
	BYTE Data[257];
};

struct SDHP_PLAYEROFFICIALHELPER_SEND
{
	PWMSG_HEAD h;
	BYTE Data[257];
};

struct SDHP_GETCUSTOMQUEST
{
	BYTE State;
	BYTE Status;
};

struct SDHP_GETPLAYERCUSTOMQUESTS_SAVE
{
	PWMSG_HEAD h;
	char Name[11];
	short aIndex;
	SDHP_GETCUSTOMQUEST CustomQuest[256];
};

struct SDHP_DELETEPLAYERS5QUEST_INFO
{
	PBMSG_HEAD h;
	char AccountID[11];
	char Name[11];
};

struct SDHP_GETPLAYERS5QUEST_SAVE
{
	PWMSG_HEAD h;
	char AccountID[11];
	char Name[11];
	short aIndex;
	S5QUEST S5Quest[MAX_QUEST_COUNT];
};

struct SDHP_GETPLAYERS5QUEST_INFOSAVE
{
	PWMSG_HEAD h;
	char AccountID[11];
	char Name[11];
	S5QUEST S5Quest[MAX_QUEST_COUNT];
};

struct SDHP_ADDGOBLINCOIN
{
	PBMSG_HEAD h;	// C1:01
	char AccountID[11];
	int GoblinCoin;
};

struct SDHP_MUITEMSHOPBUY
{
	PBMSG_HEAD h;
	char AccountID[11];
	short aIndex;

	WORD ItemID;
	WORD ID1;
	WORD ID2;
};

struct SDHP_MUITEMSHOPGIFTBUY
{
	PBMSG_HEAD h;
	char AccountID[11];
	short aIndex;

	DWORD m_Serial2;
	WORD ItemID;
	WORD ID1;
	WORD ID2;
	char Name[11];
	char Msg[100];
};

struct SDHP_MUITEMSHOPBUYRECV
{
	PBMSG_HEAD h;
	char AccountID[11];
	short aIndex;
	DWORD m_Serial2;
	WORD ItemID;
	WORD ID1;
	WORD ID2;
};

struct SDHP_MUSHOPLISTSAVE
{
	PWMSG_HEAD h;	
	char AccountID[11];
	short aIndex;
	int WCoin;
	int WCoinP;
	int GoblinCoin;

	BYTE List[MUSHOPLIST_SIZE];	//
	BYTE GiftList[MUSHOPLIST_SIZE];	//
	BYTE GiftMsg[MUSHOPLIST_MSG_SIZE];	//
};


struct SDHP_MUSHOPLISTSAVE_Normal
{
	PWMSG_HEAD h;	
	char AccountID[11];

	BYTE List[MUSHOPLIST_SIZE];	//
};

struct SDHP_MUSHOPLISTSAVE_Gift
{
	PWMSG_HEAD h;	
	char AccountID[11];

	BYTE GiftList[MUSHOPLIST_SIZE];	//
	BYTE GiftMsg[MUSHOPLIST_MSG_SIZE];	//
};

struct SDHP_MUSHOPLISTSAVE_Points
{
	PBMSG_HEAD h;
	char AccountID[11];

	int WCoin;
	int WCoinP;
	int GoblinCoin;
};

struct SDHP_GETACCOUNTEXTRAINFO
{
	PBMSG_HEAD h;	// C1:01
	char AccountID[11];
	short aIndex;
};


struct PMSG_ANS_MONSTERKILLED
{
	PBMSG_HEAD h;
	int ServerCode;
	char AccountID[10];
	char Name[10];
	WORD Monster;
};


void ExtraServerLoginRecv(SDHP_RESULT * lpMsg);
void ExServerProtocolCore(BYTE protoNum, LPBYTE aRecv, int aLen);
void ExServerClientMsgProc(WPARAM wParam, LPARAM lParam);
BOOL ExServerConnect(char* ConnectServer,int ExServerPort ,DWORD wMsg);

void GetPlayerExServerInfo(int aIndex, BYTE Value);
void GetAccountExServerInfo(int aIndex, BYTE Value);

void SetPlayerS5QuestInfo(int aIndex);
void DGGetPlayerS5QuestInfo(SDHP_GETPLAYERS5QUEST_SAVE * lpMsg);
void JGDeletePlayerQ5Info(char * Name, char* AccountID);

void GDGetOfficialHelperInfo(SDHP_GETPLAYEROFFICIALHELPER_SAVE * lpMsg);
void JGSetOfficialHelper(LPOBJ lpObj);

void JGSetCustomQuests(LPOBJ lpObj);
void GDSetPlayerCustomQuests(SDHP_GETPLAYERCUSTOMQUESTS_SAVE * lpMsg);

void MuItemShopUpdateGoblinCoin(int aIndex, int GoblinCoin);

void MuItemShopBuyRecv(SDHP_MUITEMSHOPBUYRECV * lpMsg);
void MuItemShopBuyGiftRecv(SDHP_MUITEMSHOPGIFTBUY * lpMsg);
void MuItemShopBuySend(int aIndex, WORD ProductIndex, BYTE Type);
void MuItemShopGiftBuySend(int aIndex, WORD ProductIndex, BYTE Type, char * Name, char * Msg);
void MuItemShopListRecv(SDHP_MUSHOPLISTSAVE * lpMsg);
void MuItemShopListSend(int aIndex);
void ExServerLogin();

void SendMonsterKilled(int aIndex, WORD MonsterID);