#include "stdafx.h"
#include "GameServer.h"
#include "LogProc.h"
#include "GameMain.h"
#include "..\include\readscript.h"
#include "..\common\winutil.h"
#include "SCFExDBProtocol.h"
#include "SCFExDB_Defines.h"
#include "QuestS5Info.h"
#include "NpcTalk.h"
#include "DSProtocol.h"
#include "GensPvP.h"
#include "LogToFile.h"


extern CLogToFile ANTI_HACK_LOG;

QuestS5 qs5;

struct PMSG_KILLPLAYER
{
	PBMSG_HEAD h;	// C1:16
	BYTE NumberH;	// 3
	BYTE NumberL;	// 4
	BYTE ExpH;	// 5
	BYTE ExpL;	// 6
	BYTE DamageH;	// 7
	BYTE DamageL;	// 8
};

void QuestS5::ProtocolCore(int aIndex,char * lpMsg)
{
	if ( gObjIsConnectedGP(aIndex) == FALSE )
	{
		LogAddC(2, "error-L3 [%s][%d]", __FILE__, __LINE__);
		return;
	}

	if (gObj[aIndex].PlayerQ5InfoReceived != 1)
	{
		//GCServerMsgStringSend ("ERROR: Please relogin!",aIndex,0x01 ) ;
		LogAddC(2, "ERROR: S5QUEST Info [%s][%s] Line: %d - RE-REQUESTING INFO", gObj[aIndex].AccountID, gObj[aIndex].Name, __LINE__);
		GetPlayerExtraInfo(aIndex,SCFExDB_GSSend_GetPlayerQ5Info);
		return;
	}

	//for(int n=0;n<MAX_QUEST_COUNT;n++)
	//{
	//	gObj[aIndex].S5Quest[n].Active = 0;
	//	gObj[aIndex].S5Quest[n].NumID = 0;
	//	for(int i=0;i<MAX_QUEST_REQ;i++)
	//		gObj[aIndex].S5Quest[n].State[i] = 0;
	//}

	//gObj[aIndex].PlayerQ5InfoModified = 1;

	if(ReadConfig.S5E2 == TRUE)
	{
		PMSG_QUESTS5ACCEPTRECVs * aRecv = (PMSG_QUESTS5ACCEPTRECVs *)(lpMsg);
		switch(aRecv->subcode)
		{
			case 0x0A://Elf Soldier Accept Quest
			{
				aRecv->subcode = 0x0B;
				int QuestID = this->GetQuestID(aRecv->Num,aRecv->Quest);
				if(QuestID != -1)
				{
					if(gObj[aIndex].S5Quest[QuestID].Active == 1)
					{
						int RewID = this->GetQuestReqID(QuestID,aRecv->Num);
						if(RewID != -1)
							this->SendDialogReq(aIndex,0x1B,QuestID,RewID,aRecv->Quest);
						DataSend(gObj[aIndex].m_Index ,(UCHAR*)aRecv, aRecv->h.size);
						this->SendTWindow(aIndex);
					}else
					{
						aRecv->Num = this->quest[QuestID].Num;
						DataSend(gObj[aIndex].m_Index ,(UCHAR*)aRecv, aRecv->h.size);
					}
				}
			}break;
			case 0x0B://Accept Quest
			{					
				if(aRecv->Unk == 1)
				{	
					int NextStatus = this->CheckNextStatus(aIndex,aRecv->Num,aRecv->Quest);
					if(NextStatus != -1)
					{
						WORD QuestID = this->GetQuestID(aRecv->Num,aRecv->Quest);
						WORD QNum = this->quest[QuestID].Req[NextStatus].Num;
						this->AddQuest(aIndex,QNum,aRecv->Quest);
						this->SendTWindow(aIndex);
						gObj[aIndex].PlayerQ5InfoModified = 1;
					}
				}else
				{					
					BYTE cBUFFER[0xF1]={0}; 
					memcpy(&cBUFFER[0],aRecv,9);
					cBUFFER[1] = 0xF1;
					cBUFFER[3] = 0x0C;
					cBUFFER[4] -= 1;
					cBUFFER[8] = 0x01;
					DataSend(aIndex, cBUFFER,cBUFFER[1]);
				}
			}break;
			case 0x0D: //Send Button OK on Quest Finish
			{
				int QuestID = this->GetQuestID(aRecv->Num,aRecv->Quest);
				if(QuestID != -1)
				{
					this->GiveReward(aIndex,QuestID);
					gObj[aIndex].PlayerQ5InfoModified = 1;
				}else
				{					
					PMSG_QUESTS5ACCEPTRECVs pResult = {0};
					PHeadSubSetB((LPBYTE)&pResult,0xF6,0x0D,9);
					pResult.Num = aRecv->Num;
					pResult.Quest = aRecv->Quest;
					pResult.Unk = 1;
					DataSend(aIndex ,(UCHAR*)&pResult, pResult.h.size);
				}
			}break;
			case 0x0F: //CANCEL QUEST
			{
				int QuestID = this->GetQuestID(aRecv->Num,aRecv->Quest);
				if(QuestID != -1)
				{
					this->DeletePlayerQuest(aIndex,QuestID,false);
					this->SendTWindow(aIndex);
					gObj[aIndex].PlayerQ5InfoModified = 1;
				}
			}break;
			case 0x1B:
			{
				int QuestID = this->GetQuestID(aRecv->Num,aRecv->Quest);
				if(QuestID != -1)
				{
					int RewID = this->GetQuestReqID(QuestID,aRecv->Num);
					if(RewID != -1)
						this->SendDialogReq(aIndex,0x1B,QuestID,RewID,aRecv->Quest);
				}
			}break;
			case 0x30://Elf Soldier
			{
				this->SendNPCQuest(aIndex);
			}break;
			case 0x31://Elf Soldier SendBuff
			{
				NpcShadowPhantom(&gObj[aIndex]);
			}break;
		}
	}
}

BOOL QuestS5::PlayerCheckSkill(int aIndex,int Skill)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return false;
	}

	LPOBJ lpObj = &gObj[aIndex];
	for(int n = 0; n < MAGIC_SIZE; n++)
	{
		if(lpObj->Magic[n].IsMagic() == 1)
		{
			if(lpObj->Magic[n].m_Skill == Skill)
			{
				return TRUE;
			}
		}
	}
	return FALSE;
}


int QuestS5::PlayerCheckItemCount(int aIndex,int QuestID, int ReqID,bool Delete)
{
	int i = ReqID;
	if(Delete == false)
	{
		return gObjCheckItemsCount(aIndex,ITEMGET(this->quest[QuestID].Req[i].Monster,this->quest[QuestID].Req[i].ItemIndex),this->quest[QuestID].Req[i].iLevel,this->quest[QuestID].Req[i].iOpt/4,this->quest[QuestID].Req[i].iLuck,this->quest[QuestID].Req[i].iSkill,this->quest[QuestID].Req[i].iExc,this->quest[QuestID].Req[i].iAnc);
	}else
	{
		return gObjDeleteItemsCount(aIndex,ITEMGET(this->quest[QuestID].Req[i].Monster,this->quest[QuestID].Req[i].ItemIndex),this->quest[QuestID].Req[i].iLevel,this->quest[QuestID].Req[i].iOpt/4,this->quest[QuestID].Req[i].iLuck,this->quest[QuestID].Req[i].iSkill,this->quest[QuestID].Req[i].iExc,this->quest[QuestID].Req[i].iAnc,this->quest[QuestID].Req[i].KillCount);
	}
}

void QuestS5::KillPlayerGensEnemy(int aIndex)
{
	if ( gObj[aIndex].Type != OBJ_USER )
	{
#if (PACK_EDITION>=3)
		if (gObj[aIndex].IsBot != 1)
		{
#endif
			LogAddC(2, "error-L3 [%s][%d]", __FILE__, __LINE__);
			return;
#if (PACK_EDITION>=3)
		}else
		{
			aIndex = gObj[aIndex].BotNumOwner;
		}
#endif
	}
	
	if ( gObjIsConnectedGP(aIndex) == FALSE )
	{
		LogAddC(2, "error-L3 [%s][%d]", __FILE__, __LINE__);
		return;
	}

	if (gObj[aIndex].PlayerQ5InfoReceived != 1)
	{
		//GCServerMsgStringSend ("ERROR: Please relogin!",aIndex,0x01 ) ;
		LogAddC(2, "ERROR: S5QUEST Info [%s][%s] Line: %d - RE-REQUESTING INFO", gObj[aIndex].AccountID, gObj[aIndex].Name, __LINE__);
		GetPlayerExtraInfo(aIndex,SCFExDB_GSSend_GetPlayerQ5Info);

		return;
	}

	for(int i=0;i<MAX_QUEST_COUNT;i++)
	{
		if(gObj[aIndex].S5Quest[i].Active == 1)
		{
			WORD Number = this->quest[i].Req[gObj[aIndex].S5Quest[i].NumID].Num;
			for(int x=0;x<this->quest[i].ReqCount;x++)
			{
				if(this->quest[i].Req[x].Type == 262 && Number == this->quest[i].Req[x].Num)
				{
					if(gObj[aIndex].S5Quest[i].State[x] < this->quest[i].Req[x].KillCount)
					{
						gObj[aIndex].S5Quest[i].State[x]++;
						gObj[aIndex].PlayerQ5InfoModified = 1;
					}
				}
			}
		}
	}
}

