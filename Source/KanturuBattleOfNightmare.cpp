// KanturuBattleOfNightmare.cpp: implementation of the CKanturuBattleOfNightmare class.
//	GS-N	1.00.18	JPN	0x0057D3F0	-	Completed
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "KanturuBattleOfNightmare.h"
#include "Kanturu.h"
#include "KanturuMonsterMng.h"
#include "KanturuBattleUserMng.h"
#include "Gamemain.h"
#include "TMonsterAIGroup.h"
#include "LogProc.h"
#include "..\include\ReadScript.h"
#include "KanturuUtil.h"

#if (GS_CASTLE==0)

static CKanturuUtil KANTURU_UTIL;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CKanturuBattleOfNightmare::CKanturuBattleOfNightmare()
{
	this->SetBattleOfNightmareState(KANTURU_NIGHTMARE_NONE);
	this->ResetAllData();
	this->m_iNightmareAIGroupNumber = 0;
	this->m_iNightmareObjIndex = -1;
	this->SetSuccessValue(FALSE);
}

CKanturuBattleOfNightmare::~CKanturuBattleOfNightmare()
{
	return;
}



void CKanturuBattleOfNightmare::ResetAllData()
{
	this->m_StateInfoCount = 0;

	for ( int iCount=0;iCount<KANTURU_NIGHTMARE_STATE_INFO;iCount++)
	{
		this->m_StateInfo[iCount].ResetTimeInfo();
	}
}



BOOL CKanturuBattleOfNightmare::LoadData(LPSTR lpszFileName)
{
	this->m_bFileDataLoad = FALSE;

	if ( !lpszFileName || !strcmp(lpszFileName, ""))
	{
		MsgBox("[Kanturu][BattleOfNightmare] - File load error : File Name Error");
		return FALSE;
	}

	try
	{
		
		SMDFile = fopen(lpszFileName, "r");

		if ( SMDFile == NULL )
		{
			MsgBox("[Kanturu][BattleOfNightmare] - Can't Open %s ",
				lpszFileName);

			return FALSE;
		}

		SMDToken Token;
		this->ResetAllData();

		int iType = -1;
		int iState = 0;
		int iCondition = 0;
		int iValue = 0;

		while ( true )
		{
			Token = GetToken();

			if ( Token == END )
			{
				break;
			}

			iType = TokenNumber;

			while ( true )
			{
				if ( iType == 7 )
				{
					iState = 0;
					iCondition = 0;
					iValue = 0;

					Token = GetToken();

					if ( !strcmp("end", TokenString))
					{
						break;
					}

					iState = TokenNumber;

					Token = GetToken();
					iCondition = TokenNumber;

					Token = GetToken();
					iValue = TokenNumber;

					if ( this->m_StateInfoCount < 0 || this->m_StateInfoCount >= KANTURU_NIGHTMARE_STATE_INFO )
					{
						MsgBox("[Kanturu][BattleOfNightmare] - Exceed Max State Time (%d)",
							this->m_StateInfoCount);

						break;
					}

					this->m_StateInfo[this->m_StateInfoCount].SetStateInfo(iState);
					this->m_StateInfo[this->m_StateInfoCount].SetCondition(iCondition);

					if ( iCondition == 1 )
					{
						iValue *= 1000;
					}

					this->m_StateInfo[this->m_StateInfoCount].SetValue(iValue);
					this->m_StateInfoCount++;
				}
				else if ( iType == 8 )
				{
					int iValue = 0;

					Token = GetToken();

					if ( !strcmp("end", TokenString))
					{
						break;
					}

					iValue = TokenNumber;

					if ( iValue < 0 )
					{
						MsgBox("[Kanturu][BattleOfNightmare] - Nightmare Hand AI Group Read Error(%d)",
							iValue);

						iValue=0;
					}

					this->m_iNightmareAIGroupNumber = iValue;
				}
				else
				{
					Token = GetToken();

					if ( !strcmp("end", TokenString))
					{
						break;
					}
				}

			}	// while
		}	// while

		fclose(SMDFile);

		LogAddTD("[Kanturu][BattleOfNightmare]  %s file is Loaded",
			lpszFileName);

		this->m_bFileDataLoad = TRUE;
	}	// __try
	catch ( DWORD )
	{
		MsgBox("[Kanturu][BattleOfNightmare] - Loading Exception Error (%s) File. ", lpszFileName);
	}

	return this->m_bFileDataLoad;
}



