#include "stdafx.h"
#include "CastleSiege.h"

// GS-CS 0.99.60T 0x00533AC0
// GS-CS 1.00.16  0x00565480
#if (GS_CASTLE==1)

#include "DSProtocol.h"
#include "GameMain.h"
#include "TNotice.h"
#include "TUnion.h"
#include "LifeStone.h"
#include "Mercenary.h"
#include "Guardian.h"
#include "DBSockMng.h"
#include "LogToFile.h"
#include "logproc.h"
#include "CastleSiegeSync.h"
#include "SCFExDBProtocol.h"
#include "..\include\readscript.h"

CLogToFile g_LogCsPcRoomUserList("CS_PCROOM_USER_LOG", ".\\CS_PCROOM_USER_LOG", 1);

CCastleSiege g_CastleSiege;

// Added (26-04-2009 @ 19:03 ) By Prody

#define CS_CHECK_UPGRADE(value) ( (( (value)   )<0)?FALSE:(( (value)  )>CS_MAX_UPGRADE-1)?FALSE:TRUE  )

#define MACRO1(value) (( value & 0xFF) & 0xFF)
#define MACRO2(value) (( value & 0xFFFF ) & 0xFFFF)

#define MONTH_MACRO(value) ( (( (value)   )<0)?FALSE:(( (value)  )>12)?FALSE:TRUE  )
#define DAY_MACRO(value) ( (( (value - 1)   )<0)?FALSE:(( (value - 1)  )>30)?FALSE:TRUE  )
#define HOUR_MACRO(value) ( (( (value)   )<0)?FALSE:(( (value)  )>23)?FALSE:TRUE  )
#define MINUTE_MACRO(value) ( (( (value)   )<0)?FALSE:(( (value)  )>59)?FALSE:TRUE  )

#define XY_MACRO(value) ( (( (value)   )<0)?FALSE:(( (value)  )>255)?FALSE:TRUE  )

/*******************************************************************************************************
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*
 *::::::::::::::::::::::::::::::::		_CS_REG_GUILD_DATA Struct		:::::::::::::::::::::::::::::::*
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*
 *******************************************************************************************************/



_CS_REG_GUILD_DATA::_CS_REG_GUILD_DATA()
{
	Clear();
}

_CS_REG_GUILD_DATA::~_CS_REG_GUILD_DATA()
{

}
void _CS_REG_GUILD_DATA::Clear()
{
	m_bIN_USE = 0;
	m_strGuildName = "";
	m_iRegMarkCount = 0;
	m_iGuildMemberCount = 0;
	m_iGuildMasterLevel = 0;
	m_iSeqNum = 0;
	m_i64TotolScore = 0;
}
/* Prodito : Fuck you Fucking operator =, Vector hater! :@
_CS_REG_GUILD_DATA & _CS_REG_GUILD_DATA::operator = (_CS_REG_GUILD_DATA & __that)
{
	this->m_bIN_USE = __that.m_bIN_USE;
	this->m_strGuildName = __that.m_strGuildName;
	this->m_iRegMarkCount = __that.m_iRegMarkCount;
	this->m_iGuildMemberCount = __that.m_iGuildMemberCount;
	this->m_iGuildMasterLevel = __that.m_iGuildMasterLevel;
	this->m_iSeqNum = __that.m_iSeqNum;
	this->m_i64TotolScore = __that.m_i64TotolScore;

	return *this;
}*/

/*******************************************************************************************************
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*
 *::::::::::::::::::::::::::::::::			CCastleSiege Class			:::::::::::::::::::::::::::::::*
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*
 *******************************************************************************************************/

CCastleSiege::CCastleSiege()
{
	InitializeCriticalSection(&m_critScheduleData);
	InitializeCriticalSection(&m_critNpcData);
	InitializeCriticalSection(&m_critCsTotalGuildInfo);
	InitializeCriticalSection(&m_critCsBasicGuildInfo);
	InitializeCriticalSection(&m_critCsMiniMap);

	Clear();
}

CCastleSiege::~CCastleSiege()
{
	DeleteCriticalSection(&m_critScheduleData);
	DeleteCriticalSection(&m_critNpcData);
	DeleteCriticalSection(&m_critCsTotalGuildInfo);
	DeleteCriticalSection(&m_critCsBasicGuildInfo);
	DeleteCriticalSection(&m_critCsMiniMap);
}

void CCastleSiege::Clear()
{
	//DebugLog("%s START",__FUNCTION__);
	m_bCastleSiegeEnable = 1;
	m_bFileDataLoadOK = 0;
	m_bDbDataLoadOK = 0;
	m_bDoRun = 0;
	m_iMapSvrGroup = -1;
	m_iCastleSiegeCycle = 14;
	m_iCastleSiegeState = -1;
	m_bFixCastleCycleStartDate = 0;
	m_i64CastleMoney = 0;

	memset(&m_tmFixCastleCycleStartDate,0x00,sizeof(m_tmFixCastleCycleStartDate));

	m_iFixCastleSpecificState = 0;
	m_bFixCastleStateStartDate = 0;

	memset(&m_tmFixCastleStateStartDate,0x00,sizeof(m_tmFixCastleStateStartDate));

	m_iCastleDataLoadState = 0;
	m_dwDbDataLoadTickCount = 0;
	m_iCS_REMAIN_MSEC = -1;

	memset(&m_tmStateStartDate,0x00,sizeof(m_tmStateStartDate));
	memset(&m_tmLeftCastleSiegeDate,0x00,sizeof(m_tmLeftCastleSiegeDate));

	m_bCsBasicGuildInfoLoadOK = 0;
	m_bCsTotalGuildInfoLoadOK = 0;
	m_btIsSiegeGuildList = 0;
	m_btIsCastleOccupied = 0;

	memset(m_szCastleOwnerGuild,0x00,sizeof(m_szCastleOwnerGuild));

	m_bEnterHuntZone = 1; //TRIALS OPEN

	m_dwNPC_DBSAVE_TICK_COUNT = GetTickCount();
	m_dwCALC_LEFTSIEGE_TICK_COUNT = 0;
	m_dwEVENT_MSG_TICK_COUNT = 0;
	m_dwCHECK_GATE_ALIVE_COUNT = 0;
	m_dwCS_LEFTTIME_TICK_COUNT = 0;
	m_dwCS_STARTTIME_TICK_COUNT = 0;
	m_dwCS_JOINSIDE_REFRESH_TICK_COUNT = 0;
	m_dwCS_JOINSIDE_REFRESH_TOTAL_COUNT = 0;
	m_dwCS_MINIMAP_SEND_TICK_COUNT = 0;
	m_bCastleTowerAccessable = 0;
	m_bRegCrownAvailable = 0;
	m_iCastleCrownAccessUser = -1;
	m_btCastleCrownAccessUserX = 0;
	m_btCastleCrownAccessUserY = 0;
	m_iCastleSwitchAccessUser1 = -1;
	m_iCastleSwitchAccessUser2 = -1;
	m_dwCrownAccessTime = 0;

	memset(m_szMiddleWinnerGuild,0x00,sizeof(m_szMiddleWinnerGuild));

	//EnterCriticalSection(&m_critScheduleData);
	m_vtScheduleData.clear();
	//LeaveCriticalSection(&m_critScheduleData);

	//EnterCriticalSection(&m_critNpcData);
	m_vtNpcData.clear();
	//LeaveCriticalSection(&m_critNpcData);

	m_bDbNpcCreated = 0;

	ResetAllUserCsJoinSide();
	//DebugLog("%s END",__FUNCTION__);
}

void CCastleSiege::ResetAllUserCsJoinSide()
{
	//DebugLog("%s START",__FUNCTION__);
	//EnterCriticalSection(&m_critCsTotalGuildInfo);

	__try
	{

		for(int iUSER = OBJ_STARTUSERINDEX; iUSER < OBJMAX; iUSER++)
		{
			if(::gObjIsConnected(iUSER) == FALSE)
				continue;

			gObj[iUSER].m_btCsJoinSide = 0;
			//gObj[iUSER].m_bCsGuildInvolved = false;
			gObj[iUSER].m_btLifeStoneCount = 0;

			NotifySelfCsJoinSide(iUSER);
		}
	} __finally {
		//LeaveCriticalSection(&m_critCsTotalGuildInfo);
		//DebugLog("%s END",__FUNCTION__);
	}
}

struct PMSG_ANS_NOTIFYCSJOINSIDE
{
	PBMSG_HEAD2 h;
	unsigned char btCsJoinSide;
};

void CCastleSiege::NotifySelfCsJoinSide(int iIndex)
{
	//DebugLog("%s START",__FUNCTION__);
	__try
	{
		if(gObjIsConnected(iIndex) == FALSE)
			return;

		unsigned char btCsJoinSide = gObj[iIndex].m_btCsJoinSide;

		PMSG_ANS_NOTIFYCSJOINSIDE pMsg;

		if(btCsJoinSide < 0)
		{
			btCsJoinSide = 0;
		}

		if(btCsJoinSide > 2)
		{
			btCsJoinSide = 2;
		}

		pMsg.h.set((PBYTE)&pMsg,0xB2,0x19,sizeof(pMsg));
		pMsg.btCsJoinSide = btCsJoinSide;
		DataSend(iIndex,(PBYTE)&pMsg,pMsg.h.size);

		switch (gObj[iIndex].m_btCsJoinSide)
		{
			case 0x01:
				GCSkillInfoSend(&gObj[iIndex], 1, 0x0E);
			break;
			case 0x02:
				GCSkillInfoSend(&gObj[iIndex], 1, 0x0F);
			break;
			case 0x03:
				GCSkillInfoSend(&gObj[iIndex], 1, 0x10);
			break;
			case 0x04:
				GCSkillInfoSend(&gObj[iIndex], 1, 0x11);
			break;
		}
	}__finally
	{
		//DebugLog("%s END",__FUNCTION__);
	}
}

int CCastleSiege::Ready(int iMapSvrGroup)
{
	//DebugLog("%s START",__FUNCTION__);
	__try
	{
		if(iMapSvrGroup < 0)
			return false;

		Clear();
		ClearAllNPC();

		m_iMapSvrGroup = iMapSvrGroup;
		m_iCastleDataLoadState = 1;

		//DebugLog("%s END",__FUNCTION__);
		return true;
	}__finally
	{
		//DebugLog("%s END",__FUNCTION__);
	}
}

void CCastleSiege::ClearAllNPC()
{
	//DebugLog("%s START",__FUNCTION__);
	__try
	{
		for(int n = 0; n < OBJ_MAXMONSTER; n++)
		{
			if(gObj[n].m_btCsNpcType)
			{
				gObjDel(n);
			}
		}
	}__finally
	{
	//DebugLog("%s END",__FUNCTION__);
	}
}

int CCastleSiege::LoadPreFixData(char * lpszFileName)
{
		//DebugLog("%s START",__FUNCTION__);
		__try
		{
		if(lpszFileName == 0)
		{
			//DebugLog("%s END lpszFileName == 0",__FUNCTION__);
			return false;
		}

		_SYSTEMTIME tmToDay;

		GetLocalTime(&tmToDay);

		int iTODAY_DATE_NUM = MACRO2(( MACRO1(tmToDay.wDay) | MACRO1(tmToDay.wMonth) << 8 ) & 0xFFFF ) | MACRO2(tmToDay.wYear) << 16;
		__int64 iTODAY_DATE_NUM64 = (MACRO2(tmToDay.wMinute) | MACRO2(tmToDay.wHour) << 16) | ((__int64)(MACRO2(( MACRO1(tmToDay.wDay) | MACRO1(tmToDay.wMonth) << 8 ) & 0xFFFF ) | MACRO2(tmToDay.wYear) << 16) << 0x20);

		m_bCastleSiegeEnable = GetPrivateProfileInt("GameServerInfo","CastleSiegeRun",1,lpszFileName);
		m_tmFixCastleCycleStartDate.wYear = GetPrivateProfileInt("GameServerInfo","CastleCycleStartYear",0,lpszFileName);
		m_tmFixCastleCycleStartDate.wMonth = GetPrivateProfileInt("GameServerInfo","CastleCycleStartMonth",0,lpszFileName);
		m_tmFixCastleCycleStartDate.wDay = GetPrivateProfileInt("GameServerInfo","CastleCycleStartDay",0,lpszFileName);

		m_iFixCastleSpecificState = GetPrivateProfileInt("GameServerInfo","CastleSpecificState",0,lpszFileName);

		m_tmFixCastleStateStartDate.wYear = GetPrivateProfileInt("GameServerInfo","CastleStateStartYear",0,lpszFileName);
		m_tmFixCastleStateStartDate.wMonth = GetPrivateProfileInt("GameServerInfo","CastleStateStartMonth",0,lpszFileName);
		m_tmFixCastleStateStartDate.wDay = GetPrivateProfileInt("GameServerInfo","CastleStateStartDay",0,lpszFileName);
		m_tmFixCastleStateStartDate.wHour = GetPrivateProfileInt("GameServerInfo","CastleStateStartHour",0,lpszFileName);
		m_tmFixCastleStateStartDate.wMinute = GetPrivateProfileInt("GameServerInfo","CastleStateStartMinute",0,lpszFileName);

		if(m_tmFixCastleCycleStartDate.wYear == 0
			|| m_tmFixCastleCycleStartDate.wMonth == 0
			|| m_tmFixCastleCycleStartDate.wDay == 0)
		{
			m_bFixCastleCycleStartDate = 0;
		}
		else
		{
			_SYSTEMTIME tmEndDate = m_tmFixCastleCycleStartDate;
			GetNextDay(&tmEndDate,m_iCastleSiegeCycle,0,0,0);

			int iEVENT_END_DATE_NUM = MACRO2(( MACRO1(tmEndDate.wDay) | MACRO1(tmEndDate.wMonth) << 8 ) & 0xFFFF ) | MACRO2(tmEndDate.wYear) << 16;

			if(iEVENT_END_DATE_NUM < iTODAY_DATE_NUM)
			{
				m_bFixCastleCycleStartDate = 0;
				MsgBox("[CastleSiege] CCastleSiege::LoadPreFixData() - iEVENT_END_DATE_NUM (%04d-%02d-%02d) < iTODAY_DATE_NUM (%04d-%02d-%02d)",
					tmEndDate.wYear,tmEndDate.wMonth,tmEndDate.wDay,tmToDay.wYear,tmToDay.wMonth,tmToDay.wDay);
			}
			else
			{
				m_bFixCastleCycleStartDate = 1;
			}

			if(MONTH_MACRO(m_tmFixCastleCycleStartDate.wMonth) == FALSE
			|| DAY_MACRO(m_tmFixCastleCycleStartDate.wDay) == FALSE)
			{
				m_bFixCastleCycleStartDate = 0;
			}
		}

		if(m_tmFixCastleStateStartDate.wYear == 0
			|| m_tmFixCastleStateStartDate.wMonth == 0
			|| m_tmFixCastleStateStartDate.wDay == 0)
		{
			m_bFixCastleStateStartDate = 0;
		}
		else if(m_iFixCastleSpecificState < 1 || m_iFixCastleSpecificState > 9)
		{
			m_bFixCastleStateStartDate = 0;
		}
		else
		{
			__int64 iSIEGE_DATE_NUM64 = (MACRO2(m_tmFixCastleStateStartDate.wMinute) | MACRO2(m_tmFixCastleStateStartDate.wHour) << 16) | ((__int64)(MACRO2(( MACRO1(m_tmFixCastleStateStartDate.wDay) | MACRO1(m_tmFixCastleStateStartDate.wMonth) << 8 ) & 0xFFFF ) | MACRO2(m_tmFixCastleStateStartDate.wYear) << 16) << 0x20);

			if(iSIEGE_DATE_NUM64 < iTODAY_DATE_NUM64)
			{
				m_bFixCastleStateStartDate = 0;
				MsgBox("[CastleSiege] CCastleSiege::LoadPreFixData() - iSIEGE_DATE_NUM64 (STATE:%d, %04d-%02d-%02d %02d:%02d:%02d) < iTODAY_DATE_NUM64 (%04d-%02d-%02d %02d:%02d:%02d)",
					m_iFixCastleSpecificState,m_tmFixCastleStateStartDate.wYear,m_tmFixCastleStateStartDate.wMonth,m_tmFixCastleStateStartDate.wDay,m_tmFixCastleStateStartDate.wHour,m_tmFixCastleStateStartDate.wMinute,m_tmFixCastleStateStartDate.wSecond,
					tmToDay.wYear,tmToDay.wMonth,tmToDay.wDay,tmToDay.wHour,tmToDay.wMinute,tmToDay.wSecond);
			}
			else
			{
				m_bFixCastleStateStartDate = 1;
			}

			if(m_iFixCastleSpecificState == 8)
			{
				MsgBox("[CastleSiege] CCastleSiege::LoadPreFixData() - FIX STATE 'CASTLESIEGE_STATE_ENDSIEGE' CAN'T BE USED");
				m_bFixCastleStateStartDate = 0;
			}

			if(MONTH_MACRO(m_tmFixCastleStateStartDate.wMonth) == FALSE
				|| DAY_MACRO(m_tmFixCastleStateStartDate.wDay) == FALSE
				|| HOUR_MACRO(m_tmFixCastleStateStartDate.wHour) == FALSE
				|| MINUTE_MACRO(m_tmFixCastleStateStartDate.wMinute) == FALSE)
			{
				m_bFixCastleStateStartDate = 0;
			}
		}

		WritePrivateProfileString("GameServerInfo","CastleCycleStartYear","0",lpszFileName);
		WritePrivateProfileString("GameServerInfo","CastleCycleStartMonth","0",lpszFileName);
		WritePrivateProfileString("GameServerInfo","CastleCycleStartDay","0",lpszFileName);
		WritePrivateProfileString("GameServerInfo","CastleSpecificState","0",lpszFileName);
		WritePrivateProfileString("GameServerInfo","CastleStateStartYear","0",lpszFileName);
		WritePrivateProfileString("GameServerInfo","CastleStateStartMonth","0",lpszFileName);
		WritePrivateProfileString("GameServerInfo","CastleStateStartDay","0",lpszFileName);
		WritePrivateProfileString("GameServerInfo","CastleStateStartHour","0",lpszFileName);
		WritePrivateProfileString("GameServerInfo","CastleStateStartMinute","0",lpszFileName);

		//DebugLog("%s END",__FUNCTION__);
		return m_bFixCastleCycleStartDate & m_bFixCastleStateStartDate;
	}__finally
	{
		//DebugLog("%s END",__FUNCTION__);
	}

	//this->SetState(CASTLESIEGE_STATE_ENDCYCLE, 1);
	//add statereset somewhere......
}

void CCastleSiege::GetNextDay(_SYSTEMTIME * st,int iAfterDay,int iAfterHour,int iAfterMin,int iAfterSec)
{
	//DebugLog("%s START",__FUNCTION__);
	__try
	{
		__int64 i64;
		_FILETIME fst;
		st->wDayOfWeek = 0;
		st->wMilliseconds = 0;
		st->wSecond = 0;
		SystemTimeToFileTime(st,&fst);
		i64 = ((__int64)fst.dwHighDateTime << 0x20) + fst.dwLowDateTime;
		i64 = i64 + (__int64)iAfterDay * 864000000000 + (__int64)iAfterHour * 36000000000 +(__int64) iAfterMin * 600000000 + (__int64)iAfterSec * 10000000;
		fst.dwHighDateTime = i64 >> 0x20;
		fst.dwLowDateTime = i64 & 0xFFFFFFFF;
		FileTimeToSystemTime(&fst,st);
	}__finally
	{
		//DebugLog("%s END",__FUNCTION__);
	}
}

int CCastleSiege::LoadData(char * lpszFileName)
{
	//DebugLog("%s START",__FUNCTION__);
	using namespace std;

	if(lpszFileName == NULL || strcmp(lpszFileName,"")==NULL)
	{
		MsgBox("[CastleSiege] file load error - File Name Error");
		return false;
	}

	if(m_iCastleDataLoadState != 1)
	{
		LogAddC(2,"[CastleSiege] CCastleSiege::LoadData() - m_iCastleDataLoadState != CASTLESIEGE_DATALOAD_1 (%d)",m_iCastleDataLoadState);
		return false;
	}

	SMDFile = fopen(lpszFileName,"r");

	if(SMDFile == NULL)
	{
		MsgBox("[CastleSiege] file load error - fopen() : %s",lpszFileName);
		return false;
	}

	int type = -1;
	enum SMDToken Token;

	vector<_CS_SCHEDULE_DATA> vtScheduleData;
	vector<_CS_NPC_DATA> vtNpcData;
	map<unsigned int,int> mpNpcDataCheck;

	int iCountDfUpgrade_Gate = 0;
	int iCountDfUpgrade_Statue = 0;
	int iCountHpUpgrade_Gate = 0;
	int iCountHpUpgrade_Statue = 0;
	int iCountRgUpgrade_Statue = 0;
	int iCountDfValue_Gate = 0;
	int iCountDfValue_Statue = 0;

	while( true )
	{
		Token = GetToken();

		if(Token == 2)
		{
			break;
		}

		type = (int)TokenNumber;

		while ( true )
		{
			if(type == 0)
			{
				Token = GetToken();
				if(strcmp("end",TokenString)==NULL)
				{
					break;
				}

				int iCastleSiegeCycle = (int)TokenNumber;

				if(iCastleSiegeCycle <= 0)
				{
					LogAddC(2,"[CastleSiege] CCastleSiege::LoadData() - iCastleSiegeCycle <= 0 : %d",iCastleSiegeCycle);
					return false;
				}
				else
				{
					m_iCastleSiegeCycle = iCastleSiegeCycle;
				}
			}
			else if(type == 1)
			{
				int iSTATE = -1;
				int iSTART_DAY = -1;
				int iSTART_HOUR = -1;
				int iSTART_MIN = -1;

				Token = GetToken();
				if(strcmp("end",TokenString)==NULL)
				{
					break;
				}

				iSTATE = (int)TokenNumber;
				Token = GetToken();
				iSTART_DAY = (int)TokenNumber;
				Token = GetToken();
				iSTART_HOUR = (int)TokenNumber;
				Token = GetToken();
				iSTART_MIN = (int)TokenNumber;

				if(!vtScheduleData.empty())
				{
					//for(_CS_SCHEDULE_DATA * it = vtScheduleData.begin(); it != vtScheduleData.end(); it++)
					for(std::vector<_CS_SCHEDULE_DATA>::iterator it = vtScheduleData.begin(); it != vtScheduleData.end(); it++)
					{
						_CS_SCHEDULE_DATA pScheduleData = _CS_SCHEDULE_DATA(*it);

						if(pScheduleData.m_bIN_USE != 0)
						{
							if(pScheduleData.m_iSTATE == iSTATE)
							{
								LogAddC(2,"[CastleSiege] CCastleSiege::LoadData() - Same State Exist : %d",iSTATE);
								return false;
							}
							
							if(pScheduleData.m_iSTATE > iSTATE)
							{
								int iSTATE1_TIME = pScheduleData.m_iADD_DAY * 24 * 60 + pScheduleData.m_iADD_HOUR * 60 + pScheduleData.m_iADD_MIN;
								int iSTATE2_TIME = iSTART_DAY * 24 * 60 + iSTART_HOUR * 60 + iSTART_MIN;
	
								if(iSTATE1_TIME <= iSTATE2_TIME)
								{
									LogAddC(2,"[CastleSiege] CCastleSiege::LoadData() - Date Order is wrong : %d",iSTATE2_TIME);
									return false;
								}
							}
							else
							{
								int iSTATE1_TIME = pScheduleData.m_iADD_DAY * 24 * 60 + pScheduleData.m_iADD_HOUR * 60 + pScheduleData.m_iADD_MIN;
								int iSTATE2_TIME = iSTART_DAY * 24 * 60 + iSTART_HOUR * 60 + iSTART_MIN;
								if(iSTATE1_TIME >= iSTATE2_TIME)
								{
									LogAddC(2,"[CastleSiege] CCastleSiege::LoadData() - Date Order is wrong : %d",iSTATE2_TIME);
									return false;
								}
							}
						}
					}
				}

				_CS_SCHEDULE_DATA pScheduleDataInsert;

				pScheduleDataInsert.m_bIN_USE = 1;
				pScheduleDataInsert.m_iSTATE = iSTATE;
				pScheduleDataInsert.m_iADD_DAY = iSTART_DAY;
				pScheduleDataInsert.m_iADD_HOUR = iSTART_HOUR;
				pScheduleDataInsert.m_iADD_MIN = iSTART_MIN;

				vtScheduleData.push_back(pScheduleDataInsert);
			}
			else if(type == 2)
			{
				int iNPC_NUM = -1;
				int iNPC_INDEX = -1;
				int iNPC_SIDE = 0;
				int iNPC_DBSAVE = 0;
				int iNPC_DF_LEVEL = 0;
				int iNPC_RG_LEVEL = 0;
				int iNPC_MAXHP = 0;
				int iNPC_SX = -1;
				int iNPC_SY = -1;
				int iNPC_DX = -1;
				int iNPC_DY = -1;
				int iNPC_DIR = -1;

				Token = GetToken();
				if(strcmp("end",TokenString)==NULL)
				{
					break;
				}

				iNPC_NUM = (int)TokenNumber;
				Token = GetToken();
				iNPC_INDEX = (int)TokenNumber;
				Token = GetToken();
				iNPC_DBSAVE = (int)TokenNumber;
				Token = GetToken();
				iNPC_SIDE = (int)TokenNumber;
				Token = GetToken();
				iNPC_DF_LEVEL = (int)TokenNumber;
				Token = GetToken();
				iNPC_RG_LEVEL = (int)TokenNumber;
				Token = GetToken();
				iNPC_MAXHP = (int)TokenNumber;
				Token = GetToken();
				iNPC_SX = (int)TokenNumber;
				Token = GetToken();
				iNPC_SY = (int)TokenNumber;
				Token = GetToken();
				iNPC_DX = (int)TokenNumber;
				Token = GetToken();
				iNPC_DY = (int)TokenNumber;
				Token = GetToken();
				iNPC_DIR = (int)TokenNumber;


				if(iNPC_NUM < 0)
				{
					MsgBox("[CastleSiege] CCastleSiege::LoadData() - Data Fail (iNPC_NUM < 0)");
					return false;
				}

				if(iNPC_INDEX < 0)
				{
					MsgBox("[CastleSiege] CCastleSiege::LoadData() - Data Fail (iNPC_INDEX < 0)");
					return false;
				}

				if(iNPC_SIDE < 0)
				{
					MsgBox("[CastleSiege] CCastleSiege::LoadData() - Data Fail (iNPC_SIDE < 0)");
					return false;
				}

				if(iNPC_DF_LEVEL < 0)
				{
					MsgBox("[CastleSiege] CCastleSiege::LoadData() - Data Fail (iNPC_DF_LEVEL < 0)");
					return false;
				}

				if(iNPC_RG_LEVEL < 0)
				{
					MsgBox("[CastleSiege] CCastleSiege::LoadData() - Data Fail (iNPC_RG_LEVEL < 0)");
					return false;
				}

				if(iNPC_MAXHP < 0)
				{
					MsgBox("[CastleSiege] CCastleSiege::LoadData() - Data Fail (iNPC_MAXHP < 0)");
					return false;
				}

				if(iNPC_SX < 0)
				{
					MsgBox("[CastleSiege] CCastleSiege::LoadData() - Data Fail (iNPC_SX < 0)");
					return false;
				}

				if(iNPC_SY < 0)
				{
					MsgBox("[CastleSiege] CCastleSiege::LoadData() - Data Fail (iNPC_SY < 0)");
					return false;
				}

				unsigned int uNpcKey = MACRO2(iNPC_INDEX) | (MACRO2(iNPC_NUM) << 16);

				//map<unsigned int,int>::iterator it = (map<unsigned int,int>::iterator)mpNpcDataCheck.find(uNpcKey);
				map<unsigned int,int>::iterator it = mpNpcDataCheck.find(uNpcKey);

				if(it != mpNpcDataCheck.end())
				{
					MsgBox("[CastleSiege] CCastleSiege::LoadData() - Same Npc Key Exist : NUM:%d-IDX:%d",iNPC_NUM,iNPC_INDEX);
					return false;
				}

				mpNpcDataCheck.insert(pair <unsigned int, int > ( ( MACRO2(iNPC_INDEX) | (MACRO2(iNPC_NUM) << 16) ), 0));

				_CS_NPC_DATA pNpcDataInsert;

				pNpcDataInsert.m_bIN_USE = 1;
				pNpcDataInsert.m_iNPC_NUM = iNPC_NUM;
				pNpcDataInsert.m_iNPC_INDEX = iNPC_INDEX;
				pNpcDataInsert.m_iNPC_SIDE = iNPC_SIDE;
				pNpcDataInsert.m_bNPC_DBSAVE = iNPC_DBSAVE;

				pNpcDataInsert.m_iNPC_BASE_DF_LEVEL = iNPC_DF_LEVEL;
				pNpcDataInsert.m_iNPC_DF_LEVEL = pNpcDataInsert.m_iNPC_BASE_DF_LEVEL;

				pNpcDataInsert.m_iNPC_BASE_RG_LEVEL = iNPC_RG_LEVEL;
				pNpcDataInsert.m_iNPC_RG_LEVEL = pNpcDataInsert.m_iNPC_BASE_RG_LEVEL;

				pNpcDataInsert.m_iNPC_BASE_MAXHP = iNPC_MAXHP;
				pNpcDataInsert.m_iNPC_MAXHP = pNpcDataInsert.m_iNPC_BASE_MAXHP;

				pNpcDataInsert.m_iNPC_BASE_HP = iNPC_MAXHP;
				pNpcDataInsert.m_iNPC_HP = pNpcDataInsert.m_iNPC_BASE_HP;

				pNpcDataInsert.m_iNPC_BASE_SX = iNPC_SX;
				pNpcDataInsert.m_iNPC_SX = pNpcDataInsert.m_iNPC_BASE_SX;

				pNpcDataInsert.m_iNPC_BASE_SY = iNPC_SY;
				pNpcDataInsert.m_iNPC_SY = pNpcDataInsert.m_iNPC_BASE_SY;

				pNpcDataInsert.m_iNPC_BASE_DX = iNPC_DX;
				pNpcDataInsert.m_iNPC_DX = pNpcDataInsert.m_iNPC_BASE_DX;

				pNpcDataInsert.m_iNPC_BASE_DY = iNPC_DY;
				pNpcDataInsert.m_iNPC_DY = pNpcDataInsert.m_iNPC_BASE_DY;

				pNpcDataInsert.m_iNPC_BASE_DIR = iNPC_DIR;
				pNpcDataInsert.m_iNPC_DIR = pNpcDataInsert.m_iNPC_BASE_DIR;

				vtNpcData.push_back(pNpcDataInsert);
			}
			else if(type == 3)
			{
				int iLEVEL = 0;
				int iGEMCOUNT = 0;
				int iZEN = 0;

				Token = GetToken();
				if(strcmp("end",TokenString)==NULL)
				{
					break;
				}

				iLEVEL = (int)TokenNumber;
				Token = GetToken();
				iGEMCOUNT = (int)TokenNumber;
				Token = GetToken();
				iZEN = (int)TokenNumber;

				if(iCountDfUpgrade_Gate >= CS_MAX_UPGRADE)
					break;

				g_iNpcUpDfLevel_CGATE[iCountDfUpgrade_Gate][0] = iLEVEL;
				g_iNpcUpDfLevel_CGATE[iCountDfUpgrade_Gate][1] = iGEMCOUNT;
				g_iNpcUpDfLevel_CGATE[iCountDfUpgrade_Gate][2] = iZEN;

				iCountDfUpgrade_Gate++;
			}
			else if(type == 4)
			{
				int iLEVEL = 0;
				int iGEMCOUNT = 0;
				int iZEN = 0;

				Token = GetToken();
				if(strcmp("end",TokenString)==NULL)
				{
					break;
				}

				iLEVEL = (int)TokenNumber;
				Token = GetToken();
				iGEMCOUNT = (int)TokenNumber;
				Token = GetToken();
				iZEN = (int)TokenNumber;

				if(iCountDfUpgrade_Statue >= CS_MAX_UPGRADE)
					break;

				g_iNpcUpDfLevel_CSTATUE[iCountDfUpgrade_Statue][0] = iLEVEL;
				g_iNpcUpDfLevel_CSTATUE[iCountDfUpgrade_Statue][1] = iGEMCOUNT;
				g_iNpcUpDfLevel_CSTATUE[iCountDfUpgrade_Statue][2] = iZEN;

				iCountDfUpgrade_Statue++;
			}
			else if(type == 5)
			{
				int iHP = 0;
				int iGEMCOUNT = 0;
				int iZEN = 0;

				Token = GetToken();
				if(strcmp("end",TokenString)==NULL)
				{
					break;
				}

				iHP = (int)TokenNumber;
				Token = GetToken();
				iGEMCOUNT = (int)TokenNumber;
				Token = GetToken();
				iZEN = (int)TokenNumber;

				if(iCountHpUpgrade_Gate >= CS_MAX_UPGRADE)
					break;

				g_iNpcUpMaxHP_CGATE[iCountHpUpgrade_Gate][0] = iHP;
				g_iNpcUpMaxHP_CGATE[iCountHpUpgrade_Gate][1] = iGEMCOUNT;
				g_iNpcUpMaxHP_CGATE[iCountHpUpgrade_Gate][2] = iZEN;

				iCountHpUpgrade_Gate++;
			}
			else if(type == 6)
			{
				int iHP = 0;
				int iGEMCOUNT = 0;
				int iZEN = 0;

				Token = GetToken();
				if(strcmp("end",TokenString)==NULL)
				{
					break;
				}

				iHP = (int)TokenNumber;
				Token = GetToken();
				iGEMCOUNT = (int)TokenNumber;
				Token = GetToken();
				iZEN = (int)TokenNumber;

				if(iCountHpUpgrade_Statue >= CS_MAX_UPGRADE)
					break;

				g_iNpcUpMaxHP_CSTATUE[iCountHpUpgrade_Statue][0] = iHP;
				g_iNpcUpMaxHP_CSTATUE[iCountHpUpgrade_Statue][1] = iGEMCOUNT;
				g_iNpcUpMaxHP_CSTATUE[iCountHpUpgrade_Statue][2] = iZEN;

				iCountHpUpgrade_Statue++;
			}
			else if(type == 7)
			{
				int iLEVEL = 0;
				int iGEMCOUNT = 0;
				int iZEN = 0;

				Token = GetToken();
				if(strcmp("end",TokenString)==NULL)
				{
					break;
				}

				iLEVEL = (int)TokenNumber;
				Token = GetToken();
				iGEMCOUNT = (int)TokenNumber;
				Token = GetToken();
				iZEN = (int)TokenNumber;

				if(iCountRgUpgrade_Statue >= CS_MAX_UPGRADE)
					break;

				g_iNpcUpRgLevel_CSTATUE[iCountRgUpgrade_Statue][0] = iLEVEL;
				g_iNpcUpRgLevel_CSTATUE[iCountRgUpgrade_Statue][1] = iGEMCOUNT;
				g_iNpcUpRgLevel_CSTATUE[iCountRgUpgrade_Statue][2] = iZEN;

				iCountRgUpgrade_Statue++;
			}
			else if(type == 8)
			{
				int iDFVALUE = 0;

				Token = GetToken();
				if(strcmp("end",TokenString)==NULL)
				{
					break;
				}

				iDFVALUE = (int)TokenNumber;

				if(iCountDfValue_Gate >= CS_MAX_UPGRADE + 1)
					break;

				g_iNpcDefense_CGATE[iCountDfValue_Gate] = iDFVALUE;

				iCountDfValue_Gate++;
			}
			else if(type == 9)
			{
				int iDFVALUE = 0;

				Token = GetToken();
				if(strcmp("end",TokenString)==NULL)
				{
					break;
				}

				iDFVALUE = (int)TokenNumber;

				if(iCountDfValue_Statue >= CS_MAX_UPGRADE + 1)
					break;

				g_iNpcDefense_CSTATUE[iCountDfValue_Statue] = iDFVALUE;

				iCountDfValue_Statue++;
			}
		}
	}

	fclose(SMDFile);

	//EnterCriticalSection(&m_critScheduleData);

	if(!vtScheduleData.empty())
	{
		//for(_CS_SCHEDULE_DATA * it = vtScheduleData.begin(); it != vtScheduleData.end(); it++)
		for(std::vector<_CS_SCHEDULE_DATA>::iterator it = vtScheduleData.begin(); it != vtScheduleData.end(); it++)
		{
			m_vtScheduleData.push_back(*it);
		}
	}

	sort(m_vtScheduleData.begin(),m_vtScheduleData.end(),ScheduleStateCompFunc);

	if(!m_vtScheduleData.empty())
	{
		for(int iIDX = 0; iIDX < m_vtScheduleData.size(); iIDX++)
		{
			if(iIDX+1 < m_vtScheduleData.size())
			{
				int iIDX_SEC1 = m_vtScheduleData[iIDX].m_iADD_DAY * 24 * 60 * 60 + m_vtScheduleData[iIDX].m_iADD_HOUR * 60 * 60 + m_vtScheduleData[iIDX].m_iADD_MIN * 60;
				int iIDX_SEC2 = m_vtScheduleData[iIDX+1].m_iADD_DAY * 24 * 60 * 60 + m_vtScheduleData[iIDX+1].m_iADD_HOUR * 60 * 60 + m_vtScheduleData[iIDX+1].m_iADD_MIN * 60;

				int iIDX_RESULT = iIDX_SEC2 - iIDX_SEC1;

				if(iIDX_RESULT < 0)
				{
					LogAddC(2,"[CastleSiege] CCastleSiege::LoadData() - Date Order is wrong (sort fail) : %d-%d",
						m_vtScheduleData[iIDX].m_iSTATE,m_vtScheduleData[iIDX+1].m_iSTATE);
				}
				else
				{
					m_vtScheduleData[iIDX].m_iGAP_SEC = iIDX_RESULT;
				}
			}
			else
			{
				m_vtScheduleData[iIDX].m_iGAP_SEC = 0;
			}
		}
	}

	//LeaveCriticalSection(&m_critScheduleData);

	//EnterCriticalSection(&m_critNpcData);

	if(!vtNpcData.empty())
	{
		//for(_CS_NPC_DATA * it = vtNpcData.begin();it != vtNpcData.end(); it++)
		for(std::vector<_CS_NPC_DATA>::iterator it = vtNpcData.begin();it != vtNpcData.end(); it++) 
		{
			//LogAddTD("[CastleSiege][LoadData] Found NPC Index:%d Num:%d", it->m_iNPC_INDEX, it->m_iNPC_NUM);
			m_vtNpcData.push_back(*it);
		}
	}

	//LeaveCriticalSection(&m_critNpcData);

	//_CS_SCHEDULE_DATA * it1 = m_vtScheduleData.begin();
	std::vector<_CS_SCHEDULE_DATA>::iterator it1 = m_vtScheduleData.begin();

	while(it1 != m_vtScheduleData.end())
	{
		_CS_SCHEDULE_DATA stTEMP = *it1;
		char szTEMP[0x100] = {0};

		wsprintf(szTEMP,"STT:%d, DAY:%d, HOUR:%d, MIN:%d, GAP:%d\n",
			stTEMP.m_iSTATE,stTEMP.m_iADD_DAY,stTEMP.m_iADD_HOUR,stTEMP.m_iADD_MIN,stTEMP.m_iGAP_SEC);

		OutputDebugString(szTEMP);
		it1++;
	}

	//_CS_NPC_DATA * it2 = this->m_vtNpcData.begin();
	std::vector<_CS_NPC_DATA>::iterator it2 = this->m_vtNpcData.begin();

	while(it2 != this->m_vtNpcData.end())
	{
		_CS_NPC_DATA stTEMP = *it2;
		char szTEMP[0x100] = {0};

		wsprintf(szTEMP,"NUM:%d, IDX:%d, DBSAVE:%s, SIDE:%d, DFLEVEL:%d, RGLEVEL:%d, MXHP:%d, (%d,%d)-(%d,%d), DIR:%d\n",
			stTEMP.m_iNPC_NUM,stTEMP.m_iNPC_INDEX,(stTEMP.m_bNPC_DBSAVE != 0) ? "YES" : "NO",stTEMP.m_iNPC_SIDE,stTEMP.m_iNPC_DF_LEVEL,
			stTEMP.m_iNPC_RG_LEVEL,stTEMP.m_iNPC_MAXHP,stTEMP.m_iNPC_SX,stTEMP.m_iNPC_SY,stTEMP.m_iNPC_DX,stTEMP.m_iNPC_DY,stTEMP.m_iNPC_DIR);
		OutputDebugString(szTEMP);
		it2++;
	}

	m_bFileDataLoadOK = TRUE;

	//DebugLog("%s END",__FUNCTION__);
	return true;

}

