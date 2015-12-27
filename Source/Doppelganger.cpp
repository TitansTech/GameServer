#include "stdafx.h"
#include "user.h"
#include "DoppelGanger.h"
#include "protocol.h"
#include "logproc.h"
#include "..\common\winutil.h"
#include "..\include\readscript.h"
#include "TMonsterAIUtil.h"
#include "NpcTalk.h"
#include "BloodCastle.h"
#include "DevilSquare.h"
#include "GameMain.h"
#include "Event.h"
#include "TNotice.h"
#include "gObjMonster.h"
#include "LogToFile.h"
#include "ExServerProtocol.h"
#include "SCFExDB_Defines.h"

extern CLogToFile ANTI_HACK_LOG;
CDoppelGanger g_DoppelGanger;
static TMonsterAIUtil DG_UTIL;

void CDoppelGanger::Init()
{

	// player pos
	this->POSX[0] = 197,	this->POSY[0] = 30;
	this->POSX[1] = 133,	this->POSY[1] = 68;
	this->POSX[2] = 110,	this->POSY[2] = 60;
	this->POSX[3] = 95,		this->POSY[3] = 15;

	// monster pos
	this->MPOSX[0] = 224,	this->MPOSY[0] = 100;
	this->MPOSX[1] = 113,	this->MPOSY[1] = 180;
	this->MPOSX[2] = 110,	this->MPOSY[2] = 150;
	this->MPOSX[3] = 43,	this->MPOSY[3] = 108;

	this->m_DGData.Clear();

	char FilePath[] = "..\\SCFData\\EventsConfig\\SCF_Doppelganger.ini";

	this->Enabled				= GetPrivateProfileInt("Common", "SCFDoppelGangerEventEnabled",0, FilePath) ;
	this->InterimChestRate		= GetPrivateProfileInt("Common", "SCFDoppelGangerInterimChestRate",25, FilePath) ;
	this->GOER_MOBS_DROP_ITEM	= GetPrivateProfileInt("Common", "SCFDoppelNormalMobsDropItems",0, FilePath) ;
	this->GOER_MAX_PASS_PORTAL  = GetPrivateProfileInt("Common", "SCFDoppelGangerMaxMonstersPassInPortal",3, FilePath) ;

	this->SaveInDB				= GetPrivateProfileInt("Events", "SCFDoppelGangerSaveRankInDB",0, RANKINGINI) ;

	if(this->GOER_MAX_PASS_PORTAL < 1)
		this->GOER_MAX_PASS_PORTAL = 1;

	this->UseAutoStat			= GetPrivateProfileInt("MonsterStats", "SCFDoppelGangerAutoIncrementMobStat",25, FilePath) ;
	this->PartyAutoStat[0]		= GetPrivateProfileInt("MonsterStats", "SCFDoppelGangerPartyIncrementMobStat2",25, FilePath) ;
	this->PartyAutoStat[1]		= GetPrivateProfileInt("MonsterStats", "SCFDoppelGangerPartyIncrementMobStat3",50, FilePath) ;
	this->PartyAutoStat[2]		= GetPrivateProfileInt("MonsterStats", "SCFDoppelGangerPartyIncrementMobStat4",100, FilePath) ;
	this->PartyAutoStat[3]		= GetPrivateProfileInt("MonsterStats", "SCFDoppelGangerPartyIncrementMobStat5",150, FilePath) ;

	this->DropBagOnKillBoss[0]	= GetPrivateProfileInt("MonsterStats", "SCFDoppelGangerKillFuriosSlaughtererDropBag",0, FilePath) ;
	this->DropBagOnKillBoss[1]	= GetPrivateProfileInt("MonsterStats", "SCFDoppelGangerKillSlaughtererDropBag",0, FilePath) ;
	this->DropBagOnKillBoss[2]	= GetPrivateProfileInt("MonsterStats", "SCFDoppelGangerKillIceWalkerDropBag",0, FilePath) ;

	this->LoadConfigFile("..\\SCFData\\EventsConfig\\SCF_Doppelganger.dat");

	if(this->Enabled == 0)
	{
		this->SetState(DG_STATE_NONE);
	}
	else
	{
		this->SetState(DG_STATE_OPEN);
	}
}

void CDoppelGanger::SetState(int iState)
{
	switch(iState)
	{
		case DG_STATE_NONE:
			return;
		case DG_STATE_OPEN:
			this->SetState_OPEN();
			break;
		case DG_STATE_WAITING:
			this->SetState_WAITING();
			break;
		case DG_STATE_STARTED:
			this->SetState_STARTED();
			break;
		case DG_STATE_ENDED:
			this->SetState_ENDED();
			break;
	}
}

void CDoppelGanger::SetState_OPEN()
{
	for(int i=OBJ_STARTUSERINDEX;i<OBJMAX;++i)
	{
		if(DG_MAP_RANGE(gObj[i].MapNumber) && gObj[i].Connected == PLAYER_PLAYING)
		{
			gObjMoveGate(i, 267);
		}
	}
	for(int i=0;i<OBJ_MAXMONSTER;++i)
	{
		if(gObjIsConnected(i) && DG_MAP_RANGE(gObj[i].MapNumber))
			gObjDel(i);
	}
	this->m_DGData.Clear();
	this->m_DGData.btPlayState = DG_STATE_OPEN;
	this->m_DGData.IceWalkerLive = 0;
	LogAddC(8, "[DoppelGanger] State(%d) -> OPEN",this->m_DGData.btPlayState);
}

BYTE CDoppelGanger::GetState()
{
	return this->m_DGData.btPlayState;
}

void CDoppelGanger::SetState_WAITING()
{
	this->m_DGData.eDuration = 660000; // 11 minutes (10 for event, 1 for cooldown)
	this->m_DGData.TickCount = GetTickCount();
	this->m_DGData.btPlayState = DG_STATE_WAITING;
	LogAddC(9, "[DoppelGanger] State(%d) -> WAITING",this->m_DGData.btPlayState);
}

void CDoppelGanger::SetState_STARTED()
{
	this->m_DGData.eDuration = 600000; // for certainly..
	this->m_DGData.HalfTimeInfoSent = false; // for Ice Walker msg
	this->m_DGData.CreateMonsterDelay = 2; // first monster create
	this->m_DGData.MonsterPassed = 0; // set monster passed portal to 0
	if(this->SetEventLevel() == false)
	{
		this->SetState(DG_STATE_ENDED);
		this->m_DGData.eDuration = 0;
		return;
	}
	this->SetMapAttr(false, NULL); // unblock portal zone exit
	this->SetEventInterface(); // activate interface
	this->SetMonsterCountOnInterface();
	this->m_DGData.TickCount = GetTickCount();
	this->m_DGData.btPlayState = DG_STATE_STARTED;

	LogAddC(10, "[DoppelGanger] (Map:%d) State(%d) -> StartEvent PlayerCount: %d", this->m_DGData.MapNumber, this->m_DGData.btPlayState, this->m_DGData.PlayerCount);
}

