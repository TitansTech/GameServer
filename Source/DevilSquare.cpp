// ------------------------------
// Decompiled by Deathway
// Date : 2007-05-09
// ------------------------------
// GS-N 0.99.60T 0x00453780
// GS-N	1.00.18	JPN	0x00462B60	-	Completed
#include "stdafx.h"
#include "DevilSquare.h"
#include "GameServer.h"
#include "LogProc.h"
#include "GameMain.h"
#include "TNotice.h"
#include "..\include\readscript.h"
#include "gObjMonster.h"
#include "BloodCastle.h"
#include "ChaosCastle.h"
#include "..\common\winutil.h"
#include "CrywolfSync.h"
#include "Crywolf.h"

#include "XMasEvent.h"
#include "HappyHour.h"
#include "HitAndUp.h"
#include "ResetSystem.h"
#include "ObjBotPet.h"
#include "ViewportGuild.h"
#include "VIPSystem.h"
#include "RProtocol.h"
#include "QuestS5Info.h"

CDevilSquare g_DevilSquare;



unsigned int __stdcall DevilSquareThreadFunc(void * p)
{
	CDevilSquare * pDevilSquare = (CDevilSquare *)p;
	return pDevilSquare->Run();
}

#pragma warning ( disable : 4060 )
void DevilSquareProtocolCore(BYTE protoNum, LPBYTE aRecv, int aLen)
{
	#ifdef TRACE_PROTOCOL
		LogAddHeadHex("R","DEVIL_SQUARE", aRecv, aLen);
	#endif
	switch ( protoNum )
	{
	}
}
#pragma warning ( default : 4060 )




void DataSendRank(char* pMsg, int size)
{	
	if(ReadConfig.SCFRSON == FALSE)
	{
		if ( IsDevilSquareEventConnected == FALSE && DevilSquareEventConnect == FALSE )
		{
			wsRServerCli.Close();
			wsRServerCli.CreateSocket(ghWnd);

			if ( GMRankingServerConnect(gDevilSquareEventServerIp, WM_GM_RANKING_CLIENT_MSG_PROC) == FALSE )
			{
				IsDevilSquareEventConnected = FALSE;
				LogAddTD("Can not connect Ranking Server");

				return;
			}

			IsDevilSquareEventConnected = TRUE;
		}

		if ( DevilSquareEventConnect == FALSE && IsDevilSquareEventConnected != FALSE )
		{
			wsRServerCli.DataSend(pMsg, size);
		}
	}
}


CDevilSquare::CDevilSquare()
{
	this->m_bQuit = FALSE;
	this->m_iCloseTime = 2;
	this->m_iOpenTime = 2;
	this->m_iPlaytime = 10;

	this->m_BonusScoreTable[CLASS_WIZARD][0] = 0;
	this->m_BonusScoreTable[CLASS_WIZARD][1] = 0;
	this->m_BonusScoreTable[CLASS_WIZARD][2] = 0;
	this->m_BonusScoreTable[CLASS_WIZARD][3] = 170;
	this->m_BonusScoreTable[CLASS_WIZARD][4] = 170;
	this->m_BonusScoreTable[CLASS_WIZARD][5] = 170;
	this->m_BonusScoreTable[CLASS_WIZARD][6] = 170;
	
	this->m_BonusScoreTable[CLASS_KNIGHT][0] = 20;
	this->m_BonusScoreTable[CLASS_KNIGHT][1] = 90;
	this->m_BonusScoreTable[CLASS_KNIGHT][2] = 120;
	this->m_BonusScoreTable[CLASS_KNIGHT][3] = 400;
	this->m_BonusScoreTable[CLASS_KNIGHT][4] = 400;
	this->m_BonusScoreTable[CLASS_KNIGHT][5] = 400;
	this->m_BonusScoreTable[CLASS_KNIGHT][6] = 400;

	this->m_BonusScoreTable[CLASS_ELF][0] = 10;
	this->m_BonusScoreTable[CLASS_ELF][1] = 10;
	this->m_BonusScoreTable[CLASS_ELF][2] = 10;
	this->m_BonusScoreTable[CLASS_ELF][3] = 200;
	this->m_BonusScoreTable[CLASS_ELF][4] = 200;
	this->m_BonusScoreTable[CLASS_ELF][5] = 200;
	this->m_BonusScoreTable[CLASS_ELF][6] = 200;

	this->m_BonusScoreTable[CLASS_MAGICGLADIATOR][0] = 0;
	this->m_BonusScoreTable[CLASS_MAGICGLADIATOR][1] = 0;
	this->m_BonusScoreTable[CLASS_MAGICGLADIATOR][2] = 0;
	this->m_BonusScoreTable[CLASS_MAGICGLADIATOR][3] = 0;
	this->m_BonusScoreTable[CLASS_MAGICGLADIATOR][4] = 0;
	this->m_BonusScoreTable[CLASS_MAGICGLADIATOR][5] = 0;
	this->m_BonusScoreTable[CLASS_MAGICGLADIATOR][6] = 0;

	this->m_BonusScoreTable[CLASS_DARKLORD][0] = 0;
	this->m_BonusScoreTable[CLASS_DARKLORD][1] = 0;
	this->m_BonusScoreTable[CLASS_DARKLORD][2] = 0;
	this->m_BonusScoreTable[CLASS_DARKLORD][3] = 0;
	this->m_BonusScoreTable[CLASS_DARKLORD][4] = 0;
	this->m_BonusScoreTable[CLASS_DARKLORD][5] = 0;
	this->m_BonusScoreTable[CLASS_DARKLORD][6] = 0;

	this->m_BonusScoreTable[CLASS_SUMMONER][0] = 0;
	this->m_BonusScoreTable[CLASS_SUMMONER][1] = 0;
	this->m_BonusScoreTable[CLASS_SUMMONER][2] = 0;
	this->m_BonusScoreTable[CLASS_SUMMONER][3] = 0;
	this->m_BonusScoreTable[CLASS_SUMMONER][4] = 0;
	this->m_BonusScoreTable[CLASS_SUMMONER][5] = 0;
	this->m_BonusScoreTable[CLASS_SUMMONER][6] = 0;

	this->m_BonusScoreTable[CLASS_RAGEFIGHTER][0] = 0;
	this->m_BonusScoreTable[CLASS_RAGEFIGHTER][1] = 0;
	this->m_BonusScoreTable[CLASS_RAGEFIGHTER][2] = 0;
	this->m_BonusScoreTable[CLASS_RAGEFIGHTER][3] = 0;
	this->m_BonusScoreTable[CLASS_RAGEFIGHTER][4] = 0;
	this->m_BonusScoreTable[CLASS_RAGEFIGHTER][5] = 0;
	this->m_BonusScoreTable[CLASS_RAGEFIGHTER][6] = 0;

}




