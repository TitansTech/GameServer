
#if (PACK_EDITION>=3)


#define cRainItemEvent_DropItemPos_COUNT	100

struct cRainItemEvent_DropItemPos
{
	BYTE map;
	BYTE x;
	BYTE y;
};

class cRainItemEvent
{
public:
	void StartEvent();
	void ReadPositions(char * FilePath);
	bool Start;
	BYTE posCount;
	cRainItemEvent_DropItemPos pos[cRainItemEvent_DropItemPos_COUNT];
};

extern cRainItemEvent RainItemEvent;

#endif