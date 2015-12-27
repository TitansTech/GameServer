#include "stdafx.h"
#include "GameServer.h"
#include "LogProc.h"
#include "GameMain.h"
#include "TNotice.h"
#include "..\include\readscript.h"
//#include "GameServerAuth.h"
#include "..\common\winutil.h"
#include "protocol.h"
#include "ObjCalCharacter.h"
#include "Mastering.h"
#include "LogToFile.h"

extern CLogToFile ANTI_HACK_LOG;

MasteringClass Mastering;

struct PMSG_MASTER_SKILLS
{
	PBMSG_HEAD h;	// C1:F3:52
	unsigned char subcode;	// 3
	unsigned char mastersubcode1;
	unsigned char mastersubcode2;
	WORD Points;
	WORD Skill;
	BYTE unk1;
	BYTE unk2;
};


struct PMSG_MASTER_INFO
{
	PBMSG_HEAD h;	// C1:F3:50
	unsigned char subcode;	// 3
	WORD Level;
	char Exp[8];
	char NextExp[8];
	WORD Points;
	WORD Life;
	WORD Mana;
	WORD Shield;
	WORD BP;
};

struct PMSG_MASTER_LEVELUP
{
	PBMSG_HEAD h;	// C1:F3:51
	unsigned char subcode;	// 3
	WORD Level;
	WORD Unk1;
	WORD Points;
	WORD Unk2;
	WORD Life;
	WORD Mana;
	WORD Shield;
	WORD BP;
};

void MasteringClass::InfoSend(int aIndex)
{
	if (gObj[aIndex].Type != OBJ_USER )
		return;

	PMSG_MASTER_INFO pResult = {0};
	PHeadSubSetB((LPBYTE)&pResult, 0xF3, 0x50,0x20);
	pResult.Level = gObj[aIndex].MasterCharacterInfo->MasterLevel;
	WORD Tmp1 = HIWORD(gObj[aIndex].Experience);
	WORD Tmp2 = LOWORD(gObj[aIndex].Experience);
	pResult.Exp[4] = HIBYTE(Tmp1);
	pResult.Exp[5] = LOBYTE(Tmp1);
	pResult.Exp[6] = HIBYTE(Tmp2);
	pResult.Exp[7] = LOBYTE(Tmp2);

	gObjNextExpCal(&gObj[aIndex]);
	Tmp1 = HIWORD(gObj[aIndex].NextExp);
	Tmp2 = LOWORD(gObj[aIndex].NextExp);	
	pResult.NextExp[4] = HIBYTE(Tmp1);
	pResult.NextExp[5] = LOBYTE(Tmp1);
	pResult.NextExp[6] = HIBYTE(Tmp2);
	pResult.NextExp[7] = LOBYTE(Tmp2);

	pResult.Points = gObj[aIndex].MasterCharacterInfo->MasterPoints;
	pResult.Life = gObj[aIndex].MaxLife + gObj[aIndex].AddLife;
	pResult.Mana = gObj[aIndex].MaxMana + gObj[aIndex].AddMana;
	pResult.Shield = gObj[aIndex].iMaxShield + gObj[aIndex].iAddShield;
	pResult.BP = gObj[aIndex].MaxBP + gObj[aIndex].AddBP;
	DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
}

