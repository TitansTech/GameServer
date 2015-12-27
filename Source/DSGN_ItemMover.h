#ifndef ITEM_MOVE_H
#define ITEM_MOVE_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "stdafx.h"

#if (DSGN_ITEM_DBMOVE == 1)

#define	MAX_REC_ITEM_MACRO		2048

struct DSGNRECITEMLIST
{
	char loaded;
	int oldIndex;		// 0
	int oldDBNumber;	// 4
	int newIndex;		// 8
	int newDBNumber;	// 4
};

class DSGN_Item_Mover
{
public:
	void Init();
	void LoadItem(LPSTR script_file);
	int CheckTransformItem(int mNumber, int DbVersion);

	BYTE highestDBNumber;

private:
	int ItemObjectCount;
	DSGNRECITEMLIST recArray[MAX_REC_ITEM_MACRO];

};

extern DSGN_Item_Mover DS_Mover;

#endif
#endif
