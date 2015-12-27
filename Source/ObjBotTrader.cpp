#include "stdafx.h"
#include "GameServer.h"
#include "LogProc.h"
#include "GameMain.h"
#include "User.h"
#include "SCFExDBProtocol.h"
#include "ObjBotTrader.h"
#include "..\common\winutil.h"
#include "..\include\readscript.h"
#include "..\common\SetItemOption.h"
#include "gObjMonster.h"
#include "ChaosBox.h"

#if (PACK_EDITION>=3)

ObjBotTrader BotTrader;

int ObjBotTrader::MixNumber(int index)
{
	for(int i=0;i<this->MixCount;i++)
	{
		if(this->Mix[i].Index == index)
			return i;
	}
	return -1;
}

void ObjBotTrader::Read(char * FilePath)
{
	if(this->Enabled == true)
	{
		for(int botNum=0;botNum<MAX_BOTTRADER;botNum++)
		{
			if(this->bot[botNum].Enabled == true)
			{
				int bIndex = this->bot[botNum].index;
				if(gObjIsConnected(bIndex) == TRUE)
				{
					gObjDel(bIndex);
				}
			}
		}
	}

	memset(&this->bot,0,sizeof(this->bot));
	this->Enabled = FALSE;
	int Token;
	SMDFile = fopen(FilePath, "r");
	this->MixCount = 0;

	if ( SMDFile == NULL )
	{
		MsgBox("BotTrader data load error %s", FilePath);
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
				if ( strcmp("end", TokenString) == 0 )
				{
					break;
				}

				int BotNum = TokenNumber;
				if(BotNum < 0 || BotNum > MAX_BOTTRADER-1)
				{
					MsgBox("BotTrader error: BotPetIndex:%d out of range!", BotNum);
					return;
				}

				Token = GetToken();
				this->bot[BotNum].Class = TokenNumber;

				Token = GetToken();
				strncpy(this->bot[BotNum].Name,TokenString,sizeof(this->bot[BotNum].Name));

				Token = GetToken();
				this->bot[BotNum].Map = TokenNumber;

				Token = GetToken();
				this->bot[BotNum].X = TokenNumber;

				Token = GetToken();
				this->bot[BotNum].Y = TokenNumber;

				Token = GetToken();
				this->bot[BotNum].Dir = TokenNumber;

				Token = GetToken();
				this->bot[BotNum].OnlyVip = TokenNumber;

				this->bot[BotNum].MixesCount = 0;

				this->bot[BotNum].Enabled = true;
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

				int BotNum = TokenNumber;
				if(BotNum < 0 || BotNum > MAX_BOTTRADER-1)
				{
					MsgBox("BotTrader error: BotPetIndex:%d doesnt exist", BotNum);
					return;
				}
				if(this->bot[BotNum].Enabled == false)
				{	
					MsgBox("BotTrader error: BotPetIndex:%d doesnt exist", BotNum);
					return;
				}
				Token = GetToken();
				int Slot = TokenNumber;
				
				if(Slot < 0 || Slot > 8)
				{	
					MsgBox("BotTrader error: Min Slot 0 ; Max Slot 8");
					return;
				}

				Token = GetToken();
				int iType = TokenNumber;

				Token = GetToken();
				int iIndex = TokenNumber;

				this->bot[BotNum].body[Slot].num = ITEMGET(iType,iIndex);

				Token = GetToken();
				this->bot[BotNum].body[Slot].level = TokenNumber;

				Token = GetToken();
				this->bot[BotNum].body[Slot].opt = TokenNumber;

				this->bot[BotNum].body[Slot].Enabled = true;
			}
		}
		
		iType = GetToken();
		iType = TokenNumber;		
		if ( iType == 3 )
		{
			while(true)
			{
				Token = GetToken();
				if ( strcmp("end", TokenString) == 0 )
				{
					break;
				}
				this->Mix[this->MixCount].Index = TokenNumber;

				Token = GetToken();
				strcpy(this->Mix[this->MixCount].Name,TokenString);

				Token = GetToken();
				this->Mix[this->MixCount].SuccessRate = TokenNumber;

				Token = GetToken();
				this->Mix[this->MixCount].Zen = TokenNumber;

				Token = GetToken();
				this->Mix[this->MixCount].VipMoney = TokenNumber;

				this->MixCount++;
			}
		}
		
		iType = GetToken();
		iType = TokenNumber;
		if ( iType == 4 )
		{
			while(true)
			{
				Token = GetToken();
				if ( strcmp("end", TokenString) == 0 )
				{
					break;
				}
				int mIndex = TokenNumber;

				int Num = this->MixNumber(mIndex);

				if (Num  == -1)
				{
					MsgBox("BotTrader data error on Need Items (INDEX:%d)", mIndex);
					return;
				}

				Token = GetToken();
				int iType = TokenNumber;

				Token = GetToken();
				int iIndex = TokenNumber;

				this->Mix[Num].i_Need[this->Mix[Num].i_NeedCount].Type = ITEMGET(iType,iIndex);

				this->Mix[Num].i_Need[this->Mix[Num].i_NeedCount].LevelMin = 0;

				Token = GetToken();
				this->Mix[Num].i_Need[this->Mix[Num].i_NeedCount].LevelMax = TokenNumber;

				this->Mix[Num].i_Need[this->Mix[Num].i_NeedCount].OptionMin = 0;

				Token = GetToken();
				this->Mix[Num].i_Need[this->Mix[Num].i_NeedCount].OptionMax = TokenNumber;

				Token = GetToken();
				this->Mix[Num].i_Need[this->Mix[Num].i_NeedCount].Luck = TokenNumber;

				Token = GetToken();
				this->Mix[Num].i_Need[this->Mix[Num].i_NeedCount].Skill = TokenNumber;

				Token = GetToken();
				this->Mix[Num].i_Need[this->Mix[Num].i_NeedCount].Exc = TokenNumber;

				Token = GetToken();
				this->Mix[Num].i_Need[this->Mix[Num].i_NeedCount].Dur = TokenNumber;

				this->Mix[Num].i_NeedCount++;
			}
		}
		
		iType = GetToken();
		iType = TokenNumber;
		if ( iType == 5 )
		{
			while(true)
			{
				Token = GetToken();
				if ( strcmp("end", TokenString) == 0 )
				{
					break;
				}
				int mIndex = TokenNumber;

				int Num = this->MixNumber(mIndex);

				if (Num  == -1)
				{
					MsgBox("TradeMix data error on Success Items (INDEX:%d)", mIndex);
					return;
				}

				Token = GetToken();
				int iType = TokenNumber;

				Token = GetToken();
				int iIndex = TokenNumber;

				this->Mix[Num].i_Success[this->Mix[Num].i_SuccessCount].Type = ITEMGET(iType,iIndex);

				Token = GetToken();
				this->Mix[Num].i_Success[this->Mix[Num].i_SuccessCount].LevelMin = TokenNumber;

				Token = GetToken();
				this->Mix[Num].i_Success[this->Mix[Num].i_SuccessCount].LevelMax = TokenNumber;

				Token = GetToken();
				this->Mix[Num].i_Success[this->Mix[Num].i_SuccessCount].OptionMin = TokenNumber;

				Token = GetToken();
				this->Mix[Num].i_Success[this->Mix[Num].i_SuccessCount].OptionMax = TokenNumber;

				Token = GetToken();
				this->Mix[Num].i_Success[this->Mix[Num].i_SuccessCount].Luck = TokenNumber;

				Token = GetToken();
				this->Mix[Num].i_Success[this->Mix[Num].i_SuccessCount].Skill = TokenNumber;

				Token = GetToken();
				this->Mix[Num].i_Success[this->Mix[Num].i_SuccessCount].Exc = TokenNumber;

				Token = GetToken();
				this->Mix[Num].i_Success[this->Mix[Num].i_SuccessCount].Dur = TokenNumber;

				this->Mix[Num].i_SuccessCount++;
			}
		}
		
		iType = GetToken();
		iType = TokenNumber;		
		if ( iType == 6 )
		{
			int Count = 0;
			while(true)
			{
				Token = GetToken();
				if ( strcmp("end", TokenString) == 0 )
				{
					break;
				}

				int BotNum = TokenNumber;
				if(BotNum < 0 || BotNum > MAX_BOTTRADER-1)
				{
					MsgBox("BotTrader error: BotPetIndex:%d doesnt exist", BotNum);
					return;
				}
				if(this->bot[BotNum].Enabled == false)
				{	
					MsgBox("BotTrader error: BotPetIndex:%d doesnt exist", BotNum);
					return;
				}

				Token = GetToken();
				int mIndex = TokenNumber;

				int Num = this->MixNumber(mIndex);

				if (Num  == -1)
				{
					MsgBox("TradeMix data error on Success Items (INDEX:%d)", mIndex);
					return;
				}
				this->bot[BotNum].MixesIndex[this->bot[BotNum].MixesCount] = Num;

				this->bot[BotNum].MixesCount++;
				this->Enabled = true;
			}
		}		
		break;
	}
			
	LogAddTD("[BotTrader] - %s file is Loaded",FilePath);
	fclose(SMDFile);
}

