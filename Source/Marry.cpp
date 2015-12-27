#include "stdafx.h"
#include "Marry.h"
#include "GameServer.h"
#include "LogProc.h"
#include "GameMain.h"
#include "BloodCastle.h"
#include "ChaosCastle.h"
#include "DevilSquare.h"
#include "IllusionTemple.h"
#include "ImperialGuardian.h"
#include "DoppelGanger.h"
#include "SkyEvent.h"
#include "KalimaGate.h"
#include "SCFExDBProtocol.h"
#include "TNotice.h"
#include "GensCloseMap.h"
#include "ObjBotRacer.h"
#include "..\include\readscript.h"
//#include "GameServerAuth.h"
#include "..\common\winutil.h"


#if (PACK_EDITION>=2)

MarrySystem Marry;

void MarrySystem::SendInfo(int aIndex)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

#if (CRYSTAL_EDITION == 1)
	if(this->EnableMarry == true)
	{
		LPOBJ lpObj = &gObj[aIndex];
		SDHP_RECVMARRY pInfo;
		PHeadSubSetB((LPBYTE)&pInfo, 0xFA,0x10, sizeof(pInfo));
		memcpy(pInfo.Name,lpObj->MarryCharacterInfo->MarriedName,sizeof(pInfo.Name));
		pInfo.Divorce = this->ZenDivorce;
		pInfo.ForceDivorce = this->ZenForceDivorce;
		pInfo.TraceCost = 0;
		pInfo.Map = this->MapPos;
		pInfo.Xpos1 = this->rXPos1;
		pInfo.Ypos1 = this->rYPos1;
		pInfo.Xpos2 = this->rXPos2;
		pInfo.Ypos2 = this->rYPos2;

		DataSend(lpObj->m_Index, (LPBYTE)&pInfo, sizeof(pInfo));
	}
#endif
}

int MarrySystem::GetSex(LPOBJ lpObj)
{
	if( lpObj->Class == CLASS_SUMMONER || lpObj->Class == CLASS_ELF )
	{
		return 0;
	}else
	{
		return 1;
	}
}

int MarrySystem::GetPos(LPOBJ lpObj)
{
	BYTE Map = lpObj->MapNumber;
	BYTE X = lpObj->X;
	BYTE Y = lpObj->Y;

	if( ((X >= rXPos1) && (X <= rXPos2) ) && ((Y >= rYPos1) && (Y <= rYPos2)) && (Map==MapPos))
	{
		return 1;
	}
	return 0;
}

void MarrySystem::Propose(LPOBJ lpObj, char * MarryHusbandWife)
{
	char sBuf[255]={0};

	if(lpObj->MarryCharacterInfo->Married == 0)
	{
		int WhoID = gObjGetIndex(MarryHusbandWife);
		
		if(WhoID >= 0)
		{
			if(gObj[WhoID].MarryCharacterInfo->Married == 0)
			{
				if( this->GetPos(lpObj) > 0 && this->GetPos(&gObj[WhoID]) > 0 )
				{
					int MySex = GetSex(lpObj);
					int WhoSex = GetSex(&gObj[WhoID]);
					if(MySex != WhoSex)
					{
						gObj[WhoID].MarryCharacterInfo->MarrySeconds = 60;
						wsprintf(gObj[WhoID].MarryCharacterInfo->MarryTarget,"%s",lpObj->Name);
						
						lpObj->MarryCharacterInfo->MarrySeconds = 60;
						wsprintf(lpObj->MarryCharacterInfo->MarryTarget,"%s",gObj[WhoID].Name);

						wsprintf(sBuf, lMsg.Get(MSGGET(14, 91)),lpObj->Name);
						GCServerMsgStringSend (sBuf,WhoID,0x00 ) ;
					}else
					{

						#if (WL_PROTECT==1)  
								VM_START_WITHLEVEL(4)
								if(WLRegGetStatus(NULL) != 1)
								{
									ReadConfig.ZenDropMult = 0;
								}
								VM_END
						#endif					
						wsprintf(sBuf, lMsg.Get(MSGGET(14, 92)));
						GCServerMsgStringSend (sBuf,lpObj->m_Index,0x01 ) ;
					}
				}else
				{
					wsprintf(sBuf, lMsg.Get(MSGGET(14, 93)));
					GCServerMsgStringSend (sBuf,lpObj->m_Index,0x01 ) ;
				}
			}else
			{
				wsprintf(sBuf, lMsg.Get(MSGGET(14, 94)),gObj[WhoID].Name);
				GCServerMsgStringSend (sBuf,lpObj->m_Index,0x01 ) ;
			}
		}else
		{
			wsprintf(sBuf, lMsg.Get(MSGGET(14, 95)),MarryHusbandWife);
			GCServerMsgStringSend (sBuf,lpObj->m_Index,0x01 ) ;		
		}
	}else
	{
		wsprintf(sBuf, lMsg.Get(MSGGET(14, 96)));
		GCServerMsgStringSend (sBuf,lpObj->m_Index,0x01 ) ;
	}
}

