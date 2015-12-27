#include "stdafx.h"
#include "ItemBagEx.h"
#include "gObjMonster.h"
#include "DSProtocol.h"
#include "GameMain.h"
#include "..\include\readscript.h"
#include "..\common\winutil.h"
#include "logproc.h"
#include "ImperialGuardian.h"
#include "DoppelGanger.h"
#include "HalloweenEvent.h"
#include "SwampEvent.h"
#include "Raklion.h"

#include "LogToFile.h"
extern CLogToFile ITEMBAG_DROP_LOG;

// GS-N 0.99.60T 0x0046A030
//	GS-N	1.00.18	JPN	0x0047CAA0	-	Completed

CItemBagEx::CItemBagEx()
{
	return;
}

CItemBagEx::~CItemBagEx()
{
	return;
}

void CItemBagEx::Init(char* name)
{
	this->m_bLoad = FALSE;
	this->m_sEventName[0] = 0;
	this->m_iEventItemType = -1;
	this->m_iEventItemLevel = 0;
	this->m_iDropZen = 0;
	this->m_iEventItemDropRate = 0;
	this->m_iItemDropRate = 0;
	this->m_iExItemDropRate = 0;
	this->m_iBagObjectCount = 0;

	this->LoadItem(name);
}

void CItemBagEx::LoadItem(char* script_file)
{
	int Token;

	this->m_bLoad = FALSE;
	SMDFile = fopen(script_file, "r");

	if ( SMDFile == NULL )
	{
		ITEMBAG_DROP_LOG.Output(lMsg.Get(MSGGET(1, 197)), script_file);
		return;
	}

	int n=0;

	while ( true )
	{
		Token = GetToken();

		if ( Token == 2 )
		{
			break;
		}

		if ( Token == 1 )
		{
			int st = TokenNumber;	// script_type

			if ( st == 0 )
			{
				while ( true )
				{
					Token = GetToken();

					if ( Token == 0 )
					{
						if ( strcmp("end", TokenString) == 0 )
						{
							break;
						}
					}

					int map = TokenNumber;

					if ( MAX_MAP_RANGE(map) == FALSE )
					{
						MsgBox("ExEvent ItemBag LoadFail [%s]", script_file);
						return;
					}

					Token = GetToken();
					this->DropMapInfo[map].m_bIsDrop = TokenNumber;

					Token = GetToken();
					this->DropMapInfo[map].m_MinMonsterLevel = TokenNumber;

					Token = GetToken();
					this->DropMapInfo[map].m_MaxMonsterLevel = TokenNumber;
				}
			
			}
			else if ( st == 1 )
			{
				while ( true )
				{
					Token = GetToken();

					if ( Token == 0 )
					{
						if ( strcmp("end", TokenString) == 0 )
						{
							break;
						}
					}

					strcpy(this->m_sEventName, TokenString);

					Token = GetToken();
					this->m_iDropZen = TokenNumber;

					Token = GetToken();
					int type = TokenNumber;

					Token = GetToken();
					int index = TokenNumber;

					this->m_iEventItemType = ITEMGET(type, index);

					Token = GetToken();
					this->m_iEventItemLevel = TokenNumber;

					Token = GetToken();
					this->m_iEventItemDropRate = TokenNumber;

					Token = GetToken();
					this->m_iItemDropRate = TokenNumber;

					Token = GetToken();
					this->m_iExItemDropRate = TokenNumber;

					char szTemp[256];

					wsprintf(szTemp, "[%s] Eventitemnum = %d,EventItemLevel = %d, EventItemDropRate = %d, ItemDropRate = %d, ExItemDropRate = %d",
						this->m_sEventName, this->m_iEventItemType, this->m_iEventItemLevel, this->m_iEventItemDropRate, this->m_iItemDropRate, this->m_iExItemDropRate);
					ITEMBAG_DROP_LOG.Output(szTemp);
				}
			}
			else if ( st == 2 )
			{
				while ( true )
				{
					Token = GetToken();

					if ( Token == 0 )
					{
						if ( strcmp("end", TokenString) == 0 )
						{
							break;
						}
					}

					this->BagObject[n].m_type = TokenNumber;

					Token = GetToken();
					this->BagObject[n].m_index = TokenNumber;

					Token = GetToken();
					this->BagObject[n].m_minLevel = TokenNumber;

					Token = GetToken();
					this->BagObject[n].m_maxLevel = TokenNumber;

					Token = GetToken();
					this->BagObject[n].m_isskill = TokenNumber;

					Token = GetToken();
					this->BagObject[n].m_isluck = TokenNumber;

					Token = GetToken();
					this->BagObject[n].m_isoption = TokenNumber;

					Token = GetToken();
					this->BagObject[n].m_isexitem = TokenNumber;

					n++;
					this->m_iBagObjectCount++;

					if ( this->m_iBagObjectCount > MAX_ITEMBAG_ATTR-1 )
					{
						break;
					}
				}
			}
		}
	}


	if(this->m_iBagObjectCount <= 0)
	{
		MsgBox("There need to be at least one item in %s", script_file);
		exit(1);
	}

	fclose(SMDFile);
	ITEMBAG_DROP_LOG.Output(lMsg.Get(MSGGET(1, 198)), script_file);
	this->m_bLoad = TRUE;
}


BYTE CItemBagEx::GetLevel(int n)
{
	if ( n <0 || n > MAX_ITEMBAG_ATTR-1 )
	{
		return 0;
	}

	if ( this->BagObject[n].m_minLevel == this->BagObject[n].m_maxLevel )
	{
		return this->BagObject[n].m_minLevel;
	}

	int sub = (this->BagObject[n].m_maxLevel - this->BagObject[n].m_minLevel) + 1;
	int level = this->BagObject[n].m_minLevel + (rand()%sub);

	return level;
}

BYTE CItemBagEx::GetMinLevel(int n)
{
	if ( n<0 || n > MAX_ITEMBAG_ATTR-1 )
		return 0;

	return this->BagObject[n].m_minLevel;
}

BYTE CItemBagEx::GetMaxLevel(int n)
{
	if ( n<0 || n > MAX_ITEMBAG_ATTR-1 )
		return 0;

	return this->BagObject[n].m_maxLevel;
}


BOOL CItemBagEx::PickItem(CItem & objItem, int & nItemIndex)
{
	if ( this->m_bLoad == FALSE )
		return FALSE;

	int DropItemNum;

	if ( this->GetBagCount() > 0 )
	{
		DropItemNum = rand() % this->GetBagCount();
		nItemIndex = DropItemNum;
		objItem.m_Type = ItemGetNumberMake(this->BagObject[DropItemNum].m_type, this->BagObject[DropItemNum].m_index);
		objItem.m_Level = this->GetLevel(DropItemNum);

		if ( objItem.m_Type == -1 )
			return FALSE;

		if ( this->BagObject[DropItemNum].m_isskill != 0 )
			objItem.m_SkillOption = 1;

		if ( this->BagObject[DropItemNum].m_isluck != 0 )
			objItem.m_LuckOption = 1;

		if ( this->BagObject[DropItemNum].m_isoption > 0 )
		{
			int z28Percent = this->BagObject[DropItemNum].m_isoption*100;
			objItem.m_Z28Option = rand()%(z28Percent+1);
			objItem.m_Z28Option = objItem.m_Z28Option / 100;
		} else {
			objItem.m_Z28Option = 0;
		}

		objItem.m_NewOption = 0;
		int DropItemRate = (rand() % 1000)+1;
		if (DropItemRate <= this->GetExItemDropRate())
		{
			if ( this->BagObject[DropItemNum].m_isexitem != 0 )
			{
				objItem.m_NewOption = BoxExcOptions(this->BagObject[DropItemNum].m_isexitem);
			}
		}

		if ( objItem.m_Type == ITEMGET(12,15) ||
			 objItem.m_Type == ITEMGET(14,13) ||
			 objItem.m_Type == ITEMGET(14,14) )
		{
			objItem.m_SkillOption = 0;
			objItem.m_LuckOption = 0;
			objItem.m_Z28Option = 0;
			objItem.m_Level = 0;
		}

		if ( objItem.m_Type == ITEMGET(13,0) ||
			 objItem.m_Type == ITEMGET(13,1) ||
			 objItem.m_Type == ITEMGET(13,2) ||
			 objItem.m_Type == ITEMGET(13,8) ||
			 objItem.m_Type == ITEMGET(13,9) ||
			 objItem.m_Type == ITEMGET(13,12) ||
			 objItem.m_Type == ITEMGET(13,13) )
		{
			objItem.m_Level = 0;
		}

		objItem.m_Durability = 0;

		return TRUE;
	}

	return FALSE;
}

BOOL CItemBagEx::IsEnableEventItemDrop(int aIndex)
{
	int iMapNumber = gObj[aIndex].MapNumber;

	if ( this->DropMapInfo[iMapNumber].m_bIsDrop == FALSE )
	{
		return FALSE;
	}

	int iLevel = gObj[aIndex].Level;

	if  ( iLevel < this->DropMapInfo[iMapNumber].m_MinMonsterLevel || iLevel > this->DropMapInfo[iMapNumber].m_MaxMonsterLevel)
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CItemBagEx::IsMobIdExist(int aIndex)
{
	int iMonsterClass = gObj[aIndex].Class;

	for (int i=0; i<MAX_MAP_NUMBER; i++)
	{
		if (iMonsterClass == this->DropMapInfo[i].m_MaxMonsterLevel)
		{
			return TRUE;
		}
	}

	return FALSE;
}

//Commonserver replacement of Item drop rate
BOOL CItemBagEx::DropEventItem(int aIndex)
{
	if ( this->m_bLoad == FALSE )
	{
		return FALSE;
	}

	if ( this->IsEnableEventItemDrop(aIndex) == FALSE )
	{
		return FALSE;
	}

	LPOBJ lpObj = &gObj[aIndex];

	int randN = rand()%10000;
	if ( randN < this->m_iEventItemDropRate )
	{
		int ei;
		int eil;
		int x;
		int y;
		float dur = 0;
		dur = 255.0;

		x = lpObj->X;
		y = lpObj->Y;
		eil = this->m_iEventItemLevel;
		ei = this->m_iEventItemType;

		int thdu = gObjMonsterTopHitDamageUser(lpObj);
		ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, x ,y, ei, eil, dur, 0, 0, 0, thdu, 0, 0);
		ITEMBAG_DROP_LOG.Output("[%s][%s] Event ItemDrop RND:%d/%d (%d)(%d/%d)", 
			lpObj->Name,
			this->m_sEventName, 
			randN, this->m_iEventItemDropRate,
			lpObj->MapNumber, x, y
		);
		return TRUE;
	}

	return FALSE;
}

BOOL CItemBagEx::DropEventItemByMobId(int aIndex)
{
	if ( this->m_bLoad == FALSE )
	{
		return FALSE;
	}

	if ( this->IsMobIdExist(aIndex) == FALSE )
	{
		return FALSE;
	}

	LPOBJ lpObj = &gObj[aIndex];

	int randN = rand()%10000;
	if ( randN < this->m_iEventItemDropRate )
	{
		int ei;
		int eil;
		int x;
		int y;
		float dur = 0;
		dur = 255.0;

		x = lpObj->X;
		y = lpObj->Y;
		eil = this->m_iEventItemLevel;
		ei = this->m_iEventItemType;

		int thdu = gObjMonsterTopHitDamageUser(lpObj);
		ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, x ,y, ei, eil, dur, 0, 0, 0, thdu, 0, 0);
		ITEMBAG_DROP_LOG.Output("[%s][%s] Event ItemDrop RND:%d/%d (%d)(%d/%d)", 
			lpObj->Name, 
			this->m_sEventName, 
			randN, this->m_iEventItemDropRate, 
			lpObj->MapNumber, x, y
		);
		return TRUE;
	}

	return FALSE;
}

BOOL CItemBagEx::DropStarOfXMasEventItem(int aIndex)
{
	if ( this->m_bLoad == FALSE )
		return FALSE;

	float dur;
	int type;
	int level;
	int x;
	int y;
	int Option1 = 0;
	int Option2 = 0;
	int Option3 = 0;
	int DropItemRate;
	int DropItemNum;
	int ExOption = 0;
	LPOBJ lpObj = &gObj[aIndex];

	DropItemRate = rand() % 100;
	if ( DropItemRate > this->GetItemDropRate() )
	{
		if (this->GetDropZen() <= 0)
		{
			ITEMBAG_DROP_LOG.Output("[CHRISTMAS] StarOfXMas Event NO ItemDrop [%s][%s] [RATE:%d/%d]",
				lpObj->AccountID, lpObj->Name, DropItemRate, this->GetItemDropRate());
			return FALSE;
		}

		MapC[lpObj->MapNumber].MoneyItemDrop(this->GetDropZen(), 
			lpObj->X, lpObj->Y);

		ITEMBAG_DROP_LOG.Output("[CHRISTMAS] StarOfXMas Event ItemDrop [%s][%s] [%d Zen] [RATE:%d/%d]",
			lpObj->AccountID, lpObj->Name, this->GetDropZen(), DropItemRate, this->GetItemDropRate());

		return TRUE;
	}

	if ( this->GetBagCount() > 0 )
	{
		DropItemNum = rand() % this->GetBagCount();
		dur = 0;
		x = lpObj->X;
		y = lpObj->Y;
		level = this->GetLevel(DropItemNum);
		type = ItemGetNumberMake(this->BagObject[DropItemNum].m_type, this->BagObject[DropItemNum].m_index);

		if ( type == -1 )
			return FALSE;

		if ( this->BagObject[DropItemNum].m_isskill != 0 )
			Option1 = 1;

		if ( this->BagObject[DropItemNum].m_isluck != 0 )
		{
			Option2 = 0;

			if ( (rand()%2) == 0 )
			{
				Option2 = 1;
			}
		}

		if ( this->BagObject[DropItemNum].m_isoption > 0 )
		{
			int z28Percent = this->BagObject[DropItemNum].m_isoption*100;
			Option3 = rand()%(z28Percent+1);
			Option3 = Option3 / 100;
			/*if ( rand()%5 < 1 )
			{
				Option3 = this->BagObject[DropItemNum].m_isoption;
			}
			else
			{
				Option3 = rand()%this->BagObject[DropItemNum].m_isoption + 1;
			}*/
		}

		DropItemRate = (rand() % 1000)+1;
		if (DropItemRate <= this->GetExItemDropRate())
		{
			if ( this->BagObject[DropItemNum].m_isexitem != 0 )
			{
				ExOption = BoxExcOptions(this->BagObject[DropItemNum].m_isexitem);
			}
		}

		if ( type == ITEMGET(12,15) ||
			 type == ITEMGET(14,13) ||
			 type == ITEMGET(14,14) )
		{
			Option1 = 0;
			Option2 = 0;
			Option3 = 0;
			level = 0;
		}

		if ( type == ITEMGET(14,19) ||
			 type == ITEMGET(14,28) ||
			 type == ITEMGET(13,18) )
		{
			Option1 = 0;
			Option2 = 0;
			Option3 = 0;
		}
			
		ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, x, y, type, level, dur,
			Option1, Option2, Option3, lpObj->m_Index, ExOption, 0);

		ITEMBAG_DROP_LOG.Output("[CHRISTMAS] StarOfXMas Event ItemDrop [%s][%s] [%s] : (%d)(%d/%d) Item:(%s)%d Level:%d op1:%d op2:%d op3:%d",
			lpObj->AccountID, lpObj->Name, this->m_sEventName,
			lpObj->MapNumber, x, y, ItemAttribute[type].Name,
			type, level, Option1, Option2, Option3);

		PMSG_SERVERCMD ServerCmd;

		PHeadSubSetB((LPBYTE)&ServerCmd, 0xF3, 0x40, sizeof(ServerCmd));
		ServerCmd.CmdType = 0;
		ServerCmd.X = lpObj->X;
		ServerCmd.Y = lpObj->Y;

		MsgSendV2(lpObj, (LPBYTE)&ServerCmd, sizeof(ServerCmd));
		DataSend(lpObj->m_Index, (LPBYTE)&ServerCmd, sizeof(ServerCmd));
	}

	return TRUE;
}

BOOL CItemBagEx::DropRedRibbonBoxEventItem(int aIndex)
{
	if ( this->m_bLoad == FALSE )
		return FALSE;

	float dur;
	int type;
	int level;
	int x;
	int y;
	int Option1 = 0;
	int Option2 = 0;
	int Option3 = 0;
	int DropItemRate;
	int DropItemNum;
	int ExOption = 0;
	LPOBJ lpObj = &gObj[aIndex];

	DropItemRate = rand() % 100;
	if ( DropItemRate > this->GetItemDropRate() )
	{
		if (this->GetDropZen() <= 0)
		{
			ITEMBAG_DROP_LOG.Output("[EVENT][CHRISTMAS RIBBONBOX] REDRIBBONBOX Event NO ItemDrop [%s][%s] [RATE:%d/%d]",
				lpObj->AccountID, lpObj->Name, DropItemRate, this->GetItemDropRate());
			return FALSE;
		}

		MapC[lpObj->MapNumber].MoneyItemDrop(this->GetDropZen(), 
			lpObj->X, lpObj->Y);

		ITEMBAG_DROP_LOG.Output("[EVENT][CHRISTMAS RIBBONBOX] REDRIBBONBOX Event ItemDrop [%s][%s] [%d Zen] [RATE:%d/%d]",
			lpObj->AccountID, lpObj->Name, this->GetDropZen(), DropItemRate, this->GetItemDropRate());

		return TRUE;
	}

	if ( this->GetBagCount() > 0 )
	{
		DropItemNum = rand() % this->GetBagCount();
		dur = 0;
		x = lpObj->X;
		y = lpObj->Y;
		level = this->GetLevel(DropItemNum);
		type = ItemGetNumberMake(this->BagObject[DropItemNum].m_type, this->BagObject[DropItemNum].m_index);

		if ( type == -1 )
			return FALSE;

		if ( this->BagObject[DropItemNum].m_isskill != 0 )
			Option1 = 1;

		if ( this->BagObject[DropItemNum].m_isluck != 0 )
		{
			Option2 = 0;

			if ( (rand()%2) == 0 )
			{
				Option2 = 1;
			}
		}

		if ( this->BagObject[DropItemNum].m_isoption > 0 )
		{
			int z28Percent = this->BagObject[DropItemNum].m_isoption*100;
			Option3 = rand()%(z28Percent+1);
			Option3 = Option3 / 100;
		}

		DropItemRate = (rand() % 1000)+1;
		if (DropItemRate <= this->GetExItemDropRate())
		{
			if ( this->BagObject[DropItemNum].m_isexitem != 0 )
			{
				ExOption = BoxExcOptions(this->BagObject[DropItemNum].m_isexitem);
			}
		}

		if ( type == ITEMGET(12,15) ||
			 type == ITEMGET(14,13) ||
			 type == ITEMGET(14,14) )
		{
			Option1 = 0;
			Option2 = 0;
			Option3 = 0;
			level = 0;
		}

		if ( type == ITEMGET(14,19) ||
			 type == ITEMGET(14,28) ||
			 type == ITEMGET(13,18) )
		{
			Option1 = 0;
			Option2 = 0;
			Option3 = 0;
		}
			
		ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, x, y, type, level, dur,
			Option1, Option2, Option3, lpObj->m_Index, ExOption, 0);

		ITEMBAG_DROP_LOG.Output("[EVENT][CHRISTMAS RIBBONBOX] REDRIBBONBOX Event ItemDrop [%s][%s] [%s] : (%d)(%d/%d) Item:(%s)%d Level:%d op1:%d op2:%d op3:%d",
			lpObj->AccountID, lpObj->Name, this->m_sEventName,
			lpObj->MapNumber, x, y, ItemAttribute[type].Name,
			type, level, Option1, Option2, Option3);

		PMSG_SERVERCMD ServerCmd;

		PHeadSubSetB((LPBYTE)&ServerCmd, 0xF3, 0x40, sizeof(ServerCmd));
		ServerCmd.CmdType = 0;
		ServerCmd.X = lpObj->X;
		ServerCmd.Y = lpObj->Y;

		MsgSendV2(lpObj, (LPBYTE)&ServerCmd, sizeof(ServerCmd));
		DataSend(lpObj->m_Index, (LPBYTE)&ServerCmd, sizeof(ServerCmd));
	}

	return TRUE;
}

