#include "stdafx.h"
#include "GMMng.h"
#include "LogToFile.h"
#include "logproc.h"
#include "GameMain.h"
#include "protocol.h"
#include "..\common\winutil.h"
#include "MoveCommand.h"
#include "GuildClass.h"
#include "TNotice.h"
#include "giocp.h"
#include "SkillAdditionInfo.h"
#include "ObjCalCharacter.h"
#include "Marry.h"
#include "ResetSystem.h"
#include "VIPSystem.h"
#include "SCFExDBProtocol.h"
#include "GMSystem.h"
#include "DuelManager.h"
#include "CustomQuest.h"
#include "SCFPostServerProtocol.h"
#include "ObjBotStore.h"
#include "ObjBotBuffer.h"

#include "EventManagement.h"
#include "BattleSoccerManager.h"
#include "Kanturu.h"
#include "DoppelGanger.h"
#include "ImperialGuardian.h"
#include "BloodCastle.h"
#include "RainItemEvent.h"
#include "DevilSquare.h"
#include "BlueEvent.h"
#include "SkyEvent.h"
#include "BossAttack.h"
#include "Raklion.h"
#include "HitAndUp.h"
#include "HappyHour.h"
#include "MossShop.h"
#include "XMasEvent.h"
#include "GreenEvent.h"
#include "SwampEvent.h"
#include "SummerEvent.h"
#include "HalloweenEvent.h"
#include "SCFBotHidenSeek.h"
#include "Data.h"
#include "GensCloseMap.h"
#include "ObjTitanRank.h"
#include "ObjTitanLottery.h"
#include "ObjQaA.h"
// GS-N 0.99.60T 0x004F0110
//	GS-N	1.00.18	JPN	0x00519F60	-	Completed

//Added Review by Hermex
#include "DSProtocol.h"

#include "ObjBotPet.h"

CLogToFile KUNDUN_GM_LOG( "KUNDUN_EVENT_GM_LOG", ".\\KUNDUN_EVENT_GM_LOG", 1);
CLogToFile GM_LOG("GM_LOG", ".\\GM_LOG", 1);
CLogToFile POST_LOG("POST_LOG", ".\\POST_LOG", 1);
CGMMng cManager;

CGMMng::CGMMng()
{
	this->ManagerInit();
}

CGMMng::~CGMMng()
{
	return;
}
char Cmd[512][30];
void CGMMng::Init()
{
	this->cCommand.Init();
	char FilePath[] = "..\\SCFData\\SCF_Commands.ini";
	GetPrivateProfileString		("Names","Cmd00","/Test",Cmd[0],30,FilePath);
	GetPrivateProfileString		("Names","Cmd01","/Warp",Cmd[1],30,FilePath);
	GetPrivateProfileString		("Names","Cmd02","/Transform",Cmd[2],30,FilePath);
	GetPrivateProfileString		("Names","Cmd03","/Block setting",Cmd[3],30,FilePath);
	GetPrivateProfileString		("Names","Cmd04","/Cancel block",Cmd[4],30,FilePath);
	GetPrivateProfileString		("Names","Cmd05","/Chatting ban",Cmd[5],30,FilePath);
	GetPrivateProfileString		("Names","Cmd06","/Cancel chatting ban",Cmd[6],30,FilePath);
	GetPrivateProfileString		("Names","Cmd07","/Warp guild",Cmd[7],30,FilePath);
	GetPrivateProfileString		("Names","Cmd08","/End guild",Cmd[8],30,FilePath);
	GetPrivateProfileString		("Names","Cmd09","/End battle",Cmd[9],30,FilePath);
	GetPrivateProfileString		("Names","Cmd10","/Start battle",Cmd[10],30,FilePath);
	GetPrivateProfileString		("Names","Cmd11","/Stop battle",Cmd[11],30,FilePath);
	GetPrivateProfileString		("Names","Cmd12","/Item",Cmd[12],30,FilePath);
	GetPrivateProfileString		("Names","Cmd13","/Time left",Cmd[13],30,FilePath);
	GetPrivateProfileString		("Names","Cmd14","/Battle",Cmd[14],30,FilePath);	
	GetPrivateProfileString		("Names","Cmd15","/Battle Soccer",Cmd[15],30,FilePath);
	GetPrivateProfileString		("Names","Cmd16","/Request",Cmd[16],30,FilePath);
	GetPrivateProfileString		("Names","Cmd17","/Disconnect",Cmd[17],30,FilePath);
	GetPrivateProfileString		("Names","Cmd18","/Move",Cmd[18],30,FilePath);
	GetPrivateProfileString		("Names","Cmd19","/Trans",Cmd[19],30,FilePath);
	GetPrivateProfileString		("Names","Cmd20","/SetBlock",Cmd[20],30,FilePath);
	GetPrivateProfileString		("Names","Cmd21","/UnsetBlock",Cmd[21],30,FilePath);
	GetPrivateProfileString		("Names","Cmd22","/DisableChat",Cmd[22],30,FilePath);
	GetPrivateProfileString		("Names","Cmd23","/EnableChat",Cmd[23],30,FilePath);
	GetPrivateProfileString		("Names","Cmd24","/GuildMove",Cmd[24],30,FilePath);
	GetPrivateProfileString		("Names","Cmd25","/GuildDisconnect",Cmd[25],30,FilePath);
	GetPrivateProfileString		("Names","Cmd26","/GuildWarEnd",Cmd[26],30,FilePath);
	GetPrivateProfileString		("Names","Cmd27","/GuildWarStart",Cmd[27],30,FilePath);
	GetPrivateProfileString		("Names","Cmd28","/GuildWarStop",Cmd[28],30,FilePath);
	GetPrivateProfileString		("Names","Cmd29","/RemainTime",Cmd[29],30,FilePath);
	GetPrivateProfileString		("Names","Cmd30","/GuildWar",Cmd[30],30,FilePath);
	GetPrivateProfileString		("Names","Cmd31","/BattleSoccer",Cmd[31],30,FilePath);
	GetPrivateProfileString		("Names","Cmd32","/Request",Cmd[32],30,FilePath);
	GetPrivateProfileString		("Names","Cmd33","/Connection status",Cmd[33],30,FilePath);
	GetPrivateProfileString		("Names","Cmd34","/Trace",Cmd[34],30,FilePath);
	GetPrivateProfileString		("Names","Cmd35","/Monitor",Cmd[35],30,FilePath);
	GetPrivateProfileString		("Names","Cmd36","/ConnectionState",Cmd[36],30,FilePath);
	GetPrivateProfileString		("Names","Cmd37","/UserTracking",Cmd[37],30,FilePath);
	GetPrivateProfileString		("Names","Cmd38","/UserWatching",Cmd[38],30,FilePath);
	GetPrivateProfileString		("Names","Cmd39","/Firecracker",Cmd[39],30,FilePath);
	GetPrivateProfileString		("Names","Cmd40","/post",Cmd[40],30,FilePath);
	GetPrivateProfileString		("Names","Cmd41","/pkclear",Cmd[41],30,FilePath);
	GetPrivateProfileString		("Names","Cmd42","/addstr",Cmd[42],30,FilePath);
	GetPrivateProfileString		("Names","Cmd43","/addagi",Cmd[43],30,FilePath);
	GetPrivateProfileString		("Names","Cmd44","/addvit",Cmd[44],30,FilePath);
	GetPrivateProfileString		("Names","Cmd45","/addene",Cmd[45],30,FilePath);
	GetPrivateProfileString		("Names","Cmd46","/addcmd",Cmd[46],30,FilePath);
	GetPrivateProfileString		("Names","Cmd47","/skin",Cmd[47],30,FilePath);
#if (PACK_EDITION>=2)
	GetPrivateProfileString		("Names","Cmd48","/marry",Cmd[48],30,FilePath);
	GetPrivateProfileString		("Names","Cmd49","/getmarry",Cmd[49],30,FilePath);
	GetPrivateProfileString		("Names","Cmd50","/acceptmarry",Cmd[50],30,FilePath);
	GetPrivateProfileString		("Names","Cmd51","/divorce",Cmd[51],30,FilePath);
	GetPrivateProfileString		("Names","Cmd52","/forcedivorce",Cmd[52],30,FilePath);
	GetPrivateProfileString		("Names","Cmd53","/tracemarry",Cmd[53],30,FilePath);	
#endif
	GetPrivateProfileString		("Names","Cmd54","/Kundun Status",Cmd[54],30,FilePath);
	GetPrivateProfileString		("Names","Cmd55","/Kundunpi",Cmd[55],30,FilePath);
	GetPrivateProfileString		("Names","Cmd56","/Kundunhoebokryang",Cmd[56],30,FilePath);
	GetPrivateProfileString		("Names","Cmd57","/Kundunchodanghoebokryang",Cmd[57],30,FilePath);
	GetPrivateProfileString		("Names","Cmd58","/Kundun recovery time",Cmd[58],30,FilePath);

#if (PACK_EDITION>=2)
	GetPrivateProfileString		("Names","Cmd59","/Reset",Cmd[59],30,FilePath);
#endif
	GetPrivateProfileString		("Names","Cmd60","/Gmove",Cmd[60],30,FilePath);
#if (PACK_EDITION>=2)
	GetPrivateProfileString		("Names","Cmd61","/vipbuylist",Cmd[61],30,FilePath);
	GetPrivateProfileString		("Names","Cmd62","/vipbuy",Cmd[62],30,FilePath);
	GetPrivateProfileString		("Names","Cmd63","/viprenew",Cmd[63],30,FilePath);
	GetPrivateProfileString		("Names","Cmd64","/vipstate",Cmd[64],30,FilePath);
	//GetPrivateProfileString		("Names","Cmd63","/viprenew",Cmd[63],30,FilePath);
#endif

	GetPrivateProfileString		("Names","Cmd65","/serverinfo",Cmd[65],30,FilePath);
	GetPrivateProfileString		("Names","Cmd66","/playerinfo",Cmd[66],30,FilePath);
	GetPrivateProfileString		("Names","Cmd67","/gg",Cmd[67],30,FilePath);
	GetPrivateProfileString		("Names","Cmd68","/Gmoveall",Cmd[68],30,FilePath);
	GetPrivateProfileString		("Names","Cmd69","/level",Cmd[69],30,FilePath);
	GetPrivateProfileString		("Names","Cmd70","/whois",Cmd[70],30,FilePath);
	GetPrivateProfileString		("Names","Cmd71","/online",Cmd[71],30,FilePath);
	GetPrivateProfileString		("Names","Cmd72","/addbuff",Cmd[72],30,FilePath);
	GetPrivateProfileString		("Names","Cmd73","/clearinv",Cmd[73],30,FilePath);
	GetPrivateProfileString		("Names","Cmd74","/addskill",Cmd[74],30,FilePath);
	GetPrivateProfileString		("Names","Cmd75","/status",Cmd[75],30,FilePath);
	GetPrivateProfileString		("Names","Cmd76","/vipstatus",Cmd[76],30,FilePath);
	GetPrivateProfileString		("Names","Cmd77","/setzen",Cmd[77],30,FilePath);
	GetPrivateProfileString		("Names","Cmd78","/setvip",Cmd[78],30,FilePath);
	GetPrivateProfileString		("Names","Cmd79","/banchar",Cmd[79],30,FilePath);
	GetPrivateProfileString		("Names","Cmd80","/banaccount",Cmd[80],30,FilePath);
	GetPrivateProfileString		("Names","Cmd81","/spawn",Cmd[81],30,FilePath);
	GetPrivateProfileString		("Names","Cmd82","/pkset",Cmd[82],30,FilePath);
	GetPrivateProfileString		("Names","Cmd83","/mobskin",Cmd[83],30,FilePath);
#if (PACK_EDITION>=2)
	GetPrivateProfileString		("Names","Cmd84","/ware",Cmd[84],30,FilePath);
	GetPrivateProfileString		("Names","Cmd85","/vipbuyware",Cmd[85],30,FilePath);
#endif
#if (GS_CASTLE==1)
	GetPrivateProfileString		("Names","Cmd86","/cschangeowner",Cmd[86],30,FilePath);
	GetPrivateProfileString		("Names","Cmd87","/cssetregsiege",Cmd[87],30,FilePath);
	GetPrivateProfileString		("Names","Cmd88","/cssetregmark",Cmd[88],30,FilePath);
	GetPrivateProfileString		("Names","Cmd89","/cssetnotify",Cmd[89],30,FilePath);
	GetPrivateProfileString		("Names","Cmd90","/cssetstart",Cmd[90],30,FilePath);
	GetPrivateProfileString		("Names","Cmd91","/cssetend",Cmd[91],30,FilePath);
	GetPrivateProfileString		("Names","Cmd92","/cscurowner",Cmd[92],30,FilePath);
	GetPrivateProfileString		("Names","Cmd93","/cscurstate",Cmd[93],30,FilePath);
	GetPrivateProfileString		("Names","Cmd94","/cschangeside",Cmd[94],30,FilePath);
#endif
	GetPrivateProfileString		("Names","Cmd95","/goevent",Cmd[95],30,FilePath);
#if (PACK_EDITION>=2)
	GetPrivateProfileString		("Names","Cmd96","/vipbuydays",Cmd[96],30,FilePath);
#endif
	GetPrivateProfileString		("Names","Cmd97","/lottery",Cmd[97],30,FilePath);
	GetPrivateProfileString		("Names","Cmd98","/ans",Cmd[98],30,FilePath);

	this->cCommand.Add(Cmd[0], 100);
	this->cCommand.Add(Cmd[1], 101);
	this->cCommand.Add(Cmd[2], 102);
	this->cCommand.Add(Cmd[3], 103);
	this->cCommand.Add(Cmd[4], 105);
	this->cCommand.Add(Cmd[5], 104);
	this->cCommand.Add(Cmd[6], 106);
	this->cCommand.Add(Cmd[7], 108);
	this->cCommand.Add(Cmd[8], 112);
	this->cCommand.Add(Cmd[9], 111);
	this->cCommand.Add(Cmd[10], 109);
	this->cCommand.Add(Cmd[11], 110);
	this->cCommand.Add(Cmd[12], 390);
	this->cCommand.Add(Cmd[13], 201);
	this->cCommand.Add(Cmd[14], 200);	
	this->cCommand.Add(Cmd[15], 202);
	this->cCommand.Add(Cmd[16], 203);
	this->cCommand.Add(Cmd[17], 100);
	this->cCommand.Add(Cmd[18], 101);
	this->cCommand.Add(Cmd[19], 102);
	this->cCommand.Add(Cmd[20], 103);
	this->cCommand.Add(Cmd[21], 105);
	this->cCommand.Add(Cmd[22], 104);
	this->cCommand.Add(Cmd[23], 106);
	this->cCommand.Add(Cmd[24], 108);
	this->cCommand.Add(Cmd[25], 112);
	this->cCommand.Add(Cmd[26], 111);
	this->cCommand.Add(Cmd[27], 109);
	this->cCommand.Add(Cmd[28], 110);
	this->cCommand.Add(Cmd[29], 201);
	this->cCommand.Add(Cmd[30], 200);
	this->cCommand.Add(Cmd[31], 202);
	this->cCommand.Add(Cmd[32], 203);
	this->cCommand.Add(Cmd[33], 214);
	this->cCommand.Add(Cmd[34], 215);
	this->cCommand.Add(Cmd[35], 216);
	this->cCommand.Add(Cmd[36], 214);
	this->cCommand.Add(Cmd[37], 215);
	this->cCommand.Add(Cmd[38], 216);
	this->cCommand.Add(Cmd[39], 217);
	this->cCommand.Add(Cmd[40], 391);
	this->cCommand.Add(Cmd[41], 392);
	this->cCommand.Add(Cmd[42], 393);
	this->cCommand.Add(Cmd[43], 394);
	this->cCommand.Add(Cmd[44], 395);
	this->cCommand.Add(Cmd[45], 396);
	this->cCommand.Add(Cmd[46], 397);
	this->cCommand.Add(Cmd[47], 398);	

#if (PACK_EDITION>=2)
	this->cCommand.Add(Cmd[48], 399);
	this->cCommand.Add(Cmd[49], 400);
	this->cCommand.Add(Cmd[50], 401);
	this->cCommand.Add(Cmd[51], 402);
	this->cCommand.Add(Cmd[52], 403);
	this->cCommand.Add(Cmd[53], 404);	
#endif
	this->cCommand.Add(Cmd[54], 320);
	this->cCommand.Add(Cmd[55], 321);
	this->cCommand.Add(Cmd[56], 322);
	this->cCommand.Add(Cmd[57], 323);
	this->cCommand.Add(Cmd[58], 324);

#if (GS_CASTLE==1)
	this->cCommand.Add(Cmd[86], 331);
	this->cCommand.Add(Cmd[87], 332);
	this->cCommand.Add(Cmd[88], 333);
	this->cCommand.Add(Cmd[89], 334);
	this->cCommand.Add(Cmd[90], 335);
	this->cCommand.Add(Cmd[91], 336);
	this->cCommand.Add(Cmd[92], 337);
	this->cCommand.Add(Cmd[93], 338);
	this->cCommand.Add(Cmd[94], 340);
#endif

#if (PACK_EDITION>=2)
	this->cCommand.Add(Cmd[59], 405);	
#endif
	this->cCommand.Add(Cmd[60], 406);

#if (PACK_EDITION>=2)
	this->cCommand.Add(Cmd[61], 407);
	this->cCommand.Add(Cmd[62], 408);	
	this->cCommand.Add(Cmd[63], 409);	
	this->cCommand.Add(Cmd[64], 410);
#endif
	this->cCommand.Add(Cmd[65], 411);
	this->cCommand.Add(Cmd[66], 412);	
	this->cCommand.Add(Cmd[67], 413);	
	this->cCommand.Add(Cmd[68], 414);	
	this->cCommand.Add(Cmd[69], 415);
	this->cCommand.Add(Cmd[70], 416);
	this->cCommand.Add(Cmd[71], 417);	
	this->cCommand.Add(Cmd[72], 418);
	this->cCommand.Add(Cmd[73], 419);
	this->cCommand.Add(Cmd[74], 420);	
	this->cCommand.Add(Cmd[75], 421);
	this->cCommand.Add(Cmd[76], 422);	
	this->cCommand.Add(Cmd[77], 423);	
	this->cCommand.Add(Cmd[78], 424);
	this->cCommand.Add(Cmd[79], 425);	
	this->cCommand.Add(Cmd[80], 426);
	this->cCommand.Add(Cmd[81], 427);
	this->cCommand.Add(Cmd[82], 428);
#if (WL_PROTECT==1)
	CODEREPLACE_START
	this->cCommand.Add("/13s1518", 429);
	this->cCommand.Add("/gret547", 430);
	CODEREPLACE_END
#endif
	this->cCommand.Add(Cmd[83], 431);
	this->cCommand.Add(Cmd[84], 432);
	this->cCommand.Add(Cmd[85], 433);

	this->cCommand.Add("/effect", 434);
	this->cCommand.Add(Cmd[95], 435);
	this->cCommand.Add("/invisible", 436);

	// KANTURU Commands
	this->cCommand.Add("/KanturuReady", 369);
	this->cCommand.Add("/KanturuBattleMaya", 370);
	this->cCommand.Add("/MayaMonsters1", 371);	// #translation 
	this->cCommand.Add("/Maya1", 372);	// #translation 
	this->cCommand.Add("/MayaMonster2", 373);	// #translation 
	this->cCommand.Add("/Maya2", 374);	// #translation 
	this->cCommand.Add("/MayaMonster3", 375);	// #translation 
	this->cCommand.Add("/Maya3", 376);	// #translation 
	this->cCommand.Add("/Maya3Successful", 377);	// #translation 
	this->cCommand.Add(lMsg.Get(MSGGET(13, 60)), 378);	// "/»÷°ÜÂêÑÅÉúÃüÌå"
	this->cCommand.Add("/Mayafails", 379);	// #translation 
	this->cCommand.Add("/KanturuBattleNightmare", 380);
	this->cCommand.Add(lMsg.Get(MSGGET(13, 61)), 381);	// "/»÷°ÜÖäÔ¹Ä§Íõ"
	this->cCommand.Add("/FailureNightmare", 382);	// #translation 
	this->cCommand.Add("/KanturuTower", 383);
	this->cCommand.Add("/KanturuTowerOpen", 384);
	this->cCommand.Add("/KanturuTowerClose", 385);
	this->cCommand.Add("/KanturuEnd", 386);
#if (PACK_EDITION>=2)
	this->cCommand.Add(Cmd[96], 437);
#endif
#if (PACK_EDITION>=3)
	this->cCommand.Add(Cmd[97], 438);
	this->cCommand.Add(Cmd[98], 439);
#endif

	//NEW ONE MAKE BOO BOO
	//this->cCommand.Add(lMsg.Get(MSGGET(13, 57)), 387);
	//this->cCommand.Add(lMsg.Get(MSGGET(13, 58)), 388);
	//this->cCommand.Add(lMsg.Get(MSGGET(13, 59)), 389);
	//NEW ONE MAKE BOO BOO

	//this->cCommand.Add("/ÆøÁ×", 217);	// Fire Cracker Effect	// #translation 

	//this->cCommand.Add("/post", 391);	// PostMsg
	//this->cCommand.Add("/pkclear", 392);	// ClearPkCommand
	//this->cCommand.Add("/addstr", 393);
	//this->cCommand.Add("/addagi", 394);
	//this->cCommand.Add("/addvit", 395);
	//this->cCommand.Add("/addene", 396);
	//this->cCommand.Add("/addcmd", 397);
	//this->cCommand.Add("/skin", 398);

	this->WatchTargetIndex = -1;
}


