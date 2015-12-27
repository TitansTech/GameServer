// ------------------------------
// Decompiled by Deathway
// Date : 2007-03-09
// ------------------------------
// GS-n 0.99.60T 0x0041D4D0
// GS-N	1.00.18	JNP	0x00424550	-	Completed


#include "stdafx.h"
#include "DSProtocol.h"
#include "logproc.h"
#include "DBSockMng.h"
#include "GameMain.h"
#include "GameServer.h"
//#include "GameServerAuth.h"
#include "MapServerManager.h"
#include "CastleSiegeSync.h"
#include "ChaosBox.h"
#include "QuestInfo.h"
#include "TNotice.h"
#include "SProtocol.h"
#include "EDSProtocol.h"
#include "protocol.h"
#include "user.h"
#include "giocp.h"
#include "ObjUseSkill.h"
#include "CashItemPeriodSystem.h"
#include "BloodCastle.h"
#include "..\common\winutil.h"
#include "ObjCalCharacter.h"
#include "CrywolfSync.h"
#include "SCFExDBProtocol.h"
#include "GMSystem.h"
#include "gObjMonster.h"
#include "XMasEvent.h"
#include "QuestS5Info.h"
#include "MuItemShop.h"
#include "Mastering2.h"
#include "SCFExDB_Defines.h"
#include "RProtocol.h"
#include "EProtocol.h"
#include "ExServerProtocol.h"
#include "CustomQuest.h"
#include "User.h"
#include "CrystalWall.h"
#include "ObjTitanRank.h"
#include "ObjTitanLottery.h"

#include "DSGN_ItemMover.h"

#if (GS_CASTLE==1)
#include "Crywolf.h"
#include "CastleSiege.h"
#endif

#include "LogToFile.h"
extern CLogToFile DSGN_ITEMMOVE_LOG;
extern CLogToFile DSGN_ITEMFIX_LOG;
extern CLogToFile DSGN_RINGFIX_LOG;
extern CLogToFile DSGN_WINGFIX_LOG;
extern CLogToFile CHAOS_LOG;

void DataServerProtocolCore(BYTE protoNum, unsigned char* aRecv, int aLen)
{
	#ifdef TRACE_PROTOCOL
		LogAddHeadHex("R","DATA_SERVER", aRecv, aLen);
	#endif
Again2:
	switch ( protoNum )
	{
		case 0x90:
			DataServerLoginResult((SDHP_RESULT *)aRecv);
			break;

		case 0x91:
			JGPGetCharList(aRecv);
			break;

		case 0x94:
			JGCharacterCreateRequest((SDHP_CREATECHARRESULT *)aRecv);
			break;

		case 0x95:
			JGCharDelRequest((SDHP_CHARDELETERESULT *)aRecv);
			break;

		case 0x06:
			{
				if(ReadConfig.S6E2 == 0)
					JGGetCharacterInfo((SDHP_DBCHAR_INFORESULT *)aRecv);
				else
					JGGetCharacterInfoS6E2(( SDHP_DBCHAR_INFORESULTS6E2*)aRecv);
			}break;

		case 0x08:
			{
				if(ReadConfig.S6E2 == 0)
					DGGetWarehouseList((SDHP_GETWAREHOUSEDB_SAVE *)aRecv);
				else
					DGGetWarehouseList_S6E2((SDHP_GETWAREHOUSEDB_S6E2_SAVE *)aRecv);
			}
			break;

		case 0x10:
			{
				if(ReadConfig.S6E2 == 0)
					GDGetWarehouseNoItem((SDHP_GETWAREHOUSEDB_RESULT *)aRecv);
				else
					GDGetWarehouseNoItem((SDHP_GETWAREHOUSEDB_RESULT *)aRecv);
			}
			break;

		case 0x52:
			ItemSerialCreateRecv((SDHP_ITEMCREATERECV *)aRecv);
			break;

		case 0x56:
			{
				#if (WL_PROTECT==1)
					VM_START_WITHLEVEL(7)
						if(WLRegGetStatus(NULL) != 1)
						{
							int per = rand()%100;
							if(per<40)
							{
								protoNum = rand()%80;
								goto Again2;
							}else
							{
								gCreateCharacter = 0;
								gItemDropPer = 1;
								gAddExperience = 1.0f;
							}
						}
					VM_END
				#endif
				DGRecvPetItemInfo(aRecv);
			}
			break;

		case 0x60:
			DGOptionDataRecv((SDHP_SKILLKEYDATA_SEND *)aRecv);
			break;

		case 0x80:
			{
				PMSG_DEFAULT2 * lpDef1 = (PMSG_DEFAULT2 *)aRecv;

				switch ( lpDef1->subcode )
				{
					case 0x00:
						GS_DGAnsCastleTotalInfo(aRecv);
						break;

					case 0x01:
						GS_DGAnsOwnerGuildMaster(aRecv);
						break;

					case 0x03:
						GS_DGAnsCastleNpcBuy(aRecv);
						break;

					case 0x04:
						GS_DGAnsCastleNpcRepair(aRecv);
						break;

					case 0x05:
						GS_DGAnsCastleNpcUpgrade(aRecv);
						break;

					case 0x06:
						GS_DGAnsTaxInfo(aRecv);
						break;

					case 0x07:
						GS_DGAnsTaxRateChange(aRecv);
						break;

					case 0x08:
						GS_DGAnsCastleMoneyChange(aRecv);
						break;

					case 0x09:
						GS_DGAnsSiegeDateChange(aRecv);
						break;

					case 0x0A:
						GS_DGAnsGuildMarkRegInfo(aRecv);
						break;

					case 0x0B:
						GS_DGAnsSiegeEndedChange(aRecv);
						break;

					case 0x0C:
						GS_DGAnsCastleOwnerChange(aRecv);
						break;

					case 0x0D:
						GS_DGAnsRegAttackGuild(aRecv);
						break;

					case 0x0E:
						GS_DGAnsRestartCastleState(aRecv);
						break;

					case 0x0F:
						GS_DGAnsMapSvrMsgMultiCast(aRecv);
						break;

					case 0x10:
						GS_DGAnsRegGuildMark(aRecv);
						break;

					case 0x11:
						GS_DGAnsGuildMarkReset(aRecv);
						break;

					case 0x12:
						GS_DGAnsGuildSetGiveUp(aRecv);
						break;

					case 0x16:
						GS_DGAnsNpcRemove(aRecv);
						break;

					case 0x17:
						GS_DGAnsCastleStateSync(aRecv);
						break;

					case 0x18:
						GS_DGAnsCastleTributeMoney(aRecv);
						break;

					case 0x19:
						GS_DGAnsResetCastleTaxInfo(aRecv);
						break;

					case 0x1A:
						GS_DGAnsResetSiegeGuildInfo(aRecv);
						break;

					case 0x1B:
						GS_DGAnsResetRegSiegeInfo(aRecv);
						break;
				}
			}
			break;

		case 0x81:
			GS_DGAnsCastleInitData(aRecv);
			break;

		case 0x82:
			GS_DGAnsCastleNpcInfo(aRecv);
			break;

		case 0x83:
			GS_DGAnsAllGuildMarkRegInfo(aRecv);
			break;

		case 0x84:
			GS_DGAnsFirstCreateNPC(aRecv);
			break;

		case 0x85:
			GS_DGAnsCalcREgGuildList(aRecv);
			break;

		case 0x86:
			GS_DGAnsCsGulidUnionInfo(aRecv);
			break;

		case 0x87:
			GS_DGAnsCsSaveTotalGuildInfo(aRecv);
			break;

		case 0x88:
			GS_DGAnsCsLoadTotalGuildInfo(aRecv);
			break;

		case 0x89:
			GS_DGAnsCastleNpcUpdate(aRecv);
			break;

		case 0xB0:
			DGAnsCrywolfSync(aRecv);
			break;

		case 0xB1:
			DGAnsCrywolfInfoLoad(aRecv);
			break;

		case 0xB2:
			DGAnsCrywolfInfoSave(aRecv);
			break;

		case 0xCF:
			DGMoveOtherServer((SDHP_CHARACTER_TRANSFER_RESULT *)aRecv);
			break;

		case 0xD0:
			{
				PMSG_DEFAULT2 * lpMsg = (PMSG_DEFAULT2 *)aRecv;

				switch ( lpMsg->subcode )
				{
					case 0x02:
						g_CashItemPeriodSystem.DGAnsPeriodItemInquiry((PMSG_ANS_PERIODITEM_INQUIRY *)aRecv);
						break;

					case 0x04:
						g_CashItemPeriodSystem.DGAnsPeriodItemInsert((PMSG_ANS_PERIODITEM_INSERT *)aRecv);
						break;

					case 0x06:
						g_CashItemPeriodSystem.DGAnsPeriodItemList((PMSG_ANS_PERIODITEM_LIST *)aRecv);
						break;

					case 0x08:
						g_CashItemPeriodSystem.DGAnsPeriodItemUpdate((PMSG_ANS_PERIODITEM_UPDATE *)aRecv);
						break;
				}
			}
			break;

		case 0xFF:	// Test Packet
			{
				PMSG_TEST * pMsg = (PMSG_TEST *)aRecv;

				LogAddC(2, "error-L2DataServer RECV : %d (%d)", aLen, pMsg->count);
			}
			break;

		//SCFEXDB EXTRA:		
		case SCFExDB_GSRecv_RProtocol:
			{
				switch(aRecv[3])
				{
					case 0x0B:
					{
						BC_EGAnsBloodCastleEnterCount((PMSG_ANS_BLOODCASTLE_ENTERCOUNT_SCFEXDB *)aRecv);
					}break;
				}
			}break;
		case SCFExDB_GSRecv_EProtocol:
			{
				EProtocolCore(aRecv[2],aRecv);
			}break;

		case SCFExDB_GSRecv_SetPlayerMasterSkill:
			GJSetPlayerMasterSkillInfo((SDHP_GETPLAYERMASTERSKILL_SAVE *) aRecv);
			break;

		case SCFExDB_GSRecv_GetAccountExtraInfo:
			DGGetAccountExtraInfo((SDHP_GETACCOUNTEXTRAINFO_SAVE *)aRecv);
			break;

		case SCFExDB_GSRecv_GetPlayerExtraInfo:
			DGGetPlayerExtraInfo((SDHP_GETPLAYEREXTRAINFO_SAVE *)aRecv);
			break;

		case SCFExDB_GSRecv_GetGuildGensInfo:
			DGGetGuildGensInfo((SDHP_GENSGUILD_INFO_GET *)aRecv);
			break;

#if (PACK_EDITION>=3)
		case SCFExDB_GSRecv_CreateResultPetBot:
			GDCreateResultPetBot((SDHP_PETBOTCREATERESULT *)aRecv);
			break;
#endif

#if (PACK_EDITION>=2)
		case SCFExDB_GSRecv_GetWarehouseList_2:
			DGGetWarehouseList((SDHP_GETWAREHOUSEDB_SAVE *)aRecv);
			break;

		case SCFExDB_GSRecv_GetWarehouseNoItemEx_2:
			GDGetWarehouseNoItemEx((SDHP_GETWAREHOUSEDB_RESULT *)aRecv);
			break;

		case SCFExDB_GSRecv_GetWarehouseList_1:
			DGGetWarehouseList((SDHP_GETWAREHOUSEDB_SAVE *)aRecv);
			break;

		case SCFExDB_GSRecv_GetWarehouseNoItemEx_1:
			GDGetWarehouseNoItemEx((SDHP_GETWAREHOUSEDB_RESULT *)aRecv);
			break;

		case SCFExDB_GSRecv_GetGuildWarehouseList://GUILD
		case SCFExDB_GSRecv_GetWarehouseListS6E2:
			DGGetWarehouseList_S6E2((SDHP_GETWAREHOUSEDB_S6E2_SAVE *)aRecv);
			break;
#endif
#if (PACK_EDITION>=3)
		case SCFExDB_GSRecv_GetPetBot:
			GDGetPetBot((SDHP_GETPETBOT *)aRecv);
			break;
#endif

		case SCFExDB_GSRecv_GetCleanLuckyCoinsInfo:
			DGGetCleanLuckyCoinsInfo((PMSG_DEFRESULT *)aRecv);
			break;

#if (PACK_EDITION>=3)
		case SCFExDB_GSRecv_GetBotReward:
			GetTitanReward((SDHP_GETREWARD_INFOSAVE *)aRecv);
			break;
#endif

#if (PACK_EDITION>=3)
		case SCFExDB_GSRecv_GetTitanRankInfo:
			ObjTitanRank.ShowInfo(aRecv);
			break;
		case SCFExDB_GSRecv_GetTitanLotteryInfo:
			ObjTitanLottery.ShowInfo((SDHP_RECVLOTTERYINFO *)aRecv);
			break;
#endif

#if (GS_CASTLE==1)
		case SCFExDB_GSRecv_SiegeProtocol://SIEGE THING
			{
				switch(aRecv[3])
				{
					case 0x00:
					{
						GS_DGGetHuntZoneValue((SDHP_SETHUNTZONEOPEN *) aRecv);
					}break;
				}
			}break;
#endif

	}
}








void TestDSSend()
{
	PMSG_TEST pMsg;
	int size;

	size = sizeof(PMSG_TEST);
	pMsg.h.c=0xC2;
	pMsg.h.headcode =0xFF;
	pMsg.h.sizeH = SET_NUMBERH(size);
	pMsg.h.sizeL = SET_NUMBERL(size);

	wsprintf(pMsg.testbuf, "%d", iCount);
	LogAdd("%s", pMsg.testbuf );
	cDBSMng.Send((char *)&pMsg, size);
	iCount++;

	wsprintf(pMsg.testbuf, "%d", iCount);
	LogAdd("%s", pMsg.testbuf);
	cDBSMng.Send((char *)&pMsg, size);
	iCount++;

	wsprintf(pMsg.testbuf, "%d", iCount);
	LogAdd("%s", pMsg.testbuf);
	cDBSMng.Send((char *)&pMsg, size);
	iCount++;

	wsprintf(pMsg.testbuf, "%d", iCount);
	LogAdd("%s", pMsg.testbuf);
	cDBSMng.Send((char *)&pMsg, size);
	iCount++;

	wsprintf(pMsg.testbuf, "%d", iCount);
	LogAdd("%s", pMsg.testbuf);
	cDBSMng.Send((char *)&pMsg, size);
	iCount++;

	wsprintf(pMsg.testbuf, "%d", iCount);
	LogAdd("%s", pMsg.testbuf);
	cDBSMng.Send((char *)&pMsg, size);
	iCount++;

	wsprintf(pMsg.testbuf, "%d", iCount);
	LogAdd("%s", pMsg.testbuf);
	cDBSMng.Send((char *)&pMsg, size);
	iCount++;

	wsprintf(pMsg.testbuf, "%d", iCount);
	LogAdd("%s", pMsg.testbuf);
	cDBSMng.Send((char *)&pMsg, size);
	iCount++;

	wsprintf(pMsg.testbuf, "%d", iCount);
	LogAdd("%s", pMsg.testbuf);
	cDBSMng.Send((char *)&pMsg, size);
	iCount++;

	wsprintf(pMsg.testbuf, "%d", iCount);
	LogAdd("%s", pMsg.testbuf);
	cDBSMng.Send((char *)&pMsg, size);
	iCount++;

	wsprintf(pMsg.testbuf, "%d", iCount);
	LogAdd("%s", pMsg.testbuf);
	cDBSMng.Send((char *)&pMsg, size);
	iCount++;

	wsprintf(pMsg.testbuf, "%d", iCount);
	LogAdd("%s", pMsg.testbuf);
	cDBSMng.Send((char *)&pMsg, size);
	iCount++;

	wsprintf(pMsg.testbuf, "%d", iCount);
	LogAdd("%s", pMsg.testbuf);
	cDBSMng.Send((char *)&pMsg, size);
	iCount++;

	wsprintf(pMsg.testbuf, "%d", iCount);
	LogAdd("%s", pMsg.testbuf);
	cDBSMng.Send((char *)&pMsg, size);
	iCount++;

	wsprintf(pMsg.testbuf, "%d", iCount);
	LogAdd("%s", pMsg.testbuf);
	cDBSMng.Send((char *)&pMsg, size);
	iCount++;

	wsprintf(pMsg.testbuf, "%d", iCount);
	LogAdd("%s", pMsg.testbuf);
	cDBSMng.Send((char *)&pMsg, size);
	iCount++;

	wsprintf(pMsg.testbuf, "%d", iCount);
	LogAdd("%s", pMsg.testbuf);
	cDBSMng.Send((char *)&pMsg, size);
	iCount++;

	wsprintf(pMsg.testbuf, "%d", iCount);
	LogAdd("%s", pMsg.testbuf);
	cDBSMng.Send((char *)&pMsg, size);
	iCount++;

	wsprintf(pMsg.testbuf, "%d", iCount);
	LogAdd("%s", pMsg.testbuf);
	cDBSMng.Send((char *)&pMsg, size);
	iCount++;

	wsprintf(pMsg.testbuf, "%d", iCount);
	LogAdd("%s", pMsg.testbuf);
	cDBSMng.Send((char *)&pMsg, size);
	iCount++;

	wsprintf(pMsg.testbuf, "%d", iCount);
	LogAdd("%s", pMsg.testbuf);
	cDBSMng.Send((char *)&pMsg, size);
	iCount++;

	wsprintf(pMsg.testbuf, "%d", iCount);
	LogAdd("%s", pMsg.testbuf);
	cDBSMng.Send((char *)&pMsg, size);
	iCount++;

	wsprintf(pMsg.testbuf, "%d", iCount);
	LogAdd("%s", pMsg.testbuf);
	cDBSMng.Send((char *)&pMsg, size);
	iCount++;

	LogAdd("Test Data Send");
}





void DataServerLogin(int server)
{
	SDHP_SERVERINFO pInfo;

	pInfo.h.c =0xC1;
	pInfo.h.size = sizeof(SDHP_SERVERINFO);	// Size
	pInfo.h.headcode =0x90;
	pInfo.Port =(WORD)GameServerPort;
	pInfo.Type =1;
	pInfo.ServerCode =gGameServerCode;
	strcpy( (char*)&pInfo.ServerName[0], (const char*)&szServerName[0] );

	cDBSMng.Send(server, (char*)&pInfo , pInfo.h.size  );	
}






void DataServerLoginResult(SDHP_RESULT * lpMsg)
{
	if (lpMsg->Result == false )
	{
		MsgBox(lMsg.Get(MSGGET(1, 210)));	// Authentication error from authentication server
		return;
	}
	
	DataServerConnected = TRUE;
	gServerReady++;
	SendMessage(ghWnd, WM_START_SERVER, 0, 0); // Review the Message WM_USER +5
	
}


/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Char List Count Packet
 *	Direction : GameServer -> Client
 *  Code     : 0xC1
 *	HeadCode : 0xF3
 *	SubCode  : 0x00
 *	BASE PACKET - Complete with PMSG_CHARLIST
 */
struct PMSG_CHARLISTCOUNT
{
	PBMSG_HEAD h;	// C1:F3:00
	BYTE subcode;	// 3
	BYTE MaxClass;	// 4
	BYTE MoveCnt;	// 5
	BYTE Count;	// 6
	//BYTE BlueByte;	// 6
};

struct PMSG_CHARLISTCOUNTS5E2
{
	PBMSG_HEAD h;	// C1:F3:00
	BYTE subcode;	// 3
	BYTE MaxClass;	// 4
	BYTE MoveCnt;	// 5
	BYTE Count;	// 6
	BYTE BlueByte;	// 6
};


struct PMSG_CHARLIST
{
	BYTE Index;	// 0
	char Name[10];	// 1
	WORD Level;	// C
	BYTE CtlCode;	// E
	BYTE CharSet[CHAR_SET_SIZE];	// F
	BYTE btGuildStatus;	// 21
};


struct PMSG_SUMMONER_ENABLE_CREATION
{
    PBMSG_HEAD h; // 0
    BYTE btBuyRecv; // 4
    BYTE btResult;// 5
};

