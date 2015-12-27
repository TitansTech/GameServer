#include "stdafx.h"
#include "user.h"
#include "NpcTalk.h"
#include "gObjMonster.h"
#include "..\common\winutil.h"
#include "protocol.h"
#include "ObjCalCharacter.h"
#include "GameMain.h"
#include "BloodCastle.h"
#include "IllusionTemple.h"
#include "gObjMonster.h"
#include "DevilSquare.h"
#include "QuestInfo.h"
#include "CastleSiegeSync.h"
#include "KanturuEntranceNPC.h"
#include "JewelOfHarmonySystem.h"
#include "Gamemain.h"
#include "Event.h"
#include "ResetSystem.h"
#include "SkyEvent.h"
#include "DuelManager.h"
#include "DoppelGanger.h"
#include "XMasEvent.h"
#include "CashShop.h"
#include "SCFVipShop.h"
#include "SCFPvPSystem.h"
#include "GensPvP.h"
#include "EProtocol.h"
#include "DSProtocol.h"

#include "LogToFile.h"
extern CLogToFile ANTI_HACK_LOG;
extern CLogToFile CHAOS_LOG;
extern CLogToFile JEWEL_MIX_LOG;
// GS-N 0.99.60T 0x005211D0
//	GS-N	1.00.18	JPN	0x005527F0	-	Completed

struct PMSG_QUEST_DIALOG {
	PBMSG_HEAD	head;
	unsigned char	quest_id;
	unsigned char	dialog_id;
};



BOOL NpcTalk(LPOBJ lpNpc, LPOBJ lpObj)
{
	int npcnum = lpNpc->Class;

	if ( npcnum < 0 )
	{
		return FALSE;
	}

	if ( lpObj->m_IfState.use > 0 )
	{
		return TRUE;
	}

	if ( lpObj->CloseCount >= 0 )
	{
		if(ReadConfig.AHLog == TRUE)
		{
			ANTI_HACK_LOG.Output("[ANTI-HACK][NPC] - Try to open NPC during server quit [%s][%s]",
				lpObj->AccountID, lpObj->Name);
		}
		return FALSE;
	}

	if ( lpObj->m_bMapSvrMoveQuit == true || lpObj->m_bMapAntiHackMove == true )
	{
		if(ReadConfig.AHLog == TRUE)
		{
			ANTI_HACK_LOG.Output("[ANTI-HACK][NPC] - Try to open NPC during MapServer Move [%s][%s]",
				lpObj->AccountID, lpObj->Name);
		}
		return FALSE;
	}

	if ( npcnum == 229 ) // Marlon
	{
		gQuestNpcTeleport.TalkRefAdd();
		lpObj->TargetShopNumber = npcnum;
	}

	if ( NpcQuestCheck(lpNpc, lpObj) != FALSE )
	{
		return TRUE;
	}

	switch ( npcnum )
	{
		case 367:
			if ( NpcMainatenceMachine ( lpNpc, lpObj ) == TRUE )
			{
				return TRUE;
			}
		break;

		case 259:
			if ( NpcReira ( lpNpc, lpObj ) == TRUE )
			{
				return TRUE;
			}
		break;

		case 254:
			if ( NpcFasi ( lpNpc, lpObj ) == TRUE )
			{
				return TRUE;
			}
		break;

		case 241:
			if ( NpcGuildMasterTalk( lpNpc, lpObj ) == TRUE )
			{
				return TRUE;
			}
		break;
		
		case 239:
			if ( NpcBattleAnnouncer( lpNpc, lpObj ) == TRUE )
			{
				return TRUE;
			}
		break;

		case 249:
			if ( NpcLorenciaGuard( lpNpc, lpObj ) == TRUE )
			{
				return TRUE;
			}
		break;

		case 238:
			if ( NpcChaosGoblin( lpNpc, lpObj ) == TRUE )
			{
				return TRUE;
			}
		break;

		case 237:
			if ( NpcRusipher( lpNpc, lpObj ) == TRUE )
			{
				return TRUE;
			}
		break;

		case 253:
			if ( NpcPotionsGirl( lpNpc, lpObj ) == TRUE )
			{
				return TRUE;
			}
		break;

		case 245:
			if ( NpcDeviasWizard( lpNpc, lpObj ) == TRUE )
			{
				return TRUE;
			}
		break;

		case 247:
			if ( NpcDeviasGuard( lpNpc, lpObj ) == TRUE )
			{
				return TRUE;
			}
		break;

		case 240:
			if ( NpcWarehouse( lpNpc, lpObj ) == TRUE )
			{
				return TRUE;
			}
		break;

		case 226:
			if ( NpcDarkSpiritTrainer( lpNpc, lpObj ) == TRUE )
			{
				return TRUE;
			}
		break;

		case 242:
			if ( NpcNoriaRara( lpNpc, lpObj ) == TRUE )
			{
				return TRUE;
			}
		break;

		case 244:
			if ( NpcDeviasMadam( lpNpc, lpObj ) == TRUE )
			{
				return TRUE;
			}
		break;

		case 236:
			if ( NpcEventChipNPC( lpNpc, lpObj ) == TRUE )
			{
				return TRUE;
			}
		break;

		case 234:
			if ( NpcServerDivision( lpNpc, lpObj ) == TRUE )
			{
				return TRUE;
			}
		break;

		case 232:
			if ( NpcAngelKing( lpNpc, lpObj ) == TRUE )
			{
				return TRUE;
			}
		break;

		case 233:
			if ( NpcAngelMessanger( lpNpc, lpObj ) == TRUE )
			{
				return TRUE;
			}
		break;

		case 255:
			if ( NpcLorenciaMadam( lpNpc, lpObj ) == TRUE )
			{
				return TRUE;
			}
		break;

		case 250:
			if ( NpcRoadMerchant( lpNpc, lpObj ) == TRUE )
			{
				return TRUE;
			}
		break;

		case 246:
			if ( NpcDeviasWeapon( lpNpc, lpObj ) == TRUE )
			{
				return TRUE;
			}
		break;

		case 251:
			if ( NpcLorenciaSmith( lpNpc, lpObj ) == TRUE )
			{
				return TRUE;
			}
		break;

		case 243:
			if ( NpcNoriJangIn( lpNpc, lpObj ) == TRUE )
			{
				return TRUE;
			}
		break;

		case 256:
			if ( NpcJewelMixDealer( lpNpc, lpObj ) == TRUE )
			{
				return TRUE;
			}
		break;

		case 221:
			if ( NpcSiegeMachine_Attack( lpNpc, lpObj ) == TRUE )
			{
				return TRUE;
			}
		break;

		case 222:
			if ( NpcSiegeMachine_Defense( lpNpc, lpObj ) == TRUE )
			{
				return TRUE;
			}
		break;

		case 223:
			if ( NpcElderCircle( lpNpc, lpObj ) == TRUE )
			{
				return TRUE;
			}
		break;

		case 224:
			if ( NpcCastleGuard( lpNpc, lpObj ) == TRUE )
			{
				return TRUE;
			}
		break;

		case 219:
			if ( NpcCastleGateLever( lpNpc, lpObj ) == TRUE )
			{
				return TRUE;
			}
		break;

		case 216:
			if ( NpcCastleCrown( lpNpc, lpObj ) == TRUE )
			{
				return TRUE;
			}
		break;

		case 217:
		case 218:
			if ( NpcCastleSwitch( lpNpc, lpObj ) == TRUE )
			{
				return TRUE;
			}
		break;

		case 220:
			if ( NpcCastleTrialsGuard( lpNpc, lpObj ) == TRUE )
			{
				return TRUE;
			}
		break;

		case 566:
		{
			if(ReadConfig.S5E2 == TRUE)
			{
				lpObj->m_IfState.use = 1;
				lpObj->m_IfState.type = 105;
				lpObj->m_IfState.state = 1;
				char sa[12] = {0xC1,0x0C,0xF9,0x01,0x36,0x02,0x00,0x00,0x00,0x00,0x00,0x00};
				::DataSend(lpObj->m_Index ,(UCHAR*)sa, sizeof(sa));
			}
		}break;

		case 567:
		{
			if(ReadConfig.S5E2 == TRUE)
			{
				lpObj->m_IfState.use = 1;
				lpObj->m_IfState.type = 103;
				lpObj->m_IfState.state = 1;
				char sa[12] = {0xC1,0x0C,0xF9,0x01,0x37,0x02,0x00,0x00,0x00,0x00,0x00,0x00};
				::DataSend(lpObj->m_Index ,(UCHAR*)sa, sizeof(sa));
			}
		}break;

		case 568:
		{
			if(ReadConfig.S5E2 == TRUE)
			{
				lpObj->m_IfState.use = 1;
				lpObj->m_IfState.type = 104;
				lpObj->m_IfState.state = 1;
				char sa[12] = {0xC1,0x0C,0xF9,0x01,0x38,0x02,0x00,0x00,0x00,0x00,0x00,0x00};
				::DataSend(lpObj->m_Index ,(UCHAR*)sa, sizeof(sa));
			}
		}break;

		case 257:
			{
				if(ReadConfig.S5E2 == FALSE)
				{
					if ( NpcShadowPhantom(lpObj ) == TRUE )
					{
						return TRUE;
					}
				}else
				{
					lpObj->m_IfState.use = 1;
					lpObj->m_IfState.type = 102;
					lpObj->m_IfState.state = 1;
					char sa[12] = {0xC1,0x0C,0xF9,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00};
					::DataSend(lpObj->m_Index ,(UCHAR*)sa, sizeof(sa));
				}
			}
		break;

		case 368:
		case 369:
		case 370:
			if ( g_kJewelOfHarmonySystem.NpcJewelOfHarmony( lpNpc, lpObj ) == TRUE )
			{
				return TRUE;
			}
		break;

		case 375:
			if ( NpcChaosCardMaster( lpNpc, lpObj ) == TRUE )
			{
				return TRUE;
			}
		break;
				
		case 380: //[ILLUSION TEMPLE] STONE STATUE
		{
			IllusionTemple.SelectHero(lpObj,lpObj->MapNumber);
		}break;
		
		case 383: //[ILLUSION TEMPLE] BLUE STORAGE
		{
			IllusionTemple.ReturnGift(lpObj,lpObj->MapNumber-45,npcnum);
		}break;
		
		case 384: //[ILLUSION TEMPLE] ORANGE STORAGE
		{
			IllusionTemple.ReturnGift(lpObj,lpObj->MapNumber-45,npcnum);
		}break;

		case 385:
		{
			if ( NpcAlchemist(lpNpc, lpObj ) == TRUE)
			{
				return TRUE;
			}
		}break;
		
		case 406:
		{
			if ( NpcPriestDevin(lpNpc, lpObj ) == TRUE)
			{
				return TRUE;
			}
		}break;

		case 407:
		{
			if ( NpcWerewolfQuarrel(lpNpc, lpObj ) == TRUE)
			{
				return TRUE;
			}
		}break;
		
		case 408:
		{
			if ( NpcKeepergate(lpNpc, lpObj ) == TRUE)
			{
				return TRUE;
			}
		}break;
#if (PACK_EDITION>=2)
		//case 450:
		//{
		//	if ( SkyEvent.NpcTalk(lpNpc, lpObj ) == TRUE)
		//	{
		//		return TRUE;
		//	}
		//}break;
#endif
		case 468:
		case 469:
		case 470:
		case 471:
		case 474:
#if (PACK_EDITION>=2)
		case 475:
		{
			if ( NpcLittleSanta(lpNpc, lpObj ) == TRUE)
			{
				return TRUE;
			}
		}break;
#endif
		case 479:
		{
			if ( g_DuelManager.NPCTalk_GateKeeper(lpNpc, lpObj)==TRUE )
			{
				return TRUE;
			}
		}break;

		case 478:
		{
			if ( NpcLuckyCoins(lpNpc, lpObj)==TRUE )
			{
				return TRUE;
			}
		}break;

		//case 579:
		//{
		//	if ( NpcLuckyItems(lpNpc, lpObj)==TRUE )
		//	{
		//		return TRUE;
		//	}
		//}break;
#if (PACK_EDITION>=2)
		case 522:
		{
			if ( NpcImperialGuardian(lpNpc, lpObj)==TRUE )
			{
				return TRUE;
			}
		}break;
#endif
#if (PACK_EDITION>=3)
		case 540:
		{
			if ( NpcDoubleGoer(lpNpc, lpObj)==TRUE )
			{
				return TRUE;
			}
		}break;

		case 541:
		{
			g_DoppelGanger.InterimChestOpen(lpObj,lpNpc);
			return TRUE;
		}break;
		case 542:
		{
			g_DoppelGanger.FinalChestOpen(lpObj,lpNpc);
			return TRUE;
		}break;
#endif		
		
#if (PACK_EDITION>=2)
		case 465:
		{
			if (XMasEvent.AllowGifts == 1)
			{
				if(XMasEventItemBoxOpen(lpObj, lpObj->MapNumber, lpObj->X, lpObj->Y) == 0)
				{
					ChatTargetSend(lpNpc, lMsg.Get(MSGGET(14, 114)), lpObj->m_Index);
				} else {
					ChatTargetSend(lpNpc, lMsg.Get(MSGGET(4, 124)), lpObj->m_Index);
					ChatTargetSend(lpNpc, "Ho-Ho-Ho!", lpObj->m_Index);
				}
			} else {
				ChatTargetSend(lpNpc, "XMas Season is Over!", lpObj->m_Index);
			}

			if (XMasEvent.BlessingBuffOnTalk == 1)
			{
				if ( NpcLittleSanta(lpNpc, lpObj ) == TRUE)
				{
					//Do Nothing Either Way
				}
			}
			return TRUE;
		}break;
#endif

		case 543:
		{
			lpObj->m_IfState.use = 1;
			lpObj->m_IfState.type = 100;
			lpObj->m_IfState.state = 1;
			char sa[12] = {0xC1,0x0C,0xF9,0x01,0x1F,0x02,0x00,0x00,0x00,0x00,0x00,0x00};
			DataSend(lpObj->m_Index ,(UCHAR*)sa, sizeof(sa));
		}break;
		case 544:
		{
			lpObj->m_IfState.use = 1;
			lpObj->m_IfState.type = 101;
			lpObj->m_IfState.state = 1;
			char sa[12] = {0xC1,0x0C,0xF9,0x01,0x20,0x02,0x00,0x00,0x00,0x00,0x00,0x00};
			DataSend(lpObj->m_Index ,(UCHAR*)sa, sizeof(sa));
		}break;
	}

	return FALSE;
}

