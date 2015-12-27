#ifndef MAGICDAMAGE_H
#define MAGICDAMAGE_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAX_SKILL			65536
#define MAX_REQ_SKILL_STATUS	3

#include "..\common\classdef.h"

#define SKILL_SKILLTREE_START	300
#define SKILL_SKILLTREE_END		554

enum eSkillType
{
	AT_SKILL_POISON = 0x1,
	AT_SKILL_METEO = 0x2,
	AT_SKILL_THUNDER = 0x3,
	AT_SKILL_FIREBALL = 0x4,
	AT_SKILL_FLAME = 0x5,
	AT_SKILL_TELEPORT = 0x6,
	AT_SKILL_SLOW = 0x7,
	AT_SKILL_STORM = 0x8,
	AT_SKILL_EVIL = 0x9,
	AT_SKILL_HELL = 0xa,
	AT_SKILL_POWERWAVE = 0xb,
	AT_SKILL_FLASH = 0xc,
	AT_SKILL_BLAST = 0xd,
	AT_SKILL_INFERNO = 0xe,
	AT_SKILL_TARGET_TELEPORT = 0xf,
	AT_SKILL_MAGICDEFENSE = 0x10,
	AT_SKILL_ENERGYBALL = 0x11,
	AT_SKILL_BLOCKING = 0x12,
	AT_SKILL_FALLINGSLASH = 0x13,
	AT_SKILL_LUNGE = 0x14,
	AT_SKILL_UPPERCUT = 0x15,
	AT_SKILL_CYCLONE = 0x16,
	AT_SKILL_SLASH = 0x17,
	AT_SKILL_CROSSBOW = 0x18,
	AT_SKILL_BOW = 0x19,
	AT_SKILL_HEALING = 0x1a,
	AT_SKILL_DEFENSE = 0x1b,
	AT_SKILL_ATTACK = 0x1c,
	AT_SKILL_CALLMON1 = 0x1e,
	AT_SKILL_CALLMON2 = 0x1f,
	AT_SKILL_CALLMON3 = 0x20,
	AT_SKILL_CALLMON4 = 0x21,
	AT_SKILL_CALLMON5 = 0x22,
	AT_SKILL_CALLMON6 = 0x23,
	AT_SKILL_CALLMON7 = 0x24,
	AT_SKILL_TWISTINGSLASH = 0x29,
	AT_SKILL_BLOWOFFURY = 0x2a,
	AT_SKILL_DEATHSTAB = 0x2b,
	AT_SKILL_BKIMPALE = 0x2f,
	AT_SKILL_KNIGHTADDLIFE = 0x30,
	AT_SKILL_KNIGHTDINORANT = 0x31,
	AT_SKILL_ICEARROW = 0x33,
	AT_SKILL_PENETRATION = 0x34,
	AT_SKILL_FIRESLASH = 0x37,
	AT_SKILL_POWERSLASH = 0x38,
	AT_SKILL_5CROSSBOW = 0x36,				//Skill Does not exist
	AT_SKILL_EXPPOISON = 0x26,
	AT_SKILL_EXPICE = 0x27,
	AT_SKILL_EXPHELL = 0x28,
	AT_SKILL_EXPHELL_START = 0x3a,
	AT_SKILL_IMPROVE_AG_REFILL = 0x35,
	AT_SKILL_DEVILFIRE = 0x32,
	AT_SKILL_COMBO = 0x3b,
	AT_SKILL_SPEAR = 0x3c,
	AT_SKILL_FIREBURST = 0x3d,
	AT_SKILL_DARKHORSE_ATTACK = 0x3e,
	AT_SKILL_RECALL_PARTY = 0x3f,
	AT_SKILL_ADD_CRITICALDAMAGE = 0x40,
	AT_SKILL_ELECTRICSPARK = 0x41,
	AT_SKILL_LONGSPEAR = 0x42,
	AT_SKILL_CRESCENTSLASH = 0x2c,
	AT_SKILL_JAVALIN = 0x2d,
	AT_SKILL_STARFALL = 0x2e,
	AT_SKILL_SPIRALSLASH = 0x39,
	AT_SKILL_DEATH_CANNON = 0x49,
	AT_SKILL_SPACE_SPLIT = 0x4a,
	AT_SKILL_BRAND_OF_SKILL = 0x4b,
	AT_SKILL_STUN = 0x43,
	AT_SKILL_REMOVAL_STUN = 0x44,
	AT_SKILL_ADD_MANA = 0x45,
	AT_SKILL_INVISIBLE = 0x46,
	AT_SKILL_REMOVAL_INVISIBLE = 0x47,
	AT_SKILL_REMOVAL_MAGIC = 0x48,
	AT_SKILL_FENRIR_ATTACK = 0x4c,
	AT_SKILL_INFINITY_ARROW = 0x4d,
	AT_SKILL_FIRESCREAM = 0x4e,
	AT_SKILL_EXPLOSION = 0x4f,
	AT_SKILL_SUMMON = 0xc8,
	AT_SKILL_IMMUNE_TO_MAGIC = 0xc9,
	AT_SKILL_IMMUNE_TO_HARM = 0xca,
	