BOOL CItemBagEx::DropGreenRibbonBoxEventItem(int aIndex)
{
	if ( this->m_bLoad == FALSE )
		return FALSE;

	float dur;
	int type;
	int level;
	int x;
	int y;
	int Option1 = 0;
	int Option2 = 0;
	int Option3 = 0;
	int DropItemRate;
	int DropItemNum;
	int ExOption = 0;
	LPOBJ lpObj = &gObj[aIndex];

	DropItemRate = rand() % 100;
	if ( DropItemRate > this->GetItemDropRate() )
	{
		if (this->GetDropZen() <= 0)
		{
			ITEMBAG_DROP_LOG.Output("[EVENT][CHRISTMAS RIBBONBOX] GREENRIBBONBOX Event NO ItemDrop [%s][%s] [RATE:%d/%d]",
				lpObj->AccountID, lpObj->Name, DropItemRate, this->GetItemDropRate());
			return FALSE;
		}

		MapC[lpObj->MapNumber].MoneyItemDrop(this->GetDropZen(), 
			lpObj->X, lpObj->Y);

		ITEMBAG_DROP_LOG.Output("[EVENT][CHRISTMAS RIBBONBOX] GREENRIBBONBOX Event ItemDrop [%s][%s] [%d Zen] [RATE:%d/%d]",
			lpObj->AccountID, lpObj->Name, this->GetDropZen(), DropItemRate, this->GetItemDropRate());

		return TRUE;
	}

	if ( this->GetBagCount() > 0 )
	{
		DropItemNum = rand() % this->GetBagCount();
		dur = 0;
		x = lpObj->X;
		y = lpObj->Y;
		level = this->GetLevel(DropItemNum);
		type = ItemGetNumberMake(this->BagObject[DropItemNum].m_type, this->BagObject[DropItemNum].m_index);

		if ( type == -1 )
			return FALSE;

		if ( this->BagObject[DropItemNum].m_isskill != 0 )
			Option1 = 1;

		if ( this->BagObject[DropItemNum].m_isluck != 0 )
		{
			Option2 = 0;

			if ( (rand()%2) == 0 )
			{
				Option2 = 1;
			}
		}

		if ( this->BagObject[DropItemNum].m_isoption > 0 )
		{
			int z28Percent = this->BagObject[DropItemNum].m_isoption*100;
			Option3 = rand()%(z28Percent+1);
			Option3 = Option3 / 100;
		}

		DropItemRate = (rand() % 1000)+1;
		if (DropItemRate <= this->GetExItemDropRate())
		{
			if ( this->BagObject[DropItemNum].m_isexitem != 0 )
			{
				ExOption = BoxExcOptions(this->BagObject[DropItemNum].m_isexitem);
			}
		}

		if ( type == ITEMGET(12,15) ||
			 type == ITEMGET(14,13) ||
			 type == ITEMGET(14,14) ||
			 type == ITEMGET(14,16) )
		{
			Option1 = 0;
			Option2 = 0;
			Option3 = 0;
			level = 0;
		}

		if ( type == ITEMGET(14,19) ||
			 type == ITEMGET(14,28) ||
			 type == ITEMGET(13,18) )
		{
			Option1 = 0;
			Option2 = 0;
			Option3 = 0;
		}
			
		ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, x, y, type, level, dur,
			Option1, Option2, Option3, lpObj->m_Index, ExOption, 0);

		ITEMBAG_DROP_LOG.Output("[EVENT][CHRISTMAS RIBBONBOX] GREENRIBBONBOX Event ItemDrop [%s][%s] [%s] : (%d)(%d/%d) Item:(%s)%d Level:%d op1:%d op2:%d op3:%d",
			lpObj->AccountID, lpObj->Name, this->m_sEventName,
			lpObj->MapNumber, x, y, ItemAttribute[type].Name,
			type, level, Option1, Option2, Option3);

		PMSG_SERVERCMD ServerCmd;

		PHeadSubSetB((LPBYTE)&ServerCmd, 0xF3, 0x40, sizeof(ServerCmd));
		ServerCmd.CmdType = 0;
		ServerCmd.X = lpObj->X;
		ServerCmd.Y = lpObj->Y;

		MsgSendV2(lpObj, (LPBYTE)&ServerCmd, sizeof(ServerCmd));
		DataSend(lpObj->m_Index, (LPBYTE)&ServerCmd, sizeof(ServerCmd));
	}

	return TRUE;
}




BOOL CItemBagEx::DropBlueRibbonBoxEventItem(int aIndex)
{
	if ( this->m_bLoad == FALSE )
		return FALSE;

	float dur;
	int type;
	int level;
	int x;
	int y;
	int Option1 = 0;
	int Option2 = 0;
	int Option3 = 0;
	int DropItemRate;
	int DropItemNum;
	int ExOption = 0;
	LPOBJ lpObj = &gObj[aIndex];

	DropItemRate = rand() % 100;
	if ( DropItemRate > this->GetItemDropRate() )
	{
		if (this->GetDropZen() <= 0)
		{
			ITEMBAG_DROP_LOG.Output("[EVENT][CHRISTMAS RIBBONBOX] BLUERIBBONBOX NO ItemDrop [%s][%s] [RATE:%d/%d]",
				lpObj->AccountID, lpObj->Name, DropItemRate, this->GetItemDropRate());
			return FALSE;
		}

		MapC[lpObj->MapNumber].MoneyItemDrop(this->GetDropZen(), 
			lpObj->X, lpObj->Y);

		ITEMBAG_DROP_LOG.Output("[EVENT][CHRISTMAS RIBBONBOX] BLUERIBBONBOX Event ItemDrop [%s][%s] [%d Zen] [RATE:%d/%d]",
			lpObj->AccountID, lpObj->Name, this->GetDropZen(), DropItemRate, this->GetItemDropRate());

		return TRUE;
	}

	if ( this->GetBagCount() > 0 )
	{
		DropItemNum = rand() % this->GetBagCount();
		dur = 0;
		x = lpObj->X;
		y = lpObj->Y;
		level = this->GetLevel(DropItemNum);
		type = ItemGetNumberMake(this->BagObject[DropItemNum].m_type, this->BagObject[DropItemNum].m_index);

		if ( type == -1 )
			return FALSE;

		if ( this->BagObject[DropItemNum].m_isskill != 0 )
			Option1 = 1;

		if ( this->BagObject[DropItemNum].m_isluck != 0 )
		{
			Option2 = 0;

			if ( (rand()%2) == 0 )
			{
				Option2 = 1;
			}
		}

		if ( this->BagObject[DropItemNum].m_isoption > 0 )
		{
			int z28Percent = this->BagObject[DropItemNum].m_isoption*100;
			Option3 = rand()%(z28Percent+1);
			Option3 = Option3 / 100;
		}

		DropItemRate = (rand() % 1000)+1;
		if (DropItemRate <= this->GetExItemDropRate())
		{
			if ( this->BagObject[DropItemNum].m_isexitem != 0 )
			{
				ExOption = BoxExcOptions(this->BagObject[DropItemNum].m_isexitem);
			}
		}

		if ( type == ITEMGET(12,15) ||
			 type == ITEMGET(14,13) ||
			 type == ITEMGET(14,14)  )
		{
			Option1 = 0;
			Option2 = 0;
			Option3 = 0;
			level = 0;
		}

		if ( type == ITEMGET(13,8) ||
			 type == ITEMGET(13,9) ||
			 type == ITEMGET(13,12) ||
			 type == ITEMGET(13,21) )
		{
			level = 0;
		}
			
		ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, x, y, type, level, dur,
			Option1, Option2, Option3, lpObj->m_Index, ExOption, 0);

		ITEMBAG_DROP_LOG.Output("[EVENT][CHRISTMAS RIBBONBOX] BLUERIBBONBOX Event ItemDrop [%s][%s] [%s] : (%d)(%d/%d) Item:(%s)%d Level:%d op1:%d op2:%d op3:%d",
			lpObj->AccountID, lpObj->Name, this->m_sEventName,
			lpObj->MapNumber, x, y, ItemAttribute[type].Name,
			type, level, Option1, Option2, Option3);

		PMSG_SERVERCMD ServerCmd;

		PHeadSubSetB((LPBYTE)&ServerCmd, 0xF3, 0x40, sizeof(ServerCmd));
		ServerCmd.CmdType = 0;
		ServerCmd.X = lpObj->X;
		ServerCmd.Y = lpObj->Y;

		MsgSendV2(lpObj, (LPBYTE)&ServerCmd, sizeof(ServerCmd));
		DataSend(lpObj->m_Index, (LPBYTE)&ServerCmd, sizeof(ServerCmd));
	}

	return TRUE;
}

BOOL CItemBagEx::DropPinkChocolateBoxEventItem(int aIndex)
{
	if ( this->m_bLoad == FALSE )
		return FALSE;

	float dur;
	int type;
	int level;
	int x;
	int y;
	int Option1 = 0;
	int Option2 = 0;
	int Option3 = 0;
	int DropItemRate;
	int DropItemNum;
	int ExOption = 0;
	LPOBJ lpObj = &gObj[aIndex];

	DropItemRate = rand() % 100;
	if ( DropItemRate > this->GetItemDropRate() )
	{
		if (this->GetDropZen() <= 0)
		{
			ITEMBAG_DROP_LOG.Output("[EVENT][VALENTINE'S DAY CHOCOLATEBOX] PINKCHOCOLATEBOX NO ItemDrop [%s][%s] [RATE:%d/%d]",
				lpObj->AccountID, lpObj->Name, DropItemRate, this->GetItemDropRate());
			return FALSE;
		}

		MapC[lpObj->MapNumber].MoneyItemDrop(this->GetDropZen(), 
			lpObj->X, lpObj->Y);

		ITEMBAG_DROP_LOG.Output("[EVENT][VALENTINE'S DAY CHOCOLATEBOX] PINKCHOCOLATEBOX Event ItemDrop [%s][%s] [%d Zen] [RATE:%d/%d]",
			lpObj->AccountID, lpObj->Name, this->GetDropZen(), DropItemRate, this->GetItemDropRate());

		return TRUE;
	}

	if ( this->GetBagCount() > 0 )
	{
		DropItemNum = rand() % this->GetBagCount();
		dur = 0;
		x = lpObj->X;
		y = lpObj->Y;
		level = this->GetLevel(DropItemNum);
		type = ItemGetNumberMake(this->BagObject[DropItemNum].m_type, this->BagObject[DropItemNum].m_index);

		if ( type == -1 )
			return FALSE;

		if ( this->BagObject[DropItemNum].m_isskill != 0 )
			Option1 = 1;

		if ( this->BagObject[DropItemNum].m_isluck != 0 )
		{
			Option2 = 0;

			if ( (rand()%2) == 0 )
			{
				Option2 = 1;
			}
		}

		if ( this->BagObject[DropItemNum].m_isoption > 0 )
		{
			int z28Percent = this->BagObject[DropItemNum].m_isoption*100;
			Option3 = rand()%(z28Percent+1);
			Option3 = Option3 / 100;
		}

		DropItemRate = (rand() % 1000)+1;
		if (DropItemRate <= this->GetExItemDropRate())
		{
			if ( this->BagObject[DropItemNum].m_isexitem != 0 )
			{
				ExOption = BoxExcOptions(this->BagObject[DropItemNum].m_isexitem);
			}
		}

		if ( type == ITEMGET(12,15) ||
			 type == ITEMGET(14,13) ||
			 type == ITEMGET(14,14) )
		{
			Option1 = 0;
			Option2 = 0;
			Option3 = 0;
			level = 0;
		}

		if ( type == ITEMGET(14,19) ||
			 type == ITEMGET(14,28) ||
			 type == ITEMGET(13,18) )
		{
			Option1 = 0;
			Option2 = 0;
			Option3 = 0;
		}
			
		ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, x, y, type, level, dur,
			Option1, Option2, Option3, lpObj->m_Index, ExOption, 0);

		ITEMBAG_DROP_LOG.Output("[EVENT][VALENTINE'S DAY CHOCOLATEBOX] PINKCHOCOLATEBOX Event ItemDrop [%s][%s] [%s] : (%d)(%d/%d) Item:(%s)%d Level:%d op1:%d op2:%d op3:%d",
			lpObj->AccountID, lpObj->Name, this->m_sEventName,
			lpObj->MapNumber, x, y, ItemAttribute[type].Name,
			type, level, Option1, Option2, Option3);

		PMSG_SERVERCMD ServerCmd;

		PHeadSubSetB((LPBYTE)&ServerCmd, 0xF3, 0x40, sizeof(ServerCmd));
		ServerCmd.CmdType = 0;
		ServerCmd.X = lpObj->X;
		ServerCmd.Y = lpObj->Y;

		MsgSendV2(lpObj, (LPBYTE)&ServerCmd, sizeof(ServerCmd));
		DataSend(lpObj->m_Index, (LPBYTE)&ServerCmd, sizeof(ServerCmd));
	}

	return TRUE;
}


BOOL CItemBagEx::DropRedChocolateBoxEventItem(int aIndex)
{
	if ( this->m_bLoad == FALSE )
		return FALSE;

	float dur;
	int type;
	int level;
	int x;
	int y;
	int Option1 = 0;
	int Option2 = 0;
	int Option3 = 0;
	int DropItemRate;
	int DropItemNum;
	int ExOption = 0;
	LPOBJ lpObj = &gObj[aIndex];

	DropItemRate = rand() % 100;
	if ( DropItemRate > this->GetItemDropRate() )
	{
		if (this->GetDropZen() <= 0)
		{
			ITEMBAG_DROP_LOG.Output("[EVENT][VALENTINE'S DAY CHOCOLATEBOX] REDCHOCOLATEBOX NO ItemDrop [%s][%s] [RATE:%d/%d]",
				lpObj->AccountID, lpObj->Name, DropItemRate, this->GetItemDropRate());
			return FALSE;
		}

		MapC[lpObj->MapNumber].MoneyItemDrop(this->GetDropZen(), 
			lpObj->X, lpObj->Y);

		ITEMBAG_DROP_LOG.Output("[EVENT][VALENTINE'S DAY CHOCOLATEBOX] REDCHOCOLATEBOX Event ItemDrop [%s][%s] [%d Zen] [RATE:%d/%d]",
			lpObj->AccountID, lpObj->Name, this->GetDropZen(), DropItemRate, this->GetItemDropRate());

		return TRUE;
	}

	if ( this->GetBagCount() > 0 )
	{
		DropItemNum = rand() % this->GetBagCount();
		dur = 0;
		x = lpObj->X;
		y = lpObj->Y;
		level = this->GetLevel(DropItemNum);
		type = ItemGetNumberMake(this->BagObject[DropItemNum].m_type, this->BagObject[DropItemNum].m_index);

		if ( type == -1 )
			return FALSE;

		if ( this->BagObject[DropItemNum].m_isskill != 0 )
			Option1 = 1;

		if ( this->BagObject[DropItemNum].m_isluck != 0 )
		{
			Option2 = 0;

			if ( (rand()%2) == 0 )
			{
				Option2 = 1;
			}
		}

		if ( this->BagObject[DropItemNum].m_isoption > 0 )
		{
			int z28Percent = this->BagObject[DropItemNum].m_isoption*100;
			Option3 = rand()%(z28Percent+1);
			Option3 = Option3 / 100;
		}

		DropItemRate = (rand() % 1000)+1;
		if (DropItemRate <= this->GetExItemDropRate())
		{
			if ( this->BagObject[DropItemNum].m_isexitem != 0 )
			{
				ExOption = BoxExcOptions(this->BagObject[DropItemNum].m_isexitem);
			}
		}

		if ( type == ITEMGET(12,15) ||
			 type == ITEMGET(14,13) ||
			 type == ITEMGET(14,14) ||
			 type == ITEMGET(14,16) )
		{
			Option1 = 0;
			Option2 = 0;
			Option3 = 0;
			level = 0;
		}

		if ( type == ITEMGET(14,19) ||
			 type == ITEMGET(14,28) ||
			 type == ITEMGET(13,18) )
		{
			Option1 = 0;
			Option2 = 0;
			Option3 = 0;
		}
			
		ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, x, y, type, level, dur,
			Option1, Option2, Option3, lpObj->m_Index, ExOption, 0);

		ITEMBAG_DROP_LOG.Output("[EVENT][VALENTINE'S DAY CHOCOLATEBOX] REDCHOCOLATEBOX Event ItemDrop [%s][%s] [%s] : (%d)(%d/%d) Item:(%s)%d Level:%d op1:%d op2:%d op3:%d",
			lpObj->AccountID, lpObj->Name, this->m_sEventName,
			lpObj->MapNumber, x, y, ItemAttribute[type].Name,
			type, level, Option1, Option2, Option3);

		PMSG_SERVERCMD ServerCmd;

		PHeadSubSetB((LPBYTE)&ServerCmd, 0xF3, 0x40, sizeof(ServerCmd));
		ServerCmd.CmdType = 0;
		ServerCmd.X = lpObj->X;
		ServerCmd.Y = lpObj->Y;

		MsgSendV2(lpObj, (LPBYTE)&ServerCmd, sizeof(ServerCmd));
		DataSend(lpObj->m_Index, (LPBYTE)&ServerCmd, sizeof(ServerCmd));
	}

	return TRUE;
}


BOOL CItemBagEx::DropBlueChocolateBoxEventItem(int aIndex)
{
	if ( this->m_bLoad == FALSE )
		return FALSE;

	float dur;
	int type;
	int level;
	int x;
	int y;
	int Option1 = 0;
	int Option2 = 0;
	int Option3 = 0;
	int DropItemRate;
	int DropItemNum;
	int ExOption = 0;
	LPOBJ lpObj = &gObj[aIndex];

	DropItemRate = rand() % 100;
	if ( DropItemRate > this->GetItemDropRate() )
	{
		if (this->GetDropZen() <= 0)
		{
			ITEMBAG_DROP_LOG.Output("[EVENT][VALENTINE'S DAY CHOCOLATEBOX] BLUECHOCOLATEBOX NO ItemDrop [%s][%s] [RATE:%d/%d]",
				lpObj->AccountID, lpObj->Name, DropItemRate, this->GetItemDropRate());
			return FALSE;
		}

		MapC[lpObj->MapNumber].MoneyItemDrop(this->GetDropZen(), 
			lpObj->X, lpObj->Y);

		ITEMBAG_DROP_LOG.Output("[EVENT][VALENTINE'S DAY CHOCOLATEBOX] BLUECHOCOLATEBOX Event ItemDrop [%s][%s] [%d Zen] [RATE:%d/%d]",
			lpObj->AccountID, lpObj->Name, this->GetDropZen(), DropItemRate, this->GetItemDropRate());

		return TRUE;
	}

	if ( this->GetBagCount() > 0 )
	{
		DropItemNum = rand() % this->GetBagCount();
		dur = 0;
		x = lpObj->X;
		y = lpObj->Y;
		level = this->GetLevel(DropItemNum);
		type = ItemGetNumberMake(this->BagObject[DropItemNum].m_type, this->BagObject[DropItemNum].m_index);

		if ( type == -1 )
			return FALSE;

		if ( this->BagObject[DropItemNum].m_isskill != 0 )
			Option1 = 1;

		if ( this->BagObject[DropItemNum].m_isluck != 0 )
		{
			Option2 = 0;

			if ( (rand()%2) == 0 )
			{
				Option2 = 1;
			}
		}

		if ( this->BagObject[DropItemNum].m_isoption > 0 )
		{
			int z28Percent = this->BagObject[DropItemNum].m_isoption*100;
			Option3 = rand()%(z28Percent+1);
			Option3 = Option3 / 100;
		}

		DropItemRate = (rand() % 1000)+1;
		if (DropItemRate <= this->GetExItemDropRate())
		{
			if ( this->BagObject[DropItemNum].m_isexitem != 0 )
			{
				ExOption = BoxExcOptions(this->BagObject[DropItemNum].m_isexitem);
			}
		}

		if ( type == ITEMGET(12,15) ||
			 type == ITEMGET(14,13) ||
			 type == ITEMGET(14,14)  )
		{
			Option1 = 0;
			Option2 = 0;
			Option3 = 0;
			level = 0;
		}

		if ( type == ITEMGET(13,8) ||
			 type == ITEMGET(13,9) ||
			 type == ITEMGET(13,12) ||
			 type == ITEMGET(13,21) )
		{
			level = 0;
		}
			
		ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, x, y, type, level, dur,
			Option1, Option2, Option3, lpObj->m_Index, ExOption, 0);

		ITEMBAG_DROP_LOG.Output("[EVENT][VALENTINE'S DAY CHOCOLATEBOX] BLUECHOCOLATEBOX Event ItemDrop [%s][%s] [%s] : (%d)(%d/%d) Item:(%s)%d Level:%d op1:%d op2:%d op3:%d",
			lpObj->AccountID, lpObj->Name, this->m_sEventName,
			lpObj->MapNumber, x, y, ItemAttribute[type].Name,
			type, level, Option1, Option2, Option3);

		PMSG_SERVERCMD ServerCmd;

		PHeadSubSetB((LPBYTE)&ServerCmd, 0xF3, 0x40, sizeof(ServerCmd));
		ServerCmd.CmdType = 0;
		ServerCmd.X = lpObj->X;
		ServerCmd.Y = lpObj->Y;

		MsgSendV2(lpObj, (LPBYTE)&ServerCmd, sizeof(ServerCmd));
		DataSend(lpObj->m_Index, (LPBYTE)&ServerCmd, sizeof(ServerCmd));
	}

	return TRUE;
}



