#include "stdafx.h"
#include "Weapon.h"
#include "user.h"
#include "gObjMonster.h"
#include "ObjAttack.h"
#include "IllusionTemple.h"

// GS-N 0.99.60T 0x0051A5F0
//	GS-N	1.00.18	JPN	0x0054BF60	-	Completed

CWeapon g_CsNPC_Weapon;

#define MAX_ST_CS_WEAPON	(4)


struct ST_CS_WEAPON
{
	BYTE btXBegin;	// 0
	BYTE btYBegin;	// 1
	BYTE btXEnd;	// 2
	BYTE btYEnd;	// 3
};

static const ST_CS_WEAPON WEAPON_CS_ATTACKER[MAX_ST_CS_WEAPON] =
{
	0x3E, 0x67, 0x48, 0x70, 0x58, 0x68, 0x7C, 0x6F,
	0x74, 0x69, 0x7C, 0x70, 0x49, 0x56, 0x69, 0x67
};

static const ST_CS_WEAPON WEAPON_CS_DEFENSER[MAX_ST_CS_WEAPON] =
{
	0x3D, 0x58, 0x5D, 0x6C, 0x5C, 0x59, 0x7F, 0x6F,
	0x54, 0x34, 0x66, 0x42, 0x00, 0x00, 0x00, 0x00
};




CWeapon::CWeapon()
{
	//for ( int i=0;i<MAX_WEAPON_CAL_DAMAGER_INFO;i++)	// #error Cahnge to MAX_WEAPON_DAMAGED_TARGET_INFO
	//{
	//	this->m_WeaponDamagedTargetInfo[i].RESET();
	//}

	//for ( int i=0;i<MAX_WEAPON_DAMAGED_TARGET_INFO;i++)	// #error Cahnge to MAX_WEAPON_CAL_DAMAGER_INFO
	//{
	//	this->m_WeaponCalDamageInfo[i].RESET();
	//}
	for ( int i=0;i<MAX_WEAPON_DAMAGED_TARGET_INFO;i++)	// FIX
	{
		this->m_WeaponDamagedTargetInfo[i].RESET();
	}

	for ( int i=0;i<MAX_WEAPON_CAL_DAMAGER_INFO;i++)	// FIX
	{
		this->m_WeaponCalDamageInfo[i].RESET();
	}
}





CWeapon::~CWeapon()
{
	return;
}


BOOL CWeapon::SetWeaponCalDamageInfo(WORD wObjIndex, BYTE btTargetX, BYTE btTargetY, int iDelayTime)
{
	for ( int i=0;i<MAX_WEAPON_CAL_DAMAGER_INFO;i++)
	{
		if ( this->m_WeaponCalDamageInfo[i].m_IsUsed == FALSE )
		{
			this->m_WeaponCalDamageInfo[i].m_iWeaponObjIndex = wObjIndex;
			this->m_WeaponCalDamageInfo[i].m_iTargetX = btTargetX;
			this->m_WeaponCalDamageInfo[i].m_iTargetY = btTargetY;
			this->m_WeaponCalDamageInfo[i].m_iLimitTime = GetTickCount() + iDelayTime;
			this->m_WeaponCalDamageInfo[i].m_IsUsed = TRUE;

			return TRUE;
		}
	}

	return FALSE;
}


BOOL CWeapon::GetWeaponCalDamageInfo(WORD wObjIndex, BYTE & btTargetX, BYTE & btTargetY)
{
	for( int i=0;i<MAX_WEAPON_CAL_DAMAGER_INFO;i++)
	{
		if ( this->m_WeaponCalDamageInfo[i].m_IsUsed == TRUE && this->m_WeaponCalDamageInfo[i].m_iWeaponObjIndex == wObjIndex)
		{
			btTargetX = this->m_WeaponCalDamageInfo[i].m_iTargetX;
			btTargetY = this->m_WeaponCalDamageInfo[i].m_iTargetY;

			this->m_WeaponCalDamageInfo[i].RESET();

			return TRUE;
		}
	}

	return FALSE;
}



