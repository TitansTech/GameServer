#include "stdafx.h"
#include "user.h"
#include "protocol.h"
#include "..\include\prodef.h"
#include "..\common\winutil.h"
#include "LogProc.h"
#include "DuelManager.h"
#include "GameMain.h"
#include "IllusionTemple.h"
#include "ImperialGuardian.h"
#include "DoppelGanger.h"
#include "BloodCastle.h"
#include "ChaosCastle.h"
#include "DevilSquare.h"
#include "DarkSpirit.h"
#include "ObjUseSkill.h"
#include "SCFPvPSystem.h"
#include "GensPvP.h"
#include "ExServerProtocol.h"
#include "SCFExDB_Defines.h"

static const struct DUEL_GATES
{
	int UserGate01;
	int UserGate02;
	int LearnerGate;
} 
g_DuelGates[MAX_DUEL_ROOMS] = 
{
	{ 295, 296, 303 },
	{ 297, 298, 304 },
	{ 299, 300, 305 },
	{ 301, 302, 306 }
};


CDuelManager g_DuelManager;
BOOL g_DuelSystemState = TRUE;

CDuelManager::CDuelManager(void)
{
	ZeroMemory(&this->m_Rooms, sizeof(this->m_Rooms));
	this->Init();
}

CDuelManager::~CDuelManager(void)
{
}

void CDuelManager::Init()
{
	for(int i = 0; i < MAX_DUEL_ROOMS; i++)
	{
		this->RoomReset(i);
	}
}

int CDuelManager::GetUserDuelRoom(LPOBJ lpObj)
{
	for(int i = 0; i < MAX_DUEL_ROOMS; i++)
	{
		if(this->m_Rooms[i].lpObj01 == lpObj ||
			this->m_Rooms[i].lpObj02 == lpObj) 
		{
			return i;
		}
	}
	return -1;
}

void CDuelManager::Run()
{
	for(int i = 0; i < MAX_DUEL_ROOMS; i++)
	{
		if(this->m_Rooms[i].bFree == false)
		{
			if(GetTickCount() - this->m_Rooms[i].dwStartTime < 5000) continue;

			for(int u = 0; u < MAX_DUEL_LEARNERS; u++)
			{
				if(this->m_Rooms[i].lpLearners[u] == NULL) continue;
				if(gObjIsConnected(this->m_Rooms[i].lpLearners[u]->m_Index))
				{
					if(this->m_Rooms[i].lpLearners[u]->MapNumber != MAP_INDEX_DUELMAP)
					{
						this->gObjSendDuelEnd(this->m_Rooms[i].lpLearners[u]);
						//GCSkillInfoSend(this->m_Rooms[i].lpLearners[u], 0, eVS_INVISIBLE);
						GCSkillInfoSend(this->m_Rooms[i].lpLearners[u], 0, eVS_TRANSPARENCY);
						GCSkillInfoSend(this->m_Rooms[i].lpLearners[u], 0, eVS_DUEL_INTERFACE);
						LogAddTD("[Duel Manager] Spectator (%s) leaves room %d", this->m_Rooms[i].lpLearners[u]->Name, i + 1);
						this->SendSpectatorRemove(u, i);
						this->m_Rooms[i].lpLearners[u] = NULL;
					}
				}
				else
				{
					LogAddTD("[Duel Manager] Some spectator leaves room %d", this->m_Rooms[i].lpLearners[u]->Name, i + 1);
					this->m_Rooms[i].lpLearners[u] = NULL;
					this->SendSpectatorList(i);
				}
			}

			if(this->m_Rooms[i].lpObj01 != NULL && this->m_Rooms[i].lpObj02 != NULL)
			{
				if(gObjIsConnected(this->m_Rooms[i].lpObj01->m_Index) == FALSE ||
					gObjIsConnected(this->m_Rooms[i].lpObj02->m_Index) == FALSE)
				{
					this->RoomReset(i);
					continue;
				}
				else if ((this->m_Rooms[i].lpObj01->MapNumber != MAP_INDEX_DUELMAP ||
					this->m_Rooms[i].lpObj02->MapNumber != MAP_INDEX_DUELMAP) && 
					this->m_Rooms[i].bHasWinner == false)
				{
					this->RoomReset(i);
					continue;
				}
				else if (this->m_Rooms[i].lpObj01->Connected < PLAYER_PLAYING ||
					this->m_Rooms[i].lpObj02->Connected < PLAYER_PLAYING)
				{
					this->RoomReset(i);
					continue;
				}
			}
			else
			{
				this->RoomReset(i);
				continue;
			}

			if(this->m_UpdateLifebarTime < GetTickCount())
			{
				this->SendLifebarStatus(i);
				this->m_UpdateLifebarTime = GetTickCount() + 2000;
			}
			if(this->m_Rooms[i].dwTickCount != 0)
			{
				if(this->m_Rooms[i].dwTickCount < GetTickCount())
				{
					this->RoomReset(i);
				}
			}
		}
	}

	if(this->m_UpdateTickCount < GetTickCount())
	{
		for(int i = OBJ_STARTUSERINDEX; i < OBJMAX; i++)
		{
			if((gObj[i].m_IfState.use) && gObj[i].m_IfState.type == 20)
			{
				this->SendDuelStatus(&gObj[i]);
			}	
		}
		this->m_UpdateTickCount = GetTickCount() + 6000;
	}
}

