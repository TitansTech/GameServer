#include "stdafx.h"
#include "XMasEvent.h"
#include "GameServer.h"
#include "GameMain.h"
#include "LogProc.h"
#include "GameMain.h"
#include "TNotice.h"
#include "..\include\readscript.h"
#include "gObjMonster.h"
//#include "GameServerAuth.h"
#include "Event.h"
#include "..\common\winutil.h"

#if (PACK_EDITION>=2)

cXMasEvent XMasEvent;

void cXMasEvent::StartEvent()
{
	if((this->Start == 0) && (this->Enabled == 1))
		_beginthread( cXMasEvent__InsideTrigger, 0, NULL  );
}

void cXMasEvent::Init(char * FilePath)
{
	this->gXMasEvent			= GetPrivateProfileInt("WZ_XMas", "SCFWZXMasEvent", 0, FilePath) ;
	this->gHappyNewYearNpcEvent	= GetPrivateProfileInt("WZ_XMas", "SCFWZHappyNewYearTalkNpc", 0, FilePath) ;
	this->gMerryXMasNpcEvent	= GetPrivateProfileInt("WZ_XMas", "SCFWZMerryXMasTalkNpc", 0, FilePath) ;

	this->g_bRibbonBoxEvent		= GetPrivateProfileInt("WZ_XMas","SCFWZRibbonBoxEvent",0, FilePath) ;

	if(this->gMerryXMasNpcEvent != 0)
		LogAddTD(lMsg.Get(MSGGET(2, 60)));
	if(this->gHappyNewYearNpcEvent != 0)
		LogAddTD(lMsg.Get(MSGGET(2, 61)));

	this->Start					= 0;
	this->Enabled				= GetPrivateProfileInt("SCF_XMas", "SCFXMasEventEnabled",0, FilePath) ;

	if (this->Enabled == 1)
	{
		this->AllowInvitation		= 1;
		this->AllowGifts			= 1;
	} else {
		this->AllowInvitation		= GetPrivateProfileInt("SCF_XMas", "SCFXMasAllowInvitationWhenDisabled",0, FilePath) ;
		this->AllowGifts			= GetPrivateProfileInt("SCF_XMas", "SCFXMasAllowGiftsWhenDisabled",0, FilePath) ;
	}

	this->Minutes				= GetPrivateProfileInt("SCF_XMas", "SCFXMasEventMinutes",5, FilePath) ;
	this->SantaTicketDropPer	= GetPrivateProfileInt("SCF_XMas", "SCFXMasEventSantaTicketDropPer",30, FilePath) ;
	this->SnowManTicketDropPer	= GetPrivateProfileInt("SCF_XMas", "SCFXMasEventSnowManTicketDropPer",10, FilePath) ;
	this->GoblinTicketDropPer	= GetPrivateProfileInt("SCF_XMas", "SCFXMasEventGoblinTicketDropPer",5, FilePath) ;

	this->MaxTripsPerTicket		= GetPrivateProfileInt("SCF_XMas", "SCFXMasMaxTripsPerTicket",1, FilePath) ;

	this->ExtraExperience		= GetPrivateProfileInt("SCF_XMas", "SCFXMasExtraExperience",5, FilePath) ;
	this->ExtraDropPercent		= GetPrivateProfileInt("SCF_XMas", "SCFXMasExtraDropPercent",5, FilePath) ;

	this->FireworksDropRate		= GetPrivateProfileInt("SCF_XMas", "SCFXMasFireworksDropRate",1, FilePath) ;
	this->SantaGirlRingDropRate	= GetPrivateProfileInt("SCF_XMas", "SCFXMasSantaGirlRingDropRate",1, FilePath) ;
	this->RudolphPetDropRate	= GetPrivateProfileInt("SCF_XMas", "SCFXMasRudolphPetDropRate",1, FilePath) ;
	this->SnownmanRingDropRate	= GetPrivateProfileInt("SCF_XMas", "SCFXMasSnownmanRingDropRate",1, FilePath) ;

	this->BlessingBuffOnKill	= GetPrivateProfileInt("SCF_XMas", "SCFXMasBlessingBuffOnEvilSantaKill", 1, FilePath) ;
	this->BlessingBuffOnTalk	= GetPrivateProfileInt("SCF_XMas", "SCFXMasBlessingBuffOnSantaTalk", 0, FilePath) ;
	this->BlessingBuffMinutes	= GetPrivateProfileInt("SCF_XMas", "SCFXMasBlessingBuffMinutes", 60, FilePath)&255 ;
	this->LittleBuffMinutes		= GetPrivateProfileInt("SCF_XMas", "SCFXMasLittleSantaBuffMinutes", 30, FilePath)&255 ;

	this->BuffDefIncrease		= GetPrivateProfileInt("SCF_XMas", "SCFXMasBlessingBuffDefIncrease", 5, FilePath)&100 ;
    this->BuffAttIncrease		= GetPrivateProfileInt("SCF_XMas", "SCFXMasBlessingBuffAttIncrease", 5, FilePath)&100 ;

	LogAddTD("[SCF XMasEvent] - %s file is Loaded",FilePath);
}

