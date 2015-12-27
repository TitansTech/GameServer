#include "stdafx.h"
#include "MagicInf.h"
#include "LogProc.h"
#include "GameMain.h"
// GS-N 0.99.60T 0x0046DE00
//	GS-N	1.00.18	JPN	0x00484870	-	Completed

CMagicInf DefMagicInf[MAX_SKILL-1];




CMagicInf::CMagicInf()
{
	this->Clear();
}




CMagicInf::~CMagicInf()
{
	this->m_DamageMin  = 0;
	this->m_DamageMax  = 0;
	this->m_MasterSkill = 0;
}




void CMagicInf::Clear()
{
	this->m_Skill  = -1;
	this->m_Level  = 0;
	this->m_MasterSkill = 0;
}




BOOL CMagicInf::IsMagic()
{
	try
	{
		if ( this->m_Skill == 0xFFFF )
		{
			return false;
		}
		return true;
	}catch(...)
	{
		LogAdd("error : %s %d", __FILE__, __LINE__ );
		return false;
	}
}




int GetSkillNumberInex(int type, int Index, int level)
{
	int skillnumber = -1;

	if ( type == 15 )
	{
		if ( Index >= 0 && Index < 16 )
		{
			skillnumber = Index + 1;
		}
		else if ( Index == 16 )
		{
			skillnumber = 38;
		}
		else if ( Index == 17 )
		{
			skillnumber = 39;
		}
		else if ( Index == 18 )
		{
			skillnumber = 40;
		}
		else if ( (( Index >= 19 ) && ( Index <= 22 )) )
		{
			if(ReadConfig.S6E2 == 0)
				skillnumber = Index + 195;
			else
			{
				if(Index == 19)
					skillnumber = 215;
				else if(Index == 20)
					skillnumber = 214;
				else if(Index == 21)
					skillnumber = 230;
				else if(Index == 22)
					skillnumber = 217;
			}
		}
		else if ( Index == 23 )
		{
			skillnumber = 218;
		}
		else if ( Index == 24 )
		{
			skillnumber = 219;
		}
		else if(( Index >= 25 ) && ( Index <= 26 ))
		{
			if(ReadConfig.S6E2 == 0)
				skillnumber = Index + 196;
			else
			{
				if( Index == 26 )
					skillnumber = 221;
				else
					skillnumber = 216;
			}
		}
		else if ( Index == 27 )
		{
			if(ReadConfig.S6E2 == 0)
				skillnumber = 230;
			else
				skillnumber = 222;
		}
		else if ( Index == 28 )
		{
			skillnumber = 233;
		}
		else if ( Index == 29 )
		{
			skillnumber = 237;
		}
		else if ( Index == 30 )
		{
			skillnumber = 262;
		}
		else if ( Index == 31 )
		{
			skillnumber = 263;
		}
		else if ( Index == 32 )
		{
			skillnumber = 264;
		}
		else if ( Index == 33 )
		{
			skillnumber = 265;
		}
		else if ( Index == 34 )
		{
			skillnumber = 266;
		}
		else if ( Index == 35 )
		{
			skillnumber = 267;
		}
		else if ( Index == 36 )
		{
			skillnumber = 268;
		}
	}

	else if ( type == 12 )
	{
		switch ( Index )
		{
			case 7:		skillnumber = 41;	break;
			case 8:		skillnumber = 26;	break;
			case 9:		skillnumber = 27;	break;
			case 10:	skillnumber = 28;	break;
			case 12:	skillnumber = 42;	break;
			case 13:	skillnumber = 47;	break;
			case 14:	skillnumber = 48;	break;
			case 19:	skillnumber = 43;	break;
			case 17:	skillnumber = 52;	break;
			case 18:	skillnumber = 51;	break;
			case 16:	skillnumber = 55;	break;
			case 21:	skillnumber = 61;	break;
			case 22:	skillnumber = 63;	break;
			case 23:	skillnumber = 64;	break;
			case 24:	skillnumber = 65;	break;
			case 11:	skillnumber = level + 30;	break;
			case 35:	skillnumber = 78;	break;
			case 44:	skillnumber = 232;	break;
			case 45:	skillnumber = 235;	break;
			case 46:	skillnumber = 234;	break;
			case 47:	skillnumber = 236;	break;
			case 48:	skillnumber = 238;	break;
		}
	}
		
	return skillnumber;
}





int CMagicInf::Set(BYTE aType, BYTE aIndex, BYTE aLevel)
{
	if ( this->m_Skill != 0xFFFF )
	{
		LogAdd(lMsg.Get(MSGGET(1, 203)), __FILE__, __LINE__);
		return -1;
	}

	this->m_Level = aLevel;
	this->m_Skill = GetSkillNumberInex(aType, aIndex, aLevel);
	
	if ( this->m_Skill < 0 )
	{
		this->m_Skill = 1;
	}

	int damage = MagicDamageC.SkillGet(this->m_Skill);
	this->m_DamageMin = damage;
	this->m_DamageMax = damage + damage/2;
	this->m_MasterSkill = 0;

	return this->m_Skill;
}





int CMagicInf::GetDamage()
{
	int damage = 0;

	if ( this->m_Skill == 0xFFFF )
	{
		LogAdd(lMsg.Get(MSGGET(1, 202)), __FILE__, __LINE__);
		return 0;
	}

	int subd = this->m_DamageMax - this->m_DamageMin;

	__try
	{
		damage = this->m_DamageMin + (rand() % 1);
	}
	__except(subd=1,1)
	{
		LogAdd(lMsg.Get(MSGGET(0, 111)), __FILE__, __LINE__);
	}

	return damage;
}






int CMagicInf::Set(WORD aSkill, BYTE aLevel)
{
	if ( this->m_Skill != 0xFFFF )
	{
		return -1;
	}

	this->m_Skill = aSkill;
	this->m_Level = aLevel;
	int damage = MagicDamageC.SkillGet(this->m_Skill);
	this->m_DamageMin = damage;
	this->m_DamageMax = damage + damage/2;
	this->m_MasterSkill = 0;

	return this->m_Skill;
}






void MagicByteConvert(unsigned char* buf, CMagicInf* const Magici, int maxmagic)
{
	int n=0;

	for (int index = 0;index < maxmagic;index++)
	{
		buf[n] = LOBYTE(Magici[index].m_Skill);
		n++;

		buf[n] = HIBYTE(Magici[index].m_Skill);
		n++;

		buf[n] = Magici[index].m_Level << 3;
		n++;
	}
}
