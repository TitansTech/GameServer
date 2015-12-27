#include "stdafx.h"
#include "ChaosCastle.h"
#include "BloodCastle.h"
#include "GameServer.h"
#include "GameMain.h"
//#include "GameServerAuth.h"
#include "giocp.h"
#include "DSProtocol.h"
#include "LogProc.h"
#include "TNotice.h"
#include "CrywolfSync.h"
#include "Crywolf.h"
#include "Event.h"
#include "DevilSquareGround.h"
#include "HitAndUp.h"
#include "VIPSystem.h"
#include "QuestS5Info.h"

#include "..\include\readscript.h"
#include "..\common\winutil.h"


// GS-N 0.99.60T 0x0050BB10
//	GS-N	1.00.18	JPN	0x0053D380	-	Completed



int g_iChaosCastle_OffLineGiftRate = 2;







CChaosCastle g_ChaosCastle;



BOOL CC_MAP_RANGE(int Map)
{
	if (Map == MAP_INDEX_CHAOSCASTLE7)
	{
		return TRUE;
	}
	if ((Map < MAP_INDEX_CHAOSCASTLE1) || (Map > MAP_INDEX_CHAOSCASTLE6))
	{
		return FALSE;
	}
	return TRUE;
}


CChaosCastle::CChaosCastle()
{
	this->m_bCC_EVENT_ENABLE = FALSE;
	this->m_iCC_TIME_MIN_OPEN = 10;
	this->m_iCC_NORMAL_ITEM_DROP = 100;
	this->m_iCC_EXCEL_ITEM_DROP = 1200;
	this->m_iCC_MONSTER_REGEN = 0;

	for ( int i=0;i<MAX_CHAOSCASTLE_LEVEL;i++)
	{
		this->m_stChaosCastleData[i].m_iCC_STATE = CC_STATE_NONE;
		if (i != 6)
			this->m_stChaosCastleData[i].m_iMapNumber = i + MAP_INDEX_CHAOSCASTLE1;
		else
			this->m_stChaosCastleData[i].m_iMapNumber = i + 47;
		this->m_stChaosCastleData[i].m_iChaosCastleIndex = i;
		this->m_stChaosCastleData[i].m_iCC_REMAIN_MSEC = -1;
		this->m_stChaosCastleData[i].m_iCC_TICK_COUNT = -1;
		this->ClearChaosCastleData(i);
	}

	srand(time(NULL));
}


CChaosCastle::~CChaosCastle()
{
	for ( int i=0;i<MAX_CHAOSCASTLE_LEVEL;i++)
	{
		// Empty
	}
}



void CChaosCastle::Run()
{
	if ( this->m_bCC_EVENT_ENABLE != FALSE )
	{
		for ( int i=0;i<MAX_CHAOSCASTLE_LEVEL;i++)
		{
			switch ( this->m_stChaosCastleData[i].m_iCC_STATE )
			{
				case CC_STATE_NONE:
					this->ProcState_None(i);
					break;

				case CC_STATE_CLOSED:
					this->ProcState_Closed(i);
					break;

				case CC_STATE_PLAYING:
					this->ProcState_Playing(i);
					break;

				case CC_STATE_PLAYEND:
					this->ProcState_PlayEnd(i);
					break;
			}
		}
	}
}


void CChaosCastle::Init(bool bEVENT_ENABLE)
{
	this->m_bCC_EVENT_ENABLE = bEVENT_ENABLE;

	for ( int i=0;i<MAX_CHAOSCASTLE_LEVEL;i++)
	{
		this->SetState(i, CC_STATE_CLOSED);
	}

	for ( int iTRAP_STEP = 0; iTRAP_STEP<MAX_CC_TRAP_STEP;iTRAP_STEP++)
	{
		g_rtPOINT_FRAME[0].left = (::g_iChaosCastle_GroundAxis[2] + ::g_iChaosCastle_GroundAxis[0]) / 2 + 1;
		g_rtPOINT_FRAME[0].top = g_iChaosCastle_GroundAxis[1];
		g_rtPOINT_FRAME[0].right = ::g_iChaosCastle_GroundAxis[2];
		g_rtPOINT_FRAME[0].bottom = (::g_iChaosCastle_GroundAxis[3] + ::g_iChaosCastle_GroundAxis[1] ) / 2 ;

		g_rtPOINT_FRAME[1].left = (::g_iChaosCastle_GroundAxis[2] + ::g_iChaosCastle_GroundAxis[0]) / 2 + 1;
		g_rtPOINT_FRAME[1].top = (::g_iChaosCastle_GroundAxis[3] + ::g_iChaosCastle_GroundAxis[1] ) / 2 + 1;
		g_rtPOINT_FRAME[1].right = g_iChaosCastle_GroundAxis[2];
		g_rtPOINT_FRAME[1].bottom = ::g_iChaosCastle_GroundAxis[3];
		
		g_rtPOINT_FRAME[2].left = ::g_iChaosCastle_GroundAxis[0];
		g_rtPOINT_FRAME[2].top = (::g_iChaosCastle_GroundAxis[3] + ::g_iChaosCastle_GroundAxis[1] ) / 2 + 1;
		g_rtPOINT_FRAME[2].right = (::g_iChaosCastle_GroundAxis[2] + ::g_iChaosCastle_GroundAxis[0] ) / 2 ;
		g_rtPOINT_FRAME[2].bottom = ::g_iChaosCastle_GroundAxis[3];

		g_rtPOINT_FRAME[3].left = ::g_iChaosCastle_GroundAxis[0];
		g_rtPOINT_FRAME[3].top = ::g_iChaosCastle_GroundAxis[1];
		g_rtPOINT_FRAME[3].right = (::g_iChaosCastle_GroundAxis[2] + ::g_iChaosCastle_GroundAxis[0] ) / 2;
		g_rtPOINT_FRAME[3].bottom = (::g_iChaosCastle_GroundAxis[3] + ::g_iChaosCastle_GroundAxis[1] ) / 2;




		g_rtPOINT_TRAP[0].left = (::g_iChaosCastle_DamageAxis[iTRAP_STEP][2] + ::g_iChaosCastle_DamageAxis[iTRAP_STEP][0]) / 2 + 1;
		g_rtPOINT_TRAP[0].top = ::g_iChaosCastle_DamageAxis[iTRAP_STEP][1];
		g_rtPOINT_TRAP[0].right = ::g_iChaosCastle_DamageAxis[iTRAP_STEP][2];
		g_rtPOINT_TRAP[0].bottom = (::g_iChaosCastle_DamageAxis[iTRAP_STEP][3] + ::g_iChaosCastle_DamageAxis[iTRAP_STEP][1]) / 2;

		g_rtPOINT_TRAP[1].left = (::g_iChaosCastle_DamageAxis[iTRAP_STEP][2] + ::g_iChaosCastle_DamageAxis[iTRAP_STEP][0]) / 2 + 1;
		g_rtPOINT_TRAP[1].top = (::g_iChaosCastle_DamageAxis[iTRAP_STEP][3] + ::g_iChaosCastle_DamageAxis[iTRAP_STEP][1] ) / 2 + 1;
		g_rtPOINT_TRAP[1].right = g_iChaosCastle_DamageAxis[iTRAP_STEP][2];
		g_rtPOINT_TRAP[1].bottom = ::g_iChaosCastle_DamageAxis[iTRAP_STEP][3];

		g_rtPOINT_TRAP[2].left = ::g_iChaosCastle_DamageAxis[iTRAP_STEP][0];
		g_rtPOINT_TRAP[2].top = (::g_iChaosCastle_DamageAxis[iTRAP_STEP][3] + ::g_iChaosCastle_DamageAxis[iTRAP_STEP][1] ) / 2 + 1;
		g_rtPOINT_TRAP[2].right = (::g_iChaosCastle_DamageAxis[iTRAP_STEP][2] + ::g_iChaosCastle_DamageAxis[iTRAP_STEP][0] ) / 2;
		g_rtPOINT_TRAP[2].bottom = ::g_iChaosCastle_DamageAxis[iTRAP_STEP][3];

		g_rtPOINT_TRAP[3].left = ::g_iChaosCastle_DamageAxis[iTRAP_STEP][0];
		g_rtPOINT_TRAP[3].top = ::g_iChaosCastle_DamageAxis[iTRAP_STEP][1];
		g_rtPOINT_TRAP[3].right = (::g_iChaosCastle_DamageAxis[iTRAP_STEP][2] + ::g_iChaosCastle_DamageAxis[iTRAP_STEP][0] ) / 2;
		g_rtPOINT_TRAP[3].bottom = (::g_iChaosCastle_DamageAxis[iTRAP_STEP][3] + ::g_iChaosCastle_DamageAxis[iTRAP_STEP][1] ) / 2;
	}
}



void CChaosCastle::Load(LPSTR filename)
{
	SMDFile = fopen(filename, "r");

	if ( SMDFile == NULL )
	{
		MsgBox("[Chaos Castle] Info file Load Fail [%s]", filename);
		return;
	}

	int Token;
	int type = -1;

	this->m_vtChaosCastleOpenTime.erase( this->m_vtChaosCastleOpenTime.begin(), this->m_vtChaosCastleOpenTime.end());

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
			if ( type == 0 )	// Time Setting
			{
				Token = GetToken();

				if ( strcmp("end", TokenString) == 0 )
				{
					break;
				}

				this->m_iCC_TIME_MIN_OPEN = TokenNumber;

				Token = GetToken();
				this->m_iCC_TIME_MIN_PLAY = TokenNumber;

				Token = GetToken();
				this->m_iCC_TIME_MIN_REST = TokenNumber;
			}
			else if ( type == 1 ) // Drops
			{
				Token = GetToken();

				if ( strcmp("end", TokenString) == 0 )
				{
					break;
				}

				this->m_iCC_NORMAL_ITEM_DROP = TokenNumber;

				Token = GetToken();
				this->m_iCC_EXCEL_ITEM_DROP = TokenNumber;

				Token = GetToken();
				this->m_iCC_MONSTER_REGEN = TokenNumber;
			}
			else if ( type == 2 ) // Schedule
			{
				Token = GetToken();

				if ( strcmp("end", TokenString) == 0 )
				{
					break;
				}
				
				CHAOSCASTLE_START_TIME Schedule;

				Schedule.m_iHour = TokenNumber;

				Token = GetToken();
				Schedule.m_iMinute = TokenNumber;

				this->m_vtChaosCastleOpenTime.push_back(Schedule);
			}
			else if ( type == 3 ) // ItemDropProb
			{
				Token = GetToken();

				if ( strcmp("end", TokenString) == 0 )
				{
					break;
				}
		
				g_iChaosCastle_OffLineGiftRate = TokenNumber;
				g_iChaosCastle_OffLineGiftRate %= 10000;
			}
		}
	}

	fclose(SMDFile);

	LogAdd("%s file load!", filename);
}





void CChaosCastle::CheckSync(int iChaosCastleIndex)
{
	if ( CC_FLOOR_RANGE(iChaosCastleIndex) == FALSE )
	{
		return;
	}

	if ( this->m_vtChaosCastleOpenTime.empty() != false )
	{
		LogAddC(2, "[Chaos Castle] (%d) No Schedule Time Data - Chaos Castle Terminated (m_vtChaosCastleOpenTime.empty())", iChaosCastleIndex);
		this->SetState(iChaosCastleIndex+1, CC_STATE_NONE);
		
		return;
	}

	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);
	int iMIN_HOUR = 24;
	int iMIN_MINUTE = 60;
	BOOL bTIME_CHANGED = FALSE;
	
	std::vector<CHAOSCASTLE_START_TIME>::iterator it;

	for ( it = this->m_vtChaosCastleOpenTime.begin() ; it != this->m_vtChaosCastleOpenTime.end() ; it++ )
	{
		//CHAOSCASTLE_START_TIME * stCCTime = it;//ORIGINAL

		//IcaruS FIX
		CHAOSCASTLE_START_TIME * stCCTime = new(CHAOSCASTLE_START_TIME);
		stCCTime->m_iHour = it->m_iHour;
		stCCTime->m_iMinute = it->m_iMinute;
		//IcaruS FIX

		if ( (sysTime.wHour * 60 + sysTime.wMinute) < (stCCTime->m_iHour * 60 + stCCTime->m_iMinute) )
		{
			if ( ( iMIN_HOUR * 60 + iMIN_MINUTE ) > ( stCCTime->m_iHour * 60 + stCCTime->m_iMinute ) )
			{
				bTIME_CHANGED = TRUE;
				iMIN_HOUR = stCCTime->m_iHour;
				iMIN_MINUTE = stCCTime->m_iMinute;
			}
		}
	}

	if ( bTIME_CHANGED == FALSE )
	{
		it = this->m_vtChaosCastleOpenTime.begin();
		iMIN_HOUR = 24;
		iMIN_MINUTE = 60;

		for (it = this->m_vtChaosCastleOpenTime.begin() ; it != this->m_vtChaosCastleOpenTime.end() ; it++ )
		{
			//CHAOSCASTLE_START_TIME * stCCTime = it;//ORIGINAL

			//IcaruS FIX
			CHAOSCASTLE_START_TIME * stCCTime = new(CHAOSCASTLE_START_TIME);
			stCCTime->m_iHour = it->m_iHour;
			stCCTime->m_iMinute = it->m_iMinute;
			//IcaruS FIX
			if ( ( iMIN_HOUR * 60 + iMIN_MINUTE ) > ( stCCTime->m_iHour * 60 + stCCTime->m_iMinute ) )
			{
				bTIME_CHANGED = 2;
				iMIN_HOUR = stCCTime->m_iHour;
				iMIN_MINUTE = stCCTime->m_iMinute;
			}
		}
	}

	switch ( bTIME_CHANGED )
	{
		case TRUE:
			this->m_stChaosCastleData[iChaosCastleIndex].m_iCC_REMAIN_MSEC = (((iMIN_HOUR * 60 * 60) + (iMIN_MINUTE * 60)) - ((sysTime.wHour * 60 * 60) + (sysTime.wMinute * 60) + (sysTime.wSecond))) * 1000;
			break;

		case 2:
			this->m_stChaosCastleData[iChaosCastleIndex].m_iCC_REMAIN_MSEC = ((((iMIN_HOUR+24) * 60 * 60) + (iMIN_MINUTE * 60)) - ((sysTime.wHour * 60 * 60) + (sysTime.wMinute * 60) + (sysTime.wSecond))) * 1000;
			break;

		default:
			LogAddC(2, "[Chaos Castle] (%d) No Schedule Time Data - Chaos Castle Terminated (bTIME_CHANGED=%d)",
				iChaosCastleIndex + 1, bTIME_CHANGED);
			this->SetState(iChaosCastleIndex, CC_STATE_NONE);
			return;

	}

	this->m_stChaosCastleData[iChaosCastleIndex].m_iCC_TICK_COUNT = GetTickCount();

	LogAddTD("[Chaos Castle] (%d) Sync Start Time. [%d] min remain (START HOUR:%d, MIN:%d)",
		iChaosCastleIndex+1, this->m_stChaosCastleData[iChaosCastleIndex].m_iCC_REMAIN_MSEC / 60000,
		iMIN_HOUR, iMIN_MINUTE);

}





void CChaosCastle::ProcState_None(int iChaosCastleIndex)
{
	return;
}