void CDuelManager::UpdateDuelScore(int iRoom) 
{ 
	if(iRoom < 0 || iRoom > MAX_DUEL_ROOMS - 1)
		return;

	if(this->m_Rooms[iRoom].lpObj01 == NULL || this->m_Rooms[iRoom].lpObj02 == NULL) return;

	int aIndex1 = this->m_Rooms[iRoom].lpObj01->m_Index;
	int aIndex2 = this->m_Rooms[iRoom].lpObj02->m_Index;

	if ( !OBJMAX_RANGE(aIndex1) || !OBJMAX_RANGE(aIndex2))
		return;

	if ( !gObjIsConnected(aIndex1) || !gObjIsConnected(aIndex2) )
		return;

	if ( gObj[aIndex1].Type == OBJ_MONSTER || gObj[aIndex2].Type == OBJ_MONSTER )
		return;
	
	if ( gObj[aIndex1].CloseCount >= 0 || gObj[aIndex2].CloseCount >= 0 )
		return;

	if ( gObj[aIndex1].m_bMapAntiHackMove == true || gObj[aIndex2].m_bMapAntiHackMove == true )
		return;

	PMSG_DUEL_SCORE pMsg;

	pMsg.h.c = 0xC1;
	pMsg.h.size = sizeof(pMsg);
	pMsg.h.headcode = 0xAA;
	pMsg.h.subcode = 0x04;
	pMsg.NumberH1 = SET_NUMBERH(aIndex1);
	pMsg.NumberL1 = SET_NUMBERL(aIndex1);
	pMsg.NumberH2 = SET_NUMBERH(aIndex2);
	pMsg.NumberL2 = SET_NUMBERL(aIndex2);
	pMsg.btDuelScore1 = this->m_Rooms[iRoom].btPoints01;
	pMsg.btDuelScore2 = this->m_Rooms[iRoom].btPoints02;

	DataSend(aIndex1, (LPBYTE)&pMsg, pMsg.h.size);
	DataSend(aIndex2, (LPBYTE)&pMsg, pMsg.h.size);
	//GCSendDuelScore(aIndex1,aIndex2);

	for(int i = 0; i < MAX_DUEL_LEARNERS; i++)
	{
		if(this->m_Rooms[iRoom].lpLearners[i] == NULL) 
		{
			continue;
		}
		DataSend(this->m_Rooms[iRoom].lpLearners[i]->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
	}

}

void CDuelManager::PlayerScore(LPOBJ lpObj)
{
	int iRoom = this->GetUserDuelRoom(lpObj);
	if(iRoom >= 0 && iRoom < MAX_DUEL_ROOMS)
	{
		if(this->m_Rooms[iRoom].lpObj01 == lpObj)
		{
			this->m_Rooms[iRoom].btPoints01++;
			this->UpdateDuelScore(iRoom);
		}
		else if(this->m_Rooms[iRoom].lpObj02 == lpObj)
		{
			this->m_Rooms[iRoom].btPoints02++;
			this->UpdateDuelScore(iRoom);
		}
	}
}

void CDuelManager::gObjSendDuelEnd(LPOBJ lpObj)
{
	PMSG_END_DUEL pMsg;
	if(gObjIsConnected(lpObj->m_Index)==0)
		return;

	if(lpObj->Type == OBJ_MONSTER)
		return;

	if(lpObj->CloseCount >= 0)
		return;

	if ( lpObj->m_bMapSvrMoveQuit == true || lpObj->m_bMapAntiHackMove == true )
		return;

	lpObj->m_iDuelUser = -1;
	lpObj->m_iDuelUserReserved = -1;
	lpObj->m_iDuelUserRequested = -1;
	lpObj->m_btDuelScore = 0;
	lpObj->m_iDuelTickCount = 0;

	PHeadSubSetB((LPBYTE)&pMsg, 0xAA, 0x03, sizeof(pMsg));
	pMsg.Unk = 0;

	DataSend(lpObj->m_Index,(unsigned char *)&pMsg,pMsg.h.size);
}

void CDuelManager::gObjResetDuel(LPOBJ lpObj)
{
	int iDuelIndex = lpObj->m_iDuelUser;

	if ( OBJMAX_RANGE(iDuelIndex))
	{
		lpObj->m_iDuelUser = -1;
		lpObj->m_iDuelUserReserved = -1;
		lpObj->m_btDuelScore = 0;
		lpObj->m_iDuelTickCount = 0;
		gDarkSpirit[lpObj->m_Index].ReSetTarget(iDuelIndex);
		gDarkSpirit[iDuelIndex].ReSetTarget(lpObj->m_Index);
		gObj[iDuelIndex].m_iDuelUser = -1;
		gObj[iDuelIndex].m_iDuelUserReserved = -1;
		gObj[iDuelIndex].m_iDuelUserRequested = -1;
		gObj[iDuelIndex].m_iDuelTickCount = 0;
	}
}

int CDuelManager::GetSpectatorCount(int iRoom)
{
	if(iRoom < 0 || iRoom >= MAX_DUEL_ROOMS) 
	{
		return -1;
	}

	int iCount = 0;
	for(int i = 0; i < MAX_DUEL_LEARNERS; i++)
	{
		if(this->m_Rooms[iRoom].lpLearners[i] != NULL) 
		{
			iCount++;
		}
	}

	return iCount;
}

void CDuelManager::RemoveUser(LPOBJ lpObj) 
{ 

}

void CDuelManager::SendDuelStatus(LPOBJ lpObj)
{
	PMSG_DUEL_STATUS pMsg;

	pMsg.h.c = 0xC1;
	pMsg.h.size = sizeof(pMsg);
	pMsg.h.headcode = 0xAA;
	pMsg.h.subcode = 0x06;

	ZeroMemory(&pMsg.pRoomStatus, sizeof(pMsg.pRoomStatus));
	
	for(int i = 0; i < MAX_DUEL_ROOMS; i++)
	{
		if(this->m_Rooms[i].bFree == false)
		{
			if(this->m_Rooms[i].lpObj01 == NULL || this->m_Rooms[i].lpObj02 == NULL)
			{
				continue;
			}

			pMsg.pRoomStatus[i].btDuelRunning = TRUE;

			int iSpecCount = this->GetSpectatorCount(i);
			if(iSpecCount < 0 || iSpecCount >= 10)
			{
				pMsg.pRoomStatus[i].btDuelOpen = FALSE;
			}
			else
			{
				pMsg.pRoomStatus[i].btDuelOpen = TRUE;
			}

			memcpy(&pMsg.pRoomStatus[i].szName1[0], &this->m_Rooms[i].lpObj01->Name[0], 10);
			memcpy(&pMsg.pRoomStatus[i].szName2[0], &this->m_Rooms[i].lpObj02->Name[0], 10);
		}
	}

	DataSend(lpObj->m_Index, (BYTE*)&pMsg, pMsg.h.size);
}

int CDuelManager::GetFreeRoomIndex()
{
	for(int i = 0; i < MAX_DUEL_ROOMS; i++)
	{
		//LogAddTD("[Duel Manager][Tracer] Room:%d [%d,%d,%d,%d,%d]", 
		//	i,this->m_Rooms[i].bFree,this->m_Rooms[i].bWaiting,this->m_Rooms[i].dwWatingTime,this->m_Rooms[i].dwStartTime,this->m_Rooms[i].dwTickCount);

		if(this->m_Rooms[i].bFree == true)
		{
			if ( (GetTickCount() - this->m_Rooms[i].dwWatingTime) > 10000 )
			{
				this->m_Rooms[i].dwWatingTime = 0;
				this->m_Rooms[i].bWaiting = false;
				LogAddTD("[Duel Manager][Fixer] Room:%d [%d,%d,%d,%d,%d]", 
					i,this->m_Rooms[i].bFree,this->m_Rooms[i].bWaiting,this->m_Rooms[i].dwWatingTime,this->m_Rooms[i].dwStartTime,this->m_Rooms[i].dwTickCount);
			}
			if(this->m_Rooms[i].bWaiting == false)
			{
				return i;	
			}
		}
	}
	return -1;
}

void CDuelManager::UserDuelInfoReset(LPOBJ lpObj) 
{ 
	if(lpObj == NULL) 
	{	
		return;
	}
	lpObj->m_iDuelRoom = -1;
	lpObj->m_iDuelUser = -1;
	lpObj->m_iDuelUserRequested = -1;
	lpObj->m_iDuelUserReserved = -1;
	lpObj->m_btDuelScore = 0;
	gDarkSpirit[lpObj->m_Index].ReSetTarget(lpObj->m_Index);
}

void CDuelManager::RoomReset(int iRoom, bool dontMove, bool dontSendEnd)
{
	if(this->m_Rooms[iRoom].lpObj01 != NULL)
	{
		if(gObjIsConnected(this->m_Rooms[iRoom].lpObj01->m_Index))
		{
			if(dontSendEnd == false)
				this->gObjSendDuelEnd(this->m_Rooms[iRoom].lpObj01);
			if(this->m_Rooms[iRoom].lpObj01->MapNumber == MAP_INDEX_DUELMAP)
			{
				if(dontMove == false)
					gObjMoveGate(this->m_Rooms[iRoom].lpObj01->m_Index, 294);
			}
		}
		this->UserDuelInfoReset(this->m_Rooms[iRoom].lpObj01);
	}

	if(this->m_Rooms[iRoom].lpObj02 != NULL)
	{
		if(gObjIsConnected(this->m_Rooms[iRoom].lpObj02->m_Index))
		{
			if(dontSendEnd == false)
				this->gObjSendDuelEnd(this->m_Rooms[iRoom].lpObj02);
			if(this->m_Rooms[iRoom].lpObj02->MapNumber == MAP_INDEX_DUELMAP)
			{
				if(dontMove == false)
					gObjMoveGate(this->m_Rooms[iRoom].lpObj02->m_Index, 294);
			}
		}
		this->UserDuelInfoReset(this->m_Rooms[iRoom].lpObj02);
	}

	for(int i = 0; i < MAX_DUEL_LEARNERS; i++)
	{
		if(this->m_Rooms[iRoom].lpLearners[i] != NULL)
		{
			gObj[this->m_Rooms[iRoom].lpLearners[i]->m_Index].m_isDuelSpectator = 0;
			this->gObjSendDuelEnd(this->m_Rooms[iRoom].lpLearners[i]);
			//GCSkillInfoSend(this->m_Rooms[iRoom].lpLearners[i], 0, eVS_INVISIBLE);
			GCSkillInfoSend(this->m_Rooms[iRoom].lpLearners[i], 0, eVS_TRANSPARENCY);
			GCSkillInfoSend(this->m_Rooms[iRoom].lpLearners[i], 0, eVS_DUEL_INTERFACE);
			gObjMoveGate(this->m_Rooms[iRoom].lpLearners[i]->m_Index, 294);
		}
		this->m_Rooms[iRoom].lpLearners[i] = NULL;
	}

	this->m_Rooms[iRoom].lpObj01 = NULL;
	this->m_Rooms[iRoom].btPoints01 = 0;

	this->m_Rooms[iRoom].lpObj02 = NULL;
	this->m_Rooms[iRoom].btPoints02 = 0;

	this->m_Rooms[iRoom].bFree = true;
	this->m_Rooms[iRoom].bWaiting = false;
	this->m_Rooms[iRoom].bHasWinner = false;

	this->m_Rooms[iRoom].dwTickCount = 0;
	this->m_Rooms[iRoom].dwStartTime = 0;

	//LogAddTD("[Duel Manager] Room Reset - Number: %d", iRoom + 1);
}

void CDuelManager::gObjSendDuelEndNotification(LPOBJ lpObj, char* Winner, char* Looser)
{

	PMSG_DUEL_FINISH pMsg;
	pMsg.h.c = 0xC1;
	pMsg.h.size = sizeof(pMsg);
	pMsg.h.headcode = 0xAA;
	pMsg.h.subcode = 0x0C;
	memcpy(pMsg.szWinner, Winner, 10);
	memcpy(pMsg.szLooser, Looser, 10);

	DataSend(lpObj->m_Index, (BYTE*)&pMsg, pMsg.h.size);

}

void CDuelManager::SendSpectatorAdd(int iSpecIndex, int iRoom)
{
	if(iRoom < 0 || iRoom >= MAX_DUEL_ROOMS)
	{
		return;
	}

	if(iSpecIndex < 0 || iSpecIndex >= MAX_DUEL_LEARNERS)
	{
		return;
	}

	if(this->m_Rooms[iRoom].lpLearners[iSpecIndex] == NULL) return;

	if(this->m_Rooms[iRoom].lpObj01 == NULL || 
		this->m_Rooms[iRoom].lpObj02 == NULL)
	{
		return;
	}

	//Add Spectator State
	this->m_Rooms[iRoom].lpLearners[iSpecIndex]->m_isDuelSpectator = TRUE;
	this->m_Rooms[iRoom].lpLearners[iSpecIndex]->m_IfState.state = 0;
	this->m_Rooms[iRoom].lpLearners[iSpecIndex]->m_IfState.type = 0;
	this->m_Rooms[iRoom].lpLearners[iSpecIndex]->m_IfState.use = 0;

	PMSG_DUEL_SPEC_ADD pMsg;
	pMsg.h.c = 0xC1;
	pMsg.h.size = sizeof(pMsg);
	pMsg.h.headcode = 0xAA;
	pMsg.h.subcode = 0x08;
	memcpy(pMsg.szName, this->m_Rooms[iRoom].lpLearners[iSpecIndex]->Name, 10);

	for(int i = 0; i < MAX_DUEL_LEARNERS; i++)
	{
		if(i == iSpecIndex) continue;
		if(this->m_Rooms[iRoom].lpLearners[i] != NULL)
		{
			DataSend(this->m_Rooms[iRoom].lpLearners[i]->m_Index, (BYTE*)&pMsg, pMsg.h.size);
		}
	}
}

void CDuelManager::SendSpectatorRemove(int iSpecIndex, int iRoom)
{
	if(iRoom < 0 || iRoom >= MAX_DUEL_ROOMS)
	{
		return;
	}

	if(iSpecIndex < 0 || iSpecIndex >= MAX_DUEL_LEARNERS)
	{
		return;
	}

	if(this->m_Rooms[iRoom].lpLearners[iSpecIndex] == NULL) return;

	if(this->m_Rooms[iRoom].lpObj01 == NULL || 
		this->m_Rooms[iRoom].lpObj02 == NULL)
	{
		return;
	}

	//Remove Spectator State
	this->m_Rooms[iRoom].lpLearners[iSpecIndex]->m_isDuelSpectator = FALSE;
	gObj[this->m_Rooms[iRoom].lpLearners[iSpecIndex]->m_Index].m_isDuelSpectator = 0;

	PMSG_DUEL_SPEC_ADD pMsg;
	pMsg.h.c = 0xC1;
	pMsg.h.size = sizeof(pMsg);
	pMsg.h.headcode = 0xAA;
	pMsg.h.subcode = 0x0A;
	memcpy(pMsg.szName, this->m_Rooms[iRoom].lpLearners[iSpecIndex]->Name, 10);

	for(int i = 0; i < MAX_DUEL_LEARNERS; i++)
	{
		if(i == iSpecIndex) continue;
		if(this->m_Rooms[iRoom].lpLearners[i] != NULL)
		{
			DataSend(this->m_Rooms[iRoom].lpLearners[i]->m_Index, (BYTE*)&pMsg, pMsg.h.size);
		}
	}
}

void CDuelManager::SendSpectatorList(int iRoom)
{
	if(iRoom < 0 || iRoom >= MAX_DUEL_ROOMS)
	{
		return;
	}

	if(this->m_Rooms[iRoom].bFree == false)
	{
		for(int u = 0; u < MAX_DUEL_LEARNERS; u++)
		{
			if(this->m_Rooms[iRoom].lpLearners[u] != NULL)
			{
				this->SendSpectatorList(this->m_Rooms[iRoom].lpLearners[u], iRoom);
			}
		}
	}
}

void CDuelManager::SendSpectatorList(LPOBJ lpObj, int iRoom)
{
	if(iRoom < 0 || iRoom >= MAX_DUEL_ROOMS)
	{
		return;
	}

	if(this->m_Rooms[iRoom].lpObj01 == NULL || 
		this->m_Rooms[iRoom].lpObj02 == NULL)
	{
		return;
	}

	PMSG_DUEL_SPEC_LIST pMsg;
	pMsg.h.c = 0xC1;
	pMsg.h.headcode = 0xAA;
	pMsg.h.subcode = 0x0B;
	pMsg.btCount = 0;

	if(this->m_Rooms[iRoom].bFree == false)
	{
		for(int u = 0; u < MAX_DUEL_LEARNERS; u++)
		{
			if(this->m_Rooms[iRoom].lpLearners[u] != NULL)
			{
				memcpy(&pMsg.szName[pMsg.btCount++][0], this->m_Rooms[iRoom].lpLearners[u]->Name, 10);
			}
		}
	}

	pMsg.h.size = 5 + (pMsg.btCount * 10);

	DataSend(lpObj->m_Index, (BYTE*)&pMsg, pMsg.h.size);
}

void CDuelManager::SendLifebarStatus(int iRoom)
{
	if(iRoom < 0 || iRoom >= MAX_DUEL_ROOMS)
	{
		return;
	}

	if(this->m_Rooms[iRoom].lpObj01 == NULL || 
		this->m_Rooms[iRoom].lpObj02 == NULL)
	{
		return;
	}

	if(this->m_Rooms[iRoom].bFree == false)
	{
		for(int u = 0; u < MAX_DUEL_LEARNERS; u++)
		{
			if(this->m_Rooms[iRoom].lpLearners[u] != NULL)
			{
				this->SendLifebarStatus(this->m_Rooms[iRoom].lpLearners[u], iRoom);
			}
		}
	}
}

void CDuelManager::SendLifebarStatus(LPOBJ lpObj, int iRoom)
{
	if(iRoom < 0 || iRoom >= MAX_DUEL_ROOMS)
	{
		return;
	}

	if(this->m_Rooms[iRoom].lpObj01 == NULL || 
		this->m_Rooms[iRoom].lpObj02 == NULL)
	{
		return;
	}

	PMSG_DUEL_LIFEBAR_REFILL pMsg;
	pMsg.h.c = 0xC1;
	pMsg.h.size = sizeof(pMsg);
	pMsg.h.headcode = 0xAA;
	pMsg.h.subcode = 0x05;

	pMsg.btObjId01H = HIBYTE(this->m_Rooms[iRoom].lpObj01->m_Index);
	pMsg.btObjId01L = LOBYTE(this->m_Rooms[iRoom].lpObj01->m_Index);

	pMsg.btObjId02H = HIBYTE(this->m_Rooms[iRoom].lpObj02->m_Index);
	pMsg.btObjId02L = LOBYTE(this->m_Rooms[iRoom].lpObj02->m_Index);

	pMsg.btLife01 = this->m_Rooms[iRoom].lpObj01->Life / ((this->m_Rooms[iRoom].lpObj01->MaxLife + this->m_Rooms[iRoom].lpObj01->AddLife) / 100);
	pMsg.btLife02 = this->m_Rooms[iRoom].lpObj02->Life / ((this->m_Rooms[iRoom].lpObj02->MaxLife + this->m_Rooms[iRoom].lpObj02->AddLife) / 100);

	pMsg.btShield01 = this->m_Rooms[iRoom].lpObj01->iShield / ((this->m_Rooms[iRoom].lpObj01->iMaxShield + this->m_Rooms[iRoom].lpObj01->iAddShield) / 100);
	pMsg.btShield02 = this->m_Rooms[iRoom].lpObj02->iShield / ((this->m_Rooms[iRoom].lpObj02->iMaxShield + this->m_Rooms[iRoom].lpObj02->iAddShield) / 100);

	DataSend(lpObj->m_Index, (BYTE*)&pMsg, pMsg.h.size);
}

void CDuelManager::SendLifebarInit(LPOBJ lpObj, int iRoom)
{
	if(iRoom < 0 || iRoom >= MAX_DUEL_ROOMS)
	{
		return;
	}

	if(this->m_Rooms[iRoom].lpObj01 == NULL || 
		this->m_Rooms[iRoom].lpObj02 == NULL)
	{
		return;
	}

	PMSG_DUEL_LIFEBAR_NAME pMsg2;
	pMsg2.h.c = 0xC1;
	pMsg2.h.size = sizeof(pMsg2);
	pMsg2.h.headcode = 0xAA;
	pMsg2.h.subcode = 0x07;
	pMsg2.Type = 0x00;
	
	memcpy(pMsg2.szName1, this->m_Rooms[iRoom].lpObj01->Name, 10);
	memcpy(pMsg2.szName2, this->m_Rooms[iRoom].lpObj02->Name, 10);

	pMsg2.btObjId1H = HIBYTE(this->m_Rooms[iRoom].lpObj01->m_Index);
	pMsg2.btObjId1L = LOBYTE(this->m_Rooms[iRoom].lpObj01->m_Index);

	pMsg2.btObjId2H = HIBYTE(this->m_Rooms[iRoom].lpObj02->m_Index);
	pMsg2.btObjId2L = LOBYTE(this->m_Rooms[iRoom].lpObj02->m_Index);

	DataSend(lpObj->m_Index, (BYTE*)&pMsg2, pMsg2.h.size);

	PMSG_DUEL_LIFEBAR_INIT pMsg;
	pMsg.h.c = 0xC1;
	pMsg.h.size = sizeof(pMsg);
	pMsg.h.headcode = 0xAA;
	pMsg.h.subcode = 0x0D;

	DataSend(lpObj->m_Index, (BYTE*)&pMsg, pMsg.h.size);
}



void CDuelManager::SendRewardScore(int iDuelRoom)
{
	if(ReadConfig.DuelManager__IsSaveInDB == 0)
		return;

	if(ReadConfig.SCFRSON == TRUE)
	{
		LPOBJ lpObj = this->m_Rooms[iDuelRoom].lpObj01;
		LPOBJ lpTarget = this->m_Rooms[iDuelRoom].lpObj02;

		if ( OBJMAX_RANGE(lpObj->m_Index) == FALSE )
		{
			return;
		}

		if ( OBJMAX_RANGE(lpTarget->m_Index) == FALSE )
		{
			return;
		}
		
		int Level1 = lpObj->Level + lpObj->MasterCharacterInfo->MasterLevel;
		int Level2 = lpTarget->Level + lpTarget->MasterCharacterInfo->MasterLevel;

		if(ReadConfig.DuelManager__IsLevelDiferenceForSave == 1)
		{
			if(abs(Level1 - Level2) > ReadConfig.DuelManager__LevelDiferenceForSave)
				return;
			/*if(Level1 <= Level2)
			{
				if((Level1 + ReadConfig.DuelManager__LevelDiferenceForSave) < Level2)
					return;
			}else if(Level2 <= Level1)
			{
				if((Level2 + ReadConfig.DuelManager__LevelDiferenceForSave) < Level1)
					return;
			}*/
		}

		if(ReadConfig.DuelManager__IsResetDiferenceForSave == 1)
		{
			if(abs(lpObj->Resets - lpTarget->Resets) > ReadConfig.DuelManager__ResetDiferenceForSave)
				return;
			/*if(lpObj->Resets < lpTarget->Resets)
			{
				if((lpObj->Resets + ReadConfig.DuelManager__ResetDiferenceForSave) < lpTarget->Resets)
					return;
			}else if(lpTarget->Resets < lpObj->Resets)
			{
				if((lpTarget->Resets + ReadConfig.DuelManager__ResetDiferenceForSave) < lpObj->Resets)
					return;
			}*/
		}

		if(ReadConfig.DuelManager__MinLevelForSave > Level1)
			return;

		if(ReadConfig.DuelManager__MinLevelForSave > Level2)
			return;

		if(ReadConfig.DuelManager__MinResetForSave > lpObj->Resets)
			return;

		if(ReadConfig.DuelManager__MinResetForSave > lpTarget->Resets)
			return;

		PMSG_ANS_DUELSCORE pMsg = {0};
		PHeadSetB((LPBYTE)&pMsg, SCFExDB_GSSend_DuelScore, sizeof(pMsg));

		memcpy(pMsg.AccountID_1, lpObj->AccountID, MAX_ACCOUNT_LEN);
		memcpy(pMsg.Name_1, lpObj->Name, MAX_ACCOUNT_LEN);
		memcpy(pMsg.AccountID_2, lpTarget->AccountID, MAX_ACCOUNT_LEN);
		memcpy(pMsg.Name_2, lpTarget->Name, MAX_ACCOUNT_LEN);
		pMsg.ServerCode = gGameServerCode;
		pMsg.Player1_Score = this->m_Rooms[iDuelRoom].btPoints01;
		pMsg.Player2_Score = this->m_Rooms[iDuelRoom].btPoints02;

		wsExServerCli.DataSend((char*)&pMsg, pMsg.h.size);
	}
}


void CDuelManager::KillUserProc(LPOBJ lpObj, LPOBJ lpTarget)
{ 	
	if(!this->DuelCheck(lpObj, lpTarget)) return;
	
	int iDuelRoom = this->GetUserDuelRoom(lpObj);
	if(iDuelRoom == -1)
	{
		return;
	}

	lpTarget->KillerType = 3;

	int Points = ((lpObj == this->m_Rooms[iDuelRoom].lpObj01) ? this->m_Rooms[iDuelRoom].btPoints01 : this->m_Rooms[iDuelRoom].btPoints02);
	if(Points >= DUEL_WIN_POINT_COUNT)
	{
		this->m_Rooms[iDuelRoom].bHasWinner = true;

		this->gObjSendDuelEnd(lpTarget);
		this->gObjSendDuelEnd(lpObj);

		this->gObjSendDuelEndNotification(lpTarget, lpObj->Name, lpTarget->Name);
		this->gObjSendDuelEndNotification(lpObj, lpObj->Name, lpTarget->Name);

		this->SendRewardScore(iDuelRoom);

		gDarkSpirit[lpObj->m_Index].ReSetTarget(lpTarget->m_Index);
		gDarkSpirit[lpTarget->m_Index].ReSetTarget(lpObj->m_Index);

		char szMsg[256];

		wsprintf(szMsg,lMsg.Get(1216),lpTarget->Name);
		GCServerMsgStringSend(szMsg,lpObj->m_Index,1);

		wsprintf(szMsg,lMsg.Get(1217),lpObj->Name);
		GCServerMsgStringSend(szMsg,lpTarget->m_Index,1);

		PMSG_SERVERCMD ServerCmd;

		PHeadSubSetB((LPBYTE)&ServerCmd, 0xF3, 0x40, sizeof(ServerCmd));
		ServerCmd.CmdType = 0;
		ServerCmd.X = lpObj->X;
		ServerCmd.Y = lpObj->Y;

		MsgSendV2(lpObj,(unsigned char *)&ServerCmd,sizeof(ServerCmd));
		DataSend(lpObj->m_Index,(unsigned char *)&ServerCmd,sizeof(ServerCmd));

		gObjUseSkill.GladiatorsGlory(eVS_DUEL_MEDAL, lpObj->m_Index, TRUE);

		this->m_Rooms[iDuelRoom].dwTickCount = GetTickCount() + 10000;

		char szTemp[256]="";
		wsprintf(szTemp,lMsg.Get(MSGGET(14, 87)),lpObj->Name,lpTarget->Name);
		if(ReadConfig.DuelManager__IsGlobalMsg == 1)
			AllSendServerMsg(szTemp);
		
		LogAdd("[Duel] [%s][%s] Win Duel, Loser [%s][%s]",
			lpObj->AccountID,lpObj->Name,lpTarget->AccountID,lpTarget->Name,
			this->m_Rooms[iDuelRoom].btPoints01,this->m_Rooms[iDuelRoom].btPoints02);
	}
}

void CDuelManager::ProtocolCore(LPOBJ lpObj, BYTE* lpPacket)
{
#if (GS_CASTLE==0)
	PMSG_DEFAULT2* pMsg = (PMSG_DEFAULT2*)lpPacket;
	switch(pMsg->subcode)
	{
		case 0x01:
			this->RecvDuelRequest(lpObj, (PMSG_DUEL_REQUEST_START*)lpPacket);
		break;
		case 0x02:
			this->RecvDuelAnswer(lpObj, (PMSG_DUEL_ANSWER_START*)lpPacket);
		break;
		case 0x07:
			this->RecvWatchRequest(lpObj, (PMSG_DUEL_REQUEST_WATCH*)lpPacket);
		break;
		case 0x09:
			if(lpObj->MapNumber == MAP_INDEX_DUELMAP)
			{
				gObjMoveGate(lpObj->m_Index, 294); //Vulcan map
			}
		break;
	}
#endif
}

void CDuelManager::RecvWatchRequest(LPOBJ lpObj, PMSG_DUEL_REQUEST_WATCH* lpMsg)
{

	if(lpObj->m_IfState.use == 0 || lpObj->m_IfState.type != 20) 
	{
		return;
	}

	lpObj->m_IfState.use = 0;
	lpObj->m_IfState.type = 0;

	if(lpMsg->btRoomIndex >= 0 && lpMsg->btRoomIndex < MAX_DUEL_ROOMS)
	{

		if(this->m_Rooms[lpMsg->btRoomIndex].bFree == true) 
		{
			GCServerMsgStringSend("Watch request error.", lpObj->m_Index, 1);
			LogAddC(2, "[Duel Manager] (%s)(%s) Watch request error: wrong duel state! (%d)", lpObj->AccountID, lpObj->Name, lpMsg->btRoomIndex);
			return;
		}

		for(int i = 0; i < MAX_DUEL_LEARNERS; i++)
		{
			if(this->m_Rooms[lpMsg->btRoomIndex].lpLearners[i] == NULL)
			{
				if(gObjMoveGate(lpObj->m_Index, g_DuelGates[lpMsg->btRoomIndex].LearnerGate))
				{
					this->m_Rooms[lpMsg->btRoomIndex].lpLearners[i] = lpObj;
					this->SendSpectatorList(lpObj, lpMsg->btRoomIndex);
					this->SendSpectatorAdd(i, lpMsg->btRoomIndex);
					GCServerMsgStringSend(lMsg.Get(MSGGET(14, 88)), lpObj->m_Index, 1);
					GCSkillInfoSend(lpObj, 1, eVS_DUEL_INTERFACE);
					GCSkillInfoSend(lpObj, 1, eVS_TRANSPARENCY);
					lpObj->m_isDuelSpectator = 1;

					Sleep(500);		//DaRKav DC FIX

					//GCSkillInfoSend(lpObj, 1, eVS_INVISIBLE);
					this->SendLifebarInit(lpObj, lpMsg->btRoomIndex);
					this->SendLifebarStatus(lpObj, lpMsg->btRoomIndex);
					this->UpdateDuelScore(lpMsg->btRoomIndex);
					return;
				}			
				GCServerMsgStringSend(lMsg.Get(MSGGET(14, 89)), lpObj->m_Index, 1);
				return;
			}
		}

		GCServerMsgStringSend(lMsg.Get(MSGGET(14, 90)), lpObj->m_Index, 1);
		LogAddC(2, "[Duel Manager] (%s)(%s) Watch request error: room is full! (%d)", lpObj->AccountID, lpObj->Name, lpMsg->btRoomIndex);

		return;

	}

	LogAddC(2, "[Duel Manager] (%s)(%s) Wrong room index! (%d)", lpObj->AccountID, lpObj->Name, lpMsg->btRoomIndex);

}

void CDuelManager::RecvDuelAnswer(LPOBJ lpObj, PMSG_DUEL_ANSWER_START* lpMsg)
{
	int iDuelIndex = -1;
	int iDuelRoom = lpObj->m_iDuelRoom;
	
	PMSG_DUEL_START pMsgSend;
	
	pMsgSend.h.c = 0xC1;
	pMsgSend.h.headcode = 0xAA;
	pMsgSend.h.subcode = 0x01;
	pMsgSend.bDuelStart = 0;
	pMsgSend.h.size = sizeof(pMsgSend);

	if(iDuelRoom < 0 || iDuelRoom > MAX_DUEL_ROOMS - 1) 
	{
		return;
	}

	this->m_Rooms[iDuelRoom].bWaiting = false;

	if ( gObj[lpObj->m_Index].CloseType != -1 ) 
		return;
	
	iDuelIndex = MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL);

	if ( OBJMAX_RANGE(iDuelIndex) )
	{
		if ( !gObjIsConnected(iDuelIndex) )
			return;

		if ( gObj[iDuelIndex].Type == OBJ_MONSTER )
			return;

		if ( gObj[iDuelIndex].CloseCount >= 0 )
			return;

		if ( gObj[iDuelIndex].m_bMapSvrMoveQuit == true || gObj[iDuelIndex].m_bMapAntiHackMove == true )
			return;

		if(this->m_Rooms[iDuelRoom].bFree == false)
		{
			this->RoomReset(iDuelRoom, true, true);
			DataSend(iDuelIndex, (LPBYTE)&pMsgSend, pMsgSend.h.size);
			return;
		}

		if ( lpMsg->bDuelOK )
		{
			if ( BC_MAP_RANGE(gObj[iDuelIndex].MapNumber) ||
				 CC_MAP_RANGE(gObj[iDuelIndex].MapNumber) ||
				 DS_MAP_RANGE(gObj[iDuelIndex].MapNumber) ||
#if (PACK_EDITION>=2)
				 IMPERIALGUARDIAN_MAP_RANGE(gObj[iDuelIndex].MapNumber) ||
#endif
#if (PACK_EDITION>=3)
				 DG_MAP_RANGE(gObj[iDuelIndex].MapNumber) ||
#endif
				 IT_MAP_RANGE(gObj[iDuelIndex].MapNumber) )
			{

				GCServerMsgStringSend(lMsg.Get(MSGGET(4, 207)), lpObj->m_Index, 1);

				this->RoomReset(iDuelRoom, true, true);

				memcpy(pMsgSend.szName, lpObj->Name, sizeof(pMsgSend.szName));
				DataSend(iDuelIndex, (LPBYTE)&pMsgSend, pMsgSend.h.size);
				return;
			}
		}

		if ( OBJMAX_RANGE(lpObj->m_iDuelUser ) )
		{
			GCServerMsgStringSend(lMsg.Get(MSGGET(4, 160)), lpObj->m_Index, 1);
			GCServerMsgStringSend(lMsg.Get(MSGGET(4, 161)), lpObj->m_Index, 1);
			
			this->RoomReset(iDuelRoom, true, true);

			memcpy(pMsgSend.szName, lpObj->Name, sizeof(pMsgSend.szName));
			DataSend(iDuelIndex, (LPBYTE)&pMsgSend, pMsgSend.h.size);
			return;
		}

		if ( OBJMAX_RANGE(lpObj->m_iDuelUserReserved) )
		{
			GCServerMsgStringSend(lMsg.Get(MSGGET(4, 173)), lpObj->m_Index, 1);

			this->RoomReset(iDuelRoom, true, true);

			memcpy(pMsgSend.szName, lpObj->Name, sizeof(pMsgSend.szName));
			DataSend(iDuelIndex, (LPBYTE)&pMsgSend, pMsgSend.h.size);
			return;
		}

		if ( gObj[iDuelIndex].m_iDuelUserReserved == lpObj->m_Index )
		{
			char szDuelName[MAX_ACCOUNT_LEN+1]={0};
			char szDuelName2[MAX_ACCOUNT_LEN+1]={0};

			memcpy(szDuelName, gObj[iDuelIndex].Name, MAX_ACCOUNT_LEN);
			szDuelName[MAX_ACCOUNT_LEN] = 0;

			memcpy(szDuelName2, lpMsg->szName, MAX_ACCOUNT_LEN);
			szDuelName2[MAX_ACCOUNT_LEN] = 0;

			if ( !strcmp(szDuelName, szDuelName2))
			{
				if ( lpMsg->bDuelOK == false)
				{
					this->RoomReset(iDuelRoom, true, true);
					pMsgSend.bDuelStart = 0x0F;
					memcpy(pMsgSend.szName, lpObj->Name, sizeof(pMsgSend.szName));
					DataSend(iDuelIndex, (LPBYTE)&pMsgSend, pMsgSend.h.size);
				}
				else if ( lpMsg->bDuelOK == true )
				{

					if ( lpObj->Money < 30000 )
					{
						this->gObjSendDuelEnd(&gObj[iDuelIndex]);
						this->RoomReset(iDuelRoom, true);
						pMsgSend.bDuelStart = 0x0e;
						pMsgSend.h.size = sizeof(pMsgSend);
						DataSend(lpObj->m_Index, (BYTE*)&pMsgSend, pMsgSend.h.size);
						return;
					}
					if(gObj[iDuelIndex].Money < 30000)
					{
						this->gObjSendDuelEnd(lpObj);
						this->RoomReset(iDuelRoom, true);
						pMsgSend.bDuelStart = 0x0e;
						pMsgSend.h.size = sizeof(pMsgSend);
						DataSend(iDuelIndex, (BYTE*)&pMsgSend, pMsgSend.h.size);
						return;
					}

					if(gObjMoveGate(lpObj->m_Index, g_DuelGates[iDuelRoom].UserGate01) == false)
					{
						this->RoomReset(iDuelRoom, true, true);
						return;
					}
					if(gObjMoveGate(iDuelIndex, g_DuelGates[iDuelRoom].UserGate02) == false)
					{
						this->RoomReset(iDuelRoom, true, true);
						return;
					}

					Sleep(500);		//DaRKav DC FIX

					lpObj->Money -= 30000;
					GCMoneySend(lpObj->m_Index, lpObj->Money);

					gObj[iDuelIndex].Money -= 30000;
					GCMoneySend(iDuelIndex, gObj[iDuelIndex].Money);

					gObj[iDuelIndex].m_iDuelUserReserved = -1;
					gObj[iDuelIndex].m_btDuelScore = 0;
					gObj[iDuelIndex].m_iDuelUser = lpObj->m_Index;
					gObj[iDuelIndex].m_iDuelTickCount = GetTickCount();

					this->m_Rooms[iDuelRoom].dwStartTime = GetTickCount();
					this->m_Rooms[iDuelRoom].dwTickCount = GetTickCount() + (DUEL_TIME * 60000);

					lpObj->m_iDuelUserRequested = -1;
					lpObj->m_iDuelUserReserved = -1;

					lpObj->m_btDuelScore = 0;
					lpObj->m_iDuelUser = iDuelIndex;
					lpObj->m_iDuelTickCount = GetTickCount();

					this->m_Rooms[iDuelRoom].bFree = false;
					this->m_Rooms[iDuelRoom].bWaiting = false;
					this->m_Rooms[iDuelRoom].bHasWinner = false;

					pMsgSend.bDuelStart = 0;

					pMsgSend.NumberH = SET_NUMBERH(iDuelIndex);
					pMsgSend.NumberL = SET_NUMBERL(iDuelIndex);

					memcpy(pMsgSend.szName, szDuelName, sizeof(pMsgSend.szName));

					DataSend(lpObj->m_Index, (LPBYTE)&pMsgSend, pMsgSend.h.size);

					pMsgSend.NumberH = SET_NUMBERH(lpObj->m_Index);
					pMsgSend.NumberL = SET_NUMBERL(lpObj->m_Index);

					memcpy(pMsgSend.szName, lpObj->Name, sizeof(pMsgSend.szName));

					DataSend(iDuelIndex, (LPBYTE)&pMsgSend, pMsgSend.h.size);

					this->UpdateDuelScore(iDuelRoom);

					char szTemp[256]="";
					wsprintf(szTemp,lMsg.Get(MSGGET(14, 80)),lpObj->Name,gObj[iDuelIndex].Name,iDuelRoom + 1);
					if(ReadConfig.DuelManager__IsGlobalMsg == 1)
						AllSendServerMsg(szTemp);
					LogAddTD("[Duel] [%s][%s] Duel Started [%s][%s] on Room [%d]", lpObj->AccountID, lpObj->Name,
						gObj[iDuelIndex].AccountID, gObj[iDuelIndex].Name, iDuelRoom + 1);
				}
			}
			else
			{
				this->RoomReset(iDuelRoom);
				DataSend(iDuelIndex, (LPBYTE)&pMsgSend, pMsgSend.h.size);
				return;
			}
		}
		else
		{
			this->RoomReset(iDuelRoom);
			DataSend(iDuelIndex, (LPBYTE)&pMsgSend, pMsgSend.h.size);
			return;
		}
	}
}

