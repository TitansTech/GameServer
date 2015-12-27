#include "stdafx.h"
#include "..\common\MyWinsockBase.h"
#include "wsJoinServerCli.h"
#include "GameServer.h"
#include "GameMain.h"
#include "DBSockMng.h"
#include "SCFExDBProtocol.h"
#include "SCFExDB_Defines.h"
#include "ExServerProtocol.h"
#include "logproc.h"
#include "CustomQuest.h"
#include "QuestS5Info.h"
#include "MuItemShop.h"
#include "..\common\winutil.h"

wsJoinServerCli wsExServerCli;



void ExServerClientMsgProc(WPARAM wParam, LPARAM lParam)
{
	switch ( lParam & 0xFFFF & 0xFFFF )
	{
		case 1:
			wsExServerCli.DataRecv();
			break;

		case 2:
			wsExServerCli.FDWRITE_MsgDataSend();
			break;

		case 32:
			LogAddC(2, "Extra Server Close");
			wsExServerCli.Close();
			break;
	}
}


BOOL ExServerConnect(char* ConnectServer,int ExServerPort ,DWORD wMsg)
{
	wsExServerCli.SetProtocolCore(ExServerProtocolCore);
	int result = wsExServerCli.Connect(ConnectServer, ExServerPort, wMsg);

	if ( result == 0 )
	{
		LogAdd("Error: Cant Connect To Extra Server (%s:%d)", ConnectServer, ExServerPort);
		return FALSE;
	}

	ExServerLogin();
	LogAddC(2, "Extra Server Online");
	return TRUE;
}

//Connection

void ExServerLogin()
{

#if (WL_PROTECT==1)	
	int MyCheckVar;   
	VM_START_WITHLEVEL(13)
		CHECK_PROTECTION(MyCheckVar, 0x75118422)  	
		if (MyCheckVar != 0x75118422)
		{			
			_beginthread( Teaser__InsideTrigger, 0, NULL  );
		}
	VM_END
#endif

	SDHP_SERVERINFO pInfo;

	PHeadSetB((LPBYTE)&pInfo, 0x00, sizeof(pInfo));
	pInfo.h.headcode = 0x00;
	pInfo.Port = GameServerPort;
	pInfo.Type = 0x01;
	pInfo.ServerCode = gGameServerCode;
	strcpy(pInfo.ServerName, szServerName);

	wsExServerCli.DataSend((char*)&pInfo, pInfo.h.size);
}

void ExServerProtocolCore(BYTE protoNum, LPBYTE aRecv, int aLen)
{	
	switch ( protoNum )
	{
		case 0x00:
			ExtraServerLoginRecv((SDHP_RESULT *)aRecv);
			break;
			
		case SCFExDB_GSRecv_GetPlayerS5QuestInfo:
			DGGetPlayerS5QuestInfo((SDHP_GETPLAYERS5QUEST_SAVE *)aRecv);
			break;
			
		case SCFExDB_GSRecv_GetOfficialHelperData:
			GDGetOfficialHelperInfo((SDHP_GETPLAYEROFFICIALHELPER_SAVE *)aRecv);
			break;
			
		case SCFExDB_GSRecv_GetPlayerCustomQuestsInfo:
			GDSetPlayerCustomQuests((SDHP_GETPLAYERCUSTOMQUESTS_SAVE *)aRecv);
			break;

		case SCFExDB_GSRecv_CShopDataListRecv:
			MuItemShopListRecv((SDHP_MUSHOPLISTSAVE *) aRecv);
			break;

		case SCFExDB_GSRecv_CShopDataGiftRecv:
			MuItemShopBuyGiftRecv((SDHP_MUITEMSHOPGIFTBUY *) aRecv);
			break;

		case SCFExDB_GSRecv_CShopDataRecv:
			MuItemShopBuyRecv((SDHP_MUITEMSHOPBUYRECV *) aRecv);
			break;
	}
}

void ExtraServerLoginRecv(SDHP_RESULT * lpMsg)
{

}