CDevilSquare::~CDevilSquare()
{
	this->m_bQuit = TRUE;
}




void CDevilSquare::Init()
{
#if(GS_CASTLE_NOEVENTS == 0)
	this->m_bQuit = TRUE;
	Sleep(500);

	this->SetState(DevilSquare_CLOSE);
	this->m_bQuit = FALSE;

	DWORD dwThreadId;

//#ifdef _DEBUG
	this->m_hThread = (UINT)_beginthreadex( 0, 0, DevilSquareThreadFunc, this, 0, (LPUINT)&dwThreadId);
//#else
//	this->m_hThread = (UINT)CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)DevilSquareThreadFunc, this, 0, (LPDWORD)&dwThreadId);
//#endif
#endif
}



void CDevilSquare::Load(char * filename)
{
	for ( int i=0;i<MAX_DEVILSQUARE_GROUND;i++)
	{
		this->m_DevilSquareGround[i].Init(i);
	}

	int Token;
	int number;
	int type;
	int monstertype;
	int starttime;
	int endtime;
	int x;
	int y;
	int tx;
	int ty;

	SMDFile = fopen(filename, "r");

	if ( SMDFile == NULL )
	{
		MsgBox("[DevilSquare] Info file Load Fail [%s]", filename);
		return;
	}

	while ( true )
	{
		Token = GetToken();

		if ( Token == 2 )
		{
			break;
		}

		type = TokenNumber;

		while ( true )
		{
			if ( type == 0 )	// Time Sync
			{
				Token = GetToken();

				if ( strcmp("end", TokenString) == 0 )
				{
					break;
				}

				this->m_iCloseTime = TokenNumber;

				Token = GetToken();
				this->m_iOpenTime = TokenNumber;

				Token = GetToken();
				this->m_iPlaytime = TokenNumber;
			}
			else if ( type == 1 )	// Normal Monster Settings
			{
				Token = GetToken();

				if ( strcmp("end", TokenString) == 0 )
				{
					break;
				}

				number = TokenNumber;	// Devils Square Index;

				Token = GetToken();
				monstertype = TokenNumber;

				Token = GetToken();
				starttime = TokenNumber;

				Token = GetToken();
				endtime = TokenNumber;

				this->m_DevilSquareGround[number].Set(monstertype, starttime, endtime);
			}
			else if ( type == 2 )	// Boss Monster Set
			{
				Token = GetToken();

				if ( strcmp("end", TokenString) == 0 )
				{
					break;
				}

				number = TokenNumber;	// Devils Square Index;

				Token = GetToken();
				monstertype = TokenNumber;

				Token = GetToken();
				starttime = TokenNumber;

				Token = GetToken();
				x = TokenNumber;

				Token = GetToken();
				y = TokenNumber;

				Token = GetToken();
				tx = TokenNumber;

				Token = GetToken();
				ty = TokenNumber;

				this->m_DevilSquareGround[number].SetBoss(monstertype, starttime, x, y, tx, ty);
			}
			else if ( type == 3 ) // Bonus Sttings
			{
				Token = GetToken();

				if ( strcmp("end", TokenString) == 0 )
				{
					break;
				}

				int index;
				int rank;
				int zen;
				int exp;

				index = TokenNumber;

				Token = GetToken();
				rank = TokenNumber;

				Token = GetToken();
				exp = TokenNumber;

				Token = GetToken();
				zen = TokenNumber;

				this->m_DevilSquareGround[index].SetBonus(rank, exp, zen);
			}
		}
	}

	fclose (SMDFile );

	LogAdd("%s file load!", filename);
}


void CDevilSquare::SetState(enum eDevilSquareState eState)
{
	this->m_eState = eState;

	switch ( this->m_eState )
	{
		case DevilSquare_CLOSE:		this->SetClose();	break;
		case DevilSquare_OPEN:		this->SetOpen();	break;
		case DevilSquare_PLAYING:	this->SetPlaying();	break;
	}
}


void CDevilSquare::SetClose()
{
	if ( LogAdd != NULL )	// LoL
	{
		LogAddTD("[DevilSquare] Closed");
	}

	this->ClearMonstr();
	this->CalcScore();

	for ( int i=0;i<MAX_DEVILSQUARE_GROUND;i++)
	{
		this->m_DevilSquareGround[i].Clear();
	}

	this->m_iRemainTime = this->m_iCloseTime;
	this->CheckSync();
	this->m_bFlag = 0;

	if ( this->m_iRemainTime <= 1 )
	{
		this->m_iTime = GetTickCount();
		this->m_iremainTimeSec = 60;
	}
	else
	{
		this->m_iTime = GetTickCount() + 60000;
		this->m_iremainTimeSec = -1;
	}
	
	this->m_bSendTimeCount = FALSE;
}





void CDevilSquare::SetOpen()
{
	this->m_iRemainTime = this->m_iOpenTime;

	if ( this->m_iOpenTime <= 1 )
	{
		this->m_iTime = GetTickCount();
		this->m_iremainTimeSec = 60;
	}
	else
	{
		this->m_iTime = GetTickCount() + 60000;
		this->m_iremainTimeSec = -1;
	}

	this->m_bSendTimeCount = FALSE;
}





void CDevilSquare::SetPlaying()
{
	this->gObjScoreClear();
	this->SetMonster();

	this->m_iRemainTime = this->m_iPlaytime;

	if ( this->m_iPlaytime <= 1 )
	{
		this->m_iTime = GetTickCount();
		this->m_iremainTimeSec = 60;
	}
	else
	{
		this->m_iTime = GetTickCount() + 60000;
		this->m_iremainTimeSec = -1;
	}
	
	this->m_bSendTimeCount = FALSE;
}





int  CDevilSquare::GetDevilSquareIndex(int iGateNumber)
{
	switch ( iGateNumber )
	{

		case 58:
		case 59:
		case 60:
		case 61:
			return iGateNumber - 58;	// Devil 1 - 4
			break;

		case 78:
			return iGateNumber - 71;	// Devil 1 - 4
			break;

		case 111:
		case 112:
			return iGateNumber - 107;	// Devil 5 - 6
			break;

		case 270:
			return iGateNumber - 264; //Devil 7
			break;

		default:
			return -1;
			break;
	}
}





