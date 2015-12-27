#include "stdafx.h"
#include "GameServer.h"
#include "LogProc.h"
#include "GameMain.h"
#include "User.h"
#include "SCFExDBProtocol.h"
#include "ObjBotVipShop.h"
#include "SCFVipShop.h"
#include "ObjUseSkill.h"
#include "DevilSquare.h"
#include "BloodCastle.h"
#include "ChaosCastle.h"
#include "..\common\winutil.h"
#include "..\include\readscript.h"
#include "..\common\SetItemOption.h"


#if (PACK_EDITION>=3)
#if (CRYSTAL_EDITION == 1)

ObjBotVipShop BotVipShop;

void ObjBotVipShop::Read(char * FilePath)
{
	if(this->Enabled == true)
	{
		for(int botNum=0;botNum<MAX_BOTVIPSHOP;botNum++)
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
	for(int i=0;i<MAX_BOTVIPSHOP;i++)
	{
		this->bot[i].index = -1;
		this->bot[i].Enabled = false;
		for(int j=0;j<9;j++)
			this->bot[i].body[j].num =-1;
	}

	this->Enabled = FALSE;
	int Token;
	SMDFile = fopen(FilePath, "r");

	if ( SMDFile == NULL )
	{
		MsgBox("BotVipShop data load error %s", FilePath);
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
				if(BotNum < 0 || BotNum > MAX_BOTVIPSHOP-1)
				{
					MsgBox("BotVipShop error: BotPetIndex:%d out of range!", BotNum);
					return;
				}

				Token = GetToken();
				this->bot[BotNum].Class = TokenNumber;

				Token = GetToken();
				this->bot[BotNum].OnlyVip = TokenNumber;

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

				this->bot[BotNum].Enabled = true;

				this->Enabled = true;
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
				if(BotNum < 0 || BotNum > MAX_BOTVIPSHOP-1)
				{
					MsgBox("BotVipShop error: BotPetIndex:%d doesnt exist", BotNum);
					return;
				}
				if(this->bot[BotNum].Enabled == false)
				{	
					MsgBox("BotVipShop error: BotPetIndex:%d doesnt exist", BotNum);
					return;
				}
				Token = GetToken();
				int Slot = TokenNumber;
				
				if(Slot < 0 || Slot > 8)
				{	
					MsgBox("BotVipShop error: Min Slot 0 ; Max Slot 8");
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
		break;
	}

	LogAddTD("[BotVipShop] - %s file is Loaded",FilePath);
	fclose(SMDFile);
}

void ObjBotVipShop::MakeBot()
{
	if(this->Enabled == true)
	{
		for(int botNum=0;botNum<MAX_BOTVIPSHOP;botNum++)
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
					gObj[result].IsBot = 6;
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

					ChatSend(&gObj[result],"GO HERE VIP PLAYERS!");
					GCActionSend(&gObj[result],AT_SALUTE1,result,result);
				}
			}
		}
	}
}


int ObjBotVipShop::GetBotIndex(int aIndex)
{
	for(int i=0;i<MAX_BOTVIPSHOP;i++)
	{
		if(this->bot[i].Enabled == true)
		{
			if(this->bot[i].index == aIndex)
				return i;
		}
	}
	return -1;
}

BOOL ObjBotVipShop::TradeOpen(int index, int nindex)
{
	if ( gObjIsConnectedGP(index) == FALSE )
	{
		LogAddC(2, "error-L3 [%s][%d]", __FILE__, __LINE__);
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
		ChatTargetSend(&gObj[this->bot[number].index],"Im only show my shop to VIPs!",index);
		return 0;
	}

	SVShop.WindowState(index,1);
	ChatTargetSend(&gObj[this->bot[number].index],"Check my shop!",index);
	return 1;
}
#endif
#endif