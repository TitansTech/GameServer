// ------------------------------
// Decompiled by Deathway
// Date : 2007-05-09
// ------------------------------
#include "stdafx.h"
#include "EledoradoEvent.h"
#include "Gamemain.h"
#include "LogProc.h"
#include "user.h"
// GS-N 0.99.60T 0x0045BAC0
//	GS-N	1.00.18	JPN	0x0046B3C0	-	Completed

CEledoradoEvent gEledoradoEvent;



CEledoradoEvent::CEledoradoEvent()
{
	this->GoldGoblenEventStartTime = 0;
	this->TitanEventStartTime = 0;
	this->GoldDerconEventStartTime = 0;
	this->DevilLizardKingEventStartTime = 0;
	this->KanturEventStartTime = 0;
	this->m_BossGoldDerconMapNumber[0] = -1;
	this->m_BossGoldDerconMapNumber[1] = -1;
	this->m_BossGoldDerconMapNumber[2] = -1;
}




CEledoradoEvent::~CEledoradoEvent()
{
	return;
}





void CEledoradoEvent::Init()
{
	return;
}




void CEledoradoEvent::SetEventState(int State)
{
	this->EventState = State;
}


int CEledoradoEvent::GetEventState()
{
	return this->EventState;
}

void CEledoradoEvent::Run()
{
	if ( this->m_bMenualStart != FALSE )
	{
		return;
	}

	if ( this->EventState == 0 )
	{
		return;
	}

	if ( this->GoldGoblenEventStartTime == 0 )
	{
		tm * today;
		time_t ltime;

		time(&ltime);
		today = localtime(&ltime);

		if ( today->tm_min >= 30 && today->tm_min  <= 35 )
		{
			int Time = GetTickCount();

			this->GoldGoblenEventStartTime = Time;
			this->RegenGoldGoblen();

			this->TitanEventStartTime = Time;
			this->RegenTitan();

			this->GoldDerconEventStartTime = Time;
			this->RegenGoldDercon();

			this->DevilLizardKingEventStartTime = Time;
			this->RegenDevilLizardKing();

			this->KanturEventStartTime = Time;
			this->RegenKantur();

			for(int i=0;i<10;i++)
			{
				this->m_BossElDorado2MapNumber[i] = Time;
				this->RegenElDorado2(i);
			}

			AllSendServerMsg(lMsg.Get(MSGGET(4, 134)));
		}
	}
	else
	{
		DWORD Time = GetTickCount();

		if ( ( Time - this->GoldGoblenEventStartTime ) > gEledoradoGoldGoblenRegenTime * 60000 )
		{
			this->GoldGoblenEventStartTime = Time;
			this->RegenGoldGoblen();			
			AllSendServerMsg(lMsg.Get(MSGGET(4, 134)));
		}

		if ( ( Time - this->TitanEventStartTime ) > gEledoradoTitanRegenTime * 60000 )
		{
			this->TitanEventStartTime = Time;
			this->RegenTitan();
			AllSendServerMsg(lMsg.Get(MSGGET(4, 134)));
		}

		if ( ( Time - this->GoldDerconEventStartTime ) > gEledoradoGoldDerconRegenTime * 60000 )
		{
			this->GoldDerconEventStartTime = Time;
			this->RegenGoldDercon();
			AllSendServerMsg(lMsg.Get(MSGGET(4, 134)));
		}

		if ( ( Time - this->DevilLizardKingEventStartTime ) > gEledoradoDevilLizardKingRegenTime * 60000 )
		{
			this->DevilLizardKingEventStartTime = Time;
			this->RegenDevilLizardKing();
			AllSendServerMsg(lMsg.Get(MSGGET(4, 134)));
		}

		if ( ( Time - this->KanturEventStartTime ) > gEledoradoDevilTantarosRegenTime * 60000 )
		{
			this->KanturEventStartTime = Time;
			this->RegenKantur();
			AllSendServerMsg(lMsg.Get(MSGGET(4, 134)));
		}
		for(int i=0;i<10;i++)
		{
			if ( ( Time - this->BossElDorado2StartTime[i] ) > gEledorado2EventRegenTime[i] * 60000 )
			{
				this->BossElDorado2StartTime[i] = Time;
				this->RegenElDorado2(i);
				AllSendServerMsg(lMsg.Get(MSGGET(4, 134)));
			}
		}
	}
}

