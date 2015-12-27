#include "stdafx.h"
#if (CRYSTAL_EDITION == 1)
#include "GameServer.h"
#include "LogProc.h"
#include "GameMain.h"
#include "CustomWings.h"
#include "..\include\readscript.h"

cCustomWings CWings;

void cCustomWings::Read(char * FilePath)
{
	int Token;

	SMDFile = fopen(FilePath, "r");

	if ( SMDFile == NULL )
	{
		MsgBox("Custom Wings data load error %s", FilePath);
		return;
	}

	for(int i=0;i<55;i++)
	{
		this->Options[i].AbsorbDmg.Loaded = false;
		this->Options[i].IncreaseDmg.Loaded = false;
		this->Options[i].DefenseLevel.Loaded = false;
		this->Options[i].ExcAttrib.Loaded = false;
	}

	while ( true )
	{
		int iType = GetToken();
		
		if ( iType == 1 )
		{
			while(true)
			{
				Token = GetToken();
				if ( strcmp("end", TokenString) == 0 )
				{
					break;
				}

				int Type = TokenNumber;
				if(Type < 0 || Type > 54)
				{
					LogAddTD("[CWing][Load] WingID Overflowed %d in absorb dmg!!",Type);
					break;
				}

				Token = GetToken();
				this->Options[Type].AbsorbDmg.Opt1 = TokenNumber;

				Token = GetToken();
				this->Options[Type].AbsorbDmg.Opt2 = TokenNumber;

				Token = GetToken();
				this->Options[Type].AbsorbDmg.Opt3 = TokenNumber;

				this->Options[Type].AbsorbDmg.Loaded = true;
			}
		}
		
		iType = GetToken();
		iType = TokenNumber;
		
		
		if ( iType == 2 )
		{
			while(true)
			{
				Token = GetToken();
				if ( strcmp("end", TokenString) == 0 )
				{
					break;
				}

				int Type = TokenNumber;
				if(Type < 0 || Type > 54)
				{
					LogAddTD("[CWing][Load] WingID Overflowed %d in increase dmg!!",Type);
					break;
				}

				Token = GetToken();
				this->Options[Type].IncreaseDmg.Opt1 = TokenNumber;

				Token = GetToken();
				this->Options[Type].IncreaseDmg.Opt2 = TokenNumber;

				Token = GetToken();
				this->Options[Type].IncreaseDmg.Opt3 = TokenNumber;

				this->Options[Type].IncreaseDmg.Loaded = true;
			}
		}
		
		iType = GetToken();
		iType = TokenNumber;
		
		
		if ( iType == 3 )
		{
			while(true)
			{
				Token = GetToken();
				if ( strcmp("end", TokenString) == 0 )
				{
					break;
				}

				int Type = TokenNumber;
				if(Type < 0 || Type > 54)
				{
					LogAddTD("[CWing][Load] WingID Overflowed %d in defense Lvl!!",Type);
					break;
				}

				Token = GetToken();
				this->Options[Type].DefenseLevel.Opt1 = TokenNumber;

				this->Options[Type].DefenseLevel.Loaded = true;
			}
		}
		
		iType = GetToken();
		iType = TokenNumber;
		
		
		if ( iType == 4 )
		{
			while(true)
			{
				Token = GetToken();
				if ( strcmp("end", TokenString) == 0 )
				{
					break;
				}

				int Type = TokenNumber;
				if(Type < 0 || Type > 54)
				{
					LogAddTD("[CWing][Load] WingID Overflowed %d in exc attrib!!",Type);
					break;
				}

				Token = GetToken();
				this->Options[Type].ExcAttrib.Opt1 = TokenNumber;

				Token = GetToken();
				this->Options[Type].ExcAttrib.Opt2 = TokenNumber;

				Token = GetToken();
				this->Options[Type].ExcAttrib.Opt3 = TokenNumber;

				Token = GetToken();
				this->Options[Type].ExcAttrib.Opt4 = TokenNumber;

				this->Options[Type].ExcAttrib.Loaded = true;
			}
		}
		break;
	}
			
	LogAddTD("[Custom Wings] - %s file is Loaded",FilePath);
	fclose(SMDFile);
}

float cCustomWings::GetAbsorbDmg(int ItemID, BYTE Level, int AttackDamage)
{
	if (ItemID >= ITEMGET(12,200) && ItemID <= ITEMGET(12,254))
	{
		int Index = ItemID - ITEMGET(12,200);
		if(this->Options[Index].AbsorbDmg.Loaded == true)
		{
			float damage = (float)(AttackDamage * (this->Options[Index].AbsorbDmg.Opt1 - (Level * this->Options[Index].AbsorbDmg.Opt2))) / this->Options[Index].AbsorbDmg.Opt3;
			return damage;
		}
	}

	float damage = (float)(AttackDamage * (61 - (Level * 2))) / 100.0f;
	return damage;
}

int cCustomWings::GetIncreaseDmg(int ItemID, BYTE Level, int AttackDamage)
{
	if (ItemID >= ITEMGET(12,200) && ItemID <= ITEMGET(12,254))
	{
		int Index = ItemID - ITEMGET(12,200);
		if(this->Options[Index].IncreaseDmg.Loaded == true)
		{
			int damage = AttackDamage * (Level * this->Options[Index].IncreaseDmg.Opt1 + this->Options[Index].IncreaseDmg.Opt2) / this->Options[Index].IncreaseDmg.Opt3 ;
			return damage;
		}
	}

	int damage = AttackDamage * (Level * 2 + 139) / 100 ;
	return damage;
}

WORD cCustomWings::GetDefenseLevel(int ItemID, BYTE Level, int Defense)
{
	if (ItemID >= ITEMGET(12,200) && ItemID <= ITEMGET(12,254))
	{
		int Index = ItemID - ITEMGET(12,200);
		if(this->Options[Index].DefenseLevel.Loaded == true)
		{
			int def = Defense + Level * this->Options[Index].DefenseLevel.Opt1;
			return def;
		}
	}

	int def = Defense + Level * 5;
	return def;
}

BYTE cCustomWings::GetExcOption(int ItemID, BYTE Option)
{
	if (ItemID >= ITEMGET(12,200) && ItemID <= ITEMGET(12,254))
	{
		int Index = ItemID - ITEMGET(12,200);
		if(this->Options[Index].ExcAttrib.Loaded == true)
		{
			switch(Option)
			{
				case 1:
				{
					return this->Options[Index].ExcAttrib.Opt1;
				}break;
				case 2:
				{
					return this->Options[Index].ExcAttrib.Opt2;
				}break;
				case 3:
				{
					return this->Options[Index].ExcAttrib.Opt3;
				}break;
				case 4:
				{
					return this->Options[Index].ExcAttrib.Opt4;
				}break;
			}
		}
	}

	switch(Option)
	{
		case 1:
		{
			return g_iWings3ReturnDamageSuccessRate;
		}break;
		case 2:
		{
			return g_iWings3RecoverFullLifeSuccessRate;
		}break;
		case 3:
		{
			return g_iWings3RecoverFullManaSuccessRate;
		}break;
		case 4:
		{
			return g_iWings3SuccessfullBlockingRate;
		}break;
	}

	return 5;
}

#endif