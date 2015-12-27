// CrywolfSync.cpp: implementation of the CCrywolfSync class.
//	GS-N	1.00.18	JPN	0x00	0x0056F470	-	Completed
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CrywolfSync.h"
#include "Crywolf.h"
#include "user.h"
#include "DSprotocol.h"
#include "..\common\winutil.h"

CCrywolfSync g_CrywolfSync;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCrywolfSync::CCrywolfSync()
{
	this->m_bCrywolfEnable = TRUE;
	this->m_bApplyBenefit = FALSE;
	this->m_bApplyPenalty = FALSE;
	this->m_iOccupationState = 1;
	this->m_iCrywolfState = 0;
	this->m_iPlusChaosRate = 0;
	this->m_iGemDropPenaltyRate = 100;
	this->m_iGettingExpPenaltyRate = 100;
	this->m_iMinusMonHPBenefitRate = 100;
	this->m_iKundunHPRefillState = 1;
}

CCrywolfSync::~CCrywolfSync()
{
	return;
}



BOOL CCrywolfSync::CheckEnableCrywolf()
{
	return this->m_bCrywolfEnable;
}


void CCrywolfSync::SetEnableCrywolf(BOOL bEnable)
{
	this->m_bCrywolfEnable = bEnable;
}

int CCrywolfSync::GetOccupationState()
{
	return this->m_iOccupationState;
}

int CCrywolfSync::GetCrywolfState()
{
	return this->m_iCrywolfState;
}

void CCrywolfSync::SetOccupationState(int iOccupationState)
{
	this->m_iOccupationState = iOccupationState;
}

void CCrywolfSync::SetCrywolfState(int iCrywolfState)
{
	this->m_iCrywolfState = iCrywolfState;
}

int CCrywolfSync::GetPlusChaosRate()
{
	return this->m_iPlusChaosRate;
}

void CCrywolfSync::SetPlusChaosRate(int iPlusChaosRate)
{
	this->m_iPlusChaosRate = iPlusChaosRate;
}


int CCrywolfSync::GetGemDropPenaltiyRate()
{
	return this->m_iGemDropPenaltyRate;
}


void CCrywolfSync::SetGemDropPenaltiyRate(int iGemDropPenaltyRate)
{
	this->m_iGemDropPenaltyRate = iGemDropPenaltyRate;
}


int CCrywolfSync::GetGettingExpPenaltyRate()
{
	return this->m_iGettingExpPenaltyRate;
}


void CCrywolfSync::SetGettingExpPenaltyRate(int iGettingExpPenaltyRate)
{
	this->m_iGettingExpPenaltyRate = iGettingExpPenaltyRate;
}


int CCrywolfSync::GetMonHPBenefitRate()
{
	return this->m_iMinusMonHPBenefitRate;
}


void CCrywolfSync::SetMonHPBenefitRate(int iMinusMonHPBenefitRate)
{
	this->m_iMinusMonHPBenefitRate = iMinusMonHPBenefitRate;
}


int CCrywolfSync::GetKundunHPRefillState()
{
	return this->m_iKundunHPRefillState;
}


void CCrywolfSync::SetKundunHPRefillState(int iKundunHPRefillState)
{
	this->m_iKundunHPRefillState = iKundunHPRefillState;
}

void CCrywolfSync::NotifyCrywolfCurrentStateByUserId(int aIndex)
{
	PMSG_ANS_CRYWOLF_INFO pMsg = {0};

	PHeadSubSetB((LPBYTE)&pMsg, 0xBD, 0x00, sizeof(pMsg));
	pMsg.btOccupationState = this->GetOccupationState();
	pMsg.btCrywolfState = this->GetCrywolfState();

	//DaRKav CryWolf Interface Bug Fix Attempt
	//DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg));
}