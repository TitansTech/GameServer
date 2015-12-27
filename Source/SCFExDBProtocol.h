#include "DSProtocol.h"
#include "stdafx.h"
#include "user.h"

struct SDHP_GETMASTERINFO_SAVE
{
	PBMSG_HEAD h;
	char AccountID[11];
	char Name[11];
	short aIndex;
	int MasterLevel;
	int MasterPoints;
};


struct SDHP_GETACCOUNTINFO
{
	PBMSG_HEAD h;	// C1:01
	char AccountID[11];
	short aIndex;
};

struct SDHP_GETACCOUNTEXTRAINFO_SAVE
{
	PBMSG_HEAD h;
	char AccountID[11];
	short aIndex;
	BYTE Vip;
	int VipMoney;
	int VipDays;
	int WarehouseVipCount;
	BYTE HaveBot;
	int LuckyCoins;
	BYTE ExWarehouse;
	BYTE LotteryCnt;
};

struct SDHP_SETSTATUSBAN_INFOSAVE
{
	PBMSG_HEAD h;
	char Name[11];
	BYTE Ban;
	BOOL Type;
};

struct SDHP_GETPLAYEREXTRAINFO
{
	PBMSG_HEAD h;
	char AccountID[11];
	char Name[11];
	short aIndex;
};

struct SDHP_SETPLAYERGENSLEAVEINFO_SAVE
{
	PBMSG_HEAD h;
	char AccountID[11];
	char Name[11];
};

struct SDHP_GETPLAYEREXTRAINFO_SAVE
{
	PBMSG_HEAD h;
	char AccountID[11];
	char Name[11];
	short aIndex;
	int MasterLevel;
	int MasterPoints;
	int Resets;
	int PCPoints;
	WORD CShop_SealItem;
	int CShop_SealTime;
	WORD CShop_ScrollItem;
	int CShop_ScrollTime;
	BOOL Married;
	char MarriedName[11];

	BYTE Family;
	int Contribution;
	int Rank;
	BYTE NotAllow;
	BYTE RecvRewItem;
};

struct SDHP_GETPLAYERINFO
{
	PBMSG_HEAD h;	// C1:01
	char AccountID[11];
	char Name[11];
	short aIndex;
};

struct SDHP_GETPLAYERINFO_INFOSAVE
{
	PBMSG_HEAD h;
	char Name[11];
	int MasterLevel;
	int MasterPoints;
	int Resets;
	int PCPoints;
	WORD CShop_SealItem;
	int CShop_SealTime;
	WORD CShop_ScrollItem;
	int CShop_ScrollTime;
	BOOL Married;
	char MarriedName[11];
	BYTE GensFamily;
	int GensCont;
	BYTE GensNotAllow;
	BYTE GensRecvRewItem;
};

struct SDHP_SETMARRYDIVORSE_INFOSAVE
{
	PBMSG_HEAD h;
	char Name[11];
};

struct SDHP_GETWAREHOUSEDBEx_S6E2_SAVE
{
	PWMSG_HEAD h;
	char AccountID[10];	// 4
	short aIndex;	// E
	int Money;	// 10
	BYTE dbItems[3840];	// 14
	BYTE DbVersion;	// 794
	short pw;	// 796
	int ExNum;
};

struct SDHP_GETWAREHOUSEDBEx_SAVE
{
	PWMSG_HEAD h;
	char AccountID[10];	// 4
	short aIndex;	// E
	int Money;	// 10
	BYTE dbItems[1920];	// 14
	BYTE DbVersion;	// 794
	short pw;	// 796
	int ExNum;
};

struct SDHP_GETWAREHOUSEDBEX
{
	PBMSG_HEAD h;	// C1:08
	char AccountID[10];	// 3
	short aIndex;	// E
	int ExNum;
	BYTE IsS6E2;
};

struct SDHP_GETPETBOT
{
	PWMSG_HEAD h;	// C1:16
	BYTE Result;
	short aIndex;
	char AccountID[11];
	char Name[11];
	BYTE Class;
	short Level;
	unsigned long Exp;
	int Power;
	int Defense;
	WORD Life;
	WORD MaxLife;
	WORD Mana;
	WORD MaxMana;
	BYTE dbInventory[1728];
	BYTE dbMagicList[180];
	BYTE MapNumber;
	BYTE MapX;
	BYTE MapY;
	int BotLife;
	int BotMaxLife;

	int BotLvlUpDefense;
	int BotLvlUpPower;
	int BotLvlUpLife;
	int BotLvlUpMana;
	int BotLvlUpExp;
	int BotLvlUpMaxLevel;
	BYTE BotVersion;
};

struct SDHP_PETBOTCREATERESULT
{
	PBMSG_HEAD h;	//
	int aIndex;
	char Name[11];
	int BotPetNum;
	BYTE result;
};


struct SDHP_GENSGUILD_INFO_REQUEST
{
	PBMSG_HEAD h;	// C1:7E
	char GuildName[9];	// 8
};

struct SDHP_GENSGUILD_INFO_GET
{
	PBMSG_HEAD h;	// C1:7E
	char GuildName[9];	// 8
	BYTE Family;
};

