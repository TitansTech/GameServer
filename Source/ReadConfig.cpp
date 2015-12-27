#include "stdafx.h"
#include "LogProc.h"
#include "News.h"
#include "AutoExp.h"
#include "Marry.h"
#include "PCShop.h"
#include "Raklion.h"
#include "HappyHour.h"
#include "GreenEvent.h"
#include "HitAndUp.h"
#include "BlueEvent.h"
#include "BossAttack.h"
#include "ResetSystem.h"
#include "VIPSystem.h"
#include "SkyEvent.h"
#include "GMSystem.h"
#include "MossShop.h"
#include "CustomQuest.h"
#include "MonsterItemMng.h"
#include "ImperialGuardian.h"
#include "SwampEvent.h"
#include "SummerEvent.h"
#include "DoppelGanger.h"
#include "XMasEvent.h"
#include "CustomJewels.h"
#include "ObjBotPet.h"
#include "SCFVipShop.h"
#include "SCFPvPSystem.h"
#include "QuestS5Info.h"
#include "IllusionTemple.h"
#include "GensPvP.h"
#include "..\include\readscript.h"
#include "ObjBotAlchemist.h"
#include "ObjBotTrader.h"
#include "ObjBotStore.h"
#include "ObjBotBuffer.h"
#include "ObjBotVipShop.h"
#include "ObjLuckyPenny.h"
#include "AntiFloodSystem.h"
#include "HalloweenEvent.h"
#include "SCFBotHidenSeek.h"
#include "Mastering2.h"
#include "ObjCherryBlossom.h"
#include "MuItemShop.h"
#include "ObjSkillAutoLearn.h"
#include "RainItemEvent.h"
#include "CrystalWall.h"
#include "ObjBotRacer.h"
#include "ObjBotReward.h"
#include "ObjBotWarper.h"
#include "ObjTitanRank.h"
#include "ObjTitanLottery.h"
#include "CustomWings.h"
#include "ObjQaA.h"


CReadConfig ReadConfig;
char FilePathSpawn[] = "..\\SCFData\\SCF_SpawnMonsters.txt";
extern int gLogOutType;
extern char szGameServerExeSerial[24];
extern char szClientVersion[8];

extern int  GameServerPort;
extern int  JoinServerPort;
extern int  DataServerPort;
extern int  DataServerPort2;
extern int  ExDbPort;
extern char ExDbIp[256];
extern char DataServerIp2[256];
extern char DataServerIp[256];
extern char JoinServerIp[256];

void CReadConfig::ReadConnections(char * FilePath)
{
	GetPrivateProfileString	("Connect","SCF_JoinServerIP","127.0.0.1",JoinServerIp,255,FilePath);
	GetPrivateProfileString	("Connect","SCF_DataServerIP","127.0.0.1",DataServerIp,255,FilePath);
	GetPrivateProfileString	("Connect","SCF_DataServerIP2","127.0.0.1",DataServerIp2,255,FilePath);
	GetPrivateProfileString	("Connect","SCF_ExDbIP","127.0.0.1",ExDbIp,255,FilePath);

	GameServerPort	= GetPrivateProfileIntA("Connect", "SCF_ServerPort",55900, FilePath) ;
	JoinServerPort	= GetPrivateProfileIntA("Connect", "SCF_JoinServerPort",55970, FilePath) ;
	DataServerPort	= GetPrivateProfileIntA("Connect", "SCF_DataServerPort",55960, FilePath) ;
	DataServerPort2	= GetPrivateProfileIntA("Connect", "SCF_DataServerPort2",55962, FilePath) ;
	ExDbPort		= GetPrivateProfileIntA("Connect", "SCF_ExDbPort",55906, FilePath) ;
}

void CReadConfig::Init()
{
	this->Connect(".\\SCF_Connect.ini",1);
	this->Common("..\\SCFData\\SCF_Common.ini");

#if(IsS6E2_And_EngProtocol_Enabled==1)
	if(this->S6E2 == 1)
		Mastering2.Load("..\\SCFData\\Skills\\SCF_SkillTreeData.txt");
#endif

	this->Character("..\\SCFData\\SCF_Character.ini");
	lMsg.DataFileLoad(this->ConnDataFiles[7]);
	qs5.LoadQuestInfo("..\\SCFData\\SCF_Quest.txt");
	Gens.LoadCfg("..\\SCFData\\SCF_GensPvP.ini");
	Gens.LoadOtherCfg("..\\SCFData\\SCF_GensPvP.txt");
	this->ExtraExp(".\\SCF_ExtraExp.ini");
	this->Skill("..\\SCFData\\Skills\\SCF_Skills.ini");
#if (PACK_EDITION>=2)
	if(ObjSkillAutoLearn.Enabled == 1)
		ObjSkillAutoLearn.Load("..\\SCFData\\Skills\\SCF_SkillsAutoLearn.txt");
#endif
	this->Commands("..\\SCFData\\SCF_Commands.ini");

	this->Items("..\\SCFData\\Items\\SCF_Items.ini");
	this->ChaosMachine("..\\SCFData\\Items\\SCF_ChaosMachine.ini");
	this->SocketOptions("..\\SCFData\\Items\\SCF_SocketOptions.ini","..\\SCFData\\Items\\SCF_SocketOptions.txt");
	g_MonsterItemMng.ItemDropLoadScript("..\\SCFData\\Items\\SCF_ItemDropRate.txt");

	NewsSystem.Init(".\\SCF_News.ini");
	Raklion.ReadINI("..\\SCFData\\EventsConfig\\SCF_RaklionEvent.ini");	
#if (GS_CASTLE==0)
	IllusionTemple.Init("..\\SCFData\\EventsConfig\\SCF_IllusionTemple.ini");
#endif

#if(GS_CASTLE_NOEVENTS == 0)
#if (PACK_EDITION>=1)
	HappyHour.Init("..\\SCFData\\EventsConfig\\SCF_HappyHourEvent.ini");
	GreenEvent.Init("..\\SCFData\\EventsConfig\\SCF_GreenEvent.ini");
	BlueEvent.Init("..\\SCFData\\EventsConfig\\SCF_BlueEvent.ini");
	SummerEvent.Init("..\\SCFData\\EventsConfig\\SCF_SummerEvent.ini");
	g_ImperialGuardian.ReadConfigs("..\\SCFData\\EventsConfig\\SCF_ImperialGuardian.ini");
#endif
#endif

#if (PACK_EDITION>=2)
	Marry.Init("..\\SCFData\\SCF_Marry.ini");

	#if(GS_CASTLE_NOEVENTS == 0)
		HalloweenEvent.Init("..\\SCFData\\EventsConfig\\SCF_HalloweenEvent.ini");
		XMasEvent.Init("..\\SCFData\\EventsConfig\\SCF_XMasEvent.ini");
		SkyEvent.ReadINI("..\\SCFData\\EventsConfig\\SCF_SkyEvent.ini");
		HitAndUp.Init("..\\SCFData\\EventsConfig\\SCF_HitAndUpEvent.ini");
		customQuest.Init("..\\SCFData\\SCF_CustomQuests.ini");
	#endif

	VipSystem.Init(".\\SCF_VIP.ini");
	VipSystem.LoadShopItem("..\\SCFData\\PointsShops\\SCF_VIPBag.txt");

	#if (DSGN_RESET_SYSTEM==0)
		ResetChar.Init(".\\SCF_Reset.ini",0);
		ResetChar.Init(".\\SCF_ResetMasters.ini",1);
	#endif	//DSGN_RESET_SYSTEM

	PvP.Read(".\\SCF_PvPSystem.ini");
#endif
	

#if (DSGN_RESET_SYSTEM==1)
	//ResetChar.Init(".\\SCF_Reset.ini",0);
#endif	//DSGN_RESET_SYSTEM

#if (PACK_EDITION>=3)	

#if (WL_PROTECT==1)
		VM_START_WITHLEVEL(20)
			int MyCheckVar1;
			CHECK_REGISTRATION(MyCheckVar1, 0x25454718)  
			if (MyCheckVar1 == 0x25454718)				
			{				
				BossAttack.Init("..\\SCFData\\EventsConfig\\SCF_BossAttack.ini");
				g_Swamp.ReadConfigs("..\\SCFData\\EventsConfig\\SCF_SwampEvent.ini");
				AntiAFK.Init(".\\SCF_AntiAFK.ini");
				AutoExp.Init("..\\SCFData\\SCF_Common.ini");
				this->Warehouse("..\\SCFData\\SCF_Warehouse.ini");
				botPet.Read("..\\SCFData\\SCFBots\\SCF_BotPet.txt");
				botPet.ReadCfg("..\\SCFData\\SCFBots\\SCF_BotConfigs.ini");
				BotStore.Read("..\\SCFData\\SCFBots\\SCF_BotStore.txt");
				BotBuff.Read("..\\SCFData\\SCFBots\\SCF_BotBuffer.txt");
				BotAlchemist.Read("..\\SCFData\\SCFBots\\SCF_BotAlchemist.txt");
				BotTrader.Read("..\\SCFData\\SCFBots\\SCF_BotTrader.txt");
				BotHider.Init("..\\SCFData\\EventsConfig\\SCF_HideAndSeekEvent.ini");
				RainItemEvent.ReadPositions("..\\SCFData\\EventsConfig\\SCF_RainItemEventPosition.txt");
#if (PACK_EDITION>=3)
				BotRacer.Init("..\\SCFData\\EventsConfig\\SCF_Racer.ini");
				BotReward.Read("..\\SCFData\\SCFBots\\SCF_BotReward.txt");
				BotWarper.Read("..\\SCFData\\SCFBots\\SCF_BotWarper.txt");
				ObjTitanRank.Read("..\\SCFData\\EventsConfig\\SCF_TitanRank.txt");
				ObjTitanLottery.Read("..\\SCFData\\EventsConfig\\SCF_TitanLottery.txt");
				ObjQaA.Read("..\\SCFData\\EventsConfig\\SCF_QaA.txt");
#endif
			}
		VM_END
#else
	#if(GS_CASTLE_NOEVENTS == 0)
		BossAttack.Init("..\\SCFData\\EventsConfig\\SCF_BossAttack.ini");
		#if (GS_CASTLE==1)
			g_Swamp.ReadConfigs("..\\SCFData\\EventsConfig\\SCF_SwampEvent.ini");
		#endif
	#endif

	AntiAFK.Init(".\\SCF_AntiAFK.ini");
	AutoExp.Init("..\\SCFData\\SCF_Common.ini");
	this->Warehouse("..\\SCFData\\SCF_Warehouse.ini");
	botPet.Read("..\\SCFData\\SCFBots\\SCF_BotPet.txt");
	botPet.ReadCfg("..\\SCFData\\SCFBots\\SCF_BotConfigs.ini");
	BotStore.Read("..\\SCFData\\SCFBots\\SCF_BotStore.txt");
	BotBuff.Read("..\\SCFData\\SCFBots\\SCF_BotBuffer.txt");
	BotAlchemist.Read("..\\SCFData\\SCFBots\\SCF_BotAlchemist.txt");
	BotTrader.Read("..\\SCFData\\SCFBots\\SCF_BotTrader.txt");
	BotHider.Init("..\\SCFData\\EventsConfig\\SCF_HideAndSeekEvent.ini");
	RainItemEvent.ReadPositions("..\\SCFData\\EventsConfig\\SCF_RainItemEventPosition.txt");
#if (PACK_EDITION>=3)
	BotRacer.Init("..\\SCFData\\EventsConfig\\SCF_Racer.ini");
	BotReward.Read("..\\SCFData\\SCFBots\\SCF_BotReward.txt");
	BotWarper.Read("..\\SCFData\\SCFBots\\SCF_BotWarper.txt");
	ObjTitanRank.Read("..\\SCFData\\EventsConfig\\SCF_TitanRank.txt");
	ObjTitanLottery.Read("..\\SCFData\\EventsConfig\\SCF_TitanLottery.txt");
	ObjQaA.Read("..\\SCFData\\EventsConfig\\SCF_QaA.txt");
#endif
#endif

#endif
	CherryBlossom.Read("..\\SCFData\\EventsConfig\\SCF_CherryBlossom.ini");
	CherryBlossom.ReadItems("..\\SCFData\\EventsConfig\\SCF_CherryBlossom.txt");

	GMSystem.ReadConfig(".\\SCF_GMSystem.txt");

#if(GS_CASTLE_NOEVENTS == 0)
	MossShop.Init("..\\SCFData\\EventsConfig\\SCF_MossMerchant.ini");
	LuckyPenny.Read("..\\SCFData\\EventsConfig\\SCF_LuckyPenny.ini");
	LuckyPenny.ReadBag("..\\SCFData\\EventsConfig\\SCF_LuckyPenny.txt");
#endif

#if (CRYSTAL_EDITION == 1)
	CJewel.Init();
	CWings.Read("..\\SCFData\\CrystalEdition\\SCF_CustomWings.txt");
#if (PACK_EDITION>=3)
	BotVipShop.Read("..\\SCFData\\CrystalEdition\\BotVipShop\\SCF_BotVipShop.txt");
	SVShop.Init("..\\SCFData\\CrystalEdition\\BotVipShop\\SCF_VipShop.ini","..\\SCFData\\CrystalEdition\\BotVipShop\\SCF_VipShop.txt");
	CrystalWall.Read("..\\SCFData\\CrystalEdition\\SCF_CrystalWall.ini");
#endif
#endif

#if (ENABLE_CSHOP == 1)
	MuItemShop.Init("..\\SCFData\\PointsShops\\WzShop\\SCF_CashShop.ini");
	MuItemShop.Read_Category("..\\SCFData\\PointsShops\\WzShop\\SCF_CashShop_Category.txt");
	MuItemShop.Read_Package("..\\SCFData\\PointsShops\\WzShop\\SCF_CashShop_Package.txt");
	MuItemShop.Read_Product("..\\SCFData\\PointsShops\\WzShop\\SCF_CashShop_Product.txt");
#endif
	AntiFlood.Init();

	LogAddTD("[SCF Configs] All INIS are Loaded!");
}

void CReadConfig::SocketOptions(char * FilePath,char * FilePath2)
{	
	char etiqueta[8];
    for(int n=0;n<MAX_MAP_NUMBER;n++) 
	{
		wsprintf(etiqueta,"Map%02d", n);
		SocketDropRate[n] = GetPrivateProfileInt("SocketDropRate", etiqueta,0, FilePath);
	}

	Soket__MaxSoketsOnItem					= GetPrivateProfileInt("SocketDropOption", "SCFMaxSocketsOnItem",5, FilePath) ;
	Soket__MinSoketsOnItem					= GetPrivateProfileInt("SocketDropOption", "SCFMinSocketsOnItem",0, FilePath) ;
	Soket__EnableExcSocketItemsDrop			= GetPrivateProfileInt("SocketDropOption", "SCFEnableExcSocketItemsDrop",0, FilePath) ;
	Soket__ExcAdditionalPercentOnSoketItem	= GetPrivateProfileInt("SocketDropOption", "SCFExcAdditionalPercentOnSocketItem",5, FilePath) ;
	Soket__ExcMaximumOptionsOnSoketItem		= GetPrivateProfileInt("SocketDropOption", "SCFExcMaximumOptionsOnSocketItem",5, FilePath) ;

	Soket__EnableExcOnZeroSoketItemsDrop	= GetPrivateProfileInt("SocketDropOption", "SCFEnableExcOnZeroSoketItemsDrop",0, FilePath) ;
	Soket__ExcAdditionalPercentOnZSoketItem	= GetPrivateProfileInt("SocketDropOption", "SCFExcAdditionalPercentOnZeroSocketItem",5, FilePath) ;
	Soket__ExcMaximumOptionsOnZSoketItem	= GetPrivateProfileInt("SocketDropOption", "SCFExcMaximumOptionsOnZeroSocketItem",5, FilePath) ;

	Soket__DropEmptySpearRate1				= GetPrivateProfileInt("SocketDropOption", "SCFDropEmptySpearRate1",400, FilePath) ;
	Soket__DropEmptySpearLevel1				= GetPrivateProfileInt("SocketDropOption", "SCFDropEmptySpearMonsterLevel1",20, FilePath) ;
	Soket__DropEmptySpearRate2				= GetPrivateProfileInt("SocketDropOption", "SCFDropEmptySpearRate2",350, FilePath) ;
	Soket__DropEmptySpearLevel2				= GetPrivateProfileInt("SocketDropOption", "SCFDropEmptySpearMonsterLevel2",40, FilePath) ;
	Soket__DropEmptySpearRate3				= GetPrivateProfileInt("SocketDropOption", "SCFDropEmptySpearRate3",300, FilePath) ;
	Soket__DropEmptySpearLevel3				= GetPrivateProfileInt("SocketDropOption", "SCFDropEmptySpearMonsterLevel3",60, FilePath) ;
	Soket__DropEmptySpearRate4				= GetPrivateProfileInt("SocketDropOption", "SCFDropEmptySpearRate4",250, FilePath) ;
	Soket__DropEmptySpearLevel4				= GetPrivateProfileInt("SocketDropOption", "SCFDropEmptySpearMonsterLevel4",80, FilePath) ;
	Soket__DropEmptySpearRate5				= GetPrivateProfileInt("SocketDropOption", "SCFDropEmptySpearRate5",200, FilePath) ;
	Soket__DropEmptySpearLevel5				= GetPrivateProfileInt("SocketDropOption", "SCFDropEmptySpearMonsterLevel5",100, FilePath) ;
	Soket__AutoRemoveSameSocket				= GetPrivateProfileInt("SocketDropOption", "SCFAutoRemoveSameSocket",0, FilePath) ;

	Soket__IsRecoverySlot					= GetPrivateProfileInt("SlotRecovery", "SCFIsRecoverEmptySlotItems",0, FilePath) ;
	Soket__SlotsToAdd						= GetPrivateProfileInt("SlotRecovery", "SCFNumberOfSoketsToAdd",3, FilePath) ;
	Soket__MinExcOptionsCountOnItem			= GetPrivateProfileInt("SlotRecovery", "SCFMinExcOptionsCountOnSoketItem",2, FilePath) ;

	if(Soket__MinExcOptionsCountOnItem > 6)
		Soket__MinExcOptionsCountOnItem = 6;


	memset(SocketOptValue,0,sizeof(SocketOptValue));

	int Token;
	SMDFile = fopen(FilePath2, "r");

	if ( SMDFile == NULL )
	{
		MsgBox("SocketOption data load error %s", FilePath2);
		return;
	}

	for(int i = 0; i < 7; i++)
	{
		int count=0;
		int x=0;
		int Sock=0;
		GetToken();
		int iType = (int)TokenNumber;
		switch(iType)
		{
			case 1:
			{
				Sock = 0;
				count = 6;
			}break;
			case 2:
			{
				Sock = 0x0A;
				count = 5;
			}break;
			case 3:
			{
				Sock = 0x10;
				count = 5;
			}break;
			case 4:
			{
				Sock = 0x15;
				count = 6;
			}break;
			case 5:
			{
				Sock = 0x1D;
				count = 4;
			}break;
			case 6:
			{
				Sock = 0x24;
				count = 1;
			}break;
			case 7:
			{
				Sock = 0xFF;
				count = 1;
			}break;
		}

		while(true)
		{
			Token = GetToken();
			if ( strcmp("end", TokenString) == 0 )
			{
				break;
			}
			SocketOptValue[Sock+x+(50*0)] = TokenNumber;

			if(iType < 7)
			{
				for(int lvl=1;lvl<5;lvl++)
				{
					Token = GetToken();
					SocketOptValue[Sock+x+(50*lvl)] = TokenNumber;
				}
			}
			
			x++;
			if(x >= count)
			{
				Token = GetToken();
				break;
			}
		}
	}
		
	fclose(SMDFile);

	//SocketOptValue[0] = GetPrivateProfileInt("Fire", "SCF_IncreaseSkillAttackPower",20, FilePath) ;
	//SocketOptValue[1] = GetPrivateProfileInt("Fire", "SCF_IncreaseAttackSpeed",7, FilePath) ;
	//SocketOptValue[2] = GetPrivateProfileInt("Fire", "SCF_IncreaseMaximumDamage_SkillPower",30, FilePath) ;
	//SocketOptValue[3] = GetPrivateProfileInt("Fire", "SCF_IncreaseMinimumDamage_SkillPower",20, FilePath) ;
	//SocketOptValue[4] = GetPrivateProfileInt("Fire", "SCF_IncreaseDamage_SkillPower",20, FilePath) ;
	//SocketOptValue[5] = GetPrivateProfileInt("Fire", "SCF_DecreaseAGuse",40, FilePath) ;
	//SocketOptValue[0x0A] = GetPrivateProfileInt("Water", "SCF_IncreaseDefenseSuccessRate_Percent",10, FilePath) ;
	//SocketOptValue[0x0B] = GetPrivateProfileInt("Water", "SCF_IncreaseDefense",30, FilePath) ;
	//SocketOptValue[0x0C] = GetPrivateProfileInt("Water", "SCF_IncreaseDefenseShield_Percent",7, FilePath) ;
	//SocketOptValue[0x0D] = GetPrivateProfileInt("Water", "SCF_DamageReduction",4, FilePath) ;
	//SocketOptValue[0x0E] = GetPrivateProfileInt("Water", "SCF_DamageReflections",5, FilePath) ;
	//SocketOptValue[0x10] = GetPrivateProfileInt("Ice", "SCF_MonsterDieGetLife",8, FilePath) ;
	//SocketOptValue[0x11] = GetPrivateProfileInt("Ice", "SCF_MonsterDieGetMana",8, FilePath) ;
	//SocketOptValue[0x12] = GetPrivateProfileInt("Ice", "SCF_IncreaseSkillAttackPower",37, FilePath) ;
	//SocketOptValue[0x13] = GetPrivateProfileInt("Ice", "SCF_IncreaseAttackSuccessRate",25, FilePath) ;
	//SocketOptValue[0x14] = GetPrivateProfileInt("Ice", "SCF_IncreaseDurabilty_Percent",30, FilePath) ;
	//SocketOptValue[0x15] = GetPrivateProfileInt("Wind", "SCF_IncreaseLifeAutoRecovery",8, FilePath) ;
	//SocketOptValue[0x16] = GetPrivateProfileInt("Wind", "SCF_IncreaseMaximumLife_Percent",4, FilePath) ;
	//SocketOptValue[0x17] = GetPrivateProfileInt("Wind", "SCF_IncreaseMaximumMana_Percent",4, FilePath) ;
	//SocketOptValue[0x18] = GetPrivateProfileInt("Wind", "SCF_IncreaseManaAutoRecovery",7, FilePath) ;
	//SocketOptValue[0x19] = GetPrivateProfileInt("Wind", "SCF_IncreaseMaximumAG",25, FilePath) ;
	//SocketOptValue[0x1A] = GetPrivateProfileInt("Wind", "SCF_IncreaseAGAmount",3, FilePath) ;
	//SocketOptValue[0x1D] = GetPrivateProfileInt("Lightning", "SCF_IncreaseExcellentDamage",15, FilePath) ;
	//SocketOptValue[0x1E] = GetPrivateProfileInt("Lightning", "SCF_IncreaseExcellentDamageSuccessRate",10, FilePath) ;
	//SocketOptValue[0x1F] = GetPrivateProfileInt("Lightning", "SCF_IncreaseCriticalDamage",30, FilePath) ;
	//SocketOptValue[0x20] = GetPrivateProfileInt("Lightning", "SCF_IncreaseCriticalDamageSuccessRate",8, FilePath) ;
	//SocketOptValue[0x24] = GetPrivateProfileInt("Earth", "SCF_IncreaseStamina",30, FilePath) ;
	//SocketOptValue[0xFF] = GetPrivateProfileInt("Bonus", "SCF_IncreaseDamage",11, FilePath) ;
	LogAddTD("[SCF SocketOptions] - %s file is Loaded",FilePath);
	LogAddTD("[SCF SocketOptions] - %s file is Loaded",FilePath2);
}