void GetPlayerExServerInfo(int aIndex, BYTE Value)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	SDHP_GETPLAYEREXTRAINFO pMsg = {0};
	PHeadSetB((LPBYTE)&pMsg, Value, sizeof(pMsg));
	memcpy(pMsg.AccountID, gObj[aIndex].AccountID, MAX_ACCOUNT_LEN);
	memcpy(pMsg.Name, gObj[aIndex].Name, MAX_ACCOUNT_LEN);
	pMsg.aIndex = aIndex;
	wsExServerCli.DataSend((char*)&pMsg, pMsg.h.size);
}

void GetAccountExServerInfo(int aIndex, BYTE Value)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	SDHP_GETACCOUNTEXTRAINFO pMsg = {0};
	PHeadSetB((LPBYTE)&pMsg, Value, sizeof(pMsg));
	memcpy(pMsg.AccountID, gObj[aIndex].AccountID, MAX_ACCOUNT_LEN);
	pMsg.aIndex = aIndex;
	wsExServerCli.DataSend((char*)&pMsg, pMsg.h.size);
}


void JGSetCustomQuests(LPOBJ lpObj)
{
#if (PACK_EDITION>=2)

	if(customQuest.Enabled == 1)
	{
		if (lpObj->PlayerCustomQuestInfoModified == 1 &&
			lpObj->PlayerCQInfoReceived == 1 //&&
			//lpObj->PlayerQ5InfoReceived == 1
			)
		{
			lpObj->PlayerCustomQuestInfoModified = 0;

			SDHP_GETPLAYERCUSTOMQUESTS_SAVE pMsg = {0};
			PHeadSetW((LPBYTE)&pMsg, SCFExDB_GSSend_SendPlayerCustomQuestsInfo, sizeof(pMsg));

			pMsg.aIndex = lpObj->m_Index;
			memcpy(pMsg.Name, lpObj->Name, MAX_ACCOUNT_LEN);


			for (int i=0;i<256;i++)
			{
				pMsg.CustomQuest[i].State = lpObj->CustomQuest[i].State;
				pMsg.CustomQuest[i].Status = lpObj->CustomQuest[i].Status;
			}
		
			wsExServerCli.DataSend((char*)&pMsg, sizeof(pMsg));
		}
	}
#endif
}

void GDSetPlayerCustomQuests(SDHP_GETPLAYERCUSTOMQUESTS_SAVE * lpMsg)
{
	char szName[MAX_ACCOUNT_LEN+1]={0};

	memcpy(szName, lpMsg->Name, sizeof(szName));
	int aIndex = lpMsg->aIndex;

	if ( gObjIsConnectedGP(aIndex) == FALSE )	//if ( !gObjIsConnectedGP(aIndex, szName))
	{
		LogAddTD("error-L2 : Index %s %d [%s]", __FILE__, __LINE__, szName);
		return;
	}
	
	LPOBJ lpObj = &gObj[aIndex];

	if ( lpObj->m_Index == aIndex)
	{
#if (PACK_EDITION>=2)
		LogAddTD("[CQuest] Received info for ID:%d Account: %s Character: %s/%s", 
			aIndex, lpObj->AccountID, lpObj->Name, szName
		);
		for (int i=0;i<256;i++)
		{
			//memcpy(lpObj->CustomQuest, lpMsg->CustomQuest, 255);
			lpObj->CustomQuest[i].State = lpMsg->CustomQuest[i].State;
			lpObj->CustomQuest[i].Status = lpMsg->CustomQuest[i].Status;
		}
		
		lpObj->PlayerCQInfoReceived = 1;
		customQuest.LevelUp(lpObj);
#endif
	}
}

