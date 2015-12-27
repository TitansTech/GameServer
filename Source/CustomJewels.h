#include "user.h"

#if (CRYSTAL_EDITION==1)
#define MAX_CUSTOM_JEWELS		20

struct CJewel_Prop
{
	WORD ItemID;
	BYTE IsJewel;
	BYTE MinLevel;
	BYTE MaxLevel;

	short hasToHaveLuck;		//DaRKav Jewel Limit Addon
	short hasToHaveSkill;
	short hasToBeAncient;
	short hasToBeExcellent;
	short hasToBeSoketItem;

	BYTE MinZ28Option;
	BYTE MaxExc;
	BYTE MinNumberSokets;
	BYTE MaxNumberSokets;


	BYTE Rate;
};

struct CJewel_Status
{
	WORD ItemID;

	short Level;
	short Option;
	short Luck;
	short Skill;
	short Excellent;
	short SetItem;
	short Sokets;
	short Destroy;
};


class CustomJewels
{
public:
	void Init();
	void Read(char * FilePath);

	int IsJewel(int ItemID);
	int GetFailPosition(int ItemID);
	int GetSuccessPosition(int ItemID);

	bool Apply(LPOBJ lpObj, int source, int target);

private:
	CJewel_Prop Property[MAX_CUSTOM_JEWELS];
	CJewel_Status Success[MAX_CUSTOM_JEWELS];
	CJewel_Status Fail[MAX_CUSTOM_JEWELS];

};

extern CustomJewels CJewel;

#endif
