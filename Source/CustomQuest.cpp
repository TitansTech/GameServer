#include "stdafx.h"
#include "GameServer.h"
#include "LogProc.h"
#include "GameMain.h"
#include "TNotice.h"
#include "..\include\readscript.h"
//#include "GameServerAuth.h"
#include "LogToFile.h"
#include "..\common\winutil.h"
#include "EDSProtocol.h"
#include "DSProtocol.h"
#include "CustomQuest.h"
#include "ObjCalCharacter.h"
#include "Mastering.h"

#if (PACK_EDITION>=2)

cCQuest customQuest;

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


class cCQScript
{
public:
	cCQScript();
	virtual ~cCQScript();

	char ITEM_TYPE[20];
	char ITEM_INDEX[20];
	char ITEM_LEVEL[20];
	char ITEM_OPT[20];
	char ITEM_LUCK[20];
	char ITEM_SKILL[20];
	char ITEM_DUR[20];
	char ITEM_EXC[20];
	char ITEM_COUNT[20];
	char ZEN[20];
	char MASTERLEVEL[20];
	char LEVEL[20];
	char CLASS[20];
	char RESET[20];
	char QUEST[20];
	char MONSTER_KILL[20];
	char LEVELUP[20];
	char LEVELUPPOINT[20];
	char MASTERPOINT[20];
	char IF_FINISH[20];
	char ELSE_FINISH[20];
	char END_FINISH[20];
	char BEGIN_QUEST[20];
	char END_QUEST[20];
	char BEGIN_STATE[20];
	char END_STATE[20];
	char TITLE[20];
	char TEXT[20];
	char CMD_ACCEPT[20];
	char CMD_DONE[20];

	void StringCmp(char * Str, int QuestNr, int StateNr,int ReadStatus);
};

cCQScript::cCQScript()
{
	strcpy(CMD_ACCEPT,"CMD_ACCEPT");
	strcpy(CMD_DONE,"CMD_DONE");
	strcpy(ITEM_TYPE,"I_TYPE");
	strcpy(ITEM_INDEX,"I_INDEX");
	strcpy(ITEM_LEVEL,"I_LEVEL");
	strcpy(ITEM_OPT,"I_OPT");
	strcpy(ITEM_LUCK,"I_LUCK");
	strcpy(ITEM_SKILL,"I_SKILL");
	strcpy(ITEM_DUR,"I_DUR");
	strcpy(ITEM_EXC,"I_EXC");
	strcpy(ITEM_COUNT,"I_COUNT");
	strcpy(ZEN,"ZEN");
	strcpy(LEVEL,"LEVEL");
	strcpy(MASTERLEVEL,"MASTERLEVEL");
	strcpy(CLASS,"CLASS");
	strcpy(RESET,"RESET");
	strcpy(QUEST,"QUEST");
	strcpy(MONSTER_KILL,"MONSTER_KILL");
	strcpy(LEVELUP,"LEVELUP");
	strcpy(LEVELUPPOINT,"LEVELUPPOINT");
	strcpy(MASTERPOINT,"MASTERPOINT");
	strcpy(IF_FINISH,"IF_FINISH");
	strcpy(ELSE_FINISH,"ELSE_FINISH");
	strcpy(END_FINISH,"END_FINISH");
	strcpy(BEGIN_QUEST,"BEGIN_QUEST");
	strcpy(END_QUEST,"END_QUEST");
	strcpy(BEGIN_STATE,"BEGIN_STATE");
	strcpy(END_STATE,"END_STATE");
	strcpy(TITLE,"TITLE");
	strcpy(TEXT,"TEXT");
}


cCQScript::~cCQScript()
{
	return;
}

cCQScript QScript;

BOOL StrReplace(char * str,char Replace,char ReplaceTo)
{
	int i=0;
	BOOL isReplaced = FALSE;

	while(true)
	{
		if(str[i] == 0x00)
		{
			break;
		}
		else if(str[i] == Replace)
		{
			str[i] = ReplaceTo;
			isReplaced = TRUE;
		}
		i++;
	}
	return isReplaced;
}

