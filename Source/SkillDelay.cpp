// ------------------------------
// Decompiled by Deathway
// Date : 2007-03-09
// ------------------------------
#include "stdafx.h"
#include "SkillDelay.h"

// GS-N 0.99.60T Status Completed : in oder version the m_SkillDelay coudl change
//	GS-N	1.00.18	JPN	0x00485540	-	Completed

CSkillDelay::CSkillDelay()
{
	this->Init();
}


CSkillDelay::~CSkillDelay()
{
	return;
}


void CSkillDelay::Init()
{
	memset(this->LastSkillUseTime, 0, sizeof(this->LastSkillUseTime) );
}


#pragma warning ( disable : 4101 )
int CSkillDelay::Check(WORD skill)
{
	int skilldelaytime;
	DWORD dwtime;
	return 1;
}
#pragma warning ( default : 4101 )