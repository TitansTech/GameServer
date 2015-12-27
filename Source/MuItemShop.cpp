#include "stdafx.h"
#include "GameServer.h"
#include "GameMain.h"
#include "LogProc.h"
#include "DSProtocol.h"
#include "MuItemShop.h"
#include "ExServerProtocol.h"
#include "SCFExDBProtocol.h"
#include "SCFExDB_Defines.h"
#include "ItemAddOption.h"
#include "SProtocol.h"
#include "..\include\Readscript.h"
#include "..\common\winutil.h"

#include "LogToFile.h"
extern CLogToFile WZ_ITEMSHOP_LOG;

cMuItemShop MuItemShop;

//extern void LogLine(char *formatstring, ...);
//extern void ToHex(unsigned char * out, unsigned char * data, int len);

double cMuItemShop::getPoints(double points)
{
	int POWER_OF_TWO = /*floor*/( log10(points)/log10(2.0f) );
	return ( ( 0x1000 / pow (2.0f,POWER_OF_TWO )) * points ) + ((POWER_OF_TWO-1) * 0x1000) + 0x3FF000;
}

void cMuItemShop::SetPoints(double Val, LPBYTE Bytes)
{
	DWORD WCoin_DH = SET_NUMBERHDW(Val);
	DWORD WCoin_DL = SET_NUMBERLDW(Val);

	WORD WCoin_WH = SET_NUMBERLW(WCoin_DL);
	WORD WCoin_WL = SET_NUMBERHW(WCoin_DL);

	WORD WCoin_WH1 = SET_NUMBERLW(WCoin_DH);
	WORD WCoin_WL1 = SET_NUMBERHW(WCoin_DH);

	if(HIBYTE(WCoin_WL) == 0)
	{
		Bytes[5]= LOBYTE(WCoin_WH);
		Bytes[6]= HIBYTE(WCoin_WH);
		Bytes[7]= LOBYTE(WCoin_WL);
	}else
	{
		Bytes[0]= LOBYTE(WCoin_WH1);
		Bytes[1]= HIBYTE(WCoin_WH1);
		Bytes[2]= LOBYTE(WCoin_WL1);
		Bytes[3]= HIBYTE(WCoin_WL1);

		Bytes[4]= LOBYTE(WCoin_WH);
		Bytes[5]= HIBYTE(WCoin_WH);
		Bytes[6]= LOBYTE(WCoin_WL);
		Bytes[7]= HIBYTE(WCoin_WL);
	}
}


void cMuItemShop::ProtocolCore(int aIndex,BYTE * aRecv)
{
	/*if ( aIndex < 0 || aIndex > OBJMAX-1 )
	{
		return;
	}
	if(gObj[aIndex].Type != OBJ_USER)
	{
		return;
	}*/

	//char _debuginfoNew0[1024]="\x00";
	//ToHex((unsigned char *)_debuginfoNew0, aRecv, (aRecv[1]));
	//LogLine("[Data RECV][CashShop] %s\n", _debuginfoNew0);

	if(this->Enabled == TRUE)
	{
		switch(aRecv[3])
		{
			case 1:
			{
				this->SendPoints(aIndex);
				//C1,04,D2,01 = Update Information
				//BYTE buff[0x2d] = { 0xc1,0x2d,0xd2,0x01,0x00,0x00,0x00,0x00,0x00,0x00,
				//	0x00,0x00,0x00,
				//	0x00,0x00,0x00,0x00,0x00,0x88,0xc3,0x40,
				//	0x00,0x00,0x00,0x00,0x00,0x70,0x60,0x40,
				//	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
				//	0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x40 };
				//DataSend(aIndex, buff, 0x2d);
			}break;
			case 2:
			{
				switch ( aRecv[4] )
				{
					case 0x00: //Open Cash Shop State				
					{
						if(gObj[aIndex].CShopInfoRequested == 2)
						{
							this->OpenRequest(aIndex);
						}else if(gObj[aIndex].CShopInfoRequested == 0)
						{
							gObj[aIndex].CShopInfoRequested = 1;
							GetAccountExServerInfo(aIndex,SCFExDB_GSSend_CShopMuItemShopGetList);
						}
					}break;
				}
			}break;
			case 3:
			{
				this->ItemBuyRecv(aIndex,(PMSG_CSHOPBUYRECV *)aRecv);
			}break;
			case 4:
			{
				this->ItemBuyGiftRecv(aIndex,(PMSG_CSHOPBUYGIFTRECV *)aRecv);
			}break;
			case 5: //Storage List & Gift List
			{
				this->ListReqRecv(aIndex,(PMSG_CSHOPLISTRECV *)aRecv);
			}break;

			case 11: //Use Item
			{
				this->ItemUse(aIndex,(PMSG_CSHOPUSEITEMRECV *)aRecv);
			}break;

	//		case 5:
	//		{
	//			BYTE buff1[0x06] = { 0xC1, 0x06, 0xD2, 0x06, 0x11, 0x00  };
	//			DataSend(lpObj->m_Index, buff1, 0x06);
	//			BYTE buff2[0x06] = { 0xC1, 0x06, 0xD2, 0x06, 0x0F, 0x00  };
	//			DataSend(lpObj->m_Index, buff2, 0x06);{
	//		}break;
		};
	}
}
void cMuItemShop::OpenRequest(int aIndex)
{
	PMSG_CSHOPOPENSEND pMsg;			
	PHeadSubSetB((LPBYTE)&pMsg, 0xD2, 0x02, sizeof(pMsg));
	pMsg.State = 1;

	DataSend(aIndex, (UCHAR*)&pMsg, sizeof(pMsg));
}

void cMuItemShop::SendPoints(int aIndex)
{
	if ( aIndex < 0 || aIndex > OBJMAX-1 )
	{
		return;
	}
	if(gObj[aIndex].Type != OBJ_USER)
	{
		return;
	}

	MUITEMSHOP_UPDATEINFO_SEND_2 pMsg2 = {0};
	PHeadSubSetB((LPBYTE)&pMsg2, 0xD2, 0x01, 0x2D);

	double WCoin = this->getPoints(gObj[aIndex].WCoin);
	double WCoinP = this->getPoints(gObj[aIndex].WCoinP);
	double GoblinCoin = this->getPoints(gObj[aIndex].GoblinCoin);

	this->SetPoints(WCoin,(LPBYTE)pMsg2.Coin);
	this->SetPoints(WCoinP,(LPBYTE)pMsg2.CoinP);
	this->SetPoints(GoblinCoin,(LPBYTE)pMsg2.GoblinCoin);

	DataSend(aIndex, (UCHAR*)&pMsg2, pMsg2.h.size);
}


int cMuItemShop::UseBuffGetTime(int aIndex, int Slot)
{
	if ( aIndex < 0 || aIndex > OBJMAX-1 )
	{
		return 0;
	}
	if(gObj[aIndex].Type != OBJ_USER)
	{
		return 0;
	}

	if(this->IsCShopItem(aIndex,Slot) == true)
	{
		time_t t = time(NULL);
		localtime(&t);
		return (gObj[aIndex].pInventory[Slot].m_Number - (DWORD)t);
	}
	return 0;
}


void cMuItemShop::SetBuff(int aIndex)
{
	if ( aIndex < 0 || aIndex > OBJMAX-1 )
	{
		return;
	}
	if(gObj[aIndex].Type != OBJ_USER)
	{
		return;
	}

	if(gObj[aIndex].m_wItem_MuShop_Seal_EffectType > 0)
	{
		time_t t = time(NULL);
		localtime(&t);
		DWORD Time = 0;

		if ((DWORD)gObj[aIndex].m_iItem_MuShop_Seal_EffectValidTime > (DWORD)t)
		{
			Time = ((DWORD)gObj[aIndex].m_iItem_MuShop_Seal_EffectValidTime - (DWORD)t)/60;
		}

		if(Time > 0)
		{
			WZ_ITEMSHOP_LOG.Output("[CashShop](%s)(%s) Retreiving Seal Buff:%d Time:%d,%d [%d]", 
				gObj[aIndex].AccountID, gObj[aIndex].Name, 
				gObj[aIndex].m_wItem_MuShop_Seal_EffectType,
				gObj[aIndex].m_iItem_MuShop_Seal_EffectValidTime, (DWORD)t, 
				Time);

			int BuffType = g_ItemAddOption.GetItemEffect(gObj[aIndex].m_wItem_MuShop_Seal_EffectType);
			if(BuffType != -1)
			{
				SENDBUFF pMsg={0};
				PHeadSubSetB((LPBYTE)&pMsg, 0x2D, 0x00, sizeof(pMsg));
				pMsg.BuffType = BuffType;
				pMsg.Time = (Time*60);
				DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg));
			}

			g_ItemAddOption.SetItemEffect(&gObj[aIndex], gObj[aIndex].m_wItem_MuShop_Seal_EffectType, Time);
		}else
		{
			WZ_ITEMSHOP_LOG.Output("[CashShop](%s)(%s) Expired Seal Buff:%d Time:%d,%d [%d]", 
				gObj[aIndex].AccountID, gObj[aIndex].Name, 
				gObj[aIndex].m_wItem_MuShop_Seal_EffectType,
				gObj[aIndex].m_iItem_MuShop_Seal_EffectValidTime, (DWORD)t, Time);

			gObj[aIndex].m_wItem_MuShop_Seal_EffectType = 0;
			gObj[aIndex].m_iItem_MuShop_Seal_EffectValidTime = 0;
			gObj[aIndex].PlayerExtraInfoModified = 1;
		}
	}
	if(gObj[aIndex].m_wItem_MuShop_Scroll_EffectType > 0)
	{
		time_t t = time(NULL);
		localtime(&t);
		DWORD Time = 0;

		if ((DWORD)gObj[aIndex].m_iItem_MuShop_Scroll_EffectValidTime > (DWORD)t)
		{
			Time = ((DWORD)gObj[aIndex].m_iItem_MuShop_Scroll_EffectValidTime - (DWORD)t)/60;
		}

		if(Time > 0)
		{
			WZ_ITEMSHOP_LOG.Output("[CashShop](%s)(%s) Retreiving Scroll Buff:%d Time:%d,%d [%d]", 
				gObj[aIndex].AccountID, gObj[aIndex].Name, 
				gObj[aIndex].m_wItem_MuShop_Scroll_EffectType,
				gObj[aIndex].m_iItem_MuShop_Scroll_EffectValidTime, (DWORD)t, Time);

			int BuffType = g_ItemAddOption.GetItemEffect(gObj[aIndex].m_wItem_MuShop_Scroll_EffectType);
			if(BuffType != -1)
			{
				SENDBUFF pMsg={0};
				PHeadSubSetB((LPBYTE)&pMsg, 0x2D, 0x00, sizeof(pMsg));
				pMsg.BuffType = BuffType;
				pMsg.Time = (Time*60);
				DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg));
			}

			g_ItemAddOption.SetItemEffect(&gObj[aIndex], gObj[aIndex].m_wItem_MuShop_Scroll_EffectType, Time);
		}else
		{
			WZ_ITEMSHOP_LOG.Output("[CashShop](%s)(%s) Expired Scroll Buff:%d Time:%d,%d [%d]", 
				gObj[aIndex].AccountID, gObj[aIndex].Name, 
				gObj[aIndex].m_wItem_MuShop_Scroll_EffectType,
				gObj[aIndex].m_iItem_MuShop_Scroll_EffectValidTime, (DWORD)t, Time);

			gObj[aIndex].m_wItem_MuShop_Scroll_EffectType = 0;
			gObj[aIndex].m_iItem_MuShop_Scroll_EffectValidTime = 0;
			gObj[aIndex].PlayerExtraInfoModified = 1;
		}
	}
}


