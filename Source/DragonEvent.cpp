// ------------------------------
// Decompiled by Deathway
// Date : 2007-05-09
// ------------------------------
// GS-N 0.99.60T 45a900
// GS-N	1.00.18	JPN	0x0046A1F0	-	Completed
#include "stdafx.h"
#include "DragonEvent.h"
#include "GameMain.h"
#include "logproc.h"
#include "Protocol.h"

CDragonEvent::CDragonEvent()
{
	this->Init();
}



CDragonEvent::~CDragonEvent()
{
	return;
}


void CDragonEvent::Init()
{
	this->EventState=0;
	this->EventStartTime =0;
	this->m_MapNumber =0;
	this->m_bMenualStart =0;
}

void CDragonEvent::Start()
{
	char szTemp[256];
	char map[3];

	if ( this->EventState != 0 )
	{
		return;
	}

	this->Init();

	map[0]=MAP_INDEX_LORENCIA;
	map[1]=MAP_INDEX_DEVIAS;
	map[2]=MAP_INDEX_NORIA;

	this->EventState = 1;
	this->m_MapNumber  = map [ rand()%3 ];

	strcpy(szTemp, "¡Ù ");

	switch ( this->m_MapNumber  )
	{
		case MAP_INDEX_LORENCIA:
			strcat(szTemp, lMsg.Get(MSGGET(7, 208)) );
			break;

		case MAP_INDEX_DEVIAS:
			strcat(szTemp, lMsg.Get(MSGGET(7, 210)) );
			break;

		case MAP_INDEX_NORIA:
			strcat(szTemp, lMsg.Get(MSGGET(7, 211)) );
			break;

	}
	
	strcat(szTemp, lMsg.Get(MSGGET(4, 132)) );
	strcat(szTemp, " ¡Ù");

	AllSendServerMsg(lMsg.Get(MSGGET(4, 132)));
	this->EventStartTime  = GetTickCount();
}
	
	
void CDragonEvent::End()
{
	GCMapEventStateSend(this->m_MapNumber, 0, 1 );
	LogAdd("Event 1 End");
	this->EventState=0;
}


void CDragonEvent::DragonActive()
{
	for ( int n = 0; n<OBJ_MAXMONSTER ; n++)
	{
		if ( gObj[n].Class == 44 )
		{
			gObj[n].Live = TRUE;
			gObj[n].MapNumber = this->m_MapNumber;

			switch ( this->m_MapNumber )
			{
				case MAP_INDEX_LORENCIA:
					if ( (rand()%3) == 0 )
					{
						gMSetBase.GetBoxPosition(this->m_MapNumber, 135, 61, 146, 70, gObj[n].X, gObj[n].Y);
					}
					else if ( (rand()%3) == 1 )
					{
						gMSetBase.GetBoxPosition(this->m_MapNumber, 120, 204, 126, 219, gObj[n].X, gObj[n].Y);
					}
					else
					{
						gMSetBase.GetBoxPosition(this->m_MapNumber, 67, 116, 77, 131, gObj[n].X, gObj[n].Y);
					}

					break;

				case MAP_INDEX_DEVIAS:
					
					if ( (rand()%2) == 0 )
					{
						gMSetBase.GetBoxPosition(this->m_MapNumber, 155, 16, 169, 55, gObj[n].X, gObj[n].Y);
					}
					else
					{
						gMSetBase.GetBoxPosition(this->m_MapNumber, 193, 110, 232, 126, gObj[n].X, gObj[n].Y);
					}

					break;

				case MAP_INDEX_NORIA:

					if ( (rand()%2) == 0 )
					{
						gMSetBase.GetBoxPosition(this->m_MapNumber, 198, 161, 223, 175, gObj[n].X, gObj[n].Y);
					}
					else
					{
						gMSetBase.GetBoxPosition(this->m_MapNumber, 136, 50, 164, 59, gObj[n].X, gObj[n].Y);
					}

					break;
			}

			gObj[n].m_State = 1;
		}
	}
}





void CDragonEvent::Run()
{
	if ( this->m_bMenualStart != FALSE )
	{
		return;
	}

	if ( this->EventState == 0 )
	{
		return;
	}

	if ( this->EventState == 1 )
	{
		if ( ( GetTickCount() - this->EventStartTime ) > 3000 )
		{
			this->EventState = 2;
			GCMapEventStateSend(this->m_MapNumber , 1, 1 );
			LogAdd("Event Start 1");
			this->DragonActive();
			this->EventStartTime = GetTickCount();
			return;
		}
	}
	else
	{
		if ( this->EventState == 2 )
		{
			if ( (GetTickCount() - this->EventStartTime ) > 300000 )
			{
				this->End();
			}
		}
	}
}




void CDragonEvent::Start_Menual()
{
	this->SetMenualStart(TRUE);
	LogAddTD("[Event Management] [Start] Dragon Event!");
	this->EventState=0;
	this->Start();
}


			

void CDragonEvent::End_Menual()
{
	this->SetMenualStart(FALSE);
	this->End();
}
