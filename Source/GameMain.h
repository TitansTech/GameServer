// ------------------------------
// Decompiled by Deathway
// Date : 2007-03-09
// ------------------------------
//------------------------------------------
// GameMain.h
//------------------------------------------

#ifndef GAMEMAIN_H
#define	GAMEMAIN_H


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "stdafx.h"
#include "wsJoinServerCli.h"
#include "wsGameServer.h"
#include "DirPath.h"
#include "MapClass.h"
#include "NSerialCheck.h"
#include "..\common\classdef.h"
#include "CLoginCount.h"
#include "DragonEvent.h"
#include "AttackEvent.h"
#include "WzUdp.h"
#include "WhisperCash.h"
#include "DbSave.h"
#include "SimpleModulus.h"
#include "MonsterAttr.h"
#include "MonsterSetBase.h"
#include "PartyClass.h"
#include "ItemBagEx.h"

#if (GS_CASTLE==1)
#include "CastleSiege.h"
#include "CastleDeepEvent.h"
#include "Crywolf.h"
#endif

#define FIRST_PATH "c:\\muproject\\data\\"


struct PMSG_FRIEND_STATE
{
	PBMSG_HEAD h;	// C1:C4
	char Name[10];	// 3
	BYTE State;	// D
};




enum MU_EVENT_TYPE {
	MU_EVENT_ALL = 0x0,
	MU_EVENT_DEVILSQUARE = 0x1,
	MU_EVENT_BLOODCASTLE = 0x2,
	MU_EVENT_ATTACKEVENTTROOP = 0x3,
	MU_EVENT_GOLDENTROOP = 0x4,
	MU_EVENT_WHITEMAGETROOP = 0x5,
	MU_EVENT_LOVEPANGPANG = 0x6,
	MU_EVENT_FIRECRACKER = 0x7,
	MU_EVENT_MEDALION = 0x8,
	MU_EVENT_XMASSTAR = 0x9,
	MU_EVENT_HEARTOFLOVE = 0xa,
	MU_EVENT_SAY_HAPPYNEWYEAR = 0xb,
	MU_EVENT_SAY_MERRYXMAS = 0xc,
	MU_EVENT_CHAOSCASTLE = 0xd,
	MU_EVENT_CHRISTMAS_RIBBONBOX = 0xe,
	MU_EVENT_VALENTINESDAY_CHOCOLATEBOX = 0xf,
	MU_EVENT_WHITEDAY_CANDYBOX = 0x10,
};



enum MU_ETC_TYPE {
	MU_ETC_ALL = 0x0,
	MU_ETC_CREATECHARACTER = 0x1,
	MU_ETC_GUILD = 0x2,
	MU_ETC_TRADE = 0x3,
	MU_ETC_USECHAOSBOX = 0x4,
	MU_ETC_PERSONALSHOP = 0x5,
	MU_ETC_PKITEMDROP = 0x6,
	MU_ETC_ITEMDROPRATE = 0x7,
	MU_ETC_SPEEDHACK = 0x8,
	MU_ETC_GAMEGUARD = 0x9,
};


