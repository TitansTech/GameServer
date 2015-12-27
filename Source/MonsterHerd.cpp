// ------------------------------
// Decompiled by Deathway
// Date : 2007-03-09
// ------------------------------
#include "stdafx.h"
#include "MonsterHerd.h"
#include "MapClass.h"
#include "GameMain.h"
#include "gObjMonster.h"
// GS-N 0.99.60T 0x004084D0
//	GS-N	1.00.18	JPN	0x0040B8F0	-	Completed

MonsterHerd::MonsterHerd()
{
	this->m_iMapNumber =-1;
	this->m_iCUR_X =0;
	this->m_iCUR_Y =0;
	this->m_iRADIUS =0;
	this->m_bHerdActive =0;
	this->m_bHasInfo=0;
	
	InitializeCriticalSection(&this->m_critMonsterHerd);
}





MonsterHerd::~MonsterHerd()
{
	DeleteCriticalSection(&this->m_critMonsterHerd);
}





BOOL MonsterHerd::SetTotalInfo(int iMapNumber, int iRadius, int iStartX, int iStartY)
{
	if ( MAX_MAP_RANGE(iMapNumber) == FALSE ) 
	{
		return FALSE;
	}

	if ( ((iStartX < 0)? FALSE : (iStartX > 254 )? FALSE : TRUE ) == FALSE || ( ( iStartY < 0)?FALSE : (iStartY > 254)? FALSE : TRUE) == FALSE)
	{
		return FALSE;
	}

	BYTE btMapAttr = MapC[iMapNumber].GetAttr(iStartX, iStartY);

	if ( ( btMapAttr & 1) == 1  || (btMapAttr & 4) == 4 || (btMapAttr&8)==8)
	{
		return FALSE;
	}

	if ( ( ( iRadius < 0 )? FALSE : ( iRadius > 15 )? FALSE : TRUE ) == FALSE )
	{
		return FALSE;
	}

	this->m_iMapNumber = iMapNumber;
	this->m_iCUR_X = iStartX;
	this->m_iCUR_Y = iStartY;
	this->m_iRADIUS = iRadius;
	this->m_bHasInfo = 1;
	return TRUE;
}





BOOL MonsterHerd::AddMonster(int iMonsterType, BYTE bRegen, BYTE bAttackFirst)
{
	if ( this->m_bHasInfo == 0 )
	{
		return false;
	}

	int iIndex;
	BYTE cX=0;
	BYTE cY=0;

	if (this->GetRandomLocation(cX, cY) == FALSE )
	{
		return false;
	}

	iIndex = gObjAddMonster(this->m_iMapNumber);

	if ( iIndex >= 0 )
	{
		gObj[iIndex].m_PosNum = -1;
		gObj[iIndex].X = cX;
		gObj[iIndex].Y = cY;
		gObj[iIndex].MapNumber = this->m_iMapNumber;
		gObj[iIndex].TX = gObj[iIndex].X;
		gObj[iIndex].TY = gObj[iIndex].Y;
		gObj[iIndex].m_OldX = gObj[iIndex].X;
		gObj[iIndex].m_OldY = gObj[iIndex].Y;
		gObj[iIndex].StartX = gObj[iIndex].X;
		gObj[iIndex].StartY = gObj[iIndex].Y;

		LPMONSTER_ATTRIBUTE iAttr = gMAttr.GetAttr(iMonsterType);

		if ( iAttr == NULL )
		{
			gObjDel(iIndex);
			return false;
		}

		gObj[iIndex].Level = iAttr->m_Level;
		gObjSetMonster(iIndex, iMonsterType,"MonsterHerd::AddMonster");
		gObj[iIndex].MaxRegenTime = 1000;
		gObj[iIndex].Dir = rand() % 8;
		gObj[iIndex].m_bIsInMonsterHerd = true;
		gObj[iIndex].m_bIsMonsterAttackFirst = bAttackFirst;
		gObj[iIndex].m_lpMonsterHerd = this;

		EnterCriticalSection(&this->m_critMonsterHerd);

		_MONSTER_HERD_DATA pMonsterData;
		pMonsterData.m_iIndex = iIndex;
		pMonsterData.m_iType = iMonsterType;
		pMonsterData.m_iX = cX;
		//pMonsterData.m_iX = cY;	// #error Apply Deathway Fix, change btXMap for btYMap
		pMonsterData.m_iY = cY;	// #FIX
		pMonsterData.m_bRegen = bRegen;

		this->m_mapMonsterHerd.insert( std::pair<int, _MONSTER_HERD_DATA>(iIndex, pMonsterData) );

		LeaveCriticalSection(&this->m_critMonsterHerd);
	}
	else
	{
		return FALSE;
	}

	return TRUE;

}




