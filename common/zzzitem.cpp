// ------------------------------
// Decompiled by Deathway
// Date : 2007-03-09
// ------------------------------
// GS-N 0.99.60T 0x004E5740 - Status Completed :)
// GS-N	1.00.18	JPN	0x0051E110	-	Completed

#include "StdAfx.h"
//#include "iostream.h"
//#include "iomanip.h"
#include "zzzitem.h"
#include "SetItemOption.h"
#include "..\SOURCE\logproc.h"
#include "WzMemScript.h"
#include "..\include\Readscript.h"
#include "..\SOURCE\JewelOfHarmonySystem.h"
#include "..\SOURCE\ItemSystemFor380.h"
#include "..\SOURCE\user.h"
#include "..\SOURCE\LogToFile.h"
#include "..\SOURCE\StdAfx.h"
#include "..\SOURCE\DarkSpirit.h"
#include "..\SOURCE\MuItemShop.h"
#include "..\SOURCE\CustomWings.h"



ITEM_ATTRIBUTE ItemAttribute[MAX_ITEMS];
CPetItemExp gPetItemExp;
int g_MaxItemIndexOfEachItemType[MAX_TYPE_ITEMS];

CItem::CItem()
{
	this->Clear();
}

void CItem::Clear()	// Fine
{
	__try
	{
	this->m_Type =-1;
	this->m_Level =0;
	this->m_Part=0;
	this->m_Class=0;
	this->m_TwoHand=0;
	this->m_AttackSpeed=0;
	this->m_DamageMin=0;
	this->m_DamageMax=0;
	this->m_SuccessfulBlocking=0;
	this->m_Defense=0;
	this->m_MagicDefense =0;
	this->m_Durability=0;
	this->m_SpecialNum=0;
	this->m_Value=0;
	this->m_SkillOption =0;
	this->m_LuckOption =0;
	this->m_Z28Option =0;
	this->m_NewOption =0;
	this->m_Number =0;
	this->m_DurabilitySmall =0;
	this->m_iPShopValue=-1;
	this->m_bItemExist=true;
	this->m_CurrentDurabilityState=-1;
	this->m_SetOption=0;
	this->m_QuestItem=false;


	memset(this->m_Special, 0, sizeof(this->m_Special));
	memset(this->m_SpecialValue, 0, sizeof(this->m_SpecialValue));
	memset(this->m_Resistance, 0, sizeof(this->m_Resistance));

	this->m_IsLoadPetItemInfo=0;
	this->m_PetItem_Level =1;
	this->m_PetItem_Exp =0;
	this->m_Leadership=0;
	this->m_JewelOfHarmonyOption = 0;
	this->m_ItemOptionEx = 0;
	}__except( EXCEPTION_ACCESS_VIOLATION == GetExceptionCode() )
	{
		LogAddC(2, "error-L1 : Can not clear item!!!");
	}
}

BOOL CItem::IsItem() // Good
{
	__try
	{
		if (this->m_Type < 0)
		{
			return 0;
		}
		else
		{
			if (this->m_Type > MAX_ITEMS)
			{
				return 0;
			}
			else
			{
				return 1;
			}
		}
	}__except( EXCEPTION_ACCESS_VIOLATION == GetExceptionCode() )
	{
		return 0;
	}
}

int CItem::IsSetItem() // Good
{
	if (this->m_Type < 0)
	{
		return 0;
	}
	else
	{
		return this->m_SetOption & 3;	// Limit of the SetItem
	}
}	

