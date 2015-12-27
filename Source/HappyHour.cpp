#include "stdafx.h"
#include "GameServer.h"
#include "LogProc.h"
#include "GameMain.h"
#include "TNotice.h"
#include "..\include\readscript.h"
//#include "GameServerAuth.h"
#include "..\common\winutil.h"
#include "HappyHour.h"
#include "XMasEvent.h"

#if (PACK_EDITION>=1)

cHappyHour HappyHour;

void cHappyHour::StartEvent()
{
	if((this->Start == 0) && (this->Enabled == 1))
		_beginthread( cHappyHour__InsideTrigger, 0, NULL  );
}

void cHappyHour::Init(char * FilePath)
{
	this->Start			= 0;
	this->Enabled		= GetPrivateProfileInt("Common", "SCFHappyHourEnabled",0, FilePath) ;
	this->Minutes		= GetPrivateProfileInt("Common", "SCFHappyHourMinutes",5, FilePath) ;
	this->ExtraExp		= GetPrivateProfileInt("HappyHour", "SCFHappyHourExtraExperience",200, FilePath) ;
	this->ExtraDrop		= GetPrivateProfileInt("HappyHour", "SCFHappyHourExtraItemDropPer",40, FilePath) ;
	LogAddTD("[SCF HappyHour] - %s file is Loaded",FilePath);
}

void cHappyHour__InsideTrigger(void * lpParam)
{
	HappyHour.Start = 1;

	char sBuf[1024] = {0};
	int ItemDropPer = gItemDropPer;
	int totalExpMultiplyer = gAddExperience;

	ItemDropPer += HappyHour.ExtraDrop;
	totalExpMultiplyer += HappyHour.ExtraExp;

#if (PACK_EDITION>=2)
	if ( XMasEvent.Enabled == 1 )
	{
		totalExpMultiplyer += XMasEvent.ExtraExperience;
		ItemDropPer += XMasEvent.ExtraDropPercent;
	}
#endif

	wsprintf(sBuf, lMsg.Get(MSGGET(14, 0)),totalExpMultiplyer,ItemDropPer);
	AllSendServerMsg(sBuf);
	
	wsprintf(sBuf, "[HappyHour] Event Duration is %d minutes",HappyHour.Minutes);
	AllSendServerMsg(sBuf);

	LogAddTD("[HappyHour] Event Start, Extra Exp:%d, Extra Drop:%d, Total Exp:%d, Total Drop:%d, Duration:%d(min)",
		HappyHour.ExtraExp,HappyHour.ExtraDrop,totalExpMultiplyer,ItemDropPer,HappyHour.Minutes);

	Sleep(HappyHour.Minutes * 60000);

	LogAddTD("[HappyHour] Event End");
	AllSendServerMsg(lMsg.Get(MSGGET(14, 1)));

	HappyHour.Start = 0;
	_endthread();
}

#endif