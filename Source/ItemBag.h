#ifndef ITEMBAG_H
#define ITEMBAG_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAX_ITEMBAG_ATTR 150

class CItemBagAttr
{

public:

	CItemBagAttr()	// line : 28
	{
		this->m_type=0;
		this->m_index=0;
		this->m_level=0;
		this->m_op1=0;
		this->m_op2=0;
		this->m_op3=0;
	}	// line : 36

public:

	BYTE m_type;	// 0
	BYTE m_index;	// 1
	BYTE m_level;	// 2
	BYTE m_op1;	// 3
	BYTE m_op2;	// 4
	BYTE m_op3;	// 5

};


class CItemBag
{

public:

	CItemBag();
	virtual ~CItemBag();

	void Init(LPSTR name);
	void LoadItem(LPSTR script_file);
	int GetBagCount()	// line : 62
	{
		return this->BagObjectCount;
	};	// line : 64
	BYTE GetType(int n);
	BYTE GetIndex(int n);
	BYTE GetLevel(int n);
	BYTE GetOp1(int n);
	BYTE GetOp2(int n);
	BYTE GetOp3(int n);

private:

	CItemBagAttr BagObject[MAX_ITEMBAG_ATTR];	// 4
	int BagObjectCount;	// 388

/*	CItemBagAttr m_cItemBagAttr[MAX_ITEMBAG_ATTR];	// unk4
	int m_iItemBagCount;	// unk388
	int m_iNormalItemCount;	// unk38C
	int m_iExItemCount;	// unk390*/

};

#endif