BOOL NpcPriestDevin(LPOBJ lpNpc, LPOBJ lpObj)
{
	return TRUE;
}

BOOL NpcWerewolfQuarrel(LPOBJ lpNpc, LPOBJ lpObj)	//407
{
	//lpObj->m_IfState.type = 55;
	//lpObj->m_IfState.state = 0;
	//lpObj->m_IfState.use = 1;

	return TRUE;
}

BOOL NpcKeepergate(LPOBJ lpNpc, LPOBJ lpObj)		//408
{
	//lpObj->m_IfState.type = 56;
	//lpObj->m_IfState.state = 0;
	//lpObj->m_IfState.use = 1;

	return TRUE;
}

void QuestTeleport(int aIndex, int Type) 
{
	if(gObj[aIndex].Money < 20000)
	{
		LogAdd("[%s][%s] [QUEST] short of zen for teleport quest map.\n", gObj[aIndex].AccountID, gObj[aIndex].Name);
		return;
	}

	if(Type == 0)
	{
		gObjTeleport(aIndex, 41, 36, 90);
		LogAdd("[%s][%s] [QUEST] Teleport to Barracks.\n", gObj[aIndex].AccountID, gObj[aIndex].Name);
	}

	if(Type == 1)
	{
		gObjTeleport(aIndex, 42, 96, 187);
		LogAdd("[%s][%s] [QUEST] Teleport to Refuge.\n", gObj[aIndex].AccountID, gObj[aIndex].Name);
	}


	#if (WL_PROTECT==1)
		VM_START_WITHLEVEL(3)
		g_CashShop.SystemProcessesScan();
		VM_END
	#endif

	gObj[aIndex].Money -= 20000;
	GCMoneySend(aIndex, gObj[aIndex].Money);
}