void cCQScript::StringCmp(char * Str, int QuestNr, int StateNr,int ReadStatus)
{
	int Token;
	if ( strcmp(QScript.LEVEL, Str) == 0 )
	{
		Token = GetToken();
		if(ReadStatus == 0 || ReadStatus == 1)
			customQuest.Quest[QuestNr][StateNr].Level[0] = TokenNumber;
	}else if ( strcmp(QScript.MASTERLEVEL, Str) == 0 )
	{
		Token = GetToken();
		if(ReadStatus == 0 || ReadStatus == 1)
			customQuest.Quest[QuestNr][StateNr].MasterLevel = TokenNumber;
	}else if( strcmp(QScript.ZEN, Str) == 0 )
	{
		Token = GetToken();
		if(ReadStatus == 0 || ReadStatus == 1)
			customQuest.Quest[QuestNr][StateNr].Zen[ReadStatus] = TokenNumber;

	}else if( strcmp(QScript.RESET, Str) == 0 )
	{
		Token = GetToken();
		if(ReadStatus == 0 || ReadStatus == 1)
			customQuest.Quest[QuestNr][StateNr].Reset[ReadStatus] = TokenNumber;

	}else if( strcmp(QScript.QUEST, Str) == 0 )
	{
		Token = GetToken();
		customQuest.Quest[QuestNr][StateNr].Quest = TokenNumber;

	}else if( strcmp(QScript.CLASS, Str) == 0 )
	{
		Token = GetToken();
		if(ReadStatus == 0 || ReadStatus == 1)
			customQuest.Quest[QuestNr][StateNr].Class[ReadStatus] = TokenNumber;

	}else if( strcmp(QScript.TITLE, Str) == 0 )
	{
		Token = GetToken2();
		strcpy(customQuest.Quest[QuestNr][StateNr].Title[ReadStatus],TokenString);

	}else if( strcmp(QScript.TEXT, Str) == 0 )
	{
		Token = GetToken2();
		StrReplace(TokenString,'|','\n');
		strcpy(customQuest.Quest[QuestNr][StateNr].Text[ReadStatus],TokenString);

	}else if( strcmp(QScript.ITEM_TYPE, Str) == 0 )
	{
		Token = GetToken();
		if(ReadStatus == 0 || ReadStatus == 1)
			customQuest.Quest[QuestNr][StateNr].item[ReadStatus].Type = TokenNumber;

	}else if( strcmp(QScript.ITEM_INDEX, Str) == 0 )
	{
		Token = GetToken();
		if(ReadStatus == 0 || ReadStatus == 1)
			customQuest.Quest[QuestNr][StateNr].item[ReadStatus].Index = TokenNumber;
	}else if( strcmp(QScript.ITEM_LEVEL, Str) == 0 )
	{
		Token = GetToken();
		if(ReadStatus == 0 || ReadStatus == 1)
			customQuest.Quest[QuestNr][StateNr].item[ReadStatus].Level = TokenNumber;
	}else if( strcmp(QScript.ITEM_OPT, Str) == 0 )
	{	
		Token = GetToken();
		if(ReadStatus == 0 || ReadStatus == 1)
			customQuest.Quest[QuestNr][StateNr].item[ReadStatus].Opt = TokenNumber;
	}else if( strcmp(QScript.ITEM_LUCK, Str) == 0 )
	{

		Token = GetToken();
		if(ReadStatus == 0 || ReadStatus == 1)
			customQuest.Quest[QuestNr][StateNr].item[ReadStatus].Luck = TokenNumber;
	}else if( strcmp(QScript.ITEM_SKILL, Str) == 0 )
	{
		Token = GetToken();
		if(ReadStatus == 0 || ReadStatus == 1)
			customQuest.Quest[QuestNr][StateNr].item[ReadStatus].Skill = TokenNumber;
	}else if( strcmp(QScript.ITEM_DUR, Str) == 0 )
	{
		Token = GetToken();
		if(ReadStatus == 0 || ReadStatus == 1)
			customQuest.Quest[QuestNr][StateNr].item[ReadStatus].Dur = TokenNumber;
	}else if( strcmp(QScript.ITEM_EXC, Str) == 0 )
	{
		Token = GetToken();
		if(ReadStatus == 0 || ReadStatus == 1)
			customQuest.Quest[QuestNr][StateNr].item[ReadStatus].Exc = TokenNumber;
	}else if( strcmp(QScript.ITEM_COUNT, Str) == 0 )
	{
		Token = GetToken();
		if(ReadStatus == 0 || ReadStatus == 1)
			customQuest.Quest[QuestNr][StateNr].item[ReadStatus].Count = TokenNumber;

	}else if( strcmp(QScript.LEVELUP, Str) == 0 )
	{
		Token = GetToken();
		if(ReadStatus == 0 || ReadStatus == 1)
			customQuest.Quest[QuestNr][StateNr].Level[1] = TokenNumber;
	}else if( strcmp(QScript.LEVELUPPOINT, Str) == 0 )
	{
		Token = GetToken();
		if(ReadStatus == 0 || ReadStatus == 1)
			customQuest.Quest[QuestNr][StateNr].LevelUpPoint = TokenNumber;
	}else if( strcmp(QScript.MASTERPOINT, Str) == 0 )
	{
		Token = GetToken();
		if(ReadStatus == 0 || ReadStatus == 1)
			customQuest.Quest[QuestNr][StateNr].MasterPoint = TokenNumber;
	}else if( strcmp(QScript.MONSTER_KILL, Str) == 0 )
	{
		Token = GetToken();
		if(ReadStatus == 0 || ReadStatus == 1)
		{
			customQuest.Quest[QuestNr][StateNr].Monster_Kill[0] = TokenNumber;	
			Token = GetToken();	
			customQuest.Quest[QuestNr][StateNr].Monster_Kill[1] = TokenNumber;
		}else
		{
			Token = GetToken();
		}
	}
}