void JGPGetCharList(unsigned char * lpRecv)
{
	SDHP_CHARLISTCOUNT * lpCount = (SDHP_CHARLISTCOUNT *)lpRecv;
	SDHP_CHARLIST * lpCL;
	char szId[MAX_ACCOUNT_LEN+1];
	BYTE sendbuf[256];
	int lOfs = 0;
	int lsOfs = sizeof(SDHP_CHARLISTCOUNT);
	int aIndex = lpCount->Number;
	BYTE index;
	PMSG_CHARLISTCOUNT pCLCount;	// Packet Char List Count
	PMSG_CHARLIST pCList;
	WORD TempInventory[12];

	BOOL isAllowMG = FALSE;
	BOOL isAllowDL = FALSE;
	BOOL isAllowSU = FALSE;
	BOOL isAllowRF = FALSE;

	pCLCount.h.c = 0xC1;
	pCLCount.h.headcode = 0xF3;
	pCLCount.subcode = 0x00;
	pCLCount.Count = lpCount->Count;
	//pCLCount.BlueByte = 0;

	szId[MAX_ACCOUNT_LEN] = 0;
	memcpy(szId, lpCount->AccountId, MAX_ACCOUNT_LEN);

	if ( ::gObjIsAccontConnect(aIndex, szId) == FALSE )
	{
		LogAddC(2, lMsg.Get(MSGGET(1, 165)), gObj[aIndex].AccountID, szId);
		CloseClient(aIndex);

		return;
	}

	//DaRKav: discontinue DS involvement in character creation
	//gObj[aIndex].Magumsa = lpCount->Magumsa;
	pCLCount.MoveCnt = lpCount->MoveCnt;

	memset(sendbuf, 0, sizeof(sendbuf));

	if(ReadConfig.S5E2 == 0)
	{
		lOfs += sizeof(PMSG_CHARLISTCOUNT);
	}else
	{
		lOfs += sizeof(PMSG_CHARLISTCOUNT)+1;
	}
	

	if ( pCLCount.Count > 0 )
	{
		for ( int n=0;n<pCLCount.Count ;n++)
		{
			memset(&pCList, 0, sizeof(pCList));
			lpCL = (SDHP_CHARLIST *)&lpRecv[lsOfs];
			pCList.Index = lpCL->Index;
			pCList.Level = lpCL->Level;

			if (lpCL->Level >= ReadConfig.MadicGladiatorLvlReq)
			{
				isAllowMG = TRUE;
				gObj[aIndex].Magumsa |= 0x01;
			}
			if (lpCL->Level >= ReadConfig.DarkLordLvlReq)
			{
				isAllowDL = TRUE;
				gObj[aIndex].Magumsa |= 0x10;
			}
			if (lpCL->Level >= ReadConfig.SummonerLvlReq)
			{
				isAllowSU = TRUE;
				gObj[aIndex].AllowSummoner = 1;
			}
			if (lpCL->Level >= ReadConfig.RageFighterLvlReq)
			{
				isAllowRF = TRUE;
				gObj[aIndex].AllowRageFighter = 1;
			}

			pCList.CtlCode = lpCL->CtlCode;
			pCList.btGuildStatus = lpCL->btGuildStatus;
			
			if ( gObj[aIndex].m_cAccountItemBlock != 0 )
				pCList.CtlCode |= 0x10;	// Set Block Item

			memcpy(pCList.Name, lpCL->Name, MAX_ACCOUNT_LEN);
			pCList.CharSet[CS_CLASS] = CS_GET_CLASS(lpCL->Class);
			int changeup = CS_GET_CHANGEUP(lpCL->Class);
			pCList.CharSet[CS_CLASS] |= CS_SET_CHANGEUP(changeup);

			if((lpCL->Class == 50) || (lpCL->Class == 66))
			{
				lpCL->Class--;
			}			
			if(lpCL->Class == 2)
			{
				pCList.CharSet[CS_CLASS] = 0x18;
			}
			if(lpCL->Class == 18)
			{
				pCList.CharSet[CS_CLASS] = 0x38;
			}
			if(lpCL->Class == 34)
			{
				pCList.CharSet[CS_CLASS] = 0x58;
			}
			if(lpCL->Class == 49)
			{
				pCList.CharSet[CS_CLASS] = 0x78;
			}
			if(lpCL->Class == 65)
			{
				pCList.CharSet[CS_CLASS] = 0x98;
			}
			if(lpCL->Class == 80)
			{
				pCList.CharSet[CS_CLASS] = 0xA0;
			}
			if(lpCL->Class == 81)
			{
				pCList.CharSet[CS_CLASS] = 0xB0;
			}
			if(lpCL->Class == 82)
			{
				pCList.CharSet[CS_CLASS] = 0xB8;
			}
			if(lpCL->Class == DB_RAGEFIGHER)
			{
				pCList.CharSet[CS_CLASS] = PR_RAGEFIGHER;
			}
			if(lpCL->Class == DB_RAGEFIGHER_EVO)
			{
				pCList.CharSet[CS_CLASS] = PR_RAGEFIGHER_EVO;
			}


			pCList.CharSet[9] = 0;

#if (CRYSTAL_EDITION == 1)
			BYTE NewWings = 0;
#endif


			if ( lpCL->DbVersion == 0 )
			{
				if ( lpCL->dbInventory[0] != (BYTE)-1 )
				{
					WORD wItemCode = lpCL->dbInventory[0] / MAX_TYPE_ITEMS * MAX_SUBTYPE_ITEMS + lpCL->dbInventory[0] % MAX_TYPE_ITEMS;
					pCList.CharSet[1] = wItemCode;
					pCList.CharSet[12] |= DBI_GET_TYPE(wItemCode);
				}
				else
				{
					pCList.CharSet[1] = -1;
					pCList.CharSet[12] |= 0xF0;
				}

				if ( lpCL->dbInventory[2] != (BYTE)-1 )
				{
					WORD wItemCode = lpCL->dbInventory[2] / MAX_TYPE_ITEMS * MAX_SUBTYPE_ITEMS + lpCL->dbInventory[2] % MAX_TYPE_ITEMS;
					pCList.CharSet[2] = wItemCode;
					pCList.CharSet[13] |= DBI_GET_TYPE(wItemCode);
				}
				else
				{
					pCList.CharSet[2] = -1;
					pCList.CharSet[13] |= 0xF0;
				}

				if ( lpCL->dbInventory[4] == (BYTE)-1 )
				{
					index = 0xF0;
				}
				else
				{
					index = (lpCL->dbInventory[4] & 0x0F) * MAX_TYPE_ITEMS;
				}

				if ( lpCL->dbInventory[6] == (BYTE)-1 )
				{
					index |= 0x0F;
				}
				else
				{
					index |= (lpCL->dbInventory[6] & 0x0F);
				}

				pCList.CharSet[3] = index;

				if ( lpCL->dbInventory[8] == (BYTE)-1 )
				{
					index = 0xF0;
				}
				else
				{
					index = (lpCL->dbInventory[8] & 0x0F) * MAX_TYPE_ITEMS;
				}

				if ( lpCL->dbInventory[10] == (BYTE)-1 )
				{
					index |= 0x0F;
				}
				else
				{
					index |= (lpCL->dbInventory[10] & 0x0F);
				}

				pCList.CharSet[4] = index;

				if ( lpCL->dbInventory[12] == (BYTE)-1 )
				{
					index = 0xF0;
				}
				else
				{
					index = (lpCL->dbInventory[12] & 0x0F) * MAX_TYPE_ITEMS;
				}

				if ( lpCL->dbInventory[14] == (BYTE)-1 )
				{
					index |= 0x0C;
				}
				else
				{
					index |= ((lpCL->dbInventory[14] & 0x03) * 4 );
				}

				if ( lpCL->dbInventory[16] == (BYTE)-1 )
				{
					index |= 0x03;
				}
				else
				{
					index |= (lpCL->dbInventory[16] & 0x03);
				}

				pCList.CharSet[5] = index;

				if ( lpCL->dbInventory[4] == (BYTE)-1 )
					pCList.CharSet[9] = 0x80;

				if ( lpCL->dbInventory[6] == (BYTE)-1 )
					pCList.CharSet[9] |= 0x40;

				if ( lpCL->dbInventory[8] == (BYTE)-1 )
					pCList.CharSet[9] |= 0x20;

				if ( lpCL->dbInventory[10] == (BYTE)-1 )
					pCList.CharSet[9] |= 0x10;

				if ( lpCL->dbInventory[12] == (BYTE)-1 )
					pCList.CharSet[9] |= 0x08;

				int levelindex = LevelSmallConvert(DBI_GET_LEVEL(lpCL->dbInventory[1]));
				levelindex |= (int)LevelSmallConvert(DBI_GET_LEVEL(lpCL->dbInventory[3])) << 3;
				levelindex |= (int)LevelSmallConvert(DBI_GET_LEVEL(lpCL->dbInventory[5])) << 6;
				levelindex |= (int)LevelSmallConvert(DBI_GET_LEVEL(lpCL->dbInventory[7])) << 9;
				levelindex |= (int)LevelSmallConvert(DBI_GET_LEVEL(lpCL->dbInventory[9])) << 12;
				levelindex |= (int)LevelSmallConvert(DBI_GET_LEVEL(lpCL->dbInventory[11])) << 15;
				levelindex |= (int)LevelSmallConvert(DBI_GET_LEVEL(lpCL->dbInventory[13])) << 18;

				pCList.CharSet[6] = levelindex >> 16 & 0xFF;
				pCList.CharSet[7] = (levelindex >> 8) & 0xFF;
				pCList.CharSet[8] = (levelindex ) & 0xFF;
				pCList.CharSet[10] = 0;
				pCList.CharSet[11] = 0;

				if ( lpCL->dbInventory[4] == (BYTE)-1 )
					pCList.CharSet[13] |= 0x0F;

				if ( lpCL->dbInventory[8] == (BYTE)-1 )
					pCList.CharSet[14] |= 0xF0;

				if ( lpCL->dbInventory[10] == (BYTE)-1 )
					pCList.CharSet[14] |= 0x0F;

				if ( lpCL->dbInventory[12] == (BYTE)-1 )
					pCList.CharSet[15] |= 0xF0;

				if ( lpCL->dbInventory[14] == (BYTE)-1 )
					pCList.CharSet[15] |= 0x0F;

				pCList.CharSet[16] = 0;
				pCList.CharSet[17] = 0;
			}
			else if ( lpCL->DbVersion <= 2 )
			{
				if ( lpCL->dbInventory[0] != (BYTE)-1 )
				{
					TempInventory[0] = (lpCL->dbInventory[0] / 32) * MAX_SUBTYPE_ITEMS + lpCL->dbInventory[0] % 32;
				}
				else
				{
					TempInventory[0] = -1;
				}

				if ( lpCL->dbInventory[3] != (BYTE)-1 )
				{
					TempInventory[1] = (lpCL->dbInventory[3] / 32) * MAX_SUBTYPE_ITEMS + lpCL->dbInventory[3] % 32;
				}
				else
				{
					TempInventory[1] = -1;
				}

				if ( lpCL->dbInventory[6] == (BYTE)-1 && (lpCL->dbInventory[8] & 0x80) == 0x80)
				{
					TempInventory[2] = -1;
				}
				else
				{
					TempInventory[2] = (lpCL->dbInventory[6] + ((lpCL->dbInventory[8] >> 7) <<  8) ) % 32;
				}

				if ( lpCL->dbInventory[9] == (BYTE)-1 && (lpCL->dbInventory[11] & 0x80) == 0x80)
				{
					TempInventory[3] = -1;
				}
				else
				{
					TempInventory[3] = (lpCL->dbInventory[9] + ((lpCL->dbInventory[11] >> 7) <<  8) ) % 32;
				}

				if ( lpCL->dbInventory[12] == (BYTE)-1 && (lpCL->dbInventory[14] & 0x80) == 0x80)
				{
					TempInventory[4] = 0x1F;
				}
				else
				{
					TempInventory[4] = (lpCL->dbInventory[12] + ((lpCL->dbInventory[14] >> 7) <<  8) ) % 32;
				}

				if ( lpCL->dbInventory[15] == (BYTE)-1 && (lpCL->dbInventory[17] & 0x80) == 0x80)
				{
					TempInventory[5] = 0x1F;
				}
				else
				{
					TempInventory[5] = (lpCL->dbInventory[15] + ((lpCL->dbInventory[17] >> 7) <<  8) ) % 32;
				}

				if ( lpCL->dbInventory[18] == (BYTE)-1 && (lpCL->dbInventory[20] & 0x80) == 0x80)
				{
					TempInventory[6] = 0x1F;
				}
				else
				{
					TempInventory[6] = (lpCL->dbInventory[18] + ((lpCL->dbInventory[20] >> 7) <<  8) ) % 32;
				}

				if ( lpCL->dbInventory[21] == (BYTE)-1 && (lpCL->dbInventory[23] & 0x80) == 0x80)
				{
					TempInventory[7] = 0x1F;
				}
				else
				{
					TempInventory[7] = (lpCL->dbInventory[21] + ((lpCL->dbInventory[23] >> 7) <<  8) ) % 32;
				}

				if ( lpCL->dbInventory[24] == (BYTE)-1 && (lpCL->dbInventory[26] & 0x80) == 0x80)
				{
					TempInventory[8] = 0x1F;
				}
				else
				{
					TempInventory[8] = (lpCL->dbInventory[24] + ((lpCL->dbInventory[26] >> 7) <<  8) ) % 32;
				}

				if ( TempInventory[0] != (WORD)-1 )
				{
					pCList.CharSet[1] = TempInventory[0];
					pCList.CharSet[12] |= DBI_GET_TYPE(TempInventory[0]);
				}
				else
				{
					pCList.CharSet[1] = -1;
					pCList.CharSet[12] |= 0xF0;
				}

				if ( TempInventory[1] != (WORD)-1 )
				{
					pCList.CharSet[2] = TempInventory[1];
					pCList.CharSet[13] |= DBI_GET_TYPE(TempInventory[1]);
				}
				else
				{
					pCList.CharSet[2] = -1;
					pCList.CharSet[13] |= 0xF0;
				}

				pCList.CharSet[3] = TempInventory[2] % MAX_TYPE_ITEMS * MAX_TYPE_ITEMS + TempInventory[3] % MAX_TYPE_ITEMS;
				pCList.CharSet[4] = TempInventory[4] % MAX_TYPE_ITEMS * MAX_TYPE_ITEMS + TempInventory[5] % MAX_TYPE_ITEMS;
				index = TempInventory[6] % MAX_TYPE_ITEMS * MAX_TYPE_ITEMS;
				
				if (TempInventory[7]== 0x1F )
				{
					index |= 0x0C;
				}
				else
				{
					index |= (TempInventory[7] & 0x03) * 4;
				}

				if ( (TempInventory[8] & 0x04) == 0 )
				{
					if ( TempInventory[8] == 0x1F )
					{
						index |= 0x03;
					}
					else
					{
						index |= TempInventory[8] & 0x03;
					}
				}
				else
				{
					index |= 0x03;
				}

				pCList.CharSet[5] = index;
				int levelindex = 0;

				if ( TempInventory[0] != (WORD)-1 )
				{
					levelindex = LevelSmallConvert(DBI_GET_LEVEL(lpCL->dbInventory[1]));
				}

				if ( TempInventory[1] != (WORD)-1 )
				{
					levelindex |= (int)LevelSmallConvert(DBI_GET_LEVEL(lpCL->dbInventory[4]))<<3;
				}

				if ( TempInventory[2] < 0x1F )
				{	
					levelindex |= (int)LevelSmallConvert(DBI_GET_LEVEL(lpCL->dbInventory[7]))<<6;
				}

				if ( TempInventory[3] < 0x1F )
				{	
					levelindex |= (int)LevelSmallConvert(DBI_GET_LEVEL(lpCL->dbInventory[10]))<<9;
				}

				if ( TempInventory[4] < 0x1F )
				{	
					levelindex |= (int)LevelSmallConvert(DBI_GET_LEVEL(lpCL->dbInventory[13]))<<12;
				}

				if ( TempInventory[5] < 0x1F )
				{	
					levelindex |= (int)LevelSmallConvert(DBI_GET_LEVEL(lpCL->dbInventory[16]))<<15;
				}

				if ( TempInventory[6] < 0x1F )
				{	
					levelindex |= (int)LevelSmallConvert(DBI_GET_LEVEL(lpCL->dbInventory[19]))<<18;
				}

				pCList.CharSet[6] = ((int)levelindex>>16 ) & 0xFF;
				pCList.CharSet[7] = ((int)levelindex>>8 ) & 0xFF;
				pCList.CharSet[8] = levelindex & 0xFF;

				if ( TempInventory[2] > 0x0F )
					pCList.CharSet[9] = 0x80;

				if ( TempInventory[3] > 0x0F )
					pCList.CharSet[9] |= 0x40;

				if ( TempInventory[4] > 0x0F )
					pCList.CharSet[9] |= 0x20;

				if ( TempInventory[5] > 0x0F )
					pCList.CharSet[9] |= 0x10;

				if ( TempInventory[6] > 0x0F )
					pCList.CharSet[9] |= 0x8;

				if ( (TempInventory[7] >= 3 && TempInventory[7] <= 6) || TempInventory[7] == 0x1E)
				{
					pCList.CharSet[5] |= 0x0C;
					
					if ( TempInventory[7] == 0x1E )
					{
						pCList.CharSet[9] |= 0x05;
					}
					else
					{
						pCList.CharSet[9] |= (TempInventory[7]-2) & 0x07;
					}
				}

				pCList.CharSet[10] = 0;

				if ( (TempInventory[8] & 0x03) != 0 && TempInventory[8] != 0x1F )
					pCList.CharSet[10] |= 1;

				pCList.CharSet[11] = 0;

				if ( (TempInventory[8] & 0x04) != 0 && TempInventory[8] != 0x1F )
					pCList.CharSet[12] |= 1;

				if ( TempInventory[2] >= 0x1F ) pCList.CharSet[13] |= 0x0F;
				if ( TempInventory[3] >= 0x1F ) pCList.CharSet[14] |= 0xF0;
				if ( TempInventory[4] >= 0x1F ) pCList.CharSet[14] |= 0x0F;
				if ( TempInventory[5] >= 0x1F ) pCList.CharSet[15] |= 0xF0;
				if ( TempInventory[6] >= 0x1F ) pCList.CharSet[15] |= 0x0F;

				pCList.CharSet[16] = 0;
				pCList.CharSet[17] = 0;
			}
			else	// Case DB 3
			{
				if ( lpCL->dbInventory[0] == (BYTE)-1 && (lpCL->dbInventory[2]& 0x80) == 0x80 && (lpCL->dbInventory[3]& 0xF0) == 0xF0  )
				{
					TempInventory[0] = -1;
				}
				else
				{
					TempInventory[0] = (lpCL->dbInventory[0] + (lpCL->dbInventory[2]&0x80)*2)+ (lpCL->dbInventory[3]&0xF0)*32;
				}

				if ( lpCL->dbInventory[4] == (BYTE)-1 && (lpCL->dbInventory[6]& 0x80) == 0x80 && (lpCL->dbInventory[7]& 0xF0) == 0xF0  )
				{
					TempInventory[1] = -1;
				}
				else
				{
					TempInventory[1] = (lpCL->dbInventory[4] + (lpCL->dbInventory[6]&0x80)*2)+ (lpCL->dbInventory[7]&0xF0)*32;
				}

				if ( lpCL->dbInventory[8] == (BYTE)-1 && (lpCL->dbInventory[10]& 0x80) == 0x80 && (lpCL->dbInventory[11]& 0xF0) == 0xF0  )
				{
					TempInventory[2] = 0x1FF;
				}
				else
				{
					TempInventory[2] = ((lpCL->dbInventory[8] + (lpCL->dbInventory[10]&0x80)*2)+ (lpCL->dbInventory[11]&0xF0)*32)%MAX_SUBTYPE_ITEMS;
				}

				if ( lpCL->dbInventory[12] == (BYTE)-1 && (lpCL->dbInventory[14]& 0x80) == 0x80 && (lpCL->dbInventory[15]& 0xF0) == 0xF0  )
				{
					TempInventory[3] = 0x1FF;
				}
				else
				{
					TempInventory[3] = ((lpCL->dbInventory[12] + (lpCL->dbInventory[14]&0x80)*2)+ (lpCL->dbInventory[15]&0xF0)*32)%MAX_SUBTYPE_ITEMS;
				}

				if ( lpCL->dbInventory[16] == (BYTE)-1 && (lpCL->dbInventory[18]& 0x80) == 0x80 && (lpCL->dbInventory[19]& 0xF0) == 0xF0  )
				{
					TempInventory[4] = 0x1FF;
				}
				else
				{
					TempInventory[4] = ((lpCL->dbInventory[16] + (lpCL->dbInventory[18]&0x80)*2)+ (lpCL->dbInventory[19]&0xF0)*32)%MAX_SUBTYPE_ITEMS;
				}

				if ( lpCL->dbInventory[20] == (BYTE)-1 && (lpCL->dbInventory[22]& 0x80) == 0x80 && (lpCL->dbInventory[23]& 0xF0) == 0xF0  )
				{
					TempInventory[5] = 0x1FF;
				}
				else
				{
					TempInventory[5] = ((lpCL->dbInventory[20] + (lpCL->dbInventory[22]&0x80)*2)+ (lpCL->dbInventory[23]&0xF0)*32)%MAX_SUBTYPE_ITEMS;
				}

				if ( lpCL->dbInventory[24] == (BYTE)-1 && (lpCL->dbInventory[26]& 0x80) == 0x80 && (lpCL->dbInventory[27]& 0xF0) == 0xF0  )
				{
					TempInventory[6] = 0x1FF;
				}
				else
				{
					TempInventory[6] = ((lpCL->dbInventory[24] + (lpCL->dbInventory[26]&0x80)*2)+ (lpCL->dbInventory[27]&0xF0)*32)%MAX_SUBTYPE_ITEMS;
				}

				if ( lpCL->dbInventory[28] == (BYTE)-1 && (lpCL->dbInventory[30]& 0x80) == 0x80 && (lpCL->dbInventory[31]& 0xF0) == 0xF0  )
				{
					TempInventory[7] = 0x1FF;
				}
				else
				{
					TempInventory[7] = ((lpCL->dbInventory[28] + (lpCL->dbInventory[30]&0x80)*2)+ (lpCL->dbInventory[31]&0xF0)*32)%MAX_SUBTYPE_ITEMS;
				}

				if ( lpCL->dbInventory[32] == (BYTE)-1 && (lpCL->dbInventory[34]& 0x80) == 0x80 && (lpCL->dbInventory[35]& 0xF0) == 0xF0  )
				{
					TempInventory[8] = 0x1FF;
				}
				else
				{
					TempInventory[8] = ((lpCL->dbInventory[32] + (lpCL->dbInventory[34]&0x80)*2)+ (lpCL->dbInventory[35]&0xF0)*32)%MAX_SUBTYPE_ITEMS;
				}

				pCList.CharSet[12] |= DBI_GET_TYPE(TempInventory[0]);
				pCList.CharSet[1] = TempInventory[0] % 256;

				pCList.CharSet[13] |= DBI_GET_TYPE(TempInventory[1]);
				pCList.CharSet[2] = TempInventory[1] % 256;

				pCList.CharSet[13] |= (int)(TempInventory[2] & 0x1E0) >> 5;
				pCList.CharSet[9] |= (int)(TempInventory[2] & 0x10) << 3;
				pCList.CharSet[3] |= (int)(TempInventory[2] & 0x0F) << 4;

				pCList.CharSet[14] |= (int)(TempInventory[3] & 0x1E0) >> 1;
				pCList.CharSet[9] |= (int)(TempInventory[3] & 0x10) << 2;
				pCList.CharSet[3] |= (int)(TempInventory[3] & 0x0F);

				pCList.CharSet[14] |= (int)(TempInventory[4] & 0x1E0) >> 5;
				pCList.CharSet[9] |= (int)(TempInventory[4] & 0x10) << 1;
				pCList.CharSet[4] |= (int)(TempInventory[4] & 0x0F) << 4;

				pCList.CharSet[15] |= (int)(TempInventory[5] & 0x1E0) >> 1;
				pCList.CharSet[9] |= (int)(TempInventory[5] & 0x10);
				pCList.CharSet[4] |= (int)(TempInventory[5] & 0x0F);

				pCList.CharSet[15] |= (int)(TempInventory[6] & 0x1E0) >> 5;
				pCList.CharSet[9] |= (int)(TempInventory[6] & 0x10) >> 1;
				pCList.CharSet[5] |= (int)(TempInventory[6] & 0x0F) << 4;

				if(ReadConfig.S6E1 == 1)
				{
					switch(TempInventory[7])
					{
						case 0:
						case 1:
						case 4:
						case 5:
						case 6:
						case 41:
						case 49:
						{
							pCList.CharSet[5] |= 4;
							BYTE wID = TempInventory[7]+1;
							if(TempInventory[7] == 41)
								pCList.CharSet[9] |= 4;
							else if(TempInventory[7] == 49)
								pCList.CharSet[9] |= 7;
							else
								pCList.CharSet[9] |= wID;
						}break;
						case 3:
						{
							pCList.CharSet[9] |= 1;
						}break;
						case 42:
						{
							pCList.CharSet[9] |= 6;
						}break;
						case 30:
						{
							pCList.CharSet[9] |= 5;
						}break;

						case 36:
						case 37:
						case 38:
						case 39:
						case 40:
						case 43:
						case 50:
						{
							pCList.CharSet[5] |= 0x0C;

							BYTE wID = (TempInventory[7] - 36)+1;
							if(TempInventory[7] == 43)
								pCList.CharSet[9] |= 6;
							else if(TempInventory[7] == 50)
								pCList.CharSet[9] |= 7;
							else
								pCList.CharSet[9] |= wID;
						}break;
					};

	#if (CRYSTAL_EDITION == 1)
						if(TempInventory[7] >= 200 && TempInventory[7] <= 254)
						{
							for(int i=0;i<55;i++)
							{
								if ( TempInventory[7] == (i+200))
								{
									pCList.CharSet[9] |= 4;
									NewWings = ((i+1)*4);
									break;
								}
							}
						}
	#endif
				}

				index = 0;

				if ( TempInventory[7] == 0x1FF )
				{
					if(ReadConfig.S6E1 == 0)
						index |= 0x0C;
				}
				else
				{
					index |= (int)(TempInventory[7] & 0x03) << 2;
				}

				if ( (TempInventory[8] & 0x04) == 0 )
				{
					if ( TempInventory[8] == 0x1FF )
					{
						index |= 0x03;
					}
					else
					{
						index |= (TempInventory[8] ) & 0x03;
					}
				}
				else
				{
					index |= 0x03;
				}

				if(ReadConfig.S6E1 == 0)
				{
					pCList.CharSet[5] |= index;
				}else
				{
					switch(TempInventory[7])
					{
						case 49:
						{
							pCList.CharSet[5] += index;
						}break;
						case 3:
						{
							//if ( (TempInventory[8] & 0x04) == 0 )
							//{
								if ( TempInventory[8] == 0x1FF )
								{
									pCList.CharSet[5] |= 0x0B;
								}
								else
								{
									pCList.CharSet[5] |= 0x08;
									pCList.CharSet[5] +=  (TempInventory[8] ) & 0x03;
								}
							//}
						}break;

						case 0:
						case 1:
						case 4:
						case 5:
						case 6:
						{
							if ( (TempInventory[8] & 0x04) == 0 )
							{
								if ( TempInventory[8] == 0x1FF )
								{
									pCList.CharSet[5] |= 0x03;
								}
								else
								{
									pCList.CharSet[5] +=  (TempInventory[8] ) & 0x03;
								}
							}
						}break;
						default:
						{
							pCList.CharSet[5] |= index;
						}break;
					};
				}

				int levelindex = 0;

				if ( TempInventory[0] != (WORD)-1 )
				{
					levelindex = LevelSmallConvert(DBI_GET_LEVEL(lpCL->dbInventory[1]));
				}

				if ( TempInventory[1] != (WORD)-1 )
				{
					levelindex |= (int)LevelSmallConvert(DBI_GET_LEVEL(lpCL->dbInventory[5])) << 3;
				}

				if ( TempInventory[2] < 0x1FF )
				{
					levelindex |= (int)LevelSmallConvert(DBI_GET_LEVEL(lpCL->dbInventory[9])) << 6;
				}

				if ( TempInventory[3] < 0x1FF )
				{
					levelindex |= (int)LevelSmallConvert(DBI_GET_LEVEL(lpCL->dbInventory[13])) << 9;
				}

				if ( TempInventory[4] < 0x1FF )
				{
					levelindex |= (int)LevelSmallConvert(DBI_GET_LEVEL(lpCL->dbInventory[17])) << 12;
				}

				if ( TempInventory[5] < 0x1FF )
				{
					levelindex |= (int)LevelSmallConvert(DBI_GET_LEVEL(lpCL->dbInventory[21])) << 15;
				}

				if ( TempInventory[6] < 0x1FF )
				{
					levelindex |= (int)LevelSmallConvert(DBI_GET_LEVEL(lpCL->dbInventory[25])) << 18;
				}

				pCList.CharSet[6] = ((int)levelindex >> 0x10) & 0xFF;
				pCList.CharSet[7] = ((int)levelindex >> 0x08) & 0xFF;
				pCList.CharSet[8] = ((int)levelindex) & 0xFF;
	
				if(ReadConfig.S6E1 == 0)
				{
					if ( (TempInventory[7] >= 3 && TempInventory[7] <= 6) || TempInventory[7] == 0x1E  || TempInventory[7] == 0x29
	#if (CRYSTAL_EDITION == 1)
						 || (TempInventory[7] >= 200 && TempInventory[7] <= 254)
	#endif
						 )
					{
						pCList.CharSet[5] |= 0x0C;

						if ( TempInventory[7] == 0x1E )
						{
							pCList.CharSet[9] |= 0x05;
						}else if ( TempInventory[7] == 0x29 )
						{
							pCList.CharSet[9] |= 0x06;
						}

	#if (CRYSTAL_EDITION == 1)
						else if(TempInventory[7] >= 200 && TempInventory[7] <= 254)
						{
							for(int i=0;i<55;i++)
							{
								if ( TempInventory[7] == (i+200))
								{
									pCList.CharSet[9] |= 4;
									NewWings = ((i+1)*4);
									break;
								}
							}
						}
						//else if ( TempInventory[7] == 49 )
						//{
						//	pCList.CharSet[9] |= 0x04;
						//	NewWings = 4;
						//}
	#endif
						else
						{
							pCList.CharSet[9] |= (TempInventory[7] - 2) & 0x07;
						}
					}
				}

				pCList.CharSet[10] =0;

				if ( (TempInventory[8]& 0x03) != 0 && TempInventory[8] != 0x1FF )
				{
					pCList.CharSet[10] |= 0x01;
				}

				pCList.CharSet[11] = 0;

				if ( (TempInventory[8]& 0x04) != 0 && TempInventory[8] != 0x1FF )
				{
					pCList.CharSet[12] |= 0x01;
				}

				pCList.CharSet[16] = 0;
				pCList.CharSet[17] = 0;

				if ( TempInventory[8] == 0x40 && TempInventory[8] != 0x1FF )
				{
					pCList.CharSet[5] &= 0xFC;
					pCList.CharSet[16] |= 0x20;

				}else if ( TempInventory[8] == 0x41 && TempInventory[8] != 0x1FF )
				{
					pCList.CharSet[5] &= 0xFD;
					pCList.CharSet[16] |= 0x40;

				}else if ( TempInventory[8] == 0x43 && TempInventory[8] != 0x1FF )
				{
					pCList.CharSet[5] &= 0xFC;
					pCList.CharSet[16] |= 0x80;

				}else if ( TempInventory[8] == 0x50 && TempInventory[8] != 0x1FF )
				{
					pCList.CharSet[5] &= 0xFC;
					pCList.CharSet[16] |= 0xE0;

				}else if ( TempInventory[8] == 0x6A && TempInventory[8] != 0x1FF )
				{
					pCList.CharSet[5] &= 0xFC;
					pCList.CharSet[16] |= 0xA0;

				}else if ( TempInventory[8] == 0x7B && TempInventory[8] != 0x1FF )
				{
					pCList.CharSet[5] &= 0xFC;
					pCList.CharSet[16] |= 0x60;

				}else if ( TempInventory[8] == 0x25 && TempInventory[8] != 0x1FF )
				{
					pCList.CharSet[10] &= 0xFE;
					pCList.CharSet[12] &= 0xFE;
					pCList.CharSet[12] |= 0x04;
					BYTE btExcellentOption = lpCL->dbInventory[34] & 0x3F;
					if ( (btExcellentOption&1) == 1 )
					{
						pCList.CharSet[16] |= 0x01;
					}

					if ( (btExcellentOption&2) == 2 )
					{
						pCList.CharSet[16] |= 0x02;
					}
					if ( (btExcellentOption&4) == 4 )
					{
						pCList.CharSet[17] = 0x01;//GoldFenrir
					}
				}
			}

			switch(lpCL->dbInventory[28])
			{
			case 36:
				pCList.CharSet[16] |=0x04;
					break;
			case 37:
				pCList.CharSet[16] |= 0x08;
					break;
			case 38:
				pCList.CharSet[16] |= 0x0c;
					break;
			case 39:
				pCList.CharSet[16] |= 0x10;
					break;
			case 40:
				{
					pCList.CharSet[16] |= 0x14;
				}break;
			case 42:
				pCList.CharSet[16] |= 28;
					break;
			case 43:
				pCList.CharSet[16] |= 24;
					break;
			case 130:
				{
					pCList.CharSet[5] += 0x04;
					pCList.CharSet[17] |= 32;
				}break;
			case 131:
				{
					pCList.CharSet[17] |= 64;
				}break;
			case 132:
				{
					pCList.CharSet[5] += 0x0C;
					pCList.CharSet[17] |= 96;
				}break;
			case 133:
				{
					pCList.CharSet[5] += 0x08;
					pCList.CharSet[17] |= 128;
				}break;
			case 134:
				{
					pCList.CharSet[5] += 0x04;
					pCList.CharSet[17] |= 160;
				}break;
			case 135:
				{
					pCList.CharSet[17] |= 192;
				}break;
			}
			#if(CRYSTAL_EDITION == 1)
				if(NewWings > 0)
				{
					if (pCList.CharSet[17] + NewWings <= 0xFF)
						pCList.CharSet[17] += NewWings;
				}
			#endif

			memcpy(&sendbuf[lOfs], &pCList, sizeof(pCList));
			lsOfs += sizeof(SDHP_CHARLIST);
			lOfs += sizeof(pCList);
		}

	}
	pCLCount.h.size = lOfs;

	//Allow DK,DW,ELF,SUM
	pCLCount.MaxClass = 3;
	BYTE AllowClass = 0;

	if (isAllowMG)
	{
		pCLCount.MaxClass += 1;
		AllowClass += 4;
	}
	if (isAllowDL)
	{
		pCLCount.MaxClass += 1;
		AllowClass += 2;
	}
	if (isAllowSU)
	{
		pCLCount.MaxClass += 1;
		AllowClass += 1;
	}
	if (isAllowRF)
	{
		pCLCount.MaxClass += 1;
		AllowClass += 8;
	}


	if(ReadConfig.S5E2 == 0)
	{
		memcpy(sendbuf, &pCLCount, sizeof(PMSG_CHARLISTCOUNT));
	}else
	{
		memcpy(sendbuf, &pCLCount, sizeof(PMSG_CHARLISTCOUNT)+1);
	}
	DataSend(aIndex, sendbuf, lOfs);

	if(ReadConfig.IsEngProtocol == 1)
	{
		PMSG_SUMMONER_ENABLE_CREATION pMsg;
		PHeadSetB((LPBYTE)&pMsg,0xDE,sizeof(PMSG_SUMMONER_ENABLE_CREATION));
		pMsg.btBuyRecv = 0;
		pMsg.btResult = AllowClass;
		DataSend(aIndex,(UCHAR *)&pMsg,pMsg.h.size);
	}
	
	//GetAccountExServerInfo(aIndex,SCFExDB_GSSend_CShopMuItemShopGetList);
}








/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Get Char List Request Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC1
 *	HeadCode  : 0x01
 */
struct SDHP_GETCHARLIST
{
	PBMSG_HEAD h;	// C1:01
	char Id[10];	// 3
	short Number;	// E
};




void DataServerGetCharListRequest(short aIndex)
{
	SDHP_GETCHARLIST pMsg;
	char TempId[11];
	char* AccountId;

	memset(TempId, 0, sizeof(TempId));
	AccountId = gObjGetAccountId(aIndex);

	if ( AccountId == 0 )
	{
		LogAdd(lMsg.Get(MSGGET(1, 166)), __FILE__, __LINE__);
		CloseClient(aIndex);
		return;
	}

	// When Len is Less than 1
	if ( strlen(AccountId) < 1 )
	{
		// Error Message
		LogAdd(lMsg.Get(MSGGET(1, 167)), __FILE__, __LINE__);
		CloseClient(aIndex);
		return;
	}


#if(ENABLE_CSHOP == 1)
	//GDAskCShopList(aIndex,AccountId);
#endif

	strcpy((char*)TempId, AccountId);

	pMsg.h.c  = 0xC1;
	pMsg.h.size  = sizeof(SDHP_GETCHARLIST);
	pMsg.h.headcode = 0x91;
	pMsg.Number = aIndex;
	memcpy(pMsg.Id , TempId, sizeof(TempId)-1);

	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
	
}






/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Get Char List Result Packet
 *	Direction : GameServer -> Client
 *  Code     : 0xC1
 *	HeadCode : 0xF3
 *	SubCode  : 0x01
 */
struct PMSG_CHARCREATERESULT
{
	PBMSG_HEAD h;	// C1:F3:01
	BYTE subcode;	// 3
	unsigned char Result;	// 4
	unsigned char Name[10];	// 5
	BYTE pos;	// F
	WORD Level;	// 10
	BYTE Class;	// 12
	BYTE Equipment[24];	// 13
};





void JGCharacterCreateRequest(SDHP_CREATECHARRESULT* lpMsg)
{
	PMSG_CHARCREATERESULT pResult;
	char szAccountID[MAX_ACCOUNT_LEN+1];
	int aIndex = lpMsg->Number;

	szAccountID[MAX_ACCOUNT_LEN] = 0;
	memcpy(szAccountID, lpMsg->AccountId, sizeof(lpMsg->AccountId));

	if ( gObjIsAccontConnect(aIndex, szAccountID) == FALSE )
	{
		LogAddC(2, lMsg.Get(MSGGET(1, 168)), szAccountID);
		CloseClient(aIndex);
		return;
	}

	pResult.h.c = 0xC1;
	pResult.h.size = sizeof(PMSG_CHARCREATERESULT);
	pResult.h.headcode = 0xF3;
	pResult.subcode = 0x01;
	pResult.Result = lpMsg->Result;
	pResult.pos = lpMsg->Pos;
	//pResult.Class = (lpMsg->ClassSkin >> 5 << 5) & 0xE0;
	//int changeup = lpMsg->ClassSkin & 0x07; 
	//pResult.Class |= (changeup << 4) & 0x10;
	pResult.Class = gObjFixClassPacket(lpMsg->ClassSkin);
	int changeup = CS_GET_CHANGEUP(lpMsg->ClassSkin);
	pResult.Class |= CS_SET_CHANGEUP(changeup);
	pResult.Level = lpMsg->Level;
	memcpy(pResult.Name, lpMsg->Name, sizeof(pResult.Name));

	if ( pResult.Result == 1 )
	{
		memcpy(pResult.Equipment, lpMsg->Equipment, sizeof(pResult.Equipment));
	}

	DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
}



void JGCharacterCreateFailSend(int aIndex, char* id)
{
	PMSG_CHARCREATERESULT  pResult;

	pResult.h.c = 0xC1;
	pResult.h.size = sizeof (PMSG_CHARCREATERESULT);
	pResult.h.headcode = 0xF3;
	pResult.subcode = 0x01;
	pResult.Result = false;
	pResult.pos = 0;
	memcpy(pResult.Name, id, sizeof(pResult.Name));

	DataSend(aIndex, (UCHAR*)&pResult, pResult.h.size);
}








void JGCharDelRequest(SDHP_CHARDELETERESULT * lpMsg)
{
	PMSG_RESULT pResult;
	char szAccountId[MAX_ACCOUNT_LEN+1];
	int aIndex = lpMsg->Number;
	szAccountId[MAX_ACCOUNT_LEN] = 0;
	memcpy(szAccountId, lpMsg->AccountID, sizeof(lpMsg->AccountID));

	if ( ::gObjIsAccontConnect(aIndex, szAccountId) == FALSE )
	{
		LogAddC(2, lMsg.Get(MSGGET(1, 169)), szAccountId);
		CloseClient(aIndex);

		return;
	}

	pResult.h.c = 0xC1;
	pResult.h.size = sizeof(PMSG_RESULT);
	pResult.h.headcode = 0xF3;
	pResult.subcode = 0x02;
	pResult.result = lpMsg->Result;

	DataSend(lpMsg->Number, (UCHAR*)&pResult, pResult.h.size);
}





/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Get Character Info Packet
 *	Direction : GameServer -> Client
 *  Code     : 0xC3
 *	HeadCode : 0xF3
 *	SubCode  : 0x03
 */
/*struct PMSG_CHARMAPJOINRESULT
{
	PBMSG_HEAD h;	// C3:F3:03
	BYTE subcode;	// 3
	BYTE MapX;	// 4
	BYTE MapY;	// 5
	BYTE MapNumber;	// 6
	BYTE Dir;	// 7
	DWORD Exp;	// 8
	DWORD NextExp;	// C
	WORD LevelUpPoint;	// 10
	WORD Str;	// 12
	WORD Dex;	// 14
	WORD Vit;	// 16
	WORD Energy;	// 18
	WORD Life;	// 1A
	WORD MaxLife;	// 1C
	WORD Mana;	// 1E
	WORD MaxMana;	// 20
	WORD wShield;	// 22
	WORD wMaxShield;	// 24
	WORD BP;	// 26
	WORD MaxBP;	// 28
	int Money;	// 2C
	BYTE PkLevel;	// 30
	BYTE CtlCode;	// 31
	short AddPoint;	// 32
	short MaxAddPoint;	// 34
	WORD Leadership;	// 36
	WORD wMinusPoint;	// 38
	WORD wMaxMinusPoint;	// 3A
};*/


struct PMSG_CHARMAPJOINRESULT
{
	PBMSG_HEAD h;	// C3:F3:03
	BYTE subcode;	// 3
	BYTE MapX;	// 4
	BYTE MapY;	// 5
	BYTE MapNumber;	// 6
	BYTE Dir;	// 7
	DWORD unk; //s4 IcaruS FIX
	//DWORD Exp;	// 8
	BYTE Exp[4];
	DWORD unk2; //s4 IcaruS FIX
	BYTE NextExp[4];
	//DWORD NextExp;	// C
	WORD LevelUpPoint;	// 10
	WORD Str;	// 12
	WORD Dex;	// 14
	WORD Vit;	// 16
	WORD Energy;	// 18
	WORD Life;	// 1A
	WORD MaxLife;	// 1C
	WORD Mana;	// 1E
	WORD MaxMana;	// 20
	WORD wShield;	// 22
	WORD wMaxShield;	// 24
	WORD BP;	// 26
	WORD MaxBP;	// 28
	int Money;	// 2C
	BYTE PkLevel;	// 30
	BYTE CtlCode;	// 31
	short AddPoint;	// 32
	short MaxAddPoint;	// 34
	WORD Leadership;	// 36
	WORD wMinusPoint;	// 38
	WORD wMaxMinusPoint;	// 3A
	BYTE ExpandedInv;
	short sExGameServerCode; //FIX FOR VAULT
	BYTE ExpandedVault;
};

