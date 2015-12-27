#include "stdafx.h"
#include "MonsterAttr.h"
#include "LogProc.h"
#include "..\include\readscript.h"
#include "..\common\WzMemScript.h"
#include "user.h"
#include "GameMain.h"

// GS-N 0.99.60T 0x00418C10 - Completed - Just few disorders with same result int LoadAttr
//	GS-N	1.00.18	JPN	0x0041F300	-	Completed

CMonsterAttr::CMonsterAttr()
{
	return;
}

CMonsterAttr::~CMonsterAttr()
{
	return;
}


LPMONSTER_ATTRIBUTE CMonsterAttr::GetAttr(int aClass)
{
	for ( int n = 0; n<MAX_MONSTER_TYPE;n++)
	{
		if ( this->m_MonsterAttr[n].m_Index == aClass )
		{
			return &this->m_MonsterAttr[n];
		}
	}

	return NULL;
}

void CMonsterAttr::LoadAttr(char* filename)
{
	int Token;
	int n;

	SMDFile = fopen(filename, "r");

	if ( SMDFile == NULL )
	{
		MsgBox("load error %s", filename);
	}
	else
	{
		n= 0;

		while ( true )
		{
			Token = GetToken();

			if ( Token == 2 )
			{
				break;
			}
			else
			{
				if ( Token == 1 )
				{
					this->m_MonsterAttr[n].m_Index = TokenNumber;

					Token = GetToken();
					this->m_MonsterAttr[n].m_Rate = TokenNumber;

					Token = GetToken();
					strcpy(this->m_MonsterAttr[n].m_Name, TokenString);

					Token = GetToken();
					this->m_MonsterAttr[n].m_Level = TokenNumber;

					Token = GetToken();
					this->m_MonsterAttr[n].m_Hp = TokenNumber;

					Token = GetToken();
					this->m_MonsterAttr[n].m_Mp = TokenNumber;
					
					Token = GetToken();
					this->m_MonsterAttr[n].m_DamageMin = TokenNumber;
					
					Token = GetToken();
					this->m_MonsterAttr[n].m_DamageMax = TokenNumber;
					
					Token = GetToken();
					this->m_MonsterAttr[n].m_Defense = TokenNumber;
					
					Token = GetToken();
					this->m_MonsterAttr[n].m_MagicDefense = TokenNumber;
					
					Token = GetToken();
					this->m_MonsterAttr[n].m_AttackRating = TokenNumber;
					
					Token = GetToken();
					this->m_MonsterAttr[n].m_Successfulblocking = TokenNumber;
					
					Token = GetToken();
					this->m_MonsterAttr[n].m_MoveRange = TokenNumber;
					
					Token = GetToken();
					this->m_MonsterAttr[n].m_AttackType = TokenNumber;
					
					Token = GetToken();
					this->m_MonsterAttr[n].m_AttackRange = TokenNumber;
					
					Token = GetToken();
					this->m_MonsterAttr[n].m_ViewRange = TokenNumber;
					
					Token = GetToken();
					this->m_MonsterAttr[n].m_MoveSpeed = TokenNumber;
					
					Token = GetToken();
					this->m_MonsterAttr[n].m_AttackSpeed = TokenNumber;
					
					Token = GetToken();
					this->m_MonsterAttr[n].m_RegenTime = TokenNumber;
					
					Token = GetToken();
					this->m_MonsterAttr[n].m_Attribute = TokenNumber;
					
					Token = GetToken();
					this->m_MonsterAttr[n].m_ItemRate = TokenNumber;
					
					Token = GetToken();
					this->m_MonsterAttr[n].m_MoneyRate = TokenNumber;
					
					Token = GetToken();
					this->m_MonsterAttr[n].m_MaxItemLevel = TokenNumber;
					
					Token = GetToken();
					this->m_MonsterAttr[n].m_MonsterSkill = TokenNumber;

					Token = GetToken();
					this->m_MonsterAttr[n].m_Resistance[R_ICE]  = TokenNumber;
					
					Token = GetToken();
					this->m_MonsterAttr[n].m_Resistance[R_POISON] = TokenNumber;
					
					Token = GetToken();
					this->m_MonsterAttr[n].m_Resistance[R_LIGHTNING] = TokenNumber;
					
					Token = GetToken();
					this->m_MonsterAttr[n].m_Resistance[R_FIRE] = TokenNumber;

					this->m_MonsterAttr[n].m_Hp = (INT)(this->m_MonsterAttr[n].m_Hp * gMonsterHp/100.0f);

					this->m_MonsterAttr[n].m_iScriptHP = this->m_MonsterAttr[n].m_Hp;

					n++;

					if ( n> MAX_MONSTER_TYPE-1 )
					{
						MsgBox("Monster attribute max over %s %d", __FILE__, __LINE__);
						//fclose(SMDFile);
						return;
					}
				}
			}
		}
	}

	fclose(SMDFile);
}


