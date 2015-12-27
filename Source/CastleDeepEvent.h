// CastleDeepEvent.h: interface for the CCastleDeepEvent class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CASTLEDEEPEVENT_H__F6667979_5AD8_4879_AA04_5C25C47DFADE__INCLUDED_)
#define AFX_CASTLEDEEPEVENT_H__F6667979_5AD8_4879_AA04_5C25C47DFADE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#if (GS_CASTLE==1)

#define CASTLE_STATE_NONE 0
#define CASTLE_STATE_CLOSED 1
#define CASTLE_STATE_PLAYING 2

struct CASTLEDEEP_MONSTERINFO // <size 0x1c>
{
	short m_iMonsterGroup; // 0
	short m_iMonsterType; // 4
	short m_iMonsterNumber; // 8
	BYTE m_iSX; // c
	BYTE m_iSY; // 10
	BYTE m_iDX; // 14
	BYTE m_iDY; // 18
};


struct CASTLEDEEP_SPAWNTIME // <size 0x8>
{
	short m_iMonsterGroup; // 0
	BYTE m_iMonsterSpawnMinute; // 4
};

struct CASTLEDEEP_EVENTTIME // <size 0x8>
{
	char m_iHour; // 0
	char m_iMinute; // 4
};




class CCastleDeepEvent  
{
public:
	CCastleDeepEvent();
	virtual ~CCastleDeepEvent();
//
	void StartEvent();
	void StopEvent();
	void EnableEvent(BOOL bDoEvent);
	BOOL Load(char* lpszFileName);
	void Run();
	void CheckSync();
	void SetState(int iSTATE);
	void ProcState_None();
	void ProcState_Closed();
	void ProcState_Playing();
	void SetState_None();
	void SetState_Closed();
	void SetState_Playing();
	void ClearMonster();
	void AddMonster(int iAssultType, int iGroup);
 

public:
	BOOL m_bDoEvent; // 4
	BOOL m_bHasData; // 8
	BOOL m_bCheckEnd; // c
	DWORD m_uCheckEndTick; // 10
	int m_iTIME_MSEC_REMAIN; // 14
	int m_iTIME_MIN_PLAY; // 18
	int m_iTIME_TICK_COUNT; // 1c
	BYTE m_iTIME_NOTIFY_COUNT; // 20
	BYTE m_iEVENT_STATE; // 24
	BYTE m_iCUR_ASSULT_TYPE; // 28
	
	std::map<int, std::vector<CASTLEDEEP_MONSTERINFO> > m_mapMonsterInfo; // 30
	std::map<int, std::vector<CASTLEDEEP_SPAWNTIME> > m_mapSpawnTime;// 40
	std::vector<CASTLEDEEP_EVENTTIME> m_vtEventTime; // 50
	std::vector<int> m_vtAssultType; // 60
	std::vector<int> m_vtAssultTypeRate; // 70
	std::map<int, int> m_mapRunCheck; // 80
	CRITICAL_SECTION m_critEventData; // 90
};


extern CCastleDeepEvent g_CastleDeepEvent;
#endif 

#endif // !defined(AFX_CASTLEDEEPEVENT_H__F6667979_5AD8_4879_AA04_5C25C47DFADE__INCLUDED_)