void CChaosCastle::ProcState_Closed(int iChaosCastleIndex)
{
	int iTICK_MSEC = GetTickCount() - this->m_stChaosCastleData[iChaosCastleIndex].m_iCC_TICK_COUNT;

	if ( iTICK_MSEC >= 1000 )
	{
		this->m_stChaosCastleData[iChaosCastleIndex].m_iCC_REMAIN_MSEC -= iTICK_MSEC;
		this->m_stChaosCastleData[iChaosCastleIndex].m_iCC_TICK_COUNT = GetTickCount();

		if ( g_bChaosCastle != FALSE )
		{
			if ( this->m_stChaosCastleData[iChaosCastleIndex].m_iCC_REMAIN_MSEC <= ( this->m_iCC_TIME_MIN_OPEN * 60 * 1000 ) && this->m_stChaosCastleData[iChaosCastleIndex].m_bCC_CAN_ENTER == false)
			{
				this->m_stChaosCastleData[iChaosCastleIndex].m_bCC_CAN_ENTER = true;
				this->m_stChaosCastleData[iChaosCastleIndex].m_bCC_CAN_PARTY = true;
			}

			if ( this->m_stChaosCastleData[iChaosCastleIndex].m_iCC_REMAIN_MSEC <= ( this->m_iCC_TIME_MIN_OPEN * 60 * 1000 ) && this->m_stChaosCastleData[iChaosCastleIndex].m_iCC_REMAIN_MSEC > 0 && (this->m_stChaosCastleData[iChaosCastleIndex].m_iCC_REMAIN_MSEC/60000) !=this->m_stChaosCastleData[iChaosCastleIndex].m_iCC_NOTIFY_COUNT)
			{
				this->m_stChaosCastleData[iChaosCastleIndex].m_iCC_NOTIFY_COUNT = this->m_stChaosCastleData[iChaosCastleIndex].m_iCC_REMAIN_MSEC / 60000;

				if ( iChaosCastleIndex == 0 )
				{
					PMSG_NOTICE pNotice;
					pNotice.type = 0;	// 3
					pNotice.btCount = 0;	// 4
					pNotice.wDelay = 0;	// 6	
					pNotice.dwColor = 0;	// 8
					pNotice.btSpeed = 0;	// C

					TNotice::MakeNoticeMsgEx(&pNotice, 0, lMsg.Get( MSGGET(4, 208)), this->m_stChaosCastleData[iChaosCastleIndex].m_iCC_NOTIFY_COUNT+1);
					this->SendAllUserAnyMsg( (LPBYTE)&pNotice, pNotice.h.size);
				}
			}

			if ( this->m_stChaosCastleData[iChaosCastleIndex].m_iCC_REMAIN_MSEC <= 30000 && this->m_stChaosCastleData[iChaosCastleIndex].m_iCC_REMAIN_MSEC > 0 && this->m_stChaosCastleData[iChaosCastleIndex].m_bCC_MSG_BEFORE_ENTER == false )
			{
				this->m_stChaosCastleData[iChaosCastleIndex].m_bCC_MSG_BEFORE_ENTER = true;

				if ( iChaosCastleIndex == 0 )
				{
					PMSG_SET_DEVILSQUARE pMsg;

					PHeadSetB((LPBYTE)&pMsg, 0x92, sizeof(pMsg));
					pMsg.Type = 10;

					for (int i= OBJ_STARTUSERINDEX;i<OBJMAX;i++)
					{
						if ( gObj[i].Connected == PLAYER_PLAYING && gObj[i].Type == OBJ_USER)
						{
							if ( CC_MAP_RANGE(gObj[i].MapNumber) == FALSE )
							{
								if ( BC_MAP_RANGE(gObj[i].MapNumber) == FALSE )
								{
									DataSend(i, (UCHAR*)&pMsg, pMsg.h.size);
								}
							}
						}
					}
				}
			}
		}
	}

	if ( this->m_stChaosCastleData[iChaosCastleIndex].m_iCC_REMAIN_MSEC <= 0 )
	{
		if ( g_bChaosCastle != FALSE )
		{
			this->SetState(iChaosCastleIndex, CC_STATE_PLAYING);
		}
		else
		{
			this->SetState(iChaosCastleIndex, CC_STATE_CLOSED);
		}
	}
}





void CChaosCastle::ProcState_Playing(int iChaosCastleIndex)
{
	int iTICK_MSEC = GetTickCount() - this->m_stChaosCastleData[iChaosCastleIndex].m_iCC_TICK_COUNT;

	if ( iTICK_MSEC >= 1000 )
	{
		this->m_stChaosCastleData[iChaosCastleIndex].m_iCC_REMAIN_MSEC -= iTICK_MSEC;
		this->m_stChaosCastleData[iChaosCastleIndex].m_iCC_TICK_COUNT = GetTickCount();

		if ( this->m_stChaosCastleData[iChaosCastleIndex].m_iCC_REMAIN_MSEC <= ((this->m_iCC_TIME_MIN_PLAY*60-30)*1000) &&
			 this->m_stChaosCastleData[iChaosCastleIndex].m_bCC_MSG_BEFORE_PLAY == false )
		{
			this->m_stChaosCastleData[iChaosCastleIndex].m_bCC_MSG_BEFORE_PLAY = true;
			
			PMSG_SET_DEVILSQUARE pMsg;
			PHeadSetB((LPBYTE)&pMsg, 0x92, sizeof(pMsg));
			pMsg.Type = 11;
			this->SendChaosCastleAnyMsg((BYTE*)&pMsg, sizeof(pMsg), iChaosCastleIndex);
		}

		if ( this->m_stChaosCastleData[iChaosCastleIndex].m_iCC_REMAIN_MSEC <= 30000 &&
			 this->m_stChaosCastleData[iChaosCastleIndex].m_iCC_REMAIN_MSEC > 0  &&
			 this->m_stChaosCastleData[iChaosCastleIndex].m_bCC_MSG_BEFORE_END == false )
		{
			this->m_stChaosCastleData[iChaosCastleIndex].m_bCC_MSG_BEFORE_END = true;
			PMSG_SET_DEVILSQUARE pMsg;
			PHeadSetB((LPBYTE)&pMsg, 0x92, sizeof(pMsg));
			pMsg.Type = 12;
			this->SendChaosCastleAnyMsg((BYTE*)&pMsg, sizeof(pMsg), iChaosCastleIndex);
		}

		// Set Play Quest
		if ( this->m_stChaosCastleData[iChaosCastleIndex].m_iCC_REMAIN_MSEC <= ((this->m_iCC_TIME_MIN_PLAY*60-60)*1000) &&
			this->m_stChaosCastleData[iChaosCastleIndex].m_bCC_PLAY_START == false )
		{
			PMSG_NOTICE pNotice;
			pNotice.type = 0;	// 3
			pNotice.btCount = 0;	// 4
			pNotice.wDelay = 0;	// 6	
			pNotice.dwColor = 0;	// 8
			pNotice.btSpeed = 0;	// C

			this->m_stChaosCastleData[iChaosCastleIndex].m_iCC_REMAIN_MSEC = (this->m_iCC_TIME_MIN_PLAY*60)*1000;
			this->m_stChaosCastleData[iChaosCastleIndex].m_bCC_CAN_PARTY = false;
			TNotice::MakeNoticeMsgEx((TNotice*)&pNotice, 0, lMsg.Get(MSGGET(4,209)), iChaosCastleIndex+1);
			this->SendChaosCastleAnyMsg( (LPBYTE)&pNotice, pNotice.h.size, iChaosCastleIndex);
			this->UnSafetyCastleZone(iChaosCastleIndex);
			this->SendCastleZoneSafetyInfo(iChaosCastleIndex, 0);
			this->m_stChaosCastleData[iChaosCastleIndex].m_bCC_PLAY_START = true;
			this->m_stChaosCastleData[iChaosCastleIndex].m_iCC_FIRST_USER_COUNT = this->ReCalcUserCount(iChaosCastleIndex);
			this->m_stChaosCastleData[iChaosCastleIndex].m_lCC_CURRENT_MONSTER_COUNT = this->SetMonster(iChaosCastleIndex);
			this->m_stChaosCastleData[iChaosCastleIndex].m_iCC_FIRST_MONSTER_COUNT = this->m_stChaosCastleData[iChaosCastleIndex].m_lCC_CURRENT_MONSTER_COUNT; 
			this->SetItemsToMonster(iChaosCastleIndex);
			this->SendNoticeState(iChaosCastleIndex, 5);
			
			LogAddTD("[Chaos Castle] (%d) Chaos Castle Quest Start (UserCount:%d)",
				iChaosCastleIndex+1, this->GetCurPlayUser(iChaosCastleIndex));
		}

		if ( this->m_stChaosCastleData[iChaosCastleIndex].m_bCC_PLAY_START == false &&
			 this->m_stChaosCastleData[iChaosCastleIndex].m_bCC_CAN_ENTER == false )
		{
			int iCurPlayUser = this->GetCurPlayUser(iChaosCastleIndex);

			if (iCurPlayUser == 0 )
			{
				this->SetState(iChaosCastleIndex, CC_STATE_CLOSED);
				LogAddTD("[Chaos Castle] (%d) Chaos Castle Quest Closed - No User",
					iChaosCastleIndex+1);

				return;
			}
		}

		if ( this->m_stChaosCastleData[iChaosCastleIndex].m_bCC_PLAY_START == true )
		{
			this->ProcessFallUser(iChaosCastleIndex);
			this->CheckUserInDieTile(iChaosCastleIndex);
			this->CheckMonsterInDieTile(iChaosCastleIndex);
			this->ProcessTrapStatus(iChaosCastleIndex);

			if ( this->m_stChaosCastleData[iChaosCastleIndex].m_iCC_TRAP_STATUS != this->m_stChaosCastleData[iChaosCastleIndex].m_iCC_TRAP_OLD_STATUS )
			{
				this->m_stChaosCastleData[iChaosCastleIndex].m_iCC_TRAP_OLD_STATUS = this->m_stChaosCastleData[iChaosCastleIndex].m_iCC_TRAP_STATUS;
				this->SetMapAttrHollow(iChaosCastleIndex, this->m_stChaosCastleData[iChaosCastleIndex].m_iCC_TRAP_STATUS);
				this->PullObjInnerPlace(iChaosCastleIndex, this->m_stChaosCastleData[iChaosCastleIndex].m_iCC_TRAP_STATUS);
			}

			BOOL bPlayEnded = this->CheckPlayEnded(iChaosCastleIndex);

			if ( bPlayEnded == TRUE )
			{
				int iWinnerIndex = -1;
				int iMonsterCount = this->GetMonsterListCount(iChaosCastleIndex);

				if ( iMonsterCount <= 0 )
				{
					iWinnerIndex = this->GetCurrentWinUser(iChaosCastleIndex);

					if ( iWinnerIndex != -1 )
					{
						LogAddTD("[Chaos Castle] (%d) [%s][%s] is Winner. [CharInfoSave : Class=%d, Level=%d, LvPoint=%d, Exp=%d, Str=%d, Dex=%d, Vit=%d, Energy=%d, LeaderShip=%d, Map=%d, Pk=%d]",
							iChaosCastleIndex+1, gObj[iWinnerIndex].AccountID, gObj[iWinnerIndex].Name,
							gObj[iWinnerIndex].Class, gObj[iWinnerIndex].Level, gObj[iWinnerIndex].LevelUpPoint,
							gObj[iWinnerIndex].Experience, gObj[iWinnerIndex].Strength,
							gObj[iWinnerIndex].Dexterity, gObj[iWinnerIndex].Vitality, gObj[iWinnerIndex].Energy,
							gObj[iWinnerIndex].Leadership, gObj[iWinnerIndex].MapNumber, gObj[iWinnerIndex].m_PK_Level);

						this->SendWinMessage(iChaosCastleIndex, iWinnerIndex);
						this->GiveWinnerItem(iChaosCastleIndex, iWinnerIndex);
					}
					else
					{
						LogAddTD("[Chaos Castle] (%d) Has No Winner.", iChaosCastleIndex+1);
					}
				}
				else
				{
					LogAddTD("[Chaos Castle] (%d) Has No Winner : Monster Left (%d)",
						iChaosCastleIndex+1, this->m_stChaosCastleData[iChaosCastleIndex].m_lCC_CURRENT_MONSTER_COUNT);
				}

				this->SendAllLoserFailMessage(iChaosCastleIndex, iWinnerIndex);
				this->SetState(iChaosCastleIndex, CC_STATE_PLAYEND);

				return;
			}
		
			if ( bPlayEnded == 2 )
			{
				int iWinnerIndex = -1;
				int iMonsterCount = this->GetMonsterListCount(iChaosCastleIndex);

				if ( iMonsterCount <= 0 )
				{
					iWinnerIndex = this->GetCurrentWinUser(iChaosCastleIndex);

					if ( iWinnerIndex != -1 )
					{
						LogAddTD("[Chaos Castle] (%d) [%s][%s] is Winner. [CharInfoSave : Class=%d, Level=%d, LvPoint=%d, Exp=%d, Str=%d, Dex=%d, Vit=%d, Energy=%d, LeaderShip=%d, Map=%d, Pk=%d]",
							iChaosCastleIndex+1, gObj[iWinnerIndex].AccountID, gObj[iWinnerIndex].Name,
							gObj[iWinnerIndex].Class, gObj[iWinnerIndex].Level, gObj[iWinnerIndex].LevelUpPoint,
							gObj[iWinnerIndex].Experience, gObj[iWinnerIndex].Strength,
							gObj[iWinnerIndex].Dexterity, gObj[iWinnerIndex].Vitality, gObj[iWinnerIndex].Energy,
							gObj[iWinnerIndex].Leadership, gObj[iWinnerIndex].MapNumber, gObj[iWinnerIndex].m_PK_Level);

						this->SendWinMessage(iChaosCastleIndex, iWinnerIndex);
						this->GiveWinnerItem(iChaosCastleIndex, iWinnerIndex);
					}
					else
					{
						LogAddTD("[Chaos Castle] (%d) Has No Winner.", iChaosCastleIndex+1);
					}
				}
				else
				{
					LogAddTD("[Chaos Castle] (%d) Has No Winner : Monster Left (%d)",
						iChaosCastleIndex+1, this->m_stChaosCastleData[iChaosCastleIndex].m_lCC_CURRENT_MONSTER_COUNT);
				}

				this->SendAllLoserFailMessage(iChaosCastleIndex, iWinnerIndex);
				this->SetState(iChaosCastleIndex, CC_STATE_PLAYEND);

				return;
			}
		
			if ( bPlayEnded == FALSE )
			{
				this->SendNoticeState(iChaosCastleIndex, 6);
			}
		}
	}

	if ( this->m_stChaosCastleData[iChaosCastleIndex].m_iCC_REMAIN_MSEC <= 0 )
	{
		LogAddTD("[Chaos Castle] (%d) is Over : TIME-OUT (Left User:%d, Monster:%d)",
			iChaosCastleIndex+1, this->GetCurPlayUser(iChaosCastleIndex), this->GetMonsterListCount(iChaosCastleIndex));


		int iWinnerIndex = -1;
		int iMonsterCount = this->GetMonsterListCount(iChaosCastleIndex);

		if ( iMonsterCount <= 0 )
		{
			iWinnerIndex = this->GetCurrentWinUser(iChaosCastleIndex);

			if ( iWinnerIndex != -1 )
			{
				LogAddTD("[Chaos Castle] (%d) [%s][%s] is Winner. [CharInfoSave : Class=%d, Level=%d, LvPoint=%d, Exp=%d, Str=%d, Dex=%d, Vit=%d, Energy=%d, LeaderShip=%d, Map=%d, Pk=%d]",
					iChaosCastleIndex+1, gObj[iWinnerIndex].AccountID, gObj[iWinnerIndex].Name,
					gObj[iWinnerIndex].Class, gObj[iWinnerIndex].Level, gObj[iWinnerIndex].LevelUpPoint,
					gObj[iWinnerIndex].Experience, gObj[iWinnerIndex].Strength,
					gObj[iWinnerIndex].Dexterity, gObj[iWinnerIndex].Vitality, gObj[iWinnerIndex].Energy,
					gObj[iWinnerIndex].Leadership, gObj[iWinnerIndex].MapNumber, gObj[iWinnerIndex].m_PK_Level);

				this->SendWinMessage(iChaosCastleIndex, iWinnerIndex);
				this->GiveWinnerItem(iChaosCastleIndex, iWinnerIndex);
			}
			else
			{
				LogAddTD("[Chaos Castle] (%d) Has No Winner.", iChaosCastleIndex+1);
			}
		}
		else
		{
			LogAddTD("[Chaos Castle] (%d) Has No Winner : Monster Left (%d)",
				iChaosCastleIndex+1, this->m_stChaosCastleData[iChaosCastleIndex].m_lCC_CURRENT_MONSTER_COUNT);
		}

		this->SendAllLoserFailMessage(iChaosCastleIndex, iWinnerIndex);
		this->SetState(iChaosCastleIndex, CC_STATE_PLAYEND);
	}
}




