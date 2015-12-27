#ifndef DARKSPIRIT_H
#define DARKSPIRIT_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "user.h"
#include "MagicInf.h"
#include "..\\common\\zzzitem.h"
#include "ObjBaseAttack.h"



//#define RAVEN_ATTACK_DISTANCE		7



struct PMSG_SEND_PET_ITEMINFO
{
	PBMSG_HEAD h;	// C1:[A9:AC]
	BYTE PetType;	// 3
	BYTE InvenType;	// 4
	BYTE nPos;	// 5
	BYTE Level;	// 6
	int Exp;	// 8
};


class CDarkSpirit : public CObjBaseAttack
{

public:

	int m_AttackDamageMin;	// 4
	int m_AttackDamageMax;	// 8
	int m_AttackSpeed;	// C
	int m_SuccessAttackRate;	// 10
	int m_iMasterIndex;	// 14
	int m_iTargetIndex;	// 18
	DWORD m_dwLastAttackTime;	// 1C
	enum ePetItem_Mode
	{
		PetItem_Mode_Normal = 0x0,
		PetItem_Mode_Attack_Random = 0x1,
		PetItem_Mode_Attack_WithMaster = 0x2,
		PetItem_Mode_Attack_Target = 0x3,

	} m_ActionMode;	// 20
	CItem * m_pPetItem;	// 24

public:

  CDarkSpirit();
  virtual ~CDarkSpirit();

  void Init();
  void Run();
  void ModeNormal();
  void ModeAttackRandom();
  void ModeAttackWithMaster();
  void ModeAttakTarget();
  void SetTarget(int aTargetIndex);
  void ReSetTarget(int aTargetIndex);
  void Set(int aIndex, CItem* pPetItem);
  void SetMode(ePetItem_Mode mode, int iTargetindex);
  int Attack(LPOBJ lpObj, LPOBJ lpTargetObj, CMagicInf* lpMagic, int criticaldamage, int iActionType);
  int GetAttackDamage(LPOBJ lpObj, int targetDefense, int bIsOnDuel,  int criticaldamage);
  int MissCheck(LPOBJ lpObj, LPOBJ lpTargetObj, int skill,  int skillSuccess, int& bAllMiss);
  int MissCheckPvP(LPOBJ lpObj, LPOBJ lpTargetObj, int skill,  int skillSuccess, int& bAllMiss);
  void ChangeCommand(int command, int targetindex);
  void RangeAttack(int aIndex, int aTargetIndex);
  void SendAttackMsg(int aIndex, int aTargetIndex, int criticaldamage, int iActionType);
  int GetShieldDamage(LPOBJ lpObj, LPOBJ lpTargetObj, int iAttackDamage);
 
  static void __cdecl CDarkSpirit::SendLevelmsg(int aIndex, int nPos, int PetType, int InvenType);
  static void __cdecl CDarkSpirit::SendExpmsg(int aIndex, int nPos, int PetType, int InvenType);
};

extern CDarkSpirit gDarkSpirit[OBJMAX];

#endif