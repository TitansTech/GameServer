#include "stdafx.h"
#include "ObjBaseAttack.h"
#include "ObjAttack.h"
#include "GameMain.h"
#include "LogProc.h"
#include "DevilSquare.h"
#include "BloodCastle.h"
#include "ChaosCastle.h"
#include "DuelManager.h"
#include "SCFPvPSystem.h"
#include "CastleSiege.h"
#include "IllusionTemple.h"
#include "CustomWings.h"

#include "CrywolfAltar.h"
// GS-N 0.99.60T 0x00499B40
//	GS-N	1.00.18	JPN	0x004B5E50	-	Completed

#include "LogToFile.h"
extern CLogToFile ANTI_HACK_LOG;	// line : 68

CObjBaseAttack::CObjBaseAttack()
{
	return;
}


CObjBaseAttack::~CObjBaseAttack()
{
	return;
}


BOOL CObjBaseAttack::DecreaseArrow(LPOBJ lpObj)
{
	if ( lpObj->Class == CLASS_ELF && lpObj->Type == OBJ_USER )
	{
		CItem * Right = &lpObj->pInventory[0];
		CItem * Left  = &lpObj->pInventory[1];

		if ( (Right->m_Type >= ITEMGET(4,8) && Right->m_Type <= ITEMGET(4,14)) ||
			  Right->m_Type == ITEMGET(4,16) || Right->m_Type == ITEMGET(4,18) ||
			  Right->m_Type == ITEMGET(4,19) )
		{
			if ( Left->m_Type == ITEMGET(4,7) )
			{
				if ( Left->m_Durability < 1.0f )
				{
					return FALSE;
				}

				Left->m_Durability -= 1.0f;
				GCItemDurSend(lpObj->m_Index, 1, Left->m_Durability, 0);

				if ( Left->m_Durability < 1.0f )
				{
					lpObj->pInventory[1].Clear();
					GCInventoryItemDeleteSend(lpObj->m_Index, 1, 0);
				}
			}
			else
			{
				return FALSE;
			}
		}
		else if ( (Left->m_Type >= ITEMGET(4,0) && Left->m_Type <= ITEMGET(4,6) ) ||
				   Left->m_Type == ITEMGET(4,17) ||
				   Left->m_Type == ITEMGET(4,20) ||
				   Left->m_Type == ITEMGET(4,21) ||
				   Left->m_Type == ITEMGET(4,22) ||
				   Left->m_Type == ITEMGET(4,23) ||
				   Left->m_Type == ITEMGET(4,24) )
		{
			if ( Right->m_Type == ITEMGET(4,15) )
			{
				if ( Right->m_Durability < 1.0f )
				{
					return FALSE;
				}

				Right->m_Durability -= 1.0f;
				GCItemDurSend(lpObj->m_Index, 0, Right->m_Durability, 0);

				if ( Right->m_Durability < 1.0f )
				{
					lpObj->pInventory[0].Clear();
					GCInventoryItemDeleteSend(lpObj->m_Index, 0, 0);
				}
			}
			else
			{
				return FALSE;
			}
		}
	}

	return TRUE;
}



BOOL CObjBaseAttack::CheckAttackArea(LPOBJ lpObj, LPOBJ lpTargetObj)
{
#if (GS_CASTLE==1)
	if(lpTargetObj->Class == 277 && g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE)			//Castle Gate
	{
		return TRUE;
	}
	if(lpTargetObj->Class == 283 && g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE)	//Guardian Statue
	{
		return TRUE;
	}
#endif

	if ( lpTargetObj->Type == OBJ_USER || lpTargetObj->m_RecallMon >= 0 )
	{
		BYTE attr = MapC[lpTargetObj->MapNumber].GetAttr(lpTargetObj->X, lpTargetObj->Y);

		if (( (attr&1) == 1 ) && ( BC_MAP_RANGE(lpObj->MapNumber) == FALSE)) 
		{
			if ( lpObj->Type == OBJ_USER )
			{
				if(ReadConfig.AHLog == TRUE)
				{
					ANTI_HACK_LOG.Output("[%s][%s] Try Attack In Not Attack Area (%d,%d,%d) attr = %d",
						lpObj->AccountID, lpObj->Name, 
						lpObj->MapNumber, lpObj->X, lpObj->Y, attr);
				}
				GCServerMsgStringSend("[Anti-Hack] Attack In Non-Attack Area!",lpObj->m_Index, 0x01);
			}
			return FALSE;
		}
	}

	if ( lpObj->Type == OBJ_USER )
	{
		int iRet = gObjCheckAttackArea(lpObj->m_Index, lpTargetObj->m_Index);
		if ( iRet != 0 )
		{
			if ((lpObj->m_Index >= OBJ_STARTUSERINDEX)&&(lpObj->m_Index <= OBJMAX))
			{
				/*
				if(ReadConfig.AHLog == TRUE)
				{
					ANTI_HACK_LOG.Output("[%s][%s] Try Attack In Not Attack Area (%d,%d,%d) errortype = %d",
						lpObj->AccountID, lpObj->Name, lpObj->MapNumber,
						lpObj->X, lpObj->Y, iRet);
				}
				GCServerMsgStringSend("Monster has dodged the attack!",lpObj->m_Index, 0x01);*/
			}

			return FALSE;
		}
	}

	return TRUE;
}


