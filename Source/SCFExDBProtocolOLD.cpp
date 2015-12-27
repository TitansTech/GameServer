#include "stdafx.h"
#include "..\common\MyWinsockBase.h"
#include "wsJoinServerCli.h"
#include "GameServer.h"
#include "GameMain.h"
#include "DBSockMng.h"
#include "SCFExDBProtocol.h"
#include "logproc.h"
#include "..\common\winutil.h"
#include "Mastering.h"
#include "PCShop.h"
#include "CustomQuest.h"
#include "DSProtocol.h"
#include "ObjBotPet.h"
#include "..\common\SetItemOption.h"

wsJoinServerCli wsSCFExDbCli;

void SCFExDataClientMsgProc(WPARAM wParam, LPARAM lParam)
{
	switch ( lParam & 0xFFFF & 0xFFFF )
	{
		case 1:
			wsSCFExDbCli.DataRecv();
			break;

		case 2:
			wsSCFExDbCli.FDWRITE_MsgDataSend();
			break;

		case 32:
			LogAddC(2, "SCF ExDB Close");
			wsSCFExDbCli.Close();
			break;
	}
}


BOOL SCFExDataServerConnect(char* ConnectServer,int SCFExDBPort ,DWORD wMsg)
{
	wsSCFExDbCli.SetProtocolCore(SCFExDataServerProtocolCore);
	int result = wsSCFExDbCli.Connect(ConnectServer, SCFExDBPort, wMsg);

	if ( result == 0 )
	{
		LogAdd("Error: Cant Connect To SCF ExDB (%s:%d)", ConnectServer, SCFExDBPort);
		return FALSE;
	}

	SCFExDataServerLogin();
	LogAddC(2, "SCF ExDB Server Online");
	return TRUE;
}

void SCFExDataServerProtocolCore(BYTE protoNum, LPBYTE aRecv, int aLen)
{	
	switch ( protoNum )
	{
		case 0x00:
			SCFExDataServerLoginResult((SDHP_RESULT *)aRecv);
			break;
		case 0x10:
			DGGetPlayerExtraInfo((SDHP_GETPLAYEREXTRAINFO_SAVE *)aRecv);
			break;
		case 0x12:
			DGGetAccountExtraInfo((SDHP_GETACCOUNTEXTRAINFO_SAVE *)aRecv);
			break;
#if (PACK_EDITION>=2)
		case 0x13:
			DGGetWarehouseList((SDHP_GETWAREHOUSEDB_SAVE *)aRecv);
			break;
		case 0x14:
			GDGetWarehouseNoItemEx((SDHP_GETWAREHOUSEDB_RESULT *)aRecv);
			break;
			
		case 0x16:
			GDGetPetBot((SDHP_GETPETBOT *)aRecv);
			break;
			
		case 0x18:
			GDCreateResultPetBot((SDHP_PETBOTCREATERESULT *)aRecv);
			break;
#endif
	}
}