// lpObj->CustomQuest[i].Status = 0xFF -> Mail Dont Sended
// lpObj->CustomQuest[i].Status = 0x00 -> Mail Sended
// lpObj->CustomQuest[i].Status != 0 && lpObj->CustomQuest[i].Status != 0xFF -> Monsters Killed
// lpObj->CustomQuest[i].State  != 0xFF -> Quest State
// lpObj->CustomQuest[i].State  == 0x00 -> Quest State 0 -> Quest Accepted

void cCQuest::Clear()
{
	for(int i = 0; i < CQMAXCOUNT; i++)
	{
		for(int j = 0; j < 10; j++)
		{
			this->Quest[i][j].Active = FALSE;
			this->Quest[i][j].LevelUpPoint = 0;
			this->Quest[i][j].MasterPoint = 0;
			this->Quest[i][j].MasterLevel = 0;
			this->Quest[i][j].Quest = -1;
			for(int z = 0; z < 2; z++)
			{
				this->Quest[i][j].Class[z] = -1;
				this->Quest[i][j].Level[z] = 0;
				this->Quest[i][j].Monster_Kill[z] = 0;
				this->Quest[i][j].Reset[z] = 0;
				this->Quest[i][j].Zen[z] = 0;

				this->Quest[i][j].item[z].Count = 0;
				this->Quest[i][j].item[z].Dur = 0;
				this->Quest[i][j].item[z].Exc = 0;
				this->Quest[i][j].item[z].Index = 0;
				this->Quest[i][j].item[z].Level = 0;
				this->Quest[i][j].item[z].Luck = 0;
				this->Quest[i][j].item[z].Opt = 0;
				this->Quest[i][j].item[z].Skill = 0;
				this->Quest[i][j].item[z].Type = 0;
			}
		}
	}
}




