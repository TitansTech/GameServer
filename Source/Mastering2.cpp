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
#include "Mastering2.h"
#include "LogToFile.h"
#include "SCFExDBProtocol.h"
#include "SCFExDB_Defines.h"


extern CLogToFile ANTI_HACK_LOG;

struct PMSG_MASTER_POINTS
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


struct PMSG_MASTER_SKILLSEND
{
	PWMSG_HEAD h;
	BYTE subcode;
	DWORD dwCountMaster;
};

struct PMSG_MASTER_SKILLS
{
	BYTE btPos;//Skill Pos
	BYTE btLevel;//Level
	float fCurInfo;//Current Skill Percent
	float fNextInfo;//Next Skill Percent
};

cMastering2 Mastering2;

void cMastering2::GetValuebyLevel(DWORD Skill, BYTE Level, float & Current, float & Next)
{
	int SkillPos = this->GetSkillPosition(Skill);
	Current = 0;
	Next = 0;
	if(SkillPos >= 0)
	{
		if(Level == 1)
			Current = this->Skill[SkillPos].Value;
		else
		{
			if(this->Skill[SkillPos].Value != 1.0f)
			{
				if( Skill != 533)
				{
					float Val = this->Skill[SkillPos].Value + (this->Skill[SkillPos].Value * Level / ReadConfig.Master_Div1);
					if(Val > ReadConfig.Master_ValueLimit)
					{
						Current = this->Skill[SkillPos].Value + (this->Skill[SkillPos].Value * Level / ReadConfig.Master_Div2);
					}else
						Current = Val;
				}else
				{
					float Val = this->Skill[SkillPos].Value - (this->Skill[SkillPos].Value * Level / ReadConfig.Master_Div3);
					if(Val >= 5)
					{
						Current = Val;
					}else
						Current = 5;
				}
			}
			else
				Current = this->Skill[SkillPos].Value + (this->Skill[SkillPos].Value * Level);
		}

		if(Level < this->Skill[SkillPos].LevelMax)
		{
			if(Level == 1)	
			{
				if(this->Skill[SkillPos].Value != 1.0f)	
				{
					if( Skill != 533)
					{
						float Val = this->Skill[SkillPos].Value + (this->Skill[SkillPos].Value * ((Level-1)+this->Skill[SkillPos].LevelReq) / ReadConfig.Master_Div1);
						if(Val > ReadConfig.Master_ValueLimit)
						{
							Next = this->Skill[SkillPos].Value + (this->Skill[SkillPos].Value * ((Level-1)+this->Skill[SkillPos].LevelReq) / ReadConfig.Master_Div2);
						}else
							Next = Val;
					}else
					{
						float Val = this->Skill[SkillPos].Value - (this->Skill[SkillPos].Value * ((Level-1)+this->Skill[SkillPos].LevelReq) / ReadConfig.Master_Div3);
						if(Val >= 5)
						{
							Next = Val;
						}else
							Next = 5;
					}
				}
				else
					Next = this->Skill[SkillPos].Value + (this->Skill[SkillPos].Value * ((Level-1)+this->Skill[SkillPos].LevelReq));
			}else				
			{
				if(this->Skill[SkillPos].Value != 1.0f)
				{
					if( Skill != 533)
					{
						float Val = this->Skill[SkillPos].Value + (this->Skill[SkillPos].Value * (Level+this->Skill[SkillPos].LevelReq) / ReadConfig.Master_Div1);
						if(Val > ReadConfig.Master_ValueLimit)
						{
							Next = this->Skill[SkillPos].Value + (this->Skill[SkillPos].Value * (Level+this->Skill[SkillPos].LevelReq) / ReadConfig.Master_Div2);
						}else
							Next = Val;
					}else
					{
						float Val = this->Skill[SkillPos].Value - (this->Skill[SkillPos].Value * (Level+this->Skill[SkillPos].LevelReq) / ReadConfig.Master_Div3);
						if(Val >= 5)
						{
							Next = Val;
						}else
							Next = 5;
					}
				}else					
					Next = this->Skill[SkillPos].Value + (this->Skill[SkillPos].Value * (Level+this->Skill[SkillPos].LevelReq));
			}
		}else
			Next = Current;
	}
}


float cMastering2::GetValuebyLevel(int Skill, int Level)
{
	int SkillPos = this->GetSkillPosition(Skill);

	if(SkillPos >= 0)
	{
		if(Level == 1)
			return this->Skill[SkillPos].Value;
		else
			if(this->Skill[SkillPos].Value != 1)
			{
				if( Skill != 533)
				{
					float Val = this->Skill[SkillPos].Value + (this->Skill[SkillPos].Value * Level / ReadConfig.Master_Div1);
					if(Val > ReadConfig.Master_ValueLimit)
					{
						Val = this->Skill[SkillPos].Value + (this->Skill[SkillPos].Value * Level / ReadConfig.Master_Div2);
					}
					return Val;
				}else
				{
					float Val = this->Skill[SkillPos].Value - (this->Skill[SkillPos].Value * Level / ReadConfig.Master_Div3);
					if(Val < 5)
					{
						Val = 5;
					}
					return Val;
				}
			}else
				return this->Skill[SkillPos].Value + (this->Skill[SkillPos].Value * Level);
	}
	return 0;
}

void cMastering2::GCMagicListMultiSend(int aIndex, bool SendMagicReplace) //, bool ReformuleValues)
{	
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	bool HaveOffensive = false;
	PMSG_MASTER_SKILLSEND pMsgSLC;
	PMSG_MASTER_SKILLS pMsgSL;
	int sOfs = sizeof(PMSG_MASTER_SKILLSEND);
	int pMsgSLSize = sizeof(PMSG_MASTER_SKILLS);
	BYTE sendBuf[2048];
	LPOBJ lpObj = &gObj[aIndex];
	int skillcount = 0;

	for(int n = 0; n < MAX_SKILLTREE; n++)
	{
		if(lpObj->MasterCharacterInfo->Skill[n].Type >= SKILL_SKILLTREE_START)
		{
			int Pos = this->GetSkillPosition(lpObj->MasterCharacterInfo->Skill[n].Type);
			pMsgSL.btPos = this->Skill[Pos].RequireClass[lpObj->Class];

			if(Pos > -1)
			{
				if (this->Skill[Pos].LevelMax < lpObj->MasterCharacterInfo->Skill[n].Level)
				{
					LogAddTD("[Master S6][Skill: %d][%s][%s] Level error detected: %d VS %d",
						lpObj->MasterCharacterInfo->Skill[n].Type,
						lpObj->AccountID, lpObj->Name,
						lpObj->MasterCharacterInfo->Skill[n].Level, this->Skill[Pos].LevelMax
					);

					int diffLvl = 0;
					diffLvl = lpObj->MasterCharacterInfo->Skill[n].Level - this->Skill[Pos].LevelMax;
					lpObj->MasterCharacterInfo->MasterPoints += diffLvl;

					lpObj->MasterCharacterInfo->Skill[n].Level = this->Skill[Pos].LevelMax;

					Mastering.InfoSend(lpObj->m_Index);
				}

				if(lpObj->MasterCharacterInfo->Skill[n].Type >= 325)//Offensive
				{
					if(SendMagicReplace == true)
					{
						this->MagicReplace(aIndex,this->Skill[Pos].OriginalSkill,lpObj->MasterCharacterInfo->Skill[n].Type);
						HaveOffensive = true;
					}
				}
			}

			pMsgSL.btLevel = lpObj->MasterCharacterInfo->Skill[n].Level;

			float Current = 0;
			float Next = 0;
			this->GetValuebyLevel(lpObj->MasterCharacterInfo->Skill[n].Type,lpObj->MasterCharacterInfo->Skill[n].Level,Current,Next);
			pMsgSL.fCurInfo = Current;
			pMsgSL.fNextInfo = Next;
			memcpy(&sendBuf[sOfs], &pMsgSL, pMsgSLSize);
			skillcount++;
			sOfs += pMsgSLSize;

			//if(ReformuleValues == true)
			//	this->AddSinglePassive(aIndex,n,0);
		}
	}

	PHeadSubSetW((LPBYTE)&pMsgSLC, 0xF3, 0x53, sizeof(pMsgSLC));
	pMsgSLC.dwCountMaster = skillcount;
	pMsgSLC.h.sizeH = SET_NUMBERH(sOfs);
	pMsgSLC.h.sizeL = SET_NUMBERL(sOfs);
	memcpy(sendBuf, &pMsgSLC, sizeof(PMSG_MASTER_SKILLSEND));
	sendBuf[sOfs] = 0;
	DataSend(aIndex, sendBuf, sOfs);
	
	if(ReadConfig.S6E2 == 1)
	{
		if(HaveOffensive == true)
		{
			PMSG_SKILLKEY pMsg = {0};

			PHeadSubSetB((LPBYTE)&pMsg, 0xF3, 0x30, sizeof(pMsg));
			memcpy(&pMsg.SKillKey[0], &gObj[aIndex].sSkillKey, sizeof(gObj[aIndex].sSkillKey));
			DataSend(aIndex, (UCHAR*)&pMsg, pMsg.h.size);
		}
	}
}

