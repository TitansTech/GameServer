// ------------------------------
// Decompiled by Deathway
// Date : 2007-03-09
// ------------------------------
// GS-N 0.99.60T 0x004F2D50
//	GS-N	1.00.18	JPN	0x0051D480	-	Completed
#include "stdafx.h"

#include "MoveCommand.h"
#include "..\include\readscript.h"
#include "logproc.h"
#include "..\common\WzMemScript.h"
#include "DevilSquare.h"
#include "KalimaGate.h"
#include "ChaosCastle.h"
#include "CastleSiegeSync.h"
#include "CashShop.h"
#include "IllusionTemple.h"
#include "Raklion.h"
#include "ImperialGuardian.h"
#include "DoppelGanger.h"
#include "Crywolf.h"
#include "CrywolfSync.h"
#include "ObjBotRacer.h"

CMoveCommand gMoveCommand;


CMoveCommand::CMoveCommand()
{
	return;
}


CMoveCommand::~CMoveCommand()
{
	return;
}


void CMoveCommand::Init()
{
	memset(this->m_MoveCommandData, 0, sizeof(this->m_MoveCommandData));

	for (int i =0;i<MAX_MOVE_COMMAND;i++)
	{
		this->m_MoveCommandData[i].Index = -1;
	}
}


int CMoveCommand::Load(char* filename)
{
	int Token;
	int counter;

	this->Init();

	SMDFile=fopen(filename, "r");

	if ( SMDFile == 0 )
	{
		MsgBox("MoveCommand %s File Load Error", filename);
		return 0;
	}

	counter=0;

	while ( true )
	{
		Token=GetToken();

		if ( Token == 2)
		{
			break;
		}
		
		
		if ( Token == 1)
		{
			this->m_MoveCommandData[counter].Index = TokenNumber;

			Token = GetToken();
			strcpy(this->m_MoveCommandData[counter].Name, TokenString);

			Token = GetToken();
			strcpy(this->m_MoveCommandData[counter].EngName, TokenString);

			Token = GetToken();
			this->m_MoveCommandData[counter].NeedZen = TokenNumber;

			Token = GetToken();
			this->m_MoveCommandData[counter].NeedLevel = TokenNumber;

			Token = GetToken();
			this->m_MoveCommandData[counter].GateNumber = TokenNumber;

			counter++;
		}
		else
		{
			this->Init();
			fclose(SMDFile);
			MsgBox("MoveCommand script load fail");
			return FALSE;
		}
	}
	fclose(SMDFile);
	return TRUE;
}

int CMoveCommand::Load(char* Buffer, int iSize)
{
	this->Init();
	CWzMemScript WzMemScript;
	int Token;
	int counter;
	
	WzMemScript.SetBuffer(Buffer, iSize);

	counter=0;

	while ( true )
	{
		Token=WzMemScript.GetToken();

		if ( Token == 2)
		{
			break;
		}
		
		
		if ( Token == 1)
		{
			this->m_MoveCommandData[counter].Index = WzMemScript.GetNumber();

			Token = WzMemScript.GetToken();
			strcpy(this->m_MoveCommandData[counter].Name, WzMemScript.GetString());

			Token = WzMemScript.GetToken();
			strcpy(this->m_MoveCommandData[counter].EngName, WzMemScript.GetString());

			Token = WzMemScript.GetToken();
			this->m_MoveCommandData[counter].NeedZen = WzMemScript.GetNumber();

			Token = WzMemScript.GetToken();
			this->m_MoveCommandData[counter].NeedLevel = WzMemScript.GetNumber();

			Token = WzMemScript.GetToken();
			this->m_MoveCommandData[counter].GateNumber = WzMemScript.GetNumber();

			counter++;
		}
		else
		{
			this->Init();
			MsgBox("MoveCommand script load fail");
			return FALSE;
		}
	}
	return TRUE;
}