int  CDevilSquare::GetUserLevelToEnter(int iUserIndex, WORD& btMoveGate)
{
	btMoveGate = -1;

	if ( gObjIsConnected(iUserIndex) == FALSE )
	{
		return -1;
	}

	int iENTER_LEVEL = -1;

	if(gObjIsNewClass(&gObj[iUserIndex])==1)
	{
		iENTER_LEVEL = 6;
		btMoveGate = g_sttDEVILSQUARE_LEVEL[6].MoveGate;
	}else
	{
		for ( int i=0;i<MAX_DEVILSQUARE_GROUND-1;i++)
		{
			if (gObj[iUserIndex].Class == CLASS_DARKLORD || 
				gObj[iUserIndex].Class == CLASS_MAGICGLADIATOR || 
				gObj[iUserIndex].Class == CLASS_RAGEFIGHTER )
			{
				if ( gObj[iUserIndex].Level >= g_sttDEVILSQUARE_LEVEL[i].SpecialCharacterMinLevel  && gObj[iUserIndex].Level <= g_sttDEVILSQUARE_LEVEL[i].SpecialCharacterMaxLevel )
				{
					iENTER_LEVEL = i;
					btMoveGate = g_sttDEVILSQUARE_LEVEL[i].MoveGate;
					break;
				}
			}
			else
			{
				if ( gObj[iUserIndex].Level >= g_sttDEVILSQUARE_LEVEL[i].NormalCharacterMinLevel  && gObj[iUserIndex].Level <= g_sttDEVILSQUARE_LEVEL[i].NormalCharacterMaxLevel )
				{
					iENTER_LEVEL = i;
					btMoveGate = g_sttDEVILSQUARE_LEVEL[i].MoveGate;
					break;
				}
			}
		}
	}

	return iENTER_LEVEL;
}

BOOL CDevilSquare::Run()
{
	int count = 0;

	while ( this->m_bQuit == FALSE )
	{
		if ( gDevilSquareEvent != FALSE )
		{
			switch ( this->m_eState )
			{
				case DevilSquare_CLOSE:	this->ProcClose();		break;
				case DevilSquare_OPEN:		this->ProcOpen();		break;
				case DevilSquare_PLAYING:	this->ProcPlaying();	break;
			}
		}

		Sleep(100);
		count++;

		if ( count > 600 )
		{
			this->SendLiveCall();
			count = 0;
		}
	}

	return TRUE;
}


void CDevilSquare::CheckSync()
{
	tm * today;
	time_t ltime;

	time(&ltime);
	today = localtime(&ltime);

	if ( (today->tm_hour % 2) == 0 )
	{
		this->m_iRemainTime = 120 - today->tm_min - this->m_iOpenTime;
	}
	else if ( today->tm_min >= 55 )
	{
		this->m_iRemainTime = 60 - today->tm_min - this->m_iOpenTime;
		this->m_iRemainTime += 120;
	}
	else
	{
		this->m_iRemainTime = 60 - today->tm_min - this->m_iOpenTime;
	}

	if ( this->m_iRemainTime < 0 )
	{
		this->m_iRemainTime += 60;
	}

	LogAddTD("[DevilSquare] Sync Open Time. [%d]min remain", this->m_iRemainTime);
}



void CDevilSquare::ProcClose()
{
	if ( this->m_iremainTimeSec != -1 )
	{
		int lc2 = (GetTickCount() - this->m_iTime)/1000;

		if ( lc2 != 0 )
		{
			this->m_iremainTimeSec -= lc2;
			this->m_iTime += lc2 * 1000;

			if ( this->m_iremainTimeSec <= 30 && this->m_bSendTimeCount == FALSE )
			{
				PMSG_SET_DEVILSQUARE pMsg;

				PHeadSetB((LPBYTE)&pMsg, 0x92, sizeof(pMsg));
				pMsg.Type = DevilSquare_CLOSE;

				for ( int i=OBJ_STARTUSERINDEX;i<OBJMAX;i++)
				{
					if ( gObj[i].Connected == PLAYER_PLAYING && gObj[i].Type == OBJ_USER)
					{
						if ( BC_MAP_RANGE(gObj[i].MapNumber) == FALSE )
						{
							if ( CC_MAP_RANGE(gObj[i].MapNumber) == FALSE )
							{
								DataSend(i, (BYTE *)&pMsg, pMsg.h.size);
							}
						}
					}
				}


				this->m_bSendTimeCount = TRUE;
			}		

			if (this->m_iremainTimeSec < 1 )
			{
				PMSG_NOTICE pNotice;
				pNotice.type = 0;	// 3
				pNotice.btCount = 0;	// 4
				pNotice.wDelay = 0;	// 6	
				pNotice.dwColor = 0;	// 8
				pNotice.btSpeed = 0;	// C

				TNotice::MakeNoticeMsg((TNotice *)&pNotice, 0, lMsg.Get(MSGGET(2, 191)));

				for ( int i=OBJ_STARTUSERINDEX;i<OBJMAX;i++)
				{
					if ( gObj[i].Connected == PLAYER_PLAYING && gObj[i].Type == OBJ_USER)
					{
						if ( BC_MAP_RANGE(gObj[i].MapNumber) == FALSE )
						{
							if ( CC_MAP_RANGE(gObj[i].MapNumber) == FALSE )
							{
								DataSend(i, (BYTE *)&pNotice, pNotice.h.size);
							}
						}
					}
				}

				LogAddTD(pNotice.Notice);
				this->SetState(DevilSquare_OPEN);
			}
		
		}
	}
	else if ( this->m_iTime < GetTickCount() )
	{
		this->ClearMonstr();
		this->m_iRemainTime--;

		if ( this->m_bFlag == 0 )
		{
			for ( int i=OBJ_STARTUSERINDEX;i<OBJMAX;i++)
			{
				if ( gObj[i].Connected == PLAYER_PLAYING )
				{
					if ( DS_MAP_RANGE(gObj[i].MapNumber) != FALSE )
					{
						gObjMoveGate(i, 27);
					}
				}
			}

			this->ItemClear();
		}
		else
		{
			this->CheckInvalidUser();
		}

		if ( this->m_iRemainTime <= 15 )
		{
			if ( (this->m_iRemainTime%5)== 0 )
			{
				PMSG_NOTICE pNotice;
				pNotice.type = 0;	// 3
				pNotice.btCount = 0;	// 4
				pNotice.wDelay = 0;	// 6	
				pNotice.dwColor = 0;	// 8
				pNotice.btSpeed = 0;	// C

				TNotice::MakeNoticeMsgEx((TNotice *)&pNotice, 0, lMsg.Get(MSGGET(2, 192)), this->m_iRemainTime);

				for ( int i=OBJ_STARTUSERINDEX;i<OBJMAX;i++)
				{
					if ( gObj[i].Connected == PLAYER_PLAYING && gObj[i].Type == OBJ_USER)
					{
						if ( BC_MAP_RANGE(gObj[i].MapNumber) == FALSE )
						{
							if ( CC_MAP_RANGE(gObj[i].MapNumber) == FALSE )
							{
								DataSend(i, (BYTE *)&pNotice, pNotice.h.size);
							}
						}
					}
				}

				LogAddTD((char*)pNotice.Notice);
			}
		}

		if ( this->m_iRemainTime == 1 && this->m_iremainTimeSec == -1 )
		{
			this->m_iremainTimeSec = 60;
			this->m_iTime = GetTickCount();
		}
		else
		{
			this->m_iTime = GetTickCount() + 60000;
		}
	}
}




