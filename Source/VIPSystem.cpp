#include "stdafx.h"
#include "VIPSystem.h"
#include "GameServer.h"
#include "LogProc.h"
#include "GameMain.h"
#include "TNotice.h"
#include "..\include\readscript.h"
//#include "GameServerAuth.h"
#include "..\common\winutil.h"
#include "EDSProtocol.h"
#include "ConMember.h"

#if (PACK_EDITION>=2)

cVipSystem VipSystem;

struct PMSG_BUYRESULT
{
	PBMSG_HEAD h;	// C1:32
	BYTE Result;	// 3
	BYTE ItemInfo[MAX_ITEM_INFO];	// 4
};

struct FHP_FRIEND_MEMO_SEND
{
	PWMSG_HEAD h;
	short Number;	// 4
	DWORD WindowGuid;	// 8
	char Name[10];	// C
	char ToName[10];	// 16
	char Subject[32];	// 20
	BYTE Dir;	// 40
	BYTE Action;	// 41
	short MemoSize;	// 42
	BYTE Photo[18];	// 44
	char Memo[1000];	// 56
};

void cVipSystem__ConnectMemberReloader(void * lpParams)
{
	VipSystem.ConnectMemberReload_Status = FALSE;
	while(VipSystem.IsConnectMemberReload == TRUE)
	{
		VipSystem.ConnectMemberReload_Status = TRUE;
		Sleep(VipSystem.ConnectMemberReload_Mins);
		ConMember.Load(ReadConfig.ConnDataFiles[38]);
		LogAddTD("ConnectMember Reloaded!");
	}
	VipSystem.ConnectMemberReload_Status = FALSE;
}

void cVipSystem::Init(char * FilePath)
{
	this->IsVipSwitchServer			= GetPrivateProfileInt("Common", "AllowVIPSwitchServer",0, FilePath) ;

	this->IsVipServer				= GetPrivateProfileInt("VIP System", "SCFVipServer",0, FilePath) ;
	this->VipExperience				= GetPrivateProfileInt("VIP System", "SCFVipExperience",0, FilePath) ;
	this->VipIsApplyCWExpPenalty	= GetPrivateProfileInt("VIP System", "SCFVipIsApplyCWExpPenalty",0, FilePath) ;
	this->VipIsApplyResetExpPenalty	= GetPrivateProfileInt("VIP System", "SCFVipIsApplyResetExpPenalty",0, FilePath) ;
	this->ResetFreeLimitEnabled		= GetPrivateProfileInt("VIP System", "SCFIsResetFreeLimit",0, FilePath) ;
	this->ResetFreeLimitMax			= GetPrivateProfileInt("VIP System", "SCFFreeResetMax",10, FilePath) ;

	this->VipBuy					= GetPrivateProfileInt("VIP System", "SCFVipBuy",0, FilePath) ;
	this->VipRenewPoints			= GetPrivateProfileInt("VIP System", "SCFVipRenewPoints",50, FilePath) ;

	this->IsConnectMemberReload		= GetPrivateProfileInt("VIP System", "SCFConnectMemberReloadEnabled",1, FilePath) ;
	this->ConnectMemberReload_Mins	= (GetPrivateProfileInt("VIP System", "SCFConnectMemberReloadMins",60, FilePath) * (1000 * 60)) ;

	if(this->IsConnectMemberReload == TRUE && this->ConnectMemberReload_Status == FALSE)
		_beginthread( cVipSystem__ConnectMemberReloader, 0, NULL  );
	
	LogAddTD("[SCF VIPSystem] - %s file is Loaded",FilePath);
}


void cVipSystem::State(LPOBJ lpObj)
{
	char Tmp[512]={0};
	if (lpObj->VipDays >= 1)
	{
		wsprintf(Tmp,lMsg.Get(MSGGET(14, 57)),lpObj->VipDays,lpObj->VipMoney);
		GCServerMsgStringSend(Tmp,lpObj->m_Index, 0x00);
	} else {
		if (lpObj->VipMoney > 0)
		{
			wsprintf(Tmp,lMsg.Get(MSGGET(14, 153)),lpObj->VipMoney);
			GCServerMsgStringSend(Tmp,lpObj->m_Index, 0x00);
		}
	}
}


void cVipSystem::BuyDays(LPOBJ lpObj, int Days)
{
	int RenewCost = (Days * this->VipRenewPoints);
	if(lpObj->VipMoney >= RenewCost)
	{
		lpObj->AccountExtraInfoModified = 1;

		lpObj->VipMoney -= RenewCost;
		lpObj->VipDays += Days;
		lpObj->Vip = 1;

		char sbuf[1024]={0};
		wsprintf(sbuf, "Status: VIP: %d / VIP Days: %d / VIP Money: %d",lpObj->Vip,lpObj->VipDays,lpObj->VipMoney);
		GCServerMsgStringSend(sbuf,lpObj->m_Index, 0x00);
	}
}