void ObjBotTrader::MakeBot()
{
	if(this->Enabled == true)
	{
		for(int botNum=0;botNum<MAX_BOTTRADER;botNum++)
		{
			if(this->bot[botNum].Enabled == true)
			{
				int result = gObjAddCallMon();

				if(result >= 0)
				{
					this->bot[botNum].index = result;
					gObj[result].m_PosNum = (WORD)-1;
					gObj[result].X = this->bot[botNum].X;
					gObj[result].Y = this->bot[botNum].Y;
					gObj[result].m_OldX = this->bot[botNum].X;
					gObj[result].m_OldY = this->bot[botNum].Y;
					gObj[result].TX = this->bot[botNum].X;
					gObj[result].TY = this->bot[botNum].Y;
					gObj[result].MTX = this->bot[botNum].X;
					gObj[result].MTY = this->bot[botNum].Y;
					gObj[result].Dir = this->bot[botNum].Dir;
					gObj[result].MapNumber = this->bot[botNum].Map;
					gObj[result].Live = TRUE;
					gObj[result].PathCount = 0;
					gObj[result].IsBot = 5;
					gObjSetMonster(result,this->bot[botNum].Class,"PetBot");

					gObj[result].ChangeUP = this->bot[botNum].Class & 0x07;	// Set Second Type of Character
					gObj[result].Class = this->bot[botNum].Class;
					gObj[result].Level = 400;
					gObj[result].Life = 1;
					gObj[result].MaxLife = 2;
					gObj[result].Mana = 1;
					gObj[result].MaxMana = 2;
					gObj[result].Experience = 0;
					gObj[result].DbClass = this->bot[botNum].Class;
					
					gObj[result].pInventory = new CItem[INVENTORY_NORMAL_SIZE];					
					gObj[result].Inventory1 = new CItem[INVENTORY_NORMAL_SIZE];
					gObj[result].InventoryMap1 = new BYTE[INVENTORY_NORMAL_SIZE];
					gObj[result].pInventoryMap = new BYTE[INVENTORY_NORMAL_SIZE];

					for (int i=0;i<INVENTORY_NORMAL_SIZE;i++)
					{
						gObj[result].pInventory[i].Clear();
						gObj[result].Inventory1[i].Clear();
					}

					memset(&gObj[result].pInventoryMap[0], (BYTE)-1, INVENTORY_NORMAL_SIZE);
					memset(&gObj[result].InventoryMap1[0], (BYTE)-1, INVENTORY_NORMAL_SIZE);

					strncpy(gObj[result].Name,this->bot[botNum].Name,sizeof(gObj[result].Name));


					for(int i=0;i<9;i++)
					{
						if(this->bot[botNum].body[i].num >= 0 && this->bot[botNum].body[i].Enabled == true)
						{
							CItem item;
							item.m_Level = this->bot[botNum].body[i].level;
							item.m_SkillOption = 0;
							item.m_LuckOption = 1;
							item.m_Z28Option = this->bot[botNum].body[i].opt;
							item.m_Durability = 255.0f;
							item.m_JewelOfHarmonyOption = 0;
							item.m_ItemOptionEx = 0;
							item.m_ItemSlot1 = 0;
							item.m_ItemSlot2 = 0;
							item.m_ItemSlot3 = 0;
							item.m_ItemSlot4 = 0;
							item.m_ItemSlot5 = 0;
							item.Convert(this->bot[botNum].body[i].num,item.m_SkillOption,item.m_LuckOption,item.m_Z28Option,0,0,item.m_ItemOptionEx,3);

							gObj[result].pInventory[i].m_SkillOption = item.m_SkillOption;
							gObj[result].pInventory[i].m_LuckOption = item.m_LuckOption;
							gObj[result].pInventory[i].m_Z28Option = item.m_Z28Option;
							gObj[result].pInventory[i].m_JewelOfHarmonyOption = item.m_JewelOfHarmonyOption;
							gObj[result].pInventory[i].m_ItemOptionEx = item.m_ItemOptionEx;

							item.m_Number = 0;
							
							gObjInventoryInsertItemPos(gObj[result].m_Index,item,i,0);
						}
					}
					
					gObj[result].Inventory1 = gObj[result].pInventory;
					gObj[result].InventoryMap1 = gObj[result].pInventoryMap;
					gObjMakePreviewCharSet(result);

					gObj[result].m_AttackType = 0;
					gObj[result].BotSkillAttack = 0;					

					gObj[result].m_Attribute = 100;
					gObj[result].TargetNumber = (WORD)-1;
					gObj[result].m_ActState.Emotion = 0;
					gObj[result].m_ActState.Attack = 0;
					gObj[result].m_ActState.EmotionCount = 0;
					gObj[result].PathCount = 0;
					gObj[result].BotPower = 0;
					gObj[result].BotDefense = this->bot[botNum].OnlyVip;
					gObj[result].BotLife = 1;
					gObj[result].BotMaxLife = 1;
					
					gObj[result].BotLvlUpDefense = 1;
					gObj[result].BotLvlUpPower = 1;
					gObj[result].BotLvlUpLife = 1;
					gObj[result].BotLvlUpMana = 1;
					gObj[result].BotLvlUpExp = 1;
					gObj[result].BotLvlUpMaxLevel = 1;

					gObj[result].m_MoveRange = 1;
					gObj[result].BotFollowMe = 0;
					gObj[result].NextExp = gLevelExperience[gObj[result].Level];

					GCRecallMonLife(gObj[result].m_RecallMon,gObj[result].MaxLife,gObj[result].Life);

					ChatSend(&gObj[result],"I'll TRADE YOU!");
					GCActionSend(&gObj[result],AT_SALUTE1,result,result);
				}
			}
		}
	}
}

