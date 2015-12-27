#define MAX_MAP_NUMBER			82
#define MAX_QUEST_COUNT			100
#define MAX_QUEST_REQ			75

extern char FilePathSpawn[];
#define CONNDATAFILES_NUM		50
#define EVENTITEMBAG_NUM		84
#define MAX_SHOP				29


#define MUITEMSHOP_MAXITEMPERPAGE		9
#define MUITEMSHOP_MAXPAGES				5

#define RANKINGINI				"..\\SCFData\\SCF_Ranking.ini"

class CReadConfig
{
public:
	void Init();
	void ReadConnections(char * FilePath);
	void Connect(char * FilePath,BYTE State);
	void Skill(char * FilePath);
	void Common(char * FilePath);
	void Ranking();
	void Character(char * FilePath);
	void Commands(char * FilePath);
	void Items(char * FilePath);
	void ChaosMachine(char * FilePath);
	void ExtraExp(char * FilePath);
	void Warehouse(char * FilePath);
	void SocketOptions(char * FilePath,char * FilePath2);
	BYTE WAREHOUSE_SIZE(int aIndex, bool Max);
	BOOL WAREHOUSE_RANGE(int aIndex, BYTE Pos, bool Max);

	BYTE GETSLOTS(int aIndex);
	BYTE GETHEIGHT(int aIndex);
	BOOL IS_EXTENDED_INV(int aIndex, BYTE INVNUM);
	BYTE INVENTORY_SIZE(int aIndex, bool Max);
	BOOL INVENTORY_RANGE(int aIndex, BYTE Pos, bool Max);
	BYTE INVENTORY_MAP_SIZE(int aIndex, bool Max);
	BYTE MAIN_INVENTORY_SIZE(int aIndex, bool Max);
	BOOL MAIN_INVENTORY_RANGE(int aIndex, BYTE Pos, bool Max);
	BOOL PSHOP_RANGE(int aIndex, BYTE Pos, bool Max);

	//
	
//WORD WAREHOUSE_NEW_SIZE;
	WORD MAIN_INVENTORY_NEW_SIZE;
	WORD INVENTORY_MAP_NEW_SIZE;

//SocketOptions var:
	
	BYTE SocketDropRate[MAX_MAP_NUMBER];
	BYTE SocketOptValue[256];

//Warehouse vars:
	int WareExVipValue;
	int WareExLimit;
	BYTE WareSystem;
	BYTE Ware3Secs;
	BOOL UseGuildWare;
	int GuildWareMap;
	BYTE GuildWareOnlyExt;

//ExtraExp vars:
	BYTE ExpSys;
	BOOL ShowExp;
	int MapExtraExpSingle[MAX_MAP_NUMBER];
	int MapExtraExpParty[MAX_MAP_NUMBER];
	int ExpDayofWeek[7];
	int ExpDay;

	BYTE IsResetExtraExp;
	int ResetExtraExp[3];
	int ResetNrExtraExp[3];

	BYTE IsResetPenaltyExp;
	float PenaltyExpPerReset;

	BYTE IsVipExtraExp;
	int VIPExtraExp[3];
	int VIPDaysExtraExp[3];

//Connect Vars:
	int UDPPort;
	char SCFEXSIP[20];
	int SCFEXSPort;
	char SCFPSIP[20];
	BOOL SCFRSON;
	BOOL SCFESON;
	BOOL SCFPSON;

	BOOL SCFIOGSON;
	char SCFIOGSIP[20];
	int SCFIOGSPort;

	int GSCSSyncTime;
	int SCFPSPort;
	BOOL isCheckSum;
	char ConnDataFiles[CONNDATAFILES_NUM][256];
	char ConnShopDataFiles[MAX_SHOP][256];
	char ConnEventDropDataFiles[EVENTITEMBAG_NUM][256];
	char ConnectNotice[512];
	BOOL AHLog;
	BOOL PostLog;
	BOOL MSGLog;
	BOOL PMLog;
	BOOL DropLog;
	BOOL TradeLog;
	BOOL GMLog;

//Skill vars:
	BOOL DisableLevelReq_gObjMagicEnergyCheck;
	BYTE TwistingSlashNoWeapon_CGDurationMagicRecv;
	BYTE ResetComboBelow220Level;

	BYTE FernirDKStrDiv; 
	BYTE FernirMGStrDiv; 
	BYTE FernirRFStrDiv; 
	BYTE FernirDWStrDiv; 
	BYTE FernirSUStrDiv; 
	BYTE FernirDLStrDiv; 
	BYTE FernirElfStrDiv; 

	BYTE FernirDKAgiDiv; 
	BYTE FernirMGAgiDiv; 
	BYTE FernirRFAgiDiv; 
	BYTE FernirDWAgiDiv; 
	BYTE FernirSUAgiDiv; 
	BYTE FernirDLAgiDiv; 
	BYTE FernirElfAgiDiv; 

