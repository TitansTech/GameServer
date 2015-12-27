#ifndef CASTLESIEGE_H
#define CASTLESIEGE_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#if (GS_CASTLE==1)

using namespace std;

#include "protocol.h"

#define CASTLESIEGE_CROWN_REGISTER_TIME 60000

#define CS_MAX_UPGRADE 3

#define CASTLESIEGE_DATALOAD_1 1
#define CASTLESIEGE_DATALOAD_2 2
#define CASTLESIEGE_DATALOAD_3 3
#define CASTLESIEGE_DATALOAD_4 4

#define CASTLESIEGE_STATE_NONE -1
#define CASTLESIEGE_STATE_IDLE_1 0
#define CASTLESIEGE_STATE_REGSIEGE 1
#define CASTLESIEGE_STATE_IDLE_2 2
#define CASTLESIEGE_STATE_REGMARK 3
#define CASTLESIEGE_STATE_IDLE_3 4
#define CASTLESIEGE_STATE_NOTIFY 5
#define CASTLESIEGE_STATE_READYSIEGE 6
#define CASTLESIEGE_STATE_STARTSIEGE 7
#define CASTLESIEGE_STATE_ENDSIEGE 8
#define CASTLESIEGE_STATE_ENDCYCLE 9

// Added By Prodito ( 24-04-2009 @ 11:26 )
#define CASTLESIEGE_LEFTTIME_NOTRUNNING -3
#define CASTLESIEGE_LEFTTIME_ENDSIEGE -2
#define CASTLESIEGE_LEFTTIME_ONSIGE -1

struct PMSG_SENDMINIMAPDATA
{
	unsigned char btX;
	unsigned char btY;
};

struct _CS_SCHEDULE_DATA
{
/* this+0x4 */ int m_bIN_USE;
/* this+0x8 */ int m_iSTATE;
/* this+0xC */ int m_iADD_DAY;
/* this+0x10 */ int m_iADD_HOUR;
/* this+0x14 */ int m_iADD_MIN;
/* this+0x18 */ int m_iGAP_SEC;

	_CS_SCHEDULE_DATA()
	{
		this->Clear();
	};

	virtual ~_CS_SCHEDULE_DATA(){};

	void Clear()
	{
		this->m_bIN_USE = 0;
		this->m_iSTATE = -1;
		this->m_iADD_DAY = -1;
		this->m_iADD_HOUR = -1; 
		this->m_iADD_MIN = -1;
		this->m_iGAP_SEC = 0;
	};
};

struct _CS_NPC_DATA
{
	_CS_NPC_DATA()
	{
		Clear();
		m_iCS_GATE_LEVER_INDEX = -1;
	};

	virtual ~_CS_NPC_DATA()
	{

	};

	void Clear()
	{
		m_bIN_USE = 0;
		m_iNPC_NUM = -1;
		m_iNPC_INDEX = -1;
		m_iNPC_LIVE = 0;
		m_bNPC_DBSAVE = 0;
		m_iNPC_SIDE = 0;
		m_iNPC_OBJINDEX = -1;
		m_iNPC_DF_LEVEL = 0;
		m_iNPC_RG_LEVEL = 0;
		m_iNPC_MAXHP = 0;
		m_iNPC_HP = 0;
		m_iNPC_SX = -1;
		m_iNPC_SY = -1;
		m_iNPC_DX = -1;
		m_iNPC_DY = -1;
		m_iNPC_DIR = -1;
		m_iNPC_BASE_DF_LEVEL = 0;
		m_iNPC_BASE_RG_LEVEL = 0;
		m_iNPC_BASE_MAXHP = 0;
		m_iNPC_BASE_HP = 0;
		m_iNPC_BASE_SX = -1;
		m_iNPC_BASE_SY = -1;
		m_iNPC_BASE_DX = -1;
		m_iNPC_BASE_DY = -1;
		m_iNPC_BASE_DIR = -1;
		//m_iCsNpcExistVal = 0;
	}