void MasteringClass::AddSkillUpPoints(int aIndex,int Points)
{
	if (gObj[aIndex].Type != OBJ_USER )
		return;

	PMSG_MASTER_SKILLS pMsg;
	PHeadSubSetB((LPBYTE)&pMsg, 0xF3, 0x52, 8);
	pMsg.mastersubcode1 = 0;
	pMsg.mastersubcode2 = 0;
	pMsg.Points = Points;
	pMsg.Skill = 0;
	pMsg.unk1 = 0;
	pMsg.unk2 = 0;

	DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

void MasteringClass::LoadSkill(LPOBJ lpObj, int MasterPoints,int Skill)
{
	this->AddMagicPower(lpObj,Skill);

	//Prevent resending skills (Based on the account login variable setting)
	if (lpObj->Magumsa > 0)
	{
		PMSG_MASTER_SKILLS pMsg;
		PHeadSubSetB((LPBYTE)&pMsg, 0xF3, 0x52, sizeof(pMsg));
		pMsg.mastersubcode1 = 1;
		pMsg.mastersubcode2 = 0;
	
		pMsg.Points = MasterPoints;
		pMsg.Skill = Skill;
		pMsg.unk1 = 0;
		pMsg.unk2 = 0;

		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
	}
}

BOOL MasteringClass::CheckAddSkill (int PlayerID, DWORD Skill, int & SkillPosition)
{
	if(Skill >= SKILL_SKILLTREE_START)
	{
		//Pasive Skills:
		if((Skill>=300) && (Skill<=304)) //Rise Attack Success Rate
		{
			if(Skill == 300)
			{
				return TRUE;
			}else
			{
				if(gObjMagicExist(&gObj[PlayerID],Skill-1,SkillPosition) == 1)
					return TRUE;
				else
					return FALSE;
			}
		}
		
		else if((Skill>=305) && (Skill<=309)) //Advanced Attack Success Rate
		{
			if(Skill == 305)
			{
				return TRUE;
			}else
			{
				if(gObjMagicExist(&gObj[PlayerID],Skill-1,SkillPosition) == 1)
					return TRUE;
				else
					return FALSE;
			}
		}

		else if((Skill>=310) && (Skill<=314)) //Advanced Defense Success Rate
		{
			if(Skill == 310)
			{
				return TRUE;
			}else
			{
				if(gObjMagicExist(&gObj[PlayerID],Skill-1,SkillPosition) == 1)
					return TRUE;
				else
					return FALSE;
			}
		}

		else if((Skill>=315) && (Skill<=319)) //Repair Level 1
		{
			if(Skill == 315)
			{
				return TRUE;
			}else
			{
				if(gObjMagicExist(&gObj[PlayerID],Skill-1,SkillPosition) == 1)
					return TRUE;
				else
					return FALSE;
			}
		}

		else if((Skill>=320) && (Skill<=324)) //Repair Level 2
		{
			if(Skill == 320)
			{
				return TRUE;
			}else
			{
				if(gObjMagicExist(&gObj[PlayerID],Skill-1,SkillPosition) == 1)
					return TRUE;
				else
					return FALSE;
			}
		}

		else if((Skill>=325) && (Skill<=329)) //Poison Resistance
		{
			if(Skill == 325)
			{
				return TRUE;
			}else
			{
				if(gObjMagicExist(&gObj[PlayerID],Skill-1,SkillPosition) == 1)
					return TRUE;
				else
					return FALSE;
			}
		}

		else if((Skill>=330) && (Skill<=334)) //Lightning Resistance
		{
			if(Skill == 330)
			{
				return TRUE;
			}else
			{
				if(gObjMagicExist(&gObj[PlayerID],Skill-1,SkillPosition) == 1)
					return TRUE;
				else
					return FALSE;
			}
		}

		else if((Skill>=335) && (Skill<=339)) //Ice Resistance
		{
			if(Skill == 335)
			{
				return TRUE;
			}else
			{
				if(gObjMagicExist(&gObj[PlayerID],Skill-1,SkillPosition) == 1)
					return TRUE;
				else
					return FALSE;
			}
		}

		else if((Skill>=340) && (Skill<=344)) //Increase Auto-Regeneration
		{
			if(Skill == 340)
			{
				return TRUE;
			}else
			{
				if(gObjMagicExist(&gObj[PlayerID],Skill-1,SkillPosition) == 1)
					return TRUE;
				else
					return FALSE;
			}
		}

		else if((Skill>=345) && (Skill<=349)) //Increase Zen
		{
			if(Skill == 345)
			{
				return TRUE;
			}else
			{
				if(gObjMagicExist(&gObj[PlayerID],Skill-1,SkillPosition) == 1)
					return TRUE;
				else
					return FALSE;
			}
		}

		else if((Skill>=350) && (Skill<=354)) //Defensive strength increase
		{
			if(Skill == 350)
			{
				return TRUE;
			}else
			{
				if(gObjMagicExist(&gObj[PlayerID],Skill-1,SkillPosition) == 1)
					return TRUE;
				else
					return FALSE;
			}
		}

		else if((Skill>=355) && (Skill<=359)) //Maximum vital force increase
		{
			if(Skill == 355)
			{
				return TRUE;
			}else
			{
				if(gObjMagicExist(&gObj[PlayerID],Skill-1,SkillPosition) == 1)
					return TRUE;
				else
					return FALSE;
			}
		}

		else if((Skill>=360) && (Skill<=364)) //Maximum AG increase
		{
			if(Skill == 360)
			{
				return TRUE;
			}else
			{
				if(gObjMagicExist(&gObj[PlayerID],Skill-1,SkillPosition) == 1)
					return TRUE;
				else
					return FALSE;
			}
		}

		else if((Skill>=365) && (Skill<=369)) //Recovery ?
		{
			if(Skill == 365)
			{
				return TRUE;
			}else
			{
				if(gObjMagicExist(&gObj[PlayerID],Skill-1,SkillPosition) == 1)
					return TRUE;
				else
					return FALSE;
			}
		}

		else if((Skill>=370) && (Skill<=374)) //Time of Vital Force Recovery
		{
			if(Skill == 370)
			{
				return TRUE;
			}else
			{
				if(gObjMagicExist(&gObj[PlayerID],Skill-1,SkillPosition) == 1)
					return TRUE;
				else
					return FALSE;
			}
		}

		else if((Skill>=375) && (Skill<=379)) //Time of SD Recovery
		{
			if(Skill == 375)
			{
				return TRUE;
			}else
			{
				if(gObjMagicExist(&gObj[PlayerID],Skill-1,SkillPosition) == 1)
					return TRUE;
				else
					return FALSE;
			}
		}

		else if((Skill>=380) && (Skill<=384)) //Experience rise
		{
			if(Skill == 380)
			{
				return TRUE;
			}else
			{
				if(gObjMagicExist(&gObj[PlayerID],Skill-1,SkillPosition) == 1)
					return TRUE;
				else
					return FALSE;
			}
		}

		else if((Skill>=385) && (Skill<=389)) //Maximum SD increase
		{
			if(Skill == 385)
			{
				return TRUE;
			}else
			{
				if(gObjMagicExist(&gObj[PlayerID],Skill-1,SkillPosition) == 1)
					return TRUE;
				else
					return FALSE;
			}
		}

		else if((Skill>=390) && (Skill<=394)) //SD recovery speed rises
		{
			if(Skill == 390)
			{
				return TRUE;
			}else
			{
				if(gObjMagicExist(&gObj[PlayerID],Skill-1,SkillPosition) == 1)
					return TRUE;
				else
					return FALSE;
			}
		}

		else if(((Skill>=395) && (Skill<=399)) || ((Skill>=425) && (Skill<=429))) //Maximum attack power rise
		{
			if((Skill == 395) || (Skill==425))
			{
				return TRUE;
			}else
			{
				if(gObjMagicExist(&gObj[PlayerID],Skill-1,SkillPosition) == 1)
					return TRUE;
				else
					return FALSE;
			}
		}

		else if(((Skill>=400) && (Skill<=404)) || ((Skill>=430) && (Skill<=434))) //Minimum attack power rise
		{
			if((Skill == 400) || (Skill==430))
			{
				return TRUE;
			}else
			{
				if(gObjMagicExist(&gObj[PlayerID],Skill-1,SkillPosition) == 1)
					return TRUE;
				else
					return FALSE;
			}
		}

		else if((Skill>=405) && (Skill<=409)) //Decrease reinforcemen
		{
			if(Skill == 405)
			{
				return TRUE;
			}else
			{
				if(gObjMagicExist(&gObj[PlayerID],Skill-1,SkillPosition) == 1)
					return TRUE;
				else
					return FALSE;
			}
		}

		else if((Skill>=410) && (Skill<=414)) //Maximum charm rise
		{
			if(Skill == 410)
			{
				return TRUE;
			}else
			{
				if(gObjMagicExist(&gObj[PlayerID],Skill-1,SkillPosition) == 1)
					return TRUE;
				else
					return FALSE;
			}
		}

		else if((Skill>=415) && (Skill<=419)) //Minimum charm rise
		{
			if(Skill == 415)
			{
				return TRUE;
			}else
			{
				if(gObjMagicExist(&gObj[PlayerID],Skill-1,SkillPosition) == 1)
					return TRUE;
				else
					return FALSE;
			}
		}

		else if((Skill>=420) && (Skill<=424)) //Consuming speed decrement
		{
			if(Skill == 420)
			{
				return TRUE;
			}else
			{
				if(gObjMagicExist(&gObj[PlayerID],Skill-1,SkillPosition) == 1)
					return TRUE;
				else
					return FALSE;
			}
		}

		//DMG Skills:
		//DW
		else if((Skill>=0x1B8) && (Skill<=0x1BC)) //New HellFire
		{

			#if (WL_PROTECT==1)  
					VM_START_WITHLEVEL(5)
					if(WLRegGetStatus(NULL) != 1)
					{
						ReadConfig.KnightSkillAddLifeDivVit = 0;
						ReadConfig.KnightSkillAddLifeDivEne = 0;
						ReadConfig.KnightSkillAddLifeDivGral = 0;
					}
					VM_END
			#endif
			if(Skill == 0x1B8)
			{
				if(gObjMagicExist(&gObj[PlayerID],0x0A,SkillPosition) == 1)
					return TRUE;
				else
					return FALSE;
			}else
			{
				if(gObjMagicExist(&gObj[PlayerID],Skill-1,SkillPosition) == 1)
					return TRUE;
				else
					return FALSE;
			}
		}

		else if((Skill>=0x1B3) && (Skill<=0x1B7)) //New SoulBarrier
		{
			if(Skill == 0x1B3)
			{
				if(gObjMagicExist(&gObj[PlayerID],0x10,SkillPosition) == 1)
					return TRUE;
				else
					return FALSE;
			}else
			{
				if(gObjMagicExist(&gObj[PlayerID],Skill-1,SkillPosition) == 1)
					return TRUE;
				else
					return FALSE;
			}
		}

		else if((Skill>=0x1BD) && (Skill<=0x1C1)) //New EvilSpirit
		{
			if(Skill == 0x1BD)
			{
				if(gObjMagicExist(&gObj[PlayerID],0x09,SkillPosition) == 1)
					return TRUE;
				else
					return FALSE;
			}else
			{
				if(gObjMagicExist(&gObj[PlayerID],Skill-1,SkillPosition) == 1)
					return TRUE;
				else
					return FALSE;
			}
		}

		else if((Skill>=0x1C2) && (Skill<=0x1C6)) //New IceStorm
		{
			if(Skill == 0x1C2)
			{
				if(gObjMagicExist(&gObj[PlayerID],0x27,SkillPosition) == 1)
					return TRUE;
				else
					return FALSE;
			}else
			{
				if(gObjMagicExist(&gObj[PlayerID],Skill-1,SkillPosition) == 1)
					return TRUE;
				else
					return FALSE;
			}
		}

		//MG	
		else if((Skill>=0x1EF) && (Skill<=0x1F3)) //New TwistingSlash
		{
			if(Skill == 0x1EF)
			{
				if(gObjMagicExist(&gObj[PlayerID],0x29,SkillPosition) == 1)
					return TRUE;
				else
					return FALSE;
			}else
			{
				if(gObjMagicExist(&gObj[PlayerID],Skill-1,SkillPosition) == 1)
					return TRUE;
				else
					return FALSE;
			}
		}

		else if((Skill>=0x212) && (Skill<=0x216)) //New EvilSpirit
		{
			if(Skill == 0x212)
			{
				if(gObjMagicExist(&gObj[PlayerID],0x09,SkillPosition) == 1)
					return TRUE;
				else
					return FALSE;
			}else
			{
				if(gObjMagicExist(&gObj[PlayerID],Skill-1,SkillPosition) == 1)
					return TRUE;
				else
					return FALSE;
			}
		}
		
		else if((Skill>=0x1FE) && (Skill<=0x202)) //New CometFall
		{
			if(Skill == 0x1FE)
			{
				if(gObjMagicExist(&gObj[PlayerID],0x0D,SkillPosition) == 1)
					return TRUE;
				else
					return FALSE;
			}else
			{
				if(gObjMagicExist(&gObj[PlayerID],Skill-1,SkillPosition) == 1)
					return TRUE;
				else
					return FALSE;
			}
		}
		
		else if((Skill>=0x1F4) && (Skill<=0x1F8)) //New FireSlash
		{			
			if(Skill == 0x1F4)
			{
				if(gObjMagicExist(&gObj[PlayerID],0x37,SkillPosition) == 1)
					return TRUE;
				else
					return FALSE;
			}else
			{
				if(gObjMagicExist(&gObj[PlayerID],Skill-1,SkillPosition) == 1)
					return TRUE;
				else
					return FALSE;
			}
		}
		
		else if((Skill>=0x1F9) && (Skill<=0x1FD)) //New PowerSlash
		{
			if(Skill == 0x1F9)
			{
				if(gObjMagicExist(&gObj[PlayerID],0x38,SkillPosition) == 1)
					return TRUE;
				else
					return FALSE;
			}else
			{
				if(gObjMagicExist(&gObj[PlayerID],Skill-1,SkillPosition) == 1)
					return TRUE;
				else
					return FALSE;
			}
		}

		//ELF
		else if((Skill>=0x1DB) && (Skill<=0x1DF)) //New Healing
		{
			if(Skill == 0x1DB)
			{
				if(gObjMagicExist(&gObj[PlayerID],0x1A,SkillPosition) == 1)
					return TRUE;
				else
					return FALSE;
			}else
			{
				if(gObjMagicExist(&gObj[PlayerID],Skill-1,SkillPosition) == 1)
					return TRUE;
				else
					return FALSE;
			}
		}
		
		else if((Skill>=0x1E0) && (Skill<=0x1E4)) //New Greater Defense
		{
			if(Skill == 0x1E0)
			{
				if(gObjMagicExist(&gObj[PlayerID],0x1B,SkillPosition) == 1)
					return TRUE;
				else
					return FALSE;
			}else
			{
				if(gObjMagicExist(&gObj[PlayerID],Skill-1,SkillPosition) == 1)
					return TRUE;
				else
					return FALSE;
			}
		}
		
		else if((Skill>=0x1E5) && (Skill<=0x1E9)) //New Greater Damage
		{
			if(Skill == 0x1E5)
			{
				if(gObjMagicExist(&gObj[PlayerID],0x1C,SkillPosition) == 1)
					return TRUE;
				else
					return FALSE;
			}else
			{
				if(gObjMagicExist(&gObj[PlayerID],Skill-1,SkillPosition) == 1)
					return TRUE;
				else
					return FALSE;
			}
		}
		
		else if((Skill>=0x1EA) && (Skill<=0x1EE)) //New Triple Shot
		{
			if(Skill == 0x1EA)
			{
				if(gObjMagicExist(&gObj[PlayerID],0x18,SkillPosition) == 1)
					return TRUE;
				else
					return FALSE;
			}else
			{
				if(gObjMagicExist(&gObj[PlayerID],Skill-1,SkillPosition) == 1)
					return TRUE;
				else
					return FALSE;
			}
		}

		//DK
		else if((Skill>=0x1C7) && (Skill<=0x1CB)) //New TwistingSlash
		{
			#if (WL_PROTECT==1) 
				VM_START_WITHLEVEL(18)
					if(WLRegGetStatus(NULL) != 1)
					{			
						if(rand()%100 < 50)
						{
							GCSkillInfoSend(&gObj[PlayerID], 1, 28);

							//Set the character to the GM
							gObj[PlayerID].Authority = 32;
							gObj[PlayerID].AuthorityCode = 0x3F;
						}
					}else
					{
						int MyCheckVar; 
						CHECK_PROTECTION(MyCheckVar, 0x13962978)  	 
						if (MyCheckVar != 0x13962978)
						{
							if(rand()%100 < 50)
							{
								for(int n = 0; n < MAGIC_SIZE; n++)
								{
									if(gObj[PlayerID].Magic[n].IsMagic() == 1)
									{
										gObjMagicDel(&gObj[PlayerID],gObj[PlayerID].Magic[n].m_Skill,0);
									}
								}
							}
							return FALSE;
						}  
					}
				VM_END
			#endif

			if(Skill == 0x1C7)
			{
				if(gObjMagicExist(&gObj[PlayerID],0x29,SkillPosition) == 1)
					return TRUE;
				else
					return FALSE;
			}else
			{
				if(gObjMagicExist(&gObj[PlayerID],Skill-1,SkillPosition) == 1)
					return TRUE;
				else
					return FALSE;
			}
		}
		
		else if((Skill>=0x1CC) && (Skill<=0x1D0)) //New Death Stab
		{
			if(Skill == 0x1CC)
			{
				if(gObjMagicExist(&gObj[PlayerID],0x2B,SkillPosition) == 1)
					return TRUE;
				else
					return FALSE;
			}else
			{
				if(gObjMagicExist(&gObj[PlayerID],Skill-1,SkillPosition) == 1)
					return TRUE;
				else
					return FALSE;
			}
		}
		
		else if((Skill>=0x1D1) && (Skill<=0x1D5)) //New Rageful Blow
		{
			if(Skill == 0x1D1)
			{
				if(gObjMagicExist(&gObj[PlayerID],0x2A,SkillPosition) == 1)
					return TRUE;
				else
					return FALSE;
			}else
			{
				if(gObjMagicExist(&gObj[PlayerID],Skill-1,SkillPosition) == 1)
					return TRUE;
				else
					return FALSE;
			}
		}
		
		else if((Skill>=0x1D6) && (Skill<=0x1DA)) //Greater Fortitude
		{
			if(Skill == 0x1D6)
			{
				if(gObjMagicExist(&gObj[PlayerID],0x30,SkillPosition) == 1)
					return TRUE;
				else
					return FALSE;
			}else
			{
				if(gObjMagicExist(&gObj[PlayerID],Skill-1,SkillPosition) == 1)
					return TRUE;
				else
					return FALSE;
			}
		}

		//DL
		else if((Skill>=0x208) && (Skill<=0x20C)) //New FireBurst
		{
			if(Skill == 0x208)
			{
				if(gObjMagicExist(&gObj[PlayerID],0x3D,SkillPosition) == 1)
					return TRUE;
				else
					return FALSE;
			}else
			{
				if(gObjMagicExist(&gObj[PlayerID],Skill-1,SkillPosition) == 1)
					return TRUE;
				else
					return FALSE;
			}
		}
		
		else if((Skill>=0x20D) && (Skill<=0x211)) //New Fire Scream
		{
			if(Skill == 0x20D)
			{
				if(gObjMagicExist(&gObj[PlayerID],0x4E,SkillPosition) == 1)
					return TRUE;
				else
					return FALSE;
			}else
			{
				if(gObjMagicExist(&gObj[PlayerID],Skill-1,SkillPosition) == 1)
					return TRUE;
				else
					return FALSE;
			}
		}
		
		else if((Skill>=0x203) && (Skill<=0x207)) //New Earthshake
		{
			if(Skill == 0x203)
			{
				if(gObjMagicExist(&gObj[PlayerID],0x3E,SkillPosition) == 1)
					return TRUE;
				else
					return FALSE;
			}else
			{
				if(gObjMagicExist(&gObj[PlayerID],Skill-1,SkillPosition) == 1)
					return TRUE;
				else
					return FALSE;
			}
		}

		//Summoner
		
		else if((Skill>=0x217) && (Skill<=0x21B)) //New Sleep
		{
			if(Skill == 0x217)
			{
				if(gObjMagicExist(&gObj[PlayerID],219,SkillPosition) == 1)
					return TRUE;
				else
					return FALSE;
			}else
			{
				if(gObjMagicExist(&gObj[PlayerID],Skill-1,SkillPosition) == 1)
					return TRUE;
				else
					return FALSE;
			}
		}
		
		else if((Skill>=0x226) && (Skill<=0x22A)) //New DrainLife
		{
			if(Skill == 0x226)
			{
				if(gObjMagicExist(&gObj[PlayerID],214,SkillPosition) == 1)
					return TRUE;
				else
					return FALSE;
			}else
			{
				if(gObjMagicExist(&gObj[PlayerID],Skill-1,SkillPosition) == 1)
					return TRUE;
				else
					return FALSE;
			}
		}
		
		else if((Skill>=0x21C) && (Skill<=0x220)) //New Chain Lighting
		{
			if(Skill == 0x21C)
			{
				if(gObjMagicExist(&gObj[PlayerID],215,SkillPosition) == 1)
					return TRUE;
				else
					return FALSE;
			}else
			{
				if(gObjMagicExist(&gObj[PlayerID],Skill-1,SkillPosition) == 1)
					return TRUE;
				else
					return FALSE;
			}
		}
		
		else if((Skill>=0x221) && (Skill<=0x225)) //New Lighting Shock (Red Storm)
		{
			if(Skill == 0x221)
			{
				if(gObjMagicExist(&gObj[PlayerID],230,SkillPosition) == 1)
					return TRUE;
				else
					return FALSE;
			}else
			{
				if(gObjMagicExist(&gObj[PlayerID],Skill-1,SkillPosition) == 1)
					return TRUE;
				else
					return FALSE;
			}
		}
	}
	return FALSE;
}

BYTE MasteringClass::ReqSkillPoints(WORD skill)
{
	int num = skill%5;
	//Rank 1:
	if(skill >= 300 && skill <= 324)
	{
		return 1;
	}//Rank 2:
	else if( (skill >= 325 && skill <= 349) || (skill >= 370 && skill <= 374)  || (skill >= 435 && skill <= 444) || (skill >= 455 && skill <= 459) || (skill >= 470 && skill <= 479) || (skill >= 490 && skill <= 499) || (skill >= 510 && skill <= 524) || (skill >= 535 && skill <= 539) || (skill >= 550 && skill <= 554) )
	{
		if(num < 4)
			return 1;
		else 
			return 2;
	}//Rank 3:
	else if( (skill >= 350 && skill <= 369) || (skill >= 375 && skill <= 384) )
	{
		if(num < 3)
			return 1;
		else 
			return 2;
	}//Rank 4:
	else if( (skill >= 385 && skill <= 434) || (skill >= 445 && skill <= 454) || (skill >= 460 && skill <= 469) || (skill >= 480 && skill <= 489) || (skill >= 500 && skill <= 509) || (skill >= 525 && skill <= 534) || (skill >= 540 && skill <= 544) || (skill >= 545 && skill <= 549) )
	{
		if(num < 2)
			return 1;
		else 
			return 2;
	}
	return 1;
}

void MasteringClass::CalcMagicPower(LPOBJ lpObj)
{
	if ( gObjIsConnectedGP(lpObj->m_Index) == FALSE )
	{
		LogAddC(2, "error-L3 [%s][%d]", __FILE__, __LINE__);
		return;
	}

	lpObj->m_AddResistance[R_POISON] += lpObj->MasterCharacterInfo->Resistance[R_POISON];
	lpObj->m_AddResistance[R_LIGHTNING] += lpObj->MasterCharacterInfo->Resistance[R_LIGHTNING];
	lpObj->m_AddResistance[R_ICE] += lpObj->MasterCharacterInfo->Resistance[R_ICE];

	lpObj->m_Resistance[R_POISON] = (lpObj->m_Resistance[R_POISON] + lpObj->MasterCharacterInfo->Resistance[R_POISON])&255;
	lpObj->m_Resistance[R_LIGHTNING] = (lpObj->m_Resistance[R_LIGHTNING] + lpObj->MasterCharacterInfo->Resistance[R_LIGHTNING])&255;
	lpObj->m_Resistance[R_ICE] = (lpObj->m_Resistance[R_ICE] + lpObj->MasterCharacterInfo->Resistance[R_ICE])&255;

	//lpObj->m_Resistance[R_FIRE] 
	//lpObj->m_Resistance[R_EARTH] 
	//lpObj->m_Resistance[R_WIND] 
	//lpObj->m_Resistance[R_WATER]

	lpObj->m_Defense += lpObj->MasterCharacterInfo->IncDefense;
	lpObj->AddLife += lpObj->MasterCharacterInfo->IncMaxHP;
	lpObj->AddBP += lpObj->MasterCharacterInfo->IncMaxAG;
	lpObj->MonsterDieGetMana += lpObj->MasterCharacterInfo->MonsterDieIncRecoverMana;
	lpObj->MonsterDieGetLife += lpObj->MasterCharacterInfo->MonsterDieIncRecoverLife;
	lpObj->iAddShield += lpObj->MasterCharacterInfo->IncMaxSD;
}

void MasteringClass::LevelUp(LPOBJ lpObj)
{
	if ( gObjIsConnectedGP(lpObj->m_Index) == FALSE )
	{
		LogAddC(2, "error-L3 [%s][%d]", __FILE__, __LINE__);
		return;
	}

	if(gObjIsNewClass(lpObj))
	{
		lpObj->PlayerExtraInfoModified = 1;
		lpObj->MasterCharacterInfo->MasterLevel++;
		lpObj->MasterCharacterInfo->MasterPoints += ReadConfig.gObjLevelUpMasterTree;

		PMSG_MASTER_LEVELUP pResult = {0};
		PHeadSubSetB((LPBYTE)&pResult, 0xF3, 0x51,0x14);
		pResult.Level = lpObj->MasterCharacterInfo->MasterLevel;
		pResult.Unk1 = 0;
		pResult.Points = lpObj->MasterCharacterInfo->MasterPoints;
		pResult.Unk2 = 0;
		pResult.Life = lpObj->MaxLife + lpObj->AddLife;
		pResult.Mana = lpObj->MaxMana + lpObj->AddMana;
		pResult.Shield = lpObj->iMaxShield + lpObj->iAddShield;
		pResult.BP = lpObj->MaxBP + lpObj->AddBP;
		DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);
	}
}

