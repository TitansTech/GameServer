#include "stdafx.h"
#include "GameServer.h"
#include "LogProc.h"
#include "GameMain.h"
#include "User.h"
#include "ObjLuckyPenny.h"
#include "DSProtocol.h"
#include "..\common\winutil.h"
#include "..\include\readscript.h"
#include "..\common\SetItemOption.h"

#include "LogToFile.h"
extern CLogToFile ANTI_HACK_LOG;

cLuckyPenny LuckyPenny;

void cLuckyPenny::RegisterOpt(int aIndex)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	if(this->Enabled == TRUE)
	{
		PMSG_LUCKYCOINSCOUNT result;
		PHeadSubSetB((LPBYTE)&result, 0xBF, 0x0B, sizeof(result));
		result.Count = gObj[aIndex].LuckyCoins;
		DataSend(gObj[aIndex].m_Index,(BYTE*)&result,sizeof(result));
	}
}

void cLuckyPenny::RegisterSend(int aIndex)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	if(this->Enabled == TRUE)
	{
		if ( gObj[aIndex].m_IfState.type != 97 )
		{
			ANTI_HACK_LOG.Output("[ANTI-HACK][Coins][Register] User in map(%d) [%s][%s] State(u:%d,t:%d,s:%d)",
				gObj[aIndex].MapNumber, gObj[aIndex].AccountID, gObj[aIndex].Name,
				gObj[aIndex].m_IfState.use,gObj[aIndex].m_IfState.type,gObj[aIndex].m_IfState.state);
			return;
		}

		int pos = gObjCheckItemPos(aIndex,ITEMGET(14,100));
		if(pos >= 0)
		{
			if ( ReadConfig.MAIN_INVENTORY_RANGE(aIndex,pos,false) != FALSE )
			{
				gObj[aIndex].pInventory[pos].m_Durability -= 1;
				gObj[aIndex].LuckyCoins++;
				
				PMSG_LUCKYCOINSCOUNT result;
				PHeadSubSetB((LPBYTE)&result, 0xBF, 0x0B, sizeof(result));
				result.Count = gObj[aIndex].LuckyCoins;
				DataSend(gObj[aIndex].m_Index,(BYTE*)&result,sizeof(result));
				
				if(gObj[aIndex].pInventory[pos].m_Durability <= 0)
				{
					gObjInventoryItemSet(aIndex, pos, -1);
					gObj[aIndex].pInventory[pos].Clear();
					GCInventoryItemDeleteSend(aIndex, pos, 1);
				}else
				{
					GCItemDurSend(aIndex, pos, gObj[aIndex].pInventory[pos].m_Durability, 0);
				}
				
				char RegisterButton[4] = {0xC1,0x04,0xBF,0x0C};
				DataSend(aIndex ,(UCHAR*)RegisterButton, sizeof(RegisterButton));

				gObj[aIndex].AccountExtraInfoModified = 1;
			}
		}else
		{
			char LackPacket[5] = {0xC1,0x05,0xBF,0x0C,0x00};
			DataSend(aIndex ,(UCHAR*)LackPacket, sizeof(LackPacket));
		}
	}
}

void cLuckyPenny::Exchange(int aIndex,BYTE Count)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	if(this->Enabled == TRUE)
	{
		if ( gObj[aIndex].m_IfState.type != 97 )
		{
			ANTI_HACK_LOG.Output("[ANTI-HACK][Coins][Exchange] User in map(%d) [%s][%s] State(u:%d,t:%d,s:%d)",
				gObj[aIndex].MapNumber, gObj[aIndex].AccountID, gObj[aIndex].Name,
				gObj[aIndex].m_IfState.use,gObj[aIndex].m_IfState.type,gObj[aIndex].m_IfState.state);
			return;
		}

		if(Count != 10 && Count != 20 && Count != 30)
			return;

		int pos = gObjCheckItemPosByDur(aIndex,ITEMGET(14,100),Count);

		if(pos >= 0)
		{
			if ( ReadConfig.MAIN_INVENTORY_RANGE(aIndex,pos,false) != FALSE )
			{
				if(this->GetReward(aIndex,Count) == true)
				{
					gObj[aIndex].pInventory[pos].m_Durability -= Count;
				
					if(gObj[aIndex].pInventory[pos].m_Durability <= 0)
					{
						gObjInventoryItemSet(aIndex, pos, -1);
						gObj[aIndex].pInventory[pos].Clear();
						GCInventoryItemDeleteSend(aIndex, pos, 1);
					}else
					{
						GCItemDurSend(aIndex, pos, gObj[aIndex].pInventory[pos].m_Durability, 0);
					}

					LogAddTD("[LuckyPenny] [%s][%s][%d] LuckyPenny Item Buy Exchange Success",
						gObj[aIndex].AccountID, gObj[aIndex].Name,Count);
				}else
				{
					LogAddTD("[LuckyPenny] [%s][%s] LuckyPenny Item Buy Exchange Failed : No Room to Get Item",
						gObj[aIndex].AccountID, gObj[aIndex].Name);
					GCServerMsgStringSend("No Room in Inventory to Get Item!!",aIndex, 1);
				}
			}
		}else
		{
			char LackPacket[5] = {0xC1,0x05,0xBF,0x0C,0x00};
			DataSend(aIndex ,(UCHAR*)LackPacket, sizeof(LackPacket));
		}
	}
}

