#include "stdafx.h"
#include "..\common\MyWinsockBase.h"
#include "wsJoinServerCli.h"
#include "GameServer.h"
#include "GameMain.h"
#include "DBSockMng.h"
#include "SCFExDBProtocol.h"
#include "SCFExDB_Defines.h"
#include "RProtocol.h"
#include "EProtocol.h"
#include "logproc.h"
#include "..\common\winutil.h"
#include "PCShop.h"
#include "DSProtocol.h"
#include "ObjBotPet.h"
#include "..\common\SetItemOption.h"
#include "SCFVipShop.h"
#include "Marry.h"
#include "VIPSystem.h"
#include "Mastering.h"
#include "Mastering2.h"
#include "EDSProtocol.h"
#include "ExServerProtocol.h"
#include "GensPvP.h"
#include "ObjCalCharacter.h"
#include "MapServerManager.h"
#include "GensCloseMap.h"
#include "LogToFile.h"
#include "MuItemShop.h"
#include "DSGN_ItemMover.h"
#include "ObjBotRacer.h"
#include "ObjBotReward.h"

extern CLogToFile DSGN_ITEMFIX_LOG;
extern CLogToFile BOTREWARD_LOG;


//void SCFExDataServerProtocolCore(BYTE protoNum, LPBYTE aRecv, int aLen)
//{	
//	switch ( protoNum )
//	{
//		case SCFExDB_GSRecv_LoginResult:
//			SCFExDataServerLoginResult((SDHP_RESULT *)aRecv);
//			break;	
//
//		case SCFExDB_GSRecv_GetCharacterInfo:
//			JGGetCharacterInfo((SDHP_DBCHAR_INFORESULT *)aRecv);
//			break;
//
//		case SCFExDB_GSRecv_OptionDataRecv:
//			DGOptionDataRecv((SDHP_SKILLKEYDATA_SEND *)aRecv);
//			break;
//
//	}
//}

#if (PACK_EDITION>=3)

void SendRaceInfo(int aIndex)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	SDHP_SETPLAYERRACE_INFOSAVE pMsg = {0};
	PHeadSetB((LPBYTE)&pMsg, SCFExDB_GSSend_RaceInsert, sizeof(pMsg));
	memcpy(pMsg.AccountID, gObj[aIndex].AccountID, MAX_ACCOUNT_LEN);
	memcpy(pMsg.Name, gObj[aIndex].Name, MAX_ACCOUNT_LEN);
	pMsg.Seconds = gObj[aIndex].RaceTime;
	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}

struct SDHP_SETRACEREWARD_INFOSAVE
{
	PBMSG_HEAD h;
	BYTE Count;
	botReward_Data_Struct rew[MAX_BOTRACER_REWARD];
};

void SendRaceReward()
{
	SDHP_SETRACEREWARD_INFOSAVE pMsg = {0};
	PHeadSetB((LPBYTE)&pMsg, SCFExDB_GSSend_RaceReward, sizeof(pMsg));
	pMsg.Count = BotRacer.RewardCount;
	for(int i=0;i<BotRacer.RewardCount;i++)
	{
		memcpy(&pMsg.rew[i],&BotRacer.reward[i],sizeof(BotRacer.reward[i]));
	}

	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}

void SendReward(int aIndex, int FromEvent, botReward_Data_Struct rew)
{
	SDHP_GETREWARD_INFOSAVE pMsg = {0};
	PHeadSetB((LPBYTE)&pMsg, SCFExDB_GSSend_RewardInfo, sizeof(pMsg));

	strcpy(pMsg.AccountID, gObj[aIndex].AccountID);
	strcpy(pMsg.Name,gObj[aIndex].Name);
	pMsg.aIndex = aIndex;
	pMsg.ID_Num = FromEvent;
	memcpy(&pMsg.rew,&rew,sizeof(pMsg.rew));
	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}

//--

void DelTitanReward(int aIndex, int ID_Num)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	SDHP_GETREWARD_INFODEL pMsg = {0};
	PHeadSetB((LPBYTE)&pMsg, SCFExDB_GSSend_DelBotReward, sizeof(pMsg));
	memcpy(pMsg.AccountID, gObj[aIndex].AccountID, MAX_ACCOUNT_LEN);
	memcpy(pMsg.Name, gObj[aIndex].Name, MAX_ACCOUNT_LEN);
	pMsg.ID_Num = ID_Num;
	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}