void CDoppelGanger::SetState_ENDED()
{
	this->m_DGData.eDuration = 30 * 1000;
	this->DeleteMonster();
	this->m_DGData.TickCount = GetTickCount();
	this->m_DGData.btPlayState = DG_STATE_ENDED;
	LogAddC(10, "[DoppelGanger] SetState(%d) -> END", this->m_DGData.btPlayState);
}

void CDoppelGanger::Run()
{
	switch ( this->m_DGData.btPlayState )
	{
		case DG_STATE_NONE:
			return;
		case DG_STATE_OPEN:
			this->ProcState_OPEN();
			break;
		case DG_STATE_WAITING:
			this->ProcState_WAITING();
			break;
		case DG_STATE_STARTED:
			this->ProcState_STARTED();
			break;
		case DG_STATE_ENDED:
			this->ProcState_ENDED();
			break;
	}
}

void CDoppelGanger::ProcState_OPEN()
{
	if(this->m_DGData.PlayerCount > 0)
	{
		this->SetState(DG_STATE_WAITING);
	}
}

void CDoppelGanger::ProcState_WAITING()
{
	int Tick = GetTickCount() - this->m_DGData.TickCount;

	if(Tick >= 1000)
	{
		this->m_DGData.eDuration -= Tick;
		this->m_DGData.TickCount = GetTickCount();

		this->CheckUsers();

		if(this->m_DGData.PlayerCount <= 0)
		{
			LogAddC(10, "[DoppelGanger] State -> FAIL: NO PLAYERS INSIDE");
			this->SetState(DG_STATE_ENDED);
			this->m_DGData.eDuration = 0;
			return;
		}


		if(this->m_DGData.eDuration <= 630000 && this->m_DGData.HalfTimeInfoSent == false)
		{
			this->SendHalfTimeInfo(17);
		}

		if(this->m_DGData.eDuration <= 600000)
		{
			this->SetState(DG_STATE_STARTED);
		}
	}
}

void CDoppelGanger::ProcState_STARTED()
{
	int Tick = GetTickCount() - this->m_DGData.TickCount;

	if(Tick >= 1000)
	{
		this->m_DGData.eDuration -= Tick;
		this->m_DGData.TickCount = GetTickCount();
		++this->m_DGData.CreateMonsterDelay;
		if(this->m_DGData.CreateMonsterDelay == 3)
		{
			this->CreateMonster();
			this->m_DGData.CreateMonsterDelay = 0;
		}

		if(this->m_DGData.PlayerCount <= 0)
		{
			LogAddC(10, "[DoppelGanger] State -> FAIL: NO PLAYERS INSIDE");
			this->SetState(DG_STATE_ENDED);
			this->m_DGData.eDuration = 0;
			return;
		}

		this->SendPlayerPos();
		this->SendMonsterPos();
		this->CheckPosition();
		this->CheckUsers();
		if(this->m_DGData.eDuration < 480000 && this->m_DGData.PhazesPassed == 0)
		{
			this->InvokeSlaughterer();
		}

		if(this->m_DGData.eDuration < 360000 && this->m_DGData.PhazesPassed == 1)
		{
			this->InvokeSlaughterer();
		}

		if(this->m_DGData.eDuration < 300000 && this->m_DGData.PhazesPassed == 2)
		{
			this->InvokeFuriousSlaughterer();
		}

		if(this->m_DGData.eDuration < 210000 && this->m_DGData.PhazesPassed == 3)
		{
			this->InvokeIceWalker();
		}

		if(this->m_DGData.PhazesPassed == 4 && gObjIsConnected(this->m_DGData.IceWalkerIndex))
		{
			this->SendIceWalkerPos();
			if(gObj[this->m_DGData.IceWalkerIndex].Live == true)
			{
				--this->m_DGData.IceWalkerPhazeTime;
				if(this->m_DGData.IceWalkerPhazeTime <= 30 && this->m_DGData.HalfTimeInfoSent == false)
				{
					this->SendHalfTimeInfo(18);
					this->m_DGData.HalfTimeInfoSent = true;
				}
				if(this->m_DGData.IceWalkerPhazeTime <= 0)
				{
					LogAddC(9, "[DoppelGanger] State -> Ice Walker has retreated.");
					this->MsgStringSend("-Ice Walker has retreated.-",0);
					gObjDel(this->m_DGData.IceWalkerIndex);
					this->m_DGData.IceWalkerIndex = -1;
					this->m_DGData.IceWalkerLive = 0;
				}
			}
		}

		if(this->m_DGData.eDuration < 150000 && this->m_DGData.PhazesPassed == 4)
		{
			this->InvokeFuriousSlaughterer();
		}

		if(this->m_DGData.MonsterPassed >= this->GOER_MAX_PASS_PORTAL)
		{
			this->SendRanking(0);
			this->SendResultMessage(DG_RESULT_DEFENSE_FAIL, true, -1);
			this->SetState(DG_STATE_ENDED);
			LogAddC(10,"[DoppelGanger] State(%d) -> FAIL: MORE THAN THREE MONSTERS PASS THE PORTAL", this->m_DGData.btPlayState);
			this->MsgStringSend("Doppelganger will close in 30 seconds.",1);
		}

		if(this->m_DGData.eDuration <= 0)
		{
			this->SendRanking(1);
			this->SendResultMessage(DG_RESULT_SUCCESS, true, -1);
			this->SetState(DG_STATE_ENDED);
			this->InvokeFinalChest();
			LogAddC(10,"[DoppelGanger] State(%d) -> SUCCESS PASSED MONSTERS: (%d/%d)", this->m_DGData.btPlayState, this->m_DGData.MonsterPassed, this->GOER_MAX_PASS_PORTAL);
			this->MsgStringSend("Doppelganger will close in 30 seconds.",1);
		}
	}
}



void CDoppelGanger::SendRanking(BYTE State)
{
	if(this->SaveInDB == 1)
	{
		if(this->m_DGData.btPlayState != DG_STATE_ENDED)
		{
			for(int i=0;i<5;++i)
			{
				if(this->m_DGData.PlayerIndex[i] != -1)
				{
					if(gObjIsConnectedGP(this->m_DGData.PlayerIndex[i]))
					{
						if(gObj[this->m_DGData.PlayerIndex[i]].MapNumber == this->m_DGData.MapNumber)
						{
							PMSG_ANS_DGSCORE pMsg = {0};
							PHeadSetB((LPBYTE)&pMsg, SCFExDB_GSSend_DGScore, sizeof(pMsg));

							memcpy(pMsg.AccountID, gObj[this->m_DGData.PlayerIndex[i]].AccountID, MAX_ACCOUNT_LEN);
							memcpy(pMsg.Name, gObj[this->m_DGData.PlayerIndex[i]].Name, MAX_ACCOUNT_LEN);
							pMsg.ServerCode = gGameServerCode;
							pMsg.Result = State;
							pMsg.MobPassed = this->m_DGData.MonsterPassed;
							pMsg.MobToPass = this->GOER_MAX_PASS_PORTAL;

							wsExServerCli.DataSend((char*)&pMsg, pMsg.h.size);
						}
					}
				}
			}
		}
	}
}