void cMastering2::BaseSkill(int & Skill)
{
	if(ReadConfig.S6E2 == 1)
	{
		if(Skill >= 325) //Offensive Start
		{
			int skillPos = this->GetSkillPosition(Skill);

			if(skillPos == -1)
				return;

			if(this->Skill[skillPos].OriginalSkill > 0)
			{
				Skill = this->Skill[skillPos].OriginalSkill;
			}
		}
	}
}

void cMastering2::Load(char * FilePath)
{
	memset(&this->Skill[0],0,sizeof(this->Skill));
	this->SkillCount = 0;

	int Token;

	SMDFile = fopen(FilePath, "r");

	if ( SMDFile == NULL )
	{
		MsgBox("SCF MasterSkillTreeData data load error %s", FilePath);
		return;
	}

	while ( true )
	{
		int iType = GetToken();
		iType = TokenNumber;
		if ( iType == 0 )
		{
			while(true)
			{
				if( (this->SkillCount+1) >= MAXSKILLTREECOUNT)
					break; 
				Token = GetToken();
				if ( strcmp("end", TokenString) == 0 )
				{
					break;
				}else
				{
					this->Skill[this->SkillCount].Type									= TokenNumber;
					Token = GetToken();
					this->Skill[this->SkillCount].RequireSkill[0]	 					= TokenNumber;
					Token = GetToken();
					this->Skill[this->SkillCount].RequireSkill[1]						= TokenNumber;
					Token = GetToken();
					this->Skill[this->SkillCount].Category 								= TokenNumber;
					Token = GetToken();
					this->Skill[this->SkillCount].Ranking								= TokenNumber;
					Token = GetToken();	
					this->Skill[this->SkillCount].OriginalSkill							= TokenNumber;					
					Token = GetToken();	
					this->Skill[this->SkillCount].LevelReq								= TokenNumber;					
					Token = GetToken();	
					this->Skill[this->SkillCount].LevelMax								= TokenNumber;		
					Token = GetToken();	
					this->Skill[this->SkillCount].Value									= TokenNumber;
					Token = GetToken();
					this->Skill[this->SkillCount].RequireClass[CLASS_WIZARD]			= TokenNumber;
					Token = GetToken();
					this->Skill[this->SkillCount].RequireClass[CLASS_KNIGHT]			= TokenNumber;
					Token = GetToken();
					this->Skill[this->SkillCount].RequireClass[CLASS_ELF]				= TokenNumber;
					Token = GetToken();
					this->Skill[this->SkillCount].RequireClass[CLASS_MAGICGLADIATOR]	= TokenNumber;
					Token = GetToken();
					this->Skill[this->SkillCount].RequireClass[CLASS_DARKLORD]			= TokenNumber;
					Token = GetToken();
					this->Skill[this->SkillCount].RequireClass[CLASS_SUMMONER]			= TokenNumber;
					Token = GetToken();
					this->Skill[this->SkillCount].RequireClass[CLASS_RAGEFIGHTER]		= TokenNumber;

					LogAddTD("[SCF MasterSkillTreeData][Skill][%d] Type:%d [%d,%d,%d,%d,%d,%d,%d,%f,%d,%d,%d,%d,%d,%d,%d]",
						this->SkillCount,
						this->Skill[this->SkillCount].Type,
						this->Skill[this->SkillCount].RequireSkill[0],
						this->Skill[this->SkillCount].RequireSkill[1],
						this->Skill[this->SkillCount].Category,
						this->Skill[this->SkillCount].Ranking,
						this->Skill[this->SkillCount].OriginalSkill,
						this->Skill[this->SkillCount].LevelReq,
						this->Skill[this->SkillCount].LevelMax,
						this->Skill[this->SkillCount].Value,
						this->Skill[this->SkillCount].RequireClass[CLASS_WIZARD],
						this->Skill[this->SkillCount].RequireClass[CLASS_KNIGHT],
						this->Skill[this->SkillCount].RequireClass[CLASS_ELF],
						this->Skill[this->SkillCount].RequireClass[CLASS_MAGICGLADIATOR],
						this->Skill[this->SkillCount].RequireClass[CLASS_DARKLORD],
						this->Skill[this->SkillCount].RequireClass[CLASS_SUMMONER],
						this->Skill[this->SkillCount].RequireClass[CLASS_RAGEFIGHTER]
					);
					this->SkillCount++;
				}
			}
		}
		break;
	}

	fclose(SMDFile);
	LogAddTD("[SCF MasterSkillTreeData][Skills: %d] - %s file is Loaded",this->SkillCount,FilePath);
}

void cMastering2::AddSkillUpPoints(int aIndex)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	if (gObj[aIndex].Type != OBJ_USER )
		return;

	PMSG_MASTER_POINTS pMsg;
	PHeadSubSetB((LPBYTE)&pMsg, 0xF3, 0x52, 8);
	pMsg.mastersubcode1 = 0;
	pMsg.mastersubcode2 = 0;
	pMsg.Points = gObj[aIndex].MasterCharacterInfo->MasterPoints;
	pMsg.Skill = 0;
	pMsg.unk1 = 0;
	pMsg.unk2 = 0;

	DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

void cMastering2::GCSkillAddRecv(PMSG_SKILLMASTERADD* lpMsg, int aIndex)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}
	if ( gObjIsConnectedGP(aIndex) == FALSE )
	{
		LogAddC(2, "error-L3 [%s][%d]", __FILE__, __LINE__);
		return;
	}

	if (gObj[aIndex].PlayerExtraInfoReceived != 1)
	{
		GCServerMsgStringSend ("ERROR: Please relogin!",aIndex,0x01 ) ;
		LogAddC(2, "ERROR: MASTERING Info [%s][%s] Line: %d", gObj[aIndex].AccountID, gObj[aIndex].Name, __LINE__);
		return;
	}

	if ( gObj[aIndex].m_IfState.use == 1 && gObj[aIndex].m_IfState.type > 0 )
	{
		ANTI_HACK_LOG.Output("[Anti-HACK][Master S6] (%s)(%s) Spam skill add mastering detected! [%d]",
			gObj[aIndex].AccountID, gObj[aIndex].Name, 
			lpMsg->Skill
		);
		return;
	}

	//Make Sure Point gets added properly with no spam skill tree
	gObj[aIndex].m_IfState.use = 1;
	gObj[aIndex].m_IfState.type = 255;

	int SkillPos = -1;

	if(gObj[aIndex].PlayerSkillTreeInfoReceived == 1)
	{

		if(this->CheckAddSkill(aIndex,lpMsg->Skill,SkillPos) == true)
		{
			int ExistSkill = -1;
			if(this->MagicExist(aIndex,lpMsg->Skill,ExistSkill) == 1)
			{
				gObj[aIndex].MasterCharacterInfo->Skill[ExistSkill].Level += this->Skill[SkillPos].LevelReq;

				LogAddTD("[%s][%s] Skill Level Upgrade: %d / Pos: %d / Exist: %d / Lvl: %d",
					gObj[aIndex].AccountID, gObj[aIndex].Name, 
					lpMsg->Skill,
					SkillPos, ExistSkill,
					gObj[aIndex].MasterCharacterInfo->Skill[ExistSkill].Level 
				);
				//this->AddSinglePassive(aIndex,ExistSkill,1);
			}
			else
			{
				int NewMagic = this->MagicAdd(aIndex,lpMsg->Skill,this->Skill[SkillPos].LevelReq);
				
				if(NewMagic >= 0)
				{
					LogAddTD("[%s][%s] Skill Added: %d / Pos: %d / LvlReq: %d",
						gObj[aIndex].AccountID, gObj[aIndex].Name, 
						lpMsg->Skill,
						SkillPos,
						this->Skill[SkillPos].LevelReq 
					);
					//this->AddSinglePassive(aIndex,NewMagic,0);
				} else {
					LogAddTD("[%s][%s] ERROR: Cant add skill: %d / Pos: %d / LvlReq: %d",
						gObj[aIndex].AccountID, gObj[aIndex].Name, 
						lpMsg->Skill,
						SkillPos,
						this->Skill[SkillPos].LevelReq 
					);

					GCServerMsgStringSend ("ERROR: Cant add skill!",aIndex,0x01 ) ;
					gObj[aIndex].m_IfState.use = 0;
					gObj[aIndex].m_IfState.type = 0;
					return;
				}
			}

			gObj[aIndex].MasterCharacterInfo->MasterPoints -= this->Skill[SkillPos].LevelReq;
			this->MagicReplace(aIndex,this->Skill[SkillPos].OriginalSkill,lpMsg->Skill);
			gObj[aIndex].PlayerExtraInfoModified = 1;
			gObj[aIndex].PlayerSkillTreeInfoModified = 1;

			this->GCMagicListMultiSend(aIndex,false);
			this->AddSkillUpPoints(aIndex);

			//if(this->Skill[SkillPos].OriginalSkill == 0)
			//{
				gObjCalCharacter(aIndex);
			//}
		} else 
		{
			GCServerMsgStringSend ("ERROR: Add skill failed!",aIndex,0x01 ) ;
		}
	}else
	{
		GetPlayerExtraInfo(aIndex,SCFExDB_GSSend_GetMasterSkillTreeData);

		GCServerMsgStringSend ("ERROR: ReRequesting SkillTree info",aIndex, 0x01);
		LogAddC(2, "ReRequesting: SKILTREE Info [%s] Line: %d", gObj[aIndex].AccountID, __LINE__);
	}

	gObj[aIndex].m_IfState.use = 0;
	gObj[aIndex].m_IfState.type = 0;
}

