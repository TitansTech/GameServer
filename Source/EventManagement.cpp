#include "stdafx.h"
#include "EventManagement.h"
#include "LogProc.h"
#include "DragonEvent.h"
#include "AttackEvent.h"
#include "EledoradoEvent.h"
#include "RingAttackEvent.h"
#include "..\include\readscript.h"
#include "BlueEvent.h"
#include "SkyEvent.h"
#include "BossAttack.h"
#include "Raklion.h"
#include "HitAndUp.h"
#include "HappyHour.h"
#include "MossShop.h"
#include "XMasEvent.h"
#include "GreenEvent.h"
#include "SwampEvent.h"
#include "SummerEvent.h"
#include "HalloweenEvent.h"
#include "SCFBotHidenSeek.h"
#include "GensCloseMap.h"
#include "RainItemEvent.h"
#include "ObjBotRacer.h"
#include "ObjTitanRank.h"
#include "ObjTitanLottery.h"
#include "ObjQaA.h"


// GS-N 0.99.60T 0x00464EC0
//	GS-M	1.00.18	JPN	0x00477D60	-	Completed

CEventManagement g_EventManager;



CEventManagement::CEventManagement()
{
	this->m_bEventStart = false;
	this->m_bHasData = false;
	this->Clear();
}



CEventManagement::~CEventManagement()
{
	return;
}



void CEventManagement::Clear()
{
	this->m_vtEventTime.clear();
	this->m_mapEventObj.clear();
	this->m_wToday_Year = 0;
	this->m_wToday_Month = 0;
	this->m_wToday_Day = 0;
}




void CEventManagement::Init(BOOL bEventStart)
{
	this->m_bEventStart = bEventStart;

	if ( this->m_bHasData != false && this->m_bEventStart != false )
	{
		SYSTEMTIME sysTime;

		GetLocalTime(&sysTime);
		this->m_wToday_Year = sysTime.wYear;
		this->m_wToday_Month = sysTime.wMonth;
		this->m_wToday_Day = sysTime.wDay;

		std::map<int, void *>::iterator _Itor;
		
		_Itor = this->m_mapEventObj.find(EVENT_ID_DRAGONHERD);

		if ( _Itor != this->m_mapEventObj.end() )
		{
			CDragonEvent * lpEvent = (CDragonEvent *)_Itor->second;
			lpEvent->SetMenualStart(FALSE);

			for (std::vector<EVENT_ID_TIME>::iterator it = this->m_vtEventTime.begin() ; it != this->m_vtEventTime.end(); it++)
			{
				if ( (*(it)).m_eEventKind == EVENT_ID_DRAGONHERD )
				{
					lpEvent->SetMenualStart(TRUE);
				}
			}
		}

		_Itor = this->m_mapEventObj.find(EVENT_ID_UNDERTROOP);

		if ( _Itor != this->m_mapEventObj.end() )
		{
			CAttackEvent * lpEvent = (CAttackEvent *)_Itor->second;
			lpEvent->SetMenualStart(FALSE);

			for (std::vector<EVENT_ID_TIME>::iterator it = this->m_vtEventTime.begin() ; it != this->m_vtEventTime.end(); it++)
			{
				if ( (*(it)).m_eEventKind == EVENT_ID_UNDERTROOP )
				{
					lpEvent->SetMenualStart(TRUE);
				}
			}
		}

		_Itor = this->m_mapEventObj.find(EVENT_ID_ELDORADO);

		if ( _Itor != this->m_mapEventObj.end() )
		{
			CEledoradoEvent * lpEvent = (CEledoradoEvent *)_Itor->second;
			lpEvent->SetMenualStart(FALSE);

			for (std::vector<EVENT_ID_TIME>::iterator it = this->m_vtEventTime.begin() ; it != this->m_vtEventTime.end(); it++)
			{
				if ( (*(it)).m_eEventKind == EVENT_ID_ELDORADO )
				{
					lpEvent->SetMenualStart(TRUE);
				}
			}
		}


		_Itor = this->m_mapEventObj.find(EVENT_ID_WHITEMAGE);

		if ( _Itor != this->m_mapEventObj.end() )
		{
			CRingAttackEvent * lpEvent = (CRingAttackEvent *)_Itor->second;
			lpEvent->SetMenualStart(FALSE);

			for (std::vector<EVENT_ID_TIME>::iterator it = this->m_vtEventTime.begin() ; it != this->m_vtEventTime.end(); it++)
			{
				if ( (*(it)).m_eEventKind == EVENT_ID_ELDORADO )	// #error Deathway Fix Here (GS says ELEDORADO
				{
					lpEvent->SetMenualStart(TRUE);
				}
			}
		}

	}
	else
	{
		std::map<int, void *>::iterator _Itor;
		
		_Itor = this->m_mapEventObj.find(EVENT_ID_DRAGONHERD);
		
		if ( _Itor != this->m_mapEventObj.end() )
		{
			CDragonEvent * lpEvent = (CDragonEvent *)_Itor->second;
			lpEvent->SetMenualStart(FALSE);
		}

	
		_Itor = this->m_mapEventObj.find(EVENT_ID_UNDERTROOP);
		
		if ( _Itor != this->m_mapEventObj.end() )
		{
			CAttackEvent * lpEvent = (CAttackEvent *)_Itor->second;
			lpEvent->SetMenualStart(FALSE);
		}

		_Itor = this->m_mapEventObj.find(EVENT_ID_ELDORADO);
		
		if ( _Itor != this->m_mapEventObj.end() )
		{
			CEledoradoEvent * lpEvent = (CEledoradoEvent *)_Itor->second;
			lpEvent->SetMenualStart(FALSE);
		}

		_Itor = this->m_mapEventObj.find(EVENT_ID_WHITEMAGE);
		
		if ( _Itor != this->m_mapEventObj.end() )
		{
			CRingAttackEvent * lpEvent = (CRingAttackEvent *)_Itor->second;
			lpEvent->SetMenualStart(FALSE);
		}
	}
}



