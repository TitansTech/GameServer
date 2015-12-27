// ------------------------------
// Decompiled by Deathway
// Date : 2007-03-09
// ------------------------------
//------------------------------------------
// GameMain.cpp
//------------------------------------------
// GS-N 0.99.60T 0x004F4490
//	GS-N	1.00.18	JPN	0x0051FE00	-	Completed
#include "stdafx.h"
#include "AcceptIp.h"
#include "gamemain.h"
#include "..\common\winutil.h"
#include "GameServer.h"
#include "DirPath.h"
#include "logproc.h"
////#include "GameServerAuth.h"
#include "wsJoinServerCli.h"
#include "DirPath.h"
#include "DSProtocol.h"
#include "CLoginCount.h"
#include "MapClass.h"
#include "..\common\classdef.h"
#include "..\include\prodef.h"
#include "wsGameServer.h"
#include "EledoradoEvent.h"
#include "WhisperCash.h"
#include "SProtocol.h"
#include "ChaosCastle.h"
#include "DevilSquare.h"
#include "Gate.h"
#include "user.h"
#include "Event.h"
#include "PacketCheckSum.h"
#include "DragonEvent.h"
#include "AttackEvent.h"
#include "SimpleModulus.h"
#include "MapServerManager.h"
#include "..\ggsvr\ggsvr.h"
#include "QuestInfo.h"
#include "SkillHitBox.h"
#include "EventManagement.h"
#include "..\common\SetItemOption.h"
#include "EDSProtocol.h"
#include "MonsterItemMng.h"
#include "ConMember.h"
#include "GMMng.h"
#include "BattleSoccerManager.h"
#include "GuildClass.h"
#include "MoveCommand.h"
#include "ChaosBox.h"
#include "BloodCastle.h"
#include "Shop.h"
#include "ItemAddOption.h"
#include "CrywolfSync.h"
#include "Kanturu.h"
#include "KanturuMonsterMng.h"
#include "SkillAdditionInfo.h"
#include "TMonsterSkillElement.h"
#include "TMonsterSkillUnit.h"
#include "TMonsterSkillManager.h"
#include "TMonsterAIElement.h"
#include "TMonsterAIAutomata.h"
#include "TMonsterAIUnit.h"
#include "TMonsterAIRule.h"
#include "TMonsterAIGroup.h"
#include "CashShop.h"
#include "CashItemPeriodSystem.h"
#include "CashLotterySystem.h"
#include "Raklion.h"
#include "IllusionTemple.h"
#include "SwampEvent.h"
#include "BlueEvent.h"
#include "BossAttack.h"
#include "SkyEvent.h"
#include "SummerEvent.h"
#include "MossShop.h"
#include "ImperialGuardian.h"
#include "DoppelGanger.h"
#include "XMasEvent.h"
#include "CSAuth/CSAuth.h"
#include "SCFPvPSystem.h"
#include "ObjBotStore.h"
#include "ObjBotBuffer.h"
#include "ObjBotAlchemist.h"
#include "ObjBotTrader.h"
#include "ObjBotVipShop.h"
#include "ObjBotReward.h"
#include "ObjBotWarper.h"
#include "ObjBotRacer.h"

#include "DSGN_ItemMover.h"
//***********************************************
// Security Stuff, new implementation on the way.
//***********************************************
//#include "AntiRE.h"

//+-----------------------------------------
//	Castle Siege
//+-----------------------------------------
#if (GS_CASTLE==1)
	#include "CastleDeepEvent.h"
#endif

//------------------------------------------
// GameMain.cpp Variables
//------------------------------------------
BOOL JoinServerConnected;
BOOL DataServerConnected;
BOOL GameServerCreated;
BOOL DevilSquareEventConnect;
BOOL IsDevilSquareEventConnected;
BOOL EventChipServerConnect;
BOOL IsEventChipServerConnected;

CDragonEvent * DragonEvent;
CAttackEvent * AttackEvent;

CItemBag * LuckboxItemBag;
CItemBag * Mon55;
CItemBag * Mon53;
CItemBag * FireCrackerItemBag;
CItemBag * HeartOfLoveItemBag;
CItemBag * IllusionItemBag;
CItemBag * GoldMedalItemBag;
CItemBag * SilverMedalItemBag;
CItemBag * EventChipItemBag;
CItemBag * GoldGoblenItemBag;
CItemBag * TitanItemBag;
CItemBag * GoldDerconItemBag;
CItemBag * DevilLizardKingItemBag;
CItemBag * KanturItemBag;
CItemBag * RingEventItemBag;
CItemBag * FriendShipItemBag;
CItemBag * DarkLordHeartItemBag;

CItemBagEx * StarOfXMasItemBag;
CItemBagEx * KundunEventItemBag;
CItemBagEx * SelupanEventItemBag;
#if (PACK_EDITION>=1)
CItemBagEx * BlueEventItemBag;
CItemBagEx * SummerEventItemBag;
#endif
#if (PACK_EDITION>=3)
CItemBagEx * SwampEventItemBag;
CItemBagEx * BossAttackItemBag;
#endif
CItemBagEx * HiddenTreasureBoxItemBag;
CItemBagEx * RedRibbonBoxEventItemBag;
CItemBagEx * GreenRibbonBoxEventItemBag;
CItemBagEx * BlueRibbonBoxEventItemBag;
CItemBagEx * PinkChocolateBoxEventItemBag;
CItemBagEx * RedChocolateBoxEventItemBag;
CItemBagEx * BlueChocolateBoxEventItemBag;
CItemBagEx * LightPurpleCandyBoxEventItemBag;
CItemBagEx * VermilionCandyBoxEventItemBag;
CItemBagEx * DeepBlueCandyBoxEventItemBag;
CItemBagEx * CrywolfDarkElfItemBag;
CItemBagEx * CrywolfBossMonsterItemBag;
CItemBagEx * CrywolfPedestalRewardItemBag;
CItemBagEx * KanturuMayaHandItemBag;
CItemBagEx * KanturuNightmareItemBag;
CItemBagEx * HellMainItemBag;
CItemBagEx * HalloweenDayEventItemBag;
CItemBagEx * GreenMysteryEventItemBag;
CItemBagEx * RedMysteryEventItemBag;
CItemBagEx * PurpleMysteryEventItemBag;
CItemBagEx * CherryBlossomEventItemBag;
CItemBagEx * GMEventItemBag;
//ImperialGuardian
CItemBagEx * IGStatueItemBag;
CItemBagEx * IGSundayItemBag2;
CItemBagEx * IGSundayItemBag1;
CItemBagEx * IGMondayItemBag;
CItemBagEx * IGTuesdayItemBag;
CItemBagEx * IGWednesdayItemBag;
CItemBagEx * IGThursdayItemBag;
CItemBagEx * IGFridayItemBag;
CItemBagEx * IGSaturdayItemBag;
#if (PACK_EDITION>=2)

CItemBagEx * HalloweenPKEventItemBag;
#endif
//DoubleGoer
CItemBagEx * DGBoss1ItemBag;
CItemBagEx * DGBoss2ItemBag;
CItemBagEx * DGBoss3ItemBag;
CItemBagEx * DGTreasureItemBag;
CItemBagEx * DGSilverTreasureItemBag;

CItemBag * GoldDarkKnightItemBag;
CItemBag * GoldDevilItemBag;
CItemBag * GoldStoneGolemItemBag;
CItemBag * GoldCrustItemBag;
CItemBag * GoldSatyrosItemBag;
CItemBag * GoldTwinTaleItemBag;
CItemBag * GoldIronKnightItemBag;
CItemBag * GoldNapinItemBag;
CItemBag * GoldGreatDragonItemBag;
CItemBag * GoldRabbitItemBag;

#if (PACK_EDITION>=1)
CItemBagEx * XMasEventItemBag;
#endif
CItemBagEx * FortunePouchItemBag;

CItemBagEx * ElegantJewerlyItemBag;
CItemBagEx * MetalJewerlyItemBag;
CItemBagEx * OldJewerlyItemBag;
CItemBagEx * GoldBoxItemBag;
CItemBagEx * SilverBoxItemBag;
CItemBagEx * RainItemsEvent;
CItemBagEx * GreenBoxEventItemBag;
CItemBagEx * RedBoxEventItemBag;
CItemBagEx * PurpleBoxEventItemBag;

BOOL SpeedHackPlayerBlock;
BOOL bCanConnectMember;
BOOL gEnableEventNPCTalk;
BOOL gEnableServerDivision;
BOOL gEvent1;
int  gMonsterHp;

int  gEvent1ItemDropTodayCount;
BOOL gChaosEvent;
char gChaosEventServerIp[20];
char gDevilSquareEventServerIp[20];
char gEventChipServerIp[20];

//BYTE gXMasEvent;
//BYTE gMerryXMasNpcEvent;
//BYTE gHappyNewYearNpcEvent;

BOOL gFireCrackerEvent;
BOOL gHeartOfLoveEvent;
BOOL gMedalEvent;
BOOL gEventChipEvent;
BOOL gDevilSquareEvent;
BOOL gWriteSkillLog;
BOOL g_bDoRingEvent;
BOOL g_bEventManagerOn;
BOOL g_bSCFEventManagerOn;
int  g_iKundunMarkDropRate;
int  g_iMarkOfTheLord;
int g_iJapan1StAnivItemDropRate;
int  g_iDarkLordHeartDropRate;
int  g_iDarkLordHeartOffEventRate;
int g_iMysteriousBeadDropRate1;
int g_iMysteriousBeadDropRate2;
int g_iHiddenTreasureBoxOfflineRate;
BOOL bIsIgnorePacketSpeedHackDetect;
BOOL gIsKickDetecHackCountLimit;
BOOL gTamaJJangEvent;
int gAppearTamaJJang;
int gTamaJJangTime;
BOOL gIsItemDropRingOfTransform;
BOOL gIsEledoradoEvent;
BOOL gIsEledorado2Event;
BOOL gDoPShopOpen;
BOOL gDisconnectHackUser;
int g_iBlockKanturuMapEnter;
int g_iBlockCastleSiegeMapEnter;
BOOL GSInfoSendFlag;




///////////////////////////////////////////////////////////////////////////////



int  GameServerPort;
int  JoinServerPort;
int  DataServerPort;
int  DataServerPort2;
int  ExDbPort;

CwsGameServer wsGServer;	// line : 213GameServer
wsJoinServerCli wsJServerCli;	// line : 214 Join Server
wsJoinServerCli wsDataCli;	// line : 215 DataServer






wsJoinServerCli wsExDbCli;	// line : 239 Extra DataBase Server




wsJoinServerCli wsRServerCli;	// line : 244 Ranking Server

wsJoinServerCli wsEvenChipServerCli; // line : 246 EVENT_MU2003

CDirPath gDirPath;	// line : 248 Directory Path
MapClass MapC[MAX_MAP_NUMBER];	// line 249	// Map Manager
CMonsterAttr gMAttr;	// line 250
CMonsterSetBase gMSetBase;	// line 251
classdef DCInfo;	// line 252:
CWhisperCash WhisperCash;	// line 253
PartyClass gParty;	// line 254
CDbSave gDbSave;	// line 255
WzUdp gUdpSoc;	// line 256

WzUdp gUdpSocCE;	// line 258
WzUdp gUdpSocCER;	// line 259

WzUdp gUdpSocCRank;	// line 261
WzUdp gUdpSocCRankR;	//line 262



CGuildClass Guild;	// line 265





NSerialCheck gNSerialCheck[OBJMAX];	// line 326


CLoginCount gLCount[3];	// line 329



DWORD  gLevelExperience[MAX_CHAR_LEVEL+1];
char szGameServerExeSerial[24];
char szServerName[50];

BOOL gIsDropDarkLordItem;
int  gSleeveOfLordDropRate;
int  gSleeveOfLordDropLevel;

int  gSoulOfDarkHorseDropRate;
int  gSoulOfDarkHorseropLevel;
int  gSoulOfDarkSpiritDropRate;
int  gSoulOfDarkSpiritDropLevel;
float gDarkSpiritAddExperience;
BOOL gIsDropGemOfDefend;
int  gGemOfDefendDropRate;
int  gGemOfDefendDropLevel;
int  g_iUseCharacterAutoRecuperationSystem;
int  g_iCharacterRecuperationMaxLevel;
int g_iServerGroupGuildChatting;
int g_iServerGroupUnionChatting;

BOOL g_bChocolateBoxEvent;
BOOL g_bCandyBoxEvent;
BOOL g_bMysteryBoxEvent;

BOOL g_bFenrirStuffItemDrop;
int g_iFenrirStuff_01_DropLv_Min;
int g_iFenrirStuff_01_DropLv_Max;
int g_iFenrirStuff_01_DropMap;
int g_iFenrirStuff_01_DropRate;
int g_iFenrirStuff_02_DropLv_Min;
int g_iFenrirStuff_02_DropLv_Max;
int g_iFenrirStuff_02_DropMap;
int g_iFenrirStuff_02_DropRate;
int g_iFenrirStuff_03_DropLv_Min;
int g_iFenrirStuff_03_DropLv_Max;
int g_iFenrirStuff_03_DropMap;
int g_iFenrirStuff_03_DropRate;
int g_iFenrirRepairRate;
int g_iFenrirDefaultMaxDurSmall;
int g_iFenrirElfMaxDurSmall;
BOOL g_bCrywolfMonsterDarkElfItemDrop;
BOOL g_bCrywolfBossMonsterItemDrop;
int g_iCrywolfApplyMvpBenefit;
int g_iCrywolfApplyMvpPenalty;
int g_iSkillDistanceCheck;
int g_iSkillDistanceCheckTemp;
int g_iSkillDistanceKick;
int g_iSkillDistanceKickCount;
int g_iSkillDiatanceKickCheckTime;
BOOL g_bKanturuMayaHandItemDrop;
BOOL g_bKanturuNightmareItemDrop;
BOOL g_bHellMainItemDrop;
BOOL g_bKanturuSpecialItemDropOn;
int g_iKanturuMoonStoneDropRate;
int g_iKanturuJewelOfHarmonyDropRate;
BOOL g_bHalloweenDayEventOn;
int g_iHalloweenDayEventJOLBlessDropRate;
int g_iHalloweenDayEventJOLAngerDropRaTe;
int g_iHalloweenDayEventJOLScreamDropRate;
int g_iHalloweenDayEventJOLFoodDropRate;
int g_iHalloweenDayEventJOLDrinkDropRate;
int g_iHalloweenDayEventJOLPolymorphRingDropRate;

BYTE g_iWings3ReturnDamageSuccessRate;
BYTE g_iWings3RecoverFullLifeSuccessRate;
BYTE g_iWings3RecoverFullManaSuccessRate;
BYTE g_iWings3SuccessfullBlockingRate;

DWORD dwgCheckSum[MAX_CHECKSUM_KEY];
char connectserverip[20];
int  connectserverport;
short gGameServerCode;
BOOL gStalkProtocol;
int g_iRingOrcKillGiftRate;
int g_iRingDropGiftRate;
int g_iRingDropItemWhiteWizType;
int g_iRingDropItemWhiteWizIndex;
int g_iRingEventOrcRewardDropRate;
int g_iRingDropItemOrcType;
int g_iRingDropItemOrcIndex;

CSimpleModulus g_SimpleModulusCS;	// line 751
CSimpleModulus g_SimpleModulusSC;	// line 752

int gEledorado2EventItemDropRate[10];
int gEledorado2EventExItemDropRate[10];
int  gEledoradoGoldGoblenItemDropRate;
int  gEledoradoTitanItemDropRate;
int  gEledoradoGoldDerconItemDropRate;
int  gEledoradoDevilLizardKingItemDropRate;
int  gEledoradoDevilTantarosItemDropRate;
int  gEledoradoGoldGoblenExItemDropRate;
int  gEledoradoTitanExItemDropRate;
int  gEledoradoGoldDerconExItemDropRate;
int  gEledoradoDevilLizardKingExItemDropRate;
int  gEledoradoDevilTantarosExItemDropRate;
int  giKundunRefillHPSec;
int  giKundunRefillHP;
int  giKundunRefillHPTime;
int  giKundunHPLogSaveTime;
BOOL gUseNPGGChecksum;
int g_ShieldSystemOn;
int g_iDamageDevideToSDRate;
int g_iDamageDevideToHPRate;
float g_fSuccessAttackRateOption;
int g_ShieldAutoRefillOn;
int g_ShieldAutoRefillOnSafeZone;
int g_CompoundPotionDropOn;
int g_iCompoundPotionLv1DropRate;
int g_iCompoundPotionLv2DropRate;
int g_iCompoundPotionLv3DropRate;
int g_iCompoundPotionLv1DropLevel;
int g_iCompoundPotionLv2DropLevel;
int g_iCompoundPotionLv3DropLevel;
BOOL g_bShieldComboMissOptionOn;
int g_iShieldGageConstA;
int g_iShieldGageConstB;
char gMapName[MAX_MAP_NUMBER][25];


////////////////////////////////////////////////////////////////////




char szClientVersion[8]="000000";
BOOL bCanTrade=1;
BOOL bCanChaosBox=1;
BOOL bCanWarehouseLock=1;
// Here Appears Ring Data : 
short  MapMinUserLevel[MAX_MAP_NUMBER] = { 0, 20, 15, 10, 80, 0, 0, 60, 130, 0, 160, 10, 36, 80, 130, 170, 210,
										310, 15, 30, 100, 160, 220, 280, 15, 50, 120, 180, 240, 300, 10, 10, 0};
BOOL gEnableBattleSoccer=1;
int  gLootingTime=3;
int  gItemDropPer=10;
int  gEvent1ItemDropTodayMax=1;
int  gEvent1ItemDropTodayPercent=80;
BOOL gCreateCharacter=1;
BOOL gGuildCreate=1;
BOOL gGuildDestroy=1;
int  gGuildCreateLevel=100;
BOOL gItemSerialCheck=1;
BOOL gItemZeroSerialCheck;
float  gAddExperience=1.0f;
int  gFireCrackerDropRate=80;
int  g_ItemDropRateForgFireCracker=2;
int  gHeartOfLoveDropRate=80;
int  g_ItemDropRateForgHeartOfLove=2;
int  gGoldMedalDropRate=2;
int  gSilverMedalDropRate=2;
int  g_ItemDropRateForGoldMedal=2;
int  g_ItemDropRateForSilverMedal=2;
int  gBoxOfGoldDropRate=2;
int  gBoxOfGoldMinMobLevel=10;
int  gBoxOfGoldMaxMobLevel=100;
int  g_ItemDropRateForBoxOfGold=2;
int  g_ItemEXDropRateForBoxOfGold=2;
int  g_EventChipDropRateForBoxOfGold=2;
int  gEyesOfDevilSquareDropRate=2;
int  gKeyOfDevilSquareDropRate=2;
BOOL g_bBloodCastle=1;
int  g_iBloodCastle_StartHour=1;
int  g_iAngelKingsPaperDropRate=10;
int  g_iBloodBoneDropRate=20;
int g_iNpcAngelKingRemainTime=900;
int g_iBCDropChaosGemItemType=12;
int g_iBCDropChaosGemItemIndex=15;
int g_iBCDropChaosGemItemLevel=0;
int g_iBCDropChaosGemItemDur=0;
int g_iBCDropChaosGemItemSkill=0;
int g_iBCDropChaosGemItemLuck=0;
int g_iBCDropChaosGemItemOpt=0;
int g_iBCDropChaosGemItemExc=0;
BOOL g_bChaosCastle=1;

BYTE g_iCheckCanStartPlayCCMinPlayers=2;
BYTE g_iGiveWinnerItemCCType1=14;
short g_iGiveWinnerItemCCIndex1=14;
BYTE g_iGiveWinnerItemCCType2=14;
short g_iGiveWinnerItemCCIndex2=22;
BYTE g_iGiveWinnerItemCCType3=14;
short g_iGiveWinnerItemCCIndex3=13;
BYTE g_iGiveWinnerItemCCType4=14;
short g_iGiveWinnerItemCCIndex4=16;

BYTE g_bCC1SetDropRate=5;
BYTE g_bCC2SetDropRate=5;
BYTE g_bCC3SetDropRate=5;
BYTE g_bCC4SetDropRate=5;
BYTE g_bCC5SetDropRate=5;
BYTE g_bCC6SetDropRate=5;
BYTE g_bCC7SetDropRate=5;

BYTE g_bCC1TypeDropRate=50;
BYTE g_bCC2TypeDropRate=50;
BYTE g_bCC3TypeDropRate=50;
BYTE g_bCC4TypeDropRate=50;
BYTE g_bCC5TypeDropRate=50;
BYTE g_bCC6TypeDropRate=50;
BYTE g_bCC7TypeDropRate=50;

DWORD  gAttackSpeedTimeLimit=130;
DWORD  gHackCheckCount=5;
float gDecTimePerAttackSpeed=5.33f;
int  gMinimumAttackSpeedTime=8;
int  gDetectedHackKickCount=10;
int gTamaJJangKeepTime = 100;
int gTamaJJangDisappearTime = 1800;
int gTamaJJangDisappearTimeRandomRange = 1000;
BOOL gItemDropRingOfTransform=1;
int  gQuestNPCTeleportTime=60;
DWORD  gEledoradoGoldGoblenRegenTime=60;
DWORD  gEledoradoTitanRegenTime=60;
DWORD  gEledoradoGoldDerconRegenTime=60;
DWORD  gEledoradoDevilLizardKingRegenTime=60;
DWORD  gEledoradoDevilTantarosRegenTime=60;
DWORD  gEledorado2EventRegenTime[10]={60,60,60,60,60,60,60,60,60,60};
int  gZenDurationTime=30;
BOOL gEnableCheckPenetrationSkill=TRUE;

#if (GS_CASTLE==1)
BOOL g_bDoCastleDeepEvent;
int gIsDropSetItemInCastleHuntZone;
int gSetItemInCastleHuntZoneDropRate;
int gSetItemInCastleHuntZoneDropLevel;
int g_iCastleItemMixLimit = 1;
CItemBagEx * CastleItemMixItemBag;
CItemBagEx * ErohimCastleZoneItemBag;
CItemBagEx * HuntZoneItemBag;
#endif


