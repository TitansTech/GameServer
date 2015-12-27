// CrywolfAltar.cpp: implementation of the CCrywolfAltar class.
//	GS-N	1.00.18	JPN	0x00565f30	-	Completed
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CrywolfAltar.h"
#include "CrywolfUtil.h"
#include "Gamemain.h"
#include "logproc.h"

#if (GS_CASTLE==1)

static CCrywolfUtil UTIL;
CCrywolfAltar g_CrywolfNPC_Altar;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCrywolfAltar::CCrywolfAltar()
{
	return;
}

CCrywolfAltar::~CCrywolfAltar()
{
	return;
}

//=========================================================================================
//CRYWOLF ALTAR EFFECTS
//=========================================================================================
//[SKILL STATE] 0x10000		= The altar contract with the wolf		0x15 <<Blue, availiable
//[SKILL STATE] 0x00000		= Wolf altar contract inability			0x16 <<Red, unavailiable
//[SKILL STATE] 0x40000		= Wolf altar contract condition			0x17 <<Yellow, Active Contract
//[SKILL STATE] 0x80000		= Wolf altar contract reading			0x18 <<Blue, Reading
//[SKILL STATE] 0x20000		= Wolf altar occupied condition			0x19 <<Red, Cancelled
//[SKILL STATE] 0x10000000	= Wolf NPC transparent					0x1B <<Invisible NPC
//=========================================================================================
int CCrywolfAltar::GetAltarState(int iClass)
{
	int iAltarState = 0;

	switch ( iClass )
	{
		case 205:
			iAltarState = this->m_AltarInfo[0].GetAltarState();
			break;
		case 206:
			iAltarState = this->m_AltarInfo[1].GetAltarState();
			break;
		case 207:
			iAltarState = this->m_AltarInfo[2].GetAltarState();
			break;
		case 208:
			iAltarState = this->m_AltarInfo[3].GetAltarState();
			break;
		case 209:
			iAltarState = this->m_AltarInfo[4].GetAltarState();
			break;
	}

	return iAltarState;
}




int CCrywolfAltar::GetAltarRemainContractCount(int iClass)
{
	int iAltarRemainContractCount = 0;

	switch ( iClass )
	{
		case 205:
			iAltarRemainContractCount = this->m_AltarInfo[0].GetRemainContractCount();
			break;
		case 206:
			iAltarRemainContractCount = this->m_AltarInfo[1].GetRemainContractCount();
			break;
		case 207:
			iAltarRemainContractCount = this->m_AltarInfo[2].GetRemainContractCount();
			break;
		case 208:
			iAltarRemainContractCount = this->m_AltarInfo[3].GetRemainContractCount();
			break;
		case 209:
			iAltarRemainContractCount = this->m_AltarInfo[4].GetRemainContractCount();
			break;
	}

	return iAltarRemainContractCount;
}




int CCrywolfAltar::GetAlatarNumber(int iClass)
{
	int iAltarNumber = 0;

	switch ( iClass )
	{
		case 205:
			iAltarNumber = 0;
			break;
		case 206:
			iAltarNumber = 1;
			break;
		case 207:
			iAltarNumber = 2;
			break;
		case 208:
			iAltarNumber = 3;
			break;
		case 209:
			iAltarNumber = 4;
			break;
	}

	return iAltarNumber;
}




int CCrywolfAltar::GetAltarUserIndex(int iClass)
{
	int iUserIndex = -1;

	switch ( iClass )
	{
		case 205:
			iUserIndex = this->m_AltarInfo[0].m_iUserIndex;
			break;
		case 206:
			iUserIndex = this->m_AltarInfo[1].m_iUserIndex;
			break;
		case 207:
			iUserIndex = this->m_AltarInfo[2].m_iUserIndex;
			break;
		case 208:
			iUserIndex = this->m_AltarInfo[3].m_iUserIndex;
			break;
		case 209:
			iUserIndex = this->m_AltarInfo[4].m_iUserIndex;
			break;
	}

	return iUserIndex;
}