void GetTitanReward(SDHP_GETREWARD_INFOSAVE * lpMsg)
{	
	char szId[11]={0};
	int aIndex = 0;

	szId[MAX_ACCOUNT_LEN]=0;
	memcpy(szId, lpMsg->AccountID, sizeof(lpMsg->AccountID));
	aIndex = lpMsg->aIndex;

	if ( !gObjIsAccontConnect(aIndex, szId))
	{
		LogAddC(2, lMsg.Get(MSGGET(1, 175)), szId, aIndex);
		return;
	}
	
	LPOBJ lpObj = &gObj[aIndex];

	if (( lpObj->m_Index == aIndex) && (!strcmp(lpMsg->Name,lpObj->Name)))
	{
		char exVipMoney[512]={0};
		char exZen[512]={0};
		wsprintf(exVipMoney,"VipMoney increased in %d",lpMsg->rew.VipMoney);
		wsprintf(exZen,"Zen increased in %d",lpMsg->rew.Zen);

		if(lpMsg->ID_Num >= 0)
		{
			if(lpMsg->rew.num != (WORD) -1)
			{
				if(lpMsg->rew.Days == 0)
				{
					PMSG_BUYSHOPRESULT pResult;	
					PHeadSetB((LPBYTE)&pResult, 0x32, sizeof(pResult));
					CItem item;
					item.m_Level = lpMsg->rew.Level;
					item.m_SkillOption = lpMsg->rew.Skill;
					item.m_LuckOption = lpMsg->rew.Luck;
					item.m_Z28Option = lpMsg->rew.Opt;
					item.m_Durability = lpMsg->rew.Dur;
					item.m_ItemOptionEx = lpMsg->rew.Exc;
					item.m_JewelOfHarmonyOption = 0;
					if(lpMsg->rew.JOH > 0)
					{
						int btOptionLevel = item.m_Level;
						if(btOptionLevel > ReadConfig.JOHLevelMax)
							btOptionLevel = ReadConfig.JOHLevelMax;
						item.m_JewelOfHarmonyOption |= lpMsg->rew.JOH << 4;
						item.m_JewelOfHarmonyOption |= btOptionLevel & 0x0F;
					}
						
					item.Convert(lpMsg->rew.num, lpMsg->rew.Skill, lpMsg->rew.Luck, lpMsg->rew.Opt, lpMsg->rew.Exc, 0, 0, CURRENT_DB_VERSION);
					item.m_Durability = item.m_BaseDurability;
					item.m_ItemSlot1 = lpMsg->rew.Sock1;
					item.m_ItemSlot2 = lpMsg->rew.Sock2;
					item.m_ItemSlot3 = lpMsg->rew.Sock3;
					item.m_ItemSlot4 = lpMsg->rew.Sock4;
					item.m_ItemSlot5 = lpMsg->rew.Sock5;

					item.Value();
					pResult.Result = gObjShopBuyInventoryInsertItem(lpObj->m_Index,item);
						
					if ( pResult.Result != 0xFF )
					{
						DelTitanReward(aIndex,lpMsg->ID_Num);

						if(lpMsg->rew.IsForeverFFFE == 1)
							lpObj->pInventory[pResult.Result].m_Number = 0xFFFFFFFE;

						ItemByteConvert((LPBYTE)&pResult.ItemInfo, item);
						DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);

						if(lpMsg->rew.VipMoney > 0)
						{						
							lpObj->AccountExtraInfoModified = 1;
							lpObj->VipMoney += lpMsg->rew.VipMoney;
							GCServerMsgStringSend(exVipMoney, lpObj->m_Index, 0x01);
						}
						if(lpMsg->rew.Zen > 0)
						{
							if((__int64)(lpMsg->rew.Zen + gObj[aIndex].Money) > (__int64)MAX_ZEN)
								gObj[aIndex].Money = MAX_ZEN;
							else
								gObj[aIndex].Money += lpMsg->rew.Zen;

							::GCMoneySend(aIndex, gObj[aIndex].Money);
							GCServerMsgStringSend(exZen, lpObj->m_Index, 0x01);
						}
							
						GCServerMsgStringSend("Get Reward Success!", aIndex, 0x01);
						
						char sbuf[512]={0};
						wsprintf(sbuf,"[BotReward](%s)(%s) Buy Item:%d(%d %d) ID_DB:%d [LVL:%d O:%d L:%d S:%d E:%d]",
							gObj[aIndex].AccountID,gObj[aIndex].Name,
							lpMsg->rew.num, lpMsg->rew.num/512, lpMsg->rew.num-((int)(lpMsg->rew.num/512)*512), lpMsg->ID_Num,
							lpMsg->rew.Level,lpMsg->rew.Opt,lpMsg->rew.Luck,lpMsg->rew.Skill,lpMsg->rew.Exc);
						LogAddTD(sbuf);

						BOTREWARD_LOG.Output(sbuf);
					}
				}else
				{
					time_t t = time(NULL);
					localtime(&t);
					DWORD iTime = (DWORD)t + lpMsg->rew.Days * 86400;
					BYTE iItemPos = gObjInventoryInsertItem(&gObj[aIndex], ITEM_GET_TYPE(lpMsg->rew.num), ITEM_GET_INDEX(lpMsg->rew.num), lpMsg->rew.Level, iTime, 0xff,0,0,0,0,0);
					
					if ( iItemPos != (BYTE)-1 )
					{
						DelTitanReward(aIndex,lpMsg->ID_Num);
						gObj[aIndex].pInventory[iItemPos].m_JewelOfHarmonyOption = 0;
						if(lpMsg->rew.JOH > 0)
						{
							int btOptionLevel = gObj[aIndex].pInventory[iItemPos].m_Level;
							if(btOptionLevel > ReadConfig.JOHLevelMax)
								btOptionLevel = ReadConfig.JOHLevelMax;
							gObj[aIndex].pInventory[iItemPos].m_JewelOfHarmonyOption |= lpMsg->rew.JOH << 4;
							gObj[aIndex].pInventory[iItemPos].m_JewelOfHarmonyOption |= btOptionLevel & 0x0F;
						}
						gObj[aIndex].pInventory[iItemPos].Convert(lpMsg->rew.num, lpMsg->rew.Skill, lpMsg->rew.Luck, lpMsg->rew.Opt, lpMsg->rew.Exc, 0, 0, CURRENT_DB_VERSION);
						BYTE btItemType=0;
						btItemType |= (lpMsg->rew.num & 0x1E00 ) >> 5;
						gObj[aIndex].pInventory[iItemPos].m_ItemOptionEx = btItemType+2;
						gObj[aIndex].pInventory[iItemPos].m_ItemSlot1 = lpMsg->rew.Sock1;
						gObj[aIndex].pInventory[iItemPos].m_ItemSlot2 = lpMsg->rew.Sock2;
						gObj[aIndex].pInventory[iItemPos].m_ItemSlot3 = lpMsg->rew.Sock3;
						gObj[aIndex].pInventory[iItemPos].m_ItemSlot4 = lpMsg->rew.Sock4;
						gObj[aIndex].pInventory[iItemPos].m_ItemSlot5 = lpMsg->rew.Sock5;
						::GCInventoryItemOneSend(aIndex, iItemPos);
						MuItemShop.ItemSendTime(aIndex,iItemPos);					

						if(lpMsg->rew.VipMoney > 0)
						{						
							lpObj->AccountExtraInfoModified = 1;
							lpObj->VipMoney += lpMsg->rew.VipMoney;
							GCServerMsgStringSend(exVipMoney, lpObj->m_Index, 0x01);
						}
						if(lpMsg->rew.Zen > 0)
						{
							if((__int64)(lpMsg->rew.Zen + gObj[aIndex].Money) > (__int64)MAX_ZEN)
								gObj[aIndex].Money = MAX_ZEN;
							else
								gObj[aIndex].Money += lpMsg->rew.Zen;

							::GCMoneySend(aIndex, gObj[aIndex].Money);
							GCServerMsgStringSend(exZen, lpObj->m_Index, 0x01);
						}

						char sbuf[512]={0};
						GCServerMsgStringSend("Get Reward Success!", lpObj->m_Index, 0x01);
						wsprintf(sbuf,"[BotReward](%s)(%s) Buy Item:%d(%d %d) ID_DB:%d [LVL:%d O:%d L:%d S:%d E:%d]",
							gObj[aIndex].AccountID,gObj[aIndex].Name,
							lpMsg->rew.num, lpMsg->rew.num/512, lpMsg->rew.num-((int)(lpMsg->rew.num/512)*512), lpMsg->ID_Num,
							lpMsg->rew.Level,lpMsg->rew.Opt,lpMsg->rew.Luck,lpMsg->rew.Skill,lpMsg->rew.Exc);
						LogAddTD(sbuf);
						BOTREWARD_LOG.Output(sbuf);
					}
				}
			}else
			{
				if(lpMsg->rew.VipMoney > 0)
				{						
					lpObj->AccountExtraInfoModified = 1;
					lpObj->VipMoney += lpMsg->rew.VipMoney;
					GCServerMsgStringSend(exVipMoney, lpObj->m_Index, 0x01);
				}
				if(lpMsg->rew.Zen > 0)
				{
					if((__int64)(lpMsg->rew.Zen + gObj[aIndex].Money) > (__int64)MAX_ZEN)
						gObj[aIndex].Money = MAX_ZEN;
					else
						gObj[aIndex].Money += lpMsg->rew.Zen;

					::GCMoneySend(aIndex, gObj[aIndex].Money);
					GCServerMsgStringSend(exZen, lpObj->m_Index, 0x01);
				}

				if(lpMsg->rew.Zen > 0 || lpMsg->rew.VipMoney > 0)
					DelTitanReward(aIndex,lpMsg->ID_Num);
			}
		}else
		{
			GCServerMsgStringSend("You dont have rewards!", aIndex, 0x01);
		}
	}
}

//--

void PetDbItemSetInByte(LPOBJ lpObj, struct SDHP_GETPETBOT* lpMsg)
{
	int n;
	int itype;
	int _type;
	CItem item;
	BYTE OptionData;
	WORD hiWord;
	WORD loWord;
	int ItemDbByte = 16;

	memset(lpObj->pInventoryMap, (BYTE)-1, INVENTORY_NORMAL_SIZE);

	for(n = 0; n < INVENTORY_NORMAL_SIZE;n++)
	{
		lpObj->pInventory[n].Clear();
		itype = lpMsg->dbInventory[n*ItemDbByte];

		//LogAddTD("Set Bot Pet Inventory: %d / %d",n,lpObj->pInventory[n].m_Type);

		if(lpMsg->dbInventory[n*ItemDbByte] == 0xFF
			&& (lpMsg->dbInventory[n*ItemDbByte+DBI_NOPTION_DATA] & 0x80) == 0x80
			&& (lpMsg->dbInventory[n*ItemDbByte+DBI_OPTION380_DATA] & 0xF0) == 0xF0)
		{
			continue;
		}

		itype |= (lpMsg->dbInventory[n*ItemDbByte+DBI_OPTION380_DATA] & 0xF0) * 32;
		itype |= (lpMsg->dbInventory[n*ItemDbByte+DBI_NOPTION_DATA] & 0x80) * 2;
		_type = itype;

		if(lpMsg->dbInventory[n*ItemDbByte] == 0xFF
			&& (lpMsg->dbInventory[n*ItemDbByte+DBI_NOPTION_DATA] & 0x80) == 0x80
			&& (lpMsg->dbInventory[n*ItemDbByte+DBI_OPTION380_DATA] & 0xF0) == 0xF0)
		{
			itype = -1;
		}

		if(_type == ITEMGET(14,17) || _type == ITEMGET(14,18))
		{
			if(lpMsg->dbInventory[n*ItemDbByte+DBI_DUR]==0)
			{
				itype = -1;
			}
		}

		if(_type == ITEMGET(14,19))
		{
			if(lpMsg->dbInventory[n*ItemDbByte+DBI_DUR]==(BYTE)-1)
			{
				itype = -1;
			}
		}

		//LogAddTD("Set Bot Pet ITYPE: %d / %d",n,itype);

		if(itype != -1)
		{
			item.m_Level = DBI_GET_LEVEL(lpMsg->dbInventory[n*ItemDbByte+DBI_OPTION_DATA]);

			if(_type != ITEMGET(14,11))
			{
				if(item.m_Level > MAX_ITEM_LEVEL)
				{
					item.m_Level = MAX_ITEM_LEVEL;
				}
			}

			OptionData = lpMsg->dbInventory[n*ItemDbByte+DBI_OPTION_DATA];
			item.m_SkillOption = DBI_GET_SKILL(OptionData);
			item.m_LuckOption = DBI_GET_LUCK(OptionData);
			item.m_Z28Option = DBI_GET_OPTION(OptionData);

			if(_type == ITEMGET(13,3))
			{
				item.m_Z28Option |= DBI_GET_OPTION16(lpMsg->dbInventory[n*ItemDbByte+DBI_NOPTION_DATA]) >> 4;
			}
			else
			{
				if ( DBI_GET_OPTION16(lpMsg->dbInventory[n*ItemDbByte+DBI_NOPTION_DATA]) == 0x40 )
					item.m_Z28Option += 4;	// Set +24
			}

			item.m_Durability = lpMsg->dbInventory[n*ItemDbByte+DBI_DUR];

			item.m_JewelOfHarmonyOption = lpMsg->dbInventory[n*ItemDbByte+DBI_JOH_DATA];
			item.m_ItemOptionEx = DBI_GET_380OPTION(lpMsg->dbInventory[n*ItemDbByte+DBI_OPTION380_DATA]);

			if(item.m_ItemOptionEx)
			{
				item.m_Type = itype;
				if(g_kItemSystemFor380.Is380Item(&item)==false)
				{
					item.m_ItemOptionEx = 0;
					LogAddTD("[380Item][%s][%s] Invalid 380 Item Option in Inventory pos[%d]",
						lpObj->AccountID,lpObj->Name,n);
				}
			}

			if(IsSlotItem(_type))
			{
				item.m_ItemSlot1 = lpMsg->dbInventory[n*ItemDbByte+11];
				item.m_ItemSlot2 = lpMsg->dbInventory[n*ItemDbByte+12];
				item.m_ItemSlot3 = lpMsg->dbInventory[n*ItemDbByte+13];
				item.m_ItemSlot4 = lpMsg->dbInventory[n*ItemDbByte+14];
				item.m_ItemSlot5 = lpMsg->dbInventory[n*ItemDbByte+15];
			} else {
				item.m_ItemSlot1 = 0;
				item.m_ItemSlot2 = 0;
				item.m_ItemSlot3 = 0;
				item.m_ItemSlot4 = 0;
				item.m_ItemSlot5 = 0;
			}

			item.Convert(itype,item.m_SkillOption,item.m_LuckOption,item.m_Z28Option,DBI_GET_NOPTION(lpMsg->dbInventory[n*ItemDbByte+DBI_NOPTION_DATA]),lpMsg->dbInventory[n*ItemDbByte+DBI_SOPTION_DATA],item.m_ItemOptionEx,3);

			if(_type == ITEMGET(14,7))
			{

			}
			else
			{
				if( (_type >= ITEMGET(14,0)  && _type <= ITEMGET(14,8)) || 
					(_type >= ITEMGET(14,35) && _type <= ITEMGET(14,40))||
					(_type >= ITEMGET(14,70) && _type <= ITEMGET(14,71))||	//Elite Healing/Mana Potions
					(_type == ITEMGET(14,94)) ||							//Elite Medium Healing Potion
					(_type == ITEMGET(14,133))								//Elite SD Potion
					)
				{
					if(item.m_Durability == 0.0f)
					{
						item.m_Durability = 1.0f;
					}

					if(item.m_Durability > 255.0f) //3.0f ERROR POTION FIX
					{
						item.m_Durability = 255.0f;
					}
				}
				else if(_type >= ITEMGET(14,46) && _type <= ITEMGET(14,50))
				{
					if(item.m_Durability == 0.0f)
					{
						item.m_Durability = 1.0f;
					}

					if(item.m_Durability > 3.0f)
					{
						item.m_Durability = 3.0f;
					}
				}
				//else if(_type >= ITEMGET(14,35) && _type <= ITEMGET(14,37))
				//{
				//	if(item.m_Durability == 0.0f)
				//	{
				//		item.m_Durability = 1.0f;
				//	}

				//	if(item.m_Durability > 1.0f)
				//	{
				//		item.m_Durability = 1.0f;
				//	}
				//}
			}

			lpObj->pInventory[n].m_SkillOption = item.m_SkillOption;
			lpObj->pInventory[n].m_LuckOption = item.m_LuckOption;
			lpObj->pInventory[n].m_Z28Option = item.m_Z28Option;
			lpObj->pInventory[n].m_JewelOfHarmonyOption = item.m_JewelOfHarmonyOption;
			lpObj->pInventory[n].m_ItemOptionEx = item.m_ItemOptionEx;

			hiWord = ((lpMsg->dbInventory[n*ItemDbByte+DBI_SERIAL2])&0xFF)&0xFF | (((lpMsg->dbInventory[n*ItemDbByte+DBI_SERIAL1])&0xFF)&0xFF) * 256;
			loWord = ((lpMsg->dbInventory[n*ItemDbByte+DBI_SERIAL4])&0xFF)&0xFF | (((lpMsg->dbInventory[n*ItemDbByte+DBI_SERIAL3])&0xFF)&0xFF) * 256;

			item.m_Number = ((loWord &0xFFFF)&0xFFFF) | ((hiWord & 0xFFFF)&0xFFFF) << 16;

			gObjInventoryInsertItemPos(lpObj->m_Index,item,n,0);
			//lpObj->pInventory[n] = item;
		}
	}
}

