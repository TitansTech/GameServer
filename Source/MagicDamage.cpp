#include "stdafx.h"
#include "MagicDamage.h"
#include "..\include\readscript.h"
#include "..\common\WzMemScript.h"
#include "LogProc.h"
#include "GameMain.h"

// GS-N 0.99.60T 0x0046B890 - Completed
//	GS-N	1.00.18	JPN	0x00482100	-	Completed

CMagicDamage MagicDamageC;

CMagicDamage::CMagicDamage()
{
	return;
}


CMagicDamage::~CMagicDamage()
{
	return;
}

void CMagicDamage::Init()
{
	for ( int n=0; n< MAX_SKILL;n++)
	{
		this->m_Damage[n] = -1;
	}
}

extern void WriteTxt(LPSTR FileName,LPSTR String);
void CMagicDamage::LogSkillList(char * filename)
{
	this->Init();


	SMDFile = fopen(filename, "r");

	if ( SMDFile == NULL )
	{
		MsgBox(lMsg.Get(MSGGET(1, 199)), filename);
		return;
	}

	int loop_count = 0;
	int Token;
	int number;
	char name[50];
	int requireLevel;
	int damage;
	int mana;
	int bp=0;
	int dis;
	int rEnergy;
	int iSkillType;
	int rLeadership;
	int rDelay;
	int rResistance;
	int iSkillUseType;
	int iSkillBrand;
	int iKillCount;
	int RequireStatus[MAX_REQ_SKILL_STATUS] = {0};
	BYTE RequireClass[MAX_TYPE_PLAYER];

	while ( true )
	{
		Token = GetToken();

		if ( Token == 2 )
		{
			break;
		}

		if ( Token == 1 )
		{
			number = TokenNumber;

			Token = GetToken();
			strcpy(name, TokenString);

			if(!strcmp(TokenString,""))
			{
				MsgBox("ERROR: FILE %s (INDEX:%d)", filename,loop_count);
				exit(1);
			}

			Token = GetToken();
			requireLevel = TokenNumber;

			Token = GetToken();
			damage = TokenNumber;

			Token = GetToken();
			mana = TokenNumber;

			Token = GetToken();
			bp = TokenNumber;	// Agility Gauge

			Token = GetToken();
			dis = TokenNumber;

			Token = GetToken();
			rDelay = TokenNumber;

			Token = GetToken();
			rEnergy = TokenNumber;

			Token = GetToken();
			rLeadership = TokenNumber;

			Token = GetToken();
			rResistance = TokenNumber;

			Token = GetToken();
			iSkillType = TokenNumber;

			Token = GetToken();
			iSkillUseType = TokenNumber;

			Token = GetToken();
			iSkillBrand = TokenNumber;

			Token = GetToken();
			iKillCount = TokenNumber;

			Token = GetToken();
			RequireStatus[0] = TokenNumber;

			Token = GetToken();
			RequireStatus[1] = TokenNumber;

			Token = GetToken();
			RequireStatus[2] = TokenNumber;

			Token = GetToken();
			RequireClass[CLASS_WIZARD] = TokenNumber;

			Token = GetToken();
			RequireClass[CLASS_KNIGHT] = TokenNumber;

			Token = GetToken();
			RequireClass[CLASS_ELF] = TokenNumber;

			Token = GetToken();
			RequireClass[CLASS_MAGICGLADIATOR] = TokenNumber;

			Token = GetToken();
			RequireClass[CLASS_DARKLORD] = TokenNumber;

			Token = GetToken();
			RequireClass[CLASS_SUMMONER] = TokenNumber;

			if(ReadConfig.S6E1 == 1)
			{
				Token = GetToken();
				RequireClass[CLASS_RAGEFIGHTER] = TokenNumber;
			}

			//if(number < SKILL_SKILLTREE_START)
			//{
			//	for(int i=0;i<7;i++)
			//	{
			//		if(RequireClass[i] > 0)
			//		{
			//			BYTE Req[7]={0};
			//			Req[i] = RequireClass[i];
			//			char sBuf[1024]={0};
			//			wsprintf(sBuf,"%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t//%s",number,requireLevel,0,Req[0],Req[1],Req[2],Req[3],Req[4],Req[5],Req[6],name);
			//			WriteTxt("SCF_SkillsAutoLearn.txt",sBuf);
			//		}
			//	}
			//}

			#if (WL_PROTECT==1)
			int MyCheckVar;   
			VM_START_WITHLEVEL(20)
				CHECK_PROTECTION(MyCheckVar, 0x75456783)  	 
				if (MyCheckVar != 0x75456783)
				{				
					for(int i=0;i<6;i++)
					RequireClass[i]=0;
				}
				VM_END
			#endif
			
			if(number > 434)
			{
				iSkillUseType = 0;
			}

			this->Set(name, number,damage, requireLevel, mana, bp, dis, rResistance, iSkillType, RequireClass, rEnergy, rLeadership, rDelay);
			this->SetEx(number, iSkillUseType, iSkillBrand, iKillCount, RequireStatus);
			DefMagicInf[number].Set(number, 0);
			loop_count++;
		}
	}

	fclose(SMDFile);
	LogAdd(lMsg.Get(MSGGET(1, 200)), filename);
}


