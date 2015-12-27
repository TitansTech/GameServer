#ifndef GMMNG_H
#define GMMNG_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAX_GM_COMMAND 200
#define MAX_GMCOMMAND_LEN 20
#define MAX_GAME_MASTER 5


#include "user.h"


class CGMCommand
{

public:


	CGMCommand() // line : 119
	{
		this->Init();
	}	// line : 122

	void Init() // line : 125
	{
		for ( int n = 0; n<  MAX_GM_COMMAND; n++ )
		{
			this->nCmdCode[n] = -1;	// #error Change the Zero for n -> FIXED ALREADY
		}

		this->count=0;
	}	// line : 128

	void Add(LPSTR command_str, int command_code) // line : 131
	{
		strcpy(this->szCmd[this->count], command_str);
		this->nCmdCode[this->count]=command_code;

		if ( this->count < MAX_GM_COMMAND-1 )
		{
			this->count++;
		}
	}	// line : 135


public:
	
	char szCmd[MAX_GM_COMMAND][MAX_GMCOMMAND_LEN];	// 0
	int nCmdCode[MAX_GM_COMMAND];	// 7D0
	int count;	// 960
};



class CGMMng
{

public:

	CGMMng();
	virtual ~CGMMng();

	void Init();
	int GetCmd(LPSTR szCmd);
	int ManagementProc(LPOBJ lpObj, LPSTR szCmd, int aIndex);
	LPSTR GetTokenString();
	int GetTokenNumber();
	void GetInfinityArrowMPConsumption(LPOBJ lpObj);
	void ControlInfinityArrowMPConsumption0(LPOBJ lpObj, int iValue);
	void ControlInfinityArrowMPConsumption1(LPOBJ lpObj, int iValue);
	void ControlInfinityArrowMPConsumption2(LPOBJ lpObj, int iValue);
	void SetInfinityArrowTime(LPOBJ lpObj, int iValue);
	void ControlFireScreamDoubleAttackDistance(LPOBJ lpObj, int iValue);
	void ManagerInit();
	int  ManagerAdd(LPSTR name, int aIndex);
	void ManagerDel(LPSTR name);
	void ManagerSendData(LPSTR szMsg, int size);
	void BattleInfoSend(LPSTR Name1, BYTE score1, LPSTR Name2, BYTE score2);
	void DataSend(LPBYTE szMsg, int size);


private:

	CGMCommand cCommand;	// 4
	char szManagerName[MAX_GAME_MASTER][11];	// 968
	int ManagerIndex[MAX_GAME_MASTER];	// 9A0

public:

	int WatchTargetIndex;	// 9B4

};
void ServerMsgSendPost(int Type,char Sender[20],const char*Message,...);
extern CGMMng cManager;

#endif