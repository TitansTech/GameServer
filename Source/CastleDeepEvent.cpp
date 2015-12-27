// CastleDeepEvent.cpp: implementation of the CCastleDeepEvent class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GameServer.h"
#include "GameMain.h"
#include "CastleDeepEvent.h"
#include "CastleSiegeSync.h"
#include "LogProc.h"
#include "..\include\readscript.h"
#include "..\common\winutil.h"

#if (GS_CASTLE==1)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CCastleDeepEvent g_CastleDeepEvent;

#define CASTLE_MAX_MONSTER OBJ_MAXMONSTER


CCastleDeepEvent::CCastleDeepEvent()
{
	this->m_bDoEvent = FALSE;
	this->m_bHasData = FALSE;

	srand(time(0)); // Seed random
	
	InitializeCriticalSection(&this->m_critEventData);
}
CCastleDeepEvent::~CCastleDeepEvent()
{
	// Empty
}
void CCastleDeepEvent::StartEvent()
{
	if( this->m_vtAssultType.size() < 0 )
	{
		LogAddTD("[CastleDeep Event] Start Fail - No Assult Data");
		this->SetState(0);
		return;
	}

	if ( g_CastleSiegeSync.GetCastleState() == 7 )
	{
		LogAddTD("[CastleDeep Event] Start Fail - Castle Siege in progress!");
		this->SetState(CASTLE_STATE_CLOSED);
		return;
	}

	int iCheckCount = 100;
	int iASSULT_RATE = 0;
	BOOL bDoStart = FALSE;

	while( (iCheckCount--) > 0 )
	{
		int iRandomAssult = rand() % this->m_vtAssultType.size();

		this->m_iCUR_ASSULT_TYPE = this->m_vtAssultType[iRandomAssult];
		iASSULT_RATE = this->m_vtAssultTypeRate[iRandomAssult];

		if( (rand()%100) < iASSULT_RATE )
		{
			LogAddTD("[CastleDeep Event] Assault Chosen - GROUP: %d RATE : %d",this->m_iCUR_ASSULT_TYPE,iASSULT_RATE);
			bDoStart = TRUE;
			break;
		}
	}
	if( bDoStart == FALSE )
	{
		LogAddTD("[CastleDeep Event] Start Fail - bDoStart == FALSE");
		this->SetState(0);
		return;
	}

	std::map<int, std::vector<CASTLEDEEP_SPAWNTIME> >::iterator it;

	it = this->m_mapSpawnTime.find(this->m_iCUR_ASSULT_TYPE);

	if( it == this->m_mapSpawnTime.end() )
	{
		LogAddTD("[CastleDeep Event] Start Fail - it == this->m_mapSpawnTime.end()");
		this->SetState(0);
		return;
	}
	this->m_mapRunCheck.clear();

	for( std::vector<CASTLEDEEP_SPAWNTIME>::iterator it2 = it->second.begin(); it2 != it->second.end(); it2++ )
	{
		//CASTLEDEEP_SPAWNTIME * lpSpawnTime = it2;
		std::vector<CASTLEDEEP_SPAWNTIME>::iterator lpSpawnTime = it2;
		
		this->m_mapRunCheck.insert( std::pair<int,int>(lpSpawnTime->m_iMonsterGroup, lpSpawnTime->m_iMonsterSpawnMinute) );
	}  
}

void CCastleDeepEvent::StopEvent()
{
	// Empty
}

void CCastleDeepEvent::EnableEvent(BOOL bDoEvent) {
	this->m_bDoEvent = bDoEvent;

	if( this->m_bDoEvent ) {
		this->SetState(CASTLE_STATE_CLOSED);
	} else {
		this->SetState(CASTLE_STATE_NONE);
	}
}

