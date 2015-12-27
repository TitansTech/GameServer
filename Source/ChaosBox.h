#ifndef CHAOSBOX_H
#define CHAOSBOX_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "protocol.h"
#include "..\common\zzzitem.h"


#define CHAOS_BOX_SIZE 32
#define MIN_CHAOS_ITEM_LEVEL 4

#define CHAOS_BOX_RANGE(x) (((x)<0)?FALSE:((x)>CHAOS_BOX_SIZE-1)?FALSE:TRUE )

enum CHAOS_TYPE
{
	CHAOS_TYPE_DEFAULT = 0x1,
	CHAOS_TYPE_DEVILSQUARE = 0x2,
	CHAOS_TYPE_UPGRADE_10 = 0x3,
	CHAOS_TYPE_UPGRADE_11 = 0x4,
	CHAOS_TYPE_UPGRADE_12 = 0x16,
	CHAOS_TYPE_UPGRADE_13 = 0x17,
	CHAOS_TYPE_DINORANT = 0x5,
	CHAOS_TYPE_FRUIT = 0x6,
	CHAOS_TYPE_SECOND_WING = 0x7,
	CHAOS_TYPE_BLOODCATLE = 0x8,
	CHAOS_TYPE_FIRST_WING = 0xb,
	CHAOS_TYPE_SETITEM = 0xc,
	CHAOS_TYPE_DARKHORSE = 0xd,
	CHAOS_TYPE_DARKSPIRIT = 0xe,
	CHAOS_TYPE_CLOAK = 0x18,
	CHAOS_TYPE_BLESS_POTION = 0xf,
	CHAOS_TYPE_SOUL_POTION = 0x10,
	CHAOS_TYPE_LIFE_STONE = 0x11,
#if (GS_CASTLE==1)
	CHAOS_TYPE_CASTLE_SPECIAL_ITEM_MIX = 0x12,
#endif
	CHAOS_TYPE_HT_BOX = 0x14,
	CHAOS_TYPE_FENRIR_01 = 0x19,
	CHAOS_TYPE_FENRIR_02 = 0x1a,
	CHAOS_TYPE_FENRIR_03 = 0x1b,
	CHAOS_TYPE_FENRIR_04 = 0x1c,
	CHAOS_TYPE_COMPOUNDPOTION_LV1 = 0x1e,
	CHAOS_TYPE_COMPOUNTPOTION_LV2 = 0x1f,
	CHAOS_TYPE_COMPOUNTPOTION_LV3 = 0x20,
	CHAOS_TYPE_JEWELOFHARMONY_PURITY = 0x21,
	CHAOS_TYPE_JEWELOFHARMONY_MIX_SMELTINGITEM = 0x22,
	CHAOS_TYPE_JEWELOFHARMONY_RESTORE_ITEM = 0x23,
	CHAOS_TYPE_380_OPTIONITEM = 0x24,
	CHAOS_TYPE_LOTTERY_MIX = 40,
	CHAOS_TYPE_OLD_PAPER									= 0x25,
	CHAOS_TYPE_CONDOR_FEATHER								= 0x26,
	CHAOS_TYPE_THIRD_WING									= 0x27,
	
	CHAOS_TYPE_SEED_JEWEL									= 0x2A,
	CHAOS_TYPE_SEED_SPEAR									= 0x2B,
	CHAOS_TYPE_SOCKET_ITEM_ADD								= 0x2C,
	CHAOS_TYPE_SOCKET_ITEM_DEL								= 0x2D,
	CHAOS_TYPE_CHERRYBLOSSOM								= 41,
	CHAOS_TYPE_IMPERIALGUARDIANTICKET						= 46,
	CHAOS_TYPE_NEWBOX_MIX									= 47,
	CHAOS_TYPE_UPGRADE_14									= 49,
	CHAOS_TYPE_UPGRADE_15									= 50,
	CHAOS_TYPE_LUCKYITEM_CREATE								= 51,
	CHAOS_TYPE_LUCKYITEM_REFINE								= 52
};


struct PMSG_CHAOSMIXRESULT
{
	PBMSG_HEAD h;	// C1:86
	BYTE Result;	// 3
	BYTE ItemInfo[MAX_ITEM_INFO];	// 4
};





struct CB_PMSG_REQ_PRESENT_RESULT
{
	PBMSG_HEAD h;
	char AccountId[10];	// 3
	char GameId[10];	// D
	int Sequence;	// 18
	char present;	// 1C
	char presentname[50];	// 1D
	int Seq_present;	// 50
};