bool cMuItemShop::DeleteItem(int aIndex)
{	
	if ( aIndex < 0 || aIndex > OBJMAX-1 )
	{
		return false;
	}
	if(gObj[aIndex].Type != OBJ_USER)
	{
		return false;
	}

	bool delWearItem = false;
	for(int Pos = 0; Pos < ReadConfig.INVENTORY_SIZE(aIndex,false);Pos++)
	{
		if(this->IsCShopItem(aIndex,Pos) == true)
		{
			time_t t = time(NULL);
			localtime(&t);
			if(gObj[aIndex].pInventory[Pos].m_Number <= (DWORD)t)
			{
				GCServerMsgStringSend("CashShop item has Vanished!", gObj[aIndex].m_Index, 1);
				WZ_ITEMSHOP_LOG.Output(lMsg.Get(MSGGET(2, 10)), gObj[aIndex].AccountID, gObj[aIndex].Name,
					gObj[aIndex].pInventory[Pos].GetName(), gObj[aIndex].pInventory[Pos].m_Level);

				gObjInventoryDeleteItem(gObj[aIndex].m_Index, Pos);
				GCInventoryItemDeleteSend(gObj[aIndex].m_Index, Pos, 0);

				if(Pos < 12)
				{
					gObjMakePreviewCharSet(gObj[aIndex].m_Index);
					GCEquipmentChange(gObj[aIndex].m_Index, Pos);
					delWearItem = true;
				}
			}
		}
	}
	return delWearItem;
}

bool cMuItemShop::IsCShopItem(int aIndex, BYTE Pos)
{	
	if ( aIndex < 0 || aIndex > OBJMAX-1 )
	{
		return false;
	}
	if(gObj[aIndex].Type != OBJ_USER)
	{
		return false;
	}

	if(gObj[aIndex].pInventory[Pos].IsItem())
	{
		BYTE btItemType=0;
		btItemType |= (gObj[aIndex].pInventory[Pos].m_Type & 0x1E00 ) >> 5;

		if((btItemType+2) == gObj[aIndex].pInventory[Pos].m_ItemOptionEx)
		{
			return true;
		}
	}
	return false;
}

bool cMuItemShop::IsCShopItemByType(WORD ItemType, BYTE m_ItemOptionEx)
{	
	BYTE btItemType=0;
	btItemType |= (ItemType & 0x1E00 ) >> 5;

	if((btItemType+2) == m_ItemOptionEx)
	{
		return true;
	}
	
	return false;
}

void cMuItemShop::ItemSendTime(int aIndex, BYTE Pos)
{
	if ( aIndex < 0 || aIndex > OBJMAX-1 )
	{
		return;
	}
	if(gObj[aIndex].Type != OBJ_USER)
	{
		return;
	}

	if(gObj[aIndex].pInventory[Pos].IsItem())
	{		
		BYTE btItemType=0;
		btItemType |= (gObj[aIndex].pInventory[Pos].m_Type & 0x1E00 ) >> 5;

		if((btItemType+2) == gObj[aIndex].pInventory[Pos].m_ItemOptionEx)
		{
			PMSG_CSHOPSENDITEMTIMESTAMP pMsg={0};
			PHeadSubSetB((LPBYTE)&pMsg, 0xD2, 0x12, sizeof(pMsg));
			pMsg.ItemID = gObj[aIndex].pInventory[Pos].m_Type;
			pMsg.Pos = Pos;
			pMsg.Time = gObj[aIndex].pInventory[Pos].m_Number;
			DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
		}
	}
}


void cMuItemShop::DeleteFromList(int aIndex, BYTE Pos, BYTE ListType)
{
	if ( aIndex < 0 || aIndex > OBJMAX-1 )
	{
		return;
	}
	if(gObj[aIndex].Type != OBJ_USER)
	{
		return;
	}

	if(ListType == 0x53)
	{
		gObj[aIndex].CShop[Pos].m_Serial1 = 0;
		gObj[aIndex].CShop[Pos].m_Serial2 = 0;
		gObj[aIndex].CShop[Pos].ItemID = 0;
		gObj[aIndex].CShop[Pos].ID1 = 0;
		gObj[aIndex].CShop[Pos].ID2 = 0;
		gObj[aIndex].CShopInfoModified_Normal = 1;
	}else if(ListType == 0x47)
	{
		gObj[aIndex].CShopGift[Pos].m_Serial1 = 0;
		gObj[aIndex].CShopGift[Pos].m_Serial2 = 0;
		gObj[aIndex].CShopGift[Pos].ItemID = 0;
		gObj[aIndex].CShopGift[Pos].ID1 = 0;
		gObj[aIndex].CShopGift[Pos].ID2 = 0;
		gObj[aIndex].CShopInfoModified_Gift = 1;
	}
}

bool cMuItemShop::HaveSpace(int aIndex, BYTE ListType)
{
	if ( aIndex < 0 || aIndex > OBJMAX-1 )
	{
		return false;
	}
	if(gObj[aIndex].Type != OBJ_USER)
	{
		return false;
	}

	for(int i=0;i<(MUITEMSHOP_MAXITEMPERPAGE*MUITEMSHOP_MAXPAGES);i++)
	{
		if(ListType == 0x53)
		{
			if(gObj[aIndex].CShop[i].m_Serial2 == 0)
			{
				return true;
			}
		}else if(ListType == 0x47)
		{
			if(gObj[aIndex].CShopGift[i].m_Serial2 == 0)
			{
				return true;
			}
		}
	}
	return false;
}

bool cMuItemShop::HaveSpace(int aIndex, BYTE ListType, int SpacesCount)
{
	if ( aIndex < 0 || aIndex > OBJMAX-1 )
	{
		return false;
	}
	if(gObj[aIndex].Type != OBJ_USER)
	{
		return false;
	}

	int count = 0;
	for(int i=0;i<(MUITEMSHOP_MAXITEMPERPAGE*MUITEMSHOP_MAXPAGES);i++)
	{
		if(ListType == 0x53)
		{
			if(gObj[aIndex].CShop[i].m_Serial2 == 0)
			{
				count++;
				if(count >= SpacesCount)
					return true;
			}
		}else if(ListType == 0x47)
		{
			if(gObj[aIndex].CShopGift[i].m_Serial2 == 0)
			{
				count++;
				if(count >= SpacesCount)
					return true;
			}
		}
	}
	return false;
}


void cMuItemShop::AddItem(int aIndex, DWORD m_Serial2, WORD ItemID, WORD ID1, WORD ID2, BYTE ListType)
{
	if ( aIndex < 0 || aIndex > OBJMAX-1 )
	{
		return;
	}
	if(gObj[aIndex].Type != OBJ_USER)
	{
		return;
	}

	if(ListType == 0x53)
	{
		for(int i=0;i<(MUITEMSHOP_MAXITEMPERPAGE*MUITEMSHOP_MAXPAGES);i++)
		{
			if(gObj[aIndex].CShop[i].m_Serial2 == 0)
			{
				gObj[aIndex].CShopInfoModified_Normal = 1;

				gObj[aIndex].CShop[i].m_Serial2 = m_Serial2;
				gObj[aIndex].CShop[i].ItemID = ItemID;
				gObj[aIndex].CShop[i].ID1 = ID1;
				gObj[aIndex].CShop[i].ID2 = ID2;
				
				this->ListAddItem(aIndex,ID1,ID2,m_Serial2);
				this->ListSend(aIndex,0x53,1);
				return;
			}
		}
	}
}

void cMuItemShop::AddGiftItem(int aIndex, DWORD m_Serial2, WORD ItemID, WORD ID1, WORD ID2, BYTE ListType, char * Name, char * Msg)
{
	if ( aIndex < 0 || aIndex > OBJMAX-1 )
	{
		return;
	}
	if(gObj[aIndex].Type != OBJ_USER)
	{
		return;
	}

	if(ListType == 0x47)
	{
		for(int i=0;i<(MUITEMSHOP_MAXITEMPERPAGE*MUITEMSHOP_MAXPAGES);i++)
		{
			if(gObj[aIndex].CShopGift[i].m_Serial2 == 0)
			{
				gObj[aIndex].CShopInfoModified_Gift = 1;

				gObj[aIndex].CShopGift[i].m_Serial2 = m_Serial2;
				gObj[aIndex].CShopGift[i].ItemID = ItemID;
				gObj[aIndex].CShopGift[i].ID1 = ID1;
				gObj[aIndex].CShopGift[i].ID2 = ID2;
				strcpy(gObj[aIndex].CShopGiftMsg[i].Name, Name);
				strcpy(gObj[aIndex].CShopGiftMsg[i].Msg, Msg);

				return;
			}
		}
	}
}

int cMuItemShop::ItemUse_SearchBySerial2(int aIndex, DWORD Serial2, BYTE & ListType)
{
	if ( aIndex < 0 || aIndex > OBJMAX-1 )
	{
		return -1;
	}
	if(gObj[aIndex].Type != OBJ_USER)
	{
		return -1;
	}

	for(int i=0;i<(MUITEMSHOP_MAXITEMPERPAGE*MUITEMSHOP_MAXPAGES);i++)
	{
		if(gObj[aIndex].CShop[i].m_Serial2 == Serial2)
		{
			ListType = 0x53;
			return i;
		}
		if(gObj[aIndex].CShopGift[i].m_Serial2 == Serial2)
		{
			ListType = 0x47;
			return i;
		}
	}
	return -1;
}

struct PMSG_USE_STAT_FRUIT {

  // static data ------------------------------------

  // non-static data --------------------------------
  /*<thisrel this+0x0>*/ /*|0x3|*/ struct PBMSG_HEAD h;
  /*<thisrel this+0x3>*/ /*|0x1|*/ unsigned char result;
  /*<thisrel this+0x4>*/ /*|0x1|*/ WORD btStatValue;
  /*<thisrel this+0x5>*/ /*|0x1|*/ unsigned char btFruitType;

  // base classes -----------------------------------

  // friends ----------------------------------------

  // static functions -------------------------------

  // non-virtual functions --------------------------

  // virtual functions ------------------------------
};

bool cMuItemShop::UseFruit(int aIndex, WORD ItemID)
{
	if ( aIndex < 0 || aIndex > OBJMAX-1 )
	{
		return false;
	}
	if(gObj[aIndex].Type != OBJ_USER)
	{
		return false;
	}

	BYTE bEnableUseFruit = 1;
	BYTE iItemLevel = 0;
	int iCharacterClass = gObj[aIndex].Class;

	if(ItemID == ITEMGET(13,57))
	{
		iItemLevel = 0;
		int iDefaultStat = DCInfo.DefClass[iCharacterClass].Energy;
		int iPresentStat = gObj[aIndex].Energy - 200;

		if(iPresentStat <= iDefaultStat)
		{
			bEnableUseFruit = 0;
		}else
		{
			gObj[aIndex].Energy -= 200; 
		}
	}
	else if(ItemID == ITEMGET(13,56))
	{
		iItemLevel = 1;
		int iDefaultStat = DCInfo.DefClass[iCharacterClass].Vitality;
		int iPresentStat = gObj[aIndex].Vitality - 200;

		if(ItemID <= iDefaultStat)
		{
			bEnableUseFruit = 0;
		}else
		{
			gObj[aIndex].Vitality -= 200; 
		}
	}
	else if(ItemID == ITEMGET(13,55))
	{
		iItemLevel = 2;
		int iDefaultStat = DCInfo.DefClass[iCharacterClass].Dexterity;
		int iPresentStat = gObj[aIndex].Dexterity - 200;

		if(iPresentStat <= iDefaultStat)
		{
			bEnableUseFruit = 0;
		}else
		{
			gObj[aIndex].Dexterity -= 200; 
		}
	}
	else if(ItemID == ITEMGET(13,54))
	{
		iItemLevel = 3;
		int iDefaultStat = DCInfo.DefClass[iCharacterClass].Strength;
		int iPresentStat = gObj[aIndex].Strength - 200;

		if(iPresentStat <= iDefaultStat)
		{
			bEnableUseFruit = 0;
		}else
		{
			gObj[aIndex].Strength -= 200;
		}
	}
	else if(ItemID == ITEMGET(13,58))
	{
		iItemLevel = 4;
		int iDefaultStat = DCInfo.DefClass[iCharacterClass].Leadership;
		int iPresentStat = gObj[aIndex].Leadership - 200;

		if(iPresentStat <= iDefaultStat)
		{
			bEnableUseFruit = 0;
		}else
		{
			gObj[aIndex].Leadership -= 200; 
		}
	}

	if(bEnableUseFruit == 1)
	{
		gObj[aIndex].LevelUpPoint += 200;
		PMSG_USE_STAT_FRUIT pResult;
		PHeadSetB((LPBYTE)&pResult,0x2C,sizeof(pResult));

		pResult.result = 3;
		pResult.btFruitType = iItemLevel;
		pResult.btStatValue = 200;

		DataSend(aIndex,(unsigned char *)&pResult,pResult.h.size);

		return true;
	}

	return false;
}