void CMagicDamage::LogSkillNameList(char* filename)
{
	int Token;
	int n;
	int number;
	char name[50];
	
	SMDFile = fopen(filename, "r");

	if ( SMDFile == NULL )
	{
		MsgBox(lMsg.Get(MSGGET(1, 199)), filename);
		return;
	}

	n=0;

	while ( true )
	{
		Token = GetToken();

		if ( Token == 2 )
		{
			break;
		}

		if ( Token == 1 )
		{
			number = TokenNumber;

			Token = GetToken();
			strcpy(name, TokenString);

			strcpy(this->m_Name[number], name);
		}
	}

	fclose(SMDFile);
	LogAdd(lMsg.Get(MSGGET(1, 200)), filename);
}

void CMagicDamage::LogSkillList(char* Buffer, int iSize)
{
	this->Init();
	CWzMemScript WzMemScript;

	WzMemScript.SetBuffer(Buffer, iSize);

	int Token;
	int number;
	char name[50];
	int requireLevel;
	int damage;
	int mana;
	int bp=0;
	int dis;
	int rEnergy;
	int iSkillType;
	int rLeadership;
	int rDelay;
	int rResistance;
	int iSkillUseType;
	int iSkillBrand;
	int iKillCount;
	int RequireStatus[MAX_REQ_SKILL_STATUS] = {0};
	BYTE RequireClass[MAX_TYPE_PLAYER];

	while ( true )
	{
		Token = WzMemScript.GetToken();

		if ( Token == 2 )
		{
			break;
		}

		if ( Token == 1 )
		{
			number = WzMemScript.GetNumber();

			Token = WzMemScript.GetToken();
			strcpy(name, WzMemScript.GetString() );

			Token = WzMemScript.GetToken();
			requireLevel = WzMemScript.GetNumber();

			Token = WzMemScript.GetToken();
			damage = WzMemScript.GetNumber();

			Token = WzMemScript.GetToken();
			mana = WzMemScript.GetNumber();

			Token = WzMemScript.GetToken();
			bp = WzMemScript.GetNumber();	// Agility Gauge

			Token = WzMemScript.GetToken();
			dis = WzMemScript.GetNumber();

			Token = WzMemScript.GetToken();
			rDelay = WzMemScript.GetNumber();

			Token = WzMemScript.GetToken();
			rEnergy = WzMemScript.GetNumber();

			Token = WzMemScript.GetToken();
			rLeadership = WzMemScript.GetNumber();

			Token = WzMemScript.GetToken();
			rResistance = WzMemScript.GetNumber();

			Token = WzMemScript.GetToken();
			iSkillType = WzMemScript.GetNumber();

			Token = WzMemScript.GetToken();
			iSkillUseType = WzMemScript.GetNumber();

			Token = WzMemScript.GetToken();
			iSkillBrand = WzMemScript.GetNumber();

			Token = WzMemScript.GetToken();
			iKillCount = WzMemScript.GetNumber();

			Token = WzMemScript.GetToken();
			RequireStatus[0] = WzMemScript.GetNumber();

			Token = WzMemScript.GetToken();
			RequireStatus[1] = WzMemScript.GetNumber();

			Token = WzMemScript.GetToken();
			RequireStatus[2] = WzMemScript.GetNumber();

			Token = WzMemScript.GetToken();
			RequireClass[CLASS_WIZARD] = WzMemScript.GetNumber();

			Token = WzMemScript.GetToken();
			RequireClass[CLASS_KNIGHT] = WzMemScript.GetNumber();

			Token = WzMemScript.GetToken();
			RequireClass[CLASS_ELF] = WzMemScript.GetNumber();

			Token = WzMemScript.GetToken();
			RequireClass[CLASS_MAGICGLADIATOR] = WzMemScript.GetNumber();

			Token = WzMemScript.GetToken();
			RequireClass[CLASS_DARKLORD] = WzMemScript.GetNumber();

			Token = WzMemScript.GetToken();
			RequireClass[CLASS_SUMMONER] = WzMemScript.GetNumber();
			
			if(ReadConfig.S6E1 == 1)
			{
				Token = WzMemScript.GetToken();
				RequireClass[CLASS_RAGEFIGHTER] = WzMemScript.GetNumber();
			}


			this->Set(name, number,damage, requireLevel, mana, bp, dis, rResistance, iSkillType, RequireClass, rEnergy, rLeadership, rDelay);
			this->SetEx(number, iSkillUseType, iSkillBrand, iKillCount, RequireStatus);
			DefMagicInf[number].Set(number, 0);
		}
	}

	LogAdd(lMsg.Get(MSGGET(1, 200)), "Skill");
}