extern BOOL JoinServerConnected;
extern BOOL DataServerConnected;
extern BOOL GameServerCreated;
extern BOOL DevilSquareEventConnect;
extern BOOL IsDevilSquareEventConnected;
extern BOOL EventChipServerConnect;
extern BOOL IsEventChipServerConnected;
extern CDragonEvent * DragonEvent;
extern CAttackEvent * AttackEvent;
extern CItemBag * LuckboxItemBag;
extern CItemBag * Mon55;
extern CItemBag * Mon53;
extern CItemBagEx * StarOfXMasItemBag;
extern CItemBag * FireCrackerItemBag;
extern CItemBag * HeartOfLoveItemBag;
extern CItemBag * IllusionItemBag;
extern CItemBag * GoldMedalItemBag;
extern CItemBag * SilverMedalItemBag;
extern CItemBag * EventChipItemBag;
extern CItemBag * GoldGoblenItemBag;
extern CItemBag * TitanItemBag;
extern CItemBag * GoldDerconItemBag;
extern CItemBag * DevilLizardKingItemBag;
extern CItemBag * KanturItemBag;
extern CItemBag * RingEventItemBag;
extern CItemBag * FriendShipItemBag;
extern CItemBag * DarkLordHeartItemBag;
#if (GS_CASTLE==1)
extern CItemBagEx * ErohimCastleZoneItemBag;
extern CItemBagEx * HuntZoneItemBag;
extern CItemBagEx * CastleItemMixItemBag;
#endif
extern CItemBagEx * KundunEventItemBag;
extern CItemBagEx * SelupanEventItemBag;
#if (PACK_EDITION>=1)
extern CItemBagEx * BlueEventItemBag;
extern CItemBagEx * SummerEventItemBag;
#endif
#if (PACK_EDITION>=2)
extern CItemBagEx * XMasEventItemBag;
#endif
#if (PACK_EDITION>=3)
extern CItemBagEx * SwampEventItemBag;
extern CItemBagEx * BossAttackItemBag;
#endif
extern CItemBagEx * HiddenTreasureBoxItemBag;
extern CItemBagEx * RedRibbonBoxEventItemBag;
extern CItemBagEx * GreenRibbonBoxEventItemBag;
extern CItemBagEx * BlueRibbonBoxEventItemBag;
extern CItemBagEx * PinkChocolateBoxEventItemBag;
extern CItemBagEx * RedChocolateBoxEventItemBag;
extern CItemBagEx * BlueChocolateBoxEventItemBag;
extern CItemBagEx * LightPurpleCandyBoxEventItemBag;
extern CItemBagEx * VermilionCandyBoxEventItemBag;
extern CItemBagEx * DeepBlueCandyBoxEventItemBag;
extern CItemBagEx * CrywolfDarkElfItemBag;
extern CItemBagEx * CrywolfBossMonsterItemBag;
extern CItemBagEx * CrywolfPedestalRewardItemBag;
extern CItemBagEx * KanturuMayaHandItemBag;
extern CItemBagEx * KanturuNightmareItemBag;
extern CItemBagEx * HellMainItemBag;
extern CItemBagEx * HalloweenDayEventItemBag;
extern CItemBagEx * GreenMysteryEventItemBag;
extern CItemBagEx * RedMysteryEventItemBag;
extern CItemBagEx * PurpleMysteryEventItemBag;
extern CItemBagEx * CherryBlossomEventItemBag;
extern CItemBagEx * GMEventItemBag;

//ImperialGuardian
extern CItemBagEx * IGStatueItemBag;
extern CItemBagEx * IGSundayItemBag1;
extern CItemBagEx * IGSundayItemBag2;
extern CItemBagEx * IGMondayItemBag;
extern CItemBagEx * IGTuesdayItemBag;
extern CItemBagEx * IGWednesdayItemBag;
extern CItemBagEx * IGThursdayItemBag;
extern CItemBagEx * IGFridayItemBag;
extern CItemBagEx * IGSaturdayItemBag;

#if (PACK_EDITION>=2)
extern CItemBagEx * HalloweenPKEventItemBag;
#endif

//DoubleGoer
extern CItemBagEx * DGBoss1ItemBag;
extern CItemBagEx * DGBoss2ItemBag;
extern CItemBagEx * DGBoss3ItemBag;
extern CItemBagEx * DGTreasureItemBag;
extern CItemBagEx * DGSilverTreasureItemBag;


extern CItemBag * GoldDarkKnightItemBag;
extern CItemBag * GoldDevilItemBag;
extern CItemBag * GoldStoneGolemItemBag;
extern CItemBag * GoldCrustItemBag;
extern CItemBag * GoldSatyrosItemBag;
extern CItemBag * GoldTwinTaleItemBag;
extern CItemBag * GoldIronKnightItemBag;
extern CItemBag * GoldNapinItemBag;
extern CItemBag * GoldGreatDragonItemBag;
extern CItemBag * GoldRabbitItemBag;

extern CItemBagEx * FortunePouchItemBag;

extern CItemBagEx * ElegantJewerlyItemBag;
extern CItemBagEx * MetalJewerlyItemBag;
extern CItemBagEx * OldJewerlyItemBag;
extern CItemBagEx * GoldBoxItemBag;
extern CItemBagEx * SilverBoxItemBag;
extern CItemBagEx * RainItemsEvent;


