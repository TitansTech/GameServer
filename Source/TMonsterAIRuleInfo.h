// TMonsterAIRuleInfo.h: interface for the TMonsterAIRuleInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TMONSTERAIRULEINFO_H__0A96A7D0_3DC5_439F_BEB1_940C22757704__INCLUDED_)
#define AFX_TMONSTERAIRULEINFO_H__0A96A7D0_3DC5_439F_BEB1_940C22757704__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class TMonsterAIRuleInfo  
{

public:

	TMonsterAIRuleInfo();
	void Reset();
	int IsValid();
	int CheckConditionSpecificDate();
	int CheckConditionCrywolfStart();
	int CheckConditionCrywolfEnd();

public:

	short m_iRuleNumber;	// 0
	short m_iMonsterClass;	// 4
	short m_iMonsterAIUnit;	// 8
	short m_iRuleCondition;	// C
	short m_iWaitTime;	// 10
	int m_iContinuanceTime;	// 14
	char m_iMonth;	// 18
	char m_iDay;	// 1C
	char m_iWeekDay;	// 20
	char m_iHour;	// 24
	char m_iMinute;	// 28
	int m_iAppliedTime;	// 2C
	short m_iApplyingTime;	// 30
	char m_szRuleDesc[50];	// 34


};

#endif // !defined(AFX_TMONSTERAIRULEINFO_H__0A96A7D0_3DC5_439F_BEB1_940C22757704__INCLUDED_)
