// CrywolfStatue.cpp: implementation of the CCrywolfStatue class.
//	GS-N	1.00.18	JPN	0x00567640	-	Completed
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CrywolfStatue.h"
#include "CrywolfAltar.h"
#include "CrywolfUtil.h"
#include "Gamemain.h"
#include "user.h"

#if (GS_CASTLE==1)

static CCrywolfUtil UTIL;
CCrywolfStatue g_CrywolfNPC_Statue;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCrywolfStatue::CCrywolfStatue()
{

}

CCrywolfStatue::~CCrywolfStatue()
{

}



int CCrywolfStatue::GetStatueViewState(int iPriestNumber)
{
	int iViewState = 0;

	switch ( iPriestNumber )
	{
		case 0:
			iViewState = 0;
			break;
		case 1:
			iViewState = 1;
			break;
		case 2:
			iViewState = 2;
			break;
		case 3:
			iViewState = 4;
			break;
		case 4:
			iViewState = 8;
			break;
		case 5:
			iViewState = 256;
			break;
	}

	return iViewState;
}



void CCrywolfStatue::CrywolfStatueAct(int iIndex)
{
	if ( !gObjIsConnected(iIndex))
		return;

	if ( gObj[iIndex].Type != OBJ_NPC || CHECK_CLASS(204, gObj[iIndex].Class) == FALSE )
		return;

	LPOBJ lpObj = &gObj[iIndex];
	int iContractedAlterCount = g_CrywolfNPC_Altar.GetContractedAltarCount();
	int iPriestHPSum = g_CrywolfNPC_Altar.GetPriestHPSum();
	int iPriestMaxHPSum = g_CrywolfNPC_Altar.GetPriestMaxHPSum();

	if ( iContractedAlterCount == 0 || this->m_Shield.m_iShieldHP == 0 )
	{
		//=========================================================================================
		//CRYWOLF STATUE EFFECTS
		//=========================================================================================
		//[SKILL STATE] 			= Wolf personal status shields		0x42
		//[SKILL STATE] 			= Wolf personal status shields		0x43
		//[SKILL STATE] 			= Wolf personal status shields		0x44
		//[SKILL STATE] 			= Wolf personal status shields		0x45
		//[SKILL STATE] 			= Wolf personal status shields		0x46
		//=========================================================================================

		if ( this->m_Shield.m_iShieldState == 1 )
		{
			UTIL.SendCrywolfUserAnyMsg(2, lMsg.Get(MSGGET(13, 5)));
			this->m_Shield.m_iShieldState = 0;
			this->m_Shield.m_iShieldHP = 0;
			this->m_Shield.m_iShieldMaxHP = 0;
			this->m_Shield.m_iPriestNumber = iContractedAlterCount;
			lpObj->m_ViewSkillState |= this->GetStatueViewState(iContractedAlterCount);
			GCSkillInfoSend(lpObj, 1, 0x44);
		}
		else if ( this->m_Shield.m_iPriestNumber != iContractedAlterCount )
		{
			this->m_Shield.m_iShieldState = 1;
			this->m_Shield.m_iShieldHP = iPriestHPSum;
			this->m_Shield.m_iShieldMaxHP = iPriestMaxHPSum;
			this->m_Shield.m_iPriestNumber = iContractedAlterCount;
			UTIL.SendCrywolfUserAnyMsg(2, lMsg.Get(MSGGET(13, 6)), iContractedAlterCount, iPriestHPSum);
			lpObj->m_ViewSkillState |= this->GetStatueViewState(iContractedAlterCount);
			GCSkillInfoSend(lpObj, 1, 0x45);
		}

		return;
	}
	else if ( this->m_Shield.m_iShieldState == 0 )
	{
		UTIL.SendCrywolfUserAnyMsg(2, lMsg.Get(MSGGET(13, 7)));
		lpObj->m_ViewSkillState |= this->GetStatueViewState(iContractedAlterCount);
		GCSkillInfoSend(lpObj, 1, 0x42);
	}
	else if ( this->m_Shield.m_iPriestNumber != iContractedAlterCount )
	{
		UTIL.SendCrywolfUserAnyMsg(2, lMsg.Get(MSGGET(13, 6)), iContractedAlterCount, iPriestHPSum);
		lpObj->m_ViewSkillState |= this->GetStatueViewState(iContractedAlterCount);
		GCSkillInfoSend(lpObj, 1, 0x43);
	}

	this->m_Shield.m_iShieldState = 1;
	this->m_Shield.m_iShieldHP = iPriestHPSum;
	this->m_Shield.m_iShieldMaxHP = iPriestMaxHPSum;
	this->m_Shield.m_iPriestNumber = iContractedAlterCount;
}

#endif