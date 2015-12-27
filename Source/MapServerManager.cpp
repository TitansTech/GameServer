#include "stdafx.h"
#include "MapServerManager.h"
#include "MapClass.h"
#include "..\include\readscript.h"
#include "GameMain.h"
#include "LogProc.h"
#include "user.h"
#include "IllusionTemple.h"

// GS-N 0.99.60T 0x0051BF30
//	GS-N	1.00.18	JPN	0x0054D8C0	-	Completed


_MAPSVR_DATA::_MAPSVR_DATA()
{
	this->Clear(1);
}

_MAPSVR_DATA::~_MAPSVR_DATA()
{
	return;
}

void _MAPSVR_DATA::Clear(int iInitSetVal)
{
	this->m_bIN_USE = FALSE;
	this->m_btMAPSVR_GROUP = -1;
	this->m_sSVR_CODE = -1;
	memset(this->m_cIPADDR, 0, sizeof(this->m_cIPADDR));
	this->m_wPORT = 0;

	for ( int iMAP_COUNT =0;iMAP_COUNT<MAX_MAP_NUMBER;iMAP_COUNT++)
	{
		switch ( iInitSetVal )
		{
			case -1:
				this->m_sMAP_MOVE[iMAP_COUNT] = -2;
				break;
			case 0:
				this->m_sMAP_MOVE[iMAP_COUNT] = -1;
				break;
			default:
				this->m_sMAP_MOVE[iMAP_COUNT] = -3;
		}
	}
}




CMapServerManager::CMapServerManager()
{
	srand(time(NULL)); 
	this->m_bMapDataLoadOk=FALSE;
	this->m_lpThisMapSvrData = NULL;

	InitializeCriticalSection(&this->m_critSVRCODE_MAP);
}


CMapServerManager::~CMapServerManager()
{
	DeleteCriticalSection(&this->m_critSVRCODE_MAP);
}


void CMapServerManager::Clear()
{
	this->m_bMapDataLoadOk = FALSE;

	for ( int iGROUP_COUNT=0;iGROUP_COUNT<MAX_MAP_GROUPS;iGROUP_COUNT++)
	{
		this->m_iMAPSVR_GROUP_COUNT[iGROUP_COUNT] = 0;

		for ( int iSUB_GROUP_COUNT=0;iSUB_GROUP_COUNT<MAX_MAP_SUBGROUPS;iSUB_GROUP_COUNT++)
		{
			this->m_MAPSVR_DATA[iGROUP_COUNT][iSUB_GROUP_COUNT].Clear(1);
		}
	}

	this->m_mapSVRCODE_MAP.clear();
}


