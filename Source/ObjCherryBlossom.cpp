#include "stdafx.h"
#include "GameServer.h"
#include "LogProc.h"
#include "GameMain.h"
#include "User.h"
#include "ChaosBox.h"
#include "ObjCherryBlossom.h"
#include "DSProtocol.h"
#include "gObjMonster.h"
#include "..\common\winutil.h"
#include "..\include\readscript.h"
#include "..\common\SetItemOption.h"

#include "LogToFile.h"
extern CLogToFile ANTI_HACK_LOG;
extern CLogToFile CHAOS_LOG;

cCherryBlossom CherryBlossom;

void cCherryBlossom::SendDialog(int aIndex)
{	
	LPOBJ lpObj = &gObj[aIndex];
	if(this->Enabled == TRUE)
	{
		if ( bCanChaosBox == TRUE )
		{
			if ( lpObj->m_bPShopOpen == true )
			{
				LogAdd("[%s][%s] is Already Opening PShop, ChaosBox Failed",
					lpObj->AccountID, lpObj->Name);

				GCServerMsgStringSend(lMsg.Get(MSGGET(4,194)), lpObj->m_Index, 1);
				return;
			}


			PMSG_TALKRESULT pResult;
			pResult.h.c = 0xC3;
			pResult.h.headcode = 0x30;
			pResult.h.size = sizeof(pResult);
			pResult.result = 22;
			::DataSend(gObj[aIndex].m_Index, (LPBYTE)&pResult, pResult.h.size);


			lpObj->m_IfState.use = 1;
			lpObj->m_IfState.type = 7;

			LogAddTD("[CherryBlossom] [%s][%s] CherryBlossom Mix Governor Blossom Start",
				lpObj->AccountID, lpObj->Name);

			gObjItemTextSave(lpObj);
			gObjWarehouseTextSave(lpObj);
		}
	}
}

bool cCherryBlossom::GetReward(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];
	lpObj->ChaosLock = TRUE;

	PMSG_CHAOSMIXRESULT pMsg;

	PHeadSetB((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR;

	int iItemCount = 0;
	int iInvalidItemCount = 0;
	int iItemHeight = 0;
	int iItemWidth = 0;

	if (OBJMAX_RANGE(aIndex) == 0)
	{
		return false;
	}

	for ( int n=0;n<CHAOS_BOX_SIZE;n++)
	{
		if ( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,90) )
			{
				if(lpObj->pChaosBox[n].m_Durability == 255.0f)
					iItemCount++;
				else
					iInvalidItemCount++;
			}
			else
			{
				iInvalidItemCount++;
			}
		}
	}

	if ( iInvalidItemCount > 0 || iItemCount > 1 || iItemCount == 0 )
	{
		pMsg.Result = CB_TOO_MANY_ITEMS;
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;
		lpObj->m_IfState.use = 0;
		lpObj->m_IfState.type = 0;

		return false;
	}

	LogChaosItem(lpObj, "CherryBlossom");
	iItemHeight = 4;
	iItemWidth = 2;

	if ( !CheckInventoryEmptySpace(lpObj, iItemHeight, iItemWidth))
	{
		pMsg.Result = CB_ERROR;
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		lpObj->m_IfState.use = 0;
		lpObj->m_IfState.type = 0;

		return false;
	}
	
	int RandNum = rand()%this->iCount;
		
	//BYTE TempInventoryMap[BASIC_INVENTORY_SIZE + (MAX_EXTENDED_INV*EXTENDED_INV_SIZE)];
	BYTE TempInventoryMap[MAIN_INVENTORY_NORMAL_SIZE + (MAX_EXTENDED_INV*EXTENDED_INV_SIZE) + PSHOP_SIZE];
	memcpy(TempInventoryMap,gObj[aIndex].pInventoryMap,ReadConfig.GETSLOTS(aIndex));

	if(gObjTempInventoryInsertItem(&gObj[aIndex],this->items[RandNum].num,TempInventoryMap) != 255)
	{	
		pMsg.Result = CB_SUCCESS;	

		CItem Item;

		int nLevel = rand()%(this->items[RandNum].Level+1);
		int nOpt = rand()%(this->items[RandNum].Opt+1);
		int nLuck = 0;
		int nSkill = 0;
		int nAnc = 0;
		int nExc = 0;

		if (rand()%100 < this->items[RandNum].pExc)
			int nExc = BoxExcOptions(this->items[RandNum].Exc);
		if (rand()%100 < this->items[RandNum].Luck)
			nLuck = 1;
		if (rand()%100 < this->items[RandNum].Skill)
			nSkill = 1;

		if (gSetItemOption.IsSetItem(this->items[RandNum].num) != 0)
		{
			if (rand()%100 < this->items[RandNum].Anc)
			{
				nAnc = gSetItemOption.GenSetOption(this->items[RandNum].num);
				nExc = 0;
			}
		}

		Item.m_Level = nLevel;
		Item.m_Durability = this->items[RandNum].Dur;

		Item.m_ItemSlot1 = 0;
		Item.m_ItemSlot2 = 0;
		Item.m_ItemSlot3 = 0;
		Item.m_ItemSlot4 = 0;
		Item.m_ItemSlot5 = 0;

		Item.Convert(this->items[RandNum].num, nSkill, nLuck,
			nOpt, nExc, 
			nAnc, 0, CURRENT_DB_VERSION);

		Item.m_Number = 0;

		ItemByteConvert(pMsg.ItemInfo, Item);
		ChaosBoxInit(lpObj);
		::gObjChaosBoxInsertItemPos(lpObj->m_Index, Item, 0, -1);
		gObjChaosItemSet(lpObj->m_Index, 0, 1);

		//ItemSerialCreateSend(aIndex, -1, 0, 0, this->items[RandNum].num, this->items[RandNum].Level, this->items[RandNum].Dur, this->items[RandNum].Skill, this->items[RandNum].Luck, (this->items[RandNum].Opt), -1, this->items[RandNum].Exc, this->items[RandNum].Anc);
		//GCUserChaosBoxSend(lpObj, 0);
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
	}else
	{
		pMsg.Result = CB_ERROR;
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;
		lpObj->m_IfState.use = 0;
		lpObj->m_IfState.type = 0;

		return false;
	}
	gObjInventoryCommit(lpObj->m_Index);

	CHAOS_LOG.Output("[ChaosMix][CherryBlossomItemMix] Cherry Blossom Item Mix Success [%s][%s]",
		lpObj->AccountID, lpObj->Name);
	
	CHAOS_LOG.Output("[ChaosMix][CherryBlossomItemMix] - User(ID:%s,Name:%s) Item(Name:%s,Type:%d,Level:%d,Dur:%d,Skill:%d,Luck:%d,AddOption:%d,Ex:%d,Anc:%d)",
		lpObj->AccountID, lpObj->Name, ItemAttribute[this->items[RandNum].num].Name,
		this->items[RandNum].num, this->items[RandNum].Level, this->items[RandNum].Dur,
		this->items[RandNum].Skill, this->items[RandNum].Luck, this->items[RandNum].Opt,
		this->items[RandNum].Exc,this->items[RandNum].Anc);

	lpObj->ChaosLock = FALSE;
	lpObj->m_IfState.use = 0;
	lpObj->m_IfState.type = 0;

	return true;
}

