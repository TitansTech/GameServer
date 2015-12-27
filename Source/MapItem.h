// ------------------------------
// Decompiled by Deathway
// Date : 2007-05-09
// ------------------------------
#ifndef MAPITEM_H
#define MAPITEM_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "..\common\zzzitem.h"
#include "user.h"


class CMapItem : public CItem
{

public:

	CMapItem();
	virtual ~CMapItem();

	void Init();
	void CreateItem(int type, int level, int x, int y, float dur, BYTE Option1, BYTE Option2, BYTE Option3, BYTE NOption, BYTE SOption, DWORD number, BYTE ItemEffectEx);
	void DropCreateItem(int type, int level, int x, int y, float dur, BYTE Option1, BYTE Option2, BYTE Option3, BYTE NOption, BYTE SOption, DWORD number, int PetLevel, int PetExp, BYTE ItemEffectEx, BYTE ItemSocket1, BYTE ItemSocket2, BYTE ItemSocket3, BYTE ItemSocket4, BYTE ItemSocket5);

public:
	
	BYTE px;	// B4
	BYTE py;	// B5
	bool live;	// B6
	bool Give;	// B7
	DWORD m_State;	// B8
	DWORD m_Time;	// BC
	int m_UserIndex;	// C0
	DWORD m_LootTime;	// C4
	VIEWPORT_PLAYER_STRUCT VpPlayer[MAX_VIEWPORT];	// C8
	int VPCount;	// 44C
};


#endif