void CCrywolfAltar::SetAltarValidContract(int iAltarObjIndex, int iClass)
{
	int iAltarNumber = this->GetAlatarNumber(iClass);
	CCrywolfAltarInfo * AltarInfo = &this->m_AltarInfo[iAltarNumber];

	if ( AltarInfo->GetAltarState() != 2 )
		return;

	if ( AltarInfo->m_dwValidContractTime < GetTickCount() )
	{
		GCSkillInfoSend(&gObj[iAltarObjIndex], 1, 0x17);
		
		MsgOutput(AltarInfo->m_iUserIndex, lMsg.Get(MSGGET(12, 251)), iAltarNumber);
		UTIL.SendCrywolfUserAnyMsg(2, lMsg.Get(MSGGET(12, 252)), gObj[AltarInfo->m_iUserIndex].Name, iAltarNumber);

		LogAddTD("[Crywolf][Altar Op.]  [%s][%s] Set Valid Contract Altar[%d]",
			gObj[AltarInfo->m_iUserIndex].AccountID, gObj[AltarInfo->m_iUserIndex].Name, iAltarNumber);

		AltarInfo->SetAltarState(1);

		gObj[iAltarObjIndex].m_ViewSkillState = 0x40000;
	}
}



BOOL CCrywolfAltar::CheckAltarValidContract(int iClass)
{
	int iAltarNumber = this->GetAlatarNumber(iClass);
	CCrywolfAltarInfo * AltarInfo = &this->m_AltarInfo[iAltarNumber];

	if ( AltarInfo->GetAltarState() == 1 )
		return TRUE;

	return FALSE;
}



BOOL CCrywolfAltar::SetAltarUserIndex(int iAltarObjIndex, int iClass, int iUserIndex)
{
	int iAltarNumber = this->GetAlatarNumber(iClass);
	CCrywolfAltarInfo * AltarInfo = &this->m_AltarInfo[iAltarNumber];
	DWORD dwCurrentTime = GetTickCount();

	if ( gObjIsConnected(AltarInfo->m_iUserIndex) != FALSE )
	{
		MsgOutput(iUserIndex, lMsg.Get(MSGGET(12, 253)), iAltarNumber);
		return FALSE;
	}

	if ( (dwCurrentTime-AltarInfo->m_dwLastValidContractTime) < 10000 )
	{
		MsgOutput(iUserIndex, lMsg.Get(MSGGET(12, 254)), iAltarNumber);
		return FALSE;
	}

	if ( AltarInfo->m_iContractCount > MAX_ALTAR_CONTRACTS ) 
	{
		MsgOutput(iUserIndex, lMsg.Get(MSGGET(12, 255)), iAltarNumber);
		return FALSE;
	}

	if ( abs(gObj[iAltarObjIndex].Y - gObj[iUserIndex].Y) > 0 || abs(gObj[iAltarObjIndex].X - gObj[iUserIndex].X) > 0 )
	{
		MsgOutput(iUserIndex, lMsg.Get(MSGGET(13, 0)), iAltarNumber);
		return FALSE;
	}

	GCSkillInfoSend(&gObj[iAltarObjIndex], 1, 0x18);
	
	AltarInfo->SetAltarState(2);
	AltarInfo->m_iUserIndex = iUserIndex;
	AltarInfo->m_dwAppliedContractTime = dwCurrentTime;
	AltarInfo->m_dwValidContractTime = dwCurrentTime + 5000;	// It Get Active after 5 Seconds
	AltarInfo->m_dwLastValidContractTime = dwCurrentTime;
	AltarInfo->m_iContractCount++;
	gObj[iAltarObjIndex].m_ViewSkillState = 0x80000;

	UTIL.SendCrywolfUserAnyMsg(2, lMsg.Get(MSGGET(13, 1)), gObj[AltarInfo->m_iUserIndex].Name, iAltarNumber);

	LogAddTD("[Crywolf][Altar Op.] [%s][%s] Attempt to contract Altar[%d]",
		gObj[AltarInfo->m_iUserIndex].AccountID, gObj[AltarInfo->m_iUserIndex].Name,
		iAltarNumber);

	MsgOutput(iUserIndex, lMsg.Get(MSGGET(13, 2)));
	MsgOutput(iUserIndex, lMsg.Get(MSGGET(13, 3)));

	return TRUE;
}
	