struct SDHP_GETHUNTZONEOPEN
{
	PBMSG_HEAD h;
	BYTE subcode;
	int mapServer;
};


struct SDHP_SETHUNTZONEOPEN
{
	PBMSG_HEAD h;
	BYTE subcode;
	int state;
	int mapServer;
};

//

struct SDHP_GETGUILDWAREHOUSEDB
{
	PBMSG_HEAD h;	// C1:08
	char G_Name[10];
	char AccountID[10];	// 3
	short aIndex;	// E
	BYTE IsS6E2;
};


struct SDHP_GETGUILDWAREHOUSEDB_SAVE
{
	PWMSG_HEAD h;
	char G_Name[10];
	char AccountID[10];	// 4
	short aIndex;	// E
	int Money;	// 10
	BYTE dbItems[1920];	// 14
	BYTE DbVersion;	// 794
	short pw;	// 796
};
struct SDHP_GETGUILDWAREHOUSEDB_S6E2_SAVE
{
	PWMSG_HEAD h;
	char G_Name[10];
	char AccountID[10];	// 4
	short aIndex;	// E
	int Money;	// 10
	BYTE dbItems[3840];	// 14
	BYTE DbVersion;	// 794
	short pw;	// 796
};

struct SDHP_GETPLAYERMASTERSKILL_SAVE
{
	PWMSG_HEAD h;
	char AccountID[11];
	char Name[11];
	short aIndex;
	MASTERSKILLS Skill[MAX_SKILLTREE];
};

struct MS_ANS_MSLISTDATA
{
	struct PWMSG_HEAD h;	// C2:53
	char AccountID[11];
	short aIndex;
	int iCount;				//
};

struct MS_MSLISTDATA
{	
	DWORD m_Serial1;
	DWORD m_Serial2;
	WORD ItemID;
	WORD ID1;
	WORD ID2;
};



#if (PACK_EDITION>=3)
struct SDHP_SETPLAYERRACE_INFOSAVE
{
	PBMSG_HEAD h;
	char AccountID[11];
	char Name[11];
	int Seconds;
};

struct SDHP_GETREWARD_INFOSAVE
{
	PBMSG_HEAD h;
	char AccountID[11];
	char Name[11];
	short aIndex;

	int ID_Num;
	botReward_Data_Struct rew;
};

struct SDHP_GETREWARD_INFODEL
{
	PBMSG_HEAD h;
	char AccountID[11];
	char Name[11];
	int ID_Num;
};

struct SDHP_SETRESET_LOG
{
	PBMSG_HEAD h;
	char AccountID[11];
	char Name[11];
	int Reset;
	BYTE Type;
};


#endif

//Protocols Functions:

void SCFExDataServerLoginResult(SDHP_RESULT * lpMsg);
void DGGetPlayerExtraInfo(SDHP_GETPLAYEREXTRAINFO_SAVE * lpMsg);

void JGSetPlayerExtraInfo(LPOBJ lpObj);

void JGSetPlayerMarryDivorseInfo(char * Name);

void DGGetAccountExtraInfo(SDHP_GETACCOUNTEXTRAINFO_SAVE * lpMsg);
void JGSetAccountExtraInfo(LPOBJ lpObj);
void GetPlayerExtraInfo(int aIndex, BYTE Value);

void GJSetStatusBan(char* Name, BOOL Type, BYTE Ban);

#if (PACK_EDITION>=2)
void GDGetWarehouseNoItemEx( SDHP_GETWAREHOUSEDB_RESULT * lpMsg);
void GDSetWarehouseListEx(int aIndex);
void GDGetWarehouseListEx(int aIndex, char* AccountID);
void GDGetGuildWarehouseList(int aIndex);
void GDDelGuildWarehouseList(int aIndex);
void GDSetGuildWarehouseList(int aIndex);
#endif

#if (PACK_EDITION>=3)
void GDGetPetBot(SDHP_GETPETBOT * lpMsg);
void GDAskPetBot(int aIndex, char* AccountID);
void GDSetPetBot(int aIndex);
void GDCreateResultPetBot(SDHP_PETBOTCREATERESULT * aRecv);
void GDCreatePetBot(int aIndex,int BotNum,char * Name);
void SendRaceInfo(int aIndex);
void SendRaceReward();
void SendReward(int aIndex, int FromEvent, botReward_Data_Struct rew);
void GetTitanReward(SDHP_GETREWARD_INFOSAVE * lpMsg);
#endif


void GensGuildFamily(char * GuildName);
void DGGetGuildGensInfo(SDHP_GENSGUILD_INFO_GET * lpMsg);

void DGGetCleanLuckyCoinsInfo(PMSG_DEFRESULT * lpMsg);

#if (GS_CASTLE==1)
void GS_DGGetHuntZoneValue(SDHP_SETHUNTZONEOPEN * lpMsg);
void GS_DGSetHuntZoneOpen();
void GS_DGGetHuntZoneOpen();
#endif
void GJSetPlayerMasterSkillInfo(SDHP_GETPLAYERMASTERSKILL_SAVE * aRecv);
void GDGetAccounExtraInfo(int aIndex, char* AccountID);
#if (PACK_EDITION>=3)
void SetResetLog(int aIndex, BYTE MasterReset);
#endif