	BYTE FernirDKVitDiv; 
	BYTE FernirMGVitDiv; 
	BYTE FernirRFVitDiv; 
	BYTE FernirDWVitDiv; 
	BYTE FernirSUVitDiv; 
	BYTE FernirDLVitDiv; 
	BYTE FernirElfVitDiv; 

	BYTE FernirDKEneDiv; 
	BYTE FernirMGEneDiv; 
	BYTE FernirRFEneDiv; 
	BYTE FernirDWEneDiv; 
	BYTE FernirSUEneDiv; 
	BYTE FernirDLEneDiv; 
	BYTE FernirElfEneDiv; 

	BYTE FernirDLCmdDiv;

	short FernirDmgBase;
	short FernirDmgBaseDiv;
	short FernirDmgAdd;
	short FernirDmgTDiv;

	short MGTwistingSlashBase;
	short MGTwistingSlashEneDiv;
	short MGTwistingSlashMultDiv;
	float MGTwistingSlashMultMin;
	float MGTwistingSlashMultMax;

	short MGMagicDefenseBase;
	short MGMagicDefenseDiv1;
	short MGMagicDefenseDiv2;
	BYTE MGMagicMaximumDefense;
	short MGMagicDefenseTimeLimit;
	short MGMagicDefenseTimeBase;
	short MGMagicDefenseTimeDiv1;

	short SpiralSlashBase;
	short SpiralSlashEneDiv;
	short SpiralSlashMultDiv;
	float SpiralSlashMultMin;
	float SpiralSlashMultMax;

	short FireSlashBase;
	short FireSlashEneDiv;
	short FireSlashMultDiv;
	float FireSlashMultMin;
	float FireSlashMultMax;

	short PowerSlashBase;
	short PowerSlashEneDiv;
	short PowerSlashMultDiv;
	float PowerSlashMultMin;
	float PowerSlashMultMax;

	short WizardMagicDefenseBase;
	short WizardMagicDefenseDiv1;
	short WizardMagicDefenseDiv2;
	BYTE WizardMagicMaximumDefense;
	short WizardMagicDefenseTimeLimit;
	short WizardMagicDefenseTimeBase;
	short WizardMagicDefenseTimeDiv1;

	float MagicCircleMult;
	short MagicCircleMinDur;
	short MagicCircleDurDiv;

	short NovaBase;
	short NovaEneDiv;
	short NovaMultDiv;
	float NovaMultMin;
	float NovaMultMax;

	short ComboDivisor;

	short KnightSkillAddLifeBase;
	short KnightSkillAddLifeDivVit;
	short KnightSkillAddLifeDivEne;
	short KnightSkillAddLifeDivGral;
	short KnightSkillAddTimeDivGral;
	short KnightSkillAddLifeTimeBase;
	short KnightSkillAddLifeMaxTimeBase;
	BYTE KnightMaxMultiplierHP;

	short DKTwistingSlashBase;
	short DKTwistingSlashEneDiv;
	short DKTwistingSlashMultDiv;
	float DKTwistingSlashMultMax;

	short CrescentSlashBase;
	short CrescentSlashEneDiv;
	short CrescentSlashMultDiv;
	float CrescentSlashMultMax;

	short DKExplotionBase;
	short DKExplotionEneDiv;
	short DKExplotionMultDiv;
	float DKExplotionMultMax;

	short DeathStabBase;
	short DeathStabEneDiv;
	short DeathStabMultDiv;
	float DeathStabMultMax;

	short BlowOfFuryBase;
	short BlowOfFuryEneDiv;
	short BlowOfFuryMultDiv;
	float BlowOfFuryMultMax;

	short SkillMonsterCall1;
	short SkillMonsterCall2;
	short SkillMonsterCall3;
	short SkillMonsterCall4;
	short SkillMonsterCall5;
	short SkillMonsterCall6;
	short SkillMonsterCall7;

	short ElfBuffAttackLvlMult;
	float ElfBuffAttackAddPlus;
	short ElfBuffAttackDiv;
	short ElfBuffAttackTime;
	short ElfBuffAttackTimeEneDiv;
	short ElfBuffAttackTimeMax;

	short ElfBuffDefenseTime;
	short ElfBuffDefenseTimeEneDiv;
	short ElfBuffDefenseDiv;
	float ElfBuffDefenseAddPlus;
	short ElfBuffDefenseLvlMult;
	short ElfBuffDefenseTimeMax;

	short ElfBuffHealingLvl;
	short ElfBuffHealingDiv;

	short ElfBuffReduceShieldLvl;
	short ElfBuffReduceShieldDiv;

	short StarFallBase;
	short StarFallEneDiv;
	short StarFallMultDiv;
	float StarFallMultMin;
	float StarFallMultMax;

	short IceArrowBase;
	short IceArrowEneDiv;
	short IceArrowMultDiv;
	float IceArrowMultMin;
	float IceArrowMultMax;

	short PenetrationBase;
	short PenetrationEneDiv;
	short PenetrationMultDiv;
	float PenetrationMultMin;
	float PenetrationMultMax;