int CMoveCommand::LoadMoveLevel(char* filename)
{
	int Token;
	int counter;

	memset(this->m_MoveLevel, -1, sizeof(this->m_MoveLevel));

	SMDFile=fopen(filename, "r");

	if ( SMDFile == 0 )
	{
		MsgBox("MoveLevel %s File Load Error", filename);
		return 0;
	}

	counter=0;

	while ( true )
	{
		Token=GetToken();

		if ( Token == 2)
		{
			break;
		}
		
		
		if ( Token == 1)
		{
			this->m_MoveLevel[counter].MoveLevel = TokenNumber;

			Token = GetToken();
			this->m_MoveLevel[counter].MapNumber = TokenNumber;

			Token = GetToken();
			this->m_MoveLevel[counter].X = TokenNumber;

			Token = GetToken();
			this->m_MoveLevel[counter].Y = TokenNumber;

			Token = GetToken();
			this->m_MoveLevel[counter].TX = TokenNumber;

			Token = GetToken();
			this->m_MoveLevel[counter].TY = TokenNumber;

			counter++;
		}
		else
		{
			this->Init();
			fclose(SMDFile);
			MsgBox("MoveLevel script load fail");
			return FALSE;
		}
	}
	fclose(SMDFile);
	return TRUE;
}



int  CMoveCommand::GetMoveLevel(int mapnumber, int x, int y, int Class)
{
	for( int i=0;i<MAX_MOVE_COMMAND;i++)
	{
		if ( this->m_MoveLevel[i].MapNumber == mapnumber )
		{
			//if ( x >= this->m_MoveLevel[i].X && x <= this->m_MoveLevel[i].TX && y <= this->m_MoveLevel[i].Y && y >= this->m_MoveLevel[i].TY )
			//{
				if (Class == CLASS_MAGICGLADIATOR || 
					Class == CLASS_DARKLORD || 
					Class == CLASS_RAGEFIGHTER)
				{
					return this->m_MoveLevel[i].MoveLevel * 2 / 3;
				}
				else
				{
					return this->m_MoveLevel[i].MoveLevel;
				}
			//}
		}
	}

	return -1;
}



BOOL CMoveCommand::CheckMainToMove(LPOBJ lpObj)
{
	if ( BC_MAP_RANGE(lpObj->MapNumber) != FALSE )
	{
		return FALSE;
	}

	if ( CC_MAP_RANGE(lpObj->MapNumber) != FALSE )
	{
		return FALSE;
	}

	if ( KALIMA_MAP_RANGE(lpObj->MapNumber) != FALSE )
	{
		return FALSE;
	}

	if ( DS_MAP_RANGE(lpObj->MapNumber) != FALSE )
	{
		return FALSE;
	}

	if ( IT_MAP_RANGE(lpObj->MapNumber) != FALSE )
	{
		return FALSE;
	}

#if (PACK_EDITION>=3)
	if ( DG_MAP_RANGE(lpObj->MapNumber) != FALSE )
	{
		return FALSE;
	}
#endif
#if (PACK_EDITION>=2)
	if ( IMPERIALGUARDIAN_MAP_RANGE(lpObj->MapNumber) != FALSE )
	{
		return FALSE;
	}
#endif
	if ( lpObj->MapNumber == MAP_INDEX_KANTURU_BOSS )
		return FALSE;

	if ( lpObj->MapNumber == MAP_INDEX_RAKLIONBOSS )
		return FALSE;

	if ( lpObj->MapNumber == MAP_INDEX_CASTLEHUNTZONE )
		return FALSE;

	if ( lpObj->MapNumber == MAP_INDEX_BARRACKS )
		return FALSE;

	if ( lpObj->MapNumber == MAP_INDEX_REFUGE )
		return FALSE;

	if ( lpObj->MapNumber == MAP_INDEX_DUELMAP )
		return FALSE;

	return TRUE;
}