void QuestS5::KillPlayerInChaosCastle(int aIndex, BYTE eLevel)
{
	if ( gObj[aIndex].Type != OBJ_USER )
	{
#if (PACK_EDITION>=3)
		if (gObj[aIndex].IsBot != 1)
		{
#endif
			LogAddC(2, "error-L3 [%s][%d]", __FILE__, __LINE__);
			return;
#if (PACK_EDITION>=3)
		}else
		{
			aIndex = gObj[aIndex].BotNumOwner;
		}
#endif
	}
	
	if ( gObjIsConnectedGP(aIndex) == FALSE )
	{
		LogAddC(2, "error-L3 [%s][%d]", __FILE__, __LINE__);
		return;
	}

	if (gObj[aIndex].PlayerQ5InfoReceived != 1)
	{
		//GCServerMsgStringSend ("ERROR: Please relogin!",aIndex,0x01 ) ;
		LogAddC(2, "ERROR: S5QUEST Info [%s][%s] Line: %d - RE-REQUESTING INFO", gObj[aIndex].AccountID, gObj[aIndex].Name, __LINE__);
		GetPlayerExtraInfo(aIndex,SCFExDB_GSSend_GetPlayerQ5Info);
		return;
	}

	for(int i=0;i<MAX_QUEST_COUNT;i++)
	{
		if(gObj[aIndex].S5Quest[i].Active == 1)
		{
			WORD Number = this->quest[i].Req[gObj[aIndex].S5Quest[i].NumID].Num;
			for(int x=0;x<this->quest[i].ReqCount;x++)
			{
				if(this->quest[i].Req[x].Type == 64 && Number == this->quest[i].Req[x].Num)
				{
					if(this->quest[i].Req[x].Monster == eLevel) 
					{
						if(gObj[aIndex].S5Quest[i].State[x] < this->quest[i].Req[x].KillCount)
						{
							gObj[aIndex].S5Quest[i].State[x]++;
							gObj[aIndex].PlayerQ5InfoModified = 1;
						}
					}
				}
			}
		}
	}
}

void QuestS5::FinishEvent(int aIndex, BYTE Event, BYTE eLevel)
{
	//1 DS
	//2 CC
	//3 BC
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	if ( gObj[aIndex].Type != OBJ_USER )
	{
#if (PACK_EDITION>=3)
		if (gObj[aIndex].IsBot != 1)
		{
#endif
			LogAddC(2, "error-L3 [%s][%d]", __FILE__, __LINE__);
			return;
#if (PACK_EDITION>=3)
		}else
		{
			aIndex = gObj[aIndex].BotNumOwner;
		}
#endif
	}
	
	if ( gObjIsConnectedGP(aIndex) == FALSE )
	{
		LogAddC(2, "error-L3 [%s][%d]", __FILE__, __LINE__);
		return;
	}

	if (gObj[aIndex].PlayerQ5InfoReceived != 1)
	{
		//GCServerMsgStringSend ("ERROR: Please relogin!",aIndex,0x01 ) ;
		LogAddC(2, "ERROR: S5QUEST Info [%s][%s] Line: %d - RE-REQUESTING INFO", gObj[aIndex].AccountID, gObj[aIndex].Name, __LINE__);
		GetPlayerExtraInfo(aIndex,SCFExDB_GSSend_GetPlayerQ5Info);
		return;
	}

	for(int i=0;i<MAX_QUEST_COUNT;i++)
	{
		if(gObj[aIndex].S5Quest[i].Active == 1)
		{
			WORD Number = this->quest[i].Req[gObj[aIndex].S5Quest[i].NumID].Num;
			for(int x=0;x<this->quest[i].ReqCount;x++)
			{
				if(this->quest[i].Req[x].Type == 258 && Number == this->quest[i].Req[x].Num)
				{
					if(Event == 1 && this->quest[i].Req[x].Monster == eLevel )
					{
						gObj[aIndex].S5Quest[i].State[x] = 1;
						gObj[aIndex].PlayerQ5InfoModified = 1;
					}
				}
				else if(this->quest[i].Req[x].Type == 257 && Number == this->quest[i].Req[x].Num)
				{
					if(Event == 2 && this->quest[i].Req[x].Monster == eLevel)
					{
						gObj[aIndex].S5Quest[i].State[x] = 1;
						gObj[aIndex].PlayerQ5InfoModified = 1;
					}
				}
				else if(this->quest[i].Req[x].Type == 256 && Number == this->quest[i].Req[x].Num)
				{
					if(Event == 3 && this->quest[i].Req[x].Monster == eLevel)
					{
						gObj[aIndex].S5Quest[i].State[x] = 1;
						gObj[aIndex].PlayerQ5InfoModified = 1;
					}
				}
			}
		}
	}
}

void QuestS5::MonsterDie(int aIndex,int MonsterIndex)
{
	if ( gObj[aIndex].Type != OBJ_USER )
	{
#if (PACK_EDITION>=3)
		if (gObj[aIndex].IsBot != 1)
		{
#endif
			if(gObj[MonsterIndex].m_RecallMon != -1)
			{
				return;
			}
			LogAddC(2, "error-L3 [%s][%d]", __FILE__, __LINE__);
			return;
#if (PACK_EDITION>=3)
		}else
		{
			aIndex = gObj[aIndex].BotNumOwner;
		}
#endif
	}
	
	if ( gObjIsConnectedGP(aIndex) == FALSE )
	{
		LogAddC(2, "error-L3 [%s][%d]", __FILE__, __LINE__);
		return;
	}

	if (gObj[aIndex].PlayerQ5InfoReceived != 1)
	{
		//GCServerMsgStringSend ("ERROR: Please relogin!",aIndex,0x01 ) ;
		LogAddC(2, "ERROR: S5QUEST Info [%s][%s] Line: %d - RE-REQUESTING INFO", gObj[aIndex].AccountID, gObj[aIndex].Name, __LINE__);
		GetPlayerExtraInfo(aIndex,SCFExDB_GSSend_GetPlayerQ5Info);
		return;
	}

#if (PACK_EDITION>=3)
	if (gObj[MonsterIndex].IsBot >= 1)
		return;
#endif

	for(int i=0;i<MAX_QUEST_COUNT;i++)
	{
		if(gObj[aIndex].S5Quest[i].Active == 1)
		{
			WORD Number = this->quest[i].Req[gObj[aIndex].S5Quest[i].NumID].Num;
			for(int x=0;x<this->quest[i].ReqCount;x++)
			{
				if(this->quest[i].Req[x].Type == 1 && Number == this->quest[i].Req[x].Num)
				{
					if(gObj[MonsterIndex].Class == this->quest[i].Req[x].Monster)
					{
						if(gObj[aIndex].S5Quest[i].State[x] < this->quest[i].Req[x].KillCount)
						{
							gObj[aIndex].S5Quest[i].State[x]++;
							gObj[aIndex].PlayerQ5InfoModified = 1;
						}
					}
				}
				else if(this->quest[i].Req[x].Type == 66 && Number == this->quest[i].Req[x].Num)
				{
					if(gObj[MonsterIndex].Class == 131)
					{
						if((this->quest[i].Req[x].Monster == 1 && gObj[MonsterIndex].MapNumber == MAP_INDEX_BLOODCASTLE1) 
							|| (this->quest[i].Req[x].Monster == 2 && gObj[MonsterIndex].MapNumber == MAP_INDEX_BLOODCASTLE2) 
							|| (this->quest[i].Req[x].Monster == 3 && gObj[MonsterIndex].MapNumber == MAP_INDEX_BLOODCASTLE3) 
							|| (this->quest[i].Req[x].Monster == 4 && gObj[MonsterIndex].MapNumber == MAP_INDEX_BLOODCASTLE4) 
							|| (this->quest[i].Req[x].Monster == 5 && gObj[MonsterIndex].MapNumber == MAP_INDEX_BLOODCASTLE5) 
							|| (this->quest[i].Req[x].Monster == 6 && gObj[MonsterIndex].MapNumber == MAP_INDEX_BLOODCASTLE6) 
							|| (this->quest[i].Req[x].Monster == 7 && gObj[MonsterIndex].MapNumber == MAP_INDEX_BLOODCASTLE7) 
							|| (this->quest[i].Req[x].Monster == 8 && gObj[MonsterIndex].MapNumber == MAP_INDEX_BLOODCASTLE8) 
							)
						{
							if(gObj[aIndex].S5Quest[i].State[x] < this->quest[i].Req[x].KillCount)
							{
								gObj[aIndex].S5Quest[i].State[x]++;
								gObj[aIndex].PlayerQ5InfoModified = 1;
							}
						}
					}
				}
				else if(this->quest[i].Req[x].Type == 65 && Number == this->quest[i].Req[x].Num)
				{
					if((this->quest[i].Req[x].Monster == 1 && gObj[MonsterIndex].MapNumber == MAP_INDEX_CHAOSCASTLE1) 
					|| (this->quest[i].Req[x].Monster == 2 && gObj[MonsterIndex].MapNumber == MAP_INDEX_CHAOSCASTLE2) 
					|| (this->quest[i].Req[x].Monster == 3 && gObj[MonsterIndex].MapNumber == MAP_INDEX_CHAOSCASTLE3) 
					|| (this->quest[i].Req[x].Monster == 4 && gObj[MonsterIndex].MapNumber == MAP_INDEX_CHAOSCASTLE4) 
					|| (this->quest[i].Req[x].Monster == 5 && gObj[MonsterIndex].MapNumber == MAP_INDEX_CHAOSCASTLE5) 
					|| (this->quest[i].Req[x].Monster == 6 && gObj[MonsterIndex].MapNumber == MAP_INDEX_CHAOSCASTLE6) 
					|| (this->quest[i].Req[x].Monster == 7 && gObj[MonsterIndex].MapNumber == MAP_INDEX_CHAOSCASTLE7) 
						)
					{
						if(gObj[aIndex].S5Quest[i].State[x] < this->quest[i].Req[x].KillCount)
						{
							gObj[aIndex].S5Quest[i].State[x]++;
							gObj[aIndex].PlayerQ5InfoModified = 1;
						}
					}
				}
			}
		}
	}
}