	short FiveShotBase;
	short FiveShotEneDiv;
	short FiveShotMultDiv;
	float FiveShotMultMin;
	float FiveShotMultMax;


	//==========================================
	short DLAddSkillDmgStrDiv;
	short DLAddSkillDmgEneDiv;

	BYTE IsSkillFireBurstStr;
	short FireBurstBase;
	short FireBurstEneDiv;
	short FireBurstMultDiv;
	float FireBurstMultMax;

	short FireScreamBase;
	short FireScreamEneDiv;
	short FireScreamMultDiv;
	float FireScreamMultMax;

	short FireBlastBase;
	short FireBlastEneDiv;
	short FireBlastMultDiv;
	float FireBlastMultMax;

	BYTE DarkHorseMovePlayerInCS;
	short DarkHorseBase;
	short DarkHorseEneDiv;
	short DarkHorseMultDiv;
	float DarkHorseMultMax;

	short DarkHorseDmgStrDiv;
	short DarkHorseDmgCmdDiv;
	short DarkHorseDmgLvlMul;

	short ElectricSparkBase;
	short ElectricSparkEneDiv;
	short ElectricSparkMultDiv;
	float ElectricSparkMultMax;
	short ElectricSparkDmgCmdDiv;
	short ElectricSparkDmgPartyMul;

	short BirdsBase;
	short BirdsEneDiv;
	short BirdsMultDiv;
	float BirdsMultMax;

	short ForceBase;
	short ForceEneDiv;
	short ForceMultDiv;
	float ForceMultMax;

	short ForceWaveBase;
	short ForceWaveEneDiv;
	short ForceWaveMultDiv;
	float ForceWaveMultMax;

	float AddCriticalDmgDiv1;
	float AddCriticalDmgDiv2;
	float AddCriticalDmgTimeDiv;
	int AddCriticalDmgTimeBase;

	short SkillReflectDiv;
	short SkillReflectTimeDiv;
	short SkillReflectTimeBase;

	short SkillBerserkerDefBase;
	float SkillBerserkerDefDiv;
	float SkillBerserkerAttMult;
	float SkillBerserkerAttMaxMult;
	short SkillBerserkerTimeDiv;
	short SkillBerserkerTimeBase;

	short SkillReduceDamageDiv;
	float SkillReduceDamageMult;
	short SkillReduceDamageTimeDiv;
	short SkillReduceDamageTimeBase;

	short SkillReduceDefenseDiv1;
	short SkillReduceDefenseDiv2;
	short SkillReduceDefenseTimeDiv;
	short SkillReduceDefenseTimeBase;

	short SkillSleepTimeDiv;
	BYTE  SkillSleepUseMaxTime;
	short SkillSleepMaxTime;

	short SkillNightDiv;
	short SkillNightTimeBase;
	short SkillNightMaxMonsterLevel;

	float SkillSummonerBook_SahamutDiv;
	BYTE SkillSummonerBook_SahamutDist;
	BYTE SkillSummonerBook_SahamutHit1;
	BYTE SkillSummonerBook_SahamutHit2;
	BYTE SkillSummonerBook_SahamutSec;
	BYTE SkillSummonerBook_SahamutAddMagic;

	float SkillSummonerBook_NailDiv;
	BYTE SkillSummonerBook_NailDist;
	BYTE SkillSummonerBook_NailHit1;
	BYTE SkillSummonerBook_NailHit2;
	BYTE SkillSummonerBook_NailSec;
	BYTE SkillSummonerBook_NailAddMagic;

	float SkillGhostPhantom_Div;
	BYTE SkillGhostPhantom_Dist;
	BYTE SkillGhostPhantom_Hit1;
	BYTE SkillGhostPhantom_Hit2;
	BYTE SkillGhostPhantom_Sec;
	BYTE SkillGhostPhantom_AddMagic;

	//=========================================================
	short RFOtherSkillBase;
	short RFOtherSkillEneDiv;
	short RFOtherSkillMultDiv;
	float RFOtherSkillMultMin;
	float RFOtherSkillMultMax;

	short MGOtherSkillBase;
	short MGOtherSkillEneDiv;
	short MGOtherSkillMultDiv;
	float MGOtherSkillMultMin;
	float MGOtherSkillMultMax;

	short DLOtherSkillBase;
	short DLOtherSkillEneDiv;
	short DLOtherSkillMultDiv;
	float DLOtherSkillMultMin;
	float DLOtherSkillMultMax;

	short DKOtherSkillBase;
	short DKOtherSkillEneDiv;
	short DKOtherSkillMultDiv;
	float DKOtherSkillMultMax;

	short DWOtherSkillBase;
	short DWOtherSkillEneDiv;
	short DWOtherSkillMultDiv;
	float DWOtherSkillMultMax;

	short ElfOtherSkillBase;
	short ElfOtherSkillEneDiv;
	short ElfOtherSkillMultDiv;
	float ElfOtherSkillMultMax;