	void SetBaseValue()
	{
		m_iNPC_DF_LEVEL = m_iNPC_BASE_DF_LEVEL;
		m_iNPC_RG_LEVEL = m_iNPC_BASE_RG_LEVEL;
		m_iNPC_MAXHP = m_iNPC_BASE_MAXHP;
		m_iNPC_HP = m_iNPC_BASE_HP;
		m_iNPC_SX = m_iNPC_BASE_SX;
		m_iNPC_SY = m_iNPC_BASE_SY;
		m_iNPC_DX = m_iNPC_BASE_DX;
		m_iNPC_DY = m_iNPC_BASE_DY;
		m_iNPC_DIR = m_iNPC_BASE_DIR;
	}

/* this+0x4 */ int m_bIN_USE;
/* this+0x8 */ short m_iNPC_NUM;
/* this+0xC */ short m_iNPC_INDEX;
/* this+0x10 */ BYTE m_bNPC_DBSAVE;
/* this+0x14 */ BYTE m_iNPC_SIDE;
/* this+0x18 */ short m_iNPC_OBJINDEX;
/* this+0x1C */ short m_iNPC_DF_LEVEL;
/* this+0x20 */ short m_iNPC_RG_LEVEL;
/* this+0x24 */ int m_iNPC_MAXHP;
/* this+0x28 */ int m_iNPC_HP;
/* this+0x2C */ BYTE m_iNPC_SX;
/* this+0x30 */ BYTE m_iNPC_SY;
/* this+0x34 */ BYTE m_iNPC_DX;
/* this+0x38 */ BYTE m_iNPC_DY;
/* this+0x3C */ BYTE m_iNPC_DIR;
/* this+0x40 */ short m_iNPC_BASE_DF_LEVEL;
/* this+0x44 */ short m_iNPC_BASE_RG_LEVEL;
/* this+0x48 */ int m_iNPC_BASE_MAXHP;
/* this+0x4C */ int m_iNPC_BASE_HP;
/* this+0x50 */ BYTE m_iNPC_BASE_SX;
/* this+0x54 */ BYTE m_iNPC_BASE_SY;
/* this+0x58 */ BYTE m_iNPC_BASE_DX;
/* this+0x5C */ BYTE m_iNPC_BASE_DY;
/* this+0x60 */ BYTE m_iNPC_BASE_DIR;
/* this+0x64 */ short m_iCS_GATE_LEVER_INDEX;
/* this+0x68 */ BYTE m_iNPC_LIVE;

	//union
	//{
	//	struct
	//	{
		/* this+0x70 */ //unsigned char m_btCsNpcExistVal1;
		/* this+0x71 */ //unsigned char m_btCsNpcExistVal2;
		/* this+0x72 */ //unsigned char m_btCsNpcExistVal3;
		/* this+0x73 */ //unsigned char m_btCsNpcExistVal4;
	//	};
		/* this+0x70 */ //int m_iCsNpcExistVal;
	//};
};

struct _CS_REG_GUILD_DATA
{
	_CS_REG_GUILD_DATA();

	virtual ~_CS_REG_GUILD_DATA();

	void Clear();

	//_CS_REG_GUILD_DATA & operator = (_CS_REG_GUILD_DATA & __that);

	BYTE m_bIN_USE;
	std::basic_string<char> m_strGuildName;
	short m_iRegMarkCount;
	BYTE m_iGuildMemberCount;
	short m_iGuildMasterLevel;
	short m_iSeqNum;
	__int64 m_i64TotolScore;
};

struct _CS_TOTAL_GUILD_DATA
{
	_CS_TOTAL_GUILD_DATA()
	{
		this->Clear();
	};

	virtual ~_CS_TOTAL_GUILD_DATA()
	{

	};

	void Clear()
	{
		m_bIN_USE = 0;
		m_iCsGuildID = 0;
		m_bGuildInvolved = 0;
	};

