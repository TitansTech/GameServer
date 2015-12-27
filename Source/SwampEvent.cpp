#include "stdafx.h"
#include "GameServer.h"
#include "LogProc.h"
#include "GameMain.h"
#include "TNotice.h"
#include "..\include\readscript.h"
#include "gObjMonster.h"
#include "MapServerManager.h"
#include "DSProtocol.h"
#include "Event.h"
#include "ImperialGuardian.h"
#include "SwampEvent.h"
#include "..\common\winutil.h"
#include "SCFPostServerProtocol.h"

#if (PACK_EDITION>=3)
cSwampEvent g_Swamp;

void cSwampEvent::Clean()
{
	this->Start = FALSE;

	this->CurrentRoom = 0;
	this->CurrentGroup = 0;

	this->MobsStageGroupCount = 0;
	this->MobsKilled = 0;
}

void cSwampEvent::ReadConfigs(char * FilePath)
{
	this->Enabled						= GetPrivateProfileInt("Common", "SCFSwampEventEnabled",0, FilePath);
	this->OnlyMastersEnter				= GetPrivateProfileInt("Common", "SCFSwampEventOnlyMasters",0, FilePath);

	this->SwampMapEnterOnlyWhenStarted	= GetPrivateProfileInt("Common", "SCFSwampMapEnterOnlyWhenStarted",0, FilePath);
	this->SwampIfWonKeepOpenMapMinutes	= GetPrivateProfileInt("Common", "SCFSwampIfWonKeepOpenMapMinutes",0, FilePath);

	this->MedusaDropItemCount			= GetPrivateProfileInt("Common", "SCFSwampMedusaDropItemCount",4, FilePath);

	this->SwampWarpGate					= GetPrivateProfileInt("Common", "SCFSwampWarpGate",273, FilePath);
	this->SwampGateRangeStart			= GetPrivateProfileInt("Common", "SCFSwampGateRangeStart",274, FilePath);
	this->SwampGateRangeEnd				= GetPrivateProfileInt("Common", "SCFSwampGateRangeEnd",285, FilePath);

	this->Clean();
	
	LogAddTD("[Swamp Event] - %s file is Loaded",FilePath);
}

void cSwampEvent::SendMessageToAllGS(char * Msg)
{
	if(ReadConfig.SCFPSON == FALSE)
		AllSendServerMsg(Msg);
	else
		SCFAllServerMsgSend(Msg);
}


#if (GS_CASTLE==1)

