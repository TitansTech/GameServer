#include "stdafx.h"
#include "LogProc.h"
#include "GameMain.h"
#include "SCFExDBProtocol.h"
#include "SCFBotHidenSeek.h"
#include "..\include\readscript.h"
#include "..\common\SetItemOption.h"
#include "..\common\winutil.h"
#include "MapServerManager.h"

#if (PACK_EDITION>=3)

ObjBotHider BotHider;

void ObjBotHider::Init(char * FilePath)
{
	this->Start					= false;
	this->Enabled				= GetPrivateProfileInt("Common", "SCFHideAndSeekEnabled",0, FilePath) ;
	this->Minutes				= GetPrivateProfileInt("Common", "SCFHideAndSeekMinutes",10, FilePath) ;
	this->RepeatMsgAfterXSecs	= GetPrivateProfileInt("Common", "SCFHideAndSeekRepeatMsgAfterXSecs",60, FilePath) ;

	this->Read("..\\SCFData\\SCFBots\\SCF_BotHideAndSeek.txt");

	LogAddTD("[SCF HideAndSeek] - %s file is Loaded",FilePath);
}

void ObjBotHider::Read(char * FilePath)
{
	if(this->Enabled == TRUE)
	{
		if(this->bot.Enabled == true)
		{
			if(gObjIsConnected(this->bot.index) == TRUE)
			{
				gObjDel(this->bot.index);
			}
		}

		memset(&this->bot,0,sizeof(this->bot));
		int Token;
		SMDFile = fopen(FilePath, "r");
		this->posCount = 0;

		if ( SMDFile == NULL )
		{
			MsgBox("BotHider data load error %s", FilePath);
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
					this->bot.Class = TokenNumber;

					Token = GetToken();
					strncpy(this->bot.Name,TokenString,sizeof(this->bot.Name));

					this->bot.Enabled = true;
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

					int Slot = TokenNumber;
					
					if(Slot < 0 || Slot > 8)
					{	
						MsgBox("BotHider error: Min Slot 0 ; Max Slot 8");
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
					if(this->posCount < 0 || this->posCount > MAX_BOTHIDER_POS-1)
					{
						MsgBox("BotHideAndSeek error: Max Position Count", MAX_BOTHIDER_POS);
						return;
					}
					this->pos[this->posCount].Map = TokenNumber;

					Token = GetToken();
					this->pos[this->posCount].X1 = TokenNumber;

					Token = GetToken();
					this->pos[this->posCount].Y1 = TokenNumber;

					Token = GetToken();
					this->pos[this->posCount].X2 = TokenNumber;

					Token = GetToken();
					this->pos[this->posCount].Y2 = TokenNumber;

					Token = GetToken();
					strcpy(this->pos[this->posCount].LocName,TokenString);

					this->posCount++;
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
					if(this->ItemCount < 0 || this->ItemCount > MAX_BOTHIDER_REWARD-1)
					{
						MsgBox("BotHideAndSeek error: Max Reward Count", MAX_BOTHIDER_REWARD);
						return;
					}

					int iType = TokenNumber;

					Token = GetToken();
					int iIndex = TokenNumber;

					this->reward[this->ItemCount].num = ITEMGET(iType,iIndex);

					Token = GetToken();
					this->reward[this->ItemCount].Level = TokenNumber;

					Token = GetToken();
					this->reward[this->ItemCount].Opt = TokenNumber;

					Token = GetToken();
					this->reward[this->ItemCount].Luck = TokenNumber;

					Token = GetToken();
					this->reward[this->ItemCount].Skill = TokenNumber;

					Token = GetToken();
					this->reward[this->ItemCount].Dur = TokenNumber;

					Token = GetToken();
					this->reward[this->ItemCount].Exc = TokenNumber;

					Token = GetToken();
					this->reward[this->ItemCount].Anc = TokenNumber;

					this->ItemCount++;
				}
			}
			break;
		}
		fclose(SMDFile);
	}
			
	LogAddTD("[BotHider] - %s file is Loaded",FilePath);
}

void ObjBotHider::StartEvent()
{
	if((this->Start == false) && (this->Enabled == TRUE))
	{
		_beginthread( ObjBotHider__InsideTrigger, 0, NULL  );
	}
}