unsigned char CCastleSiege::ScheduleStateCompFunc(_CS_SCHEDULE_DATA & A,_CS_SCHEDULE_DATA & B)
{
	//DebugLog("%s START",__FUNCTION__);
	__try
	{
		if(A.m_iSTATE > B.m_iSTATE)
		{
			//DebugLog("%s END A.m_iSTATE > B.m_iSTATE",__FUNCTION__);
			return false;
		}

		//DebugLog("%s END",__FUNCTION__);
		return true;
	}__finally
	{
		//DebugLog("%s END",__FUNCTION__);
	}
}

int CCastleSiege::DataRequest()
{
	//DebugLog("%s START",__FUNCTION__);
	__try
	{
		if(m_bFileDataLoadOK == FALSE)
		{
			LogAddC(2,"[CastleSiege] CCastleSiege::DataRequest() - m_bFileDataLoadOK == FALSE");
			return false;
		}

		if(m_iMapSvrGroup < 0)
		{
			LogAddC(2,"[CastleSiege] CCastleSiege::DataRequest() - m_iMapSvrGroup < 0");
			return false;
		}

		if(m_iCastleDataLoadState != CASTLESIEGE_DATALOAD_2)
		{
			LogAddC(2,"[CastleSiege] CCastleSiege::DataRequest() - m_iCastleDataLoadState != CASTLESIEGE_DATALOAD_2 (%d)",m_iCastleDataLoadState);
			return false;
		}

		int bDbRequestData = FALSE;

		if(m_dwDbDataLoadTickCount == 0)
		{
			bDbRequestData = TRUE;
			m_dwDbDataLoadTickCount = GetTickCount();
			LogAddC(3,"[CastleSiege] CCastleSiege::DataRequest() - REQUEST DATA FIRST");
		}
		else if(GetTickCount() - m_dwDbDataLoadTickCount > 3000)
		{
			bDbRequestData = TRUE;
			m_dwDbDataLoadTickCount = GetTickCount();
			LogAddC(2,"[CastleSiege] CCastleSiege::DataRequest() - REQUEST DATA AGAIN");
		}

		if(bDbRequestData != FALSE)
		{
			GS_GDReqCastleInitData(m_iMapSvrGroup,m_iCastleSiegeCycle);
		}

		return true;
	}__finally
	{
		//DebugLog("%s END",__FUNCTION__);
	}
}

int CCastleSiege::Init()
{
	//DebugLog("%s START",__FUNCTION__);
	__try
	{
		if(m_iCastleDataLoadState != CASTLESIEGE_DATALOAD_4)
		{
			LogAddC(2,"[CastleSiege] CCastleSiege::Init() - m_iCastleDataLoadState != CASTLESIEGE_DATALOAD_4 (%d)",m_iCastleDataLoadState);
			MsgBox("[CastleSiege] CCastleSiege::Init() - m_iCastleDataLoadState != CASTLESIEGE_DATALOAD_4 (%d)",m_iCastleDataLoadState);
			return false;
		}

		if(m_bFileDataLoadOK == 0 || m_bDbDataLoadOK == 0)
		{
			LogAddC(2,"[CastleSiege] CCastleSiege::Init() - Data Load Fail (FILE:%d, DB:%d)",
				m_bFileDataLoadOK,m_bDbDataLoadOK);
			MsgBox("[CastleSiege] CCastleSiege::Init() - Data Load Fail (FILE:%d, DB:%d)",
				m_bFileDataLoadOK,m_bDbDataLoadOK);
			return false;
		}

		LogAddTD("[CastleSiege] CCastleSiege::Init() - START",m_bFileDataLoadOK,m_bDbDataLoadOK);

		m_iCastleDataLoadState = 0;

		CreateAllCsGateLever();

		if(CheckSync() == FALSE)
		{
			LogAddC(2,"[CastleSiege] CCastleSiege::Init() - CheckSync() == FALSE");
			MsgBox("[CastleSiege] CCastleSiege::Init() - CheckSync() == FALSE");
			return false;
		}

		m_bDoRun = TRUE;

		m_dwNPC_DBSAVE_TICK_COUNT = GetTickCount();
		m_dwCALC_LEFTSIEGE_TICK_COUNT = 0;
		m_dwEVENT_MSG_TICK_COUNT = 0;
		m_dwCHECK_GATE_ALIVE_COUNT = 0;
		m_dwCS_STARTTIME_TICK_COUNT = 0;
		m_dwCS_LEFTTIME_TICK_COUNT = 0;
		m_dwCS_JOINSIDE_REFRESH_TICK_COUNT = 0;
		m_dwCS_JOINSIDE_REFRESH_TOTAL_COUNT = 0;
		m_dwCS_MINIMAP_SEND_TICK_COUNT = 0;
	
		return true;
	}__finally
	{
		//DebugLog("%s END",__FUNCTION__);
	}
}

void CCastleSiege::CreateAllCsGateLever()
{
	//DebugLog("%s START",__FUNCTION__);
	//EnterCriticalSection(&m_critNpcData);

	if(!m_vtNpcData.empty())
	{
		//for(_CS_NPC_DATA * it = this->m_vtNpcData.begin(); it != m_vtNpcData.end(); it++)
		for(std::vector<_CS_NPC_DATA>::iterator it = this->m_vtNpcData.begin(); it != this->m_vtNpcData.end(); it++)
		{
			std::vector<_CS_NPC_DATA>::iterator pNpcData = it;
			//_CS_NPC_DATA & pNpcData = _CS_NPC_DATA(*it);

			if(pNpcData->m_iNPC_NUM == 277)
			{
				if(pNpcData->m_iCS_GATE_LEVER_INDEX != -1 && CheckLeverAlive(pNpcData->m_iCS_GATE_LEVER_INDEX) == 1)
				{
					continue;
				}

				int iLeverIndex = CreateCsGateLever(pNpcData->m_iNPC_SX,pNpcData->m_iNPC_SY + 4);

				if(OBJMAX_RANGE(iLeverIndex))
				{
					it->m_iCS_GATE_LEVER_INDEX = iLeverIndex;
				}
				else
				{
					it->m_iCS_GATE_LEVER_INDEX = -1;
					LogAddC(2,"[CastleSiege] CCastleSiege::CreateAllCsGateLever() ERROR - Failed to Create Gate Lever : GateIndex:%d",pNpcData->m_iNPC_INDEX);
				}
			}
		}
	}

	//LeaveCriticalSection(&m_critNpcData);
	//DebugLog("%s END",__FUNCTION__);
}

int CCastleSiege::CreateCsGateLever(int iX, int iY)
{
	__try
	{
		//DebugLog("%s START",__FUNCTION__);
		if(XY_MACRO(iX) == FALSE || XY_MACRO(iY) == FALSE)
		{
			return -1;
		}

		int iNPC_INDEX = gObjAddMonster(MAP_INDEX_CASTLESIEGE);
		LogAddTD("[CastleSiege][CreateCsGateLever] gObjAddMonster %d", iNPC_INDEX);

		if(iNPC_INDEX >= 0)
		{
			gObjSetMonster(iNPC_INDEX, 219,"CCastleSiege::CreateCsGateLever");

			gObj[iNPC_INDEX].m_PosNum = -1;
			gObj[iNPC_INDEX].X = iX;
			gObj[iNPC_INDEX].Y = iY;
			gObj[iNPC_INDEX].MapNumber = MAP_INDEX_CASTLESIEGE;
			gObj[iNPC_INDEX].TX = gObj[iNPC_INDEX].X;
			gObj[iNPC_INDEX].TY = gObj[iNPC_INDEX].Y;
			gObj[iNPC_INDEX].m_OldX = gObj[iNPC_INDEX].X;
			gObj[iNPC_INDEX].m_OldY = gObj[iNPC_INDEX].Y;
			gObj[iNPC_INDEX].Dir = 1;
			gObj[iNPC_INDEX].StartX = gObj[iNPC_INDEX].X;
			gObj[iNPC_INDEX].StartY = gObj[iNPC_INDEX].Y;
			gObj[iNPC_INDEX].DieRegen = 0;
			gObj[iNPC_INDEX].MaxRegenTime = 0;
			gObj[iNPC_INDEX].Life = 10000.0f;
			gObj[iNPC_INDEX].MaxLife = 10000.0f;
			gObj[iNPC_INDEX].m_btCsNpcType = 2;
			gObj[iNPC_INDEX].m_iCsGateLeverLinkIndex = -1;
	
			return iNPC_INDEX;
		}	

		return -1;
	}__finally
	{
		//DebugLog("%s END",__FUNCTION__);
	}
}

int CCastleSiege::CheckLeverAlive(int iLeverIndex)
{
	//DebugLog("%s START",__FUNCTION__);
	__try
	{
		if(gObjIsConnected(iLeverIndex) == FALSE || gObj[iLeverIndex].Class != 219 )
		{
			return false;
		}
		return true;
	}__finally
	{
		//DebugLog("%s END",__FUNCTION__);
	}
}

int CCastleSiege::CheckSync()
{
	//DebugLog("%s START",__FUNCTION__);
	if(m_bFileDataLoadOK == FALSE)
	{
		LogAddC(2,"[CastleSiege] CCastleSiege::CheckSync() - m_bFileDataLoadOK == FALSE");
		//DebugLog("%s END",__FUNCTION__);
		return false;
	}

	m_iCS_TICK_COUNT = GetTickCount();

	 _SYSTEMTIME tmToDay;

	 GetLocalTime(&tmToDay);
	 int iTODAY_DATE_NUM = MACRO2(( MACRO1(tmToDay.wDay) | MACRO1(tmToDay.wMonth) << 8 ) & 0xFFFF ) | MACRO2(tmToDay.wYear) << 16;
	 int iCastleInitState = 0;

	 if(m_bFixCastleCycleStartDate != 0)
	 {
		 m_tmStartDate.wYear = m_tmFixCastleCycleStartDate.wYear;
		 m_tmStartDate.wMonth = m_tmFixCastleCycleStartDate.wMonth;
		 m_tmStartDate.wDay = m_tmFixCastleCycleStartDate.wDay;

		 m_tmEndDate = m_tmStartDate;
		 GetNextDay(&m_tmEndDate,m_iCastleSiegeCycle,0,0,0);
	 }

	 int iEVENT_END_DATE_NUM = MACRO2(( MACRO1(m_tmEndDate.wDay) | MACRO1(m_tmEndDate.wMonth) << 8 ) & 0xFFFF ) | MACRO2(m_tmEndDate.wYear) << 16;

	 if(iEVENT_END_DATE_NUM <= iTODAY_DATE_NUM)
	 {
		m_bFixCastleCycleStartDate = 0;
		MsgBox("[CastleSiege] CCastleSiege::CheckSync() - iEVENT_END_DATE_NUM (%04d-%02d-%02d) <= iTODAY_DATE_NUM (%04d-%02d-%02d)",
			m_tmEndDate.wYear,m_tmEndDate.wMonth,m_tmEndDate.wDay,tmToDay.wYear,tmToDay.wMonth,tmToDay.wDay);
		//DebugLog("%s END",__FUNCTION__);
		return false;
	 }

	 if(m_bFixCastleCycleStartDate != 0)
	 {
		GS_GDReqSiegeDateChange(m_iMapSvrGroup,-1,m_tmStartDate.wYear,m_tmStartDate.wMonth,m_tmStartDate.wDay,
			m_tmEndDate.wYear,m_tmEndDate.wMonth,m_tmEndDate.wDay);
	 }

	 if(m_bFixCastleStateStartDate != 0)
	 {
		 int bSpecificStateExist = FALSE;

		 //EnterCriticalSection(&m_critScheduleData);

		 //_CS_SCHEDULE_DATA * it = m_vtScheduleData.begin();
		std::vector<_CS_SCHEDULE_DATA>::iterator it = m_vtScheduleData.begin();

		 while(it != m_vtScheduleData.end())
		 {
			 _CS_SCHEDULE_DATA & pScheduleData = (_CS_SCHEDULE_DATA &)*it;

			 if(pScheduleData.m_iSTATE == m_iFixCastleSpecificState)
			 {
				 bSpecificStateExist = TRUE;
				 break;
			 }

			 it++;
		 }
		 //LeaveCriticalSection(&m_critScheduleData);

		 if(bSpecificStateExist != FALSE)
		 {
			 //long ttSTime;
			 time_t ttSTime;
			 long ttETime;
			 tm tmETime;
			 double dResultSecond;
			 memset(&tmETime,0x00,sizeof(tmETime));

			 tmETime.tm_year = m_tmFixCastleStateStartDate.wYear - 1900;
			 tmETime.tm_mon = m_tmFixCastleStateStartDate.wMonth - 1;
			 tmETime.tm_mday = m_tmFixCastleStateStartDate.wDay;
			 tmETime.tm_hour = m_tmFixCastleStateStartDate.wHour;
			 tmETime.tm_min = m_tmFixCastleStateStartDate.wMinute;

			 time(&ttSTime);
			 ttETime = mktime(&tmETime);

			dResultSecond = difftime(ttETime,ttSTime);

			m_iCS_REMAIN_MSEC = dResultSecond * (double)1000.0;
			m_iCastleSiegeState = m_iFixCastleSpecificState -1;
			SetState(m_iCastleSiegeState,0);
			//DebugLog("%s END",__FUNCTION__);
			return true;
		 }
		 else
		 {
			 MsgBox("[CastleSiege] CCastleSiege::CheckSync() - FIXED STATE:%d NOT FOUND IN SCHEDULE (P.S.> Check File 'MuCastleData.dat', 'commonserver.cfg')",m_iFixCastleSpecificState);
			 m_bFixCastleStateStartDate = 0;
		 }
	 }

	 if(m_btIsSiegeEnded)
	 {
		SetState(CASTLESIEGE_STATE_ENDSIEGE,1);
		//DebugLog("%s END",__FUNCTION__);
		return true;
	 }

	 int iEVENT_START_DATE_NUM = MACRO2(( MACRO1(m_tmStartDate.wDay) | MACRO1(m_tmStartDate.wMonth) << 8 ) & 0xFFFF ) | MACRO2(m_tmStartDate.wYear) << 16;

	 if(iEVENT_START_DATE_NUM > iTODAY_DATE_NUM)
	 {
		SetState(CASTLESIEGE_STATE_IDLE_1,1);
		//DebugLog("%s END",__FUNCTION__);
		return true;
	 }
	 else
	 {
		 int bStateSetted = 0;
		_CS_SCHEDULE_DATA pScheData_FR;
		_CS_SCHEDULE_DATA pScheData_RR;
		//_CS_SCHEDULE_DATA * it;
		//std::vector<_CS_SCHEDULE_DATA>::iterator pScheData_FR;
		//std::vector<_CS_SCHEDULE_DATA>::iterator pScheData_RR;
		std::vector<_CS_SCHEDULE_DATA>::iterator it;

		 //EnterCriticalSection(&m_critScheduleData);

		 it = m_vtScheduleData.begin();

		 while(it != m_vtScheduleData.end())
		 {
			_CS_SCHEDULE_DATA & pScheduleData = (_CS_SCHEDULE_DATA &)*it;;
			_SYSTEMTIME tmSchduleDate;

			tmSchduleDate = m_tmStartDate;
			GetNextDay(&tmSchduleDate,pScheduleData.m_iADD_DAY,0,0,0);
			tmSchduleDate.wHour = pScheduleData.m_iADD_HOUR;
			tmSchduleDate.wMinute = pScheduleData.m_iADD_MIN;

			__int64 i64ScheduleDateNum;
			__int64 i64ToDayDateNum;

			i64ScheduleDateNum = (MACRO2(tmSchduleDate.wMinute) | MACRO2(tmSchduleDate.wHour) << 16) | ((__int64)(MACRO2(( MACRO1(tmSchduleDate.wDay) | MACRO1(tmSchduleDate.wMonth) << 8 ) & 0xFFFF ) | MACRO2(tmSchduleDate.wYear) << 16) << 0x20);
			i64ToDayDateNum = (MACRO2(tmToDay.wMinute) | MACRO2(tmToDay.wHour) << 16) | ((__int64)(MACRO2(( MACRO1(tmToDay.wDay) | MACRO1(tmToDay.wMonth) << 8 ) & 0xFFFF ) | MACRO2(tmToDay.wYear) << 16) << 0x20);

			 if(i64ScheduleDateNum > i64ToDayDateNum)
			 {
				 bStateSetted = 1;
				 pScheData_RR = pScheduleData;
				 m_iCastleSiegeState = iCastleInitState;

				 g_CastleSiegeSync.SetCastleState(m_iCastleSiegeState); 

				 break;
			 }

			 iCastleInitState = pScheduleData.m_iSTATE;
			 pScheData_FR = pScheduleData;

			 it++;
		 }
		 //LeaveCriticalSection(&m_critScheduleData);


		 _SYSTEMTIME tmSchduleDate;
	
		time_t ttSTime;
		//long ttSTime;
		long ttETime;
		struct tm tmETime;
		double dResultSecond;

		if(bStateSetted == FALSE)
		{
			MsgBox("[CastleSiege] CCastleSiege::CheckSync() - bStateSetted == FALSE  START_DATE (%04d-%02d-%02d), END_DATE (%04d-%02d-%02d)",
				m_tmStartDate.wYear,m_tmStartDate.wMonth,m_tmStartDate.wDay,m_tmEndDate.wYear,m_tmEndDate.wMonth,m_tmEndDate.wDay);
			//DebugLog("%s END",__FUNCTION__);
			return false;
		}

		tmSchduleDate = m_tmStartDate;
		GetNextDay(&tmSchduleDate,pScheData_RR.m_iADD_DAY,0,0,0);
		tmSchduleDate.wHour = pScheData_RR.m_iADD_HOUR;
		tmSchduleDate.wMinute = pScheData_RR.m_iADD_MIN;

		memset(&tmETime,0x00,sizeof(tmETime));

		tmETime.tm_year = tmSchduleDate.wYear - 1900;
		tmETime.tm_mon = tmSchduleDate.wMonth - 1;
		tmETime.tm_mday = tmSchduleDate.wDay;
		tmETime.tm_hour = tmSchduleDate.wHour;
		tmETime.tm_min = tmSchduleDate.wMinute;

		time(&ttSTime);
		ttETime = mktime(&tmETime);

		dResultSecond = difftime(ttETime,ttSTime);
		m_iCS_REMAIN_MSEC = dResultSecond * (double)1000.0;

		if(m_iCastleSiegeState == CASTLESIEGE_STATE_STARTSIEGE)
		{
			m_iCastleSiegeState = CASTLESIEGE_STATE_READYSIEGE;
			m_iCS_REMAIN_MSEC = 900000;
		}
		else if(m_iCastleSiegeState == CASTLESIEGE_STATE_READYSIEGE)
		{
			if(m_iCS_REMAIN_MSEC < 900000)
			{
				m_iCastleSiegeState = CASTLESIEGE_STATE_READYSIEGE;
				m_iCS_REMAIN_MSEC = 900000;
			}
		}

		SetState(m_iCastleSiegeState,0);
	 }

	//DebugLog("%s END",__FUNCTION__);
	return true;
}

int CCastleSiege::LinkCsGateLever(int iLeverIndex,int iGateIndex)
{
	//DebugLog("%s START",__FUNCTION__);
	__try
	{
		if(!gObjIsConnected(iLeverIndex) || gObj[iLeverIndex].Class != 219)
			return false;
		if(!gObjIsConnected(iGateIndex) || gObj[iGateIndex].Class != 277)
			return false;

		gObj[iLeverIndex].m_iCsGateLeverLinkIndex = iGateIndex;
		gObj[iGateIndex].m_iCsGateLeverLinkIndex = iLeverIndex;

		return true;
	}__finally
	{
		//DebugLog("%s END",__FUNCTION__);
	}
}

void CCastleSiege::SetGateBlockState(int iX,int iY,int iGateState)
{
	//DebugLog("%s START",__FUNCTION__);
	__try
	{
		if(XY_MACRO(iX) == FALSE || XY_MACRO(iY) == FALSE)
			return;

		switch(iGateState)
		{
			case 0:
			{
				for(int i = iX -2; i <= iX + 3; i++)
				{
					for(int j = iY; j <= iY + 2;j++)
					{
						MapC[MAP_INDEX_CASTLESIEGE].m_attrbuf[j * 256 + i] |= 16;
					}
				}
			}
			break;
			case 1:
			{
				for(int i = iX -2; i <= iX + 3; i++)
				{
					for(int j = iY; j <= iY + 2;j++)
					{
						MapC[MAP_INDEX_CASTLESIEGE].m_attrbuf[j * 256 + i] &= ~16;
					}
				}
			}
			break;
		}
	}__finally
	{
		//DebugLog("%s END",__FUNCTION__);
	}
}

void CCastleSiege::CreateDbNPC()
{
	//DebugLog("%s START",__FUNCTION__);
	//EnterCriticalSection(&m_critNpcData);

	//_CS_NPC_DATA * it = this->m_vtNpcData.begin();
	std::vector<_CS_NPC_DATA>::iterator it = this->m_vtNpcData.begin();
	int bExist = 0;

	while(it != this->m_vtNpcData.end())
	{
		std::vector<_CS_NPC_DATA>::iterator pNpcData = it;
		//_CS_NPC_DATA & pNpcData = _CS_NPC_DATA(*it);
		//LogAddTD("[CastleSiege][CreateDbNPC] NPC Record Index:%d Num:%d", pNpcData->m_iNPC_INDEX, pNpcData->m_iNPC_NUM);

		if(pNpcData->m_bIN_USE == 1 && pNpcData->m_iNPC_LIVE == 1)
		{
			int iNPC_INDEX = gObjAddMonster(MAP_INDEX_CASTLESIEGE);

			if(iNPC_INDEX >= 0)
			{
				gObjSetMonster(iNPC_INDEX, pNpcData->m_iNPC_NUM,"CCastleSiege::CreateDbNPC");
				gObj[iNPC_INDEX].m_PosNum = -1;
				gObj[iNPC_INDEX].X = pNpcData->m_iNPC_SX;
				gObj[iNPC_INDEX].Y = pNpcData->m_iNPC_SY;
				gObj[iNPC_INDEX].MapNumber = MAP_INDEX_CASTLESIEGE;
				gObj[iNPC_INDEX].TX = gObj[iNPC_INDEX].X;
				gObj[iNPC_INDEX].TY = gObj[iNPC_INDEX].Y;
				gObj[iNPC_INDEX].m_OldX = gObj[iNPC_INDEX].X;
				gObj[iNPC_INDEX].m_OldY = gObj[iNPC_INDEX].Y;
				gObj[iNPC_INDEX].Dir = pNpcData->m_iNPC_DIR;
				gObj[iNPC_INDEX].StartX = gObj[iNPC_INDEX].X;
				gObj[iNPC_INDEX].StartY = gObj[iNPC_INDEX].Y;
				gObj[iNPC_INDEX].DieRegen = 0;
				gObj[iNPC_INDEX].MaxRegenTime = 0;
				gObj[iNPC_INDEX].Life = pNpcData->m_iNPC_HP;
				gObj[iNPC_INDEX].MaxLife = pNpcData->m_iNPC_MAXHP;

				gObj[iNPC_INDEX].m_btCsNpcType = 1;

				int iNPC_DF_LEVEL = pNpcData->m_iNPC_DF_LEVEL;

				if(iNPC_DF_LEVEL < 0)
				{
					iNPC_DF_LEVEL = 0;
				}

				if(iNPC_DF_LEVEL > CS_MAX_UPGRADE)
				{
					iNPC_DF_LEVEL = CS_MAX_UPGRADE;
				}

				int iNPC_RG_LEVEL = pNpcData->m_iNPC_RG_LEVEL;

				if(iNPC_RG_LEVEL < 0)
				{
					iNPC_RG_LEVEL = 0;
				}

				if(iNPC_RG_LEVEL > CS_MAX_UPGRADE)
				{
					iNPC_RG_LEVEL = CS_MAX_UPGRADE;
				}

				switch(pNpcData->m_iNPC_NUM)
				{
					case 277:
						gObj[iNPC_INDEX].m_Defense = g_iNpcDefense_CGATE[iNPC_DF_LEVEL];
						gObj[iNPC_INDEX].m_MagicDefense = g_iNpcDefense_CGATE[iNPC_DF_LEVEL];
						gObj[iNPC_INDEX].m_btCsNpcDfLevel = iNPC_DF_LEVEL;
					break;
					case 283:
						gObj[iNPC_INDEX].m_Defense = g_iNpcDefense_CSTATUE[iNPC_DF_LEVEL];
						gObj[iNPC_INDEX].m_MagicDefense = g_iNpcDefense_CSTATUE[iNPC_DF_LEVEL];
						gObj[iNPC_INDEX].m_btCsNpcDfLevel = iNPC_DF_LEVEL;
						gObj[iNPC_INDEX].m_btCsNpcRgLevel = iNPC_RG_LEVEL;
					break;
				}

				if(pNpcData->m_iNPC_NUM == 277)
				{
					SetGateBlockState(pNpcData->m_iNPC_SX,pNpcData->m_iNPC_SY,1);
					gObj[iNPC_INDEX].m_btCsGateOpen = TRUE;
					int bCreateLever = FALSE;

					if(pNpcData->m_iCS_GATE_LEVER_INDEX == -1)
					{
						bCreateLever = TRUE;
					}
					else if(CheckLeverAlive(pNpcData->m_iCS_GATE_LEVER_INDEX) == FALSE)
					{
						bCreateLever = TRUE;
					}

					if(bCreateLever != FALSE)
					{
						int iLeverIndex = CreateCsGateLever(pNpcData->m_iNPC_SX,pNpcData->m_iNPC_SY+4);

						if(OBJMAX_RANGE(iLeverIndex))
						{
							it->m_iCS_GATE_LEVER_INDEX = iLeverIndex;

							int bRETVAL = LinkCsGateLever(iLeverIndex,iNPC_INDEX);

							if(bRETVAL == FALSE)
							{
								LogAddC(2,"[CastleSiege] CCastleSiege::CreateDbNPC() ERROR - Failed to Create Gate Lever : GateIndex:%d",pNpcData->m_iNPC_INDEX);
							}
						}
						else
						{
							it->m_iCS_GATE_LEVER_INDEX = -1;
							LogAddC(2,"[CastleSiege] CCastleSiege::CreateDbNPC() ERROR - Failed to Create Gate Lever : GateIndex:%d",pNpcData->m_iNPC_INDEX);
						}
					}
					else
					{
						int bRETVAL = LinkCsGateLever(pNpcData->m_iCS_GATE_LEVER_INDEX,iNPC_INDEX);

						if(bRETVAL == FALSE)
						{
							LogAddC(2,"[CastleSiege] CCastleSiege::CreateDbNPC() ERROR - Failed to Create Gate Lever : GateIndex:%d",pNpcData->m_iNPC_INDEX);
						}
					}
				}

				it->m_iNPC_OBJINDEX = iNPC_INDEX;
				it->m_iNPC_LIVE = 2;
			}
		}

		it++;
	}
	//LeaveCriticalSection(&m_critNpcData);

	//DebugLog("%s END",__FUNCTION__);
}