void PetDbSkillSetInByte(LPOBJ lpObj, struct SDHP_GETPETBOT* lpMsg)
{
	for (int n=0;n<MAX_MAGIC;n++)
	{
		int itype = MAKEWORD(lpMsg->dbMagicList[n*3],lpMsg->dbMagicList[n*3+1]);

		if (( itype != (WORD)-1 ) && ( itype != (BYTE)-1 ))
		{			
			lpObj->Magic[n].Set(itype,1);
			lpObj->MagicCount++;
		}
	}
}

void GDSetPetBot(int aIndex)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	int result = gObj[aIndex].BotNumOwner;

	if(gObj[aIndex].BotNumOwner >= OBJ_MAXMONSTER)
	{
		if (gObj[result].BotNumOwner != aIndex)
		{
			LogAddTD("[%s][%s] BotPet pointer is incorrect : [%d][%s]",
				gObj[aIndex].AccountID, gObj[aIndex].Name,
				result, gObj[result].Name);

			return;
		}

		if (gObj[result].IsBot != 1)
		{
			LogAddTD("[%s][%s] Is NOT a BotPet, skip save : [%s] [%d:%d,%d]",
				gObj[aIndex].AccountID, gObj[aIndex].Name,
				gObj[result].Name,
				aIndex,gObj[aIndex].BotNumOwner,gObj[aIndex].m_RecallMon );

			return;
		}

		SDHP_GETPETBOT lpMsg;
		PHeadSetW((LPBYTE)&lpMsg, SCFExDB_GSSend_SetPetBot, sizeof(lpMsg));
		lpMsg.Result = 1;

		strcpy(lpMsg.AccountID,gObj[aIndex].AccountID);
		strcpy(lpMsg.Name,gObj[result].Name);
		lpMsg.aIndex = gObj[aIndex].m_Index;
		lpMsg.Class = gObj[result].DbClass;
		lpMsg.Level = gObj[result].Level;
		lpMsg.Exp = gObj[result].Experience;

		lpMsg.Life = gObj[result].Life;
		lpMsg.MaxLife = gObj[result].MaxLife+gObj[result].AddLife;

		lpMsg.Mana = gObj[result].Mana;
		lpMsg.MaxMana = gObj[result].MaxMana + gObj[result].AddMana;

		lpMsg.MapNumber = gObj[result].MapNumber;
		lpMsg.MapX = gObj[result].X;
		lpMsg.MapY = gObj[result].Y;
		lpMsg.Power = gObj[result].BotPower;
		lpMsg.Defense = gObj[result].BotDefense;
		lpMsg.BotLife = gObj[result].BotLife;
		lpMsg.BotMaxLife = gObj[result].BotMaxLife;
		lpMsg.BotLvlUpDefense = gObj[result].BotLvlUpDefense;
		lpMsg.BotLvlUpPower = gObj[result].BotLvlUpPower;
		lpMsg.BotLvlUpLife = gObj[result].BotLvlUpLife;
		lpMsg.BotLvlUpMana = gObj[result].BotLvlUpMana;
		lpMsg.BotLvlUpExp = gObj[result].BotLvlUpExp;
		lpMsg.BotLvlUpMaxLevel = gObj[result].BotLvlUpMaxLevel;
		lpMsg.BotVersion = gObj[result].BotVersion;


		ItemByteConvert16((LPBYTE)lpMsg.dbInventory , gObj[result].pInventory, INVENTORY_NORMAL_SIZE);
		MagicByteConvert(lpMsg.dbMagicList, gObj[result].Magic, MAGIC_SIZE);

		cDBSMng.Send((char*)&lpMsg, sizeof(lpMsg));
	} else {
		if (gObj[aIndex].BotNumOwner != -1)
		{
			LogAddTD("[%s][%s] BotPet out of range, skip save : [ID:%d][PID:%d:%d,%d]",
				gObj[aIndex].AccountID, gObj[aIndex].Name,
				result,aIndex,gObj[aIndex].BotNumOwner,gObj[aIndex].m_RecallMon );
		}
	}
}

void GDAskPetBot(int aIndex, char* AccountID)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	if ( strlen(AccountID) < 1 )
	{
		return;
	}

	SDHP_GETACCOUNTINFO	pMsg = {0};

	PHeadSetB((LPBYTE)&pMsg, SCFExDB_GSSend_AskPetBot, sizeof(pMsg));
	pMsg.aIndex = aIndex;
	memcpy(pMsg.AccountID, AccountID, MAX_ACCOUNT_LEN);

	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}

