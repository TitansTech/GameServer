#include "stdafx.h"

#if (CRYSTAL_EDITION == 1)
#if (PACK_EDITION>=3)

#include "GameServer.h"
#include "LogProc.h"
#include "GameMain.h"
#include "User.h"
#include "..\common\winutil.h"
#include "..\include\readscript.h"
#include "LogToFile.h"
#include "SCFVipShop.h"
#include "MuItemShop.h"


CLogToFile ANTI_HACK_VIPSHOP_LOG("ANTI_HACK_LOG", ".\\SCFVIPSHOP_LOG", 1);	// line : 68
CLogToFile VIPSHOP_LOG("ITEM_SELL_LOG", ".\\SCFVIPSHOP_LOG", 1);	// line : 68
SCFVipShop SVShop;

void SCFVipShop::Init(char * Ini, char * FilePath)
{
	this->Enabled		= GetPrivateProfileInt("Common", "SCFVipShopEnabled",0, Ini) ;
	this->IsForeverFFFE	= GetPrivateProfileInt("Common", "SCFVipShopForeverItemIsFFFFFFFE",0, Ini) ;

	this->pLevel		= GetPrivateProfileInt("Cost", "SCFVipShopExtraLevel",0, Ini) ;
	this->pOpt			= GetPrivateProfileInt("Cost", "SCFVipShopExtraOpt",0, Ini) ;
	this->pExc			= GetPrivateProfileInt("Cost", "SCFVipShopExtraExc",0, Ini) ;
	this->pLuck			= GetPrivateProfileInt("Cost", "SCFVipShopExtraLuck",0, Ini) ;
	this->pSkill		= GetPrivateProfileInt("Cost", "SCFVipShopExtraSkill",0, Ini) ;
	this->pHarmony		= GetPrivateProfileInt("Cost", "SCFVipShopExtraHarmony",0, Ini) ;
	this->pSocket		= GetPrivateProfileInt("Cost", "SCFVipShopExtraSocket",0, Ini) ;
	this->pFreeSocket	= GetPrivateProfileInt("Cost", "SCFVipShopExtraSocketFree",0, Ini) ;
	this->pSocketLevel	= GetPrivateProfileInt("Cost", "SCFVipShopExtraSocketLevel",0, Ini) ;

	this->MaxLevel		= GetPrivateProfileInt("Max", "SCFVipShopLevel",0, Ini) ;
	this->MaxOpt		= GetPrivateProfileInt("Max", "SCFVipShopOpt",0, Ini) ;
	this->MaxExc		= GetPrivateProfileInt("Max", "SCFVipShopExc",0, Ini) ;
	this->MaxLuck		= GetPrivateProfileInt("Max", "SCFVipShopLuck",0, Ini) ;
	this->MaxSkill		= GetPrivateProfileInt("Max", "SCFVipShopSkill",0, Ini) ;
	this->MaxHarmony	= GetPrivateProfileInt("Max", "SCFVipShopHarmony",0, Ini) ;
	this->MaxSock		= GetPrivateProfileInt("Max", "SCFVipShopSockets",0, Ini) ;
	this->MaxSockLevel	= GetPrivateProfileInt("Max", "SCFVipShopSocketLevel",0, Ini) ;

	this->TypeCount = 0;

	for(int i=0;i<(MAX_TYPE_ITEMS*MAX_SUBTYPE_ITEMS);i++)
	{
		this->item[i].Enabled = false;
	}

	this->Read(FilePath);
}