void cSwampEvent::ReadMonsters(char * FilePath)
{
	if(this->Enabled == 1)
	{
		SMDFile = fopen(FilePath, "r");
		if ( SMDFile == NULL )
		{
			MsgBox("Swamp Event data load error %s", FilePath);
			return;
		}

		//Reset All Rooms
		for(int RoomID=0; RoomID < SWAMP_ROOMS; RoomID++)
		{
			pRoom[RoomID].Active = 0;
			pRoom[RoomID].BossID = 0;
			pRoom[RoomID].Groups = 0;
			memset(pRoom[RoomID].Territory, 0,sizeof(pRoom[RoomID].Territory));
		}

		//Reset Each Group Timer to 5 minutes
		for(int GroupID=0; GroupID < SWAMP_MAXGROUPS; GroupID++)
		{
				GroupTimer[GroupID] = 0;
		}

		//Clear Monster Structure
		EnterCriticalSection(&this->m_critEventData);
		this->m_swampMonsterInfo.clear();
		LeaveCriticalSection(&this->m_critEventData);

		int TotalMobCount = 0;
		int TotalMobLines = 0;

		//Reset Event Statistics
		this->CurrentRoom = 0;
		this->CurrentGroup = 0;
		this->MobsStageGroupCount = 0;
		this->MobsKilled = 0;

		if (g_MapServerManager.CheckMapCanMove(MAP_INDEX_SWAMP) == FALSE )
		{
			//fclose(SMDFile);
			LogAddCTD(3,"[Swamp Event] No need to load monster base file!");
			//return;
		}

		int type = -1;
		SMDToken Token;

		while ( true )
		{
			Token = GetToken();
			if( Token == 2 )
			{
				break;
			}

			if( Token == NUMBER )
			{
				while(true)
				{
					type = TokenNumber;

					if( type == 0 )
					{
						while(true)
						{
							Token = GetToken();
					
							if( strcmp("end", TokenString) == 0 ) 
							{
								type++;
								break;
							}
					
							int RoomID = TokenNumber; 
							RoomID = RoomID - 1;

							if( RoomID >= SWAMP_ROOMS ) 
							{
								type++;
								break;
							}

							Token = GetToken();
							pRoom[RoomID].Groups = TokenNumber;

							Token = GetToken();
							pRoom[RoomID].BossID = TokenNumber;

							Token = GetToken();
							strcpy(pRoom[RoomID].Territory, TokenString);

							//Mark Room as loaded
							if (pRoom[RoomID].Groups > 0)
								pRoom[RoomID].Active = 1;
							else
								pRoom[RoomID].Active = 0;

							//Add room id to the monster vector
							std::vector<SWAMP_EVENTMOBDATA> vecMonsterInfo;
							this->m_swampMonsterInfo.insert( std::pair<int, std::vector<SWAMP_EVENTMOBDATA> >(RoomID+1,vecMonsterInfo) );

							LogAddCTD(3,"[Swamp Event] ROOM:%d [%s] has %d groups",
								RoomID+1, pRoom[RoomID].Territory, pRoom[RoomID].Groups
							);
						}
					}
					else if( type == 1 )
					{
						while(true)
						{
							Token = GetToken();

							if( strcmp("end", TokenString) == 0 ) 
							{
								type++;
								break;
							}

							int GroupID = TokenNumber; 
							GroupID = GroupID - 1;

							if( GroupID >= SWAMP_MAXGROUPS ) 
							{
								type++;
								break;
							}

							Token = GetToken();
							GroupTimer[GroupID] = TokenNumber;

							LogAddCTD(3,"[Swamp Event] GROUP:%d set to %d minutes",
								GroupID+1, GroupTimer[GroupID]
							);
						}
					}
					else if(type == 2)
					{
						int MobNum;
						int XF;
						int YF;
						int XT;
						int YT;
						int Room;
						int Wave;
						int Count;

						while(true)
						{
							Token = GetToken();
							if( strcmp("end", TokenString) == 0 ) 
							{
								type++;
								break;
							}

							SWAMP_EVENTMOBDATA	m_MonsterInfo;

							m_MonsterInfo.Number = TokenNumber;

							Token = GetToken();
							m_MonsterInfo.XF = TokenNumber;

							Token = GetToken();
							m_MonsterInfo.YF = TokenNumber;

							Token = GetToken();
							m_MonsterInfo.XT = TokenNumber;

							Token = GetToken();
							m_MonsterInfo.YT = TokenNumber;

							Token = GetToken();
							m_MonsterInfo.Room = TokenNumber;

							Token = GetToken();
							m_MonsterInfo.Group = TokenNumber;

							Token = GetToken();
							m_MonsterInfo.Count = TokenNumber;

							TotalMobCount += m_MonsterInfo.Count;
							TotalMobLines += 1;

							EnterCriticalSection(&this->m_critEventData);

							std::map<int, std::vector<SWAMP_EVENTMOBDATA> >::iterator it = this->m_swampMonsterInfo.find(m_MonsterInfo.Room);
							if( it != this->m_swampMonsterInfo.end() )
							{
								it->second.push_back(m_MonsterInfo);

								LogAddCTD(3,"[Swamp Event] ROOM:%d[%d] Monster:%d Count:%d",
									m_MonsterInfo.Room, m_MonsterInfo.Group,
									m_MonsterInfo.Number, m_MonsterInfo.Count
								);
							}
					
							LeaveCriticalSection(&this->m_critEventData);
						}
					}
					break;
				}
			}
		}

		fclose(SMDFile);
		LogAddCTD(3,"[Swamp Event] - %s file is Loaded Event Lines:%d Mobs:%d",
			FilePath,TotalMobLines,TotalMobCount 
		);
	}
}