BOOL CMoveCommand::CheckEquipmentToMove(LPOBJ lpObj, int iTargetMapNumber)
{
	if ( iTargetMapNumber == MAP_INDEX_ATLANS )
	{
		if ( lpObj->pInventory[8].IsItem() != FALSE )
		{
			if ( lpObj->pInventory[8].m_Type == ITEMGET(13,2) ) // Uniria
			{
				return FALSE;
			}

			if ( lpObj->pInventory[8].m_Type == ITEMGET(13,3) ) // Dino
			{
				return FALSE;
			}
		}
	}

	if ( iTargetMapNumber == MAP_INDEX_ICARUS )
	{
		if ( (lpObj->pInventory[8].m_Type != ITEMGET(13,3) &&
			 lpObj->pInventory[7].m_Type != ITEMGET(13,30) &&
			 lpObj->pInventory[7].m_Type != ITEMGET(12,49) &&
			 lpObj->pInventory[7].m_Type != ITEMGET(12,50) &&
			 lpObj->pInventory[8].m_Type != ITEMGET(13,37) &&
			 (lpObj->pInventory[7].m_Type < ITEMGET(12,0) || lpObj->pInventory[7].m_Type > ITEMGET(12,6)) ) ||			 
#if (CRYSTAL_EDITION == 1)
			(lpObj->pInventory[7].m_Type < ITEMGET(12,36) || (lpObj->pInventory[7].m_Type > ITEMGET(12,40) && (lpObj->pInventory[7].m_Type < ITEMGET(12,200) || lpObj->pInventory[7].m_Type > ITEMGET(12,254))))  ||
#else
			(lpObj->pInventory[7].m_Type < ITEMGET(12,36) || lpObj->pInventory[7].m_Type > ITEMGET(12,40))  ||
#endif
			(lpObj->pInventory[7].m_Type < ITEMGET(12,130) || lpObj->pInventory[7].m_Type > ITEMGET(12,135))  ||
			 lpObj->pInventory[8].m_Type == ITEMGET(13,2) ||
			 lpObj->pInventory[11].m_Type == ITEMGET(13,10) ||
			 lpObj->pInventory[10].m_Type == ITEMGET(13,10)  )
		{
			return FALSE;
		}

		if ( lpObj->m_RecallMon >= 0 )
		{
			GCRecallMonLife(lpObj->m_Index, 60, 0);
			gObjMonsterCallKill(lpObj->m_Index);
		}
	}

	if(Raklion.WarpCheck(lpObj->m_Index,lpObj->MapNumber,iTargetMapNumber) == 0)
		return FALSE;

	return TRUE;
}





BOOL CMoveCommand::CheckInterfaceToMove(LPOBJ lpObj)
{
	if ( lpObj->m_IfState.use == 1 )
	{
		return FALSE;
	}

	if ( lpObj->m_bPShopOpen == true )
	{
		return FALSE;
	}

	return TRUE;
}




int  CMoveCommand::FindIndex(LPSTR mapname)
{
	for ( int i=0;i<MAX_MOVE_COMMAND;i++)
	{
		if ( this->m_MoveCommandData[i].Index == -1 )
		{
			return -1;
		}

		if ( strcmp(mapname, this->m_MoveCommandData[i].Name) == 0 || strcmpi(mapname, this->m_MoveCommandData[i].EngName) == 0  )
		{
			return i;
		}
	}

	return -1;
}

int  CMoveCommand::FindPosByIndex(int Index)
{
	for ( int i=0;i<MAX_MOVE_COMMAND;i++)
	{
		if ( this->m_MoveCommandData[i].Index == -1 )
		{
			return -1;
		}

		if ( this->m_MoveCommandData[i].Index == Index  )
		{
			return i;
		}
	}

	return -1;
}



BOOL CMoveCommand::CheckMoveMapBound(int iMapIndex)
{
	switch ( iMapIndex )
	{
		case 1:	case 2:	case 3:	case 7:	case 13:
			return TRUE;
	}
	
	return FALSE;
}

BOOL CheckMoveCryWolf(char * str)
{
	char buf[100]={0};
	wsprintf(buf,"%s",str);
	int i=0;
	char c;
	while (str[i])
	{
		c = str[i];
		buf[i] = (char)(tolower(c));
		i++;
	}
	if(strstr(buf,"crywolf")!= 0)
		return TRUE;
	return FALSE;
}

BOOL CheckMoveKalima(char * str)
{
	char buf[100]={0};
	wsprintf(buf,"%s",str);
	int i=0;
	char c;
	while (str[i])
	{
		c = str[i];
		buf[i] = (char)(tolower(c));
		i++;
	}
	if(strstr(buf,"kalima")!= 0)
		return TRUE;
	return FALSE;
}