void JGGetCharacterInfoS6E2( SDHP_DBCHAR_INFORESULTS6E2 * lpMsg)
{
	PMSG_CHARMAPJOINRESULT pjMsg;

	char szAccountId[MAX_ACCOUNT_LEN+1];
	char szName[MAX_ACCOUNT_LEN+1];
	int aIndex = lpMsg->Number;
	
	szAccountId[MAX_ACCOUNT_LEN] = 0;
	memcpy(szAccountId, lpMsg->AccountID, sizeof(lpMsg->AccountID));
	memset(&pjMsg, 0, sizeof(PMSG_CHARMAPJOINRESULT));

	if ( gObjIsAccontConnect(aIndex, szAccountId) == FALSE )
	{
		LogAddC(2, lMsg.Get(MSGGET(1, 170)), szAccountId);
		CloseClient(aIndex);

		return;
	}

	szName[MAX_ACCOUNT_LEN] = 0;
	memcpy(szName, lpMsg->Name, MAX_ACCOUNT_LEN);

	if ( lpMsg->result == false )
	{
		LogAddC(2, lMsg.Get(MSGGET(1, 171)), szName, gObj[aIndex].AccountID);
		CloseClient(aIndex);

		return;
	}

	if ( (lpMsg->CtlCode&1) != 0 )
	{
		LogAddC(2, "error-L1 : Load Block Character infomation", szName, gObj[aIndex].AccountID);	// Deathway Fix HERE
		CloseClient(aIndex);

		return;
	}

	for (int i=OBJ_STARTUSERINDEX;i<OBJMAX;i++)
	{
		if ( gObj[i].Connected == PLAYER_PLAYING )
		{
			if ( gObj[i].Type == OBJ_USER )
			{
				if ( !strncmp(szName, gObj[i].Name, MAX_ACCOUNT_LEN) || !strncmp(szAccountId, gObj[i].AccountID, MAX_ACCOUNT_LEN))
				{
					LogAddTD("[Anti-HACK][JGGetCharacterInfo] Attempted Character-Copy by double logging [%s][%s]",
						szName, gObj[aIndex].AccountID);
					CloseClient(aIndex);
					return;
				}
			}
		}
	}

	if ( gObjSetCharacter( (UCHAR*)lpMsg, aIndex) == FALSE )
	{
		LogAddC(2, lMsg.Get(MSGGET(1, 172)), szName);
		CloseClient(aIndex);

		return;
	}
	LPOBJ lpObj = &gObj[aIndex];
	lpObj->ExInventory = lpMsg->ExInventory;	
	lpObj->iShield = lpObj->iMaxShield + lpObj->iAddShield;

	if(ReadConfig.IsRecoverPShop == 1)
	{
		if(lpMsg->UpdatedPShop == 0)
		{
			for ( BYTE ItemPos = INVENTORY_NORMAL_SIZE /*108*/ ; ItemPos < ReadConfig.INVENTORY_SIZE(aIndex,true)/*236*/ ; ItemPos ++ )
			{
				if ( lpObj->pInventory[ItemPos].IsItem() == TRUE)
				{
					gObjInventoryDeleteItem ( lpObj->m_Index , ItemPos );		
				}
			}
			for ( BYTE ItemPos = MAIN_INVENTORY_NORMAL_SIZE /*76*/ ; ItemPos < INVENTORY_NORMAL_SIZE /*108*/ ; ItemPos ++ )
			{
				if ( lpObj->pInventory[ItemPos].IsItem() == TRUE)
				{
					BYTE ItemInfo[MAX_ITEM_INFO];
					BOOL DurSend;
					BOOL DurTargetSend;

					ItemByteConvert(ItemInfo, lpObj->pInventory[ItemPos]);					
					gObjInventoryMoveItem(lpObj->m_Index, ItemPos,ItemPos+(MAX_EXTENDED_INV*EXTENDED_INV_SIZE), DurSend, DurTargetSend, 0, 4, ItemInfo);
				}
			}
		}
	} else {
		if(lpMsg->UpdatedPShop == 0)
		{
			for ( BYTE ItemPos = INVENTORY_NORMAL_SIZE /*108*/ ; ItemPos < ReadConfig.INVENTORY_SIZE(aIndex,true)/*236*/ ; ItemPos ++ )
			{
				if ( lpObj->pInventory[ItemPos].IsItem() == TRUE)
				{
					gObjInventoryDeleteItem ( lpObj->m_Index , ItemPos );		
				}
			}
		}
	}

	if ( lpObj->m_Index != aIndex )
	{
		LogAddC(2, lMsg.Get(MSGGET(1, 173)), __FILE__, __LINE__);
		CloseClient(aIndex);

		return;
	}

	if ( lpObj->Money < 0 )
	{
		LogAddC(2, "[ZEN MODIFY][%s][%s] %d -> 0", szName, gObj[aIndex].AccountID, lpObj->Money);
		lpObj->Money = 0;
	}

	if ( lpObj->m_bMapSvrMoveReq == false )
	{
		if ( MapNumberCheck(lpObj->MapNumber) == FALSE )
		{
			LogAdd("error-L3 : %s %d", __FILE__, __LINE__);
			CloseClient(aIndex);

			return;
		}

		short wGameServerCode = g_MapServerManager.CheckMoveMapSvr(lpObj->m_Index, lpObj->MapNumber, lpObj->m_sPrevMapSvrCode);

		if ( wGameServerCode != gGameServerCode )
		{
			if ( wGameServerCode == -1 )
			{
				LogAddC(2,"[MapServerMng] Map Server Move Fail : CheckMoveMapSvr() == -1 [%s][%s] (%d)",
					lpObj->AccountID, lpObj->Name, lpObj->m_Index);
				CloseClient(aIndex);

				return;
			}

			GJReqMapSvrMove(lpObj->m_Index, wGameServerCode, lpObj->MapNumber, lpObj->X, lpObj->Y);
			LogAddTD("[MapServerMng] Request to Move Map Server : (%d) - [%s][%s] (%d)",
				wGameServerCode, lpObj->AccountID, lpObj->Name, lpObj->m_Index);

			return;
		}
	}

	lpObj->m_bMapSvrMoveReq = false;

	pjMsg.h.c = 0xC3;
	pjMsg.h.headcode = 0xF3;
	pjMsg.h.size = sizeof(PMSG_CHARMAPJOINRESULT);//0x49;//sizeof(PMSG_CHARMAPJOINRESULT);
	pjMsg.subcode = 0x03;
	pjMsg.MapX = lpObj->X;
	pjMsg.MapY = lpObj->Y;
	pjMsg.MapNumber = lpObj->MapNumber;
	pjMsg.Dir = lpObj->Dir;
	//pjMsg.Exp = lpObj->Experience;

	WORD Tmp1 = HIWORD(lpObj->Experience);
	WORD Tmp2 = LOWORD(lpObj->Experience);
	pjMsg.Exp[0] = HIBYTE(Tmp1);
	pjMsg.Exp[1] = LOBYTE(Tmp1);
	pjMsg.Exp[2] = HIBYTE(Tmp2);
	pjMsg.Exp[3] = LOBYTE(Tmp2);

	pjMsg.unk = 0;
	pjMsg.unk2 = 0;

	Tmp1 = HIWORD(lpObj->NextExp);
	Tmp2 = LOWORD(lpObj->NextExp);	
	pjMsg.NextExp[0] = HIBYTE(Tmp1);
	pjMsg.NextExp[1] = LOBYTE(Tmp1);
	pjMsg.NextExp[2] = HIBYTE(Tmp2);
	pjMsg.NextExp[3] = LOBYTE(Tmp2);

	//pjMsg.NextExp = lpObj->NextExp;
	pjMsg.LevelUpPoint = lpObj->LevelUpPoint;
	pjMsg.Str = lpObj->Strength;
	pjMsg.Dex = lpObj->Dexterity;
	pjMsg.Vit = lpObj->Vitality;
	pjMsg.Energy = lpObj->Energy;
	pjMsg.Money = lpObj->Money;
	pjMsg.PkLevel = lpObj->m_PK_Level;
	pjMsg.Life = lpObj->Life;
	pjMsg.MaxLife = lpObj->AddLife + lpObj->MaxLife;
	pjMsg.Mana = lpObj->Mana;
	pjMsg.MaxMana = lpObj->AddMana + lpObj->MaxMana;
	pjMsg.wShield = lpObj->iShield;
	pjMsg.wMaxShield = lpObj->iMaxShield + lpObj->iAddShield;
	pjMsg.CtlCode = lpMsg->CtlCode;
	pjMsg.BP = lpObj->BP;
	pjMsg.MaxBP = lpObj->MaxBP + lpObj->AddBP;
	pjMsg.Leadership = lpObj->Leadership;

	short AddPoint = 0;
	short MaxAddPoint = 0;
	short MinusPoint = 0;
	short MaxMinusPoint = 0;
	
	gObjGetStatPointState(lpObj->m_Index, AddPoint, MaxAddPoint, MinusPoint, MaxMinusPoint);
	pjMsg.AddPoint = AddPoint;
	pjMsg.MaxAddPoint = MaxAddPoint;
	pjMsg.wMinusPoint = MinusPoint;
	pjMsg.wMaxMinusPoint = MaxMinusPoint;

	LogAddTD("[FRUIT System] [%s][%s] (MinusPoint:%d/PlusPoint:%d) (MaxMinus:%d/MaxPlus:%d)",
		lpObj->AccountID, lpObj->Name, MinusPoint, AddPoint, MaxMinusPoint, MaxAddPoint);

	if ( AddPoint < 0 || AddPoint > MaxAddPoint || MinusPoint < 0 || MinusPoint > MaxMinusPoint )
	{
		LogAddTD("[FRUIT System] Character Stat Error [%s][%s] (MinusPoint:%d/PlusPoint:%d) (MaxMinus:%d/MaxPlus:%d)",
			lpObj->AccountID, lpObj->Name, MinusPoint, AddPoint, MaxMinusPoint, MaxAddPoint);
	}

	g_QuestInfo.QuestInfoSave(lpObj);
	//BYTE buff[0x48] = {0xC3,0x48,0xF3,0x3,0xBF,0x3C,0x8,0x0,0x0,0x0,0x0,0x0,0x17,0x63,0xDC,0x71,0x0,0x0,0x0,0x0,0x17,0xAD,0x72,0xDC,0x0,0x0,0xEE,0x3,0x10,0x2,0x32,0x0,0x64,0x0,0x11,0x3,0x11,0x3,0x4,0x1,0x4,0x1,0x8,0x14,0x8,0x14,0xB9,0x0,0x73,0x1,0x15,0x0,0x35,0xA3,0x8C,0x0,0x3,0x0,0x0,0x0,0x55,0x0,0x0,0x0,0x0,0x0,0x55,0x0,0x2,0x0,0x0,0x0};

	//DataSend(aIndex, buff, sizeof(buff));
	pjMsg.ExpandedInv = lpObj->ExInventory;
	pjMsg.ExpandedVault = lpObj->ExWarehouse;
	DataSend(aIndex, (UCHAR*)&pjMsg, pjMsg.h.size);

	GCItemListSend(aIndex);
	GCMagicListMultiSend(lpObj, 0);
	CGRequestQuestInfo(aIndex);
	DGGuildMemberInfoRequest(aIndex);
	FriendListRequest(aIndex);

	if ( gTamaJJangEvent != FALSE )
	{
		if ( gAppearTamaJJang != 0 )
		{
			if ( lpObj->MapNumber == MAP_INDEX_LORENCIA )
			{
				GCMapEventStateSend(MAP_INDEX_LORENCIA, gAppearTamaJJang, 2);
			}
		}
	}

	gObjUseSkill.SkillChangeUse(lpObj->m_Index);
	
	//IcaruS Life,Mana,BP,Shield in 0 FIX:
	GCReFillSend(lpObj->m_Index, pjMsg.MaxLife, 0xFE, TRUE, pjMsg.wMaxShield);
	GCManaSend(lpObj->m_Index,pjMsg.MaxMana,0xFE,0,pjMsg.MaxBP);
	//PlayerLogin:
	GMSystem.CheckStatus(lpObj);
	GCServerMsgStringSend(ReadConfig.ConnectNotice,lpObj->m_Index, 0x00);

	//XMas Event Notice
#if (PACK_EDITION>=2)
	if (XMasEvent.Enabled == 1)
	{
		char sBuf[256]={0};
		wsprintf(sBuf, lMsg.Get(MSGGET(14, 154)),XMasEvent.ExtraExperience,XMasEvent.ExtraDropPercent);
		GCServerMsgStringSend(sBuf,lpObj->m_Index, 0x00);
	}
#endif

#if (ENABLE_CSHOP == 1)
	MuItemShop.SendShopValue(aIndex);
#endif

	//Season 5 Quest (FIX T BUG)

#if (WL_PROTECT==1)  
		VM_START_WITHLEVEL(3)
		if(WLRegGetStatus(NULL) != 1)
		{
		}else
		{
			BYTE buff2[9] = { 0xc1,0x09,0xf6,0x1a,0x01,0x00,0x00,0x00,0x00};
			DataSend(lpObj->m_Index, buff2, 0x09);
		}
		VM_END
#else

	//BYTE buff2[9] = { 0xc1,0x09,0xf6,0x1a,0x01,0x00,0x00,0x00,0x00};
	//DataSend(lpObj->m_Index, buff2, 0x09);
//#if (PACK_EDITION>=3)
	qs5.SendTWindow(lpObj->m_Index);
//#else
//	BYTE buff2[9] = { 0xc1,0x09,0xf6,0x1a,0x01,0x00,0x00,0x00,0x00};
//	DataSend(lpObj->m_Index, buff2, 0x09);
//#endif
	
#endif

	GetPlayerExServerInfo(aIndex,SCFExDB_GSSend_GetPlayerOfficialHelperInfo);
	GetPlayerExServerInfo(aIndex,SCFExDB_GSSend_GetPlayerQ5Info);

#if (PACK_EDITION>=2)
	if(customQuest.Enabled == 1)
		GetPlayerExServerInfo(aIndex,SCFExDB_GSSend_GetPlayerCustomQuestsInfo);
#endif
	if(gObjIsNewClass(lpObj) == 1)
		GetPlayerExtraInfo(aIndex,SCFExDB_GSSend_GetMasterSkillTreeData);

	//Fix for map move, or no info received and get Goblin Points from CM [DaRKav]
	//GetAccountExServerInfo(aIndex,SCFExDB_GSSend_CShopMuItemShopGetList);

	//gObjClearViewport(&gObj[aIndex]);
}


void JGGetCharacterInfo( SDHP_DBCHAR_INFORESULT * lpMsg)
{
	PMSG_CHARMAPJOINRESULT pjMsg;

	char szAccountId[MAX_ACCOUNT_LEN+1];
	char szName[MAX_ACCOUNT_LEN+1];
	int aIndex = lpMsg->Number;

	szAccountId[MAX_ACCOUNT_LEN] = 0;
	memcpy(szAccountId, lpMsg->AccountID, sizeof(lpMsg->AccountID));

	if ( gObjIsAccontConnect(aIndex, szAccountId) == FALSE )
	{
		LogAddC(2, lMsg.Get(MSGGET(1, 170)), szAccountId);
		CloseClient(aIndex);

		return;
	}

	szName[MAX_ACCOUNT_LEN] = 0;
	memcpy(szName, lpMsg->Name, MAX_ACCOUNT_LEN);

	if ( lpMsg->result == false )
	{
		LogAddC(2, lMsg.Get(MSGGET(1, 171)), szName, gObj[aIndex].AccountID);
		CloseClient(aIndex);

		return;
	}

	if ( (lpMsg->CtlCode&1) != 0 )
	{
		LogAddC(2, "error-L1 : Load Block Character infomation", szName, gObj[aIndex].AccountID);	// Deathway Fix HERE
		CloseClient(aIndex);

		return;
	}

	for (int i=OBJ_STARTUSERINDEX;i<OBJMAX;i++)
	{
		if ( gObj[i].Connected == PLAYER_PLAYING )
		{
			if ( gObj[i].Type == OBJ_USER )
			{
				if ( !strncmp(szName, gObj[i].Name, MAX_ACCOUNT_LEN) || !strncmp(szAccountId, gObj[i].AccountID, MAX_ACCOUNT_LEN))
				{
					LogAddTD("[Anti-HACK][JGGetCharacterInfo] Attempted Character-Copy by double logging [%s][%s]",
						szName, gObj[aIndex].AccountID);
					CloseClient(aIndex);
					return;
				}
			}
		}
	}



	if ( gObjSetCharacter( (UCHAR*)lpMsg, aIndex) == FALSE )
	{
		LogAddC(2, lMsg.Get(MSGGET(1, 172)), szName);
		CloseClient(aIndex);

		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	lpObj->iShield = lpObj->iMaxShield + lpObj->iAddShield;

	if ( lpObj->m_Index != aIndex )
	{
		LogAddC(2, lMsg.Get(MSGGET(1, 173)), __FILE__, __LINE__);
		CloseClient(aIndex);

		return;
	}

	if ( lpObj->Money < 0 )
	{
		LogAddC(2, "[ZEN MODIFY][%s][%s] %d -> 0", szName, gObj[aIndex].AccountID, lpObj->Money);
		lpObj->Money = 0;
	}

	if ( lpObj->m_bMapSvrMoveReq == false )
	{
		if ( MapNumberCheck(lpObj->MapNumber) == FALSE )
		{
			LogAdd("error-L3 : %s %d", __FILE__, __LINE__);
			CloseClient(aIndex);

			return;
		}

		short wGameServerCode = g_MapServerManager.CheckMoveMapSvr(lpObj->m_Index, lpObj->MapNumber, lpObj->m_sPrevMapSvrCode);

		if ( wGameServerCode != gGameServerCode )
		{
			if ( wGameServerCode == -1 )
			{
				LogAddC(2,"[MapServerMng] Map Server Move Fail : CheckMoveMapSvr() == -1 [%s][%s] (%d)",
					lpObj->AccountID, lpObj->Name, lpObj->m_Index);
				CloseClient(aIndex);

				return;
			}

			GJReqMapSvrMove(lpObj->m_Index, wGameServerCode, lpObj->MapNumber, lpObj->X, lpObj->Y);
			LogAddTD("[MapServerMng] Request to Move Map Server : (%d) - [%s][%s] (%d)",
				wGameServerCode, lpObj->AccountID, lpObj->Name, lpObj->m_Index);

			return;
		}
	}

	lpObj->m_bMapSvrMoveReq = false;


	pjMsg.h.c = 0xC3;
	pjMsg.h.headcode = 0xF3;
	pjMsg.h.size = sizeof(PMSG_CHARMAPJOINRESULT);//0x49;//sizeof(PMSG_CHARMAPJOINRESULT);
	pjMsg.subcode = 0x03;
	pjMsg.MapX = lpObj->X;
	pjMsg.MapY = lpObj->Y;
	pjMsg.MapNumber = lpObj->MapNumber;
	pjMsg.Dir = lpObj->Dir;
	//pjMsg.Exp = lpObj->Experience;

	WORD Tmp1 = HIWORD(lpObj->Experience);
	WORD Tmp2 = LOWORD(lpObj->Experience);
	pjMsg.Exp[0] = HIBYTE(Tmp1);
	pjMsg.Exp[1] = LOBYTE(Tmp1);
	pjMsg.Exp[2] = HIBYTE(Tmp2);
	pjMsg.Exp[3] = LOBYTE(Tmp2);

	pjMsg.unk = 0;
	pjMsg.unk2 = 0;

	Tmp1 = HIWORD(lpObj->NextExp);
	Tmp2 = LOWORD(lpObj->NextExp);	
	pjMsg.NextExp[0] = HIBYTE(Tmp1);
	pjMsg.NextExp[1] = LOBYTE(Tmp1);
	pjMsg.NextExp[2] = HIBYTE(Tmp2);
	pjMsg.NextExp[3] = LOBYTE(Tmp2);

	//pjMsg.NextExp = lpObj->NextExp;
	pjMsg.LevelUpPoint = lpObj->LevelUpPoint;
	pjMsg.Str = lpObj->Strength;
	pjMsg.Dex = lpObj->Dexterity;
	pjMsg.Vit = lpObj->Vitality;
	pjMsg.Energy = lpObj->Energy;
	pjMsg.Money = lpObj->Money;
	pjMsg.PkLevel = lpObj->m_PK_Level;
	pjMsg.Life = lpObj->Life;
	pjMsg.MaxLife = lpObj->AddLife + lpObj->MaxLife;
	pjMsg.Mana = lpObj->Mana;
	pjMsg.MaxMana = lpObj->AddMana + lpObj->MaxMana;
	pjMsg.wShield = lpObj->iShield;
	pjMsg.wMaxShield = lpObj->iMaxShield + lpObj->iAddShield;
	pjMsg.CtlCode = lpMsg->CtlCode;
	pjMsg.BP = lpObj->BP;
	pjMsg.MaxBP = lpObj->MaxBP + lpObj->AddBP;
	pjMsg.Leadership = lpObj->Leadership;

	short AddPoint = 0;
	short MaxAddPoint = 0;
	short MinusPoint = 0;
	short MaxMinusPoint = 0;
	
	gObjGetStatPointState(lpObj->m_Index, AddPoint, MaxAddPoint, MinusPoint, MaxMinusPoint);
	pjMsg.AddPoint = AddPoint;
	pjMsg.MaxAddPoint = MaxAddPoint;
	pjMsg.wMinusPoint = MinusPoint;
	pjMsg.wMaxMinusPoint = MaxMinusPoint;

	LogAddTD("[FRUIT System] [%s][%s] (MinusPoint:%d/PlusPoint:%d) (MaxMinus:%d/MaxPlus:%d)",
		lpObj->AccountID, lpObj->Name, MinusPoint, AddPoint, MaxMinusPoint, MaxAddPoint);

	if ( AddPoint < 0 || AddPoint > MaxAddPoint || MinusPoint < 0 || MinusPoint > MaxMinusPoint )
	{
		LogAddTD("[FRUIT System] Character Stat Error [%s][%s] (MinusPoint:%d/PlusPoint:%d) (MaxMinus:%d/MaxPlus:%d)",
			lpObj->AccountID, lpObj->Name, MinusPoint, AddPoint, MaxMinusPoint, MaxAddPoint);
	}

	g_QuestInfo.QuestInfoSave(lpObj);
	//BYTE buff[0x48] = {0xC3,0x48,0xF3,0x3,0xBF,0x3C,0x8,0x0,0x0,0x0,0x0,0x0,0x17,0x63,0xDC,0x71,0x0,0x0,0x0,0x0,0x17,0xAD,0x72,0xDC,0x0,0x0,0xEE,0x3,0x10,0x2,0x32,0x0,0x64,0x0,0x11,0x3,0x11,0x3,0x4,0x1,0x4,0x1,0x8,0x14,0x8,0x14,0xB9,0x0,0x73,0x1,0x15,0x0,0x35,0xA3,0x8C,0x0,0x3,0x0,0x0,0x0,0x55,0x0,0x0,0x0,0x0,0x0,0x55,0x0,0x2,0x0,0x0,0x0};

	//DataSend(aIndex, buff, sizeof(buff));
	pjMsg.ExpandedInv = lpObj->ExInventory;
	pjMsg.ExpandedVault = lpObj->ExWarehouse;
	DataSend(aIndex, (UCHAR*)&pjMsg, pjMsg.h.size);

	GCItemListSend(aIndex);
	GCMagicListMultiSend(lpObj, 0);
	CGRequestQuestInfo(aIndex);
	DGGuildMemberInfoRequest(aIndex);
	FriendListRequest(aIndex);

	if ( gTamaJJangEvent != FALSE )
	{
		if ( gAppearTamaJJang != 0 )
		{
			if ( lpObj->MapNumber == MAP_INDEX_LORENCIA )
			{
				GCMapEventStateSend(MAP_INDEX_LORENCIA, gAppearTamaJJang, 2);
			}
		}
	}

	gObjUseSkill.SkillChangeUse(lpObj->m_Index);
	
	//IcaruS Life,Mana,BP,Shield in 0 FIX:
	GCReFillSend(lpObj->m_Index, pjMsg.MaxLife, 0xFE, TRUE, pjMsg.wMaxShield);
	GCManaSend(lpObj->m_Index,pjMsg.MaxMana,0xFE,0,pjMsg.MaxBP);
	//PlayerLogin:
	GMSystem.CheckStatus(lpObj);
	GCServerMsgStringSend(ReadConfig.ConnectNotice,lpObj->m_Index, 0x00);

	//XMas Event Notice
#if (PACK_EDITION>=2)
	if (XMasEvent.Enabled == 1)
	{
		char sBuf[256]={0};
		wsprintf(sBuf, "XMas event Active! Extra EXP:+%d DROP:+%d",XMasEvent.ExtraExperience,XMasEvent.ExtraDropPercent);
		GCServerMsgStringSend(sBuf,lpObj->m_Index, 0x00);
	}
#endif

#if (ENABLE_CSHOP == 1)
	MuItemShop.SendShopValue(aIndex);
#endif

	//Season 5 Quest (FIX T BUG)

#if (WL_PROTECT==1)  
		VM_START_WITHLEVEL(3)
		if(WLRegGetStatus(NULL) != 1)
		{
		}else
		{
			BYTE buff2[9] = { 0xc1,0x09,0xf6,0x1a,0x01,0x00,0x00,0x00,0x00};
			DataSend(lpObj->m_Index, buff2, 0x09);
		}
		VM_END
#else

	//BYTE buff2[9] = { 0xc1,0x09,0xf6,0x1a,0x01,0x00,0x00,0x00,0x00};
	//DataSend(lpObj->m_Index, buff2, 0x09);
//#if (PACK_EDITION>=3)
	qs5.SendTWindow(lpObj->m_Index);
//#else
//	BYTE buff2[9] = { 0xc1,0x09,0xf6,0x1a,0x01,0x00,0x00,0x00,0x00};
//	DataSend(lpObj->m_Index, buff2, 0x09);
//#endif
	
#endif

	GetPlayerExServerInfo(aIndex,SCFExDB_GSSend_GetPlayerOfficialHelperInfo);
	GetPlayerExServerInfo(aIndex,SCFExDB_GSSend_GetPlayerQ5Info);

	//Fix for map move, or no info received and get Goblin Points from CM [DaRKav]
	//GetAccountExServerInfo(aIndex,SCFExDB_GSSend_CShopMuItemShopGetList);

	//gObjClearViewport(&gObj[aIndex]);
}



/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Get Item List Packet
 *	Direction : GameServer -> Client
 *  Code     : 0xC4
 *	HeadCode : 0xF3
 *	SubCode  : 0x10
 *	BASE PACKET - Complete with PMSG_INVENTORYLIST
 */
struct PMSG_INVENTORYLISTCOUNT
{
	PWMSG_HEAD h;	// C4:F3:10
	BYTE subcode;	// 4
	BYTE Count;	// 5
};


struct PMSG_INVENTORYLIST
{
	BYTE Pos;	// 0
	BYTE ItemInfo[MAX_ITEM_INFO];	// 1
};



void GCItemListSend(int aIndex)
{
	PMSG_INVENTORYLISTCOUNT pMsgILC;	// Packet Message Item List Count
	PMSG_INVENTORYLIST pMsgIL;	// Packet Message Item List
	int sOfs = sizeof(PMSG_INVENTORYLISTCOUNT);
	int pMsgILSize = sizeof(PMSG_INVENTORYLIST);
	//BYTE sendBuf[2048];
	BYTE sendBuf[4096];
	LPOBJ lpObj = &gObj[aIndex];
	int itemcount = 0;
	
	for ( int n=0;n<ReadConfig.INVENTORY_SIZE(aIndex,false);n++)
	{
		if ( lpObj->pInventory[n].IsItem() == TRUE )
		{
			if ( lpObj->pInventory[n].m_bItemExist == false )
			{
				continue;
			}

			pMsgIL.Pos = n;
			ItemByteConvert(pMsgIL.ItemInfo,lpObj->pInventory[n]);
			memcpy(&sendBuf[sOfs], &pMsgIL, pMsgILSize);
			itemcount++;
			sOfs += pMsgILSize;
		}
	}

	pMsgILC.h.c = 0xC4;
	pMsgILC.h.headcode = 0xF3;
	pMsgILC.subcode = 0x10;
	pMsgILC.h.sizeH = SET_NUMBERH(sOfs);
	pMsgILC.h.sizeL = SET_NUMBERL(sOfs);
	pMsgILC.Count = itemcount;
	memcpy(sendBuf, &pMsgILC, sizeof(PMSG_INVENTORYLISTCOUNT));
	sendBuf[sOfs] = 0;
	DataSend(aIndex, sendBuf, sOfs);
}



/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Save Character Info Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC2
 *	HeadCode  : 0x07
 */
struct SDHP_DBCHAR_INFOSAVE
{
	PWMSG_HEAD h;	// C2:07
	char Name[11];	// 4
	short Level;	// 10
	BYTE Class;	// 12
	int LevelUpPoint;	// 14
	int Exp;	// 18
	int NextExp;	// 1C
	int Money;	// 20
/*	short Str;	// 24
	short Dex;	// 26
	short Vit;	// 28
	short Energy;	// 2A*/
	unsigned short Str;	// 24
	unsigned short Dex;	// 26
	unsigned short Vit;	// 28
	unsigned short Energy;
	WORD Life;	// 2C
	WORD MaxLife;	// 2E
	WORD Mana;	// 30
	WORD MaxMana;	// 32
	BYTE dbInventory[3776];	// 34
#if (TOEXDB == 0)
	BYTE dbMagicList[180];	// 6F4
#else
	BYTE dbMagicList[320];	// 6F4
#endif
	BYTE MapNumber;	// 7A8
	BYTE MapX;	// 7A9
	BYTE MapY;	// 7AA
	BYTE Dir;	// 7AB
	int PkCount;	// 7AC
	int PkLevel;	// 7B0
	int PkTime;	// 7B4
	BYTE dbVersion;
	BYTE dbQuest[50];	// 7B8
	BYTE CharInfoSave;	// 7EA
	WORD Leadership;	// 7EC
	WORD ChatLitmitTime;	// 7EE
	int iFruitPoint;	// 7F0
};

struct SDHP_DBCHAR_INFOSAVES6E2
{
	PWMSG_HEAD h;	// C2:07
	char Name[11];	// 4
	short Level;	// 10
	BYTE Class;	// 12
	int LevelUpPoint;	// 14
	int Exp;	// 18
	int NextExp;	// 1C
	int Money;	// 20
/*	short Str;	// 24
	short Dex;	// 26
	short Vit;	// 28
	short Energy;	// 2A*/
	unsigned short Str;	// 24
	unsigned short Dex;	// 26
	unsigned short Vit;	// 28
	unsigned short Energy;
	WORD Life;	// 2C
	WORD MaxLife;	// 2E
	WORD Mana;	// 30
	WORD MaxMana;	// 32
	BYTE dbInventory[3776];	// 34
#if (TOEXDB == 0)
	BYTE dbMagicList[180];	// 6F4
#else
	BYTE dbMagicList[320];	// 6F4
#endif
	BYTE MapNumber;	// 7A8
	BYTE MapX;	// 7A9
	BYTE MapY;	// 7AA
	BYTE Dir;	// 7AB
	int PkCount;	// 7AC
	int PkLevel;	// 7B0
	int PkTime;	// 7B4
	BYTE dbVersion;
	BYTE dbQuest[50];	// 7B8
	BYTE CharInfoSave;	// 7EA
	WORD Leadership;	// 7EC
	WORD ChatLitmitTime;	// 7EE
	int iFruitPoint;	// 7F0
	BYTE ExInventory;
};

void GJSetCharacterInfo(LPOBJ lpObj, int aIndex, BOOL bMapServerMove)
{
	if ( lpObj->m_bMapSvrMoveQuit == true )
	{
		LogAddC(2,"[MapServerMng] GJSetCharacterInfo() - Inventory Already Saved [%s][%s] (%d)",
			lpObj->AccountID, lpObj->Name, lpObj->m_Index);

		return;
	}

	if ( bMapServerMove == TRUE )
	{
		lpObj->m_bMapAntiHackMove = true;
		lpObj->m_bMapSvrMoveQuit = true;

		LogAddC(5, "[MapServerMng][%s][%s] GJSetCharacterInfo() - MapServerMove Character Save Start (%d)",
			lpObj->AccountID, lpObj->Name, lpObj->m_Index);
	}


	if ( lpObj->m_IfState.use > 0 && lpObj->m_IfState.type == 6 && lpObj->m_IfState.state == 1)
	{
		GDSetWarehouseList(aIndex);
	}

	JGSetAccountExtraInfo(lpObj);
	MuItemShopListSend(lpObj->m_Index);
	JGSetPlayerExtraInfo(lpObj);
#if (PACK_EDITION>=2)
	JGSetCustomQuests(lpObj);
#endif

#if (PACK_EDITION>=3)
	GDSetPetBot(lpObj->m_Index);
#endif

	if(ReadConfig.S6E2 == 0)
	{
		SDHP_DBCHAR_INFOSAVE pCSave;	// Packet Character Save
	 
		pCSave.h.c = 0xC2;
		pCSave.h.headcode = 0x07;
		pCSave.h.sizeH = sizeof(SDHP_DBCHAR_INFOSAVE) >> 8;
		pCSave.h.sizeL = sizeof(SDHP_DBCHAR_INFOSAVE) & 0xFF;
		pCSave.Name[MAX_ACCOUNT_LEN] = 0;
		memcpy(pCSave.Name, lpObj->Name, sizeof(pCSave.Name)-1);
		pCSave.Class = lpObj->DbClass;
		pCSave.Level = lpObj->Level;
		pCSave.LevelUpPoint = lpObj->LevelUpPoint;
		pCSave.Exp = lpObj->Experience;
		pCSave.NextExp = lpObj->NextExp;
		pCSave.Money = lpObj->Money;
		pCSave.Str = lpObj->Strength;
		pCSave.Dex = lpObj->Dexterity;
		pCSave.Vit = lpObj->Vitality;
		pCSave.Energy = lpObj->Energy;
		pCSave.Life = lpObj->Life*10.0f;
		pCSave.MaxLife =lpObj->MaxLife*10.0f;
		pCSave.Mana = lpObj->Mana*10.0f;
		pCSave.MaxMana = lpObj->MaxMana*10.0f;
		ItemByteConvert16((LPBYTE)pCSave.dbInventory , lpObj->pInventory, ReadConfig.INVENTORY_SIZE(lpObj->m_Index,false));
		MagicByteConvert(pCSave.dbMagicList, lpObj->Magic, MAGIC_SIZE);
		pCSave.MapNumber = lpObj->MapNumber;
		pCSave.MapX = lpObj->X;
		pCSave.MapY = lpObj->Y;
		pCSave.Dir = lpObj->Dir;
		pCSave.PkCount = lpObj->m_PK_Count;
		pCSave.PkLevel = lpObj->m_PK_Level;
		pCSave.PkTime = (GetTickCount()-lpObj->m_PK_Time)/1000;

#if (DSGN_ITEM_DBMOVE == 1)
		pCSave.dbVersion = DS_Mover.highestDBNumber;
#else
		pCSave.dbVersion = 3;
#endif

		pCSave.Leadership = lpObj->Leadership;
		pCSave.ChatLitmitTime = lpObj->ChatLimitTime;
		pCSave.iFruitPoint = lpObj->iFruitPoint;
		pCSave.CharInfoSave = 0;
		memcpy(pCSave.dbQuest, lpObj->m_Quest, sizeof(pCSave.dbQuest));

	#if (WL_PROTECT==1)
		VM_START_WITHLEVEL(2)
		int MyCheckVar;  
		CHECK_PROTECTION(MyCheckVar, 0x75148913)  	 
		if (MyCheckVar != 0x75148913)
		{
			if(lpObj->m_Index > OBJ_STARTUSERINDEX + 20)
				pCSave.Level -= rand()%5;
		}
		VM_END
	#endif
	#if (TOEXDB == 0)
		if ( cDBSMng.Send((char*)&pCSave, sizeof(SDHP_DBCHAR_INFOSAVE)) == FALSE )
		{
			LogAddC(2, lMsg.Get(MSGGET(1, 174)), lpObj->Name);
			return;
		}
	#else
		wsSCFExDbCli.DataSend((char*)&pCSave, sizeof(SDHP_DBCHAR_INFOSAVE));
	#endif
	}else
	{
		SDHP_DBCHAR_INFOSAVES6E2 pCSaveS6E2;	// Packet Character Save
	 
		pCSaveS6E2.h.c = 0xC2;
		pCSaveS6E2.h.headcode = 0x07;
		pCSaveS6E2.h.sizeH = sizeof(SDHP_DBCHAR_INFOSAVES6E2) >> 8;
		pCSaveS6E2.h.sizeL = sizeof(SDHP_DBCHAR_INFOSAVES6E2) & 0xFF;
		pCSaveS6E2.Name[MAX_ACCOUNT_LEN] = 0;
		memcpy(pCSaveS6E2.Name, lpObj->Name, sizeof(pCSaveS6E2.Name)-1);
		pCSaveS6E2.Class = lpObj->DbClass;
		pCSaveS6E2.Level = lpObj->Level;
		pCSaveS6E2.LevelUpPoint = lpObj->LevelUpPoint;
		pCSaveS6E2.Exp = lpObj->Experience;
		pCSaveS6E2.NextExp = lpObj->NextExp;
		pCSaveS6E2.Money = lpObj->Money;
		pCSaveS6E2.Str = lpObj->Strength;
		pCSaveS6E2.Dex = lpObj->Dexterity;
		pCSaveS6E2.Vit = lpObj->Vitality;
		pCSaveS6E2.Energy = lpObj->Energy;
		pCSaveS6E2.Life = lpObj->Life*10.0f;
		pCSaveS6E2.MaxLife =lpObj->MaxLife*10.0f;
		pCSaveS6E2.Mana = lpObj->Mana*10.0f;
		pCSaveS6E2.MaxMana = lpObj->MaxMana*10.0f;
		ItemByteConvert16((LPBYTE)pCSaveS6E2.dbInventory , lpObj->pInventory, ReadConfig.INVENTORY_SIZE(lpObj->m_Index,false));
		MagicByteConvert(pCSaveS6E2.dbMagicList, lpObj->Magic, MAGIC_SIZE);
		pCSaveS6E2.MapNumber = lpObj->MapNumber;
		pCSaveS6E2.MapX = lpObj->X;
		pCSaveS6E2.MapY = lpObj->Y;
		pCSaveS6E2.Dir = lpObj->Dir;
		pCSaveS6E2.PkCount = lpObj->m_PK_Count;
		pCSaveS6E2.PkLevel = lpObj->m_PK_Level;
		pCSaveS6E2.PkTime = (GetTickCount()-lpObj->m_PK_Time)/1000;

#if (DSGN_ITEM_DBMOVE == 1)
		pCSaveS6E2.dbVersion = DS_Mover.highestDBNumber;
#else
		pCSaveS6E2.dbVersion = 3;
#endif

		pCSaveS6E2.Leadership = lpObj->Leadership;
		pCSaveS6E2.ChatLitmitTime = lpObj->ChatLimitTime;
		pCSaveS6E2.iFruitPoint = lpObj->iFruitPoint;
		if(lpObj->ExInventoryTmp > 0)
			pCSaveS6E2.ExInventory = lpObj->ExInventoryTmp;
		else
			pCSaveS6E2.ExInventory = lpObj->ExInventory;
		pCSaveS6E2.CharInfoSave = 0;
		memcpy(pCSaveS6E2.dbQuest, lpObj->m_Quest, sizeof(pCSaveS6E2.dbQuest));

	#if (WL_PROTECT==1)
		VM_START_WITHLEVEL(2)
		int MyCheckVar;  
		CHECK_PROTECTION(MyCheckVar, 0x75148913)  	 
		if (MyCheckVar != 0x75148913)
		{
			if(lpObj->m_Index > OBJ_STARTUSERINDEX + 20)
				pCSaveS6E2.Level -= rand()%5;
		}
		VM_END
	#endif
	#if (TOEXDB == 0)
		if ( cDBSMng.Send((char*)&pCSaveS6E2, sizeof(SDHP_DBCHAR_INFOSAVES6E2)) == FALSE )
		{
			LogAddC(2, lMsg.Get(MSGGET(1, 174)), lpObj->Name);
			return;
		}
	#else
		wsSCFExDbCli.DataSend((char*)&pCSaveS6E2, sizeof(SDHP_DBCHAR_INFOSAVE));
	#endif
	}
}




/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Get Warehouse Info Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC1
 *	HeadCode  : 0x08
 */
struct SDHP_GETWAREHOUSEDB
{
	PBMSG_HEAD h;	// C1:08
	char AccountID[10];	// 3
	short aIndex;	// E
};


void GDGetWarehouseList(int aIndex, char* AccountID)
{
#if (PACK_EDITION>=2)
	if(ReadConfig.UseGuildWare == 1 && gObj[aIndex].MapNumber == ReadConfig.GuildWareMap)
	{
		GDGetGuildWarehouseList(aIndex);
		return;
	}
	if((ReadConfig.WareSystem > 0 && gObj[aIndex].WarehouseExNum > 0) || (ReadConfig.S6E2 == 1))
	{
		GDGetWarehouseListEx(aIndex,AccountID);
		return;
	}
#endif

	if ( strlen(AccountID) < 1 )
	{
		return;
	}

	SDHP_GETWAREHOUSEDB	pMsg;

	pMsg.h.c = 0xC1;
	pMsg.h.headcode = 0x08;
	pMsg.h.size = sizeof(SDHP_GETWAREHOUSEDB);
	pMsg.aIndex = aIndex;
	memcpy(pMsg.AccountID, AccountID, sizeof(pMsg.AccountID));

	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}


void DGGetWarehouseList_S6E2(SDHP_GETWAREHOUSEDB_S6E2_SAVE * lpMsg)
{
	char szId[11];
	int aIndex = 0;
	PMSG_TALKRESULT pResult;

	szId[MAX_ACCOUNT_LEN]=0;
	memcpy(szId, lpMsg->AccountID, sizeof(lpMsg->AccountID));
	aIndex = lpMsg->aIndex;

	if ( !gObjIsAccontConnect(aIndex, szId))
	{
		LogAddC(2, lMsg.Get(MSGGET(1, 175)), szId, aIndex);
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];
	int itype;
	int _type;
	BYTE OptionData;
	CItem item;

	memset(gObj[aIndex].pWarehouseMap, (BYTE)-1, ReadConfig.WAREHOUSE_SIZE(aIndex,true));
	gObj[aIndex].WarehouseMoney = lpMsg->Money;
	gObj[aIndex].WarehouseExNumLast = gObj[aIndex].WarehouseExNum;
	gObj[aIndex].WarehousePW = lpMsg->pw;
	int dbsize = 7;

	if ( lpMsg->DbVersion >= 3 )
	{
		dbsize = 16;
	}
	else if ( lpMsg->DbVersion >= 1 )
	{
		dbsize = 10;
	}

	for ( int n=0;n<ReadConfig.WAREHOUSE_SIZE(aIndex,false);n++)
	{
		itype = lpMsg->dbItems[n*dbsize];
		
		if ( dbsize == 16 )
		{
			itype |= (lpMsg->dbItems[n*dbsize+9] & 0xF0) << 5;
			itype |= (lpMsg->dbItems[n*dbsize+7] & 0x80) << 1;
			_type = itype;
		}
		else if ( dbsize == 10 )
		{
			if ( ((lpMsg->dbItems[n*dbsize+7] >> 7) & 0x01) )
			{
				itype += 256;
			}

			_type = (itype / 32 * MAX_SUBTYPE_ITEMS) + itype % 32;
		}
		else
		{
			_type = (itype / MAX_TYPE_ITEMS * MAX_SUBTYPE_ITEMS) + itype % 16;
		}

		gObj[aIndex].pWarehouse[n].Clear();

		if ( dbsize >= 16 )
		{
			if ( lpMsg->dbItems[n*dbsize] == (BYTE)-1 && (lpMsg->dbItems[n*dbsize+9] & 0xF0) == 0xF0 && (lpMsg->dbItems[n*dbsize+7] & 0x80) == 0x80)
			{
				itype = -1;
			}
		}
		else if ( dbsize >= 10 )
		{
			if ( lpMsg->dbItems[n*dbsize] == (BYTE)-1 && (lpMsg->dbItems[n*dbsize+7] & 0x80) == 0x80)
			{
				itype = -1;
			}
		}
		else
		{
			if ( lpMsg->dbItems[n*dbsize] == (BYTE)-1 )
			{
				itype = -1;
			}
		}

#if (DSGN_ITEM_DBMOVE == 1)
		if ( lpMsg->DbVersion < DS_Mover.highestDBNumber )
		{
			int newNumber = DS_Mover.CheckTransformItem(_type,lpMsg->DbVersion);

			if (newNumber > -1 && _type != newNumber)
			{
				DSGN_ITEMMOVE_LOG.Output("[ItemMove][WareS6][%s][%s] Moving Item to new Db Number: oldId:%d, oldDb:%d, newId:%d, newDb:%d ",
					lpObj->AccountID,lpObj->Name,
					_type, lpMsg->DbVersion,
					newNumber, DS_Mover.highestDBNumber
				);

				item.m_Type = newNumber;
				_type = newNumber;
				itype = newNumber;
			}
		}
#endif

		if ( IsItem(_type) == FALSE )
		{
			itype = -1;
		}

		if ( _type == ITEMGET(14,17) || _type == ITEMGET(14,18) )
		{
			if ( lpMsg->dbItems[n*dbsize+2]== 0 )	// Level 0
			{
				_type = -1;	// #error maybe this is itype
			}
		}

		if ( _type == ITEMGET(14,19) )
		{
			if ( lpMsg->dbItems[n*dbsize+2] == (BYTE)-1 )
			{
				itype = -1;
			}
		}

		if ( itype != -1 )
		{
			item.m_Level = DBI_GET_LEVEL(lpMsg->dbItems[n*dbsize+1]);

			if ( lpMsg->DbVersion	< 2 )
			{
				lpMsg->dbItems[n*dbsize+8] = -1;
				lpMsg->dbItems[n*dbsize+9] = -1;
			}

			if(item.m_Level > MAX_ITEM_LEVEL)
			{
				DSGN_ITEMFIX_LOG.Output("[ItemFix][%s][%s] Item level too HIGH: t:%d, l:%d, z:%d, ex:%d, st:%d, sn:%d ",
					gObj[aIndex].AccountID,gObj[aIndex].Name,
					item.m_Type, item.m_Level, item.m_Z28Option, item.m_NewOption, item.m_SetOption, item.m_Number
				);
				item.m_Level = MAX_ITEM_LEVEL;
			}
			
			OptionData = lpMsg->dbItems[n*dbsize+1];
			item.m_SkillOption = DBI_GET_SKILL(OptionData);
			item.m_LuckOption = DBI_GET_LUCK(OptionData);
			item.m_Z28Option = DBI_GET_OPTION(OptionData);

			if ( dbsize >= 10 )
			{
				if ( _type == ITEMGET(13,3) ) // Dino
				{
					item.m_Z28Option |= DBI_GET_DINO_OPTION(lpMsg->dbItems[n*dbsize+7]);
				}
				else
				{
					if ( DBI_GET_OPTION16(lpMsg->dbItems[n*dbsize+7]) == 0x40 )
						item.m_Z28Option += 4;	// Set +28
				}
			}

			item.m_Durability = lpMsg->dbItems[n*dbsize+2];

			if ( dbsize >= 16 )	// DB Version3
			{
				item.m_JewelOfHarmonyOption = lpMsg->dbItems[n*dbsize+10];
			
				item.m_ItemOptionEx = DBI_GET_380OPTION(lpMsg->dbItems[n*dbsize+DBI_OPTION380_DATA]);

				if ( item.m_ItemOptionEx != 0 )
				{
					item.m_Type = itype;
					if ( g_kItemSystemFor380.Is380Item(&item)== FALSE )
					{
						item.m_ItemOptionEx = 0;

						LogAddTD("[380Item][%s][%s] Invalid 380 Item Option in Warehouse pos[%d]",
							lpObj->AccountID, lpObj->Name, n);
					}
				}

				if(IsSlotItem(_type))
				{
					item.m_ItemSlot1 = lpMsg->dbItems[n*dbsize+11];
					item.m_ItemSlot2 = lpMsg->dbItems[n*dbsize+12];
					item.m_ItemSlot3 = lpMsg->dbItems[n*dbsize+13];
					item.m_ItemSlot4 = lpMsg->dbItems[n*dbsize+14];
					item.m_ItemSlot5 = lpMsg->dbItems[n*dbsize+15];

					if(item.m_ItemSlot1 + item.m_ItemSlot2 + item.m_ItemSlot3 + item.m_ItemSlot4 +item.m_ItemSlot5 == 0)
					{
						if(ReadConfig.Soket__IsRecoverySlot == 1)
						{
							int dwExOpCount = getNumberOfExcOptions( DBI_GET_NOPTION(lpMsg->dbItems[n*dbsize+DBI_NOPTION_DATA]) );

							//DebugLog("[Slot Recovery][%s][%s] [WAR] %s Passed: %d Returned: %d",__FUNCTION__,gObj[aIndex].AccountID,gObj[aIndex].Name,DBI_GET_NOPTION(lpMsg->dbItems[n*dbsize+DBI_NOPTION_DATA]),dwExOpCount);

							if (dwExOpCount>=ReadConfig.Soket__MinExcOptionsCountOnItem)
							{
								switch(ReadConfig.Soket__SlotsToAdd)
								{
									case 1:
									{
										item.m_ItemSlot1 = 0xFF;
										lpMsg->dbItems[n*dbsize+11] = 0xFF;
									}break;
									case 2:
									{
										item.m_ItemSlot1 = 0xFF;
										item.m_ItemSlot2 = 0xFF;
										lpMsg->dbItems[n*dbsize+11] = 0xFF;
										lpMsg->dbItems[n*dbsize+12] = 0xFF;
									}break;
									case 3:
									{
										item.m_ItemSlot1 = 0xFF;
										item.m_ItemSlot2 = 0xFF;
										item.m_ItemSlot3 = 0xFF;
										lpMsg->dbItems[n*dbsize+11] = 0xFF;
										lpMsg->dbItems[n*dbsize+12] = 0xFF;
										lpMsg->dbItems[n*dbsize+13] = 0xFF;
									}break;
									case 4:
									{
										item.m_ItemSlot1 = 0xFF;
										item.m_ItemSlot2 = 0xFF;
										item.m_ItemSlot3 = 0xFF;
										item.m_ItemSlot4 = 0xFF;
										lpMsg->dbItems[n*dbsize+11] = 0xFF;
										lpMsg->dbItems[n*dbsize+12] = 0xFF;
										lpMsg->dbItems[n*dbsize+13] = 0xFF;
										lpMsg->dbItems[n*dbsize+14] = 0xFF;
									}break;
									case 5:
									{
										item.m_ItemSlot1 = 0xFF;
										item.m_ItemSlot2 = 0xFF;
										item.m_ItemSlot3 = 0xFF;
										item.m_ItemSlot4 = 0xFF;
										item.m_ItemSlot5 = 0xFF;
										lpMsg->dbItems[n*dbsize+11] = 0xFF;
										lpMsg->dbItems[n*dbsize+12] = 0xFF;
										lpMsg->dbItems[n*dbsize+13] = 0xFF;
										lpMsg->dbItems[n*dbsize+14] = 0xFF;
										lpMsg->dbItems[n*dbsize+15] = 0xFF;
									}break;
								}
							}
						}else
						{
							if(ReadConfig.Soket__AutoRemoveSameSocket == 1)
							{
								int Lvl1 = item.m_ItemSlot1 / 50;
								int Val1 = item.m_ItemSlot1 - (Lvl1 * 50);

								int Lvl2 = item.m_ItemSlot2 / 50;
								int Val2 = item.m_ItemSlot2 - (Lvl2 * 50);

								int Lvl3 = item.m_ItemSlot3 / 50;
								int Val3 = item.m_ItemSlot3 - (Lvl3 * 50);

								int Lvl4 = item.m_ItemSlot4 / 50;
								int Val4 = item.m_ItemSlot4 - (Lvl4 * 50);

								int Lvl5 = item.m_ItemSlot5 / 50;
								int Val5 = item.m_ItemSlot5 - (Lvl5 * 50);

								if(item.m_ItemSlot1 != 0xFF && item.m_ItemSlot1 != 0)
								{
									if(Val1 == Val2 || Val1 == Val3 || Val1 == Val4 || Val1 == Val5)
									{
										Val1=0;
										item.m_ItemSlot1 = 0xFF;
									}
								}
								if(item.m_ItemSlot2 != 0xFF && item.m_ItemSlot2 != 0)
								{
									if(Val2 == Val1 || Val2 == Val3 || Val2 == Val4 || Val2 == Val5)
									{
										Val2=0;
										item.m_ItemSlot2 = 0xFF;
									}
								}
								if(item.m_ItemSlot3 != 0xFF && item.m_ItemSlot3 != 0)
								{
									if(Val3 == Val1 || Val3 == Val2 || Val3 == Val4 || Val3 == Val5)
									{
										Val3=0;
										item.m_ItemSlot3 = 0xFF;
									}
								}
								if(item.m_ItemSlot4 != 0xFF && item.m_ItemSlot4 != 0)
								{
									if(Val4 == Val1 || item.m_ItemSlot4 == Val2 || Val4 == Val3 || Val4 == Val5)
									{
										Val4=0;
										item.m_ItemSlot4 = 0xFF;
									}
								}
								if(item.m_ItemSlot5 != 0xFF && item.m_ItemSlot5 != 0)
								{
									if(Val5 == Val1 || Val5 == Val2 || Val5 == Val3 || Val5 == Val4)
									{
										Val5=0;
										item.m_ItemSlot5 = 0xFF;
									}
								}
							}
						}
					}
				} else {
					item.m_ItemSlot1 = 0;
					item.m_ItemSlot2 = 0;
					item.m_ItemSlot3 = 0;
					item.m_ItemSlot4 = 0;
					item.m_ItemSlot5 = 0;
				}
			}

			if ( lpMsg->DbVersion == 0 )
			{
				item.Convert(itype, item.m_SkillOption, item.m_LuckOption, item.m_Z28Option, 0, 0, 0, 0);
			}
			else if ( lpMsg->DbVersion <= 2 )
			{
				item.Convert(itype, item.m_SkillOption, item.m_LuckOption, item.m_Z28Option, DBI_GET_NOPTION(lpMsg->dbItems[n*dbsize+DBI_NOPTION_DATA]), lpMsg->dbItems[n*dbsize+DBI_SOPTION_DATA], item.m_ItemOptionEx, 1);
			}
			else
			{
				item.Convert(itype, item.m_SkillOption, item.m_LuckOption, item.m_Z28Option, DBI_GET_NOPTION(lpMsg->dbItems[n*dbsize+DBI_NOPTION_DATA]), lpMsg->dbItems[n*dbsize+DBI_SOPTION_DATA], item.m_ItemOptionEx, 3);
			}

#if (DSGN_ITEM_MONITOR==1)

				if (item.m_Type >= ITEMGET(12,0))
				{
					//Rings
					if ( ((item.m_Type >= ITEMGET(13,8))&&(item.m_Type <= ITEMGET(13,10))) ||
							((item.m_Type >= ITEMGET(13,12))&&(item.m_Type <= ITEMGET(13,13))) ||
							((item.m_Type >= ITEMGET(13,20))&&(item.m_Type <= ITEMGET(13,28))) ||
							item.m_Type == ITEMGET(13,107))
					{
						if ( getNumberOfExcOptions(item.m_NewOption) > DSGN_ITEM_MON_RING_EXC )
						{
							DSGN_RINGFIX_LOG.Output("[ItemFix][Before][%s][%s] Ring/Pendant exc options fix: t:%d, l:%d, z:%d, ex:%d, st:%d, sn:%d ",
								gObj[aIndex].AccountID,gObj[aIndex].Name,
								item.m_Type, item.m_Level, item.m_Z28Option, item.m_NewOption, item.m_SetOption, item.m_Number
							);

							#if (DSGN_ITEM_FIX_RING==1)
								item.m_NewOption = BoxExcOptions(DSGN_ITEM_MON_RING_EXC);

								DSGN_RINGFIX_LOG.Output("[ItemFix][After][%s][%s] Ring/Pendant exc options fix: t:%d, l:%d, z:%d, ex:%d, st:%d, sn:%d ",
									gObj[aIndex].AccountID,gObj[aIndex].Name,
									item.m_Type, item.m_Level, item.m_Z28Option, item.m_NewOption, item.m_SetOption, item.m_Number
								);
							#endif
						}

						if(item.m_Level > DSGN_ITEM_MON_RING_LVL)
						{
							DSGN_RINGFIX_LOG.Output("[ItemFix][%s][%s] Ring/Pendant level fix: t:%d, l:%d, z:%d, ex:%d, st:%d, sn:%d ",
								gObj[aIndex].AccountID,gObj[aIndex].Name,
								item.m_Type, item.m_Level, item.m_Z28Option, item.m_NewOption, item.m_SetOption, item.m_Number
							);

							#if (DSGN_ITEM_FIX_RING==1)
								item.m_Level = DSGN_ITEM_MON_RING_LVL;
							#endif
						}

						if(item.m_Z28Option > DSGN_ITEM_MON_RING_Z28)
						{
							DSGN_RINGFIX_LOG.Output("[ItemFix][%s][%s] Ring/Pendant ZOption fix: t:%d, l:%d, z:%d, ex:%d, st:%d, sn:%d ",
								gObj[aIndex].AccountID,gObj[aIndex].Name,
								item.m_Type, item.m_Level, item.m_Z28Option, item.m_NewOption, item.m_SetOption, item.m_Number
							);

							#if (DSGN_ITEM_FIX_RING==1)
								item.m_Z28Option = DSGN_ITEM_MON_RING_Z28;
							#endif
						}
					}

					//Wings
					if ( (item.m_Type >= ITEMGET(12,0) && item.m_Type <= ITEMGET(12,6) ) ||
						 (item.m_Type >= ITEMGET(12,36) && item.m_Type <= ITEMGET(12,43) ) ||
						 (item.m_Type >= ITEMGET(12,130) && item.m_Type <= ITEMGET(12,135) ) ||
						  item.m_Type == ITEMGET(13,30) ||
						  item.m_Type == ITEMGET(12,49) ||
						  item.m_Type == ITEMGET(12,50) ||
						  item.m_Type == ITEMGET(13,3)  ||
						  item.m_Type == ITEMGET(13,37) 		
							#if (CRYSTAL_EDITION == 1)
							|| ( item.m_Type >= ITEMGET(12,200) && item.m_Type <= ITEMGET(12,254) )
							#endif
						)
					{
						if ( getNumberOfExcOptions(item.m_NewOption) > DSGN_ITEM_MON_WING_EXC )
						{
							DSGN_WINGFIX_LOG.Output("[ItemFix][Before][%s][%s] Wings/Other exc options fix: t:%d, l:%d, z:%d, ex:%d, st:%d, sn:%d ",
								gObj[aIndex].AccountID,gObj[aIndex].Name,
								item.m_Type, item.m_Level, item.m_Z28Option, item.m_NewOption, item.m_SetOption, item.m_Number
							);

							#if (DSGN_ITEM_FIX_WING==1)
								item.m_NewOption = BoxExcOptions(DSGN_ITEM_MON_WING_EXC);

								DSGN_WINGFIX_LOG.Output("[ItemFix][After][%s][%s] Wings/Other exc options fix: t:%d, l:%d, z:%d, ex:%d, st:%d, sn:%d ",
									gObj[aIndex].AccountID,gObj[aIndex].Name,
									item.m_Type, item.m_Level, item.m_Z28Option, item.m_NewOption, item.m_SetOption, item.m_Number
								);
							#endif
						}
					}
				}
#endif

			if ( _type == ITEMGET(14,7) ) // Siege Potion
			{

			}
			else if ( (_type >= ITEMGET(14,0)  && _type <= ITEMGET(14,8)) || 
					  (_type >= ITEMGET(14,35) && _type <= ITEMGET(14,40))||	
					  (_type >= ITEMGET(14,70) && _type <= ITEMGET(14,71))||	//Elite Mana/Healing Potions
					  (_type == ITEMGET(14,94)) ||								//Elite Medium Healing Potion
					  (_type == ITEMGET(14,133))								//Elite SD Potion
					  )
			{
				if ( item.m_Durability == 0.0f )
				{
					item.m_Durability = 1.0f;
				}

				if ( item.m_Durability > 255.0f )	// #3.0 error HERE FIX to 255 POTION FIX
				{
					item.m_Durability = 255.0f;
				}
			}
			else if ( _type != ITEMGET(13,10) && _type != ITEMGET(14,29) && _type != ITEMGET(14,153) && _type != ITEMGET(14,154) && _type != ITEMGET(14,155) && _type != ITEMGET(14,156) && _type != ITEMGET(14,100) && _type != ITEMGET(14,101) && _type != ITEMGET(14,110) && (_type != ITEMGET(14,21) && item.m_Level == 3) )
			{
				if ( item.m_Durability > item.m_BaseDurability )
				{
					item.m_Durability = item.m_BaseDurability;
				}
			}

			lpObj->pWarehouse[n].m_SkillOption = item.m_SkillOption;
			lpObj->pWarehouse[n].m_LuckOption = item.m_LuckOption;
			lpObj->pWarehouse[n].m_Z28Option = item.m_Z28Option;
			lpObj->pWarehouse[n].m_JewelOfHarmonyOption = item.m_JewelOfHarmonyOption;
			lpObj->pWarehouse[n].m_ItemOptionEx = item.m_ItemOptionEx;
			WORD hiword = MAKE_NUMBERW(lpMsg->dbItems[n*dbsize+DBI_SERIAL1], lpMsg->dbItems[n*dbsize+DBI_SERIAL2]);
			WORD loword = MAKE_NUMBERW(lpMsg->dbItems[n*dbsize+DBI_SERIAL3], lpMsg->dbItems[n*dbsize+DBI_SERIAL4]);
			item.m_Number = MAKE_NUMBERDW(hiword, loword);
			gObjWarehouseInsertItemPos(lpObj->m_Index, item, n, -1);
			gObjWarehouseItemSet(lpObj->m_Index, n, 1);
		}
	}

	pResult.h.c = 0xC3;
	pResult.h.headcode = 0x30;
	pResult.h.size = sizeof(PMSG_TALKRESULT);
	pResult.result = 2;

	DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);
	GCUserWarehouseSend(&gObj[aIndex]);

	if ( bCanWarehouseLock == TRUE )
	{
		if ( gObj[aIndex].WarehousePW > 0 )
		{
			if ( gObj[aIndex].WarehouseLock == 0 )
			{
				GCWarehouseStateSend(aIndex, 12);
			}
			else
			{
				GCWarehouseStateSend(aIndex, 1);
			}
		}
		else
		{
			GCWarehouseStateSend(aIndex, 0);
		}
	}

	lpObj->LoadWareHouseInfo = true;
	lpObj->m_ReqWarehouseOpen = false;
	gObjRequestPetItemInfo(lpObj->m_Index, 1);

#if (PACK_EDITION>=3)
#if (CRYSTAL_EDITION == 1)
	if(CrystalWall.CheckItemSerialOnOpenVault == 1)
	{
		for ( BYTE ItemPos = 0; ItemPos < ReadConfig.INVENTORY_SIZE(aIndex,true); ItemPos ++)
		{
			bool deletedItem = false;
			if ( lpObj->pInventory[ItemPos].IsItem() == TRUE)
			{			
				unsigned long serial = lpObj->pInventory[ItemPos].GetNumber();

				for (int i=0;i<ReadConfig.WAREHOUSE_SIZE(aIndex,false);i++)
				{
					if(gObjWarehouseSearchSerialNumber2(lpObj,serial,false) == 0)
					{
						if(CrystalWall.IsHaveSameSerialDeleteItems >= 2)
						{
							if(deletedItem == false)
							{
								deletedItem = true;
								gObjInventoryDeleteItem(lpObj->m_Index, ItemPos);
								GCInventoryItemDeleteSend(lpObj->m_Index, ItemPos, 1);
							}
						}

						if(CrystalWall.IsHaveSameSerialKickPlayer == 1)
						{
							gObjUserKill(lpObj->m_Index);
							return;
						}
					}
				}
			}
		}
		
		for (int i=0;i<ReadConfig.WAREHOUSE_SIZE(aIndex,false);i++)
		{
			unsigned long serial = lpObj->pWarehouse[i].GetNumber();
			if(gObjWarehouseSearchSerialNumber(lpObj,serial) == 0)
			{
				return;
			}
		}
	}
#endif
#endif
}