void MonsterHerd::SetRadius(int iRadius)
{
	if ( ((iRadius<0)?FALSE:(iRadius>15)?FALSE:TRUE) != FALSE )
	{
		return;
	}

	this->m_iRADIUS = iRadius;
}





void MonsterHerd::SetPosition(BYTE iTX, BYTE iTY)
{
	if ( this->m_bHasInfo == 0 )
	{
		return;
	}

	BYTE btMapAttr = MapC[this->m_iMapNumber].GetAttr(iTX, iTY);

	if ( (btMapAttr&1) == 1 || (btMapAttr&4)==4 || (btMapAttr&8) == 8 )
	{
		return;
	}

	this->m_iCUR_X = iTX;
	this->m_iCUR_Y = iTY;
}





BOOL MonsterHerd::Start()
{
	if ( this->m_bHerdActive != 0 )
	{
		return FALSE;
	}

	if ( this->m_bHasInfo == 0 )
	{
		return FALSE;
	}

	this->m_bHerdActive = 1;
	return TRUE;
}




void MonsterHerd::Stop()
{
	this->m_bHerdActive = 0;

	if ( this->m_mapMonsterHerd.empty() == false )
	{
		EnterCriticalSection(&this->m_critMonsterHerd);

		if ( this->m_mapMonsterHerd.empty() != false )
		{
			LeaveCriticalSection(&this->m_critMonsterHerd);
		}
		else
		{
			for ( std::map<int, _MONSTER_HERD_DATA>::iterator _Iter = this->m_mapMonsterHerd.begin() ; _Iter != this->m_mapMonsterHerd.end() ; _Iter++)
			{
				_MONSTER_HERD_DATA & pMH_DATA = _Iter->second;
				gObjDel(pMH_DATA.m_iIndex);
			}

			this->m_mapMonsterHerd.erase( this->m_mapMonsterHerd.begin(), this->m_mapMonsterHerd.end() );
			LeaveCriticalSection(&this->m_critMonsterHerd);
		}
	}

}



_MONSTER_HERD_DATA * MonsterHerd::GetMonsterData(int iIndex)
{
	if ( OBJMAX_RANGE(iIndex) == FALSE )
	{
		return NULL;
	}

	LPOBJ lpOBJ = &gObj[iIndex];

	if ( lpOBJ->Connected < PLAYER_PLAYING || lpOBJ->Type != OBJ_MONSTER )
	{
		return NULL;
	}

	MonsterHerd * lpMH = (MonsterHerd *)lpOBJ->m_lpMonsterHerd;

	if ( lpMH == NULL )
	{
		return NULL;
	}

	_MONSTER_HERD_DATA * lpMHD_RETURN = NULL;

	EnterCriticalSection(&lpMH->m_critMonsterHerd);
	
	std::map<int, _MONSTER_HERD_DATA>::iterator it = lpMH->m_mapMonsterHerd.find(iIndex);

	if ( it != lpMH->m_mapMonsterHerd.end() )
	{
		lpMHD_RETURN = &it->second;
	}

	LeaveCriticalSection(&lpMH->m_critMonsterHerd);

	return lpMHD_RETURN;
}