void GameMainInit(HWND hWnd)
{
	CreateDirectory("LOG", NULL);
	#if (WL_PROTECT==1) 
		int MyCheckVar;   
		VM_START_WITHLEVEL(14)
			CHECK_PROTECTION(MyCheckVar, 0x95450743)  	 
			if (MyCheckVar != 0x95450743)
			{			
				exit(1);
			}
		VM_END
	#endif

	int n;
	//int DataBufferSize;
	//char* DataBuffer;
	int LevelOver_N;
	
	srand(time(NULL));
	ReadServerInfo();
	LogInit(TRUE);	// 1 : Enabled 0 : Disabled

	// Establish the work path of the files
	gDirPath.SetFirstPath(FIRST_PATH);
	gDirPath.SetFirstPath(FINAL_PATH);
	
	// WARNING
	// This will enable the auth server from Korea
	// Please check that you want to use this option
	// Default is : Enabled;

	//gGameServerAuth.Init();
	//gGameServerAuth.SetInfo(0, szGameServerVersion, szServerName, gServerType, GameServerAuthCallBackFunc);

	DragonEvent = new CDragonEvent;

	#if (WL_PROTECT==1) 
		int CheckCode;   
		VM_START_WITHLEVEL(14)
			CHECK_PROTECTION(CheckCode, 0x95450743)  	 
			if (CheckCode != 0x95450743)
			{			
				exit(1);
			}
		VM_END
	#endif
	
	if ( DragonEvent == 0 )
	{
		MsgBox("CDragonEvent %s", lMsg.Get( MSGGET(0,110)) );	// Memory allocation error
		return;
	}

	AttackEvent = new CAttackEvent;
	
	if ( AttackEvent == 0 )
	{
		MsgBox("AttackEvent %s", lMsg.Get(MSGGET(0,110)) );	// Memory allocation error
		return;
	}

	ReadCommonServerInfo();
	g_MapServerManager.LoadData( ReadConfig.ConnDataFiles[9]);
#if (GS_CASTLE==1)
	if( g_CastleSiege.Ready(g_MapServerManager.GetMapSvrGroup()) == TRUE )
	{
		if( g_CastleSiege.LoadData(ReadConfig.ConnDataFiles[20]) )
		{
			g_CastleSiege.LoadPreFixData(ReadConfig.ConnDataFiles[0]);
			g_CastleSiege.SetDataLoadState(CASTLESIEGE_DATALOAD_2);
		}
	}
#endif

	if ( gEnableServerDivision != 0 )
	{
		MessageBox(NULL, "Server division is ENABLED!!!", "Warning", MB_OK);
	}

	if ( gUdpSoc.CreateSocket() == 0)
	{
		MsgBox("UDP Socket create error");
		return;
	}

	if ( gUdpSocCER.CreateSocket() == 0)
	{
		MsgBox("UDP Socket create error");
		return;
	}

	gUdpSocCER.RecvSet( ReadConfig.UDPPort ); // Same as MuManager to JS 60006
	gUdpSocCER.Run();

	if ( gUdpSocCE.CreateSocket() == 0)
	{
		MsgBox("UDP Socket create error");
		return;
	}
	
	gUdpSocCER.SetProtocolCore(ChaosEventProtocolCore);
	gUdpSocCE.SetProtocolCore(ChaosEventProtocolCore); 

	if ( false )
	{
		MsgBox(lMsg.Get(MSGGET(0, 210)) );	// No check on speed hack.
	}
	if ( false )
	{
		MsgBox("주의!! 캐릭터 데이터를 저장하지 않습니다.");
	}

	gObjInit();
	InitBattleSoccer();

	gMAttr.LoadAttr(ReadConfig.ConnDataFiles[11]);
	gMSetBase.LoadSetBase(ReadConfig.ConnDataFiles[10]);
	
	g_MonsterItemMng.Init();

	gLevelExperience[0]=0;
	LevelOver_N=1;

	#if (WL_PROTECT==1)
		CODEREPLACE_START
		g_CashShop.SystemProcessesScan();
		CODEREPLACE_END
	#endif
	

	for ( n=1;n<MAX_CHAR_LEVEL+1;n++)
	{
		gLevelExperience[n] = (((n+9)*n)*n)*10;

		if ( n > 255 )
		{
			gLevelExperience[n] += ((((LevelOver_N+9)*LevelOver_N)*LevelOver_N)*1000);
			LevelOver_N++;
		}
	}

	/*char MapAttrName[MAX_MAP_NUMBER][15] =
	{
		"terrain1.att",
		"terrain2.att",
		"terrain3.att",
		"terrain4.att",
		"terrain5.att",
		"terrain6.att",
		"terrain7.att",
		"terrain8.att",
		"terrain9.att",
		"terrain10.att",
		"terrain11.att",
		"terrain12.att",
		"terrain12.att",
		"terrain12.att",
		"terrain12.att",
		"terrain12.att",
		"terrain12.att",
		"terrain12.att",
		"terrain19.att",
		"terrain19.att",
		"terrain19.att",
		"terrain19.att",
		"terrain19.att",
		"terrain19.att",
		"terrain25.att",
		"terrain25.att",
		"terrain25.att",
		"terrain25.att",
		"terrain25.att",
		"terrain25.att",
		"terrain31.att",
		"terrain32.att",
		"terrain33.att",
		"terrain34.att",
		"terrain35.att",
		"terrain36.att",
		"terrain37.att",
		"terrain38.att",
		"terrain39.att",
		"terrain40.att",
		"terrain41.att", 
		"terrain42.att", 
		"terrain43.att", 
		"terrain43.att", 
		"terrain43.att", 
		"terrain46.att", 
		"terrain47.att", 
		"terrain47.att", 
		"terrain47.att", 
		"terrain47.att", 
		"terrain51.att",
		"terrain52.att", 
		"terrain12.att", 
		"terrain19.att" 
	};*/

	char MapAttrName[MAX_MAP_NUMBER][64];
	char MapAttrKanturuName[2][64];
	char MapAttrCryWolfName[3][64];
	char etiqueta[8];

	#if (WL_PROTECT==1)
		int MyCheckVar2;
		CODEREPLACE_START
			CHECK_PROTECTION(MyCheckVar2, 0x12345678)
			if (MyCheckVar2 != 0x12345678)  	
			{
			   return;
			}
		CODEREPLACE_END
	#endif

	for ( n=0;n<MAX_MAP_NUMBER;n++)
	{
		wsprintf(etiqueta,"Map%02d", n);
		GetPrivateProfileString("Terrains", etiqueta, "..\\Data\\Terrains\\(null).att", MapAttrName[n], 64, "..\\SCFData\\SCF_Common.ini");
	}

	for ( n=0;n<MAX_MAP_NUMBER;n++)
	{
		MapC[n].ItemInit();
		MapC[n].LoadMapAttr(gDirPath.GetNewPath(MapAttrName[n]), n);
	}

	GetPrivateProfileString("Terrains", "Map40_Close", "..\\SCFData\\Terrains\\Kanturu_Close.att", MapAttrKanturuName[0], 64, "..\\SCFData\\SCF_Common.ini");
	GetPrivateProfileString("Terrains", "Map40_Open", "..\\SCFData\\Terrains\\Kanturu_Open.att", MapAttrKanturuName[1], 64, "..\\SCFData\\SCF_Common.ini");

	GetPrivateProfileString("Terrains", "Map35_Peace", "..\\SCFData\\Terrains\\CryWolf_Peace.att", MapAttrCryWolfName[0], 64, "..\\SCFData\\SCF_Common.ini");
	GetPrivateProfileString("Terrains", "Map35_Occupied", "..\\SCFData\\Terrains\\CryWolf_Occupied.att", MapAttrCryWolfName[1], 64, "..\\SCFData\\SCF_Common.ini");
	GetPrivateProfileString("Terrains", "Map35_War", "..\\SCFData\\Terrains\\CryWolf_War.att", MapAttrCryWolfName[2], 64, "..\\SCFData\\SCF_Common.ini");
	
	#if (GS_CASTLE==1)
		g_Crywolf.LoadCrywolfMapAttr(MapAttrCryWolfName[0],0);	//terrain35_PEACE.att
		g_Crywolf.LoadCrywolfMapAttr(MapAttrCryWolfName[1],1);	//terrain35_OCCUPIED.att
		g_Crywolf.LoadCrywolfMapAttr(MapAttrCryWolfName[2],2);	//terrain35_WAR.att
	#else
		g_Kanturu.LoadKanturuMapAttr(MapAttrKanturuName[0], 0);	//terrain40_CLOSE.att
		g_Kanturu.LoadKanturuMapAttr(MapAttrKanturuName[1], 1);	//terrain40_OPEN.att
	#endif

	DCInfo.Init();
	ShopDataLoad();

	#if (WL_PROTECT==1)

	CODEREPLACE_START
		if(WLRegGetStatus(NULL) == 1)
		{
			wsGServer.CreateSocket(hWnd);
		}
	CODEREPLACE_END 

	#else
		wsGServer.CreateSocket(hWnd);
	#endif

	wsJServerCli.CreateSocket(hWnd);
	wsDataCli.CreateSocket(hWnd);
	
	if(ReadConfig.SCFRSON == FALSE)
	{
		wsRServerCli.CreateSocket(hWnd);
	}
	if(ReadConfig.SCFESON == FALSE)
	{
		wsEvenChipServerCli.CreateSocket(hWnd);
	}
	GameServerInfoSend();
	GameMonsterAllAdd();

	acceptIP.Load(gDirPath.GetNewPath("Iplist.dat"));
	ConMember.Load(ReadConfig.ConnDataFiles[38]);
	GCTeleportSend(gObj, 1, 1, 1, 2, 3);
}


int GetWarehouseUsedHowMuch(int UserLevel, BOOL IsLock)
{
	int rZen=0;
	rZen = (UserLevel * UserLevel)* 0.1 * 0.4;

	if (bCanWarehouseLock == TRUE)
	{
		if ( IsLock != 0)
		{
			rZen = rZen + ( UserLevel * 2 );
		}
	}
	if ( rZen < 1 )
	{
		rZen=1;
	}

	if ( rZen >= 1000 )
	{
		rZen = (rZen/100)*100;
	}
	
	else if ( rZen >= 100 )
	{
		rZen = (rZen/10)*10;
	}
	
	if ( rZen == 0 )
	{
		rZen=1;
	}
	
	return rZen;
}

void GraphPaint(HWND hWnd)
{
	HDC hdc;
	RECT rect;
	int val[10];
	HBRUSH colBrush[4];
	int n;

	hdc = GetDC(hWnd);

	colBrush[0]=CreateSolidBrush( RGB(100, 100, 100));
	colBrush[1]=CreateSolidBrush( RGB(200, 100, 100));
	colBrush[2]=CreateSolidBrush( RGB(200, 200, 200));
	colBrush[3]=CreateSolidBrush( RGB(200, 200, 100));

	for ( n=0; n<4;n++)
	{
		val[0] = DCInfo.DefClass[n].Strength;
		val[1] = DCInfo.DefClass[n].Dexterity ;
		val[2] = DCInfo.DefClass[n].Vitality;
		val[3] = DCInfo.DefClass[n].Energy;
		val[4] = DCInfo.DefClass[n].Life ;
		val[5] = DCInfo.DefClass[n].Mana ;
		val[6] = DCInfo.DefClass[n].LevelLife * 10.0f;
		val[7] = DCInfo.DefClass[n].LevelMana * 10.0f;
		val[8] = DCInfo.DefClass[n].VitalityToLife * 10.0f;
		val[9] = DCInfo.DefClass[n].EnergyToMana * 10.0f;

		for (int k=0; k<10;k++)
		{
			rect.left = (k * 45) + (n * 10);
			rect.right = ((k * 45) + (n * 10))+10;
			rect.top = 0;
			rect.bottom = val[k];
			FillRect(hdc, &rect, colBrush[n]);
		}
	}
	
	ReleaseDC(hWnd, hdc);
	DeleteObject( (HGDIOBJ)colBrush[0]);
	DeleteObject( (HGDIOBJ)colBrush[1]);
	DeleteObject( (HGDIOBJ)colBrush[2]);
	DeleteObject( (HGDIOBJ)colBrush[3]);
	DeleteObject( (HGDIOBJ)val[0]);



}
/*
void PaintGraph(HWND hWnd, int argC)
{
	int i;
	HDC hdc;

	int lc118 = 0;
	int lc11C = 0;
	int lc120 = 0;
	int lc124 = 0;
	int lc128 = 0;

	if ( argC < 0 || argC > 3999 )
	{
		return;
	}

	hdc = GetDC(hWnd);

	for ( i = 0 ; i < 4000 ; i++ )
	{
		if ( gObj[i]. xx != 0 && gObj[i].xxx != 0 && gObj[i].xx == gCurMap && gObj[i].xxx == 1)
		{
			lc124++;
		}		

		if ( gObj[i].xxx == 1 && gObj[i]. != 0 )
		{
			lc128++;
		}
		else if ( gObj[i] != 0 )
		{
			lc118++;
		}
	}

	if ( gLogTypePaint == 1 )
	{
		lc13C = 100;
		lc140 = 50;
		lc144 = 1;
		lc148 = 1;
		lc14C = 3;
		HBRUSH colBrush[5];
		int bkMode;

		colBrush[0] = CreateSolidBrush(0xFF);
		colBrush[1] = CreateSolidBrush(0x808080);
		colBrush[2] = CreateSolidBrush(0xFFFF00);
		colBrush[3] = CreateSolidBrush(0xFF0000);
		colBrush[4] = CreateSolidBrush(0xFF00);
		bkMode = SetBkMode(hdc, 1);

		hdc = GetDC(hWnd);
*/

void GameMonsterAllAdd()
{
	gCurPaintPlayer = 0;

	int result;

	for ( int n=0;n< gMSetBase.m_Count ;n++)
	{
		if ( DS_MAP_RANGE(gMSetBase.m_Mp[n].m_MapNumber) != FALSE )
		{
			continue;
		}

		if ( BC_MAP_RANGE(gMSetBase.m_Mp[n].m_MapNumber) != FALSE )
		{
			if ( gMSetBase.m_Mp[n].m_Type != 232 )
			{
				continue;
			}
		}

		if ( IT_MAP_RANGE(gMSetBase.m_Mp[n].m_MapNumber) != FALSE )
		{
			if(gMSetBase.m_Mp[n].m_Type < 380 || gMSetBase.m_Mp[n].m_Type > 384)
			{
				continue;
			}
		}

		if ( CC_MAP_RANGE(gMSetBase.m_Mp[n].m_MapNumber) != FALSE )
		{
			continue;
		}
			
		result = gObjAddMonster(gMSetBase.m_Mp[n].m_MapNumber);

		if ( result >= 0 )
		{
			gObjSetPosMonster(result, n);
			gObjSetMonster(result, gMSetBase.m_Mp[n].m_Type,"GameMonsterAllAdd");
			gCurPaintPlayer++;

			if ( BC_MAP_RANGE(gObj[result].MapNumber) )
			{
				int BCRest = MAP_INDEX_BLOODCASTLE1;
				if(gObj[result].MapNumber == MAP_INDEX_BLOODCASTLE8)
					BCRest = 45;
				if ( gObj[result].Class == 232 )
				{
					gObj[result].m_cBloodCastleIndex = gObj[result].MapNumber - BCRest;
				}
			}

			if ( IT_MAP_RANGE(gObj[result].MapNumber) && gObj[result].Class == 380 )
			{
				WORD Room = gObj[result].MapNumber - 45;
				if(Room >= 0 && Room <= 6)
				{
					IllusionTemple.Room[Room].StatueNum = result;
					IllusionTemple.Room[Room].Statue_X = gObj[result].X;
					IllusionTemple.Room[Room].Statue_Y = gObj[result].Y;
				}
			}

#if (GS_CASTLE==1)
			if ( gObj[result].MapNumber == MAP_INDEX_CASTLESIEGE )
			{
				if ( gObj[result].Class == 216 )
				{
					//DebugLog("%s: Crown Index: %d",__FUNCTION__,result);
					g_CastleSiege.SetCrownIndex(result);
				}
			}
#endif
		}
	}

	g_DevilSquare.Init();

	if ( g_bBloodCastle != FALSE )
	{
		g_BloodCastle.Init(TRUE);
	}
	else
	{
		g_BloodCastle.Init(FALSE);
	}

	if ( g_bChaosCastle != FALSE )
	{
		g_ChaosCastle.Init(true);
	}
	else
	{
		g_ChaosCastle.Init(false);
	}
	g_DoppelGanger.Init();
	
	Raklion.ReadMonsters("..\\SCFData\\EventMobs\\SCF_RaklionEvent.dat");
	g_ImperialGuardian.ReadMonsters("..\\SCFData\\EventMobs\\SCF_ImperialGuardian.dat");
#if(GS_CASTLE_NOEVENTS == 0)
	#if (PACK_EDITION>=1)
		BlueEvent.ReadMonsters("..\\SCFData\\EventMobs\\SCF_BlueEvent.dat");
		SummerEvent.ReadMonsters("..\\SCFData\\EventMobs\\SCF_SummerEvent.dat");
	#endif
	#if (PACK_EDITION>=2)
		XMasEvent.ReadMonsters("..\\SCFData\\EventMobs\\SCF_XMasEvent.dat");
		SkyEvent.ReadMonsters("..\\SCFData\\EventMobs\\SCF_SkyEvent.dat");
	#endif
#endif

#if (PACK_EDITION>=3)
	#if(GS_CASTLE_NOEVENTS == 0)
		//g_DoubleGoer.ReadMonsters("..\\SCFData\\EventMobs\\SCF_DoubleGoer.dat");
		BossAttack.ReadMonsters("..\\SCFData\\EventMobs\\SCF_BossAttack.dat");
	#endif

	#if (GS_CASTLE==1)
		g_Swamp.ReadMonsters("..\\SCFData\\EventMobs\\SCF_SwampEvent.dat");
	#endif

	BotStore.MakeBot();
	BotBuff.MakeBot();
	BotAlchemist.MakeBot();
	BotTrader.MakeBot();
	BotReward.MakeBot();
	BotWarper.MakeBot();
	BotRacer.ForceClose = true;
	#if (CRYSTAL_EDITION==1)	
		BotVipShop.MakeBot();
	#endif
#endif

#if(GS_CASTLE_NOEVENTS == 0)
	MossShop.LoadMonster();
#endif
	ReadMonstersAndAdd(FilePathSpawn);
}







void GameMonsterAllCloseAndReLoad()
{
	for ( int n=0;n<OBJ_MAXMONSTER;n++)
	{
		if ( gObj[n].Type == OBJ_MONSTER || gObj[n].Type == OBJ_NPC )
		{
			if ( gObj[n].MapNumber == MAP_INDEX_KANTURU_BOSS )
			{
				//continue;
			}

			if ( gObj[n].m_iCurrentAI )
			{
				gObj[n].Live = FALSE;
				gObjViewportListProtocolDestroy(&gObj[n]);
				gObjViewportClose(&gObj[n]);
			}
			
			gObjDel(n);
		}
	}

#if (PACK_EDITION>=3)
	for ( int n=OBJ_MAXMONSTER;n<OBJ_STARTUSERINDEX;n++)
	{
		if ( gObj[n].Type == OBJ_MONSTER || gObj[n].Type == OBJ_NPC )
		{
			if(gObj[n].IsBot >= 1 && gObj[n].BotNumOwner <= 0 && gObj[n].m_RecallMon <= 0)
			{
				if ( gObj[n].m_iCurrentAI )
				{
					gObj[n].Live = FALSE;
					gObjViewportListProtocolDestroy(&gObj[n]);
					gObjViewportClose(&gObj[n]);
				}
				gObjCallMonCount--;
				gObjDel(n);
			}
		}
	}
#endif

	gMAttr.LoadAttr(ReadConfig.ConnDataFiles[11]);
	gMSetBase.LoadSetBase(ReadConfig.ConnDataFiles[10]);

	g_MonsterItemMng.Init();
	gObjMonCount = 0;
	GameMonsterAllAdd();
}





void GameMainFree()
{
	gDbSave.End();
	GDGameServerInfoSave();
	gObjEnd();
	ClearBattleSoccer();
	LogClose();
#if (WL_PROTECT==1)
	CODEREPLACE_START
#endif
	CleanupGameguardAuth();
#if (WL_PROTECT==1)
	CODEREPLACE_END
#endif
}




BOOL GMJoinServerConnect(LPSTR ConnectServer, DWORD wMsg)
{
	wsJServerCli.SetProtocolCore(SProtocolCore);
	int result = wsJServerCli.Connect(ConnectServer, JoinServerPort, wMsg);

	if ( result == 0 )
	{
		return FALSE;
	}

	GJServerLogin();

	LogAdd( lMsg.Get( MSGGET(1, 145)), ConnectServer);
	return TRUE;

}






BOOL GMRankingServerConnect(char* RankingServer, DWORD wMsg)
{
	if(ReadConfig.SCFRSON == FALSE)
	{
		wsRServerCli.SetProtocolCore(DevilSquareProtocolCore);
		int result = wsRServerCli.Connect(RankingServer, RANKING_SERVER_PORT, wMsg);
		if ( result == 0 )
		{
			return FALSE;
		}
		LogAdd("RankingServer Connect IP [%s]", RankingServer);
		return TRUE;
	}
	return FALSE;
}





BOOL GMEventChipServerConnect(char* ServerIP, DWORD wMsg)
{
	if(ReadConfig.SCFESON == FALSE)
	{
		wsEvenChipServerCli.SetProtocolCore(EventChipEventProtocolCore);
		int result = wsEvenChipServerCli.Connect(ServerIP, EVENT_CHIP_SERVER_PORT, wMsg);
		if ( result == 0 )
		{
			return FALSE;
		}
	
		LogAdd("EventChip Connect IP [%s]", ServerIP);
		return TRUE;
	}
	return FALSE;
}





