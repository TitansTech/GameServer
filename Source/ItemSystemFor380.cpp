// ItemSystemFor380.cpp: implementation of the CItemSystemFor380 class.
//	GS-N	1.00.18	JPN	0x00570590	-	Completed
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ItemSystemFor380.h"
#include "Gamemain.h"
#include "CastleSiegeSync.h"
#include "LogProc.h"
#include "..\include\ReadScript.h"

#include "ChaosBox.h"
#include "..\common\winutil.h"

#include "MuItemShop.h"
#include "CrywolfSync.h"
#include "Crywolf.h"

CItemSystemFor380 g_kItemSystemFor380;

#include "LogToFile.h"
extern CLogToFile CHAOS_LOG;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CItemSystemFor380::CItemSystemFor380()
{
	memset(this->m_itemOption, 0, sizeof(this->m_itemOption));
}

CItemSystemFor380::~CItemSystemFor380()
{
	return;
}


void CItemSystemFor380::_InitOption()
{
	memset(this->m_itemOption, 0, sizeof(this->m_itemOption));

	this->m_iNeedJewelOfHarmonyCount = 1;
	this->m_iNeedJewelOfSuhoCount = 1;
	this->m_iNeedZenFor380Option = 10000000;
	this->m_iRateSuccessRateForMix1 = 50;
	this->m_iRateSuccessRateForMix2 = 60;
	this->m_iRateSuccessRateForMix3 = 100;

	this->m_bSystemFor380ItemOption = GetPrivateProfileInt("GameServerInfo", "System380ItemSystem", 0, ReadConfig.ConnDataFiles[0]);
	this->m_iNeedJewelOfHarmonyCount = GetPrivateProfileInt("GameServerInfo", "CountOfJewelOfHarmonyForMix", 1, ReadConfig.ConnDataFiles[0]);
	this->m_iNeedJewelOfSuhoCount = GetPrivateProfileInt("GameServerInfo", "CountOfSuhoOfHarmonyForMix", 1, ReadConfig.ConnDataFiles[0]);
	this->m_iNeedZenFor380Option = GetPrivateProfileInt("GameServerInfo", "NeedZenForMix", 10000000, ReadConfig.ConnDataFiles[0]);
	this->m_iRateSuccessRateForMix1 = GetPrivateProfileInt("GameServerInfo", "MixRateForGrade1", 50, ReadConfig.ConnDataFiles[0]);
	this->m_iRateSuccessRateForMix2 = GetPrivateProfileInt("GameServerInfo", "MixRateForGrade2", 60, ReadConfig.ConnDataFiles[0]);
	this->m_iRateSuccessRateForMix3 = GetPrivateProfileInt("GameServerInfo", "MixRateForGrade3", 60, ReadConfig.ConnDataFiles[0]);
}




BOOL CItemSystemFor380::Load380ItemOptionInfo(LPSTR filename)
{
	enum SMDToken Token;
	SMDFile = fopen(filename, "r");

	if ( SMDFile == NULL )
		return FALSE;

	this->_InitOption();


	while ( true )
	{
		Token = (SMDToken)GetToken();

		if ( Token == END )
			break;

		if ( Token == NUMBER )
		{
			int Index = TokenNumber;

			while ( true )
			{
				if ( Index == 0 )
				{
					Token = (SMDToken)GetToken();
					int _type = TokenNumber;

					if ( Token == NAME )
					{
						if ( !strcmp("end", TokenString))
						{
							fclose(SMDFile);
							return TRUE;
							#pragma message (" [TO FIX] !!! : Recode all Load380ItemOptionInfo")
						}
					}

					Token = (SMDToken)GetToken();
					int _index = TokenNumber;

					ITEMOPTION_FOR380ITEM * p = &this->m_itemOption[ITEMGET(_type, _index)];
					p->IsLoad = TRUE;
					p->m_Type = _type;
					p->m_Index = _index;

					Token = (SMDToken)GetToken();
					p->m_SkillOption = TokenNumber;

					Token = (SMDToken)GetToken();
					p->m_Value1 = TokenNumber;

					Token = (SMDToken)GetToken();
					p->m_LuckOption = TokenNumber;

					Token = (SMDToken)GetToken();
					p->m_Value2 = TokenNumber;

					Token = (SMDToken)GetToken();
					int iTime = TokenNumber;
				}
			}
		}
	}
	fclose(SMDFile);
	return TRUE;
}