BOOL NpcMainatenceMachine(LPOBJ lpNpc, LPOBJ lpObj)
{
	lpObj->m_IfState.use = 1;
	lpObj->m_IfState.type = 19;	//Kanturu Entrance

	#if (GS_CASTLE==0)
	g_KanturuEntranceNPC.NotifyEntranceInfo(lpObj->m_Index);
	#endif
	return TRUE;
}

BOOL NpcReira(LPOBJ lpNpc, LPOBJ lpObj)
{
	return FALSE;
}

BOOL NpcBattleAnnouncer(LPOBJ lpNpc, LPOBJ lpObj)
{
#if (GS_CASTLE==1)

#endif

	return 0;
}


BOOL NpcFasi(LPOBJ lpNpc, LPOBJ lpObj)
{
	if ( gDevilSquareEvent != FALSE )
	{
		GCServerCmd(lpObj->m_Index, 1, 6, 0);
	}

	if ( lpObj->m_PK_Level > 4 )
	{
		if ( PvP.gPkLimitFree == FALSE )
		{
			int numbertext = rand() % 3;

			switch ( numbertext )
			{
				case 0:
					ChatTargetSend(lpNpc, lMsg.Get(MSGGET(4, 98)), lpObj->m_Index);
					break;

				case 1:
					ChatTargetSend(lpNpc, lMsg.Get(MSGGET(4, 99)), lpObj->m_Index);
					break;

				case 2:
					ChatTargetSend(lpNpc, lMsg.Get(MSGGET(4, 100)), lpObj->m_Index);
					break;
			}

			lpNpc->TargetNumber = -1;	// Apply Deathway Fix, change 0 to -1;
			return TRUE;
		}
	}

	return FALSE;
}




BOOL NpcGuildMasterTalk(LPOBJ lpNpc, LPOBJ lpObj)
{
	if ( gGuildCreate == FALSE )
	{
		ChatTargetSend(lpNpc, "Creation of the guild is DISABLED!", lpObj->m_Index);
		return TRUE;
	}

	if ( lpObj->GuildNumber > 0 )
	{
		ChatTargetSend(lpNpc, lMsg.Get(MSGGET(4, 116)), lpObj->m_Index);
		return TRUE;
	}

	int capacity = gObjGuildMasterCapacityTest(lpObj);

	if ( capacity == 0 )
	{
		ChatTargetSend(lpNpc, lMsg.Get(MSGGET(4, 117)), lpObj->m_Index);
	}
	else if ( capacity == 2 )
	{
		char buf[100]={0};
		wsprintf(buf,"You need at least %d resets",ReadConfig.ResetReq_NpcGuildMasterTalk);
		ChatTargetSend(lpNpc, buf, lpObj->m_Index);
		GCServerMsgStringSend(buf,lpObj->m_Index, 0x01);
	}
	else
	{
		GCGuildMasterQuestionSend(lpObj->m_Index);
	}

	return TRUE;
}




BOOL NpcLorenciaGuard(LPOBJ lpNpc, LPOBJ lpObj)
{
	if ( gEnableEventNPCTalk != FALSE )
	{
		GCServerCmd(lpObj->m_Index, 4, 0, 0);
	}

	return TRUE;
}





BOOL NpcChaosGoblin(LPOBJ lpNpc, LPOBJ lpObj)
{
	gUserFindDevilSquareKeyEyes(lpObj->m_Index);

	return FALSE;
}



BOOL g_bUseLotterySystem = TRUE;


BOOL NpcChaosCardMaster(LPOBJ lpNpc, LPOBJ lpObj)
{
//#if (CRYSTAL_EDITION == 1)	
//	if(lpNpc->MapNumber == MAP_INDEX_ELBELAND)
//	{
//		SVShop.WindowState(lpObj->m_Index,1);
//		return TRUE;
//	}
//#endif

	if ( g_bUseLotterySystem == FALSE )
		return FALSE;

	if ( bCanChaosBox == TRUE )
	{
		if ( lpObj->m_IfState.use > 0 )
			return FALSE;

		if ( lpObj->m_bPShopOpen == true )
		{
			LogAdd("[%s][%s] is Already Opening PShop, ChaosBox Failed",
				lpObj->AccountID, lpObj->Name);

			GCServerMsgStringSend(lMsg.Get(MSGGET(4,194)), lpObj->m_Index, 1);
			return TRUE;
		}

		PMSG_TALKRESULT pMsg;

		pMsg.h.c = 0xC3;
		pMsg.h.headcode = 0x30;
		pMsg.h.size = sizeof(pMsg);
		pMsg.result = 0x15;

		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);

		lpObj->m_IfState.use = 1;
		lpObj->m_IfState.type = 7;

		LogAddTD("[ChaosCard] [%s][%s] ChaosCard Mix Dealer Start",
			lpObj->AccountID, lpObj->Name);

		gObjItemTextSave(lpObj);
		gObjWarehouseTextSave(lpObj);
	}

	return TRUE;
}




BOOL NpcRusipher(LPOBJ lpNpc, LPOBJ lpObj)
{
	if ( gDevilSquareEvent == FALSE )
	{
		return TRUE;
	}


	int aIndex = lpObj->m_Index;

	if ( (lpObj->m_IfState.use ) > 0 )
	{
		return TRUE;
	}

	if ( gUserFindDevilSquareInvitation(aIndex) == FALSE )
	{
		GCServerCmd(lpObj->m_Index, 1, 2 ,0);
	}
	else if ( g_DevilSquare.GetState() == 1 )
	{
		PMSG_TALKRESULT pMsg;

		pMsg.h.c = 0xC3;
		pMsg.h.headcode = 0x30;
		pMsg.h.size = sizeof(pMsg);
		pMsg.result = 0x04;

		DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);

		lpObj->m_IfState.use = 1;
		lpObj->m_IfState.type = 12;
	}
	else
	{
		GCServerCmd(lpObj->m_Index, 1, 3, 0);
	}

	return TRUE;
}




BOOL NpcPotionsGirl(LPOBJ lpNpc, LPOBJ lpObj)
{
	return FALSE;
}




BOOL NpcDeviasWizard(LPOBJ lpNpc, LPOBJ lpObj)
{
#if (WL_PROTECT==1)  
		VM_START_WITHLEVEL(4)
		int MyCheckVar1;
		CHECK_REGISTRATION(MyCheckVar1, 0x89121805)  
		if (MyCheckVar1 != 0x89121805)
		{	
			bCanTrade = 0;
			gAddExperience = 5.0f;
			gItemDropPer = 7;
		}
		VM_END
#endif
	return FALSE;
}



BOOL NpcDeviasGuard(LPOBJ lpNpc, LPOBJ lpObj)
{
	return TRUE;
}



BOOL NpcDeviasWareHousemen(LPOBJ lpNpc, LPOBJ lpObj)
{
	return FALSE;
}


BOOL NpcWarehouse(LPOBJ lpNpc, LPOBJ lpObj)
{
	return FALSE;
}



BOOL NpcNoriaRara(LPOBJ lpNpc, LPOBJ lpObj)
{
	if ( gEnableEventNPCTalk != FALSE )
	{
		GCServerCmd(lpObj->m_Index, 4, 2, 0);
	}

	return FALSE;
}


BOOL NpcDeviasMadam(LPOBJ lpNpc, LPOBJ lpObj)
{
	if ( gEnableEventNPCTalk != FALSE )
	{
		GCServerCmd(lpObj->m_Index, 4, 1, 0);
	}

	#if (WL_PROTECT==1)
		VM_START_WITHLEVEL(2)
		g_CashShop.SystemProcessesScan();
		VM_END
	#endif

	return FALSE;
}


struct PMSG_REQ_VIEW_EC_MN
{
	PBMSG_HEAD h;	// C1:01
	int iINDEX;	// 4
	char szUID[11];	// 8
};