void CEventManagement::Run()
{
	if ( this->m_bEventStart == false || this->m_bHasData == false || this->m_vtEventTime.empty() != FALSE )
	{
		return;
	}

	SYSTEMTIME sysTime;
	std::vector<EVENT_ID_TIME>::iterator it;

	GetLocalTime(&sysTime);

	if ( this->m_wToday_Year != sysTime.wYear || this->m_wToday_Month != sysTime.wMonth || this->m_wToday_Day != sysTime.wDay )
	{
		for ( it = this->m_vtEventTime.begin() ; it != this->m_vtEventTime.end() ; it++ )
		{
			(*(it)).m_bEventStarted = false;
		}

		LogAddTD("[Event Management] RUN() Year-Month-Day: %02d %02d %02d", this->m_wToday_Year, this->m_wToday_Month, this->m_wToday_Day);	// Deathway Need Translation

		this->m_wToday_Year = sysTime.wYear;
		this->m_wToday_Month = sysTime.wMonth;
		this->m_wToday_Day = sysTime.wDay;
	}

	for ( it = this->m_vtEventTime.begin() ; it != this->m_vtEventTime.end() ; it++ )
	{
		if ( sysTime.wHour == (*(it)).m_iHour && sysTime.wMinute == (*(it)).m_iMinute && (*(it)).m_bEventStarted == false )
		{
			LogAddTD("[Event Management] Starting Event. Kind(%d) %02d %02d (state=%d)",
				(*(it)).m_eEventKind , (*(it)).m_iHour, sysTime.wMinute , (*(it)).m_bEventStarted);

			(*(it)).m_bEventStarted = true;
			this->StartEvent((*(it)).m_eEventKind);
		}
	}
}