void CItem::Convert(int type, BYTE Option1, BYTE Option2, BYTE Option3,  BYTE Attribute2, BYTE SetOption, BYTE ItemEffectEx,  BYTE DbVersion)
{
	int _type;
	int ItemLevel;
	LPITEM_ATTRIBUTE p;
	int SOption;
	int SOptionStatValue;
	int iChaosItem;
	int i;
	
	_type = type;

	if ( (DbVersion ) == 0)
	{
		_type=(type/MAX_TYPE_ITEMS*MAX_SUBTYPE_ITEMS)+(type % MAX_TYPE_ITEMS);

	}
	else if ( DbVersion <= 2 )
	{
		_type = (type/32*MAX_SUBTYPE_ITEMS) + (type % 32 );
	}
	
	if ( _type >MAX_ITEMS-1)
	{
		LogAddC(2, "error-L1 : ItemIndex error %d", _type);
	}
	
	p=&ItemAttribute[_type];
	this->m_serial=ItemAttribute[_type].Serial ;
	this->m_Type=type;
	
	if ( (DbVersion ) <= 2)
	{
		this->m_Type=_type;
	}

	this->m_NewOption =Attribute2;
	
	if (p->OptionFlag  == 0)
	{
		this->m_NewOption=0;
	}

	if ( ((_type >= ITEMGET(12,3) ) && (_type <= ITEMGET(12,6) ) )  || ((_type >= ITEMGET(12,36) ) && (_type <= ITEMGET(12,43) ) ) || _type == ITEMGET(13,30) || _type == ITEMGET(12,49) || _type == ITEMGET(12,50)  
#if (CRYSTAL_EDITION == 1)
			|| (_type >= ITEMGET(12,200) && _type <= ITEMGET(12,254))
#endif
			) //Third Wings Fix
	{
		Attribute2=0;
	}

	if ( _type == ITEMGET(13,37) ) //Fenrir
		Attribute2 = 0;

	SetOption = SetOption & 15;
		
	SOption=SetOption & 3;
	SOptionStatValue=0;

	if (SOption != 1 && SOption != 2)
	{
		SOption=0;
	}

	if ( (( (SetOption>>2) & 3) != 1) && ( ((SetOption>>2) & 3) != 2) )
	{
		SOption=0;
	}

	if ( gSetItemOption.IsSetItem(_type) != 0 && SetOption != 255 && SOption != 0)
	{
		this->m_SetOption = SetOption;
		SOptionStatValue = (SetOption>>2)&3;

		if (( this->m_SetOption != 0 ) && (ReadConfig.CItem_ConvertExcAncient == 0))
		{
			this->m_NewOption = 0;
			Attribute2 = 0;
		}
	}
	else
	{
		this->m_SetOption = 0;
	}
	
	this->m_ItemOptionEx = ItemEffectEx;
	memset( this->m_SkillResistance , 0, sizeof(this->m_SkillResistance) );
	
	if ( _type == ITEMGET(0,19) || _type == ITEMGET(4,18) || _type == ITEMGET(5,10) || _type == ITEMGET(2,13))
	{
		Attribute2=0;
	}

	iChaosItem = 0;

	if ( this->m_Type == ITEMGET(2,6) )
	{
		iChaosItem = 15;
	}
	else if ( this->m_Type == ITEMGET(5,7) )
	{
		iChaosItem = 25;
	}
	else if ( this->m_Type == ITEMGET(4,6) )
	{
		iChaosItem = 30;
	}

	if ( ItemGetDurability(this->m_Type, 0, 0, 0) != 0 )
	{
		this->m_BaseDurability = (float)ItemGetDurability(this->m_Type, this->m_Level, Attribute2 & 0x7F, this->m_SetOption );
	}
	else
	{
		this->m_BaseDurability = 0;
	}

	this->m_DurabilityState[0] = this->m_BaseDurability * 0.5f;
	this->m_DurabilityState[1] = this->m_BaseDurability * 0.3f;
	this->m_DurabilityState[2] = this->m_BaseDurability * 0.2f;
	this->m_Value = p->Value;
	this->m_AttackSpeed = p->AttackSpeed;
	this->m_TwoHand = p->TwoHand;
	this->m_DamageMin = p->DamageMin;
	this->m_DamageMax = p->DamageMax;
	this->m_SuccessfulBlocking = p->SuccessfulBlocking;
	this->m_Defense = p->Defense;
	this->m_MagicDefense = p->MagicDefense;
	this->m_WalkSpeed = p->WalkSpeed;
	this->m_Magic = p->MagicPW;
	this->m_serial = p->Serial;
	this->m_QuestItem = (bool)p->QuestItem;
	this->m_RequireLeaderShip = 0;

	if ( this->m_Durability == 0 )
	{
		this->m_CurrentDurabilityState = 1;
	}
	else if ( this->m_Durability < this->m_DurabilityState[2] )
	{
		this->m_CurrentDurabilityState = (float)0.5;
	}
	else if ( this->m_Durability < this->m_DurabilityState[1] )
	{
		this->m_CurrentDurabilityState = (float)0.3;
	}
	else if ( this->m_Durability < this->m_DurabilityState[0] )
	{
		this->m_CurrentDurabilityState = (float)0.2;
	}
	else
	{
		this->m_CurrentDurabilityState = 0.0;
	}

	memcpy(this->m_RequireClass, p->RequireClass , sizeof(this->m_RequireClass));

	for ( i = 0; i<MAX_ITEM_SPECIAL_ATTRIBUTE ; i++ )
	{
		this->m_Resistance[i] = p->Resistance[i] * (this->m_Level + 1);
	}

	ItemLevel = p->Level;

	if ( ( Attribute2 & 0x3F) > 0 )
	{
		ItemLevel = p->Level + 25;
	}
	else
	{
		if (this->m_SetOption != 0 )
		{
			ItemLevel = p->Level + 25;
		}
	}

	if ( p->RequireStrength != 0 )
	{
		this->m_RequireStrength = ((p->RequireStrength * (ItemLevel + this->m_Level * 3 ) ) * 3) / 100 + 20;
	}
	else
	{
		this->m_RequireStrength = 0;
	}


	if ( p->RequireDexterity != 0 )
	{
		this->m_RequireDexterity = ((p->RequireDexterity * (ItemLevel + this->m_Level * 3 ) ) * 3) / 100 + 20;
	}
	else
	{
		this->m_RequireDexterity  = 0;
	}

	if ( p->RequireEnergy != 0 )
	{
		this->m_RequireEnergy = ((p->RequireEnergy * (ItemLevel + this->m_Level * 3 ) ) * 3) / 100 + 20;
	}
	else
	{
		this->m_RequireEnergy = 0;
	}

	if ( _type >= ITEMGET(0,0) && _type < ITEMGET(12,0) )
	{
		if ( p->RequireVitality != 0 )
		{
			this->m_RequireVitality = ((p->RequireVitality * (ItemLevel + this->m_Level * 3 ) ) * 3) / 100 + 20;
		}
		else
		{
			this->m_RequireVitality = 0;
		}

		if ( p->RequireLeadership != 0 )
		{
			this->m_RequireLeaderShip = ((p->RequireVitality * (ItemLevel + this->m_Level * 3 ) ) * 3) / 100 + 20;
		}
		else
		{
			this->m_RequireLeaderShip = 0;
		}
	}

	if ( _type == ITEMGET(13,5) ) //Dark Spirit
		this->m_RequireLeaderShip = this->m_PetItem_Level * 15 + 185;

	if ( g_kJewelOfHarmonySystem.IsActive(this) == true )
	{
		BYTE btOption = g_kJewelOfHarmonySystem.GetItemStrengthenOption(this);

		if ( btOption == AT_JEWELOFHARMONY_WEAPON_DECREASE_REQUIRE_STR )
			this->m_HJOpStrength = g_kJewelOfHarmonySystem.GetItemEffectValue(this,
			AT_JEWELOFHARMONY_WEAPON_DECREASE_REQUIRE_STR);
		else if ( btOption == AT_JEWELOFHARMONY_WEAPON_IMPROVE_MAXATTACKDAMAGE )
			this->m_HJOpStrength = g_kJewelOfHarmonySystem.GetItemEffectValue(this,
			AT_JEWELOFHARMONY_WEAPON_IMPROVE_MAXATTACKDAMAGE);
		else if ( btOption == AT_JEWELOFHARMONY_WEAPON_DECREASE_REQUIRE_DEX )
			this->m_HJOpDexterity = g_kJewelOfHarmonySystem.GetItemEffectValue(this,
			AT_JEWELOFHARMONY_WEAPON_DECREASE_REQUIRE_DEX);
		else if ( btOption == AT_JEWELOFHARMONY_WEAPON_DECREASE_REQUIRE_STR )
			this->m_HJOpDexterity = g_kJewelOfHarmonySystem.GetItemEffectValue(this,
			AT_JEWELOFHARMONY_WEAPON_DECREASE_REQUIRE_STR);
	}

	if ( p->RequireLevel != 0 )
	{
		if ( _type == ITEMGET(13,4) )	// DarkHorse
		{
			this->m_RequireLevel = this->m_PetItem_Level * 2 + 218;
		}
		else if ( _type >= ITEMGET(12,3) && _type <= ITEMGET(12,6) )
		{
			this->m_RequireLevel = p->RequireLevel + (this->m_Level * 5);
		}
		else if ( _type >= ITEMGET(12,36) && _type <= ITEMGET(12,40) ) //Third Wings Fix Required Level
		{
			this->m_RequireLevel = p->RequireLevel;
		}
		else if ( _type == ITEMGET(12,50))
		{
			this->m_RequireLevel = p->RequireLevel;
		}
#if (CRYSTAL_EDITION == 1)		
		else if ( _type >= ITEMGET(12,200) && _type <= ITEMGET(12,254) ) //NEW CRYSTAL WINGS
		{
			this->m_RequireLevel = p->RequireLevel;
		}
#endif
		else if ( _type == ITEMGET(12,42) )	// wing sum 2
		{
			this->m_RequireLevel = p->RequireLevel + (this->m_Level * 5);
		}
		else if ( _type == ITEMGET(12,43) )	// wing sum 3
		{
			this->m_RequireLevel = p->RequireLevel;
		}
		else if ( _type >= ITEMGET(12,7) && _type <= ITEMGET(12,24) )
		{
			this->m_RequireLevel = p->RequireLevel;
		}
		else if ( _type >= ITEMGET(12,44) && _type <= ITEMGET(12,48) )
		{
			this->m_RequireLevel = p->RequireLevel;
		}
		else if ( _type >= ITEMGET(0,0) && _type < ITEMGET(12,0) )
		{
			if ( p->RequireLevel )
				this->m_RequireLevel = p->RequireLevel;
		}
		else
		{
			this->m_RequireLevel = p->RequireLevel + this->m_Level * 4;
		}
	}
	else
	{
		this->m_RequireLevel = 0;
	}

	/*if ( _type == ITEMGET(13,5) )	// Dark Spirit
	{
		this->m_RequireLeaderShip = this->m_PetItem_Level  * 15 + 185;
	}
	else
	{
		this->m_RequireLeaderShip = 0;
	}*/


	if ( this->m_Type == ITEMGET(13,10) )
	{
		if ( ItemLevel <= 2 )
		{
			this->m_RequireLevel = 20;
		}
		else
		{
			this->m_RequireLevel = 50;
		}
	}

	if ( (Attribute2 & 0x3F) > 0 )
	{
		if ( this->m_RequireLevel > 0 )
		{
			if ( _type >= ITEMGET(12,0) )
			{
				if (_type < ITEMGET(12,36) || _type > ITEMGET(12,43) && (_type != ITEMGET(12,50)))
				{
					this->m_RequireLevel += 20;
				}
				if (_type >= ITEMGET(12,130) || _type <= ITEMGET(12,135))
				{
					this->m_RequireLevel += 0;
				}
			}
			//if ( _type < ITEMGET(0,0) || _type >= ITEMGET(12,0) )
			//	this->m_RequireLevel += 20;
		}
	}

	if ( this->m_SetOption != 0 )
	{
		ItemLevel = p->Level + 30;
	}
	
	this->m_Leadership = 0;

	if ( this->m_DamageMax > 0 )
	{
		if ( this->m_SetOption != 0 && ItemLevel != 0 )
		{
			this->m_DamageMax += (this->m_DamageMin * 25) / p->Level + 5;
			this->m_DamageMax += ItemLevel / 40 + 5;
		}
		else
		{
			if ( (Attribute2 & 0x3F)  > 0 )
			{
				if ( iChaosItem != 0 )
				{
					this->m_DamageMax += iChaosItem;
				}
				else if ( p->Level != 0 )
				{
					this->m_DamageMax += (this->m_DamageMin * 25) / p->Level + 5;
				}
			}
		}

		this->m_DamageMax += this->m_Level * 3;

		if ( this->m_Level >= 10 )
		{
			this->m_DamageMax += (this->m_Level - 9) * (this->m_Level - 8) / 2;
		}
	}

	if ( this->m_DamageMin > 0 )	
	{
		if ( this->m_SetOption != 0 && ItemLevel != 0 )
		{
			this->m_DamageMin += (this->m_DamageMin * 25) / p->Level + 5;
			this->m_DamageMin += ItemLevel / 40 + 5;
		}
		else
		{
			if ( (Attribute2 & 0x3F)  > 0 )
			{
				if ( iChaosItem != 0 )
				{
					this->m_DamageMin += iChaosItem;
				}
				else if ( p->Level != 0 )
				{
					this->m_DamageMin += (this->m_DamageMin * 25) / p->Level + 5;
				}
			}
		}

		this->m_DamageMin += this->m_Level * 3;

		if ( this->m_Level >= 10 )
		{
			this->m_DamageMin += (this->m_Level - 9) * (this->m_Level - 8) / 2;
		}
	}

	if ( this->m_Magic > 0 )	
	{
		if ( this->m_SetOption != 0 && ItemLevel != 0 )
		{
			this->m_Magic += (this->m_Magic * 25) / p->Level + 5;
			this->m_Magic += ItemLevel / 60 + 2;
		}
		else
		{
			if ( (Attribute2 & 0x3F)  > 0 )
			{
				if ( iChaosItem != 0 )
				{
					this->m_Magic += iChaosItem;
				}
				else if ( p->Level != 0 )
				{
					this->m_Magic += (this->m_Magic * 25) / p->Level + 5;
				}
			}
		}

		this->m_Magic += this->m_Level * 3;

		if ( this->m_Level >= 10 )
		{
			this->m_Magic += (this->m_Level - 9) * (this->m_Level - 8) / 2;
		}
	}

	if ( p->SuccessfulBlocking > 0 )	
	{
		if ( this->m_SetOption != 0 && ItemLevel != 0 )
		{
			this->m_SuccessfulBlocking += (this->m_SuccessfulBlocking * 25) / p->Level + 5;
			this->m_SuccessfulBlocking += ItemLevel / 40 + 5;
		}
		else
		{
			if ( (Attribute2 & 0x3F)  > 0 )
			{
				if ( p->Level != 0 )
				{
					this->m_SuccessfulBlocking += (this->m_SuccessfulBlocking * 25) / p->Level + 5;
				}
			}
		}

		this->m_SuccessfulBlocking += this->m_Level * 3;

		if ( this->m_Level >= 10 )
		{
			this->m_SuccessfulBlocking += (this->m_Level - 9) * (this->m_Level - 8) / 2;
		}
	}

	if ( p->Defense > 0 )	
	{
		if ( this->m_Type >= ITEMGET(6,0) && this->m_Type < ITEMGET(7,0) )
		{
			this->m_Defense += this->m_Level;

			if ( this->m_SetOption != 0 && ItemLevel != 0 )
			{
				this->m_Defense += (this->m_Defense * 20) / ItemLevel + 2;
			}
		}
		else 
		{
			if ( this->m_SetOption != 0 && ItemLevel != 0 )
			{
				this->m_Defense += (this->m_Defense * 12) / p->Level + (p->Level / 5 ) + 4;
				this->m_Defense += (this->m_Defense * 3) / ItemLevel  + ( ItemLevel / 30 ) + 2;
			}
			else if ( (Attribute2 & 0x3F) > 0 )
			{
				if ( p->Level != 0 )
				{
					this->m_Defense += (this->m_Defense * 12) / p->Level + (p->Level / 5 ) + 4;
				}
			}
			
			if ( ( _type >= ITEMGET(12,3) && _type <= ITEMGET(12,6)) || _type == ITEMGET(13,30) || _type == ITEMGET(12,49) || _type == ITEMGET(13,4) || _type == ITEMGET(12,42) )
			{
				this->m_Defense += this->m_Level * 2;
			}
			else if (( _type >= ITEMGET(12,36) && _type <= ITEMGET(12,40) ) || _type == ITEMGET(12,43) || _type == ITEMGET(12,50)) //Third Wings Defense * 4
			{
				this->m_Defense += this->m_Level * 4;
			}			
#if (CRYSTAL_EDITION == 1)
			else if (( _type >= ITEMGET(12,200) && _type <= ITEMGET(12,254) )) //Third Wings Defense * 4
			{
				this->m_Defense = CWings.GetDefenseLevel(_type,this->m_Level,this->m_Defense);
				//this->m_Defense += this->m_Level * 5;
			}
#endif
			else
			{
				this->m_Defense += this->m_Level * 3;
			}

			//if ( this->m_Level >= 10 )
			//{
			//	this->m_Defense += (this->m_Level - 9) * (this->m_Level - 8) / 2;
			//}
			if ( this->m_Level >= 10 && !(_type >= ITEMGET(12,36) && _type <= ITEMGET(12,40)) && !(_type == ITEMGET(12,43)) && !(_type == ITEMGET(12,50))  //Third Wings Anti-Hack, Prevent add Normal Wings Defense
#if (CRYSTAL_EDITION == 1)
			&& !(_type >= ITEMGET(12,200) && _type <= ITEMGET(12,254)) //NEW Wings Anti-Hack, Prevent add Normal Wings Defense
#endif
			)
			{
				this->m_Defense += (this->m_Level - 9) * (this->m_Level - 8) / 2; //Normal Wings
			}
		}
	}

	if ( _type == ITEMGET(13,30) || _type == ITEMGET(12,49) )
	{
		this->m_Defense += this->m_Level*2 + 15;

		if ( this->m_Level >= 10 )
		{
			this->m_Defense += (this->m_Level - 9) * (this->m_Level - 8) / 2;
		}
	}

	
	if (( _type >= ITEMGET(12,36) ) && (_type <= ITEMGET(12,43)))	//Wings S3 FIX EXC 1
	{
		//this->m_Defense += this->m_Level*2 + 15;

		if ( this->m_Level >= 10 )
		{
			this->m_Defense += (this->m_Level - 9) * (this->m_Level - 8) / 2;
		}
	}

#if (CRYSTAL_EDITION == 1)
	if (( _type >= ITEMGET(12,200) ) && (_type <= ITEMGET(12,254)))	//Wings CRYSTAL FIX EXC 1
	{
		//this->m_Defense += this->m_Level*2 + 15;

		if ( this->m_Level >= 10 )
		{
			this->m_Defense += (this->m_Level - 9) * (this->m_Level - 8) / 2;
		}
	}
#endif

	if ( p->MagicDefense > 0 )
	{
		this->m_MagicDefense += this->m_Level * 3;

		if ( this->m_Level >= 10 )
		{
			this->m_MagicDefense += (this->m_Level - 9) * (this->m_Level - 8) / 2;
		}
	}

	this->m_Level &= MAX_ITEM_LEVEL;

	memset(this->m_Special, 0, sizeof(this->m_Special));
	memset(this->m_SpecialValue, 0, sizeof(this->m_SpecialValue));
	this->m_SpecialNum = 0;
	this->m_SkillOption = 0;
	this->m_LuckOption = 0;
	this->m_Z28Option = 0;
	this->m_SkillChange = FALSE;

	if ( Option1	!= 0 )
	{
		if ( p->SkillType != 0 )
		{
			if ( p->SkillType == 66 )
			{
				this->m_SkillChange = true;
				this->m_Special[this->m_SpecialNum] = 0;
				this->m_SkillOption = 1;
			}
			else
			{
				this->m_Special[this->m_SpecialNum] = p->SkillType;
				this->m_SkillOption = 1;
			}
		}
	}

	if ( this->m_Type == ITEMGET(13,3) )	// Dinorant
	{
		this->m_SkillOption = true;
		this->m_Special[this->m_SpecialNum] = 49;
	}

	if ( this->m_Type == ITEMGET(13,4) )	//Dark Horse
	{
		this->m_SkillOption = true;
		this->m_Special[this->m_SpecialNum] = 62;
	}

	if ( this->m_Type == ITEMGET(13,37) )
	{
		this->m_SkillOption = true;
		this->m_Special[this->m_SpecialNum] = 76; //Fenrir
	}

	if ( this->m_Type == ITEMGET(5,21) )
	{
		this->m_SkillOption = true;
		this->m_Special[this->m_SpecialNum] = 223; //Sahamut
	}

	if ( this->m_Type == ITEMGET(5,22) )
	{
		this->m_SkillOption = true;
		this->m_Special[this->m_SpecialNum] = 224; //Nail
	}

	if ( this->m_Type == ITEMGET(5,23) )
	{
		this->m_SkillOption = true;
		this->m_Special[this->m_SpecialNum] = 225; //Ghost Phantom
	}

	this->m_SpecialNum++;

	if ( Option2 != 0 )
	{
		if ( _type >= ITEMGET(0,0) && _type < ITEMGET(12,0) )
		{
			this->m_Special[this->m_SpecialNum] = 84;
			this->m_LuckOption = 1;
		}

		if ( _type >= ITEMGET(12,0) && _type <= ITEMGET(12,6) )	// Wings
		{
			this->m_Special[this->m_SpecialNum] = 84;
			this->m_LuckOption = 1;
		}

		if ( _type >= ITEMGET(12,130) && _type <= ITEMGET(12,135) )	// Wings
		{
			this->m_Special[this->m_SpecialNum] = 84;
			this->m_LuckOption = 1;
		}

		if ( _type >= ITEMGET(12,36) && _type <= ITEMGET(12,43) )	// Wings S3 LUCK FIX
		{
			this->m_Special[this->m_SpecialNum] = 84;
			this->m_LuckOption = 1;
		}

		if ( _type == ITEMGET(12,50))	// Wings S5 LUCK FIX
		{
			this->m_Special[this->m_SpecialNum] = 84;
			this->m_LuckOption = 1;
		}
#if (CRYSTAL_EDITION == 1)

		if ( _type >= ITEMGET(12,200) && _type <= ITEMGET(12,254) )	// Wings CRYSTAL LUCK FIX
		{
			this->m_Special[this->m_SpecialNum] = 84;
			this->m_LuckOption = 1;
		}
#endif

		if ( _type == ITEMGET(13,30) || _type == ITEMGET(12,49))	// Cape of Lord
		{
			this->m_Special[this->m_SpecialNum] = 84;
			this->m_LuckOption = 1;
		}
	}

	this->m_SpecialNum++;

	if ( Option3 != 0 )
	{
		if ( _type >= ITEMGET(0,0) && _type < ITEMGET(5,0) )
		{
			this->m_Special[this->m_SpecialNum] = 80;
			this->m_Z28Option = Option3;
			this->m_RequireStrength +=  Option3 * 4;
		}

		if ( _type >= ITEMGET(5,0) && _type < ITEMGET(6,0) )
		{
			this->m_Special[this->m_SpecialNum] = 81;
			this->m_Z28Option = Option3;
			this->m_RequireStrength +=  Option3 * 4;
		}
		
		if ( _type >= ITEMGET(6,0) && _type < ITEMGET(7,0) )
		{
			this->m_Special[this->m_SpecialNum] = 82;
			this->m_Z28Option = Option3;
			this->m_RequireStrength +=  Option3 * 4;
		}

		if ( _type >= ITEMGET(7,0) && _type < ITEMGET(12,0) )
		{
			this->m_Special[this->m_SpecialNum] = 83;
			this->m_Z28Option = Option3;
			this->m_RequireStrength +=  Option3 * 4;
		}

		if ((_type >= ITEMGET(13,8) && _type < ITEMGET(13,14))  || 
			(_type >= ITEMGET(13,20) && _type <= ITEMGET(13,28))||
			 _type == ITEMGET(13,107) )
		{
			if ( _type == ITEMGET(13,24) ) // Ring of MAgic
			{
				this->m_Special[this->m_SpecialNum] = 172;
				this->m_Z28Option = Option3;
			}
			else if ( _type == ITEMGET(13,28) ) // Pendant of abilit
			{
				this->m_Special[this->m_SpecialNum] = 173;
				this->m_Z28Option = Option3;
			}
			else  
			{
				this->m_Special[this->m_SpecialNum] = 85;
				this->m_Z28Option = Option3;
			}
		}
		
		if ( _type == ITEMGET(13,30) || _type == ITEMGET(12,49)) // Cape of lord 
		{
			this->m_Special[this->m_SpecialNum] = 80;
			this->m_Z28Option = Option3;
			this->m_RequireStrength +=  Option3 * 4;
		}

		if ( _type == ITEMGET(12,0) )	// Wing elf
		{
			this->m_Special[this->m_SpecialNum] = 85;
			this->m_Z28Option = Option3;
		}
		else if ( _type == ITEMGET(12,1) ) // winf Heaven
		{
			this->m_Special[this->m_SpecialNum] = 81;
			this->m_Z28Option = Option3;
			this->m_RequireStrength +=  Option3 * 4;
		}
		else if ( _type == ITEMGET(12,2) ) // wing devil
		{
			this->m_Special[this->m_SpecialNum] = 80;
			this->m_Z28Option = Option3;
			this->m_RequireStrength +=  Option3 * 4;
		}
		else if ( _type == ITEMGET(12,3) ) // wing spitits
		{
			this->m_Special[this->m_SpecialNum] = 80;
			this->m_Z28Option = Option3;
			this->m_RequireStrength +=  Option3 * 4;

			if ( (this->m_NewOption&0x20) != 0 )
			{
				this->m_Special[this->m_SpecialNum] = 85;
			}
			else
			{
				this->m_Special[this->m_SpecialNum] = 80;
			}

		}
		else if ( _type == ITEMGET(12,4) ) // wing soul
		{
			this->m_Z28Option = Option3;
			this->m_RequireStrength +=  Option3 * 4;

			if ( (this->m_NewOption&0x20) != 0 )
			{
				this->m_Special[this->m_SpecialNum] = 81;
			}
			else
			{
				this->m_Special[this->m_SpecialNum] = 85;
			}

		}
		else if ( _type == ITEMGET(12,5) ) // wing dragon
		{
			this->m_Z28Option = Option3;
			this->m_RequireStrength +=  Option3 * 4;

			if ( (this->m_NewOption&0x20) != 0 )
			{
				this->m_Special[this->m_SpecialNum] = 80;
			}
			else
			{
				this->m_Special[this->m_SpecialNum] = 85;
			}

		}			
		else if ( _type == ITEMGET(12,6) ) // wing darkness
		{
			this->m_Z28Option = Option3;
			this->m_RequireStrength +=  Option3 * 4;

			if ( (this->m_NewOption&0x20) != 0 )
			{
				this->m_Special[this->m_SpecialNum] = 80;
			}
			else
			{
				this->m_Special[this->m_SpecialNum] = 81;
			}

		}
		else if ( _type == ITEMGET(12,36) ) // Third Wings "Wing of Storm"
		{
			this->m_Z28Option = Option3;
			this->m_RequireStrength +=  Option3 * 4;

			if ( (this->m_NewOption&0x20) >= 0 && (this->m_NewOption&0x10) != 0 ) //JoL + Increase Attack Speed PS: I set both Add. Defense and Add. Damage enabled because when item is full have Additional Damage!
			{
				this->m_Special[this->m_SpecialNum] = 80; //Additional Damage
			}
			else if ( (this->m_NewOption&0x20) == 0 && (this->m_NewOption&0x10) == 0 ) //Only JoL Option
			{
				this->m_Special[this->m_SpecialNum] = 85; //Recover Life
			}
			else if ( (this->m_NewOption&0x20) != 0 && (this->m_NewOption&0x10) == 0 ) //JoL + Take No Effect PS: I set 0x10 equal 0 because Additional Defense only appears with Increase Attack Speed Disabled! It's a Nice Anti-hack or Checker :D
			{
				this->m_Special[this->m_SpecialNum] = 83; //Additional Defense
			}

		}
		else if ( _type == ITEMGET(12,37) ) // Third Wings "Wing of Vortex"
		{
			this->m_Z28Option = Option3;
			this->m_RequireStrength +=  Option3 * 4;

			if ( (this->m_NewOption&0x20) >= 0 && (this->m_NewOption&0x10) != 0 ) //JoL + Increase Attack Speed PS: I set both Add. Defense and Add. Magic enabled because when item is full have Additional Damage!
			{
				this->m_Special[this->m_SpecialNum] = 81; //Additional Magic
			}
			else if ( (this->m_NewOption&0x20) == 0 && (this->m_NewOption&0x10) == 0 ) //Only JoL Option
			{
				this->m_Special[this->m_SpecialNum] = 85; //Recover Life
			}
			else if ( (this->m_NewOption&0x20) != 0 && (this->m_NewOption&0x10) == 0 ) //JoL + Take No Effect PS: I set 0x10 equal 0 because Additional Defense only appears with Increase Attack Speed Disabled! It's a Nice Anti-hack or Checker :D
			{
				this->m_Special[this->m_SpecialNum] = 83; //Additional Defense
			}

		}
		else if ( _type == ITEMGET(12,38) ) // Third Wings "Wing of Illusion"
		{
			this->m_Z28Option = Option3;
			this->m_RequireStrength +=  Option3 * 4;

			if ( (this->m_NewOption&0x20) >= 0 && (this->m_NewOption&0x10) != 0 ) //JoL + Increase Attack Speed PS: I set both Add. Defense and Add. Damage enabled because when item is full have Additional Damage!
			{
				this->m_Special[this->m_SpecialNum] = 80; //Additional Damage
			}
			else if ( (this->m_NewOption&0x20) == 0 && (this->m_NewOption&0x10) == 0 ) //Only JoL Option
			{
				this->m_Special[this->m_SpecialNum] = 85; //Recover Life
			}
			else if ( (this->m_NewOption&0x20) != 0 && (this->m_NewOption&0x10) == 0 ) //JoL + Take No Effect PS: I set 0x10 equal 0 because Additional Defense only appears with Increase Attack Speed Disabled! It's a Nice Anti-hack or Checker :D
			{
				this->m_Special[this->m_SpecialNum] = 83; //Additional Defense
			}

		}
		else if ( _type == ITEMGET(12,39) ) // Third Wings "Wing of Downcast"
		{
			this->m_Z28Option = Option3;
			this->m_RequireStrength +=  Option3 * 4;

			if ( (this->m_NewOption&0x20) >= 0 && (this->m_NewOption&0x10) != 0 ) //JoL + Increase Attack Speed PS: I set both Add. Defense and Add. Damage enabled because when item is full have Additional Damage!
			{
				this->m_Special[this->m_SpecialNum] = 80; //Additional Damage
			}
			else if ( (this->m_NewOption&0x20) == 0 && (this->m_NewOption&0x10) == 0 ) //Only JoL Option
			{
				this->m_Special[this->m_SpecialNum] = 85; //Recover Life
			}
			else if ( (this->m_NewOption&0x20) != 0 && (this->m_NewOption&0x10) == 0 ) //JoL + Take No Effect PS: I set 0x10 equal 0 because Additional Defense only appears with Increase Attack Speed Disabled! It's a Nice Anti-hack or Checker :D
			{
				this->m_Special[this->m_SpecialNum] = 81; //Additional Magic
			}

		}
		else if ( _type == ITEMGET(12,40) || _type == ITEMGET(12,50)) // Third Wings "Mantle of Monarch"
		{
			this->m_Z28Option = Option3;
			this->m_RequireStrength +=  Option3 * 4;

			if ( (this->m_NewOption&0x20) >= 0 && (this->m_NewOption&0x10) != 0 ) //JoL + Increase Attack Speed PS: I set both Add. Defense and Add. Damage enabled because when item is full have Additional Damage!
			{
				this->m_Special[this->m_SpecialNum] = 80; //Additional Damage
			}
			else if ( (this->m_NewOption&0x20) == 0 && (this->m_NewOption&0x10) == 0 ) //Only JoL Option
			{
				this->m_Special[this->m_SpecialNum] = 85; //Recover Life
			}
			else if ( (this->m_NewOption&0x20) != 0 && (this->m_NewOption&0x10) == 0 ) //JoL + Take No Effect PS: I set 0x10 equal 0 because Additional Defense only appears with Increase Attack Speed Disabled! It's a Nice Anti-hack or Checker :D
			{
				this->m_Special[this->m_SpecialNum] = 83; //Additional Defense
			}

		}
#if (CRYSTAL_EDITION == 1)
		else if ( _type >= ITEMGET(12,200) && _type <= ITEMGET(12,254)) // CRYSTAL Wings
		{
			this->m_Z28Option = Option3;
			this->m_RequireStrength +=  Option3 * 4;

			if ( (this->m_NewOption&0x20) >= 0 && (this->m_NewOption&0x10) != 0 ) //JoL + Increase Attack Speed PS: I set both Add. Defense and Add. Damage enabled because when item is full have Additional Damage!
			{
				this->m_Special[this->m_SpecialNum] = 80; //Additional Damage
			}
			else if ( (this->m_NewOption&0x20) == 0 && (this->m_NewOption&0x10) == 0 ) //Only JoL Option
			{
				this->m_Special[this->m_SpecialNum] = 85; //Recover Life
			}
			else if ( (this->m_NewOption&0x20) != 0 && (this->m_NewOption&0x10) == 0 ) //JoL + Take No Effect PS: I set 0x10 equal 0 because Additional Defense only appears with Increase Attack Speed Disabled! It's a Nice Anti-hack or Checker :D
			{
				this->m_Special[this->m_SpecialNum] = 83; //Additional Defense
			}

		}
#endif

		else if ( _type == ITEMGET(12,41) ) // Wing of Mystery
		{
			this->m_Special[this->m_SpecialNum] = 81;
			this->m_Z28Option = Option3;
			this->m_RequireStrength +=  Option3 * 4;
		}
		else if ( _type == ITEMGET(12,42) ) // wing of Despair
		{
			this->m_Z28Option = Option3;
			this->m_RequireStrength +=  Option3 * 4;

			if ( (this->m_NewOption&0x20) != 0 )
			{
				this->m_Special[this->m_SpecialNum] = 81;
			}
			else
			{
				this->m_Special[this->m_SpecialNum] = 85;
			}

		}
		else if ( _type == ITEMGET(12,43) ) // Third Wings "Wing of Violent Wind"
		{
			this->m_Z28Option = Option3;
			this->m_RequireStrength +=  Option3 * 4;

			if ( (this->m_NewOption&0x20) >= 0 && (this->m_NewOption&0x10) != 0 ) //JoL + Increase Attack Speed PS: I set both Add. Defense and Add. Damage enabled because when item is full have Additional Damage!
			{
				this->m_Special[this->m_SpecialNum] = 80; //Additional Damage
			}
			else if ( (this->m_NewOption&0x20) == 0 && (this->m_NewOption&0x10) == 0 ) //Only JoL Option
			{
				this->m_Special[this->m_SpecialNum] = 85; //Recover Life
			}
			else if ( (this->m_NewOption&0x20) != 0 && (this->m_NewOption&0x10) == 0 ) //JoL + Take No Effect PS: I set 0x10 equal 0 because Additional Defense only appears with Increase Attack Speed Disabled! It's a Nice Anti-hack or Checker :D
			{
				this->m_Special[this->m_SpecialNum] = 83; //Additional Defense
			}
		}
		
		else if ( _type == ITEMGET(12,130) ) // Cape of lord 
		{
			this->m_Special[this->m_SpecialNum] = 80;
			this->m_Z28Option = Option3;
			this->m_RequireStrength +=  Option3 * 4;
		}
		else if ( _type == ITEMGET(12,131) ) // Wing of Mystery
		{
			this->m_Special[this->m_SpecialNum] = 81;
			this->m_Z28Option = Option3;
			this->m_RequireStrength +=  Option3 * 4;
		}
		else if ( _type == ITEMGET(12,132) )	// Wing elf
		{
			this->m_Special[this->m_SpecialNum] = 85;
			this->m_Z28Option = Option3;
		}
		else if ( _type == ITEMGET(12,133) ) // winf Heaven
		{
			this->m_Special[this->m_SpecialNum] = 81;
			this->m_Z28Option = Option3;
			this->m_RequireStrength +=  Option3 * 4;
		}
		else if ( _type == ITEMGET(12,134) ) // wing devil
		{
			this->m_Special[this->m_SpecialNum] = 80;
			this->m_Z28Option = Option3;
			this->m_RequireStrength +=  Option3 * 4;
		}
		else if ( _type == ITEMGET(12,135) ) // Cape of rage 
		{
			this->m_Special[this->m_SpecialNum] = 80;
			this->m_Z28Option = Option3;
			this->m_RequireStrength +=  Option3 * 4;
		}
		else if ( _type == ITEMGET(13,3) ) // dinorant
		{
			this->m_Z28Option = Option3;
	
			if ( (this->m_Z28Option&0x02) != 0 )
			{
				this->m_Special[this->m_SpecialNum] = 0x67;
				this->m_SpecialNum++;
			}
		
			if ( (this->m_Z28Option&0x04) != 0 )
			{
				this->m_Special[this->m_SpecialNum] = 0x61;
				this->m_SpecialNum++;
				this->m_AttackSpeed+=5;
			}
		
			if ( (this->m_Z28Option&0x01) != 0 )
			{
				this->m_Special[this->m_SpecialNum] = 0x68;
			}
		}

		if ( _type == ITEMGET(13,30) ||  _type == ITEMGET(12,49)) // Cape ofLord
		{
			this->m_Z28Option = Option3;
			this->m_RequireStrength +=  Option3 * 4;
			this->m_Special[this->m_SpecialNum] = 0x50;
		}
	}



	this->m_SpecialNum++;

	if ( (_type >=ITEMGET(6,0) && _type <ITEMGET(12,0) ) || ( _type >=ITEMGET(13,8) && _type <= ITEMGET(13,9)) || (_type>=ITEMGET(13,21) && _type <= ITEMGET(13,24) ) )
	{
		if ( ((this->m_NewOption>>5)&1) != 0 )
		{
			this->m_Special[this->m_SpecialNum] = 0x56;
			this->m_SpecialNum++;
		}

		if ( ((this->m_NewOption>>4)&1) != 0 )
		{
			this->m_Special[this->m_SpecialNum] = 0x57;
			this->m_SpecialNum++;
		}

		if ( ((this->m_NewOption>>3)&1) != 0 )
		{
			this->m_Special[this->m_SpecialNum] = 0x58;
			this->m_SpecialNum++;
		}

		if ( ((this->m_NewOption>>2)&1) != 0 )
		{
			this->m_Special[this->m_SpecialNum] = 0x59;
			this->m_SpecialNum++;
		}

		if ( ((this->m_NewOption>>1)&1) != 0 )
		{
			this->m_Special[this->m_SpecialNum] = 0x5A;
			this->m_SpecialNum++;
		}

		if ( ((this->m_NewOption)&1) != 0 )
		{
			this->m_Special[this->m_SpecialNum] = 0x5B;
			this->m_SpecialNum++;
		}
	}

	if ( (_type >=ITEMGET(0,0) && _type <ITEMGET(6,0) ) || ( _type >=ITEMGET(13,12) && _type <= ITEMGET(13,13)) || (_type>=ITEMGET(13,25) && _type <= ITEMGET(13,28) ) )
	{
		if ( ((this->m_NewOption>>5)&1) != 0 )
		{
			this->m_Special[this->m_SpecialNum] = 0x5C;
			this->m_SpecialNum++;
		}

		if ( (_type >=ITEMGET(5,0) && _type <ITEMGET(6,0) ) || (_type ==ITEMGET(13,12) ) || (_type == ITEMGET(13,25)) || (_type == ITEMGET(13,27) ) )
		{
			if ( ((this->m_NewOption>>4)&1) != 0 )
			{
				this->m_Special[this->m_SpecialNum] = 0x5F;
				this->m_SpecialNum++;
			}
			if ( ((this->m_NewOption>>3)&1) != 0 )
			{
				this->m_Special[this->m_SpecialNum] = 0x60;
				this->m_SpecialNum++;
			}
		}
		else
		{
			if ( ((this->m_NewOption>>4)&1) != 0 )
			{
				this->m_Special[this->m_SpecialNum] = 0x5D;
				this->m_SpecialNum++;
			}
			if ( ((this->m_NewOption>>3)&1) != 0 )
			{
				this->m_Special[this->m_SpecialNum] = 0x5E;
				this->m_SpecialNum++;
			}
		}

		if ( ((this->m_NewOption>>2)&1) != 0 )
		{
			this->m_Special[this->m_SpecialNum] = 0x61;
			this->m_SpecialNum++;
			this->m_AttackSpeed += 7;
		}

		if ( ((this->m_NewOption>>1)&1) != 0 )
		{
			this->m_Special[this->m_SpecialNum] = 0x62;
			this->m_SpecialNum++;
		}

		if ( ((this->m_NewOption)&1) != 0 )
		{
			this->m_Special[this->m_SpecialNum] = 0x63;
			this->m_SpecialNum++;
		}
	}

	//if ( ((_type >= ITEMGET(12,3)) && (_type <= ITEMGET(12,6))) || (_type ==ITEMGET(13,30)) )
	if ( ((_type >= ITEMGET(12,3)) && (_type <= ITEMGET(12,6))) || ((_type >= ITEMGET(12,36)) && (_type <= ITEMGET(12,40))) || (_type ==ITEMGET(13,30)) || (_type ==ITEMGET(12,42)) || (_type ==ITEMGET(12,43)) || (_type ==ITEMGET(12,49)) || (_type ==ITEMGET(12,50))  //Third Wings Applied
#if (CRYSTAL_EDITION == 1)
			|| (_type >= ITEMGET(12,200) && _type <= ITEMGET(12,254)) //CRYSTAL Wings Applied
#endif
			)
	{
		if ( ((this->m_NewOption)&1) != 0 )
		{
			this->m_Special[this->m_SpecialNum] = 0x64;
			this->m_SpecialNum++;
		}

		if ( ((this->m_NewOption)&2) != 0 )
		{
			this->m_Special[this->m_SpecialNum] = 0x65;
			this->m_SpecialNum++;
		}

		if ( ((this->m_NewOption)&4) != 0 )
		{
			this->m_Special[this->m_SpecialNum] = 0x66;
			this->m_SpecialNum++;
		}

		if ( ((this->m_NewOption)&8) != 0 )
		{
			this->m_Special[this->m_SpecialNum] = 0x69;
			this->m_SpecialNum++;
		}
	}

	/*if ( _type == ITEMGET(13,37) ) //Fenrir
	{
		if ( (this->m_NewOption &1) )
		{
			this->m_Special[this->m_SpecialNum] = 0x6A;
			this->m_SpecialNum++;
		}
		else if ( (this->m_NewOption &2) )
		{
			this->m_Special[this->m_SpecialNum] = 0x6B;
			this->m_SpecialNum++;
		}
	}*/

	if ( SOptionStatValue != 0 )
	{
		if (p->SetAttr != 0 )
		{
			this->m_Special[this->m_SpecialNum] = p->SetAttr + 0xC3;
			this->m_SetAddStat = SOptionStatValue * 5;
			this->m_SpecialNum++;
		}
	}

	this->m_Part = p->ItemSlot;
	this->Value();

	if ( this->m_Type != ITEMGET(4,7) && this->m_Type != ITEMGET(4,15) ) // arrow and bolts
	{
		this->m_DamageMinOrigin = this->m_DamageMin;
		this->m_DefenseOrigin = this->m_Defense;
		this->m_DamageMin -= (int)(this->m_DamageMin * this->m_CurrentDurabilityState);
		this->m_DamageMax -= (int)(this->m_DamageMax * this->m_CurrentDurabilityState);
		this->m_Defense -= (int)(this->m_Defense * this->m_CurrentDurabilityState);
		this->m_SuccessfulBlocking -= (int)(this->m_SuccessfulBlocking * this->m_CurrentDurabilityState);

		if ( this->m_Durability < 1.0f )
		{
			this->m_AttackSpeed = 0;
		}
	}
	
	if ( this->m_Durability == 0.0f )
	{
		memset(this->m_Resistance , 0, sizeof(this->m_Resistance));
	}
	
}

 
/*int CItem::GetLevel() 
{
	return ItemAttribute[this->m_Type].Level;
}*/