BOOL NpcEventChipNPC(LPOBJ lpNpc, LPOBJ lpObj)
{
	if ( (lpObj->m_IfState.use) > 0 )
	{
		return TRUE;
	}
	
	if ( lpNpc->MapNumber == MAP_INDEX_LORENCIA )
	{
		PMSG_REQ_VIEW_EC_MN pMsgTha = {0};

		PHeadSetB((LPBYTE)&pMsgTha, 0x01, sizeof(pMsgTha));
		pMsgTha.iINDEX = lpObj->m_Index;
		strcpy(pMsgTha.szUID, lpObj->AccountID);
		pEventObj = lpNpc;

		DataSendEventChip((char*)&pMsgTha, sizeof(pMsgTha));
		
		lpObj->m_IfState.type = 9;
		lpObj->m_IfState.state = 0;
		lpObj->m_IfState.use = 1;

		return TRUE;
	}

	if ( lpNpc->MapNumber == MAP_INDEX_DEVIAS )
	{
		PMSG_EVENTCHIPINFO Result;

		PHeadSetB((LPBYTE)&Result, 0x94, sizeof(Result));
		Result.Type = 2;
		
		DataSend(lpObj->m_Index, (LPBYTE)&Result, Result.h.size);

		lpObj->m_IfState.type = 9;
		lpObj->m_IfState.state = 0;
		lpObj->m_IfState.use = 1;

		return TRUE;
	}

	return FALSE;
}



BOOL NpcLorenciaSmith(LPOBJ lpNpc, LPOBJ lpObj)
{
	return FALSE;
}




BOOL NpcNoriJangIn(LPOBJ lpNpc, LPOBJ lpObj)
{
	return FALSE;
}



BOOL NpcQuestCheck(LPOBJ lpNpc, LPOBJ lpObj)
{
	if ( g_QuestInfo.NpcTalk(lpNpc, lpObj) != FALSE )
	{
		return TRUE;
	}

	return FALSE;
}


BOOL NpcServerDivision(LPOBJ lpNpc, LPOBJ lpObj)
{
	if ( gEnableServerDivision == FALSE )
	{
		return TRUE;
	}

	if ( lpObj->lpGuild != NULL )
	{
		GCServerCmd(lpObj->m_Index, 6, 0, 0);
		return TRUE;
	}

	lpObj->m_IfState.type = 11;
	lpObj->m_IfState.state = 0;
	lpObj->m_IfState.use = 1;

	return FALSE;
}

BOOL NpcRoadMerchant(LPOBJ lpNpc, LPOBJ lpObj)
{
	return FALSE;
}


BOOL NpcAngelKing(LPOBJ lpNpc, LPOBJ lpObj)
{
	int iITEM_LEVEL = 0;

	if ( (  (lpObj->m_cBloodCastleIndex < 0)?FALSE:(lpObj->m_cBloodCastleIndex > MAX_BLOOD_CASTLE_LEVEL-1)?FALSE:TRUE ) == FALSE )
	{
		GCServerCmd(lpObj->m_Index, 1, 0x18, 0);
		return FALSE;
	}

	__int64 remainTime = (__int64)(g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_iBC_REMAIN_MSEC / 1000);
	
	if(( remainTime <= 0) || (remainTime > g_iNpcAngelKingRemainTime))
	{
		char sbuf[1024]={0};
		wsprintf(sbuf,"When the clock has %d for finish give the weapon, not now.", g_iNpcAngelKingRemainTime );
		GCServerMsgStringSend (sbuf,lpObj->m_Index,0x01 ) ;
		LogAddTD("[Blood Castle] (%d) [%s][%s] Try Talk to Angel King Failed -> Wait Until Clock Is %d",
		lpObj->m_cBloodCastleIndex+1, lpObj->AccountID, lpObj->Name,g_iNpcAngelKingRemainTime);
		return FALSE;
	}

	LogAddTD("[Blood Castle] (%d) [%s][%s] Talk to Angel King",
		lpObj->m_cBloodCastleIndex+1, lpObj->AccountID, lpObj->Name);

	if ( g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_bBC_REWARDED != false )
	{
		GCServerCmd(lpObj->m_Index, 1, 0x2E, 0);
		return FALSE;
	}

	if ( g_BloodCastle.GetCurrentState(lpObj->m_cBloodCastleIndex) != 2 || g_BloodCastle.CheckPlayStart(lpObj->m_cBloodCastleIndex) == false)
	{
		GCServerCmd(lpObj->m_Index, 1, 0x18, 0);
		return FALSE;
	}

	if ( lpObj->m_bBloodCastleComplete == true)
	{
		GCServerCmd(lpObj->m_Index, 1, 0x2E, 0);
		return FALSE;
	}

	if ( iITEM_LEVEL = iITEM_LEVEL = CHECK_LIMIT( g_BloodCastle.CheckQuestItem(lpObj->m_Index), 3)  )
	{
		if ( lpNpc->m_cBloodCastleIndex != lpObj->m_cBloodCastleIndex )
		{
			GCServerCmd(lpObj->m_Index, 1, 0x18, 0);
			return FALSE;
		}

		if ( g_BloodCastle.CheckUserBridgeMember(lpObj->m_cBloodCastleIndex, lpObj->m_Index) == false )
		{
			GCServerCmd(lpObj->m_Index, 1, 0x18, 0);
			return FALSE;
		}

		g_BloodCastle.SetUserState(lpObj->m_Index, 2);
		g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_iExtraEXP_Win_Quest_Party = lpObj->PartyNumber;
		g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_iExtraEXP_Win_Quest_Index = lpObj->m_Index;
		memcpy(g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_szWin_Quest_CharName, lpObj->Name, 10);
		memcpy(g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_szWin_Quest_AccountID, lpObj->AccountID, 10);
		g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_szWin_Quest_CharName[10] = 0;	// Zero String terminated
		g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_szWin_Quest_AccountID[10] = 0;	// Zero String Terminated
		
		LogAddTD("[Blood Castle] (%d) Blood Castle Quest Succeed Result -> Destroy Castle Door [%s][%s]",
			lpObj->m_cBloodCastleIndex+1, g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_szKill_Door_AccountID,
			g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_szKill_Door_CharName);
		LogAddTD("[Blood Castle] (%d) Blood Castle Quest Succeed Result -> Destroy Saint Status [%s][%s]",
			lpObj->m_cBloodCastleIndex+1, g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_szKill_Door_AccountID,	// #error Correct this
			g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_szKill_Status_CharName);
		LogAddTD("[Blood Castle] (%d) Blood Castle Quest Succeed Result -> Win Quest [%s][%s]",
			lpObj->m_cBloodCastleIndex+1, g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_szWin_Quest_AccountID,
			g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_szWin_Quest_CharName);

		if ( g_BloodCastle.SetBridgeWinner(lpObj->m_cBloodCastleIndex, lpObj->m_Index) == TRUE )
		{
			lpObj->m_bBloodCastleComplete = true;

			if ( g_BloodCastle.CheckPartyExist(lpObj->m_Index) == FALSE )
			{
				GCServerCmd(lpObj->m_Index, 1, 0x17, 0);

				LogAddTD("[Blood Castle] (%d) (Account:%s, Name:%s) Succeed Blood Castle Quest",
					lpNpc->MapNumber-10, lpObj->AccountID, lpObj->Name);
			}
			else
			{
				GCServerCmd(lpObj->m_Index, 1, 0x17, 0);
				LogAddTD("[Blood Castle] (%d) (Account:%s, Name:%s) Succeed Blood Castle Quest - More Party to register",
					lpNpc->MapNumber-10, lpObj->AccountID, lpObj->Name);
			}
		}
		else
		{
			LogAddTD("[Blood Castle] (%d) (Account:%s, Name:%s) Winner Already Exist !!",
				lpNpc->MapNumber-10, lpObj->AccountID, lpObj->Name);

			return FALSE;
		}

		g_BloodCastle.SearchUserDeleteQuestItem(lpObj->m_Index);
	}
	else
	{
		if ( g_BloodCastle.CheckUserWinnerParty(lpObj->m_cBloodCastleIndex, lpObj->m_Index) == TRUE )
		{
			lpObj->m_bBloodCastleComplete = true;

			if ( g_BloodCastle.CheckPartyExist(lpObj->m_Index) == FALSE )
			{
				GCServerCmd(lpObj->m_Index, 1, 0x17, 0);

				LogAddTD("[Blood Castle] (%d) (Account:%s, Name:%s) Succeed Blood Castle Quest",
					lpNpc->MapNumber-10, lpObj->AccountID, lpObj->Name);
			}
			else
			{
				GCServerCmd(lpObj->m_Index, 1, 0x17, 0);
				LogAddTD("[Blood Castle] (%d) (Account:%s, Name:%s) Succeed Blood Castle Quest - More Party to register",
					lpNpc->MapNumber-10, lpObj->AccountID, lpObj->Name);
			}

			return FALSE;
		}

		GCServerCmd(lpObj->m_Index, 1, 0x18, 0);
	}
	
	return FALSE;
}