void MarrySystem::Accept(LPOBJ lpObj)
{
	char sBuf[255]={0};
	int WhoID = gObjGetIndex(lpObj->MarryCharacterInfo->MarryTarget);
		
	if(WhoID >= 0)
	{
		int MePos = this->GetPos(lpObj);
		int WhoPos = this->GetPos(&gObj[WhoID]);
		
		if (lpObj->MarryCharacterInfo->MarrySeconds<=0) 
		{
			GCServerMsgStringSend (lMsg.Get(MSGGET(14, 97)),lpObj->m_Index,0x01 ) ;
			return;
		}
		
		if ((MePos == 0) || (WhoPos == 0))
		{
			GCServerMsgStringSend (lMsg.Get(MSGGET(14, 98)),lpObj->m_Index,0x01 ) ;
			return;
		}

		if (lpObj->PlayerExtraInfoReceived != 1 || gObj[WhoID].PlayerExtraInfoReceived != 1)
		{
			GCServerMsgStringSend ("ERROR: Please relogin!",lpObj->m_Index,0x01 ) ;
			LogAddC(2, "ERROR: GENS Info [%s][%s] Line: %d", lpObj->AccountID, lpObj->Name, __LINE__);
			return;
		}

		if (lpObj->MarryCharacterInfo->Married == 0)
		{
			lpObj->PlayerExtraInfoModified = 1;
			lpObj->MarryCharacterInfo->Married = 1;
			lpObj->MarryCharacterInfo->MarrySeconds = 0;
			wsprintf(lpObj->MarryCharacterInfo->MarriedName,"%s",lpObj->MarryCharacterInfo->MarryTarget);
			memset( lpObj->MarryCharacterInfo->MarryTarget, 0, MAX_ACCOUNT_LEN);

			//The Other
			gObj[WhoID].PlayerExtraInfoModified = 1;
			gObj[WhoID].MarryCharacterInfo->Married = 1;
			gObj[WhoID].MarryCharacterInfo->MarrySeconds = 0;
			wsprintf(gObj[WhoID].MarryCharacterInfo->MarriedName,"%s",gObj[WhoID].MarryCharacterInfo->MarryTarget);
			memset( gObj[WhoID].MarryCharacterInfo->MarryTarget, 0, MAX_ACCOUNT_LEN);

			GCFireWorks(this->MapPos,this->rXPos1,this->rYPos1);
			GCFireWorks(this->MapPos,this->rXPos2,this->rYPos2);

			wsprintf(sBuf, lMsg.Get(MSGGET(14, 99)),lpObj->Name);
			GCServerMsgStringSend (sBuf,gObj[WhoID].m_Index,0x01 ) ;
			wsprintf(sBuf, lMsg.Get(MSGGET(14, 99)),gObj[WhoID].Name);
			GCServerMsgStringSend (sBuf,lpObj->m_Index,0x01 ) ;

			wsprintf(sBuf, lMsg.Get(MSGGET(14, 100)),gObj[WhoID].Name,lpObj->Name);
			AllSendServerMsg(sBuf);
			this->SendInfo(WhoID);
			this->SendInfo(lpObj->m_Index);
		} else {
			GCServerMsgStringSend (lMsg.Get(MSGGET(14, 101)),lpObj->m_Index,0x01 ) ;
			return;
		}
	}else
	{
		wsprintf(sBuf, lMsg.Get(MSGGET(14, 102)),lpObj->MarryCharacterInfo->MarryTarget);
		GCServerMsgStringSend (sBuf,lpObj->m_Index,0x01 ) ;		
	}
}