	short SUOtherSkillBase;
	short SUOtherSkillEneDiv;
	short SUOtherSkillMultDiv;
	float SUOtherSkillMultMax;
	//=========================================================
	float RFGlovesSkillAttackMultMax;

	short RFGlovesSkillAttackMultDiv;
	short RFGlovesSkillAttackVitDiv;
	short RFGlovesSkillAttackVitBase;

	//=========================================================
	float ChainDriveMultMax;

	short ChainDriveMultDiv;
	short ChainDriveVitDiv;
	short ChainDriveVitBase;

	//=========================================================
	float DarkSideParchmentMultMax;
	
	short DarkSideParchmentMultDiv;
	short DarkSideParchmentMultBase;
	short DarkSideParchmentAgiDiv;
	short DarkSideParchmentEneDiv;
	short DarkSideParchmentVitDiv;

	//=========================================================
	float DragonLoreAttackMultMax;

	short DragonLoreAttackMultDiv;
	short DragonLoreEnergyDiv;
	short DragonLoreEnergyBase;

	//=========================================================
	float SkillDragonSlayerAttMax;

	short SkillDragonSlayerEneBase;
	short SkillDragonSlayerEneDiv;

	short SkillDragonSlayerDmgBase;
	short SkillDragonSlayerDmgDiv;
	short SkillDragonSlayerDmgMult;

	short SkillDragonSlayerTimeDiv;
	short SkillDragonSlayerTimeBase;
	short SkillDragonSlayerTimeMinMax;

	//=========================================================
	short SkillRedArmorIgnoreTimeDiv;
	short SkillRedArmorIgnoreTimeBase;
	short SkillRedArmorIgnoreMinMax;

	short SkillRedArmorIgnoreBase;
	short SkillRedArmorIgnoreEneSubtract;
	short SkillRedArmorIgnoreDivider;
	BYTE SkillRedArmorIgnoreMaxPercent;

	//=========================================================
	short SkillFitnessTimeDiv;
	short SkillFitnessTimeBase;
	BYTE SkillFitnessTimeMinMax;

	short SkillFitnessVitBase;
	short SkillFitnessEneSubtract;
	short SkillFitnessVitDiv;
	short SkillFitnessVitAddMax;

	//=========================================================
	short SkillDefSuccessRateIncBase;
	short SkillDefSuccessRateIncEneSubtract;
	short SkillDefSuccessRateIncDivider;
	short SkillDefSuccessRateIncMaxPercent;

	short SkillDefSuccessRateIncTimeDiv;
	short SkillDefSuccessRateIncTimeBase;
	BYTE SkillDefSuccessRateIncMinMax;

//Ranking vars
	BYTE Ranking_Monsters_GoldenGoblin;
	BYTE Ranking_Monsters_GoldenTitan;
	BYTE Ranking_Monsters_GoldenDerkon;
	BYTE Ranking_Monsters_GoldenLizardKing;
	BYTE Ranking_Monsters_GoldenTantalos;
	BYTE Ranking_Monsters_GoldenDarkKnight;
	BYTE Ranking_Monsters_GoldenDarkDevil;
	BYTE Ranking_Monsters_GoldenStoneGolem;
	BYTE Ranking_Monsters_GoldenCrust;
	BYTE Ranking_Monsters_GoldenSatyros;
	BYTE Ranking_Monsters_GoldenTwinTale;
	BYTE Ranking_Monsters_GoldenIronKnight;
	BYTE Ranking_Monsters_GoldenNapin;
	BYTE Ranking_Monsters_GoldenGreatDragon;

//Common Vars
	BOOL S5E2;
	BOOL S6E1;
	BOOL S6E2;
	BOOL IsEngProtocol;
	BOOL IsRecoverPShop;
	BOOL IsRecoverMPoints;
	BYTE CSFix_Same_Team_FIX;
	BYTE CSFix_OnConnect_Warp_FIX;
	BOOL ResponErrorCheck;
	int ResponErrorTimePlaying;
	int ResponErrorTimeConnected;

	short MadicGladiatorLvlReq;
	short DarkLordLvlReq;
	short RageFighterLvlReq;
	short SummonerLvlReq;

	short Max_Normal_Level;
	short Max_Master_Level;

	float Master_Div1;
	float Master_Div2;
	float Master_Div3;
	float Master_ValueLimit;

	short Max_NPC_Phantom_Buffer;
	short Min_Guild_Count_For_CS;
	short ResetReq_NpcGuildMasterTalk;
	short GCGuildWarRequestResult_GuildWarMemberReq;

	BOOL gLogOutType_WriteInLogFile;
	BOOL UseSNOCheckCode;
	BOOL FixMaxGage;
	BOOL ShowMaxGageReached;
	BOOL TradeAntiHack;
	BOOL AllowPShopOnlyInLMarket;

	BOOL UseMoveperSecs;
	int AllowedMoveperSecs;

	//=================================================================

	BYTE gObjLevelUpMasterTree;

	BYTE gObjLevelUpDLNormal;
	BYTE gObjLevelUpMGNormal;
	BYTE gObjLevelUpRFNormal;

