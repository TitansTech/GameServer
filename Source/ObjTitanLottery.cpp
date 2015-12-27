#include "stdafx.h"
#if (PACK_EDITION>=3)
#include "SCFExDB_Defines.h"
#include "GameServer.h"
#include "LogProc.h"
#include "TNotice.h"
#include "..\include\readscript.h"
#include "Event.h"
#include "DBSockMng.h"
#include "ObjTitanLottery.h"
#include "..\common\winutil.h"

cObjTitanLottery ObjTitanLottery;

void cObjTitanLottery::Read(char * FilePath)
{
	this->Enabled = 0;
	this->Start = false;
	memset(&this->ExReward,0,sizeof(this->ExReward));
	memset(&this->Nums,0,sizeof(this->Nums));

	int Token;
	SMDFile = fopen(FilePath, "r");

	if ( SMDFile == NULL )
	{
		MsgBox("TitanLottery data load error %s", FilePath);
		return;
	}

	while ( true )
	{
		int iType = GetToken();
		iType = TokenNumber;

		if ( iType == 1 )
		{
			while(true)
			{
				Token = GetToken();
				if ( strcmp("end", TokenString) == 0 )
				{
					break;
				}

				this->Enabled = TokenNumber;

				Token = GetToken();
				this->Info.Type = TokenNumber;

				Token = GetToken();
				this->TypeCost = TokenNumber;

				Token = GetToken();
				this->Cost = TokenNumber;

				Token = GetToken();
				this->Taxes = TokenNumber;

				Token = GetToken();
				this->MaxNumber = TokenNumber;

				Token = GetToken();
				this->Info.MinReward = TokenNumber;

				Token = GetToken();
				this->Info.MaxReward = TokenNumber;

				Token = GetToken();
				this->MaxTicketBuy = TokenNumber;

				Token = GetToken();
				this->Info.AdditionalReward = TokenNumber;
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

				int iIndex = 0;
				int iType = 0;

				this->ExReward.Zen = TokenNumber;

				Token = GetToken();
				this->ExReward.VipMoney = TokenNumber;

				Token = GetToken();
				iType = TokenNumber;

				Token = GetToken();
				iIndex = TokenNumber;
				
				this->ExReward.num = ITEMGET(iType,iIndex);

				Token = GetToken();
				this->ExReward.Level = TokenNumber;

				Token = GetToken();
				this->ExReward.Opt = TokenNumber;

				Token = GetToken();
				this->ExReward.Luck = TokenNumber;

				Token = GetToken();
				this->ExReward.Skill = TokenNumber;

				Token = GetToken();
				this->ExReward.Dur = TokenNumber;

				Token = GetToken();
				this->ExReward.Exc = TokenNumber;

				Token = GetToken();
				this->ExReward.Anc = TokenNumber;

				Token = GetToken();
				this->ExReward.JOH = TokenNumber;

				Token = GetToken();
				this->ExReward.Sock1 = TokenNumber;

				Token = GetToken();
				this->ExReward.Sock2 = TokenNumber;

				Token = GetToken();
				this->ExReward.Sock3 = TokenNumber;

				Token = GetToken();
				this->ExReward.Sock4 = TokenNumber;

				Token = GetToken();
				this->ExReward.Sock5 = TokenNumber;

				Token = GetToken();
				this->ExReward.Days = TokenNumber;

				Token = GetToken();
				this->ExReward.IsForeverFFFE = TokenNumber;
			}
		}
		break;
	}

	LogAddTD("[TitanLottery] - %s file is Loaded",FilePath);
	fclose(SMDFile);
}

bool cObjTitanLottery::ExistNumber(BYTE Number)
{
	for(int i=0;i<MAX_TITANLOTTERY_NUMS;i++)
	{
		if(this->Nums[i] == Number)
			return true;
	}
	return false;
}