void QuestS5::SendDialogReq(int aIndex,BYTE SubCode,int QuestID,int NextStatus,WORD Quest)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	WORD Number = this->quest[QuestID].Req[NextStatus].Num;

	int MonsterKilled = 0;
	//if(gObj[aIndex].S5Quest[QuestID].Active == 1)
	//{
	//	MonsterKilled = gObj[aIndex].S5Quest[QuestID].State;
	//}

	BYTE cBUFFER[512]={0}; 
	PMSG_QUESTS5REQSEND pResult = {0};
	QUESTS5REQINFO reqAddS5[5] = {0};
	QUESTS6REQINFO reqAddS6[5] = {0};

	QUESTS5REWINFO rewAdd[5] = {0};
	pResult.Num = MAKE_NUMBERW(HIBYTE(Number),LOBYTE(Number));
	pResult.Quest = MAKE_NUMBERW(HIBYTE(Quest),LOBYTE(Quest));

	//Requeriments
	for(int i=0;i<this->quest[QuestID].ReqCount;i++)
	{
		if((Number == this->quest[QuestID].Req[i].Num) && (Quest == this->quest[QuestID].QuestIndex))
		{
			WORD ReqType = this->quest[QuestID].Req[i].Type;
			if(ReadConfig.S6E1 == 0)
				reqAddS5[pResult.ReqCount].Type = ReqType;
			else
				reqAddS6[pResult.ReqCount].Type = MAKE_NUMBERW(HIBYTE(ReqType),LOBYTE(ReqType)); //ReqType;

			WORD Monster = this->quest[QuestID].Req[i].Monster;
			WORD ToKill = this->quest[QuestID].Req[i].KillCount;
			WORD Tmp1 = HIWORD(ToKill);
			WORD Tmp2 = LOWORD(ToKill);
			switch(ReqType)
			{
				case 64://CC Players
				case 65://CC GuardsMan
				case 66://BC Gate
				case 262: //Kill Gens Other Family
				case 1: //KillMob
				{					
					if(ReadConfig.S6E1 == 0)
					{
						MonsterKilled = gObj[aIndex].S5Quest[QuestID].State[i];
						reqAddS5[pResult.ReqCount].ToKill[0] = LOBYTE(Tmp2);
						reqAddS5[pResult.ReqCount].ToKill[1] = HIBYTE(Tmp2);
						reqAddS5[pResult.ReqCount].ToKill[2] = LOBYTE(Tmp1);
						reqAddS5[pResult.ReqCount].ToKill[3] = HIBYTE(Tmp1);
						reqAddS5[pResult.ReqCount].MobNum = MAKE_NUMBERW(HIBYTE(Monster),LOBYTE(Monster));
						reqAddS5[pResult.ReqCount].Killed = MAKE_NUMBERW(HIBYTE(MonsterKilled),LOBYTE(MonsterKilled));
					}else
					{
						MonsterKilled = gObj[aIndex].S5Quest[QuestID].State[i];
						reqAddS6[pResult.ReqCount].ToKill[0] = LOBYTE(Tmp2);
						reqAddS6[pResult.ReqCount].ToKill[1] = HIBYTE(Tmp2);
						reqAddS6[pResult.ReqCount].ToKill[2] = LOBYTE(Tmp1);
						reqAddS6[pResult.ReqCount].ToKill[3] = HIBYTE(Tmp1);
						reqAddS6[pResult.ReqCount].MobNum = MAKE_NUMBERW(HIBYTE(Monster),LOBYTE(Monster));
						reqAddS6[pResult.ReqCount].Killed = MAKE_NUMBERW(HIBYTE(MonsterKilled),LOBYTE(MonsterKilled));
					}
				}break;
				case 256://DS Cleared
				case 257://CC Cleared
				case 258://BC Cleared
				{
					reqAddS6[pResult.ReqCount].MobNum = MAKE_NUMBERW(HIBYTE(Monster),LOBYTE(Monster));
					reqAddS6[pResult.ReqCount].Killed = gObj[aIndex].S5Quest[QuestID].State[i]; //Doesnt Have it // 1 Have it
				}break;
				case 2: //Skill
				{			
					if(ReadConfig.S6E1 == 0)
					{
						reqAddS5[pResult.ReqCount].MobNum = MAKE_NUMBERW(HIBYTE(Monster),LOBYTE(Monster));
						reqAddS5[pResult.ReqCount].Killed = this->PlayerCheckSkill(aIndex,Monster); //Doesnt Have it // 1 Have it
					}else
					{
						reqAddS6[pResult.ReqCount].MobNum = MAKE_NUMBERW(HIBYTE(Monster),LOBYTE(Monster));
						reqAddS6[pResult.ReqCount].Killed = this->PlayerCheckSkill(aIndex,Monster); //Doesnt Have it // 1 Have it
					}
				}break;
				case 4: //Item
				{			
					if(ReadConfig.S6E1 == 0)
					{
						MonsterKilled = this->PlayerCheckItemCount(aIndex,QuestID,i,false);
						reqAddS5[pResult.ReqCount].MobNum = ITEMGET(this->quest[QuestID].Req[i].Monster,this->quest[QuestID].Req[i].ItemIndex);
						reqAddS5[pResult.ReqCount].ItemType = this->quest[QuestID].Req[i].Monster * 16;
						reqAddS5[pResult.ReqCount].Killed = MAKE_NUMBERW(HIBYTE(MonsterKilled),LOBYTE(MonsterKilled));
						reqAddS5[pResult.ReqCount].ItemIndex = this->quest[QuestID].Req[i].ItemIndex;					
						reqAddS5[pResult.ReqCount].ToKill[0] = LOBYTE(Tmp2);
						reqAddS5[pResult.ReqCount].ToKill[1] = HIBYTE(Tmp2);
						reqAddS5[pResult.ReqCount].ToKill[2] = LOBYTE(Tmp1);
						reqAddS5[pResult.ReqCount].ToKill[3] = HIBYTE(Tmp1);
						reqAddS5[pResult.ReqCount].iExcellent = this->quest[QuestID].Req[i].iExc;
						reqAddS5[pResult.ReqCount].iAncient = this->quest[QuestID].Req[i].iAnc;
						reqAddS5[pResult.ReqCount].iDurability = this->quest[QuestID].Req[i].iDur;
						reqAddS5[pResult.ReqCount].iOpts = ( (this->quest[QuestID].Req[i].iOpt/4) + (this->quest[QuestID].Req[i].iLuck*4) + (this->quest[QuestID].Req[i].iLevel*8) + (this->quest[QuestID].Req[i].iSkill*128) );
					}else
					{
						MonsterKilled = this->PlayerCheckItemCount(aIndex,QuestID,i,false);
						reqAddS6[pResult.ReqCount].MobNum = ITEMGET(this->quest[QuestID].Req[i].Monster,this->quest[QuestID].Req[i].ItemIndex);
						reqAddS6[pResult.ReqCount].ItemType = this->quest[QuestID].Req[i].Monster * 16;
						reqAddS6[pResult.ReqCount].Killed = MAKE_NUMBERW(HIBYTE(MonsterKilled),LOBYTE(MonsterKilled));
						reqAddS6[pResult.ReqCount].ItemIndex = this->quest[QuestID].Req[i].ItemIndex;					
						reqAddS6[pResult.ReqCount].ToKill[0] = LOBYTE(Tmp2);
						reqAddS6[pResult.ReqCount].ToKill[1] = HIBYTE(Tmp2);
						reqAddS6[pResult.ReqCount].ToKill[2] = LOBYTE(Tmp1);
						reqAddS6[pResult.ReqCount].ToKill[3] = HIBYTE(Tmp1);
						reqAddS6[pResult.ReqCount].iExcellent = this->quest[QuestID].Req[i].iExc;
						reqAddS6[pResult.ReqCount].iAncient = this->quest[QuestID].Req[i].iAnc;
						reqAddS6[pResult.ReqCount].iDurability = this->quest[QuestID].Req[i].iDur;
						reqAddS6[pResult.ReqCount].iOpts = ( (this->quest[QuestID].Req[i].iOpt/4) + (this->quest[QuestID].Req[i].iLuck*4) + (this->quest[QuestID].Req[i].iLevel*8) + (this->quest[QuestID].Req[i].iSkill*128) );
					}
				}break;
				case 263:
				{
					if(Monster == 567 && gObj[aIndex].m_IfState.type == 103)
					{
						reqAddS6[pResult.ReqCount].Killed = 1;
					}else if(Monster == 568 && gObj[aIndex].m_IfState.type == 104)
					{
						reqAddS6[pResult.ReqCount].Killed = 1;
					}else if(Monster == 566 && gObj[aIndex].m_IfState.type == 105)
					{
						reqAddS6[pResult.ReqCount].Killed = 1;
					}else if(Monster == 543 && gObj[aIndex].m_IfState.type == 100)
					{
						reqAddS6[pResult.ReqCount].Killed = 1;
					}else if(Monster == 544 && gObj[aIndex].m_IfState.type == 101)
					{
						reqAddS6[pResult.ReqCount].Killed = 1;
					}
				}break;
				case 261://zen
				{
					DWORD Zen = this->quest[QuestID].Req[i].Monster;
					WORD Tmp1 = HIWORD(Zen);
					WORD Tmp2 = LOWORD(Zen);
					reqAddS6[pResult.ReqCount].ToKill[0] = LOBYTE(Tmp2);
					reqAddS6[pResult.ReqCount].ToKill[1] = HIBYTE(Tmp2);
					reqAddS6[pResult.ReqCount].ToKill[2] = LOBYTE(Tmp1);
					reqAddS6[pResult.ReqCount].ToKill[3] = HIBYTE(Tmp1);
					if(gObj[aIndex].Money >= Zen)
					{
						reqAddS6[pResult.ReqCount].Killed = Tmp1;
						reqAddS6[pResult.ReqCount].Unk6 = Tmp2;
						//reqAddS6[pResult.ReqCount].Unk6 = 1;
					}
				}break;
			}
			pResult.ReqCount++;
		}
	}
	//Reward
	for(int i=0;i<this->quest[QuestID].RewCount;i++)
	{
		if((Number == this->quest[QuestID].Rew[i].Num) && (Quest == this->quest[QuestID].QuestIndex))
		{
			BYTE RewardType = this->quest[QuestID].Rew[i].Type;
			rewAdd[pResult.RewCount].Type = RewardType;
			switch(RewardType)
			{
				case 1: //Exp
				case 2: //Zen
				case 16: //Contribution
				{
					DWORD RewardAmmount = this->quest[QuestID].Rew[i].iType;
					WORD Tmp1 = HIWORD(RewardAmmount);
					WORD Tmp2 = LOWORD(RewardAmmount);
					rewAdd[pResult.RewCount].Ammount[0] = LOBYTE(Tmp2);
					rewAdd[pResult.RewCount].Ammount[1] = HIBYTE(Tmp2);
					rewAdd[pResult.RewCount].Ammount[2] = LOBYTE(Tmp1);
					rewAdd[pResult.RewCount].Ammount[3] = HIBYTE(Tmp1);
				}break;
				case 36:
				case 4: //Item
				{
					rewAdd[pResult.RewCount].ItemType = this->quest[QuestID].Rew[i].iType * 16;
					rewAdd[pResult.RewCount].Ammount[0] = this->quest[QuestID].Rew[i].iCount;
					rewAdd[pResult.RewCount].ItemIndex = this->quest[QuestID].Rew[i].iIndex;
					rewAdd[pResult.RewCount].ItemID = ITEMGET(this->quest[QuestID].Rew[i].iType,this->quest[QuestID].Rew[i].iIndex);
					
					rewAdd[pResult.RewCount].iExcellent = this->quest[QuestID].Rew[i].iExc;
					rewAdd[pResult.RewCount].iAncient = this->quest[QuestID].Rew[i].iAnc;
					rewAdd[pResult.RewCount].iDurability = this->quest[QuestID].Rew[i].iDur;
					rewAdd[pResult.RewCount].iOpts = ( (this->quest[QuestID].Rew[i].iOpt/4) + (this->quest[QuestID].Rew[i].iLuck*4) + (this->quest[QuestID].Rew[i].iLevel*8) + (this->quest[QuestID].Rew[i].iSkill*128) );
				}break;
			}
			pResult.RewCount++;
		}
	}		

	if(ReadConfig.S6E1 == 0)
	{
		PHeadSubSetB((LPBYTE)&pResult,0xF6,SubCode,0xF1);
	}else
	{
		PHeadSubSetB((LPBYTE)&pResult,0xF6,SubCode,0xF5);
	}
	memcpy(&cBUFFER[0],(LPBYTE)&pResult,sizeof(PMSG_QUESTS5REQSEND));
	if(ReadConfig.S6E1 == 0)
	{
		memcpy(&cBUFFER[sizeof(PMSG_QUESTS5REQSEND)-1],(LPBYTE)&reqAddS5,sizeof(reqAddS5));
		memcpy(&cBUFFER[sizeof(reqAddS5)+sizeof(PMSG_QUESTS5REQSEND)-1],(LPBYTE)&rewAdd,sizeof(rewAdd));
	}else
	{
		memcpy(&cBUFFER[sizeof(PMSG_QUESTS5REQSEND)-1],(LPBYTE)&reqAddS6,sizeof(reqAddS6));
		memcpy(&cBUFFER[sizeof(reqAddS6)+sizeof(PMSG_QUESTS5REQSEND)-1],(LPBYTE)&rewAdd,sizeof(rewAdd));
	}
	DataSend(aIndex, cBUFFER, pResult.h.size);
}

