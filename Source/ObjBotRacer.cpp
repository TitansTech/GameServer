#include "stdafx.h"
#include "GameServer.h"
#include "LogProc.h"
#include "GameMain.h"
#include "User.h"
#include "SCFExDBProtocol.h"
#include "ObjBotRacer.h"
#include "ObjUseSkill.h"
#include "DevilSquare.h"
#include "BloodCastle.h"
#include "ChaosCastle.h"
#include "..\common\winutil.h"
#include "..\include\readscript.h"
#include "..\common\SetItemOption.h"

#if (PACK_EDITION>=3)

ObjBotRacer BotRacer;

struct SDHP_SETRACEREWARD_INFOSAVE
{
	PBMSG_HEAD h;
	BYTE Count;
	botReward_Data_Struct rew[MAX_BOTRACER_REWARD];
};

void ObjBotRacer::Init(char * FilePath)
{
	this->Start					= false;
	this->Enabled				= GetPrivateProfileInt("Common", "SCFRaceEnabled",0, FilePath) ;
	this->RepeatMsgAfterXMins	= GetPrivateProfileInt("Common", "SCFRaceRepeatMsgAfterXMins",30, FilePath) ;
	this->MaxTime				= GetPrivateProfileInt("Common", "SCFRaceMaxSeconds",60, FilePath) ;

	this->DayStart				= GetPrivateProfileInt("Time", "DayOfWeekStart",0, FilePath) ;
	this->DayEnd				= GetPrivateProfileInt("Time", "DayOfWeekEnd",4, FilePath) ;

	this->req.Zen				= GetPrivateProfileInt("Requeriments", "SCFRaceZen",100000, FilePath) ;
	this->req.OnlyVIP			= GetPrivateProfileInt("Requeriments", "SCFRaceOnlyVIP",0, FilePath) ;
	this->req.VipMoney  		= GetPrivateProfileInt("Requeriments", "SCFRaceVipMoney",0, FilePath) ;
	this->req.MinLevel			= GetPrivateProfileInt("Requeriments", "SCFRaceMinLevel",1, FilePath) ;

	this->Period = 2;

	this->Read("..\\SCFData\\SCFBots\\SCF_BotRacer.txt");

	LogAddTD("[SCF Racer] - %s file is Loaded",FilePath);
}

void ObjBotRacer::CheckStatus()
{
	if(this->Enabled == TRUE)
	{
		SYSTEMTIME time;
		GetLocalTime(&time);

		//0 Sunday --- 6 Saturday

		if(time.wDayOfWeek >= this->DayStart && time.wDayOfWeek < this->DayEnd)
		{
			if(this->Start == false)
				this->StartEvent();
		}else if(time.wDayOfWeek == this->DayEnd)
		{
			if(this->Period != 2)
			{
				this->Period = 2;
			}
		}
	}
}