void SCFVipShop::SendInfo(int aIndex)
{
	if(this->Enabled == 1)
	{
		LPOBJ lpObj = &gObj[aIndex];
		SDHP_SENDSVSHOP pInfo;
		PHeadSubSetB((LPBYTE)&pInfo, 0xFA,0x00, sizeof(pInfo));
		pInfo.mLevel=this->MaxLevel;
		pInfo.mOpt=this->MaxOpt;
		pInfo.mExc=this->MaxExc;
		pInfo.mSkill=this->MaxSkill;
		pInfo.mLuck=this->MaxLuck;
		pInfo.mHarmony=this->MaxHarmony;
		pInfo.mSock=this->MaxSock;
		pInfo.mSockLevel=this->MaxSockLevel;
		pInfo.pLevel=this->pLevel;
		pInfo.pOpt=this->pOpt;
		pInfo.pExc=this->pExc;
		pInfo.pSkill=this->pSkill;
		pInfo.pLuck=this->pLuck;
		pInfo.pHarmony=this->pHarmony;
		pInfo.pSock=this->pSocket;
		pInfo.pFreeSock=this->pFreeSocket;
		pInfo.pSockLevel=this->pSocketLevel;

		DataSend(lpObj->m_Index, (LPBYTE)&pInfo, sizeof(pInfo));
	}
}

void SCFVipShop::WindowState(int aIndex,BYTE State) //State 1 = Open; State 0 = Close
{
	if(this->Enabled == 1)
	{
		if(gObjIsConnected(aIndex) == 0)
		{
			return;
		}

		gObj[aIndex].VipShopOpen = State;
		LPOBJ lpObj = &gObj[aIndex];
		SDHP_OPENVSHOP pInfo;
		PHeadSubSetB((LPBYTE)&pInfo, 0xFA,0x01, sizeof(pInfo));
		pInfo.VipMoney=lpObj->VipMoney;
		pInfo.Open=State;
		DataSend(lpObj->m_Index, (LPBYTE)&pInfo, sizeof(pInfo));
	}
}

void SCFVipShop::ProtocolCore(int aIndex,BYTE* aRecv)
{
	if(this->Enabled == 1)
	{
		int SubCode = aRecv[3];
		switch(SubCode)
		{
			case 0x01:
			{
				this->RecvBuy(aIndex,(SDHP_BUYSVSHOP*)aRecv);
			}break;
			case 0x02:
			{
				if(gObjIsConnected(aIndex) == 0)
				{
					return;
				}
				gObj[aIndex].VipShopOpen = 0;
			}break;
		}
	}
}