void DGGetWarehouseList(SDHP_GETWAREHOUSEDB_SAVE * lpMsg)
{
	char szId[11];
	int aIndex = 0;
	PMSG_TALKRESULT pResult;

	szId[MAX_ACCOUNT_LEN]=0;
	memcpy(szId, lpMsg->AccountID, sizeof(lpMsg->AccountID));
	aIndex = lpMsg->aIndex;

	if ( !gObjIsAccontConnect(aIndex, szId))
	{
		LogAddC(2, lMsg.Get(MSGGET(1, 175)), szId, aIndex);
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];
	int itype;
	int _type;
	BYTE OptionData;
	CItem item;

	memset(gObj[aIndex].pWarehouseMap, (BYTE)-1, ReadConfig.WAREHOUSE_SIZE(aIndex,true));
	gObj[aIndex].WarehouseMoney = lpMsg->Money;
	gObj[aIndex].WarehouseExNumLast = gObj[aIndex].WarehouseExNum;
	gObj[aIndex].WarehousePW = lpMsg->pw;
	int dbsize = 7;

	if ( lpMsg->DbVersion >= 3 )
	{
		dbsize = 16;
	}
	else if ( lpMsg->DbVersion >= 1 )
	{
		dbsize = 10;
	}

	for ( int n=0;n<ReadConfig.WAREHOUSE_SIZE(aIndex,false);n++)
	{
		itype = lpMsg->dbItems[n*dbsize];
		
		if ( dbsize == 16 )
		{
			itype |= (lpMsg->dbItems[n*dbsize+9] & 0xF0) << 5;
			itype |= (lpMsg->dbItems[n*dbsize+7] & 0x80) << 1;
			_type = itype;
		}
		else if ( dbsize == 10 )
		{
			if ( ((lpMsg->dbItems[n*dbsize+7] >> 7) & 0x01) )
			{
				itype += 256;
			}

			_type = (itype / 32 * MAX_SUBTYPE_ITEMS) + itype % 32;
		}
		else
		{
			_type = (itype / MAX_TYPE_ITEMS * MAX_SUBTYPE_ITEMS) + itype % 16;
		}

		gObj[aIndex].pWarehouse[n].Clear();

		if ( dbsize >= 16 )
		{
			if ( lpMsg->dbItems[n*dbsize] == (BYTE)-1 && (lpMsg->dbItems[n*dbsize+9] & 0xF0) == 0xF0 && (lpMsg->dbItems[n*dbsize+7] & 0x80) == 0x80)
			{
				itype = -1;
			}
		}
		else if ( dbsize >= 10 )
		{
			if ( lpMsg->dbItems[n*dbsize] == (BYTE)-1 && (lpMsg->dbItems[n*dbsize+7] & 0x80) == 0x80)
			{
				itype = -1;
			}
		}
		else
		{
			if ( lpMsg->dbItems[n*dbsize] == (BYTE)-1 )
			{
				itype = -1;
			}
		}

#if (DSGN_ITEM_DBMOVE == 1)
		if ( lpMsg->DbVersion < DS_Mover.highestDBNumber )
		{
			int newNumber = DS_Mover.CheckTransformItem(_type,lpMsg->DbVersion);

			if (newNumber > -1 && _type != newNumber)
			{
				DSGN_ITEMMOVE_LOG.Output("[ItemMove][Ware][%s][%s] Moving Item to new Db Number: oldId:%d, oldDb:%d, newId:%d, newDb:%d ",
					lpObj->AccountID,lpObj->Name,
					_type, lpMsg->DbVersion,
					newNumber, DS_Mover.highestDBNumber
				);

				item.m_Type = newNumber;
				_type = newNumber;
				itype = newNumber;
			}
		}
#endif

		if ( IsItem(_type) == FALSE )
		{
			itype = -1;
		}

		if ( _type == ITEMGET(14,17) || _type == ITEMGET(14,18) )
		{
			if ( lpMsg->dbItems[n*dbsize+2]== 0 )	// Level 0
			{
				_type = -1;	// #error maybe this is itype
			}
		}

		if ( _type == ITEMGET(14,19) )
		{
			if ( lpMsg->dbItems[n*dbsize+2] == (BYTE)-1 )
			{
				itype = -1;
			}
		}

		if ( itype != -1 )
		{
			item.m_Level = DBI_GET_LEVEL(lpMsg->dbItems[n*dbsize+1]);

			if ( lpMsg->DbVersion	< 2 )
			{
				lpMsg->dbItems[n*dbsize+8] = -1;
				lpMsg->dbItems[n*dbsize+9] = -1;
			}

			if(item.m_Level > MAX_ITEM_LEVEL)
			{
				DSGN_ITEMFIX_LOG.Output("[ItemFix][%s][%s] Item level too HIGH: t:%d, l:%d, z:%d, ex:%d, st:%d, sn:%d ",
					gObj[aIndex].AccountID,gObj[aIndex].Name,
					item.m_Type, item.m_Level, item.m_Z28Option, item.m_NewOption, item.m_SetOption, item.m_Number
				);
				item.m_Level = MAX_ITEM_LEVEL;
			}
			
			OptionData = lpMsg->dbItems[n*dbsize+1];
			item.m_SkillOption = DBI_GET_SKILL(OptionData);
			item.m_LuckOption = DBI_GET_LUCK(OptionData);
			item.m_Z28Option = DBI_GET_OPTION(OptionData);

			if ( dbsize >= 10 )
			{
				if ( _type == ITEMGET(13,3) ) // Dino
				{
					item.m_Z28Option |= DBI_GET_DINO_OPTION(lpMsg->dbItems[n*dbsize+7]);
				}
				else
				{
					if ( DBI_GET_OPTION16(lpMsg->dbItems[n*dbsize+7]) == 0x40 )
						item.m_Z28Option += 4;	// Set +28
				}
			}

			item.m_Durability = lpMsg->dbItems[n*dbsize+2];

			if ( dbsize >= 16 )	// DB Version3
			{
				item.m_JewelOfHarmonyOption = lpMsg->dbItems[n*dbsize+10];
			
				item.m_ItemOptionEx = DBI_GET_380OPTION(lpMsg->dbItems[n*dbsize+DBI_OPTION380_DATA]);

				if ( item.m_ItemOptionEx != 0 )
				{
					item.m_Type = itype;
					if ( g_kItemSystemFor380.Is380Item(&item)== FALSE )
					{
						item.m_ItemOptionEx = 0;

						LogAddTD("[380Item][%s][%s] Invalid 380 Item Option in Warehouse pos[%d]",
							lpObj->AccountID, lpObj->Name, n);
					}
				}

				if(IsSlotItem(_type))
				{
					item.m_ItemSlot1 = lpMsg->dbItems[n*dbsize+11];
					item.m_ItemSlot2 = lpMsg->dbItems[n*dbsize+12];
					item.m_ItemSlot3 = lpMsg->dbItems[n*dbsize+13];
					item.m_ItemSlot4 = lpMsg->dbItems[n*dbsize+14];
					item.m_ItemSlot5 = lpMsg->dbItems[n*dbsize+15];

					if(item.m_ItemSlot1 + item.m_ItemSlot2 + item.m_ItemSlot3 + item.m_ItemSlot4 +item.m_ItemSlot5 == 0)
					{
						if(ReadConfig.Soket__IsRecoverySlot == 1)
						{
							int dwExOpCount = getNumberOfExcOptions( DBI_GET_NOPTION(lpMsg->dbItems[n*dbsize+DBI_NOPTION_DATA]) );

							//DebugLog("[Slot Recovery][%s][%s] [WAR] %s Passed: %d Returned: %d",__FUNCTION__,gObj[aIndex].AccountID,gObj[aIndex].Name,DBI_GET_NOPTION(lpMsg->dbItems[n*dbsize+DBI_NOPTION_DATA]),dwExOpCount);

							if (dwExOpCount>=ReadConfig.Soket__MinExcOptionsCountOnItem)
							{
								switch(ReadConfig.Soket__SlotsToAdd)
								{
									case 1:
									{
										item.m_ItemSlot1 = 0xFF;
										lpMsg->dbItems[n*dbsize+11] = 0xFF;
									}break;
									case 2:
									{
										item.m_ItemSlot1 = 0xFF;
										item.m_ItemSlot2 = 0xFF;
										lpMsg->dbItems[n*dbsize+11] = 0xFF;
										lpMsg->dbItems[n*dbsize+12] = 0xFF;
									}break;
									case 3:
									{
										item.m_ItemSlot1 = 0xFF;
										item.m_ItemSlot2 = 0xFF;
										item.m_ItemSlot3 = 0xFF;
										lpMsg->dbItems[n*dbsize+11] = 0xFF;
										lpMsg->dbItems[n*dbsize+12] = 0xFF;
										lpMsg->dbItems[n*dbsize+13] = 0xFF;
									}break;
									case 4:
									{
										item.m_ItemSlot1 = 0xFF;
										item.m_ItemSlot2 = 0xFF;
										item.m_ItemSlot3 = 0xFF;
										item.m_ItemSlot4 = 0xFF;
										lpMsg->dbItems[n*dbsize+11] = 0xFF;
										lpMsg->dbItems[n*dbsize+12] = 0xFF;
										lpMsg->dbItems[n*dbsize+13] = 0xFF;
										lpMsg->dbItems[n*dbsize+14] = 0xFF;
									}break;
									case 5:
									{
										item.m_ItemSlot1 = 0xFF;
										item.m_ItemSlot2 = 0xFF;
										item.m_ItemSlot3 = 0xFF;
										item.m_ItemSlot4 = 0xFF;
										item.m_ItemSlot5 = 0xFF;
										lpMsg->dbItems[n*dbsize+11] = 0xFF;
										lpMsg->dbItems[n*dbsize+12] = 0xFF;
										lpMsg->dbItems[n*dbsize+13] = 0xFF;
										lpMsg->dbItems[n*dbsize+14] = 0xFF;
										lpMsg->dbItems[n*dbsize+15] = 0xFF;
									}break;
								}
							}
						}else
						{
							if(ReadConfig.Soket__AutoRemoveSameSocket == 1)
							{
								int Lvl1 = item.m_ItemSlot1 / 50;
								int Val1 = item.m_ItemSlot1 - (Lvl1 * 50);

								int Lvl2 = item.m_ItemSlot2 / 50;
								int Val2 = item.m_ItemSlot2 - (Lvl2 * 50);

								int Lvl3 = item.m_ItemSlot3 / 50;
								int Val3 = item.m_ItemSlot3 - (Lvl3 * 50);

								int Lvl4 = item.m_ItemSlot4 / 50;
								int Val4 = item.m_ItemSlot4 - (Lvl4 * 50);

								int Lvl5 = item.m_ItemSlot5 / 50;
								int Val5 = item.m_ItemSlot5 - (Lvl5 * 50);

								if(item.m_ItemSlot1 != 0xFF && item.m_ItemSlot1 != 0)
								{
									if(Val1 == Val2 || Val1 == Val3 || Val1 == Val4 || Val1 == Val5)
									{
										Val1=0;
										item.m_ItemSlot1 = 0xFF;
									}
								}
								if(item.m_ItemSlot2 != 0xFF && item.m_ItemSlot2 != 0)
								{
									if(Val2 == Val1 || Val2 == Val3 || Val2 == Val4 || Val2 == Val5)
									{
										Val2=0;
										item.m_ItemSlot2 = 0xFF;
									}
								}
								if(item.m_ItemSlot3 != 0xFF && item.m_ItemSlot3 != 0)
								{
									if(Val3 == Val1 || Val3 == Val2 || Val3 == Val4 || Val3 == Val5)
									{
										Val3=0;
										item.m_ItemSlot3 = 0xFF;
									}
								}
								if(item.m_ItemSlot4 != 0xFF && item.m_ItemSlot4 != 0)
								{
									if(Val4 == Val1 || item.m_ItemSlot4 == Val2 || Val4 == Val3 || Val4 == Val5)
									{
										Val4=0;
										item.m_ItemSlot4 = 0xFF;
									}
								}
								if(item.m_ItemSlot5 != 0xFF && item.m_ItemSlot5 != 0)
								{
									if(Val5 == Val1 || Val5 == Val2 || Val5 == Val3 || Val5 == Val4)
									{
										Val5=0;
										item.m_ItemSlot5 = 0xFF;
									}
								}
							}
						}
					}
				} else {
					item.m_ItemSlot1 = 0;
					item.m_ItemSlot2 = 0;
					item.m_ItemSlot3 = 0;
					item.m_ItemSlot4 = 0;
					item.m_ItemSlot5 = 0;
				}
			}

			if ( lpMsg->DbVersion == 0 )
			{
				item.Convert(itype, item.m_SkillOption, item.m_LuckOption, item.m_Z28Option, 0, 0, 0, 0);
			}
			else if ( lpMsg->DbVersion <= 2 )
			{
				item.Convert(itype, item.m_SkillOption, item.m_LuckOption, item.m_Z28Option, DBI_GET_NOPTION(lpMsg->dbItems[n*dbsize+DBI_NOPTION_DATA]), lpMsg->dbItems[n*dbsize+DBI_SOPTION_DATA], item.m_ItemOptionEx, 1);
			}
			else
			{
				item.Convert(itype, item.m_SkillOption, item.m_LuckOption, item.m_Z28Option, DBI_GET_NOPTION(lpMsg->dbItems[n*dbsize+DBI_NOPTION_DATA]), lpMsg->dbItems[n*dbsize+DBI_SOPTION_DATA], item.m_ItemOptionEx, 3);
			}

#if (DSGN_ITEM_MONITOR==1)

				if (item.m_Type >= ITEMGET(12,0))
				{
					//Rings
					if ( ((item.m_Type >= ITEMGET(13,8))&&(item.m_Type <= ITEMGET(13,10))) ||
							((item.m_Type >= ITEMGET(13,12))&&(item.m_Type <= ITEMGET(13,13))) ||
							((item.m_Type >= ITEMGET(13,20))&&(item.m_Type <= ITEMGET(13,28))) ||
							item.m_Type == ITEMGET(13,107))
					{
						if ( getNumberOfExcOptions(item.m_NewOption) > DSGN_ITEM_MON_RING_EXC )
						{
							DSGN_RINGFIX_LOG.Output("[ItemFix][Before][%s][%s] Ring/Pendant exc options fix: t:%d, l:%d, z:%d, ex:%d, st:%d, sn:%d ",
								gObj[aIndex].AccountID,gObj[aIndex].Name,
								item.m_Type, item.m_Level, item.m_Z28Option, item.m_NewOption, item.m_SetOption, item.m_Number
							);

							#if (DSGN_ITEM_FIX_RING==1)
								item.m_NewOption = BoxExcOptions(DSGN_ITEM_MON_RING_EXC);

								DSGN_RINGFIX_LOG.Output("[ItemFix][After][%s][%s] Ring/Pendant exc options fix: t:%d, l:%d, z:%d, ex:%d, st:%d, sn:%d ",
									gObj[aIndex].AccountID,gObj[aIndex].Name,
									item.m_Type, item.m_Level, item.m_Z28Option, item.m_NewOption, item.m_SetOption, item.m_Number
								);
							#endif
						}

						if(item.m_Level > DSGN_ITEM_MON_RING_LVL)
						{
							DSGN_RINGFIX_LOG.Output("[ItemFix][%s][%s] Ring/Pendant level fix: t:%d, l:%d, z:%d, ex:%d, st:%d, sn:%d ",
								gObj[aIndex].AccountID,gObj[aIndex].Name,
								item.m_Type, item.m_Level, item.m_Z28Option, item.m_NewOption, item.m_SetOption, item.m_Number
							);

							#if (DSGN_ITEM_FIX_RING==1)
								item.m_Level = DSGN_ITEM_MON_RING_LVL;
							#endif
						}

						if(item.m_Z28Option > DSGN_ITEM_MON_RING_Z28)
						{
							DSGN_RINGFIX_LOG.Output("[ItemFix][%s][%s] Ring/Pendant ZOption fix: t:%d, l:%d, z:%d, ex:%d, st:%d, sn:%d ",
								gObj[aIndex].AccountID,gObj[aIndex].Name,
								item.m_Type, item.m_Level, item.m_Z28Option, item.m_NewOption, item.m_SetOption, item.m_Number
							);

							#if (DSGN_ITEM_FIX_RING==1)
								item.m_Z28Option = DSGN_ITEM_MON_RING_Z28;
							#endif
						}
					}

					//Wings
					if ( (item.m_Type >= ITEMGET(12,0) && item.m_Type <= ITEMGET(12,6) ) ||
						 (item.m_Type >= ITEMGET(12,36) && item.m_Type <= ITEMGET(12,43) ) ||
						 (item.m_Type >= ITEMGET(12,130) && item.m_Type <= ITEMGET(12,135) ) ||
						  item.m_Type == ITEMGET(13,30) ||
						  item.m_Type == ITEMGET(12,49) ||
						  item.m_Type == ITEMGET(12,50) ||
						  item.m_Type == ITEMGET(13,3)  ||
						  item.m_Type == ITEMGET(13,37) 		
							#if (CRYSTAL_EDITION == 1)
							|| ( item.m_Type >= ITEMGET(12,200) && item.m_Type <= ITEMGET(12,254) )
							#endif
						)
					{
						if ( getNumberOfExcOptions(item.m_NewOption) > DSGN_ITEM_MON_WING_EXC )
						{
							DSGN_WINGFIX_LOG.Output("[ItemFix][Before][%s][%s] Wings/Other exc options fix: t:%d, l:%d, z:%d, ex:%d, st:%d, sn:%d ",
								gObj[aIndex].AccountID,gObj[aIndex].Name,
								item.m_Type, item.m_Level, item.m_Z28Option, item.m_NewOption, item.m_SetOption, item.m_Number
							);

							#if (DSGN_ITEM_FIX_WING==1)
								item.m_NewOption = BoxExcOptions(DSGN_ITEM_MON_WING_EXC);

								DSGN_WINGFIX_LOG.Output("[ItemFix][After][%s][%s] Wings/Other exc options fix: t:%d, l:%d, z:%d, ex:%d, st:%d, sn:%d ",
									gObj[aIndex].AccountID,gObj[aIndex].Name,
									item.m_Type, item.m_Level, item.m_Z28Option, item.m_NewOption, item.m_SetOption, item.m_Number
								);
							#endif
						}
					}
				}
#endif

			if ( _type == ITEMGET(14,7) ) // Siege Potion
			{

			}
			else if ( (_type >= ITEMGET(14,0)  && _type <= ITEMGET(14,8)) || 
					  (_type >= ITEMGET(14,35) && _type <= ITEMGET(14,40))||	
					  (_type >= ITEMGET(14,70) && _type <= ITEMGET(14,71))||	//Elite Mana/Healing Potions
					  (_type == ITEMGET(14,94)) ||								//Elite Medium Healing Potion
					  (_type == ITEMGET(14,133))								//Elite SD potion
					  )
			{
				if ( item.m_Durability == 0.0f )
				{
					item.m_Durability = 1.0f;
				}

				if ( item.m_Durability > 255.0f )	// #3.0 error HERE FIX to 255 POTION FIX
				{
					item.m_Durability = 255.0f;
				}
			}
			else if ( _type != ITEMGET(13,10) && _type != ITEMGET(14,29) && _type != ITEMGET(14,100) && _type != ITEMGET(14,153) && _type != ITEMGET(14,154) && _type != ITEMGET(14,155) && _type != ITEMGET(14,156) && _type != ITEMGET(14,101) && _type != ITEMGET(14,110) && (_type != ITEMGET(14,21) && item.m_Level == 3) )
			{
				if ( item.m_Durability > item.m_BaseDurability )
				{
					item.m_Durability = item.m_BaseDurability;
				}
			}

			lpObj->pWarehouse[n].m_SkillOption = item.m_SkillOption;
			lpObj->pWarehouse[n].m_LuckOption = item.m_LuckOption;
			lpObj->pWarehouse[n].m_Z28Option = item.m_Z28Option;
			lpObj->pWarehouse[n].m_JewelOfHarmonyOption = item.m_JewelOfHarmonyOption;
			lpObj->pWarehouse[n].m_ItemOptionEx = item.m_ItemOptionEx;
			WORD hiword = MAKE_NUMBERW(lpMsg->dbItems[n*dbsize+DBI_SERIAL1], lpMsg->dbItems[n*dbsize+DBI_SERIAL2]);
			WORD loword = MAKE_NUMBERW(lpMsg->dbItems[n*dbsize+DBI_SERIAL3], lpMsg->dbItems[n*dbsize+DBI_SERIAL4]);
			item.m_Number = MAKE_NUMBERDW(hiword, loword);
			gObjWarehouseInsertItemPos(lpObj->m_Index, item, n, -1);
			gObjWarehouseItemSet(lpObj->m_Index, n, 1);
		}
	}

	pResult.h.c = 0xC3;
	pResult.h.headcode = 0x30;
	pResult.h.size = sizeof(PMSG_TALKRESULT);
	pResult.result = 2;

	DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);
	GCUserWarehouseSend(&gObj[aIndex]);

	if ( bCanWarehouseLock == TRUE )
	{
		if ( gObj[aIndex].WarehousePW > 0 )
		{
			if ( gObj[aIndex].WarehouseLock == 0 )
			{
				GCWarehouseStateSend(aIndex, 12);
			}
			else
			{
				GCWarehouseStateSend(aIndex, 1);
			}
		}
		else
		{
			GCWarehouseStateSend(aIndex, 0);
		}
	}

	lpObj->LoadWareHouseInfo = true;
	lpObj->m_ReqWarehouseOpen = false;
	gObjRequestPetItemInfo(lpObj->m_Index, 1);
}