void CKanturuBattleOfNightmare::Run()
{
	this->CheckStateTime();
	this->CheckNightmareBattleUser();
	g_KanturuBattleUserMng.CheckUserState();

	switch ( this->m_iBattleOfNightmareState )
	{
		case KANTURU_NIGHTMARE_NONE:
			this->ProcState_NONE();
			break;
		case KANTURU_NIGHTMARE_IDLE:
			this->ProcState_IDLE();
			break;
		case KANTURU_NIGHTMARE_NOTIFY:
			this->ProcState_NOTIFY();
			break;
		case KANTURU_NIGHTMARE_START:
			this->ProcState_START();
			break;
		case KANTURU_NIGHTMARE_END:
			this->ProcState_END();
			break;
		case KANTURU_NIGHTMARE_ENDCYCLE:
			this->ProcState_ENDCYCLE();
			break;
	}
}



void CKanturuBattleOfNightmare::SetState(int iBattleOfNightmareState)
{
	if ( this->m_StateInfo[iBattleOfNightmareState].GetCondition() > 0 )
	{
		this->m_StateInfo[iBattleOfNightmareState].SetAppliedTime();
	}

	g_KanturuMonsterMng.ResetRegenMonsterObjData();
	this->m_iNightmareObjIndex = -1;

	LogAddTD("[Kanturu][BattleOfNightmare] CKanturuBattleOfNightmare::SetState(...)  m_iNightmareObjIndex %d",
		this->m_iNightmareObjIndex);

	switch ( iBattleOfNightmareState )
	{
		case KANTURU_NIGHTMARE_NONE:
			this->SetState_NONE();
			break;
		case KANTURU_NIGHTMARE_IDLE:
			this->SetState_IDLE();
			break;
		case KANTURU_NIGHTMARE_NOTIFY:
			this->SetState_NOTIFY();
			break;
		case KANTURU_NIGHTMARE_START:
			this->SetState_START();
			break;
		case KANTURU_NIGHTMARE_END:
			this->SetState_END();
			break;
		case KANTURU_NIGHTMARE_ENDCYCLE:
			this->SetState_ENDCYCLE();
			break;
	}

	KANTURU_UTIL.NotifyKanturuChangeState(KANTURU_STATE_BATTLE_OF_NIGHTMARE, this->m_iBattleOfNightmareState);
}



void CKanturuBattleOfNightmare::SetNextState(int iCurrentState)
{
	int iNextState = iCurrentState + 1;

	if ( iNextState >= KANTURU_NIGHTMARE_STATE_INFO )
	{
		iNextState = KANTURU_NIGHTMARE_NONE;
	}

	this->SetState(iNextState);
}



void CKanturuBattleOfNightmare::SetState_NONE()
{
	LogAddC(5, "[Kanturu][BattleOfNightmare] State(%d) -> NONE",
		this->m_iBattleOfNightmareState);

	this->SetBattleOfNightmareState(KANTURU_NIGHTMARE_NONE);
	TMonsterAIGroup::DelGroupInstance(this->m_iNightmareAIGroupNumber);

	LogAddTD("[Kanturu][BattleOfNightmare] DelGroupInstance SetState_NONE");
}


void CKanturuBattleOfNightmare::SetState_IDLE()
{
	LogAddC(5, "[Kanturu][BattleOfNightmare] State(%d) -> IDLE",
		this->m_iBattleOfNightmareState);

	LogAddTD("[Kanturu][BattleOfNightmare] State(%d) -> IDLE",
		this->m_iBattleOfNightmareState);

	this->SetBattleOfNightmareState(KANTURU_NIGHTMARE_IDLE);
	this->SetSuccessValue(FALSE);
	TMonsterAIGroup::DelGroupInstance(this->m_iNightmareAIGroupNumber);

	LogAddTD("[Kanturu][BattleOfNightmare] DelGroupInstance SetState_IDLE");
	g_KanturuBattleUserMng.MoveAllUser(134);
}


void CKanturuBattleOfNightmare::SetState_NOTIFY()
{
	LogAddC(5, "[Kanturu][BattleOfNightmare] State(%d) -> NOTIFY",
		this->m_iBattleOfNightmareState);

	this->SetBattleOfNightmareState(KANTURU_NIGHTMARE_NOTIFY);
}


void CKanturuBattleOfNightmare::SetState_START()
{
	LogAddC(5, "[Kanturu][BattleOfNightmare] State(%d) -> START",
		this->m_iBattleOfNightmareState);

	this->SetBattleOfNightmareState(KANTURU_NIGHTMARE_START);

	if ( this->m_StateInfo[this->m_iBattleOfNightmareState].GetCondition() == 1 )
	{
		KANTURU_UTIL.NotifyKanturuBattleTime(this->m_StateInfo[this->m_iBattleOfNightmareState].GetValue());
	}

	g_KanturuMonsterMng.SetKanturuMonster(2);

	TMonsterAIGroup::Init(this->m_iNightmareAIGroupNumber);
	TMonsterAIGroup::ChangeAIOrder(this->m_iNightmareAIGroupNumber, 0);

	int iIndex = TMonsterAIGroup::FindGroupLeader(this->m_iNightmareAIGroupNumber);
	this->m_iNightmareObjIndex = iIndex;

	LogAddTD("[Kanturu][BattleOfNightmare] CKanturuNightmare Set m_iNightmareObjIndex %d",
		this->m_iNightmareObjIndex);
}



