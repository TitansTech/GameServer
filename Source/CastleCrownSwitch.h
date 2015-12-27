// CastleCrownSwitch.h: interface for the CCastleCrownSwitch class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CASTLECROWNSWITCH_H__E1DF5DA3_E594_4900_AF02_A71558C39951__INCLUDED_)
#define AFX_CASTLECROWNSWITCH_H__E1DF5DA3_E594_4900_AF02_A71558C39951__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#if (GS_CASTLE==1)

class CCastleCrownSwitch  
{
public:
	CCastleCrownSwitch();
	virtual ~CCastleCrownSwitch();

public:
	void CrownSwitchPushTime(int iSwitchIndex);
};

extern CCastleCrownSwitch g_CastleCrownSwitch;
#endif

#endif // !defined(AFX_CASTLECROWNSWITCH_H__E1DF5DA3_E594_4900_AF02_A71558C39951__INCLUDED_)