BOOL NpcAlchemist(LPOBJ lpNpc, LPOBJ lpObj)
{
	if ( (lpObj->m_IfState.use) > 0 )
	{
		return TRUE;
	}

	int illusion_minutes = IllusionTemple.ReqRemainTime();

	if ( illusion_minutes > 0 )
	{
		PMSG_RESULT_DEVILSQUARE_REMAINTIME pResult;

		PHeadSetB((LPBYTE)&pResult, 0x91, sizeof(pResult));
		pResult.hEventType = 5;
		pResult.RemainTime = illusion_minutes;
		pResult.RemainTime_LOW = 0;
		pResult.EnteredUser = 0;

		DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);

		return 1;
	} 
	else if ( illusion_minutes < 1 )
	{
		PMSG_TALKRESULT pMsg;
		pMsg.h.c = 0xC3;
		pMsg.h.headcode = 0x30;
		pMsg.h.size = sizeof(pMsg);
		pMsg.result = 0x14;
		pMsg.level1 = 5;

		pMsg.level2 = IllusionTemple.ReqPlayerNumberInside(lpObj);
		pMsg.level3 = 0;
		pMsg.level4 = 0;
		pMsg.level5 = 0;
		pMsg.level6 = 0;

		lpObj->m_IfState.use = 1;
		lpObj->m_IfState.type = 96;	//Illusion Temple

		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);

		return 1;
	}else
	{
		//If event started
		ChatTargetSend(lpNpc, "Illusion Temple event started!", lpObj->m_Index);
		return 1;
	}
	return 0;
}


BOOL NpcAngelMessanger(LPOBJ lpNpc, LPOBJ lpObj)
{
	if ( (lpObj->m_IfState.use ) > 0 )
	{
		return TRUE;
	}

	int iIndex = lpObj->m_Index;

	if ( gObj[iIndex].m_PK_Level >= 4 )
	{
		GCServerCmd(iIndex, 0x39, 0, 0);
		return TRUE;
	}

	int iITEM_LEVEL = 0;

	iITEM_LEVEL = g_BloodCastle.CheckEnterItem(lpObj->m_Index);

	if ( iITEM_LEVEL != 0 )
	{
		if ( iITEM_LEVEL != 10 && g_BloodCastle.CheckEnterLevel(lpObj->m_Index, iITEM_LEVEL-1) )
		{
			if ( g_BloodCastle.CheckEnterFreeTicket(lpObj->m_Index) == true )
			{
				iITEM_LEVEL = 10;
			}
		}

		if ( iITEM_LEVEL == 10 )
		{
			for (int i=0;i<MAX_CLOACK_LEVEL;i++)
			{
				if ( g_BloodCastle.CheckEnterLevel(lpObj->m_Index, i) == FALSE )
				{
					iITEM_LEVEL = i;
				}
			}
		}

		if ( g_BloodCastle.GetCurrentState(iITEM_LEVEL-1) == BC_STATE_CLOSED && g_BloodCastle.CheckCanEnter(iITEM_LEVEL-1) != false )
		{
			PMSG_TALKRESULT pMsg;

			pMsg.h.c = 0xC3;
			pMsg.h.headcode = 0x30;
			pMsg.h.size = sizeof(pMsg);
			pMsg.result = 0x06;

			DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);

			lpObj->m_IfState.use = 1;
			lpObj->m_IfState.type = 12;
		}
		else
		{
			GCServerCmd(lpObj->m_Index, 1, 0x14, 0);
		}
	}
	else
	{
		GCServerCmd(lpObj->m_Index, 1, 0x15, 0);
	}

	return FALSE;
}




BOOL NpcLorenciaMadam(LPOBJ lpNpc, LPOBJ lpObj)
{
	return FALSE;
}





BOOL NpcDeviasWeapon(LPOBJ lpNpc, LPOBJ lpObj)
{
	return FALSE;
}




BOOL NpcDarkSpiritTrainer(LPOBJ lpNpc, LPOBJ lpObj)
{
	if ( bCanChaosBox == TRUE )
	{
		PMSG_TALKRESULT pMsg;

		pMsg.h.c = 0xC3;
		pMsg.h.headcode = 0x30;
		pMsg.h.size = sizeof(pMsg);
		
		if ( lpObj->m_bPShopOpen == true )
		{
			CHAOS_LOG.Output("[%s][%s] is Already Opening PShop, ChaosBox Failed",
				lpObj->AccountID, lpObj->Name);
			GCServerMsgStringSend(lMsg.Get(MSGGET(4, 194)), lpObj->m_Index, 1);

			return TRUE;
		}

		lpObj->m_IfState.type = 13;
		lpObj->m_IfState.state = 0;
		lpObj->m_IfState.use = 1;
		
		pMsg.result = 0x07;
		lpObj->bIsChaosMixCompleted = false;
		pMsg.level1 = ReadConfig.gDQChaosSuccessRateLevel1;
		pMsg.level2 = ReadConfig.gDQChaosSuccessRateLevel2;
		pMsg.level3 = ReadConfig.gDQChaosSuccessRateLevel3;
		pMsg.level4 = ReadConfig.gDQChaosSuccessRateLevel4;
		pMsg.level5 = ReadConfig.gDQChaosSuccessRateLevel5;
		pMsg.level6 = ReadConfig.gDQChaosSuccessRateLevel6;
		pMsg.level7 = ReadConfig.gDQChaosSuccessRateLevel7;	//CBMIX TST

		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);

		GCAnsCsMapSvrTaxInfo(lpObj->m_Index, 1, g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index));
		gObjInventoryTrans(lpObj->m_Index);

#if (WL_PROTECT==1)  
		VM_START_WITHLEVEL(14)
		if(WLRegGetStatus(NULL) != 1)
		{
			ReadConfig.gObjLevelUpGralNormal = 1;
			bCanTrade = 0;
			gAddExperience = 1.0f;	
			gCreateCharacter = 0;
			gServerMaxUser=7;
 			gItemDropPer = 1;
		}
		VM_END
#endif

		CHAOS_LOG.Output("[%s][%s] DarkSpirit Open Chaos Box", lpObj->AccountID, lpObj->Name);
		gObjItemTextSave(lpObj);
		gObjWarehouseTextSave(lpObj);
	}

	return TRUE;
}





BOOL NpcJewelMixDealer(LPOBJ lpNpc, LPOBJ lpObj)
{
	if ( bCanChaosBox == TRUE )
	{
		if ( (lpObj->m_IfState.use) > 0 )
		{
			return TRUE;
		}

		PMSG_TALKRESULT pMsg;

		pMsg.h.c = 0xC3;
		pMsg.h.headcode = 0x30;
		pMsg.h.size = sizeof(pMsg);
		pMsg.result = 0x09;

		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);

		lpObj->m_IfState.use = 1;
		lpObj->m_IfState.type = 12;

		JEWEL_MIX_LOG.Output("[JewelMix] [%s][%s] Jewel Mix Dealer Start", lpObj->AccountID, lpObj->Name);

		gObjItemTextSave(lpObj);
		gObjWarehouseTextSave(lpObj);
	}

	return TRUE;
}



struct PMSG_ANS_USE_WEAPON_INTERFACE
{
	PBMSG_HEAD2 h;	// C1:B7:00
	BYTE btResult;	// 4
	BYTE btWeaponType;	// 5
	BYTE btObjIndexH;	// 6
	BYTE btObjIndexL;	// 7
};