void cCQuest::ReadTxt(char * filename)
{
	int Token;

	SMDFile = fopen(filename, "r");

	this->StateCount[this->Count] = 0;

	if ( SMDFile == NULL )
	{
		MsgBox("Custom Quest data load error %s", filename);
		return;
	}

	int StateCount = 0;

	while ( true )
	{
		Token = GetToken2();

		if ( strcmp(QScript.END_QUEST, TokenString) == 0 )
		{
			break;
		}
		if ( Token == 2 )
		{
			break;
		}

		if ( strcmp(QScript.BEGIN_QUEST, TokenString) == 0 )
		{
			while(true)
			{
				Token = GetToken();
				if ( strcmp(QScript.END_QUEST, TokenString) == 0 )
				{
					break;
				}
				if ( strcmp(QScript.BEGIN_STATE, TokenString) == 0 )
				{
					this->StateCount[this->Count]++;
					this->Quest[this->Count][StateCount].Active = TRUE;
					while(true)
					{
						Token = GetToken2();
						if ( strcmp(QScript.END_STATE, TokenString) == 0 )
						{
							StateCount++;
							break;
						}
						else if ( strcmp(QScript.IF_FINISH, TokenString) == 0 )
						{
							while(true)
							{
								Token = GetToken2();
								if ( strcmp(QScript.END_FINISH, TokenString) == 0 )
								{
									break;
								}else if ( strcmp(QScript.ELSE_FINISH, TokenString) == 0 )
								{
									while(true)
									{
										Token = GetToken2();
										if ( strcmp(QScript.END_FINISH, TokenString) == 0 )
										{
											break;
										}else
										{
											QScript.StringCmp(TokenString,this->Count,StateCount,2);
										}
									}
									break;
								}else
								{
									QScript.StringCmp(TokenString,this->Count,StateCount,1);
								}
								
							}
						}else
						{
							QScript.StringCmp(TokenString,this->Count,StateCount,0);
						}
					}
				}else if ( strcmp(QScript.CMD_ACCEPT, TokenString) == 0 )
				{
					Token = GetToken2();
					strcpy(customQuest.Cmd[this->Count][0],TokenString);
				}else if ( strcmp(QScript.CMD_DONE, TokenString) == 0 )
				{
					Token = GetToken2();
					strcpy(customQuest.Cmd[this->Count][1],TokenString);
				}
			}			
			this->Count++;
		}
	}
	fclose(SMDFile);
}

void cCQuest::MailSend(LPOBJ lpObj, int QuestNr, int StateNr, BYTE TextNr)
{
	if(this->Enabled == 0)
		return;

	if (lpObj->PlayerCQInfoReceived != 1)
	{
		GCServerMsgStringSend ("ERROR: Please relogin!",lpObj->m_Index,0x01 ) ;
		LogAddC(2, "ERROR: CQUEST Info [%s][%s] Line: %d", lpObj->AccountID, lpObj->Name, __LINE__);
		return;
	}

	if(StateNr == 0xFF)
		StateNr = 0;

	FHP_FRIEND_MEMO_SEND pMsg;

	pMsg.WindowGuid = 0;
	pMsg.Number = lpObj->m_Index;
	pMsg.Dir = 143;
	pMsg.Action = 27;
	wsprintf(pMsg.Subject,this->Quest[QuestNr][StateNr].Title[TextNr]);
	memcpy(pMsg.Name, lpObj->Name, sizeof(pMsg.Name));
	memcpy(pMsg.Photo, lpObj->CharSet, sizeof(pMsg.Photo));
	memcpy(pMsg.ToName, lpObj->Name, sizeof(pMsg.ToName));
	strcpy(pMsg.Memo,this->Quest[QuestNr][StateNr].Text[TextNr]);
	pMsg.MemoSize = strlen(this->Quest[QuestNr][StateNr].Text[TextNr]);
	int bsize = pMsg.MemoSize + sizeof(pMsg) - sizeof(pMsg.Memo);
	pMsg.h.set((LPBYTE)&pMsg, 0x70, bsize);
	wsExDbCli.DataSend((PCHAR)&pMsg, bsize);

	lpObj->PlayerCustomQuestInfoModified = 1;
	LogAdd("[%s](%d)(State:%d - Msg:%d) Custom Quest Mail Send",  lpObj->Name,QuestNr,StateNr,TextNr);
}