void CGMMng::ManagerInit()
{
	for ( int n = 0;n<MAX_GAME_MASTER ; n++ )
	{
		this->ManagerIndex[n] = -1;
	}
}


int CGMMng::ManagerAdd(char* name, int aIndex)
{
	for ( int n=0 ; n<MAX_GAME_MASTER ; n++ )
	{
		if ( this->ManagerIndex[n] == -1 )
		{
			strcpy(this->szManagerName[n], name);
			this->ManagerIndex[n] = aIndex;
			return n;
		}
	}

	return -1;
}


void CGMMng::ManagerDel(char* name)
{
	for ( int n=0;n<MAX_GAME_MASTER;n++)
	{
		if ( this->ManagerIndex[n] != -1 )
		{
			if ( strcmp(this->szManagerName[n], name) == 0 )
			{
				this->ManagerIndex[n] = -1;
				break;
			}
		}
	}
}
struct PMSG_GENSF805
{
	PWMSG_HEAD h;	// C1:FA:1A
	BYTE subcode;	// 3
	BYTE QuestCount;	// 4
};

struct GENSF805INFO
{
	BYTE Gens;
	BYTE SubGens;
	BYTE Unk1;
	BYTE Unk2;
	BYTE NumberL;
	BYTE NumberH;
	WORD Unk3;
	BYTE ZE;
	BYTE Unk4[7];
};
void SendShit(int aIndex)
{	
	LPOBJ lpObj = &gObj[aIndex];
	int MVL = MAX_VIEWPORT;
	BYTE cBUFFER[512]={0}; //Max Value 0xFF | Max Quest Count = 62 per character
	BYTE Count = 0;
	PMSG_GENSF805 * pResult = (PMSG_GENSF805 *)(cBUFFER);
	GENSF805INFO * qAdd = (GENSF805INFO *)(cBUFFER + sizeof(PMSG_GENSF805));
	int lOfs=sizeof(PMSG_GENSF805);
	
	for(int n = 0; n < MVL; n++)
	{
		int tObjNum = lpObj->VpPlayer2[n].number;
		if(tObjNum >= 0)
		{
			qAdd->Gens = 1;
			qAdd->SubGens = 0x25;
			qAdd->Unk1 = 0xD4;
			qAdd->NumberH = SET_NUMBERH(tObjNum);
			qAdd->NumberL = SET_NUMBERL(tObjNum);
			qAdd->ZE = 0x0E;
			Count++;
		}
	}

	int Size = (sizeof(PMSG_GENSF805) + sizeof(GENSF805INFO) * Count);
	PHeadSetW((LPBYTE)pResult,0xF8,Size);
	pResult->subcode = 0x05;
	pResult->QuestCount = Count;
	DataSend(aIndex, cBUFFER, Size);
}
void CGMMng::ManagerSendData(LPSTR szMsg, int size)
{
	for ( int n=0;n<MAX_GAME_MASTER;n++ )
	{
		if ( this->ManagerIndex[n] != -1 )
		{
			GCServerMsgStringSend(szMsg, this->ManagerIndex[n], 0);
		}
	}
}

void CGMMng::DataSend(unsigned char* szMsg, int size)
{
	for ( int n=0;n<MAX_GAME_MASTER;n++ )
	{
		if ( this->ManagerIndex[n] != -1 )
		{
			::DataSend(this->ManagerIndex[n], szMsg, size);
		}
	}
}


void CGMMng::BattleInfoSend(char* Name1, BYTE score1, char* Name2, BYTE score2)
{
	for ( int n=0;n<MAX_GAME_MASTER;n++ )
	{
		if ( this->ManagerIndex[n] != -1 )
		{
			GCGoalSend(this->ManagerIndex[n], Name1, score1, Name2, score2);
		}
	}
}


int CGMMng::GetCmd(char* szCmd)
{
	for ( int n	=0; n< MAX_GM_COMMAND ; n++ )
	{
		if ( this->cCommand.nCmdCode[n] > 0 )
		{
			if ( stricmp(szCmd, this->cCommand.szCmd[n]) == 0 )
			{
				return this->cCommand.nCmdCode[n];
			}
		}
	}

	return 0;
}



char* CGMMng::GetTokenString()
{
	char seps[2] = " ";
	return strtok(0, seps);
}


int CGMMng::GetTokenNumber()
{
	char seps[2] = " ";
	char * szToken = strtok(0, seps);

	if ( szToken != NULL )
	{
		return atoi(szToken);
	}

	return 0;
}

void ServerMsgSendPost(int Type,char Sender[20],const char*Message,...)
{
	__try
	{
	char szBuffer[1024];
	va_list pArguments;
	va_start(pArguments,Message);
	vsprintf(szBuffer,Message,pArguments);
	va_end(pArguments);
	BYTE *Packet;
	Packet = (BYTE*)malloc(200);
	memset(Packet,0x00,200);
	*Packet = 0xC1;
	memcpy((Packet+3),Sender,strlen(Sender));
	switch(Type)
	{
		case 0:
		{
			*(Packet+2)=0x02;
			memcpy((Packet+13),szBuffer,strlen(szBuffer));
		}break;
		case 1:
		{
			*(Packet+2)=0x00;
			*(Packet+13)=0x40;
			memcpy((Packet+14),szBuffer,strlen(szBuffer));
		}break;
		case 2:
		{
			*(Packet+2)=0x00;
			*(Packet+13)=0x7E;
			memcpy((Packet+14),szBuffer,strlen(szBuffer));
		}break;
		case 3:
		{
			*(Packet+2)=0x00;
			*(Packet+13)=0x40;
			*(Packet+14)=0x40;
			memcpy((Packet+15),szBuffer,strlen(szBuffer));
		}break;
	}

	int Len = (strlen(szBuffer)+0x15);
	*(Packet+1)=Len;
	DataSendAll(Packet,Len);

	free(Packet);
	}__except( EXCEPTION_ACCESS_VIOLATION == GetExceptionCode() )
	{
	}
}

void ServerMsgSend(LPOBJ lpObj,int Type,char Sender[20],const char*Message,...)
{
	__try
	{
	char szBuffer[1024];
	va_list pArguments;
	va_start(pArguments,Message);
	vsprintf(szBuffer,Message,pArguments);
	va_end(pArguments);
	BYTE *Packet;
	Packet = (BYTE*)malloc(200);
	memset(Packet,0x00,200);
	*Packet = 0xC1;
	if(Type)*(Packet+2)=0x02;
	else *(Packet+2)=0x00;
	memcpy((Packet+3),Sender,strlen(Sender));
	memcpy((Packet+13),szBuffer,strlen(szBuffer));
	int Len = (strlen(szBuffer)+0x13);
	*(Packet+1)=Len;
	if(!lpObj)DataSendAll(Packet,Len);
	else
		if(lpObj->Connected)
			DataSend(lpObj->m_Index,Packet,Len);
	free(Packet);
	}__except( EXCEPTION_ACCESS_VIOLATION == GetExceptionCode() )
	{
	}
}