void CCrywolfAltar::ResetAltarUserIndex(int iAltarObjIndex, int iClass)
{
	int iAltarNumber = this->GetAlatarNumber(iClass);
	CCrywolfAltarInfo * AltarInfo = &this->m_AltarInfo[iAltarNumber];

	if ( gObjIsConnected(AltarInfo->m_iUserIndex) != FALSE )
	{
		MsgOutput(AltarInfo->m_iUserIndex, lMsg.Get(MSGGET(13, 4)), iAltarNumber);
		LogAddTD("[Crywolf][Altar Op.] [%s][%s] Remove contract Altar[%d][%d,%d]",
		gObj[AltarInfo->m_iUserIndex].AccountID, gObj[AltarInfo->m_iUserIndex].Name,
		iAltarNumber,gObj[AltarInfo->m_iUserIndex].X,gObj[AltarInfo->m_iUserIndex].Y);
	}

	if ( AltarInfo->m_iContractCount >= MAX_ALTAR_CONTRACTS )
	{
		GCSkillInfoSend(&gObj[iAltarObjIndex], 1, 0x19);
		
		AltarInfo->SetAltarState(3);
		gObj[iAltarObjIndex].m_ViewSkillState = 0x20000;
	}
	else
	{
		GCSkillInfoSend(&gObj[iAltarObjIndex], 1, 0x15);
		
		AltarInfo->SetAltarState(0);
		gObj[iAltarObjIndex].m_ViewSkillState = 0x10000;
	}

	AltarInfo->m_iUserIndex = -1;
	AltarInfo->m_dwAppliedContractTime = 0;
	AltarInfo->m_dwValidContractTime = 0;
	AltarInfo->m_dwLastValidContractTime = GetTickCount();
}



void CCrywolfAltar::SetAltarObjIndex(int iClass, int iObjIndex)
{
	int iAltarNumber = this->GetAlatarNumber(iClass);
	this->ResetAltar(iClass);
	CCrywolfAltarInfo * AltarInfo = &this->m_AltarInfo[iAltarNumber];
	AltarInfo->SetAltarIndex(iObjIndex);
}



void CCrywolfAltar::SetAllAltarObjectIndex()
{
	for ( int i=0;i<OBJ_MAXMONSTER;i++)
	{
		if ( !gObjIsConnected(i) )
			continue;

		if ( gObj[i].MapNumber != MAP_INDEX_CRYWOLF_FIRSTZONE )
			continue;

		if ( CRYWOLF_ALTAR_CLASS_RANGE(gObj[i].Class ) != FALSE )
			g_CrywolfNPC_Altar.SetAltarObjIndex(gObj[i].Class, i);
	}
}


void CCrywolfAltar::ResetAllAltar()
{
	this->ResetAltar(205);
	this->ResetAltar(206);
	this->ResetAltar(207);
	this->ResetAltar(208);
	this->ResetAltar(209);
}



void CCrywolfAltar::ResetAltar(int iClass)
{
	int iAltarNumber = this->GetAlatarNumber(iClass);
	CCrywolfAltarInfo * AltarInfo = &this->m_AltarInfo[iAltarNumber];

	if ( AltarInfo->m_iAltarIndex == -1 )
		return;

	GCSkillInfoSend(&gObj[AltarInfo->m_iAltarIndex], 1, 0x16);

	gObj[AltarInfo->m_iAltarIndex].m_ViewSkillState = 0;
	AltarInfo->Reset();
}



void CCrywolfAltar::SetAllAltarViewState(int iAltarState)
{
	this->SetAltarViewState(205, iAltarState);
	this->SetAltarViewState(206, iAltarState);
	this->SetAltarViewState(207, iAltarState);
	this->SetAltarViewState(208, iAltarState);
	this->SetAltarViewState(209, iAltarState);
}

