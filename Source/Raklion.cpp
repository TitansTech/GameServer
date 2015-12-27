#include "stdafx.h"
#include "Raklion.h"
#include "GameServer.h"
#include "LogProc.h"
#include "GameMain.h"
#include "TNotice.h"
#include "..\include\readscript.h"
#include "gObjMonster.h"
//#include "GameServerAuth.h"
#include "MapServerManager.h"
#include "DSProtocol.h"
#include "Event.h"
#include "..\common\winutil.h"

RaklionClass Raklion;

BOOL RaklionClass::WarpCheck(int PlayerID,int Map, int TargetMap)
{
	if(Enabled == 1)
	{
		if(EnableWarp == 0)
		{
			if(TargetMap == MAP_INDEX_RAKLIONBOSS)
			{
				GCServerMsgStringSend("The Portal is closed!",PlayerID, 0x01);
				return 0;
			} 
			else
			{
				if(Map == MAP_INDEX_RAKLIONBOSS)
				{				
					GCServerMsgStringSend("You cant leave the area until event ends!",PlayerID, 0x01);
					return 0;
				}			
			}
		} 
		else 
		{
			if( (Raklion.PlayersCount()+1)>MaxPlayers )
			{
				GCServerMsgStringSend("Bad luck, the map is full!",PlayerID, 0x01);
				return 0;
			}
		}
	}
	return 1;
}

void RaklionClass::ExitPlayer()
{
	for ( int n = OBJ_STARTUSERINDEX ; n < OBJMAX ; n++)
	{
		if ( gObj[n].Connected == PLAYER_PLAYING )
		{
			if (( gObj[n].Type  == OBJ_USER ) && ( gObj[n].MapNumber == MAP_INDEX_RAKLIONBOSS))
			{
				gObjTeleport(n, MAP_INDEX_RAKLION, 172, 24);
			}
		}
	}
}

int RaklionClass::PlayersCount()
{
	int Count=0;

	for ( int n = OBJ_STARTUSERINDEX ; n < OBJMAX ; n++)
	{
		if ( gObj[n].Connected == PLAYER_PLAYING )
		{
			if (( gObj[n].Type  == OBJ_USER ) && ( gObj[n].MapNumber == MAP_INDEX_RAKLIONBOSS))
			{
				Count++;
			}
		}
	}	
	return Count;
}