void CEventManagement::StartEvent(int eEventKind)
{
#if(GS_CASTLE_NOEVENTS == 0)
	switch ( eEventKind )
	{
		case EVENT_ID_DRAGONHERD:
			{
				std::map<int, void *>::iterator _Itor = this->m_mapEventObj.find(EVENT_ID_DRAGONHERD);

				if ( _Itor != this->m_mapEventObj.end() )
				{
					CDragonEvent * lpEvent = (CDragonEvent *)_Itor->second;
					lpEvent->Start_Menual();
				}
			}
			break;

		case EVENT_ID_UNDERTROOP:
			{
				std::map<int, void *>::iterator _Itor = this->m_mapEventObj.find(EVENT_ID_UNDERTROOP);

				if ( _Itor != this->m_mapEventObj.end() )
				{
					CAttackEvent * lpEvent = (CAttackEvent *)_Itor->second;
					lpEvent->Start_Menual();
				}
			}
			break;

		case EVENT_ID_ELDORADO:
			{
				std::map<int, void *>::iterator _Itor = this->m_mapEventObj.find(EVENT_ID_ELDORADO);

				if ( _Itor != this->m_mapEventObj.end() )
				{
					CEledoradoEvent * lpEvent = (CEledoradoEvent *)_Itor->second;
					lpEvent->Start_Menual();
				}
			}
			break;

		case EVENT_ID_WHITEMAGE:
			{
				std::map<int, void *>::iterator _Itor = this->m_mapEventObj.find(EVENT_ID_WHITEMAGE);

				if ( _Itor != this->m_mapEventObj.end() )
				{
					CRingAttackEvent * lpEvent = (CRingAttackEvent *)_Itor->second;
					lpEvent->Start_Menual();
				}
			}
			break;
	}
#endif
}






bool CEventManagement::Load(LPSTR lpszFileName)
{
	this->m_bHasData = false;
	this->Clear();

	SMDFile = fopen(lpszFileName, "r");

	if ( SMDFile == NULL )
	{
		MsgBox("[DOTH] Info file Load Fail [%s]", lpszFileName);

		return false;
	}

	int Token;
	int type = -1;

	this->SCFEventCount = 0;

	while ( true )
	{
		Token = GetToken();

		if ( Token == 2 )
		{
			break;
		}

		type = TokenNumber;

		while ( true )
		{
			if ( type == 0 )
			{
				Token = GetToken();

				if ( strcmp("end", TokenString) == 0 )
				{
					break;
				}

				EVENT_ID_TIME EIT;

				EIT.m_eEventKind = TokenNumber;

				Token = GetToken();
				EIT.m_iHour = TokenNumber;

				Token = GetToken();
				EIT.m_iMinute = TokenNumber;

				EIT.m_bEventStarted = false;

				this->m_vtEventTime.push_back(EIT);
			}
			else if ( type == 1 )
			{
				Token = GetToken();

				if ( strcmp("end", TokenString) == 0 )
				{
					break;
				}

				this->SCFEvent[this->SCFEventCount].m_eEventKind = TokenNumber;

				Token = GetToken();
				this->SCFEvent[this->SCFEventCount].m_iMonth = TokenNumber;

				Token = GetToken();
				this->SCFEvent[this->SCFEventCount].m_iDay = TokenNumber;

				Token = GetToken();
				this->SCFEvent[this->SCFEventCount].m_iDayOfWeek = TokenNumber;

				Token = GetToken();
				this->SCFEvent[this->SCFEventCount].m_iHour = TokenNumber;

				Token = GetToken();
				this->SCFEvent[this->SCFEventCount].m_iMinute = TokenNumber;

				this->SCFEventCount++;
			}
		}
	}

	fclose(SMDFile);
	LogAdd("[DOTH] %s file load!", lpszFileName);

	this->m_bHasData = true;

	return true;
}


void CEventManagement::RegEvent(int eEventKind, void * lpEventObj)
{
	this->m_mapEventObj.insert( std::pair<int, void *>(eEventKind, lpEventObj) );
}

//----------------

void CEventManagement::SCFInit(BOOL Start)
{
	if(this->SCFEventManagerRunning == 0)
		_beginthread( CEventManagement__SCFEventManager, 0, NULL  );
}