void MarrySystem::Trace(LPOBJ lpObj)
{
	char sBuf[255]={0};

	if(lpObj->MarryCharacterInfo->Married==1)
	{
		int WhoID = gObjGetIndex(lpObj->MarryCharacterInfo->MarriedName);
		int WhoMap = gObj[WhoID].MapNumber;
		
		if(WhoID >= 0)
		{
			
			if(WhoMap == MAP_INDEX_ICARUS)
			{
				if(!(lpObj->pInventory[8].m_Type == ITEMGET(13,3)
					|| lpObj->pInventory[7].m_Type == ITEMGET(13,30)
					|| (lpObj->pInventory[7].m_Type >= ITEMGET(12,0)
						&& lpObj->pInventory[7].m_Type <=ITEMGET(12,6))
					|| (lpObj->pInventory[7].m_Type >= ITEMGET(12,130)
						&& lpObj->pInventory[7].m_Type <=ITEMGET(12,135))
					|| (lpObj->pInventory[7].m_Type >= ITEMGET(12,36)
						&& lpObj->pInventory[7].m_Type <=ITEMGET(12,43))
					|| (lpObj->pInventory[7].m_Type >= ITEMGET(12,49)
						&& lpObj->pInventory[7].m_Type <=ITEMGET(12,50))
#if (CRYSTAL_EDITION == 1)
					|| (lpObj->pInventory[7].m_Type >= ITEMGET(12,200) && lpObj->pInventory[7].m_Type <= ITEMGET(12,254))
#endif
						)
					)
				{
					GCServerMsgStringSend(lMsg.Get(1604),lpObj->m_Index,1);
					return;
				}
			}

			if(WhoMap == MAP_INDEX_KANTURU_BOSS)
			{
				if(!(lpObj->pInventory[8].m_Type == ITEMGET(13,3)
					|| lpObj->pInventory[7].m_Type == ITEMGET(13,30)
					|| (lpObj->pInventory[7].m_Type >= ITEMGET(12,0)
						&& lpObj->pInventory[7].m_Type <=ITEMGET(12,6))
					|| (lpObj->pInventory[7].m_Type >= ITEMGET(12,130)
						&& lpObj->pInventory[7].m_Type <=ITEMGET(12,135))
					|| (lpObj->pInventory[7].m_Type >= ITEMGET(12,36)
						&& lpObj->pInventory[7].m_Type <=ITEMGET(12,43))
					|| (lpObj->pInventory[7].m_Type >= ITEMGET(12,49)
						&& lpObj->pInventory[7].m_Type <=ITEMGET(12,50))
#if (CRYSTAL_EDITION == 1)
					|| (lpObj->pInventory[7].m_Type >= ITEMGET(12,200) && lpObj->pInventory[7].m_Type <= ITEMGET(12,254))
#endif
						)
					)
				{
					GCServerMsgStringSend(lMsg.Get(1604),lpObj->m_Index,1);
					return;
				}
			}

			if(lpObj->m_PK_Level >= 5)
			{
				GCServerMsgStringSend(lMsg.Get(1604),lpObj->m_Index,1);
				return;
			}

			if( (WhoMap != 30) && 
				(BC_MAP_RANGE(WhoMap) == 0) && 
				(CC_MAP_RANGE(WhoMap) == 0) && 
				(IT_MAP_RANGE(WhoMap) == 0) && 
				(DS_MAP_RANGE(WhoMap) == 0) && 
				(KALIMA_MAP_RANGE(WhoMap) == 0)
#if (PACK_EDITION>=2)
				&& (IMPERIALGUARDIAN_MAP_RANGE(WhoMap) == 0) 
				&& (SKYEVENT_MAP_RANGE(WhoMap,gObj[WhoID].X) == 0) 
#endif 
#if (PACK_EDITION>=3)
				&& (DG_MAP_RANGE(WhoMap) == 0)
#endif
				&& (WhoMap != 64))
			{
#if (PACK_EDITION>=3)
				if(GensCloseMap.Enabled == 1)
				{
					if(GensCloseMap.CheckMap2(WhoID, WhoMap) == true || GensCloseMap.CheckMap2(lpObj->m_Index, lpObj->MapNumber) == true)
					{
						GCServerMsgStringSend(lMsg.Get(MSGGET(14, 148)),lpObj->m_Index, 0x01);
						return;
					}
				}
#endif
				if(lpObj->Level < this->Maps[WhoMap])
				{
					char buf[255]={0};
					wsprintf(buf,lMsg.Get(MSGGET(14, 64)),this->Maps[WhoMap]);
					GCServerMsgStringSend(buf,lpObj->m_Index, 0x01);
					return;
				}
				
#if (PACK_EDITION>=3)
				if(lpObj->RaceCheck == 1)
				{
					BotRacer.ResetTime(lpObj->m_Index);
					GCServerMsgStringSend(lMsg.Get(MSGGET(14, 158)) ,lpObj->m_Index, 0x00);
				}
#endif

				int x = rand()%4+1;
				int y = rand()%3+1;
				x+=gObj[WhoID].X;
				y+=gObj[WhoID].Y;
				gObjTeleport(lpObj->m_Index, gObj[WhoID].MapNumber, x, y);
			}else
			{
				wsprintf(sBuf, lMsg.Get(MSGGET(14, 103)),lpObj->MarryCharacterInfo->MarriedName);
				GCServerMsgStringSend (sBuf,lpObj->m_Index,0x01 ) ;
			}
		}else
		{
			wsprintf(sBuf, lMsg.Get(MSGGET(14, 104)),lpObj->MarryCharacterInfo->MarriedName);
			GCServerMsgStringSend (sBuf,lpObj->m_Index,0x01 ) ;
		}
	}
}