void CChaosCastle::PullObjInnerPlace(int iChaosCastleIndex, int iTRAP_STEP)
{
	if ( CC_FLOOR_RANGE(iChaosCastleIndex) == FALSE )
		return;

	if ( CC_TRAP_STEP_RANGE(iTRAP_STEP) == FALSE )
		return;

	if ( iTRAP_STEP == 0 )
		return;

	int CCSum = MAP_INDEX_CHAOSCASTLE1;

	if (iChaosCastleIndex == 6)
		CCSum = 47;

	int iUSER_AXIS = 0;
	std::vector<POINT> vtMAP_UNTRAP[MAX_CC_TRAP_STEP];

	for ( int iAXIS = 0; iAXIS < MAX_CC_TRAP_STEP ; iAXIS++)
	{
		for ( int iMAPX = g_rtPOINT_TRAP[iAXIS].left ; iMAPX <= g_rtPOINT_TRAP[iAXIS].right ; iMAPX ++ )
		{
			for ( int iMAPY = g_rtPOINT_TRAP[iAXIS].top ; iMAPY <= g_rtPOINT_TRAP[iAXIS].bottom ; iMAPY++ )
			{
				BYTE btMapAttr = MapC[iChaosCastleIndex + CCSum].GetAttr(iMAPX, iMAPY);

				if ( (btMapAttr&2) != 2 && (btMapAttr&4) != 4 && (btMapAttr&8) != 8 ) 
				{
					POINT iPT = {iMAPX, iMAPY};
					vtMAP_UNTRAP[iAXIS].push_back(iPT);
				}
			}
		}
	}

	for ( int i=0;i<MAX_CHAOSCASTLE_USER;i++)
	{
		if ( this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex != -1 )
		{
			if ( gObjIsConnected(this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex) ==TRUE && gObj[this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].MapNumber == (iChaosCastleIndex + CCSum) )
			{
				int iSX = gObj[this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].X;
				int iSY = gObj[this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].Y;
				BYTE btMapAttr = MapC[iChaosCastleIndex + CCSum].m_attrbuf[iSY * 256 + iSX] & 0x08;

				if ( btMapAttr == 8 )
				{
					int n=0;
					for ( n=0;n<MAX_CC_TRAP_STEP;n++)
					{
						if ( iSX >= g_rtPOINT_FRAME[n].left && iSX <= g_rtPOINT_FRAME[n].right &&
							 iSY >= g_rtPOINT_FRAME[n].top && iSY <= g_rtPOINT_FRAME[n].bottom )
						{
							iUSER_AXIS = n;
							break;
						}
					}

					if ( n == MAX_CC_TRAP_STEP )
						continue;

					if ( vtMAP_UNTRAP[iUSER_AXIS].empty() == false )
					{
						POINT ptEMPTY = vtMAP_UNTRAP[iUSER_AXIS].back();
						vtMAP_UNTRAP[iUSER_AXIS].pop_back();

						if ( this->ObjSetPosition(this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex, ptEMPTY.x, ptEMPTY.y) == TRUE )
						{
							gObj[this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].m_iChaosCastleBlowTime = GetTickCount();
						}
					}
				}
			}
		}
	}

	for ( int iMON = 0; iMON < MAX_CHAOSCASTLE_MONSTER ; iMON++ )
	{
		if ( this->m_stChaosCastleData[iChaosCastleIndex].m_lCC_MONSTER_COUNT[iMON] == -1 )
			continue;

		int iMON_INDEX = this->m_stChaosCastleData[iChaosCastleIndex].m_lCC_MONSTER_COUNT[iMON];

		if ( !CHECK_LIMIT(iMON_INDEX, OBJ_MAXMONSTER))
		{
			this->m_stChaosCastleData[iChaosCastleIndex].m_lCC_MONSTER_COUNT[iMON] = -1;
			continue;
		}

		if ( gObj[iMON_INDEX].Life > 0.0f && CC_MAP_RANGE(gObj[iMON_INDEX].MapNumber) != FALSE && gObj[iMON_INDEX].Connected > PLAYER_EMPTY )
		{
			int iSX = gObj[iMON_INDEX].X;
			int iSY = gObj[iMON_INDEX].Y;

			BYTE btMapAttr = MapC[iChaosCastleIndex + CCSum].m_attrbuf[iSY * 256 + iSX] & 0x08;

			if ( btMapAttr == 8 )
			{
				int n;
				for ( n=0;n<MAX_CC_TRAP_STEP;n++)
				{
					if ( iSX >= g_rtPOINT_FRAME[n].left && iSX <= g_rtPOINT_FRAME[n].right &&
						 iSY >= g_rtPOINT_FRAME[n].top && iSY <= g_rtPOINT_FRAME[n].bottom )
					{
						iUSER_AXIS = n;
						break;
					}
				}

				if ( n == MAX_CC_TRAP_STEP )
					continue;

				if ( vtMAP_UNTRAP[iUSER_AXIS].empty() == false )
				{
					POINT ptEMPTY = vtMAP_UNTRAP[iUSER_AXIS].back();
					vtMAP_UNTRAP[iUSER_AXIS].pop_back();

					if ( this->ObjSetPosition(iMON_INDEX, ptEMPTY.x, ptEMPTY.y) == TRUE )
					{
						gObj[iMON_INDEX].m_iChaosCastleBlowTime = GetTickCount();
					}
				}
			}
		}
	}

	for ( int iMAP_ITEM =0;iMAP_ITEM<MAX_MAPITEM;iMAP_ITEM++)
	{
		if ( MapC[iChaosCastleIndex + CCSum].m_cItem[iMAP_ITEM].IsItem() )
		{
			int x = MapC[iChaosCastleIndex + CCSum].m_cItem[iMAP_ITEM].px;
			int y = MapC[iChaosCastleIndex + CCSum].m_cItem[iMAP_ITEM].py;

			if ( x < ::g_iChaosCastle_DamageAxis[iTRAP_STEP][0] || x > ::g_iChaosCastle_DamageAxis[iTRAP_STEP][2] ||
				 y < ::g_iChaosCastle_DamageAxis[iTRAP_STEP][1] || y > ::g_iChaosCastle_DamageAxis[iTRAP_STEP][3] )
			{
				MapC[iChaosCastleIndex + CCSum].m_cItem[iMAP_ITEM].m_State = 8;
				MapC[iChaosCastleIndex + CCSum].m_cItem[iMAP_ITEM].Give = true;
				MapC[iChaosCastleIndex + CCSum].m_cItem[iMAP_ITEM].live = false;
			}
		}
	}
}





void CChaosCastle::SetMapAttrHollow(int iChaosCastleIndex, int iTRAP_STEP)
{
	if ( CC_FLOOR_RANGE(iChaosCastleIndex) == FALSE )
	{
		return;
	}

	if ( CC_TRAP_STEP_RANGE(iTRAP_STEP) == FALSE )
	{
		return;
	}

	if ( iTRAP_STEP == 0 )
	{
		return;
	}

	for ( int a=0;a<MAX_CC_HOLLOW_ZONE;a++)
	{
		for ( int b=::g_iChaosCastle_MapHollowZone[iTRAP_STEP][a][0] ; b<=::g_iChaosCastle_MapHollowZone[iTRAP_STEP][a][2] ; b++)
		{
			for ( int c=::g_iChaosCastle_MapHollowZone[iTRAP_STEP][a][1] ; c<=::g_iChaosCastle_MapHollowZone[iTRAP_STEP][a][3] ; c++)
			{
				int CCSum = MAP_INDEX_CHAOSCASTLE1;
				if( iChaosCastleIndex == 6)
					CCSum = 47;
				MapC[iChaosCastleIndex +CCSum].m_attrbuf[ c * 256 + b] |= 8;
			}
		}
	}
	
}





void CChaosCastle::SetMapAttrFill(int iChaosCastleIndex)
{
	if ( CC_FLOOR_RANGE(iChaosCastleIndex) == FALSE )
	{
		return;
	}
	
	for ( int iTRAP_STEP=1;iTRAP_STEP<MAX_CC_TRAP_STEP;iTRAP_STEP++)
	{
		for ( int a=0;a<MAX_CC_HOLLOW_ZONE;a++)
		{
			for ( int b=::g_iChaosCastle_MapHollowZone[iTRAP_STEP][a][0] ; b<=::g_iChaosCastle_MapHollowZone[iTRAP_STEP][a][2] ; b++)
			{
				for ( int c=::g_iChaosCastle_MapHollowZone[iTRAP_STEP][a][1] ; c<=::g_iChaosCastle_MapHollowZone[iTRAP_STEP][a][3] ; c++)
				{
					int CCSum = MAP_INDEX_CHAOSCASTLE1;
					if( iChaosCastleIndex == 6)
						CCSum = 47;
					MapC[iChaosCastleIndex +CCSum].m_attrbuf[ c * 256 + b] &= ~8;
				}
			}
		}
	}
}





void CChaosCastle::ProcState_PlayEnd(int iChaosCastleIndex)
{
	int iTICK_MSEC = GetTickCount() - this->m_stChaosCastleData[iChaosCastleIndex].m_iCC_TICK_COUNT;

	if ( iTICK_MSEC >= 1000 )
	{
		this->m_stChaosCastleData[iChaosCastleIndex].m_iCC_REMAIN_MSEC -= iTICK_MSEC;
		this->m_stChaosCastleData[iChaosCastleIndex].m_iCC_TICK_COUNT = GetTickCount();
		this->CheckUserInDieTile(iChaosCastleIndex);

		if ( this->m_stChaosCastleData[iChaosCastleIndex].m_iCC_REMAIN_MSEC <= 30000 && this->m_stChaosCastleData[iChaosCastleIndex].m_iCC_REMAIN_MSEC > 0 && this->m_stChaosCastleData[iChaosCastleIndex].m_bCC_MSG_BEFORE_QUIT == false)
		{
			this->m_stChaosCastleData[iChaosCastleIndex].m_bCC_MSG_BEFORE_QUIT = true;

			PMSG_SET_DEVILSQUARE pMsg;

			PHeadSetB((LPBYTE)&pMsg, 0x92, sizeof(pMsg));
			pMsg.Type = 13;

			this->SendChaosCastleAnyMsg((BYTE *)&pMsg, sizeof(pMsg), iChaosCastleIndex);
		}


	}

	if ( this->m_stChaosCastleData[iChaosCastleIndex].m_iCC_REMAIN_MSEC <= 0 )
	{
		this->SetState(iChaosCastleIndex, CC_STATE_CLOSED);
	}
}





void CChaosCastle::SetState(int iChaosCastleIndex, int iCC_STATE)
{
	if ( CC_FLOOR_RANGE(iChaosCastleIndex) == FALSE )
	{
		return;
	}

	if ( iCC_STATE < CC_STATE_NONE || iCC_STATE > CC_STATE_PLAYEND )
	{
		return;
	}

	this->m_stChaosCastleData[iChaosCastleIndex].m_iCC_STATE = iCC_STATE;

	switch ( this->m_stChaosCastleData[iChaosCastleIndex].m_iCC_STATE )
	{
		case CC_STATE_NONE:
			this->SetState_None(iChaosCastleIndex);
			break;

		case CC_STATE_CLOSED:
			this->SetState_Closed(iChaosCastleIndex);
			break;

		case CC_STATE_PLAYING:
			this->SetState_Playing(iChaosCastleIndex);
			break;

		case CC_STATE_PLAYEND:
			this->SetState_PlayEnd(iChaosCastleIndex);
	}
	
}





void CChaosCastle::SetState_None(int iChaosCastleIndex)
{
	return;
}





void CChaosCastle::SetState_Closed(int iChaosCastleIndex)
{
	if ( CC_FLOOR_RANGE(iChaosCastleIndex) == FALSE )
	{
		return;
	}

	int CCRest = MAP_INDEX_CHAOSCASTLE1;
	if (iChaosCastleIndex == 6)
		CCRest = 47;

	this->SendNoticeState(iChaosCastleIndex, 7);
	this->ClearChaosCastleData(iChaosCastleIndex);
	this->ClearMonster(iChaosCastleIndex);

	for ( int n=OBJ_STARTUSERINDEX;n<OBJMAX;n++)
	{
		if ( gObj[n].MapNumber == (iChaosCastleIndex + CCRest))
		{
			if ( gObj[n].Connected > PLAYER_LOGGED )
			{
				gObjMoveGate(n, 22);
			}
		}
	}

	this->SafetyCastleZone(iChaosCastleIndex);
	this->SetMapAttrFill(iChaosCastleIndex);
	this->CheckSync(iChaosCastleIndex);

	LogAddTD("[Chaos Castle] (%d) SetState CLOSED", iChaosCastleIndex+1);
}





void CChaosCastle::SetState_Playing(int iChaosCastleIndex)
{
	if ( CC_FLOOR_RANGE(iChaosCastleIndex) == FALSE )
	{
		return;
	}

	this->m_stChaosCastleData[iChaosCastleIndex].m_bCC_CAN_ENTER = false;
	this->m_stChaosCastleData[iChaosCastleIndex].m_iCC_REMAIN_MSEC = this->m_iCC_TIME_MIN_PLAY * 60 * 1000;
	this->SendCastleZoneSafetyInfo(iChaosCastleIndex, true);

	if ( this->CheckCanStartPlay(iChaosCastleIndex) == false )
	{
		this->PlayFailedRollBack(iChaosCastleIndex);
		LogAddTD("[Chaos Castle] (%d) Failed to Start Chaos Castle : Lack of User", iChaosCastleIndex +1 );
		this->SetState_Closed(iChaosCastleIndex);
	}
	else
	{
#if (CHAOS_CASTLE_ZEN_REWARD==1)
		this->RewardZettoMoney(iChaosCastleIndex);	// #warning In older version it gives reward to the last user entering
#endif

		LogAddTD("[Chaos Castle] (%d) SetState PLAYING", iChaosCastleIndex + 1 );
	}
}





void CChaosCastle::SetState_PlayEnd(int iChaosCastleIndex)
{
	if ( CC_FLOOR_RANGE(iChaosCastleIndex) == FALSE )
	{
		return;
	}

	this->SendNoticeState(iChaosCastleIndex, 7);
	this->ClearMonster(iChaosCastleIndex);
	this->m_stChaosCastleData[iChaosCastleIndex].m_bCC_CAN_ENTER = false;
	this->m_stChaosCastleData[iChaosCastleIndex].m_iCC_REMAIN_MSEC = this->m_iCC_TIME_MIN_REST * 60 * 1000;

	LogAddTD("[Chaos Castle] (%d) SetState PLAYEND", iChaosCastleIndex+1);


}





void CChaosCastle::ClearChaosCastleData(int iChaosCastleIndex)
{
	if ( CC_FLOOR_RANGE(iChaosCastleIndex) == FALSE )
	{
		return;
	}

	this->m_stChaosCastleData[iChaosCastleIndex].Clear();

	for ( int i=0;i<MAX_CHAOSCASTLE_USER;i++ )
	{
		if ( this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex != -1 )
		{
			gObj[this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].m_cChaosCastleIndex = -1;
			gObj[this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].m_cChaosCastleSubIndex = -1;
			gObj[this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].m_iChaosCastleBlowTime = 0;
			gObj[this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].m_cKillUserCount = 0;
			gObj[this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].m_cKillMonsterCount = 0;
		}

		this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].Clear();
	}
}





void CChaosCastle::ClearMonster(int iChaosCastleIndex)
{
	if ( CC_FLOOR_RANGE(iChaosCastleIndex) == FALSE )
	{
		return;
	}

	int CCRest = MAP_INDEX_CHAOSCASTLE1;

	if((iChaosCastleIndex == 6) )
		CCRest = 47;

	for ( int n=0;n<OBJ_MAXMONSTER;n++)
	{
		if ( gObj[n].MapNumber == (iChaosCastleIndex+CCRest) )
		{
			gObjDel(n);
		}
	}
}