BOOL CCastleDeepEvent::Load(char * lpszFileName)
{	
	//DebugLog("%s START",__FUNCTION__);
	if( lpszFileName == NULL ) return FALSE;
		
	SMDFile = fopen(lpszFileName, "r");
	
	if ( SMDFile == FALSE )
	{
		MsgBox("[CastleDeep Event] Info file Load Fail [%s]", lpszFileName);
		
		//DebugLog("%s END",__FUNCTION__);
		return FALSE;
	}
	EnterCriticalSection(&this->m_critEventData);

	this->m_vtAssultType.clear();
	this->m_vtAssultTypeRate.clear();
	this->m_mapSpawnTime.clear();
	this->m_mapMonsterInfo.clear();
	this->m_vtEventTime.clear();
	this->m_vtAssultTypeRate.clear();
	this->m_mapRunCheck.clear();

	LeaveCriticalSection(&this->m_critEventData);

	this->m_bHasData = FALSE;
	int type = -1;
	SMDToken Token;

	while( true )
	{
		Token = GetToken();
		if( Token == 2 )
		{
			break;
		}

		if( Token == NUMBER )
		{
			while(true)
			{
				//Token = GetToken();
				type = TokenNumber;

				if( type == 0 )
				{
					while(true)
					{
						Token = GetToken();
					
						if( strcmp("end", TokenString) == 0 ) 
						{
							type++;
							break;
						}
					
						CASTLEDEEP_EVENTTIME m_EventTime;

						m_EventTime.m_iHour = TokenNumber; 
						Token = GetToken();
						m_EventTime.m_iMinute = TokenNumber;

						this->m_vtEventTime.push_back(m_EventTime);
					}
				}
				else if( type == 1 )
				{
					while(true)
					{
						std::vector<CASTLEDEEP_MONSTERINFO> vecMonsterInfo;
						std::vector<CASTLEDEEP_SPAWNTIME> vecEventSpawnTime;
						Token = GetToken();

						if( strcmp("end", TokenString) == 0 ) 
						{
							vecMonsterInfo.clear();
							vecEventSpawnTime.clear();
							type++;
							break;
						}
						CASTLEDEEP_SPAWNTIME	m_SpawnTime;

						m_SpawnTime.m_iMonsterGroup = TokenNumber;
						
						Token = GetToken();
						m_SpawnTime.m_iMonsterSpawnMinute = TokenNumber;

						this->m_mapMonsterInfo.insert( std::pair<int, std::vector<CASTLEDEEP_MONSTERINFO> >(m_SpawnTime.m_iMonsterGroup,vecMonsterInfo) );
						this->m_mapSpawnTime.insert( std::pair<int, std::vector<CASTLEDEEP_SPAWNTIME> >(m_SpawnTime.m_iMonsterGroup, vecEventSpawnTime ) );
					
						this->m_vtAssultType.push_back(m_SpawnTime.m_iMonsterGroup);
						this->m_vtAssultTypeRate.push_back(m_SpawnTime.m_iMonsterSpawnMinute);
					}
				}
				else if(type == 2)
				{
					while(true)
					{
						Token = GetToken();
						if( strcmp("end", TokenString) == 0 ) 
						{
							type++;
							break;
						}

						CASTLEDEEP_SPAWNTIME	m_SpawnTime;
						int m_iSpawnTimeIndex;

						m_iSpawnTimeIndex = TokenNumber;

						Token = GetToken();
						m_SpawnTime.m_iMonsterGroup = TokenNumber;

						Token = GetToken();
						m_SpawnTime.m_iMonsterSpawnMinute = TokenNumber;

						EnterCriticalSection(&this->m_critEventData);

						std::map<int, std::vector<CASTLEDEEP_SPAWNTIME> >::iterator it = this->m_mapSpawnTime.find(m_iSpawnTimeIndex);

						if( it != this->m_mapSpawnTime.end() )
						{
							it->second.push_back(m_SpawnTime);
						}
					
						LeaveCriticalSection(&this->m_critEventData);
					}
				}
				else if(type == 3)
				{
					while(true)
					{
						Token = GetToken();
						if( strcmp("end", TokenString) == 0 ) 
						{
							break;
						}

						CASTLEDEEP_MONSTERINFO	m_MonsterInfo;
						int m_iMonsterInfoIndex;

						m_iMonsterInfoIndex = TokenNumber;

						Token = GetToken();
						m_MonsterInfo.m_iMonsterGroup = TokenNumber;

						Token = GetToken();
						m_MonsterInfo.m_iMonsterType = TokenNumber;

						Token = GetToken();
						m_MonsterInfo.m_iMonsterNumber = TokenNumber;

						Token = GetToken();
						m_MonsterInfo.m_iSX = TokenNumber;
						
						Token = GetToken();
						m_MonsterInfo.m_iSY = TokenNumber;

						Token = GetToken();
						m_MonsterInfo.m_iDX = TokenNumber;

						Token = GetToken();
						m_MonsterInfo.m_iDY = TokenNumber;

						EnterCriticalSection(&this->m_critEventData);

						std::map<int, std::vector<CASTLEDEEP_MONSTERINFO> >::iterator it = this->m_mapMonsterInfo.find(m_iMonsterInfoIndex);
						if( it != this->m_mapMonsterInfo.end() )
						{
							it->second.push_back(m_MonsterInfo);
						}
					
						LeaveCriticalSection(&this->m_critEventData);
					}
				}
				break;
			}
		}
	}

	fclose(SMDFile);

	LogAdd("%s file load!",lpszFileName);

	this->m_bHasData = TRUE;

	//DebugLog("%s END",__FUNCTION__);
	return TRUE;
}