void ObjBotRacer::Read(char * FilePath)
{
	if(this->Enabled == TRUE)
	{
		for(int botNum=0;botNum<MAX_BOTRACER;botNum++)
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
	for(int i=0;i<MAX_BOTRACER;i++)
	{
		this->bot[i].index = -1;
		this->bot[i].Enabled = FALSE;
		memset(&this->bot[i].Pos,0,sizeof(this->bot[i].Pos));
		for(int j=0;j<9;j++)
			this->bot[i].body[j].num =-1;
	}

	this->PosCount = 0;
	this->RewardCount = 0;

	for(int i=0;i<MAX_BOTRACER_REWARD;i++)
	{
		this->reward[i].num = 0;
		this->reward[i].Level = 0;
		this->reward[i].Opt = 0;
		this->reward[i].Luck = 0;
		this->reward[i].Skill = 0;
		this->reward[i].Dur = 0;
		this->reward[i].Exc = 0;
		this->reward[i].Anc = 0;

		this->reward[i].Sock1 = 0;
		this->reward[i].Sock2 = 0;
		this->reward[i].Sock3 = 0;
		this->reward[i].Sock4 = 0;
		this->reward[i].Sock5 = 0;

		this->reward[i].Days = 0;
		this->reward[i].JOH = 0;
		this->reward[i].IsForeverFFFE = 0;
		
		this->reward[i].Zen = 0;
		this->reward[i].VipMoney = 0;
	}

	//this->Enabled = FALSE;
	int Token;
	SMDFile = fopen(FilePath, "r");

	if ( SMDFile == NULL )
	{
		MsgBox("BotRacer data load error %s", FilePath);
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
				if(BotNum < 0 || BotNum > MAX_BOTRACER-1)
				{
					MsgBox("BotRacer error: BotPetIndex:%d out of range!", BotNum);
					return;
				}

				Token = GetToken();
				this->bot[BotNum].Class = TokenNumber;

				Token = GetToken();
				strncpy(this->bot[BotNum].Name,TokenString,sizeof(this->bot[BotNum].Name));

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
				if(BotNum < 0 || BotNum > MAX_BOTRACER-1)
				{
					MsgBox("BotRacer error: BotPetIndex:%d doesnt exist", BotNum);
					return;
				}
				if(this->bot[BotNum].Enabled == false)
				{	
					MsgBox("BotRacer error: BotPetIndex:%d doesnt exist", BotNum);
					return;
				}
				Token = GetToken();
				int Slot = TokenNumber;
				
				if(Slot < 0 || Slot > 8)
				{	
					MsgBox("BotRacer error: Min Slot 0 ; Max Slot 8");
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
			//this->Enabled = TRUE;
			while(true)
			{
				Token = GetToken();
				if ( strcmp("end", TokenString) == 0 )
				{
					break;
				}

				if(this->bot[0].Enabled == false || this->bot[1].Enabled == false)
				{	
					MsgBox("BotRacer error: BotPet doesnt exist");
					return;
				}

				this->bot[0].Pos[this->PosCount].Map = TokenNumber;
				
				Token = GetToken();
				this->bot[0].Pos[this->PosCount].X = TokenNumber;

				Token = GetToken();
				this->bot[0].Pos[this->PosCount].Y = TokenNumber;
				
				Token = GetToken();
				strncpy(this->bot[0].Pos[this->PosCount].Text,TokenString,sizeof(this->bot[0].Pos[this->PosCount].Text));
				
				Token = GetToken();
				strncpy(this->bot[1].Pos[this->PosCount].Text,TokenString,sizeof(this->bot[1].Pos[this->PosCount].Text));

				Token = GetToken();
				this->bot[1].Pos[this->PosCount].Map = TokenNumber;
				
				Token = GetToken();
				this->bot[1].Pos[this->PosCount].X = TokenNumber;

				Token = GetToken();
				this->bot[1].Pos[this->PosCount].Y = TokenNumber;

				this->PosCount++;
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
				if(this->RewardCount < 0 || this->RewardCount > MAX_BOTRACER_REWARD-1)
				{
					MsgBox("BotRacer error: Max Reward Count", MAX_BOTRACER_REWARD);
					return;
				}

				this->reward[this->RewardCount].Zen = TokenNumber;

				Token = GetToken();
				this->reward[this->RewardCount].VipMoney = TokenNumber;

				Token = GetToken();
				int iType = TokenNumber;

				Token = GetToken();
				int iIndex = TokenNumber;

				this->reward[this->RewardCount].num = ITEMGET(iType,iIndex);

				Token = GetToken();
				this->reward[this->RewardCount].Level = TokenNumber;

				Token = GetToken();
				this->reward[this->RewardCount].Opt = TokenNumber;

				Token = GetToken();
				this->reward[this->RewardCount].Luck = TokenNumber;

				Token = GetToken();
				this->reward[this->RewardCount].Skill = TokenNumber;

				Token = GetToken();
				this->reward[this->RewardCount].Dur = TokenNumber;

				Token = GetToken();
				this->reward[this->RewardCount].Exc = TokenNumber;

				Token = GetToken();
				this->reward[this->RewardCount].Anc = TokenNumber;

				Token = GetToken();
				this->reward[this->RewardCount].Sock1 = TokenNumber;

				Token = GetToken();
				this->reward[this->RewardCount].Sock2 = TokenNumber;

				Token = GetToken();
				this->reward[this->RewardCount].Sock3 = TokenNumber;

				Token = GetToken();
				this->reward[this->RewardCount].Sock4 = TokenNumber;

				Token = GetToken();
				this->reward[this->RewardCount].Sock5 = TokenNumber;

				Token = GetToken();
				this->reward[this->RewardCount].JOH = TokenNumber;

				Token = GetToken();
				this->reward[this->RewardCount].Days = TokenNumber;

				Token = GetToken();
				this->reward[this->RewardCount].IsForeverFFFE = TokenNumber;

				if(this->reward[this->RewardCount].Days != 0)
					this->reward[this->RewardCount].IsForeverFFFE = 0;

				this->RewardCount++;
			}
		}
		break;
	}

	LogAddTD("[BotRacer] - %s file is Loaded",FilePath);
	fclose(SMDFile);
}

BYTE ObjBotRacer::GetDir()
{
	if(this->Enabled == TRUE)
	{
		srand(static_cast<int>(time(NULL)));
		int r = rand()%(this->PosCount);
		return r;
	}else
	{
		return 255;
	}
}

bool ObjBotRacer::MakeBot()
{
	if(this->Enabled == TRUE)
	{
		this->CurrentPos = this->GetDir();

		for(int botNum=0;botNum<MAX_BOTRACER;botNum++)
		{
			this->bot[botNum].index = 0;

			if(this->bot[botNum].Enabled == true)
			{
				int result = gObjAddCallMon();

				if(result >= 0)
				{
					this->bot[botNum].index = result;
					gObj[result].m_PosNum = (WORD)-1;
					gObj[result].X = this->bot[botNum].Pos[this->CurrentPos].X;
					gObj[result].Y = this->bot[botNum].Pos[this->CurrentPos].Y;
					gObj[result].m_OldX = this->bot[botNum].Pos[this->CurrentPos].X;
					gObj[result].m_OldY = this->bot[botNum].Pos[this->CurrentPos].Y;
					gObj[result].TX = this->bot[botNum].Pos[this->CurrentPos].X;
					gObj[result].TY = this->bot[botNum].Pos[this->CurrentPos].Y;
					gObj[result].MTX = this->bot[botNum].Pos[this->CurrentPos].X;
					gObj[result].MTY = this->bot[botNum].Pos[this->CurrentPos].Y;
					gObj[result].Dir = rand()%9;
					gObj[result].MapNumber = this->bot[botNum].Pos[this->CurrentPos].Map;
					gObj[result].Live = TRUE;
					gObj[result].PathCount = 0;
					gObj[result].IsBot = 8;
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

					ChatSend(&gObj[result],"Race Start!!");
					GCActionSend(&gObj[result],AT_SALUTE1,result,result);
				}else
				{
					return false;
				}
			}
		}
	}
	return true;
}

int ObjBotRacer::GetBotIndex(int aIndex)
{
	for(int i=0;i<MAX_BOTRACER;i++)
	{
		if(this->bot[i].Enabled == true)
		{
			if(this->bot[i].index == aIndex)
				return i;
		}
	}
	return -1;
}

BOOL ObjBotRacer::TradeOpen(int index, int nindex)
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

	LPOBJ lpObj = &gObj[index];
	LPOBJ lpBot = &gObj[nindex];

	char tmp[512]={0};
	if(number == 0)
	{
		if(this->req.MinLevel > gObj[index].Level)
		{
			wsprintf(tmp,lMsg.Get(MSGGET(14, 159)),this->req.MinLevel);
			GCServerMsgStringSend(tmp ,lpObj->m_Index, 0x01);
			return 1;
		}
		if(this->req.Zen > gObj[index].Money)
		{
			wsprintf(tmp,lMsg.Get(MSGGET(14, 160)),this->req.Zen);
			GCServerMsgStringSend(tmp ,lpObj->m_Index, 0x01);
			return 1;		
		}
		if(this->req.VipMoney > gObj[index].VipMoney)
		{
			wsprintf(tmp,lMsg.Get(MSGGET(14, 161)),this->req.VipMoney);
			GCServerMsgStringSend(tmp ,lpObj->m_Index, 0x01);
			return 1;
		}
		if(this->req.OnlyVIP == 1 && gObj[index].Vip == 0)
		{
			GCServerMsgStringSend(lMsg.Get(MSGGET(14, 162)) ,lpObj->m_Index, 0x01);
			return 1;
		}
		
		if(this->req.Zen > 0)
		{
			gObj[index].Money -= this->req.Zen;
			::GCMoneySend(index, gObj[index].Money);
		}
		if(this->req.VipMoney > 0)
		{
			gObj[index].VipMoney -= this->req.VipMoney;
			gObj[index].AccountExtraInfoModified = 1;
			wsprintf(tmp,"VipMoney = %d",gObj[index].VipMoney);
			GCServerMsgStringSend(tmp,index, 1);
		}

		ChatTargetSend(&gObj[this->bot[number].index],"Race Start!",index);
		lpObj->RaceCheck = 1;
		lpObj->RaceTime = 0;
		GCServerMsgStringSend(this->bot[1].Pos[this->CurrentPos].Text,lpObj->m_Index, 0x00);
	}else if(number == 1)
	{
		if(lpObj->RaceCheck == 1)
		{
			wsprintf(tmp,lMsg.Get(MSGGET(14, 163)),lpObj->RaceTime);
			GCServerMsgStringSend(tmp ,lpObj->m_Index, 0x01);
			SendRaceInfo(index);
			this->ResetTime(index);
		}else
		{
			ChatTargetSend(&gObj[this->bot[number].index],"Go to the Start CheckPoint First!",index);
		}
	}

	return 1;
}

