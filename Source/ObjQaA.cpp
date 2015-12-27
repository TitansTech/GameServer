#include "stdafx.h"
#include "GameServer.h"
#include "LogProc.h"
#include "GameMain.h"
#include "User.h"
#include "SCFExDBProtocol.h"
#include "ObjQaA.h"
#include "..\common\winutil.h"
#include "..\include\readscript.h"
#include "..\common\SetItemOption.h"

cObjQaA ObjQaA;

int cObjQaA::SearchQuestion(BYTE ID)
{
	for(int i=0;i<QaA_QUESTIONSCOUNT;i++)
	{
		if(this->Question[i].ID == ID)
			return i;
	}
	return -1;
}

void cObjQaA::Read(char * FilePath)
{
	this->CurrentQuestion = 0;
	this->Start = false;
	this->Enabled = 0;
	this->QuestionsCnt = 0;
	this->RewardCount = 0;
	this->PlayerIDReply = 0;
	memset(this->Question,0,sizeof(this->Question));
	memset(this->reward,0,sizeof(this->reward));

	int Token;
	SMDFile = fopen(FilePath, "r");

	if ( SMDFile == NULL )
	{
		MsgBox("QaA data load error %s", FilePath);
		return;
	}

	while ( true )
	{
		int iType = GetToken();
		iType = TokenNumber;

		if ( iType == 1 )
		{
			while(true)
			{
				Token = GetToken();
				if ( strcmp("end", TokenString) == 0 )
				{
					break;
				}
				this->Enabled = TokenNumber;
				
				Token = GetToken();
				this->MaxQuestions = TokenNumber;
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
				this->Question[this->QuestionsCnt].ID = TokenNumber;
				
				Token = GetToken();
				strcpy(this->Question[this->QuestionsCnt].Question ,TokenString);

				Token = GetToken();
				this->Question[this->QuestionsCnt].CorrectAnswer = TokenString[0];
				
				if((this->QuestionsCnt + 1) >= QaA_QUESTIONSCOUNT)
					break;
				else
					this->QuestionsCnt++;
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

				int qID = 0;
				qID = this->SearchQuestion(TokenNumber);
				
				Token = GetToken();
				
				if(qID != -1)
				{
					int AnswerCnt = this->Question[qID].AnswersCnt;
					if(AnswerCnt < QaA_ANSWERSCOUNT)
					{
						strcpy(this->Question[qID].Answer[AnswerCnt] ,TokenString);
						this->Question[qID].AnswersCnt++;
					}
				}
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
				if(this->RewardCount < 0 || this->RewardCount > MAX_QaA_REWARD-1)
				{
					MsgBox("QaA error: Max Reward Count", MAX_QaA_REWARD);
					return;
				}

				this->reward[this->RewardCount].Zen = TokenNumber;

				Token = GetToken();
				this->reward[this->RewardCount].VipMoney = TokenNumber;

				Token = GetToken();
				int iType = TokenNumber;

				Token = GetToken();
				int iIndex = TokenNumber;

				this->reward[this->RewardCount].num = ITEMGET(iType,iIndex);

				Token = GetToken();
				this->reward[this->RewardCount].Level = TokenNumber;

				Token = GetToken();
				this->reward[this->RewardCount].Opt = TokenNumber;

				Token = GetToken();
				this->reward[this->RewardCount].Luck = TokenNumber;

				Token = GetToken();
				this->reward[this->RewardCount].Skill = TokenNumber;

				Token = GetToken();
				this->reward[this->RewardCount].Dur = TokenNumber;

				Token = GetToken();
				this->reward[this->RewardCount].Exc = TokenNumber;

				Token = GetToken();
				this->reward[this->RewardCount].Anc = TokenNumber;

				Token = GetToken();
				this->reward[this->RewardCount].Sock1 = TokenNumber;

				Token = GetToken();
				this->reward[this->RewardCount].Sock2 = TokenNumber;

				Token = GetToken();
				this->reward[this->RewardCount].Sock3 = TokenNumber;

				Token = GetToken();
				this->reward[this->RewardCount].Sock4 = TokenNumber;

				Token = GetToken();
				this->reward[this->RewardCount].Sock5 = TokenNumber;

				Token = GetToken();
				this->reward[this->RewardCount].JOH = TokenNumber;

				Token = GetToken();
				this->reward[this->RewardCount].Days = TokenNumber;

				Token = GetToken();
				this->reward[this->RewardCount].IsForeverFFFE = TokenNumber;

				if(this->reward[this->RewardCount].Days != 0)
					this->reward[this->RewardCount].IsForeverFFFE = 0;

				this->RewardCount++;
			}
		}
		break;
	}
	
	if(this->MaxQuestions > this->QuestionsCnt)
		this->MaxQuestions = this->QuestionsCnt;
	
	this->CurrentQuestion = 0;
		
	LogAddTD("[QaA] - %s file is Loaded",FilePath);
	fclose(SMDFile);
}