	BYTE gObjLevelUpDLMGCW;
	BYTE gObjLevelUpDLMGBC;
	BYTE gObjLevelUpDLMGCC;

	BYTE gObjLevelUpGralNormal;
	BYTE gObjLevelUpGralCW;
	BYTE gObjLevelUpGralBC;
	BYTE gObjLevelUpGralCC;

	BYTE gObjLevelUpPlusQuestNormal;
	BYTE gObjLevelUpPlusQuestCW;
	BYTE gObjLevelUpPlusQuestBC;
	BYTE gObjLevelUpPlusQuestCC;

	BYTE gObjLevelUpMasterNormal;
	BYTE gObjLevelUpMasterCW;
	BYTE gObjLevelUpMasterBC;
	BYTE gObjLevelUpMasterCC;

	//=================================================================

	unsigned short gObjCalCharacter_AttackDamageMin_Bow_StrDexDiv_Elf;
	unsigned short gObjCalCharacter_AttackDamageMax_Bow_StrDexDiv_Elf;
	unsigned short gObjCalCharacter_AttackDamageMin_NoBow_DexDiv_Elf;
	unsigned short gObjCalCharacter_AttackDamageMin_NoBow_StrDiv_Elf;
	unsigned short gObjCalCharacter_AttackDamageMax_NoBow_DexDiv_Elf;
	unsigned short gObjCalCharacter_AttackDamageMax_NoBow_StrDiv_Elf;
	unsigned short gObjCalCharacter_AttackDamageMin_DexStrDiv_Elf;
	unsigned short gObjCalCharacter_AttackDamageMax_DexStrDiv_Elf;
	unsigned short gObjCalCharacter_AttackSpeed_Div_Elf;
	unsigned short gObjCalCharacter_MagicSpeed_Div_Elf;
	unsigned short gObjCalCharacter_Defense_Div_Elf;
	unsigned short gObjCalCharacter_Blocking_Div_Elf;
	unsigned short gObjCalCharacter_m_MagicDamageMin_Div_Elf;
	unsigned short gObjCalCharacter_m_MagicDamageMax_Div_Elf;

	unsigned short gObjCalCharacter_AttackDamageMin_StrDiv_DK;
	unsigned short gObjCalCharacter_AttackDamageMax_StrDiv_DK;
	unsigned short gObjCalCharacter_AttackSpeed_Div_DK;
	unsigned short gObjCalCharacter_MagicSpeed_Div_DK;
	unsigned short gObjCalCharacter_Blocking_Div_DK;
	unsigned short gObjCalCharacter_Defense_Div_DK;
	unsigned short gObjCalCharacter_m_MagicDamageMin_Div_DK;
	unsigned short gObjCalCharacter_m_MagicDamageMax_Div_DK;

	unsigned short gObjCalCharacter_AttackDamageMin_StrDiv_DW;
	unsigned short gObjCalCharacter_AttackDamageMax_StrDiv_DW;
	unsigned short gObjCalCharacter_AttackSpeed_Div_DW;
	unsigned short gObjCalCharacter_MagicSpeed_Div_DW;
	unsigned short gObjCalCharacter_Defense_Div_DW;
	unsigned short gObjCalCharacter_Blocking_Div_DW;
	unsigned short gObjCalCharacter_m_MagicDamageMin_Div_DW;
	unsigned short gObjCalCharacter_m_MagicDamageMax_Div_DW;

	int gObjCalCharacter_AttackDamageMin_StrDiv_MG;
	int gObjCalCharacter_AttackDamageMin_EneDiv_MG;
	int gObjCalCharacter_AttackDamageMax_StrDiv_MG;
	int gObjCalCharacter_AttackDamageMax_EneDiv_MG;
	int gObjCalCharacter_AttackSpeed_Div_MG;
	int gObjCalCharacter_MagicSpeed_Div_MG;
	int gObjCalCharacter_Defense_Div_MG;
	int gObjCalCharacter_Blocking_Div_MG;
	int gObjCalCharacter_m_MagicDamageMin_Div_MG;
	int gObjCalCharacter_m_MagicDamageMax_Div_MG;

	int gObjCalCharacter_AttackDamageMin_StrDiv_RF;
	int gObjCalCharacter_AttackDamageMin_VitDiv_RF;
	int gObjCalCharacter_AttackDamageMax_StrDiv_RF;
	int gObjCalCharacter_AttackDamageMax_VitDiv_RF;
	
	int gObjCalCharacter_AttackDamageMin_PercentPlus_RF;
	int gObjCalCharacter_AttackDamageMax_PercentPlus_RF;

	int gObjCalCharacter_AttackSpeed_Div_RF;
	int gObjCalCharacter_MagicSpeed_Div_RF;
	int gObjCalCharacter_Defense_Div_RF;
	int gObjCalCharacter_Blocking_Div_RF;
	int gObjCalCharacter_m_MagicDamageMin_Div_RF;
	int gObjCalCharacter_m_MagicDamageMax_Div_RF;