void ObjBotRacer::ResetTime(int aIndex)
{
	if (aIndex < 0 || aIndex > OBJMAX-1 )
	{
		return;
	}

	gObj[aIndex].RaceCheck = 0;
	gObj[aIndex].RaceTime = 0;
}

void ObjBotRacer::CheckRaceTime(int aIndex)
{
	if ( gObjIsConnectedGP(aIndex) == FALSE )
	{
		//LogAddC(2, "error-L3 [%s][%d]", __FILE__, __LINE__);
		return;
	}

	if(gObj[aIndex].RaceCheck == 1)
	{
		gObj[aIndex].RaceTime++;		

		if(gObj[aIndex].RaceTime >= this->MaxTime)
		{
			this->ResetTime(aIndex);
			GCServerMsgStringSend(lMsg.Get(MSGGET(14, 157)) ,aIndex, 0x00);
		}
	}
}

void ObjBotRacer::StartEvent()
{
	if((this->Start == false) && (this->Enabled == TRUE))
	{
		_beginthread( ObjBotRacer__InsideTrigger, 0, NULL  );
	}
}

void ObjBotRacer__InsideTrigger(void * lpParam)
{
	BotRacer.ForceClose = false;
	BotRacer.Start = true;
	BotRacer.Period = 1;

	bool isEvent = BotRacer.MakeBot();
	int Mins = 0;
	BYTE Secs = 0;

	if(isEvent == true)
	{
		while(true)
		{
			if(BotRacer.ForceClose == true)
			{
				BotRacer.Start = false;
				return;
			}

			if(BotRacer.Period == 2)
				break;

			if(Secs == 59 && Mins%BotRacer.RepeatMsgAfterXMins == 0)
			{
				AllSendServerMsg(BotRacer.bot[0].Pos[BotRacer.CurrentPos].Text);
				if(Mins != 0)
					Mins = 0;
			}

			Secs++;
			if(Secs == 60)
			{
				Mins++;
				Secs = 0;
			}

			Sleep(1000);
		}
	}

	for(int botNum=0;botNum<MAX_BOTRACER;botNum++)
	{
		if(BotRacer.bot[botNum].Enabled == true)
		{
			int bIndex = BotRacer.bot[botNum].index;
			if(bIndex != 0)
			{
				if(gObjIsConnected(bIndex) == TRUE)
				{
					gObjDel(bIndex);
				}
			}
		}
	}

	SendRaceReward();
	BotRacer.Start = false;
}

#endif