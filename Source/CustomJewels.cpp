#include "stdafx.h"
#include "GameServer.h"
#include "LogProc.h"
#include "GameMain.h"
#include "TNotice.h"
#include "..\include\readscript.h"
#include "LogToFile.h"
#include "..\common\winutil.h"
#include "ObjCalCharacter.h"
#include "CustomJewels.h"
#include "..\common\SetItemOption.h"
#include "user.h"
#include "gObjMonster.h"

#if (CRYSTAL_EDITION==1)

extern CLogToFile ANTI_HACK_LOG;

CustomJewels CJewel;

void CustomJewels::Read(char * FilePath)
{
	int Token;
	int Level;
	int Option;
	int Luck;
	int Skill;
	int Excellent;
	int SetItem;
	int Sokets;
	int Destroy;

	int MinLevel;
	int MaxLevel;

	int hasToHaveLuck;		//DaRKav Jewel Limit Addon
	int hasToHaveSkill;
	int MinZ28Option;
	int hasToBeAncient;
	int hasToBeExcellent;
	int MaxExc;
	int MinNumberSokets;
	int MaxNumberSokets;
	int SoketItem;

	int Rate;
	int Counter;

	SMDFile = fopen(FilePath, "r");

	if ( SMDFile == NULL )
	{
		MsgBox("Custom Jewels data load error %s", FilePath);
		return;
	}

	while ( true )
	{
		int iType = GetToken();
		
		if ( iType == 1 )
		{
			Counter = 0;

			while(true)
			{
				if (Counter >= MAX_CUSTOM_JEWELS)
				{
					LogAddTD("[CJewel][Load] Jewel Ammount overflow, breaking load...");
					break;
				}

				Token = GetToken();
				if ( strcmp("end", TokenString) == 0 )
				{
					break;
				}
				int Type = TokenNumber;

				Token = GetToken();
				int Index = TokenNumber;

				int Item = ITEMGET(Type,Index);

				Token = GetToken();
				MinLevel = TokenNumber;

				Token = GetToken();
				MaxLevel = TokenNumber;

				Token = GetToken();
				hasToHaveLuck = TokenNumber;

				Token = GetToken();
				hasToHaveSkill = TokenNumber;

				Token = GetToken();
				MinZ28Option = TokenNumber;

				Token = GetToken();
				hasToBeAncient = TokenNumber;

				Token = GetToken();
				hasToBeExcellent = TokenNumber;

				Token = GetToken();
				MaxExc = TokenNumber;

				Token = GetToken();
				SoketItem = TokenNumber;

				Token = GetToken();
				MinNumberSokets = TokenNumber;

				Token = GetToken();
				MaxNumberSokets = TokenNumber;

				Token = GetToken();
				Rate = TokenNumber;

				this->Property[Counter].hasToHaveLuck = hasToHaveLuck;
				this->Property[Counter].hasToHaveSkill = hasToHaveSkill;
				this->Property[Counter].MinZ28Option = MinZ28Option;
				this->Property[Counter].hasToBeAncient = hasToBeAncient;
				this->Property[Counter].hasToBeExcellent = hasToBeExcellent;
				this->Property[Counter].MaxExc = MaxExc;
				this->Property[Counter].MinNumberSokets = MinNumberSokets;
				this->Property[Counter].MaxNumberSokets = MaxNumberSokets;
				this->Property[Counter].hasToBeSoketItem = SoketItem;

				this->Property[Counter].IsJewel = TRUE;
				this->Property[Counter].ItemID = Item;
				this->Property[Counter].MinLevel = MinLevel;
				this->Property[Counter].MaxLevel = MaxLevel;
				this->Property[Counter].Rate = Rate;
				LogAddTD("[CJewel][Load][%d] Item:%d(%d,%d) Set:[%d,%d,%d]/[%d,%d,%d]",
				Counter, Item, Type, Index,
				this->Property[Item].MinLevel, this->Property[Item].MaxLevel, this->Property[Item].Rate,
				MinLevel, MaxLevel, Rate);

				Counter++;
			}
		}
		
		iType = GetToken();
		iType = TokenNumber;
		if ( iType == 2 )
		{
			Counter = 0;

			while(true)
			{
				if (Counter >= MAX_CUSTOM_JEWELS)
				{
					LogAddTD("[CJewel][Success] Jewel Ammount overflow, breaking load...");
					break;
				}

				Token = GetToken();
				if ( strcmp("end", TokenString) == 0 )
				{
					break;
				}
				int Type = TokenNumber;

				Token = GetToken();
				int Index = TokenNumber;

				int Item = ITEMGET(Type,Index);

				Token = GetToken();
				Level = TokenNumber;

				Token = GetToken();
				Option = TokenNumber;

				Token = GetToken();
				Luck = TokenNumber;

				Token = GetToken();
				Skill = TokenNumber;

				Token = GetToken();
				Excellent = TokenNumber;

				Token = GetToken();
				SetItem = TokenNumber;

				Token = GetToken();
				Sokets = TokenNumber;

				Destroy = 0;
				
				this->Success[Counter].ItemID = Item;
				this->Success[Counter].Level = Level;
				this->Success[Counter].Option = Option;
				this->Success[Counter].Luck = Luck;
				this->Success[Counter].Skill = Skill;
				this->Success[Counter].Excellent = Excellent;
				this->Success[Counter].SetItem = SetItem;
				this->Success[Counter].Sokets = Sokets;
				this->Success[Counter].Destroy = Destroy;
				LogAddTD("[CJewel][Success][%d] Item:%d(%d,%d) [%d,%d,%d,%d,%d,%d,%d,%d]",
				Counter, Item, Type, Index,
				Level,Option,Luck,Skill,Excellent,SetItem,Sokets,Destroy);

				Counter++;
			}
		}
		
		iType = GetToken();
		iType = TokenNumber;
		if ( iType == 3 )
		{
			Counter = 0;

			while(true)
			{
				if (Counter >= MAX_CUSTOM_JEWELS)
				{
					LogAddTD("[CJewel][Fail] Jewel Ammount overflow, breaking load...");
					break;
				}

				Token = GetToken();
				if ( strcmp("end", TokenString) == 0 )
				{
					break;
				}
				int Type = TokenNumber;

				Token = GetToken();
				int Index = TokenNumber;

				int Item = ITEMGET(Type,Index);

				Token = GetToken();
				Level = TokenNumber;

				Token = GetToken();
				Option = TokenNumber;

				Token = GetToken();
				Luck = TokenNumber;

				Token = GetToken();
				Skill = TokenNumber;

				Token = GetToken();
				Excellent = TokenNumber;

				Token = GetToken();
				SetItem = TokenNumber;

				Token = GetToken();
				Sokets = TokenNumber;

				Token = GetToken();
				Destroy = TokenNumber;
				
				this->Fail[Counter].ItemID = Item;
				this->Fail[Counter].Level = Level;
				this->Fail[Counter].Option =Option;
				this->Fail[Counter].Luck = Luck;
				this->Fail[Counter].Skill = Skill;
				this->Fail[Counter].Excellent = Excellent;
				this->Fail[Counter].SetItem = SetItem;
				this->Fail[Counter].Sokets = Sokets;
				this->Fail[Counter].Destroy = Destroy;
				LogAddTD("[CJewel][Fail][%d] Item:%d(%d,%d) [%d,%d,%d,%d,%d,%d,%d,%d]",
				Counter, Item, Type, Index,
				Level,Option,Luck,Skill,Excellent,SetItem,Sokets,Destroy);

				Counter++;
			}
		}
		break;
	}
			
	LogAddTD("[Custom Jewels] - %s file is Loaded",FilePath);
	fclose(SMDFile);
}