BOOL CItemSystemFor380::Is380Item(CItem const *  pItem)
{
	if ( pItem->m_Type < 0 || pItem->m_Type > MAX_ITEMS-1 )
	{
		LogAdd("[380Item] OptionItem Check error: (iItemNum:%d)", pItem->m_Type);
		return FALSE;
	}

	if ( this->m_itemOption[pItem->m_Type ].IsLoad == FALSE )
		return FALSE;

	return TRUE;
}


BOOL CItemSystemFor380::Is380OptionItem(CItem const *  pItem)
{
	if ( this->Is380Item(pItem) == FALSE )
		return FALSE;

	BYTE bResult = (pItem->m_ItemOptionEx & 0x80) >> 7;

	if ( bResult != 1 )
		return FALSE;

	return TRUE;
}


void CItemSystemFor380::InitEffectValue(ITEMOPTION_FOR380ITEM_EFFECT * pItemEffect)
{
	pItemEffect->OpAddAttackSuccessRatePVP = 0;
	pItemEffect->OpAddDamage = 0;
	pItemEffect->OpAddDefenseSuccessRatePvP = 0;
	pItemEffect->OpAddDefense = 0;
	pItemEffect->OpAddMaxHP = 0;
	pItemEffect->OpAddMaxSD = 0;
	pItemEffect->OpRefillOn  = 0;
	pItemEffect->OpAddRefillSD = 0;
}



BOOL CItemSystemFor380::ApplyFor380Option(LPOBJ lpObj)
{
	ITEMOPTION_FOR380ITEM_EFFECT * pItemEffect = &lpObj->m_ItemOptionExFor380;
	this->InitEffectValue(pItemEffect);
	int iItemIndex = 0;

	for ( iItemIndex = 0; iItemIndex < INVETORY_WEAR_SIZE ; iItemIndex++)
	{
		CItem * pItem = &lpObj->pInventory[iItemIndex];

		if ( pItem->IsItem() &&
			 pItem->m_IsValidItem &&
			 this->Is380OptionItem(pItem) == TRUE )
		{
			BYTE iOption1, iOption2;
			WORD iValue1, iValue2;

			iOption1 = this->m_itemOption[pItem->m_Type ].m_SkillOption;
			iValue1 = this->m_itemOption[pItem->m_Type ].m_Value1;
			iOption2 = this->m_itemOption[pItem->m_Type ].m_LuckOption;
			iValue2 = this->m_itemOption[pItem->m_Type ].m_Value2;
			BOOL bResult = FALSE;
			bResult = this->_CalcItemEffectValue(iOption1, iValue1, pItemEffect);
			bResult |= this->_CalcItemEffectValue(iOption2, iValue2, pItemEffect);
		}
	}

	lpObj->AddLife += pItemEffect->OpAddMaxHP;
	lpObj->iAddShield += pItemEffect->OpAddMaxSD;

	return TRUE;
}