DWORD CItem::GetNumber()
{
	__try
	{
		if (this->m_Type < 0)
		{
			return 0;
		}
		if (this->m_IsValidItem == false)
		{
			return 0;
		}

		return this->m_Number;
	}
	__except( EXCEPTION_ACCESS_VIOLATION == GetExceptionCode() )
	{
		return 0;
	}
}


void CItem::Value()
{
	if ( this->m_Type == -1 )
	{
		return;
	}

	LPITEM_ATTRIBUTE p = &ItemAttribute[this->m_Type];

	if ( p->BuyMoney != 0 )
	{
		this->m_SellMoney = p->BuyMoney;
		this->m_BuyMoney = p->BuyMoney;
		this->m_SellMoney /= 3;
		
		if ( this->m_SellMoney >= 1000 )
		{
			this->m_SellMoney = this->m_SellMoney/100*100;
		}
		else if ( this->m_SellMoney >= 100 )
		{
			this->m_SellMoney = this->m_SellMoney/10*10;
		}

		if ( this->m_BuyMoney >= 1000 )
		{
			this->m_BuyMoney = this->m_BuyMoney/100*100;
		}
		else if ( this->m_BuyMoney >= 100 )
		{
			this->m_BuyMoney = this->m_BuyMoney/10*10;
		}

		return;
	}

	int Level = (int)sqrt((float)this->m_Level);
	__int64 Gold = 0;
	int Type = (this->m_Type)/MAX_SUBTYPE_ITEMS;
	int Level2 = p->Level + this->m_Level*3;
	int excellent = 0;

	for (int i=0;i<this->m_SpecialNum;i++)
	{
		switch ( this->m_Special[i] )
		{
			case 0x56:	// 86.
			case 0x57:
			case 0x58:
			case 0x59:
			case 0x5A:
			case 0x5B:
			case 0x5C:
			case 0x5D:
			case 0x5E:
			case 0x5F:
			case 0x60:
			case 0x61:
			case 0x62:
			case 0x63:	// 99.
				excellent = 1;
		}
	}

	if ( excellent != 0 )
	{
		Level2 += 25;
	}

	if ( this->m_Type == ITEMGET(4,15) ) // Arrow
	{
		int lc10 = 70;

		if ( this->m_Level == 1 )
		{
			lc10 = 1200;
		}
		else if ( this->m_Level == 2 )
		{
			lc10 = 2000;
		}

		if ( this->m_Durability > 0.0f )
		{
			Gold = lc10 * this->m_Durability/p->Durability;
		}
	}
	else if ( this->m_Type == ITEMGET(4,7) ) // Arrow of Crossbow
	{
		int lc11;

		if ( this->m_Durability > 0.0f )
		{
			lc11 = 100;

			if ( this->m_Level == 1 )
			{
				lc11 = 1400;
			}
			else if ( this->m_Level == 2 )
			{
				lc11 = 2200;
			}
		
			Gold = lc11 * this->m_Durability/p->Durability;
		}
	}
	else if ( this->m_Type == ITEMGET(14,13) ) // Bless
	{
		Gold = ReadConfig.CItem__ValueBless;
	}
	else if ( this->m_Type == ITEMGET(14,14) ) // soul
	{
		Gold =  ReadConfig.CItem__ValueSoul;
	}
	else if ( this->m_Type == ITEMGET(12,15) ) // chaos
	{
		Gold =  ReadConfig.CItem__ValueChaos;
	}
	else if ( this->m_Type == ITEMGET(14,16) ) // Life
	{
		Gold =  ReadConfig.CItem__ValueLife;
	}
	else if ( this->m_Type == ITEMGET(14,22) ) // Creation
	{
		Gold =  ReadConfig.CItem__ValueCreation;
	}
	else if ( this->m_Type == ITEMGET(12,30) ) //Pack Of Bless
	{
		Gold = (this->m_Level+1)* ReadConfig.CItem__ValueBless*10;
	}
	else if ( this->m_Type == ITEMGET(12,31) ) //Pack Of Soul
	{
		Gold = (this->m_Level+1)* ReadConfig.CItem__ValueSoul*10;
	}
	else if ( this->m_Type == ITEMGET(12,136) ) //Jewel of Life Bundle
	{
		Gold = (this->m_Level+1)* ReadConfig.CItem__ValueLife*10;
	}
	else if ( this->m_Type == ITEMGET(12,137) ) //Jewel of Creation Bundle
	{
		Gold = (this->m_Level+1)* ReadConfig.CItem__ValueCreation*10;
	}
	else if ( this->m_Type == ITEMGET(12,138) ) //Jewel of Guardian Bundle
	{
		Gold = (this->m_Level+1)* ReadConfig.CItem__ValueGuardian*10;
	}
	//else if ( this->m_Type == ITEMGET(12,140) ) //Jewel of Harmony Bundle
	//{
	//	Gold = (this->m_Level+1)* ReadConfig.???? *10;
	//}
	//else if ( this->m_Type == ITEMGET(12,139) ) //Gemstone Bundle
	//{
	//	Gold = (this->m_Level+1)* ReadConfig.???? *10;
	//}
	else if ( this->m_Type == ITEMGET(12,141) ) //Jewel of Chaos Bundle
	{
		Gold = (this->m_Level+1)* ReadConfig.CItem__ValueChaos*10;
	}
	else if ( this->m_Type == ITEMGET(13,15) ) //Fruits
	{
		Gold = 33000000;
	}
	else if ( this->m_Type == ITEMGET(13,14) ) //Loch Feather
	{
		if ( this->m_Level == 1 )
		{
			Gold = 7500000;	// Crest ofMonarch
		}
		else
		{
			Gold = 180000;	// Blue Feather
		}
	}
	else if ( this->m_Type == ITEMGET(14,31) ) // Jewel of Guardian
	{
		Gold =  ReadConfig.CItem__ValueGuardian;
	}
	else if ( this->m_Type == ITEMGET(14,7) ) // Siege Potion
	{
		if ( this->m_Level == 0 )
		{
			Gold = (int)(this->m_Durability) *  ReadConfig.CItem__ValueBlessWater;
		}
		else if ( this->m_Level == 1 )
		{
			Gold = (int)(this->m_Durability) *  ReadConfig.CItem__ValueSoulWater;
		}
	}
	else if ( this->m_Type == ITEMGET(13,11) ) // Order(Guardian/Life Stone)
	{
		if ( this->m_Level == 1 )
		{
			Gold = 2400000;
		}
	}
	else if ( this->m_Type == ITEMGET(13,7) ) // Contract(Summon)
	{
		if ( this->m_Level == 0 )
		{
			Gold = 1500000;
		}
		else if ( this->m_Level == 1 )
		{
			Gold = 1200000;
		}
	}
	else if ( this->m_Type == ITEMGET(13,32) ) 
	{
		Gold = (int)this->m_Durability * 150;
	}
	else if ( this->m_Type == ITEMGET(13,33) ) 
	{
		Gold = (int)this->m_Durability * 300;
	}
	else if ( this->m_Type == ITEMGET(13,34) ) 
	{
		Gold = (int)this->m_Durability * 3000;
	}
	else if ( this->m_Type == ITEMGET(13,35) ) 
	{
		Gold = 30000;
	}
	else if ( this->m_Type == ITEMGET(13,36) ) 
	{
		Gold = 90000;
	}
	else if ( this->m_Type == ITEMGET(13,37) ) 
	{
		Gold = 150000;
	}
	else if ( this->m_Type == ITEMGET(14,35) ) 
	{
		Gold = (int)this->m_Durability * 2000;
	}
	else if ( this->m_Type == ITEMGET(14,36) ) 
	{
		Gold = (int)this->m_Durability * 4000;
	}
	else if ( this->m_Type == ITEMGET(14,37) ) 
	{
		Gold = (int)this->m_Durability * 6000;
	}
	else if ( this->m_Type == ITEMGET(14,38) ) 
	{
		Gold = (int)this->m_Durability * 2500;
	}
	else if ( this->m_Type == ITEMGET(14,39) ) 
	{
		Gold = (int)this->m_Durability * 5000;
	}
	else if ( this->m_Type == ITEMGET(14,40) ) 
	{
		Gold = (int)this->m_Durability * 7500;
	}
	else if ( this->m_Type == ITEMGET(13,3) ) // Dinorant
	{
		Gold = 960000;

		for ( int k=0;k<3;k++)
		{
			if ( (this->m_Z28Option & (1<<k)) != 0 )
			{
				Gold += 300000;
			}
		}
	}
	else if ( this->m_Type == ITEMGET(14,17) ) // Devil Eye
	{
		switch ( this->m_Level )
		{
			case 1:
				Gold = 15000;
				break;
			case 2:
				Gold = 21000;
				break;
			case 3:
				Gold = 30000;
				break;
			case 4:
				Gold = 45000;
				break;
			case 5:
				Gold = 60000;
				break;
			case 6:
				Gold = 75000;
				break;
			default:
				Gold = 30000;
				break;
		}
	}
	else if ( this->m_Type == ITEMGET(14,18) ) // Devil Key
	{
		switch ( this->m_Level )
		{
			case 1:
				Gold = 15000;
				break;
			case 2:
				Gold = 21000;
				break;
			case 3:
				Gold = 30000;
				break;
			case 4:
				Gold = 45000;
				break;
			case 5:
				Gold = 60000;
				break;
			case 6:
				Gold = 75000;
				break;
			default:
				Gold = 30000;
				break;
		}
	}
	else if ( this->m_Type == ITEMGET(14,19) ) // Devil's Invitation
	{
		switch ( this->m_Level )
		{
			case 1:
				Gold = 60000;
				break;
			case 2:
				Gold = 84000;
				break;
			case 3:
				Gold = 120000;
				break;
			case 4:
				Gold = 180000;
				break;
			case 5:
				Gold = 240000;
				break;
			case 6:
				Gold = 300000;
				break;
			default:
				Gold = 120000;
				break;
		}
	}
	else if ( this->m_Type == ITEMGET(14,20) ) // Remedy Of Love
	{
		Gold = 900;
	}
	else if ( this->m_Type == ITEMGET(14,21) ) // Rena
	{
		switch ( this->m_Level )
		{
			case 0:	// Rena
				Gold = 9000;
				this->m_Durability = this->m_BaseDurability;
			case 1:	// Stone
				Gold = 9000;
				this->m_Durability = this->m_BaseDurability;
				break;
			case 3:	// Mark of Lord
				Gold = (int)(this->m_Durability) * 3900;
				break;
			default:
				Gold = 9000;
				this->m_Durability = this->m_BaseDurability;
		}
	}
	else if ( this->m_Type == ITEMGET(14,9) && this->m_Level == 1) // Ale
	{
		Gold = 1000;
	}
	else if ( this->m_Type == ITEMGET(13,18) ) // Invisibility Cloak
	{
		switch ( this->m_Level )
		{
			case 1:
				Gold = 150000;
				break;
			case 2:
				Gold = 660000;
				break;
			case 3:
				Gold = 720000;
				break;
			case 4:
				Gold = 780000;
				break;
			case 5:
				Gold = 840000;
				break;
			case 6:
				Gold = 900000;
				break;
			case 7:
				Gold = 960000;
				break;
			default:
				Gold = 600000;
				break;
		}
	}
	else if ( this->m_Type == ITEMGET(13,16) || this->m_Type == ITEMGET(13,17) ) // Blood and Paper of BloodCastle
	{
		switch ( this->m_Level )
		{
			case 1:
				Gold = 15000;
				break;
			case 2:
				Gold = 21000;
				break;
			case 3:
				Gold = 30000;
				break;
			case 4:
				Gold = 39000;
				break;
			case 5:
				Gold = 48000;
				break;
			case 6:
				Gold = 60000;
				break;
			case 7:
				Gold = 75000;
				break;
			default:
				Gold = 15000;
				break;
		}
	}
	else if ( this->m_Type == ITEMGET(13,29) ) // Armor of Guardman
	{
		Gold = 5000;
	}
	else if ( this->m_Type == ITEMGET(13,20) ) // Wizards Ring
	{
		switch ( this->m_Level )
		{
			case 0:
				Gold =30000;
				break;

			default:
				Gold = 0;
		}
	}
	else if ( this->m_Type == ITEMGET(13,31) ) // Spirit
	{
		switch ( this->m_Level )
		{
			case 0:
				Gold = 30000000;
				break;

			case 1:
				Gold = 15000000;
		}
	}
	else if ( this->m_Type == ITEMGET(14,28) ) // Lost Map
	{
		Gold = 600000;
	}
	else if ( this->m_Type == ITEMGET(14,29) ) // Symbol of Kundun
	{
		Gold = ((10000.0f) * this->m_Durability) * 3.0f;
	}
	else if ( this->m_Type == ITEMGET(14,45) ||
		      this->m_Type == ITEMGET(14,46) ||
			  this->m_Type == ITEMGET(14,47) ||
			  this->m_Type == ITEMGET(14,48) ||
			  this->m_Type == ITEMGET(14,49) ||
			  this->m_Type == ITEMGET(14,50) )
	{
		Gold = ((50.0f) * this->m_Durability) * 3.0f;
	}
	else if ( this->m_Type == ITEMGET(12,26) )
	{
		switch ( this->m_Level )
		{
			case 0:
				Gold = 60000;
				break;
		}
	}
	else if ( p->Value > 0 )
	{
		Gold += (p->Value * p->Value * 10)/12;

		if ( this->m_Type >= ITEMGET(14,0) && this->m_Type <= ITEMGET(14,8) )	// Potions + Antidote
		{
			if ( this->m_Level > 0 )
			{
				Gold *= (__int64)(pow(2.0f, Level)) ;
			}

			Gold = Gold/10*10;
			Gold *= (__int64)this->m_Durability;
			this->m_BuyMoney = Gold;
			this->m_SellMoney = Gold/3;
			this->m_SellMoney = this->m_SellMoney/10*10;
			return;
		}
	}
	else if ( (Type == 12 && this->m_Type > ITEMGET(12,6)) && this->m_Type!=ITEMGET(12,42) && this->m_Type!=ITEMGET(12,41) || Type == 13 || Type == 15 )
	{
		Gold = Level2*Level2*Level2+100;

		for (int u=0;u<this->m_SpecialNum;u++)
		{
			switch ( this->m_Special[u] )
			{
				case 0x55:
					Gold += Gold*this->m_Z28Option;
			}
		}
	}
	else
	{
		switch ( this->m_Level )
		{
			case  5:	Level2 +=   4;	break;
			case  6:	Level2 +=  10;	break;
			case  7:	Level2 +=  25;	break;
			case  8:	Level2 +=  45;	break;
			case  9:	Level2 +=  65;	break;
			case 10:	Level2 +=  95;	break;
			case 11:	Level2 += 135;	break;
			case 12:	Level2 += 185;	break;
			case 13:	Level2 += 245;	break;
		}

		if ( Type == 12 && (this->m_Type <= ITEMGET(12,6) || this->m_Type == ITEMGET(12,42) || this->m_Type == ITEMGET(12,41)) )	// Wings
		{
			Gold = (Level2+40)*Level2*Level2*11+40000000;
		}
		else
		{
			Gold = (Level2+40)*Level2*Level2/8+100;
		}

		if ( Type >= 0 && Type <=  6 )
		{
			if ( p->TwoHand == 0 )
			{
				Gold = Gold*80/100;
			}
		}

		for ( int g=0;g<this->m_SpecialNum;g++)
		{
			switch ( this->m_Special[g] )
			{
				case 18:
				case 19:
				case 20:
				case 21:
				case 22:
				case 23:
				case 24:
				case 56:
					Gold += (__int64)(Gold * 1.5);
					break;

				case 80:
				case 81:
				case 83:
				case 85:

					switch ( this->m_Z28Option )
					{
						case 1:
							Gold += (__int64)(Gold * 6.0 / 10.0);
							break;

						case 2:
							Gold += (__int64)(Gold * 14.0 / 10.0);
							break;

						case 3:
							Gold += (__int64)(Gold * 28.0 / 10.0);
							break;

						case 4:
							Gold += (__int64)(Gold * 56.0 / 10.0);
							break;

						case 5:
							Gold += (__int64)(Gold * 112.0 / 10.0);
							break;

						case 6:
							Gold += (__int64)(Gold * 224.0 / 10.0);
							break;

						case 7:
							Gold += (__int64)(Gold * 448.0 / 10.0);
							break;
					}
					break;

				case 82:

					switch ( this->m_Z28Option )
					{
						case 1:
							Gold += (__int64)(Gold * 6.0 / 10.0);
							break;

						case 2:
							Gold += (__int64)(Gold * 14.0 / 10.0);
							break;

						case 3:
							Gold += (__int64)(Gold * 28.0 / 10.0);
							break;

						case 4:
							Gold += (__int64)(Gold * 56.0 / 10.0);
							break;

						case 5:
							Gold += (__int64)(Gold * 112.0 / 10.0);
							break;

						case 6:
							Gold += (__int64)(Gold * 224.0 / 10.0);
							break;

						case 7:
							Gold += (__int64)(Gold * 448.0 / 10.0);
							break;
					}
					break;

				case 84:
					Gold += (__int64)(Gold * 25.0 / 100.0);
					break;

				case 86:
				case 87:
				case 88:
				case 89:
				case 90:
				case 91:
				case 92:
				case 93:
				case 94:
				case 95:
				case 96:
				case 97:
				case 98:
				case 99:
					Gold += Gold;
					break;

				case 100:
				case 101:
				case 102:
				case 103:
				case 104:
					Gold += (__int64)(Gold * 25.0 / 100.0);
					break;

			}
		}
	}

	if ( g_kItemSystemFor380.Is380OptionItem(this) == true )
	{
		Gold += (__int64)(Gold * 16.0 / 100.0);
	}

	if ( Gold > 3000000000 )
	{
		Gold = 3000000000;
	}

	this->m_BuyMoney = (DWORD)Gold;
	this->m_SellMoney = (DWORD)Gold;
	
	this->m_SellMoney = this->m_SellMoney/3;

	//if ( (this->m_Type < ITEMGET(14,0) || this->m_Type >  ITEMGET(14,8)) &&
	//	  this->m_Type != ITEMGET(13,20) &&
	//	  this->m_Type != ITEMGET(14,28) &&
	//	  this->m_Type != ITEMGET(14,29) &&
	//	  this->m_Type != ITEMGET(14,21) &&
	//	  this->m_Type != ITEMGET(13,15) &&
	//	  this->m_Type != ITEMGET(13,16) &&
	//	  this->m_Type != ITEMGET(13,17) &&
	//	  this->m_Type != ITEMGET(13,18) &&
	//	  this->m_Type != ITEMGET(13,11) &&
	//	  this->m_Type != ITEMGET(13,7)  &&
	//	  this->m_Type != ITEMGET(13,32) &&
	//	  this->m_Type != ITEMGET(13,33) &&
	//	  this->m_Type != ITEMGET(13,34) &&
	//	  this->m_Type != ITEMGET(13,35) &&
	//	  this->m_Type != ITEMGET(13,36) &&
	//	  this->m_Type != ITEMGET(13,37) &&
	//	  this->m_Type != ITEMGET(14,45) &&
	//	  this->m_Type != ITEMGET(14,46) &&
	//	  this->m_Type != ITEMGET(14,47) &&
	//	  this->m_Type != ITEMGET(14,48) &&
	//	  this->m_Type != ITEMGET(14,49) &&
	//	  this->m_Type != ITEMGET(14,50) )
	//{
	//	float lc15 = 1.0f-(this->m_Durability / this->m_BaseDurability);
	//	int lc16 = this->m_SellMoney*0.6*lc15;
	//	this->m_SellMoney -= lc16;
	//}

	if ( this->m_SellMoney >= 1000 )
	{
		this->m_SellMoney = this->m_SellMoney/100*100;
	}
	else if ( this->m_SellMoney >= 100 )
	{
		this->m_SellMoney = this->m_SellMoney/10*10;
	}

	if ( this->m_BuyMoney >= 1000 )
	{
		this->m_BuyMoney = this->m_BuyMoney/100*100;
	}
	else if ( this->m_BuyMoney >= 100 )
	{
		this->m_BuyMoney = this->m_BuyMoney/10*10;
	}
}