void GDGetWarehouseNoItem( SDHP_GETWAREHOUSEDB_RESULT * lpMsg)
{
	char szId[11];
	int aIndex = lpMsg->aIndex;
	PMSG_TALKRESULT pResult;

	szId[MAX_ACCOUNT_LEN] = 0;
	memcpy(szId, lpMsg->AccountID, sizeof(lpMsg->AccountID));

	if ( !gObjIsAccontConnect(aIndex, szId) )
	{
		LogAddC(2, lMsg.Get(MSGGET(1, 175)), szId, aIndex);
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];
	
	if ( lpObj->m_IfState.type == 6 )
	{
		if ( lpObj->m_IfState.state == 0 )
		{
			lpObj->m_IfState.state = 1;
			lpObj->WarehouseSave = 1;
			pResult.h.c = 0xC3;
			pResult.h.headcode = 0x30;
			pResult.h.size = sizeof(pResult);
			pResult.result = 2;

			DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);
			LogAddC(2, lMsg.Get(MSGGET(1, 176)), lpObj->Name);
		}
	}

	for (int i=0;i<ReadConfig.WAREHOUSE_SIZE(aIndex,false);i++)
	{
		lpObj->pWarehouse[i].Clear();
	}

	lpObj->LoadWareHouseInfo = true;
	lpObj->m_ReqWarehouseOpen = false;
	
	//gObj[aIndex].pWarehouse = new CItem[WAREHOUSE_SIZE];
	//gObj[aIndex].pWarehouseMap = new BYTE[WAREHOUSE_SIZE];
	//memset(gObj[aIndex].pWarehouse, (BYTE)-1, WAREHOUSE_SIZE);
	//memset(gObj[aIndex].pWarehouseMap, (BYTE)-1, WAREHOUSE_SIZE);
}

void GDSetWarehouseList(int aIndex)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

#if (PACK_EDITION>=2)
	if(ReadConfig.UseGuildWare == 1 && gObj[aIndex].MapNumber == ReadConfig.GuildWareMap)
	{
		if ( gObj[aIndex].WarehouseSave == 1 )
		{
			GDSetGuildWarehouseList(aIndex);
			gObj[aIndex].WarehouseTimeCount = 3;
			return;
		}else
		{
			goto closeware;
		}
	}

	if(ReadConfig.WareSystem > 0)
	{
		if(gObj[aIndex].WarehouseExNum > 0 || ReadConfig.S6E2 == 1)
		{
			if ( gObj[aIndex].WarehouseSave == 1 )
			{
				GDSetWarehouseListEx(aIndex);
				if(ReadConfig.WareSystem > 0)
					gObj[aIndex].WarehouseTimeCount = 3;
				return;
			}else
			{
				goto closeware;
			}
		}else
		{
			if ( gObj[aIndex].WarehouseSave != 1 )
			{
				closeware:
				LogAddTD("[%s][%s] WareHouse Save Fail : WarehouseSave = 0",
					gObj[aIndex].AccountID, gObj[aIndex].Name);
				return;
			}
		}
	}
#endif

	if(ReadConfig.S6E2 == 0)
	{
		SDHP_GETWAREHOUSEDB_SAVE pMsg;
	
		if ( gObj[aIndex].LoadWareHouseInfo == false )
		{
			LogAddTD("[%s][%s] WareHouse Save Fail : Not Open",
				gObj[aIndex].AccountID, gObj[aIndex].Name);

			return;
		}

		memcpy(pMsg.AccountID, gObj[aIndex].AccountID, sizeof(pMsg.AccountID));
		pMsg.aIndex = aIndex;
		pMsg.h.c = 0xC2;
		pMsg.h.headcode = 0x09;
		pMsg.h.sizeH = HIBYTE(sizeof(pMsg));
		pMsg.h.sizeL = LOBYTE(sizeof(pMsg));
		
#if (DSGN_ITEM_DBMOVE == 1)
		pMsg.DbVersion = DS_Mover.highestDBNumber;
#else
		pMsg.DbVersion = 3;
#endif

		pMsg.Money = gObj[aIndex].WarehouseMoney;
		pMsg.pw = gObj[aIndex].WarehousePW;

		memset(pMsg.dbItems,0xFF,sizeof(pMsg.dbItems));
		ItemByteConvert16(pMsg.dbItems, gObj[aIndex].pWarehouse, ReadConfig.WAREHOUSE_SIZE(aIndex,false));

		cDBSMng.Send((PCHAR)&pMsg, sizeof(pMsg));
	} else {
		SDHP_GETWAREHOUSEDB_S6E2_SAVE pMsg;
	
		if ( gObj[aIndex].LoadWareHouseInfo == false )
		{
			LogAddTD("[%s][%s] WareHouse Save Fail : Not Open",
				gObj[aIndex].AccountID, gObj[aIndex].Name);

			return;
		}

		memcpy(pMsg.AccountID, gObj[aIndex].AccountID, sizeof(pMsg.AccountID));
		pMsg.aIndex = aIndex;
		pMsg.h.c = 0xC2;
		pMsg.h.headcode = 0x09;
		pMsg.h.sizeH = HIBYTE(sizeof(pMsg));
		pMsg.h.sizeL = LOBYTE(sizeof(pMsg));
		pMsg.Money = gObj[aIndex].WarehouseMoney;
		pMsg.pw = gObj[aIndex].WarehousePW;

#if (DSGN_ITEM_DBMOVE == 1)
		pMsg.DbVersion = DS_Mover.highestDBNumber;
#else
		pMsg.DbVersion = 3;
#endif

		memset(pMsg.dbItems,0xFF,sizeof(pMsg.dbItems));
		ItemByteConvert16(pMsg.dbItems, gObj[aIndex].pWarehouse, ReadConfig.WAREHOUSE_SIZE(aIndex,false));

		cDBSMng.Send((PCHAR)&pMsg, sizeof(pMsg));
	}

	gObjSavePetItemInfo(aIndex, 1);
	gObjWarehouseTextSave(&gObj[aIndex]);
	//gObj[aIndex].pWarehouse = new CItem[WAREHOUSE_SIZE];
	//gObj[aIndex].pWarehouseMap = new BYTE[WAREHOUSE_SIZE];
	//memset(gObj[aIndex].pWarehouse, (BYTE)-1, WAREHOUSE_SIZE);
	//memset(gObj[aIndex].pWarehouseMap, (BYTE)-1, WAREHOUSE_SIZE);

	gObj[aIndex].b_BlockMoveCommandSeconds = 30;

	if(ReadConfig.WareSystem > 0 || ReadConfig.UseGuildWare == 1)
		gObj[aIndex].WarehouseTimeCount = 3;
	else
		if(ReadConfig.Ware3Secs == 1)
			gObj[aIndex].WarehouseTimeCount = 3;
}


/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu DB Char Item Save Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC2
 *	HeadCode  : 0x11
 */
struct SDHP_DBCHAR_ITEMSAVE
{
	PWMSG_HEAD h;	// C2:11
	char Name[10];	// 4
	BYTE dbInventory[3776];	// E
};


void GDUserItemSave(LPOBJ lpObj)
{
	SDHP_DBCHAR_ITEMSAVE pMsg;

	pMsg.h.c = 0xC2;
	pMsg.h.headcode = 0x11;
	pMsg.h.sizeH = HIBYTE(sizeof(pMsg));
	pMsg.h.sizeL = LOBYTE(sizeof(pMsg));
	memcpy(pMsg.Name, lpObj->Name, sizeof(pMsg.Name));
	ItemByteConvert16(pMsg.dbInventory, lpObj->pInventory, ReadConfig.INVENTORY_SIZE(lpObj->m_Index,false));

	cDBSMng.Send((PCHAR)&pMsg, sizeof(pMsg));
}




/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Warehouse Money Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC1
 *	HeadCode  : 0x12
 */
struct SDHP_WAREHOUSEMONEY_SAVE
{
	PBMSG_HEAD h;	// C1:12
	char AccountID[10];	// 3
	short aIndex;	// E
	int Money;	// 10
};

struct SDHP_WAREHOUSEMONEYEX_SAVE
{
	PBMSG_HEAD h;	// C1:12
	char AccountID[10];	// 3
	short aIndex;	// E
	int Money;	// 10
	int ExNum;
};


void GDSetWarehouseMoney(int aIndex)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	if(ReadConfig.WareSystem == 0)
	{
		SDHP_WAREHOUSEMONEY_SAVE pMsg;

		memcpy(pMsg.AccountID, gObj[aIndex].AccountID, sizeof(pMsg.AccountID));
		pMsg.aIndex = aIndex;
		pMsg.h.c = 0xC1;
		pMsg.h.headcode = 0x12;
		pMsg.h.size = sizeof(pMsg);
		pMsg.Money = gObj[aIndex].WarehouseMoney;

		cDBSMng.Send((PCHAR)&pMsg, sizeof(pMsg));
	}else
	{		
		SDHP_WAREHOUSEMONEYEX_SAVE pMsg;

		memcpy(pMsg.AccountID, gObj[aIndex].AccountID, sizeof(pMsg.AccountID));
		pMsg.aIndex = aIndex;
		pMsg.h.c = 0xC1;
		pMsg.h.headcode = SCFExDB_GSSend_SetWarehouseMoneyEx;
		pMsg.h.size = sizeof(pMsg);
		pMsg.Money = gObj[aIndex].WarehouseMoney;
		pMsg.ExNum = gObj[aIndex].WarehouseExNum;

		cDBSMng.Send((PCHAR)&pMsg, sizeof(pMsg));
	}
}

void GDGameServerInfoSave()
{
	return;		// GHere goes a MACRO
}




/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Item Serial Create Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC1
 *	HeadCode  : 0x52, 0x55 (in Case of PET )
 */
struct SDHP_ITEMCREATE
{
	PBMSG_HEAD h;	// C1:XX
	BYTE x;	// 3
	BYTE y;	// 4
	BYTE MapNumber;	// 5 [235:Mu_2nd_Aniv], [236:CashShop]
	short Type;	// 6
	BYTE Level;	// 8
	BYTE Dur;	// 9
	BYTE Op1;	// A
	BYTE Op2;	// B
	BYTE Op3;	// C
	BYTE NewOption;	// D
	int aIndex;	// 10
	short lootindex;	// 14
	BYTE SetOption;	// 16
};



// MapNumber [235:Mu_2nd_Aniv], [236:CashShop]
void ItemSerialCreateSend(int aIndex, BYTE MapNumber, BYTE x, BYTE y, int type, BYTE level, BYTE dur, BYTE Op1, BYTE Op2, BYTE Op3, int LootIndex, BYTE NewOption, BYTE SetOption)
{
	SDHP_ITEMCREATE  pMsg;

	pMsg.h.c = 0xC1;
	pMsg.h.headcode = 0x52;
	pMsg.h.size = sizeof(pMsg);
	pMsg.MapNumber = MapNumber;
	pMsg.x = x;
	pMsg.y = y;
	pMsg.Type = type;
	pMsg.Level = level;

	if (dur == 0 )
	{
		dur = ItemGetDurability(type, level, NewOption, SetOption);
	}

	pMsg.Dur = dur;
	pMsg.Op1 = Op1;
	pMsg.Op2 = Op2;
	pMsg.Op3 = Op3;
	pMsg.NewOption = NewOption;
	pMsg.aIndex = aIndex;
	pMsg.lootindex = LootIndex;
	pMsg.SetOption = SetOption;

	if ( OBJMAX_RANGE(aIndex) != FALSE  && gObj[aIndex].Type != OBJ_USER )
	{
		BYTE NewOption[MAX_EXOPTION_SIZE];

		for ( int i=0;i<MAX_EXOPTION_SIZE;i++)
		{
			NewOption[i] = 0;
		}

		if ( (pMsg.NewOption & 0x20) != 0 )
		{
			NewOption[0] = TRUE;
		}
		if ( (pMsg.NewOption & 0x10) != 0 )
		{
			NewOption[1] = TRUE;
		}

		if ( (pMsg.NewOption & 0x8) != 0 )
		{
			NewOption[2] = TRUE;
		}
		if ( (pMsg.NewOption & 0x4) != 0 )
		{
			NewOption[3] = TRUE;
		}

		if ( (pMsg.NewOption & 0x2) != 0 )
		{
			NewOption[4] = TRUE;
		}
		if ( (pMsg.NewOption & 0x1) != 0 )
		{
			NewOption[5] = TRUE;
		}

		/*LogAddTD("Monster Item Drop Request [%s] [%d][%d][%d] : [%s][%d][%d][%d][%d] Ex:[%d,%d,%d,%d,%d,%d,%d] Set:[%d]",
			gObj[aIndex].Name, MapNumber, pMsg.x, pMsg.y, ItemAttribute[pMsg.Type].Name,
			pMsg.Level, pMsg.Op1,pMsg.Op2, pMsg.Op3, NewOption[0], NewOption[1], NewOption[2],
			NewOption[3], NewOption[4],	NewOption[5], NewOption[6], pMsg.SetOption);*/
	}

	cDBSMng.Send( (char*)&pMsg, pMsg.h.size);
}





void PetItemSerialCreateSend(int aIndex, BYTE MapNumber, BYTE x, BYTE y, int type, BYTE level, BYTE dur, BYTE Op1, BYTE Op2, BYTE Op3, int LootIndex, BYTE NewOption, BYTE SetOption)
{
	SDHP_ITEMCREATE  pMsg;

	pMsg.h.c = 0xC1;
	pMsg.h.headcode = 0x55;
	pMsg.h.size = sizeof(pMsg);
	pMsg.MapNumber = MapNumber;
	pMsg.x = x;
	pMsg.y = y;
	pMsg.Type = type;
	pMsg.Level = level;

	if (dur == 0 )
	{
		dur = ItemGetDurability(type, level, NewOption, SetOption);
	}

	pMsg.Dur = dur;
	pMsg.Op1 = Op1;
	pMsg.Op2 = Op2;
	pMsg.Op3 = Op3;
	pMsg.NewOption = NewOption;
	pMsg.aIndex = aIndex;
	pMsg.lootindex = LootIndex;
	pMsg.SetOption = SetOption;

	cDBSMng.Send( (char*)&pMsg, pMsg.h.size);
}


void ItemSerialCreateRecv(SDHP_ITEMCREATERECV * lpMsg)
{
	LPOBJ lpObj;
	int mapnumber = lpMsg->MapNumber;
	int aIndex = lpMsg->aIndex;
	int lootindex = lpMsg->lootindex;

	if ( mapnumber != (BYTE)-1)
	{
		if ( mapnumber < 0 || (mapnumber > MAX_MAP_NUMBER-1 && mapnumber < 235) )
		{
			return;
		}
	}

	if ( lpMsg->MapNumber == (BYTE)-1 || lpMsg->MapNumber == (BYTE)-2 )
	{
		if ( gObjIsConnectedGP(aIndex) == FALSE )
		{
			LogAddC(2, "error-L3 [%s][%d]", __FILE__, __LINE__);
			return;
		}
		
		lpObj = &gObj[aIndex];

		if ( lpMsg->MapNumber == (BYTE)-2 )
		{
			if ( lpObj->m_IfState.type != 13 )
			{
				CHAOS_LOG.Output("[%s][%s] error-L2: DarkTrainerBox not used.", lpObj->AccountID, lpObj->Name);
				return;
			}
		}
		else if ( lpObj->m_IfState.type != 7 )
		{
			CHAOS_LOG.Output("[%s][%s] error-L2: ChaosBox not used.", lpObj->AccountID, lpObj->Name);
			return;
		}
		
		PMSG_CHAOSMIXRESULT pMsg;

		pMsg.h.c = 0xC1;
		pMsg.h.headcode = 0x86;
		pMsg.h.size = sizeof(PMSG_CHAOSMIXRESULT);

		if ( lpMsg->MapNumber == (BYTE)-2 )
		{
			pMsg.Result = 100;
		}
		else
		{
			pMsg.Result = 1;
		}

		CItem NewItem;

		NewItem.m_Level = lpMsg->Level;
		NewItem.m_Durability = ItemGetDurability(lpMsg->Type, lpMsg->Level, lpMsg->NewOption, lpMsg->SetOption);

		if ( lpMsg->Type == ITEMGET(14,7) ) // Siege Potion
		{
			NewItem.m_Durability = lpMsg->Dur;
		}

		if ( lpMsg->Type == ITEMGET(13,37) )
		{
			NewItem.m_Durability = lpMsg->Dur;
		}

		NewItem.m_ItemSlot1 = 0;
		NewItem.m_ItemSlot2 = 0;
		NewItem.m_ItemSlot3 = 0;
		NewItem.m_ItemSlot4 = 0;
		NewItem.m_ItemSlot5 = 0;

		NewItem.Convert(lpMsg->Type,lpMsg->Op1, lpMsg->Op2, lpMsg->Op3,lpMsg->NewOption, lpMsg->SetOption,0, CURRENT_DB_VERSION);
		ItemByteConvert(pMsg.ItemInfo, NewItem);
		NewItem.m_Number = lpMsg->m_Number;
		ChaosBoxInit(lpObj);
		gObjChaosBoxInsertItemPos(aIndex, NewItem, 0, -1);
		gObjChaosItemSet(aIndex, 0, 1);
		BYTE ExOption[MAX_EXOPTION_SIZE];
		ItemIsBufExOption(ExOption, &NewItem);

		CHAOS_LOG.Output("[%s][%s] CBMix Item Create Item:%s [%d][%d] [%d][%d][%d][%d] Ex:[%d,%d,%d,%d,%d,%d,%d] Set:[%d]",
			lpObj->AccountID, lpObj->Name, NewItem.GetName(), lpMsg->m_Number, lpMsg->Dur, NewItem.m_Level, 
			NewItem.m_SkillOption, NewItem.m_LuckOption, NewItem.m_Z28Option, ExOption[0], ExOption[1],
			ExOption[2], ExOption[3], ExOption[4], ExOption[5], ExOption[6], lpMsg->SetOption);

		DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
		gObj[aIndex].ChaosLock = FALSE;

		return;
	}

	if ( lpMsg->MapNumber == 235 )
	{
		if ( gObj[lpMsg->aIndex].Connected > PLAYER_CONNECTED )
		{
			int iType  = ITEM_GET_TYPE(lpMsg->Type);
			int iTypeIndex = ITEM_GET_INDEX(lpMsg->Type);

			BYTE iItemPos = gObjInventoryInsertItem(&gObj[lpMsg->aIndex], iType, iTypeIndex, lpMsg->Level, lpMsg->m_Number, lpMsg->Dur,0,0,0,0,0);


			if ( iItemPos == (BYTE)-1 )
			{
				LogAddTD("[Mu_2Anv_Event] Error : Failed To Gift Item Type:%d, TypeIndex:%d to [%s][%s]",
					iType, iTypeIndex, gObj[lpMsg->aIndex].AccountID, gObj[lpMsg->aIndex].Name);
				
			
			}
			else
			{
				gObj[lpMsg->aIndex].pInventory[iItemPos].Convert(lpMsg->Type,lpMsg->Op1, lpMsg->Op2, lpMsg->Op3,lpMsg->NewOption, lpMsg->SetOption,0, CURRENT_DB_VERSION);
				::GCInventoryItemOneSend(lpMsg->aIndex, iItemPos);
			}

		}
	}

	if ( lpMsg->MapNumber == 236 )
	{
		if ( gObj[lpMsg->aIndex].Connected > PLAYER_CONNECTED )
		{
			CItem pCreateItem;
			int iItemType = ITEM_GET_TYPE(lpMsg->Type);
			int iItemIndex	= ITEM_GET_INDEX(lpMsg->Type);

			pCreateItem.Convert(ITEMGET(iItemType, iItemIndex), lpMsg->Op1, lpMsg->Op2,
				lpMsg->Op3, lpMsg->NewOption, lpMsg->SetOption, 0, 3);
			pCreateItem.m_Level = lpMsg->Level;
			pCreateItem.m_Durability = lpMsg->Dur;

			BYTE btItemPos = gObjInventoryInsertItem(lpMsg->aIndex, pCreateItem);

			if ( btItemPos == (BYTE)-1 )
			{
				LogAddTD("[CashShop] Error : Failed To Insert Item Type:%d, TypeIndex:%d to [%s][%s]",
					iItemType, iItemIndex, gObj[lpMsg->aIndex].AccountID, gObj[lpMsg->aIndex].Name);
			}
			else
			{
				GCInventoryItemOneSend(lpMsg->aIndex, btItemPos);
			}
		}
	}
	else
	{
		int iRetMapNumber = lpMsg->MapNumber;
		BOOL BC8QItem = FALSE;

		if ( lpMsg->MapNumber >= 238 && lpMsg->MapNumber <= 244 )
		{
			mapnumber -= 227;
		}

		if ( lpMsg->MapNumber >= 247 && lpMsg->MapNumber <= 253 )
		{
			mapnumber -= 236;
		}

		int iItemCount = MapC[mapnumber].MonsterItemDrop(lpMsg->Type, lpMsg->Level, lpMsg->Dur, lpMsg->x,
			lpMsg->y, lpMsg->Op1, lpMsg->Op2, lpMsg->Op3, lpMsg->NewOption, lpMsg->SetOption,
			lootindex, lpMsg->m_Number, 0,0,0,0,0,0);

		if ( lpMsg->MapNumber == MAP_INDEX_BLOODCASTLE8)
		{
			if(MapC[mapnumber].m_cItem[iItemCount].m_Type ==  ItemGetNumberMake(13,19))
			{
				BC8QItem = 1;
			}
		}

		if ( iItemCount != -1 )
		{
			if ( iRetMapNumber >= 238 && iRetMapNumber <= 244)
			{
				MapC[mapnumber].m_cItem[iItemCount].m_Time = GetTickCount() + 300000;
				MapC[mapnumber].m_cItem[iItemCount].m_LootTime = GetTickCount() + 20000;
			}

			if (( iRetMapNumber >= 247 && iRetMapNumber <= 253) || (BC8QItem == 1))
			{
				MapC[mapnumber].m_cItem[iItemCount].m_Time = GetTickCount() + 900000;
				MapC[mapnumber].m_cItem[iItemCount].m_LootTime = GetTickCount() + 10000;
				if(mapnumber != 52)
					g_BloodCastle.m_BridgeData[iRetMapNumber-247].m_nBC_QUESTITEM_SERIAL = lpMsg->m_Number;
				else
					g_BloodCastle.m_BridgeData[7].m_nBC_QUESTITEM_SERIAL = lpMsg->m_Number;
			}
		}

		//Soket Options Drop
		BYTE NewSlotOption[5];

		for(int i=0;i<5;i++)
		{
			NewSlotOption[i] = 0x00;
		}

		MapC[mapnumber].m_cItem[iItemCount].m_ItemSlot1 = NewSlotOption[0];
		MapC[mapnumber].m_cItem[iItemCount].m_ItemSlot2 = NewSlotOption[1];
		MapC[mapnumber].m_cItem[iItemCount].m_ItemSlot3 = NewSlotOption[2];
		MapC[mapnumber].m_cItem[iItemCount].m_ItemSlot4 = NewSlotOption[3];
		MapC[mapnumber].m_cItem[iItemCount].m_ItemSlot5 = NewSlotOption[4];

		if (IsSlotItem(lpMsg->Type)==1)
		{
			int Dif = ReadConfig.Soket__MaxSoketsOnItem - ReadConfig.Soket__MinSoketsOnItem;

			BYTE MaxSlots = 0;
			if(Dif <= 0)
				MaxSlots = ReadConfig.Soket__MinSoketsOnItem;
			else
				MaxSlots = rand() % Dif + ReadConfig.Soket__MinSoketsOnItem;

			if(MaxSlots > 5)
				MaxSlots = 5;

			if(ReadConfig.SocketDropRate[mapnumber] > 0)
			{
				if (rand()%100<=ReadConfig.SocketDropRate[mapnumber])
				{
					if (MaxSlots>=1) {
						MapC[mapnumber].m_cItem[iItemCount].m_ItemSlot1 = 0xFF;
						NewSlotOption[0] = 0xFF;
					}
					if (MaxSlots>=2) {
						MapC[mapnumber].m_cItem[iItemCount].m_ItemSlot2 = 0xFF;
						NewSlotOption[1] = 0xFF;
					}
					if (MaxSlots>=3) {
						MapC[mapnumber].m_cItem[iItemCount].m_ItemSlot3 = 0xFF;
						NewSlotOption[2] = 0xFF;
					}
					if (MaxSlots>=4) {
						MapC[mapnumber].m_cItem[iItemCount].m_ItemSlot4 = 0xFF;
						NewSlotOption[3] = 0xFF;
					}
					if (MaxSlots>=5) {
						MapC[mapnumber].m_cItem[iItemCount].m_ItemSlot5 = 0xFF;
						NewSlotOption[4] = 0xFF;
					}					
				} else 
				{
					MaxSlots = 0;
				}
			}


			if (MaxSlots > 0)
			{
				if (ReadConfig.Soket__EnableExcSocketItemsDrop)
				{
					if (rand()%100 <= ReadConfig.Soket__ExcAdditionalPercentOnSoketItem)
					{
						lpMsg->NewOption = BoxExcOptions(ReadConfig.Soket__ExcMaximumOptionsOnSoketItem);
						MapC[mapnumber].m_cItem[iItemCount].m_NewOption = lpMsg->NewOption;
					}

					int dwExOpCount = getNumberOfExcOptions(MapC[mapnumber].m_cItem[iItemCount].m_NewOption);
					if (dwExOpCount > ReadConfig.Soket__ExcMaximumOptionsOnSoketItem)
					{
						lpMsg->NewOption = BoxExcOptions(ReadConfig.Soket__ExcMaximumOptionsOnSoketItem);
						MapC[mapnumber].m_cItem[iItemCount].m_NewOption = lpMsg->NewOption;
					}
				} else {
					MapC[mapnumber].m_cItem[iItemCount].m_NewOption = 0;
					lpMsg->NewOption = 0;
				}
			} 
			else 
			{
				if (ReadConfig.Soket__EnableExcOnZeroSoketItemsDrop)
				{
					if (rand()%100 <= ReadConfig.Soket__ExcAdditionalPercentOnZSoketItem)
					{
						lpMsg->NewOption = BoxExcOptions(ReadConfig.Soket__ExcMaximumOptionsOnZSoketItem);
						MapC[mapnumber].m_cItem[iItemCount].m_NewOption = lpMsg->NewOption;
					}

					int dwExOpCount = getNumberOfExcOptions(MapC[mapnumber].m_cItem[iItemCount].m_NewOption);
					if (dwExOpCount > ReadConfig.Soket__ExcMaximumOptionsOnZSoketItem)
					{
						lpMsg->NewOption = BoxExcOptions(ReadConfig.Soket__ExcMaximumOptionsOnZSoketItem);
						MapC[mapnumber].m_cItem[iItemCount].m_NewOption = lpMsg->NewOption;
					}
				} else {
					MapC[mapnumber].m_cItem[iItemCount].m_NewOption = 0;
					lpMsg->NewOption = 0;
				}
			}
		}
		
		//Excellent Options Drop
		BYTE NewOption[MAX_EXOPTION_SIZE];

		for ( int i=0;i<MAX_EXOPTION_SIZE;i++)
		{
			NewOption[i] = FALSE;
		}

		if ( (lpMsg->NewOption & 0x20) != 0 )
		{
			NewOption[0] = TRUE;
		}
		if ( (lpMsg->NewOption & 0x10) != 0 )
		{
			NewOption[1] = TRUE;
		}

		if ( (lpMsg->NewOption & 0x8) != 0 )
		{
			NewOption[2] = TRUE;
		}
		if ( (lpMsg->NewOption & 0x4) != 0 )
		{
			NewOption[3] = TRUE;
		}

		if ( (lpMsg->NewOption & 0x2) != 0 )
		{
			NewOption[4] = TRUE;
		}
		if ( (lpMsg->NewOption & 0x1) != 0 )
		{
			NewOption[5] = TRUE;
		}

		LogAddTD("Monster Item Drop [%d][%d][%d] : serial:%d [%s][%d][%d][%d][%d] Ex:[%d,%d,%d,%d,%d,%d,%d] Set:[%d] Slot:[%d,%d,%d,%d,%d]",
			mapnumber, lpMsg->x, lpMsg->y, lpMsg->m_Number, ItemAttribute[lpMsg->Type].Name, lpMsg->Level,
			lpMsg->Op1, lpMsg->Op2, lpMsg->Op3, 
			NewOption[0],NewOption[1],NewOption[2],NewOption[3],NewOption[4],NewOption[5],NewOption[6],
			lpMsg->SetOption,
			NewSlotOption[0],NewSlotOption[1],NewSlotOption[2],NewSlotOption[3],NewSlotOption[4]
		);
	}
}


