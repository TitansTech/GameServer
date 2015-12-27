#include "stdafx.h"
#include "GameServer.h"
#include "LogProc.h"
#include "GameMain.h"
#include "..\include\readscript.h"
#include "..\common\winutil.h"
#include "QuestS5Info.h"
#include "GensPvP.h"
#include "NpcTalk.h"
#include "DSProtocol.h"
#include "TUnionInfo.h"
#include "TUnion.h"
#include "GensCloseMap.h"

GensPvP Gens;

//static const struct CONTRIBUTION_RANK
//{
//	int ContMin;
//	int ContMax;
//
//} g_GensContributionNeed[6] = {
//
//	0,999,			//14
//	1000,4999,		//13
//	5000,14999,		//12
//	15000,49999,	//11
//	50000,99999,	//10
//	100000,9999999	//9
//}

struct CONTRIBUTION_RANK
{
	int ContMin;
	int ContMax;

};

CONTRIBUTION_RANK	g_GensContributionNeed[6];

struct PMSG_GENSREPLY
{
	PBMSG_HEAD h;	// C1:F8
	BYTE subcode;
	BYTE state;
};

struct PMSG_GENSRANKSEND
{
	PBMSG_HEAD h;	// C1:F8:07
	BYTE subcode;
	BYTE Family;
	BYTE Unk1[3];
	DWORD Ranking;
	BYTE Level;
	BYTE Unk2[3];
	DWORD Contribution;
	DWORD ContNeedNextLevel;
};

void GensPvP::LoadCfg(char * FilePath)
{
	this->Enabled = GetPrivateProfileInt("Common", "SCFGensSystemEnabled",0, FilePath) ;
	this->MinLevel = GetPrivateProfileInt("Common", "SCFGensMinLevel",50, FilePath);

	this->MinReset								= GetPrivateProfileInt("Common", "SCFGensMinReset",0, FilePath) ;
	this->AllowJoinInParty						= GetPrivateProfileInt("Common", "SCFGensAllowJoinInParty",0, FilePath) ;
	this->AllowJoinInGuild						= GetPrivateProfileInt("Common", "SCFGensAllowJoinInGuild",0, FilePath) ;
	this->AllowJoinIfGuildMemberIsInOtherGens	= GetPrivateProfileInt("Common", "SCFGensAllowJoinIfGuildMemberIsInOtherGens",0, FilePath) ;
	this->AllowJoinInGuildAlliance				= GetPrivateProfileInt("Common", "SCFGensAllowJoinInGuildAlliance",0, FilePath) ;
	this->AllowJoinIfLeftLessThan3Days			= GetPrivateProfileInt("Common", "SCFGensAllowJoinIfLeftLessThan3Days",0, FilePath) ;
	this->JoinGuildIntoGensWhenYouAreGM			= GetPrivateProfileInt("Common", "SCFGensJoinGuildIntoGensWhenYouAreGM",1, FilePath) ;
	this->AllowLeaveGM							= GetPrivateProfileInt("Common", "SCFGensAllowLeaveGM",0, FilePath) ;
	this->AllowPartyWithOtherGens				= GetPrivateProfileInt("Common", "SCFGensAllowPartyWithOtherGens",0, FilePath) ;
	this->AllowDuelWithSameGensFam				= GetPrivateProfileInt("Common", "SCFGensAllowDuelWithSameGens",0, FilePath) ;
	this->AllowWarWithSameGensFam				= GetPrivateProfileInt("Common", "SCFGensAllowWarWithSameGens",0, FilePath);
	this->AllowGuildAllianceWithOtherGensFam	= GetPrivateProfileInt("Common", "SCFGensAllowGuildAllianceWithOtherGens",0, FilePath);
	this->WinPointsIfRivalDoesntHavePoints		= GetPrivateProfileInt("Common", "SCFGensWinPointsIfRivalDoesntHavePoints",1, FilePath);

#if (PACK_EDITION>=3)
	GensCloseMap.Init(FilePath);
#endif
}

