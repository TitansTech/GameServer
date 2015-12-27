#include "stdafx.h"
#include "GameServer.h"
//#include "GameServerAuth.h"
#include "Shop.h"
#include "LogProc.h"
#include "..\include\readscript.h"
#include "GameMain.h"
#include "..\common\winutil.h"
#include "..\common\WzMemScript.h"
#include "MapServerManager.h"
#include "MossShop.h"
#include "gObjMonster.h"

cMossShop MossShop;

extern int MossItemType[5];
extern int MossItemIndex[5];

struct PMSG_BUYRESULT
{
	PBMSG_HEAD h;	// C1:32
	BYTE Result;	// 3
	BYTE ItemInfo[MAX_ITEM_INFO];	// 4
};


void cMossShop::StartEvent()
{
	if((this->Start == 0) && (this->Enabled == 1))
		_beginthread( MossShop__InsideTrigger, 0, NULL  );
}

void cMossShop::SetMonster()
{
	if(this->Enabled == 1 && this->Start == 1)
	{
		if (this->MobPos >= 0)
		{
			if (this->MobID == -1)
			{
				int MonsterID = gObjAddMonster(gMSetBase.m_Mp[this->MobPos].m_MapNumber);
				if(MonsterID >= 0)
				{
					this->MobID = MonsterID;
					gObjSetPosMonster(MonsterID, this->MobPos);
					gObjSetMonster(MonsterID, gMSetBase.m_Mp[this->MobPos].m_Type,"cMossShop::SetMonster");
				}
			} 
		
			if (this->MobID >= 0)
			{
				gObj[this->MobID].m_ViewSkillState &= ~0x200000;	//ViewSkillState 2097152
				GCSkillInfoSend(&gObj[this->MobID], 0, eVS_INVISIBLE);
			}
		}
	}
}

struct PMSG_HIDE_MOSS
{
	PBMSG_HEAD h;	// C1:17
	BYTE NumberH;	// 4
	BYTE NumberL;	// 5
	WORD Skill;	// 7
	BYTE KillerNumberH;	// 8
	BYTE KillerNumberL;	// 9
};

void cMossShop::ClearMonster()
{
	if(this->Enabled == 1 && this->Start == 1)
	{
		if (this->MobPos >= 0)
		{		
			if (this->MobID >= 0)
			{
				if (gObj[this->MobID].Class == this->MobNr)
				{
					//gObjDel(this->MobID);
					PMSG_HIDE_MOSS pDieMsg;

					PHeadSetB((LPBYTE)&pDieMsg, 0x17, sizeof(pDieMsg));
					pDieMsg.NumberH = SET_NUMBERH(this->MobID);
					pDieMsg.NumberL = SET_NUMBERL(this->MobID);
					pDieMsg.Skill = 0x00;
					pDieMsg.KillerNumberH = 0x00;
					pDieMsg.KillerNumberL = 0x00;

					MsgSendDistNear(&gObj[this->MobID], (UCHAR*)&pDieMsg, pDieMsg.h.size, 16);

					gObj[this->MobID].m_ViewSkillState |= 0x200000;
				}

				//this->MobID = -1;
			}
		}
	}
}

