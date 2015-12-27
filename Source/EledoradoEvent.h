// ------------------------------
// Decompiled by Deathway
// Date : 2007-05-09
// ------------------------------
#ifndef ELEDORADOEVENT_H
#define ELEDORADOEVENT_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CEledoradoEvent
{

public:

	CEledoradoEvent();
	virtual ~CEledoradoEvent();

	void Init();
	void SetEventState(int State);
	int  GetEventState();
	void Run();
	void RegenGoldGoblen();
	void RegenTitan();
	void RegenGoldDercon();
	void RegenDevilLizardKing();
	void RegenKantur();
	void CheckGoldDercon(int MapNumber);
	void SetMenualStart(BOOL bMenualStart){this->m_bMenualStart = bMenualStart;}	// line : 57
	void Start_Menual();
	void End_Menual();
	void RegenElDorado2(int MobClass);
public:

	int EventState;	// 4
	
	DWORD GoldGoblenEventStartTime;	// 8
	DWORD TitanEventStartTime;	// C
	DWORD GoldDerconEventStartTime;	// 10
	DWORD DevilLizardKingEventStartTime;	// 14
	DWORD KanturEventStartTime;	// 18

	BYTE m_BossTitanMapNumber;	// 1C
	BYTE m_BossTitanMapX;	// 20
	BYTE m_BossTitanMapY;	// 24
	
	BYTE m_BossDevilLizardKingMapNumber;	// 28
	BYTE m_BossDevilLizardKingMapX;	// 2C
	BYTE m_BossDevilLizardKingMapY;	// 30
	
	BYTE m_BossKanturMapNumber;	// 34
	BYTE m_BossKanturMapX;	// 38
	BYTE m_BossKanturMapY;	// 3C

	BOOL m_bMenualStart;	// 40
	BYTE m_BossGoldDerconMapNumber[3];	// 44

	DWORD BossElDorado2StartTime[10];	// 18

	BYTE m_BossElDorado2MapNumber[10];	// 28
	BYTE m_BossElDorado2MapX[10];	// 2C
	BYTE m_BossElDorado2MapY[10];	// 30
};

extern CEledoradoEvent gEledoradoEvent;

#endif