bool cMastering2::CheckRanking(int aIndex, BYTE Ranking)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return false;
	}

	if(Ranking == 1)
		return true;

	WORD Class = gObj[aIndex].Class;

	for(int n = 0; n < this->SkillCount; n++)
	{
		if(this->Skill[n].Ranking == (Ranking-1) && this->Skill[n].RequireClass[Class] > 0)
		{
			int skillPosTmp = 0;
			if(this->MagicExist(aIndex,this->Skill[n].Type,skillPosTmp) == 1)
			{
				if(gObj[aIndex].MasterCharacterInfo->Skill[skillPosTmp].Level >= 10)
					return true;
			}
			//if(gObjMagicExist(&gObj[aIndex],this->Skill[n].Type,skillPosTmp) == 1)
			//{
			//	if(gObj[aIndex].Magic[skillPosTmp].m_Level >= 10)
			//		return true;
			//}
		}
	}

	return false;
}

BOOL cMastering2::MagicExist(int aIndex, WORD Skill, int & SkillPosition)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return false;
	}

	LPOBJ lpObj = &gObj[aIndex];

	for(int n = 0; n < MAX_SKILLTREE; n++)
	{
		if(lpObj->MasterCharacterInfo->Skill[n].Type >= SKILL_SKILLTREE_START)
		{
			if(lpObj->MasterCharacterInfo->Skill[n].Type == Skill)
			{
				SkillPosition = n;
				return 1;
			}
		}
	}
	SkillPosition = -1;
	return 0;
}

int cMastering2::MagicAdd(int aIndex, WORD aSkill, BYTE Level)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return -1;
	}

	LPOBJ lpObj = &gObj[aIndex];
	int skill,n,reqeng;

	for(n = 0; n < MAX_SKILLTREE; n++)
	{
		if(lpObj->MasterCharacterInfo->Skill[n].Type >= SKILL_SKILLTREE_START)
		{
			if(lpObj->MasterCharacterInfo->Skill[n].Type == aSkill)
			{
				LogAdd(lMsg.Get(555),lpObj->MasterCharacterInfo->Skill[n].Type,aSkill);
				return -1;
			}
		}
	}

	for(n = 0; n < MAX_SKILLTREE; n++)
	{
		if(lpObj->MasterCharacterInfo->Skill[n].Type < SKILL_SKILLTREE_START)
		{
			lpObj->MasterCharacterInfo->Skill[n].Type = aSkill;
			lpObj->MasterCharacterInfo->Skill[n].Level = Level;

			return n;
		}
	}

	LogAdd(lMsg.Get(554),__FILE__,__LINE__);
	return -1;
}


int cMastering2::GetSkillPosition(int Type)
{
	for(int i=0;i<this->SkillCount;i++)
	{
		if(this->Skill[i].Type == Type)
		{
			return i;
		}
	}
	return -1;	
}

bool cMastering2::CheckAddSkill(int PlayerID, DWORD Skill, int & SkillPosition)
{
	if(OBJMAX_RANGE(PlayerID) == 0)
	{
		return false;
	}

	int ExistSkill = 0;

	int skillPos = this->GetSkillPosition(Skill);

	SkillPosition = skillPos;//

	if(skillPos == -1)
	{
		GCServerMsgStringSend ("Cant find this skill!",PlayerID,0x01 ) ;
		LogAddTD("[%s][%s] Cant find this skill: %d !",
			gObj[PlayerID].AccountID, gObj[PlayerID].Name, 
			Skill 
		);

		return false;
	}

	if(this->CheckRanking(PlayerID,this->Skill[skillPos].Ranking) == false)
	{
		GCServerMsgStringSend ("Need more skill ranking!",PlayerID,0x01 ) ;
		LogAddTD("[%s][%s] Need more skill ranking: %d/%d ",
			gObj[PlayerID].AccountID, gObj[PlayerID].Name, 
			Skill, this->Skill[skillPos].Ranking
		);

		return false;
	}

	if(gObj[PlayerID].MasterCharacterInfo->MasterPoints < this->Skill[skillPos].LevelReq)
	{
		GCServerMsgStringSend ("Not enough master points!",PlayerID,0x01 ) ;
		LogAddTD("[%s][%s] Not enough master points: [%d] %d/%d ",
			gObj[PlayerID].AccountID, gObj[PlayerID].Name, 
			Skill, 
			gObj[PlayerID].MasterCharacterInfo->MasterPoints, this->Skill[skillPos].LevelReq
		);

		return false;
	}

	if(this->MagicExist(PlayerID,Skill,ExistSkill) == 1)
	{
		if(gObj[PlayerID].MasterCharacterInfo->Skill[ExistSkill].Level < this->Skill[skillPos].LevelMax)
		{
			return true;
		}else
		{
			GCServerMsgStringSend ("Your Level is too low!",PlayerID,0x01 ) ;
			LogAddTD("[%s][%s] Your Level is too low: [%d] %d/%d ",
				gObj[PlayerID].AccountID, gObj[PlayerID].Name, 
				Skill, 
				gObj[PlayerID].Magic[ExistSkill].m_Level, this->Skill[skillPos].LevelMax
			);

			return false;
		}
	}

	////Passive
	//if(this->Skill[skillPos].Offensive == 0)
	//{
		int SkillPos1 = 0;
		int SkillPos2 = 0;

		if(this->Skill[skillPos].RequireSkill[0] > 0)
		{
			BOOL Exist = FALSE;
			if(this->Skill[skillPos].RequireSkill[0] >= SKILL_SKILLTREE_START)
			{
				Exist = this->MagicExist(PlayerID,this->Skill[skillPos].RequireSkill[0],SkillPos1);
			}
			else
			{
				Exist = gObjMagicExist(&gObj[PlayerID],this->Skill[skillPos].RequireSkill[0],SkillPos1);
			}

			if(Exist == 1)
			{
				if(this->Skill[skillPos].RequireSkill[1] > 0)
				{
					BOOL Exist2 = FALSE;
					if(this->Skill[skillPos].RequireSkill[1] >= SKILL_SKILLTREE_START)
					{
						Exist2 = this->MagicExist(PlayerID,this->Skill[skillPos].RequireSkill[1],SkillPos2);
					}else
					{
						Exist2 = gObjMagicExist(&gObj[PlayerID],this->Skill[skillPos].RequireSkill[1],SkillPos2);
					}

					if(Exist2 == 1)
					{
						return true;
					}
					else
					{
						GCServerMsgStringSend ("Skill requirement failed!",PlayerID,0x01 ) ;
						LogAddTD("[%s][%s] Skill requirement failed: [%d] %d, %d ",
							gObj[PlayerID].AccountID, gObj[PlayerID].Name, 
							Skill, 
							this->Skill[skillPos].RequireSkill[0], this->Skill[skillPos].RequireSkill[1]
						);

						return false;
					}
				}
				else
				{
					return true;
				}
			}
			else
			{
				GCServerMsgStringSend ("Skill basic requirement failed!",PlayerID,0x01 ) ;
				LogAddTD("[%s][%s] Skill basic requirement failed: [%d] %d, %d ",
					gObj[PlayerID].AccountID, gObj[PlayerID].Name, 
					Skill, 
					this->Skill[skillPos].RequireSkill[0], this->Skill[skillPos].RequireSkill[1]
				);

				return false;
			}
		}
		return true;
	//}

	////Offensive
	//if(this->Skill[skillPos].RequireSkill[0] > 0)
	//{
	//	bool Exist1 = false;
	//	bool Exist2 = false;
	//	int SkillPos1 = 0;
	//	int SkillPos2 = 0;

	//	if(gObjMagicExist(&gObj[PlayerID],this->Skill[skillPos].RequireSkill[0],SkillPos1) == 1)
	//	{
	//		Exist1 = true;
	//		SkillPosition = SkillPos1;
	//	}

	//	if(this->Skill[skillPos].RequireSkill[1] > 0)
	//	{
	//		if(gObjMagicExist(&gObj[PlayerID],this->Skill[skillPos].RequireSkill[1],SkillPos2) == 1)
	//		{
	//			Exist2 = true;
	//		}
	//	}else
	//	{
	//		Exist2 = true;
	//	}

	//	if(Exist1 == true && Exist2 == true)
	//		return true;

		//if(this->Skill[skillPos].RequireSkill[0] > SKILL_SKILLTREE_START)
		//{
		//}else
		//{
		//}

	//}
	//return false;
}