void CReadConfig::Warehouse(char * FilePath)
{	
	Ware3Secs		= GetPrivateProfileInt("Common", "SCFWarehouse3SecondsBeforeOpen",0, FilePath);

	WareSystem		= GetPrivateProfileInt("System", "SCFWarehouseSystem",0, FilePath);

	WareExLimit		= GetPrivateProfileInt("Config", "SCFWarehouseExLimit",32767, FilePath);
	WareExVipValue	= GetPrivateProfileInt("Config", "SCFWarehouseExVIPPointsValue",100, FilePath);	

	UseGuildWare	= GetPrivateProfileInt("System", "SCFUseGuildWarehouseSystem",0, FilePath);
	GuildWareMap	= GetPrivateProfileInt("Config", "SCFGuildWarehouseMap",79, FilePath);
	GuildWareOnlyExt= GetPrivateProfileInt("Config", "SCFGuildWarehouseOnlyWithExtWare",0, FilePath);

#if (WL_PROTECT==1)
		VM_START_WITHLEVEL(20)
			int MyCheckVar1;
			CHECK_REGISTRATION(MyCheckVar1, 0x55191417)  
			if (MyCheckVar1 != 0x55191417)				
			{
				WareSystem = 0;
			}
		VM_END
#endif
	LogAddTD("[SCF Warehouse] - %s file is Loaded",FilePath);
}


void CReadConfig::ExtraExp(char * FilePath)
{
	ExpSys = GetPrivateProfileInt("System", "SCFExpSystem",0, FilePath) ;
	ShowExp = GetPrivateProfileInt("System", "SCFShowExp",0,FilePath);
	char etiqueta[8];
    for(int n=0;n<MAX_MAP_NUMBER;n++) 
	{
		wsprintf(etiqueta,"Map%02d", n);
		MapExtraExpSingle[n] = GetPrivateProfileInt("MapsExtraExpSingle", etiqueta,0, FilePath) ;
		MapExtraExpParty[n] = GetPrivateProfileInt("MapsExtraExpParty", etiqueta,0, FilePath) ;
	}

	//Party Experience %
	gObjExpParty_viewpercent1				= GetPrivateProfileInt("PartyExp", "SCFPartyExpPercent3",230, FilePath) ;
	gObjExpParty_viewpercent2				= GetPrivateProfileInt("PartyExp", "SCFPartyExpPercent4",270, FilePath) ;
	gObjExpParty_viewpercent3				= GetPrivateProfileInt("PartyExp", "SCFPartyExpPercent5",300, FilePath) ;
	gObjExpParty_viewpercent4				= GetPrivateProfileInt("PartyExp", "SCFPartyExpPercentOther",120, FilePath) ;
	gObjExpParty_viewpercent5				= GetPrivateProfileInt("PartyExp", "SCFPartyExpSetPercent2",160, FilePath) ;
	gObjExpParty_viewpercent6				= GetPrivateProfileInt("PartyExp", "SCFPartyExpSetPercent3",180, FilePath) ;
	gObjExpParty_viewpercent7				= GetPrivateProfileInt("PartyExp", "SCFPartyExpSetPercent4",200, FilePath) ;
	gObjExpParty_viewpercent8				= GetPrivateProfileInt("PartyExp", "SCFPartyExpSetPercent5",220, FilePath) ;
	gObjExpParty_viewpercent9				= GetPrivateProfileInt("PartyExp", "SCFPartyExpSetPercent1",120, FilePath) ;

	//Reset Users EXTRA Experience
	IsResetExtraExp = GetPrivateProfileInt("ResetsExtraExp", "SCFIsResetExtraExp",0, FilePath) ;

	ResetNrExtraExp[0] = GetPrivateProfileInt("ResetsExtraExp", "SCFResetNr1Limit",10, FilePath) ;
	ResetNrExtraExp[1] = GetPrivateProfileInt("ResetsExtraExp", "SCFResetNr2Limit",20, FilePath) ;
	ResetNrExtraExp[2] = GetPrivateProfileInt("ResetsExtraExp", "SCFResetNr3Limit",1000, FilePath) ;
	ResetExtraExp[0] = GetPrivateProfileInt("ResetsExtraExp", "SCFResetNr1Exp",0, FilePath) ;
	ResetExtraExp[1] = GetPrivateProfileInt("ResetsExtraExp", "SCFResetNr2Exp",0, FilePath) ;
	ResetExtraExp[2] = GetPrivateProfileInt("ResetsExtraExp", "SCFResetNr3Exp",0, FilePath) ;

	//Reset Users Penalty Experience
	IsResetPenaltyExp = GetPrivateProfileInt("ResetsPenaltyExp", "SCFIsResetPenaltyExp",0, FilePath) ;

	char Tmp[512] = {0};
	GetPrivateProfileString	("ResetsPenaltyExp","SCFPenaltyExpMultiPerReset","0.0",Tmp,100,FilePath);
	PenaltyExpPerReset = atof(Tmp);

	if ((IsResetExtraExp > 0)&&(IsResetPenaltyExp > 0))
	{
		IsResetExtraExp = 0;
		LogAddTD("[SCF ExtraExp][ERROR] - Disabling ResetExtraExp / Reason: ResetPenaltyExp enabled!");
	}

	//Vip Users
	IsVipExtraExp = GetPrivateProfileInt("VIPExtraExp", "SCFIsVipExtraExp",0, FilePath) ;

	VIPDaysExtraExp[0] = GetPrivateProfileInt("VIPExtraExp", "SCFVIPDays1Limit",30, FilePath) ;
	VIPDaysExtraExp[1] = GetPrivateProfileInt("VIPExtraExp", "SCFVIPDays2Limit",60, FilePath) ;
	VIPDaysExtraExp[2] = GetPrivateProfileInt("VIPExtraExp", "SCFVIPDays3Limit",9999, FilePath) ;
	VIPExtraExp[0] = GetPrivateProfileInt("VIPExtraExp", "SCFVIPDays1Exp",0, FilePath) ;
	VIPExtraExp[1] = GetPrivateProfileInt("VIPExtraExp", "SCFVIPDays2Exp",0, FilePath) ;
	VIPExtraExp[2] = GetPrivateProfileInt("VIPExtraExp", "SCFVIPDays3Exp",0, FilePath) ;

	char Day[12]={0};
    for(int n=0;n<7;n++) 
	{
		wsprintf(Day,"SCFDay_%02d", n);
		ExpDayofWeek[n] = GetPrivateProfileInt("DayOfWeekExtraExp", Day, 0, FilePath) ;
	}
	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);
	ExpDay = ExpDayofWeek[sysTime.wDayOfWeek];

	//for(int i=0;i<MAX_MAP_NUMBER;i++)
	//{
	//	if(MapExtraExpSingle[i] < 0)
	//		MapExtraExpSingle[i] = 1;
	//	if(MapExtraExpParty[i] < 0)
	//		MapExtraExpParty[i] = 1;
	//}

	for(int i=0;i<3;i++)
	{
		if(ResetExtraExp[i] < 1)
			ResetExtraExp[i] = 1;
		if(VIPExtraExp[i] < 1)
			VIPExtraExp[i] = 1;
	}
	LogAddTD("[SCF ExtraExp] - %s file is Loaded",FilePath);
}

void CReadConfig::Connect(char * FilePath,BYTE State)
{
	if(State == 0)
	{
		char etiqueta[12];
		AHLog			= GetPrivateProfileInt("Logs", "SCFWriteAntiHackLog",0, FilePath) ;
		DropLog			= GetPrivateProfileInt("Logs", "SCFWriteDropLog",0, FilePath) ;
		TradeLog		= GetPrivateProfileInt("Logs", "SCFWriteTradeLog",0, FilePath) ;
		GMLog			= GetPrivateProfileInt("Logs", "SCFWriteGMLog",0, FilePath) ;

		PostLog			= GetPrivateProfileInt("Logs", "SCFWritePostLog",0, FilePath) ;
		MSGLog			= GetPrivateProfileInt("Logs", "SCFWriteMSGLog",0, FilePath) ;
		PMLog			= GetPrivateProfileInt("Logs", "SCFWritePMLog",0, FilePath) ;

		#if (WL_PROTECT==1)
			int MyCheckVar;   
			//VM_START_WITHLEVEL(20)
				CLEAR_START
					CHECK_PROTECTION(MyCheckVar, 0x51890254)  	 
					if (MyCheckVar != 0x51890254)
					{	
						UDPPort				= 40001 ;
						strcpy(SCFEXSIP,"127.0.0.2");
						SCFEXSPort			= 80 ;
						isCheckSum			= 1;
						for ( int i=0;i<CONNDATAFILES_NUM;i++)
						{
							strcpy(ConnDataFiles[i],"log");
						}

						for ( int i=0;i<MAX_SHOP;i++)
						{
							strcpy(ConnShopDataFiles[i],"log");
						}

						for ( int i=0;i<EVENTITEMBAG_NUM;i++)
						{
							strcpy(ConnEventDropDataFiles[i],"log");
						}
					}else
					{	
						UDPPort				= GetPrivateProfileInt("Connect", "SCFGsUdpPort",60006, FilePath) ;
						GetPrivateProfileString	("Connect","SCF_ExServerIP","127.0.0.1",SCFEXSIP,100,FilePath);
						SCFEXSPort			= GetPrivateProfileInt("Connect", "SCF_ExServerPort",55999, FilePath) ;			
						
						SCFRSON				= GetPrivateProfileInt("Connect", "SCF_SCFRSOn",0, FilePath) ;
						SCFESON				= GetPrivateProfileInt("Connect", "SCF_SCFESOn",0, FilePath) ;
						SCFPSON				= GetPrivateProfileInt("Connect", "SCF_SCFPostServerOn",0, FilePath) ;
						GetPrivateProfileString	("Connect","SCF_SCFPostServerIP","127.0.0.1",SCFPSIP,100,FilePath);
						SCFPSPort			= GetPrivateProfileInt("Connect", "SCF_SCFPostServerPort",55998, FilePath) ;
						
						SCFIOGSON				= GetPrivateProfileInt("Connect", "SCF_SCFIOGSOn",0, FilePath) ;
						GetPrivateProfileString	("Connect","SCF_SCFIOGSIP","127.0.0.1",SCFIOGSIP,100,FilePath);
						SCFIOGSPort			= GetPrivateProfileInt("Connect", "SCF_SCFIOGSPort",55997, FilePath) ;
						
						isCheckSum			= GetPrivateProfileInt("Connect", "SCFUseCheckSum",0, FilePath) ;
						for ( int i=0;i<CONNDATAFILES_NUM;i++)
						{
							wsprintf(etiqueta,"Data%02d", i);
							GetPrivateProfileString("Data", etiqueta, "", ConnDataFiles[i], 256, FilePath);
						}

						for ( int i=0;i<MAX_SHOP;i++)
						{
							wsprintf(etiqueta,"Shop%02d", i);
							GetPrivateProfileString("Data", etiqueta, "", ConnShopDataFiles[i], 256, FilePath);
						}

						for ( int i=0;i<EVENTITEMBAG_NUM;i++)
						{
							wsprintf(etiqueta,"EventBag%02d", i);
							GetPrivateProfileString("Data", etiqueta, "", ConnEventDropDataFiles[i], 256, FilePath);
						}
					}
				CLEAR_END
			//VM_END
		#else
			UDPPort				= GetPrivateProfileInt("Connect", "SCFGsUdpPort",60006, FilePath) ;
			GetPrivateProfileString	("Connect","SCF_ExServerIP","127.0.0.1",SCFEXSIP,100,FilePath);
			SCFEXSPort			= GetPrivateProfileInt("Connect", "SCF_ExServerPort",55999, FilePath) ;
			
			SCFRSON				= GetPrivateProfileInt("Connect", "SCF_SCFRSOn",0, FilePath) ;
			SCFESON				= GetPrivateProfileInt("Connect", "SCF_SCFESOn",0, FilePath) ;
			SCFPSON				= GetPrivateProfileInt("Connect", "SCF_SCFPostServerOn",0, FilePath) ;
			GetPrivateProfileString	("Connect","SCF_SCFPostServerIP","127.0.0.1",SCFPSIP,100,FilePath);
			SCFPSPort			= GetPrivateProfileInt("Connect", "SCF_SCFPostServerPort",55998, FilePath) ;
						
			SCFIOGSON				= GetPrivateProfileInt("Connect", "SCF_SCFIOGSOn",0, FilePath) ;
			GetPrivateProfileString	("Connect","SCF_SCFIOGSIP","127.0.0.1",SCFIOGSIP,100,FilePath);
			SCFIOGSPort			= GetPrivateProfileInt("Connect", "SCF_SCFIOGSPort",55997, FilePath) ;

			isCheckSum			= GetPrivateProfileInt("Connect", "SCFUseCheckSum",0, FilePath) ;

			for ( int i=0;i<CONNDATAFILES_NUM;i++)
			{
				wsprintf(etiqueta,"Data%02d", i);
				GetPrivateProfileString("Data", etiqueta, "", ConnDataFiles[i], 256, FilePath);
			}

			for ( int i=0;i<MAX_SHOP;i++)
			{
				wsprintf(etiqueta,"Shop%02d", i);
				GetPrivateProfileString("Data", etiqueta, "", ConnShopDataFiles[i], 256, FilePath);
			}

			for ( int i=0;i<EVENTITEMBAG_NUM;i++)
			{
				wsprintf(etiqueta,"EventBag%02d", i);
				GetPrivateProfileString("Data", etiqueta, "", ConnEventDropDataFiles[i], 256, FilePath);
			}
		#endif
	}else
	{
		GetPrivateProfileString	("Common","SCFConnectNotice","Welcome to TitansTech Server!!",ConnectNotice,100,FilePath);
		GSCSSyncTime	= GetPrivateProfileInt("Common", "SCFGSCSSyncTime",1, FilePath) ;
		GSCSSyncTime =  GSCSSyncTime * 1000;
#if (PACK_EDITION>=3)
		GMSystem.Enabled	= GetPrivateProfileInt("Common", "SCFGMSystemEnabled",0, FilePath) ;
		GMSystem.Announce	= GetPrivateProfileInt("Common", "SCFGMSystemAnnounceOnConnect",0, FilePath) ;
#else
		GMSystem.Enabled  = 0;
		GMSystem.Announce = 0;
#endif
		LogAddTD("[SCF Connect] - %s file is Loaded",FilePath);
	}
}

void CReadConfig::Commands(char * FilePath)
{
	CmdItemEnabled		= GetPrivateProfileInt("Item", "SCFItemEnabled",1, FilePath) ;

	CmdPostEnabled		= GetPrivateProfileInt("Post", "SCFPostEnabled",1, FilePath) ;
	CmdPostColor		= GetPrivateProfileInt("Post", "SCFPostColor",2, FilePath) ;
	CmdPostMoney		= GetPrivateProfileInt("Post", "SCFPostMoney",50000, FilePath) ;
	CmdPostLevel		= GetPrivateProfileInt("Post", "SCFPostLevel",50, FilePath) ;

	CmdAddEnabled		= GetPrivateProfileInt("Add", "SCFAddEnabled",1, FilePath) ;
	CmdAddMaxNumber		= GetPrivateProfileInt("Add", "SCFAddMaxNumber",500, FilePath) ;
	CmdAddMoney			= GetPrivateProfileInt("Add", "SCFAddReqMoney",200000, FilePath) ;

	CmdPKClearEnabled	= GetPrivateProfileInt("PkClear", "SCFPKClearEnabled",1, FilePath) ;
	CmdPKClearMoney		= GetPrivateProfileInt("PkClear", "SCFPKClearMoney",100000000, FilePath) ;

	CmdSkinEnabled		= GetPrivateProfileInt("Skin", "SCFSkinEnabled",1, FilePath) ;
	CmdSkinOnlyForGM	= GetPrivateProfileInt("Skin", "SCFSkinOnlyForGM",1, FilePath) ;
	CmdSkinMoney		= GetPrivateProfileInt("Skin", "SCFSkinReqMoney",2000000, FilePath) ;
	CmdSkinLevel		= GetPrivateProfileInt("Skin", "SCFSkinReqLevel",200, FilePath) ;	

	CmdMarryAF			= GetPrivateProfileInt("AntiFlood", "SCFMarryCmdAntiFloodSec",3, FilePath) ;	
	CmdBotPetAF			= GetPrivateProfileInt("AntiFlood", "SCFBotPetCmdAntiFloodSec",3, FilePath) ;	
	CmdPostAF			= GetPrivateProfileInt("AntiFlood", "SCFPostCmdAntiFloodSec",3, FilePath) ;	

	LogAddTD("[SCF Commands] - %s file is Loaded",FilePath);
}