void cCherryBlossom::Read(char * FilePath)
{	
	this->Enabled = GetPrivateProfileInt("Common", "SCFCherryBlossomEnabled",0, FilePath);

	LogAddTD("[SCFCherryBlossom] - %s file is Loaded",FilePath);
}

void cCherryBlossom::ReadItems(char * FilePath)
{
	if(this->Enabled == TRUE)
	{
		int Num=0;
		int Token;
		SMDFile = fopen(FilePath, "r");

		this->iCount = 0;

		if ( SMDFile == NULL )
		{
			MsgBox("CherryBlossom data load error %s", FilePath);
			return;
		}

		while ( true )
		{
			Num=0;
			int iType = GetToken();
			
			if ( iType == 1 )
			{
				while(true)
				{
					Token = GetToken();
					if ( strcmp("end", TokenString) == 0 )
					{
						break;
					}

					if(Num >= CHERRYBLOSSOM_REWARDITEMS_COUNT)
					{
						MsgBox("CherryBlossom error: Items out of range!");
						return;
					}

					int iType = TokenNumber;

					Token = GetToken();
					int iIndex = TokenNumber;

					this->items[Num].num = ITEMGET(iType,iIndex);

					Token = GetToken();
					this->items[Num].Level = TokenNumber;

					Token = GetToken();
					this->items[Num].Opt = TokenNumber;

					Token = GetToken();
					this->items[Num].Luck = TokenNumber;

					Token = GetToken();
					this->items[Num].Skill = TokenNumber;

					Token = GetToken();
					this->items[Num].Dur = TokenNumber;

					Token = GetToken();
					this->items[Num].Exc = TokenNumber;

					Token = GetToken();
					this->items[Num].pExc = TokenNumber;

					Token = GetToken();
					this->items[Num].Anc = TokenNumber;

					Num++;
					this->iCount++;
				}
			}
			break;
		}
				
		LogAddTD("[SCFCherryBlossom] - %s file is Loaded",FilePath);
		fclose(SMDFile);
	}
}