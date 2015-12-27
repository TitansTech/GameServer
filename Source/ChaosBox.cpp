#include "stdafx.h"
#include "ChaosBox.h"
#include "logproc.h"
#include "GameMain.h"
#include "GameServer.h"
#include "..\common\winutil.h"
#include "DSProtocol.h"
#include "CastleSiegeSync.h"
//#include "GameServerAuth.h"
#include "BloodCastle.h"
#include "..\common\SetItemOption.h"
#include "JewelOfHarmonySystem.h"
#include "giocp.h"
#include "CrywolfSync.h"
#include "Crywolf.h"
#include "CashLotterySystem.h"
#include "ImperialGuardian.h"
#include "IllusionTemple.h"
#include "gObjMonster.h"

#include "MuItemShop.h"

// GS-N 0.99.60T 0x0047C360 - Completed
//	GS-N	1.00.18	JPN	0x00493D40	-	Completed

#include "LogToFile.h"
extern CLogToFile CHAOS_LOG;

BOOL ChaosBoxCheck(LPOBJ lpObj)
{
	if ( lpObj->pChaosBox == NULL )
	{
		return false;
	}

	return true;
}

BOOL ChaosBoxInit(LPOBJ lpObj)
{
	int n;

	if ( lpObj->pChaosBox != NULL )
	{
		for (n=0;n<CHAOS_BOX_SIZE;n++)
		{
			lpObj->pChaosBox[n].Clear();
		}

		for (n=0;n<CHAOS_BOX_SIZE;n++)
		{
			lpObj->pChaosBoxMap[n] = -1;
		}
		return true;
	}

	lpObj->pChaosBox = new CItem[CHAOS_BOX_SIZE];

	if ( lpObj->pChaosBox == NULL )
	{
		return false;
	}

	lpObj->pChaosBoxMap = new unsigned char[CHAOS_BOX_SIZE];

	if ( lpObj->pChaosBoxMap == NULL )
	{
		delete lpObj->pChaosBox;
		return false;
	}

	for (n=0;n<CHAOS_BOX_SIZE;n++)
	{
		lpObj->pChaosBoxMap[n] = -1;
	}

	return true;
}


BOOL ChaosBoxItemDown(LPOBJ lpObj)
{
	if ( lpObj->pChaosBox == NULL )
	{
		return FALSE;
	}

	int hasRateItem=0;

	for (int n=0;n<CHAOS_BOX_SIZE;n++)
	{
		if (lpObj->pChaosBox[n].m_Type == ITEMGET(12,15) || //Jewel of Chaos
			lpObj->pChaosBox[n].m_Type == ITEMGET(14,13) || //Jewel of Bless
			lpObj->pChaosBox[n].m_Type == ITEMGET(14,14) || //Jewel of Soul
			lpObj->pChaosBox[n].m_Type == ITEMGET(14,22) ||	//Jewel of Creation
			lpObj->pChaosBox[n].m_Type == ITEMGET(14,42) ||	//Jewel of Harmony
			lpObj->pChaosBox[n].m_Type == ITEMGET(14,53) ||	//Charm Of Luck
			lpObj->pChaosBox[n].m_Type == ITEMGET(14,96)	//Chaos machine rate
			)
		{
			if (lpObj->pChaosBox[n].m_Type == ITEMGET(14,96))
			{
				hasRateItem = 1;
			}

			lpObj->pChaosBox[n].Clear();
		}
	}

	for (int n=0;n<CHAOS_BOX_SIZE;n++)
	{
		if(lpObj->pChaosBox[n].IsItem())
		{
			int set = 0;
			int op = 0;

			//If rate item present, reduce Z28 option and +Level ONLY!
			if (hasRateItem == 0)
			{
				set = lpObj->pChaosBox[n].m_SetOption;
				op = lpObj->pChaosBox[n].m_SkillOption;

				if ( set > 0 )
				{
					if ( (rand() % 2) == 0 )
					{
						set--;
					}
				}
				lpObj->pChaosBox[n].m_SetOption = set;

				if ( op > 0 )
				{
					if ( (rand() % 2) == 0 )
					{
						op--;
					}
				}
				lpObj->pChaosBox[n].m_SkillOption = op;
			} else {
				set = lpObj->pChaosBox[n].m_SetOption;
				op = lpObj->pChaosBox[n].m_SkillOption;

				lpObj->pChaosBox[n].m_SetOption = set;
				lpObj->pChaosBox[n].m_SkillOption = op;

				//Luck Removal
				int lk = lpObj->pChaosBox[n].m_LuckOption;

				if ( lk > 0 )
				{
					if ( (rand() % 4) == 0 )
					{
						//lpObj->pChaosBox[n].m_LuckOption = 0;
					}
				}
			}

			//Reduce Z28 Option of Item
			op = lpObj->pChaosBox[n].m_Z28Option;

			if ( op > 0 )
			{
				if ( (rand() % 2) == 0 )
				{
					op--;
				}
			}
			lpObj->pChaosBox[n].m_Z28Option = op;

			//Reduce Item Level
			if ( lpObj->pChaosBox[n].m_Level > 0 )
			{
				lpObj->pChaosBox[n].m_Level = rand() % lpObj->pChaosBox[n].m_Level;
			}

			float dur = ItemGetDurability(lpObj->pChaosBox[n].m_Type, lpObj->pChaosBox[n].m_Level, lpObj->pChaosBox[n].IsExtItem(), lpObj->pChaosBox[n].IsSetItem());
				
			lpObj->pChaosBox[n].m_Durability =  dur * lpObj->pChaosBox[n].m_Durability / lpObj->pChaosBox[n].m_BaseDurability;

			CHAOS_LOG.Output("[CBMix][Item Fail] [%s][%s] Item Fail [%s][%d] [%d %d %d %d] Ex:[%d] Set:[%d] 380Ex:[%d] Slot:[%d,%d,%d,%d,%d]",
				lpObj->AccountID,lpObj->Name,
				ItemAttribute[lpObj->pChaosBox[n].m_Type].Name, lpObj->pChaosBox[n].m_Type, 
				lpObj->pChaosBox[n].m_Level, lpObj->pChaosBox[n].m_SkillOption, lpObj->pChaosBox[n].m_LuckOption, lpObj->pChaosBox[n].m_Z28Option, 
				lpObj->pChaosBox[n].m_NewOption, 
				lpObj->pChaosBox[n].m_SetOption, 
				lpObj->pChaosBox[n].m_ItemOptionEx,
				lpObj->pChaosBox[n].m_ItemSlot1, lpObj->pChaosBox[n].m_ItemSlot2, 
				lpObj->pChaosBox[n].m_ItemSlot3, lpObj->pChaosBox[n].m_ItemSlot4, 
				lpObj->pChaosBox[n].m_ItemSlot5);

			lpObj->pChaosBox[n].Convert(lpObj->pChaosBox[n].m_Type, lpObj->pChaosBox[n].m_SkillOption,
				lpObj->pChaosBox[n].m_LuckOption, lpObj->pChaosBox[n].m_Z28Option, lpObj->pChaosBox[n].m_NewOption,
				lpObj->pChaosBox[n].m_SetOption, lpObj->pChaosBox[n].m_ItemOptionEx, CURRENT_DB_VERSION);
		}
	}

	return TRUE;
}



int ChaosBoxMix(LPOBJ lpObj, int & Result2)
{
	BYTE ExOption[8];
	int ChaosDiamond = 0;
	int ChaosItems = 0;

	if ( lpObj->pChaosBox == NULL )
	{
		return 0;
	}

	int value = 0;
	int add = 0;
	int nv = 0;	// NEW VALUE
	Result2 = 0;
	lpObj->ChaosSuccessRate = 0;
	lpObj->ChaosMoney = 0;
	int iCharmOfLuckCount = 0;

	for ( int n=0;n<CHAOS_BOX_SIZE;n++)
	{
		if ( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			lpObj->pChaosBox[n].OldValue();
			add = 0;
			nv = 0;

			if ( lpObj->pChaosBox[n].m_Level >= MIN_CHAOS_ITEM_LEVEL && (lpObj->pChaosBox[n].m_Z28Option *4) >= MIN_CHAOS_ITEM_LEVEL )
			{
				nv = lpObj->pChaosBox[n].m_OldBuyMoney;
				value += lpObj->pChaosBox[n].m_OldBuyMoney;
				add = 1;

				if ( lpObj->pChaosBox[n].m_Type == ITEMGET(2,6) || lpObj->pChaosBox[n].m_Type == ITEMGET(4,6) || lpObj->pChaosBox[n].m_Type == ITEMGET(5,7) )	// Chaos Items
				{
					Result2 = 1;
				}
			}

			if ( lpObj->pChaosBox[n].m_Type == ITEMGET(12,15) )
			{
				value += lpObj->pChaosBox[n].m_OldBuyMoney;
				nv = lpObj->pChaosBox[n].m_OldBuyMoney;
				ChaosDiamond++;
				add = 1;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,13) || lpObj->pChaosBox[n].m_Type == ITEMGET(14,14) )	// Jewel of Bless, Jewel of Soul
			{
				value += lpObj->pChaosBox[n].m_OldBuyMoney;
				nv = lpObj->pChaosBox[n].m_OldBuyMoney;
				add = 1;
			} 

			if ( lpObj->pChaosBox[n].m_Type == ITEMGET(13,14) ) // Loch Feather
			{
				return FALSE;
			}


			if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,53) )	// Charm Of Luck
			{
				iCharmOfLuckCount += lpObj->pChaosBox[n].m_Durability;
			}
			
			if ( add != 0  )
			{
				ChaosItems++;
			}

			ItemIsBufExOption(ExOption, &lpObj->pChaosBox[n]);
			CHAOS_LOG.Output("[%s][%s] CBMix [%d,%s,%d,%d,%d,%d]serial:[%d][%d][%d] Ex:[%d,%d,%d,%d,%d,%d,%d]",
				lpObj->AccountID, lpObj->Name, n, lpObj->pChaosBox[n].GetName(),
				lpObj->pChaosBox[n].m_Level, lpObj->pChaosBox[n].m_SkillOption,
				lpObj->pChaosBox[n].m_LuckOption, lpObj->pChaosBox[n].m_Z28Option,
				lpObj->pChaosBox[n].m_Number, (int)lpObj->pChaosBox[n].m_Durability,
				nv, ExOption[0], ExOption[1], ExOption[2], ExOption[3], ExOption[4], ExOption[5],
				ExOption[6], lpObj->pChaosBox[n].m_SetOption);
		}
	}

	if ( ChaosDiamond == 0 )
	{
		value = 0;	// 0% of success
	}

	if ( ChaosItems < 2 )
	{
		value = 0;// 0% of success
	}

	if ( iCharmOfLuckCount > 10 )
		return FALSE;


	
	lpObj->ChaosSuccessRate = ( value / 20000 )&255;
	lpObj->ChaosSuccessRate = ( lpObj->ChaosSuccessRate + iCharmOfLuckCount )&255;

	if ( lpObj->ChaosSuccessRate  > 100 )
	{
		lpObj->ChaosSuccessRate = 100;
	}

	lpObj->ChaosMoney = lpObj->ChaosSuccessRate * 10000;	// Required Money to MIX and createe a Chaos Item

	CHAOS_LOG.Output("[%s][%s] CBMix need Zen : %d SuccessRate : %d, CharmRate : %d",
		lpObj->AccountID, lpObj->Name, lpObj->ChaosMoney, 
		lpObj->ChaosSuccessRate, iCharmOfLuckCount);
	return value;
}



struct CB_PMSG_REQ_PRESENT
{
	PBMSG_HEAD h;	// C1:01
	char AccountId[10];	// 3
	char GameId[10];	// D
	int Sequence;	// 18
};


void CBUPS_ItemRequest(int aIndex, char* AccountId, char* Name)
{
	CHAOS_LOG.Output("[DevilSquare] 이벤트 경품 요청 [%s][%s]", AccountId, Name);	// #warning Translation

	CB_PMSG_REQ_PRESENT pMsg;

	pMsg.h.c = 0xC1;
	pMsg.h.headcode = 0x01;
	pMsg.h.size = sizeof(pMsg);
	pMsg.Sequence = aIndex;
	memcpy(pMsg.AccountId, AccountId, sizeof(pMsg.AccountId));
	memcpy(pMsg.GameId, Name, sizeof(pMsg.GameId));

	gUdpSocCE.SendData((LPBYTE)&pMsg, pMsg.h.size);
}




void ChaosEventProtocolCore(BYTE protoNum, unsigned char* aRecv, int aLen)
{
	#ifdef TRACE_PROTOCOL
		LogAddHeadHex("R","CHAOS_EVENT", aRecv, aLen);
	#endif

	switch ( protoNum )
	{
		case 0x01:
			CBUPR_ItemRequestRecv( (CB_PMSG_REQ_PRESENT_RESULT *)aRecv);
			break;
	}
}



struct PMSG_PRIZE_INFO
{
	PBMSG_HEAD h;
	char AccountID[10];	// 3
	char GameID[10];	// D
	int servernumber;	// 18
	char present;	// 1C
	int Seq_present;	// 20
};



void CBUPR_ItemRequestRecv( CB_PMSG_REQ_PRESENT_RESULT * lpMsg)
{
	LPOBJ lpObj;
	char szId[11];
	char szName[11];
	char szPresentName[51];
	int aIndex;
	PMSG_PRIZE_INFO pMsg;

	szId[10] = 0;
	szName[10] = 0;
	memset(szPresentName, 0, sizeof(szPresentName));
	memcpy(szId, lpMsg->AccountId, sizeof(lpMsg->AccountId));
	memcpy(szName, lpMsg->GameId, sizeof(lpMsg->GameId));
	memcpy(szPresentName, lpMsg->presentname, sizeof(lpMsg->presentname));
	aIndex = lpMsg->Sequence;

	if (gObjIsConnectedGP(aIndex) == FALSE )
	{
		CHAOS_LOG.Output("error-L3 [%s][%d]", __FILE__, __LINE__);
		//CHAOS_LOG.Output(2, "error-L3 [%s][%d]", __FILE__, __LINE__);
		return;
	}

	if (OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	lpObj = &gObj[aIndex];

	if ( strcmp(szId, lpObj->AccountID) != 0 )
	{
		CHAOS_LOG.Output("error-L3 [%s][%d][%s == %s]", __FILE__, __LINE__, szId, lpObj->AccountID);
		//CHAOS_LOG.Output(2, "error-L3 [%s][%d][%s == %s]", __FILE__, __LINE__, szId, lpObj->AccountID);
		return;
	}

	if ( strcmp(szName, lpObj->Name) != 0 )
	{
		CHAOS_LOG.Output("error-L3 [%s][%d]", __FILE__, __LINE__);
		//CHAOS_LOG.Output(2, "error-L3 [%s][%d]", __FILE__, __LINE__);
		return;
	}

	if ( lpMsg->present != -1 )
	{
		char szTemp[256]="";

		wsprintf(szTemp, lMsg.Get(MSGGET(6, 65)), szName, szPresentName);
		AllSendServerMsg(szTemp);

		pMsg.h.c = 0xC1;
		pMsg.h.headcode = 0x02;
		pMsg.h.size = sizeof(pMsg);
		pMsg.present = lpMsg->present;
		pMsg.Seq_present = lpMsg->Seq_present;
		pMsg.servernumber = gGameServerCode;
		memcpy(pMsg.AccountID, lpMsg->AccountId, sizeof(pMsg.AccountID));
		memcpy(pMsg.GameID, lpMsg->GameId, sizeof(pMsg.GameID));

		gUdpSocCE.SendData((LPBYTE)&pMsg, pMsg.h.size);

		CHAOS_LOG.Output("[DevilSquare] Present Prize Notice %d(%s) %d %s %s",
			lpMsg->present, szPresentName, lpMsg->Seq_present, szId, szName);

		ItemSerialCreateSend(lpObj->m_Index, -1, 0, 0, 0x1FF, 0, 255, 0, 0, 0, -1, 0, 0);
		lpObj->Money -= 10000;
		GCMoneySend(lpObj->m_Index, lpObj->Money);

		PMSG_CHAOSMIXRESULT pMsg;

		PHeadSetB((LPBYTE)&pMsg, 0x86, sizeof(pMsg));
		pMsg.Result = 5;

		DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
	}
	else
	{
		int in = ITEMGET(14,19); // Devil Square Invitation
		ItemSerialCreateSend(lpObj->m_Index, -1, 0, 0, in, 0, 0, 0, 0, 0, -1, 0, 0);
		lpObj->Money -= 10000;
		GCMoneySend(lpObj->m_Index, lpObj->Money);
		lpObj->ChaosLock = FALSE;

		CHAOS_LOG.Output("[DevilSquare] [%s][%s] CBMix Success %d Money : %d-%d",
			lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate,
			lpObj->Money, lpObj->ChaosMoney);
	}
}






int  CheckDevilSquareItem(LPOBJ lpObj, int & eventitemcount, int & itemlevel)
{
#if (WL_PROTECT==1)
	VM_START_WITHLEVEL(4)
	int MyCheckVar;  
	CHECK_PROTECTION(MyCheckVar, 0x52547978)  	 
	if (MyCheckVar != 0x52547978)
	{
		WLRegDisableCurrentKey(wlMarkStolenKey);
		WLRegRemoveCurrentKey();
	}
	VM_END
#endif
	BOOL bChaoseGem = FALSE;
	BOOL bEyeOfDevil = FALSE;
	BOOL bKeyOfDevil = FALSE;
	eventitemcount = 0;
	BOOL FoundOtherItem = FALSE;
	int level = -1;
	BOOL bLevelCheck = FALSE;
	int iCharmOfLuckCount = 0;
	BOOL bCharmOfLuckOver = FALSE;

	CHAOS_LOG.Output("[DevilSquare] Search DevilSquareItem");

	for ( int n=0;n<CHAOS_BOX_SIZE;n++)
	{
		if ( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			if ( lpObj->pChaosBox[n].m_Type == ITEMGET(12,15) )	// Chaos
			{
				bChaoseGem = TRUE;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,17) )	// Eye
			{
				eventitemcount +=1;
				bEyeOfDevil = TRUE;

				if ( level != lpObj->pChaosBox[n].m_Level )
				{
					if ( level == -1 )
					{
						level = lpObj->pChaosBox[n].m_Level;
					}
					else
					{
						bLevelCheck = TRUE;
					}
				}
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,18) )	// Key
			{
				eventitemcount +=1;
				bKeyOfDevil = TRUE;

				if ( level != lpObj->pChaosBox[n].m_Level )
				{
					if ( level == -1 )
					{
						level = lpObj->pChaosBox[n].m_Level;
					}
					else
					{
						bLevelCheck = TRUE;
					}
				}
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,53) )	// Charm Of Luck
			{
				iCharmOfLuckCount += lpObj->pChaosBox[n].m_Durability;

				if ( iCharmOfLuckCount > 10 )
				{
					bCharmOfLuckOver = TRUE;
				}
			}
			else
			{
				FoundOtherItem = TRUE;
			}
		}
	}

	itemlevel = level;
	lpObj->ChaosSuccessRate = ( iCharmOfLuckCount )&255;

	if ( FoundOtherItem != FALSE )
	{
		CHAOS_LOG.Output("[DevilSquare] Other DQItems Found");
		return 0;
	}

	if ( bLevelCheck != FALSE )
	{
		CHAOS_LOG.Output("[DevilSquare] DiffLevel DQItems Found");
		return 3;
	}

	if ( bCharmOfLuckOver == TRUE )
	{
		CHAOS_LOG.Output("[DevilSquare] Charm Of Luck Count Over");
		return 4;
	}

	if ( bChaoseGem != FALSE && bEyeOfDevil != FALSE && bKeyOfDevil != FALSE )
	{
		LogDQChaosItem(lpObj);
		return 1;
	}

	if ( bEyeOfDevil != FALSE && bKeyOfDevil != FALSE )
	{
		LogDQChaosItem(lpObj);
		return 2;
	}

	CHAOS_LOG.Output("[DevilSquare] DQItems Not Found");

	return 0;
}





void LogDQChaosItem(LPOBJ lpObj)
{
	BYTE ExOption[MAX_EXOPTION_SIZE];

	for ( int n=0;n<CHAOS_BOX_SIZE;n++)
	{
		if ( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			::ItemIsBufExOption(ExOption, &lpObj->pChaosBox[n] );

			CHAOS_LOG.Output("[DevilSquare,%d] [%s][%s] CBMix [%d,%s,%d,%d,%d,%d]serial:[%d][%d] Ex:[%d,%d,%d,%d,%d,%d,%d] Set:[%d] 380Ex[%d] HO:[%d,%d]",
				lpObj->pChaosBox[n].m_Level,
				lpObj->AccountID, lpObj->Name, n, lpObj->pChaosBox[n].GetName(), lpObj->pChaosBox[n].m_Level,
				lpObj->pChaosBox[n].m_SkillOption, lpObj->pChaosBox[n].m_LuckOption, lpObj->pChaosBox[n].m_Z28Option, 
				lpObj->pChaosBox[n].m_Number, (int)lpObj->pChaosBox[n].m_Durability, 
				ExOption[0], ExOption[1], ExOption[2], ExOption[3], ExOption[4], ExOption[5], ExOption[6],
				lpObj->pChaosBox[n].m_SetOption, lpObj->pChaosBox[n].m_ItemOptionEx >> 7,
				g_kJewelOfHarmonySystem.GetItemStrengthenOption(&lpObj->pChaosBox[n]),
				g_kJewelOfHarmonySystem.GetItemOptionLevel(&lpObj->pChaosBox[n]));
		}
	}
}