void CCastleSiege::SetState(int iCastleSiegeState,int bSetRemainMsec)
{
	//DebugLog("%s START",__FUNCTION__);
	__try
	{
		if(iCastleSiegeState < CASTLESIEGE_STATE_NONE || iCastleSiegeState > CASTLESIEGE_STATE_ENDCYCLE)
			return;

		m_iCastleSiegeState = iCastleSiegeState;

		g_CastleSiegeSync.SetCastleState(m_iCastleSiegeState); 

		switch(m_iCastleSiegeState)
		{
			case CASTLESIEGE_STATE_NONE: SetState_NONE(bSetRemainMsec); break;
			case CASTLESIEGE_STATE_IDLE_1: SetState_IDLE_1(bSetRemainMsec); break;
			case CASTLESIEGE_STATE_REGSIEGE: SetState_REGSIEGE(bSetRemainMsec); break;
			case CASTLESIEGE_STATE_IDLE_2: SetState_IDLE_2(bSetRemainMsec); break;
			case CASTLESIEGE_STATE_REGMARK: SetState_REGMARK(bSetRemainMsec); break;
			case CASTLESIEGE_STATE_IDLE_3: SetState_IDLE_3(bSetRemainMsec); break;
			case CASTLESIEGE_STATE_NOTIFY: SetState_NOTIFY(bSetRemainMsec); break;
			case CASTLESIEGE_STATE_READYSIEGE: SetState_READYSIEGE(bSetRemainMsec); break;
			case CASTLESIEGE_STATE_STARTSIEGE: SetState_STARTSIEGE(bSetRemainMsec); break;
			case CASTLESIEGE_STATE_ENDSIEGE: SetState_ENDSIEGE(bSetRemainMsec); break;
			case CASTLESIEGE_STATE_ENDCYCLE: SetState_ENDCYCLE(bSetRemainMsec); break;
		}

		if(m_bDbNpcCreated == FALSE)
		{
			m_bDbNpcCreated = TRUE;
			CreateDbNPC();
		}

		GetLocalTime(&m_tmStateStartDate);
		m_dwEVENT_MSG_TICK_COUNT = 0;
		m_dwCHECK_GATE_ALIVE_COUNT = 0;
	}__finally
	{
		//DebugLog("%s END",__FUNCTION__);
	}
}

void CCastleSiege::SetState_NONE(int bSetRemainMsec)
{

}

void CCastleSiege::SetState_IDLE_1(int bSetRemainMsec)
{
	//DebugLog("%s START",__FUNCTION__);
	__try
	{
		LogAddTD("SET SetState_IDLE_1()");

		if(bSetRemainMsec != FALSE)
		{
			time_t ttSTime;	//long ttSTime;
			long ttETime;
			tm tmETime;
			double dResultSecond;

			memset(&tmETime,0x00,sizeof(tmETime));

			tmETime.tm_year = m_tmStartDate.wYear - 1900;
			tmETime.tm_mon = m_tmStartDate.wMonth - 1;
			tmETime.tm_mday = m_tmStartDate.wDay;

			ttETime = mktime(&tmETime);
			time(&ttSTime);

			dResultSecond = difftime(ttETime,ttSTime);
			m_iCS_REMAIN_MSEC = dResultSecond * (double)1000.0;
		}
	}__finally
	{
		//DebugLog("%s END",__FUNCTION__);
	}
}

void CCastleSiege::SetState_REGSIEGE(int bSetRemainMsec)
{
	//DebugLog("%s START",__FUNCTION__);
	__try
	{
		LogAddTD("SET SetState_REGSIEGE()");

		if(bSetRemainMsec != FALSE)
		{
			int iGAP_SEC = GetStateGapSec(CASTLESIEGE_STATE_REGSIEGE);
	
			if(iGAP_SEC < 0)
			{
				m_bDoRun = 0;
				MsgBox("[CastleSiege] CCastleSiege::SetState_REGSIEGE() - iGAP_SEC < 0");
			}

			m_iCS_REMAIN_MSEC = iGAP_SEC * 1000;
		}

		GS_GDReqRestartCastleState(m_iMapSvrGroup);
	}__finally
	{
		//DebugLog("%s END",__FUNCTION__);
	}
}

void CCastleSiege::SetState_IDLE_2(int bSetRemainMsec)
{
	//DebugLog("%s START",__FUNCTION__);
	__try
	{
		LogAddTD("SET SetState_IDLE_2()");

		if(bSetRemainMsec != FALSE)
		{
			int iGAP_SEC = GetStateGapSec(CASTLESIEGE_STATE_IDLE_2);

			if(iGAP_SEC < 0)
			{
				m_bDoRun = 0;
				MsgBox("[CastleSiege] CCastleSiege::SetState_IDLE_2() - iGAP_SEC < 0");
			}

			m_iCS_REMAIN_MSEC = iGAP_SEC * 1000;
		}
	}__finally
	{
		//DebugLog("%s END",__FUNCTION__);
	}
}

void CCastleSiege::SetState_REGMARK(int bSetRemainMsec)
{
	//DebugLog("%s START",__FUNCTION__);
	__try
	{
		LogAddTD("SET SetState_REGMARK()");

		if(bSetRemainMsec != FALSE)
		{
			int iGAP_SEC = GetStateGapSec(CASTLESIEGE_STATE_REGMARK);

			if(iGAP_SEC < 0)
			{
				m_bDoRun = 0;
				MsgBox("[CastleSiege] CCastleSiege::SetState_REGMARK() - iGAP_SEC < 0");
			}

			m_iCS_REMAIN_MSEC = iGAP_SEC * 1000;
		}
	}__finally
	{
		//DebugLog("%s END",__FUNCTION__);
	}
}

void CCastleSiege::SetState_IDLE_3(int bSetRemainMsec)
{
	//DebugLog("%s START",__FUNCTION__);
	__try
	{
		LogAddTD("SET SetState_IDLE_3()");

		if(bSetRemainMsec != FALSE)
		{
			int iGAP_SEC = GetStateGapSec(CASTLESIEGE_STATE_IDLE_3);

			if(iGAP_SEC < 0)
			{
				m_bDoRun = 0;
				MsgBox("[CastleSiege] CCastleSiege::SetState_IDLE_3() - iGAP_SEC < 0");
			}

			m_iCS_REMAIN_MSEC = iGAP_SEC * 1000;
		}
	}__finally
	{
		//DebugLog("%s END",__FUNCTION__);
	}
}

void CCastleSiege::SetState_NOTIFY(int bSetRemainMsec)
{
	//DebugLog("%s START",__FUNCTION__);
	__try
	{
		LogAddTD("SET SetState_NOTIFY()");

		if(bSetRemainMsec != FALSE)
		{
			int iGAP_SEC = GetStateGapSec(CASTLESIEGE_STATE_NOTIFY);

			if(iGAP_SEC < 0)
			{
				m_bDoRun = 0;
				MsgBox("[CastleSiege] CCastleSiege::SetState_NOTIFY() - iGAP_SEC < 0");
			}

			m_iCS_REMAIN_MSEC = iGAP_SEC * 1000;
		}

		CheckBuildCsGuildInfo();
	}__finally
	{
		//DebugLog("%s END",__FUNCTION__);
	}
}

void CCastleSiege::SetState_READYSIEGE(int bSetRemainMsec)
{
	//DebugLog("%s START",__FUNCTION__);
	__try
	{
		LogAddTD("SET SetState_READYSIEGE()");

		if(bSetRemainMsec != FALSE)
		{
			int iGAP_SEC = GetStateGapSec(CASTLESIEGE_STATE_READYSIEGE);

			if(iGAP_SEC < 0)
			{
				m_bDoRun = 0;
				MsgBox("[CastleSiege] CCastleSiege::SetState_READYSIEGE() - iGAP_SEC < 0");
			}

			m_iCS_REMAIN_MSEC = iGAP_SEC * 1000;
		}

		CheckBuildCsGuildInfo();
	}__finally
	{
		//DebugLog("%s END",__FUNCTION__);
	}
}

void CCastleSiege::SetState_STARTSIEGE(int bSetRemainMsec)
{
	//DebugLog("%s START",__FUNCTION__);
	__try
	{
		LogAddTD("SET SetState_STARTSIEGE()");

		if(bSetRemainMsec != FALSE)
		{
			int iGAP_SEC = GetStateGapSec(CASTLESIEGE_STATE_STARTSIEGE);

			if(iGAP_SEC < 0)
			{
				m_bDoRun = 0;
				MsgBox("[CastleSiege] CCastleSiege::SetState_STARTSIEGE() - iGAP_SEC < 0");
			}

			m_iCS_REMAIN_MSEC = iGAP_SEC * 1000;
		}

		m_bCastleTowerAccessable = FALSE;
		m_bRegCrownAvailable = FALSE;
		m_iCastleCrownAccessUser = -1;
		m_btCastleCrownAccessUserX = 0;
		m_btCastleCrownAccessUserY = 0;
		m_iCastleSwitchAccessUser1 = -1;
		m_iCastleSwitchAccessUser2 = -1;
		m_dwCrownAccessTime = 0;
		m_dwCHECK_GATE_ALIVE_COUNT = 0;
		m_dwCS_STARTTIME_TICK_COUNT = 0;
		m_dwCS_LEFTTIME_TICK_COUNT = GetTickCount();
		m_dwCS_JOINSIDE_REFRESH_TICK_COUNT = GetTickCount();
		m_dwCS_JOINSIDE_REFRESH_TOTAL_COUNT = 0;
		m_dwCS_MINIMAP_SEND_TICK_COUNT = GetTickCount();

		memset(m_szMiddleWinnerGuild,0x00,sizeof(m_szMiddleWinnerGuild));

		if(CheckAttackGuildExist() == FALSE)
		{
			CheckCastleSiegeResult();
			SetState(CASTLESIEGE_STATE_ENDSIEGE,TRUE);
			LogAddC(2,"[CastleSiege] CCastleSiege::SetState_STARTSIEGE() - CheckAttackGuildExist() == FALSE");
		}
		else
		{
			StoreDbNpc();
			ClearNonDbNPC();
			CheckReviveNonDbNPC();

			if( CheckGuardianStatueExist() == FALSE )
			{
				ClearCastleTowerBarrier();
				SetCastleTowerAccessable(TRUE);
			}


			CheckReviveGuardianStatue();
			AdjustDbNpcLevel();
			SetAllCastleGateState(FALSE);
			SetAllUserCsJoinSide();
			ReSpawnEnemyUser(TRUE);
			NotifyAllUserCsStartState(1);
		}
	}__finally
	{
		//DebugLog("%s END",__FUNCTION__);
	}
}

void CCastleSiege::SetState_ENDSIEGE(int bSetRemainMsec)
{
	//DebugLog("%s START",__FUNCTION__);
	__try
	{
		LogAddTD("SET SetState_ENDSIEGE()");

		if(bSetRemainMsec != FALSE)
		{
			time_t ttSTime; //long ttSTime;
			long ttETime;
			tm tmETime;
			double dResultSecond;

			memset(&tmETime,0x00,sizeof(tmETime));

			tmETime.tm_year = m_tmEndDate.wYear - 1900;
			tmETime.tm_mon = m_tmEndDate.wMonth - 1;
			tmETime.tm_mday = m_tmEndDate.wDay;

			time(&ttSTime);
			ttETime = mktime(&tmETime);

			dResultSecond = difftime(ttETime,ttSTime);
			m_iCS_REMAIN_MSEC = dResultSecond * (double)1000.0;
		}

		ClearNonDbNPC();
		ResetAllUserCsJoinSide();
		SendAllUserAnyMsg(lMsg.Get(MSGGET(6, 84)),1);
	}__finally
	{
		//DebugLog("%s END",__FUNCTION__);
	}
}

/*typedef struct _SYSTEMTIME {
    WORD wYear;
    WORD wMonth;
    WORD wDayOfWeek;
    WORD wDay;
    WORD wHour;
    WORD wMinute;
    WORD wSecond;
    WORD wMilliseconds;
}*/

void CCastleSiege::SetState_ENDCYCLE(int bSetRemainMsec)
{
	//DebugLog("%s START",__FUNCTION__);
	__try
	{
		LogAddTD("SET SetState_ENDCYCLE()");

		SYSTEMTIME tmNowDate;

		GetLocalTime(&tmNowDate);

		//tmNowDate.wDay = 0;
		tmNowDate.wHour = 0;
		tmNowDate.wMinute = 0;
		tmNowDate.wSecond = 0;

		m_tmSiegeEndSchedule = tmNowDate;
		m_tmStartDate = m_tmSiegeEndSchedule;

		GetNextDay(&tmNowDate,m_iCastleSiegeCycle,0,0,0);

		GetNextDay(&m_tmSiegeEndSchedule, m_iCastleSiegeCycle,0,0,0);

		m_tmEndDate = tmNowDate;

		LogAddTD("[CastleSiege] State - End Cycle Schedule : (%d-%d-%d(%d:%d:%d)) (%d-%d-%d(%d:%d:%d))",
			m_tmStartDate.wYear, m_tmStartDate.wMonth, m_tmStartDate.wDay, m_tmStartDate.wHour, m_tmStartDate.wMinute, m_tmStartDate.wSecond,
			m_tmEndDate.wYear, m_tmEndDate.wMonth, m_tmEndDate.wDay, m_tmEndDate.wHour, m_tmEndDate.wMinute, m_tmEndDate.wSecond);

		LogAddTD("[CastleSiege] State - End Cycle : Date-Changing Info (%d-%d-%d) (%d-%d-%d)",
			m_tmStartDate.wYear, m_tmStartDate.wMonth, m_tmStartDate.wDay,
			m_tmEndDate.wYear, m_tmEndDate.wMonth, m_tmEndDate.wDay);

		GS_GDReqSiegeDateChange(m_iMapSvrGroup,-1,m_tmStartDate.wYear,m_tmStartDate.wMonth,m_tmStartDate.wDay,m_tmEndDate.wYear,m_tmEndDate.wMonth,m_tmEndDate.wDay);
		GS_GDReqResetRegSiegeInfo(m_iMapSvrGroup);
		GS_GDReqResetSiegeGuildInfo(m_iMapSvrGroup);
		SetState(CASTLESIEGE_STATE_REGSIEGE,1);
	}__finally
	{
		//DebugLog("%s END",__FUNCTION__);
	}
}

int CCastleSiege::GetStateGapSec(int iCastleSiegeState)
{
	int iGAP_SEC = -1;

	//DebugLog("%s START",__FUNCTION__);
	//EnterCriticalSection(&m_critScheduleData);

	//for(_CS_SCHEDULE_DATA * it = m_vtScheduleData.begin(); it != m_vtScheduleData.end(); it++)
	for(std::vector<_CS_SCHEDULE_DATA>::iterator it = m_vtScheduleData.begin(); it != m_vtScheduleData.end(); it++)
	{
		_CS_SCHEDULE_DATA & pScheduleData = *it;

		if(pScheduleData.m_iSTATE == iCastleSiegeState)
		{
			iGAP_SEC = pScheduleData.m_iGAP_SEC;
			break;
		}
	}

	//LeaveCriticalSection(&m_critScheduleData);
	//DebugLog("%s END",__FUNCTION__);
	return iGAP_SEC;
}

void CCastleSiege::CheckBuildCsGuildInfo()
{
	//DebugLog("%s START",__FUNCTION__);
	__try
	{
		m_bCsBasicGuildInfoLoadOK = 0;
		m_bCsTotalGuildInfoLoadOK = 0;

		///EnterCriticalSection(&m_critCsBasicGuildInfo);
		m_mapCsBasicGuildInfo.clear();
		////LeaveCriticalSection(&m_critCsBasicGuildInfo);

		////EnterCriticalSection(&m_critCsTotalGuildInfo);
		m_mapCsTotalGuildInfo.clear();
		////LeaveCriticalSection(&m_critCsTotalGuildInfo);

		if(m_btIsSiegeGuildList == TRUE)
		{
			GS_GDReqCsLoadTotalGuildInfo(m_iMapSvrGroup);
		}
		else
		{
			GS_GDReqCalcRegGuildList(m_iMapSvrGroup);
		}
	}__finally
	{
		//DebugLog("%s END",__FUNCTION__);
	}
}

int CCastleSiege::CheckAttackGuildExist()
{
	//DebugLog("%s START",__FUNCTION__);
	__try
	{
		BOOL iRETVAL = FALSE;

		////EnterCriticalSection(&m_critCsTotalGuildInfo);
		if(!m_mapCsTotalGuildInfo.empty())
		{
			iRETVAL = TRUE;
		}
		////LeaveCriticalSection(&m_critCsTotalGuildInfo);

		//DebugLog("%s END",__FUNCTION__);
		return iRETVAL;
	}__finally
	{
		//DebugLog("%s END",__FUNCTION__);
	}
}

int CCastleSiege::CheckCastleSiegeResult()
{
	//DebugLog("%s START",__FUNCTION__);
	__try
	{
		char szMsg[0x100] = {0};
		int bRETVAL = FALSE;

		if(m_btIsCastleOccupied == TRUE && strcmp(m_szCastleOwnerGuild,"") != FALSE)
		{
			if(strcmp(m_szMiddleWinnerGuild,"") == FALSE || strcmp(m_szMiddleWinnerGuild,m_szCastleOwnerGuild) == FALSE)
			{
				m_btIsCastleOccupied = TRUE;
				wsprintf(szMsg,lMsg.Get(MSGGET(6, 197)),m_szCastleOwnerGuild);
				bRETVAL = FALSE;
			}
			else
			{
				m_btIsCastleOccupied = TRUE;
				memset(m_szCastleOwnerGuild,0x00,MAX_GUILD_LEN);
				memcpy(m_szCastleOwnerGuild,m_szMiddleWinnerGuild,MAX_GUILD_LEN);

				g_CastleSiegeSync.SetCastleOwnerGuild(this->m_szCastleOwnerGuild);

				wsprintf(szMsg,lMsg.Get(MSGGET(6, 198)),m_szCastleOwnerGuild);
				bRETVAL = TRUE;
			}
		}
		else
		{
			if(strcmp(m_szMiddleWinnerGuild,"") == FALSE)
			{
				m_btIsCastleOccupied = FALSE;
				wsprintf(szMsg,lMsg.Get(MSGGET(6, 199)));
				bRETVAL = FALSE;
			}
			else
			{
				m_btIsCastleOccupied = TRUE;
				memset(m_szCastleOwnerGuild,0x00,MAX_GUILD_LEN);
				memcpy(m_szCastleOwnerGuild,m_szMiddleWinnerGuild,MAX_GUILD_LEN);

				g_CastleSiegeSync.SetCastleOwnerGuild(this->m_szCastleOwnerGuild);

				wsprintf(szMsg,lMsg.Get(MSGGET(6, 198)),m_szCastleOwnerGuild);
				bRETVAL = TRUE;
			}
		}

		m_btIsSiegeEnded = TRUE;

		LogAddTD("[CastleSiege] Castle Final Winner Guild : %s",(strcmp(m_szCastleOwnerGuild,"") == FALSE) ? "NONE" : m_szCastleOwnerGuild);
		SendMapServerGroupMsg(szMsg);

		return bRETVAL;
	}__finally
	{
		//DebugLog("%s END",__FUNCTION__);
	}
}

struct CSP_REQ_NPCUPDATEDATA
{
	PWMSG_HEAD h;	// C2:89
	unsigned short wMapSvrNum;	// 4
	int iCount;	// 8
};

struct CSP_NPCUPDATEDATA
{
	int iNpcNumber;	// 0
	int iNpcIndex;	// 4
	int iNpcDfLevel;	// 8
	int iNpcRgLevel;	// C
	int iNpcMaxHp;	// 10
	int iNpcHp;	// 14
	unsigned char btNpcX;	// 18
	unsigned char btNpcY;	// 19
	unsigned char btNpcDIR;	// 1A
};

void CCastleSiege::StoreDbNpc()
{

	//DebugLog("%s START",__FUNCTION__);
	LogAddTD("[CastleSiege] CCastleSiege::StoreDbNpc() - << START >>");

	char cBUFFER[0x1090];

	CSP_REQ_NPCUPDATEDATA * lpMsg = (CSP_REQ_NPCUPDATEDATA *)cBUFFER;
	CSP_NPCUPDATEDATA * lpMsgBody = (CSP_NPCUPDATEDATA *)&cBUFFER[sizeof(CSP_REQ_NPCUPDATEDATA)];

	int iCOUNT = 0;

	//EnterCriticalSection(&m_critNpcData);
	//_CS_NPC_DATA * it = this->m_vtNpcData.begin();
	std::vector<_CS_NPC_DATA>::iterator it = this->m_vtNpcData.begin();
	int bExist = FALSE;

	while(it != this->m_vtNpcData.end())
	{
		if(iCOUNT >= 150)
			break;

		std::vector<_CS_NPC_DATA>::iterator pNpcData = it;
		//_CS_NPC_DATA & pNpcData = _CS_NPC_DATA(*it);

		if(pNpcData->m_bNPC_DBSAVE == TRUE 
			&& gObjIsConnected(pNpcData->m_iNPC_OBJINDEX)
			&& gObj[pNpcData->m_iNPC_OBJINDEX].Class == pNpcData->m_iNPC_NUM
			&& gObj[pNpcData->m_iNPC_OBJINDEX].Live == TRUE
			&& gObj[pNpcData->m_iNPC_OBJINDEX].Life > 0)
		{
			lpMsgBody[iCOUNT].iNpcNumber = pNpcData->m_iNPC_NUM;
			lpMsgBody[iCOUNT].iNpcIndex = pNpcData->m_iNPC_INDEX;
			lpMsgBody[iCOUNT].iNpcDfLevel = pNpcData->m_iNPC_DF_LEVEL;
			lpMsgBody[iCOUNT].iNpcRgLevel = pNpcData->m_iNPC_RG_LEVEL;
			lpMsgBody[iCOUNT].iNpcMaxHp = gObj[pNpcData->m_iNPC_OBJINDEX].MaxLife;
			lpMsgBody[iCOUNT].iNpcHp = gObj[pNpcData->m_iNPC_OBJINDEX].Life;
			lpMsgBody[iCOUNT].btNpcX = pNpcData->m_iNPC_SX;
			lpMsgBody[iCOUNT].btNpcY = pNpcData->m_iNPC_SY;
			lpMsgBody[iCOUNT].btNpcDIR = pNpcData->m_iNPC_DIR;


			LogAddTD("[CastleSiege] CCastleSiege::StoreDbNpc() NPC INFO (CLS:%d, IDX:%d(%d), MAXHP:%d, HP:%d, DF:%d, RG:%d)",
				pNpcData->m_iNPC_NUM,pNpcData->m_iNPC_INDEX,pNpcData->m_iNPC_OBJINDEX,lpMsgBody[iCOUNT].iNpcMaxHp,lpMsgBody[iCOUNT].iNpcHp,lpMsgBody[iCOUNT].iNpcDfLevel,lpMsgBody[iCOUNT].iNpcRgLevel);
			iCOUNT++;
		}

		it++;
	}

	//LeaveCriticalSection(&m_critNpcData);

	if(iCOUNT < 0)
	{
		iCOUNT = 0;
	}

	lpMsg->wMapSvrNum = m_iMapSvrGroup;
	lpMsg->iCount = iCOUNT;
	lpMsg->h.set((PBYTE)lpMsg,0x89,iCOUNT * sizeof(CSP_NPCUPDATEDATA) + sizeof(CSP_REQ_NPCUPDATEDATA));

	cDBSMng.Send((char *)lpMsg,iCOUNT * sizeof(CSP_NPCUPDATEDATA) + sizeof(CSP_REQ_NPCUPDATEDATA));
	LogAddTD("[CastleSiege] CCastleSiege::StoreDbNpc() - << END >>");

	//DebugLog("%s END",__FUNCTION__);
}

void CCastleSiege::ClearNonDbNPC()
{
	//DebugLog("%s START",__FUNCTION__);
	__try
	{
		for(int n = 0; n < OBJ_MAXMONSTER; n++)
		{
			if(gObjIsConnected(n)
				&& gObj[n].MapNumber == MAP_INDEX_CASTLESIEGE
				&& gObj[n].m_btCsNpcType != NULL
				&& gObj[n].m_btCsNpcType != 1)
			{
				if(gObj[n].Class == 278)
				{
					g_CsNPC_LifeStone.DeleteLifeStone(n);
				}

				if(gObj[n].Class == 287 || gObj[n].Class == 286)
				{
					g_CsNPC_Mercenary.DeleteMercenary(n);
				}

				//[219]	"Castle Gate Switch"
				if(gObj[n].Class == 219)
					continue;

				gObjDel(n);
			}
		}
	}__finally
	{
		//DebugLog("%s END",__FUNCTION__);
	}
}

void CCastleSiege::CheckReviveNonDbNPC()
{
	//DebugLog("%s START",__FUNCTION__);
	//EnterCriticalSection(&m_critNpcData);

	//_CS_NPC_DATA * it = this->m_vtNpcData.begin();
	std::vector<_CS_NPC_DATA>::iterator it = this->m_vtNpcData.begin();

	while(it != this->m_vtNpcData.end())
	{
		std::vector<_CS_NPC_DATA>::iterator pNpcData = it;
		//_CS_NPC_DATA & pNpcData = _CS_NPC_DATA(*it);

		if(pNpcData->m_bNPC_DBSAVE == 0)
		{
			int iNPC_INDEX = gObjAddMonster(MAP_INDEX_CASTLESIEGE);

			if(iNPC_INDEX >= 0)
			{
				gObjSetMonster(iNPC_INDEX,pNpcData->m_iNPC_NUM,"CCastleSiege::CheckReviveNonDbNPC");

				gObj[iNPC_INDEX].m_PosNum = -1;
				gObj[iNPC_INDEX].X = pNpcData->m_iNPC_SX;
				gObj[iNPC_INDEX].Y = pNpcData->m_iNPC_SY;
				gObj[iNPC_INDEX].MapNumber = MAP_INDEX_CASTLESIEGE;
				gObj[iNPC_INDEX].TX = gObj[iNPC_INDEX].X;
				gObj[iNPC_INDEX].TY = gObj[iNPC_INDEX].Y;
				gObj[iNPC_INDEX].m_OldX = gObj[iNPC_INDEX].X;
				gObj[iNPC_INDEX].m_OldY = gObj[iNPC_INDEX].Y;
				gObj[iNPC_INDEX].Dir = pNpcData->m_iNPC_DIR;
				gObj[iNPC_INDEX].StartX = gObj[iNPC_INDEX].X;
				gObj[iNPC_INDEX].StartY = gObj[iNPC_INDEX].Y;
				gObj[iNPC_INDEX].DieRegen = 0;
				gObj[iNPC_INDEX].MaxRegenTime = 0;
				gObj[iNPC_INDEX].Life = pNpcData->m_iNPC_HP;
				gObj[iNPC_INDEX].MaxLife = pNpcData->m_iNPC_MAXHP;

				switch(pNpcData->m_iNPC_NUM)
				{
					case 215:
						gObj[iNPC_INDEX].m_btCsNpcType = 2;
					break;
					case 221:
						gObj[iNPC_INDEX].m_btCsNpcType = 3;
					break;
					case 222:
						gObj[iNPC_INDEX].m_btCsNpcType = 2;
					break;
					case 288:
						gObj[iNPC_INDEX].m_btCsNpcType = 2;
					break;
					default:
						gObj[iNPC_INDEX].m_btCsNpcType = 2;
					break;
				}

				it->m_bIN_USE = TRUE;
			}
		}

		it++;
	}

	//LeaveCriticalSection(&m_critNpcData);
	//DebugLog("%s END",__FUNCTION__);
}

void CCastleSiege::CheckReviveGuardianStatue()
{
	//DebugLog("%s START",__FUNCTION__);
	//EnterCriticalSection(&m_critNpcData);

	//_CS_NPC_DATA * it = this->m_vtNpcData.begin();
	std::vector<_CS_NPC_DATA>::iterator it = this->m_vtNpcData.begin();

	while(it != this->m_vtNpcData.end())
	{

		std::vector<_CS_NPC_DATA>::iterator pNpcData = it;
		//_CS_NPC_DATA & pNpcData = _CS_NPC_DATA(*it);

		if(pNpcData->m_iNPC_NUM == 283
			&& pNpcData->m_iNPC_LIVE == FALSE
			)
		{
			int iNPC_INDEX = gObjAddMonster(MAP_INDEX_CASTLESIEGE);

			if(iNPC_INDEX >= 0)
			{
				gObjSetMonster(iNPC_INDEX,pNpcData->m_iNPC_NUM,"CCastleSiege::CheckReviveGuardianStatue");

				gObj[iNPC_INDEX].m_PosNum = -1;
				gObj[iNPC_INDEX].X = pNpcData->m_iNPC_SX;
				gObj[iNPC_INDEX].Y = pNpcData->m_iNPC_SY;
				gObj[iNPC_INDEX].MapNumber = MAP_INDEX_CASTLESIEGE;
				gObj[iNPC_INDEX].TX = gObj[iNPC_INDEX].X;
				gObj[iNPC_INDEX].TY = gObj[iNPC_INDEX].Y;
				gObj[iNPC_INDEX].m_OldX = gObj[iNPC_INDEX].X;
				gObj[iNPC_INDEX].m_OldY = gObj[iNPC_INDEX].Y;
				gObj[iNPC_INDEX].Dir = pNpcData->m_iNPC_DIR;
				gObj[iNPC_INDEX].StartX = gObj[iNPC_INDEX].X;
				gObj[iNPC_INDEX].StartY = gObj[iNPC_INDEX].Y;
				gObj[iNPC_INDEX].DieRegen = 0;
				gObj[iNPC_INDEX].MaxRegenTime = 0;
				gObj[iNPC_INDEX].Life = pNpcData->m_iNPC_HP;
				gObj[iNPC_INDEX].MaxLife = pNpcData->m_iNPC_MAXHP;
				gObj[iNPC_INDEX].m_btCsNpcType = 1;

				int iNPC_DF_LEVEL = pNpcData->m_iNPC_BASE_DF_LEVEL;

				if(iNPC_DF_LEVEL < 0)
				{
					iNPC_DF_LEVEL = 0;
				}

				if(iNPC_DF_LEVEL > CS_MAX_UPGRADE)
				{
					iNPC_DF_LEVEL = CS_MAX_UPGRADE;
				}

				int iNPC_RG_LEVEL = pNpcData->m_iNPC_BASE_RG_LEVEL;

				if(iNPC_RG_LEVEL < 0)
				{
					iNPC_RG_LEVEL = 0;
				}

				if(iNPC_RG_LEVEL > CS_MAX_UPGRADE)
				{
					iNPC_RG_LEVEL = CS_MAX_UPGRADE;
				}

				gObj[iNPC_INDEX].m_Defense = g_iNpcDefense_CSTATUE[iNPC_DF_LEVEL];
				gObj[iNPC_INDEX].m_btCsNpcDfLevel = iNPC_DF_LEVEL;
				gObj[iNPC_INDEX].m_btCsNpcRgLevel = iNPC_RG_LEVEL;

				it->m_iNPC_OBJINDEX = iNPC_INDEX;
				it->m_iNPC_LIVE = 2;
				it->m_bIN_USE = TRUE;
			}
		}
		it++;
	}

	//LeaveCriticalSection(&m_critNpcData);
	//DebugLog("%s END",__FUNCTION__);
}

void CCastleSiege::AdjustDbNpcLevel()
{
	//DebugLog("%s START",__FUNCTION__);
	//EnterCriticalSection(&m_critNpcData);

	//_CS_NPC_DATA * it = this->m_vtNpcData.begin();
	std::vector<_CS_NPC_DATA>::iterator it = this->m_vtNpcData.begin();

	while(it != this->m_vtNpcData.end())
	{
		std::vector<_CS_NPC_DATA>::iterator pNpcData = it;
		//_CS_NPC_DATA & pNpcData = _CS_NPC_DATA(*it);

		if(pNpcData->m_bIN_USE == 1
			&& pNpcData->m_iNPC_LIVE > 0)
		{
			int iNPC_INDEX = pNpcData->m_iNPC_OBJINDEX;

			if(gObjIsConnected(iNPC_INDEX)
				&& gObj[iNPC_INDEX].Class == pNpcData->m_iNPC_NUM
				&& gObj[iNPC_INDEX].m_btCsNpcType != FALSE )
			{
				switch(gObj[iNPC_INDEX].Class)
				{
				case 277:
					{
						int iDF_LEVEL = pNpcData->m_iNPC_DF_LEVEL;

						if(iDF_LEVEL < 0)
						{
							iDF_LEVEL = 0;
						}

						if(iDF_LEVEL > CS_MAX_UPGRADE)
						{
							iDF_LEVEL = CS_MAX_UPGRADE;
						}

						gObj[iNPC_INDEX].m_btCsNpcDfLevel = iDF_LEVEL;
						gObj[iNPC_INDEX].m_Defense = g_iNpcDefense_CGATE[iDF_LEVEL];
					}
					break;
				case 283:
					{
						int iDF_LEVEL = pNpcData->m_iNPC_DF_LEVEL;

						if(iDF_LEVEL < 0)
						{
							iDF_LEVEL = 0;
						}

						if(iDF_LEVEL > CS_MAX_UPGRADE)
						{
							iDF_LEVEL = CS_MAX_UPGRADE;
						}

						gObj[iNPC_INDEX].m_btCsNpcDfLevel = iDF_LEVEL;
						gObj[iNPC_INDEX].m_Defense = g_iNpcDefense_CSTATUE[iDF_LEVEL];
						gObj[iNPC_INDEX].m_btCsNpcRgLevel = pNpcData->m_iNPC_RG_LEVEL;
					}
					break;
				}
			}

			break;
		}

		it++;
	}

	//LeaveCriticalSection(&m_critNpcData);
	//DebugLog("%s END",__FUNCTION__);
}

void CCastleSiege::SetAllCastleGateState(int bOpenType)
{
	//DebugLog("%s START",__FUNCTION__);
	//EnterCriticalSection(&m_critNpcData);

	//_CS_NPC_DATA * it = this->m_vtNpcData.begin();
	std::vector<_CS_NPC_DATA>::iterator it = this->m_vtNpcData.begin();
	int bExist = FALSE;

	while(it != this->m_vtNpcData.end())
	{
		std::vector<_CS_NPC_DATA>::iterator pNpcData = it;
		//_CS_NPC_DATA & pNpcData = _CS_NPC_DATA(*it);

		if(pNpcData->m_iNPC_NUM == 277
			&& gObjIsConnected(pNpcData->m_iNPC_OBJINDEX)
			&& gObj[pNpcData->m_iNPC_OBJINDEX].Class == 277)
		{
			switch(bOpenType)
			{
				case 0:
					SetGateBlockState(pNpcData->m_iNPC_SX,pNpcData->m_iNPC_SY,0);
					gObj[pNpcData->m_iNPC_OBJINDEX].m_btCsGateOpen = 0;
				break;
				case 1:
					SetGateBlockState(pNpcData->m_iNPC_SX,pNpcData->m_iNPC_SY,1);
					gObj[pNpcData->m_iNPC_OBJINDEX].m_btCsGateOpen = 1;
				break;
			}
		}

		it++;
	}

	//LeaveCriticalSection(&m_critNpcData);
	//DebugLog("%s END",__FUNCTION__);
}