BOOL CItemBagEx::DropLightPurpleCandyBoxEventItem(int aIndex)
{
	if ( this->m_bLoad == FALSE )
		return FALSE;

	float dur;
	int type;
	int level;
	int x;
	int y;
	int Option1 = 0;
	int Option2 = 0;
	int Option3 = 0;
	int DropItemRate;
	int DropItemNum;
	int ExOption = 0;
	LPOBJ lpObj = &gObj[aIndex];

	DropItemRate = rand() % 100;
	if ( DropItemRate > this->GetItemDropRate() )
	{
		if (this->GetDropZen() <= 0)
		{
			ITEMBAG_DROP_LOG.Output("[EVENT WHITE DAY CANDY BOX] LIGHT PURPLE BOX NO ItemDrop [%s][%s] [RATE:%d/%d]",
				lpObj->AccountID, lpObj->Name, DropItemRate, this->GetItemDropRate());
			return FALSE;
		}

		MapC[lpObj->MapNumber].MoneyItemDrop(this->GetDropZen(), 
			lpObj->X, lpObj->Y);

		ITEMBAG_DROP_LOG.Output("[EVENT WHITE DAY CANDY BOX] LIGHT PURPLE BOX Event ItemDrop [%s][%s] [%d Zen] [RATE:%d/%d]",
			lpObj->AccountID, lpObj->Name, this->GetDropZen(), DropItemRate, this->GetItemDropRate());

		return TRUE;
	}

	if ( this->GetBagCount() > 0 )
	{
		DropItemNum = rand() % this->GetBagCount();
		dur = 0;
		x = lpObj->X;
		y = lpObj->Y;
		level = this->GetLevel(DropItemNum);
		type = ItemGetNumberMake(this->BagObject[DropItemNum].m_type, this->BagObject[DropItemNum].m_index);

		if ( type == -1 )
			return FALSE;

		if ( this->BagObject[DropItemNum].m_isskill != 0 )
			Option1 = 1;

		if ( this->BagObject[DropItemNum].m_isluck != 0 )
		{
			Option2 = 0;

			if ( (rand()%2) == 0 )
			{
				Option2 = 1;
			}
		}

		if ( this->BagObject[DropItemNum].m_isoption > 0 )
		{
			int z28Percent = this->BagObject[DropItemNum].m_isoption*100;
			Option3 = rand()%(z28Percent+1);
			Option3 = Option3 / 100;
		}

		DropItemRate = (rand() % 1000)+1;
		if (DropItemRate <= this->GetExItemDropRate())
		{
			if ( this->BagObject[DropItemNum].m_isexitem != 0 )
			{
				ExOption = BoxExcOptions(this->BagObject[DropItemNum].m_isexitem);
			}
		}

		if ( type == ITEMGET(12,15) ||
			 type == ITEMGET(14,13) ||
			 type == ITEMGET(14,14) )
		{
			Option1 = 0;
			Option2 = 0;
			Option3 = 0;
			level = 0;
		}

		if ( type == ITEMGET(14,19) ||
			 type == ITEMGET(14,28) ||
			 type == ITEMGET(13,18) )
		{
			Option1 = 0;
			Option2 = 0;
			Option3 = 0;
		}
			
		ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, x, y, type, level, dur,
			Option1, Option2, Option3, lpObj->m_Index, ExOption, 0);

		ITEMBAG_DROP_LOG.Output("[EVENT WHITE DAY CANDY BOX] LIGHT PURPLE BOX Event ItemDrop [%s][%s] [%s] : (%d)(%d/%d) Item:(%s)%d Level:%d op1:%d op2:%d op3:%d",
			lpObj->AccountID, lpObj->Name, this->m_sEventName,
			lpObj->MapNumber, x, y, ItemAttribute[type].Name,
			type, level, Option1, Option2, Option3);

		PMSG_SERVERCMD ServerCmd;

		PHeadSubSetB((LPBYTE)&ServerCmd, 0xF3, 0x40, sizeof(ServerCmd));
		ServerCmd.CmdType = 0;
		ServerCmd.X = lpObj->X;
		ServerCmd.Y = lpObj->Y;

		MsgSendV2(lpObj, (LPBYTE)&ServerCmd, sizeof(ServerCmd));
		DataSend(lpObj->m_Index, (LPBYTE)&ServerCmd, sizeof(ServerCmd));
	}

	return TRUE;
}


BOOL CItemBagEx::DropVermilionCandyBoxEventItem(int aIndex)
{
	if ( this->m_bLoad == FALSE )
		return FALSE;

	float dur;
	int type;
	int level;
	int x;
	int y;
	int Option1 = 0;
	int Option2 = 0;
	int Option3 = 0;
	int DropItemRate;
	int DropItemNum;
	int ExOption = 0;
	LPOBJ lpObj = &gObj[aIndex];

	DropItemRate = rand() % 100;
	if ( DropItemRate > this->GetItemDropRate() )
	{
		if (this->GetDropZen() <= 0)
		{
			ITEMBAG_DROP_LOG.Output("[EVENT WHITE DAY CANDY BOX] VERMILION BOX NO ItemDrop [%s][%s] [RATE:%d/%d]",
				lpObj->AccountID, lpObj->Name, DropItemRate, this->GetItemDropRate());
			return FALSE;
		}

		MapC[lpObj->MapNumber].MoneyItemDrop(this->GetDropZen(), 
			lpObj->X, lpObj->Y);

		ITEMBAG_DROP_LOG.Output("[EVENT WHITE DAY CANDY BOX] VERMILION BOX Event ItemDrop [%s][%s] [%d Zen] [RATE:%d/%d]",
			lpObj->AccountID, lpObj->Name, this->GetDropZen(), DropItemRate, this->GetItemDropRate());

		return TRUE;
	}

	if ( this->GetBagCount() > 0 )
	{
		DropItemNum = rand() % this->GetBagCount();
		dur = 0;
		x = lpObj->X;
		y = lpObj->Y;
		level = this->GetLevel(DropItemNum);
		type = ItemGetNumberMake(this->BagObject[DropItemNum].m_type, this->BagObject[DropItemNum].m_index);

		if ( type == -1 )
			return FALSE;

		if ( this->BagObject[DropItemNum].m_isskill != 0 )
			Option1 = 1;

		if ( this->BagObject[DropItemNum].m_isluck != 0 )
		{
			Option2 = 0;

			if ( (rand()%2) == 0 )
			{
				Option2 = 1;
			}
		}

		if ( this->BagObject[DropItemNum].m_isoption > 0 )
		{
			int z28Percent = this->BagObject[DropItemNum].m_isoption*100;
			Option3 = rand()%(z28Percent+1);
			Option3 = Option3 / 100;
		}

		DropItemRate = (rand() % 1000)+1;
		if (DropItemRate <= this->GetExItemDropRate())
		{
			if ( this->BagObject[DropItemNum].m_isexitem != 0 )
			{
				ExOption = BoxExcOptions(this->BagObject[DropItemNum].m_isexitem);
			}
		}

		if ( type == ITEMGET(12,15) ||
			 type == ITEMGET(14,13) ||
			 type == ITEMGET(14,14) ||
			 type == ITEMGET(14,16) )
		{
			Option1 = 0;
			Option2 = 0;
			Option3 = 0;
			level = 0;
		}

		if ( type == ITEMGET(14,19) ||
			 type == ITEMGET(14,28) ||
			 type == ITEMGET(13,18) )
		{
			Option1 = 0;
			Option2 = 0;
			Option3 = 0;
		}
			
		ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, x, y, type, level, dur,
			Option1, Option2, Option3, lpObj->m_Index, ExOption, 0);

		ITEMBAG_DROP_LOG.Output("[EVENT WHITE DAY CANDY BOX] VERMILION BOX Event ItemDrop [%s][%s] [%s] : (%d)(%d/%d) Item:(%s)%d Level:%d op1:%d op2:%d op3:%d",
			lpObj->AccountID, lpObj->Name, this->m_sEventName,
			lpObj->MapNumber, x, y, ItemAttribute[type].Name,
			type, level, Option1, Option2, Option3);

		PMSG_SERVERCMD ServerCmd;

		PHeadSubSetB((LPBYTE)&ServerCmd, 0xF3, 0x40, sizeof(ServerCmd));
		ServerCmd.CmdType = 0;
		ServerCmd.X = lpObj->X;
		ServerCmd.Y = lpObj->Y;

		MsgSendV2(lpObj, (LPBYTE)&ServerCmd, sizeof(ServerCmd));
		DataSend(lpObj->m_Index, (LPBYTE)&ServerCmd, sizeof(ServerCmd));
	}

	return TRUE;
}


BOOL CItemBagEx::DropDeepBlueCandyBoxEventItem(int aIndex)
{
	if ( this->m_bLoad == FALSE )
		return FALSE;

	float dur;
	int type;
	int level;
	int x;
	int y;
	int Option1 = 0;
	int Option2 = 0;
	int Option3 = 0;
	int DropItemRate;
	int DropItemNum;
	int ExOption = 0;
	LPOBJ lpObj = &gObj[aIndex];

	DropItemRate = rand() % 100;
	if ( DropItemRate > this->GetItemDropRate() )
	{
		if (this->GetDropZen() <= 0)
		{
			ITEMBAG_DROP_LOG.Output("[EVENT WHITE DAY CANDY BOX] DEEP BLUE BOX NO ItemDrop [%s][%s] [RATE:%d/%d]",
				lpObj->AccountID, lpObj->Name, DropItemRate, this->GetItemDropRate());
			return FALSE;
		}

		MapC[lpObj->MapNumber].MoneyItemDrop(this->GetDropZen(), 
			lpObj->X, lpObj->Y);

		ITEMBAG_DROP_LOG.Output("[EVENT WHITE DAY CANDY BOX] DEEP BLUE BOX Event ItemDrop [%s][%s] [%d Zen] [RATE:%d/%d]",
			lpObj->AccountID, lpObj->Name, this->GetDropZen(), DropItemRate, this->GetItemDropRate());

		return TRUE;
	}

	if ( this->GetBagCount() > 0 )
	{
		DropItemNum = rand() % this->GetBagCount();
		dur = 0;
		x = lpObj->X;
		y = lpObj->Y;
		level = this->GetLevel(DropItemNum);
		type = ItemGetNumberMake(this->BagObject[DropItemNum].m_type, this->BagObject[DropItemNum].m_index);

		if ( type == -1 )
			return FALSE;

		if ( this->BagObject[DropItemNum].m_isskill != 0 )
			Option1 = 1;

		if ( this->BagObject[DropItemNum].m_isluck != 0 )
		{
			Option2 = 0;

			if ( (rand()%2) == 0 )
			{
				Option2 = 1;
			}
		}

		if ( this->BagObject[DropItemNum].m_isoption > 0 )
		{
			int z28Percent = this->BagObject[DropItemNum].m_isoption*100;
			Option3 = rand()%(z28Percent+1);
			Option3 = Option3 / 100;
		}

		DropItemRate = (rand() % 1000)+1;
		if (DropItemRate <= this->GetExItemDropRate())
		{
			if ( this->BagObject[DropItemNum].m_isexitem != 0 )
			{
				ExOption = BoxExcOptions(this->BagObject[DropItemNum].m_isexitem);
			}
		}

		if ( type == ITEMGET(12,15) ||
			 type == ITEMGET(14,13) ||
			 type == ITEMGET(14,14)  )
		{
			Option1 = 0;
			Option2 = 0;
			Option3 = 0;
			level = 0;
		}

		if ( type == ITEMGET(13,8) ||
			 type == ITEMGET(13,9) ||
			 type == ITEMGET(13,12) ||
			 type == ITEMGET(13,21) )
		{
			level = 0;
		}
			
		ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, x, y, type, level, dur,
			Option1, Option2, Option3, lpObj->m_Index, ExOption, 0);

		ITEMBAG_DROP_LOG.Output("[EVENT WHITE DAY CANDY BOX] DEEP BLUE BOX Event ItemDrop [%s][%s] [%s] : (%d)(%d/%d) Item:(%s)%d Level:%d op1:%d op2:%d op3:%d",
			lpObj->AccountID, lpObj->Name, this->m_sEventName,
			lpObj->MapNumber, x, y, ItemAttribute[type].Name,
			type, level, Option1, Option2, Option3);

		PMSG_SERVERCMD ServerCmd;

		PHeadSubSetB((LPBYTE)&ServerCmd, 0xF3, 0x40, sizeof(ServerCmd));
		ServerCmd.CmdType = 0;
		ServerCmd.X = lpObj->X;
		ServerCmd.Y = lpObj->Y;

		MsgSendV2(lpObj, (LPBYTE)&ServerCmd, sizeof(ServerCmd));
		DataSend(lpObj->m_Index, (LPBYTE)&ServerCmd, sizeof(ServerCmd));
	}

	return TRUE;
}

BOOL CItemBagEx::GreenMysteryBoxEventItem(int aIndex)
{
	if ( this->m_bLoad == FALSE )
		return FALSE;

	float dur;
	int type;
	int level;
	int x;
	int y;
	int Option1 = 0;
	int Option2 = 0;
	int Option3 = 0;
	int DropItemRate;
	int DropItemNum;
	int ExOption = 0;
	LPOBJ lpObj = &gObj[aIndex];

	DropItemRate = rand() % 100;
	if ( DropItemRate > this->GetItemDropRate() )
	{
		if (this->GetDropZen() <= 0)
		{
			ITEMBAG_DROP_LOG.Output("[Mystery BOX] Green Mystery Box NO ItemDrop [%s][%s] [RATE:%d/%d]",
				lpObj->AccountID, lpObj->Name, DropItemRate, this->GetItemDropRate());
			return FALSE;
		}

		MapC[lpObj->MapNumber].MoneyItemDrop(this->GetDropZen(), 
			lpObj->X, lpObj->Y);

		ITEMBAG_DROP_LOG.Output("[Mystery BOX] Green Mystery Box ItemDrop [%s][%s] [%d Zen] [RATE:%d/%d]",
			lpObj->AccountID, lpObj->Name, this->GetDropZen(), DropItemRate, this->GetItemDropRate());

		return TRUE;
	}

	if ( this->GetBagCount() > 0 )
	{
		DropItemNum = rand() % this->GetBagCount();
		dur = 0;
		x = lpObj->X;
		y = lpObj->Y;
		level = this->GetLevel(DropItemNum);
		type = ItemGetNumberMake(this->BagObject[DropItemNum].m_type, this->BagObject[DropItemNum].m_index);

		if ( type == -1 )
			return FALSE;

		if ( this->BagObject[DropItemNum].m_isskill != 0 )
			Option1 = 1;

		if ( this->BagObject[DropItemNum].m_isluck != 0 )
		{
			Option2 = 0;

			if ( (rand()%2) == 0 )
			{
				Option2 = 1;
			}
		}

		if ( this->BagObject[DropItemNum].m_isoption > 0 )
		{
			int z28Percent = this->BagObject[DropItemNum].m_isoption*100;
			Option3 = rand()%(z28Percent+1);
			Option3 = Option3 / 100;
		}

		DropItemRate = (rand() % 1000)+1;
		if (DropItemRate <= this->GetExItemDropRate())
		{
			if ( this->BagObject[DropItemNum].m_isexitem != 0 )
			{
				ExOption = BoxExcOptions(this->BagObject[DropItemNum].m_isexitem);
			}
		}

		if ( type == ITEMGET(12,15) ||
			 type == ITEMGET(14,13) ||
			 type == ITEMGET(14,14)  )
		{
			Option1 = 0;
			Option2 = 0;
			Option3 = 0;
			level = 0;
		}

		if ( type == ITEMGET(13,8) ||
			 type == ITEMGET(13,9) ||
			 type == ITEMGET(13,12) ||
			 type == ITEMGET(13,21) )
		{
			level = 0;
		}
			
		ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, x, y, type, level, dur,
			Option1, Option2, Option3, lpObj->m_Index, ExOption, 0);

		ITEMBAG_DROP_LOG.Output("[Mystery BOX] Green Mystery Box ItemDrop [%s][%s] [%s] : (%d)(%d/%d) Item:(%s)%d Level:%d op1:%d op2:%d op3:%d",
			lpObj->AccountID, lpObj->Name, this->m_sEventName,
			lpObj->MapNumber, x, y, ItemAttribute[type].Name,
			type, level, Option1, Option2, Option3);

		PMSG_SERVERCMD ServerCmd;

		PHeadSubSetB((LPBYTE)&ServerCmd, 0xF3, 0x40, sizeof(ServerCmd));
		ServerCmd.CmdType = 0;
		ServerCmd.X = lpObj->X;
		ServerCmd.Y = lpObj->Y;

		MsgSendV2(lpObj, (LPBYTE)&ServerCmd, sizeof(ServerCmd));
		DataSend(lpObj->m_Index, (LPBYTE)&ServerCmd, sizeof(ServerCmd));
	}

	return TRUE;
}

BOOL CItemBagEx::RedMysteryBoxEventItem(int aIndex)
{
	if ( this->m_bLoad == FALSE )
		return FALSE;

	float dur;
	int type;
	int level;
	int x;
	int y;
	int Option1 = 0;
	int Option2 = 0;
	int Option3 = 0;
	int DropItemRate;
	int DropItemNum;
	int ExOption = 0;
	LPOBJ lpObj = &gObj[aIndex];

	DropItemRate = rand() % 100;
	if ( DropItemRate > this->GetItemDropRate() )
	{
		if (this->GetDropZen() <= 0)
		{
			ITEMBAG_DROP_LOG.Output("[Mystery BOX] Red Mystery Box NO ItemDrop [%s][%s] [RATE:%d/%d]",
				lpObj->AccountID, lpObj->Name, DropItemRate, this->GetItemDropRate());
			return FALSE;
		}

		MapC[lpObj->MapNumber].MoneyItemDrop(this->GetDropZen(), 
			lpObj->X, lpObj->Y);

		ITEMBAG_DROP_LOG.Output("[Mystery BOX] Red Mystery Box ItemDrop [%s][%s] [%d Zen] [RATE:%d/%d]",
			lpObj->AccountID, lpObj->Name, this->GetDropZen(), DropItemRate, this->GetItemDropRate());

		return TRUE;
	}

	if ( this->GetBagCount() > 0 )
	{
		DropItemNum = rand() % this->GetBagCount();
		dur = 0;
		x = lpObj->X;
		y = lpObj->Y;
		level = this->GetLevel(DropItemNum);
		type = ItemGetNumberMake(this->BagObject[DropItemNum].m_type, this->BagObject[DropItemNum].m_index);

		if ( type == -1 )
			return FALSE;

		if ( this->BagObject[DropItemNum].m_isskill != 0 )
			Option1 = 1;

		if ( this->BagObject[DropItemNum].m_isluck != 0 )
		{
			Option2 = 0;

			if ( (rand()%2) == 0 )
			{
				Option2 = 1;
			}
		}

		if ( this->BagObject[DropItemNum].m_isoption > 0 )
		{
			int z28Percent = this->BagObject[DropItemNum].m_isoption*100;
			Option3 = rand()%(z28Percent+1);
			Option3 = Option3 / 100;
		}

		DropItemRate = (rand() % 1000)+1;
		if (DropItemRate <= this->GetExItemDropRate())
		{
			if ( this->BagObject[DropItemNum].m_isexitem != 0 )
			{
				ExOption = BoxExcOptions(this->BagObject[DropItemNum].m_isexitem);
			}
		}

		if ( type == ITEMGET(12,15) ||
			 type == ITEMGET(14,13) ||
			 type == ITEMGET(14,14)  )
		{
			Option1 = 0;
			Option2 = 0;
			Option3 = 0;
			level = 0;
		}

		if ( type == ITEMGET(13,8) ||
			 type == ITEMGET(13,9) ||
			 type == ITEMGET(13,12) ||
			 type == ITEMGET(13,21) )
		{
			level = 0;
		}
			
		ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, x, y, type, level, dur,
			Option1, Option2, Option3, lpObj->m_Index, ExOption, 0);

		ITEMBAG_DROP_LOG.Output("[Mystery BOX] Red Mystery Box ItemDrop [%s][%s] [%s] : (%d)(%d/%d) Item:(%s)%d Level:%d op1:%d op2:%d op3:%d",
			lpObj->AccountID, lpObj->Name, this->m_sEventName,
			lpObj->MapNumber, x, y, ItemAttribute[type].Name,
			type, level, Option1, Option2, Option3);

		PMSG_SERVERCMD ServerCmd;

		PHeadSubSetB((LPBYTE)&ServerCmd, 0xF3, 0x40, sizeof(ServerCmd));
		ServerCmd.CmdType = 0;
		ServerCmd.X = lpObj->X;
		ServerCmd.Y = lpObj->Y;

		MsgSendV2(lpObj, (LPBYTE)&ServerCmd, sizeof(ServerCmd));
		DataSend(lpObj->m_Index, (LPBYTE)&ServerCmd, sizeof(ServerCmd));
	}

	return TRUE;
}