int  CChaosCastle::SetMonster(int iChaosCastleIndex)
{
	if ( CC_FLOOR_RANGE(iChaosCastleIndex) == FALSE )
	{
		return -1;
	}

	int iNOW_ADDED_MONSTER_COUNT = 0;
	int iMAX_ADDABLE_MONSTER_COUNT = MAX_CC_MONSTERS - this->GetCurPlayUser(iChaosCastleIndex);
	int result;

	if ( iMAX_ADDABLE_MONSTER_COUNT < 0 )
	{
		iMAX_ADDABLE_MONSTER_COUNT = 0;
	}

	for ( int n=0;n<gMSetBase.m_Count;n++)
	{
		if ( iNOW_ADDED_MONSTER_COUNT >= iMAX_ADDABLE_MONSTER_COUNT )
		{
			break;
		}

		if ( CC_MAP_RANGE(gMSetBase.m_Mp[n].m_MapNumber) != FALSE )
		{
			WORD btMonsterIndex = gMSetBase.m_Mp[n].m_Type;
			BYTE btMapNumber = gMSetBase.m_Mp[n].m_MapNumber;
			BYTE btChaosCastleIndex = btMapNumber - MAP_INDEX_CHAOSCASTLE1;

			if( btMapNumber == MAP_INDEX_CHAOSCASTLE7)
				btChaosCastleIndex = 6;

			if ( btChaosCastleIndex != iChaosCastleIndex )
			{
				continue;
			}

			result = gObjAddMonster(btMapNumber);

			if ( result >= 0 )
			{
				gObj[result].m_PosNum = n;
				gObj[result].X = gMSetBase.m_Mp[n].m_X;
				gObj[result].Y = gMSetBase.m_Mp[n].m_Y;
				gObj[result].MapNumber = gMSetBase.m_Mp[n].m_MapNumber;
				gObj[result].TX = gObj[result].X;
				gObj[result].TY = gObj[result].Y;
				gObj[result].m_OldX = gObj[result].X;
				gObj[result].m_OldY = gObj[result].Y;
				gObj[result].Dir = gMSetBase.m_Mp[n].m_Dir;
				gObj[result].StartX = gObj[result].X;
				gObj[result].StartY = gObj[result].Y;
				gObjSetMonster(result, btMonsterIndex,"CChaosCastle::SetMonster");
				gObj[result].MaxRegenTime = this->m_iCC_MONSTER_REGEN;
				gObj[result].m_cChaosCastleIndex = btChaosCastleIndex;
				gObj[result].Dir = rand() % 8;
				iNOW_ADDED_MONSTER_COUNT++;
				this->AddMonsterList(iChaosCastleIndex, result);
			}
		}
	}

	return iNOW_ADDED_MONSTER_COUNT;
}





void CChaosCastle::AddMonsterList(int iChaosCastleIndex, int iMonsterIndex)
{
	if ( CC_FLOOR_RANGE(iChaosCastleIndex) == FALSE )
	{
		return;
	}

	for ( int iMON =0;iMON<MAX_CC_MONSTERS;iMON++)
	{
		if ( this->m_stChaosCastleData[iChaosCastleIndex].m_lCC_MONSTER_COUNT[iMON] == -1 )
		{
			InterlockedExchange((LPLONG)&this->m_stChaosCastleData[iChaosCastleIndex].m_lCC_MONSTER_COUNT[iMON], iMonsterIndex);
			break;
		}
	}
}





void CChaosCastle::DelMonsterList(int iChaosCastleIndex, int iMonsterIndex)
{
	if ( CC_FLOOR_RANGE(iChaosCastleIndex) == FALSE )
	{
		return;
	}

	LogAddTD("[Chaos Castle][Bug Tracer] MONSTER DEL (%d) Count(%d) : ENTER - Index(%d)",
		iChaosCastleIndex+1, this->m_stChaosCastleData[iChaosCastleIndex].m_lCC_CURRENT_MONSTER_COUNT, iMonsterIndex);

	for ( int iMON =0;iMON<MAX_CC_MONSTERS;iMON++)
	{
		if ( this->m_stChaosCastleData[iChaosCastleIndex].m_lCC_MONSTER_COUNT[iMON] == iMonsterIndex )
		{
			InterlockedExchange(&this->m_stChaosCastleData[iChaosCastleIndex].m_lCC_MONSTER_COUNT[iMON], -1);
			break;
		}
	}

	InterlockedDecrement(&this->m_stChaosCastleData[iChaosCastleIndex].m_lCC_CURRENT_MONSTER_COUNT);

	if ( this->m_stChaosCastleData[iChaosCastleIndex].m_lCC_CURRENT_MONSTER_COUNT < 0 )
	{
		this->m_stChaosCastleData[iChaosCastleIndex].m_lCC_CURRENT_MONSTER_COUNT = 0;
	}

	LogAddTD("[Chaos Castle][Bug Tracer] MONSTER DEL (%d) Count(%d) : OUT - Index(%d)",
		iChaosCastleIndex+1, this->m_stChaosCastleData[iChaosCastleIndex].m_lCC_CURRENT_MONSTER_COUNT, iMonsterIndex);
}





int  CChaosCastle::GetMonsterListCount(int iChaosCastleIndex)
{
	if ( CC_FLOOR_RANGE(iChaosCastleIndex) == FALSE )
	{
		return -1;
	}

	int iAliveMonsterCount = 0;
	int iMapNumber = iChaosCastleIndex + 18;

	if(iChaosCastleIndex == 6)
		iMapNumber = 53;

	for(int i=0;i<OBJ_STARTUSERINDEX;i++)
	{
		if ( gObj[i].Connected == PLAYER_PLAYING &&
			 gObj[i].Type == OBJ_MONSTER &&
			 gObj[i].Live == 1) 
		{
			if ( gObj[i].MapNumber == iMapNumber )
			{
				iAliveMonsterCount++;
			}
		}
	}
	
	return iAliveMonsterCount;
}





void CChaosCastle::SetItemsToMonster(int iChaosCastleIndex)
{
	if ( CC_FLOOR_RANGE(iChaosCastleIndex) == FALSE )
	{
		return;
	}

	int iMAX_MONSTER = this->GetMonsterListCount(iChaosCastleIndex);

	if ( iMAX_MONSTER <= 0 )
	{
		return;
	}

	std::map<int, _MONSTER_ITEM_DROP>::iterator it;
	_MONSTER_ITEM_DROP MID;

	for ( int iA = 0;iA < MAX_CC_MONSTER_DROP_ITEM ; iA++)
	{
		for ( int iB=0; iB< ::g_iChaosCastle_MonsterItems[iChaosCastleIndex][iA][1] ; iB++)
		{
			for ( int iC=0;iC<200;iC++)	// Try 200 times
			{
				int iMONSTER_INDEX = this->m_stChaosCastleData[iChaosCastleIndex].m_lCC_MONSTER_COUNT[rand() % iMAX_MONSTER];
				it = this->m_stChaosCastleData[iChaosCastleIndex].m_mapMonsterItemList.find(iMONSTER_INDEX);

				if ( it == this->m_stChaosCastleData[iChaosCastleIndex].m_mapMonsterItemList.end() )
				{
					MID.m_iIndex = iMONSTER_INDEX;
					MID.m_iItemKind = ::g_iChaosCastle_MonsterItems[iChaosCastleIndex][iA][0];
					this->m_stChaosCastleData[iChaosCastleIndex].m_mapMonsterItemList.insert( std::pair<int,_MONSTER_ITEM_DROP>(iMONSTER_INDEX, MID) );
					break;
				}
			}
		}
	}
}





void CChaosCastle::SearchNDropMonsterItem(int iChaosCastleIndex, int iMonsterIndex, int iMaxHitUserIndex)
{
	if ( CC_FLOOR_RANGE(iChaosCastleIndex) == FALSE )
	{
		return;
	}

	if ( OBJMAX_RANGE(iMonsterIndex) == FALSE )
	{
		return;
	}

	if ( OBJMAX_RANGE(iMaxHitUserIndex) == FALSE )
	{
		return;
	}

	std::map<int, _MONSTER_ITEM_DROP>::iterator it = this->m_stChaosCastleData[iChaosCastleIndex].m_mapMonsterItemList.find(iMonsterIndex);

	if ( it == this->m_stChaosCastleData[iChaosCastleIndex].m_mapMonsterItemList.end() )
	{
		return;
	}

	_MONSTER_ITEM_DROP MID = it->second;
	int iType = MID.m_iItemKind;
	int iLevel = 0;
	
	if ( iType == ITEMGET(13,15) ) // Fruit
	{
		iLevel = rand()%4;
	}

	::ItemSerialCreateSend(gObj[iMaxHitUserIndex].m_Index, gObj[iMaxHitUserIndex].MapNumber, gObj[iMaxHitUserIndex].X,
		gObj[iMaxHitUserIndex].Y, iType, iLevel, 0, 0, 0, 0, iMaxHitUserIndex, 0, 0);

	if ( OBJMAX_RANGE(iMaxHitUserIndex) != FALSE )
	{
		LogAddTD("[Chaos Castle] (%d) Monster Dropped ChaosCastle Item To [%s][%s] (Item:%s)",
			iChaosCastleIndex+1, gObj[iMaxHitUserIndex].AccountID, gObj[iMaxHitUserIndex].Name,
			ItemAttribute[iType].Name);
	}
}





void CChaosCastle::SendAllUserAnyMsg(LPBYTE lpMsg, int iSize)
{
	for ( int i=OBJ_STARTUSERINDEX;i<OBJMAX;i++)
	{
		if ( gObj[i].Connected == PLAYER_PLAYING )
		{
			if ( gObj[i].Type == OBJ_USER )
			{
				DataSend(i, lpMsg, iSize);
			}
		}
	}
}





void CChaosCastle::SendChaosCastleAnyMsg(LPBYTE lpMsg, int iSize, int iChaosCastleIndex)
{
	if ( CC_FLOOR_RANGE(iChaosCastleIndex) == FALSE )
	{
		return;
	}

	for ( int i=0;i<MAX_CHAOSCASTLE_USER;i++)
	{
		if ( this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex != -1 )
		{
			if ( gObjIsConnected(this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex) != FALSE )
			{
				if ( gObj[this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].m_cChaosCastleIndex != -1 && gObj[this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].m_cChaosCastleSubIndex != -1 )
				{
					DataSend(this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex, lpMsg, iSize);
				}
			}
		}
	}
}





void CChaosCastle::SendNoticeMessage(int iChaosCastleIndex, LPSTR lpszMSG)
{
	if ( CC_FLOOR_RANGE(iChaosCastleIndex) == FALSE )
	{
		return;
	}

	PMSG_NOTICE pNotice;
	pNotice.type = 0;	// 3
	pNotice.btCount = 0;	// 4
	pNotice.wDelay = 0;	// 6	
	pNotice.dwColor = 0;	// 8
	pNotice.btSpeed = 0;	// C
	TNotice::MakeNoticeMsgEx((TNotice *)&pNotice, 0, lpszMSG);

	for ( int i=0;i<MAX_CHAOSCASTLE_USER;i++)
	{
		if ( this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex != -1 )
		{
			if ( gObj[this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].Connected > PLAYER_LOGGED )
			{
				if ( gObj[this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].m_cChaosCastleIndex != -1 && gObj[this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].m_cChaosCastleSubIndex != -1 )
				{
					DataSend(this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex, (UCHAR*)&pNotice, pNotice.h.size);
				}
			}
		}
	}

}





void CChaosCastle::SendNoticeState(int iChaosCastleIndex, int iPlayState)
{
	if ( CC_FLOOR_RANGE(iChaosCastleIndex) == FALSE )
	{
		return;
	}

	PMSG_STATEBLOODCASTLE pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x9B, sizeof(PMSG_STATEBLOODCASTLE));
	pMsg.btPlayState = iPlayState;
	pMsg.wRemainSec = this->m_stChaosCastleData[iChaosCastleIndex].m_iCC_REMAIN_MSEC / 1000;
	pMsg.wMaxKillMonster = this->m_stChaosCastleData[iChaosCastleIndex].m_iCC_FIRST_USER_COUNT + this->m_stChaosCastleData[iChaosCastleIndex].m_iCC_FIRST_MONSTER_COUNT;
	pMsg.wCurKillMonster = this->GetCurPlayUser(iChaosCastleIndex) + this->GetMonsterListCount(iChaosCastleIndex);
	pMsg.wUserHaveWeapon = -1;
	pMsg.btWeaponNum = -1;

	for ( int i=0;i<MAX_CHAOSCASTLE_USER;i++)
	{
		if ( this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex != -1 )
		{
			if ( gObj[this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].Connected > PLAYER_LOGGED )
			{
				if ( gObj[this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].m_cChaosCastleIndex != -1 && gObj[this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].m_cChaosCastleSubIndex != -1 )
				{
					DataSend(this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex, (UCHAR*)&pMsg, pMsg.h.size);
				}
			}
		}
	}
}





void CChaosCastle::SendWinMessage(int iChaosCastleIndex, int iWinnerIndex)
{
	if ( CC_FLOOR_RANGE(iChaosCastleIndex) == FALSE )
	{
		return;
	}

	if ( OBJMAX_RANGE(iWinnerIndex) == FALSE )
	{
		return;
	}

	int iChaosCastleSubIndex = gObj[iWinnerIndex].m_cChaosCastleSubIndex;

	if ( CC_SUB_FLOOR_RANGE(iChaosCastleSubIndex) == FALSE )
	{
		return;
	}

	if ( this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[iChaosCastleSubIndex].m_bSendQuitMsg == true )
	{
		return;
	}

	this->RewardUserEXP(iChaosCastleIndex, gObj[iWinnerIndex].m_cChaosCastleSubIndex, true );
	::GCServerMsgStringSend(lMsg.Get(MSGGET(4, 210)), iWinnerIndex, 1);
	this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[iChaosCastleSubIndex].m_bSendQuitMsg = true;
}





void CChaosCastle::SendFailMessage(int iChaosCastleIndex, int iLoserIndex)
{
	if ( CC_FLOOR_RANGE(iChaosCastleIndex) == FALSE )
	{
		return;
	}

	if ( OBJMAX_RANGE(iLoserIndex) == FALSE )
	{
		return;
	}

	int iChaosCastleSubIndex = gObj[iLoserIndex].m_cChaosCastleSubIndex;

	if ( CC_SUB_FLOOR_RANGE(iChaosCastleSubIndex) == FALSE )
	{
		return;
	}

	if ( this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[iChaosCastleSubIndex].m_bSendQuitMsg == true )
	{
		return;
	}

	this->RewardUserEXP(iChaosCastleIndex, gObj[this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[iChaosCastleSubIndex].m_iIndex].m_cChaosCastleSubIndex, false );
	this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[iChaosCastleSubIndex].m_bSendQuitMsg = true;
}





void CChaosCastle::SendAllLoserFailMessage(int iChaosCastleIndex, int iWinnerIndex)
{
	if ( CC_FLOOR_RANGE(iChaosCastleIndex) == FALSE )
	{
		return;
	}

	if ( OBJMAX_RANGE(iWinnerIndex) == FALSE && iWinnerIndex != -1)	
	{
		return;
	}


	for ( int i=0;i<MAX_CHAOSCASTLE_USER;i++)
	{
		if ( this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex != -1 )
		{
			if ( this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex != iWinnerIndex )
			{
				if ( this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_bSendQuitMsg == false )
				{
					this->RewardUserEXP(iChaosCastleIndex, gObj[this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].m_cChaosCastleSubIndex, false);
					::GCServerMsgStringSend(lMsg.Get(MSGGET(4, 211)), this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex, 1);
					this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_bSendQuitMsg = true;
				}
			}
		}
	}
}





