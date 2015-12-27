// ------------------------------
// Decompiled by Deathway
// Date : 2007-05-09
// ------------------------------
#ifndef QUESTINFO_H
#define QUESTINFO_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\common\classdef.h"
#include "user.h"

#define MAX_QUEST_INFO 7//200
#define MAX_SUBINFO_QUEST 10
#define MAX_CONDITION_QUEST 10
#define ITEM_QUEST_DROP_PROBABILITY 1000
#define QUEST_COMPENSATION_CHANGEUP 201
#define QUEST_COMPENSATION_STATUP 200
#define QUEST_COMPENSATION_PLUSSSTAT 202
#define QUEST_COMPENSATION_COMBOSKILL 203
#define QUEST_COMPENSATION_MASTER 204
#define QUEST_MINLEVEL_PLUSSTAT 220
#define QUEST_STATE_SIZE 50


typedef struct QUEST_SUB_INFO
{
	short QuestType;	// 0
	short NeedType;	// 4
	short NeedSubType;	// 8
	BYTE ItemLevel;	// C
	short NeedNumber;	// 10
	short NeedTargetMinLevel;	// 14
	short NeedTargetMaxLevel;	// 18
	short NeedDropRate;	// 1C
	short RewardType;	// 20
	short RewardSubType;	// 24
	short RewardCount;	// 28
	short LinkConditionIndex;	// 2C
	short ContextBeforeReg;	// 30
	short ContextAfterReg;	// 34
	short ContextCompleteQuest;	// 38
	short ContextQuestClear;	// 3C
	BYTE RequireClass[MAX_TYPE_PLAYER];	// 40

} QUEST_SUB_INFO, * LPQUEST_SUB_INFO;


typedef struct QUEST_CONDITION
{
	short Index;	// 0
	short NeedQuestIndex;	// 4
	short MinLevel;	// 8
	short MaxLevel;	// C
	short ReqStr;	// 10
	short NeedZen;	// 14
	short StartContext;	// 18

/*	int iConditionalIndex;
	int iQuestIndex;
	int iLevelMin;
	int iLevelMax;
	int iReqStrength;
	int iReqZen;
	int iStartContextTextNumber;*/

} QUEST_CONDITION, * LPQUEST_CONDITION;


typedef struct QUEST_INFO	
{
	char Name[50];	// 0
	short QuestIndex;	// 34
	short QuestStartType;	// 38
	short QuestStartSubType;	// 3C
	short QuestSubInfoCount;	// 40
	QUEST_SUB_INFO QuestSubInfo[MAX_SUBINFO_QUEST];	// 44
	short QuestConditionCount;	// 314
	QUEST_CONDITION QuestCondition[MAX_CONDITION_QUEST];	// 318

} QUEST_INFO, * LPQUEST_INFO;


enum QUEST_STAGES {
	Q_STRENGTH_CERTIFICATE01	= 0xff,
	Q_STRENGTH_CERTIFICATE02	= 0xfd,

	Q_BARRACKS_INFILTRATION01	= 0xfe,
	Q_BARRACKS_INFILTRATION02	= 0xf6,

	Q_INTO_THE_DARKNESS01		= 0xfa,
	Q_INTO_THE_DARKNESS02		= 0xda,

	Q_END_SEASON3_QUEST			= 0xea,
};

enum QUEST_TRANSFORMS {
	QTR_GRAND_MASTER	= 31,
	QTR_BLADE_MASTER	= 63,
	QTR_HIGH_ELF		= 95,
	QTR_DUEL_MASTER		= 127,
	QTR_LORD_EMPEROR	= 159,
};

class CQuestInfo
{
public:

	CQuestInfo();
	virtual ~CQuestInfo();

	void Init();
	int IsQuest(int QuestIndex);
	int GetQuestCount(){return m_QuestCount;}
	int LoadQuestInfo(char* Buffer, int iSize);
	int LoadQuestInfo(char* filename);
	void InitQuestItem();
	LPQUEST_INFO GetQuestInfo(int QuestIndex);
	LPQUEST_SUB_INFO GetSubquestInfo(LPOBJ lpObj, LPQUEST_INFO lpQuestInfo, int subquestindex);
	LPQUEST_CONDITION GetQuestCondition(LPQUEST_INFO lpQuestInfo, int conditionindex);
	int GetQuestState(LPOBJ lpObj, int QuestIndex);
	BYTE GetQuestStateBYTE(LPOBJ lpObj, int QuestIndex);
	unsigned char SetQuestState(LPOBJ lpObj, int QuestIndex, int State);
	unsigned char ReSetQuestState(LPOBJ lpObj, int QuestIndex);
	unsigned char QuestClearConditionCheck(LPOBJ lpObj, int QuestIndex);
	unsigned char QuestRunConditionCheck(LPOBJ lpObj, int QuestIndex);
	int CompareCondition(LPOBJ lpObj, LPQUEST_CONDITION lpCondition);
	int NpcTalk(LPOBJ lpNpc, LPOBJ lpObj);
	int MonsterItemDrop(LPOBJ lpObj);
	int QuestAccept(LPOBJ lpObj, int QuestIndex);
	int QuestClear(LPOBJ lpObj, int QuestIndex);
	void QuestInfoSave(LPOBJ lpObj);
	void FixQuestInfo(LPOBJ lpObj);

private:

	short m_QuestCount;	// 4
	QUEST_INFO QuestInfo[MAX_QUEST_INFO];	// 8

};


extern CQuestInfo g_QuestInfo;

#endif