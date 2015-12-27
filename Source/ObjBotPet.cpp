#include "stdafx.h"
#include "GameServer.h"
#include "LogProc.h"
#include "GameMain.h"
#include "User.h"
#include "SCFExDBProtocol.h"
#include "ObjBotPet.h"
#include "..\common\winutil.h"
#include "..\include\readscript.h"
#include "..\common\SetItemOption.h"
#include "BloodCastle.h"
#include "DevilSquare.h"
#include "Raklion.h"
#include "KalimaGate.h"
#include "ChaosCastle.h"
#include "IllusionTemple.h"
#include "ImperialGuardian.h"
#include "DoppelGanger.h"
#include "ItemAddOption.h"

//reqlevel = MagicDamageC.SkillGetRequireLevel(aSkill);
#include "LogToFile.h"
extern CLogToFile TRADE_LOG;

#if (PACK_EDITION>=3)
ObjBotPet botPet;

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


void ObjBotPet::SendInfo(int aIndex)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

#if (CRYSTAL_EDITION == 1)
	int botNum = gObj[aIndex].BotNumOwner;
	if((botNum >= 0) && (gObjIsConnected(botNum) >= 0))
	{
		if(this->Enabled == TRUE)
		{
			LPOBJ lpObj = &gObj[botNum];
			SDHP_RECVBOT pInfo;
			PHeadSubSetB((LPBYTE)&pInfo, 0xFA,0x20, sizeof(pInfo));
			memcpy(pInfo.Name,lpObj->Name,sizeof(pInfo.Name));
			pInfo.Level = lpObj->Level;
			pInfo.Defense = lpObj->m_Defense;
			pInfo.Power = lpObj->BotPower;
			pInfo.Lives1 = lpObj->BotLife;
			pInfo.Lives2 = lpObj->BotMaxLife;

			DataSend(aIndex, (LPBYTE)&pInfo, sizeof(pInfo));
		}
	}
#endif
}


void ObjBotPet::gProcessAutoRecuperation(LPOBJ lpObj)
{
	if(lpObj->Life == (lpObj->MaxLife + lpObj->AddLife)
		&& lpObj->Mana == (lpObj->MaxMana + lpObj->AddMana)
		)
	{
		return;
	}

	lpObj->m_ManaFillCount++;

	if(lpObj->m_ManaFillCount > 2)
	{
		lpObj->m_ManaFillCount = 0;

		if(lpObj->MaxMana + lpObj->AddMana > lpObj->Mana)
		{
			float tmana = lpObj->Mana;
			tmana += (lpObj->MaxMana + lpObj->AddMana)*0.037f;

			tmana += lpObj->m_JewelOfHarmonyEffect.HJOpAddRefillMP;

			if(lpObj->MaxMana + lpObj->AddMana < tmana)
			{
				tmana = lpObj->MaxMana + lpObj->AddMana;
			}

			lpObj->Mana = tmana;
		}
	}

	int percent = 3;
	lpObj->m_LifeFillCount++;

	if(lpObj->m_LifeFillCount > 4)
	{
		lpObj->m_LifeFillCount = 0;
		float tlife = lpObj->Life;

		tlife += (lpObj->MaxLife + lpObj->AddLife)*percent / 100.0f;
		tlife += lpObj->m_JewelOfHarmonyEffect.HJOpAddRefillHP;
			
		if((lpObj->MaxLife + lpObj->AddLife) < tlife)
		{
			tlife = lpObj->MaxLife + lpObj->AddLife;
		}

		lpObj->Life = tlife;

		if(lpObj->BotNumOwner >= OBJ_MAXMONSTER)
		{
			if((lpObj->Life < 40) && (lpObj->MaxLife + lpObj->AddLife > 80))
			{
				char sbuf[512]={0};
				wsprintf(sbuf,"%d/%d",(int)lpObj->Life,((int)lpObj->MaxLife + (int)lpObj->AddLife));
				int len = strlen(sbuf);
				if ( len < 1 )
					return;
				if ( len > MAX_CHAT_LEN-1 )
					return;

				PMSG_CHATDATA_NUMBER pMsg;
				PHeadSetB((LPBYTE)&pMsg, 0x01, len+6);
				strcpy(pMsg.chatmsg, sbuf);
				pMsg.NumberH = SET_NUMBERH(lpObj->m_Index);
				pMsg.NumberL = SET_NUMBERL(lpObj->m_Index);
				if(gObjIsConnected(lpObj->BotNumOwner) >= 0)
					DataSend(lpObj->BotNumOwner,(UCHAR*)&pMsg,sizeof(pMsg));
			}
		}
	}
}

void ObjBotPet::ChangeSkill(int botNum, int Skill)
{
	if(OBJMAX_RANGE(botNum) == 0)
	{
		return;
	}

	char sbuf[512]={0};
	gObj[botNum].BotSkillAttack = 0;
	if((botNum >= 0) && (gObjIsConnected(botNum) >= 0))
	{
		if(Skill > gObj[botNum].MagicCount)
		{
			wsprintf(sbuf,"I only have %d skills!",gObj[botNum].MagicCount);
			ChatSend(&gObj[botNum], sbuf);
			//wsprintf(sbuf,"%s BotPet only have %d skills!",gObj[botNum].Name,gObj[botNum].MagicCount);
			//GCServerMsgStringSend(sbuf, aIndex, 1);
		}
		if ( Skill == -1 )
		{
			ChatSend(&gObj[botNum], "I will fight without skills now!");
			//wsprintf(sbuf,"%s BotPet will fight with hands!",gObj[botNum].Name);
			//GCServerMsgStringSend(sbuf, aIndex, 1);
			gObj[botNum].m_AttackType = 0;
			if((gObj[botNum].pInventory[0].m_Type >= ITEMGET(4,0) && gObj[botNum].pInventory[0].m_Type < ITEMGET(5,0)) || (gObj[botNum].pInventory[1].m_Type >= ITEMGET(4,0) && gObj[botNum].pInventory[1].m_Type < ITEMGET(5,0)))
				gObj[botNum].m_AttackRange = 6;
			else
				gObj[botNum].m_AttackRange = 1;
		}
		else if ( gObj[botNum].Magic[Skill].IsMagic() == TRUE )
		{				
			ChatSend(&gObj[botNum], "Skill changed!");
			//wsprintf(sbuf,"%s BotPet Skill Changed!",gObj[botNum].Name);
			//GCServerMsgStringSend(sbuf, aIndex, 1);
			gObj[botNum].m_AttackType	= gObj[botNum].Magic[Skill].m_Skill;
			gObj[botNum].m_SkillNumber  = gObj[botNum].Magic[Skill].m_Skill;
			gObj[botNum].UseMagicNumber = gObj[botNum].Magic[Skill].m_Skill;
			gObj[botNum].m_AttackRange  = MagicDamageC.GetSkillDistance(gObj[botNum].Magic[Skill].m_Skill);			
			gObj[botNum].BotSkillAttack = 1;
		}
	}
}

BOOL ObjBotPet::BotDecreaseArrow(LPOBJ lpObj)
{
	if ( lpObj->Class == CLASS_ELF )
	{
		CItem * Right = &lpObj->pInventory[0];
		CItem * Left  = &lpObj->pInventory[1];

		if ( (Right->m_Type >= ITEMGET(4,8) && Right->m_Type <= ITEMGET(4,14)) ||
			  Right->m_Type == ITEMGET(4,16) || Right->m_Type == ITEMGET(4,18) ||
			  Right->m_Type == ITEMGET(4,19) )
		{
			if ( Left->m_Type == ITEMGET(4,7) )
			{
				if ( Left->m_Durability < 1.0f )
				{
					int pos = gObjGetItemPos(lpObj->m_Index,4,7);
					if (pos > 0)
					{
						ChatSend(lpObj, "Switching Bolts!");
						lpObj->pInventory[1] = lpObj->pInventory[pos];
						lpObj->pInventory[pos].Clear();

					} else {
						ChatSend(lpObj, "I dont have enough Bolts!");
					}
					return FALSE;
				}

				Left->m_Durability -= 1.0f;

				if ( Left->m_Durability < 1.0f )
				{
					lpObj->pInventory[1].Clear();
					int pos = gObjGetItemPos(lpObj->m_Index,4,7);
					if (pos > 0)
					{
						ChatSend(lpObj, "Switching Bolts!");
						lpObj->pInventory[1] = lpObj->pInventory[pos];
						lpObj->pInventory[pos].Clear();
					} else {
						ChatSend(lpObj, "I dont have enough Bolts!");
					}
				}
			}
			else
			{
				int pos = gObjGetItemPos(lpObj->m_Index,4,7);
				if (pos > 0)
				{
					ChatSend(lpObj, "Switching Bolts!");
					lpObj->pInventory[1] = lpObj->pInventory[pos];
					lpObj->pInventory[pos].Clear();
				} else {
					ChatSend(lpObj, "I dont have enough Bolts!");
				}
				return FALSE;
			}
		}
		else if ( (Left->m_Type >= ITEMGET(4,0) && Left->m_Type <= ITEMGET(4,6) ) ||
				   Left->m_Type == ITEMGET(4,17) ||
				   Left->m_Type == ITEMGET(4,20) ||
				   Left->m_Type == ITEMGET(4,21) ||
				   Left->m_Type == ITEMGET(4,22) ||
				   Left->m_Type == ITEMGET(4,23) ||
				   Left->m_Type == ITEMGET(4,24) )
		{
			if ( Right->m_Type == ITEMGET(4,15) )
			{
				if ( Right->m_Durability < 1.0f )
				{
					int pos = gObjGetItemPos(lpObj->m_Index,4,15);
					if (pos > 0)
					{
						ChatSend(lpObj, "Switching Arrows!");
						lpObj->pInventory[0] = lpObj->pInventory[pos];
						lpObj->pInventory[pos].Clear();
					} else {
						ChatSend(lpObj, "I dont have enough Arrows!");
					}
					return FALSE;
				}

				Right->m_Durability -= 1.0f;

				if ( Right->m_Durability < 1.0f )
				{
					lpObj->pInventory[0].Clear();
					int pos = gObjGetItemPos(lpObj->m_Index,4,15);
					if (pos > 0)
					{
						ChatSend(lpObj, "Switching Arrows!");
						lpObj->pInventory[0] = lpObj->pInventory[pos];
						lpObj->pInventory[pos].Clear();
					} else {
						ChatSend(lpObj, "I dont have enough Arrows!");
					}
				}
			}
			else
			{
				int pos = gObjGetItemPos(lpObj->m_Index,4,15);
				if (pos > 0)
				{
					ChatSend(lpObj, "Switching Arrows!");
					lpObj->pInventory[0] = lpObj->pInventory[pos];
					lpObj->pInventory[pos].Clear();
				} else {
					ChatSend(lpObj, "I dont have enough Arrows!");
				}
				return FALSE;
			}
		}
	}

	return TRUE;
}


