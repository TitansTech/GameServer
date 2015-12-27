

class PvPSystem
{
public:
	void Read(char * FilePath);
	BOOL Check(int Map);
	BYTE GetType(int Map);
	void SetType(int Map, BYTE Type);
	
	BOOL gNonPKServer;
	BOOL gPkLimitFree;
	BOOL gPkCanBuy;
	BYTE gPkItemDrop;
	BYTE gPkItemDropPercent;
	BYTE gPKDropMaxLevel;
	BYTE gPKDropMaxExc;
	BYTE gPKLevelIncreaseOff;
	int gPkTimeInMinutes;
	int gHeroExtraDropPercent;

	WORD gObjPlayerKiller_PKMap;
	BYTE gObjPlayerKiller_PKX1;
	BYTE gObjPlayerKiller_PKY1;
	BYTE gObjPlayerKiller_PKX2;
	BYTE gObjPlayerKiller_PKY2;

private:
	BOOL Enabled;
	BYTE MapPvP[MAX_MAP_NUMBER];
};

extern PvPSystem PvP;