void cObjTitanLottery__StartEvent(void * pvoid)
{
	ObjTitanLottery.Start = true;

	int Cnt=0;
	
	memset(&ObjTitanLottery.Nums,0,sizeof(ObjTitanLottery.Nums));

	srand(time(0));

	while(Cnt < MAX_TITANLOTTERY_NUMS)
	{
		BYTE Num = rand()%ObjTitanLottery.MaxNumber + 1;
		if(ObjTitanLottery.ExistNumber(Num) == false)
		{
			ObjTitanLottery.Nums[Cnt] = Num;
			Cnt++;
		}
		Sleep(100);
	}

	//Array order for send to LinkServer

	for(int i=0;i<MAX_TITANLOTTERY_NUMS;i++)
	{
		for (int j=0;j<MAX_TITANLOTTERY_NUMS-1;j++)
		{
			if (ObjTitanLottery.Nums[j] > ObjTitanLottery.Nums[j+1])
			{
				BYTE temp = ObjTitanLottery.Nums[j];
				ObjTitanLottery.Nums[j] = ObjTitanLottery.Nums[j+1];
				ObjTitanLottery.Nums[j+1] = temp;
			}
		}
	}
	
	char sTmp[100] = {0};
	wsprintf(sTmp,"[Lottery] Nums: %d, %d, %d, %d",ObjTitanLottery.Nums[0],ObjTitanLottery.Nums[1],ObjTitanLottery.Nums[2],ObjTitanLottery.Nums[3]);
	AllSendServerMsg(sTmp);

	SDHP_SENDLOTTERYINFO result = {0};
	PHeadSubSetB((LPBYTE)&result, SCFExDB_GSSend_SendTitanLotteryInfo,0, sizeof(result));
	memcpy(&result.info,&ObjTitanLottery.Info,sizeof(result.info));
	memcpy(&result.Nums,&ObjTitanLottery.Nums,sizeof(result.Nums));
	memcpy(&result.ExReward,&ObjTitanLottery.ExReward,sizeof(result.ExReward));
	cDBSMng.Send((char *)&result,sizeof(result));

	ObjTitanLottery.Start = false;
}

void cObjTitanLottery::StartEvent()
{	
	if(this->Enabled == 1 && this->Start == false)
	{
		_beginthread(cObjTitanLottery__StartEvent, 0, NULL  );
	}
}

void cObjTitanLottery::SendBuyTicket(int aIndex, BYTE Num1, BYTE Num2, BYTE Num3, BYTE Num4)
{
	char sTmp[100]={0};

	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	SDHP_SENDLOTTERYBUYTICKET result = {0};
	PHeadSubSetB((LPBYTE)&result, SCFExDB_GSSend_SendTitanLotteryInfo,1, sizeof(result));
	result.Nums[0] = Num1;
	result.Nums[1] = Num2;
	result.Nums[2] = Num3;
	result.Nums[3] = Num4;

	for(int i=0;i<MAX_TITANLOTTERY_NUMS;i++)
	{
		for (int j=0;j<MAX_TITANLOTTERY_NUMS;j++)
		{
			if(i!=j)
			{
				if (result.Nums[j] == result.Nums[i])
				{
					GCServerMsgStringSend("You cant choose the same number more than once." ,aIndex, 0x01);
					return;
				}
			}
		}
	}

	if(this->TypeCost == 0)
	{
		if(this->Cost <= gObj[aIndex].Money)
		{
			gObj[aIndex].Money -= this->Cost;
			::GCMoneySend(aIndex, gObj[aIndex].Money);
		}
	}else if(this->TypeCost == 1)
	{
		if(this->Cost <= gObj[aIndex].VipMoney)
		{
			gObj[aIndex].VipMoney -= this->Cost;
			gObj[aIndex].AccountExtraInfoModified = 1;
			wsprintf(sTmp,"VipMoney = %d",gObj[aIndex].VipMoney);
			GCServerMsgStringSend(sTmp,aIndex, 1);
		}
	}
	
	//Array order for send to LinkServer
	for(int i=0;i<MAX_TITANLOTTERY_NUMS;i++)
	{
		for (int j=0;j<MAX_TITANLOTTERY_NUMS-1;j++)
		{
			if (result.Nums[j] > result.Nums[j+1])
			{
				BYTE temp = result.Nums[j];
				result.Nums[j] = result.Nums[j+1];
				result.Nums[j+1] = temp;
			}
		}
	}
	
	strcpy(result.AccountID,gObj[aIndex].AccountID);
	strcpy(result.Name,gObj[aIndex].Name);

	result.ToReward = ((this->Taxes * this->Cost) / 100.0f);
	if(result.ToReward <= 0)
		result.ToReward = 1;
		
	gObj[aIndex].LotteryCnt++;
	
	cDBSMng.Send((char *)&result,sizeof(result));
		
	wsprintf(sTmp,"You cant buy more than %d tickets",this->MaxTicketBuy);

	GCServerMsgStringSend("Ticket buyed." ,aIndex, 0x01);
}