bool ObjBotPet::Command(int aIndex,char * cmd, int opt1,char * Name)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return false;
	}

	char sbuf[512]={0};
	int botNum = gObj[aIndex].BotNumOwner;

	if(ReadConfig.CmdBotPetAF > 0)
	{
		if(gObj[aIndex].aFloodBotPetCmd > 0)
		{
			GCServerMsgStringSend("You must wait before use botpet command again", aIndex, 1);
			return true;	
		}
		gObj[aIndex].aFloodBotPetCmd = ReadConfig.CmdBotPetAF;
	}

	#if (WL_PROTECT==1)  
		VM_START_WITHLEVEL(2)
		if(WLRegGetStatus(NULL) != 1)
		{	
		}else
		{
			if(cmd == NULL || !stricmp(cmd,""))
			{
				return true;
			}
		}
		VM_END
	
	#else

	if(cmd == NULL || !stricmp(cmd,""))
	{
		return true;
	}

	#endif
	if(!stricmp(cmd,"create"))
	{
#if (WL_PROTECT==1)  
		VM_START_WITHLEVEL(2)
		if(WLRegGetStatus(NULL) == 1)
		{	
			if(Name == NULL)
			{
				GCServerMsgStringSend("Name must be 4 chars or more", aIndex, 1);
				return true;
			}
		}
		VM_END	
#else
		if(Name == NULL)			
		{
			GCServerMsgStringSend("Name must be 4 chars or more", aIndex, 1);
			return true;
		}
#endif

		if(!stricmp(Name,"") || strlen(Name) < 4)
		{
			GCServerMsgStringSend("Name must be 4 chars or more", aIndex, 1);
			return true;
		}
		if(!stricmp(Name,"") || strlen(Name) > 10)
		{
			GCServerMsgStringSend("Name must have 10 chars or less", aIndex, 1);
			return true;
		}
		if(gObj[aIndex].HaveBot == 1)
		{
			GCServerMsgStringSend("You have a BotPet already", aIndex, 1);
			return true;
		}
		if(opt1 < 0 || opt1 > MAX_BOTS-1)
		{
			GCServerMsgStringSend("BotPet number is out of range!", aIndex, 1);
			return true;
		}
		if(this->bot[opt1].Enabled == false)
		{
			GCServerMsgStringSend("BotPet Dissabled!", aIndex, 1);
			return true;
		}
		int WhoMap = gObj[aIndex].MapNumber;
		if( (BC_MAP_RANGE(WhoMap) == 1) || 
			(CC_MAP_RANGE(WhoMap) == 1) || 
			(IT_MAP_RANGE(WhoMap) == 1) || 
			(DS_MAP_RANGE(WhoMap) == 1) || 
			(KALIMA_MAP_RANGE(WhoMap) == 1) || 
#if (PACK_EDITION>=2)
			(IMPERIALGUARDIAN_MAP_RANGE(WhoMap) == 1) || 
#endif
#if (PACK_EDITION>=3)
			(DG_MAP_RANGE(WhoMap) == 1) || 
#endif
			(WhoMap == MAP_INDEX_DUELMAP))
		{
			GCServerMsgStringSend("You cant create BotPet inside one event or duel map!", aIndex, 1);
			return true;
		}
		if((gObj[aIndex].Vip != 1)&&(this->bot[opt1].OnlyVip>0))
		{
			GCServerMsgStringSend("This BotPet is only for VIPs!", aIndex, 1);
			return true;
		}
		if(this->bot[opt1].VipCost > 0)
		{
			if(gObj[aIndex].VipMoney - this->bot[opt1].VipCost < 0)
			{
				wsprintf(sbuf,"You need %d VipMoney and you have %d!",this->bot[opt1].VipCost,gObj[aIndex].VipMoney);
				GCServerMsgStringSend(sbuf, aIndex, 1);
				return true;
			}
		}
		GDCreatePetBot(aIndex,opt1,Name);
		return true;
	}	
	else if(!stricmp(cmd,"respect"))
	{
		if((botNum >= 0) && (gObjIsConnected(botNum) >= 0))
		{
			GCActionSend(&gObj[botNum],AT_RESPECT1,botNum,aIndex);
		}
		return true;
	}
	else if(!stricmp(cmd,"cheer"))
	{
		if((botNum >= 0) && (gObjIsConnected(botNum) >= 0))
		{
			GCActionSend(&gObj[botNum],AT_CHEER1,botNum,aIndex);
		}
		return true;
	}
	else if(!stricmp(cmd,"sir"))
	{
		if((botNum >= 0) && (gObjIsConnected(botNum) >= 0))
		{
			GCActionSend(&gObj[botNum],AT_SALUTE1,botNum,aIndex);
		}
		return true;
	}
	else if(!stricmp(cmd,"bye"))
	{
		if((botNum >= 0) && (gObjIsConnected(botNum) >= 0))
		{
			GCActionSend(&gObj[botNum],AT_GOODBYE1,botNum,aIndex);
		}
		return true;
	}
	else if(!stricmp(cmd,"hi"))
	{
		if((botNum >= 0) && (gObjIsConnected(botNum) >= 0))
		{
			GCActionSend(&gObj[botNum],AT_GREETING1,botNum,aIndex);
		}
		return true;
	}
	else if(!stricmp(cmd,"cry"))
	{
		if((botNum >= 0) && (gObjIsConnected(botNum) >= 0))
		{
			GCActionSend(&gObj[botNum],AT_CRY1,botNum,aIndex);
		}
		return true;
	}
	else if(!stricmp(cmd,"win"))
	{
		if((botNum >= 0) && (gObjIsConnected(botNum) >= 0))
		{
			GCActionSend(&gObj[botNum],AT_WIN1,botNum,aIndex);
		}
		return true;
	}
	else if(!stricmp(cmd,"hehe"))
	{
		if((botNum >= 0) && (gObjIsConnected(botNum) >= 0))
		{
			GCActionSend(&gObj[botNum],AT_SMILE1,botNum,aIndex);
		}
		return true;
	}
	else if(!stricmp(cmd,"dance"))
	{
		if((botNum >= 0) && (gObjIsConnected(botNum) >= 0))
		{
			GCActionSend(&gObj[botNum],AT_DANCE,botNum,aIndex);
		}
		return true;
	}
	else if(!stricmp(cmd,"comeon"))
	{
		if((botNum >= 0) && (gObjIsConnected(botNum) >= 0))
		{
			GCActionSend(&gObj[botNum],AT_GESTURE1,botNum,aIndex);
		}
		return true;
	}
	
#if (WL_PROTECT==1)
	else if(!stricmp(cmd,"turn"))
	{
		VM_START_WITHLEVEL(3)
		int MyCheckVar;  
		CHECK_PROTECTION(MyCheckVar, 0x43567978)  	 
		if (MyCheckVar != 0x43567978)
		{
			exit(1);
		}
		VM_END
		return true;
	}
#endif
	
	else if(!stricmp(cmd,"skill"))
	{
		this->ChangeSkill(botNum,opt1);
		return true;
	}
	else if(!stricmp(cmd,"invoke"))
	{
		if((botNum >= 0) && (gObjIsConnected(botNum) >= 0))
		{
			GCServerMsgStringSend("BotPet already invoked!", aIndex, 1);
			return true;
		}
		if(gObj[aIndex].m_RecallMon >= 0)
		{
			GCServerMsgStringSend("You cant invoke BotPet, remove your invocation!", aIndex, 1);
			return true;
		}
		int WhoMap = gObj[aIndex].MapNumber;
		if( (BC_MAP_RANGE(WhoMap) == 1) || 
			(CC_MAP_RANGE(WhoMap) == 1) || 
			(IT_MAP_RANGE(WhoMap) == 1) || 
			(DS_MAP_RANGE(WhoMap) == 1) || 
			(KALIMA_MAP_RANGE(WhoMap) == 1) || 
#if (PACK_EDITION>=2)
			(IMPERIALGUARDIAN_MAP_RANGE(WhoMap) == 1) || 
#endif
#if (PACK_EDITION>=3)
			(DG_MAP_RANGE(WhoMap) == 1) || 
#endif
			(WhoMap == MAP_INDEX_DUELMAP))
		{
			GCServerMsgStringSend("You cant invoke BotPet inside one event!", aIndex, 1);
			return true;
		}
		GDAskPetBot(aIndex,gObj[aIndex].AccountID);
		return true;
	}
	else if(!stricmp(cmd,"followme"))
	{
		if((botNum >= 0) && (gObjIsConnected(botNum) >= 0))
		{
			ChatSend(&gObj[botNum], "Ok, im moving with you sir!");
			gObj[botNum].BotFollowMe = 1;
		}
		return true;
	}
	else if(!stricmp(cmd,"freeze"))
	{
		if((botNum >= 0) && (gObjIsConnected(botNum) >= 0))
		{
			ChatSend(&gObj[botNum], "I will stay here sir!");
			gObj[botNum].BotFollowMe = 0;
		}
		return true;
	}	
	else if(!stricmp(cmd,"life"))
	{
		if((botNum >= 0) && (gObjIsConnected(botNum) >= 0))
		{
			wsprintf(sbuf,"%d/%d",(int)gObj[botNum].Life,((int)gObj[botNum].MaxLife + (int)gObj[botNum].AddLife));
			ChatSend(&gObj[botNum], sbuf);
		}
		return true;
	}		
	else if(!stricmp(cmd,"mana"))
	{
		if((botNum >= 0) && (gObjIsConnected(botNum) >= 0))
		{
			wsprintf(sbuf,"%d/%d",(int)gObj[botNum].Mana,((int)gObj[botNum].MaxMana + (int)gObj[botNum].AddMana));
			ChatSend(&gObj[botNum], sbuf);
		}
		return true;
	}	
	else if(!stricmp(cmd,"stats"))
	{
		if((botNum >= 0) && (gObjIsConnected(botNum) >= 0))
		{				
			FHP_FRIEND_MEMO_SEND pMsg;

			pMsg.WindowGuid = 0;
			pMsg.Number = gObj[aIndex].m_Index;
			pMsg.Dir = 143;
			pMsg.Action = 27;
			wsprintf(pMsg.Subject,"%s Stat Info",gObj[botNum].Name);
			memcpy(pMsg.Name, gObj[aIndex].Name, sizeof(pMsg.Name));
			memcpy(pMsg.Photo, gObj[botNum].CharSet, sizeof(pMsg.Photo));
			memcpy(pMsg.ToName, gObj[aIndex].Name, sizeof(pMsg.ToName));
			
			wsprintf(pMsg.Memo,"Hi, im reporting you: \n My Stats are: \n Level: %d \n Power: %d \n Defense: %d \n Life: %d/%d \n Lives Left: %d (Used: %d out of %d)",gObj[botNum].Level,gObj[botNum].BotPower,gObj[botNum].m_Defense,(int)gObj[botNum].Life,(int)(gObj[botNum].MaxLife + gObj[botNum].AddLife),(gObj[botNum].BotMaxLife-gObj[botNum].BotLife),gObj[botNum].BotLife,gObj[botNum].BotMaxLife);

			pMsg.MemoSize = strlen(pMsg.Memo);
			int bsize = pMsg.MemoSize + sizeof(pMsg) - sizeof(pMsg.Memo);
			pMsg.h.set((LPBYTE)&pMsg, 0x70, bsize);
			wsExDbCli.DataSend((PCHAR)&pMsg, bsize);
		}
		return true;
	}
	else if(!stricmp(cmd,"trade"))
	{
		if((botNum >= 0) && (gObjIsConnected(botNum) >= 0))
		{
			this->OpenTrade(aIndex, botNum);
		}
		return true;
	}
	else if(!stricmp(cmd,"potmana"))
	{
		if((botNum >= 0) && (gObjIsConnected(botNum) >= 0))
		{
			int pos = gObjGetManaItemPos(botNum);

			if (pos > 0)
			{
				PMSG_USEITEM pMsg;
				pMsg.inventoryPos = pos;
				pMsg.invenrotyTarget = 0;
				CGUseItemRecv(&pMsg, botNum);

				ChatSend(&gObj[botNum], "Mmmm Mana...!");

				wsprintf(sbuf,"%d/%d",(int)gObj[botNum].Mana,((int)gObj[botNum].MaxMana + (int)gObj[botNum].AddMana));
				ChatSend(&gObj[botNum], sbuf);
			} else {
				ChatSend(&gObj[botNum], "I dont have Mana pots!");
			}
		}
		return true;
	}
	else if(!stricmp(cmd,"pothp"))
	{
		if((botNum >= 0) && (gObjIsConnected(botNum) >= 0))
		{
			int pos = gObjGetHPItemPos(botNum);

			if (pos > 0)
			{
				PMSG_USEITEM pMsg;
				pMsg.inventoryPos = pos;
				pMsg.invenrotyTarget = 0;
				CGUseItemRecv(&pMsg, botNum);

				ChatSend(&gObj[botNum], "I'm feeling better!");

				wsprintf(sbuf,"%d/%d",(int)gObj[botNum].Life,((int)gObj[botNum].MaxLife + (int)gObj[botNum].AddLife));
				ChatSend(&gObj[botNum], sbuf);
			} else {
				ChatSend(&gObj[botNum], "I dont have HP pots!");
			}
		}
		return true;
	}
	else if(!stricmp(cmd,"kill"))
	{
		if((botNum >= 0) && (gObjIsConnected(botNum) >= 0))
		{
			GCServerMsgStringSend("BotPet is DEAD, please create new one!", gObj[botNum].BotNumOwner, 1);

			gObj[botNum].BotLife = gObj[botNum].BotMaxLife;
			gObj[aIndex].HaveBot = 0;

			LogAddTD("[BotPet][KillCMD] [%s][%s] BotPet has died [%s][%d][%d/%d]! ",
					gObj[aIndex].AccountID,gObj[aIndex].Name,
					gObj[botNum].Name,gObj[botNum].Level,gObj[botNum].BotLife,gObj[botNum].BotMaxLife);

			this->Close(aIndex);
		}
		return true;
	}
	else if(!stricmp(cmd,"close"))
	{
		if((botNum >= 0) && (gObjIsConnected(botNum) >= 0))
		{
			GCServerMsgStringSend("BotPet is Logged Out!", gObj[botNum].BotNumOwner, 1);

			LogAddTD("[BotPet][CloseCMD] [%s][%s] BotPet has been closed [%s][%d][%d/%d]! ",
					gObj[aIndex].AccountID,gObj[aIndex].Name,
					gObj[botNum].Name,gObj[botNum].Level,gObj[botNum].BotLife,gObj[botNum].BotMaxLife);

			this->Close(aIndex);
		}
		return true;
	}
	else if(!stricmp(cmd,"relife"))
	{
		if((botNum >= 0) && (gObjIsConnected(botNum) >= 0))
		{
			int Refill = gObj[botNum].Level * this->RelifeBot1;
			if(gObj[botNum].BotVersion > 1)
			{
				Refill = gObj[botNum].Level * this->RelifeBot2;
			}
			if(gObj[aIndex].VipMoney - Refill < 0)
			{
				wsprintf(sbuf,"You need %d VipMoney and you have %d!",Refill,gObj[aIndex].VipMoney);
				GCServerMsgStringSend(sbuf, aIndex, 1);
			}else
			{
				gObj[aIndex].VipMoney = gObj[aIndex].VipMoney - Refill;
				gObj[botNum].BotMaxLife = 0;
				ChatSend(&gObj[botNum], "Thanks! i feel my lifes back!");	
			}
			return true;		
		}
		return true;
	}
	else if((!stricmp(cmd,"?"))||(!stricmp(cmd,"help")))
	{
		
	}
	else
	{
		GCServerMsgStringSend("/botpet CMD Opt1 Name", aIndex, 1);
	}
	return false;
}