void MonsterHerd::BeenAttacked(LPOBJ lpObj, LPOBJ lpTargetObj)
{
	if ( lpObj == NULL )
	{
		return;
	}

	if ( lpObj->Connected < PLAYER_PLAYING || lpObj->Type != OBJ_MONSTER )
	{
		return;
	}

	if ( lpTargetObj->Connected < PLAYER_PLAYING )
	{
		return;
	}

	if ( this->m_bHerdActive == 0 )
	{
		return;
	}

	for ( std::map<int, _MONSTER_HERD_DATA>::iterator it = this->m_mapMonsterHerd.begin(); it != this->m_mapMonsterHerd.end() ; it++)
	{
		_MONSTER_HERD_DATA * lpMHD = &it->second;

		if (  OBJMAX_RANGE(lpMHD->m_iIndex)== FALSE )
		{
			continue;
		}

		if ( gObj[lpMHD->m_iIndex].Live == FALSE || gObj[lpMHD->m_iIndex].m_State != 2 )
		{
			continue;
		}

		if ( gObj[lpMHD->m_iIndex].m_lpMonsterHerd != lpObj->m_lpMonsterHerd )
		{
			continue;
		}

		if ( lpMHD->m_iIndex == lpObj->m_Index )
		{
			continue;
		}

		if ( gObj[lpMHD->m_iIndex].TargetNumber == -1 )
		{
			if ( (rand()%100) < 70 )
			{
				gObj[lpMHD->m_iIndex].TargetNumber = lpTargetObj->m_Index;
				gObj[lpMHD->m_iIndex].m_ActState.Emotion = 1;
				gObj[lpMHD->m_iIndex].m_ActState.EmotionCount = 50;
				continue;
			}
		}
		else if ( (rand()%100) < 30 )
		{
			gObj[lpMHD->m_iIndex].TargetNumber = lpTargetObj->m_Index;
			gObj[lpMHD->m_iIndex].m_ActState.Emotion = 1;
			gObj[lpMHD->m_iIndex].m_ActState.EmotionCount = 50;
			continue;
		}
	}
}




void MonsterHerd::OrderAttack(LPOBJ lpObj, LPOBJ lpTargetObj, int iAttackPercent)
{
	if ( lpObj == NULL )
	{
		return;
	}

	if ( lpObj->Connected < PLAYER_PLAYING || lpObj->Type != OBJ_MONSTER )
	{
		return;
	}

	if ( lpTargetObj->Connected < PLAYER_PLAYING )
	{
		return;
	}

	if ( this->m_bHerdActive == 0 )
	{
		return;
	}

	if ( ((iAttackPercent<0)?FALSE:(iAttackPercent>100)?FALSE:TRUE) == FALSE )
	{
		return;
	}

	for ( std::map<int, _MONSTER_HERD_DATA>::iterator it = this->m_mapMonsterHerd.begin(); it != this->m_mapMonsterHerd.end() ; it++)
	{
		_MONSTER_HERD_DATA * lpMHD = &it->second;

		if ( OBJMAX_RANGE(lpMHD->m_iIndex)  == FALSE )
		{
			continue;
		}

		if ( gObj[lpMHD->m_iIndex].Live == FALSE || gObj[lpMHD->m_iIndex].m_State != 2 )
		{
			continue;
		}

		if ( gObj[lpMHD->m_iIndex].m_lpMonsterHerd != lpObj->m_lpMonsterHerd )
		{
			continue;
		}

		if ( lpMHD->m_iIndex == lpObj->m_Index )
		{
			continue;
		}

		if ( (rand()%100) < iAttackPercent )
		{
			gObj[lpMHD->m_iIndex].TargetNumber = lpTargetObj->m_Index;
			gObj[lpMHD->m_iIndex].m_ActState.Emotion = 1;
			gObj[lpMHD->m_iIndex].m_ActState.EmotionCount = 50;

			continue;
		}
	}
}



