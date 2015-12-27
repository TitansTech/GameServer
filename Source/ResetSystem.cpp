#include "stdafx.h"
#include "GameServer.h"
#include "LogProc.h"
#include "GameMain.h"
#include "TNotice.h"
#include "..\include\readscript.h"
//#include "GameServerAuth.h"
#include "LogToFile.h"
#include "..\common\winutil.h"
#include "ResetSystem.h"
#include "EDSProtocol.h"
#include "SCFExDBProtocol.h"
#include "VipSystem.h"

#if (DSGN_RESET_SYSTEM==0)
#if (PACK_EDITION>=2)

CLogToFile RESET_LOG("RESET_LOG", ".\\RESET_LOG", 1);
cResetSystem ResetChar;

struct FHP_FRIEND_MEMO_SEND
{
	PWMSG_HEAD h;
	short Number;	// 4
	DWORD WindowGuid;	// 8
	char Name[10];	// C
	char ToName[10];	// 16
	char Subject[32];	// 20
	BYTE Dir;	// 40
	BYTE Action;	// 41
	short MemoSize;	// 42
	BYTE Photo[18];	// 44
	char Memo[1000];	// 56
};

void cResetSystem::Init(char * FilePath, BYTE ResetType)
{
	ResetStruct ResetTmp;

	ResetTmp.Enabled			=GetPrivateProfileInt("Reset System", "SCFIsReset",0, FilePath) ;
	ResetTmp.EnabledWriteLog	=GetPrivateProfileInt("Reset System", "SCFIsResetLog",0, FilePath) ;
	ResetTmp.LimitCount			=GetPrivateProfileInt("Reset System", "SCFResetLimitCount",-1, FilePath) ;
#if (PACK_EDITION>=3)
	ResetTmp.EnabledDBLog		=GetPrivateProfileInt("Reset System", "SCFIsResetDBLog",0, FilePath) ;
#endif
	ResetTmp.Level				=GetPrivateProfileInt("Reset System", "SCFResetLevel",400, FilePath) ;
	ResetTmp.System				=GetPrivateProfileInt("Reset System", "SCFResetSystem",0, FilePath) ;
	ResetTmp.AddPoints			=GetPrivateProfileInt("Reset System", "SCFResetPoints",250, FilePath) ;
	ResetTmp.AddPCPoints		=GetPrivateProfileInt("Reset System", "SCFResetAddPCPoints",0, FilePath) ;
	ResetTmp.AddVIPPoints		=GetPrivateProfileInt("Reset System", "SCFResetAddVIPPoints",0, FilePath) ;
	ResetTmp.Zen	    		=GetPrivateProfileInt("Reset System", "SCFResetZen",2000000, FilePath) ;
	ResetTmp.ZenAutoIncrement	=GetPrivateProfileInt("Reset System", "SCFResetIsZenAutoIncrement",0, FilePath) ;
	ResetTmp.ClearLevelUpPoint	=GetPrivateProfileInt("Reset System", "SCFResetClearLevelUpPoint",1, FilePath) ;


	ResetTmp.MailSend			=GetPrivateProfileInt("Reset System", "SCFResetIsStringSend",0, FilePath) ;
	GetPrivateProfileString		("Reset System","SCFResetString","Guerrero largo fue el camino que recorriste para este momento, ahora te toca renacer, con %d zen y %d %s +%d+%d %s & %s podras lograrlo.",ResetTmp.MailString,sizeof(ResetTmp.MailString),FilePath);

	ResetTmp.ItemNeed			=GetPrivateProfileInt("Reset Items", "SCFResetIsNeedSpecialItem",0, FilePath) ;
	
	ResetTmp.ItemNeedType[0]	=GetPrivateProfileInt("Reset Items", "SCFDWResetNeedSpecialItemType",12, FilePath) ;
	ResetTmp.ItemNeedIndex[0]	=GetPrivateProfileInt("Reset Items", "SCFDWResetNeedSpecialItemIndex",15, FilePath) ;
	ResetTmp.ItemNeedLevel[0]	=GetPrivateProfileInt("Reset Items", "SCFDWResetNeedSpecialItemLevel",0, FilePath) ;
	ResetTmp.ItemNeedOpt[0]		=GetPrivateProfileInt("Reset Items", "SCFDWResetNeedSpecialItemOpt",0, FilePath) ;
	ResetTmp.ItemNeedLuck[0]	=GetPrivateProfileInt("Reset Items", "SCFDWResetNeedSpecialItemLuck",0, FilePath) ;
	ResetTmp.ItemNeedSkill[0]	=GetPrivateProfileInt("Reset Items", "SCFDWResetNeedSpecialItemSkill",0, FilePath) ;
	ResetTmp.ItemNeedCount[0]	=GetPrivateProfileInt("Reset Items", "SCFDWResetNeedSpecialItemCount",1, FilePath) ;

	ResetTmp.ItemNeedType[1]	=GetPrivateProfileInt("Reset Items", "SCFDKResetNeedSpecialItemType",12, FilePath) ;
	ResetTmp.ItemNeedIndex[1]	=GetPrivateProfileInt("Reset Items", "SCFDKResetNeedSpecialItemIndex",15, FilePath) ;
	ResetTmp.ItemNeedLevel[1]	=GetPrivateProfileInt("Reset Items", "SCFDKResetNeedSpecialItemLevel",0, FilePath) ;
	ResetTmp.ItemNeedOpt[1]		=GetPrivateProfileInt("Reset Items", "SCFDKResetNeedSpecialItemOpt",0, FilePath) ;
	ResetTmp.ItemNeedLuck[1]	=GetPrivateProfileInt("Reset Items", "SCFDKResetNeedSpecialItemLuck",0, FilePath) ;
	ResetTmp.ItemNeedSkill[1]	=GetPrivateProfileInt("Reset Items", "SCFDKResetNeedSpecialItemSkill",0, FilePath) ;
	ResetTmp.ItemNeedCount[1]	=GetPrivateProfileInt("Reset Items", "SCFDKResetNeedSpecialItemCount",1, FilePath) ;

	ResetTmp.ItemNeedType[2]	=GetPrivateProfileInt("Reset Items", "SCFElfResetNeedSpecialItemType",12, FilePath) ;
	ResetTmp.ItemNeedIndex[2]	=GetPrivateProfileInt("Reset Items", "SCFElfResetNeedSpecialItemIndex",15, FilePath) ;
	ResetTmp.ItemNeedLevel[2]	=GetPrivateProfileInt("Reset Items", "SCFElfResetNeedSpecialItemLevel",0, FilePath) ;
	ResetTmp.ItemNeedOpt[2]		=GetPrivateProfileInt("Reset Items", "SCFElfResetNeedSpecialItemOpt",0, FilePath) ;
	ResetTmp.ItemNeedLuck[2]	=GetPrivateProfileInt("Reset Items", "SCFElfResetNeedSpecialItemLuck",0, FilePath) ;
	ResetTmp.ItemNeedSkill[2]	=GetPrivateProfileInt("Reset Items", "SCFElfResetNeedSpecialItemSkill",0, FilePath) ;
	ResetTmp.ItemNeedCount[2]	=GetPrivateProfileInt("Reset Items", "SCFElfResetNeedSpecialItemCount",1, FilePath) ;

	ResetTmp.ItemNeedType[3]	=GetPrivateProfileInt("Reset Items", "SCFMGResetNeedSpecialItemType",12, FilePath) ;
	ResetTmp.ItemNeedIndex[3]	=GetPrivateProfileInt("Reset Items", "SCFMGResetNeedSpecialItemIndex",15, FilePath) ;
	ResetTmp.ItemNeedLevel[3]	=GetPrivateProfileInt("Reset Items", "SCFMGResetNeedSpecialItemLevel",0, FilePath) ;
	ResetTmp.ItemNeedOpt[3]		=GetPrivateProfileInt("Reset Items", "SCFMGResetNeedSpecialItemOpt",0, FilePath) ;
	ResetTmp.ItemNeedLuck[3]	=GetPrivateProfileInt("Reset Items", "SCFMGResetNeedSpecialItemLuck",0, FilePath) ;
	ResetTmp.ItemNeedSkill[3]	=GetPrivateProfileInt("Reset Items", "SCFMGResetNeedSpecialItemSkill",0, FilePath) ;
	ResetTmp.ItemNeedCount[3]	=GetPrivateProfileInt("Reset Items", "SCFMGResetNeedSpecialItemCount",1, FilePath) ;

	ResetTmp.ItemNeedType[4]	=GetPrivateProfileInt("Reset Items", "SCFDLResetNeedSpecialItemType",12, FilePath) ;
	ResetTmp.ItemNeedIndex[4]	=GetPrivateProfileInt("Reset Items", "SCFDLResetNeedSpecialItemIndex",15, FilePath) ;
	ResetTmp.ItemNeedLevel[4]	=GetPrivateProfileInt("Reset Items", "SCFDLResetNeedSpecialItemLevel",0, FilePath) ;
	ResetTmp.ItemNeedOpt[4]		=GetPrivateProfileInt("Reset Items", "SCFDLResetNeedSpecialItemOpt",0, FilePath) ;
	ResetTmp.ItemNeedLuck[4]	=GetPrivateProfileInt("Reset Items", "SCFDLResetNeedSpecialItemLuck",0, FilePath) ;
	ResetTmp.ItemNeedSkill[4]	=GetPrivateProfileInt("Reset Items", "SCFDLResetNeedSpecialItemSkill",0, FilePath) ;
	ResetTmp.ItemNeedCount[4]	=GetPrivateProfileInt("Reset Items", "SCFDLResetNeedSpecialItemCount",1, FilePath) ;

	ResetTmp.ItemNeedType[5]	=GetPrivateProfileInt("Reset Items", "SCFSUResetNeedSpecialItemType",12, FilePath) ;
	ResetTmp.ItemNeedIndex[5]	=GetPrivateProfileInt("Reset Items", "SCFSUResetNeedSpecialItemIndex",15, FilePath) ;
	ResetTmp.ItemNeedLevel[5]	=GetPrivateProfileInt("Reset Items", "SCFSUResetNeedSpecialItemLevel",0, FilePath) ;
	ResetTmp.ItemNeedOpt[5]		=GetPrivateProfileInt("Reset Items", "SCFSUResetNeedSpecialItemOpt",0, FilePath) ;
	ResetTmp.ItemNeedLuck[5]	=GetPrivateProfileInt("Reset Items", "SCFSUResetNeedSpecialItemLuck",0, FilePath) ;
	ResetTmp.ItemNeedSkill[5]	=GetPrivateProfileInt("Reset Items", "SCFSUResetNeedSpecialItemSkill",0, FilePath) ;
	ResetTmp.ItemNeedCount[5]	=GetPrivateProfileInt("Reset Items", "SCFSUResetNeedSpecialItemCount",1, FilePath) ;

	ResetTmp.ItemNeedType[5]	=GetPrivateProfileInt("Reset Items", "SCFRFResetNeedSpecialItemType",12, FilePath) ;
	ResetTmp.ItemNeedIndex[5]	=GetPrivateProfileInt("Reset Items", "SCFRFResetNeedSpecialItemIndex",15, FilePath) ;
	ResetTmp.ItemNeedLevel[5]	=GetPrivateProfileInt("Reset Items", "SCFRFResetNeedSpecialItemLevel",0, FilePath) ;
	ResetTmp.ItemNeedOpt[5]		=GetPrivateProfileInt("Reset Items", "SCFRFResetNeedSpecialItemOpt",0, FilePath) ;
	ResetTmp.ItemNeedLuck[5]	=GetPrivateProfileInt("Reset Items", "SCFRFResetNeedSpecialItemLuck",0, FilePath) ;
	ResetTmp.ItemNeedSkill[5]	=GetPrivateProfileInt("Reset Items", "SCFRFResetNeedSpecialItemSkill",0, FilePath) ;
	ResetTmp.ItemNeedCount[5]	=GetPrivateProfileInt("Reset Items", "SCFRFResetNeedSpecialItemCount",1, FilePath) ;

	ResetTmp.ClearInventory		=GetPrivateProfileInt("Reset System", "SCFResetClearInventory",0, FilePath) ;
	
	ResetTmp.ClearSkills		=GetPrivateProfileInt("Reset System", "SCFResetClearSkills",0, FilePath) ;
	ResetTmp.ClearStats			=GetPrivateProfileInt("Reset System", "SCFResetClearStats",0, FilePath) ;

	ResetTmp.System2Level[0]	=GetPrivateProfileInt("Reset SP System", "SCFResetSPLimit1",10, FilePath) ;
	ResetTmp.System2Level[1]	=GetPrivateProfileInt("Reset SP System", "SCFResetSPLimit2",20, FilePath) ;
	ResetTmp.System2Level[2]	=GetPrivateProfileInt("Reset SP System", "SCFResetSPLimit3",30, FilePath) ;
	ResetTmp.System2Level[3]	=GetPrivateProfileInt("Reset SP System", "SCFResetSPLimit4",40, FilePath) ;
	ResetTmp.System2Level[4]	=GetPrivateProfileInt("Reset SP System", "SCFResetSPLimit5",50, FilePath) ;
	ResetTmp.System2Level[5]	=GetPrivateProfileInt("Reset SP System", "SCFResetSPLimit6",60, FilePath) ;
	ResetTmp.System2Level[6]	=GetPrivateProfileInt("Reset SP System", "SCFResetSPLimit7",9999, FilePath) ;
	ResetTmp.System2Points[0]	=GetPrivateProfileInt("Reset SP System", "SCFResetSPPoints1",450, FilePath) ;
	ResetTmp.System2Points[1]	=GetPrivateProfileInt("Reset SP System", "SCFResetSPPoints2",400, FilePath) ;
	ResetTmp.System2Points[2]	=GetPrivateProfileInt("Reset SP System", "SCFResetSPPoints3",350, FilePath) ;
	ResetTmp.System2Points[3]	=GetPrivateProfileInt("Reset SP System", "SCFResetSPPoints4",300, FilePath) ;
	ResetTmp.System2Points[4]	=GetPrivateProfileInt("Reset SP System", "SCFResetSPPoints5",250, FilePath) ;
	ResetTmp.System2Points[5]	=GetPrivateProfileInt("Reset SP System", "SCFResetSPPoints6",200, FilePath) ;
	ResetTmp.System2Points[6]	=GetPrivateProfileInt("Reset SP System", "SCFResetSPPoints7",150, FilePath) ;
	switch(ResetType)
	{
		case 0:
		{
			ResetTmp.ClearResets		=FALSE;
			ResetTmp.ClearQuests		=GetPrivateProfileInt("Reset System", "SCFResetClearQuests",0, FilePath) ;
			ResetTmp.ClearEvolutions	=GetPrivateProfileInt("Reset System", "SCFResetClearEvolutions",0, FilePath) ;
			ResetTmp.ClearMasterPoints	=FALSE;
			this->Normal = ResetTmp;
		}break;
		case 1:
		{
			ResetTmp.ClearResets		=GetPrivateProfileInt("Reset System", "SCFResetClearPreviousResets",0, FilePath) ;
			ResetTmp.ClearMasterPoints  =GetPrivateProfileInt("Reset System", "SCFResetClearMasterPoints",0, FilePath) ;
			
			ResetTmp.ClearQuests		=FALSE;
			ResetTmp.ClearEvolutions	=FALSE;
			this->Masters = ResetTmp;
		}break;
	}
	LogAddTD("[SCF ResetSystem] - %s file is Loaded",FilePath);
}