void cMossShop::Init(char * FilePath)
{
	this->Start = FALSE;
	this->Enabled				= GetPrivateProfileInt("Common", "SCFMossMerchantEnabled",0, FilePath) ;
	this->Minutes				= GetPrivateProfileInt("Common", "SCFMossMerchantMinutes",5, FilePath) ;

	this->LevelReq				= GetPrivateProfileInt("BuyRequeriments", "SCFMossMerchantMinLevel",380, FilePath) ;
	this->ResetsReq				= GetPrivateProfileInt("BuyRequeriments", "SCFMossMerchantResets",0, FilePath) ;
	this->ResetsDecEnabled		= GetPrivateProfileInt("BuyRequeriments", "SCFMossMerchantResetsDecEnabled",0, FilePath) ;
	this->OnlyForMarried		= GetPrivateProfileInt("BuyRequeriments", "SCFMossMerchantOnlyForMarried",0, FilePath) ;
	this->ZenReq				= GetPrivateProfileInt("BuyRequeriments", "SCFMossMerchantZen",2000000, FilePath) ;

	this->ItemEnabledReq		= GetPrivateProfileInt("BuyRequeriments", "SCFMossMerchantReqItemEnabled",0, FilePath) ;
	this->ItemTypeReq			= GetPrivateProfileInt("BuyRequeriments", "SCFMossMerchantReqItemType",0, FilePath) ;
	this->ItemIndexReq			= GetPrivateProfileInt("BuyRequeriments", "SCFMossMerchantReqItemIndex",0, FilePath) ;
	this->ItemCountReq			= GetPrivateProfileInt("BuyRequeriments", "SCFMossMerchantReqItemCount",0, FilePath) ;
	this->ItemLevelReq			= GetPrivateProfileInt("BuyRequeriments", "SCFMossMerchantReqItemLevel",0, FilePath) ;
	this->ItemOptionReq			= GetPrivateProfileInt("BuyRequeriments", "SCFMossMerchantReqItemOption",0, FilePath) ;
	this->ItemLuckReq			= GetPrivateProfileInt("BuyRequeriments", "SCFMossMerchantReqItemLuck",0, FilePath) ;
	this->ItemSkillReq			= GetPrivateProfileInt("BuyRequeriments", "SCFMossMerchantReqItemSkill",0, FilePath) ;
}

void cMossShop::LoadMonster()
{
	this->MobID = -1;
	this->MobPos = -1;
	this->MobNr = -1;

	if(this->Enabled == 1)
	{
		if (g_MapServerManager.CheckMapCanMove(MAP_INDEX_ELBELAND) == FALSE )
		{
			LogAddCTD(3,"[Moss Event] No need to load monster base file!");
			return;
		}

		int Pos = gObjMonsterAdd(492,MAP_INDEX_ELBELAND,23,226);
		if(Pos >= 0)
		{
			this->MobID = -1;
			this->MobPos = Pos;
			this->MobNr = 492;
		}
	}
}