	int m_bIN_USE;
	int m_iCsGuildID;
	int m_bGuildInvolved;
};

struct _CS_MINIMAP_DATA
{
	_CS_MINIMAP_DATA()
	{
		this->Clear();
	};

	virtual ~_CS_MINIMAP_DATA()
	{

	};

	void Clear()
	{
		m_iGuildMasterIndex = -1;
		memset(m_szGuildName,0x00,sizeof(m_szGuildName));
		memset(m_stMiniMapPoint,0x00,sizeof(m_stMiniMapPoint));
		m_iMiniMapPointCount = 0;
	};


	int m_iGuildMasterIndex;
	char m_szGuildName[16];
	int m_iMiniMapPointCount;
	PMSG_SENDMINIMAPDATA m_stMiniMapPoint[1000];
};

class CCastleSiege
{
// Class Constructor / Destructor
public:
	CCastleSiege();
	virtual ~CCastleSiege();

// Class member functions
public:
	void Clear();
	int Ready(int iMapSvrGroup);
	int LoadPreFixData(char * lpszFileName);
	int LoadData(char * lpszFileName);
	int DataRequest();
	int Init();
	void Run();
	static BYTE ScheduleStateCompFunc(_CS_SCHEDULE_DATA & A,_CS_SCHEDULE_DATA & B);
	int GetDataLoadState(){return m_iCastleDataLoadState;}
	void SetDataLoadState(int iDataLoadState){m_iCastleDataLoadState = iDataLoadState;}
	void SetDbDataLoadOK(int bDbDataLoadOK){m_bDbDataLoadOK = bDbDataLoadOK;}
	int FirstCreateDbNPC();
	int SetCastleInitData(CSP_ANS_CSINITDATA * lpMsg);
	int SetCastleNpcData(CSP_CSINITDATA * lpMsg,int iCOUNT);
	int CheckSync();
	void GetNextDay(SYSTEMTIME * st, int iAfterDay, int iAfterHour, int iAfterMin, int iAfterSec);
	int GetStateGapSec(int iCastleSiegeState);
	void SetState(int iCastleSiegeState,int bSetRemainMsec);
	void SetState_NONE(int bSetRemainMsec);
	void SetState_IDLE_1(int bSetRemainMsec);
	void SetState_REGSIEGE(int bSetRemainMsec);
	void SetState_IDLE_2(int bSetRemainMsec);
	void SetState_REGMARK(int bSetRemainMsec);
	void SetState_IDLE_3(int bSetRemainMsec);
	void SetState_NOTIFY(int bSetRemainMsec);
	void SetState_READYSIEGE(int bSetRemainMsec);
	void SetState_STARTSIEGE(int bSetRemainMsec);
	void SetState_ENDSIEGE(int bSetRemainMsec);
	void SetState_ENDCYCLE(int bSetRemainMsec);
	void ProcState_NONE();
	void ProcState_IDLE_1();
	void ProcState_REGSIEGE();
	void ProcState_IDLE_2();
	void ProcState_REGMARK();
	void ProcState_IDLE_3();
	void ProcState_NOTIFY();
	void ProcState_READYSIEGE();
	void ProcState_STARTSIEGE();
	void ProcState_ENDSIEGE();
	void ProcState_ENDCYCLE();
	int GetCastleState(){return m_iCastleSiegeState;}
	SYSTEMTIME GetCastleLeftSiegeDate() {return m_tmLeftCastleSiegeDate;}
	int CalcCastleLeftSiegeDate();
	int GetCastleStateTerm(SYSTEMTIME * tmStateStartDate, SYSTEMTIME * tmStateEndDate);
	int GetCurRemainSec();
	void ClearDbNPC();
	void ClearNonDbNPC();
	void ClearNonDbNPC_MidWin();
	void ClearAllNPC();
	void CreateDbNPC();
	void CreateNonDbNPC(int bDelFirst);
	int CheckAddDbNPC(int iIndex, int iNpcType, int iNpcIndex, BYTE & btResult);
	int AddDbNPC(int iNpcType, int iNpcIndex);
	int RepairDbNPC(int iNpcType, int iNpcIndex, int iNpcHP, int iNpcMaxHP);
	int UpgradeDbNPC(int iIndex, int iNpcType, int iNpcIndex, int iNpcUpType, int iNpcUpValue, int iNpcUpIndex);
	int UpgradeDbNPC_DFLEVEL(int iNpcType, int iNpcIndex, int iNpcDfLevel);
	int UpgradeDbNPC_RGLEVEL(int iNpcType, int  iNpcIndex, int iNpcRgLevel);
	int UpgradeDbNPC_MAXHP(int iNpcType, int iNpcIndex, int iNpcMaxHP);
	void StoreDbNpc();
	int DelNPC(int iIndex, int iNpcType, int bDbSave);
	void SendNpcStateList(int iIndex, int iNpcType);
	int GetNpcData(int iNpcType, int iNpcIndex,_CS_NPC_DATA & pRetNpcData);
	void AdjustDbNpcLevel();
	void SendCsGateStateViewPort(int iGateIndex, BYTE btOperate);
	void CreateAllCsGateLever();
	int CreateCsGateLever(int iX, int iY);
	int LinkCsGateLever(int iLeverIndex, int iGateIndex);
	int CheckLeverAlive(int iLeverIndex);
	int CheckCsGateAlive(int iGateIndex);
	void CheckCsDbNpcAlive();
	void SetCrownIndex(int iIndex){m_iCrownIndex = iIndex;}
	void NotifyCrownState(BYTE btState);
	int GetRegCrownAvailable(){return m_bRegCrownAvailable;}
	void SetRegCrownAvailable(int bRegCrownAvailable){m_bRegCrownAvailable = bRegCrownAvailable;}
	int GetCastleTowerAccessable(){return m_bCastleTowerAccessable;}
	void SetCastleTowerAccessable(int bTowerAccessable){m_bCastleTowerAccessable = bTowerAccessable;}
	int DelGemOfDefend(int iIndex, int iNeedGemOfDefend);
	void SetTaxRate(int iTaxType, int iTaxRate);
	int CheckCastleHasMoney(__int64 i64CastleMoney);
	__int64 GetCastleMoney() { return m_i64CastleMoney; }
	void SetCastleMoney(__int64 i64CastleMoney);
	void ResetCastleTaxInfo();
	void SetIsSiegeGuildList(int bIsSiegGuildList){m_btIsSiegeGuildList = bIsSiegGuildList;}
	void SendCastleStateSync();
	int GetDbNpcCreated(){return m_bDbNpcCreated;}
	void SetDbNpcCreated(int bDbNpcCreated){m_bDbNpcCreated = bDbNpcCreated;}
	int CheckGuildOwnCastle(char * lpszGuildName);
	void ResetCastleCycle();
	void SetGateBlockState(int iX, int iY, int iGateState);
	char * GetCastleOwnerGuild(){return m_szCastleOwnerGuild;}
	int GetHuntZoneEnter(){return m_bEnterHuntZone;}
	void SetHuntZoneEnter(int bEnterHuntZone){m_bEnterHuntZone = bEnterHuntZone;}