void GDGetPetBot(SDHP_GETPETBOT * lpMsg)
{
	if(lpMsg->Result != 0)
	{	
		int aIndex = lpMsg->aIndex;
		if( aIndex < OBJ_STARTUSERINDEX || aIndex > OBJMAX-1)
		{
			return;
		}
		if(gObj[aIndex].m_RecallMon >= 0)
		{
			GCRecallMonLife(aIndex,60,0);
			gObjMonsterCallKill(aIndex);
		}

		if (lpMsg->Class < 0)
			lpMsg->Class = 0;
		if (lpMsg->BotLife < 0)
			lpMsg->BotLife = 0;
		if (lpMsg->BotLvlUpDefense < 0)
			lpMsg->BotLvlUpDefense = 1;
		if (lpMsg->BotLvlUpExp < 0)
			lpMsg->BotLvlUpExp = 1;
		if (lpMsg->BotLvlUpLife < 0)
			lpMsg->BotLvlUpLife = 1;
		if (lpMsg->BotLvlUpMana < 0)
			lpMsg->BotLvlUpMana = 1;
		if (lpMsg->BotLvlUpMaxLevel < 0)
			lpMsg->BotLvlUpMaxLevel = 1;
		if (lpMsg->BotLvlUpPower < 0)
			lpMsg->BotLvlUpPower = 1;
		if (lpMsg->BotMaxLife < 0)
			lpMsg->BotMaxLife = 1;


		int Class = lpMsg->Class >> 4;
		LPOBJ lpObj = &gObj[lpMsg->aIndex];
		//int result = gObjAddMonster(lpObj->MapNumber);
		int result = gObjAddCallMon();

		if(result >= 0)
		{
			gObj[result].m_PosNum = (WORD)-1;
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
			gObj[result].IsBot = 1;
			gObjSetMonster(result,Class,"PetBot");

			gObj[result].ChangeUP = lpMsg->Class & 0x07;	// Set Second Type of Character
			gObj[result].Class = Class;
			gObj[result].Level = lpMsg->Level;
			gObj[result].Life = lpMsg->Life;
			gObj[result].MaxLife = lpMsg->MaxLife;
			gObj[result].Mana = lpMsg->Mana;
			gObj[result].MaxMana = lpMsg->MaxMana;
			gObj[result].Experience = lpMsg->Exp;
			gObj[result].DbClass = lpMsg->Class;

			//if (gObj[result].MasterCharacterInfo == NULL)
			//	gObj[result].MasterCharacterInfo = new MASTERCHARINFO;
			//if (gObj[result].Trade == NULL)
				gObj[result].Trade = new CItem[TRADE_BOX_SIZE];
			//if (gObj[result].TradeMap == NULL)
				gObj[result].TradeMap = new BYTE[TRADE_BOX_SIZE];
			memset(gObj[result].TradeMap, (BYTE)-1, TRADE_BOX_SIZE);
			
		//	if (gObj[result].pInventory == NULL)
				gObj[result].pInventory = new CItem[INVENTORY_NORMAL_SIZE];
		//	if (gObj[result].pInventoryMap == NULL)
				gObj[result].pInventoryMap = new BYTE[INVENTORY_NORMAL_SIZE];
			memset(&gObj[result].pInventoryMap[0], (BYTE)-1, INVENTORY_NORMAL_SIZE);

			for (int i=0;i<INVENTORY_NORMAL_SIZE;i++)
			{
				gObj[result].pInventory[i].Clear();
			}

			strcpy(gObj[result].Name,lpMsg->Name);
			//memcpy(gObj[result].pInventory,lpMsg->dbInventory,sizeof(gObj[result].pInventory));

			PetDbItemSetInByte(&gObj[result],lpMsg);
			PetDbSkillSetInByte(&gObj[result],lpMsg);
			if ( lpObj->pInventory[8].m_Type == ITEMGET(13,37) )
			{
				gObj[result].Magic[gObj[result].MagicCount].Set(76,1);
				gObj[result].MagicCount++;
			}
			gObjMakePreviewCharSet(result);
			//memcpy(gObj[result].CharSet,gObj[aIndex].CharSet,sizeof(gObj[result].CharSet));
			gObj[result].BotNumOwner = aIndex;
			if(gObj[result].MagicCount > 0)
			{
				gObj[result].m_AttackType = gObj[result].Magic[0].m_Skill;
				gObj[result].m_SkillNumber = gObj[result].Magic[0].m_Skill;
				gObj[result].UseMagicNumber = gObj[result].Magic[0].m_Skill;
				if(gObj[result].m_AttackRange >= 0 && gObj[result].m_AttackRange <= 1)
					gObj[result].m_AttackRange = 2;
				gObj[result].BotSkillAttack = 1;
			}
			else
			{
				gObj[result].m_AttackType = 0;
				gObj[result].BotSkillAttack = 0;			
				if((gObj[result].pInventory[0].m_Type >= ITEMGET(4,0) && gObj[result].pInventory[0].m_Type < ITEMGET(5,0)) || (gObj[result].pInventory[1].m_Type >= ITEMGET(4,0) && gObj[result].pInventory[1].m_Type < ITEMGET(5,0)))
					gObj[result].m_AttackRange = 6;
			}

			gObj[result].m_RecallMon = aIndex;
			gObj[result].m_Attribute = 100;
			gObj[result].TargetNumber = (WORD)-1;
			gObj[result].m_ActState.Emotion = 0;
			gObj[result].m_ActState.Attack = 0;
			gObj[result].m_ActState.EmotionCount = 0;
			gObj[result].PathCount = 0;
			gObj[result].BotPower = lpMsg->Power;
			gObj[result].BotDefense = lpMsg->Defense;
			gObj[result].BotLife = lpMsg->BotLife;
			gObj[result].BotMaxLife = lpMsg->BotMaxLife;
			
			gObj[result].BotLvlUpDefense = lpMsg->BotLvlUpDefense;
			gObj[result].BotLvlUpPower = lpMsg->BotLvlUpPower;
			gObj[result].BotLvlUpLife = lpMsg->BotLvlUpLife;
			gObj[result].BotLvlUpMana = lpMsg->BotLvlUpMana;
			gObj[result].BotLvlUpExp = lpMsg->BotLvlUpExp;
			gObj[result].BotLvlUpMaxLevel = lpMsg->BotLvlUpMaxLevel;
			gObj[result].BotVersion = lpMsg->BotVersion;

			gObj[aIndex].m_RecallMon = result;
			gObj[aIndex].BotNumOwner = result;
			gObj[result].m_MoveRange = 10;
			gObj[result].BotFollowMe = 1;
			gObj[result].NextExp = gLevelExperience[gObj[result].Level];

			gObj[aIndex].HaveBot = 1;

			botPet.CalcStats(result);

			botPet.SendInfo(aIndex);
			GCRecallMonLife(gObj[result].m_RecallMon,gObj[result].MaxLife,gObj[result].Life);

			ChatSend(&gObj[result],"I'm here to protect you!");
			GCActionSend(&gObj[result],AT_SALUTE1,result,aIndex);
		}
	}
}

void GDCreatePetBot(int aIndex,int BotNum,char * Name)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	SDHP_PETBOTCREATERESULT pMsg = {0};
	PHeadSetB((LPBYTE)&pMsg, SCFExDB_GSSend_CreatePetBot, sizeof(pMsg));
	pMsg.aIndex = aIndex;
	pMsg.BotPetNum = BotNum;
	pMsg.result = 0;
	strcpy(pMsg.Name,Name);

	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}

void GDCreateResultPetBot(SDHP_PETBOTCREATERESULT * aRecv)
{
	if(aRecv->result == 1)
	{
		if(gObjIsConnected(aRecv->aIndex))
		{
			char sbuf[512]={0};
			int makebot = botPet.Make(aRecv->aIndex,aRecv->BotPetNum,aRecv->Name);
			if(makebot == 1)
			{
				LogAddTD("[BotPet] (%s)(%s) Index:%d Created",gObj[aRecv->aIndex].AccountID,aRecv->Name,aRecv->BotPetNum);
				wsprintf(sbuf,"%s BotPet Created!",aRecv->Name);
				GCServerMsgStringSend(sbuf, aRecv->aIndex, 1);
			}else
			{
				LogAddTD("[BotPet] (%s) Creation Failed",gObj[aRecv->aIndex].AccountID);
				GCServerMsgStringSend("You cant create a BotPet right now, try it later", aRecv->aIndex, 1);
			}
		}
	}else
	{
		GCServerMsgStringSend("botPet name already in use!", aRecv->aIndex, 1);
	}
}
#endif

void GJSetPlayerMasterSkillInfo(SDHP_GETPLAYERMASTERSKILL_SAVE * lpMsg)
{
	char szId[11];
	int aIndex = 0;
	//PMSG_TALKRESULT pResult = {0};

	szId[MAX_ACCOUNT_LEN]=0;
	memcpy(szId, lpMsg->AccountID, sizeof(lpMsg->AccountID));
	aIndex = lpMsg->aIndex;

	if ( !gObjIsAccontConnect(aIndex, szId))
	{
		LogAddC(2, lMsg.Get(MSGGET(1, 175)), szId, aIndex);
		return;
	}
	
	LPOBJ lpObj = &gObj[aIndex];

	if (( lpObj->m_Index == aIndex) && (!strcmp(lpMsg->Name,lpObj->Name)))
	{
		for (int i=0;i<MAX_SKILLTREE;i++)
		{
			//memcpy(lpObj->CustomQuest, lpMsg->CustomQuest, 255);
			lpObj->MasterCharacterInfo->Skill[i].Type = lpMsg->Skill[i].Type;
			lpObj->MasterCharacterInfo->Skill[i].Level = lpMsg->Skill[i].Level;
		}
		if ( lpObj->Type == OBJ_USER && gObjIsNewClass(lpObj) == 1)
			Mastering2.GCMagicListMultiSend(lpObj->m_Index,true);

		gObjCalCharacter(lpObj->m_Index);

		lpObj->PlayerSkillTreeInfoReceived = 1;
	}
}