void SCFVipShop::RecvBuy(int aIndex,SDHP_BUYSVSHOP * lpMsg)
{	
	if(gObjIsConnected(aIndex) == 0)
	{
		return;
	}
	LPOBJ lpObj = &gObj[aIndex];

	int id=lpMsg->ItemID;
	int ExOpCount = getNumberOfExcOptions( lpMsg->mExc );
	int SockOpCount = 0;
	if(this->item[id].Enabled == true)
	{
		char sbuf[512]={0};
		if(lpObj->VipShopOpen == 0)
		{
			//Hack attempt
			wsprintf(sbuf,"[Anti-HACK][SCFVipShop] Attempted Character-Buy item without WINDOW [%s][%s]",
				gObj[aIndex].AccountID,gObj[aIndex].Name);

			GCServerMsgStringSend(sbuf, lpObj->m_Index, 0x00);
			LogAddTD(sbuf);
			ANTI_HACK_VIPSHOP_LOG.Output(sbuf);
			return;
		}
		if(lpMsg->mLevel > this->MaxLevel)
		{
			//Hack attempt
			wsprintf(sbuf,"[Anti-HACK][SCFVipShop] Attempted Character-Buy item with Hack in LEVEL [%s][%s]",
				gObj[aIndex].AccountID,gObj[aIndex].Name);

			GCServerMsgStringSend(sbuf, lpObj->m_Index, 0x00);
			LogAddTD(sbuf);
			ANTI_HACK_VIPSHOP_LOG.Output(sbuf);
			return;
		}
		if(lpMsg->mOpt > this->MaxOpt)
		{
			//Hack attempt
			wsprintf(sbuf,"[Anti-HACK][SCFVipShop] Attempted Character-Buy item with Hack in OPT [%s][%s]",
				gObj[aIndex].AccountID,gObj[aIndex].Name);

			GCServerMsgStringSend(sbuf, lpObj->m_Index, 0x00);
			LogAddTD(sbuf);
			ANTI_HACK_VIPSHOP_LOG.Output(sbuf);
			return;
		}
		if(lpMsg->mLuck > this->MaxLuck)
		{
			//Hack attempt
			wsprintf(sbuf,"[Anti-HACK][SCFVipShop] Attempted Character-Buy item with Hack in LUCK [%s][%s]",
				gObj[aIndex].AccountID,gObj[aIndex].Name);

			GCServerMsgStringSend(sbuf, lpObj->m_Index, 0x00);
			LogAddTD(sbuf);
			ANTI_HACK_VIPSHOP_LOG.Output(sbuf);
			return;
		}
		if(lpMsg->mSkill > this->MaxSkill)
		{
			//Hack attempt
			wsprintf(sbuf,"[Anti-HACK][SCFVipShop] Attempted Character-Buy item with Hack in SKILL [%s][%s]",
				gObj[aIndex].AccountID,gObj[aIndex].Name);

			GCServerMsgStringSend(sbuf, lpObj->m_Index, 0x00);
			LogAddTD(sbuf);
			ANTI_HACK_VIPSHOP_LOG.Output(sbuf);
			return;
		}
		if(this->MaxHarmony == 0 && lpMsg->mHarmony > 0)
		{
			//Hack attempt
			wsprintf(sbuf,"[Anti-HACK][SCFVipShop] Attempted Character-Buy item with Hack in HARMONY [%s][%s]",
				gObj[aIndex].AccountID,gObj[aIndex].Name);

			GCServerMsgStringSend(sbuf, lpObj->m_Index, 0x00);
			LogAddTD(sbuf);
			ANTI_HACK_VIPSHOP_LOG.Output(sbuf);
			return;
		}
		if(ExOpCount > this->MaxExc)
		{
			//Hack attempt
			wsprintf(sbuf,"[Anti-HACK][SCFVipShop] Attempted Character-Buy item with Hack in EXC [%s][%s]",
				gObj[aIndex].AccountID,gObj[aIndex].Name);

			GCServerMsgStringSend(sbuf, lpObj->m_Index, 0x00);
			LogAddTD(sbuf);
			ANTI_HACK_VIPSHOP_LOG.Output(sbuf);
			return;
		}

		if(lpMsg->mSock1 > 0)
			SockOpCount++;
		if(lpMsg->mSock2 > 0)
			SockOpCount++;
		if(lpMsg->mSock3 > 0)
			SockOpCount++;
		if(lpMsg->mSock4 > 0)
			SockOpCount++;
		if(lpMsg->mSock5 > 0)
			SockOpCount++;
		
		if(SockOpCount > this->MaxSock)
		{
			//Hack attempt
			wsprintf(sbuf,"[Anti-HACK][SCFVipShop] Attempted Character-Buy item with Hack in SOCKET [%s][%s]",
				gObj[aIndex].AccountID,gObj[aIndex].Name);

			GCServerMsgStringSend(sbuf, lpObj->m_Index, 0x00);
			LogAddTD(sbuf);
			ANTI_HACK_VIPSHOP_LOG.Output(sbuf);
			return;
		}

		BYTE SockLevel1 = lpMsg->mSock1/50;
		BYTE SockLevel2 = lpMsg->mSock2/50;
		BYTE SockLevel3 = lpMsg->mSock3/50;
		BYTE SockLevel4 = lpMsg->mSock4/50;
		BYTE SockLevel5 = lpMsg->mSock5/50;

		if(this->MaxSock > 0)
		{
			if( (SockLevel1 > this->MaxSockLevel && lpMsg->mSock1 != 255 ) || 
				(SockLevel2 > this->MaxSockLevel && lpMsg->mSock2 != 255 ) || 
				(SockLevel3 > this->MaxSockLevel && lpMsg->mSock3 != 255 ) || 
				(SockLevel4 > this->MaxSockLevel && lpMsg->mSock4 != 255 ) || 
				(SockLevel5 > this->MaxSockLevel && lpMsg->mSock5 != 255 )
				)
			{
				//Hack attempt
				wsprintf(sbuf,"[Anti-HACK][SCFVipShop] Attempted Character-Buy item with Hack in SOCKET (Exceed Max Level) [%s][%s]",
					gObj[aIndex].AccountID,gObj[aIndex].Name);

				GCServerMsgStringSend(sbuf, lpObj->m_Index, 0x00);
				LogAddTD(sbuf);
				ANTI_HACK_VIPSHOP_LOG.Output(sbuf);
				return;
			}
		}
		
		if(ReadConfig.Soket__AutoRemoveSameSocket == 1)
		{
			bool SockSame = false;
			if (lpMsg->mSock1 != 255 && lpMsg->mSock1 != 0)
			{
				if (lpMsg->mSock1 == lpMsg->mSock2 ||
					lpMsg->mSock1 == lpMsg->mSock3 ||
					lpMsg->mSock1 == lpMsg->mSock4 ||
					lpMsg->mSock1 == lpMsg->mSock5)
				{	
					SockSame = true;
				}
			}
			if (lpMsg->mSock2 != 255 && lpMsg->mSock2 != 0)
			{
				if (lpMsg->mSock2 == lpMsg->mSock1 ||
					lpMsg->mSock2 == lpMsg->mSock3 ||
					lpMsg->mSock2 == lpMsg->mSock4 ||
					lpMsg->mSock2 == lpMsg->mSock5)
				{	
					SockSame = true;
				}
			}
			if (lpMsg->mSock3 != 255 && lpMsg->mSock3 != 0)
			{
				if (lpMsg->mSock3 == lpMsg->mSock1 ||
					lpMsg->mSock3 == lpMsg->mSock2 ||
					lpMsg->mSock3 == lpMsg->mSock4 ||
					lpMsg->mSock3 == lpMsg->mSock5)
				{	
					SockSame = true;
				}
			}
			if (lpMsg->mSock4 != 255 && lpMsg->mSock4 != 0)
			{
				if (lpMsg->mSock4 == lpMsg->mSock1 ||
					lpMsg->mSock4 == lpMsg->mSock2 ||
					lpMsg->mSock4 == lpMsg->mSock3 ||
					lpMsg->mSock4 == lpMsg->mSock5)
				{	
					SockSame = true;
				}
			}
			if (lpMsg->mSock5 != 255 && lpMsg->mSock5 != 0)
			{
				if (lpMsg->mSock5 == lpMsg->mSock1 ||
					lpMsg->mSock5 == lpMsg->mSock2 ||
					lpMsg->mSock5 == lpMsg->mSock3 ||
					lpMsg->mSock5 == lpMsg->mSock4)
				{	
					SockSame = true;
				}
			}
			
			if(SockSame == true)
			{
				GCServerMsgStringSend("[SCFVipShop] All socket options must be different!", lpObj->m_Index, 0x00);
				return;
			}
		}

		if(IsSlotItem(id) == 0 && SockOpCount > 0)
		{
			GCServerMsgStringSend("This item cant have socket attributes!", lpObj->m_Index, 0x01);
			return;
		}

		if((id >= ITEMGET(0,0) && id < ITEMGET(12,0))
			|| ( id == ITEMGET(13,30))
			|| (id>=ITEMGET(13,8) && id<=ITEMGET(13,13)) 
			|| (id >=ITEMGET(13,20) && id<=ITEMGET(13,28))
			|| (id >= ITEMGET(12,0) && id <=ITEMGET(12,6)) 
			|| (id >= ITEMGET(12,36) && id <=ITEMGET(12,43))
			|| (id >= ITEMGET(12,130) && id<=ITEMGET(12,135)) 
			|| (id >= ITEMGET(12,200) && id <=ITEMGET(12,254))
			|| (id == ITEMGET(13,107))
			)
		{
		}else
		{
			if(id > ITEMGET(12,0))
			{						
				if(this->MaxHarmony > 0 && lpMsg->mHarmony > 0)
				{
					GCServerMsgStringSend("This item cant have those attributes!", lpObj->m_Index, 0x01);
					return;
				}
			}
			if(lpMsg->mLevel > 0 || lpMsg->mOpt > 0 || lpMsg->mExc > 0 || lpMsg->mLuck > 0 || lpMsg->mSkill > 0)
			{
				GCServerMsgStringSend("This item cant have those attributes!", lpObj->m_Index, 0x01);
				return;
			}
		}
		int Val = 0;
		switch(lpMsg->Days)
		{
			case 0:
			{
				Val = this->item[id].Price_1Day;
			}break;
			case 1:
			{
				Val = this->item[id].Price_7Days;
			}break;
			case 2:
			{
				Val = this->item[id].Price_30Days;
			}break;
			case 3:
			{
				Val = this->item[id].Price_Forever;
			}break;
		}
		int Total = Val;//this->item[id].Price;

		if(Val == 0)
		{
			GCServerMsgStringSend("You cant buy items with that number of days!", lpObj->m_Index, 0x01);
			return;
		}

		Total += (getNumberOfExcOptions(lpMsg->mExc) * this->pExc);
		Total += (lpMsg->mLevel * this->pLevel);
		Total += (lpMsg->mLuck * this->pLuck);
		Total += (lpMsg->mOpt * this->pOpt);
		Total += (lpMsg->mSkill * this->pSkill);

		if(lpMsg->mHarmony > 0)
			Total += this->pHarmony;		

		int FreeSockCnt = 0;
		int SockCnt = 0;
		if(lpMsg->mSock1 == 255)
			FreeSockCnt++;
		else if(lpMsg->mSock1 != 0)
			SockCnt++;
		if(lpMsg->mSock2 == 255)
			FreeSockCnt++;
		else if(lpMsg->mSock2 != 0)
			SockCnt++;
		if(lpMsg->mSock3 == 255)
			FreeSockCnt++;
		else if(lpMsg->mSock3 != 0)
			SockCnt++;
		if(lpMsg->mSock4 == 255)
			FreeSockCnt++;
		else if(lpMsg->mSock4 != 0)
			SockCnt++;
		if(lpMsg->mSock5 == 255)
			FreeSockCnt++;
		else if(lpMsg->mSock5 != 0)
			SockCnt++;

		Total += this->pSocket * SockCnt;
		Total += this->pFreeSocket * FreeSockCnt;

		if(lpMsg->mSock1 > 0 && lpMsg->mSock1 != 255 && SockLevel1 > 1)
			Total += (SockLevel1 * this->pSocketLevel);
		if(lpMsg->mSock2 > 0 && lpMsg->mSock2 != 255 && SockLevel2 > 1)
			Total += (SockLevel2 * this->pSocketLevel);
		if(lpMsg->mSock3 > 0 && lpMsg->mSock3 != 255 && SockLevel3 > 1)
			Total += (SockLevel3 * this->pSocketLevel);
		if(lpMsg->mSock4 > 0 && lpMsg->mSock4 != 255 && SockLevel4 > 1)
			Total += (SockLevel4 * this->pSocketLevel);
		if(lpMsg->mSock5 > 0 && lpMsg->mSock5 != 255 && SockLevel5 > 1)
			Total += (SockLevel5 * this->pSocketLevel);

		if(lpObj->VipMoney >= Total)
		{	
			if(lpMsg->Days == 3)
			{
				PMSG_BUYSHOPRESULT pResult;	
				PHeadSetB((LPBYTE)&pResult, 0x32, sizeof(pResult));
				CItem item;
				item.m_Level = lpMsg->mLevel;
				item.m_SkillOption = lpMsg->mSkill;
				item.m_LuckOption = lpMsg->mLuck;
				item.m_Z28Option = lpMsg->mOpt;
				item.m_Durability = 100.0f;
				item.m_ItemOptionEx = lpMsg->mExc;
				item.m_JewelOfHarmonyOption = 0;
				if(lpMsg->mHarmony > 0)
				{
					//int iItemType = g_kJewelOfHarmonySystem._GetItemType(pTarget);
					//int iItemOption = lpMsg->mHarmony;
					//int iItemOptionLevel = this->m_itemOption[iItemType][iItemOption].iRequireLevel;
					int btOptionLevel = item.m_Level;//g_kJewelOfHarmonySystem._GetItemOptionRequireLevel(&item);
					if(btOptionLevel > ReadConfig.JOHLevelMax)
						btOptionLevel = ReadConfig.JOHLevelMax;
					item.m_JewelOfHarmonyOption |= lpMsg->mHarmony << 4;
					item.m_JewelOfHarmonyOption |= btOptionLevel & 0x0F;
				}
					
				item.Convert(id, lpMsg->mSkill, lpMsg->mLuck, lpMsg->mOpt, lpMsg->mExc, 0, 0, CURRENT_DB_VERSION);
				item.m_Durability = item.m_BaseDurability;
				item.m_ItemSlot1 = lpMsg->mSock1;
				item.m_ItemSlot2 = lpMsg->mSock2;
				item.m_ItemSlot3 = lpMsg->mSock3;
				item.m_ItemSlot4 = lpMsg->mSock4;
				item.m_ItemSlot5 = lpMsg->mSock5;

				item.Value();
				pResult.Result = gObjShopBuyInventoryInsertItem(lpObj->m_Index,item);
					
				if ( pResult.Result != 0xFF )
				{
					lpObj->AccountExtraInfoModified = 1;

					if(this->IsForeverFFFE == 1)
						lpObj->pInventory[pResult.Result].m_Number = 0xFFFFFFFE;

					ItemByteConvert((LPBYTE)&pResult.ItemInfo, item);
					DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);
					lpObj->VipMoney -= Total;
					
					GCServerMsgStringSend("Item Buy Success!", lpObj->m_Index, 0x01);

					wsprintf(sbuf,"[SCFVipShop](%s)(%s) Buy Item:%d(%d %d) Price:%d Total:%d [LVL:%d O:%d L:%d S:%d E:%d]",
						gObj[aIndex].AccountID,gObj[aIndex].Name,
						id, id/512, id-((int)(id/512)*512), Val, Total,
						lpMsg->mLevel,lpMsg->mOpt,lpMsg->mLuck,lpMsg->mSkill,lpMsg->mExc);
					LogAddTD(sbuf);

					VIPSHOP_LOG.Output(sbuf);
				}else
				{				
					GCServerMsgStringSend(lMsg.Get(MSGGET(14, 58)),lpObj->m_Index, 0x01);
				}
			}else
			{
				int Days = 1;
				switch(lpMsg->Days)
				{
					case 0:
					{
						Days = 1;
					}break;
					case 1:
					{
						Days = 7;
					}break;
					case 2:
					{
						Days = 30;
					}break;
				}
				time_t t = time(NULL);
				localtime(&t);
				DWORD iTime = (DWORD)t + Days * 86400;
				BYTE iItemPos = gObjInventoryInsertItem(&gObj[aIndex], ITEM_GET_TYPE(id), ITEM_GET_INDEX(id), lpMsg->mLevel, iTime, 0xff,0,0,0,0,0);
				
				if ( iItemPos != (BYTE)-1 )
				{
					gObj[aIndex].pInventory[iItemPos].m_JewelOfHarmonyOption = 0;
					if(lpMsg->mHarmony > 0)
					{
						int btOptionLevel = gObj[aIndex].pInventory[iItemPos].m_Level;
						if(btOptionLevel > ReadConfig.JOHLevelMax)
							btOptionLevel = ReadConfig.JOHLevelMax;
						gObj[aIndex].pInventory[iItemPos].m_JewelOfHarmonyOption |= lpMsg->mHarmony << 4;
						gObj[aIndex].pInventory[iItemPos].m_JewelOfHarmonyOption |= btOptionLevel & 0x0F;
					}
					gObj[aIndex].pInventory[iItemPos].Convert(id, lpMsg->mSkill, lpMsg->mLuck, lpMsg->mOpt, lpMsg->mExc, 0, 0, CURRENT_DB_VERSION);
					BYTE btItemType=0;
					btItemType |= (id & 0x1E00 ) >> 5;
					gObj[aIndex].pInventory[iItemPos].m_ItemOptionEx = btItemType+2;
					gObj[aIndex].pInventory[iItemPos].m_ItemSlot1 = lpMsg->mSock1;
					gObj[aIndex].pInventory[iItemPos].m_ItemSlot2 = lpMsg->mSock2;
					gObj[aIndex].pInventory[iItemPos].m_ItemSlot3 = lpMsg->mSock3;
					gObj[aIndex].pInventory[iItemPos].m_ItemSlot4 = lpMsg->mSock4;
					gObj[aIndex].pInventory[iItemPos].m_ItemSlot5 = lpMsg->mSock5;
					::GCInventoryItemOneSend(aIndex, iItemPos);
					MuItemShop.ItemSendTime(aIndex,iItemPos);
					
					lpObj->AccountExtraInfoModified = 1;
					lpObj->VipMoney -= Total;					
					GCServerMsgStringSend("Item Buy Success!", lpObj->m_Index, 0x01);
					wsprintf(sbuf,"[SCFVipShop](%s)(%s) Buy Item:%d(%d %d) Price:%d Total:%d [LVL:%d O:%d L:%d S:%d E:%d]",
						gObj[aIndex].AccountID,gObj[aIndex].Name,
						id, id/512, id-((int)(id/512)*512), Val, Total,
						lpMsg->mLevel,lpMsg->mOpt,lpMsg->mLuck,lpMsg->mSkill,lpMsg->mExc);
					LogAddTD(sbuf);
					VIPSHOP_LOG.Output(sbuf);
				}
			}
		}else
		{
			GCServerMsgStringSend("You need more VipMoney!", lpObj->m_Index, 0x01);
		}
	}else
	{
		GCServerMsgStringSend("Item Dissabled!", lpObj->m_Index, 0x01);
	}
	gObj[aIndex].VipShopOpen = 0;
}