bool CheckUnionGuild(LPOBJ lpObj, LPOBJ lpTargetObj)
{	
	if(gObjIsConnected(lpObj->m_Index) == false || gObjIsConnected(lpTargetObj->m_Index) == false)
	{
		return false;
	}

	if ( lpObj->GuildNumber == lpTargetObj->GuildNumber )
	{
		return true;
	}

	if ( lpObj->lpGuild == NULL )
	{
		return false;
	}

	if ( lpObj->lpGuild->iGuildUnion == 0 )
	{
		return false;
	}

	if ( lpTargetObj->lpGuild == NULL )
	{
		return false;
	}

	if ( lpTargetObj->lpGuild->iGuildUnion == 0 )
	{
		return false;
	}

	if ( lpObj->lpGuild->iGuildUnion == lpTargetObj->GuildNumber )
	{
		return true;
	}

	if ( lpTargetObj->lpGuild->iGuildUnion == lpObj->GuildNumber )
	{
		return true;
	}

	if ( lpObj->lpGuild->iGuildUnion == lpTargetObj->lpGuild->iGuildUnion )
	{
		return true;
	}

	return false;
}


BOOL CObjBaseAttack::PkCheck(LPOBJ lpObj, LPOBJ lpTargetObj)
{
	if ( lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER)
	{
#if (GS_CASTLE==1)
		//CryWolf Elf Altar Fix from PK abuse
		if ( CRYWOLF_MAP_RANGE(lpObj->MapNumber) != FALSE )
		{
			if ( lpTargetObj->Class == CLASS_ELF ) // Elf
			{
				for ( int iAltar = 205 ; iAltar <= 209 ; iAltar ++ )
				{
					int iUserIndex = g_CrywolfNPC_Altar.GetAltarUserIndex(iAltar);

					if ( iUserIndex == lpTargetObj->m_Index )
					{
						LogAddTD("[PVP][Crywolf][%d] User:[%s][%s] try kill Altar owner:[%s][%s]",
							iAltar, 
							lpObj->AccountID, lpObj->Name, 
							lpTargetObj->AccountID, lpTargetObj->Name
						);
						return FALSE;
					}
				}
			}
		}
		if( ReadConfig.CSFix_Same_Team_FIX == 1)
		{
			if ( lpObj->MapNumber == MAP_INDEX_CASTLESIEGE )
			{
				if(g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE) //Good
				{
					if(CheckUnionGuild(lpObj,lpTargetObj) == true)
						return FALSE;
				}
			}
		}
#endif	
		if(gObjItsInSafeZone(lpTargetObj->m_Index) == 1)
		{
			return FALSE;
		}

		if ( lpTargetObj->Level <= 5 || lpObj->Level <= 5 )
		{
			return FALSE;
		}

		if ( gObjTargetGuildWarCheck(lpObj, lpTargetObj) == FALSE && lpTargetObj->lpGuild != NULL && lpTargetObj->lpGuild->WarState != 0)
		{
			if ( lpTargetObj->lpGuild->WarType == 1 )
			{
				if ( lpTargetObj->MapNumber != 6 )
				{					
					if(PvP.Check(lpTargetObj->MapNumber) == FALSE)
					{
						return TRUE;
					}
				}
			}

			if ( CC_MAP_RANGE(lpTargetObj->MapNumber) == FALSE )
			{
				return FALSE;
			}
		}

		if ( IT_MAP_RANGE(lpObj->MapNumber) != FALSE ) // DEvil
		{
			if (lpObj->m_Change != lpTargetObj->m_Change)
			{
				return TRUE;
			} else {
				
				LogAddTD("[Illusion Temple][Skill] %s(%d) using skill %d on teammate",
					lpObj->Name, lpObj->Class, lpObj->UseMagicNumber

				);

				return FALSE;
			}
		}

		if ( CC_MAP_RANGE(lpObj->MapNumber) != FALSE )
		{
			int CCRest = MAP_INDEX_CHAOSCASTLE1;
			if (lpObj->MapNumber == MAP_INDEX_CHAOSCASTLE7)
				CCRest = 47;
			if ( g_ChaosCastle.GetCurrentState(lpObj->MapNumber-CCRest) != 2 )
			{
				return FALSE;
			} else {
				return TRUE;
			}
		}
		

		if ( gObjGetRelationShip(lpObj, lpTargetObj) == 2 ) // Rivals
		{
			if(PvP.Check(lpObj->MapNumber) == FALSE)
			{
				return TRUE;
			}
		}

		if ( lpObj->lpGuild != NULL && lpTargetObj->lpGuild != NULL )
		{
			if ( lpObj->lpGuild->WarState == 1 && lpTargetObj->lpGuild->WarState == 1 )
			{
				if ( lpObj->lpGuild->Number == lpTargetObj->lpGuild->Number )
				{
					return FALSE;
				}
			}
		}

		if(PvP.Check(lpObj->MapNumber) != FALSE)
		{
			return FALSE;
		}

		if(PvP.GetType(lpObj->MapNumber) == 4)
		{
			if((lpObj->GensFam != 0 && lpTargetObj->GensFam != 0) && (lpObj->GensFam == lpTargetObj->GensFam))
			{
				return FALSE;
			}else
			{
			}
		}

		/*
		if ( DS_MAP_RANGE(lpObj->MapNumber) ) // DEvil
		{
			return FALSE;
		}

		if ( BC_MAP_RANGE(lpObj->MapNumber) )
		{
			return FALSE;
		}*/
		
		if ( g_DuelManager.gObjDuelCheck(lpObj, lpTargetObj) == FALSE )
		{
			if ( g_DuelManager.gObjDuelCheck(lpTargetObj) )
			{
				return FALSE;
			}
		}

		if (lpObj->m_PK_Level >= 6 )
		{
			if ( PvP.gPkLimitFree == FALSE )
			{
				return FALSE;
			}
		}
			
		if (lpTargetObj->m_PK_Level >= 6 )
		{
			if ( PvP.gPkLimitFree == FALSE )
			{
				return FALSE;
			}
		}
	}
	return TRUE;
}