BOOL CItemBagEx::PurpleMysteryBoxEventItem(int aIndex)
{
	if ( this->m_bLoad == FALSE )
		return FALSE;

	float dur;
	int type;
	int level;
	int x;
	int y;
	int Option1 = 0;
	int Option2 = 0;
	int Option3 = 0;
	int DropItemRate;
	int DropItemNum;
	int ExOption = 0;
	LPOBJ lpObj = &gObj[aIndex];

	DropItemRate = rand() % 100;
	if ( DropItemRate > this->GetItemDropRate() )
	{
		if (this->GetDropZen() <= 0)
		{
			ITEMBAG_DROP_LOG.Output("[Mystery BOX] Purple Mystery Box NO ItemDrop [%s][%s] [RATE:%d/%d]",
				lpObj->AccountID, lpObj->Name, DropItemRate, this->GetItemDropRate());
			return FALSE;
		}

		MapC[lpObj->MapNumber].MoneyItemDrop(this->GetDropZen(), 
			lpObj->X, lpObj->Y);

		ITEMBAG_DROP_LOG.Output("[Mystery BOX] Purple Mystery Box ItemDrop [%s][%s] [%d Zen] [RATE:%d/%d]",
			lpObj->AccountID, lpObj->Name, this->GetDropZen(), DropItemRate, this->GetItemDropRate());

		return TRUE;
	}

	if ( this->GetBagCount() > 0 )
	{
		DropItemNum = rand() % this->GetBagCount();
		dur = 0;
		x = lpObj->X;
		y = lpObj->Y;
		level = this->GetLevel(DropItemNum);
		type = ItemGetNumberMake(this->BagObject[DropItemNum].m_type, this->BagObject[DropItemNum].m_index);

		if ( type == -1 )
			return FALSE;

		if ( this->BagObject[DropItemNum].m_isskill != 0 )
			Option1 = 1;

		if ( this->BagObject[DropItemNum].m_isluck != 0 )
		{
			Option2 = 0;

			if ( (rand()%2) == 0 )
			{
				Option2 = 1;
			}
		}

		if ( this->BagObject[DropItemNum].m_isoption > 0 )
		{
			int z28Percent = this->BagObject[DropItemNum].m_isoption*100;
			Option3 = rand()%(z28Percent+1);
			Option3 = Option3 / 100;
		}

		DropItemRate = (rand() % 1000)+1;
		if (DropItemRate <= this->GetExItemDropRate())
		{
			if ( this->BagObject[DropItemNum].m_isexitem != 0 )
			{
				ExOption = BoxExcOptions(this->BagObject[DropItemNum].m_isexitem);
			}
		}

		if ( type == ITEMGET(12,15) ||
			 type == ITEMGET(14,13) ||
			 type == ITEMGET(14,14)  )
		{
			Option1 = 0;
			Option2 = 0;
			Option3 = 0;
			level = 0;
		}

		if ( type == ITEMGET(13,8) ||
			 type == ITEMGET(13,9) ||
			 type == ITEMGET(13,12) ||
			 type == ITEMGET(13,21) )
		{
			level = 0;
		}
			
		ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, x, y, type, level, dur,
			Option1, Option2, Option3, lpObj->m_Index, ExOption, 0);

		ITEMBAG_DROP_LOG.Output("[Mystery BOX] Purple Mystery Box ItemDrop [%s][%s] [%s] : (%d)(%d/%d) Item:(%s)%d Level:%d op1:%d op2:%d op3:%d ex:%d",
			lpObj->AccountID, lpObj->Name, this->m_sEventName,
			lpObj->MapNumber, x, y, ItemAttribute[type].Name,
			type, level, Option1, Option2, Option3, ExOption);

		PMSG_SERVERCMD ServerCmd;

		PHeadSubSetB((LPBYTE)&ServerCmd, 0xF3, 0x40, sizeof(ServerCmd));
		ServerCmd.CmdType = 0;
		ServerCmd.X = lpObj->X;
		ServerCmd.Y = lpObj->Y;

		MsgSendV2(lpObj, (LPBYTE)&ServerCmd, sizeof(ServerCmd));
		DataSend(lpObj->m_Index, (LPBYTE)&ServerCmd, sizeof(ServerCmd));
	}

	return TRUE;
}
BOOL CItemBagEx::CherryBlossomBoxEventItem(int aIndex)
{
	if ( this->m_bLoad == FALSE )
		return FALSE;

	float dur;
	int type;
	int level;
	int x;
	int y;
	int Option1 = 0;
	int Option2 = 0;
	int Option3 = 0;
	int DropItemRate;
	int DropItemNum;
	int ExOption = 0;
	LPOBJ lpObj = &gObj[aIndex];

	DropItemRate = rand() % 100;
	if ( DropItemRate > this->GetItemDropRate() )
	{
		if (this->GetDropZen() <= 0)
		{
			ITEMBAG_DROP_LOG.Output("[CHERRY BLOSSOM EVENT] Cherry Blossom NO ItemDrop [%s][%s] [RATE:%d/%d]",
				lpObj->AccountID, lpObj->Name, DropItemRate, this->GetItemDropRate());
			return FALSE;
		}

		MapC[lpObj->MapNumber].MoneyItemDrop(this->GetDropZen(), 
			lpObj->X, lpObj->Y);

		ITEMBAG_DROP_LOG.Output("[CHERRY BLOSSOM EVENT] Cherry Blossom Box ItemDrop [%s][%s] [%d Zen] [RATE:%d/%d]",
			lpObj->AccountID, lpObj->Name, this->GetDropZen(), DropItemRate, this->GetItemDropRate());

		return TRUE;
	}

	if ( this->GetBagCount() > 0 )
	{
		DropItemNum = rand() % this->GetBagCount();
		dur = 0;
		x = lpObj->X;
		y = lpObj->Y;
		level = this->GetLevel(DropItemNum);
		type = ItemGetNumberMake(this->BagObject[DropItemNum].m_type, this->BagObject[DropItemNum].m_index);

		if ( type == -1 )
			return FALSE;

		if ( this->BagObject[DropItemNum].m_isskill != 0 )
			Option1 = 1;

		if ( this->BagObject[DropItemNum].m_isluck != 0 )
		{
			Option2 = 0;

			if ( (rand()%2) == 0 )
			{
				Option2 = 1;
			}
		}

		if ( this->BagObject[DropItemNum].m_isoption > 0 )
		{
			int z28Percent = this->BagObject[DropItemNum].m_isoption*100;
			Option3 = rand()%(z28Percent+1);
			Option3 = Option3 / 100;
		}

		DropItemRate = (rand() % 1000)+1;
		if (DropItemRate <= this->GetExItemDropRate())
		{
			if ( this->BagObject[DropItemNum].m_isexitem != 0 )
			{
				ExOption = BoxExcOptions(this->BagObject[DropItemNum].m_isexitem);
			}
		}

		if ( type == ITEMGET(12,15) ||
			 type == ITEMGET(14,13) ||
			 type == ITEMGET(14,14)  )
		{
			Option1 = 0;
			Option2 = 0;
			Option3 = 0;
			level = 0;
		}

		if ( type == ITEMGET(13,8) ||
			 type == ITEMGET(13,9) ||
			 type == ITEMGET(13,12) ||
			 type == ITEMGET(13,21) )
		{
			level = 0;
		}

		if (type >= ITEMGET(14,88) && type <= ITEMGET(14,90))
		{
			dur = 1;
		}
			
		ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, x, y, type, level, dur,
			Option1, Option2, Option3, lpObj->m_Index, ExOption, 0);

		ITEMBAG_DROP_LOG.Output("[CHERRY BLOSSOM EVENT] Cherry Blossom Box ItemDrop [%s][%s] [%s] : (%d)(%d/%d) Item:(%s)%d Level:%d op1:%d op2:%d op3:%d",
			lpObj->AccountID, lpObj->Name, this->m_sEventName,
			lpObj->MapNumber, x, y, ItemAttribute[type].Name,
			type, level, Option1, Option2, Option3);

		PMSG_SERVERCMD ServerCmd;

		PHeadSubSetB((LPBYTE)&ServerCmd, 0xF3, 0x40, sizeof(ServerCmd));
		ServerCmd.CmdType = 0;
		ServerCmd.X = lpObj->X;
		ServerCmd.Y = lpObj->Y;

		MsgSendV2(lpObj, (LPBYTE)&ServerCmd, sizeof(ServerCmd));
		DataSend(lpObj->m_Index, (LPBYTE)&ServerCmd, sizeof(ServerCmd));
	}

	return TRUE;
}
BOOL CItemBagEx::GMBoxEventItem(int aIndex)
{
	if ( this->m_bLoad == FALSE )
		return FALSE;

	float dur;
	int type;
	int level;
	int x;
	int y;
	int Option1 = 0;
	int Option2 = 0;
	int Option3 = 0;
	int DropItemRate;
	int DropItemNum;
	int ExOption = 0;
	LPOBJ lpObj = &gObj[aIndex];

	DropItemRate = rand() % 100;
	if ( DropItemRate > this->GetItemDropRate() )
	{
		if (this->GetDropZen() <= 0)
		{
			ITEMBAG_DROP_LOG.Output("[GM BOX] GM Box NO ItemDrop [%s][%s] [RATE:%d/%d]",
				lpObj->AccountID, lpObj->Name, DropItemRate, this->GetItemDropRate());
			return FALSE;
		}

		MapC[lpObj->MapNumber].MoneyItemDrop(this->GetDropZen(), 
			lpObj->X, lpObj->Y);

		ITEMBAG_DROP_LOG.Output("[GM BOX] GM Box ItemDrop [%s][%s] [%d Zen] [RATE:%d/%d]",
			lpObj->AccountID, lpObj->Name, this->GetDropZen(), DropItemRate, this->GetItemDropRate());

		return TRUE;
	}

	if ( this->GetBagCount() > 0 )
	{
		DropItemNum = rand() % this->GetBagCount();
		dur = 0;
		x = lpObj->X;
		y = lpObj->Y;
		level = this->GetLevel(DropItemNum);
		type = ItemGetNumberMake(this->BagObject[DropItemNum].m_type, this->BagObject[DropItemNum].m_index);

		if ( type == -1 )
			return FALSE;

		if ( this->BagObject[DropItemNum].m_isskill != 0 )
			Option1 = 1;

		if ( this->BagObject[DropItemNum].m_isluck != 0 )
		{
			Option2 = 0;

			if ( (rand()%2) == 0 )
			{
				Option2 = 1;
			}
		}

		if ( this->BagObject[DropItemNum].m_isoption > 0 )
		{
			int z28Percent = this->BagObject[DropItemNum].m_isoption*100;
			Option3 = rand()%(z28Percent+1);
			Option3 = Option3 / 100;
		}

		DropItemRate = (rand() % 1000)+1;
		if (DropItemRate <= this->GetExItemDropRate())
		{
			if ( this->BagObject[DropItemNum].m_isexitem != 0 )
			{
				ExOption = BoxExcOptions(this->BagObject[DropItemNum].m_isexitem);
			}
		}

		if ( type == ITEMGET(12,15) ||
			 type == ITEMGET(14,13) ||
			 type == ITEMGET(14,14)  )
		{
			Option1 = 0;
			Option2 = 0;
			Option3 = 0;
			level = 0;
		}

		if ( type == ITEMGET(13,8) ||
			 type == ITEMGET(13,9) ||
			 type == ITEMGET(13,12) ||
			 type == ITEMGET(13,21) )
		{
			level = 0;
		}
		
		ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, x, y, type, level, dur,
			Option1, Option2, Option3, lpObj->m_Index, ExOption, 0);

		ITEMBAG_DROP_LOG.Output("[GM BOX] GM Box ItemDrop [%s][%s] [%s] : (%d)(%d/%d) Item:(%s)%d Level:%d op1:%d op2:%d op3:%d",
			lpObj->AccountID, lpObj->Name, this->m_sEventName,
			lpObj->MapNumber, x, y, ItemAttribute[type].Name,
			type, level, Option1, Option2, Option3);

		PMSG_SERVERCMD ServerCmd;

		PHeadSubSetB((LPBYTE)&ServerCmd, 0xF3, 0x40, sizeof(ServerCmd));
		ServerCmd.CmdType = 0;
		ServerCmd.X = lpObj->X;
		ServerCmd.Y = lpObj->Y;

		MsgSendV2(lpObj, (LPBYTE)&ServerCmd, sizeof(ServerCmd));
		DataSend(lpObj->m_Index, (LPBYTE)&ServerCmd, sizeof(ServerCmd));
	}

	return TRUE;
}

BOOL CItemBagEx::DropSelupanEventItem(int aIndex, BYTE btMapNumber, BYTE cX, BYTE cY)
{
	if ( this->m_bLoad == FALSE )
	{
		return FALSE;
	}

	float dur;
	int type;
	int level;
	int x;
	int y;
	int Option1 = 0;
	int Option2 = 0;
	int Option3 = 0;
	int DropItemNum;
	int DropItemRate;
	int ExOption = 0;
	LPOBJ lpObj = &gObj[aIndex];
	int iItemBagCount = this->GetBagCount();

	int itemDrops = rand()%Raklion.SelupanDropItems;
	itemDrops++;

	for(int i=0;i<itemDrops;i++)
	{
		DropItemRate = rand() % 100;
		if ( DropItemRate > this->GetItemDropRate() )
		{
			if (this->GetDropZen() <= 0)
			{
				ITEMBAG_DROP_LOG.Output("[RAKLION EVENT] SELUPAN NO ItemDrop [%s][%s] [RATE:%d/%d]",
					lpObj->AccountID, lpObj->Name, DropItemRate, this->GetItemDropRate());
				continue;
			}

			MapC[lpObj->MapNumber].MoneyItemDrop(this->GetDropZen(), 
				lpObj->X, lpObj->Y);

			ITEMBAG_DROP_LOG.Output("[RAKLION EVENT] SELUPAN Event ItemDrop [%s][%s] [%d Zen] [RATE:%d/%d]",
				lpObj->AccountID, lpObj->Name, this->GetDropZen(), DropItemRate, this->GetItemDropRate());

			continue;
		}

		if ( this->GetBagCount() > 0 )
		{
			DropItemNum =  rand() % this->GetBagCount();
			dur = 0;
			
			if ( cX == 0 && cY == 0 )
			{
				x = lpObj->X;
				y = lpObj->Y;
			}
			else
			{
				x = cX;
				y = cY;
			}

			level = this->GetLevel(DropItemNum);
			type = ItemGetNumberMake(this->BagObject[DropItemNum].m_type, this->BagObject[DropItemNum].m_index);

			if ( type == -1 )
			{
				continue;
			}

			if ( this->BagObject[DropItemNum].m_isskill != 0 )
			{
				Option1 =1;
			}

			if ( this->BagObject[DropItemNum].m_isluck != 0 )
			{
				Option2 = 0;

				if ( (rand()%2) == 0 )
				{
					Option2 = 1;
				}
			}

			if ( this->BagObject[DropItemNum].m_isoption > 0 )
			{
				int z28Percent = this->BagObject[DropItemNum].m_isoption*100;
				Option3 = rand()%(z28Percent+1);
				Option3 = Option3 / 100;
			}

			DropItemRate = (rand() % 1000)+1;
			if (DropItemRate <= this->GetExItemDropRate())
			{
				if ( this->BagObject[DropItemNum].m_isexitem != 0 )
				{
					ExOption = BoxExcOptions(this->BagObject[DropItemNum].m_isexitem);
				}
			}

			if ( type == ITEMGET(12,15) || type == ITEMGET(14,13) || type == ITEMGET(14,14))	// Chaos, Bless, Soul
			{
				Option1 = 0;
				Option2 = 0;
				Option3 = 0;
				level = 0;
			}

			if (type == ITEMGET(13,0) ||
				type == ITEMGET(13,1) ||
				type == ITEMGET(13,2) ||
				type == ITEMGET(13,8) ||
				type == ITEMGET(13,9) ||
				type == ITEMGET(13,12) ||
				 type ==ITEMGET(13,13) )	// Angel, imp, unirioa, dino, r and pendant of ice, poisonm
			{
				level = 0;
			}

			ItemSerialCreateSend(lpObj->m_Index, btMapNumber, x, y, type, level, dur, Option1, Option2, Option3, lpObj->m_Index, ExOption, 0);

			ITEMBAG_DROP_LOG.Output("[RAKLION EVENT] SELUPAN Event ItemDrop [%s][%s] [%s] : (%d)(%d/%d) Item:(%s)%d Level:%d op1:%d op2:%d op3:%d",
				lpObj->AccountID, lpObj->Name, this->m_sEventName, btMapNumber, x, y, ItemAttribute[type].Name, type, level, Option1, Option2, Option3);
		}
	}

	return TRUE;
}
BOOL CItemBagEx::DoubleGoerEventItem(int aIndex, BYTE btMapNumber, BYTE cX, BYTE cY)
{
	if ( this->m_bLoad == FALSE )
	{
		return FALSE;
	}

	float dur;
	int type;
	int level;
	int x;
	int y;
	int Option1 = 0;
	int Option2 = 0;
	int Option3 = 0;
	int DropItemRate;
	int DropItemNum;
	int ExOption = 0;
	LPOBJ lpObj = &gObj[aIndex];
	int iItemBagCount = this->GetBagCount();

	DropItemRate = rand() % 100;
	if ( DropItemRate > (this->GetItemDropRate() /** g_DoubleGoer.Difficulty*/) )
	{
		if (this->GetDropZen() <= 0)
		{
			ITEMBAG_DROP_LOG.Output("[Double Goer] No Zen Drop!");

			return FALSE;
		}

		MapC[lpObj->MapNumber].MoneyItemDrop(this->GetDropZen(), 
			lpObj->X, lpObj->Y);

		ITEMBAG_DROP_LOG.Output("[Double Goer] Event (%s) ItemDrop [%s][%s] [%d Zen] [RATE:%d/%d]",
			this->m_sEventName,
			lpObj->AccountID, lpObj->Name, 
			this->GetDropZen(), DropItemRate, this->GetItemDropRate());

		return TRUE;
	}

	if ( this->GetBagCount() > 0 )
	{
		DropItemNum =  rand() % this->GetBagCount();
		dur = 0;
			
		if ( cX == 0 && cY == 0 )
		{
			x = lpObj->X;
			y = lpObj->Y;
		}
		else
		{
			x = cX;
			y = cY;
		}

		level = this->GetLevel(DropItemNum);
		type = ItemGetNumberMake(this->BagObject[DropItemNum].m_type, this->BagObject[DropItemNum].m_index);

		if ( type == -1 )
		{
			return FALSE;
		}

		if ( this->BagObject[DropItemNum].m_isskill != 0 )
		{
			Option1 =1;
		}

		if ( this->BagObject[DropItemNum].m_isluck != 0 )
		{
			Option2 = 0;

			if ( (rand()%2) == 0 )
			{
				Option2 = 1;
			}
		}

		if ( this->BagObject[DropItemNum].m_isoption > 0 )
		{
			int z28Percent = this->BagObject[DropItemNum].m_isoption*100;
			Option3 = rand()%(z28Percent+1);
			Option3 = Option3 / 100;
		}

		DropItemRate = (rand() % 1000)+1;
		if (DropItemRate <= (this->GetExItemDropRate() /** g_DoubleGoer.Difficulty*/))
		{
			if ( this->BagObject[DropItemNum].m_isexitem != 0 )
			{
				ExOption = BoxExcOptions(this->BagObject[DropItemNum].m_isexitem);
			}
		}

		if ( type == ITEMGET(12,15) || type == ITEMGET(14,13) || type == ITEMGET(14,14))	// Chaos, Bless, Soul
		{
			Option1 = 0;
			Option2 = 0;
			Option3 = 0;
			level = 0;
		}

		if ( type == ITEMGET(13,0) ||
			 type == ITEMGET(13,1) ||
			 type == ITEMGET(13,2) ||
			 type == ITEMGET(13,8) ||
			 type == ITEMGET(13,9) ||
			 type == ITEMGET(13,12) ||
			 type ==ITEMGET(13,13) )	// Angel, imp, unirioa, dino, r and pendant of ice, poisonm
		{
			level = 0;
		}

		ItemSerialCreateSend(lpObj->m_Index, btMapNumber, x, y, type, level, dur, Option1, Option2, Option3, lpObj->m_Index, ExOption, 0);

		ITEMBAG_DROP_LOG.Output("[Double Goer] Event ItemDrop [%s][%s] [%s] : (%d)(%d/%d) Item:(%s)%d Level:%d op1:%d op2:%d op3:%d",
			lpObj->AccountID, lpObj->Name, this->m_sEventName, btMapNumber, x, y, ItemAttribute[type].Name, type, level, Option1, Option2, Option3);
	}

	return TRUE;
}

