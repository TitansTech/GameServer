#include "stdafx.h"
#include "GameServer.h"
#include "LogProc.h"
#include "GameMain.h"
#include "TNotice.h"
#include "..\include\readscript.h"
//#include "GameServerAuth.h"
#include "..\common\winutil.h"
#include "GreenEvent.h"

#if (PACK_EDITION>=1)

cGreenEvent GreenEvent;

void cGreenEvent::StartEvent()
{
	if((this->Start == 0) && (this->Enabled == 1))
		_beginthread( cGreenEvent__InsideTrigger, 0, NULL  );
}

void cGreenEvent::Init(char * FilePath)
{
	this->Start					= 0;
	this->Enabled				= GetPrivateProfileInt("Common", "SCFGreenEventSwitch",0, FilePath) ;
	this->Minutes				= GetPrivateProfileInt("Common", "SCFGreenEventMinutesTime",5, FilePath) ;
	this->ExcDropRate			= GetPrivateProfileInt("DropItem", "SCFGreenEventExItemDropRate",1000, FilePath) ;
	this->ExcItemDropSkill		= GetPrivateProfileInt("DropItem", "SCFGreenEventExItemDropSkillRate",100, FilePath) ;
	this->ExcItemDropLuck		= GetPrivateProfileInt("DropItem", "SCFGreenEventExItemDropLuckRate",50, FilePath) ;
	this->NormalItemDropSkill	= GetPrivateProfileInt("DropItem", "SCFGreenEventNormalItemDropSkillRate",6, FilePath) ;
	this->NormalItemDropLuck	= GetPrivateProfileInt("DropItem", "SCFGreenEventNormalItemDropLuckRate",4, FilePath) ;
	LogAddTD("[SCF GreenEvent] - %s file is Loaded",FilePath);
}

void cGreenEvent__InsideTrigger(void * lpParam)
{
	GreenEvent.Start = 1;

	LogAddTD("[GreenEvent] Event Start");
	AllSendServerMsg(lMsg.Get(MSGGET(14, 6)));
	Sleep(GreenEvent.Minutes * 60000);
	LogAddTD("[GreenEvent] Event End");
	AllSendServerMsg(lMsg.Get(MSGGET(14, 7)));

	GreenEvent.Start = 0;
	_endthread();
}

#endif