BOOL CObjBaseAttack::ResistanceCheck(LPOBJ lpObj, LPOBJ lpTargetObj, int skill)
{
	if (lpTargetObj->Type != OBJ_USER)
	{
		//Castle Siege NPC and Objects
		if (lpTargetObj->Class == 277 || 
			lpTargetObj->Class == 283 || //Guardian Statue
			lpTargetObj->Class == 285 || //Guardian
			lpTargetObj->Class == 288 || 
			lpTargetObj->Class == 278 || 
			lpTargetObj->Class == 215 || 
			lpTargetObj->Class == 216 || 
			lpTargetObj->Class == 217 || 
			lpTargetObj->Class == 218 || 
			lpTargetObj->Class == 219)
		{
			return FALSE;
		}

		//CryWolf Altars
		if ((lpTargetObj->Class >= 205)&&(lpTargetObj->Class <= 209))
			return FALSE;

		//CryWolf Wolf Statue
		if (lpTargetObj->Class == 204)
			return FALSE;

		//Imperial Statues, Gates
		if ((lpTargetObj->Class >= 524)&&(lpTargetObj->Class <= 528))
			return FALSE;
	}

	int resistanceType = MagicDamageC.GetSkillResistance(skill);
	if ( resistanceType != -1 )
	{
		if ( retResistance(lpTargetObj, resistanceType) > 0 )
		{
			return FALSE;
		}
	}

	if (skill == AT_SKILL_FALLINGSLASH || 
		skill == AT_SKILL_LUNGE || 
		skill == AT_SKILL_UPPERCUT || 
		skill == AT_SKILL_CYCLONE || 
		skill == AT_SKILL_SLASH)
	{
		gObjAddMsgSendDelay(lpTargetObj, 2, lpObj->m_Index, 150, 0);
	}
	else if(skill == AT_SKILL_THUNDER || 
			skill == AT_SKILL_CHAIN_LIGHTING || 
			skill == AT_SKILL_ELECTRIC_SURGE || 
			skill == AT_SKILL_LIGHTING_STORM || 
			(skill >= 540 && skill <= 549))	// Lighting
	{
		//if ( retResistance(lpTargetObj, R_LIGHTNING) == 0 )
		//{
			gObjAddMsgSendDelay(lpTargetObj, 2, lpObj->m_Index, 150, 0);
			return TRUE;
		//}

		//return FALSE;
	}
	else if (skill == AT_SKILL_DARKHORSE_ATTACK || 
			(skill >= 515 && skill <= 519) )
	{
		gObjBackSpring2(lpTargetObj, lpObj, 3);

#if (GS_CASTLE==1)
		if(lpObj->MapNumber == MAP_INDEX_CASTLESIEGE)
		{
			if (ReadConfig.DarkHorseMovePlayerInCS == 0)
			{
				//Horse Skill on DL does not PUSH people away from the attacker
				gObjAddMsgSendDelay(lpTargetObj, 2, lpObj->m_Index, 150, 0); 
				return TRUE;
			}
		}
#endif
	}
	else if ( skill == AT_SKILL_ELECTRICSPARK)	// Lighting
	{
		/*if ( retResistance(lpTargetObj, R_LIGHTNING) == 0 )
		{
		}else
		{
			return FALSE;
		}*/
	}
	else if ( skill == AT_SKILL_POISON )	//Poison
	{
		if ( lpTargetObj->m_PoisonType == 0 )
		{
			//if ( retResistance(lpTargetObj, R_POISON) == 0 )
			//{
				lpTargetObj->m_PoisonType = AT_SKILL_POISON;
				lpTargetObj->m_PoisonAdd = 0;
				lpTargetObj->m_PoisonBeattackCount = 20;
				lpTargetObj->lpAttackObj = lpObj;
				lpTargetObj->m_ViewSkillState |= 1;
			//}
			//else
			//{
			//	return FALSE;
			//}	
		}
		else
		{
			return FALSE;
		}
	}
	else if ( skill == AT_SKILL_EXPPOISON ) // Death Poison
	{
		if ( lpTargetObj->m_PoisonType == 0 || lpTargetObj->m_PoisonType == 1 )
		{
			//if ( retResistance(lpTargetObj, R_POISON) == 0 )
			//{
				lpTargetObj->m_PoisonType = AT_SKILL_EXPPOISON;
				lpTargetObj->m_PoisonBeattackCount = 10;
				lpTargetObj->lpAttackObj = lpObj;
				lpTargetObj->m_ViewSkillState |= 1;

				GCSkillInfoSend(lpTargetObj, 1, 0x37);
			//}
			//else
			//{
			//	return FALSE;
			//}
		}
		else
		{
			return FALSE;
		}

		return TRUE;
	}
	else if( skill == AT_SKILL_SLOW || 
			 skill == AT_SKILL_EXPICE ||
			 skill == AT_SKILL_CHAINDRIVE ||
			(skill >= 450 && skill <= 454))
	{
		if (lpTargetObj->m_ColdBeattackCount == 0 )
		{
			//if ( retResistance(lpTargetObj, R_ICE) == 0 )
			//{
				lpTargetObj->m_ColdBeattackCount = 10;
				lpTargetObj->lpAttackObj = lpObj;
				lpTargetObj->DelayActionTime = 800;
				lpTargetObj->DelayLevel = 1;
				lpTargetObj->m_ViewSkillState |= 2;

				if ( skill == AT_SKILL_CHAINDRIVE ||
					(skill >= 450 && skill <= 454))
					lpTargetObj->m_IceType = AT_SKILL_EXPICE;
				else
					lpTargetObj->m_IceType = skill;

				if ( skill == AT_SKILL_EXPICE || 
					 skill == AT_SKILL_CHAINDRIVE || 
					(skill >= 450 && skill <= 454))
				{
					GCSkillInfoSend(lpTargetObj, 1, 0x38);
				}
			//}
			//else
			//{
			//	return FALSE;
			//}
		}
		else
		{
			return FALSE;
		}
	}
	else if ( skill == AT_SKILL_ICEARROW ) // Ice Arrow
	{
		if ( lpTargetObj->m_SkillHardenTime <= 0 )
		{
			//if ( retResistance(lpTargetObj, R_ICE) == 0 )
			//{
				//Do nothing
				//Implemented in ObjAttack.cpp
			//}
			//else
			//{
			//	return FALSE;
			//}
		}
		else
		{
			return FALSE;
		}
	}
	else if (  skill == AT_SKILL_METEO
			|| skill == AT_SKILL_FIREBALL
			|| skill == AT_SKILL_FLAME
			|| skill == AT_SKILL_HELL
			|| skill == AT_SKILL_INFERNO
			|| skill == AT_SKILL_KNIGHTDINORANT
			|| skill == AT_SKILL_FIRESLASH
			|| skill == AT_SKILL_SPACE_SPLIT
			|| skill == AT_SKILL_FIRESCREAM
			|| skill == AT_SKILL_SWORD_SLASH
			|| (skill >= 440 && skill <= 444)
			|| (skill >= 500 && skill <= 504)
			|| (skill >= 520 && skill <= 529))
	{
		//if ( retResistance(lpTargetObj, R_FIRE) == 0 )
		//{
			//Do nothing
		//}else
		//{
		//	return FALSE;
		//}
	}
	else if (  skill == AT_SKILL_STORM
			|| skill == AT_SKILL_DEATHSTAB
			|| (skill >= 460 && skill <= 464))
	{
		//if ( retResistance(lpTargetObj, R_WIND) == 0 )
		//{
			//Do nothing
		//}else
		//{
		//	return FALSE;
		//}
	}
	else if (skill == AT_SKILL_BLOWOFFURY || 
			(skill >= 465 && skill <= 469))
	{
		//if ( retResistance(lpTargetObj, R_EARTH) == 0 )
		//{
			//Do nothing
		//}else
		//{
		//	return FALSE;
		//}
	}
	else if ( skill == AT_SKILL_FLASH)
	{
		//if ( retResistance(lpTargetObj, R_WATER) == 0 )
		//{
			//Do nothing
		//}else
		//{
		//	return FALSE;
		//}
	}

	return TRUE;
}





