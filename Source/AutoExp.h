
#if (PACK_EDITION>=3)

class AutoExpClass
{
public:
	void Init(char * FilePath);

//Vars:
	BYTE Enabled;
	int	Time;
	short BaseExp;
	BYTE LevelUpPoints;	
};

extern AutoExpClass AutoExp;
void AutoExpClass__AddExp(void * lpParam);

#endif