int  CChaosCastle::EnterUserChaosCastle(int iChaosCastleIndex, int iUserIndex)
{
	if ( CC_FLOOR_RANGE(iChaosCastleIndex) == FALSE )
	{
		return -1;
	}

	int iRET_VAL = -1;

	for ( int i=0;i<MAX_CHAOSCASTLE_USER;i++)
	{
		if ( this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex != -1 )
		{
			if ( this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex == iUserIndex )
			{
				iRET_VAL = i;
				break;
			}
		}

		if ( this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex == -1 )
		{
			iRET_VAL = i;
			this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex = iUserIndex;
			this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iEXP = 0;
			this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iScore = 0;
			this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iUserState = 0;
			this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_bSendQuitMsg = false;
			this->m_stChaosCastleData[iChaosCastleIndex].m_iCC_ENTERED_USER++;
	
			if ( this->m_stChaosCastleData[iChaosCastleIndex].m_iCC_ENTERED_USER > MAX_CHAOSCASTLE_USER)
			{
				this->m_stChaosCastleData[iChaosCastleIndex].m_iCC_ENTERED_USER = MAX_CHAOSCASTLE_USER;
			}

			break;
		}
	}

	return iRET_VAL;
}





int  CChaosCastle::LeaveUserChaosCastle(int iChaosCastleIndex, int iUserIndex)
{
	if ( CC_FLOOR_RANGE(iChaosCastleIndex) == FALSE )
	{
		return -1;
	}

	int iRET_VAL = -1;

	for ( int i=0;i<MAX_CHAOSCASTLE_USER;i++)
	{
		if ( this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex == iUserIndex )
		{
			iRET_VAL = iUserIndex;	// #error Instead of iUserIndex put i
			this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex = -1;
			this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iEXP = 0;
			this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iScore = 0;
			this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iUserState = 0;
			this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_bSendQuitMsg = false;
			this->m_stChaosCastleData[iChaosCastleIndex].m_iCC_ENTERED_USER--;
	
			if ( this->m_stChaosCastleData[iChaosCastleIndex].m_iCC_ENTERED_USER < 0)
			{
				this->m_stChaosCastleData[iChaosCastleIndex].m_iCC_ENTERED_USER = 0;
			}

			break;
		}
	}

	return iRET_VAL;
}





void CChaosCastle::AddFallUser(int iChaosCastleIndex, int iUserIndex)
{
	if ( CC_FLOOR_RANGE(iChaosCastleIndex) == FALSE )
	{
		return;
	}

	for ( int iFALL=0;iFALL<MAX_CHAOSCASTLE_USER;iFALL++)
	{
		if ( this->m_stChaosCastleData[iChaosCastleIndex].m_lFallUser[iFALL] == -1 )
		{
			InterlockedExchange(&this->m_stChaosCastleData[iChaosCastleIndex].m_lFallUser[iFALL], iUserIndex);
			break;
		}
	}
}





void CChaosCastle::ProcessFallUser(int iChaosCastleIndex)
{
	if ( CC_FLOOR_RANGE(iChaosCastleIndex) == FALSE )
	{
		return;
	}

	int CCRest = MAP_INDEX_CHAOSCASTLE1;
	if((iChaosCastleIndex == 6) )
		CCRest = 47;

	for ( int iFALL=0;iFALL<MAX_CHAOSCASTLE_USER;iFALL++)
	{
		if ( this->m_stChaosCastleData[iChaosCastleIndex].m_lFallUser[iFALL] != -1 )
		{
			int iFALL_INDEX = this->m_stChaosCastleData[iChaosCastleIndex].m_lFallUser[iFALL];
			InterlockedExchange(&this->m_stChaosCastleData[iChaosCastleIndex].m_lFallUser[iFALL], -1);

			if ( (gObj[iFALL_INDEX].MapNumber) == (iChaosCastleIndex+CCRest) &&
				  gObjIsConnected(iFALL_INDEX) &&
				  gObj[iFALL_INDEX].Life > 0 )
			{
				gObj[iFALL_INDEX].Life = 0;
				gObj[iFALL_INDEX].m_State = 4;
				gObj[iFALL_INDEX].KillerType = 1;
				gObj[iFALL_INDEX].RegenTime = GetTickCount();
				gObj[iFALL_INDEX].DieRegen = 1;
				gObj[iFALL_INDEX].PathCount = 0;
				
				GCDiePlayerSend(&gObj[iFALL_INDEX], gObj[iFALL_INDEX].m_Index, 0, 0);
			}
		}
	}
}





void CChaosCastle::SetUserState(int iUserIndex, int iState)
{
	if ( !OBJMAX_RANGE(iUserIndex))
		return;

	int iChaosCastleIndex = gObj[iUserIndex].m_cChaosCastleIndex;
	int iChaosCastleSubIndex = gObj[iUserIndex].m_cChaosCastleSubIndex;

	if ( CC_FLOOR_RANGE(iChaosCastleIndex) == FALSE )
		return;

	if ( CC_SUB_FLOOR_RANGE(iChaosCastleSubIndex) == FALSE )
		return;

	switch ( iState )
	{
		case 0:
			this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[iChaosCastleSubIndex].m_iUserState = 0;
			break;
		case 1:
			this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[iChaosCastleSubIndex].m_iUserState = 1;
	}
}





int  CChaosCastle::CalcSendRewardEXP(int iUserIndex, int iEXP, int iKILLCOUNT_USER, int iKILLCOUNT_MONSTER)
{
	if ( iEXP <= 0 )
		return 0;

	int iRET_EXP = 0;
	int iCAL_EXP = iEXP + (iKILLCOUNT_MONSTER + iKILLCOUNT_USER)*1000;
	int iMAX_LEVCOUNT = 0;

	if ( !gObjIsConnected(iUserIndex))
		return 0;

	if ( g_CrywolfSync.GetOccupationState() == CRYWOLF_OCCUPATION_FAILED && g_iCrywolfApplyMvpPenalty )
	{
#if (PACK_EDITION>=2)
		if ((VipSystem.VipIsApplyCWExpPenalty == 0)&&(gObj[iUserIndex].Vip >= 1))
		{
		} else {
			iEXP = iEXP * g_CrywolfSync.GetGettingExpPenaltyRate() / 100;
		}
#else
		iEXP = iEXP * g_CrywolfSync.GetGettingExpPenaltyRate() / 100;
#endif
	}

	iRET_EXP = iCAL_EXP;

	if ( gObj[iUserIndex].Type == OBJ_USER )
	{
		gObjSetExpPetItem(iUserIndex, iRET_EXP);
		while ( iCAL_EXP > 0 )
		{
			if ( iCAL_EXP > 0 )
			{
				int LevelUp;
				iCAL_EXP = gObjLevelUp(&gObj[iUserIndex], iCAL_EXP, 0, EVENT_TYPE_CHAOSCASTLE, LevelUp);
			}

			iMAX_LEVCOUNT++;

			if ( iMAX_LEVCOUNT > 5 )
				break;
		}
		
		PMSG_KILLPLAYER_EXT pkillMsg;

		PHeadSetBE((LPBYTE)&pkillMsg, 0x9C, sizeof(pkillMsg));
		pkillMsg.NumberH = -1;
		pkillMsg.NumberL = -1;
		pkillMsg.ExpH = SET_NUMBERHW(iEXP);
		pkillMsg.ExpL = SET_NUMBERLW(iEXP);
		pkillMsg.DamageH = 0;
		pkillMsg.DamageL = 0;

		DataSend(iUserIndex, (LPBYTE)&pkillMsg, pkillMsg.h.size);
	}

	return iRET_EXP;
}





int  CChaosCastle::GetUserLevelToEnter(int iUserIndex)
{
	if ( !gObjIsConnected(iUserIndex))
		return -1;

	int iENTER_LEVEL = -1;

	if(gObjIsNewClass(&gObj[iUserIndex]) == 1)
	{
		iENTER_LEVEL = 6;
	}else
	{
		for(int i=0;i<MAX_CHAOSCASTLE_LEVEL;i++)
		{
			if ( gObj[iUserIndex].Class == CLASS_RAGEFIGHTER || 
				 gObj[iUserIndex].Class == CLASS_DARKLORD ||
				 gObj[iUserIndex].Class == CLASS_MAGICGLADIATOR )
			{
				if ( gObj[iUserIndex].Level >= g_sttCHAOSCASTLE_LEVEL[i].iLOWER_BOUND_MAGUMSA &&
					 gObj[iUserIndex].Level <= g_sttCHAOSCASTLE_LEVEL[i].iUPPER_BOUND_MAGUMSA )
				{
					iENTER_LEVEL = i;
					break;
				}
			}
			else
			{
				if ( gObj[iUserIndex].Level >= g_sttCHAOSCASTLE_LEVEL[i].iLOWER_BOUND &&
					 gObj[iUserIndex].Level <= g_sttCHAOSCASTLE_LEVEL[i].iUPPER_BOUND )
				{
					iENTER_LEVEL = i;
					break;
				}
			}
		}
	}
			 
	return iENTER_LEVEL;
}





BOOL CChaosCastle::CheckUserEnterMoney(int iUserIndex, int iEnterLevel)
{
	if ( OBJMAX_RANGE(iUserIndex) == FALSE )
	{
		return FALSE;
	}

	if ( CC_FLOOR_RANGE(iEnterLevel) == FALSE )
	{
		return FALSE;
	}

	if ( gObj[iUserIndex].Money >= ::g_iChaosCastle_EnterCost[iEnterLevel] )
	{
		return TRUE;
	}

	return FALSE;
}





BOOL CChaosCastle::PayUserEnterMoney(int iUserIndex, int iEnterLevel)
{
	if ( OBJMAX_RANGE(iUserIndex) == FALSE )
	{
		return FALSE;
	}

	if ( CC_FLOOR_RANGE(iEnterLevel) == FALSE )
	{
		return FALSE;
	}

	if ( gObj[iUserIndex].Money >= ::g_iChaosCastle_EnterCost[iEnterLevel] )
	{
		gObj[iUserIndex].Money -= ::g_iChaosCastle_EnterCost[iEnterLevel];
		GCMoneySend(iUserIndex, gObj[iUserIndex].Money);

		return TRUE;
	}

	return FALSE;
}





void CChaosCastle::GiveUserDamage(int iUserIndex, int iDamage)
{
	if ( !gObjIsConnected(iUserIndex))
		return;

	if ( gObj[iUserIndex].Life <= 0.0 )
		return;

	int newDamage = 0;

#if(CRYSTAL_MOB_BAR == 1)
	PMSG_ATTACKRESULT pResult;

	pResult.Life = gObj[iUserIndex].Life;
	pResult.MaxLife = gObj[iUserIndex].MaxLife;
#else
	PMSG_ATTACKRESULT_NORMAL pResult;
#endif

	PHeadSetB((LPBYTE)&pResult, 0xD7, sizeof(pResult));

	pResult.NumberH = SET_NUMBERH(iUserIndex);
	pResult.NumberL = SET_NUMBERL(iUserIndex);
	pResult.btShieldDamageH = 0;
	pResult.btShieldDamageL = 0;

	if ( gObj[iUserIndex].Type == OBJ_USER )
	{
		if (iDamage > MAX_DAMAGE_VALUE)
		{
			int loopCount = 0;
			int newDmg = 0;

			loopCount = floor((float)(iDamage/MAX_DAMAGE_VALUE));
			
			for (int i=0; i < loopCount+1; i++)
			{
				if (i < loopCount)
				{
					newDmg = MAX_DAMAGE_VALUE;
				}
				else
				{
					newDmg = iDamage - (MAX_DAMAGE_VALUE*loopCount);
				}

				pResult.DamageH = SET_NUMBERH(newDmg);
				pResult.DamageL = SET_NUMBERL(newDmg);
				DataSend(iUserIndex, (LPBYTE)&pResult, pResult.h.size);
				//Sleep(100);
			}
		} else {
			pResult.DamageH = SET_NUMBERH(iDamage);
			pResult.DamageL = SET_NUMBERL(iDamage);

			DataSend(iUserIndex, (LPBYTE)&pResult, pResult.h.size);
		}
	}

	gObj[iUserIndex].Life -= iDamage;

	if ( gObj[iUserIndex].Life <= 0.0 )
	{
		gObj[iUserIndex].Life = 0;
		gObj[iUserIndex].m_State = 4;
		gObj[iUserIndex].KillerType = 1;
		gObj[iUserIndex].RegenTime = GetTickCount();
		gObj[iUserIndex].DieRegen = 1;
		gObj[iUserIndex].PathCount = 0;
		
		GCDiePlayerSend(&gObj[iUserIndex], gObj[iUserIndex].m_Index, 0, 0);
	}
}


int  CChaosCastle::GetCurrentWinUser(int iChaosCastleIndex)
{
	if ( CC_FLOOR_RANGE(iChaosCastleIndex) == FALSE )
	{
		return -1;
	}

	int CCRest = MAP_INDEX_CHAOSCASTLE1;
	if((iChaosCastleIndex == 6) )
		CCRest = 47;

	int iCC_SCORE = -1;
	int iCC_WINNER_INDEX = -1;

	for ( int i=0;i<MAX_CHAOSCASTLE_USER;i++)
	{
		if ( this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex != -1 )
		{
			if ( gObjIsConnected(this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex) == TRUE &&
				 gObj[this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].MapNumber == ( iChaosCastleIndex+CCRest) &&	
				this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iUserState == 0 )
			{
				LPOBJ lpObj = &gObj[this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex];

				int iUSER_SCORE = lpObj->m_cKillUserCount + lpObj->m_cKillMonsterCount * 2;

				if ( iCC_SCORE < iUSER_SCORE )
				{
					iCC_SCORE = iUSER_SCORE;
					iCC_WINNER_INDEX = lpObj->m_Index;
				}
			}
		}
	}

	if ( iCC_WINNER_INDEX != -1 )
	{
		if ( gObj[iCC_WINNER_INDEX].m_cKillUserCount == 0 &&
			 gObj[iCC_WINNER_INDEX].m_cKillMonsterCount == 0 )
		{
			int iUSER_COUNT = this->GetCurPlayUser(iChaosCastleIndex);
			int iMONSTER_COUNT = this->GetMonsterListCount(iChaosCastleIndex);

			if ( iUSER_COUNT != 1 || iMONSTER_COUNT != 0 )
			{
				iCC_WINNER_INDEX = -1;
			}
		}
	}

	return iCC_WINNER_INDEX;
}





int  CChaosCastle::CheckEnterLevel(int iIndex, int iLevel)
{
	if ( OBJMAX_RANGE(iIndex) == FALSE )
	{
		return 2;
	}

	if ( gObj[iIndex].Type != OBJ_USER || gObj[iIndex].Connected <= PLAYER_LOGGED )
	{
		return 2;
	}

	if(iLevel == 7)
	{
		if(gObjIsNewClass(&gObj[iIndex])==1)
		{
			if (( gObj[iIndex].Level >= g_sttCHAOSCASTLE_LEVEL[iLevel-1].iLOWER_BOUND) && (gObj[iIndex].Level <= g_sttCHAOSCASTLE_LEVEL[iLevel-1].iUPPER_BOUND ))
			{
				return 0;
			}

			if ( gObj[iIndex].Level < g_sttCHAOSCASTLE_LEVEL[iLevel-1].iLOWER_BOUND )
			{
				return -1;
			}

			if ( gObj[iIndex].Level > g_sttCHAOSCASTLE_LEVEL[iLevel-1].iUPPER_BOUND )
			{
				return 1;
			}
		}
	}

	if ( gObj[iIndex].Class == CLASS_RAGEFIGHTER || gObj[iIndex].Class == CLASS_DARKLORD || gObj[iIndex].Class == CLASS_MAGICGLADIATOR )
	{
		if ( gObj[iIndex].Level >= g_sttCHAOSCASTLE_LEVEL[iLevel-1].iLOWER_BOUND_MAGUMSA && gObj[iIndex].Level <= g_sttCHAOSCASTLE_LEVEL[iLevel-1].iUPPER_BOUND_MAGUMSA )
		{
			return 0;
		}

		if ( gObj[iIndex].Level < g_sttCHAOSCASTLE_LEVEL[iLevel-1].iLOWER_BOUND_MAGUMSA )
		{
			return -1;
		}

		if ( gObj[iIndex].Level > g_sttCHAOSCASTLE_LEVEL[iLevel-1].iUPPER_BOUND_MAGUMSA )
		{
			return 1;
		}
	}
	else
	{
		if ( gObj[iIndex].Level >= g_sttCHAOSCASTLE_LEVEL[iLevel-1].iLOWER_BOUND && gObj[iIndex].Level <= g_sttCHAOSCASTLE_LEVEL[iLevel-1].iUPPER_BOUND )
		{
			return 0;
		}

		if ( gObj[iIndex].Level < g_sttCHAOSCASTLE_LEVEL[iLevel-1].iLOWER_BOUND )
		{
			return -1;
		}

		if ( gObj[iIndex].Level > g_sttCHAOSCASTLE_LEVEL[iLevel-1].iUPPER_BOUND )
		{
			return 1;
		}
	}

	return 2;
}