BOOL CObjBaseAttack::MissCheck(LPOBJ lpObj, LPOBJ lpTargetObj, int skill, int skillSuccess, int magicsend, BOOL& bAllMiss)
{
	__try
	{
		int iAttackRate = 0;	// lc20
		int iDefenseRate = lpTargetObj->m_SuccessfulBlocking;	// lc24
		//int iMSBDamage = 0;	// MonsterSetBasse Damage MISS

		if ( lpTargetObj->m_SkillIT_Time > 0 )
		{
			if ( lpTargetObj->m_SkillIT_Number == IL_ORDER_OF_PROTECT )
			{
				bAllMiss = TRUE;

				//GCDamageSend(lpObj->m_Index, lpTargetObj->m_Index, 0, 0, iMSBDamage, 0);
				GCDamageSend(lpObj->m_Index, lpTargetObj->m_Index, 0, 0, MSG_DAMAGE_MISS, 0);
				skillSuccess = 0;

				if (magicsend != 0 )
				{
					GCMagicAttackNumberSend(lpObj, skill, lpTargetObj->m_Index, skillSuccess);
				}

				return FALSE;
			}
		}

		if ( lpObj->Type == OBJ_USER )	// Miss for Users
		{
			if ( lpObj->Class == CLASS_DARKLORD ) // DL
			{
				iAttackRate = lpObj->Level * 5 + ((lpObj->Dexterity + lpObj->AddDexterity) * 6) / 2 + ( lpObj->Strength + lpObj->AddStrength) / 4  + ((lpObj->Leadership + lpObj->AddLeadership)/10);
				iAttackRate += iAttackRate * lpObj->SetOpImproveSuccessAttackRate / 100;
			}
			else if ( lpObj->Class == CLASS_RAGEFIGHTER ) // RF
			{
				iAttackRate = lpObj->Level * 5 + ((lpObj->Dexterity + lpObj->AddDexterity) * 2.5f) + ( lpObj->Vitality + lpObj->Vitality) / 6  + ((lpObj->Energy + lpObj->AddEnergy)/4);
				iAttackRate += iAttackRate * lpObj->SetOpImproveSuccessAttackRate / 100;
			}
			else
			{
				iAttackRate = lpObj->Level * 5 + ((lpObj->Dexterity + lpObj->AddDexterity) * 3) / 2 + ( lpObj->Strength + lpObj->AddStrength) / 4 ;
				iAttackRate += iAttackRate * lpObj->SetOpImproveSuccessAttackRate / 100;
			}
			iAttackRate += lpObj->MasterCharacterInfo->AttackSucRate;
		}
		else	// Miss for Monsters
		{
			iAttackRate = lpObj->m_AttackRating;
		}

		if ( lpTargetObj->Type == OBJ_USER )	// Miss for Uses
		{
			if(lpTargetObj->m_SkillDefSuccessRateIncTime > 0)
			{
				iDefenseRate += lpTargetObj->m_SkillDefSuccessRateIncNum;
			}
			iDefenseRate += (iDefenseRate * (lpTargetObj->MasterCharacterInfo->DefenseSucRate / 100));
		}


		if ( iAttackRate < iDefenseRate )
		{
			bAllMiss = TRUE;
		}

		if ( bAllMiss != FALSE )	// When All Miss
		{
			if ( (rand()%100) >= 5 )
			{
				//GCDamageSend(lpObj->m_Index, lpTargetObj->m_Index, 0, 0, iMSBDamage, 0);
				GCDamageSend(lpObj->m_Index, lpTargetObj->m_Index, 0, 0, MSG_DAMAGE_MISS, 0);

				if (magicsend != 0 )
				{
					GCMagicAttackNumberSend(lpObj, skill, lpTargetObj->m_Index, skillSuccess);
				}

				return FALSE;
			}
		}
		else	// if the is a chance  ot hit the target
		{
			if ( (rand()%iAttackRate) < iDefenseRate)
			{
				//GCDamageSend(lpObj->m_Index, lpTargetObj->m_Index, 0, 0, iMSBDamage, 0);
				GCDamageSend(lpObj->m_Index, lpTargetObj->m_Index, 0, 0, MSG_DAMAGE_MISS, 0);

				if ( magicsend != 0 )
				{
					GCMagicAttackNumberSend(lpObj, skill, lpTargetObj->m_Index,  skillSuccess);
				}

				return FALSE;
			}
		}
	}
	__except (lpObj->Level=1,-1)
	{
		LogAdd(lMsg.Get(MSGGET(2, 12)), lpObj->Name);
		return FALSE;
	}

	return TRUE;
}