void cMastering2::AddSinglePassive(int aIndex, int Pos, int Exist)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	if(gObj[aIndex].MasterCharacterInfo->Skill[Pos].Type >= SKILL_SKILLTREE_START)
	{
		//gObj[aIndex].MasterCharacterInfo->IncMinSkillPower = 0;
		//gObj[aIndex].MasterCharacterInfo->IncDefenseShield = 0;

		float Reduction = 0.0;
		if (Exist == 1)
		{
			Reduction = this->GetValuebyLevel(gObj[aIndex].MasterCharacterInfo->Skill[Pos].Type,(gObj[aIndex].MasterCharacterInfo->Skill[Pos].Level-1));
		}
		float Val = this->GetValuebyLevel(gObj[aIndex].MasterCharacterInfo->Skill[Pos].Type,gObj[aIndex].MasterCharacterInfo->Skill[Pos].Level) - Reduction;

		switch(gObj[aIndex].MasterCharacterInfo->Skill[Pos].Type)
		{
			//Universal
			case 300:
			case 578:
			{
				if(Val > 100)
					Val = 100;
				gObj[aIndex].MasterCharacterInfo->Repair[0] += Val;
			}break;
			case 301:
			case 579:
			{
				gObj[aIndex].MasterCharacterInfo->DefenseSucRatePVP += Val;
			}break;
			case 302:
			case 580:
			{
				gObj[aIndex].MasterCharacterInfo->IncMaxSD += Val;
			}break;
			case 303:
			case 581:
			{
				gObj[aIndex].MasterCharacterInfo->RegenMana += Val;
			}break;
			case 304:
			case 582:
			{
				gObj[aIndex].MasterCharacterInfo->Resistance2[R_POISON] += Val;
			}break;
			case 305:
			case 583:
			{
				if(Val > 100)
					Val = 100;
				gObj[aIndex].MasterCharacterInfo->Repair[1] += Val;
			}break;
			case 306:
			case 584:
			{
				//if(Val > 100)
				//	Val = 100;
				gObj[aIndex].MasterCharacterInfo->IncRecoverSDRate += Val;
			}break;
			case 307:
			case 585:
			{
				//if(Val > 100)
				//	Val = 100;
				gObj[aIndex].MasterCharacterInfo->AutoRegen += Val;
			}break;
			case 308:
			case 586:
			{
				gObj[aIndex].MasterCharacterInfo->Resistance2[R_LIGHTNING] += Val;
			}break;
			case 309:
			case 587:
			{	
				gObj[aIndex].MasterCharacterInfo->IncDefense += Val;
			}break;
			case 310:
			case 588:
			{	
				gObj[aIndex].MasterCharacterInfo->RegenAG += Val;
			}break;
			case 311:
			case 589:
			{
				gObj[aIndex].MasterCharacterInfo->Resistance2[R_ICE] += Val;
			}break;
			case 312:
			case 590:
			{
				if(Val > 100)
					Val = 100;
				gObj[aIndex].MasterCharacterInfo->Repair[2] += Val;
			}break;
			case 313:
			case 591:
			{
				if(Val > 100)
					Val = 100;
				gObj[aIndex].MasterCharacterInfo->DefenseSucRate += Val;
			}break;

			//2nd Tree
			case 325:
			case 599:
			{				
				gObj[aIndex].MasterCharacterInfo->AttackSucRate += Val;
			}break;
			case 334:
			case 600:
			{
				gObj[aIndex].MasterCharacterInfo->IncMaxHP += Val;
			}break;
			case 338:
			case 601:
			{
				gObj[aIndex].MasterCharacterInfo->IncMaxMana += Val;
			}break;

			//DK
			case 326:
			{
				gObj[aIndex].MasterCharacterInfo->IncCyclone += Val;
			}break;
			case 327:
			{
				gObj[aIndex].MasterCharacterInfo->IncSlash += Val;
			}break;
			case 328:
			{
				gObj[aIndex].MasterCharacterInfo->IncFallingSlash += Val;
			}break;
			case 329:
			{
				gObj[aIndex].MasterCharacterInfo->IncLunge += Val;
			}break;
			case 330:
			{
				gObj[aIndex].MasterCharacterInfo->IncTwistingSlash += Val;
			}break;
			case 331:
			{
				if(Val > 100)
					Val = 100;
				gObj[aIndex].MasterCharacterInfo->IncRagefulBlow += Val;
			}break;
			case 332:
			{
				if(Val > 100)
					Val = 100;
				gObj[aIndex].MasterCharacterInfo->PullTwistingSlash += Val;
			}break;
			case 333:
			{
				if(Val > 100)
					Val = 100;
				gObj[aIndex].MasterCharacterInfo->BrokenArmorRagefulBlow += Val;
			}break;
			case 335:
			{
				gObj[aIndex].MasterCharacterInfo->IncPhysicalAttackPower += Val;
			}break;
			case 336:
			{
				gObj[aIndex].MasterCharacterInfo->IncDeathStab += Val;
			}break;
			case 337:
			{
				gObj[aIndex].MasterCharacterInfo->IncStrikeofDestruction += Val;
			}break;

			//DW
			case 378:
			{
				gObj[aIndex].MasterCharacterInfo->IncFlame += Val;
			}break;
			case 379:
			{
				gObj[aIndex].MasterCharacterInfo->IncLighting += Val;
			}break;
			case 381:
			{
				gObj[aIndex].MasterCharacterInfo->IncInferno += Val;
			}break;
			case 382:
			{
				gObj[aIndex].MasterCharacterInfo->IncCometFall += Val;
			}break;
			case 385:
			{
				gObj[aIndex].MasterCharacterInfo->IncEvilSpirits += Val;
			}break;
			case 387:
			{
				gObj[aIndex].MasterCharacterInfo->IncDecay += Val;
			}break;
			case 388:
			{
				gObj[aIndex].MasterCharacterInfo->IncHellFire += Val;
			}break;
			case 389:
			{
				gObj[aIndex].MasterCharacterInfo->IncIce += Val;
			}break;
			case 380:
			{
				if(Val > 100)
					Val = 100;
				gObj[aIndex].MasterCharacterInfo->IncExpansionWizardy1 += Val;
			}break;
			case 383:
			{
				if(Val > 100)
					Val = 100;
				gObj[aIndex].MasterCharacterInfo->IncExpansionWizardy2 += Val;
			}break;
			case 386:
			{				
				gObj[aIndex].MasterCharacterInfo->IncMinSkillPower += Val;
				gObj[aIndex].MasterCharacterInfo->IncMaxSkillPower += Val;
			}break;
			case 384:
			{				
				gObj[aIndex].MasterCharacterInfo->IncPoison += Val;
			}break;

			//ELF
			case 413:
			{				
				gObj[aIndex].MasterCharacterInfo->IncHealing += Val;
			}break;
			case 414:
			{				
				gObj[aIndex].MasterCharacterInfo->IncTripleShot += Val;
			}break;
			case 415:
			{				
				gObj[aIndex].MasterCharacterInfo->IncMonsterCallHP += Val;
			}break;
			case 416:
			{				
				gObj[aIndex].MasterCharacterInfo->IncPenetration += Val;
			}break;
			case 417:
			{		
				if(Val > 100)
					Val = 100;		
				gObj[aIndex].MasterCharacterInfo->IncGreaterDefense += Val;
			}break;
			case 418:
			{				
				gObj[aIndex].MasterCharacterInfo->IncTripleShotOneArrow += Val;
			}break;
			case 419:
			{				
				gObj[aIndex].MasterCharacterInfo->IncMonsterCallDefense += Val;
			}break;
			case 420:
			{		
				if(Val > 100)
					Val = 100;		
				gObj[aIndex].MasterCharacterInfo->IncGreaterAttack += Val;
			}break;
			case 421:
			{
				gObj[aIndex].MasterCharacterInfo->IncPhysicalAttackPower += Val;
			}break;
			case 422:
			{		
				if(Val > 100)
					Val = 100;		
				gObj[aIndex].MasterCharacterInfo->IncGreaterAttackTime += Val;
			}break;
			case 423:
			{		
				if(Val > 100)
					Val = 100;		
				gObj[aIndex].MasterCharacterInfo->IncGreaterDefenseTime += Val;
			}break;
			case 424:
			{
				gObj[aIndex].MasterCharacterInfo->IncIceArrow += Val;
			}break;

			//SUM

			case 448:
			{
				gObj[aIndex].MasterCharacterInfo->IncFireBook1 += Val;
			}break;
			case 449:
			{
				gObj[aIndex].MasterCharacterInfo->IncWindBook1 += Val;
			}break;
			case 450:
			{
				gObj[aIndex].MasterCharacterInfo->IncLightingBook2 += Val;
			}break;
			case 451:
			{
				gObj[aIndex].MasterCharacterInfo->IncFireBook2 += Val;
			}break;
			case 452:
			{
				gObj[aIndex].MasterCharacterInfo->IncWindBook2 += Val;
			}break;
			case 453:
			{
				if(Val > 100)
					Val = 100;
				gObj[aIndex].MasterCharacterInfo->IncLightingBook2 += Val;
			}break;
			case 454:
			{
				if(Val > 100)
					Val = 100;
				gObj[aIndex].MasterCharacterInfo->IncSleep += Val;
			}break;
			case 455:
			{
				gObj[aIndex].MasterCharacterInfo->IncChainLighting += Val;
			}break;
			case 456:
			{
				gObj[aIndex].MasterCharacterInfo->IncLightingShock += Val;
			}break;
			case 457:
			{
				gObj[aIndex].MasterCharacterInfo->IncMinSkillPower += Val;
				gObj[aIndex].MasterCharacterInfo->IncMaxSkillPower += Val;
			}break;
			case 458:
			{
				gObj[aIndex].MasterCharacterInfo->IncDrainLife += Val;
			}break;

			//MG
			case 479:
			{
				gObj[aIndex].MasterCharacterInfo->IncCyclone += Val;
			}break;
			case 480:
			{
				gObj[aIndex].MasterCharacterInfo->IncLighting += Val;
			}break;
			case 481:
			{
				gObj[aIndex].MasterCharacterInfo->IncTwistingSlash += Val;
			}break;
			case 482:
			{
				gObj[aIndex].MasterCharacterInfo->IncPowerSlash += Val;
			}break;
			case 483:
			{
				gObj[aIndex].MasterCharacterInfo->IncFlame += Val;
			}break;
			case 484:
			{
				gObj[aIndex].MasterCharacterInfo->IncCometFall += Val;
			}break;
			case 485:
			{
				gObj[aIndex].MasterCharacterInfo->IncPhysicalAttackPower += Val;
			}break;
			case 486:
			{
				gObj[aIndex].MasterCharacterInfo->IncInferno += Val;
			}break;
			case 487:
			{
				gObj[aIndex].MasterCharacterInfo->IncEvilSpirits += Val;
			}break;
			case 488:
			{				
				gObj[aIndex].MasterCharacterInfo->IncMinSkillPower += Val;
				gObj[aIndex].MasterCharacterInfo->IncMaxSkillPower += Val;
			}break;
			case 489:
			{
				gObj[aIndex].MasterCharacterInfo->IncIce += Val;
			}break;
			case 490:
			{
				gObj[aIndex].MasterCharacterInfo->IncFireSlash += Val;
			}break;

			case 508:
			{
				gObj[aIndex].MasterCharacterInfo->IncFireBurst1 += Val;
			}break;
			case 509:
			{
				gObj[aIndex].MasterCharacterInfo->IncForceWave += Val;
			}break;
			case 510:
			{
				gObj[aIndex].MasterCharacterInfo->IncDefenseWDarkHorse += Val;
			}break;
			case 511:
			{
				gObj[aIndex].MasterCharacterInfo->IncCriticalDamage1 += Val;
			}break;
			case 512:
			{
				gObj[aIndex].MasterCharacterInfo->IncEarthShake1 += Val;
			}break;
			case 513:
			case 557:
			{
				gObj[aIndex].MasterCharacterInfo->IncPhysicalAttackPower += Val;
			}break;
			case 514:
			{
				if(Val > 100)
					Val = 100;
				gObj[aIndex].MasterCharacterInfo->IncFireBurst2 += Val;
			}break;
			case 515:
			{
				gObj[aIndex].MasterCharacterInfo->IncCriticalDamage2 += Val;
			}break;
			case 516:
			{
				if(Val > 100)
					Val = 100;
				gObj[aIndex].MasterCharacterInfo->IncEarthShake2 += Val;
			}break;
			case 517:
			{
				if(Val > 100)
					Val = 100;
				gObj[aIndex].MasterCharacterInfo->IncCriticalDamage3 += Val;
			}break;
			case 518:
			{
				gObj[aIndex].MasterCharacterInfo->IncFireScream += Val;
			}break;
			
			
			//3rd Tree
			case 347:
			case 603:
			{
				gObj[aIndex].MasterCharacterInfo->AttackSucRatePVP += Val;
			}break;
			case 357:
			case 604:
			{
				if(Val > 100)
					Val = 100;
				gObj[aIndex].MasterCharacterInfo->DecManaReduction += Val;
			}break;
			case 358:
			case 605:
			{
				gObj[aIndex].MasterCharacterInfo->MonsterDieIncRecoverSD += Val;
			}break;
			case 359:
			case 606:
			{
				gObj[aIndex].MasterCharacterInfo->MonsterDieIncRecoverLife += Val;
			}break;
			case 361:
			case 607:
			{
				gObj[aIndex].MasterCharacterInfo->IncMinDmg += Val;
			}break;
			case 362:
			case 608:
			{
				gObj[aIndex].MasterCharacterInfo->MonsterDieIncRecoverMana += Val;
			}break;

			//DK
			case 348:
			{
				gObj[aIndex].MasterCharacterInfo->IncPhysicalAttackPowerTwoHand += Val;
			}break;
			case 349:
			{
				gObj[aIndex].MasterCharacterInfo->IncPhysicalAttackPowerOneHand += Val;
			}break;
			case 350:
			{
				gObj[aIndex].MasterCharacterInfo->IncPhysicalAttackPowerMace += Val;
			}break;
			case 351:
			{
				gObj[aIndex].MasterCharacterInfo->IncPhysicalAttackPowerSpear += Val;
			}break;
			case 352:
			{
				gObj[aIndex].MasterCharacterInfo->IncPhysicalAttackPowerTwoHandPvP += Val;
			}break;
			case 353:
			{
				gObj[aIndex].MasterCharacterInfo->IncAttackSpeedOneHand += Val;
			}break;
			case 354:
			{
				if(Val > 100)
					Val = 100;
				gObj[aIndex].MasterCharacterInfo->MaceStun += Val;
			}break;
			case 355:
			{
				if(Val > 100)
					Val = 100;
				gObj[aIndex].MasterCharacterInfo->SpearDoubleDmg += Val;
			}break;
			case 356:
			{
				if(Val > 100)
					Val = 100;
				gObj[aIndex].MasterCharacterInfo->SwellLifeStamina += Val;
			}break;
			case 360:
			{
				if(Val > 100)
					Val = 100;
				gObj[aIndex].MasterCharacterInfo->SwellLifeMana += Val;
			}break;

			//DW
			case 397:
			{
				gObj[aIndex].MasterCharacterInfo->IncWizardyOneHand += Val;
			}break;
			case 398:
			{
				gObj[aIndex].MasterCharacterInfo->IncWizardyTwoHand += Val;
			}break;
			case 399:
			{
				gObj[aIndex].MasterCharacterInfo->IncDefenseShield += Val;
			}break;
			case 400:
			{
				gObj[aIndex].MasterCharacterInfo->IncStaffSpeedOneHand += Val;
			}break;
			case 401:
			{
				gObj[aIndex].MasterCharacterInfo->IncStaffPvPAttackTwoHand += Val;
			}break;
			case 402:
			{
				gObj[aIndex].MasterCharacterInfo->IncDefenseShield += Val;
			}break;
			case 403:
			{
				if(Val > 100)
					Val = 100;
				gObj[aIndex].MasterCharacterInfo->IncSoulBarrier += Val;
			}break;
			case 404:
			{
				gObj[aIndex].MasterCharacterInfo->IncSoulBarrierTime += Val;
			}break;
			case 405:
			{
				gObj[aIndex].MasterCharacterInfo->IncMinSkillPower += Val;
			}break;

			//Elf
			
			case 435:
			{
				gObj[aIndex].MasterCharacterInfo->IncPhysicalAttackPowerMace += Val;
			}break;
			case 436:
			{
				gObj[aIndex].MasterCharacterInfo->IncPhysicalAttackPowerSpear += Val;
			}break;
			case 437:
			{
				gObj[aIndex].MasterCharacterInfo->IncDefenseShield += Val;
			}break;
			case 438:
			{
				gObj[aIndex].MasterCharacterInfo->IncAttackSpeedOneHand += Val;
			}break;
			case 439:
			{
				gObj[aIndex].MasterCharacterInfo->IncPhysicalAttackPowerTwoHandPvP += Val;
			}break;
			case 440:
			{
				gObj[aIndex].MasterCharacterInfo->IncDefenseShield += Val;
			}break;
			case 441:
			{
				gObj[aIndex].MasterCharacterInfo->IncInfiniteArrow += Val;
			}break;
			case 442:
			{
				gObj[aIndex].MasterCharacterInfo->IncMinDmg += Val;
			}break;

			//Summoner

			case 465:
			{
				gObj[aIndex].MasterCharacterInfo->IncWizardyOneHand += Val;
			}break;
			case 466:
			{
				gObj[aIndex].MasterCharacterInfo->IncWizardyTwoHand += Val;
			}break;
			case 467:
			{
				gObj[aIndex].MasterCharacterInfo->IncStaffPvPAttackTwoHand += Val;
			}break;
			case 468:
			{
				gObj[aIndex].MasterCharacterInfo->IncAttackSpeedOneHand += Val;
			}break;
			case 470:
			{
				gObj[aIndex].MasterCharacterInfo->IncBerserker1 += Val;
			}break;
			case 471:
			{
				gObj[aIndex].MasterCharacterInfo->IncBerserker2 += Val;
			}break;
			case 472:
			{
				gObj[aIndex].MasterCharacterInfo->IncMinSkillPower += Val;
				gObj[aIndex].MasterCharacterInfo->IncExpansionWizardy1 += Val;
			}break;

			//DL
			case 526:
			{
				gObj[aIndex].MasterCharacterInfo->IncDarkSpirit1 += Val;
			}break;
			case 527:
			{
				gObj[aIndex].MasterCharacterInfo->IncWizardyTwoHand += Val;
			}break;
			case 528:
			{
				gObj[aIndex].MasterCharacterInfo->IncDefenseShield += Val;
			}break;
			case 529:
			{
				gObj[aIndex].MasterCharacterInfo->IncScepterAdds1 += Val;
			}break;
			case 530:
			{
				gObj[aIndex].MasterCharacterInfo->IncDarkSpirit2 += Val;
			}break;
			case 531:
			{
				gObj[aIndex].MasterCharacterInfo->IncStaffPvPAttackTwoHand += Val;
			}break;
			case 532:
			{
				gObj[aIndex].MasterCharacterInfo->IncDefenseShield += Val;
			}break;
			case 533:
			{
				gObj[aIndex].MasterCharacterInfo->IncScepterAdds2 += Val;
			}break;
			case 534:
			{
				gObj[aIndex].MasterCharacterInfo->IncDarkSpirit3 += Val;
			}break;
			case 535:
			{
				if(Val > 100)
					Val = 100;
				gObj[aIndex].MasterCharacterInfo->DecPetLifeReduction += Val;
			}break;

			//RF			
			case 568:
			{
				gObj[aIndex].MasterCharacterInfo->IncPhysicalAttackPowerMace += Val;
			}break;
			case 569:
			{
				if(Val > 100)
					Val = 100;
				gObj[aIndex].MasterCharacterInfo->DefSucRateSkillMULT += Val;
			}break;
			case 571:
			{
				if(Val > 100)
					Val = 100;
				gObj[aIndex].MasterCharacterInfo->SpearDoubleDmg += Val;
			}break;
			case 572:
			{
				gObj[aIndex].MasterCharacterInfo->DefSucRateSkillSUM += Val;
			}break;
			case 573:
			{
				gObj[aIndex].MasterCharacterInfo->FitnessSkillSUM += Val;
			}break;		
			case 558:
			{
				gObj[aIndex].MasterCharacterInfo->IncChainDrive += Val;
			}break;		
			case 559:
			{
				gObj[aIndex].MasterCharacterInfo->IncDarkSide += Val;
			}break;		
			case 560:
			{
				gObj[aIndex].MasterCharacterInfo->IncDragonLore += Val;
			}break;		
			case 551:
			{
				gObj[aIndex].MasterCharacterInfo->IncLargerRingBlower += Val;
			}break;		
			case 552:
			{
				gObj[aIndex].MasterCharacterInfo->IncUpperBeast += Val;
			}break;		
			case 554:
			{
				gObj[aIndex].MasterCharacterInfo->IncLargerRingBlowerDebuf += Val;
			}break;		
			case 555:
			{
				gObj[aIndex].MasterCharacterInfo->IncUpperBeastDebuf += Val;
			}break;		
		}
	}
}