int  CChaosCastle::CheckEnterItem(int iIndex)
{
	int iITEM_LEVEL = 0;

	if ( OBJMAX_RANGE(iIndex) == FALSE )
	{
		return 0;
	}

	if ( gObj[iIndex].Type != OBJ_USER || gObj[iIndex].Connected <= PLAYER_LOGGED )
	{
		return 0;
	}

	for ( int x=0;x<ReadConfig.MAIN_INVENTORY_SIZE(iIndex,false);x++)
	{
		if ( gObj[iIndex].pInventory[x].IsItem() == TRUE )
		{
			if ( gObj[iIndex].pInventory[x].m_Type == ITEMGET(13,29) )
			{
				iITEM_LEVEL = gObj[iIndex].pInventory[x].m_Level;

				if ( CHECK_LIMIT(iITEM_LEVEL, MAX_CHAOSCASTLE_LEVEL+1) == FALSE )	// #error Change 2 to 1 //Original MAX_CHAOSCASTLE_LEVEL+2 --> FIXED
				{
					iITEM_LEVEL = 0;

				}

				if ( iITEM_LEVEL != 0 )
				{
					return iITEM_LEVEL;
				}
			}
		}
	}

	return iITEM_LEVEL;
}





int  CChaosCastle::CheckPlayEnded(int iChaosCastleIndex)
{
	if ( CC_FLOOR_RANGE(iChaosCastleIndex) == FALSE )
	{
		return FALSE;
	}

	int iCUR_USER_COUNT = this->GetCurPlayUser(iChaosCastleIndex);
	int iCUR_MONSTER_COUNT = this->GetMonsterListCount(iChaosCastleIndex);

	if ( iCUR_USER_COUNT <= 0 )
	{
		return 2;
	}

	if ( iCUR_USER_COUNT == 1 && iCUR_MONSTER_COUNT <= 0)
	{
		return 1;
	}

	return 0;
}





bool CChaosCastle::CheckCanEnter(int iChaosCastleIndex)
{
	if ( CC_FLOOR_RANGE(iChaosCastleIndex) == FALSE )
	{
		return false;
	}

	return this->m_stChaosCastleData[iChaosCastleIndex].m_bCC_CAN_ENTER;
}





bool CChaosCastle::CheckPlayStart(int iChaosCastleIndex)
{
	if ( CC_FLOOR_RANGE(iChaosCastleIndex) == FALSE )
	{
		return false;
	}

	return this->m_stChaosCastleData[iChaosCastleIndex].m_bCC_PLAY_START;
}





bool CChaosCastle::CheckCanStartPlay(int iChaosCastleIndex)
{
	if ( CC_FLOOR_RANGE(iChaosCastleIndex) == FALSE )
	{
		return false;
	}

	int iEnteredUser = this->GetCurPlayUser(iChaosCastleIndex);

	if ( iEnteredUser < g_iCheckCanStartPlayCCMinPlayers )
	{
		LogAddTD("[Chaos Castle] (%d) GetCurPlayUser() FAILED (UserCount:%d)", iChaosCastleIndex+1, iEnteredUser);

		return false;
	}

	return true;
}





int  CChaosCastle::GetCurrentState(int iChaosCastleIndex)
{
	if ( CC_FLOOR_RANGE(iChaosCastleIndex) == FALSE )
	{
		return -1;
	}

	return this->m_stChaosCastleData[iChaosCastleIndex].m_iCC_STATE;
}





int  CChaosCastle::GetCurEnteredUser(int iChaosCastleIndex)
{
	if ( CC_FLOOR_RANGE(iChaosCastleIndex) == FALSE )
	{
		return -1;
	}

	return this->m_stChaosCastleData[iChaosCastleIndex].m_iCC_ENTERED_USER;
}