void ObjBotHider__InsideTrigger(void * lpParam)
{
	int Seconds = 0;
	char tmp[255]={0};
	BotHider.Founded = false;
	BotHider.Start = true;

	BYTE haveBot = BotHider.MakeBot();

	if(haveBot != 255)
	{
		wsprintf(tmp,lMsg.Get(MSGGET(14, 139)),BotHider.GetBotName());
		LogAddTD(tmp);
		AllSendServerMsg(tmp);
		int EventSec = BotHider.Minutes * 60;

		while(Seconds < EventSec)
		{
			if(BotHider.Founded == true)
			{
				Sleep(5000);
				break;
			}else
			{
				if(Seconds%BotHider.RepeatMsgAfterXSecs == 0)
				{
					wsprintf(tmp,lMsg.Get(MSGGET(14, 142)),BotHider.GetBotLocName(haveBot));
					AllSendServerMsg(tmp);

					//int number = 12345;
					//int digit = number % 10;			=> 5
					//number = (number-digit) / 10;		=> 1234
					//gObj[BotHider.bot.index].X
					//gObj[BotHider.bot.index].Y

					BYTE botX = BotHider.GetX();				//123
					BYTE botX1 = (botX - (botX%100))/100;		//=> 1
					BYTE botX2 = ((botX%100) - (botX%10))/10;	//=> 2
					BYTE botX3 = botX%10;						//=> 3

					BYTE botY = BotHider.GetY();
					BYTE botY1 = (botY - (botY%100))/100;		//=> 1
					BYTE botY2 = ((botY%100) - (botY%10))/10;	//=> 2
					BYTE botY3 = botY%10;						//=> 3

					int randMsg = rand()%9;

					if (randMsg == 0)
						wsprintf(tmp,"X:_%d_ Y:__%d", botX2, botY3);	// 2 - 3
					else if  (randMsg == 1)
						wsprintf(tmp,"X:%d__ Y:__%d", botX1, botY3);	// 1 - 3
					else if  (randMsg == 2)
						wsprintf(tmp,"X:__%d Y:__%d", botX3, botY3);	// 3 - 3
					else if  (randMsg == 3)
						wsprintf(tmp,"X:%d__ Y:_%d_", botX1, botY2);	// 1 - 2
					else if  (randMsg == 4)
						wsprintf(tmp,"X:_%d_ Y:_%d_", botX2, botY2);	// 2 - 2
					else if  (randMsg == 5)
						wsprintf(tmp,"X:__%d Y:_%d_", botX3, botY2);	// 3 - 2
					else if  (randMsg == 6)
						wsprintf(tmp,"X:_%d_ Y:%d__", botX2, botY1);	// 2 - 1
					else if  (randMsg == 7)
						wsprintf(tmp,"X:%d__ Y:%d__", botX1, botY1);	// 1 - 1
					else
						wsprintf(tmp,"X:__%d Y:%d__", botX3, botY1);	// 3 - 1

					AllSendServerMsg(tmp);

				}
			}
			Sleep(1000);
			Seconds++;
		}
		if(haveBot != 255)
			BotHider.RemoveBot();

		wsprintf(tmp,lMsg.Get(MSGGET(14, 140)),BotHider.GetBotName());
		LogAddTD(tmp);
		AllSendServerMsg(tmp);
	}

	BotHider.Start = false;
}

BYTE ObjBotHider::GetDir()
{
	if(this->Enabled == TRUE)
	{
		srand(static_cast<int>(time(NULL)));
		int r = rand()%(this->posCount);
		return r;
	}else
	{
		return 255;
	}
}

void ObjBotHider::GetGoodPosition(BYTE mapnumber, short x1, short y1, short x2, short y2, short &mx, short &my)
{
	int tx = RandomNum(x1,x2,true);

	for(int i=0;i<3;i++)
	{
		int ty = RandomNum(y1,y2,false);
		
		BYTE attr = MapC[mapnumber].GetAttr(tx, ty);

		if ( ((attr&1) != 1) && ((attr &4) != 4) && ((attr&8) != 8) )
		{
			mx = tx;
			my = ty;
			return;
		}

		tx = RandomNum(x1,x2,false);
	}

	while ( gMSetBase.GetBoxPosition(mapnumber, x1, y1, x2, y2, mx, my) == 0 )
	{

	}
}