int ObjBotPet::gObjMagicAddCheck(LPOBJ lpObj, BYTE Type, BYTE Index, BYTE Level, WORD & SkillNumber)
{

	int skill = -1,reqMana;

	skill = GetSkillNumberInex(Type,Index,Level);

	if(skill < 0)
	{
		return -1;
	}

	reqMana = MagicDamageC.SkillGetMana(skill);

	if((lpObj->Mana) < reqMana)
	{
		return -2;
	}

	if(MagicDamageC.SkillGetRequireClass(lpObj->Class,lpObj->ChangeUP,skill) < 1)
	{
		return -3;
	}

	for(int n = 0; n < MAGIC_SIZE; n++)
	{
		if(lpObj->Magic[n].IsMagic() == 1)
		{
			if(lpObj->Magic[n].m_Skill == skill)
			{
				return -4;
			}
		}
	}

	return 1;
}

int ObjBotPet::gObjMagicAdd(LPOBJ lpObj, BYTE Type, BYTE Index, BYTE Level, WORD & SkillNumber)
{

	int skill = -1,reqMana;

	skill = GetSkillNumberInex(Type,Index,Level);

	if(skill < 0)
	{
		LogAdd(lMsg.Get(548));
		return -1;
	}

	reqMana = MagicDamageC.SkillGetMana(skill);

	if((lpObj->Mana) < reqMana)
	{
		LogAdd(lMsg.Get(549));
		return -1;
	}

	if(MagicDamageC.SkillGetRequireClass(lpObj->Class,lpObj->ChangeUP,skill) < 1)
	{
		LogAdd(lMsg.Get(550));
		return -1;
	}

	for(int n = 0; n < MAGIC_SIZE; n++)
	{
		if(lpObj->Magic[n].IsMagic() == 1)
		{
			if(lpObj->Magic[n].m_Skill == skill)
			{
				LogAdd(lMsg.Get(552));
				return -1;
			}
		}
	}

	skill = -1;

	for(int n = 0; n < MAGIC_SIZE; n++)
	{
		if(lpObj->Magic[n].IsMagic() == 0)
		{
			skill = lpObj->Magic[n].Set(Type,Index,Level);
			if(skill < 0)
			{
				LogAdd(lMsg.Get(553),__FILE__,__LINE__);
				return -1;
			}
			else
			{
				lpObj->MagicCount++;
				SkillNumber = skill;
				return n;
			}
		}
	}

	LogAdd(lMsg.Get(554),__FILE__,__LINE__);
	return -1;
}

void ObjBotPet::OpenTrade(int index, int nindex)
{
	if(OBJMAX_RANGE(index) == 0)
	{
		return;
	}
	if(OBJMAX_RANGE(nindex) == 0)
	{
		return;
	}

	LPOBJ lpObj = &gObj[index];
	LPOBJ lpBot = &gObj[nindex];
	if(this->Enabled == TRUE)
	{
		if(!gObjItsInSafeZone(index) || !gObjItsInSafeZone(nindex))
		{
			ChatSend(&gObj[nindex], "Only in safe zone!");
			return;
		}

		if ( lpObj->m_IfState.use > 0 )
		{
			return;
		}else
		{	
			for(int n = 0; n < TRADE_BOX_SIZE; n++)
			{
				lpObj->Trade[n].Clear();
				lpBot->Trade[n].Clear();
			}

			memset(lpObj->TradeMap, (BYTE)-1, TRADE_BOX_SIZE );
			memset(lpBot->TradeMap, (BYTE)-1, TRADE_BOX_SIZE );

			gObjInventoryTrans(lpObj->m_Index);
			GCTradeResponseSend(true, lpObj->m_Index, lpBot->Name, lpBot->Level, 0);
			GCTradeOkButtonSend(lpObj->m_Index, 1);
			lpObj->m_IfState.state = 1;
			lpObj->m_IfState.use = 1;
			lpObj->m_IfState.type = 1;
			lpObj->TradeMoney = 0;
			lpObj->TargetNumber = nindex;
			lpObj->TargetShopNumber = 32760;
			lpObj->pTransaction = 1;

			lpBot->BotFollowMe = 0;
			lpBot->TargetNumber = lpObj->m_Index;
			lpBot->TradeOk = 1;
			lpBot->pTransaction = 1;

			if(ReadConfig.TradeLog == TRUE)
				TRADE_LOG.Output("[TradeBot] (%s)(%s) OPEN",gObj[index].AccountID,gObj[index].Name);
		}
	}
}

BYTE ObjBotPet::IsInTrade(int aIndex)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return 0;
	}

	if ( gObj[aIndex].TargetShopNumber == 32760)
	{
		return 1;
	}else
	{
		return 0;
	}
}

void ObjBotPet::CancelTrade(int aIndex)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	gObj[gObj[aIndex].BotNumOwner].BotFollowMe = 1;
	gObjTradeCancel(aIndex);
	CGTradeResult(aIndex,3);
}

BOOL ObjBotPet::BotTradeItemInventoryPutTest(int aIndex)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return false;
	}

	BYTE TempInventoryMap[BASIC_INVENTORY_SIZE];
	int number,result = 1;

	number = gObj[aIndex].BotNumOwner;

	memcpy(TempInventoryMap,gObj[aIndex].pInventoryMap,BASIC_INVENTORY_SIZE);	//64

	for(int n = 0; n < TRADE_BOX_SIZE; n++)
	{
		if(gObj[number].Trade[n].IsItem() == 1)
		{
			int Type = gObj[number].Trade[n].m_Type;
			int Part = gObj[number].Trade[n].m_Part;
			if(gObj[aIndex].IsBot == 1 && gObj[aIndex].BotVersion > 1)
			{
				if((Part >= 0) && (Part < 9))
				{
					if(gObj[number].Trade[n].IsClass(gObj[aIndex].Class,gObj[aIndex].ChangeUP) == FALSE)
					{
						ChatTargetSend(&gObj[aIndex], "This is not for me!",number);
						return false;
					}
				}
				else if ( Type == ITEMGET(12,7) ||  //
						Type == ITEMGET(12,12) || //
						Type == ITEMGET(12,24) || //
						(Type >= ITEMGET(12,44) && Type <= ITEMGET(12,45) ) ||	// Orbs
						(Type >= ITEMGET(12,47) && Type <= ITEMGET(12,48) ) ||	// Orbs
						(Type >= ITEMGET(12,16) && Type <= ITEMGET(12,21) ) ||	// Orbs
						(Type >= ITEMGET(15,0) && Type <= ITEMGET(15,4) ) ||	// 
						(Type >= ITEMGET(15,6) && Type <= ITEMGET(15,13) ) ||	// 
						(Type >= ITEMGET(15,16) && Type <= ITEMGET(15,17) ) ||	// 
						(Type >= ITEMGET(15,19) && Type <= ITEMGET(15,21) ) ||	// 
						Type == ITEMGET(15,27) || //
						(Type >= ITEMGET(15,29) && Type <= ITEMGET(15,33) ) ||	// 
						Type == ITEMGET(12,35) )	// Scroll of Fire Scream
				{
					if(gObj[number].Trade[n].IsClass(gObj[aIndex].Class,gObj[aIndex].ChangeUP) == FALSE)
					{
						ChatTargetSend(&gObj[aIndex], "This is not for me!",number);
						return false;
					}else
					{
						WORD skillnumber;
						int check = this->gObjMagicAddCheck(&gObj[aIndex], Type>>(9), Type %  MAX_SUBTYPE_ITEMS, gObj[number].Trade[n].m_Level, skillnumber);
						if(check < 0)
						{
							if(check == -2)
							{
								ChatTargetSend(&gObj[aIndex], "I need more mana!",number);
							}
							else if(check == -3)
							{
								ChatTargetSend(&gObj[aIndex], "This is not for me!",number);
							}
							else if(check == -4)
							{
								ChatTargetSend(&gObj[aIndex], "I have this skill already!",number);
							}
							return false;
						}
					}
				}else if(gObjTempInventoryInsertItem(&gObj[aIndex],gObj[number].Trade[n],TempInventoryMap) == 255)
				{
					return false;
				}
			}
		}
	}

	if(result == 1)
	{
		return true;
	}

	return false;
}


