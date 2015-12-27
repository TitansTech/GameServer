#include "stdafx.h"
#include "ObjCalCharacter.h"
#include "Gamemain.h"
#include "DarkSpirit.h"
#include "ItemAddOption.h"
#include "..\common\SetItemOption.h"
#include "LogProc.h"
#include "Mastering.h"
#include "ObjUseSkill.h"
#include "Mastering2.h"
#include "CrystalWall.h"

#include "XMasEvent.h"



// GS-N 0.99.60T 0x004A4740
//	GS-N	1.00.18	JPN	0x004C2650	-	Completed

void gObjCalCharacter(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];
	if ( lpObj->Type != OBJ_USER )
	{
		return;
	}

	int Strength = 0;
	int Dexterity = 0;
	int Vitality = 0;
	int Energy = 0;
	CItem * Right = &lpObj->pInventory[0];
	CItem * Left  = &lpObj->pInventory[1];
	CItem * Gloves = &lpObj->pInventory[5];
	CItem * Amulet = &lpObj->pInventory[9];
	CItem * Helper = &lpObj->pInventory[8];
	lpObj->HaveWeaponInHand = true;

	if ( Right->IsItem() == FALSE && Left->IsItem() == FALSE )
	{
		lpObj->HaveWeaponInHand = false;
	}
	else if ( Left->IsItem() == FALSE && Right->m_Type == ITEMGET(4,15) )	// Arrow
	{
		lpObj->HaveWeaponInHand = false;
	}
	else if ( Right->IsItem() == FALSE )
	{
		int iType = Left->m_Type / MAX_SUBTYPE_ITEMS;

		if ( Left->m_Type == ITEMGET(4,7) ) // Bolt
		{
			lpObj->HaveWeaponInHand = false;
		}
		else if ( iType == 6 )
		{
			lpObj->HaveWeaponInHand = false;
		}
	}

	lpObj->m_wExprienceRate = 100;
	lpObj->m_wItemDropRate = 100;

	lpObj->AddLife = 0;
	lpObj->AddMana = 0;
	lpObj->MonsterDieGetMoney = 0;
	lpObj->MonsterDieGetLife = 0;
	lpObj->MonsterDieGetMana = 0;
	lpObj->DamageReflect = 0;
	lpObj->DamageMinus = 0;
	lpObj->SkillLongSpearChange = false;

	int iItemIndex;
	BOOL bIsChangeItem;	// lc34

	for ( iItemIndex=0; iItemIndex<MAX_PLAYER_EQUIPMENT;iItemIndex++)
	{
		if ( lpObj->pInventory[iItemIndex].IsItem() != FALSE )
		{
			lpObj->pInventory[iItemIndex].m_IsValidItem = true;
		}
	}

	//===================================================================================================
	//Apply Ancient Items
	//===================================================================================================
	do
	{
		lpObj->SetOpAddMaxAttackDamage = 0;
		lpObj->SetOpAddMinAttackDamage = 0;
		lpObj->SetOpAddDamage = 0;
		lpObj->SetOpIncAGValue = 0;
		lpObj->SetOpAddCriticalDamageSuccessRate = 0;
		lpObj->SetOpAddCriticalDamage = 0;
		lpObj->SetOpAddExDamageSuccessRate = 0;
		lpObj->SetOpAddExDamage = 0;
		lpObj->SetOpAddSkillAttack = 0;
		lpObj->AddStrength = 0;
		lpObj->AddDexterity = 0;
		lpObj->AddVitality = 0;
		lpObj->AddEnergy = 0;
		lpObj->AddBP = 0;
		lpObj->iAddShield = 0;
		lpObj->SetOpAddAttackDamage = 0;
		lpObj->SetOpAddDefence = 0;
		lpObj->SetOpAddMagicPower = 0;
		lpObj->SetOpAddDefenceRate = 0;
		lpObj->SetOpIgnoreDefense = 0;
		lpObj->SetOpDoubleDamage = 0;
		lpObj->SetOpTwoHandSwordImproveDamage = 0;
		lpObj->SetOpImproveSuccessAttackRate = 0;
		lpObj->SetOpReflectionDamage = 0;
		lpObj->SetOpImproveSheldDefence = 0;
		lpObj->SetOpDecreaseAG = 0;
		lpObj->SetOpImproveItemDropRate = 0;
		lpObj->IsFullSetItem = false;
		memset(lpObj->m_AddResistance, 0, sizeof(lpObj->m_AddResistance));
		bIsChangeItem = 0;
		
		gObjCalcSetItemStat(lpObj);
		gObjCalcSetItemOption(lpObj);

		for (iItemIndex=0;iItemIndex<MAX_PLAYER_EQUIPMENT;iItemIndex++)
		{
			if ( lpObj->pInventory[iItemIndex].IsItem() != FALSE && lpObj->pInventory[iItemIndex].m_IsValidItem != false )
			{
				if ( gObjValidItem( lpObj, &lpObj->pInventory[iItemIndex], iItemIndex) != FALSE )
				{
					lpObj->pInventory[iItemIndex].m_IsValidItem = true;
				}
				else
				{
					lpObj->pInventory[iItemIndex].m_IsValidItem  = false;
					bIsChangeItem = TRUE;
				}
			}
		}
	}
	while ( bIsChangeItem != FALSE );

	//===================================================================================================
	//Skill Effects: Vitality, Dexterity, Strenght, Energy, Command
	//===================================================================================================
	if( lpObj->m_SkillFitnessTime > 0 )		//Fitness Skill Fix
	{
		lpObj->AddVitality = lpObj->AddVitality + lpObj->m_SkillFitnessVal;
	}

	g_ItemAddOption.ApplyInventoryItemsStats(lpObj);
	for(int i=0;i<2;i++)
	{
		if ( lpObj->m_iItemEffectValidTime[i] > 0 )	//Fix item stats effects
		{
			g_ItemAddOption.SetItemLastEffectStats(lpObj,i);
		}
	}
	//===================================================================================================

	Strength = lpObj->Strength + lpObj->AddStrength;
	Dexterity = lpObj->Dexterity + lpObj->AddDexterity;
	Vitality = lpObj->Vitality + lpObj->AddVitality;
	Energy = lpObj->Energy + lpObj->AddEnergy;

	if ( lpObj->Class == CLASS_ELF ) // Elf
	{
		if ( (Right->m_Type >= ITEMGET(4,8) && Right->m_Type < ITEMGET(4,15) ) ||
			 (Left->m_Type >= ITEMGET(4,0) && Left->m_Type < ITEMGET(4,7)) ||
			  Right->m_Type == ITEMGET(4,16) ||
			  Left->m_Type == ITEMGET(4,20) ||
			  Left->m_Type == ITEMGET(4,21) ||
			  Left->m_Type == ITEMGET(4,22) ||
			  Left->m_Type == ITEMGET(4,23) ||
			  Right->m_Type == ITEMGET(4,18) ||
			  Right->m_Type == ITEMGET(4,19) ||
			  Left->m_Type == ITEMGET(4,17) )
		{
			if ( (Right->IsItem() != FALSE && Right->m_IsValidItem == false) || (Left->IsItem() != FALSE && Left->m_IsValidItem == false) )
			{
				lpObj->m_AttackDamageMinRight = (Dexterity + Strength ) / ReadConfig.gObjCalCharacter_AttackDamageMin_Bow_StrDexDiv_Elf;
				lpObj->m_AttackDamageMaxRight = (Dexterity + Strength ) / ReadConfig.gObjCalCharacter_AttackDamageMax_Bow_StrDexDiv_Elf;
				lpObj->m_AttackDamageMinLeft = (Dexterity + Strength ) / ReadConfig.gObjCalCharacter_AttackDamageMin_Bow_StrDexDiv_Elf;
				lpObj->m_AttackDamageMaxLeft = (Dexterity + Strength ) / ReadConfig.gObjCalCharacter_AttackDamageMax_Bow_StrDexDiv_Elf;
			}
			else
			{
				lpObj->m_AttackDamageMinRight = (Dexterity / ReadConfig.gObjCalCharacter_AttackDamageMin_NoBow_DexDiv_Elf) + (Strength / ReadConfig.gObjCalCharacter_AttackDamageMin_NoBow_StrDiv_Elf);
				lpObj->m_AttackDamageMaxRight = (Dexterity / ReadConfig.gObjCalCharacter_AttackDamageMax_NoBow_DexDiv_Elf) + (Strength / ReadConfig.gObjCalCharacter_AttackDamageMax_NoBow_StrDiv_Elf );
				lpObj->m_AttackDamageMinLeft = (Dexterity / ReadConfig.gObjCalCharacter_AttackDamageMin_NoBow_DexDiv_Elf) + (Strength / ReadConfig.gObjCalCharacter_AttackDamageMin_NoBow_StrDiv_Elf);
				lpObj->m_AttackDamageMaxLeft = (Dexterity / ReadConfig.gObjCalCharacter_AttackDamageMax_NoBow_DexDiv_Elf) + (Strength / ReadConfig.gObjCalCharacter_AttackDamageMax_NoBow_StrDiv_Elf );
			}
		}
		else
		{
			lpObj->m_AttackDamageMinRight = (Dexterity + Strength) / ReadConfig.gObjCalCharacter_AttackDamageMin_DexStrDiv_Elf;
			lpObj->m_AttackDamageMaxRight = (Dexterity + Strength) / ReadConfig.gObjCalCharacter_AttackDamageMax_DexStrDiv_Elf;
			lpObj->m_AttackDamageMinLeft = (Dexterity + Strength) / ReadConfig.gObjCalCharacter_AttackDamageMin_DexStrDiv_Elf;
			lpObj->m_AttackDamageMaxLeft = (Dexterity + Strength) / ReadConfig.gObjCalCharacter_AttackDamageMax_DexStrDiv_Elf;
		}
	}
	else if ( lpObj->Class == CLASS_KNIGHT ) // Dark Knight
	{
		lpObj->m_AttackDamageMinRight = Strength / ReadConfig.gObjCalCharacter_AttackDamageMin_StrDiv_DK;
		lpObj->m_AttackDamageMaxRight = Strength / ReadConfig.gObjCalCharacter_AttackDamageMax_StrDiv_DK;
		lpObj->m_AttackDamageMinLeft = Strength / ReadConfig.gObjCalCharacter_AttackDamageMin_StrDiv_DK;
		lpObj->m_AttackDamageMaxLeft = Strength / ReadConfig.gObjCalCharacter_AttackDamageMax_StrDiv_DK;
	}
	else if (lpObj->Class == CLASS_MAGICGLADIATOR) // MAgic Gladiator
	{
		lpObj->m_AttackDamageMinRight = (Strength / ReadConfig.gObjCalCharacter_AttackDamageMin_StrDiv_MG) + (Energy / ReadConfig.gObjCalCharacter_AttackDamageMin_EneDiv_MG);
		lpObj->m_AttackDamageMaxRight = (Strength / ReadConfig.gObjCalCharacter_AttackDamageMax_StrDiv_MG) + (Energy / ReadConfig.gObjCalCharacter_AttackDamageMax_EneDiv_MG);
		lpObj->m_AttackDamageMinLeft = (Strength / ReadConfig.gObjCalCharacter_AttackDamageMin_StrDiv_MG) + (Energy / ReadConfig.gObjCalCharacter_AttackDamageMin_EneDiv_MG);
		lpObj->m_AttackDamageMaxLeft = (Strength / ReadConfig.gObjCalCharacter_AttackDamageMax_StrDiv_MG) + (Energy / ReadConfig.gObjCalCharacter_AttackDamageMax_EneDiv_MG);
	}
	else if (lpObj->Class == CLASS_RAGEFIGHTER) // RF
	{
		lpObj->m_AttackDamageMinRight = (Strength / ReadConfig.gObjCalCharacter_AttackDamageMin_StrDiv_RF) + (Vitality / ReadConfig.gObjCalCharacter_AttackDamageMin_VitDiv_RF);
		lpObj->m_AttackDamageMaxRight = (Strength / ReadConfig.gObjCalCharacter_AttackDamageMax_StrDiv_RF) + (Vitality / ReadConfig.gObjCalCharacter_AttackDamageMax_VitDiv_RF);
		lpObj->m_AttackDamageMinLeft = (Strength / ReadConfig.gObjCalCharacter_AttackDamageMin_StrDiv_RF) + (Vitality / ReadConfig.gObjCalCharacter_AttackDamageMin_VitDiv_RF);
		lpObj->m_AttackDamageMaxLeft = (Strength / ReadConfig.gObjCalCharacter_AttackDamageMax_StrDiv_RF) + (Vitality / ReadConfig.gObjCalCharacter_AttackDamageMax_VitDiv_RF);
	}
	else if ( lpObj->Class == CLASS_DARKLORD ) // Dark Lord
	{
		lpObj->m_AttackDamageMinRight = (Strength / ReadConfig.gObjCalCharacter_AttackDamageMin_StrDiv_DL) + (Energy / ReadConfig.gObjCalCharacter_AttackDamageMin_EneDiv_DL);
		lpObj->m_AttackDamageMaxRight = (Strength / ReadConfig.gObjCalCharacter_AttackDamageMax_StrDiv_DL) + (Energy / ReadConfig.gObjCalCharacter_AttackDamageMax_EneDiv_DL);
		lpObj->m_AttackDamageMinLeft = (Strength / ReadConfig.gObjCalCharacter_AttackDamageMin_StrDiv_DL) + (Energy / ReadConfig.gObjCalCharacter_AttackDamageMin_EneDiv_DL);
		lpObj->m_AttackDamageMaxLeft = (Strength / ReadConfig.gObjCalCharacter_AttackDamageMax_StrDiv_DL) + (Energy / ReadConfig.gObjCalCharacter_AttackDamageMax_EneDiv_DL);
	}
	else if ( lpObj->Class == CLASS_WIZARD ) // 
	{
		lpObj->m_AttackDamageMinRight = (Strength / ReadConfig.gObjCalCharacter_AttackDamageMin_StrDiv_DW);
		lpObj->m_AttackDamageMaxRight = (Strength / ReadConfig.gObjCalCharacter_AttackDamageMax_StrDiv_DW);
		lpObj->m_AttackDamageMinLeft = (Strength / ReadConfig.gObjCalCharacter_AttackDamageMin_StrDiv_DW);
		lpObj->m_AttackDamageMaxLeft = (Strength / ReadConfig.gObjCalCharacter_AttackDamageMax_StrDiv_DW);
	}
	else if ( lpObj->Class == CLASS_SUMMONER ) // 
	{
		lpObj->m_AttackDamageMinRight = (Strength / ReadConfig.gObjCalCharacter_AttackDamageMin_StrDiv_SU);
		lpObj->m_AttackDamageMaxRight = (Strength / ReadConfig.gObjCalCharacter_AttackDamageMax_StrDiv_SU);
		lpObj->m_AttackDamageMinLeft = (Strength / ReadConfig.gObjCalCharacter_AttackDamageMin_StrDiv_SU);
		lpObj->m_AttackDamageMaxLeft = (Strength / ReadConfig.gObjCalCharacter_AttackDamageMax_StrDiv_SU);
	}
	else
	{
		lpObj->m_AttackDamageMinRight = (Strength / ReadConfig.gObjCalCharacter_AttackDamageMin_StrDiv_DW);
		lpObj->m_AttackDamageMaxRight = (Strength / ReadConfig.gObjCalCharacter_AttackDamageMax_StrDiv_DW);
		lpObj->m_AttackDamageMinLeft = (Strength / ReadConfig.gObjCalCharacter_AttackDamageMin_StrDiv_DW);
		lpObj->m_AttackDamageMaxLeft = (Strength / ReadConfig.gObjCalCharacter_AttackDamageMax_StrDiv_DW);
	}

	lpObj->pInventory[7].PlusSpecial(&lpObj->m_AttackDamageMinRight, 80);
	lpObj->pInventory[7].PlusSpecial(&lpObj->m_AttackDamageMaxRight, 80);
	lpObj->pInventory[7].PlusSpecial(&lpObj->m_AttackDamageMinLeft, 80);
	lpObj->pInventory[7].PlusSpecial(&lpObj->m_AttackDamageMaxLeft, 80);

	int AddLeadership = 0;

	if ( lpObj->pInventory[7].IsItem() != FALSE && lpObj->pInventory[7].m_IsValidItem != false)
	{
		AddLeadership += lpObj->pInventory[7].m_Leadership;
	}

	if ( Right->m_Type != -1 )
	{
		if ( Right->m_IsValidItem != false )
		{
			if ( Right->m_Type >= ITEMGET(5,0) && Right->m_Type < ITEMGET(6,0) )
			{
				lpObj->m_AttackDamageMinRight += Right->m_DamageMin / 2;
				lpObj->m_AttackDamageMaxRight += Right->m_DamageMax / 2;
			}
			else
			{
				lpObj->m_AttackDamageMinRight += Right->m_DamageMin;
				lpObj->m_AttackDamageMaxRight += Right->m_DamageMax;
			}

			if(lpObj->Class == CLASS_RAGEFIGHTER)
			{
				if ( Right->m_Type >= ITEMGET(0,32) && Right->m_Type <= ITEMGET(0,35))
				{
					//lpObj->m_AttackDamageMinRight += (Right->m_DamageMin + Left->m_DamageMin)/2;
					//lpObj->m_AttackDamageMaxRight += (Right->m_DamageMax + Left->m_DamageMax)/2;

					int pMin = lpObj->m_AttackDamageMinRight * (ReadConfig.gObjCalCharacter_AttackDamageMin_PercentPlus_RF/100.0f + 1.0f);
					int pMax = lpObj->m_AttackDamageMaxRight * (ReadConfig.gObjCalCharacter_AttackDamageMax_PercentPlus_RF/100.0f + 1.0f);

					lpObj->m_AttackDamageMinRight=pMin;
					lpObj->m_AttackDamageMaxRight=pMax;
				}
			}
		}

		if ( lpObj->pInventory[0].m_SkillChange != FALSE )
		{
			lpObj->SkillLongSpearChange = true;
		}

		lpObj->pInventory[0].PlusSpecial(&lpObj->m_AttackDamageMinRight, 80);
		lpObj->pInventory[0].PlusSpecial(&lpObj->m_AttackDamageMaxRight, 80);
	}
		
	if ( Left->m_Type != -1 )
	{
		if ( Left->m_IsValidItem != false)
		{
			lpObj->m_AttackDamageMinLeft += Left->m_DamageMin;
			lpObj->m_AttackDamageMaxLeft += Left->m_DamageMax;
		}

		if(lpObj->Class == CLASS_RAGEFIGHTER)
		{
			if ( Left->m_Type >= ITEMGET(0,32) && Left->m_Type <= ITEMGET(0,35))
			{
				//lpObj->m_AttackDamageMinLeft += (Right->m_DamageMin + Left->m_DamageMin)/2;
				//lpObj->m_AttackDamageMaxLeft += (Right->m_DamageMax + Left->m_DamageMax)/2;

				int pMin = lpObj->m_AttackDamageMinLeft * (ReadConfig.gObjCalCharacter_AttackDamageMin_PercentPlus_RF/100.0f + 1.0f);
				int pMax = lpObj->m_AttackDamageMaxLeft * (ReadConfig.gObjCalCharacter_AttackDamageMax_PercentPlus_RF/100.0f + 1.0f);

				lpObj->m_AttackDamageMinLeft=pMin;
				lpObj->m_AttackDamageMaxLeft=pMax;
			}
		}

		lpObj->pInventory[1].PlusSpecial(&lpObj->m_AttackDamageMinLeft, 80);
		lpObj->pInventory[1].PlusSpecial(&lpObj->m_AttackDamageMaxLeft, 80);
	}

	lpObj->m_CriticalDamage = 0;
	lpObj->m_ExcelentDamage = 0;

	lpObj->pInventory[0].PlusSpecial(&lpObj->m_CriticalDamage, 84);
	lpObj->pInventory[1].PlusSpecial(&lpObj->m_CriticalDamage, 84);
	lpObj->pInventory[2].PlusSpecial(&lpObj->m_CriticalDamage, 84);
	lpObj->pInventory[3].PlusSpecial(&lpObj->m_CriticalDamage, 84);
	lpObj->pInventory[4].PlusSpecial(&lpObj->m_CriticalDamage, 84);
	lpObj->pInventory[5].PlusSpecial(&lpObj->m_CriticalDamage, 84);
	lpObj->pInventory[6].PlusSpecial(&lpObj->m_CriticalDamage, 84);
	lpObj->pInventory[7].PlusSpecial(&lpObj->m_CriticalDamage, 84);

	if ( lpObj->Class == CLASS_WIZARD )
	{
		lpObj->m_MagicDamageMin = Energy / ReadConfig.gObjCalCharacter_m_MagicDamageMin_Div_DW;
		lpObj->m_MagicDamageMax = Energy / ReadConfig.gObjCalCharacter_m_MagicDamageMax_Div_DW;
	}else if (lpObj->Class == CLASS_RAGEFIGHTER)
	{
		lpObj->m_MagicDamageMin = Energy / ReadConfig.gObjCalCharacter_m_MagicDamageMin_Div_RF;
		lpObj->m_MagicDamageMax = Energy / ReadConfig.gObjCalCharacter_m_MagicDamageMax_Div_RF;
	}else if (lpObj->Class == CLASS_MAGICGLADIATOR)
	{
		lpObj->m_MagicDamageMin = Energy / ReadConfig.gObjCalCharacter_m_MagicDamageMin_Div_MG;
		lpObj->m_MagicDamageMax = Energy / ReadConfig.gObjCalCharacter_m_MagicDamageMax_Div_MG;
	}else if (lpObj->Class == CLASS_SUMMONER)
	{
		lpObj->m_MagicDamageMin = Energy / ReadConfig.gObjCalCharacter_m_MagicDamageMin_Div_SU;
		lpObj->m_MagicDamageMax = Energy / ReadConfig.gObjCalCharacter_m_MagicDamageMax_Div_SU;
	}else if (lpObj->Class == CLASS_ELF)
	{
		lpObj->m_MagicDamageMin = Energy / ReadConfig.gObjCalCharacter_m_MagicDamageMin_Div_Elf;
		lpObj->m_MagicDamageMax = Energy / ReadConfig.gObjCalCharacter_m_MagicDamageMax_Div_Elf;
	}else if (lpObj->Class == CLASS_KNIGHT)
	{
		lpObj->m_MagicDamageMin = Energy / ReadConfig.gObjCalCharacter_m_MagicDamageMin_Div_DK;
		lpObj->m_MagicDamageMax = Energy / ReadConfig.gObjCalCharacter_m_MagicDamageMax_Div_DK;
	}else if (lpObj->Class == CLASS_DARKLORD)
	{
		lpObj->m_MagicDamageMin = Energy / ReadConfig.gObjCalCharacter_m_MagicDamageMin_Div_DL;
		lpObj->m_MagicDamageMax = Energy / ReadConfig.gObjCalCharacter_m_MagicDamageMax_Div_DL;
	}else
	{
		lpObj->m_MagicDamageMin = Energy / ReadConfig.gObjCalCharacter_m_MagicDamageMin_Div_DW;
		lpObj->m_MagicDamageMax = Energy / ReadConfig.gObjCalCharacter_m_MagicDamageMin_Div_DW;
	}

	lpObj->pInventory[7].PlusSpecial(&lpObj->m_MagicDamageMin, 81);
	lpObj->pInventory[7].PlusSpecial(&lpObj->m_MagicDamageMax, 81);

	if ( Right->m_Type != -1 )
	{
		if ( lpObj->pInventory[0].m_Type == ITEMGET(0,31) ||
			 lpObj->pInventory[0].m_Type == ITEMGET(0,21) ||
			 lpObj->pInventory[0].m_Type == ITEMGET(0,23) ||
			 lpObj->pInventory[0].m_Type == ITEMGET(0,25) ) 
		{
			lpObj->pInventory[0].PlusSpecial(&lpObj->m_MagicDamageMin, 80);
			lpObj->pInventory[0].PlusSpecial(&lpObj->m_MagicDamageMax, 80);
		}
		else
		{
			lpObj->pInventory[0].PlusSpecial(&lpObj->m_MagicDamageMin, 81);
			lpObj->pInventory[0].PlusSpecial(&lpObj->m_MagicDamageMax, 81);
		}
	}

	lpObj->m_AttackRating = (Strength + Dexterity) / 2;
	lpObj->m_AttackRating += lpObj->pInventory[5].ItemDefense();

	if ( lpObj->Class == CLASS_ELF ) // Elf
	{
		lpObj->m_AttackSpeed = Dexterity / ReadConfig.gObjCalCharacter_AttackSpeed_Div_Elf;
		lpObj->m_MagicSpeed = Dexterity / ReadConfig.gObjCalCharacter_MagicSpeed_Div_Elf;
	}
	else if ( lpObj->Class == CLASS_KNIGHT ) // Dark Knigh
	{
		lpObj->m_AttackSpeed = Dexterity / ReadConfig.gObjCalCharacter_AttackSpeed_Div_DK;
		lpObj->m_MagicSpeed = Dexterity / ReadConfig.gObjCalCharacter_MagicSpeed_Div_DK;
	}
	else if ( lpObj->Class == CLASS_MAGICGLADIATOR ) // MG
	{
		lpObj->m_AttackSpeed = Dexterity / ReadConfig.gObjCalCharacter_AttackSpeed_Div_MG;
		lpObj->m_MagicSpeed = Dexterity / ReadConfig.gObjCalCharacter_MagicSpeed_Div_MG;
	}
	else if ( lpObj->Class == CLASS_RAGEFIGHTER ) // RF
	{
		lpObj->m_AttackSpeed = Dexterity / ReadConfig.gObjCalCharacter_AttackSpeed_Div_RF;
		lpObj->m_MagicSpeed = Dexterity / ReadConfig.gObjCalCharacter_MagicSpeed_Div_RF;
	}
	else if ( lpObj->Class == CLASS_DARKLORD ) // DarkLord
	{
		lpObj->m_AttackSpeed = Dexterity / ReadConfig.gObjCalCharacter_AttackSpeed_Div_DL;
		lpObj->m_MagicSpeed = Dexterity / ReadConfig.gObjCalCharacter_MagicSpeed_Div_DL;
	}
	else if ( lpObj->Class == CLASS_WIZARD )
	{
		lpObj->m_AttackSpeed = Dexterity / ReadConfig.gObjCalCharacter_AttackSpeed_Div_DW;
		lpObj->m_MagicSpeed = Dexterity / ReadConfig.gObjCalCharacter_MagicSpeed_Div_DW;
	}
	else if ( lpObj->Class == CLASS_SUMMONER )
	{
		lpObj->m_AttackSpeed = Dexterity / ReadConfig.gObjCalCharacter_AttackSpeed_Div_SU;
		lpObj->m_MagicSpeed = Dexterity / ReadConfig.gObjCalCharacter_MagicSpeed_Div_SU;
	}
	else
	{
		lpObj->m_AttackSpeed = Dexterity / ReadConfig.gObjCalCharacter_AttackSpeed_Div_DW;
		lpObj->m_MagicSpeed = Dexterity / ReadConfig.gObjCalCharacter_MagicSpeed_Div_DW;
	}

	bool bRight = false;
	bool bLeft = false;

	if ( Right->m_Type != ITEMGET(4,7) && Right->m_Type != ITEMGET(4,15) && Right->m_Type >= ITEMGET(0,0) && Right->m_Type < ITEMGET(6,0) )
	{
		if ( Right->m_IsValidItem != false )
		{
			bRight = true;
		}
	}

	if ( Left->m_Type != ITEMGET(4,7) && Left->m_Type != ITEMGET(4,15) && Left->m_Type >= ITEMGET(0,0) && Left->m_Type < ITEMGET(6,0) )
	{
		if ( Left->m_IsValidItem != false )
		{
			bLeft = true;
		}
	}

	if ( bRight != false && bLeft != false )
	{
		lpObj->m_AttackSpeed += (Right->m_AttackSpeed + Left->m_AttackSpeed)/2;
		lpObj->m_MagicSpeed += (Right->m_AttackSpeed + Left->m_AttackSpeed)/2;
	}
	else if ( bRight!= false )
	{
		lpObj->m_AttackSpeed += Right->m_AttackSpeed;
		lpObj->m_MagicSpeed += Right->m_AttackSpeed;
	}
	else if ( bLeft != false )
	{
		lpObj->m_AttackSpeed += Left->m_AttackSpeed;
		lpObj->m_MagicSpeed += Left->m_AttackSpeed;
	}

	if ( Gloves->m_Type != -1 )
	{
		if ( Gloves->m_IsValidItem != false )
		{
			lpObj->m_AttackSpeed += Gloves->m_AttackSpeed;
			lpObj->m_MagicSpeed += Gloves->m_AttackSpeed;
		}
	}

	if ( Helper->m_Type != -1 )
	{
		if ( Helper->m_IsValidItem != false )
		{
			lpObj->m_AttackSpeed += Helper->m_AttackSpeed;
			lpObj->m_MagicSpeed += Helper->m_AttackSpeed;
		}
	}

	if ( Amulet->m_Type != -1 )
	{
		if ( Amulet->m_IsValidItem != false )
		{
			lpObj->m_AttackSpeed += Amulet->m_AttackSpeed;
			lpObj->m_MagicSpeed += Amulet->m_AttackSpeed;
		}
	}

	//===================================================================================================
	//Calculate Success Blocking
	//===================================================================================================
	if ( lpObj->Class == CLASS_ELF )
	{
		lpObj->m_SuccessfulBlocking = Dexterity / ReadConfig.gObjCalCharacter_Blocking_Div_Elf;
	}
	else if ( lpObj->Class == CLASS_DARKLORD )
	{
		lpObj->m_SuccessfulBlocking = Dexterity / ReadConfig.gObjCalCharacter_Blocking_Div_DL;
	}
	else if ( lpObj->Class == CLASS_RAGEFIGHTER )
	{
		lpObj->m_SuccessfulBlocking = Dexterity / ReadConfig.gObjCalCharacter_Blocking_Div_RF;
	}
	else if ( lpObj->Class == CLASS_WIZARD )
	{
		lpObj->m_SuccessfulBlocking = Dexterity / ReadConfig.gObjCalCharacter_Blocking_Div_DW;
	}
	else if ( lpObj->Class == CLASS_KNIGHT )
	{
		lpObj->m_SuccessfulBlocking = Dexterity / ReadConfig.gObjCalCharacter_Blocking_Div_DK;
	}
	else if ( lpObj->Class == CLASS_MAGICGLADIATOR )
	{
		lpObj->m_SuccessfulBlocking = Dexterity / ReadConfig.gObjCalCharacter_Blocking_Div_MG;
	}
	else if ( lpObj->Class == CLASS_SUMMONER )
	{
		lpObj->m_SuccessfulBlocking = Dexterity / ReadConfig.gObjCalCharacter_Blocking_Div_SU;
	}
	else
	{
		lpObj->m_SuccessfulBlocking = Dexterity / ReadConfig.gObjCalCharacter_Blocking_Div_DW;
	}

	//===================================================================================================
	//Fernir [13 37] = 0x1A25
	//===================================================================================================
	if ( lpObj->pInventory[8].IsItem() == TRUE && lpObj->pInventory[8].m_Type == ITEMGET(13,37) )
	{
		if ( (lpObj->pInventory[8].m_NewOption &1) )		//Offence
		{
			//	1 = attack power 10% increase
		}
		else if ( (lpObj->pInventory[8].m_NewOption &2) )	//Deffence
		{
			//	2 = defense power 10% increase
		}
		else if ( (lpObj->pInventory[8].m_NewOption &4) )	//Gold
		{
			//	4 = life 200 increase
			//		mana 200 increase
			//		attack power 33% increase
			//		wizzardy 16% increase 
			int increaseAttack = 0;
			int increaseWizzardy = 0;

			if (lpObj->Class == CLASS_DARKLORD || 
				lpObj->Class == CLASS_MAGICGLADIATOR || 
				lpObj->Class == CLASS_RAGEFIGHTER )
			{
				lpObj->AddLife += 37;			//life 37 increase
				lpObj->AddMana += 37;			//mana 37 increase

				increaseAttack = 16;
				increaseWizzardy = 2;
			} else {
				lpObj->AddLife += 200;			//life 200 increase
				lpObj->AddMana += 200;			//mana 200 increase

				increaseAttack = 33;
				increaseWizzardy = 16;
			}

			//attack power 33% increase
			lpObj->m_AttackDamageMinRight += lpObj->m_AttackDamageMinRight * increaseAttack / 100;
			lpObj->m_AttackDamageMaxRight += lpObj->m_AttackDamageMaxRight * increaseAttack / 100;
			lpObj->m_AttackDamageMinLeft += lpObj->m_AttackDamageMinLeft * increaseAttack / 100;
			lpObj->m_AttackDamageMaxLeft += lpObj->m_AttackDamageMaxLeft * increaseAttack / 100;

			//wizzardy 16% increase 
			lpObj->m_MagicDamageMin += lpObj->m_MagicDamageMin * increaseWizzardy / 100;
			lpObj->m_MagicDamageMax += lpObj->m_MagicDamageMax * increaseWizzardy / 100;
		}
	}

	//===================================================================================================
	//New Wings S4 = [12 36] [12 37] [12 38] [12 39] [12 40] [12 43];
	//	1 = ignore opponent defensive power by 5%
	//	2 = enemy attack power it returns with 5% probabilities
	//	4 = life 5% recovery
	//	8 = mana 5% recovery	
	//===================================================================================================
	if (( lpObj->pInventory[7].m_Type >= ITEMGET(12,36) && 
		lpObj->pInventory[7].m_Type <= ITEMGET(12,40) ) ||		
#if (CRYSTAL_EDITION == 1)
		( lpObj->pInventory[7].m_Type >= ITEMGET(12,200) && 
		lpObj->pInventory[7].m_Type <= ITEMGET(12,254) ) ||
#endif
		lpObj->pInventory[7].m_Type == ITEMGET(12,43) ||
		lpObj->pInventory[7].m_Type == ITEMGET(12,50) )
	{
		//Wizzardy DMG Fix + Regular DMG
		if ( lpObj->pInventory[7].m_NewOption > 15 )
		{
			if ( lpObj->pInventory[7].m_Z28Option > 0)
			{
				lpObj->SetOpAddSkillAttack += lpObj->pInventory[7].m_Z28Option * 4;
				lpObj->SetOpAddDamage += lpObj->pInventory[7].m_Z28Option * 4;
			}
		}
	}

	//===================================================================================================
	// Wizard Ring effect (13,20)
	//===================================================================================================
	if ((lpObj->pInventory[10].IsItem() == TRUE && lpObj->pInventory[10].m_Type == ITEMGET(13,20) && lpObj->pInventory[10].m_Level == 0 && lpObj->pInventory[10].m_Durability > 0.0f ) || 
		(lpObj->pInventory[11].IsItem() == TRUE && lpObj->pInventory[11].m_Type == ITEMGET(13,20) && lpObj->pInventory[11].m_Level == 0 && lpObj->pInventory[11].m_Durability > 0.0f ) )
	{
		lpObj->m_AttackDamageMinRight += lpObj->m_AttackDamageMinRight * 10 / 100;
		lpObj->m_AttackDamageMaxRight += lpObj->m_AttackDamageMaxRight * 10 / 100;
		lpObj->m_AttackDamageMinLeft += lpObj->m_AttackDamageMinLeft * 10 / 100;
		lpObj->m_AttackDamageMaxLeft += lpObj->m_AttackDamageMaxLeft * 10 / 100;
		lpObj->m_MagicDamageMin += lpObj->m_MagicDamageMin * 10 / 100;
		lpObj->m_MagicDamageMax += lpObj->m_MagicDamageMax * 10 / 100;
		lpObj->m_AttackSpeed += 10;
		lpObj->m_MagicSpeed += 10;
	}
	//===================================================================================================
	//Lethal Wizard's Ring (13,107)
	//===================================================================================================
	if ((lpObj->pInventory[10].IsItem() == TRUE && lpObj->pInventory[10].m_Type == ITEMGET(13,107) && lpObj->pInventory[10].m_Durability > 0.0f ) || 
		(lpObj->pInventory[11].IsItem() == TRUE && lpObj->pInventory[11].m_Type == ITEMGET(13,107) && lpObj->pInventory[11].m_Durability > 0.0f ) )
	{
		lpObj->m_AttackDamageMinRight += lpObj->m_AttackDamageMinRight * 15 / 100;
		lpObj->m_AttackDamageMaxRight += lpObj->m_AttackDamageMaxRight * 15 / 100;
		lpObj->m_AttackDamageMinLeft += lpObj->m_AttackDamageMinLeft * 15 / 100;
		lpObj->m_AttackDamageMaxLeft += lpObj->m_AttackDamageMaxLeft * 15 / 100;
		lpObj->m_MagicDamageMin += lpObj->m_MagicDamageMin * 15 / 100;
		lpObj->m_MagicDamageMax += lpObj->m_MagicDamageMax * 15 / 100;
		lpObj->m_AttackSpeed += 10;
		lpObj->m_MagicSpeed += 10;
	}
	//===================================================================================================

	if ( Left->m_Type != -1 )
	{
		if ( Left->m_IsValidItem != false )
		{
			lpObj->m_SuccessfulBlocking += Left->m_SuccessfulBlocking;
			lpObj->pInventory[1].PlusSpecial(&lpObj->m_SuccessfulBlocking, 82);
		}
	}

	bool Success = true;

	//0		Right
	//1		Left
	//2		Helmet
	//3		Armor
	//4		Pants
	//5		Gloves
	//6		Boots
	//7		Wing
	//8		Pet
	//9		Pendant
	//10	Ring
	//11	Ring
	if ( lpObj->Class == CLASS_MAGICGLADIATOR ) // MG
	{
		for ( int j=3;j<=6;j++)		//Skip Helmet
		{
			if ( lpObj->pInventory[j].m_Type == -1 )
			{
				Success = false;
				break;
			}

			if ( lpObj->pInventory[j].m_IsValidItem == false )
			{
				Success = false;
				break;
			}
		}
	}
	else
	{
		for  ( int k=2;k<=6;k++)
		{
			if (k == 5)		//Skip Gloves
			{
				if (lpObj->Class == CLASS_RAGEFIGHTER)
					continue;
			}

			if ( lpObj->pInventory[k].m_Type == -1 )
			{
				Success = false;
				break;
			}

			if ( lpObj->pInventory[k].m_IsValidItem == false )
			{
				Success = false;
				break;
			}
		}
	}

	int Level11Count = 0;
	int Level10Count = 0;
	int Level12Count = 0;
	int Level13Count = 0;

	if ( Success != false )
	{
		int in;

		if ( lpObj->Class == CLASS_MAGICGLADIATOR ) // MG
		{
			in = lpObj->pInventory[3].m_Type % MAX_SUBTYPE_ITEMS;

			if ( in != ITEMGET(0,15) &&	//Storm Crow Armor
				 in != ITEMGET(0,20) &&	//Thunder Hawk Armor
				 in != ITEMGET(0,23) && //Hurricane Armor
				 in != ITEMGET(0,32) && //Volcano Armor
				 in != ITEMGET(0,33) && //Sunlight Armor
				 in != ITEMGET(0,37) &&	//Valiant Armor
				 in != ITEMGET(0,47) &&	//Destory Armor
				 in != ITEMGET(0,48) &&	//Phantom Armor
				 in != ITEMGET(0,71)	//Atlantis Armor
				 )
			{
				Success = false;
			}
			else
			{
				Level13Count++;

				for (int m=3;m<=6;m++)
				{
					if ( in != ( lpObj->pInventory[m].m_Type % MAX_SUBTYPE_ITEMS) )
					{
						Success = false;
					}

					if ( lpObj->pInventory[m].m_Level > 12 )
					{
						Level13Count++;
					}
					else if ( lpObj->pInventory[m].m_Level > 11 )
					{
						Level12Count++;
					}
					else if ( lpObj->pInventory[m].m_Level > 10 )
					{
						Level11Count++;
					}
					else if ( lpObj->pInventory[m].m_Level > 9 )
					{
						Level10Count++;
					}
				}
			}
		}
		else
		{
			in = lpObj->pInventory[2].m_Type % MAX_SUBTYPE_ITEMS;

			for (int m=2;m<=6;m++)
			{
				if ( m == 5)
				{
					if ( lpObj->Class == CLASS_RAGEFIGHTER)
						continue;
				}

				if ( in !=  ( lpObj->pInventory[m].m_Type % MAX_SUBTYPE_ITEMS) )
				{
					Success = false;
				}

				if ( lpObj->pInventory[m].m_Level > 12 )
				{
					Level13Count++;
				}
				else if ( lpObj->pInventory[m].m_Level > 11 )
				{
					Level12Count++;
				}
				else if ( lpObj->pInventory[m].m_Level > 10 )
				{
					Level11Count++;
				}
				else if ( lpObj->pInventory[m].m_Level > 9 )
				{
					Level10Count++;
				}
			}
		}

		if ( Success != false )	// #warning unuseful if
		{
			lpObj->m_SuccessfulBlocking += lpObj->m_SuccessfulBlocking / 10;
		}
	}

	if ( lpObj->Class == CLASS_ELF ) // Elf
	{
		lpObj->m_Defense = Dexterity / ReadConfig.gObjCalCharacter_Defense_Div_Elf;
	}
	else if ( lpObj->Class == CLASS_KNIGHT ) // DK
	{
		lpObj->m_Defense = Dexterity / ReadConfig.gObjCalCharacter_Defense_Div_DK;
	}
	else if ( lpObj->Class == CLASS_DARKLORD ) // DL
	{
		lpObj->m_Defense = Dexterity / ReadConfig.gObjCalCharacter_Defense_Div_DL;
	}
	else if ( lpObj->Class == CLASS_RAGEFIGHTER ) // RF
	{
		lpObj->m_Defense = Dexterity / ReadConfig.gObjCalCharacter_Defense_Div_RF;
	}
	else if ( lpObj->Class == CLASS_WIZARD )
	{
		lpObj->m_Defense = Dexterity / ReadConfig.gObjCalCharacter_Defense_Div_DW;
	}
	else if ( lpObj->Class == CLASS_MAGICGLADIATOR )
	{
		lpObj->m_Defense = Dexterity / ReadConfig.gObjCalCharacter_Defense_Div_MG;
	}
	else if ( lpObj->Class == CLASS_SUMMONER )
	{
		lpObj->m_Defense = Dexterity / ReadConfig.gObjCalCharacter_Defense_Div_SU;
	}
	else
	{
		lpObj->m_Defense = Dexterity / ReadConfig.gObjCalCharacter_Defense_Div_DW;
	}

	lpObj->m_Defense += lpObj->pInventory[2].ItemDefense();
	lpObj->m_Defense += lpObj->pInventory[3].ItemDefense();
	lpObj->m_Defense += lpObj->pInventory[4].ItemDefense();
	lpObj->m_Defense += lpObj->pInventory[5].ItemDefense();
	lpObj->m_Defense += lpObj->pInventory[6].ItemDefense();
	lpObj->m_Defense += lpObj->pInventory[1].ItemDefense();
	lpObj->m_Defense += lpObj->pInventory[7].ItemDefense();

	if ( lpObj->pInventory[8].IsItem() != FALSE )
	{
		if ( lpObj->pInventory[8].m_Type == ITEMGET(13,4) && lpObj->pInventory[8].m_Durability > 0.0f )	// Dark Horse
		{
			lpObj->m_Defense += INT( Dexterity / ReadConfig.gObjCalCharacterDakHorseDiv + 5 + Helper->m_PetItem_Level * 2 );
		}
	}

	if ( (Level13Count + Level12Count + Level11Count + Level10Count) >= 5 )
	{
		if ( Success != false )
		{
			if ( Level13Count == 5 )
			{
				lpObj->m_Defense += lpObj->m_Defense * 20 / 100;
			}
			else if ( Level12Count == 5 || (Level12Count + Level13Count) == 5 )
			{
				lpObj->m_Defense += lpObj->m_Defense * 15 / 100;
			}
			else if ( Level11Count == 5 || (Level11Count + Level12Count + Level13Count) == 5 )
			{
				lpObj->m_Defense += lpObj->m_Defense * 10 / 100;
			}
			else if ( Level10Count == 5 || (Level10Count + Level11Count + Level12Count + Level13Count) == 5)
			{
				lpObj->m_Defense += lpObj->m_Defense * 5 / 100;
			}
		}
	}

	lpObj->m_Defense = lpObj->m_Defense * 10 / 20;

	if ( lpObj->m_Change == 9 )
	{

	}
	else if ( lpObj->m_Change == 41 )
	{
		lpObj->AddLife = ((int)(lpObj->MaxLife * 20.0f))/100;
	}
	else if ( lpObj->m_Change == 372 )
	{
		lpObj->m_Defense += lpObj->m_Defense / 10;
		lpObj->AddLife += lpObj->Level;
	}

	if ( lpObj->pInventory[8].m_Type == ITEMGET(13,0) )
	{
		lpObj->AddLife += 50;
	}

	int addlife = 0;
	int addmana = 0;

	lpObj->pInventory[7].PlusSpecial(&addlife, 100);
	lpObj->pInventory[7].PlusSpecial(&addmana, 101);
	lpObj->pInventory[7].PlusSpecial(&AddLeadership, 105);
	lpObj->AddLeadership = AddLeadership;
	lpObj->AddLife += addlife;
	lpObj->AddMana += addmana;


	//===================================================================================================
	//Skill Effects: Life and Mana
	//===================================================================================================
	if(lpObj->m_SkillAddLifeTime > 0)			//Fix Add Life Skill
	{
		lpObj->AddLife += lpObj->m_SkillAddLife;
	}
	if(lpObj->m_iSkillManaSwellTime > 0)		//Fix Mana Swell Skill
	{
		lpObj->AddMana += lpObj->m_iSkillManaSwell;
	}
	for(int i=0;i<2;i++)
	{
		if ( lpObj->m_iItemEffectValidTime[i] > 0 )	//Fix item Special Effect
		{
			g_ItemAddOption.SetItemLastEffectLifeMana(lpObj,i);
		}
	}
	//===================================================================================================


	if ( lpObj->pInventory[8].m_Type == ITEMGET(13,3) )
	{
		lpObj->pInventory[8].PlusSpecial(&lpObj->AddBP, 103 );
	}

	if ( lpObj->Type == OBJ_USER )
	{
		gDarkSpirit[lpObj->m_Index].Set(lpObj->m_Index, &lpObj->pInventory[1]);
	}

	lpObj->pInventory[9].PlusSpecialPercentEx(&lpObj->AddBP, lpObj->MaxBP, 173);
	lpObj->pInventory[10].PlusSpecialPercentEx(&lpObj->AddMana, lpObj->MaxMana, 172);
	lpObj->pInventory[11].PlusSpecialPercentEx(&lpObj->AddMana, lpObj->MaxMana, 172);

	CItem * rItem[3];
	rItem[0] = &lpObj->pInventory[10];
	rItem[1] = &lpObj->pInventory[11];
	rItem[2] = &lpObj->pInventory[9];

	/*#define GET_MAX_RESISTANCE(x,y,z) ( ( ( ( (x) > (y) ) ? (x) : (y) ) > (z) ) ? ( ( (x) > (y) ) ? (x) : (y) ) : (z) )	
	lpObj->m_Resistance[R_POISON] = GET_MAX_RESISTANCE(rItem[0]->m_Resistance[R_POISON], rItem[1]->m_Resistance[R_POISON], rItem[2]->m_Resistance[R_POISON]);
	lpObj->m_Resistance[R_ICE] = GET_MAX_RESISTANCE(rItem[0]->m_Resistance[R_ICE], rItem[1]->m_Resistance[R_ICE], rItem[2]->m_Resistance[R_ICE]);
	lpObj->m_Resistance[R_LIGHTNING] = GET_MAX_RESISTANCE(rItem[0]->m_Resistance[R_LIGHTNING], rItem[1]->m_Resistance[R_LIGHTNING], rItem[2]->m_Resistance[R_LIGHTNING]);
	lpObj->m_Resistance[R_FIRE] = GET_MAX_RESISTANCE(rItem[0]->m_Resistance[R_FIRE], rItem[1]->m_Resistance[R_FIRE], rItem[2]->m_Resistance[R_FIRE]);
	lpObj->m_Resistance[R_EARTH] = GET_MAX_RESISTANCE(rItem[0]->m_Resistance[R_EARTH], rItem[1]->m_Resistance[R_EARTH], rItem[2]->m_Resistance[R_EARTH]);
	lpObj->m_Resistance[R_WIND] = GET_MAX_RESISTANCE(rItem[0]->m_Resistance[R_WIND], rItem[1]->m_Resistance[R_WIND], rItem[2]->m_Resistance[R_WIND]);
	lpObj->m_Resistance[R_WATER] = GET_MAX_RESISTANCE(rItem[0]->m_Resistance[R_WATER], rItem[1]->m_Resistance[R_WATER], rItem[2]->m_Resistance[R_WATER]);*/

	lpObj->m_Resistance[R_POISON] = (rItem[0]->m_Resistance[R_POISON]+rItem[1]->m_Resistance[R_POISON]+rItem[2]->m_Resistance[R_POISON])&255;
	lpObj->m_Resistance[R_ICE] = (rItem[0]->m_Resistance[R_ICE]+rItem[1]->m_Resistance[R_ICE]+rItem[2]->m_Resistance[R_ICE])&255;
	lpObj->m_Resistance[R_LIGHTNING] = (rItem[0]->m_Resistance[R_LIGHTNING]+rItem[1]->m_Resistance[R_LIGHTNING]+rItem[2]->m_Resistance[R_LIGHTNING])&255;
	lpObj->m_Resistance[R_FIRE] = (rItem[0]->m_Resistance[R_FIRE]+rItem[1]->m_Resistance[R_FIRE]+rItem[2]->m_Resistance[R_FIRE])&255;
	lpObj->m_Resistance[R_EARTH] = (rItem[0]->m_Resistance[R_EARTH]+rItem[1]->m_Resistance[R_EARTH]+rItem[2]->m_Resistance[R_EARTH])&255;
	lpObj->m_Resistance[R_WIND] = (rItem[0]->m_Resistance[R_WIND]+rItem[1]->m_Resistance[R_WIND]+rItem[2]->m_Resistance[R_WIND])&255;
	lpObj->m_Resistance[R_WATER] = (rItem[0]->m_Resistance[R_WATER]+rItem[1]->m_Resistance[R_WATER]+rItem[2]->m_Resistance[R_WATER])&255;


	//===================================================================================================
	//
	//===================================================================================================
	if(ReadConfig.S6E2 == 0)
	{
		Mastering.CalcMagicPower(lpObj);
	}
	else
	{
		lpObj->MasterCharacterInfo->Clear();
		Mastering2.Passive(lpObj->m_Index);
		Mastering2.ApplyPassiveSkills(lpObj->m_Index);
	}

	gObjSantaBuffApply(lpObj);
	g_kItemSystemFor380.ApplyFor380Option(lpObj);
	g_kJewelOfHarmonySystem.SetApplyStrengthenItem(lpObj); 
	GObjExtItemApply(lpObj);
	gObjSetItemApply(lpObj);
	gObjNextExpCal(lpObj);
	g_ItemAddOption.ApplyInventoryItems(lpObj);	//Apply Item Effects (Angel, daemon, skeleton, panda)
	gObjSocketItemApply(lpObj);

	for(int i=0;i<2;i++)
	{
		if ( lpObj->m_iItemEffectValidTime[i] > 0 )
		{
			g_ItemAddOption.SetItemLastEffectOther(lpObj,i);
		}
	}
	//===================================================================================================

	if ( (Left->m_Type >= ITEMGET(4,0) && Left->m_Type < ITEMGET(4,7) ) ||
		  Left->m_Type == ITEMGET(4,17) ||
		  Left->m_Type == ITEMGET(4,20) ||
		  Left->m_Type == ITEMGET(4,21) ||
		  Left->m_Type == ITEMGET(4,22) ||
		  Left->m_Type == ITEMGET(4,23)  )
	{
		if ( Right->m_Type == ITEMGET(4,15) && Right->m_Level == 1 )
		{
			lpObj->m_AttackDamageMinLeft += (WORD)(lpObj->m_AttackDamageMinLeft * 0.03f + 1.0f);
			lpObj->m_AttackDamageMaxLeft += (WORD)(lpObj->m_AttackDamageMaxLeft * 0.03f + 1.0f);
		}
		else if ( Right->m_Type == ITEMGET(4,15) && Right->m_Level == 2 )
		{
			lpObj->m_AttackDamageMinLeft += (WORD)(lpObj->m_AttackDamageMinLeft * 0.05f + 1.0f);
			lpObj->m_AttackDamageMaxLeft += (WORD)(lpObj->m_AttackDamageMaxLeft * 0.05f + 1.0f);
		}
	}
	else if ( (Right->m_Type >= ITEMGET(4,8) && Right->m_Type < ITEMGET(4,15) ) ||
		      (Right->m_Type >= ITEMGET(4,16) && Right->m_Type < ITEMGET(5,0)) )
	{
		if ( Left->m_Type == ITEMGET(4,7) && Left->m_Level == 1 )
		{
				lpObj->m_AttackDamageMinRight += (WORD)(lpObj->m_AttackDamageMinRight * 0.03f + 1.0f);
				lpObj->m_AttackDamageMaxRight += (WORD)(lpObj->m_AttackDamageMaxRight * 0.03f + 1.0f);
		}
		else if ( Left->m_Type == ITEMGET(4,7) && Left->m_Level == 2 )
		{
				lpObj->m_AttackDamageMinRight += (WORD)(lpObj->m_AttackDamageMinRight * 0.05f + 1.0f);
				lpObj->m_AttackDamageMaxRight += (WORD)(lpObj->m_AttackDamageMaxRight * 0.05f + 1.0f);
		}
	}

	if (lpObj->Class == CLASS_KNIGHT || 
		lpObj->Class == CLASS_MAGICGLADIATOR || 
		lpObj->Class == CLASS_DARKLORD || 
		lpObj->Class == CLASS_RAGEFIGHTER  )
	{
		if ( Right->m_Type != -1 && Left->m_Type != -1 )
		{
			if (Right->m_Type >= ITEMGET(0,0) && Right->m_Type < ITEMGET(4,0) && 
				Left->m_Type >= ITEMGET(0,0) && Left->m_Type < ITEMGET(4,0) )
			{
				lpObj->m_AttackDamageMinRight = lpObj->m_AttackDamageMinRight * 55 / 100;
				lpObj->m_AttackDamageMaxRight = lpObj->m_AttackDamageMaxRight * 55 / 100;
				lpObj->m_AttackDamageMinLeft = lpObj->m_AttackDamageMinLeft * 55 / 100;
				lpObj->m_AttackDamageMaxLeft = lpObj->m_AttackDamageMaxLeft * 55 / 100;
			}
		}
	}

	//===================================================================================================
	//New Skill Effects FIX [Season 4,5]
	//===================================================================================================
	if(lpObj->m_SkillGladiatorsGloryTime > 0)
	{
		if(gObjIsNewClass(lpObj) == 0)
		{
			lpObj->m_wExprienceRate += ReadConfig.DuelManager__GladiatorsGloryExp;
			lpObj->m_wItemDropRate += ReadConfig.DuelManager__GladiatorsGloryDrop;
		}
	}
	if(lpObj->m_SkillMagicCircleTime > 0)
	{
		lpObj->m_MagicDamageMin += lpObj->m_SkillMagicCircle;
		if(ReadConfig.S6E2 == 1)
		{
			if(lpObj->MasterCharacterInfo->IncExpansionWizardy2 > 0)
			{
				lpObj->m_CriticalDamage += (lpObj->m_CriticalDamage * lpObj->MasterCharacterInfo->IncExpansionWizardy2) / 100.0f;
			}
		}
	}
	if(lpObj->m_SkillReflectTime > 0)
	{			
		lpObj->DamageReflect += lpObj->m_SkillReflect;
	}
	if(lpObj->Type == OBJ_USER && lpObj->Class == CLASS_SUMMONER && lpObj->m_SkillBerserkerTime > 0)
	{
		lpObj->m_Defense = lpObj->m_Defense * lpObj->m_SkillBerserkerDef;

		lpObj->m_AttackDamageMinRight = lpObj->m_AttackDamageMinRight * lpObj->m_SkillBerserkerAttack;
		lpObj->m_AttackDamageMaxRight = lpObj->m_AttackDamageMaxRight * lpObj->m_SkillBerserkerAttack;
		lpObj->m_AttackDamageMinLeft = lpObj->m_AttackDamageMinLeft * lpObj->m_SkillBerserkerAttack;
		lpObj->m_AttackDamageMaxLeft = lpObj->m_AttackDamageMaxLeft * lpObj->m_SkillBerserkerAttack;
		lpObj->m_MagicDamageMin = lpObj->m_MagicDamageMin * lpObj->m_SkillBerserkerAttack;
		lpObj->m_MagicDamageMax = lpObj->m_MagicDamageMax * lpObj->m_SkillBerserkerAttack;

		lpObj->AddLife = lpObj->AddLife * 0.85;
		lpObj->AddMana = lpObj->AddMana * 1.15;
	}

	gObjCalcShieldPoint(lpObj);
	gObjSetBP(lpObj->m_Index);

	//===================================================================================================
	//FIX AND SEND LIFE/MANA/SD/AG
	//===================================================================================================
	gObjFixGagesOverflow(lpObj);
	GCReFillSend(lpObj->m_Index, lpObj->MaxLife + lpObj->AddLife, 0xFE, 0, lpObj->iMaxShield + lpObj->iAddShield);
	GCManaSend(lpObj->m_Index, lpObj->MaxMana + lpObj->AddMana, 0xFE, 0, lpObj->MaxBP + lpObj->AddBP);

	if ( lpObj->Life > ( lpObj->MaxLife + lpObj->AddLife ) )
	{
		lpObj->Life = lpObj->MaxLife + lpObj->AddLife;
		GCReFillSend(lpObj->m_Index, lpObj->Life, 0xFF, 0, lpObj->iShield);
	}
	if ( lpObj->Mana > ( lpObj->MaxMana + lpObj->AddMana ) )
	{
		lpObj->Mana = lpObj->MaxMana + lpObj->AddMana;
		GCManaSend(lpObj->m_Index, lpObj->Mana, 0xFF, 0, lpObj->BP);
	}

	//===================================================================================================
	//ANTI-HACK Settings
	//===================================================================================================
	if ( lpObj->Class == CLASS_WIZARD || lpObj->Class == CLASS_SUMMONER )
	{
		lpObj->m_DetectSpeedHackTime = (gAttackSpeedTimeLimit - (lpObj->m_MagicSpeed*2 * gDecTimePerAttackSpeed) );
	}
	else
	{
		lpObj->m_DetectSpeedHackTime = (gAttackSpeedTimeLimit - (lpObj->m_AttackSpeed * gDecTimePerAttackSpeed) );
	}

	if ( lpObj->m_DetectSpeedHackTime < gMinimumAttackSpeedTime )
	{
		lpObj->m_DetectSpeedHackTime = gMinimumAttackSpeedTime;
	}

