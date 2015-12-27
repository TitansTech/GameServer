#include "stdafx.h"
#if (PACK_EDITION>=3)
#include "SCFExDB_Defines.h"
#include "GameServer.h"
#include "LogProc.h"
#include "TNotice.h"
#include "..\include\readscript.h"
#include "ObjTitanRank.h"
#include "Event.h"
#include "DBSockMng.h"
#include "..\common\winutil.h"

cObjTitanRank ObjTitanRank;

void cObjTitanRank::Read(char * FilePath)
{
	this->Enabled = false;
	this->Start = false;
	this->Cnt = 0;
	memset(this->Tops,0,sizeof(this->Tops));

	int Token;
	SMDFile = fopen(FilePath, "r");

	if ( SMDFile == NULL )
	{
		MsgBox("TitanRank data load error %s", FilePath);
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
				Token = GetToken();
				if ( strcmp("end", TokenString) == 0 )
				{
					break;
				}
				this->Enabled = TokenNumber;

				Token = GetToken();
				this->SecsDelay = TokenNumber*1000;
					
				Token = GetToken();
				strcpy(this->StartMsg,TokenString);
			}
		}
		
		iType = GetToken();
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

				int tIndex = TokenNumber;
				if(tIndex < 0 || tIndex > MAX_TITANRANK_CNT-1)
				{
					MsgBox("TitanRank error: TopPlayer Index:%d out of range!", tIndex);
					return;
				}

				this->Tops[tIndex].Enabled = 1;

				Token = GetToken();
				this->Tops[tIndex].Type = TokenNumber;

				Token = GetToken();
				this->Tops[tIndex].SubType = TokenNumber;

				Token = GetToken();
				this->Tops[tIndex].Class = TokenNumber;

				Token = GetToken();
				this->Tops[tIndex].Top = TokenNumber;
				
				Token = GetToken();
				strcpy(this->Tops_Text[tIndex],TokenString);
				
				if(this->Tops[tIndex].Top > 3)
					this->Tops[tIndex].Top = 3;
				else if(this->Tops[tIndex].Top < 0)
					this->Tops[tIndex].Top = 1;
				
				this->Tops[tIndex].rIndex = tIndex;
				
				this->Cnt++;
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

				int tIndex = TokenNumber;
				if(tIndex < 0 || tIndex > MAX_TITANRANK_CNT-1)
				{
					MsgBox("TopPlayer error: TopPlayerIndex:%d doesnt exist", tIndex);
					return;
				}
				if(this->Tops[tIndex].Enabled == false)
				{	
					MsgBox("TopPlayer error: TopPlayerIndex:%d doesnt exist", tIndex);
					return;
				}

				if(this->Tops[tIndex].RewCnt >= MAX_TITANRANK_REW)
				{
					MsgBox("TopPlayer error: TopPlayerIndex:%d doesnt exist", tIndex);
					return;
				}

				int rcnt = this->Tops[tIndex].RewCnt;

				int iIndex = 0;
				int iType = 0;

				Token = GetToken();
				this->Tops[tIndex].Reward[rcnt].Zen = TokenNumber;

				Token = GetToken();
				this->Tops[tIndex].Reward[rcnt].VipMoney = TokenNumber;

				Token = GetToken();
				iType = TokenNumber;

				Token = GetToken();
				iIndex = TokenNumber;
				
				this->Tops[tIndex].Reward[rcnt].num = ITEMGET(iType,iIndex);

				Token = GetToken();
				this->Tops[tIndex].Reward[rcnt].Level = TokenNumber;

				Token = GetToken();
				this->Tops[tIndex].Reward[rcnt].Opt = TokenNumber;

				Token = GetToken();
				this->Tops[tIndex].Reward[rcnt].Luck = TokenNumber;

				Token = GetToken();
				this->Tops[tIndex].Reward[rcnt].Skill = TokenNumber;

				Token = GetToken();
				this->Tops[tIndex].Reward[rcnt].Dur = TokenNumber;

				Token = GetToken();
				this->Tops[tIndex].Reward[rcnt].Exc = TokenNumber;

				Token = GetToken();
				this->Tops[tIndex].Reward[rcnt].Anc = TokenNumber;

				Token = GetToken();
				this->Tops[tIndex].Reward[rcnt].JOH = TokenNumber;

				Token = GetToken();
				this->Tops[tIndex].Reward[rcnt].Sock1 = TokenNumber;

				Token = GetToken();
				this->Tops[tIndex].Reward[rcnt].Sock2 = TokenNumber;

				Token = GetToken();
				this->Tops[tIndex].Reward[rcnt].Sock3 = TokenNumber;

				Token = GetToken();
				this->Tops[tIndex].Reward[rcnt].Sock4 = TokenNumber;

				Token = GetToken();
				this->Tops[tIndex].Reward[rcnt].Sock5 = TokenNumber;

				Token = GetToken();
				this->Tops[tIndex].Reward[rcnt].Days = TokenNumber;

				Token = GetToken();
				this->Tops[tIndex].Reward[rcnt].IsForeverFFFE = TokenNumber;

				this->Tops[tIndex].RewCnt++;
			}
		}
		break;
	}

	LogAddTD("[TopPlayer] - %s file is Loaded",FilePath);
	fclose(SMDFile);
}

void cObjTitanRank::StartEvent()
{	
	if(this->Enabled == 1 && this->Start == false)
	{
		_beginthread(cObjTitanRank__StartEvent, 0, NULL  );
	}
}

void cObjTitanRank__StartEvent(void * pvoid)
{
	ObjTitanRank.Start = true;

	AllSendServerMsg(ObjTitanRank.StartMsg);
	
	int tmpCnt = 0;
	int i=0;

	while(tmpCnt < ObjTitanRank.Cnt)
	{
		if(i >= MAX_TITANRANK_CNT)
			break;

		if(ObjTitanRank.Tops[i].Enabled == 1)
		{		
			SDHP_GETRANKINFO result = {0};
			PHeadSetB((LPBYTE)&result, SCFExDB_GSSend_SetTitanRankInfo, sizeof(result));
			result.rIndex = i;
			memcpy(&result.info,&ObjTitanRank.Tops[i],sizeof(result.info));
			cDBSMng.Send((char *)&result,sizeof(result));//ENVIAR AL LINKSERVER LA DATA DEL PREMIO Y LOS REQg
			tmpCnt++;
		}

		i++;
		Sleep(ObjTitanRank.SecsDelay);
	}
	ObjTitanRank.Start = false;
}

void cObjTitanRank::ShowInfo(BYTE * pRecv)
{
	SDHP_GETRANK_INFORECV * aRecv = (SDHP_GETRANK_INFORECV *) pRecv;
	if(this->Enabled == 1)
	{
		if(this->Tops[aRecv->rIndex].Enabled == 1)
		{			
			AllSendServerMsg(this->Tops_Text[aRecv->rIndex]);
			for(int i=0;i<MAX_TITANRANK_REW;i++)
			{
				if(aRecv->info[i].Name != 0 && aRecv->info[i].AccountID != 0)
				{
					AllSendServerMsg(aRecv->info[i].Name);
				}
			}
		}
	}
}

#endif