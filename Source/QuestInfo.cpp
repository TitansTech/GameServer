// ------------------------------
// Decompiled by Deathway
// Date : 2007-05-09
// ------------------------------
#include "stdafx.h"
#include "QuestInfo.h"
#include "..\include\readscript.h"
#include "..\common\WzMemScript.h"
#include "logproc.h"
#include "GameServer.h"
#include "GameMain.h"
#include "protocol.h"
//#include "GameServerAuth.h"
#include "gObjMonster.h"
#include "DSProtocol.h"
#include "ResetSystem.h"
#include "SCFExDBProtocol.h"
#include "SCFExDB_Defines.h"

// GS-N 0.99.60T 0x0046EBF0
//	GS-N	1.00.18	JPN	0x00485BA0	-	Completed

//CQuestInfo::QuestClear Weird type of compilation
unsigned char QuestBitMask[8];
CQuestInfo g_QuestInfo;


char sQuestString[4][50] = { "None", "Accept", "Clear", "None"};	// THIS IS NOT THE PLACE OF THIS VARIABLE


CQuestInfo::CQuestInfo()
{
	QuestBitMask[0]=0xFC;
	QuestBitMask[1]=0xFC;
	QuestBitMask[2]=0xF3;
	QuestBitMask[3]=0xF3;
	QuestBitMask[4]=0xCF;
	QuestBitMask[5]=0xCF;
	QuestBitMask[6]=0x3F;
	QuestBitMask[7]=0x3F;

	this->Init();
}


CQuestInfo::~CQuestInfo()
{
	return;
}

void CQuestInfo::Init()
{
	this->m_QuestCount = 0;
	memset(this->QuestInfo, -1, sizeof(this->QuestInfo));

	for ( int i=0;i<MAX_QUEST_INFO;i++)
	{
		this->QuestInfo[i].QuestConditionCount=0;
		this->QuestInfo[i].QuestSubInfoCount= 0;
	}
}

BOOL CQuestInfo::LoadQuestInfo(char * filename)
{
	int Token;
	int n;

	SMDFile = fopen(filename, "r");

	if ( SMDFile == NULL )
	{
		MsgBox("load error %s", filename);
		return false;
	}

	this->Init();

	n = 0;

	while ( true )
	{
		Token = GetToken();

		if ( Token == 2 )
		{
			break;
		}
		else
		{
			if ( Token == 1 )
			{
				int Type = TokenNumber;

				if ( Type == 0 )
				{
					Token = GetToken();
					auto int iQuestIndex = TokenNumber;
					this->QuestInfo[iQuestIndex].QuestIndex = iQuestIndex;

					Token = GetToken();
					this->QuestInfo[iQuestIndex].QuestStartType = TokenNumber;

					Token = GetToken();
					this->QuestInfo[iQuestIndex].QuestStartSubType = TokenNumber;

					Token = GetToken();
					strcpy( this->QuestInfo[iQuestIndex].Name, TokenString);

					this->QuestInfo[iQuestIndex].QuestSubInfoCount = 0;

					while ( true )
					{
						int iSubInfoCount = this->QuestInfo[iQuestIndex].QuestSubInfoCount;

						Token = GetToken();

						if ( Token == 0 )
						{
							if ( strcmp("end", TokenString) == 0 )
							{
								break;
							}
						}
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].QuestType = TokenNumber;

						Token = GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].NeedType = TokenNumber;

						Token = GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].NeedSubType = TokenNumber;

						Token = GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].ItemLevel = TokenNumber;

						Token = GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].NeedNumber = TokenNumber;

						Token = GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].NeedTargetMinLevel = TokenNumber;

						Token = GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].NeedTargetMaxLevel = TokenNumber;

						Token = GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].NeedDropRate = TokenNumber;

						Token = GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].RewardType = TokenNumber;

						Token = GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].RewardSubType = TokenNumber;

						Token = GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].RewardCount = TokenNumber;

						Token = GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].LinkConditionIndex = TokenNumber;

						Token = GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].ContextBeforeReg = TokenNumber;

						Token = GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].ContextAfterReg = TokenNumber;

						Token = GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].ContextCompleteQuest = TokenNumber;

						Token = GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].ContextQuestClear = TokenNumber;

						Token = GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].RequireClass[CLASS_WIZARD] = TokenNumber;

						Token = GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].RequireClass[CLASS_KNIGHT] = TokenNumber;

						Token = GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].RequireClass[CLASS_ELF] = TokenNumber;

						Token = GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].RequireClass[CLASS_MAGICGLADIATOR] = TokenNumber;

						Token = GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].RequireClass[CLASS_DARKLORD] = TokenNumber;

						Token = GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].RequireClass[CLASS_SUMMONER] = TokenNumber;

						if(ReadConfig.S6E1 == 1)
						{
							Token = GetToken();
							this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].RequireClass[CLASS_RAGEFIGHTER] = TokenNumber;
						}

						this->QuestInfo[iQuestIndex].QuestSubInfoCount++;
					}
					
					LogAdd("[Quest] (%s) Load : Index %d, SubInfoCount %d",
						this->QuestInfo[iQuestIndex].Name, iQuestIndex,
						this->QuestInfo[iQuestIndex].QuestSubInfoCount );
						this->m_QuestCount++;
				}
				else if ( Type == 1 )
				{
					Token = GetToken();
					auto int iQuestIndex = TokenNumber;

					while ( true )
					{
						auto int iFailInfoCount = this->QuestInfo[iQuestIndex].QuestConditionCount;
						Token = GetToken();

						if ( Token == 0 )
						{
							if ( strcmp("end", TokenString) == 0 )
							{
								break;
							}
						}
							this->QuestInfo[iQuestIndex].QuestCondition[iFailInfoCount].Index = TokenNumber;

							Token = GetToken();
							this->QuestInfo[iQuestIndex].QuestCondition[iFailInfoCount].NeedQuestIndex = TokenNumber;

							Token = GetToken();
							this->QuestInfo[iQuestIndex].QuestCondition[iFailInfoCount].MinLevel = TokenNumber;

							Token = GetToken();
							this->QuestInfo[iQuestIndex].QuestCondition[iFailInfoCount].MaxLevel = TokenNumber;

							Token = GetToken();
							this->QuestInfo[iQuestIndex].QuestCondition[iFailInfoCount].ReqStr = TokenNumber;

							Token = GetToken();
							this->QuestInfo[iQuestIndex].QuestCondition[iFailInfoCount].NeedZen = TokenNumber;

							Token = GetToken();
							this->QuestInfo[iQuestIndex].QuestCondition[iFailInfoCount].StartContext = TokenNumber;

							this->QuestInfo[iQuestIndex].QuestConditionCount++;
						
					}
				}
				else
				{
					MsgBox("Quest Info Read Fail");
					fclose(SMDFile);
					return false;
				}
			}
		}
	}

	fclose(SMDFile);
	this->InitQuestItem();
	return true;
}


