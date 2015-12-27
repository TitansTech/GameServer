// ItemAddOption.cpp: implementation of the CItemAddOption class.
//	GS-N	1.00.18	JPN	0x00508C40	-	Completed
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ItemAddOption.h"
#include "GameMain.h"
#include "LogProc.h"
#include "ObjCalCharacter.h"
#include "Mastering.h"
#include "ObjUseSkill.h"
#include "..\include\Readscript.h"


CItemAddOption g_ItemAddOption;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CItemAddOption::CItemAddOption()
{
	this->Initialize();
}

CItemAddOption::~CItemAddOption()
{
	return;
}



void CItemAddOption::Load(PCHAR chFileName)
{
	SMDToken Token;

	this->Initialize();

	SMDFile = fopen(chFileName, "r");

	if ( SMDFile == NULL )
		return;

	int iItemAddOptionIndex = 0;

	while ( true )
	{
		Token = (SMDToken)GetToken();

		if ( Token == END )
			break;

		if ( Token == NUMBER )
		{
			int iType = TokenNumber;

			while ( true )
			{
				if ( iType == 0 )
				{
					Token = (SMDToken)GetToken();

					if ( strcmp("end", TokenString) == 0 )
						break;
				}

				if ( iType == 1 )
				{
					int iItemNumber = 0;
					int iItemType = 0;
					int iItemIndex = 0;
					int iItemOption1 = 0;
					int iItemValue1 = 0;
					int iItemOption2 = 0;
					int iItemValue2 = 0;
					int iItemValidTime = 0;

					Token = (SMDToken)GetToken();

					if ( !strcmp("end", TokenString))
						break;

					iItemType = TokenNumber;

					Token = (SMDToken)GetToken();
					iItemIndex = TokenNumber;

					Token = (SMDToken)GetToken();
					iItemOption1 = TokenNumber;					

					Token = (SMDToken)GetToken();
					iItemValue1 = TokenNumber;

					Token = (SMDToken)GetToken();
					iItemOption2 = TokenNumber;

					Token = (SMDToken)GetToken();
					iItemValue2 = TokenNumber;

					Token = (SMDToken)GetToken();
					iItemValidTime = TokenNumber;

					iItemNumber = ITEMGET(iItemType, iItemIndex);
					this->m_ItemAddOption[iItemAddOptionIndex].wOptionType = iType;
					this->m_ItemAddOption[iItemAddOptionIndex].iItemNumber = iItemNumber;
					this->m_ItemAddOption[iItemAddOptionIndex].iItemType = iItemType;
					this->m_ItemAddOption[iItemAddOptionIndex].iItemIndex = iItemIndex;
					this->m_ItemAddOption[iItemAddOptionIndex].wEffectType1 = iItemOption1;
					this->m_ItemAddOption[iItemAddOptionIndex].iEffectValue1 = iItemValue1;
					this->m_ItemAddOption[iItemAddOptionIndex].wEffectType2 = iItemOption2;
					this->m_ItemAddOption[iItemAddOptionIndex].iEffectValue2 = iItemValue2;
					this->m_ItemAddOption[iItemAddOptionIndex].iEffectValidTime = iItemValidTime;

					LogAddTD("[ItemAddOption][Load][1] %d,%d [%d,%d,%d,%d,%d]", 
						this->m_ItemAddOption[iItemAddOptionIndex].iItemType,
						this->m_ItemAddOption[iItemAddOptionIndex].iItemIndex,
						this->m_ItemAddOption[iItemAddOptionIndex].wEffectType1,
						this->m_ItemAddOption[iItemAddOptionIndex].iEffectValue1,
						this->m_ItemAddOption[iItemAddOptionIndex].wEffectType2,
						this->m_ItemAddOption[iItemAddOptionIndex].iEffectValue2,
						this->m_ItemAddOption[iItemAddOptionIndex].iEffectValidTime
					);

					iItemAddOptionIndex++;

					if (iItemAddOptionIndex > MAX_ITEM_ADD_OPTION )
					{
						MsgBox("[ItemAddOption] Overflow ItemAddOption Table Index");
					}
				}
				else if ( iType == 2 )
				{
					int iItemNumber = 0;
					int iItemType = 0;
					int iItemIndex = 0;
					int iItemOption1 = 0;
					int iItemValue1 = 0;
					int iItemOption2 = 0;
					int iItemValue2 = 0;
					int iItemValidTime = 0;

					Token = (SMDToken)GetToken();

					if ( !strcmp("end", TokenString))
						break;

					iItemType = TokenNumber;

					Token = (SMDToken)GetToken();
					iItemIndex = TokenNumber;

					Token = (SMDToken)GetToken();
					iItemOption1 = TokenNumber;					

					Token = (SMDToken)GetToken();
					iItemValue1 = TokenNumber;

					Token = (SMDToken)GetToken();
					iItemOption2 = TokenNumber;

					Token = (SMDToken)GetToken();
					iItemValue2 = TokenNumber;

					Token = (SMDToken)GetToken();
					iItemValidTime = TokenNumber;

					iItemNumber = ITEMGET(iItemType, iItemIndex);
					this->m_ItemAddOption[iItemAddOptionIndex].wOptionType = iType;
					this->m_ItemAddOption[iItemAddOptionIndex].iItemNumber = iItemNumber;
					this->m_ItemAddOption[iItemAddOptionIndex].iItemType = iItemType;
					this->m_ItemAddOption[iItemAddOptionIndex].iItemIndex = iItemIndex;
					this->m_ItemAddOption[iItemAddOptionIndex].wEffectType1 = iItemOption1;
					this->m_ItemAddOption[iItemAddOptionIndex].iEffectValue1 = iItemValue1;
					this->m_ItemAddOption[iItemAddOptionIndex].wEffectType2 = iItemOption2;
					this->m_ItemAddOption[iItemAddOptionIndex].iEffectValue2 = iItemValue2;
					this->m_ItemAddOption[iItemAddOptionIndex].iEffectValidTime = iItemValidTime;

					LogAddTD("[ItemAddOption][Load][2] %d,%d [%d,%d,%d,%d,%d]", 
						this->m_ItemAddOption[iItemAddOptionIndex].iItemType,
						this->m_ItemAddOption[iItemAddOptionIndex].iItemIndex,
						this->m_ItemAddOption[iItemAddOptionIndex].wEffectType1,
						this->m_ItemAddOption[iItemAddOptionIndex].iEffectValue1,
						this->m_ItemAddOption[iItemAddOptionIndex].wEffectType2,
						this->m_ItemAddOption[iItemAddOptionIndex].iEffectValue2,
						this->m_ItemAddOption[iItemAddOptionIndex].iEffectValidTime
					);

					iItemAddOptionIndex++;

					if (iItemAddOptionIndex > MAX_ITEM_ADD_OPTION )
					{
						MsgBox("[ItemAddOption] Overflow ItemAddOption Table Index");
					}
				}
			}
		}
	}

	fclose(SMDFile);//->FIX
	// #error need fclose here
}



