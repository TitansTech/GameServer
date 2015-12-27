// CrywolfStateTimeInfo.h: interface for the CCrywolfStateTimeInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CRYWOLFSTATETIMEINFO_H__0CB24640_4832_4A12_85E0_88DF55431959__INCLUDED_)
#define AFX_CRYWOLFSTATETIMEINFO_H__0CB24640_4832_4A12_85E0_88DF55431959__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#if (GS_CASTLE==1)

class CCrywolfStateTimeInfo
{

public:

	CCrywolfStateTimeInfo()
	{
		this->Reset();
	}

	void Reset()
	{
		this->m_bUsed = FALSE;
		this->m_iStateNumber = -1;
		this->m_iMonth = -1;	
		this->m_iDay = -1;	
		this->m_iDayOfWeek = -1;	
		this->m_iHour = -1;	
		this->m_iMinute = -1;
		this->m_iContinuanceTime = 0;	
		this->m_dwAppliedTickCount = 0;	
	}

	void SetAppliedTime()
	{
		this->m_dwAppliedTickCount = GetTickCount();
	}

	void ResetAppliedTime()
	{
		this->m_dwAppliedTickCount = 0;
	}

	BOOL CheckContinuanceTime()
	{
		if ( (GetTickCount() - this->m_dwAppliedTickCount ) > this->m_iContinuanceTime )
		{
			return FALSE;
		}

		return TRUE;
	}

	BOOL CheckScheduleTime()
	{
		CTime tCurrentTime = CTime::GetTickCount();

		int iMonth = tCurrentTime.GetMonth();
		int iDay = tCurrentTime.GetDay();
		int iDayOfWeek = tCurrentTime.GetDayOfWeek();
		int iHour = tCurrentTime.GetHour();
		int iMinute = tCurrentTime.GetMinute();

		if ((this->m_iMonth == -1)||(this->m_iMonth == iMonth))
		{
			if ((this->m_iDay == -1)||(this->m_iDay == iDay))
			{
				if ((this->m_iDayOfWeek == -1)||(this->m_iDayOfWeek == iDayOfWeek))
				{
					if ((this->m_iHour == -1)||(this->m_iHour == iHour))
					{
						if ((this->m_iMinute != -1 )&&((this->m_iMinute == iMinute)||(this->m_iMinute == iMinute+1)))
						{
							return TRUE;
						}
					}
				}
			}
		}

		return FALSE;
	}

	int GetLeftTime()
	{
		if ( this->CheckContinuanceTime() )
		{
			int iLeftTime = this->m_iContinuanceTime - (GetTickCount() - this->m_dwAppliedTickCount);

			if ( iLeftTime < 0 )
				iLeftTime = 0;

			return iLeftTime;
		}

		return 0;
	}

public:

	BOOL m_bUsed;	// 0
	char m_iStateNumber;	// 4
	char m_iMonth;	// C
	char m_iDay;	// 10
	char m_iDayOfWeek;	// 14
	char m_iHour;	// 18
	char m_iMinute;	// 1C
	int m_iContinuanceTime;	// 24
	DWORD m_dwAppliedTickCount;	// 28

};

#endif
#endif // !defined(AFX_CRYWOLFSTATETIMEINFO_H__0CB24640_4832_4A12_85E0_88DF55431959__INCLUDED_)