/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Item Move Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC1
 *	HeadCode  : 0x53
 */
struct SDHP_ITEMMOVESAVE
{
	PBMSG_HEAD h;	// C1:53
	DWORD Serial;	// 4
	char ServerName[20];	// 8
	char Account[10];	// 1C
	char Name[10];	// 26
	char ItemName[21];	// 30
	BYTE X;	// 45
	BYTE Y;	// 46
	BYTE ItemLevel;	// 47
	BYTE ItemOp1;	// 48
	BYTE ItemOp2;	// 49
	BYTE ItemOp3;	// 4A
};



void ItemMovePathSave(char* ActID, char* Name, BYTE level, BYTE mapnumber, BYTE x, BYTE y, char* Item, BYTE op1, BYTE op2, BYTE op3, DWORD serial)
{
	SDHP_ITEMMOVESAVE pMsg;

	pMsg.h.c = 0xC1;
	pMsg.h.headcode = 0x53;
	pMsg.h.size = sizeof(pMsg);
	pMsg.ItemLevel = level;
	pMsg.Serial = serial;
	pMsg.ItemOp1 = op1;
	pMsg.ItemOp2 = op2;
	pMsg.ItemOp3 = op3;
	pMsg.X = x;
	pMsg.Y = y;
	memcpy(pMsg.Account, ActID, sizeof(pMsg.Account));
	strcpy(pMsg.ItemName, Item);
	memcpy(pMsg.Name, Name, sizeof(pMsg.Name));
	strcpy(pMsg.ServerName, szServerName);

	cDBSMng.Send((PCHAR)&pMsg, pMsg.h.size);
}





/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Skill Key Data Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC1
 *	HeadCode  : 0x60
 */
struct SDHP_SKILLKEYDATA
{
	PBMSG_HEAD h;	// C1:60
	char Name[10];	// 3
#if (BORDS == 0)
	BYTE SkillKeyBuffer[10];	// D
#else
	BYTE SkillKeyBuffer[20];	// D
#endif
	BYTE GameOption;	// 17
	BYTE QkeyDefine;	// 18
	BYTE WkeyDefine;	// 19
	BYTE EkeyDefine;	// 1A
	BYTE ChatWindow;	// 1B
#if (BORDS == 1)
	BYTE RkeyDefine;	// 1A
#endif
};



#if (BORDS == 0)
	void DGOptionDataSend(int aIndex, char* szName, BYTE * KeyBuffer, BYTE GO, BYTE Qk, BYTE Wk, BYTE Ek, BYTE ChatWnd)
#else
	void DGOptionDataSend(int aIndex, char* szName, BYTE * KeyBuffer, BYTE GO, BYTE Qk, BYTE Wk, BYTE Ek, BYTE ChatWnd, BYTE Rk)
#endif
{
	SDHP_SKILLKEYDATA pMsg;

	pMsg.h.c = 0xC1;
	pMsg.h.headcode = 0x60;
	pMsg.h.size = sizeof(pMsg);
	memcpy(pMsg.Name, szName, sizeof(pMsg.Name));
	memcpy(pMsg.SkillKeyBuffer, KeyBuffer, sizeof(pMsg.SkillKeyBuffer));
	pMsg.GameOption = GO;
	pMsg.QkeyDefine = Qk;
	pMsg.WkeyDefine = Wk;
	pMsg.EkeyDefine = Ek;
	pMsg.ChatWindow = ChatWnd;

#if (BORDS == 0)
	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
#else
	pMsg.RkeyDefine = Rk;
	cDBSMng.Send((char*)&pMsg,pMsg.h.size);
#endif
}




void DGOptionDataRecv(SDHP_SKILLKEYDATA_SEND * lpMsg)
{
	int aIndex = lpMsg->aIndex;
	char szName[MAX_ACCOUNT_LEN+1];

	if ( aIndex < 0 || aIndex >= OBJMAX )
	{
		return;
	}

	memset(szName, 0, sizeof(szName));
	memcpy(szName, lpMsg->Name, sizeof(lpMsg->Name));

	if ( strcmp(gObj[aIndex].Name, szName) != 0 )
	{
		return;
	}

#if (BORDS == 0)
	::GCSkillKeySend(aIndex, lpMsg->SkillKeyBuffer, lpMsg->GameOption, lpMsg->QkeyDefine, lpMsg->WkeyDefine, lpMsg->EkeyDefine, lpMsg->ChatWindow);
#else
	::GCSkillKeySend(aIndex, lpMsg->SkillKeyBuffer, lpMsg->GameOption, lpMsg->QkeyDefine, lpMsg->WkeyDefine, lpMsg->EkeyDefine, lpMsg->ChatWindow, lpMsg->RkeyDefine);
#endif
	
}






void DGMoveOtherServer(SDHP_CHARACTER_TRANSFER_RESULT * lpMsg)
{
	if ( !gObjIsConnectedGP(lpMsg->Number))
	{
		LogAddC(2, "error-L3 [%s][%d]", __FILE__, __LINE__);
		return;
	}

	LPOBJ lpObj = &gObj[lpMsg->Number];

	if ( lpMsg->Result != 0 )
	{
		PMSG_DEFRESULT pResult;

		PHeadSetB((LPBYTE)&pResult, 0x99, sizeof(pResult));
		pResult.result = 1;

		DataSend(lpMsg->Number, (LPBYTE)&pResult, pResult.h.size);

		LogAddTD("[CharTrasfer] Fail [%s][%s] (%d)",
			lpObj->AccountID, lpObj->Name, lpMsg->Result);
		
		lpObj->m_MoveOtherServer = 0;
		
		GCServerMsgStringSend("ERROR! Contact SCFMT members for fix!",lpObj->m_Index, 1);
		// Deathway translation here
		return;
	}

	LogAddTD("[CharTrasfer] Success [%s][%s] (%d)",
		lpObj->AccountID, lpObj->Name, lpMsg->Result);

	GCServerMsgStringSend("접속이 종료됩니다.", lpObj->m_Index, 1);// Deathway translation here
	GCServerMsgStringSend("분할 서버로 접속해주시기 바랍니다.", lpObj->m_Index, 1);// Deathway translation here
	GJSetCharacterInfo(lpObj, lpObj->m_Index, 0);
	lpObj->LoadWareHouseInfo = false;
	gObjCloseSet(lpObj->m_Index, 2);
	lpObj->m_MoveOtherServer = 0;
}




struct Request_PetItem_Info
{
	BYTE nPos;	// 0
	int nSerial;	// 4
};



/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Request Pet Info Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC2
 *	HeadCode  : 0x56
 *	BASE PACKET - Complete with Request_PetItem_Info
 */
struct SDHP_REQUEST_PETITEM_INFO
{
	PWMSG_HEAD h;
	char AccountID[11];	// 4
	WORD Number;	// 10
	BYTE InvenType;	// 12
	BYTE nCount;	// 13
};


void gObjRequestPetItemInfo(int aIndex, int inventype)
{
	LPOBJ lpObj = &gObj[aIndex];
	char pbuffer[1024];
	int lofs = sizeof(SDHP_REQUEST_PETITEM_INFO);
	int founditemcount = 0;
	Request_PetItem_Info petIteminfo;

	int NumRequest = 0;

	if ( inventype == 0 )
	{
		for ( int n=0;n<ReadConfig.INVENTORY_SIZE(lpObj->m_Index,false);n++)
		{
			if ( lpObj->pInventory[n].IsItem() )
			{
				if ( lpObj->pInventory[n].m_Type == ITEMGET(13,4) || lpObj->pInventory[n].m_Type == ITEMGET(13,5) )
				{
					founditemcount++;
					petIteminfo.nPos = n;
					petIteminfo.nSerial = lpObj->pInventory[n].m_Number;
					memcpy(&pbuffer[lofs], &petIteminfo, sizeof(petIteminfo));
					lofs +=sizeof(petIteminfo);
				}
			}
		}
	}
	else if ( inventype == 1 )
	{
		for ( int n=0;n<ReadConfig.WAREHOUSE_SIZE(aIndex,false);n++)
		{
			if ( lpObj->pWarehouse[n].IsItem() )
			{
				if ( lpObj->pWarehouse[n].m_Type == ITEMGET(13,4) || lpObj->pWarehouse[n].m_Type == ITEMGET(13,5) )
				{
					if(NumRequest > 60)
						break;
					NumRequest++;
					founditemcount++;
					petIteminfo.nPos = n;
					petIteminfo.nSerial = lpObj->pWarehouse[n].m_Number;
					memcpy(&pbuffer[lofs], &petIteminfo, sizeof(petIteminfo));
					lofs +=sizeof(petIteminfo);
				}
			}
		}
	}
	
	if ( founditemcount )
	{
		SDHP_REQUEST_PETITEM_INFO pMsg;

		pMsg.h.set((LPBYTE)&pMsg, 0x56, lofs + sizeof(pMsg));
		memcpy(pMsg.AccountID, lpObj->AccountID, MAX_ACCOUNT_LEN);
		pMsg.Number = aIndex;
		pMsg.InvenType = inventype;
		pMsg.nCount = founditemcount;
		memcpy(pbuffer, &pMsg, sizeof(pMsg));

		cDBSMng.Send((PCHAR)pbuffer, lofs+sizeof(pMsg));
	}
}



struct Recv_PetItem_Info
{
	BYTE nPos;	// 0
	int nSerial;	// 4
	BYTE Level;	// 8
	int Exp;	// C
};


struct SDHP_RECV_PETITEM_INFO
{
	PWMSG_HEAD h;
	char AccountID[11];	// 4
	WORD Number;	// 10
	BYTE InvenType;	// 12
	BYTE nCount;	// 13
};




void DGRecvPetItemInfo(LPBYTE lpData)
{
	SDHP_RECV_PETITEM_INFO* lpMsg = (SDHP_RECV_PETITEM_INFO *)lpData;
	Recv_PetItem_Info* pPetItemInfo = (Recv_PetItem_Info *)((DWORD)lpData + sizeof(SDHP_RECV_PETITEM_INFO));
	int aIndex = lpMsg->Number;
	char szAccountId[MAX_ACCOUNT_LEN+1];
	szAccountId[MAX_ACCOUNT_LEN]=0;

	memcpy(szAccountId, lpMsg->AccountID, MAX_ACCOUNT_LEN);
	
	if ( !gObjIsAccontConnect(aIndex, szAccountId))
	{
		LogAddC(2, "Request to receive petitem infomation doesn't match the user %s", szAccountId);
		CloseClient(aIndex);
		
		return;
	}

	if ( lpMsg->InvenType == 0 )
	{
		BOOL ReCalc = 0;

		for ( int n=0;n<lpMsg->nCount;n++)
		{
			if ( pPetItemInfo[n].nPos < INVETORY_WEAR_SIZE )
			{
				ReCalc = 1;
			}

			if ( gObj[aIndex].pInventory[pPetItemInfo[n].nPos].IsItem() )
			{
				if ( gObj[aIndex].pInventory[pPetItemInfo[n].nPos].m_Number == pPetItemInfo[n].nSerial )
				{
					gObj[aIndex].pInventory[pPetItemInfo[n].nPos].SetPetItemInfo(pPetItemInfo[n].Level, pPetItemInfo[n].Exp);
				}
			}
		}

		if ( ReCalc != FALSE )
		{
			gObjCalCharacter(aIndex);
		}
	}
	else if ( lpMsg->InvenType == 1 )
	{
		for ( int n=0;n<lpMsg->nCount;n++)
		{

			if ( gObj[aIndex].pWarehouse[pPetItemInfo[n].nPos].IsItem() )
			{
				if ( gObj[aIndex].pWarehouse[pPetItemInfo[n].nPos].m_Number == pPetItemInfo[n].nSerial )
				{
					gObj[aIndex].pWarehouse[pPetItemInfo[n].nPos].SetPetItemInfo(pPetItemInfo[n].Level, pPetItemInfo[n].Exp);
				}
			}
		}

	}
}



struct Save_PetItem_Info
{
	int nSerial;	// 0
	BYTE Level;	// 3
	int Exp;	// 8
};



/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu PetInfo Save Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC2
 *	HeadCode  : 0x55
 *	BASE PACKET - Complete with Save_PetItem_Info
 */
struct SDHP_SAVE_PETITEM_INFO
{
	PWMSG_HEAD h;
	BYTE nCount;	// 4
};


void gObjSavePetItemInfo(int aIndex, int inventype)
{
	LPOBJ lpObj = &gObj[aIndex];
	char pbuffer[1024];
	int lofs = sizeof(SDHP_SAVE_PETITEM_INFO);
	int founditemcount = 0;
	Save_PetItem_Info petIteminfo;

	if ( inventype == 0 )
	{
		for ( int n=0;n<ReadConfig.INVENTORY_SIZE(lpObj->m_Index,false);n++)
		{
			if ( lpObj->pInventory[n].IsItem() )
			{
				if ( lpObj->pInventory[n].m_Type == ITEMGET(13,4) || lpObj->pInventory[n].m_Type == ITEMGET(13,5) )
				{
					if ( lpObj->pInventory[n].m_IsLoadPetItemInfo != FALSE )
					{
						founditemcount++;
						petIteminfo.nSerial = lpObj->pInventory[n].m_Number;
						petIteminfo.Level = lpObj->pInventory[n].m_PetItem_Level;
						petIteminfo.Exp = lpObj->pInventory[n].m_PetItem_Exp;

						LogAddTD("[%s][%s][PI][%d,%s] serial:[%d] Level:%d Exp:%d",
							lpObj->AccountID, lpObj->Name, n, lpObj->pInventory[n].GetName(),petIteminfo.nSerial,
							petIteminfo.Level, petIteminfo.Exp);

						memcpy(&pbuffer[lofs], &petIteminfo, sizeof(petIteminfo));
						lofs += sizeof(petIteminfo);
					}
				}
			}
		}
	}
	else if ( inventype == 1 )
	{
		for ( int n=0;n<ReadConfig.WAREHOUSE_SIZE(aIndex,false);n++)
		{
			if ( lpObj->pWarehouse[n].IsItem() )
			{
				if ( lpObj->pWarehouse[n].m_Type == ITEMGET(13,4) || lpObj->pWarehouse[n].m_Type == ITEMGET(13,5) )
				{
					if ( lpObj->pWarehouse[n].m_IsLoadPetItemInfo != FALSE )
					{
						founditemcount++;
						petIteminfo.nSerial = lpObj->pWarehouse[n].m_Number;
						petIteminfo.Level = lpObj->pWarehouse[n].m_PetItem_Level;
						petIteminfo.Exp = lpObj->pWarehouse[n].m_PetItem_Exp;

						LogAddTD("[%s][%s][PW][%d,%s] serial:[%d] Level:%d Exp:%d",
							lpObj->AccountID, lpObj->Name, n, lpObj->pWarehouse[n].GetName(),petIteminfo.nSerial,
							petIteminfo.Level, petIteminfo.Exp);

						memcpy(&pbuffer[lofs], &petIteminfo, sizeof(petIteminfo));
						lofs += sizeof(petIteminfo);
					}
				}
			}
		}
	}

	if ( founditemcount )
	{
		SDHP_SAVE_PETITEM_INFO pMsg;

		pMsg.h.set((LPBYTE)&pMsg, 0x57, lofs+sizeof(pMsg));
		pMsg.nCount = founditemcount;
		memcpy(pbuffer, &pMsg, sizeof(pMsg));

		cDBSMng.Send(pbuffer, lofs+sizeof(pMsg));
	}
}




/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Castle Siege          Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC1
 *	HeadCode  : 
 *	SubCode   :
 */
struct CSP_REQ_CASTLEDATA
{
	PBMSG_HEAD2 h;	// C1:80:00
	WORD wMapSvrNum;	// 4
	int iCastleEventCycle;	// 8
};



void GS_GDReqCastleTotalInfo(int iMapSvrGroup, int iCastleEventCycle)
{
	if ( iMapSvrGroup < 0 )
	{
		return;
	}

	CSP_REQ_CASTLEDATA pMsg;

	pMsg.h.set((LPBYTE)&pMsg, 0x80, 0x00, sizeof(CSP_REQ_CASTLEDATA));
	pMsg.wMapSvrNum = iMapSvrGroup;
	pMsg.iCastleEventCycle = iCastleEventCycle;

	::cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}



/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Castle Siege          Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC1
 *	HeadCode  : 
 *	SubCode   :
 */
struct CSP_REQ_OWNERGUILDMASTER
{
	PBMSG_HEAD2 h;
	WORD wMapSvrNum;	// 4
	int iIndex;	// 8
};


void GS_GDReqOwnerGuildMaster(int iMapSvrGroup, int aIndex)
{
	if ( iMapSvrGroup < 0 )
	{
		return;
	}

	if ( OBJMAX_RANGE(aIndex) == FALSE )
	{
		return;
	}


	CSP_REQ_OWNERGUILDMASTER pMsg;

	pMsg.h.set((LPBYTE)&pMsg, 0x80, 0x01, sizeof(CSP_REQ_OWNERGUILDMASTER));
	pMsg.wMapSvrNum = iMapSvrGroup;
	pMsg.iIndex = aIndex;

	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}





/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Castle Siege          Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC1
 *	HeadCode  : 
 *	SubCode   :
 */
struct CSP_REQ_NPCBUY
{
	PBMSG_HEAD2 h;
	WORD wMapSvrNum;	// 4
	int iIndex;	// 8
	int iNpcNumber;	// C
	int iNpcIndex;	// 10
	int iNpcDfLevel;	// 14
	int iNpcRgLevel;	// 18
	int iNpcMaxHp;	// 1C
	int iNpcHp;	// 20
	BYTE btNpcX;	// 24
	BYTE btNpcY;	// 25
	BYTE btNpcDIR;	// 26
	int iBuyCost;	// 28
};


void GS_GDReqCastleNpcBuy(int iMapSvrGroup, int aIndex, int iNpcNumber, int iNpcIndex, int iNpcDfLevel, int iNpcRgLevel, int iNpcMaxHP, int iNpcHP, BYTE btNpcX, BYTE btNpcY, BYTE btNpcDIR, int iBuyCost)
{
	if ( iMapSvrGroup < 0 )
	{
		return;
	}

	if ( OBJMAX_RANGE(aIndex) == FALSE )
	{
		return;
	}


	CSP_REQ_NPCBUY pMsg;

	pMsg.h.set((LPBYTE)&pMsg, 0x80, 0x03, sizeof(CSP_REQ_NPCBUY));
	pMsg.wMapSvrNum = iMapSvrGroup;
	pMsg.iIndex = aIndex;
	pMsg.iNpcNumber = iNpcNumber;
	pMsg.iNpcIndex = iNpcIndex;
	pMsg.iNpcDfLevel = iNpcDfLevel;
	pMsg.iNpcRgLevel = iNpcRgLevel;
	pMsg.iNpcMaxHp = iNpcMaxHP;
	pMsg.iNpcHp = iNpcHP;
	pMsg.btNpcX = btNpcX;
	pMsg.btNpcY = btNpcY;
	pMsg.btNpcDIR = btNpcDIR;
	pMsg.iBuyCost = iBuyCost;

	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}





/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Castle Siege          Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC1
 *	HeadCode  : 
 *	SubCode   :
 */
struct CSP_REQ_NPCREPAIR
{
	PBMSG_HEAD2 h;
	WORD wMapSvrNum;	// 4
	int iIndex;	// 8
	int iNpcNumber;	// C
	int iNpcIndex;	// 10
	int iRepairCost;	// 14
};




void GS_GDReqCastleNpcRepair(int iMapSvrGroup, int aIndex, int iNpcNumber, int iNpcIndex, int iRepairCost)
{
	if ( iMapSvrGroup < 0 )
	{
		return;
	}

	if ( OBJMAX_RANGE(aIndex) == FALSE )
	{
		return;
	}


	CSP_REQ_NPCREPAIR pMsg;

	pMsg.h.set((LPBYTE)&pMsg, 0x80, 0x04, sizeof(CSP_REQ_NPCREPAIR));
	pMsg.wMapSvrNum = iMapSvrGroup;
	pMsg.iIndex = aIndex;
	pMsg.iNpcNumber = iNpcNumber;
	pMsg.iNpcIndex = iNpcIndex;
	pMsg.iRepairCost = iRepairCost;

	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}



/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Castle Siege          Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC1
 *	HeadCode  : 
 *	SubCode   :
 */
struct CSP_REQ_NPCUPGRADE
{
	PBMSG_HEAD2 h;
	WORD wMapSvrNum;	// 4
	int iIndex;	// 8
	int iNpcNumber;	// C
	int iNpcIndex;	// 10
	int iNpcUpType;	// 14
	int iNpcUpValue;	// 18
	int iNpcUpIndex;	// 1C
};


void GS_GDReqCastleNpcUpgrade(int iMapSvrGroup, int aIndex, int iNpcNumber, int iNpcIndex, int iNpcUpType, int iNpcUpValue, int iNpcUpIndex)
{
	if ( iMapSvrGroup < 0 )
	{
		return;
	}

	if ( OBJMAX_RANGE(aIndex) == FALSE )
	{
		return;
	}


	CSP_REQ_NPCUPGRADE pMsg;

	pMsg.h.set((LPBYTE)&pMsg, 0x80, 0x05, sizeof(CSP_REQ_NPCUPGRADE));
	pMsg.wMapSvrNum = iMapSvrGroup;
	pMsg.iIndex = aIndex;
	pMsg.iNpcNumber = iNpcNumber;
	pMsg.iNpcIndex = iNpcIndex;
	pMsg.iNpcUpType = iNpcUpType;
	pMsg.iNpcUpValue = iNpcUpValue;
	pMsg.iNpcUpIndex = iNpcUpIndex;

	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}


/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Castle Siege          Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC1
 *	HeadCode  : 
 *	SubCode   :
 */
struct CSP_REQ_TAXINFO
{
	PBMSG_HEAD2 h;
	WORD wMapSvrNum;	// 3
	int iIndex;	// 8
};


void GS_GDReqTaxInfo(int iMapSvrGroup, int aIndex)
{
	if ( iMapSvrGroup < 0 )
	{
		return;
	}

	if ( OBJMAX_RANGE(aIndex) == FALSE )
	{
		return;
	}


	CSP_REQ_TAXINFO pMsg;

	pMsg.h.set((LPBYTE)&pMsg, 0x80, 0x06, sizeof(CSP_REQ_TAXINFO));
	pMsg.wMapSvrNum = iMapSvrGroup;
	pMsg.iIndex = aIndex;

	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}



/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Castle Siege          Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC1
 *	HeadCode  : 
 *	SubCode   :
 */
struct CSP_REQ_TAXRATECHANGE
{
	PBMSG_HEAD2 h;
	WORD wMapSvrNum;	// 4
	int iIndex;	// 8
	int iTaxKind;	// C
	int iTaxRate;	// 10
};



void GS_GDReqTaxRateChange( int iMapSvrGroup, int aIndex, int iTaxType, int iTaxRate)
{
	if ( iMapSvrGroup < 0 )
	{
		return;
	}

	if ( OBJMAX_RANGE(aIndex) == FALSE )
	{
		return;
	}


	CSP_REQ_TAXRATECHANGE pMsg;

	pMsg.h.set((LPBYTE)&pMsg, 0x80, 0x07, sizeof(CSP_REQ_TAXRATECHANGE));
	pMsg.wMapSvrNum = iMapSvrGroup;
	pMsg.iIndex = aIndex;
	pMsg.iTaxRate = iTaxRate; 
	pMsg.iTaxKind = iTaxType;
	
	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}





/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Castle Siege          Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC1
 *	HeadCode  : 
 *	SubCode   :
 */
struct CSP_REQ_MONEYCHANGE
{
	PBMSG_HEAD2 h;
	WORD wMapSvrNum;	// 4
	int iIndex;	// 8
	int iMoneyChanged;	// C
};


void GS_GDReqCastleMoneyChange(int iMapSvrGroup, int aIndex,int iMoneyChange)
{
	if ( iMapSvrGroup < 0 )
	{
		return;
	}

	if ( OBJMAX_RANGE(aIndex) == FALSE )
	{
		return;
	}


	CSP_REQ_MONEYCHANGE pMsg;

	pMsg.h.set((LPBYTE)&pMsg, 0x80, 0x08, sizeof(CSP_REQ_MONEYCHANGE));
	pMsg.wMapSvrNum = iMapSvrGroup;
	pMsg.iIndex = aIndex;
	pMsg.iMoneyChanged = iMoneyChange;
	
	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}




/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Castle Siege          Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC1
 *	HeadCode  : 
 *	SubCode   :
 */
struct CSP_REQ_SDEDCHANGE
{
	PBMSG_HEAD2 h;
	WORD wMapSvrNum;	// 4
	int iIndex;	// 8
	WORD wStartYear;	// C
	BYTE btStartMonth;	// E
	BYTE btStartDay;	// F
	WORD wEndYear;	// 10
	BYTE btEndMonth;	// 12
	BYTE btEndDay;	// 13
};


//void GS_GDReqCastleDateChange(int iMapSvrGroup, int aIndex,WORD wStartYear, BYTE btStartMonth, BYTE btStartDay, WORD wEndYear, BYTE btEndMonth, BYTE btEndDay)
void GS_GDReqSiegeDateChange(int iMapSvrGroup, int aIndex,WORD wStartYear, BYTE btStartMonth, BYTE btStartDay, WORD wEndYear, BYTE btEndMonth, BYTE btEndDay)
{
	if ( iMapSvrGroup < 0 )
	{
		return;
	}

	CSP_REQ_SDEDCHANGE pMsg;

	pMsg.h.set((LPBYTE)&pMsg, 0x80, 0x09, sizeof(CSP_REQ_SDEDCHANGE));
	pMsg.wMapSvrNum = iMapSvrGroup;
	pMsg.iIndex = aIndex;
	pMsg.wStartYear = wStartYear;
	pMsg.btStartMonth = btStartMonth;
	pMsg.btStartDay = btStartDay;
	pMsg.wEndYear = wEndYear;
	pMsg.btEndMonth = btEndMonth;
	pMsg.btEndDay = btEndDay;
	
	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}




/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Castle Siege          Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC1
 *	HeadCode  : 
 *	SubCode   :
 */
struct CSP_REQ_GUILDREGINFO
{
	PBMSG_HEAD2 h;
	WORD wMapSvrNum;	// 4
	int iIndex;	// 8
	char szGuildName[8];	// C
};


void GS_GDReqGuildMarkRegInfo(int iMapSvrGroup, int aIndex)
{
	if ( iMapSvrGroup < 0 )
	{
		return;
	}

	if ( OBJMAX_RANGE(aIndex) == FALSE )
	{
		return;
	}


	CSP_REQ_GUILDREGINFO pMsg ={0};

	pMsg.h.set((LPBYTE)&pMsg, 0x80, 0x0A, sizeof(CSP_REQ_GUILDREGINFO));
	pMsg.wMapSvrNum = iMapSvrGroup;
	pMsg.iIndex = aIndex;
	memcpy(pMsg.szGuildName, gObj[aIndex].GuildName, 8);
	
	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}



/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Castle Siege          Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC1
 *	HeadCode  : 
 *	SubCode   :
 */
struct CSP_REQ_SIEGEENDCHANGE
{
	PBMSG_HEAD2 h;
	WORD wMapSvrNum;	// 4
	BOOL bIsSiegeEnded;	// 8
};


void GS_GDReqSiegeEndedChange(int iMapSvrGroup, BOOL bIsSiegeEnded)
{
	if ( iMapSvrGroup < 0 )
	{
		return;
	}


	CSP_REQ_SIEGEENDCHANGE pMsg;

	pMsg.h.set((LPBYTE)&pMsg, 0x80, 0x0B, sizeof(CSP_REQ_SIEGEENDCHANGE));
	pMsg.wMapSvrNum = iMapSvrGroup;
	pMsg.bIsSiegeEnded = bIsSiegeEnded;
	
	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}


/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Castle Siege          Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC1
 *	HeadCode  : 
 *	SubCode   :
 */
struct CSP_REQ_CASTLEOWNERCHANGE
{
	PBMSG_HEAD2 h;
	WORD wMapSvrNum;	// 4
	BOOL bIsCastleOccupied;	// 8
	char szOwnerGuildName[8];	// C
};


void GS_GDReqCastleOwnerChange( int iMapSvrGroup, BOOL bIsCastleOccupied, char* lpszGuildName)
{
	if ( iMapSvrGroup < 0 )
	{
		return;
	}

	if ( lpszGuildName == NULL )
	{
		return;
	}


	CSP_REQ_CASTLEOWNERCHANGE pMsg;

	pMsg.h.set((LPBYTE)&pMsg, 0x80, 0x0C, sizeof(CSP_REQ_CASTLEOWNERCHANGE));
	pMsg.wMapSvrNum = iMapSvrGroup;
	pMsg.bIsCastleOccupied = bIsCastleOccupied;
	memcpy(pMsg.szOwnerGuildName, lpszGuildName, 8);
	
	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}



/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Castle Siege          Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC1
 *	HeadCode  : 
 *	SubCode   :
 */
struct CSP_REQ_REGATTACKGUILD
{
	PBMSG_HEAD2 h;
	WORD wMapSvrNum;	// 4
	int iIndex;	// 8
	char szEnemyGuildName[8];	// C
};


void GS_GDReqRegAttackGuild( int iMapSvrGroup, int aIndex)
{
	if ( iMapSvrGroup < 0 )
	{
		return;
	}

	if ( OBJMAX_RANGE(aIndex) == FALSE )
	{
		return;
	}

	//if( strcmp( gObj[aIndex].GuildName, .m_szCastleOwnerGuild) == 0)
	//{
	//	return;
	//}

	CSP_REQ_REGATTACKGUILD pMsg;

	pMsg.h.set((LPBYTE)&pMsg, 0x80, 0x0D, sizeof(CSP_REQ_REGATTACKGUILD));
	pMsg.wMapSvrNum = iMapSvrGroup;
	pMsg.iIndex = aIndex;
	memcpy(pMsg.szEnemyGuildName, gObj[aIndex].GuildName, 8);
	
	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}



/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Castle Siege          Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC1
 *	HeadCode  : 
 *	SubCode   :
 */
struct CSP_REQ_CASTLESIEGEEND
{
	PBMSG_HEAD2 h;
	WORD wMapSvrNum;	// 4
};


void GS_GDReqRestartCastleState( int iMapSvrGroup)
{
	if ( iMapSvrGroup < 0 )
	{
		return;
	}

	CSP_REQ_CASTLESIEGEEND pMsg;

	pMsg.h.set((LPBYTE)&pMsg, 0x80, 0x0E, sizeof(CSP_REQ_CASTLESIEGEEND));
	pMsg.wMapSvrNum = iMapSvrGroup;
	
	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}



/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Castle Siege          Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC1
 *	HeadCode  : 
 *	SubCode   :
 */
struct CSP_REQ_MAPSVRMULTICAST
{
	PBMSG_HEAD2 h;
	WORD wMapSvrNum;	// 4
	char szMsgText[128];	// 6
};