#if (PACK_EDITION>=3)
#if (CRYSTAL_EDITION == 1)
	if(CrystalWall.SpeedMax_UseMaxLimit == 1)
	{
		if(lpObj->m_AttackSpeed > CrystalWall.SpeedMax_Physic)
			lpObj->m_AttackSpeed = CrystalWall.SpeedMax_Physic;

		if(lpObj->m_MagicSpeed > CrystalWall.SpeedMax_Magic)
			lpObj->m_MagicSpeed = CrystalWall.SpeedMax_Magic;
	}
#endif
#endif
}

void gObjFixGagesOverflow(LPOBJ lpObj)
{
	if(ReadConfig.FixMaxGage == 1)
	{
		//Check if Max Stats overflow
		if(lpObj->iMaxShield + lpObj->iAddShield > MAX_SD_VALUE)
		{
			lpObj->iMaxShield = MAX_SD_VALUE;
			lpObj->iAddShield = 0;

			if(ReadConfig.ShowMaxGageReached == 1)
				GCServerMsgStringSend ("[Info] Maximum SD reached!",lpObj->m_Index,0x01 ) ;
		}

		if(lpObj->MaxBP + lpObj->AddBP > MAX_BP_VALUE)
		{
			lpObj->MaxBP = MAX_BP_VALUE;
			lpObj->AddBP = 0;

			if(ReadConfig.ShowMaxGageReached == 1)
				GCServerMsgStringSend ("[Info] Maximum BP reached!",lpObj->m_Index,0x01 ) ;
		}

		if(lpObj->MaxLife + lpObj->AddLife > MAX_LIFE_VALUE)
		{
			lpObj->MaxLife = MAX_LIFE_VALUE;
			lpObj->AddLife = 0;

			if(ReadConfig.ShowMaxGageReached == 1)
				GCServerMsgStringSend ("[Info] Maximum HP reached!",lpObj->m_Index,0x01 ) ;
		}

		if(lpObj->MaxMana + lpObj->AddMana > MAX_MANA_VALUE)
		{
			lpObj->MaxMana = MAX_MANA_VALUE;
			lpObj->AddMana = 0;

			if(ReadConfig.ShowMaxGageReached == 1)
				GCServerMsgStringSend ("[Info] Maximum Mana reached!",lpObj->m_Index,0x01 ) ;
		}

		//Check if Anything is out of bound
		if (lpObj->AddLife<0) lpObj->AddLife = 0;
		if (lpObj->AddMana<0) lpObj->AddMana = 0;
		if (lpObj->AddBP<0) lpObj->AddBP = 0;
		if (lpObj->iAddShield<0) lpObj->iAddShield = 0;

		//Check if Current Stats overflow
		bool SendLife = false;
		bool SendMana = false;

		if ( lpObj->iShield > ( lpObj->iMaxShield + lpObj->iAddShield ) )
		{
			lpObj->iShield = lpObj->iMaxShield + lpObj->iAddShield;
			SendLife = true;
		}
		if ( lpObj->Life > ( lpObj->MaxLife + lpObj->AddLife ) )
		{
			lpObj->Life = lpObj->MaxLife + lpObj->AddLife;
			SendLife = true;
		}
		if ( lpObj->Mana > ( lpObj->MaxMana + lpObj->AddMana ) )
		{
			lpObj->Mana = lpObj->MaxMana + lpObj->AddMana;
			SendMana = true;
		}
		if ( lpObj->BP > ( lpObj->MaxBP + lpObj->AddBP ) )
		{
			lpObj->BP = lpObj->MaxBP + lpObj->AddBP;
			SendMana = true;
		}

		if(SendLife)
			GCReFillSend(lpObj->m_Index, lpObj->Life, 0xFF, 0, lpObj->iShield);

		if(SendMana)
			GCManaSend(lpObj->m_Index, lpObj->Mana, 0xFF, 0, lpObj->BP);
	}
}