void cMuItemShop::ItemUse(int aIndex, PMSG_CSHOPUSEITEMRECV *lpMsg)
{
	if ( aIndex < 0 || aIndex > OBJMAX-1 )
	{
		return;
	}
	if(gObj[aIndex].Type != OBJ_USER)
	{
		return;
	}

	BYTE ListType = 0;
	int itemIndex = this->ItemUse_SearchBySerial2(aIndex,lpMsg->Serial2,ListType);
	if(itemIndex != -1)
	{	
		MUITEMSHOP tmpShop[MUITEMSHOP_MAXITEMPERPAGE*MUITEMSHOP_MAXPAGES];
		memset(tmpShop,0,sizeof(tmpShop));

		if(ListType == 0x53)
			memcpy(tmpShop,gObj[aIndex].CShop,sizeof(tmpShop));
		else if(ListType == 0x47)
			memcpy(tmpShop,gObj[aIndex].CShopGift,sizeof(tmpShop));

		int ProdIndex = this->GetProductID_Index(tmpShop[itemIndex].ID1,tmpShop[itemIndex].ID2,tmpShop[itemIndex].ItemID);
		if(ProdIndex != -1)
		{
			WZ_ITEMSHOP_LOG.Output("[CashShop](%s) Ask for Use Item [%d](%d,%d,%d)(%d,%d) Dur: %d", 
				gObj[aIndex].AccountID, ProdIndex, 
				tmpShop[itemIndex].ID1,tmpShop[itemIndex].ID2,
				tmpShop[itemIndex].ItemID,
				tmpShop[itemIndex].m_Serial1,
				tmpShop[itemIndex].m_Serial2,
				this->ProdID[ProdIndex].Dur);

			int iType  = ITEM_GET_TYPE(tmpShop[itemIndex].ItemID);
			int iTypeIndex = ITEM_GET_INDEX(tmpShop[itemIndex].ItemID);
			DWORD iTime = 0;
			BYTE iItemPos = (BYTE)-1;

			bool ShowMsg = false;
			
			if ( (tmpShop[itemIndex].ItemID >= ITEMGET(13,43) && tmpShop[itemIndex].ItemID <= ITEMGET(13,45)) ||			//Seals
				 (tmpShop[itemIndex].ItemID == ITEMGET(13,59)) ||
				 (tmpShop[itemIndex].ItemID >= ITEMGET(13,62) && tmpShop[itemIndex].ItemID <= ITEMGET(13,63)) ||
				 (tmpShop[itemIndex].ItemID >= ITEMGET(13,81) && tmpShop[itemIndex].ItemID <= ITEMGET(13,82)) ||
				 (tmpShop[itemIndex].ItemID >= ITEMGET(13,93) && tmpShop[itemIndex].ItemID <= ITEMGET(13,94)) ||
				 (tmpShop[itemIndex].ItemID == ITEMGET(13,96)) ||
				 (tmpShop[itemIndex].ItemID >= ITEMGET(13,103) && tmpShop[itemIndex].ItemID <= ITEMGET(13,105)) ||
				 (tmpShop[itemIndex].ItemID == ITEMGET(13,116))
				)
			{
				if ((tmpShop[itemIndex].ItemID >= ITEMGET(13,43) && tmpShop[itemIndex].ItemID <= ITEMGET(13,45)) ||
					tmpShop[itemIndex].ItemID == ITEMGET(13,62)
					)
				{
					if (gObjIsNewClass(&gObj[aIndex]) == 1)
					{
						GCServerMsgStringSend("Master cant use this item!",aIndex , 0x01);
						return;
					}
				}
				if ((tmpShop[itemIndex].ItemID >= ITEMGET(13,93) && tmpShop[itemIndex].ItemID <= ITEMGET(13,94)) || tmpShop[itemIndex].ItemID == ITEMGET(13,96))
				{
					if (gObjIsNewClass(&gObj[aIndex]) == 0)
					{
						GCServerMsgStringSend("This Item is for masters only!",aIndex , 0x01);
						return;
					}
				}

				ShowMsg = true;
				int Secs = this->ProdID[ProdIndex].Dur;

				time_t t = time(NULL);
				localtime(&t);

				if(Secs > 0)
				{
					iTime = (DWORD)t + this->ProdID[ProdIndex].Dur;

					int BuffType = g_ItemAddOption.GetItemEffect(tmpShop[itemIndex].ItemID);
					SENDBUFF pMsg={0};
					PHeadSubSetB((LPBYTE)&pMsg, 0x2D, 0x00, sizeof(pMsg));
					pMsg.BuffType = BuffType;
					pMsg.Time = (iTime);
					DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg));
				}
				g_ItemAddOption.SetItemEffect(&gObj[aIndex], tmpShop[itemIndex].ItemID, (Secs/60));

				gObj[aIndex].m_wItem_MuShop_Seal_EffectType = tmpShop[itemIndex].ItemID;
				gObj[aIndex].m_iItem_MuShop_Seal_EffectValidTime = iTime;
				gObj[aIndex].PlayerExtraInfoModified = 1;

				WZ_ITEMSHOP_LOG.Output("[CashShop](%s) Use Item [%d](%d,%d,%d)(%d,%d) Dur:%d ExpTime:%d CurTime:%d", 
					gObj[aIndex].AccountID, ProdIndex, 
					tmpShop[itemIndex].ID1,tmpShop[itemIndex].ID2,
					tmpShop[itemIndex].ItemID,
					tmpShop[itemIndex].m_Serial1,
					tmpShop[itemIndex].m_Serial2,
					Secs,
					iTime,
					(DWORD)t
				);

				this->DeleteFromList(aIndex,itemIndex,ListType);
				this->ListSend(aIndex,0x53,1);

			}else if((tmpShop[itemIndex].ItemID >= ITEMGET(14,72) && tmpShop[itemIndex].ItemID <= ITEMGET(14,77)) || //Scrolls					 
					 (tmpShop[itemIndex].ItemID >= ITEMGET(14,97) && tmpShop[itemIndex].ItemID <= ITEMGET(14,98))
					)
			{
				ShowMsg = true;
				int Secs = this->ProdID[ProdIndex].Dur;

				time_t t = time(NULL);
				localtime(&t);
				if(Secs > 0)
				{
					iTime = (DWORD)t + this->ProdID[ProdIndex].Dur;
								
					int BuffType = g_ItemAddOption.GetItemEffect(tmpShop[itemIndex].ItemID);
					SENDBUFF pMsg={0};
					PHeadSubSetB((LPBYTE)&pMsg, 0x2D, 0x00, sizeof(pMsg));
					pMsg.BuffType = BuffType;
					pMsg.Time = (iTime);
					DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg));
				}
				//BYTE buff3[0x14] = { 0xc1,0x14,0x2d,0x00,0x04,0x00,0x03,0x00,0x00,0x15,0x8c,0x0c,00,00,00,0x00,0x2d,0xed,0x12,0x00  };
				//DataSend(aIndex, buff3, 0x14);
				//BYTE buff2[0x14] = { 0xc1,0x14,0x2d,0x00,0x04,0x00,0x03,0x00,0x00,0x15,0x8c,0x0c,0x80,0x3a,0x09,0x00,0x2d,0xed,0x12,0x00  };
				//DataSend(aIndex, buff2, 0x14);

				g_ItemAddOption.SetItemEffect(&gObj[aIndex], tmpShop[itemIndex].ItemID, (Secs/60));

				gObj[aIndex].m_wItem_MuShop_Scroll_EffectType = tmpShop[itemIndex].ItemID;
				gObj[aIndex].m_iItem_MuShop_Scroll_EffectValidTime = iTime;
				gObj[aIndex].PlayerExtraInfoModified = 1;

				WZ_ITEMSHOP_LOG.Output("[CashShop](%s) Use Item [%d](%d,%d,%d)(%d,%d) Dur:%d ExpTime:%d CurTime:%d", 
					gObj[aIndex].AccountID, ProdIndex, 
					tmpShop[itemIndex].ID1,tmpShop[itemIndex].ID2,
					tmpShop[itemIndex].ItemID,
					tmpShop[itemIndex].m_Serial1,
					tmpShop[itemIndex].m_Serial2,
					Secs,
					iTime,
					(DWORD)t
				);

				this->DeleteFromList(aIndex,itemIndex,ListType);
				this->ListSend(aIndex,0x53,1);

			}else if((tmpShop[itemIndex].ItemID >= ITEMGET(14,78) && tmpShop[itemIndex].ItemID <= ITEMGET(14,82)) ||
					 (tmpShop[itemIndex].ItemID >= ITEMGET(14,46) && tmpShop[itemIndex].ItemID <= ITEMGET(14,50)) ||
					 (tmpShop[itemIndex].ItemID >= ITEMGET(14,85) && tmpShop[itemIndex].ItemID <= ITEMGET(14,88)) ||
					 (tmpShop[itemIndex].ItemID >= ITEMGET(13,128) && tmpShop[itemIndex].ItemID <= ITEMGET(13,134))
					)
			{
				ShowMsg = true;
				int Secs = this->ProdID[ProdIndex].Dur;
				g_ItemAddOption.SetItemEffect(&gObj[aIndex], tmpShop[itemIndex].ItemID, (Secs/60));

				this->DeleteFromList(aIndex,itemIndex,ListType);
				this->ListSend(aIndex,0x53,1);

			}else if(tmpShop[itemIndex].ItemID == ITEMGET(14,162))
			{
				if(gObj[aIndex].ExInventory + gObj[aIndex].ExInventoryTmp < 2)
				{
					ShowMsg = true;
					gObj[aIndex].ExInventoryTmp = gObj[aIndex].ExInventory + 1;

					if(gObj[aIndex].ExInventoryTmp == 1)
					{
						for ( BYTE ItemPos = MAIN_INVENTORY_NORMAL_SIZE /*76*/ ; ItemPos < (MAIN_INVENTORY_NORMAL_SIZE + (MAX_EXTENDED_INV*EXTENDED_INV_SIZE)) /* was 140 added extra inv space*/ ; ItemPos ++ )
						{
							if ( gObj[aIndex].pInventory[ItemPos].IsItem() == TRUE)
							{
								gObjInventoryDeleteItem ( aIndex , ItemPos );		
							}
						}
					}
					this->DeleteFromList(aIndex,itemIndex,ListType);
					this->ListSend(aIndex,0x53,1);

					BYTE buff[4] = {0xc1,0x04,0x2B,0x01};
					DataSend(aIndex, buff, sizeof(buff));
				}
			}else if(tmpShop[itemIndex].ItemID == ITEMGET(14,163))
			{
				if(gObj[aIndex].ExWarehouse + gObj[aIndex].ExWarehouseTmp == 0)
				{
					ShowMsg = true;
					gObj[aIndex].ExWarehouseTmp = 1;
					gObj[aIndex].AccountExtraInfoModified = 1;

					this->DeleteFromList(aIndex,itemIndex,ListType);
					this->ListSend(aIndex,0x53,1);

					BYTE buff[4] = {0xc1,0x04,0x2B,0x01};
					DataSend(aIndex, buff, sizeof(buff));
				}
			}else if (tmpShop[itemIndex].ItemID >= ITEMGET(13,54) && tmpShop[itemIndex].ItemID <= ITEMGET(13,58))
			{
				if(this->UseFruit(aIndex,tmpShop[itemIndex].ItemID) == true)
				{
					ShowMsg = true;
					this->DeleteFromList(aIndex,itemIndex,ListType);
					this->ListSend(aIndex,0x53,1);
				}
			}else if (tmpShop[itemIndex].ItemID == ITEMGET(13,124)) //Gold Channel
			{
				if(this->ProdID[ProdIndex].Dur > 0)
				{
					ShowMsg = true;
					time_t t = time(NULL);
					localtime(&t);
					DWORD goldTime = (DWORD)t + this->ProdID[ProdIndex].Dur;
					GDSendGoldChannel(aIndex,goldTime);
					this->DeleteFromList(aIndex,itemIndex,ListType);
				}
			}else
			{
				if(this->ProdID[ProdIndex].Dur > 0)
				{
					time_t t = time(NULL);
					localtime(&t);
					iTime = (DWORD)t + this->ProdID[ProdIndex].Dur;

					iItemPos = gObjInventoryInsertItem(&gObj[aIndex], iType, iTypeIndex, 0, iTime, 0xff,0,0,0,0,0);
				}else if(this->ProdID[ProdIndex].Quant > 0)
				{
					iItemPos = gObjInventoryInsertItem(&gObj[aIndex], iType, iTypeIndex, 0, 0, this->ProdID[ProdIndex].Quant,0,0,0,0,0);
				}else if(this->ProdID[ProdIndex].Rate > 0)
				{
					iItemPos = gObjInventoryInsertItem(&gObj[aIndex], iType, iTypeIndex, 0, 0, this->ProdID[ProdIndex].Rate,0,0,0,0,0);
				}

				if ( iItemPos != (BYTE)-1 )
				{
					WZ_ITEMSHOP_LOG.Output("[CashShop](%s) Use Item (%d,%d,%d)", gObj[aIndex].AccountID,tmpShop[itemIndex].ID1,tmpShop[itemIndex].ID2,tmpShop[itemIndex].ItemID);

					gObj[aIndex].pInventory[iItemPos].Convert(tmpShop[itemIndex].ItemID,0, 0, 0,0, 0,0, CURRENT_DB_VERSION);
					
					if(this->ProdID[ProdIndex].Dur > 0)
					{							
						BYTE btItemType=0;
						btItemType |= (tmpShop[itemIndex].ItemID & 0x1E00 ) >> 5;
						gObj[aIndex].pInventory[iItemPos].m_ItemOptionEx = btItemType+2;
					}
					
					::GCInventoryItemOneSend(aIndex, iItemPos);

					if(this->ProdID[ProdIndex].Dur > 0)
						this->ItemSendTime(aIndex,iItemPos);

					this->DeleteFromList(aIndex,itemIndex,ListType);
					this->ListSend(aIndex,0x53,1);

					ShowMsg = true;
				}
			}
			if(ShowMsg == true)
			{			
				BYTE buff1[0x09] = { 0xC1, 0x09, 0xD2, 11, 0x00, 0xff, 0xff, 0xff, 0xff  };
				DataSend(aIndex, buff1, 0x09);
			}
		}
	}
}

