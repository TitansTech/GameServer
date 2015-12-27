#include "stdafx.h"
#include "GameServer.h"
#include "LogProc.h"
#include "GameMain.h"
#include "User.h"
#include "SCFExDBProtocol.h"
#include "ObjBotBuffer.h"
#include "ObjUseSkill.h"
#include "DevilSquare.h"
#include "BloodCastle.h"
#include "ChaosCastle.h"
#include "..\common\winutil.h"
#include "..\include\readscript.h"
#include "..\common\SetItemOption.h"

#if (PACK_EDITION>=3)

ObjBotBuffer BotBuff;

void ObjBotBuffer::Read(char * FilePath)
{
	if(this->Enabled == true)
	{
		for(int botNum=0;botNum<MAX_BOTBUFFER;botNum++)
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
	for(int i=0;i<MAX_BOTBUFFER;i++)
	{
		this->bot[i].index = -1;
		this->bot[i].SkillCount = 0;
		this->bot[i].MaxLevel = ReadConfig.Max_Normal_Level;
		memset(&this->bot[i].skill,0,sizeof(this->bot[i].skill));
		this->bot[i].Enabled = false;
		for(int j=0;j<9;j++)
			this->bot[i].body[j].num =-1;
	}

	this->Enabled = FALSE;
	int Token;
	SMDFile = fopen(FilePath, "r");

	if ( SMDFile == NULL )
	{
		MsgBox("BotBuffer data load error %s", FilePath);
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
				if(BotNum < 0 || BotNum > MAX_BOTBUFFER-1)
				{
					MsgBox("BotBuffer error: BotPetIndex:%d out of range!", BotNum);
					return;
				}

				Token = GetToken();
				this->bot[BotNum].Class = TokenNumber;

				Token = GetToken();
				this->bot[BotNum].OnlyVip = TokenNumber;

				Token = GetToken();
				this->bot[BotNum].MaxLevel = TokenNumber;

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
				if(BotNum < 0 || BotNum > MAX_BOTBUFFER-1)
				{
					MsgBox("BotBuffer error: BotPetIndex:%d doesnt exist", BotNum);
					return;
				}
				if(this->bot[BotNum].Enabled == false)
				{	
					MsgBox("BotBuffer error: BotPetIndex:%d doesnt exist", BotNum);
					return;
				}
				Token = GetToken();
				int Slot = TokenNumber;
				
				if(Slot < 0 || Slot > 8)
				{	
					MsgBox("BotBuffer error: Min Slot 0 ; Max Slot 8");
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
			this->Enabled = TRUE;
			while(true)
			{
				Token = GetToken();
				if ( strcmp("end", TokenString) == 0 )
				{
					break;
				}

				int BotNum = TokenNumber;
				if(BotNum < 0 || BotNum > MAX_BOTBUFFER-1)
				{
					MsgBox("BotBuffer error: BotPetIndex:%d doesnt exist", BotNum);
					return;
				}
				if(this->bot[BotNum].Enabled == false)
				{	
					MsgBox("BotBuffer error: BotPetIndex:%d doesnt exist", BotNum);
					return;
				}
				if(this->bot[BotNum].SkillCount < 0 || this->bot[BotNum].SkillCount > MAX_BOTBUFFERSKILLS-1)
				{
					MsgBox("BotBuffer error: Skill Buff out of range (MAX BUFF PER BOT = %d)!", this->bot[BotNum].SkillCount);
					return;
				}

				Token = GetToken();
				this->bot[BotNum].skill[this->bot[BotNum].SkillCount].skill = TokenNumber;

				Token = GetToken();
				this->bot[BotNum].skill[this->bot[BotNum].SkillCount].time = TokenNumber;

				this->bot[BotNum].SkillCount++;
			}
		}
		break;
	}

	LogAddTD("[BotBuffer] - %s file is Loaded",FilePath);
	fclose(SMDFile);
}

void ObjBotBuffer::MakeBot()
{
	if(this->Enabled == true)
	{
		for(int botNum=0;botNum<MAX_BOTBUFFER;botNum++)
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
					gObj[result].IsBot = 3;
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

					ChatSend(&gObj[result],"I'll HELP ALL FIGHTERS!");
					GCActionSend(&gObj[result],AT_SALUTE1,result,result);
				}
			}
		}
	}
}


int ObjBotBuffer::GetBotIndex(int aIndex)
{
	for(int i=0;i<MAX_BOTBUFFER;i++)
	{
		if(this->bot[i].Enabled == true)
		{
			if(this->bot[i].index == aIndex)
				return i;
		}
	}
	return -1;
}