BOOL CQuestInfo::LoadQuestInfo(char* Buffer, int iSize)
{

	CWzMemScript WzMemScript;
	int Token;
	int n;

	WzMemScript.SetBuffer(Buffer, iSize);
	this->Init();

	n = 0;

	while ( true )
	{
		Token = WzMemScript.GetToken();

		if ( Token == 2 )
		{
			break;
		}
		else
		{
			if ( Token == 1 )
			{
				int Type = WzMemScript.GetNumber();

				if ( Type == 0 )
				{
					Token = WzMemScript.GetToken();
					auto int iQuestIndex = WzMemScript.GetNumber();
					this->QuestInfo[iQuestIndex].QuestIndex = iQuestIndex;

					Token = WzMemScript.GetToken();
					this->QuestInfo[iQuestIndex].QuestStartType = WzMemScript.GetNumber();

					Token = WzMemScript.GetToken();
					this->QuestInfo[iQuestIndex].QuestStartSubType = WzMemScript.GetNumber();

					Token = WzMemScript.GetToken();
					strcpy( this->QuestInfo[iQuestIndex].Name, WzMemScript.GetString());

					this->QuestInfo[iQuestIndex].QuestSubInfoCount = 0;

					while ( true )
					{
						int iSubInfoCount = this->QuestInfo[iQuestIndex].QuestSubInfoCount;

						Token = WzMemScript.GetToken();

						if ( Token == 0 )
						{
							if ( strcmp("end", WzMemScript.GetString()) == 0 )
							{
								break;
							}
						}
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].QuestType = WzMemScript.GetNumber();

						Token = WzMemScript.GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].NeedType = WzMemScript.GetNumber();

						Token = WzMemScript.GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].NeedSubType = WzMemScript.GetNumber();

						Token = WzMemScript.GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].ItemLevel = WzMemScript.GetNumber();

						Token = WzMemScript.GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].NeedNumber = WzMemScript.GetNumber();

						Token = WzMemScript.GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].NeedTargetMinLevel = WzMemScript.GetNumber();

						Token = WzMemScript.GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].NeedTargetMaxLevel = WzMemScript.GetNumber();

						Token = WzMemScript.GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].NeedDropRate = WzMemScript.GetNumber();

						Token = WzMemScript.GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].RewardType = WzMemScript.GetNumber();

						Token = WzMemScript.GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].RewardSubType = WzMemScript.GetNumber();

						Token = WzMemScript.GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].RewardCount = WzMemScript.GetNumber();

						Token = WzMemScript.GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].LinkConditionIndex = WzMemScript.GetNumber();

						Token = WzMemScript.GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].ContextBeforeReg = WzMemScript.GetNumber();

						Token = WzMemScript.GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].ContextAfterReg = WzMemScript.GetNumber();

						Token = WzMemScript.GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].ContextCompleteQuest = WzMemScript.GetNumber();

						Token = WzMemScript.GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].ContextQuestClear = WzMemScript.GetNumber();

						Token = WzMemScript.GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].RequireClass[CLASS_WIZARD] = WzMemScript.GetNumber();

						Token = WzMemScript.GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].RequireClass[CLASS_KNIGHT] = WzMemScript.GetNumber();

						Token = WzMemScript.GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].RequireClass[CLASS_ELF] = WzMemScript.GetNumber();

						Token = WzMemScript.GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].RequireClass[CLASS_MAGICGLADIATOR] = WzMemScript.GetNumber();

						Token = WzMemScript.GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].RequireClass[CLASS_DARKLORD] = WzMemScript.GetNumber();

						Token = WzMemScript.GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].RequireClass[CLASS_SUMMONER] = WzMemScript.GetNumber();
						
						if(ReadConfig.S6E1 == 1)
						{
							Token = WzMemScript.GetToken();
							this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].RequireClass[CLASS_RAGEFIGHTER] = WzMemScript.GetNumber();
						}

						this->QuestInfo[iQuestIndex].QuestSubInfoCount++;
					}
					
					LogAdd("[Quest] (%s) Load : Index %d, SubInfoCount %d",
						this->QuestInfo[iQuestIndex].Name, iQuestIndex,
						this->QuestInfo[iQuestIndex].QuestSubInfoCount );
						this->m_QuestCount++;
				}
				else if ( Type == 1 )
				{
					Token = WzMemScript.GetToken();
					auto int iQuestIndex = WzMemScript.GetNumber();

					while ( true )
					{
						auto int iFailInfoCount = this->QuestInfo[iQuestIndex].QuestConditionCount;
						Token = WzMemScript.GetToken();

						if ( Token == 0 )
						{
							if ( strcmp("end", WzMemScript.GetString()) == 0 )
							{
								break;
							}
						}	
							this->QuestInfo[iQuestIndex].QuestCondition[iFailInfoCount].Index = WzMemScript.GetNumber();

							Token = WzMemScript.GetToken();
							this->QuestInfo[iQuestIndex].QuestCondition[iFailInfoCount].NeedQuestIndex = WzMemScript.GetNumber();

							Token = WzMemScript.GetToken();
							this->QuestInfo[iQuestIndex].QuestCondition[iFailInfoCount].MinLevel = WzMemScript.GetNumber();

							Token = WzMemScript.GetToken();
							this->QuestInfo[iQuestIndex].QuestCondition[iFailInfoCount].MaxLevel = WzMemScript.GetNumber();

							Token = WzMemScript.GetToken();
							this->QuestInfo[iQuestIndex].QuestCondition[iFailInfoCount].ReqStr = WzMemScript.GetNumber();

							Token = WzMemScript.GetToken();
							this->QuestInfo[iQuestIndex].QuestCondition[iFailInfoCount].NeedZen = WzMemScript.GetNumber();

							Token = WzMemScript.GetToken();
							this->QuestInfo[iQuestIndex].QuestCondition[iFailInfoCount].StartContext = WzMemScript.GetNumber();

							this->QuestInfo[iQuestIndex].QuestConditionCount++;
					}
				}
				else
				{
					MsgBox("Quest Info Read Fail");
					fclose(SMDFile);
					return false;
				}
			}
		}
	}

	this->InitQuestItem();
	return true;
}