void CDuelManager::SendRefuseDuel(LPOBJ lpObj)
{
	if(lpObj == NULL) return;

	PMSG_DUEL_START pMsgSend;
		
	pMsgSend.h.c = 0xC1;
	pMsgSend.h.headcode = 0xAA;
	pMsgSend.h.subcode = 0x01;
	pMsgSend.bDuelStart = 0x0F;
	pMsgSend.h.size = sizeof(pMsgSend);

	this->m_Rooms[lpObj->m_iDuelRoom].lpObj01 = NULL;
	this->m_Rooms[lpObj->m_iDuelRoom].lpObj02 = NULL;
	this->m_Rooms[lpObj->m_iDuelRoom].bWaiting = false;
	this->m_Rooms[lpObj->m_iDuelRoom].bFree = true;

	DataSend(lpObj->m_Index, (BYTE*)&pMsgSend, pMsgSend.h.size);
}

void CDuelManager::RecvDuelRequest(LPOBJ lpObj, PMSG_DUEL_REQUEST_START* lpMsg)
{
	if(g_DuelSystemState != TRUE)
	{
		GCServerMsgStringSend(lMsg.Get(MSGGET(14, 81)), lpObj->m_Index, 1);
		return;
	}

	int iDuelIndex = MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL);

	if ( !OBJMAX_RANGE(iDuelIndex) )
	{
		LogAdd("error : %s %d (%d)", __FILE__, __LINE__, iDuelIndex);
		return;
	}

	if ( iDuelIndex == lpObj->m_Index )
	{
		GCServerMsgStringSend(lMsg.Get(MSGGET(14, 82)), lpObj->m_Index, 1);
		return;
	}

	char szTempText[256];

	if ( lpObj->CloseType != -1 )
		return;
	
	if ( PvP.gNonPKServer ) 
	{
		GCServerMsgStringSend(lMsg.Get(MSGGET(4, 174)), lpObj->m_Index, 1);
		return;
	}

	if ( !PvP.gPkLimitFree ) 
	{
		if ( lpObj->m_PK_Level >= 5 )
		{
			GCServerMsgStringSend(lMsg.Get(MSGGET(4, 175)), lpObj->m_Index, 1);
			return;
		}
		if ( gObj[iDuelIndex].m_PK_Level >= 5 )
		{
			GCServerMsgStringSend(lMsg.Get(MSGGET(4, 176)), lpObj->m_Index, 1);
			return;
		}
	}

	if ( OBJMAX_RANGE(lpObj->m_iDuelUserReserved) )
	{
		GCServerMsgStringSend(lMsg.Get(MSGGET(4, 159)), lpObj->m_Index, 1);
		GCServerMsgStringSend(lMsg.Get(MSGGET(4, 161)), lpObj->m_Index, 1);
		return;
	}

	if ( OBJMAX_RANGE( lpObj->m_iDuelUser ) )
	{
		GCServerMsgStringSend(lMsg.Get(MSGGET(4, 160)), lpObj->m_Index, 1);
		GCServerMsgStringSend(lMsg.Get(MSGGET(4, 161)), lpObj->m_Index, 1);
		return;
	}

	if ( DS_MAP_RANGE(gObj[iDuelIndex].MapNumber ) )
	{
		GCServerMsgStringSend(lMsg.Get(MSGGET(4, 180)), lpObj->m_Index, 1);
		return;
	}

	if ( BC_MAP_RANGE(lpObj->MapNumber) )
	{
		GCServerMsgStringSend(lMsg.Get(MSGGET(4, 166)), lpObj->m_Index, 1);
		return;
	}

	if ( CC_MAP_RANGE(lpObj->MapNumber) )
	{
		GCServerMsgStringSend(lMsg.Get(MSGGET(4, 207)), lpObj->m_Index, 1);
		return;
	}

	if ( IT_MAP_RANGE(lpObj->MapNumber) != FALSE )
	{
		::GCServerMsgStringSend(lMsg.Get(MSGGET(14, 83)), lpObj->m_Index, 1);
		return;
	}

	if(Gens.Enabled == 1)
	{
		if(Gens.AllowDuelWithSameGensFam == 0)
		{
			if ( lpObj->GensFam == gObj[iDuelIndex].GensFam )
			{
				if(lpObj->GensFam != 0 && gObj[iDuelIndex].GensFam != 0)
				{
					GCServerMsgStringSend("You cant make Duel with same Gens Family", lpObj->m_Index, 1);
					return;
				}
			}
		}
	}

	if ( lpObj->Level < ReadConfig.DuelManager__MinLevel || gObj[iDuelIndex].Level < ReadConfig.DuelManager__MinLevel)
	{
		GCServerMsgStringSend(lMsg.Get(MSGGET(14, 84)), lpObj->m_Index, 1);
		PMSG_DUEL_START pMsgSend;
		pMsgSend.h.c = 0xC1;
		pMsgSend.h.headcode = 0xAA;
		pMsgSend.h.subcode = 0x01;
		pMsgSend.bDuelStart = 0x0C;
		pMsgSend.h.size = sizeof(pMsgSend);
		DataSend(lpObj->m_Index, (BYTE*)&pMsgSend, pMsgSend.h.size);
		return;
	}

	if ( lpObj->Money < 30000 || gObj[iDuelIndex].Money < 30000 )
	{
		GCServerMsgStringSend(lMsg.Get(MSGGET(14, 85)), lpObj->m_Index, 1);
		PMSG_DUEL_START pMsgSend;
		pMsgSend.h.c = 0xC1;
		pMsgSend.h.headcode = 0xAA;
		pMsgSend.h.subcode = 0x01;
		pMsgSend.bDuelStart = 0x1e;
		pMsgSend.h.size = sizeof(pMsgSend);
		DataSend(lpObj->m_Index, (BYTE*)&pMsgSend, pMsgSend.h.size);
		return;
	}

	int iDuelRoom = this->GetFreeRoomIndex();
	if ( iDuelRoom == -1 )
	{
		GCServerMsgStringSend(lMsg.Get(MSGGET(14, 86)), lpObj->m_Index, 1);
		PMSG_DUEL_START pMsgSend;
		pMsgSend.h.c = 0xC1;
		pMsgSend.h.headcode = 0xAA;
		pMsgSend.h.subcode = 0x01;
		pMsgSend.bDuelStart = 0x10;
		pMsgSend.h.size = sizeof(pMsgSend);
		DataSend(lpObj->m_Index, (BYTE*)&pMsgSend, pMsgSend.h.size);
		return;
	}

	if ( ( GetTickCount() - lpObj->m_PacketCheckTime ) < 300 )
	{
		return;
	}
	lpObj->m_PacketCheckTime = GetTickCount();

	char szDuelName[MAX_ACCOUNT_LEN+1] = {0};
	char szDuelName2[MAX_ACCOUNT_LEN+1] = {0};

	memcpy(szDuelName, gObj[iDuelIndex].Name, MAX_ACCOUNT_LEN);
	szDuelName[MAX_ACCOUNT_LEN] = 0;

	memcpy(szDuelName2, lpMsg->szName, MAX_ACCOUNT_LEN);
	szDuelName2[MAX_ACCOUNT_LEN] = 0;

	if ( strcmp(szDuelName, szDuelName2) )
	{
		GCServerMsgStringSend(lMsg.Get(MSGGET(4, 162)), lpObj->m_Index, 1);
		return;
	}

	if ( this->IsDuelEnable(iDuelIndex) == FALSE )
	{
		GCServerMsgStringSend(lMsg.Get(MSGGET(4, 163)), lpObj->m_Index, 1);
		return;
	}

	if ( lpObj->lpGuild && lpObj->lpGuild->WarState == 1 )
	{
		GCServerMsgStringSend(lMsg.Get(MSGGET(4, 164)), lpObj->m_Index, 1);
		return;
	}

	if ( gObj[iDuelIndex].lpGuild && gObj[iDuelIndex].lpGuild->WarState == 1 )
	{
		GCServerMsgStringSend(lMsg.Get(MSGGET(4, 165)), lpObj->m_Index, 1);
		return;
	}

	if ( gObjIsConnected(iDuelIndex) == FALSE )
		return;

	if ( gObj[iDuelIndex].Type == OBJ_MONSTER )
		return;

	if ( gObj[iDuelIndex].CloseCount >= 0 )
		return;

	if ( gObj[iDuelIndex].m_bMapSvrMoveQuit == true || gObj[iDuelIndex].m_bMapAntiHackMove == true )
		return;

	for (int n=0;n<MAX_SELF_DEFENSE;n++)
	{
		if ( lpObj->SelfDefense[n] >= 0 || gObj[iDuelIndex].SelfDefense[n] >= 0 )
		{
			GCServerMsgStringSend(lMsg.Get(MSGGET(4, 189)), lpObj->m_Index, 1);
			return;
		}
	}

	if ((lpObj->TargetNumber!=-1)&&(lpObj->pTransaction==1))
	{
		GCServerMsgStringSend(lMsg.Get(MSGGET(4, 167)), lpObj->m_Index, 1);
		return;
	}

	if ((gObj[iDuelIndex].TargetNumber!=-1)&&(gObj[iDuelIndex].pTransaction==1))
	{
		GCServerMsgStringSend(lMsg.Get(MSGGET(4, 167)), lpObj->m_Index, 1);
		return;
	}

	if ( lpObj->m_IfState.use > 0 )
	{
		GCServerMsgStringSend(lMsg.Get(MSGGET(4, 167)), lpObj->m_Index, 1);
		return;
	}

	if ( gObj[iDuelIndex].m_IfState.use > 0 )
	{
		wsprintf(szTempText, lMsg.Get(MSGGET(4, 168)), gObj[iDuelIndex].Name);
		GCServerMsgStringSend(szTempText, lpObj->m_Index, 1);
		return;
	}

	if ( OBJMAX_RANGE(gObj[iDuelIndex].m_iDuelUserRequested) )
	{
		wsprintf(szTempText, lMsg.Get(MSGGET(4, 169)), gObj[iDuelIndex].Name);
		GCServerMsgStringSend(szTempText, lpObj->m_Index, 1);
		return;
	}

	if ( OBJMAX_RANGE(gObj[iDuelIndex].m_iDuelUserReserved) )
	{
		wsprintf(szTempText, lMsg.Get(MSGGET(4, 170)), gObj[iDuelIndex].Name);
		GCServerMsgStringSend(szTempText, lpObj->m_Index, 1);
		return;
	}

	if ( OBJMAX_RANGE(gObj[iDuelIndex].m_iDuelUser) )
	{
		wsprintf(szTempText, lMsg.Get(MSGGET(4, 171)), gObj[iDuelIndex].Name);
		GCServerMsgStringSend(szTempText, lpObj->m_Index, 1);
		return;
	}

	lpObj->m_iDuelUser = -1;
	lpObj->m_iDuelUserReserved = iDuelIndex;
	gObj[iDuelIndex].m_iDuelUserRequested = lpObj->m_Index;
	lpObj->m_iDuelRoom = iDuelRoom;
	gObj[iDuelIndex].m_iDuelRoom = iDuelRoom;

	this->m_Rooms[iDuelRoom].lpObj01 = lpObj;
	this->m_Rooms[iDuelRoom].lpObj02 = &gObj[iDuelIndex];
	this->m_Rooms[iDuelRoom].bWaiting = true;
	this->m_Rooms[iDuelRoom].dwWatingTime = GetTickCount();

	PMSG_DUEL_QUESTION_START pMsg;

	pMsg.h.c = 0xC1;
	pMsg.h.headcode = 0xAA;
	pMsg.h.subcode = 0x02;
	pMsg.h.size = sizeof(pMsg);
	pMsg.NumberH = SET_NUMBERH(lpObj->m_Index);
	pMsg.NumberL = SET_NUMBERL(lpObj->m_Index);
	memcpy(pMsg.szName, lpObj->Name, sizeof(pMsg.szName));

	DataSend(iDuelIndex, (LPBYTE)&pMsg, pMsg.h.size);

	wsprintf(szTempText, lMsg.Get(MSGGET(4, 172)), gObj[iDuelIndex].Name);
	GCServerMsgStringSend(szTempText, lpObj->m_Index, 1);

	LogAddTD("[Duel Manager] [%s][%s] Requested to Start Duel to [%s][%s] on Room [%d]", lpObj->AccountID, lpObj->Name, gObj[iDuelIndex].AccountID, gObj[iDuelIndex].Name, iDuelRoom+1);
}