void CEledoradoEvent::RegenGoldGoblen()
{
	int n=0;
	int MapNumber;
	int Map[2]={0,3};

	for (n=0;n<OBJ_MAXMONSTER;n++)
	{
		if ( gObj[n].Class == 78 )
		{
			gObj[n].Live = TRUE;
			MapNumber = Map[rand()%2];
			gObj[n].MapNumber = MapNumber;

			while ( gMSetBase.GetBoxPosition(MapNumber, 50, 50, 200, 200, gObj[n].X, gObj[n].Y) == 0 )
			{

			}

			gObj[n].Life = gObj[n].MaxLife;
			gObj[n].TX = gObj[n].X;
			gObj[n].TY = gObj[n].Y;
			gObj[n].MTX = gObj[n].X;
			gObj[n].MTY = gObj[n].Y;
			gObj[n].StartX = gObj[n].X;
			gObj[n].StartY = gObj[n].Y;
				//gObj[n].m_OldX = gObj[n].X;
				//gObj[n].m_OldY = gObj[n].Y;
			gObj[n].m_State = 1;
			gObj[n].PathCount = 0;

			LogAddTD("[Eldorado Event] Make GoldGoblen : %d, %d,%d",
				MapNumber, gObj[n].X, gObj[n].Y);
		}
	}
}



void CEledoradoEvent::RegenTitan()
{
	int n=0;
	int MapNumber;
//	int Map[2]={0,3};

	for (n=0;n<OBJ_MAXMONSTER;n++)
	{
		if ( gObj[n].Class == 53 )	// Golden Titan
		{
			gObj[n].Live = TRUE;
			MapNumber = 2;
			gObj[n].MapNumber = MapNumber;

			while ( gMSetBase.GetBoxPosition(MapNumber, 50, 50, 200, 200, gObj[n].X, gObj[n].Y) == 0 )
			{

			}

			this->m_BossTitanMapNumber = gObj[n].MapNumber;
			this->m_BossTitanMapX = gObj[n].X;
			this->m_BossTitanMapY = gObj[n].Y;

			gObj[n].Life = gObj[n].MaxLife;
			gObj[n].TX = gObj[n].X;
			gObj[n].TY = gObj[n].Y;
			gObj[n].MTX = gObj[n].X;
			gObj[n].MTY = gObj[n].Y;
			gObj[n].StartX = gObj[n].X;
			gObj[n].StartY = gObj[n].Y;
				//gObj[n].m_OldX = gObj[n].X;
				//gObj[n].m_OldY = gObj[n].Y;

			gObj[n].m_State = 1;
			gObj[n].PathCount = 0;

			LogAddTD("[Eldorado Event] Make GoldTaitan : %d, %d,%d",
				MapNumber, gObj[n].X, gObj[n].Y);
		}
		else if ( gObj[n].Class == 54 ) // Golden Soldier
		{
			gObj[n].Live = TRUE;
			MapNumber = this->m_BossTitanMapNumber;
			gObj[n].MapNumber = MapNumber;

			gMSetBase.GetBoxPosition(MapNumber, this->m_BossTitanMapX-4, this->m_BossTitanMapY-4,
				this->m_BossTitanMapX+4, this->m_BossTitanMapY+4, gObj[n].X, gObj[n].Y);

						gObj[n].Life = gObj[n].MaxLife;
			gObj[n].TX = gObj[n].X;
			gObj[n].TY = gObj[n].Y;
			gObj[n].MTX = gObj[n].X;
			gObj[n].MTY = gObj[n].Y;
			gObj[n].StartX = gObj[n].X;
			gObj[n].StartY = gObj[n].Y;
				//gObj[n].m_OldX = gObj[n].X;
				//gObj[n].m_OldY = gObj[n].Y;
			gObj[n].m_State = 1;
			gObj[n].PathCount = 0;
		}
	}
}