void CCastleDeepEvent::CheckSync()
{
	if( m_vtEventTime.empty() )
	{
		LogAddC(2, "[CastleDeep Event] No Schedule Time Data");
		this->SetState(CASTLE_STATE_NONE);
		return;
	}
	
	SYSTEMTIME sysTime;

	GetLocalTime(&sysTime);

	int iMIN_HOUR = 24;
	int iMIN_MINUTE = 60;
	int bTIME_CHANGED = 0;

	//CASTLEDEEP_EVENTTIME *it;
	std::vector<CASTLEDEEP_EVENTTIME>::iterator it;

	for( it = this->m_vtEventTime.begin(); it != this->m_vtEventTime.end(); it++ )
	{
		//CASTLEDEEP_EVENTTIME *pEventTime = it;
		std::vector<CASTLEDEEP_EVENTTIME>::iterator pEventTime = it;

		if( ((sysTime.wHour*60)+sysTime.wMinute) < ((pEventTime->m_iHour*60)+pEventTime->m_iMinute) )
		{
			if( ((iMIN_HOUR*60)+iMIN_MINUTE) > ((pEventTime->m_iHour*60)+pEventTime->m_iMinute) )
			{
				bTIME_CHANGED = 1;
				iMIN_HOUR = pEventTime->m_iHour;
				iMIN_MINUTE = pEventTime->m_iMinute;
			}
		}
	}
	if( bTIME_CHANGED == 0 )
	{
		it = this->m_vtEventTime.begin();

		iMIN_HOUR = 24;
		iMIN_MINUTE = 60;

		for( ; it != this->m_vtEventTime.end(); it++ )
		{
			//CASTLEDEEP_EVENTTIME *pEventTime = it;
			std::vector<CASTLEDEEP_EVENTTIME>::iterator pEventTime = it;

			if( ((iMIN_HOUR*60)+iMIN_MINUTE) > ((pEventTime->m_iHour*60)+pEventTime->m_iMinute) )
			{
				bTIME_CHANGED = 2;
				iMIN_HOUR = pEventTime->m_iHour;
				iMIN_MINUTE = pEventTime->m_iMinute;
			}
		}
	}
	switch( bTIME_CHANGED )
	{
	case 1:
		this->m_iTIME_MSEC_REMAIN = ( ((iMIN_HOUR*60*60)+(iMIN_MINUTE*60)) - ( (sysTime.wHour*60*60)+(sysTime.wMinute*60)+sysTime.wSecond) ) * 1000;
		break;
	case 2:
		this->m_iTIME_MSEC_REMAIN = ( (((iMIN_HOUR+24)*60*60)+(iMIN_MINUTE*60)) - ( (sysTime.wHour*60*60)+(sysTime.wMinute*60)+sysTime.wSecond) ) * 1000;
		break;
	default:
		LogAddC(2, "[CastleDeep Event] No Schedule Time Data");
		this->SetState(CASTLE_STATE_NONE);
		return;
		break;
	}
	this->m_iTIME_TICK_COUNT = GetTickCount();
	LogAddTD("[CastleDeep Event] Sync Start Time. [%d] min remain (START HOUR:%d, MIN:%d)",	(this->m_iTIME_MSEC_REMAIN%60000), iMIN_HOUR, iMIN_MINUTE);
	this->m_iTIME_NOTIFY_COUNT = 5;

}
void CCastleDeepEvent::SetState(int iSTATE)
{
	if( (this->m_iEVENT_STATE < 0) || (this->m_iEVENT_STATE > 2) )	return;
	
	this->m_iEVENT_STATE = iSTATE;

	switch( this->m_iEVENT_STATE )
	{
		case CASTLE_STATE_NONE:
			this->SetState_None();
		break;
		case CASTLE_STATE_CLOSED:
			this->SetState_Closed();
		break;
		case CASTLE_STATE_PLAYING:
			this->SetState_Playing();
		break;
	}
}

void CCastleDeepEvent::SetState_None() {
	this->StopEvent();
}