void DGGetPlayerExtraInfo(SDHP_GETPLAYEREXTRAINFO_SAVE * lpMsg)
{
	char szId[11];
	int aIndex = 0;
	//PMSG_TALKRESULT pResult = {0};

	szId[MAX_ACCOUNT_LEN]=0;
	memcpy(szId, lpMsg->AccountID, sizeof(lpMsg->AccountID));
	aIndex = lpMsg->aIndex;

	if ( !gObjIsAccontConnect(aIndex, szId))
	{
		LogAddC(2, lMsg.Get(MSGGET(1, 175)), szId, aIndex);
		return;
	}
	
	LPOBJ lpObj = &gObj[aIndex];

	if (( lpObj->m_Index == aIndex) && (!strcmp(lpMsg->Name,lpObj->Name)))
	{
		lpObj->PlayerExtraInfoReceived = 1;

		char sBuf[255]={0};
		lpObj->MasterCharacterInfo->MasterLevel = lpMsg->MasterLevel;
		lpObj->MasterCharacterInfo->MasterPoints = lpMsg->MasterPoints;
		if(ReadConfig.S6E2 == 1)
		{
			if(lpObj->MasterCharacterInfo->MasterPointsTmp > 0)
			{
				lpObj->MasterCharacterInfo->MasterPoints += lpObj->MasterCharacterInfo->MasterPointsTmp;
				lpObj->PlayerExtraInfoModified = 1;
				wsprintf(sBuf, "[Mastering] Restored %d Master Points",lpObj->MasterCharacterInfo->MasterPointsTmp);
				GCServerMsgStringSend (sBuf,lpObj->m_Index,0x01 ) ;
			}
		}
		lpObj->Resets = lpMsg->Resets;
		lpObj->PCPoints = lpMsg->PCPoints;
		
		lpObj->m_wItem_MuShop_Seal_EffectType = lpMsg->CShop_SealItem;
		lpObj->m_iItem_MuShop_Seal_EffectValidTime = lpMsg->CShop_SealTime;		
		lpObj->m_wItem_MuShop_Scroll_EffectType = lpMsg->CShop_ScrollItem;
		lpObj->m_iItem_MuShop_Scroll_EffectValidTime = lpMsg->CShop_ScrollTime;
#if(ENABLE_CSHOP == 1)
		MuItemShop.SetBuff(aIndex);
#endif
		LogAddTD("[%s][%s] PlayerExtraInfoGet : Resets=%d PCPoints=%d MasterLvl=%d MasterPnt=%d",
			lpObj->AccountID,lpObj->Name,
			lpObj->Resets,lpObj->PCPoints,
			lpObj->MasterCharacterInfo->MasterLevel,lpObj->MasterCharacterInfo->MasterPoints
		);


#if (PACK_EDITION>=2)
		lpObj->MarryCharacterInfo->Married = lpMsg->Married;
		memcpy(lpObj->MarryCharacterInfo->MarriedName, lpMsg->MarriedName, MAX_ACCOUNT_LEN);
		if (lpObj->MarryCharacterInfo->Married == 1)
		{
			wsprintf(sBuf, "[Marry] Married to %s",lpObj->MarryCharacterInfo->MarriedName);
			GCServerMsgStringSend (sBuf,lpObj->m_Index,0x01 ) ;
		}

		LogAddTD("[%s][%s] PlayerMarryInfoGet : isMarry=%d Wife=%s",
			lpObj->AccountID,lpObj->Name,
			lpObj->MarryCharacterInfo->Married,lpObj->MarryCharacterInfo->MarriedName
		);
#endif

		lpObj->GensFam = lpMsg->Family;
		lpObj->GensCont = lpMsg->Contribution;
		lpObj->GensRank = lpMsg->Rank;
		lpObj->GensNotAllow = lpMsg->NotAllow;
		lpObj->RecvRewItem = lpMsg->RecvRewItem;

		if(Gens.AllowJoinIfGuildMemberIsInOtherGens == 0)
		{
			if(lpObj->lpGuild != NULL)
			{
				if ( strcmp(lpObj->Name, lpObj->lpGuild->Names[0] ) )
				{
					if(lpObj->GensFam != 0 && lpObj->lpGuild->GensFam != 0)
					{
						if(lpObj->GensFam != lpObj->lpGuild->GensFam)
						{
							GDGuildMemberDel(lpObj->m_Index, lpObj->lpGuild->Name, lpObj->Name);
							GCServerMsgStringSend("You leave the guild because GM has joined into other Gens family!",aIndex, 1);
						}
					}
				}
			}
		}

		Gens.Ranking(aIndex);

		if(lpObj->GensFam != 0)
		{
			SYSTEMTIME sysTime;
			GetLocalTime(&sysTime);
			if(sysTime.wDay >= 1 && sysTime.wDay <= 7)
			{
				GCServerMsgStringSend("Gens ranking reward is given today. Check with the gens NPC if you are eligible.",aIndex, 1);
			}
		}

		LogAddTD("[%s][%s] PlayerGensInfoGet : GensFam=%d GensCont=%d GensRank=%d GensNotAllow=%d RecvRewItem=%d",
			lpObj->AccountID,lpObj->Name,
			lpObj->GensFam,lpObj->GensCont,lpObj->GensRank,lpObj->GensNotAllow,lpObj->RecvRewItem
		);

#if (PACK_EDITION>=3)
		if(GensCloseMap.Enabled == 1)
		{
			if(GensCloseMap.CheckMap(lpObj->m_Index,lpObj->MapNumber) == true)
			{
				GCServerMsgStringSend(lMsg.Get(MSGGET(14, 148)),lpObj->m_Index, 0x01);
				gObjMoveGate(lpObj->m_Index,17);
			}
		}
#endif

		wsprintf(sBuf, "[ResetSystem] Resets: %d", lpObj->Resets);
		GCServerMsgStringSend (sBuf,lpObj->m_Index,0x01 );

#if (PACK_EDITION>=2)
		VipSystem.State(lpObj);
#endif

		//Display Extra Experience Info
		gObjGetExtraExperienceInfoMsg(lpObj->m_Index);

		Mastering.InfoSend(aIndex);

#if (PACK_EDITION>=2)
		PCShop.SendPoints(lpObj);
#endif
#if (CRYSTAL_EDITION == 1)
#if (PACK_EDITION>=2)
		Marry.SendInfo(lpObj->m_Index);
#endif
#endif
		gObjCalCharacter(lpObj->m_Index);
	}

#if (ENABLE_CSHOP == 1)
	for(int n = 0; n < ReadConfig.INVENTORY_SIZE(lpObj->m_Index,false);n++)
	{
		MuItemShop.ItemSendTime(aIndex,n);
	}
#endif
}


//Player Gens Info

void GensGuildFamily(char * GuildName)
{
	SDHP_GENSGUILD_INFO_REQUEST pMsg;

	pMsg.h.set((LPBYTE)&pMsg, SCFExDB_GSSend_GuildFamily, sizeof(pMsg));
	memcpy(pMsg.GuildName, GuildName, sizeof(pMsg.GuildName));

	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}

void DGGetGuildGensInfo(SDHP_GENSGUILD_INFO_GET * lpMsg)
{	
	_GUILD_INFO_STRUCT * pNode = Guild.SearchGuild(lpMsg->GuildName);

	if (pNode == NULL )
	{
		return;
	}

	pNode->GensFam = lpMsg->Family;

	if(Gens.AllowJoinIfGuildMemberIsInOtherGens == 0)
	{
		for ( int n=1;n<MAX_USER_GUILD;n++)
		{
			if ( pNode->Use[n] == TRUE )
			{
				int gpIndex = gObjGetIndex(pNode->Names[n]);
				if ( gpIndex != -1 )
				{
					if(gObj[gpIndex].GensFam != 0 && pNode->GensFam != 0)
					{
						if(gObj[gpIndex].GensFam != pNode->GensFam)
						{
							GDGuildMemberDel(gpIndex, pNode->Name, gObj[gpIndex].Name);
							GCServerMsgStringSend("You leave the guild because GM has joined into other Gens family!", gpIndex, 1);
						}
					}
				}
			}
		}
	}
}

void GensLeaveFamily(int aIndex)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	SDHP_SETPLAYERGENSLEAVEINFO_SAVE pMsg;

	pMsg.h.set((LPBYTE)&pMsg, SCFExDB_GSSend_GensLeaveFamily, sizeof(pMsg));
	memcpy(pMsg.Name, gObj[aIndex].Name, sizeof(pMsg.Name));
	memcpy(pMsg.AccountID, gObj[aIndex].AccountID, sizeof(pMsg.AccountID));

	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}