void GS_GDReqMapSvrMsgMultiCast( int iMapSvrGroup,char * lpszMsgText)
{
	if ( iMapSvrGroup < 0 )
	{
		return;
	}

	if ( lpszMsgText == NULL )
	{
		return;
	}


	CSP_REQ_MAPSVRMULTICAST pMsg;

	pMsg.h.set((LPBYTE)&pMsg, 0x80, 0x0F, sizeof(CSP_REQ_MAPSVRMULTICAST));
	pMsg.wMapSvrNum = iMapSvrGroup;
	strcpy(pMsg.szMsgText, lpszMsgText);
	pMsg.szMsgText[127] = 0;
	
	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}



/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Castle Siege          Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC1
 *	HeadCode  : 
 *	SubCode   :
 */
struct CSP_REQ_GUILDREGMARK
{
	PBMSG_HEAD2 h;
	WORD wMapSvrNum;	// 4
	int iIndex;	// 8
	char szGuildName[8];	// C
	int iItemPos;	// 14
};


void GS_GDReqGuildMark( int iMapSvrGroup, int aIndex, int iItemPos)
{
	if ( iMapSvrGroup < 0 )
	{
		return;
	}

	if ( OBJMAX_RANGE(aIndex) == FALSE )
	{
		return;
	}


	CSP_REQ_GUILDREGMARK pMsg;

	pMsg.h.set((LPBYTE)&pMsg, 0x80, 0x10, sizeof(CSP_REQ_GUILDREGMARK));
	pMsg.wMapSvrNum = iMapSvrGroup;
	pMsg.iIndex = aIndex;
	pMsg.iItemPos = iItemPos;
	memcpy(pMsg.szGuildName, gObj[aIndex].GuildName, sizeof(pMsg.szGuildName));
	
	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}


/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Castle Siege          Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC1
 *	HeadCode  : 
 *	SubCode   :
 */
struct CSP_REQ_GUILDRESETMARK
{
	PBMSG_HEAD2 h;
	WORD wMapSvrNum;	// 4
	int iIndex;	// 8
	char szGuildName[8];	// C
};


void GS_GDReqGuildMarkReset( int iMapSvrGroup, int aIndex, char* lpszGuildName)
{
	if ( iMapSvrGroup < 0 )
	{
		return;
	}

	if ( OBJMAX_RANGE(aIndex) == FALSE )
	{
		return;
	}

	if ( lpszGuildName == NULL )
	{
		return;
	}

	CSP_REQ_GUILDRESETMARK pMsg;

	pMsg.h.set((LPBYTE)&pMsg, 0x80, 0x11, sizeof(CSP_REQ_GUILDRESETMARK));
	pMsg.wMapSvrNum = iMapSvrGroup;
	pMsg.iIndex = aIndex;
	memcpy(pMsg.szGuildName, lpszGuildName, 8);
	
	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}


/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Castle Siege          Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC1
 *	HeadCode  : 
 *	SubCode   :
 */
struct CSP_REQ_GUILDSETGIVEUP
{
	PBMSG_HEAD2 h;
	WORD wMapSvrNum;	// 4
	int iIndex;	// 8
	char szGuildName[8];	// C
	BOOL bIsGiveUp;	// 14
};


void GS_GDReqGuildSetGiveUp( int iMapSvrGroup, int aIndex, BOOL bIsGiveUp)
{
	if ( iMapSvrGroup < 0 )
	{
		return;
	}

	if ( OBJMAX_RANGE(aIndex) == FALSE )
	{
		return;
	}

	CSP_REQ_GUILDSETGIVEUP pMsg;

	pMsg.h.set((LPBYTE)&pMsg, 0x80, 0x12, sizeof(CSP_REQ_GUILDSETGIVEUP));
	pMsg.wMapSvrNum = iMapSvrGroup;
	pMsg.iIndex = aIndex;
	pMsg.bIsGiveUp = bIsGiveUp;
	memcpy(pMsg.szGuildName, gObj[aIndex].GuildName, 8);
	
	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}



/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Castle Siege          Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC1
 *	HeadCode  : 
 *	SubCode   :
 */
struct CSP_REQ_NPCREMOVE
{
	PBMSG_HEAD2 h;
	WORD wMapSvrNum;	// 4
	int iNpcNumber;	// 8
	int iNpcIndex;	// C
};


void GS_GDReqNpcRemove(int iMapSvrGroup, int iNpcNumber, int iNpcIndex)
{
	if ( iMapSvrGroup < 0 )
	{
		return;
	}

	CSP_REQ_NPCREMOVE pMsg;

	pMsg.h.set((LPBYTE)&pMsg, 0x80, 0x16, sizeof(CSP_REQ_NPCREMOVE));
	pMsg.wMapSvrNum = iMapSvrGroup;
	pMsg.iNpcNumber = iNpcNumber;
	pMsg.iNpcIndex = iNpcIndex;
	
	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}


/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Castle Siege          Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC1
 *	HeadCode  : 
 *	SubCode   :
 */
struct CSP_REQ_CASTLESTATESYNC
{
	PBMSG_HEAD2 h;
	WORD wMapSvrNum;	// 4
	int iCastleState;	// 8
	int iTaxRateChaos;	// C
	int iTaxRateStore;	// 10
	int iTaxHuntZone;	// 14
	char szOwnerGuildName[8];	// 18
};


void GS_GDReqCastleStateSync( int iMapSvrGroup, int iCastleState, int iTaxRateChaos, int iTaxRateStore, int iTaxHuntZone, char* lpszOwnerGuild)
{
	if ( iMapSvrGroup < 0 )
	{
		return;
	}

	CSP_REQ_CASTLESTATESYNC pMsg;

	pMsg.h.set((LPBYTE)&pMsg, 0x80, 0x17, sizeof(CSP_REQ_CASTLESTATESYNC));
	pMsg.wMapSvrNum = iMapSvrGroup;
	pMsg.iCastleState = iCastleState;
	pMsg.iTaxRateChaos = iTaxRateChaos;
	pMsg.iTaxRateStore = iTaxRateStore;
	pMsg.iTaxHuntZone = iTaxHuntZone;
	memcpy(pMsg.szOwnerGuildName, lpszOwnerGuild, 8);
	
	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}




/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Castle Siege          Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC1
 *	HeadCode  : 
 *	SubCode   :
 */
struct CSP_REQ_CASTLETRIBUTEMONEY
{
	PBMSG_HEAD2 h;
	WORD wMapSvrNum;	// 4
	int iCastleTributeMoney;	// 8
};



void GS_GDReqCastleTributeMoney(int iMapSvrGroup, int iCastleTributeMoney)
{
	if ( iMapSvrGroup < 0 )
	{
		return;
	}

	if ( iCastleTributeMoney < 0 )
	{
		return;
	}

	CSP_REQ_CASTLETRIBUTEMONEY pMsg;

	pMsg.h.set((LPBYTE)&pMsg, 0x80, 0x18, sizeof(CSP_REQ_CASTLETRIBUTEMONEY));
	pMsg.wMapSvrNum = iMapSvrGroup;
	pMsg.iCastleTributeMoney = iCastleTributeMoney;

	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}



/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Castle Siege          Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC1
 *	HeadCode  : 
 *	SubCode   :
 */
struct CSP_REQ_RESETCASTLETAXINFO
{
	PBMSG_HEAD2 h;
	WORD wMapSvrNum;	// 4
};


void GS_GDReqResetCastleTaxInfo( int iMapSvrGroup)
{
	if ( iMapSvrGroup < 0 )
	{
		return;
	}

	CSP_REQ_RESETCASTLETAXINFO pMsg;

	pMsg.h.set((LPBYTE)&pMsg, 0x80, 0x19, sizeof(CSP_REQ_RESETCASTLETAXINFO));
	pMsg.wMapSvrNum = iMapSvrGroup;

	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}


/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Castle Siege          Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC1
 *	HeadCode  : 
 *	SubCode   :
 */
struct CSP_REQ_RESETSIEGEGUILDINFO
{
	PBMSG_HEAD2 h;
	WORD wMapSvrNum;	// 4
};


void GS_GDReqResetSiegeGuildInfo( int iMapSvrGroup)
{
	if ( iMapSvrGroup < 0 )
	{
		return;
	}

	CSP_REQ_RESETSIEGEGUILDINFO pMsg;

	pMsg.h.set((LPBYTE)&pMsg, 0x80, 0x1A, sizeof(CSP_REQ_RESETSIEGEGUILDINFO));
	pMsg.wMapSvrNum = iMapSvrGroup;

	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}



/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Castle Siege          Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC1
 *	HeadCode  : 
 *	SubCode   :
 */
struct CSP_REQ_RESETREGSIEGEINFO
{
	PBMSG_HEAD2 h;
	WORD wMapSvrNum;	// 2
};



void GS_GDReqResetRegSiegeInfo( int iMapSvrGroup)
{
	if ( iMapSvrGroup < 0 )
	{
		return;
	}

	CSP_REQ_RESETREGSIEGEINFO pMsg;

	pMsg.h.set((LPBYTE)&pMsg, 0x80, 0x1B, sizeof(CSP_REQ_RESETREGSIEGEINFO));
	pMsg.wMapSvrNum = iMapSvrGroup;

	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}




/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Castle Siege          Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC1
 *	HeadCode  : 
 */
struct CSP_REQ_CSINITDATA
{
	PBMSG_HEAD h;
	WORD wMapSvrNum;	// 4
	int iCastleEventCycle;	// 8
};


void GS_GDReqCastleInitData( int iMapSvrGroup, int iCastleEventCycle)
{
	if ( iMapSvrGroup < 0 )
	{
		return;
	}

	CSP_REQ_CSINITDATA pMsg;

	pMsg.h.set((LPBYTE)&pMsg, 0x81, sizeof(CSP_REQ_CSINITDATA));
	pMsg.wMapSvrNum = iMapSvrGroup;
	pMsg.iCastleEventCycle = iCastleEventCycle;

	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}




/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Castle Siege          Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC1
 *	HeadCode  : 
 */
struct CSP_REQ_NPCDATA
{
	PBMSG_HEAD h;
	WORD wMapSvrNum;	// 4
	int iIndex;	// 8
};



void GS_GDReqCaslteNpcInfo( int iMapSvrGroup, int aIndex)
{
	if ( iMapSvrGroup < 0 )
	{
		return;
	}

	CSP_REQ_NPCDATA pMsg;

	pMsg.h.set((LPBYTE)&pMsg, 0x82, sizeof(CSP_REQ_NPCDATA));
	pMsg.wMapSvrNum = iMapSvrGroup;
	pMsg.iIndex = aIndex;

	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}



/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Castle Siege          Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC1
 *	HeadCode  : 
 */
struct CSP_REQ_ALLGUILDREGINFO
{
	PBMSG_HEAD h;
	WORD wMapSvrNum;	// 4
	int iIndex;	// 8
};


void GS_GDReqAllGuildMarkRegInfo( int iMapSvrGroup, int aIndex)
{
	if ( iMapSvrGroup < 0 )
	{
		return;
	}

	CSP_REQ_ALLGUILDREGINFO pMsg;

	pMsg.h.set((LPBYTE)&pMsg, 0x83, sizeof(CSP_REQ_ALLGUILDREGINFO));
	pMsg.wMapSvrNum = iMapSvrGroup;
	pMsg.iIndex = aIndex;

	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}


/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Castle Siege          Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC1
 *	HeadCode  : 
 */
struct CSP_REQ_CALCREGGUILDLIST
{
	PBMSG_HEAD h;
	WORD wMapSvrNum;	// 4
};


void GS_GDReqCalcRegGuildList( int iMapSvrGroup)
{
	if ( iMapSvrGroup < 0 )
	{
		return;
	}

	CSP_REQ_CALCREGGUILDLIST pMsg;

	pMsg.h.set((LPBYTE)&pMsg, 0x85, sizeof(CSP_REQ_CALCREGGUILDLIST));
	pMsg.wMapSvrNum = iMapSvrGroup;

	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}



/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Castle Siege          Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC1
 *	HeadCode  : 
 */
struct CSP_REQ_CSLOADTOTALGUILDINFO
{
	PBMSG_HEAD h;
	WORD wMapSvrNum;	// 4
};


void GS_GDReqCsLoadTotalGuildInfo( int iMapSvrGroup)
{
	if ( iMapSvrGroup < 0 )
	{
		return;
	}

	CSP_REQ_CSLOADTOTALGUILDINFO pMsg;

	pMsg.h.set((LPBYTE)&pMsg, 0x88, sizeof(CSP_REQ_CSLOADTOTALGUILDINFO));
	pMsg.wMapSvrNum = iMapSvrGroup;

	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}

struct CSP_ANS_CASTLEDATA {

  // static data ------------------------------------

  // non-static data --------------------------------
  /*<thisrel this+0x0>*/ /*|0x4|*/ struct PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int iResult;
  /*<thisrel this+0x8>*/ /*|0x2|*/ unsigned short wMapSvrNum;
  /*<thisrel this+0xa>*/ /*|0x2|*/ unsigned short wStartYear;
  /*<thisrel this+0xc>*/ /*|0x1|*/ unsigned char btStartMonth;
  /*<thisrel this+0xd>*/ /*|0x1|*/ unsigned char btStartDay;
  /*<thisrel this+0xe>*/ /*|0x2|*/ unsigned short wEndYear;
  /*<thisrel this+0x10>*/ /*|0x1|*/ unsigned char btEndMonth;
  /*<thisrel this+0x11>*/ /*|0x1|*/ unsigned char btEndDay;
  /*<thisrel this+0x12>*/ /*|0x1|*/ unsigned char btIsSiegeGuildList;
  /*<thisrel this+0x13>*/ /*|0x1|*/ unsigned char btIsSiegeEnded;
  /*<thisrel this+0x14>*/ /*|0x1|*/ unsigned char btIsCastleOccupied;
  /*<thisrel this+0x15>*/ /*|0x8|*/ char szCastleOwnGuild[8];
  /*<thisrel this+0x20>*/ /*|0x8|*/ __int64 i64CastleMoney;
  /*<thisrel this+0x28>*/ /*|0x4|*/ int iTaxRateChaos;
  /*<thisrel this+0x2c>*/ /*|0x4|*/ int iTaxRateStore;
  /*<thisrel this+0x30>*/ /*|0x4|*/ int iTaxHuntZone;
  /*<thisrel this+0x34>*/ /*|0x4|*/ int iFirstCreate;
};

void GS_DGAnsCastleTotalInfo(LPBYTE lpRecv)
{
	#if (GS_CASTLE==1)
	CSP_ANS_CASTLEDATA* lpMsg = (CSP_ANS_CASTLEDATA*)lpRecv;

	if(lpMsg == NULL)
		return;

	if(lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup())
		LogAddC(2,"[CastleSiege] PACKET-ERROR [0x80][0x00] GS_DGAnsCastleTotalInfo() - lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()");
#else
	return;
#endif
}

struct CSP_ANS_OWNERGUILDMASTER {

  // static data ------------------------------------

  // non-static data --------------------------------
  /*<thisrel this+0x0>*/ /*|0x4|*/ struct PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int iResult;
  /*<thisrel this+0x8>*/ /*|0x2|*/ unsigned short wMapSvrNum;
  /*<thisrel this+0xc>*/ /*|0x4|*/ int iIndex;
  /*<thisrel this+0x10>*/ /*|0x8|*/ char szCastleOwnGuild[8];
  /*<thisrel this+0x18>*/ /*|0xa|*/ char szCastleOwnGuildMaster[10];

};

void GS_DGAnsOwnerGuildMaster(LPBYTE lpRecv)
{
	#if (GS_CASTLE==1)
	CSP_ANS_OWNERGUILDMASTER* lpMsg = (CSP_ANS_OWNERGUILDMASTER*)lpRecv;

	if(lpMsg == NULL)
		return;

	if(lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup())
	{
		LogAddC(2,"[CastleSiege] PACKET-ERROR [0x80][0x00] GS_DGAnsCastleTotalInfo() - lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()") ;
		return;
	}

	GCAnsCastleSiegeState(lpMsg->iIndex,lpMsg->iResult,lpMsg->szCastleOwnGuild,lpMsg->szCastleOwnGuildMaster);
#else
	return;
#endif
}

struct CSP_ANS_NPCBUY {

  /*<thisrel this+0x0>*/ /*|0x4|*/ struct PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int iResult;
  /*<thisrel this+0x8>*/ /*|0x2|*/ unsigned short wMapSvrNum;
  /*<thisrel this+0xc>*/ /*|0x4|*/ int iIndex;
  /*<thisrel this+0x10>*/ /*|0x4|*/ int iNpcNumber;
  /*<thisrel this+0x14>*/ /*|0x4|*/ int iNpcIndex;
  /*<thisrel this+0x18>*/ /*|0x4|*/ int iBuyCost;
};


void GS_DGAnsCastleNpcBuy(unsigned char * lpRecv )
{
#if (GS_CASTLE==1)
	CSP_ANS_NPCBUY* lpMsg = (CSP_ANS_NPCBUY*)lpRecv;

	if(lpMsg == NULL)
		return;

	if(lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup())
	{
		LogAddC(2,"[CastleSiege] PACKET-ERROR [0x80][0x03] GS_DGAnsCastleNpcBuy() - lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()") ;
		return;
	}

	if (lpMsg->iResult == 1)
	{
		if(g_CastleSiege.AddDbNPC(lpMsg->iNpcNumber,lpMsg->iNpcIndex) == TRUE)
		{
			if(gObjIsConnected(lpMsg->iIndex))
			{
				gObj[lpMsg->iIndex].Money -= lpMsg->iBuyCost;

				if(gObj[lpMsg->iIndex].Money < 0 )
				{
					gObj[lpMsg->iIndex].Money = 0;
				}

				GCMoneySend(lpMsg->iIndex,gObj[lpMsg->iIndex].Money);
			}

			LogAddTD("[CastleSiege] GS_DGAnsCastleNpcBuy() - CCastleSiege::AddDbNPC() OK - Npc:(CLS:%d, IDX:%d)",lpMsg->iNpcNumber,lpMsg->iNpcIndex);
		}
		else
		{
			LogAddTD("[CastleSiege] GS_DGAnsCastleNpcBuy() - CCastleSiege::AddDbNPC() FAILED - Npc:(CLS:%d, IDX:%d)",lpMsg->iNpcNumber,lpMsg->iNpcIndex);
		}
	}
	else
	{
		LogAddTD("[CastleSiege] GS_DGAnsCastleNpcBuy() FAILED - Result:(%d), Npc:(CLS:%d, IDX:%d)",lpMsg->iResult,lpMsg->iNpcNumber,lpMsg->iNpcIndex); 
	}

	GCAnsNpcBuy(lpMsg->iIndex,lpMsg->iResult,lpMsg->iNpcNumber,lpMsg->iNpcIndex);
#else
	return;
#endif
}

struct CSP_ANS_NPCREPAIR {
  /*<thisrel this+0x0>*/ /*|0x4|*/ struct PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int iResult;
  /*<thisrel this+0x8>*/ /*|0x2|*/ unsigned short wMapSvrNum;
  /*<thisrel this+0xc>*/ /*|0x4|*/ int iIndex;
  /*<thisrel this+0x10>*/ /*|0x4|*/ int iNpcNumber;
  /*<thisrel this+0x14>*/ /*|0x4|*/ int iNpcIndex;
  /*<thisrel this+0x18>*/ /*|0x4|*/ int iNpcMaxHp;
  /*<thisrel this+0x1c>*/ /*|0x4|*/ int iNpcHp;
  /*<thisrel this+0x20>*/ /*|0x4|*/ int iRepairCost;
};

void GS_DGAnsCastleNpcRepair(LPBYTE lpRecv)
{
	#if (GS_CASTLE==1)
	CSP_ANS_NPCREPAIR* lpMsg = (CSP_ANS_NPCREPAIR*) lpRecv;

	if(lpMsg == NULL)
		return;

	if(lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup())
	{
		LogAddC(2,"[CastleSiege] PACKET-ERROR [0x80][0x04] GS_DGAnsCastleNpcRepair() - lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()") ;
		return;
	}

	if(lpMsg->iResult == 1)
	{
		if(g_CastleSiege.RepairDbNPC(lpMsg->iNpcNumber,lpMsg->iNpcIndex,lpMsg->iNpcHp,lpMsg->iNpcMaxHp) == TRUE)
		{
			if(gObjIsConnected(lpMsg->iIndex))
			{
				gObj[lpMsg->iIndex].Money -= lpMsg->iRepairCost;

				if(gObj[lpMsg->iIndex].Money < 0 )
				{
					gObj[lpMsg->iIndex].Money = 0;
				}

				GCMoneySend(lpMsg->iIndex,gObj[lpMsg->iIndex].Money);
			}

			LogAddTD("[CastleSiege] GS_DGAnsCastleNpcRepair() - CCastleSiege::RepairDbNPC() OK - Npc:(CLS:%d, IDX:%d)",lpMsg->iNpcNumber,lpMsg->iNpcIndex);
		}
		else
		{
			LogAddTD("[CastleSiege] GS_DGAnsCastleNpcRepair() - CCastleSiege::RepairDbNPC() FAILED - Npc:(CLS:%d, IDX:%d)",lpMsg->iNpcNumber,lpMsg->iNpcIndex);
		}
	}
	else
	{
		LogAddTD("[CastleSiege] GS_DGAnsCastleNpcRepair() FAILED - Result:(%d), Npc:(CLS:%d, IDX:%d)",lpMsg->iResult,lpMsg->iNpcNumber,lpMsg->iNpcIndex);
	}

	GCAnsNpcRepair(lpMsg->iIndex,lpMsg->iResult,lpMsg->iNpcNumber,lpMsg->iNpcIndex,lpMsg->iNpcHp,lpMsg->iNpcMaxHp);
#else
	return;
#endif
}

struct CSP_ANS_NPCUPGRADE {
  /*<thisrel this+0x0>*/ /*|0x4|*/ struct PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int iResult;
  /*<thisrel this+0x8>*/ /*|0x2|*/ unsigned short wMapSvrNum;
  /*<thisrel this+0xc>*/ /*|0x4|*/ int iIndex;
  /*<thisrel this+0x10>*/ /*|0x4|*/ int iNpcNumber;
  /*<thisrel this+0x14>*/ /*|0x4|*/ int iNpcIndex;
  /*<thisrel this+0x18>*/ /*|0x4|*/ int iNpcUpType;
  /*<thisrel this+0x1c>*/ /*|0x4|*/ int iNpcUpValue;
  /*<thisrel this+0x20>*/ /*|0x4|*/ int iNpcUpIndex;
};

void GS_DGAnsCastleNpcUpgrade(LPBYTE lpRecv)
{
	#if (GS_CASTLE==1)
	CSP_ANS_NPCUPGRADE* lpMsg = (CSP_ANS_NPCUPGRADE*)lpRecv;

	if(lpMsg == NULL)
		return;

	if(lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup())
	{
		LogAddC(2,"[CastleSiege] PACKET-ERROR [0x80][0x05] GS_DGAnsCastleNpcUpgrade() - lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()") ;
		return;
	}
	
	if(lpMsg->iResult == TRUE)
	{
		if(!g_CastleSiege.UpgradeDbNPC(lpMsg->iIndex,lpMsg->iNpcNumber,lpMsg->iNpcIndex,lpMsg->iNpcUpType,lpMsg->iNpcUpValue,lpMsg->iNpcUpIndex))
			return;

		LogAddTD("[CastleSiege] [0x80][0x06] GS_DGAnsTaxInfo() - Npc Upgrade OK (CLS:%d, IDX:%d, UPTYPE:%d, UPVAL:%d)",lpMsg->iNpcNumber,
			lpMsg->iNpcIndex,lpMsg->iNpcUpType,lpMsg->iNpcUpValue);
	}

	GCAnsNpcUpgrade(lpMsg->iIndex,lpMsg->iResult,lpMsg->iNpcNumber,lpMsg->iNpcIndex,lpMsg->iNpcUpType,lpMsg->iNpcUpValue);
#else
	return;
#endif
}

struct CSP_ANS_TAXINFO {
  /*<thisrel this+0x0>*/ /*|0x4|*/ struct PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int iResult;
  /*<thisrel this+0x8>*/ /*|0x2|*/ unsigned short wMapSvrNum;
  /*<thisrel this+0xc>*/ /*|0x4|*/ int iIndex;
  /*<thisrel this+0x10>*/ /*|0x8|*/ __int64 i64CastleMoney;
  /*<thisrel this+0x18>*/ /*|0x4|*/ int iTaxRateChaos;
  /*<thisrel this+0x1c>*/ /*|0x4|*/ int iTaxRateStore;
  /*<thisrel this+0x20>*/ /*|0x4|*/ int iTaxHuntZone;
};

void GS_DGAnsTaxInfo(LPBYTE lpRecv)
{
	#if (GS_CASTLE==1)
	CSP_ANS_TAXINFO* lpMsg = (CSP_ANS_TAXINFO*)lpRecv;

	if(lpMsg == NULL)
		return;
	
	if(lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup())
	{
		LogAddC(2,"[CastleSiege] PACKET-ERROR [0x80][0x06] GS_DGAnsTaxInfo() - lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()") ;
		return;
	}

	if (lpMsg->i64CastleMoney > MAX_ZEN)
	{
		lpMsg->i64CastleMoney = MAX_ZEN;
	}

	if(lpMsg->iResult == TRUE)
	{
		GCAnsTaxMoneyInfo(lpMsg->iIndex,lpMsg->iResult,lpMsg->iTaxRateChaos,lpMsg->iTaxRateStore,lpMsg->i64CastleMoney);
	}

	g_CastleSiege.SetCastleMoney(lpMsg->i64CastleMoney);
#else
	return;
#endif
}

struct CSP_ANS_TAXRATECHANGE {
  /*<thisrel this+0x0>*/ /*|0x4|*/ struct PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int iResult;
  /*<thisrel this+0x8>*/ /*|0x2|*/ unsigned short wMapSvrNum;
  /*<thisrel this+0xc>*/ /*|0x4|*/ int iIndex;
  /*<thisrel this+0x10>*/ /*|0x4|*/ int iTaxKind;
  /*<thisrel this+0x14>*/ /*|0x4|*/ int iTaxRate;
};

void GS_DGAnsTaxRateChange(LPBYTE lpRecv)
{
	#if (GS_CASTLE==1)
	CSP_ANS_TAXRATECHANGE* lpMsg = (CSP_ANS_TAXRATECHANGE*)lpRecv;

	if(lpMsg == NULL)
		return;

	if(lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup())
	{
		LogAddC(2,"[CastleSiege] PACKET-ERROR [0x80][0x07] GS_DGAnsTaxRateChange() - lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()") ;
		return;
	}

	if(lpMsg->iResult == 1)
	{
		g_CastleSiege.SetTaxRate(lpMsg->iTaxKind,lpMsg->iTaxRate);
	}

	GCAnsTaxRateChange(lpMsg->iIndex,lpMsg->iResult,lpMsg->iTaxKind,lpMsg->iTaxRate);
#else
	return;
#endif
}

struct CSP_ANS_MONEYCHANGE {
  /*<thisrel this+0x0>*/ /*|0x4|*/ struct PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int iResult;
  /*<thisrel this+0x8>*/ /*|0x2|*/ unsigned short wMapSvrNum;
  /*<thisrel this+0xc>*/ /*|0x4|*/ int iIndex;
  /*<thisrel this+0x10>*/ /*|0x4|*/ int iMoneyChanged;
  /*<thisrel this+0x18>*/ /*|0x8|*/ __int64 i64CastleMoney;
};

void GS_DGAnsCastleMoneyChange(LPBYTE lpRecv)
{
	#if (GS_CASTLE==1)
	CSP_ANS_MONEYCHANGE* lpMsg = (CSP_ANS_MONEYCHANGE*)lpRecv;

	if(lpMsg == NULL)
		return;

	if(lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup())
	{
		LogAddC(2,"[CastleSiege] PACKET-ERROR [0x80][0x08] GS_DGAnsCastleMoneyChange() - lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()") ;
		return;
	}

	if (lpMsg->i64CastleMoney > MAX_ZEN)
		lpMsg->i64CastleMoney = MAX_ZEN;

	if(lpMsg->iResult == TRUE)
	{
		if(gObjIsConnected(lpMsg->iIndex))
		{
			//if(lpMsg->iMoneyChanged < 0)
			//{
				gObj[lpMsg->iIndex].Money -= lpMsg->iMoneyChanged;

				if(gObj[lpMsg->iIndex].Money > MAX_ZEN)
				{
					gObj[lpMsg->iIndex].Money = MAX_ZEN;
				}
			//}
			GCMoneySend(lpMsg->iIndex,gObj[lpMsg->iIndex].Money);

			LogAddTD("[CastleSiege] [0x80][0x08] GS_DGAnsCastleMoneyChange() - Withdraw Request OK [%s][%s] (ReqMoney:%d, TotMoney:%I64d)",gObj[lpMsg->iIndex].AccountID,
				gObj[lpMsg->iIndex].Name,lpMsg->iMoneyChanged,lpMsg->i64CastleMoney);
		}
		g_CastleSiege.SetCastleMoney(lpMsg->i64CastleMoney);
	}
	
	GCAnsMoneyDrawOut(lpMsg->iIndex,lpMsg->iResult,lpMsg->i64CastleMoney);
#else
	return;
#endif
}

struct CSP_ANS_SDEDCHANGE {
  /*<thisrel this+0x0>*/ /*|0x4|*/ struct PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int iResult;
  /*<thisrel this+0x8>*/ /*|0x2|*/ unsigned short wMapSvrNum;
  /*<thisrel this+0xc>*/ /*|0x4|*/ int iIndex;
  /*<thisrel this+0x10>*/ /*|0x2|*/ unsigned short wStartYear;
  /*<thisrel this+0x12>*/ /*|0x1|*/ unsigned char btStartMonth;
  /*<thisrel this+0x13>*/ /*|0x1|*/ unsigned char btStartDay;
  /*<thisrel this+0x14>*/ /*|0x2|*/ unsigned short wEndYear;
  /*<thisrel this+0x16>*/ /*|0x1|*/ unsigned char btEndMonth;
  /*<thisrel this+0x17>*/ /*|0x1|*/ unsigned char btEndDay;
};

void GS_DGAnsSiegeDateChange(LPBYTE lpRecv)
{
	#if (GS_CASTLE==1)
	CSP_ANS_SDEDCHANGE* lpMsg = (CSP_ANS_SDEDCHANGE*)lpRecv;

	if(lpMsg == NULL)
		return;

	if(lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup())
	{
		LogAddC(2,"[CastleSiege] PACKET-ERROR [0x80][0x09] GS_DGAnsSiegeDateChange() - lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()") ;
		return;
	}
#else
	return;
#endif
}

void GS_DGAnsGuildMarkRegInfo(LPBYTE lpRecv)
{
	#if (GS_CASTLE==1)
	CSP_ANS_GUILDREGINFO* lpMsg = (CSP_ANS_GUILDREGINFO*)lpRecv;

	if(lpMsg == NULL)
		return;
	
	if(lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup())
	{
		LogAddC(2,"[CastleSiege] PACKET-ERROR [0x80][0x0A] GS_DGAnsGuildMarkRegInfo() - lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()") ;
		return;
	}

	GCAnsGuildRegInfo(lpMsg->iIndex,lpMsg->iResult,lpMsg);
#else
	return;
#endif
}

struct CSP_ANS_SIEGEENDCHANGE {
  /*<thisrel this+0x0>*/ /*|0x4|*/ struct PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int iResult;
  /*<thisrel this+0x8>*/ /*|0x2|*/ unsigned short wMapSvrNum;
  /*<thisrel this+0xc>*/ /*|0x4|*/ int bIsSiegeEnded;
};

void GS_DGAnsSiegeEndedChange(LPBYTE lpRecv)
{
	#if (GS_CASTLE==1)
	CSP_ANS_SIEGEENDCHANGE* lpMsg = (CSP_ANS_SIEGEENDCHANGE*)lpRecv;

	if(lpMsg == NULL)
		return;

	if(lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup())
	{
		LogAddC(2,"[CastleSiege] PACKET-ERROR [0x80][0x0B] GS_DGAnsSiegeEndedChange() - lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()") ;
		return;
	}
#else
	return;
#endif
}

struct CSP_ANS_CASTLEOWNERCHANGE {

  // static data ------------------------------------

  // non-static data --------------------------------
  /*<thisrel this+0x0>*/ /*|0x4|*/ struct PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int iResult;
  /*<thisrel this+0x8>*/ /*|0x2|*/ unsigned short wMapSvrNum;
  /*<thisrel this+0xc>*/ /*|0x4|*/ int bIsCastleOccupied;
  /*<thisrel this+0x10>*/ /*|0x8|*/ char szOwnerGuildName[8];
};

void GS_DGAnsCastleOwnerChange(LPBYTE lpRecv)
{
	#if (GS_CASTLE==1)
	CSP_ANS_CASTLEOWNERCHANGE* lpMsg = (CSP_ANS_CASTLEOWNERCHANGE*)lpRecv;

	if(lpMsg == NULL)
		return;

	if(lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup())
	{
		LogAddC(2,"[CastleSiege] PACKET-ERROR [0x80][0x0C] GS_DGAnsCastleOwnerChange() - lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()") ;
		return;
	}
#else
	return;
#endif
}

struct CSP_ANS_REGATTACKGUILD {

  /*<thisrel this+0x0>*/ /*|0x4|*/ struct PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int iResult;
  /*<thisrel this+0x8>*/ /*|0x2|*/ unsigned short wMapSvrNum;
  /*<thisrel this+0xc>*/ /*|0x4|*/ int iIndex;
  /*<thisrel this+0x10>*/ /*|0x8|*/ char szEnemyGuildName[8];
};

void GS_DGAnsRegAttackGuild(LPBYTE lpRecv)
{
	#if (GS_CASTLE==1)
	CSP_ANS_REGATTACKGUILD* lpMsg = (CSP_ANS_REGATTACKGUILD*)lpRecv;

	if(lpMsg == NULL)
		return;

	if(lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup())
	{
		LogAddC(2,"[CastleSiege] PACKET-ERROR [0x80][0x0D] GS_DGAnsRegAttackGuild() - lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()") ;
		return;
	}

	GCAnsRegCastleSiege(lpMsg->iIndex,lpMsg->iResult,lpMsg->szEnemyGuildName);
	#else
	return;
	#endif
}

struct CSP_ANS_CASTLESIEGEEND {
  /*<thisrel this+0x0>*/ /*|0x4|*/ struct PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int iResult;
  /*<thisrel this+0x8>*/ /*|0x2|*/ unsigned short wMapSvrNum;
};

void GS_DGAnsRestartCastleState(LPBYTE lpRecv)
{
	#if (GS_CASTLE==1)
	CSP_ANS_CASTLESIEGEEND* lpMsg = (CSP_ANS_CASTLESIEGEEND*)lpRecv;

	if(lpMsg == NULL)
		return;

	if(lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup())
	{
		LogAddC(2,"[CastleSiege] PACKET-ERROR [0x80][0x0E] GS_DGAnsRestartCastleState() - lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()") ;
		return;
	}
	#else
	return;
	#endif
}