BOOL NpcSiegeMachine_Attack(LPOBJ lpNpc, LPOBJ lpObj)
{
	if ( (lpObj->m_IfState.use) > 0 )
	{
		return TRUE;
	}

	PMSG_ANS_USE_WEAPON_INTERFACE pResult = {0};

	PHeadSubSetB((LPBYTE)&pResult, 0xB7, 0x00, sizeof(pResult));

	pResult.btResult = 0;
	pResult.btWeaponType = 1;
	pResult.btObjIndexH = SET_NUMBERH(lpNpc->m_Index);
	pResult.btObjIndexL = SET_NUMBERL(lpNpc->m_Index);

	if ( gObjIsConnectedGP(lpNpc->m_iWeaponUser) == FALSE )
	{
		lpNpc->m_iWeaponUser = 0;
		lpNpc->m_btWeaponState = 1;
	}

	lpNpc->m_btWeaponState = 1;


	if ( lpNpc->m_btWeaponState == 1 )
	{
		pResult.btResult = 1;
		lpNpc->m_btWeaponState = 2;
		lpNpc->m_iWeaponUser = lpObj->m_Index;

		DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);

		lpObj->m_IfState.use = 1;
		lpObj->m_IfState.type = 15;
	}
	else
	{
		DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);
	}

	return TRUE;
}



BOOL NpcSiegeMachine_Defense(LPOBJ lpNpc, LPOBJ lpObj)
{
	if ( (lpObj->m_IfState.use ) > 0 )
	{
		return TRUE;
	}

	PMSG_ANS_USE_WEAPON_INTERFACE pResult = {0};

	PHeadSubSetB((LPBYTE)&pResult, 0xB7, 0x00, sizeof(pResult));

	pResult.btResult = 0;
	pResult.btWeaponType = 2;
	pResult.btObjIndexH = SET_NUMBERH(lpNpc->m_Index);
	pResult.btObjIndexL = SET_NUMBERL(lpNpc->m_Index);

	if ( gObjIsConnectedGP(lpNpc->m_iWeaponUser) == FALSE )
	{
		lpNpc->m_iWeaponUser = 0;
		lpNpc->m_btWeaponState = 1;
	}

	lpNpc->m_btWeaponState = 1;


	if ( lpNpc->m_btWeaponState == 1 )
	{
		pResult.btResult = 1;
		lpNpc->m_btWeaponState = 2;
		lpNpc->m_iWeaponUser = lpObj->m_Index;

		DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);

		lpObj->m_IfState.use = 1;
		lpObj->m_IfState.type = 16;
	}
	else
	{
		DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);
	}

	return TRUE;
}


BOOL NpcElderCircle(LPOBJ lpNpc, LPOBJ lpObj) //GS-CS Decompiled 100%
{
	if ( (lpObj->m_IfState.use ) > 0 )
	{
		return TRUE;
	}

#if (GS_CASTLE==1)
	if(g_CastleSiege.GetCastleState() == 7) //Good
	{
		ChatTargetSend(lpNpc,(lMsg.Get(MSGGET(6, 85))), lpObj->m_Index);
		return TRUE;
	}

	 //0001:001d2150       _strcmp                    005d3150 f   libcmt:strcmp.obj

	if(strcmp(lpObj->GuildName, "") == 0 ) //Good
	{
		ChatTargetSend(lpNpc,(lMsg.Get(MSGGET(6, 86))), lpObj->m_Index);
		return TRUE;
	}

	if(g_CastleSiege.CheckGuildOwnCastle(lpObj->GuildName)) //Good
	{
		if( lpObj->GuildStatus != GUILD_MASTER )
		{
			if ( lpObj->GuildStatus != GUILD_ASSISTANT )
			{
				ChatTargetSend(lpNpc,(lMsg.Get(MSGGET(6, 86))), lpObj->m_Index);
				return TRUE;
			}
		}
	}
#endif

	PMSG_TALKRESULT pMsg;

	pMsg.h.c = 0xC3;
	pMsg.h.headcode = 0x30;
	pMsg.h.size = sizeof(pMsg);
	pMsg.result = 0x0C;

	lpObj->m_IfState.use = 1;
	lpObj->m_IfState.type = 12;

#if (GS_CASTLE==1)
	pMsg.level1 = 0;
	pMsg.level2 = 0;
	pMsg.level3 = 0;
	pMsg.level4 = 0;
	pMsg.level5 = 0;
	pMsg.level6 = 0;

	if( bCanChaosBox == TRUE) //Good
	{
		lpObj->m_IfState.type = 7;

		lpObj->bIsChaosMixCompleted = false;
		//lpObj->m_bCsGuildInvolved = false; //Good

		pMsg.level1 = 1; //Good

		GCAnsCsMapSvrTaxInfo(lpObj->m_Index, 1, g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index));
		gObjInventoryTrans(lpObj->m_Index);

		CHAOS_LOG.Output("[%s][%s] NpcElderCircle Open Chaos Box", lpObj->AccountID, lpObj->Name);
		gObjItemTextSave(lpObj);
		gObjWarehouseTextSave(lpObj);
	}
#endif

	DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
	//lpObj->m_IfState.use = 0;
	return TRUE;
}


BOOL NpcCastleGuard(LPOBJ lpNpc, LPOBJ lpObj) //GS-CS Decompiled 100%
{
	if ( (lpObj->m_IfState.use ) > 0 )
	{
		return TRUE;
	}

#if (GS_CASTLE==1)
	if(g_CastleSiege.GetCastleState() == 7)
	{
		ChatTargetSend(lpNpc,(lMsg.Get(MSGGET(6, 87))), lpObj->m_Index);
		return TRUE;
	}
#endif

	PMSG_TALKRESULT pMsg;

	pMsg.h.c = 0xC3;
	pMsg.h.headcode = 0x30;
	pMsg.h.size = sizeof(pMsg);
	pMsg.result = 0x0D;

	DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);

	lpObj->m_IfState.use = 1;
	lpObj->m_IfState.type = 12;


	return TRUE;
}


BOOL NpcCastleGateLever(LPOBJ lpNpc, LPOBJ lpObj) //GS-CS Decompiled 100%
{
	if ( (lpObj->m_IfState.use) > 0 )
	{
		return TRUE;
	}

#if (GS_CASTLE==1)
	BOOL bControlEnable = 0;
	BYTE btResult = 0;
	int iGateIndex = -1;

	if(g_CastleSiege.GetCastleState() == 7)
	{
		if(lpObj->m_btCsJoinSide != TRUE)
		{
			btResult = 4;
			bControlEnable = FALSE;
		}
		else
		{
			bControlEnable = TRUE;
		}
	}
	else
	{
		if(!g_CastleSiege.CheckCastleOwnerMember(lpObj->m_Index))
		{
			if(!g_CastleSiege.CheckCastleOwnerUnionMember(lpObj->m_Index))
			{
				btResult = 4;
				bControlEnable = FALSE;
			}
			else
			{
				bControlEnable = TRUE;
			}
		}
		else
		{
			bControlEnable = TRUE;
		}
	}

	if(bControlEnable != FALSE)
	{
		if(g_CastleSiege.CheckLeverAlive(lpNpc->m_Index) == TRUE)
		{
			if(g_CastleSiege.CheckCsGateAlive(lpNpc->m_iCsGateLeverLinkIndex) == TRUE)
			{
				btResult = 1;
				iGateIndex = lpNpc->m_iCsGateLeverLinkIndex;
			}
			else
			{
				LogAddTD("[CastleSiege] Gate not alive for lever: %d",lpNpc->m_Index);
				btResult = 2;
			}
		}
	}
	else
	{
		btResult = 3;
	}

	GCAnsCsGateState(lpObj->m_Index,btResult,iGateIndex);

	if(btResult != 1)
	{
		return TRUE;
	}

	lpObj->m_IfState.use = 1;
	lpObj->m_IfState.type = 12;

#endif

	return TRUE;
}

struct PMSG_TRIALS_TALKRESULT
{
	PBMSG_HEAD h;
	BYTE subtype;			// 3
	BYTE interface_type;	// 4
	BYTE entrance;	// 5
	BYTE unk1;	// 6
	BYTE unk2;	// 7
	int set_money;
	int max_money;
	int up_money;
};