void CReadConfig::Items(char * FilePath)
{
	char szTemp[256];

	CGSellRequestRecvQuestSell	= GetPrivateProfileInt("Common", "SCFCanSellQuestItems", 1, FilePath);
	CMapItem_xItemDropTime		= (GetPrivateProfileInt("Common", "SCFItemsDurationTime", 120, FilePath) * 1000);
	gObjCalCharacterDakHorseDiv	= GetPrivateProfileInt("Common", "SCFDarkHorseDefenseDivisor", 20, FilePath);

	RAVEN_ATTACK_DISTANCE			 	= GetPrivateProfileInt("Common", "SCFDarkSpiritRangeAttackRate", 7, FilePath);
	DarkSpiritAttackDamageMinConstA	 	= GetPrivateProfileInt("Common", "SCFDarkSpiritAttackDamageMinConstA", 180, FilePath);
	DarkSpiritAttackDamageMinConstB	 	= GetPrivateProfileInt("Common", "SCFDarkSpiritAttackDamageMinConstB", 15, FilePath);
	DarkSpiritAttackDamageMinConstC	 	= GetPrivateProfileInt("Common", "SCFDarkSpiritAttackDamageMinConstC", 8, FilePath);
	DarkSpiritAttackDamageMaxConstA	 	= GetPrivateProfileInt("Common", "SCFDarkSpiritAttackDamageMaxConstA", 200, FilePath);
	DarkSpiritAttackDamageMaxConstB	 	= GetPrivateProfileInt("Common", "SCFDarkSpiritAttackDamageMaxConstB", 15, FilePath);
	DarkSpiritAttackDamageMaxConstC	 	= GetPrivateProfileInt("Common", "SCFDarkSpiritAttackDamageMaxConstC", 4, FilePath);
	DarkSpiritAttackSpeedConstA		 	= GetPrivateProfileInt("Common", "SCFDarkSpiritAttackSpeedConstA", 20, FilePath);
	DarkSpiritAttackSpeedConstB		 	= GetPrivateProfileInt("Common", "SCFDarkSpiritAttackSpeedConstB", 4, FilePath);
	DarkSpiritAttackSpeedConstC		 	= GetPrivateProfileInt("Common", "SCFDarkSpiritAttackSpeedConstC", 5, FilePath);
	DarkSpiritAttackSpeedConstD			= GetPrivateProfileInt("Common", "SCFDarkSpiritAttackSpeedConstD", 50, FilePath);
	DarkSpiritAttackSuccessRateConstA 	= GetPrivateProfileInt("Common", "SCFDarkSpiritAttackSuccessRateConstA", 1000, FilePath);
	DarkSpiritAttackSuccessRateConstB 	= GetPrivateProfileInt("Common", "SCFDarkSpiritAttackSuccessRateConstB", 15, FilePath);

	DarkSpiritDurSmallConstA			= GetPrivateProfileInt("Common", "SCFDarkSpiritDurSmallConstA", 2, FilePath);
	DarkSpiritDurSmallConstB			= GetPrivateProfileInt("Common", "SCFDarkSpiritDurSmallConstB", 100, FilePath);
	DarkSpiritDurSmallConstC			= GetPrivateProfileInt("Common", "SCFDarkSpiritDurSmallConstC", 800, FilePath);

	gObjMonsterItemDropConfigScriptEnabled = GetPrivateProfileInt("DropItem", "SCFItemDropScriptEnabled",1, FilePath);
	gObjMonsterDieGiveItemExcDrop		 = GetPrivateProfileInt("DropItem", "SCFExItemDropRate",2000, FilePath);
	gObjMonsterDieRewardItemsExcSkill	 = GetPrivateProfileInt("DropItem", "SCFExItemDropSkillRate",100, FilePath) ;
	gObjMonsterDieRewardItemsExcLuck	 = GetPrivateProfileInt("DropItem", "SCFExItemDropLuckRate",1, FilePath) ;
	gObjMonsterDieRewardItemsNormalSkill = GetPrivateProfileInt("DropItem", "SCFNormalItemDropSkillRate",6, FilePath) ;
	gObjMonsterDieRewardItemsNormalLuck	 = GetPrivateProfileInt("DropItem", "SCFNormalItemDropLuckRate",4, FilePath) ;
	gObjMonsterDieRewardItemsMaxItemsExc = GetPrivateProfileInt("DropItem", "SCFExcellentMaxOptionDrop", 6, FilePath);

	CGUseItemRecvHP0	= GetPrivateProfileInt("Potions", "SCFHpRecoverPercent0",10, FilePath) ;
	CGUseItemRecvHP1	= GetPrivateProfileInt("Potions", "SCFHpRecoverPercent1",20, FilePath) ;
	CGUseItemRecvHP2	= GetPrivateProfileInt("Potions", "SCFHpRecoverPercent2",30, FilePath) ;
	CGUseItemRecvHP3	= GetPrivateProfileInt("Potions", "SCFHpRecoverPercent3",40, FilePath) ;
	CGUseItemRecvMP1	= GetPrivateProfileInt("Potions", "SCFMpRecoverPercent1",20, FilePath) ;
	CGUseItemRecvMP2	= GetPrivateProfileInt("Potions", "SCFMpRecoverPercent2",30, FilePath) ;
	CGUseItemRecvMP3	= GetPrivateProfileInt("Potions", "SCFMpRecoverPercent3",40, FilePath) ;

	SkillChangeUseTransRing1	= GetPrivateProfileInt("Rings", "SCFTransformationRing1",2, FilePath) ;
	SkillChangeUseTransRing2	= GetPrivateProfileInt("Rings", "SCFTransformationRing2",7, FilePath) ;
	SkillChangeUseTransRing3	= GetPrivateProfileInt("Rings", "SCFTransformationRing3",14, FilePath) ;
	SkillChangeUseTransRing4	= GetPrivateProfileInt("Rings", "SCFTransformationRing4",8, FilePath) ;
	SkillChangeUseTransRing5	= GetPrivateProfileInt("Rings", "SCFTransformationRing5",9, FilePath) ;
	SkillChangeUseTransRing6	= GetPrivateProfileInt("Rings", "SCFTransformationRing6",41, FilePath) ;

	CItem__ValueBless		= GetPrivateProfileInt("Jewels", "SCFBlessiPrice",9000000, FilePath) ;
	CItem__ValueSoul		= GetPrivateProfileInt("Jewels", "SCFSoulPrice",6000000, FilePath) ;
	CItem__ValueChaos		= GetPrivateProfileInt("Jewels", "SCFChaosPrice",810000, FilePath) ;
	CItem__ValueLife		= GetPrivateProfileInt("Jewels", "SCFLifePrice",45000000, FilePath) ;
	CItem__ValueCreation	= GetPrivateProfileInt("Jewels", "SCFCreationPrice",36000000, FilePath) ;
	CItem__ValueGuardian	= GetPrivateProfileInt("Jewels", "SCFGuardianPrice",60000000, FilePath) ;
	CItem__ValueBlessWater	= GetPrivateProfileInt("Jewels", "SCFBlessWaterPrice",900000, FilePath) ;
	CItem__ValueSoulWater	= GetPrivateProfileInt("Jewels", "SCFSoulWaterPrice" ,450000, FilePath) ;
	
	gObjItemRandomLevelUpSoulLuck	= GetPrivateProfileInt("Jewels", "SCFSoulWithLuckSuccessRate" ,70, FilePath) ;
	gObjItemRandomLevelUpSoulNoLuck	= GetPrivateProfileInt("Jewels", "SCFSoulNoLuckSuccessRate" ,55, FilePath) ;
	gObjItemRandomOption3UpLife		= GetPrivateProfileInt("Jewels", "SCFLifeSuccessRate" ,50, FilePath) ;

	ItemTradeLimit				= GetPrivateProfileInt("ItemMoveLimit", "SCFItemTradeLimit",0, FilePath) ;
	ItemTradeJOHBlock			= GetPrivateProfileInt("ItemMoveLimit", "SCFBlockHarmonyItemsTrade",0, FilePath) ;
	ItemSellLimit				= GetPrivateProfileInt("ItemMoveLimit", "SCFItemSellLimit",0, FilePath) ;
	ItemMixLimit				= GetPrivateProfileInt("ItemMoveLimit", "SCFItemMixLimit",0, FilePath) ;
	ItemPshopLimit				= GetPrivateProfileInt("ItemMoveLimit", "SCFItemPshopLimit",0, FilePath) ;
	ItemJinhuaLimit				= GetPrivateProfileInt("ItemMoveLimit", "SCFItemJinhuaLimit",0, FilePath) ;
	ItemPlusLimit				= GetPrivateProfileInt("ItemMoveLimit", "SCFItemPlusLimit",13, FilePath) ;
	ExItemPlusLimit				= GetPrivateProfileInt("ItemMoveLimit", "SCFExItemPlusLimit",13, FilePath) ;
	ExItemOpLimit				= GetPrivateProfileInt("ItemMoveLimit", "SCFExItemOpLimit",4 , FilePath) ;
	SetItemPlusLimit			= GetPrivateProfileInt("ItemMoveLimit", "SCFSetItemPlusLimit",13, FilePath) ;

	ItemTradeFFFFBlock			= GetPrivateProfileInt("ItemMoveLimit", "SCFBlockFFFFFFFFSerialTrade",0, FilePath) ;
	BlockFFFFFFFFSerialRepair	= GetPrivateProfileInt("Durability", "SCFBlockFFFFFFFFSerialRepair",0, FilePath) ;
	DeleteFFFFFFFFSerial		= GetPrivateProfileInt("Durability", "SCFDeleteFFFFFFFFSerialItemIfBroken",0, FilePath) ;
	GetPrivateProfileString("Durability", "SCFIncreaseFFFFFFFFSerialDurDownBy", "1.0", szTemp, 5, FilePath);
	IncreaseFFFFFFFFSerialDurDown = atof(szTemp);	

	ItemTradeFFFEBlock			= GetPrivateProfileInt("ItemMoveLimit", "SCFBlockFFFFFFFESerialTrade",0, FilePath) ;
	BlockFFFFFFFESerialRepair	= GetPrivateProfileInt("Durability", "SCFBlockFFFFFFFESerialRepair",0, FilePath) ;
	DeleteFFFFFFFESerial		= GetPrivateProfileInt("Durability", "SCFDeleteFFFFFFFESerialItemIfBroken",0, FilePath) ;
	GetPrivateProfileString("Durability", "SCFIncreaseFFFFFFFESerialDurDownBy", "1.0", szTemp, 5, FilePath);
	IncreaseFFFFFFFESerialDurDown = atof(szTemp);	

	ItemTradeFFFDBlock			= GetPrivateProfileInt("ItemMoveLimit", "SCFBlockFFFFFFFDSerialTrade",0, FilePath) ;
	BlockFFFFFFFDSerialRepair	= GetPrivateProfileInt("Durability", "SCFBlockFFFFFFFDSerialRepair",0, FilePath) ;
	DeleteFFFFFFFDSerial		= GetPrivateProfileInt("Durability", "SCFDeleteFFFFFFFDSerialItemIfBroken",0, FilePath) ;
	GetPrivateProfileString("Durability", "SCFIncreaseFFFFFFFDSerialDurDownBy", "1.0", szTemp, 5, FilePath);
	IncreaseFFFFFFFDSerialDurDown = atof(szTemp);	

	ItemTradeFFFCBlock			= GetPrivateProfileInt("ItemMoveLimit", "SCFBlockFFFFFFFCSerialTrade",0, FilePath) ;
	BlockFFFFFFFCSerialRepair	= GetPrivateProfileInt("Durability", "SCFBlockFFFFFFFCSerialRepair",0, FilePath) ;
	DeleteFFFFFFFCSerial		= GetPrivateProfileInt("Durability", "SCFDeleteFFFFFFFCSerialItemIfBroken",0, FilePath) ;
	GetPrivateProfileString("Durability", "SCFIncreaseFFFFFFFCSerialDurDownBy", "1.0", szTemp, 5, FilePath);
	IncreaseFFFFFFFCSerialDurDown = atof(szTemp);	

	StrengthenItemByJewelOfHarmony_MAXLevel = GetPrivateProfileInt("Harmony", "SCFJOHItemMaxLevelApply",13, FilePath) ;
	MAXJOHITEMOPTION = GetPrivateProfileInt("Harmony", "SCFJOHItemMaxLevel",13, FilePath) ;
	JOHLevelMax = GetPrivateProfileInt("Harmony", "SCFJOHLevelMax",13, FilePath) ;

	if(MAXJOHITEMOPTION > 15)
		MAXJOHITEMOPTION = 15;

	CItem_ConvertExcAncient					= GetPrivateProfileInt("Ancient", "SCFisExcellentAncient", 0, FilePath);
	StrengthenItemByJewelOfHarmonyAncient	= GetPrivateProfileInt("Ancient", "SCFisJOHonAncient", 0, FilePath);

	UseChangeSellMoney						= GetPrivateProfileInt("SellMoney", "SCFUseChangeSellMoneyChange", 0, FilePath);
	MaxSellMoney							= GetPrivateProfileInt("SellMoney", "SCFMaxSellMoney", 0, FilePath);

	LogAddTD("[SCF Items] - %s file is Loaded",FilePath);
}