bool cSwampEvent::GateMove(int aIndex, int Gate)
{
	if(this->Enabled == 1)
	{
		//Entrance to All Sector Gates Except Boss Gate
		if(Gate >= this->SwampGateRangeStart && Gate <= this->SwampGateRangeEnd)
		{
			//Class Check
			if (this->OnlyMastersEnter == 1)
			{
				if(gObjIsNewClass(&gObj[aIndex]) == FALSE)
				{
					::GCServerMsgStringSend(lMsg.Get(MSGGET(14, 126)),aIndex,1);
					return false;
				}
			}

			//Gate Check
			if (this->SwampWarpGate != Gate)
			{
				if(this->Start == FALSE)
				{
					if(this->SwampMapEnterOnlyWhenStarted == 1)
					{
						//You must w8 X mins in this room before event start
						::GCServerMsgStringSend(lMsg.Get(MSGGET(14, 128)),aIndex,1);
						return false;
					}
				}
			}
		}
	}
	return true;
}

void cSwampEvent::MonsterDie(LPOBJ lpMon, int pIndex)
{
	if(this->Enabled == 1)
	{
		if(lpMon->MapNumber == MAP_INDEX_SWAMP)
		{
			if( lpMon->m_Attribute == 63 )
			{
				//Count Killed Bosses
				if ( this->MobsStageGroupCount > this->MobsKilled)
				{
					EnterCriticalSection(&this->m_critEventData);
					this->MobsKilled++;
					LeaveCriticalSection(&this->m_critEventData);
				}

				//Delete Monster from the map
				//gObjDel(lpMon->m_Index);
				lpMon->m_Attribute = 60;
			}
		}
	}
}

void cSwampEvent::Timer(int Seconds, BYTE State)
{
	PMSG_TIMER_IMPERIALGUARDIAN Time;
	PHeadSubSetB((LPBYTE)&Time, 0xF7, 0x04, sizeof(Time));

	int Remaining = 0;
	if (State == 0)
	{
		Remaining = SWAMP_STANBYSECONDS - Seconds;
	} else {
		Remaining = (this->GroupTimer[this->CurrentGroup-1] * 60) - Seconds;
	}

	int toTimer=0;
	if(Remaining == 34)
	{
		toTimer = (Remaining * 3.911) + 1;
	}else
	{
		toTimer = Remaining * 3.911;
	}
	Time.Time1 = LOBYTE(toTimer);
	Time.Time2 = HIBYTE(toTimer);
	Time.Time3 = 0;

	Time.MonstersLeft = this->MobsStageGroupCount - this->MobsKilled;
	Time.State = State;

	DataSendInside((BYTE*)&Time,sizeof(Time));
}

void cSwampEvent::DataSendInside(LPBYTE lpMsg, int iMsgSize)
{
	for ( int n = OBJ_STARTUSERINDEX ; n < OBJMAX ; n++)
	{
		if ( gObj[n].Connected == PLAYER_PLAYING )
		{
			if ( gObj[n].Type == OBJ_USER && gObj[n].MapNumber == MAP_INDEX_SWAMP)
			{
				DataSend(n, (unsigned char*)lpMsg , iMsgSize );
			}
		}
	}
}

void cSwampEvent::GCServerMsgStringSend(LPSTR szMsg, BYTE type) 
{
	PMSG_NOTICE pNotice;
	pNotice.type = 0;	// 3
	pNotice.btCount = 0;	// 4
	pNotice.wDelay = 0;	// 6	
	pNotice.dwColor = 0;	// 8
	pNotice.btSpeed = 0;	// C
	
	TNotice::MakeNoticeMsg((TNotice*)&pNotice, type, szMsg);


	for ( int n = OBJ_STARTUSERINDEX ; n < OBJMAX ; n++)
	{
		if ( gObj[n].Connected == PLAYER_PLAYING )
		{
			if ( gObj[n].Type  == OBJ_USER && gObj[n].MapNumber == MAP_INDEX_SWAMP)
			{
				::DataSend(n, (UCHAR*)&pNotice, pNotice.h.size);
			}
		}
	}
}

