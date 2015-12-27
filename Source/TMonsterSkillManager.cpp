// TMonsterSkillManager.cpp: implementation of the TMonsterSkillManager class.
//	GS-N	1.00.18	JPN	0x00558440	-	Completed
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TMonsterSkillManager.h"

#include "ObjUseSkill.h"
#include "SkillHitBox.h"

#include "..\include\ReadScript.h"
#include "LogProc.h"


TMonsterSkillInfo TMonsterSkillManager::s_MonsterSkillInfoArray[MAX_MONSTER_SKILL_INFO_ARRAY];
_ST_MONSTER_SKILL_DELAYTIME_INFO TMonsterSkillManager::s_MonsterSkillDelayInfoArray[MAX_MONSTER_SKILL_DELAY_INFO_ARRAY];
TSync TMonsterSkillManager::s_Sync;
BOOL TMonsterSkillManager::s_bDataLoad;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TMonsterSkillManager::TMonsterSkillManager()
{
	return;
}

TMonsterSkillManager::~TMonsterSkillManager()
{
	return;
}


BOOL TMonsterSkillManager::LoadData(LPSTR lpszFileName)
{
	TMonsterSkillManager::s_bDataLoad = FALSE;

	if ( !lpszFileName || !strcmp(lpszFileName, ""))
	{
		MsgBox("[Monster Skill Manager] - File load error : File Name Error");
		return FALSE;
	}

	try
	{
		SMDToken Token;
		SMDFile = fopen(lpszFileName, "r");

		if ( SMDFile == NULL )
		{
			MsgBox("[Monster Skill Manager] - Can't Open %s ", lpszFileName);
			return FALSE;
		}

		TMonsterSkillManager::DelAllSkillManagerInfo();
		int iType = -1;

		while ( true )
		{
			Token = GetToken();

			if ( Token == END )
				break;

			iType = TokenNumber;

			while ( true )
			{
				if ( iType == 0 )
				{
					BOOL bVerified = TRUE;
					char szMonsterName[20]={0};
					TMonsterSkillInfo MonsterSkillInfo;

					Token = GetToken();

					if ( !strcmp("end", TokenString))
						break;

					MonsterSkillInfo.m_iMonsterIndex = TokenNumber;

					Token = GetToken();
					memcpy(szMonsterName, TokenString, 20);

					for ( int i=0;i<5;i++)
					{
						int iSkillUnitNumber = -1;
						Token = GetToken();
						MonsterSkillInfo.m_iSkillUnitTypeArray[i] = TokenNumber;

						Token = GetToken();
						iSkillUnitNumber = TokenNumber;

						if ( iSkillUnitNumber == -1 )
							continue;

						TMonsterSkillUnit * lpSkillUnit = TMonsterSkillUnit::FindSkillUnit(iSkillUnitNumber);
						
						if ( lpSkillUnit == NULL )
						{
							bVerified = FALSE;
							continue;
						}

						MonsterSkillInfo.m_lpSkillUnitArray[i] = lpSkillUnit;
						MonsterSkillInfo.m_iSkillUnitCount++;
					}



					if ( false || MonsterSkillInfo.m_iMonsterIndex >= MAX_MONSTER_SKILL_INFO_ARRAY )
					{
						MsgBox("[Monster Manager] - MonsterIndex(%d) Error (%s) File. ", 
							MonsterSkillInfo.m_iMonsterIndex, lpszFileName);
						continue;
					}

					if ( bVerified ==FALSE )
					{
						LogAddC(2, "[Monster Manager] - Invalid SkillInfo : MIndex(%s/%d)",
							szMonsterName, MonsterSkillInfo.m_iMonsterIndex);//, lpszFileName);	// #error Remove the lpszFileNAme-->FIXED
					}

					memcpy(&TMonsterSkillManager::s_MonsterSkillInfoArray[MonsterSkillInfo.m_iMonsterIndex],
						&MonsterSkillInfo, sizeof(MonsterSkillInfo));

					char szMsg[200] = {0};
					TMonsterSkillInfo * pMonSInfo = &TMonsterSkillManager::s_MonsterSkillInfoArray[MonsterSkillInfo.m_iMonsterIndex];

					wsprintf(szMsg, "¡ÛIndex:%d,Name:%s,Count:%d,Type0:%d,Unit0:%d,Type1:%d,Unit1:%d,Type2:%d,Unit2:%d,Type3:%d,Unit3:%d,Type4:%d,Unit4:%d\n",
						pMonSInfo->m_iMonsterIndex, szMonsterName, pMonSInfo->m_iSkillUnitCount,
						pMonSInfo->m_iSkillUnitTypeArray[0], pMonSInfo->m_lpSkillUnitArray[0],
						pMonSInfo->m_iSkillUnitTypeArray[1], pMonSInfo->m_lpSkillUnitArray[1],
						pMonSInfo->m_iSkillUnitTypeArray[2], pMonSInfo->m_lpSkillUnitArray[2],
						pMonSInfo->m_iSkillUnitTypeArray[3], pMonSInfo->m_lpSkillUnitArray[3],
						pMonSInfo->m_iSkillUnitTypeArray[4], pMonSInfo->m_lpSkillUnitArray[4]);

					OutputDebugString(szMsg);
				}
			}
		}

		fclose(SMDFile);

		LogAddC(2, "[Monster Skill Manager] - %s file is Loaded", lpszFileName);

		TMonsterSkillManager::s_bDataLoad = TRUE;
	}
	catch(DWORD)
	{
		MsgBox("[Monster Skill Manager] - Loading Exception Error (%s) File. ", lpszFileName);
	}

	return FALSE;
}



