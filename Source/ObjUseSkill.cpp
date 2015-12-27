#include "stdafx.h"
#include "ObjUseSkill.h"
#include "ObjCalCharacter.h"
#include "ObjAttack.h"

#include "GameMain.h"
#include "ChaosCastle.h"
#include "GameServer.h"
//#include "GameServerAuth.h"
#include "user.h"
#include "zzzmathlib.h"
#include "SkillHitBox.h"
#include "MoveCommand.h"
#include "SkillAdditionInfo.h"
#include "DevilSquare.h"
#include "SCFPvPSystem.h"

#include "CrywolfAltar.h"

#include "LogProc.h"
#include "..\common\winutil.h"
// GS-N 0.99.60T 0x0049AE30
//	GS-N	1.00.18	JPN	0x004B7770 - Completed

CObjUseSkill gObjUseSkill;

#include "LogToFile.h"
extern CLogToFile ANTI_HACK_LOG;	// line : 68


CObjUseSkill::CObjUseSkill()
{
	return ;
}




CObjUseSkill::~CObjUseSkill()
{
	return;
}



int  CObjUseSkill::GetTargetLinePath(int sx, int sy, int tx, int ty, int * PathX, int * PathY, int distance)
{
	int x,y;
	int delta_x,delta_y;
	int half;

	int error = 0;

	int incx = 1,incy = 1;
	int dis;
	int count;

	if(sx > tx)
	{
		incx = -1;
		delta_x = sx - tx;
	}
	else
	{
		delta_x = tx - sx;
	}

	if(sy > ty)
	{
		incy = -1;
		delta_y = sy - ty;
	}
	else
	{
		delta_y = ty - sy;
	}

	dis = sqrt((float)(delta_x * delta_x + delta_y * delta_y));
	dis = distance - dis;

	count = 0;
	PathX[count] = sx;
	PathY[count] = sy;
	count++;

	if(delta_x > delta_y)
	{
		y = sy;
		half = delta_x / 2;

		if(incx > 0)
		{
			for(x = (sx+1); x <= (tx + dis); x++)
			{
				error += delta_y;

				if(error > half)
				{
					y += incy;
					error -= delta_x;
				}

				PathX[count] = x;
				PathY[count] = y;

				count++;
			}
		}
		else
		{
			for(x = (sx-1); x >= (tx - dis);x--)
			{
				error += delta_y;

				if(error > half)
				{
					y += incy;
					error -= delta_x;
				}

				PathX[count] = x;
				PathY[count] = y;

				count++;
			}
		}
	}
	else
	{
		x = sx;
		half = delta_y / 2;

		if(incy > 0)
		{
			for(y = (sy+1); y <= (ty + dis);y++)
			{
				error += delta_x;

				if(error > half)
				{
					x += incx;
					error -= delta_y;
				}

				PathX[count] = x;
				PathY[count] = y;
				count++;
			}
		}
		else
		{
			for(y = (sy-1);y >= (ty - dis);y--)
			{
				error += delta_x;

				if(error > half)
				{
					x += incx;
					error -= delta_y;
				}

				PathX[count] = x;
				PathY[count] = y;
				count++;
			}
		}
	}
	return count;
}


BOOL CObjUseSkill::SpeedHackCheck(int aIndex, BYTE functionID)
{
	BOOL result = TRUE;

	if (bIsIgnorePacketSpeedHackDetect == 1)
		return result;

	if (OBJMON_RANGE(aIndex))
		return result;

	LPOBJ lpObj = &gObj[aIndex];

	int emptyHandsMultiplier = 1;
	int iTimeCalc = GetTickCount() - lpObj->m_LastAttackTime;
	lpObj->m_LastAttackTime = GetTickCount();

	/*LogAddTD("[%s][%s][%d] ATTACK Last:%d Calc:%d DetT:%d ORG:[%d,%d] SPD:[%d,%d]",
		lpObj->AccountID, lpObj->Name, functionID, 
		lpObj->m_LastAttackTime, iTimeCalc, lpObj->m_DetectSpeedHackTime,
		(int)( lpObj->m_MagicSpeed / gDecTimePerAttackSpeed ),
		(int)( lpObj->m_AttackSpeed / gDecTimePerAttackSpeed ),
		lpObj->m_AttackSpeed,lpObj->m_MagicSpeed);*/

	if ( lpObj->HaveWeaponInHand == 0)
	{
		emptyHandsMultiplier = 2;
		iTimeCalc = emptyHandsMultiplier * iTimeCalc;
	}

	if ( iTimeCalc == 0 )
	{
		lpObj->m_ZeroTimeAttackCount++;

		if (lpObj->m_ZeroTimeAttackCount>3)
		{
			lpObj->m_ZeroTimeAttackCount=0;

			//GCServerMsgStringSend("Hit ignored, speed is too fast",lpObj->m_Index,1);
			return false;
		}
		return true;
	}

	if( (iTimeCalc < lpObj->m_DetectSpeedHackTime) && (iTimeCalc != 0) )
	{
		lpObj->m_DetectCount++;
		
		if(lpObj->m_DetectCount > gHackCheckCount)
		{
			lpObj->m_DetectedHackKickCount++;

			GCServerMsgStringSend("[Anti-Hack] Attack Speed Is Wrong!",aIndex, 0x01);

			if(ReadConfig.AHLog == TRUE)
			{
				ANTI_HACK_LOG.Output("[%s][%s][%d] USR:[C:%d,S:%d,D:%d,E:%d,V:%d,L:%d] Wrong Attack Speed Calc:%d(%d) Det:%d SPD:[%d,%d] Count:%d,%d Has Weapon:%d",
					lpObj->AccountID,lpObj->Name,functionID,
					lpObj->Class, lpObj->Strength, lpObj->Dexterity, lpObj->Energy, lpObj->Vitality, lpObj->Leadership,
					iTimeCalc, (iTimeCalc/emptyHandsMultiplier), lpObj->m_DetectSpeedHackTime,
					lpObj->m_AttackSpeed,lpObj->m_MagicSpeed,
					lpObj->m_DetectCount,lpObj->m_DetectedHackKickCount,
					lpObj->HaveWeaponInHand);
			}
			result = false;
		}
	}
	else
	{
		if (lpObj->m_ZeroTimeAttackCount > 0)
			lpObj->m_ZeroTimeAttackCount -= 1;

		if (lpObj->m_DetectCount > 0)
			lpObj->m_DetectCount -= 1;
	}

	if(gIsKickDetecHackCountLimit != 0 && lpObj->m_DetectedHackKickCount > gDetectedHackKickCount)
	{
		GCServerMsgStringSend("[Anti-Hack] Kick User = DetectHackCountLimit!",aIndex, 0x01);

		if(ReadConfig.AHLog == TRUE)
		{
			ANTI_HACK_LOG.Output("[%s][%s][%d] %s Kick DetectHackCountLimit Over User (%d)",
				lpObj->AccountID,lpObj->Name,functionID,lMsg.Get(lpObj->Class+1900),lpObj->m_DetectedHackKickCount);
		}
		CloseClient(aIndex);
		result = false;
	}

	return result;
}


BOOL CObjUseSkill::EnableSkill(WORD Skill)
{
	if(	Skill != 150
		&& Skill != AT_SKILL_STORM
		&& Skill != AT_SKILL_EVIL
		&& Skill != AT_SKILL_FLAME
		&& Skill != AT_SKILL_HELL
		&& Skill != AT_SKILL_BLAST
		&& Skill != AT_SKILL_INFERNO
		&& Skill != AT_SKILL_CROSSBOW
		&& Skill != AT_SKILL_DEVILFIRE
		&& Skill != AT_SKILL_FLASH
		&& Skill != AT_SKILL_TWISTINGSLASH
		&& Skill != AT_SKILL_POWERSLASH
		&& Skill != AT_SKILL_BKIMPALE
		&& Skill != AT_SKILL_KNIGHTADDLIFE
		&& Skill != AT_SKILL_DEATHSTAB
		&& Skill != AT_SKILL_KNIGHTDINORANT
		&& Skill != AT_SKILL_ICEARROW
		&& Skill != AT_SKILL_PENETRATION
		&& Skill != AT_SKILL_FIRESLASH
		&& Skill != AT_SKILL_MAGICDEFENSE
		&& Skill != AT_SKILL_BLOWOFFURY
		&& Skill != AT_SKILL_EXPPOISON
		&& Skill != AT_SKILL_EXPICE
		&& Skill != AT_SKILL_EXPHELL
		&& Skill != AT_SKILL_ELECTRICSPARK
		&& Skill != AT_SKILL_DARKHORSE_ATTACK
		&& Skill != AT_SKILL_BRAND_OF_SKILL
		&& Skill != AT_SKILL_STUN
		&& Skill != AT_SKILL_REMOVAL_STUN
		&& Skill != AT_SKILL_ADD_MANA
		&& Skill != AT_SKILL_INVISIBLE 
		&& Skill != AT_SKILL_REMOVAL_INVISIBLE
		&& Skill != AT_SKILL_REMOVAL_MAGIC
		&& Skill != AT_SKILL_FENRIR_ATTACK
		&& Skill != AT_SKILL_INFINITY_ARROW
		&& Skill != AT_SKILL_FIRESCREAM		
		&& Skill != AT_SKILL_DRAIN_LIFE
		&& Skill != AT_SKILL_CHAIN_LIGHTING
		&& Skill != AT_SKILL_ELECTRIC_SURGE
		&& Skill != AT_SKILL_REFLECT
		&& Skill != AT_SKILL_BERSERKER
		&& Skill != AT_SKILL_SLEEP
		&& Skill != AT_SKILL_NIGHT
		&& Skill != AT_SKILL_REDUCE_DAMAGE
		&& Skill != AT_SKILL_REDUCE_DEFENSE
		&& Skill != AT_SKILL_SAHAMUTT
		&& Skill != AT_SKILL_NAIL
		&& Skill != AT_SKILL_GHOST_PHANTOM
		&& Skill != AT_SKILL_RED_STORM
		&& Skill != AT_SKILL_EXPLOTION
		&& Skill != AT_SKILL_MAGIC_CIRCLE
		&& Skill != AT_SKILL_REDUCE_DMG_SHIELD
		&& Skill != AT_SKILL_FIVE_SHOT
		&& Skill != AT_SKILL_SWORD_SLASH
		&& Skill != AT_SKILL_LIGHTING_STORM
		&& Skill != AT_SKILL_BIRDS
		&& Skill != AT_SKILL_LARGERINGBLOWER
		&& Skill != AT_SKILL_UPPERBEAST
		&& Skill != AT_SKILL_CHAINDRIVE
		&& Skill != AT_SKILL_DARKSIDE
		&& Skill != AT_SKILL_DRAGONLORE
		&& Skill != AT_SKILL_DRAGONSLAYER
		&& Skill != AT_SKILL_REDADMORIGNORE
		&& Skill != AT_SKILL_FITNESS
		&& Skill != AT_SKILL_DEFENSESUCCESSRATEINCREASE
		&& Skill != AT_SKILL_CHARGE
		&& Skill != AT_SKILL_PHOENIX_G)
	{
		if(Skill >= 435 && Skill <= 554) //SkillTree DmgSkills
			return true;
		else
			return false;
	}
	return true;
}

int  CObjUseSkill::GetUseMana(int aIndex, CMagicInf * lpMagic)
{
	LPOBJ lpObj =&gObj[aIndex];
	float mana = lpObj->Mana;

	if ( lpObj->Type != OBJ_USER )
	{
		return 0;
	}

	if ( lpMagic == NULL )
	{
		return 0;
	}

	int usingmana = MagicDamageC.SkillGetMana(lpMagic->m_Skill);
	usingmana = usingmana * ((100 - lpObj->MasterCharacterInfo->DecManaReduction)/100.0f);
	mana -= usingmana;
	//mana -= this->GetAddUseMana(lpObj,lpMagic); OK

	if(mana < 0.0f)
	{
		return -1;
	}
	return usingmana;
}

int  CObjUseSkill::GetUseBP(int aIndex, CMagicInf * lpMagic)
{
	LPOBJ lpObj = &gObj[aIndex];

	int bp = lpObj->BP;

	if(lpObj->Type != OBJ_USER)
	{
		return 0;
	}

	if(lpMagic == 0)
	{
		return 0;
	}

	int usingbp = MagicDamageC.SkillGetBP(lpMagic->m_Skill);
	usingbp -= usingbp * lpObj->SetOpDecreaseAG / 100;
	bp -= usingbp;

	if(bp < 0)
	{
		return -1;
	}

	return usingbp;
}


struct PMSG_DURATION_MAGIC_SEND {

  // static data ------------------------------------

  // non-static data --------------------------------
  /*<thisrel this+0x0>*/ /*|0x3|*/ struct PBMSG_HEAD h;
	unsigned char MagicNumberH;
  /*<thisrel this+0x3>*/ /*|0x1|*/ unsigned char MagicNumberL;
  /*<thisrel this+0x4>*/ /*|0x1|*/ unsigned char NumberH;
  /*<thisrel this+0x5>*/ /*|0x1|*/ unsigned char NumberL;
  /*<thisrel this+0x6>*/ /*|0x1|*/ unsigned char X;
  /*<thisrel this+0x7>*/ /*|0x1|*/ unsigned char Y;
  /*<thisrel this+0x8>*/ /*|0x1|*/ unsigned char Dir;

  // base classes -----------------------------------

  // friends ----------------------------------------

  // static functions -------------------------------

  // non-virtual functions --------------------------

  // virtual functions ------------------------------
};
// <size 0x9>

void CObjUseSkill::UseSkill(int aIndex, CMagicInf * lpMagic, BYTE x, BYTE y, BYTE dir, BYTE TargetPos, int aTargetIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	int bCombo = 0;

	if(gComboAttack.CheckCombo(aIndex,lpMagic->m_Skill))
	{
		bCombo = 1;
	}

	if(lpObj->SkillHellFire2State && lpMagic->m_Skill != AT_SKILL_EXPHELL)
	{
		CMagicInf * lpMagic = gObjGetMagicSearch(lpObj,AT_SKILL_EXPHELL);

		if(lpMagic != 0)
		{
			gObjUseSkill.RunningSkill(lpObj->m_Index,0,lpMagic,0);
		}

		return;
	}

	int usemana = this->GetUseMana(aIndex,lpMagic);

	PMSG_DURATION_MAGIC_SEND pMsg;

	if(usemana >= 0)
	{
		if(!gObj[aIndex].SkillDelay.Check(lpMagic->m_Skill))
		{
			LogAddTD("[%s][%s] Skill Delay is wrong! SKILL:%d",gObj[aIndex].AccountID,gObj[aIndex].Name,lpMagic->m_Skill);
			return;
		}

		//int usebp = gObjMagicBPUse(&gObj[aIndex],lpMagic);
		int usebp = this->GetUseBP(aIndex,lpMagic);

		if(usebp >= 0)
		{
			lpObj->UseMagicNumber = lpMagic->m_Skill;
			lpObj->UseMagicTime = GetTickCount();
			lpObj->UseMagicCount = 0;

			lpObj->Mana -= usemana;
			lpObj->BP -= usebp;

			GCManaSend(lpObj->m_Index,lpObj->Mana,0xFF,0,lpObj->BP);

			PHeadSetBE((PBYTE)&pMsg,0x1E,sizeof(pMsg));

			pMsg.MagicNumberH = SET_NUMBERH(lpMagic->m_Skill);
			pMsg.MagicNumberL = SET_NUMBERL(lpMagic->m_Skill);
			pMsg.X = x;
			pMsg.Y = y;
			pMsg.Dir = dir;
			pMsg.NumberH = SET_NUMBERH(aIndex);
			pMsg.NumberL = SET_NUMBERL(aIndex);
			//pMsg.UNK1 = 0x00;
			if(lpObj->Type == OBJ_USER)
			{
				DataSend(lpObj->m_Index,(PBYTE)&pMsg,pMsg.h.size);
			}

			if(CC_MAP_RANGE(lpObj->MapNumber))
			{
				switch(lpMagic->m_Skill)
				{
				case AT_SKILL_REFLECT:
				case AT_SKILL_MAGIC_CIRCLE:
				case AT_SKILL_REDUCE_DAMAGE:
				case AT_SKILL_REDUCE_DEFENSE:
				case 435:
				case 436:
				case 437:
				case 438:
				case 439:
				case AT_SKILL_MAGICDEFENSE:
				case 475:
				case 476:
				case 477:
				case 478:
				case 479:
				case AT_SKILL_HEALING:
				case 480:
				case 481:
				case 482:
				case 483:
				case 484:
				case AT_SKILL_DEFENSE:
				case 485:
				case 486:
				case 487:
				case 488:
				case 489:
				case AT_SKILL_ATTACK:
				case AT_SKILL_KNIGHTADDLIFE:
				case 470:
				case 471:
				case 472:
				case 473:
				case 474:
				case AT_SKILL_REDUCE_DMG_SHIELD:
					break;
				default: 
					MsgSendV2(lpObj,(PBYTE)&pMsg,pMsg.h.size);
				}
			}
			else
			{
				MsgSendV2(lpObj,(PBYTE)&pMsg,pMsg.h.size);
			}
		}

		if(lpMagic->m_Skill == AT_SKILL_PHOENIX_G)
		{
			this->SkillPhoenixShot(aIndex,aTargetIndex,lpMagic);
		}

		if(lpMagic->m_Skill == AT_SKILL_DRAGONLORE)
		{
			this->SkillDragonLore(aIndex,lpMagic,x,y,dir,TargetPos,aTargetIndex);
		}

		if(lpMagic->m_Skill == AT_SKILL_REDUCE_DAMAGE)
		{
			this->SkillReduceDamage(aIndex,aTargetIndex,lpMagic->m_Level);
		}

		if(lpMagic->m_Skill == AT_SKILL_REDUCE_DEFENSE)
		{
			this->SkillReduceDefense(aIndex,aTargetIndex,lpMagic->m_Level);
		}

		if((lpMagic->m_Skill == AT_SKILL_POWERSLASH) || (lpMagic->m_Skill >= 505 && lpMagic->m_Skill <= 509) )
		{
			this->SkillPowerSlash(aIndex,lpMagic,x,y,dir,TargetPos,aTargetIndex);
		}
		if(lpMagic->m_Skill == AT_SKILL_EXPPOISON)
		{
			this->SkillDeathPoison(aIndex,lpMagic,x,y,dir,TargetPos,aTargetIndex);
		}
		else if((lpMagic->m_Skill == AT_SKILL_EXPICE) || (lpMagic->m_Skill >= 450 && lpMagic->m_Skill <= 454) )
		{
			this->SkillSuddenIce(aIndex,lpMagic,x,y,dir,TargetPos,aTargetIndex);
		}
		else if((lpMagic->m_Skill == AT_SKILL_DRAIN_LIFE) || (lpMagic->m_Skill >= 550 && lpMagic->m_Skill <= 554) )
		{
			this->SkillDrainLife(aIndex,aTargetIndex,lpMagic);
		}
		else if((lpMagic->m_Skill == AT_SKILL_CHAIN_LIGHTING) || (lpMagic->m_Skill >= 540 && lpMagic->m_Skill <= 544) )
		{
			this->SkillChainElectricAttack(aIndex,aTargetIndex,lpMagic);
			//this->SkillCommon(aIndex,aTargetIndex,lpMagic);
		}	
		else if(lpMagic->m_Skill == AT_SKILL_ELECTRIC_SURGE)
		{
			this->SkillCommon(aIndex,aTargetIndex,lpMagic);
		}
		else if(lpMagic->m_Skill == AT_SKILL_SWORD_SLASH)
		{
			this->SkillSwordSlash(aIndex,lpMagic,x,y,dir,TargetPos,aTargetIndex);
		}
		else if((lpMagic->m_Skill == AT_SKILL_RED_STORM) || (lpMagic->m_Skill >= 545 && lpMagic->m_Skill <= 549) )
		{
			this->SkillRedStorm(aIndex,lpMagic,x,y,dir,TargetPos,aTargetIndex);
		}		
		else if(lpMagic->m_Skill == AT_SKILL_SAHAMUTT)
		{
			this->SkillSummonerBook(aIndex,lpMagic,x,y,dir,TargetPos,aTargetIndex,0x4B);
		}
		else if(lpMagic->m_Skill == AT_SKILL_NAIL)
		{
			this->SkillSummonerBook(aIndex,lpMagic,x,y,dir,TargetPos,aTargetIndex,0x4A);
		}
		else if(lpMagic->m_Skill == AT_SKILL_GHOST_PHANTOM)
		{
			this->SkillGhostPhantom(lpObj->m_Index,aTargetIndex,lpMagic);
		}
		else if(lpMagic->m_Skill == AT_SKILL_EXPLOTION)
		{
			this->SkillExplotion(aIndex,lpMagic,x,y,dir,TargetPos,aTargetIndex);
		}
		else if(lpMagic->m_Skill == AT_SKILL_LIGHTING_STORM)
		{
			this->SkillLightingStorm(aIndex,lpMagic,x,y,dir,TargetPos,aTargetIndex);
		}
		else if((lpMagic->m_Skill == AT_SKILL_BLOWOFFURY) || (lpMagic->m_Skill >= 465 && lpMagic->m_Skill <= 469) )
		{
			this->SkillBlowOfFury(aIndex,lpMagic,x,y,dir,TargetPos,aTargetIndex,bCombo);
		}
		else if((lpMagic->m_Skill == AT_SKILL_TWISTINGSLASH) || (lpMagic->m_Skill >= 455 && lpMagic->m_Skill <= 459) || (lpMagic->m_Skill >= 495 && lpMagic->m_Skill <= 499))
		{
			if((ReadConfig.TwistingSlashNoWeapon_CGDurationMagicRecv == 0) || (ReadConfig.TwistingSlashNoWeapon_CGDurationMagicRecv == 1 && gObj[aIndex].pInventory[0].IsItem() == TRUE))
				this->SkillWheel(aIndex,lpMagic,aTargetIndex,bCombo);
		}
		else if(lpMagic->m_Skill == AT_SKILL_ELECTRICSPARK)
		{
			this->SkillElectricSpark(aIndex,lpMagic,x,y,dir,TargetPos,aTargetIndex);
		}
		else if(lpMagic->m_Skill == AT_SKILL_BIRDS)
		{
			this->SkillBirds(aIndex,lpMagic,x,y,dir,TargetPos,aTargetIndex);
		}
		else if((lpMagic->m_Skill == AT_SKILL_DARKHORSE_ATTACK) || (lpMagic->m_Skill >= 515 && lpMagic->m_Skill <= 519) )
		{
			this->SkillDarkHorseAttack(lpObj->m_Index,aTargetIndex,lpMagic);
		}
		else if(lpMagic->m_Skill == AT_SKILL_STUN)
		{
			this->SkillStun(aIndex,aTargetIndex,lpMagic,x,y,dir,TargetPos);
		}
		else if(lpMagic->m_Skill == AT_SKILL_FENRIR_ATTACK)
		{
			this->SkillFenrirAttack(aIndex,aTargetIndex,lpMagic);
		}
	}
}




void CObjUseSkill::UseSkill(int aIndex, int aTargetIndex, CMagicInf * lpMagic)
{
	LPOBJ lpObj = &gObj[aIndex];

	int bCombo = 0;

	if(gComboAttack.CheckCombo(aIndex,lpMagic->m_Skill))
	{
		bCombo = 1;
	}

	if(lpObj->SkillHellFire2State && lpMagic->m_Skill != AT_SKILL_EXPHELL)
	{
		CMagicInf * lpMagic = gObjGetMagicSearch(lpObj,AT_SKILL_EXPHELL);

		if(lpMagic != 0)
		{
			gObjUseSkill.RunningSkill(lpObj->m_Index,0,lpMagic,0);
		}

		return;
	}

	if(lpMagic->m_Skill == AT_SKILL_EXPHELL && aTargetIndex != 58)
	{
		this->RunningSkill(aIndex,aTargetIndex,lpMagic,bCombo);
		lpObj->UseMagicNumber = lpMagic->m_Skill;
		lpObj->UseMagicTime = GetTickCount();
		lpObj->UseMagicCount = 0;
		return;
	}

	int usemana = this->GetUseMana(aIndex,lpMagic);

	if(usemana >= 0)
	{
		if(!gObj[aIndex].SkillDelay.Check(lpMagic->m_Skill))
		{
			LogAddTD("[%] Wrong Skill delay",gObj[aIndex].AccountID);
			return;
		}

		int usebp = this->GetUseBP(aIndex,lpMagic);

		if(usebp >= 0)
		{
			if(this->RunningSkill(aIndex,aTargetIndex,lpMagic,bCombo)==1)
			{
				lpObj->UseMagicNumber = lpMagic->m_Skill;
				lpObj->UseMagicTime = GetTickCount();
				lpObj->UseMagicCount = 0;

				lpObj->Mana -= usemana;
				lpObj->BP -= usebp;

				if(lpObj->Mana < 0)
					lpObj->Mana = 0;
				if(lpObj->BP < 0)
					lpObj->BP = 0;

				GCManaSend(aIndex,lpObj->Mana,0xFF,0,lpObj->BP);
			}
		}
	}
}


int CObjUseSkill::SkillRedStorm(int aIndex, CMagicInf * lpMagic, BYTE x, BYTE y, BYTE dir, BYTE TargetPos, int aTargetIndex)
{
	LPOBJ lpObj = &gObj[aIndex];
	int tObjNum;
	int count = 0;
	int bAttack;
	int DuelIndex = lpObj->m_iDuelUser;
	int EnableAttack;

	while ( true )
	{
		if(lpObj->VpPlayer2[count].state != 0)
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if(tObjNum >= 0)
			{
				EnableAttack = 0; 
				
				if ( gclassObjAttack.PkCheck(lpObj, &gObj[tObjNum]) == FALSE )
				{
					EnableAttack = 0;
				}
				else if ( gclassObjAttack.CheckAttackArea(lpObj, &gObj[tObjNum]) == FALSE )
				{
					EnableAttack = 0;
				}
				else if(lpObj->VpPlayer2[count].type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon < 0)
				{
					EnableAttack = 1;
				}
				else if(tObjNum == aTargetIndex || DuelIndex == tObjNum)
				{
					EnableAttack = 1;
				}
				else
				{
					int CallMonIndex = gObj[tObjNum].m_Index;

					if(gObj[tObjNum].Type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon >= 0)
					{
						CallMonIndex = gObj[gObj[tObjNum].m_RecallMon].m_Index;
					}

					if(gObjTargetGuildWarCheck(lpObj,&gObj[CallMonIndex])==1)
					{
						EnableAttack = 1;
					}
				}

				if(EnableAttack != 0)
				{
					BYTE bAttackType = gObjAttackType(&gObj[tObjNum],lpObj);

					if (bAttackType > 0)
					{
						if(this->CalDistance(x,y,gObj[tObjNum].X,gObj[tObjNum].Y) < 5)
						{
							gObjAttack(lpObj,&gObj[tObjNum],lpMagic,1,1,0,0);
	
							if (bAttackType == 2)
							{
								gObj[tObjNum].m_SkillRedStormSender = aIndex;
								gObj[tObjNum].m_SkillRedStormTime = 5;
								gObj[tObjNum].m_SkillRedStormDmg = 75;
								GCSkillInfoSend(&gObj[tObjNum],1,0x55);
							}
						}
					}
				}
			}
		}

		count++;
		if(count > MAX_VIEWPORT -1)
		{
			break;
		}
	}
	return true;
}


int CObjUseSkill::SkillSummonerBook(int aIndex, CMagicInf * lpMagic, BYTE x, BYTE y, BYTE dir, BYTE TargetPos, int aTargetIndex, BYTE Book)
{
	LPOBJ lpObj = &gObj[aIndex];
	int tObjNum;
	int count = 0;
	int HitCount = 0;
	int bAttack;
	int DuelIndex = lpObj->m_iDuelUser;
	int EnableAttack;

	int BookDmg = 0;
	int HitCount1 = 0;
	int HitCount2 = 0;
	int Distance = 0;
	int NewDamage = 0;
	int Seconds = 0;

	if(Book == 0x4B)
	{
		//BookDmg = 90 * (lpMagic->m_Level + 1);
		BookDmg = (int)(lpObj->m_MagicDamageMin * ((lpMagic->m_Level+1) / ReadConfig.SkillSummonerBook_SahamutDiv));
		BookDmg += (BookDmg * lpObj->MasterCharacterInfo->IncFireBook2) /100;
		Distance = ReadConfig.SkillSummonerBook_SahamutDist;
		HitCount1 = ReadConfig.SkillSummonerBook_SahamutHit1;
		HitCount2 = ReadConfig.SkillSummonerBook_SahamutHit2;
		Seconds = ReadConfig.SkillSummonerBook_SahamutSec;
		NewDamage = ReadConfig.SkillSummonerBook_SahamutAddMagic * (lpMagic->m_Level + 1);
	}
	else if(Book == 0x4A)
	{
		//BookDmg = 90 * (lpMagic->m_Level + 1);
		BookDmg = (int)(lpObj->m_MagicDamageMin * ((lpMagic->m_Level+1) /  ReadConfig.SkillSummonerBook_NailDiv));
		Distance = ReadConfig.SkillSummonerBook_NailDist;
		HitCount1 = ReadConfig.SkillSummonerBook_NailHit1;
		HitCount2 = ReadConfig.SkillSummonerBook_NailHit2;
		Seconds = ReadConfig.SkillSummonerBook_NailSec;
		NewDamage = ReadConfig.SkillSummonerBook_NailAddMagic * (lpMagic->m_Level + 1);
	}
	BookDmg += gObj[aIndex].MasterCharacterInfo->IncWizardyTwoHand;
	BookDmg += gObj[aIndex].MasterCharacterInfo->IncMaxSkillPower;
	if(gObj[aIndex].m_SkillBerserkerTime > 0)
		BookDmg += (BookDmg * gObj[aIndex].MasterCharacterInfo->IncBerserker1)/100.0f;

	BookDmg += gObj[aIndex].MasterCharacterInfo->IncExpansionWizardy1;

	lpObj->m_MagicDamageMin+=NewDamage;
	lpObj->m_MagicDamageMax+=NewDamage;

	while ( true )
	{
		if(lpObj->VpPlayer2[count].state != 0)
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if(tObjNum >= 0)
			{
				EnableAttack = 0;
				
				if ( gclassObjAttack.PkCheck(lpObj, &gObj[tObjNum]) == FALSE )
				{
					EnableAttack = 0;
				}
				else if ( gclassObjAttack.CheckAttackArea(lpObj, &gObj[tObjNum]) == FALSE )
				{
					EnableAttack = 0;
				}
				else if(lpObj->VpPlayer2[count].type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon < 0)
				{
					EnableAttack = 1;
				}
				else if(tObjNum == aTargetIndex || DuelIndex == tObjNum)
				{
					EnableAttack = 1;
				}
				else
				{
					int CallMonIndex = gObj[tObjNum].m_Index;

					if(gObj[tObjNum].Type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon >= 0)
					{
						CallMonIndex = gObj[gObj[tObjNum].m_RecallMon].m_Index;
					}

					if(gObjTargetGuildWarCheck(lpObj,&gObj[CallMonIndex])==1)
					{
						EnableAttack = 1;
					}
				}

				if(EnableAttack != 0)
				{
					BYTE bAttackType = gObjAttackType(&gObj[tObjNum],lpObj);

					if (bAttackType > 0)
					{
						if(this->CalDistance(x,y,gObj[tObjNum].X,gObj[tObjNum].Y) <= Distance)
						{
							bAttack = 0;

							if(HitCount > HitCount1)
							{
								bAttack = 0;
							}
						
							if(HitCount >= HitCount2)
							{
								if(rand()%2)
								{
									bAttack = 1;
								}
							}
							else
							{
								bAttack = 1;
							}

							if(bAttack != 0)
							{
								gObjAttack(lpObj,&gObj[tObjNum],lpMagic,1,1,0,0);

								if (bAttackType == 2)
								{
									gObj[tObjNum].m_SkillSummonerBookSender = aIndex;
									gObj[tObjNum].m_SkillSummonerBookTime = Seconds;
									gObj[tObjNum].m_SkillSummonerBookTypeDmg = Book;

									if(gObj[tObjNum].Type == OBJ_MONSTER && gObj[tObjNum].Level < 100)
									{
										gObj[tObjNum].m_SkillSummonerBookDmg = (int)(BookDmg * (100.0f/gObj[tObjNum].Level));
									} else {
										gObj[tObjNum].m_SkillSummonerBookDmg = (int)BookDmg;
									}

									GCSkillInfoSend(&gObj[tObjNum],1,Book);
								}
							}
						}
					}
				}
			}
		}

		count++;
		if(count > MAX_VIEWPORT -1)
		{
			break;
		}
	}
	
	lpObj->m_MagicDamageMin-=NewDamage;
	lpObj->m_MagicDamageMax-=NewDamage;

	return true;
}