bool CDuelManager::IsDuelEnable(int aIndex)
{

	if ( OBJMAX_RANGE(aIndex ) == FALSE)
	{
		return false;
	}

	if ( gObjIsConnected(aIndex) == TRUE )
	{
		if ( ( gObj[aIndex].m_Option & 2 ) == 2 )
		{
			return true;
		}
	}

	return false;
}

void CDuelManager::SetDuelOption(int lpObj, BOOL bState) 
{
	if ( gObjIsConnected(lpObj) == TRUE )
	{
		if ( bState == 0 )
		{
			gObj[lpObj].m_Option = 0;
		}
		else
		{
			gObj[lpObj].m_Option |= 2;
		}
	}
}

BOOL CDuelManager::gObjDuelCheck(LPOBJ lpObj)
{
	BOOL bRetVal = FALSE;

	if ( lpObj->Type == OBJ_USER )
	{
		int iDuelIndex = lpObj->m_iDuelUser;
		if (  OBJMAX_RANGE(iDuelIndex) )
		{
			bRetVal = TRUE;
		}
	}

	return bRetVal;
}

BOOL CDuelManager::gObjDuelCheck(LPOBJ lpObj, LPOBJ lpTargetObj)
{
	BOOL bRetVal = FALSE;

	if ( lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER)
	{
		if ( lpObj->m_iDuelUser == lpTargetObj->m_Index && lpTargetObj->m_iDuelUser == lpObj->m_Index)
		{
			bRetVal = TRUE;
		}
	}

	return bRetVal;
}