void GDGetOfficialHelperInfo(SDHP_GETPLAYEROFFICIALHELPER_SAVE * lpMsg)
{
	char szId[11];
	int aIndex = 0;
	//PMSG_TALKRESULT pResult = {0};

	szId[MAX_ACCOUNT_LEN]=0;
	memcpy(szId, lpMsg->AccountID, sizeof(lpMsg->AccountID));
	aIndex = lpMsg->aIndex;

	if ( !gObjIsAccontConnect(aIndex, szId))
	{
		LogAddC(2, lMsg.Get(MSGGET(1, 175)), szId, aIndex);
		return;
	}
	
	LPOBJ lpObj = &gObj[aIndex];

	if (( lpObj->m_Index == aIndex) && (!strcmp(lpMsg->Name,lpObj->Name)))
	{
		memcpy(&lpObj->OfficialHelper[0],&lpMsg->Data[0],sizeof(lpObj->OfficialHelper));
		SDHP_PLAYEROFFICIALHELPER_SEND pResult={0};
		PHeadSetW((LPBYTE)&pResult, 0xAE, sizeof(pResult));
		memcpy(&pResult.Data[0],&lpMsg->Data[0],sizeof(pResult.Data));
		DataSend(lpObj->m_Index, (LPBYTE)&pResult, sizeof(pResult));

		LogAddTD("[%s][%s] OfficialHelper: Info Received",lpObj->AccountID,lpObj->Name);
	}
}

void JGSetOfficialHelper(LPOBJ lpObj)
{
	if(ReadConfig.OfficialHelper_Activate == 1)
	{
		if (lpObj->PlayerOfficialHelperInfoModified == 1)
		{
			lpObj->PlayerOfficialHelperInfoModified = 0;

			SDHP_GETPLAYEROFFICIALHELPER_SAVE pMsg = {0};
			PHeadSetW((LPBYTE)&pMsg, SCFExDB_GSSend_SetOfficialHelperData, sizeof(pMsg));

			pMsg.aIndex = lpObj->m_Index;
			memcpy(pMsg.Name, lpObj->Name, MAX_ACCOUNT_LEN);		
			
			memcpy(&pMsg.Data[0],&lpObj->OfficialHelper[0],sizeof(pMsg.Data));

		
			wsExServerCli.DataSend((char*)&pMsg, sizeof(pMsg));
		}
	}
}


//Player S5Quest Info

void JGDeletePlayerQ5Info(char * Name, char* AccountID)
{
	SDHP_DELETEPLAYERS5QUEST_INFO pMsg = {0};
	PHeadSetB((LPBYTE)&pMsg, SCFExDB_GSSend_DeletePlayerQ5Info, sizeof(pMsg));
	memcpy(pMsg.AccountID, AccountID, MAX_ACCOUNT_LEN);
	memcpy(pMsg.Name, Name, MAX_ACCOUNT_LEN);
	wsExServerCli.DataSend((char*)&pMsg, pMsg.h.size);
}

void DGGetPlayerS5QuestInfo(SDHP_GETPLAYERS5QUEST_SAVE * lpMsg)
{
	char szId[11];
	int aIndex = 0;

	szId[MAX_ACCOUNT_LEN]=0;
	memcpy(szId, lpMsg->AccountID, sizeof(lpMsg->AccountID));
	aIndex = lpMsg->aIndex;

	if ( !gObjIsAccontConnect(aIndex, szId))
	{
		LogAddC(2, lMsg.Get(MSGGET(1, 175)), szId, aIndex);
		return;
	}
	
	LPOBJ lpObj = &gObj[aIndex];

	if (( lpObj->m_Index == aIndex) && (!strcmp(lpMsg->Name,lpObj->Name)))
	{
		lpObj->PlayerQ5InfoReceived = 1;

		//memcpy(lpObj->S5Quest, lpMsg->S5Quest, sizeof(lpMsg->S5Quest));
		for (int i=0;i<MAX_QUEST_COUNT;i++)
		{
			lpObj->S5Quest[i].Active= lpMsg->S5Quest[i].Active;
			lpObj->S5Quest[i].NumID = lpMsg->S5Quest[i].NumID;
			for(int j=0;j<MAX_QUEST_REQ;j++)
				lpObj->S5Quest[i].State[j] = lpMsg->S5Quest[i].State[j];
		}
		
		qs5.SendTWindow(lpObj->m_Index);
		LogAddTD("[%s][%s] QuestSystem: Info Received",lpObj->AccountID,lpObj->Name);
	}
}