void QuestS5::AddQuest(int aIndex,WORD Number,WORD Quest)
{
	if ( gObjIsConnectedGP(aIndex) == FALSE )
	{
		LogAddC(2, "error-L3 [%s][%d]", __FILE__, __LINE__);
		return;
	}

	int QuestID = this->GetQuestID(Number,Quest);
	if(QuestID != -1)
	{
		LogAddTD("[QuestS5] AcceptQuest [%s][%s] (%s)", gObj[aIndex].AccountID, gObj[aIndex].Name,
			this->quest[QuestID].Name);
		if(Quest == 20 && Number == 59)
			Number++;
		int NumID = GetQuestReqID(QuestID,Number);
		gObj[aIndex].S5Quest[QuestID].Active = 1;
		gObj[aIndex].S5Quest[QuestID].NumID = NumID;
		for(int i=0;i<MAX_QUEST_REQ;i++)
			gObj[aIndex].S5Quest[QuestID].State[i] = 0;
	}
}

int QuestS5::QuestRunning(int aIndex,WORD Number,WORD Quest)
{
	if ( gObjIsConnectedGP(aIndex) == FALSE )
	{
		LogAddC(2, "error-L3 [%s][%d]", __FILE__, __LINE__);
		return -1;
	}

	if(Quest == 20)
	{
		if(Number != 0)
		{
			if(Number == 5)
			{
				int q1 = this->GetQuestID(5,20);
				if(gObj[aIndex].S5Quest[q1].Active == 2)
				{
					int Day = gObj[aIndex].S5Quest[q1].NumID;
					SYSTEMTIME sysTime;
					GetLocalTime(&sysTime);
					WORD Day2 = sysTime.wDayOfWeek;//1 Monday
					if((Day >= 0 && Day <= 6) && (Day2 != Day))
					{
						this->DeletePlayerDayofWeekQuest(aIndex, Day);
						this->DelExactQuest(aIndex,5,20);
					}
				}
				int q = this->GetQuestID(0,20);
				if(q != -1)
				{
					if(gObj[aIndex].S5Quest[q].Active == 0)
						return 2;
				}
			}else
			{
				int q = this->GetQuestID(5,20);
				if(q != -1)
				{
					if(gObj[aIndex].S5Quest[q].Active == 0)
						return 2;
				}	
				if(	Number == 9 || 
					Number == 16 || 
					Number == 23 || 
					Number == 30 || 
					Number == 37 || 
					Number == 44 ||
					Number == 51
					)
				{
					int x = this->GetQuestID(5,20);
					if(gObj[aIndex].S5Quest[x].State[0] == 0)
						return 2;
				}

				if(Number != 59 && 
					Number != 9 && 
					Number != 16 && 
					Number != 23 && 
					Number != 30 && 
					Number != 37 && 
					Number != 44 &&
					Number != 51
					)
				{

					SYSTEMTIME sysTime;
					GetLocalTime(&sysTime);
					WORD Day = sysTime.wDayOfWeek;//1 Monday

					switch(Day)
					{
						case 1:
						{
							if(Number != 10 && Number != 13)
								return 2;
							else
							{
								if(Number == 13)
								{
									int q = this->GetQuestID(10,20);
									if(q != -1)
									{
										if(gObj[aIndex].S5Quest[q].Active != 2)
											return 2;
									}
								}else
								{
									int q = this->GetQuestID((Number-1),20);
									if(q != -1)
									{
										if(gObj[aIndex].S5Quest[q].Active != 2)
											return 2;
									}
								}
							}
						}break;
						case 2:
						{
							if(Number != 17 && Number != 20)
								return 2;
							else
							{
								if(Number == 20)
								{
									int q = this->GetQuestID(17,20);
									if(q != -1)
									{
										if(gObj[aIndex].S5Quest[q].Active != 2)
											return 2;
									}
								}else
								{
									int q = this->GetQuestID((Number-1),20);
									if(q != -1)
									{
										if(gObj[aIndex].S5Quest[q].Active != 2)
											return 2;
									}
								}
							}
						}break;
						case 3:
						{
							if(Number != 24 && Number != 27)
								return 2;
							else
							{
								if(Number == 27)
								{
									int q = this->GetQuestID(24,20);
									if(q != -1)
									{
										if(gObj[aIndex].S5Quest[q].Active != 2)
											return 2;
									}
								}else
								{
									int q = this->GetQuestID((Number-1),20);
									if(q != -1)
									{
										if(gObj[aIndex].S5Quest[q].Active != 2)
											return 2;
									}
								}
							}
						}break;
						case 4:
						{
							if(Number != 31 && Number != 34)
								return 2;
							else
							{
								if(Number == 34)
								{
									int q = this->GetQuestID(31,20);
									if(q != -1)
									{
										if(gObj[aIndex].S5Quest[q].Active != 2)
											return 2;
									}
								}else
								{
									int q = this->GetQuestID((Number-1),20);
									if(q != -1)
									{
										if(gObj[aIndex].S5Quest[q].Active != 2)
											return 2;
									}
								}
							}
						}break;
						case 5:
						{
							if(Number != 38 && Number != 41)
								return 2;
							else
							{
								if(Number == 41)
								{
									int q = this->GetQuestID(38,20);
									if(q != -1)
									{
										if(gObj[aIndex].S5Quest[q].Active != 2)
											return 2;
									}
								}else
								{
									int q = this->GetQuestID((Number-1),20);
									if(q != -1)
									{
										if(gObj[aIndex].S5Quest[q].Active != 2)
											return 2;
									}
								}
							}
						}break;
						case 6:
						{
							if(Number != 45 && Number != 48)
								return 2;
							else
							{
								if(Number == 48)
								{
									int q = this->GetQuestID(45,20);
									if(q != -1)
									{
										if(gObj[aIndex].S5Quest[q].Active != 2)
											return 2;
									}
								}else
								{
									int q = this->GetQuestID((Number-1),20);
									if(q != -1)
									{
										if(gObj[aIndex].S5Quest[q].Active != 2)
											return 2;
									}
								}
							}
						}break;
						case 0:
						{
							if(Number != 52 && Number != 56)
								return 2;
							else
							{
								if(Number == 56)
								{
									int q = this->GetQuestID(52,20);
									if(q != -1)
									{
										if(gObj[aIndex].S5Quest[q].Active != 2)
											return 2;
									}
								}else
								{
									int q = this->GetQuestID((Number-1),20);
									if(q != -1)
									{
										if(gObj[aIndex].S5Quest[q].Active != 2)
											return 2;
									}
								}
							}
						}break;
					};
				}
			}
		}
	}


	int QuestID = this->GetQuestID(Number,Quest);
	if(QuestID != -1)
	{
		if(gObj[aIndex].S5Quest[QuestID].Active >= 1)
			return gObj[aIndex].S5Quest[QuestID].NumID;
	}
	return -1;
}

