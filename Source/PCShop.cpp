#include "stdafx.h"
#include "GameServer.h"
#include "LogProc.h"
#include "GameMain.h"
#include "TNotice.h"
#include "..\include\readscript.h"
//#include "GameServerAuth.h"
#include "..\common\winutil.h"
#include "protocol.h"
#include "Shop.h"
#include "PCShop.h"
#include "DSProtocol.h"

#if (PACK_EDITION>=2)

CPCShop PCShop;

void CPCShop::ShopLoad()
{	
	PMSG_PCSHOPINFO pcShopInfo;
	PMSG_PCSHOPITEMCOUNT pShopItemCount;
	PMSG_PCSHOPITEMSINFO pMsg;

	int lOfs = 0;
	
	PHeadSubSetB((LPBYTE)&pcShopInfo, 0xD0, 0x06, 5);
	pcShopInfo.unk = 0;
	lOfs += sizeof(pcShopInfo );
	memcpy(SendByte, &pcShopInfo, sizeof(pcShopInfo));
	pShopItemCount.h.c = 0xC2;
	pShopItemCount.h.headcode = 0x31;
	pShopItemCount.HICount = HIBYTE(ItemsCount);
	pShopItemCount.LOCount = LOBYTE(ItemsCount);
	memcpy(&SendByte[lOfs], &pShopItemCount, sizeof(pShopItemCount));
	lOfs += sizeof(pShopItemCount );

	for(int i=0;i<ItemsCount;i++)
	{
		pMsg.Position = ShopInventoryMap[i].Position;
		pMsg.Index = ShopInventoryMap[i].Index;
		pMsg.Opts = ( (ShopInventoryMap[i].Opt/4) + (ShopInventoryMap[i].Luck*4) + (ShopInventoryMap[i].Level*8) + (ShopInventoryMap[i].Skill*128) );
		pMsg.Dur = ShopInventoryMap[i].Dur;
		pMsg.unk1 = 0;
		pMsg.Exc = ShopInventoryMap[i].Exc;
		pMsg.Type16 = ShopInventoryMap[i].Type*16;
		pMsg.NewOpt1 = ShopInventoryMap[i].NewOpt1-1;
		pMsg.NewOpt2 = ShopInventoryMap[i].NewOpt2-1;
		pMsg.NewOpt3 = ShopInventoryMap[i].NewOpt3-1;
		pMsg.NewOpt4 = ShopInventoryMap[i].NewOpt4-1;
		pMsg.NewOpt5 = ShopInventoryMap[i].NewOpt5-1;
		pMsg.unk2 = 0;
		memcpy(&this->SendByte[lOfs], &pMsg, sizeof(pMsg));	
		///if( i + 1 < ItemsCount)
		lOfs += sizeof(pMsg);
	}
	this->SendByte[6] = HIBYTE(lOfs);
	this->SendByte[7] = LOBYTE(lOfs);
	this->PCShopPacketSize = lOfs;
	pShopItemCount.h.sizeH = HIBYTE(this->PCShopPacketSize);
	pShopItemCount.h.sizeL = LOBYTE(this->PCShopPacketSize);
}

int CPCShop::InventoryMapCheck(int type, int index)
{
	int X;
	int Y;
	int itemp = type * MAX_SUBTYPE_ITEMS + index;
	ItemGetSize(itemp, X, Y);
	
	if ( X < 0 || Y < 0 )
	{
		// Error in getting item size in shop %s %d
		MsgBox(lMsg.Get(MSGGET(2, 54)), __FILE__, __LINE__);
		return FALSE;
	}

	int YPos=0;
	int XPos=0;
	int j=-1;
	//Y-=1;

	while(YPos<15)
	{
		XPos=0;
		while(XPos<8)
		{
			if((this->PC_Shop[XPos+(YPos*8)]==0) && (this->PC_Shop[XPos+(YPos*8)+X-1]==0) && (XPos+X-1<8))
			{
				if(Y==0)
				{
					for(j=0;j<X;j++)
						this->PC_Shop[XPos+(YPos*8)]=1;
					return (XPos+(YPos*8));
				}else
				{

					if((this->PC_Shop[(XPos+(YPos*8))+(Y*8)]==0) /*&& (PC_Shop[(XPos+(YPos*8)+X)+(Y*8)]==0)*/ && (XPos+X-1<8))
					{
						for(j=0;j<X;j++)
						{
							for(int z=1;z<=Y;z++)
							{
								this->PC_Shop[XPos+(YPos*8)+j+z*8]=1;
							}
							this->PC_Shop[XPos+(YPos*8)+j]=1;
						}
						return (XPos+(YPos*8));
					}
				}
			}
			XPos++;
		}
		YPos++;
	}

	return j;
}

