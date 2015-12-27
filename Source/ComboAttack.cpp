#include "stdafx.h"
#include "ComboAttack.h"
#include "user.h"
// GS-N 0.99.60T 0x004A42C0 : Completed
//	GS-N	1.00.18	JPN	0x004C2270	-	Completed

CComboAttack gComboAttack;


CComboAttack::CComboAttack()
{
	return;
}

CComboAttack::~CComboAttack()
{
	return;
}

int CComboAttack::GetSkillPos(int skillnum)
{
	switch (skillnum)
	{
		case 19:
		case 20:
		case 21:
		case 22:
		case 23:
			return 0;
			break;
		case 455:
		case 456:
		case 457:
		case 458:
		case 459:
		case 495:
		case 496:
		case 497:
		case 498:
		case 499:
		case 41:
		case 42:
		case 465:
		case 466:
		case 467:
		case 468:
		case 469:
		case 43:
		case 460:
		case 461:
		case 462:
		case 463:
		case 464:
		case 232:
			return 1;
			break;
		default:
			return -1;
			break;
	}
}


BOOL CComboAttack::CheckCombo(int aIndex, int skillnum)
{
	LPOBJ lpObj = &gObj[aIndex];
	
	if ( lpObj->ComboSkillquestClear  != false )
	{
		int sp = this->GetSkillPos(skillnum);

		if ( sp == 0 )
		{
			lpObj->comboSkill.ProgressIndex  = 0;
			lpObj->comboSkill.dwTime = GetTickCount() + 3000;
			lpObj->comboSkill.Skill[0]  = skillnum;
		}
		else if ( sp == 1 )
		{
			if ( lpObj->comboSkill.Skill[0]  == 0xFF )
			{
				lpObj->comboSkill.Init();
				return 0;
			}

			//int Time =GetTickCount();

			if (lpObj->comboSkill.dwTime < GetTickCount() )
			{
				lpObj->comboSkill.Init();
				return 0;
			}

			if ( lpObj->comboSkill.ProgressIndex == 0 )
			{
				lpObj->comboSkill.ProgressIndex = 1;
				lpObj->comboSkill.dwTime = GetTickCount() + 3000;
				lpObj->comboSkill.Skill[1] = skillnum;
			}
			else if ( lpObj->comboSkill.Skill[1]  != skillnum )
			{
				lpObj->comboSkill.Init();
				return 1;
			}
			else
			{
				lpObj->comboSkill.Init();
			}
		}
		else
		{
			lpObj->comboSkill.ProgressIndex= -1;
			lpObj->comboSkill.dwTime = 0;
			lpObj->comboSkill.Skill[0] = -1;
		}
	}

	return 0;
}
			