BOOL ObjBotTrader::IsInTrade(int aIndex)
{
	int number = this->GetBotIndex(gObj[aIndex].TargetNumber);
	if(number != -1)
		return 1;
	else
		return 0;
}

int ObjBotTrader::GetBotIndex(int aIndex)
{
	for(int i=0;i<MAX_BOTTRADER;i++)
	{
		if(this->bot[i].Enabled == true)
		{
			if(this->bot[i].index == aIndex)
				return i;
		}
	}
	return -1;
}

void ObjBotTrader::TradeCancel(int aIndex)
{
	gObjTradeCancel(aIndex);
	CGTradeResult(aIndex,3);
}

void ObjBotTrader::TradeOk(int aIndex)
{
	int MixNum=-1;
	int MixItem=-1;

	int number = this->GetBotIndex(gObj[aIndex].TargetNumber);

	if(number == -1)
	{			
		gObj[aIndex].m_IfState.state = 0;
		gObj[aIndex].m_IfState.type = 0;
		gObj[aIndex].TargetShopNumber = -1;
		gObj[aIndex].pTransaction = 0;

		return;
	}
	this->TradeMix(aIndex,number);
}


BOOL ObjBotTrader::TradeOpen(int index, int nindex)
{
	if(gObjIsConnected(index) == 0)
	{
		return 0;
	}
	if(gObjIsConnected(nindex) == 0)
	{
		return 0;
	}
	int number = this->GetBotIndex(nindex);
	if(number == -1)
		return 0;
	char sbuf[512]={0};
	LPOBJ lpObj = &gObj[index];
	LPOBJ lpBot = &gObj[nindex];

	if(this->bot[number].OnlyVip == 1 && gObj[index].Vip == 0)
	{
		ChatTargetSend(&gObj[this->bot[number].index],"Im only work for VIPs!",index);
		LogAddTD("[BotTrader][Bot:%d](%s) Account is not VIP",number,gObj[index].AccountID);
		return 0;
	}

	if(this->Enabled == TRUE)
	{
		if ( lpObj->m_IfState.use > 0 )
		{
			return 0;
		}else
		{	
			for(int n = 0; n < TRADE_BOX_SIZE; n++)
			{
				lpObj->Trade[n].Clear();
			}
			memset(lpObj->TradeMap, (BYTE)-1, TRADE_BOX_SIZE );

			gObjInventoryTrans(lpObj->m_Index);
			GCTradeResponseSend(true, lpObj->m_Index, lpBot->Name, 400, 0);
			GCTradeOkButtonSend(lpObj->m_Index, 1);
			lpObj->m_IfState.state = 1;
			lpObj->m_IfState.use = 1;
			lpObj->m_IfState.type = 1;
			lpObj->TradeMoney = 0;
			lpObj->TargetNumber =lpBot->m_Index;
			lpObj->pTransaction = 1;

			char wbuf[1024]={0};
			wsprintf(wbuf,"[BotTrader] (%s)(%s) OPEN",gObj[index].AccountID,gObj[index].Name);
			LogAddTD(wbuf);
			ChatTargetSend(&gObj[this->bot[number].index],"Im Ready!",index);
			
			//if(this->bot[number].PCPoint > 0)
			//{
			//	wsprintf(sbuf,"I Need %d VIPMoney!",this->bot[number].VipMoney );
			//	GCServerMsgStringSend(sbuf,index, 0x01);
			//}				
		}
	}
	return 1;
}