BOOL cCQuest::IsActive(LPOBJ lpObj,int Quest)
{
	if(lpObj->CustomQuest[Quest].State != 0xFF && lpObj->CustomQuest[Quest].Status != 0xFF)
		return 1;
	else
		return 0;
}

void cCQuest::LevelUp(LPOBJ lpObj)
{
	if(this->Enabled == 0)
		return;

	if ( gObjIsConnectedGP(lpObj->m_Index) == FALSE )
	{
		LogAddC(2, "error-L3 [%s][%d]", __FILE__, __LINE__);
		return;
	}

	if (lpObj->PlayerCQInfoReceived != 1)
	{
		GCServerMsgStringSend ("ERROR: Please relogin!",lpObj->m_Index,0x01 ) ;
		LogAddC(2, "ERROR: CQUEST Info [%s][%s] Line: %d", lpObj->AccountID, lpObj->Name, __LINE__);
		return;
	}

	if(this->Enabled == TRUE)
	{
		for(int i = 0; i < this->Count; i++)
		{
			if(lpObj->CustomQuest[i].Status == 0xFF) //Mail Not Sent
			{
				int State = lpObj->CustomQuest[i].State;
				if(State == 0xFF)
					State = 0;
				if((lpObj->DbClass == this->Quest[i][State].Class[0]) || (this->Quest[i][State].Class[0] == -1))
				{
					if((this->Quest[i][State].Quest == -1) || (lpObj->CustomQuest[i].State == this->StateCount[i]))
					{
						if(lpObj->Level >= this->Quest[i][State].Level[0] && lpObj->Resets >= this->Quest[i][State].Reset[0] && lpObj->MasterCharacterInfo->MasterLevel >= this->Quest[i][State].MasterLevel)
						{
							lpObj->CustomQuest[i].Status = 0;
							this->MailSend(lpObj,i,State,0);	
							lpObj->PlayerCustomQuestInfoModified = 1;	
						}
					}
				}
			}
		}
	}
}