void CReadConfig::ChaosMachine(char * FilePath)
{
	IsVipExtraMixPercent		= GetPrivateProfileInt("VIPMixExtra", "SCFIsVipExtraMixPercent",1, FilePath) ;
	VipExtraMixPercent			= GetPrivateProfileInt("VIPMixExtra", "SCFVipExtraMixPercent",5, FilePath) ;

	ChaosWeaponMinPercent		= GetPrivateProfileInt("DefaultMix", "SCFChaosWeaponMinPercent",0, FilePath) ;
	ChaosWeaponMaxPercent		= GetPrivateProfileInt("DefaultMix", "SCFChaosWeaponMaxPercent",100, FilePath) ;
	ChaosWeaponMaxMixLevel		= GetPrivateProfileInt("DefaultMix", "SCFChaosWeaponMaxMixLevel",5, FilePath) ;
	ChaosWeaponSkillPercent		= GetPrivateProfileInt("DefaultMix", "SCFChaosWeaponSkillPercent",45, FilePath) ;
	ChaosWeaponLuckPercent		= GetPrivateProfileInt("DefaultMix", "SCFChaosWeaponLuckPercent",35, FilePath) ;
	ChaosWeaponZ28Max			= GetPrivateProfileInt("DefaultMix", "SCFChaosWeaponZ28Max",35, FilePath) ;
	IsAllowExcChaosWeaponMix	= GetPrivateProfileInt("DefaultMix", "SCFIsAllowExcChaosWeapon",0, FilePath) ;
	ExcChaosWeaponPercent		= GetPrivateProfileInt("DefaultMix", "SCFExcChaosWeaponPercent",10, FilePath) ;
	ExcChaosWeaponMaxOpt		= GetPrivateProfileInt("DefaultMix", "SCFExcChaosWeaponMaxOpt",2, FilePath) ;

	FirstWingMinPercent			= GetPrivateProfileInt("DefaultMix", "SCFFirstWingMinPercent",0, FilePath) ;
	FirstWingMaxPercent			= GetPrivateProfileInt("DefaultMix", "SCFFirstWingMaxPercent",100, FilePath) ;
	FirstWingMaxMixLevel		= GetPrivateProfileInt("DefaultMix", "SCFFirstWingMaxMixLevel",5, FilePath) ;
	FirstWingSkillPercent		= GetPrivateProfileInt("DefaultMix", "SCFFirstWingSkillPercent",35, FilePath) ;
	FirstWingLuckPercent		= GetPrivateProfileInt("DefaultMix", "SCFFirstWingLuckPercent",45, FilePath) ;
	FirstWingZ28Percent			= GetPrivateProfileInt("DefaultMix", "SCFFirstWingZ28Percent",70, FilePath) ;
	FirstWingZ28Max				= GetPrivateProfileInt("DefaultMix", "SCFFirstWingZ28Max",3, FilePath) ;

	DinorantMixPercent			= GetPrivateProfileInt("DefaultMix", "SCFDinorantMixPercent",70, FilePath) ;
	DinorantSkillPercent		= GetPrivateProfileInt("DefaultMix", "SCFDinorantSkillPercent",100, FilePath) ;
	DinorantZ28Percent			= GetPrivateProfileInt("DefaultMix", "SCFDinorantZ28Percent",30, FilePath) ;
	DinorantZ28Max				= GetPrivateProfileInt("DefaultMix", "SCFDinorantZ28Max",3, FilePath) ;

	FruitMixPercent				= GetPrivateProfileInt("DefaultMix", "SCFFruitMixPercent",90, FilePath) ;

	DLRFCapeMinPercent			= GetPrivateProfileInt("SecondWingMix", "SCFDLRFCapeMinPercent",0, FilePath) ;
	DLRFCapeMaxPercent			= GetPrivateProfileInt("SecondWingMix", "SCFDLRFCapeMaxPercent",90, FilePath) ;
	RFCapeProbability			= GetPrivateProfileInt("SecondWingMix", "SCFRFCapeProbability",40, FilePath) ;
	DLRFIsUseNewExcMixSystem	= GetPrivateProfileInt("SecondWingMix", "SCFDLRFIsUseNewExcMixSystem",0, FilePath) ;
	DLRFExcOptionProbability	= GetPrivateProfileInt("SecondWingMix", "SCFDLRFExcOptionProbability",20, FilePath) ;
	DLRFMaxExcOption			= GetPrivateProfileInt("SecondWingMix", "SCFDLRFMaxExcOption",3, FilePath) ;
	DLRFCapeLuckPercent			= GetPrivateProfileInt("SecondWingMix", "SCFDLRFCapeLuckPercent",15, FilePath) ;
	DLRFCapeZ28Percent			= GetPrivateProfileInt("SecondWingMix", "SCFDLRFCapeZ28Percent",15, FilePath) ;
	DLRFCapeZ28Max				= GetPrivateProfileInt("SecondWingMix", "SCFDLRFCapeZ28Max",2, FilePath) ;

	SecondWingMinPercent		= GetPrivateProfileInt("SecondWingMix", "SCFSecondWingMinPercent",0, FilePath) ;
	SecondWingMaxPercent		= GetPrivateProfileInt("SecondWingMix", "SCFSecondWingMaxPercent",100, FilePath) ;
	SecondWingIsUseNewExcMix	= GetPrivateProfileInt("SecondWingMix", "SCFSecondWingIsUseNewExcMixSystem",0, FilePath) ;
	SecondWingExcOptionProb		= GetPrivateProfileInt("SecondWingMix", "SCFSecondWingExcOptionProbability",25, FilePath) ;
	SecondWingMaxExcOption		= GetPrivateProfileInt("SecondWingMix", "SCFSecondWingMaxExcOption",3, FilePath) ;
	SecondWingLuckPercent		= GetPrivateProfileInt("SecondWingMix", "SCFSecondWingLuckPercent",15, FilePath) ;
	SecondWingZ28Percent		= GetPrivateProfileInt("SecondWingMix", "SCFSecondWingZ28Percent",15, FilePath) ;
	SecondWingZ28Max			= GetPrivateProfileInt("SecondWingMix", "SCFSecondWingZ28Max",2, FilePath) ;

	CondorMixMinPercent			= GetPrivateProfileInt("ThirdWingMix", "SCFCondorFeatherMixMinPercent",0, FilePath) ;
	CondorMixMaxPercent			= GetPrivateProfileInt("ThirdWingMix", "SCFCondorFeatherMixMaxPercent",0, FilePath) ;

	ThirdWingMinPercent			= GetPrivateProfileInt("ThirdWingMix", "SCFThirdWingMinPercent",0, FilePath) ;
	ThirdWingMaxPercent			= GetPrivateProfileInt("ThirdWingMix", "SCFThirdWingMaxPercent",90, FilePath) ;
	ThirdWingIsUseNewExcMix		= GetPrivateProfileInt("ThirdWingMix", "SCFThirdWingIsUseNewExcMixSystem",0, FilePath) ;
	ThirdWingExcOptionProb		= GetPrivateProfileInt("ThirdWingMix", "SCFThirdWingExcOptionProbability",25, FilePath) ;
	ThirdWingMaxExcOption		= GetPrivateProfileInt("ThirdWingMix", "SCFThirdWingMaxExcOption",3, FilePath) ;
	ThirdWingLuckPercent		= GetPrivateProfileInt("ThirdWingMix", "SCFThirdWingLuckPercent",15, FilePath) ;
	ThirdWingZ28Percent			= GetPrivateProfileInt("ThirdWingMix", "SCFThirdWingZ28Percent",15, FilePath) ;
	ThirdWingZ28Max				= GetPrivateProfileInt("ThirdWingMix", "SCFThirdWingZ28Max",4, FilePath) ;

	SetItemMixMinPercent		= GetPrivateProfileInt("SetItemMix", "SCFSetItemMixMinPercent",0, FilePath) ;
	SetItemMixMaxPercent		= GetPrivateProfileInt("SetItemMix", "SCFSetItemMixMaxPercent",80, FilePath) ;
	SetItemOptionPercent		= GetPrivateProfileInt("SetItemMix", "SCFSetItemOptionPercent",80, FilePath) ;

	DarkHorseMixPercent			= GetPrivateProfileInt("DLPetMix", "SCFDarkHorseMixPercent",60, FilePath) ;
	DarkHorseSkillPercent 		= GetPrivateProfileInt("DLPetMix", "SCFDarkHorseSkillPercent",18, FilePath) ;
	DarkSpiritMixPercent		= GetPrivateProfileInt("DLPetMix", "SCFDarkSpiritMixPercent",60, FilePath) ;
	DarkSpiritSkillPercent		= GetPrivateProfileInt("DLPetMix", "SCFDarkSpiritSkillPercent",18, FilePath) ;

	BlessPotionMixPercent		= GetPrivateProfileInt("SiegeMix", "SCFBlessPotionMixPercent",100, FilePath) ;
	SoulPotionMixPercent		= GetPrivateProfileInt("SiegeMix", "SCFSoulPotionMixPercent",100, FilePath) ;
	LifeStoneMixPercent			= GetPrivateProfileInt("SiegeMix", "SCFLifeStoneMixPercent",100, FilePath) ;
	CastleSpecialMixPercent		= GetPrivateProfileInt("SiegeMix", "SCFCastleSpecialMixPercent",100, FilePath) ;

	b7BlueCrystalMix			= GetPrivateProfileInt("HiddenTreasureMix", "SCF7BlueCrystalMixPercent",100, FilePath) ;
	b5RedCrystalMix				= GetPrivateProfileInt("HiddenTreasureMix", "SCF5RedCrystalMixPercent",100, FilePath) ;
	b3BlackCrystalMix			= GetPrivateProfileInt("HiddenTreasureMix", "SCF3BlackCrystalMixPercent",100, FilePath) ;
	BlueRedBlackCrystalMix		= GetPrivateProfileInt("HiddenTreasureMix", "SCFBlueRedBlackCrystalMixPercent",100, FilePath) ;

	Fenrir01LevelMixRate		= GetPrivateProfileInt("FernirMix", "SCFFenrir01LevelMixRate",100, FilePath) ;
	Fenrir02LevelMixRate		= GetPrivateProfileInt("FernirMix", "SCFFenrir02LevelMixRate",50, FilePath) ;
	Fenrir03LevelMixRate		= GetPrivateProfileInt("FernirMix", "SCFFenrir03LevelMixRatet",30, FilePath) ;
	FenrirUpgradeMaxPercent		= GetPrivateProfileInt("FernirMix", "SCFFenrirUpgradeMaxPercent",79, FilePath) ;

	ShieldPotionLv1MixRate		= GetPrivateProfileInt("ShieldPotionMix", "SCFShieldPotionLv1MixRate",50, FilePath) ;
	ShieldPotionLv1MixMoney		= GetPrivateProfileInt("ShieldPotionMix", "SCFShieldPotionLv1MixMoney",100000, FilePath) ;
	ShieldPotionLv2MixRate		= GetPrivateProfileInt("ShieldPotionMix", "SCFShieldPotionLv2MixRate",30, FilePath) ;
	ShieldPotionLv2MixMoney		= GetPrivateProfileInt("ShieldPotionMix", "SCFShieldPotionLv2MixMoney",500000, FilePath) ;
	ShieldPotionLv3MixRate		= GetPrivateProfileInt("ShieldPotionMix", "SCFShieldPotionLv3MixRate",30, FilePath) ;
	ShieldPotionLv3MixMoney		= GetPrivateProfileInt("ShieldPotionMix", "SCFShieldPotionLv3MixMoney",1000000, FilePath) ;

	SeedMixMaxSuccessRate		= GetPrivateProfileInt("SocketMix", "SCFSeedMixMaxSuccessRate",85, FilePath) ;
	ExtractSeedChaosMixBaseRate = GetPrivateProfileInt("SocketMix", "SCFExtractSeedChaosMixBaseRate",80, FilePath) ; 

	BCChaosSuccessRate1			= GetPrivateProfileInt("BloodCastleMix", "SCFBCInviteChaosSuccessRate1",80, FilePath) ; 
	BCChaosSuccessRate2			= GetPrivateProfileInt("BloodCastleMix", "SCFBCInviteChaosSuccessRate2",80, FilePath) ; 
	BCChaosSuccessRate3			= GetPrivateProfileInt("BloodCastleMix", "SCFBCInviteChaosSuccessRate3",80, FilePath) ; 
	BCChaosSuccessRate4			= GetPrivateProfileInt("BloodCastleMix", "SCFBCInviteChaosSuccessRate4",80, FilePath) ; 
	BCChaosSuccessRate5			= GetPrivateProfileInt("BloodCastleMix", "SCFBCInviteChaosSuccessRate5",80, FilePath) ; 
	BCChaosSuccessRate6			= GetPrivateProfileInt("BloodCastleMix", "SCFBCInviteChaosSuccessRate6",80, FilePath) ; 
	BCChaosSuccessRate7			= GetPrivateProfileInt("BloodCastleMix", "SCFBCInviteChaosSuccessRate7",80, FilePath) ; 
	BCChaosSuccessRate8			= GetPrivateProfileInt("BloodCastleMix", "SCFBCInviteChaosSuccessRate8",80, FilePath) ; 

	BCChaosMixMoney1			= GetPrivateProfileInt("BloodCastleMix", "SCFBCInviteChaosMixMoney1",50000, FilePath) ; 
	BCChaosMixMoney2			= GetPrivateProfileInt("BloodCastleMix", "SCFBCInviteChaosMixMoney2",80000, FilePath) ; 
	BCChaosMixMoney3			= GetPrivateProfileInt("BloodCastleMix", "SCFBCInviteChaosMixMoney3",150000, FilePath) ; 
	BCChaosMixMoney4			= GetPrivateProfileInt("BloodCastleMix", "SCFBCInviteChaosMixMoney4",250000, FilePath) ; 
	BCChaosMixMoney5			= GetPrivateProfileInt("BloodCastleMix", "SCFBCInviteChaosMixMoney5",400000, FilePath) ; 
	BCChaosMixMoney6			= GetPrivateProfileInt("BloodCastleMix", "SCFBCInviteChaosMixMoney6",600000, FilePath) ; 
	BCChaosMixMoney7			= GetPrivateProfileInt("BloodCastleMix", "SCFBCInviteChaosMixMoney7",850000, FilePath) ; 
	BCChaosMixMoney8			= GetPrivateProfileInt("BloodCastleMix", "SCFBCInviteChaosMixMoney8",1100000, FilePath) ; 

	ITChaosSuccessRate1			= GetPrivateProfileInt("IllusionTempleMix", "SCFITChaosSuccessRate1", 70, FilePath);
	ITChaosSuccessRate2			= GetPrivateProfileInt("IllusionTempleMix", "SCFITChaosSuccessRate2", 70, FilePath);
	ITChaosSuccessRate3			= GetPrivateProfileInt("IllusionTempleMix", "SCFITChaosSuccessRate3", 70, FilePath);
	ITChaosSuccessRate4			= GetPrivateProfileInt("IllusionTempleMix", "SCFITChaosSuccessRate4", 70, FilePath);
	ITChaosSuccessRate5			= GetPrivateProfileInt("IllusionTempleMix", "SCFITChaosSuccessRate5", 70, FilePath);
	ITChaosSuccessRate6			= GetPrivateProfileInt("IllusionTempleMix", "SCFITChaosSuccessRate6", 70, FilePath);

	gDQChaosSuccessRateLevel1	= GetPrivateProfileInt("DevilSquareMix","SCFDQChaosSuccessRateLevel1",75, FilePath);
	gDQChaosSuccessRateLevel2	= GetPrivateProfileInt("DevilSquareMix","SCFDQChaosSuccessRateLevel2",70, FilePath);
	gDQChaosSuccessRateLevel3	= GetPrivateProfileInt("DevilSquareMix","SCFDQChaosSuccessRateLevel3",65, FilePath);
	gDQChaosSuccessRateLevel4	= GetPrivateProfileInt("DevilSquareMix","SCFDQChaosSuccessRateLevel4",60, FilePath);
	gDQChaosSuccessRateLevel5	= GetPrivateProfileInt("DevilSquareMix","SCFDQChaosSuccessRateLevel5",55, FilePath);
	gDQChaosSuccessRateLevel6	= GetPrivateProfileInt("DevilSquareMix","SCFDQChaosSuccessRateLevel6",50, FilePath);
	gDQChaosSuccessRateLevel7	= GetPrivateProfileInt("DevilSquareMix","SCFDQChaosSuccessRateLevel7",45, FilePath);

	PlusItemLuckPercent			= (GetPrivateProfileInt("PlusItemMix", "SCFCBmixLuckAddPercent",5, FilePath) )&255;
	PlusItemLevelChaosMix10		= (GetPrivateProfileInt("PlusItemMix", "SCFCBmixStart10",50, FilePath) )&255;
	PlusItemLevelChaosMix11		= (GetPrivateProfileInt("PlusItemMix", "SCFCBmixStart11",45, FilePath) )&255;
	PlusItemLevelChaosMix12		= (GetPrivateProfileInt("PlusItemMix", "SCFCBmixStart12",45, FilePath) )&255;
	PlusItemLevelChaosMix13		= (GetPrivateProfileInt("PlusItemMix", "SCFCBmixStart13",45, FilePath) )&255;
	PlusItemLevelChaosMix14		= (GetPrivateProfileInt("PlusItemMix", "SCFCBmixStart14",40, FilePath) )&255;
	PlusItemLevelChaosMix15		= (GetPrivateProfileInt("PlusItemMix", "SCFCBmixStart15",40, FilePath) )&255;
	PlusItemLevelChaosMaxRate10	= (GetPrivateProfileInt("PlusItemMix", "SCFCBmixMaxRate10",75, FilePath) )&255;
	PlusItemLevelChaosMaxRate11	= (GetPrivateProfileInt("PlusItemMix", "SCFCBmixMaxRate11",75, FilePath) )&255;
	PlusItemLevelChaosMaxRate12	= (GetPrivateProfileInt("PlusItemMix", "SCFCBmixMaxRate12",75, FilePath) )&255;
	PlusItemLevelChaosMaxRate13	= (GetPrivateProfileInt("PlusItemMix", "SCFCBmixMaxRate13",75, FilePath) )&255;
	PlusItemLevelChaosMaxRate14	= (GetPrivateProfileInt("PlusItemMix", "SCFCBmixMaxRate14",75, FilePath) )&255;
	PlusItemLevelChaosMaxRate15	= (GetPrivateProfileInt("PlusItemMix", "SCFCBmixMaxRate15",75, FilePath) )&255;

	PlusItemLevelChaosLuckyRefine255 = GetPrivateProfileInt("LuckyItem", "SCFLuckyJewelMaxDurabilitySuccess",60, FilePath) ;
	ChaosLuckyItemMinLevel		= GetPrivateProfileInt("LuckyItem", "SCFLuckyItemMinLevel",1, FilePath) ;
	ChaosLuckyItemMaxLevel		= GetPrivateProfileInt("LuckyItem", "SCFLuckyItemMaxLevel",15, FilePath) ;
	ChaosLuckyItemMaxOption		= GetPrivateProfileInt("LuckyItem", "SCFLuckyItemMaxOption",7, FilePath) ;
	ChaosLuckyItemLuckPercent	= GetPrivateProfileInt("LuckyItem", "SCFChaosLuckyItemLuckPercent",40, FilePath) ;
	if(ReadConfig.ChaosLuckyItemMaxOption > 7)
		ReadConfig.ChaosLuckyItemMaxOption = 7;

	LogAddTD("[SCF ChaosMachine] - %s file is Loaded",FilePath);
}

void CReadConfig::Common(char * FilePath)
{
	char* cvstr;
	char szTemp[1000]={0};
	char Tmp[512] = {0};

	GetPrivateProfileString("Version","ClientExeVersion","",szTemp,0x14,FilePath);
	GetPrivateProfileString("Version","ClientExeSerial","",szGameServerExeSerial,0x14,FilePath);
	S5E2			= GetPrivateProfileInt("Version", "SCFSeason5e2Enabled",0, FilePath) ;
	S6E1			= GetPrivateProfileInt("Version", "SCFSeason6e1Enabled",0, FilePath) ;
	
#if(IsS6E2_And_EngProtocol_Enabled==1)
	S6E2			= GetPrivateProfileInt("Version", "SCFSeason6e2Enabled",0, FilePath) ;
#else
	S6E2 = 0;
#endif
	if(S6E1 == 1)
		S5E2 = 1;
	cvstr = strtok(szTemp, ".");
	szClientVersion[0] = cvstr[0];
	cvstr = strtok(NULL, ".");
	szClientVersion[1] = cvstr[0];
	szClientVersion[2] = cvstr[1];
	cvstr = strtok(NULL, ".");
	szClientVersion[3] = cvstr[0];
	szClientVersion[4] = cvstr[1];	
	
#if(IsS6E2_And_EngProtocol_Enabled==1)
	IsEngProtocol	 = GetPrivateProfileInt("Version", "SCFIsEngProtocol",0, FilePath) ;
	IsRecoverPShop	 = GetPrivateProfileInt("Version", "SCFIsRecoverPShop",0, FilePath) ;
	IsRecoverMPoints = GetPrivateProfileInt("Version", "SCFIsRecoverMasterPoints",1, FilePath) ;
#else
	IsEngProtocol = 0;
	IsRecoverPShop = 0;
#endif
	
	CSFix_Same_Team_FIX				= GetPrivateProfileInt("Fix", "SCFFixSiegeCanHitSameTeam",0, FilePath);
	CSFix_OnConnect_Warp_FIX		= GetPrivateProfileInt("Fix", "SCFFixSiegeWarpAttackTeamOnConnect",0, FilePath);

	ResponErrorCheck				= GetPrivateProfileInt("Common", "SCFResponErrorCheck",1, FilePath) ;
	ResponErrorTimePlaying			= GetPrivateProfileInt("Common", "SCFResponErrorMSTimePlaying",60000, FilePath) ;
	ResponErrorTimeConnected		= GetPrivateProfileInt("Common", "SCFResponErrorMSTimeConnected",30000, FilePath) ;

	Max_NPC_Phantom_Buffer		= GetPrivateProfileInt("Common", "SCFElfSoldierMaxLevel",180, FilePath) ;
	Min_Guild_Count_For_CS		= GetPrivateProfileInt("Common", "SCFAllianceRequireMembers",20, FilePath) ;
	ResetReq_NpcGuildMasterTalk	= GetPrivateProfileInt("Common", "SCFGuildResetReq",0, FilePath) ;
	GCGuildWarRequestResult_GuildWarMemberReq = GetPrivateProfileInt("Common", "SCFGuildWarMemberReq",0, FilePath) ;
	gLogOutType_WriteInLogFile	= GetPrivateProfileInt("Common", "SCFLogOutType",1, FilePath) ;
	UseSNOCheckCode				= GetPrivateProfileInt("Common", "SCFUseSNOCheckCode",0, FilePath) ;

	TradeAntiHack				= GetPrivateProfileInt("Common", "SCFIsTradeAntiHackTime", 0, FilePath);
	
	TradeAntiHack				= GetPrivateProfileInt("Common", "SCFIsTradeAntiHackTime", 0, FilePath);
	TradeAntiHack				= GetPrivateProfileInt("Common", "SCFIsTradeAntiHackTime", 0, FilePath);

	gLogOutType					= gLogOutType_WriteInLogFile;
	AllowPShopOnlyInLMarket		= GetPrivateProfileInt("Common", "SCFUsePShopOnlyInLorenMarket",1, FilePath) ;

	UseMoveperSecs				= GetPrivateProfileInt("Common", "SCFUseAllowedMoveperSecs",0, FilePath) ;
	AllowedMoveperSecs			= GetPrivateProfileInt("Common", "SCFAllowedMoveperSecs",60, FilePath) ;

	GetPrivateProfileString	("ZenDrop","SCFZenDropMult","1.0",Tmp,100,FilePath);
	ZenDropMult = atof(Tmp);
	IsZenDrop	= GetPrivateProfileInt("ZenDrop", "SCFUseZenDrop",0, FilePath) ;
	ZenDrop	= GetPrivateProfileInt("ZenDrop", "SCFZenDrop",100, FilePath) ;
	//if(ZenDrop > 100)
	//	ZenDrop = 100;
	//ZenDrop = 100 - ZenDrop;

	//CPCShop__SendDataOnlyForGM				= GetPrivateProfileInt("PCPointShop", "SCFOnlyForGM",0, FilePath) ;
	//CPCShop__MonsterDieAddPoints				= GetPrivateProfileInt("PCPointShop", "SCFAddPointsWhenKillMobs",0, FilePath) ;

	LogAddTD("[SCF Common] - %s file is Loaded",FilePath);

	#if (WL_PROTECT==1)
		int MyCheckVar;   
		VM_START_WITHLEVEL(20)
			CHECK_PROTECTION(MyCheckVar, 0x12345678)  	 
			if (MyCheckVar != 0x12345678)
			{				
				for(int i=0;i<6;i++)
				{
					for(int j=0;j<5;j++)
						MaxPoints[i][j]=30;
				}
			}
		VM_END
	#endif

#if (WL_PROTECT==1)
	int MyCheckVar1;
	VM_START
	CHECK_REGISTRATION(MyCheckVar1, 0x64763510)  
	if (MyCheckVar1 != 0x64763510)
	{						
		for(int i=0;i<6;i++)
		{					
			for(int j=0;j<5;j++)
				ReadConfig.MaxPoints[i][j]=30;
		}
	}
	VM_END
#endif
}

void CReadConfig::Ranking()
{
	Ranking_Monsters_GoldenGoblin		= GetPrivateProfileInt("Monsters", "SCFGoldenGoblinSaveInDB",0, RANKINGINI) ;
	Ranking_Monsters_GoldenTitan		= GetPrivateProfileInt("Monsters", "SCFGoldenTitanSaveInDB",0, RANKINGINI) ;
	Ranking_Monsters_GoldenDerkon		= GetPrivateProfileInt("Monsters", "SCFGoldenDerkonSaveInDB",0, RANKINGINI) ;
	Ranking_Monsters_GoldenLizardKing	= GetPrivateProfileInt("Monsters", "SCFGoldenLizardKingSaveInDB",0, RANKINGINI) ;
	Ranking_Monsters_GoldenTantalos		= GetPrivateProfileInt("Monsters", "SCFGoldenTantalosSaveInDB",0, RANKINGINI) ;
	
	Ranking_Monsters_GoldenDarkKnight	= GetPrivateProfileInt("Monsters", "SCFGoldenDarkKnightSaveInDB",0, RANKINGINI) ;
	Ranking_Monsters_GoldenDarkDevil	= GetPrivateProfileInt("Monsters", "SCFGoldenDarkDevilSaveInDB",0, RANKINGINI) ;
	Ranking_Monsters_GoldenStoneGolem	= GetPrivateProfileInt("Monsters", "SCFGoldenStoneGolemSaveInDB",0, RANKINGINI) ;
	Ranking_Monsters_GoldenCrust		= GetPrivateProfileInt("Monsters", "SCFGoldenCrustSaveInDB",0, RANKINGINI) ;
	Ranking_Monsters_GoldenSatyros		= GetPrivateProfileInt("Monsters", "SCFGoldenSatyrosSaveInDB",0, RANKINGINI) ;
	Ranking_Monsters_GoldenTwinTale		= GetPrivateProfileInt("Monsters", "SCFGoldenTwinTaleSaveInDB",0, RANKINGINI) ;
	Ranking_Monsters_GoldenIronKnight	= GetPrivateProfileInt("Monsters", "SCFGoldenIronKnightSaveInDB",0, RANKINGINI) ;
	Ranking_Monsters_GoldenNapin		= GetPrivateProfileInt("Monsters", "SCFGoldenNapinSaveInDB",0, RANKINGINI) ;
	Ranking_Monsters_GoldenGreatDragon	= GetPrivateProfileInt("Monsters", "SCFGoldenGreatDragonSaveInDB",0, RANKINGINI) ;
}