void JGSetPlayerExtraInfo(LPOBJ lpObj)
{
	if (lpObj->PlayerExtraInfoModified == 1 &&
		lpObj->PlayerExtraInfoReceived == 1 //&&
		//lpObj->PlayerQ5InfoReceived == 1
		)
	{
		lpObj->PlayerExtraInfoModified = 0;

		SDHP_GETPLAYERINFO_INFOSAVE pMsg = {0};
		PHeadSetB((LPBYTE)&pMsg, SCFExDB_GSSend_SetPlayerExtraInfo, sizeof(pMsg));

		memcpy(pMsg.Name, lpObj->Name, MAX_ACCOUNT_LEN);
		pMsg.MasterLevel = lpObj->MasterCharacterInfo->MasterLevel;
		pMsg.MasterPoints = lpObj->MasterCharacterInfo->MasterPoints;
		pMsg.Resets = lpObj->Resets;
		pMsg.PCPoints = lpObj->PCPoints;
		
		pMsg.CShop_SealItem	= lpObj->m_wItem_MuShop_Seal_EffectType;
		pMsg.CShop_SealTime	= lpObj->m_iItem_MuShop_Seal_EffectValidTime;		
		pMsg.CShop_ScrollItem = lpObj->m_wItem_MuShop_Scroll_EffectType;
		pMsg.CShop_ScrollTime = lpObj->m_iItem_MuShop_Scroll_EffectValidTime;

		//pMsg.ExInventory = lpObj->ExInventory;

		LogAddTD("[%s][%s] PlayerExtraInfoSave : Resets=%d PCPoints=%d MasterLvl=%d MasterPnt=%d",
			lpObj->AccountID,lpObj->Name,
			lpObj->Resets,lpObj->PCPoints,
			lpObj->MasterCharacterInfo->MasterLevel,lpObj->MasterCharacterInfo->MasterPoints
		);

#if (PACK_EDITION>=2)
		pMsg.Married = lpObj->MarryCharacterInfo->Married;
		memcpy(pMsg.MarriedName, lpObj->MarryCharacterInfo->MarriedName, MAX_ACCOUNT_LEN);

		LogAddTD("[%s][%s] PlayerMarryInfoSave : isMarry=%d Wife=%s",
			lpObj->AccountID,lpObj->Name,
			lpObj->MarryCharacterInfo->Married,lpObj->MarryCharacterInfo->MarriedName
		);
#endif

		pMsg.GensFamily = lpObj->GensFam;
		pMsg.GensCont = lpObj->GensCont;
		pMsg.GensNotAllow = lpObj->GensNotAllow;
		pMsg.GensRecvRewItem = lpObj->RecvRewItem;

		LogAddTD("[%s][%s] PlayerGensInfoSave : GensFam=%d GensCont=%d GensRank=%d GensNotAllow=%d RecvRewItem=%d",
			lpObj->AccountID,lpObj->Name,
			lpObj->GensFam,lpObj->GensCont,lpObj->GensRank,lpObj->GensNotAllow,lpObj->RecvRewItem
		);

		cDBSMng.Send((char*)&pMsg, sizeof(pMsg));

	} else 
	{
		if (lpObj->PlayerExtraInfoReceived != 1 //||
			//lpObj->PlayerQ5InfoReceived != 1
			)
		{
			GCServerMsgStringSend ("ERROR: ReRequesting character info",lpObj->m_Index, 0x00);
			LogAddC(2, "ReRequesting: PROTOCOL Info [%s][%s] Line: %d", lpObj->AccountID, lpObj->Name, __LINE__);
		}
	}

	if (lpObj->PlayerSkillTreeInfoModified == 1)
	{
		lpObj->PlayerSkillTreeInfoModified = 0;

		if(ReadConfig.S6E2 == 1)
		{
			SDHP_GETPLAYERMASTERSKILL_SAVE pMsg3 = {0};
			PHeadSetW((LPBYTE)&pMsg3, SCFExDB_GSSend_SetPlayerSkillTreeS6E2, sizeof(pMsg3));
			memcpy(pMsg3.AccountID, lpObj->AccountID, MAX_ACCOUNT_LEN);
			memcpy(pMsg3.Name, lpObj->Name, MAX_ACCOUNT_LEN);

			for (int i=0;i<MAX_SKILLTREE;i++)
			{
				pMsg3.Skill[i].Type = lpObj->MasterCharacterInfo->Skill[i].Type;
				pMsg3.Skill[i].Level = lpObj->MasterCharacterInfo->Skill[i].Level;

				LogAddTD("[%s][%s][%d] Skill: %d / Level: %d",
					lpObj->AccountID,lpObj->Name,
					i, pMsg3.Skill[i].Type, pMsg3.Skill[i].Level 
				);
			}
		
			cDBSMng.Send((char*)&pMsg3, sizeof(pMsg3));
		}
	}
	

	if (lpObj->PlayerQ5InfoModified == 1)
	{
		lpObj->PlayerQ5InfoModified = 0;

		if(ReadConfig.S5E2 == 1)
		{
			SetPlayerS5QuestInfo(lpObj->m_Index);
		}
	}

	JGSetOfficialHelper(lpObj);
}


void JGSetPlayerMarryDivorseInfo(char * Name)
{
	SDHP_SETMARRYDIVORSE_INFOSAVE pMsg = {0};
	PHeadSetB((LPBYTE)&pMsg, SCFExDB_GSSend_SetPlayerMarryInfo, sizeof(pMsg));
	memcpy(pMsg.Name, Name, MAX_ACCOUNT_LEN);
	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}


void DGGetAccountExtraInfo(SDHP_GETACCOUNTEXTRAINFO_SAVE * lpMsg)
{
	char szId[11];
	int aIndex = 0;
	//PMSG_TALKRESULT pResult = {0};

	szId[MAX_ACCOUNT_LEN]=0;
	memcpy(szId, lpMsg->AccountID, sizeof(lpMsg->AccountID));
	aIndex = lpMsg->aIndex;

	if ( !gObjIsAccontConnect(aIndex, szId))
	{
		LogAddC(2, lMsg.Get(MSGGET(1, 175)), szId, aIndex);
		return;
	}
	
	LPOBJ lpObj = &gObj[aIndex];

	if (( lpObj->m_Index == aIndex) && (!strcmp(lpMsg->AccountID,lpObj->AccountID)))
	{
		lpObj->Vip = lpMsg->Vip;
		lpObj->VipMoney = lpMsg->VipMoney;
		lpObj->VipDays = lpMsg->VipDays;
		lpObj->WarehouseVipCount = lpMsg->WarehouseVipCount;
		lpObj->LuckyCoins = lpMsg->LuckyCoins;
		lpObj->ExWarehouse = lpMsg->ExWarehouse;
#if (PACK_EDITION>=3)
		lpObj->HaveBot = lpMsg->HaveBot;
		lpObj->LotteryCnt = lpMsg->LotteryCnt;
#endif
#if (CRYSTAL_EDITION == 1)
#if (PACK_EDITION>=3)
		SVShop.SendInfo(aIndex);
#endif
#endif

		if(gObj[aIndex].Vip == 0 
#if (PACK_EDITION>=2)
			&& VipSystem.IsVipServer == 1
#endif
			)
		{				
			LogAddTD("[%s][NonVIP] User Disconnection - No VIP User",lpObj->AccountID);
			GCServerMsgStringSend("This Server is only for VIP",aIndex, 0x00);
			gObjCloseSet(aIndex, 2);
		}

		lpObj->AccountExtraInfoReceived = 1;
	}

	LogAddTD("[%s][ExtraAccount] Vip:%d VipDays:%d VipMoney:%d VipWareEx:%d",lpObj->AccountID,lpObj->Vip,lpObj->VipDays,lpObj->VipMoney,lpMsg->WarehouseVipCount);
#if (PACK_EDITION>=3)
	LogAddTD("[%s][ExtraAccount] Have Bot Status:%d",lpObj->AccountID,lpObj->HaveBot);
#endif
}

void JGSetAccountExtraInfo(LPOBJ lpObj)
{
	if (lpObj->AccountExtraInfoModified == 1 && 
		lpObj->AccountExtraInfoReceived == 1)
	{
		lpObj->AccountExtraInfoModified = 0;

		if(lpObj->ExWarehouseTmp > 0)
			lpObj->ExWarehouse = lpObj->ExWarehouseTmp;

		SDHP_GETACCOUNTEXTRAINFO_SAVE pMsg = {0};
		PHeadSetB((LPBYTE)&pMsg, SCFExDB_GSSend_SetAccountExtraInfo, sizeof(pMsg));

		memcpy(pMsg.AccountID, lpObj->AccountID, MAX_ACCOUNT_LEN);
		pMsg.aIndex = lpObj->m_Index;
		pMsg.Vip = lpObj->Vip;
		pMsg.VipMoney = lpObj->VipMoney;
		pMsg.VipDays = lpObj->VipDays;
		pMsg.WarehouseVipCount = lpObj->WarehouseVipCount;
#if (PACK_EDITION>=3)
		pMsg.HaveBot = lpObj->HaveBot;
		pMsg.LotteryCnt = 0;
#endif
		pMsg.LuckyCoins = lpObj->LuckyCoins;
		pMsg.ExWarehouse = lpObj->ExWarehouse;
		cDBSMng.Send((char*)&pMsg, pMsg.h.size);
	} else 
	{
		if (lpObj->AccountExtraInfoReceived != 1)
		{
			GCServerMsgStringSend ("ERROR: ReRequesting account info",lpObj->m_Index, 0x01);
			LogAddC(2, "ReRequesting: PROTOCOL Info [%s] Line: %d", lpObj->AccountID, __LINE__);
		}
	}
}