BOOL CObjBaseAttack::MissCheckPvP(LPOBJ lpObj , LPOBJ lpTargetObj, int skill, int skillSuccess, int magicsend, BOOL & bAllMiss)
{
	float iAttackRate = 0;
	float iDefenseRate = 0;
	int iAttackSuccessRate = 0;

	if ( lpTargetObj->m_SkillIT_Time > 0 )
	{
		if ( lpTargetObj->m_SkillIT_Number == IL_ORDER_OF_PROTECT )
		{
			bAllMiss = TRUE;

			GCDamageSend(lpObj->m_Index, lpTargetObj->m_Index, 0, 0, MSG_DAMAGE_MISS, 0);

			if ( g_bShieldComboMissOptionOn == TRUE )
			{
				if ( lpObj->comboSkill.ProgressIndex >= 0 )
				{
					LogAddTD("[Shield] ComboSkill Cancel! [%s][%s]",
						lpObj->AccountID, lpObj->Name);

					lpObj->comboSkill.Init();
				}
			}

			return FALSE;
		}
	}

	if ( lpObj->Class == CLASS_KNIGHT )
	{
		iAttackRate = lpObj->Level * 3 + ( lpObj->Dexterity * 45) / 10;	// #formula
	}
	else if ( lpObj->Class == CLASS_DARKLORD )
	{
		iAttackRate = lpObj->Level * 3 + ( lpObj->Dexterity * 4 );	// #formula
	}
	else if ( lpObj->Class == CLASS_ELF )
	{
		iAttackRate = lpObj->Level * 3 + ( lpObj->Dexterity * 6 ) / 10;	// #formula
	}
	else if ( lpObj->Class == CLASS_RAGEFIGHTER )
	{
		iAttackRate = lpObj->Level * 3 + ( lpObj->Dexterity * 4 );	// #formula
	}
	else if ( lpObj->Class == CLASS_MAGICGLADIATOR )
	{
		iAttackRate = lpObj->Level * 3 + ( lpObj->Dexterity * 35 ) / 10;	// #formula
	}
	else if ( lpObj->Class == CLASS_WIZARD )
	{
		iAttackRate = lpObj->Level * 3 + ( lpObj->Dexterity * 4 );	// #formula
	}
	else if ( lpObj->Class == CLASS_SUMMONER )
	{
		iAttackRate = lpObj->Level * 3 + ( lpObj->Dexterity * 4 );	// #formula
	}

	if (lpObj->Class == CLASS_KNIGHT || lpObj->Class == CLASS_MAGICGLADIATOR)
	{
		if(ReadConfig.S6E2 == 1)
		{
			CItem * Right = &lpObj->pInventory[0];
			if ( Right->IsItem() == TRUE )
			{
				if((Right->m_Type >= ITEMGET(0,0)) && (Right->m_Type < ITEMGET(1,0)))
				{
					if(Right->m_TwoHand == 1)
					{
						iAttackRate += lpObj->MasterCharacterInfo->IncPhysicalAttackPowerTwoHandPvP;
					}
				}
			}
		}
	}

	if (lpObj->Class == CLASS_ELF)
	{
		if(ReadConfig.S6E2 == 1)
		{
			CItem * Right = &lpObj->pInventory[0];
			if ( Right->IsItem() == TRUE )
			{
				if((Right->m_Type >= ITEMGET(4,0) && Right->m_Type < ITEMGET(5,0)) && Right->m_Type != ITEMGET(4,15))
				{
					iAttackRate += lpObj->MasterCharacterInfo->IncPhysicalAttackPowerTwoHandPvP;
				}
			}
		}
	}

	if (lpObj->Class == CLASS_WIZARD || lpObj->Class == CLASS_MAGICGLADIATOR || lpObj->Class == CLASS_SUMMONER)
	{
		if(ReadConfig.S6E2 == 1)
		{
			CItem * Right = &lpObj->pInventory[0];
			if ( Right->IsItem() == TRUE )
			{
				if((Right->m_Type >= ITEMGET(5,0)) && (Right->m_Type < ITEMGET(6,0)))
				{
					if(Right->m_TwoHand == 1)
					{
						iAttackRate += lpObj->MasterCharacterInfo->IncStaffPvPAttackTwoHand;
					}
				}
			}
		}
	}

	if (lpObj->Class == CLASS_DARKLORD)
	{
		if(ReadConfig.S6E2 == 1)
		{
			CItem * Right = &lpObj->pInventory[0];
			if ( Right->IsItem() == TRUE )
			{
				if((Right->m_Type >= ITEMGET(2,8)) && (Right->m_Type < ITEMGET(3,0)))
				{
					iAttackRate += lpObj->MasterCharacterInfo->IncStaffPvPAttackTwoHand;
				}
			}
		}
	}

	if ( lpTargetObj->Class == CLASS_KNIGHT )
	{
		iDefenseRate = lpTargetObj->Level * 2 + lpTargetObj->Dexterity / 2;	// #formula
	}
	else if ( lpTargetObj->Class == CLASS_DARKLORD )
	{
		iDefenseRate = lpTargetObj->Level * 2 + lpTargetObj->Dexterity / 2;	// #formula
	}
	else if ( lpTargetObj->Class == CLASS_ELF )
	{
		iDefenseRate = lpTargetObj->Level * 2 + lpTargetObj->Dexterity / 10;	// #formula
	}
	else if ( lpTargetObj->Class == CLASS_RAGEFIGHTER )
	{
		iDefenseRate = lpTargetObj->Level * 2 + lpTargetObj->Dexterity / 8;	// #formula
	}
	else if ( lpTargetObj->Class == CLASS_MAGICGLADIATOR )
	{
		iDefenseRate = lpTargetObj->Level * 2 + lpTargetObj->Dexterity / 4;	// #formula
	}
	else if ( lpTargetObj->Class == CLASS_WIZARD )
	{
		iDefenseRate = lpTargetObj->Level * 2 + lpTargetObj->Dexterity / 4;	// #formula
	}
	else if ( lpTargetObj->Class == CLASS_SUMMONER )
	{
		iDefenseRate = lpTargetObj->Level * 2 + lpTargetObj->Dexterity / 4;	// #formula
	}

	if ( iAttackRate <= 0.0f || iDefenseRate <= 0.0f || lpObj->Level <= 0 || (DWORD)lpTargetObj <= 0 )	// #error lpTargetObj
		return FALSE;

	iAttackRate += lpObj->m_ItemOptionExFor380.OpAddAttackSuccessRatePVP;
	iDefenseRate += lpTargetObj->m_ItemOptionExFor380.OpAddDefenseSuccessRatePvP;

	iAttackRate += lpObj->m_JewelOfHarmonyEffect.HJOpAddAttackSuccessRatePVP;
	iDefenseRate += lpTargetObj->m_JewelOfHarmonyEffect.HJOpAddDefenseSuccessRatePvP;

	iAttackRate += lpObj->MasterCharacterInfo->AttackSucRatePVP;
	iDefenseRate += lpObj->MasterCharacterInfo->DefenseSucRatePVP;

	float iExpressionA = ( iAttackRate * 10000.0f ) / ( iAttackRate + iDefenseRate );	// #formula
	float iExpressionB = ( lpObj->Level * 10000 ) / ( lpObj->Level + lpTargetObj->Level );	// #formula

	iExpressionA /= 10000.0f;
	iExpressionB /= 10000.0f;

	iAttackSuccessRate = 100.0f * iExpressionA * g_fSuccessAttackRateOption * iExpressionB;

	if ( (lpTargetObj->Level - lpObj->Level) >= 100 )
	{
		iAttackSuccessRate -= 5;
	}
	else if ( (lpTargetObj->Level - lpObj->Level) >= 200 )
	{
		iAttackSuccessRate -= 10;
	}
	else if ( (lpTargetObj->Level - lpObj->Level) >= 300 )
	{
		iAttackSuccessRate -= 15;
	}

	DWORD dwRate = rand() % 100;

	if ( dwRate > iAttackSuccessRate )
	{
		GCDamageSend(lpObj->m_Index, lpTargetObj->m_Index, 0, 0, MSG_DAMAGE_MISS, 0);

		if ( g_bShieldComboMissOptionOn == TRUE )
		{
			if ( lpObj->comboSkill.ProgressIndex >= 0 )
			{
				LogAddTD("[Shield] ComboSkill Cancel! [%s][%s]",
					lpObj->AccountID, lpObj->Name);

				lpObj->comboSkill.Init();
			}
		}

		return FALSE;
	}

	return TRUE;
}