	int gObjCalCharacter_AttackDamageMin_StrDiv_DL;
	int gObjCalCharacter_AttackDamageMin_EneDiv_DL;
	int gObjCalCharacter_AttackDamageMax_StrDiv_DL;
	int gObjCalCharacter_AttackDamageMax_EneDiv_DL;
	int gObjCalCharacter_AttackSpeed_Div_DL;
	int gObjCalCharacter_MagicSpeed_Div_DL;
	int gObjCalCharacter_Defense_Div_DL;
	int gObjCalCharacter_Blocking_Div_DL;
	int gObjCalCharacter_m_MagicDamageMin_Div_DL;
	int gObjCalCharacter_m_MagicDamageMax_Div_DL;

	int gObjCalCharacter_AttackDamageMin_StrDiv_SU;
	int gObjCalCharacter_AttackDamageMax_StrDiv_SU;
	int gObjCalCharacter_AttackSpeed_Div_SU;
	int gObjCalCharacter_MagicSpeed_Div_SU;
	int gObjCalCharacter_Defense_Div_SU;
	int gObjCalCharacter_Blocking_Div_SU;
	int gObjCalCharacter_m_MagicDamageMin_Div_SU;
	int gObjCalCharacter_m_MagicDamageMax_Div_SU;
	BOOL OfficialHelper_Activate;
	DWORD OfficialHelper_Zen;


	int Max_Party_Level_Dif;

	int gObjExpParty_viewpercent1;
	int gObjExpParty_viewpercent2;
	int gObjExpParty_viewpercent3;
	int gObjExpParty_viewpercent4;
	int gObjExpParty_viewpercent5;
	int gObjExpParty_viewpercent6;
	int gObjExpParty_viewpercent7;
	int gObjExpParty_viewpercent8;
	int gObjExpParty_viewpercent9;

	float ZenDropMult;
	BYTE ZenDrop;
	BOOL IsZenDrop;

	BOOL CPCShop__MonsterDieAddPoints;
	BOOL CPCShop__SendDataOnlyForGM;

	int MaxPoints[7][5];
	int DuelManager__GladiatorsGloryExp;
	int DuelManager__GladiatorsGloryDrop;
	BOOL DuelManager__IsGlobalMsg;
	int DuelManager__MinLevel;
	BYTE DuelManager__IsSaveInDB;
	WORD DuelManager__IsLevelDiferenceForSave;
	WORD DuelManager__LevelDiferenceForSave;
	WORD DuelManager__IsResetDiferenceForSave;
	WORD DuelManager__ResetDiferenceForSave;
	WORD DuelManager__MinLevelForSave;
	WORD DuelManager__MinResetForSave;

//Commands vars
	BOOL CmdItemEnabled;

	BOOL CmdPostEnabled;
	int CmdPostColor;
	int CmdPostMoney;
	int CmdPostLevel;

	BOOL CmdAddEnabled;
	int CmdAddMaxNumber;
	int CmdAddMoney;

	BOOL CmdPKClearEnabled;
	int CmdPKClearMoney;

	BOOL CmdSkinEnabled;
	BOOL CmdSkinOnlyForGM;
	int CmdSkinMoney;
	int CmdSkinLevel;

	int CmdMarryAF;
	int CmdBotPetAF;
	int CmdPostAF;

//Items Var
	BOOL CGSellRequestRecvQuestSell;
	int CMapItem_xItemDropTime;
	int gObjCalCharacterDakHorseDiv;

	int RAVEN_ATTACK_DISTANCE;
	int DarkSpiritAttackDamageMinConstA;
	int DarkSpiritAttackDamageMinConstB;
	int DarkSpiritAttackDamageMinConstC;
	int DarkSpiritAttackDamageMaxConstA;
	int DarkSpiritAttackDamageMaxConstB;
	int DarkSpiritAttackDamageMaxConstC;
	int DarkSpiritAttackSpeedConstA;
	int DarkSpiritAttackSpeedConstB;
	int DarkSpiritAttackSpeedConstC;
	int DarkSpiritAttackSpeedConstD;
	int DarkSpiritAttackSuccessRateConstA;
	int DarkSpiritAttackSuccessRateConstB;
	
	int DarkSpiritDurSmallConstA;
	int DarkSpiritDurSmallConstB;
	int DarkSpiritDurSmallConstC;

	int gObjMonsterItemDropConfigScriptEnabled;
	int gObjMonsterDieGiveItemExcDrop;
	int gObjMonsterDieRewardItemsExcSkill;
	int gObjMonsterDieRewardItemsExcLuck;
	int gObjMonsterDieRewardItemsNormalSkill;
	int gObjMonsterDieRewardItemsNormalLuck;
	BYTE gObjMonsterDieRewardItemsMaxItemsExc;

	int CGUseItemRecvHP0;
	int CGUseItemRecvHP1;
	int CGUseItemRecvHP2;
	int CGUseItemRecvHP3;
	int CGUseItemRecvMP1;
	int CGUseItemRecvMP2;
	int CGUseItemRecvMP3;