void GensPvP::LoadOtherCfg(char* filename )
{
	int Token;
	int iCount = 0;
	this->KillLessCount=0;
	this->KillMoreCount=0;

	g_GensContributionNeed[0].ContMin=0;
	g_GensContributionNeed[0].ContMax=999;
	g_GensContributionNeed[1].ContMin=1000;
	g_GensContributionNeed[1].ContMax=4999;
	g_GensContributionNeed[2].ContMin=5000;
	g_GensContributionNeed[2].ContMax=14999;
	g_GensContributionNeed[3].ContMin=15000;
	g_GensContributionNeed[3].ContMax=49999;
	g_GensContributionNeed[4].ContMin=50000;
	g_GensContributionNeed[4].ContMax=99999;
	g_GensContributionNeed[5].ContMin=100000;
	g_GensContributionNeed[5].ContMax=9999999;


	SMDFile = fopen(filename, "r");

	if ( SMDFile == NULL )
	{
		MsgBox("GensPvP data load error %s", filename);
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
					this->KillLess[this->KillLessCount].MinLevel = TokenNumber;
					Token = GetToken();
					this->KillLess[this->KillLessCount].MaxLevel = TokenNumber;
					Token = GetToken();
					this->KillLess[this->KillLessCount].ContKill = TokenNumber;
					Token = GetToken();
					this->KillLess[this->KillLessCount].ContKilled = TokenNumber;

					this->KillLessCount++;
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
					this->KillMore[this->KillMoreCount].MinLevel = TokenNumber;
					Token = GetToken();
					this->KillMore[this->KillMoreCount].MaxLevel = TokenNumber;
					Token = GetToken();
					this->KillMore[this->KillMoreCount].ContKill = TokenNumber;
					Token = GetToken();
					this->KillMore[this->KillMoreCount].ContKilled = TokenNumber;

					this->KillMoreCount++;
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
					if(iCount > 8)
						break;
					this->rItem[iCount].Type = TokenNumber;
					Token = GetToken();
					this->rItem[iCount].Index = TokenNumber;
					Token = GetToken();
					this->rItem[iCount].Level = TokenNumber;
					Token = GetToken();
					this->rItem[iCount].Opt = TokenNumber;
					Token = GetToken();
					this->rItem[iCount].Luck = TokenNumber;
					Token = GetToken();
					this->rItem[iCount].Skill = TokenNumber;
					Token = GetToken();
					this->rItem[iCount].Dur = TokenNumber;
					Token = GetToken();
					this->rItem[iCount].Exc = TokenNumber;
					Token = GetToken();
					this->rItem[iCount].Count = TokenNumber;

					iCount++;
				}
			}
			iType = GetToken();
			iType = TokenNumber;
			if ( iType == 4 )
			{
				int CountC=0;
				while(true)
				{
					Token = GetToken();
					if ( strcmp("end", TokenString) == 0 )
					{
						break;
					}
					if(CountC > 6)
						break;
					g_GensContributionNeed[CountC].ContMin = TokenNumber;
					Token = GetToken();
					g_GensContributionNeed[CountC].ContMax = TokenNumber;

					CountC++;
				}
			}
		break;
	}
		
	fclose(SMDFile);
	return;
}


void GensPvP::ProtocolCore(int aIndex,char * lpMsg)
{
	if(this->Enabled == 1)
	{
		if ( OBJMAX_RANGE(aIndex) == FALSE )
		{
			return;
		}

		PMSG_DEFAULT2 * aRecv = (PMSG_DEFAULT2 *)(lpMsg);
		switch(aRecv->subcode)
		{
			case 0x01://Join
			{
				this->JoinGens(aIndex);
			}break;
			case 0x03://Leave
			{
				this->LeaveGens(aIndex);
			}break;
			case 0x09://Reward Rank
			{
				this->RewardRank(aIndex);
			}break;
		}
	}
}