void CustomJewels::Init()
{
	for(int i=0; i<MAX_CUSTOM_JEWELS; i++)
	{
		this->Property[i].IsJewel = 0;
		this->Property[i].ItemID = 0;

		this->Property[i].MaxLevel = MAX_ITEM_LEVEL;
		this->Property[i].MinLevel = 0;
		this->Property[i].Rate = 100;

		this->Property[i].hasToHaveLuck = 0;
		this->Property[i].hasToHaveSkill = 0;
		this->Property[i].MinZ28Option = 0;
		this->Property[i].hasToBeAncient = 0;
		this->Property[i].hasToBeExcellent = 0;
		this->Property[i].MinNumberSokets = 0;
		this->Property[i].hasToBeSoketItem = 0;

		this->Success[i].ItemID = 0;
		this->Success[i].Level = 0;
		this->Success[i].Option =0;
		this->Success[i].Luck = 0;
		this->Success[i].Skill = 0;
		this->Success[i].Excellent = 0;
		this->Success[i].SetItem = 0;
		this->Success[i].Sokets = 0;
		this->Success[i].Destroy = 0;
		
		this->Fail[i].ItemID = 0;
		this->Fail[i].Level = 0;
		this->Fail[i].Option =0;
		this->Fail[i].Luck = 0;
		this->Fail[i].Skill = 0;
		this->Fail[i].Excellent = 0;
		this->Fail[i].SetItem = 0;
		this->Fail[i].Sokets = 0;
		this->Fail[i].Destroy = 0;
	}

	this->Read("..\\SCFData\\CrystalEdition\\SCF_CustomJewels.txt");
}

