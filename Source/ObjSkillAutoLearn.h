
#if (PACK_EDITION>=2)

struct AUTOLEARN_SKILL_INFO
{
	WORD number;
	WORD requireLevel;
	WORD requireReset;
	BYTE RequireClass[MAX_TYPE_PLAYER];
};


class cObjSkillAutoLearn
{
public:
	void Load(char * FileName);
	void LevelUp(int aIndex);
	BYTE Enabled;
private:
	AUTOLEARN_SKILL_INFO skill[MAX_SKILL-1];
	int AddSkill(int aIndex, int aSkill);
	int Count;
};

extern cObjSkillAutoLearn ObjSkillAutoLearn;

#endif