BOOL ObjBotPet::BotTradeitemInventoryPut(int aIndex)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return false;
	}

	bool changeEquip = false;
	int number;
	BYTE insert;
	char szItemName[50] = "Item";
	BYTE ExOption[8];

	number = gObj[aIndex].BotNumOwner;

	for(int n = 0; n < TRADE_BOX_SIZE; n++)
	{
		if(gObj[number].Trade[n].IsItem() == 1)
		{
			int Type = gObj[number].Trade[n].m_Type;
			int Part = gObj[number].Trade[n].m_Part;
			if(gObj[aIndex].IsBot == 1 && gObj[aIndex].BotVersion > 1)
			{
				if((Part >= 0) && (Part < 9))
				{
					if(Part == 0)
					{
						if(gObj[number].Trade[n].m_TwoHand == 0)
						{
							if((n >= 0 && n<=3) || (n >= 8 && n<=11) || (n >= 16 && n<=19) || (n >= 24 && n<=27))
							{
								Part = 0;
							}else
							{
								Part = 1;
							}
						}else
						{
							//gObjInventoryItemSet(aIndex, Part, -1);
							gObj[aIndex].pInventory[1].Clear();
						}
					}
					if(gObj[aIndex].pInventory[0].m_TwoHand == 1 && Part == 1)
					{
						gObj[aIndex].pInventory[0].Clear();
					}
					//gObjInventoryItemSet(aIndex, Part, -1);
					gObj[aIndex].pInventory[Part].Clear();
					gObjInventoryInsertItemPos(aIndex,gObj[number].Trade[n],Part,0);
					changeEquip=true;
				}
				else if ( Type == ITEMGET(12,7) ||  //
						Type == ITEMGET(12,12) || //
						Type == ITEMGET(12,24) || //
						(Type >= ITEMGET(12,44) && Type <= ITEMGET(12,45) ) ||	// Orbs
						(Type >= ITEMGET(12,47) && Type <= ITEMGET(12,48) ) ||	// Orbs
						(Type >= ITEMGET(12,16) && Type <= ITEMGET(12,21) ) ||	// Orbs
						(Type >= ITEMGET(15,0) && Type <= ITEMGET(15,4) ) ||	// 
						(Type >= ITEMGET(15,6) && Type <= ITEMGET(15,13) ) ||	// 
						(Type >= ITEMGET(15,16) && Type <= ITEMGET(15,17) ) ||	// 
						(Type >= ITEMGET(15,19) && Type <= ITEMGET(15,21) ) ||	// 
						Type == ITEMGET(15,27) || //
						(Type >= ITEMGET(15,29) && Type <= ITEMGET(15,33) ) ||	// 
						Type == ITEMGET(12,35) )	// Scroll of Fire Scream
				{
					WORD skillnumber;
					int addskill = this->gObjMagicAdd(&gObj[aIndex], Type>>(9), Type %  MAX_SUBTYPE_ITEMS, gObj[number].Trade[n].m_Level, skillnumber);
					if ( addskill >= 0 )
					{
						char sbuff[255]={0};
						wsprintf(sbuff,"The new Skill Number is %d",addskill);
						ChatTargetSend(&gObj[aIndex], sbuff,number);
					}
				}else
				{
					insert = gObjInventoryInsertItem(aIndex,gObj[number].Trade[n]);
					if(insert != 255)
					{
						gObjInventoryInsertItemPos(aIndex,gObj[number].Trade[n],insert,0);

						strcpy(szItemName,gObj[number].Trade[n].GetName());
						ItemIsBufExOption(ExOption,&gObj[number].Trade[n]);

						if(ReadConfig.TradeLog == TRUE)
						{
							TRADE_LOG.Output(lMsg.Get(538),
								gObj[number].AccountID,gObj[number].Name,gObj[number].MapNumber,gObj[number].X,gObj[number].Y,
								gObj[aIndex].AccountID,gObj[aIndex].Name,gObj[aIndex].MapNumber,gObj[aIndex].X,gObj[aIndex].Y,
								szItemName,gObj[number].Trade[n].m_Number,gObj[number].Trade[n].m_Level,gObj[number].Trade[n].m_SkillOption,gObj[number].Trade[n].m_LuckOption,gObj[number].Trade[n].m_Z28Option,
								ExOption[0],ExOption[1],ExOption[2],ExOption[3],ExOption[4],ExOption[5],ExOption[6],gObj[number].Trade[n].m_SetOption);
						}
					}
					else
					{
						if(ReadConfig.TradeLog == TRUE)
							TRADE_LOG.Output("Error : [%s] vs [%s] BotTradeitemInventoryPut!",gObj[number].Name,gObj[aIndex].Name);
						return false;
					}
				}
			}
		}
	}

	if(changeEquip == true)
	{
		ChatTargetSend(&gObj[aIndex], "Thanks for the items Master!",number);
		GCEquipmentSend(aIndex);
		this->CalcStats(aIndex);
	}
	return true;
}

void ObjBotPet::OKTrade(int aIndex)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	int shopnumber = gObj[aIndex].TargetShopNumber;

	if(shopnumber != 32760)
	{			
		gObj[aIndex].m_IfState.use = 0;
		gObj[aIndex].m_IfState.state = 0;
		gObj[aIndex].m_IfState.type = 0;
		gObj[aIndex].TargetShopNumber = -1;
		gObj[aIndex].TargetNumber = -1;
		gObj[aIndex].pTransaction = 0;
		return;
	} else 
	{
		BYTE result = 1;

		int number = gObj[aIndex].BotNumOwner;

		if(this->BotTradeItemInventoryPutTest(aIndex) == 1)
		{
			if(this->BotTradeItemInventoryPutTest(number) == 1)
			{
				result = 1;
			}
			else
			{
				result = 2;
			}
		}
		else
		{
			result = 2;
		}

		/*if(gObjCheckMaxZen(aIndex,gObj[number].TradeMoney) == 0)
		{
			result = 2;
		}

		if(gObjCheckMaxZen(number,gObj[aIndex].TradeMoney) == 0)
		{
			result = 2;
		}*/

		if(ReadConfig.ItemTradeJOHBlock == TRUE)
		{
			if(g_kJewelOfHarmonySystem.IsEnableToTrade(&gObj[number]) == 0 || g_kJewelOfHarmonySystem.IsEnableToTrade(&gObj[aIndex]) == 0)
			{
				result = 4;
				gObjTradeCancel(aIndex);
				CGTradeResult(aIndex,result);
				return;
			}
		}

		if(!gObjItsInSafeZone(aIndex) || !gObjItsInSafeZone(number))
		{
			ChatSend(&gObj[number], "Only in safe zone!");
			this->CancelTrade(aIndex);
			return;
		}
		
		if ( !gObjIsConnected(aIndex) || !gObjIsConnected(number))
		{
			this->CancelTrade(aIndex);
			return;
		}

		if(gObj[number].IsBot == 0)
		{
			this->CancelTrade(aIndex);
			return;
		}


		if(result == 1)
		{
			this->BotTradeitemInventoryPut(aIndex);
			this->BotTradeitemInventoryPut(number);

			gObjInventoryCommit(aIndex);
			//gObjInventoryCommit(number); FALSE FOR NO LOST ITEMS
			
			gObjMakePreviewCharSet(aIndex);
			gObjMakePreviewCharSet(number);

			gObj[aIndex].Money -= gObj[aIndex].TradeMoney;
			if(gObj[aIndex].Money < 0)
			{
				gObj[aIndex].Money = 0;
			}
			gObj[number].Money -= gObj[number].TradeMoney;
			if(gObj[number].Money < 0)
			{
				gObj[number].Money = 0;
			}

			__int64 tmpZen = (__int64)gObj[aIndex].Money + (__int64)gObj[number].TradeMoney;
			if ( tmpZen > MAX_ZEN )
			{
				gObj[aIndex].Money = MAX_ZEN;
			} else {
				gObj[aIndex].Money += gObj[number].TradeMoney;
			}

			if(ReadConfig.TradeLog == TRUE)
				TRADE_LOG.Output("[%s][%s] , To BOT: [%s] Money trade (%d) (%d)",gObj[aIndex].AccountID,gObj[aIndex].Name,gObj[number].Name,gObj[aIndex].TradeMoney,gObj[number].TradeMoney);

			GJSetCharacterInfo(&gObj[aIndex],aIndex,0);
			gObjItemTextSave(&gObj[aIndex]);
			gObjItemTextSave(&gObj[number]);
			gObjSavePetItemInfo(aIndex, 0);

			gObj[number].BotFollowMe = 1;
		}
		else
		{
			this->CancelTrade(aIndex);
			return;
		}

		gObj[aIndex].TargetNumber = -1;
		gObj[aIndex].TargetShopNumber = -1;
		gObj[aIndex].pTransaction = 0;

		gObj[aIndex].m_IfState.use = 0;
		gObj[aIndex].m_IfState.state = 0;
		gObj[aIndex].m_IfState.type = 0;

		gObj[number].TargetNumber = -1;
		gObj[number].TargetShopNumber = -1;
		gObj[number].pTransaction = 0;

		gObj[number].m_IfState.use = 0;
		gObj[number].m_IfState.state = 0;
		gObj[number].m_IfState.type = 0;

		gObj[aIndex].TradeOk = 0;
		gObj[number].TradeOk = 0;
		
		gObj[aIndex].TradeMoney = 0;
		gObj[number].TradeMoney = 0;

		GCMoneySend(aIndex,gObj[aIndex].Money);
		CGTradeResult(aIndex,result);

		GCItemListSend(aIndex);

		for(int n = 0; n < TRADE_BOX_SIZE; n++)
		{
			gObj[aIndex].Trade[n].Clear();
			gObj[number].Trade[n].Clear();
		}

		gObjNotifyUpdateUnionV1(&gObj[aIndex]);
		gObjNotifyUpdateUnionV2(&gObj[aIndex]);
	}
}

void ObjBotPet::Read(char * FilePath)
{
	for(int i=0;i<MAX_BOTS;i++)
	{
		this->bot[i].Enabled = false;
		this->bot[i].skillcount=0;
		for(int j=0;j<9;j++)
			this->bot[i].item[j].num =-1;
		for(int x=0;x<40;x++)
			this->bot[i].skill[x] = -1;
	}

	this->Enabled = FALSE;
	int Token;
	SMDFile = fopen(FilePath, "r");

	if ( SMDFile == NULL )
	{
		MsgBox("BotPet data load error %s", FilePath);
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
				if(BotNum < 0 || BotNum > MAX_BOTS-1)
				{
					MsgBox("BotPet error: BotPetIndex:%d out of range!", BotNum);
					return;
				}

				Token = GetToken();
				this->bot[BotNum].Class = TokenNumber;

				Token = GetToken();
				this->bot[BotNum].OnlyVip = TokenNumber;

				Token = GetToken();
				this->bot[BotNum].VipCost = TokenNumber;

				Token = GetToken();
				this->bot[BotNum].MaxLife = TokenNumber;

				Token = GetToken();
				this->bot[BotNum].MaxMana = TokenNumber;

				Token = GetToken();
				this->bot[BotNum].BotLifes = TokenNumber;

				Token = GetToken();
				this->bot[BotNum].Version = TokenNumber;

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
				if(BotNum < 0 || BotNum > MAX_BOTS-1)
				{
					MsgBox("BotPet error: BotPetIndex:%d doesnt exist", BotNum);
					return;
				}
				if(this->bot[BotNum].Enabled == false)
				{	
					MsgBox("BotPet error: BotPetIndex:%d doesnt exist", BotNum);
					return;
				}
				Token = GetToken();
				int Slot = TokenNumber;
				
				if(Slot < 0 || Slot > 8)
				{	
					MsgBox("BotPet error: Min Slot 0 ; Max Slot 8");
					return;
				}

				Token = GetToken();
				int iType = TokenNumber;

				Token = GetToken();
				int iIndex = TokenNumber;

				this->bot[BotNum].item[Slot].num = ITEMGET(iType,iIndex);

				Token = GetToken();
				this->bot[BotNum].item[Slot].level = TokenNumber;

				Token = GetToken();
				this->bot[BotNum].item[Slot].opt = TokenNumber;

				this->bot[BotNum].item[Slot].Enabled = true;
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

				int BotNum = TokenNumber;
				if(BotNum < 0 || BotNum > MAX_BOTS-1)
				{
					MsgBox("BotPet error: BotPetIndex:%d doesnt exist", BotNum);
					return;
				}
				if(this->bot[BotNum].Enabled == false)
				{	
					MsgBox("BotPet error: BotPetIndex:%d doesnt exist", BotNum);
					return;
				}

				Token = GetToken();
				int skill = TokenNumber;
				
				if((skill >= 27 && skill <= 36)
					|| (skill >= 67 && skill <= 72)
					|| (skill >= 200 && skill <= 213)
					|| (skill >= 217 && skill <= 222)
					|| skill == 48
					|| skill == 64
					|| skill == 77
					|| skill == 233
					|| skill >= SKILL_SKILLTREE_START
					)
				{	
					MsgBox("BotPet error: Doesnt allow skill:%d",skill);
					return;
				}

				this->bot[BotNum].skill[this->bot[BotNum].skillcount] = skill;
				this->bot[BotNum].skillcount++;
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

				int BotNum = TokenNumber;
				if(BotNum < 0 || BotNum > MAX_BOTS-1)
				{
					MsgBox("BotPet error: BotPetIndex:%d doesnt exist", BotNum);
					return;
				}
				if(this->bot[BotNum].Enabled == false)
				{	
					MsgBox("BotPet error: BotPetIndex:%d doesnt exist", BotNum);
					return;
				}
				Token = GetToken();
				this->bot[BotNum].LvlUpLife = TokenNumber;

				Token = GetToken();
				this->bot[BotNum].LvlUpMana = TokenNumber;

				Token = GetToken();
				this->bot[BotNum].LvlUpPower = TokenNumber;

				Token = GetToken();
				this->bot[BotNum].LvlUpDefense = TokenNumber;

				Token = GetToken();
				this->bot[BotNum].LvlUpExp = TokenNumber;

				Token = GetToken();
				this->bot[BotNum].LvlUpMaxLevel = TokenNumber;
			}
		}
		
		break;
	}
			
	this->Enabled = TRUE;
	LogAddTD("[BotPet] - %s file is Loaded",FilePath);
	fclose(SMDFile);
}