void CDevilSquare::ProcOpen()
{
	if ( this->m_iremainTimeSec != -1 )
	{

		int lc2 = (GetTickCount() - this->m_iTime)/1000;

		if ( lc2 != 0 )
		{
			this->m_iremainTimeSec -= lc2;
			this->m_iTime += lc2 * 1000;

			if ( this->m_iremainTimeSec <= 30 && this->m_bSendTimeCount == FALSE )
			{
				PMSG_SET_DEVILSQUARE pMsg;

				PHeadSetB((LPBYTE)&pMsg, 0x92, sizeof(pMsg));
				pMsg.Type = DevilSquare_OPEN;

				for ( int i=OBJ_STARTUSERINDEX;i<OBJMAX;i++)
				{
					if ( gObj[i].Connected == PLAYER_PLAYING && gObj[i].Type == OBJ_USER)
					{
						if ( BC_MAP_RANGE(gObj[i].MapNumber) == FALSE )
						{
							if ( CC_MAP_RANGE(gObj[i].MapNumber) == FALSE )
							{
								DataSend(i, (BYTE *)&pMsg, pMsg.h.size);
							}
						}
					}
				}

				this->m_bSendTimeCount = TRUE;
			}

			if (this->m_iremainTimeSec < 1 )
			{
				this->SendEventStartMsg();
				this->SetState(DevilSquare_PLAYING);
			}
		
		}
	}
	else
	{
		if ( this->m_iTime < GetTickCount() )
		{
			this->ClearMonstr();
			this->m_iRemainTime--;

			if ( this->m_iRemainTime > 0 )
			{
				PMSG_NOTICE pNotice;
				pNotice.type = 0;	// 3
				pNotice.btCount = 0;	// 4
				pNotice.wDelay = 0;	// 6	
				pNotice.dwColor = 0;	// 8
				pNotice.btSpeed = 0;	// C

				TNotice::MakeNoticeMsgEx(&pNotice, 0, lMsg.Get(MSGGET(2, 193)), this->m_iRemainTime);

				for ( int i=OBJ_STARTUSERINDEX;i<OBJMAX;i++)
				{
					if ( gObj[i].Connected == PLAYER_PLAYING && gObj[i].Type == OBJ_USER)
					{
						if ( BC_MAP_RANGE(gObj[i].MapNumber) == FALSE )
						{
							if ( CC_MAP_RANGE(gObj[i].MapNumber) == FALSE )
							{
								DataSend(i, (BYTE *)&pNotice, pNotice.h.size);
							}
						}
					}
				}

				LogAddTD((char*)pNotice.Notice);

				this->m_iTime = GetTickCount();
			}

			if (this->m_iRemainTime == 1 && this->m_iremainTimeSec == -1)
			{
				this->m_iremainTimeSec = 60;
				this->m_iTime = GetTickCount();
			}
			else
			{
				this->m_iTime = GetTickCount() + 60000;
			}
		}
	}
}







void CDevilSquare::ProcPlaying()
{
	if ( this->m_iremainTimeSec != -1 )
	{

		int lc2 = (GetTickCount() - this->m_iTime)/1000;

		if ( lc2 != 0 )
		{
			this->m_iremainTimeSec -= lc2;
			this->m_iTime += lc2 * 1000;

			if ( this->m_iremainTimeSec <= 30 && this->m_bSendTimeCount == FALSE )
			{
				PMSG_SET_DEVILSQUARE pMsg;

				PHeadSetB((LPBYTE)&pMsg, 0x92, sizeof(pMsg));
				pMsg.Type = DevilSquare_PLAYING;
				AllSendSameMapMsg((UCHAR*)&pMsg, sizeof(pMsg), MAP_INDEX_DEVILSQUARE);

				this->m_bSendTimeCount = TRUE;
			}



			if ( this->m_iremainTimeSec < 1 )
			{
				this->SetState(DevilSquare_CLOSE);
			}
		}
	}
	else
	{
		if ( this->m_iTime < GetTickCount() )
		{
			this->m_iRemainTime--;

			if ( this->m_iRemainTime > 0 )
			{
				if ( (this->m_iRemainTime%5) == 0 )
				{
					PMSG_NOTICE pNotice;
					pNotice.type = 0;	// 3
					pNotice.btCount = 0;	// 4
					pNotice.wDelay = 0;	// 6	
					pNotice.dwColor = 0;	// 8
					pNotice.btSpeed = 0;	// C

					TNotice::MakeNoticeMsgEx((TNotice *)&pNotice, 0, lMsg.Get(MSGGET(2, 194)), this->m_iRemainTime);
					LogAddTD((char*)pNotice.Notice);
					AllSendSameMapMsg((UCHAR*)&pNotice, pNotice.h.size, MAP_INDEX_DEVILSQUARE);
				}

				for ( int i=0;i<MAX_DEVILSQUARE_GROUND;i++)
				{
					this->m_DevilSquareGround[i].RegenBossMonster(this->m_iPlaytime - this->m_iRemainTime);
				}

				if ( this->m_iRemainTime == 1 && this->m_iremainTimeSec == -1 )
				{
					this->m_iremainTimeSec = 60;
					this->m_iTime = GetTickCount();
				}
				else
				{
					this->m_iTime = GetTickCount() + 60000;
				}
			}
		}
	}
}





