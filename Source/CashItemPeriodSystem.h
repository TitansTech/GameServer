// CashItemPeriodSystem.h: interface for the CCashItemPeriodSystem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CASHITEMPERIODSYSTEM_H__11ADB3C6_2988_4060_913A_824DCFA381E8__INCLUDED_)
#define AFX_CASHITEMPERIODSYSTEM_H__11ADB3C6_2988_4060_913A_824DCFA381E8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "user.h"

#define MAX_ITEM_PERIOD_INFO	1000

struct PMSG_ANS_PERIODITEM_LIST
{
	PBMSG_HEAD2 head;	// 0
	int iUserIndex;	// 4
	int iItemCode;	// 8
	BYTE btEffectType1;	// C
	BYTE btEffectType2;	// D
	int iLeftTime;	// 10
	int iUserGuid;	// 14
	char btResult;	// 18
};


struct PMSG_ANS_PERIODITEM_INSERT
{
	PBMSG_HEAD2 head;
	int iUserIndex;	// 4
	int iItemCode;	// 8
	int iItemPeriodDate;	// C
	BYTE btItemEffectType1;	// 10
	BYTE btItemEffectType2;	// 11
	BYTE btResult;	// 12
};


struct PMSG_ANS_PERIODITEM_INSERT_RESULT
{
	PBMSG_HEAD2 head;	// C1:D0:03
	BYTE btResult;	// 4
};


struct ITEMPERIOD_INFO
{
	BYTE btUsedInfo;	// 0
	int iUserIndex;	// 4
	int iUserGuid;	// 8
	char chCharacterName[11];	// C
	int iItemCode;	// 18
	BYTE btEffectType1;	// 1C
	BYTE btEffectType2;	// 1D
	int iItemPeriodUsedTime;	// 20
	int iItemPeriodLeftTime;	// 24
	char chItemPeriodBuyDate[20];	// 28
	char chItemPeriodEndDate[20];	// 3C
	DWORD dwItemCheckTickCount;	// 50
};


struct PMSG_ANS_PERIODITEM_INQUIRY
{
	PBMSG_HEAD2 head;
	int iUserIndex;	// 4
	BYTE btResult;	// 8
};


struct PMSG_ANS_PERIODITEM_UPDATE
{
	PBMSG_HEAD2 head;
	int iUserIndex;	// 4
	int iItemCode;	// 8
	BYTE btResult;	// C
};


struct PMSG_ANS_PERIODITEM_DELETE
{
	PBMSG_HEAD2 head;
	int iUserIndex;	// 4
	int iUserGuid;	// 8
	BYTE btResult;	// C
};



class CCashItemPeriodSystem
{

public:

	CCashItemPeriodSystem();
	virtual ~CCashItemPeriodSystem();

	void Initialize();
	int SetPeriodItemEffect(LPOBJ lpObj, int iItemCode,BYTE btEffectType1, BYTE btEffectType2, DWORD dwItemPeriod);
	int ClearPeriodItemEffect(LPOBJ lpObj);
	void GDReqPeriodItemList(LPOBJ lpObj);
	void GDReqPeriodItemInsert(LPOBJ lpObj, int iItemCode, int iItemEffectDate);
	void GDReqPeriodItemInquiry(LPOBJ lpObj);
	void GDReqPeriodItemUpdate(LPOBJ lpObj);
	void DGAnsPeriodItemList(PMSG_ANS_PERIODITEM_LIST* aRecv);
	void DGAnsPeriodItemInsert(PMSG_ANS_PERIODITEM_INSERT* aRecv);
	void DGAnsPeriodItemInquiry(PMSG_ANS_PERIODITEM_INQUIRY* aRecv);
	void DGAnsPeriodItemUpdate(PMSG_ANS_PERIODITEM_UPDATE* aRecv);
	void GDReqPeriodItemDelete(LPOBJ lpObj, LPSTR pchCharacterName);
	void DGAnsPeriodItemDelete(PMSG_ANS_PERIODITEM_DELETE* aRecv);
	void GCSendPeriodItemInserResult(LPOBJ lpObj, BYTE btResult);
	int CalcItemUseLeftTime(LPSTR pchBuyDate);
	class CTime CalcItemUseEndTime(int iLeftTime);
	void ConvertStringToTime(const char * pchDate,class CTime* lpTime);
	void ConvertTimeToString(const class CTime* lpTime, LPSTR pchDate);

	static DWORD WINAPI CCashItemPeriodSystem::PeriodCheckThread(void* lpPeriodInfo);

private:

	int iItemPeriodInfoCount;	// 4
	ITEMPERIOD_INFO ItemPeriodInfo[MAX_ITEM_PERIOD_INFO];	// 8 #error was the original size 928
	HANDLE hThreadHandle;	// 130B8
};


extern CCashItemPeriodSystem g_CashItemPeriodSystem;

#endif // !defined(AFX_CASHITEMPERIODSYSTEM_H__11ADB3C6_2988_4060_913A_824DCFA381E8__INCLUDED_)