BOOL CMoveCommand::Move(LPOBJ lpObj, int MapIndex)
{
	int index = this->FindPosByIndex(MapIndex);

	if ( g_bUseMoveMapBound == TRUE )
	{
		if ( lpObj->m_iPeriodItemEffectIndex == -1 )
		{
			if ( !this->CheckMoveMapBound(index))
			{
				return FALSE;
			}
		}
	}

	/*if ( index == 23 )
	{
		if ( g_CastleSiegeSync.GetCastleState() == 7 )
		{
			return FALSE;
		}

		if ( g_CastleSiegeSync.CheckCastleOwnerMember(lpObj->m_Index) == FALSE && g_CastleSiegeSync.CheckCastleOwnerUnionMember(lpObj->m_Index) == FALSE)
		{
			return FALSE;
		}
	}*/

	if ( index == -1 )
	{
		return FALSE;
	}

	int NeedLevel = this->m_MoveCommandData[index].NeedLevel;
	int NeedZen   = this->m_MoveCommandData[index].NeedZen;
	int GateNumber= this->m_MoveCommandData[index].GateNumber;

	if(ReadConfig.S5E2 == 1)
	{
		if(CheckMoveKalima(this->m_MoveCommandData[index].EngName) == TRUE)
		{
			if(lpObj->Vip != 1)
			{
				GCServerMsgStringSend("Only VIP can use that MOVE.",lpObj->m_Index, 0x01);
				return false;
			}
		}
	}

	if (lpObj->Class == CLASS_MAGICGLADIATOR || 
		lpObj->Class == CLASS_DARKLORD ||
		lpObj->Class == CLASS_RAGEFIGHTER)
	{
		if ( NeedLevel > 0 )
		{
			NeedLevel = NeedLevel *2 / 3;
		}
	}

#if (PACK_EDITION>=3)
	if(lpObj->RaceCheck == 1)
	{
		BotRacer.ResetTime(lpObj->m_Index);
		GCServerMsgStringSend(lMsg.Get(MSGGET(14, 158)) ,lpObj->m_Index, 0x00);
	}
#endif

	if ( GateNumber >= 0 )
	{
		if ( NeedLevel <= lpObj->Level )
		{
			if ( (lpObj->Money - NeedZen ) >= 0 )
			{
				//CheckMoveCryWolf

				if ( gObjMoveGate(lpObj->m_Index, GateNumber ) != FALSE )
				{
					lpObj->m_bPShopRedrawAbs = true;
					lpObj->Money -= NeedZen;

					LogAddTD("[%s][%s] Use [%s %s] Success! Reduces money: %d(%d) NeedLevel: %d"
						, lpObj->AccountID, lpObj->Name,
						lMsg.Get(MSGGET(11, 185)), this->m_MoveCommandData[index].EngName, lpObj->Money, NeedZen, NeedLevel);

					GCMoneySend(lpObj->m_Index, lpObj->Money );
					return TRUE;
				}
			}
			else
			{
				::GCServerMsgStringSend(lMsg.Get(MSGGET(4, 102)), lpObj->m_Index, 1);
			}
		}
		else
		{
			char szTemp[256];

			wsprintf(szTemp, lMsg.Get(MSGGET(4, 93)), NeedLevel);
			::GCServerMsgStringSend(szTemp, lpObj->m_Index, 1);
		}
	}

	LogAddTD("[%s][%s] Use [%s %s] Fail GateNumber = %d",
		lpObj->AccountID, lpObj->Name,
		lMsg.Get(MSGGET(11, 185)), this->m_MoveCommandData[index].EngName, GateNumber);

	return FALSE;

}