int cMuItemShop::GetProductID(WORD PackageID, WORD ID2, int ItemID)
{
	for(int i=0;i<this->PackageCount;i++)
	{
		if(this->PackID[i].PackID == PackageID)
		{
			for(int x=0;x<CSHOP_PACKAGE_ID2_COUNT;x++)
			{
				if(this->PackID[i].ID2[x] == ID2)
					if(this->PackID[i].ItemID == ItemID)
						return this->PackID[i].ProdID;
			}
			return -1;
		}
	}
	return -1;
}

int cMuItemShop::GetPack_Index(WORD PackageID, WORD ID2, int ItemID)
{
	for(int i=0;i<this->PackageCount;i++)
	{
		if(this->PackID[i].PackID == PackageID)
		{
			for(int x=0;x<CSHOP_PACKAGE_ID2_COUNT;x++)
			{
				if(this->PackID[i].ID2[x] == ID2)
					if(this->PackID[i].ItemID == ItemID)
						return i;
			}
			return -1;
		}
	}
	return -1;
}

int cMuItemShop::GetProductID_Index(WORD ProductID, WORD ID2, int ItemID)
{
	for(int i=0;i<this->ProductCount;i++)
	{
		if(this->ProdID[i].ID1 == ProductID)
		{
			if(this->ProdID[i].ItemID == ItemID)
				if(ID2 == 0 || this->ProdID[i].ID2 == ID2)
					return i;
		}
	}
	return -1;
}

int cMuItemShop::GetProductID_Index(int ItemID, int Dur, BYTE Rate, BYTE Quant)
{
	for(int i=0;i<this->ProductCount;i++)
	{
		if(this->ProdID[i].ItemID == ItemID)
		{
			if(this->ProdID[i].Dur == Dur && this->ProdID[i].Rate == Rate && this->ProdID[i].Quant == Quant)
				return i;
		}
	}
	return -1;
}

void cMuItemShop::ListAddGiftItem(int aIndex, WORD ID1, WORD ID2, DWORD Serial2, char * Name, char * Msg)
{
	if ( aIndex < 0 || aIndex > OBJMAX-1 )
	{
		return;
	}
	if(gObj[aIndex].Type != OBJ_USER)
	{
		return;
	}

	//55 = Item (Check IBSProduct)
	//90 = ProductID (Check IBSPackage |) (IBSProduct two after @sec)

	PMSG_CSHOPLISTGIFTITEMS_SEND pMsg = {0};
	PHeadSubSetB((LPBYTE)&pMsg, 0xD2, 0x0E, sizeof(pMsg));

	pMsg.Unk1 = 0xA1;
	pMsg.Unk2 = 0x02;
	pMsg.Unk16 = 0x50;

	pMsg.ID1 = ID1;
	pMsg.ID2 = ID2;
	pMsg.Serial2 = Serial2;
	strcpy(pMsg.Name,Name);
	strcpy(pMsg.Msg,Msg);

	DataSend(aIndex, (UCHAR*)&pMsg, sizeof(pMsg));
}

void cMuItemShop::ListAddItem(int aIndex, WORD ID1, WORD ID2, DWORD Serial2)
{
	if ( aIndex < 0 || aIndex > OBJMAX-1 )
	{
		return;
	}
	if(gObj[aIndex].Type != OBJ_USER)
	{
		return;
	}

	//55 = Item (Check IBSProduct)
	//90 = ProductID (Check IBSPackage |) (IBSProduct two after @sec)

	PMSG_CSHOPLISTITEMS_SEND pMsg = {0};
	PHeadSubSetB((LPBYTE)&pMsg, 0xD2, 0x0D, sizeof(pMsg));

	pMsg.Unk1 = 0xA1;
	pMsg.Unk2 = 0x02;
	pMsg.Unk16 = 0x50;

	pMsg.ID1 = ID1;
	pMsg.ID2 = ID2;
	pMsg.Serial2 = Serial2;

	DataSend(aIndex, (UCHAR*)&pMsg, sizeof(pMsg));
}

char * cMuItemShop::GetCategoryID(BYTE Category)
{
	for(int i=0;i<this->CategoryCount;i++)
	{
		if(this->CatID[i].ID == Category)
			return this->CatID[i].Name;
	}
	return "UNK";
}