int ObjBotTrader::ItemCount(int aIndex)
{
	int Count=0;
	for(int n = 0; n < TRADE_BOX_SIZE; n++)
	{
		if(gObj[aIndex].Trade[n].IsItem() == 1)
		{
			Count++;
		}
	}
	return Count;
}

bool ObjBotTrader::SearchSameIDNumber(int mIndex, int To)
{
	if(To == 0)
		return true;
	for(int in=0;in<To;in++)
	{
		if(this->Mix[mIndex].i_Need[in].Type == this->Mix[mIndex].i_Need[To].Type)
		{
			return false;
		}
	}
	return true;
}

BYTE ObjBotTrader::TradeMix(int aIndex,int BotNum)
{
	if(gObjIsConnected(aIndex) == 0)
	{
		return 0;
	}

	//PMSG_CHAOSMIXRESULT pMsg;
	//PHeadSetB((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	//pMsg.Result = CB_ERROR;

	int mIndex = -1;
	
	for(int i=0;i<this->bot[BotNum].MixesCount;i++)
	{
		mIndex = this->bot[BotNum].MixesIndex[i];
		if(this->Mix[mIndex].i_NeedCount == this->ItemCount(aIndex))
		{
			int found=0;
			for(int in=0;in<this->Mix[mIndex].i_NeedCount;in++)
			{
				if(this->SearchSameIDNumber(mIndex,in) == true)
				{
					for(int n = 0; n < TRADE_BOX_SIZE; n++)
					{
						if(gObj[aIndex].Trade[n].IsItem() == 1)
						{
							if(this->Mix[mIndex].i_Need[in].Type == gObj[aIndex].Trade[n].m_Type)
							{
								if(gObj[aIndex].Trade[n].m_Level >= this->Mix[mIndex].i_Need[in].LevelMax && gObj[aIndex].Trade[n].m_LuckOption >= this->Mix[mIndex].i_Need[in].Luck && gObj[aIndex].Trade[n].m_Z28Option >= this->Mix[mIndex].i_Need[in].OptionMax && gObj[aIndex].Trade[n].m_SkillOption >= this->Mix[mIndex].i_Need[in].Skill && gObj[aIndex].Trade[n].m_NewOption >= this->Mix[mIndex].i_Need[in].Exc && gObj[aIndex].Trade[n].m_Durability >= this->Mix[mIndex].i_Need[in].Dur)
								{
									found++;
								}
							}
						}
					}
				}
			}
			if(found == this->Mix[mIndex].i_NeedCount)
			{
				goto OK;
			}
		}
	}
	ChatTargetSend(&gObj[this->bot[BotNum].index],"I dont need those items!",aIndex);
	goto Cancel;
OK:
	char sbuf[512] = {0};
	if(mIndex >= 0)
	{
		if(this->Mix[mIndex].VipMoney > 0)
		{
			if((gObj[aIndex].VipMoney - this->Mix[mIndex].VipMoney) < 0)
			{
				wsprintf(sbuf,"I Need %d VIPMoney!",this->Mix[mIndex].VipMoney );
				ChatTargetSend(&gObj[this->bot[BotNum].index],sbuf,aIndex);
				LogAddTD("[BotTrader][Bot:%d](%s) Insuficent VIPMoney",BotNum,gObj[aIndex].AccountID);
				goto Cancel;
			}
		}

		if(this->Mix[mIndex].Zen > 0)
		{
			if(gObj[aIndex].TradeMoney == this->Mix[mIndex].Zen)
			{
				wsprintf(sbuf,"I Need %d Zen!",this->Mix[mIndex].Zen );
				ChatTargetSend(&gObj[this->bot[BotNum].index],sbuf,aIndex);
				LogAddTD("[BotTrader][Bot:%d](%s) Insuficent Zen",BotNum,gObj[aIndex].AccountID);
				//pMsg.Result = CB_NOT_ENOUGH_ZEN;
				goto Cancel;
			}
		}
		
		srand(static_cast<int>(time(NULL)));
		int random = rand()%100;

		if(random <= this->Mix[mIndex].SuccessRate)
		{
			//SUCCESS
			srand(static_cast<int>(time(NULL)));
			int rItem = rand()%this->Mix[mIndex].i_SuccessCount;
			int Level=0;
			int Opt=0;
			int Exc=0;
			int Luck=0;
			int Skill=0;

			if(this->Mix[mIndex].i_Success[rItem].LevelMax == this->Mix[mIndex].i_Success[rItem].LevelMin)
			{
				Level=this->Mix[mIndex].i_Success[rItem].LevelMax;
			}else
			{
				srand(static_cast<int>(time(NULL)));
				Level = rand()%(this->Mix[mIndex].i_Success[rItem].LevelMax - this->Mix[mIndex].i_Success[rItem].LevelMin) + this->Mix[mIndex].i_Success[rItem].LevelMin;
			}
			if(this->Mix[mIndex].i_Success[rItem].OptionMax == this->Mix[mIndex].i_Success[rItem].OptionMin)
			{
				Opt=this->Mix[mIndex].i_Success[rItem].OptionMax;
			}else
			{
				srand(static_cast<int>(time(NULL)));
				Opt = rand()%(this->Mix[mIndex].i_Success[rItem].OptionMax - this->Mix[mIndex].i_Success[rItem].OptionMin) + this->Mix[mIndex].i_Success[rItem].OptionMin;
			}
			if(this->Mix[mIndex].i_Success[rItem].Exc > 0)
			{
				Exc=BoxExcOptions(this->Mix[mIndex].i_Success[rItem].Exc);
			}
			if(this->Mix[mIndex].i_Success[rItem].Luck == 0)
			{
				Luck = 0;
			}else
			{
				srand(static_cast<int>(time(NULL)));
				BYTE lRnd = rand()%100;
				if(lRnd <= this->Mix[mIndex].i_Success[rItem].Luck)
				{
					Luck=1;
				}else
				{
					Luck=0;
				}
			}
			if(this->Mix[mIndex].i_Success[rItem].Skill == 0)
			{
				Skill = 0;
			}else
			{
				srand(static_cast<int>(time(NULL)));
				BYTE sRnd = rand()%100;
				if(sRnd <= this->Mix[mIndex].i_Success[rItem].Skill)
				{
					Skill=1;
				}else
				{
					Skill=0;
				}
			}

			//CItem item;
			//item.m_Level = Level;
			//item.m_SkillOption = Skill;
			//item.m_LuckOption = Luck;
			//item.m_Z28Option = Opt;
			//item.m_Durability = 255.0f;
			//item.m_JewelOfHarmonyOption = 0;
			//item.m_ItemOptionEx = 0;
			//item.m_ItemSlot1 = 0;
			//item.m_ItemSlot2 = 0;
			//item.m_ItemSlot3 = 0;
			//item.m_ItemSlot4 = 0;
			//item.m_ItemSlot5 = 0;
			//item.Convert(this->Mix[mIndex].i_Success[rItem].Type,item.m_SkillOption,item.m_LuckOption,item.m_Z28Option,0,0,item.m_ItemOptionEx,3);

			//item.m_NewOption = Exc;
			//item.m_Number = 0;

			//int a = gObjInventoryInsertItem(aIndex,item);
			//if(a == 255)
			//{
			//	ChatTargetSend(&gObj[this->bot[BotNum].index],"Insuficent space in inventory!",aIndex);
			//	LogAddTD("[BotTrader](%s)(%s) Doesnt have space on inventory",gObj[aIndex].AccountID,gObj[aIndex].Name);
			//	//pMsg.Result = CB_TOO_MANY_ITEMS;
			//	goto Cancel;
			//}
			LPOBJ lpObj = &gObj[aIndex];
			BYTE TempInventoryMap[BASIC_INVENTORY_SIZE];
			memcpy(TempInventoryMap,lpObj->pInventoryMap,BASIC_INVENTORY_SIZE);	//64
				
			if(gObjTempInventoryInsertItem(lpObj,this->Mix[mIndex].i_Success[rItem].Type,TempInventoryMap) == 255)
			{
				ChatTargetSend(&gObj[this->bot[BotNum].index],"Insuficent space in inventory!",aIndex);
				LogAddTD("[BotTrader](%s)(%s) Doesnt have space on inventory",gObj[aIndex].AccountID,gObj[aIndex].Name);
				goto Cancel;
			}
			else
			{
				ChatTargetSend(&gObj[this->bot[BotNum].index],"Combination Suceeded!",aIndex);
				ItemSerialCreateSend(lpObj->m_Index, 235, 0, 0, this->Mix[mIndex].i_Success[rItem].Type,Level, 255, Skill, Luck, Opt, -1, Exc, 0);
			}
		} else {
			ChatTargetSend(&gObj[this->bot[BotNum].index],"Combination Failed!",aIndex);
		}

		if(this->Mix[mIndex].VipMoney > 0)
		{
			gObj[aIndex].VipMoney -= this->Mix[mIndex].VipMoney;
			gObj[aIndex].AccountExtraInfoModified = 1;
		}
		gObj[aIndex].Money -= this->Mix[mIndex].Zen;

		gObjInventoryCommit(aIndex);
		gObjMakePreviewCharSet(aIndex);
		GJSetCharacterInfo(&gObj[aIndex],aIndex,0);
		gObjItemTextSave(&gObj[aIndex]);
		gObjStatTextSave(&gObj[aIndex]);
		gObjSavePetItemInfo(aIndex, 0);
		gObj[aIndex].TargetNumber = -1;
		gObj[aIndex].m_IfState.use = 0;
		gObj[aIndex].TradeOk = 0;
		gObj[aIndex].TradeMoney = 0;
		GCMoneySend(aIndex,gObj[aIndex].Money);
		CGTradeResult(aIndex,1);
		GCItemListSend(aIndex);
		for(int n = 0; n < TRADE_BOX_SIZE; n++)
		{
			gObj[aIndex].Trade[n].Clear();
		}
		gObjNotifyUpdateUnionV1(&gObj[aIndex]);
		gObjNotifyUpdateUnionV2(&gObj[aIndex]);
		
		//ChatTargetSend(&gObj[this->bot[BotNum].index],"Mix Success!",aIndex);
		LogAddTD("[BotTrader](%s)(%s) %s Completed [%d/%d]",
			gObj[aIndex].AccountID,gObj[aIndex].Name,
			this->Mix[mIndex].Name,
			random, this->Mix[mIndex].SuccessRate
		);
		//pMsg.Result = CB_SUCCESS;
		//DataSend(aIndex, (BYTE*)&pMsg, pMsg.h.size);
		return 1;
	}

	ChatTargetSend(&gObj[this->bot[BotNum].index],"Mix Fail!",aIndex);
	LogAddTD("[BotTrader](%s)(%s) %s Fail [%d]",
		gObj[aIndex].AccountID,gObj[aIndex].Name,
		this->Mix[mIndex].Name,
		this->Mix[mIndex].SuccessRate
	);

Cancel:
	this->TradeCancel(aIndex);
	//DataSend(aIndex, (BYTE*)&pMsg, pMsg.h.size);
	return 0;
}

#endif