int  CObjBaseAttack::GetTargetDefense(LPOBJ lpObj, LPOBJ lpTargetObj, unsigned char& MsgDamage)
{
	int skilldefense = lpTargetObj->m_SkillDefense;
	int skillReducedefense = lpTargetObj->m_SkillReduceDefense + lpTargetObj->m_RFBufReduceDefense;
	int targetdefense = lpTargetObj->m_Defense + skilldefense - skillReducedefense;

	if(lpObj->Type == OBJ_USER)
	{
		if(lpObj->m_SkillRedArmorIgnoreTime > 0)
		{
			targetdefense = (targetdefense * (100 - lpObj->m_SkillRedArmorIgnoreNum)/100);
		}
	}


	if ( lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER )
	{
		targetdefense += lpTargetObj->m_ItemOptionExFor380.OpAddDefense / 2;
	}

	targetdefense -= (targetdefense * lpTargetObj->m_SkillMagumReduceDefense)/100;
	
	if ( lpObj->m_MonsterSkillElementInfo.m_iSkillElementDefenseTime > 0 )
	{
		targetdefense += lpObj->m_MonsterSkillElementInfo.m_iSkillElementDefense;

		if ( targetdefense <0 )
			targetdefense = 0;

	}

	if(targetdefense < 0) 
		targetdefense = 0;

	int percentdamage = 0;

	if ( lpObj->pInventory[7].IsItem() != FALSE && 
		((lpObj->pInventory[7].m_Type >= ITEMGET(12,0) && lpObj->pInventory[7].m_Type <= ITEMGET(12,6)) || 
		(lpObj->pInventory[7].m_Type >= ITEMGET(12,130) && lpObj->pInventory[7].m_Type <= ITEMGET(12,135)) || 
		lpObj->pInventory[7].m_Type == ITEMGET(13,30) || 
		lpObj->pInventory[7].m_Type == ITEMGET(12,49) || 
		lpObj->pInventory[7].m_Type == ITEMGET(12,41) || 
		lpObj->pInventory[7].m_Type == ITEMGET(12,42)))
	{
		percentdamage = lpObj->pInventory[7].IsWingOpGetOnePercentDamage();
	}

	//3rd Wings Ignore Opponents Deffence / Successful Blocking
	if ( lpObj->pInventory[7].IsItem() != FALSE && 
		((lpObj->pInventory[7].m_Type >= ITEMGET(12,36) && lpObj->pInventory[7].m_Type <= ITEMGET(12,40)) || 
#if (CRYSTAL_EDITION == 1)
		(lpObj->pInventory[7].m_Type >= ITEMGET(12,200) && lpObj->pInventory[7].m_Type <= ITEMGET(12,254)) || 
#endif
		lpObj->pInventory[7].m_Type == ITEMGET(12,43) || lpObj->pInventory[7].m_Type == ITEMGET(12,50)) && 
		lpObj->pInventory[7].CheckThirdWingsSuccessfulBlocking() ) //Third Wings
	{
		percentdamage = g_iWings3SuccessfullBlockingRate;
#if (CRYSTAL_EDITION == 1)
		if(lpObj->pInventory[7].m_Type >= ITEMGET(12,200) && lpObj->pInventory[7].m_Type <= ITEMGET(12,254))
			percentdamage = CWings.GetExcOption(lpObj->pInventory[7].m_Type,4);
#endif
	}

	percentdamage += lpObj->SetOpIgnoreDefense;

	if ( percentdamage != 0)
	{
		if ( (rand()%100) <= percentdamage)
		{
			targetdefense = 0;
			MsgDamage = MSG_DAMAGE_REGULAR;
		}
	}

	return targetdefense;
}







int  CObjBaseAttack::GetPartyMemberCount(LPOBJ lpObj)
{
	LPOBJ lpPartyObj;
	int partynum = lpObj->PartyNumber;

	if ( OBJMAX_RANGE(partynum) == FALSE )
	{
		LogAdd("error : DecPartyMemberHPandMP %s %d", __FILE__, __LINE__);
		return 0;
	}

	//int partycount = gParty.m_PartyS[partynum].Count;
	int retcount = 0;

	for ( int n=0;n<MAX_USER_IN_PARTY;n++)
	{
		int memberindex = gParty.m_PartyS[partynum].Number[n];

		if ( memberindex >= 0 )
		{
			lpPartyObj = &gObj[memberindex];

			if ( lpObj->MapNumber == lpPartyObj->MapNumber )
			{
				int dis = gObjCalDistance(lpObj, &gObj[memberindex]);
				
				if ( dis < MAX_PARTY_DISTANCE_EFFECT )
				{
					retcount++;
				}
			}
		}
	}

	return retcount;
}
		