void CMagicDamage::LogSkillNameList(char* Buffer, int iSize)
{
	CWzMemScript WzMemScript;
	int Token;
	int n;
	int number;
	char name[50];
	
	WzMemScript.SetBuffer(Buffer, iSize);

	n=0;

	while ( true )
	{
		Token = WzMemScript.GetToken();

		if ( Token == 2 )
		{
			break;
		}

		if ( Token == 1 )
		{
			number = TokenNumber;	// #error Aply Deathway Fix

			Token = WzMemScript.GetToken();	
			strcpy(name, TokenString);	// #error Deathway Fix

			strcpy(this->m_Name[number], name);
		}
	}

	
	LogAdd(lMsg.Get(MSGGET(1, 200)), "Skill_Local");
}



void CMagicDamage::SetEx(int iSkill, int iSkillUseType, int iSkillBrand, int iKillCount, int * pReqStatus)
{
	if ( iSkill <0 || iSkill > MAX_SKILL -1 )
	{
		MsgBox(lMsg.Get(MSGGET(1, 201)), __FILE__, __LINE__);
		return;
	}

	if ( this->m_Damage[iSkill] == -1 )
	{
		MsgBox(lMsg.Get(MSGGET(2, 50)), __FILE__, __LINE__);
		return;
	}

	this->m_iSkillUseType[iSkill] = iSkillUseType;
	//this->m_iSkillBrand[iSkill] = iSkillBrand;
	this->m_iKillCount[iSkill] = iKillCount;
	memcpy(this->m_iRequireStatus[iSkill], pReqStatus, sizeof(this->m_iRequireStatus[0]));	// #error Deathway Fix
}