extern CItemBagEx * GreenBoxEventItemBag;
extern CItemBagEx * RedBoxEventItemBag;
extern CItemBagEx * PurpleBoxEventItemBag;

extern BOOL SpeedHackPlayerBlock;
extern BOOL bCanConnectMember;
extern BOOL gEnableEventNPCTalk;
extern BOOL gEnableServerDivision;
extern BOOL gEvent1;
extern int  gMonsterHp;
extern int  gEvent1ItemDropTodayCount;
extern BOOL gChaosEvent;
extern char gChaosEventServerIp[20];
extern char gDevilSquareEventServerIp[20];
extern char gEventChipServerIp[20];

//extern BYTE gXMasEvent;
//extern BYTE gMerryXMasNpcEvent;
//extern BYTE gHappyNewYearNpcEvent;

extern BOOL gFireCrackerEvent;
extern BOOL gHeartOfLoveEvent;
extern BOOL gMedalEvent;
extern BOOL gEventChipEvent;
extern BOOL gDevilSquareEvent;
extern BOOL gWriteSkillLog;
extern BOOL g_bDoRingEvent;
extern BOOL g_bEventManagerOn;
extern int  g_iKundunMarkDropRate;
extern int  g_iMarkOfTheLord;
extern int g_iJapan1StAnivItemDropRate;
extern int  g_iDarkLordHeartDropRate;
extern int  g_iDarkLordHeartOffEventRate;
extern int g_iMysteriousBeadDropRate1;
extern int g_iMysteriousBeadDropRate2;
extern int g_iHiddenTreasureBoxOfflineRate;
extern BOOL bIsIgnorePacketSpeedHackDetect;
extern BOOL gIsKickDetecHackCountLimit;
extern BOOL gTamaJJangEvent;
extern int gAppearTamaJJang;
extern int gTamaJJangTime;
extern BOOL gIsItemDropRingOfTransform;
extern BOOL gIsEledoradoEvent;
extern BOOL gIsEledorado2Event;
extern BOOL gDoPShopOpen;
extern BOOL gDisconnectHackUser;
extern int g_iBlockKanturuMapEnter;
extern int g_iBlockCastleSiegeMapEnter;
extern BOOL GSInfoSendFlag;					//USELESS VARIABLE
extern int  GameServerPort;
extern int  JoinServerPort;
extern int  DataServerPort;
extern int  DataServerPort2;
extern int  ExDbPort;
extern CwsGameServer wsGServer;	// line : 213GameServer
extern wsJoinServerCli wsJServerCli;	// line : 214 Join Server
extern wsJoinServerCli wsDataCli;	// line : 215 DataServer
extern wsJoinServerCli wsExDbCli;	// line : 239 Extra DataBase Server
extern wsJoinServerCli wsRServerCli;	// line : 244 Ranking Server
extern wsJoinServerCli wsEvenChipServerCli; // line : 246 EVENT_MU2003
extern CDirPath gDirPath;	// line : 248 Directory Path
extern MapClass MapC[MAX_MAP_NUMBER];	// line 249	// Map Manager
extern CMonsterAttr gMAttr;	// line 250
extern CMonsterSetBase gMSetBase;	// line 251
extern classdef DCInfo;	// line 252:
extern CWhisperCash WhisperCash;	// line 253
extern PartyClass gParty;	// line 254
extern CDbSave gDbSave;	// line 255
extern WzUdp gUdpSoc;	// line 256
extern WzUdp gUdpSocCE;	// line 258
extern WzUdp gUdpSocCER;	// line 259
extern WzUdp gUdpSocCRank;	// line 261
extern WzUdp gUdpSocCRankR;	//line 262
extern CGuildClass Guild;	// line 265
extern CMsg lMsg;	// line 324
extern NSerialCheck gNSerialCheck[OBJMAX];	// line 326
extern CLoginCount gLCount[3];	// line 329
extern DWORD  gLevelExperience[MAX_CHAR_LEVEL+1];
extern char szGameServerExeSerial[24];
extern char szServerName[50];
extern BOOL gIsDropDarkLordItem;
extern int  gSleeveOfLordDropRate;
extern int  gSleeveOfLordDropLevel;
extern int  gSoulOfDarkHorseDropRate;
extern int  gSoulOfDarkHorseropLevel;
extern int  gSoulOfDarkSpiritDropRate;
extern int  gSoulOfDarkSpiritDropLevel;
extern float gDarkSpiritAddExperience;
extern BOOL gIsDropGemOfDefend;
extern int  gGemOfDefendDropRate;
extern int  gGemOfDefendDropLevel;
extern int  g_iUseCharacterAutoRecuperationSystem;
extern int  g_iCharacterRecuperationMaxLevel;
extern int g_iServerGroupGuildChatting;
extern int g_iServerGroupUnionChatting;