BOOL cCQuest::Command(LPOBJ lpObj,char * Command)
{
	if(this->Enabled == 0)
		return 0;

	if ( gObjIsConnectedGP(lpObj->m_Index) == FALSE )
	{
		LogAddC(2, "error-L3 [%s][%d]", __FILE__, __LINE__);
		return 0;
	}

	if (lpObj->PlayerCQInfoReceived != 1)
	{
		GCServerMsgStringSend ("ERROR: Please relogin!",lpObj->m_Index,0x01 ) ;
		LogAddC(2, "ERROR: CQUEST Info [%s][%s] Line: %d", lpObj->AccountID, lpObj->Name, __LINE__);
		return 0;
	}

	if(this->Enabled == TRUE)
	{
		for(int i = 0; i < this->Count; i++)
		{
			if(stricmp( Command,this->Cmd[i][0]) == 0 )
			{
				if((lpObj->CustomQuest[i].Status == 0x00) && (lpObj->CustomQuest[i].State == 0xFF)) //Mail Sended & Doesnt Start
				{
					GCServerMsgStringSend("Quest Accepted!",lpObj->m_Index, 0x01);
					lpObj->PlayerCustomQuestInfoModified = 1;
					lpObj->CustomQuest[i].State = 0;
				}
				return 1;
			}else if(stricmp( Command,this->Cmd[i][1]) == 0 )
			{
				if(this->CheckStateComplete(lpObj,i) == TRUE)
				{
					if((lpObj->CustomQuest[i].Status != 0xFF) && (lpObj->CustomQuest[i].State != 0xFF)) //Mail Already sended, quest already started
					{
						if(this->Quest[i][lpObj->CustomQuest[i].State].Active == TRUE)
						{
							this->StateComplete(lpObj,i);
							lpObj->CustomQuest[i].State++;
							lpObj->CustomQuest[i].Status = 0xFF;
							lpObj->PlayerCustomQuestInfoModified = 1;
							if(this->Quest[i][lpObj->CustomQuest[i].State].Active == TRUE)
							{
								if((this->Quest[i][lpObj->CustomQuest[i].State].Quest == -1) || (lpObj->CustomQuest[i].State == this->StateCount[i]))
								{
									if(lpObj->Level >= this->Quest[i][lpObj->CustomQuest[i].State].Level[0] && lpObj->Resets >= this->Quest[i][lpObj->CustomQuest[i].State].Reset[0] && lpObj->MasterCharacterInfo->MasterLevel >= this->Quest[i][lpObj->CustomQuest[i].State].MasterLevel)
									{
										lpObj->CustomQuest[i].Status = 0;
										this->MailSend(lpObj,i,lpObj->CustomQuest[i].State,0);
									}
								}
							}
						}
					}
				}else
				{
					if(lpObj->Level >= this->Quest[i][lpObj->CustomQuest[i].State].Level[0] && lpObj->Resets >= this->Quest[i][lpObj->CustomQuest[i].State].Reset[0] && lpObj->MasterCharacterInfo->MasterLevel >= this->Quest[i][lpObj->CustomQuest[i].State].MasterLevel)
						this->MailSend(lpObj,i,lpObj->CustomQuest[i].State,2);
					else
						GCServerMsgStringSend("Quest not available right now!",lpObj->m_Index, 0x01);
				}
				return 1;
			}
		}
	}
	return 0;
}

BOOL cCQuest::CheckStateComplete(LPOBJ lpObj, int QuestNr)
{
	if(this->Enabled == 0)
		return FALSE;

	if ( gObjIsConnectedGP(lpObj->m_Index) == FALSE )
	{
		LogAddC(2, "error-L3 [%s][%d]", __FILE__, __LINE__);
		return FALSE;
	}

	int State = lpObj->CustomQuest[QuestNr].State;

	if(lpObj->MasterCharacterInfo->MasterLevel < this->Quest[QuestNr][State].MasterLevel)
		return FALSE;

	if(lpObj->Level < this->Quest[QuestNr][State].Level[0])
		return FALSE;

	if(lpObj->Resets < this->Quest[QuestNr][State].Reset[0])
		return FALSE;

	if(lpObj->Money < this->Quest[QuestNr][State].Zen[0])
		return FALSE;

	if(this->Quest[QuestNr][State].Monster_Kill[1] > 0)
	{
		if(lpObj->CustomQuest[QuestNr].Status == 0xFF)
			return FALSE;
		if(lpObj->CustomQuest[QuestNr].Status < this->Quest[QuestNr][State].Monster_Kill[1])
			return FALSE;
	}

	if((lpObj->DbClass != this->Quest[QuestNr][State].Class[0]) && (this->Quest[QuestNr][State].Class[0] != -1))
		return FALSE;

	if(this->Quest[QuestNr][State].Quest != -1) 
		if(this->IsActive(lpObj,this->Quest[QuestNr][State].Quest) == 0)
			return FALSE;

	if(this->Quest[QuestNr][State].item[0].Count > 0)
	{
		int item = ((this->Quest[QuestNr][State].item[0].Type * 512) + this->Quest[QuestNr][State].item[0].Index);
		if(gObjCheckItemsCount(lpObj->m_Index,item,this->Quest[QuestNr][State].item[0].Level,this->Quest[QuestNr][State].item[0].Opt,this->Quest[QuestNr][State].item[0].Luck,this->Quest[QuestNr][State].item[0].Skill)  < this->Quest[QuestNr][State].item[0].Count)
			return FALSE;
	}

	if(lpObj->CustomQuest[QuestNr].Status < this->Quest[QuestNr][State].Monster_Kill[1])
		return FALSE;

	return TRUE;
}