void CReadConfig::Character(char * FilePath)
{
	FixMaxGage							= GetPrivateProfileInt("Common", "SCFIsFixMaxGage",1, FilePath) ;
	ShowMaxGageReached					= GetPrivateProfileInt("Common", "SCFShowMaxGageReachedInfo", 0, FilePath);

	MadicGladiatorLvlReq				= GetPrivateProfileInt("Common", "SCFMadicGladiatorLvlReq",220, FilePath) ;
	DarkLordLvlReq						= GetPrivateProfileInt("Common", "SCFDarkLordLvlReq",250, FilePath) ;
	SummonerLvlReq						= GetPrivateProfileInt("Common", "SCFSummonerLevelReq",1, FilePath) ;
	RageFighterLvlReq					= GetPrivateProfileInt("Common", "SCFRageFighterLevelReq",1, FilePath) ;

	Max_Normal_Level					= GetPrivateProfileInt("Common", "SCFMaxNormalLevel",400, FilePath) ;
	Max_Master_Level					= GetPrivateProfileInt("Common", "SCFMaxMasterLevel",100, FilePath) ;

#if(IsS6E2_And_EngProtocol_Enabled==1)
	char Tmp[512]={0};
	GetPrivateProfileString	("Master","SCFMaster_Div1","2.8",Tmp,100,FilePath);
	Master_Div1 = atof(Tmp);
	if(Master_Div1 <= 0)
		Master_Div1 = 2.8f;
	GetPrivateProfileString	("Master","SCFMaster_Div2","10.7",Tmp,100,FilePath);
	Master_Div2 = atof(Tmp);
	if(Master_Div2 <= 0)
		Master_Div2 = 10.7f;
	GetPrivateProfileString	("Master","SCFMaster_Div3","30.0",Tmp,100,FilePath);
	Master_Div3 = atof(Tmp);
	if(Master_Div3 <= 0)
		Master_Div3 = 30.0f;
	GetPrivateProfileString	("Master","SCFMaster_ValueLimit","800.0",Tmp,100,FilePath);
	Master_ValueLimit = atof(Tmp);
	if(Master_ValueLimit <= 0)
		Master_ValueLimit = 800.0f;
#endif

	DuelManager__GladiatorsGloryExp		= GetPrivateProfileInt("Duel", "SCFGladiatorsGloryExtraExp",20, FilePath) ;
	DuelManager__GladiatorsGloryDrop	= GetPrivateProfileInt("Duel", "SCFGladiatorsGloryExtraDrop",10, FilePath) ;
	DuelManager__IsGlobalMsg			= GetPrivateProfileInt("Duel", "SCFDuelIsGlobalMsg",1, FilePath) ;
	DuelManager__MinLevel				= GetPrivateProfileInt("Duel", "SCFDuelMinLevel",30, FilePath) ;

	DuelManager__IsSaveInDB				= GetPrivateProfileInt("PvP", "SCFIsDuelSaveInDB",0, RANKINGINI) ;
	DuelManager__IsLevelDiferenceForSave= GetPrivateProfileInt("PvP", "SCFIsDuelLevelDiferenceForSaveCheck",0, RANKINGINI) ;
	DuelManager__LevelDiferenceForSave	= GetPrivateProfileInt("PvP", "SCFDuelLevelDiferenceForSave",20, RANKINGINI) ;
	DuelManager__IsResetDiferenceForSave= GetPrivateProfileInt("PvP", "SCFIsDuelResetDiferenceForSaveCheck",0, RANKINGINI) ;
	DuelManager__ResetDiferenceForSave	= GetPrivateProfileInt("PvP", "SCFDuelResetDiferenceForSave",20, RANKINGINI) ;
	DuelManager__MinLevelForSave		= GetPrivateProfileInt("PvP", "SCFDuelMinLevelForSave",1, RANKINGINI) ;
	DuelManager__MinResetForSave		= GetPrivateProfileInt("PvP", "SCFDuelMinResetForSave",0, RANKINGINI) ;
	

	gObjLevelUpMasterTree				= GetPrivateProfileInt("Points", "SCFMasterTreeLevelUpPoint",1, FilePath) ;
	gObjLevelUpDLNormal					= GetPrivateProfileInt("Points", "SCFNormalDLUpPoint",7, FilePath) ;
	gObjLevelUpMGNormal					= GetPrivateProfileInt("Points", "SCFNormalMGUpPoint",7, FilePath) ;
	gObjLevelUpRFNormal					= GetPrivateProfileInt("Points", "SCFNormalRFUpPoint",7, FilePath) ;

	gObjLevelUpDLMGCW					= GetPrivateProfileInt("Points", "SCFCWDLMGRFUpPoint",7, FilePath) ;
	gObjLevelUpDLMGBC					= GetPrivateProfileInt("Points", "SCFBCDLMGRFUpPoint",7, FilePath) ;
	gObjLevelUpDLMGCC					= GetPrivateProfileInt("Points", "SCFCCDLMGRFUpPoint",7, FilePath) ;

	gObjLevelUpGralNormal				= GetPrivateProfileInt("Points", "SCFNormalGeneralUpPoint",5, FilePath) ;
	gObjLevelUpGralCW					= GetPrivateProfileInt("Points", "SCFCWGeneralUpPoint",5, FilePath) ;
	gObjLevelUpGralBC					= GetPrivateProfileInt("Points", "SCFBCGeneralUpPoint",5, FilePath) ;
	gObjLevelUpGralCC					= GetPrivateProfileInt("Points", "SCFCCGeneralUpPoint",5, FilePath) ;

	gObjLevelUpPlusQuestNormal			= GetPrivateProfileInt("Points", "SCFNormalLUP_2ndChgUp",6, FilePath) ;
	gObjLevelUpPlusQuestCW				= GetPrivateProfileInt("Points", "SCFCWLUP_2ndChgUp",6, FilePath) ;
	gObjLevelUpPlusQuestBC				= GetPrivateProfileInt("Points", "SCFBCLUP_2ndChgUp",6, FilePath) ;
	gObjLevelUpPlusQuestCC				= GetPrivateProfileInt("Points", "SCFCCLUP_2ndChgUp",6, FilePath) ;

	gObjLevelUpMasterNormal				= GetPrivateProfileInt("Points", "SCFNormalLUP_3ndChgUpExtraPoint",1, FilePath) ;
	gObjLevelUpMasterBC					= GetPrivateProfileInt("Points", "SCFBCLUP_3ndChgUpExtraPoint",1, FilePath) ;
	gObjLevelUpMasterCC					= GetPrivateProfileInt("Points", "SCFCCLUP_3ndChgUpExtraPoint",1, FilePath) ;
	gObjLevelUpMasterCW					= GetPrivateProfileInt("Points", "SCFCWLUP_3ndChgUpExtraPoint",1, FilePath) ;

	Max_Party_Level_Dif					= GetPrivateProfileInt("Party", "SCFMaxPartyLevelDiferense",130, FilePath) ;

	MaxPoints[0][0]	=	GetPrivateProfileIntA("MaxPoints", "SCFDWMaxStrPoints",65000, FilePath) ;
	MaxPoints[0][1]	=	GetPrivateProfileIntA("MaxPoints", "SCFDWMaxAgiPoints",65000, FilePath) ;
	MaxPoints[0][2]	=	GetPrivateProfileIntA("MaxPoints", "SCFDWMaxVitPoints",65000, FilePath) ;
	MaxPoints[0][3]	=	GetPrivateProfileIntA("MaxPoints", "SCFDWMaxEnePoints",65000, FilePath) ;
	MaxPoints[0][4]	=	0;

	MaxPoints[1][0]	=	GetPrivateProfileIntA("MaxPoints", "SCFDKMaxStrPoints",65000, FilePath) ;
	MaxPoints[1][1]	=	GetPrivateProfileIntA("MaxPoints", "SCFDKMaxAgiPoints",65000, FilePath) ;
	MaxPoints[1][2]	=	GetPrivateProfileIntA("MaxPoints", "SCFDKMaxVitPoints",65000, FilePath) ;
	MaxPoints[1][3]	=	GetPrivateProfileIntA("MaxPoints", "SCFDKMaxEnePoints",65000, FilePath) ;
	MaxPoints[1][4]	=	0;

	MaxPoints[2][0]	=	GetPrivateProfileIntA("MaxPoints", "SCFElfMaxStrPoints",65000, FilePath) ;
	MaxPoints[2][1]	=	GetPrivateProfileIntA("MaxPoints", "SCFElfMaxAgiPoints",65000, FilePath) ;
	MaxPoints[2][2]	=	GetPrivateProfileIntA("MaxPoints", "SCFElfMaxVitPoints",65000, FilePath) ;
	MaxPoints[2][3]	=	GetPrivateProfileIntA("MaxPoints", "SCFElfMaxEnePoints",65000, FilePath) ;
	MaxPoints[2][4]	=	0;

	MaxPoints[3][0]	=	GetPrivateProfileIntA("MaxPoints", "SCFMGMaxStrPoints",65000, FilePath) ;
	MaxPoints[3][1]	=	GetPrivateProfileIntA("MaxPoints", "SCFMGMaxAgiPoints",65000, FilePath) ;
	MaxPoints[3][2]	=	GetPrivateProfileIntA("MaxPoints", "SCFMGMaxVitPoints",65000, FilePath) ;
	MaxPoints[3][3]	=	GetPrivateProfileIntA("MaxPoints", "SCFMGMaxEnePoints",65000, FilePath) ;
	MaxPoints[3][4]	=	0;

	MaxPoints[4][0]	=	GetPrivateProfileIntA("MaxPoints", "SCFDLMaxStrPoints",65000, FilePath) ;
	MaxPoints[4][1]	=	GetPrivateProfileIntA("MaxPoints", "SCFDLMaxAgiPoints",65000, FilePath) ;
	MaxPoints[4][2]	=	GetPrivateProfileIntA("MaxPoints", "SCFDLMaxVitPoints",65000, FilePath) ;
	MaxPoints[4][3]	=	GetPrivateProfileIntA("MaxPoints", "SCFDLMaxEnePoints",65000, FilePath) ;
	MaxPoints[4][4]	=	GetPrivateProfileIntA("MaxPoints", "SCFDLMaxCmdPoints",65000, FilePath) ;

	MaxPoints[5][0]	=	GetPrivateProfileIntA("MaxPoints", "SCFSUMaxStrPoints",65000, FilePath) ;
	MaxPoints[5][1]	=	GetPrivateProfileIntA("MaxPoints", "SCFSUMaxAgiPoints",65000, FilePath) ;
	MaxPoints[5][2]	=	GetPrivateProfileIntA("MaxPoints", "SCFSUMaxVitPoints",65000, FilePath) ;
	MaxPoints[5][3]	=	GetPrivateProfileIntA("MaxPoints", "SCFSUMaxEnePoints",65000, FilePath) ;
	MaxPoints[5][4]	=	0;

	MaxPoints[6][0]	=	GetPrivateProfileIntA("MaxPoints", "SCFRFMaxStrPoints",65000, FilePath) ;
	MaxPoints[6][1]	=	GetPrivateProfileIntA("MaxPoints", "SCFRFMaxAgiPoints",65000, FilePath) ;
	MaxPoints[6][2]	=	GetPrivateProfileIntA("MaxPoints", "SCFRFMaxVitPoints",65000, FilePath) ;
	MaxPoints[6][3]	=	GetPrivateProfileIntA("MaxPoints", "SCFRFMaxEnePoints",65000, FilePath) ;
	MaxPoints[6][4]	=	0;

	gObjCalCharacter_AttackDamageMin_Bow_StrDexDiv_Elf	= GetPrivateProfileInt("Elf", "SCFAttackDamageMin_Bow_StrDexDiv_Elf",7, FilePath) ;
	gObjCalCharacter_AttackDamageMax_Bow_StrDexDiv_Elf	= GetPrivateProfileInt("Elf", "SCFAttackDamageMax_Bow_StrDexDiv_Elf",4, FilePath) ;
	gObjCalCharacter_AttackDamageMin_NoBow_DexDiv_Elf	= GetPrivateProfileInt("Elf", "SCFAttackDamageMin_NoBow_DexDiv_Elf",7, FilePath) ;
	gObjCalCharacter_AttackDamageMin_NoBow_StrDiv_Elf	= GetPrivateProfileInt("Elf", "SCFAttackDamageMin_NoBow_StrDiv_Elf",14, FilePath) ;
	gObjCalCharacter_AttackDamageMax_NoBow_DexDiv_Elf	= GetPrivateProfileInt("Elf", "SCFAttackDamageMax_NoBow_DexDiv_Elf",4, FilePath) ;
	gObjCalCharacter_AttackDamageMax_NoBow_StrDiv_Elf	= GetPrivateProfileInt("Elf", "SCFAttackDamageMax_NoBow_StrDiv_Elf",8, FilePath) ;
	gObjCalCharacter_AttackDamageMin_DexStrDiv_Elf		= GetPrivateProfileInt("Elf", "SCFAttackDamageMin_DexStrDiv_Elf",7, FilePath) ;
	gObjCalCharacter_AttackDamageMax_DexStrDiv_Elf		= GetPrivateProfileInt("Elf", "SCFAttackDamageMax_DexStrDiv_Elf",4, FilePath) ;
	gObjCalCharacter_AttackSpeed_Div_Elf				= GetPrivateProfileInt("Elf", "SCFAttackSpeed_Div_ELF",50, FilePath) ;
	gObjCalCharacter_MagicSpeed_Div_Elf					= GetPrivateProfileInt("Elf", "SCFMagicSpeed_Div_ELF",50, FilePath) ;
	gObjCalCharacter_Defense_Div_Elf					= GetPrivateProfileInt("Elf", "SCFDefense_Div_ELF",10, FilePath) ;
	gObjCalCharacter_Blocking_Div_Elf					= GetPrivateProfileInt("Elf", "SCFSuccessfulBlocking_Div_ELF",4, FilePath) ;
	gObjCalCharacter_m_MagicDamageMin_Div_Elf			= GetPrivateProfileInt("Elf", "SCFMagicDamageMin_Div_Elf",9, FilePath) ;
	gObjCalCharacter_m_MagicDamageMax_Div_Elf			= GetPrivateProfileInt("Elf", "SCFMagicDamageMax_Div_Elf",4, FilePath) ;

	gObjCalCharacter_AttackDamageMin_StrDiv_DK			= GetPrivateProfileInt("DarkNight", "SCFAttackDamageMin_StrDiv_DK",6, FilePath) ;
	gObjCalCharacter_AttackDamageMax_StrDiv_DK			= GetPrivateProfileInt("DarkNight", "SCFAttackDamageMax_StrDiv_DK",4, FilePath) ;
	gObjCalCharacter_AttackSpeed_Div_DK					= GetPrivateProfileInt("DarkNight", "SCFAttackSpeed_Div_DK",15, FilePath) ;
	gObjCalCharacter_MagicSpeed_Div_DK					= GetPrivateProfileInt("DarkNight", "SCFMagicSpeed_Div_DK",20, FilePath) ;
	gObjCalCharacter_Defense_Div_DK						= GetPrivateProfileInt("DarkNight", "SCFDefense_Div_DK",3, FilePath) ;
	gObjCalCharacter_Blocking_Div_DK					= GetPrivateProfileInt("DarkNight", "SCFSuccessfulBlocking_Div_DK",7, FilePath) ;
	gObjCalCharacter_m_MagicDamageMin_Div_DK			= GetPrivateProfileInt("DarkNight", "SCFMagicDamageMin_Div_DK",9, FilePath) ;
	gObjCalCharacter_m_MagicDamageMax_Div_DK			= GetPrivateProfileInt("DarkNight", "SCFMagicDamageMax_Div_DK",4, FilePath) ;

	gObjCalCharacter_AttackDamageMin_StrDiv_DW			= GetPrivateProfileInt("DarkWizard", "SCFAttackDamageMin_StrDiv_DW",8, FilePath) ;
	gObjCalCharacter_AttackDamageMax_StrDiv_DW			= GetPrivateProfileInt("DarkWizard", "SCFAttackDamageMax_StrDiv_DW",4, FilePath) ;
	gObjCalCharacter_AttackSpeed_Div_DW					= GetPrivateProfileInt("DarkWizard", "SCFAttackSpeed_Div_DW",20, FilePath) ;
	gObjCalCharacter_MagicSpeed_Div_DW					= GetPrivateProfileInt("DarkWizard", "SCFMagicSpeed_Div_DW",10, FilePath) ;
	gObjCalCharacter_Defense_Div_DW						= GetPrivateProfileInt("DarkWizard", "SCFDefense_Div_DW",4, FilePath) ;
	gObjCalCharacter_Blocking_Div_DW					= GetPrivateProfileInt("DarkWizard", "SCFSuccessfulBlocking_Div_DW",7, FilePath) ;
	gObjCalCharacter_m_MagicDamageMin_Div_DW			= GetPrivateProfileInt("DarkWizard", "SCFMagicDamageMin_Div_DW",9, FilePath) ;
	gObjCalCharacter_m_MagicDamageMax_Div_DW			= GetPrivateProfileInt("DarkWizard", "SCFMagicDamageMax_Div_DW",4, FilePath) ;

	gObjCalCharacter_AttackDamageMin_StrDiv_MG			= GetPrivateProfileInt("MagicGladiator", "SCFAttackDamageMin_StrDiv_MG",6, FilePath) ;
	gObjCalCharacter_AttackDamageMin_EneDiv_MG			= GetPrivateProfileInt("MagicGladiator", "SCFAttackDamageMin_EneDiv_MG",12, FilePath) ;
	gObjCalCharacter_AttackDamageMax_StrDiv_MG			= GetPrivateProfileInt("MagicGladiator", "SCFAttackDamageMax_StrDiv_MG",4, FilePath) ;
	gObjCalCharacter_AttackDamageMax_EneDiv_MG			= GetPrivateProfileInt("MagicGladiator", "SCFAttackDamageMax_EneDiv_MG",8, FilePath) ;
	gObjCalCharacter_AttackSpeed_Div_MG					= GetPrivateProfileInt("MagicGladiator", "SCFAttackSpeed_Div_MG",15, FilePath) ;
	gObjCalCharacter_MagicSpeed_Div_MG					= GetPrivateProfileInt("MagicGladiator", "SCFMagicSpeed_Div_MG",20, FilePath) ;
	gObjCalCharacter_Defense_Div_MG						= GetPrivateProfileInt("MagicGladiator", "SCFDefense_Div_MG",4, FilePath) ;
	gObjCalCharacter_Blocking_Div_MG					= GetPrivateProfileInt("MagicGladiator", "SCFSuccessfulBlocking_Div_MG",7, FilePath) ;
	gObjCalCharacter_m_MagicDamageMin_Div_MG			= GetPrivateProfileInt("MagicGladiator", "SCFMagicDamageMin_Div_MG",9, FilePath) ;
	gObjCalCharacter_m_MagicDamageMax_Div_MG			= GetPrivateProfileInt("MagicGladiator", "SCFMagicDamageMax_Div_MG",4, FilePath) ;

	gObjCalCharacter_AttackDamageMin_StrDiv_DL			= GetPrivateProfileInt("DarkLord", "SCFAttackDamageMin_StrDiv_DL",7, FilePath) ;
	gObjCalCharacter_AttackDamageMin_EneDiv_DL			= GetPrivateProfileInt("DarkLord", "SCFAttackDamageMin_EneDiv_DL",14, FilePath) ;
	gObjCalCharacter_AttackDamageMax_StrDiv_DL			= GetPrivateProfileInt("DarkLord", "SCFAttackDamageMax_StrDiv_DL",5, FilePath) ;
	gObjCalCharacter_AttackDamageMax_EneDiv_DL			= GetPrivateProfileInt("DarkLord", "SCFAttackDamageMax_EneDiv_DL",10, FilePath) ;
	gObjCalCharacter_AttackSpeed_Div_DL					= GetPrivateProfileInt("DarkLord", "SCFAttackSpeed_Div_DL",10, FilePath) ;
	gObjCalCharacter_MagicSpeed_Div_DL					= GetPrivateProfileInt("DarkLord", "SCFMagicSpeed_Div_DL",10, FilePath) ;
	gObjCalCharacter_Defense_Div_DL						= GetPrivateProfileInt("DarkLord", "SCFDefense_Div_DL",7, FilePath) ;
	gObjCalCharacter_Blocking_Div_DL					= GetPrivateProfileInt("DarkLord", "SCFSuccessfulBlocking_Div_DL",3, FilePath) ;
	gObjCalCharacter_m_MagicDamageMin_Div_DL			= GetPrivateProfileInt("DarkLord", "SCFMagicDamageMin_Div_DL",9, FilePath) ;
	gObjCalCharacter_m_MagicDamageMax_Div_DL			= GetPrivateProfileInt("DarkLord", "SCFMagicDamageMax_Div_DL",4, FilePath) ;

	gObjCalCharacter_AttackDamageMin_StrDiv_RF			= GetPrivateProfileInt("RageFighter", "SCFAttackDamageMin_StrDiv_RF",6, FilePath) ;
	gObjCalCharacter_AttackDamageMin_VitDiv_RF			= GetPrivateProfileInt("RageFighter", "SCFAttackDamageMin_VitDiv_RF",12, FilePath) ;
	gObjCalCharacter_AttackDamageMin_PercentPlus_RF		= GetPrivateProfileInt("RageFighter", "SCFAttackDamageMin_GlovesMultPercent_RF",30, FilePath) ;
	gObjCalCharacter_AttackDamageMax_StrDiv_RF			= GetPrivateProfileInt("RageFighter", "SCFAttackDamageMax_StrDiv_RF",4, FilePath) ;
	gObjCalCharacter_AttackDamageMax_VitDiv_RF			= GetPrivateProfileInt("RageFighter", "SCFAttackDamageMax_VitDiv_RF",8, FilePath) ;
	gObjCalCharacter_AttackDamageMax_PercentPlus_RF		= GetPrivateProfileInt("RageFighter", "SCFAttackDamageMax_GlovesMultPercent_RF",30, FilePath) ;
	gObjCalCharacter_AttackSpeed_Div_RF					= GetPrivateProfileInt("RageFighter", "SCFAttackSpeed_Div_RF",20, FilePath) ;
	gObjCalCharacter_MagicSpeed_Div_RF					= GetPrivateProfileInt("RageFighter", "SCFMagicSpeed_Div_RF",10, FilePath) ;
	gObjCalCharacter_Defense_Div_RF						= GetPrivateProfileInt("RageFighter", "SCFDefense_Div_RF",4, FilePath) ;
	gObjCalCharacter_Blocking_Div_RF					= GetPrivateProfileInt("RageFighter", "SCFSuccessfulBlocking_Div_RF",7, FilePath) ;
	gObjCalCharacter_m_MagicDamageMin_Div_RF			= GetPrivateProfileInt("RageFighter", "SCFMagicDamageMin_Div_RF",9, FilePath) ;
	gObjCalCharacter_m_MagicDamageMax_Div_RF			= GetPrivateProfileInt("RageFighter", "SCFMagicDamageMax_Div_RF",4, FilePath) ;

	gObjCalCharacter_AttackDamageMin_StrDiv_SU			= GetPrivateProfileInt("Summoner", "SCFAttackDamageMin_StrDiv_SU",8, FilePath) ;
	gObjCalCharacter_AttackDamageMax_StrDiv_SU			= GetPrivateProfileInt("Summoner", "SCFAttackDamageMax_StrDiv_SU",4, FilePath) ;
	gObjCalCharacter_AttackSpeed_Div_SU					= GetPrivateProfileInt("Summoner", "SCFAttackSpeed_Div_SU",20, FilePath) ;
	gObjCalCharacter_MagicSpeed_Div_SU					= GetPrivateProfileInt("Summoner", "SCFMagicSpeed_Div_SU",10, FilePath) ;
	gObjCalCharacter_Defense_Div_SU						= GetPrivateProfileInt("Summoner", "SCFDefense_Div_SU",4, FilePath) ;
	gObjCalCharacter_Blocking_Div_SU					= GetPrivateProfileInt("Summoner", "SCFSuccessfulBlocking_Div_SU",7, FilePath) ;
	gObjCalCharacter_m_MagicDamageMin_Div_SU			= GetPrivateProfileInt("Summoner", "SCFMagicDamageMin_Div_SU",9, FilePath) ;
	gObjCalCharacter_m_MagicDamageMax_Div_SU			= GetPrivateProfileInt("Summoner", "SCFMagicDamageMax_Div_SU",4, FilePath) ;

	OfficialHelper_Activate								= GetPrivateProfileInt("OfficialHelper", "OfficialHelper_Activate",1, FilePath) ;
	OfficialHelper_Zen									= GetPrivateProfileInt("OfficialHelper", "OfficialHelper_ZenConsume_x5Mins",500, FilePath) ;


	if(Max_Normal_Level + Max_Master_Level > 1000)
	{
		if (Max_Normal_Level >= 1000)
		{
			Max_Normal_Level = 1000 - 100;
			Max_Master_Level = 1000 - Max_Normal_Level;
		} else {
			Max_Master_Level = 1000 - Max_Normal_Level;
		}
	}

	LogAddTD("[SCF Character] - %s file is Loaded",FilePath);
}

