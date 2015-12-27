#include "stdafx.h"
#include "GameServer.h"
#include "LogProc.h"
#include "GameMain.h"
#include "TNotice.h"
#include "..\include\readscript.h"
//#include "GameServerAuth.h"
#include "..\common\winutil.h"
#include "HitAndUp.h"

#if (PACK_EDITION>=2)

cHitAndUp HitAndUp;

void cHitAndUp::StartEvent()
{
	if((this->Start == 0) && (this->Enabled == 1))
		_beginthread( cHitAndUp__InsideTrigger, 0, NULL  );
}

void cHitAndUp::Init(char * FilePath)
{
	this->Start					= 0;
	this->Enabled				= GetPrivateProfileInt("Common", "SCFHaPEnabled",0, FilePath) ;
	this->Minutes				= GetPrivateProfileInt("Common", "SCFHaPMinutes",5, FilePath) ;
	this->MGExtraPoint			= GetPrivateProfileInt("Points", "SCFHaPMGExtraPoint",1, FilePath) ;
	this->DLExtraPoint			= GetPrivateProfileInt("Points", "SCFHaPDLExtraPoint",1, FilePath) ;
	this->RFExtraPoint			= GetPrivateProfileInt("Points", "SCFHaPRFExtraPoint",1, FilePath) ;
	this->FirstClassExtraPoint	= GetPrivateProfileInt("Points", "SCFHaPFirstClassExtraPoint",1, FilePath) ;
	this->SecondClassExtraPoint	= GetPrivateProfileInt("Points", "SCFHaPSecondClassExtraPoint",2, FilePath) ;
	this->ThirdClassExtraPoint	= GetPrivateProfileInt("Points", "SCFHaPThirdClassExtraPoint",3, FilePath) ;
	LogAddTD("[SCF HitAndUp] - %s file is Loaded",FilePath);
}

int cHitAndUp::GetAddExtraPoint(BYTE Class)
{
	if(this->Enabled == 1 && this->Start == 1)
	{
		int ExtraPoints = 0;
		switch(Class)
		{
			case 0:
			case 16:
			case 32:
			case 80:
				ExtraPoints = this->FirstClassExtraPoint;
				break;
			case 1:
			case 17:
			case 33:
			case 81:
			case 97:
				ExtraPoints = this->SecondClassExtraPoint;
				break;
			case 2:
			case 18:
			case 34:
			case 49:
			case 50:
			case 65:
			case 66:
			case 82:
			case 98:
				ExtraPoints = this->ThirdClassExtraPoint;
				break;
			case 48:
				ExtraPoints = this->MGExtraPoint;
				break;
			case 64:
				ExtraPoints = this->DLExtraPoint;
				break;
			case 96:
				ExtraPoints = this->RFExtraPoint;
				break;
		}
		
		return ExtraPoints;
	}
	return 0;
}

void cHitAndUp__InsideTrigger(void * lpParam)
{
	HitAndUp.Start = 1;

	LogAddTD("[HitAndUp] Event Start");
	AllSendServerMsg(lMsg.Get(MSGGET(14, 8)));
	Sleep(HitAndUp.Minutes * 60000);
	LogAddTD("[HitAndUp] Event End");
	AllSendServerMsg(lMsg.Get(MSGGET(14, 9)));

	HitAndUp.Start = 0;
	_endthread();
}

#endif