void CItemAddOption::Initialize()
{
	for ( int i=0;i<MAX_ITEM_ADD_OPTION;i++)
	{
		this->m_ItemAddOption[i].iItemNumber = 0;
		this->m_ItemAddOption[i].iItemType = 0;
		this->m_ItemAddOption[i].iItemIndex = 0;
		this->m_ItemAddOption[i].wEffectType1 = 0;
		this->m_ItemAddOption[i].iEffectValue1 = 0;
		this->m_ItemAddOption[i].wEffectType2 = 0;
		this->m_ItemAddOption[i].iEffectValue2 = 0;
		this->m_ItemAddOption[i].iEffectValidTime = 0;
	}
}

bool CItemAddOption::ApplyInventoryItemsStats(LPOBJ lpObj)
{
	//DebugLog("%s START",__FUNCTION__);
	int iItemIndex = 0;

	for ( iItemIndex = 0; iItemIndex < INVETORY_WEAR_SIZE ; iItemIndex++)
	{
		CItem * pItem = &lpObj->pInventory[iItemIndex];

		if ( pItem->IsItem() && pItem->m_IsValidItem)
		{
			LPITEMEFFECT lpItemEffect = NULL;

			lpItemEffect = this->_SearchItemEffect(pItem->m_Type);

			if ( lpItemEffect != NULL )
			{
				if (lpItemEffect->wEffectType1 == ADD_OPTION_STRENGTH ||
					lpItemEffect->wEffectType1 == ADD_OPTION_DEXTERITY ||
					lpItemEffect->wEffectType1 == ADD_OPTION_VITALITY ||
					lpItemEffect->wEffectType1 == ADD_OPTION_ENERGY ||
					lpItemEffect->wEffectType1 == ADD_OPTION_LEADERSHIP		
					)
				{
					this->_SetItemEffect(lpObj, lpItemEffect->wEffectType1, lpItemEffect->iEffectValue1);
				}

				if (lpItemEffect->wEffectType2 == ADD_OPTION_STRENGTH ||
					lpItemEffect->wEffectType2 == ADD_OPTION_DEXTERITY ||
					lpItemEffect->wEffectType2 == ADD_OPTION_VITALITY ||
					lpItemEffect->wEffectType2 == ADD_OPTION_ENERGY ||
					lpItemEffect->wEffectType2 == ADD_OPTION_LEADERSHIP		
					)
				{
					this->_SetItemEffect(lpObj, lpItemEffect->wEffectType2, lpItemEffect->iEffectValue2);
				}
			}
		}
	}

	return TRUE;
}