extern BOOL g_bChocolateBoxEvent;
extern BOOL g_bCandyBoxEvent;
extern BOOL g_bMysteryBoxEvent;

extern BOOL g_bFenrirStuffItemDrop;
extern int g_iFenrirStuff_01_DropLv_Min;
extern int g_iFenrirStuff_01_DropLv_Max;
extern int g_iFenrirStuff_01_DropMap;
extern int g_iFenrirStuff_01_DropRate;
extern int g_iFenrirStuff_02_DropLv_Min;
extern int g_iFenrirStuff_02_DropLv_Max;
extern int g_iFenrirStuff_02_DropMap;
extern int g_iFenrirStuff_02_DropRate;
extern int g_iFenrirStuff_03_DropLv_Min;
extern int g_iFenrirStuff_03_DropLv_Max;
extern int g_iFenrirStuff_03_DropMap;
extern int g_iFenrirStuff_03_DropRate;
extern int g_iFenrirRepairRate;
extern int g_iFenrirDefaultMaxDurSmall;
extern int g_iFenrirElfMaxDurSmall;
extern BOOL g_bCrywolfMonsterDarkElfItemDrop;
extern BOOL g_bCrywolfBossMonsterItemDrop;
extern int g_iCrywolfApplyMvpBenefit;
extern int g_iCrywolfApplyMvpPenalty;
extern int g_iSkillDistanceCheck;
extern int g_iSkillDistanceCheckTemp;
extern int g_iSkillDistanceKick;
extern int g_iSkillDistanceKickCount;
extern int g_iSkillDiatanceKickCheckTime;
extern BOOL g_bKanturuMayaHandItemDrop;
extern BOOL g_bKanturuNightmareItemDrop;
extern BOOL g_bHellMainItemDrop;
extern BOOL g_bKanturuSpecialItemDropOn;
extern int g_iKanturuMoonStoneDropRate;
extern int g_iKanturuJewelOfHarmonyDropRate;
extern BOOL g_bHalloweenDayEventOn;
extern int g_iHalloweenDayEventJOLBlessDropRate;
extern int g_iHalloweenDayEventJOLAngerDropRaTe;
extern int g_iHalloweenDayEventJOLScreamDropRate;
extern int g_iHalloweenDayEventJOLFoodDropRate;
extern int g_iHalloweenDayEventJOLDrinkDropRate;
extern int g_iHalloweenDayEventJOLPolymorphRingDropRate;
extern BYTE g_iWings3ReturnDamageSuccessRate;
extern BYTE g_iWings3RecoverFullLifeSuccessRate;
extern BYTE g_iWings3RecoverFullManaSuccessRate;
extern BYTE g_iWings3SuccessfullBlockingRate;
extern DWORD dwgCheckSum[MAX_CHECKSUM_KEY];
extern char connectserverip[20];
extern int  connectserverport;
extern short gGameServerCode;
extern BOOL gStalkProtocol;
extern int  g_iRingOrcKillGiftRate;
extern int  g_iRingDropGiftRate;
extern int  g_iRingDropItemWhiteWizType;
extern int  g_iRingDropItemWhiteWizIndex;
extern int  g_iRingEventOrcRewardDropRate;
extern int  g_iRingDropItemOrcType;
extern int  g_iRingDropItemOrcIndex;
extern CSimpleModulus g_SimpleModulusCS;	// line 751
extern CSimpleModulus g_SimpleModulusSC;	// line 752
extern int gEledorado2EventItemDropRate[10];
extern int gEledorado2EventExItemDropRate[10];
extern int  gEledoradoGoldGoblenItemDropRate;
extern int  gEledoradoTitanItemDropRate;
extern int  gEledoradoGoldDerconItemDropRate;
extern int  gEledoradoDevilLizardKingItemDropRate;
extern int  gEledoradoDevilTantarosItemDropRate;
extern int  gEledoradoGoldGoblenExItemDropRate;
extern int  gEledoradoTitanExItemDropRate;
extern int  gEledoradoGoldDerconExItemDropRate;
extern int  gEledoradoDevilLizardKingExItemDropRate;
extern int  gEledoradoDevilTantarosExItemDropRate;
extern int  giKundunRefillHPSec;
extern int  giKundunRefillHP;
extern int  giKundunRefillHPTime;
extern int  giKundunHPLogSaveTime;
extern BOOL gUseNPGGChecksum;
extern int g_ShieldSystemOn;
extern int g_iDamageDevideToSDRate;
extern int g_iDamageDevideToHPRate;
extern float g_fSuccessAttackRateOption;
extern int g_ShieldAutoRefillOn;
extern int g_ShieldAutoRefillOnSafeZone;
extern int g_CompoundPotionDropOn;
extern int g_iCompoundPotionLv1DropRate;
extern int g_iCompoundPotionLv2DropRate;
extern int g_iCompoundPotionLv3DropRate;
extern int g_iCompoundPotionLv1DropLevel;
extern int g_iCompoundPotionLv2DropLevel;
extern int g_iCompoundPotionLv3DropLevel;
extern BOOL g_bShieldComboMissOptionOn;
extern int g_iShieldGageConstA;
extern int g_iShieldGageConstB;
extern char gMapName[MAX_MAP_NUMBER][25];
extern char szClientVersion[8];
extern BOOL bCanTrade;
extern BOOL bCanChaosBox;
extern BOOL bCanWarehouseLock;
extern short  MapMinUserLevel[MAX_MAP_NUMBER];
extern BOOL gEnableBattleSoccer;
extern int  gLootingTime;
extern int  gItemDropPer;
extern int  gEvent1ItemDropTodayMax;
extern int  gEvent1ItemDropTodayPercent;
extern BOOL gCreateCharacter;
extern BOOL gGuildCreate;
extern BOOL gGuildDestroy;
extern int  gGuildCreateLevel;
extern BOOL gItemSerialCheck;
extern BOOL gItemZeroSerialCheck;
extern float  gAddExperience;
extern int  gFireCrackerDropRate;
extern int  g_ItemDropRateForgFireCracker;
extern int  gHeartOfLoveDropRate;
extern int  g_ItemDropRateForgHeartOfLove;
extern int  gGoldMedalDropRate;
extern int  gSilverMedalDropRate;
extern int  g_ItemDropRateForGoldMedal;
extern int  g_ItemDropRateForSilverMedal;
extern int  gBoxOfGoldDropRate;
extern int  gBoxOfGoldMinMobLevel;
extern int  gBoxOfGoldMaxMobLevel;
extern int  g_ItemDropRateForBoxOfGold;
extern int  g_ItemEXDropRateForBoxOfGold;
extern int  g_EventChipDropRateForBoxOfGold;
extern int  gEyesOfDevilSquareDropRate;
extern int  gKeyOfDevilSquareDropRate;
extern BOOL g_bBloodCastle;
extern int  g_iBloodCastle_StartHour;
extern int  g_iAngelKingsPaperDropRate;
extern int  g_iBloodBoneDropRate;
extern int g_iNpcAngelKingRemainTime;
extern int g_iBCDropChaosGemItemType;
extern int g_iBCDropChaosGemItemIndex;
extern int g_iBCDropChaosGemItemLevel;
extern int g_iBCDropChaosGemItemDur;
extern int g_iBCDropChaosGemItemSkill;
extern int g_iBCDropChaosGemItemLuck;
extern int g_iBCDropChaosGemItemOpt;
extern int g_iBCDropChaosGemItemExc;
extern BOOL g_bChaosCastle;