void RaklionClass__InsideTrigger(void * lpParam)
{
	char sBuf[1024] = {0};
	int FiveMins=(1000 * (5*60));
	Raklion.EnableWarp = 1;
	Raklion.Stage = 0;
	Raklion.Start = 0;

	int Seconds = 0;

	for(int i=5;i>0;i--)
	{
		wsprintf(sBuf, lMsg.Get(MSGGET(14, 32)), i);
		LogAddTD("[Raklion Hatchery] Portal will close after %d minute(s).",i);
		AllSendServerMsg(sBuf);
		
		Sleep(60000);
	}
	LogAddTD("[Raklion Hatchery] Portal is closed.");
	AllSendServerMsg(lMsg.Get(MSGGET(14, 33)));
	
	Raklion.EnableWarp = 0;

	if(Raklion.PlayersCount()>0)
	{
		Raklion.Start = 1;
		GCServerMsgStringSendToMap(lMsg.Get(MSGGET(14, 35)),58,0);
		GCServerMsgStringSendToMap(lMsg.Get(MSGGET(14, 36)),58,0);
		Raklion.SetMonsters(0);
		Raklion.Stage = 0;

		Sleep(FiveMins);
		Raklion.ClearStageMonsters(0);

		if(Raklion.PlayersCount()>0)
		{
			GCServerMsgStringSendToMap(lMsg.Get(MSGGET(14, 37)),58,0);
			Raklion.SetMonsters(1);
			Raklion.Stage = 1;

			BOOL EggDie = FALSE;

			while((EggDie==FALSE) && (Seconds < 300))
			{
				Seconds++;
				if(Raklion.EggKills >= Raklion.EggCount)
				{
					EggDie = TRUE;
				}
				Sleep(1000);
			}
			
			if(EggDie == TRUE)
			{
				GCServerMsgStringSendToMap(lMsg.Get(MSGGET(14, 38)),MAP_INDEX_RAKLIONBOSS,0);
				Sleep(20000);
				Raklion.SetMonsters(2);
				Raklion.Stage = 2;

				BOOL BossDie = FALSE;
				Seconds = 0;

				while((BossDie==FALSE)&&(Seconds < 300))
				{
					Seconds++;
					if(Raklion.BossKills >= Raklion.BossCount)
					{
						BossDie = TRUE;
						GCServerMsgStringSendToMap(lMsg.Get(MSGGET(14, 39)),MAP_INDEX_RAKLIONBOSS,0);
						LogAddTD("[Raklion Hatchery] Event SUCCESS! All Selupans are defeated!");
					}

					if(Seconds==270)
					{
						GCServerMsgStringSendToMap(lMsg.Get(MSGGET(14, 40)),MAP_INDEX_RAKLIONBOSS,0);
					}
					if(Seconds==280)
					{
						GCServerMsgStringSendToMap(lMsg.Get(MSGGET(14, 41)),MAP_INDEX_RAKLIONBOSS,0);
					}
					if(Seconds==290)
					{
						GCServerMsgStringSendToMap(lMsg.Get(MSGGET(14, 42)),MAP_INDEX_RAKLIONBOSS,0);
					}
					Sleep(1000);
				}
				if(BossDie == FALSE)
				{
					GCServerMsgStringSendToMap(lMsg.Get(MSGGET(14, 43)),MAP_INDEX_RAKLIONBOSS,0);
					LogAddTD("[Raklion Hatchery] Event FAILURE! Selupans have retreated.");
				}
			} else 
			{
				GCServerMsgStringSendToMap(lMsg.Get(MSGGET(14, 44)),MAP_INDEX_RAKLIONBOSS,0);
				LogAddTD("[Raklion Hatchery] Event FAILURE! Must kill all eggs before Selupan appear!");
			}

			Raklion.ClearAllMonsters();
			GCServerMsgStringSendToMap(lMsg.Get(MSGGET(14, 45)),MAP_INDEX_RAKLIONBOSS,1);
			Sleep(30000);
			Raklion.ExitPlayer();
		} else 
		{			
			LogAddTD("[Raklion Hatchery] Event finished: All players gone!");
		}
	} else 
	{
		LogAddTD("[Raklion Hatchery] FAILURE: No players inside!");
	}
	Raklion.Start = 0;
	_endthread();
}

void RaklionClass::EventStart()
{
	EggKills = 0;
	BossKills = 0;

	if((Enabled == 1) && (Start == 0))
	{
		_beginthread( RaklionClass__InsideTrigger, 0, NULL  );
	}
}