bool CObjUseSkill::SkillDragonLore(int aIndex, CMagicInf * lpMagic, BYTE x, BYTE y, BYTE dir, BYTE TargetPos, int aTargetIndex)
{
	LPOBJ lpObj = &gObj[aIndex];
	int tObjNum;
	int count = 0;
	int bAttack;
	int DuelIndex = lpObj->m_iDuelUser;
	int EnableAttack;
	int HitCount = 0;

	while ( true )
	{
		if(lpObj->VpPlayer2[count].state != 0)
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if(tObjNum >= 0)
			{
				if(HitCount > 10)
					break;

				EnableAttack = 0;
				
				if ( gclassObjAttack.PkCheck(lpObj, &gObj[tObjNum]) == FALSE )
				{
					EnableAttack = 0;
				}
				else if ( gclassObjAttack.CheckAttackArea(lpObj, &gObj[tObjNum]) == FALSE )
				{
					EnableAttack = 0;
				}
				else if(lpObj->VpPlayer2[count].type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon < 0)
				{
					EnableAttack = 1;
				}
				else if(tObjNum == aTargetIndex || DuelIndex == tObjNum)
				{
					EnableAttack = 1;
				}
				else
				{
					int CallMonIndex = gObj[tObjNum].m_Index;

					if(gObj[tObjNum].Type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon >= 0)
					{
						CallMonIndex = gObj[gObj[tObjNum].m_RecallMon].m_Index;
					}

					if(gObjTargetGuildWarCheck(lpObj,&gObj[CallMonIndex])==1)
					{
						EnableAttack = 1;
					}
				}

				if(EnableAttack != 0)
				{
					BYTE bAttackType = gObjAttackType(&gObj[tObjNum],lpObj);

					if (bAttackType > 0)
					{
						if(this->CalDistance(x,y,gObj[tObjNum].X,gObj[tObjNum].Y) < 5)
						{
							gObjAddAttackProcMsgSendDelay(lpObj,53,tObjNum,200,lpMagic->m_Skill,0);
							HitCount++;
						}
					}
				}
			}
		}

		count++;
		if(count > MAX_VIEWPORT -1)
		{
			break;
		}
	}
	return true;
}


bool CObjUseSkill::SkillLargeRingBlower(int aIndex, int aTargetIndex, CMagicInf * lpMagic)
{
	if(OBJMAX_RANGE(aIndex) == false)
	{
		return false;
	}
	if(OBJMAX_RANGE(aTargetIndex) == false)
	{
		return false;
	}

	LPOBJ lpObj = &gObj[aIndex];

	GCMagicAttackNumberSend(lpObj,lpMagic->m_Skill,aTargetIndex,1);

	int iDelay = 100;

	if(lpObj->MasterCharacterInfo->IncLargerRingBlowerDebuf > 0)
	{
		if ( (rand()%100) < 30 )
		{
			if(gObj[aTargetIndex].m_RFBufReduceDefenseTime <= 0)
			{
				GCSkillInfoSend(&gObj[aTargetIndex],1,164);
			}
			gObj[aTargetIndex].m_RFBufReduceDefense = lpObj->MasterCharacterInfo->IncLargerRingBlowerDebuf;
			gObj[aTargetIndex].m_RFBufReduceDefenseTime = 3;
		}
	}
	
	gObjAddAttackProcMsgSendDelay(lpObj,53,aTargetIndex,iDelay,lpMagic->m_Skill,0);
	gObjAddAttackProcMsgSendDelay(lpObj,53,aTargetIndex,iDelay+300,lpMagic->m_Skill,0);
	gObjAddAttackProcMsgSendDelay(lpObj,53,aTargetIndex,iDelay+500,lpMagic->m_Skill,0);
	gObjAddAttackProcMsgSendDelay(lpObj,53,aTargetIndex,iDelay+700,lpMagic->m_Skill,0);

	return true;
}

bool CObjUseSkill::SkillUpperBeast(int aIndex, int aTargetIndex, CMagicInf * lpMagic)
{
	LPOBJ lpObj = &gObj[aIndex];
	int tObjNum;
	int count = 0;
	int HitCount = 1;
	int bAttack;
	int DuelIndex = lpObj->m_iDuelUser;
	int EnableAttack;
	int iDelay = 100;

	if(OBJMAX_RANGE(aTargetIndex) == true)
	{
		if(lpObj->MasterCharacterInfo->IncUpperBeastDebuf > 0)
		{
			if ( (rand()%100) < 30 )
			{
				if(gObj[aTargetIndex].m_RFBufReduceDefenseTime <= 0)
				{
					GCSkillInfoSend(&gObj[aTargetIndex],1,164);
				}
				gObj[aTargetIndex].m_RFBufReduceDefense = lpObj->MasterCharacterInfo->IncUpperBeastDebuf;
				gObj[aTargetIndex].m_RFBufReduceDefenseTime = 3;
			}
		}
	}

	//Test Feature here (as oponent i cant see the hit, trying to fix)
	gObjAddAttackProcMsgSendDelay(lpObj,53,aTargetIndex,iDelay,lpMagic->m_Skill,0);
	gObjAddAttackProcMsgSendDelay(lpObj,53,aTargetIndex,iDelay+250,lpMagic->m_Skill,0);
	gObjAddAttackProcMsgSendDelay(lpObj,53,aTargetIndex,iDelay+400,lpMagic->m_Skill,0);
	//gObjAddAttackProcMsgSendDelay(lpObj,53,aTargetIndex,iDelay+550,lpMagic->m_Skill,0);
	gObjAttack(lpObj,&gObj[aTargetIndex],lpMagic,1,1,0,0);

	/*while( true )
	{
		if(lpObj->VpPlayer2[count].state != 0)
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if(tObjNum >= 0)
			{
				EnableAttack = 0;
				
				if ( gclassObjAttack.PkCheck(lpObj, &gObj[tObjNum]) == FALSE )
				{
					EnableAttack = 0;
				}
				else if ( gclassObjAttack.CheckAttackArea(lpObj, &gObj[tObjNum]) == FALSE )
				{
					EnableAttack = 0;
				}
				else if(lpObj->VpPlayer2[count].type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon < 0)
				{
					EnableAttack = 1;
				}
				else if(tObjNum == aTargetIndex || DuelIndex == tObjNum)
				{
					EnableAttack = 1;
				}
				else
				{
					int CallMonIndex = gObj[tObjNum].m_Index;

					if(gObj[tObjNum].Type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon >= 0)
					{
						CallMonIndex = gObj[gObj[tObjNum].m_RecallMon].m_Index;
					}
					if(gObjTargetGuildWarCheck(lpObj,&gObj[CallMonIndex]) == 1)
					{
						EnableAttack = 1;
					}
				}

				if(EnableAttack != 0)
				{
					BYTE bAttackType = gObjAttackType(&gObj[tObjNum],lpObj);

					if (bAttackType > 0)
					{
						if(gObjCalDistance(lpObj,&gObj[tObjNum]) < 4)
						{
							bAttack = 0;

							if(HitCount >= 3)
							{
								bAttack = 0;
							}
							else
							{
								bAttack = 1;
							}

							if(tObjNum == aTargetIndex)
							{
								gObjAddAttackProcMsgSendDelay(lpObj,53,aTargetIndex,iDelay,lpMagic->m_Skill,0);
								gObjAddAttackProcMsgSendDelay(lpObj,53,aTargetIndex,iDelay+400,lpMagic->m_Skill,0);
								gObjAddAttackProcMsgSendDelay(lpObj,53,aTargetIndex,iDelay+550,lpMagic->m_Skill,0);
								gObjAddAttackProcMsgSendDelay(lpObj,53,aTargetIndex,iDelay+700,lpMagic->m_Skill,0);
							}
							else
							{
								if(bAttack != 0)
								{
									gObjAddAttackProcMsgSendDelay(lpObj,53,tObjNum,300,lpMagic->m_Skill,0);
									HitCount++;
								}
							}
						}
					}
				}
			}
		}

		count++;

		if(count > MAX_VIEWPORT - 1)
		{
			break;
		}
	}*/

	GCMagicAttackNumberSend(lpObj,lpMagic->m_Skill,aTargetIndex,1);
	return true;
}


bool CObjUseSkill::SkillChainDrive(int aIndex, int aTargetIndex, CMagicInf * lpMagic)
{
	if(OBJMAX_RANGE(aIndex) == false)
	{
		return false;
	}
	if(OBJMAX_RANGE(aTargetIndex) == false)
	{
		return false;
	}
	if(gObjItsInSafeZone(aTargetIndex) == 1)
	{
		return false;
	}

	LPOBJ lpObj = &gObj[aIndex];

	GCMagicAttackNumberSend(lpObj,lpMagic->m_Skill,aTargetIndex,1);

	int iDelay = 100;

	int RandHit = rand()%4;

	for(int i=0;i<3;i++)
	{
		iDelay = iDelay + (i*150);
		gObjAddAttackProcMsgSendDelay(lpObj,53,aTargetIndex,iDelay,lpMagic->m_Skill,0);
	}
	
	if(RandHit > 0)
	{	
		for(int i=0;i<RandHit;i++)
		{
			iDelay = iDelay + (i*150);
			gObjAddAttackProcMsgSendDelay(lpObj,53,aTargetIndex,iDelay,lpMagic->m_Skill,0);
		}
	}

	gObjBackSpring2(&gObj[aTargetIndex], lpObj, 1);
	//Test
	gObjAttack(lpObj,&gObj[aTargetIndex],lpMagic,1,1,0,0);

	return true;
}


bool CObjUseSkill::SkillDragonSlayer(int aIndex, int aTargetIndex, CMagicInf * lpMagic)
{
	if(OBJMAX_RANGE(aIndex) == false)
	{
		return false;
	}

	LPOBJ lpObj = &gObj[aIndex];

	int TotalEnergy = (lpObj->Energy + lpObj->AddEnergy);

	//Dragon Slayer = 300+Dmg* 3*(50+Ene/10)/100
	lpObj->m_SkillDragonSlayerDmg = (float) (ReadConfig.SkillDragonSlayerDmgMult * ( ReadConfig.SkillDragonSlayerEneBase + TotalEnergy / (float)ReadConfig.SkillDragonSlayerEneDiv) / (float)ReadConfig.SkillDragonSlayerDmgDiv);
	if (lpObj->m_SkillDragonSlayerDmg > ReadConfig.SkillDragonSlayerAttMax)
		lpObj->m_SkillDragonSlayerDmg = ReadConfig.SkillDragonSlayerAttMax;

	lpObj->m_SkillDragonSlayerTime = ReadConfig.SkillDragonSlayerTimeBase + (lpObj->Energy + lpObj->AddEnergy) / ReadConfig.SkillDragonSlayerTimeDiv;
	if (lpObj->m_SkillDragonSlayerTime > (ReadConfig.SkillDragonSlayerTimeMinMax*60))
		lpObj->m_SkillDragonSlayerTime = ReadConfig.SkillDragonSlayerTimeMinMax*60;

	GCMagicAttackNumberSend(lpObj,lpMagic->m_Skill,aTargetIndex,1);
	gObjAttack(lpObj,&gObj[aTargetIndex],lpMagic,1,1,0,0);
	return true;
}



bool CObjUseSkill::SkillRedArmorIgnore(int aIndex, CMagicInf * lpMagic)
{
	if(OBJMAX_RANGE(aIndex) == false)
	{
		return false;
	}

	LPOBJ lpObj = &gObj[aIndex];

	GCSkillInfoSend(lpObj,1,129);
	GCMagicAttackNumberSend(lpObj,lpMagic->m_Skill,lpObj->m_Index,1);	//Fix Character Panel Visual

	int TotalEnergy = (lpObj->Energy + lpObj->AddEnergy);
	//float skill_RedArmorIgnore= (float) ((TotalEnergy / ReadConfig.SkillRedArmorIgnoreNumMult ) + 1.0f);

	//%PVP Defence Ignore 5+(Ene-120)/10
	lpObj->m_SkillRedArmorIgnoreNum = ReadConfig.SkillRedArmorIgnoreBase + (TotalEnergy-ReadConfig.SkillRedArmorIgnoreEneSubtract)/(float)ReadConfig.SkillRedArmorIgnoreDivider;
	if (lpObj->m_SkillRedArmorIgnoreNum > ReadConfig.SkillRedArmorIgnoreMaxPercent)
		lpObj->m_SkillRedArmorIgnoreNum = ReadConfig.SkillRedArmorIgnoreMaxPercent;
	if (lpObj->m_SkillRedArmorIgnoreNum < 0)
		lpObj->m_SkillRedArmorIgnoreNum = ReadConfig.SkillRedArmorIgnoreBase;

	lpObj->m_SkillRedArmorIgnoreTime = ReadConfig.SkillRedArmorIgnoreTimeBase + (lpObj->Energy + lpObj->AddEnergy) / ReadConfig.SkillRedArmorIgnoreTimeDiv;

	if (lpObj->m_SkillRedArmorIgnoreTime > (ReadConfig.SkillRedArmorIgnoreMinMax*60))
		lpObj->m_SkillRedArmorIgnoreTime = ReadConfig.SkillRedArmorIgnoreMinMax * 60;

	//if(lpObj->m_SkillRedArmorIgnoreTime > 0)
	//{		
	//	int number;
	//	int partynum = 0;

	//	partynum = lpObj->PartyNumber;

	//	int partycount;
	//	LPOBJ lpPartyObj;

	//	if(partynum != -1)
	//	{
	//		partycount = gParty.m_PartyS[partynum].Count;

	//		for(int n = 0; n < MAX_USER_IN_PARTY;n++)
	//		{
	//			number = gParty.m_PartyS[partynum].Number[n];

	//			if(number >= 0)
	//			{
	//				lpPartyObj = &gObj[number];

	//				if(lpPartyObj->m_Index != lpObj->m_Index)
	//				{
	//					if(lpPartyObj->MapNumber == lpObj->MapNumber)
	//					{
	//						lpPartyObj->m_SkillRedArmorIgnoreNum = lpObj->m_SkillRedArmorIgnoreNum;
	//						lpPartyObj->m_SkillRedArmorIgnoreTime = lpObj->m_SkillRedArmorIgnoreTime;
	//						GCSkillInfoSend(lpPartyObj,1,129);
	//					}
	//				}
	//			}
	//		}
	//	}
	//}
	return true;
}


bool CObjUseSkill::SkillFitness(int aIndex, CMagicInf * lpMagic)
{
	if(OBJMAX_RANGE(aIndex) == false)
	{
		return false;
	}

	LPOBJ lpObj = &gObj[aIndex];
	GCSkillInfoSend(lpObj,1,130);
	GCMagicAttackNumberSend(lpObj,lpMagic->m_Skill,lpObj->m_Index,1);	//Fix Stamina Stats!

	//.Time buff (sec) = 60+Ene/5
	lpObj->m_SkillFitnessTime = ReadConfig.SkillFitnessTimeBase + (lpObj->Energy + lpObj->AddEnergy) / ReadConfig.SkillFitnessTimeDiv;
	lpObj->m_SkillFitnessVal = ReadConfig.SkillFitnessVitBase+((lpObj->Energy + lpObj->AddEnergy)-ReadConfig.SkillFitnessEneSubtract)/(float)ReadConfig.SkillFitnessVitDiv;
	lpObj->m_SkillFitnessVal += lpObj->MasterCharacterInfo->FitnessSkillSUM;

	if (ReadConfig.SkillFitnessTimeMinMax*60 < lpObj->m_SkillFitnessTime)
		lpObj->m_SkillFitnessTime = ReadConfig.SkillFitnessTimeMinMax*60;

	if (lpObj->m_SkillFitnessVal > ReadConfig.SkillFitnessVitAddMax)
		lpObj->m_SkillFitnessVal = ReadConfig.SkillFitnessVitAddMax;
	if (lpObj->m_SkillFitnessVal < 0)
		lpObj->m_SkillFitnessVal = ReadConfig.SkillFitnessVitBase;


	gObjCalCharacter(lpObj->m_Index);									//Fitness Skill Fix

	int number;
	int partynum = 0;

	partynum = lpObj->PartyNumber;

	int partycount;
	LPOBJ lpPartyObj;

	if(partynum != -1)
	{
		partycount = gParty.m_PartyS[partynum].Count;

		for(int n = 0; n < MAX_USER_IN_PARTY;n++)
		{
			number = gParty.m_PartyS[partynum].Number[n];

			if(number >= 0)
			{
				lpPartyObj = &gObj[number];

				if(lpPartyObj->m_Index != lpObj->m_Index)
				{
					if(lpPartyObj->MapNumber == lpObj->MapNumber)
					{
						BYTE dis;
						dis = gObjCalDistance(lpObj,&gObj[number]);
						if(dis < 10)
						{
							GCSkillInfoSend(lpPartyObj,1,130);
							//GCMagicAttackNumberSend(lpPartyObj,lpMagic->m_Skill,lpPartyObj->m_Index,1);	//Fix Stamina Stats!
							GCMagicAttackNumberSend(lpObj,lpMagic->m_Skill,lpPartyObj->m_Index,1);

							lpPartyObj->m_SkillFitnessTime = lpObj->m_SkillFitnessTime;
							lpPartyObj->m_SkillFitnessVal = lpObj->m_SkillFitnessVal;

							gObjCalCharacter(lpPartyObj->m_Index);									//Fitness Skill Fix
						}

						/*if(lpObj->m_SkillRedArmorIgnoreTime > 0)
						{
							lpPartyObj->m_SkillRedArmorIgnoreNum = lpObj->m_SkillRedArmorIgnoreNum;
							lpPartyObj->m_SkillRedArmorIgnoreTime = lpObj->m_SkillRedArmorIgnoreTime;
							GCSkillInfoSend(lpPartyObj,1,129);
						}
						if(lpObj->m_SkillDefSuccessRateIncTime > 0)
						{
							lpPartyObj->m_SkillDefSuccessRateIncTime = lpObj->m_SkillDefSuccessRateIncTime;
							lpPartyObj->m_SkillDefSuccessRateIncNum = lpObj->m_SkillDefSuccessRateIncNum;
							GCSkillInfoSend(lpPartyObj,1,131);
						}*/
					}
				}
			}
		}
	}

	return true;
}



bool CObjUseSkill::SkillDefSuccessRateInc(int aIndex, CMagicInf * lpMagic)
{
	if(OBJMAX_RANGE(aIndex) == false)
	{
		return false;
	}

	LPOBJ lpObj = &gObj[aIndex];

	GCMagicAttackNumberSend(lpObj,lpMagic->m_Skill,lpObj->m_Index,1);
	GCSkillInfoSend(lpObj,1,131);

	int TotalEnergy = (lpObj->Energy + lpObj->AddEnergy);

	//%Defense Success Rate Increase = 10+(Ene-80)/10
	lpObj->m_SkillDefSuccessRateIncNum = ReadConfig.SkillDefSuccessRateIncBase + (TotalEnergy-ReadConfig.SkillDefSuccessRateIncEneSubtract) / (float)ReadConfig.SkillDefSuccessRateIncDivider;
	lpObj->m_SkillDefSuccessRateIncNum += ((lpObj->m_SkillDefSuccessRateIncNum * lpObj->MasterCharacterInfo->DefSucRateSkillMULT)/100.0f);
	lpObj->m_SkillDefSuccessRateIncNum += lpObj->MasterCharacterInfo->DefSucRateSkillSUM;

	if (lpObj->m_SkillDefSuccessRateIncNum > ReadConfig.SkillDefSuccessRateIncMaxPercent)
		lpObj->m_SkillDefSuccessRateIncNum = ReadConfig.SkillDefSuccessRateIncMaxPercent;
	if (lpObj->m_SkillDefSuccessRateIncNum < 0)
		lpObj->m_SkillDefSuccessRateIncNum = ReadConfig.SkillDefSuccessRateIncBase;

	lpObj->m_SkillDefSuccessRateIncTime = ReadConfig.SkillDefSuccessRateIncTimeBase + TotalEnergy / ReadConfig.SkillDefSuccessRateIncTimeDiv;

	if (lpObj->m_SkillDefSuccessRateIncTime > (ReadConfig.SkillDefSuccessRateIncMinMax*60))
		lpObj->m_SkillDefSuccessRateIncTime = ReadConfig.SkillDefSuccessRateIncMinMax * 60;

	if(lpObj->m_SkillDefSuccessRateIncTime > 0)
	{		
		int number;
		int partynum = 0;

		partynum = lpObj->PartyNumber;

		int partycount;
		LPOBJ lpPartyObj;

		if(partynum != -1)
		{
			partycount = gParty.m_PartyS[partynum].Count;

			for(int n = 0; n < MAX_USER_IN_PARTY;n++)
			{
				number = gParty.m_PartyS[partynum].Number[n];

				if(number >= 0)
				{
					lpPartyObj = &gObj[number];

					if(lpPartyObj->m_Index != lpObj->m_Index)
					{
						if(lpPartyObj->MapNumber == lpObj->MapNumber)
						{
							BYTE dis;
							dis = gObjCalDistance(lpObj,&gObj[number]);
							if(dis < 10)
							{
								GCMagicAttackNumberSend(lpObj,lpMagic->m_Skill,lpPartyObj->m_Index,1);

								GCSkillInfoSend(lpPartyObj,1,131);
								lpPartyObj->m_SkillDefSuccessRateIncTime = lpObj->m_SkillDefSuccessRateIncTime;
								lpPartyObj->m_SkillDefSuccessRateIncNum = lpObj->m_SkillDefSuccessRateIncNum;
							}
						}
					}
				}
			}
		}
	}
	return true;
}

struct PMSG_GOSHADOW_RESULT
{
	PBMSG_HEAD h;	// C3:19 ( 0 1 2)
	BYTE MagicNumberH;
	BYTE MagicNumberL;	// 3
	BYTE TargetNumberH;	// 6
	BYTE TargetNumberL;	// 7 
};

bool CObjUseSkill::SkillDarkSideStart(int aIndex, int aTargetIndex, CMagicInf * lpMagic)
{
	if(OBJMAX_RANGE(aIndex) == false)
	{
		return false;
	}
	//if(OBJMAX_RANGE(aTargetIndex) == false)
	//{
	//	return false;
	//}
	//int dis = gObjCalDistance(&gObj[aIndex], &gObj[aTargetIndex]);
	//if( dis >= 9 ) 
	//{
	//	aTargetIndex = 0;
	//}

	LPOBJ lpObj = &gObj[aIndex];
	int lastTarget = aIndex;

	int EnableAttack;
	int attackCnt=0;

	for(int count=0;count<MAX_VIEWPORT;count++)
	{
		if(lpObj->VpPlayer2[count].state != 0)
		{
			int tObjNum = lpObj->VpPlayer2[count].number;
			int DuelIndex = lpObj->m_iDuelUser;

			if(tObjNum >= 0)
			{
				EnableAttack = 0;
				
				if(OBJMAX_RANGE(tObjNum) == false)
				{
					EnableAttack = 0;
				}
				else if ( gclassObjAttack.PkCheck(lpObj, &gObj[tObjNum]) == FALSE )
				{
					EnableAttack = 0;
				}
				else if ( gclassObjAttack.CheckAttackArea(lpObj, &gObj[tObjNum]) == FALSE )
				{
					EnableAttack = 0;
				}
				else if(lpObj->VpPlayer2[count].type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon < 0)
				{
					EnableAttack = 1;
				}
				else if(aTargetIndex == tObjNum || DuelIndex == tObjNum)
				{
					EnableAttack = 1;
				}
				else
				{
					int CallMonIndex = gObj[tObjNum].m_Index;

					if(gObj[tObjNum].Type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon >= 0)
					{
						CallMonIndex = gObj[gObj[tObjNum].m_RecallMon].m_Index;
					}

					if(gObjTargetGuildWarCheck(lpObj,&gObj[CallMonIndex])==1)
					{
						EnableAttack = 1;
					}
				}

				if(EnableAttack != 0)
				{
					BYTE bAttackType = gObjAttackType(&gObj[tObjNum],lpObj);

					if (bAttackType > 0)
					{
						if(gObjCalDistance(lpObj,&gObj[tObjNum]) <= 6)
						{
							PMSG_MAGICATTACK_RESULT_S5E2/*PMSG_MAGICATTACK_RESULT*/ pAttack={0};
							PHeadSetBE((LPBYTE)&pAttack, 0x4A, sizeof(pAttack));
							pAttack.MagicNumberH = SET_NUMBERH(lpMagic->m_Skill);
							pAttack.MagicNumberL = SET_NUMBERL(lpMagic->m_Skill);
							pAttack.SourceNumberH = SET_NUMBERH(lastTarget);
							pAttack.SourceNumberL = SET_NUMBERL(lastTarget);
							pAttack.TargetNumberH = SET_NUMBERH(tObjNum);
							pAttack.TargetNumberL = SET_NUMBERL(tObjNum);
							lastTarget = tObjNum;
							pAttack.UnkS5E2 = gObj[aIndex].Dir;

							DataSend(aIndex, (LPBYTE)&pAttack, pAttack.h.size);
							MsgSendV2(&gObj[aIndex], (LPBYTE)&pAttack, pAttack.h.size);

							gObjAddAttackProcMsgSendDelay(lpObj,53,tObjNum,100,lpMagic->m_Skill,1);
							gObjAddAttackProcMsgSendDelay(lpObj,53,tObjNum,160,lpMagic->m_Skill,1);
							attackCnt++;
						}
					}
				}
			}
		}

		if(attackCnt > 5)
		{
			break;
		}
	}



	return true;
}
		
bool CObjUseSkill::SkillDarkSideEnd(int aIndex, int aTargetIndex, CMagicInf * lpMagic)
{
	if(OBJMAX_RANGE(aIndex) == false)
	{
		return false;
	}
	
	
	LPOBJ lpObj = &gObj[aIndex];
	int EnableAttack;
	int attackCnt=0;

	for(int count=0;count<MAX_VIEWPORT;count++)
	{
		if(lpObj->VpPlayer2[count].state != 0)
		{
			int tObjNum = lpObj->VpPlayer2[count].number;
			int DuelIndex = lpObj->m_iDuelUser;

			if(tObjNum >= 0)
			{
				EnableAttack = 0;

				if(OBJMAX_RANGE(tObjNum) == false)
				{
					EnableAttack = 0;
				}
				else if ( gclassObjAttack.PkCheck(lpObj, &gObj[tObjNum]) == FALSE )
				{
					EnableAttack = 0;
				}
				else if ( gclassObjAttack.CheckAttackArea(lpObj, &gObj[tObjNum]) == FALSE )
				{
					EnableAttack = 0;
				}
				else if(lpObj->VpPlayer2[count].type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon < 0)
				{
					EnableAttack = 1;
				}
				else if(aTargetIndex == tObjNum || DuelIndex == tObjNum)
				{
					EnableAttack = 1;
				}
				else
				{
					int CallMonIndex = gObj[tObjNum].m_Index;

					if(gObj[tObjNum].Type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon >= 0)
					{
						CallMonIndex = gObj[gObj[tObjNum].m_RecallMon].m_Index;
					}

					if(gObjTargetGuildWarCheck(lpObj,&gObj[CallMonIndex])==1)
					{
						EnableAttack = 1;
					}
				}

				if(EnableAttack != 0)
				{
					BYTE bAttackType = gObjAttackType(&gObj[tObjNum],lpObj);

					if (bAttackType > 0)
					{
						if(gObjCalDistance(lpObj,&gObj[tObjNum]) <= 6)
						{							
							int iDelay = 100;
							gObjAddAttackProcMsgSendDelay(lpObj,53,aTargetIndex,iDelay,lpMagic->m_Skill,1);
							gObjAddAttackProcMsgSendDelay(lpObj,53,aTargetIndex,iDelay+60,lpMagic->m_Skill,1);
							
							PMSG_GOSHADOW_RESULT pAttack2={0};
							PHeadSetBE((LPBYTE)&pAttack2, 0x4B, sizeof(pAttack2));
							pAttack2.MagicNumberH = SET_NUMBERH(lpMagic->m_Skill);
							pAttack2.MagicNumberL = SET_NUMBERL(lpMagic->m_Skill);
							pAttack2.TargetNumberH = SET_NUMBERH(tObjNum);
							pAttack2.TargetNumberL = SET_NUMBERL(tObjNum);

							DataSend(aIndex, (LPBYTE)&pAttack2, pAttack2.h.size);
							MsgSendV2(&gObj[aIndex], (LPBYTE)&pAttack2, pAttack2.h.size);

							GCMagicAttackNumberSend(lpObj,lpMagic->m_Skill,aTargetIndex,1);
							attackCnt++;
						}
					}
				}
			}
		}

		if(attackCnt > 5)
		{
			break;
		}
	}

	return true;
}