void CCastleSiege::SetAllUserCsJoinSide()
{
	//DebugLog("%s START",__FUNCTION__);
	if(m_bCsTotalGuildInfoLoadOK == FALSE)
		return;

	////EnterCriticalSection(&m_critCsTotalGuildInfo);

	for(int iUSER = OBJ_STARTUSERINDEX; iUSER < OBJMAX; iUSER++)
	{
		if(gObjIsConnected(iUSER) == FALSE)
			continue;

		SetUserCsJoinSide(iUSER);
		NotifySelfCsJoinSide(iUSER);
	}

	////LeaveCriticalSection(&m_critCsTotalGuildInfo);
	//DebugLog("%s END",__FUNCTION__);
}

void CCastleSiege::SetUserCsJoinSide(int iUSER)
{
	//DebugLog("%s START",__FUNCTION__);

	if(m_bCsTotalGuildInfoLoadOK == FALSE)
		return;

	if(gObjIsConnected(iUSER) == FALSE)
		return;

	gObj[iUSER].m_ViewSkillState &= 0xFFF3FFFF;

	char szGuildName[MAX_GUILD_LEN+1] = {0};

	memcpy(szGuildName,gObj[iUSER].GuildName,MAX_GUILD_LEN);

	map<basic_string<char,char_traits<char> >,_CS_TOTAL_GUILD_DATA,less<basic_string<char,char_traits<char> > > >::iterator it = (map<basic_string<char,char_traits<char> >,_CS_TOTAL_GUILD_DATA,less<basic_string<char,char_traits<char> > > >::iterator)m_mapCsTotalGuildInfo.find(szGuildName);

	if(it != m_mapCsTotalGuildInfo.end())
	{
		gObj[iUSER].m_btCsJoinSide = it->second.m_iCsGuildID;

		//if (gObj[iUSER].m_btCsJoinSide == 1)
		//	gObj[iUSER].m_bCsGuildInvolved = true;
		//gObj[iUSER].m_bCsGuildInvolved = !(!it->second.m_bGuildInvolved);

		LogAddTD("[CastleSiege] CCastleSiege::SetUserCsJoinSide() [%s][%s] GUILD:[%s] Castle Side:%d",
			gObj[iUSER].AccountID,gObj[iUSER].Name,
			gObj[iUSER].GuildName,gObj[iUSER].m_btCsJoinSide);
	}

	map<basic_string<char>, _CS_TOTAL_GUILD_DATA>::iterator it2 = (map<basic_string<char>, _CS_TOTAL_GUILD_DATA>::iterator)m_mapCsTotalGuildInfo.begin();
	
	for( ; it2 != m_mapCsTotalGuildInfo.end(); it2++ )
	{
		basic_string<char> GuildName = it2->first;
		_CS_TOTAL_GUILD_DATA GuildData = _CS_TOTAL_GUILD_DATA(it2->second);

		LogAddTD("[CastleSiege] Siege Guild Check - GNAME:%s, CSGUID:%d, INVOLVED:%d",
			&GuildName[0], GuildData.m_iCsGuildID, GuildData.m_bGuildInvolved);
	}

	//DebugLog("%s END",__FUNCTION__);
}

void CCastleSiege::ReSpawnEnemyUser(int bRefreshOwnerUser)
{
	//DebugLog("%s START",__FUNCTION__);
	__try
	{
		int iTX;
		int iTY;
		unsigned char btMapAttr;

		for(int iUSER = OBJ_STARTUSERINDEX; iUSER < OBJMAX; iUSER++)
		{
			if(gObjIsConnected(iUSER) == FALSE)
				continue;

			if(gObj[iUSER].MapNumber != MAP_INDEX_CASTLESIEGE)
				continue;

			unsigned char btCsJoinSide = gObj[iUSER].m_btCsJoinSide;

			if(btCsJoinSide < 0)
			{
				btCsJoinSide = 0;
			}

			if(btCsJoinSide > 2)
			{
				btCsJoinSide = 2;
			}

			int bMoveOK = FALSE;

			switch(btCsJoinSide)
			{
				case 1:
				{
					if(bRefreshOwnerUser != FALSE)
					{
						gObj[iUSER].RegenMapNumber = gObj[iUSER].MapNumber;
						gObj[iUSER].RegenMapX = gObj[iUSER].X;
						gObj[iUSER].RegenMapY = gObj[iUSER].Y;

						gObjClearViewport(&gObj[iUSER]);

						GCTeleportSend(&gObj[iUSER],0xFF,gObj[iUSER].MapNumber,gObj[iUSER].X,gObj[iUSER].Y,gObj[iUSER].Dir);

						if(gObj[iUSER].m_Change >= 0)
						{
							gObjViewportListProtocolCreate(&gObj[iUSER]);
						}

						gObj[iUSER].RegenOk = TRUE;
					}
					bMoveOK = FALSE;
				}
				break;
				case 2:
				{
					for(int iCNT = 0; iCNT < 100; iCNT++)
					{
						iTX = (rand() % (g_iCsUserReSpawnArea[2][2] - g_iCsUserReSpawnArea[2][0])) + g_iCsUserReSpawnArea[2][0];
						iTY = (rand() % (g_iCsUserReSpawnArea[2][3] - g_iCsUserReSpawnArea[2][1])) + g_iCsUserReSpawnArea[2][1];

						btMapAttr = MapC[MAP_INDEX_CASTLESIEGE].m_attrbuf[iTY * 256 + iTX];

						if((btMapAttr & 4) != 4)
						{
							bMoveOK = TRUE;
							break;
						}
					}
				}
				break;
				case 0:
				{
					for(int iCNT = 0; iCNT < 100; iCNT++)
					{
						iTX = (rand()% (g_iCsUserReSpawnArea[0][2] - g_iCsUserReSpawnArea[0][0])) + g_iCsUserReSpawnArea[0][0];
						iTY = (rand()% (g_iCsUserReSpawnArea[0][3] - g_iCsUserReSpawnArea[0][1])) + g_iCsUserReSpawnArea[0][1];

						btMapAttr = MapC[MAP_INDEX_CASTLESIEGE].m_attrbuf[iTY * 256 + iTX];

						if((btMapAttr & 4) != 4)
						{
							bMoveOK = TRUE;
							break;
						}
					}
				}
				break;
			}

			if(bMoveOK != FALSE)
			{
				gObj[iUSER].m_State = 32;
				gObj[iUSER].X = iTX;
				gObj[iUSER].Y = iTY;
				gObj[iUSER].TX = iTX;
				gObj[iUSER].TX = iTY;
				gObj[iUSER].MapNumber = MAP_INDEX_CASTLESIEGE;
				gObj[iUSER].PathCount = 0;
				gObj[iUSER].Teleport = 0;

				gObjClearViewport(&gObj[iUSER]);
				GCTeleportSend(&gObj[iUSER],0xFF,MAP_INDEX_CASTLESIEGE,gObj[iUSER].X,gObj[iUSER].Y,gObj[iUSER].Dir);

				if(gObj[iUSER].m_Change >= 0)
				{
					gObjViewportListProtocolCreate(&gObj[iUSER]);
				}

				gObj[iUSER].RegenMapNumber = MAP_INDEX_CASTLESIEGE;
				gObj[iUSER].RegenMapX = iTX;
				gObj[iUSER].RegenMapY = iTY;
				gObj[iUSER].RegenOk = TRUE;
			}

			NotifySelfCsJoinSide(iUSER);
		}
	}__finally
	{
		//DebugLog("%s END",__FUNCTION__);
	}
}

/*
struct PMSG_ANS_NOTIFYCSSTART
{
	PBMSG_HEAD2 h;
	unsigned char btStartState;
};*/

void CCastleSiege::NotifyAllUserCsStartState(BYTE btStartState)
{
	//DebugLog("%s START",__FUNCTION__);
	__try
	{
		PMSG_ANS_NOTIFYCSSTART pMsg;

		pMsg.h.set((PBYTE)&pMsg,0xB2,0x17,sizeof(pMsg));
		pMsg.btStartState = btStartState;

		for(int i = OBJ_STARTUSERINDEX; i < OBJMAX; i++)
		{
			if(gObj[i].Connected == PLAYER_PLAYING && gObj[i].Type == OBJ_USER)
			{
				DataSend(i,(PBYTE)&pMsg,pMsg.h.size);
			}
		}
	}__finally
	{
		//DebugLog("%s END",__FUNCTION__);
	}
}

void CCastleSiege::SendAllUserAnyMsg(char * lpszMsg, int iType)
{
	//DebugLog("%s START",__FUNCTION__);
	__try
	{
		PMSG_NOTICE pNotice;
		pNotice.type = 0;	// 3
		pNotice.btCount = 0;	// 4
		pNotice.wDelay = 0;	// 6	
		pNotice.dwColor = 0;	// 8
		pNotice.btSpeed = 0;	// C

		if( lpszMsg == NULL )
			return;

		switch( iType )
		{
			case 1:
				TNotice::MakeNoticeMsg(&pNotice, 0, lpszMsg);
				TNotice::SetNoticeProperty(&pNotice, 10, _ARGB(255, 255, 200, 80), 1, 0, 20);
				TNotice::SendNoticeToAllUser(&pNotice);
			break;

			case 2:
				TNotice::MakeNoticeMsg(&pNotice, 0, lpszMsg);
			break;
		}

		for( int i = OBJ_STARTUSERINDEX; i < OBJMAX; i++ )
		{
			if( (gObj[i].Connected == PLAYER_PLAYING) && (gObj[i].Type == OBJ_USER) )
			{
				::DataSend(i, (LPBYTE)&pNotice, pNotice.h.size);
			}
		}
	}__finally
	{
		//DebugLog("%s END",__FUNCTION__);
	}
}

void CCastleSiege::SendMapServerGroupMsg(char * lpszMsg)
{
	//DebugLog("%s START",__FUNCTION__);
	__try
	{
		GS_GDReqMapSvrMsgMultiCast(this->m_iMapSvrGroup, lpszMsg);
	}__finally
	{
		//DebugLog("%s END",__FUNCTION__);
	}
}

void CCastleSiege::SetTaxRate(int iTaxType, int iTaxRate)
{
	//DebugLog("%s START",__FUNCTION__);
	__try
	{
		int iMaxTaxRate = 0;
	
		switch( iTaxType )
		{
			case 1:
				iMaxTaxRate = 3;
			break;
			case 2:
				iMaxTaxRate = 3;
			break;
			case 3:
				iMaxTaxRate = 300000;
			break;
		}

		if( iTaxRate >= 0 && iTaxRate > iMaxTaxRate )
		{
			return;
		}
	
		switch( iTaxType )
		{
			case 1:
				this->m_iTaxRateChaos = iTaxRate;
				g_CastleSiegeSync.SetTaxRateChaos(this->m_iTaxRateChaos);
			break;
			case 2:
				this->m_iTaxRateStore = iTaxRate;
				g_CastleSiegeSync.SetTaxRateStore(this->m_iTaxRateStore);
			break;
			case 3:
				this->m_iTaxHuntZone = iTaxRate;
				g_CastleSiegeSync.SetTaxHuntZone(this->m_iTaxHuntZone);
			break;
		}
	
		LogAddTD("[CastleSiege] Tax Rate Setted - Type:%d, Rate:%d", iTaxType, iTaxRate);
	}__finally
	{
		//DebugLog("%s END",__FUNCTION__);
	}
}

void CCastleSiege::SetCastleMoney(__int64 i64CastleMoney)
{
	//DebugLog("%s START",__FUNCTION__);
	__try
	{
		if (i64CastleMoney > MAX_ZEN)
			i64CastleMoney = MAX_ZEN;

		this->m_i64CastleMoney = i64CastleMoney;
	}__finally
	{
		//DebugLog("%s END",__FUNCTION__);
	}
}

//0056E140   1.00.19
int CCastleSiege::AddDbNPC(int iNpcType, int iNpcIndex)
{
	//DebugLog("%s START",__FUNCTION__);
	int iRETVAL = 0;
	
	//EnterCriticalSection(&this->m_critNpcData);

	//_CS_NPC_DATA* it = this->m_vtNpcData.begin();
	std::vector<_CS_NPC_DATA>::iterator it = this->m_vtNpcData.begin();

	while( it != this->m_vtNpcData.end() )
	{
		std::vector<_CS_NPC_DATA>::iterator NpcData = it;
		//_CS_NPC_DATA & NpcData = _CS_NPC_DATA(*it);
		//LogAddTD("[CastleSiege][AddDbNPC] NPC Record Index:%d Num:%d [%d/%d]", NpcData->m_iNPC_INDEX, NpcData->m_iNPC_NUM,iNpcType, iNpcIndex);

		if( (NpcData->m_iNPC_NUM == iNpcType) && (NpcData->m_iNPC_INDEX == iNpcIndex) )
		{
			if( gObjIsConnected(NpcData->m_iNPC_OBJINDEX) )
			{
				if( gObj[NpcData->m_iNPC_OBJINDEX].Class == iNpcType )
				{
					if( gObj[NpcData->m_iNPC_INDEX].Live == PLAYER_CONNECTED )
					{
						LogAddTD("[CastleSiege] CCastleSiege::AddDbNPC() ERROR - DB NPC EXIST (CLS:%d, IDX:%d(%d), MAXHP:%d, HP:%d, DF:%d, RG:%d)",
							NpcData->m_iNPC_NUM, 
							NpcData->m_iNPC_INDEX, 
							NpcData->m_iNPC_OBJINDEX, 
							gObj[NpcData->m_iNPC_OBJINDEX].MaxLife, 
							gObj[NpcData->m_iNPC_OBJINDEX].Life,
							gObj[NpcData->m_iNPC_OBJINDEX].m_btCsNpcDfLevel, 
							gObj[NpcData->m_iNPC_OBJINDEX].m_btCsNpcRgLevel);

						break;
					}
				}
			}

			int iObjIndex = gObjAddMonster(MAP_INDEX_CASTLESIEGE);

			if( iObjIndex >= 0 )
			{
				::gObjSetMonster(iObjIndex, NpcData->m_iNPC_NUM,"CCastleSiege::AddDbNPC");
					
				gObj[iObjIndex].m_PosNum = -1;
				gObj[iObjIndex].X = NpcData->m_iNPC_SX;
				gObj[iObjIndex].Y = NpcData->m_iNPC_SY;
				gObj[iObjIndex].MapNumber = MAP_INDEX_CASTLESIEGE;
				gObj[iObjIndex].TX = gObj[iObjIndex].X;
				gObj[iObjIndex].TY = gObj[iObjIndex].Y;
				gObj[iObjIndex].m_OldX = gObj[iObjIndex].X;
				gObj[iObjIndex].m_OldY = gObj[iObjIndex].Y;
				gObj[iObjIndex].Dir = NpcData->m_iNPC_DIR;
				gObj[iObjIndex].StartX = gObj[iObjIndex].X;
				gObj[iObjIndex].StartY = gObj[iObjIndex].Y;
				gObj[iObjIndex].DieRegen = 0;
				gObj[iObjIndex].MaxRegenTime = 0;
					
				gObj[iObjIndex].Life = NpcData->m_iNPC_BASE_HP;
				gObj[iObjIndex].MaxLife = NpcData->m_iNPC_BASE_MAXHP;
				gObj[iObjIndex].m_btCsNpcType = 1;
				
				int iBaseDFLevel = NpcData->m_iNPC_BASE_DF_LEVEL;

				if( iBaseDFLevel < 0 )	
				{
					iBaseDFLevel = 0;
				}

				if( iBaseDFLevel > 3 )
				{
					iBaseDFLevel = 3;
				}

				int iBaseRGLevel = NpcData->m_iNPC_BASE_RG_LEVEL;
					
				if( iBaseRGLevel < 0 )	
				{
					iBaseRGLevel = 0;
				}

				if( iBaseRGLevel > 3 )	
				{
					iBaseRGLevel = 3;
				}
				

				switch( NpcData->m_iNPC_NUM )
				{
					case 277:
					{
						gObj[iObjIndex].m_Defense = g_iNpcDefense_CGATE[iBaseDFLevel];
						gObj[iObjIndex].m_btCsNpcDfLevel = iBaseDFLevel;
					}
					break;
					case 283:
					{
						gObj[iObjIndex].m_Defense = g_iNpcDefense_CSTATUE[iBaseDFLevel];
						gObj[iObjIndex].m_btCsNpcDfLevel = iBaseDFLevel;
						gObj[iObjIndex].m_btCsNpcRgLevel = iBaseRGLevel;
					}
					break;
				}

				if( NpcData->m_iNPC_NUM == 277 )
				{
					this->SetGateBlockState(NpcData->m_iNPC_SX, NpcData->m_iNPC_SY, 1);
					gObj[iObjIndex].m_btCsGateOpen = 1;
						
					int iGateLevelerIndex = 0;
						
					if( NpcData->m_iCS_GATE_LEVER_INDEX == -1 )
					{
						iGateLevelerIndex = 1;
					}
					else if( this->CheckLeverAlive(NpcData->m_iCS_GATE_LEVER_INDEX) == FALSE )
					{
						iGateLevelerIndex = 1;
					}

					if( iGateLevelerIndex != 0 )
					{
						iGateLevelerIndex = this->CreateCsGateLever(NpcData->m_iNPC_SX, NpcData->m_iNPC_SY+4);

/*						if( iGateLevelerIndex < 0)
						{
							iGateLevelerIndex = 0;
						}
*/
						if( OBJMAX_RANGE(iGateLevelerIndex) )
						{
							it->m_iCS_GATE_LEVER_INDEX = iGateLevelerIndex;

							int iLinkIndex = this->LinkCsGateLever(iGateLevelerIndex, iObjIndex);

							if( iLinkIndex == 0 )
							{
								::LogAddC(2, "[CastleSiege] CCastleSiege::CreateDbNPC() ERROR - Failed to Create Gate Lever : GateIndex:%d",
									NpcData->m_iNPC_INDEX);
							}
						}
						else
						{
							it->m_iCS_GATE_LEVER_INDEX = -1;
							::LogAddC(2, "[CastleSiege] CCastleSiege::CreateDbNPC() ERROR - Failed to Create Gate Lever : GateIndex:%d",
								NpcData->m_iNPC_INDEX);
						}
					}
					else
					{
						int iLinkIndex = this->LinkCsGateLever(NpcData->m_iCS_GATE_LEVER_INDEX, iObjIndex);

						if( iLinkIndex == 0 )
						{
							::LogAddC(2, "[CastleSiege] CCastleSiege::CreateDbNPC() ERROR - Failed to Create Gate Lever : GateIndex:%d",
								NpcData->m_iNPC_INDEX);
						}
					}
				}
				it->m_iNPC_OBJINDEX = iObjIndex;
				it->m_iNPC_LIVE = PLAYER_LOGGED;
				it->m_bIN_USE = TRUE;

				iRETVAL = 1;

				if( NpcData->m_iNPC_NUM == 277 || NpcData->m_iNPC_NUM == 283 )
				{
					LogAddTD("[CastleSiege][CreateDBNPC] (Index:%d, Num:%d, DF:%d, RG:%d, MaxHP:%d, HP:%d )",
						NpcData->m_iNPC_INDEX, NpcData->m_iNPC_NUM, NpcData->m_iNPC_DF_LEVEL,
						NpcData->m_iNPC_RG_LEVEL, NpcData->m_iNPC_MAXHP, NpcData->m_iNPC_HP);
				}
				break;
			}
		}

		it++;
	}
	//LeaveCriticalSection(&this->m_critNpcData);

	//DebugLog("%s END",__FUNCTION__);
	return iRETVAL;
}

BOOL CCastleSiege::RepairDbNPC(int iNpcType, int iNpcIndex, int iNpcHP, int iNpcMaxHP)
{
	
	//DebugLog("%s START",__FUNCTION__);
	BOOL bEXIST = FALSE;

	//EnterCriticalSection(&m_critNpcData);

	//_CS_NPC_DATA* it = this->m_vtNpcData.begin();
	std::vector<_CS_NPC_DATA>::iterator it = this->m_vtNpcData.begin();

	while( it != this->m_vtNpcData.end() )
	{
		std::vector<_CS_NPC_DATA>::iterator NpcData = it;
		//_CS_NPC_DATA & NpcData = _CS_NPC_DATA(*it);

		if( NpcData->m_iNPC_NUM == iNpcType && NpcData->m_iNPC_INDEX == iNpcIndex && NpcData->m_bIN_USE == 1 && NpcData->m_iNPC_LIVE > 0 )
		{
			it->m_iNPC_MAXHP = iNpcMaxHP;
			it->m_iNPC_HP = it->m_iNPC_MAXHP;
			
			int iObjIndex = NpcData->m_iNPC_OBJINDEX;

			if( gObjIsConnected(iObjIndex) && gObj[iObjIndex].m_btCsNpcType != 0 && gObj[iObjIndex].Class == iNpcType )
			{
				gObj[iObjIndex].MaxLife = iNpcMaxHP;
				gObj[iObjIndex].Life = gObj[iObjIndex].MaxLife;
				
				bEXIST = TRUE;

				break;
			}
		}
		it++;
	}

	//LeaveCriticalSection(&m_critNpcData);
	//DebugLog("%s END",__FUNCTION__);
	return bEXIST;
}

//0056FCB0   1.00.19
BOOL CCastleSiege::UpgradeDbNPC(int iIndex, int iNpcType, int iNpcIndex, int iNpcUpType, int iNpcUpValue, int iNpcUpIndex)
{
	//DebugLog("%s START",__FUNCTION__);
	int iNEED_GEMOFDEFEND = 0;
	int iNEED_MONEY = 0;

	if( CS_CHECK_UPGRADE(iNpcUpIndex) == FALSE )
	{
		LogAddC(2, "[CastleSiege] CCastleSiege::UpgradeDbNPC() ERROR - lpMsg->iNpcUpIndex is Out of Bound : %d", iNpcUpIndex);
	//DebugLog("%s END",__FUNCTION__);
		return FALSE;
	}

	_CS_NPC_DATA pNpcData;

	BOOL bIsLive = GetNpcData(iNpcType, iNpcIndex, pNpcData);

	if( bIsLive == FALSE )
	{
		LogAddC(2, "[CastleSiege] CCastleSiege::UpgradeDbNPC() ERROR - bIsLive == FALSE", iNpcUpIndex);
		//DebugLog("%s END",__FUNCTION__);
		return FALSE;
	}

	switch( iNpcType )
	{
		case 277:
		{
			switch( iNpcUpType )
			{
				case 1:
				{
					iNEED_GEMOFDEFEND = g_iNpcUpDfLevel_CGATE[iNpcUpIndex][1];
					iNEED_MONEY = g_iNpcUpDfLevel_CGATE[iNpcUpIndex][2];
					
					if( gObj[iIndex].Money < iNEED_MONEY )
					{
						GCAnsNpcUpgrade(iIndex, 3, iNpcType, iNpcIndex, iNpcUpType, iNpcUpValue);
						//DebugLog("%s END",__FUNCTION__);
						return FALSE;
					}
					int iDelResult = DelGemOfDefend(iIndex, iNEED_GEMOFDEFEND);

					if( iDelResult == FALSE )
					{
						GCAnsNpcUpgrade(iIndex, 4, iNpcType, iNpcIndex, iNpcUpType, iNpcUpValue);
						//DebugLog("%s END",__FUNCTION__);
						return FALSE;	
					}

					gObj[iIndex].Money -= iNEED_MONEY;
					this->UpgradeDbNPC_DFLEVEL(iNpcType, iNpcIndex, iNpcUpIndex+1);
				}
				break;
				case 3:
				{
					iNEED_GEMOFDEFEND = g_iNpcUpMaxHP_CGATE[iNpcUpIndex][1];
					iNEED_MONEY = g_iNpcUpMaxHP_CGATE[iNpcUpIndex][2];

					if( gObj[iIndex].Money < iNEED_MONEY )
					{
						GCAnsNpcUpgrade(iIndex, 3, iNpcType, iNpcIndex, iNpcUpType, iNpcUpValue);
						//DebugLog("%s END",__FUNCTION__);
						return FALSE;
					}
					int iDelResult = DelGemOfDefend(iIndex, iNEED_GEMOFDEFEND);

					if( iDelResult == FALSE )
					{
						GCAnsNpcUpgrade(iIndex, 4, iNpcType, iNpcIndex, iNpcUpType, iNpcUpValue);
						//DebugLog("%s END",__FUNCTION__);
						return FALSE;	
					}

					gObj[iIndex].Money -= iNEED_MONEY;
					this->UpgradeDbNPC_MAXHP(iNpcType, iNpcIndex, g_iNpcUpMaxHP_CGATE[iNpcUpIndex][0]);
				}
				break;
				default:
				{
					LogAddTD("[CastleSiege] CGReqNpcUpgrade() ERROR - UpType doesn't Exist [%s][%s], Guild:(%s)(%d), NPC CLS:%d, IDX:%d, UPTYPE:%d, UPVAL:%d",
						gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus, 
						iNpcType, iNpcUpIndex, iNpcUpType, iNpcUpValue);

					GCAnsNpcUpgrade(iIndex, 5, iNpcType, iNpcIndex, iNpcUpType, iNpcUpValue);
					//DebugLog("%s END",__FUNCTION__);
					return FALSE;	
				}
				break;
			}
		}
		break;
		case 283:
		{
			switch( iNpcUpType )
			{
				case 1:
				{
					iNEED_GEMOFDEFEND = g_iNpcUpDfLevel_CSTATUE[iNpcUpIndex][1];
					iNEED_MONEY = g_iNpcUpDfLevel_CSTATUE[iNpcUpIndex][2];
					
					if( gObj[iIndex].Money < iNEED_MONEY )
					{
						GCAnsNpcUpgrade(iIndex, 3, iNpcType, iNpcIndex, iNpcUpType, iNpcUpValue);
						//DebugLog("%s END",__FUNCTION__);
						return FALSE;
					}
					int iDelResult = DelGemOfDefend(iIndex, iNEED_GEMOFDEFEND);

					if( iDelResult == FALSE )
					{
						GCAnsNpcUpgrade(iIndex, 4, iNpcType, iNpcIndex, iNpcUpType, iNpcUpValue);
						//DebugLog("%s END",__FUNCTION__);
						return FALSE;	
					}

					gObj[iIndex].Money -= iNEED_MONEY;
					this->UpgradeDbNPC_DFLEVEL(iNpcType, iNpcIndex, g_iNpcUpDfLevel_CSTATUE[iNpcUpIndex][0]);
				}
				break;
				case 2:
				{
					iNEED_GEMOFDEFEND = g_iNpcUpRgLevel_CSTATUE[iNpcUpIndex][1];
					iNEED_MONEY = g_iNpcUpRgLevel_CSTATUE[iNpcUpIndex][2];

					if( gObj[iIndex].Money < iNEED_MONEY )
					{
						GCAnsNpcUpgrade(iIndex, 3, iNpcType, iNpcIndex, iNpcUpType, iNpcUpValue);
						//DebugLog("%s END",__FUNCTION__);
						return FALSE;
					}
					int iDelResult = DelGemOfDefend(iIndex, iNEED_GEMOFDEFEND);

					if( iDelResult == FALSE )
					{
						GCAnsNpcUpgrade(iIndex, 4, iNpcType, iNpcIndex, iNpcUpType, iNpcUpValue);
						//DebugLog("%s END",__FUNCTION__);
						return FALSE;	
					}

					gObj[iIndex].Money -= iNEED_MONEY;
					this->UpgradeDbNPC_RGLEVEL(iNpcType, iNpcIndex, g_iNpcUpRgLevel_CSTATUE[iNpcUpIndex][0]);
				}
				break;
				case 3:
				{
					iNEED_GEMOFDEFEND = g_iNpcUpMaxHP_CSTATUE[iNpcUpIndex][1];
					iNEED_MONEY = g_iNpcUpMaxHP_CSTATUE[iNpcUpIndex][2];

					if( gObj[iIndex].Money < iNEED_MONEY )
					{
						GCAnsNpcUpgrade(iIndex, 3, iNpcType, iNpcIndex, iNpcUpType, iNpcUpValue);
						//DebugLog("%s END",__FUNCTION__);
						return FALSE;
					}
					int iDelResult = DelGemOfDefend(iIndex, iNEED_GEMOFDEFEND);

					if( iDelResult == FALSE )
					{
						GCAnsNpcUpgrade(iIndex, 4, iNpcType, iNpcIndex, iNpcUpType, iNpcUpValue);
						//DebugLog("%s END",__FUNCTION__);
						return FALSE;	
					}

					gObj[iIndex].Money -= iNEED_MONEY;
					this->UpgradeDbNPC_MAXHP(iNpcType, iNpcIndex, g_iNpcUpMaxHP_CSTATUE[iNpcUpIndex][0]);
				}
				break;
				default:
				{
					GCAnsNpcUpgrade(iIndex, 5, iNpcType, iNpcIndex, iNpcUpType, iNpcUpValue);
					//DebugLog("%s END",__FUNCTION__);
					return FALSE;	
				}
				break;
			}
		}
		break;
	}

	GCMoneySend(iIndex, gObj[iIndex].Money);

	//DebugLog("%s END",__FUNCTION__);
	return TRUE;
}

struct CSP_REQ_NPCSAVEDATA {
	PWMSG_HEAD h; // 0
	WORD wMapSvrNum; // 4
	int iCount; // 8
};

struct CSP_NPCSAVEDATA {
	int iNpcNumber; // 0
	int iNpcIndex; // 4
	int iNpcDfLevel; // 8
	int iNpcRgLevel; // c
	int iNpcMaxHp; // 10
	int iNpcHp; // 14 
	BYTE btNpcX; // 18
	BYTE btNpcY; // 19
	BYTE btNpcDIR; // 1a
};

BOOL CCastleSiege::FirstCreateDbNPC()
{
	//DebugLog("%s START",__FUNCTION__);
	char cBUFFER[4240];
	CSP_REQ_NPCSAVEDATA* lpMsg;
	CSP_NPCSAVEDATA* lpMsgBody;
	
	if( this->m_iCastleDataLoadState != CASTLESIEGE_DATALOAD_4 )
	{
		LogAddC(2, "[CastleSiege] CCastleSiege::FirstCreateDbNPC() - m_iCastleDataLoadState != CASTLESIEGE_DATALOAD_4");
		return FALSE;
	}
	
	lpMsg = (CSP_REQ_NPCSAVEDATA*)cBUFFER;
	lpMsgBody = (CSP_NPCSAVEDATA*)&cBUFFER[sizeof(CSP_REQ_NPCSAVEDATA)];
	int iCOUNT = 0;

	//EnterCriticalSection(&this->m_critNpcData);
	
	//_CS_NPC_DATA *it = this->m_vtNpcData.begin();
	std::vector<_CS_NPC_DATA>::iterator it = this->m_vtNpcData.begin();
	BOOL bExist = FALSE;

	while( it != this->m_vtNpcData.end() )
	{
		std::vector<_CS_NPC_DATA>::iterator pNpcData = it;
		//_CS_NPC_DATA & pNpcData = _CS_NPC_DATA(*it);

		if( pNpcData->m_bNPC_DBSAVE == TRUE )
		{
			it->SetBaseValue();
			it->m_iNPC_LIVE = 1;
			it->m_iNPC_SIDE = 1;

			lpMsgBody[iCOUNT].iNpcNumber = pNpcData->m_iNPC_NUM;
			lpMsgBody[iCOUNT].iNpcIndex = pNpcData->m_iNPC_INDEX;
			lpMsgBody[iCOUNT].iNpcDfLevel = pNpcData->m_iNPC_BASE_DF_LEVEL;
			lpMsgBody[iCOUNT].iNpcRgLevel = pNpcData->m_iNPC_BASE_RG_LEVEL;
			lpMsgBody[iCOUNT].iNpcMaxHp = pNpcData->m_iNPC_BASE_MAXHP;
			lpMsgBody[iCOUNT].iNpcHp = pNpcData->m_iNPC_BASE_HP;
			lpMsgBody[iCOUNT].btNpcX = pNpcData->m_iNPC_BASE_SX;
			lpMsgBody[iCOUNT].btNpcY = pNpcData->m_iNPC_BASE_SY;
			lpMsgBody[iCOUNT].btNpcDIR = pNpcData->m_iNPC_BASE_DIR;

			iCOUNT++;
		}
		it++;
	}

	//LeaveCriticalSection(&this->m_critNpcData);

	lpMsg->wMapSvrNum = this->m_iMapSvrGroup;
	lpMsg->iCount = iCOUNT;

	lpMsg->h.set((LPBYTE)&lpMsg->h, 0x84, (iCOUNT * sizeof(CSP_NPCSAVEDATA)) + sizeof(CSP_REQ_NPCSAVEDATA));

	cDBSMng.Send((PCHAR)lpMsg, (iCOUNT * sizeof(CSP_NPCSAVEDATA)) + sizeof(CSP_REQ_NPCSAVEDATA));

	//DebugLog("%s END",__FUNCTION__);	
	return TRUE;
}