void SCFVipShop::Read(char * FilePath)
{
	if(this->Enabled == 1)
	{
		int Token;

		SMDFile = fopen(FilePath, "r");

		if ( SMDFile == NULL )
		{
			return;
		}

		while ( true )
		{
			int iType = GetToken();
			
			if ( iType == 1 )
			{
				while(true)
				{
					Token = GetToken();
					if ( stricmp("end", TokenString) == 0 )
					{
						break;
					}

					int type = TokenNumber;
					
					Token = GetToken();
					int Index = TokenNumber;
					
					if(type >= MAX_TYPE_ITEMS)
					{
						MsgBox("ERROR: FILE %s MAX TYPE = %d", FilePath,MAX_TYPE_ITEMS);
						exit(1);
					}
					if(type >= MAX_SUBTYPE_ITEMS)
					{
						MsgBox("ERROR: FILE %s MAX INDEX = %d", FilePath,MAX_SUBTYPE_ITEMS);
						exit(1);
					}
					
					Token = GetToken();
					int Price_1Day = TokenNumber;
					Token = GetToken();
					int Price_7Days = TokenNumber;
					Token = GetToken();
					int Price_30Days = TokenNumber;
					Token = GetToken();
					int Price_Forever = TokenNumber;

					int ID = ITEMGET(type,Index);
					this->item[ID].Enabled = true;
					this->item[ID].Price_1Day = Price_1Day;
					this->item[ID].Price_7Days = Price_7Days;
					this->item[ID].Price_30Days = Price_30Days;
					this->item[ID].Price_Forever = Price_Forever;
				}
			}
			break;
		}			
		fclose(SMDFile);
	}
}



#endif
#endif