	int SkillChangeUseTransRing1;
	int SkillChangeUseTransRing2;
	int SkillChangeUseTransRing3;
	int SkillChangeUseTransRing4;
	int SkillChangeUseTransRing5;
	int SkillChangeUseTransRing6;

	BYTE Soket__MaxSoketsOnItem;
	BYTE Soket__MinSoketsOnItem;

	BYTE Soket__EnableExcSocketItemsDrop;
	BYTE Soket__ExcAdditionalPercentOnSoketItem;
	BYTE Soket__ExcMaximumOptionsOnSoketItem;

	BYTE Soket__EnableExcOnZeroSoketItemsDrop;
	BYTE Soket__ExcAdditionalPercentOnZSoketItem;
	BYTE Soket__ExcMaximumOptionsOnZSoketItem;

	int Soket__DropEmptySpearRate1;
	short Soket__DropEmptySpearLevel1;
	int Soket__DropEmptySpearRate2;
	short Soket__DropEmptySpearLevel2;
	int Soket__DropEmptySpearRate3;
	short Soket__DropEmptySpearLevel3;
	int Soket__DropEmptySpearRate4;
	short Soket__DropEmptySpearLevel4;
	int Soket__DropEmptySpearRate5;
	short Soket__DropEmptySpearLevel5;
	BOOL Soket__AutoRemoveSameSocket;

	BOOL Soket__IsRecoverySlot;
	BYTE Soket__SlotsToAdd;
	int Soket__MinExcOptionsCountOnItem;

	int CItem__ValueBless;
	int CItem__ValueSoul;
	int CItem__ValueChaos;
	int CItem__ValueLife;
	int CItem__ValueCreation;
	int CItem__ValueGuardian;
	int CItem__ValueBlessWater;
	int CItem__ValueSoulWater;

	int gObjItemRandomLevelUpSoulLuck;
	int gObjItemRandomLevelUpSoulNoLuck;
	int gObjItemRandomOption3UpLife;

	BYTE IsVipExtraMixPercent;
	BYTE VipExtraMixPercent;

	BYTE ChaosWeaponMinPercent ;
	BYTE ChaosWeaponMaxPercent ;
	BYTE ChaosWeaponMaxMixLevel;
	BYTE ChaosWeaponSkillPercent;
	BYTE ChaosWeaponLuckPercent;
	BYTE ChaosWeaponZ28Max;
	BYTE IsAllowExcChaosWeaponMix;
	BYTE ExcChaosWeaponPercent;
	BYTE ExcChaosWeaponMaxOpt;

	BYTE FirstWingMinPercent ;
	BYTE FirstWingMaxPercent ;
	BYTE FirstWingMaxMixLevel;
	BYTE FirstWingSkillPercent;
	BYTE FirstWingLuckPercent;
	BYTE FirstWingZ28Percent;
	BYTE FirstWingZ28Max;

	BYTE DinorantMixPercent;
	BYTE DinorantSkillPercent;
	BYTE DinorantZ28Percent;
	BYTE DinorantZ28Max;

	BYTE FruitMixPercent ;

	BYTE DLRFCapeMinPercent;
	BYTE DLRFCapeMaxPercent;
	BYTE RFCapeProbability;
	BYTE DLRFIsUseNewExcMixSystem;
	BYTE DLRFExcOptionProbability;
	BYTE DLRFMaxExcOption;
	BYTE DLRFCapeLuckPercent;
	BYTE DLRFCapeZ28Percent;
	BYTE DLRFCapeZ28Max;

	BYTE SecondWingMinPercent;
	BYTE SecondWingMaxPercent;
	BYTE SecondWingIsUseNewExcMix;
	BYTE SecondWingExcOptionProb;
	BYTE SecondWingMaxExcOption;
	BYTE SecondWingLuckPercent;
	BYTE SecondWingZ28Percent;
	BYTE SecondWingZ28Max;

	BYTE CondorMixMinPercent;
	BYTE CondorMixMaxPercent;
	BYTE ThirdWingMinPercent;
	BYTE ThirdWingMaxPercent;
	BYTE ThirdWingIsUseNewExcMix;
	BYTE ThirdWingExcOptionProb;
	BYTE ThirdWingMaxExcOption;
	BYTE ThirdWingLuckPercent;
	BYTE ThirdWingZ28Percent;
	BYTE ThirdWingZ28Max;

	BYTE SetItemMixMinPercent;
	BYTE SetItemMixMaxPercent;
	BYTE SetItemOptionPercent;

	BYTE DarkHorseMixPercent;
	BYTE DarkHorseSkillPercent;
	BYTE DarkSpiritMixPercent;
	BYTE DarkSpiritSkillPercent;

	BYTE BlessPotionMixPercent;
	BYTE SoulPotionMixPercent;
	BYTE LifeStoneMixPercent;
	BYTE CastleSpecialMixPercent;