void gObjSantaBuffApply(LPOBJ lpObj)
{
	switch(lpObj->m_iSkillNPCSantaType)
	{
		case 474://orange
		{
			lpObj->AddLife += 500;
		}break;
		case 471://blue
		{
			lpObj->AddMana += 500;
		}break;
		case 470://red
		{
			lpObj->m_AttackDamageMinRight += lpObj->m_AttackDamageMinRight * 30 / 100;
			lpObj->m_AttackDamageMaxRight += lpObj->m_AttackDamageMaxRight * 30 / 100;
			lpObj->m_AttackDamageMinLeft += lpObj->m_AttackDamageMinLeft * 30 / 100;
			lpObj->m_AttackDamageMaxLeft += lpObj->m_AttackDamageMaxLeft * 30 / 100;
			lpObj->m_MagicDamageMin += lpObj->m_MagicDamageMin * 30 / 100;
			lpObj->m_MagicDamageMax += lpObj->m_MagicDamageMax * 30 / 100;
		}break;
		case 469://green
		{
			lpObj->m_Defense += 100;
		}break;
		case 475://pink
		{
			lpObj->m_Defense += 100;
		}break;
		case 468://yellow(purple)
		{
			lpObj->AddBP += 25;
		}break;
#if (PACK_EDITION>=2)
		case 465://Santa Claus: Blessing Buff
		case 476://Evil Santa: Blessing Buff
		{
			//lpObj->m_AttackDamageMin += lpObj->m_AttackDamageMin * XMasEvent.BuffAttIncrease / 100;
			//lpObj->m_AttackDamageMax += lpObj->m_AttackDamageMax * XMasEvent.BuffAttIncrease / 100;
			lpObj->m_AttackDamageMinRight += lpObj->m_AttackDamageMinRight * XMasEvent.BuffAttIncrease / 100;
			lpObj->m_AttackDamageMaxRight += lpObj->m_AttackDamageMaxRight * XMasEvent.BuffAttIncrease / 100;
			lpObj->m_AttackDamageMinLeft += lpObj->m_AttackDamageMinLeft * XMasEvent.BuffAttIncrease / 100;
			lpObj->m_AttackDamageMaxLeft += lpObj->m_AttackDamageMaxLeft * XMasEvent.BuffAttIncrease / 100;
			lpObj->m_MagicDamageMin += lpObj->m_MagicDamageMin * XMasEvent.BuffAttIncrease / 100;
			lpObj->m_MagicDamageMax += lpObj->m_MagicDamageMax * XMasEvent.BuffAttIncrease / 100;

			lpObj->m_Defense += lpObj->m_Defense * XMasEvent.BuffDefIncrease / 100;
		}break;
#endif
	}
}