bool CItemAddOption::ApplyInventoryItems(LPOBJ lpObj)
{
	//DebugLog("%s START",__FUNCTION__);
	int iItemIndex = 0;

	for ( iItemIndex = 0; iItemIndex < INVETORY_WEAR_SIZE ; iItemIndex++)
	{
		CItem * pItem = &lpObj->pInventory[iItemIndex];

		if ( pItem->IsItem() && pItem->m_IsValidItem)
		{
			LPITEMEFFECT lpItemEffect = NULL;

			lpItemEffect = this->_SearchItemEffect(pItem->m_Type);

			if ( lpItemEffect != NULL )
			{
				if (lpItemEffect->wEffectType1 != ADD_OPTION_STRENGTH &&
					lpItemEffect->wEffectType1 != ADD_OPTION_DEXTERITY &&
					lpItemEffect->wEffectType1 != ADD_OPTION_VITALITY &&
					lpItemEffect->wEffectType1 != ADD_OPTION_ENERGY &&
					lpItemEffect->wEffectType1 != ADD_OPTION_LEADERSHIP		
					)
				{
					this->_SetItemEffect(lpObj, lpItemEffect->wEffectType1, lpItemEffect->iEffectValue1);
				}

				if (lpItemEffect->wEffectType2 != ADD_OPTION_STRENGTH &&
					lpItemEffect->wEffectType2 != ADD_OPTION_DEXTERITY &&
					lpItemEffect->wEffectType2 != ADD_OPTION_VITALITY &&
					lpItemEffect->wEffectType2 != ADD_OPTION_ENERGY &&
					lpItemEffect->wEffectType2 != ADD_OPTION_LEADERSHIP		
					)
				{
					this->_SetItemEffect(lpObj, lpItemEffect->wEffectType2, lpItemEffect->iEffectValue2);
				}
			}
			//for ( int i=0;i<MAX_ITEM_ADD_OPTION;i++)
			//{
				//if (this->m_ItemAddOption[i].iItemNumber == pItem->m_Type)
				//{
				//	this->_SetItemEffect(lpObj, this->m_ItemAddOption[i].wEffectType1, this->m_ItemAddOption[i].iEffectValue1);
				//	this->_SetItemEffect(lpObj, this->m_ItemAddOption[i].wEffectType2, this->m_ItemAddOption[i].iEffectValue2);
				//}
			//}			
		}
	}

	//DebugLog("%s END",__FUNCTION__);
	return TRUE;
}

bool CItemAddOption::SetItemLastEffectStats(LPOBJ lpObj, WORD wOptionType)
{
	LPITEMEFFECT lpItemEffect = NULL;
	int iItemEffectType = lpObj->m_wItemEffectType[wOptionType];

	lpItemEffect = this->_SearchItemEffect(iItemEffectType);

	if ( lpItemEffect == NULL )
		return false;

	if (lpItemEffect->wEffectType1 == ADD_OPTION_STRENGTH || 
		lpItemEffect->wEffectType1 == ADD_OPTION_VITALITY || 
		lpItemEffect->wEffectType1 == ADD_OPTION_ENERGY || 
		lpItemEffect->wEffectType1 == ADD_OPTION_LEADERSHIP || 
		lpItemEffect->wEffectType1 == ADD_OPTION_DEXTERITY )
	{
		this->_SetItemEffect(lpObj, lpItemEffect->wEffectType1, lpItemEffect->iEffectValue1);
	}
	if (lpItemEffect->wEffectType2 == ADD_OPTION_STRENGTH || 
		lpItemEffect->wEffectType1 == ADD_OPTION_VITALITY || 
		lpItemEffect->wEffectType1 == ADD_OPTION_ENERGY || 
		lpItemEffect->wEffectType1 == ADD_OPTION_LEADERSHIP || 
		lpItemEffect->wEffectType2 == ADD_OPTION_DEXTERITY )
	{
		this->_SetItemEffect(lpObj, lpItemEffect->wEffectType2, lpItemEffect->iEffectValue2);
	}

	return true;
}

bool CItemAddOption::SetItemLastEffectLifeMana(LPOBJ lpObj, WORD wOptionType)
{
	LPITEMEFFECT lpItemEffect = NULL;
	int iItemEffectType = lpObj->m_wItemEffectType[wOptionType];

	lpItemEffect = this->_SearchItemEffect(iItemEffectType);

	if ( lpItemEffect == NULL )
		return false;

	if (lpItemEffect->wEffectType1 == ADD_OPTION_LIFE || 
		lpItemEffect->wEffectType1 == ADD_OPTION_MANA )
	{
		this->_SetItemEffect(lpObj, lpItemEffect->wEffectType1, lpItemEffect->iEffectValue1);
	}
	if (lpItemEffect->wEffectType2 == ADD_OPTION_LIFE || 
		lpItemEffect->wEffectType2 == ADD_OPTION_MANA )
	{
		this->_SetItemEffect(lpObj, lpItemEffect->wEffectType2, lpItemEffect->iEffectValue2);
	}

	return true;
}

bool CItemAddOption::SetItemLastEffectOther(LPOBJ lpObj, WORD wOptionType)
{
	LPITEMEFFECT lpItemEffect = NULL;
	int iItemEffectType = lpObj->m_wItemEffectType[wOptionType];

	lpItemEffect = this->_SearchItemEffect(iItemEffectType);

	if ( lpItemEffect == NULL )
		return false;

	if (lpItemEffect->wEffectType1 != ADD_OPTION_LIFE && 
		lpItemEffect->wEffectType1 != ADD_OPTION_MANA &&
		lpItemEffect->wEffectType1 != ADD_OPTION_STRENGTH &&
		lpItemEffect->wEffectType1 != ADD_OPTION_DEXTERITY &&
		lpItemEffect->wEffectType1 != ADD_OPTION_VITALITY &&
		lpItemEffect->wEffectType1 != ADD_OPTION_ENERGY &&
		lpItemEffect->wEffectType1 != ADD_OPTION_LEADERSHIP		
		)
	{
		this->_SetItemEffect(lpObj, lpItemEffect->wEffectType1, lpItemEffect->iEffectValue1);
	}

	if (lpItemEffect->wEffectType2 != ADD_OPTION_LIFE && 
		lpItemEffect->wEffectType2 != ADD_OPTION_MANA &&
		lpItemEffect->wEffectType2 != ADD_OPTION_STRENGTH &&
		lpItemEffect->wEffectType2 != ADD_OPTION_DEXTERITY &&
		lpItemEffect->wEffectType2 != ADD_OPTION_VITALITY &&
		lpItemEffect->wEffectType2 != ADD_OPTION_ENERGY &&
		lpItemEffect->wEffectType2 != ADD_OPTION_LEADERSHIP		
		)
	{
		this->_SetItemEffect(lpObj, lpItemEffect->wEffectType2, lpItemEffect->iEffectValue2);
	}

	return true;
}