void CDoppelGanger::ProcState_ENDED()
{
	int Tick = GetTickCount() - this->m_DGData.TickCount;

	if(Tick >= 1000)
	{
		this->m_DGData.eDuration -= Tick;
		this->m_DGData.TickCount = GetTickCount();

		if(this->m_DGData.eDuration <= 0)
		{
			if(this->m_DGData.FinalBoxIndex != -1)
			{
				gObjDel(this->m_DGData.FinalBoxIndex);
				this->m_DGData.FinalBoxIndex = -1;
			}
			this->SetState(DG_STATE_OPEN);
		}
	}
}

BYTE CDoppelGanger::CheckOverlapGoerMark(int iIndex)
{
	for ( int x = 0;x<ReadConfig.MAIN_INVENTORY_SIZE(iIndex,false);x++ )
	{
		if ( gObj[iIndex].pInventory[x].IsItem() == TRUE )
		{
			if ( gObj[iIndex].pInventory[x].m_Type == ITEMGET(14,110) )
			{
				int iITEM_DUR = gObj[iIndex].pInventory[x].m_Durability;

				if ( DG_MARK_RANGE(iITEM_DUR) != FALSE )
				{
					return x;
				}
			}
		}
	}

	return -1;
}


int CDoppelGanger::GetEnterItemPosition(int iIndex)
{
	if ( OBJMAX_RANGE(iIndex) == FALSE )
	{
		return -1;
	}

	if ( gObj[iIndex].Type != OBJ_USER || gObj[iIndex].Connected <= PLAYER_LOGGED )
	{
		return -1;
	}

	for ( int x=0;x<ReadConfig.MAIN_INVENTORY_SIZE(iIndex,false);x++)
	{
		if ( gObj[iIndex].pInventory[x].IsItem() == TRUE )
		{
			if ( gObj[iIndex].pInventory[x].m_Type == ITEMGET(14,111) ) //Double Goer Tiket
			{
				return x;
			}
			if ( gObj[iIndex].pInventory[x].m_Type == ITEMGET(13,125) ) //Double Goer Free Tiket
			{
				return x;
			}
		}
	}

	return -1;
}

void CDoppelGanger::EnterEvent(LPOBJ lpObj)
{
	if ( OBJMAX_RANGE(lpObj->m_Index) == FALSE )
		return;

	if(this->Enabled == 0)
	{
		TNotice::GCServerMsgStringSend("Event Dissabled!",lpObj->m_Index, 0x01);
		return;
	}

	if (lpObj->m_IfState.type != 99)
	{
		if (lpObj->m_IfState.type != 0 || lpObj->m_IfState.state != 0)
		{
			ANTI_HACK_LOG.Output("[ANTI-HACK][DoppelGanger][Enter] User in map(%d) [%s][%s] State(u:%d,t:%d,s:%d)",
				lpObj->MapNumber, lpObj->AccountID, lpObj->Name,
				lpObj->m_IfState.use,lpObj->m_IfState.type,lpObj->m_IfState.state);

			return;
		}
	}

	lpObj->m_IfState.state = 0;
	lpObj->m_IfState.use = 0;
	lpObj->m_IfState.type = 0;

	if(this->m_DGData.btPlayState != DG_STATE_OPEN && this->m_DGData.btPlayState != DG_STATE_WAITING)
	{
		TNotice::GCServerMsgStringSend("You cant go inside right now!",lpObj->m_Index, 0x01);
		return;
	}

	int bItemPosition = this->GetEnterItemPosition(lpObj->m_Index);

	if(bItemPosition == -1)
	{
		TNotice::GCServerMsgStringSend("Lacking ticket to enter!",lpObj->m_Index, 0x01);
		return;
	}

	if(this->m_DGData.btPlayState == DG_STATE_WAITING)
	{
		if(this->m_DGData.PartyNumber != lpObj->PartyNumber)
		{
			TNotice::GCServerMsgStringSend("You cant go inside right now!",lpObj->m_Index, 0x01);
			return;
		}
	}

	if(this->AddPlayer(lpObj) == true)
	{
		LogAddTD("[DoppelGanger] (Account:%s, Name:%s) Delete Entrance Tiket (%d) [Serial:%d]",
			lpObj->AccountID, lpObj->Name, 
			lpObj->pInventory[bItemPosition].m_Type, lpObj->pInventory[bItemPosition].m_Number);

		if ((lpObj->pInventory[bItemPosition].m_Durability > 1.0f && lpObj->pInventory[bItemPosition].m_Type == ITEMGET(13,125)) )
		{
			lpObj->pInventory[bItemPosition].m_Durability -= 1.0f;
			GCItemDurSend2(lpObj->m_Index, bItemPosition, lpObj->pInventory[bItemPosition].m_Durability, 0);
		} else 
		{
			gObjInventoryDeleteItem(lpObj->m_Index, bItemPosition);
			GCInventoryItemDeleteSend(lpObj->m_Index, bItemPosition, TRUE);
		}

		int iGate = this->SetMapNumber();

		for(int i=0;i<5;++i)
		{
			if(this->m_DGData.PlayerIndex[i] != -1)
			{
				TNotice::GCServerMsgStringSend("Entering Doppelganger Event!",this->m_DGData.PlayerIndex[i], 0x01);

				gObjMoveGate(this->m_DGData.PlayerIndex[i], iGate);
	
				this->SetMapAttr(true, &gObj[this->m_DGData.PlayerIndex[i]]);

				LogAddC(9,"[DoppelGanger] [%s][%s] Entered to Doppelganger", gObj[this->m_DGData.PlayerIndex[i]].AccountID, gObj[this->m_DGData.PlayerIndex[i]].Name);
			}
		}
	}
}

bool CDoppelGanger::AddPlayer(LPOBJ lpObj)
{
	this->m_DGData.PartyNumber = lpObj->PartyNumber;
	
	if(gParty.IsParty(this->m_DGData.PartyNumber) == TRUE)
	{
		for(int i = 0; i < MAX_USER_IN_PARTY; i++)
		{
			int pId = gParty.m_PartyS[this->m_DGData.PartyNumber].Number[i];

			if ( pId < 0 || pId > (OBJMAX-1) )
			{
				
			}else
			{
				if(gObjIsConnected(pId) == TRUE)
				{
					this->m_DGData.PlayerIndex[i] = pId;
					++this->m_DGData.PlayerCount;
				}
			}
		}
		return true;
	}else
	{
		for(int i=0;i<5;++i)
		{
			if(this->m_DGData.PlayerIndex[i] == -1)
			{
				this->m_DGData.PlayerIndex[i] = lpObj->m_Index;
				if(i == 0) // first player, set party number, that other teammates can enter to event
				{
					this->m_DGData.PartyNumber = lpObj->PartyNumber;
				}
				++this->m_DGData.PlayerCount;
				//LogAddC(9,"[DoppelGanger] (%s)(%s) Add to event, current player count: %d", lpObj->AccountID, lpObj->Name, this->m_DGData.PlayerCount);
				return true;
			}
		}
	}
	return false;
}