void GetPlayerExtraInfo(int aIndex, BYTE Value)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	SDHP_GETPLAYEREXTRAINFO pMsg = {0};
	PHeadSetB((LPBYTE)&pMsg, Value, sizeof(pMsg));
	memcpy(pMsg.AccountID, gObj[aIndex].AccountID, MAX_ACCOUNT_LEN);
	memcpy(pMsg.Name, gObj[aIndex].Name, MAX_ACCOUNT_LEN);
	pMsg.aIndex = aIndex;
	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}

//AccountExtraInfo


void GJSetStatusBan(char* Name, BOOL Type, BYTE Ban)
{
	SDHP_SETSTATUSBAN_INFOSAVE	pMsg = {0};

	PHeadSetB((LPBYTE)&pMsg, SCFExDB_GSSend_SetStatusBan, sizeof(pMsg));
	pMsg.Ban = Ban;
	pMsg.Type = Type;
	memcpy(pMsg.Name, Name, MAX_ACCOUNT_LEN);

	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}

//WarehouseEx

#if (PACK_EDITION>=2)
void GDSetWarehouseListEx(int aIndex)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	if(ReadConfig.S6E2 == 0)
	{
		SDHP_GETWAREHOUSEDBEx_SAVE pMsg;
		
		if ( gObj[aIndex].LoadWareHouseInfo == false )
		{
			LogAddTD("[%s][%s] WareHouse Save Fail : Not Open",
				gObj[aIndex].AccountID, gObj[aIndex].Name);

			return;
		}

		memcpy(pMsg.AccountID, gObj[aIndex].AccountID, sizeof(pMsg.AccountID));
		pMsg.aIndex = aIndex;
		pMsg.h.c = 0xC2;
		pMsg.h.headcode = SCFExDB_GSSend_SetWarehouseListEx;
		pMsg.h.sizeH = HIBYTE(sizeof(pMsg));
		pMsg.h.sizeL = LOBYTE(sizeof(pMsg));
		pMsg.Money = gObj[aIndex].WarehouseMoney;
		pMsg.pw = gObj[aIndex].WarehousePW;
		pMsg.ExNum = gObj[aIndex].WarehouseExNum;

#if (DSGN_ITEM_DBMOVE == 1)
		pMsg.DbVersion = DS_Mover.highestDBNumber;
#else
		pMsg.DbVersion = 3;
#endif

		memset(pMsg.dbItems,0xFF,sizeof(pMsg.dbItems));
		ItemByteConvert16(pMsg.dbItems, gObj[aIndex].pWarehouse, ReadConfig.WAREHOUSE_SIZE(aIndex,false));

		cDBSMng.Send((PCHAR)&pMsg, sizeof(pMsg));
	}else
	{
		SDHP_GETWAREHOUSEDBEx_S6E2_SAVE pMsg;
		
		if ( gObj[aIndex].LoadWareHouseInfo == false )
		{
			LogAddTD("[%s][%s] WareHouse Save Fail : Not Open",
				gObj[aIndex].AccountID, gObj[aIndex].Name);

			return;
		}

		memcpy(pMsg.AccountID, gObj[aIndex].AccountID, sizeof(pMsg.AccountID));
		pMsg.aIndex = aIndex;
		pMsg.h.c = 0xC2;
		pMsg.h.headcode = SCFExDB_GSSend_SetWarehouseListExS6E2;
		pMsg.h.sizeH = HIBYTE(sizeof(pMsg));
		pMsg.h.sizeL = LOBYTE(sizeof(pMsg));
		pMsg.Money = gObj[aIndex].WarehouseMoney;
		pMsg.pw = gObj[aIndex].WarehousePW;
		pMsg.ExNum = gObj[aIndex].WarehouseExNum;

#if (DSGN_ITEM_DBMOVE == 1)
		pMsg.DbVersion = DS_Mover.highestDBNumber;
#else
		pMsg.DbVersion = 3;
#endif

		memset(pMsg.dbItems,0xFF,sizeof(pMsg.dbItems));
		ItemByteConvert16(pMsg.dbItems, gObj[aIndex].pWarehouse, ReadConfig.WAREHOUSE_SIZE(aIndex,false));

		cDBSMng.Send((PCHAR)&pMsg, sizeof(pMsg));
	}
	gObjSavePetItemInfo(aIndex, 1);
	gObjWarehouseTextSave(&gObj[aIndex]);
	
	//gObj[aIndex].pWarehouse = new CItem[WAREHOUSE_SIZE];
	//gObj[aIndex].pWarehouseMap = new BYTE[WAREHOUSE_SIZE];

	//memset(gObj[aIndex].pWarehouse, (BYTE)-1, WAREHOUSE_SIZE);
	//memset(gObj[aIndex].pWarehouseMap, (BYTE)-1, WAREHOUSE_SIZE);
}


void GDGetWarehouseListEx(int aIndex, char* AccountID)
{
	if ( strlen(AccountID) < 1 )
	{
		return;
	}
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	SDHP_GETWAREHOUSEDBEX	pMsg;

	pMsg.h.c = 0xC1;
	pMsg.h.headcode = SCFExDB_GSSend_GetWarehouseListEx;
	pMsg.h.size = sizeof(SDHP_GETWAREHOUSEDBEX);
	pMsg.aIndex = aIndex;
	pMsg.ExNum = gObj[aIndex].WarehouseExNum;
	pMsg.IsS6E2 = ReadConfig.S6E2;
	memcpy(pMsg.AccountID, AccountID, sizeof(pMsg.AccountID));

	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}

//

void GDGetGuildWarehouseList(int aIndex)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}
	if ( strlen(gObj[aIndex].AccountID) < 1 )
	{
		return;
	}
	if ( strlen(gObj[aIndex].GuildName) < 1 )
	{
		GCServerMsgStringSend("You need to be in a Guild before open GuildWarehouse!",aIndex, 0x01);
		gObj[aIndex].m_ReqWarehouseOpen = false;
		gObj[aIndex].m_IfState.use = 0;
		gObj[aIndex].m_IfState.state = 0;
		return;
	}
	if ( gObj[aIndex].GuildNumber <= 0 )
	{
		GCServerMsgStringSend("You need to be in a Guild before open GuildWarehouse!",aIndex, 0x01);
		gObj[aIndex].m_ReqWarehouseOpen = false;
		gObj[aIndex].m_IfState.use = 0;
		gObj[aIndex].m_IfState.state = 0;
		return;
	}
	if( gObj[aIndex].lpGuild->WareOpen == 1 )
	{
		GCServerMsgStringSend("Some other character is using this GuildWarehouse!",aIndex, 0x01);
		gObj[aIndex].m_ReqWarehouseOpen = false;
		gObj[aIndex].m_IfState.use = 0;
		gObj[aIndex].m_IfState.state = 0;
		return;
	}

	if(ReadConfig.GuildWareOnlyExt != 0)
	{
		if(ReadConfig.S6E2 == 1)
		{
			if(gObj[aIndex].ExWarehouse == 0)
			{
				GCServerMsgStringSend("You need to have Ext Warehouse for open GuildWarehouse!",aIndex, 0x01);
				gObj[aIndex].m_ReqWarehouseOpen = false;
				gObj[aIndex].m_IfState.use = 0;
				gObj[aIndex].m_IfState.state = 0;
				return;
			}
		}
	}

	gObj[aIndex].lpGuild->WareOpen = 1;

	SDHP_GETGUILDWAREHOUSEDB	pMsg;

	pMsg.h.c = 0xC1;
	pMsg.h.headcode = SCFExDB_GSSend_GetGuildWarehouseList;
	pMsg.h.size = sizeof(SDHP_GETGUILDWAREHOUSEDB);
	pMsg.aIndex = aIndex;
	pMsg.IsS6E2 = ReadConfig.S6E2;

	memcpy(pMsg.G_Name, gObj[aIndex].GuildName, sizeof(pMsg.G_Name));
	memcpy(pMsg.AccountID, gObj[aIndex].AccountID, sizeof(pMsg.AccountID));

	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}

void GDDelGuildWarehouseList(int aIndex)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}
	if ( strlen(gObj[aIndex].AccountID) < 1 )
	{
		return;
	}
	if ( strlen(gObj[aIndex].GuildName) < 1 )
	{
		GCServerMsgStringSend("You need to be in a Guild before Del GuildWarehouse!",aIndex, 0x01);
		gObj[aIndex].m_ReqWarehouseOpen = false;
		gObj[aIndex].m_IfState.use = 0;
		gObj[aIndex].m_IfState.state = 0;
		return;
	}
	if ( gObj[aIndex].GuildNumber <= 0 )
	{
		GCServerMsgStringSend("You need to be in a Guild before Del GuildWarehouse!",aIndex, 0x01);
		gObj[aIndex].m_ReqWarehouseOpen = false;
		gObj[aIndex].m_IfState.use = 0;
		gObj[aIndex].m_IfState.state = 0;
		return;
	}

	SDHP_GETGUILDWAREHOUSEDB	pMsg;

	pMsg.h.c = 0xC1;
	pMsg.h.headcode = SCFExDB_GSSend_DelGuildWarehouseList;
	pMsg.h.size = sizeof(SDHP_GETGUILDWAREHOUSEDB);
	pMsg.aIndex = aIndex;

	memcpy(pMsg.G_Name, gObj[aIndex].GuildName, sizeof(pMsg.G_Name));
	memcpy(pMsg.AccountID, gObj[aIndex].AccountID, sizeof(pMsg.AccountID));

	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}

