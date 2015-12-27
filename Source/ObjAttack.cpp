#include "stdafx.h"
#include "ObjAttack.h"
#include "BattleSoccerManager.h"
#include "DarkSpirit.h"
#include "ObjUseSkill.h"
#include "gObjMonster.h"
#include "BloodCastle.h"
#include "ChaosCastle.h"
#include "DuelManager.h"
#include "ImperialGuardian.h"
#include "IllusionTemple.h"
#include "LogToFile.h"
#include "CustomWings.h"

#include "Gamemain.h"
#include "CastleSiegeSync.h"
#include "ObjBotPet.h"
// GS-N 0.99.60T 0x00496730
//	GS-N	1.00.18	JPN	0x004B1740	-	Completed
#if (GS_CASTLE==1)
#include "GameServer.h"
#include "Crywolf.h"
#endif

extern CLogToFile ANTI_HACK_LOG;
CObjAttack gclassObjAttack;


CObjAttack::CObjAttack()
{
	return;
}


CObjAttack::~CObjAttack()
{
	return;
}


BOOL CObjAttack::Attack(LPOBJ lpObj, LPOBJ lpTargetObj, CMagicInf* lpMagic,  int magicsend, unsigned char MSBFlag, int AttackDamage, BOOL bCombo)
{
	//if(OBJMAX_RANGE(lpObj->m_Index)==false || OBJMAX_RANGE(lpTargetObj->m_Index)==false)
	//{
	//	return FALSE
	//}

	int skillSuccess = 0;
	LPOBJ lpCallObj;
	LPOBJ lpCallTargetObj;
	unsigned char MsgDamage = MSG_DAMAGE_MISS;
	int ManaChange = 0;
	int iTempShieldDamage = 0;
	int iTotalShieldDamage = 0;

#if (PACK_EDITION>=3)

	if(lpTargetObj->IsBot > 1)
	{
		return false;
	}

	bool BotChangeSkill = false;

	if(ReadConfig.S6E2 == 1)
	{
		if ( lpObj->Type == OBJ_USER )
		{
			if ( lpObj->Class == CLASS_KNIGHT )
			{							
				if(OBJMAX_RANGE(lpTargetObj->m_Index) == true)
				{
					if ( lpTargetObj->Connected >= PLAYER_CONNECTED )
					{
						if(lpTargetObj->m_iSkillStunTime <= 0)
						{
							if(lpObj->MasterCharacterInfo->MaceStun > 0)
							{
								CItem * Right = &lpObj->pInventory[0];
								CItem * Left = &lpObj->pInventory[1];
								if((Right->IsItem() && (Right->m_Type >= ITEMGET(2,0) && Right->m_Type < ITEMGET(3,0))) || (Left->IsItem() && (Left->m_Type >= ITEMGET(2,0) && Left->m_Type < ITEMGET(3,0))))
								{
									if ( (rand()%100) < lpObj->MasterCharacterInfo->MaceStun )
										gObjUseSkill.SkillStun(lpObj->m_Index,lpTargetObj->m_Index);
								}
							}
						}
					}
				}
			}
		}
	}

	if(lpObj->IsBot == 1)
	{
		if(lpObj->m_SkillNumber == lpObj->m_AttackType)
		{
			int manareq = MagicDamageC.SkillGetMana(lpObj->m_SkillNumber);
			if(lpObj->Mana - manareq >= 0)
			{
				lpObj->Mana -= manareq;
			}else
			{
				ChatSend(lpObj, "I need more mana!");
				lpObj->m_AttackType = 0;
				lpObj->m_AttackRange = 1;
				if ( lpObj->Class == CLASS_ELF ) // Elf
				{
					CItem * Right = &lpObj->pInventory[0];
					CItem * Left = &lpObj->pInventory[1];

					if ( (Right->m_Type >= ITEMGET(4,8) && Right->m_Type < ITEMGET(4,15) ) ||
						 (Left->m_Type >= ITEMGET(4,0) && Left->m_Type < ITEMGET(4,7)) ||
						  Right->m_Type >= ITEMGET(4,16) )
					{
						lpObj->m_AttackRange = 5;
					}
				}
				return FALSE;
			}
		} else {
			int manareq = MagicDamageC.SkillGetMana(lpObj->m_SkillNumber);
			if(lpObj->Mana - manareq >= 0)
				BotChangeSkill = true;
		}

		if ( AttackDamage == 0 )
		{
			if ( !botPet.BotDecreaseArrow(lpObj) )
			{
				return FALSE;
			}
		}
	}
#endif

	if ( lpTargetObj->Type == OBJ_USER )
	{
		if (gObjIsConnected(lpTargetObj) )
			if ( (lpTargetObj->Authority&2) == 2 )
				return FALSE;
	}

	if ( lpObj->MapNumber != lpTargetObj->MapNumber )
		return FALSE;

	/*#if (GS_CASTLE==1)
	if( g_Crywolf.GetCrywolfState() == CRYWOLF_STATE_READY || g_Crywolf.GetCrywolfState() == CRYWOLF_STATE_END )
	{
		if(CRYWOLF_MAP_RANGE(lpTargetObj->MapNumber))
		{
			if(lpTargetObj->Type == OBJ_MONSTER)
				return FALSE;
		}
	}
	#endif*/

	if ( g_iUseCharacterAutoRecuperationSystem && lpObj->Level <= g_iCharacterRecuperationMaxLevel )
	{
		if ( lpObj->Type == OBJ_USER )
		{
			if ( lpObj->Level <= g_iCharacterRecuperationMaxLevel ) // #warning useless code
			{
				lpObj->m_iAutoRecuperationTime = GetTickCount();
			}
		}

		if ( lpTargetObj->Type == OBJ_USER )
		{
			if ( lpTargetObj->Level <= g_iCharacterRecuperationMaxLevel )
			{
				lpTargetObj->m_iAutoRecuperationTime = GetTickCount();
			}
		}
	}

	if ( lpObj->Type == OBJ_USER )
		lpObj->dwShieldAutoRefillTimer = GetTickCount();

	if ( lpTargetObj->Type == OBJ_USER )
		lpTargetObj->dwShieldAutoRefillTimer = GetTickCount();

	int skill = 0;
	
	if ( lpMagic )
		skill = lpMagic->m_Skill;
	
	if(lpTargetObj->m_SkillSleep == 1)
		gObjUseSkill.SkillSleepRemove(lpTargetObj);

	if ( lpObj->Class == 77 )
	{
		if ( lpObj->m_SkyBossMonSheild && skill == AT_SKILL_ENERGYBALL )
		{
			skill = AT_SKILL_THUNDER;
		}
	}

	skillSuccess = TRUE;

	if ( lpObj->GuildNumber > 0 )
	{
		if ( lpObj->lpGuild )
		{
			if ( lpObj->lpGuild->WarState )
			{
				if ( lpObj->lpGuild->WarType == 1 )
				{
					if ( !GetBattleSoccerGoalMove(0) )
					{
						return TRUE;
					}
				}
			}

			if ( lpObj->lpGuild->WarState )
			{
				if ( lpObj->lpGuild->WarType == 0 )
				{
					if ( lpTargetObj->Type == OBJ_MONSTER )
					{
						return TRUE;
					}
				}
			}
		}
	}

	if ( lpTargetObj->Type == OBJ_MONSTER )
	{
		if ( lpTargetObj->m_iMonsterBattleDelay > 0 )
			return TRUE;
		
		if ( lpTargetObj->Class == 200 )
		{
			if ( skill )
			{
				gObjMonsterStateProc(lpTargetObj, 7, lpObj->m_Index, 0);
			}
			else
			{
				gObjMonsterStateProc(lpTargetObj, 6, lpObj->m_Index, 0);
			}

			if ( magicsend )
			{
				GCMagicAttackNumberSend(lpObj, skill, lpTargetObj->m_Index, skillSuccess);
			}

			return TRUE;
		}

		if ( lpTargetObj->m_ImmuneToMagicCount > 0 )
		{
			BOOL bCheckAttackIsMagicType = gObjCheckAttackTypeMagic(lpObj->Class, skill);

			if ( bCheckAttackIsMagicType == TRUE )
			{
				GCMagicAttackNumberSend(lpObj, skill, lpTargetObj->m_Index, 0);
				return TRUE;
			}
		}

		if ( lpTargetObj->m_ImmuneToHarmCount > 0 )
		{
			BOOL bCheckAttackIsMagicType = gObjCheckAttackTypeMagic(lpObj->Class, skill);

			if ( bCheckAttackIsMagicType == FALSE )
			{
				GCDamageSend(lpObj->m_Index, lpTargetObj->m_Index, 0, 0, MSG_DAMAGE_MISS, 0);
				return TRUE;
			}
		}
		if ( lpTargetObj->m_MonsterSkillElementInfo.m_iSkillElementImmuneTime > 0 )
		{
			if ( lpTargetObj->m_MonsterSkillElementInfo.m_iSkillElementImmuneNumber == skill )
			{
				return TRUE;
			}
		}
	}

	if ( lpObj->Type == OBJ_USER )
	{
		if ( !gObjIsConnected(lpObj) )
		{
			return FALSE;
		}

		if ( lpObj->m_Change == 8 )
		{
			skill = AT_SKILL_POISON;
			lpMagic = &DefMagicInf[1];
			magicsend = 1;
		}

		gDarkSpirit[lpObj->m_Index].SetTarget(lpTargetObj->m_Index);
	}

	if ( lpTargetObj->Type == OBJ_USER )
	{
		if ( !gObjIsConnected(lpTargetObj))
		{
			return FALSE;
		}
	}

	if ( lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_MONSTER )	// PLAYER VS MONSTER
	{
		if ( lpObj->m_RecallMon >= 0 )
		{
			if ( lpObj->m_RecallMon == lpTargetObj->m_Index )
			{
				return FALSE;
			}
		}
	}
	
//#if (GS_CASTLE==0)
	if(lpObj->Type == OBJ_USER )
	{
		if(skill == 0 && lpObj->m_AttackType == 0)
		{
			int dis = gObjCalDistance(lpObj, lpTargetObj);
			if( dis > 8 ) 
			{
				if(dis != 1000 && lpObj->Class == CLASS_ELF && dis <= 12)
				{
				}else
				{
					if(ReadConfig.AHLog == TRUE)
					{
						ANTI_HACK_LOG.Output("[ATTACK DISTANCE CHECK] [%s][%s](%d) Invalid distance attacker(%d, %d, %d), defender(%d, %d, %d), distance:%d",lpObj->AccountID, lpObj->Name, lpObj->Class,lpObj->MapNumber, lpObj->X, lpObj->Y,lpTargetObj->MapNumber, lpTargetObj->X, lpTargetObj->Y,dis);
					}

					GCServerMsgStringSend("[Anti-Hack] Invalid Attack distance.",lpObj->m_Index, 0x01);
					return 0;
				}
			}
		}
	}
//#endif

	if ( gObjAttackType(lpTargetObj,lpObj) == 0)
		return FALSE;
	if ( this->PkCheck(lpObj, lpTargetObj) == FALSE )
		return FALSE;
	if ( this->CheckAttackArea(lpObj, lpTargetObj) == FALSE )
		return FALSE;

	if ( lpObj->m_RecallMon >= 0 )
		gObjCallMonsterSetEnemy(lpObj, lpTargetObj->m_Index);

	//lpObj->m_TotalAttackCount++;

	if ( AttackDamage == 0 )
	{
		if ( skill != AT_SKILL_FENRIR_ATTACK )
		{
			if ( !lpObj->m_iMuseElfInfinityArrowSkillTime )
			{
				if ( !this->DecreaseArrow(lpObj) )
				{
					return FALSE;
				}
			}
		}
	}

	/*if(lpTargetObj->Class == 277 && g_CastleSiegeSync.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE)		//Castle Gate
	{
		//Do nothing
	}
	else if(lpTargetObj->Class == 283 && g_CastleSiegeSync.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE)	//Guardian Statue
	{
		//Do nothing
	}
	else
	{
		if ( this->CheckAttackArea(lpObj, lpTargetObj) == FALSE )
			return FALSE;
	}*/

	lpCallObj = lpObj;

	if ( lpObj->Type == OBJ_MONSTER )
	{
		if ( lpObj->m_RecallMon >= 0 )
		{
			lpCallObj = &gObj[lpObj->m_RecallMon];
		}
	}

	lpCallTargetObj = lpTargetObj;

	if ( lpTargetObj->Type == OBJ_MONSTER )
	{
		if ( lpTargetObj->m_RecallMon >= 0 )
		{
			lpCallTargetObj = &gObj[lpTargetObj->m_RecallMon];
		}
	}

	int Strength = lpObj->Strength + lpObj->AddStrength;
	int Dexterity = lpObj->Dexterity + lpObj->AddDexterity;
	int Vitality = lpObj->Vitality + lpObj->AddVitality;
	int Energy = lpObj->Energy + lpObj->AddEnergy;
	BOOL bIsOnDuel = g_DuelManager.gObjDuelCheck(lpObj, lpTargetObj);

	if ( bIsOnDuel )
	{
		lpObj->m_iDuelTickCount = GetTickCount();
		lpTargetObj->m_iDuelTickCount = GetTickCount();
	}

	if ( lpObj->pInventory[0].m_Type == ITEMGET(2,5) && lpObj->pInventory[0].m_IsValidItem != false )	// Crystal Sword
	{
		if ( (rand()%20) == 0 )
		{
			skill = AT_SKILL_SLOW;
			lpMagic = &DefMagicInf[AT_SKILL_SLOW];
			magicsend = 1;
		}
	}
	else if ( lpObj->pInventory[1].m_Type == ITEMGET(2,5) && lpObj->pInventory[1].m_IsValidItem != false )	// Crystal Sword
	{
		if ( (rand()%20) == 0 )
		{
			skill = AT_SKILL_SLOW;
			lpMagic = &DefMagicInf[AT_SKILL_SLOW];
			magicsend = 1;
		}
	}

	MSBFlag = 0;
	MsgDamage = MSG_DAMAGE_MISS;
	skillSuccess = this->ResistanceCheck(lpObj, lpTargetObj, skill);
	BOOL skillIceArrowSuccess = skillSuccess;

	if ( skill == AT_SKILL_ICEARROW )
		skillSuccess = 0;

	BOOL bAllMiss = FALSE;
	
	if ( lpObj->m_iSkillInvisibleTime > 0 )
		gObjUseSkill.RemoveCloakingEffect(lpObj->m_Index);

	BOOL bDamageReflect = FALSE;

	if ( AttackDamage == 0 )
	{
		if ( g_ShieldSystemOn == TRUE )
		{
			if ( lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER )
			{
				if ( !this->MissCheckPvP(lpObj, lpTargetObj, skill, skillSuccess, magicsend, bAllMiss) )
				{
					return FALSE;
				}
			}
			else if ( !this->MissCheck(lpObj, lpTargetObj, skill, skillSuccess, magicsend, bAllMiss) )
			{
				return FALSE;
			}

		}
		else if ( !this->MissCheck(lpObj, lpTargetObj, skill, skillSuccess, magicsend, bAllMiss) )
		{
			return FALSE;
		}

		if ( skill == AT_SKILL_ICEARROW && skillIceArrowSuccess == TRUE )
		{
			lpTargetObj->m_SkillHarden = 7;
			lpTargetObj->m_SkillHardenTime = 7;
			lpTargetObj->lpAttackObj = lpObj;
			lpTargetObj->m_ViewSkillState |= 0x20;
			lpTargetObj->PathCount = 0;
			lpTargetObj->PathStartEnd = 0;
			skillSuccess = TRUE;

			gObjSetPosition(lpTargetObj->m_Index, lpTargetObj->X, lpTargetObj->Y);
		}

		int targetdefense = this->GetTargetDefense(lpObj, lpTargetObj, MsgDamage);

		if ( lpTargetObj->m_iSkillNPCDefense )
		{
			targetdefense += lpTargetObj->m_iSkillNPCDefense;
		}

		if ( skill == AT_SKILL_FALLINGSLASH
			|| skill == AT_SKILL_LUNGE
			|| skill == AT_SKILL_UPPERCUT
			|| skill == AT_SKILL_CYCLONE
			|| skill == AT_SKILL_SLASH
			|| skill == AT_SKILL_POWERSLASH
			|| skill == AT_SKILL_TWISTINGSLASH
			|| skill == AT_SKILL_BKIMPALE
			|| skill == AT_SKILL_BLOWOFFURY
			|| skill == AT_SKILL_KNIGHTDINORANT
			|| skill == AT_SKILL_DEATHSTAB
			|| (skill == AT_SKILL_FIREBURST && ReadConfig.IsSkillFireBurstStr == 1)
			|| skill == AT_SKILL_FIRESLASH
			|| skill == AT_SKILL_CRESCENTSLASH
			|| skill == AT_SKILL_SPIRALSLASH
			|| skill == AT_SKILL_SPACE_SPLIT
			|| skill == AT_SKILL_EXPLOTION
			|| skill == AT_SKILL_CHARGE		//NEW RAGE FIGHTER SKILL
			|| (skill>= AT_SKILL_LARGERINGBLOWER && skill<= AT_SKILL_DRAGONSLAYER)	//NEW RAGE FIGHTER SKILLS
			|| (skill>= 500 && skill<=504 && ReadConfig.S6E2 == 0)//NEW FIRESLASH
			|| (skill>= 505 && skill<=509 && ReadConfig.S6E2 == 0)//NEW POWERSLASH
			|| (skill>= 455 && skill<=459 && ReadConfig.S6E2 == 0)//TWISTING NEW
			|| (skill>= 495 && skill<=499 && ReadConfig.S6E2 == 0)//TWISTING NEW
			|| (skill>= 460 && skill<=464 && ReadConfig.S6E2 == 0)//Death Stab NEW
			|| (skill>= 465 && skill<=469 && ReadConfig.S6E2 == 0)//Rageful Blow NEW)
			)
		{
			AttackDamage = this->GetAttackDamage(lpObj, targetdefense, MsgDamage, bIsOnDuel, lpMagic);
			AttackDamage += lpObj->m_iSkillNPCAttack;
			
			//if((skill>= 455 && skill<=459)//TWISTING NEW
			//|| (skill>= 495 && skill<=499))//TWISTING NEW
			if(skill>=435 && skill<=554)
			{
				float Mult = (((float)(lpMagic->m_Skill%5) + 1.0f) / 10.0f)+1.0f;
				AttackDamage = (int)AttackDamage * Mult;
			}

			if ( AttackDamage > 0 )
			{
				gObjWeaponDurDown(lpObj, lpTargetObj, 0);
			}
		}
		else if ( skill == AT_SKILL_FENRIR_ATTACK )	// pluzzmanton
		{
			int iFenrirAttackDmg = 0;

			if ( lpObj->Class == CLASS_KNIGHT )
			{
				iFenrirAttackDmg = lpObj->Strength /(float) ReadConfig.FernirDKStrDiv + lpObj->Dexterity /(float) ReadConfig.FernirDKAgiDiv + lpObj->Vitality /(float) ReadConfig.FernirDKVitDiv + lpObj->Energy /(float) ReadConfig.FernirDKEneDiv;	// #formula
			}
			else if ( lpObj->Class == CLASS_MAGICGLADIATOR )
			{
				iFenrirAttackDmg = lpObj->Strength /(float) ReadConfig.FernirMGStrDiv + lpObj->Dexterity /(float) ReadConfig.FernirMGAgiDiv + lpObj->Vitality /(float) ReadConfig.FernirMGVitDiv + lpObj->Energy /(float) ReadConfig.FernirMGEneDiv;	// #formula
			}
			else if ( lpObj->Class == CLASS_RAGEFIGHTER )
			{
				iFenrirAttackDmg = lpObj->Strength /(float) ReadConfig.FernirRFStrDiv + lpObj->Dexterity /(float) ReadConfig.FernirRFAgiDiv + lpObj->Vitality /(float) ReadConfig.FernirRFVitDiv + lpObj->Energy /(float) ReadConfig.FernirRFEneDiv;	// #formula
			}
			else if ( lpObj->Class == CLASS_WIZARD )
			{
				iFenrirAttackDmg = lpObj->Strength /(float) ReadConfig.FernirDWStrDiv + lpObj->Dexterity /(float) ReadConfig.FernirDWAgiDiv + lpObj->Vitality /(float) ReadConfig.FernirDWVitDiv + lpObj->Energy /(float) ReadConfig.FernirDWEneDiv ;	// #formula
			}
			else if ( lpObj->Class == CLASS_SUMMONER)
			{
				iFenrirAttackDmg = lpObj->Strength /(float) ReadConfig.FernirSUStrDiv + lpObj->Dexterity /(float) ReadConfig.FernirSUAgiDiv + lpObj->Vitality /(float) ReadConfig.FernirSUVitDiv + lpObj->Energy /(float) ReadConfig.FernirSUEneDiv;	// #formula
			}
			else if ( lpObj->Class == CLASS_ELF )
			{
				iFenrirAttackDmg = lpObj->Strength /(float) ReadConfig.FernirElfStrDiv + lpObj->Dexterity /(float) ReadConfig.FernirElfAgiDiv + lpObj->Vitality /(float) ReadConfig.FernirElfVitDiv + lpObj->Energy /(float) ReadConfig.FernirElfEneDiv;	// #formula
			}
			else if ( lpObj->Class == CLASS_DARKLORD )
			{
				iFenrirAttackDmg = lpObj->Strength /(float) ReadConfig.FernirDLStrDiv + lpObj->Dexterity /(float) ReadConfig.FernirDLAgiDiv + lpObj->Vitality /(float) ReadConfig.FernirDLVitDiv + lpObj->Energy /(float) ReadConfig.FernirDLEneDiv + lpObj->Leadership /(float) ReadConfig.FernirDLCmdDiv;	// #formula
			}
			else
			{
				iFenrirAttackDmg = lpObj->Strength / 5.0f + lpObj->Dexterity / 5.0f + lpObj->Vitality / 7.0f + lpObj->Energy / 3.0f;	// #formula
			}

			if ( iFenrirAttackDmg < 0 )
				iFenrirAttackDmg = 0;

			if ( lpObj->m_CriticalDamage > 0 )
			{
				if ( (rand()%100) < lpObj->m_CriticalDamage )
				{
					MsgDamage = MSG_DAMAGE_CRITICAL;
				}
			}

			if ( lpObj->m_ExcelentDamage > 0 )
			{
				if ( (rand()%100) < lpObj->m_ExcelentDamage )
				{
					MsgDamage = MSG_DAMAGE_EXCELLENT;
				}
			}

			unsigned char MsgDmgTmp = MsgDamage;
			if ( lpObj->Class == CLASS_KNIGHT )
			{
				if(lpObj->MasterCharacterInfo->SpearDoubleDmg > 0)
				{
					CItem * Right = &lpObj->pInventory[0];
					if ( Right->IsItem() == TRUE )
					{
						if(Right->m_Type >= ITEMGET(4,0) && Right->m_Type < ITEMGET(5,0))
						{
							if ( (rand()%100) < lpObj->MasterCharacterInfo->SpearDoubleDmg )
							{
								MsgDamage |= MSG_DAMAGE_DOUBLE;
							}
						}
					}
				}
			}
			if ( lpObj->Class == CLASS_RAGEFIGHTER )
			{
				if(lpObj->MasterCharacterInfo->SpearDoubleDmg > 0)
				{
					CItem * Right = &lpObj->pInventory[0];
					CItem * Left  = &lpObj->pInventory[1];
					bool enter = false;
					if ( Right->IsItem() == TRUE )
					{
						if(Right->m_Type >= ITEMGET(0,32) && Right->m_Type <= ITEMGET(0,35))
						{
							if ( (rand()%100) < lpObj->MasterCharacterInfo->SpearDoubleDmg )
							{
								MsgDamage |= MSG_DAMAGE_DOUBLE;
								enter = true;
							}
						}
					}
					if(enter == false)
					{
						if ( Left->IsItem() == TRUE )
						{
							if(Left->m_Type >= ITEMGET(0,32) && Left->m_Type <= ITEMGET(0,35))
							{
								if ( (rand()%100) < lpObj->MasterCharacterInfo->SpearDoubleDmg )
								{
									MsgDamage |= MSG_DAMAGE_DOUBLE;
								}
							}
						}
					}
				}
			}

			if ( MsgDamage%MSG_DAMAGE_DOUBLE == MSG_DAMAGE_CRITICAL )	// Critical Damage
			{
				AttackDamage =  iFenrirAttackDmg + lpMagic->m_DamageMax;
				AttackDamage += lpObj->SetOpAddCriticalDamage;
				AttackDamage += lpObj->SkillAddCriticalDamage;
				AttackDamage += lpObj->m_JewelOfHarmonyEffect.HJOpAddCriticalDamage;
			}
			else if ( MsgDamage%MSG_DAMAGE_DOUBLE == MSG_DAMAGE_EXCELLENT )	// Excellent Damage
			{
				AttackDamage = iFenrirAttackDmg + lpMagic->m_DamageMax;
				AttackDamage += AttackDamage * 20 / 100;
				AttackDamage += lpObj->SetOpAddExDamage;
			}
			else
			{
				AttackDamage = (iFenrirAttackDmg + lpMagic->m_DamageMin) + (rand()%(lpMagic->m_DamageMax - lpMagic->m_DamageMin + 1));
			}

			//Fix Double Damage :)
			if (( (MsgDamage > MSG_DAMAGE_BEAT) && (MsgDamage&MSG_DAMAGE_DOUBLE == MSG_DAMAGE_DOUBLE )) || (MsgDamage == MSG_DAMAGE_DOUBLE))
			{
				AttackDamage = AttackDamage * 2;
			}

			AttackDamage -= targetdefense;
		}
		else
		{
			if ( (	lpObj->Class == CLASS_WIZARD || 
					lpObj->Class == CLASS_MAGICGLADIATOR || 
					lpObj->Class == CLASS_RAGEFIGHTER || 
					lpObj->Class == CLASS_SUMMONER ) && skill )
			{
				AttackDamage = this->GetAttackDamageWizard(lpObj, targetdefense, lpMagic, MsgDamage, bIsOnDuel);
				AttackDamage += lpObj->m_iSkillNPCAttack;

				if ( AttackDamage > 0 )
				{
					gObjWeaponDurDown(lpObj, lpTargetObj, 1);
				}
			}
			else
			{
				AttackDamage = this->GetAttackDamage(lpObj, targetdefense, MsgDamage, bIsOnDuel, lpMagic);
				AttackDamage += lpObj->m_iSkillNPCAttack;
				if(skill>=490 && skill<=494)//New Triple Skill
				{
					float Mult = (((float)(lpMagic->m_Skill%5) + 1.0f) / 10.0f)+1.0f;
					AttackDamage = (int)AttackDamage * Mult;
				}

				if ( AttackDamage > 0 )
				{
					gObjWeaponDurDown(lpObj, lpTargetObj, 0);
				}
			}
		}

		if(ReadConfig.S6E2 == 1)
		{
			if(lpObj->Type == OBJ_USER )
			{
				switch(skill)
				{
					case AT_SKILL_POISON:
					{
						if(lpTargetObj->Type == OBJ_USER )
						{
							if ( lpTargetObj->m_PoisonType == 1 )
							{
								if(lpObj->MasterCharacterInfo->IncPoison)
								{
									lpTargetObj->m_PoisonAdd = lpObj->MasterCharacterInfo->IncPoison;
								}
							}
						}
					}break;
					case AT_SKILL_THUNDER:
					{
						AttackDamage += lpObj->MasterCharacterInfo->IncLighting;
					}break;
					case AT_SKILL_FLAME:
					{
						AttackDamage += lpObj->MasterCharacterInfo->IncFlame;
					}break;
					case AT_SKILL_SLOW:
					{
						AttackDamage += lpObj->MasterCharacterInfo->IncIce;
					}break;
					case AT_SKILL_EVIL:
					{
						AttackDamage += lpObj->MasterCharacterInfo->IncEvilSpirits;
					}break;
					case AT_SKILL_HELL:
					{
						AttackDamage += lpObj->MasterCharacterInfo->IncHellFire;
					}break;
					case AT_SKILL_POWERWAVE:
					{
						AttackDamage += lpObj->MasterCharacterInfo->IncWindBook1;
						if (lpObj->MasterCharacterInfo->IncWindBook2 > 0)
						{							
							if(OBJMAX_RANGE(lpTargetObj->m_Index) == true)
							{
								if ( lpTargetObj->Connected >= PLAYER_CONNECTED )
								{
									if(lpTargetObj->m_iSkillStunTime <= 0)
									{
										if ( (rand()%100) < lpObj->MasterCharacterInfo->IncWindBook2 )
										{
											gObjUseSkill.SkillStun(lpObj->m_Index,lpTargetObj->m_Index);
										}
									}
								}
							}
						}
					}break;
					case AT_SKILL_BLAST:
					{
						AttackDamage += lpObj->MasterCharacterInfo->IncCometFall;
					}break;
					case AT_SKILL_INFERNO:
					{
						AttackDamage += lpObj->MasterCharacterInfo->IncInferno;
					}break;
					case AT_SKILL_FALLINGSLASH:
					{
						AttackDamage += lpObj->MasterCharacterInfo->IncFallingSlash;
					}break;
					case AT_SKILL_LUNGE:
					{
						AttackDamage += lpObj->MasterCharacterInfo->IncLunge;
					}break;
					case AT_SKILL_CYCLONE:
					{
						AttackDamage += lpObj->MasterCharacterInfo->IncCyclone;
					}break;
					case AT_SKILL_SLASH:
					{
						AttackDamage += lpObj->MasterCharacterInfo->IncSlash;
					}break;
					case AT_SKILL_CROSSBOW:
					{
						AttackDamage += lpObj->MasterCharacterInfo->IncTripleShot;	
						if(lpObj->MasterCharacterInfo->IncTripleShotOneArrow > 0)
							AttackDamage += AttackDamage/3;
					}break;
					case AT_SKILL_EXPPOISON:
					{
						AttackDamage += lpObj->MasterCharacterInfo->IncDecay;
					}break;
					case AT_SKILL_TWISTINGSLASH:
					{
						AttackDamage += lpObj->MasterCharacterInfo->IncTwistingSlash;
						
						if(lpObj->MasterCharacterInfo->PullTwistingSlash > 0)
						{
							if(OBJMAX_RANGE(lpTargetObj->m_Index) == true)
							{
								if ( lpTargetObj->Connected >= PLAYER_CONNECTED )
								{									
									if ( (rand()%100) < lpObj->MasterCharacterInfo->PullTwistingSlash )
										gObjBackSpring2(lpTargetObj, lpObj, 2);
								}
							}
						}
					}break;
					case AT_SKILL_BLOWOFFURY:
					{
						AttackDamage += lpObj->MasterCharacterInfo->IncRagefulBlow;
					}break;
					case AT_SKILL_DEATHSTAB:
					{
						AttackDamage += lpObj->MasterCharacterInfo->IncDeathStab;
					}break;
					case AT_SKILL_PENETRATION:			//45: ERROR
					{
						AttackDamage += lpObj->MasterCharacterInfo->IncPenetration;
					}
					case AT_SKILL_ICEARROW:
					{
						AttackDamage += lpObj->MasterCharacterInfo->IncIceArrow;
					}break;
					case AT_SKILL_FIRESLASH:
					{
						AttackDamage += lpObj->MasterCharacterInfo->IncFireSlash;
					}break;
					case AT_SKILL_POWERSLASH:
					{
						AttackDamage += lpObj->MasterCharacterInfo->IncPowerSlash;
					}break;
					case AT_SKILL_FIREBURST:
					{
						AttackDamage += lpObj->MasterCharacterInfo->IncFireBurst1;
						if (lpObj->MasterCharacterInfo->IncFireBurst2 > 0)
						{							
							if(OBJMAX_RANGE(lpTargetObj->m_Index) == true)
							{
								if ( lpTargetObj->Connected >= PLAYER_CONNECTED )
								{
									if(lpTargetObj->m_iSkillStunTime <= 0)
									{
										if ( (rand()%100) < lpObj->MasterCharacterInfo->IncFireBurst2 )
										{
											gObjUseSkill.SkillStun(lpObj->m_Index,lpTargetObj->m_Index);
										}
									}
								}
							}
						}
					}break;
					case AT_SKILL_DARKHORSE_ATTACK:
					{
						AttackDamage += lpObj->MasterCharacterInfo->IncEarthShake1;
						if (lpObj->MasterCharacterInfo->IncEarthShake2 > 0)
						{							
							if(OBJMAX_RANGE(lpTargetObj->m_Index) == true)
							{
								if ( lpTargetObj->Connected >= PLAYER_CONNECTED )
								{
									if(lpTargetObj->m_iSkillStunTime <= 0)
									{
										if ( (rand()%100) < lpObj->MasterCharacterInfo->IncEarthShake2 )
										{
											gObjUseSkill.SkillStun(lpObj->m_Index,lpTargetObj->m_Index);
										}
									}
								}
							}
						}
					}break;
					case AT_SKILL_LONGSPEAR:
					{
						AttackDamage += lpObj->MasterCharacterInfo->IncForceWave;
					}break;
					case AT_SKILL_FIRESCREAM:
					{
						AttackDamage += lpObj->MasterCharacterInfo->IncFireScream;
					}break;
					case AT_SKILL_DRAIN_LIFE:
					{
						AttackDamage += lpObj->MasterCharacterInfo->IncDrainLife;
					}break;
					case AT_SKILL_CHAIN_LIGHTING:
					case AT_SKILL_ELECTRIC_SURGE:
					{
						AttackDamage += lpObj->MasterCharacterInfo->IncLightingBook1;
						if(lpObj->MasterCharacterInfo->IncLightingBook2 > 0)
						{
							if(OBJMAX_RANGE(lpTargetObj->m_Index) == true)
							{
								if ( lpTargetObj->Connected >= PLAYER_CONNECTED )
								{									
									if ( (rand()%100) < lpObj->MasterCharacterInfo->IncLightingBook2 )
										gObjBackSpring2(lpTargetObj, lpObj, 2);
								}
							}
						}
						if(skill == AT_SKILL_CHAIN_LIGHTING)
						{
							AttackDamage += lpObj->MasterCharacterInfo->IncChainLighting;
						}
					}break;
					case AT_SKILL_SLEEP:
					{
						AttackDamage += (AttackDamage * lpObj->MasterCharacterInfo->IncSleep) / 100.0f;
					}break;
					case AT_SKILL_SAHAMUTT:
					{
						AttackDamage += lpObj->MasterCharacterInfo->IncFireBook1;
					}break;
					case AT_SKILL_RED_STORM:
					{
						AttackDamage += lpObj->MasterCharacterInfo->IncLightingShock;
					}break;
					case AT_SKILL_EXPLOTION:
					{
						AttackDamage += lpObj->MasterCharacterInfo->IncStrikeofDestruction;
					}break;
					
					case AT_SKILL_CHAINDRIVE:
					{
						AttackDamage += lpObj->MasterCharacterInfo->IncChainDrive;
					}break;
					case AT_SKILL_DARKSIDE:
					{
						AttackDamage += lpObj->MasterCharacterInfo->IncDarkSide;
					}break;
					case AT_SKILL_DRAGONLORE:
					{
						AttackDamage += lpObj->MasterCharacterInfo->IncDragonLore;
					}break;
					case AT_SKILL_LARGERINGBLOWER:
					{
						AttackDamage += lpObj->MasterCharacterInfo->IncLargerRingBlower;
					}break;
					case AT_SKILL_UPPERBEAST:
					{
						AttackDamage += lpObj->MasterCharacterInfo->IncUpperBeast;
					}break;
				};

				if(lpObj->Class == CLASS_ELF)
				{
					/*LogAddTD("[Mastering2][AttackDamage][%s][%s] %d", 
						lpObj->AccountID, lpObj->Name,
						AttackDamage
					);*/

					if(lpObj->m_iMuseElfInfinityArrowSkillTime > 0)
					{
						AttackDamage += (float)(AttackDamage * lpObj->MasterCharacterInfo->IncInfiniteArrow)/100.0f;
						/*LogAddTD("[Mastering2][AttackDamage2][%s][%s] %d", 
							lpObj->AccountID, lpObj->Name,
							AttackDamage
						);*/
					}
				}
			}
		}

		if ( bAllMiss )
		{
			AttackDamage = ( AttackDamage * 30 ) / 100;
		}

		if ( lpTargetObj->DamageMinus )
		{
			//int beforeDamage = AttackDamage;
			AttackDamage -= ( ( AttackDamage * (int)lpTargetObj->DamageMinus) / 100 );
		}

		int tlevel = lpObj->Level / 10;

		if ( AttackDamage < tlevel )
		{
			if ( tlevel < 1 )
			{
				tlevel = 1;
			}

			AttackDamage = tlevel;
		}

		if ( lpTargetObj->m_SkillNumber == AT_SKILL_BLOCKING )
		{
			if ( AttackDamage > 1 )
			{
				AttackDamage >>= 1;
			}
		}

		gObjSpriteDamage(lpTargetObj, AttackDamage);

		if ( gObjSatanSprite(lpObj) == TRUE )
		{
			lpObj->Life -= 3.0f;

			if ( lpObj->Life < 0.0f )
			{
				lpObj->Life = 0.0f;
			}
			else
			{
				AttackDamage = AttackDamage * 13 / 10;
			}

			GCReFillSend(lpObj->m_Index, lpObj->Life, 0xFF, 0, lpObj->iShield);
		}

		if ( gObjAngelSprite(lpTargetObj) == TRUE )
		{
			if ( AttackDamage > 1 )
			{
				float  damage = (AttackDamage * 8) / 10.0f;
				AttackDamage = damage;
			}
		}

		if ( lpObj->Type == OBJ_MONSTER )
		{
			if ( gObjSpiritOfGuardianSprite(lpTargetObj) == TRUE )
			{
				if ( AttackDamage > 1 )
				{
					float  damage = (AttackDamage * 8) / 20.0f;
					AttackDamage = damage;
				}
			}
		}

		if ( gObjWingSprite(lpObj) == TRUE )
		{
			CItem * Wing = &lpObj->pInventory[7];

			if ( lpObj->Class == CLASS_WIZARD || lpObj->Class == CLASS_SUMMONER || lpObj->Class == CLASS_ELF )
			{
				lpObj->Life -= 1.0f;
			}
			else
			{
				lpObj->Life -= 3.0f;
			}

			if ( lpObj->Life < 0.0f )
			{
				lpObj->Life = 0.0f;
			}
			else
			{
				if ( Wing->m_Type == ITEMGET(13,30) || Wing->m_Type == ITEMGET(12,49))	// Cape Of Lord
				{
					AttackDamage = AttackDamage * (Wing->m_Level * 2 + 120) / 100;	// #formula
				}
				else if (( Wing->m_Type > ITEMGET(12,2) && Wing->m_Type < ITEMGET(12,7)) || ( Wing->m_Type == ITEMGET(12,42)))
				{
					AttackDamage = AttackDamage * (Wing->m_Level + 132) / 100 ;	// #formula
				}
				else if (( Wing->m_Type > ITEMGET(12,35) && Wing->m_Type < ITEMGET(12,41)) || ( Wing->m_Type == ITEMGET(12,43)) || ( Wing->m_Type == ITEMGET(12,50))) //Third Wings
				{
					AttackDamage = AttackDamage * (Wing->m_Level * 2 + 139) / 100 ;	// #formula
				}
#if (CRYSTAL_EDITION == 1)
				else if ((Wing->m_Type >= ITEMGET(12,200) && Wing->m_Type <= ITEMGET(12,254)))
				{
					AttackDamage = CWings.GetIncreaseDmg(Wing->m_Type,Wing->m_Level,AttackDamage);
				}
#endif
				else if ( Wing->m_Type == ITEMGET(12,130) )	// Small Cape Of Lord
				{
					AttackDamage = AttackDamage * (Wing->m_Level * 2 + 120) / 100;	// #formula
				}
				else if ( Wing->m_Type == ITEMGET(12,135) )	// Small Cape Of rage
				{
					AttackDamage = AttackDamage * (Wing->m_Level * 2 + 120) / 100;	// #formula
				}
				else if ( Wing->m_Type >= ITEMGET(12,131) && Wing->m_Type <= ITEMGET(12,134))
				{
					AttackDamage = AttackDamage * (Wing->m_Level * 2 + 112) / 100 ;	// #formula
				}
				else
				{
					AttackDamage = AttackDamage * (Wing->m_Level * 2 + 112) / 100;	// #formula
				}
			}

			GCReFillSend(lpObj->m_Index, lpObj->Life, 0xFF, 0, lpObj->iShield);
		}

		if ( gObjWingSprite(lpTargetObj) == TRUE )
		{
			CItem * Wing = &lpTargetObj->pInventory[7];

			if ( AttackDamage > 1 )
			{
				if ( (Wing->m_Type > ITEMGET(12,2) && Wing->m_Type < ITEMGET(12,7)) || (Wing->m_Type == ITEMGET(12,42)) )
				{
					float damage = (float)(AttackDamage * (75 - (Wing->m_Level*2))) / 100.0f;
					AttackDamage = (int)(damage);	//  #formula
				}
				else if ( (Wing->m_Type >= ITEMGET(12,0) && Wing->m_Type <= ITEMGET(12,2)) || (Wing->m_Type == ITEMGET(12,41)) )
				{
					float damage = (float)(AttackDamage * (88 - (Wing->m_Level*2))) / 100.0f;
					AttackDamage = (int)(damage);	//  #formula
				}
				else if (Wing->m_Type == ITEMGET(12,130))
				{
					float damage = (float)(AttackDamage * (80 - (Wing->m_Level*2))) / 100.0f;
					AttackDamage = (int)(damage);	//  #formula
				}
				else if (Wing->m_Type == ITEMGET(12,135))
				{
					float damage = (float)(AttackDamage * (80 - (Wing->m_Level*2))) / 100.0f;
					AttackDamage = (int)(damage);	//  #formula
				}
				else if (Wing->m_Type >= ITEMGET(12,131) && Wing->m_Type <= ITEMGET(12,134))
				{
					float damage = (float)(AttackDamage * (88 - (Wing->m_Level*2))) / 100.0f;
					AttackDamage = (int)(damage);	//  #formula
				}
				else if ( (Wing->m_Type >= ITEMGET(12,36) && Wing->m_Type <= ITEMGET(12,39)) || (Wing->m_Type == ITEMGET(12,43)) )
				{
					float damage = (float)(AttackDamage * (61 - (Wing->m_Level*2))) / 100.0f;
					AttackDamage = (int)(damage);	//  #formula
				}
#if (CRYSTAL_EDITION == 1)
				else if ( (Wing->m_Type >= ITEMGET(12,200) && Wing->m_Type <= ITEMGET(12,254)) )
				{
					AttackDamage = (int)CWings.GetAbsorbDmg(Wing->m_Type,Wing->m_Level,AttackDamage);
				}
#endif
				else if ( (Wing->m_Type == ITEMGET(12,40)) || (Wing->m_Type == ITEMGET(12,50)) )
				{
					float damage = (float)(AttackDamage * (76 - (Wing->m_Level*2))) / 100.0f;
					AttackDamage = (int)(damage);	//  #formula
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

		if ( gObjDenorantSprite(lpObj ) )
		{
			lpObj->Life -= 1.0f;

			if ( lpObj->Life < 0.0f )
			{
				lpObj->Life = 0.0f;
			}
			else
			{
				AttackDamage = AttackDamage * 115 / 100;
			}

			GCReFillSend(lpObj->m_Index, lpObj->Life, 0xFF, 0, lpObj->iShield);
		}

		if ( gObjDenorantSprite(lpTargetObj ) )
		{
			CItem * Dinorant = &lpTargetObj->pInventory[8];
			int dinorantdecdamage = 90 - Dinorant->IsDinorantReduceAttackDamaege();
			lpObj->Life -= 1.0f;

			if ( lpObj->Life < 0.0f )
			{
				lpObj->Life = 0.0f;
			}
			else
			{
				AttackDamage = AttackDamage * dinorantdecdamage / 100;
			}

			GCReFillSend(lpObj->m_Index, lpObj->Life, 0xFF, 0, lpObj->iShield);
		}

		if ( gObjDarkHorse(lpTargetObj ) )
		{
			CItem * Darkhorse = &lpTargetObj->pInventory[8];
			int decdamage = 100 - ((Darkhorse->m_PetItem_Level + 30) / 2 );

			lpTargetObj->Life -= 1.0f;

			if ( lpTargetObj->Life < 0.0f )
			{
				lpTargetObj->Life = 0.0f;
			}
			else
			{
				AttackDamage = AttackDamage * decdamage / 100;
			}

			GCReFillSend(lpTargetObj->m_Index, lpTargetObj->Life, 0xFF, 0, lpTargetObj->iShield);
		}

		if ( lpTargetObj->Live )
		{
			switch ( skill )
			{

				case AT_SKILL_LARGERINGBLOWER:
				case AT_SKILL_UPPERBEAST:
				case AT_SKILL_CHAINDRIVE:
				case AT_SKILL_DARKSIDE:
				case AT_SKILL_DRAGONLORE:
				case AT_SKILL_DRAGONSLAYER:
				case AT_SKILL_PHOENIX_G:
					{
						if (skill == AT_SKILL_DRAGONSLAYER)
						{
							if(lpObj->m_SkillDragonSlayerTime > 0)
							{
								//m_SkillDragonSlayerDmg = 3*(50+Ene/10)/100
								//300+Dmg*m_SkillDragonSlayerDmg
								AttackDamage = ReadConfig.SkillDragonSlayerDmgBase + (AttackDamage * lpObj->m_SkillDragonSlayerDmg);
							}
						}
						else if (skill == AT_SKILL_DRAGONLORE)
						{
							//Dmg*(50+Ene/10)/100
							float multiFix = (ReadConfig.DragonLoreEnergyBase + Energy / (float)ReadConfig.DragonLoreEnergyDiv) / (float)ReadConfig.DragonLoreAttackMultDiv;

							if (multiFix > ReadConfig.DragonLoreAttackMultMax)
								multiFix = ReadConfig.DragonLoreAttackMultMax;

							AttackDamage *= multiFix;
						}
						else if (skill == AT_SKILL_DARKSIDE)
						{
							//(Agi/AgiDiv+Ene/EneDiv+Dmg)*(MultBase+Agi/AgiDiv+Vit/VitDiv)/MultDiv
							float multiFix = (ReadConfig.DarkSideParchmentMultBase + Dexterity/(float)ReadConfig.DarkSideParchmentAgiDiv + Vitality/(float)ReadConfig.DarkSideParchmentVitDiv)/(float)ReadConfig.DarkSideParchmentMultDiv;

							if (multiFix > ReadConfig.DarkSideParchmentMultMax)
								multiFix = ReadConfig.DarkSideParchmentMultMax;

							AttackDamage = AttackDamage + Dexterity/(float)ReadConfig.DarkSideParchmentAgiDiv + Energy/(float)ReadConfig.DarkSideParchmentEneDiv;
							AttackDamage *= multiFix;
						}
						else if (skill == AT_SKILL_CHAINDRIVE)
						{
							//(Vit/VitDiv+Dmg)*(VitBase+Vit/VitDiv)/MultDiv
							float multiFix = (ReadConfig.ChainDriveVitBase + Vitality/(float)ReadConfig.ChainDriveVitDiv)/(float)ReadConfig.ChainDriveMultDiv;

							if (multiFix > ReadConfig.ChainDriveMultMax)
								multiFix = ReadConfig.ChainDriveMultMax;

							AttackDamage = AttackDamage + Vitality/(float)ReadConfig.ChainDriveVitDiv;
							AttackDamage *= multiFix;
						}
						else if (skill == AT_SKILL_LARGERINGBLOWER ||
								skill == AT_SKILL_UPPERBEAST)
						{
							//Dmg*(VitBase+Vit/VitDiv)/MultDiv
							float multiFix = (ReadConfig.RFGlovesSkillAttackVitBase + Vitality/(float)ReadConfig.RFGlovesSkillAttackVitDiv)/(float)ReadConfig.RFGlovesSkillAttackMultDiv;

							if (multiFix > ReadConfig.RFGlovesSkillAttackMultMax)
								multiFix = ReadConfig.RFGlovesSkillAttackMultMax;

							AttackDamage *= multiFix;
						}
					}
					break;

				case AT_SKILL_FALLINGSLASH:
				case AT_SKILL_LUNGE:
				case AT_SKILL_UPPERCUT:
				case AT_SKILL_CYCLONE:
				case AT_SKILL_SLASH:
				case AT_SKILL_KNIGHTDINORANT:	//DK
				case AT_SKILL_BKIMPALE:
					//if ( lpObj->pInventory[8].m_Type == ITEMGET(13,3) ||
					//	 lpObj->pInventory[8].m_Type == ITEMGET(13,2) ||
					//	 lpObj->pInventory[8].m_Type == ITEMGET(13,37) )
					{
						if ( lpObj->Class == CLASS_DARKLORD )
						{
							float multiFix = ( Energy/(float)ReadConfig.DLOtherSkillEneDiv + ReadConfig.DLOtherSkillBase ) /(float) ReadConfig.DLOtherSkillMultDiv;

							if (multiFix > ReadConfig.DLOtherSkillMultMax)
								AttackDamage *= ReadConfig.DLOtherSkillMultMax;
							else if (multiFix < ReadConfig.DLOtherSkillMultMin)
								AttackDamage *= ReadConfig.DLOtherSkillMultMin;
							else
								AttackDamage *= multiFix;
						}
						else if (lpObj->Class == CLASS_MAGICGLADIATOR)
						{
							float multiFix = ( Energy/(float)ReadConfig.MGOtherSkillEneDiv + ReadConfig.MGOtherSkillBase ) /(float) ReadConfig.MGOtherSkillMultDiv;

							if (multiFix > ReadConfig.MGOtherSkillMultMax)
								AttackDamage *= ReadConfig.MGOtherSkillMultMax;
							else if (multiFix < ReadConfig.MGOtherSkillMultMin)
								AttackDamage *= ReadConfig.MGOtherSkillMultMin;
							else
								AttackDamage *= multiFix;
						}
						else if (lpObj->Class == CLASS_RAGEFIGHTER)
						{
							float multiFix = ( Energy/(float)ReadConfig.RFOtherSkillEneDiv + ReadConfig.RFOtherSkillBase ) /(float) ReadConfig.RFOtherSkillMultDiv;

							if (multiFix > ReadConfig.RFOtherSkillMultMax)
								AttackDamage *= ReadConfig.RFOtherSkillMultMax;
							else if (multiFix < ReadConfig.RFOtherSkillMultMin)
								AttackDamage *= ReadConfig.RFOtherSkillMultMin;
							else
								AttackDamage *= multiFix;
						}
						else if (lpObj->Class == CLASS_WIZARD  )
						{
							float multiFix = ( Energy/(float)ReadConfig.DWOtherSkillEneDiv + ReadConfig.DWOtherSkillBase ) /(float) ReadConfig.DWOtherSkillMultDiv;

							if (multiFix > ReadConfig.DWOtherSkillMultMax)
								AttackDamage *= ReadConfig.DWOtherSkillMultMax;
							else
								AttackDamage *= multiFix;
						}
						else if (lpObj->Class == CLASS_KNIGHT )
						{
							float multiFix = ( Energy/(float)ReadConfig.DKOtherSkillEneDiv + ReadConfig.DKOtherSkillBase ) /(float) ReadConfig.DKOtherSkillMultDiv;

							if (multiFix > ReadConfig.DKOtherSkillMultMax)
								AttackDamage *= ReadConfig.DKOtherSkillMultMax;
							else
								AttackDamage *= multiFix;
						}
						else if (lpObj->Class == CLASS_ELF  )
						{
							float multiFix = ( Energy/(float)ReadConfig.ElfOtherSkillEneDiv + ReadConfig.ElfOtherSkillBase ) /(float) ReadConfig.ElfOtherSkillMultDiv;

							if (multiFix > ReadConfig.ElfOtherSkillMultMax)
								AttackDamage *= ReadConfig.ElfOtherSkillMultMax;
							else
								AttackDamage *= multiFix;
						}
						else if (lpObj->Class == CLASS_SUMMONER  )
						{
							float multiFix = ( Energy/(float)ReadConfig.SUOtherSkillEneDiv + ReadConfig.SUOtherSkillBase ) /(float) ReadConfig.SUOtherSkillMultDiv;

							if (multiFix > ReadConfig.SUOtherSkillMultMax)
								AttackDamage *= ReadConfig.SUOtherSkillMultMax;
							else
								AttackDamage *= multiFix;
						}
						else
						{
							AttackDamage = ( AttackDamage * ( Energy / 10.0f + 200 )  ) / 100.0f;
						}
					}
					break;


				case AT_SKILL_CRESCENTSLASH:	//DK
					{
						float multiFix = ( Energy/(float)ReadConfig.CrescentSlashEneDiv + ReadConfig.CrescentSlashBase ) /(float) ReadConfig.CrescentSlashMultDiv;
						if (multiFix > ReadConfig.CrescentSlashMultMax)
							AttackDamage *= ReadConfig.CrescentSlashMultMax;
						else
							AttackDamage *= multiFix;
					}
					break;

				case AT_SKILL_EXPLOTION:		//DK
					{
						float multiFix = ( Energy/(float)ReadConfig.DKExplotionEneDiv + ReadConfig.DKExplotionBase ) /(float) ReadConfig.DKExplotionMultDiv;
						if (multiFix > ReadConfig.DKExplotionMultMax)
							AttackDamage *= ReadConfig.DKExplotionMultMax;
						else
							AttackDamage *= multiFix;
					}
					break;

				case AT_SKILL_DEATHSTAB:		//DK
				case 460:	//Increase Death Stab Skill
				case 461:	//Increase Death Stab Skill
				case 462:	//Increase Death Stab Skill
				case 463:	//Increase Death Stab Skill
				case 464:	//Increase Death Stab Skill
					{
						float multiFix = ( Energy/(float)ReadConfig.DeathStabEneDiv + ReadConfig.DeathStabBase ) /(float) ReadConfig.DeathStabMultDiv;
						if (multiFix > ReadConfig.DeathStabMultMax)
							AttackDamage *= ReadConfig.DeathStabMultMax;
						else
							AttackDamage *= multiFix;
					}
					break;

				case AT_SKILL_BLOWOFFURY:		//DK
				case 465:	//Increase Rageful Blow Skill
				case 466:	//Increase Rageful Blow Skill
				case 467:	//Increase Rageful Blow Skill
				case 468:	//Increase Rageful Blow Skill
				case 469:	//Increase Rageful Blow Skill
					{
						float multiFix = ( Energy/(float)ReadConfig.BlowOfFuryEneDiv + ReadConfig.BlowOfFuryBase ) /(float) ReadConfig.BlowOfFuryMultDiv;
						if (multiFix > ReadConfig.BlowOfFuryMultMax)
							AttackDamage *= ReadConfig.BlowOfFuryMultMax;
						else
							AttackDamage *= multiFix;
					}
					break;

				case AT_SKILL_TWISTINGSLASH:	//MG, DK
				case 455:	//Increase Twisting Slash Skill
				case 456:	//Increase Twisting Slash Skill
				case 457:	//Increase Twisting Slash Skill
				case 458:	//Increase Twisting Slash Skill
				case 459:	//Increase Twisting Slash Skill
				case 495:	//Increase Twisting Slash Skill
				case 496:	//Increase Twisting Slash Skill
				case 497:	//Increase Twisting Slash Skill
				case 498:	//Increase Twisting Slash Skill
				case 499:	//Increase Twisting Slash Skill
					{
						if (lpObj->Class == CLASS_MAGICGLADIATOR)
						{
							float multiFix = ( Energy/(float)ReadConfig.MGTwistingSlashEneDiv + ReadConfig.MGTwistingSlashBase ) /(float) ReadConfig.MGTwistingSlashMultDiv;
							if (multiFix > ReadConfig.MGTwistingSlashMultMax)
								AttackDamage *= ReadConfig.MGTwistingSlashMultMax;
							else if (multiFix < ReadConfig.MGTwistingSlashMultMin)
								AttackDamage *= ReadConfig.MGTwistingSlashMultMin;
							else
								AttackDamage *= multiFix;
						} else {
							float multiFix = ( Energy/(float)ReadConfig.DKTwistingSlashEneDiv + ReadConfig.DKTwistingSlashBase ) /(float) ReadConfig.DKTwistingSlashMultDiv;
							if (multiFix > ReadConfig.DKTwistingSlashMultMax)
								AttackDamage *= ReadConfig.DKTwistingSlashMultMax;
							else
								AttackDamage *= multiFix;
						}
					}
					break;

				case AT_SKILL_SPIRALSLASH:		//MG
					{
						float multiFix = ( Energy/(float)ReadConfig.SpiralSlashEneDiv + ReadConfig.SpiralSlashBase ) /(float) ReadConfig.SpiralSlashMultDiv;
						if (multiFix > ReadConfig.SpiralSlashMultMax)
							AttackDamage *= ReadConfig.SpiralSlashMultMax;
						else if (multiFix < ReadConfig.SpiralSlashMultMin)
							AttackDamage *= ReadConfig.SpiralSlashMultMin;
						else
							AttackDamage *= multiFix;
					}
					break;

				case AT_SKILL_FIRESLASH:		//MG
				case 500:	//Increase Fire Slash Skill
				case 501:	//Increase Fire Slash Skill
				case 502:	//Increase Fire Slash Skill
				case 503:	//Increase Fire Slash Skill
				case 504:	//Increase Fire Slash Skill
					{
						float multiFix = ( Energy/(float)ReadConfig.FireSlashEneDiv + ReadConfig.FireSlashBase ) /(float) ReadConfig.FireSlashMultDiv;
						if (multiFix > ReadConfig.FireSlashMultMax)
							AttackDamage *= ReadConfig.FireSlashMultMax;
						else if (multiFix < ReadConfig.FireSlashMultMin)
							AttackDamage *= ReadConfig.FireSlashMultMin;
						else
							AttackDamage *= multiFix;
					}
					break;

				case AT_SKILL_POWERSLASH:	//Power Slash Skill
				case 505:				//Increase Power Slash Skill
				case 506:				//Increase Power Slash Skill
				case 507:				//Increase Power Slash Skill
				case 508:				//Increase Power Slash Skill
				case 509:				//Increase Power Slash Skill
					{
						float multiFix = ( Energy/(float)ReadConfig.PowerSlashEneDiv + ReadConfig.PowerSlashBase ) /(float) ReadConfig.PowerSlashMultDiv;

						if (multiFix > ReadConfig.PowerSlashMultMax)
							AttackDamage *= ReadConfig.PowerSlashMultMax;
						else if (multiFix < ReadConfig.PowerSlashMultMin)
							AttackDamage *= ReadConfig.PowerSlashMultMin;
						else
							AttackDamage *= multiFix;
					}
					break;

				case AT_SKILL_STARFALL:
					{
						float multiFix = ( Energy/(float)ReadConfig.StarFallEneDiv + ReadConfig.StarFallBase ) /(float) ReadConfig.StarFallMultDiv;

						if (multiFix > ReadConfig.StarFallMultMax)
							AttackDamage *= ReadConfig.StarFallMultMax;
						else if (multiFix < ReadConfig.StarFallMultMin)
							AttackDamage *= ReadConfig.StarFallMultMin;
						else
							AttackDamage *= multiFix;
					}
					break;

				case AT_SKILL_ICEARROW:
					{
						float multiFix = ( Energy/(float)ReadConfig.IceArrowEneDiv + ReadConfig.IceArrowBase ) /(float) ReadConfig.IceArrowMultDiv;

						if (multiFix > ReadConfig.IceArrowMultMax)
							AttackDamage *= ReadConfig.IceArrowMultMax;
						else if (multiFix < ReadConfig.IceArrowMultMin)
							AttackDamage *= ReadConfig.IceArrowMultMin;
						else
							AttackDamage *= multiFix;
					}
					break;

				case AT_SKILL_PENETRATION:
					{
						float multiFix = ( Energy/(float)ReadConfig.PenetrationEneDiv + ReadConfig.PenetrationBase ) /(float) ReadConfig.PenetrationMultDiv;

						if (multiFix > ReadConfig.PenetrationMultMax)
							AttackDamage *= ReadConfig.PenetrationMultMax;
						else if (multiFix < ReadConfig.PenetrationMultMin)
							AttackDamage *= ReadConfig.PenetrationMultMin;
						else
							AttackDamage *= multiFix;
					}
					break;

				case AT_SKILL_FIVE_SHOT:
					{
						float multiFix = ( Energy/(float)ReadConfig.FiveShotEneDiv + ReadConfig.FiveShotBase ) /(float) ReadConfig.FiveShotMultDiv;

						if (multiFix > ReadConfig.FiveShotMultMax)
							AttackDamage *= ReadConfig.FiveShotMultMax;
						else if (multiFix < ReadConfig.FiveShotMultMin)
							AttackDamage *= ReadConfig.FiveShotMultMin;
						else
							AttackDamage *= multiFix;
					}
					break;

				case AT_SKILL_EXPHELL:
					{
						float multiFix = ( Energy/(float)ReadConfig.NovaEneDiv + ReadConfig.NovaBase ) /(float) ReadConfig.NovaMultDiv;

						if (multiFix > ReadConfig.NovaMultMax)
							AttackDamage *= ReadConfig.NovaMultMax;
						else if (multiFix < ReadConfig.NovaMultMin)
							AttackDamage *= ReadConfig.NovaMultMin;
						else
							AttackDamage *= multiFix;
					}
					break;

				case AT_SKILL_SPEAR:
					{
						float multiFix = ( Energy/(float)ReadConfig.ForceEneDiv + ReadConfig.ForceBase ) /(float) ReadConfig.ForceMultDiv;

						if (multiFix > ReadConfig.ForceMultMax)
							AttackDamage *= ReadConfig.ForceMultMax;
						else
							AttackDamage *= multiFix;
					}
					break;

				case AT_SKILL_LONGSPEAR:
					{
						float multiFix = ( Energy/(float)ReadConfig.ForceWaveEneDiv + ReadConfig.ForceWaveBase ) /(float) ReadConfig.ForceWaveMultDiv;

						if (multiFix > ReadConfig.ForceWaveMultMax)
							AttackDamage *= ReadConfig.ForceWaveMultMax;
						else
							AttackDamage *= multiFix;
					}
					break;

				case AT_SKILL_FIREBURST:
				case 520:	//Increase Fire Burst Skill
				case 521:	//Increase Fire Burst Skill
				case 522:	//Increase Fire Burst Skill
				case 523:	//Increase Fire Burst Skill
				case 524:	//Increase Fire Burst Skill
					{
						if(skill == AT_SKILL_FIREBURST && ReadConfig.IsSkillFireBurstStr == 0)
							break;

						float multiFix = ( Energy/(float)ReadConfig.FireBurstEneDiv + ReadConfig.FireBurstBase ) /(float) ReadConfig.FireBurstMultDiv;

						if (multiFix > ReadConfig.FireBurstMultMax)
							AttackDamage *= ReadConfig.FireBurstMultMax;
						else
							AttackDamage *= multiFix;
					}
					break;

				case AT_SKILL_DARKHORSE_ATTACK:
					{
						float multiFix = ( Energy/(float)ReadConfig.DarkHorseEneDiv + ReadConfig.DarkHorseBase ) /(float) ReadConfig.DarkHorseMultDiv;

						if (multiFix > ReadConfig.DarkHorseMultMax)
							AttackDamage *= ReadConfig.DarkHorseMultMax;
						else
							AttackDamage *= multiFix;
					}
					break;

				case AT_SKILL_ELECTRICSPARK:
					{
						float multiFix = ( Energy/(float)ReadConfig.ElectricSparkEneDiv + ReadConfig.ElectricSparkBase ) /(float) ReadConfig.ElectricSparkMultDiv;

						if (multiFix > ReadConfig.ElectricSparkMultMax)
							AttackDamage *= ReadConfig.ElectricSparkMultMax;
						else
							AttackDamage *= multiFix;
					}
					break;

				case AT_SKILL_BIRDS:
					{
						float multiFix = ( Energy/(float)ReadConfig.BirdsEneDiv + ReadConfig.BirdsBase ) /(float) ReadConfig.BirdsMultDiv;

						if (multiFix > ReadConfig.BirdsMultMax)
							AttackDamage *= ReadConfig.BirdsMultMax;
						else
							AttackDamage *= multiFix;
					}
					break;

				case AT_SKILL_SPACE_SPLIT:
					{
						float multiFix = ( Energy/(float)ReadConfig.FireBlastEneDiv + ReadConfig.FireBlastBase ) /(float) ReadConfig.FireBlastMultDiv;

						if (multiFix > ReadConfig.FireBlastMultMax)
							AttackDamage *= ReadConfig.FireBlastMultMax;
						else
							AttackDamage *= multiFix;
					}
					break;

				case AT_SKILL_FIRESCREAM:
				case 525:	//Increase Fire Scream Skill
				case 526:	//Increase Fire Scream Skill
				case 527:	//Increase Fire Scream Skill
				case 528:	//Increase Fire Scream Skill
				case 529:	//Increase Fire Scream Skill
					{
						float multiFix = ( Energy/(float)ReadConfig.FireScreamEneDiv + ReadConfig.FireScreamBase ) /(float) ReadConfig.FireScreamMultDiv;

						if (multiFix > ReadConfig.FireScreamMultMax)
							AttackDamage *= ReadConfig.FireScreamMultMax;
						else
							AttackDamage *= multiFix;
					}
					break;

				case AT_SKILL_FENRIR_ATTACK:
					{
						int iDamageInc = lpObj->Level - ReadConfig.FernirDmgBase;

						if ( iDamageInc < 0 )
							iDamageInc = 0;

						iDamageInc /= (float)ReadConfig.FernirDmgBaseDiv;
						AttackDamage = ( AttackDamage * ( iDamageInc + ReadConfig.FernirDmgAdd ) ) /(float) ReadConfig.FernirDmgTDiv;
					}
					break;
			}

			if ( skill == 0 )
			{
				if ( lpObj->pInventory[8].m_Type == ITEMGET(13, 3) )
				{
					AttackDamage = AttackDamage * 130 / 100;
				}
			}
			
			if ( lpTargetObj->m_WizardSkillDefense && AttackDamage > 0)
			{
				int replacemana = (WORD)lpTargetObj->Mana * 2 / 100;

				if ( replacemana < lpTargetObj->Mana )
				{
					lpTargetObj->Mana -= replacemana;
					int decattackdamage = AttackDamage * lpTargetObj->m_WizardSkillDefense / 100;
					AttackDamage -= decattackdamage;
					ManaChange = TRUE;

				}
			}

			AttackDamage += lpObj->SetOpAddDamage;

			if ( lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER )
			{
				AttackDamage += lpObj->m_ItemOptionExFor380.OpAddDamage;
			}

			if ( lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER )
			{
				if ( CC_MAP_RANGE(lpObj->MapNumber ) && CC_MAP_RANGE(lpTargetObj->MapNumber) )
				{
					AttackDamage = AttackDamage * 50 / 100;
				}
			}

			if ( lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_MONSTER )
			{
				if ( lpTargetObj->Class == 283 )
				{
					if ( lpObj->m_iPotionBlessTime > 0 )
					{
						AttackDamage += (AttackDamage * 20) / 100;
					}
					else if ( lpObj->m_iPotionSoulTime > 0 )
					{
						AttackDamage = AttackDamage;
					}
					else 
					{
						if ( lpObj->m_iAccumulatedDamage > 100 )
						{
							gObjWeaponDurDownInCastle(lpObj, lpTargetObj, 1);
							lpObj->m_iAccumulatedDamage = 0;
						}
						else
						{
							lpObj->m_iAccumulatedDamage += AttackDamage;
						}

						AttackDamage = AttackDamage * 5 / 100;
					}
				}

				if ( lpTargetObj->Class == 277 )
				{
					if ( lpObj->m_iPotionBlessTime > 0 )
					{
						AttackDamage += (AttackDamage * 20) / 100;
					}
					else if ( lpObj->m_iPotionSoulTime > 0 )
					{
						AttackDamage = AttackDamage;
					}
					else 
					{
						if ( lpObj->m_iAccumulatedDamage > 100 )
						{
							gObjWeaponDurDownInCastle(lpObj, lpTargetObj, 1);
							lpObj->m_iAccumulatedDamage = 0;
						}
						else
						{
							lpObj->m_iAccumulatedDamage += AttackDamage;
						}

						AttackDamage = AttackDamage * 5 / 100;
					}
				}
			}

			if ( gObjFenrir( lpObj ) )
			{
				int iIncPercent = lpObj->pInventory[8].IsFenrirIncLastAttackDamage();

				if ( iIncPercent > 0 )
				{
					AttackDamage += AttackDamage * iIncPercent / 100;
				}
			}

			if ( gObjFenrir( lpTargetObj ) )
			{
				int iDecPercent = lpTargetObj->pInventory[8].IsFenrirDecLastAttackDamage();

				if ( iDecPercent > 0 )
				{
					AttackDamage -= AttackDamage * iDecPercent / 100;
				}
			}

			if ( AttackDamage < 0 )
				AttackDamage = 0;

			if ( skill == AT_SKILL_FENRIR_ATTACK )
			{
				if ( lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER )
				{
					if ( AttackDamage > 0 )
					{
						int iEquipmentPos = rand()%5 + 2;	// Select and Armor
						CItem * lpEquipment = &lpTargetObj->pInventory[iEquipmentPos];

						if ( lpEquipment && lpEquipment->IsItem() )
						{
							int iDurEquipment = lpEquipment->m_Durability * 50.0f / 100.0f;
							lpEquipment->m_Durability = iDurEquipment;

							if ( lpEquipment->m_Durability < 0.0f )
							{
								lpEquipment->m_Durability = 0.0f;
							}

							GCItemDurSend(lpTargetObj->m_Index, iEquipmentPos, lpEquipment->m_Durability, 0);
						}
					}
				}
			}

			if ( lpTargetObj->m_SkillIT_Time > 0 )
			{
				if ( lpTargetObj->m_SkillIT_Number == IL_ORDER_OF_PROTECT )
				{
					AttackDamage = 0;
				}
			}

			if ( lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER )
			{
				iTempShieldDamage = this->GetShieldDamage(lpObj, lpTargetObj, AttackDamage);
				lpTargetObj->iShield -= iTempShieldDamage;
				lpTargetObj->Life -= AttackDamage - iTempShieldDamage;
				iTotalShieldDamage += iTempShieldDamage;

				if ( lpTargetObj->Life < 0.0f )
				{
					lpTargetObj->Life = 0.0f;
				}
			}
			else
			{
				lpTargetObj->Life -= AttackDamage;

				if ( lpTargetObj->Life < 0.0f )
				{
					lpTargetObj->Life = 0.0f;
				}
			}
		}
	}	
	else
	{
		if ( lpTargetObj->m_SkillIT_Time > 0 )
		{
			if ( lpTargetObj->m_SkillIT_Number == IL_ORDER_OF_PROTECT )
			{
				AttackDamage = 0;
			}
		}

		if ( skill != AT_SKILL_EXPLOSION /*79*/ )
		{
			bDamageReflect = TRUE;
			MsgDamage = MSG_DAMAGE_REFLECT;
		}

		if ( lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER )
		{
			iTempShieldDamage = this->GetShieldDamage(lpObj, lpTargetObj, AttackDamage);
			lpTargetObj->iShield -= iTempShieldDamage;
			lpTargetObj->Life -= AttackDamage - iTempShieldDamage;
			iTotalShieldDamage += iTempShieldDamage;

			if ( lpTargetObj->Life < 0.0f )
			{
				lpTargetObj->Life = 0.0f;
			}
		}
		else
		{
			lpTargetObj->Life -= AttackDamage;

			if ( lpTargetObj->Life < 0.0f )
			{
				lpTargetObj->Life = 0.0f;
			}
		}
	}

	if ( lpTargetObj->Type == OBJ_MONSTER )
	{
		gObjAddMsgSendDelay(lpTargetObj, 0, lpObj->m_Index, 100, 0);
		lpTargetObj->LastAttackerID = lpObj->m_Index;

		if ( lpTargetObj->m_iCurrentAI )
		{
			lpTargetObj->m_Agro.IncAgro(lpObj->m_Index, AttackDamage / 50);
		}
	}

	BOOL selfdefense = 0;
	lpCallObj = lpTargetObj;
	
	if ( lpTargetObj->Type == OBJ_MONSTER )
	{
		if ( lpTargetObj->m_RecallMon >= 0 )
		{
			lpCallObj = &gObj[lpTargetObj->m_RecallMon];
		}
	}

	if ( AttackDamage >= 1 )
	{
		if ( lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER )
		{
			if ( g_DuelManager.gObjDuelCheck(lpObj, lpTargetObj) )
			{
				selfdefense = 0;
			}
			else if ( CC_MAP_RANGE(lpObj->MapNumber) || CC_MAP_RANGE(lpTargetObj->MapNumber) )
			{
				selfdefense = 0;
			}
			else if ( IT_MAP_RANGE(lpObj->MapNumber) || IT_MAP_RANGE(lpTargetObj->MapNumber) )
			{
				selfdefense = 0;
			}
			else
			{
				selfdefense = 1;
			}

			if ( gObjGetRelationShip(lpObj, lpTargetObj) == 2 )
			{
				selfdefense = FALSE;
			}
		}
		else if ( lpTargetObj->Type == OBJ_MONSTER && lpObj->Type == OBJ_USER )
		{
			if ( lpTargetObj->m_RecallMon >= 0 )
			{
				selfdefense = TRUE;
			}
		}

		if ( lpTargetObj->Type == OBJ_USER )
		{
			if(ReadConfig.S6E2 == 1 && skill == AT_SKILL_BLOWOFFURY)
			{
				if ( lpObj->Type == OBJ_USER )
				{
					if(lpObj->MasterCharacterInfo->BrokenArmorRagefulBlow > 0)
					{
						if ( (rand()%100) < lpObj->MasterCharacterInfo->BrokenArmorRagefulBlow )
						{
							gObjArmorRandomDurDown(lpTargetObj, lpObj,false);
						}else
						{
							gObjArmorRandomDurDown(lpTargetObj, lpObj,true);
						}
					}else
					{
						gObjArmorRandomDurDown(lpTargetObj, lpObj,true);
					}
				}				
			}else
			{
				gObjArmorRandomDurDown(lpTargetObj, lpObj,true);
			}
		}

		if ( lpTargetObj->m_SkillHarden )
		{
			lpTargetObj->m_SkillHarden--;

			if ( lpTargetObj->m_SkillHarden <= 0 )
			{
				lpTargetObj->m_SkillHardenTime = 0;
				lpTargetObj->m_SkillHarden = 0;
				lpTargetObj->m_ViewSkillState &= -33;
				GCMagicCancelSend(lpTargetObj, 51);
			}
		}
	}

	if ( selfdefense == TRUE && bDamageReflect == FALSE )
	{
		if ( !gObjTargetGuildWarCheck(lpObj, lpCallObj) )
		{
			gObjCheckSelfDefense(lpObj, lpCallObj->m_Index);
		}
	}

	if ( lpTargetObj->Class == 275 )	// KUNDUN
	{
		if ( lpTargetObj->m_iMonsterBattleDelay <= 0 )
		{
			if ( (rand()%15) < 1 )
			{
				gObjAddMsgSendDelay(lpTargetObj, 4, lpObj->m_Index, 100, 0);
				lpTargetObj->m_iMonsterBattleDelay = 10;
				GCActionSend(lpTargetObj, 126, lpTargetObj->m_Index, lpObj->m_Index);
			}
		}
	}

	if ( lpTargetObj->Class == 338 )	// KUNDUN 7
	{
		if ( lpTargetObj->m_iMonsterBattleDelay <= 0 )
		{
			if ( (rand()%15) < 1 )
			{
				gObjAddMsgSendDelay(lpTargetObj, 4, lpObj->m_Index, 100, 0);
				lpTargetObj->m_iMonsterBattleDelay = 10;
				GCActionSend(lpTargetObj, 126, lpTargetObj->m_Index, lpObj->m_Index);
			}
		}
	}

	if ( lpTargetObj->Class == 131 ||BC_STATUE_RANGE(lpTargetObj->Class-132) )
	{
		gObjAddMsgSendDelay(lpTargetObj, 4, lpObj->m_Index, 100, 0);
		gObjSetPosition(lpTargetObj->m_Index, lpTargetObj->X, lpTargetObj->Y);
	}
	else if ( AttackDamage >= 5 )	// To make strong hit
	{
		if ( lpTargetObj->Type == OBJ_MONSTER )
		{
			if ( (rand()%26) == 0 )
			{
				gObjAddMsgSendDelay(lpTargetObj,4, lpObj->m_Index, 100, 0);
			}
		}
		else if ( (rand()%4) == 0 )
		{
			if ( !gObjUniriaSprite(lpTargetObj) )
			{
				MSBFlag = 1;
			}
		}
	}

	if ( ManaChange )
	{
		GCManaSend(lpTargetObj->m_Index, lpTargetObj->Mana, 0xFF, 0, lpTargetObj->BP);
	}

	if ( magicsend )
	{	
#if (PACK_EDITION>=3)
		if(lpObj->IsBot >= 1)
		{
			if(ReadConfig.S5E2 == TRUE)
			{
				PMSG_DURATION_MAGIC_RECV_S5E2 pDuration;


				pDuration.MagicNumberL = 0;
				pDuration.Dir = 0;
				pDuration.X = lpObj->X;
				pDuration.Y = lpObj->Y;

				CGDurationMagicRecv((unsigned char *) &pDuration, lpObj->m_Index);
			}else
			{
				PMSG_DURATION_MAGIC_RECV pDuration;


				pDuration.MagicNumberL = 0;
				pDuration.Dir = 0;
				pDuration.X = lpObj->X;
				pDuration.Y = lpObj->Y;

				CGDurationMagicRecv((unsigned char *) &pDuration, lpObj->m_Index);
			}
		}
#endif
		GCMagicAttackNumberSend(lpObj, skill, lpTargetObj->m_Index, skillSuccess);
	}

	if ( lpObj->Type == OBJ_USER )
	{
		if ( lpObj->m_Change == 9 )
		{
			GCMagicAttackNumberSend(lpObj, 3, lpTargetObj->m_Index, 1);
		}
	}

	if ( lpObj->Class == CLASS_ELF && lpObj->Level == 1 && AttackDamage > 10 )
	{
		if(ReadConfig.S6E2 == 0)
		{
			LogAdd("error-Level1 : [%s][%s] Str:%d %d %d %d %d %d %d",
				lpObj->AccountID, lpObj->Name, lpObj->Strength,
				lpObj->m_AttackDamageMinRight, lpObj->m_AttackDamageMaxRight,
				lpObj->m_AttackDamageMinLeft, lpObj->m_AttackDamageMaxLeft, 
				lpObj->m_AttackDamageMax, lpObj->m_AttackDamageMin);
		}
	}

	lpObj->m_Rest = 0;

	if ( AttackDamage > 0 )
	{
		int atd_reflect = (int)((float)AttackDamage * lpTargetObj->DamageReflect / 100.0f);

		if ( atd_reflect )
		{
			gObjAddMsgSendDelay(lpTargetObj, 10, lpObj->m_Index, 10, atd_reflect);
		}

		if ( (rand()%100) < lpObj->SetOpReflectionDamage )
		{
			gObjAddMsgSendDelay(lpTargetObj, 10, lpObj->m_Index, 10, AttackDamage);
		}

		if ( bCombo )
		{
			int iComboDamage = ( Strength + Dexterity + Energy ) / ReadConfig.ComboDivisor;	// #formula
			AttackDamage += iComboDamage;

			if ( lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER )
			{
				iTempShieldDamage = this->GetShieldDamage(lpObj, lpTargetObj, iComboDamage);
				lpTargetObj->iShield -= iTempShieldDamage;
				lpTargetObj->Life -= iComboDamage - iTempShieldDamage;
				iTotalShieldDamage += iTempShieldDamage;

				if ( lpTargetObj->Life < 0.0f )
				{
					lpTargetObj->Life = 0.0f;
				}
			}
			else
			{
				lpTargetObj->Life -= iComboDamage;

				if ( lpTargetObj->Life < 0.0f )
				{
					lpTargetObj->Life = 0.0f;
				}
			}

			MsgDamage |= MSG_DAMAGE_COMBO;
			skill = AT_SKILL_COMBO;
		}

		if ( (rand()%100) < lpObj->SetOpDoubleDamage )
		{
			if (( skill == AT_SKILL_FIRESCREAM ) || (skill >= 525 && skill <= 529)) 
			{
				gObjUseSkill.FireScreamExplosionAttack(lpObj, lpTargetObj, AttackDamage);
			}

			if ( lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER )
			{
				iTempShieldDamage = this->GetShieldDamage(lpObj, lpTargetObj, AttackDamage);
				lpTargetObj->iShield -= iTempShieldDamage;
				lpTargetObj->Life -= AttackDamage - iTempShieldDamage;
				iTotalShieldDamage += iTempShieldDamage;
				AttackDamage += AttackDamage;

				if ( lpTargetObj->Life < 0.0f )
				{
					lpTargetObj->Life = 0.0f;
				}
			}
			else
			{
				lpTargetObj->Life -= AttackDamage;
				AttackDamage += AttackDamage;

				if ( lpTargetObj->Life < 0.0f )
				{
					lpTargetObj->Life = 0.0f;
				}
			}

			MsgDamage |= MSG_DAMAGE_DOUBLE;
		}

		if ( g_ShieldSystemOn == TRUE )
		{
			AttackDamage -= iTotalShieldDamage;
		}

		if ( g_ShieldSystemOn == FALSE )
		{
			if ( lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER )
			{
				if ( CC_MAP_RANGE(lpObj->MapNumber) && CC_MAP_RANGE(lpTargetObj->MapNumber) )
				{
					AttackDamage = AttackDamage * 50 / 100;
				}
			}

			if ( lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER )
			{
				if ( IT_MAP_RANGE(lpObj->MapNumber) && IT_MAP_RANGE(lpTargetObj->MapNumber) )
				{
					AttackDamage = AttackDamage * 50 / 100;
				}
			}
		}

		//Season3 Third Wings
		if(lpTargetObj->Type == OBJ_USER && lpTargetObj->pInventory[7].CheckDamageReturnWing())
		{
			BYTE tmp_Wings3ReturnDamageSuccessRate = g_iWings3ReturnDamageSuccessRate;
#if (CRYSTAL_EDITION == 1)
			if(lpTargetObj->pInventory[7].m_Type >= ITEMGET(12,200) && lpTargetObj->pInventory[7].m_Type <= ITEMGET(12,254))
			{
				tmp_Wings3ReturnDamageSuccessRate = CWings.GetExcOption(lpTargetObj->pInventory[7].m_Type,1);
			}
#endif
			if(rand()%100 <= tmp_Wings3ReturnDamageSuccessRate )
			{
				int newAttackDmg = AttackDamage;

				if(g_ShieldSystemOn == 1)
				{
					if(lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER)
					{
						if(lpObj->iShield > AttackDamage)
						{
							lpObj->iShield -= AttackDamage;
							iTotalShieldDamage += AttackDamage;
							AttackDamage = 0;
						}
						else
						{
							AttackDamage -= lpObj->iShield;
							iTotalShieldDamage += lpObj->iShield;
							lpObj->iShield = 0;

							lpObj->Life -= AttackDamage;

							if(lpObj->Life < 0)
							{
								lpObj->Life = 0;
							}
						}
					}
					else
					{
						lpObj->Life -= AttackDamage;

						if(lpObj->Life < 0)
						{
							lpObj->Life = 0;
						}
					}
				}
				else
				{
					lpObj->Life -= AttackDamage;

					if(lpObj->Life < 0)
					{
						lpObj->Life = 0;
					}
				}

				lpTargetObj->Life += newAttackDmg;
				if(lpTargetObj->Life > lpTargetObj->MaxLife + lpTargetObj->AddLife)
					lpTargetObj->Life = lpTargetObj->MaxLife + lpTargetObj->AddLife;

				GCReFillSend(lpObj->m_Index,lpObj->Life,0xFF,FALSE,lpObj->iShield);
				GCReFillSend(lpTargetObj->m_Index,lpTargetObj->Life,0xFF,FALSE,lpTargetObj->iShield);

				gObjLifeCheck(lpObj, lpTargetObj, AttackDamage, DAMAGE_TYPE_REG, MSBFlag, MsgDamage, skill, iTotalShieldDamage);
			}
			else
			{
				gObjLifeCheck(lpTargetObj, lpObj, AttackDamage, DAMAGE_TYPE_REG, MSBFlag, MsgDamage, skill, iTotalShieldDamage);
			}
		}
		else
		{
			gObjLifeCheck(lpTargetObj, lpObj, AttackDamage, DAMAGE_TYPE_REG, MSBFlag, MsgDamage, skill, iTotalShieldDamage);
		}

		if ( iTotalShieldDamage > 0 )
		{
			LogAddTD("[PvP System] Victim:[%s][%s], Attacker:[%s][%s] - SD[%d] HP[%f] -> SD[%d] HP[%f]",
				lpTargetObj->AccountID, lpTargetObj->Name, lpObj->AccountID, lpObj->Name,
				lpTargetObj->iShield + iTotalShieldDamage, lpTargetObj->Life + AttackDamage, 
				lpTargetObj->iShield, lpTargetObj->Life);
		}
	}
	else
	{
		GCDamageSend(lpObj->m_Index, lpTargetObj->m_Index, 0, 0, MsgDamage, 0);
	}

	if ( lpObj->Life <= 0.0f && lpObj->Type == OBJ_USER )
	{
		if ( lpObj->m_CheckLifeTime <= 0 )
		{
			lpObj->lpAttackObj = lpTargetObj;

			if ( lpTargetObj->Type == OBJ_USER )
			{
				lpObj->m_bAttackerKilled = true;
			}
			else
			{
				lpObj->m_bAttackerKilled = false;
			}

			lpObj->m_CheckLifeTime = 3;
		}
	}

	BYTE tmp_g_iWings3RecoverFullLifeSuccessRate = g_iWings3RecoverFullLifeSuccessRate;
#if (CRYSTAL_EDITION == 1)
	if(lpTargetObj->pInventory[7].m_Type >= ITEMGET(12,200) && lpTargetObj->pInventory[7].m_Type <= ITEMGET(12,254))
	{
		tmp_g_iWings3RecoverFullLifeSuccessRate = CWings.GetExcOption(lpTargetObj->pInventory[7].m_Type,2);
	}
#endif
	//Season3 Third Wings
	if(lpTargetObj->Type == OBJ_USER &&
	   lpTargetObj->pInventory[7].CheckFullLifeRecoveryWing() &&
	   rand()%100 <= tmp_g_iWings3RecoverFullLifeSuccessRate &&
	   lpTargetObj->Life > 0.0f)
	{
		//lpTargetObj->Life = lpTargetObj->MaxLife;
		int tLife = 0;

		tLife += ((int)(lpTargetObj->MaxLife + lpTargetObj->AddLife));

		lpTargetObj->Life += tLife;

		if ( lpTargetObj->Life > (lpTargetObj->MaxLife+lpTargetObj->AddLife-1.0f) )
				lpTargetObj->Life = lpTargetObj->MaxLife+lpTargetObj->AddLife;

		GCReFillSend(lpTargetObj->m_Index,lpTargetObj->Life,0xFF,FALSE,lpTargetObj->iShield);

		lpTargetObj->FillLifeMax = tLife;
		lpTargetObj->FillLife = tLife;
	}

	BYTE tmp_g_iWings3RecoverFullManaSuccessRate = g_iWings3RecoverFullManaSuccessRate;
#if (CRYSTAL_EDITION == 1)
	if(lpTargetObj->pInventory[7].m_Type >= ITEMGET(12,200) && lpTargetObj->pInventory[7].m_Type <= ITEMGET(12,254))
	{
		tmp_g_iWings3RecoverFullManaSuccessRate = CWings.GetExcOption(lpTargetObj->pInventory[7].m_Type,3);
	}
#endif

	if(lpTargetObj->Type == OBJ_USER &&
	   lpTargetObj->pInventory[7].CheckFullManaRecoveryWing() &&
	   rand()%100 <= tmp_g_iWings3RecoverFullManaSuccessRate )
	{
		//lpTargetObj->Mana = lpTargetObj->MaxMana;
		int tMana = 0;

		tMana += ((int)(lpTargetObj->MaxMana + lpTargetObj->AddMana));

		lpTargetObj->Mana += tMana;

		if ( lpTargetObj->Mana > (lpTargetObj->MaxMana+lpTargetObj->AddMana-1.0f) )
				lpTargetObj->Mana = lpTargetObj->MaxMana+lpTargetObj->AddMana;

		GCManaSend(lpTargetObj->m_Index,lpTargetObj->Mana,0xFF,FALSE,lpTargetObj->BP);
	}

	if ( lpMagic )
	{
		gObjUseSkill.SpecificSkillAdditionTreat(lpObj, lpTargetObj, lpMagic, AttackDamage);
	}

#if (PACK_EDITION>=3)
	if(BotChangeSkill == true && lpObj->BotSkillAttack == 1)
	{
		lpObj->m_AttackType = lpObj->m_SkillNumber;
		lpObj->m_AttackRange = MagicDamageC.GetSkillDistance(lpObj->m_SkillNumber);
	}
#endif

	return TRUE;
}


int  CObjAttack::GetAttackDamage(LPOBJ lpObj, int targetDefense, BYTE& MsgDamage, BOOL bIsOnDuel, CMagicInf* lpMagic)
{
	if ( g_ShieldSystemOn == TRUE )
	{
		if ( bIsOnDuel == TRUE )
		{
			bIsOnDuel = FALSE;
		}
	}
	int ad;
	int sub;
	int SkillRightMaxDamage =0;
	int SkillRightMinDamage =0;
	int SkillLeftMaxDamage =0;
	int SkillLeftMinDamage =0;
	CItem * Right = &lpObj->pInventory[0];
	CItem * Left = &lpObj->pInventory[1];
	BOOL bTwoHandWeapon = FALSE;
	BOOL isMagic=FALSE;

	if (( lpObj->Type == OBJ_MONSTER || lpObj->Type == OBJ_NPC )
#if (PACK_EDITION>=3)
		&& (lpObj->IsBot == 0)
#endif
		)
	{
		int AttackMin = 0;
		sub = lpObj->m_AttackDamageMax - lpObj->m_AttackDamageMin;
		if (sub > 0)
			AttackMin = lpObj->m_AttackDamageMin + (rand()%(sub+1));
		else
			AttackMin = lpObj->m_AttackDamageMin;

		lpObj->m_AttackDamageRight = AttackMin;
		lpObj->m_AttackDamageLeft = AttackMin;
	}
	else
	{
		int cDamage=0;

		if ( lpObj->Class == CLASS_DARKLORD ||
			 lpObj->Class == CLASS_KNIGHT   ||
			 lpObj->Class == CLASS_MAGICGLADIATOR || 
			 lpObj->Class == CLASS_RAGEFIGHTER )
		{
			if ( Right->m_Type >= ITEMGET(0,0) && Right->m_Type < ITEMGET(4,0) &&
				 Left->m_Type >= ITEMGET(0,0) && Left->m_Type < ITEMGET(4,0) )
			{
				if ( Right->m_IsValidItem && Left->m_IsValidItem )
				{
					bTwoHandWeapon = TRUE;
				}
			}
		}

		if ( lpMagic )
		{
			isMagic = TRUE;
			if ( lpMagic->m_Skill == AT_SKILL_SPEAR && lpObj->SkillLongSpearChange )	// #error 60 and skil from GEt is 66???
			{
				SkillRightMaxDamage = DefMagicInf[AT_SKILL_SPEAR].m_DamageMax;// FIX
				SkillRightMinDamage = DefMagicInf[AT_SKILL_SPEAR].m_DamageMin;// FIX
				//SkillRightMaxDamage = DefMagicInf[AT_SKILL_LONGSPEAR].m_DamageMax;// #error ???
				//SkillRightMinDamage = DefMagicInf[AT_SKILL_LONGSPEAR].m_DamageMin;// #error ???
			}
			else
			{
				if (( lpMagic->m_Skill >= 455 && lpMagic->m_Skill <= 459 ) || 
					( lpMagic->m_Skill >= 495 && lpMagic->m_Skill <= 499 ))
				{
					SkillRightMaxDamage = DefMagicInf[AT_SKILL_TWISTINGSLASH].m_DamageMax;
					SkillRightMinDamage = DefMagicInf[AT_SKILL_TWISTINGSLASH].m_DamageMax;
				}else if(lpMagic->m_Skill >= 460 && lpMagic->m_Skill <= 464)
				{
					SkillRightMaxDamage = DefMagicInf[AT_SKILL_DEATHSTAB].m_DamageMax;
					SkillRightMinDamage = DefMagicInf[AT_SKILL_DEATHSTAB].m_DamageMax;				
				}else if(lpMagic->m_Skill >= 465 && lpMagic->m_Skill <= 469)
				{
					SkillRightMaxDamage = DefMagicInf[AT_SKILL_BLOWOFFURY].m_DamageMax;
					SkillRightMinDamage = DefMagicInf[AT_SKILL_BLOWOFFURY].m_DamageMax;
				}else
				{
					SkillRightMaxDamage = lpMagic->m_DamageMax;
					SkillRightMinDamage = lpMagic->m_DamageMin;
				}
			}

			if ( bTwoHandWeapon == FALSE )
			{
				SkillLeftMaxDamage = SkillRightMaxDamage;
				SkillLeftMinDamage = SkillRightMinDamage;
			}

			SkillRightMaxDamage += lpObj->SetOpAddSkillAttack;
			SkillRightMinDamage += lpObj->SetOpAddSkillAttack;
			SkillLeftMaxDamage += lpObj->SetOpAddSkillAttack;
			SkillLeftMinDamage += lpObj->SetOpAddSkillAttack;

			int SkillAttr = MagicDamageC.GetSkillResistance(lpMagic->m_Skill);

			if ( SkillAttr != -1 )
			{
				SkillRightMaxDamage += (BYTE)lpObj->m_AddResistance[SkillAttr];
				SkillRightMinDamage += (BYTE)lpObj->m_AddResistance[SkillAttr];
				SkillLeftMaxDamage  += (BYTE)lpObj->m_AddResistance[SkillAttr];
				SkillLeftMinDamage  += (BYTE)lpObj->m_AddResistance[SkillAttr];
			}

			SkillRightMaxDamage += lpObj->m_JewelOfHarmonyEffect.HJOpAddSkillAttack;
			SkillRightMinDamage += lpObj->m_JewelOfHarmonyEffect.HJOpAddSkillAttack;
			SkillLeftMaxDamage += lpObj->m_JewelOfHarmonyEffect.HJOpAddSkillAttack;
			SkillLeftMinDamage += lpObj->m_JewelOfHarmonyEffect.HJOpAddSkillAttack;

			int addskilldamage = 0;
			int iadddamage = 0;

			if ( lpMagic->m_Skill == AT_SKILL_ELECTRICSPARK )
			{
				int iPartyCount = this->GetPartyMemberCount(lpObj);

				if (ReadConfig.ElectricSparkDmgCmdDiv > 0)
					addskilldamage = ( (lpObj->Leadership + lpObj->AddLeadership) / (float)ReadConfig.ElectricSparkDmgCmdDiv ) + ( iPartyCount * ReadConfig.ElectricSparkDmgPartyMul );	// #formula
				else
					addskilldamage = ( (lpObj->Leadership + lpObj->AddLeadership) / 10 ) + ( iPartyCount * ReadConfig.ElectricSparkDmgPartyMul );	// #formula

				SkillRightMaxDamage += addskilldamage;
				SkillRightMinDamage += addskilldamage;
				SkillLeftMaxDamage += addskilldamage;
				SkillLeftMinDamage += addskilldamage;
			}
			else if((lpMagic->m_Skill == AT_SKILL_DARKHORSE_ATTACK) || 
					(lpMagic->m_Skill >= 515 && lpMagic->m_Skill <= 519))
			{
				if ( lpObj->pInventory[8].m_Type == ITEMGET(13,4) )	// Dark Horse
				{
					if (ReadConfig.DarkHorseDmgStrDiv > 0 && ReadConfig.DarkHorseDmgCmdDiv > 0)
						addskilldamage = ( lpObj->Strength + lpObj->AddStrength ) / (float)ReadConfig.DarkHorseDmgStrDiv + ( lpObj->Leadership + lpObj->AddLeadership ) / (float)ReadConfig.DarkHorseDmgCmdDiv + lpObj->pInventory[8].m_PetItem_Level * ReadConfig.DarkHorseDmgLvlMul;	// #formula
					else
						addskilldamage = (( lpObj->Strength + lpObj->AddStrength ) / 10 ) + (( lpObj->Leadership + lpObj->AddLeadership ) / 5) + lpObj->pInventory[8].m_PetItem_Level * ReadConfig.DarkHorseDmgLvlMul;	// #formula

					SkillRightMaxDamage += addskilldamage;
					SkillRightMinDamage += addskilldamage;
					SkillLeftMaxDamage += addskilldamage;
					SkillLeftMinDamage += addskilldamage;
					if (lpMagic->m_Skill >= 515 && lpMagic->m_Skill <= 519)
					{
						int Max = lpMagic->m_DamageMax - DefMagicInf[AT_SKILL_DARKHORSE_ATTACK].m_DamageMax;
						int Min = lpMagic->m_DamageMin - DefMagicInf[AT_SKILL_DARKHORSE_ATTACK].m_DamageMin;

						if(Max > 0 && Min > 0)
						{
							SkillRightMaxDamage += Max;
							SkillRightMinDamage += Min;
							SkillLeftMaxDamage += Max;
							SkillLeftMinDamage += Min;
						}
					}
				}
			}
			else if ( lpObj->Class == CLASS_DARKLORD )
			{
				if (ReadConfig.DLAddSkillDmgEneDiv > 0 && ReadConfig.DLAddSkillDmgStrDiv > 0)
					iadddamage = ( lpObj->Strength + lpObj->AddStrength ) / (float)ReadConfig.DLAddSkillDmgStrDiv + ( lpObj->Energy + lpObj->AddEnergy ) / (float)ReadConfig.DLAddSkillDmgEneDiv;	// #formula
				else
					iadddamage = ( lpObj->Strength + lpObj->AddStrength ) / 25 + ( lpObj->Energy + lpObj->AddEnergy ) / 50;	// #formula

				SkillRightMaxDamage += iadddamage;
				SkillRightMinDamage += iadddamage;
				SkillLeftMaxDamage += iadddamage;
				SkillLeftMinDamage += iadddamage;
			}

			if ( lpObj->Type == OBJ_USER )
			{
				SkillRightMaxDamage += lpObj->MasterCharacterInfo->IncMaxSkillPower + lpObj->MasterCharacterInfo->IncMaxDmgSkillPower;
				SkillRightMinDamage += lpObj->MasterCharacterInfo->IncMinSkillPower + lpObj->MasterCharacterInfo->IncMinDmgSkillPower;
				SkillLeftMaxDamage += lpObj->MasterCharacterInfo->IncMaxSkillPower + lpObj->MasterCharacterInfo->IncMaxDmgSkillPower;
				SkillLeftMinDamage += lpObj->MasterCharacterInfo->IncMinSkillPower + lpObj->MasterCharacterInfo->IncMinDmgSkillPower;
			}
		}

		if ( lpObj->m_CriticalDamage > 0 )
		{
			if ( (rand()%100) < lpObj->m_CriticalDamage )
			{
				cDamage = TRUE;
				MsgDamage = MSG_DAMAGE_CRITICAL;
			}
		} 
		
		if ( lpObj->m_ExcelentDamage > 0 )
		{
			if ( (rand()%100) < lpObj->m_ExcelentDamage )
			{
				cDamage = TRUE;
				MsgDamage = MSG_DAMAGE_EXCELLENT;
			}
		} 
		
		//BYTE effectTmp = MsgDamage;
		if ( lpObj->Class == CLASS_KNIGHT )
		{
			if(lpObj->MasterCharacterInfo->SpearDoubleDmg > 0)
			{
				CItem * Right = &lpObj->pInventory[0];
				if ( Right->IsItem() == TRUE )
				{
					if(Right->m_Type >= ITEMGET(4,0) && Right->m_Type < ITEMGET(5,0))
					{
						if ( (rand()%100) < lpObj->MasterCharacterInfo->SpearDoubleDmg )
						{
							cDamage = TRUE;
							MsgDamage |= MSG_DAMAGE_DOUBLE;
						}
					}
				}
			}
		}

		if ( lpObj->Class == CLASS_RAGEFIGHTER )
		{
			if(lpObj->MasterCharacterInfo->SpearDoubleDmg > 0)
			{
				CItem * Right = &lpObj->pInventory[0];
				CItem * Left  = &lpObj->pInventory[1];
				bool enter = false;
				if ( Right->IsItem() == TRUE )
				{
					if(Right->m_Type >= ITEMGET(0,32) && Right->m_Type <= ITEMGET(0,35))
					{
						if ( (rand()%100) < lpObj->MasterCharacterInfo->SpearDoubleDmg )
						{
							cDamage = TRUE;
							MsgDamage |= MSG_DAMAGE_DOUBLE;
							enter = true;
						}
					}
				}
				if(enter == false)
				{
					if ( Left->IsItem() == TRUE )
					{
						if(Left->m_Type >= ITEMGET(0,32) && Left->m_Type <= ITEMGET(0,35))
						{
							if ( (rand()%100) < lpObj->MasterCharacterInfo->SpearDoubleDmg )
							{
								cDamage = TRUE;
								MsgDamage |= MSG_DAMAGE_DOUBLE;
							}
						}
					}
				}
			}
		}

		__try
		{
			if(isMagic == FALSE)
			{
				if ( lpObj->Type == OBJ_USER )
				{
					sub = ( lpObj->m_AttackDamageMaxRight + SkillRightMaxDamage + lpObj->MasterCharacterInfo->IncMaxDmg + lpObj->MasterCharacterInfo->IncMaxDmgSkillPower) - ( lpObj->m_AttackDamageMinRight + SkillRightMinDamage + lpObj->MasterCharacterInfo->IncMinDmg + lpObj->MasterCharacterInfo->IncMinDmgSkillPower);	// #formula
					if (sub > 0)
						lpObj->m_AttackDamageRight = ( lpObj->m_AttackDamageMinRight + SkillRightMinDamage + lpObj->MasterCharacterInfo->IncMinDmg + lpObj->MasterCharacterInfo->IncMinDmgSkillPower ) + (rand()%(sub+1));
					else
						lpObj->m_AttackDamageRight = ( lpObj->m_AttackDamageMinRight + SkillRightMinDamage + lpObj->MasterCharacterInfo->IncMinDmg + lpObj->MasterCharacterInfo->IncMinDmgSkillPower );
				} else {
					sub = ( lpObj->m_AttackDamageMaxRight + SkillRightMaxDamage + SkillRightMinDamage);
					if (sub > 0)
						lpObj->m_AttackDamageRight = ( lpObj->m_AttackDamageMinRight + SkillRightMinDamage ) + (rand()%(sub+1));
					else
						lpObj->m_AttackDamageRight = ( lpObj->m_AttackDamageMinRight + SkillRightMinDamage );
				}
			}
			else
			{
				if ( lpObj->Type == OBJ_USER )
				{
					sub = ( lpObj->m_AttackDamageMaxRight + SkillRightMaxDamage + lpObj->MasterCharacterInfo->IncMaxDmg) - ( lpObj->m_AttackDamageMinRight + SkillRightMinDamage + lpObj->MasterCharacterInfo->IncMinDmg);	// #formula
					if (sub > 0)
						lpObj->m_AttackDamageRight = ( lpObj->m_AttackDamageMinRight + SkillRightMinDamage + lpObj->MasterCharacterInfo->IncMinDmg ) + (rand()%(sub+1));
					else
						lpObj->m_AttackDamageRight = ( lpObj->m_AttackDamageMinRight + SkillRightMinDamage + lpObj->MasterCharacterInfo->IncMinDmg );
				} else {
					sub = ( lpObj->m_AttackDamageMaxRight + SkillRightMaxDamage ) - ( lpObj->m_AttackDamageMinRight + SkillRightMinDamage );	// #formula
					if (sub > 0)
						lpObj->m_AttackDamageRight = ( lpObj->m_AttackDamageMinRight + SkillRightMinDamage ) + (rand()%(sub+1));
					else
						lpObj->m_AttackDamageRight = ( lpObj->m_AttackDamageMinRight + SkillRightMinDamage );
				}
			}
		}
		__except ( sub=1, 1 )
		{

		}

		__try
		{
			if(isMagic == FALSE)
			{
				if ( lpObj->Type == OBJ_USER )
				{
					sub = ( lpObj->m_AttackDamageMaxLeft + SkillLeftMaxDamage + lpObj->MasterCharacterInfo->IncMaxDmg + lpObj->MasterCharacterInfo->IncMaxDmgSkillPower) - ( lpObj->m_AttackDamageMinLeft + SkillLeftMinDamage + lpObj->MasterCharacterInfo->IncMinDmg + lpObj->MasterCharacterInfo->IncMinDmgSkillPower);	// #formula
					if (sub > 0)
						lpObj->m_AttackDamageLeft = ( lpObj->m_AttackDamageMinLeft + SkillLeftMinDamage + lpObj->MasterCharacterInfo->IncMinDmg + lpObj->MasterCharacterInfo->IncMinDmgSkillPower) + (rand()%(sub+1));
					else
						lpObj->m_AttackDamageLeft = ( lpObj->m_AttackDamageMinLeft + SkillLeftMinDamage + lpObj->MasterCharacterInfo->IncMinDmg + lpObj->MasterCharacterInfo->IncMinDmgSkillPower);
				} else {
					sub = ( lpObj->m_AttackDamageMaxLeft + SkillLeftMaxDamage ) - ( lpObj->m_AttackDamageMinLeft + SkillLeftMinDamage );
					if (sub > 0)
						lpObj->m_AttackDamageLeft = ( lpObj->m_AttackDamageMinLeft + SkillLeftMinDamage ) + (rand()%(sub+1));
					else
						lpObj->m_AttackDamageLeft = ( lpObj->m_AttackDamageMinLeft + SkillLeftMinDamage );
				}
			}
			else
			{				
				if ( lpObj->Type == OBJ_USER )
				{
					sub = ( lpObj->m_AttackDamageMaxLeft + SkillLeftMaxDamage + lpObj->MasterCharacterInfo->IncMaxDmg) - ( lpObj->m_AttackDamageMinLeft + SkillLeftMinDamage + lpObj->MasterCharacterInfo->IncMinDmg);	// #formula
					if (sub > 0)
						lpObj->m_AttackDamageLeft = ( lpObj->m_AttackDamageMinLeft + SkillLeftMinDamage + lpObj->MasterCharacterInfo->IncMinDmg) + (rand()%(sub+1));
					else
						lpObj->m_AttackDamageLeft = ( lpObj->m_AttackDamageMinLeft + SkillLeftMinDamage + lpObj->MasterCharacterInfo->IncMinDmg);
				} else {
					sub = ( lpObj->m_AttackDamageMaxLeft + SkillLeftMaxDamage ) - ( lpObj->m_AttackDamageMinLeft + SkillLeftMinDamage );
					if (sub > 0)
						lpObj->m_AttackDamageLeft = ( lpObj->m_AttackDamageMinLeft + SkillLeftMinDamage ) + (rand()%(sub+1));
					else
						lpObj->m_AttackDamageLeft = ( lpObj->m_AttackDamageMinLeft + SkillLeftMinDamage );
				}
			}
		}
		__except ( sub=1, 1 )
		{

		}

		if ( cDamage )
		{
			lpObj->m_AttackDamageRight = lpObj->m_AttackDamageMaxRight + SkillRightMaxDamage;
			lpObj->m_AttackDamageLeft = lpObj->m_AttackDamageMaxLeft + SkillLeftMaxDamage;

			if ( lpObj->Type == OBJ_USER )
			{
				lpObj->m_AttackDamageRight += lpObj->MasterCharacterInfo->IncMaxDmg + lpObj->MasterCharacterInfo->IncMaxDmgSkillPower;
				lpObj->m_AttackDamageLeft += lpObj->MasterCharacterInfo->IncMaxDmg + lpObj->MasterCharacterInfo->IncMaxDmgSkillPower;
			}

			if ( MsgDamage%MSG_DAMAGE_DOUBLE == MSG_DAMAGE_CRITICAL ) //Critical Damage
			{
				lpObj->m_AttackDamageRight += lpObj->SetOpAddCriticalDamage;
				lpObj->m_AttackDamageLeft += lpObj->SetOpAddCriticalDamage;

				lpObj->m_AttackDamageRight += lpObj->m_JewelOfHarmonyEffect.HJOpAddCriticalDamage;
				lpObj->m_AttackDamageLeft += lpObj->m_JewelOfHarmonyEffect.HJOpAddCriticalDamage;

				lpObj->m_AttackDamageRight += lpObj->SkillAddCriticalDamage;
				lpObj->m_AttackDamageLeft += lpObj->SkillAddCriticalDamage;
			}
			
			if ( MsgDamage%MSG_DAMAGE_DOUBLE == MSG_DAMAGE_EXCELLENT )	//Excelent Damage
			{
				lpObj->m_AttackDamageRight += ( lpObj->m_AttackDamageMaxRight + SkillRightMaxDamage ) * 20 / 100;
				lpObj->m_AttackDamageLeft += ( lpObj->m_AttackDamageMaxLeft + SkillLeftMaxDamage ) * 20 / 100;

				lpObj->m_AttackDamageRight += lpObj->SetOpAddExDamage;
				lpObj->m_AttackDamageLeft += lpObj->SetOpAddExDamage;
			}

			if (( (MsgDamage > MSG_DAMAGE_BEAT) && (MsgDamage&MSG_DAMAGE_DOUBLE == MSG_DAMAGE_DOUBLE )) || (MsgDamage == MSG_DAMAGE_DOUBLE))	//Double Damage
			{
				//MsgDamage = effectTmp;
				lpObj->m_AttackDamageRight = lpObj->m_AttackDamageRight * 2;
				lpObj->m_AttackDamageLeft = lpObj->m_AttackDamageLeft * 2;
			}
		}
	}

	if ( bTwoHandWeapon )
	{
		if ( bIsOnDuel )
		{
			ad = (( lpObj->m_AttackDamageRight + lpObj->m_AttackDamageLeft ) * 60 / 100) - targetDefense;
		}
		else
		{
			ad = ( lpObj->m_AttackDamageRight + lpObj->m_AttackDamageLeft ) - targetDefense;
		}
	}
	else if ( (Left->m_Type >= ITEMGET(4, 0) && Left->m_Type < ITEMGET(4, 7)) ||
			 Left->m_Type == ITEMGET(4,20) || Left->m_Type == ITEMGET(4,21) || Left->m_Type == ITEMGET(4,22) || Left->m_Type == ITEMGET(4,23)  )
	{
		if ( bIsOnDuel )
		{
			ad = (( lpObj->m_AttackDamageLeft ) * 60 / 100) - targetDefense;
		}
		else
		{
			ad = lpObj->m_AttackDamageLeft - targetDefense;
		}
	}
	else if ( (Right->m_Type >= ITEMGET(4, 8) && Right->m_Type < ITEMGET(4, 15)) ||
			  ( Right->m_Type >= ITEMGET(4,16) && Right->m_Type < ITEMGET(5,0) )  )
	{
		if ( bIsOnDuel )
		{
			ad = (( lpObj->m_AttackDamageRight ) * 60 / 100) - targetDefense;
		}
		else
		{
			ad = lpObj->m_AttackDamageRight - targetDefense;
		}
	}
	else if ( Right->m_Type >= ITEMGET(0,0) && Right->m_Type < ITEMGET(4,0) )
	{
		if ( bIsOnDuel )
		{
			ad = (( lpObj->m_AttackDamageRight ) * 60 / 100) - targetDefense;
		}
		else
		{
			ad = lpObj->m_AttackDamageRight - targetDefense;
		}
	}
	else if ( Right->m_Type >= ITEMGET(5,0) && Right->m_Type < ITEMGET(6,0) )
	{
		if ( bIsOnDuel )
		{
			ad = (( lpObj->m_AttackDamageRight ) * 60 / 100) - targetDefense;
		}
		else
		{
			ad = lpObj->m_AttackDamageRight - targetDefense;
		}
	}
	else if ( bIsOnDuel )
	{
		ad = (( lpObj->m_AttackDamageLeft ) * 60 / 100) - targetDefense;
	}
	else
	{
		ad = lpObj->m_AttackDamageLeft - targetDefense;
	}

	if ( lpObj->SetOpTwoHandSwordImproveDamage )
	{
		ad += ad * lpObj->SetOpTwoHandSwordImproveDamage  / 100;
	}

	ad += lpObj->m_SkillAttack;

	if(lpObj->m_SkillReduceDamage > 0)
	{
		ad -= lpObj->m_SkillReduceDamage;
		if ( ad < 0 )
			ad = 0;
	}
	
	if ( lpObj->m_SkillAttack2 )
	{
		ad += 15;
	}

	if ( lpObj->m_MonsterSkillElementInfo.m_iSkillElementAttackTime > 0 )
	{
		ad += lpObj->m_MonsterSkillElementInfo.m_iSkillElementAttack;

		if ( ad < 0 )
		{
			ad = 0;
		}
	}

	return ad;
}





int  CObjAttack::GetAttackDamageWizard(LPOBJ lpObj, int targetDefense, CMagicInf* lpMagic, BYTE& effect, BOOL bIsOnDuel)
{
	if ( g_ShieldSystemOn == TRUE )
	{
		if ( bIsOnDuel == TRUE )
		{
			bIsOnDuel = FALSE;
		}
	}

	int damagemin;
	int damagemax;
	int ad;

	if ( lpMagic->m_Skill == AT_SKILL_EXPHELL )
	{
		if ( lpObj->SkillHellFire2Count >= 0 )
		{
			int SkillHellFire2CountDamageTable[13] =
			{
				0,	20,	50,	99,	160,
				225,	325,	425,	550,	700,
				880,	1090,	1320
			};
			int CountDamage;

			if ( lpObj->SkillHellFire2Count > 12 )
			{
				CountDamage = 0;
			}
			else
			{
				CountDamage = SkillHellFire2CountDamageTable[lpObj->SkillHellFire2Count];
			}

			ad = ( lpObj->Strength + lpObj->AddStrength ) / 2 + CountDamage;
			damagemin = ad + lpObj->m_MagicDamageMin;
			damagemax = ad + lpObj->m_MagicDamageMax;

			damagemin += lpObj->SetOpAddSkillAttack;
			damagemax += lpObj->SetOpAddSkillAttack;

			int SkillAttr = MagicDamageC.GetSkillResistance(lpMagic->m_Skill);

			if ( SkillAttr != -1 )
			{
				damagemin += (BYTE)lpObj->m_AddResistance[SkillAttr];
				damagemax += (BYTE)lpObj->m_AddResistance[SkillAttr];
			}

			damagemin += lpObj->m_JewelOfHarmonyEffect.HJOpAddSkillAttack;
			damagemax += lpObj->m_JewelOfHarmonyEffect.HJOpAddSkillAttack;
		}
	}
	else
	{
		ad = lpMagic->GetDamage();

		damagemin = lpMagic->m_DamageMin + lpObj->m_MagicDamageMin;
		damagemax = lpMagic->m_DamageMax + lpObj->m_MagicDamageMax;

		damagemin += lpObj->SetOpAddSkillAttack;
		damagemax += lpObj->SetOpAddSkillAttack;

		int SkillAttr = MagicDamageC.GetSkillResistance(lpMagic->m_Skill);

		if ( SkillAttr != -1 )
		{
			damagemin += (BYTE)lpObj->m_AddResistance[SkillAttr];
			damagemax += (BYTE)lpObj->m_AddResistance[SkillAttr];
		}

		damagemin += lpObj->m_JewelOfHarmonyEffect.HJOpAddSkillAttack;
		damagemax += lpObj->m_JewelOfHarmonyEffect.HJOpAddSkillAttack;
	}

	CItem * Right = &lpObj->pInventory[0];

	if ( Right->IsItem() )
	{
		if ( (Right->m_Type >= ITEMGET(5,0) && Right->m_Type < ITEMGET(6,0) ) ||
			 Right->m_Type == ITEMGET(0,31) ||
			 Right->m_Type == ITEMGET(0,21) ||
			 Right->m_Type == ITEMGET(0,23) ||
			 Right->m_Type == ITEMGET(0,25) )
		{
			if ( Right->m_IsValidItem  )
			{
				int damage = Right->m_Magic / 2 + Right->m_Level * 2;	// #formula
				damage -= (WORD)(Right->m_CurrentDurabilityState * damage);	// #formula

				damagemin += damagemin * damage / 100;	// #formula
				damagemax += damagemax * damage / 100;	// #formula
			}
		}
	}

	int subd = damagemax - damagemin;

	__try
	{
		if ( bIsOnDuel )
		{
			ad = ( damagemin + (rand()%(subd+1)) ) * 60 / 100 - targetDefense;	// #formula
		}
		else
		{
			ad = ( damagemin + (rand()%(subd+1)) ) - targetDefense;
		}

		if ( lpObj->m_CriticalDamage > 0 )
		{
			if ( (rand()%100) < lpObj->m_CriticalDamage )
			{
				if ( bIsOnDuel )
				{
					ad = damagemax * 60 / 100 - targetDefense;
				}
				else
				{
					ad = damagemax - targetDefense;
				}

				ad += lpObj->SetOpAddCriticalDamage;
				ad += lpObj->m_JewelOfHarmonyEffect.HJOpAddCriticalDamage;
				ad += lpObj->SkillAddCriticalDamage;
				effect = 3;
			}
		}

		if ( lpObj->m_ExcelentDamage > 0 )
		{
			if ( (rand()%100) < lpObj->m_ExcelentDamage )
			{
				if ( bIsOnDuel )
				{
					ad = damagemax * 60 / 100 - targetDefense;
				}
				else
				{
					ad = damagemax - targetDefense;
				}

				ad += damagemax * 20 / 100;
				ad += lpObj->SetOpAddExDamage;
				effect = 2;
			}
		}

		/*if ( lpObj->Class == CLASS_KNIGHT )	//BK NEVER GETS IN TO THIS FUNCTION
		{
			if(lpObj->MasterCharacterInfo->SpearDoubleDmg > 0)
			{
				CItem * Right = &lpObj->pInventory[0];
				if ( Right->IsItem() == TRUE )
				{
					if(Right->m_Type >= ITEMGET(4,0) && Right->m_Type < ITEMGET(5,0))
					{
						if ( (rand()%100) < lpObj->MasterCharacterInfo->SpearDoubleDmg )
						{							
							if ( bIsOnDuel )
							{
								ad = damagemax * 60 / 100 - targetDefense;
							}
							else
							{
								ad = damagemax - targetDefense;
							}

							ad = ad * 2;
						}
					}
				}
			}
		}*/

		if ( lpObj->Class == CLASS_RAGEFIGHTER )
		{
			if(lpObj->MasterCharacterInfo->SpearDoubleDmg > 0)
			{
				CItem * Right = &lpObj->pInventory[0];
				CItem * Left  = &lpObj->pInventory[1];
				bool enter = false;
				if ( Right->IsItem() == TRUE )
				{
					if(Right->m_Type >= ITEMGET(0,32) && Right->m_Type <= ITEMGET(0,35))
					{
						if ( (rand()%100) < lpObj->MasterCharacterInfo->SpearDoubleDmg )
						{		
							if ( bIsOnDuel )
							{
								ad = damagemax * 60 / 100 - targetDefense;
							}
							else
							{
								ad = damagemax - targetDefense;
							}

							ad = ad * 2;
							enter = true;
						}
					}
				}
				if(enter == false)
				{
					if ( Left->IsItem() == TRUE )
					{
						if(Left->m_Type >= ITEMGET(0,32) && Left->m_Type <= ITEMGET(0,35))
						{
							if ( (rand()%100) < lpObj->MasterCharacterInfo->SpearDoubleDmg )
							{		
								if ( bIsOnDuel )
								{
									ad = damagemax * 60 / 100 - targetDefense;
								}
								else
								{
									ad = damagemax - targetDefense;
								}

								ad = ad * 2;
							}
						}
					}
				}
			}
		}
	}
	__except ( subd=1, 1 )
	{

	}

	ad += lpObj->m_SkillAttack;

	if(lpObj->m_SkillReduceDamage > 0)
	{
		ad -= lpObj->m_SkillReduceDamage;
		if ( ad < 0 )
			ad = 0;
	}

	if ( lpObj->m_SkillAttack2 )
	{
		ad += 10;
	}

	return ad;
}




BOOL gObjDenorantSprite(LPOBJ lpObj)
{
	if ( lpObj->Type != OBJ_USER )
	{
		return FALSE;
	}

	if ( lpObj->pInventory[8].IsItem() == FALSE )
	{
		return FALSE;
	}

	CItem * Denorant = & lpObj->pInventory[8];

	if ( Denorant->m_Type == ITEMGET(13,3) )
	{
		if ( Denorant->m_Durability > 0.0f )
		{
			return TRUE;
		}
	}

	return FALSE;
}




BOOL gObjDarkHorse(LPOBJ lpObj)
{
	if ( lpObj->Type != OBJ_USER )
	{
		return FALSE;
	}

	if ( lpObj->pInventory[8].IsItem() == FALSE )
	{
		return FALSE;
	}

	CItem * Denorant = & lpObj->pInventory[8];

	if ( Denorant->m_Type == ITEMGET(13,4) )
	{
		if ( Denorant->m_Durability > 0.0f )
		{
			return TRUE;
		}
	}

	return FALSE;
}



BOOL gObjFenrir(LPOBJ lpObj)
{
	if ( lpObj->Type != OBJ_USER )
	{
		return FALSE;
	}

	if ( lpObj->pInventory[8].IsItem() == FALSE )
	{
		return FALSE;
	}

	CItem * lpFenrir = & lpObj->pInventory[8];

	if ( lpFenrir->m_Type == ITEMGET(13,37) )	// Fenrir
	{
		if ( lpFenrir->m_Durability > 0.0f )
		{
			return TRUE;
		}
	}

	return FALSE;
}


int CObjAttack::GetShieldDamage(LPOBJ lpObj, LPOBJ lpTargetObj, int iAttackDamage)
{
	if ( g_ShieldSystemOn == FALSE )
		return 0;

	if ( iAttackDamage <= 0 )
		return 0;

	int iReduceLife = 0;
	int iReduceShield = 0;
	int iReduceLifeForEffect = 0; 
	bool bReduceShieldGage = 0;
	int iDamageDevideToSDRate = g_iDamageDevideToSDRate;
	iDamageDevideToSDRate -= lpObj->m_JewelOfHarmonyEffect.HJOpDecreaseSDRate;
	iDamageDevideToSDRate += lpTargetObj->m_JewelOfHarmonyEffect.HJOpAddSDRate;

	if ( iDamageDevideToSDRate < 0 )
		iDamageDevideToSDRate = 0;

	if ( iDamageDevideToSDRate > 100 )
		iDamageDevideToSDRate = 100;

	if ( lpObj->m_JewelOfHarmonyEffect.HJOpAddIgnoreSDRate > 0 )
	{
		int iRand = rand()%100;

		if ( iRand < lpObj->m_JewelOfHarmonyEffect.HJOpAddIgnoreSDRate )
		{
			iDamageDevideToSDRate = 0;
		}
	}

	if ( lpObj->m_JewelOfHarmonyEffect.HJOpDecreaseSDRate ||
		 lpTargetObj->m_JewelOfHarmonyEffect.HJOpAddSDRate ||
		 lpObj->m_JewelOfHarmonyEffect.HJOpAddIgnoreSDRate )
	{
		LogAddTD("[JewelOfHarmony][PvP System] Attacker:[%s][%s]-SD Decrease[%d] SD Ignore[%d] Defender:[%s][%s] SD Increase Option[%d] - SD Rate[%d]",
			lpObj->AccountID, lpObj->Name,
			lpObj->m_JewelOfHarmonyEffect.HJOpDecreaseSDRate,
			lpObj->m_JewelOfHarmonyEffect.HJOpAddIgnoreSDRate,
			lpTargetObj->AccountID, lpTargetObj->Name,
			lpTargetObj->m_JewelOfHarmonyEffect.HJOpAddSDRate,
			iDamageDevideToSDRate);
	}

	iReduceShield = iAttackDamage * iDamageDevideToSDRate / 100;
	iReduceLife = iAttackDamage - iReduceShield;

	if ( (lpTargetObj->iShield-iReduceShield) <0 )
	{
		iReduceLife += iReduceShield  - lpTargetObj->iShield;
		iReduceShield = lpTargetObj->iShield;

		if ( lpTargetObj->iShield > 0 )
		{
			bReduceShieldGage = true;
		}
	}

	iReduceLifeForEffect = ( lpTargetObj->MaxLife + lpTargetObj->AddLife ) * 20.0f / 100.0f;

	if ( bReduceShieldGage == true && iReduceLife > iReduceLifeForEffect )
	{
		if ( !CC_MAP_RANGE(lpTargetObj->MapNumber) )
		{
			GCSendEffectInfo(lpTargetObj->m_Index, 17);
		}
	}

	return iReduceShield;
}