void CDevilSquare::SetMonster()
{
	int result;
	for ( int n=0;n<gMSetBase.m_Count;n++)
	{
		if ( DS_MAP_RANGE(gMSetBase.m_Mp[n].m_MapNumber) != FALSE )
		{
			WORD wMonIndex = gMSetBase.m_Mp[n].m_Type;
			BYTE btDSIndex = -1;

			if (		gMSetBase.m_Mp[n].m_X >= 110 && gMSetBase.m_Mp[n].m_X <= 160 && 
						gMSetBase.m_Mp[n].m_Y >= 80 && gMSetBase.m_Mp[n].m_Y <= 120 && 
						gMSetBase.m_Mp[n].m_MapNumber == MAP_INDEX_DEVILSQUARE )
			{
				btDSIndex = DEVIL_SQUARE_GROUND_1;
			}
			else if (	gMSetBase.m_Mp[n].m_X >= 120 && gMSetBase.m_Mp[n].m_X <= 160 && 
						gMSetBase.m_Mp[n].m_Y >= 150 && gMSetBase.m_Mp[n].m_Y <= 190 && 
						gMSetBase.m_Mp[n].m_MapNumber == MAP_INDEX_DEVILSQUARE )
			{
				btDSIndex = DEVIL_SQUARE_GROUND_2;
			}
			else if (	gMSetBase.m_Mp[n].m_X >= 40 && gMSetBase.m_Mp[n].m_X <= 90 && 
						gMSetBase.m_Mp[n].m_Y >= 130 && gMSetBase.m_Mp[n].m_Y <= 180 && 
						gMSetBase.m_Mp[n].m_MapNumber == MAP_INDEX_DEVILSQUARE )
			{
				btDSIndex = DEVIL_SQUARE_GROUND_3;
			}
			else if (	gMSetBase.m_Mp[n].m_X >= 50 && gMSetBase.m_Mp[n].m_X <= 90 && 
						gMSetBase.m_Mp[n].m_Y >= 70 && gMSetBase.m_Mp[n].m_Y <= 110 && 
						gMSetBase.m_Mp[n].m_MapNumber == MAP_INDEX_DEVILSQUARE )
			{
				btDSIndex = DEVIL_SQUARE_GROUND_4;
			}
			else if (	gMSetBase.m_Mp[n].m_X >= 120 && gMSetBase.m_Mp[n].m_X <= 150 && 
						gMSetBase.m_Mp[n].m_Y >= 80 && gMSetBase.m_Mp[n].m_Y <= 120 && 
						gMSetBase.m_Mp[n].m_MapNumber == MAP_INDEX_DEVILSQUARE2 )
			{
				btDSIndex = DEVIL_SQUARE_GROUND_5;
			}
			else if (	gMSetBase.m_Mp[n].m_X >= 120 && gMSetBase.m_Mp[n].m_X <= 160 && 
						gMSetBase.m_Mp[n].m_Y >= 150 && gMSetBase.m_Mp[n].m_Y <= 190 && 
						gMSetBase.m_Mp[n].m_MapNumber == MAP_INDEX_DEVILSQUARE2 )
			{
				btDSIndex = DEVIL_SQUARE_GROUND_6;
			}
			else if (	gMSetBase.m_Mp[n].m_X >= 40 && gMSetBase.m_Mp[n].m_X <= 90 && 
						gMSetBase.m_Mp[n].m_Y >= 130 && gMSetBase.m_Mp[n].m_Y <= 180 && 
						gMSetBase.m_Mp[n].m_MapNumber == MAP_INDEX_DEVILSQUARE2 )
			{
				btDSIndex = DEVIL_SQUARE_GROUND_7;
			}

			if ( btDSIndex == 0xFF )
			{
				LogAddTD("[DevilSquare] [%d] Invalid MonterType", wMonIndex);
				continue;
			}

			result = gObjAddMonster(gMSetBase.m_Mp[n].m_MapNumber);

			if ( result >= 0 )
			{
				gObjSetPosMonster(result, n);
				gObjSetMonster(result, wMonIndex,"CDevilSquare::SetMonster"); 
				gObj[result].MaxRegenTime = 1000;	// Rgeneration in 1 second
				gObj[result].m_bDevilSquareIndex = btDSIndex;

				LogAddTD("[DevilSquare][%d][Mob] ID:%d Location: %d [%d,%d] create ", 
					btDSIndex,
					gObj[result].Class,
					gObj[result].MapNumber, gObj[result].X, gObj[result].Y
				);
			}
		}
	}
}





void CDevilSquare::ClearMonstr()
{
	for ( int n=0;n<OBJ_MAXMONSTER;n++)
	{
		if ( DS_MAP_RANGE(gObj[n].MapNumber) != FALSE )
		{
			gObjDel(n);
		}
	}
}





void CDevilSquare::gDevilSquareMonsterRegen(LPOBJ lpObj)
{
	BYTE devilsquareindex = lpObj->m_bDevilSquareIndex;

	if ( devilsquareindex < 0 || devilsquareindex >= MAX_DEVILSQUARE_GROUND )
	{
		LogAddTD("[DevilSquare] Invalid DevilSquareIndex [%d]", devilsquareindex);
		return;
	}

	if ( lpObj->m_PosNum == -1 )
	{
		gObjDel(lpObj->m_Index);
		return;
	}

	WORD monstertype = this->m_DevilSquareGround[devilsquareindex].GetMonsterType(this->m_iPlaytime - this->m_iRemainTime );

	if ( monstertype == (WORD)-1 )
	{
		LogAddTD("[DevilSquare] [%d] Invalid MonterType", monstertype);
		return;
	}

	for ( int n=0;n<MAX_MAGIC;n++)
	{
		lpObj->Magic[n].Clear();
	}

	gObjSetMonster(lpObj->m_Index, monstertype,"CDevilSquare::gDevilSquareMonsterRegen");
	lpObj->DieRegen = FALSE;
	gObjMonsterRegen(lpObj);
	CreateFrustrum(lpObj->X, lpObj->Y, lpObj->m_Index);
	lpObj->m_bDevilSquareIndex = devilsquareindex;
	lpObj->MaxRegenTime = 1000;

	LogAddTD("[DevilSquare] Monter Regen [%d][%d][%d,%d]",
		monstertype, devilsquareindex, lpObj->X, lpObj->Y);
}





void CDevilSquare::SendEventStartMsg()
{
	PMSG_NOTICE pToEventer;
	pToEventer.type = 0;	// 3
	pToEventer.btCount = 0;	// 4
	pToEventer.wDelay = 0;	// 6	
	pToEventer.dwColor = 0;	// 8
	pToEventer.btSpeed = 0;	// C

	PMSG_NOTICE pWithOutEvneter;
	pWithOutEvneter.type = 0;	// 3
	pWithOutEvneter.btCount = 0;	// 4
	pWithOutEvneter.wDelay = 0;	// 6	
	pWithOutEvneter.dwColor = 0;	// 8
	pWithOutEvneter.btSpeed = 0;	// C

	TNotice::MakeNoticeMsg(&pToEventer, 0, lMsg.Get(MSGGET(2, 195)));
	TNotice::MakeNoticeMsg(&pWithOutEvneter, 0, lMsg.Get(MSGGET(2, 196)));

	for ( int n=OBJ_STARTUSERINDEX;n<OBJMAX;n++)
	{
		if ( gObj[n].Connected == PLAYER_PLAYING )
		{
			if ( DS_MAP_RANGE(gObj[n].MapNumber) != FALSE )
			{
				DataSend(n, (BYTE *)&pToEventer, pToEventer.h.size);
			}
			else
			{
				DataSend(n, (BYTE *)&pWithOutEvneter, pWithOutEvneter.h.size);
			}
		}
	}

	LogAddTD("[DevilSquare] Start Event");
}