void cObjQaA::AnswerCmd(int aIndex, char * pAnswer)
{	
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}
	if(this->Enabled == 1 && this->Start == true)
	{
		if(this->PlayerIDReply == 0 && StartAnswer)
		{
			if(tolower(ObjQaA.Question[this->CurrentQuestion].CorrectAnswer) == tolower(pAnswer[0]))
				this->PlayerIDReply = aIndex;
		}
	}
}

void cObjQaA::SendReward()
{
	if ( !gObjIsConnected(ObjQaA.PlayerIDReply))
	{
		return;
	}

	int ranRew = rand()%this->RewardCount;
#if (PACK_EDITION>=3)
	::SendReward(ObjQaA.PlayerIDReply,1,this->reward[ranRew]);
#endif
}

void cObjQaA::StartEvent()
{	
	if(this->Enabled == 1 && this->Start == false)
	{
		_beginthread(cObjQaA__InsideTrigger, 0, NULL  );
	}
}

void cObjQaA__InsideTrigger(void * lpParam)
{
	char sTmp[512]={0};

	ObjQaA.Start = true;
	
	int Cnt = 0;
	int insideCnt = 0;
	
	if(ObjQaA.QuestionsCnt > 0)
	{
		AllSendServerMsg("Question Event Start, Get Ready...");
		while(true)
		{
			ObjQaA.StartAnswer = false;
			ObjQaA.PlayerIDReply = 0;
			int ID = ObjQaA.CurrentQuestion;
			
			wsprintf(sTmp,"%s",ObjQaA.Question[ID].Question);
			AllSendServerMsg(sTmp);
			Sleep(2000);
			
			for(int i=0;i<ObjQaA.Question[ID].AnswersCnt;i++)
			{				
				wsprintf(sTmp,"%s",ObjQaA.Question[ID].Answer[i]);
				AllSendServerMsg(sTmp);
			}
			ObjQaA.StartAnswer = true;
			
			insideCnt = 0;
			while(true)
			{
				if(ObjQaA.PlayerIDReply != 0)
				{
					ObjQaA.SendReward();
					wsprintf(sTmp,"%s answer fine the question!",gObj[ObjQaA.PlayerIDReply].Name);
					AllSendServerMsg(sTmp);
					break;
				}
				if(insideCnt >= 60)
				{
					AllSendServerMsg("No one answer fine the question...");
					break;
				}
				insideCnt++;
				Sleep(1000);
			}
						
			if((ObjQaA.CurrentQuestion +1) >= ObjQaA.QuestionsCnt)
				ObjQaA.CurrentQuestion = 0;
			else			
				ObjQaA.CurrentQuestion++;
			
			Cnt++;
			if(Cnt >= ObjQaA.MaxQuestions)
				break;
			else
				Sleep(10000);
		}
		AllSendServerMsg("Question Event Finished!!");
#if (PACK_EDITION>=3)
		AllSendServerMsg("Winners can get their reward in Bot Reward!!");	
#endif
	}

	ObjQaA.StartAnswer = false;
	ObjQaA.Start = false;
}