	//
	int GetTaxRateChaos();
	int GetTaxRateStore();
	int GetTaxHuntZone();
	//

	void CheckBuildCsGuildInfo();
	void SetCalcRegGuildList(CSP_CALCREGGUILDLIST * lpMsg, int iCOUNT);
	void MakeCsTotalGuildInfo(CSP_CSGUILDUNIONINFO * lpMsg, int iCOUNT);
	void SaveCsTotalGuildInfo();
	void SetCsTotalGuildInfo(CSP_CSLOADTOTALGUILDINFO * lpMsg, int iCOUNT);
	int GetCsJoinSide(char * lpszGuildName, BYTE * btCsJoinSide, BYTE * bCsGuildInvolved);
	void SetUserCsJoinSide(int iUSER);
	void SetAllUserCsJoinSide();
	void ResetAllUserCsJoinSide();
	void NotifySelfCsJoinSide(int iIndex);
	int OperateGate(int iObjIndex, int bOpenType);
	int CheckAttackGuildExist();
	int CheckCastleOwnerMember(int iIndex);
	int CheckUnionMember(int iIndex);
	int CheckCastleOwnerUnionMember(int iIndex);
	int CheckGuardianStatueExist();
	void ChangeWinnerGuild(int iCsJoinSide);
	int CheckMiddleWinnerGuild();
	int CheckCastleSiegeResult();
	int CheckUnionGuildMasterOrAssistant(int iIndex);
	void SetCrownUserIndex(int iIndex){this->m_iCastleCrownAccessUser = iIndex;}
	int GetCrownUserIndex(){return m_iCastleCrownAccessUser;}
	void ResetCrownUserIndex(){m_iCastleCrownAccessUser=-1;}
	int GetCrownAccessUserX(){return m_btCastleCrownAccessUserX;}
	int GetCrownAccessUserY(){return m_btCastleCrownAccessUserY;}
	void SetCrownAccessUserX(BYTE iX){m_btCastleCrownAccessUserX = iX;}
	void SetCrownAccessUserY(BYTE iY){m_btCastleCrownAccessUserY = iY;}
	void SetCrownSwitchUserIndex(int iMonsterClass, int iIndex);
	int GetCrownSwitchUserIndex(int iMonsterClass);
	void ResetCrownSwitchUserIndex(int iMonsterClass);
	void SetCrownAccessTickCount(){this->m_dwCrownAccessTime = GetTickCount();}
	void SetCrownAccessTickCountByVal(int TickCount){this->m_dwCrownAccessTime = TickCount;}
	void ResetCrownAccessTickCount(){m_dwCrownAccessTime = 0;}
	int CheckOverlapCsMarks(int iIndex);
	int GetCsAttkGuildList(PMSG_CSATTKGUILDLIST * lpMsgBody, int & iCount);
	int CheckTeleportMagicAxisY(int iStartY, int iTargetX, int iTargetY);
	void NotifyCsSelfLeftTime(int iIndex);
	void CheckReviveNonDbNPC();
	void CheckReviveGuardianStatue();
	void ReSpawnAllUser();
	void ReSpawnEnemyUser(int bRefreshOwnerUser);
	void NotifyAllUserCsStartState(BYTE btStartState);
	void NotifyAllUserCsProgState(BYTE btProgState, char * lpszGuildName);
	void ClearCastleTowerBarrier();
	void SetAllCastleGateState(int bOpenType);
	void AddMiniMapDataReqUser(int iIndex);
	void DelMiniMapDataReqUser(int iIndex);
	void OperateMiniMapWork();
	void SendMapServerGroupMsg(char *  lpszMsg);
	void SendAllUserAnyData(BYTE * lpMsg, int iSize);
	void SendAllUserAnyMsg(char *  lpMsg, int iSize);
	void SendCsUserAnyData(BYTE * lpMsg, int iSize);
	void SendCsUserAnyMsg(char * lpszMsg);
	static BYTE RegGuildScoreCompFunc(_CS_REG_GUILD_DATA & A, _CS_REG_GUILD_DATA & B);
	void OperateGmCommand(int iIndex, int iGmCommand, void * lpParam);
	void CreateDbNPC_INS();