void GObjSocketItemAttrib(LPOBJ pObj, BYTE Socket, BYTE ItemPos)
{
	int SocketLess = Socket;

	if(Socket != 0xFF)
		 SocketLess = Socket % 0x32;

	switch(SocketLess)
	{
		//Fire
		case 0x00:		//Increase skill attack power
		{
			pObj->SetOpAddSkillAttack += ReadConfig.SocketOptValue[Socket];
		}break;

		case 0x01:		//Increase attack speed
		{
			pObj->m_AttackSpeed += ReadConfig.SocketOptValue[Socket];
		}break;

		case 0x02:		//Increase maximum damage/skill power
		{
			pObj->m_MagicDamageMax += ReadConfig.SocketOptValue[Socket];
		}break;

		case 0x03:		//Increase minimum damage/skill power
		{
			pObj->m_MagicDamageMin += ReadConfig.SocketOptValue[Socket];
		}break;

		case 0x04:		//Increase damage/skill power
		{
			pObj->m_MagicDamageMin += ReadConfig.SocketOptValue[Socket];
			pObj->m_MagicDamageMax += ReadConfig.SocketOptValue[Socket];
		}break;

		case 0x05:		//Decrease AG use
		{
			pObj->SetOpDecreaseAG += ReadConfig.SocketOptValue[Socket];
		}break;
		
		//Water
		case 0x0A:		//Increase defense success rate 10%
		{
			pObj->m_Defense += pObj->m_Defense * ReadConfig.SocketOptValue[Socket] / 100.0f; 
		}break;

		case 0x0B:		//Increase defense by 30
		{
			pObj->m_Defense += ReadConfig.SocketOptValue[Socket];
		}break;

		case 0x0C:		//Increase defense shield by 7%
		{
			//pObj->m_Defense += pObj->m_Defense * ReadConfig.SocketOptValue[Socket] / 100.0f;
			pObj->iAddShield += (pObj->iMaxShield + pObj->iAddShield) * ReadConfig.SocketOptValue[Socket] / 100.0f;
		}break;

		case 0x0D:		//Damage reduction 4%
		{
			pObj->DamageMinus += ReadConfig.SocketOptValue[Socket];
		}break;

		case 0x0E:		//Damage reflections 5%
		{
			pObj->DamageReflect += ReadConfig.SocketOptValue[Socket];
		}break;
		
		//Ice
		case 0x10:		//Increases acquisition rate of Life after hunting monsters
		{
			pObj->MonsterDieGetLife += ReadConfig.SocketOptValue[Socket];
		}break;

		case 0x11:		//Increases acquisition rate of Mana after hunting monsters
		{
			pObj->MonsterDieGetMana += ReadConfig.SocketOptValue[Socket];
		}break;

		case 0x12:		//Increase skill attack power
		{
			pObj->SetOpAddSkillAttack += ReadConfig.SocketOptValue[Socket];
		}break;

		case 0x13:		//Increase attack success rate
		{
			pObj->SetOpImproveSuccessAttackRate += ReadConfig.SocketOptValue[Socket];
		}break;

		case 0x14:		//Increase Durabilty in 30% [Fix needed]
		{
			pObj->pInventory[ItemPos].m_BaseDurability = (float)pObj->pInventory[ItemPos].m_BaseDurability * (ReadConfig.SocketOptValue[Socket]/100.0f);
			//pObj->SetOpAddDefenceRate += 30;
		}break;

		//Wind
		case 0x15:		//Increase Life autorecovery
		{
			pObj->FillLifeCount += ReadConfig.SocketOptValue[Socket];
			pObj->m_LifeFillCount += ReadConfig.SocketOptValue[Socket];
		}break;

		case 0x16:		//Increase maximum life 4%
		{
			pObj->AddLife += ((pObj->MaxLife+pObj->AddLife) * (ReadConfig.SocketOptValue[Socket]/100.0f));
			//GCReFillSend(pObj->m_Index, pObj->MaxLife, 0xFE, TRUE, (pObj->iMaxShield + pObj->iAddShield));
		}break;

		case 0x17:		//Increase maximum mana 4%
		{
			pObj->AddMana += ((pObj->MaxMana+pObj->AddMana) * (ReadConfig.SocketOptValue[Socket]/100.0f));
			//GCManaSend(pObj->m_Index,pObj->MaxMana,0xFE,0,pObj->MaxBP);
		}break;

		case 0x18:		//Increase mana autorecovery
		{
			pObj->m_ManaFillCount += ReadConfig.SocketOptValue[Socket];
		}break;

		case 0x19:		//Increase maximum AG
		{
			pObj->AddBP += ReadConfig.SocketOptValue[Socket];
			//GCManaSend(pObj->m_Index,pObj->MaxMana,0xFE,0,pObj->MaxBP);
		}break;

		case 0x1A:		//Increase AG amount
		{
			pObj->SetOpIncAGValue += ReadConfig.SocketOptValue[Socket];			
			//GCManaSend(pObj->m_Index,pObj->MaxMana,0xFE,0,pObj->MaxBP);
		}break;

		//Lightning
		case 0x1D:		//Increase Excellent Damage +15
		{
			pObj->SetOpAddExDamage +=ReadConfig.SocketOptValue[Socket];
		}break;

		case 0x1E:		//Increase Excellent Damage success rate 10%
		{
			pObj->m_ExcelentDamage += ReadConfig.SocketOptValue[Socket];
		}break;

		case 0x1F:		//Increase critical damage +30
		{
			pObj->SetOpAddCriticalDamage += ReadConfig.SocketOptValue[Socket];
		}break;

		case 0x20:		//Increase critical damage success rate 8%
		{
			pObj->m_CriticalDamage += ReadConfig.SocketOptValue[Socket];
		}break;

		//Earth
		case 0x24:		//Increase stamina +30
		{
			pObj->AddVitality += ReadConfig.SocketOptValue[Socket];
		}break;

		case 0xFF:		//Increase Damage by +11 [Bonus Option]
		{
			pObj->SetOpAddDamage += ReadConfig.SocketOptValue[Socket];
		}break;
	}
}