void CCrywolfAltar::SetAltarViewState(int iClass, int iAltarState)
{
	int iAltarNumber = this->GetAlatarNumber(iClass);
	CCrywolfAltarInfo * AltarInfo = &this->m_AltarInfo[iAltarNumber];

	if ( AltarInfo->m_iAltarIndex == -1 )
		return;

	GCSkillInfoSend(&gObj[AltarInfo->m_iAltarIndex], 1, 0x15);
	gObj[AltarInfo->m_iAltarIndex].m_ViewSkillState = 0x10000;
	AltarInfo->SetAltarState(0);
}


int CCrywolfAltar::GetContractedAltarCount()
{
	int iContractedAltarCount = 0;

	for ( int i=0;i<MAX_CRYWOLF_ALTAR;i++)
	{
		CCrywolfAltarInfo * AltarInfo = &this->m_AltarInfo[i];

		if ( AltarInfo->GetAltarState() != 1 )
			continue;

		iContractedAltarCount++;
	}

	return iContractedAltarCount;
}



int CCrywolfAltar::GetPriestHPSum()
{
	int iPriestHPSum = 0;

	for ( int i=0;i<MAX_CRYWOLF_ALTAR;i++)
	{
		CCrywolfAltarInfo * AltarInfo = &this->m_AltarInfo[i];

		if ( AltarInfo->GetAltarState() != 1 )
			continue;

		if ( !gObjIsConnected(AltarInfo->m_iUserIndex))
			continue;

		LPOBJ lpUserObj = &gObj[AltarInfo->m_iUserIndex];
		iPriestHPSum += lpUserObj->Life;
	}

	return iPriestHPSum;
}


int CCrywolfAltar::GetPriestMaxHPSum()
{
	int iPriestHPSum = 0;

	for ( int i=0;i<MAX_CRYWOLF_ALTAR;i++)
	{
		CCrywolfAltarInfo * AltarInfo = &this->m_AltarInfo[i];

		if ( AltarInfo->GetAltarState() != 1 )
			continue;

		if ( !gObjIsConnected(AltarInfo->m_iUserIndex))
			continue;

		LPOBJ lpUserObj = &gObj[AltarInfo->m_iUserIndex];
		iPriestHPSum += lpUserObj->AddLife + lpUserObj->MaxLife;
	}

	return iPriestHPSum;
}



void CCrywolfAltar::CrywolfAltarAct(int iIndex)
{
	if ( !gObjIsConnected(iIndex))
		return;

	if ( (gObj[iIndex].Type != OBJ_NPC) || (CRYWOLF_ALTAR_CLASS_RANGE(gObj[iIndex].Class) == FALSE) )
		return;

	int iUserIndex = this->GetAltarUserIndex(gObj[iIndex].Class);

	if ( gObjIsConnected(iUserIndex) == FALSE )
	{
		if ( iUserIndex != -1 )
		{
			this->ResetAltarUserIndex(iIndex, gObj[iIndex].Class);
		}
		return;
	}

	LPOBJ lpObj = &gObj[iIndex];
	LPOBJ lpUserObj = &gObj[iUserIndex];

	if ( lpUserObj->Live != FALSE && lpUserObj->MapNumber == MAP_INDEX_CRYWOLF_FIRSTZONE )
	{
		if ( (lpObj->Y-lpUserObj->Y) <= 0 && (lpObj->X - lpUserObj->X) <= 0 )
		{
			if ( this->CheckAltarValidContract(gObj[iIndex].Class) != FALSE )
			{
			}
			else
			{
				this->SetAltarValidContract(iIndex, gObj[iIndex].Class);
			}
		}
		else
		{
			this->ResetAltarUserIndex(iIndex, gObj[iIndex].Class);
		}
	}
	else
	{
		this->ResetAltarUserIndex(iIndex, gObj[iIndex].Class);
	}
}

#endif