void cMossShop::BuyItem(LPOBJ lpObj, BYTE Pos)
{
	if ( gObjIsConnectedGP(lpObj->m_Index) == FALSE )
	{
		LogAddC(2, "error-L3 [%s][%d]", __FILE__, __LINE__);
		return;
	}

	if (lpObj->PlayerExtraInfoReceived != 1)
	{
		GCServerMsgStringSend ("ERROR: Please relogin!",lpObj->m_Index,0x01 ) ;
		LogAddC(2, "ERROR: MOSS Info [%s][%s] Line: %d", lpObj->AccountID, lpObj->Name, __LINE__);
		return;
	}

	char sBuf[1024] = {0};
	CItem buyItem;
	PMSG_BUYRESULT pResult;
	PHeadSetB((LPBYTE)&pResult, 0x32, sizeof(pResult));
	pResult.Result = 0;

	int itemreq = ((this->ItemTypeReq * 512) + this->ItemIndexReq);
	if(this->Start == 0)
	{
		GCServerMsgStringSend ("Event is closed!",lpObj->m_Index,0x01 ) ;
		goto ExitFunction;
	}
	if(lpObj->Level < this->LevelReq)
	{
		wsprintf(sBuf,lMsg.Get(MSGGET(14, 31)),this->LevelReq);
		GCServerMsgStringSend (sBuf,lpObj->m_Index,0x01 ) ;
		goto ExitFunction;
	}
	if(lpObj->Resets < this->ResetsReq)
	{
		wsprintf(sBuf,lMsg.Get(MSGGET(14, 30)),this->ResetsReq);
		GCServerMsgStringSend (sBuf,lpObj->m_Index,0x01 ) ;
		goto ExitFunction;
	}
#if (PACK_EDITION>=2)
	if(lpObj->MarryCharacterInfo->Married == 0 && this->OnlyForMarried == 1)
	{
		GCServerMsgStringSend (lMsg.Get(MSGGET(14, 29)),lpObj->m_Index,0x01 ) ;
		goto ExitFunction;
	}
#endif
	if(lpObj->Money < this->ZenReq)
	{
		wsprintf(sBuf,lMsg.Get(MSGGET(14, 28)),this->ZenReq);
		GCServerMsgStringSend (sBuf,lpObj->m_Index,0x01 ) ;
		goto ExitFunction;
	}
	if((this->ItemEnabledReq == 1) && (gObjCheckItemsCount(lpObj->m_Index,itemreq,this->ItemLevelReq,this->ItemOptionReq,this->ItemLuckReq,this->ItemSkillReq) < this->ItemCountReq))
	{
		GCServerMsgStringSend (lMsg.Get(MSGGET(14, 27)),lpObj->m_Index,0x01 ) ;
		goto ExitFunction;
	}

	if(this->ItemEnabledReq == 1)
	{
		for(int i=0;i<this->ItemCountReq;i++)
			DeleteItemByMultipleValues(lpObj->m_Index,itemreq,this->ItemLevelReq,this->ItemOptionReq,this->ItemLuckReq,this->ItemSkillReq);
	}
	if(this->ZenReq > 0)
	{
		lpObj->Money -= this->ZenReq;
		CGZenSend(lpObj->m_Index);	
	}
	if(this->ResetsDecEnabled == 1 && this->ResetsReq > 0)
	{
		lpObj->Resets -= this->ResetsReq;
		lpObj->PlayerExtraInfoModified = 1;
	}

	int itemtype = 0;
	while(true)
	{
		if(ShopC[21].m_item[Pos].m_Type == ITEMGET(MossItemType[itemtype],MossItemIndex[itemtype]))
			break;
		
		if((itemtype+1) > 4)
			break;
		itemtype++;
	}

	int RandomItem = (rand()%(ShopC[21].moss_ItemCount[itemtype]));

	buyItem = ShopC[21].moss_item[itemtype][RandomItem];

	BYTE percent=0;

	//Skill
	if (buyItem.m_SkillOption > 0)
	{
		percent = (rand()%100 + 1);
		if (percent > buyItem.m_SkillOption)
		{
			buyItem.m_SkillOption = 0;
		}
	}

	//Luck
	if (buyItem.m_LuckOption > 0)
	{
		percent = (rand()%100 + 1);
		if (percent > buyItem.m_LuckOption)
		{
			buyItem.m_LuckOption = 0;
		}
	}

	//Excellent
	if (buyItem.m_NewOption > 200)
	{
		buyItem.m_NewOption = BoxExcOptions(buyItem.m_NewOption - 200);
	}

	//Z28
	if (buyItem.m_Z28Option > 0)
	{
		buyItem.m_Z28Option = (rand()%buyItem.m_Z28Option+1);
	}

	//Level
	if (buyItem.m_Level > 0)
	{
		buyItem.m_Level = (rand()%(buyItem.m_Level+1));
	}

	pResult.Result = gObjShopBuyInventoryInsertItem(lpObj->m_Index,buyItem);
				
	if ( pResult.Result != 0xFF )
	{
#if (WL_PROTECT==1)  
	VM_START_WITHLEVEL(19)
	int MyCheckVar1;
	CHECK_PROTECTION(MyCheckVar1, 0x19028607)  
	if (MyCheckVar1 != 0x19028607)
	{	
		WLRegDisableCurrentKey(wlMarkStolenKey);
		WLRegRemoveCurrentKey();
	}
	VM_END
#endif
		LogAddTD("[%s][%s] (%d) Moss buy [%s] LEV:%d, DUR:%d, OP:[%d][%d][%d]",
			lpObj->AccountID, lpObj->Name, 21, buyItem.GetName(),
			buyItem.m_Level, buyItem.m_Durability,
			buyItem.m_SkillOption, buyItem.m_LuckOption,
			buyItem.m_Z28Option);
		
		ItemByteConvert((LPBYTE)&pResult.ItemInfo, buyItem);
	}

ExitFunction:
	DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);
}

void MossShop__InsideTrigger(void * lpParam)
{
	if(MossShop.Enabled == 1)
	{
		MossShop.Start = 1;
		MossShop.SetMonster();
		//ShopC[21].LoadMossShopItem(ReadConfig.ConnShopDataFiles[21]); //reload shop for load again level, etc
		LogAddTD("[Moss] Appear in Elbeland");
		AllSendServerMsg(lMsg.Get(MSGGET(14, 25)));
		Sleep(MossShop.Minutes * 60000);
		LogAddTD("[Moss] Event End");
		AllSendServerMsg(lMsg.Get(MSGGET(14, 26)));
		MossShop.ClearMonster();

		MossShop.Start = 0;
	}
	_endthread();
}