void CDevilSquare::DieProcDevilSquare(LPOBJ lpObj)
{
	char msg[255];

	wsprintf(msg, lMsg.Get(MSGGET(2, 197)), lpObj->m_nEventScore);
	GCServerMsgStringSend(msg, lpObj->m_Index, 1);

	if ( lpObj->m_nEventScore <= 0 )
		return;

	if ( lpObj->Class == 1 )
	{
		if ( lpObj->Class >= 0 && lpObj->Class < MAX_TYPE_PLAYER )
		{
			if ( DS_LEVEL_RANGE(lpObj->m_bDevilSquareIndex) != FALSE )
			{
				lpObj->m_nEventScore+= this->m_BonusScoreTable[lpObj->Class][lpObj->m_bDevilSquareIndex]/100;
				
			}
		}
	}

	
	if(ReadConfig.SCFRSON == FALSE)
	{
		PMSG_ANS_EVENTUSERSCORE pMsg;

		pMsg.h.c = 0xC1;
		pMsg.h.headcode = 0x01;
		pMsg.h.size = sizeof(pMsg);
		pMsg.SquareNum = lpObj->m_bDevilSquareIndex;
		pMsg.Class = lpObj->Class;
		pMsg.Score = lpObj->m_nEventScore;
		pMsg.ServerCode = gGameServerCode;
		memcpy(pMsg.AccountID, lpObj->AccountID, sizeof(pMsg.AccountID));
		memcpy(pMsg.GameID, lpObj->Name, sizeof(pMsg.GameID));

		DataSendRank((char *)&pMsg, pMsg.h.size);
	}else
	{
		DS_SendRankingInfo(lpObj->m_Index);
	}

	LogAddTD("[DevilSquare] Dead [%s][%s][%d][%d]",
		lpObj->AccountID, lpObj->Name, 
		lpObj->m_nEventExp, lpObj->m_nEventScore);

	lpObj->m_nEventScore = 0;
	lpObj->m_nEventMoney = 0;
	lpObj->m_nEventExp = 0;

}






int  CDevilSquare::gObjMonsterExpSingle(LPOBJ lpObj, LPOBJ lpTargetObj, int dmg, int tot_dmg)
{
	if(lpObj->Type != OBJ_USER)
	{
		return 0;
	}

	unsigned int exp;
	INT maxexp = 0;
	int totalExpMultiplyer = 0;
	int level = ((lpTargetObj->Level + 25) * lpTargetObj->Level) / 3;

	if ( (lpTargetObj->Level + 10) < lpObj->Level )
	{
		level = (level*(lpTargetObj->Level + 10))/lpObj->Level;
	}

	if ( lpTargetObj->Level >= 65 )
	{
		level += (lpTargetObj->Level-64)*(lpTargetObj->Level / 4);
	}

	if ( level > 0 )
	{
		maxexp = level / 2;
	}
	else
	{
		level = 0;
	}

	if ( maxexp < 1 )
	{
		exp = level;
	}
	else
	{
		exp = level + rand()%maxexp;
	}

	exp = (dmg * exp)/tot_dmg;
	DWORD mymaxexp = gObjExpCal(lpObj, 0);	//gLevelExperience[lpObj->Level];

	if((ReadConfig.ExpSys == 1) || (ReadConfig.ExpSys == 2))
	{
		if ( exp > mymaxexp )
		{
			exp = mymaxexp;
		}
	}

	//Get Experience Rate
	totalExpMultiplyer = gObjGetExperienceRate(lpObj,0);

	exp *= totalExpMultiplyer;

	if ( (lpObj->m_wExprienceRate + lpObj->MasterCharacterInfo->IncExperience) == 0 )
		exp = 0;
	else
		exp =  (float)exp * ((float)(lpObj->m_wExprienceRate + lpObj->MasterCharacterInfo->IncExperience)  / 100.0f);

	if ( (lpObj->pInventory[8].IsItem() == TRUE && lpObj->pInventory[8].m_Type == ITEMGET(13,123)) ) //NEW SKELETON EFFECT
	{
		exp = (int)(exp * 1.3f);
	}

	if ( g_CrywolfSync.GetOccupationState() == CRYWOLF_OCCUPATION_FAILED && g_iCrywolfApplyMvpPenalty != FALSE)
	{
#if (PACK_EDITION>=2)
		if ((VipSystem.VipIsApplyCWExpPenalty == 0)&&(lpObj->Vip >= 1))
		{
		} else {
			exp =  (exp * g_CrywolfSync.GetGettingExpPenaltyRate()) / 100;
		}
#else
		exp =  (exp * g_CrywolfSync.GetGettingExpPenaltyRate()) / 100;
#endif
	}

	if(ReadConfig.ExpSys == 0)
	{
		if ( exp > mymaxexp )
		{
			exp = mymaxexp;
		}
	}

#if (PACK_EDITION>=3)
	if(lpObj->BotNumOwner >= OBJ_MAXMONSTER && lpObj->HaveBot == 1)
	{
		int botExp = exp * (gObj[lpObj->BotNumOwner].BotLvlUpExp/100.0f);
		exp -= botExp;
		botPet.AddExp(lpObj->BotNumOwner,botExp);
	}
#endif

	if ( exp > 0 )
	{
		if(ReadConfig.ExpSys == 2)
		{
			int LevelUp;
			gObjSetExpPetItem(lpObj->m_Index, exp);

			if((lpObj->m_wExprienceRate + lpObj->MasterCharacterInfo->IncExperience) > 0 && lpObj->m_btDisableExpGain == 0)
			{
				lpObj->Experience += exp;
			}
			gObjLevelUp(lpObj, exp, 0, EVENT_TYPE_DEVILSQUARE,LevelUp);
		}else
		{
			if ( (lpObj->m_wExprienceRate + lpObj->MasterCharacterInfo->IncExperience) > 0 && lpObj->m_btDisableExpGain == 0)
			{
				gObjSetExpPetItem(lpObj->m_Index, exp);

				lpObj->m_nEventExp += exp;
				int iMAX_LEVCOUNT = 0, iCAL_EXP = exp;

				while ( iCAL_EXP > 0 )
				{
					if ( iCAL_EXP > 0 )
					{
						int LevelUp;
						iCAL_EXP = gObjLevelUp(lpObj, iCAL_EXP, 0, EVENT_TYPE_DEVILSQUARE, LevelUp);
					}

					if(iCAL_EXP == 0)
						break;

					iMAX_LEVCOUNT++;
					if (iMAX_LEVCOUNT > 5)
						break;
				}
				if ((iCAL_EXP < 0)&&(iMAX_LEVCOUNT < 2))
					return -1;
			}
		}
	}

	return exp;
}