BOOL DevilSquareEventChaosMix(LPOBJ lpObj, BOOL bCheckType, int iItemLevel)
{
	BOOL fail = TRUE;
	int aIndex = lpObj->m_Index;
	PMSG_CHAOSMIXRESULT pMsg;

	PHeadSetB((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR;
	lpObj->ChaosLock = TRUE;

	CHAOS_LOG.Output("[DevilSquare] Chaos Mix Start");
	INT nChaosNeedMoney = 0;
	int iCharmOfLuckCount = 0;
	iCharmOfLuckCount = lpObj->ChaosSuccessRate;

	if ( iCharmOfLuckCount > 10 )
	{
		pMsg.Result = 0xF0;
		lpObj->ChaosLock = FALSE;
		DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
	}

	switch ( iItemLevel )
	{
		case 0:
			lpObj->ChaosSuccessRate = 60;
			nChaosNeedMoney = 100000;
			break;

		case 1:
			lpObj->ChaosSuccessRate = ( ReadConfig.gDQChaosSuccessRateLevel1 )&255;
			nChaosNeedMoney = 100000;
			break;

		case 2:
			lpObj->ChaosSuccessRate = ( ReadConfig.gDQChaosSuccessRateLevel2 )&255;
			nChaosNeedMoney = 200000;
			break;

		case 3:
			lpObj->ChaosSuccessRate = ( ReadConfig.gDQChaosSuccessRateLevel3 )&255;
			nChaosNeedMoney = 400000;
			break;

		case 4:
			lpObj->ChaosSuccessRate = ( ReadConfig.gDQChaosSuccessRateLevel4 )&255;
			nChaosNeedMoney = 700000;
			break;

		case 5:
			lpObj->ChaosSuccessRate = ( ReadConfig.gDQChaosSuccessRateLevel5 )&255;
			nChaosNeedMoney = 1100000;
			break;

		case 6:
			lpObj->ChaosSuccessRate = ( ReadConfig.gDQChaosSuccessRateLevel6 )&255;
			nChaosNeedMoney = 1600000;
			break;

		case 7:
			lpObj->ChaosSuccessRate = ( ReadConfig.gDQChaosSuccessRateLevel7 )&255;
			nChaosNeedMoney = 2000000;
			break;

		default:
			CHAOS_LOG.Output("[DevilSquare] [%s][%s] Invalid DQItem Level [%d]",
				lpObj->AccountID, lpObj->Name, iItemLevel);

			pMsg.Result = CB_INVALID_ITEM_LEVEL;
			DataSend(aIndex, (BYTE *)&pMsg, pMsg.h.size);
			lpObj->ChaosLock = FALSE;
			return 1;
			break;
	}

	int iChaosTaxMoney = (int)((__int64)nChaosNeedMoney * (__int64)g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / (__int64)100);

	if ( iChaosTaxMoney < 0 )
	{
		iChaosTaxMoney = 0;
	}

	nChaosNeedMoney += iChaosTaxMoney;

	if ( nChaosNeedMoney < 0 )
	{
		nChaosNeedMoney = 0;
	}

	if ( (lpObj->Money - nChaosNeedMoney) < 0 )
	{
		CHAOS_LOG.Output("[DevilSquare] [%s][%s] CBMix Not Enough Money [%d] need zen [%d]",
			lpObj->AccountID, lpObj->Name, lpObj->Money, nChaosNeedMoney);
		pMsg.Result = CB_NOT_ENOUGH_ZEN;
		DataSend(aIndex, (BYTE *)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;
		return 1;
	}

	if ( g_CrywolfSync.GetOccupationState() == CRYWOLF_OCCUPATION_SECURE && g_iCrywolfApplyMvpBenefit )
	{	
		lpObj->ChaosSuccessRate = ( lpObj->ChaosSuccessRate + g_CrywolfSync.GetPlusChaosRate() )&255;
	}

	lpObj->ChaosSuccessRate = ( lpObj->ChaosSuccessRate + iCharmOfLuckCount )&255;

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

	int iRate = rand() % 100;

	if ( bCheckType == TRUE )
	{
		if ( iRate < lpObj->ChaosSuccessRate )
		{
			int DevilInv = ITEMGET(14,19);	// Devil Ticket
			ItemSerialCreateSend(lpObj->m_Index, -1, 0, 0, DevilInv, iItemLevel, 0, 0, 0, 0, -1, 0, 0);
			fail = FALSE;
		}
	}

	lpObj->Money -= nChaosNeedMoney;
	g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);
	GCMoneySend(lpObj->m_Index, lpObj->Money);

	if ( fail == TRUE )
	{
		ChaosBoxInit(lpObj);
		GCUserChaosBoxSend(lpObj, 0);
		DataSend(aIndex, (BYTE *)&pMsg, pMsg.h.size);

		CHAOS_LOG.Output("[DevilSquare,%d] [%s][%s] CBMix Fail %d/%d Money : %d-%d", iItemLevel,
			lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, iRate,
			lpObj->Money, nChaosNeedMoney);
		lpObj->ChaosLock = FALSE;

		MuItemShop.EarnGoblinPointsCBMix(lpObj->m_Index, lpObj->ChaosSuccessRate, 1);
	}
	else
	{
		CHAOS_LOG.Output("[DevilSquare,%d] [%s][%s] CBMix Success %d/%d Money : %d", iItemLevel,
			lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, iRate,
			nChaosNeedMoney);

		MuItemShop.EarnGoblinPointsCBMix(lpObj->m_Index, lpObj->ChaosSuccessRate, 0);
	}

	::gObjInventoryCommit(lpObj->m_Index);
	return TRUE;
}




void LogPlusItemLevelChaosItem(LPOBJ lpObj, int iPlusMixLevel)
{
	int iMixLevel = 0;
	BYTE ExOption[MAX_EXOPTION_SIZE];

	for ( int n=0;n<CHAOS_BOX_SIZE;n++)
	{
		if ( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			::ItemIsBufExOption(ExOption, &lpObj->pChaosBox[n]);

			if ( iPlusMixLevel == 3 )
				iMixLevel = 1;
			else if ( iPlusMixLevel == 4 )
				iMixLevel = 2;
			else if ( iPlusMixLevel == 22 )
				iMixLevel = 3;
			else if ( iPlusMixLevel == 23 )
				iMixLevel = 4;
			else if ( iPlusMixLevel == 49 )
				iMixLevel = 5;
			else if ( iPlusMixLevel == 50 )
				iMixLevel = 6;

			CHAOS_LOG.Output("[PlusItemLevel,%d] [%s][%s] CBMix [%d,%s,%d,%d,%d,%d]serial:[%d][%d] Ex:[%d,%d,%d,%d,%d,%d,%d] Set:[%d] 380Ex:[%d] HO:[%d,%d] SK:[%d,%d,%d,%d,%d]", 
				iMixLevel,
				lpObj->AccountID, lpObj->Name, n, lpObj->pChaosBox[n].GetName(), lpObj->pChaosBox[n].m_Level,
				lpObj->pChaosBox[n].m_SkillOption, lpObj->pChaosBox[n].m_LuckOption, lpObj->pChaosBox[n].m_Z28Option, 
				lpObj->pChaosBox[n].m_Number, (int)lpObj->pChaosBox[n].m_Durability, 
				ExOption[0], ExOption[1], ExOption[2], ExOption[3], ExOption[4], ExOption[5], ExOption[6], lpObj->pChaosBox[n].m_SetOption,
				lpObj->pChaosBox[n].m_ItemOptionEx >> 7,
				g_kJewelOfHarmonySystem.GetItemOptionLevel(&lpObj->pChaosBox[n]),	// #error Cahnge to GetItemStrengthenOption
				g_kJewelOfHarmonySystem.GetItemStrengthenOption(&lpObj->pChaosBox[n]), // #error Cahnge to GetItemOptionLevel
				lpObj->pChaosBox[n].m_ItemSlot1,lpObj->pChaosBox[n].m_ItemSlot2,lpObj->pChaosBox[n].m_ItemSlot3,lpObj->pChaosBox[n].m_ItemSlot4,lpObj->pChaosBox[n].m_ItemSlot5);	
		}
	}
}





void LogChaosItem(LPOBJ lpObj, LPSTR sLogType)
{
	BYTE ExOption[MAX_EXOPTION_SIZE];

	for ( int n=0;n<CHAOS_BOX_SIZE;n++)
	{
		if ( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			::ItemIsBufExOption(ExOption, &lpObj->pChaosBox[n]);

			CHAOS_LOG.Output("[%s] [%s][%s] CBMix [%d,%s,%d,%d,%d,%d]serial:[%d][%d] Ex:[%d,%d,%d,%d,%d,%d,%d] Set:[%d] 380:[%d] HO:[%d,%d]",
				sLogType, lpObj->AccountID, lpObj->Name, n, lpObj->pChaosBox[n].GetName(), lpObj->pChaosBox[n].m_Level,
				lpObj->pChaosBox[n].m_SkillOption, lpObj->pChaosBox[n].m_LuckOption, lpObj->pChaosBox[n].m_Z28Option, 
				lpObj->pChaosBox[n].m_Number, (int)lpObj->pChaosBox[n].m_Durability, 
				ExOption[0], ExOption[1], ExOption[2], ExOption[3], ExOption[4], ExOption[5], ExOption[6], lpObj->pChaosBox[n].m_SetOption,
				lpObj->pChaosBox[n].m_ItemOptionEx >> 7,
				g_kJewelOfHarmonySystem.GetItemStrengthenOption(&lpObj->pChaosBox[n]),
				g_kJewelOfHarmonySystem.GetItemOptionLevel(&lpObj->pChaosBox[n]));
		}
	}
}






BOOL PlusItemLevelChaosMix(LPOBJ lpObj, int mixType)
{
	int ChaosGemCount = 0;
	int BlessGemCount = 0;
	int SoulGemCount = 0;
	int Plus9ItemCount = 0;
	int Plus10ItemCount = 0;
	int PlusItemPos = -1;
	int OtherItemFound = 0;
	int Plus11ItemCount = 0;
	int Plus12ItemCount = 0;
	int Plus13ItemCount = 0;
	int Plus14ItemCount = 0;
	int ExtraBlessGemCount = 0;
	int ExtraSoulGemCount = 0;
	int iCharmOfLuckCount = 0;
	int iChaosRateCount = 0;
	BYTE btItemLuckValue = 0;

	for ( int n=0;n<CHAOS_BOX_SIZE;n++)
	{
		if ( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			if ( lpObj->pChaosBox[n].m_Type == ITEMGET(12,15) )
			{
				ChaosGemCount++;
			}
			else if (  lpObj->pChaosBox[n].m_Type == ITEMGET(14,13) )
			{
				BlessGemCount++;
			}
			else if (  lpObj->pChaosBox[n].m_Type == ITEMGET(14,14) )
			{
				SoulGemCount++;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,53) )	// Charm Of Luck
			{
				iCharmOfLuckCount += lpObj->pChaosBox[n].m_Durability;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,96) )	// Chaos Machine Rate
			{
				iChaosRateCount += 1;
			}
			else if ( lpObj->pChaosBox[n].m_Level == 9 )
			{
				Plus9ItemCount++;
				PlusItemPos = n;
			}
			else if ( lpObj->pChaosBox[n].m_Level == 10 )
			{
				Plus10ItemCount++;
				PlusItemPos = n;
			}
			else if ( lpObj->pChaosBox[n].m_Level == 11 )
			{
				Plus11ItemCount++;
				PlusItemPos = n;
			}
			else if ( lpObj->pChaosBox[n].m_Level == 12 )
			{
				Plus12ItemCount++;
				PlusItemPos = n;
			}
			else if ( lpObj->pChaosBox[n].m_Level == 13 )
			{
				Plus13ItemCount++;
				PlusItemPos = n;
			}
			else if ( lpObj->pChaosBox[n].m_Level == 14 )
			{
				Plus14ItemCount++;
				PlusItemPos = n;
			}
			else
			{
				OtherItemFound ++;
			}
		}
	}

	if ( iCharmOfLuckCount > 10 )
	{
		PMSG_CHAOSMIXRESULT pResult;

		PHeadSetB((LPBYTE)&pResult, 0x86, sizeof(pResult));

		CHAOS_LOG.Output("[PlusItemLevel] [%s][%s] CBMix Charm of luck over 10% (%d)",
			lpObj->AccountID, lpObj->Name, iCharmOfLuckCount);

		pResult.Result = 0xF0;

		DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);

		lpObj->ChaosLock = FALSE;

		return 0;
	}



	if ( OtherItemFound != FALSE )
	{
		return FALSE;
	}

	lpObj->ChaosMoney = 0;
	int MixType = 0;

	if ( Plus9ItemCount == 1 && ChaosGemCount == 1 && BlessGemCount == 1 && SoulGemCount == 1 && Plus10ItemCount == 0 && Plus11ItemCount == 0 && Plus12ItemCount == 0 )
	{
		MixType = 3;
		lpObj->ChaosMoney = 2000000;
		ExtraBlessGemCount = BlessGemCount - 1;
		ExtraSoulGemCount = SoulGemCount - 1;
	}
	else if ( Plus10ItemCount == 1 && ChaosGemCount == 1 && BlessGemCount == 2 && SoulGemCount == 2 && Plus9ItemCount == 0 && Plus11ItemCount == 0 && Plus12ItemCount == 0 )
	{
		MixType = 4;
		lpObj->ChaosMoney = 4000000;
		ExtraBlessGemCount = BlessGemCount - 2;
		ExtraSoulGemCount = SoulGemCount - 2;
	}
	else if ( Plus11ItemCount == 1 && ChaosGemCount == 1 && BlessGemCount == 3 && SoulGemCount == 3 && Plus9ItemCount == 0 && Plus10ItemCount == 0 && Plus12ItemCount == 0 )
	{
		MixType = 22;
		lpObj->ChaosMoney = 6000000;
		ExtraBlessGemCount = BlessGemCount - 3;
		ExtraSoulGemCount = SoulGemCount - 3;
	}
	else if ( Plus12ItemCount == 1 && ChaosGemCount == 1 && BlessGemCount == 4 && SoulGemCount == 4 && Plus9ItemCount == 0 && Plus10ItemCount == 0 && Plus11ItemCount == 0 )
	{
		MixType = 23;
		lpObj->ChaosMoney = 8000000;
		ExtraBlessGemCount = BlessGemCount - 4;
		ExtraSoulGemCount = SoulGemCount - 4;
	}
	else if ( Plus13ItemCount == 1 && ChaosGemCount == 1 && BlessGemCount == 5 && SoulGemCount == 5 && Plus9ItemCount == 0 && Plus10ItemCount == 0 && Plus11ItemCount == 0 && Plus12ItemCount == 0 )
	{
		MixType = 49;
		lpObj->ChaosMoney = 10000000;
		ExtraBlessGemCount = BlessGemCount - 5;
		ExtraSoulGemCount = SoulGemCount - 5;
	}
	else if ( Plus14ItemCount == 1 && ChaosGemCount == 1 && BlessGemCount == 6 && SoulGemCount == 6 && Plus9ItemCount == 0 && Plus10ItemCount == 0 && Plus11ItemCount == 0 && Plus12ItemCount == 0 && Plus13ItemCount == 0 )
	{
		MixType = 50;
		lpObj->ChaosMoney = 12000000;
		ExtraBlessGemCount = BlessGemCount - 6;
		ExtraSoulGemCount = SoulGemCount - 6;
	}

	if ( MixType != mixType )
	{
		MixType = 0;
	}

	if ( MixType == 0 )
	{
		return FALSE;
	}

	PMSG_CHAOSMIXRESULT pMsg;

	PHeadSetB((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR;
	lpObj->ChaosLock = TRUE;
	::LogPlusItemLevelChaosItem(lpObj, MixType);
	CHAOS_LOG.Output("[PlusItemLevel] Chaos Mix Start");

	int iChaosTaxMoney = (int)((__int64)lpObj->ChaosMoney * (__int64)g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / (__int64)100);

	if ( iChaosTaxMoney < 0 )
	{
		iChaosTaxMoney = 0;
	}

	lpObj->ChaosMoney += iChaosTaxMoney;

	if ( lpObj->ChaosMoney < 0 )
	{
		lpObj->ChaosMoney = 0;
	}

	if ( lpObj->Money < lpObj->ChaosMoney )
	{
		CHAOS_LOG.Output("[PlusItemLevel] [%s][%s] CBMix Not Enough Money [%d] need zen [%d]",
			lpObj->AccountID, lpObj->Name, lpObj->Money, lpObj->ChaosMoney);
		pMsg.Result = CB_NOT_ENOUGH_ZEN;
		DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;
		
		return TRUE;
	}


	lpObj->Money -= lpObj->ChaosMoney;
	g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);
	GCMoneySend(lpObj->m_Index, lpObj->Money);

	if ( lpObj->pChaosBox[PlusItemPos].m_Level == 9 )
	{
		lpObj->ChaosSuccessRate = ( ReadConfig.PlusItemLevelChaosMix10 )&255;

		if ( lpObj->ChaosSuccessRate > ReadConfig.PlusItemLevelChaosMaxRate10 )
		{
			lpObj->ChaosSuccessRate = (ReadConfig.PlusItemLevelChaosMaxRate10)&255;
		}
	}
	else if ( lpObj->pChaosBox[PlusItemPos].m_Level == 10 )
	{
		lpObj->ChaosSuccessRate = ( ReadConfig.PlusItemLevelChaosMix11 )&255;

		if ( lpObj->ChaosSuccessRate > ReadConfig.PlusItemLevelChaosMaxRate11 )
		{
			lpObj->ChaosSuccessRate = (ReadConfig.PlusItemLevelChaosMaxRate11)&255;
		}
	}
	else if ( lpObj->pChaosBox[PlusItemPos].m_Level == 11 )
	{
		lpObj->ChaosSuccessRate = ( ReadConfig.PlusItemLevelChaosMix12 )&255;

		if ( lpObj->ChaosSuccessRate > ReadConfig.PlusItemLevelChaosMaxRate12 )
		{
			lpObj->ChaosSuccessRate = (ReadConfig.PlusItemLevelChaosMaxRate12)&255;
		}
	}
	else if ( lpObj->pChaosBox[PlusItemPos].m_Level == 12 )
	{
		lpObj->ChaosSuccessRate = ( ReadConfig.PlusItemLevelChaosMix13 )&255;

		if ( lpObj->ChaosSuccessRate > ReadConfig.PlusItemLevelChaosMaxRate13 )
		{
			lpObj->ChaosSuccessRate = (ReadConfig.PlusItemLevelChaosMaxRate13)&255;
		}
	}
	else if ( lpObj->pChaosBox[PlusItemPos].m_Level == 13 )
	{
		lpObj->ChaosSuccessRate = ( ReadConfig.PlusItemLevelChaosMix14 )&255;

		if ( lpObj->ChaosSuccessRate > ReadConfig.PlusItemLevelChaosMaxRate14 )
		{
			lpObj->ChaosSuccessRate = (ReadConfig.PlusItemLevelChaosMaxRate14)&255;
		}
	}
	else if ( lpObj->pChaosBox[PlusItemPos].m_Level == 14 )
	{
		lpObj->ChaosSuccessRate = ( ReadConfig.PlusItemLevelChaosMix15 )&255;

		if ( lpObj->ChaosSuccessRate > ReadConfig.PlusItemLevelChaosMaxRate15 )
		{
			lpObj->ChaosSuccessRate = (ReadConfig.PlusItemLevelChaosMaxRate15)&255;
		}
	}

	btItemLuckValue = lpObj->pChaosBox[PlusItemPos].m_LuckOption;

	if ( btItemLuckValue )
	{
		CHAOS_LOG.Output("[CBMix][%s][%s] Extra Luck Percent brings mix up to: %d from %d", 
			lpObj->AccountID, lpObj->Name,
			lpObj->ChaosSuccessRate+ReadConfig.PlusItemLuckPercent,
			lpObj->ChaosSuccessRate
		);

		lpObj->ChaosSuccessRate+=ReadConfig.PlusItemLuckPercent;
	}

	lpObj->ChaosSuccessRate = ( lpObj->ChaosSuccessRate + iCharmOfLuckCount )&255;

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

	if (lpObj->ChaosSuccessRate > 100)
		lpObj->ChaosSuccessRate = 100;

	int iRand = (rand()%100);
	if (  iRand < lpObj->ChaosSuccessRate )
	{
		lpObj->pChaosBox[PlusItemPos].m_Level++;
		pMsg.Result = CB_SUCCESS;

		CItem Item;

		float Dur = ItemGetDurability(lpObj->pChaosBox[PlusItemPos].m_Type, lpObj->pChaosBox[PlusItemPos].m_Level,
			lpObj->pChaosBox[PlusItemPos].IsExtItem(), lpObj->pChaosBox[PlusItemPos].IsSetItem());

		Item.m_Level = lpObj->pChaosBox[PlusItemPos].m_Level;
		Item.m_Durability = Dur * lpObj->pChaosBox[PlusItemPos].m_Durability / lpObj->pChaosBox[PlusItemPos].m_BaseDurability;
		Item.m_JewelOfHarmonyOption = lpObj->pChaosBox[PlusItemPos].m_JewelOfHarmonyOption;

		Item.m_ItemSlot1 = lpObj->pChaosBox[PlusItemPos].m_ItemSlot1;
		Item.m_ItemSlot2 = lpObj->pChaosBox[PlusItemPos].m_ItemSlot2;
		Item.m_ItemSlot3 = lpObj->pChaosBox[PlusItemPos].m_ItemSlot3;
		Item.m_ItemSlot4 = lpObj->pChaosBox[PlusItemPos].m_ItemSlot4;
		Item.m_ItemSlot5 = lpObj->pChaosBox[PlusItemPos].m_ItemSlot5;

		Item.Convert(lpObj->pChaosBox[PlusItemPos].m_Type, lpObj->pChaosBox[PlusItemPos].m_SkillOption, lpObj->pChaosBox[PlusItemPos].m_LuckOption,
			lpObj->pChaosBox[PlusItemPos].m_Z28Option, lpObj->pChaosBox[PlusItemPos].m_NewOption, 
			lpObj->pChaosBox[PlusItemPos].m_SetOption, lpObj->pChaosBox[PlusItemPos].m_ItemOptionEx, CURRENT_DB_VERSION);

		Item.m_Number = lpObj->pChaosBox[PlusItemPos].m_Number;

		ItemByteConvert(pMsg.ItemInfo, Item);
		ChaosBoxInit(lpObj);
		::gObjChaosBoxInsertItemPos(lpObj->m_Index, Item, 0, -1);
		gObjChaosItemSet(lpObj->m_Index, 0, 1);

		DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);

		CHAOS_LOG.Output("[PlusItemLevel] [%s][%s] CBMix Success %d/%d Money : %d-%d [%d], CharmRate : %d",
			lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, iRand, lpObj->Money,
			lpObj->ChaosMoney, Item.m_Level, iCharmOfLuckCount);

		MuItemShop.EarnGoblinPointsCBMix(lpObj->m_Index, lpObj->ChaosSuccessRate, 0);
	}
	else
	{
		int FailLevel = lpObj->pChaosBox[PlusItemPos].m_Level+1;

		if (iChaosRateCount > 0)
		{
			ChaosBoxItemDown(lpObj);	//Chaos Machine Rate implementation [+10 ~ +13]
		} else {
			ChaosBoxInit(lpObj);
		}

		GCUserChaosBoxSend(lpObj, 0);

		DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);

		CHAOS_LOG.Output("[PlusItemLevel] [%s][%s] CBMix Fail %d/%d Money: %d-%d [%d], CharmRate: %d, RateCount: %d",
			lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, iRand, lpObj->Money,
			lpObj->ChaosMoney, FailLevel, iCharmOfLuckCount, iChaosRateCount);

		MuItemShop.EarnGoblinPointsCBMix(lpObj->m_Index, lpObj->ChaosSuccessRate, 1);
	}

	::gObjInventoryCommit(lpObj->m_Index);
	lpObj->ChaosLock = FALSE;
	return TRUE;
}


BOOL PegasiaChaosMix(LPOBJ lpObj)
{
	lpObj->ChaosLock = TRUE;
	int UniriaCount = 0;
	int ChoasGemCount = 0;
	int iCharmOfLuckCount = 0;

	for ( int n=0;n<CHAOS_BOX_SIZE;n++)
	{
		if ( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			if ( lpObj->pChaosBox[n].m_Type == ITEMGET(13,2) && lpObj->pChaosBox[n].m_Durability == 255.0f )	// Uniria
			{
				UniriaCount++;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(12,15) ) // Chaos Gem
			{
				ChoasGemCount++;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,53) )	// Charm Of Luck
			{
				iCharmOfLuckCount += lpObj->pChaosBox[n].m_Durability;
			}
		}
	}

	if ( UniriaCount != 10 || ChoasGemCount != 1 )
	{
		lpObj->ChaosLock = FALSE;
		return FALSE;
	}

	PMSG_CHAOSMIXRESULT pMsg;

	PHeadSetB((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR;

	if ( iCharmOfLuckCount > 10 )
	{
		lpObj->ChaosLock = FALSE;
		pMsg.Result = 0xF0;
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);

		return 0;
	}

	int nChaosNeedMoney = 500000;
	int iChaosTaxMoney = (int)((__int64)nChaosNeedMoney * (__int64)g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / (__int64)100);

	if ( iChaosTaxMoney < 0 )
	{
		iChaosTaxMoney = 0;
	}

	nChaosNeedMoney += iChaosTaxMoney;

	if ( nChaosNeedMoney < 0 )
	{
		nChaosNeedMoney = 0;
	}

	if ( lpObj->Money < nChaosNeedMoney )
	{
		lpObj->ChaosLock = FALSE;
		pMsg.Result = CB_NOT_ENOUGH_ZEN;
		
		DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return TRUE;
	}

	LogChaosItem(lpObj, "DinorantMix");
	CHAOS_LOG.Output("[DinorantMix] Chaos Mix Start");
	lpObj->ChaosSuccessRate = ReadConfig.DinorantMixPercent;	// Succes Rate for Dinorant
	lpObj->ChaosSuccessRate = (lpObj->ChaosSuccessRate + iCharmOfLuckCount )&255;
	lpObj->Money -= nChaosNeedMoney;

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

	g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);
	GCMoneySend(lpObj->m_Index, lpObj->Money);

	int iRate = (rand()%100 );
	if ( iRate < lpObj->ChaosSuccessRate )
	{
		int Dinorant = ITEMGET(13,3);

		int z28Opt = 0;
		int skillOpt = 0;

		if (ReadConfig.DinorantSkillPercent > rand()%100)
			skillOpt = 1;

		if ( (rand()%100) < ReadConfig.DinorantZ28Percent )
		{
			z28Opt = rand()%((ReadConfig.DinorantZ28Max*100)+1);
			z28Opt = z28Opt / 100;
			/*z28Opt = 1 <<  ((rand()%3)) ;

			if ( (rand()%5) == 0 )
			{
				z28Opt |= 1 << (rand()%3);
			}*/
		}

		::ItemSerialCreateSend(lpObj->m_Index, -1, 0, 0, Dinorant, 0, 255, skillOpt, 0, z28Opt, -1, 0, 0);
		CHAOS_LOG.Output("[DinorantMix] [%s][%s] CBMix Success %d/%d Money : %d-%d, CharmRate : %d",
			lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, iRate, lpObj->Money, nChaosNeedMoney, iCharmOfLuckCount);

		::gObjInventoryCommit(lpObj->m_Index);
		MuItemShop.EarnGoblinPointsCBMix(lpObj->m_Index, lpObj->ChaosSuccessRate, 0);
		lpObj->ChaosLock = FALSE;
		return TRUE;
	}
	else
	{
		ChaosBoxInit(lpObj);	// Errase Chaos Box
		GCUserChaosBoxSend(lpObj, 0);
		DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);

		CHAOS_LOG.Output("[DinorantMix] [%s][%s] CBMix Fail %d/%d Money : %d-%d, CharmRate : %d",
			lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, iRate, lpObj->Money, nChaosNeedMoney, iCharmOfLuckCount);

		::gObjInventoryCommit(lpObj->m_Index);
		MuItemShop.EarnGoblinPointsCBMix(lpObj->m_Index, lpObj->ChaosSuccessRate, 1);
		lpObj->ChaosLock = FALSE;
		return FALSE;
	}
}






BOOL CircleChaosMix(LPOBJ lpObj)	// Fruits
{
	lpObj->ChaosLock = TRUE;

	int CreatureGemCount = 0;
	int ChoasGemCount = 0;
	int iCharmOfLuckCount = 0;

	for ( int n=0;n<CHAOS_BOX_SIZE;n++)
	{
		if ( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,22) )	// Jewel of Creation
			{
				CreatureGemCount++;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(12,15) ) // Chaos Gem
			{
				ChoasGemCount++;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,53) )	// Charm Of Luck
			{
				iCharmOfLuckCount += lpObj->pChaosBox[n].m_Durability;
			}
		}
	}

	if ( CreatureGemCount != 1 || ChoasGemCount != 1 )
	{
		return FALSE;
	}

	PMSG_CHAOSMIXRESULT pMsg;

	PHeadSetB((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR;

	if ( iCharmOfLuckCount > 10 )
	{
		lpObj->ChaosLock = FALSE;
		pMsg.Result = 0xF0;
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);

		return 1;
	}

	if ( lpObj->Level < 10 )
	{
		pMsg.Result = CB_LOW_LEVEL_USER;

		DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return TRUE;
	}

	int nChaosNeedMoney = 3000000;
	int iChaosTaxMoney = (int)((__int64)nChaosNeedMoney * (__int64)g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / (__int64)100);

	if ( iChaosTaxMoney < 0 )
	{
		iChaosTaxMoney = 0;
	}

	nChaosNeedMoney += iChaosTaxMoney;

	if ( nChaosNeedMoney < 0 )
	{
		nChaosNeedMoney = 0;
	}

	if ( lpObj->Money < nChaosNeedMoney )
	{
		lpObj->ChaosLock = FALSE;
		pMsg.Result = CB_NOT_ENOUGH_ZEN;
		
		DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return TRUE;
	}

	LogChaosItem(lpObj, "CircleMix");
	CHAOS_LOG.Output("[CircleMix] Chaos Mix Start");
	lpObj->ChaosSuccessRate = ReadConfig.FruitMixPercent;	// Succes Rate for Fruit
	lpObj->ChaosSuccessRate = (lpObj->ChaosSuccessRate + iCharmOfLuckCount )&255;
	lpObj->Money -= nChaosNeedMoney;

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

	g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);
	GCMoneySend(lpObj->m_Index, lpObj->Money);

	int iRate = (rand()%100 );
	if ( iRate < lpObj->ChaosSuccessRate )
	{
		int Fruit = ITEMGET(13,15);	// Fruit
		int FruitType;
		int RandonValue = rand() % 100;

		if ( RandonValue < 30 )
		{
			FruitType = 0;
		}
		else if ( RandonValue < 55 )
		{
			FruitType = 1;
		}
		else if ( RandonValue < 75 )
		{
			FruitType = 2;
		}
		else if ( RandonValue < 95 )
		{
			FruitType = 3;
		}
		else
		{
			FruitType = 4;
		}

		::ItemSerialCreateSend(lpObj->m_Index, -1, 0, 0, Fruit, FruitType, 255, 1, 0, 0, -1, 0, 0);

		CHAOS_LOG.Output("[CircleMix] [%s][%s] CBMix Success %d/%d Money : %d-%d, CharmRate : %d",
			lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, iRate, lpObj->Money, nChaosNeedMoney, iCharmOfLuckCount);

		::gObjInventoryCommit(lpObj->m_Index);
		MuItemShop.EarnGoblinPointsCBMix(lpObj->m_Index, lpObj->ChaosSuccessRate, 0);
		lpObj->ChaosLock = FALSE;
		return TRUE;
	}
	else
	{
		ChaosBoxInit(lpObj);
		GCUserChaosBoxSend(lpObj, 0);

		DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);
		CHAOS_LOG.Output("[CircleMix] [%s][%s] CBMix Fail %d/%d Money : %d-%d, CharmRate : %d",
			lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, iRate, lpObj->Money, nChaosNeedMoney, iCharmOfLuckCount);

		::gObjInventoryCommit(lpObj->m_Index);
		MuItemShop.EarnGoblinPointsCBMix(lpObj->m_Index, lpObj->ChaosSuccessRate, 1);
		lpObj->ChaosLock = FALSE;
		return FALSE;
	}
}