void cResetSystem::CheckSendMail(LPOBJ lpObj)
{
	if ( lpObj->Type != OBJ_USER )
		return;

	ResetStruct ResetTmp;
	BOOL MasterReset = FALSE;
	
	if((gObjIsNewClass(lpObj) == 1) && (this->Masters.Enabled == 1))
	{
		ResetTmp = this->Masters;
		MasterReset = TRUE;
	}else
	{
		ResetTmp = this->Normal;
	}

	if(ResetTmp.MailSend == 1)
	{
		if( (MasterReset == FALSE && this->Normal.Enabled == 1) || (MasterReset == TRUE && this->Masters.Enabled == 1) )
		{
			if( ((ResetTmp.Level == (lpObj->Level - 1) ) && (MasterReset == FALSE)) || ((MasterReset == TRUE) && (ResetTmp.Level == (lpObj->MasterCharacterInfo->MasterLevel -1) )) )
			{
				this->SendMail(lpObj);
			}
		}
	}
}

void cResetSystem::SendMail(LPOBJ lpObj)
{
	if ( lpObj->Type != OBJ_USER )
		return;

	int aIndex = lpObj->m_Index;
	ResetStruct ResetTmp;

	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	if((gObjIsNewClass(lpObj) == 1) && (this->Masters.Enabled == 1))
	{
		ResetTmp = this->Masters;
	}else
	{
		ResetTmp = this->Normal;
	}

	if(ResetTmp.MailSend == 1)
	{
		FHP_FRIEND_MEMO_SEND pMsg;

		pMsg.WindowGuid = 0;
		pMsg.Number = lpObj->m_Index;
		pMsg.Dir = 143;
		pMsg.Action = 27;
		wsprintf(pMsg.Subject,"Reset Quest");
		memcpy(pMsg.Name, gObj[aIndex].Name, sizeof(pMsg.Name));
		memcpy(pMsg.Photo, gObj[aIndex].CharSet, sizeof(pMsg.Photo));
		memcpy(pMsg.ToName, gObj[aIndex].Name, sizeof(pMsg.ToName));
		
		int cClass = lpObj->Class;
		int Zen = 0;
		LPSTR Luck;
		LPSTR Skill;
		
		if(ResetTmp.ItemNeed == 1)
		{		
			int ItemNr = (ResetTmp.ItemNeedType[cClass] * 512) + ResetTmp.ItemNeedIndex[cClass];
			LPSTR ItemName = &ItemAttribute[ItemNr].Name[0];

			if(ResetTmp.ZenAutoIncrement == 0)
			{
				Zen = ResetTmp.Zen;
			}else
			{
				Zen = (lpObj->Resets + 1) * ResetTmp.Zen;
			}

			if(ResetTmp.ItemNeedLuck[cClass] == 1)
			{
				Luck = "Luck";
			}else
			{
				Luck = "No Luck";
			}

			if(ResetTmp.ItemNeedSkill[cClass] == 1)
			{
				Skill = "Skill";
			}else
			{
				Skill = "No Skill";
			}

			wsprintf(pMsg.Memo,ResetTmp.MailString,Zen,ResetTmp.ItemNeedCount[cClass],ItemName,ResetTmp.ItemNeedLevel[cClass],ResetTmp.ItemNeedOpt[cClass],Luck,Skill);
		}else
		{
			wsprintf(pMsg.Memo,ResetTmp.MailString);
		}

		pMsg.MemoSize = strlen(pMsg.Memo);
		int bsize = pMsg.MemoSize + sizeof(pMsg) - sizeof(pMsg.Memo);
		pMsg.h.set((LPBYTE)&pMsg, 0x70, bsize);
		wsExDbCli.DataSend((PCHAR)&pMsg, bsize);

		LogAdd("[%s] Reset Quest Mail Send", gObj[aIndex].Name);
	}
}

