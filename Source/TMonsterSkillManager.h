// TMonsterSkillManager.h: interface for the TMonsterSkillManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TMONSTERSKILLMANAGER_H__3097B8C8_180D_44EB_AFA7_CF7156DEDC27__INCLUDED_)
#define AFX_TMONSTERSKILLMANAGER_H__3097B8C8_180D_44EB_AFA7_CF7156DEDC27__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\common\TSync.h"
#include "TMonsterSkillInfo.h"
#include "TMonsterSkillUnit.h"
#include "user.h"

#define MAX_MONSTER_SKILL_INFO_ARRAY	(600)
#define MAX_MONSTER_SKILL_DELAY_INFO_ARRAY	(3000)

struct _ST_MONSTER_SKILL_DELAYTIME_INFO
{
	void RESET()
	{
		this->iIndex = -1;
		this->iTargetIndex = -1;
		this->dwDelayTime = 0;
		this->bIsUsed = FALSE;
		this->lpMonsterSkillUnit = NULL;
	}


	BOOL IsValid()
	{
		if ( !OBJMAX_RANGE(this->iIndex) || !OBJMAX_RANGE(this->iTargetIndex) )
			return FALSE;

		if ( this->lpMonsterSkillUnit == NULL )
			return FALSE;

		return TRUE;
	}

	_ST_MONSTER_SKILL_DELAYTIME_INFO()
	{
		this->RESET();
	}

	BOOL bIsUsed;	// 0
	short iIndex;	// 4
	short iTargetIndex;	// 8
	DWORD dwDelayTime;	// C
	TMonsterSkillUnit* lpMonsterSkillUnit;	// 10
};


class TMonsterSkillManager  
{

public:

	TMonsterSkillManager();
	~TMonsterSkillManager();

	static int LoadData(char* lpszFileName);
	static void DelAllSkillManagerInfo();
	static int CheckMonsterSkill(int iMonsterClass);
	static void UseMonsterSkill(int iIndex, int iTargetIndex, int iMonsterSkillUnitType);
	static class TMonsterSkillUnit* FindMonsterSkillUnit(int iIndex, int iMonsterSkillUnitType);
	static void MonsterSkillProc();
	static int AddMonsterSkillDelayInfo(int iIndex, int iTargetIndex, int iDelayTime, TMonsterSkillUnit* lpMonsterSkillUnit);

public:

	static TSync s_Sync;
	static BOOL s_bDataLoad;
	static TMonsterSkillInfo s_MonsterSkillInfoArray[MAX_MONSTER_SKILL_INFO_ARRAY];
	static _ST_MONSTER_SKILL_DELAYTIME_INFO s_MonsterSkillDelayInfoArray[MAX_MONSTER_SKILL_DELAY_INFO_ARRAY];

};

#endif // !defined(AFX_TMONSTERSKILLMANAGER_H__3097B8C8_180D_44EB_AFA7_CF7156DEDC27__INCLUDED_)