void GensPvP::JoinGens(int aIndex)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	PMSG_GENSREPLY pResult = {0};
	PHeadSubSetB((LPBYTE)&pResult,0xF8,0x02,sizeof(PMSG_GENSREPLY));

	LPOBJ lpObj = &gObj[aIndex];
	int Family = 0;
	if(lpObj->m_IfState.type >= 100 && lpObj->m_IfState.type <= 101)
	{
		Family = lpObj->m_IfState.type - 99;
	}else
	{
		//error
		return;
	}
	
	if (lpObj->PlayerExtraInfoReceived != 1)
	{
		GCServerMsgStringSend ("ERROR: Please relogin!",lpObj->m_Index,0x01 ) ;
		LogAddC(2, "ERROR: GENS Info [%s][%s] Line: %d", lpObj->AccountID, lpObj->Name, __LINE__);
		return;
	}

	if(lpObj->GensFam != 0)
	{
		//U r already in a family
		pResult.state = 1;
		goto DSEND;
	}
	if(lpObj->GensNotAllow == 1 && this->AllowJoinIfLeftLessThan3Days == 0)
	{
		//You can not join coz need to pass 3 days before join again
		pResult.state = 2;
		goto DSEND;
	}
	if(lpObj->Level < this->MinLevel)
	{
		//Need X Level to Join in Gens
		pResult.state = 3;
		goto DSEND;
	}

	if(lpObj->Resets < this->MinReset)
	{
		//Need X Level to Join in Gens
		char sbuf[100]={0};
		wsprintf(sbuf,"You need %d Resets before join Gens",this->MinReset);
		GCServerMsgStringSend(sbuf,lpObj->m_Index, 0x01);
		pResult.state = 3;
		goto DSEND;
	}

	if(lpObj->PartyNumber >= 0 && this->AllowJoinInParty == 0)
	{
		//Cant Join if you are in Party
		pResult.state = 6;
		goto DSEND;
	}
	
	if(lpObj->GuildNumber > 0)
	{			
		
		//DWORD dwGuildNumber = lpObj->GuildNumber;
		//_GUILD_INFO_STRUCT * lpGuildInfo = Guild.SearchGuild_Number(dwGuildNumber);

		if(lpObj->lpGuild != NULL)
		{
			if(lpObj->lpGuild->GensFam != Family && lpObj->lpGuild->GensFam != 0 && this->AllowJoinIfGuildMemberIsInOtherGens == 0)
			{
				//Your GM is in other family
				pResult.state = 4;
				goto DSEND;
			}else if(lpObj->lpGuild->GensFam == 0 && ( strcmp(lpObj->Name, lpObj->lpGuild->Names[0] ) ) && this->AllowJoinInGuild == 0)
			{
				//Your GM must be in gens before join you
				pResult.state = 5;
				goto DSEND;
			}else
			{
				TUnionInfo * pUnionInfo = UnionManager.SearchUnion(lpObj->lpGuild->iGuildUnion);

				if ( pUnionInfo && this->AllowJoinInGuildAlliance == 0)
				{
					//Cant join if you are in guild alliance
					pResult.state = 7;
					goto DSEND;
				}else
				{	
					if(this->AllowJoinIfGuildMemberIsInOtherGens == 0)
					{
						for ( int n=0;n<MAX_USER_GUILD;n++)
						{
							if ( lpObj->lpGuild->Use[n] == TRUE )
							{
								int gpIndex = gObjGetIndex(lpObj->lpGuild->Names[n]);
								if ( gpIndex != -1 )
								{
									if(gObj[gpIndex].GensFam != Family && gObj[gpIndex].GensFam != 0)
									{
										//Your guild is part of a different gens
										pResult.state = 4;
										goto DSEND;
									}
								}
							}
						}
					}
				}
			}
		}
	}

	//All Ok Join in GENS Family
	pResult.state = 0;

	lpObj->GensFam = Family;
	lpObj->GensCont = 0;
	lpObj->RecvRewItem = 0;
	lpObj->GensLevel = 14;

	gObj[aIndex].PlayerExtraInfoModified = 1;
	gObjViewportListProtocolCreate(&gObj[aIndex]);
	LogAddTD("[GensPvP] [%s][%s] Join Gens Family", lpObj->AccountID, lpObj->Name);

	if(lpObj->GuildNumber > 0)
	{			
		if(lpObj->lpGuild != NULL)
		{
			if ( strcmp(lpObj->lpGuild->Names[0], lpObj->Name) == 0) // Case Guild Master
			{
				lpObj->lpGuild->GensFam = Family;
				if(this->JoinGuildIntoGensWhenYouAreGM == 1)
				{
					for ( int n=0;n<MAX_USER_GUILD;n++)
					{
						if ( lpObj->lpGuild->Use[n] == TRUE )
						{
							int gpIndex = gObjGetIndex(lpObj->lpGuild->Names[n]);
							if ( gpIndex != -1 && gpIndex != aIndex )
							{
								if(gObj[gpIndex].GensFam == 0)
								{
									if (gObj[gpIndex].PlayerExtraInfoReceived != 1)
									{
										GCServerMsgStringSend ("ERROR: Please relogin!",gpIndex,0x01 ) ;
										LogAddC(2, "ERROR: GENS Info [%s][%s] Line: %d", lpObj->AccountID, lpObj->Name, __LINE__);
										continue;
									}
									gObj[gpIndex].PlayerExtraInfoModified = 1;
									gObj[gpIndex].RecvRewItem = 0;
									gObj[gpIndex].GensFam = Family;
									gObj[gpIndex].GensCont = 0;
									gObj[gpIndex].GensLevel = 14;
									this->Ranking(gpIndex);
									gObjViewportListProtocolCreate(&gObj[gpIndex]);
									LogAddTD("[GensPvP] [%s][%s] Leave Gens Family", gObj[gpIndex].AccountID, gObj[gpIndex].Name);
								}
							}
						}
					}
				}
			}
		}
	}
	
	DataSend(aIndex,(UCHAR *)&pResult, pResult.h.size);
	this->Ranking(aIndex);
	return;

