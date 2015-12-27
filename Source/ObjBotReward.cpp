#include "stdafx.h"
#include "GameServer.h"
#include "LogProc.h"
#include "GameMain.h"
#include "User.h"
#include "SCFExDBProtocol.h"
#include "SCFExDB_Defines.h"
#include "ObjBotReward.h"
#include "ObjUseSkill.h"
#include "DevilSquare.h"
#include "BloodCastle.h"
#include "ChaosCastle.h"
#include "..\common\winutil.h"
#include "..\include\readscript.h"
#include "..\common\SetItemOption.h"
#include "LogToFile.h"

#if (PACK_EDITION>=3)

ObjBotReward BotReward;

CLogToFile BOTREWARD_LOG("ITEM_BOTREWARD_LOG", ".\\SCFBOTREWARD_LOG", 1);	// line : 68

void ObjBotReward::Read(char * FilePath)
{
	this->Enabled = FALSE;

	int Token;
	SMDFile = fopen(FilePath, "r");

	if ( SMDFile == NULL )
	{
		MsgBox("BotReward data load error %s", FilePath);
		return;
	}

	while ( true )
	{
		int iType = GetToken();
		
		if ( iType == 1 )
		{
			Token = GetToken();
			if ( strcmp("end", TokenString) == 0 )
			{
				break;
			}

			this->bot.Class = TokenNumber;

			Token = GetToken();
			strncpy(this->bot.Name,TokenString,sizeof(this->bot.Name));

			Token = GetToken();
			this->bot.Map = TokenNumber;

			Token = GetToken();
			this->bot.X = TokenNumber;

			Token = GetToken();
			this->bot.Y = TokenNumber;

			Token = GetToken();
			this->bot.Dir = TokenNumber;

			this->bot.Enabled = true;
		}
		
		GetToken();
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

				if(this->bot.Enabled == false)
				{	
					MsgBox("BotReward error: BotPet doesnt exist");
					return;
				}

				int Slot = TokenNumber;
				
				if(Slot < 0 || Slot > 8)
				{	
					MsgBox("BotReward error: Min Slot 0 ; Max Slot 8");
					return;
				}

				Token = GetToken();
				int iType = TokenNumber;

				Token = GetToken();
				int iIndex = TokenNumber;

				this->bot.body[Slot].num = ITEMGET(iType,iIndex);

				Token = GetToken();
				this->bot.body[Slot].level = TokenNumber;

				Token = GetToken();
				this->bot.body[Slot].opt = TokenNumber;

				this->bot.body[Slot].Enabled = true;
			}
		}
		break;
	}

	this->Enabled = true;
	LogAddTD("[BotReward] - %s file is Loaded",FilePath);
	fclose(SMDFile);
}

void ObjBotReward::MakeBot()
{
	if(this->Enabled == true)
	{
		if(this->bot.Enabled == true)
		{
			int result = gObjAddCallMon();

			if(result >= 0)
			{
				this->bot.index = result;
				gObj[result].m_PosNum = (WORD)-1;
				gObj[result].X = this->bot.X;
				gObj[result].Y = this->bot.Y;
				gObj[result].m_OldX = this->bot.X;
				gObj[result].m_OldY = this->bot.Y;
				gObj[result].TX = this->bot.X;
				gObj[result].TY = this->bot.Y;
				gObj[result].MTX = this->bot.X;
				gObj[result].MTY = this->bot.Y;
				gObj[result].Dir = this->bot.Dir;
				gObj[result].MapNumber = this->bot.Map;
				gObj[result].Live = TRUE;
				gObj[result].PathCount = 0;
				gObj[result].IsBot = 9;
				gObjSetMonster(result,this->bot.Class,"PetBot");

				gObj[result].ChangeUP = this->bot.Class & 0x07;	// Set Second Type of Character
				gObj[result].Class = this->bot.Class;
				gObj[result].Level = 400;
				gObj[result].Life = 1;
				gObj[result].MaxLife = 2;
				gObj[result].Mana = 1;
				gObj[result].MaxMana = 2;
				gObj[result].Experience = 0;
				gObj[result].DbClass = this->bot.Class;
				
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

				strncpy(gObj[result].Name,this->bot.Name,sizeof(gObj[result].Name));


				for(int i=0;i<9;i++)
				{
					if(this->bot.body[i].num >= 0 && this->bot.body[i].Enabled == true)
					{
						CItem item;
						item.m_Level = this->bot.body[i].level;
						item.m_SkillOption = 0;
						item.m_LuckOption = 1;
						item.m_Z28Option = this->bot.body[i].opt;
						item.m_Durability = 255.0f;
						item.m_JewelOfHarmonyOption = 0;
						item.m_ItemOptionEx = 0;
						item.m_ItemSlot1 = 0;
						item.m_ItemSlot2 = 0;
						item.m_ItemSlot3 = 0;
						item.m_ItemSlot4 = 0;
						item.m_ItemSlot5 = 0;
						item.Convert(this->bot.body[i].num,item.m_SkillOption,item.m_LuckOption,item.m_Z28Option,0,0,item.m_ItemOptionEx,3);

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
				gObj[result].BotDefense = 0;
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

				ChatSend(&gObj[result],"I'll HELP ALL FIGHTERS!");
				GCActionSend(&gObj[result],AT_SALUTE1,result,result);
			}
		}
	}
}


int ObjBotReward::GetBotIndex(int aIndex)
{
	if(this->bot.Enabled == true)
	{
		if(this->bot.index == aIndex)
			return this->bot.index;
	}
	return -1;
}

BOOL ObjBotReward::TradeOpen(int index, int nindex)
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

	GetPlayerExtraInfo(index,SCFExDB_GSSend_GetBotReward);
	ChatTargetSend(&gObj[this->bot.index],"Wait, i will check i have some reward for you!",index);
	return 1;
}

#endif