void CItemAddOption::SendItemUse(LPOBJ lpObj, int iItemNumber, int iOptionType, int iEffectType, int iEffectValue, int iValidTime)
{
	if ( lpObj == NULL ) 
		return;
	
	if ( iItemNumber== 0 || iEffectType == 0 ) 
		return;

	GCUseEffectItem(lpObj, 0, iOptionType, iEffectType, iValidTime);
}


bool CItemAddOption::SetItemEffect(LPOBJ lpObj, int iItemNumber, int iItemUseTime)
{
	LPITEMEFFECT lpItemEffectBefore = NULL;
	LPITEMEFFECT lpItemEffect = NULL;

	if ( lpObj == NULL )
		return false;

	if ( lpObj->Type != OBJ_USER )
		return false;

	lpItemEffect = this->_SearchItemEffect(iItemNumber);

	if ( lpItemEffect ==NULL )
		return false;

	WORD wOptionType = (lpItemEffect->wOptionType-1);

	lpItemEffectBefore = this->GetCurrentEffectType(lpObj,wOptionType);

	if ( lpItemEffectBefore != NULL )
	{
		if ( lpItemEffectBefore->wOptionType == lpItemEffect->wOptionType )
		{
			this->ClearItemEffect(lpObj, 2, (lpItemEffectBefore->wOptionType-1));
		}
	}

	if ( iItemUseTime > 0 )
	{
		this->_SetItemEffect(lpObj, lpItemEffect->wEffectType1, lpItemEffect->iEffectValue1);
		this->SendItemUse(lpObj, iItemNumber, lpItemEffect->wOptionType, lpItemEffect->wEffectType1,
			lpItemEffect->iEffectValue1, iItemUseTime * 60);

		this->_SetItemEffect(lpObj, lpItemEffect->wEffectType2, lpItemEffect->iEffectValue2);
		this->SendItemUse(lpObj, iItemNumber, lpItemEffect->wOptionType, lpItemEffect->wEffectType2,
			lpItemEffect->iEffectValue2, iItemUseTime * 60);
	}
	else
	{
		this->_SetItemEffect(lpObj, lpItemEffect->wEffectType1, lpItemEffect->iEffectValue1);
		this->SendItemUse(lpObj, iItemNumber, lpItemEffect->wOptionType, lpItemEffect->wEffectType1,
			lpItemEffect->iEffectValue1, lpItemEffect->iEffectValidTime);

		this->_SetItemEffect(lpObj, lpItemEffect->wEffectType2, lpItemEffect->iEffectValue2);
		this->SendItemUse(lpObj, iItemNumber, lpItemEffect->wOptionType, lpItemEffect->wEffectType2,
			lpItemEffect->iEffectValue2, lpItemEffect->iEffectValidTime);
	}

	if ( iItemUseTime > 0 )
	{
		lpObj->m_wItemEffectType[wOptionType] = lpItemEffect->iItemNumber;
		lpObj->m_iItemEffectValidTime[wOptionType] = iItemUseTime * 60;
	}
	else
	{
		lpObj->m_wItemEffectType[wOptionType] = lpItemEffect->iItemNumber;
		lpObj->m_iItemEffectValidTime[wOptionType] = lpItemEffect->iEffectValidTime;
	}

	this->ItemShowEffect(lpObj,1,wOptionType);

	return true;
}