void CItem::OldValue()
{
	if ( this->m_Type == -1 )
	{
		return;
	}

	int Gold = 0;

	if ( this->m_Type == ITEMGET(14,13) ) // Bless
	{
		Gold = 100000;
	}
	else if ( this->m_Type == ITEMGET(14,14) ) // Soul
	{
		Gold = 70000;
	}
	else if ( this->m_Type == ITEMGET(12,15) ) // Chaos
	{
		Gold = 40000;
	}
	else if ( this->m_Type == ITEMGET(14,22) ) // Creation
	{
		Gold = 450000;
	}
	else if ( this->m_Type == ITEMGET(14,16) ) // Life
	{
		Gold = 450000;
	}
	else
	{
		this->m_OldBuyMoney = this->m_BuyMoney;
		return;
	}

	this->m_OldBuyMoney = Gold;

	if ( this->m_OldBuyMoney >= 1000 )
	{
		this->m_OldBuyMoney = this->m_OldBuyMoney /100*100;
	}
	else if ( this->m_OldBuyMoney >= 100 )
	{
		this->m_OldBuyMoney = this->m_OldBuyMoney / 10 * 10;
	}
}

BOOL CItem::GetSize(int & w, int & h)
{
	w=ItemAttribute[this->m_Type].Width  ;
	h=ItemAttribute[this->m_Type].Height  ;

	return 1;
}


int CItem::ItemDamageMin()
{
	int damagemin;
	
	if (this->m_Type < 0)
	{
		return 0;
	}
	else
	{
		damagemin=this->m_DamageMinOrigin;
		this->PlusSpecial(&damagemin, 80);
		return damagemin;
	}
}


int CItem::ItemDefense()
{
	int Defense;
	
	if (this->m_Type < 0)
	{
		return 0;
	}
	else
	{
		if (this->m_IsValidItem == false)
		{
			return 0;
		}
		else
		{
			Defense=this->m_Defense;
			this->PlusSpecial(&Defense, 83);
			return Defense;
		}
	}
}






BOOL CItem::IsClass(char aClass, int ChangeUP)
{
	int addChangeUp = 0;
	if ((aClass < 0) || (aClass >= MAX_TYPE_PLAYER ))
	{
		LogAdd("Wrong Class:%d (%s %d)", aClass, __FILE__, __LINE__); // 클래스 에러 : %d (%s %d)
		return 0;
	}
	
	int requireclass=this->m_RequireClass[aClass];
	if (requireclass == 0)
	{
		return 0;
	}

	if (ChangeUP > 0)
	{
		if ((aClass == CLASS_DARKLORD) || (aClass == CLASS_MAGICGLADIATOR) || (aClass == CLASS_RAGEFIGHTER))
			addChangeUp = 2;
		else
			addChangeUp = 1;
	}

	if (requireclass > 1)
	{
		if ( requireclass > (ChangeUP+addChangeUp) )
		{
			return 0;
		}
	}
	return 1;
}

LPSTR CItem::GetName()
{
	return &ItemAttribute[this->m_Type].Name[0];
}



void CItem::PlusSpecialSetRing(BYTE * Value)
{
	if ( this->m_Type < 0 )
	{
		return;
	}

	if ( this->IsSetItem() == FALSE )
	{
		return;
	}

	if ( this->m_Durability == 0.0f)
	{
		return;
	}

	if ( this->m_IsValidItem == false )
	{
		return;
	}

	LPITEM_ATTRIBUTE p = &ItemAttribute[this->m_Type];

	if ( p->ResistanceType == -1 )
	{
		return;
	}

	BYTE iValue = 5;
	Value[p->ResistanceType] += iValue - (BYTE)( iValue * this->m_CurrentDurabilityState);

}



void CItem::PlusSpecial(int * Value, int Special)
{
	if ( this->m_Type < 0 )
	{
		return;
	}

	if ( this->m_Durability == 0.0f)
	{
		return;
	}

	if ( this->m_IsValidItem == false )
	{
		return;
	}

	for (int i =0;i<this->m_SpecialNum;i++)
	{
		if ( this->m_Special[i] == Special )
		{
			switch ( Special )
			{
				case 82:
					{
						int lc3 = this->m_Z28Option*5;
						*Value += lc3 - (int)( lc3 * this->m_CurrentDurabilityState );
					}
					break;

				case 84:
					* Value += 4;
					break;

				case 81:
				case 80:
				case 83:
					{
						int lc4 = this->m_Z28Option*4;
						*Value += lc4 - (int)( lc4 * this->m_CurrentDurabilityState );
					}
					break;

				case 100:
				case 101:
					*Value =  *Value + this->m_Level * 5 + 50;
					break;

				case 103:
					*Value += 50;
					break;

				case 105:
					*Value =  *Value + this->m_Level * 5 + 10;

					break;

				default:
					*Value = *Value + (this->m_Z28Option*4);
					break;
			}
		}
	}
}

void CItem::PlusSpecialPercent(int * Value, int Special, WORD Percent)
{
	if ( this->m_Type < 0 )
	{
		return;
	}

	for (int i=0;i<this->m_SpecialNum;i++)
	{
		if ( this->m_Special[i] == Special )
		{
			*Value += (*Value * Percent)/ 100;
		}
	}
}



void CItem::PlusSpecialPercentEx(int * Value, int SourceValue, int Special)
{
	if ( this->m_Type < 0 )
	{
		return;
	}

	if ( this->m_Durability == 0.0f )
	{
		return;
	}

	if ( this->m_IsValidItem == false  )
	{
		return;
	}

	for ( int i =0;i<this->m_SpecialNum;i++)
	{
		if ( this->m_Special[i] == Special )
		{
			int lc3 = SourceValue * this->m_Z28Option/100;
			*Value += lc3 - (int)(lc3 * this->m_CurrentDurabilityState );
		}
	}
}

void CItem::SetItemPlusSpecialStat(short * Value, int Special)
{
	if ( this->m_Type < 0 )
	{
		return;
	}

	if ( this->m_Durability == 0.0f )
	{
		return;
	}

	for ( int i =0;i<this->m_SpecialNum;i++)
	{
		if ( this->m_Special[i] == Special )
		{
			switch ( Special )
			{
				case 0xC4:
				case 0xC5:
				case 0xC6:
				case 0xC7:
					{
						int lc3 = this->m_SetAddStat;
						*Value += lc3 - (int)(lc3 * this->m_CurrentDurabilityState);
					}
					break;

				default:
					*Value += this->m_Z28Option*3;
			}
		}
	}
}

int CItem::GetAddStatType()
{
	LPITEM_ATTRIBUTE p;
	p = &ItemAttribute[this->m_Type];
	return p->SetAttr;
}

int CItem::GetWeaponType()
{
	if ( this->m_Type <= ITEMGET(5,0) )
	{
		return 1;
	}

	if ( this->m_Type <= ITEMGET(6,0) )
	{
		return 2;
	}
	return 0;
}

void CItem::SetSocketItemInfo(BYTE ItemSocket1, BYTE ItemSocket2, BYTE ItemSocket3, BYTE ItemSocket4, BYTE ItemSocket5)
{
	if ( IsSlotItem(this->m_Type) )
	{
		this->m_ItemSlot1 = ItemSocket1;
		this->m_ItemSlot2 = ItemSocket2;
		this->m_ItemSlot3 = ItemSocket3;
		this->m_ItemSlot4 = ItemSocket4;
		this->m_ItemSlot5 = ItemSocket5;
	}else
	{
		this->m_ItemSlot1 = 0;
		this->m_ItemSlot2 = 0;
		this->m_ItemSlot3 = 0;
		this->m_ItemSlot4 = 0;
		this->m_ItemSlot5 = 0;
	}
}