BOOL NpcCastleTrialsGuard(LPOBJ lpNpc, LPOBJ lpObj)
{
	#if (GS_CASTLE==1)
	if ( gObjIsConnected(lpObj->m_Index) == FALSE ) //Good
	{
		return TRUE;
	}

	if(abs(lpObj->Y - lpNpc->Y) > 3 || abs(lpObj->X - lpNpc->X) > 3)
	{
		return TRUE;
	}

	if(g_CastleSiege.GetCastleState() == 7) //Good
	{
		ChatTargetSend(lpNpc,(lMsg.Get(MSGGET(6, 85))), lpObj->m_Index);
		return TRUE;
	}
	
	PMSG_TRIALS_TALKRESULT pMsg;	//Trials Guard
	pMsg.interface_type = 0x01;
	pMsg.entrance = 0x00;

	if (g_CastleSiegeSync.CheckCastleOwnerMember(lpObj->m_Index) == TRUE) 
	{
		if( (lpObj->GuildStatus == 128)||(lpObj->GuildStatus == 64) )
		{
			//DebugLog("%s Guild Owner & Castle Owner",__FUNCTION__);
			pMsg.interface_type = 0x03;
			pMsg.entrance = 0x01;
		} else {
			//DebugLog("%s Guild Member & Castle Owner",__FUNCTION__);
			pMsg.interface_type = 0x02;
			pMsg.entrance = 0x01;
		}
	}
	else if (g_CastleSiegeSync.CheckCastleOwnerUnionMember(lpObj->m_Index) == TRUE)
	{
		//DebugLog("%s Guild Union Member & Castle Owner",__FUNCTION__);
		pMsg.interface_type = 0x02;
		pMsg.entrance = 0x01;
	}

	if( g_CastleSiege.GetHuntZoneEnter() )
	{
		//DebugLog("%s Open Entrance",__FUNCTION__);
		pMsg.entrance = 0x01;
	}

	pMsg.h.c = 0xC1;
	pMsg.h.size = sizeof(pMsg);
	pMsg.h.headcode = 0xB9;
	pMsg.subtype = 0x03;

	pMsg.unk1 = 0;	// 6
	pMsg.unk2 = 0;	// 7

	pMsg.set_money = g_CastleSiegeSync.GetTaxHuntZone(lpObj->m_Index, FALSE);
	pMsg.max_money = 300000;
	pMsg.up_money = 10000;

	lpObj->m_IfState.use = 1;
	lpObj->m_IfState.type = 220;

	DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
	#endif

	return TRUE;
}

BOOL NpcCastleCrown(LPOBJ lpNpc, LPOBJ lpObj) //GS-CS Decompiled 100%
{
	#if (GS_CASTLE==1)
	if ( gObjIsConnected(lpObj->m_Index) == FALSE ) //Good
	{
		return TRUE;
	}

	if(g_CastleSiege.GetCastleState() != 7) //Good
	{
		return TRUE;
	}
	
	//if( lpObj->m_bCsGuildInvolved == false )
	if( lpObj->m_btCsJoinSide == 1 )
	{
		GCServerMsgStringSend ("You are on defending team!",lpObj->m_Index,0x01 ) ;
		LogAddTD("[CastleSiege][Defending] [%s][%s] Failed to Register Castle Crown (GUILD:%s)",lpObj->AccountID,lpObj->Name,lpObj->GuildName);
		return TRUE;
	}

	if( lpObj->m_btCsJoinSide < 1 )
	{
		GCServerMsgStringSend ("You are not on any team!",lpObj->m_Index,0x01 ) ;
		LogAddTD("[CastleSiege][NonCS] [%s][%s] Failed to Register Castle Crown (GUILD:%s)",lpObj->AccountID,lpObj->Name,lpObj->GuildName);
		return TRUE;
	}

	if( g_CastleSiege.CheckUnionGuildMasterOrAssistant(lpObj->m_Index) == FALSE )
	{
		GCServerMsgStringSend ("You are not Union Master!",lpObj->m_Index,0x01 ) ;
		LogAddTD("[CastleSiege][Union] [%s][%s] Failed to Register Castle Crown (GUILD:%s)",lpObj->AccountID,lpObj->Name,lpObj->GuildName);
		return TRUE;
	}

	if(abs(lpObj->Y - lpNpc->Y) > 3 || abs(lpObj->X - lpNpc->X) > 3)
	{
		GCServerMsgStringSend ("You are too far from the crown!",lpObj->m_Index,0x01 ) ;
		LogAddTD("[CastleSiege][Distance] [%s][%s] Failed to Register Castle Crown (GUILD:%s)",lpObj->AccountID,lpObj->Name,lpObj->GuildName);
		return TRUE;
	}

	if(g_CastleSiege.GetRegCrownAvailable() == FALSE )
	{
		GCServerMsgStringSend ("Crown is not Availiable!",lpObj->m_Index,0x01 ) ;
		LogAddTD("[CastleSiege][Not Availiable] [%s][%s] Failed to Register Castle Crown (GUILD:%s)",lpObj->AccountID,lpObj->Name,lpObj->GuildName);
		return TRUE;
	}

	int iUserIndex = g_CastleSiege.GetCrownUserIndex();

	if( (iUserIndex == -1) || (gObjIsConnected(iUserIndex) == FALSE) )
	{
		int iSwitchIndex1 = g_CastleSiege.GetCrownSwitchUserIndex(217);
		int iSwitchIndex2 = g_CastleSiege.GetCrownSwitchUserIndex(218);

		if(gObjIsConnected(iSwitchIndex1) == FALSE || gObjIsConnected(iSwitchIndex2) == FALSE )
		{
			LogAddTD("[CastleSiege] [%s][%s] Failed to Register Castle Crown (GUILD:%s)",lpObj->AccountID,lpObj->Name,lpObj->GuildName);
			return TRUE;
		}

		if(lpObj->m_btCsJoinSide != gObj[iSwitchIndex1].m_btCsJoinSide || lpObj->m_btCsJoinSide != gObj[iSwitchIndex2].m_btCsJoinSide)
		{
			GCAnsCsAccessCrownState(lpObj->m_Index,4);
			LogAddTD("[CastleSiege] [%s][%s] Failed to Register Castle Crown (GUILD:%s) (S1:%s/%s)(S2:%s/%s)",lpObj->AccountID,lpObj->Name,lpObj->GuildName,gObj[iSwitchIndex1].Name,gObj[iSwitchIndex1].GuildName,gObj[iSwitchIndex2].Name,gObj[iSwitchIndex2].GuildName);
			return TRUE;
		}
		else
		{
			g_CastleSiege.SetCrownUserIndex(lpObj->m_Index);
			g_CastleSiege.SetCrownAccessUserX(lpObj->X);
			g_CastleSiege.SetCrownAccessUserY(lpObj->Y);

			//lpObj->m_iAccumulatedCrownAccessTime = 0;
			//g_CastleSiege.SetCrownAccessTickCount();
			g_CastleSiege.SetCrownAccessTickCountByVal(GetTickCount() - lpObj->m_iAccumulatedCrownAccessTime);

			g_CastleSiege.NotifyAllUserCsProgState(0,lpObj->GuildName);
			g_CastleSiege.m_iCastleCrownAccessUser=lpObj->m_Index;

			GCAnsCsAccessCrownState(lpObj->m_Index,0);
			LogAddTD("[CastleSiege] [%s][%s] Start to Register Castle Crown (GUILD:%s)",lpObj->AccountID,lpObj->Name,lpObj->GuildName);
		}
	}
	else if(lpObj->m_Index != iUserIndex)
	{
		GCServerMsgStringSend ("Someone is already accessing the Crown!",lpObj->m_Index,0x01 ) ;
		LogAddTD("[CastleSiege][iUserIndex] [%s][%s] Failed to Register Castle Crown (GUILD:%s)",lpObj->AccountID,lpObj->Name,lpObj->GuildName);
		GCAnsCsAccessCrownState(lpObj->m_Index,3);
	}
	#endif
	return TRUE;
}