void CReadConfig::Skill(char * FilePath)
{
	char Tmp[512] = {0};
	int IgnorePercent = 0;

#if (PACK_EDITION>=2)
	ObjSkillAutoLearn.Enabled = GetPrivateProfileInt("Common", "SCFEnableSkillAutoLearn",0, FilePath);
#endif
	DisableLevelReq_gObjMagicEnergyCheck = GetPrivateProfileInt("Common", "SCFDisableLevelReqForSkillsUsage",0, FilePath);
	TwistingSlashNoWeapon_CGDurationMagicRecv = GetPrivateProfileInt("Fix", "SCFFixDKTwisting",1, FilePath);
	ResetComboBelow220Level = GetPrivateProfileInt("Fix", "SCFResetComboBelow220Level",1, FilePath);

	//=======================================================================================
	MGOtherSkillBase					= GetPrivateProfileInt("MagicGladiator", "SCFMGOtherSkillBase", 200, FilePath) ;
	MGOtherSkillEneDiv					= GetPrivateProfileInt("MagicGladiator", "SCFMGOtherSkillEneDiv", 20, FilePath) ;
	MGOtherSkillMultDiv					= GetPrivateProfileInt("MagicGladiator", "SCFMGOtherSkillMultDiv", 100, FilePath) ;
	GetPrivateProfileString	("MagicGladiator","SCFMGOtherSkillMultMin","2.0",Tmp,100,FilePath);
	MGOtherSkillMultMin = atof(Tmp);
	GetPrivateProfileString	("MagicGladiator","SCFMGOtherSkillMultMax","2.0",Tmp,100,FilePath);
	MGOtherSkillMultMax = atof(Tmp);

	MGMagicDefenseBase					= GetPrivateProfileInt("MagicGladiator", "SCFMGSoulBarrierDefBase",10, FilePath) ;
	MGMagicDefenseDiv1					= GetPrivateProfileInt("MagicGladiator", "SCFMGSoulBarrierAgilityDiv",50, FilePath) ;
	MGMagicDefenseDiv2					= GetPrivateProfileInt("MagicGladiator", "SCFMGSoulBarrierEnergyDiv",200, FilePath) ;
	MGMagicMaximumDefense				= GetPrivateProfileInt("MagicGladiator", "SCFMGSoulBarrierMaxDeffencePercent",90, FilePath) ;

	MGMagicDefenseTimeLimit				= GetPrivateProfileInt("MagicGladiator", "SCFMGSoulBarrierTimeMaxMin",60, FilePath) ;
	MGMagicDefenseTimeBase				= GetPrivateProfileInt("MagicGladiator", "SCFMGSoulBarrierTimeSecBase",60, FilePath) ;
	MGMagicDefenseTimeDiv1				= GetPrivateProfileInt("MagicGladiator", "SCFMGSoulBarrierTimeDivider",40, FilePath) ;

	MGTwistingSlashBase					= GetPrivateProfileInt("MagicGladiator", "SCFMGTwistingSlashBase", 200, FilePath) ;
	MGTwistingSlashEneDiv				= GetPrivateProfileInt("MagicGladiator", "SCFMGTwistingSlashEneDiv", 20, FilePath) ;
	MGTwistingSlashMultDiv				= GetPrivateProfileInt("MagicGladiator", "SCFMGTwistingSlashMultDiv", 100, FilePath) ;
	GetPrivateProfileString	("MagicGladiator","SCFMGTwistingSlashMultMin","2.0",Tmp,100,FilePath);
	MGTwistingSlashMultMin = atof(Tmp);
	GetPrivateProfileString	("MagicGladiator","SCFMGTwistingSlashMultMax","2.0",Tmp,100,FilePath);
	MGTwistingSlashMultMax = atof(Tmp);

	SpiralSlashBase						= GetPrivateProfileInt("MagicGladiator", "SCFSpiralSlashBase", 200, FilePath) ;
	SpiralSlashEneDiv					= GetPrivateProfileInt("MagicGladiator", "SCFSpiralSlashEneDiv", 20, FilePath) ;
	SpiralSlashMultDiv					= GetPrivateProfileInt("MagicGladiator", "SCFSpiralSlashMultDiv", 100, FilePath) ;
	GetPrivateProfileString	("MagicGladiator","SCFSpiralSlashMultMin","2.0",Tmp,100,FilePath);
	SpiralSlashMultMin = atof(Tmp);
	GetPrivateProfileString	("MagicGladiator","SCFSpiralSlashMultMax","4.0",Tmp,100,FilePath);
	SpiralSlashMultMax = atof(Tmp);

	FireSlashBase						= GetPrivateProfileInt("MagicGladiator", "SCFFireSlashBase", 200, FilePath) ;
	FireSlashEneDiv						= GetPrivateProfileInt("MagicGladiator", "SCFFireSlashEneDiv", 20, FilePath) ;
	FireSlashMultDiv					= GetPrivateProfileInt("MagicGladiator", "SCFFireSlashMultDiv", 100, FilePath) ;
	GetPrivateProfileString	("MagicGladiator","SCFFireSlashMultMin","2.0",Tmp,100,FilePath);
	FireSlashMultMin = atof(Tmp);
	GetPrivateProfileString	("MagicGladiator","SCFFireSlashMultMax","4.0",Tmp,100,FilePath);
	FireSlashMultMax = atof(Tmp);

	PowerSlashBase						= GetPrivateProfileInt("MagicGladiator", "SCFPowerSlashBase", 200, FilePath) ;
	PowerSlashEneDiv					= GetPrivateProfileInt("MagicGladiator", "SCFPowerSlashEneDiv", 20, FilePath) ;
	PowerSlashMultDiv					= GetPrivateProfileInt("MagicGladiator", "SCFPowerSlashMultDiv", 100, FilePath) ;
	GetPrivateProfileString	("MagicGladiator","SCFPowerSlashMultMin","2.0",Tmp,100,FilePath);
	PowerSlashMultMin = atof(Tmp);
	GetPrivateProfileString	("MagicGladiator","SCFPowerSlashMultMax","4.0",Tmp,100,FilePath);
	PowerSlashMultMax = atof(Tmp);

	//=======================================================================================
	DWOtherSkillBase					= GetPrivateProfileInt("DarkWizard", "SCFDWOtherSkillBase", 200, FilePath) ;
	DWOtherSkillEneDiv					= GetPrivateProfileInt("DarkWizard", "SCFDWOtherSkillEneDiv", 20, FilePath) ;
	DWOtherSkillMultDiv					= GetPrivateProfileInt("DarkWizard", "SCFDWOtherSkillMultDiv", 100, FilePath) ;
	GetPrivateProfileString	("DarkWizard","SCFDWOtherSkillMultMax","4.0",Tmp,100,FilePath);
	DWOtherSkillMultMax = atof(Tmp);

	WizardMagicDefenseBase				= GetPrivateProfileInt("DarkWizard", "SCFDwSoulBarrierDefBase",10, FilePath) ;
	WizardMagicDefenseDiv1				= GetPrivateProfileInt("DarkWizard", "SCFDwSoulBarrierAgilityDiv",50, FilePath) ;
	WizardMagicDefenseDiv2				= GetPrivateProfileInt("DarkWizard", "SCFDwSoulBarrierEnergyDiv",200, FilePath) ;
	WizardMagicMaximumDefense			= GetPrivateProfileInt("DarkWizard", "SCFDwSoulBarrierMaxDeffencePercent",90, FilePath) ;

	WizardMagicDefenseTimeLimit			= GetPrivateProfileInt("DarkWizard", "SCFDwSoulBarrierTimeMaxMin",60, FilePath) ;
	WizardMagicDefenseTimeBase			= GetPrivateProfileInt("DarkWizard", "SCFDwSoulBarrierTimeSecBase",60, FilePath) ;
	WizardMagicDefenseTimeDiv1			= GetPrivateProfileInt("DarkWizard", "SCFDwSoulBarrierTimeDivider",40, FilePath) ;

	GetPrivateProfileString	("DarkWizard","SCFMagicCircleMult","0.20",Tmp,100,FilePath);
	MagicCircleMult = atof(Tmp);
	MagicCircleMinDur					= GetPrivateProfileInt("DarkWizard", "SCFMagicCircleMinDur",60, FilePath) ;
	MagicCircleDurDiv					= GetPrivateProfileInt("DarkWizard", "SCFMagicCircleDurDiv",40, FilePath) ;

	NovaBase							= GetPrivateProfileInt("DarkWizard", "SCFNovaBase", 200, FilePath) ;
	NovaEneDiv							= GetPrivateProfileInt("DarkWizard", "SCFNovaEneDiv", 20, FilePath) ;
	NovaMultDiv							= GetPrivateProfileInt("DarkWizard", "SCFNovaMultDiv", 100, FilePath) ;
	GetPrivateProfileString	("DarkWizard","SCFNovaMultMin","1.0",Tmp,100,FilePath);
	NovaMultMin = atof(Tmp);
	GetPrivateProfileString	("DarkWizard","SCFNovaMultMax","3.0",Tmp,100,FilePath);
	NovaMultMax = atof(Tmp);

	//=======================================================================================
	ComboDivisor						= GetPrivateProfileInt("DarkKnight", "SCFDKComboDivisor", 2, FilePath) ;
	DKOtherSkillBase					= GetPrivateProfileInt("DarkKnight", "SCFDKOtherSkillBase", 200, FilePath) ;
	DKOtherSkillEneDiv					= GetPrivateProfileInt("DarkKnight", "SCFDKOtherSkillEneDiv", 20, FilePath) ;
	DKOtherSkillMultDiv					= GetPrivateProfileInt("DarkKnight", "SCFDKOtherSkillMultDiv", 100, FilePath) ;
	GetPrivateProfileString	("DarkKnight","SCFDKOtherSkillMultMax","4.0",Tmp,100,FilePath);
	DKOtherSkillMultMax = atof(Tmp);

	DKTwistingSlashBase					= GetPrivateProfileInt("DarkKnight", "SCFDKTwistingSlashBase", 200, FilePath) ;
	DKTwistingSlashEneDiv				= GetPrivateProfileInt("DarkKnight", "SCFDKTwistingSlashEneDiv", 20, FilePath) ;
	DKTwistingSlashMultDiv				= GetPrivateProfileInt("DarkKnight", "SCFDKTwistingSlashMultDiv", 100, FilePath) ;
	GetPrivateProfileString	("DarkKnight","SCFDKTwistingSlashMultMax","4.0",Tmp,100,FilePath);
	DKTwistingSlashMultMax = atof(Tmp);

	CrescentSlashBase					= GetPrivateProfileInt("DarkKnight", "SCFCrescentSlashBase", 200, FilePath) ;
	CrescentSlashEneDiv					= GetPrivateProfileInt("DarkKnight", "SCFCrescentSlashEneDiv", 20, FilePath) ;
	CrescentSlashMultDiv				= GetPrivateProfileInt("DarkKnight", "SCFCrescentSlashMultDiv", 100, FilePath) ;
	GetPrivateProfileString	("DarkKnight","SCFCrescentSlashMultMax","4.0",Tmp,100,FilePath);
	CrescentSlashMultMax = atof(Tmp);

	DKExplotionBase						= GetPrivateProfileInt("DarkKnight", "SCFDKExplotionBase", 200, FilePath) ;
	DKExplotionEneDiv					= GetPrivateProfileInt("DarkKnight", "SCFDKExplotionEneDiv", 20, FilePath) ;
	DKExplotionMultDiv					= GetPrivateProfileInt("DarkKnight", "SCFDKExplotionMultDiv", 100, FilePath) ;
	GetPrivateProfileString	("DarkKnight","SCFDKExplotionMultMax","4.0",Tmp,100,FilePath);
	DKExplotionMultMax = atof(Tmp);

	DeathStabBase						= GetPrivateProfileInt("DarkKnight", "SCFDeathStabBase", 200, FilePath) ;
	DeathStabEneDiv						= GetPrivateProfileInt("DarkKnight", "SCFDeathStabEneDiv", 20, FilePath) ;
	DeathStabMultDiv					= GetPrivateProfileInt("DarkKnight", "SCFDeathStabMultDiv", 100, FilePath) ;
	GetPrivateProfileString	("DarkKnight","SCFDeathStabMultMax","4.0",Tmp,100,FilePath);
	DeathStabMultMax = atof(Tmp);

	BlowOfFuryBase						= GetPrivateProfileInt("DarkKnight", "SCFBlowOfFuryBase", 200, FilePath) ;
	BlowOfFuryEneDiv					= GetPrivateProfileInt("DarkKnight", "SCFBlowOfFuryEneDiv", 20, FilePath) ;
	BlowOfFuryMultDiv					= GetPrivateProfileInt("DarkKnight", "SCFBlowOfFuryMultDiv", 100, FilePath) ;
	GetPrivateProfileString	("DarkKnight","SCFBlowOfFuryMultMax","4.0",Tmp,100,FilePath);
	BlowOfFuryMultMax = atof(Tmp);

	KnightSkillAddLifeBase				= GetPrivateProfileInt("DarkKnight", "SCFDkAddLifeSkillBase",12, FilePath) ;
	KnightSkillAddLifeDivVit			= GetPrivateProfileInt("DarkKnight", "SCFDkAddLifeSkillVitDivider",100, FilePath) ;
	KnightSkillAddLifeDivEne			= GetPrivateProfileInt("DarkKnight", "SCFDkAddLifeSkillEneDivider",20, FilePath) ;

	KnightMaxMultiplierHP				= GetPrivateProfileInt("DarkKnight", "SCFDkMaxMultiplierHP",100, FilePath) ;
	KnightSkillAddLifeDivGral			= GetPrivateProfileInt("DarkKnight", "SCFDkAddLifeSkillGralDivider",10, FilePath) ;

	KnightSkillAddLifeTimeBase			= GetPrivateProfileInt("DarkKnight", "SCFDkAddLifeSkillDuration",60, FilePath) ;
	KnightSkillAddTimeDivGral			= GetPrivateProfileInt("DarkKnight", "SCFDkAddLifeSkillTimeDivider",100, FilePath) ;
	KnightSkillAddLifeMaxTimeBase		= GetPrivateProfileInt("DarkKnight", "SCFDkAddLifeSkillMaxDuration",1000, FilePath) ;

	//=======================================================================================
	ElfOtherSkillBase						= GetPrivateProfileInt("Elf", "SCFElfOtherSkillBase", 200, FilePath) ;
	ElfOtherSkillEneDiv						= GetPrivateProfileInt("Elf", "SCFElfOtherSkillEneDiv", 20, FilePath) ;
	ElfOtherSkillMultDiv						= GetPrivateProfileInt("Elf", "SCFElfOtherSkillMultDiv", 100, FilePath) ;
	GetPrivateProfileString	("Elf","SCFElfOtherSkillMultMax","4.0",Tmp,100,FilePath);
	ElfOtherSkillMultMax = atof(Tmp);

	SkillMonsterCall1					= GetPrivateProfileInt("Elf", "SCFElfSummonMob1",26, FilePath) ;
	SkillMonsterCall2					= GetPrivateProfileInt("Elf", "SCFElfSummonMob2",32, FilePath) ;
	SkillMonsterCall3					= GetPrivateProfileInt("Elf", "SCFElfSummonMob3",21, FilePath) ;
	SkillMonsterCall4					= GetPrivateProfileInt("Elf", "SCFElfSummonMob4",20, FilePath) ;
	SkillMonsterCall5					= GetPrivateProfileInt("Elf", "SCFElfSummonMob5",10, FilePath) ;
	SkillMonsterCall6					= GetPrivateProfileInt("Elf", "SCFElfSummonMob6",150, FilePath) ;
	SkillMonsterCall7					= GetPrivateProfileInt("Elf", "SCFElfSummonMob7",151, FilePath) ;

	ElfBuffReduceShieldLvl				= GetPrivateProfileInt("Elf", "SCFElfBuffReduceDmgLvlMult",5, FilePath) ;
	ElfBuffReduceShieldDiv				= GetPrivateProfileInt("Elf", "SCFElfBuffReduceDmgDiv",15, FilePath) ;

	ElfBuffHealingLvl					= GetPrivateProfileInt("Elf", "SCFElfBuffHealingLvlMult",5, FilePath) ;
	ElfBuffHealingDiv					= GetPrivateProfileInt("Elf", "SCFElfBuffHealingDiv",5, FilePath) ;

	ElfBuffDefenseLvlMult				= GetPrivateProfileInt("Elf", "SCFElfBuffDefenseLvlMult",2, FilePath) ;
	ElfBuffDefenseDiv					= GetPrivateProfileInt("Elf", "SCFElfBuffDefenseDiv",8, FilePath) ;
	GetPrivateProfileString	("Elf","SCFElfBuffDefenseAddPlus","0.1",Tmp,100,FilePath);
	ElfBuffDefenseAddPlus				= atof(Tmp);
	ElfBuffDefenseTimeEneDiv			= GetPrivateProfileInt("Elf", "SCFElfBuffDefenseTimeEneDiv",100, FilePath) ;
	ElfBuffDefenseTime					= GetPrivateProfileInt("Elf", "SCFElfBuffDefenseTime",60, FilePath) ;
	ElfBuffDefenseTimeMax				= GetPrivateProfileInt("Elf", "SCFElfBuffDefenseTimeMaxDuration",60, FilePath) ;
	
	ElfBuffAttackLvlMult				= GetPrivateProfileInt("Elf", "SCFElfBuffAttackLvlMult",3, FilePath) ;
	GetPrivateProfileString	("Elf","SCFElfBuffAttackAddPlus","0.1",Tmp,100,FilePath);
	ElfBuffAttackAddPlus				= atof(Tmp);
	ElfBuffAttackDiv					= GetPrivateProfileInt("Elf", "SCFElfBuffAttackDiv", 7, FilePath);

	ElfBuffAttackTime					= GetPrivateProfileInt("Elf", "SCFElfBuffAttackTime",60, FilePath) ;
	ElfBuffAttackTimeEneDiv				= GetPrivateProfileInt("Elf", "SCFElfBuffAttackTimeEneDiv",100, FilePath) ;
	ElfBuffAttackTimeMax				= GetPrivateProfileInt("Elf", "SCFElfBuffAttackTimeMaxDuration",120, FilePath) ;

	StarFallBase						= GetPrivateProfileInt("Elf", "SCFStarFallBase", 200, FilePath) ;
	StarFallEneDiv						= GetPrivateProfileInt("Elf", "SCFStarFallEneDiv", 20, FilePath) ;
	StarFallMultDiv						= GetPrivateProfileInt("Elf", "SCFStarFallMultDiv", 100, FilePath) ;
	GetPrivateProfileString	("Elf","SCFStarFallMultMin","2.0",Tmp,100,FilePath);
	StarFallMultMin = atof(Tmp);
	GetPrivateProfileString	("Elf","SCFStarFallMultMax","4.0",Tmp,100,FilePath);
	StarFallMultMax = atof(Tmp);

	IceArrowBase						= GetPrivateProfileInt("Elf", "SCFIceArrowBase", 200, FilePath) ;
	IceArrowEneDiv						= GetPrivateProfileInt("Elf", "SCFIceArrowEneDiv", 20, FilePath) ;
	IceArrowMultDiv						= GetPrivateProfileInt("Elf", "SCFIceArrowMultDiv", 100, FilePath) ;
	GetPrivateProfileString	("Elf","SCFIceArrowMultMin","2.0",Tmp,100,FilePath);
	IceArrowMultMin = atof(Tmp);
	GetPrivateProfileString	("Elf","SCFIceArrowMultMax","4.0",Tmp,100,FilePath);
	IceArrowMultMax = atof(Tmp);

	PenetrationBase						= GetPrivateProfileInt("Elf", "SCFPenetrationBase", 200, FilePath) ;
	PenetrationEneDiv					= GetPrivateProfileInt("Elf", "SCFPenetrationEneDiv", 20, FilePath) ;
	PenetrationMultDiv					= GetPrivateProfileInt("Elf", "SCFPenetrationMultDiv", 100, FilePath) ;
	GetPrivateProfileString	("Elf","SCFPenetrationMultMin","2.0",Tmp,100,FilePath);
	PenetrationMultMin = atof(Tmp);
	GetPrivateProfileString	("Elf","SCFPenetrationMultMax","4.0",Tmp,100,FilePath);
	PenetrationMultMax = atof(Tmp);

	FiveShotBase						= GetPrivateProfileInt("Elf", "SCFFiveShotBase", 200, FilePath) ;
	FiveShotEneDiv						= GetPrivateProfileInt("Elf", "SCFFiveShotEneDiv", 20, FilePath) ;
	FiveShotMultDiv						= GetPrivateProfileInt("Elf", "SCFFiveShotMultDiv", 100, FilePath) ;
	GetPrivateProfileString	("Elf","SCFFiveShotMultMin","2.0",Tmp,100,FilePath);
	FiveShotMultMin = atof(Tmp);
	GetPrivateProfileString	("Elf","SCFFiveShotMultMax","4.0",Tmp,100,FilePath);
	FiveShotMultMax = atof(Tmp);

	//=======================================================================================
	DLAddSkillDmgStrDiv						= GetPrivateProfileInt("DarkLord", "SCFDLAddSkillDmgStrDiv", 25, FilePath) ;
	DLAddSkillDmgEneDiv						= GetPrivateProfileInt("DarkLord", "SCFDLAddSkillDmgEneDiv", 50, FilePath) ;

	DLOtherSkillBase						= GetPrivateProfileInt("DarkLord", "SCFDLOtherSkillBase", 200, FilePath) ;
	DLOtherSkillEneDiv						= GetPrivateProfileInt("DarkLord", "SCFDLOtherSkillEneDiv", 20, FilePath) ;
	DLOtherSkillMultDiv						= GetPrivateProfileInt("DarkLord", "SCFDLOtherSkillMultDiv", 100, FilePath) ;
	GetPrivateProfileString	("DarkLord","SCFDLOtherSkillMultMin","2.0",Tmp,100,FilePath);
	DLOtherSkillMultMin = atof(Tmp);
	GetPrivateProfileString	("DarkLord","SCFDLOtherSkillMultMax","2.0",Tmp,100,FilePath);
	DLOtherSkillMultMax = atof(Tmp);

	IsSkillFireBurstStr					= GetPrivateProfileInt("DarkLord", "SCFIsSkillFireBurstStrBased",1, FilePath) ;
	FireBurstBase						= GetPrivateProfileInt("DarkLord", "SCFFireBurstBase", 200, FilePath) ;
	FireBurstEneDiv						= GetPrivateProfileInt("DarkLord", "SCFFireBurstEneDiv", 20, FilePath) ;
	FireBurstMultDiv					= GetPrivateProfileInt("DarkLord", "SCFFireBurstMultDiv", 100, FilePath) ;
	GetPrivateProfileString	("DarkLord","SCFFireBurstMultMax","4.0",Tmp,100,FilePath);
	FireBurstMultMax = atof(Tmp);

	FireScreamBase						= GetPrivateProfileInt("DarkLord", "SCFFireScreamBase", 200, FilePath) ;
	FireScreamEneDiv					= GetPrivateProfileInt("DarkLord", "SCFFireScreamEneDiv", 20, FilePath) ;
	FireScreamMultDiv					= GetPrivateProfileInt("DarkLord", "SCFFireScreamMultDiv", 100, FilePath) ;
	GetPrivateProfileString	("DarkLord","SCFFireScreamMultMax","4.0",Tmp,100,FilePath);
	FireScreamMultMax = atof(Tmp);

	FireBlastBase						= GetPrivateProfileInt("DarkLord", "SCFFireBlastBase", 200, FilePath) ;
	FireBlastEneDiv					= GetPrivateProfileInt("DarkLord", "SCFFireBlastEneDiv", 20, FilePath) ;
	FireBlastMultDiv					= GetPrivateProfileInt("DarkLord", "SCFFireBlastMultDiv", 100, FilePath) ;
	GetPrivateProfileString	("DarkLord","SCFFireBlastMultMax","4.0",Tmp,100,FilePath);
	FireBlastMultMax = atof(Tmp);

	DarkHorseMovePlayerInCS				= GetPrivateProfileInt("DarkLord", "SCFDarkHorseMovePlayerInCS", 1, FilePath) ;
	DarkHorseDmgStrDiv					= GetPrivateProfileInt("DarkLord", "SCFDarkHorseDmgStrDiv", 10, FilePath) ;
	DarkHorseDmgCmdDiv					= GetPrivateProfileInt("DarkLord", "SCFDarkHorseDmgCmdDiv", 5, FilePath) ;
	DarkHorseDmgLvlMul					= GetPrivateProfileInt("DarkLord", "SCFDarkHorseDmgLvlMul", 10, FilePath) ;
	DarkHorseBase						= GetPrivateProfileInt("DarkLord", "SCFDarkHorseBase", 200, FilePath) ;
	DarkHorseEneDiv						= GetPrivateProfileInt("DarkLord", "SCFDarkHorseEneDiv", 20, FilePath) ;
	DarkHorseMultDiv					= GetPrivateProfileInt("DarkLord", "SCFDarkHorseMultDiv", 100, FilePath) ;
	GetPrivateProfileString	("DarkLord","SCFDarkHorseMultMax","4.0",Tmp,100,FilePath);
	DarkHorseMultMax = atof(Tmp);

	ElectricSparkDmgCmdDiv				= GetPrivateProfileInt("DarkLord", "SCFElectricSparkDmgCmdDiv", 10, FilePath) ;
	ElectricSparkDmgPartyMul			= GetPrivateProfileInt("DarkLord", "SCFElectricSparkDmgPartyMul", 50, FilePath) ;
	ElectricSparkBase					= GetPrivateProfileInt("DarkLord", "SCFElectricSparkBase", 200, FilePath) ;
	ElectricSparkEneDiv					= GetPrivateProfileInt("DarkLord", "SCFElectricSparkEneDiv", 20, FilePath) ;
	ElectricSparkMultDiv				= GetPrivateProfileInt("DarkLord", "SCFElectricSparkMultDiv", 100, FilePath) ;
	GetPrivateProfileString	("DarkLord","SCFElectricSparkMultMax","4.0",Tmp,100,FilePath);
	ElectricSparkMultMax = atof(Tmp);

	BirdsBase							= GetPrivateProfileInt("DarkLord", "SCFBirdsBase", 200, FilePath) ;
	BirdsEneDiv							= GetPrivateProfileInt("DarkLord", "SCFBirdsEneDiv", 20, FilePath) ;
	BirdsMultDiv						= GetPrivateProfileInt("DarkLord", "SCFBirdsMultDiv", 100, FilePath) ;
	GetPrivateProfileString	("DarkLord","SCFBirdsMultMax","4.0",Tmp,100,FilePath);
	BirdsMultMax = atof(Tmp);

	ForceBase							= GetPrivateProfileInt("DarkLord", "SCFForceBase", 200, FilePath) ;
	ForceEneDiv							= GetPrivateProfileInt("DarkLord", "SCFForceEneDiv", 20, FilePath) ;
	ForceMultDiv						= GetPrivateProfileInt("DarkLord", "SCFForceMultDiv", 100, FilePath) ;
	GetPrivateProfileString	("DarkLord","SCFForceMultMax","4.0",Tmp,100,FilePath);
	ForceMultMax = atof(Tmp);

	ForceWaveBase							= GetPrivateProfileInt("DarkLord", "SCFForceWaveBase", 200, FilePath) ;
	ForceWaveEneDiv							= GetPrivateProfileInt("DarkLord", "SCFForceWaveEneDiv", 20, FilePath) ;
	ForceWaveMultDiv						= GetPrivateProfileInt("DarkLord", "SCFForceWaveMultDiv", 100, FilePath) ;
	GetPrivateProfileString	("DarkLord","SCFForceWaveMultMax","4.0",Tmp,100,FilePath);
	ForceWaveMultMax = atof(Tmp);

	GetPrivateProfileString	("DarkLord","SCFAddCriticalDamageDiv1","25.0",Tmp,100,FilePath);
	AddCriticalDmgDiv1 = atof(Tmp);
	GetPrivateProfileString	("DarkLord","SCFAddCriticalDamageDiv2","30.0",Tmp,100,FilePath);
	AddCriticalDmgDiv2 = atof(Tmp);
	GetPrivateProfileString	("DarkLord","SCFAddCriticalDamageTimeDiv","10.0",Tmp,100,FilePath);
	AddCriticalDmgTimeDiv = atof(Tmp);
	AddCriticalDmgTimeBase = GetPrivateProfileInt("DarkLord", "SCFAddCriticalDamageTimeBase", 60, FilePath) ;

	//=======================================================================================
	SUOtherSkillBase						= GetPrivateProfileInt("Summoner", "SCFSUOtherSkillBase", 200, FilePath) ;
	SUOtherSkillEneDiv						= GetPrivateProfileInt("Summoner", "SCFSUOtherSkillEneDiv", 20, FilePath) ;
	SUOtherSkillMultDiv						= GetPrivateProfileInt("Summoner", "SCFSUOtherSkillMultDiv", 100, FilePath) ;
	GetPrivateProfileString	("Summoner","SCFSUOtherSkillMultMax","4.0",Tmp,100,FilePath);
	SUOtherSkillMultMax = atof(Tmp);

	GetPrivateProfileString	("Summoner","SCFReduceDmgMult","0.5",Tmp,100,FilePath);
	SkillReduceDamageMult = atof(Tmp);
	SkillReduceDamageDiv				= GetPrivateProfileInt("Summoner", "SCFReduceDmgDiv",100, FilePath) ;
	SkillReduceDamageTimeDiv			= GetPrivateProfileInt("Summoner", "SCFReduceDmgTimeDiv1",10, FilePath) ;
	SkillReduceDamageTimeBase			= GetPrivateProfileInt("Summoner", "SCFReduceDmgTimeBase",60, FilePath) ;

	SkillReduceDefenseDiv1				= GetPrivateProfileInt("Summoner", "SCFReduceDefenseDiv1",34, FilePath) ;
	SkillReduceDefenseDiv2				= GetPrivateProfileInt("Summoner", "SCFReduceDefenseDiv2",7, FilePath) ;
	SkillReduceDefenseTimeDiv			= GetPrivateProfileInt("Summoner", "SCFReduceDefenseTimeDiv",10, FilePath) ;
	SkillReduceDefenseTimeBase			= GetPrivateProfileInt("Summoner", "SCFReduceDefenseTimeBase",60, FilePath) ;

	SkillSleepTimeDiv					= GetPrivateProfileInt("Summoner", "SCFSleepTimeDiv1",100, FilePath) ;
	SkillSleepUseMaxTime				= GetPrivateProfileInt("Summoner", "SCFSleepUseMaxTime",0, FilePath) ;
	SkillSleepMaxTime					= GetPrivateProfileInt("Summoner", "SCFSleepMaxTime",100, FilePath) ;

	SkillReflectDiv						= GetPrivateProfileInt("Summoner", "SCFReflectDiv1",300, FilePath) ;
	SkillReflectTimeDiv					= GetPrivateProfileInt("Summoner", "SCFReflectTimeDiv1",10, FilePath) ;
	SkillReflectTimeBase				= GetPrivateProfileInt("Summoner", "SCFReflectTimeBase",60, FilePath) ;
	
	SkillBerserkerDefBase				= GetPrivateProfileInt("Summoner", "SCFBerserkerDefBase",56, FilePath) ;
	GetPrivateProfileString	("Summoner","SCFBerserkerDefDiv","50.0",Tmp,100,FilePath);
	SkillBerserkerDefDiv = atof(Tmp);	
	GetPrivateProfileString	("Summoner","SCFBerserkerAttackMult","0.00033",Tmp,100,FilePath);
	SkillBerserkerAttMult = atof(Tmp);
	GetPrivateProfileString	("Summoner","SCFBerserkerAttackMax","2.0",Tmp,100,FilePath);
	SkillBerserkerAttMaxMult = atof(Tmp);
	SkillBerserkerTimeDiv				= GetPrivateProfileInt("Summoner", "SCFBerserkerTimeDiv1",10, FilePath) ;
	SkillBerserkerTimeBase				= GetPrivateProfileInt("Summoner", "SCFBerserkerTimeBase",60, FilePath) ;
	
	SkillNightDiv						= GetPrivateProfileInt("Summoner", "SCFNightDiv1",100, FilePath) ;
	SkillNightMaxMonsterLevel			= GetPrivateProfileInt("Summoner", "SCFNightMaxMonsterLevel",100, FilePath) ;
	SkillNightTimeBase					= GetPrivateProfileInt("Summoner", "SCFNightTimeBase",30, FilePath) ;
	
	GetPrivateProfileString	("Summoner","SCFSahamutBuffDiv","20.0",Tmp,100,FilePath);
	SkillSummonerBook_SahamutDiv = atof(Tmp);
	SkillSummonerBook_SahamutDist		= GetPrivateProfileInt("Summoner", "SCFSahamutDistance", 3, FilePath) ;
	SkillSummonerBook_SahamutHit1		= GetPrivateProfileInt("Summoner", "SCFSahamutHitCount1", 10, FilePath) ;
	SkillSummonerBook_SahamutHit2		= GetPrivateProfileInt("Summoner", "SCFSahamutHitCount2", 5, FilePath) ;
	SkillSummonerBook_SahamutSec		= GetPrivateProfileInt("Summoner", "SCFSahamutSeconds", 7, FilePath) ;
	SkillSummonerBook_SahamutAddMagic	= GetPrivateProfileInt("Summoner", "SCFSahamutAddMagicDmgBase", 50, FilePath) ;

	GetPrivateProfileString	("Summoner","SCFNailBuffDiv","30.0",Tmp,100,FilePath);
	SkillSummonerBook_NailDiv = atof(Tmp);
	SkillSummonerBook_NailDist			= GetPrivateProfileInt("Summoner", "SCFNailDistance", 2, FilePath) ;
	SkillSummonerBook_NailHit1			= GetPrivateProfileInt("Summoner", "SCFNailHitCount1", 5, FilePath) ;
	SkillSummonerBook_NailHit2			= GetPrivateProfileInt("Summoner", "SCFNailHitCount2", 2, FilePath) ;
	SkillSummonerBook_NailSec			= GetPrivateProfileInt("Summoner", "SCFNailSeconds", 5, FilePath) ;
	SkillSummonerBook_NailAddMagic		= GetPrivateProfileInt("Summoner", "SCFNailAddMagicDmgBase", 25, FilePath) ;

	GetPrivateProfileString	("Summoner","SCFGhostPhantomBuffDiv","10.0",Tmp,100,FilePath);
	SkillGhostPhantom_Div = atof(Tmp);
	SkillGhostPhantom_Dist				= GetPrivateProfileInt("Summoner", "SCFGhostPhantomDistance", 7, FilePath) ;
	SkillGhostPhantom_Hit1				= GetPrivateProfileInt("Summoner", "SCFGhostPhantomHitCount1", 20, FilePath) ;
	SkillGhostPhantom_Hit2				= GetPrivateProfileInt("Summoner", "SCFGhostPhantomHitCount2", 10, FilePath) ;
	SkillGhostPhantom_Sec				= GetPrivateProfileInt("Summoner", "SCFGhostPhantomSeconds", 7, FilePath) ;
	SkillGhostPhantom_AddMagic			= GetPrivateProfileInt("Summoner", "SCFGhostPhantomAddMagicDmgBase", 75, FilePath) ;
	
	//=======================================================================================
	RFOtherSkillBase						= GetPrivateProfileInt("RageFighter", "SCFRFOtherSkillBase", 200, FilePath) ;
	RFOtherSkillEneDiv						= GetPrivateProfileInt("RageFighter", "SCFRFOtherSkillEneDiv", 20, FilePath) ;
	RFOtherSkillMultDiv						= GetPrivateProfileInt("RageFighter", "SCFRFOtherSkillMultDiv", 100, FilePath) ;
	GetPrivateProfileString	("RageFighter","SCFRFOtherSkillMultMin","2.0",Tmp,100,FilePath);
	RFOtherSkillMultMin = atof(Tmp);
	GetPrivateProfileString	("RageFighter","SCFRFOtherSkillMultMax","2.0",Tmp,100,FilePath);
	RFOtherSkillMultMax = atof(Tmp);

	GetPrivateProfileString	("RageFighter","SCFRFGlovesSkillAttackMultMax","3.0",Tmp,100,FilePath);
	RFGlovesSkillAttackMultMax	= atof(Tmp);

	RFGlovesSkillAttackMultDiv			= GetPrivateProfileInt("RageFighter", "SCFRFGlovesSkillAttackMultDiv",100, FilePath) ;
	RFGlovesSkillAttackVitDiv			= GetPrivateProfileInt("RageFighter", "SCFRFGlovesSkillAttackVitDiv",10, FilePath) ;
	RFGlovesSkillAttackVitBase			= GetPrivateProfileInt("RageFighter", "SCFRFGlovesSkillAttackVitBase",50, FilePath) ;

	GetPrivateProfileString	("RageFighter","SCFChainDriveMultMax","3.0",Tmp,100,FilePath);
	ChainDriveMultMax	= atof(Tmp);

	ChainDriveMultDiv					= GetPrivateProfileInt("RageFighter", "SCFChainDriveMultDiv",100, FilePath) ;
	ChainDriveVitDiv					= GetPrivateProfileInt("RageFighter", "SCFChainDriveVitDiv",10, FilePath) ;
	ChainDriveVitBase					= GetPrivateProfileInt("RageFighter", "SCFChainDriveVitBase",50, FilePath) ;

	GetPrivateProfileString	("RageFighter","SCFDarkSideParchmentMultMax","4.0",Tmp,100,FilePath);
	DarkSideParchmentMultMax	= atof(Tmp);
	
	DarkSideParchmentMultDiv			= GetPrivateProfileInt("RageFighter", "SCFDarkSideParchmentMultDiv",100, FilePath) ;
	DarkSideParchmentMultBase			= GetPrivateProfileInt("RageFighter", "SCFDarkSideParchmentMultBase",100, FilePath) ;
	DarkSideParchmentAgiDiv				= GetPrivateProfileInt("RageFighter", "SCFDarkSideParchmentAgiDiv",8, FilePath) ;
	DarkSideParchmentEneDiv				= GetPrivateProfileInt("RageFighter", "SCFDarkSideParchmentEneDiv",10, FilePath) ;
	DarkSideParchmentVitDiv				= GetPrivateProfileInt("RageFighter", "SCFDarkSideParchmentVitDiv",10, FilePath) ;

	GetPrivateProfileString	("RageFighter","SCFDragonLoreAttackMultMax","4.0",Tmp,100,FilePath);
	DragonLoreAttackMultMax	= atof(Tmp);

	DragonLoreAttackMultDiv				= GetPrivateProfileInt("RageFighter", "SCFDragonLoreAttackMultDiv",100, FilePath) ;
	DragonLoreEnergyDiv					= GetPrivateProfileInt("RageFighter", "SCFDragonLoreEnergyDiv",10, FilePath) ;
	DragonLoreEnergyBase				= GetPrivateProfileInt("RageFighter", "SCFDragonLoreEnergyBase",50, FilePath) ;

	GetPrivateProfileString	("RageFighter","SCFDragonSlayerIncSkillAttackMultMax","4.0",Tmp,100,FilePath);
	SkillDragonSlayerAttMax	= atof(Tmp);

	SkillDragonSlayerEneBase			= GetPrivateProfileInt("RageFighter", "SCFDragonSlayerEnergyBase",50, FilePath) ;
	SkillDragonSlayerEneDiv				= GetPrivateProfileInt("RageFighter", "SCFDragonSlayerEnergyDiv",10, FilePath) ;

	SkillDragonSlayerDmgBase			= GetPrivateProfileInt("RageFighter", "SCFDragonSlayerSkillDmgBase",300, FilePath) ;
	SkillDragonSlayerDmgDiv				= GetPrivateProfileInt("RageFighter", "SCFDragonSlayerSkillDmgDiv",100, FilePath) ;
	SkillDragonSlayerDmgMult			= GetPrivateProfileInt("RageFighter", "SCFDragonSlayerSkillDmgMult",3, FilePath) ;

	SkillDragonSlayerTimeDiv			= GetPrivateProfileInt("RageFighter", "SCFDragonSlayerIncSkillAttackTimeDiv",5, FilePath) ;
	SkillDragonSlayerTimeBase			= GetPrivateProfileInt("RageFighter", "SCFDragonSlayerIncSkillAttackTimeBase",60, FilePath) ;
	SkillDragonSlayerTimeMinMax			= GetPrivateProfileInt("RageFighter", "SCFDragonSlayerIncSkillAttackTimeMaxMin",10, FilePath) ;

	SkillRedArmorIgnoreBase				= GetPrivateProfileInt("RageFighter", "SCFRedArmorIgnoreBase",5, FilePath) ;
	SkillRedArmorIgnoreEneSubtract		= GetPrivateProfileInt("RageFighter", "SCFRedArmorIgnoreEneSubtract",120, FilePath) ;
	SkillRedArmorIgnoreDivider			= GetPrivateProfileInt("RageFighter", "SCFRedArmorIgnoreDivider",10, FilePath) ;
	
	SkillRedArmorIgnoreTimeDiv			= GetPrivateProfileInt("RageFighter", "SCFRedArmorIgnoreTimeDiv",5, FilePath) ;
	SkillRedArmorIgnoreTimeBase			= GetPrivateProfileInt("RageFighter", "SCFRedArmorIgnoreTimeBase",60, FilePath) ;
	SkillRedArmorIgnoreMinMax			= GetPrivateProfileInt("RageFighter", "SCFRedArmorIgnoreTimeMaxMin",10, FilePath) ;

	IgnorePercent						= GetPrivateProfileInt("RageFighter", "SCFRedArmorIgnoreMaxPercent",50, FilePath);
	if (IgnorePercent > 100)
		SkillRedArmorIgnoreMaxPercent	= 100;
	else
		SkillRedArmorIgnoreMaxPercent	= IgnorePercent ;

	SkillFitnessTimeDiv					= GetPrivateProfileInt("RageFighter", "SCFFitnessTimeDiv",5, FilePath) ;
	SkillFitnessTimeBase				= GetPrivateProfileInt("RageFighter", "SCFFitnessTimeBase",60, FilePath) ;
	SkillFitnessTimeMinMax				= GetPrivateProfileInt("RageFighter", "SCFFitnessTimeMaxMin",10, FilePath) ;

	SkillFitnessVitBase					= GetPrivateProfileInt("RageFighter", "SCFFitnessVitBase",30, FilePath) ;
	SkillFitnessEneSubtract				= GetPrivateProfileInt("RageFighter", "SCFFitnessEneSubtract",132, FilePath) ;
	SkillFitnessVitDiv					= GetPrivateProfileInt("RageFighter", "SCFFitnessVitDiv",10, FilePath) ;
	SkillFitnessVitAddMax				= GetPrivateProfileInt("RageFighter", "SCFFitnessVitAddMax",200, FilePath) ;

	SkillDefSuccessRateIncBase			= GetPrivateProfileInt("RageFighter", "SCFDefSuccessRateIncBase",10, FilePath) ;
	SkillDefSuccessRateIncEneSubtract	= GetPrivateProfileInt("RageFighter", "SCFDefSuccessRateIncEneSubtract",80, FilePath) ;
	SkillDefSuccessRateIncDivider		= GetPrivateProfileInt("RageFighter", "SCFDefSuccessRateIncDivider",10, FilePath) ;
	
	SkillDefSuccessRateIncTimeDiv		= GetPrivateProfileInt("RageFighter", "SCFDefSuccessRateIncTimeDiv",5, FilePath) ;
	SkillDefSuccessRateIncTimeBase		= GetPrivateProfileInt("RageFighter", "SCFDefSuccessRateIncTimeBase",60, FilePath) ;
	SkillDefSuccessRateIncMinMax		= GetPrivateProfileInt("RageFighter", "SCFDefSuccessRateIncTimeMaxMin",10, FilePath) ;

	IgnorePercent = GetPrivateProfileInt("RageFighter", "SCFDefSuccessRateIncMaxPercent",100, FilePath);
	if (IgnorePercent > 100)
		SkillDefSuccessRateIncMaxPercent= 100;
	else
		SkillDefSuccessRateIncMaxPercent= IgnorePercent ;

	//=======================================================================================
	FernirDKStrDiv	= GetPrivateProfileInt("FernirPlasmaAttack", "SCFFernirDKStrDiv", 3, FilePath)&255;
	FernirMGStrDiv	= GetPrivateProfileInt("FernirPlasmaAttack", "SCFFernirMGStrDiv", 3, FilePath)&255;
	FernirRFStrDiv 	= GetPrivateProfileInt("FernirPlasmaAttack", "SCFFernirRFStrDiv", 3, FilePath)&255;
	FernirDWStrDiv 	= GetPrivateProfileInt("FernirPlasmaAttack", "SCFFernirDWStrDiv", 5, FilePath)&255;
	FernirSUStrDiv 	= GetPrivateProfileInt("FernirPlasmaAttack", "SCFFernirSUStrDiv", 5, FilePath)&255;
	FernirDLStrDiv 	= GetPrivateProfileInt("FernirPlasmaAttack", "SCFFernirDLStrDiv", 5, FilePath)&255;
	FernirElfStrDiv	= GetPrivateProfileInt("FernirPlasmaAttack", "SCFFernirElfStrDiv",5, FilePath)&255;

	FernirDKAgiDiv 	= GetPrivateProfileInt("FernirPlasmaAttack", "SCFFernirDKAgiDiv", 5, FilePath)&255;
	FernirMGAgiDiv 	= GetPrivateProfileInt("FernirPlasmaAttack", "SCFFernirMGAgiDiv", 5, FilePath)&255;
	FernirRFAgiDiv 	= GetPrivateProfileInt("FernirPlasmaAttack", "SCFFernirRFAgiDiv", 5, FilePath)&255;
	FernirDWAgiDiv 	= GetPrivateProfileInt("FernirPlasmaAttack", "SCFFernirDWAgiDiv", 5, FilePath)&255;
	FernirSUAgiDiv 	= GetPrivateProfileInt("FernirPlasmaAttack", "SCFFernirSUAgiDiv", 5, FilePath)&255;
	FernirDLAgiDiv 	= GetPrivateProfileInt("FernirPlasmaAttack", "SCFFernirDLAgiDiv", 5, FilePath)&255;
	FernirElfAgiDiv	= GetPrivateProfileInt("FernirPlasmaAttack", "SCFFernirElfAgiDiv",3, FilePath)&255;

	FernirDKVitDiv 	= GetPrivateProfileInt("FernirPlasmaAttack", "SCFFernirDKVitDiv", 5, FilePath)&255;
	FernirMGVitDiv 	= GetPrivateProfileInt("FernirPlasmaAttack", "SCFFernirMGVitDiv", 5, FilePath)&255;
	FernirRFVitDiv 	= GetPrivateProfileInt("FernirPlasmaAttack", "SCFFernirRFVitDiv", 5, FilePath)&255;
	FernirDWVitDiv  = GetPrivateProfileInt("FernirPlasmaAttack", "SCFFernirDWVitDiv", 7, FilePath)&255;
	FernirSUVitDiv 	= GetPrivateProfileInt("FernirPlasmaAttack", "SCFFernirSUVitDiv", 7, FilePath)&255;
	FernirDLVitDiv 	= GetPrivateProfileInt("FernirPlasmaAttack", "SCFFernirDLVitDiv", 7, FilePath)&255;
	FernirElfVitDiv	= GetPrivateProfileInt("FernirPlasmaAttack", "SCFFernirElfVitDiv",7, FilePath)&255;

	FernirDKEneDiv 	= GetPrivateProfileInt("FernirPlasmaAttack", "SCFFernirDKEneDiv", 7, FilePath)&255;
	FernirMGEneDiv 	= GetPrivateProfileInt("FernirPlasmaAttack", "SCFFernirMGEneDiv", 7, FilePath)&255;
	FernirRFEneDiv 	= GetPrivateProfileInt("FernirPlasmaAttack", "SCFFernirRFEneDiv", 7, FilePath)&255;
	FernirDWEneDiv 	= GetPrivateProfileInt("FernirPlasmaAttack", "SCFFernirDWEneDiv", 3, FilePath)&255;
	FernirSUEneDiv 	= GetPrivateProfileInt("FernirPlasmaAttack", "SCFFernirSUEneDiv", 3, FilePath)&255;
	FernirDLEneDiv 	= GetPrivateProfileInt("FernirPlasmaAttack", "SCFFernirDLEneDiv", 3, FilePath)&255;
	FernirElfEneDiv = GetPrivateProfileInt("FernirPlasmaAttack", "SCFFernirElfEneDiv",5, FilePath)&255;

	FernirDLCmdDiv 	= GetPrivateProfileInt("FernirPlasmaAttack", "SCFFernirDLCmdDiv", 3, FilePath)&255;

	FernirDmgBase 	= GetPrivateProfileInt("FernirPlasmaAttack", "SCFFernirLvlBase", 300, FilePath);
	FernirDmgBaseDiv= GetPrivateProfileInt("FernirPlasmaAttack", "SCFFernirLvlBaseDiv", 5, FilePath);
	FernirDmgAdd 	= GetPrivateProfileInt("FernirPlasmaAttack", "SCFFernirDmgAdd", 200, FilePath);
	FernirDmgTDiv 	= GetPrivateProfileInt("FernirPlasmaAttack", "SCFFernirDmgTDiv", 100, FilePath);

	LogAddTD("[SCF Skills] - %s file is Loaded",FilePath);

	#if (WL_PROTECT==1)
		int MyCheckVar;   
		VM_START_WITHLEVEL(20)
			if(WLRegGetStatus(NULL) != 1)
			{
				SkillNightDiv = 0;
				SkillReflectTimeDiv = 0;
				SkillSleepTimeDiv = 0;
				SkillReduceDamageDiv = 0;
			}
		VM_END
	#endif
}