/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Castle Siege          Packet
 *	Direction : DataServer -> GameServer
 *  Code      : 0xC1
 *	HeadCode  : 
 *	SubCode	  :
 */
struct CSP_ANS_MAPSVRMULTICAST
{
	PBMSG_HEAD2 h;
	WORD wMapSvrNum;	// 4
	char szMsgText[128];	// 6
};


void GS_DGAnsMapSvrMsgMultiCast(LPBYTE lpRecv)
{
	CSP_ANS_MAPSVRMULTICAST * lpMsg = (CSP_ANS_MAPSVRMULTICAST *)lpRecv;

	if ( lpMsg == NULL )
	{
		return;
	}

	if ( lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup() )
	{
		LogAddC(2, "[CastleSiege] PACKET-ERROR [0x80][0x0F] GS_DGAnsMapSvrMsgMultiCast() - lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()");
		return;
	}

	char szNotice[144] = {0};
	memcpy(szNotice, lpMsg->szMsgText, 128);

	if ( strlen(szNotice) > 1 )
	{
		PMSG_NOTICE pNotice;
		pNotice.type = 0;	// 3
		pNotice.btCount = 0;	// 4
		pNotice.wDelay = 0;	// 6	
		pNotice.dwColor = 0;	// 8
		pNotice.btSpeed = 0;	// C

		TNotice::MakeNoticeMsg((TNotice *)&pNotice, 0, szNotice);
		TNotice::SetNoticeProperty((TNotice *)&pNotice, 0, _ARGB(255, 255, 200, 80), 1, 0, 20);
		TNotice::SendNoticeToAllUser((TNotice *)&pNotice);
	}

}


void GS_DGAnsRegGuildMark(LPBYTE lpRecv)
{
	#if (GS_CASTLE==1)
	CSP_ANS_GUILDREGMARK* lpMsg = (CSP_ANS_GUILDREGMARK*)lpRecv;
		
	if ( lpMsg == NULL )
	{
		return;
	}

	if ( lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup() )
	{
		LogAddC(2, "[CastleSiege] PACKET-ERROR [0x80][0x10] GS_DGAnsRegGuildMark() - lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()");
		return;
	}

	::GCAnsRegGuildMark(lpMsg->iIndex,lpMsg->iResult,lpMsg);
	#endif
	return;
}

struct CSP_ANS_GUILDRESETMARK {
  /*<thisrel this+0x0>*/ /*|0x4|*/ struct PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int iResult;
  /*<thisrel this+0x8>*/ /*|0x2|*/ unsigned short wMapSvrNum;
  /*<thisrel this+0xc>*/ /*|0x4|*/ int iIndex;
  /*<thisrel this+0x10>*/ /*|0x8|*/ char szGuildName[8];
  /*<thisrel this+0x18>*/ /*|0x4|*/ int iRegMarkCount;
};

void GS_DGAnsGuildMarkReset(LPBYTE lpRecv)
{
	#if (GS_CASTLE==1)
	CSP_ANS_GUILDRESETMARK* lpMsg = (CSP_ANS_GUILDRESETMARK*)lpRecv;
	
	if ( lpMsg == NULL )
	{
		return;
	}

	if ( lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup() )
	{
		LogAddC(2, "[CastleSiege] PACKET-ERROR [0x80][0x11] GS_DGAnsRegGuildMark() - lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()");
		return;
	}
	#endif
	return;
}

struct CSP_ANS_GUILDSETGIVEUP {
  /*<thisrel this+0x0>*/ /*|0x4|*/ struct PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int iResult;
  /*<thisrel this+0x8>*/ /*|0x2|*/ unsigned short wMapSvrNum;
  /*<thisrel this+0xc>*/ /*|0x4|*/ int iIndex;
  /*<thisrel this+0x10>*/ /*|0x8|*/ char szGuildName[8];
  /*<thisrel this+0x18>*/ /*|0x4|*/ int bIsGiveUp;
  /*<thisrel this+0x1c>*/ /*|0x4|*/ int iRegMarkCount;
};

void GS_DGAnsGuildSetGiveUp(LPBYTE lpRecv)
{
	#if (GS_CASTLE==1)
	CSP_ANS_GUILDSETGIVEUP* lpMsg = (CSP_ANS_GUILDSETGIVEUP*)lpRecv;

	if ( lpMsg == NULL )
	{
		return;
	}

	if ( lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup() )
	{
		LogAddC(2, "[CastleSiege] PACKET-ERROR [0x80][0x12] GS_DGAnsGuildSetGiveUp() - lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()");
		return;
	}
	
	::GCAnsGiveUpCastleSiege(lpMsg->iIndex,lpMsg->iResult,lpMsg->bIsGiveUp,lpMsg->iRegMarkCount,lpMsg->szGuildName);
	#endif
	return;
}

struct CSP_ANS_NPCREMOVE {
  /*<thisrel this+0x0>*/ /*|0x4|*/ struct PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int iResult;
  /*<thisrel this+0x8>*/ /*|0x2|*/ unsigned short wMapSvrNum;
  /*<thisrel this+0xc>*/ /*|0x4|*/ int iNpcNumber;
  /*<thisrel this+0x10>*/ /*|0x4|*/ int iNpcIndex;
};

void GS_DGAnsNpcRemove(LPBYTE lpRecv)
{
	#if (GS_CASTLE==1)
	CSP_ANS_NPCREMOVE* lpMsg = (CSP_ANS_NPCREMOVE*)lpRecv;
	
	if ( lpMsg == NULL )
	{
		return;
	}

	if ( lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup() )
	{
		LogAddC(2, "[CastleSiege] PACKET-ERROR [0x80][0x13] GS_DGAnsNpcRemove() - lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()");
		return;
	}
	#endif
	return;
}



/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Castle Siege          Packet
 *	Direction : DataServer -> GameServer
 *  Code      : 0xC1
 *	HeadCode  : 
 *	SubCode	  :
 */
struct CSP_ANS_CASTLESTATESYNC
{
	PBMSG_HEAD2 h;
	WORD wMapSvrNum;	// 4
	int iCastleState;	// 8
	int iTaxRateChaos;	// C
	int iTaxRateStore;	// 10
	int iTaxHuntZone;	// 14
	char szOwnerGuildName[8];	// 18
};

void GS_DGAnsCastleStateSync(LPBYTE lpRecv)
{
	CSP_ANS_CASTLESTATESYNC * lpMsg = (CSP_ANS_CASTLESTATESYNC *)lpRecv;

	if ( lpMsg == NULL )
	{
		DebugLog2("%s NULL",__FUNCTION__);
		return;
	}

	//LogAddTD("[CastleStateSync][%d/%d] STATE: %d, CHAOS RATE: %d, STORE RATE: %d, HUNT ZONE: %d",
	//	lpMsg->wMapSvrNum, g_MapServerManager.GetMapSvrGroup(), lpMsg->iCastleState, lpMsg->iTaxRateChaos, lpMsg->iTaxRateStore, lpMsg->iTaxHuntZone);

	if ( lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup() )
	{
		LogAddTD("[CastleStateSync]Wrong MapServer group [%d/%d] STATE: %d, CHAOS RATE: %d, STORE RATE: %d, HUNT ZONE: %d",
			lpMsg->wMapSvrNum, g_MapServerManager.GetMapSvrGroup(), lpMsg->iCastleState, lpMsg->iTaxRateChaos, lpMsg->iTaxRateStore, lpMsg->iTaxHuntZone);
		return;
	}

	g_CastleSiegeSync.SetCastleState(lpMsg->iCastleState); 
	g_CastleSiegeSync.SetTaxRateChaos(lpMsg->iTaxRateChaos);
	g_CastleSiegeSync.SetTaxRateStore(lpMsg->iTaxRateStore);
	g_CastleSiegeSync.SetTaxHuntZone(lpMsg->iTaxHuntZone);
	g_CastleSiegeSync.SetCastleOwnerGuild(lpMsg->szOwnerGuildName);
}


/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Castle Siege          Packet
 *	Direction : DataServer -> GameServer
 *  Code      : 0xC1
 *	HeadCode  : 
 *	SubCode	  :
 */
struct CSP_ANS_CASTLETRIBUTEMONEY
{
	PBMSG_HEAD2 h;
	int iResult;	// 4
	WORD wMapSvrNum;	// 8
};


void GS_DGAnsCastleTributeMoney(LPBYTE lpRecv)
{
	CSP_ANS_CASTLETRIBUTEMONEY * lpMsg = (CSP_ANS_CASTLETRIBUTEMONEY *)lpRecv;

	if ( lpMsg == NULL )
	{
		return;
	}

	if ( lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup() )
	{
		LogAddC(2, "[CastleSiege] PACKET-ERROR [0x80][0x18] GS_DGAnsCastleTributeMoney() - lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()");
		return;
	}
	
	LogAddTD("[CastleSiege] [0x80][0x18] GS_DGAnsCastleTributeMoney() - Money Tribute OK (%d)",
		g_CastleSiegeSync.GetTributeMoney());
	g_CastleSiegeSync.ResetTributeMoney();
	
}

struct CSP_ANS_RESETCASTLETAXINFO {
  /*<thisrel this+0x0>*/ /*|0x4|*/ struct PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int iResult;
  /*<thisrel this+0x8>*/ /*|0x2|*/ unsigned short wMapSvrNum;

};

void GS_DGAnsResetCastleTaxInfo(LPBYTE lpRecv)
{
	#if (GS_CASTLE==1)
	CSP_ANS_RESETCASTLETAXINFO* lpMsg = (CSP_ANS_RESETCASTLETAXINFO*)lpRecv;

	if(lpMsg == NULL)
		return;

	if(lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup())
	{
		LogAddC(2,"[CastleSiege] PACKET-ERROR [0x80][0x19] GS_DGAnsResetCastleTaxInfo() - lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()") ;
		return;
	}
	#else
	return;
	#endif
}

struct CSP_ANS_RESETSIEGEGUILDINFO {
  /*<thisrel this+0x0>*/ /*|0x4|*/ struct PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int iResult;
  /*<thisrel this+0x8>*/ /*|0x2|*/ unsigned short wMapSvrNum;
};

void GS_DGAnsResetSiegeGuildInfo(LPBYTE lpRecv)
{
	#if (GS_CASTLE==1)
	CSP_ANS_RESETSIEGEGUILDINFO* lpMsg = (CSP_ANS_RESETSIEGEGUILDINFO*)lpRecv;

	if(lpMsg == NULL)
		return;

	if(lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup())
	{
		LogAddC(2,"[CastleSiege] PACKET-ERROR [0x80][0x1A] GS_DGAnsResetSiegeGuildInfo() - lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()") ;
		return;
	}
	#else
	return;
	#endif
}

struct CSP_ANS_RESETREGSIEGEINFO {
  /*<thisrel this+0x0>*/ /*|0x4|*/ struct PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int iResult;
  /*<thisrel this+0x8>*/ /*|0x2|*/ unsigned short wMapSvrNum;
};

void GS_DGAnsResetRegSiegeInfo(LPBYTE lpRecv)
{
	#if (GS_CASTLE==1)
	CSP_ANS_RESETREGSIEGEINFO* lpMsg = (CSP_ANS_RESETREGSIEGEINFO*)lpRecv;

	if(lpMsg == NULL)
		return;

	if(lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup())
	{
		LogAddC(2,"[CastleSiege] PACKET-ERROR [0x80][0x1B] GS_DGAnsResetRegSiegeInfo() - lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()") ;
		return;
	}
	#else
	return;
	#endif
}

void GS_DGAnsCastleInitData(LPBYTE lpRecv)
{
	#if (GS_CASTLE==1)
	CSP_ANS_CSINITDATA* lpMsg = (CSP_ANS_CSINITDATA*)lpRecv;
	CSP_CSINITDATA* lpMsgBody = (CSP_CSINITDATA*)(lpRecv + sizeof(CSP_ANS_CSINITDATA));

	if(lpMsg == NULL)
		return;

	if(lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup())
	{
		LogAddC(2,"[CastleSiege] PACKET-ERROR [0x81] GS_DGAnsCastleInitData() - lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()") ;
		return;
	}

	if(g_CastleSiege.GetDataLoadState() != 2)
	{
		LogAddC(2,"[CastleSiege] CASTLE SIEGE DATA SETTING FAILED [0x81] - m_iCastleDataLoadState != CASTLESIEGE_DATALOAD_2 (%d)",g_CastleSiege.GetDataLoadState()) ;
		return;
	}

	g_CastleSiege.SetDataLoadState(3);

	if(lpMsg->iResult == FALSE)
	{
		LogAddC(2,"[CastleSiege] CASTLE SIEGE DATA SETTING FAILED [0x81] - lpMsg->iResult == 0");
		MsgBox("[CastleSiege] CASTLE SIEGE DATA SETTING FAILED [0x81] - lpMsg->iResult == 0");
		return;
	}
	
	BOOL bRET_VAL = FALSE;

	bRET_VAL = g_CastleSiege.SetCastleInitData(lpMsg);

	if(bRET_VAL == FALSE)
	{
		LogAddC(2,"[CastleSiege] CASTLE SIEGE DATA SETTING FAILED [0x81] - g_CastleSiege.SetCastleInitData() == FALSE");
		return;
	}

	bRET_VAL = g_CastleSiege.SetCastleNpcData(lpMsgBody,lpMsg->iCount);
	
	if(bRET_VAL == FALSE)
	{
		LogAddC(2,"[CastleSiege] CASTLE SIEGE DATA SETTING FAILED [0x81] - g_CastleSiege.SetCastleNpcData() == FALSE");
		return;
	}

	g_CastleSiege.SetDbDataLoadOK(TRUE);

	g_CastleSiege.SetDataLoadState(4);

	if(lpMsg->iFirstCreate == 1)
	{
		g_CastleSiege.FirstCreateDbNPC();
	}

	g_CastleSiege.Init();
	#else
	return;
	#endif
}

struct CSP_NPCDATA {
  /*<thisrel this+0x0>*/ /*|0x4|*/ int iNpcNumber;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int iNpcIndex;
  /*<thisrel this+0x8>*/ /*|0x4|*/ int iNpcDfLevel;
  /*<thisrel this+0xc>*/ /*|0x4|*/ int iNpcRgLevel;
  /*<thisrel this+0x10>*/ /*|0x4|*/ int iNpcMaxHp;
  /*<thisrel this+0x14>*/ /*|0x4|*/ int iNpcHp;
  /*<thisrel this+0x18>*/ /*|0x1|*/ unsigned char btNpcX;
  /*<thisrel this+0x19>*/ /*|0x1|*/ unsigned char btNpcY;
  /*<thisrel this+0x1a>*/ /*|0x1|*/ unsigned char btNpcDIR;
};

void GS_DGAnsCastleNpcInfo(LPBYTE lpRecv)
{
	#if (GS_CASTLE==1)
	CSP_ANS_NPCDATA* lpMsg = (CSP_ANS_NPCDATA*)lpRecv;
	CSP_NPCDATA* lpMsgBody = (CSP_NPCDATA*)(lpRecv+sizeof(CSP_ANS_NPCDATA));

	if(lpMsg == NULL)
		return;

	if(lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup())
	{
		LogAddC(2,"[CastleSiege] PACKET-ERROR [0x82] GS_DGAnsCastleNpcInfo() - lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()") ;
		return;
	}
	#else
	return;
	#endif
}

struct CSP_ANS_ALLGUILDREGINFO { // <size 0x14>
  /*<thisrel this+0x0>*/ /*|0x4|*/ PWMSG_HEAD h;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int iResult;
  /*<thisrel this+0x8>*/ /*|0x2|*/ unsigned short wMapSvrNum;
  /*<thisrel this+0xc>*/ /*|0x4|*/ int iIndex;
  /*<thisrel this+0x10>*/ /*|0x4|*/ int iCount;
};
struct CSP_GUILDREGINFO {// <size 0x10>
  /*<thisrel this+0x0>*/ /*|0x8|*/ char szGuildName[8];
  /*<thisrel this+0x8>*/ /*|0x4|*/ int iRegMarkCount;
  /*<thisrel this+0xc>*/ /*|0x1|*/ unsigned char bIsGiveUp;
  /*<thisrel this+0xd>*/ /*|0x1|*/ unsigned char btRegRank;
};
struct PMSG_ANS_CSREGGUILDLIST {// <size 0xc>
  /*<thisrel this+0x0>*/ /*|0x4|*/ PWMSG_HEAD h;
  /*<thisrel this+0x4>*/ /*|0x1|*/ unsigned char btResult;
  /*<thisrel this+0x8>*/ /*|0x4|*/ int iCount;
};
struct PMSG_ANS_CSREGGUILDLIST2 {// <size 0xc>
  /*<thisrel this+0x0>*/ /*|0x4|*/ PWMSG_HEAD h;
  /*<thisrel this+0x4>*/ /*|0x1|*/ unsigned char btResult;
  /*<thisrel this+0x8>*/ /*|0x4|*/ int iCount;
  /*<thisrel this+0x8>*/ /*|0x4|*/ //BYTE iCount;
};
struct PMSG_CSREGGUILDLIST {// <size 0xe>
  /*<thisrel this+0x0>*/ /*|0x8|*/ char szGuildName[8];
  /*<thisrel this+0x8>*/ /*|0x1|*/ unsigned char btRegMarks1;
  /*<thisrel this+0x9>*/ /*|0x1|*/ unsigned char btRegMarks2;
  /*<thisrel this+0xa>*/ /*|0x1|*/ unsigned char btRegMarks3;
  /*<thisrel this+0xb>*/ /*|0x1|*/ unsigned char btRegMarks4;
  /*<thisrel this+0xc>*/ /*|0x1|*/ unsigned char btIsGiveUp;
  /*<thisrel this+0xd>*/ /*|0x1|*/ unsigned char btSeqNum;
};

void GS_DGAnsAllGuildMarkRegInfo(LPBYTE lpRecv)
{
#if (GS_CASTLE == 1)
        CSP_ANS_ALLGUILDREGINFO * lpMsg = (CSP_ANS_ALLGUILDREGINFO*)lpRecv;
        CSP_GUILDREGINFO * lpMsgBody = (CSP_GUILDREGINFO*)&lpRecv[sizeof(CSP_ANS_ALLGUILDREGINFO)];
 
        if(lpMsg == NULL)
                return;
 
        if(lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup())
        {
                LogAddC(2,"[CastleSiege] PACKET-ERROR [0x83] GS_DGAnsAllGuildMarkRegInfo() - lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()");
                return;
        }
 
        if(gObjIsConnected(lpMsg->iIndex) == FALSE)
                return;
 
        unsigned char cBUFFER[0x684] = {0};
 
        PMSG_ANS_CSREGGUILDLIST * lpMsgSend = (PMSG_ANS_CSREGGUILDLIST*)&cBUFFER[0];
        PMSG_CSREGGUILDLIST * lpMsgSendBody = (PMSG_CSREGGUILDLIST*)&cBUFFER[sizeof(PMSG_ANS_CSREGGUILDLIST)];
 
		unsigned char bResult = (unsigned char)lpMsg->iResult;
        lpMsgSend->iCount = 0;
        lpMsgSend->btResult = bResult;
 
        if(lpMsg->iResult == 1)
        {
                lpMsgSend->iCount = lpMsg->iCount;
 
                for(int iCNT = 0; iCNT < lpMsg->iCount; iCNT++)
                {
                        lpMsgSendBody[iCNT].btSeqNum = lpMsgBody[iCNT].btRegRank;
                        lpMsgSendBody[iCNT].btIsGiveUp = lpMsgBody[iCNT].bIsGiveUp;
 
                        lpMsgSendBody[iCNT].btRegMarks1 = DWORD((DWORD(lpMsgBody[iCNT].iRegMarkCount) >> 16) & 0xFFFF) >> 8;
                        lpMsgSendBody[iCNT].btRegMarks2 = DWORD((DWORD(lpMsgBody[iCNT].iRegMarkCount) >> 16) & 0xFFFF) & 0xFF;
                        lpMsgSendBody[iCNT].btRegMarks3 = DWORD((DWORD(lpMsgBody[iCNT].iRegMarkCount) & 0xFFFF) & 0xFFFF) >> 8;
                        lpMsgSendBody[iCNT].btRegMarks4 = DWORD((DWORD(lpMsgBody[iCNT].iRegMarkCount) & 0xFFFF) & 0xFFFF) & 0xFF;
 
						//lpMsgSendBody[iCNT].btRegMarks1 = SET_NUMBERH(SET_NUMBERHW(lpMsgBody[i].iRegMarkCount));
						//lpMsgSendBody[iCNT].btRegMarks2 = SET_NUMBERL(SET_NUMBERHW(lpMsgBody[i].iRegMarkCount));
						//lpMsgSendBody[iCNT].btRegMarks3 = SET_NUMBERH(SET_NUMBERLW(lpMsgBody[i].iRegMarkCount));
						//lpMsgSendBody[iCNT].btRegMarks4 = SET_NUMBERL(SET_NUMBERLW(lpMsgBody[i].iRegMarkCount));

                        memcpy(lpMsgSendBody[iCNT].szGuildName,lpMsgBody[iCNT].szGuildName,MAX_GUILD_LEN);
                }
		}
 
        lpMsgSend->h.set((PBYTE)lpMsgSend,0xB4,sizeof(PMSG_CSREGGUILDLIST) * lpMsgSend->iCount + sizeof(PMSG_ANS_CSREGGUILDLIST));
        DataSend(lpMsg->iIndex,(PBYTE)lpMsgSend,sizeof(PMSG_CSREGGUILDLIST) * lpMsgSend->iCount + sizeof(PMSG_ANS_CSREGGUILDLIST));
#endif
}

struct CSP_ANS_NPCSAVEDATA {

  // static data ------------------------------------

  // non-static data --------------------------------
  /*<thisrel this+0x0>*/ /*|0x3|*/ struct PBMSG_HEAD h;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int iResult;
  /*<thisrel this+0x8>*/ /*|0x2|*/ unsigned short wMapSvrNum;
};

void GS_DGAnsFirstCreateNPC(LPBYTE lpRecv)
{
	#if (GS_CASTLE==1)
	CSP_ANS_NPCSAVEDATA* lpMsg = (CSP_ANS_NPCSAVEDATA*)lpRecv;

	if(lpMsg == NULL)
	return;

	if(lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup())
	{
		LogAddC(2,"[CastleSiege] PACKET-ERROR [0x84] GS_DGAnsFirstCreateNPC() - lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()") ;
		return;
	}

	if(lpMsg->iResult == TRUE)
	{
		g_CastleSiege.GetDbNpcCreated();
		g_CastleSiege.CreateDbNPC();
		g_CastleSiege.SetDbNpcCreated(1);
	}
	#else
	return;
	#endif
}

struct CSP_ANS_CALCREGGUILDLIST {// <size 0x10>
  /*<thisrel this+0x0>*/ /*|0x4|*/ PWMSG_HEAD h;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int iResult;
  /*<thisrel this+0x8>*/ /*|0x2|*/ unsigned short wMapSvrNum;
  /*<thisrel this+0xc>*/ /*|0x4|*/ int iCount;
};

void GS_DGAnsCalcREgGuildList(LPBYTE lpRecv)
{
	#if (GS_CASTLE==1)
	CSP_ANS_CALCREGGUILDLIST* lpMsg = (CSP_ANS_CALCREGGUILDLIST*)lpRecv;
	CSP_CALCREGGUILDLIST* lpMsgBody = (CSP_CALCREGGUILDLIST*)(lpRecv+sizeof(CSP_ANS_CALCREGGUILDLIST));
	
	if(lpMsg == NULL)
	return;

	if(lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup())
	{
		LogAddC(2,"[CastleSiege] PACKET-ERROR [0x85] GS_DGAnsCalcREgGuildList() - lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()") ;
		return;
	}

	if(lpMsg->iResult == TRUE)
	{
		LogAddTD("[CastleSiege] GS_DGAnsCalcREgGuildList() - Guild Data Answer Result");
		g_CastleSiege.SetCalcRegGuildList(lpMsgBody,lpMsg->iCount);
		return;
	}

	LogAddC(2,"[CastleSiege] PACKET-ERROR [0x85] GS_DGAnsCalcREgGuildList() - lpMsg->iResult != 1") ;
	#else
	return;
	#endif
}

struct CSP_ANS_CSGUILDUNIONINFO {// <size 0x10>
  /*<thisrel this+0x0>*/ /*|0x4|*/ struct PWMSG_HEAD h;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int iResult;
  /*<thisrel this+0x8>*/ /*|0x2|*/ unsigned short wMapSvrNum;
  /*<thisrel this+0xc>*/ /*|0x4|*/ int iCount;
};

void GS_DGAnsCsGulidUnionInfo(LPBYTE lpRecv)
{
	#if (GS_CASTLE==1)
	CSP_ANS_CSGUILDUNIONINFO* lpMsg = (CSP_ANS_CSGUILDUNIONINFO*)lpRecv;
	CSP_CSGUILDUNIONINFO * lpMsgBody = (CSP_CSGUILDUNIONINFO*)((LPBYTE)lpRecv + sizeof(CSP_ANS_CSGUILDUNIONINFO));
	if(lpMsg == NULL)
		return;

	if(lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup())
	{
		LogAddC(2,"[CastleSiege] PACKET-ERROR [0x86] GS_DGAnsCsGulidUnionInfo() - lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()") ;
		return;
	}

	if(lpMsg->iResult == TRUE)
	{
		g_CastleSiege.MakeCsTotalGuildInfo(lpMsgBody,lpMsg->iCount);
		return;
	}
	#else
	return;
	#endif
}

struct CSP_ANS_CSSAVETOTALGUILDINFO {// <size 0xc>
  /*<thisrel this+0x0>*/ /*|0x3|*/ PBMSG_HEAD h;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int iResult;
  /*<thisrel this+0x8>*/ /*|0x2|*/ unsigned short wMapSvrNum;
};

void GS_DGAnsCsSaveTotalGuildInfo(LPBYTE lpRecv)
{
	#if (GS_CASTLE==1)
	CSP_ANS_CSSAVETOTALGUILDINFO* lpMsg = (CSP_ANS_CSSAVETOTALGUILDINFO*)lpRecv;
	
	if(lpMsg == NULL)
	return;

	if(lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup())
	{
		LogAddC(2,"[CastleSiege] PACKET-ERROR [0x87] GS_DGAnsCsSaveTotalGuildInfo() - lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()") ;
		return;
	}

	if(lpMsg->iResult == TRUE)
	{
		g_CastleSiege.SetIsSiegeGuildList(TRUE);
		return;
	}
	g_CastleSiege.SetIsSiegeGuildList(FALSE);
	#endif
	return;
}

struct CSP_ANS_CSLOADTOTALGUILDINFO {// <size 0x10>
  /*<thisrel this+0x0>*/ /*|0x4|*/ PWMSG_HEAD h;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int iResult;
  /*<thisrel this+0x8>*/ /*|0x2|*/ unsigned short wMapSvrNum;
  /*<thisrel this+0xc>*/ /*|0x4|*/ int iCount;
};

void GS_DGAnsCsLoadTotalGuildInfo(LPBYTE lpRecv)
{
	#if (GS_CASTLE==1)
	CSP_ANS_CSLOADTOTALGUILDINFO* lpMsg = (CSP_ANS_CSLOADTOTALGUILDINFO*)lpRecv;
	CSP_CSLOADTOTALGUILDINFO* lpMsgBody = (CSP_CSLOADTOTALGUILDINFO*)(lpRecv+sizeof(CSP_ANS_CSLOADTOTALGUILDINFO));

	if(lpMsg == NULL)
	return;

	if(lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup())
	{
		LogAddC(2,"[CastleSiege] PACKET-ERROR [0x88] GS_DGAnsCsLoadTotalGuildInfo() - lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()") ;
		return;
	}

	if(lpMsg->iResult == TRUE)
	{
		g_CastleSiege.SetCsTotalGuildInfo(lpMsgBody,lpMsg->iCount);
		LogAddTD("[CastleSiege] GS_DGAnsCsLoadTotalGuildInfo() - Guild Data Update");
	}
	#endif
	return;
}

struct CSP_ANS_NPCUPDATEDATA {// <size 0xc>
  /*<thisrel this+0x0>*/ /*|0x3|*/ PBMSG_HEAD h;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int iResult;
  /*<thisrel this+0x8>*/ /*|0x2|*/ unsigned short wMapSvrNum;
};

void GS_DGAnsCastleNpcUpdate(LPBYTE lpRecv)
{
	#if (GS_CASTLE==1)
	CSP_ANS_NPCUPDATEDATA* lpMsg = (CSP_ANS_NPCUPDATEDATA*)lpRecv;

	if(lpMsg == NULL)
	return;

	if(lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup())
	{
		LogAddC(2,"[CastleSiege] PACKET-ERROR [0x89] GS_DGAnsCastleNpcUpdate() - lpMsg->wMapSvrNum != g_MapServerManager.GetMapSvrGroup()") ;
		return;
	}

	char* szResult;

	if(lpMsg->iResult != FALSE)
	{
		szResult = "SUCCEED";
	}
	else
	{
		szResult = "FAIL";
	}

	LogAddTD("[CastleSiege] [0x89] GS_DGAnsCastleNpcUpdate() - Npc Data Update Result : '%s'",szResult);
	#else
	return;
	#endif
}


struct CWP_REQ_CRYWOLFSYNC
{
	PBMSG_HEAD h;
	WORD wMapSvrNum;	// 4
	int iCrywolfState;	// 8
	int iOccupationState;	// C
};

void GDReqCrywolfSync(int iMapSvrGroup, int iCrywolfState, int iOccupationState)
{
	if ( iMapSvrGroup <0 )
		return;

	CWP_REQ_CRYWOLFSYNC pMsg;

	pMsg.h.set((LPBYTE)&pMsg, 0xB0, sizeof(CWP_REQ_CRYWOLFSYNC));
	pMsg.wMapSvrNum = iMapSvrGroup;
	pMsg.iCrywolfState = iCrywolfState;
	pMsg.iOccupationState = iOccupationState;

	cDBSMng.Send((PCHAR)&pMsg, pMsg.h.size);
}





struct CWP_ANS_CRYWOLFSYNC
{
	PBMSG_HEAD h;
	BYTE btResult;	// 3
	int iCrywolfState;	// 4
	int iOccupationState;	// 8
};



DWORD dwCrywolfSyncLogTickCount = 0;

void DGAnsCrywolfSync(LPBYTE lpRecv)
{
	CWP_ANS_CRYWOLFSYNC * lpMsg = (CWP_ANS_CRYWOLFSYNC *)lpRecv;

	if ( lpMsg == NULL )
		return;

	//int iOldOccupationState = g_CrywolfSync.GetOccupationState();
	g_CrywolfSync.SetCrywolfState(lpMsg->iCrywolfState);
	g_CrywolfSync.SetOccupationState(lpMsg->iOccupationState);

	if ( GetTickCount() > (dwCrywolfSyncLogTickCount+ 60000) )
	{
		LogAddC(4, "[Crywolf] SYNC Occupation: %d, State: %d", lpMsg->iOccupationState, lpMsg->iCrywolfState);
		dwCrywolfSyncLogTickCount = GetTickCount();
	}
}


struct CWP_REQ_CRYWOLFINFOLOAD
{
	PBMSG_HEAD h;
	WORD wMapSvrNum;	// 4
};


void GDReqCrywolfInfoLoad(int iMapSvrGroup)
{
	if ( iMapSvrGroup <0 )
		return;

	CWP_REQ_CRYWOLFINFOLOAD pMsg;

	pMsg.h.set((LPBYTE)&pMsg, 0xB1, sizeof(CWP_REQ_CRYWOLFINFOLOAD));
	pMsg.wMapSvrNum = iMapSvrGroup;

	cDBSMng.Send((PCHAR)&pMsg, pMsg.h.size);
}




struct CWP_ANS_CRYWOLFINFOLOAD
{
	PBMSG_HEAD h;
	BYTE btResult;	// 3
	int iCrywolfState;	// 4
	int iOccupationState;	// 8
};


void DGAnsCrywolfInfoLoad(LPBYTE lpRecv)
{
	CWP_ANS_CRYWOLFINFOLOAD * lpMsg = (CWP_ANS_CRYWOLFINFOLOAD *)lpRecv;
	
#if (GS_CASTLE==1)
	if(lpMsg == NULL)
		return;

	int iResult;

	if(lpMsg->iOccupationState < 0 || lpMsg->iOccupationState > 2)
	{
		iResult = 1;
	}
	else
	{
		iResult = 0;
	}

	lpMsg->btResult = iResult;

	if((iResult & 0xFF) == 0)
	{
		g_Crywolf.ApplyCrywolfDBInfo(lpMsg->iCrywolfState,lpMsg->iOccupationState);
		return;
	}

	LogAddC(2,"[Crywolf] Incorrect DB OccupationState!!!");

#endif
}



struct CWP_REQ_CRYWOLFINFOSAVE
{
	PBMSG_HEAD h;
	WORD wMapSvrNum;	// 4
	int iCrywolfState;	// 8
	int iOccupationState;	// C
};



void GDReqCrywolfInfoSave(int iMapSvrGroup, int iCrywolfState, int iOccupationState)
{
	if ( iMapSvrGroup <0 )
		return;

	CWP_REQ_CRYWOLFINFOSAVE pMsg;

	pMsg.h.set((LPBYTE)&pMsg, 0xB2, sizeof(CWP_REQ_CRYWOLFINFOSAVE));
	pMsg.wMapSvrNum = iMapSvrGroup;
	pMsg.iCrywolfState = iCrywolfState;
	pMsg.iOccupationState = iOccupationState;

	cDBSMng.Send((PCHAR)&pMsg, pMsg.h.size);
}




struct CWP_ANS_CRYWOLFINFOSAVE
{
	PBMSG_HEAD h;
	BYTE btResult;	// 3
};


void DGAnsCrywolfInfoSave(LPBYTE lpRecv)
{
	CWP_ANS_CRYWOLFINFOSAVE * lpMsg = (CWP_ANS_CRYWOLFINFOSAVE *)lpRecv;
}


void GS_GDReqRegGuildMark(int iMapSvrGroup, int aIndex, int iItemPos)
{
#if (GS_CASTLE==1)
	CSP_REQ_GUILDREGMARK pMsg;

	if(iMapSvrGroup < 0)
		return;

	if(OBJMAX_RANGE(aIndex) == FALSE)
		return;

	pMsg.h.set((LPBYTE)&pMsg,0x80,0x10,sizeof(pMsg));

	pMsg.wMapSvrNum = iMapSvrGroup;
	pMsg.iIndex = aIndex;
	pMsg.iItemPos = iItemPos;
	memcpy(&pMsg.szGuildName,gObj[aIndex].GuildName,sizeof(pMsg.szGuildName));

	cDBSMng.Send((char*)&pMsg,pMsg.h.size);
#endif
}


// End of DSProtocol.h
// Decompiled By : Deathway