BOOL CItemBagEx::ImperialGuardianEventItem(int aIndex, BYTE btMapNumber, BYTE cX, BYTE cY)
{
	if ( this->m_bLoad == FALSE )
	{
		return FALSE;
	}

	float dur;
	int type;
	int level;
	int x;
	int y;
	int Option1 = 0;
	int Option2 = 0;
	int Option3 = 0;
	int DropItemRate;
	int DropItemNum;
	int ExOption = 0;
	LPOBJ lpObj = &gObj[aIndex];
	int iItemBagCount = this->GetBagCount();

	DropItemRate = rand() % 100;
	if ( DropItemRate > (this->GetItemDropRate() /** g_ImperialGuardian.Difficulty*/) )
	{
		if (this->GetDropZen() <= 0)
			return FALSE;

		MapC[lpObj->MapNumber].MoneyItemDrop(this->GetDropZen(), 
			lpObj->X, lpObj->Y);

		ITEMBAG_DROP_LOG.Output("[Imperial Guardian] Boss Event ItemDrop [%s][%s] [%d Zen] [RATE:%d/%d]",
			lpObj->AccountID, lpObj->Name, this->GetDropZen(), DropItemRate, this->GetItemDropRate());

		return TRUE;
	}

	if ( this->GetBagCount() > 0 )
	{
		DropItemNum =  rand() % this->GetBagCount();
		dur = 0;
			
		if ( cX == 0 && cY == 0 )
		{
			x = lpObj->X;
			y = lpObj->Y;
		}
		else
		{
			x = cX;
			y = cY;
		}

		level = this->GetLevel(DropItemNum);
		type = ItemGetNumberMake(this->BagObject[DropItemNum].m_type, this->BagObject[DropItemNum].m_index);

		if ( type == -1 )
		{
			return FALSE;
		}

		if ( this->BagObject[DropItemNum].m_isskill != 0 )
		{
			Option1 =1;
		}

		if ( this->BagObject[DropItemNum].m_isluck != 0 )
		{
			Option2 = 0;

			if ( (rand()%2) == 0 )
			{
				Option2 = 1;
			}
		}

		if ( this->BagObject[DropItemNum].m_isoption > 0 )
		{
			int z28Percent = this->BagObject[DropItemNum].m_isoption*100;
			Option3 = rand()%(z28Percent+1);
			Option3 = Option3 / 100;
		}

		DropItemRate = (rand() % 1000)+1;
		if (DropItemRate <= (this->GetExItemDropRate() /** g_ImperialGuardian.Difficulty*/))
		{
			if ( this->BagObject[DropItemNum].m_isexitem != 0 )
			{
				ExOption = BoxExcOptions(this->BagObject[DropItemNum].m_isexitem);
			}
		}

		if ( type == ITEMGET(12,15) || type == ITEMGET(14,13) || type == ITEMGET(14,14))	// Chaos, Bless, Soul
		{
			Option1 = 0;
			Option2 = 0;
			Option3 = 0;
			level = 0;
		}

		if ( type == ITEMGET(13,0) ||
			 type == ITEMGET(13,1) ||
			 type == ITEMGET(13,2) ||
			 type == ITEMGET(13,8) ||
			 type == ITEMGET(13,9) ||
			 type == ITEMGET(13,12) ||
			 type ==ITEMGET(13,13) )	// Angel, imp, unirioa, dino, r and pendant of ice, poisonm
		{
			level = 0;
		}

		ItemSerialCreateSend(lpObj->m_Index, btMapNumber, x, y, type, level, dur, Option1, Option2, Option3, lpObj->m_Index, ExOption, 0);

		ITEMBAG_DROP_LOG.Output("[Imperial Guardian] Boss Event ItemDrop [%s][%s] [%s] : (%d)(%d/%d) Item:(%s)%d Level:%d op1:%d op2:%d op3:%d",
			lpObj->AccountID, lpObj->Name, this->m_sEventName, btMapNumber, x, y, ItemAttribute[type].Name, type, level, Option1, Option2, Option3);
	}

	return TRUE;
}

BOOL CItemBagEx::DropFortunePouchItem(int aIndex, BYTE btMapNumber, BYTE cX, BYTE cY)
{
	if ( this->m_bLoad == FALSE )
	{
		return FALSE;
	}

	float dur;
	int type;
	int level;
	int x;
	int y;
	int Option1 = 0;
	int Option2 = 0;
	int Option3 = 0;
	int DropItemRate;
	int DropItemNum;
	int ExOption = 0;
	LPOBJ lpObj = &gObj[aIndex];
	int iItemBagCount = this->GetBagCount();

	DropItemRate = rand() % 100;
	if ( DropItemRate > this->GetItemDropRate() )
	{
		if (this->GetDropZen() <= 0)
		{
			ITEMBAG_DROP_LOG.Output("[FORTUNE POUCH] NO ItemDrop [%s][%s] [RATE:%d/%d]",
				lpObj->AccountID, lpObj->Name, DropItemRate, this->GetItemDropRate());
			return FALSE;
		}

		MapC[lpObj->MapNumber].MoneyItemDrop(this->GetDropZen(), 
			lpObj->X, lpObj->Y);

		ITEMBAG_DROP_LOG.Output("[FORTUNE POUCH] Event ItemDrop [%s][%s] [%d Zen] [RATE:%d/%d]",
			lpObj->AccountID, lpObj->Name, this->GetDropZen(), DropItemRate, this->GetItemDropRate());

		return TRUE;
	}

	if ( this->GetBagCount() > 0 )
	{
		DropItemNum =  rand() % this->GetBagCount();
		dur = 0;
			
		if ( cX == 0 && cY == 0 )
		{
			x = lpObj->X;
			y = lpObj->Y;
		}
		else
		{
			x = cX;
			y = cY;
		}

		level = this->GetLevel(DropItemNum);
		type = ItemGetNumberMake(this->BagObject[DropItemNum].m_type, this->BagObject[DropItemNum].m_index);

		if ( type == -1 )
		{
			return FALSE;
		}

		if ( this->BagObject[DropItemNum].m_isskill != 0 )
		{
			Option1 =1;
		}

		if ( this->BagObject[DropItemNum].m_isluck != 0 )
		{
			Option2 = 0;

			if ( (rand()%2) == 0 )
			{
				Option2 = 1;
			}
		}

		if ( this->BagObject[DropItemNum].m_isoption > 0 )
		{
			int z28Percent = this->BagObject[DropItemNum].m_isoption*100;
			Option3 = rand()%(z28Percent+1);
			Option3 = Option3 / 100;
		}

		DropItemRate = (rand() % 1000)+1;
		if (DropItemRate <= this->GetExItemDropRate())
		{
			if ( this->BagObject[DropItemNum].m_isexitem != 0 )
			{
				ExOption = BoxExcOptions(this->BagObject[DropItemNum].m_isexitem);
			}
		}

		if ( type == ITEMGET(12,15) || type == ITEMGET(14,13) || type == ITEMGET(14,14))	// Chaos, Bless, Soul
		{
			Option1 = 0;
			Option2 = 0;
			Option3 = 0;
			level = 0;
		}

		if ( type == ITEMGET(13,0) ||
			 type == ITEMGET(13,1) ||
			 type == ITEMGET(13,2) ||
			 type == ITEMGET(13,8) ||
			 type == ITEMGET(13,9) ||
			 type == ITEMGET(13,12) ||
			 type ==ITEMGET(13,13) )	// Angel, imp, unirioa, dino, r and pendant of ice, poisonm
		{
			level = 0;
		}

		ItemSerialCreateSend(lpObj->m_Index, btMapNumber, x, y, type, level, dur, Option1, Option2, Option3, lpObj->m_Index, ExOption, 0);

		ITEMBAG_DROP_LOG.Output("[FORTUNE POUCH] Event ItemDrop [%s][%s] [%s] : (%d)(%d/%d) Item:(%s)%d Level:%d op1:%d op2:%d op3:%d",
			lpObj->AccountID, lpObj->Name, this->m_sEventName, btMapNumber, x, y, ItemAttribute[type].Name, type, level, Option1, Option2, Option3);
	}

	return TRUE;
}

#if (PACK_EDITION>=2)
BOOL CItemBagEx::DropHalloweenPKEventItem(int aIndex, BYTE btMapNumber, BYTE cX, BYTE cY)
{
	if ( this->m_bLoad == FALSE )
	{
		return FALSE;
	}

	float dur;
	int type;
	int level;
	int x;
	int y;
	int Option1 = 0;
	int Option2 = 0;
	int Option3 = 0;
	int DropItemRate;
	int DropItemNum;
	int ExOption = 0;
	LPOBJ lpObj = &gObj[aIndex];
	int iItemBagCount = this->GetBagCount();

	DropItemRate = rand() % 100;
	if ( DropItemRate > this->GetItemDropRate() )
	{
		if (this->GetDropZen() <= 0)
		{
			ITEMBAG_DROP_LOG.Output("[HALLOWEEN EVENT] Halloween PK NO ItemDrop [%s][%s] [RATE:%d/%d]",
				lpObj->AccountID, lpObj->Name, DropItemRate, this->GetItemDropRate());
			return FALSE;
		}

		MapC[lpObj->MapNumber].MoneyItemDrop(this->GetDropZen(), 
			lpObj->X, lpObj->Y);

		ITEMBAG_DROP_LOG.Output("[HALLOWEEN EVENT] Halloween PK Event ItemDrop [%s][%s] [%d Zen] [RATE:%d/%d]",
			lpObj->AccountID, lpObj->Name, this->GetDropZen(), DropItemRate, this->GetItemDropRate());

		return TRUE;
	}

	if ( this->GetBagCount() > 0 )
	{
		DropItemNum =  rand() % this->GetBagCount();
		dur = 0;
			
		if ( cX == 0 && cY == 0 )
		{
			x = lpObj->X;
			y = lpObj->Y;
		}
		else
		{
			x = cX;
			y = cY;
		}

		level = this->GetLevel(DropItemNum);
		type = ItemGetNumberMake(this->BagObject[DropItemNum].m_type, this->BagObject[DropItemNum].m_index);

		if ( type == -1 )
		{
			return FALSE;
		}

		if ( this->BagObject[DropItemNum].m_isskill != 0 )
		{
			Option1 =1;
		}

		if ( this->BagObject[DropItemNum].m_isluck != 0 )
		{
			Option2 = 0;

			if ( (rand()%2) == 0 )
			{
				Option2 = 1;
			}
		}

		if ( this->BagObject[DropItemNum].m_isoption > 0 )
		{
			int z28Percent = this->BagObject[DropItemNum].m_isoption*100;
			Option3 = rand()%(z28Percent+1);
			Option3 = Option3 / 100;
		}

		DropItemRate = (rand() % 1000)+1;
		if (DropItemRate <= this->GetExItemDropRate())
		{
			if ( this->BagObject[DropItemNum].m_isexitem != 0 )
			{
				ExOption = BoxExcOptions(this->BagObject[DropItemNum].m_isexitem);
			}
		}

		if ( type == ITEMGET(12,15) || type == ITEMGET(14,13) || type == ITEMGET(14,14))	// Chaos, Bless, Soul
		{
			Option1 = 0;
			Option2 = 0;
			Option3 = 0;
			level = 0;
		}

		if ( type == ITEMGET(13,0) ||
			 type == ITEMGET(13,1) ||
			 type == ITEMGET(13,2) ||
			 type == ITEMGET(13,8) ||
			 type == ITEMGET(13,9) ||
			 type == ITEMGET(13,12) ||
			 type ==ITEMGET(13,13) )	// Angel, imp, unirioa, dino, r and pendant of ice, poisonm
		{
			level = 0;
		}

		ItemSerialCreateSend(lpObj->m_Index, btMapNumber, x, y, type, level, dur, Option1, Option2, Option3, lpObj->m_Index, ExOption, 0);

		ITEMBAG_DROP_LOG.Output("[HALLOWEEN EVENT] Halloween PK Event ItemDrop [%s][%s] [%s] : (%d)(%d/%d) Item:(%s)%d Level:%d op1:%d op2:%d op3:%d",
			lpObj->AccountID, lpObj->Name, this->m_sEventName, btMapNumber, x, y, ItemAttribute[type].Name, type, level, Option1, Option2, Option3);
	}

	return TRUE;
}
#endif

#if (PACK_EDITION>=1)
BOOL CItemBagEx::DropBlueEventItem(int aIndex, BYTE btMapNumber, BYTE cX, BYTE cY)
{
	if ( this->m_bLoad == FALSE )
	{
		return FALSE;
	}

	float dur;
	int type;
	int level;
	int x;
	int y;
	int Option1 = 0;
	int Option2 = 0;
	int Option3 = 0;
	int DropItemRate;
	int DropItemNum;
	int ExOption = 0;
	LPOBJ lpObj = &gObj[aIndex];
	int iItemBagCount = this->GetBagCount();

	DropItemRate = rand() % 100;
	if ( DropItemRate > this->GetItemDropRate() )
	{
		if (this->GetDropZen() <= 0)
		{
			ITEMBAG_DROP_LOG.Output("[BLUE EVENT] Rabbit NO ItemDrop [%s][%s] [RATE:%d/%d]",
				lpObj->AccountID, lpObj->Name, DropItemRate, this->GetItemDropRate());
			return FALSE;
		}

		MapC[lpObj->MapNumber].MoneyItemDrop(this->GetDropZen(), 
			lpObj->X, lpObj->Y);

		ITEMBAG_DROP_LOG.Output("[BLUE EVENT] Rabbit Event ItemDrop [%s][%s] [%d Zen] [RATE:%d/%d]",
			lpObj->AccountID, lpObj->Name, this->GetDropZen(), DropItemRate, this->GetItemDropRate());

		return TRUE;
	}

	if ( this->GetBagCount() > 0 )
	{
		DropItemNum =  rand() % this->GetBagCount();
		dur = 0;
			
		if ( cX == 0 && cY == 0 )
		{
			x = lpObj->X;
			y = lpObj->Y;
		}
		else
		{
			x = cX;
			y = cY;
		}

		level = this->GetLevel(DropItemNum);
		type = ItemGetNumberMake(this->BagObject[DropItemNum].m_type, this->BagObject[DropItemNum].m_index);

		if ( type == -1 )
		{
			return FALSE;
		}

		if ( this->BagObject[DropItemNum].m_isskill != 0 )
		{
			Option1 =1;
		}

		if ( this->BagObject[DropItemNum].m_isluck != 0 )
		{
			Option2 = 0;

			if ( (rand()%2) == 0 )
			{
				Option2 = 1;
			}
		}

		if ( this->BagObject[DropItemNum].m_isoption > 0 )
		{
			int z28Percent = this->BagObject[DropItemNum].m_isoption*100;
			Option3 = rand()%(z28Percent+1);
			Option3 = Option3 / 100;
		}

		DropItemRate = (rand() % 1000)+1;
		if (DropItemRate <= this->GetExItemDropRate())
		{
			if ( this->BagObject[DropItemNum].m_isexitem != 0 )
			{
				ExOption = BoxExcOptions(this->BagObject[DropItemNum].m_isexitem);
			}
		}

		if ( type == ITEMGET(12,15) || type == ITEMGET(14,13) || type == ITEMGET(14,14))	// Chaos, Bless, Soul
		{
			Option1 = 0;
			Option2 = 0;
			Option3 = 0;
			level = 0;
		}

		if ( type == ITEMGET(13,0) ||
			 type == ITEMGET(13,1) ||
			 type == ITEMGET(13,2) ||
			 type == ITEMGET(13,8) ||
			 type == ITEMGET(13,9) ||
			 type == ITEMGET(13,12) ||
			 type ==ITEMGET(13,13) )	// Angel, imp, unirioa, dino, r and pendant of ice, poisonm
		{
			level = 0;
		}

		ItemSerialCreateSend(lpObj->m_Index, btMapNumber, x, y, type, level, dur, Option1, Option2, Option3, lpObj->m_Index, ExOption, 0);

		ITEMBAG_DROP_LOG.Output("[BLUE EVENT] Rabbit Event ItemDrop [%s][%s] [%s] : (%d)(%d/%d) Item:(%s)%d Level:%d op1:%d op2:%d op3:%d",
			lpObj->AccountID, lpObj->Name, this->m_sEventName, btMapNumber, x, y, ItemAttribute[type].Name, type, level, Option1, Option2, Option3);
	}

	return TRUE;
}
BOOL CItemBagEx::DropSummerEventItem(int aIndex, BYTE btMapNumber, BYTE cX, BYTE cY)
{
	if ( this->m_bLoad == FALSE )
	{
		return FALSE;
	}

	float dur;
	int type;
	int level;
	int x;
	int y;
	int Option1 = 0;
	int Option2 = 0;
	int Option3 = 0;
	int DropItemRate;
	int DropItemNum;
	int ExOption = 0;
	LPOBJ lpObj = &gObj[aIndex];
	int iItemBagCount = this->GetBagCount();

	DropItemRate = rand() % 100;
	if ( DropItemRate > this->GetItemDropRate() )
	{
		if (this->GetDropZen() <= 0)
		{
			ITEMBAG_DROP_LOG.Output("[SUMMER EVENT] Fire Flame Ghost NO ItemDrop [%s][%s] [RATE:%d/%d]",
				lpObj->AccountID, lpObj->Name, DropItemRate, this->GetItemDropRate());
			return FALSE;
		}

		MapC[lpObj->MapNumber].MoneyItemDrop(this->GetDropZen(), 
			lpObj->X, lpObj->Y);

		ITEMBAG_DROP_LOG.Output("[SUMMER EVENT] Fire Flame Ghost Event ItemDrop [%s][%s] [%d Zen] [RATE:%d/%d]",
			lpObj->AccountID, lpObj->Name, this->GetDropZen(), DropItemRate, this->GetItemDropRate());

		return TRUE;
	}

	if ( this->GetBagCount() > 0 )
	{
		DropItemNum =  rand() % this->GetBagCount();
		dur = 0;
			
		if ( cX == 0 && cY == 0 )
		{
			x = lpObj->X;
			y = lpObj->Y;
		}
		else
		{
			x = cX;
			y = cY;
		}

		level = this->GetLevel(DropItemNum);
		type = ItemGetNumberMake(this->BagObject[DropItemNum].m_type, this->BagObject[DropItemNum].m_index);

		if ( type == -1 )
		{
			return FALSE;
		}

		if ( this->BagObject[DropItemNum].m_isskill != 0 )
		{
			Option1 =1;
		}

		if ( this->BagObject[DropItemNum].m_isluck != 0 )
		{
			Option2 = 0;

			if ( (rand()%2) == 0 )
			{
				Option2 = 1;
			}
		}

		if ( this->BagObject[DropItemNum].m_isoption > 0 )
		{
			int z28Percent = this->BagObject[DropItemNum].m_isoption*100;
			Option3 = rand()%(z28Percent+1);
			Option3 = Option3 / 100;
		}

		DropItemRate = (rand() % 1000)+1;
		if (DropItemRate <= this->GetExItemDropRate())
		{
			if ( this->BagObject[DropItemNum].m_isexitem != 0 )
			{
				ExOption = BoxExcOptions(this->BagObject[DropItemNum].m_isexitem);
			}
		}

		if ( type == ITEMGET(12,15) || type == ITEMGET(14,13) || type == ITEMGET(14,14))	// Chaos, Bless, Soul
		{
			Option1 = 0;
			Option2 = 0;
			Option3 = 0;
			level = 0;
		}

		if ( type == ITEMGET(13,0) ||
			 type == ITEMGET(13,1) ||
			 type == ITEMGET(13,2) ||
			 type == ITEMGET(13,8) ||
			 type == ITEMGET(13,9) ||
			 type == ITEMGET(13,12) ||
			 type ==ITEMGET(13,13) )	// Angel, imp, unirioa, dino, r and pendant of ice, poisonm
		{
			level = 0;
		}

		ItemSerialCreateSend(lpObj->m_Index, btMapNumber, x, y, type, level, dur, Option1, Option2, Option3, lpObj->m_Index, ExOption, 0);

		ITEMBAG_DROP_LOG.Output("[SUMMER EVENT] Fire Flame Ghost Event ItemDrop [%s][%s] [%s] : (%d)(%d/%d) Item:(%s)%d Level:%d op1:%d op2:%d op3:%d",
			lpObj->AccountID, lpObj->Name, this->m_sEventName, btMapNumber, x, y, ItemAttribute[type].Name, type, level, Option1, Option2, Option3);
	}

	return TRUE;
}
#endif