void TMonsterSkillManager::DelAllSkillManagerInfo()
{
	for ( int i=0;i<MAX_MONSTER_SKILL_INFO_ARRAY;i++)
	{
		TMonsterSkillManager::s_MonsterSkillInfoArray[i].Reset();
	}

	for ( int j=0;j<MAX_MONSTER_SKILL_DELAY_INFO_ARRAY;j++)
	{
		TMonsterSkillManager::s_MonsterSkillDelayInfoArray[j].RESET();
	}
}

BOOL TMonsterSkillManager::CheckMonsterSkill(int iMonsterClass)
{
	if ( iMonsterClass < 0 || iMonsterClass > MAX_MONSTER_SKILL_INFO_ARRAY )	// #error Change to MAX_MONSTER_SKILL_INFO_ARRAY-1
		return FALSE;

	TMonsterSkillInfo * lpMonsterSkillInfo = &TMonsterSkillManager::s_MonsterSkillInfoArray[iMonsterClass];

	if ( lpMonsterSkillInfo->IsValid() == FALSE )
		return FALSE;

	return TRUE;
}


BOOL TMonsterSkillManager::AddMonsterSkillDelayInfo(int iIndex, int iTargetIndex, int iDelayTime, TMonsterSkillUnit * lpMonsterSkillUnit)
{
	for ( int i=0;i<MAX_MONSTER_SKILL_DELAY_INFO_ARRAY;i++)
	{
		if ( TMonsterSkillManager::s_MonsterSkillDelayInfoArray[i].bIsUsed == FALSE )
		{
			TMonsterSkillManager::s_MonsterSkillDelayInfoArray[i].iIndex = iIndex;
			TMonsterSkillManager::s_MonsterSkillDelayInfoArray[i].iTargetIndex = iTargetIndex;
			TMonsterSkillManager::s_MonsterSkillDelayInfoArray[i].dwDelayTime = GetTickCount() + iDelayTime;
			TMonsterSkillManager::s_MonsterSkillDelayInfoArray[i].lpMonsterSkillUnit = lpMonsterSkillUnit;
			TMonsterSkillManager::s_MonsterSkillDelayInfoArray[i].bIsUsed = TRUE;

			return TRUE;
		}
	}

	LogAddC(2, "[Monster Skill Manager] - (MonsterSkillDelayInfo) Slot Full!! ");
	return FALSE;
}



void TMonsterSkillManager::MonsterSkillProc() 
{
	DWORD dwCurrentTick = GetTickCount();

	for ( int i=0;i<MAX_MONSTER_SKILL_DELAY_INFO_ARRAY;i++)
	{
		if ( TMonsterSkillManager::s_MonsterSkillDelayInfoArray[i].bIsUsed == TRUE )
		{
			if ( dwCurrentTick < TMonsterSkillManager::s_MonsterSkillDelayInfoArray[i].dwDelayTime )
			{
				_ST_MONSTER_SKILL_DELAYTIME_INFO & stInfo = TMonsterSkillManager::s_MonsterSkillDelayInfoArray[i];

				if ( !stInfo.IsValid() )
				{
					TMonsterSkillManager::s_MonsterSkillDelayInfoArray[i].RESET();
					continue;
				}

				LPOBJ lpObj = &gObj[stInfo.iIndex];
				LPOBJ lpTargetObj = &gObj[stInfo.iTargetIndex];

				if ( !lpObj->Live || !lpTargetObj->Live )
				{
					TMonsterSkillManager::s_MonsterSkillDelayInfoArray[i].RESET();
					continue;
				}

				if ( !gObjIsConnected(lpTargetObj))
				{
					TMonsterSkillManager::s_MonsterSkillDelayInfoArray[i].RESET();
					continue;
				}

				gObjAttack(lpObj, lpTargetObj, 0, 0, 0, 0, 0);
				stInfo.lpMonsterSkillUnit->RunSkill(stInfo.iIndex, stInfo.iTargetIndex);
				TMonsterSkillManager::s_MonsterSkillDelayInfoArray[i].RESET();
			}
		}
	}
}