int CItemAddOption::GetItemEffect(int Item)
{
	switch(Item)
	{
		case (ITEMGET(13,43)):
		{
			return 29;
		}break;
		case (ITEMGET(13,44)):
		{
			return 30;
		}break;
		case (ITEMGET(13,45)):
		{
			return 31;
		}break;
		case (ITEMGET(13,62)):
		{
			return 87;
		}break;
		case (ITEMGET(13,63)):
		{
			return 88;
		}break;
		case (ITEMGET(13,93)):
		{
			return 101;
		}break;
		case (ITEMGET(13,94)):
		{
			return 102;
		}break;
		case (ITEMGET(13,96)):
		{
			return 104;
		}break;
		case (ITEMGET(13,103)):
		{
			return 112;
		}break;
		case (ITEMGET(13,104)):
		{
			return 113;
		}break;
		case (ITEMGET(13,105)):
		{
			return 114;
		}break;
		case (ITEMGET(13,116)):
		{
			return 119;
		}break;

		case (ITEMGET(14,46)):
		{
			return 35;
		}break;
		case (ITEMGET(14,47)):
		{
			return 36;
		}break;
		case (ITEMGET(14,48)):
		{
			return 37;
		}break;
		case (ITEMGET(14,49)):
		{
			return 38;
		}break;
		case (ITEMGET(14,50)):
		{
			return 39;
		}break;
		case (ITEMGET(14,85)):
		{
			return 78;
		}break;
		case (ITEMGET(14,86)):
		{
			return 79;
		}break;
		case (ITEMGET(14,87)):
		{
			return 80;
		}break;
		
		case (ITEMGET(13,81)):
		case (ITEMGET(13,80)):
		{
			BYTE Val = (Item - ITEMGET(13,81)) + 99;
			return Val;
		}break;
		
		case (ITEMGET(13,128)):
		case (ITEMGET(13,129)):
		case (ITEMGET(13,130)):
		case (ITEMGET(13,131)):
		case (ITEMGET(13,132)):
		case (ITEMGET(13,133)):
		case (ITEMGET(13,134)):
		{
			BYTE Val = (Item - ITEMGET(13,128)) + 122;
			return Val;
		}break;
		
		case (ITEMGET(14,72)):
		case (ITEMGET(14,73)):
		case (ITEMGET(14,74)):
		case (ITEMGET(14,75)):
		case (ITEMGET(14,76)):
		case (ITEMGET(14,77)):
		case (ITEMGET(14,78)):
		case (ITEMGET(14,79)):
		case (ITEMGET(14,80)):
		case (ITEMGET(14,81)):
		case (ITEMGET(14,82)):
		{
			BYTE Val = (Item - ITEMGET(14,72)) + 44;
			return Val;
		}break;

		case (ITEMGET(14,97)):
		case (ITEMGET(14,98)):
		{
			BYTE Val = (Item - ITEMGET(14,97)) + 89;
			return Val;
		}break;
	}
	return -1;
}

void CItemAddOption::ItemShowEffect(LPOBJ lpObj,int Enabled, WORD wOptionType)
{
	switch(lpObj->m_wItemEffectType[wOptionType])
	{
		case (ITEMGET(13,43)):
		{
			GCSkillInfoSend(lpObj,Enabled,29);
		}break;
		case (ITEMGET(13,44)):
		{
			GCSkillInfoSend(lpObj,Enabled,30);
		}break;
		case (ITEMGET(13,45)):
		{
			GCSkillInfoSend(lpObj,Enabled,31);
		}break;
		case (ITEMGET(13,62)):
		{
			GCSkillInfoSend(lpObj,Enabled,87);
		}break;
		case (ITEMGET(13,63)):
		{
			GCSkillInfoSend(lpObj,Enabled,88);
		}break;
		case (ITEMGET(13,93)):
		{
			GCSkillInfoSend(lpObj,Enabled,101);
		}break;
		case (ITEMGET(13,94)):
		{
			GCSkillInfoSend(lpObj,Enabled,102);
		}break;
		case (ITEMGET(13,96)):
		{
			GCSkillInfoSend(lpObj,Enabled,104);
		}break;
		case (ITEMGET(13,103)):
		{
			GCSkillInfoSend(lpObj,Enabled,112);
		}break;
		case (ITEMGET(13,104)):
		{
			GCSkillInfoSend(lpObj,Enabled,113);
		}break;
		case (ITEMGET(13,105)):
		{
			GCSkillInfoSend(lpObj,Enabled,114);
		}break;
		case (ITEMGET(13,116)):
		{
			GCSkillInfoSend(lpObj,Enabled,119);
		}break;

		case (ITEMGET(14,46)):
		{
			GCSkillInfoSend(lpObj,Enabled,35);
		}break;
		case (ITEMGET(14,47)):
		{
			GCSkillInfoSend(lpObj,Enabled,36);
		}break;
		case (ITEMGET(14,48)):
		{
			GCSkillInfoSend(lpObj,Enabled,37);
		}break;
		case (ITEMGET(14,49)):
		{
			GCSkillInfoSend(lpObj,Enabled,38);
		}break;
		case (ITEMGET(14,50)):
		{
			GCSkillInfoSend(lpObj,Enabled,39);
		}break;
		case (ITEMGET(14,85)):
		{
			GCSkillInfoSend(lpObj,Enabled,78);
		}break;
		case (ITEMGET(14,86)):
		{
			GCSkillInfoSend(lpObj,Enabled,79);
		}break;
		case (ITEMGET(14,87)):
		{
			GCSkillInfoSend(lpObj,Enabled,80);
		}break;
		
		case (ITEMGET(13,81)):
		case (ITEMGET(13,80)):
		{
			BYTE Val = (lpObj->m_wItemEffectType[wOptionType] - ITEMGET(13,81)) + 99;
			GCSkillInfoSend(lpObj,Enabled,Val);
		}break;
		
		case (ITEMGET(13,128)):
		case (ITEMGET(13,129)):
		case (ITEMGET(13,130)):
		case (ITEMGET(13,131)):
		case (ITEMGET(13,132)):
		case (ITEMGET(13,133)):
		case (ITEMGET(13,134)):
		{
			BYTE Val = (lpObj->m_wItemEffectType[wOptionType] - ITEMGET(13,128)) + 122;
			GCSkillInfoSend(lpObj,Enabled,Val);
		}break;
		
		case (ITEMGET(14,72)):
		case (ITEMGET(14,73)):
		case (ITEMGET(14,74)):
		case (ITEMGET(14,75)):
		case (ITEMGET(14,76)):
		case (ITEMGET(14,77)):
		case (ITEMGET(14,78)):
		case (ITEMGET(14,79)):
		case (ITEMGET(14,80)):
		case (ITEMGET(14,81)):
		case (ITEMGET(14,82)):
		{
			BYTE Val = (lpObj->m_wItemEffectType[wOptionType] - ITEMGET(14,72)) + 44;
			GCSkillInfoSend(lpObj,Enabled,Val);
		}break;

		case (ITEMGET(14,97)):
		case (ITEMGET(14,98)):
		{
			BYTE Val = (lpObj->m_wItemEffectType[wOptionType] - ITEMGET(14,97)) + 89;
			GCSkillInfoSend(lpObj,Enabled,Val);
		}break;
	}
}