void ObjBotPet::ReadCfg(char * FilePath)
{
	this->RelifeBot1 = GetPrivateProfileInt("Common", "SCFBotPet1RelifeMultLevel",10, FilePath);
	this->RelifeBot2 = GetPrivateProfileInt("Common", "SCFBotPet2RelifeMultLevel",20, FilePath);
}

int ObjBotPet::Make(int PlayerIndex, int botNum, char * botName)
{
	if(OBJMAX_RANGE(PlayerIndex) == 0)
	{
		return false;
	}

	if (botNum < 0 || botNum > MAX_BOTS-1)
		return 0;

	if (this->bot[botNum].Enabled == false)
		return 0;

	LPOBJ lpObj = &gObj[PlayerIndex];	
	int Class = this->bot[botNum].Class >> 4;
	//int result = gObjAddMonster(lpObj->MapNumber);

	int result = gObjAddCallMon();

	if(result >= 0)
	{
		if(gObj[PlayerIndex].m_RecallMon >= 0)
		{
			GCRecallMonLife(PlayerIndex,60,0);
			gObjMonsterCallKill(PlayerIndex);
		}

		gObj[result].X = lpObj->X;
		gObj[result].Y = lpObj->Y;
		gObj[result].m_OldX = lpObj->X;
		gObj[result].m_OldY = lpObj->Y;
		gObj[result].TX = lpObj->X;
		gObj[result].TY = lpObj->Y;
		gObj[result].MTX = lpObj->X;
		gObj[result].MTY = lpObj->Y;
		gObj[result].Dir = 2;
		gObj[result].MapNumber = lpObj->MapNumber;
		gObj[result].Live = TRUE;
		gObj[result].PathCount = 0;
		gObj[result].m_PosNum = (WORD)-1;
		gObj[result].IsBot = 1;

		gObjSetMonster(result,Class,"PetBot");

		gObj[result].Class = Class;

		//if (gObj[result].MasterCharacterInfo == NULL)
		//gObj[result].MasterCharacterInfo = new MASTERCHARINFO;
		//if (gObj[result].Trade == NULL)
		gObj[result].Trade = new CItem[TRADE_BOX_SIZE];
		//if (gObj[result].TradeMap == NULL)
		gObj[result].TradeMap = new BYTE[TRADE_BOX_SIZE];
		memset(gObj[result].TradeMap, (BYTE)-1, TRADE_BOX_SIZE);
			
		//if (gObj[result].pInventory == NULL)
		gObj[result].pInventory = new CItem[INVENTORY_NORMAL_SIZE];
		//if (gObj[result].pInventoryMap == NULL)
		gObj[result].pInventoryMap = new BYTE[INVENTORY_NORMAL_SIZE];
		memset(&gObj[result].pInventoryMap[0], (BYTE)-1, INVENTORY_NORMAL_SIZE);

		for (int i=0;i<INVENTORY_NORMAL_SIZE;i++)
		{
			gObj[result].pInventory[i].Clear();
		}

		gObj[result].DbClass = this->bot[botNum].Class;
		gObj[result].ChangeUP = this->bot[botNum].Class & 0x07;	// Set Second Type of Character

		gObj[result].Level = 1;
		gObj[result].Life = this->bot[botNum].MaxLife;
		gObj[result].MaxLife = this->bot[botNum].MaxLife;
		gObj[result].AddLife = 0;
		gObj[result].AddMana = 0;
		gObj[result].Mana = this->bot[botNum].MaxMana;
		gObj[result].MaxMana = this->bot[botNum].MaxMana;
		gObj[result].Experience = 0;
		strncpy(gObj[result].Name,botName,10);

		//memset(gObj[result].pInventoryMap, (BYTE)-1, ReadConfig.INVENTORY_MAP_SIZE(result,false));
		//memset(&gObj[result].pInventoryMap[0], (BYTE)-1, INVENTORY_NORMAL_SIZE);

		//for(int n = 0; n < INVENTORY_NORMAL_SIZE;n++)
		//{
		//	gObj[result].pInventory[n].Clear();
		//}

		for(int i=0;i<9;i++)
		{
			if(this->bot[botNum].item[i].num >= 0 && this->bot[botNum].item[i].Enabled == true)
			{
				CItem item;
				item.m_Level = this->bot[botNum].item[i].level;
				item.m_SkillOption = 0;
				item.m_LuckOption = 1;
				item.m_Z28Option = this->bot[botNum].item[i].opt;
				item.m_Durability = 255.0f;
				item.m_JewelOfHarmonyOption = 0;
				item.m_ItemOptionEx = 0;
				item.m_ItemSlot1 = 0;
				item.m_ItemSlot2 = 0;
				item.m_ItemSlot3 = 0;
				item.m_ItemSlot4 = 0;
				item.m_ItemSlot5 = 0;
				item.Convert(this->bot[botNum].item[i].num,item.m_SkillOption,item.m_LuckOption,item.m_Z28Option,0,0,item.m_ItemOptionEx,3);

				gObj[result].pInventory[i].m_SkillOption = item.m_SkillOption;
				gObj[result].pInventory[i].m_LuckOption = item.m_LuckOption;
				gObj[result].pInventory[i].m_Z28Option = item.m_Z28Option;
				gObj[result].pInventory[i].m_JewelOfHarmonyOption = item.m_JewelOfHarmonyOption;
				gObj[result].pInventory[i].m_ItemOptionEx = item.m_ItemOptionEx;

				item.m_Number = 0;
				
				gObjInventoryInsertItemPos(gObj[result].m_Index,item,i,0);
				//gObj[result].pInventory[i] = item;
				//int width;
				//int height;
				//gObj[result].pInventory[i].GetSize((int&)width,(int &)height);
				//gObjInventoryItemBoxSet(result,i,width,height,1);
			}
		}

		gObjMakePreviewCharSet(result);

		for(int i=0;i<this->bot[botNum].skillcount;i++)
		{
			gObj[result].Magic[i].Set(this->bot[botNum].skill[i],1);
			gObj[result].MagicCount++;
		}
		if ( lpObj->pInventory[8].m_Type == ITEMGET(13,37) )
		{
			gObj[result].Magic[gObj[result].MagicCount].Set(76,1);
			gObj[result].MagicCount++;
		}
		if(gObj[result].MagicCount > 0)
		{
			gObj[result].m_AttackType = gObj[result].Magic[0].m_Skill;
			gObj[result].m_SkillNumber = gObj[result].Magic[0].m_Skill;
			gObj[result].UseMagicNumber = gObj[result].Magic[0].m_Skill;
			gObj[result].m_AttackRange = MagicDamageC.GetSkillDistance(gObj[result].Magic[0].m_Skill);
			if(gObj[result].m_AttackRange >= 0 && gObj[result].m_AttackRange <= 1)
				gObj[result].m_AttackRange = 2;
			gObj[result].BotSkillAttack = 1;
		}
		else
		{
			gObj[result].m_AttackType = 0;
			
			if((gObj[result].pInventory[0].m_Type >= ITEMGET(4,0) && gObj[result].pInventory[0].m_Type < ITEMGET(5,0)) || (gObj[result].pInventory[1].m_Type >= ITEMGET(4,0) && gObj[result].pInventory[1].m_Type < ITEMGET(5,0)))
				gObj[result].m_AttackRange = 6;
			else
				gObj[result].m_AttackRange = 1;
			gObj[result].BotSkillAttack = 0;
		}

		gObj[result].TargetNumber = (WORD)-1;
		gObj[result].m_ActState.Emotion = 0;
		gObj[result].m_ActState.Attack = 0;
		gObj[result].m_ActState.EmotionCount = 0;
		gObj[result].PathCount = 0;
		gObj[result].BotPower = 0;
		gObj[result].BotDefense = 0;
		gObj[result].BotLife = 0;
		gObj[result].BotMaxLife = this->bot[botNum].BotLifes;
		gObj[result].BotLvlUpDefense = this->bot[botNum].LvlUpDefense;
		gObj[result].BotLvlUpPower = this->bot[botNum].LvlUpPower;
		gObj[result].BotLvlUpLife = this->bot[botNum].LvlUpLife;
		gObj[result].BotLvlUpMana = this->bot[botNum].LvlUpMana;
		gObj[result].BotLvlUpExp = this->bot[botNum].LvlUpExp;
		gObj[result].BotLvlUpMaxLevel = this->bot[botNum].LvlUpMaxLevel;
		gObj[result].BotVersion = this->bot[botNum].Version;
		gObj[result].BotFollowMe = 1;

		gObj[PlayerIndex].m_RecallMon = result;
		gObj[PlayerIndex].BotNumOwner = result;
		gObj[PlayerIndex].HaveBot = 1;

		gObj[result].BotNumOwner = PlayerIndex;
		gObj[result].m_RecallMon = PlayerIndex;
		gObj[result].m_Attribute = 100;

		gObj[result].m_MoveRange = 15;
		gObj[result].NextExp = gLevelExperience[gObj[result].Level];
		gObj[result].m_MagicSpeed = 10;
		gObj[result].m_AttackSpeed = 10;

		gObj[PlayerIndex].AccountExtraInfoModified = 1;
		gObj[PlayerIndex].VipMoney -= this->bot[botNum].VipCost;

		//Display monster health info [For elf only atm]
		GCRecallMonLife(gObj[result].m_RecallMon,100,(gObj[result].Life/gObj[result].MaxLife*100) );

		ChatSend(&gObj[result],"I'm here to protect you!");
		GCActionSend(&gObj[result],AT_SALUTE1,result,PlayerIndex);
		this->SendInfo(PlayerIndex);
		return 1;
	}else
	{
		return 0;
	}
}

void ObjBotPet::DieClose(int aIndex)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	int botID = gObj[aIndex].BotNumOwner;

	if(botID >= OBJ_MAXMONSTER)
	{
		gObj[botID].Life = gObj[botID].MaxLife;
		gObj[botID].Mana = gObj[botID].MaxMana;

		this->Close(aIndex);

		GDAskPetBot(aIndex,gObj[aIndex].AccountID);
		this->SendInfo(aIndex);
	} else {
		gObjMonsterCallKill(aIndex);
	}
}