void CMonsterAttr::LoadAttr(char* Buffer, int iSize)
{
	CWzMemScript WzMemScript;
	
	WzMemScript.SetBuffer(Buffer, iSize);

	int Token;
	int n=0;

	while ( true )
	{
		Token = WzMemScript.GetToken();

		if ( Token == 2 )
		{
			break;
		}
		else
		{
			if ( Token == 1 )
			{
				this->m_MonsterAttr[n].m_Index = WzMemScript.GetNumber();

				Token = WzMemScript.GetToken();
				this->m_MonsterAttr[n].m_Rate = WzMemScript.GetNumber();

				Token = WzMemScript.GetToken();
				strcpy(this->m_MonsterAttr[n].m_Name, WzMemScript.GetString());

				Token = WzMemScript.GetToken();
				this->m_MonsterAttr[n].m_Level = WzMemScript.GetNumber();

				Token = WzMemScript.GetToken();
				this->m_MonsterAttr[n].m_Hp = WzMemScript.GetNumber();

				Token = WzMemScript.GetToken();
				this->m_MonsterAttr[n].m_Mp = WzMemScript.GetNumber();
					
				Token = WzMemScript.GetToken();
				this->m_MonsterAttr[n].m_DamageMin = WzMemScript.GetNumber();
					
				Token = WzMemScript.GetToken();
				this->m_MonsterAttr[n].m_DamageMax = WzMemScript.GetNumber();
					
				Token = WzMemScript.GetToken();
				this->m_MonsterAttr[n].m_Defense = WzMemScript.GetNumber();
					
				Token = WzMemScript.GetToken();
				this->m_MonsterAttr[n].m_MagicDefense = WzMemScript.GetNumber();
				
				Token = WzMemScript.GetToken();
				this->m_MonsterAttr[n].m_AttackRating = WzMemScript.GetNumber();
					
				Token = WzMemScript.GetToken();
				this->m_MonsterAttr[n].m_Successfulblocking = WzMemScript.GetNumber();
					
				Token = WzMemScript.GetToken();
				this->m_MonsterAttr[n].m_MoveRange = WzMemScript.GetNumber();
					
				Token = WzMemScript.GetToken();
				this->m_MonsterAttr[n].m_AttackType = WzMemScript.GetNumber();
					
				Token = WzMemScript.GetToken();
				this->m_MonsterAttr[n].m_AttackRange = WzMemScript.GetNumber();
					
				Token = WzMemScript.GetToken();
				this->m_MonsterAttr[n].m_ViewRange = WzMemScript.GetNumber();
					
				Token = WzMemScript.GetToken();
				this->m_MonsterAttr[n].m_MoveSpeed = WzMemScript.GetNumber();
					
				Token = WzMemScript.GetToken();
				this->m_MonsterAttr[n].m_AttackSpeed = WzMemScript.GetNumber();
					
				Token = WzMemScript.GetToken();
				this->m_MonsterAttr[n].m_RegenTime = WzMemScript.GetNumber();
					
				Token = WzMemScript.GetToken();
				this->m_MonsterAttr[n].m_Attribute = WzMemScript.GetNumber();
					
				Token = WzMemScript.GetToken();
				this->m_MonsterAttr[n].m_ItemRate = WzMemScript.GetNumber();
					
				Token = WzMemScript.GetToken();
				this->m_MonsterAttr[n].m_MoneyRate = WzMemScript.GetNumber();
					
				Token = WzMemScript.GetToken();
				this->m_MonsterAttr[n].m_MaxItemLevel = WzMemScript.GetNumber();
					
				Token = WzMemScript.GetToken();
				this->m_MonsterAttr[n].m_MonsterSkill = WzMemScript.GetNumber();

				Token = WzMemScript.GetToken();
				this->m_MonsterAttr[n].m_Resistance[R_ICE]  = WzMemScript.GetNumber();
					
				Token = WzMemScript.GetToken();
				this->m_MonsterAttr[n].m_Resistance[R_POISON] = WzMemScript.GetNumber();
					
				Token = WzMemScript.GetToken();
				this->m_MonsterAttr[n].m_Resistance[R_LIGHTNING] = WzMemScript.GetNumber();
					
				Token = WzMemScript.GetToken();
				this->m_MonsterAttr[n].m_Resistance[R_FIRE] = WzMemScript.GetNumber();

				this->m_MonsterAttr[n].m_Hp = (INT)(this->m_MonsterAttr[n].m_Hp * gMonsterHp/ 100.0f );

				this->m_MonsterAttr[n].m_iScriptHP = this->m_MonsterAttr[n].m_Hp;

				n++;

				if ( n> MAX_MONSTER_TYPE-1 )
				{
					MsgBox("Monster attribute max over %s %d", __FILE__, __LINE__);
					fclose(SMDFile);	// #error - Remove this fclose

					return;
				}
			}
		}
	}
}