void RaklionClass::MonsterDiePoint(LPOBJ lpObj, LPOBJ lmObj)
{
	char sBuf[1024] = {0};

	if((Enabled == 1) && (Start == 1) && (MobLoad == 1))
	{
		if(lpObj->MapNumber == MAP_INDEX_RAKLIONBOSS)
		{
			if(Stage>0)
			{
				if(Stage == 1)
				{
					EnterCriticalSection(&this->m_critEventData);
					EggKills++;
					LeaveCriticalSection(&this->m_critEventData);

					if((EggCount-EggKills>0) && (EggCount-EggKills<=5))
					{
						wsprintf(sBuf, lMsg.Get(MSGGET(14, 46)), lpObj->Name,(EggCount-EggKills));
						GCServerMsgStringSendToMap(sBuf,MAP_INDEX_RAKLIONBOSS,1);

						LogAddTD("[Raklion Hatchery][%s] Kill 1 Egg, %d Left!", lpObj->Name,(EggCount-EggKills));
					} 
					if (EggCount-EggKills<=0) {
						wsprintf(sBuf, lMsg.Get(MSGGET(14, 48)), lpObj->Name);
						GCServerMsgStringSendToMap(sBuf,MAP_INDEX_RAKLIONBOSS,0);

						LogAddTD("[Raklion Hatchery][%s] Kill the last Egg!", lpObj->Name);
					}
				}

				if(Stage == 2)
				{
					EnterCriticalSection(&this->m_critEventData);
					BossKills++;
					LeaveCriticalSection(&this->m_critEventData);

					if((BossCount-BossKills>0) && (BossCount-BossKills<=5))
					{
						wsprintf(sBuf, lMsg.Get(MSGGET(14, 47)), lpObj->Name,(BossCount-BossKills));
						GCServerMsgStringSendToMap(sBuf,MAP_INDEX_RAKLIONBOSS,1);

						LogAddTD("[Raklion Hatchery][%s] Kill 1 Selupan, %d Left!", lpObj->Name,(BossCount-BossKills));
					}
					if (BossCount-BossKills<=0) 
					{
						wsprintf(sBuf, lMsg.Get(MSGGET(14, 49)), lpObj->Name);
						GCServerMsgStringSendToMap(sBuf,MAP_INDEX_RAKLIONBOSS,0);

						LogAddTD("[Raklion Hatchery][%s] Kill the last Selupan!", lpObj->Name);
					}
					SelupanEventItemBoxOpen(lpObj, lmObj->MapNumber, lmObj->X, lmObj->Y);
				}
			}
		}
	}
}

BOOL RaklionClass::MonsterDieRemove(LPOBJ lpObj, LPOBJ lmObj)
{
	char sBuf[1024] = {0};

	if((Enabled == 1) && (Start == 1) && (MobLoad == 1))
	{
		if(lpObj->MapNumber == MAP_INDEX_RAKLIONBOSS)
		{
			if(Stage>0)
			{
				gObjDel(lmObj->m_Index);
				return TRUE;
			}
		}
	}
	return FALSE;
}

void RaklionClass::ClearStageMonsters(int Stage)
{
	if((Enabled == 1) && (MobLoad == 1))
	{
		for(int i=0;i<MobCount;i++)
		{
			if(RalikionMob[i].Stage == Stage)
			{
				if (RalikionMob[i].ID >= 0)
				{
					if (RalikionMob[i].Number == gObj[RalikionMob[i].ID].Class &&
						gObj[RalikionMob[i].ID].MapNumber == MAP_INDEX_RAKLIONBOSS)
					{
						gObjDel(RalikionMob[i].ID);
					}

					RalikionMob[i].ID = -1;
				}
			}
		}
	}
}

void RaklionClass::ClearAllMonsters()
{
	if((Enabled == 1) && (MobLoad == 1))
	{
		for(int i=0;i<MobCount;i++)
		{
			if (RalikionMob[i].ID >= 0)
			{
				if (RalikionMob[i].Number == gObj[RalikionMob[i].ID].Class &&
					gObj[RalikionMob[i].ID].MapNumber == MAP_INDEX_RAKLIONBOSS)
				{
					gObjDel(RalikionMob[i].ID);
				}

				RalikionMob[i].ID = -1;
			}
		}
	}
}

void RaklionClass::SetMonsters(int Stage)
{
	if((Enabled == 1) && (MobLoad == 1))
	{
		for(int i=0;i<MobCount;i++)
		{
			if(RalikionMob[i].Stage == Stage)
			{
				int MobID = gObjAddMonster(gMSetBase.m_Mp[RalikionMob[i].Pos].m_MapNumber);
				if(MobID>=0)
				{
					RalikionMob[i].ID=MobID;
					gObjSetPosMonster(MobID, RalikionMob[i].Pos);
					gObjSetMonster(MobID, gMSetBase.m_Mp[RalikionMob[i].Pos].m_Type,"RaklionClass::SetMonsters");
				}
			}
		}
	}
}