void CMagicDamage::Set(char* name, int skill, int damage,  int rlevel,  int mana,  int bp,  int dis, int rResistance,  int iSkillType, unsigned char* ReqClass, int rEnergy, int rLeadership, int rdelay)
{
	if ( skill <0 || skill > MAX_SKILL -1 )
	{
		MsgBox(lMsg.Get(MSGGET(1, 201)), __FILE__, __LINE__);
		return;
	}

	if ( this->m_Damage[skill] != -1 )
	{
		MsgBox(lMsg.Get(MSGGET(2, 50)), __FILE__, __LINE__);
		return;
	}

	strcpy(this->m_Name[skill], name);
	this->m_Damage[skill] = damage;
	//this->m_rLevel[skill] = rlevel;
	this->m_Mana[skill] = mana;
	this->m_Distance[skill] = dis;
	this->m_RequireLeadership[skill] = rLeadership;
	//this->m_Delay[skill] = rdelay;
	this->m_BrainPower[skill] = bp;
	this->m_Resistance[skill] = rResistance;
	this->m_iSkillType[skill] = iSkillType;
	memcpy(this->m_RequireClass[skill], ReqClass, sizeof(this->m_RequireClass[0]));

	this->m_RequireEnergy[skill] = (rEnergy * rlevel * 4) / 100 + 20;
	//this->m_RequireEnergy[skill] = (rEnergy * rlevel * 4) / 100 + 20;
	//this->m_RequireEnergy[skill] = rEnergy;

	//if ( skill == 13 ) // Blast
	//{
	//	LogAddL("skill:%d", this->m_RequireEnergy[skill]);
	//}

	// Set Level Requirements for Skills
	this->m_RequireLevel[skill] = rlevel;

	//======================================================================
	//Skill Level Requirement FIX
	//======================================================================
	if (skill > 299)												//Skill Tree (Season6E1 and down)
	{
		this->m_RequireLevel[skill] = 0;
	}

	if ( skill == 41 )												//Twisting Slash
	{
		this->m_RequireLevel[skill] = 80;
	}
	else if ( skill == 42 )											//Rageful Blow
	{
		this->m_RequireLevel[skill] = rlevel;	//170;
	}
	else if ( skill == 43 )											//Death Stab
	{
		this->m_RequireLevel[skill] = rlevel;	//160;
	}
	else if ( skill == 47 )											//Impale
	{
		this->m_RequireLevel[skill] = rlevel;	//28;
	}
	else if ( skill == 48 )											//Inner Strength
	{
		this->m_RequireLevel[skill] = rlevel;	//120;
	}
	else if ( skill == 52 )											//Penetration
	{
		this->m_RequireLevel[skill] = rlevel;	//130;
	}

	//======================================================================
	//Skill Energy Requirement FIX
	//======================================================================
	if ( skill == 30 )												//Summon Goblin
	{
		this->m_RequireEnergy[skill] = rEnergy;	//30;
	}
	else if ( skill == 31 )											//Summon Stone Golem
	{
		this->m_RequireEnergy[skill] = rEnergy;	//60;
	}
	else if ( skill == 32 )											//Summon Assassin
	{
		this->m_RequireEnergy[skill] = rEnergy;	//90;
	}
	else if ( skill == 33 )											//Summon Elite Yeti
	{
		this->m_RequireEnergy[skill] = rEnergy;	//130;
	}
	else if ( skill == 34 )											//Summon Dark Knight
	{
		this->m_RequireEnergy[skill] = rEnergy;	//170;
	}
	else if ( skill == 35 )											//Summon Bali
	{
		this->m_RequireEnergy[skill] = rEnergy;	//210;
	}
	else if ( skill == 36 )											//Summon Soldier
	{
		this->m_RequireEnergy[skill] = rEnergy;	//300;
	}
	else if ( skill == 41 )											//Twisting Slash
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 42 )											//Rageful Blow
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 43 )											//Death Stab
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 47 )											//Impale
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 48 )											//Greater Fortitude
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 49  )										//Fire Breath
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 55  )										//Fire Slash
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 51  )										//Ice Arrow
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 52  )										//Penetration
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 24  || (skill >= 490 && skill <= 494) )		//Triple Shot
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 235  )										//Five Shot
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 17 )											//Energy Ball
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 18 )											//Defense
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 19 )											//Falling Slash
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 20  )										//Lunge
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 21  )										//Uppercut
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 22  )										//Cyclone
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 23  )										//Slash
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 25  )										//
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 56  )										//Power Slash
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 60  )										//Force
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 44  )										//Crescent Moon Slash
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 45  )										//Mana Glaive
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 46  )										//Starfall
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 57  )										//Spiral Slash
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 73  )										//Mana Rays
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 74  )										//Fire Blast
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 75  )										//
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 67  )										//Stern
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 68  )										//Cancel Stern
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 69  )										//Swell Mana
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 70  )										//Transparency
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 71  )										//Cancel Transparency
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 72  )										//Cancel Magic
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 76  )										//Plasma Storm
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 77  )										//Shadow Arrow
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if (( skill == 78  ) || (skill >= 525 && skill <= 529))	//Fire Scream
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 79  )										//Explosion
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if (( skill == 62  ) || ( skill >= 515 && skill <= 519))	//Earthshake
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 232 )										//Explotion
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 234 )										//Reduce Dmg Shield
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 235 )										//Five Shot
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 236 )										//Sword Slash
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 237 )										//Lighting Storm
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 238 )										//Bird
	{
		this->m_RequireEnergy[skill] = 0;
	}
}