void CDevilSquare::gObjExpParty(LPOBJ lpObj, LPOBJ lpTargetObj, int AttackDamage, BOOL MSBFlag)
{
	int n;
	unsigned int exp;
	INT maxexp = 0;
	int totalexp;
	int level = ((lpTargetObj->Level + 25) * lpTargetObj->Level) / 3;
	int number;
	int partynum = 0;
	int totallevel = 0;
	int partylevel;
	int partycount;
	int dis[MAX_USER_IN_PARTY];
	int viewplayer = 0;
	int viewpercent = 100;
	BOOL bApplaySetParty = FALSE;
	BOOL bCheckSetParty[MAX_TYPE_PLAYER];
	partynum = lpObj->PartyNumber;
	LPOBJ lpPartyObj;

	int toplevel = 0;
	int totalExpMultiplyer = 0;

	for (n=0;n<MAX_USER_IN_PARTY;n++)
	{
		number = gParty.m_PartyS[partynum].Number[n];

		if ( number >= 0 )
		{
			lpPartyObj = &gObj[number];

			if ( lpPartyObj->Level > toplevel )
			{
				toplevel = lpPartyObj->Level;
			}
		}
	}

	if ( OBJMAX_RANGE(partynum) == FALSE )
	{
		LogAdd("error : %s %d", __FILE__, __LINE__);
		return;
	}

	partycount = gParty.m_PartyS[partynum].Count;

	for ( n =0;n<MAX_USER_IN_PARTY;n++)
	{
		number = gParty.m_PartyS[partynum].Number[n];

		if ( number >= 0 )
		{
			lpPartyObj = &gObj[number];

			if ( lpTargetObj->MapNumber == lpPartyObj->MapNumber )
			{
				dis[n] = gObjCalDistance(lpTargetObj, &gObj[number]);

				if ( dis[n] < 10 )
				{
					lpPartyObj = &gObj[number];

					if ( toplevel >= (lpPartyObj->Level + 200 ) ) // #formula
					{
						totallevel = totallevel + lpPartyObj->Level + 200;
					}
					else
					{
						totallevel += lpPartyObj->Level;
					}

					viewplayer++;
					bCheckSetParty[lpPartyObj->Class] = true;
				}
			}
		}
	}

	if ( bCheckSetParty[CLASS_WIZARD] != false && bCheckSetParty[CLASS_KNIGHT] != false && bCheckSetParty[CLASS_ELF] != false )
	{
		bApplaySetParty = TRUE;
	}

	if ( viewplayer > 1 )
	{
		if ( bApplaySetParty != FALSE )
		{
			if ( viewplayer == 3 )
			{
				viewpercent = ReadConfig.gObjExpParty_viewpercent1;
			}
			else if ( viewplayer == 4 )
			{
				viewpercent = ReadConfig.gObjExpParty_viewpercent2;
			}
			else if ( viewplayer >= 5 )
			{
				viewpercent = ReadConfig.gObjExpParty_viewpercent3;
			}
			else
			{
				viewpercent = ReadConfig.gObjExpParty_viewpercent4;
			}
		}
		else
		{
			if ( viewplayer == 2 )
			{
				viewpercent = ReadConfig.gObjExpParty_viewpercent5;
			}
			else if ( viewplayer == 3 )
			{
				viewpercent = ReadConfig.gObjExpParty_viewpercent6;
			}
			else if ( viewplayer == 4 )
			{
				viewpercent = ReadConfig.gObjExpParty_viewpercent7;
			}
			else if ( viewplayer == 5 )
			{
				viewpercent = ReadConfig.gObjExpParty_viewpercent8;
			}
			else
			{
				viewpercent = ReadConfig.gObjExpParty_viewpercent9;
			}
		}

		partylevel = totallevel / viewplayer;
	}
	else
	{
		partylevel = totallevel;
	}

	if ( (lpTargetObj->Level +10) < partylevel )
	{
		level = (level * (lpTargetObj->Level+10) ) / partylevel;
	}

	if ( lpTargetObj->Level >= 65 )
	{
		if ( viewplayer == 1 )
		{
			level += (lpTargetObj->Level - 64) * (lpTargetObj->Level/ 4);
		}
		else
		{
			level += (200.0 - (lpObj->Level * 0.2));
		}
	}

	if ( level > 0 )
	{
		maxexp = level / 2;
	}
	else
	{
		level = 0;
	}

	if ( maxexp < 1 )
	{
		totalexp = level;
	}
	else
	{
		totalexp = level + rand()%maxexp;
	}

	if ( lpTargetObj->Type == OBJ_MONSTER )
	{
		lpTargetObj->Money = totalexp;
	}

	for ( n=0;n<MAX_USER_IN_PARTY;n++)
	{
		number = gParty.m_PartyS[partynum].Number[n];

		if ( number >= 0 )
		{
			lpPartyObj = &gObj[number];
			if(lpPartyObj->Type != OBJ_USER)
			{
				continue;
			}

			if ( lpTargetObj->MapNumber == lpPartyObj->MapNumber )
			{
				if ( dis[n] < 10 )
				{
					DWORD myexp = gObjExpCal(lpPartyObj, 0);	//gLevelExperience[lpPartyObj->Level];
					exp = ((totalexp * viewpercent* lpPartyObj->Level ) / totallevel ) / 100;
					
					if(ReadConfig.ExpSys == 1)
					{
						if ( exp > myexp )
						{
							exp = myexp;
						}
					}

					//Get Exp Rate for the user
					totalExpMultiplyer = gObjGetExperienceRate(lpPartyObj,0);

					if (totalExpMultiplyer < 1)
					{
						totalExpMultiplyer = 1;
					}

					exp *= totalExpMultiplyer;
				

					if ( (lpPartyObj->m_wExprienceRate + lpPartyObj->MasterCharacterInfo->IncExperience) == 0 )
						exp = 0;
					else
						exp =  (float)exp * ((float)(lpPartyObj->m_wExprienceRate + lpPartyObj->MasterCharacterInfo->IncExperience)  / 100.0f);

					if ( (lpPartyObj->pInventory[8].IsItem() == TRUE && lpPartyObj->pInventory[8].m_Type == ITEMGET(13,123)) ) //NEW SKELETON EXTRA EXP
					{
						exp = (int)(exp * 1.3f);
					}

					if ( g_CrywolfSync.GetOccupationState() == CRYWOLF_OCCUPATION_FAILED && g_iCrywolfApplyMvpPenalty != FALSE)
					{
#if (PACK_EDITION>=2)
						if ((VipSystem.VipIsApplyCWExpPenalty == 0)&&(lpPartyObj->Vip >= 1))
						{
						} else {
							exp =  (exp * g_CrywolfSync.GetGettingExpPenaltyRate()) / 100;
						}
#else
						exp =  (exp * g_CrywolfSync.GetGettingExpPenaltyRate()) / 100;
#endif
					}

					if(ReadConfig.ExpSys == 0)
					{
						if ( exp > myexp )
						{
							exp = myexp;
						}
					}

#if (PACK_EDITION>=3)
					if(lpPartyObj->BotNumOwner >= OBJ_MAXMONSTER && lpPartyObj->HaveBot == 1)
					{
						int botExp = exp * (gObj[lpPartyObj->BotNumOwner].BotLvlUpExp/100.0f);
						exp -= botExp;
						botPet.AddExp(lpPartyObj->BotNumOwner,botExp);
					}
#endif
					int LevelUp;
					if ( exp > 0 )
					{
						if ( (lpPartyObj->m_wExprienceRate + lpPartyObj->MasterCharacterInfo->IncExperience) > 0 && lpPartyObj->m_btDisableExpGain == 0 )
						{
							gObjSetExpPetItem(lpPartyObj->m_Index, exp);
							lpPartyObj->m_nEventExp += exp;
							int iMAX_LEVCOUNT = 0, iCAL_EXP = exp;

							while ( iCAL_EXP > 0 )
							{
								if ( iCAL_EXP > 0 )
								{
									iCAL_EXP = gObjLevelUp(lpPartyObj, iCAL_EXP, 0, EVENT_TYPE_DEVILSQUARE_PARTY, LevelUp);
								}

								if(iCAL_EXP == 0)
									break;

								iMAX_LEVCOUNT++;
								if (iMAX_LEVCOUNT > 5)
									break;
							}
							if ((iCAL_EXP < 0)&&(iMAX_LEVCOUNT < 2))
								continue;
						}
					}

					if(LevelUp == 0)
					{
						if ( (lpPartyObj->m_wExprienceRate + lpPartyObj->MasterCharacterInfo->IncExperience) > 0 && lpPartyObj->m_btDisableExpGain == 0 )
						{
							GCKillPlayerExpSend(lpPartyObj->m_Index, lpTargetObj->m_Index, exp, AttackDamage, MSBFlag);
						}
					}
				}
			}
		}
	}
}