void cMastering2::Passive(int aIndex)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	if(gObjIsNewClass(&gObj[aIndex]) == FALSE)
		return;

	for(int n = 0; n < MAX_SKILLTREE; n++)
	{
		this->AddSinglePassive(aIndex,n, 0);
	}

	//LogAddTD("[Mastering2][Debugging][%s][%s] %5.2f", /*%5.2f, %5.2f, %5.2f, %5.2f, %5.2f, %5.2f, %5.2f, %5.2f, %5.2f",*/ 
	//	gObj[aIndex].AccountID, gObj[aIndex].Name,

	//	gObj[aIndex].MasterCharacterInfo->IncInfiniteArrow
			/*,
		gObj[aIndex].MasterCharacterInfo->AttackSucRatePVP,
		gObj[aIndex].MasterCharacterInfo->DefenseSucRatePVP,
		gObj[aIndex].MasterCharacterInfo->Repair[0],
		gObj[aIndex].MasterCharacterInfo->Repair[1],
		gObj[aIndex].MasterCharacterInfo->Repair[2]
		gObj[aIndex].MasterCharacterInfo->Resistance2[0],
		gObj[aIndex].MasterCharacterInfo->Resistance2[1],
		gObj[aIndex].MasterCharacterInfo->Resistance2[2],
		gObj[aIndex].MasterCharacterInfo->AutoRegen*/
	//);
}