	DWORD GetCrownAccessTickCount(){return m_dwCrownAccessTime;}
	void NotifyCrownSwitchInfo(int iIndex);

	void GetStateDate(int iCastleSiegeState, SYSTEMTIME* st);
// Class member variables
/*this+0x8*/ int m_bCastleSiegeEnable;
/*this+0xC*/ int m_bFileDataLoadOK;
/*this+0x10*/ int m_bDbDataLoadOK;
/*this+0x14*/ int m_bDoRun;
/*this+0x18*/ int m_iMapSvrGroup;
/*this+0x1C*/ int m_iCastleDataLoadState;
/*this+0x20*/ unsigned long m_dwDbDataLoadTickCount;
/*this+0x24*/ int m_iCastleSiegeCycle;
/*this+0x28*/ int m_iCastleSiegeState;
			  SYSTEMTIME m_tmSiegeEndSchedule;
/*this+0x2C*/ int m_bFixCastleCycleStartDate;
/*this+0x30*/ SYSTEMTIME m_tmFixCastleCycleStartDate;
/*this+0x40*/ int m_iFixCastleSpecificState;
/*this+0x44*/ int m_bFixCastleStateStartDate;
/*this+0x48*/ SYSTEMTIME m_tmFixCastleStateStartDate;
/*this+0x58*/ int m_iCS_REMAIN_MSEC;
/*this+0x5C*/ int m_iCS_TICK_COUNT;
/*this+0x60*/ SYSTEMTIME m_tmStateStartDate;
/*this+0x70*/ SYSTEMTIME m_tmLeftCastleSiegeDate;
/*this+0x80*/ unsigned long m_dwNPC_DBSAVE_TICK_COUNT;
/*this+0x84*/ unsigned long m_dwCALC_LEFTSIEGE_TICK_COUNT;
/*this+0x88*/ unsigned long m_dwEVENT_MSG_TICK_COUNT;
/*this+0x8C*/ unsigned long m_dwCHECK_GATE_ALIVE_COUNT;
/*this+0x90*/ unsigned long m_dwCS_JOINSIDE_REFRESH_TICK_COUNT;
/*this+0x94*/ unsigned long m_dwCS_JOINSIDE_REFRESH_TOTAL_COUNT;
/*this+0x98*/ unsigned long m_dwCS_MINIMAP_SEND_TICK_COUNT;
/*this+0x9C*/ unsigned long m_dwCS_LEFTTIME_TICK_COUNT;
/*this+0xA0*/ unsigned long m_dwCS_STARTTIME_TICK_COUNT;
/*this+0xA4*/ SYSTEMTIME m_tmStartDate;
/*this+0xB4*/ SYSTEMTIME m_tmEndDate;
/*this+0xC4*/ unsigned char m_btIsSiegeGuildList;
/*this+0xC5*/ unsigned char m_btIsSiegeEnded;
/*this+0xC6*/ unsigned char m_btIsCastleOccupied;
/*this+0xC7*/ char m_szCastleOwnerGuild[0x10];
/*this+0xD8*/ __int64 m_i64CastleMoney;
/*this+0xE0*/ int m_iTaxRateChaos;
/*this+0xE4*/ int m_iTaxRateStore;
/*this+0xE8*/ int m_iTaxHuntZone;
/*this+0xEC*/ int m_bEnterHuntZone;
/*this+0xF0*/ vector<_CS_SCHEDULE_DATA> m_vtScheduleData;
/*this+0x100*/ CRITICAL_SECTION m_critScheduleData;
/*this+0x118*/ vector<_CS_NPC_DATA> m_vtNpcData;
/*this+0x128*/ CRITICAL_SECTION m_critNpcData;
/*this+0x140*/ int m_bDbNpcCreated;
/*this+0x144*/ map<string,_CS_TOTAL_GUILD_DATA> m_mapCsBasicGuildInfo;
/*this+0x154*/ CRITICAL_SECTION m_critCsBasicGuildInfo;
/*this+0x16C*/ map<string, _CS_TOTAL_GUILD_DATA> m_mapCsTotalGuildInfo;
/*this+0x17C*/ CRITICAL_SECTION m_critCsTotalGuildInfo;
/*this+0x194*/ int m_bCsBasicGuildInfoLoadOK;
/*this+0x198*/ int m_bCsTotalGuildInfoLoadOK;
/*this+0x19C*/ map<int,_CS_MINIMAP_DATA > m_mapMiniMapData;
/*this+0x1AC*/ CRITICAL_SECTION m_critCsMiniMap;
/*this+0x1C4*/ vector<int> m_vtMiniMapReqUser;
/*this+0x1D4*/ int m_bCastleTowerAccessable;
/*this+0x1D8*/ int m_bRegCrownAvailable;
/*this+0x1DC*/ int m_iCrownIndex;
/*this+0x1E0*/ int m_iCastleCrownAccessUser;
/*this+0x1E4*/ unsigned char m_btCastleCrownAccessUserX;
/*this+0x1E5*/ unsigned char m_btCastleCrownAccessUserY;
/*this+0x1E8*/ int m_iCastleSwitchAccessUser1;
/*this+0x1EC*/ int m_iCastleSwitchAccessUser2;
/*this+0x1F0*/ unsigned long m_dwCrownAccessTime;
/*this+0x1F4*/ char m_szMiddleWinnerGuild[0x10];
}; 