bool QuestS5::CheckAllowClass(int aIndex,int QuestIndex)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return false;
	}

	int Class = gObj[aIndex].Class;
	int ChangeUp = gObj[aIndex].ChangeUP;
	for(int i=0;i<this->quest[QuestIndex].ReqCount;i++)
	{
		if(this->quest[QuestIndex].Req[i].RequireClass[Class] > 0)
		{
			switch(this->quest[QuestIndex].Req[i].RequireClass[Class])
			{
				case 1:
				{
					return true;
				}break;
				case 2:
				{
					if(ChangeUp == 1)
						return true;
				}break;
				case 3:
				{
					if(ChangeUp == 2)
						return true;
				}break;
			}
		}
	}
	return false;
}

void QuestS5::SendNPCQuest(int aIndex)
{
	if ( gObjIsConnectedGP(aIndex) == FALSE )
	{
		LogAddC(2, "error-L3 [%s][%d]", __FILE__, __LINE__);
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];
	BYTE cBUFFER[512]={0}; //Max Value 0xFF | Max Quest Count = 62 per character
	BYTE Count = 0;
	PMSG_QUESTS5NPCSEND * pResult = (PMSG_QUESTS5NPCSEND *)(cBUFFER);
	QUESTS5INFO * qAdd = (QUESTS5INFO *)(cBUFFER + sizeof(PMSG_QUESTS5NPCSEND));
	int lOfs=sizeof(PMSG_QUESTS5NPCSEND);

	for(int i=0;i<this->Count;i++)
	{
		if(((lpObj->Level >= this->quest[i].LevelMin && lpObj->Level <= this->quest[i].LevelMax) && (this->CheckAllowClass(aIndex,i) == true) ) || (lpObj->S5Quest[i].Active >= 1  && this->CheckAllowClass(aIndex,i) == true))
		{
			int Number = this->quest[i].Num;
			int Quest = this->quest[i].QuestIndex;

			int IsRunning = this->QuestRunning(aIndex,Number,Quest);

			//if(Quest == 20)
			//{
			//	if(Number > 7)
			//	{
			//		if(this->QuestRunning(aIndex,0,20) == -1)
			//			continue;
			//		if(this->QuestRunning(aIndex,5,20) == -1)
			//			continue;
			//	}else if (Number == 5)
			//	{
			//		if(this->QuestRunning(aIndex,0,20) == -1)
			//			continue;
			//	}
			//}
			
			if(Quest == 20 && IsRunning == 2)
				continue;

			if(IsRunning == -1)
				Number = this->quest[i].Num;
			else
			{
				bool Quest6 = false;
				int QuestID = this->GetQuestID(Number,Quest);
				if(gObj[aIndex].S5Quest[QuestID].Active == 2)
					continue;
				if(gObj[aIndex].S5Quest[QuestID].Active == 3)
				{
					if(this->CheckWeekQuest(aIndex,QuestID) == false)
						continue;
					else
						Quest6 = true;
				}

				if(Quest6 == false)
					Number = this->quest[i].Req[IsRunning].Num;
				else
					Number = this->quest[i].Num;
			}
			
			if ((this->quest[i].NPCNum == 566 && lpObj->m_IfState.type == 105) || 
				(this->quest[i].NPCNum == 567 && lpObj->m_IfState.type == 103) || 
				(this->quest[i].NPCNum == 568 && lpObj->m_IfState.type == 104) || 
				(this->quest[i].NPCNum == 257 && lpObj->m_IfState.type == 102) || 
				(this->quest[i].NPCNum == 543 && lpObj->m_IfState.type == 100) || 
				(this->quest[i].NPCNum == 544 && lpObj->m_IfState.type == 101))
			{
				if(this->quest[i].NPCNum != 257 && lpObj->GensFam == 0)
				{
				}else
				{
					bool access = false;
					if(this->quest[i].QuestIndex != 20)
					{
						if(this->quest[i].NPCNum == 543 && lpObj->m_IfState.type == 100 && lpObj->GensFam == 1)
							access = true;
						if(this->quest[i].NPCNum == 544 && lpObj->m_IfState.type == 101 && lpObj->GensFam == 2)
							access = true;
					}else
					{
						if((this->quest[i].NPCNum == 543 || this->quest[i].NPCNum == 544) && (lpObj->m_IfState.type == 100 || lpObj->m_IfState.type == 101) && (lpObj->GensFam == 1 || lpObj->GensFam == 2))
							access = true;
					}
					if(this->quest[i].NPCNum == 257 && lpObj->m_IfState.type == 102)
						access = true;
					if(this->quest[i].NPCNum == 567 && lpObj->m_IfState.type == 103)
						access = true;
					if(this->quest[i].NPCNum == 568 && lpObj->m_IfState.type == 104)
						access = true;
					if(this->quest[i].NPCNum == 566 && lpObj->m_IfState.type == 105)
						access = true;

					if(access == true)
					{
						qAdd = (QUESTS5INFO *)(cBUFFER + lOfs);
						qAdd->Quest = MAKE_NUMBERW(LOBYTE(Quest),HIBYTE(Quest));
						qAdd->Num = MAKE_NUMBERW(LOBYTE(Number),HIBYTE(Number));
						lOfs+=sizeof(QUESTS5INFO);
						Count++;
					}
				}
			}
		}
	}
	
	LogAddTD("[QuestS5] SendNPC QuestList [%s][%s]", lpObj->AccountID, lpObj->Name);

	int Size = (sizeof(PMSG_QUESTS5NPCSEND) + sizeof(QUESTS5INFO) * Count)+1;
	PHeadSubSetB((LPBYTE)pResult,0xF6,0x0A,Size);
	pResult->Unk1 = 1;
	pResult->Unk2 = 1;
	//pResult->Unk3 = 9;
	pResult->QuestCount = Count;
	DataSend(aIndex, cBUFFER, pResult->h.size);
}

void QuestS5::SendTWindow(int aIndex)
{
	if ( gObjIsConnectedGP(aIndex) == FALSE )
	{
		LogAddC(2, "error-L3 [%s][%d]", __FILE__, __LINE__);
		return;
	}

	BYTE cBUFFER[512]={0}; //Max Value 0xFF | Max Quest Count = 62 per character
	BYTE Count = 0;
	PMSG_QUESTS5SEND * pResult = (PMSG_QUESTS5SEND *)(cBUFFER);
	QUESTS5INFO * qAdd = (QUESTS5INFO *)(cBUFFER + sizeof(PMSG_QUESTS5SEND));
	int lOfs=sizeof(PMSG_QUESTS5SEND);
	
	for(int i=0;i<MAX_QUEST_COUNT;i++)
	{
		if(gObj[aIndex].S5Quest[i].Active == 1)
		{
			WORD Number = this->quest[i].Req[gObj[aIndex].S5Quest[i].NumID].Num;
			WORD Quest = this->quest[i].QuestIndex;
			qAdd = (QUESTS5INFO *)(cBUFFER + lOfs-1);
			qAdd->Num = MAKE_NUMBERW(LOBYTE(Number),HIBYTE(Number));
			qAdd->Quest = MAKE_NUMBERW(LOBYTE(Quest),HIBYTE(Quest));
			lOfs+=sizeof(QUESTS5INFO);
			Count++;
		}
	}

	int Size = (sizeof(PMSG_QUESTS5SEND) + sizeof(QUESTS5INFO) * Count);
	PHeadSubSetB((LPBYTE)pResult,0xF6,0x1A,Size);
	pResult->QuestCount = Count;
	DataSend(aIndex, cBUFFER, pResult->h.size);
}