BOOL CMoveCommand::Move(LPOBJ lpObj, LPSTR mapname)
{/*
	if(ReadConfig.WareSystem > 0)
		if(lpObj->WarehouseTimeCount > 0)
			::GCServerMsgStringSend("You must wait before warp", lpObj->m_Index, 1);*/

	int index = this->FindIndex(mapname);

	if ( g_bUseMoveMapBound == TRUE )
	{
		if ( lpObj->m_iPeriodItemEffectIndex == -1 )
		{
			if ( !this->CheckMoveMapBound(index))
			{
				return FALSE;
			}
		}
	}

	/*if ( index == 23 )
	{
		if ( g_CastleSiegeSync.GetCastleState() == 7 )
		{
			return FALSE;
		}

		if ( g_CastleSiegeSync.CheckCastleOwnerMember(lpObj->m_Index) == FALSE && g_CastleSiegeSync.CheckCastleOwnerUnionMember(lpObj->m_Index) == FALSE)
		{
			return FALSE;
		}
	}*/

	if ( index == -1 )
	{
		return FALSE;
	}

	int NeedLevel = this->m_MoveCommandData[index].NeedLevel;
	int NeedZen   = this->m_MoveCommandData[index].NeedZen;
	int GateNumber= this->m_MoveCommandData[index].GateNumber;

	
	if(ReadConfig.S5E2 == 1)
	{
		if(CheckMoveKalima(this->m_MoveCommandData[index].EngName) == TRUE)
		{
			if(lpObj->Vip != 1)
			{
				GCServerMsgStringSend("Only VIP can use that MOVE.",lpObj->m_Index, 0x01);
				return false;
			}
		}
	}

	if (lpObj->Class == CLASS_MAGICGLADIATOR || 
		lpObj->Class == CLASS_DARKLORD ||
		lpObj->Class == CLASS_RAGEFIGHTER)
	{
		if ( NeedLevel > 0 )
		{
			NeedLevel = NeedLevel *2 / 3;
		}
	}

	if ( GateNumber >= 0 )
	{
		if ( NeedLevel <= lpObj->Level )
		{
			if ( (lpObj->Money - NeedZen ) >= 0 )
			{
				if ( gObjMoveGate(lpObj->m_Index, GateNumber ) != FALSE )
				{
					lpObj->m_bPShopRedrawAbs = true;
					lpObj->Money -= NeedZen;

					LogAddTD("[%s][%s] Use [%s %s] Success! Reduces money: %d(%d) NeedLevel: %d"
						, lpObj->AccountID, lpObj->Name,
						lMsg.Get(MSGGET(11, 185)), mapname, lpObj->Money, NeedZen, NeedLevel);

					GCMoneySend(lpObj->m_Index, lpObj->Money );
					return TRUE;
				}
			}
			else
			{
				::GCServerMsgStringSend(lMsg.Get(MSGGET(4, 102)), lpObj->m_Index, 1);
			}
		}
		else
		{
			char szTemp[256];

			wsprintf(szTemp, lMsg.Get(MSGGET(4, 93)), NeedLevel);
			::GCServerMsgStringSend(szTemp, lpObj->m_Index, 1);
		}
	}

	LogAddTD("[%s][%s] Use [%s %s] Fail GateNumber = %d",
		lpObj->AccountID, lpObj->Name,
		lMsg.Get(MSGGET(11, 185)), mapname, GateNumber);

	return FALSE;

}






int  GetKalimaGateLevel(LPOBJ lpObj)
{
	if (lpObj->Class == CLASS_MAGICGLADIATOR || 
		lpObj->Class == CLASS_DARKLORD ||
		lpObj->Class == CLASS_RAGEFIGHTER)
	{
		for ( int i=0;i<KALIMA_FLOORS;i++)
		{
			if ( lpObj->Level >= g_sttKALIMAGATE_LEVEL[i].iLOWER_BOUND_MAGUMSA && lpObj->Level <= g_sttKALIMAGATE_LEVEL[i].iUPPER_BOUND_MAGUMSA )
			{
				return i;
			}
		}
	}
	else
	{
		for ( int i=0;i<KALIMA_FLOORS;i++)
		{
			if ( lpObj->Level >= g_sttKALIMAGATE_LEVEL[i].iLOWER_BOUND && lpObj->Level <= g_sttKALIMAGATE_LEVEL[i].iUPPER_BOUND )
			{
				return i;
			}
		}
	}

	return -1;
}




BOOL CMoveCommand::MoveFree2Kalima(LPOBJ lpObj)
{
	int iIdx = GetKalimaGateLevel(lpObj);

	if ( iIdx >= 6 )
		iIdx = 5;

	if ( iIdx < 0 )
	{
		return FALSE;
	}

	int iGateNumber = iIdx + 88;

	return gObjMoveGate(lpObj->m_Index, iGateNumber);
}