BOOL CItemSystemFor380::_CalcItemEffectValue(int iItemOptionType, int iItemEffectValue, ITEMOPTION_FOR380ITEM_EFFECT * pItemEffect)
{
	if ( iItemOptionType == 0 )
		return FALSE;

	BOOL bResult = TRUE;

	switch ( iItemOptionType )
	{
		case ITEMOPTION_FOR380ITEM_EFFECT_OPADDATTACKSUCCESSRATEPVP:
			pItemEffect->OpAddAttackSuccessRatePVP += iItemEffectValue;
			//DebugLog("380 Item: EFFECT_OPADDATTACKSUCCESSRATEPVP %d\n", iItemEffectValue);
		break;
		case ITEMOPTION_FOR380ITEM_EFFECT_OPADDDAMAGE:
			pItemEffect->OpAddDamage += iItemEffectValue;
			//DebugLog("380 Item: EFFECT_OPADDDAMAGE %d\n", iItemEffectValue);
		break;
		case ITEMOPTION_FOR380ITEM_EFFECT_OPADDDEFENSESUCCESSRATEPVP:
			pItemEffect->OpAddDefenseSuccessRatePvP += iItemEffectValue;
			//DebugLog("380 Item: EFFECT_OPADDDEFENSESUCCESSRATEPVP %d\n", iItemEffectValue);
		break;
		case ITEMOPTION_FOR380ITEM_EFFECT_OPADDDEFENSE:
			pItemEffect->OpAddDefense += iItemEffectValue;
			//DebugLog("380 Item: EFFECT_OPADDDEFENSE %d\n", iItemEffectValue);
		break;
		case ITEMOPTION_FOR380ITEM_EFFECT_OPADDMAXHP:
			pItemEffect->OpAddMaxHP += iItemEffectValue;
			//DebugLog("380 Item: EFFECT_OPADDMAXHP %d\n", iItemEffectValue);
		break;
		case ITEMOPTION_FOR380ITEM_EFFECT_OPADDMAXSD:
			pItemEffect->OpAddMaxSD += iItemEffectValue;
			//DebugLog("380 Item: EFFECT_OPADDMAXSD %d\n", iItemEffectValue);
		break;
		case ITEMOPTION_FOR380ITEM_EFFECT_OPREFILLON:
			pItemEffect->OpRefillOn += 1;
			//DebugLog("380 Item: EFFECT_OPREFILLON %d\n", iItemEffectValue);
		break;
		case ITEMOPTION_FOR380ITEM_EFFECT_OPADDREFILLSD:
			pItemEffect->OpAddRefillSD += iItemEffectValue;
			//DebugLog("380 Item: EFFECT_OPADDREFILLSD %d\n", iItemEffectValue);
		break;
		default:
			bResult = FALSE;
		break;
	}

	return bResult;
}












BOOL CItemSystemFor380::_SetOption(CItem * pItem, BOOL bOption)
{
	if ( !this->Is380Item(pItem) )
		return FALSE;

	if ( bOption  == TRUE )
		pItem->m_ItemOptionEx |= 0x80;
	else
		pItem->m_ItemOptionEx &= ~0x80;

	return TRUE;
}



void CItemSystemFor380::SetOptionItemByMacro(LPOBJ lpObj, BYTE invenrotyTargetPos, int bOption)
{
	// HERE GOES A MACRO
	return;
	CItem * pItem=NULL;
}