BOOL CReadConfig::INVENTORY_RANGE(int aIndex, BYTE Pos, bool Max)
{
//#define INVENTORY_RANGE(x) (((x)<0)?FALSE:((x)>INVENTORY_SIZE-1)?FALSE:TRUE )
	if(Pos < 0)
		return FALSE;
	if(Pos > this->INVENTORY_SIZE(aIndex,Max) - 1)
		return FALSE;

	return TRUE;
}

BYTE CReadConfig::INVENTORY_SIZE(int aIndex, bool Max)
{
	if (aIndex < OBJ_STARTUSERINDEX || aIndex > OBJMAX)
#if (PACK_EDITION>=3)
		if (gObj[aIndex].IsBot != 4)
#endif
	{
			return INVENTORY_NORMAL_SIZE;
	}

	if(this->S6E2 == 1)
	{
		return (MAIN_INVENTORY_NORMAL_SIZE + (MAX_EXTENDED_INV*EXTENDED_INV_SIZE) + PSHOP_SIZE );
	}
	return INVENTORY_NORMAL_SIZE;
}

BYTE CReadConfig::INVENTORY_MAP_SIZE(int aIndex, bool Max)
{
	if (aIndex < OBJ_STARTUSERINDEX || aIndex > OBJMAX)
		return this->MAIN_INVENTORY_SIZE(aIndex,Max) - INVETORY_WEAR_SIZE + PSHOP_SIZE;

	//if(this->S6E2 == 1)
	//	return this->MAIN_INVENTORY_SIZE(aIndex,Max) - INVETORY_WEAR_SIZE + PSHOP_SIZE + EXTENDED_INV_SIZE*MAX_EXTENDED_INV;
	//Main Inventory Calculates Extended Size Already, readding it, will cause errors
	return this->MAIN_INVENTORY_SIZE(aIndex,Max) - INVETORY_WEAR_SIZE + PSHOP_SIZE;
}