void cMuItemShop::ItemBuyRecv(int aIndex, PMSG_CSHOPBUYRECV *lpMsg)
{
	if ( aIndex < 0 || aIndex > OBJMAX-1 )
	{
		return;
	}
	if(gObj[aIndex].Type != OBJ_USER)
	{
		return;
	}

	//ID = PackageID
	//OptionsID = ID2

	int ProdID = this->GetProductID(lpMsg->ID,lpMsg->OptionsID,lpMsg->ItemID);
	if(ProdID != -1)
	{
		if(lpMsg->ItemID >= ITEMGET(14,134) && lpMsg->ItemID <= ITEMGET(14,139) || (lpMsg->ItemID == ITEMGET(13,124) && (lpMsg->ID >= 144 && lpMsg->ID <= 146))) //Packages
		{
			int packID = this->GetPack_Index(lpMsg->ID,lpMsg->OptionsID,lpMsg->ItemID);
			if(packID != -1)
			{
				switch(lpMsg->ItemID)
				{
					case ITEMGET(13,124):
					{
						if(lpMsg->ID == 145 || lpMsg->ID == 146)
						{
							if(this->HaveSpace(aIndex,0x53,4) == true)
							{
								int ProdIndex1 = this->GetProductID_Index(ITEMGET(13,94),604800,0,0);
								int ProdIndex2 = this->GetProductID_Index(ITEMGET(13,124),604800,0,0);
								int ProdIndex3 = this->GetProductID_Index(ITEMGET(14,92),0,0,1);
								int ProdIndex4 = this->GetProductID_Index(ITEMGET(14,73),86400,0,0);
								if( ProdIndex1 != -1 && 
									ProdIndex2 != -1 && 
									ProdIndex3 != -1 && 
									ProdIndex4 != -1)
								{
									if(this->RemovePointsbyPackage(aIndex,lpMsg->Type,packID) == true)
									{
										BYTE buff1[0x09] = { 0xC1, 0x09, 0xD2, 0x03, 0x00, 0xff, 0xff, 0xff, 0xff  };
										DataSend(aIndex, buff1, 0x09);
										WZ_ITEMSHOP_LOG.Output("[CashShop](%s) Buy Item:[%d,%d,%d][%s] %s", gObj[aIndex].AccountID,lpMsg->ID,lpMsg->OptionsID,lpMsg->ItemID,this->GetCategoryID(lpMsg->Category),"PACKAGE");
										gObj[aIndex].CShopInfoModified_Points = 1;
										MuItemShopBuySend(aIndex,ProdIndex1,lpMsg->Type);
										MuItemShopBuySend(aIndex,ProdIndex2,lpMsg->Type);
										MuItemShopBuySend(aIndex,ProdIndex3,lpMsg->Type);
										MuItemShopBuySend(aIndex,ProdIndex4,lpMsg->Type);
										this->SendPoints(aIndex);
									}
								}
							}
						}else if(lpMsg->ID == 143 || lpMsg->ID == 144)
						{
							if(this->HaveSpace(aIndex,0x53,4) == true)
							{
								int ProdIndex1 = this->GetProductID_Index(ITEMGET(13,44),604800,0,0);
								int ProdIndex2 = this->GetProductID_Index(ITEMGET(13,124),604800,0,0);
								int ProdIndex3 = this->GetProductID_Index(ITEMGET(14,92),0,0,1);
								int ProdIndex4 = this->GetProductID_Index(ITEMGET(14,73),86400,0,0);
								if( ProdIndex1 != -1 && 
									ProdIndex2 != -1 && 
									ProdIndex3 != -1 && 
									ProdIndex4 != -1)
								{
									if(this->RemovePointsbyPackage(aIndex,lpMsg->Type,packID) == true)
									{
										BYTE buff1[0x09] = { 0xC1, 0x09, 0xD2, 0x03, 0x00, 0xff, 0xff, 0xff, 0xff  };
										DataSend(aIndex, buff1, 0x09);
										WZ_ITEMSHOP_LOG.Output("[CashShop](%s) Buy Item:[%d,%d,%d][%s] %s", gObj[aIndex].AccountID,lpMsg->ID,lpMsg->OptionsID,lpMsg->ItemID,this->GetCategoryID(lpMsg->Category),"PACKAGE");
										gObj[aIndex].CShopInfoModified_Points = 1;
										MuItemShopBuySend(aIndex,ProdIndex1,lpMsg->Type);
										MuItemShopBuySend(aIndex,ProdIndex2,lpMsg->Type);
										MuItemShopBuySend(aIndex,ProdIndex3,lpMsg->Type);
										MuItemShopBuySend(aIndex,ProdIndex4,lpMsg->Type);
										this->SendPoints(aIndex);
									}
								}
							}
						}
					}break;
					case ITEMGET(14,134):
					{
						if(lpMsg->ID == 162 || lpMsg->ID == 163)
						{
							if(this->HaveSpace(aIndex,0x53,7) == true)
							{
								int ProdIndex1 = this->GetProductID_Index(ITEMGET(13,107),86400,0,0);
								int ProdIndex2 = this->GetProductID_Index(ITEMGET(13,64),86400,0,0);
								int ProdIndex3 = this->GetProductID_Index(ITEMGET(14,72),86400,0,0);
								int ProdIndex4 = this->GetProductID_Index(ITEMGET(14,98),86400,0,0);
								int ProdIndex5 = this->GetProductID_Index(ITEMGET(13,105),86400,0,0);
								int ProdIndex6 = this->GetProductID_Index(ITEMGET(14,133),0,0,10);
								int ProdIndex7 = this->GetProductID_Index(ITEMGET(13,113),86400,0,0);
								
								if( ProdIndex1 != -1 && 
									ProdIndex2 != -1 && 
									ProdIndex3 != -1 && 
									ProdIndex4 != -1 && 
									ProdIndex5 != -1 && 
									ProdIndex6 != -1 && 
									ProdIndex7 != -1)
								{
									if(this->RemovePointsbyPackage(aIndex,lpMsg->Type,packID) == true)
									{
										BYTE buff1[0x09] = { 0xC1, 0x09, 0xD2, 0x03, 0x00, 0xff, 0xff, 0xff, 0xff  };
										DataSend(aIndex, buff1, 0x09);
										WZ_ITEMSHOP_LOG.Output("[CashShop](%s) Buy Item:[%d,%d,%d][%s] %s", gObj[aIndex].AccountID,lpMsg->ID,lpMsg->OptionsID,lpMsg->ItemID,this->GetCategoryID(lpMsg->Category),"PACKAGE");
										gObj[aIndex].CShopInfoModified_Points = 1;
										MuItemShopBuySend(aIndex,ProdIndex1,lpMsg->Type);
										MuItemShopBuySend(aIndex,ProdIndex2,lpMsg->Type);
										MuItemShopBuySend(aIndex,ProdIndex3,lpMsg->Type);
										MuItemShopBuySend(aIndex,ProdIndex4,lpMsg->Type);
										MuItemShopBuySend(aIndex,ProdIndex5,lpMsg->Type);
										MuItemShopBuySend(aIndex,ProdIndex6,lpMsg->Type);
										MuItemShopBuySend(aIndex,ProdIndex7,lpMsg->Type);
										this->SendPoints(aIndex);
									}
								}
							}
						}else if(lpMsg->ID == 265)
						{
							if(this->HaveSpace(aIndex,0x53,1) == true)
							{
								int ProdIndex1 = -1;
								int num = rand()%2;
								if(num == 0)
								{
									ProdIndex1 = this->GetProductID_Index(ITEMGET(14,61),0,0,1);
								}else
								{
									ProdIndex1 = this->GetProductID_Index(ITEMGET(14,146),0,0,1);
								}
								if( ProdIndex1 != -1)
								{
									if(this->RemovePointsbyPackage(aIndex,lpMsg->Type,packID) == true)
									{
										BYTE buff1[0x09] = { 0xC1, 0x09, 0xD2, 0x03, 0x00, 0xff, 0xff, 0xff, 0xff  };
										DataSend(aIndex, buff1, 0x09);
										WZ_ITEMSHOP_LOG.Output("[CashShop](%s) Buy Item:[%d,%d,%d][%s] %s", gObj[aIndex].AccountID,lpMsg->ID,lpMsg->OptionsID,lpMsg->ItemID,this->GetCategoryID(lpMsg->Category),"PACKAGE");
										gObj[aIndex].CShopInfoModified_Points = 1;
										MuItemShopBuySend(aIndex,ProdIndex1,lpMsg->Type);
										this->SendPoints(aIndex);
									}
								}
							}
						}
					}break;
					
					case ITEMGET(14,135):
					{
						if(this->HaveSpace(aIndex,0x53,6) == true)
						{
							int ProdIndex3 = -1;
							int ProdIndex1 = this->GetProductID_Index(ITEMGET(14,77),86400,0,0);
							int ProdIndex2 = this->GetProductID_Index(ITEMGET(14,76),86400,0,0);
							if(lpMsg->ID == 201 || lpMsg->ID == 200)
								ProdIndex3 = this->GetProductID_Index(ITEMGET(14,74),86400,0,0);
							else if(lpMsg->ID == 203 || lpMsg->ID == 202)
								ProdIndex3 = this->GetProductID_Index(ITEMGET(14,75),86400,0,0);
							int ProdIndex4 = this->GetProductID_Index(ITEMGET(14,73),86400,0,0);
							int ProdIndex5 = this->GetProductID_Index(ITEMGET(14,72),86400,0,0);
							int ProdIndex6 = this->GetProductID_Index(ITEMGET(14,97),86400,0,0);
							
							if( ProdIndex1 != -1 && 
								ProdIndex2 != -1 && 
								ProdIndex3 != -1 && 
								ProdIndex4 != -1 && 
								ProdIndex5 != -1 && 
								ProdIndex6 != -1)
							{
								if(this->RemovePointsbyPackage(aIndex,lpMsg->Type,packID) == true)
								{
									BYTE buff1[0x09] = { 0xC1, 0x09, 0xD2, 0x03, 0x00, 0xff, 0xff, 0xff, 0xff  };
									DataSend(aIndex, buff1, 0x09);
									WZ_ITEMSHOP_LOG.Output("[CashShop](%s) Buy Item:[%d,%d,%d][%s] %s", gObj[aIndex].AccountID,lpMsg->ID,lpMsg->OptionsID,lpMsg->ItemID,this->GetCategoryID(lpMsg->Category),"PACKAGE");
									gObj[aIndex].CShopInfoModified_Points = 1;
									MuItemShopBuySend(aIndex,ProdIndex1,lpMsg->Type);
									MuItemShopBuySend(aIndex,ProdIndex2,lpMsg->Type);
									MuItemShopBuySend(aIndex,ProdIndex3,lpMsg->Type);
									MuItemShopBuySend(aIndex,ProdIndex4,lpMsg->Type);
									MuItemShopBuySend(aIndex,ProdIndex5,lpMsg->Type);
									MuItemShopBuySend(aIndex,ProdIndex6,lpMsg->Type);
									this->SendPoints(aIndex);
								}
							}
						}
					}break;
					
					case ITEMGET(14,138):
					{
						if(this->HaveSpace(aIndex,0x53,6) == true)
						{
							int ProdIndex1 = this->GetProductID_Index(ITEMGET(14,112),0,0,1);
							int ProdIndex2 = this->GetProductID_Index(ITEMGET(14,113),0,0,1);
							int	ProdIndex3 = this->GetProductID_Index(ITEMGET(14,95),0,0,1);
							int ProdIndex4 = this->GetProductID_Index(ITEMGET(14,92),0,0,1);
							int ProdIndex5 = this->GetProductID_Index(ITEMGET(14,53),0,1,0);
							int ProdIndex6 = this->GetProductID_Index(ITEMGET(14,112),0,0,1);
							
							if( ProdIndex1 != -1 && 
								ProdIndex2 != -1 && 
								ProdIndex3 != -1 && 
								ProdIndex4 != -1 && 
								ProdIndex5 != -1 && 
								ProdIndex6 != -1)
							{
								if(this->RemovePointsbyPackage(aIndex,lpMsg->Type,packID) == true)
								{
									BYTE buff1[0x09] = { 0xC1, 0x09, 0xD2, 0x03, 0x00, 0xff, 0xff, 0xff, 0xff  };
									DataSend(aIndex, buff1, 0x09);
									WZ_ITEMSHOP_LOG.Output("[CashShop](%s) Buy Item:[%d,%d,%d][%s] %s", gObj[aIndex].AccountID,lpMsg->ID,lpMsg->OptionsID,lpMsg->ItemID,this->GetCategoryID(lpMsg->Category),"PACKAGE");
									gObj[aIndex].CShopInfoModified_Points = 1;
									MuItemShopBuySend(aIndex,ProdIndex1,lpMsg->Type);
									MuItemShopBuySend(aIndex,ProdIndex2,lpMsg->Type);
									MuItemShopBuySend(aIndex,ProdIndex3,lpMsg->Type);
									MuItemShopBuySend(aIndex,ProdIndex4,lpMsg->Type);
									MuItemShopBuySend(aIndex,ProdIndex5,lpMsg->Type);
									MuItemShopBuySend(aIndex,ProdIndex6,lpMsg->Type);
									this->SendPoints(aIndex);
								}
							}
						}
					}break;
					
					case ITEMGET(14,139):
					{
						if(this->HaveSpace(aIndex,0x53,3) == true)
						{
							int ProdIndex3 = -1;
							int ProdIndex1 = this->GetProductID_Index(ITEMGET(14,73),86400,0,0);
							int ProdIndex2 = this->GetProductID_Index(ITEMGET(14,72),86400,0,0);
							if(lpMsg->ID == 197 || lpMsg->ID == 196)
								ProdIndex3 = this->GetProductID_Index(ITEMGET(14,74),86400,0,0);
							else if(lpMsg->ID == 199 || lpMsg->ID == 198)
								ProdIndex3 = this->GetProductID_Index(ITEMGET(14,75),86400,0,0);
							
							if( ProdIndex1 != -1 && 
								ProdIndex2 != -1 && 
								ProdIndex3 != -1)
							{
								if(this->RemovePointsbyPackage(aIndex,lpMsg->Type,packID) == true)
								{
									BYTE buff1[0x09] = { 0xC1, 0x09, 0xD2, 0x03, 0x00, 0xff, 0xff, 0xff, 0xff  };
									DataSend(aIndex, buff1, 0x09);
									WZ_ITEMSHOP_LOG.Output("[CashShop](%s) Buy Item:[%d,%d,%d][%s] %s", gObj[aIndex].AccountID,lpMsg->ID,lpMsg->OptionsID,lpMsg->ItemID,this->GetCategoryID(lpMsg->Category),"PACKAGE");
									gObj[aIndex].CShopInfoModified_Points = 1;
									MuItemShopBuySend(aIndex,ProdIndex1,lpMsg->Type);
									MuItemShopBuySend(aIndex,ProdIndex2,lpMsg->Type);
									MuItemShopBuySend(aIndex,ProdIndex3,lpMsg->Type);
									this->SendPoints(aIndex);
								}
							}
						}
					}break;
				}
			}
		}else
		{
			if(this->HaveSpace(aIndex,0x53))
			{
				int ProdIndex = this->GetProductID_Index(ProdID,lpMsg->OptionsID,lpMsg->ItemID);
				if(ProdIndex != -1)
				{
					WZ_ITEMSHOP_LOG.Output("[CashShop](%s) Try to Buy:[%d,%d,%d][%s] %s", gObj[aIndex].AccountID,lpMsg->ID,lpMsg->OptionsID,lpMsg->ItemID,this->GetCategoryID(lpMsg->Category),this->ProdID[ProdIndex].Name);
					if(this->RemovePoints(aIndex,lpMsg->Type,ProdIndex) == true)
					{
						BYTE buff1[0x09] = { 0xC1, 0x09, 0xD2, 0x03, 0x00, 0xff, 0xff, 0xff, 0xff  };
						DataSend(aIndex, buff1, 0x09);
						WZ_ITEMSHOP_LOG.Output("[CashShop](%s) Buy Item:[%d,%d,%d][%s] %s", gObj[aIndex].AccountID,lpMsg->ID,lpMsg->OptionsID,lpMsg->ItemID,this->GetCategoryID(lpMsg->Category),this->ProdID[ProdIndex].Name);
						gObj[aIndex].CShopInfoModified_Points = 1;
						MuItemShopBuySend(aIndex,ProdIndex,lpMsg->Type);
						this->SendPoints(aIndex);
					}else
					{
						WZ_ITEMSHOP_LOG.Output("[CashShop](%s) Not enough Money:[%d,%d,%d][%s]%s", gObj[aIndex].AccountID,lpMsg->ID,lpMsg->OptionsID,lpMsg->ItemID,this->GetCategoryID(lpMsg->Category),this->ProdID[ProdIndex].Name);
					}
				}
			}
		}
	}
}