BOOL CItemSystemFor380::ChaosMix380ItemOption(LPOBJ lpObj)
{
	if ( this->m_bSystemFor380ItemOption != TRUE )
	{
		GCServerMsgStringSend(lMsg.Get(MSGGET(13,55)), lpObj->m_Index, 1);
		lpObj->bIsChaosMixCompleted = false;

		return FALSE;
	}

	lpObj->ChaosLock = TRUE;

	int iValidItemCount = 0;
	int iJewelOfHarmony = 0;
	int iJewelOfSuho = 0;
	int iInvalidItemCount = 0;
	int iMixPrice = 0;
	int iCharmOfLuckCount = 0;

	PMSG_CHAOSMIXRESULT pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x86, sizeof(pMsg));
	pMsg.Result = 0;
	CItem * pTargetItem = NULL;
	int iPosOfJewelOfHarmony= -1;
	int iPosOfJewelOfSuho= -1;

	for ( int n=0;n<CHAOS_BOX_SIZE;n++)
	{
	if ( lpObj->pChaosBox[n].IsItem() == TRUE )
	{
		if ( this->Is380Item(&lpObj->pChaosBox[n]) == TRUE &&
			this->Is380OptionItem(&lpObj->pChaosBox[n]) == FALSE &&
			 lpObj->pChaosBox[n].m_Level > 3 &&
			 (lpObj->pChaosBox[n].m_Z28Option<<2) > 3)
		{
			iValidItemCount++;
			pTargetItem = &lpObj->pChaosBox[n];
		}
		else if ( g_kJewelOfHarmonySystem.IsJewelOfHarmonyPurity(lpObj->pChaosBox[n].m_Type) == TRUE )
		{
			iJewelOfHarmony++;
			iPosOfJewelOfHarmony = n;
		}
		else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14, 31) )
		{
			iJewelOfSuho++;
			iPosOfJewelOfSuho = n;
		}
		else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,53) )
		{
			iCharmOfLuckCount += lpObj->pChaosBox[n].m_Durability;
		}
		else
		{
			iInvalidItemCount++;
		}
	}
	}

	if ( iInvalidItemCount ||
		 iValidItemCount != 1 ||
		 iJewelOfHarmony != 1 ||
		 iJewelOfSuho != 1 ||
		 pTargetItem == NULL ||		
		 iPosOfJewelOfHarmony == -1 ||
		 iPosOfJewelOfSuho == -1 ||
		 iCharmOfLuckCount > 10 )
	{
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return FALSE;
	}

	iMixPrice = this->m_iNeedZenFor380Option;
	int iChaosTaxMoney =  iMixPrice * g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / 100;

	if ( iChaosTaxMoney < 0 )
		iChaosTaxMoney = 0;

	iMixPrice += iChaosTaxMoney;

	if ( iMixPrice < 0 )
		iMixPrice = 0;

	if ( lpObj->Money < iMixPrice )
	{
		pMsg.Result = 2;
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return FALSE;
	}

	lpObj->Money -= iMixPrice;
	g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);
	
	GCMoneySend(lpObj->m_Index, lpObj->Money);
	LogChaosItem(lpObj, "380Item][Item Mix");
	CHAOS_LOG.Output("[380Item][Item Mix] - Mix Start");

	int iRate =	rand() % 100;
	int iRateSuccess = this->m_iRateSuccessRateForMix1;

	if ( pTargetItem->m_Level < 7 )
		iRateSuccess = this->m_iRateSuccessRateForMix1;
	else if ( pTargetItem->m_Level < 10 )
		iRateSuccess = this->m_iRateSuccessRateForMix2;
	else if ( pTargetItem->m_Level < 14 )
		iRateSuccess = this->m_iRateSuccessRateForMix3;

	iRateSuccess += iCharmOfLuckCount;
	lpObj->ChaosSuccessRate = iRateSuccess;

	if (ReadConfig.IsVipExtraMixPercent == 1 && lpObj->Vip == 1)
	{
		if (lpObj->ChaosSuccessRate + ReadConfig.VipExtraMixPercent < 100)
			lpObj->ChaosSuccessRate += ReadConfig.VipExtraMixPercent;
		else
			lpObj->ChaosSuccessRate = 100;

		CHAOS_LOG.Output("[CBMix][%s][%s] VIP Extra Percent brings mix up to: %d", 
			lpObj->AccountID, lpObj->Name,
			lpObj->ChaosSuccessRate
		);

		char sbuff[1024]={0};
		wsprintf(sbuff,"[VIP] Extra %d percent added!",ReadConfig.VipExtraMixPercent);
		GCServerMsgStringSend(sbuff,lpObj->m_Index,1);
	}
	lpObj->pChaosBox[iPosOfJewelOfHarmony].Clear();
	lpObj->pChaosBoxMap[iPosOfJewelOfHarmony] = -1;
	lpObj->pChaosBox[iPosOfJewelOfSuho].Clear();
	lpObj->pChaosBoxMap[iPosOfJewelOfSuho] = -1;

	if ( iRate <= lpObj->ChaosSuccessRate )
	{
		this->_SetOption(pTargetItem, TRUE);
		GCUserChaosBoxSend(lpObj, 0);

		CHAOS_LOG.Output("[380Item][ItemMix] Mix Success [%s][%s], Money(%d-%d) Rate(%d/%d) Option(%d,%d) OptionValue(%d,%d)",
			lpObj->AccountID, lpObj->Name, lpObj->Money,
			iMixPrice, iRate, iRateSuccess,
			this->m_itemOption[(pTargetItem->m_Type )].m_SkillOption,
			this->m_itemOption[(pTargetItem->m_Type )].m_LuckOption,
			this->m_itemOption[(pTargetItem->m_Type )].m_Value1,
			this->m_itemOption[(pTargetItem->m_Type )].m_Value2);
	
		MuItemShop.EarnGoblinPointsCBMix(lpObj->m_Index, lpObj->ChaosSuccessRate, 0);
	}
	else
	{
		GCUserChaosBoxSend(lpObj, 0);
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);

		CHAOS_LOG.Output("[380Item][ItemMix] Mix Fail [%s][%s], Money(%d-%d) Rate(%d/%d)",
			lpObj->AccountID, lpObj->Name, lpObj->Money,
			iMixPrice, iRate, iRateSuccess);

		MuItemShop.EarnGoblinPointsCBMix(lpObj->m_Index, lpObj->ChaosSuccessRate, 1);
	}

	::gObjInventoryCommit(lpObj->m_Index);
	lpObj->ChaosLock = FALSE;
	return TRUE;
}