void CDevilSquare::gObjMonsterScoreDivision(LPOBJ lpMonObj, LPOBJ lpObj, int AttackDamage, BOOL MSBFlag)
{
	::gObjMonsterHitDamageUserDel(lpMonObj);
	lpMonObj->Money = 0;

	LPOBJ lpTargetObj;
	int HitIndex;
	int LastHitObjNum = ::gObjMonsterLastHitDamageUser(lpMonObj, HitIndex);

	if ( LastHitObjNum != -1 )
	{
		lpTargetObj = &gObj[LastHitObjNum];
		int lc5 = lpMonObj->sHD[HitIndex].HitDamage / lpMonObj->MaxLife * lpMonObj->Level;
		lc5 *= lpTargetObj->m_bDevilSquareIndex + 1;
		lpTargetObj->m_nEventScore += lc5;
	}
}




void CDevilSquare::ItemClear()
{
	for ( int i=0;i<MAX_MAPITEM;i++)
	{
		MapC[MAP_INDEX_DEVILSQUARE].m_cItem[i].m_State = 8;
	}
}




void CDevilSquare::CalcScore()
{
#if (WL_PROTECT==1)
	_beginthread( CViewportGuild__SystemProcessesScan, 0, NULL  );
#endif
	for ( int n=0;n<MAX_DEVILSQUARE_GROUND;n++)
	{
		this->m_DevilSquareGround[n].ClearScore();
	}

	for ( int n=OBJ_STARTUSERINDEX;n<OBJMAX;n++)
	{
		if ( gObj[n].Connected == PLAYER_PLAYING )
		{
			if ( DS_MAP_RANGE(gObj[n].MapNumber) != FALSE )
			{
				if ( DS_LEVEL_RANGE(gObj[n].m_bDevilSquareIndex) != FALSE )
				{
					//QUEST FINISH
					qs5.FinishEvent(n,1,gObj[n].m_bDevilSquareIndex+1); //FINISH BC QUEST
					this->m_DevilSquareGround[gObj[n].m_bDevilSquareIndex].InsertObj(&gObj[n]);
				}
			}
		}
	}

	for (int n=0;n<MAX_DEVILSQUARE_GROUND;n++)
	{
		this->m_DevilSquareGround[n].SortScore();
		this->m_DevilSquareGround[n].SendScore();
	}
}



struct PMSG_ANS_EVENTLIVE
{
	PBMSG_HEAD h;	// C1:03
};




void CDevilSquare::SendLiveCall()
{	
	if(ReadConfig.SCFRSON == FALSE)
	{
		PMSG_ANS_EVENTLIVE pMsg;
		
		pMsg.h.c = 0xC1;
		pMsg.h.headcode = 0x03;
		pMsg.h.size = sizeof(pMsg);

		DataSendRank((char *)&pMsg, pMsg.h.size);
	}
}




void CDevilSquare::gObjScoreClear()
{
	for ( int n=OBJ_STARTUSERINDEX;n<OBJMAX;n++)
	{
		if ( gObj[n].Connected == PLAYER_PLAYING )
		{
			if ( DS_MAP_RANGE(gObj[n].MapNumber) != FALSE )
			{
				gObj[n].m_nEventScore = 0;
				gObj[n].m_nEventMoney = 0;
				gObj[n].m_nEventExp = 0;
			}
		}
	}
}






void CDevilSquare::CheckInvalidUser()
{
	for ( int n=OBJ_STARTUSERINDEX;n<OBJMAX;n++)
	{
		if ( gObj[n].Connected == PLAYER_PLAYING )
		{
			if ( DS_MAP_RANGE(gObj[n].MapNumber) != FALSE )
			{
				if ( this->m_eState == DevilSquare_CLOSE )
				{
					LogAddC(2, "[DevilSquare] [%s][%s] Found user in DevilSquare [State:Close]",
						gObj[n].AccountID, gObj[n].Name);
					gObjUserKill(gObj[n].m_Index);
				}
			}
		}
	}
}