void cMuItemShop::ItemBuyGiftRecv(int aIndex, PMSG_CSHOPBUYGIFTRECV *lpMsg)
{
	if ( aIndex < 0 || aIndex > OBJMAX-1 )
	{
		return;
	}
	if(gObj[aIndex].Type != OBJ_USER)
	{
		return;
	}

	//252 = (C) // 253 = (P) // 0 =  Goblin
	int tIndex = gObjGetIndex(lpMsg->Name);
	if(tIndex != -1)
	{
		if(gObj[tIndex].CShopInfoRequested != 2)
		{
			char tmp[512]={0};
			wsprintf(tmp,"%s Must Open CShop First!",lpMsg->Name);
			GCServerMsgStringSend(tmp,gObj[aIndex].m_Index, 0x01);
			wsprintf(tmp,"Open Your CShop, %s have a gift for you!",gObj[aIndex].Name);
			GCServerMsgStringSend(tmp,gObj[tIndex].m_Index, 0x00);
			return;
		}
		//Check if can add shits
		int ProdID = this->GetProductID(lpMsg->ID,lpMsg->OptionsID,lpMsg->ItemID);
		if(ProdID != -1)
		{
			if(lpMsg->ItemID >= ITEMGET(14,134) && lpMsg->ItemID <= ITEMGET(14,139) || (lpMsg->ItemID == ITEMGET(13,124) && (lpMsg->ID >= 144 && lpMsg->ID <= 146))) //Packages
			{
				int packID = this->GetPack_Index(lpMsg->ID,lpMsg->OptionsID,lpMsg->ItemID);
				if(packID != -1)
				{
					switch(lpMsg->ItemID)
					{
						case ITEMGET(13,124):
						{
							if(lpMsg->ID == 145 || lpMsg->ID == 146)
							{
								if(this->HaveSpace(tIndex,0x47,4) == true)
								{
									int ProdIndex1 = this->GetProductID_Index(ITEMGET(13,94),604800,0,0);
									int ProdIndex2 = this->GetProductID_Index(ITEMGET(13,124),604800,0,0);
									int ProdIndex3 = this->GetProductID_Index(ITEMGET(14,92),0,0,1);
									int ProdIndex4 = this->GetProductID_Index(ITEMGET(14,73),86400,0,0);
									if( ProdIndex1 != -1 && 
										ProdIndex2 != -1 && 
										ProdIndex3 != -1 && 
										ProdIndex4 != -1)
									{
										if(this->RemovePointsbyPackage(aIndex,lpMsg->Type,packID) == true)
										{
											BYTE buff1[0x09] = { 0xC1, 0x09, 0xD2, 0x03, 0x00, 0xff, 0xff, 0xff, 0xff  };
											DataSend(aIndex, buff1, 0x09);
											WZ_ITEMSHOP_LOG.Output("[CashShop](%s) Gift Item(%s):[%d,%d,%d][%s]%s", gObj[aIndex].AccountID,lpMsg->Name,lpMsg->ID,lpMsg->OptionsID,lpMsg->ItemID,this->GetCategoryID(lpMsg->Category),"PACKAGE");
											gObj[aIndex].CShopInfoModified_Points = 1;
											MuItemShopGiftBuySend(tIndex,ProdIndex1,lpMsg->Type, gObj[aIndex].Name, lpMsg->Msg);
											MuItemShopGiftBuySend(tIndex,ProdIndex2,lpMsg->Type, gObj[aIndex].Name, lpMsg->Msg);
											MuItemShopGiftBuySend(tIndex,ProdIndex3,lpMsg->Type, gObj[aIndex].Name, lpMsg->Msg);
											MuItemShopGiftBuySend(tIndex,ProdIndex4,lpMsg->Type, gObj[aIndex].Name, lpMsg->Msg);
											this->SendPoints(aIndex);
										}
									}
								}
							}else if(lpMsg->ID == 143 || lpMsg->ID == 144)
							{
								if(this->HaveSpace(tIndex,0x47,4) == true)
								{
									int ProdIndex1 = this->GetProductID_Index(ITEMGET(13,44),604800,0,0);
									int ProdIndex2 = this->GetProductID_Index(ITEMGET(13,124),604800,0,0);
									int ProdIndex3 = this->GetProductID_Index(ITEMGET(14,92),0,0,1);
									int ProdIndex4 = this->GetProductID_Index(ITEMGET(14,73),86400,0,0);
									if( ProdIndex1 != -1 && 
										ProdIndex2 != -1 && 
										ProdIndex3 != -1 && 
										ProdIndex4 != -1)
									{
										if(this->RemovePointsbyPackage(aIndex,lpMsg->Type,packID) == true)
										{
											BYTE buff1[0x09] = { 0xC1, 0x09, 0xD2, 0x03, 0x00, 0xff, 0xff, 0xff, 0xff  };
											DataSend(aIndex, buff1, 0x09);
											WZ_ITEMSHOP_LOG.Output("[CashShop](%s) Gift Item(%s):[%d,%d,%d][%s]%s", gObj[aIndex].AccountID,lpMsg->Name,lpMsg->ID,lpMsg->OptionsID,lpMsg->ItemID,this->GetCategoryID(lpMsg->Category),"PACKAGE");
											gObj[aIndex].CShopInfoModified_Points = 1;
											MuItemShopGiftBuySend(tIndex,ProdIndex1,lpMsg->Type, gObj[aIndex].Name, lpMsg->Msg);
											MuItemShopGiftBuySend(tIndex,ProdIndex2,lpMsg->Type, gObj[aIndex].Name, lpMsg->Msg);
											MuItemShopGiftBuySend(tIndex,ProdIndex3,lpMsg->Type, gObj[aIndex].Name, lpMsg->Msg);
											MuItemShopGiftBuySend(tIndex,ProdIndex4,lpMsg->Type, gObj[aIndex].Name, lpMsg->Msg);
											this->SendPoints(aIndex);
										}
									}
								}
							}
						}break;
						case ITEMGET(14,134):
						{
							if(lpMsg->ID == 162 || lpMsg->ID == 163)
							{
								if(this->HaveSpace(tIndex,0x47,7) == true)
								{
									int ProdIndex1 = this->GetProductID_Index(ITEMGET(13,107),86400,0,0);
									int ProdIndex2 = this->GetProductID_Index(ITEMGET(13,64),86400,0,0);
									int ProdIndex3 = this->GetProductID_Index(ITEMGET(14,72),86400,0,0);
									int ProdIndex4 = this->GetProductID_Index(ITEMGET(14,98),86400,0,0);
									int ProdIndex5 = this->GetProductID_Index(ITEMGET(13,105),86400,0,0);
									int ProdIndex6 = this->GetProductID_Index(ITEMGET(14,133),0,0,10);
									int ProdIndex7 = this->GetProductID_Index(ITEMGET(13,113),86400,0,0);
									
									if( ProdIndex1 != -1 && 
										ProdIndex2 != -1 && 
										ProdIndex3 != -1 && 
										ProdIndex4 != -1 && 
										ProdIndex5 != -1 && 
										ProdIndex6 != -1 && 
										ProdIndex7 != -1)
									{
										if(this->RemovePointsbyPackage(aIndex,lpMsg->Type,packID) == true)
										{
											BYTE buff1[0x09] = { 0xC1, 0x09, 0xD2, 0x03, 0x00, 0xff, 0xff, 0xff, 0xff  };
											DataSend(aIndex, buff1, 0x09);
											WZ_ITEMSHOP_LOG.Output("[CashShop](%s) Gift Item(%s):[%d,%d,%d][%s]%s", gObj[aIndex].AccountID,lpMsg->Name,lpMsg->ID,lpMsg->OptionsID,lpMsg->ItemID,this->GetCategoryID(lpMsg->Category),"PACKAGE");
											gObj[aIndex].CShopInfoModified_Points = 1;
											MuItemShopGiftBuySend(tIndex,ProdIndex1,lpMsg->Type, gObj[aIndex].Name, lpMsg->Msg);
											MuItemShopGiftBuySend(tIndex,ProdIndex2,lpMsg->Type, gObj[aIndex].Name, lpMsg->Msg);
											MuItemShopGiftBuySend(tIndex,ProdIndex3,lpMsg->Type, gObj[aIndex].Name, lpMsg->Msg);
											MuItemShopGiftBuySend(tIndex,ProdIndex4,lpMsg->Type, gObj[aIndex].Name, lpMsg->Msg);
											MuItemShopGiftBuySend(tIndex,ProdIndex5,lpMsg->Type, gObj[aIndex].Name, lpMsg->Msg);
											MuItemShopGiftBuySend(tIndex,ProdIndex6,lpMsg->Type, gObj[aIndex].Name, lpMsg->Msg);
											MuItemShopGiftBuySend(tIndex,ProdIndex7,lpMsg->Type, gObj[aIndex].Name, lpMsg->Msg);
											this->SendPoints(aIndex);
										}
									}
								}
							}else if(lpMsg->ID == 265)
							{
								if(this->HaveSpace(tIndex,0x47,1) == true)
								{
									int ProdIndex1 = -1;
									int num = rand()%2;
									if(num == 0)
									{
										ProdIndex1 = this->GetProductID_Index(ITEMGET(14,61),0,0,1);
									}else
									{
										ProdIndex1 = this->GetProductID_Index(ITEMGET(14,146),0,0,1);
									}
									if( ProdIndex1 != -1)
									{
										if(this->RemovePointsbyPackage(aIndex,lpMsg->Type,packID) == true)
										{
											BYTE buff1[0x09] = { 0xC1, 0x09, 0xD2, 0x03, 0x00, 0xff, 0xff, 0xff, 0xff  };
											DataSend(aIndex, buff1, 0x09);
											WZ_ITEMSHOP_LOG.Output("[CashShop](%s) Gift Item(%s):[%d,%d,%d][%s]%s", gObj[aIndex].AccountID,lpMsg->Name,lpMsg->ID,lpMsg->OptionsID,lpMsg->ItemID,this->GetCategoryID(lpMsg->Category),"PACKAGE");
											gObj[aIndex].CShopInfoModified_Points = 1;
											MuItemShopGiftBuySend(tIndex,ProdIndex1,lpMsg->Type, gObj[aIndex].Name, lpMsg->Msg);
											this->SendPoints(aIndex);
										}
									}
								}
							}
						}break;
						
						case ITEMGET(14,135):
						{
							if(this->HaveSpace(tIndex,0x47,6) == true)
							{
								int ProdIndex3 = -1;
								int ProdIndex1 = this->GetProductID_Index(ITEMGET(14,77),86400,0,0);
								int ProdIndex2 = this->GetProductID_Index(ITEMGET(14,76),86400,0,0);
								if(lpMsg->ID == 201 || lpMsg->ID == 200)
									ProdIndex3 = this->GetProductID_Index(ITEMGET(14,74),86400,0,0);
								else if(lpMsg->ID == 203 || lpMsg->ID == 202)
									ProdIndex3 = this->GetProductID_Index(ITEMGET(14,75),86400,0,0);
								int ProdIndex4 = this->GetProductID_Index(ITEMGET(14,73),86400,0,0);
								int ProdIndex5 = this->GetProductID_Index(ITEMGET(14,72),86400,0,0);
								int ProdIndex6 = this->GetProductID_Index(ITEMGET(14,97),86400,0,0);
								
								if( ProdIndex1 != -1 && 
									ProdIndex2 != -1 && 
									ProdIndex3 != -1 && 
									ProdIndex4 != -1 && 
									ProdIndex5 != -1 && 
									ProdIndex6 != -1)
								{
									if(this->RemovePointsbyPackage(aIndex,lpMsg->Type,packID) == true)
									{
										BYTE buff1[0x09] = { 0xC1, 0x09, 0xD2, 0x03, 0x00, 0xff, 0xff, 0xff, 0xff  };
										DataSend(aIndex, buff1, 0x09);
										WZ_ITEMSHOP_LOG.Output("[CashShop](%s) Gift Item(%s):[%d,%d,%d][%s]%s", gObj[aIndex].AccountID,lpMsg->Name,lpMsg->ID,lpMsg->OptionsID,lpMsg->ItemID,this->GetCategoryID(lpMsg->Category),"PACKAGE");
										gObj[aIndex].CShopInfoModified_Points = 1;
										MuItemShopGiftBuySend(tIndex,ProdIndex1,lpMsg->Type, gObj[aIndex].Name, lpMsg->Msg);
										MuItemShopGiftBuySend(tIndex,ProdIndex2,lpMsg->Type, gObj[aIndex].Name, lpMsg->Msg);
										MuItemShopGiftBuySend(tIndex,ProdIndex3,lpMsg->Type, gObj[aIndex].Name, lpMsg->Msg);
										MuItemShopGiftBuySend(tIndex,ProdIndex4,lpMsg->Type, gObj[aIndex].Name, lpMsg->Msg);
										MuItemShopGiftBuySend(tIndex,ProdIndex5,lpMsg->Type, gObj[aIndex].Name, lpMsg->Msg);
										MuItemShopGiftBuySend(tIndex,ProdIndex6,lpMsg->Type, gObj[aIndex].Name, lpMsg->Msg);
										this->SendPoints(aIndex);
									}
								}
							}
						}break;
						
						case ITEMGET(14,138):
						{
							if(this->HaveSpace(tIndex,0x47,6) == true)
							{
								int ProdIndex1 = this->GetProductID_Index(ITEMGET(14,112),0,0,1);
								int ProdIndex2 = this->GetProductID_Index(ITEMGET(14,113),0,0,1);
								int	ProdIndex3 = this->GetProductID_Index(ITEMGET(14,95),0,0,1);
								int ProdIndex4 = this->GetProductID_Index(ITEMGET(14,92),0,0,1);
								int ProdIndex5 = this->GetProductID_Index(ITEMGET(14,53),0,1,0);
								int ProdIndex6 = this->GetProductID_Index(ITEMGET(14,112),0,0,1);
								
								if( ProdIndex1 != -1 && 
									ProdIndex2 != -1 && 
									ProdIndex3 != -1 && 
									ProdIndex4 != -1 && 
									ProdIndex5 != -1 && 
									ProdIndex6 != -1)
								{
									if(this->RemovePointsbyPackage(aIndex,lpMsg->Type,packID) == true)
									{
										BYTE buff1[0x09] = { 0xC1, 0x09, 0xD2, 0x03, 0x00, 0xff, 0xff, 0xff, 0xff  };
										DataSend(aIndex, buff1, 0x09);
										WZ_ITEMSHOP_LOG.Output("[CashShop](%s) Gift Item(%s):[%d,%d,%d][%s]%s", gObj[aIndex].AccountID,lpMsg->Name,lpMsg->ID,lpMsg->OptionsID,lpMsg->ItemID,this->GetCategoryID(lpMsg->Category),"PACKAGE");
										gObj[aIndex].CShopInfoModified_Points = 1;
										MuItemShopGiftBuySend(tIndex,ProdIndex1,lpMsg->Type, gObj[aIndex].Name, lpMsg->Msg);
										MuItemShopGiftBuySend(tIndex,ProdIndex2,lpMsg->Type, gObj[aIndex].Name, lpMsg->Msg);
										MuItemShopGiftBuySend(tIndex,ProdIndex3,lpMsg->Type, gObj[aIndex].Name, lpMsg->Msg);
										MuItemShopGiftBuySend(tIndex,ProdIndex4,lpMsg->Type, gObj[aIndex].Name, lpMsg->Msg);
										MuItemShopGiftBuySend(tIndex,ProdIndex5,lpMsg->Type, gObj[aIndex].Name, lpMsg->Msg);
										MuItemShopGiftBuySend(tIndex,ProdIndex6,lpMsg->Type, gObj[aIndex].Name, lpMsg->Msg);
										this->SendPoints(aIndex);
									}
								}
							}
						}break;
						
						case ITEMGET(14,139):
						{
							if(this->HaveSpace(tIndex,0x47,3) == true)
							{
								int ProdIndex3 = -1;
								int ProdIndex1 = this->GetProductID_Index(ITEMGET(14,73),86400,0,0);
								int ProdIndex2 = this->GetProductID_Index(ITEMGET(14,72),86400,0,0);
								if(lpMsg->ID == 197 || lpMsg->ID == 196)
									ProdIndex3 = this->GetProductID_Index(ITEMGET(14,74),86400,0,0);
								else if(lpMsg->ID == 199 || lpMsg->ID == 198)
									ProdIndex3 = this->GetProductID_Index(ITEMGET(14,75),86400,0,0);
								
								if( ProdIndex1 != -1 && 
									ProdIndex2 != -1 && 
									ProdIndex3 != -1)
								{
									if(this->RemovePointsbyPackage(aIndex,lpMsg->Type,packID) == true)
									{
										BYTE buff1[0x09] = { 0xC1, 0x09, 0xD2, 0x03, 0x00, 0xff, 0xff, 0xff, 0xff  };
										DataSend(aIndex, buff1, 0x09);
										WZ_ITEMSHOP_LOG.Output("[CashShop](%s) Gift Item(%s):[%d,%d,%d][%s]%s", gObj[aIndex].AccountID,lpMsg->Name,lpMsg->ID,lpMsg->OptionsID,lpMsg->ItemID,this->GetCategoryID(lpMsg->Category),"PACKAGE");
										gObj[aIndex].CShopInfoModified_Points = 1;
										MuItemShopGiftBuySend(tIndex,ProdIndex1,lpMsg->Type, gObj[aIndex].Name, lpMsg->Msg);
										MuItemShopGiftBuySend(tIndex,ProdIndex2,lpMsg->Type, gObj[aIndex].Name, lpMsg->Msg);
										MuItemShopGiftBuySend(tIndex,ProdIndex3,lpMsg->Type, gObj[aIndex].Name, lpMsg->Msg);
										this->SendPoints(aIndex);
									}
								}
							}
						}break;
					}
				}
			}else
			{
				if(this->HaveSpace(tIndex,0x47))
				{
					int ProdIndex = this->GetProductID_Index(ProdID,lpMsg->OptionsID,lpMsg->ItemID);
					if(ProdIndex != -1)
					{
						WZ_ITEMSHOP_LOG.Output("[CashShop](%s) Try to Gift(%s): [%d,%d,%d][%s]%s", gObj[aIndex].AccountID,lpMsg->Name,lpMsg->ID,lpMsg->OptionsID,lpMsg->ItemID,this->GetCategoryID(lpMsg->Category),this->ProdID[ProdIndex].Name);
						if(this->RemovePoints(aIndex,lpMsg->Type,ProdIndex) == true)
						{
							BYTE buff1[0x09] = { 0xC1, 0x09, 0xD2, 0x04, 0x00, 0xff, 0xff, 0xff, 0xff  };
							DataSend(aIndex, buff1, 0x09);
							WZ_ITEMSHOP_LOG.Output("[CashShop](%s) Gift Item(%s):[%d,%d,%d][%s]%s", gObj[aIndex].AccountID,lpMsg->Name,lpMsg->ID,lpMsg->OptionsID,lpMsg->ItemID,this->GetCategoryID(lpMsg->Category),this->ProdID[ProdIndex].Name);
							gObj[aIndex].CShopInfoModified_Points = 1;
							MuItemShopGiftBuySend(tIndex,ProdIndex,lpMsg->Type, gObj[aIndex].Name, lpMsg->Msg);
							this->SendPoints(aIndex);
						}else
						{
							WZ_ITEMSHOP_LOG.Output("[CashShop](%s) Not enough Money(%s):[%d,%d,%d][%s]%s", gObj[aIndex].AccountID,lpMsg->Name,lpMsg->ID,lpMsg->OptionsID,lpMsg->ItemID,this->GetCategoryID(lpMsg->Category),this->ProdID[ProdIndex].Name);
						}
					}
				}
			}
		}
	}
}