void cMastering2::ApplyPassiveSkills(int aIndex)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}
	if ( gObjIsConnectedGP(aIndex) == FALSE )
	{
		LogAddC(2, "error-L3 [%s][%d]", __FILE__, __LINE__);
		return;
	}

	int tmp = 0;

	if(gObj[aIndex].MasterCharacterInfo->Resistance2[R_POISON] > 0)
	{
		tmp = gObj[aIndex].m_AddResistance[R_POISON] * gObj[aIndex].MasterCharacterInfo->Resistance2[R_POISON];
		if(tmp >= 255)
			tmp = 255;
		gObj[aIndex].m_AddResistance[R_POISON] = tmp;
	}
	
	if(gObj[aIndex].MasterCharacterInfo->Resistance2[R_LIGHTNING] > 0)
	{
		tmp = gObj[aIndex].m_AddResistance[R_LIGHTNING] * gObj[aIndex].MasterCharacterInfo->Resistance2[R_LIGHTNING];
		if(tmp >= 255)
			tmp = 255;
		gObj[aIndex].m_AddResistance[R_LIGHTNING] = tmp;
	}
	
	if(gObj[aIndex].MasterCharacterInfo->Resistance2[R_ICE] > 0)
	{
		tmp = gObj[aIndex].m_AddResistance[R_ICE] * gObj[aIndex].MasterCharacterInfo->Resistance2[R_ICE];
		if(tmp >= 255)
			tmp = 255;
		gObj[aIndex].m_AddResistance[R_ICE] = tmp;
	}


	gObj[aIndex].iAddShield += gObj[aIndex].MasterCharacterInfo->IncMaxSD;
	gObj[aIndex].m_Defense += gObj[aIndex].MasterCharacterInfo->IncDefense;
	gObj[aIndex].AddLife += gObj[aIndex].MasterCharacterInfo->IncMaxHP;
	gObj[aIndex].AddMana += gObj[aIndex].MasterCharacterInfo->IncMaxMana;

	if(gObj[aIndex].Class == CLASS_KNIGHT || gObj[aIndex].Class == CLASS_MAGICGLADIATOR)
	{
		CItem * Right = &gObj[aIndex].pInventory[0];
		CItem * Left = &gObj[aIndex].pInventory[1];

		if(gObj[aIndex].MasterCharacterInfo->IncPhysicalAttackPower > 0)
		{
			gObj[aIndex].m_AttackDamageMinRight += gObj[aIndex].MasterCharacterInfo->IncPhysicalAttackPower;
			gObj[aIndex].m_AttackDamageMaxRight += gObj[aIndex].MasterCharacterInfo->IncPhysicalAttackPower;
			gObj[aIndex].m_AttackDamageMinLeft += gObj[aIndex].MasterCharacterInfo->IncPhysicalAttackPower;
			gObj[aIndex].m_AttackDamageMaxLeft += gObj[aIndex].MasterCharacterInfo->IncPhysicalAttackPower;

			gObj[aIndex].m_MagicDamageMin += gObj[aIndex].MasterCharacterInfo->IncPhysicalAttackPower;
			gObj[aIndex].m_MagicDamageMax += gObj[aIndex].MasterCharacterInfo->IncPhysicalAttackPower;
		}

		//If two Handed Weapon
		if ( Right->IsItem() == TRUE && Right->m_TwoHand == 1 )
		{
			if(Right->m_Type >= ITEMGET(0,0) && Right->m_Type < ITEMGET(1,0))
			{
				if(gObj[aIndex].MasterCharacterInfo->IncPhysicalAttackPowerTwoHand > 0)
				{
					gObj[aIndex].m_AttackDamageMinRight += gObj[aIndex].MasterCharacterInfo->IncPhysicalAttackPowerTwoHand / 2.0f;
					gObj[aIndex].m_AttackDamageMaxRight += gObj[aIndex].MasterCharacterInfo->IncPhysicalAttackPowerTwoHand / 2.0f;
					gObj[aIndex].m_AttackDamageMinLeft += gObj[aIndex].MasterCharacterInfo->IncPhysicalAttackPowerTwoHand / 2.0f;
					gObj[aIndex].m_AttackDamageMaxLeft += gObj[aIndex].MasterCharacterInfo->IncPhysicalAttackPowerTwoHand / 2.0f;

					gObj[aIndex].m_MagicDamageMin += gObj[aIndex].MasterCharacterInfo->IncPhysicalAttackPowerTwoHand;
					gObj[aIndex].m_MagicDamageMax += gObj[aIndex].MasterCharacterInfo->IncPhysicalAttackPowerTwoHand;
				}
			}
			else if((Right->m_Type >= ITEMGET(2,0)) && (Right->m_Type < ITEMGET(3,0)))
			{
				if(gObj[aIndex].MasterCharacterInfo->IncPhysicalAttackPowerMace > 0)
				{
					gObj[aIndex].m_AttackDamageMinRight += gObj[aIndex].MasterCharacterInfo->IncPhysicalAttackPowerMace;
					gObj[aIndex].m_AttackDamageMaxRight += gObj[aIndex].MasterCharacterInfo->IncPhysicalAttackPowerMace;
					gObj[aIndex].m_AttackDamageMinLeft += gObj[aIndex].MasterCharacterInfo->IncPhysicalAttackPowerMace;
					gObj[aIndex].m_AttackDamageMaxLeft += gObj[aIndex].MasterCharacterInfo->IncPhysicalAttackPowerMace;
				}
			}
			else if(Right->m_Type >= ITEMGET(3,0) && Right->m_Type < ITEMGET(4,0))
			{
				if(gObj[aIndex].MasterCharacterInfo->IncPhysicalAttackPowerSpear > 0)
				{
					gObj[aIndex].m_AttackDamageMinRight += gObj[aIndex].MasterCharacterInfo->IncPhysicalAttackPowerSpear;
					gObj[aIndex].m_AttackDamageMaxRight += gObj[aIndex].MasterCharacterInfo->IncPhysicalAttackPowerSpear;
					gObj[aIndex].m_AttackDamageMinLeft += gObj[aIndex].MasterCharacterInfo->IncPhysicalAttackPowerSpear;
					gObj[aIndex].m_AttackDamageMaxLeft += gObj[aIndex].MasterCharacterInfo->IncPhysicalAttackPowerSpear;
				}
			}
		} else {
			//If sword is in the Right hand
			if ( Right->IsItem() == TRUE )
			{
				if(gObj[aIndex].MasterCharacterInfo->IncAttackSpeedOneHand > 0)
					gObj[aIndex].m_AttackSpeed += (gObj[aIndex].MasterCharacterInfo->IncAttackSpeedOneHand);

				if ( (Right->m_Type >= ITEMGET(0,0) && Right->m_Type < ITEMGET(1,0)) && Right->m_TwoHand == 0)
				{
					if(gObj[aIndex].MasterCharacterInfo->IncPhysicalAttackPowerOneHand > 0)
					{
						gObj[aIndex].m_AttackDamageMinRight += (gObj[aIndex].MasterCharacterInfo->IncPhysicalAttackPowerOneHand /*/ 2.0f*/);
						gObj[aIndex].m_AttackDamageMaxRight += (gObj[aIndex].MasterCharacterInfo->IncPhysicalAttackPowerOneHand /*/ 2.0f*/);
					}
				} 
				else if((Right->m_Type >= ITEMGET(2,0)) && (Right->m_Type < ITEMGET(3,0)) && Right->m_TwoHand == 0)
				{
					if(gObj[aIndex].MasterCharacterInfo->IncPhysicalAttackPowerMace > 0)
					{
						gObj[aIndex].m_AttackDamageMinRight += gObj[aIndex].MasterCharacterInfo->IncPhysicalAttackPowerMace;
						gObj[aIndex].m_AttackDamageMaxRight += gObj[aIndex].MasterCharacterInfo->IncPhysicalAttackPowerMace;
					}
				}
				else if(Right->m_Type >= ITEMGET(3,0) && Right->m_Type < ITEMGET(4,0) && Right->m_TwoHand == 0)
				{
					if(gObj[aIndex].MasterCharacterInfo->IncPhysicalAttackPowerSpear > 0)
					{
						gObj[aIndex].m_AttackDamageMinRight += (gObj[aIndex].MasterCharacterInfo->IncPhysicalAttackPowerSpear);
						gObj[aIndex].m_AttackDamageMaxRight += (gObj[aIndex].MasterCharacterInfo->IncPhysicalAttackPowerSpear);
					}
				}
			}

			//If sword is in the LEFT hand
			if ( Left->IsItem() == TRUE )
			{
				if(gObj[aIndex].MasterCharacterInfo->IncAttackSpeedOneHand > 0)
					gObj[aIndex].m_AttackSpeed += (gObj[aIndex].MasterCharacterInfo->IncAttackSpeedOneHand);

				if ( (Left->m_Type >= ITEMGET(0,0) && Left->m_Type < ITEMGET(1,0)) && Left->m_TwoHand == 0)
				{
					if(gObj[aIndex].MasterCharacterInfo->IncPhysicalAttackPowerOneHand > 0)
					{
						gObj[aIndex].m_AttackDamageMinLeft += (gObj[aIndex].MasterCharacterInfo->IncPhysicalAttackPowerOneHand /*/ 2.0f*/);
						gObj[aIndex].m_AttackDamageMaxLeft += (gObj[aIndex].MasterCharacterInfo->IncPhysicalAttackPowerOneHand /*/ 2.0f*/);
					}
				} 
				else if((Left->m_Type >= ITEMGET(2,0)) && (Left->m_Type < ITEMGET(3,0)) && Left->m_TwoHand == 0)
				{
					if(gObj[aIndex].MasterCharacterInfo->IncPhysicalAttackPowerMace > 0)
					{
						gObj[aIndex].m_AttackDamageMinLeft += gObj[aIndex].MasterCharacterInfo->IncPhysicalAttackPowerMace;
						gObj[aIndex].m_AttackDamageMaxLeft += gObj[aIndex].MasterCharacterInfo->IncPhysicalAttackPowerMace;
					}
				}
				else if(Left->m_Type >= ITEMGET(3,0) && Left->m_Type < ITEMGET(4,0) && Left->m_TwoHand == 0)
				{
					if(gObj[aIndex].MasterCharacterInfo->IncPhysicalAttackPowerSpear > 0)
					{
						gObj[aIndex].m_AttackDamageMinLeft += (gObj[aIndex].MasterCharacterInfo->IncPhysicalAttackPowerSpear);
						gObj[aIndex].m_AttackDamageMaxLeft += (gObj[aIndex].MasterCharacterInfo->IncPhysicalAttackPowerSpear);
					}
				}
			}
		}
	}

	if(gObj[aIndex].Class == CLASS_WIZARD || gObj[aIndex].Class == CLASS_MAGICGLADIATOR)
	{
		CItem * Right = &gObj[aIndex].pInventory[0];
		CItem * Left = &gObj[aIndex].pInventory[1];

		if ( Right->IsItem() == TRUE && Right->m_TwoHand == 1)
		{
			if(Right->m_Type >= ITEMGET(5,0) && Right->m_Type < ITEMGET(6,0))
			{
				gObj[aIndex].m_MagicDamageMin += gObj[aIndex].MasterCharacterInfo->IncWizardyTwoHand;
				gObj[aIndex].m_MagicDamageMax += gObj[aIndex].MasterCharacterInfo->IncWizardyTwoHand;
			} 
		} 
		else
		{
				if ( Right->IsItem() == TRUE && Right->m_TwoHand == 0)
				{
					if(Right->m_Type >= ITEMGET(5,0) && Right->m_Type < ITEMGET(6,0))
					{
						gObj[aIndex].m_MagicDamageMin += gObj[aIndex].MasterCharacterInfo->IncWizardyOneHand;
						gObj[aIndex].m_MagicDamageMax += gObj[aIndex].MasterCharacterInfo->IncWizardyOneHand;
						gObj[aIndex].m_AttackSpeed += gObj[aIndex].MasterCharacterInfo->IncStaffSpeedOneHand;
					}
				}

				if ( Left->IsItem() == TRUE && Left->m_TwoHand == 0)
				{
					if(Left->m_Type >= ITEMGET(5,0) && Left->m_Type < ITEMGET(6,0))
					{
						gObj[aIndex].m_MagicDamageMin += gObj[aIndex].MasterCharacterInfo->IncWizardyOneHand;
						gObj[aIndex].m_MagicDamageMax += gObj[aIndex].MasterCharacterInfo->IncWizardyOneHand;
						gObj[aIndex].m_AttackSpeed += gObj[aIndex].MasterCharacterInfo->IncStaffSpeedOneHand;
					}
				}		
		}
	}

	if(gObj[aIndex].Class == CLASS_WIZARD || gObj[aIndex].Class == CLASS_ELF || gObj[aIndex].Class == CLASS_DARKLORD)
	{
		CItem * Left = &gObj[aIndex].pInventory[1];
		if ( Left->IsItem() == TRUE )
		{
			if(Left->m_Type >= ITEMGET(6,0) && Left->m_Type < ITEMGET(7,0))
				gObj[aIndex].m_Defense += gObj[aIndex].MasterCharacterInfo->IncDefenseShield;
		}	
	}

	if(gObj[aIndex].Class == CLASS_ELF)
	{
		CItem * Right = &gObj[aIndex].pInventory[0];
		CItem * Left = &gObj[aIndex].pInventory[1];
		if ( Left->IsItem() == TRUE )
		{
			if((Left->m_Type >= ITEMGET(4,0) && Left->m_Type < ITEMGET(5,0)) && Left->m_Type != ITEMGET(4,7))
			{
				gObj[aIndex].m_AttackSpeed += gObj[aIndex].MasterCharacterInfo->IncAttackSpeedOneHand;
				gObj[aIndex].m_AttackDamageMinRight += gObj[aIndex].MasterCharacterInfo->IncPhysicalAttackPowerMace;
				gObj[aIndex].m_AttackDamageMaxRight += gObj[aIndex].MasterCharacterInfo->IncPhysicalAttackPowerMace;
				gObj[aIndex].m_AttackDamageMinLeft += gObj[aIndex].MasterCharacterInfo->IncPhysicalAttackPowerMace;
				gObj[aIndex].m_AttackDamageMaxLeft += gObj[aIndex].MasterCharacterInfo->IncPhysicalAttackPowerMace;
			}
		}
		if ( Right->IsItem() == TRUE )
		{
			if((Right->m_Type >= ITEMGET(4,0) && Right->m_Type < ITEMGET(5,0)) && Right->m_Type != ITEMGET(4,15))
			{
				gObj[aIndex].m_AttackDamageMinRight += gObj[aIndex].MasterCharacterInfo->IncPhysicalAttackPowerSpear;
				gObj[aIndex].m_AttackDamageMaxRight += gObj[aIndex].MasterCharacterInfo->IncPhysicalAttackPowerSpear;
				gObj[aIndex].m_AttackDamageMinLeft += gObj[aIndex].MasterCharacterInfo->IncPhysicalAttackPowerSpear;
				gObj[aIndex].m_AttackDamageMaxLeft += gObj[aIndex].MasterCharacterInfo->IncPhysicalAttackPowerSpear;
			}
		}
	}
	if(gObj[aIndex].Class == CLASS_SUMMONER)
	{
		CItem * Right = &gObj[aIndex].pInventory[0];
		if ( Right->IsItem() == TRUE )
		{
			if(Right->m_Type >= ITEMGET(5,0) && Right->m_Type < ITEMGET(6,0))
			{
				gObj[aIndex].m_AttackSpeed += gObj[aIndex].MasterCharacterInfo->IncAttackSpeedOneHand;
				gObj[aIndex].m_MagicDamageMin += gObj[aIndex].MasterCharacterInfo->IncWizardyOneHand;
				gObj[aIndex].m_MagicDamageMax += gObj[aIndex].MasterCharacterInfo->IncWizardyOneHand;
			}
		}
		if(gObj[aIndex].m_SkillBerserkerTime > 0)
		{
			gObj[aIndex].m_AttackSpeed += (gObj[aIndex].m_AttackSpeed * gObj[aIndex].MasterCharacterInfo->IncBerserker2) / 100.0f;
			gObj[aIndex].m_MagicDamageMin += (gObj[aIndex].m_MagicDamageMin * gObj[aIndex].MasterCharacterInfo->IncBerserker2) / 100.0f;
			gObj[aIndex].m_MagicDamageMax += (gObj[aIndex].m_MagicDamageMin * gObj[aIndex].MasterCharacterInfo->IncBerserker2) / 100.0f;
		}
	}
	if(gObj[aIndex].Class == CLASS_DARKLORD)
	{
		if(gObj[aIndex].MasterCharacterInfo->IncPhysicalAttackPower > 0)
		{
			gObj[aIndex].m_AttackDamageMinRight += gObj[aIndex].MasterCharacterInfo->IncPhysicalAttackPower;
			gObj[aIndex].m_AttackDamageMaxRight += gObj[aIndex].MasterCharacterInfo->IncPhysicalAttackPower;
			gObj[aIndex].m_AttackDamageMinLeft += gObj[aIndex].MasterCharacterInfo->IncPhysicalAttackPower;
			gObj[aIndex].m_AttackDamageMaxLeft += gObj[aIndex].MasterCharacterInfo->IncPhysicalAttackPower;
			gObj[aIndex].m_MagicDamageMin += gObj[aIndex].MasterCharacterInfo->IncPhysicalAttackPower;
			gObj[aIndex].m_MagicDamageMax += gObj[aIndex].MasterCharacterInfo->IncPhysicalAttackPower;
		}
		
		if ( gObj[aIndex].pInventory[8].IsItem() != FALSE )
		{
			if ( gObj[aIndex].pInventory[8].m_Type == ITEMGET(13,4) && gObj[aIndex].pInventory[8].m_Durability > 0.0f )	// Dark Horse
			{
				gObj[aIndex].m_Defense += gObj[aIndex].MasterCharacterInfo->IncDefenseWDarkHorse;
			}
		}
		
		CItem * Right = &gObj[aIndex].pInventory[0];

		if ( Right->IsItem() == TRUE )
		{
			if(Right->m_Type >= ITEMGET(2,8) && Right->m_Type < ITEMGET(3,0))
			{
				if(gObj[aIndex].MasterCharacterInfo->IncPhysicalAttackPowerTwoHand > 0)
				{
					gObj[aIndex].m_AttackDamageMinRight += gObj[aIndex].MasterCharacterInfo->IncWizardyTwoHand;
					gObj[aIndex].m_AttackDamageMaxRight += gObj[aIndex].MasterCharacterInfo->IncWizardyTwoHand;
					gObj[aIndex].m_AttackDamageMinLeft += gObj[aIndex].MasterCharacterInfo->IncWizardyTwoHand;
					gObj[aIndex].m_AttackDamageMaxLeft += gObj[aIndex].MasterCharacterInfo->IncWizardyTwoHand;
					gObj[aIndex].m_MagicDamageMin += gObj[aIndex].MasterCharacterInfo->IncWizardyTwoHand;
					gObj[aIndex].m_MagicDamageMax += gObj[aIndex].MasterCharacterInfo->IncWizardyTwoHand;
				}
			}
		}
		if(gObj[aIndex].MasterCharacterInfo->IncScepterAdds2 >= 1)
		{
			int aDef = gObj[aIndex].Leadership / gObj[aIndex].MasterCharacterInfo->IncScepterAdds2;
			gObj[aIndex].m_Defense += aDef;
		}
	}
	if(gObj[aIndex].Class == CLASS_RAGEFIGHTER)
	{
		CItem * Right = &gObj[aIndex].pInventory[0];
		CItem * Left = &gObj[aIndex].pInventory[1];
		if ( Left->IsItem() == TRUE )
		{
			if((Left->m_Type >= ITEMGET(0,32) && Left->m_Type <= ITEMGET(0,35)))
			{
				gObj[aIndex].m_AttackDamageMinRight += (gObj[aIndex].MasterCharacterInfo->IncPhysicalAttackPowerMace / 2.0f);
				gObj[aIndex].m_AttackDamageMaxRight += (gObj[aIndex].MasterCharacterInfo->IncPhysicalAttackPowerMace / 2.0f);
				gObj[aIndex].m_AttackDamageMinLeft += (gObj[aIndex].MasterCharacterInfo->IncPhysicalAttackPowerMace / 2.0f);
				gObj[aIndex].m_AttackDamageMaxLeft += (gObj[aIndex].MasterCharacterInfo->IncPhysicalAttackPowerMace / 2.0f);
			}
		}
		if ( Right->IsItem() == TRUE )
		{
			if((Right->m_Type >= ITEMGET(0,32) && Right->m_Type <= ITEMGET(0,35)))
			{
				gObj[aIndex].m_AttackDamageMinRight += (gObj[aIndex].MasterCharacterInfo->IncPhysicalAttackPowerMace / 2.0f);
				gObj[aIndex].m_AttackDamageMaxRight += (gObj[aIndex].MasterCharacterInfo->IncPhysicalAttackPowerMace / 2.0f);
				gObj[aIndex].m_AttackDamageMinLeft += (gObj[aIndex].MasterCharacterInfo->IncPhysicalAttackPowerMace / 2.0f);
				gObj[aIndex].m_AttackDamageMaxLeft += (gObj[aIndex].MasterCharacterInfo->IncPhysicalAttackPowerMace / 2.0f);
			}
		}
	}
}