void SetPlayerS5QuestInfo(int aIndex)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];
	SDHP_GETPLAYERS5QUEST_INFOSAVE pMsg2 = {0};
	PHeadSetW((LPBYTE)&pMsg2, SCFExDB_GSSend_SetPlayerQuestSave, sizeof(pMsg2));
	memcpy(pMsg2.AccountID, lpObj->AccountID, MAX_ACCOUNT_LEN);
	memcpy(pMsg2.Name, lpObj->Name, MAX_ACCOUNT_LEN);

	for (int i=0;i<MAX_QUEST_COUNT;i++)
	{
		pMsg2.S5Quest[i].Active= lpObj->S5Quest[i].Active;
		pMsg2.S5Quest[i].NumID = lpObj->S5Quest[i].NumID;
		for(int j=0;j<MAX_QUEST_REQ;j++)
			pMsg2.S5Quest[i].State[j] = lpObj->S5Quest[i].State[j];
	}

	wsExServerCli.DataSend((char*)&pMsg2, sizeof(pMsg2));
}

void MuItemShopBuySend(int aIndex, WORD ProductIndex, BYTE Type)
{	
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	SDHP_MUITEMSHOPBUY pMsg = {0};
	PHeadSetB((LPBYTE)&pMsg, SCFExDB_GSSend_CShopDataSend,sizeof(pMsg));
	pMsg.aIndex = aIndex;
	strcpy(pMsg.AccountID,gObj[aIndex].AccountID);

	pMsg.ItemID = MuItemShop.ProdID[ProductIndex].ItemID;
	pMsg.ID1 = MuItemShop.ProdID[ProductIndex].ID1;
	pMsg.ID2 = MuItemShop.ProdID[ProductIndex].ID2;

	wsExServerCli.DataSend((char*)&pMsg, pMsg.h.size);
}

void MuItemShopUpdateGoblinCoin(int aIndex, int GoblinCoin)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	if (GoblinCoin > 0)
	{
		SDHP_ADDGOBLINCOIN pMsg = {0};
		PHeadSetB((LPBYTE)&pMsg, SCFExDB_GSSend_CShopAddGoblinCoin,sizeof(pMsg));
		strcpy(pMsg.AccountID,gObj[aIndex].AccountID);
		pMsg.GoblinCoin = GoblinCoin;

		wsExServerCli.DataSend((char*)&pMsg, pMsg.h.size);
	}
}

void MuItemShopGiftBuySend(int aIndex, WORD ProductIndex, BYTE Type, char * Name, char * Msg)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	SDHP_MUITEMSHOPGIFTBUY pMsg = {0};
	PHeadSetB((LPBYTE)&pMsg, SCFExDB_GSSend_CShopDataGiftSend,sizeof(pMsg));
	pMsg.aIndex = aIndex;
	strcpy(pMsg.AccountID,gObj[aIndex].AccountID);

	pMsg.ItemID = MuItemShop.ProdID[ProductIndex].ItemID;
	pMsg.ID1 = MuItemShop.ProdID[ProductIndex].ID1;
	pMsg.ID2 = MuItemShop.ProdID[ProductIndex].ID2;
	strcpy(pMsg.Name,Name);
	if(strlen(Msg) < sizeof(pMsg.Msg))
		strcpy(pMsg.Msg,Msg);
	else
		strncpy(pMsg.Msg,Msg,sizeof(pMsg.Msg));

	wsExServerCli.DataSend((char*)&pMsg, pMsg.h.size);
}