void GDSetGuildWarehouseList(int aIndex)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}
	SDHP_GETGUILDWAREHOUSEDB_SAVE pMsg;
	SDHP_GETGUILDWAREHOUSEDB_S6E2_SAVE pMsg2;

	if ( strlen(gObj[aIndex].AccountID) < 1 )
	{
		return;
	}
	if ( strlen(gObj[aIndex].GuildName) < 1 )
	{
		gObj[aIndex].m_ReqWarehouseOpen = false;
		gObj[aIndex].m_IfState.use = 0;
		gObj[aIndex].m_IfState.state = 0;
		return;
	}
	if ( gObj[aIndex].GuildNumber <= 0 )
	{
		gObj[aIndex].m_ReqWarehouseOpen = false;
		gObj[aIndex].m_IfState.use = 0;
		gObj[aIndex].m_IfState.state = 0;
		return;
	}
	
	if ( gObj[aIndex].LoadWareHouseInfo == false )
	{
		LogAddTD("[%s][%s] WareHouse Save Fail : Not Open",
			gObj[aIndex].AccountID, gObj[aIndex].Name);

		return;
	}

	gObj[aIndex].lpGuild->WareOpen = 0;

	if(ReadConfig.S6E2 == 0)
	{
		memcpy(pMsg.AccountID, gObj[aIndex].AccountID, sizeof(pMsg.AccountID));
		memcpy(pMsg.G_Name, gObj[aIndex].GuildName, sizeof(pMsg.G_Name));
		pMsg.aIndex = aIndex;
		pMsg.h.c = 0xC2;
		pMsg.h.headcode = SCFExDB_GSSend_SetGuildWarehouseList;
		pMsg.h.sizeH = HIBYTE(sizeof(pMsg));
		pMsg.h.sizeL = LOBYTE(sizeof(pMsg));
		pMsg.Money = gObj[aIndex].WarehouseMoney;
		pMsg.pw = gObj[aIndex].WarehousePW;

#if (DSGN_ITEM_DBMOVE == 1)
		pMsg.DbVersion = DS_Mover.highestDBNumber;
#else
		pMsg.DbVersion = 3;
#endif

		memset(pMsg.dbItems,0xFF,sizeof(pMsg.dbItems));
		ItemByteConvert16(pMsg.dbItems, gObj[aIndex].pWarehouse, ReadConfig.WAREHOUSE_SIZE(aIndex,false));

		cDBSMng.Send((PCHAR)&pMsg, sizeof(pMsg));
	}else
	{
		memcpy(pMsg2.AccountID, gObj[aIndex].AccountID, sizeof(pMsg2.AccountID));
		memcpy(pMsg2.G_Name, gObj[aIndex].GuildName, sizeof(pMsg2.G_Name));
		pMsg2.aIndex = aIndex;
		pMsg2.h.c = 0xC2;
		pMsg2.h.headcode = SCFExDB_GSSend_SetGuildWarehouseListS6E2;
		pMsg2.h.sizeH = HIBYTE(sizeof(pMsg2));
		pMsg2.h.sizeL = LOBYTE(sizeof(pMsg2));
		pMsg2.Money = gObj[aIndex].WarehouseMoney;
		pMsg2.pw = gObj[aIndex].WarehousePW;

#if (DSGN_ITEM_DBMOVE == 1)
		pMsg.DbVersion = DS_Mover.highestDBNumber;
#else
		pMsg.DbVersion = 3;
#endif

		memset(pMsg2.dbItems,0xFF,sizeof(pMsg.dbItems));
		ItemByteConvert16(pMsg2.dbItems, gObj[aIndex].pWarehouse, ReadConfig.WAREHOUSE_SIZE(aIndex,false));

		cDBSMng.Send((PCHAR)&pMsg2, sizeof(pMsg2));
	}
	gObjSavePetItemInfo(aIndex, 1);
	gObjWarehouseTextSave(&gObj[aIndex]);
}

//

void GDGetWarehouseNoItemEx( SDHP_GETWAREHOUSEDB_RESULT * lpMsg)
{
	char szId[11];
	int aIndex = lpMsg->aIndex;
	PMSG_TALKRESULT pResult;

	szId[MAX_ACCOUNT_LEN] = 0;
	memcpy(szId, lpMsg->AccountID, sizeof(lpMsg->AccountID));

	if ( !gObjIsAccontConnect(aIndex, szId) )
	{
		LogAddC(2, lMsg.Get(MSGGET(1, 175)), szId, aIndex);
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];
	
	if ( lpObj->m_IfState.type == 6 )
	{
		if ( lpObj->m_IfState.state == 0 )
		{
			lpObj->m_IfState.state = 1;
			lpObj->WarehouseSave = 1;
			pResult.h.c = 0xC3;
			pResult.h.headcode = 0x30;
			pResult.h.size = sizeof(pResult);
			pResult.result = 2;

			DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);
			LogAddC(2, lMsg.Get(MSGGET(1, 176)), lpObj->Name);
		}
	}
	lpObj->WarehouseMoney = 0;
	lpObj->WarehouseExNumLast = lpObj->WarehouseExNum;
	for (int i=0;i<ReadConfig.WAREHOUSE_SIZE(aIndex,false);i++)
	{
		lpObj->pWarehouse[i].Clear();
	}
	GCWarehouseInventoryMoneySend(lpObj->m_Index, 1, lpObj->Money, lpObj->WarehouseMoney);

	lpObj->LoadWareHouseInfo = true;
	lpObj->m_ReqWarehouseOpen = false;
	
	//gObj[aIndex].pWarehouse = new CItem[WAREHOUSE_SIZE];
	//gObj[aIndex].pWarehouseMap = new BYTE[WAREHOUSE_SIZE];
	//memset(gObj[aIndex].pWarehouse, (BYTE)-1, WAREHOUSE_SIZE);
	//memset(gObj[aIndex].pWarehouseMap, (BYTE)-1, WAREHOUSE_SIZE);
}
#endif


void DGGetCleanLuckyCoinsInfo( PMSG_DEFRESULT * lpMsg)
{
	if(lpMsg->result == 1)
	{
		for ( int n = OBJ_STARTUSERINDEX ; n < OBJMAX ; n++)
		{
			if ( gObj[n].Connected >= PLAYER_LOGGED )
			{
				if ( gObj[n].Type == OBJ_USER )
				{
					gObj[n].LuckyCoins = 0;
				}
			}
		}
	}
}

#if (GS_CASTLE==1)
void GS_DGGetHuntZoneValue(SDHP_SETHUNTZONEOPEN * lpMsg)
{
	if (g_MapServerManager.GetMapSvrGroup() == lpMsg->mapServer)
	{
		g_CastleSiege.SetHuntZoneEnter(lpMsg->state);
	} else {
		g_CastleSiege.SetHuntZoneEnter(0);
	}
}

void GS_DGSetHuntZoneOpen()
{		
	SDHP_SETHUNTZONEOPEN pMsg = {0};
	PHeadSubSetB((LPBYTE)&pMsg, SCFExDB_GSSend_SiegeProtocol,0x01,sizeof(pMsg));
	pMsg.state = g_CastleSiege.GetHuntZoneEnter();
	pMsg.mapServer = g_MapServerManager.GetMapSvrGroup();

	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}

void GS_DGGetHuntZoneOpen()
{
	SDHP_GETHUNTZONEOPEN pMsg = {0};
	PHeadSubSetB((LPBYTE)&pMsg, SCFExDB_GSSend_SiegeProtocol,0x00,sizeof(pMsg));
	pMsg.mapServer = g_MapServerManager.GetMapSvrGroup();

	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}
#endif

void GDGetAccounExtraInfo(int aIndex, char* AccountID)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}
	if ( strlen(AccountID) < 1 )
	{
		return;
	}

	SDHP_GETACCOUNTINFO	pMsg = {0};

	PHeadSetB((LPBYTE)&pMsg, SCFExDB_GSSend_GetAccounExtraInfo, sizeof(pMsg));
	pMsg.aIndex = aIndex;
	memcpy(pMsg.AccountID, AccountID, MAX_ACCOUNT_LEN);

	cDBSMng.Send((char*)&pMsg, sizeof(pMsg));
}


#if (PACK_EDITION>=3)
void SetResetLog(int aIndex, BYTE MasterReset)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}
	SDHP_SETRESET_LOG pMsg = {0};
	PHeadSetB((LPBYTE)&pMsg, SCFExDB_GSSend_SetResetLog, sizeof(pMsg));
	memcpy(pMsg.AccountID, gObj[aIndex].AccountID, MAX_ACCOUNT_LEN);
	memcpy(pMsg.Name, gObj[aIndex].Name, MAX_ACCOUNT_LEN);
	pMsg.Reset = gObj[aIndex].Resets;
	pMsg.Type = MasterReset;
	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}
#endif