/*

Data           :   ebp Relative, [0xffffffe4], Object Ptr, Type: class CMapServerManager * const, this
Data           :   ebp Relative, [0x00000008], Param, Type: char *, lpszFileName
Block          :   static, [0x0014dd90][0x0001:0x0014cd90], len = 00000841, (none)
Data           :     ebp Relative, [0xffffff9c], Local, Type: class std::_Tree<int,std::pair<int const ,_MAPSVR_DATA *>,std::map<int,_MAPSVR_DATA *,std::less<int>,std::allocator<_MAPSVR_DATA *> >::_Kfn,std::less<int>,std::allocator<_MAPSVR_DATA *> >::iterator, it
Data           :     ebp Relative, [0xffffffdc], Local, Type: enum SMDToken, Token
Data           :     ebp Relative, [0xffffffe0], Local, Type: int, type
Block          :     static, [0x0014de38][0x0001:0x0014ce38], len = 00000319, (none)
Data           :       ebp Relative, [0xffffffd8], Local, Type: short, sSVR_CODE
Data           :       ebp Relative, [0xffffffd0], Local, Type: int, iInitSetVal
Data           :       ebp Relative, [0xffffffb8], Local, Type: struct _MAPSVR_DATA *, lpMapSvrData
Data           :       ebp Relative, [0xffffffc0], Local, Type: char[0x10], szIpAddr
Data           :       ebp Relative, [0xffffffd4], Local, Type: short, sMAPSVR_GROUP
Data           :       ebp Relative, [0xffffffbc], Local, Type: unsigned short, wPortNum
Block          :     static, [0x0014e160][0x0001:0x0014d160], len = 00000398, (none)
Data           :       ebp Relative, [0xffffffb0], Local, Type: unsigned char, btNotMoveOption
Data           :       ebp Relative, [0xffffffa0], Local, Type: class std::_Tree<int,std::pair<int const ,_MAPSVR_DATA *>,std::map<int,_MAPSVR_DATA *,std::less<int>,std::allocator<_MAPSVR_DATA *> >::_Kfn,std::less<int>,std::allocator<_MAPSVR_DATA *> >::iterator, it
Data           :       ebp Relative, [0xffffffb4], Local, Type: short, sSVR_CODE
Data           :       ebp Relative, [0xffffffa4], Local, Type: struct _MAPSVR_DATA *, lpMapSvrData
Data           :       ebp Relative, [0xffffffa8], Local, Type: short, sDEST_SVR_CODE
Data           :       ebp Relative, [0xffffffac], Local, Type: unsigned short, wMapNum
*/
BOOL CMapServerManager::LoadData(char* lpszFileName)
{
	//DebugLog("%s START",__FUNCTION__);
	if ( (lpszFileName == NULL) || ( strcmp(lpszFileName, "")== 0 ) ) 
	{
		MsgBox("[MapServerMng] CMapServerManager::LoadData() - file load error : File Name Error");
		
		//DebugLog("%s END",__FUNCTION__);
		return FALSE;
	}

	EnterCriticalSection(&this->m_critSVRCODE_MAP);

	try
	{
		this->Clear();

		SMDFile = fopen(lpszFileName, "r");

		if ( SMDFile == NULL )
		{
			MsgBox("[MapServerMng] CMapServerManager::LoadData() - file load error : fopen() : %s",
				lpszFileName);

			//DebugLog("%s END",__FUNCTION__);
			return FALSE;
		}

		int type = -1;
		SMDToken Token;

		while ( true )
		{
			Token = GetToken();

			if ( Token == END )
				break;

			type = TokenNumber;

			while ( true )
			{
				if ( type == 0 )
				{
					short sSVR_CODE = -1;
					short sMAPSVR_GROUP = -1;
					int iInitSetVal = 1;
					char szIpAddr[16]={0};
					WORD wPortNum = 0;
					
					Token = GetToken();

					if ( !strcmp("end", TokenString))
						break;

					sSVR_CODE = TokenNumber;

					Token = GetToken();
					sMAPSVR_GROUP = TokenNumber;

					Token = GetToken();
					iInitSetVal = TokenNumber;

					Token = GetToken();
					memcpy(szIpAddr, &TokenString[1], 16);
					szIpAddr[15] =0;

					Token = GetToken();
					wPortNum = TokenNumber;

					if ( sSVR_CODE < 0 )
					{
						MsgBox("[MapServerMng] CMapServerManager::LoadData() - file load error : sSVR_CODE < 0 (SVR:%d) - 1",
							sSVR_CODE);

						return FALSE;
					}

					if ( iInitSetVal != -1 && iInitSetVal != 0 && iInitSetVal != 1 )
					{
						MsgBox("[MapServerMng] CMapServerManager::LoadData() - file load error : iInitSetting Value:%d (SVR:%d) - 1",
							iInitSetVal, sSVR_CODE);

						//DebugLog("%s END",__FUNCTION__);
						return FALSE;
					}

					if ( !strcmp(szIpAddr, ""))
					{
						MsgBox("[MapServerMng] CMapServerManager::LoadData() - file load error : No IpAddress (SVR:%d)",
							sSVR_CODE);

						//DebugLog("%s END",__FUNCTION__);
						return FALSE;
					}

					if ( CHECK_LIMIT(sMAPSVR_GROUP, MAX_MAP_GROUPS) == FALSE )
					{
						MsgBox("[MapServerMng] CMapServerManager::LoadData() - file load error : Map Server Group Index (IDX:%d)",
							sMAPSVR_GROUP);

						//DebugLog("%s END",__FUNCTION__);
						return FALSE;
					}

					if ( this->m_iMAPSVR_GROUP_COUNT[sMAPSVR_GROUP] >= MAX_MAP_SUBGROUPS )
					{
						MsgBox("[MapServerMng] CMapServerManager::LoadData() - file load error : No Space to Save SvrInfo (SVR:%d)",
							sSVR_CODE);

						//DebugLog("%s END",__FUNCTION__);
						return FALSE;
					}

					_MAPSVR_DATA * lpMapSvrData = NULL;

					lpMapSvrData= &this->m_MAPSVR_DATA[sMAPSVR_GROUP][this->m_iMAPSVR_GROUP_COUNT[sMAPSVR_GROUP]];

					lpMapSvrData->Clear(iInitSetVal);
					lpMapSvrData->m_bIN_USE = TRUE;
					lpMapSvrData->m_sSVR_CODE = sSVR_CODE;
					lpMapSvrData->m_btMAPSVR_GROUP = sMAPSVR_GROUP;
					lpMapSvrData->m_wPORT = wPortNum;
					memcpy(lpMapSvrData->m_cIPADDR, szIpAddr, 16);
					lpMapSvrData->m_cIPADDR[15] = 0;
					
					this->m_mapSVRCODE_MAP.insert(std::pair<int,_MAPSVR_DATA *>(sSVR_CODE, lpMapSvrData));
					this->m_iMAPSVR_GROUP_COUNT[sMAPSVR_GROUP]++;
				}
				else if ( type == 1 )
				{
					short sSVR_CODE = -1;
					BYTE btNotMoveOption = 0;
					WORD wMapNum = 0;
					short sDEST_SVR_CODE = -1;
					_MAPSVR_DATA * lpMapSvrData = NULL;

					Token = GetToken();

					if ( !strcmp("end", TokenString))
						break;

					sSVR_CODE = TokenNumber;

					Token = GetToken();
					btNotMoveOption = TokenNumber;

					Token = GetToken();
					wMapNum = TokenNumber;

					Token = GetToken();
					sDEST_SVR_CODE = TokenNumber;

					if ( sSVR_CODE < 0 )
					{
						MsgBox("[MapServerMng] CMapServerManager::LoadData() - file load error : sSVR_CODE < 0 (SVR:%d) - 2",
							sSVR_CODE);

						//DebugLog("%s END",__FUNCTION__);
						return FALSE;
					}

					if ( sDEST_SVR_CODE < -2 )
					{
						MsgBox("[MapServerMng] CMapServerManager::LoadData() - file load error : sDEST_SVR_CODE < -1 (SVR:%d, DEST_SVR:%d) - 2",
							sSVR_CODE, sDEST_SVR_CODE);

						//DebugLog("%s END",__FUNCTION__);
						return FALSE;
					}

					std::map<int  ,_MAPSVR_DATA *>::iterator it = this->m_mapSVRCODE_MAP.find(sSVR_CODE);

					if ( it == this->m_mapSVRCODE_MAP.end() )
					{
						MsgBox("[MapServerMng] CMapServerManager::LoadData() - file load error : sSVR_CODE wasn't registered (SVR:%d)",
							sSVR_CODE);

						//DebugLog("%s END",__FUNCTION__);
						return FALSE;
					}

					lpMapSvrData = it->second;

					if ( lpMapSvrData == NULL )
					{
						MsgBox("[MapServerMng] CMapServerManager::LoadData() - file load error : lpMapSvrData == NULL (SVR:%d)",
							sSVR_CODE);

						//DebugLog("%s END",__FUNCTION__);
						return FALSE;
					}

					if ( lpMapSvrData->m_bIN_USE == FALSE )
					{
						MsgBox("[MapServerMng] CMapServerManager::LoadData() - file load error : lpMapSvrData->m_bIN_USE == FALSE (SVR:%d)",
							sSVR_CODE);

						//DebugLog("%s END",__FUNCTION__);
						return FALSE;
					}

					if ( lpMapSvrData->m_sSVR_CODE != sSVR_CODE )
					{
						MsgBox("[MapServerMng] CMapServerManager::LoadData() - file load error : lpMapSvrData->m_sSVR_CODE != sSVR_CODE (SVR:%d)",
							sSVR_CODE);

						//DebugLog("%s END",__FUNCTION__);
						return FALSE;
					}

					if ( btNotMoveOption != 0 && btNotMoveOption != 1 )
					{
						MsgBox("[MapServerMng] CMapServerManager::LoadData() - file load error : lpMapSvrData->m_sSVR_CODE != sSVR_CODE (SVR:%d, OPT:%d)",
							sSVR_CODE, btNotMoveOption);

						//DebugLog("%s END",__FUNCTION__);
						return FALSE;
					}

					if ( CHECK_LIMIT(wMapNum, MAX_MAP_NUMBER) == FALSE )
					{
						MsgBox("[MapServerMng] CMapServerManager::LoadData() - file load error : Map Number is out of bound (SVR:%d, MAP:%d)",
							sSVR_CODE, wMapNum);

						//DebugLog("%s END",__FUNCTION__);
						return FALSE;
					}

					switch ( btNotMoveOption )
					{
						case 0:
							lpMapSvrData->m_sMAP_MOVE[wMapNum] = sDEST_SVR_CODE;
							break;
						case 1:
							lpMapSvrData->m_sMAP_MOVE[wMapNum] = -3;
							break;
						default:
							MsgBox("[MapServerMng] CMapServerManager::LoadData() - file load error : lpMapSvrData->m_sSVR_CODE != sSVR_CODE (SVR:%d, OPT:%d)",
								sSVR_CODE, btNotMoveOption);
							break;
					}
				}	// if type
			}	// while ( true )
		}	// while ( true )

		fclose(SMDFile);

		std::map<int  ,_MAPSVR_DATA *>::iterator it = this->m_mapSVRCODE_MAP.find(gGameServerCode);

		if ( it != this->m_mapSVRCODE_MAP.end() )
		{
			this->m_lpThisMapSvrData = it->second;
		}
		else
		{
			this->m_lpThisMapSvrData = NULL;
		}

		if ( this->m_lpThisMapSvrData == NULL )
		{
			MsgBox("[MapServerMng] CMapServerManager::LoadData() - file load error : This GameServerCode (%d) doesn't Exist at file '%s' != sSVR_CODE",
				gGameServerCode, lpszFileName);

			//DebugLog("%s END",__FUNCTION__);
			return FALSE;
		}

		this->m_bMapDataLoadOk = TRUE;
	}
	catch(LPSTR ex)
	{
		LeaveCriticalSection(&this->m_critSVRCODE_MAP);
	}

	//DebugLog("%s END",__FUNCTION__);
	return TRUE;
}