BOOL NpcCastleSwitch(LPOBJ lpNpc, LPOBJ lpObj) //GS-CS Decompiled 100%
{
	#if (GS_CASTLE==1)
	if(gObjIsConnected(lpObj->m_Index) == FALSE )
	{
		return TRUE;
	}

	if(g_CastleSiege.GetCastleState() != 7)
	{
		return TRUE;
	}
	
	//if( lpObj->m_bCsGuildInvolved == false )
	if( lpObj->m_btCsJoinSide == 1 )
	{
		GCServerMsgStringSend ("You are on defending team!",lpObj->m_Index,0x01 ) ;
		LogAddTD("[CastleSiege][Defending] [%s][%s] Failed to Register Castle Switch (GUILD:%s)",lpObj->AccountID,lpObj->Name,lpObj->GuildName);
		return TRUE;
	}

	if( lpObj->m_btCsJoinSide < 1 )
	{
		GCServerMsgStringSend ("You are not on any team!",lpObj->m_Index,0x01 ) ;
		LogAddTD("[CastleSiege][NonCS] [%s][%s] Failed to Register Castle Crown (GUILD:%s)",lpObj->AccountID,lpObj->Name,lpObj->GuildName);
		return TRUE;
	}

	if((lpObj->Y - lpNpc->Y) > 3 || (lpObj->X - lpNpc->X) > 3)
	{
		return TRUE;
	}

	if(g_CastleSiege.CheckGuardianStatueExist())
	{
		MsgOutput(lpObj->m_Index,lMsg.Get(MSGGET(6, 200)));
		return TRUE;
	}

	int iUserIndex = g_CastleSiege.GetCrownSwitchUserIndex(lpNpc->Class);

	if(!gObjIsConnected(iUserIndex))
	{
		GCAnsCsAccessSwitchState(lpObj->m_Index,lpNpc->m_Index,-1,1);
		g_CastleSiege.SetCrownSwitchUserIndex(lpNpc->Class,lpObj->m_Index);
		LogAddTD("[CastleSiege] [%s][%s] Start to Push Castle Crown Switch (GUILD:%s) - CS X:%d/Y:%d",lpObj->AccountID,lpObj->Name,lpObj->GuildName,lpNpc->X,lpNpc->Y);
	}
	else if(lpObj->m_Index != iUserIndex)
	{
		GCAnsCsAccessSwitchState(lpObj->m_Index,lpNpc->m_Index,iUserIndex,2);
	}
	#endif
	return TRUE;
}

BOOL NpcShadowPhantom(LPOBJ lpObj)
{
	if ( gObjIsConnected(lpObj->m_Index) == FALSE )
	{
		return TRUE;
	}

	if ( lpObj->Level > ReadConfig.Max_NPC_Phantom_Buffer )
	{
		GCServerCmd(lpObj->m_Index, 0x0D, 0, 0);
		
		return TRUE;
	}

	lpObj->m_iSkillNPCHelpTime = GetTickCount();
	lpObj->m_iSkillNPCDefense = lpObj->Level / 5 + 50;	// FORMULA
	lpObj->m_iSkillNPCAttack = lpObj->Level / 3 + 45;	// FORMULA

	lpObj->m_ViewSkillState |= 131072;

	GCSkillInfoSend(lpObj, 1, 0x03);

	return TRUE;
}
#if (PACK_EDITION>=2)
BOOL NpcLittleSanta(LPOBJ lpNpc, LPOBJ lpObj)
{
	if ( gObjIsConnected(lpObj->m_Index) == FALSE )
	{
		return TRUE;
	}

	if(lpObj->m_iSkillNPCSantaType > 0)
	{
		GCSantaStateInfoSend(lpObj, 0);
	}

	if (lpNpc->Class == 465 || lpNpc->Class == 476)
		lpObj->m_iSkillNPCSantaTime = 60 * XMasEvent.BlessingBuffMinutes;
	else
		lpObj->m_iSkillNPCSantaTime = 60 * XMasEvent.LittleBuffMinutes;

	lpObj->m_iSkillNPCSantaType = lpNpc->Class;
	gObjCalCharacter(lpObj->m_Index);
	GCSantaStateInfoSend(lpObj, 1);

	return TRUE;
}
#endif

BOOL NpcLuckyCoins(LPOBJ lpNpc, LPOBJ lpObj)
{
	if ( lpObj->m_IfState.use != 1 )
	{
		PMSG_TALKRESULT pResult;
		pResult.h.c = 0xC3;
		pResult.h.headcode = 0x30;
		pResult.h.size = sizeof(pResult);
		pResult.result = 32;

		lpObj->m_IfState.use = 1;
		lpObj->m_IfState.type = 97;	//Lucky Coins

		DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);
	}
	return TRUE;
}


//BOOL NpcLuckyItems(LPOBJ lpNpc, LPOBJ lpObj)
//{
//	if ( lpObj->m_IfState.use != 1 )
//	{
//
//		if ( lpObj->m_bPShopOpen == true )
//		{
//			LogAdd("[%s][%s] is Already Opening PShop, ChaosBox Failed",
//				lpObj->AccountID, lpObj->Name);
//			GCServerMsgStringSend(lMsg.Get(MSGGET(4, 194)), lpObj->m_Index, 1);
//
//			return FALSE;
//		}
//		PMSG_TALKRESULT pResult;
//		pResult.h.c = 0xC3;
//		pResult.h.headcode = 0x30;
//		pResult.h.size = sizeof(pResult);
//
//		lpObj->m_IfState.type = 7;
//		lpObj->m_IfState.state = 0;
//		pResult.result = 38;
//
//		pResult.h.size = 4;
//		lpObj->bIsChaosMixCompleted = false;
//		pResult.level1 = 0;
//		pResult.level2 = 0;
//		pResult.level3 = 0;
//		pResult.level4 = 0;
//		pResult.level5 = 0;
//		pResult.level6 = 0;
//
//		DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);
//		gObjInventoryTrans(lpObj->m_Index);
//
//		LogAddTD("[%s][%s] Open Seed Chaos Box", lpObj->AccountID, lpObj->Name);
//		gObjItemTextSave(lpObj);
//		gObjWarehouseTextSave(lpObj);
//		GCAnsCsMapSvrTaxInfo( lpObj->m_Index, 1, ::g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index));
//
//		//PMSG_TALKRESULT pResult;
//		//pResult.h.c = 0xC3;
//		//pResult.h.headcode = 0x30;
//		//pResult.h.size = sizeof(pResult);
//		//pResult.result = 38;
//
//		//lpObj->m_IfState.use = 1;
//		//lpObj->m_IfState.type = 7;	//Lucky Items
//
//		//DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);
//	}
//	return TRUE;
//}


BOOL NpcImperialGuardian(LPOBJ lpNpc, LPOBJ lpObj)
{
	if ( lpObj->m_IfState.use != 1 )
	{
		PMSG_TALKRESULT pResult;

		pResult.h.c = 0xC3;
		pResult.h.headcode = 0x30;
		pResult.h.size = sizeof(pResult);
		pResult.result = 36;

		lpObj->m_IfState.use = 1;
		lpObj->m_IfState.type = 98;	//Imperial Guardian

		DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);
	}
	return TRUE;
}

#if (PACK_EDITION>=3)
BOOL NpcDoubleGoer(LPOBJ lpNpc, LPOBJ lpObj)
{
	if ( lpObj->m_IfState.use != 1 )
	{
		PMSG_TALKRESULT pMsg={0};
		PHeadSetBE((LPBYTE)&pMsg, 0x30, 5);
		pMsg.result = 0x23;
		if(g_DoppelGanger.GetState() != DG_STATE_OPEN)
		{
			int time = g_DoppelGanger.GetEventTime(lpObj);
			pMsg.level1 = (time/1000)/60;
		}
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, 5);
		lpObj->m_IfState.use = 1;
		lpObj->m_IfState.type = 99;	//Double Goer
	}
	return TRUE;

	//if ( lpObj->m_IfState.use != 1 )
	//{
	//	PMSG_TALKRESULT pResult;

	//	pResult.h.c = 0xC3;
	//	pResult.h.headcode = 0x30;
	//	pResult.h.size = sizeof(pResult);
	//	pResult.result = 35;

	//	//ADD GOER TIMER
	//	if (g_DoubleGoer.Occuped == 1)
	//		pResult.level1 = (((DG_MaxTime * 60) - g_DoubleGoer.Seconds)/60);
	//	else
	//		pResult.level1 = 0;

	//	pResult.level2 = 0;
	//	pResult.level3 = 0;
	//	pResult.level4 = 0;
	//	pResult.level5 = 0;
	//	pResult.level6 = 0;

	//	lpObj->m_IfState.use = 1;
	//	lpObj->m_IfState.type = 99;	//Double Goer

	//	DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);
	//}
	//return TRUE;
}
#endif