BOOL QuestS5::LoadQuestInfo(char * filename)
{
	memset(&this->quest,0,sizeof(this->quest));
	this->Count = 0;
	bool Requeriment = true;
	bool Reward = false;
	int Token;
	int n;

	SMDFile = fopen(filename, "r");

	if ( SMDFile == NULL )
	{
		MsgBox("load error %s", filename);
		return false;
	}

	n = 0;

	while ( true )
	{
		Token = GetToken();

		if ( Token == 2 )
		{
			break;
		}
		else
		{
			if ( Token == 1 )
			{
				int Type = TokenNumber;

				if ( Requeriment == true )
				{
					int iQuestIndex = this->Count;
					this->quest[iQuestIndex].Num = TokenNumber;

					Token = GetToken();
					this->quest[iQuestIndex].QuestIndex = TokenNumber;

					Token = GetToken();
					this->quest[iQuestIndex].NPCNum = TokenNumber;

					Token = GetToken();
					strcpy( this->quest[iQuestIndex].Name, TokenString);

					Token = GetToken();
					this->quest[iQuestIndex].LevelMin = TokenNumber;

					Token = GetToken();
					this->quest[iQuestIndex].LevelMax = TokenNumber;

					this->quest[iQuestIndex].ReqCount = 0;
					this->quest[iQuestIndex].RewCount = 0;

					while ( true )
					{
						int iSubInfoCount = this->quest[iQuestIndex].ReqCount;

						Token = GetToken();

						if ( Token == 0 )
						{
							if ( strcmp("end", TokenString) == 0  || iSubInfoCount >= MAX_QUEST_REQ )
							{
								Requeriment = false;
								Reward = true;
								break;
							}
						}
						this->quest[iQuestIndex].Req[iSubInfoCount].Num = TokenNumber;

						Token = GetToken();
						this->quest[iQuestIndex].Req[iSubInfoCount].QuestIndex = TokenNumber;

						Token = GetToken();
						this->quest[iQuestIndex].Req[iSubInfoCount].Type = TokenNumber;

						Token = GetToken();
						this->quest[iQuestIndex].Req[iSubInfoCount].Monster = TokenNumber;

						Token = GetToken();
						this->quest[iQuestIndex].Req[iSubInfoCount].ItemIndex = TokenNumber;

						Token = GetToken();
						this->quest[iQuestIndex].Req[iSubInfoCount].iLevel = TokenNumber;

						Token = GetToken();
						this->quest[iQuestIndex].Req[iSubInfoCount].iOpt = TokenNumber;

						Token = GetToken();
						this->quest[iQuestIndex].Req[iSubInfoCount].iLuck = TokenNumber;

						Token = GetToken();
						this->quest[iQuestIndex].Req[iSubInfoCount].iSkill = TokenNumber;

						Token = GetToken();
						this->quest[iQuestIndex].Req[iSubInfoCount].iDur = TokenNumber;

						Token = GetToken();
						this->quest[iQuestIndex].Req[iSubInfoCount].iExc = TokenNumber;

						Token = GetToken();
						this->quest[iQuestIndex].Req[iSubInfoCount].iAnc = TokenNumber;

						Token = GetToken();
						this->quest[iQuestIndex].Req[iSubInfoCount].KillCount = TokenNumber;
						if(this->quest[iQuestIndex].Req[iSubInfoCount].KillCount > 255)
							this->quest[iQuestIndex].Req[iSubInfoCount].KillCount = 255;

						Token = GetToken();
						this->quest[iQuestIndex].Req[iSubInfoCount].RequireClass[CLASS_WIZARD] = TokenNumber;

						Token = GetToken();
						this->quest[iQuestIndex].Req[iSubInfoCount].RequireClass[CLASS_KNIGHT] = TokenNumber;

						Token = GetToken();
						this->quest[iQuestIndex].Req[iSubInfoCount].RequireClass[CLASS_ELF] = TokenNumber;

						Token = GetToken();
						this->quest[iQuestIndex].Req[iSubInfoCount].RequireClass[CLASS_MAGICGLADIATOR] = TokenNumber;

						Token = GetToken();
						this->quest[iQuestIndex].Req[iSubInfoCount].RequireClass[CLASS_DARKLORD] = TokenNumber;

						Token = GetToken();
						this->quest[iQuestIndex].Req[iSubInfoCount].RequireClass[CLASS_SUMMONER] = TokenNumber;
			
						if(ReadConfig.S6E1 == 1)
						{
							Token = GetToken();
							this->quest[iQuestIndex].Req[iSubInfoCount].RequireClass[CLASS_RAGEFIGHTER] = TokenNumber;			
						}

						this->quest[iQuestIndex].ReqCount++;
					}
					
					LogAdd("[Quest S5] (%s) Load : Index %d, SubInfoCount %d",
						this->quest[iQuestIndex].Name, iQuestIndex,
						this->quest[iQuestIndex].ReqCount );
				}
				else if ( Reward == true )
				{
					int iQuestIndex = this->Count;

					while ( true )
					{
							int iFailInfoCount = this->quest[iQuestIndex].RewCount;

							if ( Token == 0 || iFailInfoCount >= MAX_QUEST_REQ )
							{
								if ( strcmp("end", TokenString) == 0 )
								{
									this->Count++;
									Reward = false;
									Requeriment = true;
									break;
								}
							}

							this->quest[iQuestIndex].Rew[iFailInfoCount].Num = TokenNumber;

							Token = GetToken();
							this->quest[iQuestIndex].Rew[iFailInfoCount].QuestIndex = TokenNumber;

							Token = GetToken();
							this->quest[iQuestIndex].Rew[iFailInfoCount].Type = TokenNumber;

							Token = GetToken();
							this->quest[iQuestIndex].Rew[iFailInfoCount].iType = TokenNumber;

							Token = GetToken();
							this->quest[iQuestIndex].Rew[iFailInfoCount].iIndex = TokenNumber;

							Token = GetToken();
							this->quest[iQuestIndex].Rew[iFailInfoCount].iLevel = TokenNumber;

							Token = GetToken();
							this->quest[iQuestIndex].Rew[iFailInfoCount].iOpt = TokenNumber;

							Token = GetToken();
							this->quest[iQuestIndex].Rew[iFailInfoCount].iLuck = TokenNumber;

							Token = GetToken();
							this->quest[iQuestIndex].Rew[iFailInfoCount].iSkill = TokenNumber;

							Token = GetToken();
							this->quest[iQuestIndex].Rew[iFailInfoCount].iDur = TokenNumber;

							Token = GetToken();
							this->quest[iQuestIndex].Rew[iFailInfoCount].iExc = TokenNumber;

							Token = GetToken();
							this->quest[iQuestIndex].Rew[iFailInfoCount].iAnc = TokenNumber;

							Token = GetToken();
							this->quest[iQuestIndex].Rew[iFailInfoCount].iCount = TokenNumber;

							this->quest[iQuestIndex].RewCount++;
							Token = GetToken();
						
					}
				}
				else
				{
					MsgBox("Quest S5 Info Read Fail");
					fclose(SMDFile);
					return false;
				}
			}
		}
	}

	fclose(SMDFile);
	return true;
}

int QuestS5::GetQuestID(int QuestNum, int QuestIndex)
{
	for(int i=0;i<MAX_QUEST_COUNT;i++)
	{
		if(this->quest[i].QuestIndex == QuestIndex)
		{
			if(this->quest[i].Num == QuestNum)
				return i;
			if(this->GetQuestReqID(i,QuestNum) != -1)
				return i;
		}
	}
	return -1;
}

bool QuestS5::CheckWeekQuest(int aIndex,int QuestID)
{
	if ( gObjIsConnectedGP(aIndex) == FALSE )
	{
		LogAddC(2, "error-L3 [%s][%d]", __FILE__, __LINE__);
		return false;
	}

	if(gObj[aIndex].S5Quest[QuestID].Active == 3)
	{
		struct tm *local;
		time_t t = time(NULL);
		local = localtime(&t);

		int Val = local->tm_yday - gObj[aIndex].S5Quest[QuestID].NumID;
		if(Val < 0 || Val > 7)
		{
			gObj[aIndex].S5Quest[QuestID].Active = 0;		
			gObj[aIndex].S5Quest[QuestID].NumID = 0;
			return true;
		}
	}

	return false;
}

void QuestS5::DelExactQuest(int aIndex, int QuestNum, int QuestIndex)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	int n = this->GetQuestID(QuestNum, QuestIndex);
	if(n != -1)
	{
		gObj[aIndex].S5Quest[n].Active = 0;
		gObj[aIndex].S5Quest[n].NumID = 0;
		for(int i=0;i<MAX_QUEST_REQ;i++)
			gObj[aIndex].S5Quest[n].State[i] = 0;			
	}
}

bool QuestS5::IsQuestItemActive(int aIndex,int MobNum)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return false;
	}

	if ( gObj[aIndex].Type != OBJ_USER )
	{
#if (PACK_EDITION>=3)
		if (gObj[aIndex].IsBot != 1)
		{
#endif
			LogAddC(2, "error-L3 [%s][%d]", __FILE__, __LINE__);
			return false;
#if (PACK_EDITION>=3)
		}else
		{
			aIndex = gObj[aIndex].BotNumOwner;
		}
#endif
	}
	
	if ( gObjIsConnectedGP(aIndex) == FALSE )
	{
		LogAddC(2, "error-L3 [%s][%d]", __FILE__, __LINE__);
		return false;
	}

	switch(MobNum)
	{
		case 70:
		{
			int n = this->GetQuestID(17, 20);
			if(n != -1)
			{
				if(gObj[aIndex].S5Quest[n].Active == 1)
					return true;
			}
		}break;
		case 58:
		{
			int n = this->GetQuestID(38, 20);
			if(n != -1)
			{
				if(gObj[aIndex].S5Quest[n].Active == 1)
					return true;
			}
		}break;
		case 484:
		{
			int n = this->GetQuestID(41, 20);
			if(n != -1)
			{
				if(gObj[aIndex].S5Quest[n].Active == 1)
					return true;
			}
		}break;
		case 304:
		{
			int n = this->GetQuestID(20, 20);
			if(n != -1)
			{
				if(gObj[aIndex].S5Quest[n].Active == 1)
					return true;
			}
		}break;
	}
	return false;
}

bool QuestS5::UseItem(int aIndex, int ItemID)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return false;
	}

	if (gObj[aIndex].PlayerQ5InfoReceived != 1)
	{
		//GCServerMsgStringSend ("ERROR: Please relogin!",aIndex,0x01 ) ;
		LogAddC(2, "ERROR: S5QUEST Info [%s][%s] Line: %d - RE-REQUESTING INFO", gObj[aIndex].AccountID, gObj[aIndex].Name, __LINE__);
		GetPlayerExtraInfo(aIndex,SCFExDB_GSSend_GetPlayerQ5Info);
		return false;
	}

	if(ItemID == ITEMGET(14,151))
	{
		SYSTEMTIME sysTime;
		GetLocalTime(&sysTime);
		WORD Day = sysTime.wDayOfWeek;//1 Monday
					
		int q1 = this->GetQuestID(5,20);
		if(gObj[aIndex].S5Quest[q1].Active == 2)
		{
			gObj[aIndex].S5Quest[q1].State[0] = 1;
		}

		switch(Day)
		{
			case 1:
			{
				if(this->QuestRunning(aIndex,9,20) == -1)
				{
					this->AddQuest(aIndex,9,20);
					this->SendTWindow(aIndex);
					gObj[aIndex].PlayerQ5InfoModified = 1;
					return true;
				}
			}break;
			case 2:
			{
				if(this->QuestRunning(aIndex,16,20) == -1)
				{
					this->AddQuest(aIndex,16,20);
					this->SendTWindow(aIndex);
					gObj[aIndex].PlayerQ5InfoModified = 1;
					return true;
				}
			}break;
			case 3:
			{
				if(this->QuestRunning(aIndex,23,20) == -1)
				{
					this->AddQuest(aIndex,23,20);
					this->SendTWindow(aIndex);
					gObj[aIndex].PlayerQ5InfoModified = 1;
					return true;
				}
			}break;
			case 4:
			{
				if(this->QuestRunning(aIndex,30,20) == -1)
				{
					this->AddQuest(aIndex,30,20);
					this->SendTWindow(aIndex);
					gObj[aIndex].PlayerQ5InfoModified = 1;
					return true;
				}
			}break;
			case 5:
			{
				if(this->QuestRunning(aIndex,37,20) == -1)
				{
					this->AddQuest(aIndex,37,20);
					this->SendTWindow(aIndex);
					gObj[aIndex].PlayerQ5InfoModified = 1;
					return true;
				}
			}break;
			case 6:
			{
				if(this->QuestRunning(aIndex,44,20) == -1)
				{
					this->AddQuest(aIndex,44,20);
					this->SendTWindow(aIndex);
					gObj[aIndex].PlayerQ5InfoModified = 1;
					return true;
				}
			}break;
			case 0:
			{
				if(this->QuestRunning(aIndex,51,20) == -1)
				{
					this->AddQuest(aIndex,51,20);
					this->SendTWindow(aIndex);
					gObj[aIndex].PlayerQ5InfoModified = 1;
					return true;
				}
			}break;
		}
		GCReFillSend(aIndex, gObj[aIndex].Life, 0xFD, 1, gObj[aIndex].iShield);
	}

	return false;
}