int CDoppelGanger::GetEventTime(LPOBJ lpObj)
{
	if((lpObj->PartyNumber == this->m_DGData.PartyNumber && this->m_DGData.PartyNumber != -1) && this->m_DGData.btPlayState == DG_STATE_WAITING)
	{
		return 0;
	}
	else if(this->m_DGData.btPlayState == DG_STATE_OPEN)
	{
		return 0;
	}
	return this->m_DGData.eDuration;
}

void CDoppelGanger::SetMapAttr(bool Block, LPOBJ lpObj)
{
	char cTEMP_BUF[256];
	int lOfs = 0;
	PMSG_SETMAPATTR_COUNT pMsg;
	PMSG_SETMAPATTR pBodyMsg;
	lOfs += sizeof(pMsg);

	pMsg.btType = 0;
	pMsg.btCount = 1;
	pMsg.btMapAttr = 4;
	pMsg.btMapSetType = (Block == true) ? 4 : 1;

	pBodyMsg.btX = g_DGEntrance[this->m_DGData.MapNumber-65].iStartX;
	pBodyMsg.btY = g_DGEntrance[this->m_DGData.MapNumber-65].iStartY;
	memcpy(&cTEMP_BUF[lOfs], &pBodyMsg, sizeof(pBodyMsg));
	lOfs += sizeof(pBodyMsg);
	pBodyMsg.btX   = g_DGEntrance[this->m_DGData.MapNumber-65].iEndX;
	pBodyMsg.btY   = g_DGEntrance[this->m_DGData.MapNumber-65].iEndY;
	memcpy(&cTEMP_BUF[lOfs], &pBodyMsg, sizeof(pBodyMsg));
	lOfs += sizeof(pBodyMsg);

	pMsg.h.c = 0xC1;
	pMsg.h.size = lOfs;
	pMsg.h.headcode = 0x46;
	memcpy(&cTEMP_BUF, &pMsg, sizeof(pMsg));
	
	if(Block)
	{
		DataSend(lpObj->m_Index, (LPBYTE)&cTEMP_BUF, lOfs);
	}
	else
	{
		this->SendDoppelGangerPlayer((LPBYTE)&cTEMP_BUF, lOfs);
	}
}

void CDoppelGanger::SendDoppelGangerPlayer(LPBYTE lpMsg, int iSize)
{
	for(int i=0;i<5;++i)
	{
		if(this->m_DGData.PlayerIndex[i] != -1 && gObjIsConnectedGP(this->m_DGData.PlayerIndex[i]) && gObj[this->m_DGData.PlayerIndex[i]].MapNumber == this->m_DGData.MapNumber)
		{
			DataSend(this->m_DGData.PlayerIndex[i], lpMsg, iSize);
		}
	}
}

void CDoppelGanger::SendHalfTimeInfo(int Type)
{
	PMSG_SET_DEVILSQUARE pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x92, sizeof(pMsg));
	pMsg.Type = Type;

	this->SendDoppelGangerPlayer((LPBYTE)&pMsg, pMsg.h.size);
	this->m_DGData.HalfTimeInfoSent = true;
}

void CDoppelGanger::SetEventInterface()
{
	PMSG_DGOER_TIMER_INIT pMsg;

	PHeadSubSetB((LPBYTE)&pMsg, 0xBF, 0x10, sizeof(pMsg));
	pMsg.Timer = 2;

	this->SendDoppelGangerPlayer((LPBYTE)&pMsg, pMsg.h.size);
}

void CDoppelGanger::SetMonsterCountOnInterface()
{
	PMSG_DGOER_MONSTER_COUNT pMsg;

	PHeadSubSetB((LPBYTE)&pMsg, 0xBF, 0x14, sizeof(pMsg));
	pMsg.Killed = this->m_DGData.MonsterPassed;
	pMsg.Total = this->GOER_MAX_PASS_PORTAL;

	this->SendDoppelGangerPlayer((LPBYTE)&pMsg, pMsg.h.size);
}

void CDoppelGanger::SendPlayerPos()
{
	PMSG_DGOER_UPDATE_BAR pMsg;
	PLAYER_BAR_INFO pPlayer;

	int Time = this->m_DGData.eDuration / 1000;
	char cBUFF[256];
	int lOfs = 8;

	for(int i=0;i<5;++i)
	{
		memset(&pPlayer, 0x00, sizeof(pPlayer));
		if(gObjIsConnectedGP(this->m_DGData.PlayerIndex[i]) == true)
		{
			pPlayer.UserH = HIBYTE(this->m_DGData.PlayerIndex[i]);
			pPlayer.UserL = LOBYTE(this->m_DGData.PlayerIndex[i]);
			pPlayer.Visible = 1; // visible on bar
			pPlayer.Pos = this->CalcPos(i, false);
		}
		memcpy(&cBUFF[lOfs], &pPlayer, sizeof(pPlayer));
		lOfs += sizeof(pPlayer);
	}

	pMsg.UserCount = this->m_DGData.PlayerCount;
	pMsg.Unk = 0;
	pMsg.TimerH = HIBYTE(Time);
	pMsg.TimerL = LOBYTE(Time);

	PHeadSubSetB((LPBYTE)&pMsg, 0xBF, 0x12, lOfs);
	memcpy(&cBUFF, &pMsg, sizeof(pMsg));

	this->SendDoppelGangerPlayer((LPBYTE)&cBUFF, lOfs);
}

void CDoppelGanger::SendMonsterPos()
{
	PMSG_DGOER_TRIANG_POS pMsg;

	int nearest = 0, temp = -1;
	
	PHeadSubSetB((LPBYTE)&pMsg, 0xBF, 0x0F, sizeof(pMsg));

	for(int i=0;i<OBJ_MAXMONSTER;++i)
	{
		if(this->IsDGMonster(&gObj[i]) && this->m_DGData.MapNumber == gObj[i].MapNumber && gObj[i].Live == true && gObj[i].Connected == PLAYER_PLAYING)
		{
			temp = this->CalcPos(i, true);
		}
		if(temp > nearest)
		{
			nearest = temp;
		}
	}
	if(nearest == -1)
		nearest = 0;

	pMsg.Pos = nearest;
	this->SendDoppelGangerPlayer((LPBYTE)&pMsg, pMsg.h.size);
}

void CDoppelGanger::SendIceWalkerPos()
{
	PMSG_DGOER_RTRIANG_POS pMsg;

	PHeadSubSetB((LPBYTE)&pMsg, 0xBF, 0x11, sizeof(pMsg));

	pMsg.Unk = (gObj[this->m_DGData.IceWalkerIndex].Live==true) ? 0 : 1;
	pMsg.Pos = this->CalcPos(this->m_DGData.IceWalkerIndex, true);

	this->SendDoppelGangerPlayer((LPBYTE)&pMsg, pMsg.h.size);
}