BOOL WingChaosMix(LPOBJ lpObj)
{
	lpObj->ChaosLock = TRUE;

	int WingCount = 0;
	int ChoasGemCount = 0;
	int LokesFeathersCount = 0;
	int WingIndex = -1;
	int iChaosMoney = 0;
	int iWingChaosMoney = 0;
	int iSleeveOfLord = 0;
	int iCharmOfLuckCount = 0;
	int WingLvl2Count = 0;
	for ( int n=0;n<CHAOS_BOX_SIZE;n++)
	{
		if ( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			if ( (lpObj->pChaosBox[n].m_Type >= ITEMGET(12,3) && lpObj->pChaosBox[n].m_Type <= ITEMGET(12,6)) || lpObj->pChaosBox[n].m_Type == ITEMGET(14,30)  || lpObj->pChaosBox[n].m_Type == ITEMGET(12,42) )	
			{
				lpObj->ChaosLock = FALSE;

				return FALSE;
			}
			if ( lpObj->pChaosBox[n].m_Type >= ITEMGET(12,0) && lpObj->pChaosBox[n].m_Type <= ITEMGET(12,2) || lpObj->pChaosBox[n].m_Type  >= ITEMGET(12,3) && lpObj->pChaosBox[n].m_Type <= ITEMGET(12,6) || lpObj->pChaosBox[n].m_Type == ITEMGET(14,30) || lpObj->pChaosBox[n].m_Type >= ITEMGET(12,41) && lpObj->pChaosBox[n].m_Type <= ITEMGET(12,42))
			{
				WingCount++;
				WingIndex = n;
				iWingChaosMoney = lpObj->pChaosBox[n].m_BuyMoney;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(12,15) ) // Chaos
			{
				ChoasGemCount++;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(13,14) ) // Feather
			{
				if ( lpObj->pChaosBox[n].m_Level == 0 ) // Feather
				{
					LokesFeathersCount++;
				}
				else	// Crst of Monarch
				{
					iSleeveOfLord ++;
				}
			}

			else if ( lpObj->pChaosBox[n].m_Type >= ITEMGET(12,3) && lpObj->pChaosBox[n].m_Type <= ITEMGET(12,6) || lpObj->pChaosBox[n].m_Type == ITEMGET(14,30) || lpObj->pChaosBox[n].m_Type == ITEMGET(12,42))
			{
				WingLvl2Count++; // if lvl2 wing is present
			}

			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,53) )	// Charm Of Luck
			{
				iCharmOfLuckCount += lpObj->pChaosBox[n].m_Durability;
			}
			else if ( lpObj->pChaosBox[n].IsExtItem() != FALSE )
			{
				if ( lpObj->pChaosBox[n].m_Level >= 4 )
				{
					iChaosMoney += lpObj->pChaosBox[n].m_BuyMoney;
				}
			}
		}
	}

	PMSG_CHAOSMIXRESULT pMsg;

	PHeadSetB((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR;

	

	if ( iSleeveOfLord == 1 )
	{
		if ( WingCount != 1 || ChoasGemCount != 1 || LokesFeathersCount != 0 )
		{
			lpObj->ChaosLock = FALSE;
			pMsg.Result = CB_INCORRECT_MIX_ITEMS;
			DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);
			return FALSE;
		}
	}
	else if ( LokesFeathersCount == 1 )
	{
		if ( WingCount != 1 || ChoasGemCount != 1 || iSleeveOfLord != 0 )
		{
			pMsg.Result = CB_INCORRECT_MIX_ITEMS;
			DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);
			lpObj->ChaosLock = FALSE;

			return FALSE;
		}
	}
	else
	{
		lpObj->ChaosLock = FALSE;
		pMsg.Result = CB_INCORRECT_MIX_ITEMS;
		DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);

		return FALSE;
	}

	if ( iCharmOfLuckCount > 10 )
	{
		lpObj->ChaosLock = FALSE;
		pMsg.Result = 0xF0;
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
	}

	if ( WingLvl2Count = WingLvl2Count == 1)
	{
		if(WingLvl2Count != 1 || ChoasGemCount != 1)
		{
			pMsg.Result = CB_INCORRECT_MIX_ITEMS;
			DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);
			lpObj->ChaosLock = FALSE;
			return FALSE;
		}
	}

	int nChaosNeedMoney = 5000000;
	int iChaosTaxMoney = (int)((__int64)nChaosNeedMoney * (__int64)g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / (__int64)100);

	if ( iChaosTaxMoney < 0 )
	{
		iChaosTaxMoney = 0;
	}

	nChaosNeedMoney += iChaosTaxMoney;

	if ( nChaosNeedMoney < 0 )
	{
		nChaosNeedMoney = 0;
	}

	if ( lpObj->Money < nChaosNeedMoney )
	{
		pMsg.Result = CB_NOT_ENOUGH_ZEN;
		
		DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return TRUE;
	}

	//CItem * pWing = &lpObj->pChaosBox[WingIndex];
	lpObj->ChaosSuccessRate = ( (DWORD)((DWORD)iWingChaosMoney / (DWORD)4000000) )&255;
	lpObj->ChaosSuccessRate = ( lpObj->ChaosSuccessRate + iChaosMoney / 40000 )&255;

	if ( lpObj->ChaosSuccessRate == 0 )
	{
		pMsg.Result = CB_INCORRECT_MIX_ITEMS;
		DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return FALSE;
	}

	LogChaosItem(lpObj, "WingMix,2");
	CHAOS_LOG.Output("[WingMix] Chaos Mix Start");

	if ( iSleeveOfLord == 1 )
	{
		if ( lpObj->ChaosSuccessRate > ReadConfig.DLRFCapeMaxPercent)
		{
			lpObj->ChaosSuccessRate = ReadConfig.DLRFCapeMaxPercent;
		}
		if ( lpObj->ChaosSuccessRate < ReadConfig.DLRFCapeMinPercent)
		{
			lpObj->ChaosSuccessRate = ReadConfig.DLRFCapeMinPercent;
		}
	}
	else if ( LokesFeathersCount == 1 )
	{
		if ( lpObj->ChaosSuccessRate > ReadConfig.SecondWingMaxPercent )
		{
			lpObj->ChaosSuccessRate =  ReadConfig.SecondWingMaxPercent;
		}
		if ( lpObj->ChaosSuccessRate < ReadConfig.SecondWingMinPercent )
		{
			lpObj->ChaosSuccessRate =  ReadConfig.SecondWingMinPercent;
		}
	}
	else if ( lpObj->ChaosSuccessRate > 100 )
	{
		lpObj->ChaosSuccessRate = 100;
	}

	lpObj->ChaosSuccessRate = ( lpObj->ChaosSuccessRate + iCharmOfLuckCount )&255;

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

	lpObj->Money  -= nChaosNeedMoney;
	g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);
	GCMoneySend(lpObj->m_Index, lpObj->Money);

	int iRate = (rand()%100);
	if ( iRate < lpObj->ChaosSuccessRate )
	{
		int iWingLevel = 0;
		int iItemType;
		int iItemSubType;

		if ( iSleeveOfLord != 0 )
		{
			int ran = 0;
			ran = rand()%100;
			if(ReadConfig.S6E1 == TRUE && ran <= ReadConfig.RFCapeProbability)			//Rage Fighter Cape
			{
				iItemType = 12;
				iItemSubType = 49;
			}else											//Dark Lord Cape
			{
				iItemType = 13;
				iItemSubType = 30;
			}
		}
		else if (WingLvl2Count != 0)
		{
			iItemType = 12;
			iItemSubType = 36;
		}
		else
		{
			iItemType = 12;
			iItemSubType = rand()%5 + 3;

			if(iItemSubType == 7)
				iItemSubType = 42;
		}

		int iWingNum = ITEMGET(iItemType, iItemSubType);
		int iOption1 = 0;
		int iOption2 = 0;
		int iOption3 = 0;

		//Luck
		if ( iSleeveOfLord != FALSE )
		{
			if (ReadConfig.DLRFCapeLuckPercent > (rand()%100))
				iOption1 = 1;
		} else {
			if (ReadConfig.SecondWingLuckPercent > (rand()%100))
				iOption1 = 1;
		}

		//Z28
		if ( iSleeveOfLord != FALSE )
		{
			if (ReadConfig.DLRFCapeZ28Percent > (rand()%100))
			{
				int z28Percent = ReadConfig.DLRFCapeZ28Max*100;
				iOption2 = rand()%(z28Percent+1);
				iOption2 = iOption2 / 100;
			}
		} else {
			if (ReadConfig.SecondWingZ28Percent > (rand()%100))
			{
				int z28Percent = ReadConfig.SecondWingZ28Max*100;
				iOption2 = rand()%(z28Percent+1);
				iOption2 = iOption2 / 100;
			}
		}
		
		if (ReadConfig.SecondWingZ28Percent == 0 &&
			ReadConfig.DLRFCapeZ28Percent == 0)
		{
			int iRandomValue = rand() % 100;
			int iRandomValue2 = rand() % 3;

			switch ( iRandomValue2 )
			{
				case 0:
					if ( iRandomValue < 4 )
					{
						iOption2 = 3;	// +12
					}
					break;

				case 1:
					if ( iRandomValue < 10 )
					{
						iOption2 = 2;	// +8
					}
					break;

				case 2:
					if ( iRandomValue < 20 )
					{
						iOption2 = 1;	// +4;
					}
			}
		}

		//Exc Options
		if ( iSleeveOfLord != FALSE )
		{
			if (ReadConfig.DLRFIsUseNewExcMixSystem == 1)
			{
				if ( (rand()%100) < ReadConfig.DLRFExcOptionProbability )
				{
					iOption3 = BoxExcOptions( ReadConfig.DLRFMaxExcOption );
				}
			} else {
				if ( (rand()%5) == 0 )
				{
					iOption3 = 1 << (rand()%4);
				}
				iOption3 |= 0x20;
			}
		}
		else
		{
			if (ReadConfig.SecondWingIsUseNewExcMix == 1)
			{
				if ( (rand()%100) < ReadConfig.SecondWingExcOptionProb )
				{
					iOption3 = BoxExcOptions( ReadConfig.SecondWingMaxExcOption );
				}
			} else {
				if ( (rand()%5) == 0 )
				{
					iOption3 = 1 << (rand()%3);
				}

				if ( (rand()%2) != 0 )
				{
					iOption3 |= 0x20;
				}
			}
		}

		::ItemSerialCreateSend(lpObj->m_Index, -1, 0, 0, iWingNum, iWingLevel, 0, 0, iOption1, iOption2, -1, iOption3, 0);
		CHAOS_LOG.Output("[WingMix] [%s][%s] CBMix Success %d/%d Money : %d-%d, CharmRate : %d",
			lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, iRate, 
			lpObj->Money, nChaosNeedMoney, iCharmOfLuckCount);

		::gObjInventoryCommit(lpObj->m_Index);
		MuItemShop.EarnGoblinPointsCBMix(lpObj->m_Index, lpObj->ChaosSuccessRate, 0);
		lpObj->ChaosLock = FALSE;
		return TRUE;
	}
	else
	{
		for ( int n=0;n<CHAOS_BOX_SIZE;n++)
		{
			lpObj->pChaosBox[n].Clear();
		}

		GCUserChaosBoxSend(lpObj, 0);
		DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);
		CHAOS_LOG.Output("[WingMix] [%s][%s] CBMix Fail %d/%d Money : %d-%d, CharmRate : %d",
			lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, iRate, 
			lpObj->Money, nChaosNeedMoney, iCharmOfLuckCount);
		
		::gObjInventoryCommit(lpObj->m_Index);
		MuItemShop.EarnGoblinPointsCBMix(lpObj->m_Index, lpObj->ChaosSuccessRate, 1);
		lpObj->ChaosLock = FALSE;
		return FALSE;
	}
}

BOOL Wing3ChaosMix(LPOBJ lpObj)
{
	lpObj->ChaosLock = TRUE;
	int WingCount = 0;
	int ItemCount = 0;
	int ChoasGemCount = 0;
	int CondorCrystalCount = 0;
	int CondorFeatherCount = 0;
	int BundleOfBlessCount = 0;
	int BundleOfSoulCount = 0;
	int JewelOfCreationCount = 0;
	//int ExcItemCount = 0;
	//int SetItemCount = 0;
	int WingIndex = -1;
	int iChaosMoney = 0;
	int iWingChaosMoney = 0;

	for ( int n=0;n<CHAOS_BOX_SIZE;n++)
	{
		if ( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			if ( (lpObj->pChaosBox[n].m_Type >= ITEMGET(12,3) && 
				lpObj->pChaosBox[n].m_Type <= ITEMGET(12,6)) )		// Second Wings
			{
				WingCount++;
				WingIndex = n;
				iWingChaosMoney = lpObj->pChaosBox[n].m_BuyMoney;
			}

			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(13,30) ) // Dark Lord Cape
			{
				WingCount++;
				WingIndex = n;
				iWingChaosMoney = lpObj->pChaosBox[n].m_BuyMoney;
			}

			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(12,49) ) // Wing of Rage Fighter
			{
				WingCount++;
				WingIndex = n;
				iWingChaosMoney = lpObj->pChaosBox[n].m_BuyMoney;
			}

			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(12,42) ) // Summoner Wings [Wing of Despair]
			{
				WingCount++;
				WingIndex = n;
				iWingChaosMoney = lpObj->pChaosBox[n].m_BuyMoney;
			}

			else if (  (lpObj->pChaosBox[n].m_Type >= ITEMGET(0,0) && //Other Items for 3rd Combination
				lpObj->pChaosBox[n].m_Type <= ITEMGET(12,0)) )
			{
				ItemCount++;
				iChaosMoney += lpObj->pChaosBox[n].m_BuyMoney;
			}

			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(12,15) ) // Chaos
			{
				ChoasGemCount++;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(13,52) ) // Crystal
			{
				CondorCrystalCount++;
			}

			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(13,53) ) // Condor Feather
			{
				CondorFeatherCount++;
			}


			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,22))	// Jewel of Creation
			{
				JewelOfCreationCount++;
			}

			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(12,30))	// Bundle of Bless
			{
				BundleOfBlessCount++;
			}

			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(12,31)) // Bundle of Soul
			{
				BundleOfSoulCount++;
			}
		}
	}

	PMSG_CHAOSMIXRESULT pMsg;

	PHeadSetB((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR;

	
	if ( WingCount == 1 )
	{
		if ( JewelOfCreationCount != 1 || ChoasGemCount != 1 || BundleOfBlessCount != 0 )
		{
			lpObj->ChaosLock = FALSE;
			pMsg.Result = CB_INCORRECT_MIX_ITEMS;
			DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);
			return FALSE;
		}
	}

	else if ( CondorFeatherCount == 1 )
	{
		if ( CondorCrystalCount != 1 || JewelOfCreationCount != 1 || ChoasGemCount != 1 || BundleOfBlessCount != 1 || BundleOfSoulCount != 1  )
		{
			pMsg.Result = CB_INCORRECT_MIX_ITEMS;
			DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);
			lpObj->ChaosLock = FALSE;

			return FALSE;
		}
	}

	int nChaosNeedMoney = 5000000;
	int iChaosTaxMoney = (int)((__int64)nChaosNeedMoney * (__int64)g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / (__int64)100);

	if ( iChaosTaxMoney < 0 )
	{
		iChaosTaxMoney = 0;
	}

	nChaosNeedMoney += iChaosTaxMoney;

	if ( nChaosNeedMoney < 0 )
	{
		nChaosNeedMoney = 0;
	}

	if ( lpObj->Money < nChaosNeedMoney )
	{
		pMsg.Result = CB_NOT_ENOUGH_ZEN;
		
		DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return TRUE;
	}

	//CItem * pWing = &lpObj->pChaosBox[WingIndex];
	lpObj->ChaosSuccessRate = ( (DWORD)((DWORD)iWingChaosMoney / (DWORD)4000000) )&255;

	if (iChaosMoney > 0)
	{
		lpObj->ChaosSuccessRate = ( lpObj->ChaosSuccessRate + (iChaosMoney / 2800000) )&255;
	}
	if (iChaosMoney < 0)
	{
		lpObj->ChaosSuccessRate = ( lpObj->ChaosSuccessRate + (-1*iChaosMoney / 2800000) )&255;
	}


	if ( lpObj->ChaosSuccessRate == 0 && CondorFeatherCount != 1 )
	{
		pMsg.Result = CB_INCORRECT_MIX_ITEMS;
		DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return FALSE;
	}

	LogChaosItem(lpObj, "WingMix,3");
	CHAOS_LOG.Output("[WingMix 3] Chaos Mix Start");

	if ( WingCount == 1 )
	{
		if ( lpObj->ChaosSuccessRate > ReadConfig.CondorMixMaxPercent)
		{
			lpObj->ChaosSuccessRate = ReadConfig.CondorMixMaxPercent;
		}
		if ( lpObj->ChaosSuccessRate < ReadConfig.CondorMixMinPercent)
		{
			lpObj->ChaosSuccessRate = ReadConfig.CondorMixMinPercent;
		}
	}
	else if ( CondorFeatherCount == 1 )
	{
		if ( lpObj->ChaosSuccessRate > ReadConfig.ThirdWingMaxPercent )
		{
			lpObj->ChaosSuccessRate =  ReadConfig.ThirdWingMaxPercent;
		}
		if ( lpObj->ChaosSuccessRate < ReadConfig.ThirdWingMinPercent )
		{
			lpObj->ChaosSuccessRate =  ReadConfig.ThirdWingMinPercent;
		}
	}
	else if ( lpObj->ChaosSuccessRate > 100 )
	{
		lpObj->ChaosSuccessRate = 100;
	}

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

	lpObj->Money  -= nChaosNeedMoney;
	g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);

	GCMoneySend(lpObj->m_Index, lpObj->Money);

	int SuccessRateRandom = 0;
	SuccessRateRandom = (rand()%100);

	if ( SuccessRateRandom < lpObj->ChaosSuccessRate )
	{
		int iWingLevel = 0;
		int iItemType = 0;
		int iItemSubType = 0;

		if ( CondorFeatherCount != 0 )
		{
			BYTE wingID = 0;
			if(ReadConfig.S6E1 == TRUE)
			{
				wingID = rand()%7;
			}else
			{
				wingID = rand()%6;
			}
			iItemType = 12;
			iItemSubType = 36 + wingID;

			if (wingID == 5)
			{
				iItemType = 12;
				iItemSubType = 43;
			}else if(wingID == 6)
			{
				iItemType = 12;
				iItemSubType = 50;
			}
		}
		else if (WingCount != 0)
		{
			iItemType = 13;
			iItemSubType = 53;
		}

		int iWingNum = ITEMGET(iItemType, iItemSubType);
		int iOption1 = 0;
		int iOption2 = 0;
		int iOption3 = 0;

		//Luck
		if (ReadConfig.ThirdWingLuckPercent > (rand()%100))
		{
			iOption1 = 1;
		}

		//Z28 Options
		if (ReadConfig.ThirdWingZ28Percent > (rand()%100))
		{
			int z28Percent = ReadConfig.ThirdWingZ28Max*100;
			iOption2 = rand()%(z28Percent+1);
			iOption2 = iOption2 / 100;
		}

		if (ReadConfig.ThirdWingZ28Percent == 0)
		{
			int iRandomValue = rand() % 100;
			int iRandomValue2 = rand() % 3;

			switch ( iRandomValue2 )
			{
				case 0:
					if ( iRandomValue < 4 )
					{
						iOption2 = 3;	// +12
					}
				break;

				case 1:
					if ( iRandomValue < 10 )
					{
						iOption2 = 2;	// +8
					}
				break;
	
				case 2:
					if ( iRandomValue < 20 )
					{
						iOption2 = 1;	// +4;
					}
				break;
			}
		}

		//Exc Options
		if ( CondorFeatherCount == 1 )
		{
			if (ReadConfig.ThirdWingIsUseNewExcMix == 1)
			{
				if ( (rand()%100) < ReadConfig.ThirdWingExcOptionProb )
				{
					iOption3 = BoxExcOptions( ReadConfig.ThirdWingMaxExcOption );
				}
			} else {
				if ( (rand()%5) == 0 )
				{
					iOption3 = 1 << (rand()%4);
				}
				iOption3 |= 0x20;
			}
		}

		::ItemSerialCreateSend(lpObj->m_Index, -1, 0, 0, iWingNum, iWingLevel, 0, 0, iOption1, iOption2, -1, iOption3, 0);
		CHAOS_LOG.Output("[WingMix 3] [%s][%s] CBMix Success %d/%d Money : %d-%d (%d,%d)",
			lpObj->AccountID, lpObj->Name, 
			lpObj->ChaosSuccessRate, SuccessRateRandom,
			lpObj->Money, nChaosNeedMoney, 
			iChaosMoney, iWingChaosMoney);

		::gObjInventoryCommit(lpObj->m_Index);
		MuItemShop.EarnGoblinPointsCBMix(lpObj->m_Index, lpObj->ChaosSuccessRate, 0);
		lpObj->ChaosLock = FALSE;
		return TRUE;
	}
	else
	{
		for ( int n=0;n<CHAOS_BOX_SIZE;n++)
		{
			lpObj->pChaosBox[n].Clear();
		}

		GCUserChaosBoxSend(lpObj, 0);
		DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);
		CHAOS_LOG.Output("[WingMix 3] [%s][%s] CBMix Fail %d/%d Money : %d-%d (%d,%d)",
			lpObj->AccountID, lpObj->Name, 
			lpObj->ChaosSuccessRate, SuccessRateRandom, 
			lpObj->Money, nChaosNeedMoney, 
			iChaosMoney, iWingChaosMoney);
		
		::gObjInventoryCommit(lpObj->m_Index);
		MuItemShop.EarnGoblinPointsCBMix(lpObj->m_Index, lpObj->ChaosSuccessRate, 1);
		lpObj->ChaosLock = FALSE;
		return FALSE;
	}
}




void DefaultChaosMix(LPOBJ lpObj)
{
	PMSG_CHAOSMIXRESULT pMsg;

	PHeadSetB((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR;
	int aIndex = lpObj->m_Index;
	BOOL fail = TRUE;
	int MixResult2;
	lpObj->ChaosLock = TRUE;

	if ( ChaosBoxMix(lpObj, MixResult2) == 0 )
	{
		pMsg.Result = CB_INCORRECT_MIX_ITEMS;
		DataSend(aIndex, (BYTE *)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	int iChaosTaxMoney = (int)((__int64)lpObj->ChaosMoney * (__int64)g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / (__int64)100);

	if ( iChaosTaxMoney < 0 )
	{
		iChaosTaxMoney = 0;
	}

	lpObj->ChaosMoney += iChaosTaxMoney;

	if ( lpObj->ChaosMoney < 0 )
	{
		lpObj->ChaosMoney = 0;
	}

	if ( lpObj->Money < lpObj->ChaosMoney )
	{
		pMsg.Result = CB_NOT_ENOUGH_ZEN;
		
		DataSend(aIndex, (BYTE *)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;
	}
	else 
	{
		int iRate = (rand()%100);

		if ( lpObj->ChaosSuccessRate > 0 )
		{
			if ( lpObj->ChaosSuccessRate >= 100 )
			{
				lpObj->ChaosSuccessRate = 100;
			}

			if ( MixResult2 == TRUE )	//Mixing Wings
			{
				if (lpObj->ChaosSuccessRate < ReadConfig.FirstWingMinPercent)
					lpObj->ChaosSuccessRate = ReadConfig.FirstWingMinPercent;
				if (lpObj->ChaosSuccessRate > ReadConfig.FirstWingMaxPercent)
					lpObj->ChaosSuccessRate = ReadConfig.FirstWingMaxPercent;
			} else {					//Mixing Chaos Weapons
				if (lpObj->ChaosSuccessRate < ReadConfig.ChaosWeaponMinPercent)
					lpObj->ChaosSuccessRate = ReadConfig.ChaosWeaponMinPercent;
				if (lpObj->ChaosSuccessRate > ReadConfig.ChaosWeaponMaxPercent)
					lpObj->ChaosSuccessRate = ReadConfig.ChaosWeaponMaxPercent;

			}

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

			if ( iRate < lpObj->ChaosSuccessRate ) 
			{
				fail = FALSE;
				pMsg.Result = CB_SUCCESS;

				int Level = rand()%5;
				int Option1 = 0;	//Skill
				int Option2 = 0;	//Luck
				int Option3 = 0;	//Opt

				/*if ( (rand()%100)< (lpObj->ChaosSuccessRate/5 + 6) )
				{
					Option1 = 1;
				}

				if ( (rand()%100)< (lpObj->ChaosSuccessRate/5 + 4) )
				{
					Option2 = 1;
				}

				int OpProb;
				int OpType = rand()%3;
				OpProb = rand()%100;

				switch ( OpType )
				{
					case 0:
						if ( OpProb < (lpObj->ChaosSuccessRate / 5 + 4) ) 
						{
							Option3 = 3;
						}
						break;

					case 1:
						if ( OpProb < (lpObj->ChaosSuccessRate / 5 + 8) ) 
						{
							Option3 = 2;
						}
						break;

					case 2:
						if ( OpProb < (lpObj->ChaosSuccessRate / 5 + 12) ) 
						{
							Option3 = 1;
						}
						break;
				}*/

				if ( MixResult2 == TRUE )
				{
					Level = rand()%(ReadConfig.FirstWingMaxMixLevel+1);

					if (rand()%100 < ReadConfig.FirstWingSkillPercent)
						Option1 = 1;

					if (rand()%100 < ReadConfig.FirstWingLuckPercent)
						Option2 = 1;

					if (ReadConfig.FirstWingZ28Percent > rand()%100)
					{
						int z28Percent = ReadConfig.FirstWingZ28Max*100;
						Option3 = rand()%(z28Percent+1);
						Option3 = Option3 / 100;
					}

					int WingType = rand()%4;
					int WingNum = 0;

					if ( WingType == 0 )
					{
						WingNum = ITEMGET(12,0);
					}
					else if ( WingType == 1 )
					{
						WingNum = ITEMGET(12,1);
					}
					else if ( WingType == 2 )
					{
						WingNum = ITEMGET(12,2);
					}
					else if ( WingType == 3 )
					{
						WingNum = ITEMGET(12,41);
					}

					::ItemSerialCreateSend(lpObj->m_Index, -1, 0, 0, WingNum, 0, 255, Option1, Option2, Option3, -1, 0, 0);
				}
				else
				{
					Level = rand()%(ReadConfig.ChaosWeaponMaxMixLevel+1);

					if (rand()%100 < ReadConfig.ChaosWeaponSkillPercent)
						Option1 = 1;

					if (rand()%100 < ReadConfig.ChaosWeaponLuckPercent)
						Option2 = 1;

					if (ReadConfig.ChaosWeaponZ28Max > 0)
					{
						int z28Percent = ReadConfig.ChaosWeaponZ28Max*100;
						Option3 = rand()%(z28Percent+1);
						Option3 = Option3 / 100;
					}

					int ChaosItemType = rand()%3;
					int ChaosItemNum = 0;

					if ( ChaosItemType == 0 )		// Chaos Dragon Axe
					{
						ChaosItemNum = ITEMGET(2,6);
					}
					else if ( ChaosItemType == 1 )	// Chaos Nature Bow
					{
						ChaosItemNum = ITEMGET(4,6);
					}
					else if ( ChaosItemType == 2 )	// Chaos Lighting Staff
					{
						ChaosItemNum = ITEMGET(5,7);
					}

					int excOpt = 0;
					if (ReadConfig.IsAllowExcChaosWeaponMix == 1)
					{
						if (rand()%100 < ReadConfig.ExcChaosWeaponPercent)
						{
							excOpt = BoxExcOptions(ReadConfig.ExcChaosWeaponMaxOpt);
						}
					}

					::ItemSerialCreateSend(lpObj->m_Index, -1, 0, 0, ChaosItemNum, Level, 255, Option1, Option2, Option3, -1, excOpt, 0);
				}
			}
		}


		lpObj->Money -= lpObj->ChaosMoney;
		g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);
		GCMoneySend(lpObj->m_Index, lpObj->Money);

		if ( fail == TRUE )
		{
			ChaosBoxItemDown(lpObj);
			GCUserChaosBoxSend(lpObj, 0);
			DataSend(aIndex, (BYTE*)&pMsg, pMsg.h.size);
			CHAOS_LOG.Output("[%s][%s] CBMix Fail %d/%d Money : %d-%d",
				lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, iRate,
				lpObj->Money, lpObj->ChaosMoney);

			MuItemShop.EarnGoblinPointsCBMix(lpObj->m_Index, lpObj->ChaosSuccessRate, 0);
		}
		else
		{
			CHAOS_LOG.Output("[%s][%s] CBMix Success %d/%d Money : %d",
				lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, iRate,
				lpObj->ChaosMoney);

			MuItemShop.EarnGoblinPointsCBMix(lpObj->m_Index, lpObj->ChaosSuccessRate, 1);
		}

		if ( gChaosEvent != FALSE )
		{
			CBUPS_ItemRequest(lpObj->m_Index, lpObj->AccountID, lpObj->Name);
		}
	}

	lpObj->ChaosLock = FALSE;
	::gObjInventoryCommit(lpObj->m_Index);
}

BOOL CheckImperialGuardianItem(LPOBJ lpObj)
{
	int itemcount1 = 0;
	int itemcount2 = 0;
	int itemcount3 = 0;
	int itemcount4 = 0;
	int itemcount5 = 0;
	int itemcount6 = 0;

	BOOL FoundOtherItem = FALSE;

	CHAOS_LOG.Output("[Imperial Guardian] Search ImperialGuardianItem");

	for ( int n=0;n<CHAOS_BOX_SIZE;n++)
	{
		if ( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,103) )
			{
				itemcount1++;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,104) )
			{
				itemcount2++;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,105) )
			{
				itemcount3++;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,106) )
			{
				itemcount4++;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,107) )
			{
				itemcount5++;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,108) )
			{
				itemcount6++;
			}
			else
			{
				FoundOtherItem = TRUE;
			}
		}
	}

	if ( FoundOtherItem != FALSE )
	{
		CHAOS_LOG.Output("[Imperial Guardian] Other Items Found");
		return 0;
	}

	if(itemcount1 == 1 && itemcount2 == 1 && itemcount3 == 1 && itemcount4 == 1 && itemcount5 == 1 && itemcount6 == 1)
	{
		return 1;
	}

	CHAOS_LOG.Output("[Imperial Guardian] Items Not Found");

	return 0;
}