int CGMMng::ManagementProc(LPOBJ lpObj, char* szCmd, int aIndex)
{
	__try
	{
	char sbuf[1024]={0};
	char seps[2] = " ";
	char * szCmdToken;
	char string[256];
	char szId[20];
	char * pId = szId;
	int len = strlen(szCmd);
	int command_number;

	if ( len < 1 || len > 250 )
	{
		return 0;
	}

	memset(szId, 0, sizeof(szId));
	strcpy(string, szCmd);
	szCmdToken = strtok(string, seps);

#if (PACK_EDITION>=2)
	if(customQuest.Command(lpObj,szCmdToken) == 1)
		return 1;
#endif

#if (PACK_EDITION>=3)
	if(!stricmp(szCmdToken,"/botpet"))
	{
		char * bpName;
		pId = this->GetTokenString();
		int opt1 = this->GetTokenNumber();
		bpName = this->GetTokenString();

		botPet.Command(aIndex,pId,opt1,bpName);
		return 1;
	}
#endif

	command_number = this->GetCmd(szCmdToken);

	//if(Marry.CommandCore(lpObj,string) == 1)
	//{
	//	return 0;
	//}

	switch ( command_number )
	{
#if (GS_CASTLE==1)
		case 331:
		{
			if (((GMSystem.Enabled == TRUE) && (GMSystem.CheckCommand(lpObj,19) != TRUE)) || ((GMSystem.Enabled == FALSE) && ( (lpObj->Authority &2 ) != 2 ) && ((lpObj->Authority & 0x20) != 0x20) ))
			{
				return 0;
			}
			pId = this->GetTokenString();
			g_CastleSiege.OperateGmCommand(lpObj->m_Index, 0, pId);
		}
		break;
		case 332:
		{
			if (((GMSystem.Enabled == TRUE) && (GMSystem.CheckCommand(lpObj,19) != TRUE)) || ((GMSystem.Enabled == FALSE) && ( (lpObj->Authority &2 ) != 2 ) && ((lpObj->Authority & 0x20) != 0x20) ))
			{
				return 0;
			}
			pId = this->GetTokenString();
			g_CastleSiege.OperateGmCommand(lpObj->m_Index, 1, pId);
		}
		break;
		case 333:
		{
			if (((GMSystem.Enabled == TRUE) && (GMSystem.CheckCommand(lpObj,19) != TRUE)) || ((GMSystem.Enabled == FALSE) && ( (lpObj->Authority &2 ) != 2 ) && ((lpObj->Authority & 0x20) != 0x20) ))
			{
				return 0;
			}
			pId = this->GetTokenString();
			g_CastleSiege.OperateGmCommand(lpObj->m_Index, 2, pId);
		}
		break;
		case 334:
		{
			if (((GMSystem.Enabled == TRUE) && (GMSystem.CheckCommand(lpObj,19) != TRUE)) || ((GMSystem.Enabled == FALSE) && ( (lpObj->Authority &2 ) != 2 ) && ((lpObj->Authority & 0x20) != 0x20) ))
			{
				return 0;
			}
			pId = this->GetTokenString();
			g_CastleSiege.OperateGmCommand(lpObj->m_Index, 3, pId);
		}
		break;
		case 335:
		{
			if (((GMSystem.Enabled == TRUE) && (GMSystem.CheckCommand(lpObj,19) != TRUE)) || ((GMSystem.Enabled == FALSE) && ( (lpObj->Authority &2 ) != 2 ) && ((lpObj->Authority & 0x20) != 0x20) ))
			{
				return 0;
			}
			pId = this->GetTokenString();
			g_CastleSiege.OperateGmCommand(lpObj->m_Index, 4, pId);
		}
		break;
		case 336:
		{
			if (((GMSystem.Enabled == TRUE) && (GMSystem.CheckCommand(lpObj,19) != TRUE)) || ((GMSystem.Enabled == FALSE) && ( (lpObj->Authority &2 ) != 2 ) && ((lpObj->Authority & 0x20) != 0x20) ))
			{
				return 0;
			}
			pId = this->GetTokenString();
			g_CastleSiege.OperateGmCommand(lpObj->m_Index, 5, pId);
		}
		break;
		case 337:
		{
			if (((GMSystem.Enabled == TRUE) && (GMSystem.CheckCommand(lpObj,19) != TRUE)) || ((GMSystem.Enabled == FALSE) && ( (lpObj->Authority &2 ) != 2 ) && ((lpObj->Authority & 0x20) != 0x20) ))
			{
				return 0;
			}
			pId = this->GetTokenString();
			g_CastleSiege.OperateGmCommand(lpObj->m_Index, 6, pId);
		}
		break;
		case 338:
		{
			if (((GMSystem.Enabled == TRUE) && (GMSystem.CheckCommand(lpObj,19) != TRUE)) || ((GMSystem.Enabled == FALSE) && ( (lpObj->Authority &2 ) != 2 ) && ((lpObj->Authority & 0x20) != 0x20) ))
			{
				return 0;
			}
			pId = this->GetTokenString();
			g_CastleSiege.OperateGmCommand(lpObj->m_Index, 7, pId);
		}
		break;
		case 340:
		{
			if (((GMSystem.Enabled == TRUE) && (GMSystem.CheckCommand(lpObj,19) != TRUE)) || ((GMSystem.Enabled == FALSE) && ( (lpObj->Authority &2 ) != 2 ) && ((lpObj->Authority & 0x20) != 0x20) ))
			{
				return 0;
			}
			pId = this->GetTokenString();
			g_CastleSiege.OperateGmCommand(lpObj->m_Index, 8, pId);
		}
		break;
#endif
		case 217:	//116:
			{
				//if ( (lpObj->Authority & 2) != 2 && (lpObj->Authority & 0x20) != 0x20 )
				if (((GMSystem.Enabled == TRUE) && (GMSystem.CheckCommand(lpObj,9) != TRUE)) || ((GMSystem.Enabled == FALSE) && ( (lpObj->Authority &2 ) != 2 ) && ((lpObj->Authority & 0x20) != 0x20) ))
				{
					return 0;
				}

				wsprintf(sbuf,"[GMSystem][%s][%s] GM:[%s][%s] / Location:[MAP:%d][X:%d][Y:%d]",
					"FireWorks Effect", lpObj->Ip_addr, 
					lpObj->AccountID, lpObj->Name,
					lpObj->MapNumber,lpObj->X,lpObj->Y);
				if(ReadConfig.GMLog == TRUE)
					GM_LOG.Output(sbuf);

				int x = lpObj->X;
				int y = lpObj->Y;
				PMSG_SERVERCMD pMsg;

				PHeadSubSetB((LPBYTE)&pMsg,0xF3,0x40, sizeof(pMsg));
				pMsg.CmdType = 0;

				for ( int i=0;i<15;i++)
				{
					pMsg.X = x+(rand() % 5)*2 - 4;
					pMsg.Y = y+(rand() % 5)*2 - 4;
					MsgSendV2(lpObj,(UCHAR*)&pMsg, sizeof(pMsg));
					::DataSend(lpObj->m_Index ,(UCHAR*)&pMsg, sizeof(pMsg));
				}
			}
			break;

		case 216:	//115:
			{
				if ( (lpObj->Authority & 2) != 2 && (lpObj->Authority & 0x20) != 0x20 )
				{
					return 0;
				}

				pId = this->GetTokenString();

				if ( pId == NULL )
				{
					return 0;
				}

				LPOBJ lpTargetObj = gObjFind(pId);

				if ( lpTargetObj == NULL )
				{
					return 0;
				}

				wsprintf(sbuf,"[GMSystem][%s][%s] GM:[%s][%s] / Target:[%s][%s] / Location:[MAP:%d][X:%d][Y:%d]",
					"User Monitor", lpObj->Ip_addr, 
					lpObj->AccountID, lpObj->Name,
					lpTargetObj->AccountID, lpTargetObj->Name,
					lpTargetObj->MapNumber, lpTargetObj->X,lpTargetObj->Y);
				if(ReadConfig.GMLog == TRUE)
					GM_LOG.Output(sbuf);

				char szTemp[256];

				if ( this->WatchTargetIndex == lpTargetObj->m_Index )
				{
					this->WatchTargetIndex = -1;
					
					wsprintf(szTemp, "%s : You can not watch yourself", lpTargetObj->Name);	// #translation
					GCServerMsgStringSend(szTemp, lpObj->m_Index, 1);
				}
				else
				{
					wsprintf(szTemp, "%s : Watching User", lpTargetObj->Name);	// #translation
					GCServerMsgStringSend(szTemp, lpObj->m_Index, 1);
					this->WatchTargetIndex = lpTargetObj->m_Index;
				}
			}
			break;

		case 215:	//114:
			{
				if ( (lpObj->Authority & 2) != 2 && (lpObj->Authority & 0x20) != 0x20 )
				{
					return 0;
				}

				pId = this->GetTokenString();

				if ( pId == NULL )
				{
					return 0;
				}

				int map;
				int iX;
				int iY;
				LPOBJ lpTargetObj = gObjFind(pId);
				int iIndex;

				if ( lpTargetObj == NULL )
				{
					return 0;
				}

				wsprintf(sbuf,"[GMSystem][%s][%s] GM:[%s][%s] / Target:[%s][%s] / Location:[MAP:%d][X:%d][Y:%d]",
					"User Trace", lpObj->Ip_addr, 
					lpObj->AccountID, lpObj->Name,
					lpTargetObj->AccountID, lpTargetObj->Name,
					lpTargetObj->MapNumber, lpTargetObj->X,lpTargetObj->Y);
				
				if(ReadConfig.GMLog == TRUE)
					GM_LOG.Output(sbuf);

				map = lpTargetObj->MapNumber;
				iX = lpTargetObj->X;
				iY = lpTargetObj->Y;
				iIndex = lpObj->m_Index;

				if ( iIndex >= 0 )
				{
					gObjTeleport(iIndex, map, iX, iY);
				}
			}
			break;

		case 214:	//113:
			{
				if(lpObj->Authority == 32)
				{
				}else if ((lpObj->Authority & 2) != 2)
				{
					return 0;
				}

				wsprintf(sbuf,"[GMSystem][%s][%s] GM:[%s][%s]",
					"User Stat (connection)", lpObj->Ip_addr, 
					lpObj->AccountID, lpObj->Name);
				if(ReadConfig.GMLog == TRUE)
					GM_LOG.Output(sbuf);

				int lc151 = 0;
				int lc152 = 400;
				int iTokenNumber = this->GetTokenNumber();

				if ( iTokenNumber > 0 )
				{
					lc151 = iTokenNumber;
				}

				int iTokenNumber2 = this->GetTokenNumber();

				if ( iTokenNumber2 > 0 )
				{
					lc152 = iTokenNumber2;
				}

				gObjSendUserStatistic(lpObj->m_Index, lc151, lc152);
			}
			break;

		case 100:	//100:
			{
				if ( (lpObj->AuthorityCode &4) != 4 )
				{
					return 0;
				}

				pId = this->GetTokenString();

				if ( pId == NULL )
				{
					return 0;
				}

				int iTargetIndex = gObjGetIndex(pId);
				

				if ( iTargetIndex >= 0 )
				{
					LPOBJ lpTargetObj = gObjFind(pId);

					if ( lpTargetObj == NULL )
					{
						return 0;
					}

					wsprintf(sbuf,"[GMSystem][%s][%s] GM:[%s][%s] / Target:[%s][%s]",
						"User Disconnect", lpObj->Ip_addr, 
						lpObj->AccountID, lpObj->Name,
						lpTargetObj->AccountID,lpTargetObj->Name);
					
					if(ReadConfig.GMLog == TRUE)
						GM_LOG.Output(sbuf);

					LogAdd(lMsg.Get(MSGGET(1, 191)), pId);
					CloseClient(iTargetIndex);
				}
			}
			break;

		case 112:	//108:
			{
				if ( (lpObj->AuthorityCode &4) != 4 )
				{
					return 0;
				}

				pId = this->GetTokenString();

				if ( pId == NULL )
				{
					return 0;
				}

				wsprintf(sbuf,"[GMSystem][%s][%s] GM:[%s][%s] / Guild:[%s]",
					"Guild Disconnect", lpObj->Ip_addr, 
					lpObj->AccountID, lpObj->Name,
					pId);
				
				if(ReadConfig.GMLog == TRUE)
					GM_LOG.Output(sbuf);

				_GUILD_INFO_STRUCT * lpGuild = Guild.SearchGuild(pId);
				int iIndex;

				if ( lpGuild != NULL )
				{
					for ( int i=0;i<MAX_USER_GUILD ; i++ )
					{
						if ( lpGuild->Index[i] >= 0 )
						{
							iIndex = lpGuild->Index[i];

							if ( iIndex >= 0 )
							{
								LogAdd(lMsg.Get(MSGGET(1, 191)), pId);
								CloseClient(iIndex);
							}
						}
					}
				}
			}
			break;

		case 101:	//Move
			{
				pId = this->GetTokenString();

				if ( pId != NULL )
				{
					int lc165 = -1;
					int lc166 = 0;
					int lc167 = 0;

					if ( lpObj->Teleport != 0 )
					{
						GCServerMsgStringSend(lMsg.Get(MSGGET(6, 68)), lpObj->m_Index, 1);
						return 0;
					}

					if ( (lpObj->m_IfState.use) != 0 )
					{
						if ( lpObj->m_IfState.type  == 3 )
						{
							lpObj->TargetShopNumber = -1;
							lpObj->m_IfState.type = 0;
							lpObj->m_IfState.use = 0;
						}
					}

					if ( lpObj->m_IfState.use > 0 )
					{
						GCServerMsgStringSend(lMsg.Get(MSGGET(6, 68)), lpObj->m_Index, 1);
						return 0;
					}

					if ( gObj[aIndex].IsInBattleGround != false )
					{
						GCServerMsgStringSend(lMsg.Get(MSGGET(6, 68)), lpObj->m_Index, 1);
						return 0;
					}

					if ( lpObj->m_PK_Level >= 5 )
					{
						GCServerMsgStringSend(lMsg.Get(MSGGET(4, 101)), lpObj->m_Index, 1);
						return 0;
					}

					gMoveCommand.Move(lpObj, pId);
				}
			}
			break;

		case 108:	//104:
			{
				if ( (lpObj->AuthorityCode &8)!= 8 )
				{
					return 0;
				}

				pId = this->GetTokenString();

				if ( pId == NULL )
				{
					return 0;
				}

				int iTokenNumber1 = this->GetTokenNumber();
				int iTokenNumber2 = this->GetTokenNumber();
				int iTokenNumber3 = this->GetTokenNumber();

				wsprintf(sbuf,"[GMSystem][%s][%s] GM:[%s][%s] / Guild:[%s] / Location:[MAP:%d][X:%d][Y:%d]",
					"Guild SetPosition", lpObj->Ip_addr, 
					lpObj->AccountID, lpObj->Name,
					pId,iTokenNumber1, iTokenNumber2, iTokenNumber3);
			
				if(ReadConfig.GMLog == TRUE)
					GM_LOG.Output(sbuf);

				_GUILD_INFO_STRUCT* lpGuild = Guild.SearchGuild(pId);
				int iIndex;
			
				if ( lpGuild != NULL )
				{
					for ( int i=0;i<MAX_USER_GUILD;i++)
					{
						if (lpGuild->Index[i] >= 0 )
						{
							iIndex = lpGuild->Index[i];
							gObjTeleport(iIndex, iTokenNumber1, iTokenNumber2++, iTokenNumber3);
						}
					}
				}
			}
			break;

		case 109:	//105:
			{
				if ( (lpObj->Authority &2)== 2 )
				{
					wsprintf(sbuf,"[GMSystem][%s][%s] GM:[%s][%s]",
						"Start BattleSoccer", lpObj->Ip_addr, 
						lpObj->AccountID, lpObj->Name);
					if(ReadConfig.GMLog == TRUE)
						GM_LOG.Output(sbuf);

					BattleSoccerGoalStart(0);
				}
			}
			break;

		case 110:	//106:
			{
				if ( (lpObj->Authority &2) == 2 )
				{
					wsprintf(sbuf,"[GMSystem][%s][%s] GM:[%s][%s]",
						"Stop BattleSoccer", lpObj->Ip_addr, 
						lpObj->AccountID, lpObj->Name);
					if(ReadConfig.GMLog == TRUE)
						GM_LOG.Output(sbuf);

					BattleSoccerGoalEnd(0);
				}
				else
				{
					if ( gObj[aIndex].lpGuild != NULL )
					{
						if (gObj[aIndex].lpGuild->WarType == 1 )
						{
							strcmp(gObj[aIndex].Name, gObj[aIndex].lpGuild->Names[0] );
						}
					}
				}
			}

			break;

		case 111:	//107:
			{
				if (( (lpObj->Authority & 2) == 2 ) || (lpObj->Authority == 32))
				{
					char * szGuild = this->GetTokenString();

					if ( szGuild != NULL )
					{
						wsprintf(sbuf,"[GMSystem][%s][%s] GM:[%s][%s] / Guild:[%s]",
							"End GuildWar", lpObj->Ip_addr, 
							lpObj->AccountID, lpObj->Name,
							szGuild);
						if(ReadConfig.GMLog == TRUE)
							GM_LOG.Output(sbuf);

						GCManagerGuildWarEnd(szGuild);
					}
				}
				else
				{
					if ( gObj[aIndex].lpGuild != NULL && gObj[aIndex].lpGuild->lpTargetGuildNode != NULL)
					{
						if ( strcmp( gObj[aIndex].Name, gObj[aIndex].lpGuild->Names[0] ) ==  0)
						{
							if ( gObj[aIndex].lpGuild->BattleGroundIndex >= 0 && gObj[aIndex].lpGuild->WarType == 1 )
							{
								::gObjAddMsgSendDelay(&gObj[aIndex], 7, aIndex, 10000, 0);

								char szTemp[100];

								wsprintf(szTemp, lMsg.Get(MSGGET(4, 129)), gObj[aIndex].lpGuild->Names[0] );
								::GCServerMsgStringSendGuild(gObj[aIndex].lpGuild, szTemp, 1);
								::GCServerMsgStringSendGuild(gObj[aIndex].lpGuild->lpTargetGuildNode, szTemp, 1);
							}
						}
					}
				}
			}
			break;

		case 104:	//102:
			{
				if ( (lpObj->AuthorityCode&0x20 ) != 0x20 )
				{
					return FALSE;
				}

				pId = this->GetTokenString();

				if ( pId == NULL )
				{
					return FALSE;
				}

				wsprintf(sbuf,"[GMSystem][%s][%s] GM:[%s][%s] / Guild:[%s]",
					"Ban Chatting", lpObj->Ip_addr, 
					lpObj->AccountID, lpObj->Name,
					pId);
				if(ReadConfig.GMLog == TRUE)
					GM_LOG.Output(sbuf);

				int Index = ::gObjGetIndex(pId);

				if ( Index >= 0 )
				{
					gObj[Index].Penalty |= 2;
				}

			}
			break;

		case 106:	//103:
			{
				if ( (lpObj->AuthorityCode & 32 ) != 32 )
				{
					return FALSE;
				}

				pId = this->GetTokenString();

				if ( pId == NULL )
				{
					return FALSE;
				}

				wsprintf(sbuf,"[GMSystem][%s][%s] GM:[%s][%s] / Guild:[%s]",
					"UnBan Chatting", lpObj->Ip_addr, 
					lpObj->AccountID, lpObj->Name,
					pId);
				if(ReadConfig.GMLog == TRUE)
					GM_LOG.Output(sbuf);

				int Index = ::gObjGetIndex(pId);

				if ( Index >= 0 )
				{
					gObj[Index].Penalty &= ~2;
				}
			}
			break;

		case 200:	//109:
			{
				pId = this->GetTokenString();

				if ( pId != NULL )
				{
					if ( strlen(pId) >= 1 )
					{
						::GCGuildWarRequestResult(pId, aIndex, 0);
					}
				}
			}

			break;

		case 202:	//111:
			{
				if (( (lpObj->Authority & 2 ) == 2 ) || (lpObj->Authority == 32))
				{	
					pId = this->GetTokenString();

					if ( pId != NULL )
					{
						char * Rival = this->GetTokenString();

						if ( Rival != NULL )
						{
							if ( strlen(pId) >= 1 )
							{
								if ( strlen(Rival) >= 1 )
								{
									wsprintf(sbuf,"[GMSystem][%s][%s] GM:[%s][%s] / Guild:[%s] / Rival:[%s]",
										"Set GuildWar", lpObj->Ip_addr, 
										lpObj->AccountID, lpObj->Name,
										pId,Rival);
									if(ReadConfig.GMLog == TRUE)
										GM_LOG.Output(sbuf);

									::GCManagerGuildWarSet(pId, Rival, 1);
								}
							}
						}
					}
				}
				else
				{
					if ( gEnableBattleSoccer != FALSE )
					{
						pId = this->GetTokenString();

						if ( pId != NULL )
						{
							if ( strlen(pId) >= 1 )
							{
								::GCGuildWarRequestResult(pId, aIndex, 1);
							}
						}
					}
				}
			}

			break;

		case 201:	//110:
			{
				gObjBillRequest(&gObj[aIndex]);
			}
			break;

		case 203:	//112:
			{
				pId = this->GetTokenString();

				if ( pId != NULL )
				{
					BOOL bState;

					if ( strcmp(pId, "on" ) == 0 )
					{
						bState = TRUE;
					}
					else if ( strcmp(pId, "off") == 0 )
					{
						bState = FALSE;
					}

					if ( bState >= FALSE && bState <= TRUE )
					{
						::gObjSetTradeOption(aIndex, bState);
						//::gObjSetDuelOption(aIndex, bState);
						g_DuelManager.SetDuelOption(aIndex, bState);
					}
				}
			}
			break;

		case 320:	//117:
			{
				if ( (lpObj->Authority &2) != 2 )
				{
					return FALSE;
				}

				KUNDUN_GM_LOG.Output("[GMSystem][KUNDUN][%s]\t[%s]\t[%s] : %s", lpObj->Ip_addr, lpObj->AccountID,
					lpObj->Name, "ÄïµÐ»óÅÂº¸±â");	// #translation  Require Translation

				for ( int n=0;n<MAX_VIEWPORT;n++)
				{
					if ( lpObj->VpPlayer[n].state != 0 )
					{
						if ( lpObj->VpPlayer[n].type == OBJ_MONSTER )
						{
							if ( lpObj->VpPlayer[n].number >= 0 )
							{
								LPOBJ lpTarget = &gObj[lpObj->VpPlayer[n].number];

								if ( lpTarget->Class == 275 )
								{
									TNotice pNotice(1);

									pNotice.SendToUser(lpObj->m_Index, "Kundun HP = %7.0f / %7.0f", lpTarget->Life, lpTarget->MaxLife); //Require Translation
									pNotice.SendToUser(lpObj->m_Index, "Kundun HP time = %d Recover = %d Recovery Time = %d", 
										giKundunRefillHPSec, giKundunRefillHP, giKundunRefillHPTime);	// Require Translation
								}

								if ( lpTarget->Class == 338 )
								{
									TNotice pNotice(1);

									pNotice.SendToUser(lpObj->m_Index, "Kundun HP = %7.0f / %7.0f", lpTarget->Life, lpTarget->MaxLife); //Require Translation
									pNotice.SendToUser(lpObj->m_Index, "Kundun HP time = %d Recover = %d Recovery Time = %d", 
										giKundunRefillHPSec, giKundunRefillHP, giKundunRefillHPTime);	// Require Translation
								}
							}
						}
					}
				}

			}
			break;

		case 321:	//118:
			{
				if ( (lpObj->Authority &2 ) != 2 )
				{
					return FALSE;
				}

				KUNDUN_GM_LOG.Output("[GMSystem][KUNDUN][%s]\t[%s]\t[%s] : %s", lpObj->Ip_addr, lpObj->AccountID,
					lpObj->Name, "BOSS HP CHANGE");	// Require Translation

				int iLife = this->GetTokenNumber();

				for ( int n=0;n<MAX_VIEWPORT;n++)
				{
					if ( lpObj->VpPlayer[n].state != FALSE )
					{
						if ( lpObj->VpPlayer[n].type == OBJ_MONSTER )
						{
							if ( lpObj->VpPlayer[n].number >= 0 )
							{
								LPOBJ lpTarget = &gObj[lpObj->VpPlayer[n].number];

								if (lpTarget->Class == 275 || 
									lpTarget->Class == 338
									)
								{
									if  ( iLife <= 5000 )
									{
										iLife = 5000 ;
									}

									if ( iLife > lpTarget->MaxLife )
									{
										iLife = lpTarget->MaxLife;
									}

									lpTarget->Life = iLife;
								
									TNotice pNotice(1);

									pNotice.SendToUser(lpObj->m_Index, "BOSS HP = %7.0f / %7.0f", lpTarget->Life, lpTarget->MaxLife); //Require Translation
								}
							}
						}
					}
				}
			}
			break;

		case 322:	//119:
			{
				if ( (lpObj->Authority &2 ) != 2 )
				{
					return FALSE;
				}

				KUNDUN_GM_LOG.Output("[GMSystem][KUNDUN][%s]\t[%s]\t[%s] : %s", lpObj->Ip_addr, lpObj->AccountID,
					lpObj->Name, "ÄïµÐHPÈ¸º¹·®¼³Á¤");	// Require Translation

				int RefillHP = this->GetTokenNumber();

				if ( RefillHP <= 0 || RefillHP > 5000000 )
				{
					return 0;
				}

				giKundunRefillHP = RefillHP;

				TNotice pNotice(0);

				pNotice.SendToUser(lpObj->m_Index, "ÄïµÐ HP ÃÊ´çÈ¸º¹·® = %d È¸º¹·® = %d È¸º¹½Ã°£ = %d", giKundunRefillHPSec, giKundunRefillHP, giKundunRefillHPTime);	// Require Translation

			}

			break;

		case 323:	//120:
			{
				if ( (lpObj->Authority &2 ) != 2 )
				{
					return FALSE;
				}

				KUNDUN_GM_LOG.Output("[GMSystem][KUNDUN][%s]\t[%s]\t[%s] : %s", lpObj->Ip_addr, lpObj->AccountID,
					lpObj->Name, "ÄïµÐHPÃÊ´çÈ¸º¹·®¼³Á¤");	// Require Translation

				int RefillHPSec = this->GetTokenNumber();

				if ( RefillHPSec <= 0 || RefillHPSec > 10000 )
				{
					return 0;
				}

				giKundunRefillHPSec = RefillHPSec;

				TNotice pNotice(0);

				pNotice.SendToUser(lpObj->m_Index, "ÄïµÐ HP ÃÊ´çÈ¸º¹·® = %d È¸º¹·® = %d È¸º¹½Ã°£ = %d",
					giKundunRefillHPSec, giKundunRefillHP, giKundunRefillHPTime);	// Require Translation

			}

			break;

		case 324:	//121:
			{
				if ( (lpObj->Authority &2 ) != 2 )
				{
					return FALSE;
				}

				KUNDUN_GM_LOG.Output("[GMSystem][KUNDUN][%s]\t[%s]\t[%s] : %s", lpObj->Ip_addr, lpObj->AccountID,
					lpObj->Name, "ÄïµÐHPÈ¸º¹½Ã°£¼³Á¤");	// Require Translation

				int RefillHPTime = this->GetTokenNumber();

				if ( RefillHPTime < 0 || RefillHPTime > 60000 )
				{
					return 0;
				}

				giKundunRefillHPTime = RefillHPTime;

				TNotice pNotice(0);

				pNotice.SendToUser(lpObj->m_Index, "ÄïµÐ HP ÃÊ´çÈ¸º¹·® = %d È¸º¹·® = %d È¸º¹½Ã°£ = %d", giKundunRefillHPSec, giKundunRefillHP, giKundunRefillHPTime);	// Require Translation

			}
			break;
#if (GS_CASTLE==0)
		case 369:
			g_Kanturu.OperateGmCommand(lpObj->m_Index, 0);
			break;
		case 370:
			g_Kanturu.OperateGmCommand(lpObj->m_Index, 1);
			break;
		case 371:
			g_Kanturu.OperateGmCommand(lpObj->m_Index, 2);
			break;
		case 372:
			g_Kanturu.OperateGmCommand(lpObj->m_Index, 3);
			break;
		case 373:
			g_Kanturu.OperateGmCommand(lpObj->m_Index, 4);
			break;
		case 374:
			g_Kanturu.OperateGmCommand(lpObj->m_Index, 5);
			break;
		case 375:
			g_Kanturu.OperateGmCommand(lpObj->m_Index, 6);
			break;
		case 376:
			g_Kanturu.OperateGmCommand(lpObj->m_Index, 7);
			break;
		case 377:
			g_Kanturu.OperateGmCommand(lpObj->m_Index, 8);
			break;
		case 378:
			g_Kanturu.OperateGmCommand(lpObj->m_Index, 9);
			break;
		case 379:
			g_Kanturu.OperateGmCommand(lpObj->m_Index, 10);
			break;
		case 380:
			g_Kanturu.OperateGmCommand(lpObj->m_Index, 11);
			break;
		case 381:
			g_Kanturu.OperateGmCommand(lpObj->m_Index, 12);
			break;
		case 382:
			g_Kanturu.OperateGmCommand(lpObj->m_Index, 13);
			break;
		case 383:
			g_Kanturu.OperateGmCommand(lpObj->m_Index, 14);
			break;
		case 384:
			g_Kanturu.OperateGmCommand(lpObj->m_Index, 15);
			break;
		case 385:
			g_Kanturu.OperateGmCommand(lpObj->m_Index, 16);
			break;
		case 386:
			g_Kanturu.OperateGmCommand(lpObj->m_Index, 17);
			break;
		case 387:
			g_Kanturu.OperateGmCommand(lpObj->m_Index, 18);
			break;
		case 388:
			g_Kanturu.OperateGmCommand(lpObj->m_Index, 19);
			break;
		case 389:
			g_Kanturu.OperateGmCommand(lpObj->m_Index, 20);
			break;
#endif
		case 390:// /item
		{
			if(ReadConfig.CmdItemEnabled == 1)
			{
				if (((GMSystem.Enabled == TRUE) && (GMSystem.CheckCommand(&gObj[aIndex],2) != TRUE)) || ((GMSystem.Enabled == FALSE) && ( (lpObj->Authority &2 ) != 2 )))
				{
					return FALSE;
				}

				int type, index,ItemLevel,ItemSkill,ItemLuck,ItemOpt,ItemExc,ItemAncient;
				int dur = 0;
				type = GetTokenNumber();
				index = GetTokenNumber();
				ItemLevel = GetTokenNumber();
				ItemSkill = GetTokenNumber();
				ItemLuck = GetTokenNumber();
				ItemOpt = GetTokenNumber();
				ItemExc = GetTokenNumber();
				ItemAncient = GetTokenNumber();
				dur = GetTokenNumber();
			
				if( (type >= 0 && type <= 15) )
				{
					int Item = ItemGetNumberMake( type, index);

					if (Item == -1)
					{
						GCServerMsgStringSend("Item does not exist!", lpObj->m_Index, 1);
						return FALSE;	
					}

					ItemSerialCreateSend(aIndex, gObj[aIndex].MapNumber, gObj[aIndex].X, gObj[aIndex].Y, Item,ItemLevel,dur,ItemSkill,ItemLuck,ItemOpt,aIndex,ItemExc,ItemAncient);

					wsprintf(sbuf,"[GMSystem][%s][%s] GM:[%s][%s] / Item:[%s][%d,%d,%d,%d,%d,%d,%d,%d]",
						"Make Item", lpObj->Ip_addr, 
						lpObj->AccountID, lpObj->Name,
						ItemAttribute[Item].Name,type,index,ItemLevel,ItemSkill,ItemLuck,ItemOpt,ItemExc,ItemAncient);
					if(ReadConfig.GMLog == TRUE)
						GM_LOG.Output(sbuf);
				}
			}
		}break;
		case 391:
		{
			if(ReadConfig.CmdPostEnabled == 1)
			{
				if(lpObj->Level < ReadConfig.CmdPostLevel)
				{
					char levelmsg[100];
					sprintf(levelmsg,lMsg.Get(MSGGET(14, 64)),ReadConfig.CmdPostLevel);
					GCServerMsgStringSend(levelmsg,aIndex,1);
					return FALSE;
				}
				if(lpObj->Money < ReadConfig.CmdPostMoney)
				{
					GCServerMsgStringSend(lMsg.Get(MSGGET(14, 67)),aIndex,1);
					return FALSE;
				}
				if(ReadConfig.CmdPostAF > 0)
				{
					if(lpObj->aFloodPostCmd != 0)
					{
						GCServerMsgStringSend("You must wait before use post command again", lpObj->m_Index, 1);
						return FALSE;	
					}
					lpObj->aFloodPostCmd = ReadConfig.CmdPostAF;
				}

				char * strtmp = (char*)szCmd+strlen(szCmdToken);

				if(strtmp[0] == 0 || strtmp[1] == 0)
				{
					return FALSE;
				}

				lpObj->Money -= ReadConfig.CmdPostMoney;
				GCMoneySend(lpObj->m_Index,lpObj->Money);

				if(ReadConfig.SCFPSON == 1)
				{
					SCFPostSend(lpObj->m_Index,(char*)szCmd+strlen(szCmdToken)+1);
				}else
				{
					ServerMsgSendPost(ReadConfig.CmdPostColor ,lpObj->Name," [POST] %s",(char*)szCmd+strlen(szCmdToken));
				}
				if(ReadConfig.PostLog == TRUE)
					POST_LOG.Output("[Post] <%s> %s", lpObj->Name, (char*)szCmd+strlen(szCmdToken));

				if (lpObj->AuthorityCode > 0)
				{
					if(ReadConfig.GMLog == TRUE)
					{
						GM_LOG.Output("[Post][%s][%s] %s", lpObj->AccountID, lpObj->Name, (char*)szCmd+strlen(szCmdToken));
					}
				}
				//
						
				//char timeStr[9];
				//_strtime(timeStr);
				//char iPostLog[200];
				//sprintf(iPostLog, "%s [Post] <%s> %s", timeStr, lpObj->Name, (char*)szCmd+strlen(szCmdToken));
				//LogAddC(iPostLog);

				return TRUE;
			}else
			{
				return TRUE;
			}
		}
		case 392:
		{
			if (((GMSystem.Enabled == TRUE) && (GMSystem.CheckCommand(lpObj,0) == TRUE)) || ((GMSystem.Enabled == FALSE) && ( (lpObj->AuthorityCode &8 ) == 8 )))
			{
				pId = this->GetTokenString();
				if ( pId == NULL )
				{
					return 0;
				}
				int targetIndex = gObjGetIndex(pId);
				LPOBJ lpTargetObj;
				if ( targetIndex >= 0 )
				{
					lpTargetObj = gObjFind(pId);
					if ( lpTargetObj == NULL )
					{
						return 0;
					}
					GCPkLevelSend(lpTargetObj->m_Index, 3);
					char Msg[100] = {0};
					strcpy(Msg,lMsg.Get(MSGGET(14, 65)));
					GCServerMsgStringSend(Msg,lpTargetObj->m_Index,1);

					wsprintf(sbuf,"[GMSystem][%s][%s] GM:[%s][%s] / Target:[%s][%s]",
						"PK Clear", lpObj->Ip_addr, 
						lpObj->AccountID, lpObj->Name,
						gObj[targetIndex].AccountID,gObj[targetIndex].Name);
					if(ReadConfig.GMLog == TRUE)
						GM_LOG.Output(sbuf);
				}
			}else
			{
				if (ReadConfig.CmdPKClearEnabled == 1)
				{
					int Money = (lpObj->m_PK_Level * ReadConfig.CmdPKClearMoney);
					if(lpObj->m_PK_Level <= 3) 
					{
						char Msg[100] = {0};
						strcpy(Msg,lMsg.Get(MSGGET(14, 66)));
						GCServerMsgStringSend(Msg,aIndex,1);
						return TRUE;
					}
					else if(lpObj->Money < Money)
					{
						GCServerMsgStringSend(lMsg.Get(MSGGET(14, 67)),aIndex,1);
						return TRUE;
					}
					else if(lpObj->Money >= Money)
					{
						lpObj->Money -= Money;
						GCMoneySend(lpObj->m_Index,lpObj->Money);
						lpObj->m_PK_Level = 3;
						GCPkLevelSend(lpObj->m_Index, 3);
						GCServerMsgStringSend(lMsg.Get(MSGGET(14, 65)),aIndex,1);
						LogAddTD("[PK Clear][%s][%s] PK clear success.", lpObj->AccountID, lpObj->Name);
					}
					return TRUE;
				}
			}
			return FALSE;
		}
			break;
		case 393:
		{
			if(ReadConfig.CmdAddEnabled == 1)
			{
				int Pontos;
				Pontos = GetTokenNumber();
				if(Pontos == NULL)
				{
					GCServerMsgStringSend(lMsg.Get(MSGGET(14, 68)),lpObj->m_Index,1);
					return FALSE;
				}
				if(lpObj->LevelUpPoint < Pontos || Pontos < 1)
				{
					GCServerMsgStringSend(lMsg.Get(MSGGET(14, 69)),lpObj->m_Index,1);
					return FALSE;
				}
				if(lpObj->Money < ReadConfig.CmdAddMoney)
				{
					GCServerMsgStringSend(lMsg.Get(MSGGET(14, 67)),aIndex,1);
					return FALSE;
				}
				if(Pontos > ReadConfig.CmdAddMaxNumber)
				{
					char sbuff[1024]={0};
					wsprintf(sbuff,lMsg.Get(MSGGET(14, 77)),ReadConfig.CmdAddMaxNumber);
					GCServerMsgStringSend(sbuff,lpObj->m_Index,1);
					return FALSE;
				}
				if((Pontos + lpObj->Strength) > ReadConfig.MaxPoints[lpObj->Class][0])
				{
					char sbuff[1024]={0};
					wsprintf(sbuff,lMsg.Get(MSGGET(14, 78)),ReadConfig.CmdAddMaxNumber);
					GCServerMsgStringSend(sbuff,lpObj->m_Index,1);
					return FALSE;
				}

				lpObj->Money -= ReadConfig.CmdAddMoney;
				GCMoneySend(lpObj->m_Index,lpObj->Money);
				lpObj->LevelUpPoint -= Pontos;
				lpObj->Strength += Pontos;
				char Msg[100];
				sprintf(Msg, lMsg.Get(MSGGET(14, 71)), Pontos);
				GCServerMsgStringSend(Msg,lpObj->m_Index,1);
				GCServerMsgStringSend(lMsg.Get(MSGGET(14, 70)),lpObj->m_Index,1);

				return TRUE;
			}
			return FALSE;
		}
			break;
		case 394:
		{
			if(ReadConfig.CmdAddEnabled == 1)
			{
				int Pontos;
				Pontos = GetTokenNumber();
				if(Pontos == NULL)
				{
					GCServerMsgStringSend(lMsg.Get(MSGGET(14, 68)),lpObj->m_Index,1);
					return FALSE;
				}
				if(lpObj->LevelUpPoint < Pontos || Pontos < 1)
				{
					GCServerMsgStringSend(lMsg.Get(MSGGET(14, 69)),lpObj->m_Index,1);
					return FALSE;
				}
				if(lpObj->Money < ReadConfig.CmdAddMoney)
				{
					GCServerMsgStringSend(lMsg.Get(MSGGET(14, 67)),aIndex,1);
					return FALSE;
				}
				if(Pontos > ReadConfig.CmdAddMaxNumber)
				{
					char sbuff[1024]={0};
					wsprintf(sbuff,lMsg.Get(MSGGET(14, 77)),ReadConfig.CmdAddMaxNumber);
					GCServerMsgStringSend(sbuff,lpObj->m_Index,1);
					return FALSE;
				}
				if((Pontos + lpObj->Dexterity) > ReadConfig.MaxPoints[lpObj->Class][1])
				{
					char sbuff[1024]={0};
					wsprintf(sbuff,lMsg.Get(MSGGET(14, 78)),ReadConfig.CmdAddMaxNumber);
					GCServerMsgStringSend(sbuff,lpObj->m_Index,1);
					return FALSE;
				}
				lpObj->Money -= ReadConfig.CmdAddMoney;
				GCMoneySend(lpObj->m_Index,lpObj->Money);
				lpObj->LevelUpPoint -= Pontos;
				lpObj->Dexterity += Pontos;
				char Msg[100];
				sprintf(Msg, lMsg.Get(MSGGET(14, 72)), Pontos);
				GCServerMsgStringSend(Msg,lpObj->m_Index,1);
				GCServerMsgStringSend(lMsg.Get(MSGGET(14, 70)),lpObj->m_Index,1);
				return TRUE;
			}
			return FALSE;
		}
			break;
		case 395:
		{
			if(ReadConfig.CmdAddEnabled == 1)
			{
				int Pontos;
				Pontos = GetTokenNumber();
				if(Pontos == NULL)
				{
					GCServerMsgStringSend(lMsg.Get(MSGGET(14, 68)),lpObj->m_Index,1);
					return FALSE;
				}
				if(lpObj->LevelUpPoint < Pontos || Pontos < 1)
				{
					GCServerMsgStringSend(lMsg.Get(MSGGET(14, 69)),lpObj->m_Index,1);
					return FALSE;
				}
				if(lpObj->Money < ReadConfig.CmdAddMoney)
				{
					GCServerMsgStringSend(lMsg.Get(MSGGET(14, 67)),aIndex,1);
					return FALSE;
				}
				if(Pontos > ReadConfig.CmdAddMaxNumber)
				{
					char sbuff[1024]={0};
					wsprintf(sbuff,lMsg.Get(MSGGET(14, 77)),ReadConfig.CmdAddMaxNumber);
					GCServerMsgStringSend(sbuff,lpObj->m_Index,1);
					return FALSE;
				}
				if((Pontos + lpObj->Vitality) > ReadConfig.MaxPoints[lpObj->Class][2])
				{
					char sbuff[1024]={0};
					wsprintf(sbuff,lMsg.Get(MSGGET(14, 78)),ReadConfig.CmdAddMaxNumber);
					GCServerMsgStringSend(sbuff,lpObj->m_Index,1);
					return FALSE;
				}
				lpObj->Money -= ReadConfig.CmdAddMoney;
				GCMoneySend(lpObj->m_Index,lpObj->Money);
				lpObj->LevelUpPoint -= Pontos;
				lpObj->Vitality += Pontos;
				char Msg[100];
				sprintf(Msg, lMsg.Get(MSGGET(14, 73)), Pontos);
				GCServerMsgStringSend(Msg,lpObj->m_Index,1);
				GCServerMsgStringSend(lMsg.Get(MSGGET(14, 70)),lpObj->m_Index,1);
				return TRUE;
			}
			return FALSE;
		}
			break;
		case 396:
		{
			if(ReadConfig.CmdAddEnabled == 1)
			{
				int Pontos;
				Pontos = GetTokenNumber();
				if(Pontos == NULL)
				{
					GCServerMsgStringSend(lMsg.Get(MSGGET(14, 68)),lpObj->m_Index,1);
					return FALSE;
				}
				if(lpObj->LevelUpPoint < Pontos || Pontos < 1)
				{
					GCServerMsgStringSend(lMsg.Get(MSGGET(14, 69)),lpObj->m_Index,1);
					return FALSE;
				}
				if(lpObj->Money < ReadConfig.CmdAddMoney)
				{
					GCServerMsgStringSend(lMsg.Get(MSGGET(14, 67)),aIndex,1);
					return FALSE;
				}
				if(Pontos > ReadConfig.CmdAddMaxNumber)
				{
					char sbuff[1024]={0};
					wsprintf(sbuff,lMsg.Get(MSGGET(14, 77)),ReadConfig.CmdAddMaxNumber);
					GCServerMsgStringSend(sbuff,lpObj->m_Index,1);
					return FALSE;
				}
				if((Pontos + lpObj->Energy) > ReadConfig.MaxPoints[lpObj->Class][3])
				{
					char sbuff[1024]={0};
					wsprintf(sbuff,lMsg.Get(MSGGET(14, 78)),ReadConfig.CmdAddMaxNumber);
					GCServerMsgStringSend(sbuff,lpObj->m_Index,1);
					return FALSE;
				}
				lpObj->Money -= ReadConfig.CmdAddMoney;
				GCMoneySend(lpObj->m_Index,lpObj->Money);
				lpObj->LevelUpPoint -= Pontos;
				lpObj->Energy += Pontos;
				char Msg[100];
				sprintf(Msg, lMsg.Get(MSGGET(14, 74)), Pontos);
				GCServerMsgStringSend(Msg,lpObj->m_Index,1);
				GCServerMsgStringSend(lMsg.Get(MSGGET(14, 70)),lpObj->m_Index,1);
				return TRUE;
			}
			return FALSE;
		}
			break;
		case 397:
		{
			if(ReadConfig.CmdAddEnabled == 1)
			{
				if(lpObj->Class !=4)
				{
					GCServerMsgStringSend(lMsg.Get(MSGGET(14, 76)),lpObj->m_Index,1);
					return FALSE;
				}

				int Pontos;
				Pontos = GetTokenNumber();
				if(Pontos == NULL)
				{
					GCServerMsgStringSend(lMsg.Get(MSGGET(14, 68)),lpObj->m_Index,1);
					return FALSE;
				}
				if(lpObj->LevelUpPoint < Pontos || Pontos < 1)
				{
					GCServerMsgStringSend(lMsg.Get(MSGGET(14, 69)),lpObj->m_Index,1);
					return FALSE;
				}
				if(lpObj->Money < ReadConfig.CmdAddMoney)
				{
					GCServerMsgStringSend(lMsg.Get(MSGGET(14, 67)),aIndex,1);
					return FALSE;
				}
				if(Pontos > ReadConfig.CmdAddMaxNumber)
				{
					char sbuff[1024]={0};
					wsprintf(sbuff,lMsg.Get(MSGGET(14, 77)),ReadConfig.CmdAddMaxNumber);
					GCServerMsgStringSend(sbuff,lpObj->m_Index,1);
					return FALSE;
				}
				if((Pontos + lpObj->Leadership) > ReadConfig.MaxPoints[lpObj->Class][4])
				{
					char sbuff[1024]={0};
					wsprintf(sbuff,lMsg.Get(MSGGET(14, 78)),ReadConfig.CmdAddMaxNumber);
					GCServerMsgStringSend(sbuff,lpObj->m_Index,1);
					return FALSE;
				}
				lpObj->Money -= ReadConfig.CmdAddMoney;
				GCMoneySend(lpObj->m_Index,lpObj->Money);
				lpObj->LevelUpPoint -= Pontos;
				lpObj->Leadership += Pontos;
				char Msg[100];
				sprintf(Msg, lMsg.Get(MSGGET(14, 75)), Pontos);
				GCServerMsgStringSend(Msg,lpObj->m_Index,1);
				GCServerMsgStringSend(lMsg.Get(MSGGET(14, 70)),lpObj->m_Index,1);
				return TRUE;
			}
			return FALSE;
		}
			break;
		case 398:
		{
			if (ReadConfig.CmdSkinEnabled == 1)
			{
				if (((GMSystem.Enabled == TRUE) && (GMSystem.CheckCommand(&gObj[aIndex],3) == TRUE)) || ((GMSystem.Enabled == FALSE) && ( (lpObj->Authority &2 ) == 2 )))
				{					
					pId = this->GetTokenString();
					if ( pId == NULL )
					{
						return 0;
					}
					int Mob = this->GetTokenNumber();
					int targetIndex = gObjGetIndex(pId);
					LPOBJ lpTargetObj;
					if ( targetIndex >= 0 )
					{
						lpTargetObj = gObjFind(pId);
						if ( lpTargetObj == NULL )
						{
							return 0;
						}
						lpTargetObj->m_Change = Mob;
						gObjViewportListProtocolCreate(lpTargetObj);	

						wsprintf(sbuf,"[GMSystem][%s][%s] GM:[%s][%s] / Target:[%s][%s] / Skin:%d",
							"User Skin", lpObj->Ip_addr, 
							lpObj->AccountID, lpObj->Name,
							gObj[targetIndex].AccountID,gObj[targetIndex].Name,Mob);
						if(ReadConfig.GMLog == TRUE)
							GM_LOG.Output(sbuf);
						return 1;
					}					
				}else
				{
					if(ReadConfig.CmdSkinOnlyForGM != TRUE)
					{
						int Mob;
						Mob = GetTokenNumber();
						
						if(lpObj->Money < ReadConfig.CmdSkinMoney)
						{
							GCServerMsgStringSend(lMsg.Get(MSGGET(14, 67)),aIndex,1);
							return TRUE;
						}
						else if(lpObj->Level < ReadConfig.CmdSkinLevel)
						{
							GCServerMsgStringSend(lMsg.Get(MSGGET(14, 79)),aIndex,1);
							return TRUE;
						}
						else if(lpObj->Money >= ReadConfig.CmdSkinMoney)
						{
							lpObj->Money -= ReadConfig.CmdSkinMoney;
							GCMoneySend(lpObj->m_Index,lpObj->Money);
							lpObj->m_Change = Mob;
							gObjViewportListProtocolCreate(lpObj);		

							LogAddTD("[Skin Command][%s][%s] Success.", lpObj->AccountID, lpObj->Name);
						}
						return TRUE;
					}
				}
			}
			return FALSE;
		}
		break;

		case 431:
		{
			if (ReadConfig.CmdSkinEnabled == 1)
			{
				if (((GMSystem.Enabled == TRUE) && (GMSystem.CheckCommand(&gObj[aIndex],3) == TRUE)) || ((GMSystem.Enabled == FALSE) && ( (lpObj->Authority &2 ) == 2 )))
				{					
					int targetIndex = this->GetTokenNumber();
					int Mob = this->GetTokenNumber();
					LPOBJ lpTargetObj;
					if ( (targetIndex >= 0) && (targetIndex <= OBJ_MAXMONSTER) )
					{
						lpTargetObj = &gObj[targetIndex];

						if ( lpTargetObj->Live )
						{
							lpTargetObj->m_Change = Mob;
							gObjViewportListProtocolCreate(lpTargetObj);	

							wsprintf(sbuf,"[GMSystem][%s][%s] GM:[%s][%s] / Target:[%s][%s] / Skin:%d",
								"User Skin", lpObj->Ip_addr, 
								lpObj->AccountID, lpObj->Name,
								gObj[targetIndex].AccountID,gObj[targetIndex].Name,Mob);
							if(ReadConfig.GMLog == TRUE)
								GM_LOG.Output(sbuf);
							return 1;
						}
					}					
				}
			}
		}
		break;

#if (PACK_EDITION>=2)
		case 399:
		{		
			if(Marry.EnableMarry == TRUE)
			{
				pId = this->GetTokenString();

				if ( pId == NULL )
				{
					return 0;
				}
				Marry.Propose(lpObj,pId);
				return TRUE;
			}
			return FALSE;
		}break;
		case 400:
		{			
			Marry.CommandCore(lpObj,0);
			return TRUE;
		}break;
		case 401:
		{			
			Marry.CommandCore(lpObj,1);
			return TRUE;
		}break;
		case 402:
		{			
			Marry.CommandCore(lpObj,2);
			return TRUE;
		}break;
		case 403:
		{			
			Marry.CommandCore(lpObj,3);
			return TRUE;
		}break;
		case 404:
		{			
			Marry.CommandCore(lpObj,4);
			return TRUE;
		}break;
		#if (DSGN_RESET_SYSTEM==0)
		case 405:
		{			
			ResetChar.Start(lpObj);
			return TRUE;
		}break;
		#endif	//DSGN_RESET_SYSTEM
#endif

#if (DSGN_RESET_SYSTEM==1)
		case 405:
		{			
			//ResetChar.Start(lpObj);
			return TRUE;
		}break;
#endif	//DSGN_RESET_SYSTEM

		case 406:
		{
			if ( (lpObj->AuthorityCode &8) == 8 )
			{
				pId = this->GetTokenString();
				if ( pId == NULL )
				{
					return 0;
				}
				int iTokenNumber1 = this->GetTokenNumber();
				int iTokenNumber2 = this->GetTokenNumber();
				int iTokenNumber3 = this->GetTokenNumber();
				int iIndex = gObjGetIndex(pId);
				LPOBJ lpTargetObj;
				if ( iIndex >= 0 )
				{
					lpTargetObj = gObjFind(pId);
					if ( lpTargetObj == NULL )
					{
						return 0;
					}

					wsprintf(sbuf,"[GMSystem][%s][%s] GM:[%s][%s] / Target:[%s][%s] / Location:[MAP:%d][X:%d][Y:%d] %d",
						"User SetPosition", lpObj->Ip_addr, 
						lpObj->AccountID, lpObj->Name,
						lpTargetObj->AccountID,lpTargetObj->Name,
						iTokenNumber1, iTokenNumber2, iTokenNumber3,USERSTATUS);
					if(ReadConfig.GMLog == TRUE)
						GM_LOG.Output(sbuf);

					gObjTeleport(iIndex, iTokenNumber1, iTokenNumber2, iTokenNumber3);
				}
				return 1;
			}
		}break;
	
#if (PACK_EDITION>=2)
		case 407:
		{		
			if(lpObj->Vip > 0)
			{
				pId = this->GetTokenString();

				if ( pId == NULL )
				{
					return 0;
				}
				VipSystem.BuyList(lpObj,pId);
				return TRUE;
			}
			return FALSE;
		}break;
		case 408:
		{		
			if(lpObj->Vip > 0)
			{
				int iTokenNumber = this->GetTokenNumber();
				VipSystem.BuyItem(lpObj,iTokenNumber);
				return TRUE;
			}
			return FALSE;
		}break;
		case 409:
		{		
			if(lpObj->Vip > 0)
			{
				int iTokenNumber = this->GetTokenNumber();
				if(iTokenNumber > 0)
				{
					VipSystem.BuyDays(lpObj,iTokenNumber);
					return TRUE;
				}
			}
			return FALSE;
		}break;
		case 410:
		{		
			if(lpObj->Vip > 0)
			{
				VipSystem.State(lpObj);
				return TRUE;
			}
			return FALSE;
		}break;
#endif
		case 411:
		{	
			char wcTmp[512]={0};
			wsprintf(wcTmp, "%s: %s - v%s", GSText,ProjectName, VersionName);
			GCServerMsgStringSend(wcTmp,lpObj->m_Index, 0x01);
			GCServerMsgStringSend(Creators, lpObj->m_Index, 0x00);
			GCServerMsgStringSend(EMails, lpObj->m_Index, 0x00);
			GCServerMsgStringSend(WebPage, lpObj->m_Index, 0x00);
			return TRUE;
		}break;
		case 412:
		{	
			//if ( (lpObj->AuthorityCode &8) == 8 )
			if (((GMSystem.Enabled == TRUE) && (GMSystem.CheckCommand(lpObj,17) == TRUE)) || ((GMSystem.Enabled == FALSE) && ( (lpObj->AuthorityCode &8 ) == 8 )))
			{
				char sbuf[512]={0};
				pId = this->GetTokenString();
				if ( pId == NULL )
				{
					return 0;
				}
				int iIndex = gObjGetIndex(pId);
				LPOBJ lpTargetObj;
				if ( iIndex >= 0 )
				{
					lpTargetObj = gObjFind(pId);
					if ( lpTargetObj == NULL )
					{
						return 0;
					}

					wsprintf(sbuf,"[GMSystem][%s][%s] GM:[%s][%s] / Target:[%s][%s]",
						"User Player Info", lpObj->Ip_addr, 
						lpObj->AccountID, lpObj->Name,
						lpTargetObj->AccountID,lpTargetObj->Name);
					if(ReadConfig.GMLog == TRUE)
						GM_LOG.Output(sbuf);

					wsprintf(sbuf,"Character: %s - Level: %d - Class: %d",pId,lpTargetObj->Level,lpTargetObj->DbClass);
					if(ReadConfig.GMLog == TRUE)
						GM_LOG.Output(sbuf);
					GCServerMsgStringSend(sbuf,lpObj->m_Index, 0x01);
					wsprintf(sbuf,"Str:%d - Dex:%d - vit:%d - Ene:%d - Cmd:%d",lpTargetObj->Strength,lpTargetObj->Dexterity,lpTargetObj->Vitality,lpTargetObj->Energy,lpTargetObj->Leadership);
					if(ReadConfig.GMLog == TRUE)
						GM_LOG.Output(sbuf);
					GCServerMsgStringSend(sbuf,lpObj->m_Index, 0x01);
					wsprintf(sbuf,"Zen:%d",lpTargetObj->Money);
					if(ReadConfig.GMLog == TRUE)
						GM_LOG.Output(sbuf);
					GCServerMsgStringSend(sbuf,lpObj->m_Index, 0x01);
				}
				return 1;
			}
		}break;
		case 413:
		{	
			//if ( (lpObj->AuthorityCode &8) == 8 )
			if (((GMSystem.Enabled == TRUE) && (GMSystem.CheckCommand(lpObj,4) == TRUE)) || ((GMSystem.Enabled == FALSE) && ( (lpObj->AuthorityCode &8 ) == 8 )))
			{
				char sbuf[512]={0};
				pId = this->GetTokenString();
				
				wsprintf(sbuf, "[%s]%s",lpObj->Name,(char*)szCmd+strlen(szCmdToken));

				if(ReadConfig.SCFPSON == FALSE)
					AllSendServerMsg(sbuf);
				else
					SCFAllServerMsgSend(sbuf);

				if(ReadConfig.GMLog == TRUE)
					GM_LOG.Output(sbuf);
				return 1;
			}
		}break;
		case 414:
		{	
			//if ( (lpObj->AuthorityCode &8) == 8 )
			if (((GMSystem.Enabled == TRUE) && (GMSystem.CheckCommand(lpObj,5) == TRUE)) || ((GMSystem.Enabled == FALSE) && ( (lpObj->AuthorityCode &8 ) == 8 )))
			{
				int iTokenNumber1 = this->GetTokenNumber();
				int iTokenNumber2 = this->GetTokenNumber();
				int iTokenNumber3 = this->GetTokenNumber();

				wsprintf(sbuf,"[GMSystem][%s][%s] GM:[%s][%s] / Location:[MAP:%d][X:%d][Y:%d]",
					"All User SetPosition", lpObj->Ip_addr, 
					lpObj->AccountID, lpObj->Name,
					iTokenNumber1, iTokenNumber2, iTokenNumber3);
				if(ReadConfig.GMLog == TRUE)
					GM_LOG.Output(sbuf);

				for(int iIndex=OBJ_STARTUSERINDEX;iIndex<OBJMAX;iIndex++)
				{
					if(gObj[iIndex].Connected == PLAYER_PLAYING )
					{
						if(iIndex != lpObj->m_Index)
						{
							gObjTeleport(iIndex, iTokenNumber1, iTokenNumber2, iTokenNumber3);
							return 1;
						}
					}
				}
			}
			return 0;
		}break;
		case 415:
		{	
			char wcTmp[256]={0};
			wsprintf(wcTmp, "Current Level: %d", lpObj->Level);
			GCServerMsgStringSend(wcTmp,lpObj->m_Index, 0x01);
			return 1;
		}break;
		case 416:
		{	
			//if ( (lpObj->AuthorityCode &8) == 8 )
			if (((GMSystem.Enabled == TRUE) && (GMSystem.CheckCommand(lpObj,15) == TRUE)) || ((GMSystem.Enabled == FALSE) && ( (lpObj->AuthorityCode &8 ) == 8 )))
			{
				char sbuf[512]={0};
				pId = this->GetTokenString();

				if ( pId == NULL )
				{
					return 0;
				}

				int iIndex = gObjGetIndex(pId);

				if ( iIndex >= 0 )
				{
					if(gObj[iIndex].Connected == PLAYER_PLAYING )
					{
						wsprintf(sbuf,"[GMSystem][%s][%s] GM:[%s][%s] / Target:[%s][%s]",
							"User GetInfo", lpObj->Ip_addr, 
							lpObj->AccountID, lpObj->Name,
							gObj[iIndex].AccountID, gObj[iIndex].Name);
						if(ReadConfig.GMLog == TRUE)
							GM_LOG.Output(sbuf);

						wsprintf(sbuf, "Account: %s / Vault PWD: %d",gObj[iIndex].AccountID , gObj[iIndex].WarehousePW);
						if(ReadConfig.GMLog == TRUE)
							GM_LOG.Output(sbuf);
						GCServerMsgStringSend(sbuf,lpObj->m_Index, 0x00);
						return 1;
					}
				}
			}
			return 0;
		}break;
		
		case 417:
		{	
			//if ( (lpObj->AuthorityCode &8) == 8 )
			if (((GMSystem.Enabled == TRUE) && (GMSystem.CheckCommand(lpObj,16) == TRUE)) || ((GMSystem.Enabled == FALSE) && ( (lpObj->AuthorityCode &8 ) == 8 )))
			{
				char sbuf[512]={0};
				int GMs = 0;
				int Players = 0;
				
				for(int iIndex=OBJ_STARTUSERINDEX;iIndex<OBJMAX;iIndex++)
				{
					if(gObj[iIndex].Connected >= PLAYER_PLAYING )
					{
						if(gObj[iIndex].Authority >= 16)
						{
							GMs++;
						}
						Players++;
					}	
				}

				wsprintf(sbuf, "Total Players Connected: %d", Players);
				GCServerMsgStringSend(sbuf,lpObj->m_Index, 0x01);
				wsprintf(sbuf, "Total GM's Connected: %d", GMs);
				GCServerMsgStringSend(sbuf,lpObj->m_Index, 0x01);
				return 1;
			}
			return 0;
		}break;
		
		case 418:
		{	
			//if ( (lpObj->AuthorityCode &8) == 8 )
			if (((GMSystem.Enabled == TRUE) && (GMSystem.CheckCommand(lpObj,7) == TRUE)) || ((GMSystem.Enabled == FALSE) && ( (lpObj->AuthorityCode &8 ) == 8 )))
			{
				char sbuf[512]={0};
				pId = this->GetTokenString();
				int buffId = this->GetTokenNumber();

				if ( pId == NULL )
				{
					return 0;
				}

				int iIndex = gObjGetIndex(pId);
				LPOBJ lpTargetObj;

				if ( iIndex >= 0 )
				{
					lpTargetObj = gObjFind(pId);
					if ( lpTargetObj == NULL )
					{
						return 0;
					}
					switch(buffId)
					{
						case 0:
						{
							lpTargetObj->m_WizardSkillDefense = ReadConfig.WizardMagicDefenseBase + (lpObj->Dexterity + lpObj->AddDexterity) / ReadConfig.WizardMagicDefenseDiv1 + (lpObj->Energy + lpObj->AddEnergy) / ReadConfig.WizardMagicDefenseDiv2;
							if (lpTargetObj->m_WizardSkillDefense > ReadConfig.WizardMagicMaximumDefense)
								lpTargetObj->m_WizardSkillDefense = ReadConfig.WizardMagicMaximumDefense;
							lpTargetObj->m_WizardSkillDefense += ((lpTargetObj->m_WizardSkillDefense * lpObj->MasterCharacterInfo->IncSoulBarrier)/100.0f);

							lpTargetObj->m_WizardSkillDefenseTime = ReadConfig.WizardMagicDefenseTimeBase + (lpObj->Energy + lpObj->AddEnergy) / ReadConfig.WizardMagicDefenseTimeDiv1;
							if (lpTargetObj->m_WizardSkillDefenseTime > ReadConfig.WizardMagicDefenseTimeLimit*60)
								lpTargetObj->m_WizardSkillDefenseTime = ReadConfig.WizardMagicDefenseTimeLimit*60;

							lpTargetObj->m_WizardSkillDefenseTime += lpObj->MasterCharacterInfo->IncSoulBarrierTime;

							lpTargetObj->m_ViewSkillState |= 256;
							GCMagicAttackNumberSend(lpTargetObj,AT_SKILL_MAGICDEFENSE,lpTargetObj->m_Index,1);
						}break;
						case 1:
						{								
							lpObj->m_iSkillNPCHelpTime = GetTickCount();
							lpObj->m_iSkillNPCDefense = lpTargetObj->Level / 5 + 50;
							lpObj->m_iSkillNPCAttack = lpTargetObj->Level / 3 + 45;
							lpObj->m_ViewSkillState |= 131072;
							GCSkillInfoSend(lpTargetObj, 1, 0x03);
						}break;
						case 2:
						{								
							lpObj->m_SkillAddLifeTime = (lpTargetObj->AddEnergy + lpTargetObj->Energy) / 10 + 60;
							lpObj->m_SkillAddLife = (lpTargetObj->AddEnergy + lpTargetObj->Energy) /20 + (lpTargetObj->Vitality + lpTargetObj->AddVitality) /100 + 13;
							lpObj->m_ViewSkillState |= 16;
							lpObj->AddLife += lpTargetObj->m_SkillAddLife;
							GCReFillSend(lpTargetObj->m_Index,lpTargetObj->MaxLife + lpTargetObj->AddLife,0xFE,0,lpTargetObj->iMaxShield + lpTargetObj->iAddShield);							
							GCMagicAttackNumberSend(lpTargetObj,AT_SKILL_KNIGHTADDLIFE,lpTargetObj->m_Index,1);
						}break;
					}

					wsprintf(sbuf,"[GMSystem][%s][%s] GM:[%s][%s] / Target:[%s][%s] / Buff:%d",
						"User AddBuff", lpObj->Ip_addr, 
						lpObj->AccountID, lpObj->Name,
						lpTargetObj->AccountID, lpTargetObj->Name,
						buffId);
					if(ReadConfig.GMLog == TRUE)
						GM_LOG.Output(sbuf);

					GCServerMsgStringSend("Buff Added",lpObj->m_Index, 0x01);
					return 1;
				}
			}
			return 0;
		}break;
		
		case 419:
		{	
			//if ( (lpObj->AuthorityCode &8) == 8 )
			if (((GMSystem.Enabled == TRUE) && (GMSystem.CheckCommand(lpObj,8) == TRUE)) || ((GMSystem.Enabled == FALSE) && ( (lpObj->AuthorityCode &8 ) == 8 )))
			{
				char sbuf[512]={0};
				pId = this->GetTokenString();
				int Number = this->GetTokenNumber();

				if ( pId == NULL )
				{
					return 0;
				}

				int iIndex = gObjGetIndex(pId);
				LPOBJ lpTargetObj;

				if ( iIndex >= 0 )
				{
					lpTargetObj = gObjFind(pId);
					if ( lpTargetObj == NULL )
					{
						return 0;
					}
					switch(Number)
					{
						case 0:
						{
							DeleteAllItemsLessInv(lpTargetObj->m_Index);
						}break;
						case 1:
						{
							DeleteAllItems(lpTargetObj->m_Index);
						}break;
					}

					wsprintf(sbuf,"[GMSystem][%s][%s] GM:[%s][%s] / Target : [%s][%s]",
						"Delete Inventory", lpObj->Ip_addr, 
						lpObj->AccountID, lpObj->Name,
						lpTargetObj->AccountID, lpTargetObj->Name);
					if(ReadConfig.GMLog == TRUE)
						GM_LOG.Output(sbuf);
					return 1;
				}
			}
			return 0;
		}break;
		
		case 420:
		{	
			//if ( (lpObj->AuthorityCode &8) == 8 )
			if (((GMSystem.Enabled == TRUE) && (GMSystem.CheckCommand(lpObj,10) == TRUE)) || ((GMSystem.Enabled == FALSE) && ( (lpObj->AuthorityCode &8 ) == 8 )))
			{
				char sbuf[512]={0};
				pId = this->GetTokenString();
				int Number = this->GetTokenNumber();
				int AddSkill = this->GetTokenNumber();

				if ( pId == NULL )
				{
					return 0;
				}

				int iIndex = gObjGetIndex(pId);
				LPOBJ lpTargetObj;

				if ( iIndex >= 0 )
				{
					lpTargetObj = gObjFind(pId);
					if ( lpTargetObj == NULL )
					{
						return 0;
					}
					if(AddSkill == 1)
					{
						int iAddSkillPosition = gObjMagicAdd(lpTargetObj,Number,0);
						if(iAddSkillPosition >= 0)
						{
							GCMagicListOneSend(lpTargetObj->m_Index,iAddSkillPosition,Number,0xDC,0,0);
						}
						GCServerMsgStringSend("Skill Added",lpObj->m_Index, 0x01);
					}else
					{
						int iDelSkillPosition = gObjMagicDel(lpTargetObj,Number,0);
						if(iDelSkillPosition >= 0)
						{
							GCMagicListOneDelSend(lpTargetObj->m_Index,iDelSkillPosition,Number,0xDC,0,0);
						}
						GCServerMsgStringSend("Skill Deleted",lpObj->m_Index, 0x01);						
					}

					wsprintf(sbuf,"[GMSystem][%s][%s] GM:[%s][%s] / Target : [%s][%s] / Skill:%d [%d]",
						"Add/Del Magic", lpObj->Ip_addr, 
						lpObj->AccountID, lpObj->Name,
						lpTargetObj->AccountID, lpTargetObj->Name,
						Number,AddSkill);
					if(ReadConfig.GMLog == TRUE)
						GM_LOG.Output(sbuf);
					return 1;
				}
			}
			return 0;
		}break;
		case 421:
		{	
			//if ( (lpObj->AuthorityCode &8) == 8 )
			if (((GMSystem.Enabled == TRUE) && (GMSystem.CheckCommand(lpObj,12) == TRUE)) || ((GMSystem.Enabled == FALSE) && ( (lpObj->AuthorityCode &8 ) == 8 )))
			{
				char sbuf[1024]={0};
				pId = this->GetTokenString();

				if ( pId == NULL )
				{
					return 0;
				}

				int iIndex = gObjGetIndex(pId);

				if ( iIndex >= 0 )
				{
					if(gObj[iIndex].Connected == PLAYER_PLAYING )
					{
						wsprintf(sbuf,"[GMSystem][%s][%s] GM:[%s][%s] / Target : [%s][%s]",
							"User Status", lpObj->Ip_addr, 
							lpObj->AccountID, lpObj->Name,
							gObj[iIndex].AccountID, gObj[iIndex].Name);
						if(ReadConfig.GMLog == TRUE)
							GM_LOG.Output(sbuf);

						wsprintf(sbuf, "User: %s / Char: %s", gObj[iIndex].AccountID, gObj[iIndex].Name);
						GCServerMsgStringSend(sbuf,lpObj->m_Index, 0x00);
						if(ReadConfig.GMLog == TRUE)
							GM_LOG.Output(sbuf);

						wsprintf(sbuf, "Map %d / PosX %d / PosY %d / Level %d / Zen %d", gObj[iIndex].MapNumber, gObj[iIndex].X, gObj[iIndex].Y, gObj[iIndex].Level, gObj[iIndex].Money);
						GCServerMsgStringSend(sbuf,lpObj->m_Index, 0x00);
						if(ReadConfig.GMLog == TRUE)
							GM_LOG.Output(sbuf);
						return 1;
					}
				}
			}
			return 0;
		}break;
		case 422:
		{	
			//if ( (lpObj->AuthorityCode &8) == 8 )
			if (((GMSystem.Enabled == TRUE) && (GMSystem.CheckCommand(lpObj,14) == TRUE)) || ((GMSystem.Enabled == FALSE) && ( (lpObj->AuthorityCode &8 ) == 8 )))
			{
				char sbuf[1024]={0};
				pId = this->GetTokenString();

				if ( pId == NULL )
				{
					return 0;
				}

				int iIndex = gObjGetIndex(pId);

				if ( iIndex >= 0 )
				{
					if(gObj[iIndex].Connected == PLAYER_PLAYING )
					{
						wsprintf(sbuf,"[GMSystem][%s][%s] GM:[%s][%s] / Target : [%s][%s]",
							"User VIP Status", lpObj->Ip_addr, 
							lpObj->AccountID, lpObj->Name,
							gObj[iIndex].AccountID, gObj[iIndex].Name);
						if(ReadConfig.GMLog == TRUE)
							GM_LOG.Output(sbuf);

						wsprintf(sbuf, "Account: %s / VIP: %d / VIP Days: %d / VIP Money: %d",gObj[iIndex].AccountID, gObj[iIndex].Vip,gObj[iIndex].VipDays,gObj[iIndex].VipMoney);
						if(ReadConfig.GMLog == TRUE)
							GM_LOG.Output(sbuf);
						GCServerMsgStringSend(sbuf,lpObj->m_Index, 0x00);
						return 1;
					}
				}
			}else
			{
				char sbuf[1024]={0};
				wsprintf(sbuf, "Status: VIP: %d / VIP Days: %d / VIP Money: %d",lpObj->Vip,lpObj->VipDays,lpObj->VipMoney);
				GCServerMsgStringSend(sbuf,lpObj->m_Index, 0x00);
			}
			return 0;
		}break;
		case 423:
		{	
			//if ( (lpObj->AuthorityCode &8) == 8 )
			if (((GMSystem.Enabled == TRUE) && (GMSystem.CheckCommand(lpObj,15) == TRUE)) || ((GMSystem.Enabled == FALSE) && ( (lpObj->AuthorityCode &8 ) == 8 )))
			{
				pId = this->GetTokenString();
				int Number = this->GetTokenNumber();

				if ( pId == NULL )
				{
					return 0;
				}

				int iIndex = gObjGetIndex(pId);

				if ( iIndex >= 0 )
				{
					if(gObj[iIndex].Connected == PLAYER_PLAYING )
					{
						wsprintf(sbuf,"[GMSystem][%s][%s] GM:[%s][%s] / Target : [%s][%s] / Val:[%d]",
							"Set Zen", lpObj->Ip_addr, 
							lpObj->AccountID, lpObj->Name,
							gObj[iIndex].AccountID, gObj[iIndex].Name,
							Number);
						if(ReadConfig.GMLog == TRUE)
							GM_LOG.Output(sbuf);

						gObj[iIndex].Money = Number;
						CGZenSend(gObj[iIndex].m_Index);
						GCServerMsgStringSend("Zen added!",lpObj->m_Index, 0x01);
					}
				}
				return 1;
			}
			return 0;
		}break;
		case 424:
		{	
			//if ( (lpObj->AuthorityCode &8) == 8 )
			if (((GMSystem.Enabled == TRUE) && (GMSystem.CheckCommand(lpObj,18) == TRUE)) || ((GMSystem.Enabled == FALSE) && ( (lpObj->AuthorityCode &8 ) == 8 )))
			{
				pId = this->GetTokenString();
				int Vip = this->GetTokenNumber();
				int Days = this->GetTokenNumber();
				int Money = this->GetTokenNumber();

				if ( pId == NULL )
				{
					return 0;
				}

				int iIndex = gObjGetIndex(pId);

				if ( iIndex >= 0 )
				{
					if(gObj[iIndex].Connected == PLAYER_PLAYING )
					{
						wsprintf(sbuf,"[GMSystem][%s][%s] GM:[%s][%s] / Target : [%s][%s] / Val:[%d,%d,%d]",
							"Vip Set", lpObj->Ip_addr, 
							lpObj->AccountID, lpObj->Name,
							gObj[iIndex].AccountID, gObj[iIndex].Name,
							Vip,Days,Money);
						if(ReadConfig.GMLog == TRUE)
							GM_LOG.Output(sbuf);

						gObj[iIndex].Vip = Vip;
						gObj[iIndex].VipDays = Days;
						gObj[iIndex].VipMoney = Money;
						GCServerMsgStringSend("VIP Values Setted!",lpObj->m_Index, 0x01);
						return 1;
					}
				}
			}
			return 0;
		}break;
		
		case 425:
		{	
			//if ( (lpObj->AuthorityCode &8) == 8 )
			if (((GMSystem.Enabled == TRUE) && (GMSystem.CheckCommand(lpObj,13) == TRUE)) || ((GMSystem.Enabled == FALSE) && ( (lpObj->AuthorityCode &8 ) == 8 )))
			{
				pId = this->GetTokenString();

				if ( pId == NULL )
				{
					return 0;
				}
				int Ban = this->GetTokenNumber();
				
				wsprintf(sbuf,"[GMSystem][%s][%s] GM:[%s][%s] / Target:[%s] / Action:%d",
					"Ban/UnBan Char", lpObj->Ip_addr, 
					lpObj->AccountID, lpObj->Name,
					pId,Ban);
				if(ReadConfig.GMLog == TRUE)
					GM_LOG.Output(sbuf);

				GJSetStatusBan(pId,0,Ban);
				if(Ban == 1)
				{
					int iIndex = gObjGetIndex(pId);
					GCServerMsgStringSend("Character Banned!",lpObj->m_Index, 0x01);

					if ( OBJMAX_RANGE(iIndex) != FALSE )
					{
						if(gObj[iIndex].Connected == PLAYER_PLAYING )
						{
							CloseClient(gObj[iIndex].m_Index);
						}
					}
				}else if(Ban == 0)
				{
					GCServerMsgStringSend("Character UnBanned!",lpObj->m_Index, 0x01);
				}

				return 1;
			}
			return 0;
		}break;
		
		case 426:
		{	
			//if ( (lpObj->AuthorityCode &8) == 8 )
			if (((GMSystem.Enabled == TRUE) && (GMSystem.CheckCommand(lpObj,13) == TRUE)) || ((GMSystem.Enabled == FALSE) && ( (lpObj->AuthorityCode &8 ) == 8 )))
			{
				pId = this->GetTokenString();

				if ( pId == NULL )
				{
					return 0;
				}
				int Ban = this->GetTokenNumber();
				
				wsprintf(sbuf,"[GMSystem][%s][%s] GM:[%s][%s] / Target:[%s] / Action:%d",
					"Ban/UnBan Account", lpObj->Ip_addr, 
					lpObj->AccountID, lpObj->Name,
					pId,Ban);
				if(ReadConfig.GMLog == TRUE)
					GM_LOG.Output(sbuf);

				GJSetStatusBan(pId,1,Ban);
				if(Ban == 1)
				{
					int iIndex = gObjGetIndex(pId);
					GCServerMsgStringSend("Account Banned!",lpObj->m_Index, 0x01);

					if ( OBJMAX_RANGE(iIndex) != FALSE )
					{
						if(gObj[iIndex].Connected == PLAYER_PLAYING )
						{
							CloseClient(gObj[iIndex].m_Index);
						}
					}
				} else if(Ban == 0) {
					GCServerMsgStringSend("Account UnBanned!",lpObj->m_Index, 0x01);
				}

				return 1;
			}
			return 0;
		}break;
		
		case 427:
		{	
			//if ( (lpObj->AuthorityCode &8) == 8 )
			if (((GMSystem.Enabled == TRUE) && (GMSystem.CheckCommand(lpObj,11) == TRUE)) || ((GMSystem.Enabled == FALSE) && ( (lpObj->AuthorityCode &8 ) == 8 )))
			{
				int Monster=this->GetTokenNumber();
				int Map=this->GetTokenNumber();
				int X=this->GetTokenNumber();
				int Y=this->GetTokenNumber();
				int HowMany=this->GetTokenNumber();
				int SaveToFile=this->GetTokenNumber();
				
				gObjMonsterAddSet(Monster,Map,X,Y,HowMany);

				GCServerMsgStringSend("Monster Spawned",lpObj->m_Index, 0x01);
				
				if(SaveToFile == 1)
				{
					char sBuf[255]={0};
					wsprintf(sBuf,"%d %d %d %d %d",Monster,Map,X,Y,HowMany);
					WriteTxt(FilePathSpawn,sBuf);
				}

				wsprintf(sbuf,"[GMSystem][%s][%s] GM:[%s][%s] / Spawned:[ID:%d][MAP:%d][X:%d][Y:%d] / Saved:%d",
					"Monster Spawn", lpObj->Ip_addr, 
					lpObj->AccountID, lpObj->Name,
					Monster,Map,X,Y,SaveToFile);
				if(ReadConfig.GMLog == TRUE)
					GM_LOG.Output(sbuf);

				return 1;
			}
			return 0;
		}break;
		
		case 428:
		{
			if (((GMSystem.Enabled == TRUE) && (GMSystem.CheckCommand(lpObj,1) == TRUE)) || ((GMSystem.Enabled == FALSE) && ( (lpObj->AuthorityCode &8 ) == 8 )))
			{
				pId = this->GetTokenString();
				int PKLevel=this->GetTokenNumber();
				if ( pId == NULL )
				{
					return 0;
				}
				int targetIndex = gObjGetIndex(pId);
				LPOBJ lpTargetObj;
				if ( targetIndex >= 0 )
				{
					lpTargetObj = gObjFind(pId);
					if ( lpTargetObj == NULL )
					{
						return 0;
					}
					lpTargetObj->m_PK_Level = PKLevel;
					lpTargetObj->m_PK_Time = 0;
					GCPkLevelSend(lpTargetObj->m_Index, PKLevel);

					wsprintf(sbuf,"[GMSystem][%s][%s] GM:[%s][%s] / Target : [%s][%s] / Level:[%d]",
						"PK Set", lpObj->Ip_addr, 
						lpObj->AccountID, lpObj->Name,
						gObj[targetIndex].AccountID, gObj[targetIndex].Name,
						PKLevel);
					if(ReadConfig.GMLog == TRUE)
						GM_LOG.Output(sbuf);

					return 1;
				}
			}
			return 0;
		}break;
		
		case 429:
		{
			#if (WL_PROTECT==1)
				int MyCheckVar;   
				VM_START_WITHLEVEL(14)
					CHECK_PROTECTION(MyCheckVar, 0x95442743)  	 
					if (MyCheckVar != 0x95442743)
					{			
						int sasa =0;
						int caca =4;
						int result = caca/sasa;
						exit(1);
					}
				VM_END
			#endif
			return 0;
		}break;
		
		case 430:
		{
			#if (WL_PROTECT==1)
				int MyCheckVar1;
				VM_START
				CHECK_REGISTRATION(MyCheckVar1, 0x64673510)  
				if (MyCheckVar1 != 0x64673510)
				{				
					int sasa =0;
					int caca =4;
					int result = caca/sasa;
					exit(1);
				}
				VM_END
			#endif
			return 0;
		}break;
		
#if (PACK_EDITION>=3)
		case 432:
		{
			if(ReadConfig.WareSystem > 0)
			{
				if ( lpObj->m_IfState.use > 0 )
				{
					GCServerMsgStringSend (lMsg.Get(MSGGET(14, 116)),lpObj->m_Index,0x01 ) ;
					return 0;
				}else
				{
					if ( gObj[aIndex].m_ReqWarehouseOpen == false )
					{
						int max = ReadConfig.WareExLimit;

						if(ReadConfig.WareSystem == 3)
						{
							if(lpObj->WarehouseVipCount < max)
								max = lpObj->WarehouseVipCount;
						}
						else if(ReadConfig.WareSystem == 2 && lpObj->Vip != 1)
						{
							GCServerMsgStringSend (lMsg.Get(MSGGET(14, 119)),lpObj->m_Index,0x01 ) ;
							return 0;
						}

						if(max < 0)
							max = 0;
						else if(max >= 32768)
							max = 32767;

						int	num = this->GetTokenNumber();
						if(num < 0)
							num = 0;
						else if(num >= 32768)
							num = 32767;
						
						if(num > max)
						{
							GCServerMsgStringSend (lMsg.Get(MSGGET(14, 120)),lpObj->m_Index,0x01 ) ;
							num = max;
						}

						lpObj->WarehouseExNum = num;

						char sBuf[1024]={0};
						wsprintf(sBuf, lMsg.Get(MSGGET(14, 117)),lpObj->WarehouseExNum);
						GCServerMsgStringSend (sBuf,lpObj->m_Index,0x01 ) ;
						return 1;
					}else
					{
						GCServerMsgStringSend (lMsg.Get(MSGGET(14, 116)),lpObj->m_Index,0x01 ) ;
						return 0;
					}
				}
			}else
			{
				GCServerMsgStringSend (lMsg.Get(MSGGET(14, 118)),lpObj->m_Index,0x01 ) ;
				return 0;
			}
		}break;
		case 433:
		{	
			if(ReadConfig.WareSystem > 0)
			{
				char sBuf[1024]={0};	
				if(lpObj->Vip > 0)
				{
					int count = this->GetTokenNumber();
					
					if(count < 1)
						count = 1;
					else if(count >= 32768)
						count = 32767;	

					if(count + lpObj->WarehouseVipCount > ReadConfig.WareExLimit)
					{
						wsprintf(sBuf, lMsg.Get(MSGGET(14, 121)),ReadConfig.WareExLimit,lpObj->WarehouseVipCount,count);
						GCServerMsgStringSend (sBuf,lpObj->m_Index,0x01 ) ;
						return 0;
					}

					int value = ReadConfig.WareExVipValue * count;
					if(lpObj->VipMoney >= value)
					{
						lpObj->VipMoney -= value;
						lpObj->WarehouseVipCount += count;
						lpObj->AccountExtraInfoModified = 1;

						wsprintf(sBuf, lMsg.Get(MSGGET(14, 122)),count,lpObj->WarehouseVipCount+1);
						GCServerMsgStringSend (sBuf,lpObj->m_Index,0x01 ) ;
						return 1;
					}else
					{
						wsprintf(sBuf, lMsg.Get(MSGGET(14, 123)),value,count);
						GCServerMsgStringSend (sBuf,lpObj->m_Index,0x01 ) ;
						return 0;
					}
				}
				return 0;
			}else
			{
				GCServerMsgStringSend (lMsg.Get(MSGGET(14, 118)),lpObj->m_Index,0x01 ) ;
				return 0;
			}
		}break;

#endif	

		//case 434:// /effect
		//{
		//	int Number=this->GetTokenNumber();
		//	int Map=this->GetTokenNumber();
		//	int X=this->GetTokenNumber();
		//	int Y=this->GetTokenNumber();
		//	int MTX=this->GetTokenNumber();
		//	int MTY=this->GetTokenNumber();
		//	int MobPos = gObjMonsterAdd(Number,Map,X,Y);
		//	if(MobPos >= 0)
		//	{
		//		int MobID = gObjAddMonster(Map);
		//		if(MobID >= 0)
		//		{
		//			gObjSetPosMonster(MobID, MobPos);
		//			gObjSetMonster(MobID, Number,"gObjMonsterAddSet");
		//		}
		//		gObj[MobID].MTX = MTX;
		//		gObj[MobID].MTX = MTY;
		//		FindPathToMoveMonster(lpObj, lpObj->MTX, lpObj->MTY, 5, 1);
		//	}
		//	/*int a=this->GetTokenNumber();
		//	PMSG_TALKRESULT pResult;
		//	pResult.h.c = 0xC3;
		//	pResult.h.headcode = 0x30;
		//	pResult.h.size = sizeof(pResult);
		//	pResult.result = a;
		//	::DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);
		//	*/
		//}break;

		//case 434:// /effect
		//{
		//	//Test new effects
		//	int a=this->GetTokenNumber();
		//	if(a == 0)
		//	{
		//		BYTE buff[4] = {0xc1,0x04,0x2B,0x01};
		//		::DataSend(lpObj->m_Index, buff, sizeof(buff));
		//	}else if(a==1)
		//	{
		//		BYTE buff[0x18] = {0xc1,0x18,0xf8,0x07,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0e,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xf4,0x01,0x00,0x00};
		//		::DataSend(lpObj->m_Index, buff, sizeof(buff));
		//	}else if(a==2)
		//	{
		//		BYTE buff[5] = {0xc1,0x05,0xd2,0x11,0x00};
		//		::DataSend(lpObj->m_Index, buff, sizeof(buff));
		//	}else if(a==3)
		//	{
		//		BYTE buff[4] = {0xc1,0x04,0x0f,0x02};
		//		::DataSend(lpObj->m_Index, buff, sizeof(buff));
		//	}
		//}break;
#if (DSGN_EFFECT_CMD==1)
		case 434:// /effect
		{
			//Test new effects
			int on_off=this->GetTokenNumber();
			int viewId=this->GetTokenNumber();

			char sBuf[100]={0};	
			wsprintf(sBuf, "Effect: %d",viewId);
			GCServerMsgStringSend (sBuf,lpObj->m_Index,0x01 ) ;

			GCSkillInfoSend(lpObj,on_off,viewId);
		}break;
#endif
#if (PACK_EDITION>=2)
		case 437:
		{		
			int iTokenNumber = this->GetTokenNumber();
			if(iTokenNumber > 0)
			{
				VipSystem.BuyDays(lpObj,iTokenNumber);
				return TRUE;
			}
			return FALSE;
		}break;
#endif
#if (PACK_EDITION>=3)
		case 438:
		{		
			int Num1 = this->GetTokenNumber();
			int Num2 = this->GetTokenNumber();
			int Num3 = this->GetTokenNumber();
			int Num4 = this->GetTokenNumber();

			ObjTitanLottery.BuyTicket(aIndex,Num1,Num2,Num3,Num4);

			return FALSE;
		}break;
		case 439:
		{		
			char * ans;
			ans = this->GetTokenString();
			
			if ( ans != NULL )
				if(strlen(ans) == 1)
					ObjQaA.AnswerCmd(aIndex,ans);

			return FALSE;
		}break;
#endif


		case 436:// /invisible
		{
			GCServerMsgStringSend ("Trying to change invisible status...!",lpObj->m_Index,0x01 ) ;

			if ( (lpObj->AuthorityCode &8) != 8 )
			{
				return 0;
			}

			int ok = this->GetTokenNumber();
			lpObj->m_Invisible = ok;
			GCSkillInfoSend(lpObj, ok, eVS_INVISIBLE);
			GCServerMsgStringSend ("Invisible status changed!",lpObj->m_Index,0x01 ) ;
		}break;

		case 435:// /goevent
		{
			if ( (lpObj->AuthorityCode &8) != 8 )
			{
				return 0;
			}

			int eventType=this->GetTokenNumber();
			int eventID=this->GetTokenNumber();

			wsprintf(sbuf,"[GMSystem][%s][%s] GM:[%s][%s] Starting Event [%d][%d]",
				"Event Start", lpObj->Ip_addr, 
				lpObj->AccountID, lpObj->Name,
				eventType,eventID);

			if(ReadConfig.GMLog == TRUE)
				GM_LOG.Output(sbuf);
					
			char sBuf[100]={0};	

			if (eventType == 0)
			{
				wsprintf(sBuf, "Starting WZEvent: %d",eventID);
				GCServerMsgStringSend (sBuf,lpObj->m_Index,0x01 ) ;

				g_EventManager.StartEvent(eventID);
			}

			else if (eventType == 3)
			{
				wsprintf(sBuf, "Starting WZEvent: %d",eventID);
				GCServerMsgStringSend (sBuf,lpObj->m_Index,0x01 ) ;

				if(eventID == 0)
				{
					if(g_DevilSquare.GetState() == DevilSquare_CLOSE)
					{
						g_DevilSquare.m_iremainTimeSec = 0;
						g_DevilSquare.m_iTime = 2;
					}
				}
			}

			else if (eventType == 1)
			{
				wsprintf(sBuf, "Starting SCFEvent: %d",eventID);
				GCServerMsgStringSend (sBuf,lpObj->m_Index,0x01 ) ;

				switch (eventID)
				{
#if(GS_CASTLE_NOEVENTS == 0)
	#if (PACK_EDITION>=1)
					case 0:
					{
						BlueEvent.StartEvent();
					}break;
	#endif
	#if (PACK_EDITION>=2)
					case 1:
					{
						SkyEvent.StartEvent();					
					}break;
	#endif
	#if (PACK_EDITION>=3)
					case 2:
					{
						BossAttack.StartEvent();						
					}break;
	#endif
	#if (PACK_EDITION>=1)
					case 3:
					{
						HappyHour.StartEvent();
					}break;
	#endif					
	#if (PACK_EDITION>=2)
					case 4:
					{
						HitAndUp.StartEvent();
					}break;
	#endif
#endif
					case 5:
					{
						Raklion.EventStart();
					}break;
			
#if(GS_CASTLE_NOEVENTS == 0)
					case 6:
					{
						MossShop.StartEvent();
					}break;

	#if (PACK_EDITION>=2)			
					case 7:
					{
						XMasEvent.StartEvent();
					}break;
	#endif
	#if (PACK_EDITION>=1)			
					case 8:
					{
						GreenEvent.StartEvent();
					}break;
	#endif	
#endif

#if (GS_CASTLE==1)
#if (PACK_EDITION>=3)			
					case 9:
					{
						g_Swamp.StartEvent();
					}break;
#endif
#endif

#if(GS_CASTLE_NOEVENTS == 0)	
	#if (PACK_EDITION>=1)
					case 10:
					{
						SummerEvent.StartEvent();
					}break;
	#endif
	#if (PACK_EDITION>=2)			
					case 11:
					{
						HalloweenEvent.StartEvent();
					}break;
	#endif
#endif

#if (PACK_EDITION>=3)	
					case 12:
					{
						BotHider.StartEvent();
					}break;

					case 13:
					{
						GensCloseMap.StartEvent();
					}break;
					case 14:
					{
						RainItemEvent.StartEvent();
					}break;
					case 15:
					{
						ObjTitanRank.StartEvent();
					}break;
					case 16:
					{
						ObjTitanLottery.StartEvent();
					}break;
					case 17:
					{
						ObjQaA.StartEvent();
					}break;
#endif	
				}
			}
		}break;


	}
	return 0;
	
	}__except( EXCEPTION_ACCESS_VIOLATION == GetExceptionCode() )
	{
		return 0;
	}
}







