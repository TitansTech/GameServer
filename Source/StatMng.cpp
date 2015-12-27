// ------------------------------
// Decompiled by Deathway
// Date : 2007-05-09
// ------------------------------
#include "stdafx.h"
#include "StatMng.h"
#include "user.h"

// GS-N 0.99.60T 0x004A4121
//	GS-N	1.00.18	JPN	0x004C1DD0	-	Completed

CStatMng gStatMng;

CStatMng::CStatMng()
{
	this->Init();
}

CStatMng::~CStatMng()
{
	return;
}

void CStatMng::Init()
{
	int firstState = 2;
	int MagunfirstState = 2;
	int iDarkLordFirstState = 2;

	for (int i=0;i<STAT_TABLE_SIZE;i++)
	{
		if ( ( (i+1) % 10) == 0)
		{
			firstState = firstState + ( i * 2 + 22 ) / 400 + 2;
			MagunfirstState = MagunfirstState + ( i * 2+ 22 ) / 700 + 2;
			iDarkLordFirstState = iDarkLordFirstState + ( i * 2 + 22 ) / 500 + 2;
		}

		this->gObjNormalAddStatTable[i] = firstState;
		this->gObjMagumAddStatTable[i] = MagunfirstState;
		this->gObjDarkLordAddStatTable[i] = iDarkLordFirstState;
	}

	firstState = 2;
	MagunfirstState = 2;
	iDarkLordFirstState = 2;

	for (int i=0;i<STAT_TABLE_SIZE;i++)
	{
		if ( ( (i+1) % 10) == 0)
		{
			firstState = firstState + ( i * 2 + 22 ) / 400 + 2;
			MagunfirstState = MagunfirstState + ( i * 2+ 22 ) / 700 + 2;
			iDarkLordFirstState = iDarkLordFirstState + ( i * 2 + 22 ) / 500 + 2;
		}

		this->gObjNormalMinusStatTable[i] = firstState;
		this->gObjMagumMinusStatTable[i] = MagunfirstState;
		this->gObjDarkLordMinusStatTable[i] = iDarkLordFirstState;
	}
}


int CStatMng::GetMaxStat(int level, int Class)
{
	if ( (level < 1) || (level> MAX_CHAR_LEVEL) )
	{
		return 0;
	}

	if (Class == CLASS_MAGICGLADIATOR || Class == CLASS_RAGEFIGHTER)
	{
		return this->gObjMagumAddStatTable[level-1];
	}
	
	if ( Class == CLASS_DARKLORD )
	{
		return this->gObjDarkLordAddStatTable[level-1];
	}
	
	return this->gObjNormalAddStatTable[level-1];
}


int CStatMng::GetMaxMinusStat(int iLevel, int iClass)
{
	if ( (iLevel < 1) || (iLevel> MAX_CHAR_LEVEL) )
	{
		return 0;
	}

	if ( iClass == CLASS_MAGICGLADIATOR || iClass == CLASS_RAGEFIGHTER)
	{
		return this->gObjMagumMinusStatTable[iLevel-1];
	}
	
	if ( iClass == CLASS_DARKLORD )
	{
		return this->gObjDarkLordMinusStatTable[iLevel-1];
	}
	
	return this->gObjNormalMinusStatTable[iLevel-1];
}