DSEND:
	DataSend(aIndex,(UCHAR *)&pResult, pResult.h.size);
}

void GensPvP::LeaveGens(int aIndex)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	PMSG_GENSREPLY pResult = {0};
	PHeadSubSetB((LPBYTE)&pResult,0xF8,4,sizeof(PMSG_GENSREPLY));

	LPOBJ lpObj = &gObj[aIndex];
	int Family = 0;
	if(lpObj->m_IfState.type >= 100 && lpObj->m_IfState.type <= 101)
	{
		Family = lpObj->m_IfState.type - 99;
	}else
	{
		//error
		return;
	}

	if (lpObj->PlayerExtraInfoReceived != 1)
	{
		GCServerMsgStringSend ("ERROR: Please relogin!",lpObj->m_Index,0x01 ) ;
		LogAddC(2, "ERROR: GENS Info [%s][%s] Line: %d", lpObj->AccountID, lpObj->Name, __LINE__);
		return;
	}

	if(lpObj->GensFam == 0)
	{
		//U Must Have a Gens fam before leave it
		pResult.state = 1;
		goto DSEND;
	}

	if(lpObj->GensFam != Family)
	{
		//U r member of different fam
		pResult.state = 3;
		goto DSEND;
	}

	if(lpObj->GuildNumber > 0)
	{		
		if(lpObj->lpGuild != NULL)
		{
			if ( strcmp(gObj[aIndex].lpGuild->Names[0], gObj[aIndex].Name) == 0) // Case Guild Master
			{
				if(this->AllowLeaveGM == 0)
				{
					//GMs Cant leave Gens Family
					pResult.state = 2;
					goto DSEND;
				}else
				{
					lpObj->lpGuild->GensFam = 0;
				}
			}
		}
	}
	//All Ok Leave in GENS Family

	LogAddTD("[GensPvP] [%s][%s] Leave Gens Family", lpObj->AccountID, lpObj->Name);

	pResult.state = 0;

	lpObj->GensFam = 0;
	lpObj->GensCont = 0;
	lpObj->GensLevel = 0;
	if(this->AllowJoinIfLeftLessThan3Days == 0)
		lpObj->GensNotAllow = 1;
	lpObj->RecvRewItem = 0;


	DataSend(aIndex,(UCHAR *)&pResult, pResult.h.size);

	qs5.RemoveGensQuest(aIndex);
	this->Ranking(aIndex);
	gObjViewportListProtocolCreate(lpObj);
	gObj[aIndex].PlayerExtraInfoModified = 1;
	return;

DSEND:
	DataSend(aIndex,(UCHAR *)&pResult, pResult.h.size);
}

void GensPvP::Class(int aIndex)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];
	
	if(lpObj->GensFam > 0)
	{

		int cont = lpObj->GensCont;

		if(cont >= g_GensContributionNeed[0].ContMin && cont <= g_GensContributionNeed[0].ContMax)
		{
			lpObj->GensLevel = 14;
		}else if(cont >= g_GensContributionNeed[1].ContMin && cont <= g_GensContributionNeed[1].ContMax)
		{
			lpObj->GensLevel = 13;
		}else if(cont >= g_GensContributionNeed[2].ContMin && cont <= g_GensContributionNeed[2].ContMax)
		{
			lpObj->GensLevel = 12;
		}else if(cont >= g_GensContributionNeed[3].ContMin && cont <= g_GensContributionNeed[3].ContMax)
		{
			lpObj->GensLevel = 11;
		}else if(cont >= g_GensContributionNeed[4].ContMin && cont <= g_GensContributionNeed[4].ContMax)
		{
			lpObj->GensLevel = 10;
		}else if(cont >= g_GensContributionNeed[5].ContMin)
		{
			if(lpObj->GensRank <= 8)
			{
				SYSTEMTIME sysTime;
				GetLocalTime(&sysTime);
				if(sysTime.wDay >= 1 && sysTime.wDay <= 7)
				{
					lpObj->GensLevel = lpObj->GensRank;
				}
			}else
			{
				lpObj->GensLevel = 9;
			}
		}
	}
}