void QuestS5::DeletePlayerDayofWeekQuest(int aIndex, WORD DayofWeek)
{
	switch(DayofWeek)
	{
		case 1:
		{
			this->DelExactQuest(aIndex,9,20);
			this->DelExactQuest(aIndex,10,20);
			this->DelExactQuest(aIndex,13,20);
		}break;
		case 2:
		{
			this->DelExactQuest(aIndex,16,20);
			this->DelExactQuest(aIndex,17,20);
			this->DelExactQuest(aIndex,20,20);
		}break;
		case 3:
		{
			this->DelExactQuest(aIndex,23,20);
			this->DelExactQuest(aIndex,24,20);
			this->DelExactQuest(aIndex,27,20);
		}break;
		case 4:
		{
			this->DelExactQuest(aIndex,30,20);
			this->DelExactQuest(aIndex,31,20);
			this->DelExactQuest(aIndex,34,20);
		}break;
		case 5:
		{
			this->DelExactQuest(aIndex,37,20);
			this->DelExactQuest(aIndex,38,20);
			this->DelExactQuest(aIndex,41,20);
		}break;
		case 6:
		{
			this->DelExactQuest(aIndex,44,20);
			this->DelExactQuest(aIndex,45,20);
			this->DelExactQuest(aIndex,48,20);
		}break;
		case 0:
		{
			this->DelExactQuest(aIndex,51,20);
			this->DelExactQuest(aIndex,52,20);
			this->DelExactQuest(aIndex,56,20);
		}break;
	}
}

void QuestS5::DeletePlayerQuest(int aIndex,int QuestID,bool finished)
{
	if ( gObjIsConnectedGP(aIndex) == FALSE )
	{
		LogAddC(2, "error-L3 [%s][%d]", __FILE__, __LINE__);
		return;
	}

	if (gObj[aIndex].PlayerQ5InfoReceived != 1)
	{
		//GCServerMsgStringSend ("ERROR: Please relogin!",aIndex,0x01 ) ;
		LogAddC(2, "ERROR: S5QUEST Info [%s][%s] Line: %d - RE-REQUESTING INFO", gObj[aIndex].AccountID, gObj[aIndex].Name, __LINE__);
		GetPlayerExtraInfo(aIndex,SCFExDB_GSSend_GetPlayerQ5Info);
		return;
	}

	if(finished == true && this->quest[QuestID].QuestIndex == 6)
	{		
		struct tm *local;
		time_t t = time(NULL);
		local = localtime(&t);

		gObj[aIndex].S5Quest[QuestID].Active = 3;		
		gObj[aIndex].S5Quest[QuestID].NumID = local->tm_yday;

		for(int i=0;i<MAX_QUEST_REQ;i++)
			gObj[aIndex].S5Quest[QuestID].State[i] = 0;

	}else if(finished == true && this->quest[QuestID].QuestIndex == 20)
	{
		gObj[aIndex].S5Quest[QuestID].Active = 2;
		switch(this->quest[QuestID].Num)
		{
			case 5:
			{
				SYSTEMTIME sysTime;
				GetLocalTime(&sysTime);
				WORD Day = sysTime.wDayOfWeek;//1 Monday

				gObj[aIndex].S5Quest[QuestID].NumID = Day;
			}break;
			case 13:
			case 20:
			case 27:
			case 34:
			case 41:
			case 48:
			case 56:
			{				
				this->AddQuest(aIndex,59,20);
				this->SendTWindow(aIndex);
				gObj[aIndex].PlayerQ5InfoModified = 1;
			}break;
			case 59:
			{
				int n = this->GetQuestID(5, 20);
				if(n != -1)
				{
					int Day = gObj[aIndex].S5Quest[n].NumID;
					if(Day >= 0 && Day <= 6)
					{
						this->DeletePlayerDayofWeekQuest(aIndex, Day);
					}
				}
				this->DelExactQuest(aIndex,5,20);
				this->DelExactQuest(aIndex,59,20);
			}break;
		}
	}else
	{
		if((this->quest[QuestID].QuestIndex >= 16 && this->quest[QuestID].QuestIndex <= 18) || (this->quest[QuestID].QuestIndex == 20 ) || (finished == false) && (this->quest[QuestID].QuestIndex != 19))
		{
			gObj[aIndex].S5Quest[QuestID].Active = 0;
			gObj[aIndex].S5Quest[QuestID].NumID = 0;
			for(int i=0;i<MAX_QUEST_REQ;i++)
				gObj[aIndex].S5Quest[QuestID].State[i] = 0;
		}else
		{
			gObj[aIndex].S5Quest[QuestID].Active = 2;
		}
	}
	LogAddTD("[QuestS5] DeletePlayerQuest [%s][%s] (%s)", gObj[aIndex].AccountID, gObj[aIndex].Name,
			this->quest[QuestID].Name);
}

bool QuestS5::CheckInvFree(int aIndex,WORD QuestID)
{
	if ( gObjIsConnectedGP(aIndex) == FALSE )
	{
		LogAddC(2, "error-L3 [%s][%d]", __FILE__, __LINE__);
		return false;
	}

	BYTE TempInventoryMap[MAIN_INVENTORY_NORMAL_SIZE + (MAX_EXTENDED_INV*EXTENDED_INV_SIZE) + PSHOP_SIZE];
	memcpy(TempInventoryMap,gObj[aIndex].pInventoryMap,ReadConfig.GETSLOTS(aIndex));

	int Number = this->quest[QuestID].Rew[gObj[aIndex].S5Quest[QuestID].NumID].Num;
	int Quest = this->quest[QuestID].QuestIndex;
	for(int i=0;i<this->quest[QuestID].RewCount;i++)
	{
		if((Number == this->quest[QuestID].Rew[i].Num) && (Quest == this->quest[QuestID].QuestIndex))
		{
			BYTE RewardType = this->quest[QuestID].Rew[i].Type;
			if(RewardType == 4 || RewardType == 36)
			{
				for(int x=0;x<this->quest[QuestID].Rew[i].iCount;x++)
				{
					int item = ITEMGET(this->quest[QuestID].Rew[i].iType,this->quest[QuestID].Rew[i].iIndex);
					if(gObjTempInventoryInsertItem(&gObj[aIndex],item,TempInventoryMap) == 255)
					{
						return false;
					}
				}
			}
		}
	}
	return true;
}