int CDoppelGanger::SetMapNumber()
{
	if(this->m_DGData.MapNumber != -1)
	{
		return (264+this->m_DGData.MapNumber);
	}
	else
	{
		//srand((unsigned)time(NULL));
		this->m_DGData.MapNumber = MAP_INDEX_DOUBLE_GOER1 + (rand()%4);
		return (264+this->m_DGData.MapNumber);
	}
}

void CDoppelGanger::CreateMonster()
{
	int Num = (40 * this->m_DGData.PlayerCount);
	if(this->m_DGData.IceWalkerLive == 1 && this->m_DGData.PhazesPassed == 5)
		Num = (60 * this->m_DGData.PlayerCount);

	if(this->m_DGData.MonsterCount < Num)
	{
		for(int i=0;i<Num;++i)
		{
			if(this->m_DGData.MonsterIndex[i] == -1)
			{
				this->m_DGData.MonsterIndex[i] = gObjAddMonster(this->m_DGData.MapNumber);
				if(this->m_DGData.MonsterIndex[i] >= 0)
				{
					gObj[this->m_DGData.MonsterIndex[i]].MapNumber = this->m_DGData.MapNumber;
					gObj[this->m_DGData.MonsterIndex[i]].X = this->MPOSX[this->m_DGData.MapNumber-65];
					gObj[this->m_DGData.MonsterIndex[i]].Y = this->MPOSY[this->m_DGData.MapNumber-65];
					gObjSetMonster(this->m_DGData.MonsterIndex[i], (rand()%6+533),"CDoppelGanger::CreateMonster");
					++this->m_DGData.MonsterCount;
					LogAddC(9,"[DoppelGanger] Spawned Monster(%d): Index: %d", i, this->m_DGData.MonsterIndex[i]);
					return;
				}
			}
		}
	}
}

void CDoppelGanger::MoveProc(LPOBJ lpObj)
{
	if(this->Enabled == 0)
		return;
	
	if(DG_MAP_RANGE(lpObj->MapNumber) == FALSE)
		return;

	if(this->IsDGMonster(lpObj) == true)
	{
		lpObj->MTX = this->POSX[this->m_DGData.MapNumber-MAP_INDEX_DOUBLE_GOER1];
		lpObj->MTY = this->POSY[this->m_DGData.MapNumber-MAP_INDEX_DOUBLE_GOER1];
		if(lpObj->PathCur >= lpObj->PathCount)
		{
			if(DG_UTIL.FindPathToMoveMonster(lpObj, this->POSX[this->m_DGData.MapNumber-MAP_INDEX_DOUBLE_GOER1], this->POSY[this->m_DGData.MapNumber-MAP_INDEX_DOUBLE_GOER1], 40, 2) == FALSE)
				DG_UTIL.FindPathToMoveMonster(lpObj, this->POSX[this->m_DGData.MapNumber-MAP_INDEX_DOUBLE_GOER1], this->POSY[this->m_DGData.MapNumber-MAP_INDEX_DOUBLE_GOER1], 40, 0);
		}
	}
}

bool CDoppelGanger::IsDGMonster(LPOBJ lpObj)
{
	if(this->Enabled == 0)
		return false;
	
	if(DG_MAP_RANGE(lpObj->MapNumber) == FALSE)
		return false;

	for(int i=0;i<DG_MAX_MONSTERS;++i)
	{
		if(this->m_DGData.MonsterIndex[i] == lpObj->m_Index)
		{
			return true;
		}
	}
	if(this->m_DGData.ButcherIndex == lpObj->m_Index) return true;
	if(this->m_DGData.AngryButcherIndex == lpObj->m_Index) return true;
	if(this->m_DGData.IceWalkerIndex == lpObj->m_Index) return true;
	if(this->m_DGData.FinalBoxIndex == lpObj->m_Index) return true;

	for(int n=0;n<3;++n)
	{
		if(this->m_DGData.SilverBoxIndex[n] == lpObj->m_Index)
		{
			return true;
		}
	}
	return false;
}

void CDoppelGanger::RegenProc(LPOBJ lpObj)
{
	if(this->Enabled == 0)
		return;

	if(this->IsDGMonster(lpObj) == true)
	{
		lpObj->RegenMapX = this->MPOSX[this->m_DGData.MapNumber-MAP_INDEX_DOUBLE_GOER1];
		lpObj->RegenMapY = this->MPOSY[this->m_DGData.MapNumber-MAP_INDEX_DOUBLE_GOER1];
		lpObj->X = this->MPOSX[this->m_DGData.MapNumber-MAP_INDEX_DOUBLE_GOER1];
		lpObj->Y = this->MPOSY[this->m_DGData.MapNumber-MAP_INDEX_DOUBLE_GOER1];
	}

	if(this->m_DGData.ButcherIndex == lpObj->m_Index) gObjDel(lpObj->m_Index);
	if(this->m_DGData.AngryButcherIndex == lpObj->m_Index) gObjDel(lpObj->m_Index);
	if(this->m_DGData.IceWalkerIndex == lpObj->m_Index) gObjDel(lpObj->m_Index);
	if(this->m_DGData.FinalBoxIndex == lpObj->m_Index) 
	{
		this->m_DGData.FinalBoxIndex = -1;
		gObjDel(lpObj->m_Index);
	}

	for(int i=0;i<3;++i)
	{
		if(this->m_DGData.SilverBoxIndex[i] == lpObj->m_Index)
		{
			this->m_DGData.SilverBoxIndex[i] = -1;
			gObjDel(lpObj->m_Index);
		}
	}
}

