#include "stdafx.h"
#include "GameServer.h"
#include "LogProc.h"
#include "GameMain.h"
#include "TNotice.h"
#include "..\include\readscript.h"
#include "gObjMonster.h"
#include "RainItemEvent.h"
#include "Event.h"
#include "..\common\winutil.h"

#if (PACK_EDITION>=3)

cRainItemEvent RainItemEvent;

void cRainItemEvent__InsideTrigger(void * lpParam)
{
	RainItemEvent.Start = true;

	LogAddTD("[RainItemEvent] Event Start");
	AllSendServerMsg(lMsg.Get(MSGGET(14, 155)));


	for (int i = 0; i < RainItemEvent.posCount; i++)
	{
		RainEventItemBoxOpen(RainItemEvent.pos[i].map, RainItemEvent.pos[i].x , RainItemEvent.pos[i].y);
		Sleep(1500);
	}

	LogAddTD("[RainItemEvent] Event End");
	AllSendServerMsg(lMsg.Get(MSGGET(14, 156)));

	RainItemEvent.Start = false;
	_endthread();
}

void cRainItemEvent::StartEvent()
{
	if(this->Start == false)
		_beginthread( cRainItemEvent__InsideTrigger, 0, NULL  );
}

void cRainItemEvent::ReadPositions(char * FilePath)
{
	int Token;
	this->posCount = 0;

	for (int i = 0; i < cRainItemEvent_DropItemPos_COUNT; i++)
	{
		this->pos[i].map = 0;
		this->pos[i].x = 0;
		this->pos[i].y = 0;
	}

	SMDFile = fopen(FilePath, "r");

	if ( SMDFile == NULL )
	{
		MsgBox("RainItemEvent data load error %s", FilePath);
		return;
	}

	while ( true )
	{
		int iType = GetToken();
		
		if ( iType == 1 )
		{
			while(true)
			{
				Token = GetToken();
				if ( strcmp("end", TokenString) == 0 )
				{
					break;
				}
				this->pos[this->posCount].map = TokenNumber;

				Token = GetToken();
				this->pos[this->posCount].x = TokenNumber;

				Token = GetToken();
				this->pos[this->posCount].y = TokenNumber;

				this->posCount++;
			}
		}
		break;
	}
		
	fclose(SMDFile);
	LogAddTD("[RainItemEvent] - %s file is Loaded",FilePath);
}

#endif