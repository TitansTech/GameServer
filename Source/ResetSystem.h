#include "stdafx.h"
#include "user.h"

#if (DSGN_RESET_SYSTEM==0)
#if (PACK_EDITION>=2)

struct ResetStruct
{
	BOOL Enabled;
	BOOL ClearResets;
	BOOL ClearMasterPoints;
	BOOL EnabledWriteLog;
	int LimitCount;
#if (PACK_EDITION>=3)
	BOOL EnabledDBLog;
#endif
	BYTE System;
	int Level;
	BOOL ClearStats;
	BOOL ClearQuests;
	BOOL ClearEvolutions;
	BOOL ClearSkills;
	BOOL ClearInventory;
	BOOL ZenAutoIncrement;
	BOOL ClearLevelUpPoint;
	int AddPoints;
	int AddVIPPoints;
	int AddPCPoints;
	int  Zen;

	BOOL MailSend;
	char MailString[2048];

	BOOL ItemNeed;
	int ItemNeedType[7];
	int ItemNeedIndex[7];
	int ItemNeedLevel[7];
	int ItemNeedOpt[7];
	BOOL ItemNeedLuck[7];
	BOOL ItemNeedSkill[7];
	int ItemNeedCount[7];

	int System2Level[7];
	int System2Points[7];
};

class cResetSystem
{
public:
	void Init(char * FilePath, BYTE ResetType);
	void Start(LPOBJ lpObj);
	void SetPoints(LPOBJ lpObj, ResetStruct ResetTmp, int ResetNum, int & LvlUpPoint);
	void CheckSendMail(LPOBJ lpObj);
	void SendMail(LPOBJ lpObj);
//vars:
	
	ResetStruct Normal;
	ResetStruct Masters;
};

extern cResetSystem ResetChar;

#endif	//PACK_EDITION
#else

#endif	//DSGN_RESET_SYSTEM
