#ifndef MERCENARY_H
#define MERCENARY_H

#include "user.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#if (GS_CASTLE==1)
class CMercenary
{

public:

	CMercenary();
	virtual ~CMercenary();

	//int __thiscall UseLifeStoneScroll(int);
	BOOL CreateMercenary(int iIndex, int iMercenaryTypeIndex, BYTE cTX, BYTE cTY);
	BOOL DeleteMercenary(int iIndex);
	int SearchEnemy(struct OBJECTSTRUCT* lpObj);
	void MercenaryAct(int iIndex);


private:

	BYTE  m_iMercenaryCount;	// 4


};

extern CMercenary g_CsNPC_Mercenary;

#endif
#endif