int  CWeapon::GetAttackDamage(int iObjClass)
{
	int iDamage = 0;

	if ( iObjClass == 221 )
	{
		iDamage = 300;
	}
	else if ( iObjClass == 222 )
	{
		iDamage = 200;
	}

	return iDamage;
}




BOOL CWeapon::GetTargetPointXY(int iObjClass, int iTargetPointIndex, BYTE &btX, BYTE &btY, BOOL bRandomPos)
{
	if ( iObjClass == 221 )
	{
		if ( iTargetPointIndex >= 0 && iTargetPointIndex < MAX_ST_CS_WEAPON )
		{
			int tx = WEAPON_CS_ATTACKER[iTargetPointIndex].btXEnd - WEAPON_CS_ATTACKER[iTargetPointIndex].btXBegin;
			int ty = WEAPON_CS_ATTACKER[iTargetPointIndex].btYEnd - WEAPON_CS_ATTACKER[iTargetPointIndex].btYBegin;

			if ( bRandomPos != FALSE )
			{
				btX = WEAPON_CS_ATTACKER[iTargetPointIndex].btXBegin + rand() % tx;
				btY = WEAPON_CS_ATTACKER[iTargetPointIndex].btYBegin + rand() % ty;
			}
			else
			{
				btX = WEAPON_CS_ATTACKER[iTargetPointIndex].btXBegin + tx / 2;
				btY = WEAPON_CS_ATTACKER[iTargetPointIndex].btYBegin + ty / 2;
			}
		}
	}

	if ( iObjClass == 222 )
	{
		if ( iTargetPointIndex >= 0 && iTargetPointIndex < MAX_ST_CS_WEAPON )
		{
			int tx = WEAPON_CS_DEFENSER[iTargetPointIndex].btXEnd - WEAPON_CS_DEFENSER[iTargetPointIndex].btXBegin;
			int ty = WEAPON_CS_DEFENSER[iTargetPointIndex].btYEnd - WEAPON_CS_DEFENSER[iTargetPointIndex].btYBegin;

			if ( bRandomPos != FALSE )
			{
				btX = WEAPON_CS_DEFENSER[iTargetPointIndex].btXBegin + rand() % tx;
				btY = WEAPON_CS_DEFENSER[iTargetPointIndex].btYBegin + rand() % ty;
			}
			else
			{
				btX = WEAPON_CS_DEFENSER[iTargetPointIndex].btXBegin + tx / 2;
				btY = WEAPON_CS_DEFENSER[iTargetPointIndex].btYBegin + ty / 2;
			}
		}
	}

	return TRUE;
}



BOOL CWeapon::MissCheck(LPOBJ lpObj, LPOBJ lpTargetObj, int iSkill, int iSkillSuccess, BOOL& bAllMiss)
{
	if ( lpTargetObj->m_SkillIT_Time > 0 )
	{
		if ( lpTargetObj->m_SkillIT_Number == IL_ORDER_OF_PROTECT )
		{
			bAllMiss = TRUE;

			GCDamageSend(lpObj->m_Index, lpTargetObj->m_Index, 0, 0, MSG_DAMAGE_MISS, 0);
			return FALSE;
		}
	}

	return TRUE;
}