BOOL CCastleSiege::SetCastleNpcData(CSP_CSINITDATA* lpMsg, int iCOUNT)
{
	//DebugLog("%s START",__FUNCTION__);
	if( this->m_iCastleDataLoadState != CASTLESIEGE_DATALOAD_3 )
	{
		LogAddC(2, "[CastleSiege] CCastleSiege::SetCastleInitData() - m_iCastleDataLoadState != CASTLESIEGE_DATALOAD_3");
		//DebugLog("%s END",__FUNCTION__);
		return FALSE;
	}

	if( lpMsg == NULL )
	{
		//DebugLog("%s END",__FUNCTION__);
		return FALSE;
	}

	if( iCOUNT < 0 )
	{
		//DebugLog("%s END",__FUNCTION__);
		return FALSE;
	}

	for( int iNPC_COUNT = 0; iNPC_COUNT < iCOUNT; iNPC_COUNT++ )
	{
		//EnterCriticalSection(&this->m_critNpcData);

		//_CS_NPC_DATA *it = this->m_vtNpcData.begin();
		std::vector<_CS_NPC_DATA>::iterator it = this->m_vtNpcData.begin();
		BOOL bNpcLoaded = FALSE;

		while( it != this->m_vtNpcData.end() )
		{
			std::vector<_CS_NPC_DATA>::iterator NpcData = it;
			//_CS_NPC_DATA & NpcData = _CS_NPC_DATA(*it);

			if( NpcData->m_iNPC_NUM == lpMsg[iNPC_COUNT].iNpcNumber )
			{
				if( NpcData->m_iNPC_INDEX == lpMsg[iNPC_COUNT].iNpcIndex )
				{
					it->m_iNPC_LIVE	= 1;
					it->m_iNPC_DF_LEVEL = lpMsg[iNPC_COUNT].iNpcDfLevel;
					it->m_iNPC_RG_LEVEL = lpMsg[iNPC_COUNT].iNpcRgLevel;

					it->m_iNPC_MAXHP = lpMsg[iNPC_COUNT].iNpcMaxHp;
					it->m_iNPC_HP = lpMsg[iNPC_COUNT].iNpcHp;

					it->m_iNPC_SX = lpMsg[iNPC_COUNT].btNpcX;
					it->m_iNPC_SY = lpMsg[iNPC_COUNT].btNpcY;
					it->m_iNPC_DX = -1;
					it->m_iNPC_DY = -1;
					it->m_iNPC_DIR = lpMsg[iNPC_COUNT].btNpcDIR;
					it->m_iNPC_SIDE = 1;

					LogAddTD("[CastleSiege][SetCastleNpcData][Update] Count:%d %d %d %d %d %d %d %d %d %d",
						iNPC_COUNT,
						lpMsg[iNPC_COUNT].iNpcNumber,
						lpMsg[iNPC_COUNT].iNpcIndex,
						lpMsg[iNPC_COUNT].iNpcDfLevel,
						lpMsg[iNPC_COUNT].iNpcRgLevel,
						lpMsg[iNPC_COUNT].iNpcMaxHp,
						lpMsg[iNPC_COUNT].iNpcHp,
						lpMsg[iNPC_COUNT].btNpcX,
						lpMsg[iNPC_COUNT].btNpcY,
						lpMsg[iNPC_COUNT].btNpcDIR
					);
					bNpcLoaded = TRUE;

					break;
				}
			}
			it++;
		}
		//LeaveCriticalSection(&this->m_critNpcData);

		if( bNpcLoaded == FALSE )
		{
			_CS_NPC_DATA m_NpcData;

			m_NpcData.m_bIN_USE	= TRUE;
			m_NpcData.m_iNPC_NUM = lpMsg[iNPC_COUNT].iNpcNumber;
			m_NpcData.m_iNPC_INDEX = lpMsg[iNPC_COUNT].iNpcIndex;
			m_NpcData.m_iNPC_LIVE = 1;
			m_NpcData.m_iNPC_SIDE = 1;
			m_NpcData.m_iNPC_BASE_DF_LEVEL = lpMsg[iNPC_COUNT].iNpcDfLevel;
			m_NpcData.m_iNPC_DF_LEVEL = m_NpcData.m_iNPC_BASE_DF_LEVEL;
			m_NpcData.m_iNPC_BASE_RG_LEVEL = lpMsg[iNPC_COUNT].iNpcRgLevel;
			m_NpcData.m_iNPC_RG_LEVEL = m_NpcData.m_iNPC_BASE_RG_LEVEL;

			m_NpcData.m_iNPC_MAXHP = lpMsg[iNPC_COUNT].iNpcMaxHp;
			m_NpcData.m_iNPC_HP = m_NpcData.m_iNPC_MAXHP;

			m_NpcData.m_iNPC_BASE_MAXHP = lpMsg[iNPC_COUNT].iNpcHp;
			m_NpcData.m_iNPC_BASE_HP = m_NpcData.m_iNPC_BASE_MAXHP;

			m_NpcData.m_iNPC_SX	= lpMsg[iNPC_COUNT].btNpcX;
			m_NpcData.m_iNPC_DX	= m_NpcData.m_iNPC_BASE_SX;
			m_NpcData.m_iNPC_SY	= lpMsg[iNPC_COUNT].btNpcY;
			m_NpcData.m_iNPC_SY	= m_NpcData.m_iNPC_BASE_SY;
			
			m_NpcData.m_iNPC_BASE_DX = -1;
			m_NpcData.m_iNPC_BASE_SX = m_NpcData.m_iNPC_BASE_DX;
			
			m_NpcData.m_iNPC_BASE_SY = -1;
			m_NpcData.m_iNPC_BASE_DY = m_NpcData.m_iNPC_BASE_SY;
			
			m_NpcData.m_iNPC_BASE_DIR = lpMsg[iNPC_COUNT].btNpcDIR;
			m_NpcData.m_iNPC_DIR = m_NpcData.m_iNPC_BASE_DIR;

			LogAddTD("[CastleSiege][SetCastleNpcData][Insert] Count:%d %d %d %d %d %d %d %d %d %d",
				iNPC_COUNT,
				lpMsg[iNPC_COUNT].iNpcNumber,
				lpMsg[iNPC_COUNT].iNpcIndex,
				lpMsg[iNPC_COUNT].iNpcDfLevel,
				lpMsg[iNPC_COUNT].iNpcRgLevel,
				lpMsg[iNPC_COUNT].iNpcMaxHp,
				lpMsg[iNPC_COUNT].iNpcHp,
				lpMsg[iNPC_COUNT].btNpcX,
				lpMsg[iNPC_COUNT].btNpcY,
				lpMsg[iNPC_COUNT].btNpcDIR
			);
			//EnterCriticalSection(&this->m_critNpcData);
			this->m_vtNpcData.push_back(m_NpcData);
			//LeaveCriticalSection(&this->m_critNpcData);
		}
	}
	
	//DebugLog("%s END",__FUNCTION__);
	return TRUE;
}

BOOL CCastleSiege::SetCastleInitData(CSP_ANS_CSINITDATA* lpMsg)
{
	//DebugLog("%s START",__FUNCTION__);
	__try
	{
		if( this->m_iCastleDataLoadState != CASTLESIEGE_DATALOAD_3 )
		{
			LogAddC(2, "[CastleSiege] CCastleSiege::SetCastleInitData() - m_iCastleDataLoadState != CASTLESIEGE_DATALOAD_3");
			return FALSE;
		}

		if( lpMsg == NULL )
			return FALSE;

		memset(&this->m_tmStartDate, 0, sizeof(this->m_tmStartDate));
		memset(&this->m_tmEndDate, 0, sizeof(this->m_tmEndDate));
	
		this->m_tmStartDate.wYear = lpMsg->wStartYear;
		this->m_tmStartDate.wMonth = lpMsg->btStartMonth;
		this->m_tmStartDate.wDay = lpMsg->btStartDay;
		this->m_tmEndDate.wYear = lpMsg->wEndYear;
		this->m_tmEndDate.wMonth = lpMsg->btEndMonth;
		this->m_tmEndDate.wDay = lpMsg->btEndDay;
		this->m_btIsSiegeGuildList = lpMsg->btIsSiegeGuildList;
		this->m_btIsSiegeEnded = lpMsg->btIsSiegeEnded;

		if (lpMsg->btIsCastleOccupied > 0)
		{
			this->m_btIsCastleOccupied = TRUE;
		} else {
			this->m_btIsCastleOccupied = FALSE;
		}

		memset(&this->m_szCastleOwnerGuild, 0, sizeof(this->m_szCastleOwnerGuild));
		memcpy(&this->m_szCastleOwnerGuild, lpMsg->szCastleOwnGuild, sizeof(lpMsg->szCastleOwnGuild));
		g_CastleSiegeSync.SetCastleOwnerGuild(this->m_szCastleOwnerGuild);

		if (lpMsg->i64CastleMoney > MAX_ZEN)
		{
			lpMsg->i64CastleMoney = MAX_ZEN;
		}

		this->m_i64CastleMoney = lpMsg->i64CastleMoney;
		this->m_iTaxRateChaos = lpMsg->iTaxRateChaos;
		this->m_iTaxRateStore = lpMsg->iTaxRateStore;
		this->m_iTaxHuntZone = lpMsg->iTaxHuntZone;

		g_CastleSiegeSync.SetTaxRateChaos(this->m_iTaxRateChaos);
		g_CastleSiegeSync.SetTaxRateStore(this->m_iTaxRateStore);
		g_CastleSiegeSync.SetTaxHuntZone(this->m_iTaxHuntZone);

		LogAddTD("[CastleSiege] CCastleSiege::LoadData() - Siege Owner:%s Occupy:%d Tax:%d,%d",
			this->m_szCastleOwnerGuild, this->m_btIsCastleOccupied, 
			this->m_iTaxRateChaos, this->m_iTaxRateStore
		);

		int iSTART_DATE_NUM = MACRO2(( MACRO1(m_tmStartDate.wDay) | MACRO1(m_tmStartDate.wMonth) << 8 ) & 0xFFFF ) | MACRO2(m_tmStartDate.wYear) << 16;
		int iEND_DATE_NUM = MACRO2(( MACRO1(m_tmEndDate.wDay) | MACRO1(m_tmEndDate.wMonth) << 8 ) & 0xFFFF ) | MACRO2(m_tmEndDate.wYear) << 16;
	
		m_tmSiegeEndSchedule = m_tmStartDate;

		SYSTEMTIME tmStateTime;
		GetStateDate(CASTLESIEGE_STATE_ENDCYCLE, &tmStateTime);

		LogAddTD("[CastleSiege] CCastleSiege::LoadData() - Siege Schedule Start Date (%d-%d-%d)",
			m_tmStartDate.wYear, m_tmStartDate.wMonth, m_tmStartDate.wDay);
	
		LogAddTD("[CastleSiege] CCastleSiege::LoadData() - Siege Schedule Date (%d-%d-%d)",
			tmStateTime.wDay, tmStateTime.wHour, tmStateTime.wMinute);

		GetNextDay(&m_tmSiegeEndSchedule, tmStateTime.wDay, tmStateTime.wHour, tmStateTime.wMinute, 0);

		LogAddTD("[CastleSiege] CCastleSiege::LoadData() - Siege Schedule End Date (%d-%d-%d(%d:%d:%d)",
			m_tmSiegeEndSchedule.wYear, m_tmSiegeEndSchedule.wMonth, m_tmSiegeEndSchedule.wDay,
			m_tmSiegeEndSchedule.wHour, m_tmSiegeEndSchedule.wMinute, m_tmSiegeEndSchedule.wSecond);

		if( iSTART_DATE_NUM > iEND_DATE_NUM )
		{
			LogAddC(2, "[CastleSiege] CCastleSiege::SetCastleInitData() - iSTART_DATE_NUM > iEND_DATE_NUM");
			return FALSE;
		}
		return TRUE;
	}__finally
	{
		//DebugLog("%s END",__FUNCTION__);
	}
}

struct CSP_REQ_CSGUILDUNIONINFO // 0xc
{
    PWMSG_HEAD h; // +0x0(0x4)
    WORD wMapSvrNum; // +0x4(0x2)
    int iCount; // +0x8(0x4)
};

void CCastleSiege::SetCalcRegGuildList(CSP_CALCREGGUILDLIST* lpMsg, int iCOUNT)
{
	//DebugLog("%s START",__FUNCTION__);
	if( lpMsg == NULL )
	{
		//DebugLog("%s END",__FUNCTION__);
		return;
	}

	if( iCOUNT > 100 ) 
	{
		iCOUNT = 100;
	}

	if( iCOUNT <= 0 )
	{
		//DebugLog("%s END",__FUNCTION__);
		return;
	}

	m_bCsBasicGuildInfoLoadOK = FALSE;
	m_bCsTotalGuildInfoLoadOK = FALSE;

	std::vector<_CS_REG_GUILD_DATA> vtRegGuildScore;

	for( int iGCNT = 0; iGCNT < iCOUNT; iGCNT++ )
	{
		_CS_REG_GUILD_DATA GuildData;

		char szGuildName[MAX_GUILD_LEN+1] = {0};

		memcpy(szGuildName, lpMsg[iGCNT].szGuildName, MAX_GUILD_LEN);
		
		GuildData.m_bIN_USE = TRUE;
		GuildData.m_strGuildName = szGuildName;
		GuildData.m_iRegMarkCount = lpMsg[iGCNT].iRegMarkCount;
		GuildData.m_iGuildMasterLevel = lpMsg[iGCNT].iGuildMasterLevel;
		GuildData.m_iGuildMemberCount = lpMsg[iGCNT].iGuildMemberCount;
		GuildData.m_iSeqNum = 0x100000 - lpMsg[iGCNT].iSeqNum; 

		//int dwHighScore = ( ( lpMsg[iGCNT].iRegMarkCount * 5 ) + lpMsg[iGCNT].iGuildMemberCount + (lpMsg[iGCNT].iGuildMasterLevel / 4));
		//#define MACRO3(value) (((__int64) value & 0xFFFFFFFF ))
		//GuildData.m_i64TotolScore = ((__int64)(DWORD)MACRO3(lpMsg[iGCNT].iSeqNum)) | (((__int64)(DWORD)MACRO3(dwHighScore))<<32);

		GuildData.m_i64TotolScore = lpMsg[iGCNT].iRegMarkCount + lpMsg[iGCNT].iGuildMemberCount;

		//LogAddTD("[CastleSiege] CCastleSiege::SetCalcRegGuildList() - G:%s,R:%d,GM LVL:%d,CNT:%d,NUM:%d,TS:%d",
		//	GuildData.m_strGuildName, GuildData.m_iRegMarkCount, GuildData.m_iGuildMasterLevel,
		//	GuildData.m_iGuildMemberCount,GuildData.m_iSeqNum,GuildData.m_i64TotolScore);

		vtRegGuildScore.push_back(GuildData);
	}
 
	if( vtRegGuildScore.empty() )
	{
		//DebugLog("%s END",__FUNCTION__);
		return;
	}

	//std::sort( vtRegGuildScore.begin(), vtRegGuildScore.end(), RegGuildScoreCompFunc1);
	sort(vtRegGuildScore.begin(), vtRegGuildScore.end(), this->RegGuildScoreCompFunc);
	//RegGuildScoreCompFunc1

	int iGUILD_COUNT = vtRegGuildScore.size();
	if( iGUILD_COUNT > 3 ) 
	{
		iGUILD_COUNT = 3;
	}

	m_mapCsBasicGuildInfo.clear();


	for( int iGCNT = 0; iGCNT < iGUILD_COUNT; iGCNT++ )
	{
		_CS_TOTAL_GUILD_DATA GuildData;
		
		GuildData.m_bIN_USE = TRUE;
		GuildData.m_iCsGuildID = (iGCNT + 2);
		GuildData.m_bGuildInvolved = TRUE;

		basic_string<char> gn = vtRegGuildScore[iGCNT].m_strGuildName;
		char gName[11]={0};
		memcpy(&gName[0],  &gn[0], MAX_GUILD_LEN);

		LogAddTD("[CastleSiege] CCastleSiege::SetCalcRegGuildList() - iGCNT:%d,SIDE:%d,NAME:%s,INVOLVED:%d",
			iGCNT, GuildData.m_iCsGuildID, gName, GuildData.m_bGuildInvolved);

		m_mapCsBasicGuildInfo.insert( pair<basic_string<char>, _CS_TOTAL_GUILD_DATA>(vtRegGuildScore[iGCNT].m_strGuildName, GuildData) );
	}

	if( m_btIsCastleOccupied == TRUE )
	{
		if( strcmp(m_szCastleOwnerGuild, "") != 0 )
		{
			_CS_TOTAL_GUILD_DATA GuildData;

			GuildData.m_bIN_USE = TRUE;
			GuildData.m_bGuildInvolved = TRUE;
			GuildData.m_iCsGuildID = 1;

			LogAddTD("[CastleSiege] CCastleSiege::SetCalcRegGuildList() - OWNER SIDE:%d,NAME:%s",
				GuildData.m_iCsGuildID, m_szCastleOwnerGuild);

			m_mapCsBasicGuildInfo.insert( pair<basic_string<char>, _CS_TOTAL_GUILD_DATA>(m_szCastleOwnerGuild, GuildData) );
		}
	}

	char cBUFFER[2080];

	CSP_REQ_CSGUILDUNIONINFO* lpMsgSend = (CSP_REQ_CSGUILDUNIONINFO*)cBUFFER;
	CSP_CSGUILDUNIONINFO* lpMsgSendBody = (CSP_CSGUILDUNIONINFO*)&cBUFFER[sizeof(CSP_REQ_CSGUILDUNIONINFO)];

	lpMsgSend->wMapSvrNum = m_iMapSvrGroup;
	lpMsgSend->iCount = 0;

	m_mapCsTotalGuildInfo.clear();

	for( map<basic_string<char>, _CS_TOTAL_GUILD_DATA>::iterator it = (map<basic_string<char>, _CS_TOTAL_GUILD_DATA>::iterator)m_mapCsBasicGuildInfo.begin(); it != m_mapCsBasicGuildInfo.end(); it++ )
	{
		if( lpMsgSend->iCount > 150 )
			break;

		m_mapCsTotalGuildInfo.insert( pair<basic_string<char>, _CS_TOTAL_GUILD_DATA>(it->first, it->second));

		basic_string<char> GuildName = it->first;

		memcpy(&lpMsgSendBody[lpMsgSend->iCount].szGuildName,  &GuildName[0], MAX_GUILD_LEN);
		lpMsgSendBody[lpMsgSend->iCount].iCsGuildID = it->second.m_iCsGuildID;

		lpMsgSend->iCount++;
	}

	m_bCsBasicGuildInfoLoadOK = TRUE;

	lpMsgSend->h.set((LPBYTE)lpMsgSend, 0x86, (lpMsgSend->iCount * sizeof(CSP_CSGUILDUNIONINFO))+ sizeof(CSP_REQ_CSGUILDUNIONINFO));
	cDBSMng.Send((PCHAR)lpMsgSend, (lpMsgSend->iCount * sizeof(CSP_CSGUILDUNIONINFO))+ sizeof(CSP_REQ_CSGUILDUNIONINFO));

	//DebugLog("%s END",__FUNCTION__);
}


void CCastleSiege::MakeCsTotalGuildInfo(CSP_CSGUILDUNIONINFO* lpMsg, int iCOUNT)
{
	//DebugLog("%s START",__FUNCTION__);
	if( lpMsg == NULL )
	{
		//DebugLog("%s END",__FUNCTION__);
		return;
	}

	if( m_bCsBasicGuildInfoLoadOK == FALSE )
	{
		LogAddC(2, "[CastleSiege] CCastleSiege::MakeCsTotalGuildInfo() ERROR - m_bCsBasicGuildInfoLoadOK == FALSE");
		//DebugLog("%s END",__FUNCTION__);
		return;
	}
	if( iCOUNT <= 0 )
	{
		LogAddC(2, "[CastleSiege] CCastleSiege::MakeCsTotalGuildInfo() ERROR - iCOUNT <= 0 : %d", iCOUNT);
		//DebugLog("%s END",__FUNCTION__);
		return;
	}

	m_mapCsTotalGuildInfo.clear();

	for( int iGCNT = 0; iGCNT < iCOUNT; iGCNT++ )
	{
		char szGuildName[MAX_GUILD_LEN+1] = {0};

		memcpy(szGuildName, lpMsg[iGCNT].szGuildName, MAX_GUILD_LEN);

		map<basic_string<char>, _CS_TOTAL_GUILD_DATA>::iterator it = (map<basic_string<char>, _CS_TOTAL_GUILD_DATA>::iterator)m_mapCsTotalGuildInfo.find(szGuildName);

				PMSG_TEST * pMsg = (PMSG_TEST *)lpMsg;
		if (iGCNT > 150)
			continue;

		DebugLog("%s RECV: [%s][%d]",__FUNCTION__,szGuildName,lpMsg[iGCNT].iCsGuildID);

		_CS_TOTAL_GUILD_DATA GuildData;
		
		GuildData.m_bIN_USE = TRUE;
		GuildData.m_iCsGuildID = lpMsg[iGCNT].iCsGuildID;
		GuildData.m_bGuildInvolved = FALSE;

		m_mapCsTotalGuildInfo.insert( pair<basic_string<char>, _CS_TOTAL_GUILD_DATA>(szGuildName, GuildData));
	}

	m_bCsTotalGuildInfoLoadOK = TRUE;
	SaveCsTotalGuildInfo();
	SetAllUserCsJoinSide();

	//DebugLog("%s END",__FUNCTION__);
}


void CCastleSiege::SetCsTotalGuildInfo(CSP_CSLOADTOTALGUILDINFO* lpMsg, int iCOUNT)
{
	//DebugLog("%s START",__FUNCTION__);
	if( lpMsg == NULL )
	{
		//DebugLog("%s END",__FUNCTION__);
		return;
	}

	if( iCOUNT <= 0 )
	{
		LogAddC(2, "[CastleSiege] CCastleSiege::SetCsTotalGuildInfo() ERROR - iCOUNT <= 0 : %d",
			iCOUNT);

		//DebugLog("%s END",__FUNCTION__);
		return;
	}

	for( int iGCNT = 0; iGCNT < iCOUNT; iGCNT++ )
	{
		char szGuildName[MAX_GUILD_LEN+1] = {0};

		memcpy(szGuildName, lpMsg[iGCNT].szGuildName, MAX_GUILD_LEN);

		_CS_TOTAL_GUILD_DATA GuildData;
		
		GuildData.m_bIN_USE = TRUE;
		GuildData.m_iCsGuildID = lpMsg[iGCNT].iCsGuildID;
		GuildData.m_bGuildInvolved = lpMsg[iGCNT].iGuildInvolved;

		m_mapCsTotalGuildInfo.insert( pair<basic_string<char>, _CS_TOTAL_GUILD_DATA>(szGuildName, GuildData));
		
		if( lpMsg[iGCNT].iGuildInvolved == TRUE )
		{
			m_mapCsBasicGuildInfo.insert( pair<basic_string<char>, _CS_TOTAL_GUILD_DATA>(szGuildName, GuildData));
		}
	}

	m_bCsBasicGuildInfoLoadOK = TRUE;
	m_bCsTotalGuildInfoLoadOK = TRUE;
	SetAllUserCsJoinSide();

	map<basic_string<char>, _CS_TOTAL_GUILD_DATA>::iterator it2 = (map<basic_string<char>, _CS_TOTAL_GUILD_DATA>::iterator)m_mapCsTotalGuildInfo.begin();
	
	int iSize = m_mapCsTotalGuildInfo.size();

	for( ; it2 != m_mapCsTotalGuildInfo.end(); it2++ )
	{
		basic_string<char> GuildName = it2->first;
		_CS_TOTAL_GUILD_DATA GuildData = _CS_TOTAL_GUILD_DATA(it2->second);

		LogAddTD("[CastleSiege] Siege Guild List Load - GNAME:%s, CSGUID:%d, INVOLVED:%d",
			&GuildName[0], GuildData.m_iCsGuildID, GuildData.m_bGuildInvolved);
	}
	//DebugLog("%s END",__FUNCTION__);
}

void CCastleSiege::Run()
{
	//DebugLog("%s START",__FUNCTION__);
	try
	{
		if( this->m_bDoRun == FALSE )
		{
			//DebugLog("%s END",__FUNCTION__);
			return;
		}
		
		if( this->m_bCastleSiegeEnable == FALSE )
		{
			//DebugLog("%s END",__FUNCTION__);
			return;
		}

		switch( this->m_iCastleSiegeState )
		{
			case CASTLESIEGE_STATE_NONE:
				this->ProcState_NONE();
			break;
			case CASTLESIEGE_STATE_IDLE_1:
				this->ProcState_IDLE_1();
			break;
			case CASTLESIEGE_STATE_REGSIEGE:
				this->ProcState_REGSIEGE();
			break;
			case CASTLESIEGE_STATE_IDLE_2:
				this->ProcState_IDLE_2();
			break;
			case CASTLESIEGE_STATE_REGMARK:
				this->ProcState_REGMARK();
			break;
			case CASTLESIEGE_STATE_IDLE_3:
				this->ProcState_IDLE_3();
			break;
			case CASTLESIEGE_STATE_NOTIFY:
				this->ProcState_NOTIFY();
			break;
			case CASTLESIEGE_STATE_READYSIEGE:
				this->ProcState_READYSIEGE();
			break;
			case CASTLESIEGE_STATE_STARTSIEGE:
				this->ProcState_STARTSIEGE();
			break;
			case CASTLESIEGE_STATE_ENDSIEGE:
				this->ProcState_ENDSIEGE();
			break;
			case CASTLESIEGE_STATE_ENDCYCLE:
				this->ProcState_ENDCYCLE();
			break;
		}

		if( (GetTickCount() - this->m_dwNPC_DBSAVE_TICK_COUNT) > 300000 )//5 mins
		{
			if( this->m_iCastleSiegeState != CASTLESIEGE_STATE_STARTSIEGE )
			{
				this->StoreDbNpc();
				this->m_dwNPC_DBSAVE_TICK_COUNT = GetTickCount();
			}
		}

		if( (GetTickCount() - this->m_dwCALC_LEFTSIEGE_TICK_COUNT) > 10000 )//10 Secs
		{
			this->m_dwCALC_LEFTSIEGE_TICK_COUNT = GetTickCount();
			this->CalcCastleLeftSiegeDate();
		}

		static int g_iLastSiegeUpdate;

		if( (GetTickCount() - g_iLastSiegeUpdate) > 3000 )//3 Secs
		{
			g_iLastSiegeUpdate = GetTickCount();

			int iLeftSiegeDate = this->CalcCastleLeftSiegeDate();

			if( iLeftSiegeDate >= 0 )
			{
				SYSTEMTIME tmLeftDate = this->GetCastleLeftSiegeDate();

				//LogAddC(4, "LEFT : SEC(%d), SIEGE-DAY(%04d-%02d-%02d %02d:%02d:%02d)",
				//	iLeftSiegeDate, tmLeftDate.wYear, tmLeftDate.wMonth, tmLeftDate.wDay,
				//	tmLeftDate.wHour, tmLeftDate.wMinute, tmLeftDate.wSecond);
			}
			else
			{
				switch( iLeftSiegeDate )
				{
					case CASTLESIEGE_LEFTTIME_ONSIGE:
						//LogAddC(4, "[CastleSiege] LEFT-TIME : CASTLESIEGE_LEFTTIME_ONSIGE");
					break;
					case CASTLESIEGE_LEFTTIME_ENDSIEGE:
						//LogAddC(4, "[CastleSiege] LEFT-TIME : CASTLESIEGE_LEFTTIME_ENDSIEGE");
					break;
					case CASTLESIEGE_LEFTTIME_NOTRUNNING:
						//LogAddC(4, "[CastleSiege] LEFT-TIME : CASTLESIEGE_LEFTTIME_NOTRUNNING");
					break;
				}
			}
		}
	}catch(...)
	{}
	
	//DebugLog("%s END",__FUNCTION__);
}

int CCastleSiege::GetCurRemainSec()
{
	__try
	{
		//DebugLog("%s START",__FUNCTION__);
		if( this->m_iCS_REMAIN_MSEC < 0 )
			return -1;

		return (this->m_iCS_REMAIN_MSEC / 1000);
	}__finally
	{
		//DebugLog("%s END",__FUNCTION__);
	}
}

BOOL CCastleSiege::GetCastleStateTerm(SYSTEMTIME * tmStateStartDate, SYSTEMTIME * tmStateEndDate)
{
	__try
	{
		//DebugLog("%s START",__FUNCTION__);
		if( m_iCastleSiegeState >= CASTLESIEGE_STATE_IDLE_1 && CASTLESIEGE_STATE_ENDCYCLE < m_iCastleSiegeState )
		{
			return FALSE;
		}

		if( m_bDoRun == FALSE )
		{
			return FALSE;
		}

		*tmStateStartDate = m_tmStateStartDate;


		GetLocalTime(tmStateEndDate);

		GetNextDay(tmStateEndDate, 0, 0, 0, m_iCS_REMAIN_MSEC / 1000);

		return TRUE;
	}__finally
	{
		//DebugLog("%s END",__FUNCTION__);
	}
}

int CCastleSiege::CalcCastleLeftSiegeDate()
{
	//DebugLog("%s START",__FUNCTION__);
	memset(&this->m_tmLeftCastleSiegeDate, 0, sizeof(this->m_tmLeftCastleSiegeDate));

	if( this->m_iCastleSiegeState >= CASTLESIEGE_STATE_IDLE_1 && CASTLESIEGE_STATE_ENDCYCLE < this->m_iCastleSiegeState )
	{
		//DebugLog("%s END",__FUNCTION__);
		return CASTLESIEGE_LEFTTIME_NOTRUNNING;
	}

	if( this->m_bDoRun == FALSE )
	{
		//DebugLog("%s END",__FUNCTION__);
		return CASTLESIEGE_LEFTTIME_NOTRUNNING;
	}

	if( this->m_iCastleSiegeState == CASTLESIEGE_STATE_STARTSIEGE )
	{
		//DebugLog("%s END",__FUNCTION__);
		return CASTLESIEGE_LEFTTIME_ONSIGE;
	}

	if( this->m_btIsSiegeEnded == 1 || this->m_iCastleSiegeState >= CASTLESIEGE_STATE_ENDSIEGE )
	{
		//DebugLog("%s END",__FUNCTION__);
		return CASTLESIEGE_LEFTTIME_ENDSIEGE;
	}

	int iGAP_SEC = CASTLESIEGE_LEFTTIME_ONSIGE;
	BOOL bAddStart = FALSE;

	//EnterCriticalSection(&this->m_critScheduleData);
	
	//for( _CS_SCHEDULE_DATA* it = this->m_vtScheduleData.begin(); it != this->m_vtScheduleData.end(); it++ )
	for(std::vector<_CS_SCHEDULE_DATA>::iterator it = this->m_vtScheduleData.begin(); it != this->m_vtScheduleData.end(); it++ )
	{
		//_CS_SCHEDULE_DATA* lpScheduleData = it;
		std::vector<_CS_SCHEDULE_DATA>::iterator lpScheduleData = it;

		if( lpScheduleData->m_iSTATE == 7 )
		{
			break;
		}

		if( bAddStart )
		{
			iGAP_SEC += lpScheduleData->m_iGAP_SEC;
		}

		if( lpScheduleData->m_iSTATE == this->m_iCastleSiegeState )
		{
			iGAP_SEC = (this->m_iCS_REMAIN_MSEC / 1000);
			bAddStart = TRUE;
		}
	}

	//LeaveCriticalSection(&this->m_critScheduleData);

	if( iGAP_SEC >= 0 )
	{
		SYSTEMTIME tmLocalTime;
		GetLocalTime(&tmLocalTime);

		this->GetNextDay(&tmLocalTime, 0, 0, 0, iGAP_SEC);

		this->m_tmLeftCastleSiegeDate = tmLocalTime;

		if( iGAP_SEC == 0 )
		{
			LogAddC(2, "[CastleSiege] CCastleSiege::CalcCastleLeftSiegeDate() - iGAP_SEC == 0");
		}

		//DebugLog("%s END",__FUNCTION__);
		return iGAP_SEC;
	}

	//DebugLog("%s END",__FUNCTION__);
	return CASTLESIEGE_LEFTTIME_NOTRUNNING;
}

// ProcState
void CCastleSiege::ProcState_NONE()
{
	// Empty
}


void CCastleSiege::ProcState_IDLE_1()
{
	//DebugLog("%s START",__FUNCTION__);
	__try
	{
		int iTICK_MSEC = ( GetTickCount() - this->m_iCS_TICK_COUNT );

		if( iTICK_MSEC >= 1000 )
		{
			this->m_iCS_REMAIN_MSEC -= iTICK_MSEC;
			this->m_iCS_TICK_COUNT = GetTickCount();
			//LogAddC(3, "RUN ProcState_IDLE_1()\tLEFT-MSEC:%d", this->m_iCS_REMAIN_MSEC);
		}

		if( this->m_iCS_REMAIN_MSEC <= 0 )
		{
			this->SetState(CASTLESIEGE_STATE_REGSIEGE, 1);
		}
	}__finally
	{
		//DebugLog("%s END",__FUNCTION__);
	}
}


void CCastleSiege::ProcState_REGSIEGE()
{
	//DebugLog("%s START",__FUNCTION__);
	__try
	{
		int iTICK_MSEC = ( GetTickCount() - this->m_iCS_TICK_COUNT );

		if( iTICK_MSEC >= 1000 )
		{
			this->m_iCS_REMAIN_MSEC -= iTICK_MSEC;
			this->m_iCS_TICK_COUNT = GetTickCount();
		
			if( (GetTickCount() - this->m_dwEVENT_MSG_TICK_COUNT) > 1800000 )
			{
				this->SendAllUserAnyMsg(lMsg.Get(MSGGET(6,78)), 1);
				this->m_dwEVENT_MSG_TICK_COUNT = GetTickCount();
			}
			//LogAddC(3, "RUN ProcState_REGSIEGE()\tLEFT-MSEC:%d", this->m_iCS_REMAIN_MSEC);
		}

		if( this->m_iCS_REMAIN_MSEC <= 0 )
		{
			this->SetState(CASTLESIEGE_STATE_IDLE_2, 1);
		}
	}__finally
	{
		//DebugLog("%s END",__FUNCTION__);
	}
}


void CCastleSiege::ProcState_IDLE_2()
{
	//DebugLog("%s START",__FUNCTION__);
	__try
	{
		int iTICK_MSEC = ( GetTickCount() - this->m_iCS_TICK_COUNT );

		if( iTICK_MSEC >= 1000 )
		{
			this->m_iCS_REMAIN_MSEC -= iTICK_MSEC;
			this->m_iCS_TICK_COUNT = GetTickCount();
			//LogAddC(3, "RUN ProcState_IDLE_2()\tLEFT-MSEC:%d", this->m_iCS_REMAIN_MSEC);
		}

		if( this->m_iCS_REMAIN_MSEC <= 0 )
		{
			this->SetState(CASTLESIEGE_STATE_REGMARK, 1);
		}
	}__finally
	{
		//DebugLog("%s END",__FUNCTION__);
	}
}



void CCastleSiege::ProcState_REGMARK()
{
	//DebugLog("%s START",__FUNCTION__);
	__try
	{
		int iTICK_MSEC = ( GetTickCount() - this->m_iCS_TICK_COUNT );

		if( iTICK_MSEC >= 1000 )
		{
			this->m_iCS_REMAIN_MSEC -= iTICK_MSEC;
			this->m_iCS_TICK_COUNT = GetTickCount();
		
			if( (GetTickCount() - this->m_dwEVENT_MSG_TICK_COUNT) > 1800000 )
			{
				this->SendAllUserAnyMsg(lMsg.Get(MSGGET(6,79)), 1);
				this->m_dwEVENT_MSG_TICK_COUNT = GetTickCount();
			}
			//LogAddC(3, "RUN ProcState_REGMARK()\tLEFT-MSEC:%d", this->m_iCS_REMAIN_MSEC);
		}

		if( this->m_iCS_REMAIN_MSEC <= 0 )
		{
			this->m_btIsSiegeGuildList = FALSE;
			this->SetState(CASTLESIEGE_STATE_IDLE_3, 1);
		}
	}__finally
	{
		//DebugLog("%s END",__FUNCTION__);
	}
}