bool CPCShop::LoadShopIni(char* filename )
{
	ReadConfig.CPCShop__SendDataOnlyForGM	= GetPrivateProfileInt("PCPointShop", "SCFOnlyForGM",0, filename) ;
	ReadConfig.CPCShop__MonsterDieAddPoints	= GetPrivateProfileInt("PCPointShop", "SCFAddPointsWhenKillMobs",0, filename) ;

	return true;
}

bool CPCShop::LoadShopItem(char* filename )
{
	int Token;

	this->ItemsCount = 0;

	for(int i=0;i<MAX_MONSTER_TYPE;i++)
		this->MonsterAddPoints[i] = 0;

	memset(this->ShopInventoryMap, 0, sizeof(this->ShopInventoryMap));
	memset(this->m_item, 0, sizeof(this->m_item));
	memset(this->PC_Shop, 0, sizeof(this->PC_Shop));

	SMDFile = fopen(filename, "r");

	if ( SMDFile == NULL )
	{
		MsgBox("PCShop data load error %s", filename);
		return FALSE;
	}

	while ( true )
	{
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
					this->ShopInventoryMap[ItemsCount].Type = TokenNumber;

					Token = GetToken();
					this->ShopInventoryMap[ItemsCount].Index = TokenNumber;

					Token = GetToken();
					this->ShopInventoryMap[ItemsCount].Level = TokenNumber;

					Token = GetToken();
					this->ShopInventoryMap[ItemsCount].Opt = TokenNumber;

					Token = GetToken();
					this->ShopInventoryMap[ItemsCount].Luck = TokenNumber;

					Token = GetToken();
					this->ShopInventoryMap[ItemsCount].Skill = TokenNumber;

					Token = GetToken();
					this->ShopInventoryMap[ItemsCount].Dur = TokenNumber;

					Token = GetToken();
					this->ShopInventoryMap[ItemsCount].Exc = TokenNumber;

					Token = GetToken();
					this->ShopInventoryMap[ItemsCount].NewOpt1 = TokenNumber;

					Token = GetToken();
					this->ShopInventoryMap[ItemsCount].NewOpt2 = TokenNumber;

					Token = GetToken();
					this->ShopInventoryMap[ItemsCount].NewOpt3 = TokenNumber;

					Token = GetToken();
					this->ShopInventoryMap[ItemsCount].NewOpt4 = TokenNumber;

					Token = GetToken();
					this->ShopInventoryMap[ItemsCount].NewOpt5 = TokenNumber;

					Token = GetToken();
					this->ShopInventoryMap[ItemsCount].Cost = TokenNumber;

					this->ShopInventoryMap[ItemsCount].Position = this->InventoryMapCheck(this->ShopInventoryMap[ItemsCount].Type,this->ShopInventoryMap[ItemsCount].Index);

					int itemp = this->ShopInventoryMap[ItemsCount].Type * MAX_SUBTYPE_ITEMS + this->ShopInventoryMap[ItemsCount].Index;
					
					this->m_item[ItemsCount].m_Level = this->ShopInventoryMap[ItemsCount].Level;
					this->m_item[ItemsCount].m_Durability = this->ShopInventoryMap[ItemsCount].Dur;
					this->m_item[ItemsCount].Convert(itemp, this->ShopInventoryMap[ItemsCount].Luck, this->ShopInventoryMap[ItemsCount].Opt, this->ShopInventoryMap[ItemsCount].Skill, this->ShopInventoryMap[ItemsCount].Exc, 0, 0, CURRENT_DB_VERSION);
					this->m_item[ItemsCount].m_ItemSlot1 = this->ShopInventoryMap[ItemsCount].NewOpt1;
					this->m_item[ItemsCount].m_ItemSlot2 = this->ShopInventoryMap[ItemsCount].NewOpt2;
					this->m_item[ItemsCount].m_ItemSlot3 = this->ShopInventoryMap[ItemsCount].NewOpt3;
					this->m_item[ItemsCount].m_ItemSlot4 = this->ShopInventoryMap[ItemsCount].NewOpt4;
					this->m_item[ItemsCount].m_ItemSlot5 = this->ShopInventoryMap[ItemsCount].NewOpt5;
					this->m_item[ItemsCount].Value();

					this->ItemsCount++;
				}
			}
			iType = GetToken();
			iType = TokenNumber;
			if ( iType == 2 )
			{
				while(true)
				{
					Token = GetToken();
					if ( strcmp("end", TokenString) == 0 )
					{
						break;
					}
					int MobID = TokenNumber;

					Token = GetToken();
					if (MobID < MAX_MONSTER_TYPE)
					{
						if (TokenNumber <= 255)
						{
							this->MonsterAddPoints[MobID] = TokenNumber;
						} else {
							this->MonsterAddPoints[MobID] = 255;
							LogAddTD("[PCShop][Error] - Reward overflow for MobID:%d (Points: %d)",MobID,TokenNumber);
						}
					}
				}
			}
		break;
	}
		

	this->ShopLoad();
	fclose(SMDFile);
	return true;
}