void MuItemShopBuyRecv(SDHP_MUITEMSHOPBUYRECV * lpMsg)
{
	char szId[11];
	int aIndex = 0;

	szId[MAX_ACCOUNT_LEN]=0;
	memcpy(szId, lpMsg->AccountID, sizeof(lpMsg->AccountID));
	aIndex = lpMsg->aIndex;

	if ( !gObjIsAccontConnect(aIndex, szId))
	{
		LogAddC(2, lMsg.Get(MSGGET(1, 175)), szId, aIndex);
		return;
	}

	MuItemShop.AddItem(aIndex, lpMsg->m_Serial2, lpMsg->ItemID, lpMsg->ID1, lpMsg->ID2,0x53);
}

void MuItemShopBuyGiftRecv(SDHP_MUITEMSHOPGIFTBUY * lpMsg)
{
	char szId[11];
	int aIndex = 0;

	szId[MAX_ACCOUNT_LEN]=0;
	memcpy(szId, lpMsg->AccountID, sizeof(lpMsg->AccountID));
	aIndex = lpMsg->aIndex;

	if ( !gObjIsAccontConnect(aIndex, szId))
	{
		LogAddC(2, lMsg.Get(MSGGET(1, 175)), szId, aIndex);
		return;
	}

	MuItemShop.AddGiftItem(aIndex, lpMsg->m_Serial2, lpMsg->ItemID, lpMsg->ID1, lpMsg->ID2,0x47, lpMsg->Name, lpMsg->Msg);
}

void MuItemShopListRecv(SDHP_MUSHOPLISTSAVE * lpMsg)
{
	int aIndex = lpMsg->aIndex;
	char szAccountId[MAX_ACCOUNT_LEN+1];
	szAccountId[MAX_ACCOUNT_LEN]=0;

	memcpy(szAccountId, lpMsg->AccountID, MAX_ACCOUNT_LEN);
	
	if ( !gObjIsAccontConnect(aIndex, szAccountId))
	{
		LogAddC(2, "Request to receive CShop List infomation doesn't match the user %s", szAccountId);
		return;
	}
	gObj[aIndex].CShop = new MUITEMSHOP[MUITEMSHOP_MAXITEMPERPAGE*MUITEMSHOP_MAXPAGES];
	gObj[aIndex].CShopGift = new MUITEMSHOP[MUITEMSHOP_MAXITEMPERPAGE*MUITEMSHOP_MAXPAGES];
	gObj[aIndex].CShopGiftMsg = new MUITEMSHOPGIFT[MUITEMSHOP_MAXITEMPERPAGE*MUITEMSHOP_MAXPAGES];

	gObj[aIndex].GoblinCoin = lpMsg->GoblinCoin;
	gObj[aIndex].WCoin = lpMsg->WCoin;
	gObj[aIndex].WCoinP = lpMsg->WCoinP;

	memcpy(gObj[aIndex].CShop,&lpMsg->List[0],sizeof(lpMsg->List));	
	memcpy(gObj[aIndex].CShopGift,&lpMsg->GiftList[0],sizeof(lpMsg->GiftList));	
	memcpy(gObj[aIndex].CShopGiftMsg,&lpMsg->GiftMsg[0],sizeof(lpMsg->GiftMsg));	

	gObj[aIndex].CShopInfoRequested = 2;
	MuItemShop.OpenRequest(aIndex);
}