void cSwampEvent::ClearAllMonsters()
{
	if(this->Enabled == 1)
	{
		for ( int n = 0 ; n < OBJ_STARTUSERINDEX ; n++)
		{
			if ( gObj[n].MapNumber == MAP_INDEX_SWAMP)
			{
				if( gObj[n].m_Attribute == 63 )
				{
					gObjDel(gObj[n].m_Index);
				}
			}
		}
	}
}

void cSwampEvent::SetMonsters(int iAssultType, int iGroup)
{
	if(this->Enabled == 0)
	{
		this->Start = 0;
		return;
	}

	if (g_MapServerManager.CheckMapCanMove(MAP_INDEX_SWAMP) == FALSE )
	{
		this->Enabled = 0;
		this->Start = 0;
		return;
	}

	this->MobsStageGroupCount = 0;
	this->MobsKilled = 0;

	std::map<int, std::vector<SWAMP_EVENTMOBDATA> >::iterator it = this->m_swampMonsterInfo.find(iAssultType);

	if( it == this->m_swampMonsterInfo.end() ) 
		return;

	for( std::vector<SWAMP_EVENTMOBDATA>::iterator it2 = it->second.begin(); it2 != it->second.end(); it2++ )
	{
		std::vector<SWAMP_EVENTMOBDATA>::iterator pMonsterInfo = it2;

		if( pMonsterInfo->Group != iGroup )
			continue;

		LogAddTD("[Swamp Event][AT:%d,GR:%d] - Adding Monsters ID: %d / QTY: %d",
			iAssultType,iGroup,pMonsterInfo->Number,pMonsterInfo->Count
		);

		for( int i = 0; i < pMonsterInfo->Count; i++ )
		{
			BYTE cX, cY;

			if( ::gObjGetRandomFreeArea(MAP_INDEX_SWAMP, cX, cY, pMonsterInfo->XF, pMonsterInfo->YF, pMonsterInfo->XT, pMonsterInfo->YT, 100) == TRUE )
			{
				int iMobIndex = gObjAddMonster(MAP_INDEX_SWAMP);

				if( iMobIndex >= 0 )
				{
					gObj[iMobIndex].m_PosNum = -1;
					gObj[iMobIndex].X = cX;
					gObj[iMobIndex].Y = cY;
					gObj[iMobIndex].MapNumber = MAP_INDEX_SWAMP;
					gObj[iMobIndex].TX = gObj[iMobIndex].X;
					gObj[iMobIndex].TY = gObj[iMobIndex].Y;
					gObj[iMobIndex].m_OldX = gObj[iMobIndex].X;
					gObj[iMobIndex].m_OldY = gObj[iMobIndex].Y;
					gObj[iMobIndex].Dir = 1;
					gObj[iMobIndex].StartX = gObj[iMobIndex].X;
					gObj[iMobIndex].StartY = gObj[iMobIndex].Y;

					gObjSetMonster(iMobIndex, pMonsterInfo->Number,"cSwampEvent::SetMonsters");

					gObj[iMobIndex].m_Attribute = 63;
					gObj[iMobIndex].Dir = ( rand() % 8 );
					gObj[iMobIndex].DieRegen = 0;
					gObj[iMobIndex].RegenTime = 1;
					gObj[iMobIndex].MaxRegenTime = 1000;
					gObj[iMobIndex].m_dwLastCheckTick = GetTickCount();

					gObj[iMobIndex].m_MoveRange = 5;
					gObj[iMobIndex].m_ViewRange = 15;

					this->MobsStageGroupCount++;

					if (this->MobsStageGroupCount >= SWAMP_MOBSTOKILLCOUNT)
					{
						LogAddTD("[Swamp Event][ERROR][AT:%d,GR:%d] Monster ammount is over",
							iAssultType,iGroup
						);
						return;
					}
				} else {
					LogAddTD("[Swamp Event][ERROR][AT:%d,GR:%d] gObjAddMonster returned: %d",
							iAssultType,iGroup,
							iMobIndex
						);
				}
			} else {
				LogAddTD("[Swamp Event][ERROR][AT:%d,GR:%d][%d] - Fail Getting Location for: %d [%d,%d,%d,%d]",
					iAssultType,iGroup,i, 
					pMonsterInfo->Number,
					pMonsterInfo->XF, pMonsterInfo->YF,
					pMonsterInfo->XT, pMonsterInfo->YT
				);
			}
		}
	}
}