void CCastleSiege::ProcState_IDLE_3()
{
	//DebugLog("%s START",__FUNCTION__);
	__try
	{
		int iTICK_MSEC = ( GetTickCount() - this->m_iCS_TICK_COUNT );

		if( iTICK_MSEC >= 1000 )
		{
			this->m_iCS_REMAIN_MSEC -= iTICK_MSEC;
			this->m_iCS_TICK_COUNT = GetTickCount();
		
			if( (GetTickCount() - this->m_dwEVENT_MSG_TICK_COUNT) > 7200000 )
			{
				this->SendAllUserAnyMsg(lMsg.Get(MSGGET(6,80)), 1);
				this->m_dwEVENT_MSG_TICK_COUNT = GetTickCount();
			}

			//LogAddC(3, "RUN ProcState_IDLE_3()\tLEFT-MSEC:%d", this->m_iCS_REMAIN_MSEC);
		}

		if( this->m_iCS_REMAIN_MSEC <= 0 )
		{
			this->SetState(CASTLESIEGE_STATE_NOTIFY, 1);
		}
	}__finally
	{
		//DebugLog("%s END",__FUNCTION__);
	}
}

void CCastleSiege::ProcState_NOTIFY()
{
	//DebugLog("%s START",__FUNCTION__);
	__try
	{
		int iTICK_MSEC = ( GetTickCount() - this->m_iCS_TICK_COUNT );

		if( iTICK_MSEC >= 1000 )
		{
			this->m_iCS_REMAIN_MSEC -= iTICK_MSEC;
			this->m_iCS_TICK_COUNT = GetTickCount();
		
			if( (GetTickCount() - this->m_dwEVENT_MSG_TICK_COUNT) > 1800000 )
			{
				this->SendAllUserAnyMsg(lMsg.Get(MSGGET(6,81)), 1);
				this->m_dwEVENT_MSG_TICK_COUNT = GetTickCount();
			}
			//LogAddC(3, "RUN ProcState_NOTIFY()\tLEFT-MSEC:%d", this->m_iCS_REMAIN_MSEC);
		}

		if( this->m_iCS_REMAIN_MSEC <= 0 )
		{
			LogAddTD("Changing state to CASTLESIEGE_STATE_READYSIEGE");
			this->SetState(CASTLESIEGE_STATE_READYSIEGE, 1);
		}
	}__finally
	{
		//DebugLog("%s END",__FUNCTION__);
	}
}



void CCastleSiege::ProcState_READYSIEGE()
{
	//DebugLog("%s START",__FUNCTION__);
	__try
	{
		int iTICK_MSEC = ( GetTickCount() - this->m_iCS_TICK_COUNT );

		if( iTICK_MSEC >= 1000 )
		{
			this->m_iCS_REMAIN_MSEC -= iTICK_MSEC;
			this->m_iCS_TICK_COUNT = GetTickCount();
		
			if( (GetTickCount() - this->m_dwEVENT_MSG_TICK_COUNT) > 1800000 )
			{
				this->SendAllUserAnyMsg(lMsg.Get(MSGGET(6,82)), 1);
				this->m_dwEVENT_MSG_TICK_COUNT = GetTickCount();
			}
			if( this->m_iCS_REMAIN_MSEC <= 300000 )
			{
				if( ( GetTickCount() - this->m_dwCS_STARTTIME_TICK_COUNT ) > 60000 )
				{
					char szBuff[256]={0};
					wsprintf(szBuff, lMsg.Get(MSGGET(6,188)), (this->m_iCS_REMAIN_MSEC/60000)+1);
					//this->SendAllUserAnyMsg(szBuff, 2);
					SendMapServerGroupMsg(szBuff);
					this->m_dwCS_STARTTIME_TICK_COUNT = GetTickCount();
				}
			}
			//LogAddC(3, "RUN ProcState_READYSIEGE()\tLEFT-MSEC:%d", this->m_iCS_REMAIN_MSEC);
		}

		if( this->m_iCS_REMAIN_MSEC <= 0 )
		{
			this->SetState(CASTLESIEGE_STATE_STARTSIEGE, 1);
		}
	}__finally
	{
		//DebugLog("%s END",__FUNCTION__);
	}
}

void CCastleSiege::ProcState_STARTSIEGE()
{
	//DebugLog("%s START",__FUNCTION__);
	__try
	{
		int iTICK_MSEC = ( GetTickCount() - this->m_iCS_TICK_COUNT );

		if( iTICK_MSEC >= 1000 )
		{
			this->m_iCS_REMAIN_MSEC -= iTICK_MSEC;
			this->m_iCS_TICK_COUNT = GetTickCount();
			this->CheckMiddleWinnerGuild();
		
			if( (GetTickCount() - this->m_dwCHECK_GATE_ALIVE_COUNT) > 5000 )
			{
				this->CheckCsDbNpcAlive();
				this->m_dwCHECK_GATE_ALIVE_COUNT = GetTickCount();
			}

			if( (GetTickCount() - this->m_dwCS_LEFTTIME_TICK_COUNT) > 180000 )
			{
				if( this->m_iCS_REMAIN_MSEC > 180000 )
				{
					GCAnsCsLeftTimeAlarm( (this->m_iCS_REMAIN_MSEC/1000)/3600, ((this->m_iCS_REMAIN_MSEC/1000)%3600)/60);
					this->m_dwCS_LEFTTIME_TICK_COUNT = GetTickCount();
				}
			}

			if( this->m_iCS_REMAIN_MSEC <= 180000 )
			{
				if( (GetTickCount() - this->m_dwCS_LEFTTIME_TICK_COUNT) > 60000 )
				{
					GCAnsCsLeftTimeAlarm( ((this->m_iCS_REMAIN_MSEC/1000)/3600), ((this->m_iCS_REMAIN_MSEC/1000)%3600)/60);
					this->m_dwCS_LEFTTIME_TICK_COUNT = GetTickCount();
				}
			}

			if( this->m_dwCS_JOINSIDE_REFRESH_TOTAL_COUNT < 3 )
			{
				if( (GetTickCount() - this->m_dwCS_JOINSIDE_REFRESH_TICK_COUNT) > 10000 )
				{
					this->SetAllUserCsJoinSide();
					this->m_dwCS_JOINSIDE_REFRESH_TICK_COUNT = GetTickCount();
					this->m_dwCS_JOINSIDE_REFRESH_TOTAL_COUNT++;
				}
			}

			if( (GetTickCount() - this->m_dwCS_MINIMAP_SEND_TICK_COUNT) > 3000 )
			{
				this->OperateMiniMapWork();
				this->m_dwCS_MINIMAP_SEND_TICK_COUNT = GetTickCount();
			}

			if( (GetTickCount() - this->m_dwEVENT_MSG_TICK_COUNT) > 1800000 )
			{
				this->SendAllUserAnyMsg(lMsg.Get(MSGGET(6,83)), 1);
				this->m_dwEVENT_MSG_TICK_COUNT = GetTickCount();
			}

			//LogAddC(3, "RUN ProcState_STARTSIEGE()\tLEFT-MSEC:%d", this->m_iCS_REMAIN_MSEC);
		}
	
		if( this->m_iCS_REMAIN_MSEC <= 0 )
		{
			if( this->CheckCastleSiegeResult() == TRUE )
			{
				GS_GDReqResetCastleTaxInfo(this->m_iMapSvrGroup);
			}
			this->ReSpawnEnemyUser(FALSE);

			if( this->m_btIsCastleOccupied == 1 )
			{
				GS_GDReqCastleOwnerChange(this->m_iMapSvrGroup, this->m_btIsCastleOccupied, this->m_szCastleOwnerGuild);
			}
		
			GS_GDReqSiegeEndedChange(this->m_iMapSvrGroup, TRUE);
		
			this->StoreDbNpc();
			this->NotifyAllUserCsStartState(0);
		
			GS_GDReqResetSiegeGuildInfo(this->m_iMapSvrGroup);
		
			this->SetState(CASTLESIEGE_STATE_ENDSIEGE, 1);
		}
	}__finally
	{
		//DebugLog("%s END",__FUNCTION__);
	}
}



void CCastleSiege::ProcState_ENDSIEGE()
{
	//DebugLog("%s START",__FUNCTION__);
	__try
	{
		int iTICK_MSEC = (GetTickCount() - this->m_iCS_TICK_COUNT);

		if( iTICK_MSEC >= 1000 )
		{
			this->m_iCS_REMAIN_MSEC -= iTICK_MSEC;
			this->m_iCS_TICK_COUNT = GetTickCount();

			//LogAddC(3, "RUN ProcState_ENDSIEGE()\tLEFT-MSEC:%d", this->m_iCS_REMAIN_MSEC);
		}

		if( this->m_iCS_REMAIN_MSEC <= 0 )
		{
			this->SetState(CASTLESIEGE_STATE_ENDCYCLE, 1);
		}
	}__finally
	{
		//DebugLog("%s END",__FUNCTION__);
	}
}

void CCastleSiege::ProcState_ENDCYCLE()
{
	//DebugLog("%s START",__FUNCTION__);
	__try
	{
		int iTICK_MSEC = (GetTickCount() - this->m_iCS_TICK_COUNT);

		if( iTICK_MSEC >= 1000 )
		{
			this->m_iCS_REMAIN_MSEC -= iTICK_MSEC;
			this->m_iCS_TICK_COUNT = GetTickCount();

			//LogAddC(3, "RUN ProcState_ENDCYCLE()\tLEFT-MSEC:%d", this->m_iCS_REMAIN_MSEC);
		}

		if( this->m_iCS_REMAIN_MSEC <= 0 )
		{
			this->SetState(CASTLESIEGE_STATE_IDLE_1, 1);
		}
	}__finally
	{
		//DebugLog("%s END",__FUNCTION__);
	}
}

struct PMSG_ANS_SENDMINIMAPDATA // 0x8
{
    PWMSG_HEAD h; // +0x0(0x4)
    int iCount; // +0x4(0x4)
};

struct PMSG_ANS_SENDNPCMINIMAPDATA // 0x8
{
    PWMSG_HEAD h; // +0x0(0x4)
    int iCount; // +0x4(0x4)
};

struct PMSG_SENDNPCMINIMAPDATA // 0x3
{
    BYTE btNpcType; // +0x0(0x1)
    BYTE btX; // +0x1(0x1)
    BYTE btY; // +0x2(0x1)
};

void CCastleSiege::OperateMiniMapWork()	/*00545580*/
{
	//DebugLog("%s START",__FUNCTION__);
	if( this->m_bCsBasicGuildInfoLoadOK == FALSE )
	{
		//DebugLog("%s END",__FUNCTION__);
		return;
	}

	std::map<int, _CS_MINIMAP_DATA> mapMiniMapData;

	BYTE cBUFFER1[2264] = {0};
	PMSG_ANS_SENDMINIMAPDATA* lpMsgSend1 = (PMSG_ANS_SENDMINIMAPDATA*)cBUFFER1;
	PMSG_SENDMINIMAPDATA* lpMsgSendBody1 = (PMSG_SENDMINIMAPDATA*)&cBUFFER1[sizeof(PMSG_ANS_SENDMINIMAPDATA)];

	BYTE cBUFFER2[714] = {0};
	PMSG_ANS_SENDNPCMINIMAPDATA* lpMsgSend2 = (PMSG_ANS_SENDNPCMINIMAPDATA*)cBUFFER2;
	PMSG_SENDNPCMINIMAPDATA* lpMsgSendBody2 = (PMSG_SENDNPCMINIMAPDATA*)&cBUFFER2[sizeof(PMSG_ANS_SENDNPCMINIMAPDATA)];

	////EnterCriticalSection(&m_critCsBasicGuildInfo);

	for( map<basic_string<char>, _CS_TOTAL_GUILD_DATA>::iterator it = (map<basic_string<char>, _CS_TOTAL_GUILD_DATA>::iterator)m_mapCsBasicGuildInfo.begin(); it != m_mapCsBasicGuildInfo.end(); it++ )
	{
		_CS_TOTAL_GUILD_DATA & GuildData = it->second; //ebp-BDC
		_CS_MINIMAP_DATA MiniMapData; //ebp-13C8
		
		basic_string<char> szGuildName = it->first;
		memcpy(MiniMapData.m_szGuildName, &szGuildName[0], MAX_GUILD_LEN);

		mapMiniMapData.insert( std::pair<int, _CS_MINIMAP_DATA>(GuildData.m_iCsGuildID, MiniMapData) );
	}

	////LeaveCriticalSection(&m_critCsBasicGuildInfo);

	if( mapMiniMapData.empty() )
	{
		mapMiniMapData.clear();

		//DebugLog("%s END",__FUNCTION__);
		return;
	}

	//EnterCriticalSection(&m_critNpcData);

	lpMsgSend2->iCount = 0;

	if( m_vtNpcData.empty() == false )
	{
		//for( _CS_NPC_DATA *it = this->m_vtNpcData.begin(); it != m_vtNpcData.end(); it++ )
		for(std::vector<_CS_NPC_DATA>::iterator it = this->m_vtNpcData.begin(); it != this->m_vtNpcData.end(); it++ )
		{
			if( lpMsgSend2->iCount >= 150 )
				break;

			std::vector<_CS_NPC_DATA>::iterator NpcData = it;
			//_CS_NPC_DATA & NpcData = _CS_NPC_DATA(*it);

			if( (NpcData->m_iNPC_NUM == 277 || NpcData->m_iNPC_NUM == 283) && NpcData->m_iNPC_OBJINDEX != OBJ_EMPTY )
			{
				if( !gObjIsConnected(NpcData->m_iNPC_OBJINDEX) || (gObj[NpcData->m_iNPC_OBJINDEX].Class != 277 && gObj[NpcData->m_iNPC_OBJINDEX].Class != 283) )
				{
					BYTE btNpcType = 0;
					switch( NpcData->m_iNPC_NUM )
					{
						case 277:
							btNpcType = 0;
						break;
						case 283:
							btNpcType = 1;
						break;
						default:
							continue;
						break;
					}

					lpMsgSendBody2[lpMsgSend2->iCount].btNpcType = btNpcType;
					lpMsgSendBody2[lpMsgSend2->iCount].btX = gObj[NpcData->m_iNPC_OBJINDEX].X;
					lpMsgSendBody2[lpMsgSend2->iCount].btY = gObj[NpcData->m_iNPC_OBJINDEX].Y;

					lpMsgSend2->iCount++;
				}
			}
		}
	}

	lpMsgSend2->h.set(cBUFFER2, 0xBB, (lpMsgSend2->iCount * sizeof(PMSG_SENDNPCMINIMAPDATA)) + sizeof(PMSG_ANS_SENDNPCMINIMAPDATA));

	//LeaveCriticalSection(&m_critNpcData);

	//EnterCriticalSection(&m_critCsMiniMap);

	m_mapMiniMapData.clear();

	for( map<int, _CS_MINIMAP_DATA>::iterator it2 = (map<int, _CS_MINIMAP_DATA>::iterator)mapMiniMapData.begin(); it2 != mapMiniMapData.end(); it2++ )
	{
		m_mapMiniMapData.insert( std::pair<int, _CS_MINIMAP_DATA>(it2->first, it2->second) );
	}

	for( int iIDX = OBJ_STARTUSERINDEX; iIDX < OBJMAX; iIDX++ )
	{
		if( gObjIsConnected(iIDX) == FALSE )
			continue;

		if( gObj[iIDX].MapNumber != MAP_INDEX_CASTLESIEGE )
			continue;

		if( gObj[iIDX].m_btCsJoinSide == 0 )
			continue;

		map<int, _CS_MINIMAP_DATA>::iterator it3 = (map<int,_CS_MINIMAP_DATA>::iterator)m_mapMiniMapData.find(gObj[iIDX].m_btCsJoinSide);

		if( it3 != m_mapMiniMapData.end() )
		{
			if( it3->second.m_iMiniMapPointCount >= 1000 )
				continue;

			if( gObj[iIDX].GuildStatus == GUILD_MASTER )
			{
				if( strcmp(gObj[iIDX].GuildName, "") != 0 && strcmp(gObj[iIDX].GuildName, it3->second.m_szGuildName) == 0)
				{
					it3->second.m_iGuildMasterIndex = iIDX;
				}
			}
			else
			{
				it3->second.m_stMiniMapPoint[it3->second.m_iMiniMapPointCount].btX = gObj[iIDX].X;
				it3->second.m_stMiniMapPoint[it3->second.m_iMiniMapPointCount].btY = gObj[iIDX].Y;
				it3->second.m_iMiniMapPointCount++;
			}
		}
	}

	for( map<int, _CS_MINIMAP_DATA>::iterator it4 = (map<int, _CS_MINIMAP_DATA>::iterator)m_mapMiniMapData.begin(); it4 != m_mapMiniMapData.end(); it4++ )
	{
		lpMsgSend1->iCount = 0;

		if( gObjIsConnected(it4->second.m_iGuildMasterIndex) == FALSE )
			continue;

		lpMsgSend1->iCount = it4->second.m_iMiniMapPointCount;

		if( lpMsgSend1->iCount > 1000 )
		{
			lpMsgSend1->iCount = 1000;
		}

		memcpy(lpMsgSendBody1, it4->second.m_stMiniMapPoint, (lpMsgSend1->iCount * sizeof(PMSG_SENDMINIMAPDATA)));

		lpMsgSend1->h.set(cBUFFER1, 0xB6, (lpMsgSend1->iCount * sizeof(PMSG_SENDMINIMAPDATA)) + sizeof(PMSG_ANS_SENDMINIMAPDATA));

		DataSend(it4->second.m_iGuildMasterIndex, cBUFFER1, (lpMsgSend1->iCount * sizeof(PMSG_SENDMINIMAPDATA)) + sizeof(PMSG_ANS_SENDMINIMAPDATA));

		DataSend(it4->second.m_iGuildMasterIndex, cBUFFER2, (lpMsgSend2->iCount * sizeof(PMSG_SENDNPCMINIMAPDATA)) + sizeof(PMSG_ANS_SENDNPCMINIMAPDATA));

	}

	//LeaveCriticalSection(&m_critCsMiniMap);
	//DebugLog("%s END",__FUNCTION__);
}

void CCastleSiege::CheckCsDbNpcAlive()
{
	//DebugLog("%s START",__FUNCTION__);
	if( m_iCastleSiegeState != CASTLESIEGE_STATE_STARTSIEGE )
	{
		//DebugLog("%s END",__FUNCTION__);
		return;
	}

	//EnterCriticalSection(&m_critNpcData);

	if( m_vtNpcData.empty() == false )
	{
		//for( _CS_NPC_DATA * it = this->m_vtNpcData.begin(); it != m_vtNpcData.end(); it++ )
		for(std::vector<_CS_NPC_DATA>::iterator it = this->m_vtNpcData.begin(); it != this->m_vtNpcData.end(); it++ )
		{
			std::vector<_CS_NPC_DATA>::iterator NpcData = it;
			//_CS_NPC_DATA & NpcData = _CS_NPC_DATA(*it);
			LogAddTD("[CastleSiege][CheckCsDbNpcAlive] NPC Record Index:%d Num:%d", NpcData->m_iNPC_INDEX, NpcData->m_iNPC_NUM);

			if( (NpcData->m_iNPC_NUM == 277 || NpcData->m_iNPC_NUM == 283) && NpcData->m_iNPC_OBJINDEX != OBJ_EMPTY )
			{
				if( !gObjIsConnected(NpcData->m_iNPC_OBJINDEX) || (( gObj[NpcData->m_iNPC_OBJINDEX].Class == 277 || gObj[NpcData->m_iNPC_OBJINDEX].Class == 283 ) && gObj[NpcData->m_iNPC_OBJINDEX].Live == 0) )
				{
					switch( NpcData->m_iNPC_NUM )
					{
					case 277:
						SetGateBlockState(NpcData->m_iNPC_SX, NpcData->m_iNPC_SY, 1);
						LogAddTD("[CastleSiege] Castle Gate is Broken (CLS:%d, IDX:%d, X:%d, Y:%d)",
							NpcData->m_iNPC_NUM, NpcData->m_iNPC_INDEX, NpcData->m_iNPC_SX, NpcData->m_iNPC_SY);
						break;
					case 283:
						if( CheckGuardianStatueExist() == FALSE )
						{
							ClearCastleTowerBarrier();
							SetCastleTowerAccessable(TRUE);
						}
						LogAddTD("[CastleSiege] Defend Statue is Broken (CLS:%d, IDX:%d, X:%d, Y:%d)",
							NpcData->m_iNPC_NUM, NpcData->m_iNPC_INDEX, NpcData->m_iNPC_SX, NpcData->m_iNPC_SY);
						break;
					}

					it->m_iNPC_OBJINDEX = OBJ_EMPTY;
				}
			}
		}
	}

	//LeaveCriticalSection(&m_critNpcData);
	//DebugLog("%s END",__FUNCTION__);
}

BOOL CCastleSiege::CheckMiddleWinnerGuild()
{
	__try
	{
	//DebugLog("%s START",__FUNCTION__);
	if( this->m_dwCrownAccessTime == 0 )
	{
		return FALSE;
	}

	if( (gObjIsConnected(this->m_iCastleCrownAccessUser) == FALSE) || 
		(gObjIsConnected(this->m_iCastleSwitchAccessUser1) == FALSE) ||
		(gObjIsConnected(this->m_iCastleSwitchAccessUser2) == FALSE) )
	{
		return FALSE;
	}

	if( (strcmp(gObj[this->m_iCastleCrownAccessUser].GuildName, "") == 0) ||
		(strcmp(gObj[this->m_iCastleSwitchAccessUser1].GuildName, "") == 0) ||
		(strcmp(gObj[this->m_iCastleSwitchAccessUser2].GuildName, "") == 0) )
	{
		return FALSE;
	}

	if( (gObj[this->m_iCastleCrownAccessUser].m_btCsJoinSide >= 1 ) &&
		(gObj[this->m_iCastleSwitchAccessUser1].m_btCsJoinSide >= 1 ) &&
		(gObj[this->m_iCastleSwitchAccessUser2].m_btCsJoinSide >= 1 ) )
	{
		//if( gObj[this->m_iCastleCrownAccessUser].m_bCsGuildInvolved == true )
		//{
			if( (gObj[this->m_iCastleCrownAccessUser].m_btCsJoinSide == gObj[this->m_iCastleSwitchAccessUser1].m_btCsJoinSide) && 
				(gObj[this->m_iCastleCrownAccessUser].m_btCsJoinSide == gObj[this->m_iCastleSwitchAccessUser2].m_btCsJoinSide) )
			{
				if( ( GetTickCount() - this->m_dwCrownAccessTime ) >= CASTLESIEGE_CROWN_REGISTER_TIME )
				{
					LogAddTD("[CastleSiege] Castle Winner Has been Changed (GUILD:%s)",
						gObj[this->m_iCastleCrownAccessUser].GuildName);

					gObj[this->m_iCastleCrownAccessUser].m_iAccumulatedCrownAccessTime = 0;
					GCAnsCsAccessCrownState(this->m_iCastleCrownAccessUser, 1);
					this->NotifyCrownState(2);

					memset(&this->m_szMiddleWinnerGuild, 0, sizeof(this->m_szMiddleWinnerGuild));
					memcpy(&this->m_szMiddleWinnerGuild, gObj[this->m_iCastleCrownAccessUser].GuildName, MAX_GUILD_LEN);

					this->ClearNonDbNPC_MidWin();
					this->ChangeWinnerGuild(gObj[this->m_iCastleCrownAccessUser].m_btCsJoinSide);
					this->NotifyAllUserCsProgState(1, gObj[this->m_iCastleCrownAccessUser].GuildName);
					this->ResetCrownAccessTickCount();
					this->ResetCrownUserIndex();
					this->SetCrownAccessUserX(0);
					this->SetCrownAccessUserY(0);
					this->ResetCrownSwitchUserIndex(217);
					this->ResetCrownSwitchUserIndex(218);
					this->m_bRegCrownAvailable = FALSE;
					this->ReSpawnEnemyUser(FALSE);
	
					GCAnsCsLeftTimeAlarm( (this->m_iCS_REMAIN_MSEC/1000)/3600, ((this->m_iCS_REMAIN_MSEC/1000)%3600)/60);

					this->m_dwCS_LEFTTIME_TICK_COUNT = GetTickCount();

					return TRUE;
				}
			}
		//}
	}
	return FALSE;
	}__finally
	{
		//DebugLog("%s END",__FUNCTION__);
	}
}

void CCastleSiege::NotifyAllUserCsProgState(BYTE btProgState, char* lpszGuildName)
{
	__try
	{
	//DebugLog("%s START",__FUNCTION__);
	PMSG_ANS_NOTIFYCSPROGRESS pMsg;
	
	pMsg.h.set((LPBYTE)&pMsg, 0xB2, 0x18, sizeof(pMsg));

	pMsg.btCastleSiegeState = btProgState;

	memset(&pMsg.szGuildName, 0, MAX_GUILD_LEN);
	memcpy(&pMsg.szGuildName, lpszGuildName, MAX_GUILD_LEN);

	for( int i = OBJ_STARTUSERINDEX; i < OBJMAX; i++ )
	{
		if( (gObj[i].Connected == PLAYER_PLAYING) && (gObj[i].Type == OBJ_USER) )
		{
			if( gObj[i].MapNumber == MAP_INDEX_CASTLESIEGE )
			{
				DataSend(i, (LPBYTE)&pMsg, pMsg.h.size);
			}
		}
	}
	}__finally
	{
		//DebugLog("%s END",__FUNCTION__);
	}
}

void CCastleSiege::ChangeWinnerGuild(int iCsJoinSide)
{
	//DebugLog("%s START",__FUNCTION__);
	char szGuildTo[MAX_GUILD_LEN+1] = {0};
	char szGuildFrom[MAX_GUILD_LEN+1] = {0};

	//EnterCriticalSection(&this->m_critCsTotalGuildInfo);

	map<basic_string<char,char_traits<char> >,_CS_TOTAL_GUILD_DATA,less<basic_string<char,char_traits<char> > > >::iterator it = (map<basic_string<char,char_traits<char> >,_CS_TOTAL_GUILD_DATA,less<basic_string<char,char_traits<char> > > >::iterator)m_mapCsTotalGuildInfo.begin();

	for(; it != m_mapCsTotalGuildInfo.end(); it++)
	{
		if(it->second.m_iCsGuildID == iCsJoinSide)
		{
			it->second.m_iCsGuildID = 1;
		
			if(it->second.m_bGuildInvolved == TRUE)
			{
				basic_string<char> szGuildName = it->first;
				memcpy(&szGuildTo,&szGuildName[0], MAX_GUILD_LEN);
			}
		}
		else if(it->second.m_iCsGuildID == 1)
		{
			it->second.m_iCsGuildID = iCsJoinSide;
		
			if(it->second.m_bGuildInvolved == TRUE)
			{
				basic_string<char> szGuildName = it->first;
				memcpy(&szGuildFrom,&szGuildName[0], MAX_GUILD_LEN);
			}
		}
	}

	for( int iUSER = OBJ_STARTUSERINDEX; iUSER < OBJMAX; iUSER++ )
	{
		if(!gObjIsConnected(iUSER))
		{
			continue;
		}
			
		gObj[iUSER].m_ViewSkillState &= 0xFFF3FFFF;
		
		char szNewGuild[MAX_GUILD_LEN+1] = {0};

		memcpy(&szNewGuild,gObj[iUSER].GuildName,MAX_GUILD_LEN);

		map<basic_string<char,char_traits<char> >,_CS_TOTAL_GUILD_DATA,less<basic_string<char,char_traits<char> > > >::iterator it = (map<basic_string<char,char_traits<char> >,_CS_TOTAL_GUILD_DATA,less<basic_string<char,char_traits<char> > > >::iterator)m_mapCsTotalGuildInfo.find(szNewGuild);

		if( it != this->m_mapCsTotalGuildInfo.end() )
		{
			gObj[iUSER].m_btCsJoinSide = it->second.m_iCsGuildID;

			//if (gObj[iUSER].m_btCsJoinSide > 0)
			//if (gObj[iUSER].m_btCsJoinSide&2 == iCsJoinSide&2)
			//	gObj[iUSER].m_bCsGuildInvolved = true;
			//else
			//	gObj[iUSER].m_bCsGuildInvolved = false;

			//gObj[iUSER].m_bCsGuildInvolved = (it->second.m_bGuildInvolved != false);

			LogAddTD("[CastleSiege][ChangeWinner] User: [%s][%s] Guild: %s / Side: %d", 
				gObj[iUSER].AccountID, gObj[iUSER].Name,
				gObj[iUSER].GuildName, gObj[iUSER].m_btCsJoinSide
			);
		}
	}

	//LeaveCriticalSection(&this->m_critCsTotalGuildInfo);

	LogAddTD("[CastleSiege] Castle Winner Guild Change From '%s' To '%s'", szGuildFrom, szGuildTo);

	//DebugLog("%s END",__FUNCTION__);
}

void CCastleSiege::ClearNonDbNPC_MidWin()
{
	__try
	{
	//DebugLog("%s START",__FUNCTION__);
	for( int n = 0 ; n < OBJ_MAXMONSTER; n++ )
	{
		if( gObjIsConnected(n) )
		{
			if( gObj[n].MapNumber == MAP_INDEX_CASTLESIEGE )
			{
				if( gObj[n].m_btCsNpcType && gObj[n].m_btCsNpcType != 1 )
				{
					if( gObj[n].Class == 278 )
					{
						g_CsNPC_LifeStone.DeleteLifeStone(n);
						gObjDel(n);
					}

					if(gObj[n].Class == 285 && gObj[n].m_Attribute == 60)
					{
						g_CsNPC_Guardian.DeleteGuardian(n);
						gObjDel(n);
					}
				}
			}
		}
	}
	}__finally
	{
	//DebugLog("%s END",__FUNCTION__);
	}
}

BOOL CCastleSiege::CheckUnionMember(int iIndex)
{
	__try
	{
		//DebugLog("%s START",__FUNCTION__);
		_GUILD_INFO_STRUCT* lpGuildInfo;
		TUnionInfo * pUnionInfo;

		if( ::gObjIsConnected(iIndex) == FALSE )
		{
			return FALSE;
		}

		if( strcmp(gObj[iIndex].GuildName, "") == 0 )
		{
			return FALSE;
		}

		lpGuildInfo = gObj[iIndex].lpGuild;

		if( lpGuildInfo == NULL )
		{
			return FALSE;
		}

		pUnionInfo = UnionManager.SearchUnion(lpGuildInfo->iGuildUnion);

		if( pUnionInfo == NULL )
		{
			return FALSE;
		}

		if( strcmp(pUnionInfo->m_szMasterGuild, gObj[iIndex].GuildName) != 0 )
		{
			return TRUE;
		}

		return FALSE;
	}__finally
	{
		//DebugLog("%s END",__FUNCTION__);
	}
}

BOOL CCastleSiege::CheckCastleOwnerUnionMember(int iIndex)
{
	__try
	{
		//DebugLog("%s START",__FUNCTION__);
		_GUILD_INFO_STRUCT* lpGuildInfo;
		TUnionInfo * pUnionInfo;

		if( ::gObjIsConnected(iIndex) == FALSE )
		{
			return FALSE;
		}

		if( strcmp(this->m_szCastleOwnerGuild, "") == 0 )
		{
			return FALSE;
		}

		if( strcmp(gObj[iIndex].GuildName, "") == 0 )
		{
			return FALSE;
		}

		lpGuildInfo = gObj[iIndex].lpGuild;

		if( lpGuildInfo == NULL )
		{
			return FALSE;
		}

		pUnionInfo = UnionManager.SearchUnion(lpGuildInfo->iGuildUnion);

		if( pUnionInfo == NULL )
		{
			return FALSE;
		}

		if( strcmp(pUnionInfo->m_szMasterGuild, this->m_szCastleOwnerGuild) == 0 )
		{
			return TRUE;
		}

		return FALSE;
	}__finally
	{
		//DebugLog("%s END",__FUNCTION__);
	}
}

BOOL CCastleSiege::CheckCastleOwnerMember(int iIndex)
{
	__try
	{
	//DebugLog("%s START",__FUNCTION__);
	if( gObjIsConnected(iIndex) == FALSE )
	{
		return FALSE;
	}

	if( strcmp(this->m_szCastleOwnerGuild, "") == 0 )
	{
		return FALSE;
	}

	if( strcmp(gObj[iIndex].GuildName, "") == 0 )
	{
		return FALSE;
	}

	if( strcmp(gObj[iIndex].GuildName, this->m_szCastleOwnerGuild) != 0 )
	{
		return FALSE;
	}

	return TRUE;
	}__finally
	{
	//DebugLog("%s END",__FUNCTION__);
	}
}

BOOL CCastleSiege::CheckUnionGuildMasterOrAssistant(int iIndex)
{
	//DebugLog("%s START",__FUNCTION__);
	__try
	{
		_GUILD_INFO_STRUCT* lpGuildInfo;
	
		if( gObjIsConnected(iIndex) == FALSE )
		{
			return FALSE;
		}

		lpGuildInfo = gObj[iIndex].lpGuild;

		if( lpGuildInfo == NULL )
		{
			return FALSE;
		}

		if(( gObj[iIndex].GuildStatus == GUILD_MASTER ) || (gObj[iIndex].GuildStatus ==  GUILD_ASSISTANT))
		{
			if( lpGuildInfo->iGuildUnion == 0 )
			{
				return FALSE;
			}

			if( lpGuildInfo->Number == lpGuildInfo->iGuildUnion )
			{
				return TRUE;
			}
		}

		return FALSE;
	}__finally
	{
		//DebugLog("%s END",__FUNCTION__);
	}
}

