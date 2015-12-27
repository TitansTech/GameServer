#ifndef OBJATTACK_H
#define OBJATTACK_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MagicInf.h"
#include "ObjBaseAttack.h"

#define MSG_DAMAGE_MISS			0x00
#define MSG_DAMAGE_REGULAR		0x01
#define MSG_DAMAGE_EXCELLENT	0x02
#define MSG_DAMAGE_CRITICAL		0x03
#define MSG_DAMAGE_REFLECT		0x04
#define MSG_DAMAGE_BEAT			0x05		//NOT SURE about this one

#define MSG_DAMAGE_DOUBLE		0x40
#define MSG_DAMAGE_COMBO		0x80

#define DAMAGE_TYPE_REG			0x00
#define DAMAGE_TYPE_DELAY		0x01
#define DAMAGE_TYPE_BEAT		0x02

class CObjAttack : public CObjBaseAttack
{

public:

	CObjAttack();
	virtual ~CObjAttack();

	BOOL Attack(LPOBJ lpObj, LPOBJ lpTargetObj, CMagicInf* lpMagic,  int magicsend, unsigned char MSBFlag, int AttackDamage, BOOL bCombo);
	int GetAttackDamage(LPOBJ lpObj, int targetDefense, BYTE& effect, BOOL bIsOnDuel, CMagicInf* lpMagic);
	int GetAttackDamageWizard(LPOBJ lpObj, int targetDefense, CMagicInf* lpMagic, BYTE& effect, BOOL bIsOnDuel);
	int GetShieldDamage(LPOBJ lpObj, LPOBJ lpTargetObj, int iAttackDamage);

};

BOOL gObjDenorantSprite(LPOBJ lpObj);
BOOL gObjDarkHorse(LPOBJ lpObj);
BOOL gObjFenrir(LPOBJ lpObj);

extern CObjAttack gclassObjAttack;

#endif