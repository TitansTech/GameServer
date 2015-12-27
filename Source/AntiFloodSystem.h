#include "stdafx.h"

#define MAXIPINBLACKLIST	2048
#define BLISTFILE			"..\\SCFData\\SCF_BlackList.dat"
#define BLISTCFGFILE		"..\\SCFData\\SCF_BlackList.ini"

class cAntiFlood
{
public:
	void Init();
	bool Check(char * IP);
	bool AddToBlackList(LPSTR IP);
private:
	void ReadBlackList(LPSTR filename);
	void BlackListClean();
	void AutoClose(char * IP);

	bool BL_IPCheck(char * IP);
	int GetIPCount(char * IP);

	BOOL Enabled;
	BOOL IsAutoAdd;
	BOOL AutoCloseFloodCon;
	int BLSize;
	int MaxIPConnection;
	char BlackListIP[MAXIPINBLACKLIST][16];
};

extern cAntiFlood AntiFlood;