void CEventManagement__SCFEventManager(void * lpParam)
{
	g_EventManager.SCFEventManagerRunning = 1;
	while (g_bSCFEventManagerOn)
	{
		Sleep(60000);

		if(g_bSCFEventManagerOn == FALSE)
			break;

#if (PACK_EDITION>=3)
		BotRacer.CheckStatus();
#endif

		for (int i=0; i<g_EventManager.SCFEventCount; i++)
		{
			if(g_EventManager.SCFEvent[i].m_iHour == 24) g_EventManager.SCFEvent[i].m_iHour = 0;
			SYSTEMTIME time;
			 
			GetLocalTime(&time);
			DWORD dwEventNowTime = MAKELONG(time.wMinute, time.wHour);
			DWORD dwEventTime = MAKELONG(g_EventManager.SCFEvent[i].m_iMinute, g_EventManager.SCFEvent[i].m_iHour);
			if(dwEventNowTime == dwEventTime)
			{
				if( ( ((g_EventManager.SCFEvent[i].m_iDay == -1) && (g_EventManager.SCFEvent[i].m_iMonth == -1)) && ((g_EventManager.SCFEvent[i].m_iDayOfWeek == -1) || (g_EventManager.SCFEvent[i].m_iDayOfWeek == time.wDayOfWeek)) ) || ( ((g_EventManager.SCFEvent[i].m_iDay == time.wDay) && (g_EventManager.SCFEvent[i].m_iMonth == time.wMonth)) || (g_EventManager.SCFEvent[i].m_iDay == time.wDay) ) )
				{
					switch (g_EventManager.SCFEvent[i].m_eEventKind)
					{
#if(GS_CASTLE_NOEVENTS == 0)
	#if (PACK_EDITION>=1)
						case 0:
						{
							BlueEvent.StartEvent();
						}break;
	#endif
	#if (PACK_EDITION>=2)
						case 1:
						{
							SkyEvent.StartEvent();						
						}break;
	#endif
	#if (PACK_EDITION>=3)
						case 2:
						{
							BossAttack.StartEvent();							
						}break;
	#endif
	#if (PACK_EDITION>=1)
						case 3:
						{
							HappyHour.StartEvent();
						}break;
	#endif				
	#if (PACK_EDITION>=2)
						case 4:
						{
							HitAndUp.StartEvent();
						}break;
	#endif
#endif
						case 5:
						{
							Raklion.EventStart();
						}break;
					
#if(GS_CASTLE_NOEVENTS == 0)
						case 6:
						{
							MossShop.StartEvent();
						}break;

	#if (PACK_EDITION>=2)					
						case 7:
						{
							XMasEvent.StartEvent();
						}break;
	#endif
	#if (PACK_EDITION>=1)					
						case 8:
						{
							GreenEvent.StartEvent();
						}break;
	#endif	
#endif

#if (GS_CASTLE==1)
#if (PACK_EDITION>=3)					
						case 9:
						{
							g_Swamp.StartEvent();
						}break;
#endif
#endif

#if(GS_CASTLE_NOEVENTS == 0)	
	#if (PACK_EDITION>=1)
						case 10:
						{
							SummerEvent.StartEvent();
						}break;
	#endif
	#if (PACK_EDITION>=2)					
						case 11:
						{
							HalloweenEvent.StartEvent();
						}break;
	#endif
#endif

#if (PACK_EDITION>=3)	
						case 12:
						{
							BotHider.StartEvent();
						}break;
#endif

#if (PACK_EDITION>=3)					
						case 13:
						{
							GensCloseMap.StartEvent();
						}break;

						case 14:
						{
							RainItemEvent.StartEvent();
						}break;

						case 15:
						{
							ObjTitanRank.StartEvent();
						}break;

						case 16:
						{
							ObjTitanLottery.StartEvent();
						}break;

						case 17:
						{
							ObjQaA.StartEvent();
						}break;
#endif	
					}
				}
			}
		}
	}
	g_EventManager.SCFEventManagerRunning = 0;
	_endthread();
}