bool cMuItemShop::RemovePointsbyPackage(int aIndex, BYTE Type, WORD PackIndex)
{
	if ( aIndex < 0 || aIndex > OBJMAX-1 )
	{
		return false;
	}
	if(gObj[aIndex].Type != OBJ_USER)
	{
		return false;
	}

	int Val = this->PackID[PackIndex].Value;
	switch(Type)
	{
		case 252:
		{
			if(Val <= gObj[aIndex].WCoin)
			{
				gObj[aIndex].WCoin -= Val;
				return true;
			}
		}break;
		case 253:
		{
			if(Val <= gObj[aIndex].WCoinP)
			{
				gObj[aIndex].WCoinP -= Val;
				return true;
			}
		}break;
		case 0:
		{
			if(Val <= gObj[aIndex].GoblinCoin)
			{
				gObj[aIndex].GoblinCoin -= Val;
				return true;
			}
		}break;
	}

	return false;
}

bool cMuItemShop::RemovePoints(int aIndex, BYTE Type, WORD ProdIndex)
{
	if ( aIndex < 0 || aIndex > OBJMAX-1 )
	{
		return false;
	}
	if(gObj[aIndex].Type != OBJ_USER)
	{
		return false;
	}

	int Val = this->ProdID[ProdIndex].Value;
	switch(Type)
	{
		case 252:
		{
			if(Val <= gObj[aIndex].WCoin)
			{
				gObj[aIndex].WCoin -= Val;
				return true;
			}
		}break;
		case 253:
		{
			if(Val <= gObj[aIndex].WCoinP)
			{
				gObj[aIndex].WCoinP -= Val;
				return true;
			}
		}break;
		case 0:
		{
			if(Val <= gObj[aIndex].GoblinCoin)
			{
				gObj[aIndex].GoblinCoin -= Val;
				return true;
			}
		}break;
	}

	return false;
}

BYTE cMuItemShop::GetItemsInListCount(int aIndex, BYTE ListType)
{
	if ( aIndex < 0 || aIndex > OBJMAX-1 )
	{
		return 0;
	}
	if(gObj[aIndex].Type != OBJ_USER)
	{
		return 0;
	}

	MUITEMSHOP tmpShop[MUITEMSHOP_MAXITEMPERPAGE*MUITEMSHOP_MAXPAGES];
	memset(tmpShop,0,sizeof(tmpShop));

	if(ListType == 0x53)
		memcpy(tmpShop,gObj[aIndex].CShop,sizeof(tmpShop));
	else if(ListType == 0x47)
		memcpy(tmpShop,gObj[aIndex].CShopGift,sizeof(tmpShop));

	BYTE Count = 0;

	for(int i=0;i<(MUITEMSHOP_MAXITEMPERPAGE*MUITEMSHOP_MAXPAGES);i++)
	{
		if(tmpShop[i].m_Serial2 > 0)
		{
			Count++;
		}
	}

	return Count;
}