struct CBUPS_ITEMPRIZESEND	// Confirmed size
{
	PHEADB PHeader;
	char szAccountID[10];
	char szCharName[10];
	int iGameServerCode;
	BYTE btResult;
	int unk20;

};



#define CB_ERROR					0
#define CB_SUCCESS					1
#define CB_NOT_ENOUGH_ZEN			2
#define CB_TOO_MANY_ITEMS			3
#define CB_LOW_LEVEL_USER			4
#define CB_LACKING_MIX_ITEMS		6
#define CB_INCORRECT_MIX_ITEMS		7
#define CB_INVALID_ITEM_LEVEL		8
#define CB_USER_CLASS_LOW_LEVEL		9
#define CB_NO_BC_CORRECT_ITEMS		10
#define CB_BC_NOT_ENOUGH_ZEN		11



BOOL ChaosBoxCheck(LPOBJ lpObj);
BOOL ChaosBoxInit(LPOBJ lpObj);
BOOL ChaosBoxItemDown(LPOBJ lpObj);
int ChaosBoxMix(LPOBJ lpObj, int & Result2);
void CBUPS_ItemRequest(int aIndex, char* AccountId, char* Name);
void ChaosEventProtocolCore(BYTE protoNum, unsigned char* aRecv, int aLen);
void CBUPR_ItemRequestRecv( CB_PMSG_REQ_PRESENT_RESULT * lpMsg);
int  CheckDevilSquareItem(LPOBJ lpObj, int & eventitemcount, int & itemlevel);
void LogDQChaosItem(LPOBJ lpObj);
BOOL DevilSquareEventChaosMix(LPOBJ lpObj, BOOL bCheckType, int iItemLevel);
void LogPlusItemLevelChaosItem(LPOBJ lpObj, int iPlusMixLevel);
void LogChaosItem(LPOBJ lpObj, LPSTR sLogType);
BOOL PlusItemLevelChaosMix(LPOBJ lpObj, int mixType);
BOOL PegasiaChaosMix(LPOBJ lpObj);
BOOL CircleChaosMix(LPOBJ lpObj);
BOOL WingChaosMix(LPOBJ lpObj);
BOOL Wing3ChaosMix(LPOBJ lpObj);
BOOL CheckImperialGuardianItem(LPOBJ lpObj);
void ImperialGuardianItemChaosMix(LPOBJ lpObj);
void DefaultChaosMix(LPOBJ lpObj);
void DevilSquareItemChaosMix(LPOBJ lpObj);
BOOL ImperialGuardianEventChaosMix(LPOBJ lpObj);
void BloodCastleItemChaosMix(LPOBJ lpObj);
void SetItemChaosMix(LPOBJ lpObj);
void DarkHorseChaosMix(LPOBJ lpObj);
void DarkSpiritChaosMix(LPOBJ lpObj);
void BlessPotionChaosMix(LPOBJ lpObj);
void SoulPotionChaosMix(LPOBJ lpObj);
void LifeStoneChaosMix(LPOBJ lpObj);
#if (GS_CASTLE==1)
void CastleSpecialItemMix(LPOBJ lpObj);
#endif
void HiddenTreasureBoxItemMix(LPOBJ lpObj);
void Fenrir_01Level_Mix(LPOBJ lpObj);
void Fenrir_02Level_Mix(LPOBJ lpObj);
void Fenrir_03Level_Mix(LPOBJ lpObj);
void Fenrir_04Upgrade_Mix(LPOBJ lpObj);
void ShieldPotionLv1_Mix(LPOBJ lpObj);
void ShieldPotionLv2_Mix(LPOBJ lpObj);
void ShieldPotionLv3_Mix(LPOBJ lpObj);
void LotteryItemMix(LPOBJ lpObj);
void ExtractSeedChaosMix(LPOBJ lpObj);
void SocketSeedChaosMix(LPOBJ lpObj);
void SocketItemAddChaosMix(LPOBJ lpObj, BYTE SocketSlot);
void SocketItemDelChaosMix(LPOBJ lpObj, BYTE SocketSlot);
void IllusionTempleItemMix(LPOBJ lpObj);
void NewBoxItemChaosMix(LPOBJ lpObj);
void Box_LuckyItemsRefine(LPOBJ lpObj);
void LuckyCardItemChaosMix(LPOBJ lpObj);

#endif