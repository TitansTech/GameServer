#include "stdafx.h"
/*#include "DevilSquare.h"
#include "GameServer.h"
#include "LogProc.h"
#include "GameMain.h"
#include "TNotice.h"
#include "..\include\readscript.h"
#include "gObjMonster.h"
//#include "GameServerAuth.h"
#include "..\common\winutil.h"
#include "DSProtocol.h"
#include "ObjUseSkill.h"

#define IT_TEAM_BLUE	1
#define IT_TEAM_ORANGE	2
#define IT_MAX_LEVEL	6

static struct _IT_LEVEL
{
	int iLOWER_BOUND;	// 0
	int iUPPER_BOUND;	// 4

} g_sttIT_LEVEL[IT_MAX_LEVEL] =
{
	// Illusion Temple 1
	220, 270, 

	// Illusion Temple 2
	271, 320,

	// Illusion Temple 3
	321, 350,

	// Illusion Temple 4
	351, 380,

	// Illusion Temple 5
	381, MAX_CHAR_LEVEL,

	// Illusion Temple 6
	1, MAX_CHAR_LEVEL
};

struct PMSG_STATE_IT
{
	PBMSG_HEAD h;	// C1:BF
	BYTE subcode;	// 3
	BYTE State;	// 4
	BYTE Type;	// 5
};

struct PMSG_KILLPOINT_IT
{
	PBMSG_HEAD h;	// C1:BF
	BYTE subcode;	// 3
	BYTE Points;	// 4
};


BOOL IT_MAP_RANGE_CHECK(BYTE Map)
{	
	if ((Map < 45) || (Map > 50))
	{
		return FALSE;
	}
	return TRUE;
}

struct IT_LEVEL
{
	bool Start;

	int Hero;
	int Score_Blue;
	int Score_Orange;

	int KillPts_Blue;
	int KillPts_Orange;
	
	int Exp_Blue;
	int Exp_Orange;
};

class cIllusionTemple
{
public:
	void DataSendTeamLevel(BYTE Level, BYTE Team, LPBYTE pMsg, int size);
	void DataSendLevel(BYTE Level, LPBYTE pMsg, int size);
	void GCServerMsgStringSendOut(LPSTR  szMsg, BYTE type);
	void GCServerMsgStringSendLevel(BYTE Level, LPSTR szMsg, BYTE type);
	void RemoveUser(int aIndex);
	void JoinAsk(LPOBJ lpObj);
	void UserCount(BYTE Level, int & Blue, int & Orange);
	void Join(LPOBJ lpObj, BYTE Level);
	void JoinRoom();
	void Start();
	void GetGift(LPOBJ lpObj,int Map);
	void ReturnGift(LPOBJ lpObj,int Map, BYTE X, BYTE Y);
	void Timer(BYTE Level,int ctime);
	BOOL KillPoint(BYTE Level,BYTE Team, BOOL Add , int Points);

	//
	BOOL Enabled;
	bool EventStart;
	IT_LEVEL Level[IT_MAX_LEVEL];
	int TimerTick;
	BYTE MaxPlayers;
};

cIllusionTemple IT;

void cIllusionTemple::RemoveUser(int aIndex)
{
	gObj[aIndex].m_cIllusionTempleIndex = 255;
	gObj[aIndex].m_cIllusionTempleTeam = 0;
	gObj[aIndex].m_iIllusionTempleEXP = 0;
}

void cIllusionTemple::GCServerMsgStringSendOut(LPSTR  szMsg, BYTE type)
{
	PMSG_NOTICE pNotice;	
	TNotice::MakeNoticeMsg((TNotice*)&pNotice, type, szMsg);

	for ( int n = OBJ_STARTUSERINDEX ; n < OBJMAX ; n++)
	{
		if ( gObj[n].Connected == PLAYER_PLAYING )
		{
			if (( gObj[n].Type  == OBJ_USER ) && (IT_MAP_RANGE_CHECK(gObj[n].MapNumber) == FALSE ))
			{
				DataSend(n, (UCHAR*)&pNotice, pNotice.h.size);
			}
		}
	}
}

void cIllusionTemple::GCServerMsgStringSendLevel(BYTE Level, LPSTR szMsg, BYTE type)
{
	int Map = 45 + Level;
	PMSG_NOTICE pNotice;	
	TNotice::MakeNoticeMsg((TNotice*)&pNotice, type, szMsg);

	for ( int n = OBJ_STARTUSERINDEX ; n < OBJMAX ; n++)
	{
		if ( gObj[n].Connected == PLAYER_PLAYING )
		{
			if ( gObj[n].Type  == OBJ_USER )
			{
				if (gObj[n].MapNumber == Map)
				{
					if(gObj[n].m_cIllusionTempleIndex == Level)
					{
						DataSend(n, (UCHAR*)&pNotice, pNotice.h.size);
					}else
						this->RemoveUser(n);
				}
			}
		}
	}
}

void cIllusionTemple::DataSendLevel(BYTE Level, LPBYTE pMsg, int size)
{
	int Map = 45 + Level;
	for ( int n = OBJ_STARTUSERINDEX ; n < OBJMAX ; n++)
	{
		if ( gObj[n].Connected == PLAYER_PLAYING )
		{
			if ( gObj[n].Type  == OBJ_USER )
			{
				if(gObj[n].m_cIllusionTempleIndex == Level)
				{
					if(gObj[n].MapNumber == Map)
					{
						DataSend(n, pMsg, size);
					}else
						this->RemoveUser(n);
				}
			}
		}
	}
}

void cIllusionTemple::DataSendTeamLevel(BYTE Level, BYTE Team, LPBYTE pMsg, int size)
{
	int Map = 45 + Level;
	for ( int n = OBJ_STARTUSERINDEX ; n < OBJMAX ; n++)
	{
		if ( gObj[n].Connected == PLAYER_PLAYING )
		{
			if ( gObj[n].Type  == OBJ_USER )
			{
				if(gObj[n].m_cIllusionTempleIndex == Level)
				{
					if(gObj[n].MapNumber == Map)
					{
						if(gObj[n].m_cIllusionTempleTeam == Team)
							DataSend(n, pMsg, size);
					}else
						this->RemoveUser(n);
				}
			}
		}
	}
}

void cIllusionTemple::UserCount(BYTE Level, int & Blue, int & Orange)
{
	for ( int n = OBJ_STARTUSERINDEX ; n < OBJMAX ; n++)
	{
		if ( gObj[n].Connected == PLAYER_PLAYING )
		{
			if ( gObj[n].Type  == OBJ_USER )
			{
				if(gObj[n].m_cIllusionTempleIndex == Level)
				{
					if(IT_MAP_RANGE_CHECK(gObj[n].MapNumber) == TRUE)
					{
						if(gObj[n].m_cIllusionTempleTeam == IT_TEAM_BLUE)
							Blue++;
						else if(gObj[n].m_cIllusionTempleTeam == IT_TEAM_ORANGE)
							Orange++;
						else
							this->RemoveUser(n);
					}else
						this->RemoveUser(n);
				}
			}
		}
	}
}

void cIllusionTemple::JoinAsk(LPOBJ lpObj)
{
	if (gObjIsNewClass(lpObj) == 0)
	{
		for(int i=0;i<IT_MAX_LEVEL-1;i++)
		{
			if ((lpObj->Level >= g_sttIT_LEVEL[i].iLOWER_BOUND) && (lpObj->Level <= g_sttIT_LEVEL[i].iUPPER_BOUND))
			{
				int Blue=0;
				int Orange=0;
				this->UserCount(i,Blue,Orange);
				int PlayerCount = Blue + Orange;

				if( PlayerCount <= this->MaxPlayers )
				{
					if(DeleteItemByLevel(lpObj->m_Index,0x1A33,i+1) == 1)
					{
						this->Join(lpObj,i);
						return;
					} else 
					{
						goto NoTicket;
					}
				}else
				{
					goto Full;
				}
			}
		}
		
	}else
	{ 
		int Blue=0;
		int Orange=0;
		this->UserCount(5,Blue,Orange);
		int PlayerCount = Blue + Orange;
		if( PlayerCount <= this->MaxPlayers )
		{
			if(DeleteItemByLevel(lpObj->m_Index,0x1A33,6) == 1)
			{
				this->Join(lpObj,5);
				return;
			} else 
			{
				goto NoTicket;
			}
		}else
		{
			goto Full;
		}
	}
	
Full:
	GCServerMsgStringSend(lMsg.Get(MSGGET(14, 15)),lpObj->m_Index,1);
	return;
	
NoTicket:
	GCServerMsgStringSend(lMsg.Get(MSGGET(14, 16)),lpObj->m_Index,1);
	return;
}


void cIllusionTemple::Join(LPOBJ lpObj, BYTE Level)
{
	int Blue=0;
	int Orange=0;
	int Map = 45 + Level;

	this->UserCount(Level,Blue,Orange);

	LogAddTD("[Illusion Temple] (%d) [%s] Wait Room Warp Requested ",Level+1,lpObj->Name);

	gObjTeleport(lpObj->m_Index, Map, 104, 134);
		
	lpObj->m_cIllusionTempleIndex = Level;
	lpObj->m_iIllusionTempleEXP = 0;

	if (Blue <= Orange)
	{
		LogAddTD("[Illusion Temple] (%d) [%s] Join In Team: Blue", Level+1, lpObj->Name);
		lpObj->m_cIllusionTempleTeam = IT_TEAM_BLUE;
		lpObj->m_Change = 404;
		gObjViewportListProtocolCreate(lpObj);
	}else
	{
		LogAddTD("[Illusion Temple] (%d) [%s] Join In Team: Orange",Level+1, lpObj->Name);
		lpObj->m_cIllusionTempleTeam = IT_TEAM_ORANGE;
		lpObj->m_Change = 405;
		gObjViewportListProtocolCreate(lpObj);
	}
}

void cIllusionTemple::JoinRoom()
{
	int Map=0;
	int BlueX=196;
	int BlueY=126;
	int OrangeX=143;
	int OrangeY=49;
	
	this->GCServerMsgStringSendOut("[Illusion Temple] is closed. Try again next time.",0);

	for(int Level=0;Level<IT_MAX_LEVEL;Level++)
	{
		Map=45+Level;
		LogAddTD("[Illusion Temple] (%d) Warping players into the event room",Level+1);

		for ( int n = OBJ_STARTUSERINDEX ; n < OBJMAX ; n++)
		{
			if ( gObj[n].Connected == PLAYER_PLAYING )
			{
				if ( gObj[n].Type  == OBJ_USER )
				{
					if(IT_MAP_RANGE_CHECK(gObj[n].MapNumber) == TRUE)
					{
						if(gObj[n].m_cIllusionTempleTeam == IT_TEAM_BLUE)
						{
							gObjTeleport(n,Map,BlueX,BlueY);
						}else if(gObj[n].m_cIllusionTempleTeam == IT_TEAM_ORANGE)
						{
							gObjTeleport(n,Map,OrangeX,OrangeY);
						}else
						{
							this->RemoveUser(n);
						}
					}
				}
			}
		}			
	}
}

void cIllusionTemple::Start()
{
	PMSG_STATE_IT pMsg;
	PHeadSubSetB((LPBYTE)&pMsg, 0xBF, 0x09, sizeof(pMsg));
	pMsg.State = 1;
	this->EventStart = false;

	for(int Level=0;Level<IT_MAX_LEVEL;Level++)
	{
		int Blue=0;
		int Orange=0;
		this->UserCount(Level,Blue,Orange);
		int PlayerCount = Blue + Orange;
		
		if (PlayerCount > 0)
		{
			this->EventStart = true;
			this->Level[Level].Start = true;
			this->Level[Level].Score_Blue = 0;
			this->Level[Level].Score_Orange = 0;
			this->Level[Level].KillPts_Blue = 0;
			this->Level[Level].KillPts_Orange = 0;

			this->GCServerMsgStringSendLevel(Level,lMsg.Get(MSGGET(14, 12)),0);
			LogAddTD("[Illusion Temple] (%d) Illusion Temple Event Start", Level + 1);
			pMsg.Type  = 1;
			this->DataSendLevel(Level,(BYTE*)&pMsg,sizeof(pMsg));
			pMsg.Type  = 2;
			this->DataSendLevel(Level,(BYTE*)&pMsg,sizeof(pMsg));
		}else 
		{
			this->Level[Level].Start = false;
			LogAddTD("[Illusion Temple] (%d) Illusion Temple Event Failed -> No Users Inside", Level + 1);
		}
	}
	//_beginthread( CIllusionTemple__TimeInside, 0, NULL  );
}

void cIllusionTemple::GetGift(LPOBJ lpObj,int Map)
{
	int Level = -1;
	
	if ((Map >= 45) && (Map < 45 + IT_MAX_LEVEL))
	{
		Level = Map - 45;
	}

	if (Level >= 0)
	{
		if (this->Level[Level].Start == 1)
		{
			char sBuf[255]={0};

			if(this->Level[Level].Hero == 0)
			{
				ItemSerialCreateSend(lpObj->m_Index, 235, 0, 0, 0x1C40, 0, 255, 0, 0, 0, -1, 0, 0);
				LogAddTD("[Illusion Temple] (%d) [%s] Take the gift",Level+1,lpObj->Name);
				wsprintf(sBuf,lMsg.Get(MSGGET(14, 14)),lpObj->Name);
				this->GCServerMsgStringSendLevel(Level,sBuf,0);
				this->Level[Level].Hero = lpObj->m_Index;
			}else
			{
				if (lpObj->m_Index != this->Level[Level].Hero)
				{
					LogAddTD("[Illusion Temple] (%d) Someone took the gift before",Level+1);
					this->GCServerMsgStringSendLevel(Level,lMsg.Get(MSGGET(14, 13)),0);
				}
			}
		}else
		{
			LogAddTD("[Illusion Temple][Bug Tracer] (%d) Event is not started", Level+1);
		}
	}else
	{
		LogAddTD("[Illusion Temple][Bug Tracer] (Map:%d) It's not Illusion Temple map", Map);
	}
}
//
//void cIllusionTemple::ReturnGift(LPOBJ lpObj,int PedestalNum)
//{
//	int Level = Map - 45;
//	char sBuf[1024]={0};
//
//	if (this->Level[Level].Start == 1)
//	{
//		BOOL Flag = 0;
//		
//		if(this->Level[Level].Hero == lpObj->m_Index )
//		{
//			if(lpObj->m_cIllusionTempleTeam == IT_TEAM_BLUE)
//			{
//				if(PedestalNum == 384)
//				{
//					this->Level[Level].Score_Blue++;
//					LogAddTD("[Illusion Temple] (%d) [Blue Team][Hero:%s] Gift returned to the statue!",Level+1,lpObj->Name);
//					wsprintf(sBuf,lMsg.Get(MSGGET(14, 20)));
//					Flag=1;
//				}else
//				{
//					LogAddTD("[Illusion Temple] (%d) [Blue Team][Hero:%s] Is not the blue storage!",Level+1,lpObj->Name);
//					GCServerMsgStringSend(lMsg.Get(MSGGET(14, 21)),lpObj->m_Index , 0x01);
//				}
//			}
//			else if(lpObj->m_cIllusionTempleTeam == IT_TEAM_ORANGE)
//			{
//				if(PedestalNum == 383)
//				{
//					this->Level[Level].Score_Orange++;
//					LogAddTD("[Illusion Temple] (%d) [Orange Team][Hero:%s] Gift returned to the statue!",Level+1,lpObj->Name);
//					wsprintf(sBuf,lMsg.Get(MSGGET(14, 22)));
//					Flag=1;
//				}else
//				{
//					LogAddTD("[Illusion Temple] (%d) [Orange Team][Hero:%s] Is not the orange storage!",Level+1,lpObj->Name);
//					GCServerMsgStringSend(lMsg.Get(MSGGET(14, 23)),lpObj->m_Index , 0x01);
//				}
//			}
//			if(Flag == 1)
//			{
//				this->GCServerMsgStringSendLevel(Level,sBuf,1);
//				this->Level[Level].Hero = 0;
//				DeleteItemByID(lpObj->m_Index,0x1C40);
//			}
//		}else
//		{
//			LogAddTD("[Illusion Temple] (%d) [%s] Is not a Hero",Level+1,lpObj->Name);
//			GCServerMsgStringSend(lMsg.Get(MSGGET(14, 24)),lpObj->m_Index , 0x01);
//		}
//	}
//}

void cIllusionTemple::Timer(BYTE Level,int ctime)
{
	//====================================================================================================
	// DaRKav [Feb 28, 2008] Illusion Timer Packet Structure
	//====================================================================================================
	// C1 XX BF 01 TIME1 TIME2 00 00 XX XX X1 X1 X2 X3 X4 X4 X5
	//              4     5    6  7  8  9  A  B  C  D  E  F  10 
	//
	// Time1 = 5A = 90 seconds that means, 1 minute and 30 seconds
	// Time2 = Extra time
	// 6 and 7 = Other extra time
	// 8 9 = The 08 byte work for move the fire ball in the angle down and parallel like a slash /
	// A = Red team score
	// B = Blue team score
	// C = if 01 then i dont know XD
	// D = Number of blue lights on minimap
	// E = Cord of blue light
	// F = Cord of blue light
	// 10 = if 01 another blue light (this one needed for show it)
	// 11 = Cord of blue light
	// 12 = Cord of blue light
	// 
	// (etc repeat process of how many blue lights are, the total ammount must be on D byte)
	//====================================================================================================
	// Note: Blue Team, Orange Team => Different packets, contain only info on their teammates
	//====================================================================================================
	
	//Calculating Time
	int Map=0;
	int TIME2=0;
	int TIME1=0;
	TIME2=(int)floor((float)ctime/255);
	TIME1=ctime-(TIME2*255);

	int HeroLo=0;
	int HeroHi=0;

	Map= 45 + Level;

	BYTE X = 0;
	BYTE Y = 0;

	if(this->Level[Level].Hero !=0)
	{
		if( gObj[this->Level[Level].Hero].Connected == PLAYER_PLAYING )
		{
			if(gObj[this->Level[Level].Hero].MapNumber != Map)
			{
				this->RemoveUser(this->Level[Level].Hero);
				this->Level[Level].Hero = 0;
				X = 169;
				Y = 85;
			}else
			{
				HeroLo = LOBYTE(this->Level[Level].Hero);
				HeroHi = HIBYTE(this->Level[Level].Hero);
				X = gObj[this->Level[Level].Hero].X;
				Y = gObj[this->Level[Level].Hero].Y;
			}
		}else
		{
			this->RemoveUser(this->Level[Level].Hero);
			this->Level[Level].Hero = 0;
			X = 169;
			Y = 85;
		}
	}

	//Building Packets for players
	//...
	BYTE TimerPacket[0xFF]={0xC1,0x11,0xBF,0x01,TIME1,TIME2,HeroHi,HeroLo,X,Y,this->Level[Level].Score_Orange,this->Level[Level].Score_Blue,0x00,0x00,0x00,0x00,0x00}; 
	this->DataSendLevel(Level,(PBYTE)TimerPacket,TimerPacket[1]);
}

BOOL cIllusionTemple::KillPoint(BYTE Level,BYTE Team, BOOL Add , int Points)
{	
	PMSG_KILLPOINT_IT pMsg;
	PHeadSubSetB((LPBYTE)&pMsg, 0xBF, 0x06, sizeof(pMsg));

	if(Add == 1)
	{
		if(Team == IT_TEAM_BLUE) //If dead some guy of Blue Team then add KillPoints to Orange Team
		{
			this->Level[Level].KillPts_Orange++;
			pMsg.Points = this->Level[Level].KillPts_Orange;
			this->DataSendTeamLevel(Level,IT_TEAM_ORANGE,(BYTE*)&pMsg,sizeof(pMsg));
			return 1;

		}else if(Team==IT_TEAM_ORANGE) //If dead some guy of Orange Team then add KillPoints to Blue Team
		{
			this->Level[Level].KillPts_Blue++;
			pMsg.Points = this->Level[Level].KillPts_Blue;
			this->DataSendTeamLevel(Level,IT_TEAM_BLUE,(BYTE*)&pMsg,sizeof(pMsg));
			return 1;
		}
		return 0;
	}else
	{
		if(Team==IT_TEAM_ORANGE)
		{
			if(this->Level[Level].KillPts_Orange-Points>=0)
			{
				this->Level[Level].KillPts_Orange-=Points;
				pMsg.Points = this->Level[Level].KillPts_Orange;
				this->DataSendTeamLevel(Level,IT_TEAM_ORANGE,(BYTE*)&pMsg,sizeof(pMsg));
				return 1;
			}

		}else if(Team==IT_TEAM_BLUE)
		{			
			if(this->Level[Level].KillPts_Blue-Points>=0)
			{
				this->Level[Level].KillPts_Blue-=Points;
				pMsg.Points = this->Level[Level].KillPts_Blue;
				this->DataSendTeamLevel(Level,IT_TEAM_BLUE,(BYTE*)&pMsg,sizeof(pMsg));
				return 1;
			}
		}
		return 0;
	}
}
*/
//void cIllusionTemple__TimeInside(void * lpParam)
//{
//	for (int Level=0;Level<IT_MAX_LEVEL;Level++)
//	{
//		if (IT.Level[Level].Start == 1)
//		{	
//			LogAddTD("[Illusion Temple] (%d) SetState PLAYING");
//		}else
//		{
//			LogAddTD("[Illusion Temple] (%d) SetState CLOSED");
//		}
//	}
//	while(true)
//	{
//		Sleep(1000);
//		bool AllFinish = true;
//
//		if(IT.Enabled == 1) 
//		{
//			AllFinish = false;
//			for (int Level=0;Level<IT_MAX_LEVEL;Level++)
//			{
//				if (IT.Level[Level].Start == 1)
//				{	
//					if(IT.Timer == -50) 
//					{
//						IllusionTemple.SetState_End(Level);
//					} 
//					else if(IT.TimerTick >=0)
//					{
//						if(IT.TimerTick == 0)
//						{	
//							IllusionTemple.GiveReward_Win(Level);
//						}
//						IT.Timer(Level,IT.TimerTick);
//
//						//Player Check Status
//						IllusionTemple.Player_Check(Level);
//					}
//					IT.TimerTick--; 
//				}
//			}
//
//			//Kill Illusion timer if they are all finished
//			if (AllFinish == true)
//			{
//				LogAddTD("[Illusion Temple] SetState CLOSED");
//				_endthread();
//				return;
//			}
//		}
//	}
//	_endthread();
//}

//void cIllusionTemple::SetState_End(BYTE Level)
//{
//	SetState_Closed(Level,1);
//	LogAddTD("[Illusion Temple] (%d) SetState END -> Moving Players",Level+1);
//	gObjMassMove(45+Level,0,125,125);
//	EventStart = FALSE;
//}