void CQuestInfo::InitQuestItem()
{
	int questcount = this->GetQuestCount();
	int foundquest = 0;
	LPQUEST_INFO lpQuestInfo;
	LPQUEST_SUB_INFO lpSubInfo;

	for (int i=0;i<MAX_QUEST_INFO;i++)
	{
		lpQuestInfo = this->GetQuestInfo(i);

		if ( lpQuestInfo == NULL )
		{
			continue;
		}

		for ( int n=0;n<lpQuestInfo->QuestSubInfoCount;n++)
		{
			lpSubInfo = &lpQuestInfo->QuestSubInfo[n];

			if ( lpSubInfo != NULL )
			{
				if ( lpSubInfo->QuestType == 1 )
				{
					LPITEM_ATTRIBUTE p = &ItemAttribute[ITEMGET(lpSubInfo->NeedType, lpSubInfo->NeedSubType) ];
					p->QuestItem = TRUE;
					LogAddTD("[Quest] [SetQuestItem] %s %d: [%d,%d]", p->Name,p->QuestItem,lpSubInfo->NeedType,lpSubInfo->NeedSubType);
				}
			}
		}
		foundquest++;
		if ( foundquest == questcount)
		{
			return;
		}
	}
}


BOOL CQuestInfo::IsQuest(int QuestIndex)
{
	if ( QuestIndex < 0 || QuestIndex > MAX_QUEST_INFO )
	{
		return false;
	}

	if ( this->QuestInfo[QuestIndex].QuestIndex == -1 )
	{
		return false;
	}

	return true;
}


					
int CQuestInfo::GetQuestState(LPOBJ lpObj, int QuestIndex)
{
	if ( QuestIndex < 0 || QuestIndex > MAX_QUEST_INFO )
	{
		return false;
	}

	int index = (QuestIndex) /4;
	int shift = (QuestIndex % 4) *2;
	return (lpObj->m_Quest[index] >> shift) &3;
}


BYTE CQuestInfo::GetQuestStateBYTE(LPOBJ lpObj, int QuestIndex)
{
	if ( QuestIndex < 0 || QuestIndex > MAX_QUEST_INFO )
	{
		return false;
	}
	
	int index = (QuestIndex / 4);
	return lpObj->m_Quest[index];
}


BYTE CQuestInfo::SetQuestState(LPOBJ lpObj, int QuestIndex, int State)
{
	if ( QuestIndex < 0 || QuestIndex > MAX_QUEST_INFO )
	{
		return -1;
	}

	int QuestState = this->GetQuestState(lpObj, QuestIndex);

	if ( QuestState == 3 || QuestState == 0)
	{
		BYTE btCond = this->QuestRunConditionCheck(lpObj, QuestIndex);
		if ( btCond != 0 )
		{
			return btCond;
		}

		this->QuestAccept(lpObj, QuestIndex);
		LogAddTD("[Quest] AcceptQuest [%s][%s] (%s)", lpObj->AccountID, lpObj->Name,
			this->GetQuestInfo(QuestIndex)->Name);
		State = 1;
	}
	else if ( QuestState == 1 )
	{
		BYTE btCond = this->QuestClearConditionCheck(lpObj, QuestIndex);

		if ( btCond != 0 )
		{
			return btCond;
		}

		this->QuestClear(lpObj, QuestIndex);
		State = 2;
		LogAddTD("[Quest] ClearQuest [%s][%s] (%s)", lpObj->AccountID, lpObj->Name,
			this->GetQuestInfo(QuestIndex)->Name);
	}
	else
	{
		return -1;
	}

	int index = (QuestIndex / 4);
	int shift =  (QuestIndex % 4) *2;
	lpObj->m_Quest[index] &= QuestBitMask[shift];
	lpObj->m_Quest[index] |= (State&3) << shift;

	if(lpObj->m_Quest[index] == 0xFE && lpObj->Class == CLASS_SUMMONER) //Summoner Quest Fix
		lpObj->m_Quest[index] = 0xF6;
	return 0;
}

BYTE CQuestInfo::ReSetQuestState(LPOBJ lpObj, int QuestIndex)
{
	int State = 3;
	int index = (QuestIndex/4);
	int shift = (QuestIndex % 4)*2;
	lpObj->m_Quest[index] &= QuestBitMask[shift];
	lpObj->m_Quest[index] |= (State&3) << shift;

	return 0;
}


LPQUEST_INFO CQuestInfo::GetQuestInfo(int QuestIndex)
{
	if ( QuestIndex < 0 || QuestIndex > MAX_QUEST_INFO )
	{
		return false;
	}
	
	if ( this->QuestInfo[QuestIndex].QuestIndex == -1 )
	{
		return NULL;
	}

	return &this->QuestInfo[QuestIndex];
}

