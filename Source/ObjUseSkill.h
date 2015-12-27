#ifndef OBJUSESKILL_H
#define OBJUSESKILL_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MagicInf.h"
#include "user.h"

class CObjUseSkill
{

public:

	CObjUseSkill();
	virtual ~CObjUseSkill();

	int SpeedHackCheck(int aIndex, BYTE functionID);
	int EnableSkill(WORD Skill);
	int GetUseMana(int aIndex, CMagicInf* lpMagic);
	int GetAddUseMana(LPOBJ lpObj, CMagicInf* lpMagic);
	int GetUseBP(int aIndex, CMagicInf* lpMagic);
	void UseSkill(int aIndex, int aTargetIndex, CMagicInf* lpMagic);
	void UseSkill(int aIndex, CMagicInf* lpMagic, BYTE x, BYTE y, BYTE dir, BYTE TargetPos, int aTargetIndex);
	int RunningSkill(int aIndex, int aTargetIndex, CMagicInf* lpMagic, BOOL bCombo);
	void MaGumSkillDefenseDown(int aIndex, int aTargetIndex, int skill_level, int MagicNr);
	void KnightSkillAddLife(int aIndex, int skill_level, int MagicNumber);
	void WizardMagicDefense(int aIndex, int aTargetIndex, int skill_level, WORD SkillNr);
	void SkillDefense(int aIndex, int aTargetIndex, int skill_level, WORD SkillNr);
	void SkillAttack(int aIndex, int aTargetIndex, int skill_level, WORD SkillNr);
	void Berserker(int aIndex, int skill_level);
	void SkillReflect(int aIndex, int aTargetIndex, int skill_level);
	void SkillMagicCircle(int aIndex, int aTargetIndex, int skill_level);
	void SkillReduceDamage(int aIndex, int aTargetIndex, int skill_level);
	void SkillReduceDefense(int aIndex, int aTargetIndex, int skill_level);
	void SkillSleepRemove(LPOBJ lpObj);
	void SkillSleep(int aIndex, int aTargetIndex, int skill_level,int MagicNumber, CMagicInf* lpMagic);
	void RemoveSkillNight(int aIndex);
	void SkillNight(int aIndex, int aTargetIndex, int skill_level);
	int SkillChangeUse(int aIndex);
	void SkillHealing(int aIndex, int aTargetIndex, int skill_level, WORD SkillNr);
	void SkillHealShield(int aIndex, int aTargetIndex, int skill_level);
	int SkillMonsterCall(int aIndex, int MonsterType, int x, int y);
	int SkillDeathPoison(int aIndex, CMagicInf* lpMagic, BYTE x, BYTE y, BYTE dir, BYTE TargetPos, int aTargetIndex);
	int SkillSuddenIce(int aIndex, CMagicInf* lpMagic, BYTE x, BYTE y, BYTE dir, BYTE TargetPos, int aTargetIndex);
	int SkillSwordSlash(int aIndex, CMagicInf * lpMagic, BYTE x, BYTE y, BYTE dir, BYTE TargetPos, int aTargetIndex);
	int SkillLightingStorm(int aIndex, CMagicInf * lpMagic, BYTE x, BYTE y, BYTE dir, BYTE TargetPos, int aTargetIndex);
	int SkillSummonerBook(int aIndex, CMagicInf * lpMagic, BYTE x, BYTE y, BYTE dir, BYTE TargetPos, int aTargetIndex, BYTE Book);
	int SkillRedStorm(int aIndex, CMagicInf * lpMagic, BYTE x, BYTE y, BYTE dir, BYTE TargetPos, int aTargetIndex);
	int SkillExplotion(int aIndex, CMagicInf * lpMagic, BYTE x, BYTE y, BYTE dir, BYTE TargetPos, int aTargetIndex);
	int SkillHellFire2Start(int aIndex, CMagicInf* lpMagic);
	int SkillHellFire2(int aIndex, int aTargetIndex, CMagicInf* lpMagic);
	void SkillBlowOfFury(int aIndex, CMagicInf* lpMagic, BYTE x, BYTE y, BYTE dir, BYTE TargetPos, int aTargetIndex, BOOL isCombo);
	void SkillWheel(int aIndex, CMagicInf* lpMagic, int aTargetIndex, BOOL isCombo);
	int SkillKnightBlow(int aIndex, int aTargetIndex, CMagicInf* lpMagic, BOOL isCombo);
	void SkillPowerSlash(int aIndex, CMagicInf* lpMagic, BYTE x, BYTE y, BYTE dir, BYTE Targetangle, int aTargetIndex);
	void SkillDrainLife(int aIndex, int aTargetIndex, CMagicInf * lpMagic);
	void SkillPhoenixShot(int aIndex, int aTargetIndex, CMagicInf * lpMagic);
	void SkillCommon(int aIndex, int aTargetIndex, CMagicInf * lpMagic);
	int SkillChainElectricAttack(int aIndex,int aTargetIndex,CMagicInf * lpMagic);
	void SkillFrustrum(int aIndex, float fangle, float tx, float ty);
	int GetTargetLinePath(int sx, int sy, int tx, int ty,  int* PathX,int* PathY, int distance);
	int GetAngle(int x, int y, int tx,  int ty);
	int CalDistance(int x, int y, int x1, int y1);
	void SkillFrustrum2(int aIndex, float fangle, float tx, float ty);
	int SkillAreaCheck(int angel, int x, int y,  int tx, int ty,  int dis,  int arc);
	int SkillSpear(int aIndex, int aTargetIndex, CMagicInf* lpMagic);
	int SkillFireBurst(int aIndex, int aTargetIndex, CMagicInf* lpMagic);
	int SkillDarkHorseAttack(int aIndex, int aTargetIndex, CMagicInf* lpMagic);
	int SkillGhostPhantom(int aIndex, int aTargetIndex, CMagicInf * lpMagic);
	int RecallPartyCheck(int aIndex, int skill_level);
	void SkillRecallParty(int aIndex, int skill_level);
	void SkillAddCriticalDamage(int aIndex, int skill_level);
	void SkillElectricSpark(int aIndex, CMagicInf* lpMagic, BYTE x, BYTE y, BYTE dir, BYTE TargetPos, int aTargetIndex);
	void SkillBirds(int aIndex, CMagicInf * lpMagic, BYTE x, BYTE y, BYTE dir, BYTE TargetPos, int aTargetIndex);
	int SkillSummon(int aIndex, int aTargetIndex, CMagicInf* lpMagic);
	int SkillImmuneToMagic(int aIndex, CMagicInf* lpMagic);
	int SkillImmuneToHarm(int aIndex, CMagicInf* lpMagic);
	void GladiatorsGlory(int iSkillIcon, int aIndex,BOOL state);
	int SkillKnightRush(int aIndex, int aTargetIndex, CMagicInf* lpMagic);
	int SkillWizardJavalin(int aIndex, int aTargetIndex, CMagicInf* lpMagic);
	int SkillElfRush(int aIndex, int aTargetIndex, CMagicInf* lpMagic);
	int SkillMagumOneFlash(int aIndex, int aTargetIndex, CMagicInf* lpMagic);
	int SkillMagumDeathCannon(int aIndex, int aTargetIndex, CMagicInf* lpMagic);
	int SkillDarklordSpaceSplit(int aIndex, int aTargetIndex, CMagicInf* lpMagic);
	int SkillBrandOfSkill(int aIndex, int aTargetIndex, CMagicInf* lpMagic);
	int SkillStun(int aIndex, int aTargetIndex);
	int SkillRemoveStun(int aIndex, int aTargetIndex, CMagicInf* lpMagic);
	int SkillAddMana(int aIndex, int aTargetIndex, CMagicInf* lpMagic);
	int SkillCloaking(int aIndex, int aTargetIndex, CMagicInf* lpMagic);
	int SkillRemoveCloaking(int aIndex, int aTargetIndex, CMagicInf* lpMagic);
	int SkillRemoveMagic(int aIndex, int aTargetIndex, CMagicInf* lpMagic);
	int RemoveCloakingEffect(int aIndex);
	int SkillStun(int aIndex, int aTargetIndex, CMagicInf* lpMagic, BYTE btX, BYTE btY, BYTE btDir, BYTE btTargetPos);
	int RemoveAllCharacterInvalidMagicAndSkillState(LPOBJ lpObj);
	int RemoveSpecificSkillState(LPOBJ lpObj, int iSkillNumber);
	int SkillFenrirAttack(int aIndex, int aTargetIndex, CMagicInf* lpMagic);
	int SkillInfinityArrow(int aIndex, int aTargetIndex, CMagicInf* lpMagic);
	int SkillFireScream(int aIndex, int aTargetIndex, CMagicInf* lpMagic);
	int FireScreamExplosionAttack(LPOBJ lpObj, LPOBJ lpTargetObj, int iAttackDamage);
	void SpecificSkillAdditionTreat(LPOBJ lpObj, LPOBJ lpTargetObj, CMagicInf* lpMagic, int iAttackDamage);

	//Rage Fighter
	
	bool SkillChainDrive(int aIndex, int aTargetIndex, CMagicInf * lpMagic);
	bool SkillLargeRingBlower(int aIndex, int aTargetIndex, CMagicInf * lpMagic);
	bool SkillUpperBeast(int aIndex, int aTargetIndex, CMagicInf * lpMagic);
	bool SkillDragonLore(int aIndex, CMagicInf * lpMagic, BYTE x, BYTE y, BYTE dir, BYTE TargetPos, int aTargetIndex);
	bool SkillDragonSlayer(int aIndex, int aTargetIndex, CMagicInf * lpMagic);
	bool SkillRedArmorIgnore(int aIndex, CMagicInf * lpMagic);
	bool SkillFitness(int aIndex, CMagicInf * lpMagic);
	bool SkillDefSuccessRateInc(int aIndex, CMagicInf * lpMagic);
	bool SkillDarkSideStart(int aIndex, int aTargetIndex, CMagicInf * lpMagic);
	bool SkillDarkSideEnd(int aIndex, int aTargetIndex, CMagicInf * lpMagic);
};


int  DecPartyMemberHPandMP(LPOBJ lpObj);

extern CObjUseSkill gObjUseSkill;

#endif