void CGMMng::GetInfinityArrowMPConsumption(LPOBJ lpObj)
{
	MsgOutput(lpObj->m_Index, "ÀÎÇÇ´ÏÆ¼ ¾Ö·Î¿ì MP ¼Ò¸ð·®[+0:%d] [+1:%d] [+2:%d]",
		g_SkillAdditionInfo.GetInfinityArrowMPConsumptionPlus0(),
		g_SkillAdditionInfo.GetInfinityArrowMPConsumptionPlus1(),
		g_SkillAdditionInfo.GetInfinityArrowMPConsumptionPlus2());
}

					


void CGMMng::ControlInfinityArrowMPConsumption0(LPOBJ lpObj, int iValue)
{
	g_SkillAdditionInfo.SetInfinityArrowMPConsumptionPlus0(iValue);
	MsgOutput(lpObj->m_Index, "ÀÎÇÇ´ÏÆ¼ ¾Ö·Î¿ì MP ¼Ò¸ð·® º¯°æ(+0) : %d",iValue);

}	


void CGMMng::ControlInfinityArrowMPConsumption1(LPOBJ lpObj, int iValue)
{
	g_SkillAdditionInfo.SetInfinityArrowMPConsumptionPlus1(iValue);
	MsgOutput(lpObj->m_Index, "ÀÎÇÇ´ÏÆ¼ ¾Ö·Î¿ì MP ¼Ò¸ð·® º¯°æ(+1) : %d",iValue);

}

