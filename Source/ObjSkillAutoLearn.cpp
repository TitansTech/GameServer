#include "stdafx.h"
#include "MagicInf.h"
#include "..\include\readscript.h"
#include "LogProc.h"
#include "User.h"
#include "ObjSkillAutoLearn.h"

#if (PACK_EDITION>=2)

cObjSkillAutoLearn ObjSkillAutoLearn;

void cObjSkillAutoLearn::Load(char * FileName)
{
	this->Count = 0;
	memset(&this->skill[0],0,sizeof(this->skill));
	int Token;

	SMDFile = fopen(FileName, "r");

	if ( SMDFile == NULL )
	{
		MsgBox("SkillAutoLearn data load error %s", FileName);
		return;
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

					this->skill[this->Count].number = TokenNumber;

					Token = GetToken();
					this->skill[this->Count].requireLevel = TokenNumber;

					Token = GetToken();
					this->skill[this->Count].requireReset = TokenNumber;

					for(int i=0;i<MAX_TYPE_PLAYER;i++)
					{
						Token = GetToken();
						this->skill[this->Count].RequireClass[i] = TokenNumber;
					}
					
					this->Count++;
				}
			}
		break;
	}
		
	fclose(SMDFile);
}

int cObjSkillAutoLearn::AddSkill(int aIndex, int aSkill)
{
	if((MagicDamageC.SkillGetRequireClass(gObj[aIndex].Class,gObj[aIndex].ChangeUP,aSkill) < 1) && (aSkill != 0xFFFF))
	{
		return -1;
	}

	for(int n = 0; n < MAGIC_SIZE; n++)
	{
		if(gObj[aIndex].Magic[n].IsMagic() == 1)
		{
			if(gObj[aIndex].Magic[n].m_Skill == aSkill)
			{
				return -1;
			}
		}
	}

	for(int n = 0; n < MAGIC_SIZE; n++)
	{
		if(gObj[aIndex].Magic[n].IsMagic() == 0)
		{
			int skill = gObj[aIndex].Magic[n].Set(aSkill,0);
			if(skill < 0)
			{
				return -1;
			}
			else
			{
				gObj[aIndex].MagicCount++;
				GCMagicListOneSend(gObj[aIndex].m_Index,n,aSkill,0xDC,0,0);
				return n;
			}
		}
	}

	return -1;
}

void cObjSkillAutoLearn::LevelUp(int aIndex)
{
	if ( this->Enabled == 0 )
	{
		return;
	}

	if ( gObj[aIndex].Type != OBJ_USER )
	{
		return;
	}
	
	if ( gObjIsConnectedGP(aIndex) == FALSE )
	{
		LogAddC(2, "error-L3 [%s][%d]", __FILE__, __LINE__);
		return;
	}

	BYTE Class = gObj[aIndex].Class;
	for(int i=0;i<this->Count;i++)
	{
		if(this->skill[i].RequireClass[Class] > 0)
		{
			if(gObj[aIndex].Resets >= this->skill[i].requireReset)
			{
				if(gObj[aIndex].Level == this->skill[i].requireLevel)
				{
					if(this->AddSkill(aIndex,this->skill[i].number) != -1)
					{
						char sbuf[512]={0};
						wsprintf(sbuf,"Now I feel the power of %s inside me.",MagicDamageC.GetName(this->skill[i].number));	
						GCServerMsgStringSend(sbuf,gObj[aIndex].m_Index, 0x01);						
						LogAddTD("[%s][%s] Auto Learn (%d)%s skill",gObj[aIndex].AccountID,gObj[aIndex].Name,this->skill[i].number,MagicDamageC.GetName(this->skill[i].number));
					}
				}
			}
		}
	}
}

#endif