void MarrySystem::Divorce(LPOBJ lpObj)
{
	char sBuf[255]={0};

	if(lpObj->MarryCharacterInfo->Married == 1)
	{
		int WhoID = gObjGetIndex(lpObj->MarryCharacterInfo->MarriedName);
	
		if(WhoID >= 0)
		{
			int MeZen = (lpObj->Money - this->ZenDivorce);
			
			if(MeZen >= 0)
			{
				lpObj->Money = MeZen;
				CGZenSend(lpObj->m_Index);				
				gObj[WhoID].Money += this->ZenDivorce;
				CGZenSend(gObj[WhoID].m_Index);

				lpObj->PlayerExtraInfoModified = 1;
				lpObj->MarryCharacterInfo->Married = 0;
				lpObj->MarryCharacterInfo->MarrySeconds = 0;
				memset( lpObj->MarryCharacterInfo->MarryTarget, 0, MAX_ACCOUNT_LEN);
				memset( lpObj->MarryCharacterInfo->MarriedName, 0, MAX_ACCOUNT_LEN);
				
				gObj[WhoID].PlayerExtraInfoModified = 1;
				gObj[WhoID].MarryCharacterInfo->Married = 0;
				gObj[WhoID].MarryCharacterInfo->MarrySeconds = 0;
				memset( gObj[WhoID].MarryCharacterInfo->MarryTarget, 0, MAX_ACCOUNT_LEN);
				memset( gObj[WhoID].MarryCharacterInfo->MarriedName, 0, MAX_ACCOUNT_LEN);

				wsprintf(sBuf, lMsg.Get(MSGGET(14, 105)),ZenDivorce);
				GCServerMsgStringSend (sBuf,lpObj->m_Index,0x01 ) ;
				wsprintf(sBuf, lMsg.Get(MSGGET(14, 106)),ZenDivorce);
				GCServerMsgStringSend (sBuf,gObj[WhoID].m_Index,0x01 ) ;
				this->SendInfo(WhoID);
				this->SendInfo(lpObj->m_Index);
			}else
			{
				wsprintf(sBuf, lMsg.Get(MSGGET(14, 107)),this->ZenDivorce);
				GCServerMsgStringSend (sBuf,lpObj->m_Index,0x01 ) ;
			}
		}else
		{			
			wsprintf(sBuf, lMsg.Get(MSGGET(14, 104)),lpObj->MarryCharacterInfo->MarriedName);
			GCServerMsgStringSend (sBuf,lpObj->m_Index,0x01 ) ;
		}
	}else
	{
		GCServerMsgStringSend (lMsg.Get(MSGGET(14, 108)),lpObj->m_Index,0x01 ) ;
	}
}