int CDoppelGanger::CalcPos(int player, bool MonsterCalc)
{
	if(MonsterCalc == false)
	{
		if(this->m_DGData.MapNumber == MAP_INDEX_DOUBLE_GOER1)
		{
			if(gObj[this->m_DGData.PlayerIndex[player]].Y>31)
			{
				return 22-((gObj[this->m_DGData.PlayerIndex[player]].Y-31)/3.2);
			}
			else
			{
				return 22;
			}
		}
		else if(this->m_DGData.MapNumber == MAP_INDEX_DOUBLE_GOER2)
		{
			if(gObj[this->m_DGData.PlayerIndex[player]].Y>69)
			{
				return 22-(((gObj[this->m_DGData.PlayerIndex[player]].Y -70)*2)/10);
			}
			else
			{
				return 22;
			}
		}
		else if(this->m_DGData.MapNumber == MAP_INDEX_DOUBLE_GOER3)
		{
			if(gObj[this->m_DGData.PlayerIndex[player]].Y>61)
			{
				return 22-((gObj[this->m_DGData.PlayerIndex[player]].Y-61)/4.1);
			}
			else
			{
				return 22;
			}
		}
		else if(this->m_DGData.MapNumber == MAP_INDEX_DOUBLE_GOER4)
		{
			int Result = 0;
			int TX = 0;
			int TY = 0;
			int T = 0;

			int pX = gObj[this->m_DGData.PlayerIndex[player]].X;
			int pY = gObj[this->m_DGData.PlayerIndex[player]].Y;

			int iX = this->POSX[3];
			int iY = this->POSY[3];

			if(iX >= pX)
				TX = iX - pX;
			else
				TX = pX - iX;

			if(iY >= pY)
				TY = iY - pY;
			else
				TY = pY - iY;

			T = (TX + TY);

			Result = (((100 - T) * 22) / 100);
					
			if(Result > 22)
				Result = 22;
			else if(Result < 0)
				Result = 0;

			return Result;
		}
	}
	else
	{
		if(this->m_DGData.MapNumber == MAP_INDEX_DOUBLE_GOER1)
		{
			if(gObj[player].Y>31)
			{
				return 22-((gObj[player].Y-31)/3.2);
			}
			else
			{
				return 22;
			}
		}
		else if(this->m_DGData.MapNumber == MAP_INDEX_DOUBLE_GOER2)
		{
			if(gObj[player].Y>69)
			{
				return 22-(((gObj[player].Y -70)*2)/10);
			}
			else
			{
				return 22;
			}
		}
		else if(this->m_DGData.MapNumber == MAP_INDEX_DOUBLE_GOER3)
		{
			if(gObj[player].Y>61)
			{
				return 22-((gObj[player].Y-61)/4.1);
			}
			else
			{
				return 22;
			}
		}
		else if(this->m_DGData.MapNumber == MAP_INDEX_DOUBLE_GOER4)
		{
			int Result = 0;
			int TX = 0;
			int TY = 0;
			int T = 0;

			int pX = gObj[player].X;
			int pY = gObj[player].Y;

			int iX = this->POSX[3];
			int iY = this->POSY[3];

			if(iX >= pX)
				TX = iX - pX;
			else
				TX = pX - iX;

			if(iY >= pY)
				TY = iY - pY;
			else
				TY = pY - iY;

			T = (TX + TY);

			Result = (((100 - T) * 22) / 100);
					
			if(Result > 22)
				Result = 22;
			else if(Result < 0)
				Result = 0;

			return Result;
		}
	}
	return 22;
}

void CDoppelGanger::CheckPosition()
{
	for(int i=0;i<DG_MAX_MONSTERS;++i)
	{
		if(gObj[this->m_DGData.MonsterIndex[i]].X > g_DGPortal[this->m_DGData.MapNumber-MAP_INDEX_DOUBLE_GOER1].StartX - 3 &&
			gObj[this->m_DGData.MonsterIndex[i]].X < g_DGPortal[this->m_DGData.MapNumber-MAP_INDEX_DOUBLE_GOER1].StartX +3 )
		{
			if(gObj[this->m_DGData.MonsterIndex[i]].Y > g_DGPortal[this->m_DGData.MapNumber-MAP_INDEX_DOUBLE_GOER1].StartY - 5 &&
				gObj[this->m_DGData.MonsterIndex[i]].Y < g_DGPortal[this->m_DGData.MapNumber-MAP_INDEX_DOUBLE_GOER1].StartY +5 )
			{
				GCSkillInfoSend(&gObj[this->m_DGData.MonsterIndex[i]],1,105);
				gObjDel(this->m_DGData.MonsterIndex[i]);
				this->m_DGData.MonsterIndex[i] = -1;
				--this->m_DGData.MonsterCount;
				++this->m_DGData.MonsterPassed;
				this->SetMonsterCountOnInterface();
				LogAddC(8,"[DoppelGanger] Pass Monsters INC: (%d/%d)", this->m_DGData.MonsterPassed, this->GOER_MAX_PASS_PORTAL);
			}
		}
	}
}

void CDoppelGanger::SendResultMessage(BYTE Result, bool SendForAll, int aIndex)
{
	PMSG_DGOER_END pResult;

	PHeadSubSetB((LPBYTE)&pResult, 0xBF, 0x13, sizeof(pResult));

	pResult.Status = Result;

	if(SendForAll)
	{
		this->SendDoppelGangerPlayer((LPBYTE)&pResult, pResult.h.size);
	}
	else
	{
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
	}
}

void CDoppelGanger::DeleteMonster()
{
	for(int i=0;i<DG_MAX_MONSTERS;++i)
	{
		if(this->m_DGData.MonsterIndex[i] != -1)
		{
			gObjDel(this->m_DGData.MonsterIndex[i]);
			this->m_DGData.MonsterIndex[i] = -1;
			--this->m_DGData.MonsterCount;
		}
	}
	if(this->m_DGData.ButcherIndex >= 0)
		gObjDel(this->m_DGData.ButcherIndex);
	if(this->m_DGData.AngryButcherIndex >= 0)
		gObjDel(this->m_DGData.AngryButcherIndex);
	if(this->m_DGData.IceWalkerIndex >= 0)
		gObjDel(this->m_DGData.IceWalkerIndex);

	this->m_DGData.SilverBoxIndex[0] = -1;
	this->m_DGData.SilverBoxIndex[1] = -1;
	this->m_DGData.SilverBoxIndex[2] = -1;

	for(int i=0;i<OBJ_MAXMONSTER;++i)
	{
		if(DG_MAP_RANGE(gObj[i].MapNumber))
			if(gObj[i].Class == 541 || gObj[i].Class == 532)
				gObjDel(i);
	}
}

void CDoppelGanger::InvokeSlaughterer()
{
	if(this->m_DGData.ButcherIndex >= 0) gObjDel(this->m_DGData.ButcherIndex);
	this->m_DGData.ButcherIndex = gObjAddMonster(this->m_DGData.MapNumber);
	if(this->m_DGData.ButcherIndex >= 0)
	{
		gObj[this->m_DGData.ButcherIndex].MapNumber = this->m_DGData.MapNumber;
		gObj[this->m_DGData.ButcherIndex].X = this->MPOSX[this->m_DGData.MapNumber-65];
		gObj[this->m_DGData.ButcherIndex].Y = this->MPOSY[this->m_DGData.MapNumber-65];
		gObj[this->m_DGData.ButcherIndex].m_MoveRange = 30;
		++this->m_DGData.PhazesPassed;
		gObjSetMonster(this->m_DGData.ButcherIndex, 530,"CDoppelGanger::InvokeSlaughterer");

		LogAddC(9,"[DoppelGanger] State -> SLAUGHTERER INVOKED");
	}
}

