// CrywolfTanker.h: interface for the CCrywolfTanker class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CRYWOLFTANKER_H__8CBC3465_0A8F_4932_825C_4BC8F77DA823__INCLUDED_)
#define AFX_CRYWOLFTANKER_H__8CBC3465_0A8F_4932_825C_4BC8F77DA823__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#if (GS_CASTLE==1)

class CCrywolfTanker
{

public:

  CCrywolfTanker();
  virtual ~CCrywolfTanker();

  void CrywolfTankerAct(int iIndex);

};

extern CCrywolfTanker g_CrywolfMON_Tanker;

#endif
#endif // !defined(AFX_CRYWOLFTANKER_H__8CBC3465_0A8F_4932_825C_4BC8F77DA823__INCLUDED_)