void cObjTitanLottery::BuyTicket(int aIndex, BYTE Num1, BYTE Num2, BYTE Num3, BYTE Num4)
{
	char sTmp[100]={0};
	wsprintf(sTmp,"Number cant be 0");
	
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	if(this->Enabled == FALSE)
	{
		GCServerMsgStringSend("Lottery Event is dissabled" ,aIndex, 0x01);
		return;
	}
	if(Num1 <= 0)
	{
		GCServerMsgStringSend(sTmp ,aIndex, 0x01);
		return;
	}
	if(Num2 <= 0)
	{
		GCServerMsgStringSend(sTmp ,aIndex, 0x01);
		return;
	}
	if(Num3 <= 0)
	{
		GCServerMsgStringSend(sTmp ,aIndex, 0x01);
		return;
	}
	if(Num4 <= 0)
	{
		GCServerMsgStringSend(sTmp ,aIndex, 0x01);
		return;
	}
	if(this->TypeCost == 0)
	{
		if(this->Cost > gObj[aIndex].Money)
		{
			wsprintf(sTmp,lMsg.Get(MSGGET(14, 160)),this->Cost);
			GCServerMsgStringSend(sTmp ,aIndex, 0x01);
			return;
		}
	}else if(this->TypeCost == 1)
	{
		if(this->Cost > gObj[aIndex].VipMoney)
		{
			wsprintf(sTmp,lMsg.Get(MSGGET(14, 161)),this->Cost);
			GCServerMsgStringSend(sTmp ,aIndex, 0x01);
			return;
		}
	}
	
	if(gObj[aIndex].LotteryCnt < this->MaxTicketBuy || this->MaxTicketBuy == 0)
	{
		this->SendBuyTicket(aIndex,Num1,Num2,Num3,Num4);
	}else
	{
		wsprintf(sTmp,"You cant buy more than %d tickets",this->MaxTicketBuy);
		GCServerMsgStringSend(sTmp ,aIndex, 0x01);
	}	
}

void cObjTitanLottery::ShowInfo(SDHP_RECVLOTTERYINFO * aRecv)
{
	char sTmp[256]={0};
	
	if(this->Enabled == FALSE)
	{
		return;
	}
	
	if(aRecv->WinnersCount > 0)
	{
		char sTmp2[256]={0};
		char sTmp3[50]={0};
		
		if(this->Info.Type == 0)
			wsprintf(sTmp2,"Zen");
		else if(this->Info.Type == 1)
			wsprintf(sTmp2,"VIP Money");
			
		if(this->Info.AdditionalReward <= aRecv->WinnersCount)
			wsprintf(sTmp3,"+ Extras");
			
		wsprintf(sTmp,"[Lottery] Winners:%d - Reward for each one:%d of %s %s",aRecv->WinnersCount,aRecv->Reward, sTmp2, sTmp3);
		AllSendServerMsg(sTmp);	
	}else
	{
		AllSendServerMsg("[Lottery] There is no winners");
	}
	
	wsprintf(sTmp,"[Lottery] Next Reward:%d (Max for each player %d)",aRecv->NextReward,this->Info.MaxReward);
	AllSendServerMsg(sTmp);
}

#endif