void CCastleDeepEvent::SetState_Closed() {
	this->StopEvent();

	if( this->m_bDoEvent ) {
		this->CheckSync();
	} else {
		this->SetState(CASTLE_STATE_NONE);
	}
}

void CCastleDeepEvent::SetState_Playing() {
	this->m_iTIME_MSEC_REMAIN = 0;
	this->m_bCheckEnd = FALSE;
	this->m_uCheckEndTick = 0;

	this->ClearMonster();
	this->StartEvent();
}
void CCastleDeepEvent::Run() 
{
	if( this->m_bDoEvent ) 
	{
		switch( this->m_iEVENT_STATE )
		{
		case CASTLE_STATE_NONE:
			this->ProcState_None();
			break;
		case CASTLE_STATE_CLOSED:
			this->ProcState_Closed();
			break;
		case CASTLE_STATE_PLAYING:
			this->ProcState_Playing();
			break;
		}
	}
}
void CCastleDeepEvent::ProcState_None() 
{
	// Empty function
}


void CCastleDeepEvent::ProcState_Closed()  //004685C0
{
	int iTICK_MSEC = ( GetTickCount() - this->m_iTIME_TICK_COUNT );

	if( iTICK_MSEC >= 1000 )
	{
		this->m_iTIME_MSEC_REMAIN -= iTICK_MSEC;
		this->m_iTIME_TICK_COUNT = GetTickCount();

		if( this->m_bDoEvent )
		{
			if( (this->m_iTIME_MSEC_REMAIN <= 300000) && (this->m_iTIME_MSEC_REMAIN >= 0) )
			{
				if( (this->m_iTIME_MSEC_REMAIN/60000) != this->m_iTIME_NOTIFY_COUNT )
				{
					this->m_iTIME_NOTIFY_COUNT = ( this->m_iTIME_MSEC_REMAIN / 60000 );

					if( (this->m_iTIME_NOTIFY_COUNT+1) <= 5 )
					{
						if ( g_CastleSiegeSync.GetCastleState() != 7 )
						{
							char szMsg[256];
							wsprintf(szMsg, "[CastleDeep] The event will start in %d mintes.",(this->m_iTIME_NOTIFY_COUNT+1));
							g_CastleSiege.SendMapServerGroupMsg(szMsg);
						}
					}
				}
			}
		}
		//LogAddC(3, "[CastleDeep Event] ProcState_Closed()\tLEFT-MSEC:%d", this->m_iTIME_MSEC_REMAIN);
	}
	if( this->m_iTIME_MSEC_REMAIN <= 0 )
	{
		if( this->m_bDoEvent )
		{
			if ( g_CastleSiegeSync.GetCastleState() != 7 )
			{
				char szMsg[256];
				wsprintf(szMsg, "[CastleDeep] The event started.");
				g_CastleSiege.SendMapServerGroupMsg(szMsg);
				LogAddTD("[CastleDeep Event] - Event Started");

				this->SetState(CASTLE_STATE_PLAYING);
			} else {
				this->SetState(CASTLE_STATE_CLOSED);
			}
		}
		else
		{
			this->SetState(CASTLE_STATE_NONE);
		}
	}
}

void CCastleDeepEvent::ProcState_Playing() 
{
	int iTICK_MSEC = ( GetTickCount() - this->m_iTIME_TICK_COUNT );

	if( iTICK_MSEC >= 1000 )
	{
		this->m_iTIME_MSEC_REMAIN += iTICK_MSEC;
		this->m_iTIME_TICK_COUNT = GetTickCount();

		for( std::map<int, int>::iterator it = this->m_mapRunCheck.begin(); it != this->m_mapRunCheck.end(); it++)
		{
			int iMapTime = it->second;

			//DebugLog("%s Info: iMapTime:%d, m_mapRunCheck:%d, m_iTIME_MSEC_REMAIN:%d, m_iCUR_ASSULT_TYPE:%d",
			//	__FUNCTION__,iMapTime,this->m_mapRunCheck.size(),this->m_iTIME_MSEC_REMAIN,this->m_iCUR_ASSULT_TYPE
			//);

			if( iMapTime == (this->m_iTIME_MSEC_REMAIN / 60000) )
			{
				this->AddMonster(this->m_iCUR_ASSULT_TYPE,it->first);
				this->m_mapRunCheck.erase(it);
				break;
			}
		}
		
		//LogAddC(3, "[CastleDeep Event] ProcState_Playing()\tLEFT-MSEC:%d", this->m_iTIME_MSEC_REMAIN);

		if( this->m_mapRunCheck.size() <= 0 )
		{
			if( this->m_bCheckEnd == FALSE )
			{
				this->m_bCheckEnd = TRUE;
				this->m_uCheckEndTick = GetTickCount();
				LogAddTD("[CastleDeep Event] - Preparing to END Event");
			}
			else
			{
				if( GetTickCount() > (this->m_uCheckEndTick + 600000) )
				{
					char szMsg[256];
					//wsprintf(szMsg, "3.");
					wsprintf(szMsg, "[CastleDeep] The event finished.");
					AllSendServerMsg(szMsg);

					if( this->m_bDoEvent )
					{
						this->SetState(CASTLE_STATE_CLOSED);
					} 
					else 
					{
						this->SetState(CASTLE_STATE_NONE);
					}
					LogAddTD("[CastleDeep Event] - Event Ended");
					this->ClearMonster();
				}
			}
		}
	}
}