void CEledoradoEvent::RegenGoldDercon()
{
	int n=0;
	int MapNumber=-1;
	int Map[3]={0,3,2};
	int SelMap = -1;
	int count = 0;

	this->CheckGoldDercon(MapNumber);	// #error??? MapNumber hasn't been setted

	for (n=0;n<OBJ_MAXMONSTER;n++)
	{
		if ( gObj[n].Class == 79 )	// Gold Dercon
		{
			gObj[n].Live = TRUE;

			if ( SelMap == -1 )	
			{
				MapNumber = Map[rand()%3];
			}
			else
			{
				MapNumber = SelMap;
			}

			gObj[n].MapNumber = MapNumber;

			while ( gMSetBase.GetBoxPosition(MapNumber, 80, 80, 170, 170, gObj[n].X, gObj[n].Y) == 0 )
			{
				//Do nothing here
			}

			BYTE attr = MapC[MapNumber].GetAttr(gObj[n].X, gObj[n].Y);

			while ( (attr&1) == 1 )
			{
				while ( gMSetBase.GetBoxPosition(MapNumber, 80, 80, 170, 170, gObj[n].X, gObj[n].Y) == 0 )
				{
					//Do nothing here
				}
				attr = MapC[MapNumber].GetAttr(gObj[n].X, gObj[n].Y);
			}

			gObj[n].Life = gObj[n].MaxLife;
			gObj[n].TX = gObj[n].X;
			gObj[n].TY = gObj[n].Y;
			gObj[n].MTX = gObj[n].X;
			gObj[n].MTY = gObj[n].Y;
			gObj[n].StartX = gObj[n].X;
			gObj[n].StartY = gObj[n].Y;
				//gObj[n].m_OldX = gObj[n].X;
				//gObj[n].m_OldY = gObj[n].Y;

			gObj[n].m_State = 1;
			gObj[n].PathCount = 0;

			LogAddTD("[Eldorado Event] Make GoldDercon : %d, %d,%d",
				MapNumber, gObj[n].X, gObj[n].Y);

			if ( SelMap == -1 )
			{
				char szTemp[256];
				strcpy(szTemp, " ");

				switch ( MapNumber )
				{
					case 0:
						strcat(szTemp, lMsg.Get(MSGGET(7, 208)));
						break;
					case 2:
						strcat(szTemp, lMsg.Get(MSGGET(7, 210)));
						break;
					case 3:
						strcat(szTemp, lMsg.Get(MSGGET(7, 211)));
						break;
				}

				strcat(szTemp, lMsg.Get(MSGGET(4, 134)));
			}

			this->m_BossGoldDerconMapNumber[count] = MapNumber;
			gObj[n].m_BossGoldDerconMapNumber = MapNumber;
			count++;
			SelMap = MapNumber;
		}
	}

	this->CheckGoldDercon(MapNumber);
}