extern CCastleSiege g_CastleSiege;

//Gate Defence Level Settings
//==========================================================
static int g_iNpcUpDfLevel_CGATE[CS_MAX_UPGRADE][0x3] = {
	1,	2,	3000000,									//
	2,	3,	3000000,									//
	3,	4,	3000000										//
};

static int g_iNpcDefense_CGATE[CS_MAX_UPGRADE + 1] = {
	100, 180, 300, 520									//
};

//Gate HP/Durability Settings
//==========================================================
static int g_iNpcUpMaxHP_CGATE[CS_MAX_UPGRADE][0x3] = {
	2500000,	2,	1000000,							//+600 000		ORG: 800 000
	3500000,	3,	1000000,							//+1 000 000	ORG: 1 500 000
	5200000,	4,	1000000,							//+1 700 000	ORG: 3 000 000
};

//static int g_iNpcUpValue_CGATE[CS_MAX_UPGRADE][0x2] = {
//	2500000,	1900000,								//500000
//	3500000,	2500000,								//800000
//	5200000,	3500000									//1500000
//};
static int g_iNpcUpValue_CGATE[CS_MAX_UPGRADE][0x2] = {
	2500000,500000,
	3500000,2500000,
	5200000,3500000
};

//Statue Defence Level Settings
//==========================================================
static int g_iNpcUpDfLevel_CSTATUE[CS_MAX_UPGRADE][0x3] = {
	1,	3,	3000000,									//
	2,	5,	3000000,									//
	3,	7,	3000000,									//
};