void cSwampEvent::StartEvent()
{
	if(this->Enabled == 1)
	{
		if (this->Start == TRUE)
			this->Start = FALSE;

		_beginthread( cSwampEvent__InsideTrigger, 0, NULL  );
	}
}

void cSwampEvent::WarpOutside() 
{
	for ( int n = OBJ_STARTUSERINDEX ; n < OBJMAX ; n++)
	{
		if ( gObj[n].Connected == PLAYER_PLAYING )
		{
			if ( gObj[n].Type  == OBJ_USER )
			{
				if (gObj[n].MapNumber == MAP_INDEX_SWAMP)
				{
					BOOL ret = gObjMoveGate(gObj[n].m_Index, this->SwampWarpGate);

					if (ret = false)
					{
						CloseClient(n);
					}
				}
			}
		}
	}
	LogAddC(12, "[Swamp Event] State -> FINISH: REMOVE_USERS");
}

void cSwampEvent__InsideTrigger(void *  lpParam)
{
	int Seconds = 0;
	char sBuf[512]={0};
	
	for(int i=6;i>0;i--)
	{
		if (i == 6)
		{
			LogAddCTD(3, lMsg.Get(MSGGET(14, 124)));
			g_Swamp.SendMessageToAllGS(lMsg.Get(MSGGET(14, 124)));
		}
		else
		{
			wsprintf(sBuf,lMsg.Get(MSGGET(14, 125)),i);
			LogAddCTD(3, sBuf);
			g_Swamp.SendMessageToAllGS(sBuf);
		}
		Sleep(60000);
	}
	//g_Swamp.GCServerMsgStringSend(lMsg.Get(MSGGET(14, 130)),1);

	BOOL Activate = FALSE;
	BYTE Overflow = 0;
	BYTE Success = 0;

	int roomCalculation = 0;
	roomCalculation = (rand()%(SWAMP_ROOMS*100))/100;
	g_Swamp.CurrentRoom = roomCalculation;
	g_Swamp.CurrentGroup = 1;
	g_Swamp.Start = TRUE;

	while (Activate == FALSE && Overflow < 10)
	{
		g_Swamp.CurrentRoom = (rand()%SWAMP_ROOMS) + 1;

		if (g_Swamp.pRoom[g_Swamp.CurrentRoom - 1].Active == 1)
			Activate = TRUE;

		Overflow++;
	}

	memset(sBuf,0,sizeof(sBuf));
	wsprintf(sBuf, lMsg.Get(MSGGET(14, 132)), g_Swamp.pRoom[g_Swamp.CurrentRoom - 1].Territory);
	g_Swamp.SendMessageToAllGS(sBuf);
	g_Swamp.SendMessageToAllGS("Find Medusa troops and destroy them!");
	g_Swamp.SetMonsters(g_Swamp.CurrentRoom,g_Swamp.CurrentGroup);

	while(Activate)
	{
		if (g_Swamp.Start == FALSE)
		{
			Activate = FALSE;
			Success = 0;
			break;
		}

		g_Swamp.Timer(Seconds, 1);
		if(Seconds >= (g_Swamp.GroupTimer[g_Swamp.CurrentGroup-1] * 60))
		{
			//time exceed
			LogAddC(13, "[Swamp Event] State -> FAIL: TIME EXCEED");
			g_Swamp.GCServerMsgStringSend(lMsg.Get(MSGGET(14, 134)),0);
			break;
		}
		
		//Check Finish
		if( g_Swamp.MobsKilled >= g_Swamp.MobsStageGroupCount )
		{
			LogAddC(13, "[Swamp Event] Room:%d Group:%d Defeated -> SUCCESS", g_Swamp.CurrentRoom, g_Swamp.CurrentGroup);
			g_Swamp.CurrentGroup++;

			if (g_Swamp.pRoom[g_Swamp.CurrentRoom-1].Groups >= g_Swamp.CurrentGroup &&
				g_Swamp.GroupTimer[g_Swamp.CurrentGroup-1] > 0)
			{
				g_Swamp.GCServerMsgStringSend(lMsg.Get(MSGGET(14, 130)),0);
				Seconds = 0;

				while( true )
				{
					g_Swamp.Timer(Seconds, 0);
					Seconds++;
					Sleep(1000);

					if (Seconds == SWAMP_STANBYSECONDS)
						break;
				}

				g_Swamp.SetMonsters(g_Swamp.CurrentRoom,g_Swamp.CurrentGroup);
				g_Swamp.GCServerMsgStringSend(lMsg.Get(MSGGET(14, 131)),0);
			} else {
				Success = 1;
				g_Swamp.GCServerMsgStringSend(lMsg.Get(MSGGET(14, 133)),0);	
				Sleep(5000);
				break;
			}
		}

		if (g_Swamp.MobsStageGroupCount <= 0)
		{
			Success = 1;
			LogAddC(13, "[Swamp Event] Room:%d Group:%d Defeated -> MOBCOUNT_ERROR -> SUCCESS", g_Swamp.CurrentRoom, g_Swamp.CurrentGroup);

			g_Swamp.GCServerMsgStringSend(lMsg.Get(MSGGET(14, 133)),0);	
			Sleep(5000);
			break;
		}
		
		Seconds++;
		Sleep(1000);
	}

	LogAddC(13, "[Swamp Event] State -> PRECLOSURE: Monster Clean");
	g_Swamp.ClearAllMonsters();

	if (g_Swamp.SwampMapEnterOnlyWhenStarted == 1)
	{
		if (Success == 1 && g_Swamp.SwampIfWonKeepOpenMapMinutes > 0)
		{
			int secondsopen = 0;
			secondsopen = g_Swamp.SwampIfWonKeepOpenMapMinutes*60*60;

			memset(sBuf,0,sizeof(sBuf));
			wsprintf(sBuf,lMsg.Get(MSGGET(13, 63)),"Swamp",g_Swamp.SwampIfWonKeepOpenMapMinutes);
			LogAddCTD(3, sBuf);
			g_Swamp.SendMessageToAllGS(sBuf);

			LogAddC(13, "[Swamp Event] State -> PRECLOSURE: Keep Map Open");

			while(secondsopen > 0)
			{
				if (secondsopen == 300 ||
					secondsopen == 240 ||
					secondsopen == 180 ||
					secondsopen == 120 ||
					secondsopen == 60 
					)
				{
					memset(sBuf,0,sizeof(sBuf));
					wsprintf(sBuf,lMsg.Get(MSGGET(13, 64)),"Swamp",secondsopen/60);
					LogAddCTD(3, sBuf);
					g_Swamp.SendMessageToAllGS(sBuf);
				}	

				secondsopen--;
				Sleep(1000);
			}
		}

		LogAddC(13, "[Swamp Event] State -> PRECLOSURE: Prepare kick players");

		if (Activate == TRUE)
		{
			//Final Warp Out Message
			memset(sBuf,0,sizeof(sBuf));
			wsprintf(sBuf, lMsg.Get(MSGGET(14, 127)), SWAMP_STANBYSECONDS);
			g_Swamp.GCServerMsgStringSend(sBuf,1);
			Sleep(SWAMP_STANBYSECONDS*1000);
		}
		g_Swamp.WarpOutside();
	}

	LogAddC(13, "[Swamp Event] State -> CLOSED");
	g_Swamp.Clean();

	_endthread();
}

#endif
#endif