void CEledoradoEvent::RegenDevilLizardKing()
{
	int n=0;
	int MapNumber;
//	int Map[2]={0,3};

	for (n=0;n<OBJ_MAXMONSTER;n++)
	{
		if ( gObj[n].Class == 80 )	// Devil Lizard King
		{
			gObj[n].Live = TRUE;
			MapNumber = 7;
			gObj[n].MapNumber = MapNumber;

			while ( gMSetBase.GetBoxPosition(MapNumber, 50, 50, 200, 200, gObj[n].X, gObj[n].Y) == 0 )
			{

			}

			this->m_BossDevilLizardKingMapNumber = gObj[n].MapNumber;
			this->m_BossDevilLizardKingMapX = gObj[n].X;
			this->m_BossDevilLizardKingMapY = gObj[n].Y;

			gObj[n].Life = gObj[n].MaxLife;
			gObj[n].TX = gObj[n].X;
			gObj[n].TY = gObj[n].Y;
			gObj[n].MTX = gObj[n].X;
			gObj[n].MTY = gObj[n].Y;
			gObj[n].StartX = gObj[n].X;
			gObj[n].StartY = gObj[n].Y;
				//gObj[n].m_OldX = gObj[n].X;
				//gObj[n].m_OldY = gObj[n].Y;

			gObj[n].m_State = 1;
			gObj[n].PathCount = 0;

			LogAddTD("[Eldorado Event] Make GoldLizarKing : %d, %d,%d",
				MapNumber, gObj[n].X, gObj[n].Y);
		}
		else if ( gObj[n].Class == 81 ) // Golden Velparie
		{
			gObj[n].Live = TRUE;
			MapNumber = this->m_BossDevilLizardKingMapNumber;
			gObj[n].MapNumber = MapNumber;

			gMSetBase.GetBoxPosition(MapNumber, this->m_BossDevilLizardKingMapX-4, this->m_BossDevilLizardKingMapY-4,
				this->m_BossDevilLizardKingMapX+4, this->m_BossDevilLizardKingMapY+4, gObj[n].X, gObj[n].Y);

			gObj[n].Life = gObj[n].MaxLife;
			gObj[n].TX = gObj[n].X;
			gObj[n].TY = gObj[n].Y;
			gObj[n].MTX = gObj[n].X;
			gObj[n].MTY = gObj[n].Y;
			gObj[n].StartX = gObj[n].X;
			gObj[n].StartY = gObj[n].Y;
				//gObj[n].m_OldX = gObj[n].X;
				//gObj[n].m_OldY = gObj[n].Y;
			gObj[n].m_State = 1;
			gObj[n].PathCount = 0;
		}
	}
}

void CEledoradoEvent::RegenKantur()
{
	int n=0;
	int MapNumber;
//	int Map[2]={0,3};

	for (n=0;n<OBJ_MAXMONSTER;n++)
	{
		if ( gObj[n].Class == 82 )	// Gold Tantalos
		{
			gObj[n].Live = TRUE;
			MapNumber = 8;
			gObj[n].MapNumber = MapNumber;

			while ( gMSetBase.GetBoxPosition(MapNumber, 50, 50, 200, 200, gObj[n].X, gObj[n].Y) == 0 )
			{

			}

			this->m_BossKanturMapNumber = gObj[n].MapNumber;
			this->m_BossKanturMapX = gObj[n].X;
			this->m_BossKanturMapY = gObj[n].Y;

			gObj[n].Life = gObj[n].MaxLife;
			gObj[n].TX = gObj[n].X;
			gObj[n].TY = gObj[n].Y;
			gObj[n].MTX = gObj[n].X;
			gObj[n].MTY = gObj[n].Y;
			gObj[n].StartX = gObj[n].X;
			gObj[n].StartY = gObj[n].Y;
				//gObj[n].m_OldX = gObj[n].X;
				//gObj[n].m_OldY = gObj[n].Y;

			gObj[n].m_PoisonBeattackCount = 0;
			gObj[n].m_ColdBeattackCount = 0;
			//gObj[n].m_ViewState = 0;
			gObj[n].Teleport = 0;
			gObj[n].m_State = 1;
			gObj[n].PathCount = 0;

			LogAddTD("[Eldorado Event] Make GoldTantarus : %d, %d,%d",
				MapNumber, gObj[n].X, gObj[n].Y);
		}
		else if ( gObj[n].Class == 83 ) // Gold Iron Wheel
		{
			gObj[n].Live = TRUE;
			MapNumber = this->m_BossKanturMapNumber;
			gObj[n].MapNumber = MapNumber;

			gMSetBase.GetBoxPosition(MapNumber, this->m_BossKanturMapX-10, this->m_BossKanturMapY-10,
				this->m_BossKanturMapX+10, this->m_BossKanturMapY+10, gObj[n].X, gObj[n].Y);

			gObj[n].Life = gObj[n].MaxLife;
			gObj[n].TX = gObj[n].X;
			gObj[n].TY = gObj[n].Y;
			gObj[n].MTX = gObj[n].X;
			gObj[n].MTY = gObj[n].Y;
			gObj[n].StartX = gObj[n].X;
			gObj[n].StartY = gObj[n].Y;
				//gObj[n].m_OldX = gObj[n].X;
				//gObj[n].m_OldY = gObj[n].Y;

			gObj[n].m_State = 1;
			gObj[n].PathCount = 0;
		}
	}
}