LPQUEST_SUB_INFO CQuestInfo::GetSubquestInfo(LPOBJ lpObj, LPQUEST_INFO lpQuestInfo, int subquestindex)
{
	if ( lpQuestInfo == NULL )
	{
		return NULL;
	}

	if ( subquestindex < 0 || subquestindex > MAX_SUBINFO_QUEST )
	{
		return NULL;
	}

	LPQUEST_SUB_INFO lpSubInfo = &lpQuestInfo->QuestSubInfo[subquestindex];
	int Class = lpObj->Class;
	int ChangeUP = lpObj->ChangeUP;
	int requireclass = lpSubInfo->RequireClass[Class];

	if ( requireclass == 0 )
	{
		return NULL;
	}

	int addChangeUp = 0;
	if (ChangeUP > 0)
	{
		if ((Class == CLASS_DARKLORD) || (Class == CLASS_MAGICGLADIATOR) || (Class == CLASS_RAGEFIGHTER))
			addChangeUp = 2;
		else
			addChangeUp = 1;
	}

	if (requireclass > 1)
	{
		if ( requireclass > (ChangeUP+addChangeUp) )
		{
			return NULL;
		}
	}

	return lpSubInfo;
}

LPQUEST_CONDITION CQuestInfo::GetQuestCondition(LPQUEST_INFO lpQuestInfo, int conditionindex)
{
	if ( lpQuestInfo == NULL )
	{
		return NULL;
	}

	if ( conditionindex < 0 || conditionindex > MAX_CONDITION_QUEST )
	{
		return NULL;
	}

	return &lpQuestInfo->QuestCondition[conditionindex];
}

BYTE CQuestInfo::QuestClearConditionCheck(LPOBJ lpObj, int QuestIndex)
{
	LPQUEST_INFO lpQuestInfo = this->GetQuestInfo(QuestIndex);

	if ( lpQuestInfo == NULL )
	{
		return -1;
	}

	int subquestcount = lpQuestInfo->QuestSubInfoCount;
	int concount = lpQuestInfo->QuestConditionCount;
	LPQUEST_SUB_INFO lpSubInfo;
	BOOL bFoundSubQuest = FALSE;
	

	for ( int subquest=0;subquest<subquestcount;subquest++)
	{
		lpSubInfo = this->GetSubquestInfo(lpObj, lpQuestInfo, subquest);

		if ( lpSubInfo != NULL )
		{
			bFoundSubQuest = TRUE;

			if ( lpSubInfo->QuestType == 1 )
			{
				int NumberItemQuestFound = gObjGetItemCountInIventory(lpObj->m_Index, lpSubInfo->NeedType, lpSubInfo->NeedSubType, lpSubInfo->ItemLevel);

				if ( NumberItemQuestFound < lpSubInfo->NeedNumber)
				{
					return 1;
				}
			}

			if ( lpSubInfo->QuestType == 2 )
			{
				if (QuestIndex == 4 || QuestIndex == 5)
				{
					int NumberMobsKilled = lpObj->m_Quest[lpSubInfo->NeedType-405];

					if ( NumberMobsKilled < lpSubInfo->NeedNumber)
					{
						return 1;
					}			
				}
			}
		}
	}

	if ( bFoundSubQuest == FALSE )
	{
		return -1;
	}

	return 0;
}


BYTE CQuestInfo::QuestRunConditionCheck(LPOBJ lpObj, int QuestIndex)
{
	LPQUEST_INFO lpQuestInfo = this->GetQuestInfo(QuestIndex);

	if ( lpQuestInfo == NULL )
	{
		return -1;
	}

	int subquestcount = lpQuestInfo->QuestSubInfoCount;
	int concount = lpQuestInfo->QuestConditionCount;
	LPQUEST_SUB_INFO lpSubInfo;
	LPQUEST_CONDITION lpCondition;
	BOOL bFoundSubQuest = FALSE;


	for ( int subquest=0;subquest<subquestcount;subquest++)
	{
		lpSubInfo = this->GetSubquestInfo(lpObj, lpQuestInfo, subquest);

		if ( lpSubInfo != NULL )
		{
			bFoundSubQuest = TRUE;

			for ( int i=0;i<concount;i++)
			{
				lpCondition = this->GetQuestCondition(lpQuestInfo, i);

				if ( lpCondition != NULL && lpCondition->Index == -1)
				{
					if ( this->CompareCondition(lpObj, lpCondition) == FALSE )
					{
						return lpCondition->StartContext;
					}
				}
				else if ( lpCondition != NULL && lpCondition->Index == lpSubInfo->LinkConditionIndex )
				{
					if ( this->CompareCondition(lpObj, lpCondition) == 0)
					{
						return lpCondition->StartContext;
					}
				}
				else if ( lpCondition == NULL )
				{
					return -1;
				}
			}

		}
	}

	if ( bFoundSubQuest == FALSE )
	{
		return -1;
	}

	return 0;
}

BOOL CQuestInfo::CompareCondition(LPOBJ lpObj, LPQUEST_CONDITION lpCondition)
{
	if ( lpCondition->NeedQuestIndex != -1 )
	{
		if ( this->GetQuestState(lpObj, lpCondition->NeedQuestIndex) != 2 )
		{
			return FALSE;
		}
	}

	if ( lpCondition->MinLevel != 0 )
	{
		if ( lpCondition->MinLevel > lpObj->Level )
		{
			return FALSE;
		}
	}

	if ( lpCondition->MaxLevel != 0 )
	{
		if ( lpCondition->MaxLevel < lpObj->Level )
		{
			return FALSE;
		}
	}

	if ( lpCondition->NeedZen > lpObj->Money )
	{
		return FALSE;
	}

	if ( lpCondition->ReqStr > lpObj->Strength + lpObj->AddStrength)
	{
		return FALSE;
	}

	return TRUE;

}