int CCastleSiege::CheckAddDbNPC(int iIndex, int iNpcType, int iNpcIndex, BYTE & btResult)
{
	//DebugLog("%s START",__FUNCTION__);
	int iRETVAL = 0;

	//EnterCriticalSection(&m_critNpcData);

	//_CS_NPC_DATA * it = this->m_vtNpcData.begin();
	std::vector<_CS_NPC_DATA>::iterator it = this->m_vtNpcData.begin();

	while( it != this->m_vtNpcData.end() )
	{
		std::vector<_CS_NPC_DATA>::iterator NpcData = it;
		//_CS_NPC_DATA & NpcData = _CS_NPC_DATA(*it);
		LogAddTD("[CastleSiege][CheckAddDbNPC] NPC Record Index:%d Num:%d", NpcData->m_iNPC_INDEX, NpcData->m_iNPC_NUM);

		if( NpcData->m_iNPC_NUM == iNpcType && NpcData->m_iNPC_INDEX == iNpcIndex )
		{
			if( NpcData->m_bIN_USE == TRUE && NpcData->m_iNPC_LIVE > 0 && gObjIsConnected(NpcData->m_iNPC_OBJINDEX) && gObj[NpcData->m_iNPC_OBJINDEX].Class == iNpcType )
			{
				if( gObj[NpcData->m_iNPC_OBJINDEX].Live == 1 )
				{
					btResult = 0;
					iRETVAL = 0;

					break;
				}
			}
			
			int iNpcBuyCost = 0;

			switch( NpcData->m_iNPC_NUM )
			{
				case 277:
					iNpcBuyCost = NpcData->m_iNPC_BASE_MAXHP * 5;
				break;
				case 283:
					iNpcBuyCost = NpcData->m_iNPC_BASE_MAXHP * 3;
				break;
			}

			if( gObj[iIndex].Money < iNpcBuyCost )
			{
				btResult = 3;
				iRETVAL = 0;

				break;
			}
			
			GS_GDReqCastleNpcBuy(m_iMapSvrGroup, iIndex, iNpcType, iNpcIndex, NpcData->m_iNPC_DF_LEVEL,
				NpcData->m_iNPC_RG_LEVEL, NpcData->m_iNPC_MAXHP, NpcData->m_iNPC_MAXHP, NpcData->m_iNPC_SX,
				NpcData->m_iNPC_SY, NpcData->m_iNPC_DIR, iNpcBuyCost);

			btResult = 1;
			iRETVAL = 1;

			break;
		}

		it++;
	}

	//LeaveCriticalSection(&m_critNpcData);
	
	//DebugLog("%s END",__FUNCTION__);
	return iRETVAL;
}

BOOL CCastleSiege::CheckGuildOwnCastle(char* lpszGuildName)
{
	__try
	{
		//DebugLog("%s START",__FUNCTION__);
		if( lpszGuildName == NULL )
		{
			return FALSE;
		}

		if( strcmp(this->m_szCastleOwnerGuild, "") == 0 )
		{
			return FALSE;
		}

		if( strcmp(this->m_szCastleOwnerGuild, lpszGuildName) == 0 )
		{
			return TRUE;
		}

		return FALSE;
	}__finally
	{
		//DebugLog("%s END",__FUNCTION__);
	}
}

int CCastleSiege::GetNpcData(int iNpcType, int iNpcIndex, _CS_NPC_DATA & pRetNpcData)
{
	//DebugLog("%s START",__FUNCTION__);
	pRetNpcData.Clear();
	BOOL bEXIST = FALSE;
	
	//EnterCriticalSection(&m_critNpcData);

	//_CS_NPC_DATA* it = this->m_vtNpcData.begin();
	std::vector<_CS_NPC_DATA>::iterator it = this->m_vtNpcData.begin();

	while( it != this->m_vtNpcData.end() )
	{
		//std::vector<_CS_NPC_DATA>::iterator NpcData = it;
		//_CS_NPC_DATA & NpcData = _CS_NPC_DATA(*it);
		_CS_NPC_DATA wNpcData(*it);
		_CS_NPC_DATA & NpcData = wNpcData;

		if( NpcData.m_iNPC_NUM == iNpcType && NpcData.m_iNPC_INDEX == iNpcIndex && NpcData.m_bIN_USE == 1 && NpcData.m_iNPC_LIVE > 0 )
		{
			int iObjIndex = NpcData.m_iNPC_OBJINDEX;

			if( gObjIsConnected(iObjIndex) && gObj[iObjIndex].m_btCsNpcType != 0 )
			{
				pRetNpcData = NpcData;
				bEXIST = TRUE;
				break;
			}
		}
		it++;
	}

	//LeaveCriticalSection(&m_critNpcData);

	//DebugLog("%s END",__FUNCTION__);
	return bEXIST;
}

BOOL CCastleSiege::CheckCastleHasMoney(__int64 i64CastleMoney)
{
	__try
	{
	//DebugLog("%s START",__FUNCTION__);
	if( this->m_i64CastleMoney >= i64CastleMoney )
	{
		return TRUE;
	}
	return FALSE;
	}__finally
	{
	//DebugLog("%s END",__FUNCTION__);
	}
}

void CCastleSiege::SendCsGateStateViewPort(int iGateIndex, BYTE btOperate)
{
	__try
	{
	//DebugLog("%s START",__FUNCTION__);
	if( gObjIsConnected(iGateIndex) == FALSE )
		return;

	LPOBJ lpObj = &gObj[iGateIndex];

	if( lpObj->VPCount2 < 1 )
		return;

	int tObjNum = -1;

	for( int i = 0; i < MAX_VIEWPORT; i++ )
	{
		tObjNum = lpObj->VpPlayer2[i].number;

		if( tObjNum >= 0 )
		{
			if( gObj[tObjNum].Type == OBJ_USER && gObjIsConnected(tObjNum) )
			{
				GCAnsCsGateCurState(tObjNum, iGateIndex, btOperate);
			}
		}
	}
	}__finally
	{
	//DebugLog("%s END",__FUNCTION__);
	}
}

int CCastleSiege::OperateGate(int iObjIndex, int bOpenType)
{
	//DebugLog("%s START",__FUNCTION__);
	if( gObjIsConnected(iObjIndex) == FALSE )
	{
	//DebugLog("%s END",__FUNCTION__);
		return FALSE;
	}

	BOOL bRETVAL = FALSE;

	//EnterCriticalSection(&m_critNpcData);

	//_CS_NPC_DATA* it = this->m_vtNpcData.begin();
	std::vector<_CS_NPC_DATA>::iterator it = this->m_vtNpcData.begin();

	while( it != this->m_vtNpcData.end() )
	{
		std::vector<_CS_NPC_DATA>::iterator NpcData = it;
		//_CS_NPC_DATA & NpcData = _CS_NPC_DATA(*it);
		//LogAddTD("[CastleSiege][OperateGate] Looking Index:%d Obj:%d Num:%d / Looking Obj:%d Num:277", NpcData->m_iNPC_INDEX, NpcData->m_iNPC_OBJINDEX, NpcData->m_iNPC_NUM, iObjIndex);

		if( (NpcData->m_iNPC_NUM == 277) && (NpcData->m_iNPC_OBJINDEX == iObjIndex) && (NpcData->m_bIN_USE == TRUE) && (NpcData->m_iNPC_LIVE == 2) )
		{
			LogAddTD("[CastleSiege][OperateGate] Found Index:%d Obj:%d Num:%d / Looking Obj:%d Num:277", NpcData->m_iNPC_INDEX, NpcData->m_iNPC_OBJINDEX, NpcData->m_iNPC_NUM, iObjIndex);
			int iObjNumber = NpcData->m_iNPC_OBJINDEX;

			if( gObj[iObjNumber].m_btCsNpcType != 0 )
			{
				SetGateBlockState(NpcData->m_iNPC_SX, NpcData->m_iNPC_SY, bOpenType);
				gObj[iObjNumber].m_btCsGateOpen = bOpenType;

				if (bOpenType == 1)
				{
					GCSkillInfoSend(&gObj[iObjNumber], 1, 0x0D);
				} else {
					GCSkillInfoSend(&gObj[iObjNumber], 0, 0x0D);
				}

				bRETVAL = TRUE;

				break;
			}
		}
		it++;
	}

	//LeaveCriticalSection(&m_critNpcData);

	//DebugLog("%s END",__FUNCTION__);
	return bRETVAL;
}

BOOL CCastleSiege::CheckCsGateAlive(int iGateIndex)
{
	__try
	{
	//DebugLog("%s START",__FUNCTION__);
	if( !gObjIsConnected(iGateIndex) || gObj[iGateIndex].Class != 277 ) 
	{
		return FALSE;
	}

	return TRUE;
	}__finally
	{
	//DebugLog("%s END",__FUNCTION__);
	}
}

void CCastleSiege::AddMiniMapDataReqUser(int iIndex)
{
	//DebugLog("%s START",__FUNCTION__);
	//EnterCriticalSection(&this->m_critCsMiniMap);
	
	
	if( std::find(this->m_vtMiniMapReqUser.begin(), this->m_vtMiniMapReqUser.end(), iIndex) == this->m_vtMiniMapReqUser.end() )
	{
		this->m_vtMiniMapReqUser.push_back(iIndex);
	}

	//LeaveCriticalSection(&this->m_critCsMiniMap);

	//DebugLog("%s END",__FUNCTION__);
}



void CCastleSiege::DelMiniMapDataReqUser(int iIndex)
{
	//DebugLog("%s START",__FUNCTION__);
	//EnterCriticalSection(&this->m_critCsMiniMap);
	
	PINT it;
	
	//if( ( it = std::find(this->m_vtMiniMapReqUser.begin(), this->m_vtMiniMapReqUser.end(), iIndex)) != this->m_vtMiniMapReqUser.end() )
	if ( (std::find(this->m_vtMiniMapReqUser.begin(), this->m_vtMiniMapReqUser.end(), iIndex)) != this->m_vtMiniMapReqUser.end() )
	{
		//this->m_vtMiniMapReqUser.erase(it);
		this->m_vtMiniMapReqUser.erase( std::find(this->m_vtMiniMapReqUser.begin(), this->m_vtMiniMapReqUser.end(), iIndex) );
	}

	//LeaveCriticalSection(&this->m_critCsMiniMap);

	//DebugLog("%s END",__FUNCTION__);
}

struct PMSG_ANS_NPCDBLIST // 0xc
{
    PWMSG_HEAD h; // +0x0(0x4)
    BYTE btResult; // +0x4(0x1)
    int iCount; // +0x8(0x4)
};

struct PMSG_NPCDBLIST // 0x1c
{
    int iNpcNumber; // +0x0(0x4)
    int iNpcIndex; // +0x4(0x4)
    int iNpcDfLevel; // +0x8(0x4)
    int iNpcRgLevel; // +0xc(0x4)
    int iNpcMaxHp; // +0x10(0x4)
    int iNpcHp; // +0x14(0x4)
    UCHAR btNpcX; // +0x18(0x1)
    UCHAR btNpcY; // +0x19(0x1)
    UCHAR btNpcLive; // +0x1a(0x1)
};

void CCastleSiege::SendNpcStateList(int iIndex, int iNpcType)
{
	//DebugLog("%s START",__FUNCTION__);
	if( gObjIsConnected(iIndex) == FALSE )
		return;

	char cNPC_LIST[(sizeof(PMSG_NPCDBLIST) * 50)+sizeof(PMSG_ANS_NPCDBLIST)] = {0};
	int iNPC_COUNT = 0;

	PMSG_ANS_NPCDBLIST* lpMsgSend = (PMSG_ANS_NPCDBLIST*)&cNPC_LIST[0];
	PMSG_NPCDBLIST* lpMsgSendBody = (PMSG_NPCDBLIST*)&cNPC_LIST[sizeof(PMSG_ANS_NPCDBLIST)];

	//EnterCriticalSection(&m_critNpcData);
	
	//_CS_NPC_DATA* it =  this->m_vtNpcData.begin();
	std::vector<_CS_NPC_DATA>::iterator it =  this->m_vtNpcData.begin();

	while( it != this->m_vtNpcData.end() )
	{
		if( iNPC_COUNT >= 50 )
			break;
	
		std::vector<_CS_NPC_DATA>::iterator NpcData = it;
		//_CS_NPC_DATA & NpcData = _CS_NPC_DATA(*it);
		//LogAddTD("[CastleSiege][SendNpcStateList] Found NPC Index:%d Num:%d [Looking:%d]", NpcData->m_iNPC_INDEX, NpcData->m_iNPC_NUM, iNpcType);

		if( NpcData->m_iNPC_NUM == iNpcType && NpcData->m_bIN_USE == TRUE )
		{
			int iLife = 0;
			int iMaxLife = 0;
			BYTE btNpcLive = 0;
			int iObjIndex = NpcData->m_iNPC_OBJINDEX;

			if( gObjIsConnected(iObjIndex) )
			{
				if( gObj[iObjIndex].Class == iNpcType && gObj[iObjIndex].Live == 1 && gObj[iObjIndex].Life != 0 && gObj[iObjIndex].m_btCsNpcType != 0 )
				{
					iLife = gObj[iObjIndex].Life;
					iMaxLife = gObj[iObjIndex].MaxLife;
					btNpcLive = 1;
				}
				else
				{
					iLife = NpcData->m_iNPC_BASE_MAXHP;
					iMaxLife = NpcData->m_iNPC_BASE_MAXHP;
					btNpcLive = 0;
				}
			}
			else
			{
				iLife = NpcData->m_iNPC_BASE_MAXHP;
				iMaxLife = NpcData->m_iNPC_BASE_MAXHP;
				btNpcLive = 0;
			}
			lpMsgSendBody[iNPC_COUNT].iNpcNumber = NpcData->m_iNPC_NUM;
			lpMsgSendBody[iNPC_COUNT].iNpcIndex = NpcData->m_iNPC_INDEX;
			lpMsgSendBody[iNPC_COUNT].iNpcDfLevel = NpcData->m_iNPC_DF_LEVEL;
			lpMsgSendBody[iNPC_COUNT].iNpcRgLevel = NpcData->m_iNPC_RG_LEVEL;
			lpMsgSendBody[iNPC_COUNT].iNpcHp = iLife;
			lpMsgSendBody[iNPC_COUNT].iNpcMaxHp = iMaxLife;
			lpMsgSendBody[iNPC_COUNT].btNpcX = NpcData->m_iNPC_SX;
			lpMsgSendBody[iNPC_COUNT].btNpcY = NpcData->m_iNPC_SY;
			lpMsgSendBody[iNPC_COUNT].btNpcLive = btNpcLive;

			iNPC_COUNT++;
		}

		it++;
	}
	//LeaveCriticalSection(&m_critNpcData);

	lpMsgSend->btResult = 1;
	lpMsgSend->h.set((LPBYTE)&cNPC_LIST, 0xB3, (sizeof(PMSG_NPCDBLIST) * iNPC_COUNT)+sizeof(PMSG_ANS_NPCDBLIST));
	lpMsgSend->iCount = iNPC_COUNT;

	DataSend(iIndex, (LPBYTE)&cNPC_LIST, (sizeof(PMSG_NPCDBLIST) * iNPC_COUNT)+sizeof(PMSG_ANS_NPCDBLIST));

	//DebugLog("%s END",__FUNCTION__);
}

int CCastleSiege::GetCsAttkGuildList(PMSG_CSATTKGUILDLIST* lpMsgBody, int & iCount)
{
	//DebugLog("%s START",__FUNCTION__);
	iCount = 0;

	if( lpMsgBody == NULL )
	{
		//DebugLog("%s lpMsgBody == NULL END\n",__FUNCTION__);
		return 0;
	}

	if( this->m_iCastleSiegeState == CASTLESIEGE_STATE_IDLE_3 )
	{
		//DebugLog("%s this->m_iCastleSiegeState END\n",__FUNCTION__);
		return 2;
	}


	//if( (this->m_iCastleSiegeState != CASTLESIEGE_STATE_NOTIFY) && (this->m_iCastleSiegeState != CASTLESIEGE_STATE_READYSIEGE) )
	if( this->m_iCastleSiegeState == CASTLESIEGE_STATE_STARTSIEGE )
	{
		//DebugLog("%s this->m_iCastleSiegeState END\n",__FUNCTION__);
		return 2;
	}

	if( (this->m_bCsBasicGuildInfoLoadOK == FALSE) || (this->m_bCsTotalGuildInfoLoadOK == FALSE ) )
	{
		//DebugLog("%s this->m_bCsBasicGuildInfoLoadOK END\n",__FUNCTION__);
		return 3;
	}

	////EnterCriticalSection(&this->m_critCsTotalGuildInfo);

	for( map<basic_string<char> , _CS_TOTAL_GUILD_DATA>::iterator it = (map<basic_string<char> , _CS_TOTAL_GUILD_DATA>::iterator)m_mapCsTotalGuildInfo.begin(); it != this->m_mapCsTotalGuildInfo.end(); it++)
	{
		lpMsgBody[iCount].btCsJoinSide = it->second.m_iCsGuildID;
		lpMsgBody[iCount].btGuildInvolved = it->second.m_bGuildInvolved;
		
		basic_string<char> szGuildName = it->first;

		memset(&lpMsgBody[iCount].szGuildName, 0x00, MAX_GUILD_LEN);
		memcpy(&lpMsgBody[iCount].szGuildName, &szGuildName[0], MAX_GUILD_LEN);

		iCount++;
	}

	////LeaveCriticalSection(&this->m_critCsTotalGuildInfo);

	//DebugLog("%s END",__FUNCTION__);
	return 1;
}

BOOL CCastleSiege::CheckGuardianStatueExist()
{
	//DebugLog("%s START",__FUNCTION__);
	__try
	{
		for( int iIDX = 0; iIDX < OBJ_MAXMONSTER; iIDX++ )
		{
			if( gObj[iIDX].Class == 283 )
			{
				if( gObj[iIDX].Connected > PLAYER_LOGGED && gObj[iIDX].Live == 1 )
				{
					if( gObj[iIDX].MapNumber == MAP_INDEX_CASTLESIEGE )
					{
						return TRUE;
					}
				}
			}
		}

		return FALSE;
	}__finally
	{
		//DebugLog("%s END",__FUNCTION__);
	}
}



int CCastleSiege::CheckOverlapCsMarks(int iIndex)
{
	__try
	{
	//DebugLog("%s START",__FUNCTION__);
	for( int x = INVETORY_WEAR_SIZE; x < ReadConfig.MAIN_INVENTORY_SIZE(iIndex,false); x++ )
	{
		if( gObj[iIndex].pInventory[x].IsItem() == TRUE )
		{
			if( (gObj[iIndex].pInventory[x].m_Type == ITEMGET(14,21)) && (gObj[iIndex].pInventory[x].m_Level == 3) )
			{
				int dur = gObj[iIndex].pInventory[x].m_Durability;

				if( ITEM_DUR_RANGE(dur) )
				{
					return x;
				}
			}
		}
	}
	return -1;
	}__finally
	{
	//DebugLog("%s END",__FUNCTION__);
	}
}


BOOL CCastleSiege::CheckTeleportMagicAxisY(int iStartY, int iTargetX, int iTargetY)
{
	__try
	{
	//DebugLog("%s START",__FUNCTION__);
	for( int iSTEP = 0; iSTEP < 3; iSTEP++ )
	{
		if( (iStartY >= g_iCastleWallStepAxisY[iSTEP]) && (iTargetY <= g_iCastleWallStepAxisY[iSTEP]) && (iTargetX < 144) )
		{
			return FALSE;
		}
	}
	return TRUE;
	}__finally
	{
	//DebugLog("%s END",__FUNCTION__);
	}
}

void CCastleSiege::ClearCastleTowerBarrier()
{
	__try
	{
		//DebugLog("%s START",__FUNCTION__);
		for( int iIDX = 0; iIDX < OBJ_MAXMONSTER; iIDX++ )
		{
			if( ::gObjIsConnected(iIDX) == FALSE )
				continue;

			if( gObj[iIDX].Class == 215 )	//Shield
				gObjDel(iIDX);
		}
	}__finally
	{
		//DebugLog("%s END",__FUNCTION__);
	}
}

void CCastleSiege::ClearDbNPC()
{
	__try
	{
	//DebugLog("%s START",__FUNCTION__);
	for( int n = 0 ; n < OBJ_MAXMONSTER; n++ )
	{
		if( gObj[n].MapNumber == MAP_INDEX_CASTLESIEGE )
		{
			//[277]		"Castle Gate"	[283]	"Guardian Statue"		[219]	"Castle Gate Switch"	
			if( gObj[n].Class == 277 || gObj[n].Class == 283 || gObj[n].Class == 219 )
			{
				if( gObj[n].Class == 277 )
				{
					this->SetGateBlockState(gObj[n].X, gObj[n].Y, 1);
				}
				gObjDel(n);
			}
		}
	}
	}__finally
	{
	//DebugLog("%s END",__FUNCTION__);
	}
}

void CCastleSiege::CreateDbNPC_INS()
{
	//DebugLog("%s START",__FUNCTION__);
	this->ClearDbNPC();

	//EnterCriticalSection(&this->m_critNpcData);

	//_CS_NPC_DATA* it = this->m_vtNpcData.begin();
	std::vector<_CS_NPC_DATA>::iterator it = this->m_vtNpcData.begin();

	while( it != this->m_vtNpcData.end() )
	{
		_CS_NPC_DATA tmpNpcData = *it;
		_CS_NPC_DATA & NpcData = tmpNpcData;

		if( NpcData.m_bNPC_DBSAVE == TRUE )
		{
			it->SetBaseValue();
			it->m_iNPC_LIVE = TRUE;
			it->m_iNPC_SIDE = 1;
			it->m_iNPC_OBJINDEX = -1;
			it->m_iCS_GATE_LEVER_INDEX = -1;
		}
		it++;
	}

	//LeaveCriticalSection(&this->m_critNpcData);

	this->m_bDbNpcCreated = FALSE;

	//DebugLog("%s END",__FUNCTION__);
}

void CCastleSiege::CreateNonDbNPC(int bDelFirst)
{
	//DebugLog("%s START",__FUNCTION__);
	if( bDelFirst )
	{
		ClearNonDbNPC();
	}

	//EnterCriticalSection(&m_critNpcData);

	//_CS_NPC_DATA * it = this->m_vtNpcData.begin();
	std::vector<_CS_NPC_DATA>::iterator it = this->m_vtNpcData.begin();

	while(it != this->m_vtNpcData.end())
	{
		std::vector<_CS_NPC_DATA>::iterator pNpcData = it;
		//_CS_NPC_DATA & pNpcData = _CS_NPC_DATA(*it);

		if(pNpcData->m_bIN_USE == TRUE && pNpcData->m_bNPC_DBSAVE == FALSE)
		{
			int iNPC_INDEX = gObjAddMonster(MAP_INDEX_CASTLESIEGE);

			if(iNPC_INDEX >= 0)
			{
				gObjSetMonster(iNPC_INDEX, pNpcData->m_iNPC_NUM,"CCastleSiege::CreateNonDbNPC");
				gObj[iNPC_INDEX].m_PosNum = -1;
				gObj[iNPC_INDEX].X = pNpcData->m_iNPC_SX;
				gObj[iNPC_INDEX].Y = pNpcData->m_iNPC_SY;
				gObj[iNPC_INDEX].MapNumber = MAP_INDEX_CASTLESIEGE;
				gObj[iNPC_INDEX].TX = gObj[iNPC_INDEX].X;
				gObj[iNPC_INDEX].TY = gObj[iNPC_INDEX].Y;
				gObj[iNPC_INDEX].m_OldX = gObj[iNPC_INDEX].X;
				gObj[iNPC_INDEX].m_OldY = gObj[iNPC_INDEX].Y;
				gObj[iNPC_INDEX].Dir = pNpcData->m_iNPC_DIR;
				gObj[iNPC_INDEX].StartX = gObj[iNPC_INDEX].X;
				gObj[iNPC_INDEX].StartY = gObj[iNPC_INDEX].Y;
				gObj[iNPC_INDEX].DieRegen = 0;
				gObj[iNPC_INDEX].MaxRegenTime = 0;
				gObj[iNPC_INDEX].Life = pNpcData->m_iNPC_HP;
				gObj[iNPC_INDEX].MaxLife = pNpcData->m_iNPC_MAXHP;


				switch( pNpcData->m_iNPC_SIDE )
				{
				case 1:
					gObj[iNPC_INDEX].m_btCsNpcType = 2;
					break;
				case 2:
					gObj[iNPC_INDEX].m_btCsNpcType = 3;
					break;
				default:
					gObj[iNPC_INDEX].m_btCsNpcType = 3;
					break;
				}

				it->m_iNPC_OBJINDEX = iNPC_INDEX;
				it->m_iNPC_LIVE = 2;
			}
		}
		it++;
	}
	//LeaveCriticalSection(&m_critNpcData);

	//DebugLog("%s END",__FUNCTION__);
}


int CCastleSiege::DelGemOfDefend(int iIndex, int iNeedGemOfDefend)
{
	__try
	{
	//DebugLog("%s START",__FUNCTION__);
	if( gObjIsConnected(iIndex) == FALSE )
	{
		return FALSE;
	}

	if( iNeedGemOfDefend <= 0 )
	{
		return FALSE;
	}


	int iGemOfDefendCount = 0;

	for( int x = 0; x < ReadConfig.INVENTORY_SIZE(iIndex,false); x++ )
	{
		if( gObj[iIndex].pInventory[x].IsItem() == TRUE )
		{
			if( gObj[iIndex].pInventory[x].m_Type == ITEMGET(14,31) )
			{
				iGemOfDefendCount++;
			}
		}
	}

	if( iGemOfDefendCount < iNeedGemOfDefend )
	{
		return FALSE;
	}

	int iGemOfDefendDeletCount = 0;

	for( int x = 0; x < ReadConfig.INVENTORY_SIZE(iIndex,false); x++ )
	{
		if( gObj[iIndex].pInventory[x].IsItem() == TRUE )
		{
			if( gObj[iIndex].pInventory[x].m_Type == ITEMGET(14,31) )
			{
				LogAddTD("[CastleSiege] [%s][%s] Deleting Gem of Defend (GUILD:%s) - Serial:%d",
					gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].pInventory[x].m_Number);

				gObjInventoryDeleteItem(iIndex, x);
				GCInventoryItemDeleteSend(iIndex, x, 1);

				iGemOfDefendDeletCount++;

				if( iGemOfDefendDeletCount >= iNeedGemOfDefend )
				{
					break;
				}
			}
		}
	}

	return TRUE;
	}__finally
	{
	//DebugLog("%s END",__FUNCTION__);
	}
}

BOOL CCastleSiege::DelNPC(int iIndex, int iNpcType, int bDbSave)
{
	//DebugLog("%s START",__FUNCTION__);
	if( !OBJMAX_RANGE(iIndex) )
	{
		//DebugLog("%s END OBJMAX_RANGE(iIndex)",__FUNCTION__);
		return FALSE;
	}

	BOOL bEXIST = FALSE;
	BOOL bNPC_DB = FALSE;

	_CS_NPC_DATA pTempNpcData;

	//EnterCriticalSection(&m_critNpcData);

	//_CS_NPC_DATA* it = this->m_vtNpcData.begin();
	std::vector<_CS_NPC_DATA>::iterator it = this->m_vtNpcData.begin();

	while( it != this->m_vtNpcData.end() )
	{
		_CS_NPC_DATA wNpcData(*it);

		_CS_NPC_DATA & NpcData = wNpcData;

		if( NpcData.m_iNPC_NUM == iNpcType && NpcData.m_iNPC_OBJINDEX == iIndex)
		{
			if( NpcData.m_bIN_USE == TRUE )
			{
				if( NpcData.m_iNPC_LIVE > 0 )
				{
					it->SetBaseValue();

					it->m_iNPC_OBJINDEX = OBJ_EMPTY;
					it->m_iNPC_LIVE = FALSE;

					if( NpcData.m_iNPC_NUM == 277 || NpcData.m_iNPC_NUM == 283 )
					{
						NpcData.m_iNPC_HP = NpcData.m_iNPC_BASE_HP;
						NpcData.m_iNPC_MAXHP = NpcData.m_iNPC_BASE_MAXHP;
						NpcData.m_iNPC_RG_LEVEL = NpcData.m_iNPC_BASE_RG_LEVEL;
						NpcData.m_iNPC_DF_LEVEL = NpcData.m_iNPC_BASE_DF_LEVEL;
					}
					
					bEXIST = TRUE;
					pTempNpcData = NpcData;

					if( NpcData.m_iNPC_NUM == 277 )
					{
						SetGateBlockState(NpcData.m_iNPC_SX, NpcData.m_iNPC_SY, 1);

						LogAddTD("[CastleSiege][State:%d] Delete Castle Gate (CLS:%d, IDX:%d, X:%d. Y:%d)",
							m_iCastleSiegeState, NpcData.m_iNPC_NUM, NpcData.m_iNPC_INDEX, NpcData.m_iNPC_SX, NpcData.m_iNPC_SY);
					}

					if( NpcData.m_iNPC_NUM == 283 )
					{
						if( m_iCastleSiegeState == CASTLESIEGE_STATE_STARTSIEGE )
						{
							if( CheckGuardianStatueExist() == FALSE )
							{
								ClearCastleTowerBarrier();
								SetCastleTowerAccessable(TRUE);
							}
							LogAddTD("[CastleSiege] Defend Statue is Broken (CLS:%d, IDX:%d, X:%d, Y:%d)",
								NpcData.m_iNPC_NUM, NpcData.m_iNPC_INDEX, NpcData.m_iNPC_SX, NpcData.m_iNPC_SY);
						}
					}
					break;
				}
			}
		}
		it++;
	}
	//LeaveCriticalSection(&m_critNpcData);

	if( bEXIST == TRUE && bDbSave == TRUE && m_iCastleSiegeState != CASTLESIEGE_STATE_STARTSIEGE )
	{
		GS_GDReqNpcRemove(m_iMapSvrGroup, pTempNpcData.m_iNPC_NUM, pTempNpcData.m_iNPC_INDEX);
	}

	//DebugLog("%s END",__FUNCTION__);
	return TRUE;
}

void CCastleSiege::SetCrownSwitchUserIndex(int iMonsterClass, int iIndex)
{
	__try
	{
	//DebugLog("%s START",__FUNCTION__);
	switch( iMonsterClass )
	{
	case 217:	/*Crown Switch 1*/
			this->m_iCastleSwitchAccessUser1 = iIndex;
		break;
	case 218:	/*Crown Switch 2*/
			this->m_iCastleSwitchAccessUser2 = iIndex;
		break;
	}
	}__finally
	{
	//DebugLog("%s END",__FUNCTION__);
	}
}

int CCastleSiege::GetCrownSwitchUserIndex(int iMonsterClass)
{
	__try
	{
	//DebugLog("%s START",__FUNCTION__);
	int iUserIndex = OBJ_EMPTY;

	switch( iMonsterClass )
	{
	case 217:	/*Crown Switch 1*/
			iUserIndex = this->m_iCastleSwitchAccessUser1;
		break;
	case 218:	/*Crown Switch 2*/
			iUserIndex = this->m_iCastleSwitchAccessUser2;
		break;
	}
	return iUserIndex;
	}__finally
	{
	//DebugLog("%s END",__FUNCTION__);
	}
}


void CCastleSiege::ResetCrownSwitchUserIndex(int iMonsterClass)
{
	__try
	{
	//DebugLog("%s START",__FUNCTION__);
	switch( iMonsterClass )
	{
	case 217:	/*Crown Switch 1*/
			this->m_iCastleSwitchAccessUser1 = OBJ_EMPTY;
		break;
	case 218:	/*Crown Switch 2*/
			this->m_iCastleSwitchAccessUser2 = OBJ_EMPTY;
		break;
	}
	}__finally
	{
	//DebugLog("%s END",__FUNCTION__);
	}
}

void CCastleSiege::ReSpawnAllUser()
{
	__try
	{
	//DebugLog("%s START",__FUNCTION__);
	int iTX, iTY;
	BYTE btMapAttr;

	for( int iUSER = OBJ_STARTUSERINDEX; iUSER < OBJMAX; iUSER++ )
	{
		if( gObjIsConnected(iUSER) == FALSE )
			continue;

		if( gObj[iUSER].MapNumber != MAP_INDEX_CASTLESIEGE )
			continue;

		BYTE btJoinSide = gObj[iUSER].m_btCsJoinSide;

		if( btJoinSide < 0 ) 
		{
			btJoinSide = 0;
		}

		if( btJoinSide > 2 )
		{
			btJoinSide = 2;
		}

		BOOL bMoveOK = FALSE;

		switch( btJoinSide )
		{
		case 1:
			{
				for(int iCNT = 0; iCNT < 100; iCNT++)
				{
					iTX = (rand() % (g_iCsUserReSpawnArea[1][2] - g_iCsUserReSpawnArea[1][0])) + g_iCsUserReSpawnArea[1][0];
					iTY = (rand() % (g_iCsUserReSpawnArea[1][3] - g_iCsUserReSpawnArea[1][1])) + g_iCsUserReSpawnArea[1][1];

					btMapAttr = MapC[MAP_INDEX_CASTLESIEGE].m_attrbuf[iTY * 256 + iTX];

					if((btMapAttr & 4) != 4)
					{
						bMoveOK = TRUE;
						break;
					}
				}
			}
			break;
		case 2:
			{
				for(int iCNT = 0; iCNT < 100; iCNT++)
				{
					iTX = (rand() % (g_iCsUserReSpawnArea[2][2] - g_iCsUserReSpawnArea[2][0])) + g_iCsUserReSpawnArea[2][0];
					iTY = (rand() % (g_iCsUserReSpawnArea[2][3] - g_iCsUserReSpawnArea[2][1])) + g_iCsUserReSpawnArea[2][1];

					btMapAttr = MapC[MAP_INDEX_CASTLESIEGE].m_attrbuf[iTY * 256 + iTX];

					if((btMapAttr & 4) != 4)
					{
						bMoveOK = TRUE;
						break;
					}
				}
			}
			break;
		default:
			{
				for(int iCNT = 0; iCNT < 100; iCNT++)
				{
					iTX = (rand() % (g_iCsUserReSpawnArea[0][2] - g_iCsUserReSpawnArea[0][0])) + g_iCsUserReSpawnArea[0][0];
					iTY = (rand() % (g_iCsUserReSpawnArea[0][3] - g_iCsUserReSpawnArea[0][1])) + g_iCsUserReSpawnArea[0][1];

					btMapAttr = MapC[MAP_INDEX_CASTLESIEGE].m_attrbuf[iTY * 256 + iTX];

					if((btMapAttr & 4) != 4)
					{
						bMoveOK = TRUE;
						break;
					}
				}
			}
			break;
		}

		if( bMoveOK )
		{
			gObj[iUSER].m_State = 32;
			gObj[iUSER].X = iTX;
			gObj[iUSER].Y = iTY;
			gObj[iUSER].TX = iTX;
			gObj[iUSER].TY = iTY;
			gObj[iUSER].MapNumber = MAP_INDEX_CASTLESIEGE;
			gObj[iUSER].PathCount = 0;
			gObj[iUSER].Teleport = 0;

			gObjClearViewport(&gObj[iUSER]);
			GCTeleportSend(&gObj[iUSER], 0xFF, MAP_INDEX_CASTLESIEGE, gObj[iUSER].X, gObj[iUSER].Y, gObj[iUSER].Dir);

			if( gObj[iUSER].m_Change >= 0 )
			{
				gObjViewportListProtocolCreate(&gObj[iUSER]);
			}
			gObj[iUSER].RegenMapNumber = MAP_INDEX_CASTLESIEGE;
			gObj[iUSER].RegenMapX = iTX;
			gObj[iUSER].RegenMapY = iTY;
			gObj[iUSER].RegenOk = TRUE;
		}
	}
	}__finally
	{
	//DebugLog("%s END",__FUNCTION__);
	}
}

