// CastleCrown.h: interface for the CCastleCrown class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CASTLECROWN_H__1494B89A_2CEA_41C6_9B1A_C5CF4064943F__INCLUDED_)
#define AFX_CASTLECROWN_H__1494B89A_2CEA_41C6_9B1A_C5CF4064943F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#if (GS_CASTLE==1)

class CCastleCrown  
{
public:
	CCastleCrown();
	virtual ~CCastleCrown();

public:
	void CalcAccessCrownTime(int iCrownIndex);
};

extern CCastleCrown g_CastleCrown;

#endif

#endif // !defined(AFX_CASTLECROWN_H__1494B89A_2CEA_41C6_9B1A_C5CF4064943F__INCLUDED_)