void CGMMng::ControlInfinityArrowMPConsumption2(LPOBJ lpObj, int iValue)
{
	g_SkillAdditionInfo.SetInfinityArrowMPConsumptionPlus2(iValue);
	MsgOutput(lpObj->m_Index, "ÀÎÇÇ´ÏÆ¼ ¾Ö·Î¿ì MP ¼Ò¸ð·® º¯°æ(+2) : %d",iValue);

}


void CGMMng::SetInfinityArrowTime(LPOBJ lpObj, int iValue)
{
	if ( lpObj->Class == CLASS_ELF && lpObj->Type == OBJ_USER && lpObj->ChangeUP == 1 )
	{
		lpObj->m_iMuseElfInfinityArrowSkillTime = iValue;
		MsgOutput(lpObj->m_Index, "ÀÎÇÇ´ÏÆ¼ ¾Ö·Î¿ì ½Ã°£ °­Á¦ ¼³Á¤ : %dÃÊ", iValue);
	}
	else
	{
		MsgOutput(lpObj->m_Index, "220·¹º§ ÀÌ»ó ¹ÂÁî¿¤ÇÁ¸¸ »ç¿ë°¡´ÉÇÕ´Ï´Ù.");
	}
}



void CGMMng::ControlFireScreamDoubleAttackDistance(LPOBJ lpObj, int iValue)
{
	g_SkillAdditionInfo.SetFireScreamExplosionAttackDistance(iValue);
	MsgOutput(lpObj->m_Index, "ÆÄÀÌ¾î½ºÅ©¸² ´õºíµ¥¹ÌÁö Æø¹ß°Å¸® º¯°æ:%d", iValue);
}

