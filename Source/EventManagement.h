#ifndef EVENTMANAGEMENT_H
#define EVENTMANAGEMENT_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

void CEventManagement__SCFEventManager(void * lpParam);
extern BOOL g_bSCFEventManagerOn;

struct EVENT_ID_TIME
{
	BYTE m_eEventKind;	// 0
	char m_iHour;	// 4
	char m_iMinute;	// 8
	bool m_bEventStarted;	// C
};

struct SCFEVENT_ID_TIME
{
	BYTE m_eEventKind;	// 0
	char m_iHour;	// 4
	char m_iMinute;	// 8
	char m_iMonth;
	char m_iDay;
	char m_iDayOfWeek;
};

class CEventManagement
{
public:

	CEventManagement();
	virtual ~CEventManagement();

	void Clear();
	void Init(BOOL bEventStart);
	bool Load(LPSTR lpszFileName);
	void Run();
	void StartEvent(int eEventKind);
	void RegEvent(int eEventKind, void* lpEventObj);
	//
	void SCFInit(BOOL Start);
	BOOL SCFEventManagerRunning;
	SCFEVENT_ID_TIME SCFEvent[255];
	BYTE SCFEventCount;


private:

	BOOL m_bEventStart;	// 4
	BOOL m_bHasData;	// 8
	std::vector<EVENT_ID_TIME> m_vtEventTime;	// C
	std::map<int,void *> m_mapEventObj;	// 1C
	WORD m_wToday_Year;	// 2C
	BYTE m_wToday_Month;	// 2E
	BYTE m_wToday_Day;	// 30

	enum {
	  EVENT_ID_DRAGONHERD = 0x0,
	  EVENT_ID_UNDERTROOP = 0x1,
	  EVENT_ID_ELDORADO = 0x2,
	  EVENT_ID_WHITEMAGE = 0x3,
	};
};

extern CEventManagement g_EventManager;

//----------------------------------------------------------



#endif