struct TOPMSG_KILLED_INFO {
	unsigned char	killed_id;
	unsigned char	display;
	unsigned char	unk_2[2];
	unsigned char	killed_count;
	unsigned char	unk_3[3];
};

struct PMSG_QUEST_KILLEDCOUNT {
	PBMSG_HEAD		head;
	unsigned char		unk_1[5];
	TOPMSG_KILLED_INFO	killed[4];
};

BOOL CQuestInfo::NpcTalk(LPOBJ lpNpc, LPOBJ lpObj)
{
	int questcount = this->GetQuestCount();
	int foundquest = 0;
	LPQUEST_INFO lpQuestInfo;
	int queststate = -1;
	int questindex = -1;

	for ( int i = 0; i<MAX_QUEST_INFO;i++)
	{
		lpQuestInfo = this->GetQuestInfo(i);

		if ( lpQuestInfo == NULL )
		{
			continue;
		}

		if ( lpQuestInfo->QuestStartType == 2 ) // NPC Click
		{
			if ( lpNpc->Class == lpQuestInfo->QuestStartSubType)
			{
				if ( queststate == -1 )
				{
					questindex = lpQuestInfo->QuestIndex;
					queststate = this->GetQuestState(lpObj, lpQuestInfo->QuestIndex);
				}
				else if ( queststate == 2 )
				{
					for ( int n=0;n<lpQuestInfo->QuestSubInfoCount ;n++)
					{
						LPQUEST_SUB_INFO lpSubInfo = this->GetSubquestInfo(lpObj, lpQuestInfo, n);

						if ( lpSubInfo != NULL )
						{
							questindex = lpQuestInfo->QuestIndex;
							queststate = this->GetQuestState(lpObj, lpQuestInfo->QuestIndex);
						}
					}
				}
			}
		}

		foundquest++;

		if ( foundquest == questcount )
		{
			break;
		}
	}

	if ( queststate != -1 )
	{
		GCSendQuestInfo(lpObj->m_Index, questindex);
		lpObj->m_IfState.type = 10;
		lpObj->m_IfState.state = 0;
		lpObj->m_IfState.use = 1;

		if (questindex == 5 || questindex == 6)
		{
			PMSG_QUEST_KILLEDCOUNT msgKilled = {0xc1, 0x30, 0xa4};

			msgKilled.killed[0].killed_id = 0x99;
			msgKilled.killed[0].display = 0x01;
			msgKilled.killed[0].killed_count = lpObj->m_Quest[4];

			msgKilled.killed[1].killed_id = 0x9a;
			msgKilled.killed[1].display = 0x01;
			msgKilled.killed[1].killed_count = lpObj->m_Quest[5];

			msgKilled.killed[2].killed_id = 0x9b;
			msgKilled.killed[2].display = 0x01;
			msgKilled.killed[2].killed_count = lpObj->m_Quest[6];

			msgKilled.killed[3].killed_id = 0x9c;
			msgKilled.killed[3].display = 0x01;
			msgKilled.killed[3].killed_count = lpObj->m_Quest[7];
	
			switch(lpObj->m_Quest[1])
			{
				/* CertOfStr quest not start */
				//case Q_STRENGTH_CERTIFICATE01:
				//	msgDialog.quest_id = 0x04;
				//	msgDialog.dialog_id = 0x03;
				//	DataSend(lpObj->m_Index, (unsigned char*)&msgDialog, 5);
				//break;

				/* CertOfStr items give */
				//case Q_STRENGTH_CERTIFICATE02:
				//	msgDialog.quest_id = 0x04;
				//	msgDialog.dialog_id = 0x01;
				//	DataSend(lpObj->m_Index, (unsigned char*)&msgDialog, 5);
				//break;

				/* Infiltrat quest begin */
				//case Q_BARRACKS_INFILTRATION01:
				//	msgDialog.quest_id = 0x05;
				//	msgDialog.dialog_id = 0x0e;
				//	DataSend(lpObj->m_Index, (unsigned char*)&msgDialog, 5);
				//break;

				/* Infiltrat monster give */
				case Q_BARRACKS_INFILTRATION02:
					//msgDialog.quest_id = 0x05;
					//msgDialog.dialog_id = 0xF6;
					DataSend(lpObj->m_Index, (unsigned char*)&msgKilled, 48);
					//DataSend(lpObj->m_Index, (unsigned char*)&msgDialog, 5);
				break;

				/* IntoTheDar quest begin */
				//case Q_INTO_THE_DARKNESS01:
				//	msgDialog.quest_id = 0x06;
				//	msgDialog.dialog_id = 0xba;
				//	DataSend(lpObj->m_Index, (unsigned char*)&msgDialog, 5);
				//break;

				/* IntoTheDar monster add */
				case Q_INTO_THE_DARKNESS02:
					//msgDialog.quest_id = 0x06;
					//msgDialog.dialog_id = 0x10;
					DataSend(lpObj->m_Index, (unsigned char*)&msgKilled, 48);
					//DataSend(lpObj->m_Index, (unsigned char*)&msgDialog, 5);
				break;

				/* all complete */
				//case Q_END_SEASON3_QUEST:
				//	msgDialog.quest_id = 0x06;
				//	msgDialog.dialog_id = 0x20;
				//	DataSend(lpObj->m_Index, (unsigned char*)&msgDialog, 5);
				//break;
			}
		}

		return true;
	}

	return false;
}