int CustomJewels::IsJewel(int ItemID)
{
	for(int i=0; i<MAX_CUSTOM_JEWELS; i++)
	{
		if(this->Property[i].IsJewel == TRUE)
		{
			if(this->Property[i].ItemID == ItemID)
			{
				return i;
			}
		}
	}

	return -1;
}

int CustomJewels::GetSuccessPosition(int ItemID)
{
	for(int i=0; i<MAX_CUSTOM_JEWELS; i++)
	{
		if(this->Success[i].ItemID == ItemID)
		{
			return i;
		}
	}

	return -1;
}

int CustomJewels::GetFailPosition(int ItemID)
{
	for(int i=0; i<MAX_CUSTOM_JEWELS; i++)
	{
		if(this->Fail[i].ItemID == ItemID)
		{
			return i;
		}
	}

	return -1;
}

bool CustomJewels::Apply(LPOBJ lpObj, int source, int target)
{
	char sbuf[1024]={0};
	int TIndex = lpObj->pInventory[target].m_Type;
	int SIndex = lpObj->pInventory[source].m_Type;

	if(source < 0 || source > ReadConfig.MAIN_INVENTORY_SIZE(lpObj->m_Index,false)-1 )
	{
		return false;
	}

	if(target < 0 || target > ReadConfig.MAIN_INVENTORY_SIZE(lpObj->m_Index,false)-1 )
	{
		return false;
	}

	if(lpObj->pInventory[source].IsItem() == 0)
	{
		return false;
	}

	if(lpObj->pInventory[target].IsItem() == 0)
	{
		return false;
	}

	if (lpObj->pInventory[target].m_Type >= ITEMGET(12,0))
	{
		GCServerMsgStringSend("Can not aply this jewel to this item!",lpObj->m_Index, 0x01);
		return false;
	}

	if (!gObjJewelUpHackCheck(lpObj,target))
	{
		GCServerMsgStringSend("[Anti-Hack] Shadow Bug Attempt!",lpObj->m_Index, 0x01);
		if(ReadConfig.AHLog == TRUE)
		{
			ANTI_HACK_LOG.Output("[Anti-Hack][Custom Jewel][%s][%s] Shadow Bug Attempt, Item: %d",
				lpObj->AccountID,lpObj->Name,lpObj->pInventory[target].m_Type);
		}
		return false;
	}

	BYTE jPos = IsJewel(SIndex);
	BYTE jSuccess = GetSuccessPosition(SIndex);
	BYTE jFail = GetFailPosition(SIndex);

	if(jPos == -1)
	{
		LogAddTD("[CJewel] Could not find jewel property with id: %d", SIndex);
		GCServerMsgStringSend("JEWEL ERROR, contact GM!",lpObj->m_Index, 0x01);
		return false;
	}
	if(jSuccess == -1)
	{
		LogAddTD("[CJewel] Could not find jewel success with id: %d", SIndex);
		GCServerMsgStringSend("JEWEL ERROR, contact GM!",lpObj->m_Index, 0x01);
		return false;
	}
	if(jFail == -1)
	{
		LogAddTD("[CJewel] Could not find jewel fail with id: %d", SIndex);
		GCServerMsgStringSend("JEWEL ERROR, contact GM!",lpObj->m_Index, 0x01);
		return false;
	}

	if(lpObj->pInventory[target].m_Level < this->Property[jPos].MinLevel)
	{
		wsprintf(sbuf,"Item level must be higher than %d!",this->Property[jPos].MinLevel);
		GCServerMsgStringSend(sbuf,lpObj->m_Index, 0x01);
		return false;
	}

	if(lpObj->pInventory[target].m_Level > this->Property[jPos].MaxLevel)
	{
		wsprintf(sbuf,"Item level must be lower than %d!",this->Property[jPos].MaxLevel);
		GCServerMsgStringSend(sbuf,lpObj->m_Index, 0x01);
		return false;
	}

	if(lpObj->pInventory[target].m_Z28Option < this->Property[jPos].MinZ28Option)
	{
		wsprintf(sbuf,"Item JOL option minimum is +%d",(this->Property[jPos].MinZ28Option*4));
		GCServerMsgStringSend(sbuf,lpObj->m_Index, 0x01);
		return false;
	}

	if(this->Property[jPos].hasToHaveLuck == -1)
	{
		if (lpObj->pInventory[target].m_LuckOption != 0)
		{
			GCServerMsgStringSend("Item can not have Luck!",lpObj->m_Index, 0x01);
			return false;
		}
	}
	if(this->Property[jPos].hasToHaveLuck > 0)
	{
		if (lpObj->pInventory[target].m_LuckOption == 0)
		{
			GCServerMsgStringSend("Item has to have Luck!",lpObj->m_Index, 0x01);
			return false;
		}
	}

	if(this->Property[jPos].hasToHaveSkill == -1)
	{
		if (lpObj->pInventory[target].m_SkillOption != 0)
		{
			GCServerMsgStringSend("Item can not have Skill!",lpObj->m_Index, 0x01);
			return false;
		}
	}
	if(this->Property[jPos].hasToHaveSkill > 0)
	{
		if (lpObj->pInventory[target].m_SkillOption == 0)
		{
			GCServerMsgStringSend("Item has to have Skill!",lpObj->m_Index, 0x01);
			return false;
		}
	}

	if(this->Property[jPos].hasToBeAncient == -1)
	{
		if (lpObj->pInventory[target].m_SetOption != 0)
		{
			GCServerMsgStringSend("Item can not be Ancient!",lpObj->m_Index, 0x01);
			return false;
		}
	}
	if(this->Property[jPos].hasToBeAncient > 0)
	{
		if (lpObj->pInventory[target].m_SetOption == 0)
		{
			GCServerMsgStringSend("Item must be Ancient!",lpObj->m_Index, 0x01);
			return false;
		}
	}

	if(this->Property[jPos].hasToBeExcellent == -1)
	{
		if (lpObj->pInventory[target].m_NewOption != 0)
		{
			GCServerMsgStringSend("Item can not be Excellent!",lpObj->m_Index, 0x01);
			return false;
		}
	}
	if(this->Property[jPos].hasToBeExcellent > 0)
	{
		if (lpObj->pInventory[target].m_NewOption == 0)
		{
			GCServerMsgStringSend("Item must be Excellent!",lpObj->m_Index, 0x01);
			return false;
		}
	}

	if(this->Property[jPos].MaxExc != 0)
	{
		if(NumOfExcOptions(lpObj->pInventory[target].m_NewOption) >= this->Property[jPos].MaxExc)
		{
			wsprintf(sbuf,"Cant add jewel, you have a lot of excellent options, max opt are %d!",this->Property[jPos].MaxExc);
			GCServerMsgStringSend(sbuf,lpObj->m_Index, 0x01);
			return false;
		}
	}

	if(this->Property[jPos].hasToBeSoketItem == -1)
	{
		if(IsSlotItem(lpObj->pInventory[target].m_Type)==1)
		{
			GCServerMsgStringSend("Item can not be socket item!",lpObj->m_Index, 0x01);
			return false;
		}
	}
	if(this->Property[jPos].hasToBeSoketItem == 1)
	{
		if(IsSlotItem(lpObj->pInventory[target].m_Type)==0)
		{
			GCServerMsgStringSend("This item has to be socket item!",lpObj->m_Index, 0x01);
			return false;
		} else {
			if(this->Property[jPos].MinNumberSokets > 0)
			{
				BYTE counter = 0;

				if (lpObj->pInventory[target].m_ItemSlot1 > 0)
					counter += 1;
				if (lpObj->pInventory[target].m_ItemSlot2 > 0)
					counter += 1;
				if (lpObj->pInventory[target].m_ItemSlot3 > 0)
					counter += 1;
				if (lpObj->pInventory[target].m_ItemSlot4 > 0)
					counter += 1;
				if (lpObj->pInventory[target].m_ItemSlot5 > 0)
					counter += 1;

				if (this->Property[jPos].MinNumberSokets > counter)
				{
					wsprintf(sbuf,"Item minimum socket count is %d",this->Property[jPos].MinNumberSokets);
					GCServerMsgStringSend(sbuf,lpObj->m_Index, 0x01);
					return false;
				}
			}
		}
	}

	if(this->Property[jPos].MaxNumberSokets != 0)
	{
		BYTE counter = 0;

		if (lpObj->pInventory[target].m_ItemSlot1 > 0)
			counter += 1;
		if (lpObj->pInventory[target].m_ItemSlot2 > 0)
			counter += 1;
		if (lpObj->pInventory[target].m_ItemSlot3 > 0)
			counter += 1;
		if (lpObj->pInventory[target].m_ItemSlot4 > 0)
			counter += 1;
		if (lpObj->pInventory[target].m_ItemSlot5 > 0)
			counter += 1;

		if(counter >= this->Property[jPos].MaxNumberSokets)
		{
			wsprintf(sbuf,"Cant add jewel, you have a lot of socket slots, max slot are %d!",this->Property[jPos].MaxNumberSokets);
			GCServerMsgStringSend(sbuf,lpObj->m_Index, 0x01);
			return false;
		}
	}

	//if(MuItemShop.IsCShopItem(aIndex,target) == true)
	//{
	//	GCServerMsgStringSend("You can not use the jewel in this item!",lpObj->m_Index, 0x01);
	//	return true;
	//}

	LogAddTD("[CJewel][%s][%s] Use %s on item %s (%d)",
		lpObj->AccountID,lpObj->Name,lpObj->pInventory[source].GetName(),lpObj->pInventory[target].GetName(),
		lpObj->pInventory[target].m_Number);

	int _r = rand()%100;
	if(_r < this->Property[jPos].Rate )
	{
		//Success
		GCServerMsgStringSend("Jewel Succeded!",lpObj->m_Index, 0x01);
				
		LogAddTD("[CJewel][%s][%s] Success %s on item %s (%d) [%d][%d,%d]",
			lpObj->AccountID,lpObj->Name,lpObj->pInventory[source].GetName(),lpObj->pInventory[target].GetName(),
			lpObj->pInventory[target].m_Number,
			TIndex,_r,this->Property[jPos].Rate);

		if(this->Success[jSuccess].Luck == 1)
		{
			lpObj->pInventory[target].m_LuckOption = 1;
		}
		if(this->Success[jSuccess].Skill == 1)
		{
			lpObj->pInventory[target].m_SkillOption = 1;
		}
		if(this->Success[jSuccess].SetItem == 1)
		{
			lpObj->pInventory[target].m_SetOption = gSetItemOption.GenSetOption(lpObj->pInventory[target].m_Type);
		}
		if(this->Success[jSuccess].Excellent != 63)
		{
			if(this->Success[jSuccess].Excellent > 0) 
			{
				lpObj->pInventory[target].m_NewOption |= this->Success[jSuccess].Excellent;
			}
		}else
		{
			lpObj->pInventory[target].m_NewOption = 63;
		}
		if(this->Success[jSuccess].Level > 0)
		{
			if((lpObj->pInventory[target].m_Level + this->Success[jSuccess].Level) > MAX_ITEM_LEVEL)
			{
				lpObj->pInventory[target].m_Level = MAX_ITEM_LEVEL;
			}else
			{
				lpObj->pInventory[target].m_Level += this->Success[jSuccess].Level;
			}
		}

		if(this->Success[jSuccess].Sokets > 0)
		{
			if ( IsSlotItem(lpObj->pInventory[target].m_Type) )
			{
				for(int k=0;k<this->Success[jSuccess].Sokets;k++)
				{
					if (lpObj->pInventory[target].m_ItemSlot1 == 0)
						lpObj->pInventory[target].m_ItemSlot1 = 0xFF;
					else if (lpObj->pInventory[target].m_ItemSlot2 == 0)
						lpObj->pInventory[target].m_ItemSlot2 = 0xFF;
					else if (lpObj->pInventory[target].m_ItemSlot3 == 0)
						lpObj->pInventory[target].m_ItemSlot3 = 0xFF;
					else if (lpObj->pInventory[target].m_ItemSlot4 == 0)
						lpObj->pInventory[target].m_ItemSlot4 = 0xFF;
					else if (lpObj->pInventory[target].m_ItemSlot5 == 0)
						lpObj->pInventory[target].m_ItemSlot5 = 0xFF;
				}
			}
		}

		if(this->Success[jSuccess].Option > 0)
		{
			if((lpObj->pInventory[target].m_Z28Option + this->Success[jSuccess].Option) > 7)
			{
				lpObj->pInventory[target].m_Z28Option = 7;
			}else
			{
				lpObj->pInventory[target].m_Z28Option += this->Success[jSuccess].Option;
			}
		}
	}
	else
	{
		//Fail
		GCServerMsgStringSend("Jewel Failed!",lpObj->m_Index, 0x01);
		
		LogAddTD("[CJewel][%s][%s] Fail %s on item %s (%d) [%d][%d,%d]",
			lpObj->AccountID,lpObj->Name,lpObj->pInventory[source].GetName(),lpObj->pInventory[target].GetName(),
			lpObj->pInventory[target].m_Number,
			TIndex,_r,this->Property[jPos].Rate);

		if(this->Fail[jFail].Destroy == 1)
		{			
			gObjInventoryItemSet(lpObj->m_Index, target, -1);
			lpObj->pInventory[target].Clear();
			GCInventoryItemDeleteSend(lpObj->m_Index, target, 1);
			return true;
		}
		
		if(this->Fail[jFail].Luck == -1)
		{
			lpObj->pInventory[target].m_LuckOption = 0;
		}
		if(this->Fail[jFail].Skill == -1)
		{
			lpObj->pInventory[target].m_SkillOption = 0;
		}
		if(this->Fail[jFail].SetItem == -1)
		{
			lpObj->pInventory[target].m_SetOption = 0;
		}
		if(this->Fail[jFail].Excellent < 0)
		{
			if(this->Fail[jFail].Excellent == -63)
				lpObj->pInventory[target].m_NewOption = 0;
			else if( (lpObj->pInventory[target].m_NewOption&(this->Fail[jFail].Excellent)) )
				lpObj->pInventory[target].m_NewOption += (this->Fail[jFail].Excellent);

			if(lpObj->pInventory[target].m_NewOption < 0)
				lpObj->pInventory[target].m_NewOption = 0;
		}
		if(this->Fail[jFail].Level < 0)
		{
			if((lpObj->pInventory[target].m_Level + (this->Fail[jFail].Level)) < 0)
			{
				lpObj->pInventory[target].m_Level = 0;
			}else
			{
				lpObj->pInventory[target].m_Level += (this->Fail[jFail].Level);
			}
		}
		if(this->Fail[jFail].Sokets < 0)
		{
			if ( IsSlotItem(lpObj->pInventory[target].m_Type) )
			{
				for(int k=0;k<(this->Fail[jFail].Sokets);k++)
				{
					if (lpObj->pInventory[target].m_ItemSlot5 > 0)
						lpObj->pInventory[target].m_ItemSlot5 = 0x00;
					else if (lpObj->pInventory[target].m_ItemSlot4 > 0)
						lpObj->pInventory[target].m_ItemSlot4 = 0x00;
					else if (lpObj->pInventory[target].m_ItemSlot3 > 0)
						lpObj->pInventory[target].m_ItemSlot3 = 0x00;
					else if (lpObj->pInventory[target].m_ItemSlot2 > 0)
						lpObj->pInventory[target].m_ItemSlot2 = 0x00;
					else if (lpObj->pInventory[target].m_ItemSlot1 > 0)
						lpObj->pInventory[target].m_ItemSlot1 = 0x00;
				}
			}
		}
		if(this->Fail[jFail].Option < 0)
		{
			if((lpObj->pInventory[target].m_Z28Option + (this->Fail[jFail].Option)) < 0)
			{
				lpObj->pInventory[target].m_Z28Option = 0;
			}else
			{
				lpObj->pInventory[target].m_Z28Option += (this->Fail[jFail].Option);
			}
		}
	}

	float levelitemdur = ItemGetDurability(lpObj->pInventory[target].m_Type,lpObj->pInventory[target].m_Level,lpObj->pInventory[target].IsExtItem(),lpObj->pInventory[target].IsSetItem());

	lpObj->pInventory[target].m_Durability = levelitemdur * lpObj->pInventory[target].m_Durability / lpObj->pInventory[target].m_BaseDurability;

	lpObj->pInventory[target].Convert(
			lpObj->pInventory[target].m_Type,
			lpObj->pInventory[target].m_SkillOption,
			lpObj->pInventory[target].m_LuckOption,
			lpObj->pInventory[target].m_Z28Option,
			lpObj->pInventory[target].m_NewOption,
			lpObj->pInventory[target].m_SetOption,
			lpObj->pInventory[target].m_ItemOptionEx,
			CURRENT_DB_VERSION);

	if(g_kJewelOfHarmonySystem.IsStrengthenByJewelOfHarmony(&lpObj->pInventory[target])== 1)
	{
		if(g_kJewelOfHarmonySystem.IsActive(&lpObj->pInventory[target]) == 0)
		{
			GCServerMsgStringSend(lMsg.Get(3370),lpObj->m_Index,1);
		}
	}

	return true;
}

#endif