void cVipSystem::BuyItem(LPOBJ lpObj, int IDNumber)
{
	if(IDNumber >= 0 && IDNumber <= (this->ItemsCount-1))
	{
		BOOL IfBuy = FALSE;	
		PMSG_BUYRESULT pResult;
		PHeadSetB((LPBYTE)&pResult, 0x32, sizeof(pResult));

		if(this->Shop[IDNumber].Cost < lpObj->VipMoney)
		{
			pResult.Result = gObjShopBuyInventoryInsertItem(lpObj->m_Index,this->m_item[IDNumber]);
				
			if ( pResult.Result != 0xFF )
			{
				lpObj->AccountExtraInfoModified = 1;

				ItemByteConvert((LPBYTE)&pResult.ItemInfo, this->m_item[IDNumber]);
				DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);
				lpObj->VipMoney -= this->Shop[IDNumber].Cost;
				IfBuy = TRUE;
			}else
			{				
				GCServerMsgStringSend(lMsg.Get(MSGGET(14, 58)),lpObj->m_Index, 0x01);
			}
		}
		//Log
		if(IfBuy == TRUE)
		{
			LogAddTD("[VIPShop] BuyItem [%s][%s] Item {%d %d} Cost: %d",lpObj->AccountID,lpObj->Name,this->Shop[IDNumber].Type,this->Shop[IDNumber].Index,this->Shop[IDNumber].Cost);
		} else 
		{
			LogAddTD("[VIPShop] Attempt to BuyItem [%s][%s] Item {%d %d} Cost: %d",lpObj->AccountID,lpObj->Name,this->Shop[IDNumber].Type,this->Shop[IDNumber].Index,this->Shop[IDNumber].Cost);
		}
	}
}

void cVipSystem::BuyList(LPOBJ lpObj, char * sType)
{
	char Temp[1000]={0};
	char Nuevo[1000]={0};
	for(int i=0;i<this->ItemsCount;i++)
	{
		if (strcmp(sType,Shop[i].sType) == 0)
		{
			wsprintf(Temp, "ID: %d - Type: %s - Item: %s - Price: %d\n", i, this->Shop[i].sType, this->Shop[i].sName, this->Shop[i].Cost);
			strcat(Nuevo,Temp);
		}
	}
	
	FHP_FRIEND_MEMO_SEND pMsg;

	pMsg.WindowGuid = 0;
	pMsg.Number = lpObj->m_Index;
	pMsg.Dir = 143;
	pMsg.Action = 27;
	wsprintf(pMsg.Subject,"VIP Shop List");
	memcpy(pMsg.Name, lpObj->Name, sizeof(pMsg.Name));
	memcpy(pMsg.Photo, lpObj->CharSet, sizeof(pMsg.Photo));
	memcpy(pMsg.ToName, lpObj->Name, sizeof(pMsg.ToName));
	strcpy(pMsg.Memo,Nuevo);
	pMsg.MemoSize = strlen(Nuevo);
	int bsize = pMsg.MemoSize + sizeof(pMsg) - sizeof(pMsg.Memo);
	pMsg.h.set((LPBYTE)&pMsg, 0x70, bsize);
	wsExDbCli.DataSend((PCHAR)&pMsg, bsize);

	LogAdd("[%s] VIP Shop Mail Send",  lpObj->Name);
}

bool cVipSystem::LoadShopItem(char* filename )
{
	int Token;

	SMDFile = fopen(filename, "r");

	if ( SMDFile == NULL )
	{
		MsgBox("VIPShop data load error %s", filename);
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

					this->Shop[ItemsCount].Cost = TokenNumber;

					Token = GetToken();
					this->Shop[ItemsCount].Type = TokenNumber;

					Token = GetToken();
					this->Shop[ItemsCount].Index = TokenNumber;

					Token = GetToken();
					this->Shop[ItemsCount].Dur = TokenNumber;

					Token = GetToken();
					this->Shop[ItemsCount].Level = TokenNumber;

					Token = GetToken();
					this->Shop[ItemsCount].Opt = TokenNumber;

					Token = GetToken();
					this->Shop[ItemsCount].Skill = TokenNumber;

					Token = GetToken();
					this->Shop[ItemsCount].Luck = TokenNumber;

					Token = GetToken();
					this->Shop[ItemsCount].Exc = TokenNumber;

					Token = GetToken();
					this->Shop[ItemsCount].NewOpt1 = TokenNumber;

					Token = GetToken();
					this->Shop[ItemsCount].NewOpt2 = TokenNumber;

					Token = GetToken();
					this->Shop[ItemsCount].NewOpt3 = TokenNumber;

					Token = GetToken();
					this->Shop[ItemsCount].NewOpt4 = TokenNumber;

					Token = GetToken();
					this->Shop[ItemsCount].NewOpt5 = TokenNumber;

					Token = GetToken2();
					strcpy(this->Shop[ItemsCount].sType, TokenString);
					Token = GetToken2();
					strcpy(this->Shop[ItemsCount].sName, TokenString);

					int itemp = this->Shop[ItemsCount].Type * MAX_SUBTYPE_ITEMS + this->Shop[ItemsCount].Index;
					
					this->m_item[ItemsCount].m_Level = this->Shop[ItemsCount].Level;
					this->m_item[ItemsCount].m_Durability = this->Shop[ItemsCount].Dur;
					this->m_item[ItemsCount].Convert(itemp, this->Shop[ItemsCount].Luck, this->Shop[ItemsCount].Opt, this->Shop[ItemsCount].Skill, this->Shop[ItemsCount].Exc, 0, 0, CURRENT_DB_VERSION);
					this->m_item[ItemsCount].m_ItemSlot1 = this->Shop[ItemsCount].NewOpt1;
					this->m_item[ItemsCount].m_ItemSlot2 = this->Shop[ItemsCount].NewOpt2;
					this->m_item[ItemsCount].m_ItemSlot3 = this->Shop[ItemsCount].NewOpt3;
					this->m_item[ItemsCount].m_ItemSlot4 = this->Shop[ItemsCount].NewOpt4;
					this->m_item[ItemsCount].m_ItemSlot5 = this->Shop[ItemsCount].NewOpt5;
					this->m_item[ItemsCount].Value();
					

					this->ItemsCount++;

					if(ItemsCount >= 1023)
						break;
				}
			}
		break;
	}
		
	fclose(SMDFile);
	return true;
}

#endif