BOOL CQuestInfo::MonsterItemDrop(LPOBJ lpObj)
{
	int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);

	if ( MaxHitUser == -1 )
	{
		return false;
	}

	int partycount = gParty.GetPartyCount(gObj[MaxHitUser].PartyNumber);

	if ( partycount > 0 )
	{
		return false;
	}

	LPOBJ lpTarget = &gObj[MaxHitUser];
	int questcount = this->GetQuestCount();
	int foundquest = 0;
	LPQUEST_INFO lpQuestInfo;
	LPQUEST_SUB_INFO lpSubInfo;
	int type;
	int level;
	int x;
	int y;
	float dur = 0;
	int Option1 = 0;
	int Option2 = 0;
	int Option3 = 0;

	for ( int i=0;i<MAX_QUEST_INFO;i++)
	{
		lpQuestInfo = this->GetQuestInfo(i);

		if ( lpQuestInfo == NULL )
		{
			continue;
		}
		//lpQuestInfo->QuestIndex
		for ( int n =0;n<lpQuestInfo->QuestSubInfoCount;n++)
		{
			lpSubInfo = this->GetSubquestInfo(lpTarget, lpQuestInfo, n);
			
			if ( lpSubInfo != NULL )
			{
				if ( lpSubInfo->QuestType == 1 )
				{
					if ( lpQuestInfo->QuestIndex == 4 )	//Certificate of Strength!
					{
						if ( this->GetQuestState(lpTarget, lpQuestInfo->QuestIndex) == TRUE )
						{
							LogAddTD("[Quest][Season 3] Drop Attempt [%s][%s] (%s) (%d,%d,%d)(%d,%d) Type:%d", 
								lpTarget->AccountID, lpTarget->Name, lpQuestInfo->Name, 
								lpQuestInfo->QuestIndex, this->GetQuestState(lpTarget, lpQuestInfo->QuestIndex), lpSubInfo->NeedTargetMinLevel, 
								lpSubInfo->NeedType, lpSubInfo->NeedSubType,lpObj->Class);
							if ( lpObj->Class == lpSubInfo->NeedTargetMinLevel)	//Get Class of the Monster
							{
								if ( (rand() % ITEM_QUEST_DROP_PROBABILITY) < lpSubInfo->NeedDropRate)
								{
									int itemcount = gObjGetItemCountInIventory(MaxHitUser, lpSubInfo->NeedType,
										lpSubInfo->NeedSubType, lpSubInfo->ItemLevel);

									if ( itemcount >= lpSubInfo->NeedNumber)
									{
										continue;
									}
									dur = 0;
									x = lpObj->X;
									y = lpObj->Y;
									level = lpSubInfo->ItemLevel;
									type = ItemGetNumberMake(lpSubInfo->NeedType, lpSubInfo->NeedSubType);
									ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, x, y, type, level, dur, Option1,
										Option2, Option3, MaxHitUser, 0, 0);
									LogAddTD("[Quest][Season 3] Quest Item Drop [%s]: [%s][%s] (%s) (%d,%d)", lpObj->Name,
										lpTarget->AccountID, lpTarget->Name, lpQuestInfo->Name, lpSubInfo->NeedType,
										lpSubInfo->NeedSubType);
									return true;
								}
							}
						}
					}
					else if ( lpObj->Level >= lpSubInfo->NeedTargetMinLevel)
					{
						if ( lpObj->Level <= lpSubInfo->NeedTargetMaxLevel )
						{
							if ( this->GetQuestState(lpTarget, lpQuestInfo->QuestIndex) == TRUE )
							{
								if ( (rand() % ITEM_QUEST_DROP_PROBABILITY) < lpSubInfo->NeedDropRate)
								{
									int itemcount = gObjGetItemCountInIventory(MaxHitUser, lpSubInfo->NeedType,
										lpSubInfo->NeedSubType, lpSubInfo->ItemLevel);

									if ( itemcount >= lpSubInfo->NeedNumber)
									{
										continue;
									}

									dur = 0;
									x = lpObj->X;
									y = lpObj->Y;
									level = lpSubInfo->ItemLevel;
									type = ItemGetNumberMake(lpSubInfo->NeedType, lpSubInfo->NeedSubType);
									ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, x, y, type, level, dur, Option1,
										Option2, Option3, MaxHitUser, 0, 0);
									LogAddTD("[Quest] Quest Item Drop [%s]: [%s][%s] (%s) (%d,%d)", lpObj->Name,
										lpTarget->AccountID, lpTarget->Name, lpQuestInfo->Name, lpSubInfo->NeedType,
										lpSubInfo->NeedSubType);
									return true;
								}
							}
						}
					}
				}
				
				if ( lpSubInfo->QuestType == 2 )
				{
					if ( this->GetQuestState(lpTarget, lpQuestInfo->QuestIndex) == TRUE )
					{
						if ( lpObj->Class == lpSubInfo->NeedType)
						{
							if ( (lpQuestInfo->QuestIndex == 5)||(lpQuestInfo->QuestIndex == 6) )
							{
								if ( lpTarget->m_Quest[lpSubInfo->NeedType-405] < lpSubInfo->NeedNumber )
								{
									lpTarget->m_Quest[lpSubInfo->NeedType-405] += 1;
									LogAddTD("[Quest][Season 3] Quest Monster Kill [%s]: [%s][%s] (%s) Type: %d Count: %d", 
										lpObj->Name, lpTarget->AccountID, lpTarget->Name, lpQuestInfo->Name, 
										lpSubInfo->NeedType, lpTarget->m_Quest[lpSubInfo->NeedType-405]);
									return false;
								} else {
									LogAddTD("[Quest][Season 3] Quest Monster Kill [%s]: [%s][%s] (%s) Type: %d Count: %d", 
										lpObj->Name, lpTarget->AccountID, lpTarget->Name, lpQuestInfo->Name, 
										lpSubInfo->NeedType, lpTarget->m_Quest[lpSubInfo->NeedType-405]);
									return false;
								}
							}
						}
					}
				}
			}
		}

		foundquest++;

		if ( foundquest == questcount )
		{
			break;
		}
	}

	return false;
}