void CItem::SetPetItemInfo(int petlevel, int petexp)
{
	if ( this->m_Type == ITEMGET(13,4) )
	{
		this->m_PetItem_Level = petlevel;
		this->m_PetItem_Exp = petexp;

		if ( this->m_PetItem_Exp < gPetItemExp.m_DarkHorseExpTable[this->m_PetItem_Level] || this->m_PetItem_Exp >= gPetItemExp.m_DarkHorseExpTable[1+this->m_PetItem_Level] )
		{
			this->m_PetItem_Exp = gPetItemExp.m_DarkHorseExpTable[this->m_PetItem_Level];
		}

		this->Convert(this->m_Type, this->m_SkillOption, this->m_LuckOption,
			this->m_Z28Option, this->m_NewOption, this->m_SetOption, this->m_ItemOptionEx, CURRENT_DB_VERSION);
		this->PetValue();
		this->m_IsLoadPetItemInfo = 1;
	}
	else if ( this->m_Type == ITEMGET(13,5) )
	{
		this->m_PetItem_Level = petlevel;
		this->m_PetItem_Exp = petexp;

		if ( this->m_PetItem_Exp < gPetItemExp.m_DarkSpiritExpTable[this->m_PetItem_Level] || this->m_PetItem_Exp >= gPetItemExp.m_DarkSpiritExpTable[1+this->m_PetItem_Level] )
		{
			this->m_PetItem_Exp = gPetItemExp.m_DarkSpiritExpTable[this->m_PetItem_Level];
		}

		this->Convert(this->m_Type, this->m_SkillOption, this->m_LuckOption,
			this->m_Z28Option, this->m_NewOption, this->m_SetOption, this->m_ItemOptionEx, CURRENT_DB_VERSION);

		this->PetValue();
		this->m_IsLoadPetItemInfo = 1;
	}
}




BOOL CItem::AddPetItemExp(int aIndex,int petexp)
{
	if ( this->m_Durability <= 0.0f )
	{
		return FALSE;
	}

	if ( this->m_IsLoadPetItemInfo == 0 )
	{
		return FALSE;
	}

	if ( this->m_PetItem_Level < 1 )
	{
		this->m_PetItem_Level = 1;
		this->m_PetItem_Exp = 0;
		this->Convert(this->m_Type, this->m_SkillOption, this->m_LuckOption,
			this->m_Z28Option, this->m_NewOption, this->m_SetOption, this->m_ItemOptionEx, CURRENT_DB_VERSION);
		this->PetValue();

		return FALSE;
	}

	if ( this->m_Type == ITEMGET(13,4) )
	{
		if ( this->m_PetItem_Level >= MAX_PET_LEVEL )
		{
			return FALSE;
		}

		bool ret = false;
		int maxexp = gPetItemExp.m_DarkHorseExpTable[1+this->m_PetItem_Level];
		this->m_PetItem_Exp += petexp;

		while ( this->m_PetItem_Exp >= maxexp )
		{
			this->m_PetItem_Level++;
			maxexp = gPetItemExp.m_DarkHorseExpTable[1+this->m_PetItem_Level];
			ret = true;
		}

		if ( ret != FALSE )
		{
		this->Convert(this->m_Type, this->m_SkillOption, this->m_LuckOption,
			this->m_Z28Option, this->m_NewOption, this->m_SetOption, this->m_ItemOptionEx, CURRENT_DB_VERSION);
		this->PetValue();

		return true;
		}else
		{
			CDarkSpirit::SendExpmsg(aIndex, 8, 1, 0);
		}
	}
	else if ( this->m_Type == ITEMGET(13,5) )
	{
		if ( this->m_PetItem_Level >= MAX_PET_LEVEL )
		{
			return FALSE;
		}

		BOOL ret = FALSE;
		int maxexp = gPetItemExp.m_DarkSpiritExpTable[1+this->m_PetItem_Level];
		this->m_PetItem_Exp += petexp;

		while ( this->m_PetItem_Exp >= maxexp )
		{
			this->m_PetItem_Level++;
			maxexp = gPetItemExp.m_DarkSpiritExpTable[1+this->m_PetItem_Level];
			ret = true;
		}

		if ( ret != FALSE )
		{
		this->Convert(this->m_Type, this->m_SkillOption, this->m_LuckOption,
			this->m_Z28Option, this->m_NewOption, this->m_SetOption, this->m_ItemOptionEx, CURRENT_DB_VERSION);
		this->PetValue();

		return true;
		}else
		{
			CDarkSpirit::SendExpmsg(aIndex, 1, 0, 0);
		}
	}

	return FALSE;
}


BOOL CItem::DecPetItemExp(int percent)
{
	if ( this->m_IsLoadPetItemInfo == 0 )
	{
		return FALSE;
	}

	if ( this->m_PetItem_Level < 1 )
	{
		this->m_PetItem_Level = 1;
		this->m_PetItem_Exp = 0;
		this->Convert(this->m_Type, this->m_SkillOption, this->m_LuckOption,
			this->m_Z28Option, this->m_NewOption, this->m_SetOption, this->m_ItemOptionEx, CURRENT_DB_VERSION);
		this->PetValue();

		return FALSE;
	}

	if ( this->m_Type == ITEMGET(13,4) )
	{
		int exp = gPetItemExp.m_DarkHorseExpTable[1+this->m_PetItem_Level] - gPetItemExp.m_DarkHorseExpTable[this->m_PetItem_Level];
		exp = exp * percent/100;
		this->m_PetItem_Exp -= exp;

		if ( this->m_PetItem_Exp < 0 )
		{
			this->m_PetItem_Level = 1;
			this->m_PetItem_Exp = 0;
			this->Convert(this->m_Type, this->m_SkillOption, this->m_LuckOption,
				this->m_Z28Option, this->m_NewOption, this->m_SetOption, this->m_ItemOptionEx, CURRENT_DB_VERSION);
			this->PetValue();
		}
		else if ( this->m_PetItem_Exp < gPetItemExp.m_DarkHorseExpTable[this->m_PetItem_Level] )
		{
			this->m_PetItem_Level--;
			this->Convert(this->m_Type, this->m_SkillOption, this->m_LuckOption,
				this->m_Z28Option, this->m_NewOption, this->m_SetOption, this->m_ItemOptionEx, CURRENT_DB_VERSION);
			this->PetValue();
			
			return true;
		}
	}
	else if ( this->m_Type == ITEMGET(13,5) )
	{
		int exp = gPetItemExp.m_DarkSpiritExpTable[1+this->m_PetItem_Level] - gPetItemExp.m_DarkSpiritExpTable[this->m_PetItem_Level];
		exp = exp * percent/100;
		this->m_PetItem_Exp -= exp;

		if ( this->m_PetItem_Exp < 0 )
		{
			this->m_PetItem_Level = 1;
			this->m_PetItem_Exp = 0;
			this->Convert(this->m_Type, this->m_SkillOption, this->m_LuckOption,
				this->m_Z28Option, this->m_NewOption, this->m_SetOption, this->m_ItemOptionEx, CURRENT_DB_VERSION);
			this->PetValue();
		}
		else if ( this->m_PetItem_Exp < gPetItemExp.m_DarkSpiritExpTable[this->m_PetItem_Level] )
		{
			this->m_PetItem_Level--;
			this->Convert(this->m_Type, this->m_SkillOption, this->m_LuckOption,
				this->m_Z28Option, this->m_NewOption, this->m_SetOption, this->m_ItemOptionEx, CURRENT_DB_VERSION);
			this->PetValue();
			
			return true;
		}
	}

	return FALSE;
}


BOOL CItem::PetItemLevelDown(int exp)
{
	if ( this->m_IsLoadPetItemInfo == FALSE )
	{
		return FALSE;
	}

	if ( this->m_PetItem_Level < 2 )
	{
		return FALSE;
	}

	this->m_PetItem_Exp -= exp;
	this->m_PetItem_Level--;

	if ( this->m_Type == ITEMGET(13,4) )
	{
		int petlevel = this->m_PetItem_Level;
		int petexp = gPetItemExp.m_DarkHorseExpTable[petlevel];

		while ( this->m_PetItem_Exp <= petexp )
		{
			if ( petlevel <= 1 )
			{
				break;
			}

			petlevel--;
			petexp = gPetItemExp.m_DarkHorseExpTable[petlevel];
		}

		this->m_PetItem_Level = petlevel;
	}
	else if ( this->m_Type == ITEMGET(13,5) )
	{
		int petlevel = this->m_PetItem_Level;
		int petexp = gPetItemExp.m_DarkSpiritExpTable[petlevel];

		while ( this->m_PetItem_Exp <= petexp )
		{
			if ( petlevel <= 1 )
			{
				break;
			}

			petlevel--;
			petexp = gPetItemExp.m_DarkSpiritExpTable[petlevel];
		}

		this->m_PetItem_Level = petlevel;
	}

	this->Convert(this->m_Type, this->m_SkillOption, this->m_LuckOption,
		this->m_Z28Option, this->m_NewOption, this->m_SetOption, this->m_ItemOptionEx, CURRENT_DB_VERSION);
	this->PetValue();

	return true;
}




void CItem::PetValue()
{
	if ( this->m_Type == ITEMGET(13,4) )
	{
		this->m_SellMoney = this->m_PetItem_Level * 2000000;
	}
	else if ( this->m_Type == ITEMGET(13,5) )
	{
		this->m_SellMoney = this->m_PetItem_Level * 1000000;
	}	

	this->m_BuyMoney = this->m_SellMoney;
	this->m_SellMoney /= 3;

	if ( this->m_SellMoney >= 1000 )
	{
		this->m_SellMoney = this->m_SellMoney / 100 * 100;
	}
	else if ( this->m_SellMoney >= 100 )
	{
		this->m_SellMoney = this->m_SellMoney / 10 * 10;
	}

	if ( this->m_BuyMoney >= 1000 )
	{
		this->m_BuyMoney = this->m_BuyMoney / 100 * 100;
	}
	else if ( this->m_BuyMoney >= 100 )
	{
		this->m_BuyMoney = this->m_BuyMoney / 10 * 10;
	}
}



BOOL CItem::IsExtItem()
{
	if (( this->m_Type == ITEMGET(13,37) ) || ( this->m_Type == ITEMGET(5,21) ) || ( this->m_Type == ITEMGET(5,22) ) || ( this->m_Type == ITEMGET(5,23) ))	//Fenrir
	{
		return FALSE;
	}

	if ( (this->m_NewOption & 0x7F)!= 0 )
	{
		return true;
	}

	return FALSE;
}

BOOL CItem::IsExtLifeAdd()
{
	if (( this->m_Type >= ITEMGET(12,36) && this->m_Type <= ITEMGET(12,40) ) || (this->m_Type == ITEMGET(12,43)) || (this->m_Type == ITEMGET(12,50)) // Third Wings Prevent Wings Level 2 Attributes
#if (CRYSTAL_EDITION == 1)
			|| (this->m_Type >= ITEMGET(12,200) && this->m_Type <= ITEMGET(12,254))
#endif
		)
	{
		return 0;
	}
	if ( (this->m_NewOption & 0x20) == 32 )
	{
		return 4;
	}

	return 0;
}

BOOL CItem::IsExtManaAdd()
{
	if (( this->m_Type >= ITEMGET(12,36) && this->m_Type <= ITEMGET(12,40) ) || (this->m_Type == ITEMGET(12,43)) || (this->m_Type == ITEMGET(12,50)) // Third Wings Prevent Wings Level 2 Attributes
#if (CRYSTAL_EDITION == 1)
			|| (this->m_Type >= ITEMGET(12,200) && this->m_Type <= ITEMGET(12,254))
#endif
		)
	{
		return 0;
	}
	if ( (this->m_NewOption & 0x10) == 16 )
	{
		return 4;
	}

	return 0;
}


BOOL CItem::IsExtDamageMinus()
{
	if ( (this->m_NewOption & 8) == 8 )
	{
		return 4;
	}

	return 0;
}


BOOL CItem::IsExtDamageReflect()
{
	if ( (this->m_NewOption & 4) == 4 )
	{
		return 5;
	}

	return 0;
}


BOOL CItem::IsExtDefenseSuccessfull()
{
	if (( this->m_Type >= ITEMGET(12,36) && this->m_Type <= ITEMGET(12,40) ) || (this->m_Type == ITEMGET(12,43)) || (this->m_Type == ITEMGET(12,50)) // Third Wings Prevent Wings Level 2 Attributes
#if (CRYSTAL_EDITION == 1)
			|| (this->m_Type >= ITEMGET(12,200) && this->m_Type <= ITEMGET(12,254))
#endif
		)
	{
		return 0;
	}
	if ( (this->m_NewOption & 2) == 2 )
	{
		return 10;
	}

	return 0;
}

BOOL CItem::IsExtMonsterMoney()
{
	if ( (this->m_NewOption & 1) == 1 )
	{
		return 40;
	}

	return 0;
}



BOOL CItem::IsExtExcellentDamage()
{
	if ( (this->m_Type >= ITEMGET(6,0) ) && (this->m_Type < ITEMGET(7,0) ) )
	{
		return 0;
	}

	if ( ( this->m_NewOption & 0x20 ) == 0x20 )
	{
		return 10;
	}
	return 0;
}

BOOL CItem::IsExtAttackRate()
{
	if ( (this->m_NewOption & 0x10) == 16 )
	{
		return 1;
	}

	return 0;
}

BOOL CItem::IsExtAttackRate2()
{
	if ( (this->m_NewOption & 8) == 8 )
	{
		return 2;
	}

	return 0;
}

BOOL CItem::IsExtAttackSpeed()
{
	if (( this->m_Type >= ITEMGET(12,36) && this->m_Type <= ITEMGET(12,40) ) || (this->m_Type == ITEMGET(12,43)) || (this->m_Type == ITEMGET(12,50)) // Third Wings Prevent Wings Level 2 Attributes
#if (CRYSTAL_EDITION == 1)
			|| (this->m_Type >= ITEMGET(12,200) && this->m_Type <= ITEMGET(12,254))
#endif
		)
	{
		return 0;
	}
	if ( (this->m_NewOption & 4) == 4 )
	{
		return 7;
	}

	return 0;
}


BOOL CItem::IsExtMonsterDieLife()
{
	if ( (this->m_NewOption & 2) == 2 )
	{
		return 1;
	}

	return 0;
}


BOOL CItem::IsExtMonsterDieMana()
{
	if ( this->m_Type >= ITEMGET(6,0) && this->m_Type < ITEMGET(7,0) )
	{
		return 0;
	}

	if ( (this->m_NewOption & 1) == 1 )
	{
		return 1;
	}
	return 0;
}



BOOL CItem::IsWingOpGetOnePercentDamage()
{
	if (( this->m_Type >= ITEMGET(12,36) && this->m_Type <= ITEMGET(12,40) ) || 
		( this->m_Type == ITEMGET(12,43)) || 
		( this->m_Type == ITEMGET(12,50)) // Third Wings Prevent Wings Level 2 Attributes
#if (CRYSTAL_EDITION == 1)
			|| (this->m_Type >= ITEMGET(12,200) && this->m_Type <= ITEMGET(12,254))
#endif
		)
	{
		return 0;
	}

	if ( this->m_Durability  < 1.0f )
	{
		return 0;
	}

	if ( (this->m_NewOption & 4)!= 0 )
	{
		return 3;
	}
	return 0;
}


BOOL CItem::IsWingOpGetManaToMoster()
{
	if ( this->m_Durability < 1.0f )
	{
		return 0;
	}

	if ( (this->m_NewOption & 16 ) == 1 )
	{
		return 10;
	}
	return 0;
}

BOOL CItem::IsDinorantReduceAttackDamaege()
{
	if ( this->m_Durability  < 1.0f )
	{
		return 0;
	}

	if ( (this->m_NewOption & 1 ) == 1 )
	{
		return 5;
	}
	return 0;
}


int CItem::IsFenrirIncLastAttackDamage()
{
	if ( this->m_Durability < 1.0f )
	{
		return 0;
	}

	if ( (this->m_NewOption&1) == 1 )
	{
		return 10;
	}

	return 0;
}

int CItem::IsFenrirDecLastAttackDamage()
{
	if ( this->m_Durability < 1.0f )
	{
		return 0;
	}

	if ( (this->m_NewOption&2) == 2 )
	{
		return 10;
	}

	return 0;
}

int CItem::IsFenrirIllusion()
{
	if ( this->m_Durability < 1.0f )
	{
		return 0;
	}

	if ( (this->m_NewOption&4) == 4 )
	{
		return 10;
	}

	return 0;
}


int CItem::SimpleDurabilityDown(int iDecValue)
{
	if ( this->m_Type == -1 )
	{
		return 0;
	}
	if ( this->m_Durability <= 0 )
	{
		return 0;
	}

#if(ENABLE_CSHOP == 1)
	if ( MuItemShop.IsCShopItemByType(this->m_Type, this->m_ItemOptionEx) == true )
	{
		return 0;
	}
#endif
	if(this->GetNumber() == 0xFFFFFFFF)
	{
		this->m_Durability -= (1.0f * ReadConfig.IncreaseFFFFFFFFSerialDurDown);
	} 
	else if(this->GetNumber() == 0xFFFFFFFE)
	{
		this->m_Durability -= (1.0f * ReadConfig.IncreaseFFFFFFFESerialDurDown);
	} 
	else if(this->GetNumber() == 0xFFFFFFFD)
	{
		this->m_Durability -= (1.0f * ReadConfig.IncreaseFFFFFFFDSerialDurDown);
	} 
	else if(this->GetNumber() == 0xFFFFFFFC)
	{
		this->m_Durability -= (1.0f * ReadConfig.IncreaseFFFFFFFCSerialDurDown);
	} 
	else 
	{
		this->m_Durability -= 1.0f ;
	}

	if ( this->m_Durability <= 0 )
	{
		this->m_Durability = 0.0f;
		return 0;
	}

	if ( this->CheckDurabilityState() != 0 )
	{
		return 2;
	}

	return this->m_Durability;
}


int CItem::DurabilityDown(int dur, int iDecValue)
{
	if ( this->m_Type == -1 )
	{
		return 0;
	}
	if ( this->m_Durability <= 0 )
	{
		return 0;
	}

#if(ENABLE_CSHOP == 1)
	if ( MuItemShop.IsCShopItemByType(this->m_Type, this->m_ItemOptionEx) == true )
	{
		return 0;
	}
#endif

	this->m_DurabilitySmall += dur;
	
	if ( this->m_DurabilitySmall > 564 )
	{
		this->m_DurabilitySmall = 0;
		if(this->GetNumber() == 0xFFFFFFFF)
		{
			this->m_Durability -= (1.0f * ReadConfig.IncreaseFFFFFFFFSerialDurDown);
		} 
		else if(this->GetNumber() == 0xFFFFFFFE)
		{
			this->m_Durability -= (1.0f * ReadConfig.IncreaseFFFFFFFESerialDurDown);
		} 
		else if(this->GetNumber() == 0xFFFFFFFD)
		{
			this->m_Durability -= (1.0f * ReadConfig.IncreaseFFFFFFFDSerialDurDown);
		} 
		else if(this->GetNumber() == 0xFFFFFFFC)
		{
			this->m_Durability -= (1.0f * ReadConfig.IncreaseFFFFFFFCSerialDurDown);
		} 
		else 
		{
			this->m_Durability -= 1.0f ;
		}

		if ( this ->m_Durability < 0.0f )
		{
			this->m_Durability = 0;
		}

		if ( this->CheckDurabilityState() != FALSE )
		{
			return 2;
		}
		return 1;
	}
	
	return this->m_Durability;
}