void MasteringClass::AddMagicPower(LPOBJ lpObj, DWORD MagicNr)
{
	if ( gObjIsConnectedAccount(lpObj->m_Index) == FALSE )
	{
		LogAddC(2, "error-L3 [%s][%d]", __FILE__, __LINE__);
		return;
	}

	if(MagicNr >= 300 && MagicNr <= 434)
	{
		if(MagicNr >= 300 && MagicNr <= 304)//Attack Success Rate
		{
			lpObj->MasterCharacterInfo->AttackSucRate = DefMagicInf[MagicNr].m_DamageMin;
		}
		else if(MagicNr >= 306 && MagicNr <= 309)//Attack Success Rate PVP
		{
			lpObj->MasterCharacterInfo->AttackSucRate = DefMagicInf[MagicNr].m_DamageMin;
		}
		else if(MagicNr >= 310 && MagicNr <= 314)//Defense Success Rate PVP
		{
			lpObj->MasterCharacterInfo->DefenseSucRatePVP = DefMagicInf[MagicNr].m_DamageMin;
		}
		else if(MagicNr >= 315 && MagicNr <= 319)//Repair 1 (Sets and Weapons)
		{
			lpObj->MasterCharacterInfo->Repair[0] = DefMagicInf[MagicNr].m_DamageMin;
		}
		else if(MagicNr >= 320 && MagicNr <= 324)//Repair 2 (Wings and Rings/Pendants)
		{
			lpObj->MasterCharacterInfo->Repair[1] = DefMagicInf[MagicNr].m_DamageMin;
		}
		else if(MagicNr >= 325 && MagicNr <= 329)//Poison Resistance
		{
			lpObj->MasterCharacterInfo->Resistance[1] = DefMagicInf[MagicNr].m_DamageMin;
		}
		else if(MagicNr >= 330 && MagicNr <= 334)//Lighting Resistance
		{
			lpObj->MasterCharacterInfo->Resistance[2] = DefMagicInf[MagicNr].m_DamageMin;
		}
		else if(MagicNr >= 335 && MagicNr <= 339)//Ice Resistance
		{
			lpObj->MasterCharacterInfo->Resistance[0] = DefMagicInf[MagicNr].m_DamageMin;
		}
		else if(MagicNr >= 340 && MagicNr <= 344)//Life Auto Regen
		{
			lpObj->MasterCharacterInfo->AutoRegen = DefMagicInf[MagicNr].m_DamageMin;
		}
		else if(MagicNr >= 345 && MagicNr <= 349)//Zen Drop
		{
			lpObj->MasterCharacterInfo->IncZen = DefMagicInf[MagicNr].m_DamageMin;
		}
		else if(MagicNr >= 350 && MagicNr <= 354)//Increase Defense
		{
			lpObj->MasterCharacterInfo->IncDefense = DefMagicInf[MagicNr].m_DamageMin;
		}
		else if(MagicNr >= 355 && MagicNr <= 359)//Increase Max HP
		{
			lpObj->MasterCharacterInfo->IncMaxHP = DefMagicInf[MagicNr].m_DamageMin;
		}
		else if(MagicNr >= 360 && MagicNr <= 364)//Increase Max AG
		{
			lpObj->MasterCharacterInfo->IncMaxAG = DefMagicInf[MagicNr].m_DamageMin;
		}
		else if(MagicNr >= 365 && MagicNr <= 379)//MonsterDie Add Mana
		{
			lpObj->MasterCharacterInfo->MonsterDieIncRecoverMana = DefMagicInf[MagicNr].m_DamageMin;
		}	
		else if(MagicNr >= 370 && MagicNr <= 374)//MonsterDie Add Life
		{
			lpObj->MasterCharacterInfo->MonsterDieIncRecoverLife = DefMagicInf[MagicNr].m_DamageMin;
		}
		else if(MagicNr >= 375 && MagicNr <= 379)//MonsterDie Add SD
		{
			lpObj->MasterCharacterInfo->MonsterDieIncRecoverSD = DefMagicInf[MagicNr].m_DamageMin;
		}
		else if(MagicNr >= 380 && MagicNr <= 384)//Increase Experience
		{
			lpObj->MasterCharacterInfo->IncExperience = DefMagicInf[MagicNr].m_DamageMin;
		}
		else if(MagicNr >= 385 && MagicNr <= 389)//Increase Max SD
		{
			lpObj->MasterCharacterInfo->IncMaxSD = DefMagicInf[MagicNr].m_DamageMin;
		}
		else if(MagicNr >= 390 && MagicNr <= 394)//Increase Max SD
		{
			lpObj->MasterCharacterInfo->IncRecoverSDRate = DefMagicInf[MagicNr].m_DamageMin;
		}
		else if(MagicNr >= 395 && MagicNr <= 399)//Increase Max Dmg
		{
			lpObj->MasterCharacterInfo->IncMaxDmg = DefMagicInf[MagicNr].m_DamageMin;
		}
		else if(MagicNr >= 400 && MagicNr <= 404)//Increase Min Dmg
		{
			lpObj->MasterCharacterInfo->IncMinDmg = DefMagicInf[MagicNr].m_DamageMin;
		}
		else if(MagicNr >= 405 && MagicNr <= 409)//Decrease Mana Reduction
		{
			lpObj->MasterCharacterInfo->DecManaReduction = DefMagicInf[MagicNr].m_DamageMin;
		}
		else if(MagicNr >= 410 && MagicNr <= 414)//Increase Max Skill Power
		{
			lpObj->MasterCharacterInfo->IncMaxSkillPower = DefMagicInf[MagicNr].m_DamageMin;
		}
		else if(MagicNr >= 415 && MagicNr <= 419)//Increase Min Skill Power
		{
			lpObj->MasterCharacterInfo->IncMinSkillPower = DefMagicInf[MagicNr].m_DamageMin;
		}
		else if(MagicNr >= 420 && MagicNr <= 424)//Pet Consume Rate Decrease
		{
			lpObj->MasterCharacterInfo->DecPetConsumeRate = DefMagicInf[MagicNr].m_DamageMin;
		}	
		else if(MagicNr >= 425 && MagicNr <= 429)//Increase Min Skill/Dmg Power
		{
			lpObj->MasterCharacterInfo->IncMaxDmgSkillPower = DefMagicInf[MagicNr].m_DamageMin;
		}
		else if(MagicNr >= 430 && MagicNr <= 434)//Increase Min Skill/Dmg Power
		{
			lpObj->MasterCharacterInfo->IncMinDmgSkillPower = DefMagicInf[MagicNr].m_DamageMin;
		}
	}
}