int CMagicDamage::Get(int skill)
{
	if ( skill < 0 || skill > MAX_SKILL-1 )
	{
		LogAdd(lMsg.Get(MSGGET(1, 201)), __FILE__, __LINE__);
		return this->m_Damage[0];
	}

	if ( this->m_Damage[skill] == -1 )
	{
		LogAdd(lMsg.Get(MSGGET(1, 202)), __FILE__, __LINE__);
		return this->m_Damage[0];
	}

	return this->m_Damage[skill];
}






int CMagicDamage::SkillGet(int skill)
{
	if ( skill < 0 || skill > MAX_SKILL-1 )
	{
		LogAdd(lMsg.Get(MSGGET(1, 201)), __FILE__, __LINE__);
		return this->m_Damage[0];
	}

	if ( this->m_Damage[skill] == -1 )
	{
		if(skill != 150)
			LogAdd(lMsg.Get(MSGGET(1, 202)), __FILE__, __LINE__);
		return this->m_Damage[0];
	}

	return this->m_Damage[skill];
}







int CMagicDamage::SkillGetMana(int skill)
{
	if ( skill < 0 || skill > MAX_SKILL-1 )
	{
		LogAdd(lMsg.Get(MSGGET(1, 201)), __FILE__, __LINE__);
		return this->m_Mana[0];
	}

	if ( this->m_Damage[skill] == -1 )
	{
		LogAdd(lMsg.Get(MSGGET(1, 202)), __FILE__, __LINE__);
		return this->m_Damage[0];
	}

	return this->m_Mana[skill];
}






int CMagicDamage::SkillGetBP(int skill)
{
	if ( skill < 0 || skill > MAX_SKILL-1 )
	{
		LogAdd(lMsg.Get(MSGGET(1, 201)), __FILE__, __LINE__);
		return 0;
	}

	if ( this->m_Damage[skill] == -1 )
	{
		LogAdd(lMsg.Get(MSGGET(1, 202)), __FILE__, __LINE__);
		return 0;
	}

	return this->m_BrainPower[skill];
}








int CMagicDamage::SkillGetRequireEnergy(int skill)
{
	if ( skill < 0 || skill > MAX_SKILL-1 )
	{
		LogAdd(lMsg.Get(MSGGET(1, 201)), __FILE__, __LINE__);
		return -1;
	}

	if ( this->m_RequireEnergy[skill] == -1 )
	{
		LogAdd(lMsg.Get(MSGGET(1, 202)), __FILE__, __LINE__);
		return -1;
	}

	return this->m_RequireEnergy[skill];
}







BOOL CMagicDamage::SkillGetRequireClass(int Cclass, int ChangeUP,  int skill)
{
	int addChangeUp = 0;

	if ( skill < 0 || skill > MAX_SKILL-1 )
	{
		LogAdd(lMsg.Get(MSGGET(1, 201)), __FILE__, __LINE__);
		return -1;
	}

	if ( Cclass < 0 || Cclass > MAX_TYPE_PLAYER-1 )
	{
		LogAdd(lMsg.Get(MSGGET(1, 202)), __FILE__, __LINE__);
		return -1;
	}

	int requireclass = this->m_RequireClass[skill][Cclass];
	if ( requireclass == 0 )
	{
		return FALSE;
	}

	if (ChangeUP > 0)
	{
		if ((Cclass == CLASS_DARKLORD) || (Cclass == CLASS_MAGICGLADIATOR) || (Cclass == CLASS_RAGEFIGHTER))
			addChangeUp = 2;
		else
			addChangeUp = 1;
	}

	if (requireclass > 1)
	{
		if ( requireclass > (ChangeUP+addChangeUp) )
		{
			return FALSE;
		}
	}

	return TRUE;
}






int CMagicDamage::GetSkillResistance(int skill)
{
	if ( skill < 0 || skill > MAX_SKILL-1 )
	{
		return -1;
	}

	return this->m_Resistance[skill];
}