#if (PACK_EDITION>=3)
BOOL CItemBagEx::DropSwampEventItem(int aIndex, BYTE btMapNumber, BYTE cX, BYTE cY)
{
	if ( this->m_bLoad == FALSE )
	{
		return FALSE;
	}

	float dur;
	int type;
	int level;
	int x;
	int y;
	int Option1 = 0;
	int Option2 = 0;
	int Option3 = 0;
	int DropItemRate;
	int DropItemNum;
	int ExOption = 0;
	LPOBJ lpObj = &gObj[aIndex];
	int iItemBagCount = this->GetBagCount();

	int itemDrops = rand()%g_Swamp.MedusaDropItemCount;
	itemDrops++;

#if (GS_CASTLE==1)
	if (g_Swamp.Start)
	{
		char sbuff[1024]={0};
		wsprintf(sbuff,lMsg.Get(MSGGET(14, 129)), gObj[aIndex].Name);
		g_Swamp.GCServerMsgStringSend(sbuff,0);
	}
#endif

	for(int i=0;i<itemDrops;i++)
	{
		DropItemRate = rand() % 100;
		if ( DropItemRate > this->GetItemDropRate() )
		{
			if (this->GetDropZen() <= 0)
			{
				ITEMBAG_DROP_LOG.Output("[SWAMP EVENT] Medusa NO ItemDrop [%s][%s] [RATE:%d/%d]",
					lpObj->AccountID, lpObj->Name, DropItemRate, this->GetItemDropRate());
				continue;
			}

			MapC[lpObj->MapNumber].MoneyItemDrop(this->GetDropZen(), 
				lpObj->X, lpObj->Y);

			ITEMBAG_DROP_LOG.Output("[SWAMP EVENT] Medusa Event ItemDrop [%s][%s] [%d Zen] [RATE:%d/%d]",
				lpObj->AccountID, lpObj->Name, this->GetDropZen(), DropItemRate, this->GetItemDropRate());

			continue;
		}

		if ( this->GetBagCount() > 0 )
		{
			DropItemNum =  rand() % this->GetBagCount();
			dur = 0;
				
			if ( cX == 0 && cY == 0 )
			{
				x = lpObj->X;
				y = lpObj->Y;
			}
			else
			{
				x = cX;
				y = cY;
			}

			level = this->GetLevel(DropItemNum);
			type = ItemGetNumberMake(this->BagObject[DropItemNum].m_type, this->BagObject[DropItemNum].m_index);

			if ( type == -1 )
			{
				continue;
			}

			if ( this->BagObject[DropItemNum].m_isskill != 0 )
			{
				Option1 =1;
			}

			if ( this->BagObject[DropItemNum].m_isluck != 0 )
			{
				Option2 = 0;

				if ( (rand()%2) == 0 )
				{
					Option2 = 1;
				}
			}

			if ( this->BagObject[DropItemNum].m_isoption > 0 )
			{
				int z28Percent = this->BagObject[DropItemNum].m_isoption*100;
				Option3 = rand()%(z28Percent+1);
				Option3 = Option3 / 100;
			}

			DropItemRate = (rand() % 1000)+1;
			if (DropItemRate <= this->GetExItemDropRate())
			{
				if ( this->BagObject[DropItemNum].m_isexitem != 0 )
				{
					ExOption = BoxExcOptions(this->BagObject[DropItemNum].m_isexitem);
				}
			}

			if ( type == ITEMGET(12,15) || type == ITEMGET(14,13) || type == ITEMGET(14,14))	// Chaos, Bless, Soul
			{
				Option1 = 0;
				Option2 = 0;
				Option3 = 0;
				level = 0;
			}

			if ( type == ITEMGET(13,0) ||
				 type == ITEMGET(13,1) ||
				 type == ITEMGET(13,2) ||
				 type == ITEMGET(13,8) ||
				 type == ITEMGET(13,9) ||
				 type == ITEMGET(13,12) ||
				 type ==ITEMGET(13,13) )	// Angel, imp, unirioa, dino, r and pendant of ice, poisonm
			{
				level = 0;
			}

			BYTE cDropX = x;
			BYTE cDropY = y;
			if ( !gObjGetRandomItemDropLocation(lpObj->MapNumber, cDropX, cDropY, 4, 4, 10))
			{
				cDropX = x;
				cDropY = y;
			}
			ItemSerialCreateSend(lpObj->m_Index, btMapNumber, cDropX, cDropY, type, level, dur, Option1, Option2, Option3, lpObj->m_Index, ExOption, 0);

			ITEMBAG_DROP_LOG.Output("[SWAMP EVENT] Medusa Event ItemDrop [%s][%s] [%s] : (%d)(%d/%d) Item:(%s)%d Level:%d op1:%d op2:%d op3:%d",
				lpObj->AccountID, lpObj->Name, this->m_sEventName, btMapNumber, x, y, ItemAttribute[type].Name, type, level, Option1, Option2, Option3);
		}
	}

	return TRUE;
}
#endif

BOOL CItemBagEx::DropXMasEventItem(int aIndex, BYTE btMapNumber, BYTE cX, BYTE cY)
{
	if ( this->m_bLoad == FALSE )
	{
		return FALSE;
	}

	float dur;
	int type;
	int level;
	int x;
	int y;
	int Option1 = 0;
	int Option2 = 0;
	int Option3 = 0;
	int DropItemRate;
	int DropItemNum;
	int ExOption = 0;
	LPOBJ lpObj = &gObj[aIndex];
	int iItemBagCount = this->GetBagCount();

	GCFireWorks(lpObj->MapNumber,lpObj->X,lpObj->Y);

	DropItemRate = rand() % 100;
	if ( DropItemRate > this->GetItemDropRate() )
	{
		if (this->GetDropZen() <= 0)
		{
			ITEMBAG_DROP_LOG.Output("[XMAS EVENT] Santa NO ItemDrop [%s][%s] [RATE:%d/%d]",
				lpObj->AccountID, lpObj->Name, DropItemRate, this->GetItemDropRate());
			return FALSE;
		}

		MapC[lpObj->MapNumber].MoneyItemDrop(this->GetDropZen(), 
			lpObj->X, lpObj->Y);

		ITEMBAG_DROP_LOG.Output("[XMAS EVENT] Santa Event ItemDrop [%s][%s] [%d Zen] [RATE:%d/%d]",
			lpObj->AccountID, lpObj->Name, this->GetDropZen(), DropItemRate, this->GetItemDropRate());

		return TRUE;
	}

	if ( this->GetBagCount() > 0 )
	{
		DropItemNum =  rand() % this->GetBagCount();
		dur = 0;
			
		if ( cX == 0 && cY == 0 )
		{
			x = lpObj->X;
			y = lpObj->Y;
		}
		else
		{
			x = cX;
			y = cY;
		}

		level = this->GetLevel(DropItemNum);
		type = ItemGetNumberMake(this->BagObject[DropItemNum].m_type, this->BagObject[DropItemNum].m_index);

		if ( type == -1 )
		{
			return FALSE;
		}

		if ( this->BagObject[DropItemNum].m_isskill != 0 )
		{
			Option1 =1;
		}

		if ( this->BagObject[DropItemNum].m_isluck != 0 )
		{
			Option2 = 0;

			if ( (rand()%2) == 0 )
			{
				Option2 = 1;
			}
		}

		if ( this->BagObject[DropItemNum].m_isoption > 0 )
		{
			int z28Percent = this->BagObject[DropItemNum].m_isoption*100;
			Option3 = rand()%(z28Percent+1);
			Option3 = Option3 / 100;
		}

		DropItemRate = (rand() % 1000)+1;
		if (DropItemRate <= this->GetExItemDropRate())
		{
			if ( this->BagObject[DropItemNum].m_isexitem != 0 )
			{
				ExOption = BoxExcOptions(this->BagObject[DropItemNum].m_isexitem);
			}
		}

		if ( type == ITEMGET(12,15) || type == ITEMGET(14,13) || type == ITEMGET(14,14))	// Chaos, Bless, Soul
		{
			Option1 = 0;
			Option2 = 0;
			Option3 = 0;
			level = 0;
		}

		if ( type == ITEMGET(13,0) ||
			 type == ITEMGET(13,1) ||
			 type == ITEMGET(13,2) ||
			 type == ITEMGET(13,8) ||
			 type == ITEMGET(13,9) ||
			 type == ITEMGET(13,12) ||
			 type ==ITEMGET(13,13) )	// Angel, imp, unirioa, dino, r and pendant of ice, poisonm
		{
			level = 0;
		}

		ItemSerialCreateSend(lpObj->m_Index, btMapNumber, x, y, type, level, dur, Option1, Option2, Option3, lpObj->m_Index, ExOption, 0);

		ITEMBAG_DROP_LOG.Output("[XMAS EVENT] Santa Event ItemDrop [%s][%s] [%s] : (%d)(%d/%d) Item:(%s)%d Level:%d op1:%d op2:%d op3:%d",
			lpObj->AccountID, lpObj->Name, this->m_sEventName, btMapNumber, x, y, ItemAttribute[type].Name, type, level, Option1, Option2, Option3);
	}

	return TRUE;
}

#if (PACK_EDITION>=3)
BOOL CItemBagEx::DropBossAttackItem(int aIndex, BYTE btMapNumber, BYTE cX, BYTE cY)
{
	if ( this->m_bLoad == FALSE )
	{
		return FALSE;
	}

	float dur;
	int type;
	int level;
	int x;
	int y;
	int Option1 = 0;
	int Option2 = 0;
	int Option3 = 0;
	int DropItemNum;
	int DropItemRate;
	int ExOption = 0;
	LPOBJ lpObj = &gObj[aIndex];
	int iItemBagCount = this->GetBagCount();

	DropItemRate = rand() % 100;
	if ( DropItemRate > this->GetItemDropRate() )
	{
		if (this->GetDropZen() <= 0)
		{
			ITEMBAG_DROP_LOG.Output("[BOSS ATTACK] Boss NO ItemDrop [%s][%s] [RATE:%d/%d]",
				lpObj->AccountID, lpObj->Name, DropItemRate, this->GetItemDropRate());
			return FALSE;
		}

		MapC[lpObj->MapNumber].MoneyItemDrop(this->GetDropZen(), 
			lpObj->X, lpObj->Y);

		ITEMBAG_DROP_LOG.Output("[BOSS ATTACK] Boss Event ItemDrop [%s][%s] [%d Zen] [RATE:%d/%d]",
			lpObj->AccountID, lpObj->Name, this->GetDropZen(), DropItemRate, this->GetItemDropRate());

		return TRUE;
	}

	if ( this->GetBagCount() > 0 )
	{
		DropItemNum =  rand() % this->GetBagCount();
		dur = 0;
			
		if ( cX == 0 && cY == 0 )
		{
			x = lpObj->X;
			y = lpObj->Y;
		}
		else
		{
			x = cX;
			y = cY;
		}

		level = this->GetLevel(DropItemNum);
		type = ItemGetNumberMake(this->BagObject[DropItemNum].m_type, this->BagObject[DropItemNum].m_index);

		if ( type == -1 )
		{
			return FALSE;
		}

		if ( this->BagObject[DropItemNum].m_isskill != 0 )
		{
			Option1 =1;
		}

		if ( this->BagObject[DropItemNum].m_isluck != 0 )
		{
			Option2 = 0;

			if ( (rand()%2) == 0 )
			{
				Option2 = 1;
			}
		}

		if ( this->BagObject[DropItemNum].m_isoption > 0 )
		{
			int z28Percent = this->BagObject[DropItemNum].m_isoption*100;
			Option3 = rand()%(z28Percent+1);
			Option3 = Option3 / 100;
		}

		DropItemRate = (rand() % 1000)+1;
		if (DropItemRate <= this->GetExItemDropRate())
		{
			if ( this->BagObject[DropItemNum].m_isexitem != 0 )
			{
				ExOption = BoxExcOptions(this->BagObject[DropItemNum].m_isexitem);
			}
		}

		if ( type == ITEMGET(12,15) || type == ITEMGET(14,13) || type == ITEMGET(14,14))	// Chaos, Bless, Soul
		{
			Option1 = 0;
			Option2 = 0;
			Option3 = 0;
			level = 0;
		}

		if ( type == ITEMGET(13,0) ||
			 type == ITEMGET(13,1) ||
			 type == ITEMGET(13,2) ||
			 type == ITEMGET(13,8) ||
			 type == ITEMGET(13,9) ||
			 type == ITEMGET(13,12) ||
			 type ==ITEMGET(13,13) )	// Angel, imp, unirioa, dino, r and pendant of ice, poisonm
		{
			level = 0;
		}

		ItemSerialCreateSend(lpObj->m_Index, btMapNumber, x, y, type, level, dur, Option1, Option2, Option3, lpObj->m_Index, ExOption, 0);

		ITEMBAG_DROP_LOG.Output("[BOSS ATTACK] Boss Event ItemDrop [%s][%s] [%s] : (%d)(%d/%d) Item:(%s)%d Level:%d op1:%d op2:%d op3:%d",
			lpObj->AccountID, lpObj->Name, this->m_sEventName, btMapNumber, x, y, ItemAttribute[type].Name, type, level, Option1, Option2, Option3);
	}

	return TRUE;
}
#endif


BOOL CItemBagEx::DropS6QuestBox(int aIndex)
{
	if ( this->m_bLoad == FALSE )
		return FALSE;

	float dur;
	int type;
	int level;
	int x;
	int y;
	int Option1 = 0;
	int Option2 = 0;
	int Option3 = 0;
	int DropItemRate;
	int DropItemNum;
	int ExOption = 0;
	LPOBJ lpObj = &gObj[aIndex];

	DropItemRate = rand() % 100;
	if ( DropItemRate > this->GetItemDropRate() )
	{
		if (this->GetDropZen() <= 0)
		{
			ITEMBAG_DROP_LOG.Output("[S6Quest Box] Box NO ItemDrop [%s][%s] [RATE:%d/%d]",
				lpObj->AccountID, lpObj->Name, DropItemRate, this->GetItemDropRate());
			return FALSE;
		}

		MapC[lpObj->MapNumber].MoneyItemDrop(this->GetDropZen(), 
			lpObj->X, lpObj->Y);

		ITEMBAG_DROP_LOG.Output("[S6Quest Box] Box ItemDrop [%s][%s] [%d Zen] [RATE:%d/%d]",
			lpObj->AccountID, lpObj->Name, this->GetDropZen(), DropItemRate, this->GetItemDropRate());

		return TRUE;
	}

	if ( this->GetBagCount() > 0 )
	{
		DropItemNum = rand() % this->GetBagCount();
		dur = 0;
		x = lpObj->X;
		y = lpObj->Y;
		level = this->GetLevel(DropItemNum);
		type = ItemGetNumberMake(this->BagObject[DropItemNum].m_type, this->BagObject[DropItemNum].m_index);

		if ( type == -1 )
			return FALSE;

		if ( this->BagObject[DropItemNum].m_isskill != 0 )
			Option1 = 1;

		if ( this->BagObject[DropItemNum].m_isluck != 0 )
		{
			Option2 = 0;

			if ( (rand()%2) == 0 )
			{
				Option2 = 1;
			}
		}

		if ( this->BagObject[DropItemNum].m_isoption > 0 )
		{
			int z28Percent = this->BagObject[DropItemNum].m_isoption*100;
			Option3 = rand()%(z28Percent+1);
			Option3 = Option3 / 100;
		}

		DropItemRate = (rand() % 1000)+1;
		if (DropItemRate <= this->GetExItemDropRate())
		{
			if ( this->BagObject[DropItemNum].m_isexitem != 0 )
			{
				ExOption = BoxExcOptions(this->BagObject[DropItemNum].m_isexitem);
			}
		}

		if ( type == ITEMGET(12,15) ||
			 type == ITEMGET(14,13) ||
			 type == ITEMGET(14,14)  )
		{
			Option1 = 0;
			Option2 = 0;
			Option3 = 0;
			level = 0;
		}

		if ( type == ITEMGET(13,8) ||
			 type == ITEMGET(13,9) ||
			 type == ITEMGET(13,12) ||
			 type == ITEMGET(13,21) )
		{
			level = 0;
		}
			
		ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, x, y, type, level, dur,
			Option1, Option2, Option3, lpObj->m_Index, ExOption, 0);

		ITEMBAG_DROP_LOG.Output("[S6Quest Box] Box ItemDrop [%s][%s] [%s] : (%d)(%d/%d) Item:(%s)%d Level:%d op1:%d op2:%d op3:%d",
			lpObj->AccountID, lpObj->Name, this->m_sEventName,
			lpObj->MapNumber, x, y, ItemAttribute[type].Name,
			type, level, Option1, Option2, Option3);

		PMSG_SERVERCMD ServerCmd;

		PHeadSubSetB((LPBYTE)&ServerCmd, 0xF3, 0x40, sizeof(ServerCmd));
		ServerCmd.CmdType = 0;
		ServerCmd.X = lpObj->X;
		ServerCmd.Y = lpObj->Y;

		MsgSendV2(lpObj, (LPBYTE)&ServerCmd, sizeof(ServerCmd));
		DataSend(lpObj->m_Index, (LPBYTE)&ServerCmd, sizeof(ServerCmd));
	}

	return TRUE;
}

BOOL CItemBagEx::DropS5E4BoxItem(int aIndex)
{
	if ( this->m_bLoad == FALSE )
		return FALSE;

	float dur;
	int type;
	int level;
	int x;
	int y;
	int Option1 = 0;
	int Option2 = 0;
	int Option3 = 0;
	int DropItemRate;
	int DropItemNum;
	int ExOption = 0;
	LPOBJ lpObj = &gObj[aIndex];

	DropItemRate = rand() % 100;
	if ( DropItemRate > this->GetItemDropRate() )
	{
		if (this->GetDropZen() <= 0)
		{
			ITEMBAG_DROP_LOG.Output("[S5E4 Box] Box NO ItemDrop [%s][%s] [RATE:%d/%d]",
				lpObj->AccountID, lpObj->Name, DropItemRate, this->GetItemDropRate());
			return FALSE;
		}

		MapC[lpObj->MapNumber].MoneyItemDrop(this->GetDropZen(), 
			lpObj->X, lpObj->Y);

		ITEMBAG_DROP_LOG.Output("[S5E4 Box] Box ItemDrop [%s][%s] [%d Zen] [RATE:%d/%d]",
			lpObj->AccountID, lpObj->Name, this->GetDropZen(), DropItemRate, this->GetItemDropRate());

		return TRUE;
	}

	if ( this->GetBagCount() > 0 )
	{
		DropItemNum = rand() % this->GetBagCount();
		dur = 0;
		x = lpObj->X;
		y = lpObj->Y;
		level = this->GetLevel(DropItemNum);
		type = ItemGetNumberMake(this->BagObject[DropItemNum].m_type, this->BagObject[DropItemNum].m_index);

		if ( type == -1 )
			return FALSE;

		if ( this->BagObject[DropItemNum].m_isskill != 0 )
			Option1 = 1;

		if ( this->BagObject[DropItemNum].m_isluck != 0 )
		{
			Option2 = 0;

			if ( (rand()%2) == 0 )
			{
				Option2 = 1;
			}
		}

		if ( this->BagObject[DropItemNum].m_isoption > 0 )
		{
			int z28Percent = this->BagObject[DropItemNum].m_isoption*100;
			Option3 = rand()%(z28Percent+1);
			Option3 = Option3 / 100;
		}

		DropItemRate = (rand() % 1000)+1;
		if (DropItemRate <= this->GetExItemDropRate())
		{
			if ( this->BagObject[DropItemNum].m_isexitem != 0 )
			{
				ExOption = BoxExcOptions(this->BagObject[DropItemNum].m_isexitem);
			}
		}

		if ( type == ITEMGET(12,15) ||
			 type == ITEMGET(14,13) ||
			 type == ITEMGET(14,14)  )
		{
			Option1 = 0;
			Option2 = 0;
			Option3 = 0;
			level = 0;
		}

		if ( type == ITEMGET(13,8) ||
			 type == ITEMGET(13,9) ||
			 type == ITEMGET(13,12) ||
			 type == ITEMGET(13,21) )
		{
			level = 0;
		}
			
		ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, x, y, type, level, dur,
			Option1, Option2, Option3, lpObj->m_Index, ExOption, 0);

		ITEMBAG_DROP_LOG.Output("[S5E4 Box] Box ItemDrop [%s][%s] [%s] : (%d)(%d/%d) Item:(%s)%d Level:%d op1:%d op2:%d op3:%d",
			lpObj->AccountID, lpObj->Name, this->m_sEventName,
			lpObj->MapNumber, x, y, ItemAttribute[type].Name,
			type, level, Option1, Option2, Option3);

		PMSG_SERVERCMD ServerCmd;

		PHeadSubSetB((LPBYTE)&ServerCmd, 0xF3, 0x40, sizeof(ServerCmd));
		ServerCmd.CmdType = 0;
		ServerCmd.X = lpObj->X;
		ServerCmd.Y = lpObj->Y;

		MsgSendV2(lpObj, (LPBYTE)&ServerCmd, sizeof(ServerCmd));
		DataSend(lpObj->m_Index, (LPBYTE)&ServerCmd, sizeof(ServerCmd));
	}

	return TRUE;
}


