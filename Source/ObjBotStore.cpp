#include "stdafx.h"
#include "GameServer.h"
#include "LogProc.h"
#include "GameMain.h"
#include "User.h"
#include "SCFExDBProtocol.h"
#include "ObjBotStore.h"
#include "..\common\winutil.h"
#include "..\include\readscript.h"
#include "..\common\SetItemOption.h"

#if (PACK_EDITION>=3)

ObjBotStore BotStore;

void ObjBotStore::Read(char * FilePath)
{
	if(this->Enabled == true)
	{
		for(int botNum=0;botNum<MAX_BOTSTORE;botNum++)
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
	for(int i=0;i<MAX_BOTSTORE;i++)
	{
		this->bot[i].index = -1;
		this->bot[i].Enabled = false;
		this->bot[i].ItemCount=0;
		for(int j=0;j<9;j++)
			this->bot[i].body[j].num =-1;
	}

	this->Enabled = FALSE;
	int Token;
	SMDFile = fopen(FilePath, "r");

	if ( SMDFile == NULL )
	{
		MsgBox("BotStore data load error %s", FilePath);
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
				if(BotNum < 0 || BotNum > MAX_BOTSTORE-1)
				{
					MsgBox("BotStore error: BotPetIndex:%d out of range!", BotNum);
					return;
				}

				Token = GetToken();
				this->bot[BotNum].Class = TokenNumber;

				Token = GetToken();
				this->bot[BotNum].OnlyVip = TokenNumber;

				Token = GetToken();
				this->bot[BotNum].UseVipMoney = TokenNumber;

				Token = GetToken();
				strncpy(this->bot[BotNum].Name,TokenString,sizeof(this->bot[BotNum].Name));

				Token = GetToken();
				strncpy(this->bot[BotNum].StoreName,TokenString,sizeof(this->bot[BotNum].StoreName));

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
				if(BotNum < 0 || BotNum > MAX_BOTSTORE-1)
				{
					MsgBox("BotStore error: BotPetIndex:%d doesnt exist", BotNum);
					return;
				}
				if(this->bot[BotNum].Enabled == false)
				{	
					MsgBox("BotStore error: BotPetIndex:%d doesnt exist", BotNum);
					return;
				}
				Token = GetToken();
				int Slot = TokenNumber;
				
				if(Slot < 0 || Slot > 8)
				{	
					MsgBox("BotStore error: Min Slot 0 ; Max Slot 8");
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
				if(BotNum < 0 || BotNum > MAX_BOTSTORE-1)
				{
					MsgBox("BotStore error: BotPetIndex:%d doesnt exist", BotNum);
					return;
				}
				if(this->bot[BotNum].Enabled == false)
				{	
					MsgBox("BotStore error: BotPetIndex:%d doesnt exist", BotNum);
					return;
				}
				int Num = this->bot[BotNum].ItemCount;

				Token = GetToken();
				this->bot[BotNum].storeItem[Num].Value = TokenNumber;

				Token = GetToken();
				int iType = TokenNumber;

				Token = GetToken();
				int iIndex = TokenNumber;

				this->bot[BotNum].storeItem[Num].num = ITEMGET(iType,iIndex);

				Token = GetToken();
				this->bot[BotNum].storeItem[Num].Level = TokenNumber;

				Token = GetToken();
				this->bot[BotNum].storeItem[Num].Opt = TokenNumber;

				Token = GetToken();
				this->bot[BotNum].storeItem[Num].Luck = TokenNumber;

				Token = GetToken();
				this->bot[BotNum].storeItem[Num].Skill = TokenNumber;

				Token = GetToken();
				this->bot[BotNum].storeItem[Num].Dur = TokenNumber;

				Token = GetToken();
				this->bot[BotNum].storeItem[Num].Exc = TokenNumber;

				Token = GetToken();
				this->bot[BotNum].storeItem[Num].Anc = TokenNumber;

				for(int i=0;i<5;i++)
				{
					Token = GetToken();
					this->bot[BotNum].storeItem[Num].Sock[i] = TokenNumber;
				}

				this->bot[BotNum].ItemCount++;
			}
		}
		
		break;
	}
			
	LogAddTD("[BotStore] - %s file is Loaded",FilePath);
	fclose(SMDFile);
}




BYTE gObjTempPShopRectCheck(BYTE * TempMap, int sx, int sy, int width, int height)
{
	int x,y,blank = 0;

	if(sx + width > 12)
	{
		return -1;
	}
	if(sy + height > 8)
	{
		return -2;
	}

	for(y = 0; y < height; y++)
	{
		for(x = 0; x < width; x++)
		{
			if(*(BYTE*)(TempMap + (sy+y)*8 + (sx+x))!= 255)
			{
				blank += 1;
				return -1;
			}
		}
	}
	if(blank == 0)
	{
		return sx+sy*8+INVETORY_WEAR_SIZE;
	}
	return -1;
}



BYTE ObjBotStore::CheckSpace(LPOBJ lpObj, int type, BYTE * TempMap)
{
	int w,h,iwidth,iheight;
	BYTE blank = 0;

	iwidth=ItemAttribute[type].Width  ;
	iheight=ItemAttribute[type].Height  ;

	for(h = 0; h < 4; h++)
	{
		for(w = 0; w < 8; w++)
		{
			if(*(BYTE*)(TempMap + h * 8 + w) == 255)
			{
				blank = gObjTempPShopRectCheck(TempMap,w,h,iwidth,iheight);

				if(blank == 254)
				{
					goto GOTO_EndFunc;
				}
				if(blank != 255)
				{
					gObjTempInventoryItemBoxSet(TempMap,blank,iwidth,iheight,type);
					return blank;
				}
			}
		}
	}
GOTO_EndFunc:
	return -1;
}