void MuItemShopListSend(int aIndex)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	if (gObj[aIndex].CShopInfoModified_Normal == 1)
	{
		gObj[aIndex].CShopInfoModified_Normal = 0;
		SDHP_MUSHOPLISTSAVE_Normal pMsg = {0};
		PHeadSetW((LPBYTE)&pMsg, SCFExDB_GSSend_CShopDataListSave_Normal,sizeof(pMsg));
		strcpy(pMsg.AccountID,gObj[aIndex].AccountID);

		memcpy(&pMsg.List[0],gObj[aIndex].CShop,sizeof(pMsg.List));		
		wsExServerCli.DataSend((char*)&pMsg, sizeof(pMsg));
	}
	if (gObj[aIndex].CShopInfoModified_Gift == 1)
	{
		gObj[aIndex].CShopInfoModified_Gift = 0;
		SDHP_MUSHOPLISTSAVE_Gift pMsg = {0};
		PHeadSetW((LPBYTE)&pMsg, SCFExDB_GSSend_CShopDataListSave_Gift,sizeof(pMsg));
		strcpy(pMsg.AccountID,gObj[aIndex].AccountID);

		memcpy(&pMsg.GiftList[0],gObj[aIndex].CShopGift,sizeof(pMsg.GiftList));	
		memcpy(&pMsg.GiftMsg[0],gObj[aIndex].CShopGiftMsg,sizeof(pMsg.GiftMsg));	
		wsExServerCli.DataSend((char*)&pMsg, sizeof(pMsg));
	}	
	if (gObj[aIndex].CShopInfoModified_Points == 1)
	{
		gObj[aIndex].CShopInfoModified_Points = 0;
		
		SDHP_MUSHOPLISTSAVE_Points pMsg = {0};
		PHeadSetB((LPBYTE)&pMsg, SCFExDB_GSSend_CShopDataListSave_Points,sizeof(pMsg));
		strcpy(pMsg.AccountID,gObj[aIndex].AccountID);

		pMsg.WCoin = gObj[aIndex].WCoin;
		pMsg.WCoinP = gObj[aIndex].WCoinP;
		pMsg.GoblinCoin = gObj[aIndex].GoblinCoin;
		wsExServerCli.DataSend((char*)&pMsg, sizeof(pMsg));
	}
}

void SendMonsterKilled(int aIndex, WORD MonsterID)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	switch(MonsterID)
	{
		case 78:
		{
			if(ReadConfig.Ranking_Monsters_GoldenGoblin == 0)
				return;
		}break;
		case 53:
		{
			if(ReadConfig.Ranking_Monsters_GoldenTitan == 0)
				return;
		}break;
		case 79:
		{
			if(ReadConfig.Ranking_Monsters_GoldenDerkon == 0)
				return;
		}break;
		case 80:
		{
			if(ReadConfig.Ranking_Monsters_GoldenLizardKing == 0)
				return;
		}break;
		case 82:
		{
			if(ReadConfig.Ranking_Monsters_GoldenTantalos == 0)
				return;
		}break;
		
		case 493:
		{
			if(ReadConfig.Ranking_Monsters_GoldenDarkKnight == 0)
				return;
		}break;
		case 494:
		{
			if(ReadConfig.Ranking_Monsters_GoldenDarkDevil == 0)
				return;
		}break;
		case 495:
		{
			if(ReadConfig.Ranking_Monsters_GoldenStoneGolem == 0)
				return;
		}break;
		case 496:
		{
			if(ReadConfig.Ranking_Monsters_GoldenCrust == 0)
				return;
		}break;
		case 497:
		{
			if(ReadConfig.Ranking_Monsters_GoldenSatyros == 0)
				return;
		}break;
		case 498:
		{
			if(ReadConfig.Ranking_Monsters_GoldenTwinTale == 0)
				return;
		}break;
		case 499:
		{
			if(ReadConfig.Ranking_Monsters_GoldenIronKnight == 0)
				return;
		}break;
		case 500:
		{
			if(ReadConfig.Ranking_Monsters_GoldenNapin == 0)
				return;
		}break;
		case 501:
		{
			if(ReadConfig.Ranking_Monsters_GoldenGreatDragon == 0)
				return;
		}break;
		default:
		{
			return;
		}break;
	}

	PMSG_ANS_MONSTERKILLED pMsg = {0};
	PHeadSetB((LPBYTE)&pMsg, SCFExDB_GSSend_MonsterKilled, sizeof(pMsg));

	memcpy(pMsg.AccountID, gObj[aIndex].AccountID, MAX_ACCOUNT_LEN);
	memcpy(pMsg.Name, gObj[aIndex].Name, MAX_ACCOUNT_LEN);
	pMsg.ServerCode = gGameServerCode;
	pMsg.Monster = MonsterID;

	wsExServerCli.DataSend((char*)&pMsg, pMsg.h.size);
}