void cCQuest::StateComplete(LPOBJ lpObj, int QuestNr)
{
	if(this->Enabled == 0)
		return;

	if ( gObjIsConnectedGP(lpObj->m_Index) == FALSE )
	{
		LogAddC(2, "error-L3 [%s][%d]", __FILE__, __LINE__);
		return;
	}

	int State = lpObj->CustomQuest[QuestNr].State;

	if(this->Quest[QuestNr][State].item[0].Count > 0)
	{
		int item = ItemGetNumberMake(this->Quest[QuestNr][State].item[0].Type, this->Quest[QuestNr][State].item[0].Index);
		for(int i=0;i<this->Quest[QuestNr][State].item[0].Count;i++)
			DeleteItemByMultipleValues(lpObj->m_Index,item,this->Quest[QuestNr][State].item[0].Level,this->Quest[QuestNr][State].item[0].Opt,this->Quest[QuestNr][State].item[0].Luck,this->Quest[QuestNr][State].item[0].Skill);
	}
	if(this->Quest[QuestNr][State].Zen[0] > 0)
	{
		lpObj->Money -= this->Quest[QuestNr][State].Zen[0];
		CGZenSend(lpObj->m_Index);	
	}

	if((this->Quest[QuestNr][State].Class[1] != lpObj->DbClass) && (this->Quest[QuestNr][State].Class[1] != -1))
	{
		lpObj->DbClass = this->Quest[QuestNr][State].Class[1];
		lpObj->Class = lpObj->DbClass >> 4;
		lpObj->ChangeUP = lpObj->DbClass & 0x07;
		gObjMagicDelAll(lpObj);
		DeleteAllItems(lpObj->m_Index);
		gObjMakePreviewCharSet(lpObj->m_Index);
		GCLevelUpMsgSend(lpObj->m_Index, 1);
		gObjViewportListProtocolCreate(lpObj);
	}

	if(this->Quest[QuestNr][State].item[1].Count > 0)
	{
		int type = ItemGetNumberMake(this->Quest[QuestNr][State].item[1].Type, this->Quest[QuestNr][State].item[1].Index);
		ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, lpObj->X, lpObj->Y, type, this->Quest[QuestNr][State].item[1].Level, this->Quest[QuestNr][State].item[1].Dur, this->Quest[QuestNr][State].item[1].Luck, this->Quest[QuestNr][State].item[1].Opt, this->Quest[QuestNr][State].item[1].Skill, lpObj->m_Index, this->Quest[QuestNr][State].item[1].Exc, 0);
	}

	if(this->Quest[QuestNr][State].LevelUpPoint > 0)
	{
		lpObj->LevelUpPoint += this->Quest[QuestNr][State].LevelUpPoint;
		gObjCalCharacter(lpObj->m_Index);
		GCLevelUpMsgSend(lpObj->m_Index, 1);
		//GCSendQuestPrize(lpObj->m_Index, 200, this->Quest[QuestNr][State].LevelUpPoint );
		//gObjMakePreviewCharSet(lpObj->m_Index);
	}
	if(this->Quest[QuestNr][State].Level[1] > 0)
	{
		if (gObjIsNewClass(lpObj) != 1)
		{
			if( lpObj->Level < ReadConfig.Max_Normal_Level )
			{
				lpObj->Level += this->Quest[QuestNr][State].Level[1];
			}
		} else {
			if ( lpObj->MasterCharacterInfo->MasterLevel < ReadConfig.Max_Master_Level )
			{
				lpObj->MasterCharacterInfo->MasterLevel += this->Quest[QuestNr][State].Level[1];
			}
		}
		
		lpObj->Experience = gObjExpCal(lpObj,-1);	//gLevelExperience[lpObj->Level-1];

		gObjCalCharacter(lpObj->m_Index);

		lpObj->MaxLife += (DCInfo.DefClass[ lpObj->Class ].LevelLife * this->Quest[QuestNr][State].Level[1]);
		lpObj->MaxMana += (DCInfo.DefClass[ lpObj->Class ].LevelMana * this->Quest[QuestNr][State].Level[1]);
		//gObjCalcShieldPoint(lpObj);
		//gObjSetBP(lpObj->m_Index);

		gObjFixGagesOverflow(lpObj);

		lpObj->Life = lpObj->MaxLife + lpObj->AddLife;
		lpObj->Mana = lpObj->MaxMana + lpObj->AddMana;
		lpObj->iShield = lpObj->iMaxShield + lpObj->iAddShield;

		GCReFillSend(lpObj->m_Index,lpObj->Life,0xFF,0,lpObj->iShield);

		//gObjNextExpCal(lpObj);	
		//GJSetCharacterInfo(lpObj, lpObj->m_Index, 0);

		GCLevelUpMsgSend(lpObj->m_Index, 1);
		gObjCalcMaxLifePower(lpObj->m_Index);
	}
	if(this->Quest[QuestNr][State].MasterPoint > 0)
	{
		lpObj->MasterCharacterInfo->MasterPoints += this->Quest[QuestNr][State].MasterPoint;
		Mastering.InfoSend(lpObj->m_Index);
	}
	if(this->Quest[QuestNr][State].Reset[1] > 0)
	{
		lpObj->Resets += this->Quest[QuestNr][State].Reset[1];
	}
	if(this->Quest[QuestNr][State].Zen[1] > 0)
	{
		lpObj->Money += this->Quest[QuestNr][State].Zen[1];
		CGZenSend(lpObj->m_Index);	
	}

	//Mastering.LevelUp
	this->MailSend(lpObj,QuestNr,State,1);
}