void ObjBotPet::Close(int aIndex)
{
	if(gObjIsConnected(aIndex) == 0)
	{
		return;
	}

	GDSetPetBot(aIndex);

	int botID = gObj[aIndex].BotNumOwner;

	//Clean User
	gObj[aIndex].IsBot = 0;
	gObj[aIndex].BotNumOwner = -1;
	gObj[aIndex].BotPower = 0;
	gObj[aIndex].BotDefense = 0;
	gObj[aIndex].BotLife = 0;
	gObj[aIndex].BotMaxLife = 0;
	gObj[aIndex].BotLvlUpDefense = 0;
	gObj[aIndex].BotLvlUpPower = 0;
	gObj[aIndex].BotLvlUpLife = 0;
	gObj[aIndex].BotLvlUpMana = 0;
	gObj[aIndex].BotLvlUpExp = 0;
	gObj[aIndex].BotLvlUpMaxLevel = 0;
	gObj[aIndex].BotFollowMe = 0;

	if(botID >= OBJ_MAXMONSTER)
	{
		//Clean Bot
		gObj[botID].IsBot = 0;
		gObj[botID].BotNumOwner = -1;
		gObj[botID].BotPower = 0;
		gObj[botID].BotDefense = 0;
		gObj[botID].BotLife = 0;
		gObj[botID].BotMaxLife = 0;
		gObj[botID].BotLvlUpDefense = 0;
		gObj[botID].BotLvlUpPower = 0;
		gObj[botID].BotLvlUpLife = 0;
		gObj[botID].BotLvlUpMana = 0;
		gObj[botID].BotLvlUpExp = 0;
		gObj[botID].BotLvlUpMaxLevel = 0;
		gObj[botID].BotFollowMe = 0;
		gObj[botID].DieRegen = 0;
		gObj[botID].Teleport = 3;

		/*if ( gObj[botID].Trade != NULL )
			delete gObj[aIndex].Trade;

		if ( gObj[botID].TradeMap != NULL )
			delete gObj[aIndex].TradeMap;

		if ( gObj[botID].MasterCharacterInfo != NULL )
			delete gObj[aIndex].MasterCharacterInfo;

		gObj[aIndex].MasterCharacterInfo = NULL;
		gObj[aIndex].Trade = NULL;
		gObj[aIndex].TradeMap = NULL;*/

		LogAddTD("[%s][%s] BotPet Cleanup Complete : [%d][%s]",
			gObj[aIndex].AccountID, gObj[aIndex].Name,
			botID, gObj[botID].Name);
		this->SendInfo(aIndex);
	} else {
		LogAddTD("[%s][%s] ERROR Cleaning BotPet ID: [%d]",
			gObj[aIndex].AccountID, gObj[aIndex].Name,
			botID, gObj[botID].Name);
	}

	gObjMonsterCallKill(aIndex);
}

struct PMSG_SETQUEST_PRIZE
{
	PBMSG_HEAD h;	// C1:A3
	BYTE NumberH;	// 3
	BYTE NumberL;	// 4
	BYTE Type;	// 5
	BYTE Count;	// 6
};

void ObjBotPet::AddExp(int petIndex,unsigned long iAddExp )
{	
	if(gObjIsConnected(petIndex) == 0)
	{
		return;
	}

	LPOBJ lpObj = &gObj[petIndex];

	if(lpObj->Level >= lpObj->BotLvlUpMaxLevel)
	{
		ChatSend(lpObj, "I have reached my maximum level!");
		return;
	}
	if ( (lpObj->Experience + iAddExp) < lpObj->NextExp )
	{
		lpObj->Experience += iAddExp;
		return;
	}

	ChatSend(lpObj, "I'm feeling more power!!");
	GCSendEffectInfo(lpObj->m_Index, 0x10);

	lpObj->Level++;
	lpObj->Experience = lpObj->NextExp;
	lpObj->NextExp = gLevelExperience[lpObj->Level];

	if(lpObj->BotVersion > 1)
	{
		if(lpObj->Level == 150)
		{
			if(		lpObj->DbClass == DB_DARK_WIZARD
				||	lpObj->DbClass == DB_DARK_KNIGHT
				||	lpObj->DbClass == DB_FAIRY_ELF
				||	lpObj->DbClass == DB_SUMMONER
				)
			{
				lpObj->BotFollowMe = 0;
				lpObj->ChangeUP = true;
				lpObj->DbClass |= 1;
				PlayerRepairOverflow2ndClass(lpObj);
				gObjMakePreviewCharSet(lpObj->m_Index);
				BYTE btClass = (lpObj->Class * 32) & 224 ;
				btClass |= (lpObj->ChangeUP * 16) & 16;

				PMSG_SETQUEST_PRIZE pMsg;

				PHeadSetB((LPBYTE)&pMsg, 0xA3, sizeof(pMsg));
				pMsg.NumberH = SET_NUMBERH(lpObj->m_Index);
				pMsg.NumberL = SET_NUMBERL(lpObj->m_Index);
				pMsg.Type = 201;
				pMsg.Count = btClass;

				MsgSendV2(lpObj, (LPBYTE)&pMsg, sizeof(pMsg));
				lpObj->BotFollowMe = 1;
			}
		}else if(lpObj->Level == 400)
		{
			if(		lpObj->DbClass == DB_SOUL_MASTER
				||	lpObj->DbClass == DB_BLADE_KNIGHT
				||	lpObj->DbClass == DB_MUSE_ELF
				||	lpObj->DbClass == DB_MAGIC_GLADIATOR
				||	lpObj->DbClass == DB_DUEL_MASTER
				||	lpObj->DbClass == DB_DARK_LORD
				||	lpObj->DbClass == DB_LORD_EMPEROR
				||	lpObj->DbClass == DB_BLOODY_SUMMONER
				||	lpObj->DbClass == DB_RAGEFIGHER
				||	lpObj->DbClass == DB_RAGEFIGHER_EVO
				)
			{
				lpObj->BotFollowMe = 0;
				lpObj->ChangeUP = true;
				lpObj->DbClass += 1;
				PlayerRepairOverflow3rdClass(lpObj);
				gObjMakePreviewCharSet(lpObj->m_Index);
				BYTE btClass = (lpObj->Class * 32) & 224 ;
				btClass |= (lpObj->ChangeUP * 16) & 16;
				btClass += 0x08;

				PMSG_SETQUEST_PRIZE pMsg;

				PHeadSetB((LPBYTE)&pMsg, 0xA3, sizeof(pMsg));
				pMsg.NumberH = SET_NUMBERH(lpObj->m_Index);
				pMsg.NumberL = SET_NUMBERL(lpObj->m_Index);
				pMsg.Type = 204;
				pMsg.Count = btClass;

				MsgSendV2(lpObj, (LPBYTE)&pMsg, sizeof(pMsg));
				gObjRecall(lpObj->m_Index,lpObj->MapNumber,lpObj->X,lpObj->Y);
				lpObj->BotFollowMe = 1;
			}
		}
	}


	if(lpObj->BotPower + lpObj->BotLvlUpPower > 65535)
		lpObj->BotPower = 65534;
	else
		lpObj->BotPower += lpObj->BotLvlUpPower;

	if(lpObj->BotDefense + lpObj->BotLvlUpDefense > 65535)
		lpObj->BotDefense = 65534;
	else
		lpObj->BotDefense += lpObj->BotLvlUpDefense;

	if(lpObj->MaxLife + lpObj->BotLvlUpLife + lpObj->AddLife > 65535)
	{
		lpObj->MaxLife = 65534;
		lpObj->AddLife = 0;
		lpObj->BotLvlUpLife = 0;
	}
	else
	{
		lpObj->MaxLife += lpObj->BotLvlUpLife;
	}

	if(lpObj->MaxMana + lpObj->BotLvlUpMana + lpObj->AddMana > 65535)
	{
		lpObj->MaxMana = 65534;
		lpObj->AddMana = 0;
		lpObj->BotLvlUpMana = 0;
	}
	else
		lpObj->MaxMana += lpObj->BotLvlUpMana;

	lpObj->Life = lpObj->MaxLife + lpObj->AddLife ;
	lpObj->Mana = lpObj->MaxMana + lpObj->AddMana;

	this->SendInfo(lpObj->BotNumOwner);
	this->CalcStats(petIndex);
	return;
}

void ObjBotPet::Get(int PlayerIndex)
{
	GDAskPetBot(PlayerIndex,gObj[PlayerIndex].AccountID);
}