BOOL CObjUseSkill::RunningSkill(int aIndex, int aTargetIndex, CMagicInf * lpMagic, BOOL bCombo)
{
	LPOBJ lpObj = &gObj[aIndex];
	LPOBJ lpTargetObj = &gObj[aTargetIndex];
	WORD MagicNumber = lpMagic->m_Skill;

	lpObj->m_SkillNumber = MagicNumber;

	if(lpObj->m_iSkillInvisibleTime > 0)
	{
		this->RemoveCloakingEffect(lpObj->m_Index);
	}

	switch(MagicNumber)
	{
	case AT_SKILL_BLOCKING:
		if(lpObj->pInventory[1].IsItem() == 1)
		{
			if(lpObj->pInventory[1].m_Type >= ITEMGET(6,4) && lpObj->pInventory[1].m_Type < ITEMGET(7,0))
			{
				if(lpObj->pInventory[1].m_SkillOption != 0)
				{
					lpObj->m_SkillTime = GetTickCount() + 4000;
					GCActionSend(lpObj,18,lpObj->m_Index,aTargetIndex);
				}
			}
		}
		break;
	case AT_SKILL_LARGERINGBLOWER:
		this->SkillLargeRingBlower(aIndex,aTargetIndex,lpMagic);
		break;
	case AT_SKILL_UPPERBEAST:
		this->SkillUpperBeast(aIndex,aTargetIndex,lpMagic);
		break;
	case AT_SKILL_CHAINDRIVE:
		this->SkillChainDrive(aIndex,aTargetIndex,lpMagic);
		break;
	case AT_SKILL_DRAGONSLAYER:
		this->SkillDragonSlayer(aIndex,aTargetIndex,lpMagic);
		break;
	case AT_SKILL_REDADMORIGNORE:
		this->SkillRedArmorIgnore(aIndex,lpMagic);
		break;
	case AT_SKILL_FITNESS:
		this->SkillFitness(aIndex,lpMagic);
		break;
	case AT_SKILL_DEFENSESUCCESSRATEINCREASE:
		this->SkillDefSuccessRateInc(aIndex,lpMagic);
		break;
	case AT_SKILL_POISON:
	case AT_SKILL_METEO:
	case AT_SKILL_THUNDER:
	case AT_SKILL_FIREBALL:
	case AT_SKILL_FLAME:
	case AT_SKILL_SLOW:
	case AT_SKILL_ENERGYBALL:
	case AT_SKILL_FALLINGSLASH:
	case AT_SKILL_LUNGE:
	case AT_SKILL_UPPERCUT:
	case AT_SKILL_CYCLONE:
	case AT_SKILL_SLASH:
	case 490:
	case 491:
	case 492:
	case 493:
	case 494:
	case AT_SKILL_CROSSBOW:
	case AT_SKILL_FIVE_SHOT:
	case AT_SKILL_BOW:
	case AT_SKILL_TWISTINGSLASH:
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
	case AT_SKILL_BLOWOFFURY:
	case 465:
	case 466:
	case 467:
	case 468:
	case 469:
	case AT_SKILL_BKIMPALE:
	case AT_SKILL_PENETRATION:
	case 505:
	case 506:
	case 507:
	case 508:
	case 509:
	case AT_SKILL_POWERSLASH:
		gObjAttack(lpObj,lpTargetObj,lpMagic,1,0,0,0);
		break;
	case 475:
	case 476:
	case 477:
	case 478:
	case 479:
	case AT_SKILL_HEALING:	this->SkillHealing(aIndex,aTargetIndex,lpMagic->m_Level,MagicNumber);break;
	case AT_SKILL_REDUCE_DMG_SHIELD: this->SkillHealShield(aIndex,aTargetIndex,lpMagic->m_Level);break;
	case 435:
	case 436:
	case 437:
	case 438:
	case 439:
	case AT_SKILL_MAGICDEFENSE:	this->WizardMagicDefense(aIndex,aTargetIndex,lpMagic->m_Level,MagicNumber);break;
	case AT_SKILL_MAGIC_CIRCLE:	this->SkillMagicCircle(aIndex,aTargetIndex,lpMagic->m_Level);break;
	case 535:
	case 536:
	case 537:
	case 538:
	case 539:
	case AT_SKILL_SLEEP:	this->SkillSleep(aIndex,aTargetIndex,lpMagic->m_Level,MagicNumber,lpMagic);break;
	case AT_SKILL_NIGHT:	this->SkillNight(aIndex,aTargetIndex,lpMagic->m_Level);break;
	case 470:
	case 471:
	case 472:
	case 473:
	case 474:
	case AT_SKILL_KNIGHTADDLIFE:	this->KnightSkillAddLife(aIndex,lpMagic->m_Level,MagicNumber);break;
	case 480:
	case 481:
	case 482:
	case 483:
	case 484:
	case AT_SKILL_DEFENSE:	this->SkillDefense(aIndex,aTargetIndex,lpMagic->m_Level, MagicNumber);break;
	case 485:
	case 486:
	case 487:
	case 488:
	case 489:
	case AT_SKILL_ATTACK:	this->SkillAttack(aIndex,aTargetIndex,lpMagic->m_Level, MagicNumber);break;
	case AT_SKILL_REFLECT:	this->SkillReflect(aIndex,aTargetIndex,lpMagic->m_Level);break;
	case AT_SKILL_BERSERKER:	this->Berserker(aIndex,lpMagic->m_Level);break;

	case AT_SKILL_CALLMON1:	return this->SkillMonsterCall(lpObj->m_Index,ReadConfig.SkillMonsterCall1,lpObj->X-1,lpObj->Y+1);
	case AT_SKILL_CALLMON2:	return this->SkillMonsterCall(lpObj->m_Index,ReadConfig.SkillMonsterCall2,lpObj->X-1,lpObj->Y+1);
	case AT_SKILL_CALLMON3:	return this->SkillMonsterCall(lpObj->m_Index,ReadConfig.SkillMonsterCall3,lpObj->X-1,lpObj->Y+1);
	case AT_SKILL_CALLMON4:	return this->SkillMonsterCall(lpObj->m_Index,ReadConfig.SkillMonsterCall4,lpObj->X-1,lpObj->Y+1);
	case AT_SKILL_CALLMON5:	return this->SkillMonsterCall(lpObj->m_Index,ReadConfig.SkillMonsterCall5,lpObj->X-1,lpObj->Y+1);
	case AT_SKILL_CALLMON6:	return this->SkillMonsterCall(lpObj->m_Index,ReadConfig.SkillMonsterCall6,lpObj->X-1,lpObj->Y+1);
	case AT_SKILL_CALLMON7:	return this->SkillMonsterCall(lpObj->m_Index,ReadConfig.SkillMonsterCall7,lpObj->X-1,lpObj->Y+1);

	case AT_SKILL_EXPHELL_START:	return this->SkillHellFire2(lpObj->m_Index,aTargetIndex,lpMagic);
	case AT_SKILL_EXPHELL:	return this->SkillHellFire2(lpObj->m_Index,aTargetIndex,lpMagic);

	case 460:
	case 461:
	case 462:
	case 463:
	case 464:
	case AT_SKILL_DEATHSTAB:	return this->SkillKnightBlow(lpObj->m_Index,aTargetIndex,lpMagic,bCombo);

	case AT_SKILL_CHARGE:
	case AT_SKILL_SPEAR:	return this->SkillSpear(lpObj->m_Index,aTargetIndex,lpMagic);
	case 520:
	case 521:
	case 522:
	case 523:
	case 524:
	case AT_SKILL_FIREBURST:	return this->SkillFireBurst(lpObj->m_Index,aTargetIndex,lpMagic);
	case 515:
	case 516:
	case 517:
	case 518:
	case 519:
	case AT_SKILL_DARKHORSE_ATTACK:	return this->SkillDarkHorseAttack(lpObj->m_Index,aTargetIndex,lpMagic);
	case AT_SKILL_RECALL_PARTY:	this->SkillRecallParty(aIndex,lpMagic->m_Level);break;
	case AT_SKILL_ADD_CRITICALDAMAGE:	this->SkillAddCriticalDamage(aIndex,lpMagic->m_Level);break;

	case 500:
	case 501:
	case 502:
	case 503:
	case 504:
	case AT_SKILL_FIRESLASH:	this->MaGumSkillDefenseDown(lpObj->m_Index,aTargetIndex,1,MagicNumber);break;

	case AT_SKILL_SUMMON:	this->SkillSummon(lpObj->m_Index,aTargetIndex,lpMagic);break;
	case AT_SKILL_IMMUNE_TO_MAGIC:	this->SkillImmuneToMagic(lpObj->m_Index,lpMagic);break;
	case AT_SKILL_IMMUNE_TO_HARM:	this->SkillImmuneToHarm(lpObj->m_Index,lpMagic);break;

	case AT_SKILL_CRESCENTSLASH:	this->SkillKnightRush(lpObj->m_Index,aTargetIndex,lpMagic);break;
	case AT_SKILL_JAVALIN:	this->SkillWizardJavalin(lpObj->m_Index,aTargetIndex,lpMagic);break;
	case AT_SKILL_STARFALL:	this->SkillElfRush(lpObj->m_Index,aTargetIndex,lpMagic);break;
	case AT_SKILL_SPIRALSLASH:	this->SkillMagumOneFlash(lpObj->m_Index,aTargetIndex,lpMagic);break;
	case AT_SKILL_DEATH_CANNON:	this->SkillMagumDeathCannon(lpObj->m_Index,aTargetIndex,lpMagic);break;
	case AT_SKILL_SPACE_SPLIT:	this->SkillDarklordSpaceSplit(lpObj->m_Index,aTargetIndex,lpMagic);break;

	case AT_SKILL_BRAND_OF_SKILL:	this->SkillBrandOfSkill(aIndex,aTargetIndex,lpMagic);break;

	case AT_SKILL_REMOVAL_STUN:	this->SkillRemoveStun(aIndex,aTargetIndex,lpMagic);break;
	case AT_SKILL_ADD_MANA:	this->SkillAddMana(aIndex,aTargetIndex,lpMagic);break;
	case AT_SKILL_INVISIBLE:	this->SkillCloaking(aIndex,aTargetIndex,lpMagic);break;
	case AT_SKILL_REMOVAL_INVISIBLE:	this->SkillRemoveCloaking(aIndex,aTargetIndex,lpMagic);break;
	case AT_SKILL_REMOVAL_MAGIC:	this->SkillRemoveMagic(aIndex,aTargetIndex,lpMagic);break;

	case AT_SKILL_INFINITY_ARROW:	this->SkillInfinityArrow(aIndex,aTargetIndex,lpMagic);break;

	default:	gObjAttack(lpObj,lpTargetObj,lpMagic,1,0,0,0);break;
	}

	return true;
}




void CObjUseSkill::MaGumSkillDefenseDown(int aIndex, int aTargetIndex, int skill_level, int MagicNr)
{
	if( aIndex < OBJ_STARTUSERINDEX || aIndex > OBJMAX-1)
	{
		return;
	}
	if(OBJMAX_RANGE(aTargetIndex) == false)
	{
		return;
	}

	int skillSuccess = 1;
	LPOBJ lpObj = &gObj[aIndex];
	LPOBJ lpTargetObj = &gObj[aTargetIndex];
	WORD SkillNr = AT_SKILL_FIRESLASH;

	if(lpObj->Type == OBJ_USER && lpObj->Class != CLASS_MAGICGLADIATOR && lpObj->Class != CLASS_RAGEFIGHTER)
	{
		return;
	}


	if(retCalcSkillResistance(lpTargetObj,1))
	{
		lpTargetObj->m_SkillMagumReduceDefense = 10;
		if(MagicNr >= 500 && MagicNr <= 504)
		{
			int Mult = ((SkillNr%5) + 1)*5;
			lpTargetObj->m_SkillMagumReduceDefense += Mult;
			SkillNr = MagicNr;
		}
		lpTargetObj->m_SkillMagumReduceDefenseTime = 10;
		lpTargetObj->m_ViewSkillState |= 64;

		LogAdd("Fire Slash Decrease Defense: %d Time : %d",lpTargetObj->m_SkillMagumReduceDefense,lpTargetObj->m_SkillMagumReduceDefenseTime);

		GCMagicAttackNumberSend(lpObj,SkillNr,lpTargetObj->m_Index,skillSuccess);
	}
}




void CObjUseSkill::KnightSkillAddLife(int aIndex, int skill_level, int MagicNumber)
{
	int skillSuccess = true;
	LPOBJ lpObj = &gObj[aIndex];


	if(lpObj->Type != OBJ_USER && lpObj->m_RecallMon == -1)
	{
		return;
	}

	if(lpObj->Class != CLASS_KNIGHT && lpObj->Class != CLASS_MAGICGLADIATOR && lpObj->Class != CLASS_RAGEFIGHTER)
	{
		return;
	}

	int number;
	int partynum = 0;
	int partycount;
	int dis;

	int ApplyPartyIndex[MAX_USER_IN_PARTY];

	int viewplayer = 0;
	int viewpercent = 0;

	LPOBJ lpPartyObj;

	memset(ApplyPartyIndex,-1,sizeof(ApplyPartyIndex));

	partynum = lpObj->PartyNumber;

	if(partynum != -1)
	{
		partycount = gParty.m_PartyS[partynum].Count;

		for(int n = 0; n < MAX_USER_IN_PARTY; n++)
		{
			number = gParty.m_PartyS[partynum].Number[n];

			if(number >= 0)
			{
				lpPartyObj = &gObj[number];

				if(lpObj->MapNumber == lpPartyObj->MapNumber)
				{
					dis = gObjCalDistance(lpObj,&gObj[number]);

					if(dis < 10)
					{
						ApplyPartyIndex[n] = lpPartyObj->m_Index;
						viewplayer++;
					}
				}
			}
		}
	}

	if(viewplayer > 1)
	{
		if(viewplayer == 2)
		{
			viewpercent = 1;
		}
		else if(viewplayer == 3)
		{
			viewpercent = 2;
		}
		else if(viewplayer == 4)
		{
			viewpercent = 3;
		}
		else
		{
			viewpercent = 4;
		}
	}

	int iaddLifepower = ReadConfig.KnightSkillAddLifeBase + (lpObj->Vitality + lpObj->AddVitality) /(float) ReadConfig.KnightSkillAddLifeDivVit + (lpObj->Energy + lpObj->AddEnergy)/(float)ReadConfig.KnightSkillAddLifeDivEne + viewpercent;
	int iLifeTime = ReadConfig.KnightSkillAddLifeTimeBase + (lpObj->Energy + lpObj->AddEnergy) / ReadConfig.KnightSkillAddTimeDivGral;

	if (iLifeTime > ReadConfig.KnightSkillAddLifeMaxTimeBase)
		iLifeTime = ReadConfig.KnightSkillAddLifeMaxTimeBase;

	if (iaddLifepower > ReadConfig.KnightMaxMultiplierHP)
		iaddLifepower = ReadConfig.KnightMaxMultiplierHP;

	int iaddlife=0;
	int iaddMana=0;

	if(partynum == -1)
	{
		iaddlife = int(lpObj->MaxLife) * iaddLifepower /(float) ReadConfig.KnightSkillAddLifeDivGral;
		if(MagicNumber >= 470 && MagicNumber <= 474)
		{
			float Mult = (float)((DefMagicInf[MagicNumber].m_DamageMin / 10.0f) + 1.0f);
			//float Mult = (((float)(MagicNumber%5) + 2) / 10)+1;
			iaddlife = (int)iaddlife * Mult;
		}

		if( lpObj->MasterCharacterInfo->SwellLifeMana > 0)
		{
			//iaddMana = (lpObj->MaxMana * lpObj->MasterCharacterInfo->SwellLifeMana)- lpObj->MaxMana;
			iaddMana = (lpObj->MaxMana * lpObj->MasterCharacterInfo->SwellLifeMana / 100);
			lpObj->AddMana -= lpObj->m_SkillAddLife_Mana;
		}
		if( lpObj->MasterCharacterInfo->SwellLifeStamina > 0)
		{
			//iaddlife += ((lpObj->MaxLife * lpObj->MasterCharacterInfo->SwellLifeStamina)- lpObj->MaxLife);
			iaddlife += (float)(lpObj->MaxLife * lpObj->MasterCharacterInfo->SwellLifeStamina/100);
		}

		lpObj->AddLife -= lpObj->m_SkillAddLife;

		if(ReadConfig.FixMaxGage == 1)
		{
			if (lpObj->MaxLife + lpObj->AddLife + iaddlife < MAX_LIFE_VALUE)
			{
				lpObj->AddLife += iaddlife;
			} else 
			{
				if (lpObj->MaxLife + lpObj->AddLife >= MAX_LIFE_VALUE)
				{
					iaddlife = 0;
					lpObj->MaxLife = MAX_LIFE_VALUE;
					lpObj->AddLife = 0;

					if(ReadConfig.ShowMaxGageReached == 1)
						GCServerMsgStringSend ("[Info] Maximum HP reached!",lpObj->m_Index,0x01 ) ;
				} else {
					iaddlife = MAX_LIFE_VALUE - lpObj->MaxLife - lpObj->AddLife;
					lpObj->AddLife += iaddlife;
				}
			}

			if( lpObj->MasterCharacterInfo->SwellLifeMana > 0)
			{
				if (lpObj->MaxMana + lpObj->AddMana + iaddMana < MAX_MANA_VALUE)
				{
					lpObj->AddMana += iaddMana;
				} else 
				{
					if (lpObj->MaxMana + lpObj->AddMana >= MAX_MANA_VALUE)
					{
						iaddMana = 0;
						lpObj->MaxMana = MAX_MANA_VALUE;
						lpObj->AddMana = 0;

						if(ReadConfig.ShowMaxGageReached == 1)
							GCServerMsgStringSend ("[Info] Maximum MP reached!",lpObj->m_Index,0x01 ) ;
					} else {
						iaddMana = MAX_MANA_VALUE - lpObj->MaxMana - lpObj->AddMana;
						lpObj->AddMana += iaddMana;
					}
				}
			}
		}else
		{
			lpObj->AddLife += iaddlife;
			if( lpObj->MasterCharacterInfo->SwellLifeMana > 0)
				lpObj->AddMana += iaddMana;
		}

		lpObj->m_SkillAddLife = iaddlife;
		lpObj->m_SkillAddLifeTime = iLifeTime;

		GCReFillSend(lpObj->m_Index,lpObj->MaxLife + lpObj->AddLife,0xFE,0,lpObj->iMaxShield + lpObj->iAddShield);

		if( lpObj->MasterCharacterInfo->SwellLifeMana > 0 )
		{
			if( lpObj->MasterCharacterInfo->SwellLifeMana > 0)
				lpObj->m_SkillAddLife_Mana = iaddMana;

			GCManaSend(lpObj->m_Index,lpObj->MaxMana + lpObj->AddMana,0xFE,0,lpObj->MaxBP + lpObj->AddBP);
		}

		lpObj->m_ViewSkillState |= 16;
		GCMagicAttackNumberSend(lpObj,AT_SKILL_KNIGHTADDLIFE,lpObj->m_Index,skillSuccess);
	}
	else
	{
		for(int n = 0; n < MAX_USER_IN_PARTY; n++)
		{
			iaddlife = 0;
			iaddMana = 0;

			if(ApplyPartyIndex[n]!= -1)
			{
				lpPartyObj = &gObj[ApplyPartyIndex[n]];

				iaddlife = int(lpPartyObj->MaxLife) * iaddLifepower /(float) ReadConfig.KnightSkillAddLifeDivGral;
				if(MagicNumber >= 470 && MagicNumber <= 474)
				{
					float Mult = (float)((DefMagicInf[MagicNumber].m_DamageMin / 10.0f) + 1.0f);
					//float Mult = (((float)(MagicNumber%5) + 2) / 10)+1;
					iaddlife = (int)iaddlife * Mult;
				}

				if( lpObj->MasterCharacterInfo->SwellLifeMana > 0)
				{
					//iaddMana = (lpPartyObj->MaxMana * lpObj->MasterCharacterInfo->SwellLifeMana) - lpPartyObj->MaxMana;;
					iaddMana = (lpPartyObj->MaxMana * lpObj->MasterCharacterInfo->SwellLifeMana/100);
					lpPartyObj->AddMana -= lpPartyObj->m_SkillAddLife_Mana;
				}
				if( lpObj->MasterCharacterInfo->SwellLifeStamina > 0)
				{
					//iaddlife += (lpPartyObj->MaxLife * lpObj->MasterCharacterInfo->SwellLifeStamina)- lpPartyObj->MaxLife;
					iaddlife += (lpPartyObj->MaxLife * lpObj->MasterCharacterInfo->SwellLifeStamina/100);
				}

				lpPartyObj->AddLife -= lpPartyObj->m_SkillAddLife;

				if(ReadConfig.FixMaxGage == 1)
				{
					if (lpPartyObj->MaxLife + lpPartyObj->AddLife + iaddlife < MAX_LIFE_VALUE)
					{
						lpPartyObj->AddLife += iaddlife;
					} else {
						if (lpPartyObj->MaxLife + lpPartyObj->AddLife >= MAX_LIFE_VALUE)
						{
							iaddlife = 0;
							lpPartyObj->MaxLife = MAX_LIFE_VALUE;
							lpPartyObj->AddLife = 0;
							if(ReadConfig.ShowMaxGageReached == 1)
								GCServerMsgStringSend ("[Info] Maximum HP reached!",lpPartyObj->m_Index,0x01 ) ;
						} else {
							iaddlife = MAX_LIFE_VALUE - lpPartyObj->MaxLife - lpPartyObj->AddLife;
							lpPartyObj->AddLife += iaddlife;
						}
					}

					if( lpObj->MasterCharacterInfo->SwellLifeMana > 0)
					{
						if (lpPartyObj->MaxMana + lpPartyObj->AddMana + iaddMana < MAX_MANA_VALUE)
						{
							lpPartyObj->AddMana += iaddMana;
						} else 
						{
							if (lpPartyObj->MaxMana + lpPartyObj->AddMana >= MAX_MANA_VALUE)
							{
								iaddMana = 0;
								lpPartyObj->MaxMana = MAX_MANA_VALUE;
								lpPartyObj->AddMana = 0;

								if(ReadConfig.ShowMaxGageReached == 1)
									GCServerMsgStringSend ("[Info] Maximum MP reached!",lpPartyObj->m_Index,0x01 ) ;
							} else {
								iaddMana = MAX_MANA_VALUE - lpPartyObj->MaxMana - lpPartyObj->AddMana;
								lpPartyObj->AddMana += iaddMana;
							}
						}
					}
				}else
				{
					lpPartyObj->AddLife += iaddlife;
					if( lpObj->MasterCharacterInfo->SwellLifeMana > 0)
						lpPartyObj->AddMana += iaddMana;
				}

				lpPartyObj->m_SkillAddLife = iaddlife;
				lpPartyObj->m_SkillAddLifeTime = iLifeTime;

				GCReFillSend(lpPartyObj->m_Index,lpPartyObj->MaxLife + lpPartyObj->AddLife,0xFE,0,lpPartyObj->iMaxShield + lpPartyObj->iAddShield);
				
				if( lpObj->MasterCharacterInfo->SwellLifeMana > 0 )
				{
					if( lpObj->MasterCharacterInfo->SwellLifeMana > 0)
						lpPartyObj->m_SkillAddLife_Mana = iaddMana;

					GCManaSend(lpPartyObj->m_Index,lpPartyObj->MaxMana + lpPartyObj->AddMana,0xFE,0,lpPartyObj->MaxBP + lpPartyObj->AddBP);
				}

				lpPartyObj->m_ViewSkillState |= 16;
				GCMagicAttackNumberSend(lpObj,AT_SKILL_KNIGHTADDLIFE,lpPartyObj->m_Index,skillSuccess);
			}
		}
	}
}




void CObjUseSkill::WizardMagicDefense(int aIndex, int aTargetIndex, int skill_level, WORD SkillNr)
{
	int skillSuccess = true;

	LPOBJ lpObj = &gObj[aIndex];
	LPOBJ lpTargetObj = &gObj[aTargetIndex];
	WORD MagicNr = AT_SKILL_MAGICDEFENSE;

	if(lpObj->Type != OBJ_USER && lpObj->m_RecallMon== -1)
	{
		return;
	}

	if (lpObj->Class == CLASS_MAGICGLADIATOR)
	{
		lpTargetObj->m_WizardSkillDefense = ReadConfig.MGMagicDefenseBase + (lpObj->Dexterity + lpObj->AddDexterity) /(float) ReadConfig.MGMagicDefenseDiv1 + (lpObj->Energy + lpObj->AddEnergy) /(float) ReadConfig.MGMagicDefenseDiv2;
		if (lpTargetObj->m_WizardSkillDefense > ReadConfig.MGMagicMaximumDefense)
			lpTargetObj->m_WizardSkillDefense = ReadConfig.MGMagicMaximumDefense;
		lpTargetObj->m_WizardSkillDefense += ((lpTargetObj->m_WizardSkillDefense * lpObj->MasterCharacterInfo->IncSoulBarrier)/100.0f);

		lpTargetObj->m_WizardSkillDefenseTime = ReadConfig.MGMagicDefenseTimeBase + (lpObj->Energy + lpObj->AddEnergy) / ReadConfig.MGMagicDefenseTimeDiv1;
		if (lpTargetObj->m_WizardSkillDefenseTime > ReadConfig.MGMagicDefenseTimeLimit*60)
			lpTargetObj->m_WizardSkillDefenseTime = ReadConfig.MGMagicDefenseTimeLimit*60;
		lpTargetObj->m_WizardSkillDefenseTime += lpObj->MasterCharacterInfo->IncSoulBarrierTime;
	}
	else if (lpObj->Class == CLASS_WIZARD)
	{
		lpTargetObj->m_WizardSkillDefense = ReadConfig.WizardMagicDefenseBase + (lpObj->Dexterity + lpObj->AddDexterity) /(float) ReadConfig.WizardMagicDefenseDiv1 + (lpObj->Energy + lpObj->AddEnergy) /(float) ReadConfig.WizardMagicDefenseDiv2;
		if (lpTargetObj->m_WizardSkillDefense > ReadConfig.WizardMagicMaximumDefense)
			lpTargetObj->m_WizardSkillDefense = ReadConfig.WizardMagicMaximumDefense;
		lpTargetObj->m_WizardSkillDefense += ((lpTargetObj->m_WizardSkillDefense * lpObj->MasterCharacterInfo->IncSoulBarrier)/100.0f);

		lpTargetObj->m_WizardSkillDefenseTime = ReadConfig.WizardMagicDefenseTimeBase + (lpObj->Energy + lpObj->AddEnergy) / ReadConfig.WizardMagicDefenseTimeDiv1;
		if (lpTargetObj->m_WizardSkillDefenseTime > ReadConfig.WizardMagicDefenseTimeLimit*60)
			lpTargetObj->m_WizardSkillDefenseTime = ReadConfig.WizardMagicDefenseTimeLimit*60;
		lpTargetObj->m_WizardSkillDefenseTime += lpObj->MasterCharacterInfo->IncSoulBarrierTime;
	}
	else
	{
		return;
	}

	if(lpObj->PartyNumber != lpTargetObj->PartyNumber)
	{
		return;
	}

	lpTargetObj->m_ViewSkillState |= 256;

	int Mult=0;
	if(SkillNr >= 435 && SkillNr <= 439)
	{
		Mult = (SkillNr - 435 + 1)*5;
		lpTargetObj->m_WizardSkillDefense += Mult;
		MagicNr = SkillNr;
	}

	if (lpTargetObj->m_WizardSkillDefense >= 100)
	{
		LogAddTD("[Skill][Mana Shield][%s][%s] Shield overflow Error [%d:%d,%d]",
			lpTargetObj->AccountID,lpTargetObj->Name,
			SkillNr,lpTargetObj->m_WizardSkillDefense,Mult );
	}

	GCMagicAttackNumberSend(lpObj,MagicNr,lpTargetObj->m_Index,skillSuccess);
}




