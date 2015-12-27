// LifeStone.h: interface for the CLifeStone class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LIFESTONE_H__9E5B6740_42C2_41D9_AB3E_74F5FB0DE015__INCLUDED_)
#define AFX_LIFESTONE_H__9E5B6740_42C2_41D9_AB3E_74F5FB0DE015__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#if (GS_CASTLE==1)


class CLifeStone  
{
public:
	CLifeStone();
	virtual ~CLifeStone();

	void LifeStoneAct(int iIndex);
	void SendLifeStoneState(int iIndex);
	BOOL CreateLifeStone(int iIndex);
	int DeleteLifeStone(int iIndex);
	int SetReSpawnUserXY(int iUserIndex);
};

extern CLifeStone g_CsNPC_LifeStone;

#endif

#endif // !defined(AFX_LIFESTONE_H__9E5B6740_42C2_41D9_AB3E_74F5FB0DE015__INCLUDED_)