BOOL CWeapon::Attack(LPOBJ lpObj, LPOBJ lpTargetObj, CMagicInf * lpMagic, int iCriticalDamage, int iActionType)
{
	int iSkill = 0;
	int iSkillSuccess = TRUE;
	BYTE MsgDamage = MSG_DAMAGE_MISS;
	int ManaChange = 0;

	if ( lpMagic != NULL )
	{
		iSkill = lpMagic->m_Skill;
	}

	/*if ((lpTargetObj->Authority & 2) == 2 )
	{
		return FALSE;
	}*/

	if ( lpObj->MapNumber != lpTargetObj->MapNumber )
	{
		return FALSE;
	}

	if ( lpTargetObj->Type == OBJ_USER )
	{
		if ( gObjIsConnected(lpTargetObj) == FALSE )
		{
			return FALSE;
		}
	}

	if ( gObjAttackType(lpTargetObj,lpObj) == 0)
		return FALSE;
	if ( gclassObjAttack.PkCheck(lpObj, lpTargetObj) == FALSE )
		return FALSE;
	if ( gclassObjAttack.CheckAttackArea(lpObj, lpTargetObj) == FALSE )
		return FALSE;

	//lpObj->m_TotalAttackCount++;

	int MSBFlag = 0;

	if ( iCriticalDamage != 0 )
	{
		MsgDamage = MSG_DAMAGE_CRITICAL;
	}
	else
	{
		MsgDamage = MSG_DAMAGE_MISS;
	}

	//int target_m_Defense = lpTargetObj->m_Defense;
	int iAttackDamage = this->GetAttackDamage(lpObj->Class);
	int iSkillDefense = lpTargetObj->m_SkillDefense;

	//TEST MODE! SKILLS!!!
	/*if (iSkill == AT_SKILL_DARKSIDE)
	{
		iAttackDamage = (lpObj->Dexterity/8 + lpObj->Energy/10 + iAttackDamage)* (100 + lpObj->Dexterity/8 + lpObj->Vitality/10)/100;
	}*/

	//if(lpTargetObj->Type == OBJ_USER && lpTargetObj->Class == CLASS_SUMMONER && lpTargetObj->m_SkillBerserkerTime > 0)
	//{
	//	target_m_Defense = lpTargetObj->m_Defense * lpTargetObj->m_SkillBerserkerDef;
	//}

	int iSkillReduceDefense = lpTargetObj->m_SkillReduceDefense + lpTargetObj->m_RFBufReduceDefense;
	int iTargetDefense = lpTargetObj->m_Defense + iSkillDefense - iSkillReduceDefense;

	if(lpObj->Type == OBJ_USER)
	{
		if(lpObj->m_SkillRedArmorIgnoreTime > 0)
		{
			//iTargetDefense -= lpObj->m_SkillRedArmorIgnoreNum;
			iTargetDefense = (iTargetDefense * (100 - lpObj->m_SkillRedArmorIgnoreNum)/100);
		}
	}

	iTargetDefense -= (iTargetDefense * lpTargetObj->m_SkillMagumReduceDefense)/100;

	if(iTargetDefense < 0) 
		iTargetDefense = 0;

	BOOL bAllMiss = FALSE;
	
	if ( this->MissCheck(lpObj, lpTargetObj, iSkill, iSkillSuccess, bAllMiss) == FALSE )
	{
		return FALSE;
	}

	if ( bAllMiss != FALSE )
	{
		iAttackDamage = (iAttackDamage * 30)/100;
	}

	if ( lpTargetObj->DamageMinus != 0 )
	{
		iAttackDamage -= (iAttackDamage * lpTargetObj->DamageMinus)/100;
	}

	int iTargetLevel = lpObj->Level / 10;

	if ( iAttackDamage < iTargetLevel )
	{
		if ( iTargetLevel < 1 )
		{
			iTargetLevel = 1;
		}

		iAttackDamage = iTargetLevel;
	}

	if ( lpTargetObj->m_SkillNumber == 18 )
	{
		if ( iAttackDamage > 1 )
		{
			iAttackDamage = iAttackDamage >> 1;
		}
	}

	if ( gObjAngelSprite(lpTargetObj) == TRUE )
	{
		if ( iAttackDamage > 1 )
		{
			float lc13 = (float)(iAttackDamage * 8 ) / 10.0f;
			iAttackDamage = (int)( lc13 );
		}
	}

	if ( gObjSpiritOfGuardianSprite(lpTargetObj) == TRUE )
	{
		if ( iAttackDamage > 1 )
		{
			float lc13 = (float)(iAttackDamage * 8 ) / 20.0f;
			iAttackDamage = (int)( lc13 );
		}
	}


	if ( gObjWingSprite(lpTargetObj) == TRUE )
	{
		CItem * Wing = &lpTargetObj->pInventory[7];

		if ( iAttackDamage > 1 )
		{
			if ( (Wing->m_Type > ITEMGET(12,2) && Wing->m_Type < ITEMGET(12,7)) || (Wing->m_Type == ITEMGET(12,42)) )
			{
				float lc15 = float((int)iAttackDamage *  (int)(75 - Wing->m_Level * 2) ) / 100.0f;
				iAttackDamage = (int )lc15;
			}
			else if ( (Wing->m_Type >= ITEMGET(12,0) && Wing->m_Type <= ITEMGET(12,2)) || (Wing->m_Type == ITEMGET(12,41)) )
			{
				float lc16 = float((int)iAttackDamage *  (int)(88 - Wing->m_Level * 2) ) / 100.0f;
				iAttackDamage = (int)lc16;
			}
			else if ( Wing->m_Type == ITEMGET(12,130))
			{
				float lc16 = float((int)iAttackDamage *  (int)(80 - Wing->m_Level * 2) ) / 100.0f;
				iAttackDamage = (int)lc16;
			}
			else if ( Wing->m_Type == ITEMGET(12,135))
			{
				float lc16 = float((int)iAttackDamage *  (int)(80 - Wing->m_Level * 2) ) / 100.0f;
				iAttackDamage = (int)lc16;
			}
			else if ( Wing->m_Type >= ITEMGET(12,131) && Wing->m_Type <= ITEMGET(12,134))
			{
				float lc16 = float((int)iAttackDamage *  (int)(88 - Wing->m_Level * 2) ) / 100.0f;
				iAttackDamage = (int)lc16;
			}
			else if ( (Wing->m_Type >= ITEMGET(12,36) && Wing->m_Type <= ITEMGET(12,39)) || (Wing->m_Type == ITEMGET(12,43)))
			{
				float lc16 = float((int)iAttackDamage *  (int)(61 - Wing->m_Level * 2) ) / 100.0f;
				iAttackDamage = (int)lc16;
			}
#if (CRYSTAL_EDITION == 1)
			else if ( (Wing->m_Type >= ITEMGET(12,200) && Wing->m_Type <= ITEMGET(12,254)) )
			{
				float lc16 = float((int)iAttackDamage *  (int)(61 - Wing->m_Level * 2) ) / 100.0f;
				iAttackDamage = (int)lc16;
			}
#endif
			else if ( (Wing->m_Type == ITEMGET(12,40)) || (Wing->m_Type == ITEMGET(12,50)))
			{
				float lc16 = float((int)iAttackDamage *  (int)(76 - Wing->m_Level * 2) ) / 100.0f;
				iAttackDamage = (int)lc16;
			}
			else if ( Wing->m_Type == ITEMGET(13,30) )	// Cape Of Lord
			{
				//No such option
			}
			else if ( Wing->m_Type == ITEMGET(12,49) )	// Cape Of Lord
			{
				//No such option
			}
		}
	}

	if ( gObjDenorantSprite(lpTargetObj) != FALSE )
	{
		CItem * Dino = &lpTargetObj->pInventory[8];

		int ReduceDamage = 90 - Dino->IsDinorantReduceAttackDamaege();

		lpObj->Life -= 1.0f;

		if ( lpObj->Life < 0.0f )
		{
			lpObj->Life = 0;
		}
		else
		{
			iAttackDamage = (iAttackDamage * ReduceDamage)/100;
		}

		GCReFillSend(lpObj->m_Index, lpObj->Life, (BYTE)-1, 0, lpObj->iShield);
	}

	if ( gObjDarkHorse(lpTargetObj) != FALSE )
	{
		CItem * DarkHorse = &lpTargetObj->pInventory[8];

		int DHPercent = 100-(DarkHorse->m_PetItem_Level+30)/2;

		lpTargetObj->Life -= 1.0f;

		if ( lpTargetObj->Life < 0.0f )
		{
			lpTargetObj->Life = 0;
		}
		else
		{
			iAttackDamage = (iAttackDamage * DHPercent)/100;
		}

		GCReFillSend(lpTargetObj->m_Index, lpTargetObj->Life, (BYTE)-1, 0, lpTargetObj->iShield);
	}

	if ( lpTargetObj->Live != FALSE )
	{
		if ( lpTargetObj->m_WizardSkillDefense !=0 && iAttackDamage > 0 )
		{
			int ManaPercent = (WORD)lpTargetObj->Mana * 2 / 100;

			if ( ManaPercent < lpTargetObj->Mana )
			{
				lpTargetObj->Mana -= ManaPercent;
				int lc22 = (iAttackDamage * lpTargetObj->m_WizardSkillDefense )/100;

				iAttackDamage -= lc22;
				ManaChange = TRUE;
			}
		}

		lpTargetObj->Life -= iAttackDamage;

		if ( lpTargetObj->Life < 0.0f )
		{
			lpTargetObj->Life = 0;
		}
	}

	if ( iAttackDamage >= 1 )
	{
		if ( lpTargetObj->Type == OBJ_USER )
		{
			gObjArmorRandomDurDown(lpTargetObj, lpObj, true);
		}
	}

	if ( iAttackDamage >= 5 )
	{
		if ( lpTargetObj->Type == OBJ_MONSTER )
		{
			if ( (rand()%26) == 0 )
			{
				gObjAddMsgSendDelay(lpTargetObj, 4, lpObj->m_Index, 100, 0);
			}
		}
		else if ( (rand()%4) == 0 )
		{
			if ( gObjUniriaSprite(lpTargetObj) == FALSE )
			{
				MSBFlag = TRUE;
			}
		}
	}

	if ( ManaChange != FALSE )
	{
		GCManaSend(lpTargetObj->m_Index, lpTargetObj->Mana, (BYTE)-1, 0, lpTargetObj->BP);
	}

	lpObj->m_Rest = 0;

	if ( iAttackDamage > 0 )
	{
		gObjLifeCheck(lpTargetObj, lpObj, iAttackDamage, DAMAGE_TYPE_REG, MSBFlag, MsgDamage, iSkill, 0);
	}

	return TRUE;
}