int CItem::DurabilityDown(int dur, int iDecValue, int Pos)
{
	if ( this->m_Type == -1 )
	{
		return 0;
	}
	if ( this->m_Durability <= 0 )
	{
		return 0;
	}

#if(ENABLE_CSHOP == 1)
	if ( MuItemShop.IsCShopItemByType(this->m_Type, this->m_ItemOptionEx) == true )
	{
		return 0;
	}
#endif

	this->m_DurabilitySmall += dur;
	
	if ( this->m_DurabilitySmall > 564 )
	{
		this->m_DurabilitySmall = 0;
		if(this->GetNumber() == 0xFFFFFFFF)
		{
			this->m_Durability -= (1.0f * ReadConfig.IncreaseFFFFFFFFSerialDurDown);
		} 
		else if(this->GetNumber() == 0xFFFFFFFE)
		{
			this->m_Durability -= (1.0f * ReadConfig.IncreaseFFFFFFFESerialDurDown);
		} 
		else if(this->GetNumber() == 0xFFFFFFFD)
		{
			this->m_Durability -= (1.0f * ReadConfig.IncreaseFFFFFFFDSerialDurDown);
		} 
		else if(this->GetNumber() == 0xFFFFFFFC)
		{
			this->m_Durability -= (1.0f * ReadConfig.IncreaseFFFFFFFCSerialDurDown);
		} 
		else 
		{			
			if(ReadConfig.S6E2 == 0)
				this->m_Durability -= 1.0f ;
			else
			{
				if(Pos >= 9 && Pos <= 11)
				{
					if(gObjIsNewClass(&gObj[iDecValue]) == 1 && gObj[iDecValue].MasterCharacterInfo->Repair[1] > 0)
						this->m_Durability -= (100 - gObj[iDecValue].MasterCharacterInfo->Repair[1])/100.0f;
					else
						this->m_Durability -= 1.0f ;
				}else
				{
					this->m_Durability -= 1.0f ;
				}
			}
		}

		if ( this ->m_Durability < 0.0f )
		{
			this->m_Durability = 0;
		}

		if ( this->CheckDurabilityState() != FALSE )
		{
			return 2;
		}
		return 1;
	}
	
	return this->m_Durability;
}




int CItem::DurabilityDown2(int dur, int iDecValue)
{
	if ( this->m_Type == -1 )
	{
		return 0;
	}
	if ( this->m_Durability <= 0 )
	{
		return 0;
	}

#if(ENABLE_CSHOP == 1)
	if ( MuItemShop.IsCShopItemByType(this->m_Type, this->m_ItemOptionEx) == true )
	{
		return 0;
	}
#endif

	this->m_DurabilitySmall += dur;
	
	if ( this->m_DurabilitySmall > 1000 )
	{
		this->m_DurabilitySmall = 0;
		if(this->GetNumber() == 0xFFFFFFFF)
		{
			this->m_Durability -= (1.0f * ReadConfig.IncreaseFFFFFFFFSerialDurDown);
		} 
		else if(this->GetNumber() == 0xFFFFFFFE)
		{
			this->m_Durability -= (1.0f * ReadConfig.IncreaseFFFFFFFESerialDurDown);
		} 
		else if(this->GetNumber() == 0xFFFFFFFD)
		{
			this->m_Durability -= (1.0f * ReadConfig.IncreaseFFFFFFFDSerialDurDown);
		} 
		else if(this->GetNumber() == 0xFFFFFFFC)
		{
			this->m_Durability -= (1.0f * ReadConfig.IncreaseFFFFFFFCSerialDurDown);
		} 
		else 
		{
			this->m_Durability -= 1.0f ;
		}

		if ( this->m_Durability < 0.0f )
		{
			this->m_Durability = 0;
		}

		if ( this->CheckDurabilityState() != FALSE )
		{
			return 2;
		}
		return 1;
	}
	
	return 0;
}




int CItem::NormalWeaponDurabilityDown(int defense, int aIndex)
{
	if ( this->m_Type == -1 )
	{
		return 0;
	}
	if ( this->m_Durability <= 0 )
	{
		return 0;
	}

#if(ENABLE_CSHOP == 1)
	if ( MuItemShop.IsCShopItemByType(this->m_Type, this->m_ItemOptionEx) == true )
	{
		return 0;
	}
#endif

	int damagemin = this->m_DamageMinOrigin;
	int plusdamage = 0;
	this->PlusSpecial(&plusdamage, 80);

	if ( damagemin == 0 )
	{
		LogAdd("damagemin이 0이다");
		return 0;
	}

	int div = damagemin + damagemin/2 + plusdamage;

	if ( div == 0 )
	{
		return 0;
	}

	int DecreaseDur = (defense*2)/div;
	this->m_DurabilitySmall += DecreaseDur;

	if ( this->m_DurabilitySmall > 564 )
	{
		this->m_DurabilitySmall = 0;
		if(this->GetNumber() == 0xFFFFFFFF)
		{
			this->m_Durability -= (1.0f * ReadConfig.IncreaseFFFFFFFFSerialDurDown);
		} 
		else if(this->GetNumber() == 0xFFFFFFFE)
		{
			this->m_Durability -= (1.0f * ReadConfig.IncreaseFFFFFFFESerialDurDown);
		} 
		else if(this->GetNumber() == 0xFFFFFFFD)
		{
			this->m_Durability -= (1.0f * ReadConfig.IncreaseFFFFFFFDSerialDurDown);
		} 
		else if(this->GetNumber() == 0xFFFFFFFC)
		{
			this->m_Durability -= (1.0f * ReadConfig.IncreaseFFFFFFFCSerialDurDown);
		} 
		else 
		{
			if(ReadConfig.S6E2 == 0)
				this->m_Durability -= 1.0f ;
			else
			{
				if(gObjIsNewClass(&gObj[aIndex]) == 1 && gObj[aIndex].MasterCharacterInfo->Repair[0] > 0)
					this->m_Durability -= (100 - gObj[aIndex].MasterCharacterInfo->Repair[0])/100.0f;
				else
					this->m_Durability -= 1.0f ;				
			}
		}

		if ( this->m_Durability < 0.0f )
		{
			this->m_Durability = 0;
		}

		if ( this->CheckDurabilityState() != 0 )
		{
			return 2;
		}
		
		return 1;
		
	}
	return 0;
}



int CItem::BowWeaponDurabilityDown(int defense, int aIndex)
{
	if ( this->m_Type == -1 )
	{
		return 0;
	}
	if ( this->m_Durability <= 0 )
	{
		return 0;
	}

#if(ENABLE_CSHOP == 1)
	if ( MuItemShop.IsCShopItemByType(this->m_Type, this->m_ItemOptionEx) == true )
	{
		return 0;
	}
#endif

	int damagemin = this->m_DamageMinOrigin;
	int plusdamage = 0;
	this->PlusSpecial(&plusdamage, 80);
	
	int div = damagemin + damagemin/2 + plusdamage;

	if ( div == 0 )
	{
		return 0;
	}

	int DecreaseDur = (defense*2)/div;
	this->m_DurabilitySmall += DecreaseDur;

	if ( this->m_DurabilitySmall > 780 )
	{
		this->m_DurabilitySmall = 0;
		if(this->GetNumber() == 0xFFFFFFFF)
		{
			this->m_Durability -= (1.0f * ReadConfig.IncreaseFFFFFFFFSerialDurDown);
		}
		else if(this->GetNumber() == 0xFFFFFFFE)
		{
			this->m_Durability -= (1.0f * ReadConfig.IncreaseFFFFFFFESerialDurDown);
		} 
		else if(this->GetNumber() == 0xFFFFFFFD)
		{
			this->m_Durability -= (1.0f * ReadConfig.IncreaseFFFFFFFDSerialDurDown);
		} 
		else if(this->GetNumber() == 0xFFFFFFFC)
		{
			this->m_Durability -= (1.0f * ReadConfig.IncreaseFFFFFFFCSerialDurDown);
		} 
		else 
		{
			if(ReadConfig.S6E2 == 0)
				this->m_Durability -= 1.0f ;
			else
			{
				if(gObjIsNewClass(&gObj[aIndex]) == 1 && gObj[aIndex].MasterCharacterInfo->Repair[0] > 0)
					this->m_Durability -= (100 - gObj[aIndex].MasterCharacterInfo->Repair[0])/100.0f;
				else
					this->m_Durability -= 1.0f ;				
			}
		}

		if ( this->m_Durability < 0.0f )
		{
			this->m_Durability = 0;
		}
		
		if ( this->CheckDurabilityState() != 0 )
		{
			return 2;
		}
		
		return 1;
		
	}
	return 0;
}

int CItem::StaffWeaponDurabilityDown(int defence, int aIndex)
{
	if ( this->m_Type == -1 )
	{
		return 0;
	}
	if ( this->m_Durability <= 0 )
	{
		return 0;
	}
#if(ENABLE_CSHOP == 1)
	if ( MuItemShop.IsCShopItemByType(this->m_Type, this->m_ItemOptionEx) == true )
	{
		return 0;
	}
#endif

	int magic = this->m_Magic/2 + this->m_Level*2;
	int plusmagic = 0;
	this->PlusSpecial(&plusmagic, 81);
	int div = magic + magic/3 + plusmagic;

	if ( div == 0 )
	{
		return 0;
	}

	int DecreaseDur = defence/div;
	this->m_DurabilitySmall += DecreaseDur;

	if ( this->m_DurabilitySmall > 1050 )
	{
		this->m_DurabilitySmall = 0;
		if(this->GetNumber() == 0xFFFFFFFF)
		{
			this->m_Durability -= (1.0f * ReadConfig.IncreaseFFFFFFFFSerialDurDown);
		} 
		else if(this->GetNumber() == 0xFFFFFFFE)
		{
			this->m_Durability -= (1.0f * ReadConfig.IncreaseFFFFFFFESerialDurDown);
		} 
		else if(this->GetNumber() == 0xFFFFFFFD)
		{
			this->m_Durability -= (1.0f * ReadConfig.IncreaseFFFFFFFDSerialDurDown);
		} 
		else if(this->GetNumber() == 0xFFFFFFFC)
		{
			this->m_Durability -= (1.0f * ReadConfig.IncreaseFFFFFFFCSerialDurDown);
		} 
		else 
		{
			if(ReadConfig.S6E2 == 0)
				this->m_Durability -= 1.0f ;
			else
			{
				if(gObjIsNewClass(&gObj[aIndex]) == 1 && gObj[aIndex].MasterCharacterInfo->Repair[0] > 0)
					this->m_Durability -= (100 - gObj[aIndex].MasterCharacterInfo->Repair[0])/100.0f;
				else
					this->m_Durability -= 1.0f ;				
			}
		}

		if ( this->m_Durability < 0.0f )
		{
			this->m_Durability = 0;
		}
		
		if ( this->CheckDurabilityState() != 0 )
		{
			return 2;
		}
		return 1;
		
	}
	return 0;
}



int CItem::ArmorDurabilityDownNoCheck(int aIndex)
{
	if ( this->m_Durability <= 0 )
	{
		return 0;
	}

	if ( this->m_Type == -1 )
	{
		return 0;
	}
#if(ENABLE_CSHOP == 1)
	if ( MuItemShop.IsCShopItemByType(this->m_Type, this->m_ItemOptionEx) == true )
	{
		return 0;
	}
#endif

	int def = this->m_DefenseOrigin;
	int plusdef = 0;
	int DecreaseDur = 0;

	if ( def == 0 )
	{
		LogAdd("def이 0이다");
		return 0;
	}

	if(this->GetNumber() == 0xFFFFFFFF)
	{
		this->m_Durability -= (1.0f * ReadConfig.IncreaseFFFFFFFFSerialDurDown);
	} 
	else if(this->GetNumber() == 0xFFFFFFFE)
	{
		this->m_Durability -= (1.0f * ReadConfig.IncreaseFFFFFFFESerialDurDown);
	} 
	else if(this->GetNumber() == 0xFFFFFFFD)
	{
		this->m_Durability -= (1.0f * ReadConfig.IncreaseFFFFFFFDSerialDurDown);
	} 
	else if(this->GetNumber() == 0xFFFFFFFC)
	{
		this->m_Durability -= (1.0f * ReadConfig.IncreaseFFFFFFFCSerialDurDown);
	} 
	else 
	{
		if(ReadConfig.S6E2 == 0)
			this->m_Durability -= 1.0f ;
		else
		{
			if(gObjIsNewClass(&gObj[aIndex]) == 1 && gObj[aIndex].MasterCharacterInfo->Repair[0] > 0)
				this->m_Durability -= (100 - gObj[aIndex].MasterCharacterInfo->Repair[0])/100.0f;
			else
				this->m_Durability -= 1.0f ;				
		}
	}

	if ( this->m_Durability < 0.0f )
	{
		this->m_Durability = 0;
	}
	
	if ( this->CheckDurabilityState() != 0 )
	{
		return 2;
	}

	return 1;
}



int CItem::ArmorDurabilityDown(int damagemin, int aIndex)
{
	if ( this->m_Durability <= 0 )
	{
		return 0;
	}

	if ( this->m_Type == -1 )
	{
		return 0;
	}
#if(ENABLE_CSHOP == 1)
	if ( MuItemShop.IsCShopItemByType(this->m_Type, this->m_ItemOptionEx) == true )
	{
		return 0;
	}
#endif

	int def = this->m_DefenseOrigin;
	int plusdef = 0;
	int DecreaseDur = 0;

	if ( def == 0 )
	{
		LogAdd("def이 0이다");
		return 0;
	}

	if ( this->m_Type >= ITEMGET(6,0) && this->m_Type < ITEMGET(7,0) )
	{
		this->PlusSpecial(&plusdef, 82);
		DecreaseDur = damagemin/(def * 5+ plusdef);
	}
	else if ( this->m_RequireClass[CLASS_WIZARD] != 0 ) // Dark Wize
	{
		this->PlusSpecial(&plusdef, 83);
		DecreaseDur = damagemin/(def * 3 + plusdef);
	}
	else if ( this->m_RequireClass[CLASS_KNIGHT] != 0 )	// Dark Knight
	{
		this->PlusSpecial(&plusdef, 83);
		DecreaseDur = damagemin/(def * 3 + plusdef);
	}
	else if ( this->m_RequireClass[CLASS_ELF] != 0 )	// Elf
	{
		this->PlusSpecial(&plusdef, 83);
		DecreaseDur = damagemin/(def * 2 + plusdef);
	}
	else if ( this->m_RequireClass[CLASS_MAGICGLADIATOR] != 0 )	// MAgic Gladiator ( aka Magunsa )
	{
		this->PlusSpecial(&plusdef, 83);
		DecreaseDur = damagemin/(def * 7 + plusdef);
	}
	else if ( this->m_RequireClass[CLASS_DARKLORD] != 0 )	// Dark Lord
	{
		this->PlusSpecial(&plusdef, 83);
		DecreaseDur = damagemin/(def * 6 + plusdef);
	}
	else if ( this->m_RequireClass[CLASS_SUMMONER] != 0 )	
	{
		this->PlusSpecial(&plusdef, 83);
		DecreaseDur = damagemin/(def * 3 + plusdef);
	}
	else if ( this->m_RequireClass[CLASS_RAGEFIGHTER] != 0 )	
	{
		this->PlusSpecial(&plusdef, 83);
		DecreaseDur = damagemin/(def * 7 + plusdef);
	}
	else 
	{
		this->PlusSpecial(&plusdef, 83);
		DecreaseDur = damagemin/(def * 3 + plusdef);
	}

	this->m_DurabilitySmall += DecreaseDur;

	if ( this->m_DurabilitySmall > 69 )
	{
		this->m_DurabilitySmall = 0;

		if(this->GetNumber() == 0xFFFFFFFF)
		{
			this->m_Durability -= (1.0f * ReadConfig.IncreaseFFFFFFFFSerialDurDown);
		} 
		else if(this->GetNumber() == 0xFFFFFFFE)
		{
			this->m_Durability -= (1.0f * ReadConfig.IncreaseFFFFFFFESerialDurDown);
		} 
		else if(this->GetNumber() == 0xFFFFFFFD)
		{
			this->m_Durability -= (1.0f * ReadConfig.IncreaseFFFFFFFDSerialDurDown);
		} 
		else if(this->GetNumber() == 0xFFFFFFFC)
		{
			this->m_Durability -= (1.0f * ReadConfig.IncreaseFFFFFFFCSerialDurDown);
		} 
		else 
		{
			if(ReadConfig.S6E2 == 0)
				this->m_Durability -= 1.0f ;
			else
			{
				if(gObjIsNewClass(&gObj[aIndex]) == 1 && gObj[aIndex].MasterCharacterInfo->Repair[0] > 0)
					this->m_Durability -= (100 - gObj[aIndex].MasterCharacterInfo->Repair[0])/100.0f;
				else
					this->m_Durability -= 1.0f ;				
			}
		}

		if ( this->m_Durability < 0.0f )
		{
			this->m_Durability = 0;
		}
		
		if ( this->CheckDurabilityState() != 0 )
		{
			return 2;
		}
		
		return 1;
		
	}
	return 0;
}





BOOL CItem::CheckDurabilityState()
{
	if ( this->m_Durability == 0.0f )
	{
		if ( this->m_CurrentDurabilityState != 1.0f )
		{
			this->m_CurrentDurabilityState = 1.0f;
			this->Convert(this->m_Type, this->m_SkillOption, this->m_LuckOption, this->m_Z28Option, this->m_NewOption,
				this->m_SetOption, this->m_ItemOptionEx, CURRENT_DB_VERSION);
			return true;
		}
	}
	else if ( this->m_Durability < this->m_DurabilityState[2] )
	{
		if ( this->m_CurrentDurabilityState != 0.5f )
		{
			this->m_CurrentDurabilityState = 0.5f;
			this->Convert(this->m_Type, this->m_SkillOption, this->m_LuckOption, this->m_Z28Option, this->m_NewOption,
				this->m_SetOption,this->m_ItemOptionEx, CURRENT_DB_VERSION);
			return true;
		}
	}
	else if ( this->m_Durability < this->m_DurabilityState[1] )
	{
		if ( this->m_CurrentDurabilityState != 0.3f )
		{
			this->m_CurrentDurabilityState = 0.3f;
			this->Convert(this->m_Type, this->m_SkillOption, this->m_LuckOption, this->m_Z28Option, this->m_NewOption,
				this->m_SetOption,this->m_ItemOptionEx, CURRENT_DB_VERSION);
			return true;
		}
	}
	else if ( this->m_Durability < this->m_DurabilityState[0] )
	{
		if ( this->m_CurrentDurabilityState != 0.2f )
		{
			this->m_CurrentDurabilityState = 0.2f;
			this->Convert(this->m_Type, this->m_SkillOption, this->m_LuckOption, this->m_Z28Option, this->m_NewOption,
				this->m_SetOption,this->m_ItemOptionEx, CURRENT_DB_VERSION);
			return true;
		}
	}
	else if ( this->m_CurrentDurabilityState != 0.0f )
	{
		this->m_CurrentDurabilityState = 0;
		this->Convert(this->m_Type, this->m_SkillOption, this->m_LuckOption, this->m_Z28Option, this->m_NewOption,
			this->m_SetOption,this->m_ItemOptionEx, CURRENT_DB_VERSION);
		return true;
	}

	return false;
}


//-----------------------------------------------------------
// End of CItem class
//-----------------------------------------------------------