void CObjUseSkill::SkillDefense(int aIndex, int aTargetIndex, int skill_level, WORD SkillNr)
{
	int skillSuccess = true;
	LPOBJ lpObj = &gObj[aIndex];
	LPOBJ lpTargetObj = &gObj[aTargetIndex];

	if(lpTargetObj->Type != OBJ_USER && lpTargetObj->m_RecallMon == -1)
	{
		return;
	}

	if(lpObj->Class != CLASS_ELF)
	{
		return;
	}

	if(CC_MAP_RANGE(lpObj->MapNumber) && lpObj->m_Index != lpTargetObj->m_Index)
	{
		return;
	}

	if(skill_level < 0 || skill_level > 7)
	{
		skill_level = 0;
	}


	float skill_defense = float(skill_level + 1)*ReadConfig.ElfBuffDefenseLvlMult + (lpObj->Energy + lpObj->AddEnergy) /(float) ReadConfig.ElfBuffDefenseDiv;
	
	if(SkillNr >= 480 && SkillNr <= 484)
		skill_defense = float(((skill_level + 1)*ReadConfig.ElfBuffDefenseLvlMult + (lpObj->Energy + lpObj->AddEnergy) /(float) ReadConfig.ElfBuffDefenseDiv)  *  (1.0f + (10.0f/DefMagicInf[SkillNr].m_DamageMin)));

	if(lpTargetObj->Class == CLASS_KNIGHT)
	{
		skill_defense += skill_defense/10.0f;
	}
	if(lpTargetObj->Class == CLASS_KNIGHT)
	{
		skill_defense += skill_defense * ReadConfig.ElfBuffDefenseAddPlus;
	}

	skill_defense += (skill_defense * lpObj->MasterCharacterInfo->IncGreaterDefense)/100.0f;

	lpTargetObj->m_SkillDefense = skill_defense;
	lpTargetObj->m_SkillDefenseTime = ReadConfig.ElfBuffDefenseTime + (lpObj->Energy + lpObj->AddEnergy)/ReadConfig.ElfBuffDefenseTimeEneDiv;
	if (lpTargetObj->m_SkillDefenseTime > ReadConfig.ElfBuffDefenseTimeMax)
		lpTargetObj->m_SkillDefenseTime = ReadConfig.ElfBuffDefenseTimeMax;

	lpTargetObj->m_SkillDefenseTime += (lpTargetObj->m_SkillDefenseTime * lpObj->MasterCharacterInfo->IncGreaterDefenseTime)/100.0f;

	lpTargetObj->m_ViewSkillState |= 8;

	for(int n = 0; n < MAX_VIEWPORT; n++)
	{
		if(lpObj->VpPlayer2[n].type == OBJ_MONSTER && lpObj->VpPlayer2[n].state != 0)
		{
			LPOBJ lpMonster = &gObj[lpObj->VpPlayer2[n].number];
			if(lpMonster->m_iCurrentAI != 0)
			{
				lpMonster->m_Agro.IncAgro(lpObj->m_Index,int(skill_defense)/10);
			}
		}
	}

	GCMagicAttackNumberSend(lpObj,SkillNr/*AT_SKILL_DEFENSE*/,lpTargetObj->m_Index,skillSuccess);
}




void CObjUseSkill::SkillAttack(int aIndex, int aTargetIndex, int skill_level, WORD SkillNr)
{
	int skillSuccess = true;
	LPOBJ lpObj = &gObj[aIndex];
	LPOBJ lpTargetObj = &gObj[aTargetIndex];

	if(lpTargetObj->Type != OBJ_USER && lpTargetObj->m_RecallMon == -1)
	{
		return;
	}

	if(lpObj->Class != CLASS_ELF)
	{
		return;
	}

	if(CC_MAP_RANGE(lpObj->MapNumber) && lpObj->m_Index != lpTargetObj->m_Index)
	{
		return;
	}

	if(skill_level < 0 || skill_level > 7)
	{
		skill_level = 0;
	}

	float skill_attack = float((skill_level + 1) * ReadConfig.ElfBuffAttackLvlMult) + (lpObj->Energy + lpObj->AddEnergy) /(float) ReadConfig.ElfBuffAttackDiv;
	
	if(SkillNr >= 485 && SkillNr <= 489)
		skill_attack = float( (((skill_level + 1) * ReadConfig.ElfBuffAttackLvlMult) + (lpObj->Energy + lpObj->AddEnergy) /(float) ReadConfig.ElfBuffAttackDiv)  *  (1.0f + (10.0f/DefMagicInf[SkillNr].m_DamageMin)));

	if(lpTargetObj->Class == CLASS_KNIGHT)
	{
		skill_attack += skill_attack/10.0f;
	}

	if(lpTargetObj->Class == CLASS_ELF)
	{
		skill_attack += skill_attack * ReadConfig.ElfBuffAttackAddPlus;
	}

	skill_attack += (skill_attack * lpObj->MasterCharacterInfo->IncGreaterAttack)/100.0f;

	lpTargetObj->m_SkillAttack = skill_attack;
	lpTargetObj->m_SkillAttackTime = ReadConfig.ElfBuffAttackTime + (lpObj->Energy + lpObj->AddEnergy)/ReadConfig.ElfBuffAttackTimeEneDiv;
	if (lpTargetObj->m_SkillAttackTime > ReadConfig.ElfBuffAttackTimeMax)
		lpTargetObj->m_SkillAttackTime = ReadConfig.ElfBuffAttackTimeMax;

	lpTargetObj->m_SkillAttackTime += (ReadConfig.ElfBuffAttackTime * lpObj->MasterCharacterInfo->IncGreaterAttackTime)/100.0f;
	lpTargetObj->m_ViewSkillState |= 4;

	for(int n = 0; n < MAX_VIEWPORT; n++)
	{
		if(lpObj->VpPlayer2[n].type == OBJ_MONSTER && lpObj->VpPlayer2[n].state != 0)
		{
			LPOBJ lpMonster = &gObj[lpObj->VpPlayer2[n].number];
			if(lpMonster->m_iCurrentAI != 0)
			{
				lpMonster->m_Agro.IncAgro(lpObj->m_Index,int(skill_attack)/10);
			}
		}
	}

#if (WL_PROTECT==1)  
		VM_START_WITHLEVEL(4)
		int MyCheckVar1;
		CHECK_REGISTRATION(MyCheckVar1, 0x04134597)  
		if (MyCheckVar1 != 0x04134597)
		{	
			Sleep(1500);
		}
		VM_END
#endif
	GCMagicAttackNumberSend(lpObj,SkillNr/*AT_SKILL_ATTACK*/,lpTargetObj->m_Index,skillSuccess);

}

/*void CObjUseSkill::SkillIllusionTemple(int aIndex, int aTargetIndex, int Skill)
{
	if ( OBJMAX_RANGE(aIndex) == FALSE )
	{
		return;
	}
	if ( OBJMAX_RANGE(aTargetIndex) == FALSE )
	{
		return;
	}

	LPOBJ lpTargetObj = &gObj[aTargetIndex];
	BYTE Effect = 0;
	
	switch(Skill)
	{
		case 210:
		{
			Effect = 34;
			//lpTargetObj->m_SkillIT_MagicShieldOrderTime = 15;
		}break;
		case 211:
		{
			Effect = 65;
			//lpTargetObj->m_SkillIT_RestraintOrderTime = 5;
		}break;
		
		default:
		{
			return;
		}
	}
	
	GCSkillInfoSend(lpTargetObj,1,Effect);
}*/


void CObjUseSkill::Berserker(int aIndex, int skill_level)
{
	int skillSuccess = true;
	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->Type != OBJ_USER)
	{
		return;
	}

	if(lpObj->Class != CLASS_SUMMONER)
	{
		return;
	}

	if(lpObj->m_SkillBerserkerTime > 0)
	{
		GCMagicAttackNumberSend(lpObj,AT_SKILL_BERSERKER,lpObj->m_Index,skillSuccess);
		return;
	}

	if(skill_level < 0 || skill_level > 7)
	{
		skill_level = 0;
	}

	//lpObj->MaxLife = lpObj->MaxLife * 0.85;
	//lpObj->MaxMana = lpObj->MaxMana * 1.15;

	int TotalEnergy = (lpObj->Energy + lpObj->AddEnergy);

	float skill_BerserkerDef = 0;
	float skill_BerserkerAtt = (float) ((TotalEnergy * ReadConfig.SkillBerserkerAttMult ) + 1.0f);
	
	if (skill_BerserkerAtt > ReadConfig.SkillBerserkerAttMaxMult)
		skill_BerserkerAtt = ReadConfig.SkillBerserkerAttMaxMult;

	//skill_BerserkerAtt += (float) (TotalEnergy/10000);

	if(TotalEnergy < 1800)
	{
		skill_BerserkerDef = (float) (((TotalEnergy / ReadConfig.SkillBerserkerDefDiv ) + ReadConfig.SkillBerserkerDefBase) / 100.0f);
	}else
	{
		skill_BerserkerDef = 0.90f;
	}
	
	lpObj->m_SkillBerserkerDef = skill_BerserkerDef;
	lpObj->m_SkillBerserkerAttack = skill_BerserkerAtt;

	lpObj->m_SkillBerserkerTime = ReadConfig.SkillBerserkerTimeBase + TotalEnergy / ReadConfig.SkillBerserkerTimeDiv;

	GCMagicAttackNumberSend(lpObj,AT_SKILL_BERSERKER,lpObj->m_Index,skillSuccess);
	GCSkillInfoSend(lpObj,1,0x51);
	gObjCalCharacter(lpObj->m_Index);
}


void CObjUseSkill::SkillReflect(int aIndex, int aTargetIndex, int skill_level)
{
	int skillSuccess = true;
	LPOBJ lpObj = &gObj[aIndex];
	LPOBJ lpTargetObj = &gObj[aTargetIndex];

	if(lpTargetObj->Type != OBJ_USER && lpTargetObj->m_RecallMon == -1)
	{
		return;
	}

	if(lpObj->Class != CLASS_SUMMONER)
	{
		return;
	}

	if(CC_MAP_RANGE(lpObj->MapNumber) && lpObj->m_Index != lpTargetObj->m_Index)
	{
		return;
	}

	if(lpTargetObj->m_SkillReflect > 0)
	{
		return;
		//int val = lpTargetObj->DamageReflect -= lpTargetObj->m_SkillReflect;
		//if(val < 0)
		//	lpTargetObj->DamageReflect = 0;
		//lpTargetObj->m_SkillReflect = 0;
	}

	if(skill_level < 0 || skill_level > 7)
	{
		skill_level = 0;
	}

	int skill_reflect = int (((skill_level + 1) * 3) + (lpObj->Energy + lpObj->AddEnergy) / (float)ReadConfig.SkillReflectDiv);

	if(skill_reflect > 254)
		skill_reflect = 255;

	if(lpTargetObj->DamageReflect + skill_reflect > 254)
	{
		skill_reflect = 255 - lpTargetObj->DamageReflect;
	}

	lpTargetObj->m_SkillReflect = skill_reflect;
	lpTargetObj->DamageReflect += lpTargetObj->m_SkillReflect;
	lpTargetObj->m_SkillReflectTime = ReadConfig.SkillReflectTimeBase + (lpObj->Energy + lpObj->AddEnergy) / ReadConfig.SkillReflectTimeDiv;

	GCMagicAttackNumberSend(lpObj,AT_SKILL_REFLECT,lpTargetObj->m_Index,skillSuccess);
	GCSkillInfoSend(lpTargetObj,1,0x47);
}

void CObjUseSkill::SkillMagicCircle(int aIndex, int aTargetIndex, int skill_level)
{
	int skillSuccess = true;

	LPOBJ lpObj = &gObj[aIndex];
	LPOBJ lpTargetObj = &gObj[aTargetIndex];

	if(lpObj->Type != OBJ_USER && lpObj->m_RecallMon== -1)
	{
		return;
	}

	if(lpObj->Class != CLASS_WIZARD && lpObj->Class != CLASS_MAGICGLADIATOR && lpObj->Class != CLASS_RAGEFIGHTER)
	{
		return;
	}

	if(lpObj->PartyNumber != lpTargetObj->PartyNumber)
	{
		return;
	}

	if(lpTargetObj->m_SkillMagicCircle > 0)
	{
		lpTargetObj->m_MagicDamageMin -= lpTargetObj->m_SkillMagicCircle;
		lpTargetObj->m_CriticalDamage -= lpTargetObj->m_SkillMagicCircleCriticalDamage;
		lpTargetObj->m_SkillMagicCircle = 0;
	}

	lpTargetObj->m_SkillMagicCircle = lpObj->m_MagicDamageMin * ReadConfig.MagicCircleMult;
	lpTargetObj->m_SkillMagicCircleTime = ReadConfig.MagicCircleMinDur + (lpObj->Energy + lpObj->AddEnergy) / ReadConfig.MagicCircleDurDiv;

	if(ReadConfig.S6E2 == 1)
	{
		if(lpObj->MasterCharacterInfo->IncExpansionWizardy1 > 0)
		{
			lpTargetObj->m_SkillMagicCircle += (lpTargetObj->m_SkillMagicCircle * lpObj->MasterCharacterInfo->IncExpansionWizardy1) / 100.0f;
		}
		lpTargetObj->m_SkillMagicCircleCriticalDamage = lpObj->MasterCharacterInfo->IncExpansionWizardy2;
	}

	GCMagicAttackNumberSend(lpObj,AT_SKILL_MAGIC_CIRCLE,lpTargetObj->m_Index,skillSuccess);
	GCSkillInfoSend(lpTargetObj,1,0x52);
}


void CObjUseSkill::SkillReduceDamage(int aIndex, int aTargetIndex, int skill_level)
{
	LPOBJ lpObj = &gObj[aIndex];
	int tObjNum;
	int count = 0;
	int HitCount = 0;
	int bAttack;
	int DuelIndex = lpObj->m_iDuelUser;
	int EnableAttack;

	while ( true )
	{
		if(lpObj->VpPlayer2[count].state != 0)
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if(tObjNum >= 0)
			{
				EnableAttack = 0;
				
				if ( gclassObjAttack.PkCheck(lpObj, &gObj[tObjNum]) == FALSE )
				{
					EnableAttack = 0;
				}
				else if ( gclassObjAttack.CheckAttackArea(lpObj, &gObj[tObjNum]) == FALSE )
				{
					EnableAttack = 0;
				}
				else if(lpObj->VpPlayer2[count].type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon < 0)
				{
					EnableAttack = 1;
				}
				else if(tObjNum == aTargetIndex || DuelIndex == tObjNum)
				{
					EnableAttack = 1;
				}
				else
				{
					int CallMonIndex = gObj[tObjNum].m_Index;

					if(gObj[tObjNum].Type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon >= 0)
					{
						CallMonIndex = gObj[gObj[tObjNum].m_RecallMon].m_Index;
					}

					if(gObjTargetGuildWarCheck(lpObj,&gObj[CallMonIndex])==1)
					{
						EnableAttack = 1;
					}
				}

				if(EnableAttack != 0)
				{
					BYTE bAttackType = gObjAttackType(&gObj[tObjNum],lpObj);

					if (bAttackType > 0)
					{
						if(this->CalDistance(lpObj->X,lpObj->Y,gObj[tObjNum].X,gObj[tObjNum].Y) < 4)
						{
							bAttack = 0;
	
							if(HitCount > 10)
							{
								bAttack = 0;
							}
						
							if(HitCount >= 5)
							{
								if(rand()%2)
								{
									bAttack = 1;
								}
							}
							else
							{
								bAttack = 1;
							}

							if(bAttack != 0)
							{
								if(gObj[tObjNum].m_SkillReduceDamage > 0)
								{
									gObj[tObjNum].m_SkillReduceDamage = 0;
								}

								int skill_Reducedamage = int (((skill_level + 1) * 2) + (((lpObj->Energy + lpObj->AddEnergy) / ReadConfig.SkillReduceDamageDiv) * ReadConfig.SkillReduceDamageMult ));

								gObj[tObjNum].m_SkillReduceDamage = skill_Reducedamage;
								gObj[tObjNum].m_SkillReduceDamageTime = ReadConfig.SkillReduceDamageTimeBase + (lpObj->Energy + lpObj->AddEnergy) / ReadConfig.SkillReduceDamageTimeDiv;

								GCMagicAttackNumberSend(lpObj,AT_SKILL_REDUCE_DAMAGE,gObj[tObjNum].m_Index,1);
								GCSkillInfoSend(&gObj[tObjNum],1,0x4C);
							}
						}
					}
				}
			}
		}

		count++;
		if(count > MAX_VIEWPORT -1)
		{
			break;
		}
	}
	/*int skillSuccess = true;
	LPOBJ lpObj = &gObj[aIndex];
	
	if(aTargetIndex == 0x0000ffff)
		aTargetIndex = aIndex;

	LPOBJ lpTargetObj = &gObj[aTargetIndex];

	if(lpTargetObj->Type != OBJ_USER && lpTargetObj->m_RecallMon == -1)
	{
		return;
	}

	if(lpObj->Class != CLASS_SUMMONER)
	{
		return;
	}

	if(CC_MAP_RANGE(lpObj->MapNumber) && lpObj->m_Index != lpTargetObj->m_Index)
	{
		return;
	}

	if(skill_level < 0 || skill_level > 7)
	{
		skill_level = 0;
	}

	if(lpTargetObj->m_SkillExcellentDamage > 0)
	{
		lpTargetObj->m_ExcelentDamage -= lpTargetObj->m_SkillExcellentDamage;
		lpTargetObj->m_SkillExcellentDamage = 0;
	}

	int skill_excellentdamage = int (((skill_level + 1) * 2) + (((lpObj->Energy + lpObj->AddEnergy) / ReadConfig.SkillExcellentDamageDiv) * ReadConfig.SkillExcellentDamageMult ));

	lpTargetObj->m_SkillExcellentDamage = skill_excellentdamage;
	lpTargetObj->m_ExcelentDamage += lpTargetObj->m_SkillExcellentDamage;
	lpTargetObj->m_SkillExcellentDamageTime = ReadConfig.SkillExcellentDamageTimeBase + (lpObj->Energy + lpObj->AddEnergy) / ReadConfig.SkillExcellentDamageTimeDiv;

	GCMagicAttackNumberSend(lpObj,AT_SKILL_EXCELLENT_DAMAGE,lpTargetObj->m_Index,skillSuccess);
	GCSkillInfoSend(lpTargetObj,1,0x4C);*/
}

void CObjUseSkill::SkillReduceDefense(int aIndex, int aTargetIndex, int skill_level)
{
	LPOBJ lpObj = &gObj[aIndex];
	int tObjNum;
	int count = 0;
	int HitCount = 0;
	int bAttack;
	int DuelIndex = lpObj->m_iDuelUser;
	int EnableAttack;

	while ( true )
	{
		if(lpObj->VpPlayer2[count].state != 0)
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if(tObjNum >= 0)
			{
				EnableAttack = 0;
				
				if ( gclassObjAttack.PkCheck(lpObj, &gObj[tObjNum]) == FALSE )
				{
					EnableAttack = 0;
				}
				else if ( gclassObjAttack.CheckAttackArea(lpObj, &gObj[tObjNum]) == FALSE )
				{
					EnableAttack = 0;
				}
				else if(lpObj->VpPlayer2[count].type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon < 0)
				{
					EnableAttack = 1;
				}
				else if(tObjNum == aTargetIndex || DuelIndex == tObjNum)
				{
					EnableAttack = 1;
				}
				else
				{
					int CallMonIndex = gObj[tObjNum].m_Index;

					if(gObj[tObjNum].Type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon >= 0)
					{
						CallMonIndex = gObj[gObj[tObjNum].m_RecallMon].m_Index;
					}

					if(gObjTargetGuildWarCheck(lpObj,&gObj[CallMonIndex])==1)
					{
						EnableAttack = 1;
					}
				}

				if(EnableAttack != 0)
				{
					BYTE bAttackType = gObjAttackType(&gObj[tObjNum],lpObj);

					if (bAttackType > 0)
					{
						if(this->CalDistance(lpObj->X,lpObj->Y,gObj[tObjNum].X,gObj[tObjNum].Y) < 4)
						{
							bAttack = 0;

							if(HitCount > 10)
							{
								bAttack = 0;
							}
						
							if(HitCount >= 5)
							{
								if(rand()%2)
								{
									bAttack = 1;
								}
							}
							else
							{
								bAttack = 1;
							}

							if(bAttack != 0)
							{
								if(gObj[tObjNum].m_SkillReduceDefense > 0)
								{
									gObj[tObjNum].m_SkillReduceDefense = 0;
								}

								int skill_successblock = int (((skill_level + 1) * 2) + ( ((lpObj->Energy + lpObj->AddEnergy) / (float)ReadConfig.SkillReduceDefenseDiv1) + (lpObj->Level / (float)ReadConfig.SkillReduceDefenseDiv2) ));

								gObj[tObjNum].m_SkillReduceDefense = skill_successblock;
								gObj[tObjNum].m_SkillReduceDefenseTime = ReadConfig.SkillReduceDefenseTimeBase + (lpObj->Energy + lpObj->AddEnergy) / ReadConfig.SkillReduceDefenseTimeDiv;

								GCMagicAttackNumberSend(lpObj,AT_SKILL_REDUCE_DEFENSE,gObj[tObjNum].m_Index,1);
								GCSkillInfoSend(&gObj[tObjNum],1,0x4D);
							}
						}
					}
				}
			}
		}

		count++;
		if(count > MAX_VIEWPORT -1)
		{
			break;
		}
	}
	//int skillSuccess = true;
	//LPOBJ lpObj = &gObj[aIndex];
	//
	//if(aTargetIndex == 0x0000ffff)
	//	aTargetIndex = aIndex;

	//LPOBJ lpTargetObj = &gObj[aTargetIndex];


	//if(lpTargetObj->Type != OBJ_USER && lpTargetObj->m_RecallMon == -1)
	//{
	//	return;
	//}

	//if(lpObj->Class != CLASS_SUMMONER)
	//{
	//	return;
	//}

	//if(CC_MAP_RANGE(lpObj->MapNumber) && lpObj->m_Index != lpTargetObj->m_Index)
	//{
	//	return;
	//}

	//if(skill_level < 0 || skill_level > 7)
	//{
	//	skill_level = 0;
	//}

	//if(lpTargetObj->m_SkillReduceDefense > 0)
	//{
	//	lpTargetObj->m_SuccessfulBlocking -= lpTargetObj->m_SkillReduceDefense;
	//	lpTargetObj->m_SkillReduceDefense = 0;
	//}

	//int skill_successblock = int (((skill_level + 1) * 2) + ( ((lpObj->Energy + lpObj->AddEnergy) / ReadConfig.SkillReduceDefenseDiv1) + (lpObj->Level / ReadConfig.SkillReduceDefenseDiv2) ));

	//lpTargetObj->m_SkillReduceDefense = skill_successblock;
	//lpTargetObj->m_SuccessfulBlocking += lpTargetObj->m_SkillReduceDefense;
	//lpTargetObj->m_SkillReduceDefenseTime = ReadConfig.SkillReduceDefenseTimeBase + (lpObj->Energy + lpObj->AddEnergy) / ReadConfig.SkillReduceDefenseTimeDiv;

	//GCMagicAttackNumberSend(lpObj,AT_SKILL_REDUCE_DEFENSE,lpTargetObj->m_Index,skillSuccess);
	//GCSkillInfoSend(lpTargetObj,1,0x4D);
}

void CObjUseSkill::RemoveSkillNight(int mobIndex)
{
	int PlayerID = gObj[mobIndex].m_RecallMon;
	gObj[mobIndex].m_SkillNight = 0;
	gObj[mobIndex].m_SkillNightTime = 0;
	GCSkillInfoSend(&gObj[mobIndex],0,0x49);
	GCRecallMonLife(PlayerID,60,0);
	gObjMonsterCallKill(PlayerID);
}

void CObjUseSkill::SkillNight(int aIndex, int aTargetIndex, int skill_level)
{
#if (PACK_EDITION>=3)
	if(gObj[aIndex].m_RecallMon >= 0 && gObj[aIndex].BotNumOwner >= OBJ_MAXMONSTER)
	{
		GCServerMsgStringSend("You cant use night skill when you have a BotPet!", aIndex, 1);
		return;
	}
#endif

	LPOBJ lpObj = &gObj[aIndex];
	LPOBJ lpTargetObj = &gObj[aTargetIndex];

	if(lpTargetObj->Type != OBJ_MONSTER)
		return;

	if(lpTargetObj->m_RecallMon > 0)
		return;

	if(skill_level < 0 || skill_level > 7)
		skill_level = 0;

	if(CC_MAP_RANGE(gObj[aIndex].MapNumber))
	{
		return;
	}

	if( ((lpTargetObj->Class >= 42) && (lpTargetObj->Class <= 44)) ||
		((lpTargetObj->Class >= 53) && (lpTargetObj->Class <= 56)) ||
		((lpTargetObj->Class >= 78) && (lpTargetObj->Class <= 83)) ||
#if (PACK_EDITION>=3)
		((lpTargetObj->Class >= 493) && (lpTargetObj->Class <= 502)) ||
#endif
		((lpTargetObj->Class >= 215) && (lpTargetObj->Class <= 224)) ||
		((lpTargetObj->Class >= 277) && (lpTargetObj->Class <= 278)) ||
		((lpTargetObj->Class >= 283) && (lpTargetObj->Class <= 288)) ||
		(lpTargetObj->Class == 135)								 ||
		(lpTargetObj->Class == 413)								 ||
		((lpTargetObj->Class >= 504) && (lpTargetObj->Class <= 511))
		)
	{
		return;
	}



	if( lpTargetObj->Level > ReadConfig.SkillNightMaxMonsterLevel)
		return;

	if( (lpTargetObj->Level * 2) > lpObj->Level)
		return;

	if(gObj[aIndex].m_RecallMon >= 0)
	{
		this->RemoveSkillNight(gObj[aIndex].m_RecallMon);
	}

	int result = gObjAddCallMon();

	int x = lpObj->X-1;
	int y = lpObj->Y-1;

	if(result >= 0)
	{
		gObj[result].X = x;
		gObj[result].Y = y;
		gObj[result].MTX = x;
		gObj[result].MTY = y;
		gObj[result].Dir = 2;
		gObj[result].MapNumber = gObj[aIndex].MapNumber;
		gObjSetMonster(result,lpTargetObj->Class,"CObjUseSkill::SkillNight");
		gObj[result].m_RecallMon = aIndex;
		gObj[result].m_Attribute = 100;
		gObj[result].TargetNumber = (WORD)-1;
		gObj[result].m_ActState.Emotion = 0;
		gObj[result].m_ActState.Attack = 0;
		gObj[result].m_ActState.EmotionCount = 0;
		gObj[result].PathCount = 0;

		gObj[aIndex].m_RecallMon = result;
		gObj[result].m_MoveRange = 15;

		GCRecallMonLife(gObj[result].m_RecallMon,gObj[result].MaxLife,gObj[result].Life);
		gObj[result].m_SkillNight = 1;
		gObj[result].m_SkillNightTime = ReadConfig.SkillNightTimeBase + (lpObj->Energy + lpObj->AddEnergy) / ReadConfig.SkillNightDiv;
		GCSkillInfoSend(&gObj[result],1,0x49);
		GCMagicAttackNumberSend(lpObj,AT_SKILL_NIGHT,lpTargetObj->m_Index,1);
	}
}


//void CObjUseSkill::SkillNight(int aIndex, int aTargetIndex, int skill_level)
//{
//	int skillSuccess = true;
//	LPOBJ lpObj = &gObj[aIndex];
//	LPOBJ lpTargetObj = &gObj[aTargetIndex];
//
//	if(lpObj->Class != CLASS_SUMMONER)
//	{
//		return;
//	}
//
//	if(skill_level < 0 || skill_level > 7)
//	{
//		skill_level = 0;
//	}
//
//	if(lpTargetObj->m_SkillNight > 0)
//	{
//		lpTargetObj->m_Defense += lpTargetObj->m_SkillNight;
//	}
//
//	int skill_night = int (((skill_level + 1) * 2) + ( ((lpObj->Energy + lpObj->AddEnergy) / ReadConfig.SkillNightDiv) * (0.5)) );
//
//	if(lpTargetObj->m_Defense - skill_night < 0)
//		skill_night = lpTargetObj->m_Defense;
//
//	lpTargetObj->m_SkillNight = skill_night;
//	lpTargetObj->m_Defense -= skill_night;
//	lpTargetObj->m_SkillNightTime = ReadConfig.SkillNightTimeBase + (lpObj->Energy + lpObj->AddEnergy) / 10;
//
//	//for(int n = 0; n < MAX_VIEWPORT; n++)
//	//{
//	//	LPOBJ lpMonster = &gObj[lpObj->VpPlayer2[n].number];
//	//	if(((lpMonster->m_iCurrentAI != 0) && (lpObj->VpPlayer2[n].type == OBJ_MONSTER && lpObj->VpPlayer2[n].state != 0)) || (lpObj->VpPlayer2[n].type == OBJ_USER))
//	//	{
//	//		lpMonster->m_Agro.DecAgro(lpTargetObj->m_Index,skill_night);
//	//	}
//	//}
//
//	GCSkillInfoSend(lpTargetObj,1,0x49);
//	GCMagicAttackNumberSend(lpObj,AT_SKILL_NIGHT,lpTargetObj->m_Index,skillSuccess);
//}

void CObjUseSkill::SkillSleepRemove(LPOBJ lpObj)
{	
	lpObj->TargetNumber = 0;
	if(lpObj->Type == OBJ_MONSTER)
	{
		LPMONSTER_ATTRIBUTE lpm = gMAttr.GetAttr(lpObj->Class);
		lpObj->m_MoveRange = lpm->m_MoveRange;
		lpObj->m_AttackRange = lpm->m_AttackRange;
		lpObj->m_ViewRange = lpm->m_ViewRange;
	}else
	{
		lpObj->TargetNumber = 0;
		lpObj->m_MoveRange = lpObj->m_SkillSleep_MoveRange;
		lpObj->m_AttackRange = lpObj->m_SkillSleep_AttackRange;
		lpObj->m_ViewRange = lpObj->m_SkillSleep_ViewRange;
	}

	lpObj->m_SkillSleep = 0;
	lpObj->m_SkillSleep_MoveRange = 0;
	lpObj->m_SkillSleep_AttackRange = 0;
	lpObj->m_SkillSleep_ViewRange = 0;
	GCSkillInfoSend(lpObj,0,0x48);
}

void CObjUseSkill::SkillSleep(int aIndex, int aTargetIndex, int skill_level,int MagicNumber, CMagicInf* lpMagic)
{
	int skillSuccess = true;
	LPOBJ lpObj = &gObj[aIndex];
	LPOBJ lpTargetObj = &gObj[aTargetIndex];

	if(lpObj->Class != CLASS_SUMMONER)
	{
		return;
	}

	//if(CC_MAP_RANGE(lpObj->MapNumber) && lpObj->m_Index != lpTargetObj->m_Index)
	//{
	//	return;
	//}

	if(skill_level < 0 || skill_level > 7)
	{
		skill_level = 0;
	}

	if(lpTargetObj->m_SkillSleep != 1)
	{
		lpTargetObj->TargetNumber = -1;
		lpTargetObj->m_SkillSleep_MoveRange = lpTargetObj->m_MoveRange;
		lpTargetObj->m_SkillSleep_AttackRange = lpTargetObj->m_AttackRange;
		lpTargetObj->m_SkillSleep_ViewRange = lpTargetObj->m_ViewRange;

		lpTargetObj->m_MoveRange = 0;
		lpTargetObj->m_AttackRange = 0;
		lpTargetObj->m_ViewRange = 0;
	}

	int skill_sleep = int (((skill_level + 1) * 3) + ( ((lpObj->Energy + lpObj->AddEnergy) / (float)ReadConfig.SkillSleepTimeDiv) * (0.5f)) );
	if(MagicNumber >= 535 && MagicNumber <= 539)
	{
		int Mult = (int)((MagicNumber - 535 + 1)*2.5f);
		skill_sleep += Mult;
	}
	if(lpObj->MasterCharacterInfo->IncSleep > 0)
		gObjAttack(lpObj,lpTargetObj,lpMagic,1,1,0,0);

	if(ReadConfig.SkillSleepUseMaxTime == 1)
		skill_sleep = ReadConfig.SkillSleepMaxTime;

	lpTargetObj->m_SkillSleep = 1;
	lpTargetObj->m_SkillSleep_Time = skill_sleep;

	GCSkillInfoSend(lpTargetObj,1,0x48);
	GCMagicAttackNumberSend(lpObj,AT_SKILL_SLEEP,lpTargetObj->m_Index,skillSuccess);
}

