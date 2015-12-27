


struct MASTERSKILLS2_DATA
{
	DWORD Type;
	DWORD RequireSkill[2];
	BYTE Category;
	BYTE Ranking;
	DWORD OriginalSkill;
	BYTE LevelReq;
	BYTE LevelMax;
	float Value;
	BYTE RequireClass[MAX_TYPE_PLAYER]; // 3E
};

#define MAXSKILLTREECOUNT			512

class cMastering2
{
public:
	void Load(char * FilePath);
	void GCMagicListMultiSend(int aIndex, bool SendMagicReplace);
	void GCSkillAddRecv(PMSG_SKILLMASTERADD* lpMsg, int aIndex);
	void AddSkillUpPoints(int aIndex);
	BOOL MagicExist(int aIndex, WORD Skill, int & SkillPosition);
	int MagicAdd(int aIndex, WORD aSkill, BYTE Level);
	MASTERSKILLS2_DATA Skill[MAXSKILLTREECOUNT];
	void Passive(int aIndex);
	void AddSinglePassive(int aIndex, int Pos, int Exist);
	void ApplyPassiveSkills(int aIndex);
	bool MagicReplace(int aIndex, WORD OldSkill, WORD NewSkill);
	void GCMagicListMultiSendChanges(int aIndex,int Pos, WORD & Skill);
	void BaseSkill(int & Skill);

	void GetValuebyLevel(DWORD Skill,BYTE Level, float & Current, float & Next);
	float GetValuebyLevel(int Skill, int Level);
	bool CheckAddSkill(int PlayerID, DWORD Skill, int & SkillPosition);
	bool CheckRanking(int aIndex, BYTE Ranking);
	int GetSkillPosition(int Type);
	int SkillCount;
};

extern cMastering2 Mastering2;