BOOL CItemBagEx::DropRainItemEvent(int map, int x, int y)
{
	if ( this->m_bLoad == FALSE )
		return FALSE;

	float dur;
	int type;
	int level;
	int Option1 = 0;
	int Option2 = 0;
	int Option3 = 0;
	int DropItemRate;
	int DropItemNum;
	int ExOption = 0;

	if ( this->GetBagCount() > 0 )
	{
		//srand((unsigned)time(NULL));
		srand(time(0));
		DropItemNum = rand() % this->GetBagCount();
		dur = 0;
		//srand((unsigned)time(NULL));
		level = this->GetLevel(DropItemNum);
		type = ItemGetNumberMake(this->BagObject[DropItemNum].m_type, this->BagObject[DropItemNum].m_index);

		if ( type == -1 )
			return FALSE;

		if ( this->BagObject[DropItemNum].m_isskill != 0 )
			Option1 = 1;

		if ( this->BagObject[DropItemNum].m_isluck != 0 )
		{
			Option2 = 0;

			if ( (rand()%2) == 0 )
			{
				Option2 = 1;
			}
		}

		if ( this->BagObject[DropItemNum].m_isoption != 0 )
		{
			if ( rand()%5 < 1 )
			{
				Option3 = this->BagObject[DropItemNum].m_isoption;
			}
			else
			{
				Option3 = rand()%this->BagObject[DropItemNum].m_isoption + 1;
			}
		}

		//srand((unsigned)time(NULL));
		DropItemRate = (rand() % 1000)+1;
		if (DropItemRate <= this->GetExItemDropRate())
		{
			if ( this->BagObject[DropItemNum].m_isexitem != 0 )
			{
				ExOption = BoxExcOptions(this->BagObject[DropItemNum].m_isexitem);
			}
		}

		if ( type == ITEMGET(12,15) ||
			 type == ITEMGET(14,13) ||
			 type == ITEMGET(14,14)  )
		{
			Option1 = 0;
			Option2 = 0;
			Option3 = 0;
			level = 0;
		}

		if ( type == ITEMGET(13,8) ||
			 type == ITEMGET(13,9) ||
			 type == ITEMGET(13,12) ||
			 type == ITEMGET(13,21) )
		{
			level = 0;
		}
			
				//MapC[map].MonsterItemDrop(type, level, dur, x, y,
				//	Option1, Option2, Option3, ExOption, 0, -1, 0, 0,0,0,0,0,0);
		ItemSerialCreateSend(-1, map, x, y, type, level, dur,
			Option1, Option2, Option3, 0, ExOption, 0);

		ITEMBAG_DROP_LOG.Output("[Rain Item Event] Box ItemDrop : (%d)(%d/%d) Item:(%s)%d Level:%d op1:%d op2:%d op3:%d",
			map, x, y, ItemAttribute[type].Name,
			type, level, Option1, Option2, Option3);

		PMSG_SERVERCMD ServerCmd;

		PHeadSubSetB((LPBYTE)&ServerCmd, 0xF3, 0x40, sizeof(ServerCmd));
		ServerCmd.CmdType = 0;
		ServerCmd.X = x;
		ServerCmd.Y = y;

		MsgSendDistNear2((LPBYTE)&ServerCmd, sizeof(ServerCmd),map,x,y,15);
	}

	return TRUE;
}

BOOL CItemBagEx::DropKundunEventItem(int aIndex, BYTE btMapNumber, BYTE cX, BYTE cY)
{
	if ( this->m_bLoad == FALSE )
	{
		return FALSE;
	}

	float dur;
	int type;
	int level;
	int x;
	int y;
	int Option1 = 0;
	int Option2 = 0;
	int Option3 = 0;
	int DropItemNum;
	int DropItemRate;
	int ExOption = 0;
	LPOBJ lpObj = &gObj[aIndex];
	int iItemBagCount = this->GetBagCount();

	DropItemRate = rand() % 100;
	if ( DropItemRate > this->GetItemDropRate() )
	{
		if (this->GetDropZen() <= 0)
		{
			ITEMBAG_DROP_LOG.Output("[Kundun EVENT] KUNDUN NO ItemDrop [%s][%s] [RATE:%d/%d]",
				lpObj->AccountID, lpObj->Name, DropItemRate, this->GetItemDropRate());
			return FALSE;
		}

		MapC[lpObj->MapNumber].MoneyItemDrop(this->GetDropZen(), 
			lpObj->X, lpObj->Y);

		ITEMBAG_DROP_LOG.Output("[Kundun EVENT] KUNDUN Event ItemDrop [%s][%s] [%d Zen] [RATE:%d/%d]",
			lpObj->AccountID, lpObj->Name, this->GetDropZen(), DropItemRate, this->GetItemDropRate());

		return TRUE;
	}

	if ( this->GetBagCount() > 0 )
	{
		DropItemNum =  rand() % this->GetBagCount();
		dur = 0;
			
		if ( cX == 0 && cY == 0 )
		{
			x = lpObj->X;
			y = lpObj->Y;
		}
		else
		{
			x = cX;
			y = cY;
		}

		level = this->GetLevel(DropItemNum);
		type = ItemGetNumberMake(this->BagObject[DropItemNum].m_type, this->BagObject[DropItemNum].m_index);

		if ( type == -1 )
		{
			return FALSE;
		}

		if ( this->BagObject[DropItemNum].m_isskill != 0 )
		{
			Option1 =1;
		}

		if ( this->BagObject[DropItemNum].m_isluck != 0 )
		{
			Option2 = 0;

			if ( (rand()%2) == 0 )
			{
				Option2 = 1;
			}
		}

		if ( this->BagObject[DropItemNum].m_isoption > 0 )
		{
			int z28Percent = this->BagObject[DropItemNum].m_isoption*100;
			Option3 = rand()%(z28Percent+1);
			Option3 = Option3 / 100;
		}

		DropItemRate = (rand() % 1000)+1;
		if (DropItemRate <= this->GetExItemDropRate())
		{
			if ( this->BagObject[DropItemNum].m_isexitem != 0 )
			{
				ExOption = BoxExcOptions(this->BagObject[DropItemNum].m_isexitem);
			}
		}

		if ( type == ITEMGET(12,15) || type == ITEMGET(14,13) || type == ITEMGET(14,14))	// Chaos, Bless, Soul
		{
			Option1 = 0;
			Option2 = 0;
			Option3 = 0;
			level = 0;
		}

		if ( type == ITEMGET(13,0) ||
			 type == ITEMGET(13,1) ||
			 type == ITEMGET(13,2) ||
			 type == ITEMGET(13,8) ||
			 type == ITEMGET(13,9) ||
			 type == ITEMGET(13,12) ||
			 type ==ITEMGET(13,13) )	// Angel, imp, unirioa, dino, r and pendant of ice, poisonm
		{
			level = 0;
		}

		ItemSerialCreateSend(lpObj->m_Index, btMapNumber, x, y, type, level, dur, Option1, Option2, Option3, lpObj->m_Index, ExOption, 0);

		ITEMBAG_DROP_LOG.Output("[Kundun EVENT] KUNDUN Event ItemDrop [%s][%s] [%s] : (%d)(%d/%d) Item:(%s)%d Level:%d op1:%d op2:%d op3:%d",
			lpObj->AccountID, lpObj->Name, this->m_sEventName, btMapNumber, x, y, ItemAttribute[type].Name, type, level, Option1, Option2, Option3);
	}

	return TRUE;
}

BOOL CItemBagEx::DropErohimCastleZoneItem(int aIndex, BYTE btMapNumber, BYTE cX, BYTE cY)
{
	if ( this->m_bLoad == FALSE )
	{
		return FALSE;
	}

	float dur;
	int type;
	int level;
	int x;
	int y;
	int Option1 = 0;
	int Option2 = 0;
	int Option3 = 0;
	int DropItemNum;
	int DropItemRate;
	int ExOption = 0;
	LPOBJ lpObj = &gObj[aIndex];
	int iItemBagCount = this->GetBagCount();

	DropItemRate = rand() % 100;
	if ( DropItemRate > this->GetItemDropRate() )
	{
		if (this->GetDropZen() <= 0)
		{
			ITEMBAG_DROP_LOG.Output("[CastleDeepEvent]EROHIM NO ItemDrop [%s][%s] [RATE:%d/%d]",
				lpObj->AccountID, lpObj->Name, DropItemRate, this->GetItemDropRate());
			return FALSE;
		}

		MapC[lpObj->MapNumber].MoneyItemDrop(this->GetDropZen(), 
			lpObj->X, lpObj->Y);

		ITEMBAG_DROP_LOG.Output("[CastleDeepEvent] EROHIM Event ItemDrop [%s][%s] [%d Zen] [RATE:%d/%d]",
			lpObj->AccountID, lpObj->Name, this->GetDropZen(), DropItemRate, this->GetItemDropRate());

		return TRUE;
	}

	if ( this->GetBagCount() > 0 )
	{
		DropItemNum =  rand() % this->GetBagCount();
		dur = 0;
			
		if ( cX == 0 && cY == 0 )
		{
			x = lpObj->X;
			y = lpObj->Y;
		}
		else
		{
			x = cX;
			y = cY;
		}

		level = this->GetLevel(DropItemNum);
		type = ItemGetNumberMake(this->BagObject[DropItemNum].m_type, this->BagObject[DropItemNum].m_index);

		if ( type == -1 )
		{
			return FALSE;
		}

		if ( this->BagObject[DropItemNum].m_isskill != 0 )
		{
			Option1 =1;
		}

		if ( this->BagObject[DropItemNum].m_isluck != 0 )
		{
			Option2 = 0;

			if ( (rand()%2) == 0 )
			{
				Option2 = 1;
			}
		}

		if ( this->BagObject[DropItemNum].m_isoption > 0 )
		{
			int z28Percent = this->BagObject[DropItemNum].m_isoption*100;
			Option3 = rand()%(z28Percent+1);
			Option3 = Option3 / 100;
		}

		DropItemRate = (rand() % 1000)+1;
		if (DropItemRate <= this->GetExItemDropRate())
		{
			if ( this->BagObject[DropItemNum].m_isexitem != 0 )
			{
				ExOption = BoxExcOptions(this->BagObject[DropItemNum].m_isexitem);
			}
		}

		if ( type == ITEMGET(12,15) || type == ITEMGET(14,13) || type == ITEMGET(14,14))	// Chaos, Bless, Soul
		{
			Option1 = 0;
			Option2 = 0;
			Option3 = 0;
			level = 0;
		}

		ItemSerialCreateSend(lpObj->m_Index, btMapNumber, x, y, type, level, dur, Option1, Option2, Option3, lpObj->m_Index, ExOption, 0);

		ITEMBAG_DROP_LOG.Output("[CastleDeepEvent] EROHIM Event ItemDrop [%s][%s] [%s] : (%d)(%d/%d) Item:(%s)%d Level:%d op1:%d op2:%d op3:%d",
			lpObj->AccountID, lpObj->Name, this->m_sEventName, btMapNumber, x, y, ItemAttribute[type].Name, type, level, Option1, Option2, Option3);
	}

	return TRUE;
}

BOOL CItemBagEx::DropHuntZoneItem(int aIndex, BYTE btMapNumber, BYTE cX, BYTE cY)
{
	if ( this->m_bLoad == FALSE )
	{
		return FALSE;
	}

	float dur;
	int type;
	int level;
	int x;
	int y;
	int Option1 = 0;
	int Option2 = 0;
	int Option3 = 0;
	int DropItemNum;
	int DropItemRate;
	int ExOption = 0;
	LPOBJ lpObj = &gObj[aIndex];
	int iItemBagCount = this->GetBagCount();

	DropItemRate = rand() % 100;
	if ( DropItemRate > this->GetItemDropRate() )
	{
		if (this->GetDropZen() > 0)
		{
			int dropZen = rand() % this->GetDropZen();

			MapC[lpObj->MapNumber].MoneyItemDrop(dropZen, lpObj->X, lpObj->Y);

			ITEMBAG_DROP_LOG.Output("[CastleDeepEvent] HUNT ZONE Event ItemDrop [%s][%s] [%d Zen] [RATE:%d/%d]",
				lpObj->AccountID, lpObj->Name, dropZen, DropItemRate, this->GetItemDropRate());

			return TRUE;
		} else {
			return FALSE;
		}
	}

	if ( this->GetBagCount() > 0 )
	{
		DropItemNum =  rand() % this->GetBagCount();
		dur = 0;
			
		if ( cX == 0 && cY == 0 )
		{
			x = lpObj->X;
			y = lpObj->Y;
		}
		else
		{
			x = cX;
			y = cY;
		}

		level = this->GetLevel(DropItemNum);
		type = ItemGetNumberMake(this->BagObject[DropItemNum].m_type, this->BagObject[DropItemNum].m_index);

		if ( type == -1 )
		{
			return FALSE;
		}

		if ( this->BagObject[DropItemNum].m_isskill != 0 )
		{
			Option1 =1;
		}

		if ( this->BagObject[DropItemNum].m_isluck != 0 )
		{
			Option2 = 0;

			if ( (rand()%2) == 0 )
			{
				Option2 = 1;
			}
		}

		if ( this->BagObject[DropItemNum].m_isoption > 0 )
		{
			int z28Percent = this->BagObject[DropItemNum].m_isoption*100;
			Option3 = rand()%(z28Percent+1);
			Option3 = Option3 / 100;
		}

		DropItemRate = (rand() % 1000)+1;
		if (DropItemRate <= this->GetExItemDropRate())
		{
			if ( this->BagObject[DropItemNum].m_isexitem != 0 )
			{
				ExOption = BoxExcOptions(this->BagObject[DropItemNum].m_isexitem);
			}
		}

		if ( type == ITEMGET(12,15) || type == ITEMGET(14,13) || type == ITEMGET(14,14))	// Chaos, Bless, Soul
		{
			Option1 = 0;
			Option2 = 0;
			Option3 = 0;
			level = 0;
		}

		ItemSerialCreateSend(lpObj->m_Index, btMapNumber, x, y, type, level, dur, Option1, Option2, Option3, lpObj->m_Index, ExOption, 0);

		ITEMBAG_DROP_LOG.Output("[CastleDeepEvent] HUNT ZONE Event ItemDrop [%s][%s] [%s] : (%d)(%d/%d) Item:(%s)%d Level:%d op1:%d op2:%d op3:%d",
			lpObj->AccountID, lpObj->Name, this->m_sEventName, btMapNumber, x, y, ItemAttribute[type].Name, type, level, Option1, Option2, Option3);
	}

	return TRUE;
}

BOOL CItemBagEx::DropHalloweenEventItem(LPOBJ lpObj)
{
	if ( this->m_bLoad == FALSE )
		return FALSE;

	int iType;
	int iLevel;
	int X;
	int Y;
	int DropItemNum = 0;
	int DropItemRate;
	int iDuration = 0;
	int iTotalDropRate = 0;
	int iItemBagCount = this->GetBagCount();

	DropItemRate = rand() % 100;
	if ( DropItemRate > this->GetItemDropRate() )
	{
		if (this->GetDropZen() <= 0)
		{
			ITEMBAG_DROP_LOG.Output("[HalloweenDay Event][Pumpkin of Luck] NO ItemDrop [%s][%s] [RATE:%d/%d]",
				lpObj->AccountID, lpObj->Name, DropItemRate, this->GetItemDropRate());
			return FALSE;
		}

		MapC[lpObj->MapNumber].MoneyItemDrop(this->GetDropZen(), 
			lpObj->X, lpObj->Y);

		ITEMBAG_DROP_LOG.Output("[HalloweenDay Event][Pumpkin of Luck] Event Item Drop [%s][%s] [%d Zen] [RATE:%d/%d]",
			lpObj->AccountID, lpObj->Name, this->GetDropZen(), DropItemRate, this->GetItemDropRate());

		return TRUE;
	}

	if ( iItemBagCount > 0 )
	{
		iTotalDropRate = g_iHalloweenDayEventJOLBlessDropRate +
			             g_iHalloweenDayEventJOLAngerDropRaTe +
						 g_iHalloweenDayEventJOLScreamDropRate +
						 g_iHalloweenDayEventJOLFoodDropRate +
						 g_iHalloweenDayEventJOLDrinkDropRate +
						 g_iHalloweenDayEventJOLPolymorphRingDropRate;

		DropItemRate = rand() % iTotalDropRate;

		if ( DropItemRate >= (iTotalDropRate - g_iHalloweenDayEventJOLBlessDropRate)  )
			DropItemNum = 0;
		else if ( DropItemRate >= (iTotalDropRate - g_iHalloweenDayEventJOLBlessDropRate - g_iHalloweenDayEventJOLAngerDropRaTe) )
			DropItemNum = 1;
		else if ( DropItemRate >= (iTotalDropRate - g_iHalloweenDayEventJOLBlessDropRate - g_iHalloweenDayEventJOLAngerDropRaTe - g_iHalloweenDayEventJOLScreamDropRate) )
			DropItemNum = 2;
		else if ( DropItemRate >= (iTotalDropRate - g_iHalloweenDayEventJOLBlessDropRate - g_iHalloweenDayEventJOLAngerDropRaTe - g_iHalloweenDayEventJOLScreamDropRate - g_iHalloweenDayEventJOLFoodDropRate) )
			DropItemNum = 3;
		else if ( DropItemRate >= (iTotalDropRate - g_iHalloweenDayEventJOLBlessDropRate - g_iHalloweenDayEventJOLAngerDropRaTe - g_iHalloweenDayEventJOLScreamDropRate - g_iHalloweenDayEventJOLFoodDropRate - g_iHalloweenDayEventJOLDrinkDropRate) )
			DropItemNum = 4;
		else if ( DropItemRate >= (iTotalDropRate - g_iHalloweenDayEventJOLBlessDropRate - g_iHalloweenDayEventJOLAngerDropRaTe - g_iHalloweenDayEventJOLScreamDropRate - g_iHalloweenDayEventJOLFoodDropRate - g_iHalloweenDayEventJOLDrinkDropRate - g_iHalloweenDayEventJOLPolymorphRingDropRate) )
			DropItemNum = 5;

		X = lpObj->X;
		Y = lpObj->Y;
		iLevel = this->GetLevel(DropItemNum);
		iType = ItemGetNumberMake(this->BagObject[DropItemNum].m_type, this->BagObject[DropItemNum].m_index);

		if ( iType == -1 )
			return FALSE;

		if ( iType == ITEMGET(13,40) )
		{
			iLevel = 0;
			iDuration = 100;
		}

		ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, X, Y,
			iType, iLevel, iDuration, 0, 0, 0, lpObj->m_Index, 0, 0);

		ITEMBAG_DROP_LOG.Output("[HalloweenDay Event][Pumpkin of Luck] Event Item Drop [%s][%s] : (%d)(%d/%d) Item Attribute : %s(%d) Level : %d Dur : %d",
			lpObj->AccountID, lpObj->Name, lpObj->MapNumber, X, Y, ItemAttribute[iType].Name, iType, iLevel, iDuration);
	}

	return TRUE;
}