void CDoppelGanger::InvokeFuriousSlaughterer()
{
	if(this->m_DGData.AngryButcherIndex >= 0) gObjDel(this->m_DGData.AngryButcherIndex);
	this->m_DGData.AngryButcherIndex = gObjAddMonster(this->m_DGData.MapNumber);
	if(this->m_DGData.AngryButcherIndex >= 0)
	{
		gObj[this->m_DGData.AngryButcherIndex].MapNumber = this->m_DGData.MapNumber;
		gObj[this->m_DGData.AngryButcherIndex].X = this->MPOSX[this->m_DGData.MapNumber-65];
		gObj[this->m_DGData.AngryButcherIndex].Y = this->MPOSY[this->m_DGData.MapNumber-65];
		gObj[this->m_DGData.AngryButcherIndex].m_MoveRange = 30;
		++this->m_DGData.PhazesPassed;
		gObjSetMonster(this->m_DGData.AngryButcherIndex, 529,"CDoppelGanger::InvokeFuriousSlaughterer");

		LogAddC(9,"[DoppelGanger] State -> FURIOUS SLAUGHTERER INVOKED");
	}
}

void CDoppelGanger::InvokeIceWalker()
{
	if(this->m_DGData.IceWalkerIndex >= 0) gObjDel(this->m_DGData.IceWalkerIndex);
	this->m_DGData.IceWalkerIndex = gObjAddMonster(this->m_DGData.MapNumber);
	if(this->m_DGData.IceWalkerIndex >= 0)
	{
		gObj[this->m_DGData.IceWalkerIndex].MapNumber = this->m_DGData.MapNumber;
		gObj[this->m_DGData.IceWalkerIndex].X = this->MPOSX[this->m_DGData.MapNumber-65];
		gObj[this->m_DGData.IceWalkerIndex].Y = this->MPOSY[this->m_DGData.MapNumber-65];
		gObj[this->m_DGData.IceWalkerIndex].m_MoveRange = 30;
		++this->m_DGData.PhazesPassed;
		this->m_DGData.IceWalkerPhazeTime = 90;
		gObjSetMonster(this->m_DGData.IceWalkerIndex, 531,"CDoppelGanger::InvokeIceWalker");
		this->m_DGData.IceWalkerLive = 1;
		this->MsgStringSend("-Ice Walker appeared.-",0);
		
		LogAddC(9,"[DoppelGanger] State -> ICE WALKER INVOKED");
	}
}

bool CDoppelGanger::InvokeInterimChest(LPOBJ lpObj, LPOBJ lpMon)
{
	if(this->Enabled == 0)
		return false;

	if(DG_MAP_RANGE(lpObj->MapNumber))
	{
		if(this->IsDGMonster(lpMon))
		{
			if(lpMon->Class == 529 && this->DropBagOnKillBoss[1] == 1)
			{
				DoubleGoerItemBoxOpen(lpMon, lpMon->MapNumber, lpMon->X, lpMon->Y, lpMon->Class);
			}
			if(lpMon->Class == 530 && this->DropBagOnKillBoss[0] == 1)
			{
				DoubleGoerItemBoxOpen(lpMon, lpMon->MapNumber, lpMon->X, lpMon->Y, lpMon->Class);
			}

			for(int i=0;i<3;++i)
			{
				this->m_DGData.SilverBoxIndex[i] = gObjAddMonster(this->m_DGData.MapNumber);
				gObj[this->m_DGData.SilverBoxIndex[i]].MapNumber = lpMon->MapNumber;

				short sX, sY;
				
				while ( gMSetBase.GetBoxPosition(gObj[this->m_DGData.SilverBoxIndex[i]].MapNumber, lpMon->X-(i+1), lpMon->Y-(i+2), lpMon->X+(i+2), lpMon->Y+(i+1), sX, sY) == 0 )
				{
				}

				gObj[this->m_DGData.SilverBoxIndex[i]].X = sX;
				gObj[this->m_DGData.SilverBoxIndex[i]].Y = sY;
				gObjSetMonster(this->m_DGData.SilverBoxIndex[i], 541,"CDoppelGanger::InvokeInterimChest");
			}
			return true;
		}
	}
	return false;
}

bool CDoppelGanger::KillIceWalker(LPOBJ lpObj, LPOBJ lpMon)
{
	if(this->Enabled == 0)
		return false;

	if(DG_MAP_RANGE(lpObj->MapNumber))
	{
		if(this->m_DGData.IceWalkerIndex == lpMon->m_Index)
		{
			g_DoppelGanger.MsgStringSend("-Mission completed.-",0);

			if(this->DropBagOnKillBoss[2] == 1)
			{
				int iMaxHitUser = gObjMonsterTopHitDamageUser(lpMon);

				LogAddC(9,"[Double Goer][Reward] Boss ItemDrop MaxHitUser [%s][%s]",
					lpObj->AccountID, lpObj->Name);

				BYTE cDropX = lpMon->X;
				BYTE cDropY = lpMon->Y;

				DoubleGoerItemBoxOpen(lpObj, lpMon->MapNumber, cDropX, cDropY, lpMon->Class);

				return true;
			}
		}
	}
	return false;
}

void CDoppelGanger::DeadUserProc(LPOBJ lpObj)
{
	unsigned char index = -1;

	for(int i=0;i<5;++i)
	{
		if(this->m_DGData.PlayerIndex[i] == lpObj->m_Index)
		{
			index = i;
			break;
		}
	}
	if(index == -1)
	{
		return;
	}

	this->m_DGData.PlayerIndex[index] = -1;
	--this->m_DGData.PlayerCount;

	this->SendResultMessage(DG_RESULT_DEAD_PLAYER, false, lpObj->m_Index);
}

void CDoppelGanger::InvokeLarva(LPOBJ lpObj)
{
	int index;
	for(int i=0;i<3;++i)
	{
		index = gObjAddMonster(lpObj->MapNumber);
		if(index >= 0)
		{
			gObj[index].MapNumber = lpObj->MapNumber;
			gObj[index].X = lpObj->X;
			gObj[index].Y = lpObj->Y;
			gObjSetMonster(index, 532,"CDoppelGanger::InvokeLarva");
			LogAddC(8,"[DoppelGanger] Spawn Larva Trap (%d) (%d/%d/%d)", index, gObj[index].MapNumber, gObj[index].X, gObj[index].Y);
		}
	}
}

void CDoppelGanger::InterimChestOpen(LPOBJ lpObj, LPOBJ lpChest)
{
	if(this->Enabled == 0)
		return;
	
	if(DG_MAP_RANGE(lpObj->MapNumber) == FALSE)
		return;

	//srand((unsigned)time(NULL));
	if(rand()%100 < this->InterimChestRate)
	{
		DoubleGoerItemBoxOpen(lpObj, lpChest->MapNumber, lpChest->X, lpChest->Y, lpChest->Class);
	}
	else
	{
		this->InvokeLarva(lpChest);
	}
	lpChest->Life = 0.0;
	lpChest->m_Attribute = 60;	//Mark Chest to be deleted
	gObjLifeCheck(lpChest, lpChest, 1, 0, 0, 0, 0, 0);

	for(int i=0;i<3;++i)
	{
		if(this->m_DGData.SilverBoxIndex[i] != lpChest->m_Index)
		{
			gObjDel(this->m_DGData.SilverBoxIndex[i]);
		}
	}
}