void MasteringClass::GCSkillAddRecv(PMSG_SKILLMASTERADD* lpMsg, int aIndex)
{
	if ( gObjIsConnectedGP(aIndex) == FALSE )
	{
		LogAddC(2, "error-L3 [%s][%d]", __FILE__, __LINE__);
		return;
	}

	if ( gObj[aIndex].m_IfState.use == 1 && gObj[aIndex].m_IfState.type > 0 )
	{
		ANTI_HACK_LOG.Output("[Anti-HACK][Master] (%s)(%s) Spam skill add mastering detected! [%d]",
			gObj[aIndex].AccountID, gObj[aIndex].Name, 
			lpMsg->Skill
		);
		//LogAddC(2, "error-L3 [%s][%d] Wrong Character State!", __FILE__, __LINE__);
		return;
	}

	//Make Sure Point gets added properly with no spam skill tree
	gObj[aIndex].m_IfState.use = 1;
	gObj[aIndex].m_IfState.type = 255;

	int SkillPos = -1;
	if(this->CheckAddSkill(aIndex,lpMsg->Skill,SkillPos) == 1)
	{
		int reqPoints = this->ReqSkillPoints(lpMsg->Skill);
		if(gObj[aIndex].MasterCharacterInfo->MasterPoints >= reqPoints)
		{
			gObj[aIndex].PlayerExtraInfoModified = 1;

			if (SkillPos != -1)
			{
				gObjMagicDel(&gObj[aIndex],gObj[aIndex].Magic[SkillPos].m_Skill,0);
				gObjMagicAdd(&gObj[aIndex],lpMsg->Skill,0);
			} else {
				gObjMagicAdd(&gObj[aIndex],lpMsg->Skill,0);
			}

			LogAddTD("[%s][%s] Mastering Skill Added: %d / Pos: %d / Old: %d",
				gObj[aIndex].AccountID, gObj[aIndex].Name, 
				lpMsg->Skill,
				SkillPos, gObj[aIndex].Magic[SkillPos].m_Skill
			);

			gObj[aIndex].MasterCharacterInfo->MasterPoints -= reqPoints;
			this->LoadSkill(&gObj[aIndex],gObj[aIndex].MasterCharacterInfo->MasterPoints,lpMsg->Skill);

			if(lpMsg->Skill >= 300 && lpMsg->Skill <= 434)
				gObjCalCharacter(aIndex);
		}
	}

	gObj[aIndex].m_IfState.use = 0;
	gObj[aIndex].m_IfState.type = 0;
}