void ObjBotStore::AddItem(int bIndex,int botNum)
{
	for(int i=0;i<this->bot[botNum].ItemCount;i++)
	{
		int blank = this->CheckSpace(&gObj[bIndex],this->bot[botNum].storeItem[i].num,&gObj[bIndex].pInventoryMap[ReadConfig.MAIN_INVENTORY_SIZE(bIndex,false)]);
		if(blank != 255)
		{
			if(ReadConfig.S6E2 == FALSE)
			{
				blank += ReadConfig.MAIN_INVENTORY_SIZE(bIndex,false) - INVETORY_WEAR_SIZE;
			}
			else
			{
				blank += ReadConfig.MAIN_INVENTORY_SIZE(bIndex,true) - INVETORY_WEAR_SIZE;
			}
			CItem item;
			item.m_Level = this->bot[botNum].storeItem[i].Level;
			item.m_SkillOption = this->bot[botNum].storeItem[i].Skill;
			item.m_LuckOption = this->bot[botNum].storeItem[i].Luck;
			item.m_Z28Option = this->bot[botNum].storeItem[i].Opt;
			item.m_Durability = this->bot[botNum].storeItem[i].Dur;
			item.m_JewelOfHarmonyOption = 0;
			item.m_NewOption = this->bot[botNum].storeItem[i].Exc;
			item.m_ItemOptionEx = 0;
			item.m_SetOption = this->bot[botNum].storeItem[i].Anc;
			item.m_ItemSlot1 = this->bot[botNum].storeItem[i].Sock[0];
			item.m_ItemSlot2 = this->bot[botNum].storeItem[i].Sock[1];
			item.m_ItemSlot3 = this->bot[botNum].storeItem[i].Sock[2];
			item.m_ItemSlot4 = this->bot[botNum].storeItem[i].Sock[3];
			item.m_ItemSlot5 = this->bot[botNum].storeItem[i].Sock[4];
			item.m_iPShopValue = this->bot[botNum].storeItem[i].Value;
			item.Convert(this->bot[botNum].storeItem[i].num,item.m_SkillOption,item.m_LuckOption,item.m_Z28Option,item.m_NewOption,item.m_SetOption,item.m_ItemOptionEx,3);

			gObj[bIndex].pInventory[blank].m_SkillOption = item.m_SkillOption;
			gObj[bIndex].pInventory[blank].m_LuckOption = item.m_LuckOption;
			gObj[bIndex].pInventory[blank].m_Z28Option = item.m_Z28Option;
			gObj[bIndex].pInventory[blank].m_JewelOfHarmonyOption = item.m_JewelOfHarmonyOption;
			gObj[bIndex].pInventory[blank].m_ItemOptionEx = item.m_ItemOptionEx;

			item.m_Number = 0;

			gObj[bIndex].pInventory[blank] = item;
		}
	}	
}

void ObjBotStore::MakeBot()
{
	if(this->Enabled == true)
	{
		for(int botNum=0;botNum<MAX_BOTSTORE;botNum++)
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
					gObj[result].IsBot = 4;
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
					
					gObj[result].pInventory = new CItem[MAIN_INVENTORY_NORMAL_SIZE + (MAX_EXTENDED_INV*EXTENDED_INV_SIZE) + PSHOP_SIZE];					
					gObj[result].Inventory1 = new CItem[MAIN_INVENTORY_NORMAL_SIZE + (MAX_EXTENDED_INV*EXTENDED_INV_SIZE) + PSHOP_SIZE];
					gObj[result].InventoryMap1 = new BYTE[MAIN_INVENTORY_NORMAL_SIZE + (MAX_EXTENDED_INV*EXTENDED_INV_SIZE) + PSHOP_SIZE];
					gObj[result].pInventoryMap = new BYTE[MAIN_INVENTORY_NORMAL_SIZE + (MAX_EXTENDED_INV*EXTENDED_INV_SIZE) + PSHOP_SIZE];

					for (int i=0;i<(MAIN_INVENTORY_NORMAL_SIZE + (MAX_EXTENDED_INV*EXTENDED_INV_SIZE) + PSHOP_SIZE);i++)
					{
						gObj[result].pInventory[i].Clear();
						gObj[result].Inventory1[i].Clear();
					}

					memset(&gObj[result].pInventoryMap[0], (BYTE)-1, (MAIN_INVENTORY_NORMAL_SIZE + (MAX_EXTENDED_INV*EXTENDED_INV_SIZE) + PSHOP_SIZE));	//108+64+64
					memset(&gObj[result].InventoryMap1[0], (BYTE)-1, (MAIN_INVENTORY_NORMAL_SIZE + (MAX_EXTENDED_INV*EXTENDED_INV_SIZE) + PSHOP_SIZE));	//108+64+64

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
					this->AddItem(result,botNum);
					
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
					gObj[result].BotPower = this->bot[botNum].UseVipMoney;
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
					
					gObj[result].m_bPShopOpen = true;
					memcpy(gObj[result].m_szPShopText, this->bot[botNum].StoreName, sizeof(gObj[result].m_szPShopText));

					ChatSend(&gObj[result],"NEW SELLER!");
					GCActionSend(&gObj[result],AT_SALUTE1,result,result);
				}
			}
		}
	}
}

#endif