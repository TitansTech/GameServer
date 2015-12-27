#ifndef PARTYCLASS_H
#define PARTYCLASS_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "user.h"


#define MAX_USER_IN_PARTY 5
#define MAX_PARTY_DISTANCE_EFFECT	10

struct PARTY_STRUCT
{
	int Count;	// 0
	int Number[MAX_USER_IN_PARTY];	// 4
	int DbNumber[MAX_USER_IN_PARTY];	// 18
	short m_MaxLevel;	// 2C
	short m_MinLevel;	// 2E

};


class PartyClass
{

public:

	PartyClass();
	virtual ~PartyClass();

	BOOL IsParty(int party_number);
	int Create(int usernumber, int dbnumber, int level);
	BOOL Destroy(int party_number);
	int Add(int party_number, int usernumber, int dbnumber,  int level);
	void Delete(int party_number, int index);
	int Delete(int party_number, int usernumber, int dbnumber);
	int GetCount(int party_number);
	int GetIndexUser(int party_number, int index, int& usernumber,  int& dbnumber);
	int GetPartyCount(int party_number);
	int GetIndex(int party_number, int usernumber, int dbnumber);
	BOOL SetLevel(int party_number, int level);
	BOOL GetLevel(int party_number, int& maxlevel, int& minlevel);
	BOOL Isleader(int party_number, int usernumber, int dbnumber);
	void Paint(int party_number);
	void PartyMemberLifeSend(int party_number);

public:

	int m_PartyCount;	// 4
	PARTY_STRUCT m_PartyS[OBJMAX];	// 8
	char m_szTempPaint[256];	// 56B88

};


#endif