void gObjSocketItemApply(LPOBJ lpObj)
{
	BOOL bExtraDmg = FALSE;
	BYTE Slot1 = 0xFF;
	BYTE Slot2 = 0xFF;
	BYTE Slot3 = 0xFF;
	BYTE Slot4 = 0xFF;
	BYTE Slot5 = 0xFF;
	for ( int i=0;i<7;i++ )
	{		
		Slot1 = 0xFF;
		Slot2 = 0xFF;
		Slot3 = 0xFF;
		Slot4 = 0xFF;
		Slot5 = 0xFF;
		bExtraDmg = FALSE;
		if (lpObj->pInventory[i].IsItem())
		{
			if ( IsSlotItem(lpObj->pInventory[i].m_Type))
			{
				Slot1 = lpObj->pInventory[i].m_ItemSlot1 - 1;
				Slot2 = lpObj->pInventory[i].m_ItemSlot2 - 1;
				Slot3 = lpObj->pInventory[i].m_ItemSlot3 - 1;
				Slot4 = lpObj->pInventory[i].m_ItemSlot4 - 1;
				Slot5 = lpObj->pInventory[i].m_ItemSlot5 - 1;

				if (Slot1 != 0xFF)
				{
					bExtraDmg = TRUE;
					if(Slot1 != 0xFE)
					{
						//int LastSocket = Slot1 % 0x32;
						GObjSocketItemAttrib(lpObj,Slot1,i);
					}
				}
				if (Slot2 != 0xFF)
				{
					bExtraDmg = TRUE;
					if(Slot2 != 0xFE)
					{
						//int LastSocket = Slot2 % 0x32;
						GObjSocketItemAttrib(lpObj,Slot2,i);
					}
				}
				if (Slot3 != 0xFF)
				{
					bExtraDmg = TRUE;
					if(Slot3 != 0xFE)
					{
						//int LastSocket = Slot3 % 0x32;
						GObjSocketItemAttrib(lpObj,Slot3,i);
					}
				}
				if (Slot4 != 0xFF)
				{
					bExtraDmg = TRUE;
					if(Slot4 != 0xFE)
					{
						//int LastSocket = Slot4 % 0x32;
						GObjSocketItemAttrib(lpObj,Slot4,i);
					}
				}
				if (Slot5 != 0xFF)
				{
					bExtraDmg = TRUE;
					if(Slot5 != 0xFE)
					{
						//int LastSocket = Slot5 % 0x32;
						GObjSocketItemAttrib(lpObj,Slot5,i);
					}
				}

				if(bExtraDmg)
				{
					GObjSocketItemAttrib(lpObj,0xFF,i);
				}
			}
		}
	}
}