bool CItemAddOption::_SetItemEffect(LPOBJ lpObj, int iEffectType, int iEffectValue)
{
	//DebugLog("%s START %d %d",__FUNCTION__,iEffectType,iEffectValue);
	if ( iEffectType <= 0 )
		return false;

	switch ( iEffectType )
	{
		case ADD_OPTION_LIFE:
			lpObj->AddLife += iEffectValue;

			gObjFixGagesOverflow(lpObj);

			GCReFillSend(lpObj->m_Index, lpObj->MaxLife + lpObj->AddLife, -2, 0, lpObj->iMaxShield + lpObj->iAddShield);
			GCReFillSend(lpObj->m_Index, lpObj->Life, -1, 0, lpObj->iShield);
		break;

		case ADD_OPTION_MANA:
			lpObj->AddMana += iEffectValue;

			gObjFixGagesOverflow(lpObj);

			GCManaSend(lpObj->m_Index, lpObj->AddMana + lpObj->MaxMana, -2, 0, lpObj->MaxBP + lpObj->AddBP);
			GCManaSend(lpObj->m_Index, lpObj->Mana, -1, 0, lpObj->BP);
		break;

		case  ADD_OPTION_INCREASE_AG_PERCENT:
			lpObj->AddBP += (lpObj->MaxBP + lpObj->AddBP)*iEffectValue / 100;

			gObjFixGagesOverflow(lpObj);

			GCManaSend(lpObj->m_Index, lpObj->AddMana + lpObj->MaxMana, -2, 0, lpObj->MaxBP + lpObj->AddBP);
			GCManaSend(lpObj->m_Index, lpObj->Mana, -1, 0, lpObj->BP);
		break;

		case  ADD_OPTION_INCREASE_SD_PERCENT:
			lpObj->iAddShield += (lpObj->iMaxShield + lpObj->iAddShield)*iEffectValue / 100;

			gObjFixGagesOverflow(lpObj);

			GCReFillSend(lpObj->m_Index, lpObj->MaxLife + lpObj->AddLife, -2, 0, lpObj->iMaxShield + lpObj->iAddShield);
			GCReFillSend(lpObj->m_Index, lpObj->Life, -1, 0, lpObj->iShield);
		break;

		case ADD_OPTION_ATTACK_DAMAGE:
			//lpObj->m_AttackDamageMax += iEffectValue;
			//lpObj->m_AttackDamageMin += iEffectValue;
			lpObj->m_AttackDamageMaxLeft += iEffectValue;
			lpObj->m_AttackDamageMinLeft += iEffectValue;
			lpObj->m_AttackDamageMaxRight += iEffectValue;
			lpObj->m_AttackDamageMinRight += iEffectValue;

			lpObj->m_MagicDamageMin += iEffectValue;
			lpObj->m_MagicDamageMax += iEffectValue;
		break;

		case ADD_OPTION_SPEED:
			lpObj->m_AttackSpeed += iEffectValue;
			lpObj->m_MagicSpeed += iEffectValue;
		break;

		case ADD_OPTION_EXPERIENCE_PERCENT:
			lpObj->m_wExprienceRate += (iEffectValue-100);
		break;

		case ADD_OPTION_DROP_RATE_PERCENT:
			lpObj->m_wItemDropRate += (iEffectValue-100);
		break;

		case ADD_OPTION_REMOVE_EXTRA_EXPERIENCE:
			lpObj->m_btDisableExpGain = 1;
			lpObj->m_wExprienceRate = 100;

			if(lpObj->m_SkillGladiatorsGloryTime > 0)
			{
				lpObj->m_SkillGladiatorsGloryTime = 0;
				gObjUseSkill.GladiatorsGlory(eVS_DUEL_MEDAL, lpObj->m_Index, FALSE);
			}
		break;

		case ADD_OPTION_DEFENSE:
			lpObj->m_Defense += iEffectValue * 10 / 20;
		break;

		case ADD_OPTION_STRENGTH:
			lpObj->AddStrength += iEffectValue;
		break;

		case ADD_OPTION_DEXTERITY:
			lpObj->AddDexterity += iEffectValue;
		break;

		case ADD_OPTION_VITALITY:
			lpObj->AddVitality += iEffectValue;
		break;

		case ADD_OPTION_ENERGY:
			lpObj->AddEnergy += iEffectValue;
		break;

		case ADD_OPTION_LEADERSHIP:
			lpObj->AddLeadership += iEffectValue;
		break;

		case ADD_OPTION_ATTACKPOWER_PERCENT:
			lpObj->m_AttackDamageMin += lpObj->m_AttackDamageMin * iEffectValue / 100;
			lpObj->m_AttackDamageMax += lpObj->m_AttackDamageMax * iEffectValue / 100;
			lpObj->m_AttackDamageMinRight += lpObj->m_AttackDamageMinRight * iEffectValue / 100;
			lpObj->m_AttackDamageMaxRight += lpObj->m_AttackDamageMaxRight * iEffectValue / 100;
			lpObj->m_AttackDamageMinLeft += lpObj->m_AttackDamageMinLeft * iEffectValue / 100;
			lpObj->m_AttackDamageMaxLeft += lpObj->m_AttackDamageMaxLeft * iEffectValue / 100;
			lpObj->m_MagicDamageMin += lpObj->m_MagicDamageMin * iEffectValue / 100;
			lpObj->m_MagicDamageMax += lpObj->m_MagicDamageMax * iEffectValue / 100;
		break;

		case ADD_OPTION_DMG_ABSORB_PERCENT:
			lpObj->DamageMinus += iEffectValue;
		break;

		case ADD_OPTION_EXPERIENCE_RATE:
			lpObj->m_wExprienceRate += (((gAddExperience+iEffectValue/gAddExperience)*100)-100);
		break;
	
		case ADD_OPTION_DROP_RATE_1000_PERCENT:
			lpObj->m_wItemDropRate += (1000*iEffectValue/100);
		break;

		default:
			return false;
		break;
	}

	//DebugLog("%s END",__FUNCTION__);
	return true;
}