void cResetSystem::SetPoints(LPOBJ lpObj, ResetStruct ResetTmp, int ResetNum, int & LvlUpPoint)
{
	LvlUpPoint += (ResetTmp.System2Level[0] * ResetTmp.System2Points[0]);
	
	if(ResetNum > 0)
	{
		for(int i=1;i<ResetNum;i++)
		{
			LvlUpPoint += 	((ResetTmp.System2Level[i]-ResetTmp.System2Level[i-1]) * ResetTmp.System2Points[i]);
		}
		LvlUpPoint += 	(lpObj->Resets - ResetTmp.System2Level[ResetNum-1])*ResetTmp.System2Points[ResetNum];
	}
}

void cResetSystem::Start(LPOBJ lpObj)
{
	if ( lpObj->Type != OBJ_USER )
		return;

	ResetStruct ResetTmp;
	BOOL MasterReset = FALSE;
	int LvlUpPoint = 0;

	if((gObjIsNewClass(lpObj) == 1) && (this->Masters.Enabled == 1))
	{
		ResetTmp = this->Masters;
		MasterReset = TRUE;
	}else
	{
		ResetTmp = this->Normal;
	}

	if( (MasterReset == FALSE && this->Normal.Enabled == 1) || (MasterReset == TRUE && this->Masters.Enabled == 1) )
	{
		if(ResetTmp.LimitCount != -1)
		{
			if(lpObj->Resets >= ResetTmp.LimitCount)
			{
				GCServerMsgStringSend ("Max Reset Reached!!",lpObj->m_Index,0x01 );
				return;
			}
		}
		if((lpObj->Vip == 0) && (VipSystem.ResetFreeLimitEnabled == TRUE) && ((lpObj->Resets + 1) > VipSystem.ResetFreeLimitMax))
		{
			//Reset Max reached
			GCServerMsgStringSend ("You need to be VIP for continue reseting!",lpObj->m_Index,0x01 );
			return;
		}

		if (lpObj->PlayerExtraInfoReceived != 1 || lpObj->AccountExtraInfoReceived != 1 )
		{
			GCServerMsgStringSend ("ERROR: Please relogin!",lpObj->m_Index,0x01 ) ;
			LogAddC(2, "ERROR: RESET Info [%s][%s] Line: %d", lpObj->AccountID, lpObj->Name, __LINE__);
			return;
		}

		if( ((ResetTmp.Level <= lpObj->Level) && (MasterReset == FALSE)) || ((MasterReset == TRUE) && (ResetTmp.Level <= lpObj->MasterCharacterInfo->MasterLevel)) )
		{
			int ResetZen = ResetTmp.Zen;

			if(ResetTmp.ZenAutoIncrement == 1)
			{
				ResetZen = ResetZen * (lpObj->Resets + 1);
			}
			int MeZen = (lpObj->Money - ResetZen);
		
			if(MeZen >= 0)
			{
				int cClass = lpObj->Class;
				int item = ((ResetTmp.ItemNeedType[cClass] * 512) + ResetTmp.ItemNeedIndex[cClass]);
				
				if( ((ResetTmp.ItemNeed == 1) && (gObjCheckItemsCount(lpObj->m_Index,item,ResetTmp.ItemNeedLevel[cClass],ResetTmp.ItemNeedOpt[cClass],ResetTmp.ItemNeedLuck[cClass],ResetTmp.ItemNeedSkill[cClass]) >= ResetTmp.ItemNeedCount[cClass])) || (ResetTmp.ItemNeed == 0))
				{
					if(ResetTmp.ClearInventory == 1)
					{
						DeleteAllItems(lpObj->m_Index);
					}else
					{
						if(ResetTmp.ItemNeed == 1)
						{
							for(int i=0;i<ResetTmp.ItemNeedCount[cClass];i++)
								DeleteItemByMultipleValues(lpObj->m_Index,item,ResetTmp.ItemNeedLevel[cClass],ResetTmp.ItemNeedOpt[cClass],ResetTmp.ItemNeedLuck[cClass],ResetTmp.ItemNeedSkill[cClass]);
						}
					}
					lpObj->Money = MeZen;
					CGZenSend(lpObj->m_Index);	
					lpObj->Resets++;
					lpObj->PlayerExtraInfoModified = 1;

					if(ResetTmp.ClearLevelUpPoint == 1)
						lpObj->LevelUpPoint = 0;

					if(MasterReset == 1)
					{
						lpObj->MasterCharacterInfo->MasterLevel = 1;
						if(ResetTmp.ClearMasterPoints == 1)
							lpObj->MasterCharacterInfo->MasterPoints = 0;
					}else
					{
						lpObj->Level = 1;
						lpObj->MasterCharacterInfo->MasterLevel = 1;
					}

					if(ResetTmp.ClearStats == 1)
					{
						switch(lpObj->Class)
						{
							case 5:
							case 0:
							{
								lpObj->Strength = 18;
								lpObj->Dexterity = 18;
								lpObj->Vitality = 15;
								lpObj->Energy = 30;
							}break;
							case 1:
							{
								lpObj->Strength = 28;
								lpObj->Dexterity = 20;
								lpObj->Vitality = 25;
								lpObj->Energy = 10;
							}break;
							case 2:
							{
								lpObj->Strength = 22;
								lpObj->Dexterity = 25;
								lpObj->Vitality = 20;
								lpObj->Energy = 15;
							}break;
							case 3:
							{
								lpObj->Strength = 26;
								lpObj->Dexterity = 26;
								lpObj->Vitality = 26;
								lpObj->Energy = 26;
							}break;
							case 4:
							{
								lpObj->Strength = 26;
								lpObj->Dexterity = 20;
								lpObj->Vitality = 20;
								lpObj->Energy = 15;
								lpObj->Leadership = 15;
							}break;
							case 6:
							{
								lpObj->Strength = 32;
								lpObj->Dexterity = 27;
								lpObj->Vitality = 25;
								lpObj->Energy = 20;
							}break;
						}
					}
					if(ResetTmp.ClearEvolutions == 1)
					{
						switch(lpObj->Class)
						{
							case 0:
							{
								lpObj->DbClass = 0;
							}break;
							case 1:
							{
								lpObj->DbClass = 16;
							}break;
							case 2:
							{
								lpObj->DbClass = 32;
							}break;
							case 3:
							{
								lpObj->DbClass = 48;
							}break;
							case 4:
							{
								lpObj->DbClass = 64;
							}break;
							case 5:
							{
								lpObj->DbClass = 80;
							}break;
							case 6:
							{
								lpObj->DbClass = 96;
							}break;
						}			
					}
					if(ResetTmp.ClearSkills == 1)
					{
						gObjMagicDelAll(lpObj);
					}
					if(ResetTmp.ClearQuests == 1)
					{			
						memset(lpObj->m_Quest, (BYTE)-1, sizeof(lpObj->m_Quest));						
						lpObj->PlusStatQuestClear = 0;
						lpObj->ComboSkillquestClear = 0;
					}
					
					switch(ResetTmp.System)
					{
						case 0:
						{
							LvlUpPoint += (lpObj->Resets * ResetTmp.AddPoints);
						}break;
						case 1:
						{
							if(lpObj->Resets <= ResetTmp.System2Level[0])
							{
								LvlUpPoint += (lpObj->Resets * ResetTmp.System2Points[0]);
							}else if(lpObj->Resets > ResetTmp.System2Level[0] && lpObj->Resets <= ResetTmp.System2Level[1])
							{
								LvlUpPoint += ((ResetTmp.System2Points[0] * ResetTmp.System2Level[0]) + ( (lpObj->Resets - ResetTmp.System2Level[0]) * ResetTmp.System2Points[1]));
							}else if(lpObj->Resets > ResetTmp.System2Level[1] && lpObj->Resets <= ResetTmp.System2Level[2])
							{
								LvlUpPoint += ((ResetTmp.System2Level[0] * ResetTmp.System2Points[0]) + ((ResetTmp.System2Level[1]-ResetTmp.System2Level[0]) * ResetTmp.System2Points[1]) + ((lpObj->Resets - ResetTmp.System2Level[1])*ResetTmp.System2Points[2]));
							}else if(lpObj->Resets > ResetTmp.System2Level[2] && lpObj->Resets <= ResetTmp.System2Level[3])
							{
								this->SetPoints(lpObj,ResetTmp,3,LvlUpPoint);
							}else if(lpObj->Resets > ResetTmp.System2Level[3] && lpObj->Resets <= ResetTmp.System2Level[4])
							{
								this->SetPoints(lpObj,ResetTmp,4,LvlUpPoint);
							}else if(lpObj->Resets > ResetTmp.System2Level[4] && lpObj->Resets <= ResetTmp.System2Level[5])
							{
								this->SetPoints(lpObj,ResetTmp,5,LvlUpPoint);
							}else if(lpObj->Resets > ResetTmp.System2Level[5] && lpObj->Resets <= ResetTmp.System2Level[6])
							{
								this->SetPoints(lpObj,ResetTmp,6,LvlUpPoint);
							}else if(lpObj->Resets > ResetTmp.System2Level[6] && lpObj->Resets <= ResetTmp.System2Level[7])
							{
								this->SetPoints(lpObj,ResetTmp,7,LvlUpPoint);
							}
						}break;
						case 2:
						{
							LvlUpPoint += ResetTmp.AddPoints;
						}break;
					}
					
					if(ResetTmp.AddPCPoints > 0)
					{
						lpObj->PlayerExtraInfoModified = 1;
						lpObj->PCPoints += ResetTmp.AddPCPoints;
					}
					if(ResetTmp.AddVIPPoints > 0)
					{
						lpObj->AccountExtraInfoModified = 1;
						lpObj->VipMoney += ResetTmp.AddVIPPoints;
					}

					lpObj->LevelUpPoint += LvlUpPoint;

#if (PACK_EDITION>=3)
					if(ResetTmp.EnabledDBLog == 1)
						SetResetLog(lpObj->m_Index,MasterReset);
#endif
					GCServerMsgStringSend(lMsg.Get(MSGGET(14, 59)),lpObj->m_Index, 0x01);
					GCCloseMsgSend(lpObj->m_Index,2);

					LogAddTD("[ResetSytem][%s][%s] Character Reseted (%d)",lpObj->AccountID,lpObj->Name,lpObj->Resets);
					if(ResetTmp.EnabledWriteLog == 1)
						RESET_LOG.Output("[ResetSytem][%s][%s] Character Reseted (Reset Number: %d)",lpObj->AccountID,lpObj->Name,lpObj->Resets);
				
				}else
				{
					//Need Items
				GCServerMsgStringSend (lMsg.Get(MSGGET(14, 60)),lpObj->m_Index,0x01 );
				}
			}else
			{
				//Need More Zen
				GCServerMsgStringSend (lMsg.Get(MSGGET(14, 61)),lpObj->m_Index,0x01 );
			}
		}else
		{
			//Need More Levels
			GCServerMsgStringSend (lMsg.Get(MSGGET(14, 62)),lpObj->m_Index,0x01 );
		}
	}else
	{
		//Reset Dissabled
		GCServerMsgStringSend (lMsg.Get(MSGGET(14, 63)),lpObj->m_Index,0x01 );
	}
}
#endif	//PACK_EDITION
#else

#endif	//DSGN_RESET_SYSTEM