BOOL ObjBotBuffer::TradeOpen(int index, int nindex)
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

	if(gObj[index].Level > this->bot[number].MaxLevel)
	{
		ChatTargetSend(&gObj[this->bot[number].index],"You can go without my help!",index);
		return 0;
	}

	if(this->bot[number].OnlyVip == 1 && gObj[index].Vip == 0)
	{
		ChatTargetSend(&gObj[this->bot[number].index],"Im only work for VIPs!",index);
		return 0;
	}

	for(int x=0;x<this->bot[number].SkillCount;x++)
	{
		switch(this->bot[number].skill[x].skill)
		{
			case 16:
			{
				gObj[index].m_WizardSkillDefense = 10 + (gObj[index].Dexterity + gObj[index].AddDexterity) / ReadConfig.WizardMagicDefenseDiv1 + (gObj[index].Energy + gObj[index].AddEnergy) / ReadConfig.WizardMagicDefenseDiv2;
				if (gObj[index].m_WizardSkillDefense > ReadConfig.WizardMagicMaximumDefense)
					gObj[index].m_WizardSkillDefense = ReadConfig.WizardMagicMaximumDefense;

				gObj[index].m_WizardSkillDefenseTime = this->bot[number].skill[x].time;
				gObj[index].m_ViewSkillState |= 256;
				GCMagicAttackNumberSend(&gObj[this->bot[number].index],AT_SKILL_MAGICDEFENSE,index,1);
			}break;
			case AT_SKILL_DEFENSE:
			{
				gObj[index].m_SkillDefense = gObj[index].Level / 5 + 50;
				gObj[index].m_SkillDefenseTime = this->bot[number].skill[x].time;
				gObj[index].m_ViewSkillState |= 8;
				GCMagicAttackNumberSend(&gObj[this->bot[number].index],AT_SKILL_DEFENSE,index,1);
			}break;
			case AT_SKILL_ATTACK:
			{
				gObj[index].m_SkillDefense = gObj[index].Level / 3 + 45;
				gObj[index].m_SkillDefenseTime = this->bot[number].skill[x].time;
				gObj[index].m_ViewSkillState |= 4;
				GCMagicAttackNumberSend(&gObj[this->bot[number].index],AT_SKILL_ATTACK,index,1);
			}break;
			case 48:
			{
				gObj[index].AddLife -= gObj[index].m_SkillAddLife;
				if(gObj[index].AddLife <0)
					gObj[index].AddLife = 0;
				gObj[index].m_SkillAddLife = gObj[index].Level / 5 + 100;
				gObj[index].AddLife += gObj[index].m_SkillAddLife;
				gObj[index].m_SkillAddLifeTime = this->bot[number].skill[x].time;
				GCMagicAttackNumberSend(&gObj[this->bot[number].index],AT_SKILL_KNIGHTADDLIFE,index,1);
				gObj[index].m_ViewSkillState |= 16;
				GCReFillSend(gObj[index].m_Index,gObj[index].MaxLife + gObj[index].AddLife,0xFE,0,gObj[index].iMaxShield + gObj[index].iAddShield);
			}break;
			case 64:
			{
				if(gObj[index].SkillAddCriticalDamageTime <= 0)
				{
					int addcriticaldamagevalue = gObj[index].Level / 3 + 100;
					gObj[index].SkillAddCriticalDamage = addcriticaldamagevalue;
					gObj[index].SkillAddCriticalDamageTime = this->bot[number].skill[x].time;
					gObj[index].m_ViewSkillState |= 2048;
					GCMagicAttackNumberSend(&gObj[this->bot[number].index],AT_SKILL_ADD_CRITICALDAMAGE,index,1);
				}
			}break;
			case 217:
			{
				if(gObj[index].m_SkillReflect <= 0)
				{
					int skill_reflect = gObj[index].Level / 10 + 5;

					if(skill_reflect > 254)
						skill_reflect = 255;

					if(gObj[index].DamageReflect + skill_reflect > 254)
					{
						skill_reflect = 255 - gObj[index].DamageReflect;
					}
					gObj[index].m_SkillReflect = skill_reflect;
					gObj[index].DamageReflect += gObj[index].m_SkillReflect;
					gObj[index].m_SkillReflectTime = this->bot[number].skill[x].time;
					GCMagicAttackNumberSend(&gObj[this->bot[number].index],AT_SKILL_REFLECT,index,1);
					GCSkillInfoSend(&gObj[index],1,0x47);
				}
			}break;
			case 233:
			{
				if(gObj[index].m_SkillMagicCircle > 0)
				{
					gObj[index].m_MagicDamageMin -= gObj[index].m_SkillMagicCircle;
					gObj[index].m_SkillMagicCircle = 0;
				}

				gObj[index].m_SkillMagicCircle = gObj[index].m_MagicDamageMin * ReadConfig.MagicCircleMult;
				gObj[index].m_SkillMagicCircleTime = this->bot[number].skill[x].time;

				GCMagicAttackNumberSend(&gObj[this->bot[number].index],AT_SKILL_MAGIC_CIRCLE,index,1);
				GCSkillInfoSend(&gObj[index],1,0x52);
			}break;
		}
	}
	ChatTargetSend(&gObj[this->bot[number].index],"Now you are blessed!",index);
	return 1;
}

#endif