void cMuItemShop::ListSend(int aIndex,BYTE List, BYTE PageIndex)
{
	if ( aIndex < 0 || aIndex > OBJMAX-1 )
	{
		return;
	}
	if(gObj[aIndex].Type != OBJ_USER)
	{
		return;
	}

	MUITEMSHOP tmpShop[MUITEMSHOP_MAXITEMPERPAGE*MUITEMSHOP_MAXPAGES];
	memset(tmpShop,0,sizeof(tmpShop));

	if(List == 0x53) //List
	{
		memcpy(tmpShop,gObj[aIndex].CShop,sizeof(tmpShop));
	}else if(List == 0x47) //Gift
	{
		memcpy(tmpShop,gObj[aIndex].CShopGift,sizeof(tmpShop));
	}

	//Page List Update
	PMSG_CSHOPLISTPAGES_SEND pMsg = {0};
	PHeadSubSetB((LPBYTE)&pMsg, 0xD2, 0x06, sizeof(pMsg));
	int ItemsCount = this->GetItemsInListCount(aIndex,List);
	if(ItemsCount > MUITEMSHOP_MAXITEMPERPAGE)
		pMsg.ItemsCount = MUITEMSHOP_MAXITEMPERPAGE;
	else
		pMsg.ItemsCount = ItemsCount;
	pMsg.ItemsMax = MUITEMSHOP_MAXITEMPERPAGE;
	pMsg.PageIndex = PageIndex;
	pMsg.PageCount = ((ItemsCount-1)/MUITEMSHOP_MAXITEMPERPAGE)+1;
	if(pMsg.PageCount > MUITEMSHOP_MAXPAGES)
		pMsg.PageCount = MUITEMSHOP_MAXPAGES;

	DataSend(aIndex, (UCHAR*)&pMsg, sizeof(pMsg));

	//Show items per page
	int Min = (PageIndex-1) * MUITEMSHOP_MAXITEMPERPAGE;
	int Max = PageIndex * MUITEMSHOP_MAXITEMPERPAGE;

	int Count=0;
	for(int i=0;i<(MUITEMSHOP_MAXITEMPERPAGE*MUITEMSHOP_MAXPAGES);i++)
	{
		if(tmpShop[i].m_Serial2 > 0)
		{
			if(Count >= Max)
			{
				break;
			}
			if(Count >= Min)
			{
				if(List == 0x53) //List
					this->ListAddItem(aIndex,tmpShop[i].ID1, tmpShop[i].ID2, tmpShop[i].m_Serial2);
				else if(List == 0x47) //Gift
					this->ListAddGiftItem(aIndex,tmpShop[i].ID1, tmpShop[i].ID2, tmpShop[i].m_Serial2,gObj[aIndex].CShopGiftMsg[i].Name,gObj[aIndex].CShopGiftMsg[i].Msg);
			}
			Count++;
		}
	}
}

void cMuItemShop::ListReqRecv(int aIndex,PMSG_CSHOPLISTRECV* lpMsg)
{
	if ( aIndex < 0 || aIndex > OBJMAX-1 )
	{
		return;
	}
	if(gObj[aIndex].Type != OBJ_USER)
	{
		return;
	}

	if(lpMsg->List == 0x53) //List
	{
		WZ_ITEMSHOP_LOG.Output("[CashShop](%s) Ask for List Items", gObj[aIndex].AccountID);
		this->ListSend(aIndex,0x53,lpMsg->State);
	}else if(lpMsg->List == 0x47) //Gift List
	{
		WZ_ITEMSHOP_LOG.Output("[CashShop](%s) Ask for Gift List Items", gObj[aIndex].AccountID);
		this->ListSend(aIndex,lpMsg->List,lpMsg->State);
	}
}


void cMuItemShop::SendShopValue(int aIndex)
{
	if ( aIndex < 0 || aIndex > OBJMAX-1 )
	{
		return;
	}
	if(gObj[aIndex].Type != OBJ_USER)
	{
		return;
	}

	if(this->Enabled == TRUE)
	{
		//WZ_ITEMSHOP_LOG.Output("[CashShop](%s) Ask for InGameShopScript", gObj[aIndex].AccountID);
		//CSHOP SEND SCRIPT PACKET
		///*
		//	\Data\InGameShopScript\512.0000.000
		//*/
		PMSG_CSHOPVALUESEND pScript={0};			
		PHeadSubSetB((LPBYTE)&pScript, 0xD2, 0x0C, sizeof(pScript));
		pScript.Value1 = this->SScript1;
		pScript.Value2 = this->SScript2;
		pScript.Value3 = this->SScript3;

		DataSend(aIndex, (UCHAR*)&pScript, sizeof(pScript));

		//CSHOP SEND LOGO PACKET
		///*
		//	\Data\InGameShopBanner\583.0000.000
		//*/

		//WZ_ITEMSHOP_LOG.Output("[CashShop](%s) Ask for InGameShopBanner", gObj[aIndex].AccountID);
		PMSG_CSHOPVALUESEND pBanner={0};			
		PHeadSubSetB((LPBYTE)&pBanner, 0xD2, 0x15, sizeof(pBanner));
		pBanner.Value1 = this->SBanner1;
		pBanner.Value2 = this->SBanner2;
		pBanner.Value3 = this->SBanner3;

		DataSend(aIndex, (UCHAR*)&pBanner, sizeof(pBanner));
	}
}

void cMuItemShop::Init(char * FilePath)
{
	this->Enabled				= GetPrivateProfileInt("Common", "SCFIsCashShopEnabled",0, FilePath) ;
	
	this->SScript1				= GetPrivateProfileInt("InGameShopScript", "SCFValue1",512, FilePath) ;
	this->SScript2				= GetPrivateProfileInt("InGameShopScript", "SCFValue2",2011, FilePath) ;
	this->SScript3				= GetPrivateProfileInt("InGameShopScript", "SCFValue3",47, FilePath) ;
	
	this->SBanner1				= GetPrivateProfileInt("InGameShopBanner", "SCFValue1",583, FilePath) ;
	this->SBanner2				= GetPrivateProfileInt("InGameShopBanner", "SCFValue2",2011, FilePath) ;
	this->SBanner3				= GetPrivateProfileInt("InGameShopBanner", "SCFValue3",1, FilePath) ;

	this->IsAddGoblinShopPointsOnMixFail			= GetPrivateProfileInt("WzShopExtraCBMix", "SCFIsAddGoblinShopPointsOnMixFail",0, FilePath) ;
	this->OnFailAddPercentsOfSuccessAsPointsDiv		= GetPrivateProfileInt("WzShopExtraCBMix", "SCFOnFailAddPercentsOfSuccessAsPointsDiv",1, FilePath) ;
	this->IsAddGoblinShopPointsOnMixSuccess			= GetPrivateProfileInt("WzShopExtraCBMix", "SCFIsAddGoblinShopPointsOnMixSuccess",0, FilePath) ;
	this->OnSuccessAddPercentsOfSuccessAsPointsDiv	= GetPrivateProfileInt("WzShopExtraCBMix", "SCFOnSuccessAddPercentsOfSuccessAsPointsDiv",1, FilePath) ;

	if (this->OnSuccessAddPercentsOfSuccessAsPointsDiv < 0 || this->OnSuccessAddPercentsOfSuccessAsPointsDiv > 100)
		this->OnSuccessAddPercentsOfSuccessAsPointsDiv = 1;
	if (this->OnFailAddPercentsOfSuccessAsPointsDiv < 0 || this->OnFailAddPercentsOfSuccessAsPointsDiv > 100)
		this->OnFailAddPercentsOfSuccessAsPointsDiv = 1;

}

void cMuItemShop::Read_Package(char * FilePath)
{
	if(this->Enabled == TRUE)
	{
		int Num=0;
		int Token;
		SMDFile = fopen(FilePath, "r");

		this->PackageCount = 0;

		if ( SMDFile == NULL )
		{
			MsgBox("MuItemShop Package data load error %s", FilePath);
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

					if(Num >= CSHOP_PACKAGE_COUNT)
					{
						MsgBox("MuItemShop Package error: Items out of range!");
						return;
					}
					this->PackID[Num].CatID = TokenNumber;

					Token = GetToken();
					this->PackID[Num].Sort = TokenNumber;

					Token = GetToken();
					this->PackID[Num].PackID = TokenNumber;

					Token = GetToken();
					this->PackID[Num].ProdID = TokenNumber;

					Token = GetToken();
					this->PackID[Num].ItemID = TokenNumber;

					Token = GetToken();
					this->PackID[Num].Value = TokenNumber;

					for(int i=0;i<CSHOP_PACKAGE_ID2_COUNT;i++)
					{
						Token = GetToken();
						this->PackID[Num].ID2[i] = TokenNumber;
					}

					Num++;
					this->PackageCount++;
				}
			}
			break;
		}
				
		WZ_ITEMSHOP_LOG.Output("[MuItemShop Package] - %s file is Loaded",FilePath);
		fclose(SMDFile);
	}
}

void cMuItemShop::Read_Product(char * FilePath)
{
	if(this->Enabled == TRUE)
	{
		int Num=0;
		int Token;
		SMDFile = fopen(FilePath, "r");

		this->ProductCount = 0;

		if ( SMDFile == NULL )
		{
			MsgBox("MuItemShop Product data load error %s", FilePath);
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

					if(Num >= CSHOP_PRODUCT_COUNT)
					{
						MsgBox("MuItemShop Product error: Items out of range!");
						return;
					}

					this->ProdID[Num].ID1 = TokenNumber;

					Token = GetToken();
					this->ProdID[Num].ID2 = TokenNumber;

					Token = GetToken();
					this->ProdID[Num].Value = TokenNumber;

					Token = GetToken();
					this->ProdID[Num].ItemID = TokenNumber;

					Token = GetToken();
					this->ProdID[Num].Dur = TokenNumber;

					Token = GetToken();
					this->ProdID[Num].Rate = TokenNumber;

					Token = GetToken();
					this->ProdID[Num].Quant = TokenNumber;

					Token = GetToken();
					this->ProdID[Num].Days = TokenNumber;

					Token = GetToken();
					strcpy( this->ProdID[Num].Name, TokenString);

					Num++;
					this->ProductCount++;
				}
			}
			break;
		}
				
		WZ_ITEMSHOP_LOG.Output("[MuItemShop Product] - %s file is Loaded",FilePath);
		fclose(SMDFile);
	}
}

void cMuItemShop::Read_Category(char * FilePath)
{
	if(this->Enabled == TRUE)
	{
		int Num=0;
		int Token;
		SMDFile = fopen(FilePath, "r");

		this->CategoryCount = 0;

		if ( SMDFile == NULL )
		{
			MsgBox("MuItemShop Category data load error %s", FilePath);
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

					if(Num >= CSHOP_CATEGORY_COUNT)
					{
						MsgBox("MuItemShop Category error: Items out of range!");
						return;
					}

					this->CatID[Num].ID = TokenNumber;

					Token = GetToken();
					strcpy( this->CatID[Num].Name, TokenString);

					Num++;
					this->CategoryCount++;
				}
			}
			break;
		}
				
		WZ_ITEMSHOP_LOG.Output("[MuItemShop Category] - %s file is Loaded",FilePath);
		fclose(SMDFile);
	}
}

void cMuItemShop::EarnGoblinPointsCBMix(int aIndex, int points, int isFail)
{
	if (aIndex < 0 || aIndex > OBJMAX-1)
	{
		return;
	}
	if(gObj[aIndex].Type != OBJ_USER)
	{
		return;
	}

	if (isFail == 1)
	{
		if (this->OnFailAddPercentsOfSuccessAsPointsDiv > 0)
		{
			points = points / this->OnFailAddPercentsOfSuccessAsPointsDiv;
		} else 
		{
			return;
		}
	}
	else
	{
		if (this->OnSuccessAddPercentsOfSuccessAsPointsDiv > 0)
		{
			points = points / this->OnSuccessAddPercentsOfSuccessAsPointsDiv;
		} else 
		{
			return;
		}
	}

	if(points <= 0)
	{
		return;
	}

	if(this->Enabled == TRUE)
	{
		if ((isFail == 1 && this->IsAddGoblinShopPointsOnMixFail == 1) ||
			(isFail == 0 && this->IsAddGoblinShopPointsOnMixSuccess == 1))
		{
			if (gObj[aIndex].CShopInfoRequested == 2)
			{
				gObj[aIndex].GoblinCoin += points;
				gObj[aIndex].CShopInfoModified_Points = 1;

				char sbuf[100]={0};
				wsprintf(sbuf,"Goblin Points Earned: %d",points);
				GCServerMsgStringSend(sbuf,aIndex, 1);

				WZ_ITEMSHOP_LOG.Output("[CBMix](%s)(%s) IS_FAIL:%d Goblin Points Earned: %d", 
					gObj[aIndex].AccountID, gObj[aIndex].Name,
					isFail, points
				);

				this->SendPoints(aIndex);
			} else {
				MuItemShopUpdateGoblinCoin(aIndex, points);

				char sbuf[100]={0};
				wsprintf(sbuf,"Goblin Points Earned: %d",points);
				GCServerMsgStringSend(sbuf,aIndex, 1);
			}
		}
	}
}