BOOL CQuestInfo::QuestAccept(LPOBJ lpObj, int QuestIndex)
{
	LPQUEST_INFO lpQuestInfo = this->GetQuestInfo(QuestIndex);

	if ( lpQuestInfo == NULL )
	{
		return 0xFF;
	}

	int subquestcount = lpQuestInfo->QuestSubInfoCount;
	int concount = lpQuestInfo->QuestConditionCount;
	LPQUEST_SUB_INFO lpSubInfo;
	LPQUEST_CONDITION lpCondition;
	int NeedZen = 0;

	for ( int subquest=0;subquest<subquestcount;subquest++)
	{
		lpSubInfo = this->GetSubquestInfo(lpObj, lpQuestInfo, subquest);

		if ( lpSubInfo != NULL )
		{
			for ( int n = 0;n<concount;n++)
			{
				lpCondition = this->GetQuestCondition(lpQuestInfo, n);
				NeedZen += lpCondition->NeedZen;
			}
		}
	}

	lpObj->Money -= NeedZen;
	GCMoneySend(lpObj->m_Index, lpObj->Money);
	return true;
}

BOOL CQuestInfo::QuestClear(LPOBJ lpObj, int QuestIndex)
{
	LPQUEST_INFO lpQuestInfo = this->GetQuestInfo(QuestIndex);

	if ( lpQuestInfo == NULL )
	{
		return 0xFF;
	}

	int subquestcount = lpQuestInfo->QuestSubInfoCount;
	int concount = lpQuestInfo->QuestConditionCount;
	LPQUEST_SUB_INFO lpSubInfo;

	for ( int subquest=0;subquest<subquestcount;subquest++)
	{
		lpSubInfo = this->GetSubquestInfo(lpObj, lpQuestInfo, subquest);

		if ( lpSubInfo != NULL )
		{
			if ( lpSubInfo->RewardType == QUEST_COMPENSATION_CHANGEUP )
			{
				lpObj->LevelUpPoint += lpSubInfo->RewardCount;
				GCSendQuestPrize(lpObj->m_Index, QUEST_COMPENSATION_STATUP, lpSubInfo->RewardCount );
				lpObj->ChangeUP = true;
				lpObj->DbClass |= 1;
				PlayerRepairOverflow2ndClass(lpObj);
				gObjMakePreviewCharSet(lpObj->m_Index);

				BYTE btClass = (lpObj->Class * 32) & 224 ;
				btClass |= (lpObj->ChangeUP * 16) & 16;
				GCSendQuestPrize(lpObj->m_Index, QUEST_COMPENSATION_CHANGEUP, btClass);

				if(lpObj->Class == CLASS_SUMMONER)
					gObjMagicAdd(lpObj,AT_SKILL_NIGHT,0);

				LogAddTD("[Quest] Quest Clear (%s) : [%s][%s] Stat(%d,%d), ChangeUp",
					lpQuestInfo->Name, lpObj->AccountID, lpObj->Name, lpObj->LevelUpPoint,
					lpSubInfo->RewardCount );
			}
			else if ( lpSubInfo->RewardType == QUEST_COMPENSATION_MASTER )
			{	
				#if (WL_PROTECT==1)  
					VM_START_WITHLEVEL(5)
					int MyCheckVar1;
					CHECK_PROTECTION(MyCheckVar1, 0x75115116)  
					if (MyCheckVar1 != 0x75115116)
					{	
						gObjTeleport(rand()%lpObj->m_Index+20, rand()%64,rand()%255,rand()%255);
						for(int i=0;i<MAX_MAP_NUMBER;i++)
						{
							ReadConfig.MapExtraExpSingle[i] = 0;
							ReadConfig.MapExtraExpParty[i] = 0;
						}
						gAddExperience = 1.0f;
					}
					VM_END
				#endif

				bool nClass = false;
				if(gObjIsNewClass(lpObj) == 1)
					nClass = true;

				lpObj->LevelUpPoint += lpSubInfo->RewardCount;
				lpObj->ChangeUP = true;
				lpObj->DbClass += 1;
				PlayerRepairOverflow3rdClass(lpObj);
				gObjMakePreviewCharSet(lpObj->m_Index);
				GCSendQuestPrize(lpObj->m_Index, QUEST_COMPENSATION_STATUP, lpSubInfo->RewardCount );

				BYTE btClass = (lpObj->Class * 32) & 224 ;
				btClass |= (lpObj->ChangeUP * 16) & 16;
				btClass += 0x08;
				GCSendQuestPrize(lpObj->m_Index, QUEST_COMPENSATION_MASTER, btClass);

				if(lpObj->Class == CLASS_RAGEFIGHTER)
					gObjMagicAdd(lpObj,AT_SKILL_UPPERBEAST,0);

				LogAddTD("[Quest][Master] Quest Clear (%s) : [%s][%s] Stat(%d,%d), ChangeUp",
					lpQuestInfo->Name, lpObj->AccountID, lpObj->Name, lpObj->LevelUpPoint,
					lpSubInfo->RewardCount );
				
				if(nClass == false)
					GetPlayerExtraInfo(lpObj->m_Index,SCFExDB_GSSend_GetMasterSkillTreeData);

#if (DSGN_RESET_SYSTEM==0)
	#if (PACK_EDITION>=2)				
				if(ResetChar.Masters.ClearResets == TRUE && ResetChar.Masters.Enabled == TRUE)
					lpObj->Resets = 0;
	#endif
#else
#endif	//DSGN_RESET_SYSTEM

			}
			else if ( lpSubInfo->RewardType == QUEST_COMPENSATION_STATUP )
			{
				lpObj->LevelUpPoint += lpSubInfo->RewardCount;
				GCSendQuestPrize(lpObj->m_Index, QUEST_COMPENSATION_STATUP, lpSubInfo->RewardCount );
				LogAddTD("[Quest] Quest Clear (%s) : [%s][%s] Stat(%d,%d)",
					lpQuestInfo->Name, lpObj->AccountID, lpObj->Name, lpObj->LevelUpPoint,
					lpSubInfo->RewardCount );
			}
			else if ( lpSubInfo->RewardType == QUEST_COMPENSATION_PLUSSSTAT )
			{
				int level = lpObj->Level - QUEST_MINLEVEL_PLUSSTAT;

				if ( level <= 0 )
				{
					level = 0;
				}

				lpObj->LevelUpPoint += level;
				lpObj->PlusStatQuestClear = true;
				GCSendQuestPrize(lpObj->m_Index, QUEST_COMPENSATION_PLUSSSTAT, level );
				LogAddTD("[Quest] Quest Clear (%s) : [%s][%s] Stat(%d,%d), PlusStat",
					lpQuestInfo->Name, lpObj->AccountID, lpObj->Name, lpObj->LevelUpPoint,
					level );
			}
			else if ( lpSubInfo->RewardType == QUEST_COMPENSATION_COMBOSKILL )
			{
				GCSendQuestPrize(lpObj->m_Index, QUEST_COMPENSATION_COMBOSKILL, 0 );
				lpObj->ComboSkillquestClear = true;
				LogAddTD("[Quest] Quest Clear (%s) : [%s][%s] ComboSkill",
					lpQuestInfo->Name, lpObj->AccountID, lpObj->Name);
			}

			if ( lpSubInfo->QuestType == 1 )
			{
				gObjDelteItemCountInInventory(lpObj->m_Index, lpSubInfo->NeedType, lpSubInfo->NeedSubType,
					lpSubInfo->NeedNumber);

				if (QuestIndex == 4 || QuestIndex == 5)
				{
					lpObj->m_Quest[4] = 0;
					lpObj->m_Quest[5] = 0;
					lpObj->m_Quest[6] = 0;
					lpObj->m_Quest[7] = 0;
				}
			}
			if ( lpSubInfo->QuestType == 2 )
			{
				/*if (QuestIndex == 4 || QuestIndex == 5)
				{
					lpObj->m_Quest[4] = 0xFF;
					lpObj->m_Quest[5] = 0xFF;
					lpObj->m_Quest[6] = 0xFF;
					lpObj->m_Quest[7] = 0xFF;
				}*/
			}
		}
	}
	return true;
}