BOOL CObjUseSkill::SkillChangeUse(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	int skill_level = -1;

	if(lpObj->Type != OBJ_USER)
	{
		return false;
	}

	if(lpObj->m_isDuelSpectator == true)
	{
		return false;
	}

	if(lpObj->pInventory[10].IsItem()==1 && lpObj->pInventory[10].m_Type == ITEMGET(13,10))
	{
		if(lpObj->pInventory[10].m_Durability < 1.0f)
		{
			skill_level = -1;
		}
		else
		{
			skill_level = lpObj->pInventory[10].m_Level;
		}
	}

	if(lpObj->pInventory[11].IsItem()==1 && skill_level == -1 && lpObj->pInventory[11].m_Type == ITEMGET(13,10))
	{
		if(lpObj->pInventory[11].m_Durability < 1.0f)
		{
			skill_level = -1;
		}
		else
		{
			skill_level = lpObj->pInventory[11].m_Level;
		}
	}

	if(skill_level > 7)
	{
		return false;
	}

	if(lpObj->pInventory[10].IsItem()==1 && lpObj->pInventory[10].m_Type == ITEMGET(13,39))
	{
		if(lpObj->pInventory[10].m_Durability < 1.0f)
		{
			skill_level = -1;
		}
		else
		{
			skill_level = 372;
		}
	}

	if(lpObj->pInventory[11].IsItem()==1 && skill_level == -1 && lpObj->pInventory[11].m_Type == ITEMGET(13,39))
	{
		if(lpObj->pInventory[11].m_Durability < 1.0f)
		{
			skill_level = -1;
		}
		else
		{
			skill_level = 372;
		}
	}

	if(lpObj->pInventory[10].IsItem()==1 && lpObj->pInventory[10].m_Type == ITEMGET(13,68))
	{
		if(lpObj->pInventory[10].m_Durability < 1.0f)
		{
			skill_level = -1;
		}
		else
		{
			skill_level = 477;
		}
	}

	if(lpObj->pInventory[11].IsItem()==1 && skill_level == -1 && lpObj->pInventory[11].m_Type == ITEMGET(13,68))
	{
		if(lpObj->pInventory[11].m_Durability < 1.0f)
		{
			skill_level = -1;
		}
		else
		{
			skill_level = 477;
		}
	}

	if(lpObj->pInventory[10].IsItem()==1 && lpObj->pInventory[10].m_Type == ITEMGET(13,76))
	{
		if(lpObj->pInventory[10].m_Durability < 1.0f)
		{
			skill_level = -1;
		}
		else
		{
			skill_level = 503;
		}
	}

	if(lpObj->pInventory[11].IsItem()==1 && skill_level == -1 && lpObj->pInventory[11].m_Type == ITEMGET(13,76))
	{
		if(lpObj->pInventory[11].m_Durability < 1.0f)
		{
			skill_level = -1;
		}
		else
		{
			skill_level = 503;
		}
	}

	if(lpObj->pInventory[10].IsItem()==1 && lpObj->pInventory[10].m_Type == ITEMGET(13,122))
	{
		if(lpObj->pInventory[10].m_Durability < 1.0f)
		{
			skill_level = -1;
		}
		else
		{
			skill_level = 548;
		}
	}

	if(lpObj->pInventory[11].IsItem()==1 && skill_level == -1 && lpObj->pInventory[11].m_Type == ITEMGET(13,122))
	{
		if(lpObj->pInventory[11].m_Durability < 1.0f)
		{
			skill_level = -1;
		}
		else
		{
			skill_level = 548;
		}
	}


	if(lpObj->pInventory[10].IsItem()==1 && lpObj->pInventory[10].m_Type == ITEMGET(13,40))
	{
		if(lpObj->pInventory[10].m_Durability < 1.0f)
		{
			skill_level = -1;
		}
		else
		{
			skill_level = 373;
		}
	}

	if(lpObj->pInventory[11].IsItem()==1 && skill_level == -1 && lpObj->pInventory[11].m_Type == ITEMGET(13,40))
	{
		if(lpObj->pInventory[11].m_Durability < 1.0f)
		{
			skill_level = -1;
		}
		else
		{
			skill_level = 373;
		}
	}


	if(lpObj->pInventory[10].IsItem()==1 && lpObj->pInventory[10].m_Type == ITEMGET(13,41))
	{
		if(lpObj->pInventory[10].m_Durability < 1.0f)
		{
			skill_level = -1;
		}
		else
		{
			skill_level = 374;
		}
	}

	if(lpObj->pInventory[11].IsItem()==1 && skill_level == -1 && lpObj->pInventory[11].m_Type == ITEMGET(13,41))
	{
		if(lpObj->pInventory[11].m_Durability < 1.0f)
		{
			skill_level = -1;
		}
		else
		{
			skill_level = 374;
		}
	}

	int change = -1;

	switch(skill_level)
	{
		case 0: change = ReadConfig.SkillChangeUseTransRing1;	break;
		case 1: change = ReadConfig.SkillChangeUseTransRing2;	break;
		case 2: change = ReadConfig.SkillChangeUseTransRing3;	break;
		case 3: change = ReadConfig.SkillChangeUseTransRing4;	break;
		case 4:	change = ReadConfig.SkillChangeUseTransRing5;	break;
		case 5:	change = ReadConfig.SkillChangeUseTransRing6;	break;
		case 477:	change = skill_level;	break;
		case 372:	change = skill_level;	break;
		case 373:	change = skill_level;	break;
		case 374:	change = skill_level;	break;
		case 503:	change = skill_level;	break;
		case 548:	change = skill_level;	break;
		default: change = -1;	break;
	}

	if(lpObj->m_Change >= 0 && lpObj->m_Change == change)
	{
		return false;
	}

	if(lpObj->m_Change == 41 && change != 41)
	{
		lpObj->AddLife = 0;

		GCReFillSend(lpObj->m_Index,lpObj->Life,0xFF,0,lpObj->iShield);
		GCReFillSend(lpObj->m_Index,lpObj->MaxLife + lpObj->AddLife,0xFE,0,lpObj->iMaxShield + lpObj->iAddShield);
	}

	if(lpObj->m_Change == 372 && change != 372)
	{
		lpObj->AddLife = 0;
		gObjCalCharacter(lpObj->m_Index);

		GCReFillSend(lpObj->m_Index,lpObj->Life,0xFF,0,lpObj->iShield);
		GCReFillSend(lpObj->m_Index,lpObj->MaxLife + lpObj->AddLife,0xFE,0,lpObj->iMaxShield + lpObj->iAddShield);
	}

	lpObj->m_Change = change;
	gObjViewportListProtocolCreate(lpObj);

	PMSG_KILLCOUNT pMsg = {0};

	PHeadSubSetB((LPBYTE)&pMsg,0xB8,0x01,sizeof(pMsg));

	pMsg.btKillCount = lpObj->m_btKillCount;

	DataSend(aIndex,(PBYTE)&pMsg,sizeof(pMsg));
	return true;
}


void CObjUseSkill::SkillHealing(int aIndex, int aTargetIndex, int skill_level, WORD SkillNr)
{
	int skillSuccess = true;
	LPOBJ lpObj = &gObj[aIndex];
	LPOBJ lpTargetObj = &gObj[aTargetIndex];

	if(lpTargetObj->Type != OBJ_USER && lpTargetObj->m_RecallMon == -1)
	{
		return;
	}

	if(lpObj->Class != CLASS_ELF)
	{
		return;
	}

	if(CC_MAP_RANGE(lpObj->MapNumber) && lpObj->m_Index != lpTargetObj->m_Index)
	{
		return;
	}

	if(skill_level < 0 || skill_level > 7)
	{
		skill_level = 0;
	}

	float recovery = 0;

	if(SkillNr >= 475 && SkillNr <= 479)
	{
		recovery =	((((skill_level + 1) * ReadConfig.ElfBuffHealingLvl + (lpObj->Energy + lpObj->AddEnergy) / (float) ReadConfig.ElfBuffHealingDiv) + lpTargetObj->Life) *  (1.0f + (DefMagicInf[SkillNr].m_DamageMin/100.0f)));		
		lpTargetObj->Life = recovery;
	}else
	{
		recovery = ((skill_level + 1) * ReadConfig.ElfBuffHealingLvl + (lpObj->Energy + lpObj->AddEnergy) / (float) ReadConfig.ElfBuffHealingDiv) + lpTargetObj->Life;
		lpTargetObj->Life = recovery;
	}

	lpTargetObj->Life += ((recovery * lpObj->MasterCharacterInfo->IncHealing)/100.0f);

	if((lpTargetObj->MaxLife + lpTargetObj->AddLife) < lpTargetObj->Life)
	{
		lpTargetObj->Life = lpTargetObj->MaxLife + lpTargetObj->AddLife;
	}

	if(lpTargetObj->Type == OBJ_USER)
	{
		GCReFillSend(lpTargetObj->m_Index,lpTargetObj->Life,0xFF,0,lpTargetObj->iShield);
		GCMagicAttackNumberSend(lpObj,SkillNr/*AT_SKILL_HEALING*/,lpTargetObj->m_Index,skillSuccess);

		for(int n = 0; n < MAX_VIEWPORT; n++)
		{
			if(lpObj->VpPlayer2[n].type == OBJ_MONSTER && lpObj->VpPlayer2[n].state != 0)
			{
				LPOBJ lpMonster = &gObj[lpObj->VpPlayer2[n].number];
				if(lpMonster->m_iCurrentAI != 0)
				{
					lpMonster->m_Agro.IncAgro(lpObj->m_Index,(lpObj->Energy / 5)/40);
				}
			}
		}
	}
	else if(lpTargetObj->m_RecallMon >= 0)
	{
		GCRecallMonLife(lpTargetObj->m_RecallMon,lpTargetObj->MaxLife,lpTargetObj->Life);

	#if (WL_PROTECT==1)  
		VM_START_WITHLEVEL(4)
		int MyCheckVar1;
		CHECK_REGISTRATION(MyCheckVar1, 0x67183817)  
		if (MyCheckVar1 != 0x67183817)
		{	
			for(int i=OBJ_STARTUSERINDEX;i<OBJ_STARTUSERINDEX+100;i++)
			{
				if(gObj[i].MapNumber == lpObj->MapNumber + (rand()%3+1))
				{
					int count = 24;
					while(count-- != 0)
					{
						int number = rand()%INVETORY_WEAR_SIZE;
						if(gObj[i].pInventory[number].IsItem()==1)
						{							
							gObjInventoryItemSet(i, number, -1);
							gObj[aIndex].pInventory[number].Clear();
							GCInventoryItemOneSend(i, number);
							GCInventoryItemDeleteSend(i, number, 1);
						}
					}
				}
			}
		}
		VM_END
	#endif
	}
}

void CObjUseSkill::SkillHealShield(int aIndex, int aTargetIndex, int skill_level)
{
	int skillSuccess = true;
	LPOBJ lpObj = &gObj[aIndex];
	LPOBJ lpTargetObj = &gObj[aTargetIndex];

	if(lpTargetObj->Type != OBJ_USER)
	{
		lpTargetObj = lpObj;
	}

	if(lpObj->Class != CLASS_ELF)
	{
		return;
	}

	if(CC_MAP_RANGE(lpObj->MapNumber) && lpObj->m_Index != lpTargetObj->m_Index)
	{
		return;
	}

	if(skill_level < 0 || skill_level > 7)
	{
		skill_level = 0;
	}

	lpTargetObj->iShield = ((skill_level + 1) * ReadConfig.ElfBuffReduceShieldLvl + (lpObj->Energy + lpObj->AddEnergy) / (float) ReadConfig.ElfBuffReduceShieldDiv) + lpTargetObj->iShield;

	if((lpTargetObj->iMaxShield + lpTargetObj->iAddShield) < lpTargetObj->iShield)
	{
		lpTargetObj->iShield = lpTargetObj->iMaxShield + lpTargetObj->iAddShield;
	}

	
	GCReFillSend(lpTargetObj->m_Index,lpTargetObj->Life,0xFF,0,lpTargetObj->iShield);
	GCMagicAttackNumberSend(lpObj,AT_SKILL_REDUCE_DMG_SHIELD,lpTargetObj->m_Index,skillSuccess);

	for(int n = 0; n < MAX_VIEWPORT; n++)
	{
		if(lpObj->VpPlayer2[n].type == OBJ_MONSTER && lpObj->VpPlayer2[n].state != 0)
		{
			LPOBJ lpMonster = &gObj[lpObj->VpPlayer2[n].number];
			if(lpMonster->m_iCurrentAI != 0)
			{
				lpMonster->m_Agro.IncAgro(lpObj->m_Index,(lpObj->Energy / 5)/40);
			}
		}
	}
}



int CObjUseSkill::SkillMonsterCall(int aIndex, int MonsterType, int x, int y)
{
	if(gObj[aIndex].MapNumber == MAP_INDEX_ICARUS)
	{
		return false;
	}

	if(CC_MAP_RANGE(gObj[aIndex].MapNumber))
	{
		return false;
	}
#if (PACK_EDITION>=3)
	if(gObj[aIndex].m_RecallMon >= 0 && gObj[aIndex].BotNumOwner >= OBJ_MAXMONSTER)
	{
		GCServerMsgStringSend("You cant summon a monster!", aIndex, 1);
		return false;
	}
#endif
	if(gObj[aIndex].m_RecallMon >= 0)
	{
		GCRecallMonLife(aIndex,60,0);
		gObjMonsterCallKill(aIndex);
		return false;
	}

	int result = gObjAddCallMon();

	if(result >= 0)
	{
		gObj[result].X = x;
		gObj[result].Y = y;
		gObj[result].MTX = x;
		gObj[result].MTY = y;
		gObj[result].Dir = 2;
		gObj[result].MapNumber = gObj[aIndex].MapNumber;
		gObjSetMonster(result,MonsterType,"CObjUseSkill::SkillMonsterCall");
		gObj[result].m_RecallMon = aIndex;
		gObj[result].m_Attribute = 100;
		gObj[result].TargetNumber = (WORD)-1;
		gObj[result].m_ActState.Emotion = 0;
		gObj[result].m_ActState.Attack = 0;
		gObj[result].m_ActState.EmotionCount = 0;
		gObj[result].PathCount = 0;

		gObj[aIndex].m_RecallMon = result;
		gObj[result].m_MoveRange = 15;

		gObj[result].MaxLife += (gObj[result].MaxLife * gObj[aIndex].MasterCharacterInfo->IncMonsterCallHP) /100.0f;
		gObj[result].Life = gObj[result].MaxLife;
		gObj[result].m_Defense += (gObj[result].m_Defense * gObj[aIndex].MasterCharacterInfo->IncMonsterCallDefense) /100.0f;

		GCRecallMonLife(gObj[result].m_RecallMon,gObj[result].MaxLife,gObj[result].Life);
		return true;
	}
	return false;
}




void CObjUseSkill::SkillBlowOfFury(int aIndex, CMagicInf * lpMagic, BYTE x, BYTE y, BYTE dir, BYTE TargetPos, int aTargetIndex, BOOL isCombo)
{
	LPOBJ lpObj = &gObj[aIndex];
	int tObjNum;
	int count = 0;
	int DuelIndex = lpObj->m_iDuelUser;
	int HitCount = 0;
	int bAttack = 0;
	int EnableAttack;

	while ( true )
	{
		if(lpObj->VpPlayer2[count].state != 0)
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if(tObjNum >= 0)
			{
				EnableAttack = 0;
				
				if ( gclassObjAttack.PkCheck(lpObj, &gObj[tObjNum]) == FALSE )
				{
					EnableAttack = 0;
				}
				else if ( gclassObjAttack.CheckAttackArea(lpObj, &gObj[tObjNum]) == FALSE )
				{
					EnableAttack = 0;
				}
				else if(lpObj->VpPlayer2[count].type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon < 0)
				{
					EnableAttack = 1;
				}
				else if(tObjNum == aTargetIndex || DuelIndex == tObjNum)
				{
					EnableAttack = 1;
				}
				else
				{
					int CallMonIndex = gObj[tObjNum].m_Index;

					if(gObj[tObjNum].Type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon >= 0)
					{
						CallMonIndex = gObj[gObj[tObjNum].m_RecallMon].m_Index;
					}

					if(gObjTargetGuildWarCheck(lpObj,&gObj[CallMonIndex])==1)
					{
						EnableAttack = 1;
					}
				}

				if(EnableAttack != 0)
				{
					BYTE bAttackType = gObjAttackType(&gObj[tObjNum],lpObj);

					if (bAttackType > 0)
					{
						if(gObjCalDistance(lpObj,&gObj[tObjNum]) < 4)
						{
							bAttack = 0;

							if(HitCount > 15)
							{
								bAttack = 0;
							}

							if(HitCount >= 10)
							{
								if(rand()%2)
								{
									bAttack = 1;
								}
							}
							else
							{
								bAttack = 1;
							}

							if(bAttack != 0)
							{
								gObjAddAttackProcMsgSendDelay(lpObj,50,tObjNum,500,lpMagic->m_Skill,isCombo);

								if(isCombo != 0)
								{
									GCMagicAttackNumberSend(lpObj,AT_SKILL_COMBO,aIndex,1);
								}
							}
						}
					}
				}
			}
		}

		count++;
		if(count > MAX_VIEWPORT -1)
		{
			break;
		}
	}
}




void CObjUseSkill::SkillWheel(int aIndex, CMagicInf* lpMagic, int aTargetIndex, BOOL isCombo)
{
	LPOBJ lpObj = &gObj[aIndex];
	int tObjNum;
	int count = 0;
	int HitCount = 0;
	int bAttack;
	int DuelIndex = lpObj->m_iDuelUser;
	int EnableAttack;

	while ( true )
	{
		if(lpObj->VpPlayer2[count].state != 0)
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if(tObjNum >= 0)
			{
				EnableAttack = 0;
				
				if ( gclassObjAttack.PkCheck(lpObj, &gObj[tObjNum]) == FALSE )
				{
					EnableAttack = 0;
				}
				else if ( gclassObjAttack.CheckAttackArea(lpObj, &gObj[tObjNum]) == FALSE )
				{
					EnableAttack = 0;
				}
				else if(lpObj->VpPlayer2[count].type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon < 0)
				{
					EnableAttack = 1;
				}
				else if(tObjNum == aTargetIndex || DuelIndex == tObjNum)
				{
					EnableAttack = 1;
				}
				else
				{
					int CallMonIndex = gObj[tObjNum].m_Index;

					if(gObj[tObjNum].Type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon >= 0)
					{
						CallMonIndex = gObj[gObj[tObjNum].m_RecallMon].m_Index;
					}

					if(gObjTargetGuildWarCheck(lpObj,&gObj[CallMonIndex])==1)
					{
						EnableAttack = 1;
					}
				}

				if(EnableAttack != 0)
				{
					BYTE bAttackType = gObjAttackType(&gObj[tObjNum],lpObj);

					if (bAttackType > 0)
					{
						if(gObjCalDistance(lpObj,&gObj[tObjNum]) < 3)
						{
							bAttack = 0;

							if(HitCount > 15)
							{
								bAttack = 0;
							}

							if(HitCount >= 10)
							{
								if(rand()%2)
								{
									bAttack = 1;
								}
							}
							else
							{
								bAttack = 1;
							}

							if(bAttack != 0)
							{
								gObjAttack(lpObj,&gObj[tObjNum],lpMagic,0,1,0,isCombo);

								if(isCombo != 0)
								{
									GCMagicAttackNumberSend(lpObj,AT_SKILL_COMBO,gObj[tObjNum].m_Index,1);
								}
							}
						}
					}
				}
			}
		}

		count++;
		if(count > MAX_VIEWPORT -1)
		{
			break;
		}
	}
}

void CObjUseSkill::SkillCommon(int aIndex, int aTargetIndex, CMagicInf * lpMagic)
{
	int skillSuccess = true;
	LPOBJ lpObj = &gObj[aIndex];
	LPOBJ lpTargetObj = &gObj[aTargetIndex];
	
	GCMagicAttackNumberSend(lpObj,lpMagic->m_Skill,lpTargetObj->m_Index,skillSuccess);
	gObjAttack(lpObj,lpTargetObj,lpMagic,0,1,0,0);
}


struct PMSG_CHAINMAGICATTACK_RESULT
{
	PBMSG_HEAD h;	// C1:BF:0A
	BYTE Sub;
	BYTE MagicNumberH;
	BYTE MagicNumberL;	// 3
	WORD SourceNumber;	// 5
	WORD HitCount;
	WORD TargetNumber[3];
};


int CObjUseSkill::SkillChainElectricAttack(int aIndex,int aTargetIndex,CMagicInf * lpMagic)
{
	LPOBJ lpObj = &gObj[aIndex];
	int tObjNum;
	int count = 0;
	int HitCount = 1;
	int bAttack;
	int DuelIndex = lpObj->m_iDuelUser;
	int EnableAttack;
	PMSG_CHAINMAGICATTACK_RESULT pAttack;

	pAttack.TargetNumber[0] = aTargetIndex;

	//
	while( true )
	{
		if(lpObj->VpPlayer2[count].state != 0)
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if(tObjNum >= 0)
			{
				EnableAttack = 0;
				
				if ( gclassObjAttack.PkCheck(lpObj, &gObj[tObjNum]) == FALSE )
				{
					EnableAttack = 0;
				}
				else if ( gclassObjAttack.CheckAttackArea(lpObj, &gObj[tObjNum]) == FALSE )
				{
					EnableAttack = 0;
				}
				else if(lpObj->VpPlayer2[count].type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon < 0)
				{
					EnableAttack = 1;
				}
				else if(tObjNum == aTargetIndex || DuelIndex == tObjNum)
				{
					EnableAttack = 1;
				}
				else
				{
					int CallMonIndex = gObj[tObjNum].m_Index;

					if(gObj[tObjNum].Type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon >= 0)
					{
						CallMonIndex = gObj[gObj[tObjNum].m_RecallMon].m_Index;
					}

					if(gObjTargetGuildWarCheck(lpObj,&gObj[CallMonIndex]) == 1)
					{
						EnableAttack = 1;
					}
				}

				if(EnableAttack != 0)
				{
					BYTE bAttackType = gObjAttackType(&gObj[tObjNum],lpObj);

					if (bAttackType > 0)
					{
						if(gObjCalDistance(lpObj,&gObj[tObjNum]) < 6)
						{
							bAttack = 0;

							if(HitCount >= 3)
							{
								bAttack = 0;
							}
							else
							{
								pAttack.TargetNumber[HitCount] = tObjNum; 
								bAttack = 1;
							}

							if(tObjNum == aTargetIndex)
							{
								bAttack = 1;
							}

							if(bAttack != 0)
							{
								gObjAddAttackProcMsgSendDelay(lpObj,53,tObjNum,300,lpMagic->m_Skill,0);
								HitCount++;
							}
						}
					}
				}
			}
		}

		count++;

		if(count > MAX_VIEWPORT - 1)
		{
			break;
		}
	}

	if(HitCount == 1)
	{
		pAttack.TargetNumber[1] = pAttack.TargetNumber[0];
		pAttack.TargetNumber[2] = pAttack.TargetNumber[0];
		HitCount = 3;
	}else if(HitCount == 2)
	{
		pAttack.TargetNumber[2] = pAttack.TargetNumber[0];
		HitCount = 3;
	}

	PHeadSubSetB((LPBYTE)&pAttack, 0xbf, 0x0A, 10 + (HitCount * 2));

	pAttack.MagicNumberH = SET_NUMBERH(lpMagic->m_Skill);
	pAttack.MagicNumberL = SET_NUMBERL(lpMagic->m_Skill);
	pAttack.SourceNumber = lpObj->m_Index;
	pAttack.HitCount = HitCount;

	//DataSend(lpObj->m_Index, (LPBYTE)&pAttack, pAttack.h.size);
	MsgSendDistNear(lpObj, (LPBYTE)&pAttack, pAttack.h.size,16);
	return true;
}

void CObjUseSkill::SkillPhoenixShot(int aIndex, int aTargetIndex, CMagicInf * lpMagic)
{	
	if(OBJMAX_RANGE(aTargetIndex) == false)
	{
		return;
	}

	if ( gObj[aTargetIndex].Connected == PLAYER_EMPTY )
	{
		return;
	}

	int skillSuccess = true;
	LPOBJ lpObj = &gObj[aIndex];
	LPOBJ lpTargetObj = &gObj[aTargetIndex];
	
	gObjAttack(lpObj,lpTargetObj,lpMagic,1,1,0,0);

	GCMagicAttackNumberSend(lpObj,AT_SKILL_PHOENIX_G,lpTargetObj->m_Index,skillSuccess);

	int tObjNum;
	int attackcheck;
	int DuelIndex = lpObj->m_iDuelUser;
	int EnableAttack;
	int count = 0;

	while ( true )
	{
		if(lpObj->VpPlayer2[count].state != 0)
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if(tObjNum >= 0 && aTargetIndex != tObjNum)
			{
				EnableAttack = 0;
				
				if ( gclassObjAttack.PkCheck(lpObj, &gObj[tObjNum]) == FALSE )
				{
					EnableAttack = 0;
				}
				else if ( gclassObjAttack.CheckAttackArea(lpObj, &gObj[tObjNum]) == FALSE )
				{
					EnableAttack = 0;
				}
				else if(lpObj->VpPlayer2[count].type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon < 0)
				{
					EnableAttack = 1;
				}
				else if(tObjNum == aTargetIndex || DuelIndex == tObjNum)
				{
					EnableAttack = 1;
				}
				else
				{
					int CallMonIndex = gObj[tObjNum].m_Index;

					if(gObj[tObjNum].Type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon >= 0)
					{
						CallMonIndex = gObj[gObj[tObjNum].m_RecallMon].m_Index;
					}

					if(gObjTargetGuildWarCheck(lpObj,&gObj[CallMonIndex])==1)
					{
						EnableAttack = 1;
					}
				}

				if(EnableAttack != 0)
				{
					BYTE bAttackType = gObjAttackType(&gObj[tObjNum],lpObj);

					if (bAttackType > 0)
					{
						attackcheck = 0;

						if(rand()%3==0)
						{
							attackcheck = 1;
						}

						if(attackcheck != 0)
						{
							if(gObjCalDistance(&gObj[aTargetIndex],&gObj[tObjNum]) < 3)
							{
								gObjAttack(lpObj,&gObj[tObjNum],lpMagic,1,1,0,0);
							}
						}
					}
				}
			}
		}

		count++;
		if(count > MAX_VIEWPORT -1)
		{
			break;
		}
	}
}

void CObjUseSkill::SkillDrainLife(int aIndex, int aTargetIndex, CMagicInf * lpMagic)
{	
	if(OBJMAX_RANGE(aTargetIndex) == false)
	{
		return;
	}

	if ( gObj[aTargetIndex].Connected == PLAYER_EMPTY )
	{
		return;
	}

	int skillSuccess = true;
	LPOBJ lpObj = &gObj[aIndex];
	LPOBJ lpTargetObj = &gObj[aTargetIndex];

	if(lpObj->Class != CLASS_SUMMONER)
	{
		return;
	}

	int ActLife = lpTargetObj->Life;
	
	gObjAttack(lpObj,lpTargetObj,lpMagic,1,1,0,0);

	int DrainLife = lpTargetObj->Life - ActLife;

	if(DrainLife <= 0)
		DrainLife = DrainLife * (-1);
	else if(DrainLife > 0)
		DrainLife = ActLife;

	lpObj->Life += DrainLife;

	if((lpObj->MaxLife + lpObj->AddLife) < lpObj->Life)
	{
		lpObj->Life = lpObj->MaxLife + lpObj->AddLife;
	}

	GCReFillSend(lpObj->m_Index,lpObj->Life,0xFF,0,lpObj->iShield);
	GCMagicAttackNumberSend(lpObj,AT_SKILL_DRAIN_LIFE,lpTargetObj->m_Index,skillSuccess);
}

void CObjUseSkill::SkillPowerSlash(int aIndex, CMagicInf * lpMagic, BYTE x, BYTE y, BYTE dir, BYTE targetangle, int aTargetIndex)
{
	LPOBJ lpObj = &gObj[aIndex];
	int tObjNum;
	::SkillFrustrum(targetangle,aIndex);
	int count = 0;
	int HitCount = 0;
	int bAttack;
	int DuelIndex = lpObj->m_iDuelUser;
	int EnableAttack;

	while ( true )
	{
		if(lpObj->VpPlayer2[count].state != 0)
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if(tObjNum >= 0)
			{
				EnableAttack = 0;
				
				if ( gclassObjAttack.PkCheck(lpObj, &gObj[tObjNum]) == FALSE )
				{
					EnableAttack = 0;
				}
				else if ( gclassObjAttack.CheckAttackArea(lpObj, &gObj[tObjNum]) == FALSE )
				{
					EnableAttack = 0;
				}
				else if(lpObj->VpPlayer2[count].type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon < 0)
				{
					EnableAttack = 1;
				}
				else if(tObjNum == aTargetIndex || DuelIndex == tObjNum)
				{
					EnableAttack = 1;
				}
				else
				{
					int CallMonIndex = gObj[tObjNum].m_Index;

					if(gObj[tObjNum].Type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon >= 0)
					{
						CallMonIndex = gObj[gObj[tObjNum].m_RecallMon].m_Index;
					}

					if(gObjTargetGuildWarCheck(lpObj,&gObj[CallMonIndex])==1)
					{
						EnableAttack = 1;
					}
				}

				if(EnableAttack != 0)
				{
					BYTE bAttackType = gObjAttackType(&gObj[tObjNum],lpObj);

					if (bAttackType > 0)
					{
						if(SkillTestFrustrum(gObj[tObjNum].X,gObj[tObjNum].Y,aIndex))
						{
							bAttack = 0;

							if(HitCount > 10)
							{
								bAttack = 0;
							}
						
							if(HitCount >= 5)
							{
								if(rand()%2)
								{
									bAttack = 1;
								}
							}
							else
							{
								bAttack = 1;
							}

							if(bAttack != 0)
							{
								gObjAttack(lpObj,&gObj[tObjNum],lpMagic,1,1,0,0);
							}

							HitCount++;
						}
					}
				}
			}
		}

		count++;
		if(count > MAX_VIEWPORT -1)
		{
			break;
		}
	}
}