void CCastleDeepEvent::ClearMonster() 
{
	for( int iIndex = 0; iIndex < CASTLE_MAX_MONSTER; iIndex++ )
	{
		if( gObjIsConnected(iIndex) )
		{
			if( gObj[iIndex].m_Attribute == 62 )
				gObjDel(iIndex);
		}
	}
}

void CCastleDeepEvent::AddMonster(int iAssultType, int iGroup) 
{
	std::map<int, std::vector<CASTLEDEEP_MONSTERINFO> >::iterator it = this->m_mapMonsterInfo.find(iAssultType);

	if( it == this->m_mapMonsterInfo.end() ) 
		return;

	//for( CASTLEDEEP_MONSTERINFO *it2 = it->second.begin(); it2 != it->second.end(); it2++ )
	for( std::vector<CASTLEDEEP_MONSTERINFO>::iterator it2 = it->second.begin(); it2 != it->second.end(); it2++ )
	{
		//CASTLEDEEP_MONSTERINFO *pMonsterInfo = it2;
		std::vector<CASTLEDEEP_MONSTERINFO>::iterator pMonsterInfo = it2;

		if( pMonsterInfo->m_iMonsterGroup != iGroup )
			continue;

		LogAddTD("[CastleDeep Event][AT:%d,GR:%d] - Adding Monsters ID: %d / QTY: %d",iAssultType,iGroup,pMonsterInfo->m_iMonsterType,pMonsterInfo->m_iMonsterNumber);
		for( int i = 0; i < pMonsterInfo->m_iMonsterNumber; i++ )
		{
			BYTE cX, cY;

			if( ::gObjGetRandomFreeArea(MAP_INDEX_CASTLESIEGE, cX, cY, pMonsterInfo->m_iSX, pMonsterInfo->m_iSY, pMonsterInfo->m_iDX, pMonsterInfo->m_iDY, 50) == TRUE )
			{
				int iMobIndex = gObjAddMonster(MAP_INDEX_CASTLESIEGE);

				if( iMobIndex >= 0 )
				{
					gObj[iMobIndex].m_PosNum = -1;
					gObj[iMobIndex].X = cX;
					gObj[iMobIndex].Y = cY;
					gObj[iMobIndex].MapNumber = MAP_INDEX_CASTLESIEGE;
					gObj[iMobIndex].TX = gObj[iMobIndex].X;
					gObj[iMobIndex].TY = gObj[iMobIndex].Y;
					gObj[iMobIndex].m_OldX = gObj[iMobIndex].X;
					gObj[iMobIndex].m_OldY = gObj[iMobIndex].Y;
					gObj[iMobIndex].Dir = 1;
					gObj[iMobIndex].StartX = gObj[iMobIndex].X;
					gObj[iMobIndex].StartY = gObj[iMobIndex].Y;

					gObjSetMonster(iMobIndex, pMonsterInfo->m_iMonsterType,"CCastleDeepEvent::AddMonster");

					gObj[iMobIndex].m_Attribute = 62;
					gObj[iMobIndex].Dir = ( rand() % 8 );
					gObj[iMobIndex].DieRegen = 0;
					gObj[iMobIndex].RegenTime = 1;
					gObj[iMobIndex].MaxRegenTime = 1000;
					gObj[iMobIndex].m_dwLastCheckTick = GetTickCount();

					gObj[iMobIndex].m_MoveRange = 5;
					gObj[iMobIndex].m_ViewRange = 15;
				}
			}
		}
	}
}

#endif
