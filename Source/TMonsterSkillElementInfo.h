// TMonsterSkillElementInfo.h: interface for the TMonsterSkillElementInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TMONSTERSKILLELEMENTINFO_H__8EC5FD6E_2B9F_409A_B2AA_70024491BEE5__INCLUDED_)
#define AFX_TMONSTERSKILLELEMENTINFO_H__8EC5FD6E_2B9F_409A_B2AA_70024491BEE5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class TMonsterSkillElementInfo
{

public:

	TMonsterSkillElementInfo()
	{
		// Reset Data
		this->Reset();
	}

	void  Reset();
	void  ResetDefense();
	void  ResetAttack();
	void  ResetAutoHP();
	void  ResetAutoMP();
	void  ResetAutoAG();
	void  ResetImmune();
	void  ResetResist();
	void  ResetModifyStat();
	int  CheckDefenseTime();
	int  CheckAttackTime();
	int  CheckAutoHPTime();
	int  CheckAutoMPTime();
	int  CheckAutoAGTime();
	int  CheckImmuneTime();
	int  CheckResistTime();
	int  CheckModifyStatTime();
	int  DecDefenseTime();
	int  DecAttackTime();
	int  DecAutoHPTime();
	int  DecAutoMPTime();
	int  DecAutoAGTime();
	int  DecImmuneTime();
	int  DecResistTime();
	int  DecModifyStatTime();

	static void CheckSkillElementInfoProc(struct OBJECTSTRUCT * lpObj);

public:

	short m_iSkillElementDefense;	// 0
	int m_iSkillElementDefenseTime;	// 4
	short m_iSkillElementAttack;	// 8
	int m_iSkillElementAttackTime;	// C
	short m_iSkillElementAutoHP;	// 10
	short m_iSkillElementAutoHPCycle;	// 14
	int m_iSkillElementAutoHPTime;	// 18
	short m_iSkillElementAutoMP;	// 1C
	short m_iSkillElementAutoMPCycle;	// 20
	int m_iSkillElementAutoMPTime;	// 24
	short m_iSkillElementAutoAG;	// 28
	short m_iSkillElementAutoAGCycle;	// 2C
	int m_iSkillElementAutoAGTime;	// 30
	short m_iSkillElementImmuneNumber;	// 34
	int m_iSkillElementImmuneTime;	// 38
	short m_iSkillElementResistNumber;	// 3C
	int m_iSkillElementResistTime;	// 40
	short m_iSkillElementModifyStat;	// 44
	short m_iSkillElementModifyStatType;	// 48
	int m_iSkillElementModifyStatTime;	// 4C



};



#endif // !defined(AFX_TMONSTERSKILLELEMENTINFO_H__8EC5FD6E_2B9F_409A_B2AA_70024491BEE5__INCLUDED_)