void GObjExtItemApply(LPOBJ lpObj)
{
	int n;
	int i;
	int max_count = 0;
	int temp_n[3] = {0, 1, 9};
	int temp_n2[10] = {2, 3, 4, 5, 6, 7, 8, 10, 11, 1};
	int maxn2_loop = 9;

	for ( i=0;i<3;i++ )
	{
		n = temp_n[i];

		if ( lpObj->pInventory[n].m_Type >= ITEMGET(6, 0) && lpObj->pInventory[n].m_Type < ITEMGET(7, 0) )
			continue;

		if ( lpObj->pInventory[n].IsItem() == TRUE )
		{
			if ( lpObj->pInventory[n].m_Durability != 0.0f )
			{
				if ( lpObj->pInventory[n].m_IsValidItem == true )
				{
					if ( lpObj->pInventory[n].IsExtAttackRate() )
					{
						if ( ( lpObj->pInventory[n].m_Type >= ITEMGET(5, 0) && lpObj->pInventory[n].m_Type < ITEMGET(6, 0) ) ||
							lpObj->pInventory[n].m_Type == ITEMGET(13,12) ||
							lpObj->pInventory[n].m_Type == ITEMGET(13,25) ||
							lpObj->pInventory[n].m_Type == ITEMGET(13,27) )
						{
							lpObj->m_MagicDamageMax += lpObj->Level / 20;
							lpObj->m_MagicDamageMin += lpObj->Level / 20;
						}
						else 
						{
							if ( n == 0 || n == 9 )
							{
								lpObj->m_AttackDamageMinRight += lpObj->Level / 20;
								lpObj->m_AttackDamageMaxRight += lpObj->Level / 20;
							}
							
							if ( n == 1 || n == 9 )
							{
								lpObj->m_AttackDamageMinLeft += lpObj->Level / 20;
								lpObj->m_AttackDamageMaxLeft += lpObj->Level / 20;
							}
						}
					}

					if ( lpObj->pInventory[n].IsExtAttackRate2() )
					{
						if ( ( lpObj->pInventory[n].m_Type >= ITEMGET(5, 0) && lpObj->pInventory[n].m_Type < ITEMGET(6, 0) ) ||
							lpObj->pInventory[n].m_Type == ITEMGET(13,12) ||
							lpObj->pInventory[n].m_Type == ITEMGET(13,25) ||
							lpObj->pInventory[n].m_Type == ITEMGET(13,27) )
						{
							lpObj->m_MagicDamageMax += lpObj->m_MagicDamageMax * lpObj->pInventory[n].IsExtAttackRate2() / 100;
							lpObj->m_MagicDamageMin += lpObj->m_MagicDamageMin * lpObj->pInventory[n].IsExtAttackRate2() / 100;
						}
						else 
						{
							if ( n == 0 || n == 9 )
							{
								lpObj->m_AttackDamageMinRight += lpObj->m_AttackDamageMinRight * lpObj->pInventory[n].IsExtAttackRate2() / 100;
								lpObj->m_AttackDamageMaxRight += lpObj->m_AttackDamageMaxRight * lpObj->pInventory[n].IsExtAttackRate2() / 100;
							}
							
							if ( n == 1 || n == 9 )
							{
								lpObj->m_AttackDamageMinLeft += lpObj->m_AttackDamageMinLeft * lpObj->pInventory[n].IsExtAttackRate2() / 100;
								lpObj->m_AttackDamageMaxLeft += lpObj->m_AttackDamageMaxLeft * lpObj->pInventory[n].IsExtAttackRate2() / 100;
							}
						}
					}

					lpObj->m_ExcelentDamage += lpObj->pInventory[n].IsExtExcellentDamage();
					lpObj->m_AttackSpeed += lpObj->pInventory[n].IsExtAttackSpeed();
					lpObj->m_MagicSpeed += lpObj->pInventory[n].IsExtAttackSpeed();
					lpObj->MonsterDieGetLife += lpObj->pInventory[n].IsExtMonsterDieLife();
					lpObj->MonsterDieGetMana += lpObj->pInventory[n].IsExtMonsterDieMana();
				}
			}
		}
	}
		
	if ( lpObj->pInventory[1].IsItem() == TRUE )
	{
		if ( lpObj->pInventory[1].m_Type >= ITEMGET(6, 0) && lpObj->pInventory[1].m_Type <= ITEMGET(7, 0) )
		{
			maxn2_loop = 10;
		}
	}

	for ( i=0;i<maxn2_loop;i++)
	{
		n = temp_n2[i];

		if ( max_count < 8 )
		{
			if ( lpObj->pInventory[n].IsItem() == TRUE &&
				 lpObj->pInventory[n].IsExtItem() &&
				 lpObj->pInventory[n].m_Durability != 0.0f &&
				 lpObj->pInventory[n].m_IsValidItem == true )
			{
				//WINGS FIX
				if ( n != 7 )	// #warning this will unselect ONLY wings
				{
					lpObj->AddLife += (short)(lpObj->MaxLife * lpObj->pInventory[n].IsExtLifeAdd()  / 100.0f);	// #stat
					lpObj->AddMana += (short)(lpObj->MaxMana * lpObj->pInventory[n].IsExtManaAdd() / 100.0f);	// #stat
					lpObj->m_SuccessfulBlocking += lpObj->m_SuccessfulBlocking * lpObj->pInventory[n].IsExtDefenseSuccessfull() / 100;
					lpObj->MonsterDieGetMoney += lpObj->pInventory[n].IsExtMonsterMoney();
					lpObj->DamageMinus += lpObj->pInventory[n].IsExtDamageMinus();
					lpObj->DamageReflect += lpObj->pInventory[n].IsExtDamageReflect();
					//if ( n != 7 )	// #warning this will unselect ONLY wings
					//{
					//	max_count++;
					//}
				}
			}
		}
	}
}