extern BYTE g_iCheckCanStartPlayCCMinPlayers;
extern BYTE g_iGiveWinnerItemCCType1;
extern short g_iGiveWinnerItemCCIndex1;
extern BYTE g_iGiveWinnerItemCCType2;
extern short g_iGiveWinnerItemCCIndex2;
extern BYTE g_iGiveWinnerItemCCType3;
extern short g_iGiveWinnerItemCCIndex3;
extern BYTE g_iGiveWinnerItemCCType4;
extern short g_iGiveWinnerItemCCIndex4;

extern BYTE g_bCC1SetDropRate;
extern BYTE g_bCC2SetDropRate;
extern BYTE g_bCC3SetDropRate;
extern BYTE g_bCC4SetDropRate;
extern BYTE g_bCC5SetDropRate;
extern BYTE g_bCC6SetDropRate;
extern BYTE g_bCC7SetDropRate;

extern BYTE g_bCC1TypeDropRate;
extern BYTE g_bCC2TypeDropRate;
extern BYTE g_bCC3TypeDropRate;
extern BYTE g_bCC4TypeDropRate;
extern BYTE g_bCC5TypeDropRate;
extern BYTE g_bCC6TypeDropRate;
extern BYTE g_bCC7TypeDropRate;

extern DWORD  gAttackSpeedTimeLimit;
extern DWORD  gHackCheckCount;
extern float gDecTimePerAttackSpeed;
extern int  gMinimumAttackSpeedTime;
extern int  gDetectedHackKickCount;
extern int gTamaJJangKeepTime;
extern int gTamaJJangDisappearTime;
extern int gTamaJJangDisappearTimeRandomRange;
extern BOOL gItemDropRingOfTransform;
extern int  gQuestNPCTeleportTime;
extern DWORD  gEledoradoGoldGoblenRegenTime;
extern DWORD  gEledoradoTitanRegenTime;
extern DWORD  gEledoradoGoldDerconRegenTime;
extern DWORD  gEledoradoDevilLizardKingRegenTime;
extern DWORD  gEledoradoDevilTantarosRegenTime;
extern DWORD  gEledorado2EventRegenTime[10];
extern int  gZenDurationTime;
extern BOOL gEnableCheckPenetrationSkill;

