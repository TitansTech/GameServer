#ifndef OBJBASEATTACK_H
#define OBJBASEATTACK_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "user.h"


class CObjBaseAttack
{

public:

	CObjBaseAttack();
	virtual ~CObjBaseAttack();

	int DecreaseArrow(LPOBJ lpObj);
	int CheckAttackArea(LPOBJ lpObj, LPOBJ lpTargetObj);
	int PkCheck(LPOBJ lpObj, LPOBJ lpTargetObj);
	int ResistanceCheck(LPOBJ lpObj, LPOBJ lpTargetObj, int skill);
	int MissCheckPvP(LPOBJ lpObj, LPOBJ lpTargetObj, int skill, int skillSuccess, int magicsend, BOOL& bAllMiss);
	int GetTargetDefense(LPOBJ lpObj, LPOBJ lpTargetObj, unsigned char& MsgDamage);
	int GetPartyMemberCount(LPOBJ lpObj);

	virtual int MissCheck(LPOBJ lpObj, LPOBJ lpTargetObj, int skill, int skillSuccess, int magicsend, BOOL& bAllMiss);	// 4


};

#endif