#if (PACK_EDITION>=3)
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

	for(n = 0; n < INVENTORY_SIZE;n++)
	{
		lpObj->pInventory[n].Clear();
		itype = lpMsg->dbInventory[n*ItemDbByte];

		if(ItemDbByte >= 16)
		{
			if(lpMsg->dbInventory[n*ItemDbByte] == 0xFF
				&& (lpMsg->dbInventory[n*ItemDbByte+DBI_NOPTION_DATA] & 0x80) == 0x80
				&& (lpMsg->dbInventory[n*ItemDbByte+DBI_OPTION380_DATA] & 0xF0) == 0xF0)
			{
				continue;
			}
		}
		else if(ItemDbByte > 7)
		{
			if(lpMsg->dbInventory[n*ItemDbByte] == 0xFF
				&& (lpMsg->dbInventory[n*ItemDbByte+DBI_NOPTION_DATA] & 0x80) == 0x80)
			{
				continue;
			}
		}
		else
		{
			if(lpMsg->dbInventory[n*ItemDbByte+DBI_TYPE] == 0xFF)
			{
				continue;
			}
		}

		if(ItemDbByte == 16)
		{
			itype |= (lpMsg->dbInventory[n*ItemDbByte+DBI_OPTION380_DATA] & 0xF0) * 32;
			itype |= (lpMsg->dbInventory[n*ItemDbByte+DBI_NOPTION_DATA] & 0x80) * 2;
			_type = itype;
		}
		else if(ItemDbByte == 10)
		{
			if((lpMsg->dbInventory[n*ItemDbByte+DBI_NOPTION_DATA] & 0x80) == 0x80)
			{
				itype += 256;
			}

			_type = ((itype / 32) * 512) + itype % 32;
		}
		else
		{
			_type = ((itype / 16) * 512) + itype % 16;
		}

		if(ItemDbByte >= 16)
		{
			if(lpMsg->dbInventory[n*ItemDbByte] == 0xFF
				&& (lpMsg->dbInventory[n*ItemDbByte+DBI_NOPTION_DATA] & 0x80) == 0x80
				&& (lpMsg->dbInventory[n*ItemDbByte+DBI_OPTION380_DATA] & 0xF0) == 0xF0)
			{
				itype = -1;
			}
		}
		else if(ItemDbByte >= 10)
		{
			if(lpMsg->dbInventory[n*ItemDbByte] == 0xFF
				&& (lpMsg->dbInventory[n*ItemDbByte+DBI_NOPTION_DATA] & 0x80) == 0x80)
			{
				itype = -1;
			}
		}
		else
		{
			if(lpMsg->dbInventory[n*ItemDbByte+DBI_TYPE] == 0xFF)
			{
				itype = -1;
			}
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

		if(itype != -1)
		{
			item.m_Level = DBI_GET_LEVEL(lpMsg->dbInventory[n*ItemDbByte+DBI_OPTION_DATA]);

			if(item.m_Level > MAX_ITEM_LEVEL)
			{
				item.m_Level = 0;
			}

			OptionData = lpMsg->dbInventory[n*ItemDbByte+DBI_OPTION_DATA];
			item.m_SkillOption = DBI_GET_SKILL(OptionData);
			item.m_LuckOption = DBI_GET_LUCK(OptionData);
			item.m_Z28Option = DBI_GET_OPTION(OptionData);

			if(ItemDbByte >= 10)
			{
				if(_type == ITEMGET(13,3))
				{
					item.m_Z28Option |= DBI_GET_OPTION16(lpMsg->dbInventory[n*ItemDbByte+DBI_NOPTION_DATA]) >> 4;
				}
				else
				{
					if ( DBI_GET_OPTION16(lpMsg->dbInventory[n*ItemDbByte+DBI_NOPTION_DATA]) == 0x40 )
						item.m_Z28Option += 4;	// Set +24
				}
			}

			item.m_Durability = lpMsg->dbInventory[n*ItemDbByte+DBI_DUR];

			if(ItemDbByte >= 16)
			{
				item.m_JewelOfHarmonyOption = lpMsg->dbInventory[n*ItemDbByte+DBI_JOH_DATA];
			}

			if(ItemDbByte >= 16)
			{
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
			}

			if(ItemDbByte >= 16)
			{
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
			}

			if(ItemDbByte == 7)
			{
				item.Convert(itype,item.m_SkillOption,item.m_LuckOption,item.m_Z28Option,0,0,0,0);
			}
			else if(ItemDbByte <= 10)
			{
				item.Convert(itype,item.m_SkillOption,item.m_LuckOption,item.m_Z28Option,DBI_GET_NOPTION(lpMsg->dbInventory[n*ItemDbByte+DBI_NOPTION_DATA]),lpMsg->dbInventory[n*ItemDbByte+DBI_SOPTION_DATA],item.m_ItemOptionEx,1);
			}
			else
			{
				item.Convert(itype,item.m_SkillOption,item.m_LuckOption,item.m_Z28Option,DBI_GET_NOPTION(lpMsg->dbInventory[n*ItemDbByte+DBI_NOPTION_DATA]),lpMsg->dbInventory[n*ItemDbByte+DBI_SOPTION_DATA],item.m_ItemOptionEx,3);
			}

			if(_type == ITEMGET(14,7))
			{

			}
			else
			{
				if(_type >= ITEMGET(14,0) && _type <= ITEMGET(14,8)
					|| _type >= ITEMGET(14,38) && _type <= ITEMGET(14,40))
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
				else if(_type >= ITEMGET(14,35) && _type <= ITEMGET(14,37))
				{
					if(item.m_Durability == 0.0f)
					{
						item.m_Durability = 1.0f;
					}

					if(item.m_Durability > 1.0f)
					{
						item.m_Durability = 1.0f;
					}
				}
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
			lpObj->pInventory[n] = item;
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
	if(gObj[aIndex].BotNumOwner >= 0 && gObj[aIndex].SaveBot == 1)
	{
		gObj[aIndex].SaveBot = 0;
		SDHP_GETPETBOT lpMsg;
		PHeadSetW((LPBYTE)&lpMsg, 0x17, sizeof(lpMsg));
		lpMsg.Result = 1;
		int result = gObj[aIndex].BotNumOwner;

		strcpy(lpMsg.AccountID,gObj[aIndex].AccountID);
		strcpy(lpMsg.Name,gObj[result].Name);
		lpMsg.aIndex = gObj[aIndex].m_Index;
		lpMsg.Class = gObj[result].DbClass;
		lpMsg.Level = gObj[result].Level;
		lpMsg.Exp = gObj[result].Experience;
		if(gObj[result].Life > gObj[result].MaxLife)
			lpMsg.Life = gObj[result].MaxLife;
		else
			lpMsg.Life = gObj[result].Life;

		lpMsg.MaxLife = gObj[result].MaxLife;
		lpMsg.Mana = gObj[result].Mana;
		lpMsg.MaxMana = gObj[result].MaxMana;
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


		ItemByteConvert16((LPBYTE)lpMsg.dbInventory , gObj[result].pInventory, INVENTORY_SIZE);
		MagicByteConvert(lpMsg.dbMagicList, gObj[result].Magic, MAGIC_SIZE);

		wsSCFExDbCli.DataSend((char*)&lpMsg, sizeof(lpMsg));
	}
}

void GDAskPetBot(int aIndex, char* AccountID)
{
	if ( strlen(AccountID) < 1 )
	{
		return;
	}

	SDHP_GETACCOUNTINFO	pMsg = {0};

	PHeadSetB((LPBYTE)&pMsg, 0x16, sizeof(pMsg));
	pMsg.aIndex = aIndex;
	memcpy(pMsg.AccountID, AccountID, MAX_ACCOUNT_LEN);

	wsSCFExDbCli.DataSend((char*)&pMsg, pMsg.h.size);
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
		int Class = lpMsg->Class >> 4;
		LPOBJ lpObj = &gObj[lpMsg->aIndex];
		int result = gObjAddMonster(lpMsg->MapNumber);
		if(result >= 0)
		{
			gObj[result].ChangeUP = lpMsg->Class & 0x07;	// Set Second Type of Character
			gObjSetMonster(result,Class,"PetBot");
			gObj[result].IsBot = 1;
			gObj[result].Level = lpMsg->Level;
			gObj[result].Life = lpMsg->Life;
			gObj[result].MaxLife = lpMsg->MaxLife;
			gObj[result].Mana = lpMsg->Mana;
			gObj[result].MaxMana = lpMsg->MaxMana;
			gObj[result].Experience = lpMsg->Exp;
			gObj[result].DbClass = lpMsg->Class;
			gObj[result].X = lpMsg->MapX;
			gObj[result].Y = lpMsg->MapY;
			gObj[result].MTX = lpMsg->MapX;
			gObj[result].MTY = lpMsg->MapY;
			gObj[result].Dir = 2;
			gObj[result].MapNumber = lpMsg->MapNumber;
			strncpy(gObj[result].Name,lpMsg->Name,10);
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

			gObj[aIndex].m_RecallMon = result;
			gObj[aIndex].BotNumOwner = result;
			gObj[result].m_MoveRange = 6;
			gObj[result].BotFollowMe = 1;
			gObj[result].NextExp = gLevelExperience[gObj[result].Level];

			gObj[aIndex].HaveBot = 1;

			botPet.CalcStats(result);
			char sbuf[512]={0};
			wsprintf(sbuf,"I'm right here for protect you %s!",gObj[aIndex].Name);
			ChatSend(&gObj[result],sbuf);
			GCActionSend(&gObj[result],AT_SALUTE1,result,aIndex);
		}
	}
}

void GDCreatePetBot(int aIndex,int BotNum,char * Name)
{
	SDHP_PETBOTCREATERESULT pMsg = {0};
	PHeadSetB((LPBYTE)&pMsg, 0x18, sizeof(pMsg));
	pMsg.aIndex = aIndex;
	pMsg.BotPetNum = BotNum;
	pMsg.result = 0;
	strcpy(pMsg.Name,Name);

	wsSCFExDbCli.DataSend((char*)&pMsg, pMsg.h.size);
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
//Connection

void SCFExDataServerLogin()
{

#if (WL_PROTECT==1)

#if (MAC_PROTECT==1)
	CODEREPLACE_START
	WinUtil_Teaser();
	CODEREPLACE_END
#else		
	int MyCheckVar;   
	VM_START_WITHLEVEL(19)
		CHECK_PROTECTION(MyCheckVar, 0x93465741)  	
		if (MyCheckVar != 0x93465741)
		{			
			_beginthread( Teaser__InsideTrigger, 0, NULL  );
		}
	VM_END
#endif

#endif
	SDHP_SERVERINFO pInfo;

	PHeadSetB((LPBYTE)&pInfo, 0x00, sizeof(pInfo));
	pInfo.h.headcode = 0x00;
	pInfo.Port = GameServerPort;
	pInfo.Type = 0x01;
	pInfo.ServerCode = gGameServerCode;
	strcpy(pInfo.ServerName, szServerName);

	wsSCFExDbCli.DataSend((char*)&pInfo, pInfo.h.size);
}

void SCFExDataServerLoginResult(SDHP_RESULT * lpMsg)
{
	#if (WL_PROTECT==1) 
		int MyCheckVar;   
		VM_START_WITHLEVEL(13)
			CHECK_PROTECTION(MyCheckVar, 0x93458741)  	 
			if (MyCheckVar != 0x93458741)
			{			
				_beginthread( Teaser__InsideTrigger, 0, NULL  );
			}
		VM_END
	#endif
	if ( lpMsg->Result == false )
	{
		LogAddTD("SCF ExDB Server Login Fail: %d", lpMsg->Result);

		return;
	}	
}

//Player Extra Info
void GDGetPlayerExtraInfo(int aIndex, char* AccountID, char* Name)
{
	if ( strlen(AccountID) < 1 )
	{
		return;
	}

	SDHP_GETPLAYERINFO	pMsg = {0};

	PHeadSetB((LPBYTE)&pMsg, 0x10, sizeof(pMsg));
	pMsg.aIndex = aIndex;
	memcpy(pMsg.AccountID, AccountID, MAX_ACCOUNT_LEN);
	memcpy(pMsg.Name, Name, MAX_ACCOUNT_LEN);

	wsSCFExDbCli.DataSend((char*)&pMsg, pMsg.h.size);
}

void DGGetPlayerExtraInfo(SDHP_GETPLAYEREXTRAINFO_SAVE * lpMsg)
{
	char szId[11];
	int aIndex = 0;
	PMSG_TALKRESULT pResult = {0};

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
		lpObj->MasterLevel = lpMsg->MasterLevel;
		lpObj->MasterPoints = lpMsg->MasterPoints;
		lpObj->Resets = lpMsg->Resets;
		lpObj->PCPoints = lpMsg->PCPoints;
		lpObj->Married = lpMsg->Married;
		memcpy(lpObj->MarriedName, lpMsg->MarriedName, MAX_ACCOUNT_LEN);
		memcpy(lpObj->CustomQuest, lpMsg->CustomQuest, 255);

		char sBuf[255]={0};
		if (lpObj->Married == 1)
		{
			wsprintf(sBuf, "[Marry] Married to %s",lpObj->MarriedName);
			GCServerMsgStringSend (sBuf,lpObj->m_Index,0x01 ) ;
		}
		wsprintf(sBuf, "[ResetSystem] Resets:%d",lpObj->Resets);
		GCServerMsgStringSend (sBuf,lpObj->m_Index,0x01 ) ;

		Mastering.InfoSend(aIndex);

#if (PACK_EDITION>=2)
		PCShop.SendPoints(lpObj);
		customQuest.LevelUp(lpObj);
#endif
	}
}

void JGSetPlayerExtraInfo(LPOBJ lpObj)
{
	if (lpObj->PlayerExtraInfoModified == TRUE)
	{
		lpObj->PlayerExtraInfoModified = FALSE;

		SDHP_GETPLAYERINFO_INFOSAVE pMsg = {0};
		PHeadSetW((LPBYTE)&pMsg, 0x11, sizeof(pMsg));

		memcpy(pMsg.Name, lpObj->Name, MAX_ACCOUNT_LEN);
		pMsg.MasterLevel = lpObj->MasterLevel;
		pMsg.MasterPoints = lpObj->MasterPoints;
		pMsg.Resets = lpObj->Resets;
		pMsg.PCPoints = lpObj->PCPoints;
		pMsg.Married = lpObj->Married;
		memcpy(pMsg.MarriedName, lpObj->MarriedName, MAX_ACCOUNT_LEN);
		memcpy(pMsg.CustomQuest, lpObj->CustomQuest, 255);
		wsSCFExDbCli.DataSend((char*)&pMsg, sizeof(pMsg));
	}
}


void JGSetPlayerMarryDivorseInfo(char * Name)
{
	SDHP_SETMARRYDIVORSE_INFOSAVE pMsg = {0};
	PHeadSetB((LPBYTE)&pMsg, 0x09, sizeof(pMsg));
	memcpy(pMsg.Name, Name, MAX_ACCOUNT_LEN);
	wsSCFExDbCli.DataSend((char*)&pMsg, pMsg.h.size);
}

//Player Extra Info

//AccountExtraInfo

void GDGetAccounExtraInfo(int aIndex, char* AccountID)
{
	if ( strlen(AccountID) < 1 )
	{
		return;
	}

	SDHP_GETACCOUNTINFO	pMsg = {0};

	PHeadSetB((LPBYTE)&pMsg, 0x12, sizeof(pMsg));
	pMsg.aIndex = aIndex;
	memcpy(pMsg.AccountID, AccountID, MAX_ACCOUNT_LEN);

	wsSCFExDbCli.DataSend((char*)&pMsg, pMsg.h.size);
}

void DGGetAccountExtraInfo(SDHP_GETACCOUNTEXTRAINFO_SAVE * lpMsg)
{
	char szId[11];
	int aIndex = 0;
	PMSG_TALKRESULT pResult = {0};

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
#if (PACK_EDITION>=3)
		lpObj->HaveBot = lpMsg->HaveBot;
#endif
	}
	LogAddTD("[%s][ExtraAccount] Vip:%d VipDays:%d VipMoney:%d VipWareEx:%d",lpObj->AccountID,lpObj->Vip,lpObj->VipDays,lpObj->VipMoney,lpMsg->WarehouseVipCount);
}

void JGSetAccountExtraInfo(LPOBJ lpObj)
{
	if (lpObj->AccountExtraInfoModified == TRUE)
	{
		lpObj->AccountExtraInfoModified = FALSE;

		SDHP_GETACCOUNTEXTRAINFO_SAVE pMsg = {0};
		PHeadSetB((LPBYTE)&pMsg, 0x13, sizeof(pMsg));

		memcpy(pMsg.AccountID, lpObj->AccountID, MAX_ACCOUNT_LEN);
		pMsg.aIndex = lpObj->m_Index;
		pMsg.Vip = lpObj->Vip;
		pMsg.VipMoney = lpObj->VipMoney;
		pMsg.VipDays = lpObj->VipDays;
		pMsg.WarehouseVipCount = lpObj->WarehouseVipCount;
#if (PACK_EDITION>=3)
		pMsg.HaveBot = lpObj->HaveBot;
#endif
		wsSCFExDbCli.DataSend((char*)&pMsg, pMsg.h.size);
	}
}

//AccountExtraInfo


void GJSetStatusBan(char* Name, BOOL Type, BYTE Ban)
{
	SDHP_SETSTATUSBAN_INFOSAVE	pMsg = {0};

	PHeadSetB((LPBYTE)&pMsg, 0x08, sizeof(pMsg));
	pMsg.Ban = Ban;
	pMsg.Type = Type;
	memcpy(pMsg.Name, Name, MAX_ACCOUNT_LEN);

	wsSCFExDbCli.DataSend((char*)&pMsg, pMsg.h.size);
}

//WarehouseEx

#if (PACK_EDITION>=2)
void GDSetWarehouseListEx(int aIndex)
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
	pMsg.h.headcode = 0x15;
	pMsg.h.sizeH = HIBYTE(sizeof(pMsg));
	pMsg.h.sizeL = LOBYTE(sizeof(pMsg));
	pMsg.Money = gObj[aIndex].WarehouseMoney;
	pMsg.pw = gObj[aIndex].WarehousePW;
	pMsg.ExNum = gObj[aIndex].WarehouseExNum;
	ItemByteConvert16(pMsg.dbItems, gObj[aIndex].pWarehouse, WAREHOUSE_SIZE);

	wsSCFExDbCli.DataSend((PCHAR)&pMsg, sizeof(pMsg));
	gObjSavePetItemInfo(aIndex, 1);
	gObjWarehouseTextSave(&gObj[aIndex]);
}


void GDGetWarehouseListEx(int aIndex, char* AccountID)
{
	if ( strlen(AccountID) < 1 )
	{
		return;
	}

	SDHP_GETWAREHOUSEDBEX	pMsg;

	pMsg.h.c = 0xC1;
	pMsg.h.headcode = 0x14;
	pMsg.h.size = sizeof(SDHP_GETWAREHOUSEDBEX);
	pMsg.aIndex = aIndex;
	pMsg.ExNum = gObj[aIndex].WarehouseExNum;
	memcpy(pMsg.AccountID, AccountID, sizeof(pMsg.AccountID));

	wsSCFExDbCli.DataSend((char*)&pMsg, pMsg.h.size);
}

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
	for (int i=0;i<WAREHOUSE_SIZE;i++)
	{
		lpObj->pWarehouse[i].Clear();
	}
	GCWarehouseInventoryMoneySend(lpObj->m_Index, 1, lpObj->Money, lpObj->WarehouseMoney);

	lpObj->LoadWareHouseInfo = true;
	lpObj->m_ReqWarehouseOpen = false;
}
#endif