BOOL GMDataServerConnect(char* ConnectServer, WPARAM wMsg)
{

	#if (WL_PROTECT==1)
		VM_START_WITHLEVEL(13)
		int MyCheckVar4;  
		CHECK_PROTECTION(MyCheckVar4, 0x12482415)  	 
		if (MyCheckVar4 == 0x12482415)
		{
			wsDataCli.SetProtocolCore(DataServerProtocolCore);
			int result = wsDataCli.Connect(ConnectServer, DataServerPort, wMsg);
			if ( result == 0 )
			{
				LogAdd(lMsg.Get(MSGGET(1, 147)), ConnectServer, DataServerPort);
				return FALSE;
			}
			DataServerLogin(0);
			LogAdd(lMsg.Get(MSGGET(1, 146)), ConnectServer, DataServerPort);
		}
		VM_END
	#else
		wsDataCli.SetProtocolCore(DataServerProtocolCore);
		int result = wsDataCli.Connect(ConnectServer, DataServerPort, wMsg);
		if ( result == 0 )
		{
			LogAdd(lMsg.Get(MSGGET(1, 147)), ConnectServer, DataServerPort);
			return FALSE;
		}
		DataServerLogin(0);
		LogAdd(lMsg.Get(MSGGET(1, 146)), ConnectServer, DataServerPort);
	#endif

	return TRUE;
}

BYTE RecvTable[256]  =  
{
		0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
		0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,0x1E,0x1F,
		0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,0x2E,0x2F,
		0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,0x3E,0x3F,
		0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4A,0x4B,0x4C,0x4D,0x4E,0x4F,
		0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5A,0x5B,0x5C,0x5D,0x5E,0x5F,
		0x60,0x61,0x62,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6A,0x6B,0x6C,0x6D,0x6E,0x6F,
		0x70,0x71,0x72,0x73,0x74,0x75,0x76,0x77,0x78,0x79,0x7A,0x7B,0x7C,0x7D,0x7E,0x7F,
		0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8A,0x8B,0x8C,0x8D,0x8E,0x8F,
		0x90,0x91,0x92,0x93,0x94,0x95,0x96,0x97,0x98,0x99,0x9A,0x9B,0x9C,0x9D,0x9E,0x9F,
		0xA0,0xA1,0xA2,0xA3,0xA4,0xA5,0xA6,0xA7,0xA8,0xA9,0xAA,0xAB,0xAC,0xAD,0xAE,0xAF,
		0xB0,0xB1,0xB2,0xB3,0xB4,0xB5,0xB6,0xB7,0xB8,0xB9,0xBA,0xBB,0xBC,0xBD,0xBE,0xBF,
		0xC0,0xC1,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7,0xC8,0xC9,0xCA,0xCB,0xCC,0xCD,0xCE,0xCF,
		0xD0,0xD1,0xD2,0xD3,0xD4,0xD5,0xD6,0xD7,0xD8,0xD9,0xDA,0xDB,0xDC,0xDD,0xDE,0xDF,
		0xE0,0xE1,0xE2,0xE3,0xE4,0xE5,0xE6,0xE7,0xE8,0xE9,0xEA,0xEB,0xEC,0xED,0xEE,0xEF,
		0xF0,0xF1,0xF2,0xF3,0xF4,0xF5,0xF6,0xF7,0xF8,0xF9,0xFA,0xFB,0xFC,0xFD,0xFE,0xFF 
} ; 

BOOL ExDataServerConnect(char* ConnectServer, DWORD wMsg)
{
#if (WL_PROTECT==1)
	int MyCheckVar;  
	VM_START_WITHLEVEL(14)
	CHECK_PROTECTION(MyCheckVar, 0x15472142)  	 
	if (MyCheckVar != 0x15472142)
	{
		for(int i=0;i<256;i++)
		{
			RecvTable[i] = rand()%255;
		}
	}
	VM_END
#endif
		
	wsExDbCli.SetProtocolCore(ExDataServerProtocolCore);
	int result = wsExDbCli.Connect(ConnectServer, ExDbPort, wMsg);
	if ( result == 0 )
	{
		LogAdd(lMsg.Get(MSGGET(1, 147)), ConnectServer, ExDbPort);
		return FALSE;
	}

	ExDataServerLogin();
	LogAddC(2, "ExDB Server Online");
	return TRUE;
}






BOOL GameMainServerCreate(DWORD sMsg, DWORD cMsg)
{
	gDbSave.Begin();
	LogAdd( lMsg.Get( MSGGET(1, 148)), GameServerPort);
	return TRUE;
}







void GMServerMsgProc( WPARAM wParam, LPARAM lParam)
{
	{
		int unk =lParam & 0xFFFF & 0xFFFF;	// HERE is a switch here goes a macro
	}
}






void GMClientMsgProc(WPARAM wParam, LPARAM lParam)
{
	//int wp = wParam;
	//int lp = lParam;

	switch ( lParam & 0xFFFF & 0xFFFF)
	{
		SOCKET socket;

		case 1:
			wsGServer.DataRecv(wParam);
			break;

		case 2:
			wsGServer.FDWRITE_MsgDataSend(wParam);
			break;

		case 32:
			socket = gObjGetSocket(wParam);
			break;

	}
}


extern BOOL AutoClose;

void GMJoinClientMsgProc(WPARAM wParam, LPARAM lParam)
{
	switch ( lParam & 0xFFFF & 0xFFFF )
	{
		case 1:
			wsJServerCli.DataRecv();
			break;

		case 2:
			wsJServerCli.FDWRITE_MsgDataSend();
			break;

		case 32:
			{
			LogAddC(2, lMsg.Get(MSGGET(1, 149)));
			AutoClose = TRUE;
			SendMessage(ghWnd, WM_CLOSE, 0, 0);
			}break;
	}
}






void GMRankingClientMsgProc(WPARAM wParam, LPARAM lParam)
{
	switch ( lParam & 0xFFFF & 0xFFFF )
	{
		case 1:
			wsRServerCli.DataRecv();
			break;

		case 2:
			wsRServerCli.FDWRITE_MsgDataSend();
			break;

		case 32:
			IsDevilSquareEventConnected = FALSE;
	}
}






void GMEventChipClientMsgProc(WPARAM wParam, LPARAM lParam)
{
	switch ( lParam & 0xFFFF & 0xFFFF )
	{
		case 1:
			wsEvenChipServerCli.DataRecv();
			break;

		case 2:
			wsEvenChipServerCli.FDWRITE_MsgDataSend();
			break;

		case 32:
			IsEventChipServerConnected = FALSE;
	}
}




void ExDataClientMsgProc(WPARAM wParam, LPARAM lParam)
{
	switch ( lParam & 0xFFFF & 0xFFFF )
	{
		case 1:
			wsExDbCli.DataRecv();
			break;

		case 2:
			wsExDbCli.FDWRITE_MsgDataSend();
			break;

		case 32:
			LogAddC(2, lMsg.Get(MSGGET(1, 150)));
			wsExDbCli.Close();

			for ( int i =0;i<OBJMAX;i++)
			{
				if ( gObjIsConnectedGP(i) == TRUE )
				{
					PMSG_FRIEND_STATE pMsg;
					
					pMsg.h.set((LPBYTE)&pMsg, 0xC4 , sizeof(pMsg) );
					pMsg.State = -4;

					DataSend(i, (unsigned char*)&pMsg, sizeof(pMsg) );

					gObj[i].m_FriendServerOnline = FRIEND_SERVER_STATE_OFFLINE;
				}
			}

			LogAddTD("Error-L1 : Friend Server Down (State Send Ok)");
			LogAddC(2, "Error-L1 : Friend Server Down (State Send Ok)");
			break;
	}
}


void GMDataClientMsgProc(WPARAM wParam, LPARAM lParam)
{
	switch ( lParam & 0xFFFF & 0xFFFF )
	{
		case 1:
			wsDataCli.DataRecv();
			break;

		case 2:
			wsDataCli.FDWRITE_MsgDataSend();
			break;

		case 32:
			LogAddC(2, lMsg.Get(MSGGET(1, 150)));
	}
}





void ReadServerInfo()
{
	if (!IsFile(ReadConfig.ConnDataFiles[8]))
	{
		MsgBox("ServerInfo.dat file not found");
	}
	GetPrivateProfileString("GameServerInfo","ServerName","",szServerName,50,ReadConfig.ConnDataFiles[8]);
	gGameServerCode=GetPrivateProfileInt("GameServerInfo","ServerCode",0,ReadConfig.ConnDataFiles[8]);
	bCanConnectMember=GetPrivateProfileInt("GameServerInfo","ConnectMemberLoad",0,ReadConfig.ConnDataFiles[8]);
}

bool AlreadyReaded = false;
void ReadCommonServerInfo()
{
	char szTemp[256];
	//char szCheckSum[256];
	//char* cvstr;
	//int DataBufferSize;
	//char* DataBuffer;

	ReadConfig.Init();
#if (WL_PROTECT==1)
	VM_START_WITHLEVEL(2)

#if (MAC_PROTECT_OLD==1)
	CheckMemoryTeaser_MAC_Comparator(0);
#else		
	int MyCheckVar2;   
	CHECK_PROTECTION(MyCheckVar2, 0x18465741)  	
	if (MyCheckVar2 != 0x18465741)
	{			
		_beginthread( Teaser__InsideTrigger, 0, NULL  );
	}
#endif
	VM_END
#endif
	ReadServerInfo();

	gGateC.Load(ReadConfig.ConnDataFiles[12]);

	if(!IsFile(ReadConfig.ConnDataFiles[0]))
	{
		MsgBox("[commonserver.cfg] file not found");
	}
#if (DSGN_COMPILE == 1)
	gStalkProtocol	= GetPrivateProfileInt("GameServerInfo","StalkProtocol",0,ReadConfig.ConnDataFiles[0]);
#else
	gStalkProtocol	= 0;
#endif
	if(g_SimpleModulusCS.LoadDecryptionKey(ReadConfig.ConnDataFiles[43]) == FALSE)
	{
		MsgBox("Dec1.dat file not found");
	}
	if(g_SimpleModulusSC.LoadEncryptionKey(ReadConfig.ConnDataFiles[44]) == FALSE)
	{
		MsgBox("Enc2.dat file not found");
	}

	//strcpy(szCheckSum, ReadConfig.ConnDataFiles[4]);
	//CheckSumFileLoad(szCheckSum);
	CheckSumFileLoad(ReadConfig.ConnDataFiles[4]);

#if (CSAUTH_VERSION==1)
#if (WL_PROTECT==1)
	CODEREPLACE_START
#endif
	BOOL bret = _LoadAuthTable(ReadConfig.ConnDataFiles[45]);

	if(bret != 8)
	{
		MsgBox("CSAuth File Load Fail");
	}

	bret = _LoadAuthIndex(ReadConfig.ConnDataFiles[46]);

	if(bret != 1)
	{
		MsgBox("CSAuth.idx File Load Fail");
	}
#if (WL_PROTECT==1)
	CODEREPLACE_END
#endif
#endif


#if (CSAUTH_VERSION==2)
#if (WL_PROTECT==1)
	CODEREPLACE_START
#endif
	DWORD dwGGErrCode = InitGameguardAuth("", OBJMAXUSER);

	if ( dwGGErrCode != 0 )
	{
		MsgBox("Failed initialization of GameGuard !!! , Error: %d", dwGGErrCode);
		return;
	}
#if (WL_PROTECT==1)
	CODEREPLACE_END
#endif
#endif

	SetMapName();

	OpenItemScript(ReadConfig.ConnDataFiles[1]);
	InitSocketItems("..\\SCFData\\Items\\SCF_SocketItems.txt");

#if (DSGN_ITEM_DBMOVE == 1)
	DS_Mover.Init();
	DS_Mover.LoadItem("..\\DSGNData\\DSGN_ItemConverter.txt");
#endif

	MagicDamageC.LogSkillList(ReadConfig.ConnDataFiles[2]);

	SkillSpearHitBox.Load(ReadConfig.ConnDataFiles[39]);
	SkillElectricSparkHitBox.Load(ReadConfig.ConnDataFiles[40]);

	gSetItemOption.LoadOptionInfo(ReadConfig.ConnDataFiles[36]);
	gSetItemOption.LoadTypeInfo(ReadConfig.ConnDataFiles[37]);

	g_kJewelOfHarmonySystem.LoadScript(ReadConfig.ConnDataFiles[32]);
	g_kJewelOfHarmonySystem.LoadScriptOfSmelt(ReadConfig.ConnDataFiles[33]);
	g_kItemSystemFor380.Load380ItemOptionInfo(ReadConfig.ConnDataFiles[34]);
	g_ItemAddOption.Load(ReadConfig.ConnDataFiles[35]);

	gMoveCommand.Load(ReadConfig.ConnDataFiles[5]);
	gMoveCommand.LoadMoveLevel(ReadConfig.ConnDataFiles[22]);

	ConMember.Load(ReadConfig.ConnDataFiles[38]);

	g_QuestInfo.LoadQuestInfo(ReadConfig.ConnDataFiles[3]);	

	TMonsterSkillElement::LoadData(ReadConfig.ConnDataFiles[24]);
	TMonsterSkillUnit::LoadData(ReadConfig.ConnDataFiles[25]);
	TMonsterSkillManager::LoadData(ReadConfig.ConnDataFiles[26]);
	TMonsterAIElement::LoadData(ReadConfig.ConnDataFiles[27]);
	TMonsterAIAutomata::LoadData(ReadConfig.ConnDataFiles[28]);
	TMonsterAIUnit::LoadData(ReadConfig.ConnDataFiles[29]);
	TMonsterAIRule::LoadData(ReadConfig.ConnDataFiles[30]);
	TMonsterAIGroup::LoadData(ReadConfig.ConnDataFiles[31]);

	// Skill Addition
	g_SkillAdditionInfo.Load(ReadConfig.ConnDataFiles[41]);

	//Crywolf
	#if (GS_CASTLE==1)
	g_Crywolf.LoadData(ReadConfig.ConnDataFiles[21]);
	#endif

	// Kanturu
	#if (GS_CASTLE==0)
	g_Kanturu.LoadData(ReadConfig.ConnDataFiles[16]);
	g_KanturuMonsterMng.LoadData(ReadConfig.ConnDataFiles[23]);
	#endif

	//Read All Event Options
	ReadEventInfo(MU_EVENT_ALL );

	GetPrivateProfileString("GameServerInfo", "CreateCharacter", "1", szTemp, 5, ReadConfig.ConnDataFiles[0]);
	gCreateCharacter = atoi(szTemp);
	gServerMaxUser = GetPrivateProfileInt("GameServerInfo","NumberOfMaxUser",500, ReadConfig.ConnDataFiles[0]);

	if(gServerMaxUser >= 0 && gServerMaxUser <= OBJMAXUSER)
	{
		LogAddTD("[Option Reload] MaxUser Settings: %d", gServerMaxUser);
	} else {
		gServerMaxUser=OBJMAXUSER;
		LogAddTD("[Option Reload] MaxUser Settings: %d", gServerMaxUser);
	}

	// Guild Settingss
	g_iServerGroupGuildChatting = GetPrivateProfileInt("GameServerInfo","ServerGroupGuildChatting",0, ReadConfig.ConnDataFiles[0]);
	g_iServerGroupUnionChatting = GetPrivateProfileInt("GameServerInfo","ServerGroupUnionChatting",0, ReadConfig.ConnDataFiles[0]);

	GetPrivateProfileString("GameServerInfo", "GuildCreate", "1", szTemp, 5, ReadConfig.ConnDataFiles[0]);
	gGuildCreate = atoi(szTemp);

	LogAdd(lMsg.Get(MSGGET(2, 57)), gGuildCreate);	// Guild creation option %d

	GetPrivateProfileString("GameServerInfo", "GuildDestroy", "1", szTemp, 5, ReadConfig.ConnDataFiles[0]);
	gGuildDestroy = atoi(szTemp);
	LogAdd(lMsg.Get(MSGGET(2, 58)), gGuildDestroy);	// Guild delete option %d

	GetPrivateProfileString("GameServerInfo", "GuildCreateLevel", "100", szTemp, 5, ReadConfig.ConnDataFiles[0]);
	gGuildCreateLevel = atoi(szTemp);

	LogAdd(lMsg.Get(MSGGET(2, 59)), gGuildCreateLevel);	// Above level %d can create guild

	GetPrivateProfileString("GameServerInfo", "SpeedHackPlayerBlock", "0", szTemp, 5, ReadConfig.ConnDataFiles[0]);
	SpeedHackPlayerBlock = atoi(szTemp);
	GetPrivateProfileString("GameServerInfo", "ItemSerialCheck", "1", szTemp, 5, ReadConfig.ConnDataFiles[0]);
	gItemSerialCheck = atoi(szTemp);
	GetPrivateProfileString("GameServerInfo", "ItemZeroSerialCheck", "1", szTemp, 5, ReadConfig.ConnDataFiles[0]);
	gItemZeroSerialCheck = atoi(szTemp);
	GetPrivateProfileString("GameServerInfo", "AddExperience", "1", szTemp, 5, ReadConfig.ConnDataFiles[0]);
	gAddExperience = atof(szTemp);	

	GetPrivateProfileString("GameServerInfo", "Trade", "1", szTemp, 5, ReadConfig.ConnDataFiles[0]);
	bCanTrade = atoi(szTemp);
	if(bCanTrade != 0)
		LogAddTD(lMsg.Get(407));
	else
		LogAddTD(lMsg.Get(408));

	bCanChaosBox = GetPrivateProfileInt("GameServerInfo","ChaosBox",0, ReadConfig.ConnDataFiles[0]);
	LogAdd("[Option] Chaos Machine State: ChaosBox = %d", bCanChaosBox);

	GetPrivateProfileString("GameServerInfo", "MonsterHp", "100", szTemp, 5, ReadConfig.ConnDataFiles[0]);
	gMonsterHp = atoi(szTemp);
	if (gMonsterHp == 0)
		gMonsterHp = 100;
	LogAddTD(lMsg.Get(410), gMonsterHp);

	GetPrivateProfileString("GameServerInfo", "LootingTime", "3", szTemp, 5, ReadConfig.ConnDataFiles[6]);
	gLootingTime = atoi(szTemp);
	if(gLootingTime != 0)
		LogAddTD("[Option] Auto Looting Time Set : %d", gLootingTime);

	GetPrivateProfileString("GameServerInfo", "ItemDropPer", "10", szTemp, 5, ReadConfig.ConnDataFiles[0]);
	gItemDropPer = atoi(szTemp);
	LogAddTD(lMsg.Get(413), gItemDropPer);
	gZenDurationTime = GetPrivateProfileInt("GameServerInfo","ZenDurationTime",30, ReadConfig.ConnDataFiles[0]);

	// Antihack Settings
	gAttackSpeedTimeLimit = GetPrivateProfileInt("GameServerInfo","AttackSpeedTimeLimit",800, ReadConfig.ConnDataFiles[0]);
	bIsIgnorePacketSpeedHackDetect = GetPrivateProfileInt("GameServerInfo","IsIgnorePacketHackDetect",0, ReadConfig.ConnDataFiles[0]);
	gHackCheckCount = GetPrivateProfileInt("GameServerInfo","HackCheckCount",5, ReadConfig.ConnDataFiles[0]);
	gMinimumAttackSpeedTime = GetPrivateProfileInt("GameServerInfo","MinimumAttackSpeedTime",200, ReadConfig.ConnDataFiles[0]);
	gDetectedHackKickCount = GetPrivateProfileInt("GameServerInfo","DetectedHackKickCount",10, ReadConfig.ConnDataFiles[0]);
	gIsKickDetecHackCountLimit = GetPrivateProfileInt("GameServerInfo","IsKickDetecHackCountLimit",0, ReadConfig.ConnDataFiles[0]);

	// Ring of Transform
	gIsItemDropRingOfTransform = GetPrivateProfileInt("GameServerInfo","IsItemDropRingOfTransform",0, ReadConfig.ConnDataFiles[0]);
	gItemDropRingOfTransform = GetPrivateProfileInt("GameServerInfo","ItemDropRingOfTransform",1, ReadConfig.ConnDataFiles[0]);

	// Hack Settings
	gDisconnectHackUser = GetPrivateProfileInt("GameServerInfo","DisconnectHackUser",0, ReadConfig.ConnDataFiles[0]);
	gUseNPGGChecksum = GetPrivateProfileInt("GameServerInfo","UseNPGGChecksum",0, ReadConfig.ConnDataFiles[0]);
	GetPrivateProfileString("GameServerInfo", "DecTimePerAttackSpeed", "5.33", szTemp, 5, ReadConfig.ConnDataFiles[0]);
	gDecTimePerAttackSpeed = atof(szTemp);

	// Skill Log
	gWriteSkillLog = GetPrivateProfileInt("GameServerInfo","WriteSkillLog",0, ReadConfig.ConnDataFiles[0]);

	// Marlon Teleport
	gQuestNPCTeleportTime = GetPrivateProfileInt("GameServerInfo","QuestNPCTeleportTime",900, ReadConfig.ConnDataFiles[0]);

	#if (WL_PROTECT==1)
		int MyCheckVar;   
		VM_START_WITHLEVEL(20)
			CHECK_PROTECTION(MyCheckVar, 0x12345678)  	 
			if (MyCheckVar != 0x12345678)
			{				
				bCanTrade = 0;
				gAddExperience = 1.0f;	
				gCreateCharacter = 0;
				gServerMaxUser=5;
	 			gItemDropPer = 1;
			}
		VM_END
	#endif

	// Fenrir Settings
	g_bFenrirStuffItemDrop = GetPrivateProfileInt("GameServerInfo","FenrirStuffItemDrop", 0, ReadConfig.ConnDataFiles[0]);

		// Fenrir Stuff 1
		g_iFenrirStuff_01_DropLv_Min = GetPrivateProfileInt("GameServerInfo","FenrirStuff_01_DropLv_Min", 0, ReadConfig.ConnDataFiles[0]);
		g_iFenrirStuff_01_DropLv_Max = GetPrivateProfileInt("GameServerInfo","FenrirStuff_01_DropLv_Max", 0, ReadConfig.ConnDataFiles[0]);
		g_iFenrirStuff_01_DropMap = GetPrivateProfileInt("GameServerInfo","FenrirStuff_01_DropMap", 0, ReadConfig.ConnDataFiles[0]);
		g_iFenrirStuff_01_DropRate = GetPrivateProfileInt("GameServerInfo","FenrirStuff_01_DropRate", 0, ReadConfig.ConnDataFiles[0]);

		// Fenrir Stuff 2
		g_iFenrirStuff_02_DropLv_Min = GetPrivateProfileInt("GameServerInfo","FenrirStuff_02_DropLv_Min", 0, ReadConfig.ConnDataFiles[0]);
		g_iFenrirStuff_02_DropLv_Max = GetPrivateProfileInt("GameServerInfo","FenrirStuff_02_DropLv_Max", 0, ReadConfig.ConnDataFiles[0]);
		g_iFenrirStuff_02_DropMap = GetPrivateProfileInt("GameServerInfo","FenrirStuff_02_DropMap", 0, ReadConfig.ConnDataFiles[0]);
		g_iFenrirStuff_02_DropRate = GetPrivateProfileInt("GameServerInfo","FenrirStuff_02_DropRate", 0, ReadConfig.ConnDataFiles[0]);

		// Fenrir Stuff 3
		g_iFenrirStuff_03_DropLv_Min = GetPrivateProfileInt("GameServerInfo","FenrirStuff_03_DropLv_Min", 0, ReadConfig.ConnDataFiles[0]);
		g_iFenrirStuff_03_DropLv_Max = GetPrivateProfileInt("GameServerInfo","FenrirStuff_03_DropLv_Max", 0, ReadConfig.ConnDataFiles[0]);
		g_iFenrirStuff_03_DropMap = GetPrivateProfileInt("GameServerInfo","FenrirStuff_03_DropMap", 0, ReadConfig.ConnDataFiles[0]);
		g_iFenrirStuff_03_DropRate = GetPrivateProfileInt("GameServerInfo","FenrirStuff_03_DropRate", 0, ReadConfig.ConnDataFiles[0]);

		// Fenrir Durability Settings
		g_iFenrirRepairRate = GetPrivateProfileInt("GameServerInfo","FenrirRepairRate", 4000, ReadConfig.ConnDataFiles[0]);
		g_iFenrirDefaultMaxDurSmall = GetPrivateProfileInt("GameServerInfo","FenrirDefaultMaxDurSmall", 200, ReadConfig.ConnDataFiles[0]);
		g_iFenrirElfMaxDurSmall = GetPrivateProfileInt("GameServerInfo","FenrirElfMaxDurSmall", 160, ReadConfig.ConnDataFiles[0]);

	//Season 3 Wings
	g_iWings3ReturnDamageSuccessRate = GetPrivateProfileInt("GameServerInfo","Wings3ReturnDamageSuccessRate",5, ReadConfig.ConnDataFiles[0]);
	g_iWings3RecoverFullLifeSuccessRate = GetPrivateProfileInt("GameServerInfo","Wings3RecoverFullLifeSuccessRate",5, ReadConfig.ConnDataFiles[0]);
	g_iWings3RecoverFullManaSuccessRate = GetPrivateProfileInt("GameServerInfo","Wings3RecoverFullManaSuccessRate",5, ReadConfig.ConnDataFiles[0]);
	g_iWings3SuccessfullBlockingRate = GetPrivateProfileInt("GameServerInfo","Wings3SuccessfullBlockingRate",5, ReadConfig.ConnDataFiles[0]);

	// Servers
	GetPrivateProfileString("ConnectServerInfo", "IP", "", connectserverip, 20, ReadConfig.ConnDataFiles[6]);
	GetPrivateProfileString("ConnectServerInfo", "PORT", "", szTemp, 10, ReadConfig.ConnDataFiles[6]);
	connectserverport = atoi(szTemp);

	// (Option) Connect Server IP(%s) / PORT(%d)
	LogAddTD(lMsg.Get(MSGGET(1, 158)), connectserverip, connectserverport);
	gUdpSoc.SendSet(connectserverip, connectserverport);

	// Battle Soccer
	gEnableBattleSoccer = GetPrivateProfileInt("GameServerInfo","EnableBattleSoccer", 1, ReadConfig.ConnDataFiles[0]);

	// Server Division
	gEnableServerDivision = GetPrivateProfileInt("GameServerInfo","EnableServerDivision", 0, ReadConfig.ConnDataFiles[0]);

	// COMMANDS Init ( /make / Create /trace etc...)
	cManager.Init();

	// Penetration Skill
	gEnableCheckPenetrationSkill = GetPrivateProfileInt("GameServerInfo","EnableCheckPenetrationSkill", 1, ReadConfig.ConnDataFiles[0]);


#if (WL_PROTECT==1)
	int MyCheckVar1;
	VM_START
	CHECK_REGISTRATION(MyCheckVar1, 0x64763510)  
	if (MyCheckVar1 != 0x64763510)
	{						
		ReadConfig.SkillNightDiv = 0;
		ReadConfig.SkillReflectTimeDiv = 0;
		ReadConfig.SkillSleepTimeDiv = 0;
		ReadConfig.SkillReduceDamageDiv = 0;
	}
	VM_END
#endif

	// Shield System
	g_ShieldSystemOn = GetPrivateProfileInt("GameServerInfo","ShieldSystemOn", 0, ReadConfig.ConnDataFiles[0]);

		g_iDamageDevideToSDRate = GetPrivateProfileInt("GameServerInfo","DamageDevideToSD", 90, ReadConfig.ConnDataFiles[0]);
		g_iDamageDevideToHPRate = GetPrivateProfileInt("GameServerInfo","DamageDevideToHP", 10, ReadConfig.ConnDataFiles[0]);
		g_fSuccessAttackRateOption = (double)(GetPrivateProfileInt("GameServerInfo","SuccessAttackRateOption", 20000, ReadConfig.ConnDataFiles[0]) / 10000 );

		g_ShieldAutoRefillOn = GetPrivateProfileInt("GameServerInfo","ShieldAutoRefillOn", 0, ReadConfig.ConnDataFiles[0]);
		g_ShieldAutoRefillOnSafeZone = GetPrivateProfileInt("GameServerInfo","ShieldAutoRefilOnSafeZone", 1, ReadConfig.ConnDataFiles[0]);

	// Compund Potion
	g_CompoundPotionDropOn = GetPrivateProfileInt("GameServerInfo","CompoundPotionDropOn", 0, ReadConfig.ConnDataFiles[0]);

		g_iCompoundPotionLv1DropRate = GetPrivateProfileInt("GameServerInfo","CompoundPotionLv1DropRate", 100, ReadConfig.ConnDataFiles[0]);
		g_iCompoundPotionLv2DropRate = GetPrivateProfileInt("GameServerInfo","CompoundPotionLv2DropRate", 80, ReadConfig.ConnDataFiles[0]);
		g_iCompoundPotionLv3DropRate = GetPrivateProfileInt("GameServerInfo","CompoundPotionLv3DropRate", 50, ReadConfig.ConnDataFiles[0]);

		g_iCompoundPotionLv1DropLevel = GetPrivateProfileInt("GameServerInfo","CompoundPotionLv1DropLevel", 68, ReadConfig.ConnDataFiles[0]);
		g_iCompoundPotionLv2DropLevel = GetPrivateProfileInt("GameServerInfo","CompoundPotionLv2DropLevel", 96, ReadConfig.ConnDataFiles[0]);
		g_iCompoundPotionLv3DropLevel = GetPrivateProfileInt("GameServerInfo","CompoundPotionLv3DropLevel", 118, ReadConfig.ConnDataFiles[0]);

	// Shile System Combo
	g_bShieldComboMissOptionOn = GetPrivateProfileInt("GameServerInfo","ShieldComboMissOptionOn", 0, ReadConfig.ConnDataFiles[0]);

	// Shield GAGE
	g_iShieldGageConstA = GetPrivateProfileInt("GameServerInfo","ShieldGageConstA", 12, ReadConfig.ConnDataFiles[0]);
	g_iShieldGageConstB = GetPrivateProfileInt("GameServerInfo","ShieldGageConstB", 30, ReadConfig.ConnDataFiles[0]);

	// (%s)%s (ServiceServer) %c.%c%c.%c%c (%s)
	wsprintf(szTemp, lMsg.Get(MSGGET(1, 160)), GAMESERVER_VERSION, szServerName, szClientVersion[0], szClientVersion[1], szClientVersion[2], szClientVersion[3], szClientVersion[4], szGameServerExeSerial, "CAD");
	LogAdd("User Object Size = %d", sizeof(OBJECTSTRUCT));

#if (GS_CASTLE==1)
	strcat(szTemp, "[CastleSiege]");
#endif
	char szDiplayGSInfo[512] = {0};
	wsprintfA(szDiplayGSInfo, "%s [%d][%d][%d]", szTemp, dwgCheckSum[1]%1000, dwgCheckSum[17]%1000, dwgCheckSum[1004]%1000);
	SetWindowText(ghWnd, szDiplayGSInfo);

	// Item Bag Load
	LoadItemBag();

	// Eledorado Event Start
	gEledoradoEvent.SetEventState(gIsEledoradoEvent);
	gEledoradoEvent.Init();

	gPacketCheckSum.Init();

	gDoPShopOpen = GetPrivateProfileInt("GameServerInfo","PersonalShopOpen", 0, ReadConfig.ConnDataFiles[0]);

	// AutoRecuperation Level
	g_iUseCharacterAutoRecuperationSystem = GetPrivateProfileInt("GameServerInfo","UseCharacterAutoRecuperationSystem", 0, ReadConfig.ConnDataFiles[0]);
	g_iCharacterRecuperationMaxLevel = GetPrivateProfileInt("GameServerInfo","CharacterRecuperationMaxLevel", 100, ReadConfig.ConnDataFiles[0]);

	// Skill Check Settings
	g_iSkillDistanceCheck = GetPrivateProfileInt("GameServerInfo","SkillDistanceCheck", 0, ReadConfig.ConnDataFiles[0]);
	g_iSkillDistanceCheckTemp = GetPrivateProfileInt("GameServerInfo","SkillDistanceCheckTemp", 2, ReadConfig.ConnDataFiles[0]);
	g_iSkillDistanceKick = GetPrivateProfileInt("GameServerInfo","SkillDistanceKick", 0, ReadConfig.ConnDataFiles[0]);
	g_iSkillDistanceKickCount = GetPrivateProfileInt("GameServerInfo","SkillDistanceKickCount", 5, ReadConfig.ConnDataFiles[0]);
	g_iSkillDiatanceKickCheckTime = GetPrivateProfileInt("GameServerInfo","SkillDistanceKickCheckTime", 10, ReadConfig.ConnDataFiles[0]);


	g_CashShop.CashShopOptioNReload();
	g_CashItemPeriodSystem.Initialize();
	g_CashLotterySystem.Load(ReadConfig.ConnDataFiles[49]);

	if(AlreadyReaded)
	{
#if (PACK_EDITION>=3)
	for ( int n=OBJ_MAXMONSTER;n<OBJ_STARTUSERINDEX;n++)
	{
		if ( gObj[n].Type == OBJ_MONSTER || gObj[n].Type == OBJ_NPC )
		{
			if(gObj[n].IsBot >= 1 && gObj[n].BotNumOwner <= 0 && gObj[n].m_RecallMon <= 0)
			{
				if ( gObj[n].m_iCurrentAI )
				{
					gObj[n].Live = FALSE;
					gObjViewportListProtocolDestroy(&gObj[n]);
					gObjViewportClose(&gObj[n]);
				}
				gObjCallMonCount--;
				gObjDel(n);
			}
		}
	}
	BotStore.MakeBot();
	BotBuff.MakeBot();
	BotAlchemist.MakeBot();
	BotTrader.MakeBot();
	BotReward.MakeBot();
	BotWarper.MakeBot();
	BotRacer.ForceClose = true;
	#if (CRYSTAL_EDITION==1)	
		BotVipShop.MakeBot();
	#endif
#endif
	}
	AlreadyReaded = true;
}