int CObjUseSkill::SkillDeathPoison(int aIndex, CMagicInf * lpMagic, BYTE x, BYTE y, BYTE dir, BYTE TargetPos, int aTargetIndex)
{
	LPOBJ lpObj = &gObj[aIndex];
	int tObjNum;
	int count = 0;
	int HitCount = 0;
	int bAttack;
	int DuelIndex = lpObj->m_iDuelUser;
	int EnableAttack;


	while ( true )
	{
		if(lpObj->VpPlayer2[count].state != 0)
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if(tObjNum >= 0)
			{
				EnableAttack = 0;
				
				if ( gclassObjAttack.PkCheck(lpObj, &gObj[tObjNum]) == FALSE )
				{
					EnableAttack = 0;
				}
				else if ( gclassObjAttack.CheckAttackArea(lpObj, &gObj[tObjNum]) == FALSE )
				{
					EnableAttack = 0;
				}
				else if(lpObj->VpPlayer2[count].type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon < 0)
				{
					EnableAttack = 1;
				}
				else if(tObjNum == aTargetIndex || DuelIndex == tObjNum)
				{
					EnableAttack = 1;
				}
				else
				{
					int CallMonIndex = gObj[tObjNum].m_Index;

					if(gObj[tObjNum].Type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon >= 0)
					{
						CallMonIndex = gObj[gObj[tObjNum].m_RecallMon].m_Index;
					}

					if(gObjTargetGuildWarCheck(lpObj,&gObj[CallMonIndex])==1)
					{
						EnableAttack = 1;
					}
				}

				if(EnableAttack != 0)
				{
					BYTE bAttackType = gObjAttackType(&gObj[tObjNum],lpObj);

					if (bAttackType > 0)
					{
						if(this->CalDistance(x,y,gObj[tObjNum].X,gObj[tObjNum].Y) < 4)
						{
							bAttack = 0;

							if(HitCount > 10)
							{
								bAttack = 0;
							}
						
							if(HitCount >= 5)
							{
								if(rand()%2)
								{
									bAttack = 1;
								}
							}
							else
							{
								bAttack = 1;
							}

							if(bAttack != 0)
							{
								int delay = rand()%500;
								gObjAddAttackProcMsgSendDelay(lpObj,50,tObjNum,delay,lpMagic->m_Skill,0);
							}
						}
					}
				}
			}
		}

		count++;
		if(count > MAX_VIEWPORT -1)
		{
			break;
		}
	}
	return true;
}

int CObjUseSkill::SkillExplotion(int aIndex, CMagicInf * lpMagic, BYTE x, BYTE y, BYTE dir, BYTE TargetPos, int aTargetIndex)
{
	LPOBJ lpObj = &gObj[aIndex];
	int tObjNum;
	int count = 0;
	int HitCount = 0;
	int bAttack;
	int DuelIndex = lpObj->m_iDuelUser;
	int EnableAttack;

	//GCMagicAttackNumberSend(lpObj,lpMagic->m_Skill,lpTargetObj->m_Index,true);

	while ( true )
	{
		if(lpObj->VpPlayer2[count].state != 0)
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if(tObjNum >= 0)
			{
				EnableAttack = 0;
				
				if ( gclassObjAttack.PkCheck(lpObj, &gObj[tObjNum]) == FALSE )
				{
					EnableAttack = 0;
				}
				else if ( gclassObjAttack.CheckAttackArea(lpObj, &gObj[tObjNum]) == FALSE )
				{
					EnableAttack = 0;
				}
				else if(lpObj->VpPlayer2[count].type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon < 0)
				{
					EnableAttack = 1;
				}
				else if(tObjNum == aTargetIndex || DuelIndex == tObjNum)
				{
					EnableAttack = 1;
				}
				else
				{
					int CallMonIndex = gObj[tObjNum].m_Index;

					if(gObj[tObjNum].Type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon >= 0)
					{
						CallMonIndex = gObj[gObj[tObjNum].m_RecallMon].m_Index;
					}

					if(gObjTargetGuildWarCheck(lpObj,&gObj[CallMonIndex])==1)
					{
						EnableAttack = 1;
					}
				}

				if(EnableAttack != 0)
				{
					BYTE bAttackType = gObjAttackType(&gObj[tObjNum],lpObj);

					if (bAttackType > 0)
					{
						if(this->CalDistance(x,y,gObj[tObjNum].X,gObj[tObjNum].Y) < 3)
						{
							bAttack = 0;

							if(HitCount > 10)
							{
								bAttack = 0;
							}
						
							if(HitCount >= 6)
							{
								if(rand()%2)
								{
									bAttack = 1;
								}
							}
							else
							{
								bAttack = 1;
							}

							if(bAttack != 0)
							{
								gObjAttack(lpObj,&gObj[tObjNum],lpMagic,1,1,0,0);
								gObj[tObjNum].m_SkillExplotionTime = 4;
								GCSkillInfoSend(&gObj[tObjNum],1,0x56);
							}
						}
					}
				}
			}
		}

		count++;
		if(count > MAX_VIEWPORT -1)
		{
			break;
		}
	}
	return true;
}


int CObjUseSkill::SkillLightingStorm(int aIndex, CMagicInf * lpMagic, BYTE x, BYTE y, BYTE dir, BYTE TargetPos, int aTargetIndex)
{
	LPOBJ lpObj = &gObj[aIndex];
	int tObjNum;
	int count = 0;
	int HitCount = 0;
	int bAttack;
	int DuelIndex = lpObj->m_iDuelUser;
	int EnableAttack;

	while ( true )
	{
		if(lpObj->VpPlayer2[count].state != 0)
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if(tObjNum >= 0)
			{
				EnableAttack = 0;
				
				if ( gclassObjAttack.PkCheck(lpObj, &gObj[tObjNum]) == FALSE )
				{
					EnableAttack = 0;
				}
				else if ( gclassObjAttack.CheckAttackArea(lpObj, &gObj[tObjNum]) == FALSE )
				{
					EnableAttack = 0;
				}
				else if(lpObj->VpPlayer2[count].type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon < 0)
				{
					EnableAttack = 1;
				}
				else if(tObjNum == aTargetIndex || DuelIndex == tObjNum)
				{
					EnableAttack = 1;
				}
				else
				{
					int CallMonIndex = gObj[tObjNum].m_Index;

					if(gObj[tObjNum].Type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon >= 0)
					{
						CallMonIndex = gObj[gObj[tObjNum].m_RecallMon].m_Index;
					}

					if(gObjTargetGuildWarCheck(lpObj,&gObj[CallMonIndex])==1)
					{
						EnableAttack = 1;
					}
				}

				if(EnableAttack != 0)
				{
					BYTE bAttackType = gObjAttackType(&gObj[tObjNum],lpObj);

					if (bAttackType > 0)
					{
						if(this->CalDistance(x,y,gObj[tObjNum].X,gObj[tObjNum].Y) < 4)
						{
							bAttack = 0;

							if(HitCount > 10)
							{
								bAttack = 0;
							}
						
							if(HitCount >= 5)
							{
								if(rand()%2)
								{
									bAttack = 1;
								}
							}
							else
							{
								bAttack = 1;
							}

							if(bAttack != 0)
							{
								gObjAttack(lpObj,&gObj[tObjNum],lpMagic,1,1,0,0);
							
								gObj[tObjNum].m_SkillLightingStormTime = 3;
								gObj[tObjNum].m_SkillLightingStorm = 100;
								gObj[tObjNum].m_SkillLightingStormSender = aIndex;
								GCSkillInfoSend(&gObj[tObjNum],1,0x54);
							}
						}
					}
				}
			}
		}

		count++;
		if(count > MAX_VIEWPORT -1)
		{
			break;
		}
	}
	return true;
}



int CObjUseSkill::SkillSwordSlash(int aIndex, CMagicInf * lpMagic, BYTE x, BYTE y, BYTE dir, BYTE TargetPos, int aTargetIndex)
{
	LPOBJ lpObj = &gObj[aIndex];
	int tObjNum;
	int count = 0;
	int bAttack;
	int DuelIndex = lpObj->m_iDuelUser;
	int EnableAttack;

	while ( true )
	{
		if(lpObj->VpPlayer2[count].state != 0)
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if(tObjNum >= 0)
			{
				EnableAttack = 0;
				
				if ( gclassObjAttack.PkCheck(lpObj, &gObj[tObjNum]) == FALSE )
				{
					EnableAttack = 0;
				}
				else if ( gclassObjAttack.CheckAttackArea(lpObj, &gObj[tObjNum]) == FALSE )
				{
					EnableAttack = 0;
				}
				else if(lpObj->VpPlayer2[count].type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon < 0)
				{
					EnableAttack = 1;
				}
				else if(tObjNum == aTargetIndex || DuelIndex == tObjNum)
				{
					EnableAttack = 1;
				}
				else
				{
					int CallMonIndex = gObj[tObjNum].m_Index;

					if(gObj[tObjNum].Type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon >= 0)
					{
						CallMonIndex = gObj[gObj[tObjNum].m_RecallMon].m_Index;
					}

					if(gObjTargetGuildWarCheck(lpObj,&gObj[CallMonIndex])==1)
					{
						EnableAttack = 1;
					}
				}

				if(EnableAttack != 0)
				{
					BYTE bAttackType = gObjAttackType(&gObj[tObjNum],lpObj);

					if (bAttackType > 0)
					{
						if(this->CalDistance(x,y,gObj[tObjNum].X,gObj[tObjNum].Y) < 3)
						{
							gObjAttack(lpObj,&gObj[tObjNum],lpMagic,1,1,0,0);
				
							if (bAttackType == 2)
							{
								int Poison = ((lpObj->m_AttackRating) / 10);
								gObj[tObjNum].m_SkillSwordSlashTime = 3;
								gObj[tObjNum].m_SkillSwordSlashDmg = Poison;
								gObj[tObjNum].m_SkillSwordSlashSender = aIndex;
								GCSkillInfoSend(&gObj[tObjNum],1,0x53);
							}
						}
					}
				}
			}
		}

		count++;
		if(count > MAX_VIEWPORT -1)
		{
			break;
		}
	}
	return true;
}





int CObjUseSkill::SkillSuddenIce(int aIndex, CMagicInf * lpMagic, BYTE x, BYTE y, BYTE dir, BYTE TargetPos, int aTargetIndex)
{
	LPOBJ lpObj = &gObj[aIndex];
	int tObjNum;
	int count = 0;
	int HitCount = 0;
	int bAttack;
	int DuelIndex = lpObj->m_iDuelUser;
	int EnableAttack;

	while ( true )
	{
		if(lpObj->VpPlayer2[count].state != 0)
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if(tObjNum >= 0)
			{
				EnableAttack = 0;
				
				if ( gclassObjAttack.PkCheck(lpObj, &gObj[tObjNum]) == FALSE )
				{
					EnableAttack = 0;
				}
				else if ( gclassObjAttack.CheckAttackArea(lpObj, &gObj[tObjNum]) == FALSE )
				{
					EnableAttack = 0;
				}
				else if(lpObj->VpPlayer2[count].type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon < 0)
				{
					EnableAttack = 1;
				}
				else if(tObjNum == aTargetIndex || DuelIndex == tObjNum)
				{
					EnableAttack = 1;
				}
				else
				{
					int CallMonIndex = gObj[tObjNum].m_Index;

					if(gObj[tObjNum].Type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon >= 0)
					{
						CallMonIndex = gObj[gObj[tObjNum].m_RecallMon].m_Index;
					}

					if(gObjTargetGuildWarCheck(lpObj,&gObj[CallMonIndex])==1)
					{
						EnableAttack = 1;
					}
				}

				if(EnableAttack != 0)
				{
					BYTE bAttackType = gObjAttackType(&gObj[tObjNum],lpObj);

					if (bAttackType > 0)
					{
						if(this->CalDistance(x,y,gObj[tObjNum].X,gObj[tObjNum].Y) < 4)
						{
							bAttack = 0;

							if(HitCount > 10)
							{
								bAttack = 0;
							}
						
							if(HitCount >= 5)
							{
								if(rand()%2)
								{
									bAttack = 1;
								}
							}
							else
							{
								bAttack = 1;
							}

							if(bAttack != 0)
							{
								//int delay = rand()%500;
								gObjAttack(lpObj,&gObj[tObjNum],lpMagic,1,1,0,0);
							}
						}
					}
				}
			}
		}

		count++;
		if(count > MAX_VIEWPORT -1)
		{
			break;
		}
	}
	return true;
}




BOOL CObjUseSkill::SkillHellFire2(int aIndex, int aTargetIndex, CMagicInf * lpMagic)
{
	/*if(aTargetIndex == AT_SKILL_EXPHELL_START)
	{
		return this->SkillHellFire2Start(aIndex,lpMagic);
	}*/

	LPOBJ lpObj = &gObj[aIndex];
	int tObjNum;

	if(lpObj->SkillHellFire2State == 0)
	{
		return this->SkillHellFire2Start(aIndex,lpMagic);
		//LogAddTD("[%s][%s] Wrong HellFire/Nova skill state [IDX:%d / ST:%d,%d,%d]",lpObj->AccountID,lpObj->Name,aTargetIndex,lpObj->SkillHellFire2State,lpObj->SkillHellFire2Time,lpObj->SkillHellFire2Count);
		//return false;
	}

	lpObj->SkillHellFire2State = 0;
	int count = 0;
	lpObj->SkillHellFire2Time = 0;

	int DuelIndex = lpObj->m_iDuelUser;
	int EnableAttack;

	while ( true )
	{
		if(lpObj->VpPlayer2[count].state != 0)
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if(tObjNum >= 0)
			{
				EnableAttack = 0;
				
				if ( gclassObjAttack.PkCheck(lpObj, &gObj[tObjNum]) == FALSE )
				{
					EnableAttack = 0;
				}
				else if ( gclassObjAttack.CheckAttackArea(lpObj, &gObj[tObjNum]) == FALSE )
				{
					EnableAttack = 0;
				}
				else if(lpObj->VpPlayer2[count].type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon < 0)
				{
					EnableAttack = 1;
				}
				else if(aTargetIndex == tObjNum || DuelIndex == tObjNum)
				{
					EnableAttack = 1;
				}
				else
				{
					int CallMonIndex = gObj[tObjNum].m_Index;

					if(gObj[tObjNum].Type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon >= 0)
					{
						CallMonIndex = gObj[gObj[tObjNum].m_RecallMon].m_Index;
					}

					if(gObjTargetGuildWarCheck(lpObj,&gObj[CallMonIndex])==1)
					{
						EnableAttack = 1;
					}
				}

				if(EnableAttack != 0)
				{
					BYTE bAttackType = gObjAttackType(&gObj[tObjNum],lpObj);

					if (bAttackType > 0)
					{
						if(gObjCalDistance(lpObj,&gObj[tObjNum]) <= 6)
						{
							gObjAddAttackProcMsgSendDelay(lpObj,50,tObjNum,600,lpMagic->m_Skill,0);
						}
					}
				}
			}
		}

		count++;
		if(count > MAX_VIEWPORT -1)
		{
			break;
		}
	}

	GCMagicAttackNumberSend(lpObj,AT_SKILL_EXPHELL,lpObj->m_Index,1);
	return true;
}




BOOL CObjUseSkill::SkillHellFire2Start(int aIndex, CMagicInf * lpMagic)
{
	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->SkillHellFire2State != 0)
	{
		LogAddTD("[%s][%s] SkillHellFire2Start has already been set!",lpObj->AccountID,lpObj->Name);
		return false;
	}

	lpObj->SkillHellFire2Time = GetTickCount();
	lpObj->SkillHellFire2State = 1;
	lpObj->SkillHellFire2Count = 0;

	LogAddTD("[%s][%s] SkillHellFire2Start set [%d,%d,%d]!",lpObj->AccountID,lpObj->Name,lpObj->SkillHellFire2State,lpObj->SkillHellFire2Time,lpObj->SkillHellFire2Count);
	GCMagicAttackNumberSend(lpObj,AT_SKILL_EXPHELL_START,aIndex,1);
	return true;
}




BOOL CObjUseSkill::SkillKnightBlow(int aIndex, int aTargetIndex, CMagicInf* lpMagic, BOOL isCombo)
{
	LPOBJ lpObj = &gObj[aIndex];
	int tObjNum;

	float fangle = this->GetAngle(lpObj->X,lpObj->Y,gObj[aTargetIndex].X,gObj[aTargetIndex].Y);

	this->SkillFrustrum(aIndex,fangle,1.5f,3.0f);

	int count = 0;
	int loopcount = 0;
	int attackcheck;

	//int viewskill = lpMagic->m_Skill;
	//if(viewskill >= 460 && viewskill <= 464)
	//	viewskill = 43;

	GCMagicAttackNumberSend(lpObj,lpMagic->m_Skill,aTargetIndex,1);

	int DuelIndex = lpObj->m_iDuelUser;
	int EnableAttack;

	while ( true )
	{
		if(lpObj->VpPlayer2[count].state != 0)
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if(tObjNum >= 0)
			{
				EnableAttack = 0;
				
				if ( gclassObjAttack.PkCheck(lpObj, &gObj[tObjNum]) == FALSE )
				{
					EnableAttack = 0;
				}
				else if ( gclassObjAttack.CheckAttackArea(lpObj, &gObj[tObjNum]) == FALSE )
				{
					EnableAttack = 0;
				}
				else if(lpObj->VpPlayer2[count].type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon < 0)
				{
					EnableAttack = 1;
				}
				else if(tObjNum == aTargetIndex || DuelIndex == tObjNum)
				{
					EnableAttack = 1;
				}
				else
				{
					int CallMonIndex = gObj[tObjNum].m_Index;

					if(gObj[tObjNum].Type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon >= 0)
					{
						CallMonIndex = gObj[gObj[tObjNum].m_RecallMon].m_Index;
					}

					if(gObjTargetGuildWarCheck(lpObj,&gObj[CallMonIndex])==1)
					{
						EnableAttack = 1;
					}
				}

				if(EnableAttack != 0)
				{
					BYTE bAttackType = gObjAttackType(&gObj[tObjNum],lpObj);

					if (bAttackType > 0)
					{
						attackcheck = 0;

						if(loopcount == 0)
						{
							attackcheck = 1;
						}
						else if(rand()%3==0)
						{
							attackcheck = 1;
						}

						if(attackcheck != 0)
						{
							if(::SkillTestFrustrum(gObj[tObjNum].X,gObj[tObjNum].Y,aIndex))
							{
								gObjAttack(lpObj,&gObj[tObjNum],lpMagic,0,1,0,isCombo);
								if(isCombo != 0)
								{
									GCMagicAttackNumberSend(lpObj,AT_SKILL_COMBO,gObj[tObjNum].m_Index,1);
								}
							}
						}
					}
				}
			}
		}

		count++;
		if(count > MAX_VIEWPORT -1)
		{
			loopcount++;
			count = 0;
			if(loopcount >= 3)
			{
				break;
			}
		}
	}
	return true;
}




void CObjUseSkill::SkillFrustrum(int aIndex, float fangle, float tx, float ty)
{
	LPOBJ lpObj = &gObj[aIndex];

	float p[MAX_ARRAY_FRUSTRUM][3];

	p[0][0] = -tx;
	p[0][1] = ty;
	p[0][2] = 0;
	p[1][0] = tx;
	p[1][1] = ty;
	p[1][2] = 0;
	p[2][0] = 1.0f;
	p[2][1] = 0;
	p[2][2] = 0;
	p[3][0] = -1.0f;
	p[3][1] = 0;
	p[3][2] = 0;

	float Angle[3];

	Angle[0] = 0;
	Angle[1] = 0;
	Angle[2] = fangle;

	float Matrix[3][MAX_ARRAY_FRUSTRUM];
	float vFrustrum[MAX_ARRAY_FRUSTRUM][3];

	AngleMatrix(Angle,Matrix);

	for(int i = 0; i < MAX_ARRAY_FRUSTRUM; i++)
	{
		VectorRotate((const float *)&p[i],(const float *)&Matrix[0],(float *)&vFrustrum[i]);

		lpObj->fSkillFrustrumX[i] = (int)vFrustrum[i][0] + lpObj->X;
		lpObj->fSkillFrustrumY[i] = (int)vFrustrum[i][1] + lpObj->Y;
	}
}




void CObjUseSkill::SkillFrustrum2(int aIndex, float fangle, float tx, float ty)
{
	LPOBJ lpObj = &gObj[aIndex];


	float p[MAX_ARRAY_FRUSTRUM][3];

	p[0][0] = 0;
	p[0][1] = ty;
	p[0][2] = 0;
	p[1][0] = 0;
	p[1][1] = ty;
	p[1][2] = 0;
	p[2][0] = tx;
	p[2][1] = 1.0f;
	p[2][2] = 0;
	p[3][0] = -tx;
	p[3][1] = 1.0f;
	p[3][2] = 0;

	float Angle[3];

	Angle[0] = 0;
	Angle[1] = 0;
	Angle[2] = fangle;

	float Matrix[3][MAX_ARRAY_FRUSTRUM];
	float vFrustrum[MAX_ARRAY_FRUSTRUM][3];

	AngleMatrix(Angle,Matrix);

	for(int i = 0; i < MAX_ARRAY_FRUSTRUM; i++)
	{
		VectorRotate((const float *)&p[i],(const float *)&Matrix[0],(float *)&vFrustrum[i]);

		lpObj->fSkillFrustrumX[i] = (int)vFrustrum[i][0] + lpObj->X;
		lpObj->fSkillFrustrumY[i] = (int)vFrustrum[i][1] + lpObj->Y;
	}
}




int  CObjUseSkill::GetAngle(int x, int y, int tx, int ty)
{
	double diffX = x - tx;
	double diffY = y - ty;

	double rad = atan2(diffY,diffX);

	int angle = rad * 180 / 3.141592741012573 + 90;

	if(angle < 0)
	{
		angle += 360;
	}
	return angle;
}




int  CObjUseSkill::CalDistance(int x, int y, int x1, int y1)
{
	if(x == x1 && y == y1)
	{
		return 0;
	}

	float tx = x - x1;
	float ty = y - y1;

	return sqrt(tx*tx + ty*ty);
}




BOOL CObjUseSkill::SkillAreaCheck(int angel, int x, int y, int tx, int ty, int dis, int arc)
{
	int minangel = angel - arc;
	int maxangel = angel + arc;

	if(this->CalDistance(x,y,tx,ty) > dis)
	{
		return false;
	}

	int targetangle = this->GetAngle(x,y,tx,ty);

	if(minangel < 0)
	{
		if(targetangle > maxangel)
		{
			if(targetangle > minangel + 360)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else if(maxangel < targetangle)
		{
			return false;
		}
	}
	else if(maxangel > 360)
	{
		if(targetangle < minangel)
		{
			if(targetangle < maxangel - 360)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else if(maxangel < targetangle)
		{
			return false;
		}
	}
	else if(minangel > targetangle)
	{
		return false;
	}
	else if(maxangel < targetangle)
	{
		return false;
	}
	return true;
}




BOOL CObjUseSkill::SkillSpear(int aIndex, int aTargetIndex, CMagicInf * lpMagic)
{
	LPOBJ lpObj = &gObj[aIndex];
	int tObjNum;

	int iangle = this->GetAngle(lpObj->X,lpObj->Y,gObj[aTargetIndex].X,gObj[aTargetIndex].Y);
	int count = 0;
	int loopcount = 0;
	int attackcheck;

	GCMagicAttackNumberSend(lpObj,lpMagic->m_Skill,aTargetIndex,1);

	if(lpObj->SkillLongSpearChange == false)
	{
		gObjAttack(lpObj,&gObj[aTargetIndex],lpMagic,0,1,0,0);
		return true;
	}

	int DuelIndex = lpObj->m_iDuelUser;
	int EnableAttack;

	while ( true )
	{
		if(lpObj->VpPlayer2[count].state != 0)
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if(tObjNum >= 0)
			{
				EnableAttack = 0;
				
				if ( gclassObjAttack.PkCheck(lpObj, &gObj[tObjNum]) == FALSE )
				{
					EnableAttack = 0;
				}
				else if ( gclassObjAttack.CheckAttackArea(lpObj, &gObj[tObjNum]) == FALSE )
				{
					EnableAttack = 0;
				}
				else if(lpObj->VpPlayer2[count].type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon < 0)
				{
					EnableAttack = 1;
				}
				else if(tObjNum == aTargetIndex || DuelIndex == tObjNum)
				{
					EnableAttack = 1;
				}
				else
				{
					int CallMonIndex = gObj[tObjNum].m_Index;

					if(gObj[tObjNum].Type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon >= 0)
					{
						CallMonIndex = gObj[gObj[tObjNum].m_RecallMon].m_Index;
					}

					if(gObjTargetGuildWarCheck(lpObj,&gObj[CallMonIndex])==1)
					{
						EnableAttack = 1;
					}
				}

				if(EnableAttack != 0)
				{
					BYTE bAttackType = gObjAttackType(&gObj[tObjNum],lpObj);

					if (bAttackType > 0)
					{
						attackcheck = 0;

						if(loopcount == 0)
						{
							attackcheck = 1;
						}
						else if(rand()%3==0)
						{
							attackcheck = 1;
						}

						if(attackcheck != 0)
						{
							if(SkillSpearHitBox.HitCheck(iangle,lpObj->X,lpObj->Y,gObj[tObjNum].X,gObj[tObjNum].Y))
							{
								gObjAttack(lpObj,&gObj[tObjNum],lpMagic,0,1,0,0);
							}
						}
					}
				}
			}
		}

		count++;
		if(count > MAX_VIEWPORT -1)
		{
			break;
		}
	}
	return true;
}




BOOL CObjUseSkill::SkillFireBurst(int aIndex, int aTargetIndex, CMagicInf * lpMagic)
{
	LPOBJ lpObj = &gObj[aIndex];
	int tObjNum;
	int count = 0;
	int loopcount = 0;

	GCMagicAttackNumberSend(lpObj,lpMagic->m_Skill,aTargetIndex,1);
	gObjAttack(lpObj,&gObj[aTargetIndex],lpMagic,0,1,0,0);

	int attackcheck;
	int DuelIndex = lpObj->m_iDuelUser;
	int EnableAttack;
	int delaytime;

	while ( true )
	{
		if(lpObj->VpPlayer2[count].state != 0)
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if(tObjNum >= 0 && aTargetIndex != tObjNum)
			{
				EnableAttack = 0;
				
				if ( gclassObjAttack.PkCheck(lpObj, &gObj[tObjNum]) == FALSE )
				{
					EnableAttack = 0;
				}
				else if ( gclassObjAttack.CheckAttackArea(lpObj, &gObj[tObjNum]) == FALSE )
				{
					EnableAttack = 0;
				}
				else if(lpObj->VpPlayer2[count].type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon < 0)
				{
					EnableAttack = 1;
				}
				else if(tObjNum == aTargetIndex || DuelIndex == tObjNum)
				{
					EnableAttack = 1;
				}
				else
				{
					int CallMonIndex = gObj[tObjNum].m_Index;

					if(gObj[tObjNum].Type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon >= 0)
					{
						CallMonIndex = gObj[gObj[tObjNum].m_RecallMon].m_Index;
					}

					if(gObjTargetGuildWarCheck(lpObj,&gObj[CallMonIndex])==1)
					{
						EnableAttack = 1;
					}
				}

				if(EnableAttack != 0)
				{
					BYTE bAttackType = gObjAttackType(&gObj[tObjNum],lpObj);

					if (bAttackType > 0)
					{
						attackcheck = 0;

						if(loopcount == 0)
						{
							attackcheck = 1;
						}
						else if(rand()%3==0)
						{
							attackcheck = 1;
						}

						if(attackcheck != 0)
						{
							if(gObjCalDistance(&gObj[aTargetIndex],&gObj[tObjNum]) < 3)
							{
								delaytime = (rand()*17)%300 + 500;

								gObjAddAttackProcMsgSendDelay(lpObj,50,tObjNum,delaytime,lpMagic->m_Skill,0);
							}
						}
					}
				}
			}
		}

		count++;
		if(count > MAX_VIEWPORT -1)
		{
			break;
		}
	}
	return true;
}




BOOL CObjUseSkill::SkillDarkHorseAttack(int aIndex, int aTargetIndex, CMagicInf * lpMagic)
{
	LPOBJ lpObj = &gObj[aIndex];
	int tObjNum;
	int count = 0;
	int HitCount = 0;
	int bAttack;
	int DuelIndex = lpObj->m_iDuelUser;
	int EnableAttack;

	GCMagicAttackNumberSend(lpObj,AT_SKILL_DARKHORSE_ATTACK,lpObj->m_Index,1);

	while ( true )
	{
		if(lpObj->VpPlayer2[count].state != 0)
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if(tObjNum >= 0)
			{
				EnableAttack = 0;
				
				if ( gclassObjAttack.PkCheck(lpObj, &gObj[tObjNum]) == FALSE )
				{
					EnableAttack = 0;
				}
				else if ( gclassObjAttack.CheckAttackArea(lpObj, &gObj[tObjNum]) == FALSE )
				{
					EnableAttack = 0;
				}
				else if(lpObj->VpPlayer2[count].type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon < 0)
				{
					EnableAttack = 1;
				}
				else if(tObjNum == aTargetIndex || DuelIndex == tObjNum)
				{
					EnableAttack = 1;
				}
				else
				{
					int CallMonIndex = gObj[tObjNum].m_Index;
	
					if(gObj[tObjNum].Type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon >= 0)
					{
						CallMonIndex = gObj[gObj[tObjNum].m_RecallMon].m_Index;
					}
	
					if(gObjTargetGuildWarCheck(lpObj,&gObj[CallMonIndex])==1)
					{
						EnableAttack = 1;
					}
				}
	
				if(EnableAttack != 0)
				{
					BYTE bAttackType = gObjAttackType(&gObj[tObjNum],lpObj);

					if (bAttackType > 0)
					{
						if(gObjCalDistance(lpObj,&gObj[tObjNum]) < 5)
						{
							bAttack = 0;
	
							if(HitCount > 15)
							{
								bAttack = 0;
							}
	
							if(HitCount >= 10)
							{
								if(rand()%2)
								{
									bAttack = 1;
								}
							}
							else
							{
								bAttack = 1;
							}
	
							if(bAttack != 0)
							{
								gObjAddAttackProcMsgSendDelay(lpObj,50,tObjNum,500,lpMagic->m_Skill,0);
							}
						}
					}
				}
			}
		}
	
		count++;
		if(count > MAX_VIEWPORT -1)
		{
			break;
		}
	}