BOOL CMapServerManager::CheckMapCanMove(int iMAP_NUM)
{
	if ( MapNumberCheck(iMAP_NUM) == 0 )
	{
		return FALSE;
	}

	_MAPSVR_DATA * lpMapSvrData = this->m_lpThisMapSvrData;

	if ( lpMapSvrData == NULL )
	{
		return FALSE;
	}

	if ( lpMapSvrData->m_bIN_USE == FALSE )
	{
		return FALSE;
	}

	short sMAP_MOVE_INFO = lpMapSvrData->m_sMAP_MOVE[iMAP_NUM];

	if ( sMAP_MOVE_INFO != -3 )
	{
		return FALSE;
	}

	return TRUE;
}





short CMapServerManager::CheckMoveMapSvr(int iIndex, int iMAP_NUM, short sSVR_CODE_BEFORE)
{
	//DebugLog("%s START",__FUNCTION__);
	if ( this->m_bMapDataLoadOk == FALSE )
	{		
		//DebugLog("%s END this->m_bMapDataLoadOk == FALSE",__FUNCTION__);
		return gGameServerCode;
	}

	if ( !gObjIsConnected(iIndex) )
	{		
		//DebugLog("%s END !gObjIsConnected(iIndex)",__FUNCTION__);
		return gGameServerCode;
	}

	if ( !MapNumberCheck(iMAP_NUM) )
	{
		LogAddC(2, "[MapServerMng] CheckMoveMapSvr() - Map Index doesn't exist [%s][%s] : %d",
			gObj[iIndex].AccountID, gObj[iIndex].Name, iMAP_NUM);

		//DebugLog("%s END !MapNumberCheck(iMAP_NUM)",__FUNCTION__);
		return gGameServerCode;
	}

	if (IT_MAP_RANGE(gObj[iIndex].MapNumber) != FALSE && gObj[iIndex].MapNumber != iMAP_NUM)
	{
		IllusionTemple.Player_RemoveByID(iIndex);
	}

	_MAPSVR_DATA * lpMapSvrData = this->m_lpThisMapSvrData;

	if ( lpMapSvrData == NULL )
	{
		LogAddC(2, "[MapServerMng] CheckMoveMapSvr() - m_lpThisMapSvrData == NULL [%s][%s] : %d",
			gObj[iIndex].AccountID, gObj[iIndex].Name, iMAP_NUM);

		//DebugLog("%s END lpMapSvrData == NULL",__FUNCTION__);
		return gGameServerCode;
	}

	if ( lpMapSvrData->m_bIN_USE == FALSE )
	{
		LogAddC(2, "[MapServerMng] CheckMoveMapSvr() - lpMapSvrData->m_bIN_USE == FALSE [%s][%s] : %d",
			gObj[iIndex].AccountID, gObj[iIndex].Name, iMAP_NUM);

		//DebugLog("%s END lpMapSvrData->m_bIN_USE == FALSE",__FUNCTION__);
		return gGameServerCode;
	}

	short sMAP_MOVE_INFO = lpMapSvrData->m_sMAP_MOVE[iMAP_NUM];

	switch ( sMAP_MOVE_INFO )
	{
		case -1:
			{
				_MAPSVR_DATA * lpDestMapSvrData = NULL;

				if ( sSVR_CODE_BEFORE != -1 )
				{
					//EnterCriticalSection(&this->m_critSVRCODE_MAP);
					std::map<int, _MAPSVR_DATA *>::iterator it = this->m_mapSVRCODE_MAP.find(sSVR_CODE_BEFORE);

					if ( it != this->m_mapSVRCODE_MAP.end() )
					{
						lpDestMapSvrData = it->second;
					}
					//LeaveCriticalSection(&this->m_critSVRCODE_MAP);

					if ( lpDestMapSvrData != NULL )
					{
						if ( lpDestMapSvrData->m_sMAP_MOVE[iMAP_NUM] == -3 )
						{
							//DebugLog("%s END",__FUNCTION__);
							return sSVR_CODE_BEFORE;
						}
					}
				}

				std::vector<_MAPSVR_DATA *> vtMapSvrData;

				//EnterCriticalSection(&this->m_critSVRCODE_MAP);

				for ( std::map<int ,_MAPSVR_DATA *>::iterator it = this->m_mapSVRCODE_MAP.begin() ; it != this->m_mapSVRCODE_MAP.end() ;it++)
				{
					_MAPSVR_DATA * lpTempMapSvrData = it->second;

					if ( lpTempMapSvrData != NULL &&
						 lpTempMapSvrData->m_bIN_USE == TRUE &&
						 lpTempMapSvrData->m_sMAP_MOVE[iMAP_NUM] == -3)
					{
						vtMapSvrData.push_back(it->second);
					}
				}

				//LeaveCriticalSection(&this->m_critSVRCODE_MAP);

				short sDestServerCode = -1;

				if ( vtMapSvrData.empty() == 0 )
				{
					sDestServerCode = vtMapSvrData[rand()%vtMapSvrData.size()]->m_sSVR_CODE;
				}

				if ( sDestServerCode != -1 )
				{
					LogAddTD("[MapServerMng] CheckMoveMapSvr() - MapServer Check OK [%s][%s] : MAP-%d / SVR-%d(State Map:%d X:%d Y:%d)",
						gObj[iIndex].AccountID, gObj[iIndex].Name, iMAP_NUM, sDestServerCode,
						gObj[iIndex].MapNumber, gObj[iIndex].X, gObj[iIndex].Y);

					//DebugLog("%s END",__FUNCTION__);
					return sDestServerCode;
				}
			}
			break;

		case -2:
			{
				_MAPSVR_DATA * lpDestMapSvrData = NULL;

				if ( sSVR_CODE_BEFORE != -1 )
				{
					//EnterCriticalSection(&this->m_critSVRCODE_MAP);
					std::map<int, _MAPSVR_DATA *>::iterator it = this->m_mapSVRCODE_MAP.find(sSVR_CODE_BEFORE);

					if ( it != this->m_mapSVRCODE_MAP.end() )
					{
						lpDestMapSvrData = it->second;
					}
					//LeaveCriticalSection(&this->m_critSVRCODE_MAP);

					if ( lpDestMapSvrData != NULL )
					{
						if ( lpDestMapSvrData->m_sMAP_MOVE[iMAP_NUM] == -3 )
						{
							//DebugLog("%s END",__FUNCTION__);
							return sSVR_CODE_BEFORE;
						}
					}
				}
				
				short sDestServerCode = -1;

				//EnterCriticalSection(&this->m_critSVRCODE_MAP);

				for ( std::map<int ,_MAPSVR_DATA *>::iterator it = this->m_mapSVRCODE_MAP.begin() ; it != this->m_mapSVRCODE_MAP.end() ;it++)
				{
					_MAPSVR_DATA * lpTempMapSvrData = it->second;

					if ( lpTempMapSvrData != NULL &&
						 lpTempMapSvrData->m_bIN_USE == TRUE &&
						 lpTempMapSvrData->m_sMAP_MOVE[iMAP_NUM] == -3)
					{
						sDestServerCode = lpTempMapSvrData->m_sSVR_CODE;
					}
				}

				//LeaveCriticalSection(&this->m_critSVRCODE_MAP);

				if ( sDestServerCode != -1 )
				{
					LogAddTD("[MapServerMng] CheckMoveMapSvr() - MapServer Check OK [%s][%s] : MAP-%d / SVR-%d(State Map:%d X:%d Y:%d)",
						gObj[iIndex].AccountID, gObj[iIndex].Name, iMAP_NUM, sDestServerCode,
						gObj[iIndex].MapNumber, gObj[iIndex].X, gObj[iIndex].Y);

					//DebugLog("%s END",__FUNCTION__);
					return sDestServerCode;
				}
			}
			break;

		case -3:
			{
				LogAddTD("[MapServerMng] CheckMoveMapSvr() - MapServer Check OK [%s][%s] : MAP-%d / SVR-%d (State Map:%d X:%d Y:%d)",
					gObj[iIndex].AccountID, gObj[iIndex].Name, iMAP_NUM, gGameServerCode,
					gObj[iIndex].MapNumber, gObj[iIndex].X, gObj[iIndex].Y);

				//DebugLog("%s END",__FUNCTION__);
				return gGameServerCode;
			}break;

		default:
			if ( sMAP_MOVE_INFO > -1 )
			{
				_MAPSVR_DATA * lpDestMapSvrData = NULL;

				//EnterCriticalSection(&this->m_critSVRCODE_MAP);

				std::map<int, _MAPSVR_DATA *>::iterator it = this->m_mapSVRCODE_MAP.find(sMAP_MOVE_INFO);

				if ( it != this->m_mapSVRCODE_MAP.end() )
				{
					lpDestMapSvrData = it->second;
				}

				//LeaveCriticalSection(&this->m_critSVRCODE_MAP);

				if ( lpDestMapSvrData != NULL &&
					 lpDestMapSvrData->m_bIN_USE == TRUE &&
					 lpDestMapSvrData->m_sMAP_MOVE[iMAP_NUM] == -3)
				{					
					//DebugLog("%s END",__FUNCTION__);
					return sMAP_MOVE_INFO;
				}

				short sDestServerCode = -1;

				//EnterCriticalSection(&this->m_critSVRCODE_MAP);

				for ( it = this->m_mapSVRCODE_MAP.begin() ; it != this->m_mapSVRCODE_MAP.end() ;it++)
				{
					_MAPSVR_DATA * lpTempMapSvrData = it->second;

					if ( lpTempMapSvrData != NULL &&
						 lpTempMapSvrData->m_bIN_USE == TRUE &&
						 lpTempMapSvrData->m_sMAP_MOVE[iMAP_NUM] == -3)
					{
						sDestServerCode = lpTempMapSvrData->m_sSVR_CODE;
					}
				}
				
				//LeaveCriticalSection(&this->m_critSVRCODE_MAP);

				if ( sDestServerCode != -1 )
				{
					LogAddTD("[MapServerMng] CheckMoveMapSvr() - MapServer Check OK [%s][%s] : MAP-%d / SVR-%d(State Map:%d X:%d Y:%d)",
						gObj[iIndex].AccountID, gObj[iIndex].Name, iMAP_NUM, sDestServerCode,
						gObj[iIndex].MapNumber, gObj[iIndex].X, gObj[iIndex].Y);

					//DebugLog("%s END",__FUNCTION__);
					return sDestServerCode;
				}
			}
			else
			{
				LogAddC(2, "[MapServerMng] CheckMoveMapSvr() - Unknown MapMove Info [%s][%s] : MAP-%d / INFO-%d",
					gObj[iIndex].AccountID, gObj[iIndex].Name, iMAP_NUM, sMAP_MOVE_INFO);

				//DebugLog("%s END",__FUNCTION__);
				return gGameServerCode;
			}
	}

	//DebugLog("%s END",__FUNCTION__);
	return gGameServerCode;
}




BOOL CMapServerManager::GetSvrCodeData(WORD wServerCode, char* lpszIpAddress, WORD * lpwPort)
{
	//DebugLog("%s START",__FUNCTION__);
	if ( !lpszIpAddress || !lpwPort )
	{		
		//DebugLog("%s END",__FUNCTION__);
		return FALSE;
	}

	_MAPSVR_DATA * lpMapSvrData = NULL;

	EnterCriticalSection(&this->m_critSVRCODE_MAP);

	std::map<int, _MAPSVR_DATA *>::iterator it = this->m_mapSVRCODE_MAP.find(wServerCode);

	if ( it != this->m_mapSVRCODE_MAP.end() )
	{
		lpMapSvrData = it->second;
	}

	LeaveCriticalSection(&this->m_critSVRCODE_MAP);

	if ( lpMapSvrData == NULL )
	{		
		//DebugLog("%s END",__FUNCTION__);
		return FALSE;
	}

	strcpy(lpszIpAddress, lpMapSvrData->m_cIPADDR);
	*lpwPort = lpMapSvrData->m_wPORT;

	
	//DebugLog("%s END",__FUNCTION__);
	return TRUE;
}




	
CMapServerManager g_MapServerManager;