void cCQuest::Init(char * FilePath)
{
	this->Clear();
	this->Enabled				= GetPrivateProfileInt("Common", "SCFCustomQuestsEnabled",0, FilePath) ;
	
	if(this->Enabled == TRUE)
	{
		int Counter					= GetPrivateProfileInt("Common", "SCFCustomQuestsCount",0, FilePath) ;

		char etiqueta[100];

		for (int i = 0; i < Counter; i++)
		{
			char buf[1024]={0};
			wsprintf(etiqueta,"Quest%03d", i+1);
			GetPrivateProfileString("Path", etiqueta, "", buf, 1024, FilePath);
			this->ReadTxt(buf);
		}
	}
}

void cCQuest::MonsterDie(LPOBJ lpObj,int MonsterIndex)
{
	if(this->Enabled == 0)
		return;

	if ( gObjIsConnectedGP(lpObj->m_Index) == FALSE )
	{
		//LogAddC(2, "error-L3 [%s][%d]", __FILE__, __LINE__);
		return;
	}

	if (lpObj->PlayerCQInfoReceived != 1)
	{
		GCServerMsgStringSend ("ERROR: Please relogin!",lpObj->m_Index,0x01 ) ;
		LogAddC(2, "ERROR: CQUEST Info [%s][%s] Line: %d", lpObj->AccountID, lpObj->Name, __LINE__);
		return;
	}

	if(this->Enabled == TRUE)
	{
		for(int i = 0; i < this->Count; i++)
		{
			if(lpObj->CustomQuest[i].Status != 0xFF && lpObj->CustomQuest[i].State != 0xFF)
			{
				int State = lpObj->CustomQuest[i].State;
				if((this->Quest[i][State].Monster_Kill[1] > 0) && (this->Quest[i][State].Monster_Kill[0] == MonsterIndex))
				{
					if(lpObj->CustomQuest[i].Status < this->Quest[i][State].Monster_Kill[1])
					{
						lpObj->CustomQuest[i].Status++;
						lpObj->PlayerCustomQuestInfoModified = 1;
					}
				}
			}
		}
	}
}

#endif