BYTE CReadConfig::GETSLOTS(int aIndex)
{
	if(OBJMAX_RANGE(aIndex) == false)
		return BASIC_INVENTORY_SIZE;

	if (aIndex < OBJ_STARTUSERINDEX || aIndex > OBJMAX)
		return BASIC_INVENTORY_SIZE;

	if(this->S6E2 == 1)
	{
		if(gObj[aIndex].ExInventory >= 1 && gObj[aIndex].ExInventory <= MAX_EXTENDED_INV)
			return BASIC_INVENTORY_SIZE + (gObj[aIndex].ExInventory * EXTENDED_INV_SIZE);
	}

	return BASIC_INVENTORY_SIZE;
}

BYTE CReadConfig::GETHEIGHT(int aIndex)
{
	if(OBJMAX_RANGE(aIndex) == false)
		return BASIC_INV_HEIGHT;

	if (aIndex < OBJ_STARTUSERINDEX || aIndex > OBJMAX)
		return BASIC_INV_HEIGHT;

	if(this->S6E2 == 1)
	{
		if(gObj[aIndex].ExInventory >= 1 && gObj[aIndex].ExInventory <= MAX_EXTENDED_INV)
			return BASIC_INV_HEIGHT + (EXTENDED_INV_HEIGHT*gObj[aIndex].ExInventory); 
		//if(gObj[aIndex].ExInventory == 1)
		//	return 12;
		//if(gObj[aIndex].ExInventory == 2)
		//	return 16;
	}
	return BASIC_INV_HEIGHT;
}
BOOL CReadConfig::IS_EXTENDED_INV(int aIndex, BYTE INVNUM)
{
	if(OBJMAX_RANGE(aIndex) == false)
		return FALSE;

	if (aIndex < OBJ_STARTUSERINDEX || aIndex > OBJMAX)
		return FALSE;

	if(this->S6E2 == 1)
	{
		if(gObj[aIndex].ExInventory >= 1 && INVNUM == 1)
			return TRUE;
		else if(gObj[aIndex].ExInventory == 2 && INVNUM == 2)
			return TRUE;
		else if(gObj[aIndex].ExInventory == 3 && INVNUM == 3)
			return TRUE;
		else if(gObj[aIndex].ExInventory == 4 && INVNUM == 4)
			return TRUE;
	}
	return FALSE;
}

BYTE CReadConfig::MAIN_INVENTORY_SIZE(int aIndex, bool Max)
{		
	if(OBJMAX_RANGE(aIndex) == false)
		return MAIN_INVENTORY_NORMAL_SIZE;

	if (aIndex < OBJ_STARTUSERINDEX || aIndex > OBJMAX)
	{
#if (PACK_EDITION>=3)
		if (gObj[aIndex].IsBot != 4)
#endif
			return MAIN_INVENTORY_NORMAL_SIZE;
	}

	if(Max == false)
	{
		if(this->S6E2 == 1)
		{
			if(gObj[aIndex].ExInventory > 0 && gObj[aIndex].ExInventory <= MAX_EXTENDED_INV )
				return MAIN_INVENTORY_NORMAL_SIZE + (EXTENDED_INV_SIZE*gObj[aIndex].ExInventory);
		}
		return MAIN_INVENTORY_NORMAL_SIZE;
	}

	if(this->S6E2 == 1)
		return MAIN_INVENTORY_NORMAL_SIZE + (EXTENDED_INV_SIZE*MAX_EXTENDED_INV);
	
	return MAIN_INVENTORY_NORMAL_SIZE;
}

BYTE CReadConfig::WAREHOUSE_SIZE(int aIndex, bool Max)
{
	if(OBJMAX_RANGE(aIndex) == false)
		return WAREHOUSE_NORMAL_SIZE;

	if (aIndex < OBJ_STARTUSERINDEX || aIndex > OBJMAX)
		return WAREHOUSE_NORMAL_SIZE;

	if(Max == false)
	{
		if(this->S6E2 == 1)
		{
			if(gObj[aIndex].ExWarehouse == 1)
				return WAREHOUSE_S6E2_SIZE;
		}
		return WAREHOUSE_NORMAL_SIZE;
	}

	if(this->S6E2 == 1)
		return WAREHOUSE_S6E2_SIZE;
	
	return WAREHOUSE_NORMAL_SIZE;
}

BOOL CReadConfig::MAIN_INVENTORY_RANGE(int aIndex, BYTE Pos, bool Max)
{
	//#define MAIN_INVENTORY_NORMAL_RANGE(x) (((x)<0)?FALSE:((x)>MAIN_INVENTORY_NORMAL_SIZE-1)?FALSE:TRUE )

	if(Pos < 0)
		return FALSE;

	if(Pos > this->MAIN_INVENTORY_SIZE(aIndex,Max) -1 )
		return FALSE;

	return TRUE;
}

BOOL CReadConfig::PSHOP_RANGE(int aIndex, BYTE Pos, bool Max)
{
	//#define PSHOP_RANGE(x) ( (((x)-MAIN_INVENTORY_NORMAL_SIZE)<0)?FALSE:(((x)-MAIN_INVENTORY_NORMAL_SIZE)>(INVENTORY_SIZE-MAIN_INVENTORY_NORMAL_SIZE-1))?FALSE:TRUE )

	if(Pos - this->MAIN_INVENTORY_SIZE(aIndex,Max) < 0)
		return FALSE;

	if(Pos - this->MAIN_INVENTORY_SIZE(aIndex,Max) > (this->INVENTORY_SIZE(aIndex,Max)-this->MAIN_INVENTORY_SIZE(aIndex,Max)-1))
		return FALSE;

	return TRUE;
}

BOOL CReadConfig::WAREHOUSE_RANGE(int aIndex, BYTE Pos, bool Max)
{
	if(OBJMAX_RANGE(aIndex) == false)
		return WAREHOUSE_NORMAL_RANGE(Pos);

	if (aIndex < OBJ_STARTUSERINDEX || aIndex > OBJMAX)
		return WAREHOUSE_NORMAL_RANGE(Pos);

	if(Max == false)
	{
		if(this->S6E2 == 1)
		{
			if(gObj[aIndex].ExWarehouse == 1)
				return WAREHOUSE_S6E2_RANGE(Pos);
		}
		return WAREHOUSE_NORMAL_RANGE(Pos);
	}

	if(this->S6E2 == 1)
		return WAREHOUSE_S6E2_RANGE(Pos);
	
	return WAREHOUSE_NORMAL_RANGE(Pos);
}