void CEledoradoEvent::CheckGoldDercon(int MapNumber)
{
	if ( this->EventState == 0 )
		return;

	BOOL EventOn = FALSE;
	int EventClearMapNumber = -1;

	for ( int i=0;i<3;i++)
	{
		if ( this->m_BossGoldDerconMapNumber[i] != -1 )
		{
			if ( this->m_BossGoldDerconMapNumber[i] == MapNumber )
			{
				EventOn = TRUE;
			}
			else
			{
				EventClearMapNumber = this->m_BossGoldDerconMapNumber[i];
			}
		}
	}

	if ( EventClearMapNumber != -1 )
	{
		GCMapEventStateSend(EventClearMapNumber, 0, 3);
	}

	if ( EventOn != FALSE )
	{
		GCMapEventStateSend(MapNumber, 1, 3);
	}
	else
	{
		GCMapEventStateSend(MapNumber, 0, 3);
	}
}



void CEledoradoEvent::Start_Menual()
{
	this->SetMenualStart(TRUE);

	LogAddTD("[Event Management] [Start] EledoradoEvent Event!");

	DWORD TickCount = GetTickCount();

	this->GoldGoblenEventStartTime = TickCount;
	this->RegenGoldGoblen();

	this->TitanEventStartTime = TickCount;
	this->RegenTitan();

	this->GoldDerconEventStartTime = TickCount;
	this->RegenGoldDercon();

	this->DevilLizardKingEventStartTime = TickCount;
	this->RegenDevilLizardKing();

	this->KanturEventStartTime = TickCount;
	this->RegenKantur();
	for(int i=0;i<10;i++)
	{
		this->BossElDorado2StartTime[i] = TickCount;
		this->RegenElDorado2(i);
	}
	AllSendServerMsg(lMsg.Get(MSGGET(4, 134)));
	

}
void CEledoradoEvent::End_Menual()
{
	this->SetMenualStart(FALSE);
}



//
void CEledoradoEvent::RegenElDorado2(int Num)
{
	if(gIsEledorado2Event == TRUE && ReadConfig.S5E2 == TRUE)
	{
		int n=0;
		int MapNumber;

		int MobClass = 493 + Num;

		for (n=0;n<OBJ_MAXMONSTER;n++)
		{
			if ( gObj[n].Class == MobClass )
			{
				gObj[n].Live = TRUE;
				MapNumber = gObj[n].MapNumber;

				while ( gMSetBase.GetBoxPosition(MapNumber, 50, 50, 200, 200, gObj[n].X, gObj[n].Y) == 0 )
				{

				}

				this->m_BossElDorado2MapNumber[Num] = gObj[n].MapNumber;
				this->m_BossElDorado2MapX[Num] = gObj[n].X;
				this->m_BossElDorado2MapY[Num] = gObj[n].Y;

				gObj[n].Life = gObj[n].MaxLife;
				gObj[n].TX = gObj[n].X;
				gObj[n].TY = gObj[n].Y;
				gObj[n].MTX = gObj[n].X;
				gObj[n].MTY = gObj[n].Y;
				gObj[n].StartX = gObj[n].X;
				gObj[n].StartY = gObj[n].Y;
					//gObj[n].m_OldX = gObj[n].X;
					//gObj[n].m_OldY = gObj[n].Y;

				gObj[n].m_State = 1;
				gObj[n].PathCount = 0;

				LogAddTD("[Eldorado Event 2] Make Monster Class : %d, %d, %d,%d",
					gObj[n].Class, MapNumber, gObj[n].X, gObj[n].Y);
			}
		}
	}
}