void CDoppelGanger::InvokeFinalChest()
{
	for(int i=0;i<5;++i)
	{
		if(this->m_DGData.PlayerIndex[i] != -1)
		{
			this->m_DGData.FinalBoxIndex = gObjAddMonster(this->m_DGData.MapNumber);
			if(this->m_DGData.FinalBoxIndex >= 0)
			{
				gObj[this->m_DGData.FinalBoxIndex].MapNumber = this->m_DGData.MapNumber;
				gObj[this->m_DGData.FinalBoxIndex].X = gObj[this->m_DGData.PlayerIndex[i]].X;
				gObj[this->m_DGData.FinalBoxIndex].Y = gObj[this->m_DGData.PlayerIndex[i]].Y;
				gObjSetMonster(this->m_DGData.FinalBoxIndex, 542,"CDoppelGanger::InvokeFinalChest");
				return;
			}
		}
	}
}

void CDoppelGanger::FinalChestOpen(LPOBJ lpObj, LPOBJ lpChest)
{
	if(this->Enabled == 0)
		return;
	
	if(DG_MAP_RANGE(lpObj->MapNumber) == FALSE)
		return;

	DoubleGoerItemBoxOpen(lpObj, lpChest->MapNumber, lpChest->X, lpChest->Y, lpChest->Class);
	lpChest->Life = 0.0;
	lpChest->m_Attribute = 60;	//Mark Chest to be deleted
	gObjLifeCheck(lpChest, lpChest, 1, 0, 0, 0, 0, 0);
	//gObjDel(lpChest->m_Index);
}

void CDoppelGanger::CheckUsers()
{
	for(int i=0;i<5;++i)
	{
		if(this->m_DGData.PlayerIndex[i] != -1)
		{
			if(gObj[this->m_DGData.PlayerIndex[i]].MapNumber != this->m_DGData.MapNumber || !gObjIsConnectedGP(this->m_DGData.PlayerIndex[i]))
			{
				this->SendResultMessage(DG_RESULT_DEAD_PLAYER, false, this->m_DGData.PlayerIndex[i]);
				this->m_DGData.PlayerIndex[i] = -1;
				--this->m_DGData.PlayerCount;
			}
		}
	}
}

void CDoppelGanger::LoadConfigFile(LPSTR FilePath)
{
	int Token;
	int n = 0;
	int type;

	this->m_DGMobStatCnt = 0;
	memset(this->m_DGMobStat,-1,sizeof(this->m_DGMobStat));

	SMDFile = fopen(FilePath, "r");

	if ( SMDFile == NULL )
	{
		MsgBox("[DoppelGanger] data load error %s", FilePath);
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

				this->m_DGMobStat[n].EventLevel = TokenNumber;

				Token = GetToken();
				this->m_DGMobStat[n].Class = TokenNumber;

				Token = GetToken();
				this->m_DGMobStat[n].Level = TokenNumber;

				Token = GetToken();
				this->m_DGMobStat[n].HP = TokenNumber;

				Token = GetToken();
				this->m_DGMobStat[n].DmgMin = TokenNumber;

				Token = GetToken();
				this->m_DGMobStat[n].DmgMax = TokenNumber;

				Token = GetToken();
				this->m_DGMobStat[n].Defense = TokenNumber;

				this->m_DGMobStatCnt++;

				n++;
				if(n == MAX_DG_STATS-1)
				{
					LogAddTD("[DoppelGanger][Overflow] - Max Mob Stat %d",MAX_DG_STATS);
					return;
				}
			}
		}
		break;
	}

	LogAddC(9,"[DoppelGanger] Loaded %d Monster stats",n);
}

bool CDoppelGanger::SetEventLevel()
{
	if(this->UseAutoStat == 0)
	{
		return true;
	}

	int temp = -1;
	int level = 0;

	for(int i=0;i<5;++i)
	{
		if(this->m_DGData.PlayerIndex[i] != -1)
		{
			level = (gObj[this->m_DGData.PlayerIndex[i]].Level + gObj[this->m_DGData.PlayerIndex[i]].MasterCharacterInfo->MasterLevel);
			
			if(level > temp)
			{
				temp = level;
			}
		}
	}

	if(temp == -1)
	{
		LogAddC(4, "[DoppelGanger] ERROR: Cant find Max Level!");
		return false;
	}

	this->m_DGData.EventLevel = temp/10;
	return true;
}

void CDoppelGanger::SetMonsterStats(LPOBJ lpObj)
{
	if(this->UseAutoStat == 0)
	{
		return;
	}

	short partyinc = 0;

	switch ( this->m_DGData.PlayerCount )
	{
		case 2:
			partyinc = this->PartyAutoStat[0];
			break;
		case 3:
			partyinc = this->PartyAutoStat[1];
			break;
		case 4:
			partyinc = this->PartyAutoStat[2];
			break;
		case 5:
			partyinc = this->PartyAutoStat[3];
			break;
	}

	for(int i=0;i<this->m_DGMobStatCnt;i++)
	{
		if(this->m_DGMobStat[i].EventLevel == this->m_DGData.EventLevel && 
			this->m_DGMobStat[i].Class == lpObj->Class)
		{
			lpObj->Level = this->m_DGMobStat[i].Level;
			lpObj->Life = this->m_DGMobStat[i].HP;
			lpObj->MaxLife = this->m_DGMobStat[i].HP;
			lpObj->m_Defense = this->m_DGMobStat[i].Defense;
			lpObj->m_AttackDamageMin = this->m_DGMobStat[i].DmgMin;
			lpObj->m_AttackDamageMax = this->m_DGMobStat[i].DmgMax;
			if(partyinc != 0)
			{
				lpObj->Life += ((lpObj->Life * partyinc)/100.0f);
				lpObj->MaxLife += ((lpObj->MaxLife * partyinc)/100.0f);
				lpObj->m_Defense += ((lpObj->m_Defense * partyinc)/100.0f);
				lpObj->m_AttackDamageMin += ((lpObj->m_AttackDamageMin * partyinc)/100.0f);
				lpObj->m_AttackDamageMax += ((lpObj->m_AttackDamageMax * partyinc)/100.0f);
			}
			return;
		}
	}
}

void CDoppelGanger::MsgStringSend(LPSTR szMsg, BYTE type) 
{
	PMSG_NOTICE pNotice;
	pNotice.type = 0;	// 3
	pNotice.btCount = 0;	// 4
	pNotice.wDelay = 0;	// 6	
	pNotice.dwColor = 0;	// 8
	pNotice.btSpeed = 0;	// C
	
	TNotice::MakeNoticeMsg((TNotice*)&pNotice, type, szMsg);


	for ( int n = OBJ_STARTUSERINDEX ; n < OBJMAX ; n++)
	{
		if ( gObj[n].Connected == PLAYER_PLAYING )
		{
			if ( gObj[n].Type  == OBJ_USER )
			{
				if (DG_MAP_RANGE(gObj[n].MapNumber))
				{
					::DataSend(n, (UCHAR*)&pNotice, pNotice.h.size);
				}
			}
		}
	}
}