bool cMastering2::MagicReplace(int aIndex, WORD OldSkill, WORD NewSkill)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return false;
	}

	if(NewSkill >= 325)
	{
		for(int n = 0; n < MAGIC_SIZE; n++)
		{
			if(gObj[aIndex].Magic[n].IsMagic() == 1)
			{
				if(gObj[aIndex].Magic[n].m_Skill == OldSkill)
				{
					if(NewSkill > gObj[aIndex].Magic[n].m_MasterSkill)
					{
						if(gObj[aIndex].Magic[n].m_MasterSkill != 0)
						{
							OldSkill = gObj[aIndex].Magic[n].m_MasterSkill;
							gObj[aIndex].Magic[n].m_MasterSkill = NewSkill;
						}else
						{
							gObj[aIndex].Magic[n].m_MasterSkill = NewSkill;
						}
						if(NewSkill != 515 && NewSkill != 517 && NewSkill != 559)
						{
							GCMagicListOneDelSend(gObj[aIndex].m_Index,n,OldSkill,0xDC,0,0);
							GCMagicListOneSend(gObj[aIndex].m_Index,n,NewSkill,0xDC,0,0);
						}
						//gObj[aIndex].Magic[n].m_Skill = NewSkill;
						return true;
					}
					return true;
				}
			}
		}
		return false;
	}
	return true;
}


void cMastering2::GCMagicListMultiSendChanges(int aIndex,int Pos, WORD & Skill)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	if(ReadConfig.S6E2 == 1)
	{
		if(gObj[aIndex].Magic[Pos].m_MasterSkill > 325)
		{
			if(gObj[aIndex].Magic[Pos].m_MasterSkill == 515 || gObj[aIndex].Magic[Pos].m_MasterSkill == 517)
			{
				Skill = 511;
			}else if(gObj[aIndex].Magic[Pos].m_MasterSkill == 559)
			{
				Skill = 263;
			}else
				Skill = gObj[aIndex].Magic[Pos].m_MasterSkill;
		}
	}
}