void QuestS5::GiveReward(int aIndex,WORD QuestID)
{
	if ( gObjIsConnectedGP(aIndex) == FALSE )
	{
		LogAddC(2, "error-L3 [%s][%d]", __FILE__, __LINE__);
		return;
	}

	if(gObj[aIndex].S5Quest[QuestID].Active == 0)
	{
		GCServerMsgStringSend("[Anti-Hack] Try to finish quest",aIndex, 0x01);
		if(ReadConfig.AHLog == TRUE)
		{
			char sbuf[255] = {0};
			wsprintf(sbuf,"[Anti-Hack][QuestS5] (%s)(%s) Try to finish quest (%s) without start it.",gObj[aIndex].AccountID,gObj[aIndex].Name,this->quest[QuestID].Name);
			ANTI_HACK_LOG.Output(sbuf);
		}
		return;
	}

	int Number = this->quest[QuestID].Req[gObj[aIndex].S5Quest[QuestID].NumID].Num;
	int Quest = this->quest[QuestID].QuestIndex;

	PMSG_QUESTS5ACCEPTRECVs pResult = {0};
	PHeadSubSetB((LPBYTE)&pResult,0xF6,0x0D,9);
	pResult.Num = Number;
	pResult.Quest = Quest;

	//Check if have all:	
	DWORD MoneyRest = 0;
	for(int i=0;i<this->quest[QuestID].ReqCount;i++)
	{
		if((Number == this->quest[QuestID].Req[i].Num) && (Quest == this->quest[QuestID].QuestIndex))
		{
			WORD ReqType = this->quest[QuestID].Req[i].Type;
			switch(ReqType)
			{
				case 64:
				case 65:
				case 66:
				case 1:
				{
					WORD MonsterKilled = gObj[aIndex].S5Quest[QuestID].State[i];
					WORD ToKill = this->quest[QuestID].Req[i].KillCount;
					if(MonsterKilled < ToKill)
					{
						GCServerMsgStringSend("[Anti-Hack] Try to finish quest",aIndex, 0x01);
						if(ReadConfig.AHLog == TRUE)
						{
							char sbuf[255] = {0};
							wsprintf(sbuf,"[Anti-Hack][QuestS5] (%s)(%s) Try to finish quest (%s) without kill all monsters (%d/%d).",gObj[aIndex].AccountID,gObj[aIndex].Name,this->quest[QuestID].Name,MonsterKilled,ToKill);
							ANTI_HACK_LOG.Output(sbuf);
						}
						return;
					}
				}break;
				case 2:
				{					
					WORD Skill = this->quest[QuestID].Req[i].Monster;
					if(this->PlayerCheckSkill(aIndex,Skill) == 0)
					{
						GCServerMsgStringSend("[Anti-Hack] Try to finish quest",aIndex, 0x01);
						if(ReadConfig.AHLog == TRUE)
						{
							char sbuf[255] = {0};
							wsprintf(sbuf,"[Anti-Hack][QuestS5] (%s)(%s) Try to finish quest (%s) without have this skill %d.",gObj[aIndex].AccountID,gObj[aIndex].Name,this->quest[QuestID].Name,Skill);
							ANTI_HACK_LOG.Output(sbuf);
						}
						return;
					}
				}break;
				case 4:
				{
					int Count = this->PlayerCheckItemCount(aIndex,QuestID,i,false);
					if(Count >= this->quest[QuestID].Req[i].KillCount)
					{
						this->PlayerCheckItemCount(aIndex,QuestID,i,true);
					}else
					{
						if(ReadConfig.AHLog == TRUE)
						{
							char sbuf[255] = {0};
							wsprintf(sbuf,"[Anti-Hack][QuestS5] (%s)(%s) Try to finish quest (%s) without have all items for finish it.",gObj[aIndex].AccountID,gObj[aIndex].Name,this->quest[QuestID].Name);
							ANTI_HACK_LOG.Output(sbuf);
						}

						GCServerMsgStringSend("[Anti-Hack] Try to finish quest",aIndex, 0x01);
						return;
					}
				}break;
				case 256:
				case 257:
				case 258:
				{
					WORD MonsterKilled = gObj[aIndex].S5Quest[QuestID].State[i];
					if(MonsterKilled != 1)
					{
						GCServerMsgStringSend("[Anti-Hack] Try to finish quest",aIndex, 0x01);
						if(ReadConfig.AHLog == TRUE)
						{
							char sbuf[255] = {0};
							wsprintf(sbuf,"[Anti-Hack][QuestS5] (%s)(%s) Try to finish quest (%s) without finish the event.",gObj[aIndex].AccountID,gObj[aIndex].Name,this->quest[QuestID].Name);
							ANTI_HACK_LOG.Output(sbuf);
						}
						return;
					}
				}break;
				case 261:
				{
					DWORD Zen = this->quest[QuestID].Req[i].Monster;
					if(gObj[aIndex].Money >= Zen)
					{
						MoneyRest = Zen;
					}else
					{
						GCServerMsgStringSend("[Anti-Hack] Try to finish quest without zen",aIndex, 0x01);
						if(ReadConfig.AHLog == TRUE)
						{
							char sbuf[255] = {0};
							wsprintf(sbuf,"[Anti-Hack][QuestS5] (%s)(%s) Try to finish quest (%s) without zen.",gObj[aIndex].AccountID,gObj[aIndex].Name,this->quest[QuestID].Name);
							ANTI_HACK_LOG.Output(sbuf);
						}
						return;
					}
				}break;
			}
		}
	}
	//

	bool ok = this->CheckInvFree(aIndex,QuestID);
			
	if(ok == true)
	{
		if(MoneyRest > 0)
		{
			gObj[aIndex].Money -= MoneyRest;
			CGZenSend(gObj[aIndex].m_Index);
		}

		BYTE randRew[5] = {0};
		BYTE RandRewCount = 0;
		for(int i=0;i<this->quest[QuestID].RewCount;i++)
		{
			if((Number == this->quest[QuestID].Rew[i].Num) && (Quest == this->quest[QuestID].QuestIndex))
			{
				BYTE RewardType = this->quest[QuestID].Rew[i].Type;
				switch(RewardType)
				{
					case 1: //Exp
					{
						if (gObjIsMaxLevel(&gObj[aIndex]))
						{
							//Do nothing
						}
						else
						{
							int exp = this->quest[QuestID].Rew[i].iType;
							PMSG_KILLPLAYER pkillMsg;
							
							PHeadSetBE((LPBYTE)&pkillMsg, 0x16, sizeof(pkillMsg));

							pkillMsg.NumberH = -1;
							pkillMsg.NumberL = -1;
							pkillMsg.ExpH = SET_NUMBERH(exp);
							pkillMsg.ExpL = SET_NUMBERL(exp);
							pkillMsg.DamageH = SET_NUMBERH(0x00);
							pkillMsg.DamageL = SET_NUMBERL(0x00);

							DataSend(aIndex, (UCHAR*)&pkillMsg, pkillMsg.h.size);
							int LevelUp = 0;					
							gObjLevelUp(&gObj[aIndex],exp,gObj[aIndex].Class,EVENT_TYPE_SEASON5_QUEST,LevelUp);
						}
					}break;
					case 2: //Zen
					{		
						int Money = this->quest[QuestID].Rew[i].iType;

						if((__int64)(Money + gObj[aIndex].Money) > (__int64)MAX_ZEN)
							gObj[aIndex].Money = MAX_ZEN;
						else
							gObj[aIndex].Money += Money;

						CGZenSend(gObj[aIndex].m_Index);
					}break;

					case 4: //Items
					{						
						for(int x=0;x<this->quest[QuestID].Rew[i].iCount;x++)
						{
							int item = ITEMGET(this->quest[QuestID].Rew[i].iType,this->quest[QuestID].Rew[i].iIndex);
							ItemSerialCreateSend(aIndex, 235, 0, 0, item, this->quest[QuestID].Rew[i].iLevel, this->quest[QuestID].Rew[i].iDur, this->quest[QuestID].Rew[i].iSkill, this->quest[QuestID].Rew[i].iLuck, (this->quest[QuestID].Rew[i].iOpt/4), -1, this->quest[QuestID].Rew[i].iExc, this->quest[QuestID].Rew[i].iAnc);
						}
					}break;

					case 16: //Contribution
					{
						if(gObj[aIndex].GensFam > 0)
						{
							gObj[aIndex].GensCont += this->quest[QuestID].Rew[i].iType;
							gObj[aIndex].PlayerExtraInfoModified = 1;
							Gens.Ranking(aIndex);
						}
					}break;
					
					case 36:
					{
						randRew[RandRewCount] = i;
						RandRewCount++;
					}break;
				}
			}
		}
		if(RandRewCount > 0)
		{
			if(RandRewCount > 5)
				RandRewCount = 5;
			int RandNum = rand()%RandRewCount;

			if(RandNum > 4)
				RandNum = 4;				
			
			for(int x=0;x<this->quest[QuestID].Rew[RandNum].iCount;x++)
			{
				int item = ITEMGET(this->quest[QuestID].Rew[RandNum].iType,this->quest[QuestID].Rew[RandNum].iIndex);
				ItemSerialCreateSend(aIndex, 235, 0, 0, item, this->quest[QuestID].Rew[RandNum].iLevel, this->quest[QuestID].Rew[RandNum].iDur, this->quest[QuestID].Rew[RandNum].iSkill, this->quest[QuestID].Rew[RandNum].iLuck, (this->quest[QuestID].Rew[RandNum].iOpt/4), -1, this->quest[QuestID].Rew[RandNum].iExc, this->quest[QuestID].Rew[RandNum].iAnc);
			}

		}
		LogAddTD("[QuestS5] QuestFinished [%s][%s] (%s)", gObj[aIndex].AccountID, gObj[aIndex].Name,
			this->quest[QuestID].Name);
		pResult.Unk = 1;
		this->DeletePlayerQuest(aIndex,QuestID,true);
		this->SendTWindow(aIndex);
	}else
	{
		pResult.Unk = 3;
	}
	DataSend(aIndex ,(UCHAR*)&pResult, pResult.h.size);
}

int QuestS5::GetQuestReqID(int QuestID, int QuestNum)
{
	for(int i=0;i<MAX_QUEST_REQ;i++)
	{
		if(this->quest[QuestID].Req[i].Num == QuestNum)
			return i;
	}
	return -1;
}

int QuestS5::GetNextStatus(int PlayerID, int QuestID)
{
	if(OBJMAX_RANGE(PlayerID) == 0)
	{
		return -1;
	}

	int Class =  gObj[PlayerID].Class;
	int ChangeUp = gObj[PlayerID].ChangeUP;

	if(this->quest[QuestID].QuestIndex == 19)
	{
		int RandNum = rand()%this->quest[QuestID].ReqCount;
		return RandNum;
	}else
	{
		for(int i=0;i<this->quest[QuestID].ReqCount;i++)
		{
			if(this->quest[QuestID].Req[i].RequireClass[Class] > 0)
			{
				switch(this->quest[QuestID].Req[i].RequireClass[Class])
				{
					case 1:
					{
						return i;
					}break;
					case 2:
					{
						if(ChangeUp == 1)
							return i;
					}break;
					case 3:
					{
						if(gObjIsNewClass(&gObj[PlayerID]) == TRUE)
							return i;
					}break;
				}
			}
		}
	}
	return -1;
}

int QuestS5::CheckNextStatus(int PlayerID, int QuestNum, int QuestIndex)
{
	int QuestID = this->GetQuestID(QuestNum,QuestIndex);
	if(QuestID != -1)
	{
		int NextStatus = this->GetNextStatus(PlayerID,QuestID);
		if(QuestIndex == 20 && QuestNum < 5)
			NextStatus = QuestNum;
		if(NextStatus != -1)
		{
			this->SendDialogReq(PlayerID,0x0c,QuestID,NextStatus,QuestIndex);
			return NextStatus;
		}
	}
	return -1;
}

void QuestS5::RemoveGensQuest(int aIndex)
{
	if ( gObjIsConnectedGP(aIndex) == FALSE )
	{
		LogAddC(2, "error-L3 [%s][%d]", __FILE__, __LINE__);
		return;
	}

	for(int i=0;i<MAX_QUEST_COUNT;i++)
	{
		if(gObj[aIndex].S5Quest[i].Active == 1)
		{
			if(this->quest[i].NPCNum == 543 || this->quest[i].NPCNum == 544)
			{
				gObj[aIndex].PlayerQ5InfoModified = 1;

				gObj[aIndex].S5Quest[i].Active = 0;
				gObj[aIndex].S5Quest[i].NumID = 0;
				for(int x=0;x<MAX_QUEST_REQ;x++)
					gObj[aIndex].S5Quest[i].State[x] = 0;
			}
		}
	}
	this->SendTWindow(aIndex);
}