void ObjBotPet::CalcStats(int petIndex)
{
	if(OBJMAX_RANGE(petIndex) == 0)
	{
		return;
	}

	LPOBJ lpObj = &gObj[petIndex];
	lpObj->m_Defense = 0;
	lpObj->m_AttackDamageMin = 0;
	lpObj->m_AttackDamageMax = 0;
	lpObj->m_SuccessfulBlocking = 0;
	lpObj->m_AttackRating = 0;
	lpObj->m_MagicDamageMin = 0;
	lpObj->m_MagicDamageMax = 0;
	lpObj->m_AttackDamageMinLeft = 0;
	lpObj->m_AttackDamageMaxLeft = 0;
	lpObj->m_AttackDamageMinRight = 0;
	lpObj->m_AttackDamageMaxRight = 0;

	lpObj->m_wExprienceRate = 100;
	lpObj->m_wItemDropRate = 100;

	int Dexterity = lpObj->BotDefense;
	int Strength = lpObj->BotPower;
	int Energy = lpObj->BotPower/1.5f;
	int Vitality = lpObj->BotDefense;

	lpObj->m_Defense += lpObj->BotDefense;

	lpObj->m_Defense += lpObj->pInventory[2].ItemDefense();
	lpObj->m_Defense += lpObj->pInventory[3].ItemDefense();
	lpObj->m_Defense += lpObj->pInventory[4].ItemDefense();
	lpObj->m_Defense += lpObj->pInventory[5].ItemDefense();
	lpObj->m_Defense += lpObj->pInventory[6].ItemDefense();
	lpObj->m_Defense += lpObj->pInventory[1].ItemDefense();
	lpObj->m_Defense += lpObj->pInventory[7].ItemDefense();

	if ( lpObj->Class == CLASS_ELF )
	{
		lpObj->m_SuccessfulBlocking = Dexterity / ReadConfig.gObjCalCharacter_Blocking_Div_Elf;
	}
	else if ( lpObj->Class == CLASS_DARKLORD )
	{
		lpObj->m_SuccessfulBlocking = Dexterity / ReadConfig.gObjCalCharacter_Blocking_Div_DL;
	}
	else if ( lpObj->Class == CLASS_RAGEFIGHTER )
	{
		lpObj->m_SuccessfulBlocking = Dexterity / ReadConfig.gObjCalCharacter_Blocking_Div_RF;
	}

	else if ( lpObj->Class == CLASS_WIZARD )
	{
		lpObj->m_SuccessfulBlocking = Dexterity / ReadConfig.gObjCalCharacter_Blocking_Div_DW;
	}
	else if ( lpObj->Class == CLASS_KNIGHT )
	{
		lpObj->m_SuccessfulBlocking = Dexterity / ReadConfig.gObjCalCharacter_Blocking_Div_DK;
	}
	else if ( lpObj->Class == CLASS_MAGICGLADIATOR )
	{
		lpObj->m_SuccessfulBlocking = Dexterity / ReadConfig.gObjCalCharacter_Blocking_Div_MG;
	}
	else if ( lpObj->Class == CLASS_SUMMONER )
	{
		lpObj->m_SuccessfulBlocking = Dexterity / ReadConfig.gObjCalCharacter_Blocking_Div_SU;
	}
	else
	{
		lpObj->m_SuccessfulBlocking = Dexterity / ReadConfig.gObjCalCharacter_Blocking_Div_DW;
	}

	lpObj->m_AttackRating = lpObj->BotPower / 1.5f;
	lpObj->m_AttackRating += lpObj->pInventory[5].ItemDefense();

	CItem * Right = &lpObj->pInventory[0];
	CItem * Left  = &lpObj->pInventory[1];

	if ( lpObj->Class == CLASS_ELF ) // Elf
	{
		if ( (Right->m_Type >= ITEMGET(4,8) && Right->m_Type < ITEMGET(4,15) ) ||
			 (Left->m_Type >= ITEMGET(4,0) && Left->m_Type < ITEMGET(4,7)) ||
			  Right->m_Type == ITEMGET(4,16) ||
			  Left->m_Type == ITEMGET(4,20) ||
			  Left->m_Type == ITEMGET(4,21) ||
			  Left->m_Type == ITEMGET(4,22) ||
			  Left->m_Type == ITEMGET(4,23) ||
			  Right->m_Type == ITEMGET(4,18) ||
			  Right->m_Type == ITEMGET(4,19) ||
			  Left->m_Type == ITEMGET(4,17) )
		{
			if ( (Right->IsItem() != FALSE && Right->m_IsValidItem == false) || (Left->IsItem() != FALSE && Left->m_IsValidItem == false) )
			{
				lpObj->m_AttackDamageMin += (Dexterity + Strength ) / ReadConfig.gObjCalCharacter_AttackDamageMin_Bow_StrDexDiv_Elf;
				lpObj->m_AttackDamageMax += (Dexterity + Strength ) / ReadConfig.gObjCalCharacter_AttackDamageMax_Bow_StrDexDiv_Elf;
				lpObj->m_AttackDamageMinRight = (Dexterity + Strength ) / ReadConfig.gObjCalCharacter_AttackDamageMin_Bow_StrDexDiv_Elf;
				lpObj->m_AttackDamageMaxRight = (Dexterity + Strength ) / ReadConfig.gObjCalCharacter_AttackDamageMax_Bow_StrDexDiv_Elf;
				lpObj->m_AttackDamageMinLeft = (Dexterity + Strength ) / ReadConfig.gObjCalCharacter_AttackDamageMin_Bow_StrDexDiv_Elf;
				lpObj->m_AttackDamageMaxLeft = (Dexterity + Strength ) / ReadConfig.gObjCalCharacter_AttackDamageMax_Bow_StrDexDiv_Elf;
			}
			else
			{
				lpObj->m_AttackDamageMin = (Dexterity / ReadConfig.gObjCalCharacter_AttackDamageMin_NoBow_DexDiv_Elf) + (Strength / ReadConfig.gObjCalCharacter_AttackDamageMin_NoBow_StrDiv_Elf);
				lpObj->m_AttackDamageMax = (Dexterity / ReadConfig.gObjCalCharacter_AttackDamageMax_NoBow_DexDiv_Elf) + (Strength / ReadConfig.gObjCalCharacter_AttackDamageMax_NoBow_StrDiv_Elf );
				lpObj->m_AttackDamageMinRight = (Dexterity / ReadConfig.gObjCalCharacter_AttackDamageMin_NoBow_DexDiv_Elf) + (Strength / ReadConfig.gObjCalCharacter_AttackDamageMin_NoBow_StrDiv_Elf);
				lpObj->m_AttackDamageMaxRight = (Dexterity / ReadConfig.gObjCalCharacter_AttackDamageMax_NoBow_DexDiv_Elf) + (Strength / ReadConfig.gObjCalCharacter_AttackDamageMax_NoBow_StrDiv_Elf );
				lpObj->m_AttackDamageMinLeft = (Dexterity / ReadConfig.gObjCalCharacter_AttackDamageMin_NoBow_DexDiv_Elf) + (Strength / ReadConfig.gObjCalCharacter_AttackDamageMin_NoBow_StrDiv_Elf);
				lpObj->m_AttackDamageMaxLeft = (Dexterity / ReadConfig.gObjCalCharacter_AttackDamageMax_NoBow_DexDiv_Elf) + (Strength / ReadConfig.gObjCalCharacter_AttackDamageMax_NoBow_StrDiv_Elf );
			}
		}
		else
		{
			lpObj->m_AttackDamageMin = (Dexterity + Strength) / ReadConfig.gObjCalCharacter_AttackDamageMin_DexStrDiv_Elf;
			lpObj->m_AttackDamageMax = (Dexterity + Strength) / ReadConfig.gObjCalCharacter_AttackDamageMax_DexStrDiv_Elf;
			lpObj->m_AttackDamageMinRight = (Dexterity + Strength) / ReadConfig.gObjCalCharacter_AttackDamageMin_DexStrDiv_Elf;
			lpObj->m_AttackDamageMaxRight = (Dexterity + Strength) / ReadConfig.gObjCalCharacter_AttackDamageMax_DexStrDiv_Elf;
			lpObj->m_AttackDamageMinLeft = (Dexterity + Strength) / ReadConfig.gObjCalCharacter_AttackDamageMin_DexStrDiv_Elf;
			lpObj->m_AttackDamageMaxLeft = (Dexterity + Strength) / ReadConfig.gObjCalCharacter_AttackDamageMax_DexStrDiv_Elf;
		}
	}
	else if ( lpObj->Class == CLASS_KNIGHT ) // Dark Knight
	{
		lpObj->m_AttackDamageMin = Strength / ReadConfig.gObjCalCharacter_AttackDamageMin_StrDiv_DK;
		lpObj->m_AttackDamageMax = Strength / ReadConfig.gObjCalCharacter_AttackDamageMax_StrDiv_DK;
		lpObj->m_AttackDamageMinRight = Strength / ReadConfig.gObjCalCharacter_AttackDamageMin_StrDiv_DK;
		lpObj->m_AttackDamageMaxRight = Strength / ReadConfig.gObjCalCharacter_AttackDamageMax_StrDiv_DK;
		lpObj->m_AttackDamageMinLeft = Strength / ReadConfig.gObjCalCharacter_AttackDamageMin_StrDiv_DK;
		lpObj->m_AttackDamageMaxLeft = Strength / ReadConfig.gObjCalCharacter_AttackDamageMax_StrDiv_DK;
	}
	else if (lpObj->Class == CLASS_MAGICGLADIATOR) // MAgic Gladiator
	{
		lpObj->m_AttackDamageMin = (Strength / ReadConfig.gObjCalCharacter_AttackDamageMin_StrDiv_MG) + (Energy / ReadConfig.gObjCalCharacter_AttackDamageMin_EneDiv_MG);
		lpObj->m_AttackDamageMax = (Strength / ReadConfig.gObjCalCharacter_AttackDamageMax_StrDiv_MG) + (Energy / ReadConfig.gObjCalCharacter_AttackDamageMax_EneDiv_MG);
		lpObj->m_AttackDamageMinRight = (Strength / ReadConfig.gObjCalCharacter_AttackDamageMin_StrDiv_MG) + (Energy / ReadConfig.gObjCalCharacter_AttackDamageMin_EneDiv_MG);
		lpObj->m_AttackDamageMaxRight = (Strength / ReadConfig.gObjCalCharacter_AttackDamageMax_StrDiv_MG) + (Energy / ReadConfig.gObjCalCharacter_AttackDamageMax_EneDiv_MG);
		lpObj->m_AttackDamageMinLeft = (Strength / ReadConfig.gObjCalCharacter_AttackDamageMin_StrDiv_MG) + (Energy / ReadConfig.gObjCalCharacter_AttackDamageMin_EneDiv_MG);
		lpObj->m_AttackDamageMaxLeft = (Strength / ReadConfig.gObjCalCharacter_AttackDamageMax_StrDiv_MG) + (Energy / ReadConfig.gObjCalCharacter_AttackDamageMax_EneDiv_MG);
	}
	else if (lpObj->Class == CLASS_RAGEFIGHTER ) // Rage Fighter
	{
		lpObj->m_AttackDamageMin = (Strength / ReadConfig.gObjCalCharacter_AttackDamageMin_StrDiv_RF) + (Vitality / ReadConfig.gObjCalCharacter_AttackDamageMin_VitDiv_RF);
		lpObj->m_AttackDamageMax = (Strength / ReadConfig.gObjCalCharacter_AttackDamageMax_StrDiv_RF) + (Vitality / ReadConfig.gObjCalCharacter_AttackDamageMax_VitDiv_RF);
		lpObj->m_AttackDamageMinRight = (Strength / ReadConfig.gObjCalCharacter_AttackDamageMin_StrDiv_RF) + (Vitality / ReadConfig.gObjCalCharacter_AttackDamageMin_VitDiv_RF);
		lpObj->m_AttackDamageMaxRight = (Strength / ReadConfig.gObjCalCharacter_AttackDamageMax_StrDiv_RF) + (Vitality / ReadConfig.gObjCalCharacter_AttackDamageMax_VitDiv_RF);
		lpObj->m_AttackDamageMinLeft = (Strength / ReadConfig.gObjCalCharacter_AttackDamageMin_StrDiv_RF) + (Vitality / ReadConfig.gObjCalCharacter_AttackDamageMin_VitDiv_RF);
		lpObj->m_AttackDamageMaxLeft = (Strength / ReadConfig.gObjCalCharacter_AttackDamageMax_StrDiv_RF) + (Vitality / ReadConfig.gObjCalCharacter_AttackDamageMax_VitDiv_RF);
	}
	else if ( lpObj->Class == CLASS_DARKLORD ) // Dark Lord
	{
		lpObj->m_AttackDamageMin = (Strength / ReadConfig.gObjCalCharacter_AttackDamageMin_StrDiv_DL) + (Energy / ReadConfig.gObjCalCharacter_AttackDamageMin_EneDiv_DL);
		lpObj->m_AttackDamageMax = (Strength / ReadConfig.gObjCalCharacter_AttackDamageMax_StrDiv_DL) + (Energy / ReadConfig.gObjCalCharacter_AttackDamageMax_EneDiv_DL);
		lpObj->m_AttackDamageMinRight = (Strength / ReadConfig.gObjCalCharacter_AttackDamageMin_StrDiv_DL) + (Energy / ReadConfig.gObjCalCharacter_AttackDamageMin_EneDiv_DL);
		lpObj->m_AttackDamageMaxRight = (Strength / ReadConfig.gObjCalCharacter_AttackDamageMax_StrDiv_DL) + (Energy / ReadConfig.gObjCalCharacter_AttackDamageMax_EneDiv_DL);
		lpObj->m_AttackDamageMinLeft = (Strength / ReadConfig.gObjCalCharacter_AttackDamageMin_StrDiv_DL) + (Energy / ReadConfig.gObjCalCharacter_AttackDamageMin_EneDiv_DL);
		lpObj->m_AttackDamageMaxLeft = (Strength / ReadConfig.gObjCalCharacter_AttackDamageMax_StrDiv_DL) + (Energy / ReadConfig.gObjCalCharacter_AttackDamageMax_EneDiv_DL);
	}
	else if ( lpObj->Class == CLASS_WIZARD )
	{
		lpObj->m_AttackDamageMin = (Strength / ReadConfig.gObjCalCharacter_AttackDamageMin_StrDiv_DW);
		lpObj->m_AttackDamageMax = (Strength / ReadConfig.gObjCalCharacter_AttackDamageMax_StrDiv_DW);
		lpObj->m_AttackDamageMinRight = (Strength / ReadConfig.gObjCalCharacter_AttackDamageMin_StrDiv_DW);
		lpObj->m_AttackDamageMaxRight = (Strength / ReadConfig.gObjCalCharacter_AttackDamageMax_StrDiv_DW);
		lpObj->m_AttackDamageMinLeft = (Strength / ReadConfig.gObjCalCharacter_AttackDamageMin_StrDiv_DW);
		lpObj->m_AttackDamageMaxLeft = (Strength / ReadConfig.gObjCalCharacter_AttackDamageMax_StrDiv_DW);
	}
	else if ( lpObj->Class == CLASS_SUMMONER )
	{
		lpObj->m_AttackDamageMin = (Strength / ReadConfig.gObjCalCharacter_AttackDamageMin_StrDiv_SU);
		lpObj->m_AttackDamageMax = (Strength / ReadConfig.gObjCalCharacter_AttackDamageMax_StrDiv_SU);
		lpObj->m_AttackDamageMinRight = (Strength / ReadConfig.gObjCalCharacter_AttackDamageMin_StrDiv_SU);
		lpObj->m_AttackDamageMaxRight = (Strength / ReadConfig.gObjCalCharacter_AttackDamageMax_StrDiv_SU);
		lpObj->m_AttackDamageMinLeft = (Strength / ReadConfig.gObjCalCharacter_AttackDamageMin_StrDiv_SU);
		lpObj->m_AttackDamageMaxLeft = (Strength / ReadConfig.gObjCalCharacter_AttackDamageMax_StrDiv_SU);
	}
	else
	{
		lpObj->m_AttackDamageMin = (Strength / ReadConfig.gObjCalCharacter_AttackDamageMin_StrDiv_DW);
		lpObj->m_AttackDamageMax = (Strength / ReadConfig.gObjCalCharacter_AttackDamageMax_StrDiv_DW);
		lpObj->m_AttackDamageMinRight = (Strength / ReadConfig.gObjCalCharacter_AttackDamageMin_StrDiv_DW);
		lpObj->m_AttackDamageMaxRight = (Strength / ReadConfig.gObjCalCharacter_AttackDamageMax_StrDiv_DW);
		lpObj->m_AttackDamageMinLeft = (Strength / ReadConfig.gObjCalCharacter_AttackDamageMin_StrDiv_DW);
		lpObj->m_AttackDamageMaxLeft = (Strength / ReadConfig.gObjCalCharacter_AttackDamageMax_StrDiv_DW);
	}

	lpObj->pInventory[7].PlusSpecial(&lpObj->m_AttackDamageMin, 80);
	lpObj->pInventory[7].PlusSpecial(&lpObj->m_AttackDamageMin, 80);
	lpObj->pInventory[7].PlusSpecial(&lpObj->m_AttackDamageMinRight, 80);
	lpObj->pInventory[7].PlusSpecial(&lpObj->m_AttackDamageMaxRight, 80);
	lpObj->pInventory[7].PlusSpecial(&lpObj->m_AttackDamageMinLeft, 80);
	lpObj->pInventory[7].PlusSpecial(&lpObj->m_AttackDamageMaxLeft, 80);

	if ( Right->m_Type != -1 )
	{
		if ( Right->m_IsValidItem != false )
		{
			if ( Right->m_Type >= ITEMGET(5,0) && Right->m_Type < ITEMGET(6,0) )
			{
				lpObj->m_AttackDamageMin += Right->m_DamageMin / 2;
				lpObj->m_AttackDamageMax += Right->m_DamageMax / 2;
				lpObj->m_AttackDamageMinRight += Right->m_DamageMin / 2;
				lpObj->m_AttackDamageMaxRight += Right->m_DamageMax / 2;
			}
			else
			{
				lpObj->m_AttackDamageMin += Right->m_DamageMin;
				lpObj->m_AttackDamageMax += Right->m_DamageMax;
				lpObj->m_AttackDamageMinRight += Right->m_DamageMin;
				lpObj->m_AttackDamageMaxRight += Right->m_DamageMax;
			}

			if(lpObj->Class == CLASS_RAGEFIGHTER)
			{
				if ( Right->m_Type >= ITEMGET(0,32) && Right->m_Type <= ITEMGET(0,35))
				{
					//lpObj->m_AttackDamageMinRight += (Right->m_DamageMin + Left->m_DamageMin)/2;
					//lpObj->m_AttackDamageMaxRight += (Right->m_DamageMax + Left->m_DamageMax)/2;

					int pMin = lpObj->m_AttackDamageMinRight * (ReadConfig.gObjCalCharacter_AttackDamageMin_PercentPlus_RF/100.0f + 1.0f);
					int pMax = lpObj->m_AttackDamageMaxRight * (ReadConfig.gObjCalCharacter_AttackDamageMax_PercentPlus_RF/100.0f + 1.0f);

					lpObj->m_AttackDamageMinRight=pMin;
					lpObj->m_AttackDamageMaxRight=pMax;
				}
			}
		}

		lpObj->pInventory[0].PlusSpecial(&lpObj->m_AttackDamageMin, 80);
		lpObj->pInventory[0].PlusSpecial(&lpObj->m_AttackDamageMax, 80);
		lpObj->pInventory[0].PlusSpecial(&lpObj->m_AttackDamageMinRight, 80);
		lpObj->pInventory[0].PlusSpecial(&lpObj->m_AttackDamageMaxRight, 80);
	}
		
	if ( Left->m_Type != -1 )
	{
		if ( Left->m_IsValidItem != false)
		{
			lpObj->m_AttackDamageMin += Left->m_DamageMin;
			lpObj->m_AttackDamageMax += Left->m_DamageMax;
			lpObj->m_AttackDamageMinLeft += Left->m_DamageMin;
			lpObj->m_AttackDamageMaxLeft += Left->m_DamageMax;
		}

		if(lpObj->Class == CLASS_RAGEFIGHTER)
		{
			if ( Left->m_Type >= ITEMGET(0,32) && Left->m_Type <= ITEMGET(0,35))
			{
				//lpObj->m_AttackDamageMinLeft += (Right->m_DamageMin + Left->m_DamageMin)/2;
				//lpObj->m_AttackDamageMaxLeft += (Right->m_DamageMax + Left->m_DamageMax)/2;

				int pMin = lpObj->m_AttackDamageMinLeft * (ReadConfig.gObjCalCharacter_AttackDamageMin_PercentPlus_RF/100.0f + 1.0f);
				int pMax = lpObj->m_AttackDamageMaxLeft * (ReadConfig.gObjCalCharacter_AttackDamageMax_PercentPlus_RF/100.0f + 1.0f);

				lpObj->m_AttackDamageMinLeft=pMin;
				lpObj->m_AttackDamageMaxLeft=pMax;
			}
		}

		lpObj->pInventory[1].PlusSpecial(&lpObj->m_AttackDamageMin, 80);
		lpObj->pInventory[1].PlusSpecial(&lpObj->m_AttackDamageMax, 80);
		lpObj->pInventory[1].PlusSpecial(&lpObj->m_AttackDamageMinLeft, 80);
		lpObj->pInventory[1].PlusSpecial(&lpObj->m_AttackDamageMaxLeft, 80);
	}

	if ( lpObj->Class == CLASS_WIZARD )
	{
		lpObj->m_MagicDamageMin = Energy / ReadConfig.gObjCalCharacter_m_MagicDamageMin_Div_DW;
		lpObj->m_MagicDamageMax = Energy / ReadConfig.gObjCalCharacter_m_MagicDamageMax_Div_DW;
	}else if (lpObj->Class == CLASS_MAGICGLADIATOR)
	{
		lpObj->m_MagicDamageMin = Energy / ReadConfig.gObjCalCharacter_m_MagicDamageMin_Div_MG;
		lpObj->m_MagicDamageMax = Energy / ReadConfig.gObjCalCharacter_m_MagicDamageMax_Div_MG;
	}else if (lpObj->Class == CLASS_RAGEFIGHTER)
	{
		lpObj->m_MagicDamageMin = Energy / ReadConfig.gObjCalCharacter_m_MagicDamageMin_Div_RF;
		lpObj->m_MagicDamageMax = Energy / ReadConfig.gObjCalCharacter_m_MagicDamageMax_Div_RF;
	}else if (lpObj->Class == CLASS_SUMMONER)
	{
		lpObj->m_MagicDamageMin = Energy / ReadConfig.gObjCalCharacter_m_MagicDamageMin_Div_SU;
		lpObj->m_MagicDamageMax = Energy / ReadConfig.gObjCalCharacter_m_MagicDamageMax_Div_SU;
	}else if (lpObj->Class == CLASS_ELF)
	{
		lpObj->m_MagicDamageMin = Energy / ReadConfig.gObjCalCharacter_m_MagicDamageMin_Div_Elf;
		lpObj->m_MagicDamageMax = Energy / ReadConfig.gObjCalCharacter_m_MagicDamageMax_Div_Elf;
	}else if (lpObj->Class == CLASS_KNIGHT)
	{
		lpObj->m_MagicDamageMin = Energy / ReadConfig.gObjCalCharacter_m_MagicDamageMin_Div_DK;
		lpObj->m_MagicDamageMax = Energy / ReadConfig.gObjCalCharacter_m_MagicDamageMax_Div_DK;
	}else if (lpObj->Class == CLASS_DARKLORD)
	{
		lpObj->m_MagicDamageMin = Energy / ReadConfig.gObjCalCharacter_m_MagicDamageMin_Div_DL;
		lpObj->m_MagicDamageMax = Energy / ReadConfig.gObjCalCharacter_m_MagicDamageMax_Div_DL;
	}else
	{
		lpObj->m_MagicDamageMin = Energy / ReadConfig.gObjCalCharacter_m_MagicDamageMin_Div_DW;
		lpObj->m_MagicDamageMax = Energy / ReadConfig.gObjCalCharacter_m_MagicDamageMin_Div_DW;
	}

	lpObj->pInventory[7].PlusSpecial(&lpObj->m_MagicDamageMin, 81);
	lpObj->pInventory[7].PlusSpecial(&lpObj->m_MagicDamageMax, 81);

	if ( Right->m_Type != -1 )
	{
		if ( lpObj->pInventory[0].m_Type == ITEMGET(0,31) ||
			 lpObj->pInventory[0].m_Type == ITEMGET(0,21) ||
			 lpObj->pInventory[0].m_Type == ITEMGET(0,23) ||
			 lpObj->pInventory[0].m_Type == ITEMGET(0,25) ) 
		{
			lpObj->pInventory[0].PlusSpecial(&lpObj->m_MagicDamageMin, 80);
			lpObj->pInventory[0].PlusSpecial(&lpObj->m_MagicDamageMax, 80);
		}
		else
		{
			lpObj->pInventory[0].PlusSpecial(&lpObj->m_MagicDamageMin, 81);
			lpObj->pInventory[0].PlusSpecial(&lpObj->m_MagicDamageMax, 81);
		}
	}

	//Fernir [13 37] = 0x1A25
	if ( lpObj->pInventory[8].IsItem() == TRUE && lpObj->pInventory[8].m_Type == ITEMGET(13,37) )
	{
		if ( (lpObj->pInventory[8].m_NewOption &1) )		//Offence
		{
			//	1 = attack power 10% increase
		}
		else if ( (lpObj->pInventory[8].m_NewOption &2) )	//Deffence
		{
			//	2 = defense power 10% increase
		}
		else if ( (lpObj->pInventory[8].m_NewOption &4) )	//Gold
		{
			//	4 = life 200 increase
			//		mana 200 increase
			//		attack power 33% increase
			//		wizzardy 16% increase 
			int increaseAttack = 0;
			int increaseWizzardy = 0;

			if ( lpObj->Class == CLASS_DARKLORD || lpObj->Class == CLASS_MAGICGLADIATOR || lpObj->Class == CLASS_RAGEFIGHTER )
			{
				lpObj->AddLife += 37;			//life 37 increase
				lpObj->AddMana += 37;			//mana 37 increase

				increaseAttack = 16;
				increaseWizzardy = 2;
			} else {
				lpObj->AddLife += 200;			//life 200 increase
				lpObj->AddMana += 200;			//mana 200 increase

				increaseAttack = 33;
				increaseWizzardy = 16;
			}

			//attack power 33% increase
			lpObj->m_AttackDamageMinRight += lpObj->m_AttackDamageMinRight * increaseAttack / 100;
			lpObj->m_AttackDamageMaxRight += lpObj->m_AttackDamageMaxRight * increaseAttack / 100;
			lpObj->m_AttackDamageMinLeft += lpObj->m_AttackDamageMinLeft * increaseAttack / 100;
			lpObj->m_AttackDamageMaxLeft += lpObj->m_AttackDamageMaxLeft * increaseAttack / 100;

			//wizzardy 16% increase 
			lpObj->m_MagicDamageMin += lpObj->m_MagicDamageMin * increaseWizzardy / 100;
			lpObj->m_MagicDamageMax += lpObj->m_MagicDamageMax * increaseWizzardy / 100;
		}
	}

	lpObj->m_MagicSpeed = 20;
	lpObj->m_AttackSpeed = 20;

	//Apply Item Effects (Angel, daemon, skeleton, panda)
	g_ItemAddOption.ApplyInventoryItems(lpObj);
	g_ItemAddOption.ApplyInventoryItemsStats(lpObj);

	for(int i=0;i<2;i++)
	{
		if ( lpObj->m_iItemEffectValidTime > 0 )
		{
			g_ItemAddOption.SetItemLastEffectOther(lpObj,i);
			g_ItemAddOption.SetItemLastEffectLifeMana(lpObj,i);
		}
	}
	this->SendInfo(lpObj->BotNumOwner);
}
#endif