void GensPvP::Ranking(int aIndex)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];
	PMSG_GENSRANKSEND pResult = {0};
	PHeadSubSetB((LPBYTE)&pResult,0xF8,0x07,sizeof(PMSG_GENSRANKSEND));
	
	if(lpObj->GensFam > 0)
	{		
		this->Class(aIndex);
		int PointNeed = 0;
		if(lpObj->GensLevel > 9)
		{
			int subcode = 14 - lpObj->GensLevel;
			if(subcode <= 5)
			{
				PointNeed = (g_GensContributionNeed[subcode].ContMax - lpObj->GensCont) + 1;
			}
		}
		pResult.Contribution = lpObj->GensCont;
		pResult.ContNeedNextLevel = PointNeed;
		pResult.Family = lpObj->GensFam;
		pResult.Level = lpObj->GensLevel;
		pResult.Ranking = lpObj->GensRank;
	}
	DataSend(aIndex,(UCHAR *)&pResult, pResult.h.size);
}

bool GensPvP::CheckInvFree(int aIndex)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return false;
	}

	BYTE Num = gObj[aIndex].GensLevel -1;
	//BYTE TempInventoryMap[BASIC_INVENTORY_SIZE + (MAX_EXTENDED_INV*EXTENDED_INV_SIZE)];
	BYTE TempInventoryMap[MAIN_INVENTORY_NORMAL_SIZE + (MAX_EXTENDED_INV*EXTENDED_INV_SIZE) + PSHOP_SIZE];
	memcpy(TempInventoryMap,gObj[aIndex].pInventoryMap,ReadConfig.GETSLOTS(aIndex));

	for(int x=0;x<this->rItem[Num].Count;x++)
	{
		int item = ITEMGET(this->rItem[Num].Type,this->rItem[Num].Index);
		if(gObjTempInventoryInsertItem(&gObj[aIndex],item,TempInventoryMap) == 255)
		{
			return false;
		}
	}
	return true;
}

void GensPvP::RewardRank(int aIndex)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	PMSG_GENSREPLY pResult = {0};
	PHeadSubSetB((LPBYTE)&pResult,0xF8,0x0A,sizeof(PMSG_GENSREPLY));
	LPOBJ lpObj = &gObj[aIndex];
	int Family = 0;

	if(lpObj->m_IfState.type >= 100 && lpObj->m_IfState.type <= 101)
	{
		Family = lpObj->m_IfState.type - 99;
	}else
	{
		//error
		return;
	}

	if (lpObj->PlayerExtraInfoReceived != 1)
	{
		GCServerMsgStringSend ("ERROR: Please relogin!",lpObj->m_Index,0x01 ) ;
		LogAddC(2, "ERROR: GENS Info [%s][%s] Line: %d", lpObj->AccountID, lpObj->Name, __LINE__);
		return;
	}

	//BYTE TempInventoryMap[BASIC_INVENTORY_SIZE + (MAX_EXTENDED_INV*EXTENDED_INV_SIZE)];
	//memcpy(TempInventoryMap,gObj[aIndex].pInventoryMap,ReadConfig.GETSLOTS(aIndex));

	this->Ranking(aIndex);
	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);
	if(sysTime.wDay >= 7)
	{
		//Gens ranking rewards are given out in the first week of each month
		pResult.state = 1;
		goto DSEND;
	}
	if(gObj[aIndex].GensLevel > 8)
	{
		//Regrettably, you are not eligible for a reward. Please try harder next time
		pResult.state = 2;
		goto DSEND;
	}

	if(lpObj->RecvRewItem != 0)
	{
		//The reward has already been handed out
		pResult.state = 4;
		goto DSEND;
	}

	if(lpObj->GensFam != Family)
	{
		//U r member of different fam
		pResult.state = 5;
		goto DSEND;
	}
	if(lpObj->GensFam == 0)
	{
		//U r not joined the gens
		pResult.state = 6;
		goto DSEND;
	}

	if(this->CheckInvFree(aIndex) == false)
	{
		//You do not have enough space for the item. Please free up your inventory and try again.
		pResult.state = 3;
		goto DSEND;
	}

	pResult.state = 0;

	BYTE Num = gObj[aIndex].GensLevel -1;
	for(int x=0;x<this->rItem[Num].Count;x++)
	{
		int item = ITEMGET(this->rItem[Num].Type,this->rItem[Num].Index);
		ItemSerialCreateSend(aIndex, 235, 0, 0, item, this->rItem[Num].Level, this->rItem[Num].Dur, this->rItem[Num].Skill, this->rItem[Num].Luck, this->rItem[Num].Opt, -1, this->rItem[Num].Exc, 0);
	}
	lpObj->RecvRewItem = 1;
	gObj[aIndex].PlayerExtraInfoModified = 1;
	LogAddTD("[GensPvP] [%s][%s] Get Rank Reward", lpObj->AccountID, lpObj->Name);