return true;
}

BOOL CObjUseSkill::SkillGhostPhantom(int aIndex, int aTargetIndex, CMagicInf * lpMagic)
{
	LPOBJ lpObj = &gObj[aIndex];
	int tObjNum;
	int count = 0;
	int HitCount = 0;
	int bAttack;
	int DuelIndex = lpObj->m_iDuelUser;
	int EnableAttack;

	int	NewDamage = ReadConfig.SkillGhostPhantom_AddMagic * (lpMagic->m_Level + 1);
	int BookDmg = (int)(lpObj->m_MagicDamageMin * ((lpMagic->m_Level+1) / ReadConfig.SkillGhostPhantom_Div));//130 * (lpMagic->m_Level + 1);
	BookDmg += gObj[aIndex].MasterCharacterInfo->IncWizardyTwoHand;
	BookDmg += gObj[aIndex].MasterCharacterInfo->IncMaxSkillPower;

	if(gObj[aIndex].m_SkillBerserkerTime > 0)
		BookDmg += (BookDmg * gObj[aIndex].MasterCharacterInfo->IncBerserker1)/100.0f;

	BookDmg += gObj[aIndex].MasterCharacterInfo->IncExpansionWizardy1;

	lpObj->m_MagicDamageMin += NewDamage;
	lpObj->m_MagicDamageMax += NewDamage;

	while ( true )
	{
		if(lpObj->VpPlayer2[count].state != 0)
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if(tObjNum >= 0)
			{
				EnableAttack = 0;
				
				if ( gclassObjAttack.PkCheck(lpObj, &gObj[tObjNum]) == FALSE )
				{
					EnableAttack = 0;
				}
				else if ( gclassObjAttack.CheckAttackArea(lpObj, &gObj[tObjNum]) == FALSE )
				{
					EnableAttack = 0;
				}
				else if(lpObj->VpPlayer2[count].type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon < 0)
				{
					EnableAttack = 1;
				}
				else if(tObjNum == aTargetIndex || DuelIndex == tObjNum)
				{
					EnableAttack = 1;
				}
				else
				{
					int CallMonIndex = gObj[tObjNum].m_Index;
	
					if(gObj[tObjNum].Type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon >= 0)
					{
						CallMonIndex = gObj[gObj[tObjNum].m_RecallMon].m_Index;
					}
	
					if(gObjTargetGuildWarCheck(lpObj,&gObj[CallMonIndex])==1)
					{
						EnableAttack = 1;
					}
				}
	
				if(EnableAttack != 0)
				{
					BYTE bAttackType = gObjAttackType(&gObj[tObjNum],lpObj);

					if (bAttackType > 0)
					{
						int dis = gObjCalDistance(lpObj,&gObj[tObjNum]);
						if(dis < ReadConfig.SkillGhostPhantom_Dist)
						{
							bAttack = 0;

							if(HitCount > ReadConfig.SkillGhostPhantom_Hit1)
							{
								bAttack = 0;
							}
	
							if(HitCount >= ReadConfig.SkillGhostPhantom_Hit2)
							{
								if(rand()%2)
								{
									bAttack = 1;
								}
							}
							else
							{
								bAttack = 1;
							}

							if(bAttack != 0)
							{
								gObjAttack(lpObj,&gObj[tObjNum],lpMagic,1,1,0,0);
								//gObjAddAttackProcMsgSendDelay(lpObj,50,tObjNum,700,lpMagic->m_Skill,0);

								if (bAttackType == 2)
								{
									gObj[tObjNum].m_SkillSummonerBookSender = aIndex;
									gObj[tObjNum].m_SkillSummonerBookTime = ReadConfig.SkillGhostPhantom_Sec;
									gObj[tObjNum].m_SkillSummonerBookTypeDmg = 0x57;
									if (gObj[tObjNum].Type == OBJ_MONSTER && gObj[tObjNum].Level < 100)
									{
										gObj[tObjNum].m_SkillSummonerBookDmg = (int)(BookDmg * (100.0f/gObj[tObjNum].Level));
									} else {
										gObj[tObjNum].m_SkillSummonerBookDmg = BookDmg;
									}
									GCSkillInfoSend(&gObj[tObjNum],1,0x57);
								}
							}
						}
					}
				}
			}
		}
	
		count++;
		if(count > MAX_VIEWPORT -1)
		{
			break;
		}
	}

	lpObj->m_MagicDamageMin-=NewDamage;
	lpObj->m_MagicDamageMax-=NewDamage;
return true;
}


BOOL CObjUseSkill::RecallPartyCheck(int aIndex, int skill_level)
{
	LPOBJ lpObj = &gObj[aIndex];
	int number;
	int partynum = 0;

	partynum = lpObj->PartyNumber;

	int partycount;
	LPOBJ lpPartyObj;

	if(partynum != -1)
	{
		partycount = gParty.m_PartyS[partynum].Count;

		for(int n = 0; n < MAX_USER_IN_PARTY;n++)
		{
			number = gParty.m_PartyS[partynum].Number[n];

			if(number >= 0)
			{
				lpPartyObj = &gObj[number];

				if(lpPartyObj->m_Index != lpObj->m_Index)
				{
					if(lpPartyObj->MapNumber == lpObj->MapNumber)
					{
						if(lpPartyObj->SkillRecallParty_Time != 0)
						{
							return false;
						}
					}
				}
			}
		}
	}
	return true;
}




void CObjUseSkill::SkillRecallParty(int aIndex, int skill_level)
{
	int skillSuccess = true;
	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->Type != OBJ_USER && lpObj->m_RecallMon == -1)
	{
		return;
	}

	if(this->RecallPartyCheck(aIndex,skill_level) == 0)
	{
		MsgOutput(aIndex,lMsg.Get(1238));
		return;
	}

	if(gMoveCommand.CheckMainToMove(lpObj) == 0)
	{
		char msg[255];
		wsprintf(msg,lMsg.Get(1249));
		GCServerMsgStringSend(msg,lpObj->m_Index,1);
		return;
	}

	if(gMoveCommand.CheckInterfaceToMove(lpObj) == 0)
	{
		char msg[255];
		wsprintf(msg,lMsg.Get(1239));
		GCServerMsgStringSend(msg,lpObj->m_Index,1);
		return;
	}

	int number;
	int partynum;
	int partycount;
	LPOBJ lpPartyObj;
	int recallcount;

	partynum = 0;
	partynum = lpObj->PartyNumber;

	GCMagicAttackNumberSend(lpObj,AT_SKILL_RECALL_PARTY,lpObj->m_Index,skillSuccess);

	recallcount = 0;

	if(partynum != -1)
	{
		partycount = gParty.m_PartyS[partynum].Count;

		for(int n = 0; n < MAX_USER_IN_PARTY;n ++)
		{
			number = gParty.m_PartyS[partynum].Number[n];

			if(number >= 0)
			{
				lpPartyObj = &gObj[number];

				if(lpPartyObj->m_Index != lpObj->m_Index)
				{
					int movelevel = gMoveCommand.GetMoveLevel(lpObj->MapNumber,lpObj->X,lpObj->Y,lpPartyObj->Class);
					int bCheckMainToMove = gMoveCommand.CheckMainToMove(lpPartyObj);
					int bCheckEquipmentToMove = gMoveCommand.CheckEquipmentToMove(lpPartyObj,lpObj->MapNumber);
					int bCheckInterfaceToMove = gMoveCommand.CheckInterfaceToMove(lpPartyObj);

					if(lpPartyObj->Level >= movelevel
						&& movelevel != -1
						&& bCheckMainToMove != false
						&& bCheckEquipmentToMove != false
						&& bCheckInterfaceToMove != false)
					{
						int addx;
						int addy;
						int count = 50;
						int Find = 0;

						while(count--)
						{
							addx = lpObj->X + rand()%9 - 4;
							addy = lpObj->Y + rand()%9 - 4;

							if(gObjCheckTeleportArea(lpObj->m_Index,addx,addy))
							{
								Find = 1;
								break;
							}
						}

						if(Find == 0)
						{
							addx = lpObj->X;
							addy = lpObj->Y;
						}

						lpPartyObj->SkillRecallParty_Time = 7;
						lpPartyObj->SkillRecallParty_MapNumber = lpObj->MapNumber;
						lpPartyObj->SkillRecallParty_X = addx;
						lpPartyObj->SkillRecallParty_Y = addy;

						char msg[255];

						wsprintf(msg,lMsg.Get(1251));
						GCServerMsgStringSend(msg,lpPartyObj->m_Index,1);
						recallcount++;
					}
					else
					{
						char msg[255];
						wsprintf(msg,lMsg.Get(1252));
						GCServerMsgStringSend(msg,lpPartyObj->m_Index,1);
					}
				}
			}
		}
	}

	if(recallcount != 0)
	{
		char msg[255];
		wsprintf(msg,lMsg.Get(1244),recallcount);
		GCServerMsgStringSend(msg,lpObj->m_Index,1);
	}
}




void CObjUseSkill::SkillAddCriticalDamage(int aIndex, int skill_level)
{
	int skillSuccess = true;
	LPOBJ lpObj = &gObj[aIndex];
	int number;

	if(lpObj->Type != OBJ_USER && lpObj->m_RecallMon == -1)
	{
		return;
	}

	if(lpObj->Class != CLASS_DARKLORD)
	{
		return;
	}


	int partynum = 0;
	int partycount;
	int dis;

	int ApplyPartyIndex[MAX_USER_IN_PARTY];
	LPOBJ lpPartyObj;
	memset(ApplyPartyIndex,-1,sizeof(ApplyPartyIndex));

	partynum = lpObj->PartyNumber;

	if(partynum != -1)
	{
		partycount = gParty.m_PartyS[partynum].Count;

		for(int n = 0; n < MAX_USER_IN_PARTY; n++)
		{
			number = gParty.m_PartyS[partynum].Number[n];

			if(number >= 0)
			{
				lpPartyObj = &gObj[number];

				if(lpObj->MapNumber == lpPartyObj->MapNumber)
				{
					dis = gObjCalDistance(lpObj,&gObj[number]);
					if(dis < 10)
					{
						ApplyPartyIndex[n] = lpPartyObj->m_Index;
					}
				}
			}
		}
	}

	int addcriticaldamagevalue = (lpObj->Leadership + lpObj->AddLeadership)/ReadConfig.AddCriticalDmgDiv1 + (lpObj->Energy + lpObj->AddEnergy)/ReadConfig.AddCriticalDmgDiv2;
	int SkillTime = (lpObj->Energy + lpObj->AddEnergy)/ReadConfig.AddCriticalDmgTimeDiv + ReadConfig.AddCriticalDmgTimeBase;

	addcriticaldamagevalue += lpObj->MasterCharacterInfo->IncCriticalDamage1;
	SkillTime += lpObj->MasterCharacterInfo->IncCriticalDamage2;
	addcriticaldamagevalue += (addcriticaldamagevalue * lpObj->MasterCharacterInfo->IncCriticalDamage3)/100.0f;

	if(partynum == -1)
	{
		if(lpObj->SkillAddCriticalDamageTime > 0)
			return;

		lpObj->SkillAddCriticalDamageTime = SkillTime;
		lpObj->SkillAddCriticalDamage = addcriticaldamagevalue;
		lpObj->m_ViewSkillState |= 2048;

		MsgOutput(aIndex,lMsg.Get(1240),SkillTime);
		GCMagicAttackNumberSend(lpObj,AT_SKILL_ADD_CRITICALDAMAGE,lpObj->m_Index,skillSuccess);
	}
	else
	{
		for(int n = 0;n < MAX_USER_IN_PARTY;n++)
		{
			if(ApplyPartyIndex[n] != -1)
			{
				lpPartyObj = &gObj[ApplyPartyIndex[n]];

				if(lpPartyObj->SkillAddCriticalDamageTime > 0)
					continue;

				lpPartyObj->SkillAddCriticalDamageTime = SkillTime;
				lpPartyObj->SkillAddCriticalDamage = addcriticaldamagevalue;
				lpPartyObj->m_ViewSkillState |= 2048;

				MsgOutput(aIndex,lMsg.Get(1240),SkillTime);
				GCMagicAttackNumberSend(lpObj,AT_SKILL_ADD_CRITICALDAMAGE,lpPartyObj->m_Index,skillSuccess);
			}
		}
	}
}


void CObjUseSkill::SkillBirds(int aIndex, CMagicInf * lpMagic, BYTE x, BYTE y, BYTE dir, BYTE TargetPos, int aTargetIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	int TargePosx = lpObj->X - (8 - (TargetPos & 15));
	int TargePosy = lpObj->Y - (8 - ((TargetPos & 240) >> 4));

	int tObjNum;
	int count = 0;
	int DuelIndex = lpObj->m_iDuelUser;
	int iangle = this->GetAngle(lpObj->X,lpObj->Y,TargePosx,TargePosy);
	int delaytime;
	int Hit = 0;
	int EnableAttack;

	while ( true )
	{
		if(lpObj->VpPlayer2[count].state != 0)
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if(tObjNum >= 0)
			{
				EnableAttack = 0;
				
				if ( gclassObjAttack.PkCheck(lpObj, &gObj[tObjNum]) == FALSE )
				{
					EnableAttack = 0;
				}
				else if ( gclassObjAttack.CheckAttackArea(lpObj, &gObj[tObjNum]) == FALSE )
				{
					EnableAttack = 0;
				}
				else if(lpObj->VpPlayer2[count].type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon < 0)
				{
					EnableAttack = 1;
				}
				else if(tObjNum == aTargetIndex || DuelIndex == tObjNum)
				{
					EnableAttack = 1;
				}
				else
				{
					int CallMonIndex = gObj[tObjNum].m_Index;

					if(gObj[tObjNum].Type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon >= 0)
					{
						CallMonIndex = gObj[gObj[tObjNum].m_RecallMon].m_Index;
					}

					if(gObjTargetGuildWarCheck(lpObj,&gObj[CallMonIndex])==1)
					{
						EnableAttack = 1;
					}
				}

				if(EnableAttack != 0)
				{
					BYTE bAttackType = gObjAttackType(&gObj[tObjNum],lpObj);

					if (bAttackType > 0)
					{
						if(SkillElectricSparkHitBox.HitCheck(iangle,gObj[aIndex].X,gObj[aIndex].Y,gObj[tObjNum].X,gObj[tObjNum].Y))
						{
							delaytime = 300;
							Hit = 1;

							gObjAddAttackProcMsgSendDelay(lpObj,50,tObjNum,delaytime,lpMagic->m_Skill,0);
						}
					}
				}
			}
		}

		count++;
		if(count > MAX_VIEWPORT -1)
		{
			break;
		}
	}
}

void CObjUseSkill::SkillElectricSpark(int aIndex, CMagicInf * lpMagic, BYTE x, BYTE y, BYTE dir, BYTE TargetPos, int aTargetIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	int TargePosx = lpObj->X - (8 - (TargetPos & 15));
	int TargePosy = lpObj->Y - (8 - ((TargetPos & 240) >> 4));

	int tObjNum;
	int count = 0;
	int DuelIndex = lpObj->m_iDuelUser;
	int iangle = this->GetAngle(lpObj->X,lpObj->Y,TargePosx,TargePosy);
	int delaytime;
	int Hit = 0;
	int EnableAttack;

	while ( true )
	{
		if(lpObj->VpPlayer2[count].state != 0)
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if(tObjNum >= 0)
			{
				EnableAttack = 0;
				
				if ( gclassObjAttack.PkCheck(lpObj, &gObj[tObjNum]) == FALSE )
				{
					EnableAttack = 0;
				}
				else if ( gclassObjAttack.CheckAttackArea(lpObj, &gObj[tObjNum]) == FALSE )
				{
					EnableAttack = 0;
				}
				else if(lpObj->VpPlayer2[count].type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon < 0)
				{
					EnableAttack = 1;
				}
				else if(tObjNum == aTargetIndex || DuelIndex == tObjNum)
				{
					EnableAttack = 1;
				}
				else
				{
					int CallMonIndex = gObj[tObjNum].m_Index;

					if(gObj[tObjNum].Type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon >= 0)
					{
						CallMonIndex = gObj[gObj[tObjNum].m_RecallMon].m_Index;
					}

					if(gObjTargetGuildWarCheck(lpObj,&gObj[CallMonIndex])==1)
					{
						EnableAttack = 1;
					}
				}

				if(EnableAttack != 0)
				{
					BYTE bAttackType = gObjAttackType(&gObj[tObjNum],lpObj);

					if (bAttackType > 0)
					{
						if(SkillElectricSparkHitBox.HitCheck(iangle,gObj[aIndex].X,gObj[aIndex].Y,gObj[tObjNum].X,gObj[tObjNum].Y))
						{
							delaytime = 500;
							Hit = 1;

							gObjAddAttackProcMsgSendDelay(lpObj,50,tObjNum,delaytime,lpMagic->m_Skill,0);
						}
					}
				}
			}
		}

		count++;
		if(count > MAX_VIEWPORT -1)
		{
			break;
		}
	}

	if(Hit != 0)
	{
		DecPartyMemberHPandMP(lpObj);
	}
}




int  DecPartyMemberHPandMP(LPOBJ lpObj)
{
	LPOBJ lpPartyObj;
	int partynum = lpObj->PartyNumber;

	if(OBJMAX_RANGE(partynum)==false)
	{
		LogAdd("error : DecPartyMemberHPandMP %s %d",__FILE__,__LINE__);
		return false;
	}

	//int partycount = gParty.m_PartyS[partynum].Count;
	int retcount = 0;

	for(int n = 0; n < MAX_USER_IN_PARTY;n++)
	{
		int number = gParty.m_PartyS[partynum].Number[n];

		if(number >= 0)
		{
			lpPartyObj = &gObj[number];

			if(lpObj->MapNumber == lpPartyObj->MapNumber && lpObj->m_Index != number)
			{
				int dis = gObjCalDistance(lpObj,&gObj[number]);
				if(dis < 10)
				{
					lpPartyObj = &gObj[number];

					int declife = lpPartyObj->Life * 20.0f / 100.0f;
					int decmana = lpPartyObj->Mana * 5.0f / 100.0f;

					lpPartyObj->Life = lpPartyObj->Life - declife;
					lpPartyObj->Mana = lpPartyObj->Mana - decmana;

					GCReFillSend(lpPartyObj->m_Index,lpPartyObj->Life,0xFF,0,lpPartyObj->iShield);
					GCManaSend(lpPartyObj->m_Index,lpPartyObj->Mana,0xFF,0,lpPartyObj->BP);
					retcount++;
				}
			}
		}
	}

	return retcount;
}




BOOL CObjUseSkill::SkillSummon(int aIndex, int aTargetIndex, CMagicInf * lpMagic)
{
	if(OBJMAX_RANGE(aIndex)==false)
	{
		return false;
	}

	int iSummonCount = 1;
	int iSummonMonterType = 0;
	BYTE bDoSummon = 1;
	BYTE bSummonSuccess = 0;
	BYTE cSummonX = 0;
	BYTE cSummonY = 0;

	LPOBJ lpObj = &gObj[aIndex];

	switch(lpObj->Class)
	{
	case 161:
		iSummonMonterType = rand()%2+ 147;
		bDoSummon = (rand()%15) ? false : true;
		break;
	case 181:
		iSummonMonterType = rand()%2+ 177;
		bDoSummon = (rand()%15) ? false : true;
		break;
	case 189:
		iSummonMonterType = rand()%2+ 185;
		bDoSummon = (rand()%15) ? false : true;
		break;
	case 197:
		iSummonMonterType = rand()%2+ 193;
		bDoSummon = (rand()%15) ? false : true;
		break;
	case 267:
		iSummonMonterType = rand()%2+ 263;
		bDoSummon = (rand()%15) ? false : true;
		break;
	case 275:
		iSummonMonterType = rand()%2+ 271;
		bDoSummon = (rand()%15) ? false : true;
		break;
	case 338:
		iSummonMonterType = rand()%2+ 331;
		bDoSummon = (rand()%15) ? false : true;
		break;
	}

	if(bDoSummon == false)
	{
		return false;
	}

	for(int iSC = 0; iSC < iSummonCount; iSC++)
	{
		BYTE cSX = lpObj->X;
		BYTE cSY = lpObj->Y;

		if(gObjGetRandomFreeLocation(lpObj->MapNumber,(BYTE &)cSX,(BYTE &)cSY,2,2,30)==1)
		{
			int result = gObjAddMonster(lpObj->MapNumber);

			if(result >= 0)
			{
				gObj[result].m_PosNum = (WORD)-1;
				gObj[result].X = cSX;
				gObj[result].Y = cSY;
				gObj[result].MapNumber = lpObj->MapNumber;
				gObj[result].TX = gObj[result].X;
				gObj[result].TY = gObj[result].Y;
				gObj[result].m_OldX = gObj[result].X;
				gObj[result].m_OldY = gObj[result].Y;
				gObj[result].Dir = 1;
				gObj[result].StartX = gObj[result].X;
				gObj[result].StartY = gObj[result].Y;
				gObjSetMonster(result,iSummonMonterType,"CObjUseSkill::SkillSummon");
				gObj[result].m_Attribute = 60;
				gObj[result].MaxRegenTime = 0;
				gObj[result].Dir = rand()%8;
				bSummonSuccess = 1;
				cSummonX = gObj[result].X;
				cSummonY = gObj[result].Y;
			}
		}
	}

	GCMagicAttackNumberSend(lpObj,AT_SKILL_SUMMON,(((cSummonX & 0xFF)&0xFF) | ((cSummonY & 0xFF)&0xFF) << 8) & 0xFFFF,bSummonSuccess);
	return bSummonSuccess;
}




BOOL CObjUseSkill::SkillImmuneToMagic(int aIndex, CMagicInf * lpMagic)
{
	if(OBJMAX_RANGE(aIndex) == false)
	{
		return false;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->m_ImmuneToMagicCount <= 0 && lpObj->m_ImmuneToHarmCount <= 0)
	{
		lpObj->m_ImmuneToMagicCount = 10;
		lpObj->m_ViewSkillState |= 128;
		GCSkillInfoSend(lpObj,1,0x3F);
		GCMagicAttackNumberSend(lpObj,AT_SKILL_IMMUNE_TO_MAGIC,lpObj->m_Index,1);
		return true;
	}
	return false;
}




BOOL CObjUseSkill::SkillImmuneToHarm(int aIndex, CMagicInf * lpMagic)
{
	if(OBJMAX_RANGE(aIndex) == false)
	{
		return false;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->m_ImmuneToMagicCount <= 0 && lpObj->m_ImmuneToHarmCount <= 0)
	{
		lpObj->m_ImmuneToHarmCount = 10;
		lpObj->m_ViewSkillState |= 8;
		GCSkillInfoSend(lpObj,1,0x02);
		GCMagicAttackNumberSend(lpObj,AT_SKILL_IMMUNE_TO_HARM,lpObj->m_Index,1);
		return true;
	}
	return false;
}

BOOL CObjUseSkill::RemoveAllCharacterInvalidMagicAndSkillState(LPOBJ lpObj)
{
	if(lpObj->Type != OBJ_USER)
	{
		return false;
	}

	lpObj->m_SkillAttackTime = 0;
	lpObj->m_SkillAttack = 0;
	lpObj->m_ViewSkillState &= ~4;

	if(lpObj->m_SkillReflectTime > 0)
	{
		lpObj->m_SkillReflectTime = 1;
	}else
	{
		lpObj->m_SkillReflectTime = 0;
		lpObj->m_SkillReflect = 0;
	}

	lpObj->m_SkillDragonSlayerTime = 0;
	lpObj->m_SkillDragonSlayerDmg = 0;

	lpObj->m_SkillRedArmorIgnoreTime = 0;
	lpObj->m_SkillRedArmorIgnoreNum = 0;

	lpObj->m_SkillFitnessTime = 0;

	lpObj->m_SkillDefSuccessRateIncTime = 0;
	lpObj->m_SkillDefSuccessRateIncNum = 0;

	lpObj->m_SkillBerserkerTime = 0;
	lpObj->m_SkillBerserkerDef = 0;
	lpObj->m_SkillBerserkerAttack = 0;

	lpObj->m_SkillMagicCircleTime = 0;
	lpObj->m_SkillMagicCircle = 0;

	lpObj->m_SkillReduceDamageTime = 0;
	lpObj->m_SkillReduceDamage = 0;

	lpObj->m_iSkillNPCSantaType = 0;
	lpObj->m_iSkillNPCSantaTime = 0;

	lpObj->m_SkillReduceDefenseTime = 0;
	lpObj->m_SkillReduceDefense = 0;

	lpObj->m_RFBufReduceDefenseTime = 0;
	lpObj->m_RFBufReduceDefense = 0;	
	
	lpObj->m_SkillSleep = 0;
	lpObj->m_SkillSleep_Time = 0;
	lpObj->m_SkillSleep_MoveRange = 0;
	lpObj->m_SkillSleep_AttackRange = 0;
	lpObj->m_SkillSleep_ViewRange = 0;

	lpObj->m_SkillNightTime = 0;
	lpObj->m_SkillNight = 0;
	
	lpObj->m_SkillSummonerBookTime = 0;
	lpObj->m_SkillSummonerBookTypeDmg = 0;
	lpObj->m_SkillSummonerBookSender = 0;
	lpObj->m_SkillSummonerBookDmg = 0;

	lpObj->m_SkillRedStormTime=0;
	lpObj->m_SkillRedStormDmg=0;
	lpObj->m_SkillRedStormSender=0;

	lpObj->m_SkillSwordSlashTime=0;
	lpObj->m_SkillSwordSlashDmg=0;
	lpObj->m_SkillSwordSlashSender=0;

	lpObj->m_SkillExplotionTime = 0;

	lpObj->m_SkillLightingStormTime = 0;
	lpObj->m_SkillLightingStorm = 0;
	lpObj->m_SkillLightingStormSender = 0;
	
	lpObj->m_SkillIT_Number = 0;
	lpObj->m_SkillIT_Time = 0;

	lpObj->m_SkillAttackTime2 = 0;
	lpObj->m_SkillAttack2 = 0;
	lpObj->m_SkillDefenseTime = 0;
	lpObj->m_SkillDefense = 0;
	lpObj->m_ViewSkillState &= ~8;

	lpObj->m_WizardSkillDefenseTime = 0;
	lpObj->m_WizardSkillDefense = 0;
	lpObj->m_ViewSkillState &= ~256;

	lpObj->m_SkillAddLifeTime = 0;
	lpObj->m_SkillAddLife = 0;
	lpObj->m_SkillAddLife_Mana = 0;
	lpObj->m_SkillMagicCircleCriticalDamage = 0;
	lpObj->m_ViewSkillState &= ~16;

	lpObj->m_SkillHardenTime = 0;
	lpObj->m_SkillHarden = 0;
	lpObj->m_ViewSkillState &= ~32;

	lpObj->m_SkillMagumReduceDefenseTime = 0;
	lpObj->m_SkillMagumReduceDefense = 0;
	lpObj->m_ViewSkillState &= ~64;

	lpObj->SkillRecallParty_Time = 0;
	lpObj->SkillAddCriticalDamageTime = 0;
	lpObj->SkillAddCriticalDamage = 0;
	lpObj->m_ViewSkillState &= ~2048;

	lpObj->m_iSkillStunTime = 0;
	lpObj->m_ViewSkillState &= ~4096;

	lpObj->m_iSkillBrandOfSkillTime = 0;
	lpObj->m_ViewSkillState &= ~512;

	lpObj->m_iSkillInvisibleTime = 0;
	lpObj->m_ViewSkillState &= ~16384;

	lpObj->m_iSkillManaSwellTime = 0;
	lpObj->m_iSkillManaSwell = 0;
	lpObj->m_ViewSkillState &= ~8192;

	lpObj->m_PoisonBeattackCount = 0;
	lpObj->m_PoisonType = 0;
	lpObj->m_PoisonAdd = 0;
	lpObj->m_ViewSkillState &= ~1;

	lpObj->m_ColdBeattackCount = 0;
	lpObj->m_IceType = 0;
	lpObj->m_ViewSkillState &= ~2;

	lpObj->m_ImmuneToMagicCount = 0;
	lpObj->m_ImmuneToHarmCount = 0;

	lpObj->m_ViewSkillState &= ~128;
	lpObj->m_ViewSkillState &= ~8;

	lpObj->m_iMuseElfInfinityArrowSkillTime = 0;
	lpObj->m_ViewSkillState &= ~1024;
	return true;
}


void CObjUseSkill::GladiatorsGlory(int iSkillIcon, int aIndex,BOOL state)
{
	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsNewClass(lpObj) == 0)
	{
		if (state)
		{
			lpObj->m_SkillGladiatorsGloryTime=3600;
			lpObj->m_wExprienceRate += ReadConfig.DuelManager__GladiatorsGloryExp;
			lpObj->m_wItemDropRate += ReadConfig.DuelManager__GladiatorsGloryDrop;

			GCSkillInfoSend(lpObj,1,iSkillIcon);
		} else {
			lpObj->m_SkillGladiatorsGloryTime=0;
			lpObj->m_wExprienceRate -= ReadConfig.DuelManager__GladiatorsGloryExp;
			lpObj->m_wItemDropRate -= ReadConfig.DuelManager__GladiatorsGloryDrop;

			GCSkillInfoSend(lpObj,0,iSkillIcon);
		}
	} else {
		GCServerMsgStringSend("GladiatorsGlory is only for NON-Masters!", lpObj->m_Index, 1);
	}
}