void MarrySystem::ForceDivorce(LPOBJ lpObj)
{
	char sBuf[255]={0};

	if(lpObj->MarryCharacterInfo->Married == 1)
	{
		int MeZen = (lpObj->Money - this->ZenForceDivorce);

		if(MeZen >= 0)
		{
			int WhoID = gObjGetIndex(lpObj->MarryCharacterInfo->MarriedName);
			
			if(WhoID >= 0)
			{
				wsprintf(sBuf, lMsg.Get(MSGGET(14, 109)),lpObj->MarryCharacterInfo->MarriedName);
				GCServerMsgStringSend (sBuf,lpObj->m_Index,0x01 ) ;
			}else
			{
				lpObj->Money = MeZen;
				CGZenSend(lpObj->m_Index);		

				JGSetPlayerMarryDivorseInfo(lpObj->MarryCharacterInfo->MarriedName);

				lpObj->PlayerExtraInfoModified = 1;
				lpObj->MarryCharacterInfo->Married = 0;
				lpObj->MarryCharacterInfo->MarrySeconds = 0;
				memset( lpObj->MarryCharacterInfo->MarryTarget, 0, MAX_ACCOUNT_LEN);
				memset( lpObj->MarryCharacterInfo->MarriedName, 0, MAX_ACCOUNT_LEN);
				this->SendInfo(lpObj->m_Index);
			}
		}else
		{
			wsprintf(sBuf, lMsg.Get(MSGGET(14, 110)),this->ZenForceDivorce);
			GCServerMsgStringSend (sBuf,lpObj->m_Index,0x01 ) ;
		}
	}else
	{
		GCServerMsgStringSend (lMsg.Get(MSGGET(14, 108)),lpObj->m_Index,0x01 ) ;
	}
}

void MarrySystem::Init(char * FilePath)
{
	this->EnableMarry			= GetPrivateProfileInt("Marry", "SCFMarryEnabled", 0, FilePath);
	this->MapPos				= GetPrivateProfileInt("Marry", "SCFMarryMap", 2, FilePath);

	this->rXPos1			 	= GetPrivateProfileInt("Marry", "SCFMarryX1", 12, FilePath);
	this->rYPos1	 			= GetPrivateProfileInt("Marry", "SCFMarryY1", 23, FilePath);
	this->rXPos2			 	= GetPrivateProfileInt("Marry", "SCFMarryX2", 28, FilePath);
	this->rYPos2			 	= GetPrivateProfileInt("Marry", "SCFMarryY2", 25, FilePath);

	this->ZenDivorce	 		= GetPrivateProfileInt("Marry", "SCFMarryZenDivorce", 2000000, FilePath);
	this->ZenForceDivorce		= GetPrivateProfileInt("Marry", "SCFMarryZenForceDivorce", 20000000, FilePath);
	this->Shop					= GetPrivateProfileInt("Marry", "SCFMarryIsShop379OnlyForMarried", 0, FilePath);

	
	char etiqueta[8] = {0};
    for(int n=0;n<MAX_MAP_NUMBER;n++) 
	{
		wsprintf(etiqueta,"Map%02d", n);
		this->Maps[n] = GetPrivateProfileInt("TraceMinLevel", etiqueta,0, FilePath) ;
	}

	LogAddTD("[SCF Marry] - %s file is Loaded",FilePath);
}