void CDuelManager::gObjCheckAllUserDuelStop()
{
	for(int i = OBJ_STARTUSERINDEX; i < OBJMAX; i++)
	{
		if(gObj[i].Connected == PLAYER_PLAYING)
		{
			if(gObj[i].Type == OBJ_USER)
			{
				if(OBJMAX_RANGE(gObj[i].m_iDuelUser))
				{
					if(GetTickCount() - gObj[i].m_iDuelTickCount > 60000)
					{
						this->RoomReset(gObj[i].m_iDuelRoom);
						g_DuelManager.gObjSendDuelEnd(&gObj[gObj[i].m_iDuelUser]);
						g_DuelManager.gObjSendDuelEnd(&gObj[i]);
						g_DuelManager.gObjResetDuel(&gObj[i]);
					}
				}
			}
		}
	}
}

bool CDuelManager::IsOnDuel(int lpObj) 
{ 
	for(int i = 0; i < MAX_DUEL_ROOMS; i++)
	{
		if(this->m_Rooms[i].lpObj01 == &gObj[lpObj])
		{
			return true;
		}
		if(this->m_Rooms[i].lpObj02 == &gObj[lpObj]) 
		{
			return true;
		}
	}

	return false;
}

bool CDuelManager::IsOnDuel(int lpObj, int lpObj2) 
{
	for(int i = 0; i < MAX_DUEL_ROOMS; i++)
	{
		if(this->m_Rooms[i].bFree == false)
		{

			if(this->m_Rooms[i].lpObj01 == &gObj[lpObj] && 
				this->m_Rooms[i].lpObj02 == &gObj[lpObj2]) 
			{
				return true;
			}

			if(this->m_Rooms[i].lpObj01 == &gObj[lpObj2] && 
				this->m_Rooms[i].lpObj02 == &gObj[lpObj]) 
			{
				return true;
			}
		}
	}

	return false;
}

BOOL CDuelManager::NPCTalk_GateKeeper(LPOBJ lpNpc,LPOBJ lpObj)
{
	PMSG_TALKRESULT pResult;
	if ( lpNpc->MapNumber != lpObj->MapNumber )
		return false;

	//if ( lpObj->m_IfState.use > 0 )
	//	return false;

	if ((lpObj->TargetNumber!=-1)&&(lpObj->pTransaction==1))
		return false;

	
	if ( (lpObj->pInventory[10].IsItem() == TRUE && lpObj->pInventory[10].m_Type == ITEMGET(13,68)) || (lpObj->pInventory[11].IsItem() == TRUE && lpObj->pInventory[11].m_Type == ITEMGET(13,68)))
	{
		ChatTargetSend(lpNpc, "You cant see duels with transformation ring", lpObj->m_Index);
		return FALSE;
	}

	lpObj->m_IfState.type = 20;
	lpObj->m_IfState.state = 0;
	lpObj->m_IfState.use = 1;

	g_DuelManager.SendDuelStatus(lpObj);

	pResult.h.c = 0xC3;
	pResult.h.headcode = 0x30;
	pResult.h.size = sizeof(pResult);
	pResult.result = 0x21;

	DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);

	return TRUE;
}