BYTE ObjBotHider::MakeBot()
{
	BYTE p = 255;
	if(this->Enabled == TRUE)
	{
		if(this->bot.Enabled == true)
		{
			BYTE p = this->GetDir();

			if (g_MapServerManager.CheckMapCanMove(this->pos[p].Map) == FALSE )
			{
				LogAddTD("[BotHider] Cancel Adding Bot to Map:%d",
					this->pos[p].Map);
				
				return 255;
			}

			int result = gObjAddCallMon();

			if(result >= 0)
			{
				this->GetGoodPosition(this->pos[p].Map, this->pos[p].X1, this->pos[p].Y1, this->pos[p].X2, this->pos[p].Y2, gObj[result].X, gObj[result].Y);

				this->bot.index = result;
				gObj[result].m_PosNum = (WORD)-1;
				gObj[result].m_OldX = gObj[result].X;
				gObj[result].m_OldY = gObj[result].Y;
				gObj[result].TX = gObj[result].X;
				gObj[result].TY = gObj[result].Y;
				gObj[result].MTX = gObj[result].X;
				gObj[result].MTY = gObj[result].Y;
				gObj[result].Dir = rand()%9;
				gObj[result].MapNumber = this->pos[p].Map;
				gObj[result].Live = TRUE;
				gObj[result].PathCount = 0;
				gObj[result].IsBot = 7;
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
				gObj[result].BotDefense = this->bot.OnlyVip;
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

				ChatSend(&gObj[result],"COME AND GET ME!!");
				GCActionSend(&gObj[result],AT_SALUTE1,result,result);
				LogAddTD("[BotHider] %s Hides in Map:%d,X:%d,Y:%d",gObj[result].Name,gObj[result].MapNumber,gObj[result].X,gObj[result].Y);
				return p;
			}
		}
	}
	return p;
}


BOOL ObjBotHider::TradeOpen(int index, int nindex)
{
	if(this->Enabled == TRUE)
	{
		if(this->bot.index == nindex)
		{
			if(gObjIsConnected(index) == 0)
			{
				return 0;
			}
			if(gObjIsConnected(nindex) == 0)
			{
				return 0;
			}			
			if(this->Founded == false)
			{
				ChatSend(&gObj[nindex],"Nice, you found me!");

				this->Founded = true;
				char tmp[255]={0};
				wsprintf(tmp,lMsg.Get(MSGGET(14, 141)),gObj[index].Name,this->bot.Name);
				AllSendServerMsg(tmp);
				LogAddTD(tmp);
				this->GetReward(index);
			}else
			{
				ChatSend(&gObj[nindex], lMsg.Get(MSGGET(14, 143)));
			}
			return TRUE;
		}
		return FALSE;	
	}
	return FALSE;
}

void ObjBotHider::GetReward(int aIndex)
{
	if(this->Enabled == TRUE)
	{
		int r = rand()%(this->ItemCount);
		ItemSerialCreateSend(aIndex, gObj[aIndex].MapNumber, gObj[aIndex].X, gObj[aIndex].Y,
							this->reward[r].num , this->reward[r].Level, this->reward[r].Dur, this->reward[r].Skill, this->reward[r].Luck, this->reward[r].Opt, aIndex, this->reward[r].Exc, this->reward[r].Anc);
	}
}

void ObjBotHider::RemoveBot()
{
	ChatSend(&gObj[this->bot.index],"Good Luck!!!");
	gObjDel(this->bot.index);
}

char * ObjBotHider::GetBotName()
{
	return this->bot.Name;
}

char * ObjBotHider::GetBotLocName(BYTE pos)
{
	return this->pos[pos].LocName;
}

BYTE ObjBotHider::GetX()
{
	return gObj[this->bot.index].X;
}

BYTE ObjBotHider::GetY()
{
	return gObj[this->bot.index].Y;
}

#endif