TMonsterSkillUnit * TMonsterSkillManager::FindMonsterSkillUnit(int iIndex, int iMonsterSkillUnitType)
{
	LPOBJ lpObj = &gObj[iIndex];
	TMonsterSkillInfo * lpMonsterSkillInfo = &TMonsterSkillManager::s_MonsterSkillInfoArray[lpObj->Class];

	if ( lpMonsterSkillInfo->IsValid() == FALSE )
		return NULL;

	int iFoundSkillArray[5] = {-1};
	int iFoundSkillCount = 0;

	for(int i=0;i<5;i++)
	{
		if ( iMonsterSkillUnitType == lpMonsterSkillInfo->m_iSkillUnitTypeArray[i] )
		{
			iFoundSkillArray[iFoundSkillCount] = i;
			iFoundSkillCount++;
		}
	}

	if ( iFoundSkillCount > 0 )
	{
		int iSlot = iFoundSkillArray[rand() % iFoundSkillCount];

		if ( iSlot == -1 )
			return NULL;

		TMonsterSkillUnit * lpMonsterSkillUnit = lpMonsterSkillInfo->m_lpSkillUnitArray[iSlot];
		
		return lpMonsterSkillUnit;
	}
	
	return NULL;
}



void TMonsterSkillManager::UseMonsterSkill(int iIndex, int iTargetIndex, int iMonsterSkillUnitType)
{
	LPOBJ lpObj = &gObj[iIndex];
	LPOBJ lpTargetObj = &gObj[iTargetIndex];
	TMonsterSkillInfo * lpMonsterSkillInfo = &TMonsterSkillManager::s_MonsterSkillInfoArray[lpObj->Class];

	if ( lpMonsterSkillInfo->IsValid() == FALSE )
		return;
	
	TMonsterSkillUnit * lpMonsterSkillUnit = TMonsterSkillManager::FindMonsterSkillUnit(iIndex, iMonsterSkillUnitType);

	if ( lpMonsterSkillUnit == NULL )
	{
		LogAddC(2, "[Monster Skill Manager] SkillUnit is NULL ");
		return;
	}

	GCUseMonsterSkillSend(lpObj, lpTargetObj, lpMonsterSkillUnit->m_iUnitNumber);

	if ( lpMonsterSkillUnit->m_iUnitScopeType == -1 )
	{
		lpMonsterSkillUnit->RunSkill(iIndex, iTargetIndex);
	}
	else
	{
		int iCount = 0;
		int iRangeTargetIndex = -1;
		int iAngle = gObjUseSkill.GetAngle(lpObj->X, lpObj->Y, lpTargetObj->X, lpTargetObj->Y);

		while ( true )
		{
			if ( lpObj->VpPlayer2[iCount].state )
			{
				iRangeTargetIndex = lpObj->VpPlayer2[iCount].number;

				if ( OBJMAX_RANGE(iRangeTargetIndex))
				{
					if ( gObj[iRangeTargetIndex].Type == OBJ_USER )
					{
						BOOL bTargetOK = FALSE;

						if ( lpMonsterSkillUnit->m_iUnitScopeType == 0 )
						{
							if ( gObjCalDistance(lpObj, &gObj[iRangeTargetIndex]) < lpMonsterSkillUnit->m_iUnitScopeValue )
							{
								bTargetOK = TRUE;
							}
						}
						else  if ( lpMonsterSkillUnit->m_iUnitScopeType == 1 )
						{
							if ( SkillElectricSparkHitBox.HitCheck(iAngle, lpObj->X, lpObj->Y,
								gObj[iRangeTargetIndex].X, gObj[iRangeTargetIndex].Y) )
							{
								bTargetOK = TRUE;
							}
						}
						else if ( lpMonsterSkillUnit->m_iUnitScopeType == 2 )
						{
							bTargetOK = TRUE;
						}
						else if ( lpMonsterSkillUnit->m_iUnitScopeType == 3 )
						{
							bTargetOK = TRUE;
						}

						if ( bTargetOK )
						{
							lpMonsterSkillUnit->RunSkill(iIndex, iRangeTargetIndex);
						}
					}
				}
			}

			iCount++;

			if ( iCount > MAX_VIEWPORT_MONSTER-1)
				break;
		}
	}
						
}




