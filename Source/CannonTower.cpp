// CannonTower.cpp: implementation of the CCannonTower class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GameServer.h"
#include "CannonTower.h"
#include "user.h"

//#include "..\include\readscript.h"
#include "..\common\winutil.h"

#if (GS_CASTLE==1)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCannonTower g_CsNPC_CannonTower;

CCannonTower::CCannonTower()
{

}

CCannonTower::~CCannonTower()
{

}

void CCannonTower::CannonTowerAct(int iIndex) // 0x00560940  1.00.19
{	
	if(ReadConfig.S5E2 == FALSE)
	{
		if( ( rand() % 2 ) != 0 ) 	// review this
		{
			return;
		}
		LPOBJ lpObj = &gObj[iIndex];
		int count = 0;
		int ASBOfs = 0;
		PMSG_BEATTACK_COUNT pCount;
		PMSG_BEATTACK pAttack;

		pCount.h.c = 0xC1;
		pCount.h.headcode = 0x10;
		pCount.h.size = 0x00;
		pCount.MagicNumberH = 0;
		pCount.MagicNumberL = 0;
		pCount.Count = 0x00;
		pCount.X = lpObj->X;
		pCount.Y = lpObj->Y;

		ASBOfs = sizeof(PMSG_BEATTACK_COUNT);

		unsigned char AttackSendBuff[256];

		while( true ) 
		{
			if( lpObj->VpPlayer2[count].state ) 
			{
				if( lpObj->VpPlayer2[count].type == OBJ_USER ) 
				{
					int tObjNum = lpObj->VpPlayer2[count].number;

					if( tObjNum >= 0 ) 
					{
						if( gObj[tObjNum].m_btCsJoinSide ) 
						{
							if( gObjCalDistance(lpObj, &gObj[tObjNum]) < 7 ) 
							{
								pAttack.NumberH = SET_NUMBERH(tObjNum);
								pAttack.NumberL = SET_NUMBERL(tObjNum);
								memcpy( (AttackSendBuff+ASBOfs), (PBYTE)&pAttack, sizeof(PMSG_BEATTACK));
								ASBOfs += sizeof(PMSG_BEATTACK);
								pCount.Count++;
							}
						}
					}
				}
			}
			count++;

			if( count > MAX_VIEWPORT-1 ) break;
		}
		if( pCount.Count > 0 ) 
		{
			pCount.h.size = ASBOfs;

			memcpy(AttackSendBuff, (LPBYTE)&pCount, sizeof(PMSG_BEATTACK_COUNT));
			CGBeattackRecv(AttackSendBuff, lpObj->m_Index, 1);
			struct PMSG_DURATION_MAGIC_SEND 
			{
			  /*<thisrel this+0x0>*/ /*|0x3|*/ struct PBMSG_HEAD h;
				unsigned char MagicNumberH;
			  /*<thisrel this+0x3>*/ /*|0x1|*/ unsigned char MagicNumberL;
			  /*<thisrel this+0x4>*/ /*|0x1|*/ unsigned char NumberH;
			  /*<thisrel this+0x5>*/ /*|0x1|*/ unsigned char NumberL;
			  /*<thisrel this+0x6>*/ /*|0x1|*/ unsigned char X;
			  /*<thisrel this+0x7>*/ /*|0x1|*/ unsigned char Y;
			  /*<thisrel this+0x8>*/ /*|0x1|*/ unsigned char Dir;
			};
			DWORD Skill = 0x32;
			PMSG_DURATION_MAGIC_SEND pMsg;
			PHeadSetBE((PBYTE)&pMsg,0x1E,sizeof(pMsg));
			pMsg.MagicNumberH = SET_NUMBERH(Skill);
			pMsg.MagicNumberL = SET_NUMBERL(Skill);
			pMsg.X = lpObj->X;
			pMsg.Y = lpObj->Y;
			pMsg.Dir = lpObj->Dir;
			pMsg.NumberH = SET_NUMBERH(iIndex);
			pMsg.NumberL = SET_NUMBERL(iIndex);
			::MsgSendV2(lpObj, (PBYTE)&pMsg, pMsg.h.size);

		}
	}else
	{
		if( ( rand() % 2 ) != 0 ) 	// review this
		{
			return;
		}
		LPOBJ lpObj = &gObj[iIndex];
		int count = 0;
		int ASBOfs = 0;
		PMSG_BEATTACK_COUNT_S5E2 pCount;
		PMSG_BEATTACK_S5E2 pAttack;

		pCount.h.c = 0xC1;
		pCount.h.headcode = 0x10;
		pCount.h.size = 0x00;
		pCount.MagicNumberH = 0;
		pCount.MagicNumberL = 0;
		pCount.Count = 0x00;
		pCount.X = lpObj->X;
		pCount.Y = lpObj->Y;

		ASBOfs = sizeof(PMSG_BEATTACK_COUNT_S5E2);

		unsigned char AttackSendBuff[256];

		while( true ) 
		{
			if( lpObj->VpPlayer2[count].state ) 
			{
				if( lpObj->VpPlayer2[count].type == OBJ_USER ) 
				{
					int tObjNum = lpObj->VpPlayer2[count].number;

					if( tObjNum >= 0 ) 
					{
						if( gObj[tObjNum].m_btCsJoinSide ) 
						{
							if( gObjCalDistance(lpObj, &gObj[tObjNum]) < 7 ) 
							{
								pAttack.NumberH = SET_NUMBERH(tObjNum);
								pAttack.NumberL = SET_NUMBERL(tObjNum);
								memcpy( (AttackSendBuff+ASBOfs), (PBYTE)&pAttack, sizeof(PMSG_BEATTACK));
								ASBOfs += sizeof(PMSG_BEATTACK);
								pCount.Count++;
							}
						}
					}
				}
			}
			count++;

			if( count > MAX_VIEWPORT-1 ) break;
		}
		if( pCount.Count > 0 ) 
		{
			pCount.h.size = ASBOfs;

			memcpy(AttackSendBuff, (LPBYTE)&pCount, sizeof(PMSG_BEATTACK_COUNT_S5E2));
			CGBeattackRecv(AttackSendBuff, lpObj->m_Index, 1);
			struct PMSG_DURATION_MAGIC_SEND 
			{
			  /*<thisrel this+0x0>*/ /*|0x3|*/ struct PBMSG_HEAD h;
				unsigned char MagicNumberH;
			  /*<thisrel this+0x3>*/ /*|0x1|*/ unsigned char MagicNumberL;
			  /*<thisrel this+0x4>*/ /*|0x1|*/ unsigned char NumberH;
			  /*<thisrel this+0x5>*/ /*|0x1|*/ unsigned char NumberL;
			  /*<thisrel this+0x6>*/ /*|0x1|*/ unsigned char X;
			  /*<thisrel this+0x7>*/ /*|0x1|*/ unsigned char Y;
			  /*<thisrel this+0x8>*/ /*|0x1|*/ unsigned char Dir;
			};
			DWORD Skill = 0x32;
			PMSG_DURATION_MAGIC_SEND pMsg;
			PHeadSetBE((PBYTE)&pMsg,0x1E,sizeof(pMsg));
			pMsg.MagicNumberH = SET_NUMBERH(Skill);
			pMsg.MagicNumberL = SET_NUMBERL(Skill);
			pMsg.X = lpObj->X;
			pMsg.Y = lpObj->Y;
			pMsg.Dir = lpObj->Dir;
			pMsg.NumberH = SET_NUMBERH(iIndex);
			pMsg.NumberL = SET_NUMBERL(iIndex);
			::MsgSendV2(lpObj, (PBYTE)&pMsg, pMsg.h.size);

		}
	}
}

#endif