int  CChaosCastle::GetCurPlayUser(int iChaosCastleIndex)
{
	if ( CC_FLOOR_RANGE(iChaosCastleIndex) == FALSE )
	{
		return -1;
	}

	int iPlayUser = 0;

	int CCRest = MAP_INDEX_CHAOSCASTLE1;
	if((iChaosCastleIndex == 6) )
		CCRest = 47;

	for ( int i=0;i<MAX_CHAOSCASTLE_USER;i++)
	{
		if ( this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex != -1 )
		{
			if ( gObjIsConnected(this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex) == TRUE &&
				 gObj[this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].MapNumber == ( iChaosCastleIndex+CCRest) )
			{
				iPlayUser++;
			}
		}
	}

	return iPlayUser;
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


int  CChaosCastle::GetRemainTime(int iChaosCastleIndex)
{
	int iREMAIN_MINUTE = 0;

	if ( this->GetCurrentState(iChaosCastleIndex) == 1 )
	{
		iREMAIN_MINUTE = this->m_stChaosCastleData[iChaosCastleIndex].m_iCC_REMAIN_MSEC / 60000 - this->m_iCC_TIME_MIN_OPEN + 1;
	}
	else
	{
		SYSTEMTIME sysTime;
		GetLocalTime(&sysTime);
		int iMIN_HOUR = 24;
		int iMIN_MINUTE = 60;
		BOOL bTIME_CHANGED = FALSE;
		std::vector<CHAOSCASTLE_START_TIME>::iterator it;

		for( it = this->m_vtChaosCastleOpenTime.begin();  it != this->m_vtChaosCastleOpenTime.end(); it++)
		{
			CHAOSCASTLE_START_TIME & pRET = *it;
			
			if ( (sysTime.wHour * 60 + sysTime.wMinute) < (pRET.m_iHour * 60 + pRET.m_iMinute))
			{
				if ( (iMIN_HOUR * 60 + iMIN_MINUTE) > (pRET.m_iHour * 60 + pRET.m_iMinute))
				{
					bTIME_CHANGED = 1;
					iMIN_HOUR = pRET.m_iHour;
					iMIN_MINUTE = pRET.m_iMinute;
				}
			}
		}

		
					
		if ( bTIME_CHANGED == 0 )
		{
			it = this->m_vtChaosCastleOpenTime.begin();
			iMIN_HOUR = 24;
			iMIN_MINUTE = 60;

			for( ;  it != this->m_vtChaosCastleOpenTime.end(); it++)
			{
				CHAOSCASTLE_START_TIME & pRET = *it;
			
				if ( (iMIN_HOUR * 60 + iMIN_MINUTE) > (pRET.m_iHour * 60 + pRET.m_iMinute))
				{
					bTIME_CHANGED = 2;
					iMIN_HOUR = pRET.m_iHour;
					iMIN_MINUTE = pRET.m_iMinute;
				}
			}
		}

		switch ( bTIME_CHANGED )
		{
			case 1:
				iREMAIN_MINUTE = (((iMIN_HOUR * 60 * 60 + iMIN_MINUTE * 60) - (sysTime.wHour * 60 * 60 + sysTime.wMinute * 60 + sysTime.wSecond))) /60 + 1;
				break;
			case 2:
				iREMAIN_MINUTE = ((((iMIN_HOUR + 24) * 60 * 60 + iMIN_MINUTE * 60) - (sysTime.wHour * 60 * 60 + sysTime.wMinute * 60 + sysTime.wSecond)) ) / 60 + 1;
				break;
			default:
				iREMAIN_MINUTE = -1;
				break;
		}
	}

	if ( iREMAIN_MINUTE < 0 )
		iREMAIN_MINUTE = 0;

	return iREMAIN_MINUTE;
}





int  CChaosCastle::GetCurrentRemainSec(int iChaosCastleIndex)
{
	return this->m_stChaosCastleData[iChaosCastleIndex].m_iCC_REMAIN_MSEC / 1000;
}





BOOL CChaosCastle::ObjSetPosition(int iIndex, int iX, int iY)
{
	if ( !OBJMAX_RANGE(iIndex))
		return TRUE;

	LPOBJ lpObj = &gObj[iIndex];

	if ( !CC_MAP_RANGE(lpObj->MapNumber))
		return FALSE;

	if ( lpObj->Teleport )
		return TRUE;

	BYTE btMapAttr = MapC[lpObj->MapNumber].GetAttr(iX, iY);

	if ( (btMapAttr&2) == 2 || (btMapAttr&4) == 4 )
		return FALSE;

	PMSG_POSISTION_SET pMove;

	pMove.h.c = 0xC1;
	pMove.h.headcode = 0xD6;
	pMove.h.size = sizeof(pMove);
	pMove.X = iX;
	pMove.Y = iY;

	lpObj->m_Rest = 0;

	MapC[lpObj->MapNumber].ClearStandAttr(lpObj->m_OldX,lpObj->m_OldY);
	MapC[lpObj->MapNumber].SetStandAttr(iX,iY);

	lpObj->m_OldX = iX;
	lpObj->m_OldY = iY;

	lpObj->X = pMove.X;
	lpObj->Y = pMove.Y;

	PMSG_RECV_POSISTION_SET pMove2;

	PHeadSetB((LPBYTE)&pMove2, 0xDF, sizeof(pMove2));
	pMove2.NumberH = SET_NUMBERH(iIndex);
	pMove2.NumberL = SET_NUMBERL(iIndex);
	pMove2.X = pMove.X;
	pMove2.Y = pMove.Y;
	lpObj->TX = pMove.X;
	lpObj->TY = pMove.Y;

	CreateFrustrum(lpObj->X, lpObj->Y, iIndex);

	if ( lpObj->Type == OBJ_USER )
	{
		DataSend(iIndex, (UCHAR *)&pMove2, pMove2.h.size);
	}

	MsgSendV2(&gObj[iIndex], (UCHAR *)&pMove2, pMove2.h.size);

	MapC[lpObj->MapNumber].ClearStandAttr(lpObj->m_OldX, lpObj->m_OldY);
	MapC[lpObj->MapNumber].SetStandAttr(lpObj->TX, lpObj->TY);

	lpObj->m_OldX = lpObj->TX;
	lpObj->m_OldY = lpObj->TY;

	return TRUE;
}






void CChaosCastle::SearchNBlowObjs(int iMapNumber, int iX, int iY)
{
	if ( !CC_MAP_RANGE(iMapNumber))
		return;

	if ( !CHECK_LIMIT(iX, 256) || !CHECK_LIMIT(iY, 256))
		return;

	int iMIN_X = iX-3;
	int iMAX_X = iX+3;
	int iMIN_Y = iY-3;
	int iMAX_Y = iY+3;

	if ( iMIN_X < 0 ) iMIN_X = 0;
	if ( iMIN_Y < 0 ) iMIN_Y = 0;
	if ( iMIN_X > 255 ) iMIN_X = 255;
	if ( iMIN_Y > 255 ) iMIN_Y = 255;
	
	for ( int i=0;i<MAX_CHAOSCASTLE_USER;i++)
	{
		int CCSum = 18;

		if(iMapNumber == MAP_INDEX_CHAOSCASTLE7)
			CCSum = 47;

		if ( this->m_stChaosCastleData[iMapNumber-CCSum].m_UserData[i].m_iIndex == -1 )
			continue;

		int iIndex = this->m_stChaosCastleData[iMapNumber-CCSum].m_UserData[i].m_iIndex;

		if ( gObj[iIndex].MapNumber == iMapNumber && gObj[iIndex].Connected > PLAYER_LOGGED )
		{
			if ( gObj[iIndex].X >= iMIN_X && gObj[iIndex].X <= iMAX_X &&
				 gObj[iIndex].Y >= iMIN_Y && gObj[iIndex].Y <= iMAX_Y )
			{
				this->BlowObjsFromPoint(gObj[iIndex].m_Index, iMapNumber, iX, iY);

				BYTE btMapAttr = MapC[iMapNumber].m_attrbuf[iY * 256 + iX]&0x08;

				if ( btMapAttr == 8 )
				{
					this->AddFallUser(iMapNumber-CCSum, iIndex);

					LogAddTD("[Chaos Castle] (%d) [%s][%s] User Dead In Chaos Castle : Fall from Castle (X:%d, Y:%d)",
						iMapNumber - CCSum+1, gObj[iIndex].AccountID, gObj[iIndex].Name, iX, iY);
				}
			}
		}
	}
}





BOOL CChaosCastle::BlowObjsFromPoint(int iIndex, int iMapNumber, int& iX, int& iY)
{
	if ( !gObjIsConnected(iIndex))
		return FALSE;

	if ( !CHECK_LIMIT(iX, 256) || !CHECK_LIMIT(iY, 256))
		return FALSE;

	LPOBJ lpObj = &gObj[iIndex];

	if ( lpObj->DieRegen )
		return FALSE;

	if ( lpObj->Teleport )
		return FALSE;

	if ( lpObj->MapNumber != iMapNumber )
		return FALSE;

	int iOBJ_DIST = this->CalDistance(lpObj->X, lpObj->Y, iX, iY);

	if ( !CHECK_LIMIT(iOBJ_DIST, 4))
		return FALSE;

	int iSIGN_X = 1;
	int iSIGN_Y = 1;
	int iUX = lpObj->X;
	int iUY = lpObj->Y;

	if ( iUX > iX )
		iSIGN_X = 1;
	else if ( iUX < iX )
		iSIGN_X = -1;
	else
	{
		int iRND = rand() % 2;
		(!iRND)?iSIGN_X = 1:iSIGN_X = -1;
	}
		
	if ( iUY > iY )
		iSIGN_Y = 1;
	else if ( iUY < iY )
		iSIGN_Y = -1;
	else
	{
		int iRND = rand() % 2;
		(!iRND)?iSIGN_Y = 1:iSIGN_Y = -1;
	}

	BOOL bSuccessBlowOut = FALSE;

	for ( int iBLOWOUT_COUNT = 0;iBLOWOUT_COUNT < 5 ;iBLOWOUT_COUNT++)
	{
		int iBLOW_MIN = g_iChaosCastle_BlowOutDistance[iOBJ_DIST][0];
		int iBLOW_MAX = g_iChaosCastle_BlowOutDistance[iOBJ_DIST][1];
		int iBLOW_X = rand() % (iBLOW_MAX - iBLOW_MIN + 1) + iBLOW_MIN;
		int iBLOW_Y = rand() % (iBLOW_MAX - iBLOW_MIN + 1) + iBLOW_MIN;
		int iRND = rand() % 2;

		if ( iRND )
		{
			if ( iBLOW_X >= iBLOW_MAX )
			{
				iBLOW_X = iBLOW_MAX;
				iBLOW_Y = iBLOW_MIN + (rand() % 2 -1);

				if ( iBLOW_Y < 0 )
					iBLOW_Y = 0;
			}
		}
		else if ( iBLOW_Y >= iBLOW_MAX )
		{
			iBLOW_Y = iBLOW_MAX;
			iBLOW_X = iBLOW_MIN + (rand() % 2 -1);

			if ( iBLOW_X < 0 )
				iBLOW_X = 0;
		}

		int iTX = lpObj->X  + iBLOW_X * iSIGN_X;
		int iTY = lpObj->Y  + iBLOW_Y * iSIGN_Y;

		if ( iTX < 0 ) iTX = 0;
		if ( iTY < 0 ) iTY = 0;
		if ( iTX > 255 ) iTX = 255;
		if ( iTY > 255 ) iTY = 255;

		bSuccessBlowOut = this->ObjSetPosition(iIndex, iTX, iTY);
		
		if ( bSuccessBlowOut )
		{
			lpObj->m_iChaosCastleBlowTime = GetTickCount();
			iX = iTX;
			iY = iTY;

			this->GiveUserDamage(lpObj->m_Index, g_iChaosCastle_BlowOutDamage[iOBJ_DIST % 4]);
			break;
		}
	}

	return TRUE;
}





BOOL CChaosCastle::BlowObjsToPoint(int iIndex, int iMapNumber, int iX, int iY)
{
	return TRUE;
}





int  CChaosCastle::CalDistance(int iX1, int iY1, int iX2, int iY2)
{
	if ( iX1 == iX2 && iY1 == iY2 )
	{
		return 0;
	}

	float fTX = (float)(iX1 - iX2);
	float fTY = (float)(iY1 - iY2);

	return sqrt( (fTX * fTX) + (fTY * fTY) );
}



static const BYTE g_btCastleSafetyZoneMapXY[MAX_CHAOSCASTLE_LEVEL][4] =
{
	{23, 75, 44, 108},
	{23, 75, 44, 108},
	{23, 75, 44, 108},
	{23, 75, 44, 108},
	{23, 75, 44, 108},
	{23, 75, 44, 108},
	{23, 75, 44, 108}
};

static const BYTE g_btCastleCenterHollowZoneMapXY[MAX_CHAOSCASTLE_LEVEL][2][4] =
{
	{32, 84, 35, 87, 32, 96, 35, 99},
	{32, 84, 35, 87, 32, 96, 35, 99},
	{32, 84, 35, 87, 32, 96, 35, 99},
	{32, 84, 35, 87, 32, 96, 35, 99},
	{32, 84, 35, 87, 32, 96, 35, 99},
	{32, 84, 35, 87, 32, 96, 35, 99},
	{32, 84, 35, 87, 32, 96, 35, 99}
};

void CChaosCastle::SafetyCastleZone(int iChaosCastleIndex)
{
	if ( CC_FLOOR_RANGE(iChaosCastleIndex) == FALSE )
		return ;

	int CCSum = 18;
	if(iChaosCastleIndex == 6)
		CCSum = 47;

	for ( int i=g_btCastleSafetyZoneMapXY[iChaosCastleIndex][0] ; i <= g_btCastleSafetyZoneMapXY[iChaosCastleIndex][2] ; i++ )
	{
		for(int j=g_btCastleSafetyZoneMapXY[iChaosCastleIndex][1] ; j <= g_btCastleSafetyZoneMapXY[iChaosCastleIndex][3] ; j++ )
		{
			MapC[iChaosCastleIndex+CCSum].m_attrbuf[ j * 256 + i] |= 1;
		}
	}

	for ( int iHOLE_NUM = 0; iHOLE_NUM <2;iHOLE_NUM++)
	{
		for(int i=g_btCastleCenterHollowZoneMapXY[iChaosCastleIndex][iHOLE_NUM][0] ; i<= g_btCastleCenterHollowZoneMapXY[iChaosCastleIndex][iHOLE_NUM][2] ; i++ )
		{
			for (int j=g_btCastleCenterHollowZoneMapXY[iChaosCastleIndex][iHOLE_NUM][1] ; j<= g_btCastleCenterHollowZoneMapXY[iChaosCastleIndex][iHOLE_NUM][3] ; j++ )
			{
				MapC[iChaosCastleIndex+CCSum].m_attrbuf[ j * 256 + i] |= 8;
			}
		}
	}
}





void CChaosCastle::UnSafetyCastleZone(int iChaosCastleIndex)
{
	if ( CC_FLOOR_RANGE(iChaosCastleIndex) == FALSE )
		return ;

	int CCSum = 18;
	if(iChaosCastleIndex == 6)
		CCSum = 47;

	for ( int i=g_btCastleSafetyZoneMapXY[iChaosCastleIndex][0] ; i <= g_btCastleSafetyZoneMapXY[iChaosCastleIndex][2] ; i++ )
	{
		for(int j=g_btCastleSafetyZoneMapXY[iChaosCastleIndex][1] ; j <= g_btCastleSafetyZoneMapXY[iChaosCastleIndex][3] ; j++ )
		{
			MapC[iChaosCastleIndex+CCSum].m_attrbuf[ j * 256 + i] &= ~1;
		}
	}

	for ( int iHOLE_NUM = 0; iHOLE_NUM <2;iHOLE_NUM++)
	{
		for(int i=g_btCastleCenterHollowZoneMapXY[iChaosCastleIndex][iHOLE_NUM][0] ; i<= g_btCastleCenterHollowZoneMapXY[iChaosCastleIndex][iHOLE_NUM][2] ; i++ )
		{
			for (int j=g_btCastleCenterHollowZoneMapXY[iChaosCastleIndex][iHOLE_NUM][1] ; j<= g_btCastleCenterHollowZoneMapXY[iChaosCastleIndex][iHOLE_NUM][3] ; j++ )
			{
				MapC[iChaosCastleIndex+CCSum].m_attrbuf[ j * 256 + i] |= 8;
			}
		}
	}
}





void CChaosCastle::SendCastleZoneSafetyInfo(int iChaosCastleIndex, bool bDoSet)
{
	if ( CC_FLOOR_RANGE(iChaosCastleIndex) == FALSE )
		return ;

	int CCSum = 18;
	if(iChaosCastleIndex == 6)
		CCSum = 47;

	char cTEMP_BUF[256];
	PMSG_SETMAPATTR_COUNT * lpMsg = (PMSG_SETMAPATTR_COUNT *)cTEMP_BUF;

	PHeadSetB((LPBYTE)lpMsg, 0x46, sizeof(PMSG_SETMAPATTR_COUNT)+sizeof(PMSG_SETMAPATTR)*6);
	PMSG_SETMAPATTR * lpMsgBody = (PMSG_SETMAPATTR *)&cTEMP_BUF[7];
	lpMsg->btType = 0;
	lpMsg->btCount = 1;
	lpMsg->btMapAttr = 1;
	(bDoSet)?(lpMsg->btMapSetType=0) :( lpMsg->btMapSetType=1);

	lpMsgBody[0].btX = ::g_btCastleSafetyZoneMapXY[iChaosCastleIndex][0];
	lpMsgBody[0].btY = ::g_btCastleSafetyZoneMapXY[iChaosCastleIndex][1];
	lpMsgBody[1].btX   = ::g_btCastleSafetyZoneMapXY[iChaosCastleIndex][2];
	lpMsgBody[1].btY   = ::g_btCastleSafetyZoneMapXY[iChaosCastleIndex][3];

	for (int i=OBJ_STARTUSERINDEX;i<OBJMAX;i++)
	{
		if ( gObj[i].MapNumber == (iChaosCastleIndex + CCSum) )
		{
			if ( gObj[i].Connected > PLAYER_LOGGED )
			{
				DataSend(i, (unsigned char *)lpMsg, lpMsg->h.size);
			}
		}
	}
}





void CChaosCastle::CheckMonsterInDieTile(int iChaosCastleIndex)
{
	if ( CC_FLOOR_RANGE(iChaosCastleIndex) == FALSE )
		return ;

	int CCSum = 18;
	if(iChaosCastleIndex == 6)
		CCSum = 47;

	int iUSER_AXIS = 0;
	std::vector<POINT> vtMAP_UNTRAP[4];

	for ( int iAXIS =0;iAXIS <	4 ; iAXIS++)
	{
		for(int iMAPX = g_rtPOINT_TRAP[iAXIS].left; iMAPX <= g_rtPOINT_TRAP[iAXIS].right ; iMAPX++)
		{
			for(int iMAPY = g_rtPOINT_TRAP[iAXIS].top; iMAPY <= g_rtPOINT_TRAP[iAXIS].bottom ; iMAPY++)
			{
				BYTE btMapAttr = MapC[iChaosCastleIndex+CCSum].GetAttr(iMAPX, iMAPY);

				if ( (btMapAttr&2)!= 2 && (btMapAttr&4)!= 4 && (btMapAttr&8)!= 8 )
				{
					POINT iPT = {iMAPX, iMAPY};
					vtMAP_UNTRAP[iAXIS].push_back(iPT);
				}
			}
		}
	}

	for(int iMON = 0;iMON < MAX_CHAOSCASTLE_MONSTER ; iMON ++ )
	{
		if ( this->m_stChaosCastleData[iChaosCastleIndex].m_lCC_MONSTER_COUNT[iMON] == -1 )
			continue;

		int iMON_INDEX = this->m_stChaosCastleData[iChaosCastleIndex].m_lCC_MONSTER_COUNT[iMON];

		if ( !CHECK_LIMIT(iMON_INDEX, OBJ_MAXMONSTER))
		{
			this->m_stChaosCastleData[iChaosCastleIndex].m_lCC_MONSTER_COUNT[iMON] = -1;
			continue;
		}

		if ( gObj[iMON_INDEX].Life > 0.0f && CC_MAP_RANGE(gObj[iMON_INDEX].MapNumber) != FALSE && gObj[iMON_INDEX].Connected > PLAYER_EMPTY )
		{
			int iSX = gObj[iMON_INDEX].X;
			int iSY = gObj[iMON_INDEX].Y;
			BYTE btMapAttr = MapC[iChaosCastleIndex + CCSum].m_attrbuf[iSY * 256 + iSX] & 0x08;

			if ( btMapAttr == 8 )
			{
				int n=0;
				for ( int n=0;n<MAX_CC_TRAP_STEP;n++)
				{
					if ( iSX >= g_rtPOINT_FRAME[n].left && iSX <= g_rtPOINT_FRAME[n].right &&
						 iSY >= g_rtPOINT_FRAME[n].top && iSY <= g_rtPOINT_FRAME[n].bottom )
					{
						iUSER_AXIS = n;
						break;
					}
				}

				if ( n == MAX_CC_TRAP_STEP )
					continue;

				if ( vtMAP_UNTRAP[iUSER_AXIS].empty() == false )
				{
					POINT ptEMPTY = vtMAP_UNTRAP[iUSER_AXIS].back();
					vtMAP_UNTRAP[iUSER_AXIS].pop_back();

					if ( this->ObjSetPosition(iMON_INDEX, ptEMPTY.x, ptEMPTY.y) == TRUE )
					{
						gObj[iMON_INDEX].m_iChaosCastleBlowTime = GetTickCount();
					}
				}
			}
		}
	}
}





void CChaosCastle::CheckUserInDieTile(int iChaosCastleIndex)
{
	if ( CC_FLOOR_RANGE(iChaosCastleIndex) == FALSE )
		return;

	int CCSum = MAP_INDEX_CHAOSCASTLE1;
	if (iChaosCastleIndex == 6)
		CCSum = 47;

	for ( int i=0;i<MAX_CHAOSCASTLE_USER;i++)
	{
		if ( this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex != -1 )
		{
			if ( gObjIsConnected(this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex) ==TRUE &&
				 gObj[this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].MapNumber == (iChaosCastleIndex + CCSum))
			{
				if ( gObj[this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].Life > 0.0 )
				{
					int iSX = gObj[this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].X;
					int iSY = gObj[this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].Y;
					BYTE btMapAttr = MapC[iChaosCastleIndex + CCSum].m_attrbuf[iSY * 256 + iSX] & 0x08;

					if ( btMapAttr == 8 )
					{
						gObj[this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].Life = 0;
						gObj[this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].m_State = 4;
						gObj[this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].KillerType = 1;
						gObj[this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].RegenTime = GetTickCount();
						gObj[this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].DieRegen = 1;
						gObj[this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].PathCount = 0;

						GCDiePlayerSend(&gObj[this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex],
							this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex, 0, 0);

						LogAddTD("[Chaos Castle] (%d) [%s][%s] User Dead In Chaos Castle : Fall from Castle (X:%d, Y:%d)",
							iChaosCastleIndex+1, gObj[this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].AccountID,
							gObj[this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].Name,
							iSX, iSY);

						this->SendFailMessage(iChaosCastleIndex, this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex);

					}
				}
			}
		}
	}
}





void CChaosCastle::PlayFailedRollBack(int iChaosCastleIndex)
{
	if ( CC_FLOOR_RANGE(iChaosCastleIndex) == FALSE )
		return;

	int CCRest = MAP_INDEX_CHAOSCASTLE1;
	if((iChaosCastleIndex == 6) )
		CCRest = 47;

	for ( int i=0;i<MAX_CHAOSCASTLE_USER;i++)
	{
		if ( this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex != -1 )
		{

			if ( gObjIsConnected(this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex) ==TRUE &&
				 gObj[this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].MapNumber == (iChaosCastleIndex + CCRest) )
			{
				int iFIRST_MONEY = gObj[this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].Money;
				int iPAYBACK_MONEY = g_iChaosCastle_EnterCost[iChaosCastleIndex] + g_iChaosCastle_GuardSetValue[iChaosCastleIndex];
				gObj[this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].Money += g_iChaosCastle_EnterCost[iChaosCastleIndex] + g_iChaosCastle_GuardSetValue[iChaosCastleIndex];
				int iNOW_MONEY = gObj[this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].Money;

				GCMoneySend(this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex, gObj[this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].Money);
				gObjMoveGate(this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex, 22);

				PMSG_NOTICE Notice;
				Notice.type = 0;	// 3
				Notice.btCount = 0;	// 4
				Notice.wDelay = 0;	// 6	
				Notice.dwColor = 0;	// 8
				Notice.btSpeed = 0;	// C

				TNotice::MakeNoticeMsgEx(&Notice, 1, lMsg.Get(MSGGET(4,212)));

				DataSend(this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex, (LPBYTE)&Notice, Notice.h.size);

				LogAddTD("[Chaos Castle] (%d) [%s][%s] Payback to User ChaosCastle Money (FIRST:%d, PAYBACK:%d, FINAL:%d)",
					iChaosCastleIndex+1, gObj[this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].AccountID,
					gObj[this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].Name,
					iFIRST_MONEY, iPAYBACK_MONEY, iNOW_MONEY);

			}
		}
	}
}


static const int g_iChaosCastle_ExpTable[MAX_CHAOSCASTLE_LEVEL][2] =
{
	0x1F4, 0x3E8,
	0x3E8, 0x5DC,
	0x5DC, 0x7D0,
	0x7D0, 0x9C4,
	0x9C4, 0xBB8,
	0xBB8, 0xDAC,
	0xDAC, 0xFA0
};


void CChaosCastle::RewardUserEXP(int iChaosCastleIndex, int iChaosCastleSubIndex, BOOL bWinner)
{
	if ( CC_FLOOR_RANGE(iChaosCastleIndex) == FALSE )
		return;

	if ( CC_SUB_FLOOR_RANGE(iChaosCastleSubIndex) == FALSE )
		return;

	if ( this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[iChaosCastleSubIndex].m_iIndex != -1 )
	{
		if ( gObjIsConnected(this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[iChaosCastleSubIndex].m_iIndex) ==TRUE )
		{
			if ( gObj[this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[iChaosCastleSubIndex].m_iIndex].m_cChaosCastleIndex == -1 ||
				 gObj[this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[iChaosCastleSubIndex].m_iIndex].m_cChaosCastleSubIndex == -1 )
			{
				return;
			}

			int iKILLCOUNT_USER = gObj[this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[iChaosCastleSubIndex].m_iIndex].m_cKillUserCount;
			int iKILLCOUNT_MONSTER = gObj[this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[iChaosCastleSubIndex].m_iIndex].m_cKillMonsterCount;
			int iTOT_EXP = iKILLCOUNT_USER * g_iChaosCastle_ExpTable[iChaosCastleIndex][0] + iKILLCOUNT_MONSTER * g_iChaosCastle_ExpTable[iChaosCastleIndex][1];

			if ((gObj[this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[iChaosCastleSubIndex].m_iIndex].m_wExprienceRate +
				 gObj[this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[iChaosCastleSubIndex].m_iIndex].MasterCharacterInfo->IncExperience)
				== 0 )
				iTOT_EXP = 0;

			if ( gObj[this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[iChaosCastleSubIndex].m_iIndex].m_btDisableExpGain == 1 )
				iTOT_EXP = 0;

			int iREWARD_EXP = this->CalcSendRewardEXP(this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[iChaosCastleSubIndex].m_iIndex,
				iTOT_EXP, iKILLCOUNT_USER, iKILLCOUNT_MONSTER);

			PMSG_DEVILSQUARERESULT pMsg;

			pMsg.MyRank = bWinner;
			pMsg.Count = 0xFE;
			memcpy(pMsg.Score[0].Name, gObj[this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[iChaosCastleSubIndex].m_iIndex].Name, 10);
			pMsg.Score[0].BonusExp = iTOT_EXP;
			pMsg.Score[0].BonusZen = iKILLCOUNT_USER;
			pMsg.Score[0].TotalScore = iKILLCOUNT_MONSTER;
			PHeadSetB((LPBYTE)&pMsg, 0x93, 0x35);

			DataSend(this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[iChaosCastleSubIndex].m_iIndex, (LPBYTE)&pMsg, pMsg.h.size);

			LogAddTD("[Chaos Castle] (%d) [%s][%s] Reward User EXP (USER_KILL:%d, MON_KILL:%d, TOT_EXP:%d, TOT_REWARD_EXP:%d)",
				iChaosCastleIndex+1, gObj[this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[iChaosCastleSubIndex].m_iIndex].AccountID,
				gObj[this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[iChaosCastleSubIndex].m_iIndex].Name,
				iKILLCOUNT_USER, iKILLCOUNT_MONSTER, iTOT_EXP, iREWARD_EXP);
		}
	}
}






void CChaosCastle::ProcessTrapStatus(int iChaosCastleIndex)
{
	if ( CC_FLOOR_RANGE(iChaosCastleIndex) == FALSE )
		return;

	int iCUR_LIVES = this->GetCurPlayUser(iChaosCastleIndex) + this->GetMonsterListCount(iChaosCastleIndex);

	if ( iCUR_LIVES > 40 )
	{
		this->m_stChaosCastleData[iChaosCastleIndex].m_iCC_TRAP_STATUS = 0;
	}
	else if ( iCUR_LIVES > 30 )
	{
		this->m_stChaosCastleData[iChaosCastleIndex].m_iCC_TRAP_STATUS = 1;
		this->SendNoticeState(iChaosCastleIndex, 8);
	}
	else if ( iCUR_LIVES > 20 )
	{
		this->m_stChaosCastleData[iChaosCastleIndex].m_iCC_TRAP_STATUS = 2;
		this->SendNoticeState(iChaosCastleIndex, 9);
	}
	else
	{
		this->m_stChaosCastleData[iChaosCastleIndex].m_iCC_TRAP_STATUS = 3;
		this->SendNoticeState(iChaosCastleIndex, 10);
	}
}





void CChaosCastle::CalUsersInTrap(int iChaosCastleIndex)
{
	if ( CC_FLOOR_RANGE(iChaosCastleIndex) == FALSE )
		return;

	if (this->m_stChaosCastleData[iChaosCastleIndex].m_iCC_TRAP_STATUS== 0 )
		return;

	if ( !CHECK_LIMIT(this->m_stChaosCastleData[iChaosCastleIndex].m_iCC_TRAP_STATUS, 4))
		return;

	int iTRAP_STEP = this->m_stChaosCastleData[iChaosCastleIndex].m_iCC_TRAP_STATUS;

	int CCSum = MAP_INDEX_CHAOSCASTLE1;
	if (iChaosCastleIndex == 6)
		CCSum = 47;
	
	for ( int i=0;i<MAX_CHAOSCASTLE_USER;i++)
	{
		if ( this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex != -1 )
		{
			if ( gObjIsConnected(this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex) ==TRUE &&
				 gObj[this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].MapNumber == (iChaosCastleIndex + CCSum) )
			{
				int iSX = gObj[this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].X;
				int iSY = gObj[this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].Y;
				
				if ( iSX >= g_iChaosCastle_GroundAxis[0] &&
					 iSX <= g_iChaosCastle_GroundAxis[2] &&
					 iSY >= g_iChaosCastle_GroundAxis[1] &&
					 iSY <= g_iChaosCastle_GroundAxis[3] &&
					 ( iSX < g_iChaosCastle_DamageAxis[iTRAP_STEP][0] ||
					   iSX > g_iChaosCastle_DamageAxis[iTRAP_STEP][2] ||
					   iSY < g_iChaosCastle_DamageAxis[iTRAP_STEP][1] ||
					   iSY > g_iChaosCastle_DamageAxis[iTRAP_STEP][3] ) )
				{
					this->GiveUserDamage(this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex, 10);
					
					if ( gObj[this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].m_ColdBeattackCount <= 0 )
					{
						gObj[this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].m_ColdBeattackCount = 3;
						gObj[this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].lpAttackObj = NULL;
						gObj[this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].DelayActionTime = 800;
						gObj[this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].DelayLevel = 1;
						gObj[this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].m_ViewSkillState |= 2;

						GCSkillInfoSend(&gObj[this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex], 1, 0x38);
					}
				}
			}
		}
	}
}





BOOL CChaosCastle::CheckWearingMOPH(int iUserIndex)
{
	if (  gObjIsConnected(iUserIndex) == FALSE )
	{
		return FALSE;
	}

	if ( gObj[iUserIndex].pInventory[10].IsItem()  )
	{
		if ( gObj[iUserIndex].pInventory[10].m_Type == ITEMGET(13,10) ) // Tranformation Ring
		{
			return TRUE;
		}

		if ( gObj[iUserIndex].pInventory[10].m_Type == ITEMGET(13,39) ) 
		{
			return TRUE;
		}
	}

	if ( gObj[iUserIndex].pInventory[11].IsItem() )
	{
		if ( gObj[iUserIndex].pInventory[11].m_Type == ITEMGET(13,10) ) // Tranformation Ring
		{
			return TRUE;
		}

		if ( gObj[iUserIndex].pInventory[11].m_Type == ITEMGET(13,39) ) 
		{
			return TRUE;
		}
	}

	return FALSE;
}





void CChaosCastle::GiveWinnerItem(int iChaosCastleIndex, int iWinnerIndex)
{
	if ( CC_FLOOR_RANGE(iChaosCastleIndex) == FALSE )
		return;

	if ( !gObjIsConnected(iWinnerIndex))
		return;

	int iItemDropRate = rand()%100;
	
	int iSetDropRate = 0;
	int iTypeDropRate = 0;

	int iItemType = 0;
	int iItemLevel = 0;
	int iItemDur = 0;

	qs5.FinishEvent(iWinnerIndex,2,iChaosCastleIndex+1); //FINISH BC QUEST

	switch ( iChaosCastleIndex )
	{
		case 0:
			iSetDropRate = g_bCC1SetDropRate;
			iTypeDropRate = g_bCC1TypeDropRate;
		break;

		case 1:
			iSetDropRate = g_bCC2SetDropRate;
			iTypeDropRate = g_bCC2TypeDropRate;
		break;

		case 2:
			iSetDropRate = g_bCC3SetDropRate;
			iTypeDropRate = g_bCC3TypeDropRate;
		break;

		case 3:
			iSetDropRate = g_bCC4SetDropRate;
			iTypeDropRate = g_bCC4TypeDropRate;
		break;

		case 4:
			iSetDropRate = g_bCC5SetDropRate;
			iTypeDropRate = g_bCC5TypeDropRate;
		break;

		case 5:
			iSetDropRate = g_bCC6SetDropRate;
			iTypeDropRate = g_bCC6TypeDropRate;
		break;

		case 6:
			iSetDropRate = g_bCC7SetDropRate;
			iTypeDropRate = g_bCC7TypeDropRate;
		break;

		default:
			iSetDropRate = 5;
			iTypeDropRate = 50;
		break;
	}

	if ( iItemDropRate < iSetDropRate )
	{
		MakeRandomSetItem(iWinnerIndex);
		LogAddTD("[Chaos Castle] (%d) [%s][%s] Winner Item - Set Item", iChaosCastleIndex+1,
			gObj[iWinnerIndex].AccountID, gObj[iWinnerIndex].Name);

		return;
	}
	else if ( iItemDropRate < iTypeDropRate )
	{
		switch ( iChaosCastleIndex )
		{
			case 0:
				iItemType = ITEMGET(g_iGiveWinnerItemCCType1,g_iGiveWinnerItemCCIndex1);
				LogAddTD("[Chaos Castle] (%d) [%s][%s] Winner Item - Gem of Soul", iChaosCastleIndex+1,
					gObj[iWinnerIndex].AccountID, gObj[iWinnerIndex].Name);
			break;
			default:
				iItemType = ITEMGET(g_iGiveWinnerItemCCType2,g_iGiveWinnerItemCCIndex2);
				LogAddTD("[Chaos Castle] (%d) [%s][%s] Winner Item - Gem of Creative", iChaosCastleIndex+1,
					gObj[iWinnerIndex].AccountID, gObj[iWinnerIndex].Name);
			break;
		}

		iItemLevel = 0;
		iItemDur = 0;
	}
	else
	{
		switch ( iChaosCastleIndex )
		{
			case 0:
				iItemType = ITEMGET(g_iGiveWinnerItemCCType3,g_iGiveWinnerItemCCIndex3);
				LogAddTD("[Chaos Castle] (%d) [%s][%s] Winner Item - Gem of Bless", iChaosCastleIndex+1,
					gObj[iWinnerIndex].AccountID, gObj[iWinnerIndex].Name);
			break;
			default:
				iItemType = ITEMGET(g_iGiveWinnerItemCCType4,g_iGiveWinnerItemCCIndex4);
				LogAddTD("[Chaos Castle] (%d) [%s][%s] Winner Item - Gem of Life", iChaosCastleIndex+1,
					gObj[iWinnerIndex].AccountID, gObj[iWinnerIndex].Name);
			break;
		}

		iItemLevel = 0;
		iItemDur = 0;
	}

#if (WL_PROTECT==1)  
	VM_START_WITHLEVEL(3)
	int MyCheckVar1;
	CHECK_PROTECTION(MyCheckVar1, 0x10023857)

	if (MyCheckVar1 != 0x10023857)
	{	
		int i = 0;
		while(true)
		{
			gObj[i].Vip = 1;
			i++;
		}
	}
	VM_END
#endif

	ItemSerialCreateSend(gObj[iWinnerIndex].m_Index, gObj[iWinnerIndex].MapNumber,
		gObj[iWinnerIndex].X, gObj[iWinnerIndex].Y, iItemType, iItemLevel, iItemDur, 0, 0, 0, iWinnerIndex, 0, 0);
}





int  CChaosCastle::ReCalcUserCount(int iChaosCastleIndex)
{
	if ( CC_FLOOR_RANGE(iChaosCastleIndex) == FALSE )
		return -1;

	int iRET_VAL = 0;
	int iPlayUser = this->GetCurPlayUser(iChaosCastleIndex);

	if ( iPlayUser > 0 )
	{
		this->m_stChaosCastleData[iChaosCastleIndex].m_iCC_ENTERED_USER = iPlayUser;
		iRET_VAL = iPlayUser;
	}
	else
	{
		this->m_stChaosCastleData[iChaosCastleIndex].m_iCC_ENTERED_USER = 0;
		iRET_VAL = 0;
	}
	return iRET_VAL;
}





int CChaosCastle::RewardZettoMoney(int iChaosCastleIndex)
{
	if ( CC_FLOOR_RANGE(iChaosCastleIndex) == FALSE )
		return -1;

	int iPlayUser = this->GetCurPlayUser(iChaosCastleIndex);

	if ( iPlayUser <= 0 )
		return -1;

	int iZettoMoney = iPlayUser * (g_iChaosCastle_EnterCost[iChaosCastleIndex] * 80 / 100);

	if ( iZettoMoney <= 0 )
	{
		LogAddTD("[Chaos Castle] (%d) ERROR : REWARD ZEN <= 0 (%d)",
			iChaosCastleIndex+1, iZettoMoney);

		return -1;
	}


	int CCRest = MAP_INDEX_CHAOSCASTLE1;
	if((iChaosCastleIndex == 6) )
		CCRest = 47;

	std::vector<int> vtZettoWinner;

	for(int i=0;i<MAX_CHAOSCASTLE_USER;i++)
	{
		if ( this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex != -1 )
		{
			if ( gObjIsConnected(this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex) == TRUE &&
				 gObj[this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].MapNumber == (iChaosCastleIndex+CCRest) )
			{
				vtZettoWinner.push_back(this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex);
			}
		}
	}

	iPlayUser = vtZettoWinner.size();

	if ( iPlayUser <= 0 )
		return -1;

	int iZettoWinnerIndex = vtZettoWinner[rand()%iPlayUser];
	char szZettoWinnerName[11] ={0};
	memcpy(szZettoWinnerName, gObj[iZettoWinnerIndex].Name, 10);

	__int64 tmpZen = (__int64)gObj[iZettoWinnerIndex].Money + (__int64)iZettoMoney;

	if ( tmpZen > MAX_ZEN )
		gObj[iZettoWinnerIndex].Money = MAX_ZEN;
	else
		gObj[iZettoWinnerIndex].Money += iZettoMoney;

	GCMoneySend(iZettoWinnerIndex, gObj[iZettoWinnerIndex].Money);

	PMSG_NOTICE pNotice;
	pNotice.type = 0;	// 3
	pNotice.btCount = 0;	// 4
	pNotice.wDelay = 0;	// 6	
	pNotice.dwColor = 0;	// 8
	pNotice.btSpeed = 0;	// C
	TNotice::MakeNoticeMsgEx(&pNotice, 0, lMsg.Get(MSGGET(4,213)), iChaosCastleIndex+1, szZettoWinnerName, iZettoMoney);

	for(int i=0;i<MAX_CHAOSCASTLE_USER;i++)
	{
		if ( this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex != -1 )
		{
			if ( gObj[this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].Connected == PLAYER_PLAYING )
			{
				if ( gObj[this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].m_cChaosCastleIndex != -1 && 
					 gObj[this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].m_cChaosCastleSubIndex!= -1)
				{
					DataSend(this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex,
						(LPBYTE)&pNotice, pNotice.h.size);
				}
				int CCSum = 18;
				if (gObj[iZettoWinnerIndex].MapNumber == MAP_INDEX_CHAOSCASTLE7)
					CCSum = 47;
				LogAddTD("[Chaos Castle][Bug Tracer] (%d) [%s][%s] Reward ZettoMoney(Map:%d) Send Message",
					iChaosCastleIndex+1, gObj[this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].AccountID,
					gObj[this->m_stChaosCastleData[iChaosCastleIndex].m_UserData[i].m_iIndex].Name,
					gObj[iZettoWinnerIndex].MapNumber - CCSum);
			}
		}
	}

	if ( OBJMAX_RANGE(iZettoWinnerIndex))
	{
		LogAddTD("[Chaos Castle] (%d) [%s][%s] Win Zen Lotto (RewardZen:%d, UserZen:%d)",
			iChaosCastleIndex+1, gObj[iZettoWinnerIndex].AccountID,
			gObj[iZettoWinnerIndex].Name, iZettoMoney, gObj[iZettoWinnerIndex].Money);
	}

	return iZettoWinnerIndex;
}





void CChaosCastle::CheckRegisterOfflineGift(int iUserIndex)
{
	if ( gObjIsConnected(iUserIndex) == FALSE )
	{
		return;
	}

	int iGiftRate = rand() % 10000;

	if ( iGiftRate < ::g_iChaosCastle_OffLineGiftRate )
	{
		EGReqRegCCOfflineGift(iUserIndex);
	}
}





