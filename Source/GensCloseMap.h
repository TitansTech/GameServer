#include "stdafx.h"

#if (PACK_EDITION>=3)

struct GENMAPSSKILLS
{
	int Duprian;
	int Valiant;
};

class cGensCloseMap
{
public:
	void Init(char * FilePath);
	void StartEvent();

	BOOL Enabled;
	bool Start;
	bool CheckMap2(int aIndex, BYTE Map);
	bool CheckMap(int aIndex, BYTE Map);
	void AddKills(int aIndex);
	void Finish(char * FilePath);
	int Minutes;
	char * filePath;

	//BYTE Map;
private:
	BYTE Maps[MAX_MAP_NUMBER];
	GENMAPSSKILLS Kills[MAX_MAP_NUMBER];
	void Warp();
};

extern cGensCloseMap GensCloseMap;
void cGensCloseMap__InsideTrigger(void * lpParam);
#endif