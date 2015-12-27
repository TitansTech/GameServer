#ifndef MONSTERATTR_H
#define MONSTERATTR_H

#include "user.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef struct MONSTER_ATTRIBUTE
{
  int m_Index;	// 0
  int m_Rate;	// 4
  char m_Name[20];	// 8
  BYTE m_Level;	// 1C
  int m_iAINumber;	// 20
  int m_iScriptHP;	// 24
  int m_Hp;	// 28
  int m_Mp;	// 2C
  int m_DamageMin;	// 30
  int m_DamageMax;	// 34
  int m_Defense;	// 38
  short m_MagicDefense;	// 3C
  short m_AttackRating;	// 40
  short m_Successfulblocking;	// 44
  BYTE m_MoveRange;	// 48
  BYTE m_AttackRange;	// 4C
  short m_AttackType;	// 50
  BYTE m_ViewRange;	// 54
  short m_MoveSpeed;	// 58
  short m_AttackSpeed;	// 5C
  int m_RegenTime;	// 60
  short m_Attribute;	// 64
  short m_ItemRate;	// 68
  short m_MoneyRate;	// 6C
  BYTE m_MaxItemLevel;	// 70
  BYTE m_Resistance[MAX_RESISTENCE_TYPE];	// 74
  DWORD m_MonsterSkill;	// 7C
} MONSTER_ATTRIBUTE, * LPMONSTER_ATTRIBUTE;


class CMonsterAttr
{

public:

	LPMONSTER_ATTRIBUTE GetAttr(int aClass);
	void LoadAttr(char* Buffer, int iSize);
	void LoadAttr(char* filename);

	CMonsterAttr();
	virtual ~CMonsterAttr();

private:

	MONSTER_ATTRIBUTE m_MonsterAttr[MAX_MONSTER_TYPE];	// 4
};

#endif