bool cLuckyPenny::GetReward(int aIndex,BYTE Count)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return false;
	}

	int cnt = (Count / 10)-1;
	if(cnt >= 0 && cnt <= 2)
	{
		int RandNum = rand()%this->iCount[cnt];
		
		BYTE TempInventoryMap[MAIN_INVENTORY_NORMAL_SIZE + (MAX_EXTENDED_INV*EXTENDED_INV_SIZE) + PSHOP_SIZE];
		memcpy(TempInventoryMap,gObj[aIndex].pInventoryMap,ReadConfig.GETSLOTS(aIndex));

		if(gObjTempInventoryInsertItem(&gObj[aIndex],this->items[cnt][RandNum].num,TempInventoryMap) != 255)
		{
			ItemSerialCreateSend(aIndex, 235, 0, 0, this->items[cnt][RandNum].num, this->items[cnt][RandNum].Level, this->items[cnt][RandNum].Dur, this->items[cnt][RandNum].Skill, this->items[cnt][RandNum].Luck, (this->items[cnt][RandNum].Opt/4), -1, this->items[cnt][RandNum].Exc, this->items[cnt][RandNum].Anc);
			return true;
		}
	}
	return false;
}

void cLuckyPenny::Read(char * FilePath)
{	
	this->Enabled = GetPrivateProfileInt("Common", "SCFLuckyPennyEnabled",0, FilePath);

	LogAddTD("[SCFLuckyPenny] - %s file is Loaded",FilePath);
}

void cLuckyPenny::ReadBag(char * FilePath)
{
	if(this->Enabled == TRUE)
	{
		int Count=0;
		int Num=0;
		int Token;
		SMDFile = fopen(FilePath, "r");

		for(int i=0;i<3;i++)
		{
			this->iCount[i] = 0;
		}

		if ( SMDFile == NULL )
		{
			MsgBox("LuckyPenny data load error %s", FilePath);
			return;
		}

		while ( true )
		{
			Num=0;
			int iType = GetToken();
			
			if ( iType == 1 )
			{
				if(Count >= 3)
				{
					break;
				}
				while(true)
				{
					Token = GetToken();
					if ( strcmp("end", TokenString) == 0 )
					{
						break;
					}

					if(Num >= LUCKYPENNY_REWARDITEMS_COUNT)
					{
						MsgBox("LuckyPenny error: Items out of range!");
						return;
					}

					int iType = TokenNumber;

					Token = GetToken();
					int iIndex = TokenNumber;

					this->items[Count][Num].num = ITEMGET(iType,iIndex);

					Token = GetToken();
					this->items[Count][Num].Level = TokenNumber;

					Token = GetToken();
					this->items[Count][Num].Opt = TokenNumber;

					Token = GetToken();
					this->items[Count][Num].Luck = TokenNumber;

					Token = GetToken();
					this->items[Count][Num].Skill = TokenNumber;

					Token = GetToken();
					this->items[Count][Num].Dur = TokenNumber;

					Token = GetToken();
					this->items[Count][Num].Exc = TokenNumber;

					Token = GetToken();
					this->items[Count][Num].Anc = TokenNumber;

					Num++;
					this->iCount[Count]++;
				}
			}else
			{		
				break;
			}

			Count++;
		}
				
		LogAddTD("[SCFLuckyPenny] - %s file is Loaded",FilePath);
		fclose(SMDFile);
	}
}