struct CSP_REQ_CSSAVETOTALGUILDINFO {
	PWMSG_HEAD h; // 0
	WORD wMapSvrNum; // 4
	int iCount; // 8
};

struct CSP_CSSAVETOTALGUILDINFO {
	char szGuildName[8]; // 0
	int iCsGuildID; // 8 
	int iGuildInvolved; // c
};


void CCastleSiege::SaveCsTotalGuildInfo()
{
	//DebugLog("%s START",__FUNCTION__);
	if( m_bCsTotalGuildInfoLoadOK == FALSE )
	{
		LogAddC(2, "[CastleSiege] CCastleSiege::SaveCsTotalGuildInfo() ERROR - m_bCsTotalGuildInfoLoadOK == FALSE");
		//DebugLog("%s END",__FUNCTION__);
		return;
	}
	char cBUFFER[2684]={0};

	CSP_REQ_CSSAVETOTALGUILDINFO* lpMsgSend = (CSP_REQ_CSSAVETOTALGUILDINFO*)&cBUFFER[0];
	CSP_CSSAVETOTALGUILDINFO* lpMsgSendBody =(CSP_CSSAVETOTALGUILDINFO*)&cBUFFER[sizeof(CSP_REQ_CSSAVETOTALGUILDINFO)];

	lpMsgSend->wMapSvrNum = m_iMapSvrGroup;
	lpMsgSend->iCount = 0;

	map<basic_string<char>, _CS_TOTAL_GUILD_DATA>::iterator it;

	for( it = m_mapCsTotalGuildInfo.begin(); it != m_mapCsTotalGuildInfo.end(); it++)
	{
		if( lpMsgSend->iCount > 150 )
		{
			break;
		}

		basic_string<char> szGuildName = it->first;
		strcpy(&lpMsgSendBody[lpMsgSend->iCount].szGuildName[0],&it->first[0]);
		//DebugLog("%s RECV: %s",__FUNCTION__,szGuildName);

		//memcpy(&lpMsgSendBody[lpMsgSend->iCount].szGuildName, &szGuildName[0], MAX_GUILD_LEN);
		
		lpMsgSendBody[lpMsgSend->iCount].iCsGuildID = it->second.m_iCsGuildID;

		//GUILD CS Involvement Fix
		it->second.m_bGuildInvolved = 1;
		lpMsgSendBody[lpMsgSend->iCount].iGuildInvolved = it->second.m_bGuildInvolved;

		lpMsgSend->iCount++;
	}

	lpMsgSend->h.set((LPBYTE)lpMsgSend, 0x87, ( sizeof(CSP_CSSAVETOTALGUILDINFO) * lpMsgSend->iCount) + sizeof(CSP_REQ_CSSAVETOTALGUILDINFO) );
	cDBSMng.Send(cBUFFER, ( sizeof(CSP_CSSAVETOTALGUILDINFO) * lpMsgSend->iCount) + sizeof(CSP_REQ_CSSAVETOTALGUILDINFO) );

	map<basic_string<char>, _CS_TOTAL_GUILD_DATA>::iterator it2;
	
	for( it2 = m_mapCsTotalGuildInfo.begin(); it2 != m_mapCsTotalGuildInfo.end(); it2++ )
	{
		basic_string<char> szGuildName = it2->first;
		_CS_TOTAL_GUILD_DATA GuildData = it2->second;

		LogAddTD("[CastleSiege] Siege Guild List Save - GNAME:%s, CSGUID:%d, INVOLVED:%d",
			(char*)&szGuildName[0], GuildData.m_iCsGuildID, GuildData.m_bGuildInvolved);
	}

	//DebugLog("%s END",__FUNCTION__);
}

void CCastleSiege::SendAllUserAnyData(BYTE* lpMsg, int iSize)
{
	__try
	{
	//DebugLog("%s START",__FUNCTION__);
	for( int i = OBJ_STARTUSERINDEX; i < OBJMAX; i++ )
	{
		if( (gObj[i].Connected == PLAYER_PLAYING) && (gObj[i].Type == OBJ_USER) )
		{
			::DataSend(i, lpMsg, iSize);
		}
	}
	}__finally
	{
	//DebugLog("%s END",__FUNCTION__);
	}
}

void CCastleSiege::SendCastleStateSync()
{
	__try
	{
		//DebugLog("%s START",__FUNCTION__);
		GS_GDReqCastleStateSync(this->m_iMapSvrGroup, this->m_iCastleSiegeState, this->m_iTaxRateChaos,
			this->m_iTaxRateStore, this->m_iTaxHuntZone, this->m_szCastleOwnerGuild);
	}__finally
	{
		//DebugLog("%s END",__FUNCTION__);
	}
}

void CCastleSiege::SendCsUserAnyData(BYTE* lpMsg, int iSize)
{
	__try
	{
	//DebugLog("%s START",__FUNCTION__);
	for( int i = OBJ_STARTUSERINDEX; i < OBJMAX; i++ )
	{
		if( (gObj[i].Connected == PLAYER_PLAYING) && (gObj[i].Type == OBJ_USER) )
		{
			if( gObj[i].MapNumber == MAP_INDEX_CASTLESIEGE )
			{
				DataSend(i, lpMsg, iSize);
			}
		}
	}
	}__finally
	{
	//DebugLog("%s END",__FUNCTION__);
	}
}

void CCastleSiege::SendCsUserAnyMsg(char* lpszMsg)
{
	__try
	{
	//DebugLog("%s START",__FUNCTION__);
	PMSG_NOTICE pNotice;
	pNotice.type = 0;	// 3
	pNotice.btCount = 0;	// 4
	pNotice.wDelay = 0;	// 6	
	pNotice.dwColor = 0;	// 8
	pNotice.btSpeed = 0;	// C

	if( lpszMsg == NULL )
		return;

	TNotice::MakeNoticeMsg(&pNotice, 0, lpszMsg);

	for( int i = OBJ_STARTUSERINDEX; i < OBJMAX; i++ )
	{
		if( (gObj[i].Connected == PLAYER_PLAYING) && (gObj[i].Type == OBJ_USER) )
		{
			if( gObj[i].MapNumber == MAP_INDEX_CASTLESIEGE )
			{
				DataSend(i, (LPBYTE)&pNotice, pNotice.h.size);
			}
		}
	}
	}__finally
	{
		//DebugLog("%s END",__FUNCTION__);
	}
}

struct PMSG_ANS_NOTIFY_CROWNSWITCH_INFO {
	PBMSG_HEAD2 h; // 0
	BYTE btIndex1; // 4
	BYTE btIndex2; // 5
	BYTE btSwitchState; // 6
	BYTE btJoinSide; // 7
	BYTE szGuildName[8]; // 8
	BYTE szUserName[10]; // 10
};


void CCastleSiege::NotifyCrownSwitchInfo(int iIndex) /*00578550*/
{
	__try
	{
	//DebugLog("%s START",__FUNCTION__);
	if( gObjIsConnected(iIndex) == FALSE )
	{
		LogAddC(2, "[CastleSiege] CCastleSiege::NotifyCrownSwitchInfo() - iCrownSwitchIndex is Invalid");

		return;
	}

	if( (gObj[iIndex].Class != 217) && (gObj[iIndex].Class != 218) )
	{
		LogAddC(2, "[CastleSiege] CCastleSiege::NotifyCrownSwitchInfo() - iCrownSwitchIndex is Invalid");

		return;
	}

	LPOBJ lpObj = &gObj[iIndex];

	if( lpObj->VPCount2 < 1 )
	{
		return;
	}

	PMSG_ANS_NOTIFY_CROWNSWITCH_INFO pMsg;
	char szMasterGuildName[MAX_GUILD_LEN];

	pMsg.h.set((LPBYTE)&pMsg, 0xB2, 0x20, sizeof(pMsg));

	pMsg.btIndex1 = SET_NUMBERH(LOWORD(iIndex));
	pMsg.btIndex2 = SET_NUMBERL(LOWORD(iIndex));
	pMsg.btJoinSide = 0;
	pMsg.btSwitchState = 0;

	memset(&pMsg.szGuildName, 0, MAX_GUILD_LEN);
	memset(&pMsg.szUserName, 0, MAX_ACCOUNT_LEN);

	int iCrownSwitchUserIndex = g_CastleSiege.GetCrownSwitchUserIndex(lpObj->Class); // wtf!

	if( gObjIsConnected(iCrownSwitchUserIndex) )
	{
		LPOBJ lpSwitchObj = &gObj[iCrownSwitchUserIndex];

		pMsg.btSwitchState = TRUE;
		pMsg.btJoinSide = lpSwitchObj->m_btCsJoinSide;

		memcpy(&pMsg.szUserName, lpSwitchObj->Name, MAX_ACCOUNT_LEN);
		memcpy(&pMsg.szGuildName, lpSwitchObj->GuildName, MAX_GUILD_LEN);

		memset(&szMasterGuildName, 0, MAX_GUILD_LEN);

		if( lpSwitchObj->lpGuild )
		{
			if( UnionManager.GetUnionName(lpSwitchObj->lpGuild->iGuildUnion, szMasterGuildName) != -1 )
			{
				memcpy(&pMsg.szGuildName, szMasterGuildName, MAX_GUILD_LEN);
			}
		}
		DataSend(iCrownSwitchUserIndex, (LPBYTE)&pMsg, pMsg.h.size);
	}
	
	int tObjNum = OBJ_EMPTY;

	for( int i = 0; i < MAX_VIEWPORT; i++ )
	{
		tObjNum = lpObj->VpPlayer2[i].number;

		if( tObjNum >= 0 )
		{
			if( gObj[tObjNum].Type == OBJ_USER && gObj[tObjNum].Live )
			{
				DataSend(tObjNum, (LPBYTE)&pMsg, pMsg.h.size);
			}
		}
	}
	}__finally
	{
		//DebugLog("%s END",__FUNCTION__);
	}
}

void CCastleSiege::NotifyCsSelfLeftTime(int iIndex)
{
	__try
	{
		//DebugLog("%s START",__FUNCTION__);
		GCAnsSelfCsLeftTimeAlarm(iIndex, (this->m_iCS_REMAIN_MSEC/1000)/3600, ((this->m_iCS_REMAIN_MSEC/1000)%3600)/60);
	}__finally
	{
		//DebugLog("%s END",__FUNCTION__);
	}
}

int CCastleSiege::GetCsJoinSide(char* lpszGuildName, BYTE * btCsJoinSide, BYTE * bCsGuildInvolved)
{
	//DebugLog("%s START",__FUNCTION__);
	*btCsJoinSide = 0;
	*bCsGuildInvolved = 0;

	if( lpszGuildName == NULL )
	{
	//DebugLog("%s END",__FUNCTION__);
		return FALSE;
	}

	if( m_bDoRun == FALSE )
	{
	//DebugLog("%s END",__FUNCTION__);
		return FALSE;
	}

	if( m_iCastleSiegeState >= CASTLESIEGE_STATE_NOTIFY && CASTLESIEGE_STATE_STARTSIEGE <= m_iCastleSiegeState )
	{
	//DebugLog("%s END",__FUNCTION__);
		return FALSE;
	}

	if( m_bCsTotalGuildInfoLoadOK == FALSE )
	{
	//DebugLog("%s END",__FUNCTION__);
		return FALSE;
	}
	
	BOOL bRETVAL = FALSE;

	////EnterCriticalSection(&m_critCsTotalGuildInfo);

	//std::map<basic_string<char>, _CS_TOTAL_GUILD_DATA>::iterator it = (std::map<basic_string<char>, _CS_TOTAL_GUILD_DATA>::iterator)m_mapCsTotalGuildInfo.find(lpszGuildName);
	map<basic_string<char,char_traits<char> >,_CS_TOTAL_GUILD_DATA,less<basic_string<char,char_traits<char> > > >::iterator it = (map<basic_string<char,char_traits<char> >,_CS_TOTAL_GUILD_DATA,less<basic_string<char,char_traits<char> > > >::iterator)m_mapCsTotalGuildInfo.find(lpszGuildName);

	if( it != m_mapCsTotalGuildInfo.end() )
	{
		*btCsJoinSide = it->second.m_iCsGuildID;
		*bCsGuildInvolved = (it->second.m_bGuildInvolved != 0);

		bRETVAL = TRUE;
	}
	
	////LeaveCriticalSection(&m_critCsTotalGuildInfo);

	//DebugLog("%s END",__FUNCTION__);
	return bRETVAL;
}

void CCastleSiege::OperateGmCommand(int iIndex, int iGmCommand, void* lpParam)
{
	__try
	{
	//DebugLog("%s START",__FUNCTION__);
	if( gObjIsConnected(iIndex) == FALSE )
		return;

	//if(gObj[iIndex].Authority == 32)
	//{
	//}else
	//{
	//	if(gObj[iIndex].AuthorityCode != 0x3F)
	//	{
	//		if( (gObj[iIndex].Authority & 2) != 2 ) /*Check if user is GM*/
	//			return;
	//	}
	//}

	switch( iGmCommand )
	{
		case 0:
			{
				LPSTR szNewOwnerGuild = (char*)lpParam;

				if( szNewOwnerGuild == NULL )
				{
					MsgOutput(iIndex, "Failure to change the owner guild");
				}
				else
				{
					if( strcmp(szNewOwnerGuild, "") != 0 )
					{
						memset(&this->m_szCastleOwnerGuild, 0, sizeof(this->m_szCastleOwnerGuild));
						memcpy(&this->m_szCastleOwnerGuild, szNewOwnerGuild, MAX_GUILD_LEN);
						g_CastleSiegeSync.SetCastleOwnerGuild(this->m_szCastleOwnerGuild);
						this->m_btIsCastleOccupied = 1;
						MsgOutput(iIndex, "Owner guild changes: %s", this->m_szCastleOwnerGuild);
					}
					else
					{
						strcpy(this->m_szCastleOwnerGuild, "");
						this->m_btIsCastleOccupied = 0;
						g_CastleSiegeSync.SetCastleOwnerGuild(this->m_szCastleOwnerGuild);
						MsgOutput(iIndex, "Failure to change the owner guild");
					}
				}
			}
			break;
		case 1:
			{
				GetLocalTime(&m_tmStartDate);

				m_tmStartDate.wHour = 0;
				m_tmStartDate.wMinute = 0;
				m_tmStartDate.wSecond = 0;

				this->m_tmEndDate = this->m_tmStartDate;
				this->GetNextDay(&this->m_tmEndDate, m_iCastleSiegeCycle, 0, 0, 0);
				this->m_bFixCastleCycleStartDate = FALSE;
				this->m_btIsSiegeEnded = FALSE;
				this->CreateDbNPC_INS();

				GS_GDReqResetRegSiegeInfo(this->m_iMapSvrGroup);

				if( this->CheckSync() == FALSE )
				{
					LogAddC(2, "[CastleSiege] CCastleSiege::Init() - CheckSync() == FALSE");
					MsgBox("[CastleSiege] CCastleSiege::Init() - CheckSync() == FALSE");
					MsgOutput(iIndex, "Change of state failure - the siege was registered");
				}
				else
				{
					this->m_bDoRun = TRUE;
					this->m_dwNPC_DBSAVE_TICK_COUNT = GetTickCount();
					this->m_dwCALC_LEFTSIEGE_TICK_COUNT = 0;
					this->m_dwEVENT_MSG_TICK_COUNT = 0;
					this->m_dwCHECK_GATE_ALIVE_COUNT = 0;

					MsgOutput(iIndex, "Change of state success - the siege was registered");
				}
			}
			break;
		case 2:
			{
				this->SetState(CASTLESIEGE_STATE_IDLE_2, 1);
				MsgOutput(iIndex, "Change of state success - the siege registration statement");
			}
			break;
		case 3:
			{
				this->m_btIsSiegeGuildList = FALSE;
				this->SetState(CASTLESIEGE_STATE_IDLE_3, 1);
				MsgOutput(iIndex, "Change of state success - the siege declared");
			}
			break;
		case 4:
			{
				this->SetState(CASTLESIEGE_STATE_READYSIEGE, 1);
				MsgOutput(iIndex, "Change of state success - the siege begins");		
			}
			break;

		case 5:
			{
				if( this->m_iCastleSiegeState == CASTLESIEGE_STATE_STARTSIEGE )
				{
					this->m_iCS_REMAIN_MSEC = 0;
					MsgOutput(iIndex, "Change of state success - the siege end");
				}
				else
				{
					MsgOutput(iIndex, "Change of state failure - the siege end");
				}
			}
			break;
		case 6:
			{
				if( strcmp(this->m_szCastleOwnerGuild, "") == 0 )
				{
					MsgOutput(iIndex, "No current owner");
				}
				else
				{
					MsgOutput(iIndex, "Current Onwer Guild - %s", this->m_szCastleOwnerGuild);
				}
			}
			break;
		case 7:	/* Get Siege State Time Left */
			{
				if( this->m_bDoRun == FALSE )
				{
					MsgOutput(iIndex, "The current state of siege - Unknown");
				}
				else
				{
					switch( this->m_iCastleSiegeState )
					{
					case CASTLESIEGE_STATE_NONE:
						MsgOutput(iIndex, "The current state of siege - Unknown");
						break;
					case CASTLESIEGE_STATE_IDLE_1:
						MsgOutput(iIndex, "The current state of siege - Siege cycle before (not started)");
						break;
					case CASTLESIEGE_STATE_REGSIEGE:
						MsgOutput(iIndex, "The current state of siege - Siege of the registration period: %d Time: %d min: %d seconds remaining",
							((this->m_iCS_REMAIN_MSEC / 1000) % 3600), 
							((this->m_iCS_REMAIN_MSEC / 1000) % 3600) / 60, 
							((this->m_iCS_REMAIN_MSEC / 1000) % 3600) / 60);
						break;
					case CASTLESIEGE_STATE_IDLE_2:
						MsgOutput(iIndex, "The current state of siege - a siege after registration (rest period): %d Time: %d min: %d seconds remaining",
							((this->m_iCS_REMAIN_MSEC / 1000) % 3600), 
							((this->m_iCS_REMAIN_MSEC / 1000) % 3600) / 60,	
							((this->m_iCS_REMAIN_MSEC / 1000) % 3600) / 60);
						break;
					case CASTLESIEGE_STATE_REGMARK:
						MsgOutput(iIndex, "The current state of siege - a sentence the registration period: %d Time: %d min: %d seconds remaining",
							((this->m_iCS_REMAIN_MSEC / 1000) % 3600), 
							((this->m_iCS_REMAIN_MSEC / 1000) % 3600) / 60,	
							((this->m_iCS_REMAIN_MSEC / 1000) % 3600) / 60);
						break;
					case CASTLESIEGE_STATE_IDLE_3:
						MsgOutput(iIndex, "The current state of siege - after the registration statement (rest period): %d Time: %d min: %d seconds remaining",
							((this->m_iCS_REMAIN_MSEC / 1000) % 3600), 
							((this->m_iCS_REMAIN_MSEC / 1000) % 3600) / 60,	
							((this->m_iCS_REMAIN_MSEC / 1000) % 3600) / 60);
						break;
					case CASTLESIEGE_STATE_NOTIFY:
						MsgOutput(iIndex, "The current state of siege - Siege Guild Announces Participation Period: %d Time: %d min: %d seconds remaining",
							((this->m_iCS_REMAIN_MSEC / 1000) % 3600), 
							((this->m_iCS_REMAIN_MSEC / 1000) % 3600) / 60,	
							((this->m_iCS_REMAIN_MSEC / 1000) % 3600) / 60);
						break;
					case CASTLESIEGE_STATE_READYSIEGE:
						MsgOutput(iIndex, "The current state of siege - Siege Guild Announces Participation Period: %d Time: %d min: %d seconds remaining",
							((this->m_iCS_REMAIN_MSEC / 1000) % 3600), 
							((this->m_iCS_REMAIN_MSEC / 1000) % 3600) / 60,	
							((this->m_iCS_REMAIN_MSEC / 1000) % 3600) / 60);
						break;
					case CASTLESIEGE_STATE_STARTSIEGE:
						MsgOutput(iIndex, "The current state of siege - a siege of preparation time: %d Time: %d min: %d seconds remaining",
							((this->m_iCS_REMAIN_MSEC / 1000) % 3600), 
							((this->m_iCS_REMAIN_MSEC / 1000) % 3600) / 60,	
							((this->m_iCS_REMAIN_MSEC / 1000) % 3600) / 60);
						break;
					case CASTLESIEGE_STATE_ENDSIEGE:
						MsgOutput(iIndex, "The current state of siege - a siege during the war: %d Time: %d min: %d seconds remaining",
							((this->m_iCS_REMAIN_MSEC / 1000) % 3600), 
							((this->m_iCS_REMAIN_MSEC / 1000) % 3600) / 60,	
							((this->m_iCS_REMAIN_MSEC / 1000) % 3600) / 60);
						break;
					case CASTLESIEGE_STATE_ENDCYCLE:
						MsgOutput(iIndex, "The current state of siege - Unknown");
						break;
					default:
						MsgOutput(iIndex, "The current state of siege - Unknown");
						break;
					}
				}
			}
			break;
		case 8:
			{
				LPSTR szGuildName = (LPSTR)lpParam;

				if( this->m_iCastleSiegeState != CASTLESIEGE_STATE_STARTSIEGE )
				{
					MsgOutput(iIndex, "State failed attempt to replace ... the current state of siege - a siege cycle exit");
				}
				else if( szGuildName == NULL )
				{
					MsgOutput(iIndex, "State failed attempt to replace ... the current state of siege - a siege cycle exit");
				}
				else if( strcmp(szGuildName, "") == 0 )
				{
					MsgOutput(iIndex, "State failed attempt to replace ... the current state of siege - a siege cycle exit");
				}
				else
				{
					BYTE btCsJoinSide = 0;
					BYTE bCsGuildInvolved = false;
					int iResult = this->GetCsJoinSide(szGuildName, &btCsJoinSide, &bCsGuildInvolved);

					if( iResult == 1 )
					{
						if( btCsJoinSide > 1 && bCsGuildInvolved == 1 )
						{
							LogAddTD("[CastleSiege] Castle Winner Has been Changed - GM (GUILD:%s)",
								szGuildName);

							this->NotifyCrownState(2);

							memset(&this->m_szMiddleWinnerGuild, 0, sizeof(this->m_szMiddleWinnerGuild));
							memcpy(&this->m_szMiddleWinnerGuild, szGuildName, MAX_GUILD_LEN);

							this->ClearNonDbNPC_MidWin();
							this->ChangeWinnerGuild(btCsJoinSide);
							this->NotifyAllUserCsProgState(CASTLESIEGE_STATE_IDLE_1, szGuildName);
							this->ResetCrownAccessTickCount();
							this->ResetCrownUserIndex();
							this->SetCrownAccessUserX(0);
							this->SetCrownAccessUserY(0);
							this->ResetCrownSwitchUserIndex(217);
							this->ResetCrownSwitchUserIndex(218);

							this->m_bRegCrownAvailable = FALSE;
							this->ReSpawnEnemyUser(FALSE);

							GCAnsCsLeftTimeAlarm(((this->m_iCS_REMAIN_MSEC / 1000) % 3600), ((this->m_iCS_REMAIN_MSEC / 1000) % 3600) / 60);

							this->m_dwCS_LEFTTIME_TICK_COUNT = GetTickCount();

						}
						MsgOutput(iIndex, "State replacement success");
					}
				}
			}
			break;

		default:
			MsgOutput(iIndex, "State failed attempt to replace ... the current state of siege - a siege cycle exit");
			break;
	}
	}__finally
	{
	//DebugLog("%s END",__FUNCTION__);
	}
}




BYTE CCastleSiege::RegGuildScoreCompFunc(_CS_REG_GUILD_DATA & A, _CS_REG_GUILD_DATA & B)
{
	//__try
	//{
	////DebugLog("%s START",__FUNCTION__);
	//if(A.m_i64TotolScore < B.m_i64TotolScore)
	//{
	//	return false;
	//}
	//return true;
	//}__finally
	//{
	////DebugLog("%s END",__FUNCTION__);
	//}
	__try
	{
	//DebugLog("%s START",__FUNCTION__);
	if(A.m_i64TotolScore > B.m_i64TotolScore)
	{
		return true;
	}
	return false;
	}__finally
	{
	//DebugLog("%s END",__FUNCTION__);
	}
}

void CCastleSiege::ResetCastleCycle()
{
	__try
	{
		//DebugLog("%s START",__FUNCTION__);
		this->m_bCsBasicGuildInfoLoadOK = FALSE;
		this->m_bCsTotalGuildInfoLoadOK = FALSE;

		this->m_mapCsBasicGuildInfo.clear();
		this->m_mapCsTotalGuildInfo.clear();
	
		this->m_bCastleTowerAccessable = FALSE;
		this->m_bRegCrownAvailable = FALSE;
		this->m_btIsSiegeGuildList = 0;
		this->m_btIsSiegeEnded = 0;
	}__finally
	{
		//DebugLog("%s END",__FUNCTION__);
	}
}

void CCastleSiege::ResetCastleTaxInfo()
{
	__try
	{
		//DebugLog("%s START",__FUNCTION__);
		this->m_i64CastleMoney = 0;
		this->m_iTaxRateChaos = 0;
		this->m_iTaxRateStore = 0;
		this->m_iTaxHuntZone = 0;

		g_CastleSiegeSync.SetTaxRateChaos(this->m_iTaxRateChaos);
		g_CastleSiegeSync.SetTaxRateStore(this->m_iTaxRateStore);
		g_CastleSiegeSync.SetTaxHuntZone(this->m_iTaxHuntZone);

	}__finally
	{
		//DebugLog("%s END",__FUNCTION__);
	}
}

int CCastleSiege::UpgradeDbNPC_DFLEVEL(int iNpcType, int iNpcIndex, int iNpcDfLevel)
{
	//DebugLog("%s START",__FUNCTION__);
	BOOL bEXIST = FALSE;

	//EnterCriticalSection(&m_critNpcData);

	//_CS_NPC_DATA* it = this->m_vtNpcData.begin();
	std::vector<_CS_NPC_DATA>::iterator it = this->m_vtNpcData.begin();
	
	while( it != this->m_vtNpcData.end() )
	{
		std::vector<_CS_NPC_DATA>::iterator NpcData = it;
		//_CS_NPC_DATA & NpcData = _CS_NPC_DATA(*it);

		if( NpcData->m_iNPC_NUM == iNpcType && NpcData->m_iNPC_INDEX == iNpcIndex && NpcData->m_bIN_USE == 1 && NpcData->m_iNPC_LIVE > 0 )
		{
			int iObjIndex = NpcData->m_iNPC_OBJINDEX;

			if( gObjIsConnected(iObjIndex) && gObj[iObjIndex].m_btCsNpcType != 0 )
			{
				it->m_iNPC_DF_LEVEL = iNpcDfLevel;
				
				bEXIST = TRUE;
				if (NpcData->m_iNPC_NUM == 277)
				{
					gObj[iObjIndex].m_Defense = g_iNpcDefense_CGATE[iNpcDfLevel];
					gObj[iObjIndex].m_MagicDefense = g_iNpcDefense_CGATE[iNpcDfLevel];
					gObj[iObjIndex].m_btCsNpcDfLevel = iNpcDfLevel;
				}

				if (NpcData->m_iNPC_NUM == 283)
				{
					gObj[iObjIndex].m_Defense = g_iNpcDefense_CSTATUE[iNpcDfLevel];
					gObj[iObjIndex].m_MagicDefense = g_iNpcDefense_CSTATUE[iNpcDfLevel];
					gObj[iObjIndex].m_btCsNpcDfLevel = iNpcDfLevel;
				}

				break;
			}
		}
		it++;
	}

	//LeaveCriticalSection(&m_critNpcData);
	
	//DebugLog("%s END",__FUNCTION__);
	return bEXIST;
}

int CCastleSiege::UpgradeDbNPC_RGLEVEL(int iNpcType, int iNpcIndex, int iNpcRgLevel)
{
	//DebugLog("%s START",__FUNCTION__);
	BOOL bEXIST = FALSE;

	//EnterCriticalSection(&m_critNpcData);

	//_CS_NPC_DATA* it = this->m_vtNpcData.begin();
	std::vector<_CS_NPC_DATA>::iterator it = this->m_vtNpcData.begin();
	
	while( it != this->m_vtNpcData.end() )
	{
		std::vector<_CS_NPC_DATA>::iterator NpcData = it;
		//_CS_NPC_DATA & NpcData = _CS_NPC_DATA(*it);

		if( NpcData->m_iNPC_NUM == iNpcType && NpcData->m_iNPC_INDEX == iNpcIndex && NpcData->m_bIN_USE == 1 && NpcData->m_iNPC_LIVE > 0 )
		{
			int iObjIndex = NpcData->m_iNPC_OBJINDEX;

			if( gObjIsConnected(iObjIndex) && gObj[iObjIndex].m_btCsNpcType != 0 )
			{
				it->m_iNPC_RG_LEVEL = iNpcRgLevel;
				
				bEXIST = TRUE;
				
				gObj[iObjIndex].m_btCsNpcRgLevel = iNpcRgLevel;
				
				break;
			}
		}
		it++;
	}

	//LeaveCriticalSection(&m_critNpcData);
	
	//DebugLog("%s END",__FUNCTION__);
	return bEXIST;
}

int CCastleSiege::UpgradeDbNPC_MAXHP(int iNpcType, int iNpcIndex, int iNpcMaxHP)
{
	//DebugLog("%s START",__FUNCTION__);
	BOOL bEXIST = FALSE;

	//EnterCriticalSection(&m_critNpcData);

	//_CS_NPC_DATA* it = this->m_vtNpcData.begin();
	std::vector<_CS_NPC_DATA>::iterator it = this->m_vtNpcData.begin();
	
	while( it != this->m_vtNpcData.end() )
	{
		std::vector<_CS_NPC_DATA>::iterator NpcData = it;
		//_CS_NPC_DATA & NpcData = _CS_NPC_DATA(*it);

		if( NpcData->m_iNPC_NUM == iNpcType && NpcData->m_iNPC_INDEX == iNpcIndex && NpcData->m_bIN_USE == 1 && NpcData->m_iNPC_LIVE > 0 )
		{
			int iObjIndex = NpcData->m_iNPC_OBJINDEX;

			if( gObjIsConnected(iObjIndex) && gObj[iObjIndex].m_btCsNpcType != 0 )
			{
				it->m_iNPC_MAXHP = iNpcMaxHP;
				it->m_iNPC_HP = iNpcMaxHP;
				
				bEXIST = TRUE;
				
				gObj[iObjIndex].Life = iNpcMaxHP;
				gObj[iObjIndex].MaxLife = iNpcMaxHP;
				
				break;
			}
		}
		it++;
	}

	//LeaveCriticalSection(&m_critNpcData);
	
	//DebugLog("%s END",__FUNCTION__);
	return bEXIST;
}

struct PMSG_ANS_NOTIFYCROWNSTATE // 0x5
{
    PBMSG_HEAD2 h; // +0x0(0x4)
    BYTE btCrownState; // +0x4(0x1)
};

void CCastleSiege::NotifyCrownState(BYTE btState)
{
	__try
	{
	//DebugLog("%s START",__FUNCTION__);
	if( gObjIsConnected(m_iCrownIndex) == FALSE )
	{
		LogAddC(2, "[CastleSiege][Not Connected] CCastleSiege::NotifyCrownState() - m_iCrownIndex isn't Valid");
		return;
	}

	if( gObj[m_iCrownIndex].Class != 216 )
	{
		LogAddC(2, "[CastleSiege][NOT 216] CCastleSiege::NotifyCrownState() - m_iCrownIndex isn't Valid");
		return;
	}

	LPOBJ lpObj = &gObj[m_iCrownIndex];

	if( lpObj->VPCount2 < 1 )
	{
		return;
	}

	PMSG_ANS_NOTIFYCROWNSTATE pMsg;

	pMsg.h.set((LPBYTE)&pMsg, 0xB2, 0x16, sizeof(pMsg));
	pMsg.btCrownState = btState;

	int tObjNum = OBJ_EMPTY;

	for( int i = 0; i < MAX_VIEWPORT; i++ )
	{
		tObjNum = lpObj->VpPlayer2[i].number;

		if( tObjNum >= 0 )
		{
			if( gObj[tObjNum].Type == OBJ_USER && gObj[tObjNum].Live )
			{
				DataSend(tObjNum, (LPBYTE)&pMsg, pMsg.h.size);
			}
		}
	}

	LogAddTD("[CastleSiege] Crown State Changed (%d)", btState);
	}__finally
	{
		//DebugLog("%s END",__FUNCTION__);
	}
}



void CCastleSiege::GetStateDate(int iCastleSiegeState, SYSTEMTIME* st)
{
	//DebugLog("%s START",__FUNCTION__);
	//EnterCriticalSection(&m_critScheduleData);

	for( std::vector<_CS_SCHEDULE_DATA>::iterator it = m_vtScheduleData.begin(); it != m_vtScheduleData.end(); it++ )
	{
		std::vector<_CS_SCHEDULE_DATA>::iterator ScheduleData = it;

		if( ScheduleData->m_iSTATE == iCastleSiegeState )
		{
			st->wDay = ScheduleData->m_iADD_DAY;
			st->wHour = ScheduleData->m_iADD_HOUR;
			st->wMinute = ScheduleData->m_iADD_MIN;
			st->wSecond = 0;

			break;
		}
	}
	//LeaveCriticalSection(&m_critScheduleData);
	//DebugLog("%s END",__FUNCTION__);
}

#endif