void cXMasEvent__InsideTrigger(void * lpParam)
{
	XMasEvent.Start = 1;

	LogAddTD("[XMasEvent] Event Start");
	AllSendServerMsg(lMsg.Get(MSGGET(14, 112)));
	XMasEvent.SetMonsters();
	Sleep(XMasEvent.Minutes * 60000);
	XMasEvent.ClearMonsters();
	LogAddTD("[XMasEvent] Event End");
	AllSendServerMsg(lMsg.Get(MSGGET(14, 113)));

	XMasEvent.Start = 0;
	_endthread();
}

void cXMasEvent::ClearMonsters()
{
	if(this->Enabled == 1 && this->Start == 1)
	{
		for(int i=0;i<this->MonsterCount;i++)
		{
			if (this->Monster[i].ID >= 0)
			{
				if (this->Monster[i].Number = gObj[this->Monster[i].ID].Class &&
					gMSetBase.m_Mp[this->Monster[i].Pos].m_MapNumber == gObj[this->Monster[i].ID].MapNumber)
				{
					gObjDel(this->Monster[i].ID);
				}

				this->Monster[i].ID = -1;
			}
		}
	}
}

void cXMasEvent::SetMonsters()
{
	if(this->Enabled == 1 && this->Start == 1)
	{
		for(int i=0; i<this->MonsterCount;i++)
		{			
			int MobID = gObjAddMonster(gMSetBase.m_Mp[this->Monster[i].Pos].m_MapNumber);
			if(MobID >= 0)
			{
				this->Monster[i].ID = MobID;
				gObjSetPosMonster(MobID, this->Monster[i].Pos);
				gObjSetMonster(MobID, gMSetBase.m_Mp[this->Monster[i].Pos].m_Type,"cXMasEvent::SetMonsters");
			}
		}
	}
}

void cXMasEvent::ReadMonsters(char * FilePath)
{
	if(this->Enabled == 1)
	{
		int Token;
		int Map;
		int X;
		int Y;
		int Count;
		int MobID;
		this->MonsterCount = 0;

		for (int iM = 0; iM < 256; iM++)
		{
			this->Monster[iM].ID = -1;
			this->Monster[iM].Pos = -1;
			this->Monster[iM].Number = -1;
		}

		SMDFile = fopen(FilePath, "r");

		if ( SMDFile == NULL )
		{
			MsgBox("XMasEvent data load error %s", FilePath);
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
					MobID = TokenNumber;

					Token = GetToken();
					Map = TokenNumber;

					Token = GetToken();
					X = TokenNumber;

					Token = GetToken();
					Y = TokenNumber;

					Token = GetToken();
					Count = TokenNumber;

					for(int i=0;i<Count;i++)
					{
						if (this->MonsterCount < 256)
						{
							int MobPos = gObjMonsterAdd(MobID,Map,X,Y);
							if(MobPos >= 0)
							{
								this->Monster[this->MonsterCount].ID = -1;
								this->Monster[this->MonsterCount].Pos = MobPos;
								this->Monster[this->MonsterCount].Number = MobID;
								this->MonsterCount++;
							}
						} else {
							LogAddTD("[XMasEvent][Overflow] - Cant add monster to Map:%d:, ID:%d, X:%d, Y:%d",
								Map,MobID,X,Y);
						}
					}
				}
			}
			break;
		}
			
		fclose(SMDFile);
		LogAddTD("[XMasEvent] - %s file is Loaded",FilePath);
	}
}

#endif