int CMagicDamage::CheckSkillAttr(int skill, int attr)
{
	if ( skill < 0 || skill > MAX_SKILL-1 )
	{
		return FALSE;
	}

	return this->m_Resistance[skill];
}





int CMagicDamage::GetskillRequireLeadership(int skill)
{
	if ( skill < 0 || skill > MAX_SKILL-1 )
	{
		LogAdd(lMsg.Get(MSGGET(1, 201)), __FILE__, __LINE__);
		return -1;
	}

	if ( this->m_RequireLeadership[skill] == -1 )
	{
		LogAdd(lMsg.Get(MSGGET(1, 202)), __FILE__, __LINE__);
		return -1;
	}

	return this->m_RequireLeadership[skill];
}



/*int CMagicDamage::GetDelayTime(int skill)
{
	if ( skill < 0 || skill > MAX_SKILL-1 )
	{
		LogAdd(lMsg.Get(MSGGET(1, 201)), __FILE__, __LINE__);
		return 0;
	}

	return this->m_Delay[skill];
}*/






int CMagicDamage::GetSkillType(int iSkill)
{
	if ( iSkill < 0 || iSkill > MAX_SKILL-1 )
	{
		LogAdd(lMsg.Get(MSGGET(1, 201)), __FILE__, __LINE__);
		return 0;
	}
	
	return this->m_iSkillType[iSkill];
}





int CMagicDamage::SkillGetRequireLevel(int skill)
{
	if ( skill < 0 || skill > MAX_SKILL-1 )
	{
		LogAdd(lMsg.Get(MSGGET(1, 201)), __FILE__, __LINE__);
		return -1;
	}

	return this->m_RequireLevel[skill];
}






BOOL CMagicDamage::CheckStatus(int iSkill, int iGuildStatus)
{
	if ( iSkill < 0 || iSkill > MAX_SKILL-1 )
	{
		LogAdd(lMsg.Get(MSGGET(1, 201)), __FILE__, __LINE__);
		return 0;
	}

	if (( this->m_iSkillUseType[iSkill] == 0 ) || ((iSkill >= SKILL_SKILLTREE_START && iSkill <= SKILL_SKILLTREE_END) && ( this->m_iSkillUseType[iSkill] == 4 )))
	{
		return TRUE;
	}

	if ( iGuildStatus == 0x80 )
	{
		if ( this->m_iRequireStatus[iSkill][0] == 1 )
		{
			return TRUE;
		}
	}
	else if ( iGuildStatus == 0x40 )
	{
		if ( this->m_iRequireStatus[iSkill][1] == 1 )
		{
			return TRUE;
		}
	}
	else if ( iGuildStatus == 0x20 )
	{
		if ( this->m_iRequireStatus[iSkill][2] == 1 )
		{
			return TRUE;
		}
	}

	return FALSE;
}






/*BOOL CMagicDamage::CheckBrandOfSkill(int iSkill)
{
	if ( iSkill < 0 || iSkill > MAX_SKILL-1 )
	{
		LogAdd(lMsg.Get(MSGGET(1, 201)), __FILE__, __LINE__);
		return 0;
	}

	if ( this->m_iSkillBrand[iSkill] > 0 )
	{
		return TRUE;
	}

	return FALSE;
}*/





int CMagicDamage::CheckKillCount(int iSkill, int iKillCount)
{
	if ( iSkill < 0 || iSkill > MAX_SKILL-1 )
	{
		LogAdd(lMsg.Get(MSGGET(1, 201)), __FILE__, __LINE__);
		return -1;
	}

	return iKillCount - this->m_iKillCount[iSkill];
}


int CMagicDamage::GetSkillDistance(int skill)
{
	if ( skill < 0 || skill > MAX_SKILL-1 )
	{
		LogAdd(lMsg.Get(MSGGET(1, 201)), __FILE__, __LINE__);
		return -1;
	}

	return this->m_Distance[skill];
}

char * CMagicDamage::GetName(int skill)
{
	if ( skill < 0 || skill > MAX_SKILL-1 )
	{
		LogAdd(lMsg.Get(MSGGET(1, 201)), __FILE__, __LINE__);
		return "unk";
	}

	return this->m_Name[skill];
}