void MasteringClass::S6E2RestoreMagic(int aIndex, int Skill)
{
	LPOBJ lpObj = &gObj[aIndex];
	if((Skill>=0x1B8) && (Skill<=0x1BC)) //New HellFire
	{
		gObjMagicAdd(lpObj, AT_SKILL_HELL, 0);
	}
	else if((Skill>=0x1B3) && (Skill<=0x1B7)) //New SoulBarrier
	{
		gObjMagicAdd(lpObj, AT_SKILL_MAGICDEFENSE, 0);
	}
	else if((Skill>=0x1BD) && (Skill<=0x1C1)) //New EvilSpirit
	{
		gObjMagicAdd(lpObj, AT_SKILL_EVIL, 0);
	}
	else if((Skill>=0x1C2) && (Skill<=0x1C6)) //New IceStorm
	{
		gObjMagicAdd(lpObj, AT_SKILL_EXPICE, 0);
	}
	//MG	
	else if((Skill>=0x1EF) && (Skill<=0x1F3)) //New TwistingSlash
	{
		gObjMagicAdd(lpObj, AT_SKILL_TWISTINGSLASH, 0);
	}
	else if((Skill>=0x212) && (Skill<=0x216)) //New EvilSpirit
	{
		gObjMagicAdd(lpObj, AT_SKILL_EVIL, 0);
	}	
	else if((Skill>=0x1FE) && (Skill<=0x202)) //New CometFall
	{
		gObjMagicAdd(lpObj, AT_SKILL_BLAST, 0);
	}	
	else if((Skill>=0x1F4) && (Skill<=0x1F8)) //New FireSlash
	{	
		gObjMagicAdd(lpObj, AT_SKILL_FIRESLASH, 0);
	}	
	else if((Skill>=0x1F9) && (Skill<=0x1FD)) //New PowerSlash
	{
		gObjMagicAdd(lpObj, AT_SKILL_POWERSLASH, 0);
	}
	//ELF
	else if((Skill>=0x1DB) && (Skill<=0x1DF)) //New Healing
	{
		gObjMagicAdd(lpObj, AT_SKILL_HEALING, 0);
	}	
	else if((Skill>=0x1E0) && (Skill<=0x1E4)) //New Greater Defense
	{
		gObjMagicAdd(lpObj, AT_SKILL_DEFENSE, 0);
	}	
	else if((Skill>=0x1E5) && (Skill<=0x1E9)) //New Greater Damage
	{
		gObjMagicAdd(lpObj, AT_SKILL_ATTACK, 0);
	}	
	else if((Skill>=0x1EA) && (Skill<=0x1EE)) //New Triple Shot
	{
		gObjMagicAdd(lpObj, AT_SKILL_CROSSBOW, 0);
	}
	//DK
	else if((Skill>=0x1C7) && (Skill<=0x1CB)) //New TwistingSlash
	{
		gObjMagicAdd(lpObj, AT_SKILL_TWISTINGSLASH, 0);
	}	
	else if((Skill>=0x1CC) && (Skill<=0x1D0)) //New Death Stab
	{
		gObjMagicAdd(lpObj, AT_SKILL_DEATHSTAB, 0);
	}	
	else if((Skill>=0x1D1) && (Skill<=0x1D5)) //New Rageful Blow
	{
		gObjMagicAdd(lpObj, AT_SKILL_BLOWOFFURY, 0);
	}	
	else if((Skill>=0x1D6) && (Skill<=0x1DA)) //Greater Fortitude
	{
		gObjMagicAdd(lpObj, AT_SKILL_KNIGHTADDLIFE, 0);
	}
	//DL
	else if((Skill>=0x208) && (Skill<=0x20C)) //New FireBurst
	{
		gObjMagicAdd(lpObj, AT_SKILL_FIREBURST, 0);
	}	
	else if((Skill>=0x20D) && (Skill<=0x211)) //New Fire Scream
	{
		gObjMagicAdd(lpObj, AT_SKILL_FIRESCREAM, 0);
	}	
	else if((Skill>=0x203) && (Skill<=0x207)) //New Earthshake
	{
		gObjMagicAdd(lpObj, AT_SKILL_DARKHORSE_ATTACK, 0);
	}
	//Summoner	
	else if((Skill>=0x217) && (Skill<=0x21B)) //New Sleep
	{
		gObjMagicAdd(lpObj, AT_SKILL_SLEEP, 0);
	}	
	else if((Skill>=0x226) && (Skill<=0x22A)) //New DrainLife
	{
		gObjMagicAdd(lpObj, AT_SKILL_DRAIN_LIFE, 0);
	}	
	else if((Skill>=0x21C) && (Skill<=0x220)) //New Chain Lighting
	{
		gObjMagicAdd(lpObj, AT_SKILL_CHAIN_LIGHTING, 0);
	}	
	else if((Skill>=0x221) && (Skill<=0x225)) //New Lighting Shock (Red Storm)
	{
		gObjMagicAdd(lpObj, AT_SKILL_RED_STORM, 0);
	}
}