void gObjCalcSetItemStat(LPOBJ lpObj)
{
	for ( int i=0;i<INVETORY_WEAR_SIZE;i++)
	{
		if ( lpObj->pInventory[i].IsSetItem() && lpObj->pInventory[i].m_IsValidItem && lpObj->pInventory[i].m_Durability != 0.0f )
		{
			int AddStatType = lpObj->pInventory[i].GetAddStatType();

			switch ( AddStatType )
			{
				case 1:
					lpObj->pInventory[i].SetItemPlusSpecialStat(&lpObj->AddStrength, 196);
					break;
				case 2:
					lpObj->pInventory[i].SetItemPlusSpecialStat(&lpObj->AddDexterity, 197);
					break;
				case 3:
					lpObj->pInventory[i].SetItemPlusSpecialStat(&lpObj->AddEnergy, 198);
					break;
				case 4:
					lpObj->pInventory[i].SetItemPlusSpecialStat(&lpObj->AddVitality, 199);
					break;
			}
		}
	}
}





void gObjGetSetItemOption(LPOBJ lpObj, LPBYTE pSetOptionTable , LPBYTE pSetOptionCountTable , int * pSetOptionCount)
{
	*pSetOptionCount = 0;
	int RightSetOptionIndex = -1;
	int RightRingSetOptionIndex = -1;

	for ( int i=0;i<INVETORY_WEAR_SIZE;i++)
	{
		int iSetItemType = lpObj->pInventory[i].IsSetItem();

		if ( iSetItemType )
		{
			if ( lpObj->pInventory[i].m_IsValidItem )
			{
				int iSetItemOption = gSetItemOption.GetOptionTableIndex(lpObj->pInventory[i].m_Type, iSetItemType );

				if ( iSetItemOption != -1 )
				{
					if ( i == 0 )
					{
						RightSetOptionIndex = iSetItemOption;
					}
					else if ( i == 1  )
					{
						if (  RightSetOptionIndex == iSetItemOption )
						{
							if ( lpObj->pInventory[0].GetWeaponType() == lpObj->pInventory[1].GetWeaponType() && lpObj->pInventory[0].IsSetItem() )
							{
								//LogAddTD("[%s][%s] Same Ancient weapon equipped, skipping TYPE: %d...",
								//	lpObj->AccountID, lpObj->Name,
								//	lpObj->pInventory[0].GetWeaponType()
								//);
								continue;
							}
						}
					}
					
					if ( i == 10 )
					{
						RightRingSetOptionIndex = iSetItemOption;
					}
					else if ( i == 11   )
					{
						if ( lpObj->pInventory[10].m_Type == lpObj->pInventory[11].m_Type && lpObj->pInventory[10].IsSetItem() )
						{
							//LogAddTD("[%s][%s] Same Ancient ring equipped, skipping it TYPE: %d...",
							//	lpObj->AccountID, lpObj->Name,
							//	lpObj->pInventory[11].m_Type
							//);
							continue;
						}
					}

					BOOL bFound = FALSE;

					for ( int n=0;n<*pSetOptionCount;n++)
					{
						if ( pSetOptionTable[n] == iSetItemOption )
						{
							pSetOptionCountTable[n]++;
							bFound = TRUE;

							//LogAddTD("[%s][%s] Counting Ancient Options Set:%s, Option Count:%d",
							//	lpObj->AccountID, lpObj->Name,
							//	gSetItemOption.GetSetOptionName(iSetItemOption), pSetOptionCountTable[n]
							//);

							break;
						}
					}

					if ( bFound == FALSE )
					{
						pSetOptionTable[*pSetOptionCount] = iSetItemOption;
						pSetOptionCountTable[*pSetOptionCount]++;

						//LogAddTD("[%s][%s] Options sought %s, Count = %d",
						//	lpObj->AccountID, lpObj->Name,
						//	gSetItemOption.GetSetOptionName(iSetItemOption), pSetOptionCountTable[*pSetOptionCount]
						//);

						*pSetOptionCount += 1;
					}
				}
			}
		}
	}
}
void gObjCalcSetItemOption(LPOBJ lpObj)
{
	BYTE SetOptionTable[29];
	BYTE SetOptionCountTable[29];
	int SetOptionCount = 0;
	int op1;
	int op2;
	int op3;
	int op4;
	int op5;
	int opvalue1;
	int opvalue2;
	int opvalue3;
	int opvalue4;
	int opvalue5;

	memset(SetOptionTable, 0, sizeof(SetOptionTable));
	memset(SetOptionCountTable, 0, sizeof(SetOptionCountTable));
	gObjGetSetItemOption(lpObj, SetOptionTable, SetOptionCountTable, &SetOptionCount);

	for ( int optioncount=0;optioncount<SetOptionCount;optioncount++)
	{
		int OptionTableIndex = SetOptionTable[optioncount];
		int OptionTableCount = SetOptionCountTable[optioncount];

		if ( OptionTableCount >= 2 )
		{
			gSetItemOption.IsRequireClass(OptionTableIndex, lpObj->Class, lpObj->ChangeUP);

			for ( int tablecnt=0;tablecnt<(OptionTableCount-1);tablecnt++)
			{
				if ( gSetItemOption.GetSetOption(OptionTableIndex, tablecnt,
					op1, op2, opvalue1, opvalue2, lpObj->Class, lpObj->ChangeUP) )
				{
					gObjSetItemStatPlusSpecial(lpObj, op1, opvalue1);
					gObjSetItemStatPlusSpecial(lpObj, op2, opvalue2);
				}
			}

			if ( gSetItemOption.GetMaxSetOptionCount(OptionTableIndex) < OptionTableCount )
			{
				lpObj->IsFullSetItem = true;
				//LogAddC(2, "[%s][%s] Apply full set options %s",
				//	lpObj->AccountID, lpObj->Name,
				//	gSetItemOption.GetSetOptionName(OptionTableIndex)
				//);

				gSetItemOption.GetGetFullSetOption(OptionTableIndex,
					op1, op2, op3, op4, op5,
					opvalue1, opvalue2, opvalue3, opvalue4, opvalue5,
					lpObj->Class, lpObj->ChangeUP);

				gObjSetItemStatPlusSpecial(lpObj, op1, opvalue1);
				gObjSetItemStatPlusSpecial(lpObj, op2, opvalue2);
				gObjSetItemStatPlusSpecial(lpObj, op3, opvalue3);
				gObjSetItemStatPlusSpecial(lpObj, op4, opvalue4);
				gObjSetItemStatPlusSpecial(lpObj, op5, opvalue5);
			}		
		}
	}

	for ( int optioncount=0;optioncount<SetOptionCount;optioncount++)
	{
		int OptionTableIndex = SetOptionTable[optioncount];
		int OptionTableCount = SetOptionCountTable[optioncount];

		if ( OptionTableCount >= 2 )
		{
			//LogAdd("[%s][%s] Apply a set of options %s",
			//	lpObj->AccountID, lpObj->Name, 
			//	gSetItemOption.GetSetOptionName(OptionTableIndex)
			//);

			for ( int tablecnt=0;tablecnt<(OptionTableCount-1);tablecnt++)
			{
				if ( gSetItemOption.GetSetOption(OptionTableIndex, tablecnt,
					op1, op2, opvalue1, opvalue2, lpObj->Class, lpObj->ChangeUP) )
				{
					gObjSetItemPlusSpecial(lpObj, op1, opvalue1);
					gObjSetItemPlusSpecial(lpObj, op2, opvalue2);
				}
			}
				
			if ( gSetItemOption.GetMaxSetOptionCount(OptionTableIndex) < OptionTableCount )
			{
				lpObj->IsFullSetItem = true;
				//LogAddC(2, "[%s][%s] Apply full set options %s",
				//	lpObj->AccountID, lpObj->Name,
				//	gSetItemOption.GetSetOptionName(OptionTableIndex)
				//);

				gSetItemOption.GetGetFullSetOption(OptionTableIndex,
					op1, op2, op3, op4, op5,
					opvalue1, opvalue2, opvalue3, opvalue4, opvalue5,
					lpObj->Class, lpObj->ChangeUP);

				gObjSetItemPlusSpecial(lpObj, op1, opvalue1);
				gObjSetItemPlusSpecial(lpObj, op2, opvalue2);
				gObjSetItemPlusSpecial(lpObj, op3, opvalue3);
				gObjSetItemPlusSpecial(lpObj, op4, opvalue4);
				gObjSetItemPlusSpecial(lpObj, op5, opvalue5);
			}		
		}
	}
}




void gObjSetItemStatPlusSpecial(LPOBJ lpObj, int option, int ivalue)
{
	if ( option == -1 )
		return;

	switch ( option )	// #warning Add AT_SET_OPTION_IMPROVE_LEADERSHIP
	{
		case AT_SET_OPTION_IMPROVE_STRENGTH:
			lpObj->AddStrength += ivalue;
			break;
		case AT_SET_OPTION_IMPROVE_DEXTERITY:
			lpObj->AddDexterity += ivalue;
			break;
		case AT_SET_OPTION_IMPROVE_ENERGY:
			lpObj->AddEnergy += ivalue;
			break;
		case AT_SET_OPTION_IMPROVE_VITALITY:
			lpObj->AddVitality += ivalue;
			break;
	}
}



