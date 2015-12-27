
class MasteringClass
{
public:
	void AddSkillUpPoints(int aIndex,int Points);
	void LoadSkill(LPOBJ lpObj, int MasterPoints,int Skill);
	BYTE ReqSkillPoints(WORD skill);
	void CalcMagicPower(LPOBJ lpObj);
	void AddMagicPower(LPOBJ lpObj, DWORD MagicNr);
	void GCSkillAddRecv(PMSG_SKILLMASTERADD* lpMsg, int aIndex);
	BOOL CheckAddSkill (int PlayerID, DWORD Skill, int & SkillPosition);
	void InfoSend(int aIndex);
	void LevelUp(LPOBJ lpObj);
	void S6E2RestoreMagic(int aIndex, int Skill);
};

extern MasteringClass Mastering;