BOOL MonsterHerd::CheckInRadius(int iIndex)
{
	if ( OBJMAX_RANGE(iIndex) == FALSE )
	{
		return FALSE;
	}

	LPOBJ lpOBJ = &gObj[iIndex];

	if ( lpOBJ->Connected < PLAYER_PLAYING || lpOBJ->Type != OBJ_MONSTER )
	{
		return FALSE;
	}

	int iX2 = (lpOBJ->X - this->m_iCUR_X) * (lpOBJ->X - this->m_iCUR_X);
	int iY2 = (lpOBJ->Y - this->m_iCUR_Y) * (lpOBJ->Y - this->m_iCUR_Y);

	if ( (this->m_iRADIUS * this->m_iRADIUS) < (iX2+iY2) )
	{
		return FALSE;
	}

	return TRUE;
}






BOOL MonsterHerd::GetCurrentLocation(BYTE&cX, BYTE& cY)
{
	if ( this->m_bHasInfo == 0 )
	{
		return FALSE;
	}

	cX = this->m_iCUR_X;
	cY = this->m_iCUR_Y ;

	return TRUE;
}



BOOL MonsterHerd::GetRandomLocation(BYTE& cX, BYTE& cY)
{
	if ( this->m_bHasInfo == 0 )
	{
		return FALSE;
	}

	int iCount = 100;

	while ( iCount-- != 0)
	{
		cX = rand()%(this->m_iRADIUS+1) * (((rand()%2==0)?-1:1)) + this->m_iCUR_X;
		cY = rand()%(this->m_iRADIUS+1) * (((!(rand()%2)))?-1:1) + this->m_iCUR_Y;

		BYTE attr = MapC[this->m_iMapNumber].GetAttr(cX, cY);

		if ( attr == 0 )
		{
			return TRUE;
		}
	}

	return FALSE;
}




BOOL MonsterHerd::CheckLocation(BYTE& cX, BYTE& cY)
{
	BYTE btMapAttr = MapC[this->m_iMapNumber].GetAttr(cX, cY);

	if ( (btMapAttr&1) == 1 || (btMapAttr&4) == 4 || (btMapAttr&8) == 8 )
	{
		return FALSE;
	}

	return TRUE;
}




BOOL MonsterHerd::MoveHerd(BYTE iTX, BYTE iTY)
{
	if ( this->m_bHasInfo == 0 )
	{
		return FALSE;
	}

	BYTE btMapAttr = MapC[this->m_iMapNumber].GetAttr(iTX, iTY);

	if ( (btMapAttr&1) == 1 || (btMapAttr&4) == 4 || (btMapAttr&8) == 8 )
	{
		return FALSE;
	}

	this->m_iCUR_X = iTX;
	this->m_iCUR_Y = iTY;

	return TRUE;
}




BOOL MonsterHerd::MonsterHerdItemDrop(LPOBJ lpObj)
{
	return FALSE;	// NO MACRO
}


void MonsterHerd::MonsterMoveAction(LPOBJ lpObj)
{
	if ( lpObj->m_SkillHarden != 0 )
	{
		return;
	}

	if ( lpObj == NULL )
	{
		return;
	}

	if ( lpObj->Connected < PLAYER_PLAYING || lpObj->Type != OBJ_MONSTER )
	{
		return;
	}

	int searchc = 10;
	BYTE tpx;
	BYTE tpy;

	lpObj->NextActionTime = 1000;

	while ( searchc-- != 0 )
	{
		BOOL ret = this->GetRandomLocation(tpx, tpy);

		if ( ret != FALSE )
		{
			lpObj->TargetNumber = -1;
			lpObj->m_ActState.Attack = 0;
			lpObj->NextActionTime = 500;
			lpObj->m_ActState.Emotion = 0;
			lpObj->MTX = tpx;
			lpObj->MTY = tpy;
			lpObj->m_ActState.Move = 1;

			break;
		}
	}
}