BOOL CItemBagEx::DropCrywolfPedestalRewardItem(int aIndex, BYTE btMapNumber, BYTE cX, BYTE cY)
{
	if ( this->m_bLoad == FALSE )
		return FALSE;

	float fDur = 0;
	int iType = 0;
	int iLevel = 0;
	int X = 0;
	int Y = 0;
	int iOption1 = 0;
	int iOption2 = 0;
	int iOption3 = 0;
	int DropItemNum = 0;
	int DropItemRate = 0;
	int iExOption = 0;
	LPOBJ lpObj = &gObj[aIndex];

	DropItemRate = rand() % 100;
	if ( DropItemRate > this->GetItemDropRate() )
	{
		if (this->GetDropZen() <= 0)
		{
			ITEMBAG_DROP_LOG.Output("[Crywolf] Pedestal Elf Reward NO ItemDrop [%s][%s] [RATE:%d/%d]",
				lpObj->AccountID, lpObj->Name, DropItemRate, this->GetItemDropRate());
			return FALSE;
		}

		MapC[lpObj->MapNumber].MoneyItemDrop(this->GetDropZen(), 
			lpObj->X, lpObj->Y);

		ITEMBAG_DROP_LOG.Output("[Crywolf][Reward] Pedestal Elf Reward Item Drop [%s][%s] [%d Zen] [RATE:%d/%d]",
			lpObj->AccountID, lpObj->Name, this->GetDropZen(), DropItemRate, this->GetItemDropRate());

		return TRUE;
	}

	if ( this->GetBagCount() > 0 )
	{
		DropItemNum = rand()% this->GetBagCount();
		fDur = 0;

		if ( cX == 0 && cY == 0 )
		{
			X = lpObj->X;
			Y = lpObj->Y;
		}
		else
		{
			X = cX;
			Y = cY;
		}

		iLevel = this->GetLevel(DropItemNum);
		iType = ItemGetNumberMake(this->BagObject[DropItemNum].m_type, this->BagObject[DropItemNum].m_index);

		if ( iType == -1 )
			return FALSE;

		if ( this->BagObject[DropItemNum].m_isskill )
			iOption1 = 1;

		if ( this->BagObject[DropItemNum].m_isluck )
		{
			iOption2 = 0;

			if ( (rand()%2) == 0 )
			{
				iOption2 = 1;
			}
		}

		if ( this->BagObject[DropItemNum].m_isoption > 0 )
		{
			int z28Percent = this->BagObject[DropItemNum].m_isoption*100;
			iOption3 = rand()%(z28Percent+1);
			iOption3 = iOption3 / 100;
		}

		DropItemRate = (rand() % 1000)+1;
		if (DropItemRate <= this->GetExItemDropRate())
		{
			if ( this->BagObject[DropItemNum].m_isexitem != 0 )
			{
				iExOption = BoxExcOptions(this->BagObject[DropItemNum].m_isexitem);
			}
		}

		ItemSerialCreateSend(lpObj->m_Index, btMapNumber, X, Y, iType, iLevel, fDur,
			iOption1, iOption2, iOption3, lpObj->m_Index, iExOption, 0);

		ITEMBAG_DROP_LOG.Output("[Crywolf][Reward] Pedestal Elf Reward Item Drop [%s][%s] : (%d)(%d/%d) Item:(%s)%d Level:%d op1:%d op2:%d op3:%d ExOp:%d",
			lpObj->AccountID, lpObj->Name, btMapNumber, X, Y, ItemAttribute[iType].Name,
			iType, iLevel, iOption1, iOption2, iOption3, iExOption);
	}

	return TRUE;
}

BOOL CItemBagEx::DropCrywolfDarkElfItem(int aIndex, BYTE btMapNumber, BYTE cX, BYTE cY)
{
	if ( this->m_bLoad == FALSE )
		return FALSE;

	float fDur = 0;
	int iType = 0;
	int iLevel = 0;
	int X = 0;
	int Y = 0;
	int iOption1 = 0;
	int iOption2 = 0;
	int iOption3 = 0;
	int DropItemNum = 0;
	int DropItemRate = 0;
	int iExOption = 0;
	LPOBJ lpObj = &gObj[aIndex];

	DropItemRate = rand() % 100;
	if ( DropItemRate > this->GetItemDropRate() )
	{
		if (this->GetDropZen() <= 0)
		{
			ITEMBAG_DROP_LOG.Output("[Crywolf] Dark Elf NO ItemDrop [%s][%s] [RATE:%d/%d]",
				lpObj->AccountID, lpObj->Name, DropItemRate, this->GetItemDropRate());
			return FALSE;
		}

		MapC[lpObj->MapNumber].MoneyItemDrop(this->GetDropZen(), 
			lpObj->X, lpObj->Y);

		ITEMBAG_DROP_LOG.Output("[Crywolf][Reward] Dark Elf Item Drop [%s][%s] [%d Zen] [RATE:%d/%d]",
			lpObj->AccountID, lpObj->Name, this->GetDropZen(), DropItemRate, this->GetItemDropRate());

		return TRUE;
	}

	if ( this->GetBagCount() > 0 )
	{
		DropItemNum = rand()% this->GetBagCount();
		fDur = 0;

		if ( cX == 0 && cY == 0 )
		{
			X = lpObj->X;
			Y = lpObj->Y;
		}
		else
		{
			X = cX;
			Y = cY;
		}

		iLevel = this->GetLevel(DropItemNum);
		iType = ItemGetNumberMake(this->BagObject[DropItemNum].m_type, this->BagObject[DropItemNum].m_index);

		if ( iType == -1 )
			return FALSE;

		if ( this->BagObject[DropItemNum].m_isskill )
			iOption1 = 1;

		if ( this->BagObject[DropItemNum].m_isluck )
		{
			iOption2 = 0;

			if ( (rand()%2) == 0 )
			{
				iOption2 = 1;
			}
		}

		if ( this->BagObject[DropItemNum].m_isoption > 0 )
		{
			int z28Percent = this->BagObject[DropItemNum].m_isoption*100;
			iOption3 = rand()%(z28Percent+1);
			iOption3 = iOption3 / 100;
		}

		DropItemRate = (rand() % 1000)+1;
		if (DropItemRate <= this->GetExItemDropRate())
		{
			if ( this->BagObject[DropItemNum].m_isexitem != 0 )
			{
				iExOption = BoxExcOptions(this->BagObject[DropItemNum].m_isexitem);
			}
		}

		ItemSerialCreateSend(lpObj->m_Index, btMapNumber, X, Y, iType, iLevel, fDur,
			iOption1, iOption2, iOption3, lpObj->m_Index, iExOption, 0);

		ITEMBAG_DROP_LOG.Output("[Crywolf][Reward] Dark Elf Item Drop [%s][%s] : (%d)(%d/%d) Item:(%s)%d Level:%d op1:%d op2:%d op3:%d ExOp:%d",
			lpObj->AccountID, lpObj->Name, btMapNumber, X, Y, ItemAttribute[iType].Name,
			iType, iLevel, iOption1, iOption2, iOption3, iExOption);
	}

	return TRUE;
}

BOOL CItemBagEx::DropCrywolfBossMonsterItem(int aIndex, BYTE btMapNumber, BYTE cX, BYTE cY)
{
	if ( this->m_bLoad == FALSE )
		return FALSE;

	float fDur = 0;
	int iType = 0;
	int iLevel = 0;
	int X = 0;
	int Y = 0;
	int iOption1 = 0;
	int iOption2 = 0;
	int iOption3 = 0;
	int DropItemNum = 0;
	int DropItemRate = 0;
	int iExOption = 0;
	LPOBJ lpObj = &gObj[aIndex];

	DropItemRate = rand() % 100;
	if ( DropItemRate > this->GetItemDropRate() )
	{
		if (this->GetDropZen() <= 0)
		{
			ITEMBAG_DROP_LOG.Output("[Crywolf] Boss NO ItemDrop [%s][%s] [RATE:%d/%d]",
				lpObj->AccountID, lpObj->Name, DropItemRate, this->GetItemDropRate());
			return FALSE;
		}

		MapC[lpObj->MapNumber].MoneyItemDrop(this->GetDropZen(), 
			lpObj->X, lpObj->Y);

		ITEMBAG_DROP_LOG.Output("[Crywolf][Reward] Boss Monster Item Drop [%s][%s] [%d Zen] [RATE:%d/%d]",
			lpObj->AccountID, lpObj->Name, this->GetDropZen(), DropItemRate, this->GetItemDropRate());

		return TRUE;
	}

	if ( this->GetBagCount() > 0 )
	{
		DropItemNum = rand()% this->GetBagCount();
		fDur = 0;

		if ( cX == 0 && cY == 0 )
		{
			X = lpObj->X;
			Y = lpObj->Y;
		}
		else
		{
			X = cX;
			Y = cY;
		}

		iLevel = this->GetLevel(DropItemNum);
		iType = ItemGetNumberMake(this->BagObject[DropItemNum].m_type, this->BagObject[DropItemNum].m_index);

		if ( iType == -1 )
			return FALSE;

		if ( this->BagObject[DropItemNum].m_isskill )
			iOption1 = 1;

		if ( this->BagObject[DropItemNum].m_isluck )
		{
			iOption2 = 0;

			if ( (rand()%2) == 0 )
			{
				iOption2 = 1;
			}
		}

		if ( this->BagObject[DropItemNum].m_isoption > 0 )
		{
			int z28Percent = this->BagObject[DropItemNum].m_isoption*100;
			iOption3 = rand()%(z28Percent+1);
			iOption3 = iOption3 / 100;
		}

		DropItemRate = (rand() % 1000)+1;
		if (DropItemRate <= this->GetExItemDropRate())
		{
			if ( this->BagObject[DropItemNum].m_isexitem != 0 )
			{
				iExOption = BoxExcOptions(this->BagObject[DropItemNum].m_isexitem);
			}
		}

		ItemSerialCreateSend(lpObj->m_Index, btMapNumber, X, Y, iType, iLevel, fDur,
			iOption1, iOption2, iOption3, lpObj->m_Index, iExOption, 0);

		ITEMBAG_DROP_LOG.Output("[Crywolf][Reward] Boss Monster Item Drop [%s][%s] : (%d)(%d/%d) Item:(%s)%d Level:%d op1:%d op2:%d op3:%d ExOp:%d",
			lpObj->AccountID, lpObj->Name, btMapNumber, X, Y, ItemAttribute[iType].Name,
			iType, iLevel, iOption1, iOption2, iOption3, iExOption);
	}

	return TRUE;
}

BOOL CItemBagEx::DropKanturuMayaHandItem(int aIndex, BYTE btMapNumber, BYTE cX, BYTE cY)
{
	if ( this->m_bLoad == FALSE )
		return FALSE;

	float fDur = 0;
	int iType = 0;
	int iLevel = 0;
	int X = 0;
	int Y = 0;
	int iOption1 = 0;
	int iOption2 = 0;
	int iOption3 = 0;
	int DropItemNum = 0;
	int DropItemRate = 0;
	int iExOption = 0;
	LPOBJ lpObj = &gObj[aIndex];

	DropItemRate = rand() % 100;
	if ( DropItemRate > this->GetItemDropRate() )
	{
		if (this->GetDropZen() <= 0)
		{
			ITEMBAG_DROP_LOG.Output("[Kanturu] MayaHand NO ItemDrop [%s][%s] [RATE:%d/%d]",
				lpObj->AccountID, lpObj->Name, DropItemRate, this->GetItemDropRate());
			return FALSE;
		}

		MapC[lpObj->MapNumber].MoneyItemDrop(this->GetDropZen(), 
			lpObj->X, lpObj->Y);

		ITEMBAG_DROP_LOG.Output("[Kanturu][Reward] MayaHand Item Drop [%s][%s] [%d Zen] [RATE:%d/%d]",
			lpObj->AccountID, lpObj->Name, this->GetDropZen(), DropItemRate, this->GetItemDropRate());

		return TRUE;
	}

	if ( this->GetBagCount() > 0 )
	{
		DropItemNum = rand()% this->GetBagCount();
		fDur = 0;

		if ( cX == 0 && cY == 0 )
		{
			X = lpObj->X;
			Y = lpObj->Y;
		}
		else
		{
			X = cX;
			Y = cY;
		}

		iLevel = this->GetLevel(DropItemNum);
		iType = ItemGetNumberMake(this->BagObject[DropItemNum].m_type, this->BagObject[DropItemNum].m_index);

		if ( iType == -1 )
			return FALSE;

		if ( this->BagObject[DropItemNum].m_isskill )
			iOption1 = 1;

		if ( this->BagObject[DropItemNum].m_isluck )
		{
			iOption2 = 0;

			if ( (rand()%2) == 0 )
			{
				iOption2 = 1;
			}
		}

		if ( this->BagObject[DropItemNum].m_isoption > 0 )
		{
			int z28Percent = this->BagObject[DropItemNum].m_isoption*100;
			iOption3 = rand()%(z28Percent+1);
			iOption3 = iOption3 / 100;
		}

		DropItemRate = (rand() % 1000)+1;
		if (DropItemRate <= this->GetExItemDropRate())
		{
			if ( this->BagObject[DropItemNum].m_isexitem != 0 )
			{
				iExOption = BoxExcOptions(this->BagObject[DropItemNum].m_isexitem);
			}
		}

		ItemSerialCreateSend(lpObj->m_Index, btMapNumber, X, Y, iType, iLevel, fDur,
			iOption1, iOption2, iOption3, lpObj->m_Index, iExOption, 0);

		ITEMBAG_DROP_LOG.Output("[Kanturu][Reward] MayaHand Item Drop [%s][%s] : (%d)(%d/%d) Item:(%s)%d Level:%d op1:%d op2:%d op3:%d ExOp:%d",
			lpObj->AccountID, lpObj->Name, btMapNumber, X, Y, ItemAttribute[iType].Name,
			iType, iLevel, iOption1, iOption2, iOption3, iExOption);
	}

	return TRUE;
}

BOOL CItemBagEx::DropKanturuNightmareItem(int aIndex, BYTE btMapNumber, BYTE cX, BYTE cY)
{
	if ( this->m_bLoad == FALSE )
		return FALSE;

	float fDur = 0;
	int iType = 0;
	int iLevel = 0;
	int X = 0;
	int Y = 0;
	int iOption1 = 0;
	int iOption2 = 0;
	int iOption3 = 0;
	int DropItemNum = 0;
	int DropItemRate = 0;
	int iExOption = 0;
	LPOBJ lpObj = &gObj[aIndex];

	DropItemRate = rand() % 100;
	if ( DropItemRate > this->GetItemDropRate() )
	{
		if (this->GetDropZen() <= 0)
		{
			ITEMBAG_DROP_LOG.Output("[Kanturu] Nightmare NO ItemDrop [%s][%s] [RATE:%d/%d]",
				lpObj->AccountID, lpObj->Name, DropItemRate, this->GetItemDropRate());
			return FALSE;
		}

		MapC[lpObj->MapNumber].MoneyItemDrop(this->GetDropZen(), 
			lpObj->X, lpObj->Y);

		ITEMBAG_DROP_LOG.Output("[Kanturu][Reward] Nightmare Item Drop [%s][%s] [%d Zen] [RATE:%d/%d]",
			lpObj->AccountID, lpObj->Name, this->GetDropZen(), DropItemRate, this->GetItemDropRate());

		return TRUE;
	}

	if ( this->GetBagCount() > 0 )
	{
		DropItemNum = rand()% this->GetBagCount();
		fDur = 0;

		if ( cX == 0 && cY == 0 )
		{
			X = lpObj->X;
			Y = lpObj->Y;
		}
		else
		{
			X = cX;
			Y = cY;
		}

		iLevel = this->GetLevel(DropItemNum);
		iType = ItemGetNumberMake(this->BagObject[DropItemNum].m_type, this->BagObject[DropItemNum].m_index);

		if ( iType == -1 )
			return FALSE;

		if ( this->BagObject[DropItemNum].m_isskill )
			iOption1 = 1;

		if ( this->BagObject[DropItemNum].m_isluck )
		{
			iOption2 = 0;

			if ( (rand()%2) == 0 )
			{
				iOption2 = 1;
			}
		}

		if ( this->BagObject[DropItemNum].m_isoption > 0 )
		{
			int z28Percent = this->BagObject[DropItemNum].m_isoption*100;
			iOption3 = rand()%(z28Percent+1);
			iOption3 = iOption3 / 100;
		}

		DropItemRate = (rand() % 1000)+1;
		if (DropItemRate <= this->GetExItemDropRate())
		{
			if ( this->BagObject[DropItemNum].m_isexitem != 0 )
			{
				iExOption = BoxExcOptions(this->BagObject[DropItemNum].m_isexitem);
			}
		}

		ItemSerialCreateSend(lpObj->m_Index, btMapNumber, X, Y, iType, iLevel, fDur,
			iOption1, iOption2, iOption3, lpObj->m_Index, iExOption, 0);

		ITEMBAG_DROP_LOG.Output("[Kanturu][Reward] Nightmare Item Drop [%s][%s] : (%d)(%d/%d) Item:(%s)%d Level:%d op1:%d op2:%d op3:%d ExOp:%d",
			lpObj->AccountID, lpObj->Name, btMapNumber, X, Y, ItemAttribute[iType].Name,
			iType, iLevel, iOption1, iOption2, iOption3, iExOption);
	}

	return TRUE;
}

BOOL CItemBagEx::DropHellMainItem(int aIndex, BYTE btMapNumber, BYTE cX, BYTE cY)
{
	if ( this->m_bLoad == FALSE )
		return FALSE;

	float fDur = 0;
	int iType = 0;
	int iLevel = 0;
	int X = 0;
	int Y = 0;
	int iOption1 = 0;
	int iOption2 = 0;
	int iOption3 = 0;
	int DropItemNum = 0;
	int DropItemRate = 0;
	int iExOption = 0;
	LPOBJ lpObj = &gObj[aIndex];

	DropItemRate = rand() % 100;
	if ( DropItemRate > this->GetItemDropRate() )
	{
		if (this->GetDropZen() <= 0)
		{
			ITEMBAG_DROP_LOG.Output("[HellMain] NO ItemDrop [%s][%s] [RATE:%d/%d]",
				lpObj->AccountID, lpObj->Name, DropItemRate, this->GetItemDropRate());
			return FALSE;
		}

		MapC[lpObj->MapNumber].MoneyItemDrop(this->GetDropZen(), 
			lpObj->X, lpObj->Y);

		ITEMBAG_DROP_LOG.Output("[HellMain] Item Drop [%s][%s] [%d Zen] [RATE:%d/%d]",
			lpObj->AccountID, lpObj->Name, this->GetDropZen(), DropItemRate, this->GetItemDropRate());

		return TRUE;
	}

	if ( this->GetBagCount() > 0 )
	{
		DropItemNum = rand()% this->GetBagCount();
		fDur = 0;

		if ( cX == 0 && cY == 0 )
		{
			X = lpObj->X;
			Y = lpObj->Y;
		}
		else
		{
			X = cX;
			Y = cY;
		}

		iLevel = this->GetLevel(DropItemNum);
		iType = ItemGetNumberMake(this->BagObject[DropItemNum].m_type, this->BagObject[DropItemNum].m_index);

		if ( iType == -1 )
			return FALSE;

		if ( this->BagObject[DropItemNum].m_isskill )
			iOption1 = 1;

		if ( this->BagObject[DropItemNum].m_isluck )
		{
			iOption2 = 0;

			if ( (rand()%2) == 0 )
			{
				iOption2 = 1;
			}
		}

		if ( this->BagObject[DropItemNum].m_isoption > 0 )
		{
			int z28Percent = this->BagObject[DropItemNum].m_isoption*100;
			iOption3 = rand()%(z28Percent+1);
			iOption3 = iOption3 / 100;
		}

		DropItemRate = (rand() % 1000)+1;
		if (DropItemRate <= this->GetExItemDropRate())
		{
			if ( this->BagObject[DropItemNum].m_isexitem != 0 )
			{
				iExOption = BoxExcOptions(this->BagObject[DropItemNum].m_isexitem);
			}
		}

		ItemSerialCreateSend(lpObj->m_Index, btMapNumber, X, Y, iType, iLevel, fDur,
			iOption1, iOption2, iOption3, lpObj->m_Index, iExOption, 0);

		ITEMBAG_DROP_LOG.Output("[HellMain] Item Drop [%s][%s] : (%d)(%d/%d) Item:(%s)%d Level:%d op1:%d op2:%d op3:%d ExOp:%d",
			lpObj->AccountID, lpObj->Name, btMapNumber, X, Y, ItemAttribute[iType].Name,
			iType, iLevel, iOption1, iOption2, iOption3, iExOption);
	}

	return TRUE;
}