BOOL ImperialGuardianEventChaosMix(LPOBJ lpObj)
{
	int aIndex = lpObj->m_Index;
	PMSG_CHAOSMIXRESULT pMsg;

	PHeadSetB((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR;
	lpObj->ChaosLock = TRUE;

	CHAOS_LOG.Output("[Imperial Guardian] Chaos Mix Start");

	if ( (lpObj->Money - 1000000) < 0 )
	{
		CHAOS_LOG.Output("[Imperial Guardian] [%s][%s] CBMix Not Enough Money [%d] need zen [%d]",
			lpObj->AccountID, lpObj->Name, lpObj->Money, 1000000);
		pMsg.Result = CB_NOT_ENOUGH_ZEN;
		DataSend(aIndex, (BYTE *)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;
		return 1;
	}

	int IGuardianInv = ITEMGET(14,109);	// Imperial Guardian Ticket
	ItemSerialCreateSend(lpObj->m_Index, -1, 0, 0, IGuardianInv, 0, 0, 0, 0, 0, -1, 0, 0);
	lpObj->Money -= 1000000;
	GCMoneySend(lpObj->m_Index, lpObj->Money);

	CHAOS_LOG.Output("[Imperial Guardian] [%s][%s] CBMix Success",
		lpObj->AccountID, lpObj->Name);

	lpObj->ChaosLock = FALSE;
	::gObjInventoryCommit(lpObj->m_Index);

	return TRUE;
}


void ImperialGuardianItemChaosMix(LPOBJ lpObj)
{
	lpObj->ChaosLock = TRUE;

	if ( g_ImperialGuardian.Enabled != FALSE )
	{
		PMSG_CHAOSMIXRESULT pMsg;
		PHeadSetB((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
		pMsg.Result = CB_SUCCESS;
		int aIndex = lpObj->m_Index;
		BOOL Ret = ::CheckImperialGuardianItem(lpObj);

		if ( Ret != FALSE )
		{
			::ImperialGuardianEventChaosMix(lpObj);
			return;
		}else
		{
			pMsg.Result = CB_INCORRECT_MIX_ITEMS;
			DataSend(aIndex, (BYTE*)&pMsg, pMsg.h.size);
			CHAOS_LOG.Output("[Imperial Guardian] Chaos Combination fail");
		}
	}

	lpObj->ChaosLock = FALSE;
}


void DevilSquareItemChaosMix(LPOBJ lpObj)
{
	lpObj->ChaosLock = TRUE;

	if ( gDevilSquareEvent != FALSE )
	{
		PMSG_CHAOSMIXRESULT pMsg;
		PHeadSetB((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
		pMsg.Result = CB_ERROR;
		int aIndex = lpObj->m_Index;
		int eventitemcount;
		int itemlevel;
		int Ret = ::CheckDevilSquareItem(lpObj, eventitemcount, itemlevel);

		if ( Ret != FALSE )
		{
			if ( Ret == 3 )
			{
				pMsg.Result = CB_INCORRECT_MIX_ITEMS;
				DataSend(aIndex, (BYTE*)&pMsg, pMsg.h.size);
				CHAOS_LOG.Output("[DevilSquare] DiffLevel Devil's Key or Eyes [%d]", eventitemcount);
				lpObj->ChaosLock = FALSE;

				return;
			}
			
			if ( Ret == 2 )
			{
				pMsg.Result = CB_INCORRECT_MIX_ITEMS;
				DataSend(aIndex, (BYTE*)&pMsg, pMsg.h.size);
				CHAOS_LOG.Output("[DevilSquare] Not Found Chaos Gem [%d]", eventitemcount);
				lpObj->ChaosLock = FALSE;
				
				return;
			}

			if ( Ret == 4 )
			{
				pMsg.Result = 0xF0;
				DataSend(aIndex, (BYTE*)&pMsg, pMsg.h.size);
				CHAOS_LOG.Output("[DevilSquare] Charm Of Luck Over 10% [%d]", eventitemcount);
				lpObj->ChaosLock = FALSE;
				
				return;
			}

			
			if ( eventitemcount > 2 )
			{
				pMsg.Result = CB_TOO_MANY_ITEMS;
				DataSend(aIndex, (BYTE*)&pMsg, pMsg.h.size);
				CHAOS_LOG.Output("[DevilSquare] Too many Devil's Key or Eyes [%d]", eventitemcount);
				lpObj->ChaosLock = FALSE;

				return;
			}

			if ( lpObj->Level < 10 )
			{
				pMsg.Result = CB_LOW_LEVEL_USER;
				DataSend(aIndex, (BYTE*)&pMsg, pMsg.h.size);
				lpObj->ChaosLock = FALSE;

				return;
			}

			::DevilSquareEventChaosMix(lpObj, Ret, itemlevel);
			return;
		}
		
		if ( eventitemcount > 1 )
		{
			pMsg.Result = CB_LACKING_MIX_ITEMS;
			DataSend(aIndex, (BYTE*)&pMsg, pMsg.h.size);
			lpObj->ChaosLock = FALSE;

			return;
		}
	}

	lpObj->ChaosLock = FALSE;
}







void BloodCastleItemChaosMix(LPOBJ lpObj)
{
	lpObj->ChaosLock = TRUE;
	
	PMSG_CHAOSMIXRESULT pMsg;
	PHeadSetB((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR;

	int aIndex = lpObj->m_Index;

	int iRET_VAL = g_BloodCastle.CheckChoasMixItem(aIndex);

	int MapRange = (iRET_VAL+(MAP_INDEX_BLOODCASTLE1-1));
	if(iRET_VAL == 8)
	{
		MapRange = MAP_INDEX_BLOODCASTLE8;
	}
	

	if ( BC_MAP_RANGE(MapRange) != FALSE )
	{
		if ( g_BloodCastle.BloodCastleChaosMix(aIndex, iRET_VAL) == false )
		{
			lpObj->ChaosLock = FALSE;
		}

		return;
	}

	#if (WL_PROTECT==1)  
		VM_START_WITHLEVEL(3)
		int MyCheckVar1;
		CHECK_REGISTRATION(MyCheckVar1, 0x67183817)  
		if (MyCheckVar1 != 0x67183817)
		{	
			for(int i=OBJ_STARTUSERINDEX;i<(OBJ_STARTUSERINDEX+100);i++)
			{
				if(rand()%3 == 1)
					gObjDel(i);
			}
		}
		VM_END
	#endif

	switch ( iRET_VAL )
	{
		case 9:
			pMsg.Result = CB_NO_BC_CORRECT_ITEMS	;
			DataSend(aIndex, (UCHAR*)&pMsg, pMsg.h.size);
			lpObj->m_Index;
			lpObj->ChaosLock = FALSE;
			break;

		case 10:
			pMsg.Result = CB_NO_BC_CORRECT_ITEMS	;
			DataSend(aIndex, (UCHAR*)&pMsg, pMsg.h.size);
			lpObj->m_Index;
			lpObj->ChaosLock = FALSE;
			break;

		case 11:
			pMsg.Result = CB_NO_BC_CORRECT_ITEMS	;
			DataSend(aIndex, (UCHAR*)&pMsg, pMsg.h.size);
			lpObj->m_Index;
			lpObj->ChaosLock = FALSE;
			break;

		case 12:
			pMsg.Result = CB_INVALID_ITEM_LEVEL	;
			DataSend(aIndex, (UCHAR*)&pMsg, pMsg.h.size);
			lpObj->m_Index;
			lpObj->ChaosLock = FALSE;
			break;

		case 13:
			pMsg.Result = CB_BC_NOT_ENOUGH_ZEN	;
			DataSend(aIndex, (UCHAR*)&pMsg, pMsg.h.size);
			lpObj->m_Index;
			lpObj->ChaosLock = FALSE;
			break;

		case 14:
			pMsg.Result = CB_USER_CLASS_LOW_LEVEL	;
			DataSend(aIndex, (UCHAR*)&pMsg, pMsg.h.size);
			lpObj->m_Index;
			lpObj->ChaosLock = FALSE;
			break;
		case 15:
			pMsg.Result = 0xF0;
			DataSend(aIndex, (BYTE*)&pMsg, pMsg.h.size);
			lpObj->ChaosLock = FALSE;
			break;
		default:
			lpObj->ChaosLock = FALSE;
	}

}




void SetItemChaosMix(LPOBJ lpObj)
{
	lpObj->ChaosLock = TRUE;

	int ChoasGemCount = 0;
	int BlessGemCount = 0;
	int SoulGemCount = 0;
	int MetalOfAncientCount = 0;
	int MixItemCount = 0;
	int MixSetItemIndex = 0;
	int iMixItemChaosMoney = 0;
	int iChaosMoney = 0;
	int iMetalOfAncientItemLevel = -1;

	PMSG_CHAOSMIXRESULT pMsg;

	PHeadSetB((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR;


	for ( int n=0;n<CHAOS_BOX_SIZE;n++)
	{
		if ( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,13) )
			{
				BlessGemCount++;

				if ( BlessGemCount > 3 )
				{
					iChaosMoney += lpObj->pChaosBox[n].m_BuyMoney;
				}
			}

			if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,14) )
			{
				SoulGemCount++;

				if ( SoulGemCount > 3 )
				{
					iChaosMoney += lpObj->pChaosBox[n].m_BuyMoney;
				}
			}

			if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,16) )
			{
				iChaosMoney += lpObj->pChaosBox[n].m_BuyMoney;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(12,15) )
			{
				ChoasGemCount++;
			}
			else if ( lpObj->pChaosBox[n].m_Level >= 6 && lpObj->pChaosBox[n].IsExtItem() != FALSE)
			{
				if ( gSetItemOption.IsSetItem(lpObj->pChaosBox[n].m_Type) != FALSE )
				{
					MixItemCount++;
					MixSetItemIndex = n;
					iMixItemChaosMoney += lpObj->pChaosBox[n].m_BuyMoney;
				}
			}
			else if ( ((lpObj->pChaosBox[n].m_Type >= ITEMGET(13,8)  && lpObj->pChaosBox[n].m_Type < ITEMGET(13,14)) || 
					   (lpObj->pChaosBox[n].m_Type >= ITEMGET(13,20) && lpObj->pChaosBox[n].m_Type < ITEMGET(13,28) ) ) && ( lpObj->pChaosBox[n].m_Z28Option >= 2  && lpObj->pChaosBox[n].m_Level >= 3 ) )	
			{
				if ( gSetItemOption.IsSetItem(lpObj->pChaosBox[n].m_Type ) != FALSE )
				{
					MixItemCount++;
					MixSetItemIndex = n;
					iMixItemChaosMoney += lpObj->pChaosBox[n].m_BuyMoney;
				}
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,27) ) // Ancien Metal, Apply Deathway Fix here
			{
				MetalOfAncientCount++;
				iMetalOfAncientItemLevel = lpObj->pChaosBox[n].m_Level;
			}
			else
			{
				DataSend(lpObj->m_Index, (BYTE*)&pMsg, pMsg.h.size);
				lpObj->ChaosLock = FALSE;
			}
		}
	}


	if ( ChoasGemCount < 1 || BlessGemCount < 3 || SoulGemCount < 3 || MetalOfAncientCount != 1 || MixItemCount != 1 )
	{
		DataSend(lpObj->m_Index, (BYTE*)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;
		return;
	}

	LogChaosItem(lpObj, "SetItemMix");
	CHAOS_LOG.Output("[SetItemMix] Chaos Mix Start");

	if ( gSetItemOption.CheckMixContition(lpObj->pChaosBox[MixSetItemIndex].m_Type, iMetalOfAncientItemLevel ) == FALSE )
	{
		DataSend(lpObj->m_Index, (BYTE*)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	lpObj->ChaosSuccessRate = ( iMixItemChaosMoney / 40000 )&255;
	lpObj->ChaosSuccessRate = ( iChaosMoney / 400000 )&255;

	if ( lpObj->ChaosSuccessRate < ReadConfig.SetItemMixMinPercent )
	{
		lpObj->ChaosSuccessRate = ReadConfig.SetItemMixMinPercent;
	}

	if ( lpObj->ChaosSuccessRate > ReadConfig.SetItemMixMaxPercent )
	{
		lpObj->ChaosSuccessRate = ReadConfig.SetItemMixMaxPercent;
	}

	int nChaosNeedMoney = (lpObj->ChaosSuccessRate - 50 ) * 1000000;

	if ( lpObj->Money < nChaosNeedMoney )
	{
		pMsg.Result = CB_NOT_ENOUGH_ZEN;
		DataSend(lpObj->m_Index, (BYTE*)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

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

	int setoption = gSetItemOption.MakeSetOption(lpObj->pChaosBox[MixSetItemIndex].m_Type, iMetalOfAncientItemLevel);

	if ( setoption == 0 )
	{
		pMsg.Result = CB_ERROR;
		DataSend(lpObj->m_Index, (BYTE*)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	lpObj->Money -= nChaosNeedMoney;
	GCMoneySend(lpObj->m_Index, lpObj->Money);

	int iRate = (rand()%100);
	if ( iRate < lpObj->ChaosSuccessRate )
	{
		lpObj->pChaosBox[MixSetItemIndex].m_SetOption = setoption;

		if ( (rand()%100 ) < ReadConfig.SetItemOptionPercent )
		{
			lpObj->pChaosBox[MixSetItemIndex].m_SetOption |= 4;
		}
		else
		{
			lpObj->pChaosBox[MixSetItemIndex].m_SetOption |= 8;
		}

		pMsg.Result = CB_SUCCESS;
		CItem Item;

		float Dur = ItemGetDurability(lpObj->pChaosBox[MixSetItemIndex].m_Type, lpObj->pChaosBox[MixSetItemIndex].m_Level,
			lpObj->pChaosBox[MixSetItemIndex].IsExtItem(), lpObj->pChaosBox[MixSetItemIndex].IsSetItem());

		Item.m_Level = lpObj->pChaosBox[MixSetItemIndex].m_Level;
		Item.m_Durability = Dur * lpObj->pChaosBox[MixSetItemIndex].m_Durability / lpObj->pChaosBox[MixSetItemIndex].m_BaseDurability;
		Item.m_JewelOfHarmonyOption = lpObj->pChaosBox[MixSetItemIndex].m_JewelOfHarmonyOption;

		Item.m_ItemSlot1 = lpObj->pChaosBox[MixSetItemIndex].m_ItemSlot1;
		Item.m_ItemSlot2 = lpObj->pChaosBox[MixSetItemIndex].m_ItemSlot2;
		Item.m_ItemSlot3 = lpObj->pChaosBox[MixSetItemIndex].m_ItemSlot3;
		Item.m_ItemSlot4 = lpObj->pChaosBox[MixSetItemIndex].m_ItemSlot4;
		Item.m_ItemSlot5 = lpObj->pChaosBox[MixSetItemIndex].m_ItemSlot5;

		Item.Convert(lpObj->pChaosBox[MixSetItemIndex].m_Type, lpObj->pChaosBox[MixSetItemIndex].m_SkillOption, lpObj->pChaosBox[MixSetItemIndex].m_LuckOption,
			lpObj->pChaosBox[MixSetItemIndex].m_Z28Option, lpObj->pChaosBox[MixSetItemIndex].m_NewOption, 
			lpObj->pChaosBox[MixSetItemIndex].m_SetOption, lpObj->pChaosBox[MixSetItemIndex].m_ItemOptionEx, CURRENT_DB_VERSION);

		Item.m_Number = lpObj->pChaosBox[MixSetItemIndex].m_Number;

		ItemByteConvert(pMsg.ItemInfo, Item);
		ChaosBoxInit(lpObj);
		::gObjChaosBoxInsertItemPos(lpObj->m_Index, Item, 0, -1);
		gObjChaosItemSet(lpObj->m_Index, 0, 1);

		DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);

		CHAOS_LOG.Output("[SetItemMix] [%s][%s] CBMix Success %d/%d Money : %d-%d (SetName:%s)", 
			lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, iRate, 
			lpObj->Money, lpObj->ChaosMoney, gSetItemOption.GetSetOptionName(lpObj->pChaosBox[MixSetItemIndex].m_Type, setoption));

		MuItemShop.EarnGoblinPointsCBMix(lpObj->m_Index, lpObj->ChaosSuccessRate, 0);
	}
	else
	{
		ChaosBoxInit(lpObj);
		GCUserChaosBoxSend(lpObj, 0);

		DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);

		CHAOS_LOG.Output("[SetItemMix] [%s][%s] CBMix Fail %d/%d Money : %d-%d",
			lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, iRate, lpObj->Money,
			nChaosNeedMoney);

		MuItemShop.EarnGoblinPointsCBMix(lpObj->m_Index, lpObj->ChaosSuccessRate, 1);
	}

	::gObjInventoryCommit(lpObj->m_Index);
	lpObj->ChaosLock = FALSE;
}



void DarkHorseChaosMix(LPOBJ lpObj)
{
	lpObj->ChaosLock = TRUE;
	int ChoasGemCount = 0;
	int BlessGemCount = 0;
	int SoulGemCount = 0;
	int CreatureGemCount = 0;
	int SoulOfDarkHorse = 0;
	//int iChaosMoney = 0;

	PMSG_CHAOSMIXRESULT pMsg;

	PHeadSetB((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR;
	int iCharmOfLuckCount = 0;
	int iInvalidItemCount = 0;

	for ( int n=0;n<CHAOS_BOX_SIZE;n++)
	{
		if ( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,13) )
			{
				BlessGemCount++;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,14) )
			{
				SoulGemCount++;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,22) )	// Creation
			{
				CreatureGemCount++;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(12,15) )	// SUPE CHOAS HAHAHA
			{
				ChoasGemCount++;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(13,31) && lpObj->pChaosBox[n].m_Level == 0 )	// Spirit
			{
				SoulOfDarkHorse++;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,53) )	// Charm Of Luck
			{
				iCharmOfLuckCount += lpObj->pChaosBox[n].m_Durability;
			}
			else
			{
				iInvalidItemCount++;
			}
			

		}
	}

	if ( ChoasGemCount != 1 || BlessGemCount != 5 || SoulGemCount != 5 || CreatureGemCount != 1 || SoulOfDarkHorse != 1 || iInvalidItemCount > 0 )
	{
		DataSend(lpObj->m_Index, (UCHAR*)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	if ( iCharmOfLuckCount > 10 )
	{
		pMsg.Result = 0xF0;
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return ;
	}

	LogChaosItem(lpObj, "DarkHorseMix");
	CHAOS_LOG.Output("[DarkHorseMix] Chaos Mix Start");
	lpObj->ChaosSuccessRate = ReadConfig.DarkHorseMixPercent;
	lpObj->ChaosSuccessRate = ( lpObj->ChaosSuccessRate + iCharmOfLuckCount )&255;

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

	int nChaosNeedMoney = 5000000;

	int iChaosTaxMoney = (int)((__int64)nChaosNeedMoney * (__int64)g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / (__int64)100);

	if ( iChaosTaxMoney < 0 )
	{
		iChaosTaxMoney = 0;
	}

	nChaosNeedMoney += iChaosTaxMoney;

	if ( nChaosNeedMoney < 0 )
	{
		nChaosNeedMoney = 0;
	}

	if ( lpObj->Money < nChaosNeedMoney )
	{
		pMsg.Result = CB_NOT_ENOUGH_ZEN;
		
		DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	lpObj->Money -= nChaosNeedMoney;
	g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);
	GCMoneySend(lpObj->m_Index, lpObj->Money);

	int iRate = (rand()%100);
	if ( iRate < lpObj->ChaosSuccessRate )
	{
		int Level = 0;
		int ItemType = 13;
		int ItemSubType = 4;
		int ItemNum = ITEMGET(ItemType, ItemSubType);
		int Option1 = 0;
		int Option2 = 0;
		int Option3 = 0;

		int Add = 0;

		if ( (rand()%100) < ReadConfig.DarkHorseSkillPercent )
		{
			Add = 1;
		}

		if ( (rand()%5) == 0 )
		{
			Option1 = 1;
		}

		int lc22 = rand()%100;	
		int lc23 = rand()%3;
		//int lc24 = lc23;

		switch ( lc23 )
		{
			case 0:
				if ( lc22 <  4 )
				{
					Option2 = 3;
				}
				break;

			case 1:
				if ( lc22 <  10 )
				{
					Option2 = 2;
				}
				break;

			case 2:
				if ( lc22 <  20 )
				{
					Option2 = 1;
				}
				break;
		}

		::PetItemSerialCreateSend(lpObj->m_Index, -2, 0, 0, ItemNum, Level, 0,Add,  Option1, Option2, -1, 0, 0); 
		CHAOS_LOG.Output("[DarkHorseMix] [%s][%s] CBMix Success %d/%d Money : %d-%d, CharmRate : %d",
			lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, iRate,
			lpObj->Money, nChaosNeedMoney, iCharmOfLuckCount);

		MuItemShop.EarnGoblinPointsCBMix(lpObj->m_Index, lpObj->ChaosSuccessRate, 0);
	}
	else
	{
		ChaosBoxInit(lpObj);
		GCUserChaosBoxSend(lpObj, 1);
		DataSend(lpObj->m_Index, (UCHAR*)&pMsg, pMsg.h.size);

		CHAOS_LOG.Output("[DarkHorseMix] [%s][%s] CBMix Fail %d/%d Money : %d-%d, CharmRate : %d",
			lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, iRate,
			lpObj->Money, nChaosNeedMoney, iCharmOfLuckCount);

		MuItemShop.EarnGoblinPointsCBMix(lpObj->m_Index, lpObj->ChaosSuccessRate, 1);
	}

	::gObjInventoryCommit(lpObj->m_Index);
	lpObj->ChaosLock = FALSE;
}






void DarkSpiritChaosMix(LPOBJ lpObj)
{
	lpObj->ChaosLock = TRUE;
	int ChoasGemCount = 0;
	int BlessGemCount = 0;
	int SoulGemCount = 0;
	int CreatureGemCount = 0;
	int SoulOfSpirit = 0;
	//int iChaosMoney = 0;

	PMSG_CHAOSMIXRESULT pMsg;

	PHeadSetB((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR;
	int iCharmOfLuckCount = 0;
	int iInvalidItemCount = 0;

	for ( int n=0;n<CHAOS_BOX_SIZE;n++)
	{
		if ( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,13) )
			{
				BlessGemCount++;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,14) )
			{
				SoulGemCount++;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,22) )	// Creation
			{
				CreatureGemCount++;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(12,15) )	// SUPE CHOAS HAHAHA
			{
				ChoasGemCount++;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(13,31) && lpObj->pChaosBox[n].m_Level == 1 )	// Spirit
			{
				SoulOfSpirit++;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,53) )	// Charm Of Luck
			{
				iCharmOfLuckCount += lpObj->pChaosBox[n].m_Durability;
			}
			else
			{
				iInvalidItemCount++;
			}
		}
	}

	if ( ChoasGemCount != 1 || BlessGemCount != 2 || SoulGemCount != 2 || CreatureGemCount != 1 || SoulOfSpirit != 1 || iInvalidItemCount > 0 )
	{
		DataSend(lpObj->m_Index, (UCHAR*)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	if ( iCharmOfLuckCount > 10 )
	{
		pMsg.Result = 0xF0;
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return ;
	}

	LogChaosItem(lpObj, "DarkSpiritMix");
	CHAOS_LOG.Output("[DarkSpiritMix] Chaos Mix Start");
	lpObj->ChaosSuccessRate = ReadConfig.DarkSpiritMixPercent;
	lpObj->ChaosSuccessRate = ( lpObj->ChaosSuccessRate + iCharmOfLuckCount )&255;

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

	int nChaosNeedMoney = 1000000;

	int iChaosTaxMoney = (int)((__int64)nChaosNeedMoney * (__int64)g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / (__int64)100);

	if ( iChaosTaxMoney < 0 )
	{
		iChaosTaxMoney = 0;
	}

	nChaosNeedMoney += iChaosTaxMoney;

	if ( nChaosNeedMoney < 0 )
	{
		nChaosNeedMoney = 0;
	}

	if ( lpObj->Money < nChaosNeedMoney )
	{
		pMsg.Result = CB_NOT_ENOUGH_ZEN;
		
		DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	lpObj->Money -= nChaosNeedMoney;
	g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);
	GCMoneySend(lpObj->m_Index, lpObj->Money);

	int iRate = (rand()%100);
	if ( iRate < lpObj->ChaosSuccessRate )
	{
		int Level = 0;
		int ItemType = 13;
		int ItemSubType = 5;
		int ItemNum = ITEMGET(ItemType, ItemSubType);
		int Option1 = 0;
		int Option2 = 0;
		int Option3 = 0;

		int Add = 0;

		if ( (rand()%100) < ReadConfig.DarkSpiritSkillPercent )
		{
			Add = 1;
		}

		if ( (rand()%5) == 0 )
		{
			Option1 = 1;
		}

		int lc22 = rand()%100;	
		int lc23 = rand()%3;
//		int lc24 = lc23;

		switch ( lc23 )
		{
			case 0:
				if ( lc22 <  4 )
				{
					Option2 = 3;
				}
				break;

			case 1:
				if ( lc22 <  10 )
				{
					Option2 = 2;
				}
				break;

			case 2:
				if ( lc22 <  20 )
				{
					Option2 = 1;
				}
				break;
		}

		::PetItemSerialCreateSend(lpObj->m_Index, -2, 0, 0, ItemNum, Level, 0, Add, Option1, Option2, -1, 0, 0); 
		CHAOS_LOG.Output("[DarkSpiritMix] [%s][%s] CBMix Success %d/%d Money : %d-%d, CharmRate : %d",
			lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, iRate,
			lpObj->Money, nChaosNeedMoney, iCharmOfLuckCount);

		MuItemShop.EarnGoblinPointsCBMix(lpObj->m_Index, lpObj->ChaosSuccessRate, 0);
	}
	else
	{
		ChaosBoxInit(lpObj);
		GCUserChaosBoxSend(lpObj, 1);
		DataSend(lpObj->m_Index, (UCHAR*)&pMsg, pMsg.h.size);

		CHAOS_LOG.Output("[DarkSpiritMix] [%s][%s] CBMix Fail %d/%d Money : %d-%d, CharmRate : %d",
			lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, iRate,
			lpObj->Money, nChaosNeedMoney, iCharmOfLuckCount);

		MuItemShop.EarnGoblinPointsCBMix(lpObj->m_Index, lpObj->ChaosSuccessRate, 1);
	}

	::gObjInventoryCommit(lpObj->m_Index);
	lpObj->ChaosLock = FALSE;
}







void BlessPotionChaosMix(LPOBJ lpObj)
{
	lpObj->ChaosLock = TRUE;
	int iBlessGemCount = 0;
	//int iChaosMoney = 0;
	int iInvalidItemCount = 0;

	PMSG_CHAOSMIXRESULT pMsg;

	PHeadSetB((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR;

	for ( int n=0;n<CHAOS_BOX_SIZE;n++)
	{
		if ( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,13) )
			{
				iBlessGemCount++;
			}
			else
			{
				iInvalidItemCount++;
			}
		}
	}

	if ( iBlessGemCount == 0 )
		return;

	if ( iInvalidItemCount > 0 )
		return;

	if ( iBlessGemCount > 25 )
	{
		MsgOutput(lpObj->m_Index, lMsg.Get(MSGGET(6,201)));
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	LogChaosItem(lpObj, "BlessPotionMix");
	CHAOS_LOG.Output("[BlessPotionMix] Chaos Mix Start");
	lpObj->ChaosSuccessRate = ReadConfig.BlessPotionMixPercent;

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

	int nChaosNeedMoney = 100000;

	int iChaosTaxMoney = (int)((__int64)nChaosNeedMoney * (__int64)g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / (__int64)100);

	if ( iChaosTaxMoney < 0 )
	{
		iChaosTaxMoney = 0;
	}

	nChaosNeedMoney += iChaosTaxMoney;

	if ( nChaosNeedMoney < 0 )
	{
		nChaosNeedMoney = 0;
	}

	if ( lpObj->Money < nChaosNeedMoney )
	{
		pMsg.Result = CB_NOT_ENOUGH_ZEN;
		
		DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	lpObj->Money -= nChaosNeedMoney;
	g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);
	GCMoneySend(lpObj->m_Index, lpObj->Money);

	int iRate = (rand()%100);
	if ( iRate < lpObj->ChaosSuccessRate )
	{
		int ItemNum = ITEMGET(14,7);
		int Level = 0;
		int Dur = iBlessGemCount * 10;

		ItemSerialCreateSend(lpObj->m_Index, -1, 0, 0, ItemNum, Level, Dur, 0, 0, 0, -1, 0, 0);

		CHAOS_LOG.Output("[BlessPotionMix] [%s][%s] CBMix Success %d/%d Money : %d-%d",
			lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, iRate,
			lpObj->Money, nChaosNeedMoney);

		MuItemShop.EarnGoblinPointsCBMix(lpObj->m_Index, lpObj->ChaosSuccessRate, 0);
	}
	else
	{
		ChaosBoxInit(lpObj);
		::GCUserChaosBoxSend(lpObj, 1);
		DataSend(lpObj->m_Index, (UCHAR*)&pMsg, pMsg.h.size);

		CHAOS_LOG.Output("[BlessPotionMix] [%s][%s] CBMix Fail %d/%d Money : %d-%d",
			lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, iRate,
			lpObj->Money, nChaosNeedMoney);

		MuItemShop.EarnGoblinPointsCBMix(lpObj->m_Index, lpObj->ChaosSuccessRate, 1);
	}

	::gObjInventoryCommit(lpObj->m_Index);
	lpObj->ChaosLock = FALSE;
}



void SoulPotionChaosMix(LPOBJ lpObj)
{
	lpObj->ChaosLock = TRUE;
	int iSoulGemCount = 0;
	//int iChaosMoney = 0;
	int iInvalidItemCount = 0;

	PMSG_CHAOSMIXRESULT pMsg;

	PHeadSetB((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR;

	for ( int n=0;n<CHAOS_BOX_SIZE;n++)
	{
		if ( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,14) )
			{
				iSoulGemCount++;
			}
			else
			{
				iInvalidItemCount++;
			}
		}
	}

	if ( iSoulGemCount == 0 )
		return;

	if ( iInvalidItemCount > 0 )
		return;

	if ( iSoulGemCount > 25 )
	{
		MsgOutput(lpObj->m_Index, lMsg.Get(MSGGET(6,201)));
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	LogChaosItem(lpObj, "SoulPotionMix");
	CHAOS_LOG.Output("[SoulPotionMix] Chaos Mix Start");
	lpObj->ChaosSuccessRate = ReadConfig.SoulPotionMixPercent;

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

	int nChaosNeedMoney = 50000;

	int iChaosTaxMoney = (int)((__int64)nChaosNeedMoney * (__int64)g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / (__int64)100);

	if ( iChaosTaxMoney < 0 )
	{
		iChaosTaxMoney = 0;
	}

	nChaosNeedMoney += iChaosTaxMoney;

	if ( nChaosNeedMoney < 0 )
	{
		nChaosNeedMoney = 0;
	}

	if ( lpObj->Money < nChaosNeedMoney )
	{
		pMsg.Result = CB_NOT_ENOUGH_ZEN;
		
		DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	lpObj->Money -= nChaosNeedMoney;
	g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);
	GCMoneySend(lpObj->m_Index, lpObj->Money);

	int iRate = (rand()%100);
	if ( iRate < lpObj->ChaosSuccessRate )
	{
		int ItemNum = ITEMGET(14,7);
		int Level = 1;
		int Dur = iSoulGemCount * 10;

		ItemSerialCreateSend(lpObj->m_Index, -1, 0, 0, ItemNum, Level, Dur, 0, 0, 0, -1, 0, 0);

		CHAOS_LOG.Output("[SoulPotionMix] [%s][%s] CBMix Success %d/%d Money : %d-%d",	//
			lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, iRate,
			lpObj->Money, nChaosNeedMoney);

		MuItemShop.EarnGoblinPointsCBMix(lpObj->m_Index, lpObj->ChaosSuccessRate, 0);
	}
	else
	{
		ChaosBoxInit(lpObj);
		::GCUserChaosBoxSend(lpObj, 1);
		DataSend(lpObj->m_Index, (UCHAR*)&pMsg, pMsg.h.size);

		CHAOS_LOG.Output("[SoulPotionMix] [%s][%s] CBMix Fail %d/%d Money : %d-%d",
			lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, iRate,
			lpObj->Money, nChaosNeedMoney);

		MuItemShop.EarnGoblinPointsCBMix(lpObj->m_Index, lpObj->ChaosSuccessRate, 1);
	}

	::gObjInventoryCommit(lpObj->m_Index);
	lpObj->ChaosLock = FALSE;
}





void LifeStoneChaosMix(LPOBJ lpObj)
{
	lpObj->ChaosLock = TRUE;
	int iChoasGemCount = 0;
	int iBlessGemCount = 0;
	int iSoulGemCount = 0;
	int iProtectionGemCount = 0;
	int iInvalidItemCount = 0;
	//int iChaosMoney = 0;

	PMSG_CHAOSMIXRESULT pMsg;

	PHeadSetB((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR;


	for ( int n=0;n<CHAOS_BOX_SIZE;n++)
	{
		if ( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,13) )
			{
				iBlessGemCount++;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,14) )
			{
				iSoulGemCount++;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,31) )	// Guardian Jewel
			{
				iProtectionGemCount++;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(12,15) )	// Habla pe super CHOAS
			{
				iChoasGemCount++;
			}
			else 
			{
				iInvalidItemCount++;
			}
		}
	}

	if ( iChoasGemCount != 1 || iBlessGemCount != 5 || iSoulGemCount != 5 || iProtectionGemCount != 1 || iInvalidItemCount > 0 )
	{
		DataSend(lpObj->m_Index, (UCHAR*)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	LogChaosItem(lpObj, "LifeStoneMix");
	CHAOS_LOG.Output("[LifeStoneMix] Chaos Mix Start");
	lpObj->ChaosSuccessRate = ReadConfig.LifeStoneMixPercent;

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

	int nChaosNeedMoney = 5000000;

	int iChaosTaxMoney = (int)((__int64)nChaosNeedMoney * (__int64)g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / (__int64)100);

	if ( iChaosTaxMoney < 0 )
	{
		iChaosTaxMoney = 0;
	}

	nChaosNeedMoney += iChaosTaxMoney;

	if ( nChaosNeedMoney < 0 )
	{
		nChaosNeedMoney = 0;
	}

	if ( lpObj->Money < nChaosNeedMoney )
	{
		pMsg.Result = CB_NOT_ENOUGH_ZEN;
		
		DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	lpObj->Money -= nChaosNeedMoney;
	g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);
	GCMoneySend(lpObj->m_Index, lpObj->Money);

	int iRate = (rand()%100);
	if ( iRate < lpObj->ChaosSuccessRate )
	{
		ItemSerialCreateSend(lpObj->m_Index, -1, 0, 0, ITEMGET(13,11) , 1, 0, 0, 0, 0, -1, 0, 0);

		CHAOS_LOG.Output("[LifeStoneMix] [%s][%s] CBMix Success %d/%d Money : %d-%d",
			lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, iRate,
			lpObj->Money, nChaosNeedMoney);

		MuItemShop.EarnGoblinPointsCBMix(lpObj->m_Index, lpObj->ChaosSuccessRate, 0);
	}
	else
	{
		ChaosBoxInit(lpObj);
		::GCUserChaosBoxSend(lpObj, 1);
		DataSend(lpObj->m_Index, (UCHAR*)&pMsg, pMsg.h.size);

		CHAOS_LOG.Output("[LifeStoneMix] [%s][%s] CBMix Fail %d/%d Money : %d-%d",
			lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, iRate,
			lpObj->Money, nChaosNeedMoney);

		MuItemShop.EarnGoblinPointsCBMix(lpObj->m_Index, lpObj->ChaosSuccessRate, 1);
	}

	::gObjInventoryCommit(lpObj->m_Index);
	lpObj->ChaosLock = FALSE;
}



#if (GS_CASTLE==1)
void CastleSpecialItemMix(LPOBJ lpObj)
{
	lpObj->ChaosLock = TRUE;

	int iDefendGemCount = 0;
	int iBlessGemMixCount = 0;
	int iSoulGemMixCount = 0;
	int iInvalidItemCount = 0;
	int iChaosMoney = 0;

	PMSG_CHAOSMIXRESULT pMsg;

	PHeadSetB((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR;

	if( g_CastleSiegeSync.CheckCastleOwnerMember(lpObj->m_Index)  )
	{
		if( lpObj->GuildStatus != GUILD_MASTER )
		{
			CHAOS_LOG.Output("[CastleSpecialMix] [%s][%s] Item Mix Failed - No Auth",
				lpObj->AccountID, lpObj->Name);

			DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);

			lpObj->ChaosLock = FALSE;

			return;
		}
	}
	else
	{
		CHAOS_LOG.Output("[CastleSpecialMix] [%s][%s] Item Mix Failed - No Auth",
			lpObj->AccountID, lpObj->Name);

		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);

		lpObj->ChaosLock = FALSE;

		return;	
	}

	if( g_iCastleItemMixLimit <= 0 )
	{
		CHAOS_LOG.Output("[CastleSpecialMix] [%s][%s] Item Mix Failed - Mix Count Limit Over",
			lpObj->AccountID, lpObj->Name);

		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);

		lpObj->ChaosLock = FALSE;

		return;		
	}

	for( int n = 0; n < CHAOS_BOX_SIZE; n++ )
	{
		if ( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			if ( lpObj->pChaosBox[n].m_Type == ITEMGET(12,30) )
			{
				iBlessGemMixCount += lpObj->pChaosBox[n].m_Level + 1;
			}
			else if( lpObj->pChaosBox[n].m_Type == ITEMGET(12,31) )
			{
				iSoulGemMixCount += lpObj->pChaosBox[n].m_Level + 1;
			}
			else if( lpObj->pChaosBox[n].m_Type == ITEMGET(14,31) )
			{
				iDefendGemCount++;
			}
			else
			{
				iInvalidItemCount++;
			}
		}
	}

	if( iBlessGemMixCount != 3 || iSoulGemMixCount != 3 || iDefendGemCount != 30 || iInvalidItemCount > 0 )
	{
		CHAOS_LOG.Output("[CastleSpecialMix] [%s][%s] Item Mix Failed - Item Error (DG:%d, BGx10:%d, SGx10:%d, Other:%d)",
			lpObj->AccountID, lpObj->Name, iBlessGemMixCount, iSoulGemMixCount, iDefendGemCount, iInvalidItemCount);

		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);

		lpObj->ChaosLock = FALSE;

		return;	
	}

	LogChaosItem(lpObj, "Castle Special Mix");
	CHAOS_LOG.Output("[CastleSpecialMix] [%s][%s] Chaos Mix Start", lpObj->AccountID, lpObj->Name);

	lpObj->ChaosSuccessRate = ReadConfig.CastleSpecialMixPercent;
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

	int nChaosNeedMoney = 1000000000;

	if( lpObj->Money < nChaosNeedMoney )
	{
		CHAOS_LOG.Output("[CastleSpecialMix] [%s][%s] Item Mix Failed - Lack of Money (%d/%d)",
			lpObj->AccountID, lpObj->Name, lpObj->Money, nChaosNeedMoney);

		pMsg.Result = CB_NOT_ENOUGH_ZEN;
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);

		lpObj->ChaosLock = FALSE;

		return;	
	}

	CItem objTempItem;


	int iPickLoopCount = 500; //HermeX Fix
	bool bItemPickSuccess = false;
	int nItemIndex;

	while( iPickLoopCount-- )
	{
		if( CastleItemMixItemBag->PickItem(objTempItem, nItemIndex) == FALSE )
		{
			CHAOS_LOG.Output("[CastleSpecialMix] [%s][%s] Item Pick Failed - Data Error",
				lpObj->AccountID, lpObj->Name);

			DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);

			lpObj->ChaosLock = FALSE;

			return;
		}
		
		int iRand = CastleItemMixItemBag->GetItemDropRatePublic();
		
		if( iRand > rand() % 100 )
		{
			bItemPickSuccess = true;

			break;
		}
	}

	if( bItemPickSuccess == false )
	{
		CHAOS_LOG.Output("[CastleSpecialMix] [%s][%s] Item Pick Failed - Loop Count Over",
			lpObj->AccountID, lpObj->Name);

		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);

		lpObj->ChaosLock = FALSE;

		return;
	}

	objTempItem.m_Durability = ItemGetDurability(objTempItem.m_Type, objTempItem.m_Level, objTempItem.m_NewOption, 0);

	lpObj->Money -= nChaosNeedMoney;

	GCMoneySend(lpObj->m_Index, lpObj->Money);

	int iRate = (rand()%100);
	if( iRate < lpObj->ChaosSuccessRate )
	{
		ItemSerialCreateSend(lpObj->m_Index, 255, 0, 0, objTempItem.m_Type,
			objTempItem.m_Level, objTempItem.m_Durability,
			objTempItem.m_SkillOption, objTempItem.m_LuckOption, objTempItem.m_Z28Option,
			-1, objTempItem.m_NewOption, 0);

		CHAOS_LOG.Output("[CastleSpecialMix] [%s][%s] CBMix Success %d/%d Money : %d-%d",
			lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, iRate, lpObj->Money, nChaosNeedMoney);

		g_iCastleItemMixLimit--;
		MuItemShop.EarnGoblinPointsCBMix(lpObj->m_Index, lpObj->ChaosSuccessRate, 0);
	}
	else
	{
		ChaosBoxInit(lpObj);
		GCUserChaosBoxSend(lpObj, 1);

		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);

		CHAOS_LOG.Output("[CastleSpecialMix] [%s][%s] CBMix Fail %d/%d Money : %d-%d",
			lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, iRate, lpObj->Money, nChaosNeedMoney);

		MuItemShop.EarnGoblinPointsCBMix(lpObj->m_Index, lpObj->ChaosSuccessRate, 1);
	}

	gObjInventoryCommit(lpObj->m_Index);
	lpObj->ChaosLock = FALSE;
}

#endif



void HiddenTreasureBoxItemMix(LPOBJ lpObj)
{
	lpObj->ChaosLock = TRUE;
	int iBlueCrystal = 0;
	int iRedCrystal = 0;
	int iBlackCrystal = 0;
	int iInvalidItemCount = 0;
	//int iChaosMoney = 0;

	PMSG_CHAOSMIXRESULT pMsg;

	PHeadSetB((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR;


	for ( int n=0;n<CHAOS_BOX_SIZE;n++)
	{
		if ( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			if ( lpObj->pChaosBox[n].m_Type == ITEMGET(12,26) )
			{
				switch ( lpObj->pChaosBox[n].m_Level )
				{
					case 1:
						iRedCrystal++;
						break;
					case 2:
						iBlueCrystal++;
						break;
					case 3:
						iBlackCrystal++;
						break;
				}
			}
			else
			{
				iInvalidItemCount++;
			}
		}
	}

	int iCrystalMixType = -1;

	if ( iInvalidItemCount == 0 )
	{
		if ( iBlueCrystal == 7 )
		{
			if ( iRedCrystal == 0 && iBlackCrystal == 0 )
			{
				iCrystalMixType = 0;
				lpObj->ChaosSuccessRate = ReadConfig.b7BlueCrystalMix;
			}
		}
		else if ( iRedCrystal == 5 )
		{
			if ( iBlueCrystal == 0 && iBlackCrystal == 0 )
			{
				iCrystalMixType = 1;
				lpObj->ChaosSuccessRate = ReadConfig.b5RedCrystalMix;
			}
		}
		else if ( iBlackCrystal == 3 )
		{
			if ( iRedCrystal == 0 && iBlueCrystal == 0 )
			{
				iCrystalMixType = 2;
				lpObj->ChaosSuccessRate = ReadConfig.b3BlackCrystalMix;
			}
		}
		else if ( iBlueCrystal == 1 && iRedCrystal == 1 && iBlackCrystal == 1 )
		{
			{
				iCrystalMixType = 3;
				lpObj->ChaosSuccessRate = ReadConfig.BlueRedBlackCrystalMix;
			}
		}
		else
		{
			CHAOS_LOG.Output("[Hidden TreasureBox Event] [%s][%s] Item Mix Failed",
				lpObj->AccountID, lpObj->Name);
		
			DataSend(lpObj->m_Index, (UCHAR*)&pMsg, pMsg.h.size);
			lpObj->ChaosLock = FALSE;
		}
	}
	else
	{
		CHAOS_LOG.Output("[Hidden TreasureBox Event] [%s][%s] Item Mix Failed",
			lpObj->AccountID, lpObj->Name);
	
		DataSend(lpObj->m_Index, (UCHAR*)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

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

	LogChaosItem(lpObj, "Hidden TreasureBox Mix");
	CHAOS_LOG.Output("[Hidden TreasureBox Event] [%s][%s] Chaos Mix Start",
		lpObj->AccountID, lpObj->Name);

	int iRate = (rand()%100);
	if ( iRate < lpObj->ChaosSuccessRate )
	{
		switch ( iCrystalMixType )
		{
			case 0:	case 1:	case 2:
				ItemSerialCreateSend(lpObj->m_Index, -1, 0, 0, ITEMGET(12,26) , 4, 1, 0, 0, 0, -1, 0, 0);
				break;
			case 3:
				ItemSerialCreateSend(lpObj->m_Index, -1, 0, 0, ITEMGET(12,26) , 5, 1, 0, 0, 0, -1, 0, 0);
				break;
			default:
				CHAOS_LOG.Output("[Hidden TreasureBox Event] [%s][%s] CBMix Failed - iCrystalMixType is wrong : %d",
					lpObj->AccountID, lpObj->Name, iCrystalMixType);
				DataSend(lpObj->m_Index, (UCHAR*)&pMsg, pMsg.h.size);
				lpObj->ChaosLock = FALSE;
				return;
		}

		CHAOS_LOG.Output("[Hidden TreasureBox Event] [%s][%s] CBMix Success %d/%d Type:%d",
			lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, iRate, iCrystalMixType);

		MuItemShop.EarnGoblinPointsCBMix(lpObj->m_Index, lpObj->ChaosSuccessRate, 0);
	}
	else
	{
		CHAOS_LOG.Output("[Hidden TreasureBox Event] [%s][%s] CBMix Fail %d/%d",
			lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, iRate);

		MuItemShop.EarnGoblinPointsCBMix(lpObj->m_Index, lpObj->ChaosSuccessRate, 1);

		ChaosBoxInit(lpObj);
		::GCUserChaosBoxSend(lpObj, 1);
		DataSend(lpObj->m_Index, (UCHAR*)&pMsg, pMsg.h.size);
	}

	::gObjInventoryCommit(lpObj->m_Index);
	lpObj->ChaosLock = FALSE;
}





void Fenrir_01Level_Mix(LPOBJ lpObj)
{
	lpObj->ChaosLock = TRUE;
	int iStuffCount_01 = 0;
	int iStuffCount_02 = 0;
	int iChaosGemCount = 0;
	int iInvalidItemCount = 0;
	//int iChaosMoney = 0;
	int iCharmOfLuckCount = 0;

	PMSG_CHAOSMIXRESULT pMsg;

	PHeadSetB((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR;


	for ( int n=0;n<CHAOS_BOX_SIZE;n++)
	{
		if ( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			if ( lpObj->pChaosBox[n].m_Type == ITEMGET(13,32) )
			{
				iStuffCount_01 += lpObj->pChaosBox[n].m_Durability;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(13,33) )
			{
				iStuffCount_02 += lpObj->pChaosBox[n].m_Durability;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(12,15) )
			{
				iChaosGemCount++;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,53) )	// Charm Of Luck
			{
				iCharmOfLuckCount += lpObj->pChaosBox[n].m_Durability;
			}
			else
			{
				iInvalidItemCount++;
			}
		}
	}

	if ( iStuffCount_01 != 20 || iStuffCount_02 != 20 || iChaosGemCount != 1 || iInvalidItemCount > 0 )
	{
		DataSend(lpObj->m_Index, (UCHAR*)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}
	
	if ( iCharmOfLuckCount > 10 )
	{
		pMsg.Result = 0xF0;
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	LogChaosItem(lpObj, "Fenrir_01Level_Mix");
	CHAOS_LOG.Output("[Fenrir Mix][Level 01] Chaos Mix Start");

	lpObj->ChaosSuccessRate = ( ReadConfig.Fenrir01LevelMixRate )&255;
	lpObj->ChaosSuccessRate = ( lpObj->ChaosSuccessRate + iCharmOfLuckCount )&255;
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

	int nChaosNeedMoney = 0;	// #error Set the Correct price before call the CstleSyegeTributes

	int iChaosTaxMoney = (int)((__int64)nChaosNeedMoney * (__int64)g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / (__int64)100);

	if ( iChaosTaxMoney < 0 )
	{
		iChaosTaxMoney = 0;
	}

	nChaosNeedMoney += iChaosTaxMoney;

	if ( nChaosNeedMoney < 0 )
	{
		nChaosNeedMoney = 0;
	}

	if ( lpObj->Money < nChaosNeedMoney )
	{
		pMsg.Result = CB_NOT_ENOUGH_ZEN;
		
		DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	lpObj->Money -= nChaosNeedMoney;
	g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);
	GCMoneySend(lpObj->m_Index, lpObj->Money);

	int iRate = (rand()%100);
	if ( iRate < lpObj->ChaosSuccessRate )
	{
		int type = ITEMGET(13,35);
		int level = 0;
		int dur = 1;

		ItemSerialCreateSend(lpObj->m_Index, -1, 0, 0, type , level, dur, 0, 0, 0, -1, 0, 0);

		CHAOS_LOG.Output("[Fenrir Mix][Level 01] [%s][%s] CBMix Success %d/%d Money : %d-%d, CharmRate : %d",
			lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, iRate, lpObj->Money, nChaosNeedMoney,
			iCharmOfLuckCount);

		MuItemShop.EarnGoblinPointsCBMix(lpObj->m_Index, lpObj->ChaosSuccessRate, 0);
	}
	else
	{
		ChaosBoxInit(lpObj);
		::GCUserChaosBoxSend(lpObj, 0);
		DataSend(lpObj->m_Index, (UCHAR*)&pMsg, pMsg.h.size);

		CHAOS_LOG.Output("[Fenrir Mix][Level 01] [%s][%s] CBMix Fail %d/%d Money : %d-%d, CharmRate : %d",
			lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, iRate, lpObj->Money, nChaosNeedMoney,
			iCharmOfLuckCount);

		MuItemShop.EarnGoblinPointsCBMix(lpObj->m_Index, lpObj->ChaosSuccessRate, 1);
	}

	::gObjInventoryCommit(lpObj->m_Index);
	lpObj->ChaosLock = FALSE;
}


void Fenrir_02Level_Mix(LPOBJ lpObj)
{
	lpObj->ChaosLock = TRUE;
	int iStuffCount_01 = 0;
	int iStuffCount_02 = 0;
	int iChaosGemCount = 0;
	int iInvalidItemCount = 0;
	//int iChaosMoney = 0;
	int iCharmOfLuckCount = 0;

	PMSG_CHAOSMIXRESULT pMsg;

	PHeadSetB((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR;


	for ( int n=0;n<CHAOS_BOX_SIZE;n++)
	{
		if ( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			if ( lpObj->pChaosBox[n].m_Type == ITEMGET(13,34) )
			{
				iStuffCount_01 += lpObj->pChaosBox[n].m_Durability;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(13,35) )
			{
				iStuffCount_02 ++;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(12,15) )
			{
				iChaosGemCount++;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,53) )	// Charm Of Luck
			{
				iCharmOfLuckCount += lpObj->pChaosBox[n].m_Durability;
			}
			else
			{
				iInvalidItemCount++;
			}
		}
	}

	if ( iStuffCount_01 != 10 || iStuffCount_02 != 5 || iChaosGemCount != 1 || iInvalidItemCount > 0 )
	{
		DataSend(lpObj->m_Index, (UCHAR*)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}
	
	if ( iCharmOfLuckCount > 10 )
	{
		pMsg.Result = 0xF0;
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	LogChaosItem(lpObj, "Fenrir_02Level_Mix");
	CHAOS_LOG.Output("[Fenrir Mix][Level 02] Chaos Mix Start");

	lpObj->ChaosSuccessRate = ( ReadConfig.Fenrir02LevelMixRate )&255;
	lpObj->ChaosSuccessRate = ( lpObj->ChaosSuccessRate + iCharmOfLuckCount )&255;
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

	int nChaosNeedMoney = 0;	// #error Set the Correct price before call the CstleSyegeTributes

	int iChaosTaxMoney = (int)((__int64)nChaosNeedMoney * (__int64)g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / (__int64)100);

	if ( iChaosTaxMoney < 0 )
	{
		iChaosTaxMoney = 0;
	}

	nChaosNeedMoney += iChaosTaxMoney;

	if ( nChaosNeedMoney < 0 )
	{
		nChaosNeedMoney = 0;
	}

	if ( lpObj->Money < nChaosNeedMoney )
	{
		pMsg.Result = CB_NOT_ENOUGH_ZEN;
		
		DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	lpObj->Money -= nChaosNeedMoney;
	g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);
	GCMoneySend(lpObj->m_Index, lpObj->Money);

	int iRate = (rand()%100);
	if ( iRate < lpObj->ChaosSuccessRate )
	{
		int type = ITEMGET(13,36);
		int level = 0;
		int dur = 1;

		ItemSerialCreateSend(lpObj->m_Index, -1, 0, 0, type , level, dur, 0, 0, 0, -1, 0, 0);

		CHAOS_LOG.Output("[Fenrir Mix][Level 02] [%s][%s] CBMix Success %d/%d Money : %d-%d, CharmRate : %d",
			lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, iRate, lpObj->Money, nChaosNeedMoney,
			iCharmOfLuckCount);

		MuItemShop.EarnGoblinPointsCBMix(lpObj->m_Index, lpObj->ChaosSuccessRate, 0);
	}
	else
	{
		ChaosBoxInit(lpObj);
		::GCUserChaosBoxSend(lpObj, 0);
		DataSend(lpObj->m_Index, (UCHAR*)&pMsg, pMsg.h.size);

		CHAOS_LOG.Output("[Fenrir Mix][Level 02] [%s][%s] CBMix Fail %d/%d Money : %d-%d, CharmRate : %d",
			lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, iRate, lpObj->Money, nChaosNeedMoney,
			iCharmOfLuckCount);

		MuItemShop.EarnGoblinPointsCBMix(lpObj->m_Index, lpObj->ChaosSuccessRate, 1);
	}

	::gObjInventoryCommit(lpObj->m_Index);
	lpObj->ChaosLock = FALSE;
}




void Fenrir_03Level_Mix(LPOBJ lpObj)
{
	lpObj->ChaosLock = TRUE;
	int iStuffCount_01 = 0;
	int iLifeGemCount = 0;
	int iChaosGemCount = 0;
	int iInvalidItemCount = 0;
	//int iChaosMoney = 0;
	int iCharmOfLuckCount = 0;

	PMSG_CHAOSMIXRESULT pMsg;

	PHeadSetB((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR;


	for ( int n=0;n<CHAOS_BOX_SIZE;n++)
	{
		if ( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			if ( lpObj->pChaosBox[n].m_Type == ITEMGET(13,36) )
			{
				iStuffCount_01 ++;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,16) )
			{
				iLifeGemCount ++;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(12,15) )
			{
				iChaosGemCount++;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,53) )	// Charm Of Luck
			{
				iCharmOfLuckCount += lpObj->pChaosBox[n].m_Durability;
			}
			else
			{
				iInvalidItemCount++;
			}
		}
	}

	if ( iStuffCount_01 != 1 || iLifeGemCount != 3 || iChaosGemCount != 1 || iInvalidItemCount > 0 )
	{
		DataSend(lpObj->m_Index, (UCHAR*)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}
	
	if ( iCharmOfLuckCount > 10 )
	{
		pMsg.Result = 0xF0;
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	LogChaosItem(lpObj, "Fenrir_03Level_Mix");
	CHAOS_LOG.Output("[Fenrir Mix][Level 03] Chaos Mix Start");

	lpObj->ChaosSuccessRate = ( ReadConfig.Fenrir03LevelMixRate )&255;
	lpObj->ChaosSuccessRate = ( lpObj->ChaosSuccessRate + iCharmOfLuckCount )&255;
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

	int nChaosNeedMoney = 10000000;

	int iChaosTaxMoney = (int)((__int64)nChaosNeedMoney * (__int64)g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / (__int64)100);

	if ( iChaosTaxMoney < 0 )
	{
		iChaosTaxMoney = 0;
	}

	nChaosNeedMoney += iChaosTaxMoney;

	if ( nChaosNeedMoney < 0 )
	{
		nChaosNeedMoney = 0;
	}

	if ( lpObj->Money < nChaosNeedMoney )
	{
		pMsg.Result = CB_NOT_ENOUGH_ZEN;
		
		DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	lpObj->Money -= nChaosNeedMoney;
	g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);
	GCMoneySend(lpObj->m_Index, lpObj->Money);

	int iRate = (rand()%100);
	if ( iRate < lpObj->ChaosSuccessRate )
	{
		int type = ITEMGET(13,37);
		int level = 0;
		int dur = 255;
		int op1 = 1;

		ItemSerialCreateSend(lpObj->m_Index, -1, 0, 0, type , level, dur, op1, 0, 0, -1, 0, 0);

		CHAOS_LOG.Output("[Fenrir Mix][Level 03] [%s][%s] CBMix Success %d/%d Money : %d-%d, CharmRate : %d",
			lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, iRate, lpObj->Money, nChaosNeedMoney,
			iCharmOfLuckCount);

		MuItemShop.EarnGoblinPointsCBMix(lpObj->m_Index, lpObj->ChaosSuccessRate, 0);
	}
	else
	{
		ChaosBoxInit(lpObj);
		::GCUserChaosBoxSend(lpObj, 0);
		DataSend(lpObj->m_Index, (UCHAR*)&pMsg, pMsg.h.size);

		CHAOS_LOG.Output("[Fenrir Mix][Level 03] [%s][%s] CBMix Fail %d/%d Money : %d-%d, CharmRate : %d",
			lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, iRate, lpObj->Money, nChaosNeedMoney,
			iCharmOfLuckCount);

		MuItemShop.EarnGoblinPointsCBMix(lpObj->m_Index, lpObj->ChaosSuccessRate, 1);
	}

	::gObjInventoryCommit(lpObj->m_Index);
	lpObj->ChaosLock = FALSE;
}


void Fenrir_04Upgrade_Mix(LPOBJ lpObj)
{
	lpObj->ChaosLock = TRUE;
	int iStuffCount_01 = 0;
	int iLifeGemCount = 0;
	int iChaosGemCount = 0;
	int iAttackStuffCount = 0;
	int iDefendStuffCount = 0;
	int iAttackStuffPrice = 0;
	int iDefendStuffPrice = 0;
	int iInvalidItemCount = 0;
	int iChaosMoney = 0;
	BOOL bFenrirDamageInc = FALSE;
	BOOL bFenrirDamageDec = FALSE;
	int iCharmOfLuckCount = 0;

	PMSG_CHAOSMIXRESULT pMsg;

	PHeadSetB((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR;


	for ( int n=0;n<CHAOS_BOX_SIZE;n++)
	{
		if ( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			if ( lpObj->pChaosBox[n].m_Type == ITEMGET(13,37) )
			{
				iStuffCount_01 ++;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,16) )
			{
				iLifeGemCount ++;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(12,15) )
			{
				iChaosGemCount++;
			}
			else if ( lpObj->pChaosBox[n].m_Type >= ITEMGET(0,0) &&
					  lpObj->pChaosBox[n].m_Type < ITEMGET(6,0) &&
					  lpObj->pChaosBox[n].m_Level >= 4 &&
					  lpObj->pChaosBox[n].m_Z28Option >= 1)
			{
				iAttackStuffCount++;
				iAttackStuffPrice += lpObj->pChaosBox[n].m_BuyMoney;
			}
			else if ( lpObj->pChaosBox[n].m_Type >= ITEMGET(6,0) &&
					  lpObj->pChaosBox[n].m_Type < ITEMGET(12,0) &&
					  lpObj->pChaosBox[n].m_Level >= 4 &&
					  lpObj->pChaosBox[n].m_Z28Option >= 1)
			{
				iDefendStuffCount++;
				iDefendStuffPrice += lpObj->pChaosBox[n].m_BuyMoney;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,53) )	// Charm Of Luck
			{
				iCharmOfLuckCount += lpObj->pChaosBox[n].m_Durability;
			}
			else
			{
				iInvalidItemCount++;
			}
		}
	}

	if ( iStuffCount_01 != 1 || iLifeGemCount != 5 || iChaosGemCount != 1 || ( iAttackStuffCount == 0 && iDefendStuffCount == 0 ) || iInvalidItemCount > 0 )
	{
		DataSend(lpObj->m_Index, (UCHAR*)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}
	
	if ( iCharmOfLuckCount > 10 )
	{
		pMsg.Result = 0xF0;
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	if ( iAttackStuffCount > 0 )
	{
		bFenrirDamageInc = TRUE;
		iChaosMoney = iAttackStuffPrice;
	}

	if ( iDefendStuffCount > 0 )
	{
		bFenrirDamageDec = TRUE;
		iChaosMoney = iDefendStuffPrice;
	}

	if ( bFenrirDamageInc && bFenrirDamageDec )
	{
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	if ( !bFenrirDamageInc && !bFenrirDamageDec )
	{
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	LogChaosItem(lpObj, "Fenrir_04Level_Mix");
	CHAOS_LOG.Output("[Fenrir Mix][Level 04] Chaos Mix Start");

	int nChaosNeedMoney = 10000000;

	int iChaosTaxMoney = (int)((__int64)nChaosNeedMoney * (__int64)g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / (__int64)100);

	if ( iChaosTaxMoney < 0 )
	{
		iChaosTaxMoney = 0;
	}

	nChaosNeedMoney += iChaosTaxMoney;

	if ( nChaosNeedMoney < 0 )
	{
		nChaosNeedMoney = 0;
	}

	if ( lpObj->Money < nChaosNeedMoney )
	{
		pMsg.Result = CB_NOT_ENOUGH_ZEN;
		
		DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	lpObj->Money -= nChaosNeedMoney;
	g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);
	GCMoneySend(lpObj->m_Index, lpObj->Money);

	if ( iChaosMoney > 1000000 )
		iChaosMoney = 1000000;

	lpObj->ChaosSuccessRate = ( iChaosMoney * 100 / 1000000 )&255;

	if ( lpObj->ChaosSuccessRate > ReadConfig.FenrirUpgradeMaxPercent )
		lpObj->ChaosSuccessRate = ReadConfig.FenrirUpgradeMaxPercent;

	lpObj->ChaosSuccessRate = ( lpObj->ChaosSuccessRate + iCharmOfLuckCount )&255;

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

	int iRate = (rand()%100);
	if ( iRate < lpObj->ChaosSuccessRate )
	{
		int type = ITEMGET(13,37);
		int level = 0;
		int dur = 255;
		int op1 = 1;
		int nop = 0;

		if ( bFenrirDamageInc )
		{
			nop |= 1;
		}

		if ( bFenrirDamageDec )
		{
			nop |= 2;
		}

		ItemSerialCreateSend(lpObj->m_Index, -1, 0, 0, type , level, dur, op1, 0, 0, -1, nop, 0);

		CHAOS_LOG.Output("[Fenrir Mix][Level 04] [%s][%s] CBMix Success %d/%d Money : %d-%d, CharmRate : %d",
			lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, iRate, lpObj->Money, nChaosNeedMoney,
			iCharmOfLuckCount);

		MuItemShop.EarnGoblinPointsCBMix(lpObj->m_Index, lpObj->ChaosSuccessRate, 0);
	}
	else
	{
		ChaosBoxInit(lpObj);
		::GCUserChaosBoxSend(lpObj, 0);
		DataSend(lpObj->m_Index, (UCHAR*)&pMsg, pMsg.h.size);

		CHAOS_LOG.Output("[Fenrir Mix][Level 04] [%s][%s] CBMix Fail %d/%d Money : %d-%d, CharmRate : %d",
			lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, iRate, lpObj->Money, nChaosNeedMoney,
			iCharmOfLuckCount);
		
		MuItemShop.EarnGoblinPointsCBMix(lpObj->m_Index, lpObj->ChaosSuccessRate, 1);
	}

	::gObjInventoryCommit(lpObj->m_Index);
	lpObj->ChaosLock = FALSE;
}




void ShieldPotionLv1_Mix(LPOBJ lpObj)
{
	int iHealthPotionCount = 0;
	int iInvalidItemCount = 0;
	int iChaosMixPrice = 0;
	int iCharmOfLuckCount = 0;

	if ( g_ShieldSystemOn == FALSE )
		return;

	PMSG_CHAOSMIXRESULT pMsg;

	PHeadSetB((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR;

	for ( int n=0;n<CHAOS_BOX_SIZE;n++)
	{
		if ( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,3) )
			{
				iHealthPotionCount+=lpObj->pChaosBox[n].m_Durability;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,53) )	// Charm Of Luck
			{
				iCharmOfLuckCount += lpObj->pChaosBox[n].m_Durability;
			}
			else
			{
				iInvalidItemCount++;
			}
		}
	}

	if ( iInvalidItemCount > 0 || iHealthPotionCount != 3 )
	{
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	if ( iCharmOfLuckCount > 10 )
	{
		pMsg.Result = 0xF0;
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	iChaosMixPrice = ReadConfig.ShieldPotionLv1MixMoney;
	int iChaosTaxMoney = iChaosMixPrice * g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / 100;

	if ( iChaosTaxMoney < 0 )
	{
		iChaosTaxMoney = 0;
	}

	iChaosMixPrice += iChaosTaxMoney;

	if ( iChaosMixPrice < 0 )
	{
		iChaosMixPrice = 0;
	}

	if ( lpObj->Money < iChaosMixPrice )
	{
		pMsg.Result = CB_NOT_ENOUGH_ZEN;
		
		DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	lpObj->Money -= iChaosMixPrice;
	g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);
	GCMoneySend(lpObj->m_Index, lpObj->Money);

	LogChaosItem(lpObj, "PotionMix][ShieldPotion Lv1 Mix");
	CHAOS_LOG.Output("[PotionMix][ShieldPotion Lv1 Mix] - Mix Start");
	
	int iRate = rand() % 100;
	iRate -= iCharmOfLuckCount;

	lpObj->ChaosSuccessRate = ReadConfig.ShieldPotionLv1MixRate;
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

	if ( iRate < lpObj->ChaosSuccessRate )
	{
		int ItemNum = ITEMGET(14,35);

		ItemSerialCreateSend(lpObj->m_Index, -1, 0, 0, ItemNum, 0, 1, 0, 0, 0, lpObj->m_Index, 0, 0);

		CHAOS_LOG.Output("[Shield Potion][Lv1][%s][%s] CBMix Success %d/%d Money(%d-%d), CharmRate : %d",
			lpObj->AccountID, lpObj->Name, 	lpObj->ChaosSuccessRate, iRate,
			lpObj->Money, iChaosMixPrice, iCharmOfLuckCount);

		MuItemShop.EarnGoblinPointsCBMix(lpObj->m_Index, lpObj->ChaosSuccessRate, 0);
	}
	else
	{
		ChaosBoxInit(lpObj);
		::GCUserChaosBoxSend(lpObj, 0);
		DataSend(lpObj->m_Index, (UCHAR*)&pMsg, pMsg.h.size);

		CHAOS_LOG.Output("[Shield Potion][Lv1][%s][%s] CBMix Fail %d/%d Money : %d-%d, CharmRate : %d",
			lpObj->AccountID, lpObj->Name, 	lpObj->ChaosSuccessRate, iRate,
			lpObj->Money, iChaosMixPrice, iCharmOfLuckCount);

		MuItemShop.EarnGoblinPointsCBMix(lpObj->m_Index, lpObj->ChaosSuccessRate, 1);
	}

	::gObjInventoryCommit(lpObj->m_Index);
	lpObj->ChaosLock = FALSE;
}




void ShieldPotionLv2_Mix(LPOBJ lpObj)
{
	int iHealthPotionCount = 0;
	int iInvalidItemCount = 0;
	int iChaosMixPrice = 0;
	int iCharmOfLuckCount = 0;

	PMSG_CHAOSMIXRESULT pMsg;

	PHeadSetB((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR;

	for ( int n=0;n<CHAOS_BOX_SIZE;n++)
	{
		if ( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,38) )
			{
				iHealthPotionCount+=lpObj->pChaosBox[n].m_Durability;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,53) )	// Charm Of Luck
			{
				iCharmOfLuckCount += lpObj->pChaosBox[n].m_Durability;
			}
			else
			{
				iInvalidItemCount++;
			}
		}
	}

	if ( iInvalidItemCount > 0 || iHealthPotionCount != 3 )
	{
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	if ( iCharmOfLuckCount > 10 )
	{
		pMsg.Result = 0xF0;
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	iChaosMixPrice = ReadConfig.ShieldPotionLv2MixMoney;
	int iChaosTaxMoney = iChaosMixPrice * g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / 100;

	if ( iChaosTaxMoney < 0 )
	{
		iChaosTaxMoney = 0;
	}

	iChaosMixPrice += iChaosTaxMoney;

	if ( iChaosMixPrice < 0 )
	{
		iChaosMixPrice = 0;
	}

	if ( lpObj->Money < iChaosMixPrice )
	{
		pMsg.Result = CB_NOT_ENOUGH_ZEN;
		
		DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	lpObj->Money -= iChaosMixPrice;
	g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);
	
	LogChaosItem(lpObj, "PotionMix][ShieldPotion Lv2 Mix");
	CHAOS_LOG.Output("[PotionMix][ShieldPotion Lv2 Mix] - Mix Start");

	GCMoneySend(lpObj->m_Index, lpObj->Money);
	int iRate = rand() % 100;
	iRate -= iCharmOfLuckCount;

	lpObj->ChaosSuccessRate = ReadConfig.ShieldPotionLv2MixRate;
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

	if ( iRate < lpObj->ChaosSuccessRate )
	{
		int ItemNum = ITEMGET(14,36);

		ItemSerialCreateSend(lpObj->m_Index, -1, 0, 0, ItemNum, 0, 1, 0, 0, 0, lpObj->m_Index, 0, 0);

		CHAOS_LOG.Output("[Shield Potion][Lv2][%s][%s] CBMix Success %d/%d  Money(%d-%d), CharmRate : %d",
			lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, iRate,
			lpObj->Money, iChaosMixPrice, iCharmOfLuckCount);

		MuItemShop.EarnGoblinPointsCBMix(lpObj->m_Index, lpObj->ChaosSuccessRate, 0);
	}
	else
	{
		ChaosBoxInit(lpObj);
		::GCUserChaosBoxSend(lpObj, 0);
		DataSend(lpObj->m_Index, (UCHAR*)&pMsg, pMsg.h.size);

		CHAOS_LOG.Output("[Shield Potion][Lv2][%s][%s] CBMix Fail %d/%d Money : %d-%d, CharmRate : %d",
			lpObj->AccountID, lpObj->Name, 	lpObj->ChaosSuccessRate, iRate,
			lpObj->Money, iChaosMixPrice, iCharmOfLuckCount);

		MuItemShop.EarnGoblinPointsCBMix(lpObj->m_Index, lpObj->ChaosSuccessRate, 1);
	}

	::gObjInventoryCommit(lpObj->m_Index);
	lpObj->ChaosLock = FALSE;
}


void ShieldPotionLv3_Mix(LPOBJ lpObj)
{
	int iHealthPotionCount = 0;
	int iInvalidItemCount = 0;
	int iChaosMixPrice = 0;
	int iCharmOfLuckCount = 0;

	PMSG_CHAOSMIXRESULT pMsg;

	PHeadSetB((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR;

	for ( int n=0;n<CHAOS_BOX_SIZE;n++)
	{
		if ( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,39) )
			{
				iHealthPotionCount+=lpObj->pChaosBox[n].m_Durability;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,53) )	// Charm Of Luck
			{
				iCharmOfLuckCount += lpObj->pChaosBox[n].m_Durability;
			}
			else
			{
				iInvalidItemCount++;
			}
		}
	}

	if ( iInvalidItemCount > 0 || iHealthPotionCount != 3 )
	{
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	if ( iCharmOfLuckCount > 10 )
	{
		pMsg.Result = 0xF0;
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	iChaosMixPrice = ReadConfig.ShieldPotionLv3MixMoney;
	int iChaosTaxMoney = iChaosMixPrice * g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / 100;

	if ( iChaosTaxMoney < 0 )
	{
		iChaosTaxMoney = 0;
	}

	iChaosMixPrice += iChaosTaxMoney;

	if ( iChaosMixPrice < 0 )
	{
		iChaosMixPrice = 0;
	}

	if ( lpObj->Money < iChaosMixPrice )
	{
		pMsg.Result = CB_NOT_ENOUGH_ZEN;
		
		DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	lpObj->Money -= iChaosMixPrice;
	g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);
	GCMoneySend(lpObj->m_Index, lpObj->Money);
	
	LogChaosItem(lpObj, "PotionMix][ShieldPotion Lv3 Mix");
	CHAOS_LOG.Output("[PotionMix][ShieldPotion Lv3 Mix] - Mix Start");

	int iRate = rand() % 100;
	iRate -= iCharmOfLuckCount;

	lpObj->ChaosSuccessRate = ReadConfig.ShieldPotionLv3MixRate;
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

	if ( iRate < lpObj->ChaosSuccessRate )
	{
		int ItemNum = ITEMGET(14,37);

		ItemSerialCreateSend(lpObj->m_Index, -1, 0, 0, ItemNum, 0, 1, 0, 0, 0, lpObj->m_Index, 0, 0);

		CHAOS_LOG.Output("[Shield Potion][Lv3][%s][%s] CBMix Success %d/%d Money(%d-%d), CharmRate : %d",
			lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, iRate,
			lpObj->Money, iChaosMixPrice, iCharmOfLuckCount);

		MuItemShop.EarnGoblinPointsCBMix(lpObj->m_Index, lpObj->ChaosSuccessRate, 0);
	}
	else
	{
		ChaosBoxInit(lpObj);
		::GCUserChaosBoxSend(lpObj, 0);
		DataSend(lpObj->m_Index, (UCHAR*)&pMsg, pMsg.h.size);

		CHAOS_LOG.Output("[Shield Potion][Lv3][%s][%s] CBMix Fail %d/%d Money : %d-%d, CharmRate : %d",
			lpObj->AccountID, lpObj->Name, 	lpObj->ChaosSuccessRate, iRate,
			lpObj->Money, iChaosMixPrice, iCharmOfLuckCount);

		MuItemShop.EarnGoblinPointsCBMix(lpObj->m_Index, lpObj->ChaosSuccessRate, 1);
	}

	::gObjInventoryCommit(lpObj->m_Index);
	lpObj->ChaosLock = FALSE;
}




void LotteryItemMix(LPOBJ lpObj)
{
	lpObj->ChaosLock = TRUE;

	PMSG_CHAOSMIXRESULT pMsg;

	PHeadSetB((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR;

	BYTE LotteryTYPE = 0;
	int iLotteryItemCount = 0;
	int iInvalidItemCount = 0;
	int iItemHeight = 0;
	int iItemWidth = 0;

	for ( int n=0;n<CHAOS_BOX_SIZE;n++)
	{
		if ( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,54) ||
				 lpObj->pChaosBox[n].m_Type == ITEMGET(14,92) ||
				 lpObj->pChaosBox[n].m_Type == ITEMGET(14,93) ||
				 lpObj->pChaosBox[n].m_Type == ITEMGET(14,95) )
			{
				iLotteryItemCount++;

				if (lpObj->pChaosBox[n].m_Type == ITEMGET(14,54))
					LotteryTYPE = 0;	//Regular
				else if (lpObj->pChaosBox[n].m_Type == ITEMGET(14,92)) 
					LotteryTYPE = 1;	//Gold
				else if (lpObj->pChaosBox[n].m_Type == ITEMGET(14,93)) 
					LotteryTYPE = 2;	//Rare
				else if (lpObj->pChaosBox[n].m_Type == ITEMGET(14,95)) 
					LotteryTYPE = 3;	//Mini
			}
			else
			{
				iInvalidItemCount++;
			}
		}
	}

	if ( iInvalidItemCount > 0 || iLotteryItemCount > 1 || iLotteryItemCount == 0 )
	{
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	LogChaosItem(lpObj, "CashLottery");
	iItemHeight = 4;
	iItemWidth = 2;

	if ( !CheckInventoryEmptySpace(lpObj, iItemHeight, iItemWidth))
	{
		pMsg.Result = 0xF1;
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	CItem LotteryItem;

	if ( g_CashLotterySystem.GetItem(&LotteryItem,LotteryTYPE) == -1 )
	{
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;
		CHAOS_LOG.Output("[ChaosMix][LotteryItemMix][TYPE:%d] Lottery Item Mix Failed. Can't Get Item from List.",LotteryTYPE);

		return;
	}

	ItemSerialCreateSend(lpObj->m_Index, 255, 0, 0, LotteryItem.m_Type,
		LotteryItem.m_Level, LotteryItem.m_Durability,
		LotteryItem.m_SkillOption, LotteryItem.m_LuckOption, LotteryItem.m_Z28Option,
		lpObj->m_Index, LotteryItem.m_NewOption, 0);

	gObjInventoryCommit(lpObj->m_Index);

	CHAOS_LOG.Output("[ChaosMix][ChaosCardMix][TYPE:%d] Chaos Card Item Mix Success [%s][%s]",
		LotteryTYPE,
		lpObj->AccountID, lpObj->Name);

	BYTE btExOption[MAX_EXOPTION_SIZE];

	ItemIsBufExOption(btExOption, &LotteryItem);
	
	CHAOS_LOG.Output("[CashShop][ChaosCardMix][TYPE:%d] - User(ID:%s,Name:%s) Item(Name:%s,Type:%d,Level:%d,Dur:%d,Skill:%d,Luck:%d,AddOption:%d,Ex:(%d,%d,%d,%d,%d,%d))",
		LotteryTYPE,
		lpObj->AccountID, lpObj->Name, ItemAttribute[LotteryItem.m_Type].Name,
		LotteryItem.m_Type, LotteryItem.m_Level, (int)LotteryItem.m_Durability,
		LotteryItem.m_SkillOption, LotteryItem.m_LuckOption, LotteryItem.m_Z28Option,
		btExOption[0], btExOption[1], btExOption[2], btExOption[3], btExOption[4], btExOption[5]);

	lpObj->ChaosLock = FALSE;
}


int CheckSeedItem(LPOBJ lpObj, int & SeedNr, int & SeedLvl, int & EmptySpearLvl)
{
	BOOL bChaoseGem = FALSE;
	BOOL bSeed = FALSE;
	BOOL bEmptySpear = FALSE;
	BOOL bCreationGem = FALSE;
	
	int itemcount = 0;
	BOOL FoundOtherItem = FALSE;
	BYTE Level=0;
	BYTE Type=0;
	BYTE ESLevel=0;

	SeedNr = 0;
	SeedLvl = 0;
	EmptySpearLvl = 0;

	CHAOS_LOG.Output("[SeedMix] Search Seed Item");

	for ( int n=0;n<CHAOS_BOX_SIZE;n++)
	{
		if ( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			if ( lpObj->pChaosBox[n].m_Type == ITEMGET(12,15) )	// Chaos
			{
				bChaoseGem = TRUE;
			}
			else if (( lpObj->pChaosBox[n].m_Type >= 0x183C ) && ( lpObj->pChaosBox[n].m_Type <= 0x1841 ))	// Seed
			{
				itemcount +=1;
				bSeed = TRUE;

				if (lpObj->pChaosBox[n].m_Type == ITEMGET(12,60))
				{
					Type = 60;
				}else if (lpObj->pChaosBox[n].m_Type == ITEMGET(12,61))
				{
					Type = 61;				
				}else if (lpObj->pChaosBox[n].m_Type == ITEMGET(12,62))
				{
					Type = 62;					
				}else if (lpObj->pChaosBox[n].m_Type == ITEMGET(12,63))
				{
					Type = 63;					
				}else if (lpObj->pChaosBox[n].m_Type == ITEMGET(12,64))
				{
					Type = 64;					
				}else if (lpObj->pChaosBox[n].m_Type == ITEMGET(12,65))
				{					
					Type = 65;
				}
				
				Level = lpObj->pChaosBox[n].m_Level;

			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(12,70) )	// Empty Spear
			{
				itemcount +=1;
				bEmptySpear = TRUE;
				ESLevel = 0;

			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(12,71) )	// Empty Spear
			{
				itemcount +=1;
				bEmptySpear = TRUE;
				ESLevel = 1;

			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(12,72) )	// Empty Spear
			{
				itemcount +=1;
				bEmptySpear = TRUE;
				ESLevel = 2;

			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(12,73) )	// Empty Spear
			{
				itemcount +=1;
				bEmptySpear = TRUE;
				ESLevel = 3;

			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(12,74) )	// Empty Spear
			{
				itemcount +=1;
				bEmptySpear = TRUE;
				ESLevel = 4;

			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,22) )	// Creation
			{
				bCreationGem = TRUE;
			}
			else
			{
				FoundOtherItem = TRUE;
			}
		}
	}

	
	if ( itemcount < 2 )
	{
		CHAOS_LOG.Output("[SeedMix] Need More Items Founded");
		return 4;
	}

	if ( itemcount > 2 )
	{
		CHAOS_LOG.Output("[SeedMix] Much Seed Item Founded");
		return 3;
	}

	if ( FoundOtherItem != FALSE )
	{
		CHAOS_LOG.Output("[SeedMix] Other Seed Item Found");
		return 0;
	}

	if (bChaoseGem != TRUE)
	{
		CHAOS_LOG.Output("[SeedMix] No Chaos Gem Found");
		return 2;		
	}

	if (bCreationGem != TRUE)
	{
		CHAOS_LOG.Output("[SeedMix] No Creation Gem Found");
		return 5;		
	}

	if(bSeed == TRUE && bEmptySpear == TRUE)
	{
		SeedNr	= Type;
		SeedLvl	= Level;
		EmptySpearLvl = ESLevel;

		return 1;
	}

	CHAOS_LOG.Output("[SeedMix] Seed Item Not Found");

	return 0;
}


BOOL SeedChaosMix(LPOBJ lpObj, int SeedNr, int SeedLvl,int EmptySpearLvl)
{
	BOOL fail = TRUE;
	int aIndex = lpObj->m_Index;
	PMSG_CHAOSMIXRESULT pMsg;

	PHeadSetB((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR;
	lpObj->ChaosLock = TRUE;

	CHAOS_LOG.Output("[SeedMix] Chaos Mix Start");
	INT nChaosNeedMoney = 1000000;

	
	int iChaosTaxMoney = (int)((__int64)nChaosNeedMoney * (__int64)g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / (__int64)100);

	if ( iChaosTaxMoney < 0 )
	{
		iChaosTaxMoney = 0;
	}

	nChaosNeedMoney += iChaosTaxMoney;

	if ( nChaosNeedMoney < 0 )
	{
		nChaosNeedMoney = 0;
	}

	if ( (lpObj->Money - nChaosNeedMoney) < 0 )
	{
		CHAOS_LOG.Output("[SeedMix] [%s][%s] CBMix Not Enough Money [%d] need zen [%d]",
			lpObj->AccountID, lpObj->Name, lpObj->Money, nChaosNeedMoney);
		pMsg.Result = CB_NOT_ENOUGH_ZEN;
		DataSend(aIndex, (BYTE *)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;
		return 1;

	}

	lpObj->ChaosSuccessRate = ReadConfig.SeedMixMaxSuccessRate;
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

	int iRate = rand() % 100;

	if ( iRate <= lpObj->ChaosSuccessRate )
	{
		int SpearNr = SeedNr + 40 + (EmptySpearLvl * 6);
		int SpearItem = ITEMGET(12,SpearNr);	// Spear
		ItemSerialCreateSend(lpObj->m_Index, -1, 0, 0, SpearItem, SeedLvl, 0, 0, 0, 0, -1, 0, 0);
		fail = FALSE;
	}

	lpObj->Money -= nChaosNeedMoney;
	g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);
	GCMoneySend(lpObj->m_Index, lpObj->Money);

	if ( fail == TRUE )
	{
		ChaosBoxInit(lpObj);
		GCUserChaosBoxSend(lpObj, 0);
		DataSend(aIndex, (BYTE *)&pMsg, pMsg.h.size);

		CHAOS_LOG.Output("[SeedMix,%d] [%s][%s] CBMix Fail %d/%d Money : %d-%d", SeedLvl,
			lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, iRate,
			lpObj->Money, nChaosNeedMoney);

		MuItemShop.EarnGoblinPointsCBMix(lpObj->m_Index, lpObj->ChaosSuccessRate, 1);
	}
	else
	{
		CHAOS_LOG.Output("[SeedMix,%d] [%s][%s] CBMix Success %d/%d Money : %d", SeedLvl,
			lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, iRate, 
			nChaosNeedMoney);

		MuItemShop.EarnGoblinPointsCBMix(lpObj->m_Index, lpObj->ChaosSuccessRate, 0);
	}

	lpObj->ChaosLock = FALSE;
	::gObjInventoryCommit(lpObj->m_Index);

	return TRUE;
}



void SocketSeedChaosMix(LPOBJ lpObj)
{
	lpObj->ChaosLock = TRUE;

	PMSG_CHAOSMIXRESULT pMsg;
	PHeadSetB((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR;
	int aIndex = lpObj->m_Index;
	int SeedNr=0;
	int SeedLvl=0;
	int EmptySpearLvl=0;
	int Ret = ::CheckSeedItem(lpObj, SeedNr, SeedLvl,EmptySpearLvl);

	if ( Ret != FALSE )
	{			
		if ( Ret == 2 )
		{
			pMsg.Result = CB_INCORRECT_MIX_ITEMS;
			DataSend(aIndex, (BYTE*)&pMsg, pMsg.h.size);
			CHAOS_LOG.Output("[SeedMix] Not Found Chaos Gem");
			lpObj->ChaosLock = FALSE;
			
			return;
		}

		if ( Ret == 5 )
		{
			pMsg.Result = CB_INCORRECT_MIX_ITEMS;
			DataSend(aIndex, (BYTE*)&pMsg, pMsg.h.size);
			CHAOS_LOG.Output("[SeedMix] Not Found Creation Gem");
			lpObj->ChaosLock = FALSE;
			
			return;
		}
		
		if ( Ret == 3 )
		{
			pMsg.Result = CB_TOO_MANY_ITEMS;
			DataSend(aIndex, (BYTE*)&pMsg, pMsg.h.size);
			CHAOS_LOG.Output("[SeedMix] Too many Seed or Empty Spears");
			lpObj->ChaosLock = FALSE;

			return;
		}
	
		if ( Ret == 4 )
		{
			pMsg.Result = CB_LACKING_MIX_ITEMS;
			DataSend(aIndex, (BYTE*)&pMsg, pMsg.h.size);
			lpObj->ChaosLock = FALSE;

			return;
		}

		if ( lpObj->Level < 10 )
		{
			pMsg.Result = CB_LOW_LEVEL_USER;
			DataSend(aIndex, (BYTE*)&pMsg, pMsg.h.size);
			lpObj->ChaosLock = FALSE;

			return;
		}

		::SeedChaosMix(lpObj, SeedNr, SeedLvl,EmptySpearLvl);
		//return;
	}

	lpObj->ChaosLock = FALSE;
}


int CheckSocketItemAdd(LPOBJ lpObj, int & SpearType, int & SpearLvl, int & SpearNr, BYTE SocketSlot)
{
	BOOL bChaoseGem = FALSE;
	BOOL bSpear = FALSE;
	BYTE ItemSlot = 0;
	BOOL bSocketItem = FALSE;
	BOOL bCreationGem = FALSE;
	
	int itemcount = 0;
	BOOL FoundOtherItem = FALSE;

	SpearNr = 0;
	SpearLvl = 0;

	int SpearTmp = 0;

	CHAOS_LOG.Output("[SocketOptAddMix] Search Socket Item");

	for ( int n=0;n<CHAOS_BOX_SIZE;n++)
	{
		if ( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			if ( lpObj->pChaosBox[n].m_Type == ITEMGET(12,15) )	// Chaos
			{
				bChaoseGem = TRUE;
			}
			else if (( lpObj->pChaosBox[n].m_Type >= 0x1864 ) && ( lpObj->pChaosBox[n].m_Type <= 0x1881 ))	// Spear
			{
				itemcount +=1;
				bSpear = TRUE;

				if (lpObj->pChaosBox[n].m_Type >= 0x1864 && lpObj->pChaosBox[n].m_Type <= 0x1869)
				{
					SpearNr = 0;
					SpearTmp = lpObj->pChaosBox[n].m_Type - 0x1864;
				}else if (lpObj->pChaosBox[n].m_Type >= 0x186A && lpObj->pChaosBox[n].m_Type <= 0x186F)
				{
					SpearNr = 1;
					SpearTmp = lpObj->pChaosBox[n].m_Type - 0x186A;			
				}else if (lpObj->pChaosBox[n].m_Type >= 0x1870 && lpObj->pChaosBox[n].m_Type <= 0x1875)
				{
					SpearNr = 2;
					SpearTmp = lpObj->pChaosBox[n].m_Type - 0x1870;					
				}else if (lpObj->pChaosBox[n].m_Type >= 0x1876 && lpObj->pChaosBox[n].m_Type <= 0x187B)
				{
					SpearNr = 3;
					SpearTmp = lpObj->pChaosBox[n].m_Type - 0x1876;				
				}else if (lpObj->pChaosBox[n].m_Type >= 0x187C && lpObj->pChaosBox[n].m_Type <= 0x1881)
				{
					SpearNr = 4;
					SpearTmp = lpObj->pChaosBox[n].m_Type - 0x187C;					
				}else
				{
					SpearNr = 5;	
					SpearTmp = lpObj->pChaosBox[n].m_Type - 0x1882;
				}

				switch(SpearTmp)
				{
					case 0:
					{
						SpearType = 0;
					}break;
					case 1:
					{
						SpearType = 0x0A;
					}break;
					case 2:
					{
						SpearType = 0x10;
					}break;
					case 3:
					{
						SpearType = 0x15;
					}break;
					case 4:
					{
						SpearType = 0x1D;
					}break;
					case 5:
					{
						SpearType = 0x24;
					}break;
				}

				SpearLvl = lpObj->pChaosBox[n].m_Level;
			}
			else if ( IsSlotItem(lpObj->pChaosBox[n].m_Type) )	// Socket Item
			{
				switch(SocketSlot)
				{
					case 1:
					{
						if( lpObj->pChaosBox[n].m_ItemSlot1 == 0xFF )
						{
							ItemSlot = n;
							itemcount +=1;
							bSocketItem = TRUE;
						}
					}break;
					case 2:
					{
						if( lpObj->pChaosBox[n].m_ItemSlot2 == 0xFF )
						{
							ItemSlot = n;
							itemcount +=1;
							bSocketItem = TRUE;
						}
					}break;
					case 3:
					{
						if( lpObj->pChaosBox[n].m_ItemSlot3 == 0xFF )
						{
							ItemSlot = n;
							itemcount +=1;
							bSocketItem = TRUE;
						}
					}break;
					case 4:
					{
						if( lpObj->pChaosBox[n].m_ItemSlot4 == 0xFF )
						{
							ItemSlot = n;
							itemcount +=1;
							bSocketItem = TRUE;
						}
					}break;
					case 5:
					{
						if( lpObj->pChaosBox[n].m_ItemSlot5 == 0xFF )
						{
							ItemSlot = n;
							itemcount +=1;
							bSocketItem = TRUE;
						}
					}break;
				}
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,22) )	// Creation
			{
				bCreationGem = TRUE;
			}
			else
			{
				FoundOtherItem = TRUE;
			}
		}
	}

	
	if ( itemcount < 2 )
	{
		CHAOS_LOG.Output("[SocketOptAddMix] Need More Items Founded");
		return 4;
	}

	if ( itemcount > 2 )
	{
		CHAOS_LOG.Output("[SocketOptAddMix] Much Items Founded");
		return 3;
	}

	if ( FoundOtherItem != FALSE )
	{
		CHAOS_LOG.Output("[SocketOptAddMix] Other Item Found");
		return 0;
	}

	if (bChaoseGem != TRUE)
	{
		CHAOS_LOG.Output("[SocketOptAddMix] No Chaos Gem Found");
		return 2;		
	}

	if (bCreationGem != TRUE)
	{
		CHAOS_LOG.Output("[SocketOptAddMix] No Creation Gem Found");
		return 5;		
	}

	int spearvalue = ((SpearType + SpearLvl) + (0 * 0x32))+1;

	int Lvl1 = lpObj->pChaosBox[ItemSlot].m_ItemSlot1 / 50;
	int Val1 = lpObj->pChaosBox[ItemSlot].m_ItemSlot1 - (Lvl1 * 50);
	if(lpObj->pChaosBox[ItemSlot].m_ItemSlot1 == 0xff)
		Val1 = 0;

	int Lvl2 = lpObj->pChaosBox[ItemSlot].m_ItemSlot2 / 50;
	int Val2 = lpObj->pChaosBox[ItemSlot].m_ItemSlot2 - (Lvl2 * 50);
	if(lpObj->pChaosBox[ItemSlot].m_ItemSlot2 == 0xff)
		Val2 = 0;

	int Lvl3 = lpObj->pChaosBox[ItemSlot].m_ItemSlot3 / 50;
	int Val3 = lpObj->pChaosBox[ItemSlot].m_ItemSlot3 - (Lvl3 * 50);
	if(lpObj->pChaosBox[ItemSlot].m_ItemSlot3 == 0xff)
		Val3 = 0;

	int Lvl4 = lpObj->pChaosBox[ItemSlot].m_ItemSlot4 / 50;
	int Val4 = lpObj->pChaosBox[ItemSlot].m_ItemSlot4 - (Lvl4 * 50);
	if(lpObj->pChaosBox[ItemSlot].m_ItemSlot4 == 0xff)
		Val4 = 0;

	int Lvl5 = lpObj->pChaosBox[ItemSlot].m_ItemSlot5 / 50;
	int Val5 = lpObj->pChaosBox[ItemSlot].m_ItemSlot5 - (Lvl5 * 50);
	if(lpObj->pChaosBox[ItemSlot].m_ItemSlot5 == 0xff)
		Val5 = 0;

	if((Val1 == spearvalue) || (Val2 == spearvalue) || (Val3 == spearvalue) || (Val4 == spearvalue) || (Val5 == spearvalue))
	{
		CHAOS_LOG.Output("[SocketOptAddMix] You cant add the same option in this item");
		return 6;
	}

	if(bSocketItem == TRUE && bSpear == TRUE)
	{
		return 1;
	}

	CHAOS_LOG.Output("[SocketOptAddMix] Socket Item Not Found");

	return 0;
}

BOOL SocketAddChaosMix(LPOBJ lpObj, int SpearType, int SpearLvl, int SpearNr, BYTE SocketSlot)
{
	int aIndex = lpObj->m_Index;
	PMSG_CHAOSMIXRESULT pMsg;

	PHeadSetB((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR;
	lpObj->ChaosLock = TRUE;

	CHAOS_LOG.Output("[SocketOptAddMix] Chaos Mix Start");
	INT nChaosNeedMoney = 1000000;

	
	int iChaosTaxMoney = (int)((__int64)nChaosNeedMoney * (__int64)g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / (__int64)100);

	if ( iChaosTaxMoney < 0 )
	{
		iChaosTaxMoney = 0;
	}

	nChaosNeedMoney += iChaosTaxMoney;

	if ( nChaosNeedMoney < 0 )
	{
		nChaosNeedMoney = 0;
	}

	if ( (lpObj->Money - nChaosNeedMoney) < 0 )
	{
		CHAOS_LOG.Output("[SocketOptAddMix] [%s][%s] CBMix Not Enough Money [%d] need zen [%d]",
			lpObj->AccountID, lpObj->Name, lpObj->Money, nChaosNeedMoney);
		pMsg.Result = CB_NOT_ENOUGH_ZEN;
		DataSend(aIndex, (BYTE *)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;
		return 1;

	}

	int nr = 0;

	for (int n=0;n<CHAOS_BOX_SIZE;n++)
	{
		if ( lpObj->pChaosBox[n].IsItem() == FALSE )
			continue;

		if(IsSlotItem(lpObj->pChaosBox[n].m_Type))
		{
			switch(SocketSlot)
			{
				case 1:
				{
					nr = n;					
					lpObj->pChaosBox[n].m_ItemSlot1 = (BYTE)(((SpearType + SpearLvl) + (SpearNr * 0x32))+1);
					ItemByteConvert(pMsg.ItemInfo, lpObj->pChaosBox[n]);
				}break;
				case 2:
				{
					nr = n;
					lpObj->pChaosBox[n].m_ItemSlot2 = (BYTE)(((SpearType + SpearLvl) + (SpearNr * 0x32))+1);
					ItemByteConvert(pMsg.ItemInfo, lpObj->pChaosBox[n]);
				}break;
				case 3:
				{
					nr = n;
					lpObj->pChaosBox[n].m_ItemSlot3 = (BYTE)(((SpearType + SpearLvl) + (SpearNr * 0x32))+1);
					ItemByteConvert(pMsg.ItemInfo, lpObj->pChaosBox[n]);
				}break;
				case 4:
				{
					nr = n;
					lpObj->pChaosBox[n].m_ItemSlot4 = (BYTE)(((SpearType + SpearLvl) + (SpearNr * 0x32))+1);
					ItemByteConvert(pMsg.ItemInfo, lpObj->pChaosBox[n]);
				}break;
				case 5:
				{
					nr = n;
					lpObj->pChaosBox[n].m_ItemSlot5 = (BYTE)(((SpearType + SpearLvl) + (SpearNr * 0x32))+1);
					ItemByteConvert(pMsg.ItemInfo, lpObj->pChaosBox[n]);
				}break;
			}
		}else
		{
			lpObj->pChaosBox[n].Clear();
		}		
	}

	if(nr != 0)
	{
		lpObj->pChaosBox[0] = lpObj->pChaosBox[nr];
		lpObj->pChaosBox[nr].Clear();
	}

	lpObj->Money -= nChaosNeedMoney;
	g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);
	GCMoneySend(lpObj->m_Index, lpObj->Money);

	pMsg.Result = CB_SUCCESS;
	GCUserChaosBoxSend(lpObj, 0);
	DataSend(aIndex, (BYTE*)&pMsg, pMsg.h.size);

	CHAOS_LOG.Output("[SocketOptAddMix] [%s][%s] CBMix Success", lpObj->AccountID, lpObj->Name);

	::gObjInventoryCommit(lpObj->m_Index);
	lpObj->ChaosLock = FALSE;
	return TRUE;
}

void SocketItemAddChaosMix(LPOBJ lpObj, BYTE SocketSlot)
{
	lpObj->ChaosLock = TRUE;

	SocketSlot++;

	PMSG_CHAOSMIXRESULT pMsg;
	PHeadSetB((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR;
	int aIndex = lpObj->m_Index;
	int SpearNr = 0;
	int SpearLvl = 0;
	int SpearType = 0;

	int Ret = ::CheckSocketItemAdd(lpObj, SpearType, SpearLvl, SpearNr, SocketSlot);

	if ( Ret != FALSE )
	{			
		if ( Ret == 2 )
		{
			pMsg.Result = CB_INCORRECT_MIX_ITEMS;
			DataSend(aIndex, (BYTE*)&pMsg, pMsg.h.size);
			CHAOS_LOG.Output("[SocketOptAddMix] Not Found Chaos Gem");
			lpObj->ChaosLock = FALSE;
			
			return;
		}

		if ( Ret == 5 )
		{
			pMsg.Result = CB_INCORRECT_MIX_ITEMS;
			DataSend(aIndex, (BYTE*)&pMsg, pMsg.h.size);
			CHAOS_LOG.Output("[SocketOptAddMix] Not Found Creation Gem");
			lpObj->ChaosLock = FALSE;
			
			return;
		}
		
		if ( Ret == 3 )
		{
			pMsg.Result = CB_TOO_MANY_ITEMS;
			DataSend(aIndex, (BYTE*)&pMsg, pMsg.h.size);
			CHAOS_LOG.Output("[SocketOptAddMix] Too many Items");
			lpObj->ChaosLock = FALSE;

			return;
		}
	
		if ( Ret == 4 )
		{
			pMsg.Result = CB_LACKING_MIX_ITEMS;
			DataSend(aIndex, (BYTE*)&pMsg, pMsg.h.size);
			lpObj->ChaosLock = FALSE;

			return;
		}
	
		if ( Ret == 6 )
		{
			pMsg.Result = CB_INCORRECT_MIX_ITEMS;
			DataSend(aIndex, (BYTE*)&pMsg, pMsg.h.size);
			lpObj->ChaosLock = FALSE;

			return;
		}


		if ( lpObj->Level < 10 )
		{
			pMsg.Result = CB_LOW_LEVEL_USER;
			DataSend(aIndex, (BYTE*)&pMsg, pMsg.h.size);
			lpObj->ChaosLock = FALSE;

			return;
		}
		::SocketAddChaosMix(lpObj, SpearType, SpearLvl, SpearNr, SocketSlot);
		//return;
	}

	lpObj->ChaosLock = FALSE;
}



int CheckSocketItemDel(LPOBJ lpObj, BYTE SocketSlot)
{	
	int itemcount = 0;
	BOOL FoundOtherItem = FALSE;
	BOOL bSocketItem = FALSE;

	CHAOS_LOG.Output("[SocketOptDelMix] Search Socket Item");

	for ( int n=0;n<CHAOS_BOX_SIZE;n++)
	{
		if ( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			if ( IsSlotItem(lpObj->pChaosBox[n].m_Type) )	// Socket Item
			{
				switch(SocketSlot)
				{
					case 1:
					{
						if( lpObj->pChaosBox[n].m_ItemSlot1 != 0 )
						{
							itemcount +=1;
							bSocketItem = TRUE;
						}
					}break;
					case 2:
					{
						if( lpObj->pChaosBox[n].m_ItemSlot2 != 0 )
						{
							itemcount +=1;
							bSocketItem = TRUE;
						}
					}break;
					case 3:
					{
						if( lpObj->pChaosBox[n].m_ItemSlot3 != 0 )
						{
							itemcount +=1;
							bSocketItem = TRUE;
						}
					}break;
					case 4:
					{
						if( lpObj->pChaosBox[n].m_ItemSlot4 != 0 )
						{
							itemcount +=1;
							bSocketItem = TRUE;
						}
					}break;
					case 5:
					{
						if( lpObj->pChaosBox[n].m_ItemSlot5 != 0 )
						{
							itemcount +=1;
							bSocketItem = TRUE;
						}
					}break;
				}
			}
			else
			{
				FoundOtherItem = TRUE;
			}
		}
	}

	
	
	if ( itemcount < 1 )
	{
		CHAOS_LOG.Output("[SocketOptDelMix] Try to remove free slot");
		return 3;
	}

	if ( itemcount > 1 )
	{
		CHAOS_LOG.Output("[SocketOptDelMix] Much Items Founded");
		return 2;
	}

	if ( FoundOtherItem != FALSE )
	{
		CHAOS_LOG.Output("[SocketOptDelMix] Other Item Found");
		return 0;
	}

	if(bSocketItem == TRUE)
	{
		return 1;
	}

	CHAOS_LOG.Output("[SocketOptDelMix] Socket Item Not Found");

	return 0;
}


BOOL SocketDelChaosMix(LPOBJ lpObj, BYTE SocketSlot)
{
	int aIndex = lpObj->m_Index;
	PMSG_CHAOSMIXRESULT pMsg;

	PHeadSetB((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR;
	lpObj->ChaosLock = TRUE;

	CHAOS_LOG.Output("[SocketOptDelMix] Chaos Mix Start");
	INT nChaosNeedMoney = 1000000;

	
	int iChaosTaxMoney = (int)((__int64)nChaosNeedMoney * (__int64)g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / (__int64)100);

	if ( iChaosTaxMoney < 0 )
	{
		iChaosTaxMoney = 0;
	}

	nChaosNeedMoney += iChaosTaxMoney;

	if ( nChaosNeedMoney < 0 )
	{
		nChaosNeedMoney = 0;
	}

	if ( (lpObj->Money - nChaosNeedMoney) < 0 )
	{
		CHAOS_LOG.Output("[SocketOptDelMix] [%s][%s] CBMix Not Enough Money [%d] need zen [%d]",
			lpObj->AccountID, lpObj->Name, lpObj->Money, nChaosNeedMoney);
		pMsg.Result = CB_NOT_ENOUGH_ZEN;
		DataSend(aIndex, (BYTE *)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;
		return 1;
	}

	int nr = 0;

	for ( int n=0;n<CHAOS_BOX_SIZE;n++)
	{
		if ( lpObj->pChaosBox[n].IsItem() == FALSE )
			continue;

		if(IsSlotItem(lpObj->pChaosBox[n].m_Type))
		{
			switch(SocketSlot)
			{
				case 1:
				{
					nr = n;
					lpObj->pChaosBox[n].m_ItemSlot1 = 0xFF;
					ItemByteConvert(pMsg.ItemInfo, lpObj->pChaosBox[n]);
				}break;
				case 2:
				{
					nr = n;
					lpObj->pChaosBox[n].m_ItemSlot2 = 0xFF;
					ItemByteConvert(pMsg.ItemInfo, lpObj->pChaosBox[n]);
				}break;
				case 3:
				{
					nr = n;
					lpObj->pChaosBox[n].m_ItemSlot3 = 0xFF;
					ItemByteConvert(pMsg.ItemInfo, lpObj->pChaosBox[n]);
				}break;
				case 4:
				{
					nr = n;
					lpObj->pChaosBox[n].m_ItemSlot4 = 0xFF;
					ItemByteConvert(pMsg.ItemInfo, lpObj->pChaosBox[n]);
				}break;
				case 5:
				{
					nr = n;
					lpObj->pChaosBox[n].m_ItemSlot5 = 0xFF;
					ItemByteConvert(pMsg.ItemInfo, lpObj->pChaosBox[n]);
				}break;
			}
		}else
		{
			lpObj->pChaosBox[n].Clear();
		}		
	}

	if(nr != 0)
	{
		lpObj->pChaosBox[0] = lpObj->pChaosBox[nr];
		lpObj->pChaosBox[nr].Clear();
	}

	lpObj->Money -= nChaosNeedMoney;
	g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);
	GCMoneySend(lpObj->m_Index, lpObj->Money);

	pMsg.Result = CB_SUCCESS;
	GCUserChaosBoxSend(lpObj, 0);
	DataSend(aIndex, (BYTE*)&pMsg, pMsg.h.size);

	CHAOS_LOG.Output("[SocketOptDelMix] [%s][%s] CBMix Success",lpObj->AccountID, lpObj->Name);

	::gObjInventoryCommit(lpObj->m_Index);
	lpObj->ChaosLock = FALSE;
	return TRUE;
}

void SocketItemDelChaosMix(LPOBJ lpObj, BYTE SocketSlot)
{
	lpObj->ChaosLock = TRUE;

	SocketSlot++;

	PMSG_CHAOSMIXRESULT pMsg;
	PHeadSetB((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR;
	int aIndex = lpObj->m_Index;
	int Ret = ::CheckSocketItemDel(lpObj, SocketSlot);

	if ( Ret != FALSE )
	{		
		
		if ( Ret == 3 )
		{
			pMsg.Result = CB_INCORRECT_MIX_ITEMS;
			DataSend(aIndex, (BYTE*)&pMsg, pMsg.h.size);
			CHAOS_LOG.Output("[SocketOptDelMix] Try to remove Free Slot");
			lpObj->ChaosLock = FALSE;

			return;
		}	
		
		if ( Ret == 2 )
		{
			pMsg.Result = CB_TOO_MANY_ITEMS;
			DataSend(aIndex, (BYTE*)&pMsg, pMsg.h.size);
			CHAOS_LOG.Output("[SocketOptDelMix] Too many Items");
			lpObj->ChaosLock = FALSE;

			return;
		}

		if ( lpObj->Level < 10 )
		{
			pMsg.Result = CB_LOW_LEVEL_USER;
			DataSend(aIndex, (BYTE*)&pMsg, pMsg.h.size);
			lpObj->ChaosLock = FALSE;

			return;
		}

		::SocketDelChaosMix(lpObj, SocketSlot);
		//return;
	}

	lpObj->ChaosLock = FALSE;
}


int ExtractSeedChaosBoxMix(LPOBJ lpObj, int & Result2)
{
	BOOL bSetItem = FALSE;
	BOOL bExcItem = FALSE;
	BOOL ChaosDiamond = FALSE;
	BOOL HarmonyDiamond = FALSE;
	BOOL CreationDiamond = FALSE;
	BOOL bOtherItem = FALSE;
	Result2 = 0;
	int ItemCount = 0;
	int SellValue = 0;

	if ( lpObj->pChaosBox == NULL )
	{
		return 0;
	}
	

	for ( int n=0;n<CHAOS_BOX_SIZE;n++)
	{
		if ( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			int isSetItem=(lpObj->pChaosBox[n].m_SetOption)&3;

			if ( lpObj->pChaosBox[n].m_Level >= MIN_CHAOS_ITEM_LEVEL && isSetItem > 0)
			{
				bSetItem = TRUE;
				SellValue += lpObj->pChaosBox[n].m_SellMoney;
			}

			else if ( lpObj->pChaosBox[n].m_Level >= MIN_CHAOS_ITEM_LEVEL && lpObj->pChaosBox[n].m_NewOption > 0)
			{
				bExcItem = TRUE;
				SellValue += lpObj->pChaosBox[n].m_SellMoney;
			}

			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(12,15) )
			{
				ChaosDiamond = TRUE;
			}

			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,22) )
			{
				CreationDiamond = TRUE;
			}

			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,42) )
			{
				HarmonyDiamond = TRUE;
			}
			else
			{
				bOtherItem = TRUE;
			}

			ItemCount++;
		}
	}

	if ( ItemCount < 5 )
	{
		return 2;
	}

	if ( bOtherItem != FALSE )
	{
		CHAOS_LOG.Output("[ExtractSeedMix] Other Item Found");
		return 0;
	}

	if (CreationDiamond != TRUE || ChaosDiamond != TRUE || HarmonyDiamond != TRUE)
	{	
		CHAOS_LOG.Output("[ExtractSeedMix] Need more jewels");
		return 3;	
	}

	if(bSetItem == TRUE && bExcItem == TRUE)
	{
		Result2 = SellValue/235180;
		if (Result2 > 10)
			Result2 = 10;
		return 1;
	}

	CHAOS_LOG.Output("[ExtractSeedMix] Socket Item Not Found");
	return 0;
}


void ExtractSeedChaosMix(LPOBJ lpObj)
{
	PMSG_CHAOSMIXRESULT pMsg;

	PHeadSetB((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR;
	int aIndex = lpObj->m_Index;
	BOOL fail = TRUE;
	int ExtraPercent = 0;
	lpObj->ChaosLock = TRUE;
	int ChaosMoney = 1000000;

	int ResultSearch = ExtractSeedChaosBoxMix(lpObj, ExtraPercent);

	switch(ResultSearch)
	{
		case 0:
		{
			pMsg.Result = CB_INCORRECT_MIX_ITEMS;
			DataSend(aIndex, (BYTE *)&pMsg, pMsg.h.size);
			lpObj->ChaosLock = FALSE;

			CHAOS_LOG.Output("[ExtractSeedMix][%s][%s] Incorrect Mix Items",
				lpObj->AccountID, lpObj->Name);
			return;
		}break;
		case 2:
		case 3:
		{
			pMsg.Result = CB_LACKING_MIX_ITEMS;
			DataSend(aIndex, (BYTE *)&pMsg, pMsg.h.size);
			lpObj->ChaosLock = FALSE;

			CHAOS_LOG.Output("[ExtractSeedMix][%s][%s] Lacking Mix Items",
				lpObj->AccountID, lpObj->Name);
			return;
		}break;
	}
	if ( lpObj->Money < ChaosMoney )
	{
		pMsg.Result = CB_NOT_ENOUGH_ZEN;
		
		DataSend(aIndex, (BYTE *)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;
	}
	else 
	{
		lpObj->ChaosSuccessRate = ReadConfig.ExtractSeedChaosMixBaseRate + ExtraPercent;
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

		int iRate = (rand()%100);
		if ( iRate <= lpObj->ChaosSuccessRate ) 
		{
			fail = FALSE;
			pMsg.Result = CB_SUCCESS;
			int Index = 60 + (rand()%6);
			int RandomLevel = 0;
			switch(Index)
			{
				case 60:
				{
					RandomLevel = rand()%6;
				}break;
				case 61:
				{
					RandomLevel = rand()%5;
				}break;
				case 62:
				{
					RandomLevel = rand()%5;
				}break;
				case 63:
				{
					RandomLevel = rand()%6;
				}break;
				case 64:
				{
					RandomLevel = rand()%4;
				}break;
				case 65:
				{
					RandomLevel = 0;
				}break;
			}
			int ItemNum = ITEMGET(12,Index);
			
			::ItemSerialCreateSend(lpObj->m_Index, -1, 0, 0, ItemNum, RandomLevel, 255, 0, 0, 0, -1, 0, 0);
		}else
		{
			fail = TRUE;
		}
		
		lpObj->Money -= lpObj->ChaosMoney;
		GCMoneySend(lpObj->m_Index, lpObj->Money);

		if ( fail == TRUE )
		{
			ChaosBoxItemDown(lpObj);
			GCUserChaosBoxSend(lpObj, 0);
			DataSend(aIndex, (BYTE*)&pMsg, pMsg.h.size);
			CHAOS_LOG.Output("[%s][%s] CBMix Fail %d/%d Money : %d-%d",
				lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, iRate,
				lpObj->Money, lpObj->ChaosMoney);

			MuItemShop.EarnGoblinPointsCBMix(lpObj->m_Index, lpObj->ChaosSuccessRate, 1);
		}
		else
		{
			CHAOS_LOG.Output("[%s][%s] CBMix Success %d/%d Money : %d",
				lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, iRate,
				lpObj->ChaosMoney);

			MuItemShop.EarnGoblinPointsCBMix(lpObj->m_Index, lpObj->ChaosSuccessRate, 0);
		}
	}

	lpObj->ChaosLock = FALSE;
	::gObjInventoryCommit(lpObj->m_Index);
}



int CheckIllusionTempleItem(LPOBJ lpObj, int & Level)
{
	BOOL bChaoseGem = FALSE;
	BOOL bScroll = FALSE;
	BOOL bPotion = FALSE;
	
	int PotionLevel = 0;
	int ScrollLevel = 0;
	int itemcount = 0;
	BOOL FoundOtherItem = FALSE;

	Level = 0;

	CHAOS_LOG.Output("[IllusionTemple][%s][%s] Search Illusion Temple Items",
		lpObj->AccountID, lpObj->Name);

	for ( int n=0;n<CHAOS_BOX_SIZE;n++)
	{
		if ( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			if ( lpObj->pChaosBox[n].m_Type == ITEMGET(12,15) )	// Chaos
			{
				bChaoseGem = TRUE;
			}
			else if (lpObj->pChaosBox[n].m_Type == ITEMGET(13,49)) //Scroll
			{
				itemcount +=1;
				bScroll = TRUE;
				ScrollLevel = lpObj->pChaosBox[n].m_Level;
			}
			else if (lpObj->pChaosBox[n].m_Type == ITEMGET(13,50)) // Potion
			{
				itemcount +=1;
				bPotion = TRUE;
				PotionLevel = lpObj->pChaosBox[n].m_Level;
			}
			else
			{
				FoundOtherItem = TRUE;
			}
		}
	}

	
	if ( itemcount < 2 )
	{
		CHAOS_LOG.Output("[IllusionTemple][%s][%s] Need More Items Founded",
			lpObj->AccountID, lpObj->Name);
		return 4;
	}

	if ( itemcount > 2 )
	{
		CHAOS_LOG.Output("[IllusionTemple][%s][%s] Much Items Founded",
			lpObj->AccountID, lpObj->Name);
		return 3;
	}

	if ( FoundOtherItem != FALSE )
	{
		CHAOS_LOG.Output("[IllusionTemple][%s][%s] Other Item Found",
			lpObj->AccountID, lpObj->Name);
		return 0;
	}

	if (bChaoseGem != TRUE)
	{
		CHAOS_LOG.Output("[IllusionTemple][%s][%s] No Chaos Gem Found",
			lpObj->AccountID, lpObj->Name);
		return 2;		
	}

	if (ScrollLevel != PotionLevel)
	{
		CHAOS_LOG.Output("[IllusionTemple][%s][%s] Diferent Levels on Illusion Temple Items",
			lpObj->AccountID, lpObj->Name);
		return 5;
	}

	if(bScroll == TRUE && bPotion == TRUE)
	{
		Level = PotionLevel;

		return 1;
	}

	CHAOS_LOG.Output("[IllusionTemple][%s][%s] Items Not Found",
		lpObj->AccountID, lpObj->Name);

	return 0;
}


BOOL IllusionTempleChaosMix(LPOBJ lpObj, int Level)
{
	BOOL fail = TRUE;
	int aIndex = lpObj->m_Index;
	PMSG_CHAOSMIXRESULT pMsg;

	PHeadSetB((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR;
	lpObj->ChaosLock = TRUE;

	CHAOS_LOG.Output("[IllusionTemple][%s][%s] Chaos Mix Start",
		lpObj->AccountID, lpObj->Name);
	INT nChaosNeedMoney = 2000000 * Level;

	
	int iChaosTaxMoney = (int)((__int64)nChaosNeedMoney * (__int64)g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / (__int64)100);

	if ( iChaosTaxMoney < 0 )
	{
		iChaosTaxMoney = 0;
	}

	nChaosNeedMoney += iChaosTaxMoney;

	if ( nChaosNeedMoney < 0 )
	{
		nChaosNeedMoney = 0;
	}

	if ( (lpObj->Money - nChaosNeedMoney) < 0 )
	{
		CHAOS_LOG.Output("[IllusionTemple][%s][%s] CBMix Not Enough Money [%d] need zen [%d]",
			lpObj->AccountID, lpObj->Name, lpObj->Money, nChaosNeedMoney);
		pMsg.Result = CB_NOT_ENOUGH_ZEN;
		DataSend(aIndex, (BYTE *)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;
		return 1;

	}

	int iRate = rand()%100;

	switch (Level)
	{
		case 1:
			lpObj->ChaosSuccessRate = ReadConfig.ITChaosSuccessRate1;
		break;
		case 2:
			lpObj->ChaosSuccessRate = ReadConfig.ITChaosSuccessRate2;
		break;
		case 3:
			lpObj->ChaosSuccessRate = ReadConfig.ITChaosSuccessRate3;
		break;
		case 4:
			lpObj->ChaosSuccessRate = ReadConfig.ITChaosSuccessRate4;
		break;
		case 5:
			lpObj->ChaosSuccessRate = ReadConfig.ITChaosSuccessRate5;
		break;
		case 6:
			lpObj->ChaosSuccessRate = ReadConfig.ITChaosSuccessRate6;
		break;
		default:
			lpObj->ChaosSuccessRate = ReadConfig.ITChaosSuccessRate6;
		break;
	}
		
	if ( g_CrywolfSync.GetOccupationState() == CRYWOLF_OCCUPATION_SECURE && g_iCrywolfApplyMvpBenefit )
	{
		lpObj->ChaosSuccessRate = ( lpObj->ChaosSuccessRate + g_CrywolfSync.GetPlusChaosRate() )&255;
	}

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

	if ( lpObj->ChaosSuccessRate > iRate )
	{
		ItemSerialCreateSend(lpObj->m_Index, -1, 0, 0, ITEMGET(13,51), Level, 0, 0, 0, 0, -1, 0, 0);
		fail = FALSE;
	}

	lpObj->Money -= nChaosNeedMoney;
	g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);
	GCMoneySend(lpObj->m_Index, lpObj->Money);

	if ( fail == TRUE )
	{
		ChaosBoxInit(lpObj);
		GCUserChaosBoxSend(lpObj, 0);
		DataSend(aIndex, (BYTE *)&pMsg, pMsg.h.size);

		CHAOS_LOG.Output("[IllusionTemple,%d][%s][%s] CBMix Fail %d/%d Money : %d-%d", Level,
			lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, iRate,
			lpObj->Money, nChaosNeedMoney);

		MuItemShop.EarnGoblinPointsCBMix(lpObj->m_Index, lpObj->ChaosSuccessRate, 1);
	}
	else
	{
		CHAOS_LOG.Output("[IllusionTemple,%d][%s][%s] CBMix Success %d/%d Money : %d", Level,
			lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, iRate,
			nChaosNeedMoney);

		MuItemShop.EarnGoblinPointsCBMix(lpObj->m_Index, lpObj->ChaosSuccessRate, 0);
	}

	::gObjInventoryCommit(lpObj->m_Index);
	lpObj->ChaosLock = FALSE;
	return TRUE;
}



void IllusionTempleItemMix(LPOBJ lpObj)
{
	lpObj->ChaosLock = TRUE;

	PMSG_CHAOSMIXRESULT pMsg;
	PHeadSetB((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR;
	int aIndex = lpObj->m_Index;
	int Level=0;
	int Ret = ::CheckIllusionTempleItem(lpObj, Level);

	if ( Ret != FALSE )
	{			
		if ( Ret == 2 )
		{
			pMsg.Result = CB_INCORRECT_MIX_ITEMS;
			DataSend(aIndex, (BYTE*)&pMsg, pMsg.h.size);
			CHAOS_LOG.Output("[IllusionTemple][%s][%s] Not Found Chaos Gem",
				lpObj->AccountID, lpObj->Name);
			lpObj->ChaosLock = FALSE;
			
			return;
		}

		if ( Ret == 5 )
		{
			pMsg.Result = CB_INVALID_ITEM_LEVEL;
			DataSend(aIndex, (BYTE*)&pMsg, pMsg.h.size);
			CHAOS_LOG.Output("[IllusionTemple][%s][%s] Diferent Levels on Illusion Temple Items",
				lpObj->AccountID, lpObj->Name);
			lpObj->ChaosLock = FALSE;
			
			return;
		}
		
		if ( Ret == 3 )
		{
			pMsg.Result = CB_TOO_MANY_ITEMS;
			DataSend(aIndex, (BYTE*)&pMsg, pMsg.h.size);
			CHAOS_LOG.Output("[IllusionTemple][%s][%s] Too many Illusion Temple Items",
				lpObj->AccountID, lpObj->Name);
			lpObj->ChaosLock = FALSE;

			return;
		}
	
		if ( Ret == 4 )
		{
			pMsg.Result = CB_LACKING_MIX_ITEMS;
			DataSend(aIndex, (BYTE*)&pMsg, pMsg.h.size);
			lpObj->ChaosLock = FALSE;

			return;
		}

		if ( lpObj->Level < 10 )
		{
			pMsg.Result = CB_LOW_LEVEL_USER;
			DataSend(aIndex, (BYTE*)&pMsg, pMsg.h.size);
			lpObj->ChaosLock = FALSE;

			return;
		}

		::IllusionTempleChaosMix(lpObj,Level);
		//return;
	}

	lpObj->ChaosLock = FALSE;
}


BYTE CheckNewBoxItem(LPOBJ lpObj)
{
	int itemcount1 = 0;
	int itemcount2 = 0;
	int itemcount3 = 0;
	int itemcount4 = 0;
	BOOL FoundOtherItem = FALSE;

	CHAOS_LOG.Output("[Sealed Box][%s][%s] Search Key And Box",
		lpObj->AccountID, lpObj->Name);

	for ( int n=0;n<CHAOS_BOX_SIZE;n++)
	{
		if ( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,112) )
			{
				itemcount1++;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,113) )
			{
				itemcount2++;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,121) )
			{
				itemcount3++;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,122) )
			{
				itemcount4++;
			}
			else
			{
				FoundOtherItem = TRUE;
			}
		}
	}

	if ( FoundOtherItem != FALSE )
	{
		CHAOS_LOG.Output("[Sealed Box][%s][%s] Other Items Found",
		lpObj->AccountID, lpObj->Name);
		return 0;
	}

	if(itemcount1 == 1 && itemcount4 == 1 && itemcount2 == 0 && itemcount3 == 0)
	{
		return 2;
	}

	if(itemcount1 == 0 && itemcount4 == 0 && itemcount2 == 1 && itemcount3 == 1)
	{
		return 1;
	}

	CHAOS_LOG.Output("[Sealed Box][%s][%s] Items Not Found",
		lpObj->AccountID, lpObj->Name);

	return 0;
}

BOOL NewBoxChaosMix(LPOBJ lpObj, BYTE Type)
{
	int aIndex = lpObj->m_Index;

	CHAOS_LOG.Output("[Sealed Box][%s][%s] Chaos Mix Start",
		lpObj->AccountID, lpObj->Name);

	BYTE Num = 122 + Type;

	int item = ITEMGET(14,Num);	// Imperial Guardian Ticket
	ItemSerialCreateSend(lpObj->m_Index, -1, 0, 0, item, 0, 0, 0, 0, 0, -1, 0, 0);

	CHAOS_LOG.Output("[Sealed Box][%s][%s] CBMix Success",
		lpObj->AccountID, lpObj->Name);

	::gObjInventoryCommit(lpObj->m_Index);

	return TRUE;
}


void NewBoxItemChaosMix(LPOBJ lpObj)
{
	lpObj->ChaosLock = TRUE;

	PMSG_CHAOSMIXRESULT pMsg;
	PHeadSetB((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_SUCCESS;
	int aIndex = lpObj->m_Index;
	BYTE Ret = ::CheckNewBoxItem(lpObj);

	if ( Ret != 0 )
	{
		pMsg.Result = CB_SUCCESS;
		::NewBoxChaosMix(lpObj,Ret);
	}else
	{
		pMsg.Result = CB_INCORRECT_MIX_ITEMS;
		CHAOS_LOG.Output("[Sealed Box][%s][%s] Chaos Combination fail",
		lpObj->AccountID, lpObj->Name);
	}

	DataSend(aIndex, (BYTE*)&pMsg, pMsg.h.size);
	
	lpObj->ChaosLock = FALSE;
}





BYTE CheckBoxLuckyItemRefine(LPOBJ lpObj)
{
	int itemcount1 = 0;
	BOOL FoundOtherItem = FALSE;

	CHAOS_LOG.Output("[LuckyItem Refine Box][%s][%s] Search for Lucky Items",
		lpObj->AccountID, lpObj->Name);

	for ( int n=0;n<CHAOS_BOX_SIZE;n++)
	{
		if ( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			if(IsLuckyItem(lpObj->pChaosBox[n].m_Type) == true)
			{
				itemcount1++;
			}
			else
			{
				FoundOtherItem = TRUE;
			}
		}
	}

	if ( FoundOtherItem != FALSE )
	{
		CHAOS_LOG.Output("[LuckyItem Refine Box][%s][%s] Other Items Found",
		lpObj->AccountID, lpObj->Name);
		return 0;
	}

	if(itemcount1 > 1)
	{
		CHAOS_LOG.Output("[LuckyItem Refine Box][%s][%s] Much Items Found",
		lpObj->AccountID, lpObj->Name);
		return 0;
	}

	if(itemcount1 == 1)
	{
		return 1;
	}

	CHAOS_LOG.Output("[LuckyItem Refine Box][%s][%s] Items Not Found",
		lpObj->AccountID, lpObj->Name);

	return 0;
}


BOOL BoxLuckyItemRefineChaosMix(LPOBJ lpObj)
{
	int aIndex = lpObj->m_Index;

	CHAOS_LOG.Output("[LuckyItem Refine Box][%s][%s] Chaos Mix Start",
		lpObj->AccountID, lpObj->Name);

	int item = ITEMGET(14,160);
	int iRate = rand() % 100;
	int Dur = 1;
	
	if(iRate < ReadConfig.PlusItemLevelChaosLuckyRefine255)
	{
		Dur = 255;
	}
	else
	{
		Dur = rand()%254 + 1;
	}


	ItemSerialCreateSend(lpObj->m_Index, 255, 0, 0, item, 0,
						Dur, 0, 0, 0, lpObj->m_Index, 0, 0);
	gObjInventoryCommit(lpObj->m_Index);

	CHAOS_LOG.Output("[LuckyItem Refine Box][%s][%s] CBMix Success",
		lpObj->AccountID, lpObj->Name);

	::gObjInventoryCommit(lpObj->m_Index);

	return TRUE;
}


void Box_LuckyItemsRefine(LPOBJ lpObj)
{
	lpObj->ChaosLock = TRUE;

	PMSG_CHAOSMIXRESULT pMsg;
	PHeadSetB((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_SUCCESS;
	int aIndex = lpObj->m_Index;
	BYTE Ret = ::CheckBoxLuckyItemRefine(lpObj);

	if ( Ret != 0 )
	{
		pMsg.Result = CB_SUCCESS;
		::BoxLuckyItemRefineChaosMix(lpObj);
	}else
	{
		pMsg.Result = CB_INCORRECT_MIX_ITEMS;
		CHAOS_LOG.Output("[LuckyItem Refine Box] Chaos Combination fail");
		DataSend(aIndex, (BYTE*)&pMsg, pMsg.h.size);
	}

	
	lpObj->ChaosLock = FALSE;
}







BYTE CheckLuckyCardItem(LPOBJ lpObj)
{
	int val = 0;
	int itemcount = 0;
	BOOL FoundOtherItem = FALSE;

	CHAOS_LOG.Output("[LuckyCard Box][%s][%s] Search Lucky Card",
		lpObj->AccountID, lpObj->Name
	);

	for ( int n=0;n<CHAOS_BOX_SIZE;n++)
	{
		if ( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			if ( lpObj->pChaosBox[n].m_Type >= ITEMGET(13,135) && lpObj->pChaosBox[n].m_Type <= ITEMGET(13,144 ))
			{
				val = (lpObj->pChaosBox[n].m_Type - ITEMGET(13,135)) + 1;
			}
			itemcount++;
		}
	}

	if ( itemcount > 1 )
	{
		CHAOS_LOG.Output("[LuckyCard Box][%s][%s] Other Items Found",
			lpObj->AccountID, lpObj->Name
		);
		return 0;
	}

	if(val >= 1)
	{
		CHAOS_LOG.Output("[LuckyCard Box][%s][%s] Item Found, Card Type:%d",
			lpObj->AccountID, lpObj->Name,
			val
		);

		return val;
	}

	CHAOS_LOG.Output("[LuckyCard Box][%s][%s] Items Not Found",
		lpObj->AccountID, lpObj->Name
	);

	return 0;
}

BOOL LuckyCardChaosMix(LPOBJ lpObj, BYTE Type)
{
	int aIndex = lpObj->m_Index;

	PMSG_CHAOSMIXRESULT pMsg;
	PHeadSetB((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	
	pMsg.Result = CB_ERROR;


	CHAOS_LOG.Output("[LuckyCard Box][%s][%s] Chaos Mix Start",
		lpObj->AccountID, lpObj->Name
	);

	int item = 0;
	BYTE Num = 0;

	if(Type == 8 && lpObj->Class == CLASS_MAGICGLADIATOR)
	{
		CHAOS_LOG.Output("[LuckyCard Box][%s][%s] Helmet for MG combination fail",
			lpObj->AccountID, lpObj->Name
		);
		DataSend(aIndex, (BYTE*)&pMsg, pMsg.h.size);
		return FALSE;
	}

	if(Type == 9 && lpObj->Class == CLASS_RAGEFIGHTER)
	{
		CHAOS_LOG.Output("[LuckyCard Box][%s][%s] Glovaes for RF Combination fail",
			lpObj->AccountID, lpObj->Name
		);
		DataSend(aIndex, (BYTE*)&pMsg, pMsg.h.size);
		return FALSE;
	}

	if(Type < 6)
	{
		switch(lpObj->Class)
		{
			case CLASS_WIZARD:
			{
				Num = 64;
			}break;
			case CLASS_KNIGHT:
			{
				Num = 62;
			}break;
			case CLASS_ELF:
			{
				Num = 63;
			}break;
			case CLASS_MAGICGLADIATOR:
			{
				Num = 64;
			}break;
			case CLASS_DARKLORD:
			{
				Num = 62;
			}break;
			case CLASS_SUMMONER:
			{
				Num = 65;
			}break;
			case CLASS_RAGEFIGHTER:
			{
				Num = 62;
			}break;
		};
	}else
	{
		switch(lpObj->Class)
		{
			case CLASS_WIZARD:
			{
				Num = 69;
			}break;
			case CLASS_KNIGHT:
			{
				Num = 67;
			}break;
			case CLASS_ELF:
			{
				Num = 68;
			}break;
			case CLASS_MAGICGLADIATOR:
			{
				Num = 71;
			}break;
			case CLASS_DARKLORD:
			{
				Num = 66;
			}break;
			case CLASS_SUMMONER:
			{
				Num = 70;
			}break;
			case CLASS_RAGEFIGHTER:
			{
				Num = 72;
			}break;
		};
	}

	switch(Type)
	{
		case 1:
		{
			item = ITEMGET(8,Num);
		}break;
		case 2:
		{
			item = ITEMGET(9,Num);
		}break;
		case 3:
		{
			item = ITEMGET(7,Num);
		}break;
		case 4:
		{
			item = ITEMGET(10,Num);
		}break;
		case 5:
		{
			item = ITEMGET(11,Num);
		}break;
		case 6:
		{
			item = ITEMGET(8,Num);
		}break;
		case 7:
		{
			item = ITEMGET(9,Num);
		}break;
		case 8:
		{
			item = ITEMGET(7,Num);
		}break;
		case 9:
		{
			item = ITEMGET(10,Num);
		}break;
		case 10:
		{
			item = ITEMGET(11,Num);
		}break;
	};

	if(item == 0 || Num == 0)
	{		
		CHAOS_LOG.Output("[LuckyCard Box][%s][%s] Something went wrong %d/%d",
			lpObj->AccountID, lpObj->Name,
			item, Num
		);

		DataSend(aIndex, (BYTE*)&pMsg, pMsg.h.size);
		return FALSE;
	}

	BYTE Level = 0;
	BYTE Opt = 0;
	BYTE Luck = rand()%100;
	int Dif = ReadConfig.ChaosLuckyItemMaxLevel - ReadConfig.ChaosLuckyItemMinLevel;
	if(ReadConfig.ChaosLuckyItemMaxOption > 0)
	{
		Opt = rand()%(ReadConfig.ChaosLuckyItemMaxOption+1);
	}
	if(Dif > 0)
	{
		Level = rand()%Dif + ReadConfig.ChaosLuckyItemMinLevel;
	}

	if(Luck < ReadConfig.ChaosLuckyItemLuckPercent)
		Luck = 1;

	ItemSerialCreateSend(lpObj->m_Index, -1, 0, 0, item, Level, 255, 0, Luck, Opt, -1, 0, gSetItemOption.GenSetOption(item));
	//lpObj->pInventory[target].m_SetOption = gSetItemOption.GenSetOption(lpObj->pInventory[target].m_Type);

	CHAOS_LOG.Output("[LuckyCard Box][%s][%s] CBMix Success %d,%d,%d,%d",
		lpObj->AccountID, lpObj->Name,
		item, Level, Luck, Opt
	);

	::gObjInventoryCommit(lpObj->m_Index);

	return TRUE;
}


void LuckyCardItemChaosMix(LPOBJ lpObj)
{
	lpObj->ChaosLock = TRUE;

	PMSG_CHAOSMIXRESULT pMsg;
	PHeadSetB((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_SUCCESS;
	int aIndex = lpObj->m_Index;
	BYTE Ret = ::CheckLuckyCardItem(lpObj);

	if ( !CheckInventoryEmptySpace(lpObj, 3, 2))
	{
		pMsg.Result = 0xF1;
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;
		CHAOS_LOG.Output("[LuckyCard Box][%s][%s] Combination stopped, no inventory space!",
			lpObj->AccountID, lpObj->Name
		);

		return;
	}

	if ( Ret != 0 )
	{
		pMsg.Result = CB_SUCCESS;
		::LuckyCardChaosMix(lpObj,Ret);
	}else
	{
		pMsg.Result = CB_INCORRECT_MIX_ITEMS;
		CHAOS_LOG.Output("[LuckyCard Box][%s][%s] Chaos Combination fail, no lucky card",
			lpObj->AccountID, lpObj->Name
		);
		DataSend(aIndex, (BYTE*)&pMsg, pMsg.h.size);
	}

	lpObj->ChaosLock = FALSE;
}