void MonsterHerd::MonsterAttackAction(LPOBJ lpObj, LPOBJ lpTargetObj)
{
	return;	// Here goes a MACRO
}

//*******************************************************
// START REVIEW HERE


void MonsterHerd::MonsterBaseAct(LPOBJ lpObj)
{
	LPOBJ lpTargetObj = NULL;

	if ( lpObj->TargetNumber >= 0 )
	{
		lpTargetObj = &gObj[lpObj->TargetNumber];
	}
	else
	{
		lpObj->m_ActState.Emotion = 0;
	}

	switch ( lpObj->m_ActState.Emotion )
	{
		case 0:
			{
				if ( lpObj->m_ActState.Attack != 0 )
				{
					lpObj->m_ActState.Attack = 0;
					lpObj->TargetNumber = -1;
					lpObj->NextActionTime = 500;
				}

				int actcode1 = rand() % 2;

				if ( actcode1 == 0 )
				{
					lpObj->m_ActState.Rest = 1;
					lpObj->NextActionTime = 500;
				}
				else if ( lpObj->m_MoveRange > 0 )
				{
					if ( lpObj->m_SkillHarden == 0 )
					{
						this->MonsterMoveAction(lpObj);
					}
				}

				if ( lpObj->m_bIsMonsterAttackFirst != 0 )
				{
					lpObj->TargetNumber = gObjMonsterSearchEnemy(lpObj, OBJ_USER);

					if ( lpObj->TargetNumber >= 0 )
					{
						lpObj->m_ActState.EmotionCount = 30;
						lpObj->m_ActState.Emotion = 1;
					}
				}
			}
			break;

		case 1:

			if ( lpObj->m_ActState.EmotionCount > 0 )
			{
				lpObj->m_ActState.EmotionCount --;
			}
			else
			{
				lpObj->m_ActState.Emotion = 0;
			}

			if ( lpObj->TargetNumber >= 0 && lpObj->PathStartEnd == 0 )
			{
				int dis = gObjCalDistance(lpObj, lpTargetObj);
				int attackRange;

				if ( lpObj->m_AttackType >= 100 )
				{
					attackRange = lpObj->m_AttackRange + 2;
				}
				else
				{
					attackRange = lpObj->m_AttackRange;
				}

				if ( dis <= attackRange )
				{
					int tuser = lpObj->TargetNumber;
					int map = gObj[tuser].MapNumber;

					if ( MapC[map].CheckWall(lpObj->X, lpObj->Y, gObj[tuser].X, gObj[tuser].Y) == TRUE )
					{
						BYTE attr = MapC[map].GetAttr(gObj[tuser].X, gObj[tuser].Y);

						if ( (attr&1) != 1 )
						{
							lpObj->m_ActState.Attack = 1;
							lpObj->m_ActState.EmotionCount = (rand()%30+20);
						}
						else
						{
							lpObj->TargetNumber = -1;
							lpObj->m_ActState.EmotionCount = 30;
							lpObj->m_ActState.Emotion = 1;
						}

						lpObj->Dir = GetPathPacketDirPos(lpTargetObj->X-lpObj->X, lpTargetObj->Y-lpObj->Y);
						lpObj->NextActionTime = lpObj->m_AttackSpeed;
					}
				}
				else
				{
					if ( gObjMonsterGetTargetPos(lpObj) == TRUE )
					{
						if ( MapC[lpObj->MapNumber].CheckWall(lpObj->X, lpObj->Y, lpObj->MTX, lpObj->MTY) == TRUE )
						{
							lpObj->m_ActState.Move = 1;
							lpObj->NextActionTime = 400;
							lpObj->Dir = GetPathPacketDirPos(lpTargetObj->X - lpObj->X, lpTargetObj->Y-lpObj->Y);
						}
						else
						{
							this->MonsterMoveAction(lpObj);
							lpObj->m_ActState.Emotion = 3;
							lpObj->m_ActState.EmotionCount = 10;
						}
					}
					else
					{
						this->MonsterMoveAction(lpObj);
						lpObj->m_ActState.Emotion = 3;
						lpObj->m_ActState.EmotionCount = 10;
					}
				}
			}

			break;

		case 3:

			if ( lpObj->m_ActState.EmotionCount > 0 )
			{
				lpObj->m_ActState.EmotionCount--;
			}
			else
			{
				lpObj->m_ActState.Emotion = 0;
			}

			lpObj->m_ActState.Move = 0;
			lpObj->m_ActState.Attack = 0;
			lpObj->NextActionTime = 400;

			break;
	}
}
	