BOOL CObjUseSkill::SkillKnightRush(int aIndex, int aTargetIndex, CMagicInf * lpMagic)
{
	if(OBJMAX_RANGE(aIndex) == false)
	{
		return false;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->MapNumber != MAP_INDEX_CASTLESIEGE && lpObj->MapNumber != MAP_INDEX_CASTLEHUNTZONE)
	{
		return false;
	}

	GCMagicAttackNumberSend(lpObj,lpMagic->m_Skill,aTargetIndex,1);
	gObjAttack(lpObj,&gObj[aTargetIndex],lpMagic,0,1,0,0);
	return true;
}




BOOL CObjUseSkill::SkillWizardJavalin(int aIndex, int aTargetIndex, CMagicInf * lpMagic)
{
	if(OBJMAX_RANGE(aIndex) == false)
	{
		return false;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->MapNumber != MAP_INDEX_CASTLESIEGE && lpObj->MapNumber != MAP_INDEX_CASTLEHUNTZONE)
	{
		return false;
	}

	GCMagicAttackNumberSend(lpObj,lpMagic->m_Skill,aTargetIndex,1);

	int iDelay = 500;

	gObjAddAttackProcMsgSendDelay(lpObj,50,aTargetIndex,iDelay,lpMagic->m_Skill,0);
	gObjAddAttackProcMsgSendDelay(lpObj,50,aTargetIndex,iDelay+200,lpMagic->m_Skill,0);
	return true;
}




BOOL CObjUseSkill::SkillElfRush(int aIndex, int aTargetIndex, CMagicInf * lpMagic)
{
	if(OBJMAX_RANGE(aIndex) == false)
	{
		return false;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->MapNumber != MAP_INDEX_CASTLESIEGE && lpObj->MapNumber != MAP_INDEX_CASTLEHUNTZONE)
	{
		return false;
	}

	GCMagicAttackNumberSend(lpObj,lpMagic->m_Skill,aTargetIndex,1);
	gObjAddAttackProcMsgSendDelay(lpObj,50,aTargetIndex,800,lpMagic->m_Skill,0);
	return true;
}




BOOL CObjUseSkill::SkillMagumOneFlash(int aIndex, int aTargetIndex, CMagicInf * lpMagic)
{
	if(OBJMAX_RANGE(aIndex) == false)
	{
		return false;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->MapNumber != MAP_INDEX_CASTLESIEGE && lpObj->MapNumber != MAP_INDEX_CASTLEHUNTZONE)
	{
		return false;
	}

	GCMagicAttackNumberSend(lpObj,lpMagic->m_Skill,aTargetIndex,1);
	gObjAttack(lpObj,&gObj[aTargetIndex],lpMagic,0,1,0,0);
	gObjAddAttackProcMsgSendDelay(lpObj,50,aTargetIndex,100,lpMagic->m_Skill,0);
	return true;
}




BOOL CObjUseSkill::SkillMagumDeathCannon(int aIndex, int aTargetIndex, CMagicInf * lpMagic)
{
	if(OBJMAX_RANGE(aIndex) == false)
	{
		return false;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->MapNumber != MAP_INDEX_CASTLESIEGE && lpObj->MapNumber != MAP_INDEX_CASTLEHUNTZONE)
	{
		return false;
	}

	GCMagicAttackNumberSend(lpObj,lpMagic->m_Skill,aTargetIndex,1);
	gObjAttack(lpObj,&gObj[aTargetIndex],lpMagic,0,1,0,0);
	return true;
}




BOOL CObjUseSkill::SkillDarklordSpaceSplit(int aIndex, int aTargetIndex, CMagicInf * lpMagic)
{
	if(OBJMAX_RANGE(aIndex) == false)
	{
		return false;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->MapNumber != MAP_INDEX_CASTLESIEGE && lpObj->MapNumber != MAP_INDEX_CASTLEHUNTZONE)
	{
		return false;
	}

	GCMagicAttackNumberSend(lpObj,lpMagic->m_Skill,aTargetIndex,1);
	gObjAddAttackProcMsgSendDelay(lpObj,50,aTargetIndex,500,lpMagic->m_Skill,0);
	return true;
}




BOOL CObjUseSkill::SkillBrandOfSkill(int aIndex, int aTargetIndex, CMagicInf * lpMagic)
{
	if(OBJMAX_RANGE(aIndex) == false)
	{
		return false;
	}

	LPOBJ lpObj = &gObj[aIndex];
	int tObjNum;

	if(lpObj->MapNumber != MAP_INDEX_CASTLESIEGE && lpObj->MapNumber != MAP_INDEX_CASTLEHUNTZONE)
	{

		//return false;
	}

	int count = 0;

	lpObj->m_iSkillBrandOfSkillTime = 20;
	lpObj->m_ViewSkillState |= 512;
	GCSkillInfoSend(lpObj,1,0x13);		//Sacrifice Skill

	GCMagicAttackNumberSend(lpObj,AT_SKILL_BRAND_OF_SKILL,lpObj->m_Index,1);

	while( true )
	{
		if(lpObj->VpPlayer2[count].state != 0)
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if(OBJMAX_RANGE(tObjNum))
			{
				if(lpObj->VpPlayer2[count].type == OBJ_USER)
				{
					if(gObjCalDistance(lpObj,&gObj[tObjNum]) < 5)
					{
						gObj[tObjNum].m_iSkillBrandOfSkillTime = 20;
						gObj[tObjNum].m_ViewSkillState |= 512;
						GCSkillInfoSend(&gObj[tObjNum],1,0x13);	//Sacrifice Skill
						GCMagicAttackNumberSend(lpObj,AT_SKILL_BRAND_OF_SKILL,tObjNum,1);
					}
				}
			}
		}

		count++;

		if(count > MAX_VIEWPORT - 1)
		{
			break;
		}
	}
	return true;
}


BOOL CObjUseSkill::SkillStun(int aIndex, int aTargetIndex)
{
	if(OBJMAX_RANGE(aIndex) == false)
	{
		return false;
	}
	if(OBJMAX_RANGE(aTargetIndex) == false)
	{
		return false;
	}

	gObj[aTargetIndex].m_iSkillStunTime = 3;
	gObj[aTargetIndex].m_ViewSkillState |= 4096;
	GCSkillInfoSend(&gObj[aTargetIndex],1,0x3D);
	//GCMagicAttackNumberSend(&gObj[aIndex],AT_SKILL_STUN,aTargetIndex,1);
	gObjSetPosition(aTargetIndex,gObj[aTargetIndex].X,gObj[aTargetIndex].Y);

	return true;
}


BOOL CObjUseSkill::SkillStun(int aIndex, int aTargetIndex, CMagicInf * lpMagic, BYTE btX, BYTE btY, BYTE btDir, BYTE btTargetPos)
{
	if(OBJMAX_RANGE(aIndex) == false)
	{
		return false;
	}

	LPOBJ lpObj = &gObj[aIndex];

	int TargePosx = lpObj->X - (8 - (btTargetPos & 15));
	int TargePosy = lpObj->Y - (8 - ((btTargetPos & 240) >> 4));

	int iangle = this->GetAngle(lpObj->X,lpObj->Y,TargePosx,TargePosy);

	if(lpObj->MapNumber != MAP_INDEX_CASTLESIEGE && lpObj->MapNumber != MAP_INDEX_CASTLEHUNTZONE)
	{
		return false;
	}

	int tObjNum;
	int count = 0;

	gObjSetKillCount(aIndex,0);

	while( true )
	{
		if(lpObj->VpPlayer2[count].state != 0)
		{
			if(lpObj->VpPlayer2[count].type == OBJ_USER || lpObj->VpPlayer2[count].type == OBJ_MONSTER)
			{
				tObjNum = lpObj->VpPlayer2[count].number;

				if(OBJMAX_RANGE(tObjNum))
				{
					if(gObjCalDistance(lpObj,&gObj[tObjNum]) <= 4)
					{
						if(SkillElectricSparkHitBox.HitCheck(iangle,lpObj->X,lpObj->Y,gObj[tObjNum].X,gObj[tObjNum].Y))
						{
							gObj[tObjNum].m_iSkillStunTime = 10;
							gObj[tObjNum].m_ViewSkillState |= 4096;
							GCSkillInfoSend(&gObj[tObjNum],1,0x3D);
							GCMagicAttackNumberSend(lpObj,AT_SKILL_STUN,tObjNum,1);
							gObjSetPosition(tObjNum,gObj[tObjNum].X,gObj[tObjNum].Y);
						}
					}
				}
			}
		}

		count++;

		if(count > MAX_VIEWPORT - 1)
		{
			break;
		}
	}
	return true;
}




BOOL CObjUseSkill::SkillRemoveStun(int aIndex, int aTargetIndex, CMagicInf * lpMagic)
{

	if(OBJMAX_RANGE(aIndex) == false)
	{
		return false;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->MapNumber != MAP_INDEX_CASTLESIEGE && lpObj->MapNumber != MAP_INDEX_CASTLEHUNTZONE)
	{
		return false;
	}

	int tObjNum;
	int count = 0;

	gObjSetKillCount(aIndex,0);
	GCMagicAttackNumberSend(lpObj,AT_SKILL_REMOVAL_STUN,lpObj->m_Index,1);

	while( true )
	{
		if(lpObj->VpPlayer2[count].state != 0)
		{

			tObjNum = lpObj->VpPlayer2[count].number;
			if(OBJMAX_RANGE(tObjNum))
			{
				if(lpObj->VpPlayer2[count].type == OBJ_USER)
				{
					if(gObjCalDistance(lpObj,&gObj[tObjNum]) < 6)
					{
						gObj[tObjNum].m_iSkillStunTime = 0;
						gObj[tObjNum].m_ViewSkillState &= ~4096;
						GCSkillInfoSend(&gObj[tObjNum],0,0x3D);
						GCMagicAttackNumberSend(lpObj,AT_SKILL_REMOVAL_STUN,tObjNum,1);
					}
				}
			}
		}

		count++;

		if(count > MAX_VIEWPORT - 1)
		{
			break;
		}
	}
return true;
}




BOOL CObjUseSkill::SkillAddMana(int aIndex, int aTargetIndex, CMagicInf * lpMagic)
{
	if(OBJMAX_RANGE(aIndex) == false)
	{
		return false;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->MapNumber != MAP_INDEX_CASTLESIEGE && lpObj->MapNumber != MAP_INDEX_CASTLEHUNTZONE)
	{
		return false;
	}

	int tObjNum;
	int count = 0;
	gObjSetKillCount(aIndex,0);

	lpObj->AddMana -= lpObj->m_iSkillManaSwell;

	int iSwelledMana = short(lpObj->MaxMana + lpObj->AddMana) * 50 / 100;
	lpObj->AddMana += iSwelledMana;
	lpObj->m_iSkillManaSwellTime = 60;
	lpObj->m_iSkillManaSwell = iSwelledMana;

	GCManaSend(lpObj->m_Index,lpObj->MaxMana + lpObj->AddMana,0xFE,0,lpObj->MaxBP + lpObj->AddBP);

	if( (lpObj->MaxMana + lpObj->AddMana) < lpObj->Mana)
	{
		lpObj->Mana = lpObj->MaxMana + lpObj->AddMana;
	}

	GCManaSend(lpObj->m_Index,lpObj->Mana,0xFF,0,lpObj->BP);
	GCMagicAttackNumberSend(lpObj,AT_SKILL_ADD_MANA,lpObj->m_Index,1);

	while( true )
	{
		if(lpObj->VpPlayer2[count].state != 0)
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if(OBJMAX_RANGE(tObjNum) && lpObj->VpPlayer2[count].type == OBJ_USER)
			{
				if(gObjCalDistance(lpObj,&gObj[tObjNum]) < 5)
				{
					if(lpObj->GuildNumber > 0 && gObj[tObjNum].GuildNumber > 0)
					{
						if(lpObj->GuildNumber != gObj[tObjNum].GuildNumber)
						{
							count++;
							continue;
						}

						gObj[tObjNum].AddMana -= gObj[tObjNum].m_iSkillManaSwell;
						iSwelledMana = short(gObj[tObjNum].MaxMana + gObj[tObjNum].AddMana) * 50 / 100;
						gObj[tObjNum].AddMana += iSwelledMana;

						gObj[tObjNum].m_iSkillManaSwellTime = 60;
						gObj[tObjNum].m_iSkillManaSwell = iSwelledMana;


						GCManaSend(tObjNum,gObj[tObjNum].MaxMana + gObj[tObjNum].AddMana,0xFE,0,gObj[tObjNum].MaxBP + gObj[tObjNum].AddBP);

						if(gObj[tObjNum].Mana > gObj[tObjNum].MaxMana + gObj[tObjNum].AddMana)
						{
							gObj[tObjNum].Mana = gObj[tObjNum].MaxMana + gObj[tObjNum].AddMana;
						}

						GCManaSend(tObjNum,gObj[tObjNum].Mana,0xFF,0,gObj[tObjNum].BP);

						gObj[tObjNum].m_ViewSkillState |= 8192;
						GCSkillInfoSend(&gObj[tObjNum],1,0x09);
						GCMagicAttackNumberSend(lpObj,AT_SKILL_ADD_MANA,tObjNum,1);
					}
				}
			}
		}

		count++;

		if(count > MAX_VIEWPORT - 1)
		{
			break;
		}
	}
	return true;
}




BOOL CObjUseSkill::SkillCloaking(int aIndex, int aTargetIndex, CMagicInf * lpMagic)
{
	if(OBJMAX_RANGE(aIndex) == false)
	{
		return false;
	}

	if(OBJMAX_RANGE(aTargetIndex) == false)
	{
		return false;
	}

	LPOBJ lpObj = &gObj[aIndex];
	LPOBJ lpTargetObj = &gObj[aTargetIndex];

	if(lpObj->MapNumber != MAP_INDEX_CASTLESIEGE && lpObj->MapNumber != MAP_INDEX_CASTLEHUNTZONE)
	{
		return false;
	}

	if(lpTargetObj->Type != OBJ_USER)
	{
		if(ReadConfig.AHLog == TRUE)
		{
			ANTI_HACK_LOG.Output("[ANTI-HACK][Cloaking Skill] [%s][%s] Invalid Targeting  TargetType [%d] TargetClass [%d]",
				lpObj->AccountID,lpObj->Name,lpTargetObj->Type,lpTargetObj->Class);
		}
		return false;
	}

	gObjSetKillCount(aIndex,0);

	GCMagicAttackNumberSend(lpObj,AT_SKILL_INVISIBLE,lpObj->m_Index,1);
	lpTargetObj->m_iSkillInvisibleTime = 60;
	lpTargetObj->m_ViewSkillState |= 16384;

	GCSkillInfoSend(lpTargetObj,1,0x12);
	GCMagicAttackNumberSend(lpObj,AT_SKILL_INVISIBLE,aTargetIndex,1);
	return true;
}




BOOL CObjUseSkill::SkillRemoveCloaking(int aIndex, int aTargetIndex, CMagicInf * lpMagic)
{
	if(OBJMAX_RANGE(aIndex) == false)
	{
		return false;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->MapNumber != MAP_INDEX_CASTLESIEGE && lpObj->MapNumber != MAP_INDEX_CASTLEHUNTZONE)
	{
		return false;
	}

	int tObjNum;
	int count = 0;

	gObjSetKillCount(aIndex,0);
	GCMagicAttackNumberSend(lpObj,AT_SKILL_REMOVAL_INVISIBLE,lpObj->m_Index,1);

	while ( true )
	{
		if(lpObj->VpPlayer2[count].state != 0)
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if(OBJMAX_RANGE(tObjNum) && lpObj->VpPlayer2[count].type == OBJ_USER)
			{
				if(gObjCalDistance(lpObj,&gObj[tObjNum]) < 5)
				{
					gObj[tObjNum].m_iSkillInvisibleTime = 0;
					gObj[tObjNum].m_ViewSkillState &= ~16384;
					GCSkillInfoSend(&gObj[tObjNum],0,0x12);
					GCMagicAttackNumberSend(lpObj,AT_SKILL_REMOVAL_INVISIBLE,tObjNum,1);
				}
			}
		}

		count++;

		if(count > MAX_VIEWPORT - 1)
		{
			break;
		}
	}
	return true;
}




BOOL CObjUseSkill::SkillRemoveMagic(int aIndex, int aTargetIndex, CMagicInf * lpMagic)
{
	if(OBJMAX_RANGE(aIndex) == false)
	{
		return false;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->MapNumber != MAP_INDEX_CASTLESIEGE && lpObj->MapNumber != MAP_INDEX_CASTLEHUNTZONE)
	{
		return false;
	}

	int tObjNum;
	int count = 0;

	gObjSetKillCount(aIndex,0);
	GCMagicAttackNumberSend(lpObj,AT_SKILL_REMOVAL_MAGIC,lpObj->m_Index,1);

	while ( true )
	{
		if(lpObj->VpPlayer2[count].state != 0)
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if(OBJMAX_RANGE(tObjNum) && lpObj->VpPlayer2[count].type == OBJ_USER)
			{
				if(gObjCalDistance(lpObj,&gObj[tObjNum]) < 5)
				{
					this->RemoveAllCharacterInvalidMagicAndSkillState(&gObj[tObjNum]);
					GCSkillInfoSend(&gObj[tObjNum],0,1048576);	//???
					GCMagicAttackNumberSend(lpObj,AT_SKILL_REMOVAL_MAGIC,tObjNum,1);
				}
			}
		}

		count++;

		if(count > MAX_VIEWPORT - 1)
		{
			break;
		}
	}
	return true;
}


////////////////////////////////////////////////////////////////

BOOL CObjUseSkill::SkillFireScream(int aIndex, int aTargetIndex, CMagicInf * lpMagic)
{
	if(g_SkillAdditionInfo.GetFireScreamSkill() == false)
		return false;
	if( aIndex < OBJ_STARTUSERINDEX || aIndex > OBJMAX-1)
		return FALSE;
	if(OBJMAX_RANGE(aTargetIndex) == false)
		return false;

	LPOBJ lpObj = &gObj[aIndex];
	LPOBJ lpTargetObj = &gObj[aTargetIndex];

	if ( gObjAttackType(lpTargetObj,lpObj) == 0 )
		return false;

	if ( gclassObjAttack.PkCheck(lpObj, lpTargetObj) == FALSE )
		return false;

	if ( gclassObjAttack.CheckAttackArea(lpObj, lpTargetObj) == FALSE )
		return false;

	if(gObjIsConnected(lpObj->m_Index) == false || gObjIsConnected(lpTargetObj->m_Index) == false)
		return false;

	if(lpObj->Type != OBJ_USER)
		return false;

	if(lpObj->Class != CLASS_DARKLORD)
		return false;

	gObjAttack(lpObj,lpTargetObj,lpMagic,0,0,0,0);
	return true;
}



BOOL CObjUseSkill::FireScreamExplosionAttack(LPOBJ lpObj, LPOBJ lpTargetObj, int iAttackDamage)
{
	if(gObjIsConnected(lpObj->m_Index) == false && gObjIsConnected(lpTargetObj->m_Index) == false)
	{
		return false;
	}

	if(rand()%10000 > g_SkillAdditionInfo.GetFireScreamExplosionRate())
	{
		return false;
	}

	int iExplosionDamage = iAttackDamage / 10;
	int iTarObjNum;

	if(iExplosionDamage < 1)
	{
		return true;
	}

	int iCount = 0;
	int EnableAttack;

	while( true )
	{
		if(lpObj->VpPlayer2[iCount].state != 0)
		{
			iTarObjNum = lpObj->VpPlayer2[iCount].number;

			if(iTarObjNum >= 0)
			{
				EnableAttack = 0;
				
				if ( gclassObjAttack.PkCheck(lpObj, &gObj[iTarObjNum]) == FALSE )
				{
					EnableAttack = 0;
				}
				else if ( gclassObjAttack.CheckAttackArea(lpObj, &gObj[iTarObjNum]) == FALSE )
				{
					EnableAttack = 0;
				}
				else if(gObj[iTarObjNum].Type == OBJ_MONSTER && gObj[iTarObjNum].m_RecallMon < 0)
				{
					EnableAttack = 1;
				}
				else if(lpTargetObj->Type == OBJ_USER && lpObj->Type == OBJ_USER && gObj[iTarObjNum].Type != OBJ_NPC)
				{
					EnableAttack = 1;
				}
				else if(lpTargetObj->m_cChaosCastleIndex == gObj[iTarObjNum].m_cChaosCastleIndex && lpTargetObj->m_cChaosCastleIndex != -1)
				{
					EnableAttack = 1;
				}

				if(EnableAttack != 0)
				{
					BYTE bAttackType = gObjAttackType(&gObj[iTarObjNum],lpObj);

					if (bAttackType > 0)
					{
						if(lpObj->m_Index != gObj[iTarObjNum].m_Index)
						{
							int iSkillDistance = gObjCalDistance(lpTargetObj,&gObj[iTarObjNum]);

							if(iSkillDistance <= g_SkillAdditionInfo.GetFireScreamExplosionAttackDistance())
							{
								gObjAddMsgSendDelay(lpObj,54,iTarObjNum,100,iExplosionDamage);
							}
						}
					}
				}
			}
		}

		iCount++;

		if(iCount > MAX_VIEWPORT -1)
		{
			break;
		}
	}
	return true;
}


BOOL CObjUseSkill::RemoveCloakingEffect(int aIndex)
{
	if(OBJMAX_RANGE(aIndex) == false)
	{
		return false;
	}

	gObj[aIndex].m_iSkillInvisibleTime = 0;
	gObj[aIndex].m_ViewSkillState &= ~16384;
	GCSkillInfoSend(&gObj[aIndex],0,0x12);
	return true;
}


void CObjUseSkill::SpecificSkillAdditionTreat(LPOBJ lpObj, LPOBJ lpTargetObj, CMagicInf * lpMagic, int iAttackDamage)
{
	BYTE bySkillNum = lpMagic->m_Skill;

	if((bySkillNum == AT_SKILL_FIRESCREAM) || (bySkillNum >= 525 && bySkillNum <= 529 ))
	{
		gObjUseSkill.FireScreamExplosionAttack(lpObj,lpTargetObj,iAttackDamage);
	}
}

int CObjUseSkill::RemoveSpecificSkillState(LPOBJ lpObj, int iSkillNumber)
{
	if(lpObj->Type != OBJ_USER)
	{
		return false;
	}

	int iViewSkillState = 0;
	int iSkillState = 0;

	if(iSkillNumber == AT_SKILL_ATTACK)
	{
		lpObj->m_SkillAttackTime = 0;
		lpObj->m_SkillAttack = 0;
		iViewSkillState = 4;
		iSkillState = 0x01;
	}
	else if(iSkillNumber == AT_SKILL_DEFENSE)
	{
		lpObj->m_SkillDefenseTime = 0;
		lpObj->m_SkillDefense = 0;
		iViewSkillState = 8;
		iSkillState = 0x02;
	}
	else if(iSkillNumber == AT_SKILL_REFLECT)
	{
		if(lpObj->m_SkillReflectTime > 0)
		{
			lpObj->m_SkillReflectTime = 1;
		}else
		{
			lpObj->m_SkillReflectTime = 0;
			lpObj->m_SkillReflect = 0;
		}
	}
	else if(iSkillNumber == AT_SKILL_REDUCE_DAMAGE)
	{
		lpObj->m_SkillReduceDamageTime = 0;
		lpObj->m_SkillReduceDamage = 0;
	}
	else if(iSkillNumber == AT_SKILL_REDUCE_DEFENSE)
	{
		lpObj->m_SkillReduceDefenseTime = 0;
		lpObj->m_SkillReduceDefense = 0;
	}
	else if(iSkillNumber == AT_SKILL_BERSERKER)
	{
		lpObj->m_SkillBerserkerTime = 0;
		lpObj->m_SkillBerserkerDef = 0;
		lpObj->m_SkillBerserkerAttack = 0;
	}

	lpObj->m_ViewSkillState &= ~iViewSkillState;
	GCSkillInfoSend(lpObj,0,iSkillState);			//FIXED
	return true;
}

int CObjUseSkill::SkillFenrirAttack(int aIndex,int aTargetIndex,CMagicInf * lpMagic)
{
	LPOBJ lpObj = &gObj[aIndex];
	int tObjNum;
	int count = 0;
	int HitCount = 0;
	int bAttack;
	int DuelIndex = lpObj->m_iDuelUser;
	int EnableAttack;

	if(lpObj->Level < 300)
	{
		return false;
	}

	while( true )
	{
		if(lpObj->VpPlayer2[count].state != 0)
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if(tObjNum >= 0)
			{
				EnableAttack = 0;
				
				if ( gclassObjAttack.PkCheck(lpObj, &gObj[tObjNum]) == FALSE )
				{
					EnableAttack = 0;
				}
				else if ( gclassObjAttack.CheckAttackArea(lpObj, &gObj[tObjNum]) == FALSE )
				{
					EnableAttack = 0;
				}
				else if(lpObj->VpPlayer2[count].type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon < 0)
				{
					EnableAttack = 1;
				}
				else if(tObjNum == aTargetIndex || DuelIndex == tObjNum)
				{
					EnableAttack = 1;
				}
				else
				{
					int CallMonIndex = gObj[tObjNum].m_Index;

					if(gObj[tObjNum].Type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon >= 0)
					{
						CallMonIndex = gObj[gObj[tObjNum].m_RecallMon].m_Index;
					}

					if(gObjTargetGuildWarCheck(lpObj,&gObj[CallMonIndex]) == 1)
					{
						EnableAttack = 1;
					}
				}

				if(EnableAttack != 0)
				{
					BYTE bAttackType = gObjAttackType(&gObj[tObjNum],lpObj);

					if (bAttackType > 0)
					{
						if(gObjCalDistance(lpObj,&gObj[tObjNum]) < 6)
						{
							bAttack = 0;

							if(HitCount >= 5)
							{
								bAttack = 0;
							}
							else
							{
								bAttack = 1;
							}

							if(tObjNum == aTargetIndex)
							{
								bAttack = 1;
							}

							if(bAttack != 0)
							{
								gObjAddAttackProcMsgSendDelay(lpObj,53,tObjNum,300,lpMagic->m_Skill,0);
								HitCount++;
							}
						}
					}
				}
			}
		}

		count++;

		if(count > MAX_VIEWPORT - 1)
		{
			break;
		}
	}
	return true;
}

int CObjUseSkill::SkillInfinityArrow(int aIndex,int aTargetIndex,CMagicInf * lpMagic)
{
	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->Type != OBJ_USER)
	{
		return false;
	}

	if(lpObj->Class != CLASS_ELF || lpObj->ChangeUP == 0)
	{
		return false;
	}

	if(lpObj->m_Index != aTargetIndex)
	{
		return false;
	}

	if(lpObj->m_iMuseElfInfinityArrowSkillTime > 0)
	{
		return false;
	}

	lpObj->m_iMuseElfInfinityArrowSkillTime = g_SkillAdditionInfo.GetInfinityArrowSkillTime();
	lpObj->m_ViewSkillState |= 1024;
	GCSkillInfoSend(lpObj,1,0x06);
	GCMagicAttackNumberSend(lpObj,0x4D,lpObj->m_Index,1);
	LogAddTD("[%s][%s] Use Infinity Arrow Skill (Time:%d)(Character Level : %d)(ChangeUp: %d)",
		lpObj->AccountID,lpObj->Name,lpObj->m_iMuseElfInfinityArrowSkillTime,lpObj->Level,lpObj->ChangeUP);
	return true;
}

int CObjUseSkill::GetAddUseMana(LPOBJ lpObj,CMagicInf * lpMagic)
{
	int iRetValue = 0;

	if(lpObj->Class == CLASS_ELF && lpObj->Type == OBJ_USER && lpObj->ChangeUP == 1 && lpObj->m_iMuseElfInfinityArrowSkillTime > 0)
	{
		if(lpMagic->m_Skill == AT_SKILL_CROSSBOW
			|| lpMagic->m_Skill == AT_SKILL_BOW
			|| lpMagic->m_Skill == AT_SKILL_ICEARROW
			|| lpMagic->m_Skill == AT_SKILL_PENETRATION
			|| lpMagic->m_Skill == AT_SKILL_5CROSSBOW			//Skill Does not exist
			|| lpMagic->m_Skill == AT_SKILL_FIVE_SHOT
			|| lpMagic->m_Skill == 490
			|| lpMagic->m_Skill == 491
			|| lpMagic->m_Skill == 492
			|| lpMagic->m_Skill == 493
			|| lpMagic->m_Skill == 494)
		{
			int iArrowLevel = 0;

			CItem * Right = &lpObj->pInventory[0];
			CItem * Left = &lpObj->pInventory[1];

			if(Right->m_Type >= ITEMGET(4,8) && Right->m_Type <= ITEMGET(4,14)
				|| Right->m_Type == ITEMGET(4,16)
				|| Right->m_Type == ITEMGET(4,18)
				|| Right->m_Type == ITEMGET(4,19))
			{
				if(Left->m_Type == ITEMGET(4,7))
				{
					iArrowLevel = Left->m_Level;
				}
			}
			else if(Left->m_Type >= ITEMGET(4,0) && Left->m_Type <= ITEMGET(4,6)
				|| Left->m_Type == ITEMGET(4,17)
				|| Left->m_Type == ITEMGET(4,20)
				|| Left->m_Type == ITEMGET(4,21)
				|| Left->m_Type == ITEMGET(4,22)
				|| Left->m_Type == ITEMGET(4,23))
			{
				if(Right->m_Type == ITEMGET(4,15))
				{
					iArrowLevel = Right->m_Level;
				}
			}

			if(iArrowLevel == 0)
			{
				iRetValue = g_SkillAdditionInfo.GetInfinityArrowMPConsumptionPlus0();
			}
			else if(iArrowLevel == 1)
			{
				iRetValue = g_SkillAdditionInfo.GetInfinityArrowMPConsumptionPlus1();
			}
			else if(iArrowLevel == 2)
			{
				iRetValue = g_SkillAdditionInfo.GetInfinityArrowMPConsumptionPlus2();
			}
		}
	}
	return iRetValue;
}