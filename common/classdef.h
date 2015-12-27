// ------------------------------
// Decompiled by Deathway
// Date : 2007-05-09
// ------------------------------
#ifndef CLASSDEF_H
#define CLASSDEF_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "zzzitem.h"


#define MAX_PLAYER_EQUIPMENT 12	// need to redo agin aññ with this macro


enum ClassNumber {
  CLASS_WIZARD = 0x0,
  CLASS_KNIGHT = 0x1,
  CLASS_ELF = 0x2,
  CLASS_MAGICGLADIATOR = 0x3,
  CLASS_DARKLORD = 0x4,
  CLASS_SUMMONER = 0x5,
  CLASS_RAGEFIGHTER = 0x6,
};




struct DEFAULTCLASSTYPE
{
	int Experience;	// 0
	WORD Strength;	// 4
	WORD Dexterity;	// 6
	WORD Vitality;	// 8
	WORD Energy;	// A
	float LevelLife;	// C
	float Life;	// 10
	float MaxLife;	// 14
	float LevelMana;	// 18
	float Mana;	// 1C
	float MaxMana;	// 20
	float VitalityToLife;	// 24
	float EnergyToMana;	// 28
	CItem Equipment[MAX_PLAYER_EQUIPMENT+2];	// 2C
	WORD Leadership;	// 9CC
};



class classdef
{
public:

	classdef();
	virtual ~classdef();

	void Init();
	void SetCharacter(int Class, int Str, int Dex, int Vit, int Energy, float Life, float Mana, float LevelLife, float LevelMana, float VitalityToLife, float EnergyToMana, int Leadership);
	void SetEquipment(int Class);
	int GetDefPoint(int char_class);
	void SetMabubBanjiOption(BOOL Flag);

public:

	DEFAULTCLASSTYPE DefClass[MAX_TYPE_PLAYER];	// 4
	BOOL m_BabubBanJiCreate; // 3114
	
};

#endif