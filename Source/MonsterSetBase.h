#ifndef MONSTERSETBASE_H
#define MONSTERSETBASE_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "user.h"


typedef struct MONSTER_POSITION
{
	BYTE m_ArrangeType;	// 0
	WORD m_Type;	// 2
	BYTE m_MapNumber;	// 4
	BYTE m_Dis;	// 5
	BYTE m_X;	// 6
	BYTE m_Y;	// 7
	BYTE m_Dir;	// 8
	BYTE m_W;	// 9
	BYTE m_H;	// A

/*	BYTE btType;	// unk0
	WORD wMonsterIndex; // unk2
	BYTE btMapNumber;	// unk4
	BYTE btDistance;	// unk5
	BYTE btStartX;	// unk6
	BYTE btStartY;	// unk7
	BYTE btDirection;	// unk8
	BYTE btEndX;	// unk9
	BYTE btEndY;	// unkA*/

} MONSTER_POSITION, * LPMONSTER_POSITION;

void gObjMonsterAddSet(int Number, int Map, int X, int Y, int Count);
void ReadMonstersAndAdd(char * FilePath);

class CMonsterSetBase
{

public:

	void LoadSetBase(char* Buffer, int iSize);
	void LoadSetBase(char* filename);

	CMonsterSetBase();
	virtual ~CMonsterSetBase();

	int GetPosition(int TableNum, short MapNumber, short& x, short& y);
	int GetBoxPosition(int mapnumber, int ax, int ay, int aw, int ah, short& mx, short& my);
	void SetBoxPosition(int TableNum, int mapnumber, int ax, int ay, int aw, int ah);

public:

  MONSTER_POSITION m_Mp[OBJ_MAXMONSTER];	// 4
  int m_Count;	// 10FE4


};

#endif