void BufferItemtoConvert3(unsigned char* buf, int& type, BYTE& level, BYTE& op1, BYTE& op2, BYTE& op3, BYTE& dur)
{
	type = buf[0] + ((buf[3]&0x80)*2)+((buf[5]&0xF0)<<5);
	level = (buf[1]>>3) & 15;
	op1 = (buf[1]>>7) & 1;
	op2 = (buf[1]>>2) & 1;
	op3 = (buf[1] & 3 );
	dur = buf[2];

	if ( type == ITEMGET(13, 3) ) // Dinorant
	{
		op3 |= (buf[3] & 0x40) >> 4;
	}
	else
	{
		if ( (buf[3] & 0x40) == 0x40 )
		{
			op3 += 4;
		}
	}
}




void ItemByteConvert7(unsigned char * buf, CItem * const item, int maxitem)
{
	int n = 0;
	WORD hiWord;
	WORD loWord;

	for ( int index = 0; index < maxitem ; index++ )
	{
		buf[n] = item[index].m_Type;
		n++;
		buf[n] = 0;
		buf[n] |= item[index].m_Level * 8;
		buf[n] |= item[index].m_SkillOption * 128;
		buf[n] |= item[index].m_LuckOption * 4;
		buf[n] |= item[index].m_Z28Option;
		n++;
		buf[n] = item[index].m_Durability;
		n++;
		hiWord = item[index].m_Number >> 16;
		loWord = item[index].m_Number & 0xFFFF;
		buf[n] = SET_NUMBERH(hiWord);
		n++;
		buf[n] = SET_NUMBERL(hiWord);
		n++;
		buf[n] = SET_NUMBERH(loWord);
		n++;
		buf[n] = SET_NUMBERL(loWord);
		n++;
	}
}




void ItemByteConvert10(unsigned char * buf, CItem * const item, int maxitem)
{
	int n = 0;
	WORD hiWord;
	WORD loWord;
	int index;

	for (index = 0; index < maxitem ; index++ )
	{
		if ( item[index].m_Type == ITEMGET(13,19) )
		{
			if ( (item[index].m_Level < 0)? FALSE : ( item[index].m_Level > 2)? FALSE : true  )
			{
				buf[n] = -1;
				buf[n+1] = -1;
				buf[n+2] = -1;
				buf[n+3] = -1;
				buf[n+4] = -1;
				buf[n+5] = -1;
				buf[n+6] = -1;
				buf[n+7] = -1;
				buf[n+8] = -1;
				buf[n+9] = -1;
				n += 16;
				continue;
			}
		}
		
		if ( item[index].m_Type <  0 )
		{
			buf[n] = -1;
			buf[n+1] = -1;
			buf[n+2] = -1;
			buf[n+3] = -1;
			buf[n+4] = -1;
			buf[n+5] = -1;
			buf[n+6] = -1;
			buf[n+7] = -1;
			buf[n+8] = -1;
			buf[n+9] = -1;
			n += 16;
		}
		else
		{
			buf[n] = (BYTE)item[index].m_Type % 256 ;
			n++;
			buf[n] = 0;
			buf[n] |= item[index].m_Level * 8;
			buf[n] |= item[index].m_SkillOption * 128;
			buf[n] |= item[index].m_LuckOption * 4;
			buf[n] |= item[index].m_Z28Option & 3;
			n++;
			buf[n] = item[index].m_Durability;
			n++;
			hiWord = item[index].m_Number >> 16;
			loWord = item[index].m_Number  & 0xFFFF;
			buf[n] = SET_NUMBERH(hiWord);
			n++;
			buf[n] = SET_NUMBERL(hiWord);
			n++;
			buf[n] = SET_NUMBERH(loWord);
			n++;
			buf[n] = SET_NUMBERL(loWord);
			n++;
			buf[n] = 0;

			if ( item[index].m_Type > 255 )
			{
				buf[n] = 0x80;	// Second Type of Item
			}

			if ( item[index].m_Z28Option > 3 )
			{
				buf[n] |= 0x40;	// Z28 Option
			}

			buf[n] |= item[index].m_NewOption;
			n++;
			buf[n] = item[index].m_SetOption;
			n++;
			n++;
		}
	}
}

int IsSlotItem(WORD ItemID)
{
	if ( ItemID < 0 || ItemID >= MAX_ITEMS )
	{
		return 0;
	}

	if (ItemAttribute[ItemID].isSocketItem == TRUE)
		return 1;

	return 0;
}

void InitSocketItems(char *FilePathSockets)
{
	//	Index	ItemID
	//0
	//	0		1
	//	1		2
	//end

	int SocketItemsCount=0;
	DWORD dwArgv=0;
	FILE *fp;
	char sLineTxt[255] = {0};
	BOOL bRead = FALSE;
	
	//srand((unsigned)time(NULL));
	char path[255]; 

	for (int n=0;n<MAX_ITEMS;n++)
	{
		ItemAttribute[n].isSocketItem = FALSE;
	}

	if((fp=fopen(FilePathSockets, "r")) == NULL)
	{
		MessageBox(NULL, "Failed Reading Socket Items File", "Error ", 0);
		SocketItemsCount=0;
	}

	rewind(fp);
	
	int leestatus=-1;
	int itemSection,itemNumber;
	
	while(fgets(sLineTxt, 255, fp) != NULL)
	{
		if(sLineTxt[0] == '/')continue;  
		if(sLineTxt[0] == ';')continue;  
		
		if(bRead == FALSE && (UINT)(sLineTxt[0]-0x30) == dwArgv) bRead = TRUE;			

		if(bRead==TRUE && strlen(sLineTxt)<3)
		{
			UINT q = 0;
			sscanf(sLineTxt, "%d", &q);
			leestatus = q;
		}
		
		//if (SocketItemsCount+1>=512) leestatus = -1;

		if(leestatus == 0)
		{
			if(bRead==TRUE && strlen(sLineTxt)>3)
			{
				if ((sLineTxt[0] == 'e')&&(sLineTxt[1] == 'n')&&(sLineTxt[2] == 'd'))
				{
					leestatus = -1;
				} else {
					sscanf(sLineTxt, "%d %d", &itemSection,&itemNumber);

					ItemAttribute[ITEMGET(itemSection,itemNumber)].isSocketItem=TRUE;
					SocketItemsCount++;
				}
			}
		}
	}

	rewind(fp);
	fclose(fp);

	//Log portion for bug tracing
	LogAddTD("[Item System][LOAD][Season 4] Socket Items Loaded: %d",SocketItemsCount);
}


void ItemByteConvert(unsigned char* buf, CItem item)
{
	int n=0;

	memset(buf, 0, 7+5);
	buf[n] = item.m_Type & 0xFF;
	n++;
	buf[n] = 0;
	buf[n] |= item.m_Level * 8;
	buf[n] |= item.m_SkillOption * 128;
	buf[n] |= item.m_LuckOption * 4;
	buf[n] |= item.m_Z28Option & 3;
	n++;
	buf[n] = item.m_Durability;
	n++;
	buf[n] = 0;
	buf[n] |= ((item.m_Type & 0x100) >> 1);

	if ( item.m_Z28Option > 3 )
	{
		buf[n] |= 0x40; // item +16 option
	}

	buf[n] |= item.m_NewOption;
	n++;
	buf[n] = item.m_SetOption;
	n++;
	buf[n] |= (item.m_Type & 0x1E00 ) >> 5;

	BYTE btItemEffectFor380 = 0;

#if(ENABLE_CSHOP == 0)
	btItemEffectFor380 = (item.m_ItemOptionEx & 0x80 ) >> 4;
#else
	BYTE btItemType=0;
	btItemType |= (item.m_Type & 0x1E00 ) >> 5;

	BYTE tmp = item.m_ItemOptionEx & 2;
	if((tmp == 2) && ((btItemType+2) == item.m_ItemOptionEx))
		btItemEffectFor380 = 2;
	else
		btItemEffectFor380 = (item.m_ItemOptionEx & 0x80 ) >> 4;
#endif

	buf[n] |= btItemEffectFor380;
	n++;
	buf[n] = item.m_JewelOfHarmonyOption;

	n++;
	buf[n] = item.m_ItemSlot1-1;
	n++;
	buf[n] = item.m_ItemSlot2-1;
	n++;
	buf[n] = item.m_ItemSlot3-1;
	n++;
	buf[n] = item.m_ItemSlot4-1;
	n++;
	buf[n] = item.m_ItemSlot5-1;		
}





void ItemByteConvert(unsigned char* buf, int type, BYTE Option1, BYTE Option2, BYTE Option3, BYTE level, BYTE dur, BYTE Noption, BYTE SetOption, BYTE JewelOfHarmonyOption, BYTE ItemEffectEx, BYTE SocketSlot1, BYTE SocketSlot2, BYTE SocketSlot3, BYTE SocketSlot4, BYTE SocketSlot5)
{
	memset(buf, 0, 7);
	int n = 0;

	buf[n] = type & 0xFF;
	n++;
	buf[n] = 0;
	buf[n] |= level * 8;
	buf[n] |= Option1 * 128;
	buf[n] |= Option2 * 4;
	buf[n] |= Option3 & 3;
	n++;
	buf[n] = dur;
	n++;
	buf[n] = 0;
	buf[n] |= ((type& 0x100) >> 1);

	if ( Option3 > 3 )
	{
		buf[n] |= 0x40;
	}

	buf[n] |= Noption;
	n++;
	buf[n] = SetOption;
	n++;
	buf[n] |= (type & 0x1E00 ) >> 5;

	BYTE btItemEffectFor380 = 0;
#if(ENABLE_CSHOP == 0)
	btItemEffectFor380 = (ItemEffectEx  & 0x80 ) >> 4;
#else
	BYTE btItemType=0;
	btItemType |= (type & 0x1E00 ) >> 5;

	BYTE tmp = ItemEffectEx & 2;
	if((tmp == 2) && ((btItemType+2) == ItemEffectEx))
		btItemEffectFor380 = 2;
	else
		btItemEffectFor380 = (ItemEffectEx & 0x80 ) >> 4;
#endif
	buf[n] |= btItemEffectFor380;
	n++;
	buf[n] = JewelOfHarmonyOption;
//Slots
	n++;
	buf[n] = SocketSlot1;
	n++;
	buf[n] = SocketSlot2;
	n++;
	buf[n] = SocketSlot3;
	n++;
	buf[n] = SocketSlot4;
	n++;
	buf[n] = SocketSlot5;
}

// ///////////////////////////////////////////
// START REVIEW HERE

void ItemByteConvert16(LPBYTE buf, CItem * const item , int maxitem)
{
	int n=0;
	WORD hiWord, loWord;

	for (int index=0; index < maxitem ;index++)
	{
		if ( item[index].m_Type == ITEMGET(13,19) )		//Weapon of Archangel
		{
			if ( CHECK_LIMIT(item[index].m_Level, 3) != FALSE )
			{
				buf[n+0]=-1;
				buf[n+1]=-1;
				buf[n+2]=-1;
				buf[n+3]=-1;
				buf[n+4]=-1;
				buf[n+5]=-1;
				buf[n+6]=-1;
				buf[n+7]=-1;
				buf[n+8]=-1;
				buf[n+9]=-1;
				buf[n+10]=-1;
				buf[n+11]=-1;
				buf[n+12]=-1;
				buf[n+13]=-1;
				buf[n+14]=-1;
				buf[n+15]=-1;
				n+=16;

				continue;
			}
		}

		if ( item[index].m_Type == ITEMGET(14,64) )		//Illusion Fire Ball
		{
				buf[n+0]=-1;
				buf[n+1]=-1;
				buf[n+2]=-1;
				buf[n+3]=-1;
				buf[n+4]=-1;
				buf[n+5]=-1;
				buf[n+6]=-1;
				buf[n+7]=-1;
				buf[n+8]=-1;
				buf[n+9]=-1;
				buf[n+10]=-1;
				buf[n+11]=-1;
				buf[n+12]=-1;
				buf[n+13]=-1;
				buf[n+14]=-1;
				buf[n+15]=-1;
				n+=16;

				continue;
		}

		if ( (item[index].m_Type < ITEMGET(0,0)) )
		{
			buf[n+0]=-1;
			buf[n+1]=-1;
			buf[n+2]=-1;
			buf[n+3]=-1;
			buf[n+4]=-1;
			buf[n+5]=-1;
			buf[n+6]=-1;
			buf[n+7]=-1;
			buf[n+8]=-1;
			buf[n+9]=-1;
			buf[n+10]=-1;
			buf[n+11]=-1;
			buf[n+12]=-1;
			buf[n+13]=-1;
			buf[n+14]=-1;
			buf[n+15]=-1;
			n+=16;
		}
		else
		{
			memset(buf+n, 0, 16);

			buf[n] = (BYTE)item[index].m_Type % 256;
			n++;
			buf[n]=0;
			buf[n] |= item[index].m_Level * 8;
			buf[n] |= item[index].m_SkillOption * 128;
			buf[n] |= item[index].m_LuckOption * 4;
			buf[n] |= item[index].m_Z28Option & 3;
			n++;
			buf[n] = item[index].m_Durability;
			n++;
			hiWord = item[index].m_Number >> 16;
			loWord = item[index].m_Number  & 0xFFFF;
			buf[n] = SET_NUMBERH(hiWord);
			n++;
			buf[n] = SET_NUMBERL(hiWord);
			n++;
			buf[n] = SET_NUMBERH(loWord);
			n++;
			buf[n] = SET_NUMBERL(loWord);
			n++;
			buf[n] = 0;
			BYTE btItemType=0;
			btItemType |= (item[index].m_Type & 0x1E00 ) >> 5;
			buf[n] |= ((item[index].m_Type & 0x100) >> 1);

			if ( item[index].m_Z28Option > 3 )
			{
				buf[n] |= 0x40; // item +16 option
			}

			buf[n] |= item[index].m_NewOption;
			n++;
			buf[n] = item[index].m_SetOption;
			n++;
			buf[n] = 0;
			buf[n] |= btItemType;
			BYTE btItemEffectFor380 = 0;
			
#if(ENABLE_CSHOP == 0)
			btItemEffectFor380 = (item[index].m_ItemOptionEx & 0x80 ) >> 4;
#else
			BYTE tmp = item[index].m_ItemOptionEx & 2;
			if((tmp == 2) && ((btItemType+2) == item[index].m_ItemOptionEx))
				btItemEffectFor380 = 2;
			else
				btItemEffectFor380 = (item[index].m_ItemOptionEx & 0x80 ) >> 4;
#endif

			buf[n] |= btItemEffectFor380;
			n++;
			
			if ( item[index].m_JewelOfHarmonyOption == 0xFF )
				item[index].m_JewelOfHarmonyOption = 0;

			buf[n] = item[index].m_JewelOfHarmonyOption;
			n++;

			if (IsSlotItem(item[index].m_Type)==1)
			{
				buf[n] = item[index].m_ItemSlot1;
				n++;
				buf[n] = item[index].m_ItemSlot2;
				n++;
				buf[n] = item[index].m_ItemSlot3;
				n++;
				buf[n] = item[index].m_ItemSlot4;
				n++;
				buf[n] = item[index].m_ItemSlot5;
				n++;
			}else{
				buf[n] = 0;
				n++;
				buf[n] = 0;
				n++;
				buf[n] = 0;
				n++;
				buf[n] = 0;
				n++;
				buf[n] = 0;
				n++;
			}
		}
	}
}
	
// ItemAttribute Begin:9439368 END 9446B68 Array[512]



int ItemGetNumberMake(int type, int index)
{
	int make;

	make = type*MAX_SUBTYPE_ITEMS + index;

	if ( make < 0 || make >= MAX_ITEMS )
	{
		return -1;
	}

	if (ItemAttribute[make].Width < 1 ||  ItemAttribute[make].Height < 1)
	{
		return -1;
	}
	return make;
}

void ItemGetSize(int index, int & width, int & height)
{
	if ( index < 0 || index >= MAX_ITEMS )
	{
		width = 0;
		height = 0;
		return;
	}

	width = ItemAttribute[index].Width;
	height = ItemAttribute[index].Height;
}


BOOL HasItemDurability(int index)
{
	if ( index < 0 || index >= MAX_ITEMS )
	{
		return 0;
	}

	if ( ItemAttribute[index].Durability == 0 && ItemAttribute[index].MagicDurability == 0)
	{
		return FALSE; 
	}
	
	return true;
}

int ItemGetDurability(int index, int itemLevel, int ExcellentItem, int SetItem)
{
	if ( index < 0 || index >= MAX_ITEMS )
	{
		return 0;
	}

	if ( index == ITEMGET(14,21) && itemLevel == 3 )	// Mark Lord
		itemLevel=0;
	
	//Kundun Symbol, Piece of Paper, Lucky Peny, Blue Ball Invitation Part
	if ( (index == ITEMGET(14,29)) || (index == ITEMGET(14,100)) || (index == ITEMGET(14,101)) || (index == ITEMGET(14,110))  || (index == ITEMGET(14,153)) || (index == ITEMGET(14,154)) || (index == ITEMGET(14,155)) || (index == ITEMGET(14,156)))
		return 1;

	int dur=0;

	if ( itemLevel < 5)
	{
		dur= ItemAttribute[index].Durability + itemLevel;
	}
	else if ( itemLevel >= 5 )
	{
		if ( itemLevel == 10 )
		{
			dur=ItemAttribute[index].Durability + itemLevel*2-3;
		}
		else if (itemLevel == 11 )
		{
			dur=ItemAttribute[index].Durability + itemLevel*2-1;
		}
		else if (itemLevel == 12 )
		{
			dur=ItemAttribute[index].Durability + itemLevel*2+2;
		}
		else if (itemLevel == 13 )
		{
			dur=ItemAttribute[index].Durability + itemLevel*2+6;
		}
		else if (itemLevel == 14 )
		{
			dur=ItemAttribute[index].Durability + itemLevel*2+11;
		}
		else if (itemLevel == 15 )
		{
			dur=ItemAttribute[index].Durability + itemLevel*2+17;
		}
		else
		{
			dur=ItemAttribute[index].Durability + itemLevel*2-4;
		}
	}
	
	if ( (index < ITEMGET(12,3) || index > ITEMGET(12,6) ) &&
		  index != ITEMGET(0,19) &&
		  index != ITEMGET(4,18) &&
		  index != ITEMGET(5,10) &&
		  index != ITEMGET(2,13) &&
		  index != ITEMGET(12,49) &&
		  index != ITEMGET(13,30) )
	{
		if ( SetItem != 0 )
			dur +=20;
		else if ( ExcellentItem != 0 && index != ITEMGET(12,50) && (index < ITEMGET(12,36) || index > ITEMGET(12,43) ) ) // Third Wings Fix Durability *Prevent duple if items
			dur +=15;
	}

	if ( dur > 255 )
		dur = 255;

	return dur;
}



int ItemGetAttackDurability(int index)
{
	if ( index < 0 || index >= MAX_ITEMS )
	{
		return 0;
	}
	return ItemAttribute[index].AttackDur;
}


int ItemGetDefenseDurability(int index)
{
	if ( index < 0 || index >= MAX_ITEMS )
	{
		return 0;
	}
	return ItemAttribute[index].DefenceDur;
}


float GetRepairItemRate(int index)
{
	if ( index < 0 || index >= MAX_ITEMS )
	{
		return 0;
	}
	return ItemAttribute[index].RepaireMoneyRate;
}
// ItemAttribute Begin:9439368 END 9446B68 Array[512]

float GetAllRepairItemRate(int index)
{
	if ( index < 0 || index >= MAX_ITEMS )
	{
		return 0;
	}
	return ItemAttribute[index].AllRepaireMoneyRate;
}