void GameServerInfoSendStop()
{
	GSInfoSendFlag = 0;
}

void GameServerInfoSendStart()
{
	GSInfoSendFlag = 1;
}





struct PMSG_SERVERINFO
{
	PBMSG_HEAD h;	// C1:01
	short ServerCode;	// 4
	BYTE Percent;	// 6
	short UserCount;	// 8
	short AccountCount;	// A
	short PCbangCount;	// C
	short MaxUserCount;	// E
};


void GameServerInfoSend()
{
	PMSG_SERVERINFO pMsg;

	if ( gDisconnect == TRUE )
	{
		return;
	}

	pMsg.h.c = 0xC1;
	pMsg.h.headcode = 0x01;
	pMsg.h.size = sizeof( pMsg );

	if ( gObjTotalUser == 0 )
	{
		pMsg.Percent = 0;
	}
	else
	{
		pMsg.Percent = (gObjTotalUser * 100)/gServerMaxUser;
	}

	pMsg.ServerCode = gGameServerCode;
	pMsg.UserCount = gObjTotalUser;
	pMsg.AccountCount = gLCount[0].Get();
	pMsg.PCbangCount = gLCount[1].Get();
	pMsg.MaxUserCount = gServerMaxUser;

	gUdpSoc.SendData((LPBYTE)&pMsg, pMsg.h.size);
}

void CheckSumFileLoad(char * szCheckSum)
{
	FILE * fstream1;
	unsigned long FileLen ;
	char * FileBuffer;

	if  ( fstream1=fopen(szCheckSum,"rb") )
	{
		fseek(fstream1,0,SEEK_END);
		if (FileLen=ftell(fstream1) )
		{
			fseek(fstream1,0,SEEK_SET ) ;
			FileBuffer =new char[FileLen+1];
			fread(FileBuffer,FileLen,1,fstream1);
			memcpy(dwgCheckSum, FileBuffer, FileLen);
		}
		else
		{
			MessageBox( NULL, szCheckSum,"Error!", MB_OK );
			::ExitProcess(0);
		}
		fclose(fstream1);
	}
}

//void CheckSumFileLoad(char * szCheckSum)
//{
//	int DataBufferSize;
//	char* DataBuffer;
//
//	gGameServerAuth.RequestData(6);
//	DataBufferSize=gGameServerAuth.GetDataBufferSize();
//	DataBuffer=gGameServerAuth.GetDataBuffer();
//	memcpy(dwgCheckSum, DataBuffer, DataBufferSize);
//}