void CKanturuBattleOfNightmare::SetState_END()
{
	LogAddC(5, "[Kanturu][BattleOfNightmare] State(%d) -> END",
		this->m_iBattleOfNightmareState);

	this->SetBattleOfNightmareState(KANTURU_NIGHTMARE_END);
	KANTURU_UTIL.NotifyKanturuSuccessValue(this->GetSuccessValue());

	if ( this->GetSuccessValue() == FALSE )
	{
		KANTURU_UTIL.SendMsgKanturuBattleUser(lMsg.Get(MSGGET(13, 36)));
		LogAddTD("[Kanturu][BattleOfNightmare] Fail!! TimeOut");
	}
}


void CKanturuBattleOfNightmare::SetState_ENDCYCLE()
{
	LogAddC(5, "[Kanturu][BattleOfNightmare] State(%d) -> ENDCYCLE",
		this->m_iBattleOfNightmareState);

	LogAddTD("[Kanturu][BattleOfNightmare] State(%d) -> ENDCYCLE",
		this->m_iBattleOfNightmareState);

	this->SetBattleOfNightmareState(KANTURU_NIGHTMARE_ENDCYCLE);

	if ( this->GetSuccessValue() == TRUE )
	{
		KANTURU_UTIL.SendMsgKanturuBattleUser(lMsg.Get(MSGGET(13, 35)));
	}
	else
	{
		g_KanturuBattleUserMng.MoveAllUser(137);
		g_KanturuBattleUserMng.ResetAllData();
	}
}



void CKanturuBattleOfNightmare::ProcState_NONE()
{
	return;
}


void CKanturuBattleOfNightmare::ProcState_IDLE()
{
	return;
}


void CKanturuBattleOfNightmare::ProcState_NOTIFY()
{
	return;
}


void CKanturuBattleOfNightmare::ProcState_START()
{
	KANTURU_UTIL.NotifyKanturuUserMonsterCount(g_KanturuMonsterMng.GetAliveMonsterCount(),
		g_KanturuBattleUserMng.GetUserCount());

	int iIndex = this->m_iNightmareObjIndex;

	if ( gObj[iIndex].Live == FALSE )
	{
		this->SetSuccessValue(TRUE);

		LogAddTD("[Kanturu][BattleOfNightmare] Success!! Nightmare Die");
		g_KanturuBattleUserMng.LogBattleWinnerUserInfo(3, this->m_StateInfo[this->m_iBattleOfNightmareState].GetElapsedTime());

		this->SetNextState(this->m_iBattleOfNightmareState);
	}
}



void CKanturuBattleOfNightmare::ProcState_END()
{
	return;
}



void CKanturuBattleOfNightmare::ProcState_ENDCYCLE()
{
	TMonsterAIGroup::DelGroupInstance(this->m_iNightmareAIGroupNumber);

	LogAddTD("[Kanturu][BattleOfNightmare] DelGroupInstance ProcState_ENDCYCLE");
}


void CKanturuBattleOfNightmare::CheckStateTime()
{
	int iCurrentState = this->GetBattleOfNightmareState();

	if ( iCurrentState )
	{
		if ( this->m_StateInfo[iCurrentState].GetCondition() == 1 )
		{
			if ( this->m_StateInfo[iCurrentState].IsTimeOut() == TRUE )
			{
				this->SetNextState(iCurrentState);
			}
		}
	}
}



int CKanturuBattleOfNightmare::GetRemainTime()
{
	int iCurrentState = this->GetBattleOfNightmareState();
	return this->m_StateInfo[iCurrentState].GetRemainTime();
}



void CKanturuBattleOfNightmare::CheckNightmareBattleUser()
{
	if ( this->GetBattleOfNightmareState() == KANTURU_NIGHTMARE_NOTIFY ||
		 this->GetBattleOfNightmareState() == KANTURU_NIGHTMARE_START )
	{
		if ( g_KanturuBattleUserMng.IsEmpty() == TRUE )
		{
			g_KanturuBattleUserMng.ResetAllData();
			this->SetSuccessValue(FALSE);

			LogAddTD("[Kanturu][BattleOfNightmare] Fail!! All Battle User Die");

			this->SetState(KANTURU_NIGHTMARE_END);
		}
	}
}

#endif