static int g_iNpcDefense_CSTATUE[CS_MAX_UPGRADE + 1] = {
	80, 180, 340, 550									//
};

//Statue HP/Durability Settings
//==========================================================
static int g_iNpcUpMaxHP_CSTATUE[CS_MAX_UPGRADE][0x3] = {
	2200000,	3,	1000000,							//+700 000		ORG: 750 000
	3400000,	5,	1000000,							//+1 200 000	ORG: 1 300 000
	5000000,	7,	1000000								//+1 600 000	ORG: 2 500 000
};


static int g_iNpcUpValue_CSTATUE[CS_MAX_UPGRADE][0x2] = {
	2200000,400000,
	3400000,2200000,
	5000000,3400000
};

//static int g_iNpcUpValue_CSTATUE[CS_MAX_UPGRADE][0x2] = {
//	2200000,	1500000,								//400000
//	3400000,	2200000,								//
//	5000000,	3400000									//
//};

//Statue Regeneration Level Settings
//==========================================================
static int g_iNpcUpRgLevel_CSTATUE[CS_MAX_UPGRADE][0x3] = {
	1,	3,	5000000,									//
	2,	5,	5000000,									//
	3,	7,	5000000										//
};


static int g_iCsUserReSpawnArea[0x3][0x4] = {
	35,		17,		144,	48,
	74,		144,	115,	154,
	35,		11,		144,	48
};

static int g_iCastleWallStepAxisY[3] = {
	113, 160, 203
};

#endif

#endif