	BYTE b7BlueCrystalMix;
	BYTE b5RedCrystalMix;
	BYTE b3BlackCrystalMix;
	BYTE BlueRedBlackCrystalMix;

	BYTE Fenrir01LevelMixRate;
	BYTE Fenrir02LevelMixRate;
	BYTE Fenrir03LevelMixRate;
	BYTE FenrirUpgradeMaxPercent;

	BYTE ShieldPotionLv1MixRate;
	int  ShieldPotionLv1MixMoney;
	BYTE ShieldPotionLv2MixRate;
	int  ShieldPotionLv2MixMoney;
	BYTE ShieldPotionLv3MixRate;
	int  ShieldPotionLv3MixMoney;

	BYTE SeedMixMaxSuccessRate;
	BYTE ExtractSeedChaosMixBaseRate;

	BYTE BCChaosSuccessRate1;
	BYTE BCChaosSuccessRate2;
	BYTE BCChaosSuccessRate3;
	BYTE BCChaosSuccessRate4;
	BYTE BCChaosSuccessRate5;
	BYTE BCChaosSuccessRate6;
	BYTE BCChaosSuccessRate7;
	BYTE BCChaosSuccessRate8;

	int  BCChaosMixMoney1;
	int  BCChaosMixMoney2;
	int  BCChaosMixMoney3;
	int  BCChaosMixMoney4;
	int  BCChaosMixMoney5;
	int  BCChaosMixMoney6;
	int  BCChaosMixMoney7;
	int  BCChaosMixMoney8;

	BYTE ITChaosSuccessRate1;
	BYTE ITChaosSuccessRate2;
	BYTE ITChaosSuccessRate3;
	BYTE ITChaosSuccessRate4;
	BYTE ITChaosSuccessRate5;
	BYTE ITChaosSuccessRate6;

	BYTE gDQChaosSuccessRateLevel1;
	BYTE gDQChaosSuccessRateLevel2;
	BYTE gDQChaosSuccessRateLevel3;
	BYTE gDQChaosSuccessRateLevel4;
	BYTE gDQChaosSuccessRateLevel5;
	BYTE gDQChaosSuccessRateLevel6;
	BYTE gDQChaosSuccessRateLevel7;

	BYTE PlusItemLuckPercent;
	BYTE PlusItemLevelChaosMix10;
	BYTE PlusItemLevelChaosMix11;
	BYTE PlusItemLevelChaosMix12;
	BYTE PlusItemLevelChaosMix13;
	BYTE PlusItemLevelChaosMix14;
	BYTE PlusItemLevelChaosMix15;
	BYTE PlusItemLevelChaosMaxRate10;
	BYTE PlusItemLevelChaosMaxRate11;
	BYTE PlusItemLevelChaosMaxRate12;
	BYTE PlusItemLevelChaosMaxRate13;
	BYTE PlusItemLevelChaosMaxRate14;
	BYTE PlusItemLevelChaosMaxRate15;

	BYTE PlusItemLevelChaosLuckyRefine255;
	BYTE ChaosLuckyItemMinLevel;
	BYTE ChaosLuckyItemMaxLevel;
	BYTE ChaosLuckyItemMaxOption;
	BYTE ChaosLuckyItemLuckPercent;

	BOOL ItemTradeLimit;
	BOOL ItemTradeJOHBlock;

	//Promo Item
	BOOL ItemTradeFFFFBlock;
	BOOL BlockFFFFFFFFSerialRepair;
	BOOL DeleteFFFFFFFFSerial;
	float IncreaseFFFFFFFFSerialDurDown;

	//Non Tradeable
	BOOL ItemTradeFFFEBlock;
	BOOL BlockFFFFFFFESerialRepair;
	BOOL DeleteFFFFFFFESerial;
	float IncreaseFFFFFFFESerialDurDown;

	//Durability
	BOOL ItemTradeFFFDBlock;
	BOOL BlockFFFFFFFDSerialRepair;
	BOOL DeleteFFFFFFFDSerial;
	float IncreaseFFFFFFFDSerialDurDown;

	//Non-Repair
	BOOL ItemTradeFFFCBlock;
	BOOL BlockFFFFFFFCSerialRepair;
	BOOL DeleteFFFFFFFCSerial;
	float IncreaseFFFFFFFCSerialDurDown;

	BYTE UseChangeSellMoney;
	DWORD MaxSellMoney; 

	BOOL ItemSellLimit;
	BOOL ItemMixLimit;
	BOOL ItemPshopLimit;
	BOOL ItemJinhuaLimit;

	BYTE ItemPlusLimit;
	BYTE ExItemPlusLimit;
	BYTE ExItemOpLimit;
	BYTE SetItemPlusLimit;

	BOOL CItem_ConvertExcAncient;
	BOOL StrengthenItemByJewelOfHarmonyAncient;
	BYTE StrengthenItemByJewelOfHarmony_MAXLevel;
	BYTE MAXJOHITEMOPTION;
	BYTE JOHLevelMax;
};

extern CReadConfig ReadConfig;