extern int gIsDropSetItemInCastleHuntZone;
extern int gSetItemInCastleHuntZoneDropRate;
extern int gSetItemInCastleHuntZoneDropLevel;
extern BOOL g_bDoCastleDeepEvent;

#if (GS_CASTLE==1)
extern CCastleSiege g_CastleSiege;
extern CCastleDeepEvent g_CastleDeepEvent;
extern int g_iGemOfDefendRate;
extern int g_iCastleItemMixLimit;
#endif

//------------------------------------------
// GameMain.cpp Functions - Prototypes List - Completed
//------------------------------------------
void GameMainInit(HWND hWnd);
int GetWarehouseUsedHowMuch(int UserLevel, BOOL IsLock);
void GraphPaint(HWND hWnd);
void GameMonsterAllAdd();
void GameMonsterAllCloseAndReLoad();
void GameMainFree();
BOOL GMJoinServerConnect(char* ConnectServer, DWORD wMsg);
BOOL GMRankingServerConnect(char* RankingServer, DWORD wMsg);
BOOL GMEventChipServerConnect(char* ServerIP, DWORD wMsg);
BOOL GMDataServerConnect(char* ConnectServer, WPARAM wMsg);
BOOL ExDataServerConnect(char* ConnectServer, DWORD wMsg);
BOOL GameMainServerCreate(DWORD sMsg, DWORD cMsg);
void GMServerMsgProc( WPARAM wParam, LPARAM lParam);
void GMClientMsgProc( WPARAM wParam, LPARAM lParam);
void GMJoinClientMsgProc(WPARAM wParam, LPARAM lParam);
void GMRankingClientMsgProc(WPARAM wParam, LPARAM lParam);
void GMEventChipClientMsgProc(WPARAM wParam, LPARAM lParam);
void ExDataClientMsgProc(WPARAM wParam, LPARAM lParam);
void GMDataClientMsgProc(WPARAM wParam, LPARAM lParam);
void ReadServerInfo();
void ReadCommonServerInfo();
void GameServerInfoSendStop();
void GameServerInfoSendStart();
void GameServerInfoSend();
void CheckSumFileLoad(char * szCheckSum);
void LoadItemBag();
void SetMapName();
//int GetEventFlag();
void ReadEventInfo(MU_EVENT_TYPE eEventType);
void ReadGameEtcInfo(MU_ETC_TYPE eGameEtcType);
#endif