void gObjSetItemPlusSpecial(LPOBJ lpObj, int option, int ivalue)
{
	if ( option == -1 )
		return;

	switch ( option )
	{
		case AT_SET_OPTION_DOUBLE_DAMAGE:
			lpObj->SetOpDoubleDamage += ivalue;
			break;
		case AT_SET_OPTION_IMPROVE_SHIELD_DEFENCE:
			lpObj->SetOpImproveSheldDefence += ivalue;
			break;
		case AT_SET_OPTION_TWO_HAND_SWORD_IMPROVE_DAMAGE:
			lpObj->SetOpTwoHandSwordImproveDamage += ivalue;
			break;
		case AT_SET_OPTION_IMPROVE_MINATTACKDAMAGE:
			lpObj->SetOpAddMinAttackDamage += ivalue;
			break;
		case AT_SET_OPTION_IMPROVE_MAXATTACKDAMAGE:
			lpObj->SetOpAddMaxAttackDamage += ivalue;
			break;
		case AT_SET_OPTION_IMPROVE_MAGICDAMAGE:
			lpObj->SetOpAddMagicPower += ivalue;
			break;
		case AT_SET_OPTION_IMPROVE_DAMAGE:
			lpObj->SetOpAddDamage += ivalue;
			break;
		case AT_SET_OPTION_IMPROVE_ATTACKRATE:
			lpObj->SetOpImproveSuccessAttackRate += ivalue;
			break;
		case AT_SET_OPTION_IMPROVE_DEFENCE:
			lpObj->SetOpAddDefence += ivalue;
			break;
		case AT_SET_OPTION_DEFENCE_IGNORE:
			lpObj->SetOpIgnoreDefense += ivalue;
			break;
		case AT_SET_OPTION_IMPROVE_MAXLIFE:
			lpObj->AddLife += ivalue;
			break;
		case AT_SET_OPTION_IMPROVE_MAXMANA:
			lpObj->AddMana += ivalue;
			break;
		case AT_SET_OPTION_IMPROVE_MAXAG:
			lpObj->AddBP += ivalue;
			break;
		case AT_SET_OPTION_IMPROVE_AG_VALUE:
			lpObj->SetOpIncAGValue += ivalue;
			break;
		case AT_SET_OPTION_IMPROVE_CRITICALDAMAGE:
			lpObj->SetOpAddCriticalDamage += ivalue;
			break;
		case AT_SET_OPTION_IMPROVE_CRITICALDAMAGE_SUCCESS:
			lpObj->SetOpAddCriticalDamageSuccessRate += ivalue;
			break;
		case AT_SET_OPTION_IMPROVE_EX_DAMAGE:
			lpObj->SetOpAddExDamage += ivalue;
			break;
		case AT_SET_OPTION_IMPROVE_EX_DAMAGE_SUCCESS:
			lpObj->SetOpAddExDamageSuccessRate += ivalue;
			break;
		case AT_SET_OPTION_IMPROVE_SKILLDAMAGE:
			lpObj->SetOpAddSkillAttack += ivalue;
			break;
		case AT_SET_OPTION_IMPROVE_ATTACKDAMAGE_WITH_DEX:
			{
				if(ivalue > 0)
					lpObj->SetOpAddAttackDamage += ( lpObj->Dexterity + lpObj->AddDexterity ) / ivalue;
			}break;
		case AT_SET_OPTION_IMPROVE_ATTACKDAMAGE_WITH_STR:
			{
				if(ivalue > 0)
					lpObj->SetOpAddAttackDamage += ( lpObj->Strength + lpObj->AddStrength ) / ivalue;
			}break;
		case AT_SET_OPTION_IMPROVE_MAGICDAMAGE_WITH_ENERGY:
			{
				if(ivalue > 0)
					lpObj->SetOpAddMagicPower += ( lpObj->Energy + lpObj->AddEnergy ) / ivalue;
			}break;
		case AT_SET_OPTION_IMPROVE_DEFENCE_WITH_DEX:
			{
				if(ivalue > 0)
					lpObj->SetOpAddDefence += lpObj->Dexterity + ( lpObj->AddDexterity  / ivalue );
			}break;
		case AT_SET_OPTION_IMPROVE_DEFENCE_WITH_VIT:
			{
				if(ivalue > 0)
					lpObj->SetOpAddDefence += ( lpObj->Vitality + lpObj->AddVitality ) / ivalue;
			}break;
		case AT_SET_OPTION_FIRE_MASTERY:
			lpObj->m_AddResistance[R_FIRE] += ivalue;
			break;
		case AT_SET_OPTION_ICE_MASTERY:
			lpObj->m_AddResistance[R_ICE] += ivalue;
			break;
		case AT_SET_OPTION_THUNDER_MASTERY:
			lpObj->m_AddResistance[R_LIGHTNING] += ivalue;
			break;
		case AT_SET_OPTION_POSION_MASTERY:
			lpObj->m_AddResistance[R_POISON] += ivalue;
			break;
		case AT_SET_OPTION_WATER_MASTERY:
			lpObj->m_AddResistance[R_WATER] += ivalue;
			break;
		case AT_SET_OPTION_WIND_MASTERY:
			lpObj->m_AddResistance[R_WIND] += ivalue;
			break;
		case AT_SET_OPTION_EARTH_MASTERY:
			lpObj->m_AddResistance[R_EARTH] += ivalue;
			break;
	}
}




void gObjSetItemApply(LPOBJ lpObj)
{
	lpObj->m_AttackDamageMinLeft += lpObj->SetOpAddAttackDamage;
	lpObj->m_AttackDamageMaxLeft += lpObj->SetOpAddAttackDamage;
	lpObj->m_AttackDamageMinRight += lpObj->SetOpAddAttackDamage;
	lpObj->m_AttackDamageMaxRight += lpObj->SetOpAddAttackDamage;

	lpObj->m_AttackDamageMinLeft += lpObj->SetOpAddMinAttackDamage;
	lpObj->m_AttackDamageMinRight += lpObj->SetOpAddMinAttackDamage;

	lpObj->m_AttackDamageMaxLeft += lpObj->SetOpAddMaxAttackDamage;
	lpObj->m_AttackDamageMaxRight += lpObj->SetOpAddMaxAttackDamage;

	lpObj->m_MagicDamageMin += lpObj->m_MagicDamageMin * lpObj->SetOpAddMagicPower / 100;
	lpObj->m_MagicDamageMax += lpObj->m_MagicDamageMax * lpObj->SetOpAddMagicPower / 100;

	lpObj->AddLife += INT(lpObj->AddVitality * DCInfo.DefClass[lpObj->Class].VitalityToLife);
	lpObj->AddMana += INT(lpObj->AddEnergy * DCInfo.DefClass[lpObj->Class].EnergyToMana);

	lpObj->m_CriticalDamage += lpObj->SetOpAddCriticalDamageSuccessRate;
	lpObj->m_ExcelentDamage += lpObj->SetOpAddExDamageSuccessRate;

	if ( lpObj->pInventory[10].IsSetItem() )
	{
		lpObj->pInventory[10].PlusSpecialSetRing((LPBYTE)lpObj->m_AddResistance);
	}

	if ( lpObj->pInventory[11].IsSetItem() )
	{
		lpObj->pInventory[11].PlusSpecialSetRing((LPBYTE)lpObj->m_AddResistance);
	}

	if ( lpObj->pInventory[9].IsSetItem() )
	{
		lpObj->pInventory[9].PlusSpecialSetRing((LPBYTE)lpObj->m_AddResistance);
	}

	if ( lpObj->SetOpTwoHandSwordImproveDamage )
	{
		if ( !lpObj->pInventory[1].IsItem() && !lpObj->pInventory[0].IsItem() )
		{
			lpObj->SetOpTwoHandSwordImproveDamage = 0;
		}

		if ( lpObj->pInventory[1].IsItem() && lpObj->pInventory[1].m_TwoHand == FALSE )
		{
			lpObj->SetOpTwoHandSwordImproveDamage = 0;
		}

		if ( lpObj->pInventory[0].IsItem() && lpObj->pInventory[0].m_TwoHand == FALSE )
		{
			lpObj->SetOpTwoHandSwordImproveDamage = 0;
		}
	}

	//if ( (lpObj->MaxLife + lpObj->AddLife ) < lpObj->Life )
	//{
	//	lpObj->Life = lpObj->MaxLife + lpObj->AddLife;
	//	GCReFillSend(lpObj->m_Index, lpObj->Life, 0xFF, 0, lpObj->iShield);
	//}

	//gObjSetBP(lpObj->m_Index);

	//if ( (lpObj->MaxMana + lpObj->AddMana ) < lpObj->Mana )
	//{
	//	lpObj->Mana = lpObj->MaxMana + lpObj->AddMana;
	//	GCManaSend(lpObj->m_Index, lpObj->Mana, 0xFF, 0, lpObj->BP);
	//}

	lpObj->m_Defense += lpObj->SetOpAddDefence * 10 / 20;
	lpObj->m_Defense += lpObj->m_Defense * lpObj->SetOpAddDefenceRate / 100;

	if ( lpObj->pInventory[1].m_Type >= ITEMGET(6,0) && lpObj->pInventory[1].m_Type < ITEMGET(7,0) )
	{
		lpObj->m_Defense += lpObj->m_Defense * lpObj->SetOpImproveSheldDefence / 100;
	}
}





BOOL gObjValidItem(LPOBJ lpObj, CItem * lpItem, int pos)
{
	LPITEM_ATTRIBUTE p = &ItemAttribute[lpItem->m_Type];

	if ((lpItem->GetNumber() == 0xFFFFFFFF && ReadConfig.ItemTradeFFFFBlock == TRUE) ||
		(lpItem->GetNumber() == 0xFFFFFFFE && ReadConfig.ItemTradeFFFEBlock == TRUE) ||
		(lpItem->GetNumber() == 0xFFFFFFFD && ReadConfig.ItemTradeFFFDBlock == TRUE) ||
		(lpItem->GetNumber() == 0xFFFFFFFC && ReadConfig.ItemTradeFFFCBlock == TRUE) )
	{
		return TRUE;
	}
	
	if ( p->RequireStrength != 0 )
	{
		if ( (lpObj->Strength + lpObj->AddStrength) < ( lpItem->m_RequireStrength - lpItem->m_HJOpStrength ) )
		{
			return FALSE;
		}
	}

	if ( p->RequireDexterity != 0 )
	{
		if ( (lpObj->Dexterity + lpObj->AddDexterity) < ( lpItem->m_RequireDexterity - lpItem->m_HJOpDexterity ) )
		{
			return FALSE;
		}
	}


	if ( p->RequireEnergy != 0 )
	{
		if ( (lpObj->Energy + lpObj->AddEnergy) < lpItem->m_RequireEnergy )
		{
			return FALSE;
		}
	}

	if ( p->RequireLevel != 0 )
	{
		if ( (lpObj->Level) < lpItem->m_RequireLevel )
		{
			return FALSE;
		}
	}

	if ( lpItem->m_Type >= ITEMGET(0,0) && lpItem->m_Type <= ITEMGET(11, 0) )
	{
		if ( p->RequireVitality )
		{
			if ( (lpObj->Vitality + lpObj->AddVitality ) < lpItem->m_RequireVitality )
			{
				return FALSE;
			}
		}

		if ( p->RequireLeadership )
		{
			if ( (lpObj->Leadership + lpObj->AddLeadership ) < lpItem->m_RequireLeaderShip )
			{
				return FALSE;
			}
		}
	}

	return TRUE;
}
	


void gObjCalcShieldPoint(LPOBJ lpObj)
{
	int iMaxShieldPoint = 0;
	int iExpressionA = ( lpObj->Strength + lpObj->AddStrength ) + ( lpObj->Dexterity + lpObj->AddDexterity ) + ( lpObj->Vitality + lpObj->AddVitality ) + (lpObj->Energy + lpObj->AddEnergy );

	if ( lpObj->Class == CLASS_DARKLORD )
	{
		iExpressionA += lpObj->Leadership + lpObj->AddLeadership;
	}

	if ( g_iShieldGageConstB == 0 )
		g_iShieldGageConstB = 30;

	int iExpressionB = ( lpObj->Level * lpObj->Level) / g_iShieldGageConstB;

	if ( g_iShieldGageConstA == 0 )
		g_iShieldGageConstA = 12;

	iMaxShieldPoint = ( iExpressionA * g_iShieldGageConstA ) / 10 + iExpressionB  + lpObj->m_Defense;
	lpObj->iMaxShield = iMaxShieldPoint;
}