int MarrySystem::HusWifeOnline(LPOBJ lpObj)
{
	char sBuf[255]={0};

	if(lpObj->MarryCharacterInfo->Married == 1)
	{
		int WhoID = gObjGetIndex(lpObj->MarryCharacterInfo->MarriedName);
		
		if(WhoID >= 0)
		{
			return 2;
		}else
		{
			wsprintf(sBuf, lMsg.Get(MSGGET(14, 104)),lpObj->MarryCharacterInfo->MarriedName);
			GCServerMsgStringSend (sBuf,lpObj->m_Index,0x01 ) ;
			return 1;
		}
	}else
	{
		GCServerMsgStringSend (lMsg.Get(MSGGET(14, 108)),lpObj->m_Index,0x01 ) ;
		return 0;
	}
}

//BOOL MarrySystem::CommandCore(LPOBJ lpObj,char *Command)
//{
//	if(this->EnableMarry == 1)
//	{
//		if(strnicmp( Command,"/marry ",7) == 0 )
//		{
//			char Who[255]={0};
//			sscanf((LPCSTR)(Command+7),"%s", &Who);
//			this->Propose(lpObj,Who);
//			return 1;
//		}
//
//		else if(strnicmp( Command,"/getmarry",9) == 0 )
//		{
//			char sbuff[1024]={0};
//			if(lpObj->Married == 1)
//			{
//				wsprintf(sbuff,"You are married with %s",lpObj->MarriedName);
//			}else
//			{
//				wsprintf(sbuff,"You are not married!");
//			}
//			GCServerMsgStringSend (sbuff,lpObj->m_Index,0x01 ) ;
//		}
//
//		else if(strnicmp( Command,"/acceptmarry",12) == 0 )
//		{
//			this->Accept(lpObj);
//			return 1;
//		}
//
//		else if(strnicmp( Command,"/divorce",8) == 0 )
//		{
//			this->Divorce(lpObj);
//			return 1;
//		}
//
//		else if(strnicmp( Command,"/forcedivorce",13) == 0 )
//		{
//			this->ForceDivorce(lpObj);
//			return 1;
//		}
//
//		else if(strnicmp( Command,"/tracemarry",11) == 0 )
//		{
//			this->Trace(lpObj);
//			return 1;
//		}
//	}
//	return 0;
//}

void MarrySystem::CommandCore(LPOBJ lpObj,BYTE Command)
{
	if(this->EnableMarry == 1)
	{
	
		if(ReadConfig.CmdMarryAF > 0)
		{
			if(lpObj->MarryCharacterInfo->aFloodMarryCmd != 0)
			{
				GCServerMsgStringSend("You must wait before use marry command again", lpObj->m_Index, 1);
				return;	
			}
			lpObj->MarryCharacterInfo->aFloodMarryCmd = ReadConfig.CmdMarryAF;
		}

		switch(Command)
		{
			case 0:
			{
				char sbuff[1024]={0};
				if(lpObj->MarryCharacterInfo->Married == 1)
				{
					wsprintf(sbuff,lMsg.Get(MSGGET(14, 111)),lpObj->MarryCharacterInfo->MarriedName);
				}else
				{
					wsprintf(sbuff,lMsg.Get(MSGGET(14, 108)));
				}
				GCServerMsgStringSend (sbuff,lpObj->m_Index,0x01 ) ;
			}break;
			case 1:
			{
				this->Accept(lpObj);
			}break;
			case 2:
			{
				this->Divorce(lpObj);
			}break;
			case 3:
			{
				this->ForceDivorce(lpObj);
			}break;
			case 4:
			{
				this->Trace(lpObj);
			}break;
		}
	}
}

#endif