BOOL CWeapon::AddWeaponDamagedTargetInfo(int iWeaponIndex, int iTargetIndex, int iDelayTime)
{
	for ( int i=0;i<MAX_WEAPON_DAMAGED_TARGET_INFO;i++)
	{
		if ( this->m_WeaponDamagedTargetInfo[i].m_IsUsed == FALSE )
		{
			this->m_WeaponDamagedTargetInfo[i].m_iWeaponObjIndex = iWeaponIndex;
			this->m_WeaponDamagedTargetInfo[i].m_iTargetObjIndex = iTargetIndex;
			this->m_WeaponDamagedTargetInfo[i].m_iDamageDealingTime = GetTickCount() + iDelayTime;
			this->m_WeaponDamagedTargetInfo[i].m_IsUsed = TRUE;

			return TRUE;
		}
	}

	return FALSE;
}

	

void CWeapon::WeaponAttackProc()
{
	for ( int i=0;i<MAX_WEAPON_DAMAGED_TARGET_INFO;i++)
	{
		if ( this->m_WeaponDamagedTargetInfo[i].m_IsUsed == TRUE )
		{
			if ( this->m_WeaponDamagedTargetInfo[i].m_iDamageDealingTime > GetTickCount() )
			{
				continue;
			}

			LPOBJ lpTargetObj = &gObj[this->m_WeaponDamagedTargetInfo[i].m_iTargetObjIndex];
			LPOBJ lpWeaponObj = & gObj[this->m_WeaponDamagedTargetInfo[i].m_iWeaponObjIndex];

			if ( gObjIsConnected(lpTargetObj) == FALSE )
			{
				this->m_WeaponDamagedTargetInfo[i].RESET();
				continue;
			}

			if (  OBJMAX_RANGE(lpWeaponObj->m_Index) == FALSE )
			{
				this->m_WeaponDamagedTargetInfo[i].RESET();
				continue;
			}

			this->Attack(lpWeaponObj, lpTargetObj, NULL, FALSE, 0);

			this->m_WeaponDamagedTargetInfo[i].RESET();
		}
	}

}



void CWeapon::WeaponAct(int iIndex)
{
	for ( int i=0;i<MAX_WEAPON_CAL_DAMAGER_INFO;i++)
	{
		if ( this->m_WeaponCalDamageInfo[i].m_IsUsed == TRUE )
		{
			if ( this->m_WeaponCalDamageInfo[i].m_iWeaponObjIndex == iIndex )
			{
				if ( this->m_WeaponCalDamageInfo[i].m_iLimitTime < GetTickCount() )
				{
					this->m_WeaponCalDamageInfo[i].RESET();
				}
			}
		}
	}
}