void LoadItemBag()
{
	if ( LuckboxItemBag != FALSE )
	{
		delete LuckboxItemBag;
	}

	LuckboxItemBag = new CItemBag;

	if ( LuckboxItemBag == NULL )
	{
		MsgBox( "CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	LuckboxItemBag->Init(ReadConfig.ConnEventDropDataFiles[0]);//"eventitembag.txt");

	if ( Mon55 != FALSE )	// Death king
	{
		delete Mon55;
	}

	Mon55 = new CItemBag;

	if ( Mon55 == NULL )
	{
		MsgBox( "CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	Mon55->Init(ReadConfig.ConnEventDropDataFiles[1]);//"eventitembag2.txt");

	if ( Mon53 != FALSE )	// Golden Titan
	{
		delete Mon53;
	}

	Mon53 = new CItemBag;

	if ( Mon53 == NULL )
	{
		MsgBox( "CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	Mon53->Init(ReadConfig.ConnEventDropDataFiles[2]);//"eventitembag3.txt");

	if ( StarOfXMasItemBag != FALSE )	
	{
		delete StarOfXMasItemBag;
	}

	StarOfXMasItemBag = new CItemBagEx;

	if ( StarOfXMasItemBag == NULL )
	{
		MsgBox( "CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	StarOfXMasItemBag->Init(ReadConfig.ConnEventDropDataFiles[3]);//"eventitembag4.txt");

	if ( FireCrackerItemBag != FALSE )	
	{
		delete FireCrackerItemBag;
	}

	FireCrackerItemBag = new CItemBag;

	if ( FireCrackerItemBag == NULL )
	{
		MsgBox( "CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	FireCrackerItemBag->Init(ReadConfig.ConnEventDropDataFiles[4]);//"eventitembag5.txt");

	if ( HeartOfLoveItemBag != FALSE )	
	{
		delete HeartOfLoveItemBag;
	}

	HeartOfLoveItemBag = new CItemBag;

	if ( HeartOfLoveItemBag == NULL )
	{
		MsgBox( "CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	HeartOfLoveItemBag->Init(ReadConfig.ConnEventDropDataFiles[5]);//"eventitembag5.txt");

	if ( GoldMedalItemBag != FALSE )	
	{
		delete GoldMedalItemBag;
	}

	GoldMedalItemBag = new CItemBag;

	if ( GoldMedalItemBag == NULL )
	{
		MsgBox( "CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	GoldMedalItemBag->Init(ReadConfig.ConnEventDropDataFiles[6]);//"eventitembag6.txt");

	if ( SilverMedalItemBag != FALSE )	
	{
		delete SilverMedalItemBag;
	}

	SilverMedalItemBag = new CItemBag;

	if ( SilverMedalItemBag == NULL )
	{
		MsgBox( "CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	SilverMedalItemBag->Init(ReadConfig.ConnEventDropDataFiles[7]);//"eventitembag7.txt");

	if ( GoldGoblenItemBag != FALSE )	
	{
		delete GoldGoblenItemBag;
	}

	GoldGoblenItemBag = new CItemBag;

	if ( GoldGoblenItemBag == NULL )
	{
		MsgBox( "CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	GoldGoblenItemBag->Init(ReadConfig.ConnEventDropDataFiles[8]);//"eventitembag8.txt");

	if ( TitanItemBag != FALSE )	
	{
		delete TitanItemBag;
	}

	TitanItemBag = new CItemBag;

	if ( TitanItemBag == NULL )
	{
		MsgBox( "CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	TitanItemBag->Init(ReadConfig.ConnEventDropDataFiles[9]);//"eventitembag9.txt");

	if ( GoldDerconItemBag != FALSE )	
	{
		delete GoldDerconItemBag;
	}

	GoldDerconItemBag = new CItemBag;

	if ( GoldDerconItemBag == NULL )
	{
		MsgBox( "CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	GoldDerconItemBag->Init(ReadConfig.ConnEventDropDataFiles[10]);//"eventitembag10.txt");

	if ( DevilLizardKingItemBag != FALSE )	
	{
		delete DevilLizardKingItemBag;
	}

	DevilLizardKingItemBag = new CItemBag;

	if ( DevilLizardKingItemBag == NULL )
	{
		MsgBox( "CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	DevilLizardKingItemBag->Init(ReadConfig.ConnEventDropDataFiles[11]);//"eventitembag11.txt");

	if ( KanturItemBag != FALSE )	
	{
		delete KanturItemBag;
	}

	KanturItemBag = new CItemBag;

	if ( KanturItemBag == NULL )
	{
		MsgBox( "CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	KanturItemBag->Init(ReadConfig.ConnEventDropDataFiles[12]);//"eventitembag12.txt");

	if ( RingEventItemBag != FALSE )	
	{
		delete RingEventItemBag;
	}

	RingEventItemBag = new CItemBag;

	if ( RingEventItemBag == NULL )
	{
		MsgBox( "CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	RingEventItemBag->Init(ReadConfig.ConnEventDropDataFiles[13]);//"eventitembag13.txt");

	if ( FriendShipItemBag != FALSE )	
	{
		delete FriendShipItemBag;
	}

	FriendShipItemBag = new CItemBag;

	if ( FriendShipItemBag == NULL )
	{
		MsgBox( "CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	FriendShipItemBag->Init(ReadConfig.ConnEventDropDataFiles[14]);//"eventitembag14.txt");

	if ( DarkLordHeartItemBag != FALSE )	
	{
		delete DarkLordHeartItemBag;
	}

	DarkLordHeartItemBag = new CItemBag;

	if ( DarkLordHeartItemBag == NULL )
	{
		MsgBox( "CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	DarkLordHeartItemBag->Init(ReadConfig.ConnEventDropDataFiles[15]);//"eventitembag15.txt");

	if ( KundunEventItemBag != FALSE )	
	{
		delete KundunEventItemBag;
	}

	KundunEventItemBag = new CItemBagEx;

	if ( KundunEventItemBag == NULL )
	{
		MsgBox( "CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	KundunEventItemBag->Init(ReadConfig.ConnEventDropDataFiles[16]);//"eventitembag17.txt");

#if (GS_CASTLE==1)
	if ( ErohimCastleZoneItemBag != NULL )
		delete ErohimCastleZoneItemBag;

	ErohimCastleZoneItemBag = new CItemBagEx; 
	if ( ErohimCastleZoneItemBag == NULL )
	{
		// Memory allocation error
		MsgBox("CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	ErohimCastleZoneItemBag->Init(ReadConfig.ConnEventDropDataFiles[41]);//"eventitembag18.txt");

	//Monsters in CastleDeep Event
	if ( HuntZoneItemBag != NULL )
		delete HuntZoneItemBag;

	HuntZoneItemBag = new CItemBagEx; 
	if ( HuntZoneItemBag == NULL )
	{
		// Memory allocation error
		MsgBox("CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	HuntZoneItemBag->Init(ReadConfig.ConnEventDropDataFiles[42]);//"eventitembag18.txt");

	//GS_CASTLE EventItemBag for Special Mix
	if ( CastleItemMixItemBag != NULL )
		delete CastleItemMixItemBag;

	CastleItemMixItemBag = new CItemBagEx; 
	if ( CastleItemMixItemBag == NULL )
	{
		// Memory allocation error
		MsgBox("CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	CastleItemMixItemBag->Init(ReadConfig.ConnEventDropDataFiles[40]);//"eventitembag19.txt");
#endif

	if ( HiddenTreasureBoxItemBag != NULL )
		delete HiddenTreasureBoxItemBag;

	HiddenTreasureBoxItemBag = new CItemBagEx; 
	if ( HiddenTreasureBoxItemBag == NULL )
	{
		// Memory allocation error
		MsgBox("CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	HiddenTreasureBoxItemBag->Init(ReadConfig.ConnEventDropDataFiles[17]);//"eventitembag20.txt");

	if ( RedRibbonBoxEventItemBag != NULL )
		delete RedRibbonBoxEventItemBag;

	RedRibbonBoxEventItemBag = new CItemBagEx; 
	if ( RedRibbonBoxEventItemBag == NULL )
	{
		// Memory allocation error
		MsgBox("CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	RedRibbonBoxEventItemBag->Init(ReadConfig.ConnEventDropDataFiles[18]);//"eventitembag21.txt");

	if ( GreenRibbonBoxEventItemBag != NULL )
		delete GreenRibbonBoxEventItemBag;

	GreenRibbonBoxEventItemBag = new CItemBagEx; 
	if ( GreenRibbonBoxEventItemBag == NULL )
	{
		// Memory allocation error
		MsgBox("CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	GreenRibbonBoxEventItemBag->Init(ReadConfig.ConnEventDropDataFiles[19]);//"eventitembag22.txt");

	if ( BlueRibbonBoxEventItemBag != NULL )
		delete BlueRibbonBoxEventItemBag;

	BlueRibbonBoxEventItemBag = new CItemBagEx; 
	if ( BlueRibbonBoxEventItemBag == NULL )
	{
		// Memory allocation error
		MsgBox("CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	BlueRibbonBoxEventItemBag->Init(ReadConfig.ConnEventDropDataFiles[20]);//"eventitembag23.txt");

	if ( PinkChocolateBoxEventItemBag != NULL )
		delete PinkChocolateBoxEventItemBag;

	PinkChocolateBoxEventItemBag = new CItemBagEx; 
	if ( PinkChocolateBoxEventItemBag == NULL )
	{
		// Memory allocation error
		MsgBox("CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	PinkChocolateBoxEventItemBag->Init(ReadConfig.ConnEventDropDataFiles[21]);//"eventitembag24.txt");

	if ( RedChocolateBoxEventItemBag != NULL )
		delete RedChocolateBoxEventItemBag;

	RedChocolateBoxEventItemBag = new CItemBagEx; 
	if ( RedChocolateBoxEventItemBag == NULL )
	{
		// Memory allocation error
		MsgBox("CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	RedChocolateBoxEventItemBag->Init(ReadConfig.ConnEventDropDataFiles[22]);//"eventitembag25.txt");

	if ( BlueChocolateBoxEventItemBag != NULL )
		delete BlueChocolateBoxEventItemBag;

	BlueChocolateBoxEventItemBag = new CItemBagEx; 
	if ( BlueChocolateBoxEventItemBag == NULL )
	{
		// Memory allocation error
		MsgBox("CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	BlueChocolateBoxEventItemBag->Init(ReadConfig.ConnEventDropDataFiles[23]);//"eventitembag26.txt");

	if ( LightPurpleCandyBoxEventItemBag != NULL )
		delete LightPurpleCandyBoxEventItemBag;

	LightPurpleCandyBoxEventItemBag = new CItemBagEx; 
	if ( LightPurpleCandyBoxEventItemBag == NULL )
	{
		// Memory allocation error
		MsgBox("CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	LightPurpleCandyBoxEventItemBag->Init(ReadConfig.ConnEventDropDataFiles[24]);//"eventitembag27.txt");

	if ( VermilionCandyBoxEventItemBag != NULL )
		delete VermilionCandyBoxEventItemBag;

	VermilionCandyBoxEventItemBag = new CItemBagEx; 
	if ( VermilionCandyBoxEventItemBag == NULL )
	{
		// Memory allocation error
		MsgBox("CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	VermilionCandyBoxEventItemBag->Init(ReadConfig.ConnEventDropDataFiles[25]);//"eventitembag28.txt");

	if ( DeepBlueCandyBoxEventItemBag != NULL )
		delete DeepBlueCandyBoxEventItemBag;

	DeepBlueCandyBoxEventItemBag = new CItemBagEx; 
	if ( DeepBlueCandyBoxEventItemBag == NULL )
	{
		// Memory allocation error
		MsgBox("CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	DeepBlueCandyBoxEventItemBag->Init(ReadConfig.ConnEventDropDataFiles[26]);//"eventitembag29.txt");

	if ( CrywolfDarkElfItemBag != NULL )
		delete CrywolfDarkElfItemBag;

	CrywolfDarkElfItemBag = new CItemBagEx; 
	if ( CrywolfDarkElfItemBag == NULL )
	{
		// Memory allocation error
		MsgBox("CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	CrywolfDarkElfItemBag->Init(ReadConfig.ConnEventDropDataFiles[27]);//"eventitembag30.txt");

	if ( CrywolfBossMonsterItemBag != NULL )
		delete CrywolfBossMonsterItemBag;

	CrywolfBossMonsterItemBag = new CItemBagEx; 
	if ( CrywolfBossMonsterItemBag == NULL )
	{
		// Memory allocation error
		MsgBox("CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	CrywolfBossMonsterItemBag->Init(ReadConfig.ConnEventDropDataFiles[28]);//"eventitembag31.txt");

	if ( KanturuMayaHandItemBag != NULL )
		delete KanturuMayaHandItemBag;

	KanturuMayaHandItemBag = new CItemBagEx; 
	if ( KanturuMayaHandItemBag == NULL )
	{
		// Memory allocation error
		MsgBox("CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	KanturuMayaHandItemBag->Init(ReadConfig.ConnEventDropDataFiles[29]);//"eventitembag32.txt");

	if ( KanturuNightmareItemBag != NULL )
		delete KanturuNightmareItemBag;

	KanturuNightmareItemBag = new CItemBagEx; 
	if ( KanturuNightmareItemBag == NULL )
	{
		// Memory allocation error
		MsgBox("CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	KanturuNightmareItemBag->Init(ReadConfig.ConnEventDropDataFiles[30]);//"eventitembag33.txt");

	if ( HalloweenDayEventItemBag != NULL )
		delete HalloweenDayEventItemBag;

	HalloweenDayEventItemBag = new CItemBagEx; 
	if ( HalloweenDayEventItemBag == NULL )
	{
		// Memory allocation error
		MsgBox("CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	HalloweenDayEventItemBag->Init(ReadConfig.ConnEventDropDataFiles[31]);//"eventitembag34.txt");

	//NEW

	if ( SelupanEventItemBag != FALSE )	
	{
		delete SelupanEventItemBag;
	}

	SelupanEventItemBag = new CItemBagEx;

	if ( SelupanEventItemBag == NULL )
	{
		MsgBox( "CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	SelupanEventItemBag->Init(ReadConfig.ConnEventDropDataFiles[32]);//"eventitembag40.txt");

#if (PACK_EDITION>=1)
	//NEW

	if ( BlueEventItemBag != FALSE )	
	{
		delete BlueEventItemBag;
	}

	BlueEventItemBag = new CItemBagEx;

	if ( BlueEventItemBag == NULL )
	{
		MsgBox( "CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	BlueEventItemBag->Init(ReadConfig.ConnEventDropDataFiles[33]);//"eventitembag41.txt");
#endif
	//NEW

#if (PACK_EDITION>=3)
	if ( BossAttackItemBag != FALSE )	
	{
		delete BossAttackItemBag;
	}

	BossAttackItemBag = new CItemBagEx;

	if ( BossAttackItemBag == NULL )
	{
		MsgBox( "CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	BossAttackItemBag->Init(ReadConfig.ConnEventDropDataFiles[34]);//"eventitembag42.txt");
#endif
	if ( GreenMysteryEventItemBag != FALSE )	
	{
		delete GreenMysteryEventItemBag;
	}

	GreenMysteryEventItemBag = new CItemBagEx;

	if ( GreenMysteryEventItemBag == NULL )
	{
		MsgBox( "CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	GreenMysteryEventItemBag->Init(ReadConfig.ConnEventDropDataFiles[35]);//"eventitembag35.txt");

	if ( RedMysteryEventItemBag != FALSE )	
	{
		delete RedMysteryEventItemBag;
	}

	RedMysteryEventItemBag = new CItemBagEx;

	if ( RedMysteryEventItemBag == NULL )
	{
		MsgBox( "CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	RedMysteryEventItemBag->Init(ReadConfig.ConnEventDropDataFiles[36]);//"eventitembag36.txt");

	//NEW

	if ( PurpleMysteryEventItemBag != FALSE )	
	{
		delete PurpleMysteryEventItemBag;
	}

	PurpleMysteryEventItemBag = new CItemBagEx;

	if ( PurpleMysteryEventItemBag == NULL )
	{
		MsgBox( "CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	PurpleMysteryEventItemBag->Init(ReadConfig.ConnEventDropDataFiles[37]);//"eventitembag37.txt");

	//NEW

	if ( CherryBlossomEventItemBag != FALSE )	
	{
		delete CherryBlossomEventItemBag;
	}

	CherryBlossomEventItemBag = new CItemBagEx;

	if ( CherryBlossomEventItemBag == NULL )
	{
		MsgBox( "CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	CherryBlossomEventItemBag->Init(ReadConfig.ConnEventDropDataFiles[38]);//"eventitembag38.txt");

	//NEW

	if ( GMEventItemBag != FALSE )	
	{
		delete GMEventItemBag;
	}

	GMEventItemBag = new CItemBagEx;

	if ( GMEventItemBag == NULL )
	{
		MsgBox( "CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	GMEventItemBag->Init(ReadConfig.ConnEventDropDataFiles[39]);//"eventitembag39.txt");

	//NEW

	if ( IGSundayItemBag2 != FALSE )	
	{
		delete IGSundayItemBag2;
	}

	IGSundayItemBag2 = new CItemBagEx;

	if ( IGSundayItemBag2 == NULL )
	{
		MsgBox( "CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	IGSundayItemBag2->Init(ReadConfig.ConnEventDropDataFiles[43]);//"eventitembag43.txt");

	//NEW

	if ( IGSundayItemBag1 != FALSE )	
	{
		delete IGSundayItemBag1;
	}

	IGSundayItemBag1 = new CItemBagEx;

	if ( IGSundayItemBag1 == NULL )
	{
		MsgBox( "CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	IGSundayItemBag1->Init(ReadConfig.ConnEventDropDataFiles[44]);//"eventitembag44.txt");

	//NEW

	if ( IGMondayItemBag != FALSE )	
	{
		delete IGMondayItemBag;
	}

	IGMondayItemBag = new CItemBagEx;

	if ( IGMondayItemBag == NULL )
	{
		MsgBox( "CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	IGMondayItemBag->Init(ReadConfig.ConnEventDropDataFiles[45]);//"eventitembag45.txt");

	//NEW

	if ( IGTuesdayItemBag != FALSE )	
	{
		delete IGTuesdayItemBag;
	}

	IGTuesdayItemBag = new CItemBagEx;

	if ( IGTuesdayItemBag == NULL )
	{
		MsgBox( "CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	IGTuesdayItemBag->Init(ReadConfig.ConnEventDropDataFiles[46]);//"eventitembag46.txt");

	//NEW

	if ( IGWednesdayItemBag != FALSE )	
	{
		delete IGWednesdayItemBag;
	}

	IGWednesdayItemBag = new CItemBagEx;

	if ( IGWednesdayItemBag == NULL )
	{
		MsgBox( "CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	IGWednesdayItemBag->Init(ReadConfig.ConnEventDropDataFiles[47]);//"eventitembag47.txt");

	//NEW

	if ( IGThursdayItemBag != FALSE )	
	{
		delete IGThursdayItemBag;
	}

	IGThursdayItemBag = new CItemBagEx;

	if ( IGThursdayItemBag == NULL )
	{
		MsgBox( "CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	IGThursdayItemBag->Init(ReadConfig.ConnEventDropDataFiles[48]);//"eventitembag48.txt");

	//NEW

	if ( IGFridayItemBag != FALSE )	
	{
		delete IGFridayItemBag;
	}

	IGFridayItemBag = new CItemBagEx;

	if ( IGFridayItemBag == NULL )
	{
		MsgBox( "CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	IGFridayItemBag->Init(ReadConfig.ConnEventDropDataFiles[49]);//"eventitembag49.txt");

	//NEW

	if ( IGSaturdayItemBag != FALSE )	
	{
		delete IGSaturdayItemBag;
	}

	IGSaturdayItemBag = new CItemBagEx;

	if ( IGSaturdayItemBag == NULL )
	{
		MsgBox( "CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	IGSaturdayItemBag->Init(ReadConfig.ConnEventDropDataFiles[50]);//"eventitembag50.txt");

	//NEW

	
	if ( DGBoss1ItemBag != FALSE )	
	{
		delete DGBoss1ItemBag;
	}

	DGBoss1ItemBag = new CItemBagEx;

	if ( DGBoss1ItemBag == NULL )
	{
		MsgBox( "CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	DGBoss1ItemBag->Init(ReadConfig.ConnEventDropDataFiles[51]);//"eventitembag51.txt");

	//NEW

	if ( DGBoss2ItemBag != FALSE )	
	{
		delete DGBoss2ItemBag;
	}

	DGBoss2ItemBag = new CItemBagEx;

	if ( DGBoss2ItemBag == NULL )
	{
		MsgBox( "CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	DGBoss2ItemBag->Init(ReadConfig.ConnEventDropDataFiles[52]);//"eventitembag52.txt");

	//NEW

	if ( DGBoss3ItemBag != FALSE )	
	{
		delete DGBoss3ItemBag;
	}

	DGBoss3ItemBag = new CItemBagEx;

	if ( DGBoss3ItemBag == NULL )
	{
		MsgBox( "CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	DGBoss3ItemBag->Init(ReadConfig.ConnEventDropDataFiles[53]);//"eventitembag53.txt");

	//NEW

	if ( DGTreasureItemBag != FALSE )	
	{
		delete DGTreasureItemBag;
	}

	DGTreasureItemBag = new CItemBagEx;

	if ( DGTreasureItemBag == NULL )
	{
		MsgBox( "CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	DGTreasureItemBag->Init(ReadConfig.ConnEventDropDataFiles[54]);//"eventitembag54.txt");

	//NEW

	if ( FortunePouchItemBag != FALSE )	
	{
		delete FortunePouchItemBag;
	}

	FortunePouchItemBag = new CItemBagEx;

	if ( FortunePouchItemBag == NULL )
	{
		MsgBox( "CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	FortunePouchItemBag->Init(ReadConfig.ConnEventDropDataFiles[55]);//"eventitembag55.txt");

	//NEW
#if (PACK_EDITION>=2)
	if ( XMasEventItemBag != FALSE )	
	{
		delete XMasEventItemBag;
	}

	XMasEventItemBag = new CItemBagEx;

	if ( XMasEventItemBag == NULL )
	{
		MsgBox( "CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}
	XMasEventItemBag->Init(ReadConfig.ConnEventDropDataFiles[56]);//"eventitembag56.txt");
#endif

	//NEW

	if ( DGSilverTreasureItemBag != FALSE )	
	{
		delete DGSilverTreasureItemBag;
	}

	DGSilverTreasureItemBag = new CItemBagEx;

	if ( DGSilverTreasureItemBag == NULL )
	{
		MsgBox( "CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	DGSilverTreasureItemBag->Init(ReadConfig.ConnEventDropDataFiles[57]);//"eventitembag57.txt");
	//NEW

#if (PACK_EDITION>=3)
	if ( SwampEventItemBag != FALSE )	
	{
		delete SwampEventItemBag;
	}

	SwampEventItemBag = new CItemBagEx;

	if ( SwampEventItemBag == NULL )
	{
		MsgBox( "CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	SwampEventItemBag->Init(ReadConfig.ConnEventDropDataFiles[58]);//"eventitembag58.txt");
#endif
	//
	if ( GoldRabbitItemBag != FALSE )	
	{
		delete GoldRabbitItemBag;
	}

	GoldRabbitItemBag = new CItemBag;

	if ( GoldRabbitItemBag == NULL )
	{
		MsgBox( "CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	GoldRabbitItemBag->Init(ReadConfig.ConnEventDropDataFiles[59]);//"eventitembag59.txt");

	//
	if ( GoldDarkKnightItemBag != FALSE )	
	{
		delete GoldDarkKnightItemBag;
	}

	GoldDarkKnightItemBag = new CItemBag;

	if ( GoldDarkKnightItemBag == NULL )
	{
		MsgBox( "CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	GoldDarkKnightItemBag->Init(ReadConfig.ConnEventDropDataFiles[60]);//"eventitembag60.txt");

	//
	if ( GoldDevilItemBag != FALSE )	
	{
		delete GoldDevilItemBag;
	}

	GoldDevilItemBag = new CItemBag;

	if ( GoldDevilItemBag == NULL )
	{
		MsgBox( "CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	GoldDevilItemBag->Init(ReadConfig.ConnEventDropDataFiles[61]);//"eventitembag61.txt");

	//
	if ( GoldStoneGolemItemBag != FALSE )	
	{
		delete GoldStoneGolemItemBag;
	}

	GoldStoneGolemItemBag = new CItemBag;

	if ( GoldStoneGolemItemBag == NULL )
	{
		MsgBox( "CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	GoldStoneGolemItemBag->Init(ReadConfig.ConnEventDropDataFiles[62]);//"eventitembag62.txt");

	//
	if ( GoldCrustItemBag != FALSE )	
	{
		delete GoldCrustItemBag;
	}

	GoldCrustItemBag = new CItemBag;

	if ( GoldCrustItemBag == NULL )
	{
		MsgBox( "CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	GoldCrustItemBag->Init(ReadConfig.ConnEventDropDataFiles[63]);//"eventitembag63.txt");

	//
	if ( GoldSatyrosItemBag != FALSE )	
	{
		delete GoldSatyrosItemBag;
	}

	GoldSatyrosItemBag = new CItemBag;

	if ( GoldSatyrosItemBag == NULL )
	{
		MsgBox( "CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	GoldSatyrosItemBag->Init(ReadConfig.ConnEventDropDataFiles[64]);//"eventitembag64.txt");

	//
	if ( GoldTwinTaleItemBag != FALSE )	
	{
		delete GoldTwinTaleItemBag;
	}

	GoldTwinTaleItemBag = new CItemBag;

	if ( GoldTwinTaleItemBag == NULL )
	{
		MsgBox( "CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	GoldTwinTaleItemBag->Init(ReadConfig.ConnEventDropDataFiles[65]);//"eventitembag65.txt");

	//
	if ( GoldIronKnightItemBag != FALSE )	
	{
		delete GoldIronKnightItemBag;
	}

	GoldIronKnightItemBag = new CItemBag;

	if ( GoldIronKnightItemBag == NULL )
	{
		MsgBox( "CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	GoldIronKnightItemBag->Init(ReadConfig.ConnEventDropDataFiles[66]);//"eventitembag66.txt");

	//
	if ( GoldNapinItemBag != FALSE )	
	{
		delete GoldNapinItemBag;
	}

	GoldNapinItemBag = new CItemBag;

	if ( GoldNapinItemBag == NULL )
	{
		MsgBox( "CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	GoldNapinItemBag->Init(ReadConfig.ConnEventDropDataFiles[67]);//"eventitembag67.txt");

	//
	if ( GoldGreatDragonItemBag != FALSE )	
	{
		delete GoldGreatDragonItemBag;
	}

	GoldGreatDragonItemBag = new CItemBag;

	if ( GoldGreatDragonItemBag == NULL )
	{
		MsgBox( "CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	GoldGreatDragonItemBag->Init(ReadConfig.ConnEventDropDataFiles[68]);//"eventitembag68.txt");

	//
	//NEW

	if ( ElegantJewerlyItemBag != FALSE )	
	{
		delete ElegantJewerlyItemBag;
	}

	ElegantJewerlyItemBag = new CItemBagEx;

	if ( ElegantJewerlyItemBag == NULL )
	{
		MsgBox( "CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	ElegantJewerlyItemBag->Init(ReadConfig.ConnEventDropDataFiles[69]);//"eventitembag69.txt");
	//NEW

	if ( MetalJewerlyItemBag != FALSE )	
	{
		delete MetalJewerlyItemBag;
	}

	MetalJewerlyItemBag = new CItemBagEx;

	if ( MetalJewerlyItemBag == NULL )
	{
		MsgBox( "CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	MetalJewerlyItemBag->Init(ReadConfig.ConnEventDropDataFiles[70]);//"eventitembag70.txt");
	//NEW

	if ( OldJewerlyItemBag != FALSE )	
	{
		delete OldJewerlyItemBag;
	}

	OldJewerlyItemBag = new CItemBagEx;

	if ( OldJewerlyItemBag == NULL )
	{
		MsgBox( "CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	OldJewerlyItemBag->Init(ReadConfig.ConnEventDropDataFiles[71]);//"eventitembag71.txt");
	//NEW

	if ( GoldBoxItemBag != FALSE )	
	{
		delete GoldBoxItemBag;
	}

	GoldBoxItemBag = new CItemBagEx;

	if ( GoldBoxItemBag == NULL )
	{
		MsgBox( "CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	GoldBoxItemBag->Init(ReadConfig.ConnEventDropDataFiles[72]);//"eventitembag72.txt");
	//NEW

	if ( SilverBoxItemBag != FALSE )	
	{
		delete SilverBoxItemBag;
	}

	SilverBoxItemBag = new CItemBagEx;

	if ( SilverBoxItemBag == NULL )
	{
		MsgBox( "CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	SilverBoxItemBag->Init(ReadConfig.ConnEventDropDataFiles[73]);//"eventitembag73.txt");

#if (PACK_EDITION>=1)
	//NEW

	if ( SummerEventItemBag != FALSE )	
	{
		delete SummerEventItemBag;
	}

	SummerEventItemBag = new CItemBagEx;

	if ( SummerEventItemBag == NULL )
	{
		MsgBox( "CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	SummerEventItemBag->Init(ReadConfig.ConnEventDropDataFiles[74]);//"eventitembag74.txt");
#endif

#if (PACK_EDITION>=2)
	if ( HalloweenPKEventItemBag != FALSE )	
	{
		delete HalloweenPKEventItemBag;
	}

	HalloweenPKEventItemBag = new CItemBagEx;

	if ( HalloweenPKEventItemBag == NULL )
	{
		MsgBox( "CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	HalloweenPKEventItemBag->Init(ReadConfig.ConnEventDropDataFiles[75]);//"eventitembag75.txt");
#endif
	if ( HellMainItemBag != FALSE )	
	{
		delete HellMainItemBag;
	}

	HellMainItemBag = new CItemBagEx;

	if ( HellMainItemBag == NULL )
	{
		MsgBox( "CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	HellMainItemBag->Init(ReadConfig.ConnEventDropDataFiles[76]);//"eventitembag76.txt");

	if ( IllusionItemBag != FALSE )	
	{
		delete IllusionItemBag;
	}

	IllusionItemBag = new CItemBag;

	if ( IllusionItemBag == NULL )
	{
		MsgBox( "CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	IllusionItemBag->Init(ReadConfig.ConnEventDropDataFiles[77]);//"eventitembag77.txt");
	//NEW

	if ( GreenBoxEventItemBag != FALSE )	
	{
		delete GreenBoxEventItemBag;
	}

	GreenBoxEventItemBag = new CItemBagEx;

	if ( GreenBoxEventItemBag == NULL )
	{
		MsgBox( "CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	GreenBoxEventItemBag->Init(ReadConfig.ConnEventDropDataFiles[78]);//"eventitembag78.txt");
	//NEW

	if ( RedBoxEventItemBag != FALSE )	
	{
		delete RedBoxEventItemBag;
	}

	RedBoxEventItemBag = new CItemBagEx;

	if ( RedBoxEventItemBag == NULL )
	{
		MsgBox( "CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	RedBoxEventItemBag->Init(ReadConfig.ConnEventDropDataFiles[79]);//"eventitembag79.txt");
	//NEW

	if ( PurpleBoxEventItemBag != FALSE )	
	{
		delete PurpleBoxEventItemBag;
	}

	PurpleBoxEventItemBag = new CItemBagEx;

	if ( PurpleBoxEventItemBag == NULL )
	{
		MsgBox( "CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	PurpleBoxEventItemBag->Init(ReadConfig.ConnEventDropDataFiles[80]);//"eventitembag80.txt");

	//NEW STATUE

	if ( IGStatueItemBag != FALSE )	
	{
		delete IGStatueItemBag;
	}

	IGStatueItemBag = new CItemBagEx;

	if ( IGStatueItemBag == NULL )
	{
		MsgBox( "CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	IGStatueItemBag->Init(ReadConfig.ConnEventDropDataFiles[81]);

	if ( CrywolfPedestalRewardItemBag != FALSE )
	{
		delete CrywolfPedestalRewardItemBag;
	}

	CrywolfPedestalRewardItemBag = new CItemBagEx;

	if ( CrywolfPedestalRewardItemBag == NULL )
	{
		MsgBox( "CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	CrywolfPedestalRewardItemBag->Init(ReadConfig.ConnEventDropDataFiles[82]);

	//NEW

	if ( RainItemsEvent != FALSE )	
	{
		delete RainItemsEvent;
	}

	RainItemsEvent = new CItemBagEx;

	if ( RainItemsEvent == NULL )
	{
		MsgBox( "CItemBag %s", lMsg.Get(MSGGET(0, 110)));
		return;
	}

	RainItemsEvent->Init(ReadConfig.ConnEventDropDataFiles[83]);//"eventitembag78.txt");

}



void SetMapName()
{
	strcpy(gMapName[0], lMsg.Get(MSGGET(7,208)));
	strcpy(gMapName[1], lMsg.Get(MSGGET(7,209)));
	strcpy(gMapName[2], lMsg.Get(MSGGET(7,210)));
	strcpy(gMapName[3], lMsg.Get(MSGGET(7,211)));
	strcpy(gMapName[4], lMsg.Get(MSGGET(7,212)));
	strcpy(gMapName[5], "Reserved");
	strcpy(gMapName[6], lMsg.Get(MSGGET(7,214)));
	strcpy(gMapName[7], lMsg.Get(MSGGET(7,213)));
	strcpy(gMapName[8], lMsg.Get(MSGGET(7,216)));
	strcpy(gMapName[9], lMsg.Get(MSGGET(7,217)));
	strcpy(gMapName[10], lMsg.Get(MSGGET(7,221)));
	strcpy(gMapName[11], lMsg.Get(MSGGET(7,222)));
	strcat(gMapName[11], "1");
	strcpy(gMapName[12], lMsg.Get(MSGGET(7,222)));
	strcat(gMapName[12], "2");
	strcpy(gMapName[13], lMsg.Get(MSGGET(7,222)));
	strcat(gMapName[13], "3");
	strcpy(gMapName[14], lMsg.Get(MSGGET(7,222)));
	strcat(gMapName[14], "4");
	strcpy(gMapName[15], lMsg.Get(MSGGET(7,222)));
	strcat(gMapName[15], "5");
	strcpy(gMapName[16], lMsg.Get(MSGGET(7,222)));
	strcat(gMapName[16], "6");
}

/*
struct ST_EVENT_FLAG
{
	BOOL bDevilSquare : 1;
	BOOL bBloodCastle : 1;
	BOOL			  : 6;
	BOOL bXMaxEvent   : 1;
	BOOL bFireCracker : 1;
	BOOL bHeartOfLove : 1;
	BOOL bMedalEvent  : 1;
	BOOL bRingEvent   : 1;
	BOOL bEventChip   : 1;
	BOOL bEledorado   : 1;
	BOOL bEledorado2  : 1;
	BOOL			  : 1;
	BOOL bNPGGChecksum: 1;
};

union STU_EVENT_FLAG
{
	ST_EVENT_FLAG EventData;
	DWORD AllData;
};


// Check what functions are enabled to send 
// Authentication Server (KOREA NON_REGISTERED SERVERS)
int GetEventFlag()
{
	STU_EVENT_FLAG flag;

	flag.AllData = 0;

	if (gDevilSquareEvent!=0)
	{
		flag.EventData.bDevilSquare=1;
	}
	if (g_bBloodCastle!=0)
	{
		flag.EventData.bBloodCastle=1;
	}
	if (gXMasEvent!=0)
	{
		flag.EventData.bXMaxEvent=1;
	}
	if (gFireCrackerEvent!=0)
	{
		flag.EventData.bFireCracker=1;
	}
	if (gHeartOfLoveEvent!=0)
	{
		flag.EventData.bHeartOfLove=1;
	}
	if (gMedalEvent!=0)
	{
		flag.EventData.bMedalEvent=1;
	}
	if (g_bDoRingEvent!=0)
	{
		flag.EventData.bRingEvent=1;
	}
	if (gEventChipEvent!=0)
	{
		flag.EventData.bEventChip=1;
	}
	if (gIsEledoradoEvent!=0)
	{
		flag.EventData.bEledorado=1;
	}
	if (gUseNPGGChecksum!=0)
	{
		flag.EventData.bNPGGChecksum=1;
	}	


#if (WL_PROTECT==1)  
	VM_START_WITHLEVEL(7)
	if(WLRegGetStatus(NULL) != 1)
	{
		ReadConfig.SkillBerserkerTimeDiv = 0;
		ReadConfig.SkillBerserkerDefDiv = 0;
		ReadConfig.SkillBerserkerDefDiv = 0;	
		
		
		for(int i=0;i<OBJ_MAXMONSTER;i++)
		{
			gObj[i].m_SkillSleep = 1;
			gObj[i].TargetNumber = -1;
			gObj[i].m_SkillSleep_MoveRange = 0;
			gObj[i].m_SkillSleep_AttackRange = 0;
			gObj[i].m_SkillSleep_ViewRange = 0;

			gObj[i].m_MoveRange = 0;
			gObj[i].m_AttackRange = 0;
			gObj[i].m_ViewRange = 0;
		}
	}
	VM_END
#endif
	return flag.AllData;	
}
*/

void ReadEventInfo(MU_EVENT_TYPE eEventType)
{
	char szTemp[256];
	switch(eEventType)
	{
		case 0: //All Events
		{
			//Devil Square Event
			//if ( gDevilSquareEvent == FALSE )
			g_DevilSquare.SetClose();
			g_DevilSquare.Load(ReadConfig.ConnDataFiles[13]);
			gDevilSquareEvent = GetPrivateProfileInt("GameServerInfo","DevilSquareEvent", 0, ReadConfig.ConnDataFiles[0]);
			gEyesOfDevilSquareDropRate = GetPrivateProfileInt("GameServerInfo","EyesOfDevilSquareDropRate", 2, ReadConfig.ConnDataFiles[0]);
			gKeyOfDevilSquareDropRate = GetPrivateProfileInt("GameServerInfo","KeyOfDevilSquareDropRate", 2, ReadConfig.ConnDataFiles[0]);
			DevilSquareEventConnect = GetPrivateProfileInt("GameServerInfo","DevilSquareEventConnect", 1, ReadConfig.ConnDataFiles[0]);
			GetPrivateProfileString("GameServerInfo", "DevilSquareEventServer", "127.0.0.1", gDevilSquareEventServerIp, 20, ReadConfig.ConnDataFiles[0]);

			//TamaJJang Event
			gTamaJJangEvent = GetPrivateProfileInt("GameServerInfo", "TamaJJangEvent", 0, ReadConfig.ConnDataFiles[0]);
			gTamaJJangKeepTime = GetPrivateProfileInt("GameServerInfo", "TamaJJangKeepTime", 0, ReadConfig.ConnDataFiles[0]);
			gTamaJJangDisappearTime = GetPrivateProfileInt("GameServerInfo", "TamaJJangDisappearTime", 1800, ReadConfig.ConnDataFiles[0]);
			gTamaJJangDisappearTimeRandomRange = GetPrivateProfileInt("GameServerInfo", "TamaJJangDisappearTimeRandomRange", 1800, ReadConfig.ConnDataFiles[0]);
			gTamaJJangTime = 10;

			//Blood Castle Event
			g_BloodCastle.LoadItemDropRate();

			//Event Chip Event
			gEventChipEvent = GetPrivateProfileInt("GameServerInfo","EventChipEvent",0, ReadConfig.ConnDataFiles[0]);
			gBoxOfGoldDropRate = GetPrivateProfileInt("GameServerInfo","BoxOfGoldDropRate",2, ReadConfig.ConnDataFiles[0]);
			gBoxOfGoldMinMobLevel = GetPrivateProfileInt("GameServerInfo","BoxOfGoldMinMobLevel",10, ReadConfig.ConnDataFiles[0]);
			gBoxOfGoldMaxMobLevel = GetPrivateProfileInt("GameServerInfo","BoxOfGoldMaxMobLevel",100, ReadConfig.ConnDataFiles[0]);

			g_ItemDropRateForBoxOfGold = GetPrivateProfileInt("GameServerInfo","ItemDropRateForBoxOfGold",2, ReadConfig.ConnDataFiles[0]);
			g_ItemEXDropRateForBoxOfGold = GetPrivateProfileInt("GameServerInfo","ItemExDropRateForBoxOfGold",2, ReadConfig.ConnDataFiles[0]);
			g_EventChipDropRateForBoxOfGold = GetPrivateProfileInt("GameServerInfo","EventChipDropRateForBoxOfGold",2, ReadConfig.ConnDataFiles[0]);
			EventChipServerConnect = GetPrivateProfileInt("GameServerInfo","EventChipServerConnect", 0, ReadConfig.ConnDataFiles[0]);
			GetPrivateProfileString("GameServerInfo", "EventChipServerIp", "127.0.0.1", gEventChipServerIp, 20, ReadConfig.ConnDataFiles[0]);

			// Janpan UNIQUE
			g_iJapan1StAnivItemDropRate = GetPrivateProfileInt("GameServerInfo","Japan1StAnivItemDropRate", 0, ReadConfig.ConnDataFiles[0]);

			// Mark  of the Lord
			g_iMarkOfTheLord = GetPrivateProfileInt("GameServerInfo","MarkOfTheLord",0, ReadConfig.ConnDataFiles[0]);

			// KUNDUN Settings
			giKundunRefillHPSec = GetPrivateProfileInt("GameServerInfo","KundunRefillHPSec",500, ReadConfig.ConnDataFiles[0]);
			giKundunRefillHP = GetPrivateProfileInt("GameServerInfo","KundunRefillHP",10000, ReadConfig.ConnDataFiles[0]);
			giKundunRefillHPTime = GetPrivateProfileInt("GameServerInfo","KundunRefillHPTime",600, ReadConfig.ConnDataFiles[0]);
			giKundunHPLogSaveTime = GetPrivateProfileInt("GameServerInfo","KundunHPLogSaveTime",120, ReadConfig.ConnDataFiles[0]);

			// Crywolf Dark Elf Item drop Settingss
			g_bCrywolfMonsterDarkElfItemDrop = GetPrivateProfileInt("GameServerInfo","CrywolfMonsterDarkElfItemDrop",0, ReadConfig.ConnDataFiles[0]);

			// Crywolf Boss Monster Item drop Settingss
			g_bCrywolfBossMonsterItemDrop = GetPrivateProfileInt("GameServerInfo","CrywolfBossMonsterItemDrop",0, ReadConfig.ConnDataFiles[0]);

			// Crywolf Benefit
			g_iCrywolfApplyMvpBenefit = GetPrivateProfileInt("GameServerInfo","CrywolfApplyMvpBenefit",0, ReadConfig.ConnDataFiles[0]);
				int iCrywolfPlusChaosRateBenefit = GetPrivateProfileInt("GameServerInfo","CrywolfPlusChaosRateBenefit",0, ReadConfig.ConnDataFiles[0]);
				int iCrywolfMonHPRateBenefit = GetPrivateProfileInt("GameServerInfo","CrywolfMonHPRateBenefit", 100, ReadConfig.ConnDataFiles[0]);
				int iCrywolfKundunHPRefillState = GetPrivateProfileInt("GameServerInfo","CrywolfKundunHPRefillBenefit", 1, ReadConfig.ConnDataFiles[0]);

				g_CrywolfSync.SetKundunHPRefillState(iCrywolfKundunHPRefillState);
				g_CrywolfSync.SetPlusChaosRate(iCrywolfPlusChaosRateBenefit);
				g_CrywolfSync.SetMonHPBenefitRate(iCrywolfMonHPRateBenefit);

			// Crywolf Penalty
			g_iCrywolfApplyMvpPenalty = GetPrivateProfileInt("GameServerInfo","CrywolfApplyMvpPenalty",0, ReadConfig.ConnDataFiles[0]);
				int iCrywolfGemDropPenaltyRate = GetPrivateProfileInt("GameServerInfo","CrwyolfGemDropPenaltyRate",100, ReadConfig.ConnDataFiles[0]);
				int iCrywolfGettingExpPenaltyRate = GetPrivateProfileInt("GameServerInfo","CrwyolfGettingExpPenaltyRate",100, ReadConfig.ConnDataFiles[0]);

				g_CrywolfSync.SetGemDropPenaltiyRate(iCrywolfGemDropPenaltyRate);
				g_CrywolfSync.SetGettingExpPenaltyRate(iCrywolfGettingExpPenaltyRate);

			// Kanturu Event
			#if (GS_CASTLE==0)
			int iKanturuEnableValue = GetPrivateProfileInt("GameServerInfo","KanturuEvent",0, ReadConfig.ConnDataFiles[0]);
			g_Kanturu.SetKanturuEnable(iKanturuEnableValue);
			#endif
				// Map Enter Settings
				g_iBlockKanturuMapEnter = GetPrivateProfileInt("GameServerInfo","BlockKanturuMapEnter",0, ReadConfig.ConnDataFiles[0]);
				g_iBlockCastleSiegeMapEnter = GetPrivateProfileInt("GameServerInfo","BlockCastleSiegeMapEnter",0, ReadConfig.ConnDataFiles[0]);
				// Drops Maya Hand
				g_bKanturuMayaHandItemDrop = GetPrivateProfileInt("GameServerInfo","KanturuMayaHandItemDrop",0, ReadConfig.ConnDataFiles[0]);
				// Drops Nightmare
				g_bKanturuNightmareItemDrop = GetPrivateProfileInt("GameServerInfo","KanturuNightmareItemDrop",0, ReadConfig.ConnDataFiles[0]);
				// Special Drop
				g_bKanturuSpecialItemDropOn = GetPrivateProfileInt("GameServerInfo","KanturuSpecialItemDropOn",0, ReadConfig.ConnDataFiles[0]);
				// Other
				g_iKanturuMoonStoneDropRate = GetPrivateProfileInt("GameServerInfo","KanturuMoonStoneDropRate",0, ReadConfig.ConnDataFiles[0]);
				g_iKanturuJewelOfHarmonyDropRate = GetPrivateProfileInt("GameServerInfo","KanturuJewelOfHarmonyDropRate",0, ReadConfig.ConnDataFiles[0]);

			g_bHellMainItemDrop = GetPrivateProfileInt("GameServerInfo","HellMainItemDrop",0, ReadConfig.ConnDataFiles[0]);
			gIsDropDarkLordItem = GetPrivateProfileInt("GameServerInfo","IsDropDarkLordItem", 0, ReadConfig.ConnDataFiles[0]);
			gSleeveOfLordDropRate = GetPrivateProfileInt("GameServerInfo","SleeveOfLordDropRate", 10, ReadConfig.ConnDataFiles[0]);
			gSleeveOfLordDropLevel = GetPrivateProfileInt("GameServerInfo","SleeveOfLordDropLevel", 86, ReadConfig.ConnDataFiles[0]);
			gSoulOfDarkHorseDropRate = GetPrivateProfileInt("GameServerInfo","SoulOfDarkHorseDropRate", 10, ReadConfig.ConnDataFiles[0]);
			gSoulOfDarkHorseropLevel = GetPrivateProfileInt("GameServerInfo","SoulOfDarkHorseropLevel", 125, ReadConfig.ConnDataFiles[0]);
			gSoulOfDarkSpiritDropRate = GetPrivateProfileInt("GameServerInfo","SoulOfDarkSpiritDropRate", 10, ReadConfig.ConnDataFiles[0]);
			gSoulOfDarkSpiritDropLevel = GetPrivateProfileInt("GameServerInfo","SoulOfDarkSpiritDropLevel", 96, ReadConfig.ConnDataFiles[0]);
			GetPrivateProfileString("GameServerInfo", "DarkSpiritAddExperience", "1", szTemp, 5, ReadConfig.ConnDataFiles[0]);
			gDarkSpiritAddExperience = (float)atof(szTemp);

			gIsDropGemOfDefend = GetPrivateProfileInt("GameServerInfo","IsDropGemOfDefend", 0, ReadConfig.ConnDataFiles[0]);
			gGemOfDefendDropRate = GetPrivateProfileInt("GameServerInfo","GemOfDefendDropRate", 5, ReadConfig.ConnDataFiles[0]);
			gGemOfDefendDropLevel = GetPrivateProfileInt("GameServerInfo","GemOfDefendDropLevel", 75, ReadConfig.ConnDataFiles[0]);

#if (GS_CASTLE==1)
			gIsDropSetItemInCastleHuntZone = GetPrivateProfileInt("GameServerInfo","IsDropSetItemInCastleHuntZone", 0, ReadConfig.ConnDataFiles[0]);
			gSetItemInCastleHuntZoneDropRate = GetPrivateProfileInt("GameServerInfo","SetItemInCastleHuntZoneDropRate", 1, ReadConfig.ConnDataFiles[0]);
			gSetItemInCastleHuntZoneDropLevel = GetPrivateProfileInt("GameServerInfo","SetItemInCastleHuntZoneDropLevel", 75, ReadConfig.ConnDataFiles[0]);
#endif
			gChaosEvent = GetPrivateProfileInt("GameServerInfo","ChaosEvent",0, ReadConfig.ConnDataFiles[0]);
			GetPrivateProfileString("GameServerInfo", "ChaosEventServer", "127.0.0.1", gChaosEventServerIp, 20, ReadConfig.ConnDataFiles[0]);
			gUdpSocCE.SendSet(gChaosEventServerIp, 60005);
			LogAdd("[Option] ChaosEvent = %d", gChaosEvent);
			LogAdd("[Option] ChaosEventServer = %s", gChaosEventServerIp);

			//Eldorado Event
			gIsEledoradoEvent = GetPrivateProfileInt("GameServerInfo","IsEledoradoEvent", 0, ReadConfig.ConnDataFiles[0]);
			gEledoradoGoldGoblenRegenTime = GetPrivateProfileInt("GameServerInfo","EledoradoGoldGoblenRegenTime", 180, ReadConfig.ConnDataFiles[0]);
			gEledoradoTitanRegenTime = GetPrivateProfileInt("GameServerInfo","EledoradoTitanRegenTime", 180, ReadConfig.ConnDataFiles[0]);
			gEledoradoGoldDerconRegenTime = GetPrivateProfileInt("GameServerInfo","EledoradoGoldDerconRegenTime", 720, ReadConfig.ConnDataFiles[0]);
			gEledoradoDevilLizardKingRegenTime = GetPrivateProfileInt("GameServerInfo","EledoradoDevilLizardKingRegenTime", 360, ReadConfig.ConnDataFiles[0]);
			gEledoradoDevilTantarosRegenTime = GetPrivateProfileInt("GameServerInfo","EledoradoDevilTantarosRegenTime", 360, ReadConfig.ConnDataFiles[0]);
			gEledoradoGoldGoblenItemDropRate = GetPrivateProfileInt("GameServerInfo","EledoradoGoldGoblenItemDropRate", 10, ReadConfig.ConnDataFiles[0]);
			gEledoradoTitanItemDropRate = GetPrivateProfileInt("GameServerInfo","EledoradoTitanItemDropRate", 10, ReadConfig.ConnDataFiles[0]);
			gEledoradoGoldDerconItemDropRate = GetPrivateProfileInt("GameServerInfo","EledoradoGoldDerconItemDropRate", 10, ReadConfig.ConnDataFiles[0]);
			gEledoradoDevilLizardKingItemDropRate = GetPrivateProfileInt("GameServerInfo","EledoradoDevilLizardKingItemDropRate", 10, ReadConfig.ConnDataFiles[0]);
			gEledoradoDevilTantarosItemDropRate = GetPrivateProfileInt("GameServerInfo","EledoradoDevilTantarosItemDropRate", 10, ReadConfig.ConnDataFiles[0]);
			gEledoradoGoldGoblenExItemDropRate = GetPrivateProfileInt("GameServerInfo","EledoradoGoldGoblenExItemDropRate", 10, ReadConfig.ConnDataFiles[0]);
			gEledoradoTitanExItemDropRate = GetPrivateProfileInt("GameServerInfo","EledoradoTitanExItemDropRate", 10, ReadConfig.ConnDataFiles[0]);
			gEledoradoGoldDerconExItemDropRate = GetPrivateProfileInt("GameServerInfo","EledoradoGoldDerconExItemDropRate", 10, ReadConfig.ConnDataFiles[0]);
			gEledoradoDevilLizardKingExItemDropRate = GetPrivateProfileInt("GameServerInfo","EledoradoDevilLizardKingExItemDropRate", 10, ReadConfig.ConnDataFiles[0]);
			gEledoradoDevilTantarosExItemDropRate = GetPrivateProfileInt("GameServerInfo","EledoradoDevilTantarosExItemDropRate", 10, ReadConfig.ConnDataFiles[0]);

			//Eldorado Event 2
			gIsEledorado2Event = GetPrivateProfileInt("GameServerInfo","IsEledorado2Event", 0, ReadConfig.ConnDataFiles[0]);
			gEledorado2EventRegenTime[0] = GetPrivateProfileInt("GameServerInfo","EledoradoGoldDarkKnightRegenTime", 180, ReadConfig.ConnDataFiles[0]);
			gEledorado2EventRegenTime[1] = GetPrivateProfileInt("GameServerInfo","EledoradoGoldDevilRegenTime", 180, ReadConfig.ConnDataFiles[0]);
			gEledorado2EventRegenTime[2] = GetPrivateProfileInt("GameServerInfo","EledoradoGoldStoneGolemRegenTime", 720, ReadConfig.ConnDataFiles[0]);
			gEledorado2EventRegenTime[3] = GetPrivateProfileInt("GameServerInfo","EledoradoGoldCrustItemRegenTime", 360, ReadConfig.ConnDataFiles[0]);
			gEledorado2EventRegenTime[4] = GetPrivateProfileInt("GameServerInfo","EledoradoGoldSatyrosRegenTime", 360, ReadConfig.ConnDataFiles[0]);
			gEledorado2EventRegenTime[5] = GetPrivateProfileInt("GameServerInfo","EledoradoGoldTwinTaleRegenTime", 180, ReadConfig.ConnDataFiles[0]);
			gEledorado2EventRegenTime[6] = GetPrivateProfileInt("GameServerInfo","EledoradoGoldIronKnightRegenTime", 180, ReadConfig.ConnDataFiles[0]);
			gEledorado2EventRegenTime[7] = GetPrivateProfileInt("GameServerInfo","EledoradoGoldNapinRegenTime", 720, ReadConfig.ConnDataFiles[0]);
			gEledorado2EventRegenTime[8] = GetPrivateProfileInt("GameServerInfo","EledoradoGoldGreatDragonRegenTime", 360, ReadConfig.ConnDataFiles[0]);
			gEledorado2EventRegenTime[9] = GetPrivateProfileInt("GameServerInfo","EledoradoGoldRabbitRegenTime", 360, ReadConfig.ConnDataFiles[0]);
			gEledorado2EventItemDropRate[0] = GetPrivateProfileInt("GameServerInfo","EledoradoGoldDarkKnightItemDropRate", 10, ReadConfig.ConnDataFiles[0]);
			gEledorado2EventItemDropRate[1] = GetPrivateProfileInt("GameServerInfo","EledoradoGoldDevilItemDropRate", 10, ReadConfig.ConnDataFiles[0]);
			gEledorado2EventItemDropRate[2] = GetPrivateProfileInt("GameServerInfo","EledoradoGoldStoneGolemItemDropRate", 10, ReadConfig.ConnDataFiles[0]);
			gEledorado2EventItemDropRate[3] = GetPrivateProfileInt("GameServerInfo","EledoradoGoldCrustItemDropRate", 10, ReadConfig.ConnDataFiles[0]);
			gEledorado2EventItemDropRate[4] = GetPrivateProfileInt("GameServerInfo","EledoradoGoldSatyrosItemDropRate", 10, ReadConfig.ConnDataFiles[0]);
			gEledorado2EventItemDropRate[5] = GetPrivateProfileInt("GameServerInfo","EledoradoGoldTwinTaleItemDropRate", 10, ReadConfig.ConnDataFiles[0]);
			gEledorado2EventItemDropRate[6] = GetPrivateProfileInt("GameServerInfo","EledoradoGoldIronKnightItemDropRate", 10, ReadConfig.ConnDataFiles[0]);
			gEledorado2EventItemDropRate[7] = GetPrivateProfileInt("GameServerInfo","EledoradoGoldNapinItemDropRate", 10, ReadConfig.ConnDataFiles[0]);
			gEledorado2EventItemDropRate[8] = GetPrivateProfileInt("GameServerInfo","EledoradoGoldGreatDragonItemDropRate", 10, ReadConfig.ConnDataFiles[0]);
			gEledorado2EventItemDropRate[9] = GetPrivateProfileInt("GameServerInfo","EledoradoGoldRabbitItemDropRate", 10, ReadConfig.ConnDataFiles[0]);
			gEledorado2EventExItemDropRate[0] = GetPrivateProfileInt("GameServerInfo","EledoradoGoldDarkKnightExItemDropRate", 10, ReadConfig.ConnDataFiles[0]);
			gEledorado2EventExItemDropRate[1] = GetPrivateProfileInt("GameServerInfo","EledoradoGoldDevilExItemDropRate", 10, ReadConfig.ConnDataFiles[0]);
			gEledorado2EventExItemDropRate[2] = GetPrivateProfileInt("GameServerInfo","EledoradoGoldStoneGolemExItemDropRate", 10, ReadConfig.ConnDataFiles[0]);
			gEledorado2EventExItemDropRate[3] = GetPrivateProfileInt("GameServerInfo","EledoradoGoldCrustExItemDropRate", 10, ReadConfig.ConnDataFiles[0]);
			gEledorado2EventExItemDropRate[4] = GetPrivateProfileInt("GameServerInfo","EledoradoGoldSatyrosExItemDropRate", 10, ReadConfig.ConnDataFiles[0]);
			gEledorado2EventExItemDropRate[5] = GetPrivateProfileInt("GameServerInfo","EledoradoGoldTwinTaleExItemDropRate", 10, ReadConfig.ConnDataFiles[0]);
			gEledorado2EventExItemDropRate[6] = GetPrivateProfileInt("GameServerInfo","EledoradoGoldIronKnightExItemDropRate", 10, ReadConfig.ConnDataFiles[0]);
			gEledorado2EventExItemDropRate[7] = GetPrivateProfileInt("GameServerInfo","EledoradoGoldNapinExItemDropRate", 10, ReadConfig.ConnDataFiles[0]);
			gEledorado2EventExItemDropRate[8] = GetPrivateProfileInt("GameServerInfo","EledoradoGoldGreatDragonExItemDropRate", 10, ReadConfig.ConnDataFiles[0]);
			gEledorado2EventExItemDropRate[9] = GetPrivateProfileInt("GameServerInfo","EledoradoGoldRabbitExItemDropRate", 10, ReadConfig.ConnDataFiles[0]);

			//Ring Attack Event
			GetPrivateProfileString("GameServerInfo", "RingAttackEvent", "0", szTemp, 5, ReadConfig.ConnDataFiles[0]);
			g_bDoRingEvent = atoi(szTemp);
			GetPrivateProfileString("GameServerInfo", "RingOrcKillGiftRate", "10000", szTemp, 10, ReadConfig.ConnDataFiles[0]);
			g_iRingOrcKillGiftRate = atoi(szTemp);
			GetPrivateProfileString("GameServerInfo", "RingDropGiftRate", "10000", szTemp, 10, ReadConfig.ConnDataFiles[0]);
			g_iRingDropGiftRate = atoi(szTemp);
			g_iRingDropItemWhiteWizType	= GetPrivateProfileInt("GameServerInfo", "SCFRingEventWizardRewardItemType",14, ReadConfig.ConnDataFiles[0]) ;
			g_iRingDropItemWhiteWizIndex = GetPrivateProfileInt("GameServerInfo", "SCFRingEventWizardRewardItemIndex",13, ReadConfig.ConnDataFiles[0]) ;
			g_iRingEventOrcRewardDropRate =  GetPrivateProfileInt("GameServerInfo", "SCFRingEventOrcRewardDropRate",10, ReadConfig.ConnDataFiles[0]) ;
			g_iRingDropItemOrcType = GetPrivateProfileInt("GameServerInfo", "SCFRingEventOrcRewardItemType",13, ReadConfig.ConnDataFiles[0]) ;
			g_iRingDropItemOrcIndex = GetPrivateProfileInt("GameServerInfo", "SCFRingEventOrcRewardItemIndex",20, ReadConfig.ConnDataFiles[0]) ;

			g_RingAttackEvent.Load(ReadConfig.ConnDataFiles[17]);
			g_RingAttackEvent.EnableEvent(g_bDoRingEvent);

#if (GS_CASTLE==1)
			GetPrivateProfileString("GameServerInfo", "CastleDeepEvent", "0", szTemp, 5, ReadConfig.ConnDataFiles[0]);
			g_bDoCastleDeepEvent = atoi(szTemp);
			g_CastleDeepEvent.Load(ReadConfig.ConnDataFiles[19]);
			g_CastleDeepEvent.EnableEvent(g_bDoCastleDeepEvent);
#endif

			GetPrivateProfileString("GameServerInfo", "EVENT1", "0", szTemp, 5, ReadConfig.ConnDataFiles[0]);
			gEvent1 = atoi(szTemp);
			GetPrivateProfileString("GameServerInfo", "Event1ItemDropTodayMax", "1", szTemp, 5, ReadConfig.ConnDataFiles[0]);
			gEvent1ItemDropTodayCount = 0;
			gEvent1ItemDropTodayMax = atoi(szTemp);
			GetPrivateProfileString("GameServerInfo", "Event1ItemDropTodayPercent", "10000000", szTemp, 5, ReadConfig.ConnDataFiles[0]);
			gEvent1ItemDropTodayPercent = atoi(szTemp);

			gFireCrackerEvent = GetPrivateProfileInt("GameServerInfo","FireCrackerEvent", 0, ReadConfig.ConnDataFiles[0]);
			gFireCrackerDropRate = GetPrivateProfileInt("GameServerInfo","FireCrackerDropRate", 5000, ReadConfig.ConnDataFiles[0]);
			g_ItemDropRateForgFireCracker = GetPrivateProfileInt("GameServerInfo","ItemDropRateForFireCracker", 2, ReadConfig.ConnDataFiles[0]);
			
			gMedalEvent = GetPrivateProfileInt("GameServerInfo","MedalEvent", 0, ReadConfig.ConnDataFiles[0]);
			gGoldMedalDropRate = GetPrivateProfileInt("GameServerInfo","GoldMedalDropRate", 2, ReadConfig.ConnDataFiles[0]);
			gSilverMedalDropRate = GetPrivateProfileInt("GameServerInfo","SilverMedalDropRate", 2, ReadConfig.ConnDataFiles[0]);
			g_ItemDropRateForGoldMedal = GetPrivateProfileInt("GameServerInfo","ItemDropRateForGoldMedal", 2, ReadConfig.ConnDataFiles[0]);
			g_ItemDropRateForSilverMedal = GetPrivateProfileInt("GameServerInfo","ItemDropRateForSilverMedal", 2, ReadConfig.ConnDataFiles[0]);
			
			gHeartOfLoveEvent = GetPrivateProfileInt("GameServerInfo","HeartOfLoveEvent", 0, ReadConfig.ConnDataFiles[0]);
			gHeartOfLoveDropRate = GetPrivateProfileInt("GameServerInfo","HeartOfLoveDropRate", 5000, ReadConfig.ConnDataFiles[0]);
			g_ItemDropRateForgHeartOfLove = GetPrivateProfileInt("GameServerInfo","ItemDropRateForHeartOfLove", 2, ReadConfig.ConnDataFiles[0]);

			// NPC Event Talk
			gEnableEventNPCTalk = GetPrivateProfileInt("GameServerInfo","EnableEventNPCTalk", 0, ReadConfig.ConnDataFiles[0]);

			g_ChaosCastle.Load(ReadConfig.ConnDataFiles[15]);
			g_bChaosCastle = GetPrivateProfileInt("GameServerInfo","ChaosCastleEvent", 0, ReadConfig.ConnDataFiles[0]);
			g_iCheckCanStartPlayCCMinPlayers = GetPrivateProfileInt("GameServerInfo", "SCFChaosCastleMinStart",2, ReadConfig.ConnDataFiles[0]) ;
			g_iGiveWinnerItemCCType1 = GetPrivateProfileInt("GameServerInfo", "SCFChaosCastleReward1Type",14, ReadConfig.ConnDataFiles[0]) ;
			g_iGiveWinnerItemCCIndex1 = GetPrivateProfileInt("GameServerInfo", "SCFChaosCastleReward1Index",14, ReadConfig.ConnDataFiles[0]) ;
			g_iGiveWinnerItemCCType2 = GetPrivateProfileInt("GameServerInfo", "SCFChaosCastleReward2Type",14, ReadConfig.ConnDataFiles[0]) ;
			g_iGiveWinnerItemCCIndex2 = GetPrivateProfileInt("GameServerInfo", "SCFChaosCastleReward2Index",22, ReadConfig.ConnDataFiles[0]) ;
			g_iGiveWinnerItemCCType3 = GetPrivateProfileInt("GameServerInfo", "SCFChaosCastleReward3Type",14, ReadConfig.ConnDataFiles[0]) ;
			g_iGiveWinnerItemCCIndex3 = GetPrivateProfileInt("GameServerInfo", "SCFChaosCastleReward3Index",13, ReadConfig.ConnDataFiles[0]) ;
			g_iGiveWinnerItemCCType4 = GetPrivateProfileInt("GameServerInfo", "SCFChaosCastleReward4Type",14, ReadConfig.ConnDataFiles[0]) ;
			g_iGiveWinnerItemCCIndex4 = GetPrivateProfileInt("GameServerInfo", "SCFChaosCastleReward4Index",16, ReadConfig.ConnDataFiles[0]) ;

			g_bCC1SetDropRate = GetPrivateProfileInt("GameServerInfo", "SCFChaosCastle1AncientDropPercent",5, ReadConfig.ConnDataFiles[0]) ;
			g_bCC2SetDropRate = GetPrivateProfileInt("GameServerInfo", "SCFChaosCastle2AncientDropPercent",5, ReadConfig.ConnDataFiles[0]) ;
			g_bCC3SetDropRate = GetPrivateProfileInt("GameServerInfo", "SCFChaosCastle3AncientDropPercent",5, ReadConfig.ConnDataFiles[0]) ;
			g_bCC4SetDropRate = GetPrivateProfileInt("GameServerInfo", "SCFChaosCastle4AncientDropPercent",5, ReadConfig.ConnDataFiles[0]) ;
			g_bCC5SetDropRate = GetPrivateProfileInt("GameServerInfo", "SCFChaosCastle5AncientDropPercent",5, ReadConfig.ConnDataFiles[0]) ;
			g_bCC6SetDropRate = GetPrivateProfileInt("GameServerInfo", "SCFChaosCastle6AncientDropPercent",5, ReadConfig.ConnDataFiles[0]) ;
			g_bCC7SetDropRate = GetPrivateProfileInt("GameServerInfo", "SCFChaosCastle7AncientDropPercent",5, ReadConfig.ConnDataFiles[0]) ;

			g_bCC1TypeDropRate = GetPrivateProfileInt("GameServerInfo", "SCFChaosCastle1Type1Type2DropPercent",50, ReadConfig.ConnDataFiles[0]) ;
			g_bCC2TypeDropRate = GetPrivateProfileInt("GameServerInfo", "SCFChaosCastle2Type1Type2DropPercent",50, ReadConfig.ConnDataFiles[0]) ;
			g_bCC3TypeDropRate = GetPrivateProfileInt("GameServerInfo", "SCFChaosCastle3Type1Type2DropPercent",50, ReadConfig.ConnDataFiles[0]) ;
			g_bCC4TypeDropRate = GetPrivateProfileInt("GameServerInfo", "SCFChaosCastle4Type1Type2DropPercent",50, ReadConfig.ConnDataFiles[0]) ;
			g_bCC5TypeDropRate = GetPrivateProfileInt("GameServerInfo", "SCFChaosCastle5Type1Type2DropPercent",50, ReadConfig.ConnDataFiles[0]) ;
			g_bCC6TypeDropRate = GetPrivateProfileInt("GameServerInfo", "SCFChaosCastle6Type1Type2DropPercent",50, ReadConfig.ConnDataFiles[0]) ;
			g_bCC7TypeDropRate = GetPrivateProfileInt("GameServerInfo", "SCFChaosCastle7Type1Type2DropPercent",50, ReadConfig.ConnDataFiles[0]) ;

			g_iKundunMarkDropRate = GetPrivateProfileInt("GameServerInfo","KundunMarkDropRate", 0, ReadConfig.ConnDataFiles[0]);

			g_iMysteriousBeadDropRate1 = GetPrivateProfileInt("GameServerInfo","MysteriouseBeadDropRate1", 0, ReadConfig.ConnDataFiles[0]);
			g_iMysteriousBeadDropRate2 = GetPrivateProfileInt("GameServerInfo","MysteriouseBeadDropRate2", 0, ReadConfig.ConnDataFiles[0]);

			g_iHiddenTreasureBoxOfflineRate = GetPrivateProfileInt("GameServerInfo","HiddenTreasureBoxOfflineRate", 0, ReadConfig.ConnDataFiles[0]);
     
			GetPrivateProfileString("GameServerInfo", "EventManagerOn", "0", szTemp, 5, ReadConfig.ConnDataFiles[0]);
			g_bEventManagerOn = atoi(szTemp);
			GetPrivateProfileString("GameServerInfo", "SCFEventManagerOn", "0", szTemp, 5, ReadConfig.ConnDataFiles[0]);
			g_bSCFEventManagerOn = 0;
			g_bSCFEventManagerOn = atoi(szTemp);

			g_EventManager.Load(ReadConfig.ConnDataFiles[18]);
			g_EventManager.RegEvent(0, DragonEvent);
			g_EventManager.RegEvent(1, AttackEvent);
			g_EventManager.RegEvent(2, &gEledoradoEvent);
			g_EventManager.RegEvent(3, &g_RingAttackEvent);
			g_EventManager.Init(g_bEventManagerOn);
			g_EventManager.SCFInit(g_bSCFEventManagerOn);

			g_iDarkLordHeartDropRate = GetPrivateProfileInt("GameServerInfo","DarkLordHeartDropRate", 0, ReadConfig.ConnDataFiles[0]);
			g_iDarkLordHeartOffEventRate = GetPrivateProfileInt("GameServerInfo","DarkLoadHeartOffEventRate", 0, ReadConfig.ConnDataFiles[0]);

			// Box Events
			g_bChocolateBoxEvent = GetPrivateProfileInt("GameServerInfo","ChocolateEvent",0, ReadConfig.ConnDataFiles[0]);
			g_bCandyBoxEvent = GetPrivateProfileInt("GameServerInfo","CandyBoxEvent",0, ReadConfig.ConnDataFiles[0]);
			g_bMysteryBoxEvent = GetPrivateProfileInt("GameServerInfo","MysteryBoxEvent",0, ReadConfig.ConnDataFiles[0]);
		}
		break;

		case 1: //Devil Square
		{
			g_DevilSquare.Load(ReadConfig.ConnDataFiles[13]);
			gDevilSquareEvent = GetPrivateProfileInt("GameServerInfo","DevilSquareEvent", 0, ReadConfig.ConnDataFiles[0]);
			gEyesOfDevilSquareDropRate = GetPrivateProfileInt("GameServerInfo","EyesOfDevilSquareDropRate", 2, ReadConfig.ConnDataFiles[0]);
			gKeyOfDevilSquareDropRate = GetPrivateProfileInt("GameServerInfo","KeyOfDevilSquareDropRate", 2, ReadConfig.ConnDataFiles[0]);
			DevilSquareEventConnect = GetPrivateProfileInt("GameServerInfo","DevilSquareEventConnect", 1, ReadConfig.ConnDataFiles[0]);
			GetPrivateProfileString("GameServerInfo", "DevilSquareEventServer", "127.0.0.1", gDevilSquareEventServerIp, 20, ReadConfig.ConnDataFiles[0]);
		}
		break;

		case 2: //Blood Castle
		{
			g_BloodCastle.LoadItemDropRate();
		}
		break;

		case 3: //Attack Event
			break;

		case 4: //Eledorado Event
		{
			gIsEledoradoEvent = GetPrivateProfileInt("GameServerInfo","IsEledoradoEvent", 0, ReadConfig.ConnDataFiles[0]);
			gEledoradoGoldGoblenRegenTime = GetPrivateProfileInt("GameServerInfo","EledoradoGoldGoblenRegenTime", 180, ReadConfig.ConnDataFiles[0]);
			gEledoradoTitanRegenTime = GetPrivateProfileInt("GameServerInfo","EledoradoTitanRegenTime", 180, ReadConfig.ConnDataFiles[0]);
			gEledoradoGoldDerconRegenTime = GetPrivateProfileInt("GameServerInfo","EledoradoGoldDerconRegenTime", 720, ReadConfig.ConnDataFiles[0]);
			gEledoradoDevilLizardKingRegenTime = GetPrivateProfileInt("GameServerInfo","EledoradoDevilLizardKingRegenTime", 360, ReadConfig.ConnDataFiles[0]);
			gEledoradoDevilTantarosRegenTime = GetPrivateProfileInt("GameServerInfo","EledoradoDevilTantarosRegenTime", 360, ReadConfig.ConnDataFiles[0]);
			gEledoradoGoldGoblenItemDropRate = GetPrivateProfileInt("GameServerInfo","EledoradoGoldGoblenItemDropRate", 10, ReadConfig.ConnDataFiles[0]);
			gEledoradoTitanItemDropRate = GetPrivateProfileInt("GameServerInfo","EledoradoTitanItemDropRate", 10, ReadConfig.ConnDataFiles[0]);
			gEledoradoGoldDerconItemDropRate = GetPrivateProfileInt("GameServerInfo","EledoradoGoldDerconItemDropRate", 10, ReadConfig.ConnDataFiles[0]);
			gEledoradoDevilLizardKingItemDropRate = GetPrivateProfileInt("GameServerInfo","EledoradoDevilLizardKingItemDropRate", 10, ReadConfig.ConnDataFiles[0]);
			gEledoradoDevilTantarosItemDropRate = GetPrivateProfileInt("GameServerInfo","EledoradoDevilTantarosItemDropRate", 10, ReadConfig.ConnDataFiles[0]);
			gEledoradoGoldGoblenExItemDropRate = GetPrivateProfileInt("GameServerInfo","EledoradoGoldGoblenExItemDropRate", 10, ReadConfig.ConnDataFiles[0]);
			gEledoradoTitanExItemDropRate = GetPrivateProfileInt("GameServerInfo","EledoradoTitanExItemDropRate", 10, ReadConfig.ConnDataFiles[0]);
			gEledoradoGoldDerconExItemDropRate = GetPrivateProfileInt("GameServerInfo","EledoradoGoldDerconExItemDropRate", 10, ReadConfig.ConnDataFiles[0]);
			gEledoradoDevilLizardKingExItemDropRate = GetPrivateProfileInt("GameServerInfo","EledoradoDevilLizardKingExItemDropRate", 10, ReadConfig.ConnDataFiles[0]);
			gEledoradoDevilTantarosExItemDropRate = GetPrivateProfileInt("GameServerInfo","EledoradoDevilTantarosExItemDropRate", 10, ReadConfig.ConnDataFiles[0]);

			gIsEledorado2Event = GetPrivateProfileInt("GameServerInfo","IsEledorado2Event", 0, ReadConfig.ConnDataFiles[0]);
			gEledorado2EventRegenTime[0] = GetPrivateProfileInt("GameServerInfo","EledoradoGoldDarkKnightRegenTime", 180, ReadConfig.ConnDataFiles[0]);
			gEledorado2EventRegenTime[1] = GetPrivateProfileInt("GameServerInfo","EledoradoGoldDevilRegenTime", 180, ReadConfig.ConnDataFiles[0]);
			gEledorado2EventRegenTime[2] = GetPrivateProfileInt("GameServerInfo","EledoradoGoldStoneGolemRegenTime", 720, ReadConfig.ConnDataFiles[0]);
			gEledorado2EventRegenTime[3] = GetPrivateProfileInt("GameServerInfo","EledoradoGoldCrustItemRegenTime", 360, ReadConfig.ConnDataFiles[0]);
			gEledorado2EventRegenTime[4] = GetPrivateProfileInt("GameServerInfo","EledoradoGoldSatyrosRegenTime", 360, ReadConfig.ConnDataFiles[0]);
			gEledorado2EventRegenTime[5] = GetPrivateProfileInt("GameServerInfo","EledoradoGoldTwinTaleRegenTime", 180, ReadConfig.ConnDataFiles[0]);
			gEledorado2EventRegenTime[6] = GetPrivateProfileInt("GameServerInfo","EledoradoGoldIronKnightRegenTime", 180, ReadConfig.ConnDataFiles[0]);
			gEledorado2EventRegenTime[7] = GetPrivateProfileInt("GameServerInfo","EledoradoGoldNapinRegenTime", 720, ReadConfig.ConnDataFiles[0]);
			gEledorado2EventRegenTime[8] = GetPrivateProfileInt("GameServerInfo","EledoradoGoldGreatDragonRegenTime", 360, ReadConfig.ConnDataFiles[0]);
			gEledorado2EventRegenTime[9] = GetPrivateProfileInt("GameServerInfo","EledoradoGoldRabbitRegenTime", 360, ReadConfig.ConnDataFiles[0]);
			gEledorado2EventItemDropRate[0] = GetPrivateProfileInt("GameServerInfo","EledoradoGoldDarkKnightItemDropRate", 10, ReadConfig.ConnDataFiles[0]);
			gEledorado2EventItemDropRate[1] = GetPrivateProfileInt("GameServerInfo","EledoradoGoldDevilItemDropRate", 10, ReadConfig.ConnDataFiles[0]);
			gEledorado2EventItemDropRate[2] = GetPrivateProfileInt("GameServerInfo","EledoradoGoldStoneGolemItemDropRate", 10, ReadConfig.ConnDataFiles[0]);
			gEledorado2EventItemDropRate[3] = GetPrivateProfileInt("GameServerInfo","EledoradoGoldCrustItemDropRate", 10, ReadConfig.ConnDataFiles[0]);
			gEledorado2EventItemDropRate[4] = GetPrivateProfileInt("GameServerInfo","EledoradoGoldSatyrosItemDropRate", 10, ReadConfig.ConnDataFiles[0]);
			gEledorado2EventItemDropRate[5] = GetPrivateProfileInt("GameServerInfo","EledoradoGoldTwinTaleItemDropRate", 10, ReadConfig.ConnDataFiles[0]);
			gEledorado2EventItemDropRate[6] = GetPrivateProfileInt("GameServerInfo","EledoradoGoldIronKnightItemDropRate", 10, ReadConfig.ConnDataFiles[0]);
			gEledorado2EventItemDropRate[7] = GetPrivateProfileInt("GameServerInfo","EledoradoGoldNapinItemDropRate", 10, ReadConfig.ConnDataFiles[0]);
			gEledorado2EventItemDropRate[8] = GetPrivateProfileInt("GameServerInfo","EledoradoGoldGreatDragonItemDropRate", 10, ReadConfig.ConnDataFiles[0]);
			gEledorado2EventItemDropRate[9] = GetPrivateProfileInt("GameServerInfo","EledoradoGoldRabbitItemDropRate", 10, ReadConfig.ConnDataFiles[0]);
			gEledorado2EventExItemDropRate[0] = GetPrivateProfileInt("GameServerInfo","EledoradoGoldDarkKnightExItemDropRate", 10, ReadConfig.ConnDataFiles[0]);
			gEledorado2EventExItemDropRate[1] = GetPrivateProfileInt("GameServerInfo","EledoradoGoldDevilExItemDropRate", 10, ReadConfig.ConnDataFiles[0]);
			gEledorado2EventExItemDropRate[2] = GetPrivateProfileInt("GameServerInfo","EledoradoGoldStoneGolemExItemDropRate", 10, ReadConfig.ConnDataFiles[0]);
			gEledorado2EventExItemDropRate[3] = GetPrivateProfileInt("GameServerInfo","EledoradoGoldCrustExItemDropRate", 10, ReadConfig.ConnDataFiles[0]);
			gEledorado2EventExItemDropRate[4] = GetPrivateProfileInt("GameServerInfo","EledoradoGoldSatyrosExItemDropRate", 10, ReadConfig.ConnDataFiles[0]);
			gEledorado2EventExItemDropRate[5] = GetPrivateProfileInt("GameServerInfo","EledoradoGoldTwinTaleExItemDropRate", 10, ReadConfig.ConnDataFiles[0]);
			gEledorado2EventExItemDropRate[6] = GetPrivateProfileInt("GameServerInfo","EledoradoGoldIronKnightExItemDropRate", 10, ReadConfig.ConnDataFiles[0]);
			gEledorado2EventExItemDropRate[7] = GetPrivateProfileInt("GameServerInfo","EledoradoGoldNapinExItemDropRate", 10, ReadConfig.ConnDataFiles[0]);
			gEledorado2EventExItemDropRate[8] = GetPrivateProfileInt("GameServerInfo","EledoradoGoldGreatDragonExItemDropRate", 10, ReadConfig.ConnDataFiles[0]);
			gEledorado2EventExItemDropRate[9] = GetPrivateProfileInt("GameServerInfo","EledoradoGoldRabbitExItemDropRate", 10, ReadConfig.ConnDataFiles[0]);

		}
		break;

		case 5: //Ring Event
		{
			GetPrivateProfileString("GameServerInfo", "RingAttackEvent", "0", szTemp, 5, ReadConfig.ConnDataFiles[0]);
			g_bDoRingEvent = atoi(szTemp);
			GetPrivateProfileString("GameServerInfo", "RingOrcKillGiftRate", "10000", szTemp, 10, ReadConfig.ConnDataFiles[0]);
			g_iRingOrcKillGiftRate = atoi(szTemp);
			GetPrivateProfileString("GameServerInfo", "RingDropGiftRate", "10000", szTemp, 10, ReadConfig.ConnDataFiles[0]);
			g_iRingDropGiftRate = atoi(szTemp);
			g_iRingDropItemWhiteWizType	= GetPrivateProfileInt("GameServerInfo", "SCFRingEventWizardRewardItemType",14, ReadConfig.ConnDataFiles[0]) ;
			g_iRingDropItemWhiteWizIndex = GetPrivateProfileInt("GameServerInfo", "SCFRingEventWizardRewardItemIndex",13, ReadConfig.ConnDataFiles[0]) ;
			g_iRingEventOrcRewardDropRate =  GetPrivateProfileInt("GameServerInfo", "SCFRingEventOrcRewardDropRate",10, ReadConfig.ConnDataFiles[0]) ;
			g_iRingDropItemOrcType = GetPrivateProfileInt("GameServerInfo", "SCFRingEventOrcRewardItemType",13, ReadConfig.ConnDataFiles[0]) ;
			g_iRingDropItemOrcIndex = GetPrivateProfileInt("GameServerInfo", "SCFRingEventOrcRewardItemIndex",20, ReadConfig.ConnDataFiles[0]) ;
			
			g_RingAttackEvent.Load(ReadConfig.ConnDataFiles[17]);
			g_RingAttackEvent.EnableEvent(g_bDoRingEvent);
		}
		break;

		case 6: //Event 1
			GetPrivateProfileString("GameServerInfo", "EVENT1", "0", szTemp, 5, ReadConfig.ConnDataFiles[0]);
			gEvent1 = atoi(szTemp);
			GetPrivateProfileString("GameServerInfo", "Event1ItemDropTodayMax", "1", szTemp, 5, ReadConfig.ConnDataFiles[0]);
			gEvent1ItemDropTodayCount = 0;
			gEvent1ItemDropTodayMax = atoi(szTemp);
			GetPrivateProfileString("GameServerInfo", "Event1ItemDropTodayPercent", "10000000", szTemp, 5, ReadConfig.ConnDataFiles[0]);
			gEvent1ItemDropTodayPercent = atoi(szTemp);
		break;

		case 7: //Fire Cracker
			gFireCrackerEvent = GetPrivateProfileInt("GameServerInfo","FireCrackerEvent", 0, ReadConfig.ConnDataFiles[0]);
			gFireCrackerDropRate = GetPrivateProfileInt("GameServerInfo","FireCrackerDropRate", 5000, ReadConfig.ConnDataFiles[0]);
			g_ItemDropRateForgFireCracker = GetPrivateProfileInt("GameServerInfo","ItemDropRateForFireCracker", 2, ReadConfig.ConnDataFiles[0]);
		break;

		case 8: //Medal Event
			gMedalEvent = GetPrivateProfileInt("GameServerInfo","MedalEvent", 0, ReadConfig.ConnDataFiles[0]);
			gGoldMedalDropRate = GetPrivateProfileInt("GameServerInfo","GoldMedalDropRate", 2, ReadConfig.ConnDataFiles[0]);
			gSilverMedalDropRate = GetPrivateProfileInt("GameServerInfo","SilverMedalDropRate", 2, ReadConfig.ConnDataFiles[0]);
			g_ItemDropRateForGoldMedal = GetPrivateProfileInt("GameServerInfo","ItemDropRateForGoldMedal", 2, ReadConfig.ConnDataFiles[0]);
			g_ItemDropRateForSilverMedal = GetPrivateProfileInt("GameServerInfo","ItemDropRateForSilverMedal", 2, ReadConfig.ConnDataFiles[0]);
		break;

		//case 9: //XMax Event
			//gXMasEvent = GetPrivateProfileInt("GameServerInfo","XMasEvent", 0, ReadConfig.ConnDataFiles[0]);
		//break;

		case 10: //Heart Of Love Event
			gHeartOfLoveEvent = GetPrivateProfileInt("GameServerInfo","HeartOfLoveEvent", 0, ReadConfig.ConnDataFiles[0]);
			gHeartOfLoveDropRate = GetPrivateProfileInt("GameServerInfo","HeartOfLoveDropRate", 5000, ReadConfig.ConnDataFiles[0]);
			g_ItemDropRateForgHeartOfLove = GetPrivateProfileInt("GameServerInfo","ItemDropRateForHeartOfLove", 2, ReadConfig.ConnDataFiles[0]);
		break;

		/*case 11: //Happy New Year Event
			gHappyNewYearNpcEvent = GetPrivateProfileInt("GameServerInfo", "HappyNewYearTalkNpc", 0, ReadConfig.ConnDataFiles[0]);
			gHappyNewYearNpcEvent = atoi(szTemp);

			if(gHappyNewYearNpcEvent != 0)	//Happy-new-year NPC speaks
			{
				LogAddTD(lMsg.Get(MSGGET(2, 61)));
			}
		break;

		case 12: //Merry  XMax Event NPC talk
			gMerryXMasNpcEvent = GetPrivateProfileInt("GameServerInfo", "MerryXMasTalkNpc", 0, ReadConfig.ConnDataFiles[0]);

			if(gMerryXMasNpcEvent != 0)		//Christmas NPC speaks
			{
				LogAddTD(lMsg.Get(MSGGET(2, 60)));
			}
		break;*/

		case 13: //Chaos Castle
			g_ChaosCastle.Load(ReadConfig.ConnDataFiles[15]);
			g_bChaosCastle = GetPrivateProfileInt("GameServerInfo","ChaosCastleEvent", 0, ReadConfig.ConnDataFiles[0]);
			g_iCheckCanStartPlayCCMinPlayers = GetPrivateProfileInt("GameServerInfo", "SCFChaosCastleMinStart",2, ReadConfig.ConnDataFiles[0]) ;
			g_iGiveWinnerItemCCType1 = GetPrivateProfileInt("GameServerInfo", "SCFChaosCastleReward1Type",14, ReadConfig.ConnDataFiles[0]) ;
			g_iGiveWinnerItemCCIndex1 = GetPrivateProfileInt("GameServerInfo", "SCFChaosCastleReward1Index",14, ReadConfig.ConnDataFiles[0]) ;
			g_iGiveWinnerItemCCType2 = GetPrivateProfileInt("GameServerInfo", "SCFChaosCastleReward2Type",14, ReadConfig.ConnDataFiles[0]) ;
			g_iGiveWinnerItemCCIndex2 = GetPrivateProfileInt("GameServerInfo", "SCFChaosCastleReward2Index",22, ReadConfig.ConnDataFiles[0]) ;
			g_iGiveWinnerItemCCType3 = GetPrivateProfileInt("GameServerInfo", "SCFChaosCastleReward3Type",14, ReadConfig.ConnDataFiles[0]) ;
			g_iGiveWinnerItemCCIndex3 = GetPrivateProfileInt("GameServerInfo", "SCFChaosCastleReward3Index",13, ReadConfig.ConnDataFiles[0]) ;
			g_iGiveWinnerItemCCType4 = GetPrivateProfileInt("GameServerInfo", "SCFChaosCastleReward4Type",14, ReadConfig.ConnDataFiles[0]) ;
			g_iGiveWinnerItemCCIndex4 = GetPrivateProfileInt("GameServerInfo", "SCFChaosCastleReward4Index",16, ReadConfig.ConnDataFiles[0]) ;
		break;

#if (PACK_EDITION>=2)
		case 14:	// XMAS Event
			XMasEvent.Init("..\\SCFData\\EventsConfig\\SCF_XMasEvent.ini");
		break;
#endif

		case 15:	// Chocolate Event
			g_bChocolateBoxEvent = GetPrivateProfileInt("GameServerInfo","ChocolateEvent",0, ReadConfig.ConnDataFiles[0]);
		break;

		case 16:	// Candy Event
			g_bCandyBoxEvent = GetPrivateProfileInt("GameServerInfo","CandyBoxEvent",0, ReadConfig.ConnDataFiles[0]);
		break;
	}
}


void ReadGameEtcInfo(MU_ETC_TYPE eGameEtcType)
{
	char szTemp[256];

	switch ( eGameEtcType )
	{
		case 0:
			GetPrivateProfileString("GameServerInfo", "CreateCharacter", "1", szTemp, 5, ReadConfig.ConnDataFiles[0]);
			gCreateCharacter = atoi(szTemp);

			GetPrivateProfileString("GameServerInfo", "GuildCreate", "1", szTemp, 5, ReadConfig.ConnDataFiles[0]);
			gGuildCreate = atoi(szTemp);
			LogAdd(lMsg.Get(MSGGET(2, 57)), gGuildCreate);
			GetPrivateProfileString("GameServerInfo", "GuildDestroy", "1", szTemp, 5, ReadConfig.ConnDataFiles[0]);
			gGuildDestroy = atoi(szTemp);
			LogAdd(lMsg.Get(MSGGET(2, 58)), gGuildDestroy);
			GetPrivateProfileString("GameServerInfo", "GuildCreateLevel", "100", szTemp, 5, ReadConfig.ConnDataFiles[0]);
			gGuildCreateLevel = atoi(szTemp);
			LogAdd(lMsg.Get(MSGGET(2, 59)), gGuildCreateLevel);

			GetPrivateProfileString("GameServerInfo", "Trade", "1", szTemp, 5, ReadConfig.ConnDataFiles[0]);
			bCanTrade = atoi(szTemp);

			bCanChaosBox = GetPrivateProfileInt("GameServerInfo", "ChaosBox", 0, ReadConfig.ConnDataFiles[0]);
			GetPrivateProfileString("GameServerInfo", "ItemDropPer", "10", szTemp, 5, ReadConfig.ConnDataFiles[0]);
			gItemDropPer = atoi(szTemp);
			gDoPShopOpen = GetPrivateProfileInt("GameServerInfo", "PersonalShopOpen", 0, ReadConfig.ConnDataFiles[0]);

			gAttackSpeedTimeLimit = GetPrivateProfileInt("GameServerInfo", "AttackSpeedTimeLimit", 800, ReadConfig.ConnDataFiles[0]);
			bIsIgnorePacketSpeedHackDetect = GetPrivateProfileInt("GameServerInfo", "IsIgnorePacketHackDetect", 0, ReadConfig.ConnDataFiles[0]);
			gHackCheckCount = GetPrivateProfileInt("GameServerInfo", "HackCheckCount", 5, ReadConfig.ConnDataFiles[0]);
			gMinimumAttackSpeedTime = GetPrivateProfileInt("GameServerInfo", "MinimumAttackSpeedTime", 200 , ReadConfig.ConnDataFiles[0]);
			gDetectedHackKickCount = GetPrivateProfileInt("GameServerInfo", "DetectedHackKickCount", 10, ReadConfig.ConnDataFiles[0]);
			gIsKickDetecHackCountLimit = GetPrivateProfileInt("GameServerInfo", "IsKickDetecHackCountLimit", 0, ReadConfig.ConnDataFiles[0]);
			break;

		case 1:
			GetPrivateProfileString("GameServerInfo", "CreateCharacter", "1", szTemp, 5, ReadConfig.ConnDataFiles[0]);
			gCreateCharacter = atoi(szTemp);

			break;

		case 2:
			GetPrivateProfileString("GameServerInfo", "GuildCreate", "1", szTemp, 5, ReadConfig.ConnDataFiles[0]);
			gGuildCreate = atoi(szTemp);
			LogAdd(lMsg.Get(MSGGET(2, 57)), gGuildCreate);
			GetPrivateProfileString("GameServerInfo", "GuildDestroy", "1", szTemp, 5, ReadConfig.ConnDataFiles[0]);
			gGuildDestroy = atoi(szTemp);
			LogAdd(lMsg.Get(MSGGET(2, 58)), gGuildDestroy);
			GetPrivateProfileString("GameServerInfo", "GuildCreateLevel", "100", szTemp, 5, ReadConfig.ConnDataFiles[0]);
			gGuildCreateLevel = atoi(szTemp);
			LogAdd(lMsg.Get(MSGGET(2, 59)), gGuildCreateLevel);
			break;

		case 3:
			GetPrivateProfileString("GameServerInfo", "Trade", "1", szTemp, 5, ReadConfig.ConnDataFiles[0]);
			bCanTrade = atoi(szTemp);

			break;
		case 4:
			bCanChaosBox = GetPrivateProfileInt("GameServerInfo", "ChaosBox", 0, ReadConfig.ConnDataFiles[0]);

			break;

		case 5:
			gDoPShopOpen = GetPrivateProfileInt("GameServerInfo", "PersonalShopOpen", 0, ReadConfig.ConnDataFiles[0]);

			break;

		case 6:
			PvP.Read(".\\SCF_PvPSystem.ini");
			break;

		case 7:
			GetPrivateProfileString("GameServerInfo", "ItemDropPer", "10", szTemp, 5, ReadConfig.ConnDataFiles[0]);
			gItemDropPer = atoi(szTemp);

			break;

		case 8:
			gAttackSpeedTimeLimit = GetPrivateProfileInt("GameServerInfo", "AttackSpeedTimeLimit", 800, ReadConfig.ConnDataFiles[0]);
			bIsIgnorePacketSpeedHackDetect = GetPrivateProfileInt("GameServerInfo", "IsIgnorePacketHackDetect", 0, ReadConfig.ConnDataFiles[0]);
			gHackCheckCount = GetPrivateProfileInt("GameServerInfo", "HackCheckCount", 5, ReadConfig.ConnDataFiles[0]);
			gMinimumAttackSpeedTime = GetPrivateProfileInt("GameServerInfo", "MinimumAttackSpeedTime", 200 , ReadConfig.ConnDataFiles[0]);
			gDetectedHackKickCount = GetPrivateProfileInt("GameServerInfo", "DetectedHackKickCount", 10, ReadConfig.ConnDataFiles[0]);
			gIsKickDetecHackCountLimit = GetPrivateProfileInt("GameServerInfo", "IsKickDetecHackCountLimit", 0, ReadConfig.ConnDataFiles[0]);

			break;

		case 9:
			gUseNPGGChecksum = GetPrivateProfileInt("GameServerInfo", "UseNPGGChecksum", 0, ReadConfig.ConnDataFiles[0]);

			break;

	}
}