DSEND:
	DataSend(aIndex,(UCHAR *)&pResult, pResult.h.size);
}

bool GensPvP::KillPlayer(int aIndex, int tIndex)
{
	if ( OBJMAX_RANGE(aIndex) == FALSE || OBJMAX_RANGE(tIndex) == FALSE)
	{
		return false;
	}
	if ( gObj[aIndex].Type != OBJ_USER || gObj[aIndex].Connected <= PLAYER_LOGGED )
	{
		return false;
	}
	if ( gObj[tIndex].Type != OBJ_USER || gObj[tIndex].Connected <= PLAYER_LOGGED )
	{
		return false;
	}
	if((gObj[aIndex].GensFam != 0 && gObj[tIndex].GensFam != 0) && (gObj[aIndex].GensFam != gObj[tIndex].GensFam))
	{
		int myLevel = gObj[aIndex].Level + gObj[aIndex].MasterCharacterInfo->MasterLevel;
		int tLevel = gObj[tIndex].Level + gObj[tIndex].MasterCharacterInfo->MasterLevel;
		if(myLevel >= tLevel)
		{
			int Dif = myLevel - tLevel;
			//Less
			for(int i=0;i<this->KillLessCount;i++)
			{
				if(Dif >= this->KillLess[i].MinLevel && Dif <= this->KillLess[i].MaxLevel)
				{
					if(this->WinPointsIfRivalDoesntHavePoints == 0 && gObj[tIndex].GensCont <=0)
					{
					}else
					{
						gObj[aIndex].GensCont += this->KillLess[i].ContKill;
					}
				}				
			}
			for(int i=0;i<this->KillMoreCount;i++)
			{
				if(Dif >= this->KillMore[i].MinLevel && Dif <= this->KillMore[i].MaxLevel)
				{
					gObj[tIndex].GensCont -= this->KillMore[i].ContKill;
				}				
			}
		}else
		{
			int Dif = tLevel - myLevel;
			//More
			for(int i=0;i<this->KillMoreCount;i++)
			{
				if(Dif >= this->KillMore[i].MinLevel && Dif <= this->KillMore[i].MaxLevel)
				{
					if(this->WinPointsIfRivalDoesntHavePoints == 0 && gObj[tIndex].GensCont <=0)
					{
					}else
					{
						gObj[aIndex].GensCont += this->KillMore[i].ContKill;
					}
				}				
			}
			for(int i=0;i<this->KillLessCount;i++)
			{
				if(Dif >= this->KillLess[i].MinLevel && Dif <= this->KillLess[i].MaxLevel)
				{
					gObj[tIndex].GensCont -= this->KillLess[i].ContKill;
				}				
			}
		}
		if(gObj[aIndex].GensCont < 0)
			gObj[aIndex].GensCont = 0;
		if(gObj[tIndex].GensCont < 0)
			gObj[tIndex].GensCont = 0;
		this->Ranking(aIndex);
		this->Ranking(tIndex);

		qs5.KillPlayerGensEnemy(aIndex);
#if (PACK_EDITION>=3)
		GensCloseMap.AddKills(aIndex);
#endif
		gObj[aIndex].PlayerExtraInfoModified = 1;
		gObj[tIndex].PlayerExtraInfoModified = 1;
		return true;
	}
	return false;
}