void RaklionClass::ReadMonsters(LPSTR FileName)
{
	if((Enabled == 1) && (MobLoad == 1))
	{
		int LineCount =0;
		FILE *fp;
		char sLineTxt[255] = {0};
		BOOL bRead = FALSE;
		int Stage = -1;
		
		MobCount=0;
		BossCount=0;
		EggCount=0;

		for (int iM = 0; iM < 256; iM++)
		{
			RalikionMob[iM].ID = -1;
			RalikionMob[iM].Pos = -1;
			RalikionMob[iM].Stage = -1;
			RalikionMob[iM].Number = -1;
		}

		
		//srand((unsigned)time(NULL));
		char path[255]; 

		if((fp=fopen(FileName, "r")) == NULL)
		{
			return;
		}

		rewind(fp);
			
		while(fgets(sLineTxt, 255, fp) != NULL)
		{
			LineCount++;
			if(sLineTxt[0] == '/')continue;  
			if(sLineTxt[0] == ';')continue;  
			
			bRead = TRUE;		

			if((bRead==TRUE) && (strlen(sLineTxt)<3))
			{
				sscanf(sLineTxt, "%d", &Stage);
			}


			if((bRead==TRUE) && (strlen(sLineTxt)>3))
			{
				if ((sLineTxt[0] == 'e')&&(sLineTxt[1] == 'n')&&(sLineTxt[2] == 'd'))
				{
					Stage = -1;
					continue;
				} 
				else
				{
					if(Stage>=0)
					{
						int Monster;
						int Map = 58;
						switch(Stage)
						{
							case 0:
							{
								Monster = 457;
							}break;
							case 1:
							{
								Monster = 460 + (rand() % 2); 
							}break;
							case 2:
							{
								Monster = 459;
							}break;
						}
						int X, Y, HowMany=0;
						sscanf(sLineTxt, "%d %d %d", &X, &Y, &HowMany);
							
						for(int i=0;i<HowMany;i++)
						{
							if (MobCount < 256)
							{
								int MobPos = gObjMonsterAdd(Monster,Map,X,Y);
								if(MobPos >= 0)
								{
									RalikionMob[MobCount].ID = -1;
									RalikionMob[MobCount].Pos = MobPos;
									RalikionMob[MobCount].Stage = Stage;
									RalikionMob[MobCount].Number = Monster;
									if(Stage == 2)
									{
										BossCount++;
									}
									else if(Stage ==1)
									{
										EggCount++;
									}
									MobCount++;
								}
							} else {
								LogAddTD("[RaklionEvent][Overflow] - Cant add monster to Map:%d:, ID:%d, X:%d, Y:%d (Stage:%d)",
									Map,Monster,X,Y,Stage);
							}
						}
					}
				}
			}
		}

		rewind(fp);
		fclose(fp);

		LogAddTD("[Raklion Event] - %s file is Loaded",FileName);
	}
}

void RaklionClass::ReadINI(char * FilePath)
{
	Start				= 0;
	Enabled				= GetPrivateProfileInt("Common", "SCFRaklionEventEnabled",0, FilePath) ;
	GateIN				= GetPrivateProfileInt("Common", "SCFRaklionEventGateIN",290, FilePath) ;
	GateOUT				= GetPrivateProfileInt("Common", "SCFRaklionEventGateOUT",292, FilePath) ;
	MobLoad				= GetPrivateProfileInt("Common", "SCFRaklionEventMobsLoad",1, FilePath) ;
	MaxPlayers			= GetPrivateProfileInt("Common", "SCFRaklionEventMaxPlayers",10, FilePath) ;
	SelupanDropItems	= GetPrivateProfileInt("Common", "SCFRaklionSelupanDropItems",10, FilePath) ;

	LogAddTD("[SCF Raklion] - %s file is Loaded",FilePath);
}