void CalRepairRate( int itemtype, int itemsubtype, LPITEM_ATTRIBUTE p)
{
	p->RepaireMoneyRate = (float)0.1;
	p->AllRepaireMoneyRate = (float)0.4;
}


BOOL OpenItemScript(char* FileName) // Save Item(kor) Values Into ItemAttribute
{
	int loop_count;
	int n;
	int Token;

	SMDFile = fopen(FileName, "r");

	if ( SMDFile == 0 )
	{
		return FALSE;
	}

	memset ( ItemAttribute, 0, sizeof(ItemAttribute) );
	
	for ( n=0;n<MAX_ITEMS;n++)
	{
		ItemAttribute[n].Level = -1;
		ItemAttribute[n].RepaireMoneyRate  = 0;
		ItemAttribute[n].AllRepaireMoneyRate  = 0;
	}

	
	while ( true )
	{
		Token = GetToken();

		if ( Token == 2 )
		{
			break;
		}
		if ( Token == 1 )
		{
			int ItemType;

			ItemType = TokenNumber;
			loop_count = 0;
			while ( true )
			{
				int ItemSubType;
				LPITEM_ATTRIBUTE p;
				
				Token = GetToken();
				ItemSubType = TokenNumber;

				if ( ItemType >= 0 && ItemType < MAX_TYPE_ITEMS )
				{
					if ( g_MaxItemIndexOfEachItemType[ItemType] <= ItemSubType )
					{
						g_MaxItemIndexOfEachItemType[ItemType] = ItemSubType;
					}
				}

				
				if ( Token == 0 )
				{
					if ( strcmp("end", TokenString ) == 0 )
					{
						break;
					}
				}
					
				p = &ItemAttribute[ItemType*MAX_SUBTYPE_ITEMS + ItemSubType];

				Token = GetToken();
				p->ItemSlot = TokenNumber;

				Token = GetToken();
				p->SkillType = TokenNumber;

				Token = GetToken();
				p->Width  = TokenNumber;

				Token = GetToken();
				p->Height  = TokenNumber;

				Token = GetToken();
				p->Serial = TokenNumber;

				Token = GetToken();
				p->OptionFlag = TokenNumber;

				Token = GetToken();
				p->MondownFlag = TokenNumber;

				Token = GetToken();
				strcpy(&p->Name[0], TokenString );

				if(!strcmp(TokenString,""))
				{
					MsgBox("ERROR: FILE %s (TYPE:%d INDEX:%d)", FileName,ItemType,loop_count);
					exit(1);
				}

				if ( ItemType >= 0 && ItemType <= 5 )
				{
					Token = GetToken();
					p->Level = TokenNumber;

					Token = GetToken();
					p->DamageMin = TokenNumber;

					Token = GetToken();
					p->DamageMax = TokenNumber;

					Token = GetToken();
					p->AttackSpeed = TokenNumber;

					Token = GetToken();
					p->Durability = TokenNumber;

					Token = GetToken();
					p->MagicDurability = TokenNumber;

					Token = GetToken();
					p->MagicPW = TokenNumber;

					Token = GetToken();
					p->RequireLevel = TokenNumber;

					Token = GetToken();
					p->RequireStrength = TokenNumber;

					Token = GetToken();
					p->RequireDexterity = TokenNumber;

					Token = GetToken();
					p->RequireEnergy = TokenNumber;

					Token = GetToken();
					p->RequireVitality = TokenNumber;

					Token = GetToken();
					p->RequireLeadership = TokenNumber;

					if ( p->Width  >= 2)
					{
						p->TwoHand = true; // Review this later
					}

					(BYTE)p->Durability += (BYTE)p->MagicDurability;
				}

				if ( ItemType >= 6 && ItemType <= 11 )
				{
					Token = GetToken();
					p->Level = TokenNumber;
					
					if ( ItemType == 6 )
					{

						Token = GetToken();
						p->Defense = TokenNumber;

						Token = GetToken();
						p->SuccessfulBlocking  = TokenNumber;

					}
					else if ( ItemType >= 7 && ItemType <= 9 )
					{

						Token = GetToken();
						p->Defense  = TokenNumber;

						Token = GetToken();
						p->MagicDefense = TokenNumber;
					}
					else if ( ItemType == 10 )
					{
						Token = GetToken();
						p->Defense = TokenNumber;

						Token = GetToken();
						p->AttackSpeed = TokenNumber;
					}
					else if ( ItemType == 11 )
					{
						Token = GetToken();
						p->Defense  = TokenNumber;

						Token = GetToken();
						p->WalkSpeed = TokenNumber;
					}
					
					Token = GetToken();
					p->Durability = TokenNumber;

					Token = GetToken();
					p->RequireLevel = TokenNumber;

					Token = GetToken();
					p->RequireStrength = TokenNumber;

					Token = GetToken();
					p->RequireDexterity = TokenNumber;

					Token = GetToken();
					p->RequireEnergy = TokenNumber;

					Token = GetToken();
					p->RequireVitality = TokenNumber;

					Token = GetToken();
					p->RequireLeadership = TokenNumber;
				}

				if ( ItemType == 13 )
				{
					Token = GetToken();
					p->Level = TokenNumber;

					Token = GetToken();
					p->Durability = TokenNumber;

					Token = GetToken();
					p->Resistance[0] = TokenNumber;

					Token = GetToken();
					p->Resistance[1] = TokenNumber;

					Token = GetToken();
					p->Resistance[2] = TokenNumber;

					Token = GetToken();
					p->Resistance[3] = TokenNumber;

					Token = GetToken();
					p->Resistance[4] = TokenNumber;

					Token = GetToken();
					p->Resistance[5] = TokenNumber;

					Token = GetToken();
					p->Resistance[6] = TokenNumber;

					p->ResistanceType = -1;
					
					int n;

					for (n=0;n<MAX_ITEM_SPECIAL_ATTRIBUTE-1;n++)
					{
						if ( (p->Resistance[n]) != 0 )
						{
							p->ResistanceType=n;
						}
					}

					p->RequireLevel = p->Level ;
					
				}

				if ( ItemType == 14 )
				{
					Token = GetToken();
					p->Value = TokenNumber;

					Token = GetToken();
					p->Level = TokenNumber;

					p->Durability = 1;
				}

				if ( ItemType == 12 )
				{
					Token = GetToken();
					p->Level = TokenNumber;

					Token = GetToken();
					p->Defense = TokenNumber;

					Token = GetToken();
					p->Durability = TokenNumber;

					Token = GetToken();
					p->RequireLevel = TokenNumber;

					Token = GetToken();
					p->RequireEnergy = TokenNumber;

					Token = GetToken();
					p->RequireStrength = TokenNumber;

					Token = GetToken();
					p->RequireDexterity = TokenNumber;

					Token = GetToken();
					p->RequireLeadership = TokenNumber;

					Token = GetToken();
					p->BuyMoney = TokenNumber;

				}
				
				if ( ItemType == 15 )
				{

					Token = GetToken();
					p->Level = TokenNumber;

					Token = GetToken();
					p->RequireLevel  = TokenNumber;

					Token = GetToken();
					p->RequireEnergy  = TokenNumber;

					Token = GetToken();
					p->BuyMoney  = TokenNumber;

					p->DamageMin = p->Level;
					p->DamageMax = p->Level + p->Level/2; 
				}

				if ( ItemType <= 11 || ItemType == 13)
				{
					Token = GetToken();
				}

				p->SetAttr = TokenNumber;

				if ( ItemType <= 13 || ItemType == 15 )
				{

					Token = GetToken();
					p->RequireClass[CLASS_WIZARD]  = TokenNumber;

					Token = GetToken();
					p->RequireClass[CLASS_KNIGHT]  = TokenNumber;

					Token = GetToken();
					p->RequireClass[CLASS_ELF]  = TokenNumber;

					Token = GetToken();
					p->RequireClass[CLASS_MAGICGLADIATOR]  = TokenNumber;

					Token = GetToken();
					p->RequireClass[CLASS_DARKLORD]  = TokenNumber;

					Token = GetToken();
					p->RequireClass[CLASS_SUMMONER]  = TokenNumber;

					if(ReadConfig.S6E1 == 1)
					{
						Token = GetToken();
						p->RequireClass[CLASS_RAGEFIGHTER]  = TokenNumber;
						//p->RequireClass[CLASS_RAGEFIGHTER] = 1;
					}
				}

				CalRepairRate(ItemType, ItemSubType, p);

				p->HaveItemInfo = true;

				loop_count++;

				if ( loop_count > MAX_SUBTYPE_ITEMS )
				{
					MsgBox("Error : Item Data fail. (LoopCount:%d) Item Type:%d Index %d", loop_count, ItemType, ItemSubType);
					break;
				}
			}

			if ( loop_count > MAX_SUBTYPE_ITEMS )
			{
				break;
			}			
		}
	}

#if (WL_PROTECT==1)
	int MyCheckVar;   
	VM_START_WITHLEVEL(20)
		CHECK_PROTECTION(MyCheckVar, 0x75456783)  	 
		if (MyCheckVar != 0x75456783)
		{			
			for ( n=0;n<MAX_ITEMS;n++)
			{
				for(int i=0;i<6;i++)
					ItemAttribute[n].RequireClass[i]=rand()%2;
			}
		}
	VM_END
#endif
	
	fclose(SMDFile);
	return true;
}


BOOL OpenItemNameScript(char* FileName)	// To Change Name of Kor To Language Selected
{
	
	int Token;

	SMDFile = fopen(FileName, "r");

	if (SMDFile == 0 )
	{
		return FALSE;
	}

	while ( true )
	{
		Token = GetToken();

		if ( Token == 2 )
		{
			break;
		}
		if ( Token == 1 )
		{
			int ItemType;

			ItemType = TokenNumber;

			while ( true )
			{
				int ItemSubType;
				LPITEM_ATTRIBUTE p;

				Token = GetToken();
				ItemSubType = TokenNumber;

				if ( Token == 0 )
				{
					if ( strcmp("end", TokenString ) == 0 )
					{
						break;
					}
				}
					
				p = &ItemAttribute[ItemType*MAX_SUBTYPE_ITEMS + ItemSubType];

				Token = GetToken();
				strcpy(&p->Name[0], TokenString );

				
			}
		}
	}
	fclose(SMDFile);
	return true;
}

int zzzItemLevel(int type, int index, int level)
{
	int item_num = (type*MAX_SUBTYPE_ITEMS)+index ;

	if ( item_num < 0 || item_num >= MAX_ITEMS )
	{
		return 0;
	}

	if (level < 0 )
	{
		level = 0;
	}
	
	if ( ItemAttribute[item_num].Level == (BYTE)-1 )
	{
		return 0;
	}

	if ( ItemAttribute[item_num].Level == 0 )
	{
		return 0;
	}

	if ( ItemAttribute[item_num].Level < level )
	{
		return 1;
	}

	return 0;
}





int GetLevelItem(int type, int index, int level)
{
	int item_num;
	int itemlevel;

	if ( level < 0 )
	{
		level = 0;
	}

	item_num = (type * MAX_SUBTYPE_ITEMS) + index;
	if ( item_num < 0 || item_num >= MAX_ITEMS )
	{
		return -1;
	}

	if (ItemAttribute[item_num].MondownFlag  == 0)
	{
		return -1;
	}

	if ( ItemAttribute[item_num].Level == 0xFF )
	{
		return -1;
	}

	if ( ItemAttribute[item_num].Level == 0 )
	{
		return -1;
	}

	if ( type == 14 )
	{
		itemlevel = ItemAttribute[item_num].Level;

		if ( index == 15 )
		{
			return -1;
		}

		if ( itemlevel >= (level-8) )
		{
			if ( itemlevel <= level )
			{
				return 0;
			}
		}

		return -1;
	}

	if ( type == 13 && index == 10  )
	{
		int ilevel;

		if ( (rand()%10) == 0 )
		{
			ilevel = 0;

			if ( level < 0 )
			{
				level = 0;
			}

			ilevel= level/10;

			if ( ilevel > 0 )
			{
				ilevel--;
			}

			if ( ilevel > 5)
			{
				ilevel = 5;
			}

			return ilevel;
		}

		return -1;
	}

	if ( type == 12 && index ==  11 )
	{
		int ilevel;

		if ( (rand()%10) == 0 )
		{
			ilevel = 0;

			if ( level < 0 )
			{
				level = 0;
			}

			ilevel= level/10;

			if ( ilevel > 0 )
			{
				ilevel--;
			}

			if ( ilevel > 6)
			{
				ilevel = 6;
			}

			return ilevel;
		}

		return -1;
	}	

	itemlevel = ItemAttribute[item_num].Level;
	
	if ( itemlevel >= level - 18 && itemlevel <= level)
	{
		if ( type == 15 )
		{
			return 0;
		}

		itemlevel = (level - itemlevel)/3;

		if ( type == 13 )
		{
			if ( index == 8 || index == 9 || index == 12 || index == 13 || index == 20 || index == 21 || index == 22 || index == 23 || index == 24 || index == 25 || index == 26 || index == 27 || index == 28 )
			{
				if ( itemlevel > 4 )
				{
					itemlevel=4;
				}
			}
		}

		return itemlevel;
		
	}
	return -1;
}


int GetSerialItem(int type)
{
	int item_num = type;

	if ( item_num < 0 || item_num >= MAX_ITEMS )
	{
		return 0;
	}

	if ( ItemAttribute[item_num].Level == 0xFF )
	{
		return -1;
	}
	if ( ItemAttribute[item_num].Serial  == 0 )
	{
		return 0;
	}
	return 1;
}

bool IsLuckyItem(int item_num)
{
	if( (item_num >= ITEMGET(7,62) && item_num <= ITEMGET(7,72))
			|| (item_num >= ITEMGET(8,62) && item_num <= ITEMGET(8,72))
			|| (item_num >= ITEMGET(9,62) && item_num <= ITEMGET(9,72))
			|| (item_num >= ITEMGET(10,62) && item_num <= ITEMGET(10,72))
			|| (item_num >= ITEMGET(11,62) && item_num <= ITEMGET(11,72))
			)
	{
		return true;
	}
	return false;
}

void DestroyLuckyItem(int aIndex, int target)
{
	LPOBJ lpObj = &gObj[aIndex];
	GCServerMsgStringSend("Lucky item has Vanished!", lpObj->m_Index, 1);
	LogAddTD(lMsg.Get(MSGGET(2, 10)), lpObj->AccountID, lpObj->Name,
			lpObj->pInventory[target].GetName(), lpObj->pInventory[target].m_Level);

	gObjInventoryDeleteItem(lpObj->m_Index, target);
	GCInventoryItemDeleteSend(lpObj->m_Index, target, 0);
	gObjMakePreviewCharSet(lpObj->m_Index);
	GCEquipmentChange(lpObj->m_Index, target);
}

int IsItem(int item_num)
{
	if ( item_num < 0 || item_num >= MAX_ITEMS )
	{
		return 0;
	}

	return ItemAttribute[item_num].HaveItemInfo;
}




LPITEM_ATTRIBUTE GetItemAttr(int item_num)
{
	if ( item_num < 0 || item_num >= MAX_ITEMS )
	{
		return NULL;
	}

	return &ItemAttribute[item_num];
}

//Season 3 EP2 Third Wings Addition
BOOL CItem::CheckDamageReturnWing()
{
	if((this->m_Type < ITEMGET(12,36) || this->m_Type > ITEMGET(12,43)) || (this->m_Type == ITEMGET(12,41)) || (this->m_Type == ITEMGET(12,42)))
	{
		
#if (CRYSTAL_EDITION == 1)
		if(this->m_Type >= ITEMGET(12,200) && this->m_Type <= ITEMGET(12,254))
		{
		}else
		{
#endif
			return FALSE;		
#if (CRYSTAL_EDITION == 1)
		}
#endif
	}

	if ( this->m_Durability  < 1.0f )
	{
		return FALSE;
	}

	/*for(int n = 3;n < 8;n++)
	{
		if(this->m_Special[n] == 0x65)
		{
			return TRUE;
		}
	}*/

	if ( (this->m_NewOption & 2)!= 0 ) //This is Increase Max Mana Option for Wings Level2 but 5% Chance of return damage for Wings Level 3
	{
		return TRUE;
	}

	return FALSE;
}

BOOL CItem::CheckFullLifeRecoveryWing()
{
	if((this->m_Type < ITEMGET(12,36) || this->m_Type > ITEMGET(12,43)) || (this->m_Type == ITEMGET(12,41)) || (this->m_Type == ITEMGET(12,42)))
	{
		
#if (CRYSTAL_EDITION == 1)
		if(this->m_Type >= ITEMGET(12,200) && this->m_Type <= ITEMGET(12,254))
		{
		}else
		{
#endif
		return FALSE;		
#if (CRYSTAL_EDITION == 1)
		}
#endif
	}

	if ( this->m_Durability  < 1.0f )
	{
		return FALSE;
	}

	/*for(int n = 3;n < 8;n++)
	{
		if(this->m_Special[n] == 0x66)
		{
			return TRUE;
		}
	}*/

	if ( (this->m_NewOption & 4)!= 0 ) //This is Ignore Opponent Defensive Power Option for Wings Level2 but 5% Chance of recover full life for Wings Level 3
	{
		return TRUE;
	}

	return FALSE;
}

BOOL CItem::CheckFullManaRecoveryWing()
{
	if((this->m_Type < ITEMGET(12,36) || this->m_Type > ITEMGET(12,43)) || (this->m_Type == ITEMGET(12,41)) || (this->m_Type == ITEMGET(12,42)))
	{		
#if (CRYSTAL_EDITION == 1)
		if(this->m_Type >= ITEMGET(12,200) && this->m_Type <= ITEMGET(12,254))
		{
		}else
		{
#endif
		return FALSE;		
#if (CRYSTAL_EDITION == 1)
		}
#endif
	}

	if ( this->m_Durability  < 1.0f )
	{
		return FALSE;
	}

	/*for(int n = 3;n < 8;n++)
	{
		if(this->m_Special[n] == 0x69)
		{
			return TRUE;
		}
	}*/

	if ( (this->m_NewOption & 8)!= 0 ) //This is Increase Max Stamina Option for Wings Level2 but 5% Chance of recover full mana for Wings Level 3
	{
		return TRUE;
	}

	return FALSE;
}

BOOL CItem::CheckThirdWingsSuccessfulBlocking()
{
	if((this->m_Type < ITEMGET(12,36) || this->m_Type > ITEMGET(12,43)) || (this->m_Type == ITEMGET(12,41)) || (this->m_Type == ITEMGET(12,42)))
	{		
#if (CRYSTAL_EDITION == 1)
		if(this->m_Type >= ITEMGET(12,200) && this->m_Type <= ITEMGET(12,254))
		{
		}else
		{
#endif
		return FALSE;		
#if (CRYSTAL_EDITION == 1)
		}
#endif
	}

	if ( this->m_Durability  < 1.0f )
	{
		return FALSE;
	}

	if ( (this->m_NewOption & 1)!= 0 ) //This is Increase HP Option for Wings Level2 but Ignore Oponent Defensive for Wings Level 3
	{
		return TRUE;
	}

	return FALSE;
}


// ItemAttribute Begin:9439368 END 9446B68 Array[512]