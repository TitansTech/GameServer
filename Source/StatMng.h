// ------------------------------
// Decompiled by Deathway
// Date : 2007-05-09
// ------------------------------
#ifndef STATMNG_H
#define STATMNG_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define STAT_TABLE_SIZE	500

class CStatMng
{

public:

	CStatMng();
	virtual ~CStatMng();

	void Init();
	int GetMaxStat(int level, int Class);
	int GetMaxMinusStat(int iLevel, int iClass);

private:

	int gObjNormalAddStatTable[STAT_TABLE_SIZE];	// 4
	int gObjMagumAddStatTable[STAT_TABLE_SIZE];	// 7D4
	int gObjDarkLordAddStatTable[STAT_TABLE_SIZE];	// FA4
	int gObjNormalMinusStatTable[STAT_TABLE_SIZE];	// 1774
	int gObjMagumMinusStatTable[STAT_TABLE_SIZE];	// 1F44
	int gObjDarkLordMinusStatTable[STAT_TABLE_SIZE];	// 2714

};

extern CStatMng gStatMng;

#endif