
#if (CRYSTAL_EDITION == 1)

struct sAbsorbDmg
{
	BYTE Opt1;
	BYTE Opt2;
	float Opt3;
	bool Loaded;
};

struct sIncreaseDmg
{
	BYTE Opt1;
	BYTE Opt2;
	float Opt3;
	bool Loaded;
};

struct sDefenseLevel
{
	BYTE Opt1;
	bool Loaded;
};

struct sExcAttrib
{
	BYTE Opt1;
	BYTE Opt2;
	BYTE Opt3;
	BYTE Opt4;
	bool Loaded;
};

struct sOptions
{
	sAbsorbDmg		AbsorbDmg;
	sIncreaseDmg	IncreaseDmg;
	sDefenseLevel	DefenseLevel;
	sExcAttrib		ExcAttrib;
};

class cCustomWings
{
public:
	void Read(char * FilePath);
	float GetAbsorbDmg(int ItemID, BYTE Level, int AttackDamage);
	int GetIncreaseDmg(int ItemID, BYTE Level, int AttackDamage);
	WORD GetDefenseLevel(int ItemID, BYTE Level, int Defense);
	BYTE GetExcOption(int ItemID, BYTE Option);
private:
	sOptions	Options[55];
};

extern cCustomWings CWings;

#endif