//GetOptionValueForItem

bool CItemAddOption::_ClearItemEffect(LPOBJ lpObj, int iEffectOption, int iOptionType, int iEffectType, int iEffectValue)
{
	if ( iEffectType <= 0 )
		return false;

	switch ( iEffectType )
	{
		case ADD_OPTION_LIFE:
			//Will be automatically recalculated after in gObjCalCharacter
			//lpObj->AddLife -= iEffectValue;

			//gObjFixGagesOverflow(lpObj);

			//GCReFillSend(lpObj->m_Index, lpObj->MaxLife + lpObj->AddLife, -2, 0, lpObj->iMaxShield + lpObj->iAddShield);
			//GCReFillSend(lpObj->m_Index, lpObj->Life, -1, 0, lpObj->iShield);
		break;

		case ADD_OPTION_MANA:
			//Will be automatically recalculated after in gObjCalCharacter
			//lpObj->AddMana -= iEffectValue;

			//gObjFixGagesOverflow(lpObj);

			//GCManaSend(lpObj->m_Index, lpObj->AddMana + lpObj->MaxMana, -2, 0, lpObj->MaxBP + lpObj->AddBP);
			//GCManaSend(lpObj->m_Index, lpObj->Mana, -1, 0, lpObj->BP);
		break;

		case  ADD_OPTION_INCREASE_AG_PERCENT:
			//Will be automatically recalculated after in gObjCalCharacter
		break;

		case  ADD_OPTION_INCREASE_SD_PERCENT:
			//Will be automatically recalculated after in gObjCalCharacter
		break;

		case ADD_OPTION_ATTACK_DAMAGE:
			//Will be automatically recalculated after in gObjCalCharacter
			//lpObj->m_AttackDamageMaxLeft -= iEffectValue;
			//lpObj->m_AttackDamageMinLeft -= iEffectValue;
			//lpObj->m_AttackDamageMaxRight -= iEffectValue;
			//lpObj->m_AttackDamageMinRight -= iEffectValue;
			//lpObj->m_MagicDamageMin -= iEffectValue;
			//lpObj->m_MagicDamageMax -= iEffectValue;
		break;

		case ADD_OPTION_SPEED:
			//Will be automatically recalculated after in gObjCalCharacter
			//lpObj->m_AttackSpeed -= iEffectValue;
			//lpObj->m_MagicSpeed -= iEffectValue;
		break;

		case ADD_OPTION_EXPERIENCE_PERCENT:
			//Will be automatically recalculated after in gObjCalCharacter
			//lpObj->m_wExprienceRate -= (iEffectValue-100);
		break;

		case ADD_OPTION_DROP_RATE_PERCENT:
			//Will be automatically recalculated after in gObjCalCharacter
			//lpObj->m_wItemDropRate -= (iEffectValue-100);
		break;

		case ADD_OPTION_REMOVE_EXTRA_EXPERIENCE:
			//Will be automatically recalculated after in gObjCalCharacter
			//lpObj->m_wExprienceRate = 100;
			//lpObj->m_btDisableExpGain = 0;
		break;

		case ADD_OPTION_DEFENSE:
			//Will be automatically recalculated after in gObjCalCharacter
			//lpObj->m_Defense -= iEffectValue * 10 / 20;
		break;

		case ADD_OPTION_STRENGTH:
			//Will be automatically recalculated after in gObjCalCharacter
			//lpObj->AddStrength -= iEffectValue;
		break;

		case ADD_OPTION_DEXTERITY:
			//Will be automatically recalculated after in gObjCalCharacter
			//lpObj->AddDexterity -= iEffectValue;
		break;

		case ADD_OPTION_VITALITY:
			//Will be automatically recalculated after in gObjCalCharacter
			//lpObj->AddVitality -= iEffectValue;
		break;

		case ADD_OPTION_ENERGY:
			//Will be automatically recalculated after in gObjCalCharacter
			//lpObj->AddEnergy -= iEffectValue;
		break;

		case ADD_OPTION_LEADERSHIP:
			//Will be automatically recalculated after in gObjCalCharacter
			//lpObj->AddLeadership -= iEffectValue;
		break;

		case ADD_OPTION_ATTACKPOWER_PERCENT:
			//Will be automatically recalculated after in gObjCalCharacter
			//lpObj->m_AttackDamageMinRight -= lpObj->m_AttackDamageMinRight * iEffectValue / 100;
			//lpObj->m_AttackDamageMaxRight -= lpObj->m_AttackDamageMaxRight * iEffectValue / 100;
			//lpObj->m_AttackDamageMinLeft -= lpObj->m_AttackDamageMinLeft * iEffectValue / 100;
			//lpObj->m_AttackDamageMaxLeft -= lpObj->m_AttackDamageMaxLeft * iEffectValue / 100;
			//lpObj->m_MagicDamageMin -= lpObj->m_MagicDamageMin * iEffectValue / 100;
			//lpObj->m_MagicDamageMax -= lpObj->m_MagicDamageMax * iEffectValue / 100;
		break;

		case ADD_OPTION_DMG_ABSORB_PERCENT:
			//Will be automatically recalculated after in gObjCalCharacter
			//lpObj->DamageMinus -= iEffectValue;
		break;

		case ADD_OPTION_EXPERIENCE_RATE:
			//Will be automatically recalculated after in gObjCalCharacter
			//lpObj->m_wExprienceRate -= (((gAddExperience+iEffectValue/gAddExperience)*100)-100);
		break;
	
		case ADD_OPTION_DROP_RATE_1000_PERCENT:
			//Will be automatically recalculated after in gObjCalCharacter
			//lpObj->m_wItemDropRate -= (1000*iEffectValue/100);
		break;

		default:
			return false;
		break;
	}

	GCUseEffectItem(lpObj, iEffectOption, iOptionType, iEffectType, 0);

	return true;
}