void CPCShop::MonsterDie(LPOBJ lpObj, LPOBJ lmObj)
{
	if( ReadConfig.CPCShop__MonsterDieAddPoints == 1 )
	{
		if (lmObj->Class >= MAX_MONSTER_TYPE)
			return;

		BYTE AddPoints = this->MonsterAddPoints[lmObj->Class];
		if(AddPoints > 0)
		{
			char sBuf[255] = {0};

			if (lpObj->PCPoints < PCPOINTS_MAX_VALUE)
			{
				wsprintf(sBuf, lMsg.Get(MSGGET(14, 56)),AddPoints);
				GCServerMsgStringSend(sBuf, lpObj->m_Index, 1);

				lpObj->PCPoints += AddPoints;
				lpObj->PlayerExtraInfoModified = 1;

				if (lpObj->PCPoints >= PCPOINTS_MAX_VALUE)
					lpObj->PCPoints = PCPOINTS_MAX_VALUE;

				this->SendPoints(lpObj);
			}
		}
	}
}

void CPCShop::SendData(LPOBJ lpObj)
{
	if(ReadConfig.S5E2 == 0)
	{
		if (((ReadConfig.CPCShop__SendDataOnlyForGM == 1) && (lpObj->Authority >= 8)) || 
			 (ReadConfig.CPCShop__SendDataOnlyForGM == 0))
		{
			int aIndex = lpObj->m_Index;
			DataSend(aIndex, this->SendByte, this->PCShopPacketSize);
		}
	}
}

int CPCShop::SearchIndex(BYTE Position)
{
	for(int i=0;i < this->ItemsCount; i++)
	{
		if(Position == this->ShopInventoryMap[i].Position)
			return i;
	}
	return (-1);
}

void CPCShop::SendPoints(LPOBJ lpObj)
{
	if((ReadConfig.S5E2 == 1 && lpObj->Vip == 1) || (ReadConfig.S5E2 == 0))
	{
		PMSG_PCSHOPPOINTINFO pMsg;
		PHeadSetB((LPBYTE)&pMsg, 0xD0, sizeof(pMsg));
		pMsg.Type = 4;
		pMsg.Points = lpObj->PCPoints;
		pMsg.MaxPoints = PCPOINTS_MAX_VALUE; 
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, sizeof(pMsg));
	}

	if(ReadConfig.S5E2 == 1)
	{
		if (lpObj->PCPoints > 0)
		{
			char sbuf[100]={0};
			wsprintf(sbuf,"PCPoints = %d", lpObj->PCPoints);
			GCServerMsgStringSend(sbuf,lpObj->m_Index, 1);
		}
	}
}

struct PMSG_BUYRESULT
{
	PBMSG_HEAD h;	// C1:32
	BYTE Result;	// 3
	BYTE ItemInfo[MAX_ITEM_INFO];	// 4
};

void CPCShop::BuyItem(LPOBJ lpObj,BYTE Position)
{	
	if(ReadConfig.S5E2 == 0)
	{
		int i = this->SearchIndex(Position);
		BOOL IfBuy = FALSE;
		
		PMSG_BUYRESULT pResult;

		PHeadSetB((LPBYTE)&pResult, 0x32, sizeof(pResult));

		if(i != (-1))
		{
			if(this->ShopInventoryMap[i].Cost <= lpObj->PCPoints)
			{
				pResult.Result = gObjShopBuyInventoryInsertItem(lpObj->m_Index,this->m_item[i]);
					
				if ( pResult.Result != 0xFF )
				{
					ItemByteConvert((LPBYTE)&pResult.ItemInfo, this->m_item[i]);
					DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);
					lpObj->PCPoints -= this->ShopInventoryMap[i].Cost;
					lpObj->PlayerExtraInfoModified = 1;
					this->SendPoints(lpObj);
					IfBuy = TRUE;
				}
			}
		}
		//Log
		if((IfBuy == TRUE) && (i != (-1)))
		{
			LogAddTD("[PCPointShop] BuyItem [%s][%s] Item {%d %d} Cost: %d",lpObj->AccountID,lpObj->Name,this->ShopInventoryMap[i].Type,this->ShopInventoryMap[i].Index,this->ShopInventoryMap[i].Cost);
		} else 
		{
			LogAddTD("[PCPointShop] Attempt to BuyItem [%s][%s] Item {%d %d} Cost: %d",lpObj->AccountID,lpObj->Name,this->ShopInventoryMap[i].Type,this->ShopInventoryMap[i].Index,this->ShopInventoryMap[i].Cost);
		}
	}
}

#endif