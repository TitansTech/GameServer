// TMonsterAIGroupMember.h: interface for the TMonsterAIGroupMember class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TMONSTERAIGROUPMEMBER_H__968E48F0_A3DB_4E2E_9339_8FF4C1C704EE__INCLUDED_)
#define AFX_TMONSTERAIGROUPMEMBER_H__968E48F0_A3DB_4E2E_9339_8FF4C1C704EE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class TMonsterAIGroupMember  
{

public:

	TMonsterAIGroupMember();
	virtual ~TMonsterAIGroupMember();

	void Reset();

public:

	short m_iGroupNumber;	// 4
	short m_iGuid;	// 8
	short m_iClass;	// C
	short m_iRank;	// 10
	short m_iStartAI;	// 14
	short m_iAI01;	// 18
	short m_iAI02;	// 1C
	short m_iAI03;	// 20
	short m_iCreateType;	// 24
	short m_iMapNumber;	// 28
	short m_iStartX;	// 2C
	short m_iStartY;	// 30
	short m_iStartDir;	// 34
	short m_iRegenType;	// 38
	short m_iObjIndex;	// 3C

};

#endif // !defined(AFX_TMONSTERAIGROUPMEMBER_H__968E48F0_A3DB_4E2E_9339_8FF4C1C704EE__INCLUDED_)
