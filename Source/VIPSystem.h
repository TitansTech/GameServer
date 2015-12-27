#include "stdafx.h"


#if (PACK_EDITION>=2)

#include "user.h"

struct sVIPShop
{
	int Cost;
	int Type;	
	int Index;
	int Skill;
	int Level;
	int Luck;
	int Opt;
	int Dur;
	int Exc;

	int NewOpt1;
	int NewOpt2;
	int NewOpt3;
	int NewOpt4;
	int NewOpt5;

	char sType[512];
	char sName[512];
};
class cVipSystem
{
public:
	void Init(char * FilePath);
	bool LoadShopItem(char* filename );
	void BuyList(LPOBJ lpObj, char * sType);
	void BuyItem(LPOBJ lpObj, int IDNumber);
	void BuyDays(LPOBJ lpObj, int Days);
	void State(LPOBJ lpObj);
//Vars:
	BYTE VipIsApplyCWExpPenalty;
	BYTE VipIsApplyResetExpPenalty;
	BOOL ResetFreeLimitEnabled;
	int ResetFreeLimitMax;
	int VipExperience;
	BOOL IsVipSwitchServer;
	BOOL IsVipServer;

	BOOL VipBuy;
	int VipRenewPoints;

	sVIPShop Shop[1024];
	CItem m_item[1024];	// 80
	int ItemsCount;
	BOOL IsConnectMemberReload;
	int ConnectMemberReload_Mins;
	BOOL ConnectMemberReload_Status;
};

extern cVipSystem VipSystem;

#endif