bool CItemAddOption::ClearItemEffect(LPOBJ lpObj, int iEffectOption, WORD wOptionType)
{
	LPITEMEFFECT lpItemEffect = NULL;
	
	lpItemEffect = this->GetCurrentEffectType(lpObj,wOptionType);

	if ( lpItemEffect == NULL )
		return false;

	this->_ClearItemEffect(lpObj, iEffectOption, lpItemEffect->wOptionType,
		lpItemEffect->wEffectType1, lpItemEffect->iEffectValue1);

	this->_ClearItemEffect(lpObj, iEffectOption, lpItemEffect->wOptionType,
		lpItemEffect->wEffectType2, lpItemEffect->iEffectValue2);

	this->ItemShowEffect(lpObj,0,wOptionType);

	if(lpObj->m_wItem_MuShop_Seal_EffectType == lpObj->m_wItemEffectType[wOptionType])
	{
		lpObj->m_wItem_MuShop_Seal_EffectType = 0;
		lpObj->m_iItem_MuShop_Seal_EffectValidTime = 0;
		lpObj->PlayerExtraInfoModified = 1;
	}else if(lpObj->m_wItem_MuShop_Scroll_EffectType == lpObj->m_wItemEffectType[wOptionType])
	{
		lpObj->m_wItem_MuShop_Scroll_EffectType = 0;
		lpObj->m_iItem_MuShop_Scroll_EffectValidTime = 0;
		lpObj->PlayerExtraInfoModified = 1;
	}
	lpObj->m_wItemEffectType[wOptionType] = 0;
	lpObj->m_iItemEffectValidTime[wOptionType] = 0;

	return true;
}


LPITEMEFFECT CItemAddOption::GetCurrentEffectType(LPOBJ lpObj,WORD wOptionType)
{
	if ( lpObj->m_wItemEffectType[wOptionType] <= 0 )
		return NULL;

	return this->_SearchItemEffect(lpObj->m_wItemEffectType[wOptionType]);
}




LPITEMEFFECT CItemAddOption::_SearchItemEffect(int iItemNumber)
{
	//DebugLog("%s START",__FUNCTION__);
	for ( int i=0;i<MAX_ITEM_ADD_OPTION;i++)
	{
		if ( this->m_ItemAddOption[i].iItemNumber == iItemNumber )
		{
			//DebugLog("%s END",__FUNCTION__);
			return &this->m_ItemAddOption[i];
		}
	}

	//DebugLog("%s NULL",__FUNCTION__);
	return NULL;
}



bool CItemAddOption::SearchItemEffectType(int iItemNumber, int * iEffectType1, int * iEffectType2)
{
	LPITEMEFFECT lpItemEffect = NULL;

	lpItemEffect = this->_SearchItemEffect(iItemNumber);

	if ( lpItemEffect == NULL )
		return false;

	*iEffectType1 = lpItemEffect->wEffectType1;
	*iEffectType2 = lpItemEffect->wEffectType2;

	return true;
}