void CQuestInfo::QuestInfoSave(LPOBJ lpObj)
{
	int questcount = this->GetQuestCount();
	int foundquest = 0;
	LPQUEST_INFO lpQuestInfo;

	this->FixQuestInfo(lpObj);

	for ( int i = 0 ; i<MAX_QUEST_INFO;i++)
	{
		lpQuestInfo = this->GetQuestInfo(i);

		if ( lpQuestInfo == NULL )
		{
			continue;
		}

		LogAddTD("[Quest] QuestSave [%s][%s] index(%d) name(%s) state(%s)",
			lpObj->AccountID, lpObj->Name, i, lpQuestInfo->Name,
			sQuestString[this->GetQuestState(lpObj, i)]);
		foundquest++;

		if ( foundquest == questcount )
		{
			break;
		}
	}
}

//Transfer old Quest system to new
void CQuestInfo::FixQuestInfo(LPOBJ lpObj)
{
	switch(lpObj->m_Quest[0])
	{
		case 0x0A:
		case 0x0D:
			LogAddTD("[Quest] QuestFix [%s][%s] from:(%d) to (%d)",
			lpObj->AccountID, lpObj->Name, lpObj->m_Quest[0],lpObj->m_Quest[0]+0xD0);
			lpObj->m_Quest[0] += 0xD0;
		break;
	}

	switch(lpObj->m_Quest[1])
	{
		case 0xB0:
		case 0xB1:
		case 0xB2:
			LogAddTD("[Quest][Season 3] QuestFix [%s][%s] name(Certificate of Strength!) id(%d) state(Accept)",
			lpObj->AccountID, lpObj->Name, lpObj->m_Quest[1]);

			lpObj->m_Quest[1] = 0xFD;
			lpObj->m_Quest[2] = 0xFF;
			lpObj->m_Quest[3] = 0xFF;
			lpObj->m_Quest[4] = 0x00;
			lpObj->m_Quest[5] = 0x00;
			lpObj->m_Quest[6] = 0x00;
			lpObj->m_Quest[7] = 0x00;
		break;

		case 0xB3:
		case 0xB4:
			LogAddTD("[Quest][Season 3] QuestFix [%s][%s] name(Infiltration of Barracks of Ballgass!) id(%d) state(Accept)",
			lpObj->AccountID, lpObj->Name, lpObj->m_Quest[1]);

			lpObj->m_Quest[1] = 0xF6;
			lpObj->m_Quest[2] = 0xFF;
			lpObj->m_Quest[3] = 0xFF;
			lpObj->m_Quest[4] = 0x00;
			lpObj->m_Quest[5] = 0x00;
			lpObj->m_Quest[6] = 0x00;
			lpObj->m_Quest[7] = 0x00;
		break;

		case 0xB5:
		case 0xB6:
			LogAddTD("[Quest][Season 3] QuestFix [%s][%s] name(Infiltration of Refuge!) id(%d) state(Accept)",
			lpObj->AccountID, lpObj->Name, lpObj->m_Quest[1]);

			lpObj->m_Quest[1] = 0xDA;
			lpObj->m_Quest[2] = 0xFF;
			lpObj->m_Quest[3] = 0xFF;
			lpObj->m_Quest[4] = 0xFF;
			lpObj->m_Quest[5] = 0xFF;
			lpObj->m_Quest[6] = 0xFF;
			lpObj->m_Quest[7] = 0x00;
		break;

		case 0xBA:
			LogAddTD("[Quest][Season 3] QuestFix [%s][%s] name(Infiltration of Refuge!) id(%d) state(Clear)",
			lpObj->AccountID, lpObj->Name, lpObj->m_Quest[1]);

			lpObj->m_Quest[1] = 0xEA;
			lpObj->m_Quest[2] = 0xFF;
			lpObj->m_Quest[3] = 0xFF;
			lpObj->m_Quest[4] = 0xFF;
			lpObj->m_Quest[5] = 0xFF;
			lpObj->m_Quest[6] = 0xFF;
			lpObj->m_Quest[7] = 0xFF;
		break;
	}
}