	AT_SKILL_DRAIN_LIFE = 0xD6,
	AT_SKILL_CHAIN_LIGHTING = 0xD7,
	AT_SKILL_ELECTRIC_SURGE = 0xD8,
	AT_SKILL_REFLECT = 0xD9,
	AT_SKILL_BERSERKER = 0xDA,
	AT_SKILL_SLEEP = 0xDB,
	AT_SKILL_NIGHT = 0xDC,
	AT_SKILL_REDUCE_DAMAGE = 0xDD,
	AT_SKILL_REDUCE_DEFENSE = 0xDE,
	AT_SKILL_SAHAMUTT = 0xDF,
	AT_SKILL_NAIL = 0xE0,
	AT_SKILL_GHOST_PHANTOM = 0xE1,	
	AT_SKILL_RED_STORM = 0xE6,
	AT_SKILL_EXPLOTION = 0xE8,
	AT_SKILL_MAGIC_CIRCLE = 0xE9,
	AT_SKILL_REDUCE_DMG_SHIELD = 0xEA,
	AT_SKILL_FIVE_SHOT = 0xEB,
	AT_SKILL_SWORD_SLASH = 0xEC,
	AT_SKILL_LIGHTING_STORM = 0xED,
	AT_SKILL_BIRDS = 0xEE,
	AT_SKILL_LARGERINGBLOWER= 260,
	AT_SKILL_UPPERBEAST= 261,
	AT_SKILL_CHAINDRIVE= 262,
	AT_SKILL_DARKSIDE= 263,
	AT_SKILL_DRAGONLORE= 264,
	AT_SKILL_DRAGONSLAYER= 265,
	AT_SKILL_REDADMORIGNORE= 266,
	AT_SKILL_FITNESS= 267,
	AT_SKILL_DEFENSESUCCESSRATEINCREASE= 268,
	AT_SKILL_CHARGE= 269,
	AT_SKILL_PHOENIX_G= 270,
};


class CMagicDamage
{

private:

	int Get(int skill);
	void SetEx(int iSkill, int iSkillUseType, int iSkillBrand, int iKillCount, int* pReqStatus);
	void Set(char* name, int skill, int damage,  int rlevel,  int mana,  int bp,  int dis, int Attr,  int iSkillType, unsigned char* ReqClass, int rEnergy, int rLeadership, int rdelay);
	int CheckSkillAttr(int skill, int attr);

public:

	CMagicDamage();
	virtual ~CMagicDamage();

	void Init();
	void LogSkillList(char* Buffer, int iSize);
	void LogSkillList(char* filename);
	void LogSkillNameList(char* Buffer, int iSize);
	void LogSkillNameList(char* filename);
	int SkillGet(int skill);
	int SkillGetMana(int skill);
	int SkillGetRequireEnergy(int skill);
	int SkillGetRequireLevel(int skill);
	int SkillGetRequireClass(int Cclass, int ChangeUP, int skill);
	int SkillGetBP(int skill);
	int GetSkillResistance(int skill);
	int GetskillRequireLeadership(int skill);
	int GetDelayTime(int skill);
	int GetSkillType(int iSkill);
	int CheckStatus(int iSkill, int iGuildStatus);
	//int CheckBrandOfSkill(int iSkill);
	int CheckKillCount(int iSkill, int iKillCount);
	int GetSkillDistance(int skill);
	char * GetName(int skill);

private:

	int m_Damage[MAX_SKILL];	// 4
	char m_Name[MAX_SKILL][50];	// 404
	//unsigned char m_rLevel[MAX_SKILL];	// 3604
	unsigned short m_Mana[MAX_SKILL];	// 3704
	unsigned char m_Distance[MAX_SKILL];	// 3904
	short m_RequireEnergy[MAX_SKILL];	// 3A04
	unsigned char m_RequireClass[MAX_SKILL][MAX_TYPE_PLAYER];	// 3E04
	short m_RequireLevel[MAX_SKILL];		// 4304
	short m_RequireLeadership[MAX_SKILL];	// 4704
	//int m_Delay[MAX_SKILL];			// 4B04
	unsigned char m_BrainPower[MAX_SKILL];		// 4F04
	unsigned char m_Resistance[MAX_SKILL];				// 5104
	unsigned char m_iSkillType[MAX_SKILL];		// 5504
	unsigned char m_iSkillUseType[MAX_SKILL];	// 5904
	//int m_iSkillBrand[MAX_SKILL];		// 5D04
	WORD m_iKillCount[MAX_SKILL];		// 6104
	BYTE m_iRequireStatus[MAX_SKILL][MAX_REQ_SKILL_STATUS];	// 6504

	enum 
	{
		SKILL_TYPE_NONE = 0xffff,
		SKILL_TYPE_PHYSICS = 0x0,
		SKILL_TYPE_MAGIC = 0x1,
	};

};

extern CMagicDamage MagicDamageC;

#endif