void MonsterHerd::MonsterDieAction(LPOBJ lpObj)
{
	return;	// Here goes a MACRO
}


BOOL MonsterHerd::MonsterRegenAction(LPOBJ lpObj)
{
	if ( lpObj == NULL )
	{
		return FALSE;
	}

	if ( lpObj->Connected < PLAYER_PLAYING || lpObj->Type != OBJ_MONSTER )
	{
		return FALSE;
	}

	MonsterHerd * lpMH = (MonsterHerd *)lpObj->m_lpMonsterHerd;

	if ( lpMH == NULL )
	{
		return FALSE;
	}

	_MONSTER_HERD_DATA * lpMHD = lpMH->GetMonsterData(lpObj->m_Index);

	if ( lpMHD == NULL || lpMHD->m_bRegen == 0 )
	{
		return FALSE;
	}

	lpObj->Life = lpObj->AddLife + lpObj->MaxLife;
	lpObj->Mana = lpObj->AddMana + lpObj->MaxMana;
	lpObj->Live = TRUE;
	lpObj->m_PoisonBeattackCount = 0;
	lpObj->m_ColdBeattackCount = 0;
	//lpObj->m_ViewState = 0;
	lpObj->Teleport = 0;

	for ( int i=0;i<MAX_SELF_DEFENSE;i++)
	{
		lpObj->SelfDefenseTime[i] = 0;
	}

	gObjTimeCheckSelfDefense(lpObj);
	lpObj->m_ViewSkillState &= 0xFFFFFFFE;
	lpObj->m_ViewSkillState &= 0xFFFFFFFD;

	gObjClearViewport(lpObj);
	gObjViewportListProtocolDestroy(lpObj);
	gObjViewportClose(lpObj);

	lpObj->m_ActState.Attack = 0;
	lpObj->m_ActState.EmotionCount = 0;
	lpObj->m_ActState.Escape = 0;
	lpObj->m_ActState.Move = 0;
	lpObj->m_ActState.Rest = 0;
	lpObj->m_ActState.Emotion = 0;
	lpObj->TargetNumber = -1;
	lpObj->NextActionTime = 5000;

	BYTE cX=0;
	BYTE cY=0;
	int iCount = 100;
	BOOL bGetPosition = FALSE;

	while ( iCount-- != 0 )
	{
		if ( lpMH->GetRandomLocation(cX, cY) != FALSE )
		{
			bGetPosition = TRUE;
			break;
		}
	}

	if ( bGetPosition == FALSE )
	{
		lpObj->Live = FALSE;
		lpObj->m_State = 4;
		lpObj->RegenTime = GetTickCount();
		lpObj->DieRegen = TRUE;

		return FALSE;
	}

	lpObj->X = cX;
	lpObj->Y = cY;
	lpObj->MTX = lpObj->X;
	lpObj->MTY = lpObj->Y;
	lpObj->TX = lpObj->X;
	lpObj->TY = lpObj->Y;
	lpObj->StartX = lpObj->X;
	lpObj->StartY = lpObj->Y;
		//lpObj->m_OldX = lpObj->X;
		//lpObj->m_OldY = lpObj->Y;

	gObjMonsterHitDamageInit(lpObj);
	CreateFrustrum(lpObj->X, lpObj->Y, lpObj->m_Index);

	lpObj->DieRegen = FALSE;
	lpObj->m_State = 1;

	return TRUE;
}


