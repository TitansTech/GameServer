// ------------------------------
// Decompiled by Deathway
// Date : 2007-05-09
// ------------------------------
// GS-N 0.99.60T 0x004B1200
/*	GS-N	1.00.18	JPN	0x004CF4C0	-	Completed
	BOOL gObjJoominCheck(int aIndex, char* szInJN)	-	strcmpi comflict symbols
	void gObjGiveItemSearch(LPOBJ lpObj, int maxlevel)	-	Complete just for size
	void gObjGiveItemWarehouseSearch(LPOBJ lpObj, int maxlevel)	-	Completes just for size
	void InitFrustrum()	-	Wrong Symbol NAmes of the zzmath functions
	void SkillFrustrum()	-	Wrong Symbol NAmes of the zzmath functions
*/
#include "stdafx.h"
#include "user.h"
#include "logproc.h"
#include "GameServer.h"
#include "GameMain.h"
#include "MapServerManager.h"
#include "gObjMonster.h"
#include "ObjCalCharacter.h"
#include "ObjUseSkill.h"
#include "DarkSpirit.h"
#include "DSProtocol.h"
#include "DevilSquare.h"
#include "protocol.h"
#include "AcceptIp.h"
#include "BattleGround.h"
#include "MonsterAttr.h"
#include "ChaosBox.h"
#include "PacketChecksum.h"
#include "QuestInfo.h"
#include "EledoradoEvent.h"
#include "TNotice.h"
#include "GMMng.h"
#include "zzzmathlib.h"
#include "Gate.h"
#include "ObjAttack.h"
#include "SProtocol.h"
#include "MultiAttackHAckCheck.h"
#include "BattleSoccer.h"
#include "BattleSoccerManager.h"
#include "TUnion.h"
#include "TMonsterAIElement.h"
#include "..\common\winutil.h"
#include "Weapon.h"
#include "Crywolf.h"
#include "CrywolfSync.h"
#include "Kanturu.h"
#include "Event.h"
#include "CashShop.h"
#include "Mercenary.h"
#include "KalimaGate.h"
#include "KanturuBattleUserMng.h"
#include "Guardian.h"
#include "SProtocol.h"
#include "MoveCommand.h"
#include "ItemAddOption.h"
#include "StatMng.h"
#include "..\common\SetItemOption.h"
#include "SkillAdditionInfo.h"
#include "CashItemPeriodSystem.h"
#include "TMonsterSkillManager.h"
#include "IllusionTemple.h"
#include "Mastering.h"
#include "Mastering2.h"
#include "SCFExDBProtocol.h"
#include "HappyHour.h"
#include "HitAndUp.h"
#include "ResetSystem.h"
#include "SkyEvent.h"
#include "HalloweenEvent.h"
#include "XMasEvent.h"

#include "CastleCrown.h"
#include "CastleCrownSwitch.h"
#include "LifeStone.h"
#include "GuardianStatue.h"
#include "CannonTower.h"
#include "DuelManager.h"
#include "CustomQuest.h"
#include "AutoExp.h"
#include "CastleSiegeSync.h"
#include "ImperialGuardian.h"
#include "SwampEvent.h"
#include "Raklion.h"
#include "PCShop.h"
#include "DoppelGanger.h"
#include "XMasEvent.h"
#include "ObjBotPet.h"
#include "SCFPvPSystem.h"
#include "GensPvP.h"
#include "ChildWindows.h"
#include "GensCloseMap.h"
#include "SCFVipShop.h"
#include "QuestS5Info.h"
#include "MuItemShop.h"

#include "VIPSystem.h"
#include "CrywolfAltar.h"
#include "ObjSkillAutoLearn.h"
#include "DSGN_ItemMover.h"
#include "ObjBotRacer.h"
#include "CrystalWall.h"

#include "LogToFile.h"
CLogToFile ANTI_HACK_LOG("ANTI_HACK_LOG", ".\\ANTI_HACK_LOG", 1);	// line : 68

CLogToFile DSGN_ITEMMOVE_LOG("DSGN_ITEMMOVE_LOG", ".\\DSGN_ITEMFIX_LOG", 1);	// line : 68
CLogToFile DSGN_ITEMFIX_LOG("DSGN_ITEMFIX_LOG", ".\\DSGN_ITEMFIX_LOG", 1);	// line : 68
CLogToFile DSGN_RINGFIX_LOG("DSGN_RINGFIX_LOG", ".\\DSGN_ITEMFIX_LOG", 1);	// line : 68
CLogToFile DSGN_WINGFIX_LOG("DSGN_WINGFIX_LOG", ".\\DSGN_ITEMFIX_LOG", 1);	// line : 68

CLogToFile TRADE_LOG("TRADE_LOG", ".\\TRADE_LOG", 1);	// line : 68
CLogToFile CHAOS_LOG("CHAOS_LOG", ".\\CHAOS_LOG", 1);	// line : 68
CLogToFile JEWEL_MIX_LOG("JEWEL_MIX_LOG", ".\\CHAOS_LOG", 1);	// line : 68

CLogToFile WZ_ITEMSHOP_LOG("WZ_ITEMSHOP_LOG", ".\\WZ_ITEMSHOP_LOG", 1);	// line : 68
CLogToFile PLAYER_ITEMS_LOG("PLAYER_ITEMS_LOG", ".\\PLAYER_ITEMS_LOG", 1);	// line : 68

CLogToFile ITEMBAG_DROP_LOG("ITEMBAG_DROP_LOG", ".\\ITEMBAG_DROP_LOG", 1);	// line : 68

extern CLogToFile GM_LOG;


#define Num_ViewSkillState		47
char GlTmp[512];
char GlTmp2[512];
// directions

//Y
//|-------
//||6|5|4|
//||7|9|3|
//||8|1|2|
//|------- X



int ChangeCount; 
int lOfsChange;
int GuildUserCount;
int GuildUserOfs;
int GuildInfoCount;
int GuildInfoOfs;
//int skillSuccess;
//int gCurConnectUser;
int gDisconnect;
int gObjTotalUser;
//int gItemLoopMax;
//int gItemLoop;
//int gItemLoop2;
//int gObjCSFlag;

#include "ChaosCastle.h"
#include "..\include\readscript.h"


CViewportGuild ViewGuildMng;	// line : 130



OBJECTSTRUCT gObj[OBJMAX];	// line : 180
int gObjCount;
int gObjMonCount;
int gObjCallMonCount;




MessageStateMachine gSMMsg[OBJMAX][MAX_MONSTER_SEND_MSG];	
ExMessageStateMachine gSMAttackProcMsg[OBJMAX][MAX_MONSTER_SEND_ATTACK_MSG];


BILL_CLASS m_ObjBill[OBJMAX];	// line : 193



HANDLE hThread_gObjMove;

struct PWMSG_SUBCODE_COUNT {

  // static data ------------------------------------

  // non-static data --------------------------------
  /*<thisrel this+0x0>*/ /*|0x4|*/ struct PWMSG_HEAD h;
  /*<thisrel this+0x4>*/ /*|0x1|*/ unsigned char subcode;
  /*<thisrel this+0x5>*/ /*|0x1|*/ unsigned char count;
 //YTE unk1;
 
};

struct PWMSG_COUNT {

  // static data ------------------------------------

  // non-static data --------------------------------
  /*<thisrel this+0x0>*/ /*|0x4|*/ struct PWMSG_HEAD h;
  /*<thisrel this+0x4>*/ /*|0x1|*/ unsigned char count;
 //YTE unk1;
 
};
// <size 0x5>
struct PBMSG_COUNT {

  // static data ------------------------------------

  // non-static data --------------------------------
  /*<thisrel this+0x0>*/ /*|0x3|*/ struct PBMSG_HEAD h;
  /*<thisrel this+0x3>*/ /*|0x1|*/ unsigned char count;

  // base classes -----------------------------------

  // friends ----------------------------------------

  // static functions -------------------------------

  // non-virtual functions --------------------------

  // virtual functions ------------------------------
};
// <size 0x4>

struct PMSG_ITEMVIEWPORTCREATE {

  // static data ------------------------------------

  // non-static data --------------------------------
  /*<thisrel this+0x0>*/ /*|0x1|*/ unsigned char NumberH;
  /*<thisrel this+0x1>*/ /*|0x1|*/ unsigned char NumberL;
  /*<thisrel this+0x2>*/ /*|0x1|*/ unsigned char px;
  /*<thisrel this+0x3>*/ /*|0x1|*/ unsigned char py;
  /*<thisrel this+0x4>*/ /*|0x7|*/ unsigned char ItemInfo[7+5];

  // base classes -----------------------------------

  // friends ----------------------------------------

  // static functions -------------------------------

  // non-virtual functions --------------------------

  // virtual functions ------------------------------
};
// <size 0xb>

int SkillState_GS[20]		=	{1		,2		,4		,8		,16		,32		,64		,128	,256	,512	,1024	,2048	,4096	,8192	,16384	,32768	,65536	,131072	,1048576	,2097152};
int SkillState_Effect[20]	=	{0x37	,0x38	,0x01	,0x02	,0x08	,0x39	,0x3A	,0x3F	,0x04	,0x13	,0x06	,0x05	,0x3D	,0x09	,0x12	,0x0A	,0x0B	,0x03	,0x00		,eVS_INVISIBLE};

int SkillState_CWGS[5]		=	{0x00000,0x10000,0x20000,0x40000,0x80000};
int SkillState_CWEffect[5]	=	{0x16	,0x15	,0x19	,0x17	,0x18};

struct PMSG_VIEWPORTCREATE {

  // static data ------------------------------------

  // non-static data --------------------------------
  /*<thisrel this+0x0>*/ /*|0x1|*/ unsigned char NumberH;
  /*<thisrel this+0x1>*/ /*|0x1|*/ unsigned char NumberL;
  /*<thisrel this+0x2>*/ /*|0x1|*/ unsigned char X;
  /*<thisrel this+0x3>*/ /*|0x1|*/ unsigned char Y;
  /*<thisrel this+0x4>*/ /*|0x12|*/ unsigned char CharSet[18];
  ///*<thisrel this+0x18>*/ /*|0x4|*/ unsigned long ViewSkillState;
  /*<thisrel this+0x1c>*/ /*|0xa|*/ char Id[10];
  /*<thisrel this+0x26>*/ /*|0x1|*/ unsigned char TX;
  /*<thisrel this+0x27>*/ /*|0x1|*/ unsigned char TY;
  /*<thisrel this+0x28>*/ /*|0x1|*/ unsigned char DirAndPkLevel;
  ///*<thisrel this+0x18>*/ /*|0x4|*/ unsigned long ViewSkillState;
  unsigned char SkillStateCount;
  /*<thisrel this+0x18>*/ /*|0x4|*/ unsigned char ViewSkillState[Num_ViewSkillState];
  // base classes -----------------------------------

  // friends ----------------------------------------

  // static functions -------------------------------

  // non-virtual functions --------------------------

  // virtual functions ------------------------------
};
// <size 0x2c>

struct PMSG_VIEWPORTCREATE_CHANGE {
  // static data ------------------------------------

  // non-static data --------------------------------
  /*<thisrel this+0x0>*/ /*|0x1|*/ unsigned char NumberH;
  /*<thisrel this+0x1>*/ /*|0x1|*/ unsigned char NumberL;
  /*<thisrel this+0x2>*/ /*|0x1|*/ unsigned char X;
  /*<thisrel this+0x3>*/ /*|0x1|*/ unsigned char Y;
  /*<thisrel this+0x4>*/ /*|0x1|*/ unsigned char SkinH;
  /*<thisrel this+0x5>*/ /*|0x1|*/ unsigned char SkinL;
  /*<thisrel this+0x8>*/ /*|0x4|*/ //int ViewSkillState;
  /*<thisrel this+0xc>*/ /*|0xa|*/ char Id[10];
  /*<thisrel this+0x16>*/ /*|0x1|*/ unsigned char TX;
  /*<thisrel this+0x17>*/ /*|0x1|*/ unsigned char TY;
  /*<thisrel this+0x18>*/ /*|0x1|*/ unsigned char DirAndPkLevel;
									unsigned char CharSet[18]; //for illusion temple
  unsigned char SkillStateCount;
  unsigned char ViewChangeSkillState[Num_ViewSkillState];
  // base classes -----------------------------------

  // friends ----------------------------------------

  // static functions -------------------------------

  // non-virtual functions --------------------------

  // virtual functions ------------------------------
};
// <size 0x1c>

// <size 0x2E>

struct PMSG_VIEWPORTCREATE_GENS
{
	BYTE Family;
	BYTE NumberH;
	BYTE NumberL;
	BYTE Unk1[3];
	WORD Unk2;
	BYTE Level;
	BYTE Unk3[7];
};

struct PMSG_VIEWPORTDESTROY {

  // static data ------------------------------------

  // non-static data --------------------------------
  /*<thisrel this+0x0>*/ /*|0x1|*/ unsigned char NumberH;
  /*<thisrel this+0x1>*/ /*|0x1|*/ unsigned char NumberL;

  // base classes -----------------------------------

  // friends ----------------------------------------

  // static functions -------------------------------

  // non-virtual functions --------------------------

  // virtual functions ------------------------------
};
// <size 0x2>

struct PMSG_MONSTER_VIEWPORTCREATE {
  // static data ------------------------------------

  // non-static data --------------------------------
  /*<thisrel this+0x0>*/ /*|0x1|*/ unsigned char NumberH; //6
  /*<thisrel this+0x1>*/ /*|0x1|*/ unsigned char NumberL; 
  /*<thisrel this+0x2>*/ /*|0x1|*/ unsigned char Type_HI; 
  /*<thisrel this+0x3>*/ /*|0x1|*/ unsigned char Type_LO; 

  /*<thisrel this+0x8>*/ /*|0x1|*/ unsigned char X;  
  /*<thisrel this+0x9>*/ /*|0x1|*/ unsigned char Y; 
  /*<thisrel this+0xa>*/ /*|0x1|*/ unsigned char TX; 
  /*<thisrel this+0xb>*/ /*|0x1|*/ unsigned char TY;  
  /*<thisrel this+0xc>*/ /*|0x1|*/ unsigned char Path; 
	  ///*<thisrel this+0x4>*/ /*|0x4|*/ unsigned long ViewState;
  unsigned char SkillStateCount;
  /*<thisrel this+0x18>*/ /*|0x4|*/ unsigned char ViewSkillState[Num_ViewSkillState];

  // base classes -----------------------------------

  // friends ----------------------------------------

  // static functions -------------------------------

  // non-virtual functions --------------------------

  // virtual functions ------------------------------
};
// <size 0x10>

struct PMSG_CALLMONSTER_VIEWPORTCREATE {
  // static data ------------------------------------

  // non-static data --------------------------------
  /*<thisrel this+0x0>*/ /*|0x1|*/ unsigned char NumberH;
  /*<thisrel this+0x1>*/ /*|0x1|*/ unsigned char NumberL;
  /*<thisrel this+0x2>*/ /*|0x1|*/ unsigned char Type_HI;
  /*<thisrel this+0x3>*/ /*|0x1|*/ unsigned char Type_LO;
  ///*<thisrel this+0x4>*/ /*|0x4|*/ 
  /*<thisrel this+0x8>*/ /*|0x1|*/ unsigned char X;
  /*<thisrel this+0x9>*/ /*|0x1|*/ unsigned char Y;
  /*<thisrel this+0xa>*/ /*|0x1|*/ unsigned char TX;
  /*<thisrel this+0xb>*/ /*|0x1|*/ unsigned char TY;
  /*<thisrel this+0xc>*/ /*|0x1|*/ unsigned char Path;
  /*<thisrel this+0xd>*/ /*|0xa|*/ unsigned char Id[10];
									int ViewState; // 
  // base classes -----------------------------------

  // friends ----------------------------------------

  // static functions -------------------------------

  // non-virtual functions --------------------------

  // virtual functions ------------------------------
};
// <size 0x18>

struct PMSG_GUILDVIEWPORT_USER {

  // static data ------------------------------------

  // non-static data --------------------------------
  /*<thisrel this+0x0>*/ /*|0x1|*/ unsigned char NumberH;
  /*<thisrel this+0x1>*/ /*|0x1|*/ unsigned char NumberL;
  /*<thisrel this+0x2>*/ /*|0x1|*/ unsigned char GNumberH;
  /*<thisrel this+0x3>*/ /*|0x1|*/ unsigned char GNumberL;

  // base classes -----------------------------------

  // friends ----------------------------------------

  // static functions -------------------------------

  // non-virtual functions --------------------------

  // virtual functions ------------------------------
};
// <size 0x4>

struct PMSG_GUILDVIEWPORT {

  // static data ------------------------------------

  // non-static data --------------------------------
  /*<thisrel this+0x0>*/ /*|0x1|*/ unsigned char NumberH;
  /*<thisrel this+0x1>*/ /*|0x1|*/ unsigned char NumberL;
  /*<thisrel this+0x2>*/ /*|0x8|*/ char GuildName[8];
  /*<thisrel this+0xa>*/ /*|0x20|*/ unsigned char Mark[32];

  // base classes -----------------------------------

  // friends ----------------------------------------

  // static functions -------------------------------

  // non-virtual functions --------------------------

  // virtual functions ------------------------------
};
// <size 0x2a>

BYTE GuildInfoBuf[10000];
BYTE GuildUserBuf[10000];
BYTE SendGBufChange[5000];

PMSG_VIEWPORTCREATE pViewportCreate;
PMSG_VIEWPORTCREATE_CHANGE pViewportCreateChange;
PMSG_VIEWPORTCREATE_GENS pViewportCreateGens;
PMSG_VIEWPORTDESTROY pViewportDestroy;
PMSG_MONSTER_VIEWPORTCREATE pMonsterViewportCreate;
PMSG_CALLMONSTER_VIEWPORTCREATE pCallMonsterViewportCreate;

#pragma pack(1)

PWMSG_COUNT pItemCount;
PMSG_ITEMVIEWPORTCREATE pItemViewportCreate;
PMSG_VIEWPORTDESTROY pItemViewportDestroy;

#pragma pack()



short RoadPathTable[MAX_ROAD_PATH_TABLE] = { -1, -1, 0, -1, 1, -1, 1, 0, 1, 1, 0, 1, -1, 1, -1, 0 };	// THIS VARIABLE HAS SOME STRANGE THINGS ONT ITS UBICATION
int  gServerMaxUser=500;	// THIS VARIABLE HAS SOME STRANGE THINGS ONT ITS UBICATION
//BOOL g_EnergyCheckOff=1; // THIS VARIABLE HAS SOME STRANGE THINGS ONT ITS UBICATION

int ViewPortMonsterSkillStates(LPOBJ lpObj, PMSG_MONSTER_VIEWPORTCREATE * pViewportCreate)
{
	int baseCount=0;
	int EffectCounter=0;

	if (lpObj->m_ViewSkillState>0)
	{
		while ((EffectCounter<sizeof(SkillState_GS))&&(SkillState_GS[EffectCounter]<=lpObj->m_ViewSkillState))
		{
			int ANDResult=0;
			ANDResult=lpObj->m_ViewSkillState & SkillState_GS[EffectCounter];

			if (ANDResult==SkillState_GS[EffectCounter])
			{
				pViewportCreate->ViewSkillState[baseCount] = SkillState_Effect[EffectCounter];
				baseCount+=1;
			}
			EffectCounter+=1;
		}	
	}

	#if (GS_CASTLE==1)
	if ((lpObj->Class>=205)&&(lpObj->Class<=209))
	{
		EffectCounter=0;
		while ((EffectCounter<sizeof(SkillState_CWGS))&&(SkillState_CWGS[EffectCounter]<=lpObj->m_ViewSkillState))
		{
			//Increase baseCount+=1; for proper counter increment
			//Increase effectsCount+=1; for proper new position of the effect in packet
			if (lpObj->m_ViewSkillState==SkillState_CWGS[EffectCounter])
			{
				pViewportCreate->ViewSkillState[baseCount] = SkillState_CWEffect[EffectCounter];
				baseCount+=1;
			}
			EffectCounter+=1;
		}
	} else if (lpObj->MapNumber==34) {
		if (lpObj->m_ViewSkillState==0x10000000)
		{
			pViewportCreate->ViewSkillState[baseCount] = 0x1B;	//Make NPC Invisible
			baseCount+=1;
		}
	}

	//Castle Gate
	if( lpObj->Class == 277 )
	{
		if( lpObj->m_btCsGateOpen > 0 )
		{
			pViewportCreate->ViewSkillState[baseCount] = 0x0D;
			baseCount+=1;
		}
	}

	//Castle Crown
	if( lpObj->Class == 216 )
	{
		//Could Base on the Switches
		if( g_CastleSiege.GetRegCrownAvailable() == 1 )
		{
			pViewportCreate->ViewSkillState[baseCount] = 0x14;
			baseCount+=1;
		}
	}
	#endif

	for(int i=0;i<2;i++)
	{
		switch(lpObj->m_wItemEffectType[i])
		{
			case (ITEMGET(13,43)):
			{
				pViewportCreate->ViewSkillState[baseCount] = 29;
				baseCount++;
			}break;
			case (ITEMGET(13,44)):
			{
				pViewportCreate->ViewSkillState[baseCount] = 30;
				baseCount++;
			}break;
			case (ITEMGET(13,45)):
			{
				pViewportCreate->ViewSkillState[baseCount] = 31;
				baseCount++;
			}break;


			case (ITEMGET(13,81)):
			case (ITEMGET(13,80)):
			{
				BYTE Val = (lpObj->m_wItemEffectType[i] - ITEMGET(13,81)) + 99;
				pViewportCreate->ViewSkillState[baseCount] = Val;
				baseCount++;
			}break;

			case (ITEMGET(14,97)):
			case (ITEMGET(14,98)):
			{
				BYTE Val = (lpObj->m_wItemEffectType[i] - ITEMGET(14,97)) + 89;
				pViewportCreate->ViewSkillState[baseCount] = Val;
				baseCount++;
			}break;
			
			case (ITEMGET(14,72)):
			case (ITEMGET(14,73)):
			case (ITEMGET(14,74)):
			case (ITEMGET(14,75)):
			case (ITEMGET(14,76)):
			case (ITEMGET(14,77)):
			case (ITEMGET(14,78)):
			case (ITEMGET(14,79)):
			case (ITEMGET(14,80)):
			case (ITEMGET(14,81)):
			case (ITEMGET(14,82)):
			{
				BYTE Val = (lpObj->m_wItemEffectType[i] - ITEMGET(14,72)) + 44;
				pViewportCreate->ViewSkillState[baseCount] = Val;
				baseCount++;
			}break;
			
			case (ITEMGET(13,128)):
			case (ITEMGET(13,129)):
			case (ITEMGET(13,130)):
			case (ITEMGET(13,131)):
			case (ITEMGET(13,132)):
			case (ITEMGET(13,133)):
			case (ITEMGET(13,134)):
			{
				BYTE Val = (lpObj->m_wItemEffectType[i] - ITEMGET(13,128)) + 122;
				pViewportCreate->ViewSkillState[baseCount] = Val;
				baseCount++;
			}break;
		}
	}

	if (lpObj->MapNumber == MAP_INDEX_SWAMP)
	{
		if(lpObj->m_Attribute == 63 && (lpObj->Class < 557 || lpObj->Class > 561) )
		{
			pViewportCreate->ViewSkillState[baseCount] = 71;
			baseCount++;		
		}
	}

	
	if(lpObj->m_Invisible == 1)
	{
		pViewportCreate->ViewSkillState[baseCount] = eVS_INVISIBLE;
		baseCount++;
	}

	if(lpObj->Class == 523)
	{
		pViewportCreate->ViewSkillState[baseCount] = eVS_INVISIBLE;
		baseCount++;		
	}

	if((lpObj->AuthorityCode == 0x3F) || (lpObj->Authority >= 8))
	{
		if(lpObj->MapNumber != 30 && IT_MAP_RANGE(lpObj->MapNumber) == FALSE)
		{
#if (PACK_EDITION>=2)
			if(HalloweenEvent.Enabled == 1 && HalloweenEvent.Start == true && HalloweenEvent.CheckMap(lpObj->MapNumber) == true)
			{
			}else
#endif
			{
				pViewportCreate->ViewSkillState[baseCount] = 28;
				baseCount++;
			}
		}

	}
	if(lpObj->m_SkillReflectTime > 0)
	{
		pViewportCreate->ViewSkillState[baseCount] = 71;
		baseCount++;
	}
	if(lpObj->m_SkillMagicCircleTime > 0)
	{
		pViewportCreate->ViewSkillState[baseCount] = 82;
		baseCount++;
	}
	if(lpObj->m_SkillRedArmorIgnoreTime > 0)
	{
		pViewportCreate->ViewSkillState[baseCount] = 129;
		baseCount++;
	}
	if(lpObj->m_SkillFitnessTime > 0)
	{
		pViewportCreate->ViewSkillState[baseCount] = 130;
		baseCount++;
	}
	if(lpObj->m_SkillDefSuccessRateIncTime > 0)
	{
		pViewportCreate->ViewSkillState[baseCount] = 131;
		baseCount++;
	}
	if(lpObj->m_SkillReduceDamageTime > 0)
	{
		pViewportCreate->ViewSkillState[baseCount] = 76;
		baseCount++;
	}
	if(lpObj->m_SkillReduceDefenseTime > 0)
	{
		pViewportCreate->ViewSkillState[baseCount] = 77;
		baseCount++;
	}
	if(lpObj->m_RFBufReduceDefenseTime > 0)
	{
		pViewportCreate->ViewSkillState[baseCount] = 164;
		baseCount++;
	}

	if(lpObj->m_SkillGladiatorsGloryTime > 0)
	{
		pViewportCreate->ViewSkillState[baseCount] = eVS_DUEL_MEDAL;
		baseCount++;
	}
	if(lpObj->m_SkillSleep > 0)
	{
		pViewportCreate->ViewSkillState[baseCount] = 72;
		baseCount++;
	}
	if(lpObj->m_SkillNightTime > 0)
	{
		pViewportCreate->ViewSkillState[baseCount] = 73;
		baseCount++;
	}
	if(lpObj->m_SkillSummonerBookTime > 0)
	{
		pViewportCreate->ViewSkillState[baseCount] = lpObj->m_SkillSummonerBookTypeDmg;
		baseCount++;
	}
	if(lpObj->m_SkillExplotionTime > 0)
	{
		pViewportCreate->ViewSkillState[baseCount] = 86;
		baseCount++;
	}
	if(lpObj->m_SkillLightingStormTime > 0)
	{
		pViewportCreate->ViewSkillState[baseCount] = 84;
		baseCount++;
	}
	if(lpObj->m_SkillRedStormTime > 0)
	{
		pViewportCreate->ViewSkillState[baseCount] = 85;
		baseCount++;
	}
	if(lpObj->m_SkillSwordSlashTime > 0)
	{
		pViewportCreate->ViewSkillState[baseCount] = 83;
		baseCount++;
	}

	if(lpObj->m_SkillIT_Time > 0)
	{
		switch(lpObj->m_SkillIT_Number)
		{
			case IL_ORDER_OF_PROTECT:
			{
				pViewportCreate->ViewSkillState[baseCount] = 34;
				baseCount++;
			} break;
			case IL_ORDER_OF_FREEZE:
			{
				pViewportCreate->ViewSkillState[baseCount] = 65;
				baseCount++;
			} break;
		}
	}

	return baseCount;
}


int ViewPortChangeSkillStates(LPOBJ lpObj, PMSG_VIEWPORTCREATE_CHANGE * pViewportCreate)
{
	int baseCount=0;

	if (lpObj->m_ViewSkillState>0)
	{
		int EffectCounter=0;
		while ((EffectCounter<sizeof(SkillState_GS))&&(SkillState_GS[EffectCounter]<=lpObj->m_ViewSkillState))
		{
			int ANDResult=0;
			ANDResult=lpObj->m_ViewSkillState & SkillState_GS[EffectCounter];

			if (ANDResult==SkillState_GS[EffectCounter])
			{
				pViewportCreate->ViewChangeSkillState[baseCount] = SkillState_Effect[EffectCounter];
				baseCount+=1;
			}
			EffectCounter+=1;
		}
	}


#if (GS_CASTLE==1)
	if(g_CastleSiegeSync.GetCastleState() == 7)
	{
		switch(lpObj->m_btCsJoinSide)
		{
			case 1:
			{
				pViewportCreate->ViewChangeSkillState[baseCount] = 0x0E;
				baseCount++;
			}
			case 2:
			{
				pViewportCreate->ViewChangeSkillState[baseCount] = 0x0F;
				baseCount++;
			}
			case 3:
			{
				pViewportCreate->ViewChangeSkillState[baseCount] = 0x10;
				baseCount++;
			}
			case 4:
			{
				pViewportCreate->ViewChangeSkillState[baseCount] = 0x11;
				baseCount++;
			}
		}
	}
#endif

	for(int i=0;i<2;i++)
	{
		switch(lpObj->m_wItemEffectType[i])
		{
			case (ITEMGET(13,43)):
			{
				pViewportCreate->ViewChangeSkillState[baseCount] = 29;
				baseCount++;
			}break;
			case (ITEMGET(13,44)):
			{
				pViewportCreate->ViewChangeSkillState[baseCount] = 30;
				baseCount++;
			}break;
			case (ITEMGET(13,45)):
			{
				pViewportCreate->ViewChangeSkillState[baseCount] = 31;
				baseCount++;
			}break;


			case (ITEMGET(13,81)):
			case (ITEMGET(13,80)):
			{
				BYTE Val = (lpObj->m_wItemEffectType[i] - ITEMGET(13,81)) + 99;
				pViewportCreate->ViewChangeSkillState[baseCount] = Val;
				baseCount++;
			}break;

			case (ITEMGET(14,97)):
			case (ITEMGET(14,98)):
			{
				BYTE Val = (lpObj->m_wItemEffectType[i] - ITEMGET(14,97)) + 89;
				pViewportCreate->ViewChangeSkillState[baseCount] = Val;
				baseCount++;
			}break;
			
			case (ITEMGET(14,72)):
			case (ITEMGET(14,73)):
			case (ITEMGET(14,74)):
			case (ITEMGET(14,75)):
			case (ITEMGET(14,76)):
			case (ITEMGET(14,77)):
			case (ITEMGET(14,78)):
			case (ITEMGET(14,79)):
			case (ITEMGET(14,80)):
			case (ITEMGET(14,81)):
			case (ITEMGET(14,82)):
			{
				BYTE Val = (lpObj->m_wItemEffectType[i] - ITEMGET(14,72)) + 44;
				pViewportCreate->ViewChangeSkillState[baseCount] = Val;
				baseCount++;
			}break;
			
			case (ITEMGET(13,128)):
			case (ITEMGET(13,129)):
			case (ITEMGET(13,130)):
			case (ITEMGET(13,131)):
			case (ITEMGET(13,132)):
			case (ITEMGET(13,133)):
			case (ITEMGET(13,134)):
			{
				BYTE Val = (lpObj->m_wItemEffectType[i] - ITEMGET(13,128)) + 122;
				pViewportCreate->ViewChangeSkillState[baseCount] = Val;
				baseCount++;
			}break;
		}
	}

	if (CC_MAP_RANGE(lpObj->MapNumber) == FALSE &&
		DS_MAP_RANGE(lpObj->MapNumber) == FALSE &&
		BC_MAP_RANGE(lpObj->MapNumber) == FALSE &&
#if (PACK_EDITION>=2)
		IMPERIALGUARDIAN_MAP_RANGE(lpObj->MapNumber) == FALSE &&
#endif
#if (PACK_EDITION>=3)
		DG_MAP_RANGE(lpObj->MapNumber) == FALSE &&
#endif
		IT_MAP_RANGE(lpObj->MapNumber) == FALSE &&
		lpObj->MapNumber != 30
		)
	{
		if((lpObj->AuthorityCode == 0x3F) || (lpObj->Authority >= 8))
		{
#if (PACK_EDITION>=2)
			if(HalloweenEvent.Enabled == 1 && HalloweenEvent.Start == true && HalloweenEvent.CheckMap(lpObj->MapNumber) == true)
			{
			}else
#endif
			{
				pViewportCreate->ViewChangeSkillState[baseCount] = 28;
				baseCount++;
			}
		}
	}

	if(lpObj->m_isDuelSpectator)
	{
		pViewportCreate->ViewChangeSkillState[baseCount] = eVS_TRANSPARENCY;
		baseCount++;		
	}

	if(lpObj->m_Invisible == 1)
	{
		pViewportCreate->ViewChangeSkillState[baseCount] = eVS_INVISIBLE;
		baseCount++;
	}

	if(lpObj->m_SkillBerserkerTime > 0)
	{
		pViewportCreate->ViewChangeSkillState[baseCount] = 81;
		baseCount++;
	}

	if(lpObj->m_SkillReflectTime > 0)
	{
		pViewportCreate->ViewChangeSkillState[baseCount] = 71;
		baseCount++;
	}
	if(lpObj->m_SkillMagicCircleTime > 0)
	{
		pViewportCreate->ViewChangeSkillState[baseCount] = 82;
		baseCount++;
	}
	if(lpObj->m_SkillRedArmorIgnoreTime > 0)
	{
		pViewportCreate->ViewChangeSkillState[baseCount] = 129;
		baseCount++;
	}
	if(lpObj->m_SkillFitnessTime > 0)
	{
		pViewportCreate->ViewChangeSkillState[baseCount] = 130;
		baseCount++;
	}
	if(lpObj->m_SkillDefSuccessRateIncTime > 0)
	{
		pViewportCreate->ViewChangeSkillState[baseCount] = 131;
		baseCount++;
	}
	if(lpObj->m_SkillReduceDamageTime > 0)
	{
		pViewportCreate->ViewChangeSkillState[baseCount] = 76;
		baseCount++;
	}
	if(lpObj->m_SkillReduceDefenseTime > 0)
	{
		pViewportCreate->ViewChangeSkillState[baseCount] = 77;
		baseCount++;
	}
	if(lpObj->m_RFBufReduceDefense > 0)
	{
		pViewportCreate->ViewChangeSkillState[baseCount] = 164;
		baseCount++;
	}	
	if(lpObj->m_SkillSleep > 0)
	{
		pViewportCreate->ViewChangeSkillState[baseCount] = 72;
		baseCount++;
	}
	if(lpObj->m_SkillNightTime > 0)
	{
		pViewportCreate->ViewChangeSkillState[baseCount] = 73;
		baseCount++;
	}
	if(lpObj->m_SkillSummonerBookTime > 0)
	{
		pViewportCreate->ViewChangeSkillState[baseCount] = lpObj->m_SkillSummonerBookTypeDmg;
		baseCount++;
	}
	if(lpObj->m_SkillExplotionTime > 0)
	{
		pViewportCreate->ViewChangeSkillState[baseCount] = 86;
		baseCount++;
	}
	if(lpObj->m_SkillLightingStormTime > 0)
	{
		pViewportCreate->ViewChangeSkillState[baseCount] = 84;
		baseCount++;
	}
	if(lpObj->m_SkillRedStormTime > 0)
	{
		pViewportCreate->ViewChangeSkillState[baseCount] = 85;
		baseCount++;
	}
	if(lpObj->m_SkillSwordSlashTime > 0)
	{
		pViewportCreate->ViewChangeSkillState[baseCount] = 83;
		baseCount++;
	}

	if(lpObj->m_SkillIT_Time > 0)
	{
		switch(lpObj->m_SkillIT_Number)
		{
			case IL_ORDER_OF_PROTECT:
			{
				pViewportCreate->ViewChangeSkillState[baseCount] = 34;
				baseCount++;
			} break;
			case IL_ORDER_OF_FREEZE:
			{
				pViewportCreate->ViewChangeSkillState[baseCount] = 65;
				baseCount++;
			} break;
		}
	}

	return baseCount;
}

int ViewPortSkillStates(LPOBJ lpObj, PMSG_VIEWPORTCREATE * pViewportCreate)
{
	int baseCount=0;
	if (lpObj->m_ViewSkillState>0)
	{
		int EffectCounter=0;
		while ((EffectCounter<sizeof(SkillState_GS))&&(SkillState_GS[EffectCounter]<=lpObj->m_ViewSkillState))
		{
			int ANDResult=0;
			ANDResult=lpObj->m_ViewSkillState & SkillState_GS[EffectCounter];

			if (ANDResult==SkillState_GS[EffectCounter])
			{
				pViewportCreate->ViewSkillState[baseCount] = SkillState_Effect[EffectCounter];
				baseCount+=1;
			}
			EffectCounter+=1;
		}	
	}

	for(int i=0;i<2;i++)
	{
		switch(lpObj->m_wItemEffectType[i])
		{
			case (ITEMGET(13,43)):
			{
				pViewportCreate->ViewSkillState[baseCount] = 29;
				baseCount++;
			}break;
			case (ITEMGET(13,44)):
			{
				pViewportCreate->ViewSkillState[baseCount] = 30;
				baseCount++;
			}break;
			case (ITEMGET(13,45)):
			{
				pViewportCreate->ViewSkillState[baseCount] = 31;
				baseCount++;
			}break;


			case (ITEMGET(13,81)):
			case (ITEMGET(13,80)):
			{
				BYTE Val = (lpObj->m_wItemEffectType[i] - ITEMGET(13,81)) + 99;
				pViewportCreate->ViewSkillState[baseCount] = Val;
				baseCount++;
			}break;

			case (ITEMGET(14,97)):
			case (ITEMGET(14,98)):
			{
				BYTE Val = (lpObj->m_wItemEffectType[i] - ITEMGET(14,97)) + 89;
				pViewportCreate->ViewSkillState[baseCount] = Val;
				baseCount++;
			}break;
			
			case (ITEMGET(14,72)):
			case (ITEMGET(14,73)):
			case (ITEMGET(14,74)):
			case (ITEMGET(14,75)):
			case (ITEMGET(14,76)):
			case (ITEMGET(14,77)):
			case (ITEMGET(14,78)):
			case (ITEMGET(14,79)):
			case (ITEMGET(14,80)):
			case (ITEMGET(14,81)):
			case (ITEMGET(14,82)):
			{
				BYTE Val = (lpObj->m_wItemEffectType[i] - ITEMGET(14,72)) + 44;
				pViewportCreate->ViewSkillState[baseCount] = Val;
				baseCount++;
			}break;
			
			case (ITEMGET(13,128)):
			case (ITEMGET(13,129)):
			case (ITEMGET(13,130)):
			case (ITEMGET(13,131)):
			case (ITEMGET(13,132)):
			case (ITEMGET(13,133)):
			case (ITEMGET(13,134)):
			{
				BYTE Val = (lpObj->m_wItemEffectType[i] - ITEMGET(13,128)) + 122;
				pViewportCreate->ViewSkillState[baseCount] = Val;
				baseCount++;
			}break;
		}
	}

	
	if((lpObj->AuthorityCode == 0x3F) || (lpObj->Authority >= 8))
	{
		if(lpObj->MapNumber != 30 && IT_MAP_RANGE(lpObj->MapNumber) == FALSE)
		{
#if (PACK_EDITION>=2)
			if(HalloweenEvent.Enabled == 1 && HalloweenEvent.Start == true && HalloweenEvent.CheckMap(lpObj->MapNumber) == true)
			{
			} else 
#endif
			{
				pViewportCreate->ViewSkillState[baseCount] = 28;
				baseCount++;
			}
		}
	}

	if(lpObj->m_isDuelSpectator)
	{
		pViewportCreate->ViewSkillState[baseCount] = eVS_TRANSPARENCY;
		baseCount++;		
	}

	if(lpObj->m_Invisible == 1)
	{
		pViewportCreate->ViewSkillState[baseCount] = eVS_INVISIBLE;
		baseCount++;
	}

	if(lpObj->m_SkillBerserkerTime > 0)
	{
		pViewportCreate->ViewSkillState[baseCount] = 81;
		baseCount++;
	}

	if(lpObj->m_SkillReflectTime > 0)
	{
		pViewportCreate->ViewSkillState[baseCount] = 71;
		baseCount++;
	}
	if(lpObj->m_SkillMagicCircleTime > 0)
	{
		pViewportCreate->ViewSkillState[baseCount] = 82;
		baseCount++;
	}
	if(lpObj->m_SkillRedArmorIgnoreTime > 0)
	{
		pViewportCreate->ViewSkillState[baseCount] = 129;
		baseCount++;
	}
	if(lpObj->m_SkillFitnessTime > 0)
	{
		pViewportCreate->ViewSkillState[baseCount] = 130;
		baseCount++;
	}
	if(lpObj->m_SkillDefSuccessRateIncTime > 0)
	{
		pViewportCreate->ViewSkillState[baseCount] = 131;
		baseCount++;
	}
	if(lpObj->m_SkillReduceDamageTime > 0)
	{
		pViewportCreate->ViewSkillState[baseCount] = 76;
		baseCount++;
	}
	if(lpObj->m_SkillReduceDefenseTime > 0)
	{
		pViewportCreate->ViewSkillState[baseCount] = 77;
		baseCount++;
	}
	if(lpObj->m_RFBufReduceDefense > 0)
	{
		pViewportCreate->ViewSkillState[baseCount] = 164;
		baseCount++;
	}	
	if(lpObj->m_SkillSleep > 0)
	{
		pViewportCreate->ViewSkillState[baseCount] = 72;
		baseCount++;
	}
	if(lpObj->m_SkillNightTime > 0)
	{
		pViewportCreate->ViewSkillState[baseCount] = 73;
		baseCount++;
	}
	if(lpObj->m_SkillSummonerBookTime > 0)
	{
		pViewportCreate->ViewSkillState[baseCount] = lpObj->m_SkillSummonerBookTypeDmg;
		baseCount++;
	}
	if(lpObj->m_SkillExplotionTime > 0)
	{
		pViewportCreate->ViewSkillState[baseCount] = 86;
		baseCount++;
	}
	if(lpObj->m_SkillLightingStormTime > 0)
	{
		pViewportCreate->ViewSkillState[baseCount] = 84;
		baseCount++;
	}
	if(lpObj->m_SkillRedStormTime > 0)
	{
		pViewportCreate->ViewSkillState[baseCount] = 85;
		baseCount++;
	}
	if(lpObj->m_SkillSwordSlashTime > 0)
	{
		pViewportCreate->ViewSkillState[baseCount] = 83;
		baseCount++;
	}
	if(lpObj->m_SkillIT_Time > 0)
	{
		switch(lpObj->m_SkillIT_Number)
		{
			case IL_ORDER_OF_PROTECT:
			{
				pViewportCreate->ViewSkillState[baseCount] = 34;
				baseCount++;
			} break;
			case IL_ORDER_OF_FREEZE:
			{
				pViewportCreate->ViewSkillState[baseCount] = 65;
				baseCount++;
			} break;
		}
	}

	#if (GS_CASTLE==1)
	//=========================================================================================
	//CASTLE SIEGE TEAMS FIX
	//=========================================================================================
	//lpObj->m_btCsJoinSide 1	= Defender Shield				0x0E
	//lpObj->m_btCsJoinSide 2	= Alliance attacker 1 Sword  	0x0F
	//lpObj->m_btCsJoinSide 3	= Alliance attacker 2 Arche  	0x10
	//lpObj->m_btCsJoinSide 4	= Alliance attacker 3 Staff  	0x11
	//=========================================================================================
	if (lpObj->m_btCsJoinSide>0)
	{
		switch (lpObj->m_btCsJoinSide)
		{
			case 0x01:
				pViewportCreate->ViewSkillState[baseCount] = 0x0E;
				baseCount++;	
			break;
			case 0x02:
				pViewportCreate->ViewSkillState[baseCount] = 0x0F;
				baseCount++;	
			break;
			case 0x03:
				pViewportCreate->ViewSkillState[baseCount] = 0x10;
				baseCount++;	
			break;
			case 0x04:
				pViewportCreate->ViewSkillState[baseCount] = 0x11;
				baseCount++;	
			break;
		}
	}
	#endif
	return baseCount;
}

struct PMSG_EX_SKILL_COUNT {

  // static data ------------------------------------

  // non-static data --------------------------------
  /*<thisrel this+0x0>*/ /*|0x3|*/ struct PBMSG_HEAD h;	// C1:BA
  /*<thisrel this+0x3>*/ /*|0x1|*/ unsigned char NumberH;
  /*<thisrel this+0x4>*/ /*|0x1|*/ unsigned char NumberL;
  /*<thisrel this+0x5>*/ /*|0x1|*/ unsigned char Type;
  /*<thisrel this+0x6>*/ /*|0x1|*/ unsigned char Count;

  // base classes -----------------------------------

  // friends ----------------------------------------

  // static functions -------------------------------

  // non-virtual functions --------------------------

  // virtual functions ------------------------------
};



void gObjSkillUseProcTime500(LPOBJ lpObj)
{
	if ( lpObj->SkillHellFire2State != 0 )
	{
		unsigned long dwCurrentTick = GetTickCount();
		lpObj->SkillHellFire2Count = (dwCurrentTick - lpObj->SkillHellFire2Time) / 500;

		if ( lpObj->SkillHellFire2Count > 12 )
		{
			lpObj->SkillHellFire2Count = 12;
		}

		CMagicInf * lpMagic = gObjGetMagicSearch(lpObj, AT_SKILL_EXPHELL);

		//int mana = MagicDamageC.SkillGetMana(AT_SKILL_EXPHELL);
		//int usebp = MagicDamageC.SkillGetBP(AT_SKILL_EXPHELL);
		int mana = gObjUseSkill.GetUseMana(lpObj->m_Index,lpMagic);
		int usebp = gObjUseSkill.GetUseBP(lpObj->m_Index,lpMagic);
		mana = mana * 20 / 100;
		usebp = usebp * 20 / 100;

		if(dwCurrentTick >= (lpObj->SkillHellFire2Time + 6000))
		{
			//CMagicInf * lpMagic = gObjGetMagicSearch(lpObj,0x28);

			if(lpMagic != 0)
			{
				gObjUseSkill.RunningSkill(lpObj->m_Index,0,lpMagic,0);
			}
		}
		else
		{
			//int mana = MagicDamageC.SkillGetMana(40);
			//mana = mana * 20 / 100;
			BOOL bSuccess = TRUE;

			if ( mana > lpObj->Mana || usebp > lpObj->BP )
			{
				bSuccess = FALSE;
				int count = 0;

				while ( true )
				{
					count++;

					if ( gObjIsConnected(lpObj) == FALSE )
					{
						LogAdd("[CHECK_LOG_INFINITY] gObjIsConnected() error %s %d", __FILE__, __LINE__);
						break;
					}

					if ( count > 100 )
					{
						LogAdd("[CHECK_LOG_INFINITY] ( _count > 100 ) error %s %d", __FILE__, __LINE__);
						break;
					}

					int pos = gObjGetManaItemPos(lpObj->m_Index);

					if ( pos == -1 )
					{
						break;
					}

					PMSG_USEITEM pMsg;
					pMsg.inventoryPos = pos;
					pMsg.invenrotyTarget = 0;
					CGUseItemRecv(&pMsg, lpObj->m_Index);

					if ( mana <= lpObj->Mana && usebp <= lpObj->BP )
					{
						bSuccess = TRUE;
						break;
					}
				}
			}
			
			if ( bSuccess == FALSE )
			{
				//CMagicInf * lpMagic = gObjGetMagicSearch(lpObj, 40);
				
				if (lpMagic != NULL )
				{
					gObjUseSkill.RunningSkill(lpObj->m_Index, 0, lpMagic, 0);
				}
			}
			else
			{
				lpObj->Mana -= mana;
				lpObj->BP -= usebp;

				if ( lpObj->BP < 0.0f )
				{
					lpObj->BP = 0.0f;
				}

				if ( lpObj->Mana < 0.0f )
				{
					lpObj->Mana = 0.0f;
				}

				GCManaSend(lpObj->m_Index, lpObj->Mana, -1, 0, lpObj->BP);
				PMSG_EX_SKILL_COUNT pMsg;
				PHeadSetB( (LPBYTE)&pMsg, 0xBA, sizeof(pMsg));
				pMsg.Type = 0x28;
				pMsg.Count = lpObj->SkillHellFire2Count;
				pMsg.NumberH = SET_NUMBERH(lpObj->m_Index);
				pMsg.NumberL = SET_NUMBERL(lpObj->m_Index);

				DataSend(lpObj->m_Index, (UCHAR *)&pMsg, pMsg.h.size);
				MsgSendV2(lpObj, (UCHAR *)&pMsg, pMsg.h.size);
			}
		}
	}
}


void MonsterAndMsgProc()
{
	LPOBJ lpObj;
	int n;
	int aIndex;

	for ( n=0;n<OBJMAX;n++)
	{
		lpObj = &gObj[n];

		if ( lpObj->Connected == PLAYER_PLAYING  )
		{
			if ( lpObj->Type == OBJ_MONSTER || lpObj->Type == OBJ_NPC) 
			{
				if(lpObj->m_iCurrentAI != 0)
				{
					continue;
				}
				gObjMonsterProcess(lpObj);
			}
			else
			{
				gObjSkillUseProcTime500(lpObj);
				gObjMsgProc(lpObj);
				CreateFrustrum(lpObj->X, lpObj->Y, n);
			}

			if ( lpObj->Type == OBJ_USER )
			{
				gDarkSpirit[n].Run();
			}
		}
		else if ( lpObj->Connected >= PLAYER_LOGGED )
		{
			if ( lpObj->Type == OBJ_USER )
			{
				gObjMsgProc(lpObj);

				if ( lpObj->Connected == PLAYER_PLAYING )
				{
					CreateFrustrum(lpObj->X, lpObj->Y, n);
				}
			}
		}
	}

	for ( n=0;n<OBJMAXUSER;n++)
	{
		lpObj = &gObj[n+OBJ_STARTUSERINDEX];

		if ( lpObj->Connected == PLAYER_PLAYING  )
		{
			aIndex = lpObj->m_Index;

			for ( int i=0;i<MAX_MONSTER_SEND_ATTACK_MSG;i++)
			{
				if ( gSMAttackProcMsg[n][i].MsgCode >= 0 )
				{
					if ( GetTickCount() > gSMAttackProcMsg[n][i].MsgTime )
					{
						gObjStateAttackProc(lpObj, gSMAttackProcMsg[n][i].MsgCode, gSMAttackProcMsg[n][i].SendUser,
							gSMAttackProcMsg[n][i].SubCode, gSMAttackProcMsg[n][i].SubCode2);
						gSMAttackProcMsg[n][i].MsgCode = -1;
					}
				}
			}
		}
	}
	TMonsterSkillManager::MonsterSkillProc();
}

void MoveMonsterProc()
{
	DWORD MoveTime;
	int DelayTime;
	LPOBJ lpObj;

	for(int n = 0; n < OBJMAX;n++)
	{
		lpObj = &gObj[n];

		if(lpObj->m_iCurrentAI != 0)
		{
			if(lpObj->Type == OBJ_MONSTER || lpObj->Type == OBJ_NPC)
			{
				continue;
			}
		}

		if(lpObj->Connected == PLAYER_PLAYING)
		{
			if(lpObj->Type == OBJ_MONSTER)
			{
				if(ATTRIBUTE_RANGE(lpObj->m_Attribute) != FALSE ||
					(lpObj->Class == 277) ||
					(lpObj->Class == 283) ||	//Guardian Statue
					(lpObj->Class == 285) ||	//Guardian
					(lpObj->Class == 288) ||
					(lpObj->Class == 278) ||
					(lpObj->Class == 216) ||
					(lpObj->Class == 217) ||
					(lpObj->Class == 218) ||
					(lpObj->Class == 219) ||
					(lpObj->Class == 460) ||						//Raklion Spider Egg
					(lpObj->Class == 461) ||						//Raklion Spider Egg
					(lpObj->Class == 462) ||						//Raklion Spider Egg
					(lpObj->Class == 523) ||						//Imperial Guardian Trap
					(lpObj->Class >= 524 && lpObj->Class <= 528) ||	//Imperial Statues, Gates
					(lpObj->Class >= 204 && lpObj->Class <= 209))	//CryWolf Altars, Statues
				{
					CreateFrustrum(lpObj->X,lpObj->Y,n);
					continue;
				}
			}
			
			if(lpObj->m_State == 2)
			{
				if(lpObj->PathCount != 0 && lpObj->m_SkillHarden == 0 && lpObj->m_iSkillStunTime == 0)
				{
					if(lpObj->DelayLevel != 0)
					{
						DelayTime = 300;
					}
					else
					{
						DelayTime = 0;
					}
	
					if(lpObj->Type == OBJ_MONSTER && lpObj->m_RecallMon >= 100)
					{
						lpObj->m_MoveSpeed = 200;
					}
					else
					{
						lpObj->m_MoveSpeed = 400;
					}
	
					if(lpObj->PathDir[lpObj->PathCur]%2 == 0)
					{
						MoveTime = (lpObj->m_MoveSpeed + DelayTime)*(double)1.3;
					}
					else
					{
						MoveTime = lpObj->m_MoveSpeed + DelayTime;
					}
	
					if( (GetTickCount() - lpObj->PathTime) > MoveTime && lpObj->PathCur < 14)
					{
						int nextX = lpObj->PathX[lpObj->PathCur];
						int nextY = lpObj->PathY[lpObj->PathCur];
						BYTE mapnumber = lpObj->MapNumber;
						BYTE attr = MapC[mapnumber].GetAttr(nextX,nextY);
	
						if( lpObj->Type == OBJ_USER && ( (attr & 4) == 4 || (attr & 8) == 8) )
						{
							LogAddTD("[Check Position] MoveMonsterProc [%s][%s] Map[%d]-(%d,%d) User(%d,%d) Can not Move Position Attr[%d]",
								lpObj->AccountID,lpObj->Name,lpObj->MapNumber,nextX,nextY,lpObj->X,lpObj->Y,attr);
							for(n = 0; n < 15;n++)
							{
								lpObj->PathX[n] = 0;
								lpObj->PathY[n] = 0;
								//lpObj->PathOri[n] = 0;
							}
	
							lpObj->PathCount = 0;
							lpObj->PathCur = 0;
							lpObj->PathTime = GetTickCount();

							if(lpObj->Type == OBJ_MONSTER || lpObj->Type == OBJ_NPC)
							{
								lpObj->PathStartEnd = 0;
							}
	
							if(lpObj->Type == OBJ_USER)
							{
								gObjSetPosition(lpObj->m_Index,lpObj->X,lpObj->Y);
							}
						}
						else
						{
							lpObj->X = lpObj->PathX[lpObj->PathCur];
							lpObj->Y = lpObj->PathY[lpObj->PathCur];
							lpObj->Dir = lpObj->PathDir[lpObj->PathCur];

							lpObj->PathTime = GetTickCount();
							lpObj->PathCur++;

							if(lpObj->PathCur >= lpObj->PathCount)
							{
								lpObj->PathCur = 0;
								lpObj->PathCount = 0;

								if(lpObj->Type == OBJ_MONSTER || lpObj->Type == OBJ_NPC)
								{
									lpObj->PathStartEnd = 0;
								}
							}
						}
					}
				}
				CreateFrustrum(lpObj->X,lpObj->Y,n);
			}
		}
	}

	// STRANGE COMPILATION ... 
}

void gObjRefillProcess(int aIndex, LPOBJ lpObj)
{
	return;	// HEre is a MACRO
}

void gObjInit()
{
	gObjCount = OBJ_STARTUSERINDEX;
	gObjMonCount = 0;
	gObjCallMonCount = OBJ_MAXMONSTER;

	//108 + 64 + 64
	CItem * pTempInventory = new CItem[MAIN_INVENTORY_NORMAL_SIZE + (MAX_EXTENDED_INV*EXTENDED_INV_SIZE) + PSHOP_SIZE];
	BYTE * pTempInventoryMap = new BYTE [MAIN_INVENTORY_NORMAL_SIZE + (MAX_EXTENDED_INV*EXTENDED_INV_SIZE) + PSHOP_SIZE];

	for ( int n = 0; n< OBJMAX ; n++)
	{
		memset(&gObj[n], 0, sizeof(OBJECTSTRUCT));
		gObj[n].Type = (BYTE)OBJ_EMPTY;
		gObj[n].m_socket = INVALID_SOCKET;
		gObj[n].PathTime = GetTickCount();
		gObj[n].m_RecallMon = -1;
		gObj[n].Magic = new CMagicInf[MAGIC_SIZE];
		
		gObj[n].m_lpMagicBack = NULL;
		gObj[n].pChaosBox = NULL;
		gObj[n].pWarehouse = NULL;
		gObj[n].pWarehouseMap = NULL;
		gObj[n].MasterCharacterInfo = NULL;

#if (PACK_EDITION>=2)
		gObj[n].CustomQuest = NULL;
		gObj[n].MarryCharacterInfo = NULL;
#endif
		
#if (PACK_EDITION>=3)
		gObj[n].S5Quest = NULL;
		gObj[n].AFKCharClass = NULL;
#endif

		if ( n >= OBJ_MAXMONSTER )
		{
			gObj[n].m_lpMagicBack = new CMagicInf[MAGIC_SIZE];
			//gObj[n].MasterCharacterInfo = new MASTERCHARINFO;	//Enable If botpet will be using Mastering Stuff
		}

		if ( n >= OBJ_STARTUSERINDEX )
		{
#if(ENABLE_CSHOP == 1)
			gObj[n].CShop = new MUITEMSHOP[MUITEMSHOP_MAXITEMPERPAGE*MUITEMSHOP_MAXPAGES];
			gObj[n].CShopGift = new MUITEMSHOP[MUITEMSHOP_MAXITEMPERPAGE*MUITEMSHOP_MAXPAGES];
			gObj[n].CShopGiftMsg = new MUITEMSHOPGIFT[MUITEMSHOP_MAXITEMPERPAGE*MUITEMSHOP_MAXPAGES];
			
			gObj[n].CShopInfoModified_Gift = 0;
			gObj[n].CShopInfoModified_Points = 0;
			gObj[n].GoblinCoin = 0;
			gObj[n].WCoin = 0;
			gObj[n].WCoinP = 0;
			//int pepe=sizeof(MUITEMSHOPGIFT[MUITEMSHOP_MAXITEMPERPAGE*MUITEMSHOP_MAXPAGES]);
			//int pepe=sizeof(MUITEMSHOP[MUITEMSHOP_MAXITEMPERPAGE*MUITEMSHOP_MAXPAGES]);
#endif

			//108 + 64 + 64
			gObj[n].Inventory1 = new CItem[MAIN_INVENTORY_NORMAL_SIZE + (MAX_EXTENDED_INV*EXTENDED_INV_SIZE) + PSHOP_SIZE];
			gObj[n].Inventory2 = new CItem[MAIN_INVENTORY_NORMAL_SIZE + (MAX_EXTENDED_INV*EXTENDED_INV_SIZE) + PSHOP_SIZE];
			gObj[n].InventoryMap1 = new BYTE[ReadConfig.INVENTORY_MAP_SIZE(n,true)];
			gObj[n].InventoryMap2 = new BYTE[ReadConfig.INVENTORY_MAP_SIZE(n,true)];

			gObj[n].Trade = new CItem[TRADE_BOX_SIZE];
			gObj[n].TradeMap = new BYTE[TRADE_BOX_SIZE];

			gObj[n].pWarehouse = new CItem[ReadConfig.WAREHOUSE_SIZE(n,true)];
			gObj[n].pWarehouseMap = new BYTE[ReadConfig.WAREHOUSE_SIZE(n,true)];

			gObj[n].MasterCharacterInfo = new MASTERCHARINFO;

#if (PACK_EDITION>=2)
			gObj[n].MarryCharacterInfo = new MARRYINFO;
			gObj[n].CustomQuest = new CUSTOMQUEST[256];
#endif
			
#if (PACK_EDITION>=3)
			gObj[n].AFKCharClass = new ANTIAFKCHARINFO;
			gObj[n].S5Quest = new S5QUEST[MAX_QUEST_COUNT];
#endif

			gObj[n].PerSocketContext = new _PER_SOCKET_CONTEXT;
		}
		else
		{
			gObj[n].Inventory1 = pTempInventory;
			gObj[n].Inventory2 = pTempInventory;
			gObj[n].InventoryMap1 = pTempInventoryMap;
			gObj[n].InventoryMap2 = pTempInventoryMap;
		}

		InitializeCriticalSection(&gObj[n].m_critPShopTrade);
	}


	InitFrustrum();
}






void gObjEnd()
{
	gObjAllDisconnect();
	gObjAllLogOut();

	for ( int n=0; n< OBJMAX; n++)
	{
		if ( n >= OBJ_MAXMONSTER )
		{
			if ( gObj[n].m_lpMagicBack != NULL )
			{
				delete [] gObj[n].m_lpMagicBack;
			}
		}

		if ( n >= OBJ_STARTUSERINDEX )
		{
			if ( gObj[n].Inventory1 != NULL )
			{
				delete gObj[n].Inventory1;
			}

			if ( gObj[n].Inventory2 != NULL )
			{
				delete gObj[n].Inventory2;
			}

			if ( gObj[n].InventoryMap1 != NULL )
			{
				delete gObj[n].InventoryMap1;
			}

			if ( gObj[n].InventoryMap2 != NULL )
			{
				delete gObj[n].InventoryMap2;
			}

			if ( gObj[n].PerSocketContext != NULL )
			{
				delete gObj[n].PerSocketContext;
			}

			if ( gObj[n].Trade != NULL )
			{
				delete gObj[n].Trade;
			}

			if ( gObj[n].TradeMap != NULL )
			{
				delete gObj[n].TradeMap;
			}

			if ( gObj[n].pChaosBox != NULL )
			{
				delete  gObj[n].pChaosBox;
			}

			if ( gObj[n].pChaosBoxMap != NULL )
			{
				delete  gObj[n].pChaosBoxMap;
			}

			if ( gObj[n].pWarehouse != NULL )
			{
				delete gObj[n].pWarehouse;
			}

			if ( gObj[n].pWarehouseMap != NULL )
			{
				delete gObj[n].pWarehouseMap;
			}

			if ( gObj[n].MasterCharacterInfo != NULL )
			{
				delete gObj[n].MasterCharacterInfo;
			}

#if (PACK_EDITION>=2)
			if ( gObj[n].CustomQuest != NULL )
			{
				delete gObj[n].CustomQuest;
			}
			if ( gObj[n].MarryCharacterInfo != NULL )
			{
				delete gObj[n].MarryCharacterInfo;
			}
#endif

#if (PACK_EDITION>=3)
			if ( gObj[n].S5Quest != NULL )
			{
				delete gObj[n].S5Quest;
			}

			if ( gObj[n].AFKCharClass != NULL )
			{
				delete gObj[n].AFKCharClass;
			}
#endif

		} else {
			if ( gObj[n].Trade != NULL )
			{
				delete gObj[n].Trade;
			}

			if ( gObj[n].TradeMap != NULL )
			{
				delete gObj[n].TradeMap;
			}

			if ( gObj[n].MasterCharacterInfo != NULL )
			{
				delete gObj[n].MasterCharacterInfo;
			}
		}

		DeleteCriticalSection( &gObj[n].m_critPShopTrade );
	}
}






void gObjClearViewportOfMine(LPOBJ lpObj)
{
	for (int i=0;i<MAX_VIEWPORT;i++)
	{
		lpObj->VpPlayer[i].state =0;
		lpObj->VpPlayer[i].number =-1;
	}

	lpObj->VPCount=0;
}







void gObjClearViewport(LPOBJ lpObj)
{
	for ( int i=0;i<MAX_VIEWPORT;i++)
	{
		lpObj->VpPlayer[i].state=0;
		lpObj->VpPlayer[i].number =-1;
		lpObj->VpPlayer2[i].state=0;
		lpObj->VpPlayer2[i].number=-1;
	}

	lpObj->VPCount = 0;
	lpObj->VPCount2 = 0;
}







void gObjCloseSet(int aIndex, int Flag)
{
	if ( aIndex < 0 || aIndex > OBJMAX-1 )
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if ( lpObj->CloseCount > 0 )
	{
		return;
	}

	if ( lpObj->Connected == PLAYER_PLAYING )
	{
		
		if ( BC_MAP_RANGE(lpObj->MapNumber) )
		{
			g_BloodCastle.SearchUserDropQuestItem(aIndex);
		}
		
#if (CRYSTAL_EDITION == 1)
		if(lpObj->MuTabOpen == TRUE)
		{			
			char OpenTab[]={0xC1,0x05,0xFB,0x0F,0x00};
			DataSend(aIndex,(unsigned char *)&OpenTab,5);
		}

#if (PACK_EDITION>=3)
		if(lpObj->VipShopOpen == TRUE)
		{
			SVShop.WindowState(lpObj->m_Index,0);
		}
#endif
#endif
		if ( (GetTickCount() - lpObj->MySelfDefenseTime )< 30000)
		{
			GCServerMsgStringSend(lMsg.Get(MSGGET(4, 109)), lpObj->m_Index, 1);
			return;
		}

		if ( (lpObj->m_IfState.use != 0 ) && ( ( lpObj->m_IfState.type == 1 ) || (lpObj->m_IfState.type == 6) || (lpObj->m_IfState.type == 13) || (lpObj->m_IfState.type == 7)  ) ) 
		{
			gObjSaveChaosBoxItemList(lpObj);
			switch ( lpObj->m_IfState.type )
			{
				case 1:
					GCServerMsgStringSend(lMsg.Get(MSGGET(4, 110)), lpObj->m_Index, 1);
					break;

				case 6:
					GCServerMsgStringSend(lMsg.Get(MSGGET(4, 111)), lpObj->m_Index, 1);
					break;

				case 7:
					GCServerMsgStringSend(lMsg.Get(MSGGET(4, 127)), lpObj->m_Index, 1);
					break;

				case 13:
					GCServerMsgStringSend(lMsg.Get(MSGGET(4, 229)), lpObj->m_Index, 1);
					break;
			}

			return;
		}
	}

	
	lpObj->CloseCount = 6;
	lpObj->CloseType = Flag;
	lpObj->bEnableDelCharacter = true;
	
}







void gObjCharTradeClear(LPOBJ lpObj)
{
	if ( lpObj->Type != OBJ_USER )
	{
		return;
	}

	memset(lpObj->TradeMap, 0xFF , TRADE_BOX_SIZE);

	for ( int i=0 ; i< TRADE_BOX_SIZE ; i++)
	{
		lpObj->Trade[i].Clear();
	}

	lpObj->TradeMoney = 0;
	lpObj->TradeOk = false;
}






void gObjCharZeroSet(int aIndex)
{
	int i;
	LPOBJ lpObj = &gObj[aIndex];
	int n;

	
	lpObj->MasterCharacterInfo = new MASTERCHARINFO;

	memset( lpObj->Name, 0, MAX_ACCOUNT_LEN);
	memset( lpObj->GuildName, 0, sizeof (lpObj->GuildName ));
	lpObj->m_Option = 3;
	lpObj->m_iScriptMaxLife = 0;
	lpObj->Level = 0;

	memset( lpObj->BackName,0,sizeof(lpObj->BackName));

#if (PACK_EDITION>=3)
	lpObj->IsBot = 0;
	lpObj->BotNumOwner = -1;
	lpObj->BotPower = 0;
	lpObj->BotDefense = 0;
	lpObj->BotLife = 0;
	lpObj->BotMaxLife = 0;
	lpObj->BotLvlUpDefense = 0;
	lpObj->BotLvlUpPower = 0;
	lpObj->BotLvlUpLife = 0;
	lpObj->BotLvlUpMana = 0;
	lpObj->BotLvlUpExp = 0;
	lpObj->BotLvlUpMaxLevel = 0;
	lpObj->BotFollowMe = 0;

	lpObj->GensCont = 0;
	lpObj->GensFam = 0;
	lpObj->GensLevel = 0;
#endif

	lpObj->Life = 0;
	lpObj->MaxLife = 0;
	lpObj->AddLife = 0;
	lpObj->AddMana = 0;
	lpObj->Live = FALSE;
	lpObj->Dir = 0;
	lpObj->X = 0;
	lpObj->Y = 0;
	lpObj->TX = 0;
	lpObj->TY = 0;
	lpObj->MTX = 0;
	lpObj->MTY = 0;
	lpObj->MapNumber = MAP_INDEX_LORENCIA;
	lpObj->RegenTime = 0;
	lpObj->m_State = 0;
	lpObj->RegenOk = 0;
	lpObj->lpAttackObj = 0;
	lpObj->PartyTargetUser = -1;
	//lpObj->m_MoveGateNumber = -1;
	lpObj->CloseCount = -1;
	lpObj->CloseType = -1;
	lpObj->m_IfState.state = -1;
	lpObj->m_IfState.type = -1;
	lpObj->m_IfState.use = 0;
	lpObj->m_InterfaceTime = 0;
	lpObj->m_ActionNumber = 0;
	lpObj->m_State = 0;
	//lpObj->m_StateSub = 0;
	//lpObj->m_ViewState = 0;
	lpObj->m_Rest = 0;

	lpObj->iMaxShield = 0;
	lpObj->iShield = 0;
	lpObj->iAddShield = 0;
	lpObj->iFillShield = 0;
	lpObj->iFillShieldMax = 0;
	lpObj->iFillShieldCount = 0;
	lpObj->dwShieldAutoRefillTimer = 0;
	
	for(int i=0;i<2;i++)
	{
		lpObj->m_wItemEffectType[i] = 0;
		lpObj->m_iItemEffectValidTime[i] = 0;
	}

	lpObj->m_wItem_MuShop_Seal_EffectType = 0;
	lpObj->m_iItem_MuShop_Seal_EffectValidTime = 0;
	
	lpObj->m_wItem_MuShop_Scroll_EffectType = 0;
	lpObj->m_iItem_MuShop_Scroll_EffectValidTime = 0;

	lpObj->m_wCashPoint = 0;
	lpObj->m_iPeriodItemEffectIndex = -1;

	lpObj->m_btDisableExpGain = 0;
	lpObj->m_wExprienceRate = 100;
	lpObj->m_wItemDropRate = 100;

	lpObj->m_ViewSkillState = 0;
	lpObj->m_LastViewSkillState = 0;
	lpObj->m_PoisonBeattackCount = 0;
	lpObj->m_ColdBeattackCount = 0;
	lpObj->m_Attribute = 0;
	lpObj->m_ImmuneToMagicCount = 0;
	lpObj->m_ImmuneToHarmCount = 0;
	lpObj->m_iMonsterBattleDelay = 0;
	lpObj->m_cKalimaGateExist = FALSE;
	lpObj->m_iKalimaGateIndex = -1;
	lpObj->m_cKalimaGateEnterCount = 0;
	lpObj->TradeOk = false;
	//lpObj->sHDCount = 0;
	lpObj->PathCount = 0;
	lpObj->PathCur = 0;
	lpObj->PathStartEnd = 0;
	lpObj->VPCount = 0;
	lpObj->VPCount2 = 0;
	lpObj->m_ShopTime = 0;
	lpObj->m_ManaFillCount = 0;
	lpObj->FillLife = 0;
	lpObj->iObjectSecTimer = 0;
	lpObj->m_FriendServerOnline = FALSE;
	lpObj->SetOpAddMaxAttackDamage = 0;
	lpObj->SetOpAddMinAttackDamage = 0;
	lpObj->SetOpAddDamage = 0;
	lpObj->SetOpIncAGValue = 0;
	lpObj->SetOpAddCriticalDamageSuccessRate = 0;
	lpObj->SetOpAddCriticalDamage = 0;
	lpObj->SetOpAddExDamageSuccessRate = 0;
	lpObj->SetOpAddExDamage = 0;
	lpObj->SetOpAddSkillAttack = 0;
	lpObj->AddStrength = 0;
	lpObj->AddDexterity = 0;
	lpObj->AddVitality = 0;
	lpObj->AddEnergy = 0;
	lpObj->SetOpAddAttackDamage = 0;
	lpObj->SetOpAddDefence = 0;
	lpObj->SetOpAddMagicPower = 0;
	lpObj->SetOpAddDefenceRate = 0;
	lpObj->SetOpIgnoreDefense = 0;
	lpObj->SetOpDoubleDamage = 0;
	lpObj->SetOpTwoHandSwordImproveDamage = 0;
	lpObj->SetOpImproveSuccessAttackRate = 0;
	lpObj->SetOpReflectionDamage = 0;
	lpObj->SetOpImproveSheldDefence = 0;
	lpObj->SetOpDecreaseAG = 0;
	lpObj->SetOpImproveItemDropRate = 0;
	memset( lpObj->m_AddResistance, 0, sizeof(lpObj->m_AddResistance));
	memset( lpObj->OfficialHelper, 0, sizeof(lpObj->OfficialHelper));
	lpObj->OfficialHelper_Active = false;
	lpObj->OfficialHelper_TimeCount = 0;
	lpObj->IsFullSetItem = false;
	lpObj->DamageMinus = 0;
	lpObj->DamageReflect = 0;
	lpObj->MonsterDieGetMoney = 0;
	lpObj->MonsterDieGetLife = 0;
	lpObj->MonsterDieGetMana = 0;
	lpObj->m_CriticalDamage = 0;
	lpObj->m_ExcelentDamage = 0;
	lpObj->UseMagicNumber = 0;
	lpObj->UseMagicTime = 0;
	lpObj->UseMagicCount = 0;
	lpObj->OSAttackSerial = -1;
	//lpObj->SASCount = 0;
	lpObj->SkillAttackTime = GetTickCount();
	lpObj->m_AttackSpeed = 0;
	lpObj->m_MagicSpeed = 0;
	//lpObj->m_TotalAttackTime = GetTickCount();
	//lpObj->m_TotalAttackCount = 0;
	lpObj->CheckSumTime = GetTickCount() + 10000;
	lpObj->CheckSumTableNum = -1;
	lpObj->m_ZeroTimeAttackCount = 0;
	lpObj->m_DetectCount = 0;
	lpObj->m_DetectSpeedHackTime = 0;
	lpObj->m_DetectedHackKickCount = 0;
	lpObj->IsInBattleGround = false;
	lpObj->CheckTickCount = 0;
#if (CRYSTAL_EDITION == 1)
	lpObj->CrystalPing = 0;
	lpObj->CrystalPingReply = 0;
#endif
#if (PACK_EDITION>=3)
	BotRacer.ResetTime(aIndex);
#endif
	lpObj->m_AttackSpeedHackDetectedCount = 0;
	lpObj->m_isDuelSpectator = 0;

	for ( n =0;n<MAX_VIEWPORT ;n++)
	{
		lpObj->VpPlayer[n].state = 0;
		lpObj->VpPlayer2[n].state = 0;
		lpObj->VpPlayer[n].number = -1;
		lpObj->VpPlayer2[n].number = -1;
	}

	lpObj->GuildNumber = 0;
	lpObj->lpGuild = NULL;
	lpObj->PartyNumber = -1;
	lpObj->PartyTargetUser = -1;
	//lpObj->m_Drink = 0;
	lpObj->Authority = 0;
	lpObj->AuthorityCode = 0;
	lpObj->Penalty = 0;
	lpObj->m_RecallMon = -1;
	lpObj->m_Change = -1;
	lpObj->GuildStatus = -1;
	lpObj->LastAttackerID = -1;
	lpObj->m_cChaosCastleIndex= -1;
	lpObj->m_cChaosCastleSubIndex = -1;
	lpObj->m_iChaosCastleBlowTime = 0;
	lpObj->m_cKillUserCount = 0;
	lpObj->m_cKillMonsterCount = 0;
	//lpObj->m_InWebzen = false;
	memset( lpObj->CharSet, 0, sizeof(lpObj->CharSet));

	for ( i =0; i<MAX_SELF_DEFENSE;i++)
	{
		lpObj->SelfDefense[i] = -1;
		lpObj->SelfDefenseTime[i] = 0;
	}

	lpObj->MySelfDefenseTime = 0;
	lpObj->m_MagicDefense = 0;
	lpObj->m_SkillDefense = 0;
	lpObj->m_SkillDefenseTime = 0;
	lpObj->m_SkillAttack = 0;
	lpObj->m_SkillAttackTime = 0;
	lpObj->m_SkillAttack2 = 0;
	lpObj->m_SkillAttackTime2 = 0;
	lpObj->LoadWareHouseInfo = false;
	lpObj->m_SkillAddLife = 0;
	lpObj->m_SkillAddLife_Mana = 0;
	lpObj->m_SkillMagicCircleCriticalDamage = 0;
	lpObj->m_SkillAddLifeTime = 0;
	lpObj->m_SkillHarden = 0;
	lpObj->m_SkillHardenTime = 0;
	lpObj->m_SkillMagumReduceDefense = 0;
	lpObj->m_SkillMagumReduceDefenseTime = 0;
	lpObj->m_SkyBossMonSheildLinkIndex = -1;
	lpObj->m_SkyBossMonSheild = 0;
	lpObj->m_SkyBossMonSheildTime = 0;
	lpObj->m_bAttackerKilled = false;
	lpObj->m_MoveOtherServer = 0;
	lpObj->m_BossGoldDerconMapNumber = -1;
	lpObj->TargetNumber =	0;
	lpObj->TargetShopNumber = 0;

	lpObj->m_bDevilSquareAuth = false;
	lpObj->m_TimeCount = 0;
	lpObj->m_TimeCount2 = 0;
	lpObj->EventChipCount = 0;
	lpObj->MutoNumber = 0;
	lpObj->HaveWeaponInHand = false;
	lpObj->m_ReqWarehouseOpen = 0;
	lpObj->iStoneCount = 0;
	lpObj->m_iDuelUserReserved = -1;
	lpObj->m_iDuelUserRequested = -1;
	lpObj->m_iDuelUser = -1;
	lpObj->m_btDuelScore = 0;
	lpObj->m_iDuelTickCount = 0;
	lpObj->m_MaxLifePower = 0;
	lpObj->m_WizardSkillDefense = 0;
	lpObj->m_WizardSkillDefenseTime = 0;
	//lpObj->m_PacketChecksumTime = 0;
	lpObj->AddBP = 0;
	lpObj->m_CheckLifeTime = 0;
	memset(lpObj->m_Quest, (BYTE)-1, sizeof(lpObj->m_Quest));
	lpObj->m_SendQuestInfo = 0;
	gPacketCheckSum.ClearCheckSum(lpObj->m_Index);
	lpObj->NPggCSAuth.Init();
	lpObj->m_bSentGGAuth = false;
	lpObj->m_NPggCheckSumSendTime = 0;
	lpObj->PlusStatQuestClear = false;
	lpObj->ComboSkillquestClear = false;
	//lpObj->m_LastTeleportTime = 0;
	//lpObj->m_ClientHackLogCount = 0;
	lpObj->UseEventServer = false;
	lpObj->m_SkillBerserkerTime = 0;
	lpObj->m_SkillBerserkerDef = 0;
	lpObj->m_SkillDragonSlayerTime = 0;
	lpObj->m_SkillDragonSlayerDmg = 0;
	lpObj->m_SkillRedArmorIgnoreTime = 0;
	lpObj->m_SkillRedArmorIgnoreNum = 0;
	lpObj->m_SkillFitnessTime = 0;
	lpObj->m_SkillFitnessVal = 0;
	lpObj->m_SkillDefSuccessRateIncTime = 0;
	lpObj->m_SkillDefSuccessRateIncNum = 0;
	lpObj->m_SkillBerserkerAttack = 0;
	lpObj->m_SkillReflectTime = 0;
	lpObj->m_SkillReflect = 0;
	lpObj->m_SkillMagicCircleTime = 0;
	lpObj->m_SkillMagicCircle = 0;
	lpObj->m_SkillReduceDamageTime = 0;
	lpObj->m_SkillReduceDamage = 0;
	lpObj->m_SkillReduceDefenseTime = 0;
	lpObj->m_RFBufReduceDefenseTime = 0;
	lpObj->m_RFBufReduceDefense = 0;	
	lpObj->m_SkillReduceDefense = 0;	
	lpObj->m_iSkillNPCSantaType = 0;
	lpObj->m_iSkillNPCSantaTime = 0;

	lpObj->DieMap = 0;
	lpObj->DieX = 0;
	lpObj->DieY = 0;

	for ( i=0;i<MAX_MAGIC ; i++)
	{
		lpObj->Magic[i].Clear();
	}

	lpObj->MagicCount = 0;

	for ( i=0;i<MAX_MONSTER_SEND_MSG;i++)
	{
		gSMMsg[aIndex][i].MsgCode = -1;
	}

	lpObj->m_bIsInMonsterHerd = false;
	lpObj->m_bIsMonsterAttackFirst = 1;
	lpObj->m_lpMonsterHerd = NULL;

	if ( (aIndex-OBJ_STARTUSERINDEX) >= 0 )
	{
		for ( i=0;i<MAX_MONSTER_SEND_ATTACK_MSG;i++)
		{
			gSMAttackProcMsg[aIndex-OBJ_STARTUSERINDEX][i].MsgCode = -1;
		}
	}

	lpObj->SkillHellFire2State = 0;
	lpObj->SkillHellFire2Count = 0;
	lpObj->m_PoisonType = 0;
	lpObj->m_PoisonAdd = 0;
	lpObj->m_IceType = 0;
	lpObj->Leadership = 0;
	lpObj->AddLeadership = 0;
	lpObj->ChatLimitTime = 0;
	lpObj->ChatLimitTimeSec = 0;
	lpObj->SkillRecallParty_Time = 0;
	lpObj->SkillAddCriticalDamageTime = 0;
	lpObj->SkillAddCriticalDamage = 0;
	lpObj->SkillLongSpearChange = false;
	lpObj->SkillDelay.Init();

	lpObj->bEnableDelCharacter = true;

	::gMultiAttackHackCheck[aIndex].Init();
	lpObj->pTransaction = 0;
	::gObjMonsterHitDamageInit(lpObj);
	::gObjSetInventory1Pointer(&gObj[aIndex]);

	lpObj->m_bPShopOpen = false;
	lpObj->m_bPShopTransaction = false;
	lpObj->m_bPShopItemChange = false;
	lpObj->m_bPShopRedrawAbs = false;
	memset(lpObj->m_szPShopText, 0, sizeof(lpObj->m_szPShopText));
	lpObj->m_bPShopWantDeal = false;
	lpObj->m_iPShopDealerIndex = -1;
	memset(lpObj->m_szPShopDealerName, 0, sizeof(lpObj->m_szPShopDealerName));
	memset(lpObj->m_iVpPShopPlayer, 0, sizeof(lpObj->m_iVpPShopPlayer));
	lpObj->m_wVpPShopPlayerCount =0;
	lpObj->iGuildUnionTimeStamp = 0;

	lpObj->bIsChaosMixCompleted = false;
	lpObj->m_bMapSvrMoveQuit = false;
	lpObj->m_bMapAntiHackMove = false;
	lpObj->b_BlockMoveCommandSeconds = 0;
	
	lpObj->m_btWeaponState = 0;
	lpObj->m_iSkillStunTime = 0;
	
#if (GS_CASTLE==1)
	//lpObj->m_bIsCastleNPCUpgradeCompleted = false;
	//lpObj->m_iCsNpcExistVal = 0;
	lpObj->m_btCsGateOpen = 0;
	lpObj->m_iSkillStunTime = 0;
	lpObj->m_btCsNpcRgLevel = 0;
	lpObj->m_btCsJoinSide = 0;
	//lpObj->m_bCsGuildInvolved = false;
#endif

	lpObj->m_btKillCount = 0;
	lpObj->m_iSkillBrandOfSkillTime = 0;
	lpObj->m_iSkillInvisibleTime = 0;
	lpObj->m_iSkillManaSwellTime = 0;
	lpObj->m_iSkillManaSwell = 0;
	lpObj->m_iPotionBlessTime = 0;
	lpObj->m_iPotionSoulTime = 0;
	lpObj->m_iAccumulatedDamage = 0;
	lpObj->m_iAccumulatedCrownAccessTime = 0;
	lpObj->m_iSkillNPCHelpTime = 0;
	lpObj->m_iSkillNPCDefense = 0;
	lpObj->m_iSkillNPCAttack = 0;
	lpObj->m_iMuseElfInfinityArrowSkillTime = 0;
	lpObj->m_MonsterSkillElementInfo.Reset();
	lpObj->m_iCrywolfMVPScore = 0;
	lpObj->m_iSkillDistanceErrorCount = 0;
	lpObj->m_dwSkillDistanceErrorTick = 0;
	lpObj->m_Invisible = 0;

	if ( lpObj->Type == OBJ_USER )
	{
		lpObj->WarehouseMoney = 0;
		lpObj->WarehouseExNum = 0;
		lpObj->WarehouseExNumLast = 0;
		lpObj->WarehouseTimeCount = 0;
		lpObj->WarehouseSave = 0;
		lpObj->WarehouseLock = -1;
		lpObj->WarehousePW = 0;
		//lpObj->WarehouseCount = 0;
		//lpObj->WarehouseUnfailLock = 0;

		lpObj->PlayerExtraInfoReceived = 0;
		lpObj->PlayerQ5InfoReceived = 0;
		lpObj->PlayerCQInfoReceived = 0;
		lpObj->PlayerSkillTreeInfoReceived = 0;

		lpObj->PlayerExtraInfoModified = 0;
		lpObj->PlayerQ5InfoModified = 0;
		lpObj->PlayerSkillTreeInfoModified = 0;
		lpObj->AccountExtraInfoModified = 0;
		lpObj->PlayerCustomQuestInfoModified = 0;
		lpObj->PlayerOfficialHelperInfoModified = 0;

		lpObj->Resets = 0;
		lpObj->MovePerSecs = 0;
		lpObj->PCPoints = 0;
		lpObj->ExInventory = 0;
		lpObj->ExInventoryTmp = 0;

#if (PACK_EDITION>=3)
		gObj[aIndex].RaceCheck = 0;
		gObj[aIndex].RaceTime = 0;
#endif

		::gObjCharTradeClear(lpObj);
		memset(lpObj->pWarehouseMap, (BYTE)-1, ReadConfig.WAREHOUSE_SIZE(aIndex,true));
		memset(lpObj->pInventoryMap, (BYTE)-1, ReadConfig.INVENTORY_MAP_SIZE(aIndex,true));

		for (i=0;i<ReadConfig.WAREHOUSE_SIZE(aIndex,true);i++)
		{
			lpObj->pWarehouse[i].Clear();
		}

		for ( i=0;i<ReadConfig.INVENTORY_SIZE(aIndex,true);i++)
		{
			lpObj->Inventory1[i].Clear();
		}

		for ( i=0;i<ReadConfig.INVENTORY_SIZE(aIndex,true);i++)
		{
			lpObj->Inventory2[i].Clear();
		}

		lpObj->MasterCharacterInfo->MasterLevel = 0;
		lpObj->MasterCharacterInfo->MasterPoints = 0;
		lpObj->MasterCharacterInfo->MasterPointsTmp = 0;
		if(ReadConfig.S6E2 == 1)
		{
			lpObj->MasterCharacterInfo->Clear();
			lpObj->MasterCharacterInfo->ClearSkills();
			memset(&lpObj->sSkillKey,0,sizeof(lpObj->sSkillKey));
		}

#if (PACK_EDITION>=2)
		lpObj->MarryCharacterInfo->Married = 0;
		lpObj->MarryCharacterInfo->MarrySeconds = 0;
		lpObj->MarryCharacterInfo->aFloodMarryCmd = 0;
		memset( lpObj->MarryCharacterInfo->MarriedName, 0, sizeof(lpObj->MarryCharacterInfo->MarriedName));
		memset( lpObj->MarryCharacterInfo->MarryTarget, 0, sizeof(lpObj->MarryCharacterInfo->MarryTarget));
		for (int i=0;i<256;i++)
		{
			lpObj->CustomQuest[i].State = 0xFF;
			lpObj->CustomQuest[i].Status = 0xFF;
		}
#endif

#if (PACK_EDITION>=3)
		for (int i=0;i<MAX_QUEST_COUNT;i++)
		{
			lpObj->S5Quest[i].Active= 0;
			lpObj->S5Quest[i].NumID = 0;
			for(int j=0;j<MAX_QUEST_REQ;j++)
				lpObj->S5Quest[i].State[j] = 0;
		}

		lpObj->AFKCharClass->AntiAFK_Map = 0;
		lpObj->AFKCharClass->AntiAFK_WarnNumber = 0;
		lpObj->AFKCharClass->AntiAFK_X = 0;
		lpObj->AFKCharClass->AntiAFK_Y = 0;
#endif

		g_kJewelOfHarmonySystem.InitEffectValue(&lpObj->m_JewelOfHarmonyEffect);
		g_kItemSystemFor380.InitEffectValue(&lpObj->m_ItemOptionExFor380);
	}

	lpObj->m_bKanturuEntranceByNPC = 0;
	lpObj->m_SkillDragonSlayerTime = 0;
	lpObj->m_SkillDragonSlayerDmg = 0;
	lpObj->m_SkillRedArmorIgnoreTime = 0;
	lpObj->m_SkillRedArmorIgnoreNum = 0;
	lpObj->m_SkillFitnessTime = 0;
	lpObj->m_SkillFitnessVal = 0;
	lpObj->m_SkillDefSuccessRateIncTime = 0;
	lpObj->m_SkillDefSuccessRateIncNum = 0;
	lpObj->m_SkillBerserkerTime = 0;
	lpObj->m_SkillBerserkerDef = 0;
	lpObj->m_SkillBerserkerAttack = 0;
	lpObj->m_SkillReflectTime = 0;
	lpObj->m_SkillReflect = 0;
	lpObj->m_SkillMagicCircleTime = 0;
	lpObj->m_SkillMagicCircle = 0;
	lpObj->m_SkillReduceDamageTime = 0;
	lpObj->m_SkillReduceDamage = 0;
	lpObj->m_SkillReduceDefenseTime = 0;
	lpObj->m_RFBufReduceDefenseTime = 0;
	lpObj->m_RFBufReduceDefense = 0;
	lpObj->m_SkillReduceDefense = 0;	
	lpObj->m_SkillSleep = 0;
	lpObj->m_SkillSleep_Time = 0;
	lpObj->m_SkillSleep_MoveRange = 0;
	lpObj->m_SkillSleep_AttackRange = 0;
	lpObj->m_SkillSleep_ViewRange = 0;
	lpObj->m_SkillNightTime = 0;
	lpObj->m_SkillNight = 0;
	lpObj->m_SkillSummonerBookTime = 0;
	lpObj->m_SkillSummonerBookTypeDmg = 0;
	lpObj->m_SkillSummonerBookDmg = 0;
	lpObj->m_SkillSummonerBookSender = 0;
	lpObj->m_SkillExplotionTime = 0;
	lpObj->m_SkillLightingStormTime = 0;
	lpObj->m_SkillLightingStorm = 0;
	lpObj->m_SkillLightingStormSender = 0;
	lpObj->m_SkillRedStormTime = 0;
	lpObj->m_SkillRedStormDmg = 0;
	lpObj->m_SkillRedStormSender = 0;
	lpObj->m_SkillSwordSlashTime = 0;
	lpObj->m_SkillSwordSlashDmg = 0;
	lpObj->m_SkillSwordSlashSender = 0;

	
	lpObj->m_SkillIT_Number = 0;
	lpObj->m_SkillIT_Time = 0;

	lpObj->m_SkillGladiatorsGloryTime = 0;

	lpObj->m_iShieldFillCount = 0;
	::gObjClearViewport(&gObj[aIndex]);	
}






int gObjGetSocket(SOCKET socket)
{
	for (int n=OBJ_STARTUSERINDEX;n<OBJMAX;n++)
	{
		if (gObj[n].Connected != PLAYER_EMPTY)
		{
			if (gObj[n].m_socket == socket)
			{
				return n;
			}
		}
	}
	return -1;
}






void gObjSetTradeOption( int aIndex, int option)
{
	if ( gObjIsConnected(aIndex) == TRUE )
	{
		if ( option == 0 )
		{
			gObj[aIndex].m_Option = 0;
			GCServerMsgStringSend(lMsg.Get(MSGGET(4, 97)), aIndex, 1);
		}
		else
		{
			gObj[aIndex].m_Option |= 1;
			GCServerMsgStringSend(lMsg.Get(MSGGET(4, 96)), aIndex, 1);
		}
	}
}





void gObjSetDuelOption(int aIndex, int option)
{
	if ( gObjIsConnected(aIndex) == TRUE )
	{
		if ( option == 0 )
		{
			gObj[aIndex].m_Option = 0;
		}
		else
		{
			gObj[aIndex].m_Option |= 2;
		}
	}
}

int GetMapMoveLevel(LPOBJ lpObj, int mapnumber, int max_over)
{
	int overLevel = 0;

	if ( mapnumber > MAX_MAP_NUMBER-1 )
	{
		mapnumber = 0;
		LogAdd("error-L3 : map number not %s %d", __FILE__, __LINE__);
		return 0;
	}

	if ( max_over != 0 )
	{
		overLevel = max_over;
	}
	else
	{
		max_over = MapMinUserLevel[mapnumber];
		overLevel = MapMinUserLevel[mapnumber];
	}
	
	if (lpObj->Class == CLASS_MAGICGLADIATOR || 
		lpObj->Class == CLASS_DARKLORD ||
		lpObj->Class == CLASS_RAGEFIGHTER)
	{
		if ( overLevel > 0 && max_over > 0 )
		{
			overLevel = (max_over/3)*2;
		}
	}

	return overLevel;
}

void DbItemSetInByte(LPOBJ lpObj, struct SDHP_DBCHAR_INFORESULT* lpMsg, int ItemDbByte, bool* bAllItemExist)
{
	int n;
	int itype;
	int _type;
	CItem item;
	BYTE OptionData;
	WORD hiWord;
	WORD loWord;
	bool bIsItemExist = true;

	for(n = 0; n < ReadConfig.INVENTORY_SIZE(lpObj->m_Index,false);n++)
	{
		bIsItemExist = true;
		lpObj->pInventory[n].Clear();
		itype = lpMsg->dbInventory[n*ItemDbByte];

		if(ItemDbByte >= 16)
		{
			if(lpMsg->dbInventory[n*ItemDbByte] == 0xFF
				&& (lpMsg->dbInventory[n*ItemDbByte+DBI_NOPTION_DATA] & 0x80) == 0x80
				&& (lpMsg->dbInventory[n*ItemDbByte+DBI_OPTION380_DATA] & 0xF0) == 0xF0)
			{
				continue;
			}
		}
		else if(ItemDbByte > 7)
		{
			if(lpMsg->dbInventory[n*ItemDbByte] == 0xFF
				&& (lpMsg->dbInventory[n*ItemDbByte+DBI_NOPTION_DATA] & 0x80) == 0x80)
			{
				continue;
			}
		}
		else
		{
			if(lpMsg->dbInventory[n*ItemDbByte+DBI_TYPE] == 0xFF)
			{
				continue;
			}
		}

		if(ItemDbByte == 16)
		{
			itype |= (lpMsg->dbInventory[n*ItemDbByte+DBI_OPTION380_DATA] & 0xF0) * 32;
			itype |= (lpMsg->dbInventory[n*ItemDbByte+DBI_NOPTION_DATA] & 0x80) * 2;
			_type = itype;
		}
		else if(ItemDbByte == 10)
		{
			if((lpMsg->dbInventory[n*ItemDbByte+DBI_NOPTION_DATA] & 0x80) == 0x80)
			{
				itype += 256;
			}

			_type = ((itype / 32) * 512) + itype % 32;
		}
		else
		{
			_type = ((itype / 16) * 512) + itype % 16;
		}

		if(ItemDbByte >= 16)
		{
			if(lpMsg->dbInventory[n*ItemDbByte] == 0xFF
				&& (lpMsg->dbInventory[n*ItemDbByte+DBI_NOPTION_DATA] & 0x80) == 0x80
				&& (lpMsg->dbInventory[n*ItemDbByte+DBI_OPTION380_DATA] & 0xF0) == 0xF0)
			{
				itype = -1;
			}
		}
		else if(ItemDbByte >= 10)
		{
			if(lpMsg->dbInventory[n*ItemDbByte] == 0xFF
				&& (lpMsg->dbInventory[n*ItemDbByte+DBI_NOPTION_DATA] & 0x80) == 0x80)
			{
				itype = -1;
			}
		}
		else
		{
			if(lpMsg->dbInventory[n*ItemDbByte+DBI_TYPE] == 0xFF)
			{
				itype = -1;
			}
		}

#if (DSGN_ITEM_DBMOVE == 1)
		if ( lpMsg->DbVersion < DS_Mover.highestDBNumber )
		{
			int newNumber = DS_Mover.CheckTransformItem(_type,lpMsg->DbVersion);

			if (newNumber > -1 && _type != newNumber)
			{
				DSGN_ITEMMOVE_LOG.Output("[ItemMove][Inv][%s][%s] Moving Item to new Db Number: oldId:%d, oldDb:%d, newId:%d, newDb:%d ",
					lpObj->AccountID,lpObj->Name,
					_type, lpMsg->DbVersion,
					newNumber, DS_Mover.highestDBNumber
				);

				item.m_Type = newNumber;
				_type = newNumber;
				itype = newNumber;
			}
		}
#endif

		if( IsItem(_type) == false )
		{
			bIsItemExist = 0;

			if(bAllItemExist != 0)
			{
				*bAllItemExist = 0;
			}
		}

		if(_type == ITEMGET(14,17) || _type == ITEMGET(14,18))
		{
			if(lpMsg->dbInventory[n*ItemDbByte+DBI_DUR]==0)
			{
				itype = -1;
			}
		}

		if (_type == ITEMGET(13,46) ||	//Devil Square free admission
			_type == ITEMGET(13,47) ||	//Blood Castle free tickets
			_type == ITEMGET(13,48) ||	//Kalima Ticket free tickets
			_type == ITEMGET(13,121) ||	//Open Access Ticket to Chaos
			_type == ITEMGET(13,125) ||	//Open Access Ticket to Doppel
			_type == ITEMGET(13,126) ||	//Open Access Ticket to Varka
			_type == ITEMGET(13,127))	//Open Access Ticket to Varka 7
		{
			if(lpMsg->dbInventory[n*ItemDbByte+DBI_DUR]==0)
			{
				itype = -1;
			}
		}

		if (_type == ITEMGET(14,19) )
		{
			if(lpMsg->dbInventory[n*ItemDbByte+DBI_DUR]==(BYTE)-1)
			{
				itype = -1;
			}
		}

		if(itype != -1)
		{
			item.m_Level = DBI_GET_LEVEL(lpMsg->dbInventory[n*ItemDbByte+DBI_OPTION_DATA]);

			if(lpMsg->DbVersion < 2)
			{
				lpMsg->dbInventory[n*ItemDbByte+DBI_SOPTION_DATA] = -1;
				lpMsg->dbInventory[n*ItemDbByte+DBI_OPTION380_DATA] = -1;
			}

			OptionData = lpMsg->dbInventory[n*ItemDbByte+DBI_OPTION_DATA];
			item.m_SkillOption = DBI_GET_SKILL(OptionData);
			item.m_LuckOption = DBI_GET_LUCK(OptionData);
			item.m_Z28Option = DBI_GET_OPTION(OptionData);

			if(ItemDbByte >= 10)
			{
				if(_type == ITEMGET(13,3))
				{
					item.m_Z28Option |= DBI_GET_OPTION16(lpMsg->dbInventory[n*ItemDbByte+DBI_NOPTION_DATA]) >> 4;
				}
				else
				{
					if ( DBI_GET_OPTION16(lpMsg->dbInventory[n*ItemDbByte+DBI_NOPTION_DATA]) == 0x40 )
						item.m_Z28Option += 4;	// Set +24
				}
			}

			item.m_Durability = lpMsg->dbInventory[n*ItemDbByte+DBI_DUR];

			if(ItemDbByte >= 16)
			{
				item.m_JewelOfHarmonyOption = lpMsg->dbInventory[n*ItemDbByte+DBI_JOH_DATA];
			}

			if(ItemDbByte >= 16)
			{

#if(ENABLE_CSHOP == 0)
				item.m_ItemOptionEx = DBI_GET_380OPTION(lpMsg->dbInventory[n*ItemDbByte+DBI_OPTION380_DATA]);

				if(item.m_ItemOptionEx)
				{
					item.m_Type = itype;
					if(g_kItemSystemFor380.Is380Item(&item)==false)
					{
						item.m_ItemOptionEx = 0;
						LogAddTD("[380Item][%s][%s] Invalid 380 Item Option in Inventory pos[%d]",
							lpObj->AccountID,lpObj->Name,n);
					}
				}
#else
				BYTE tmp = lpMsg->dbInventory[n*ItemDbByte+DBI_OPTION380_DATA] & 2;
				if( ( (((itype & 0x1E00 ) >> 5)+2) == lpMsg->dbInventory[n*ItemDbByte+DBI_OPTION380_DATA]) && (tmp == 2))
				{
					item.m_ItemOptionEx = lpMsg->dbInventory[n*ItemDbByte+DBI_OPTION380_DATA];
					//item.m_ItemOptionEx = 7;
				}else
				{
					item.m_ItemOptionEx = DBI_GET_380OPTION(lpMsg->dbInventory[n*ItemDbByte+DBI_OPTION380_DATA]);
					if(item.m_ItemOptionEx)
					{
						item.m_Type = itype;

						if(g_kItemSystemFor380.Is380Item(&item)==false)
						{
							item.m_ItemOptionEx = 0;
							LogAddTD("[380Item][%s][%s] Invalid 380 Item Option in Inventory pos[%d]",
								lpObj->AccountID,lpObj->Name,n);
						}
					}
				}
#endif
			}

			if(ItemDbByte >= 16)
			{
				if(IsSlotItem(_type))
				{
					item.m_ItemSlot1 = lpMsg->dbInventory[n*ItemDbByte+11];
					item.m_ItemSlot2 = lpMsg->dbInventory[n*ItemDbByte+12];
					item.m_ItemSlot3 = lpMsg->dbInventory[n*ItemDbByte+13];
					item.m_ItemSlot4 = lpMsg->dbInventory[n*ItemDbByte+14];
					item.m_ItemSlot5 = lpMsg->dbInventory[n*ItemDbByte+15];

					if(item.m_ItemSlot1 + item.m_ItemSlot2 + item.m_ItemSlot3 + item.m_ItemSlot4 +item.m_ItemSlot5 == 0)
					{
						if(ReadConfig.Soket__IsRecoverySlot == 1)
						{
							int dwExOpCount = getNumberOfExcOptions(item.m_NewOption);

							//DebugLog("[Slot Recovery][%s][%s] [INV] %s Passed: %d Returned: %d",__FUNCTION__,lpObj->AccountID,lpObj->Name,item.m_NewOption,dwExOpCount);

							if (dwExOpCount>=ReadConfig.Soket__MinExcOptionsCountOnItem)
							{
								switch(ReadConfig.Soket__SlotsToAdd)
								{
									case 1:
									{
										item.m_ItemSlot1 = 0xFF;
									}break;
									case 2:
									{
										item.m_ItemSlot1 = 0xFF;
										item.m_ItemSlot2 = 0xFF;
									}break;
									case 3:
									{
										item.m_ItemSlot1 = 0xFF;
										item.m_ItemSlot2 = 0xFF;
										item.m_ItemSlot3 = 0xFF;
									}break;
									case 4:
									{
										item.m_ItemSlot1 = 0xFF;
										item.m_ItemSlot2 = 0xFF;
										item.m_ItemSlot3 = 0xFF;
										item.m_ItemSlot4 = 0xFF;
									}break;
									case 5:
									{
										item.m_ItemSlot1 = 0xFF;
										item.m_ItemSlot2 = 0xFF;
										item.m_ItemSlot3 = 0xFF;
										item.m_ItemSlot4 = 0xFF;
										item.m_ItemSlot5 = 0xFF;
									}break;
									
								}
							}
						}
					}else
					{
						if(ReadConfig.Soket__AutoRemoveSameSocket == 1)
						{
							int Lvl1 = item.m_ItemSlot1 / 50;
							int Val1 = item.m_ItemSlot1 - (Lvl1 * 50);

							int Lvl2 = item.m_ItemSlot2 / 50;
							int Val2 = item.m_ItemSlot2 - (Lvl2 * 50);

							int Lvl3 = item.m_ItemSlot3 / 50;
							int Val3 = item.m_ItemSlot3 - (Lvl3 * 50);

							int Lvl4 = item.m_ItemSlot4 / 50;
							int Val4 = item.m_ItemSlot4 - (Lvl4 * 50);

							int Lvl5 = item.m_ItemSlot5 / 50;
							int Val5 = item.m_ItemSlot5 - (Lvl5 * 50);

							if(item.m_ItemSlot1 != 0xFF && item.m_ItemSlot1 != 0)
							{
								if(Val1 == Val2 || Val1 == Val3 || Val1 == Val4 || Val1 == Val5)
								{
									Val1=0;
									item.m_ItemSlot1 = 0xFF;
								}
							}
							if(item.m_ItemSlot2 != 0xFF && item.m_ItemSlot2 != 0)
							{
								if(Val2 == Val1 || Val2 == Val3 || Val2 == Val4 || Val2 == Val5)
								{
									Val2=0;
									item.m_ItemSlot2 = 0xFF;
								}
							}
							if(item.m_ItemSlot3 != 0xFF && item.m_ItemSlot3 != 0)
							{
								if(Val3 == Val1 || Val3 == Val2 || Val3 == Val4 || Val3 == Val5)
								{
									Val3=0;
									item.m_ItemSlot3 = 0xFF;
								}
							}
							if(item.m_ItemSlot4 != 0xFF && item.m_ItemSlot4 != 0)
							{
								if(Val4 == Val1 || item.m_ItemSlot4 == Val2 || Val4 == Val3 || Val4 == Val5)
								{
									Val4=0;
									item.m_ItemSlot4 = 0xFF;
								}
							}
							if(item.m_ItemSlot5 != 0xFF && item.m_ItemSlot5 != 0)
							{
								if(Val5 == Val1 || Val5 == Val2 || Val5 == Val3 || Val5 == Val4)
								{
									Val5=0;
									item.m_ItemSlot5 = 0xFF;
								}
							}
						}
					}
				} else {
					item.m_ItemSlot1 = 0;
					item.m_ItemSlot2 = 0;
					item.m_ItemSlot3 = 0;
					item.m_ItemSlot4 = 0;
					item.m_ItemSlot5 = 0;
				}
			}

			if(item.m_Level > MAX_ITEM_LEVEL)
			{
				DSGN_ITEMFIX_LOG.Output("[ItemFix][%s][%s] Item level too HIGH: t:%d, l:%d, z:%d, ex:%d, st:%d, sn:%d ",
					lpObj->AccountID,lpObj->Name,
					item.m_Type, item.m_Level, item.m_Z28Option, item.m_NewOption, item.m_SetOption, item.m_Number
				);
				item.m_Level = 10;
			}

			if(ItemDbByte == 7)
			{
				item.Convert(itype,item.m_SkillOption,item.m_LuckOption,item.m_Z28Option,0,0,0,0);
			}
			else if(ItemDbByte <= 10)
			{
				item.Convert(itype,item.m_SkillOption,item.m_LuckOption,item.m_Z28Option,DBI_GET_NOPTION(lpMsg->dbInventory[n*ItemDbByte+DBI_NOPTION_DATA]),lpMsg->dbInventory[n*ItemDbByte+DBI_SOPTION_DATA],item.m_ItemOptionEx,1);
			}
			else
			{
				item.Convert(itype,item.m_SkillOption,item.m_LuckOption,item.m_Z28Option,DBI_GET_NOPTION(lpMsg->dbInventory[n*ItemDbByte+DBI_NOPTION_DATA]),lpMsg->dbInventory[n*ItemDbByte+DBI_SOPTION_DATA],item.m_ItemOptionEx,3);
			}

#if (DSGN_ITEM_MONITOR==1)
			if (itype >= ITEMGET(12,0))
			{
				//Rings
				if ( ((itype >= ITEMGET(13,8))&&(itype <= ITEMGET(13,10))) ||
						((itype >= ITEMGET(13,12))&&(itype <= ITEMGET(13,13))) ||
						((itype >= ITEMGET(13,20))&&(itype <= ITEMGET(13,28))) ||
						itype == ITEMGET(13,107) )
				{
					if ( getNumberOfExcOptions(item.m_ItemOptionEx) > DSGN_ITEM_MON_RING_EXC )
					{
						DSGN_RINGFIX_LOG.Output("[ItemFix][Before][%s][%s] Ring/Pendant exc options fix: t:%d, l:%d, z:%d, ex:%d, st:%d, sn:%d ",
							lpObj->AccountID,lpObj->Name,
							item.m_Type, item.m_Level, item.m_Z28Option, item.m_NewOption, item.m_SetOption, item.m_Number
						);

						#if (DSGN_ITEM_FIX_RING==1)
							item.m_ItemOptionEx = BoxExcOptions(DSGN_ITEM_MON_RING_EXC);

							DSGN_RINGFIX_LOG.Output("[ItemFix][After][%s][%s] Ring/Pendant exc options fix: t:%d, l:%d, z:%d, ex:%d, st:%d, sn:%d ",
								lpObj->AccountID,lpObj->Name,
								item.m_Type, item.m_Level, item.m_Z28Option, item.m_NewOption, item.m_SetOption, item.m_Number
						);
						#endif
					}

					if(item.m_Level > DSGN_ITEM_MON_RING_LVL)
					{
						DSGN_RINGFIX_LOG.Output("[ItemFix][%s][%s] Ring/Pendant level fix: t:%d, l:%d, z:%d, ex:%d, st:%d, sn:%d ",
							lpObj->AccountID,lpObj->Name,
							item.m_Type, item.m_Level, item.m_Z28Option, item.m_NewOption, item.m_SetOption, item.m_Number
						);

						#if (DSGN_ITEM_FIX_RING==1)
							item.m_Level = DSGN_ITEM_MON_RING_LVL;
						#endif
					}

					if(item.m_Z28Option > DSGN_ITEM_MON_RING_Z28)
					{
						DSGN_RINGFIX_LOG.Output("[ItemFix][%s][%s] Ring/Pendant ZOption fix: t:%d, l:%d, z:%d, ex:%d, st:%d, sn:%d ",
							lpObj->AccountID,lpObj->Name,
							item.m_Type, item.m_Level, item.m_Z28Option, item.m_NewOption, item.m_SetOption, item.m_Number
						);

						#if (DSGN_ITEM_FIX_RING==1)
							item.m_Z28Option = DSGN_ITEM_MON_RING_Z28;
						#endif
					}
				}

				//Wings
				if ( (itype >= ITEMGET(12,0) && itype <= ITEMGET(12,6) ) ||
					 (itype >= ITEMGET(12,36) && itype <= ITEMGET(12,43) ) ||
					 (itype >= ITEMGET(12,130) && itype <= ITEMGET(12,135) ) ||
					  itype == ITEMGET(13,30) ||
					  itype == ITEMGET(12,49) ||
					  itype == ITEMGET(12,50) ||
					  itype == ITEMGET(13,3)  ||
					  itype == ITEMGET(13,37) 		
						#if (CRYSTAL_EDITION == 1)
						|| ( itype >= ITEMGET(12,200) && itype <= ITEMGET(12,254) )
						#endif
					)
				{
					if ( getNumberOfExcOptions(item.m_ItemOptionEx) > DSGN_ITEM_MON_WING_EXC )
					{
						DSGN_WINGFIX_LOG.Output("[ItemFix][Before][%s][%s] Wings/Other exc options fix: t:%d, l:%d, z:%d, ex:%d, st:%d, sn:%d ",
							lpObj->AccountID,lpObj->Name,
							item.m_Type, item.m_Level, item.m_Z28Option, item.m_NewOption, item.m_SetOption, item.m_Number
						);

						#if (DSGN_ITEM_FIX_WING==1)
							item.m_ItemOptionEx = BoxExcOptions(DSGN_ITEM_MON_WING_EXC);

							DSGN_WINGFIX_LOG.Output("[ItemFix][After][%s][%s] Wings/Other exc options fix: t:%d, l:%d, z:%d, ex:%d, st:%d, sn:%d ",
								lpObj->AccountID,lpObj->Name,
								item.m_Type, item.m_Level, item.m_Z28Option, item.m_NewOption, item.m_SetOption, item.m_Number
							);
						#endif
					}
				}
			}
#endif

			if(_type == ITEMGET(14,7))
			{

			}
			else
			{
				if( (_type >= ITEMGET(14,0) && _type <= ITEMGET(14,8))  || 
					(_type >= ITEMGET(14,35) && _type <= ITEMGET(14,40))||
					(_type >= ITEMGET(14,70) && _type <= ITEMGET(14,71))||	//Elite Mana/Health potions
					(_type == ITEMGET(14,94)) ||	//Medium Elite Potion
					(_type == ITEMGET(14,133))		//Elite SD Potion
					)
				{
					if(item.m_Durability == 0.0f)
					{
						item.m_Durability = 1.0f;
					}

					if(item.m_Durability > 255.0f) //3.0f ERROR POTION FIX
					{
						item.m_Durability = 255.0f;
					}
				}
				else if(_type >= ITEMGET(14,46) && _type <= ITEMGET(14,50))
				{
					if(item.m_Durability == 0.0f)
					{
						item.m_Durability = 1.0f;
					}

					if(item.m_Durability > 3.0f)
					{
						item.m_Durability = 3.0f;
					}
				}
				//else if(_type >= ITEMGET(14,35) && _type <= ITEMGET(14,37))
				//{
				//	if(item.m_Durability == 0.0f)
				//	{
				//		item.m_Durability = 1.0f;
				//	}

				//	if(item.m_Durability > 1.0f)
				//	{
				//		item.m_Durability = 1.0f;
				//	}
				//}
				else if(_type != ITEMGET(13,10)
					&& _type != ITEMGET(14,29)	//Symbol of Kundun
					&& _type != ITEMGET(14,21)	//Mark of Lord
					&& _type != ITEMGET(14,100)	//Lucky Peny
					&& _type != ITEMGET(14,101)	//Paper Piece
					&& _type != ITEMGET(14,110)	//Goer Event
					&& _type != ITEMGET(14,153)	//QUEST ITEM
					&& _type != ITEMGET(14,154)	//QUEST ITEM
					&& _type != ITEMGET(14,155)	//QUEST ITEM
					&& _type != ITEMGET(14,156)	//QUEST ITEM
				)
				{
					if(item.m_Level == 3)
					{
						if(item.m_Durability > item.m_BaseDurability && bIsItemExist == 1)
						{
							item.m_Durability = item.m_BaseDurability;
						}
					}
				}
			}

			lpObj->pInventory[n].m_SkillOption = item.m_SkillOption;
			lpObj->pInventory[n].m_LuckOption = item.m_LuckOption;
			lpObj->pInventory[n].m_Z28Option = item.m_Z28Option;
			lpObj->pInventory[n].m_JewelOfHarmonyOption = item.m_JewelOfHarmonyOption;
			lpObj->pInventory[n].m_ItemOptionEx = item.m_ItemOptionEx;

			hiWord = ((lpMsg->dbInventory[n*ItemDbByte+DBI_SERIAL2])&0xFF)&0xFF | (((lpMsg->dbInventory[n*ItemDbByte+DBI_SERIAL1])&0xFF)&0xFF) * 256;
			loWord = ((lpMsg->dbInventory[n*ItemDbByte+DBI_SERIAL4])&0xFF)&0xFF | (((lpMsg->dbInventory[n*ItemDbByte+DBI_SERIAL3])&0xFF)&0xFF) * 256;

			item.m_Number = ((loWord &0xFFFF)&0xFFFF) | ((hiWord & 0xFFFF)&0xFFFF) << 16;

			gObjInventoryInsertItemPos(lpObj->m_Index,item,n,0);

			if(bIsItemExist == false)
			{
				lpObj->pInventory[n].m_bItemExist = 0;

				LogAddTD("error-L2 : Unknown Item found [%s][%s] (Type:%d, LEV:%d, DUR:%d OP1:%d, OP2:%d, OP3:%d, NEWOP:%d, SET:%d)",
					lpObj->AccountID,lpObj->Name,lpObj->pInventory[n].m_Type,lpObj->pInventory[n].m_Level,
					lpObj->pInventory[n].m_Durability,lpObj->pInventory[n].m_SkillOption,
					lpObj->pInventory[n].m_LuckOption,lpObj->pInventory[n].m_Z28Option,
					lpObj->pInventory[n].m_NewOption,lpObj->pInventory[n].m_SetOption);

				lpObj->pInventory[n].Clear();
			}
		}
	}
	gObjRequestPetItemInfo(lpObj->m_Index, 0);
}



void gObjSetBP(int aIndex)
{
	int Strength = gObj[aIndex].Strength + gObj[aIndex].AddStrength;
	int Dexterity = gObj[aIndex].Dexterity + gObj[aIndex].AddDexterity;
	int Vitality = gObj[aIndex].Vitality + gObj[aIndex].AddVitality;
	int Energy = gObj[aIndex].Energy + gObj[aIndex].AddEnergy;
	int Leadership = gObj[aIndex].Leadership + gObj[aIndex].AddLeadership;

	switch ( gObj[aIndex].Class )
	{
		case CLASS_WIZARD:
			gObj[aIndex].MaxBP = (Strength * 0.2) + (Dexterity * 0.4) + (Vitality * 0.3) + (Energy * 0.2);
			break;

		case CLASS_KNIGHT:
			gObj[aIndex].MaxBP = (Strength * 0.15) + (Dexterity * 0.2) + (Vitality * 0.3) + (Energy * 1.0);
			break;

		case CLASS_ELF:
			gObj[aIndex].MaxBP = (Strength * 0.3) + (Dexterity * 0.2) + (Vitality * 0.3) + (Energy * 0.2);
			break;

		case CLASS_MAGICGLADIATOR:
			gObj[aIndex].MaxBP = (Strength * 0.2) + (Dexterity * 0.25) + (Vitality * 0.3) + (Energy * 0.15);
			break;

		case CLASS_DARKLORD:
			gObj[aIndex].MaxBP = (Strength * 0.3) + (Dexterity * 0.2) + (Vitality * 0.1) + (Energy * 0.15) + (Leadership * 0.3);
			break;

		case CLASS_SUMMONER:
			gObj[aIndex].MaxBP = (Strength * 0.2) + (Dexterity * 0.4) + (Vitality * 0.3) + (Energy * 0.2);
			break;

		case CLASS_RAGEFIGHTER:
			gObj[aIndex].MaxBP = (Strength * 0.15) + (Dexterity * 0.2) + (Vitality * 0.3) + (Energy * 1.0);			
			break;

		default:
			gObj[aIndex].MaxBP = (Strength * 0.2) + (Dexterity * 0.4) + (Vitality * 0.3) + (Energy * 0.2);
			break;
	}
}


BOOL gObjSetCharacter(LPBYTE lpdata, int aIndex)
{
	int itype;
	int n;
	char szCharName[MAX_ACCOUNT_LEN+1];
	SDHP_DBCHAR_INFORESULT * lpMsg = (SDHP_DBCHAR_INFORESULT *)lpdata;

	// Check in position
	if ( OBJMAX_RANGE(aIndex) == FALSE )
	{
		LogAdd("error : %s %d", __FILE__, __LINE__);
		return FALSE;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if ( lpObj->Connected < PLAYER_LOGGED )
	{
		return FALSE;
	}

	if ( gObj[aIndex].Connected == PLAYER_PLAYING )
	{
		LogAddC(2, lMsg.Get(MSGGET(1, 242)), gObj[aIndex].AccountID, gObj[aIndex].Name);
		return FALSE;
	}

	if ( lpMsg->Class == 0x00 	||
		lpMsg->Class == 0x02 ||
		lpMsg->Class == 0x12 ||
		lpMsg->Class == 0x22 ||
		lpMsg->Class == 0x41 ||
		lpMsg->Class == 0x42 ||
		 lpMsg->Class == 0x10 	|| 
	 	 lpMsg->Class == 0x20 ||
	 	 lpMsg->Class == 0x30 || 
		 lpMsg->Class == 0x31 ||
		 lpMsg->Class == 0x32 ||
		 lpMsg->Class == 0x01 || 
		 lpMsg->Class == 0x11 || 
		 lpMsg->Class == 0x21 || 
		 lpMsg->Class == 0x40 ||
		 lpMsg->Class == 0x50 || 
		 lpMsg->Class == 0x51 || 
		 lpMsg->Class == 0x52 ||
		 lpMsg->Class == 0x60 || 
		 lpMsg->Class == 0x61 || 
		 lpMsg->Class == 0x62
		 )
	{

	}
	else
	{
		LogAdd("error--L1 : Class not found");
		return FALSE;
	}

	gObjCharZeroSet(aIndex);

	if ( ChaosBoxInit(lpObj) == FALSE )
	{
		LogAdd("error-L1 : ChaosBox Init error %s %d", __FILE__, __LINE__);
	}

	lpObj->m_PosNum = 0;
	lpObj->Level = lpMsg->Level;
	lpObj->LevelUpPoint = lpMsg->LevelUpPoint;
	lpObj->DbClass = lpMsg->Class;
	lpObj->Class = lpMsg->Class >> 4;
	lpObj->ChangeUP = lpMsg->Class & 0x07;	// Set Second Type of Character

	szCharName[MAX_ACCOUNT_LEN] = 0;
	memcpy(szCharName, lpMsg->Name , MAX_ACCOUNT_LEN);
	strcpy(lpObj->Name, szCharName);

	if ( lpObj->Level < 1 )
	{
		LogAddC(2, "error : %s Level Zero", lpMsg->Name);
		return FALSE;
	}

	lpObj->X = lpMsg->MapX;
	lpObj->Y = lpMsg->MapY;
	lpObj->MapNumber = lpMsg->MapNumber;
	lpObj->StartX = lpObj->X;
	lpObj->StartY = lpObj->Y;
		//lpObj->m_OldX = lpObj->X;
		//lpObj->m_OldY = lpObj->Y;
	
	if ( MAX_MAP_RANGE(lpObj->MapNumber) == FALSE )
	{
		LogAdd("error : Map Number max over. %s %d", __FILE__, __LINE__);
		lpObj->MapNumber = MAP_INDEX_LORENCIA;
		MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber, (short &)lpObj->X,(short &)lpObj->Y);
	}

	if ( lpObj->Level < 6 || DS_MAP_RANGE(lpObj->MapNumber) != FALSE )
	{
		if ( lpObj->Class == CLASS_ELF )
		{
			lpObj->MapNumber = MAP_INDEX_NORIA;
			MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber, (short &)lpObj->X,(short &)lpObj->Y);
		} 
		else if ( lpObj->Class == CLASS_SUMMONER )
		{
			lpObj->MapNumber = MAP_INDEX_ELBELAND;
			MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber, (short &)lpObj->X,(short &)lpObj->Y);
		}
		else
		{
			lpObj->MapNumber = MAP_INDEX_LORENCIA;
			MapC[lpObj->MapNumber].GetLevelPos(lpObj->Level, (short &)lpObj->X,(short &)lpObj->Y);
		}
	}
	if ( lpObj->MapNumber == MAP_INDEX_RAKLIONBOSS)
	{
		lpObj->MapNumber = MAP_INDEX_RAKLION;
		MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber, (short &)lpObj->X,(short &)lpObj->Y);
	}
	if ( lpObj->MapNumber == MAP_INDEX_DUELMAP)
	{
		lpObj->MapNumber = MAP_INDEX_VULCANUS;
		MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber, (short &)lpObj->X,(short &)lpObj->Y);
	}
	if ( lpObj->MapNumber == MAP_INDEX_REFUGE)
	{
		lpObj->MapNumber = MAP_INDEX_BARRACKS;
		MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber, (short &)lpObj->X,(short &)lpObj->Y);
	}

	if ( IT_MAP_RANGE(lpObj->MapNumber) != FALSE )
	{
		lpObj->MapNumber = MAP_INDEX_DEVIAS;
		MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber, (short &)lpObj->X,(short &)lpObj->Y);
	}
#if (PACK_EDITION>=2)
	if ( IMPERIALGUARDIAN_MAP_RANGE(lpObj->MapNumber) != FALSE )
	{
		lpObj->MapNumber = MAP_INDEX_DEVIAS;
		MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber, (short &)lpObj->X,(short &)lpObj->Y);
	}
#endif

#if (PACK_EDITION>=3)
	if ( DG_MAP_RANGE(lpObj->MapNumber) != FALSE )
	{
		lpObj->MapNumber = MAP_INDEX_ELBELAND;
		MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber, (short &)lpObj->X,(short &)lpObj->Y);
	}

	#if (GS_CASTLE==1)
	if ( lpObj->MapNumber == MAP_INDEX_SWAMP)
	{
		if(g_Swamp.SwampMapEnterOnlyWhenStarted == 1 && g_Swamp.Enabled)
		{
			if(g_Swamp.Start == FALSE)
			{
				lpObj->MapNumber = MAP_INDEX_SWAMP;
				MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber, (short &)lpObj->X,(short &)lpObj->Y);
			}
		}
	}
	#endif
#endif

	if ( lpObj->MapNumber == MAP_INDEX_XMAS)
	{
		lpObj->MapNumber = MAP_INDEX_DEVIAS;
		MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber, (short &)lpObj->X,(short &)lpObj->Y);
	}

	if ( lpObj->MapNumber ==  MAP_INDEX_CASTLEHUNTZONE )
	{
		int mgt = 106;
		short x,y,level;
		BYTE map = lpObj->MapNumber,dir;

		int result = gGateC.GetGate(mgt,(short &)x,(short &)y,(BYTE &)map,(BYTE &)dir,(short &)level);
		if(result >= 0)
		{
			lpObj->MapNumber = map;
			lpObj->X = x;
			lpObj->Y = y;
		}else
		{
			lpObj->MapNumber = MAP_INDEX_CASTLESIEGE;
			MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber, (short &)lpObj->X,(short &)lpObj->Y);
		}

		#if (GS_CASTLE==1)
		//Castle Siege State Send Fix [NEW]
		if ( g_CastleSiege.GetCastleState() == 7 )
		{
			GCAnsCsNotifyStart(lpObj->m_Index, 1);
			g_CastleSiege.SetUserCsJoinSide(lpObj->m_Index);
			g_CastleSiege.NotifySelfCsJoinSide(lpObj->m_Index);
			g_CastleSiege.NotifyCsSelfLeftTime(lpObj->m_Index);
		} else {
			GCAnsCsNotifyStart(lpObj->m_Index, 0);
		}
		#endif
	}

	if ( BC_MAP_RANGE(lpObj->MapNumber) != FALSE )
	{
		lpObj->MapNumber = MAP_INDEX_DEVIAS;
		MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber, (short &)lpObj->X,(short &)lpObj->Y);
	}

	if ( KALIMA_MAP_RANGE(lpObj->MapNumber) != FALSE )
	{
		lpObj->MapNumber = MAP_INDEX_DEVIAS;
		MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber, (short &)lpObj->X,(short &)lpObj->Y);
	}

	if ( lpObj->MapNumber == MAP_INDEX_CRYWOLF_FIRSTZONE )
	{
		if (g_CrywolfSync.GetCrywolfState() >= CRYWOLF_STATE_NOTIFY_1 && 
			g_CrywolfSync.GetCrywolfState() < CRYWOLF_STATE_START)
		{
			lpObj->MapNumber = MAP_INDEX_CRYWOLF_FIRSTZONE;
			MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber, (short &)lpObj->X,(short &)lpObj->Y);
		}

		g_CrywolfSync.NotifyCrywolfCurrentStateByUserId(lpObj->m_Index);
	}

	lpObj->m_cBloodCastleIndex = -1;
	lpObj->m_cBloodCastleSubIndex = -1;
	lpObj->m_iBloodCastleEXP = 0;
	lpObj->m_bBloodCastleComplete = 0;

	if ( CC_MAP_RANGE(lpObj->MapNumber) != FALSE )
	{
		lpObj->MapNumber = MAP_INDEX_DEVIAS;
		MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber, (short &)lpObj->X,(short &)lpObj->Y);
	}
	
	lpObj->m_cChaosCastleIndex = -1;
	lpObj->m_cChaosCastleSubIndex = -1;
	lpObj->m_iChaosCastleBlowTime = 0;
	lpObj->m_cKillUserCount = 0;
	lpObj->m_cKillMonsterCount = 0;

	if ( lpObj->MapNumber == MAP_INDEX_KANTURU_BOSS )
	{
		short sX;
		short sY;
		short sLVL;
		unsigned char btMAPNUM = lpObj->MapNumber;
		unsigned char btDIR;

		int iMapNumber = gGateC.GetGate(137,(short &)sX,(short &)sY,(BYTE &)btMAPNUM,(BYTE &)btDIR,(short &)sLVL);

		if(iMapNumber >= 0)
		{
			lpObj->MapNumber = btMAPNUM;
			lpObj->X = sX;
			lpObj->Y = sY;
		}
	}

	if ( lpObj->m_bMapSvrMoveReq == true )
	{
		short sSVR_CODE = ::g_MapServerManager.CheckMoveMapSvr(lpObj->m_Index, lpObj->m_sDestMapNumber, lpObj->m_sPrevMapSvrCode);

		if ( sSVR_CODE == gGameServerCode )
		{
			lpObj->MapNumber = lpObj->m_sDestMapNumber;
			lpObj->X = lpObj->m_btDestX;
			lpObj->Y = lpObj->m_btDestY;
			lpObj->StartX = lpObj->X;
			lpObj->StartY = lpObj->Y;
				//lpObj->m_OldX = lpObj->X;
				//lpObj->m_OldY = lpObj->Y;
		}
		else	
		{
			LogAddC(2, "[MapServerMng] Dest Map Server doesn't have Map [%s][%s] : %d",
				lpObj->AccountID, lpObj->Name, lpObj->m_sDestMapNumber);
			gObjDel(lpObj->m_Index);

			return FALSE;
		}
	}

	BYTE attr = MapC[lpObj->MapNumber].GetAttr(lpObj->X, lpObj->Y);
	int result;
	short x;
	short y;
	BYTE mapNumber;
	BYTE dir;
	short level;
	int gt;
	int userlevel;

	if ( (attr&4) == 4 || (attr&8) == 8 )
	{
		LogAdd(lMsg.Get(MSGGET(1, 243)));
		MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber, (short &)lpObj->X,(short &)lpObj->Y);
	}

	mapNumber = MAP_INDEX_LORENCIA;
	userlevel = gObj[aIndex].Level;
	x = lpObj->X;
	y = lpObj->Y;
	dir = lpObj->Dir;
	level = lpObj->Level;
	mapNumber = lpObj->MapNumber;
	gt = 0;

	if(g_iBlockKanturuMapEnter == 1)
	{
		if(gObj[aIndex].MapNumber >= MAP_INDEX_KANTURU1 && gObj[aIndex].MapNumber <= MAP_INDEX_KANTURU_BOSS)
		{
			LogAddTD("[Kanturu][Forced Move to Lorencia] (%s)(%s) (BeforeMap:%d(%d/%d))",
				gObj[aIndex].AccountID,gObj[aIndex].Name,gObj[aIndex].MapNumber,gObj[aIndex].X,gObj[aIndex].Y);
			gObj[aIndex].MapNumber = MAP_INDEX_LORENCIA;
			MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber,(short &)lpObj->X,(short &)lpObj->Y);
		}
	}

	if(g_iBlockCastleSiegeMapEnter == 1)
	{
		if(gObj[aIndex].MapNumber == MAP_INDEX_CASTLESIEGE
			|| gObj[aIndex].MapNumber == MAP_INDEX_CASTLEHUNTZONE
			|| gObj[aIndex].MapNumber == MAP_INDEX_CRYWOLF_FIRSTZONE
			|| gObj[aIndex].MapNumber == MAP_INDEX_CRYWOLF_SECONDZONE)
		{
			LogAddTD("[CastleSiege][Forced Move to Lorencia] (%s)(%s) (BeforeMap:%d(%d/%d))",
				gObj[aIndex].AccountID,gObj[aIndex].Name,gObj[aIndex].MapNumber,gObj[aIndex].X,gObj[aIndex].Y);
			gObj[aIndex].MapNumber = MAP_INDEX_LORENCIA;
			MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber,(short &)lpObj->X,(short &)lpObj->Y);
		}
	}

	if ( lpObj->MapNumber == MAP_INDEX_DEVIAS )
	{
		gt = 22;
	}
	else if ( lpObj->MapNumber == MAP_INDEX_NORIA )
	{
		gt = 27;
	}
	else if ( lpObj->MapNumber == MAP_INDEX_LOSTTOWER )
	{
		gt = 42;
	}

	if ( gt > 0 )
	{
		result = gGateC.GetGate(gt, x, y, mapNumber, dir, level);

		if ( result >= 0 )
		{
			if (lpObj->Class == CLASS_MAGICGLADIATOR || 
				lpObj->Class == CLASS_DARKLORD ||
				lpObj->Class == CLASS_RAGEFIGHTER)
			{
				if ( level > 0 )
				{
					level = level / 3 * 2;
				}
			}

			if ( level != 0 && lpObj->Level < level)
			{
				if ( lpObj->Class == CLASS_ELF ) // Elf
				{
					lpObj->MapNumber = MAP_INDEX_NORIA;
					MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber, (short &)lpObj->X,(short &)lpObj->Y);
				}
				else
				{
					lpObj->MapNumber = MAP_INDEX_LORENCIA;
					MapC[lpObj->MapNumber].GetLevelPos(lpObj->Level, (short &)lpObj->X,(short &)lpObj->Y);
				}
			}
		}
		else
		{
			LogAdd("error : %d (%s %d)", result, __FILE__, __LINE__);
		}
	}

	lpMsg->MapX = lpObj->X;
	lpMsg->MapY = lpObj->Y;
	lpObj->TX = lpMsg->MapX;
	lpObj->TY = lpMsg->MapY;
	lpObj->m_OldX = lpObj->X;
	lpObj->m_OldY = lpObj->Y;
	lpObj->Experience = lpMsg->Exp;
	lpObj->Strength = lpMsg->Str ;
	lpObj->Dexterity = lpMsg->Dex ;
	lpObj->Vitality = lpMsg->Vit ;
	lpObj->Energy = lpMsg->Energy ;
	lpObj->Life = lpMsg->Life;
	lpObj->Life /= 10.0f;
	lpObj->MaxLife = lpMsg->MaxLife;
	lpObj->MaxLife /= 10.0f;
	lpObj->Mana = lpMsg->Mana;
	lpObj->Mana /= 10.0f;
	lpObj->MaxMana = lpMsg->MaxMana;
	lpObj->MaxMana /= 10.0f;
	lpObj->Money = lpMsg->Money;
	lpObj->Leadership = lpMsg->Leadership;
	lpObj->ChatLimitTime = lpMsg->ChatLitmitTime;
	lpObj->iFruitPoint = lpMsg->iFruitPoint;

	if ( lpObj->Mana < 1.0f )
	{
		lpObj->Mana = 1.0f;
	}

	if ( lpObj->Life < 1.0f )
	{
		lpObj->Life = 1.0f;
	}

	lpObj->MaxLife = DCInfo.DefClass[lpObj->Class].Life + (lpObj->Level - 1) * DCInfo.DefClass[lpObj->Class].LevelLife  + ((lpObj->Vitality - DCInfo.DefClass[lpObj->Class].Vitality ) * DCInfo.DefClass[lpObj->Class].VitalityToLife);
	lpObj->MaxMana = DCInfo.DefClass[lpObj->Class].Mana + (lpObj->Level - 1) * DCInfo.DefClass[lpObj->Class].LevelMana  + ((lpObj->Energy - DCInfo.DefClass[lpObj->Class].Energy ) * DCInfo.DefClass[lpObj->Class].EnergyToMana);

	lpObj->VitalityToLife = DCInfo.DefClass[lpObj->Class].VitalityToLife;
	lpObj->EnergyToMana = DCInfo.DefClass[lpObj->Class].EnergyToMana;

	::gObjSetBP(aIndex);
	lpObj->BP = lpObj->MaxBP / 2;
	::gObjCalcMaxLifePower(aIndex);
	lpObj->m_PK_Count = lpMsg->PkCount;
	lpObj->m_PK_Level = lpMsg->PkLevel;
	lpObj->m_PK_Time  = (GetTickCount()-(lpMsg->PkTime*1000));

	lpObj->MaxRegenTime = 4000;	// Resurrection of the player if die
	lpObj->m_MoveSpeed = 1000;

	memcpy(lpObj->m_Quest, lpMsg->dbQuest, sizeof(lpObj->m_Quest));

	if ( lpObj->m_Quest[0] == 0 )
	{
		memset(lpObj->m_Quest, (BYTE)-1, sizeof(lpObj->m_Quest));
	}

	if ( ::g_QuestInfo.GetQuestState(lpObj, 2) == 2 )	// Ring of Honor
	{
		if ( (lpObj->Level < QUEST_MINLEVEL_PLUSSTAT) && (ReadConfig.ResetComboBelow220Level == 1) )	// Repair Bug of Marlon Before
		{
			::g_QuestInfo.ReSetQuestState(lpObj, 2);
			LogAddTD("[%s][%s] Find Invalid QuestInfo (%d)",
				lpObj->AccountID, lpObj->Name, 2);
		}
		else
		{
			lpObj->PlusStatQuestClear = true;
		}
	}

	if ( ::g_QuestInfo.GetQuestState(lpObj, 3) == 2 )	// Dark Stone
	{
		if ( (lpObj->Level < QUEST_MINLEVEL_PLUSSTAT) && (ReadConfig.ResetComboBelow220Level == 1) )	// Repair Bug of Marlon After
		{
			::g_QuestInfo.ReSetQuestState(lpObj, 3);
			LogAddTD("[%s][%s] Find Invalid QuestInfo (%d)",
				lpObj->AccountID, lpObj->Name, 3);
		}
		else
		{
			lpObj->ComboSkillquestClear = true;
		}
	}

	::gObjSetInventory1Pointer(lpObj);
	lpObj->GuildNumber = 0;
	lpObj->lpGuild = NULL;

	if ( lpObj->Class >= 0 && lpObj->Class <= 6 )
	{
		BOOL bEnergyBall = TRUE;
		BOOL bForceSkill = TRUE;
		BOOL bUpperBeastSkill = TRUE;
		BOOL IsGetInitinityArrowSkill = FALSE;// DarkLord

		//gObjMagicAddEnergyCheckOnOff(0);

		for ( n=0;n<MAX_MAGIC;n++)
		{
			itype = MAKEWORD(lpMsg->dbMagicList[n*3],lpMsg->dbMagicList[n*3+1]);

			if (( itype != (WORD)-1 ) && ( itype != (BYTE)-1 ))
			{
				if ( itype == AT_SKILL_ENERGYBALL )
				{
					bEnergyBall = FALSE;
				}

				if ( itype == AT_SKILL_SPEAR )
				{
					bForceSkill = FALSE;
				}

				if ( itype == AT_SKILL_INFINITY_ARROW )
				{
					IsGetInitinityArrowSkill = 1;
					gObjMagicAddResetCheck(lpObj,itype,lpMsg->dbMagicList[n*4+DBM_LEVEL]&0xF);
				}

				// Main Engine of Add Skills
				if (itype != AT_SKILL_BLOCKING && 
					itype != AT_SKILL_FALLINGSLASH && 
					itype != AT_SKILL_LUNGE && 
					itype != AT_SKILL_UPPERCUT && 
					itype != AT_SKILL_CYCLONE && 
					itype != AT_SKILL_SLASH && 
					
					itype != AT_SKILL_CROSSBOW && 
					itype != AT_SKILL_BOW && 

					itype != AT_SKILL_KNIGHTDINORANT && 
					itype != AT_SKILL_POWERSLASH && 
					itype != AT_SKILL_DARKHORSE_ATTACK && 
					itype != AT_SKILL_FENRIR_ATTACK && 
					itype != AT_SKILL_5CROSSBOW &&				//Skill Does not exist
					itype != AT_SKILL_SAHAMUTT && 
					itype != AT_SKILL_NAIL && 
					itype != AT_SKILL_GHOST_PHANTOM )
				{
					if(ReadConfig.S6E2 == 0)
					{
						if ( ((itype >= SKILL_SKILLTREE_START && itype <= SKILL_SKILLTREE_END) && (gObjIsNewClass(lpObj) == 1)) || (itype <= SKILL_SKILLTREE_START))
							gObjMagicAddResetCheck(lpObj, itype, lpMsg->dbMagicList[n*4+DBM_LEVEL] & 0x0F);
					}
					else
					{
						if(itype < SKILL_SKILLTREE_START)
							gObjMagicAddResetCheck(lpObj, itype, lpMsg->dbMagicList[n*4+DBM_LEVEL] & 0x0F);
						else
						{
							Mastering.S6E2RestoreMagic(lpObj->m_Index,itype);

							if(ReadConfig.IsRecoverMPoints == 1)
							{
								int restoreMasterPoints = (itype % 5) + 1;
								lpObj->MasterCharacterInfo->MasterPointsTmp += restoreMasterPoints;

								LogAddTD("[%s][%s] [Mastering] Restored %d Master Points from Skill: %d",
									lpObj->AccountID, lpObj->Name,
									restoreMasterPoints, itype);
							}
						}
					}
				}

				if(ReadConfig.S6E2 == 0)
				{
					if ((itype >= SKILL_SKILLTREE_START && 
						 itype <= SKILL_SKILLTREE_END) && 
						(gObjIsNewClass(lpObj) == 1)) //Masters Skills
					{
						if ( lpObj->Type == OBJ_USER )
							Mastering.LoadSkill(lpObj,lpObj->MasterCharacterInfo->MasterPoints,itype);
					}
				}

				if ( lpObj->DbClass == DB_RAGEFIGHER )
				{
					bUpperBeastSkill = FALSE;
				}
				if ( itype == AT_SKILL_UPPERBEAST && lpObj->DbClass == DB_RAGEFIGHER_EVO )
				{
					bUpperBeastSkill = FALSE;
				}
				if ( itype == AT_SKILL_UPPERBEAST && lpObj->DbClass == DB_RAGEFIGHER )
				{
					gObjMagicDel(lpObj,AT_SKILL_UPPERBEAST,0);
					bUpperBeastSkill = FALSE;
				}
			}
		}

		if ( lpObj->Class == CLASS_WIZARD )	// Wizard
		{
			if ( bEnergyBall == TRUE )
			{
				gObjMagicAdd(lpObj, AT_SKILL_ENERGYBALL, 0);
				bEnergyBall = FALSE;
			}
		}

		if ( lpObj->Class == CLASS_DARKLORD )	// Dark Lord
		{
			if ( bForceSkill == TRUE )
			{
				gObjMagicAdd(lpObj, AT_SKILL_SPEAR, 0);
				bForceSkill = FALSE;
			}
		}

		if ( IsGetInitinityArrowSkill == 0 && lpObj->Class == CLASS_ELF )
		{
			if(lpObj->Level >= g_SkillAdditionInfo.GetInfinityArrowUseLevel())
			{
				if(lpObj->ChangeUP >= 1)
				{
					int iAddSkillPosition = gObjMagicAdd(lpObj,AT_SKILL_INFINITY_ARROW,0);

					if(iAddSkillPosition > 0)
					{
						LogAddTD("[%s][%s] Add Infinity Arrow Skill (Character Level : %d)(ChangeUp: %d)",
							lpObj->AccountID,lpObj->Name,lpObj->Level,lpObj->ChangeUP);
					}
				}
			}
		}

		if ( lpObj->Class == CLASS_WIZARD )
		{
			gObjMagicAdd(lpObj, AT_SKILL_JAVALIN, 0);
		}
		else if ( lpObj->Class == CLASS_KNIGHT )
		{
			gObjMagicAdd(lpObj, AT_SKILL_CRESCENTSLASH, 0);
		}
		else if ( lpObj->Class == CLASS_ELF )
		{
			gObjMagicAdd(lpObj, AT_SKILL_STARFALL, 0);
		}
		else if ( lpObj->Class == CLASS_MAGICGLADIATOR )	// Magic Gladiator
		{
			gObjMagicAdd(lpObj, AT_SKILL_SPIRALSLASH, 0);
			gObjMagicAdd(lpObj, AT_SKILL_DEATH_CANNON, 0);
		}
		else if ( lpObj->Class == CLASS_DARKLORD )	// Dark Lord
		{
			gObjMagicAdd(lpObj, AT_SKILL_SPACE_SPLIT, 0);	// Fire Blast
			gObjMagicAdd(lpObj, AT_SKILL_BRAND_OF_SKILL, 0);	// Sacrifice
		}
		else if ( lpObj->Class == CLASS_RAGEFIGHTER )	// RageFighter
		{
			gObjMagicAdd(lpObj, AT_SKILL_CHARGE, 0);	//

			for(int n = 0; n < MAGIC_SIZE; n++)
			{
				if(lpObj->Magic[n].IsMagic() == 1)
				{
					if (lpObj->Magic[n].m_Skill == AT_SKILL_LARGERINGBLOWER || 
						lpObj->Magic[n].m_Skill == AT_SKILL_UPPERBEAST)
					{
						lpObj->Magic[n].Clear();
					}
				}
			}
		}


		gObjMagicAdd(lpObj,AT_SKILL_STUN,0);
		gObjMagicAdd(lpObj,AT_SKILL_REMOVAL_STUN,0);
		gObjMagicAdd(lpObj,AT_SKILL_ADD_MANA,0);
		gObjMagicAdd(lpObj,AT_SKILL_INVISIBLE,0);
		gObjMagicAdd(lpObj,AT_SKILL_REMOVAL_INVISIBLE,0);
		gObjMagicAdd(lpObj,AT_SKILL_REMOVAL_MAGIC,0);

		//::gObjMagicAddEnergyCheckOnOff(TRUE);
	}

	bool bAllItemExist = true;

	if ( lpMsg->DbVersion == 0 ) 
	{
		::DbItemSetInByte(lpObj, lpMsg, 7, &bAllItemExist);

		for ( int i=ReadConfig.MAIN_INVENTORY_SIZE(lpObj->m_Index,false);i<ReadConfig.INVENTORY_SIZE(lpObj->m_Index,true);i++)
		{
			lpObj->Inventory1[i].Clear();
		}

		memset(lpObj->InventoryMap1+ReadConfig.INVENTORY_SIZE(lpObj->m_Index,true)-PSHOP_SIZE, (BYTE)-1, PSHOP_SIZE);	
	}
	else if(lpMsg->DbVersion < 3)
	{
		::DbItemSetInByte(lpObj, lpMsg, 10, &bAllItemExist);
	}
	else
	{
		::DbItemSetInByte(lpObj, lpMsg, 16, &bAllItemExist);
	}

	/*if ( lpObj->Level <=5 )
	{
		for ( int i=ReadConfig.INVENTORY_SIZE(lpObj->m_Index,false)-PSHOP_SIZE;i<ReadConfig.INVENTORY_SIZE(lpObj->m_Index,false);i++) //IcaruS PShop DEL WITHOUT DEL Extra Inventory FIX
		//for ( int i=ReadConfig.MAIN_INVENTORY_SIZE(lpObj->m_Index,false);i<ReadConfig.INVENTORY_SIZE(lpObj->m_Index,false);i++)
		{
			BYTE NewOption[MAX_EXOPTION_SIZE];

			if ( lpObj->Inventory1[i].IsItem() == TRUE )
			{
				for ( int j=0;j<MAX_EXOPTION_SIZE;j++)
				{
					NewOption[j] = 0;
				}

				if ( ( lpObj->Inventory1[i].m_NewOption & 0x20 ) != 0 )
				{
					NewOption[0] = TRUE;
				}

				if ( ( lpObj->Inventory1[i].m_NewOption & 0x10 ) != 0 )
				{
					NewOption[1] = TRUE;
				}

				if ( ( lpObj->Inventory1[i].m_NewOption & 0x08 ) != 0 )
				{
					NewOption[2] = TRUE;
				}

				if ( ( lpObj->Inventory1[i].m_NewOption & 0x04 ) != 0 )
				{
					NewOption[3] = TRUE;
				}

				if ( ( lpObj->Inventory1[i].m_NewOption & 0x02 ) != 0 )
				{
					NewOption[4] = TRUE;
				}

				if ( ( lpObj->Inventory1[i].m_NewOption & 0x01 ) != 0 )
				{
					NewOption[5] = TRUE;
				}

				LogAddTD("[PShop] PShop Item Delete LV <= 5 [%s][%s] [LV:%d] : serial:%d [%s][%d][%d][%d][%d] Ex:[%d,%d,%d,%d,%d,%d,%d] Set:[%d]",
					lpObj->AccountID, lpObj->Name, lpObj->Level, lpObj->Inventory1[i].m_Number, 
					ItemAttribute[lpObj->Inventory1[i].m_Type].Name, lpObj->Inventory1[i].m_Level,
					lpObj->Inventory1[i].m_SkillOption, lpObj->Inventory1[i].m_LuckOption,
					lpObj->Inventory1[i].m_Z28Option, NewOption[0], NewOption[1], NewOption[2], NewOption[3],
					NewOption[4], NewOption[5], NewOption[6], lpObj->Inventory1[i].m_SetOption);
			}

			lpObj->Inventory1[i].Clear();
		}

		memset(lpObj->InventoryMap1+64, (BYTE)-1, PSHOP_SIZE);
	}*/

	lpObj->Live = TRUE;
	lpObj->Type = OBJ_USER;
	lpObj->TargetNumber = -1;
	lpObj->Connected = PLAYER_PLAYING;
	::gObjMakePreviewCharSet(aIndex);
	CreateFrustrum(lpObj->X, lpObj->Y, aIndex);
	MapC[lpObj->MapNumber].SetStandAttr(lpObj->X, lpObj->Y);
	lpObj->m_OldX = lpObj->X;
	lpObj->m_OldY = lpObj->Y;
	lpMsg->NextExp = lpObj->NextExp;

	if ( lpObj->Life == 0.0f )
	{
		lpObj->Live = TRUE;
		lpObj->m_State = 4;
		lpObj->DieRegen = TRUE;
		lpObj->RegenTime = GetTickCount();
	}
	else
	{
		lpObj->m_State = 1;
	}

	if ( (lpMsg->CtlCode & 8) == 8 )
	{
		lpObj->Authority = 2;
		LogAddC(2, lMsg.Get(MSGGET(1, 245)), lpObj->AccountID, lpObj->Name);
		cManager.ManagerAdd(lpObj->Name, lpObj->m_Index);
	} else {
		lpObj->Authority = 1;
	}

	if ( (lpMsg->CtlCode & 0x20 ) == 0x20 )
	{
		lpObj->Authority = 0x20;
		LogAddC(2, "(%s)(%s) Set Event GM", lpObj->AccountID, lpObj->Name);
	}

	lpObj->Penalty = 0;

	if ( lpObj->m_cAccountItemBlock != 0 )
	{
		lpObj->Penalty |= 8;
	}

	if ( (lpMsg->CtlCode & 2) == 2 )
	{
		lpObj->Penalty |= 4;
		LogAddL("Penalty : Item Don't touch  %s %s", lpObj->AccountID, lpObj->Name);
	}

	#if (GS_CASTLE==1)
	//Castle Siege State Send Fix [3]
	if ( lpObj->MapNumber == MAP_INDEX_CASTLESIEGE )
	{
		if ( g_CastleSiege.GetCastleState() == 7 )
		{
			GCAnsCsNotifyStart(lpObj->m_Index, 1);
			g_CastleSiege.SetUserCsJoinSide(lpObj->m_Index);
			g_CastleSiege.NotifySelfCsJoinSide(lpObj->m_Index);
			g_CastleSiege.NotifyCsSelfLeftTime(lpObj->m_Index);
		} else {
			GCAnsCsNotifyStart(lpObj->m_Index, 0);
		}
	}
	#endif

	if ( lpObj->MapNumber == MAP_INDEX_CRYWOLF_FIRSTZONE )
	{
		g_CrywolfSync.NotifyCrywolfCurrentStateByUserId(lpObj->m_Index);
	}

	::gObjAuthorityCodeSet(lpObj);
	LogAddTD(lMsg.Get(MSGGET(1, 246)), lpObj->m_Index, lpObj->AccountID, lpObj->Name);

	BYTE weather = MapC[lpObj->MapNumber].GetWeather();

	if ( (weather >> 4) > 0 )
	{
		CGWeatherSend(aIndex, weather);
	}

	if ( DragonEvent->GetState() > 0 )
	{
		if ( DragonEvent->GetMapNumber() == lpObj->MapNumber )
		{
			::GCMapEventStateSend(lpObj->MapNumber, 1, 1);
		}
	}

	::gEledoradoEvent.CheckGoldDercon(lpObj->MapNumber);
	::GCCheckMainExeKeySend(aIndex);

	if ( m_ObjBill[aIndex].GetCertify() >= 0 && m_ObjBill[aIndex].GetCertify() < 3 )
	{
		gLCount[m_ObjBill[aIndex].GetCertify()].Add();
	}

	if ( bAllItemExist == false )
	{
		PMSG_NOTICE pNotice;
		pNotice.type = 0;	// 3
		pNotice.btCount = 0;	// 4
		pNotice.wDelay = 0;	// 6	
		pNotice.dwColor = 0;	// 8
		pNotice.btSpeed = 0;	// C

		TNotice::MakeNoticeMsgEx((TNotice *)&pNotice, 1, "없는 아이템 발견 !!!");	// Deathway need Translation
		DataSend(lpObj->m_Index, (UCHAR *)&pNotice, pNotice.h.size);
	}

	if ( lpObj->Level <= g_iCharacterRecuperationMaxLevel )
	{
		lpObj->m_iAutoRecuperationTime = GetTickCount();
	}
	else
	{
		lpObj->m_iAutoRecuperationTime = 0;
	}

	lpObj->dwShieldAutoRefillTimer = GetTickCount();

	LogAddTD("[ShieldSystem][CalcSDPoint] [%s][%s] user SD Gage : %d",
		lpObj->AccountID,lpObj->Name,lpObj->iMaxShield+lpObj->iAddShield);

	g_CashShop.AddUser(lpObj);
	g_CashShop.CGCashPoint(lpObj);
	g_CashItemPeriodSystem.GDReqPeriodItemList(lpObj);

	return TRUE;
}


BOOL gObjCanItemTouch(LPOBJ lpObj, int type )
{
	if ( (lpObj->Penalty &4) == 4 )
	{
		return false;
	}

	if ( (lpObj->Penalty &8) == 8 )
	{
		if ( type == 6 || type == 8 )
		{
			return true;
		}
		return false;
	}
	return true;
}








void gObjMagicTextSave(LPOBJ lpObj)
{
	int total=0;

	for ( int n=0;n<MAGIC_SIZE;n++)
	{
		if ( lpObj->Magic[n].IsMagic() == TRUE )
		{
			LogAddTD(lMsg.Get(MSGGET(1, 247)), lpObj->AccountID, lpObj->Name, n, lpObj->Magic[n].m_Skill);
			total++;
		}
	}
}








void ItemIsBufExOption(BYTE * buf, CItem * lpItem)
{
	for ( int i=0;i<MAX_EXOPTION_SIZE;i++ )
	{
		buf[i] = 0;
	}

	if ( lpItem->IsExtLifeAdd() != FALSE )
	{
		buf[0] = TRUE;
	}

	if ( lpItem->IsExtManaAdd() != FALSE )
	{
		buf[1] = TRUE;
	}

	if ( lpItem->IsExtDamageMinus() != FALSE )
	{
		buf[2] = TRUE;
	}

	if ( lpItem->IsExtDamageReflect() != FALSE )
	{
		buf[3] = TRUE;
	}

	if ( lpItem->IsExtDefenseSuccessfull() != FALSE )
	{
		buf[4] = TRUE;
	}

	if ( lpItem->IsExtMonsterMoney() != FALSE )
	{
		buf[5] = TRUE;
	}

}







void gObjStatTextSave(LPOBJ lpObj)
{
	LogAddTD("[%s][%s] CharInfoSave : Class=%d Level=%d LVPoint=%d Exp=%u Str=%d Dex=%d Vit=%d Energy=%d Leadership:%d Map=%d Pk=%d",
		lpObj->AccountID, lpObj->Name, lpObj->DbClass,lpObj->Level, lpObj->LevelUpPoint, lpObj->Experience, 
		lpObj->Strength, lpObj->Dexterity, lpObj->Vitality, lpObj->Energy, lpObj->Leadership, lpObj->MapNumber,
		lpObj->m_PK_Level);
}







void gObjItemTextSave(LPOBJ lpObj)
{
	BYTE NewOption[MAX_EXOPTION_SIZE];
	int n;

	for ( n=0;n<ReadConfig.INVENTORY_SIZE(lpObj->m_Index,false);n++)
	{
		if ( lpObj->pInventory[n].IsItem() == TRUE )
		{
			if ( lpObj->pInventory[n].m_serial != 0 )
			{
				ItemIsBufExOption(NewOption, &lpObj->pInventory[n]);

				PLAYER_ITEMS_LOG.Output(lMsg.Get(MSGGET(1, 248)), lpObj->AccountID, lpObj->Name, n, lpObj->pInventory[n].GetName(),
					lpObj->pInventory[n].m_Level, lpObj->pInventory[n].m_SkillOption,
					lpObj->pInventory[n].m_LuckOption, lpObj->pInventory[n].m_Z28Option,
					lpObj->pInventory[n].m_Number, (BYTE)lpObj->pInventory[n].m_Durability,
					NewOption[0], NewOption[1], NewOption[2], NewOption[3], NewOption[4], NewOption[5], NewOption[6],
					lpObj->pInventory[n].m_SetOption,lpObj->pInventory[n].m_ItemOptionEx >> 7,g_kJewelOfHarmonySystem.GetItemStrengthenOption(&lpObj->pInventory[n]),g_kJewelOfHarmonySystem.GetItemOptionLevel(&lpObj->pInventory[n]));
			}
		}
	}

	PLAYER_ITEMS_LOG.Output(lMsg.Get(MSGGET(1, 249)), lpObj->AccountID, lpObj->Name, lpObj->Money);
}








void gObjWarehouseTextSave(LPOBJ lpObj)
{
	BYTE NewOption[MAX_EXOPTION_SIZE];
	int n;

	for ( n=0;n<ReadConfig.WAREHOUSE_SIZE(lpObj->m_Index,false);n++)
	{
		if ( lpObj->pWarehouse[n].IsItem() == TRUE )
		{
			if ( lpObj->pWarehouse[n].m_serial != 0 )
			{
				ItemIsBufExOption(NewOption, &lpObj->pWarehouse[n]);

				PLAYER_ITEMS_LOG.Output(lMsg.Get(MSGGET(1, 250)), lpObj->AccountID, lpObj->Name, n, lpObj->pWarehouse[n].GetName(),
					lpObj->pWarehouse[n].m_Level, lpObj->pWarehouse[n].m_SkillOption,
					lpObj->pWarehouse[n].m_LuckOption, lpObj->pWarehouse[n].m_Z28Option,
					lpObj->pWarehouse[n].m_Number, (BYTE)lpObj->pWarehouse[n].m_Durability,
					NewOption[0], NewOption[1], NewOption[2], NewOption[3], NewOption[4], NewOption[5], NewOption[6],
					lpObj->pWarehouse[n].m_SetOption, lpObj->pWarehouse[n].m_ItemOptionEx>>7,
					g_kJewelOfHarmonySystem.GetItemStrengthenOption(&lpObj->pWarehouse[n]),
					g_kJewelOfHarmonySystem.GetItemOptionLevel(&lpObj->pWarehouse[n]));
			}
		}
	}

	PLAYER_ITEMS_LOG.Output(lMsg.Get(MSGGET(1, 251)), lpObj->AccountID, lpObj->Name, lpObj->WarehouseMoney);
}








void gObjAuthorityCodeSet(LPOBJ lpObj)
{
	if ( (lpObj->Authority&1) == 1 )
	{
		return;
	}

	if ( (lpObj->Authority&2)== 2 )
	{
		lpObj->AuthorityCode = -1;
		return;
	}

	if ( (lpObj->Authority&4) == 4 )
	{
		return;
	}

	if ( (lpObj->Authority&8) == 8 )
	{
		lpObj->AuthorityCode |= 1;
		lpObj->AuthorityCode |= 2;
		lpObj->AuthorityCode |= 4;
		lpObj->AuthorityCode |= 8;
		lpObj->AuthorityCode |= 16;
		lpObj->AuthorityCode |= 32;
		lpObj->AuthorityCode |= 40;

		return;
	}

	if ( (lpObj->Authority&16) == 16 )
	{
		return;
	}

	if ( (lpObj->Authority&32) == 32 )
	{
		lpObj->AuthorityCode |= 1;
		lpObj->AuthorityCode |= 2;
		lpObj->AuthorityCode |= 4;
		lpObj->AuthorityCode |= 8;
		lpObj->AuthorityCode |= 16;
		lpObj->AuthorityCode |= 32;
		lpObj->AuthorityCode |= 40;

		return;
	}

	if ( (lpObj->Authority&32) == 40 )
	{
		lpObj->AuthorityCode |= 1;
		lpObj->AuthorityCode |= 2;
		lpObj->AuthorityCode |= 4;
		lpObj->AuthorityCode |= 8;
		lpObj->AuthorityCode |= 16;
		lpObj->AuthorityCode |= 32;
		lpObj->AuthorityCode |= 40;

		return;
	}

}


int gObjMonsterAdd(WORD Type, BYTE Map, BYTE X, BYTE Y)
{
	if ( gMSetBase.m_Count >= OBJ_MAXMONSTER-1 )
	{
		MsgBox("Monster attribute max over %s %d", __FILE__, __LINE__);
		return -1;
	}

	if (g_MapServerManager.CheckMapCanMove(Map) == FALSE )
	{
		LogAddCTD(3,"[MonsterAdd] No need to load monster for map %d [%d,%d,%d]",Map,Type,X,Y);
		return -1;
	}

	gMSetBase.m_Count++;
	gMSetBase.m_Mp[gMSetBase.m_Count].m_Dis			= 30;
	gMSetBase.m_Mp[gMSetBase.m_Count].m_Type		= Type;
	gMSetBase.m_Mp[gMSetBase.m_Count].m_MapNumber	= Map;
	gMSetBase.m_Mp[gMSetBase.m_Count].m_W			= X;
	gMSetBase.m_Mp[gMSetBase.m_Count].m_H			= Y;
	gMSetBase.m_Mp[gMSetBase.m_Count].m_X			= X;
	gMSetBase.m_Mp[gMSetBase.m_Count].m_Y			= Y;
	gMSetBase.m_Mp[gMSetBase.m_Count].m_ArrangeType = 1;
	gMSetBase.m_Mp[gMSetBase.m_Count].m_Dir			= 3;

	return gMSetBase.m_Count;
}


BOOL gObjSetPosMonster(int aIndex, int PosTableNum)
{
	if ( (  (aIndex<0)? FALSE: (aIndex > OBJMAX-1)? FALSE: TRUE ) == FALSE )
	{
		LogAdd("error : %s %d", __FILE__, __LINE__);
		return FALSE;
	}

	
	if ( (  (PosTableNum<0)? FALSE: (PosTableNum > OBJ_MAXMONSTER-1)? FALSE: TRUE ) == FALSE )
	{
		LogAdd("error : %s %d", __FILE__, __LINE__);
		return FALSE;
	}

	LPOBJ lpObj = &gObj[aIndex];
	lpObj->m_PosNum = PosTableNum;
	lpObj->X = gMSetBase.m_Mp[PosTableNum].m_X;
	lpObj->Y = gMSetBase.m_Mp[PosTableNum].m_Y;
	lpObj->MapNumber = gMSetBase.m_Mp[PosTableNum].m_MapNumber;

	lpObj->TX = lpObj->X;
	lpObj->TY = lpObj->Y;
	lpObj->m_OldX = lpObj->X;
	lpObj->m_OldY = lpObj->Y;
	lpObj->Dir = gMSetBase.m_Mp[PosTableNum].m_Dir;
	lpObj->StartX = lpObj->X;
	lpObj->StartY = lpObj->Y;

	if (lpObj->Class == 44 ||  
		lpObj->Class == 53 ||  
		lpObj->Class == 54 ||  
		lpObj->Class == 55 ||  
		lpObj->Class == 56 ||
		lpObj->Class == 492)
	{

	}
	else if (  lpObj->Class >= 78 &&  lpObj->Class <= 83)
	{

	}
#if (PACK_EDITION>=3)
	else if (  lpObj->Class >= 493 &&  lpObj->Class <= 502)
	{

	}
#endif
	else if ( gMSetBase.GetPosition(PosTableNum, lpObj->MapNumber, lpObj->X, lpObj->Y) == FALSE )
	{
		#if (PACK_EDITION>=2)
		if(IMPERIALGUARDIAN_MAP_RANGE(lpObj->MapNumber) == FALSE)
		{
			LogAdd(lMsg.Get(MSGGET(1, 252)), __FILE__, __LINE__);
			LogAdd("error-L2: Pos:%d, class:%d, map:%d, x:%d, y:%d", 
				PosTableNum, lpObj->Class, lpObj->MapNumber, lpObj->X, lpObj->Y);
			return FALSE;
		}
		#endif
	}

	//lpObj->TX = lpObj->X;
	//lpObj->TY = lpObj->Y;
	//lpObj->m_OldX = lpObj->X;
	//lpObj->m_OldY = lpObj->Y;
	//lpObj->Dir = gMSetBase.m_Mp[PosTableNum].m_Dir;
	//lpObj->StartX = lpObj->X;
	//lpObj->StartY = lpObj->Y;

	return true;
}









BOOL gObjSetMonster(int aIndex, int MonsterClass, LPSTR FunctionName)
{
	int Level;
	LPOBJ lpObj;

	if ( OBJMAX_RANGE(aIndex) == FALSE )
	{
		LogAdd("error : %s %d", __FILE__, __LINE__);
		return false;
	}

	lpObj = &gObj[aIndex];
	lpObj->ConnectCheckTime = GetTickCount();
	lpObj->ShopNumber = -1;
	lpObj->TargetNumber = -1;
	lpObj->m_RecallMon = -1;

	switch ( MonsterClass )	// Switch COMPLETE REVIEWED (JPN-GSn-1.00.18)
	{
		case 200:
			gBSGround[0]->m_BallIndex = aIndex;
			break;
		case 251:
			lpObj->ShopNumber = 0;
			break;
		case 255:
			lpObj->ShopNumber = 1;
			break;
		case 254:
			lpObj->ShopNumber = 2;
			break;
		case 250:
			lpObj->ShopNumber = 3;
			break;
		case 248:
		//case 253:
			lpObj->ShopNumber = 4;
			break;
		case 253:
			lpObj->ShopNumber = 5;
			break;
		case 244:
			lpObj->ShopNumber = 6;
			break;
		case 245:
			lpObj->ShopNumber = 7;
			break;
		case 246:
			lpObj->ShopNumber = 8;
			break;
		case 243:
			lpObj->ShopNumber = 9;
			break;
		case 242:
			lpObj->ShopNumber = 10;
			break;
		case 230:
			lpObj->ShopNumber = 11;
			break;
		case 231:
			lpObj->ShopNumber = 12;
			break;
		case 371:
			lpObj->ShopNumber = 13;
			break;
		case 376:
			lpObj->ShopNumber = 14;
			break;
		case 377:
			lpObj->ShopNumber = 15;
			break;
		case 379:
			lpObj->ShopNumber = 16;
			break;
		case 414:
			lpObj->ShopNumber = 17;
			break;
		case 415:
			lpObj->ShopNumber = 18;
			break;
		case 416:
			lpObj->ShopNumber = 19;
			break;
		case 417:
			lpObj->ShopNumber = 20;
			break;
		case 492:
			lpObj->ShopNumber = 21;
			break;
		case 239:
			lpObj->ShopNumber = 22;
			break;
		case 259:
			lpObj->ShopNumber = 23;
			break;
		case 545:
			lpObj->ShopNumber = 24;
			break;
		case 546:
			lpObj->ShopNumber = 25;
			break;
		case 547:
			lpObj->ShopNumber = 26;
			break;
		case 577:
			lpObj->ShopNumber = 27;
			break;
		case 578:
			lpObj->ShopNumber = 28;
			break;
		case 240:
			lpObj->ShopNumber = 100;
			break;
		case 238:
			lpObj->ShopNumber = 101;
			break;
		case 236:
			lpObj->ShopNumber = 102;
			break;
		case 368:
		case 369:
		case 370:
			lpObj->ShopNumber = 101;
			break;
		case 452:
		case 453:
		case 579:
			lpObj->ShopNumber = 110;
			break;
		case 478:
			lpObj->ShopNumber = 114;
			break;
		case 450:
			lpObj->ShopNumber = 111;
			break;
	}

	lpObj->Connected = PLAYER_PLAYING;
	lpObj->Live = FALSE;
	lpObj->m_State = 0;

	if((MonsterClass == 44 || 
		MonsterClass == 54 || 
		MonsterClass == 53 || 
		MonsterClass == 55 || 
		MonsterClass == 56 || 
		MonsterClass == 76 || 
		MonsterClass == 78 || 
		MonsterClass == 79 || 
		MonsterClass == 80 || 
		MonsterClass == 81 ||
		MonsterClass == 82 || 
		MonsterClass == 83 	
#if (PACK_EDITION>=3)
		|| (MonsterClass >= 493 && MonsterClass <= 502)) && lpObj->IsBot == 0
#else
		)
#endif
		)
	{

	}
	else
	{
		lpObj->Live = TRUE;
		lpObj->m_State = 1;
	}

	if (   (MonsterClass >= 204 && MonsterClass <= 259)
		|| (MonsterClass >= 367 && MonsterClass <= 371)
		|| (MonsterClass >= 375 && MonsterClass <= 377)
		|| (MonsterClass >= 379 && MonsterClass <= 385)
		|| MonsterClass == 406
		|| (MonsterClass >= 414 && MonsterClass <= 417)
		|| (MonsterClass >= 450 && MonsterClass <= 453)
		|| (MonsterClass >= 468 && MonsterClass <= 475)
		|| MonsterClass == 465
		|| MonsterClass == 478
		|| MonsterClass == 479
		|| MonsterClass == 492
		|| MonsterClass == 522
		//|| (MonsterClass >= 524 && MonsterClass <= 528)
		|| (MonsterClass >= 540 && MonsterClass <= 542)
		|| (MonsterClass >= 543 && MonsterClass <= 548)
		|| (MonsterClass >= 566 && MonsterClass <= 568)
		|| (MonsterClass >= 577 && MonsterClass <= 579)
		)
	{
		lpObj->Type = OBJ_NPC;
	}
	else
	{
		lpObj->Type = OBJ_MONSTER;
	}

	if ( MonsterClass == 77 )
	{
		int iSL = gObjAddMonster(MAP_INDEX_ICARUS);

		if ( iSL >= 0 )
		{
			if ( gObjSetMonster(iSL, 76, "gObjSetMonster") == FALSE )
			{
				LogAddC(2,"[MonsterAdd][Icarus][ERROR] Could not find Phoenix Boss Shield");
				return false;
			}

			lpObj->m_SkyBossMonSheildLinkIndex = iSL;
			LogAddTD("[MonsterAdd][Icarus] Phoenix Boss Monster Setting!!");
		}
		else
		{
			LogAddC(2,"[MonsterAdd][Icarus][ERROR] Could not find Phoenix Boss Shield");
			return false;
		}
	}

	if ( MonsterClass == 76 )
	{
		LogAddTD("[MonsterAdd][Icarus] Phoenix Boss Shield Setting!");
	}

	lpObj->Class = MonsterClass;
	Level = lpObj->Level;
	LPMONSTER_ATTRIBUTE lpm = gMAttr.GetAttr(lpObj->Class);

	if ( lpm == NULL )
	{
		LogAddC(2,"[MonsterAdd][ERROR][%s] Could not add: %d,%d",FunctionName,MonsterClass,lpObj->MapNumber);
		//MsgBox(lMsg.Get(MSGGET(1, 253) ), lpObj->Class, __FILE__, __LINE__);
		return false;
	}

	strncpy(gObj[aIndex].Name, lpm->m_Name , 10 );

	if (lpm->m_Level == 0)
	{
		LogAdd(lMsg.Get( MSGGET(1, 254)), lpObj->Class, __FILE__, __LINE__);
	}

	lpObj->Level = lpm->m_Level;
	lpObj->m_AttackSpeed = lpm->m_AttackSpeed;
	lpObj->m_AttackDamageMin = lpm->m_DamageMin;
	lpObj->m_AttackDamageMax = lpm->m_DamageMax;
	lpObj->m_Defense = lpm->m_Defense;
	lpObj->m_MagicDefense = lpm->m_MagicDefense;
	lpObj->m_AttackRating = lpm->m_AttackRating;
	lpObj->m_SuccessfulBlocking = lpm->m_Successfulblocking;
	lpObj->Life = lpm->m_Hp;
	lpObj->MaxLife = lpm->m_Hp;
	lpObj->Mana = lpm->m_Mp;
	lpObj->MaxMana = lpm->m_Mp;
	lpObj->m_MoveRange = lpm->m_MoveRange;
	lpObj->m_AttackSpeed = lpm->m_AttackSpeed;
	lpObj->m_MoveSpeed = lpm->m_MoveSpeed;
	lpObj->MaxRegenTime = lpm->m_RegenTime * 1000;
	lpObj->m_AttackRange = lpm->m_AttackRange;
	lpObj->m_ViewRange = lpm->m_ViewRange;
	lpObj->m_Attribute = lpm->m_Attribute;
	lpObj->m_AttackType = lpm->m_AttackType;
	lpObj->m_ItemRate = lpm->m_ItemRate;
	lpObj->m_MoneyRate = lpm->m_MoneyRate;
	lpObj->Dexterity = 0;

	lpObj->m_iScriptMaxLife = lpm->m_iScriptHP;
	lpObj->m_iBasicAI = lpm->m_iAINumber;
	lpObj->m_iCurrentAI = lpm->m_iAINumber;
	lpObj->m_iCurrentAIState = 0;
	lpObj->m_iLastAIRunTime = 0;
	lpObj->m_iGroupNumber = 0;
	//lpObj->m_iSubGroupNumber = 0;
	lpObj->m_iGroupMemberGuid = -1;
	lpObj->m_iRegenType = 0;

	lpObj->m_Agro.ResetAll();

	lpObj->m_iLastAutomataRuntime = 0;
	lpObj->m_iLastAutomataDelay = 0;

	//lpObj->pInventoryCount[0] = 0;
	memcpy(lpObj->m_Resistance, lpm->m_Resistance, MAX_RESISTENCE_TYPE );
	gObjSetInventory1Pointer(lpObj);

	if ( lpObj->m_AttackType != 0 )
	{
		if ( lpObj->m_AttackType >= 100 
#if (PACK_EDITION>=3)
			&& lpObj->IsBot == 0
#endif
			)
		{
			//DaRKav Monster Magic Dix for summoner DG mob
			//gObjMonsterMagicAdd(lpObj, lpObj->m_AttackType - 100, 1);
			gObjMonsterMagicAdd(lpObj, lpObj->m_AttackType, 1);
		}
		else
		{
			gObjMonsterMagicAdd(lpObj, lpObj->m_AttackType, 1);
		}

		if ( lpObj->Class == 144 || lpObj->Class == 174 || lpObj->Class == 182 || lpObj->Class == 190 || lpObj->Class == 260 || lpObj->Class == 268 )
		{
			gObjMonsterMagicAdd(lpObj, 17, 1);
		}

		if ( lpObj->Class == 145 
			|| lpObj->Class == 175 
			|| lpObj->Class == 183 
			|| lpObj->Class == 191 
			|| lpObj->Class == 261 
			|| lpObj->Class == 269 
			|| lpObj->Class == 146 
			|| lpObj->Class == 176 
			|| lpObj->Class == 184 
			|| lpObj->Class == 192 
			|| lpObj->Class == 262 
			|| lpObj->Class == 270 
			|| lpObj->Class == 147 
			|| lpObj->Class == 177 
			|| lpObj->Class == 185 
			|| lpObj->Class == 193 
			|| lpObj->Class == 263 
			|| lpObj->Class == 271 
			|| lpObj->Class == 148
			|| lpObj->Class == 178
			|| lpObj->Class == 186
			|| lpObj->Class == 194
			|| lpObj->Class == 264
			|| lpObj->Class == 272
			|| lpObj->Class == 160
			|| lpObj->Class == 180
			|| lpObj->Class == 188
			|| lpObj->Class == 196
			|| lpObj->Class == 266
			|| lpObj->Class == 274
			|| lpObj->Class == 332
			|| lpObj->Class == 333
			|| lpObj->Class == 334
			|| lpObj->Class == 335
			|| lpObj->Class == 337 )
		{
			gObjMonsterMagicAdd(lpObj, 17, 1);
		}

		if (lpObj->Class == 161 || 
			lpObj->Class == 181 || 
			lpObj->Class == 189 || 
			lpObj->Class == 197 || 
			lpObj->Class == 267 || 
			lpObj->Class == 275 || 
			lpObj->Class == 338 )
		{
			gObjMonsterMagicAdd(lpObj, 200, 1);
			gObjMonsterMagicAdd(lpObj, 17, 1);
			gObjMonsterMagicAdd(lpObj, 1, 1);
			gObjMonsterMagicAdd(lpObj, 201, 1);
			gObjMonsterMagicAdd(lpObj, 202, 1);
			gObjMonsterMagicAdd(lpObj, 55, 1);
		}

		if (  lpObj->Class == 149 || lpObj->Class == 179 || lpObj->Class == 187 || lpObj->Class == 195 || lpObj->Class == 265 || lpObj->Class == 273 || lpObj->Class == 335 )
		{
			gObjMonsterMagicAdd(lpObj, 17, 1);
			gObjMonsterMagicAdd(lpObj, 1, 1);
		}

		if ( lpObj->Class == 66 || lpObj->Class == 73 || lpObj->Class == 77 || lpObj->Class == 89 || lpObj->Class == 95 || lpObj->Class == 112 || lpObj->Class == 118 || lpObj->Class == 124 || lpObj->Class == 130 || lpObj->Class == 143 || lpObj->Class == 163 || lpObj->Class == 165 || lpObj->Class == 167 || lpObj->Class == 169 || lpObj->Class == 171 || lpObj->Class == 173  || lpObj->Class == 433  || lpObj->Class == 427)
		{
			gObjMonsterMagicAdd(lpObj, 17, 1);
		}
		
		if( lpObj->Class == 89 || lpObj->Class == 95 || lpObj->Class == 112 || lpObj->Class == 118 || lpObj->Class == 124 || lpObj->Class == 130 || lpObj->Class == 143  || lpObj->Class == 433)
		{
			gObjMonsterMagicAdd(lpObj, 3, 1);
		}
	}

	gObjGiveItemSearch(lpObj, lpm->m_MaxItemLevel);
	gObjGiveItemWarehouseSearch(lpObj, lpm->m_MaxItemLevel);
	CreateFrustrum(lpObj->X, lpObj->Y, aIndex);
	MapC[lpObj->MapNumber].SetStandAttr(lpObj->X, lpObj->Y);
	lpObj->m_OldX = lpObj->X;
	lpObj->m_OldY = lpObj->Y;
	lpObj->LastAttackerID = -1;
	return true;
}











void gObjDestroy(unsigned int aSocket,int client)
{
	if ( client < 0 || client > OBJMAX-1)
	{
		return;
	}

	gObj[client].Connected = PLAYER_EMPTY;
}







short gObjAddSearch(SOCKET aSocket, char* ip)
{
	int count;
	int totalcount = 0;

	if ( gDisconnect == 1 )
	{
		return -1;
	}

	if ( gObjTotalUser > gServerMaxUser )
	{
		if (acceptIP.IsIp(ip) == 0 )
		{
			GCJoinSocketResult(4, aSocket);
			return -1;
		}
	}

	count = gObjCount;
	
	while ( true )
	{
		if ( gObj[count].Connected == PLAYER_EMPTY )
		{
			return count;
		}

		count++;

		if ( count >= OBJMAX )
		{
			count = OBJ_STARTUSERINDEX;
		}
		totalcount++;
		if ( totalcount >= OBJMAXUSER )
		{
			break;
		}
	}
	return -1;
}







short gObjAdd(SOCKET aSocket, char* ip, int aIndex)
{
	if ( gObj[aIndex].Connected != PLAYER_EMPTY )
	{
		return -1;
	}

	gObjCharZeroSet(aIndex);
	gNSerialCheck[aIndex].init();
	gObj[aIndex].LoginMsgSnd = FALSE;
	gObj[aIndex].LoginMsgCount = 0;
	memset(gObj[aIndex].AccountID, 0, sizeof(gObj[0].AccountID)-1);
	gObj[aIndex].m_Index = aIndex;
	gObj[aIndex].m_socket = aSocket;
	gObj[aIndex].ConnectCheckTime = GetTickCount();
	gObj[aIndex].AutoSaveTime = gObj[aIndex].ConnectCheckTime;
	gObj[aIndex].Connected = PLAYER_CONNECTED;
	gObj[aIndex].CheckSpeedHack = false;
	gObj[aIndex].LoginMsgCount = 0;
	gObj[aIndex].Magumsa = 0;
	gObj[aIndex].AllowSummoner = 0;
	gObj[aIndex].AllowRageFighter = 0;
	gObj[aIndex].Type = OBJ_USER;
	//gObj[aIndex].SaveTimeForStatics = GetTickCount() + 3600000;
	m_ObjBill[aIndex].Init();
	strcpy(gObj[aIndex].Ip_addr, ip);
	LogAddTD("connect : [%d][%s]", aIndex, ip);
	gObjCount++;
	
	if ( gObjCount >= OBJMAX )
	{
		gObjCount = OBJ_STARTUSERINDEX;
	}

	return aIndex;
}







short gObjMonsterRecall(int iMapNumber)
{
	int number = gObjAddMonster(iMapNumber);

	if ( number < 0 )
	{
		return -1;
	}
	return -1;
}







short gObjAddMonster(int iMapNumber)
{
	if (g_MapServerManager.CheckMapCanMove(iMapNumber) == FALSE )
	{
		LogAddTD("[MapServerManager] Can not add monster to Map: %d", iMapNumber);
		return -1;
	}

	int count;
	int totalcount=0;

	count = gObjMonCount;

	while ( true )
	{
		if ( gObj[count].Connected == PLAYER_EMPTY )
		{
			gObjCharZeroSet(count);
			gObj[count].m_Index = count;
			gObj[count].Connected = PLAYER_CONNECTED;
			gObjMonCount++;

			if ( gObjMonCount >= OBJ_MAXMONSTER )
			{
				gObjMonCount = 0;
			}
			
			return count;
		}

		count ++;
		
		if ( count >= OBJ_MAXMONSTER )
		{
			count = 0;
		}

		totalcount++;

		if ( totalcount >= OBJ_MAXMONSTER )
		{
			LogAdd( lMsg.Get( MSGGET ( 1, 255 )), __FILE__, __LINE__ );
			return -1;
		}
	}
	return -1;
}

short gObjAddCallMon()//IcaruS CALL MON FIX
{

	for(int i=OBJ_MAXMONSTER;i<OBJ_STARTUSERINDEX;i++)
	{
		if ( gObj[i].Connected == PLAYER_EMPTY )
		{
			gObjCharZeroSet(i);
			gObj[i].m_Index = i;
			gObj[i].Connected = PLAYER_CONNECTED;
			gObjCallMonCount++;
			gObj[i].m_PosNum = i;

			if ( gObjCallMonCount >= OBJ_STARTUSERINDEX )
			{
				gObjCallMonCount = OBJ_MAXMONSTER;

			}
			return i;
		}
	}
	
	LogAdd(lMsg.Get(MSGGET(1, 255)), __FILE__, __LINE__);
	return -1;

	//int count;
	//int totalcount = 0;

	//count = gObjCallMonCount;
	//while ( true )
	//{
	//	if ( gObj[count].Connected == PLAYER_EMPTY )
	//	{
	//		gObjCharZeroSet(count);
	//		gObj[count].m_Index = count;
	//		gObj[count].Connected = PLAYER_CONNECTED;
	//		gObjCallMonCount++;

	//		if ( gObjCallMonCount >= OBJ_STARTUSERINDEX )
	//		{
	//			gObjCallMonCount = OBJ_MAXMONSTER;

	//		}
	//		return count;
	//	}

	//	count++;

	//	if ( count >= OBJ_STARTUSERINDEX )
	//	{
	//		count = OBJ_MAXMONSTER;
	//	}

	//	totalcount++;

	//	if ( totalcount >= OBJ_CALLMONCOUNT )
	//	{
	//		LogAdd(lMsg.Get(MSGGET(1, 255)), __FILE__, __LINE__);
	//		return -1;
	//	}
	//}

	//return -1;

}


void gObjUserKill(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	if ( lpObj->CloseCount < 1 )
	{
		lpObj->CloseCount = 6;
		lpObj->CloseType = 0;
	}
}






void gObjAllLogOut()
{
	static BOOL bAllLogOut = FALSE;

	if ( bAllLogOut == 0 )
	{
		bAllLogOut = 1;
		gObjAllDisconnect();
		Sleep(1000);
		LogAddC(2, lMsg.Get( MSGGET(2, 0)));
	}
}






void gObjAllDisconnect()
{
	gDisconnect = TRUE;

	for ( int n=OBJ_STARTUSERINDEX; n< OBJMAX; n++)
	{
		if ( gObj[n].Connected > PLAYER_EMPTY )
		{
			if ( gObj[n].Type == OBJ_USER )
			{
				CloseClient(n);
			}
		}
	}
}






void gObjTradeSave(LPOBJ lpObj, int index)
{
	if ( lpObj->m_IfState.use != 0 && lpObj->m_IfState.type == 1 )
	{
		int tObjNum = lpObj->TargetNumber;

		if ( tObjNum > 0 )
		{
			char szTemp[256];

			CGTradeResult(tObjNum, 0);
			wsprintf(szTemp, lMsg.Get(MSGGET(4, 77)), lpObj->Name);
			GCServerMsgStringSend((char*)szTemp, tObjNum, 1);
			gObjTradeCancel(tObjNum);
			if(ReadConfig.TradeLog == TRUE)
				TRADE_LOG.Output(lMsg.Get(MSGGET(2, 1)), gObj[tObjNum].AccountID, gObj[tObjNum].Name);
			//LogAdd(lMsg.Get(MSGGET(2, 1)), gObj[tObjNum].AccountID, gObj[tObjNum].Name);
		}

		gObjTradeCancel( index );
		if(ReadConfig.TradeLog == TRUE)
			TRADE_LOG.Output(lMsg.Get(MSGGET(2,2)), lpObj->AccountID, lpObj->Name);
		//LogAdd(lMsg.Get(MSGGET(2,2)), lpObj->AccountID, lpObj->Name);
	}
}






void gObjBillRequest(LPOBJ lpObj)
{
	char szMsg[128];

	if ( m_ObjBill[lpObj->m_Index].GetPayCode() == 0 )
	{
		if ( m_ObjBill[lpObj->m_Index].GetCertify() == 0 )
		{
			wsprintf(szMsg, lMsg.Get(MSGGET(4, 85)), m_ObjBill[lpObj->m_Index].GetEndTime());
		}
		else if ( m_ObjBill[lpObj->m_Index].GetCertify() == 1 )
		{
			if ( m_ObjBill[lpObj->m_Index].GetEndTime() != 0 )
			{
				wsprintf(szMsg, lMsg.Get(MSGGET(4, 86)), m_ObjBill[lpObj->m_Index].GetEndTime());
			}
		}
	}
	else if ( m_ObjBill[lpObj->m_Index].GetPayCode() == 1 )
	{
		char szTemp[20];
		szTemp[4] = 0;
		strncpy(szTemp, m_ObjBill[lpObj->m_Index].GetEndsDays(), 4);
		int Day = atoi(szTemp);

		strncpy(szTemp, m_ObjBill[lpObj->m_Index].GetEndsDays()+4, 2);
		szTemp[2] = 0;
		int Month = atoi(szTemp);

		strncpy(szTemp, m_ObjBill[lpObj->m_Index].GetEndsDays()+6, 2);
		szTemp[2] = 0;
		int Year = atoi(szTemp);

		if ( m_ObjBill[lpObj->m_Index].GetCertify() == 0 )	// Account Based
		{
			wsprintf(szMsg, lMsg.Get(MSGGET(4, 87)), Day, Month, Year);
		}
		else if ( m_ObjBill[lpObj->m_Index].GetCertify() == 1 )	// IP Based
		{
			wsprintf(szMsg, lMsg.Get(MSGGET(4, 88)), Day, Month, Year);
		}
	}
	else if ( m_ObjBill[lpObj->m_Index].GetPayCode() == 3)
	{
		char szYear[5] = "";
		char szMonth[3] = "";
		char szDay[3] = "";
		char szHour[3] = "";
		char szMin[3] = "";

		strncpy(szYear, m_ObjBill[lpObj->m_Index].GetEndsDays(), 4);
		strncpy(szMonth, m_ObjBill[lpObj->m_Index].GetEndsDays()+4, 2);
		strncpy(szDay, m_ObjBill[lpObj->m_Index].GetEndsDays()+6, 2);
		strncpy(szHour, m_ObjBill[lpObj->m_Index].GetEndsDays()+8, 2);
		strncpy(szMin, m_ObjBill[lpObj->m_Index].GetEndsDays()+10, 2);

		wsprintf(szMsg, lMsg.Get(MSGGET(5, 220)), m_ObjBill[lpObj->m_Index].GetEndTime(), szYear, szMonth,
			szDay, szHour, szMin);

		LogAddTD("[%s][%s] BillType : (Time) RemainPoint : (%d)",
			lpObj->AccountID, lpObj->Name, m_ObjBill[lpObj->m_Index].GetEndTime());
	}
	else if ( m_ObjBill[lpObj->m_Index].GetPayCode() == 4)
	{
		char szYear[5] = "";
		char szMonth[3] = "";
		char szDay[3] = "";

		strncpy(szYear, m_ObjBill[lpObj->m_Index].GetEndsDays(), 4);
		strncpy(szMonth, m_ObjBill[lpObj->m_Index].GetEndsDays()+4, 2);
		strncpy(szDay, m_ObjBill[lpObj->m_Index].GetEndsDays()+6, 2);

		wsprintf(szMsg, lMsg.Get(MSGGET(5, 221)),  szYear, szMonth,	szDay);

		LogAddTD("[%s][%s] BillType : (Date) RemainDate : (%s-%s-%s)",
			lpObj->AccountID, lpObj->Name, szYear, szMonth, szDay);
	}
	else if ( m_ObjBill[lpObj->m_Index].GetPayCode() == 5)		// FREE
	{
		wsprintf(szMsg, lMsg.Get(MSGGET(5, 222)));
		LogAddTD("[%s][%s] BillType : (NoCharge)", lpObj->AccountID, lpObj->Name);
	}
	else	// Pospaid Account
	{
		wsprintf(szMsg, lMsg.Get(MSGGET(4, 89)));
	}


	LogAdd(szMsg);
	GCServerMsgStringSend(szMsg, lpObj->m_Index, 1);
}






short gObjMemFree(int index)
{
	LPOBJ lpObj;

	if ( gDisconnect == TRUE )
	{
		return -1;
	}

	if ( index < 0 || index > OBJMAX )
	{
		LogAdd("(%s)(%d) = index over error (%d)", __FILE__, __LINE__, index);
		return false;
	}

	lpObj = &gObj[index];

	if ( lpObj->Connected < PLAYER_CONNECTED )
	{
		return false;
	}

	gObjTradeSave(lpObj, index);

	if ( lpObj->Connected >= PLAYER_LOGGED )
	{
		if ( lpObj->Type == OBJ_USER )
		{
			if ( lpObj->Connected == PLAYER_PLAYING )
			{
				if (  BC_MAP_RANGE(lpObj->MapNumber) )
				{
					g_BloodCastle.SearchUserDropQuestItem(lpObj->m_Index);
				}

				GJSetCharacterInfo(lpObj, index, 0);
				gObjItemTextSave(lpObj);
				gObjStatTextSave(lpObj);
				gObjSavePetItemInfo(lpObj->m_Index, 0);
			}
			
			GJPUserClose(lpObj);
		}
	}

	gObjViewportClose(lpObj);
	lpObj->Connected = PLAYER_EMPTY;
	return 1;
}


void gObjUserDieSave(LPOBJ lpObj)
{
	if(lpObj->MapNumber == MAP_INDEX_LORENCIA || lpObj->MapNumber == MAP_INDEX_DUNGEON)
	{
		lpObj->MapNumber = MAP_INDEX_LORENCIA;
		MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber,(short &)lpObj->X,(short &)lpObj->Y);
	}
	else if(IT_MAP_RANGE(lpObj->MapNumber))
	{
		IllusionTemple.PlayerDieRespawn(lpObj->m_Index,(short &)lpObj->X,(short &)lpObj->Y);
	}
	else if(DS_MAP_RANGE(lpObj->MapNumber))
	{
		g_DevilSquare.DieProcDevilSquare(lpObj);
		lpObj->MapNumber = MAP_INDEX_NORIA;
		MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber,(short &)lpObj->X,(short &)lpObj->Y);
	}
	else if(lpObj->MapNumber == MAP_INDEX_ICARUS)
	{
		lpObj->MapNumber = MAP_INDEX_DEVIAS;
		MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber,(short &)lpObj->X,(short &)lpObj->Y);
	}
	else if(BC_MAP_RANGE(lpObj->MapNumber))
	{
		lpObj->MapNumber = MAP_INDEX_DEVIAS;
		MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber,(short &)lpObj->X,(short &)lpObj->Y);
	}
	else if(CC_MAP_RANGE(lpObj->MapNumber))
	{
		lpObj->MapNumber = MAP_INDEX_DEVIAS;
		MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber,(short &)lpObj->X,(short &)lpObj->Y);
	}
	else if(KALIMA_MAP_RANGE(lpObj->MapNumber))
	{
		lpObj->MapNumber = MAP_INDEX_DEVIAS;
		MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber,(short &)lpObj->X,(short &)lpObj->Y);
	}
	else if(lpObj->MapNumber == MAP_INDEX_AIDA)
	{
		MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber,(short &)lpObj->X,(short &)lpObj->Y);
	}

	#if (GS_CASTLE==1)
	else if(lpObj->MapNumber == MAP_INDEX_SWAMP)
	{
		MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber,(short &)lpObj->X,(short &)lpObj->Y);
	}

	else if(lpObj->MapNumber == MAP_INDEX_CRYWOLF_FIRSTZONE)
	{
		if ( g_Crywolf.GetOccupationState() == CRYWOLF_OCCUPATION_SECURE ||
			(g_Crywolf.GetCrywolfState() >= CRYWOLF_STATE_NOTIFY_1 && 
			 g_Crywolf.GetCrywolfState() < CRYWOLF_STATE_START))
		{
			MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber,(short &)lpObj->X,(short &)lpObj->Y);
		} else {
			int mgt = 118;
			short x,y,level;
			BYTE map = lpObj->MapNumber,dir;
			int result = gGateC.GetGate(mgt,(short &)x,(short &)y,(BYTE &)map,(BYTE &)dir,(short &)level);
			if(result >= 0)
			{
				lpObj->MapNumber = map;
				lpObj->X = x;
				lpObj->Y = y;
			}
		}
	}
	#endif

	#if (GS_CASTLE==0)
	else if(lpObj->MapNumber == MAP_INDEX_KANTURU_BOSS)
	{
		int mgt = 137;
		short x,y,level;
		BYTE map = lpObj->MapNumber,dir;
		int result = gGateC.GetGate(mgt,(short &)x,(short &)y,(BYTE &)map,(BYTE &)dir,(short &)level);
		if(result >= 0)
		{
			lpObj->MapNumber = map;
			lpObj->X = x;
			lpObj->Y = y;
		}

		g_KanturuBattleUserMng.DeleteUserData(lpObj->m_Index);
	}
	#endif
	else if(lpObj->MapNumber == MAP_INDEX_KANTURU1)
	{
		int mgt = 138;
		short x,y,level;
		BYTE map = lpObj->MapNumber,dir;

		int result = gGateC.GetGate(mgt,(short &)x,(short &)y,(BYTE &)map,(BYTE &)dir,(short &)level);
		if(result >= 0)
		{
			lpObj->MapNumber = map;
			lpObj->X = x;
			lpObj->Y = y;
		}
	}
	else if(lpObj->MapNumber == MAP_INDEX_KANTURU2)
	{
		int mgt = 139;
		short x,y,level;
		BYTE map = lpObj->MapNumber,dir;

		int result = gGateC.GetGate(mgt,(short &)x,(short &)y,(BYTE &)map,(BYTE &)dir,(short &)level);
		if(result >= 0)
		{
			lpObj->MapNumber = map;
			lpObj->X = x;
			lpObj->Y = y;
		}
	}
	else if(lpObj->MapNumber == MAP_INDEX_REFUGE)
	{
		lpObj->MapNumber = MAP_INDEX_BARRACKS;
		MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber,(short &)lpObj->X,(short &)lpObj->Y);
	}
	else if(lpObj->MapNumber == MAP_INDEX_RAKLIONBOSS)
	{
		lpObj->MapNumber = MAP_INDEX_RAKLION;
		MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber,(short &)lpObj->X,(short &)lpObj->Y);
	}
	else if(lpObj->MapNumber == MAP_INDEX_DUELMAP)
	{
		lpObj->MapNumber = MAP_INDEX_VULCANUS;
		MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber,(short &)lpObj->X,(short &)lpObj->Y);
	}
	else if (lpObj->MapNumber == MAP_INDEX_CASTLESIEGE)
	{
		//Check if Siege running and lifestone exists
		#if (GS_CASTLE==1)
		if ( !g_CsNPC_LifeStone.SetReSpawnUserXY(lpObj->m_Index) )
		{
			int mgt = 100;
			short x,y,level;
			BYTE map = lpObj->MapNumber,dir;

			if (lpObj->m_btCsJoinSide == 1)
				mgt = 105;

			int result = gGateC.GetGate(mgt,(short &)x,(short &)y,(BYTE &)map,(BYTE &)dir,(short &)level);
			if(result >= 0)
			{
				lpObj->MapNumber = map;
				lpObj->X = x;
				lpObj->Y = y;
			}
		}
		#else
			MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber,(short &)lpObj->X,(short &)lpObj->Y);
		#endif
	}
	else if (lpObj->MapNumber == MAP_INDEX_CASTLEHUNTZONE)
	{
		int mgt = 106;
		short x,y,level;
		BYTE map = lpObj->MapNumber,dir;

		int result = gGateC.GetGate(mgt,(short &)x,(short &)y,(BYTE &)map,(BYTE &)dir,(short &)level);
		if(result >= 0)
		{
			lpObj->MapNumber = map;
			lpObj->X = x;
			lpObj->Y = y;
		}						
	}
	else if (lpObj->MapNumber == MAP_INDEX_KARLUTAN1)
	{
		int mgt = 335;
		short x,y,level;
		BYTE map = lpObj->MapNumber,dir;

		int result = gGateC.GetGate(mgt,(short &)x,(short &)y,(BYTE &)map,(BYTE &)dir,(short &)level);
		if(result >= 0)
		{
			lpObj->MapNumber = map;
			lpObj->X = x;
			lpObj->Y = y;
		}						
	}
	else if (lpObj->MapNumber == MAP_INDEX_KARLUTAN2)
	{
		int mgt = 344;
		short x,y,level;
		BYTE map = lpObj->MapNumber,dir;

		int result = gGateC.GetGate(mgt,(short &)x,(short &)y,(BYTE &)map,(BYTE &)dir,(short &)level);
		if(result >= 0)
		{
			lpObj->MapNumber = map;
			lpObj->X = x;
			lpObj->Y = y;
		}						
	}
	else
	{
		MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber,(short &)lpObj->X,(short &)lpObj->Y);
	}
	lpObj->Life = lpObj->MaxLife + lpObj->AddLife;
	lpObj->Mana = lpObj->MaxMana + lpObj->AddMana;
}

BOOL gObjGameClose(int aIndex)
{
	if ( aIndex < 0 || aIndex > OBJMAX )	// Deathway fix
	{
		LogAdd("(%s)(%d) = index over error (%d)", __FILE__, __LINE__, aIndex);

		return FALSE;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if ( lpObj->Connected != PLAYER_PLAYING )
	{
		return FALSE;
	}

	if ( lpObj->Type != OBJ_USER )
	{
		LogAddTD(lMsg.Get(MSGGET(2, 3)), lpObj->AccountID, lpObj->Name);
		return FALSE;
	}

	if ( lpObj->m_RecallMon >= 0 )
	{
		gObjMonsterCallKill(lpObj->m_Index);
	}

	gObjMagicTextSave(lpObj);

	if ( (   (lpObj->m_IfState.use != 0 )
		&&  (lpObj->m_IfState.type == 7 )) 
		|| ((lpObj->m_IfState.use != 0)  
		&& (lpObj->m_IfState.type == 13)))
	{
		if ( lpObj->bIsChaosMixCompleted == true )//|| lpObj->m_bIsCastleNPCUpgradeCompleted == true)
		{
			gObjInventoryCommit(aIndex);
		}
		else
		{
			gObjInventoryRollback(aIndex);
		}
	}

	gObjTradeSave(lpObj, aIndex);

	if ( (lpObj->m_IfState.use != 0 )
		&&  (lpObj->m_IfState.type == 10 ) 
		&& lpObj->TargetShopNumber == 229)
	{
		gQuestNpcTeleport.TalkRefDel();
	}

	if ( (lpObj->m_IfState.use != 0 )
		&&  (lpObj->m_IfState.type == 10 ) 
		&& lpObj->TargetShopNumber == 568)
	{
		gQuestNpcTeleport.TalkRefDel();
	}

	if(lpObj->DieRegen == 1 )
	{
		gObjUserDieSave(lpObj);
	}

	if ( (lpObj->Authority&2) == 2 )
	{
		cManager.ManagerDel(lpObj->Name);
	}

	if ( OBJMAX_RANGE(lpObj->m_iDuelUserRequested) )
	{
		g_DuelManager.gObjSendDuelEnd(&gObj[lpObj->m_iDuelUserRequested]);
	}


	if ( g_DuelManager.gObjDuelCheck(lpObj) != FALSE )
	{
		g_DuelManager.gObjSendDuelEnd(&gObj[lpObj->m_iDuelUser]);
	}


	g_DuelManager.gObjResetDuel(lpObj);

	if(lpObj->m_isDuelSpectator == true)
		GCSkillInfoSend(lpObj, 0, eVS_DUEL_INTERFACE);

	if ( lpObj->PartyNumber >= 0 )
	{
		int iPartyIndex= gParty.GetIndex(lpObj->PartyNumber, lpObj->m_Index, lpObj->DBNumber);

		if ( iPartyIndex >= 0 )
		{
			PMSG_PARTYDELUSER pMsg;

			pMsg.Number = iPartyIndex;

			CGPartyDelUser(&pMsg, lpObj->m_Index);
		}
	}

	if ( lpObj->GuildNumber > 0 )
	{
		if ( lpObj->lpGuild->WarState == 1 )
		{
			gObjGuildWarMasterClose(lpObj);
		}

		GDCharClose(0, lpObj->lpGuild->Name, lpObj->Name);
	}
	else
	{
		GDCharClose(1, NULL, lpObj->Name);
	}

	if ( lpObj->WarehouseSave == 1 )
	{
		GDSetWarehouseList(lpObj->m_Index);
	}

	if ( (lpObj->MapNumber<11)?FALSE:(lpObj->MapNumber>17)?FALSE:TRUE )
	{
		g_BloodCastle.SearchUserDropQuestItem(lpObj->m_Index);
	}

	gObjSaveChaosBoxItemList(lpObj);

	if ( gObjFixInventoryPointer(aIndex) == false )
	{
		LogAdd("[Fix Inv.Ptr] False Location - %s, %d", __FILE__, __LINE__);
	}

	g_CashItemPeriodSystem.GDReqPeriodItemUpdate(lpObj);
	g_CashItemPeriodSystem.ClearPeriodItemEffect(lpObj);

	GJSetCharacterInfo(lpObj, aIndex, 0);
	gObjItemTextSave(lpObj);
	gObjStatTextSave(lpObj);
	gObjSavePetItemInfo(lpObj->m_Index, 0);

	gObjViewportClose(lpObj);
	LogAddTD(lMsg.Get(MSGGET(2, 4)), lpObj->m_Index, lpObj->AccountID, lpObj->Name);

	if(lpObj->Name)
	{
		WhisperCash.DelCash(lpObj->Name);
	}

	g_CashShop.DeleteUser(lpObj);

	if(lpObj->m_iPeriodItemEffectIndex != -1)
	{
		g_CashItemPeriodSystem.GDReqPeriodItemUpdate(lpObj);
		g_CashItemPeriodSystem.ClearPeriodItemEffect(lpObj);
	}

	memcpy(lpObj->BackName, lpObj->Name, sizeof(lpObj->Name)-1);
	lpObj->BackName[10] = 0;


	memset(lpObj->Name, 0, sizeof(lpObj->Name)-1);
	lpObj->Connected = PLAYER_LOGGED;

	if ( m_ObjBill[aIndex].GetCertify() >= 0 && m_ObjBill[aIndex].GetCertify() < 3)
	{
		gLCount[m_ObjBill[aIndex].GetCertify()].Delete();
	}

	return TRUE;
}



short gObjDel(int index)
{
	if ( index < 0 || index > OBJMAX )
	{
		LogAdd ("(%s)(%d) = index over error (%d)", __FILE__, __LINE__, index);
		return 0;
	}

	LPOBJ lpObj = &gObj[index];

	//DebugLog("%s Mob:%d,%d,%d",__FUNCTION__,index,lpObj->Class,lpObj->MapNumber);

	if ( lpObj->Connected < PLAYER_CONNECTED )
	{
		return 0;
	}

	if ( MAX_MAP_RANGE(lpObj->MapNumber) )
	{
		MapC[lpObj->MapNumber].ClearStandAttr(lpObj->m_OldX, lpObj->m_OldY);
		MapC[lpObj->MapNumber].ClearStandAttr(lpObj->X, lpObj->Y);
	}

	int UserConnectState = lpObj->Connected;

	if ( lpObj->Type == OBJ_USER )
	{
		gObjGameClose(index);

		if ( UserConnectState >= PLAYER_LOGGED )
		{
			if ( lpObj->m_bMapSvrMoveQuit == false )
			{
				GJPUserClose(lpObj);
				LogAddTD("(%d)logout-send : %s [%s]", index, lpObj->AccountID, lpObj->Ip_addr);
			}
			else
			{
				LogAddTD("[MapServerMng] User Quit without JoinServer logout-send [%s][%s]",
					lpObj->AccountID, lpObj->Name);
			}

			memset(lpObj->AccountID, 0, sizeof(lpObj->AccountID) -1);
			memset(lpObj->LastJoominNumber, 0, sizeof(lpObj->LastJoominNumber));
		}

		LogAddTD("(%d)logout : %s [%s]", index, lpObj->AccountID, lpObj->Ip_addr);
		IpCache.DelIp(lpObj->Ip_addr);
	}

	lpObj->Connected = PLAYER_EMPTY;
	return 1;
}



short gObjSometimeClose(int index)
{
	if ( gDisconnect == 1 )
	{
		return -1;
	}

	if ( index < 0 || index > OBJMAX )	// deathway fix needed
	{
		LogAdd("(%s)(%d) = index over error (%d)", __FILE__, __LINE__, index);
		return 0;
	}

	LPOBJ lpObj = &gObj[index];
	gObjTradeSave(lpObj, index);

	if ( lpObj->Connected > PLAYER_CONNECTED )
	{
		if ( lpObj->Type == OBJ_USER )
		{
			if ( lpObj->Connected == PLAYER_PLAYING )
			{
				if ( lpObj->PartyNumber >= 0 )
				{
					PMSG_PARTYDELUSER pMsg;
					int iPartyIndex = gParty.GetIndex(lpObj->PartyNumber, lpObj->m_Index, lpObj->DBNumber);

					if ( iPartyIndex >= 0 )
					{
						pMsg.Number = iPartyIndex;
						CGPartyDelUser(&pMsg, lpObj->m_Index);
					}
				}
			}
		}
	}

	return 1;
}


BOOL gObjIsGamePlaing(LPOBJ lpObj)
{
	if ( lpObj->Connected < PLAYER_PLAYING )
	{
		return false;
	}

	return TRUE;
}


BOOL gObjIsConnectedAccount(int aIndex)
{
	if (aIndex < 0 || aIndex > OBJMAX-1 )
	{
		return FALSE;
	}
	
	if ( gObj[aIndex].Type != OBJ_USER )
	{
		return FALSE;
	}

	if ( gObj[aIndex].Connected < PLAYER_LOGGED )
	{
		return FALSE;
	}

	if ( gObj[aIndex].CloseCount > 0 )
	{
		return FALSE;
	}

	return TRUE;
}


BOOL gObjIsConnectedGP(int aIndex)
{
	if (aIndex < 0 || aIndex > OBJMAX-1 )
	{
		return FALSE;
	}
	
	if ( gObj[aIndex].Type != OBJ_USER )
	{
		return FALSE;
	}

	if ( gObj[aIndex].Connected < PLAYER_PLAYING )
	{
		return FALSE;
	}

	if ( gObj[aIndex].CloseCount > 0 )
	{
		return FALSE;
	}

	return TRUE;
}


BOOL gObjIsConnectedGPBot(int aIndex)
{
	if (aIndex < 0 || aIndex > OBJMAX-1 )
	{
		return FALSE;
	}
	
	if ( aIndex < OBJ_MAXMONSTER )
	{
		return FALSE;
	}

	if ( gObj[aIndex].Connected < PLAYER_PLAYING )
	{
		return FALSE;
	}

	if ( gObj[aIndex].CloseCount > 0 )
	{
		return FALSE;
	}

	if ( gObj[aIndex].MasterCharacterInfo == NULL )
	{
		return FALSE;
	}

	return TRUE;
}



BOOL gObjIsConnectedGP(int aIndex, char * CharName)
{
	if (aIndex < 0 || aIndex > OBJMAX-1 )
	{
		return FALSE;
	}
	
	if ( gObj[aIndex].Type != OBJ_USER )
	{
		return FALSE;
	}

	if ( gObj[aIndex].Connected < PLAYER_PLAYING )
	{
		return FALSE;
	}

	if ( gObj[aIndex].CloseCount > 0 )
	{
		return FALSE;
	}

	if ( strcmp(gObj[aIndex].Name, CharName) != 0 )
	{
		return FALSE;
	}

	return TRUE;
}




BOOL gObjIsConnected(LPOBJ lpObj)
{
	if ( lpObj->Type != OBJ_USER )
	{
		return FALSE;
	}

	if ( lpObj->Connected < PLAYER_PLAYING )
	{
		return FALSE;
	}

	if ( lpObj->CloseCount > 0 )
	{
		return FALSE;
	}

	return TRUE;
}



BOOL gObjIsConnected(int aIndex) 
{
	if ( aIndex < 0 || aIndex > OBJMAX-1)
	{
		return FALSE;
	}
	if ( gObj[aIndex].Connected < PLAYER_PLAYING )
	{
		return FALSE;
	}
	return TRUE;
}






BOOL gObjIsConnected(LPOBJ lpObj, int dbnumber)
{
	if (lpObj->Connected < PLAYER_PLAYING)
	{
		return 0;
	}

	if (lpObj->DBNumber != dbnumber)
	{
		return 0;
	}

	return 1;
}

BOOL gObjIsAccontConnectProxy(int aIndex)
{
	if (aIndex <0 || aIndex > OBJMAX-1) // Bad
	{
		return 0;
	}

	if (gObj[aIndex].Connected< PLAYER_CONNECTED)
	{
		return 0;
	}

	if (strlen(&gObj[aIndex].AccountID[0]) < 1 )
	{
		return 0;
	}

	return 1;
}

BOOL gObjIsAccontConnect(int aIndex, char* accountid)
{
	if (aIndex <0 || aIndex > OBJMAX-1) // Bad
	{
		return 0;
	}

	if (gObj[aIndex].Connected< PLAYER_CONNECTED)
	{
		return 0;
	}

	if (strlen(&gObj[aIndex].AccountID[0]) < 1 )
	{
		return 0;
	}

	if ( strcmp(accountid, &gObj[aIndex].AccountID[0]) != 0)
	{
		return 0;
	}
	return 1;
}



BOOL gObjJoominCheck(int aIndex, char* szInJN)
{
	LPOBJ lpObj = &gObj[aIndex];
	int gLanguage = 0;

	if(ReadConfig.UseSNOCheckCode == FALSE)
		return TRUE;

	switch ( gLanguage )
	{
		case 0:
			if ( strncmp(szInJN, &lpObj->LastJoominNumber[0], 7) == 0 )
			{
				return TRUE;
			}
		case 1:
		case 5:
		case 6:
		case 8:
			if ( strncmp(szInJN, lpObj->LastJoominNumber, 7) == 0 )
			{
				return TRUE;
			}
		case 2:
			if ( strncmp(szInJN, &lpObj->LastJoominNumber[5], 8) == 0 )
			{
				return TRUE;
			}
		case 3:
			if ( strcmpi(szInJN, &lpObj->LastJoominNumber[6]) == 0 )
			{
				return TRUE;
			}
			break;
		case 4:
			if ( strlen(szInJN) < 2 )
			{
				return FALSE;
			}

			if ( (szInJN[0] >= 'A' && szInJN[0] <= 'Z') || (szInJN[0] >= 'a' && szInJN[0] <= 'z') )
			{
				if ( szInJN[0] >= 'A' && szInJN[0] <= 'Z' )
				{
					szInJN[0] = szInJN[0] + 32;
				}

				if ( lpObj->LastJoominNumber[0] >= 'A' && lpObj->LastJoominNumber[0] <= 'Z' )
				{
					lpObj->LastJoominNumber[0] = lpObj->LastJoominNumber[0] + 32;
				}

				if ( strncmp(lpObj->LastJoominNumber, szInJN, 10) == 0 )
				{
					return TRUE;
				}
			}

			break;

		default:

			if ( strncmp(szInJN, &lpObj->LastJoominNumber[6], 7) == 0 )
			{
				return TRUE;
			}
	}

	return FALSE;
}

/*BOOL gObjTaiwanJoominCheck(int aIndex, char* szInJN)
{
	LPOBJ lpObj = &gObj[aIndex];

	if ( strlen(szInJN) < 2 )
	{
		return FALSE;
	}
	
	if ( strncmp(&lpObj->LastJoominNumber[1], szInJN, 9) == 0 )
	{
		return TRUE;
	}

	return FALSE;
}*/




BOOL gObjCheckXYMapTile(LPOBJ lpObj, int iDbgName)
{
	if ( lpObj->Type != OBJ_USER )
	{
		return false;
	}

	if ( (lpObj->Authority&2 ) == 2 )
	{
		return FALSE;
	}

	if ( CC_MAP_RANGE(lpObj->MapNumber) )
	{
		int CCRest = MAP_INDEX_CHAOSCASTLE1;
		if (lpObj->MapNumber == MAP_INDEX_CHAOSCASTLE7)
			CCRest = 47;
		if ( g_ChaosCastle.GetCurrentState(lpObj->MapNumber-CCRest) == 2 )
		{
			return FALSE;
		}
	}

	int x = lpObj->X;
	int y = lpObj->Y;
	int mapnumber = lpObj->MapNumber;

	if ( mapnumber > MAX_MAP_NUMBER-1 )
	{
		mapnumber = MAP_INDEX_LORENCIA;
	}

	int mapminlevel = MapMinUserLevel[mapnumber];
	mapminlevel = 0;

	if (lpObj->Class == CLASS_MAGICGLADIATOR || 
		lpObj->Class == CLASS_DARKLORD ||
		lpObj->Class == CLASS_RAGEFIGHTER)
	{
		if ( mapminlevel > 0 )
		{
			if ( MapMinUserLevel[mapnumber] > 0 )
			{
				mapminlevel = MapMinUserLevel[mapnumber] - (MapMinUserLevel[mapnumber] / 3 * 2);
			}
		}
	}

	if ( lpObj->Class == CLASS_ELF && lpObj->Level < 10 )
	{
		if ( lpObj->MapNumber != MAP_INDEX_NORIA )
		{
			LogAddTD(lMsg.Get(MSGGET(2, 5)), lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y);
			PMSG_TELEPORT pMsg;
			pMsg.MoveNumber = 27;
			gObjMoveGate(lpObj->m_Index, pMsg.MoveNumber);
		}
	}
	else
	{
		if ( mapminlevel > lpObj->Level )
		{
			LogAddTD(lMsg.Get(MSGGET(2, 5)), lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y);
			PMSG_TELEPORT pMsg;
			pMsg.MoveNumber = 17;
			gObjMoveGate(lpObj->m_Index, pMsg.MoveNumber);
		}
	}

	BYTE attr = MapC[mapnumber].GetAttr(x, y);

	if (mapnumber != 30 
#if (PACK_EDITION>=3)
		&& DG_MAP_RANGE(mapnumber) == FALSE 
#endif
#if (PACK_EDITION>=2)
		&& IMPERIALGUARDIAN_MAP_RANGE(mapnumber) == FALSE
#endif
		)
	{
		if ( (attr&4) == 4 || (attr&8) == 8 )
		{
			BYTE attr2 = MapC[mapnumber].GetAttr(x+2, y);
			BYTE attr3 = MapC[mapnumber].GetAttr(x, y+2);
			BYTE attr4 = MapC[mapnumber].GetAttr(x-2, y);
			BYTE attr5 = MapC[mapnumber].GetAttr(x, y-2);

			if ( attr2 > 1 && attr3 > 1 && attr4 > 1 && attr5 > 1 )
			{
				LogAddTD("[Check Position] DbgName[%d] [%s][%s] Map[%d]-(%d,%d) Invalid location causes to force to move",iDbgName,lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y);
				PMSG_TELEPORT pMsg;
				pMsg.MoveNumber = 17;
				gObjMoveGate(lpObj->m_Index, pMsg.MoveNumber);

				return TRUE;
			}
		}
	}

	return FALSE;

}







BOOL gObjSetAccountLogin(int aIndex, char * szId, int aUserNumber, int aDBNumber, char* JoominNumber)
{
	if ( !OBJMAX_RANGE(aIndex) )
	{
		LogAdd("error : %s %d", __FILE__, __LINE__);
		return 0;
	}

	if ( gObj[aIndex].Connected != PLAYER_CONNECTED )
	{
		LogAddTD(lMsg.Get(MSGGET(2, 6)), aIndex, szId, aDBNumber, __FILE__, __LINE__);
		return 0;
	}

	if ( strcmp(gObj[aIndex].AccountID, szId) != 0 )
	{
		LogAdd(lMsg.Get(MSGGET(2, 7)), szId);
		return 0;
	}

	gObj[aIndex].UserNumber = aUserNumber;
	gObj[aIndex].DBNumber = aDBNumber;
	gObj[aIndex].Connected = PLAYER_LOGGED;
	strcpy(gObj[aIndex].LastJoominNumber, JoominNumber);
	return TRUE;

}







BOOL gObjGetUserNumber(int aIndex)
{
	int usernumber;

	if ( !OBJMAX_RANGE(aIndex))
	{
		LogAdd("error : %s %d",__FILE__, __LINE__);
		return 0;
	}

	usernumber=gObj[aIndex].UserNumber;
	return usernumber;
}








BOOL gObjGetNumber(int aIndex, int& UN, int& DBN)
{
	if ( aIndex < 0 || aIndex > OBJMAX-1)
	{
		LogAdd("error : %s %d",__FILE__, __LINE__);
		return 0;
	}
	UN=gObj[aIndex].UserNumber;
	DBN=gObj[aIndex].DBNumber;
	return 1;
}







// gObj Address 653DBF8
char* gObjGetAccountId(int aIndex)
{
	if ( aIndex < 0 || aIndex > OBJMAX-1)
	{
		LogAdd("error : %s %d",__FILE__, __LINE__);
		return 0;
	}

	if ( gObj[aIndex].Connected == 2)
	{
		return &gObj[aIndex].AccountID[0];
	}
	return 0;
}








int gObjGetIndex(char* szId)
{
	int n;

	for (n=OBJ_STARTUSERINDEX;n<OBJMAX; n++)
	{
		if ( gObj[n].Connected >= PLAYER_PLAYING)
		{
			if ( gObj[n].Name[0] == *szId )
			{
				if ( strcmp(&gObj[n].Name[0] , szId) == 0)
				{
					return n;
				}
			}
		}
	}
	return -1;
}


int gObjGetIndexbyAccount(char* szId)
{
	int n;

	for (n=OBJ_STARTUSERINDEX;n<OBJMAX; n++)
	{
		if ( gObj[n].Connected >= PLAYER_CONNECTED)
		{
			if ( strcmp(&gObj[n].AccountID[0] , szId) == 0)
			{
				return n;
			}
		}
	}
	return -1;
}


BOOL gObjUserIdConnectCheck(char* szId,int index)
{
	if ( index < 0 || index > OBJMAX)	// Deathway Fix Here
	{
		LogAdd("error : index error !! %s %d",__FILE__, __LINE__);
		return 0;
	}

	if ( gObj[index].Connected < PLAYER_PLAYING)
	{
		return 0;
	}

	if (gObj[index].Name[0] != *szId)
	{
		return 0;
	}

	if ( strcmp(gObj[index].Name , szId) == 0)
	{
		return 1;
	}
	return 0;
}








BYTE GetPathPacketDirPos(int px, int py)
{
	BYTE pos = 0;

	if (px <= -1 && py <= -1)
	{
		pos=0;
	}
	else if (px <= -1 && py == 0)
	{
		pos=7;
	}
	else if ( px <= -1 && py >= 1)
	{
		pos=6;
	}
	else if ( px == 0 && py <= -1)
	{
		pos= 1;
	}
	else if ( px == 0 && py >= 1)
	{
		pos = 5;
	}
	else if ( px >= 1 && py <=-1)
	{
		pos=2;
	}
	else if ( px >= 1 && py == 0)
	{
		pos = 3;
	}
	else if ( px >=1  && py >= 1)
	{
		pos = 4;
	}

	return pos;
}








int  gObjCalDistance(LPOBJ lpObj1, LPOBJ lpObj2)
{
	if(lpObj1->MapNumber != lpObj2->MapNumber)
		return 1000;

	if ( lpObj1->X == lpObj2->X && lpObj1->Y == lpObj2->Y )
	{
		return 0;
	}

	float tx = lpObj1->X - lpObj2->X;
	float ty = lpObj1->Y - lpObj2->Y;

	return sqrt( (tx*tx) + (ty*ty) );
}








BOOL gObjPositionCheck(LPOBJ lpObj)
{
	int ix = lpObj->m_OldX - lpObj->TX;
	int iy = lpObj->m_OldY - lpObj->TY;

	if ( ix <= -15 ||  ix >= 15 )
	{
		return FALSE;
	}

	if ( iy <= -15 ||  iy >= 15 )
	{
		return FALSE;
	}

	return TRUE;
}








BOOL gObjCheckTileArea(int aIndex, int x, int y, int dis)
{
	LPOBJ lpObj = & gObj[aIndex];

	if ( x < (lpObj->X - dis) || x > (lpObj->X + dis) || y < (lpObj->Y - dis) || y > (lpObj->Y + dis) )
	{
		return FALSE;
	}
	return TRUE;
}








BOOL ExtentCheck(int x, int y, int w, int h)
{
	//DaRKav fix
	/*if(ReadConfig.S6E2 == 1)
	{
		if(h == 12)
			h = 28;
	}*/

	if ( (x >=0) && (x<w) && (y >=0) && (y<h) )
	{
		return 1;
	}
	return 0;
}






void gObjSetInventory1Pointer(LPOBJ lpObj)
{
	lpObj->pInventory = lpObj->Inventory1;
	lpObj->pInventoryMap = lpObj->InventoryMap1;
	//lpObj->pInventoryCount  = &lpObj->InventoryCount1;
}





void gObjSetInventory2Pointer(LPOBJ lpObj)
{
	lpObj->pInventory = lpObj->Inventory2;
	lpObj->pInventoryMap = lpObj->InventoryMap2;
	//lpObj->pInventoryCount  = &lpObj->InventoryCount2;
}



	

void gObjAddMsgSend(LPOBJ lpObj, int aMsgCode, int aIndex, int  SubCode)
{
	for ( int n=0;n<MAX_MONSTER_SEND_MSG;n++)
	{
		if ( gSMMsg[lpObj->m_Index][n].MsgCode < 0 )
		{
			gSMMsg[lpObj->m_Index][n].MsgCode = aMsgCode;
			gSMMsg[lpObj->m_Index][n].MsgTime = GetTickCount();
			gSMMsg[lpObj->m_Index][n].SendUser = aIndex;
			gSMMsg[lpObj->m_Index][n].SubCode = SubCode;

			return;
		}
	}

	for ( int n=0;n<MAX_MONSTER_SEND_MSG;n++)
	{
		if ( gSMMsg[lpObj->m_Index][n].MsgCode == 2 || gSMMsg[lpObj->m_Index][n].MsgCode == 6 || gSMMsg[lpObj->m_Index][n].MsgCode == 7 )
		{
			gSMMsg[lpObj->m_Index][n].MsgCode = aMsgCode;
			gSMMsg[lpObj->m_Index][n].MsgTime = GetTickCount();
			gSMMsg[lpObj->m_Index][n].SendUser = aIndex;
			gSMMsg[lpObj->m_Index][n].SubCode = SubCode;
			
			return;
		}
	}
}






void gObjAddMsgSendDelay(LPOBJ lpObj, int aMsgCode, int aIndex, int delay, int SubCode)
{
	for ( int n=0;n<MAX_MONSTER_SEND_MSG;n++)
	{
		if ( gSMMsg[lpObj->m_Index][n].MsgCode < 0 )
		{
			gSMMsg[lpObj->m_Index][n].MsgCode = aMsgCode;
			gSMMsg[lpObj->m_Index][n].MsgTime = GetTickCount() + delay;
			gSMMsg[lpObj->m_Index][n].SendUser = aIndex;
			gSMMsg[lpObj->m_Index][n].SubCode = SubCode;

			return;
		}
	}

	for ( int n=0;n<MAX_MONSTER_SEND_MSG;n++)
	{
		if ( gSMMsg[lpObj->m_Index][n].MsgCode == 2 || gSMMsg[lpObj->m_Index][n].MsgCode == 6 || gSMMsg[lpObj->m_Index][n].MsgCode == 7 )
		{
			gSMMsg[lpObj->m_Index][n].MsgCode = aMsgCode;
			gSMMsg[lpObj->m_Index][n].MsgTime = GetTickCount();
			gSMMsg[lpObj->m_Index][n].SendUser= aIndex;
			gSMMsg[lpObj->m_Index][n].SubCode = SubCode;
			
			return;
		}
	}

	if (aMsgCode == 1)
	{
		int n = 0;
		//for ( int n=0;n<MAX_MONSTER_SEND_MSG;n++)
		//{
			gSMMsg[lpObj->m_Index][n].MsgCode = aMsgCode;
			gSMMsg[lpObj->m_Index][n].MsgTime = GetTickCount()+ delay;
			gSMMsg[lpObj->m_Index][n].SendUser = aIndex;
			gSMMsg[lpObj->m_Index][n].SubCode = SubCode;
		
			return;
		//}
	}
}





void gObjAddMsgSendDelayInSpecificQPos(LPOBJ lpObj, int aMsgCode, int aIndex, int delay, int SubCode, int iQPosition)
{
	if ( iQPosition >= 0 &&  iQPosition < MAX_MONSTER_SEND_MSG )
	{
		gSMMsg[lpObj->m_Index][iQPosition].MsgCode = aMsgCode;
		gSMMsg[lpObj->m_Index][iQPosition].MsgTime = GetTickCount();
		gSMMsg[lpObj->m_Index][iQPosition].SendUser = aIndex;
		gSMMsg[lpObj->m_Index][iQPosition].SubCode = SubCode;
	}

}






void gObjAddAttackProcMsgSendDelay(LPOBJ lpObj, int aMsgCode, int aIndex, int delay, int SubCode, int SubCode2)
{
	for ( int n=0;n<MAX_MONSTER_SEND_ATTACK_MSG;n++)
	{
		int iIndex = lpObj->m_Index - OBJ_STARTUSERINDEX;

		if ( gSMAttackProcMsg[iIndex][n].MsgCode < 0 )
		{
			gSMAttackProcMsg[iIndex][n].MsgCode = aMsgCode;
			gSMAttackProcMsg[iIndex][n].MsgTime = GetTickCount() + delay;
			gSMAttackProcMsg[iIndex][n].SendUser = aIndex;
			gSMAttackProcMsg[iIndex][n].SubCode = SubCode;
			gSMAttackProcMsg[iIndex][n].SubCode2 = SubCode2;

			return;
		}
	}

}





void gObjMsgProc(LPOBJ lpObj)
{
	int aIndex = lpObj->m_Index;

	for ( int n=0;n<MAX_MONSTER_SEND_MSG;n++)
	{
		if ( gSMMsg[aIndex][n].MsgCode >= 0 )
		{
			if ( GetTickCount() > gSMMsg[aIndex][n].MsgTime )
			{
				if ( lpObj->Type == OBJ_MONSTER || lpObj->Type == OBJ_NPC )
				{
					gObjMonsterStateProc(lpObj, gSMMsg[aIndex][n].MsgCode, gSMMsg[aIndex][n].SendUser, gSMMsg[aIndex][n].SubCode);
				}
				else
				{
					gObjStateProc(lpObj, gSMMsg[aIndex][n].MsgCode, gSMMsg[aIndex][n].SendUser, gSMMsg[aIndex][n].SubCode);
				}

				gSMMsg[aIndex][n].MsgCode = -1;
			}
		}
	}
}






void gObjStateProc(LPOBJ lpObj, int aMsgCode, int aIndex, int SubCode)
{
	if ( (  (aIndex<0)?FALSE:(aIndex > OBJMAX-1)?FALSE:TRUE ) == FALSE )
	{
		LogAdd("error : %s %d", __FILE__, __LINE__ );
		return;
	}

	switch ( aMsgCode )
	{
		case  2:
			gObjBackSpring(lpObj, &gObj[aIndex]);
			break;

		case  3:
			gObjMonsterDieLifePlus(lpObj, &gObj[aIndex]);
			break;

		case 1000:
			gObjBillRequest(lpObj);
			break;

		case  4:
			if ( lpObj->lpGuild != NULL && lpObj->lpGuild->lpTargetGuildNode != NULL )
			{
				gObjGuildWarEnd(lpObj->lpGuild, lpObj->lpGuild->lpTargetGuildNode);
			}
			break;

		case  5:
			if ( lpObj->lpGuild != NULL && lpObj->lpGuild->WarState != NULL && lpObj->lpGuild->WarType == 1 )
			{
				BattleSoccerGoalStart(0);
			}
			break;

		case  7:
			if ( lpObj->lpGuild != NULL && lpObj->lpGuild->WarState != NULL && lpObj->lpGuild->WarType == 1)
			{
				if ( lpObj->lpGuild->BattleGroundIndex >= 0 )
				{
					GCManagerGuildWarEnd(lpObj->lpGuild->Name);
				}
			}
			break;

		case 10:
			if ( lpObj->DamageReflect != 0 )
			{
				gObjAttack(lpObj, &gObj[aIndex], 0, 0, 0, SubCode, 0);
			}
			break;
		case 54:
			if( lpObj->Live )
			{
				CMagicInf * lpMagic = &DefMagicInf[AT_SKILL_EXPLOSION];
				gObjAttack(lpObj, &gObj[aIndex], lpMagic, 1, 0, SubCode, 0);
			}
			break;
	}
}







void gObjStateAttackProc(LPOBJ lpObj, int aMsgCode, int aIndex, int SubCode, int SubCode2)
{
	if ( OBJMAX_RANGE(aIndex) == FALSE )
	{
		LogAdd("error : %s %d", __FILE__, __LINE__ );
		return;
	}

	switch ( aMsgCode )
	{
		case 50:
			if ( lpObj->Live != FALSE )
			{
				CMagicInf * lpMagic = gObjGetMagicSearch(lpObj, SubCode);
				gObjAttack(lpObj, &gObj[aIndex], lpMagic, 0, 1, 0, SubCode2);
			}
			break;
		case 53:
			if( lpObj->Live )
			{
				CMagicInf * lpMagic = gObjGetMagicSearch(lpObj, SubCode);
				gObjAttack(lpObj, &gObj[aIndex], lpMagic, 1, 1, 0, 0);
			}
			break;
		case 51:
			gDarkSpirit[lpObj->m_Index].Attack(lpObj, &gObj[aIndex], NULL, SubCode, SubCode2);
			break;
	}

}






BOOL gObjBackSpring(LPOBJ lpObj, LPOBJ lpTargetObj)
{
	int tdir;
	int x;
	int y;
	BYTE attr;
	PMSG_POSISTION_SET pMove;

	if ( MAX_MAP_RANGE(lpObj->MapNumber) == FALSE )
	{
		LogAdd("error : %s %d", __FILE__, __LINE__);
		return FALSE;
	}

	/*if ( lpObj->Type == OBJ_USER )
	{
		if ( lpObj->m_LastTeleportTime > 0 )
		{
			return FALSE;
		}
	}*/

	//Blood Castle Statue
	if ( lpObj->Class >= 131 && lpObj->Class <= 134 )
	{
		return FALSE;
	}

	//CryWolf NPC and Objects
	if ( lpObj->Class >= 204 && lpObj->Class <= 209 )
	{
		return FALSE;
	}

	//Traps
	if ( lpObj->Class >= 100 && lpObj->Class <= 106 )
	{
		return FALSE;
	}

	//Imperial Guardian Doors, Statues
	if ( lpObj->Class >= 523 && lpObj->Class <= 528 )
	{
		return FALSE;
	}

	//Raklion Eggs
	if ( lpObj->Class >= 460 && lpObj->Class <= 462 )
	{
		return FALSE;
	}

	//CryWolf Altar Elf
#if (GS_CASTLE==1)
	/*if ( lpObj->Type == OBJ_USER )
	{
		if ( CRYWOLF_MAP_RANGE(lpObj->MapNumber) != FALSE )
		{
			if ( lpObj->Class == CLASS_ELF ) // Elf
			{
				for ( int iAltar = 205 ; iAltar <= 209 ; iAltar ++ )
				{
					int iUserIndex = g_CrywolfNPC_Altar.GetAltarUserIndex(iAltar);
					if ( iUserIndex == lpObj->m_Index )
					{
						return FALSE;
					}
				}
			}
		}
	}*/
#endif

	//CryWolf Balista (Tanker)
	if ( lpObj->Class == 348 )
	{
		return FALSE;
	}
	
	//Castle Siege NPC and Objects
	if (lpObj->Class == 277 || lpObj->Class == 283 || lpObj->Class == 288 || lpObj->Class == 278 || lpObj->Class == 215 || lpObj->Class == 216 || lpObj->Class == 217 || lpObj->Class == 218 || lpObj->Class == 219)
	{
		return FALSE;
	}

	#if (GS_CASTLE==1)
	if (g_CastleSiege.GetCrownUserIndex() == lpObj->m_Index)
	{
		return FALSE;
	}
	#endif

	if (lpObj->pInventory[4].m_Type == ITEMGET(13,4)) //Dark Horse
	{
		return FALSE;
	}

	if ( (rand()%3) == 0 )
	{
		if ( lpTargetObj->Dir < 4 )
		{
			tdir = lpTargetObj->Dir + 4;
		}
		else
		{
			tdir = lpTargetObj->Dir - 4;
		}

		tdir *= 2;
	}
	else
	{
		tdir = lpTargetObj->Dir * 2;
	}

	x = lpObj->X;
	y = lpObj->Y;
	x += RoadPathTable[tdir];
	y += RoadPathTable[1+tdir];
	attr = MapC[lpObj->MapNumber].GetAttr(x, y);

	if ( (attr&1)==1 || (attr&2)==2 || (attr&4)== 4 || (attr&8)== 8  )
	{
		return FALSE;
	}

	pMove.h.c = 0xC1;
	pMove.h.headcode = 0xDF;
	pMove.h.size = sizeof(pMove);
	pMove.X = x;
	pMove.Y = y;

	lpObj->m_Rest = 0;
	MapC[lpObj->MapNumber].ClearStandAttr(lpObj->m_OldX, lpObj->m_OldY);
	MapC[lpObj->MapNumber].SetStandAttr(x, y);
	lpObj->m_OldX = x;
	lpObj->m_OldY = y;
	RecvPositionSetProc(&pMove, lpObj->m_Index);

	if ( lpObj->Type == OBJ_USER )
	{
		lpObj->PathCount = 0;
	}

	return TRUE;
}







BOOL BackSpringCheck(int & x, int & y, int & dir, BYTE map)
{
	BYTE attr;
	int tx = x;
	int ty = y;
	int tdir = dir/2;

	tx = tx + RoadPathTable[dir];
	ty = ty + RoadPathTable[1+dir];

	attr = MapC[map].GetAttr(tx, ty);

	if  (  (attr&1)==1 || (attr&4)== 4 || (attr&8)== 8 )
	{
		tdir += 4;

		if ( tdir > 7 )
		{
			tdir -= 8;
		}

		dir = tdir * 2;
		return FALSE;
	}

	x = tx;
	y = ty;

	return TRUE;
}

BOOL gObjBackSpring2(LPOBJ lpObj, LPOBJ lpTargetObj, int count)
{
	if (MAX_MAP_RANGE(lpObj->MapNumber) == FALSE)
	{
		LogAdd("error : %s %d", __FILE__, __LINE__);
		return FALSE;
	}

	if (lpObj->Class == 287 || lpObj->Class == 286)
	{
		return FALSE;
	}

	if (lpObj->Class >= 131 && lpObj->Class <= 134)
	{
		return FALSE;
	}

	//Imperial Guardian Doors, Statues
	if ( lpObj->Class >= 523 && lpObj->Class <= 528 )
	{
		return FALSE;
	}

	//Raklion Eggs
	if ( lpObj->Class >= 460 && lpObj->Class <= 462 )
	{
		return FALSE;
	}

	//CryWolf Altar Elf
#if (GS_CASTLE==1)
	/*if ( lpObj->Type == OBJ_USER )
	{
		if ( CRYWOLF_MAP_RANGE(lpObj->MapNumber) != FALSE )
		{
			if ( lpObj->Class == CLASS_ELF ) // Elf
			{
				for ( int iAltar = 205 ; iAltar <= 209 ; iAltar ++ )
				{
					int iUserIndex = g_CrywolfNPC_Altar.GetAltarUserIndex(iAltar);
					if ( iUserIndex == lpObj->m_Index )
					{
						return FALSE;
					}
				}
			}
		}
	}*/
#endif
	
	//CryWolf NPC and Objects
	if (lpObj->Class >= 204 && lpObj->Class <= 209)
	{
		return FALSE;
	}

	//CryWolf Balista (Tanker)
	if (lpObj->Class == 348)
	{
		return FALSE;
	}
	
	//Castle Siege NPC and Objects
	if (lpObj->Class == 277 || lpObj->Class == 283 || lpObj->Class == 288 || lpObj->Class == 278 || lpObj->Class == 215 || lpObj->Class == 216 || lpObj->Class == 217 || lpObj->Class == 218 || lpObj->Class == 219)
	{
		return FALSE;
	}

	#if (GS_CASTLE==1)
	if (g_CastleSiege.GetCrownUserIndex() == lpObj->m_Index)
	{
		return FALSE;
	}
	#endif

	if (lpObj->pInventory[4].m_Type == ITEMGET(13,4)) //Dark Horse
	{
		return FALSE;
	}

	int tdir = GetPathPacketDirPos(lpObj->X - lpTargetObj->X, lpObj->Y - lpTargetObj->Y) * 2;
	int x = lpObj->X;
	int y = lpObj->Y;

	for (int n=0;n<count;n++)
	{
		if ( n >= 2 )
		{
			if (lpObj->Class == CLASS_DARKLORD || 
				lpObj->Class == CLASS_MAGICGLADIATOR || 
				lpObj->Class == CLASS_RAGEFIGHTER || 
				lpObj->Class == CLASS_KNIGHT  )
			{
				BackSpringCheck(x, y, tdir, lpObj->MapNumber);
			}
		}
		else
		{
			BackSpringCheck(x, y, tdir, lpObj->MapNumber);
		}
	}

	PMSG_POSISTION_SET pMove;

	pMove.h.c = 0xC1;
	pMove.h.headcode = 0xDF;
	pMove.h.size = sizeof(pMove);
	pMove.X = x;
	pMove.Y = y;

	lpObj->m_Rest = 0;
	MapC[lpObj->MapNumber].ClearStandAttr(lpObj->m_OldX, lpObj->m_OldY);
	MapC[lpObj->MapNumber].SetStandAttr(x, y);
	lpObj->m_OldX = x;
	lpObj->m_OldY = y;
	RecvPositionSetProc(&pMove, lpObj->m_Index);

	return TRUE;
}

int getCurrentEventType(LPOBJ lpObj, BYTE isParty)
{
	#if (GS_CASTLE==1)
	if(CRYWOLF_MAP_RANGE(lpObj->MapNumber))
	{
		if ( g_Crywolf.GetCrywolfState() == CRYWOLF_STATE_START )
			return EVENT_TYPE_CRYWOLF;
	}
	#endif

	if(BC_MAP_RANGE(lpObj->MapNumber))
	{
		return EVENT_TYPE_BLOODCASTLE;
	}
	if(CC_MAP_RANGE(lpObj->MapNumber))
	{
		return EVENT_TYPE_CHAOSCASTLE;
	}
	if(DS_MAP_RANGE(lpObj->MapNumber))
	{
		if (isParty == 0)
			return EVENT_TYPE_DEVILSQUARE;
		else
			return EVENT_TYPE_DEVILSQUARE_PARTY;
	}

	//KALIMA_MAP_RANGE(lpObj->MapNumber)
	//SKYEVENT_MAP_RANGE(lpObj->MapNumber, lpObj->X)
	//IT_MAP_RANGE(lpObj->MapNumber)
	//DG_MAP_RANGE(lpObj->MapNumber)
	//IMPERIALGUARDIAN_MAP_RANGE(lpObj->MapNumber)

	if (isParty == 0)
		return EVENT_TYPE_NONE;
	else
		return EVENT_TYPE_PARTY;
}

int gObjGetLevelUpPointsByEventID(LPOBJ lpObj,int iEventType)
{
	int LevelUpPoint = 0;

#if (PACK_EDITION>=2)
	LevelUpPoint = HitAndUp.GetAddExtraPoint(lpObj->DbClass);
	if (LevelUpPoint > 0)
	{
		LogAddTD("[%s][%s][%d] HitAndUp Event AddStat %d",
			lpObj->AccountID, lpObj->Name, iEventType,LevelUpPoint);
	}
#endif

	switch(iEventType)
	{
		case EVENT_TYPE_NONE:
		{
			if (gObjIsNewClass(lpObj) == 1)
				LevelUpPoint += ReadConfig.gObjLevelUpMasterNormal;

			if ( lpObj->Class == CLASS_DARKLORD )
				LevelUpPoint += ReadConfig.gObjLevelUpDLNormal;
			else if ( lpObj->Class == CLASS_MAGICGLADIATOR)
				LevelUpPoint += ReadConfig.gObjLevelUpMGNormal;
			else if ( lpObj->Class == CLASS_RAGEFIGHTER )
				LevelUpPoint += ReadConfig.gObjLevelUpRFNormal;
			else
				LevelUpPoint += ReadConfig.gObjLevelUpGralNormal;

			if (lpObj->PlusStatQuestClear != false)
			{
				LevelUpPoint += ReadConfig.gObjLevelUpPlusQuestNormal;
			}
		}
		break;

		case EVENT_TYPE_PARTY:
		{
			if (gObjIsNewClass(lpObj) == 1)
				LevelUpPoint += ReadConfig.gObjLevelUpMasterNormal;

			if ( lpObj->Class == CLASS_DARKLORD )
				LevelUpPoint += ReadConfig.gObjLevelUpDLNormal;
			else if ( lpObj->Class == CLASS_MAGICGLADIATOR)
				LevelUpPoint += ReadConfig.gObjLevelUpMGNormal;
			else if ( lpObj->Class == CLASS_RAGEFIGHTER )
				LevelUpPoint += ReadConfig.gObjLevelUpRFNormal;
			else
				LevelUpPoint += ReadConfig.gObjLevelUpGralNormal;

			if (lpObj->PlusStatQuestClear != false)
			{
				LevelUpPoint += ReadConfig.gObjLevelUpPlusQuestNormal;
			}
		}
		break;

		case EVENT_TYPE_DEVILSQUARE:
		{
			if (gObjIsNewClass(lpObj) == 1)
				LevelUpPoint += ReadConfig.gObjLevelUpMasterNormal;

			if ( lpObj->Class == CLASS_DARKLORD )
				LevelUpPoint += ReadConfig.gObjLevelUpDLNormal;
			else if ( lpObj->Class == CLASS_MAGICGLADIATOR)
				LevelUpPoint += ReadConfig.gObjLevelUpMGNormal;
			else if ( lpObj->Class == CLASS_RAGEFIGHTER )
				LevelUpPoint += ReadConfig.gObjLevelUpRFNormal;
			else
				LevelUpPoint += ReadConfig.gObjLevelUpGralNormal;

			if (lpObj->PlusStatQuestClear != false)
			{
				LevelUpPoint += ReadConfig.gObjLevelUpPlusQuestNormal;
			}
		}
		break;

		case EVENT_TYPE_DEVILSQUARE_PARTY:
		{
			if (gObjIsNewClass(lpObj) == 1)
				LevelUpPoint += ReadConfig.gObjLevelUpMasterNormal;

			if ( lpObj->Class == CLASS_DARKLORD )
				LevelUpPoint += ReadConfig.gObjLevelUpDLNormal;
			else if ( lpObj->Class == CLASS_MAGICGLADIATOR)
				LevelUpPoint += ReadConfig.gObjLevelUpMGNormal;
			else if ( lpObj->Class == CLASS_RAGEFIGHTER )
				LevelUpPoint += ReadConfig.gObjLevelUpRFNormal;
			else
				LevelUpPoint += ReadConfig.gObjLevelUpGralNormal;

			if (lpObj->PlusStatQuestClear != false)
			{
				LevelUpPoint += ReadConfig.gObjLevelUpPlusQuestNormal;
			}
		}
		break;

		case EVENT_TYPE_CHAOSCASTLE:
		{
			if (gObjIsNewClass(lpObj) == 1)
				LevelUpPoint += ReadConfig.gObjLevelUpMasterCC;

			if ( lpObj->Class == CLASS_DARKLORD )
				LevelUpPoint += ReadConfig.gObjLevelUpDLMGCC;
			else if ( lpObj->Class == CLASS_MAGICGLADIATOR )
				LevelUpPoint += ReadConfig.gObjLevelUpDLMGCC;
			else if ( lpObj->Class == CLASS_RAGEFIGHTER )
				LevelUpPoint += ReadConfig.gObjLevelUpDLMGCC;
			else
				LevelUpPoint += ReadConfig.gObjLevelUpGralCC;

			if (lpObj->PlusStatQuestClear != false)
			{
				LevelUpPoint += ReadConfig.gObjLevelUpPlusQuestCC;
			}
		}
		break;

		case EVENT_TYPE_BLOODCASTLE:
		{
			if (gObjIsNewClass(lpObj) == 1)
				LevelUpPoint += ReadConfig.gObjLevelUpMasterBC;

			if ( lpObj->Class == CLASS_DARKLORD )
				LevelUpPoint += ReadConfig.gObjLevelUpDLMGBC;
			else if ( lpObj->Class == CLASS_MAGICGLADIATOR )
				LevelUpPoint += ReadConfig.gObjLevelUpDLMGBC;
			else if ( lpObj->Class == CLASS_RAGEFIGHTER )
				LevelUpPoint += ReadConfig.gObjLevelUpDLMGBC;
			else
				LevelUpPoint += ReadConfig.gObjLevelUpGralBC;

			if (lpObj->PlusStatQuestClear != false)
			{
				LevelUpPoint += ReadConfig.gObjLevelUpPlusQuestBC;
			}
		}
		break;

		case EVENT_TYPE_CRYWOLF:
		{
			if (gObjIsNewClass(lpObj) == 1)
				LevelUpPoint += ReadConfig.gObjLevelUpMasterCW;

			if ( lpObj->Class == CLASS_DARKLORD )
				LevelUpPoint += ReadConfig.gObjLevelUpDLMGCW;
			else if ( lpObj->Class == CLASS_MAGICGLADIATOR )
				LevelUpPoint += ReadConfig.gObjLevelUpDLMGCW;
			else if ( lpObj->Class == CLASS_RAGEFIGHTER )
				LevelUpPoint += ReadConfig.gObjLevelUpDLMGCW;
			else
				LevelUpPoint += ReadConfig.gObjLevelUpGralCW;

			if (lpObj->PlusStatQuestClear != false)
			{
				LevelUpPoint += ReadConfig.gObjLevelUpPlusQuestCW;
			}
		}
		break;

#if (PACK_EDITION>=3)
		case EVENT_TYPE_AUTOEXP:
		{
			LevelUpPoint += AutoExp.LevelUpPoints;

			if (gObjIsNewClass(lpObj) == 1)
				LevelUpPoint += ReadConfig.gObjLevelUpMasterNormal;

			if ( lpObj->Class == CLASS_DARKLORD )
				LevelUpPoint += ReadConfig.gObjLevelUpDLNormal;
			else if ( lpObj->Class == CLASS_MAGICGLADIATOR)
				LevelUpPoint += ReadConfig.gObjLevelUpMGNormal;
			else if ( lpObj->Class == CLASS_RAGEFIGHTER )
				LevelUpPoint += ReadConfig.gObjLevelUpRFNormal;
			else
				LevelUpPoint += ReadConfig.gObjLevelUpGralNormal;

			if (lpObj->PlusStatQuestClear != false)
			{
				LevelUpPoint += ReadConfig.gObjLevelUpPlusQuestNormal;
			}
		}
		break;
#endif

		//Not known events
		default:
		{
			if (gObjIsNewClass(lpObj) == 1)
				LevelUpPoint += ReadConfig.gObjLevelUpMasterNormal;

			if ( lpObj->Class == CLASS_DARKLORD )
				LevelUpPoint += ReadConfig.gObjLevelUpDLNormal;
			else if ( lpObj->Class == CLASS_MAGICGLADIATOR)
				LevelUpPoint += ReadConfig.gObjLevelUpMGNormal;
			else if ( lpObj->Class == CLASS_RAGEFIGHTER )
				LevelUpPoint += ReadConfig.gObjLevelUpRFNormal;
			else
				LevelUpPoint += ReadConfig.gObjLevelUpGralNormal;

			if (lpObj->PlusStatQuestClear != false)
			{
				LevelUpPoint += ReadConfig.gObjLevelUpPlusQuestNormal;
			}
		}
		break;
	}
	LogAddTD("[%s][%s][%d] LevelUp PlusStatQuest Clear AddStat %d",
		lpObj->AccountID, lpObj->Name, iEventType,LevelUpPoint);

	return LevelUpPoint;
}


BOOL gObjIsMaxLevel(LPOBJ lpObj)
{
	if ( lpObj->Level >= ReadConfig.Max_Normal_Level )
	{
		if ( lpObj->Type != OBJ_USER )
			return TRUE;

		if(gObjIsNewClass(lpObj) != 1)
		{
			return TRUE;
		} else {
			if (lpObj->MasterCharacterInfo->MasterLevel >= ReadConfig.Max_Master_Level)
			{
				return TRUE;
			}
		}
	}

	return FALSE;
}

int gObjLevelUp(LPOBJ lpObj, int iAddExp, int iMonsterType, int iEventType, int & LevelUp)
{
	LevelUp = 0;
	LPSTR eventName;
	int iLEFT_EXP = 0;

	if ( OBJMAX_RANGE(lpObj->m_Index) == FALSE )
	{
		return -1;
	}

	switch(iEventType)
	{
		case EVENT_TYPE_NONE:
			eventName = "Regular";
		break;
		case EVENT_TYPE_PARTY:
			eventName = "Party";
		break;
		case EVENT_TYPE_DEVILSQUARE:
			eventName = "DevilSquare";
		break;
		case EVENT_TYPE_DEVILSQUARE_PARTY:
			eventName = "DevilSquareParty";
		break;
		case EVENT_TYPE_CHAOSCASTLE:
			eventName = "ChaosCastle";
		break;
		case EVENT_TYPE_BLOODCASTLE:
			eventName = "BloodCastle";
		break;
		case EVENT_TYPE_CRYWOLF:
			eventName = "CryWolf";
		break;
		case EVENT_TYPE_AUTOEXP:
			eventName = "AutoEXP";
		break;
		case EVENT_TYPE_EXTENDED_PVPSYSTEM:
			eventName = "ExPvPSystem";
		break;
		case EVENT_TYPE_SEASON5_QUEST:
			eventName = "S5Quest";
		break;

		default:
			eventName = "Unknown";
		break;
	}
	
	LogAddTD("[%s][%d] Experience : Map[%d]-(%d,%d) [%s][%s](%d) %u %d MonsterType : %d",
		eventName,iEventType,lpObj->MapNumber, lpObj->X, lpObj->Y, lpObj->AccountID, lpObj->Name, lpObj->Level, lpObj->Experience, iAddExp, iMonsterType);

	if ( gObjIsMaxLevel(lpObj) == TRUE )
	{
		::GCServerMsgStringSend(lMsg.Get(MSGGET(4, 112)), lpObj->m_Index, 1);
		return -1;
	}

	if ( (lpObj->Experience + iAddExp) < lpObj->NextExp )
	{
		lpObj->Experience += iAddExp;
		return 0;
	}

	iLEFT_EXP = lpObj->Experience + iAddExp - lpObj->NextExp;
	lpObj->Experience = lpObj->NextExp;

	if(ReadConfig.ExpSys == 0)
	{
		iLEFT_EXP = 0;
		LevelUp = 1;
	}

	if (lpObj->Level < ReadConfig.Max_Normal_Level)
	{
		lpObj->Level++;
		lpObj->LevelUpPoint += gObjGetLevelUpPointsByEventID(lpObj,iEventType);
	}
	else if (lpObj->Level >= ReadConfig.Max_Normal_Level)
	{
		if ( lpObj->Type == OBJ_USER )
		{
			if (gObjIsNewClass(lpObj) == 1)
			{
				if (lpObj->MasterCharacterInfo->MasterLevel < ReadConfig.Max_Master_Level)
				{
					Mastering.LevelUp(lpObj);
				}
			}
		}
	}

#if (PACK_EDITION>=2)
	#if (DSGN_RESET_SYSTEM==0)
		ResetChar.CheckSendMail(lpObj);
	#endif
	customQuest.LevelUp(lpObj);
#endif

	#if (DSGN_RESET_SYSTEM==1)

	#endif

	//gObjNextExpCal(lpObj);	Called in gObjCalCharacter
	LogAddTD(lMsg.Get(MSGGET(2, 8)), lpObj->AccountID, lpObj->Name, lpObj->Level);

	//=================================================================================
	if(lpObj->Class == CLASS_ELF)
	{
		if(lpObj->Level >= g_SkillAdditionInfo.GetInfinityArrowUseLevel() && lpObj->ChangeUP >= 1)
		{
			int iAddSkillPosition = gObjMagicAdd(lpObj,AT_SKILL_INFINITY_ARROW,0);

			if(iAddSkillPosition >= 0)
			{
				GCMagicListOneSend(lpObj->m_Index,iAddSkillPosition,0x4D,0xDC,0,0);
				LogAddTD("[%s][%s] Add Infinity Arrow Skill (Character Level : %d)(ChangeUp: %d)",
					lpObj->AccountID,lpObj->Name,lpObj->Level,lpObj->ChangeUP);
			}
			else
			{
				LogAddTD("[%s][%s] Fail - Add Infinity Arrow Skill (Character Level : %d)(ChangeUp: %d)",
					lpObj->AccountID,lpObj->Name,lpObj->Level,lpObj->ChangeUP);
			}
		}
	}

	gObjCalCharacter(lpObj->m_Index);

	lpObj->MaxLife += DCInfo.DefClass[ lpObj->Class ].LevelLife;
	lpObj->MaxMana += DCInfo.DefClass[ lpObj->Class ].LevelMana;
	lpObj->Life = lpObj->MaxLife + lpObj->AddLife;
	lpObj->Mana = lpObj->MaxMana + lpObj->AddMana;
	lpObj->iShield = lpObj->iMaxShield + lpObj->iAddShield;
	
	//gObjCalcShieldPoint(lpObj);
	gObjFixGagesOverflow(lpObj);
	GCReFillSend(lpObj->m_Index,lpObj->Life,0xFF,0,lpObj->iShield);

	GJSetCharacterInfo(lpObj, lpObj->m_Index, 0);
	//gObjItemTextSave(lpObj);
	gObjStatTextSave(lpObj);
	gObjSavePetItemInfo(lpObj->m_Index, 0);

	//gObjSetBP(lpObj->m_Index);
	GCLevelUpMsgSend(lpObj->m_Index, 1);
	gObjCalcMaxLifePower(lpObj->m_Index);

#if (PACK_EDITION>=2)
	ObjSkillAutoLearn.LevelUp(lpObj->m_Index);
#endif

	if (lpObj->Level >= ReadConfig.Max_Normal_Level)
	{
		if ( lpObj->Type == OBJ_USER )
		{
			if (gObjIsNewClass(lpObj) == 1)
			{
				if (lpObj->MasterCharacterInfo->MasterLevel < ReadConfig.Max_Master_Level)
				{
					Mastering.InfoSend(lpObj->m_Index);
				}
			}
		}
	}

	return iLEFT_EXP;
}







BOOL gObjLevelUpPointAdd(BYTE type, LPOBJ lpObj)	// type : [1:Strength][2:Agility][3:Vitality][4:Energy][5:Leadership]
{
	if ( lpObj->Type > OBJ_NPC )
	{
		LogAddC(2, lMsg.Get(MSGGET(2, 9)), lpObj->AccountID, lpObj->Name, __FILE__, __LINE__);
		return FALSE;
	}

	if ( lpObj->LevelUpPoint < 1 )
	{
		return FALSE;
	}

	switch ( type )
	{
		case 0:
		{
			if((lpObj->Strength+1) <= ReadConfig.MaxPoints[lpObj->Class][type])
			{
				lpObj->Strength++;
			}
			else
			{
				return FALSE;
			}
		}break;

		case 1:
		{
			if((lpObj->Dexterity+1) <= ReadConfig.MaxPoints[lpObj->Class][type])
			{
				lpObj->Dexterity++;
			}else
			{
				return FALSE;
			}
		}break;

		case 2:
		{
			if((lpObj->Vitality+1) <= ReadConfig.MaxPoints[lpObj->Class][type])
			{
				lpObj->Vitality++;

				lpObj->MaxLife += lpObj->VitalityToLife;
				gObjCalcMaxLifePower(lpObj->m_Index);
			}else
			{
				return FALSE;
			}
		}break;

		case 3:
		{
			if((lpObj->Energy+1) <= ReadConfig.MaxPoints[lpObj->Class][type])
			{
				lpObj->Energy++;
				lpObj->MaxMana += lpObj->EnergyToMana;
			}else
			{
				return FALSE;
			}
		}break;

		case 4:
		{
			if((lpObj->Leadership+1) <= ReadConfig.MaxPoints[lpObj->Class][type])
			{
				lpObj->Leadership++;
			}else
			{
				return FALSE;
			}
		}break;
	}

	lpObj->LevelUpPoint--;
	gObjCalCharacter(lpObj->m_Index);

	return TRUE;
}







void gObjMonsterItemLoadScript(LPOBJ lpObj, char* script_file)
{
	int Token;
	int type;
	int index;
	int level;
	int op1;
	int op2;
	int op3;
	int incount;
	
	SMDFile=fopen(script_file, "r");

	if (SMDFile == 0)
	{
		MsgBox("file open error %s", script_file);
		return;
	}
	incount=0;

	while ( true )
	{
		Token=GetToken();
		
		if (Token == 2)
		{
			break;
		}

		if (Token == 1)
		{
			type=TokenNumber;

			Token=GetToken();
			index=TokenNumber;

			Token=GetToken();
			level=TokenNumber;

			Token=GetToken();
			op1=TokenNumber;

			Token=GetToken();
			op2=TokenNumber;

			Token=GetToken();
			op3=TokenNumber;

			if (  gObjMonsterInventoryInsertItem(lpObj, type, index, level, op1, op2, op3)  != 0xFF)
			{
				incount++;

				if (incount > 75)
				{
					break;
				}
			}
		}
		
	}


	fclose(SMDFile);
}


void gObjGiveItemSearch(LPOBJ lpObj, int maxlevel)
{
#if ACTIVE_ITEM_SEARCH == 0
	//goto lavelend;
#endif

//	int count;
	//while (--count)
	{

	}

#if ACTIVE_ITEM_SEARCH == 0
	//labelend:
#endif
}





BYTE gObjWarehouseInsertItem(LPOBJ lpObj, int type, int index, int level, int pos)
{
	CItem item;

	int item_type = ItemGetNumberMake(type, index);
	item.m_Level =level;
	item.Convert(item_type, 0 , 0, 0, 0, 0,0, CURRENT_DB_VERSION);
	lpObj->pWarehouse[pos] = item;
	return pos;
}







void gObjGiveItemWarehouseSearch(LPOBJ lpObj, int maxlevel)
{
#if ACTIVE_ITEM_SEARCH == 0
	//goto lavelend;
#endif

	

#if ACTIVE_ITEM_SEARCH == 0
	//labelend:
#endif
}

BYTE gObjGuildMasterCapacityTest(LPOBJ lpObj)
{
	if (lpObj->Level < gGuildCreateLevel)
		return 0;

	if (lpObj->Resets < ReadConfig.ResetReq_NpcGuildMasterTalk)
		return 2;

	return 1;
}

void gObjNextExpCal(LPOBJ lpObj)
{
	if (gObjIsNewClass(lpObj) == 1 && lpObj->Type == OBJ_USER)
	{
		if (lpObj->Level >= ReadConfig.Max_Normal_Level)
		{
			lpObj->NextExp = gLevelExperience[lpObj->Level+lpObj->MasterCharacterInfo->MasterLevel];
		} else {
			lpObj->NextExp = gLevelExperience[lpObj->Level];
		}
	} else {
		lpObj->NextExp = gLevelExperience[lpObj->Level];
	}
}


int gObjExpCal(LPOBJ lpObj, int LevelFix)
{
	if (gObjIsNewClass(lpObj) == 1 && lpObj->Type == OBJ_USER)
	{
		if (lpObj->Level >= ReadConfig.Max_Normal_Level)
		{
			return gLevelExperience[lpObj->Level+lpObj->MasterCharacterInfo->MasterLevel+LevelFix];
		} else {
			return gLevelExperience[lpObj->Level+LevelFix];
		}
	} else {
		return gLevelExperience[lpObj->Level+LevelFix];
	}
	return 0;
}

BYTE retResistance(LPOBJ lpObj, int Resistance_Type)
{
	BYTE r = lpObj->m_Resistance[Resistance_Type];

	if ( lpObj->m_iSkillBrandOfSkillTime > 0 && Resistance_Type == R_ICE )	//Sacrifice Ice Property Fix
	{
		return 0;
	}

	if( r == 0xFF)
	{
		return 1;
	}

	if ( r > 0 && lpObj->m_iPotionSoulTime > 0 && (Resistance_Type == R_LIGHTNING || Resistance_Type == R_ICE ))
	{
		r = r + (r*50/100);
	}

	if ( r == 0 )
	{
		return 0;
	}

	r += 1;
	int rRes = rand()%r;

	if ( rRes == 0 )
	{
		return 0;
	}

	return 1;
}






BOOL retCalcSkillResistance(LPOBJ lpTargetObj, BOOL isDouble)
{
	if ( isDouble != FALSE )
	{
		if ( (rand()%100) < 45 )
		{
			return TRUE;
		}
	}
	else
	{
		if ( (rand()%100) < 40 )
		{
			return TRUE;
		}
	}

	return FALSE;
}




BYTE gObjAttackType(LPOBJ lpObjTarget,LPOBJ lpObjAttacker)
{
	//Return Values
	//0 - can not be attacked
	//1 - can be attacked
	//2 - can be attacked and poisoned

	if ( ATTRIBUTE_RANGE(lpObjTarget->m_Attribute) )
	{
		return 0;
	}

	if (lpObjAttacker->m_isDuelSpectator == true)
	{
		if (lpObjAttacker->MapNumber == MAP_INDEX_VULCANUS)
		{		
			return 0;
		} else {
			lpObjAttacker->m_isDuelSpectator = false;
		}
	}

	if (lpObjTarget->m_isDuelSpectator == true)
	{
		if (lpObjTarget->MapNumber == MAP_INDEX_VULCANUS)
		{		
			return 0;
		} else {
			lpObjTarget->m_isDuelSpectator = false;
		}
	}

	if ((lpObjTarget->Class >= 524 && lpObjTarget->Class <= 528) && lpObjTarget->Class != 526)
	{
		if(g_ImperialGuardian.DoorAttack(lpObjTarget) == FALSE)
		{
			return 0;
		} else {
			return 1;
		}
	} else {
		if(g_ImperialGuardian.MonsterAttack(lpObjTarget) == FALSE)
		{
			return 0;
		}
	}

	/*if(g_ImperialGuardian.DoorAttack(lpObjTarget) == FALSE)
	{
		return 0;
	} else {
		//Imperial Doors
		if (lpObjTarget->Class >= 524 && lpObjTarget->Class <= 528 )
		{
			return 1;
		}
	}*/

#if (PACK_EDITION>=3)
	if( lpObjTarget->IsBot > 1 )
	{
		return 0;
	}
#endif

#if (GS_CASTLE==1)
	if(CRYWOLF_MAP_RANGE(lpObjTarget->MapNumber))
	{
		if (lpObjTarget->Type == OBJ_MONSTER && 
			(g_Crywolf.GetCrywolfState() >= CRYWOLF_STATE_NOTIFY_1 && 
			g_Crywolf.GetCrywolfState() < CRYWOLF_STATE_START) ||
			g_Crywolf.GetCrywolfState() == CRYWOLF_STATE_END) // Crywolf Start
		{
			return 0;
		}
	}
#endif

	if(lpObjTarget->Class == 523)		//Imperial Trap
	{
		return 0;
	}

	if ( lpObjTarget->Class  >= 100 && lpObjTarget->Class <= 110 )	//Traps
	{
		return 0;
	}

	//CS: Slingshot Attack, Slingshot Defense
	if ( lpObjTarget->Class == 221 || lpObjTarget->Class == 222 )
	{
		return 0;
	}

	//Illusion Temple NPCs
	if ( lpObjTarget->Class >= 380 && lpObjTarget->Class <= 385 )
	{
		return 0;
	}

	//CS: Castle Gate, Life Stone, Guardian Statue, Canon Tower
#if (GS_CASTLE==1)
	if (lpObjTarget->Class == 277 && g_CastleSiegeSync.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE)
	{
		return 1;
	}
	else if (lpObjTarget->Class == 283 && g_CastleSiegeSync.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE)
	{
		return 1;
	}
	else if (lpObjTarget->Class == 288 && g_CastleSiegeSync.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE)
	{
		return 1;
	}
	else if ( lpObjTarget->Class == 277 || lpObjTarget->Class == 278 || lpObjTarget->Class == 283 || lpObjTarget->Class == 288 )
	{
		return 0;
	}
#else
	if ( lpObjTarget->Class == 277 || lpObjTarget->Class == 278 || lpObjTarget->Class == 283 || lpObjTarget->Class == 288 )
	{
		return 0;
	}
#endif

	//Devil Square
	//if ( DS_MAP_RANGE(lpObjTarget->MapNumber) )
	//{
	//	if ( lpObjTarget->Type == OBJ_USER )
	//	{
	//		return 0;
	//	}
	//}

	//Blood Castle
	if ( BC_MAP_RANGE(lpObjTarget->MapNumber) )
	{
		//if ( lpObjTarget->Type == OBJ_USER )
		//{
		//	return 0;
		//}

		if ( lpObjTarget->Type >= OBJ_MONSTER )
		{
			int BCRest = MAP_INDEX_BLOODCASTLE1;
			if(lpObjTarget->MapNumber == MAP_INDEX_BLOODCASTLE8)
				BCRest = 45;

			//Castle Gate
			if ( lpObjTarget->Class == 131 && g_BloodCastle.CheckMonsterKillSuccess(lpObjTarget->MapNumber-BCRest) == false)
			{
				return 0;
			}

			//Statue of Saint
			if ( (((lpObjTarget->Class - 132) < 0)?FALSE:((lpObjTarget->Class-132) > 2)?FALSE:TRUE) !=FALSE && g_BloodCastle.CheckBossKillSuccess(lpObjTarget->MapNumber-BCRest) == false )
			{
				return 0;
			}

			//Castle Gate
			if ( lpObjTarget->Class == 131 && g_BloodCastle.CheckMonsterKillSuccess(lpObjTarget->MapNumber-BCRest) == true)
			{
				return 1;
			}

			//Statue of Saint
			if ( (((lpObjTarget->Class - 132) < 0)?FALSE:((lpObjTarget->Class-132) > 2)?FALSE:TRUE) !=FALSE && g_BloodCastle.CheckBossKillSuccess(lpObjTarget->MapNumber-BCRest) == true )
			{
				return 1;
			}
		}
	}

	//Chaos Castle
	if ( CC_MAP_RANGE(lpObjTarget->MapNumber) )
	{
		if ( lpObjTarget->Type == OBJ_USER )
		{
			return 1;
		}
	}

	//Crossbow Guard, Berdysh Guard
	if ( lpObjTarget->Class == 247 || lpObjTarget->Class == 249 )
	{
		return 0;
	}

	if ( lpObjTarget->m_State != 2 )
	{
		return 0;
	}

	if( lpObjTarget->Live == false )
	{
		return 0;
	}

	if ( lpObjTarget->Teleport == 1 )
	{
		return 0;
	}

	return 2;
}




void gObjPlayerKiller(LPOBJ lpObj, LPOBJ lpTargetObj)
{
	if(PvP.gPKLevelIncreaseOff == 1)
	{
		return;
	}

	if ( lpObj->Type != OBJ_USER )
	{
		return;
	}

	if ( gObjGetRelationShip(lpObj, lpTargetObj) == 2 )
	{
		if ( lpObj->lpGuild!= NULL && lpTargetObj->lpGuild != NULL )
		{
			LogAddTD("[U.System][Rival][Player Kill] ( Killer [%s][%s] / %s / U:%d / R:%d ) vs ( Victim [%s][%s] / %s / U:%d / R:%d )",
				lpObj->AccountID, lpObj->Name, lpObj->lpGuild->Name, lpObj->lpGuild->iGuildUnion, lpObj->lpGuild->iGuildRival,
				lpTargetObj->AccountID, lpTargetObj->Name, lpTargetObj->lpGuild->Name, lpTargetObj->lpGuild->iGuildUnion, lpTargetObj->lpGuild->iGuildRival);
		}
		else
		{
			LogAddTD("[U.System][Rival][Player Kill][ERROR : Can't find GuildInfo] (  [%s][%s] ) vs ( [%s][%s] )",
				lpObj->AccountID,lpObj->Name,lpTargetObj->AccountID,lpTargetObj->Name);
		}
		return;
	}

	if(IT_MAP_RANGE(lpTargetObj->MapNumber))
	{
		return;
	}

	if ( CC_MAP_RANGE(lpTargetObj->MapNumber)  )
	{
		int CCRest = MAP_INDEX_CHAOSCASTLE1;
		if (lpObj->MapNumber == MAP_INDEX_CHAOSCASTLE7)
			CCRest = 47;
		if ( g_ChaosCastle.GetCurrentState(lpTargetObj->MapNumber-CCRest) == 2 )
		{
			qs5.KillPlayerInChaosCastle(lpObj->m_Index,(lpTargetObj->MapNumber-CCRest)+1);
			return;
		}
	}

	if (lpTargetObj->MapNumber == MAP_INDEX_CASTLESIEGE)
	{
		if ( g_CastleSiegeSync.GetCastleState() == 7 )
		{
			//if (lpObj->m_btCsJoinSide != lpTargetObj->m_btCsJoinSide )
			return;
		}
	}

	//if (( lpTargetObj->MapNumber >= 45)  && (lpTargetObj->MapNumber <= 50))
	//{
	//	if ( IllusionTemple.EventStart == 1 )
	//	{
	//		return;
	//	}
	//}

#if (PACK_EDITION>=2)
	if(HalloweenEvent.Enabled == 1 && HalloweenEvent.Start == true && HalloweenEvent.CheckMap(lpTargetObj->MapNumber) == true)
	{
		HalloweenPKItemBoxOpen(lpObj,lpTargetObj->MapNumber,lpTargetObj->X,lpTargetObj->Y);
		return;
	}
#endif

	if (( lpTargetObj->MapNumber == PvP.gObjPlayerKiller_PKMap)  && ((lpTargetObj->X >= PvP.gObjPlayerKiller_PKX1) && (lpTargetObj->X <= PvP.gObjPlayerKiller_PKX2)) && ((lpTargetObj->Y >= PvP.gObjPlayerKiller_PKY1) && (lpTargetObj->Y <= PvP.gObjPlayerKiller_PKY2)))
	{
		Gens.KillPlayer(lpObj->m_Index,lpTargetObj->m_Index);
		return;
	}

	if ( PvP.GetType(lpTargetObj->MapNumber) == 2)
	{
		Gens.KillPlayer(lpObj->m_Index,lpTargetObj->m_Index);
		return;
	}
	
	if ( PvP.GetType(lpTargetObj->MapNumber) == 4)
	{
		if(Gens.KillPlayer(lpObj->m_Index,lpTargetObj->m_Index) == true)
			return;
	}

	for ( int n=0;n<MAX_SELF_DEFENSE;n++)
	{
		if ( lpTargetObj->SelfDefense[n] >= 0 )
		{
			if ( lpTargetObj->SelfDefense[n] == lpObj->m_Index )
			{
				return;
			}
		}
	}

	int iOriginPkLevel = lpObj->m_PK_Level;
	int iOriginPkCount = lpObj->m_PK_Count;

	if(lpTargetObj->m_PK_Level <= 4)
	{
		lpObj->m_PK_Level++;
		lpObj->m_PK_Count++;

		//if(lpObj->m_PK_Count > 65000)
		//	lpObj->m_PK_Count = 65000;

		if (lpObj->m_PK_Level > 6)
			lpObj->m_PK_Level=6;

		if (lpObj->m_PK_Level < 4)
			lpObj->m_PK_Level=4;
	} 
	else if(lpTargetObj->m_PK_Level >= 5 && lpTargetObj->Level > 20)
	{
		if(lpObj->m_PK_Level < 4)	//Must not be a killer to advance in Hero Ranking
		{
			lpObj->m_PK_Count--;
			lpObj->m_PK_Level--;

			if(lpObj->m_PK_Count < 0)
				lpObj->m_PK_Count = 0;

			if (lpObj->m_PK_Level < 1)
				lpObj->m_PK_Level=1;

			if (lpObj->m_PK_Level > 6)
				lpObj->m_PK_Level=6;
		}
	}

	lpObj->m_PK_Time = 0;
	if (lpObj->m_PK_Level==iOriginPkLevel)
	{
		return;
	}

	GCPkLevelSend(lpObj->m_Index, lpObj->m_PK_Level);

	if ( lpObj->m_PK_Level == 4 )
	{
		GCServerMsgStringSend(lMsg.Get(MSGGET(4, 113)), lpObj->m_Index, 1);
	}

	LogAddTD("[%s][%s] PK Info Set (Killed Player) Origin(PkLevel:%d, PkCount:%d) Changed(PkLevel:%d, PkCount:%d)",
		lpObj->AccountID, lpObj->Name, iOriginPkLevel, iOriginPkCount, lpObj->m_PK_Level, lpObj->m_PK_Count);
}






void gObjUserDie(LPOBJ lpObj, LPOBJ lpTargetObj)
{
	int count = 76;
	int itemdrop = 0;
	int number = 0;
	int dropresult = 0;

	if(lpObj->Type != OBJ_USER)
	{
		return;
	}

	gObjSetKillCount(lpObj->m_Index,0);

	gObjUseSkill.RemoveAllCharacterInvalidMagicAndSkillState(lpObj);

	lpObj->m_iPotionBlessTime = 0;
	lpObj->m_iPotionSoulTime = 0;

	lpObj->m_ViewSkillState &= ~0x8000;
	lpObj->m_ViewSkillState &= 0xFFFEFFFF;

	if ( gObjTargetGuildWarCheck(lpObj,lpTargetObj) == 1 )
	{
		return;
	}

	//FIX PK FOR SIEGE
	if ( g_CastleSiegeSync.GetCastleState() == 7 )
	{
		if ( ( lpObj->MapNumber == MAP_INDEX_CASTLESIEGE ) && ( lpTargetObj->MapNumber == MAP_INDEX_CASTLESIEGE ) )
			return;
	}

	if(DS_MAP_RANGE(lpObj->MapNumber))
	{
		if(lpTargetObj->Type == OBJ_MONSTER)
		{

			LPMONSTER_ATTRIBUTE mAttr = gMAttr.GetAttr(lpTargetObj->Class);

			if(mAttr != 0)
			{
				LogAddTD("[DevilSquare] Dead In DevilSquare [%s][%s][%s]",lpObj->AccountID,lpObj->Name,mAttr->m_Name);
			}
		}

		return;
	}
	
	if(BC_MAP_RANGE(lpObj->MapNumber))
	{
		g_BloodCastle.SetUserState(lpObj->m_Index,1);
		int BCRest = MAP_INDEX_BLOODCASTLE1;
		if(lpObj->MapNumber == MAP_INDEX_BLOODCASTLE8)
			BCRest = 45;
		if(g_BloodCastle.GetCurrentState(lpObj->MapNumber-BCRest) == BC_STATE_PLAYING)
		{
			LogAddTD("[Blood Castle] (%d) Try to drop Ultimate Weapon [%s][%s]",lpObj->MapNumber - (BCRest-1),lpObj->AccountID,lpObj->Name);
			g_BloodCastle.SearchUserDropQuestItem(lpObj->m_Index);
		}
		else
		{
			LogAddTD("[Blood Castle] (%d) Try to delete Ultimate Weapon [%s][%s]",lpObj->MapNumber - (BCRest-1),lpObj->AccountID,lpObj->Name);
			g_BloodCastle.SearchUserDeleteQuestItem(lpObj->m_Index);
		}

		if(lpTargetObj->Type == OBJ_MONSTER)
		{
			LPMONSTER_ATTRIBUTE mAttr = gMAttr.GetAttr(lpTargetObj->Class);

			if(mAttr != 0)
			{
				LogAddTD("[Blood Castle] (%d) Dead In Blood Castle, Killed by Monster [%s][%s][%s]",lpObj->MapNumber - (BCRest-1),lpObj->AccountID,lpObj->Name,mAttr->m_Name);
				g_BloodCastle.SearchUserDropQuestItem(lpObj->m_Index);
			}
		}
		else if(lpTargetObj->Type == OBJ_USER)
		{
			LogAddTD("[Blood Castle] (%d) Dead In Blood Castle, Killed by Other User [%s][%s][%s]",lpObj->MapNumber - (BCRest-1),lpObj->AccountID,lpObj->Name,lpTargetObj->Name);
			g_BloodCastle.SearchUserDropQuestItem(lpObj->m_Index);
		}
		return;
	}
	else if(CC_MAP_RANGE(lpObj->MapNumber))
	{
		return;
	}
	else if(IT_MAP_RANGE(lpObj->MapNumber))
	{
		IllusionTemple.PlayerDieKillPoint(lpObj->m_Index,lpTargetObj->m_Index);
		return;
	}

	if((lpObj->Authority & 0x10) || (lpTargetObj->Authority & 0x10))
	{
		return;
	}

	if(lpTargetObj->Type == OBJ_MONSTER)
	{
		LogAddTD("[%s][%s] Killed [%s][%s] Pos:(%d,%d,%d)","Monster",lpTargetObj->Name,lpObj->AccountID,lpObj->Name,lpObj->MapNumber,lpObj->X,lpObj->Y);
	}
	else
	{
		LogAddTD("[%s][%s] (PkLevel = %d) Killed [%s][%s] (PkLevel = %d) Pos:(%d,%d,%d)",lpTargetObj->AccountID,lpTargetObj->Name,lpTargetObj->m_PK_Level,lpObj->AccountID,lpObj->Name,lpObj->m_PK_Level,lpObj->MapNumber,lpObj->X,lpObj->Y);
		LogAddTD("Killed by User State Victim:[%s][%s], Murderer:[%s][%s], Dead State (Duel:%d, GuildWar:%d, SeldDefense:%d)",lpObj->AccountID,lpObj->Name,lpTargetObj->AccountID,lpTargetObj->Name,!!g_DuelManager.IsOnDuel(lpTargetObj->m_Index,lpObj->m_Index),!!gObjTargetGuildWarCheck(lpObj,lpTargetObj),!!gObjIsSelfDefense(lpTargetObj,lpObj->m_Index));
		if(PvP.GetType(lpTargetObj->MapNumber) == 3)
			gObjAddSingleExp(lpObj,lpTargetObj);
	}

	if(PvP.gPkItemDrop == 1)
	{
		if(lpObj->Type == OBJ_USER)
		{
			if(lpObj->m_PK_Level >= 4)
			{
				if ((rand()%100) <= PvP.gPkItemDropPercent)
					itemdrop = 1;
				else
					itemdrop = 0;
			}
		}
	} else {
		itemdrop = 0;
	}

	if(gObjCanItemTouch(lpObj,0)==0)
	{
		itemdrop = 0;
	}

	if(lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER)
	{
		if(g_DuelManager.IsOnDuel(lpObj->m_Index,lpTargetObj->m_Index))
		{
			itemdrop = 0;
		}
	}

	//Drop item if PK and % of drop matches
	if(itemdrop == 1)
	{
		if(lpObj->m_PK_Level >= 4)
		{
			count = 24;

			while(count-- != 0)
			{
				number = rand()%INVETORY_WEAR_SIZE;

				if(lpObj->pInventory[number].IsItem()==1)
				{
					if(g_kJewelOfHarmonySystem.IsStrengthenByJewelOfHarmony(&lpObj->pInventory[number])==0)
					{
						if(lpObj->pInventory[number].m_Level <= PvP.gPKDropMaxLevel && getNumberOfExcOptions(lpObj->pInventory[number].m_NewOption) <= PvP.gPKDropMaxExc)
						{
							PMSG_ITEMTHROW lpMsg;
							lpMsg.Ipos = number;
							lpMsg.px = lpObj->X;
							lpMsg.py = lpObj->Y;
							if(CGItemDropRequest(&lpMsg,lpObj->m_Index,1,0)==1)
							{
								dropresult = 1;
								PLAYER_ITEMS_LOG.Output("[%s][%s] User Die Item Drop, ItemName:%s",lpObj->AccountID,lpObj->Name,lpObj->pInventory[number].GetName());
								break;
							}
						}
					}
				}
			}
		}
	}

	if(lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER ) 
	{
		if ( g_DuelManager.IsOnDuel(lpObj->m_Index,lpTargetObj->m_Index) )
		{
			lpTargetObj->m_btDuelScore += 1;
			g_DuelManager.PlayerScore(lpTargetObj);
		}
		return;
	}

	DWORD minexp = gObjExpCal(lpObj,-1);	//gLevelExperience[-1 + lpObj->Level];
	DWORD maxexp = gObjExpCal(lpObj, 0);	//gLevelExperience[lpObj->Level];

	DWORD subexp = 0;
	DWORD decexp = 0;
	DWORD decexprate = 0;

	if(lpObj->Level <= 20)
	{
		decexprate = 0;
	}
	else if(lpObj->Level <= 220)
	{
		decexprate = lpObj->m_PK_Level;
	}
	else
	{
		decexprate = lpObj->m_PK_Level;
		decexprate += 1;
	}

	if (decexprate<0)
		decexprate=0;

	decexp = (maxexp - minexp)*decexprate/100;

	if (decexp<0)
		decexp = decexp * -1;

	subexp = lpObj->Experience - decexp;

	if(subexp < minexp)
		subexp = minexp;

	LogAddTD("[%s][%s] Death reduces Experience %u - %d",lpObj->AccountID,lpObj->Name,subexp,decexp);

	DWORD submoney = 0;

	if(lpObj->Money > 0)
	{
		if(lpObj->Level > 10 && lpObj->Level <= 99)
		{
			submoney = (DWORD)(lpObj->Money)/100;
		}
		else if(lpObj->Level >= 100 && lpObj->Level <= 199)
		{
			submoney = (DWORD)(lpObj->Money*2)/100;
		}
		else if(lpObj->Level > 200)
		{
			submoney = (DWORD)(lpObj->Money*3)/100;
		}

		lpObj->Money = lpObj->Money - submoney;
		
		if(lpObj->Money < 0)
		{
			lpObj->Money = 0;
		}

		LogAddTD(lMsg.Get(574),lpObj->AccountID,lpObj->Name,lpObj->Money,submoney);
	}

	if(lpObj->WarehouseMoney > 0)
	{
		if(lpObj->Level > 10 && lpObj->Level <= 99)
		{
			submoney = (DWORD)(lpObj->WarehouseMoney)/100;
		}
		else if(lpObj->Level >= 100 && lpObj->Level <= 199)
		{
			submoney = (DWORD)(lpObj->WarehouseMoney*2)/100;
		}
		else if(lpObj->Level > 200)
		{
			submoney = (DWORD)(lpObj->WarehouseMoney*3)/100;
		}

		lpObj->WarehouseMoney = lpObj->WarehouseMoney - submoney;
		
		if(lpObj->WarehouseMoney < 0)
		{
			lpObj->WarehouseMoney = 0;
		}
		
		lpObj->WarehouseExNum = lpObj->WarehouseExNumLast;

		GDSetWarehouseMoney(lpObj->m_Index);

		LogAddTD(lMsg.Get(575),lpObj->AccountID,lpObj->Name,lpObj->WarehouseMoney,submoney);
	}

	lpObj->Experience = subexp;
	lpObj->DieMap = lpObj->MapNumber;
	lpObj->DieX = lpObj->X;
	lpObj->DieY = lpObj->Y;
}






void gObjInterfaceTimeCheck(LPOBJ lpObj)
{
	int tObjNum;
	BOOL TimeCheck = FALSE;

	if ( lpObj->m_IfState.use == 0 )
	{
		return;
	}

	if ( (GetTickCount() - lpObj->m_InterfaceTime) < 5000 )
	{
		return;
	}

	if ( lpObj->m_IfState.state == 1 )
	{
		return;
	}

	tObjNum = lpObj->TargetNumber;

	if ( tObjNum < 0 )
	{
		return;
	}

	if ( lpObj->m_IfState.type == 1 )
	{
		CGTradeResult(lpObj->m_Index, 3);
		CGTradeResult(tObjNum, 3);

		GCServerMsgStringSend(lMsg.Get(MSGGET(4, 114)), tObjNum, 1);
		TimeCheck = TRUE;
	}
	else
	{
		if ( lpObj->m_IfState.type == 2 )
		{
			lpObj->PartyTargetUser = -1;
			gObj[tObjNum].PartyTargetUser = -1;
			GCResultSend(tObjNum, 0x41, FALSE );
			TimeCheck = TRUE;
		}
	}

	if ( TimeCheck != FALSE )
	{
		lpObj->m_IfState.use = 0;
		lpObj->TargetNumber = -1;
		gObj[tObjNum].m_IfState.use = 0;
		gObj[tObjNum].TargetNumber = -1;
	}

}






void gObjPkDownTimeCheck(LPOBJ lpObj, int TargetLevel)
{
	if(lpObj->m_PK_Level == 3)
	{
		lpObj->m_PK_Time = 0;
		return;
	}

	if(lpObj->m_PK_Time == 0)
	{
		lpObj->m_PK_Time = GetTickCount();
		return;
	}

	if((TargetLevel > 1)&&(lpObj->m_PK_Level > 3))
	{
		int div = (lpObj->Level/lpObj->m_PK_Level*10);
		if(div > 0)
			lpObj->m_PK_Time -= TargetLevel/div;
	}


	if(lpObj->m_PK_Level < 3)
	{
		if( (GetTickCount()-lpObj->m_PK_Time) > (60000*PvP.gPkTimeInMinutes))		//1 Minute Times Config Minutes
		{
			lpObj->m_PK_Time = 0;
			lpObj->m_PK_Level += 1;

			GCPkLevelSend(lpObj->m_Index,lpObj->m_PK_Level);
		}
	}
	else if(lpObj->m_PK_Level > 3)
	{
		if( (GetTickCount()-lpObj->m_PK_Time) > (60000*PvP.gPkTimeInMinutes))		//1 Minute Times Config Minutes
		{
			lpObj->m_PK_Time = 0;
			lpObj->m_PK_Level -= 1;

			if(lpObj->m_PK_Level <= 5)
			{
				GCPkLevelSend(lpObj->m_Index,lpObj->m_PK_Level);
			}

			if(lpObj->m_PK_Level == 4)
			{
				GCServerMsgStringSend(lMsg.Get(1137),lpObj->m_Index,1);
			}
		}
	}
}



BOOL gObjAngelSprite(LPOBJ lpObj)
{
	if ( lpObj->Type != OBJ_USER )
	{
		return FALSE;
	}

	if ( lpObj->pInventory[8].IsItem() == FALSE )
	{
		return FALSE;
	}

	CItem * Angel = &lpObj->pInventory[8];

	if ( Angel->m_Type == ITEMGET(13,0) && Angel->m_Durability > 0.0f )
	{
		return TRUE;
	}

	return FALSE;
}


BOOL gObjSpiritOfGuardianSprite(LPOBJ lpObj)
{
	if ( lpObj->Type != OBJ_USER )
	{
		return FALSE;
	}

	if ( lpObj->pInventory[8].IsItem() == FALSE )
	{
		return FALSE;
	}

	CItem * Angel = &lpObj->pInventory[8];

	if ( Angel->m_Type == ITEMGET(13,65) && Angel->m_Durability > 0.0f )
	{
		return TRUE;
	}

	return FALSE;
}





BOOL gObjSatanSprite(LPOBJ lpObj)
{
	if ( lpObj->Type != OBJ_USER )
	{
		return FALSE;
	}

	if ( lpObj->pInventory[8].IsItem() == FALSE )
	{
		return FALSE;
	}

	CItem * Satan = &lpObj->pInventory[8];

	if ( Satan->m_Type == ITEMGET(13,1) && Satan->m_Durability > 0.0f )
	{
		return TRUE;
	}

	return FALSE;
}






BOOL gObjWingSprite(LPOBJ lpObj)
{
	if ( lpObj->Type != OBJ_USER )
	{
		return FALSE;
	}

	if ( lpObj->pInventory[7].IsItem() == FALSE )
	{
		return FALSE;
	}

	CItem * Wing = &lpObj->pInventory[7];

	if ((Wing->m_Type >= ITEMGET(12,0) && Wing->m_Type <= ITEMGET(12,6)) || 
		(Wing->m_Type == ITEMGET(13,30)) || 
		(Wing->m_Type >= ITEMGET(12,36) && Wing->m_Type <= ITEMGET(12,43)) || 
		(Wing->m_Type >= ITEMGET(12,49) && Wing->m_Type <= ITEMGET(12,50)) || 
		(Wing->m_Type >= ITEMGET(12,130) && Wing->m_Type <= ITEMGET(12,135)))
	{
		if ( Wing->m_Durability > 0.0f )
		{
			return TRUE;
		}
	}

#if (CRYSTAL_EDITION == 1)
	if (Wing->m_Type >= ITEMGET(12,200) && Wing->m_Type <= ITEMGET(12,254))
	{
		if ( Wing->m_Durability > 0.0f )
		{
			return TRUE;
		}
	}
#endif



	return FALSE;
}






BOOL gObjUniriaSprite(LPOBJ lpObj)
{
	if ( lpObj->pInventory[8].IsItem() == FALSE )
	{
		return FALSE;
	}

	CItem * Uniria = &lpObj->pInventory[8];

	if ( Uniria->m_Type == ITEMGET(13,2) && Uniria->m_Durability > 0.0f )
	{
		return TRUE;
	}

	return FALSE;
}



typedef struct tagITEM_BYTE_DB
{
	BYTE btType;
	BYTE btOption;
	BYTE btDurability;
	BYTE btExcellentOption;
	BYTE btAncientOption;

} ITEM_BYTE_DB, * LPITEM_BYTE_DB;


void gObjSpriteDamage(LPOBJ lpObj, int damage)
{
	if ( lpObj->Type != OBJ_USER )
	{
		return;
	}

	int send_dur = 0;
	BYTE reduce_dur = 1;

	if ( lpObj->pInventory[8].IsItem() == FALSE )
	{
		return;
	}

	if (MuItemShop.IsCShopItem(lpObj->m_Index,8) == true)
	{
		//reduce_dur = 0;
		return;
	}

	float fN = 10.0f;
	CItem * sprite = &lpObj->pInventory[8];
	float fdamage = damage;

	if ( sprite->m_Type == ITEMGET(13,0) ) //Guardian Angel
	{
		float absorbDamage = 30;

		LPITEMEFFECT lpItemEffect = NULL;
		lpItemEffect = g_ItemAddOption._SearchItemEffect(ITEMGET(13,0));
		if (lpItemEffect != NULL )
		{
			if (lpItemEffect->wEffectType1 == ADD_OPTION_DMG_ABSORB_PERCENT)
				absorbDamage = lpItemEffect->iEffectValue1;
			if (lpItemEffect->wEffectType2 == ADD_OPTION_DMG_ABSORB_PERCENT)
				absorbDamage = lpItemEffect->iEffectValue2;
		}

		fdamage = (damage*absorbDamage)/100.0f;
		fdamage /= fN;
		//sprite->m_Durability -= fdamage
	}
	else if ( sprite->m_Type == ITEMGET(13,1) )	//Satan
	{
		float absorbDamage = 20;

		LPITEMEFFECT lpItemEffect = NULL;
		lpItemEffect = g_ItemAddOption._SearchItemEffect(ITEMGET(13,1));
		if (lpItemEffect != NULL )
		{
			if (lpItemEffect->wEffectType1 == ADD_OPTION_DMG_ABSORB_PERCENT)
				absorbDamage = lpItemEffect->iEffectValue1;
			if (lpItemEffect->wEffectType2 == ADD_OPTION_DMG_ABSORB_PERCENT)
				absorbDamage = lpItemEffect->iEffectValue2;
		}

		fdamage = (damage*absorbDamage)/100.0f;
		fdamage /= fN;
	}
	else if ( sprite->m_Type == ITEMGET(13,2) )	//Horn of Unicorn
	{
		float absorbDamage = 10;

		LPITEMEFFECT lpItemEffect = NULL;
		lpItemEffect = g_ItemAddOption._SearchItemEffect(ITEMGET(13,2));
		if (lpItemEffect != NULL )
		{
			if (lpItemEffect->wEffectType1 == ADD_OPTION_DMG_ABSORB_PERCENT)
				absorbDamage = lpItemEffect->iEffectValue1;
			if (lpItemEffect->wEffectType2 == ADD_OPTION_DMG_ABSORB_PERCENT)
				absorbDamage = lpItemEffect->iEffectValue2;
		}

		fdamage = (damage*absorbDamage)/100.0f;
		fdamage /= fN;
	}
	else if ( sprite->m_Type == ITEMGET(13,3) )	//Horn of Dinorant
	{
		float absorbDamage = 5;

		LPITEMEFFECT lpItemEffect = NULL;
		lpItemEffect = g_ItemAddOption._SearchItemEffect(ITEMGET(13,3));
		if (lpItemEffect != NULL )
		{
			if (lpItemEffect->wEffectType1 == ADD_OPTION_DMG_ABSORB_PERCENT)
				absorbDamage = lpItemEffect->iEffectValue1;
			if (lpItemEffect->wEffectType2 == ADD_OPTION_DMG_ABSORB_PERCENT)
				absorbDamage = lpItemEffect->iEffectValue2;
		}

		fdamage = (damage*absorbDamage)/100.0f;
		fdamage /= fN;
	}		
	else if ( sprite->m_Type == ITEMGET(13,4) )	//Dark Horse
	{
		if ( sprite->m_Durability < 1.0f )
		{
			return;
		}

		float absorbDamage = 20;

		LPITEMEFFECT lpItemEffect = NULL;
		lpItemEffect = g_ItemAddOption._SearchItemEffect(ITEMGET(13,4));
		if (lpItemEffect != NULL )
		{
			if (lpItemEffect->wEffectType1 == ADD_OPTION_DMG_ABSORB_PERCENT)
				absorbDamage = lpItemEffect->iEffectValue1;
			if (lpItemEffect->wEffectType2 == ADD_OPTION_DMG_ABSORB_PERCENT)
				absorbDamage = lpItemEffect->iEffectValue2;
		}

		fdamage = (damage*absorbDamage)/100.0f;
		fdamage /= fN;
		fdamage += 1.0f;

		if (reduce_dur == 1)
		{
			sprite->m_DurabilitySmall += (int)fdamage;

			if ( sprite->m_DurabilitySmall > 1500 )
			{
				if(ReadConfig.S6E2 == 0)
				{
					sprite->m_Durability -= 1.0f;
				}
				else
				{
					if(gObjIsNewClass(lpObj) == 1 && lpObj->MasterCharacterInfo->DecPetLifeReduction > 0)
						sprite->m_Durability -= (100 - lpObj->MasterCharacterInfo->DecPetLifeReduction)/100.0f;
					else
						sprite->m_Durability -= 1.0f;			
				}
				sprite->m_DurabilitySmall = 0;
				send_dur = 1;

				if ( sprite->m_Durability < 1.0f )
				{
					sprite->m_Durability = 0;

					int	iPetExperience = lpObj->pInventory[1].m_PetItem_Exp;

					if ( sprite->DecPetItemExp(10) != FALSE )
					{
						LogAddTD("[%s][%s][PetItemExpDown] [%s] Level:[%d]Exp:[%d]DecExp[%d]",
							lpObj->AccountID, lpObj->Name, lpObj->pInventory[8].GetName(),
							lpObj->pInventory[8].m_PetItem_Level, lpObj->pInventory[8].m_PetItem_Exp,
							iPetExperience - lpObj->pInventory[1].m_PetItem_Exp);
						CDarkSpirit::SendLevelmsg(lpObj->m_Index, 8, 1, (BYTE)-1);
					}

					LogAddTD("[%s][%s] Item is Broken because durability is exhausted [%d]",
						lpObj->AccountID,lpObj->Name,lpObj->pInventory[8].m_Number
					);
				}

				GCItemDurSend(lpObj->m_Index, 8, sprite->m_Durability, 0);
			}
		}
	
		return;
	}
	else if(sprite->m_Type == ITEMGET(13,37))	//Fernir
	{
		if ( sprite->m_Durability < 1.0f )
		{
			sprite->m_Durability = 1.0f;
		}

		int iMaxDurSmall = g_iFenrirDefaultMaxDurSmall;
		int iMultipleDmg = 2;

		if(lpObj->Class == CLASS_WIZARD)
		{

		}
		else if(lpObj->Class == CLASS_KNIGHT)
		{

		}
		else if(lpObj->Class == CLASS_ELF)
		{
			iMaxDurSmall = g_iFenrirElfMaxDurSmall;
			iMultipleDmg = 2;
		}

		fdamage = damage * iMultipleDmg / 10.0f;
		fdamage /= fN;
		fdamage += 1.0f;

		if (reduce_dur == 1)
		{
			sprite->m_DurabilitySmall += (int)fdamage;

			if(sprite->m_DurabilitySmall > iMaxDurSmall)
			{
				if(ReadConfig.S6E2 == 0)
				{
					sprite->m_Durability -= 1.0f;
				}
				else
				{
					if(gObjIsNewClass(lpObj) == 1 && lpObj->MasterCharacterInfo->Repair[2] > 0)
						sprite->m_Durability -= (100 - lpObj->MasterCharacterInfo->Repair[2])/100.0f;
					else
						sprite->m_Durability -= 1.0f;			
				}
				sprite->m_DurabilitySmall = 0;

				if(sprite->m_Durability < 1.0f)
				{
					sprite->m_Durability = 0;
				}

				send_dur = 1;
			}
		}

		reduce_dur = 0;
	}
	else if ( sprite->m_Type == ITEMGET(13,64) ) // Demon Durability Reduction
	{
		float absorbDamage = 20;

		LPITEMEFFECT lpItemEffect = NULL;
		lpItemEffect = g_ItemAddOption._SearchItemEffect(ITEMGET(13,64));
		if (lpItemEffect != NULL )
		{
			if (lpItemEffect->wEffectType1 == ADD_OPTION_DMG_ABSORB_PERCENT)
				absorbDamage = lpItemEffect->iEffectValue1;
			if (lpItemEffect->wEffectType2 == ADD_OPTION_DMG_ABSORB_PERCENT)
				absorbDamage = lpItemEffect->iEffectValue2;
		}

		fdamage = (damage*absorbDamage)/100.0f;
		fdamage /= fN;
	}
	else if ( sprite->m_Type == ITEMGET(13,65) ) // New Angel Durability Reduction
	{
		float absorbDamage = 30;

		LPITEMEFFECT lpItemEffect = NULL;
		lpItemEffect = g_ItemAddOption._SearchItemEffect(ITEMGET(13,65));
		if (lpItemEffect != NULL )
		{
			if (lpItemEffect->wEffectType1 == ADD_OPTION_DMG_ABSORB_PERCENT)
				absorbDamage = lpItemEffect->iEffectValue1;
			if (lpItemEffect->wEffectType2 == ADD_OPTION_DMG_ABSORB_PERCENT)
				absorbDamage = lpItemEffect->iEffectValue2;
		}

		fdamage = (damage*absorbDamage)/100.0f;
		fdamage /= fN;
	}
	else if ( sprite->m_Type == ITEMGET(13,67) ) // Rudolf
	{
		float absorbDamage = 20;

		LPITEMEFFECT lpItemEffect = NULL;
		lpItemEffect = g_ItemAddOption._SearchItemEffect(ITEMGET(13,67));
		if (lpItemEffect != NULL )
		{
			if (lpItemEffect->wEffectType1 == ADD_OPTION_DMG_ABSORB_PERCENT)
				absorbDamage = lpItemEffect->iEffectValue1;
			if (lpItemEffect->wEffectType2 == ADD_OPTION_DMG_ABSORB_PERCENT)
				absorbDamage = lpItemEffect->iEffectValue2;
		}

		fdamage = (damage*absorbDamage)/100.0f;
		fdamage /= fN;
	}
	else if ( sprite->m_Type == ITEMGET(13,80) ) // Panda Durability Decrease
	{
		float absorbDamage = 30;

		LPITEMEFFECT lpItemEffect = NULL;
		lpItemEffect = g_ItemAddOption._SearchItemEffect(ITEMGET(13,80));
		if (lpItemEffect != NULL )
		{
			if (lpItemEffect->wEffectType1 == ADD_OPTION_DMG_ABSORB_PERCENT)
				absorbDamage = lpItemEffect->iEffectValue1;
			if (lpItemEffect->wEffectType2 == ADD_OPTION_DMG_ABSORB_PERCENT)
				absorbDamage = lpItemEffect->iEffectValue2;
		}

		fdamage = (damage*absorbDamage)/100.0f;
		fdamage /= fN;
	}
	else if ( sprite->m_Type == ITEMGET(13,106) ) // Unicorn
	{
		float absorbDamage = 20;

		LPITEMEFFECT lpItemEffect = NULL;
		lpItemEffect = g_ItemAddOption._SearchItemEffect(ITEMGET(13,106));
		if (lpItemEffect != NULL )
		{
			if (lpItemEffect->wEffectType1 == ADD_OPTION_DMG_ABSORB_PERCENT)
				absorbDamage = lpItemEffect->iEffectValue1;
			if (lpItemEffect->wEffectType2 == ADD_OPTION_DMG_ABSORB_PERCENT)
				absorbDamage = lpItemEffect->iEffectValue2;
		}

		fdamage = (damage*absorbDamage)/100.0f;
		fdamage /= fN;
	}
	else if ( sprite->m_Type == ITEMGET(13,123) ) // Skeleton Durability
	{
		float absorbDamage = 30;

		LPITEMEFFECT lpItemEffect = NULL;
		lpItemEffect = g_ItemAddOption._SearchItemEffect(ITEMGET(13,123));
		if (lpItemEffect != NULL )
		{
			if (lpItemEffect->wEffectType1 == ADD_OPTION_DMG_ABSORB_PERCENT)
				absorbDamage = lpItemEffect->iEffectValue1;
			if (lpItemEffect->wEffectType2 == ADD_OPTION_DMG_ABSORB_PERCENT)
				absorbDamage = lpItemEffect->iEffectValue2;
		}

		fdamage = (damage*absorbDamage)/100.0f;
		fdamage /= fN;
	}
	else
	{
		return;
	}

	if (reduce_dur == 1)
	{
		if(ReadConfig.S6E2 == 0)
		{
			sprite->m_Durability -= fdamage;
		}
		else
		{
			if(gObjIsNewClass(lpObj) == 1 && lpObj->MasterCharacterInfo->Repair[2] > 0)
				sprite->m_Durability -= (fdamage * (100-lpObj->MasterCharacterInfo->Repair[2]))/100.0f;
			else
				sprite->m_Durability -= fdamage;			
		}
		send_dur = 1;
	}

	if ( send_dur != FALSE )
	{
		GCItemDurSend(lpObj->m_Index, 8, sprite->m_Durability, 0);

		if ( sprite->m_Durability < 1.0f )
		{
			int itemnum = sprite->m_Type;

			LogAddTD(lMsg.Get(MSGGET(2, 10)), lpObj->AccountID, lpObj->Name,
				lpObj->pInventory[8].GetName(), lpObj->pInventory[8].m_Level);

			gObjInventoryDeleteItem(lpObj->m_Index, 8);
			GCInventoryItemDeleteSend(lpObj->m_Index, 8, 0);
			gObjMakePreviewCharSet(lpObj->m_Index);
			GCEquipmentChange(lpObj->m_Index, 8);

			if ( BC_MAP_RANGE(lpObj->MapNumber) || CC_MAP_RANGE(lpObj->MapNumber) )
			{
				if ( itemnum == ITEMGET(13,0) || itemnum == ITEMGET(13,1))
				{
					int pos = -1;

					for ( int i=MAX_PLAYER_EQUIPMENT;i<ReadConfig.MAIN_INVENTORY_SIZE(lpObj->m_Index,false);i++)
					{
						if ( lpObj->pInventory[i].IsItem() == TRUE )
						{
							if ( lpObj->pInventory[i].m_Type == itemnum )
							{
								pos = i;
								break;
							}
						}
					}

					if ( pos != -1 )
					{
						BYTE ItemInfo[MAX_ITEM_INFO];
						BOOL DurSend;
						BOOL DurTargetSend;

						ItemByteConvert(ItemInfo, lpObj->pInventory[pos]);
						gObjInventoryMoveItem(lpObj->m_Index, pos, 8, DurSend, DurTargetSend, 0, 0, ItemInfo);
						GCItemMoveResultSend(lpObj->m_Index, 0, 8, ItemInfo);
						GCInventoryItemDeleteSend(lpObj->m_Index, pos, 0);
						GCServerMsgStringSend(lMsg.Get(MSGGET(4, 158)), lpObj->m_Index, 1);
					}
				}
			}
				
			if ( lpObj->MapNumber == MAP_INDEX_ICARUS && itemnum == ITEMGET(13,3) || lpObj->MapNumber == MAP_INDEX_ICARUS && itemnum == ITEMGET(13,37))
			{
				int pos = -1;

				for ( int i=MAX_PLAYER_EQUIPMENT;i<ReadConfig.MAIN_INVENTORY_SIZE(lpObj->m_Index,false);i++)
				{
					if ( lpObj->pInventory[i].IsItem() == TRUE )
					{
						if ( lpObj->pInventory[i].m_Type == ITEMGET(13,3) )
						{
							pos = i;
							break;
						}
					}
				}

				if ( pos == -1 )
				{
					if ( lpObj->pInventory[7].IsItem() == FALSE )
					{
						gObjMoveGate(lpObj->m_Index, 22);
					}
				}
				else
				{
					BYTE ItemInfo[MAX_ITEM_INFO];
					BOOL DurSend;
					BOOL DurTargetSend;

					ItemByteConvert(ItemInfo, lpObj->pInventory[pos]);
					gObjInventoryMoveItem(lpObj->m_Index, pos, 8, DurSend, DurTargetSend, 0, 0, ItemInfo);
					GCItemMoveResultSend(lpObj->m_Index, 0, 8, ItemInfo);
					GCInventoryItemDeleteSend(lpObj->m_Index, pos, 0);
					GCServerMsgStringSend(lMsg.Get(MSGGET(4, 158)), lpObj->m_Index, 1);
				}
			}
		}

	}
}

void gObjOfficialHelperStatus(LPOBJ lpObj, BYTE Status)
{
	if(ReadConfig.OfficialHelper_Activate == 1)
	{
		PMSG_OFFICIALHELPER_RESULT pHelper={0};
		PHeadSubSetB((LPBYTE)&pHelper, 0xBF, 0x51, sizeof(pHelper));
		
		pHelper.State = Status;
		if(Status == 2)
		{
			lpObj->OfficialHelper_Active = false;
			lpObj->OfficialHelper_TimeCount = 0;
			DataSend(lpObj->m_Index, (LPBYTE)&pHelper, sizeof(pHelper));
		}else if(Status == 0)
		{
			if(lpObj->Money >= ReadConfig.OfficialHelper_Zen)
			{
				lpObj->OfficialHelper_Active = true;
				lpObj->OfficialHelper_TimeCount = 0;
				DataSend(lpObj->m_Index, (LPBYTE)&pHelper, sizeof(pHelper));
			}
		}else
		{	
			if(lpObj->Money >= ReadConfig.OfficialHelper_Zen)
			{
				pHelper.State = 0;
				pHelper.IsZen = 1;
				if(ReadConfig.OfficialHelper_Zen > 0)
				{
					pHelper.Zen = (lpObj->OfficialHelper_TimeCount * 100) / 300;
					lpObj->Money -= ReadConfig.OfficialHelper_Zen;
					GCMoneySend(lpObj->m_Index,lpObj->Money);
				}
				DataSend(lpObj->m_Index, (LPBYTE)&pHelper, sizeof(pHelper));
			}
			
			lpObj->OfficialHelper_Active = false;
			lpObj->OfficialHelper_TimeCount = 0;
			pHelper.IsZen = 0;
			pHelper.Zen = 0;
			pHelper.State = 1;
			DataSend(lpObj->m_Index, (LPBYTE)&pHelper, sizeof(pHelper));
		}
	}
}

void gObjOfficialHelperTimer(LPOBJ lpObj)
{
	if(ReadConfig.OfficialHelper_Activate == 1)
	{
		if(lpObj->OfficialHelper_Active == true)
		{
			if(lpObj->OfficialHelper_TimeCount > 300)
			{
				lpObj->OfficialHelper_TimeCount = 0;
				if(lpObj->Money >= ReadConfig.OfficialHelper_Zen)
				{	
					PMSG_OFFICIALHELPER_RESULT pHelper={0};
					PHeadSubSetB((LPBYTE)&pHelper, 0xBF, 0x51, sizeof(pHelper));
					pHelper.IsZen = 1;
					pHelper.Zen = ReadConfig.OfficialHelper_Zen;
					DataSend(lpObj->m_Index, (LPBYTE)&pHelper, sizeof(pHelper));

					lpObj->Money -= ReadConfig.OfficialHelper_Zen;
					GCMoneySend(lpObj->m_Index,lpObj->Money);
				}else
				{
					gObjOfficialHelperStatus(lpObj,2);
				}
			}
			
			lpObj->OfficialHelper_TimeCount++;
		}
	}
}

void gObjSecondDurDown(LPOBJ lpObj)
{
	lpObj->m_TimeCount +=1;
	lpObj->m_TimeCount2 +=1;

	int ret = 0;
	int reCalCharacter = 0;

	if(lpObj->m_Change >= 0)
	{
		gObjChangeDurProc(lpObj);
	}

	BYTE attr = MapC[lpObj->MapNumber].GetAttr(lpObj->X,lpObj->Y);

	if(lpObj->m_TimeCount2 == 2400)//DESTROY LUCKY ITEMS
	{
		for(int i=2;i<7;i++) //Helm to Boots
		//for(int i = 0; i < ReadConfig.INVENTORY_SIZE(lpObj->m_Index,false); i++) //All Inventory
		{
			if(IsLuckyItem(lpObj->pInventory[i].m_Type) == true)
			{
				lpObj->pInventory[i].m_Durability -= 1.0f ;				
				if(lpObj->pInventory[i].m_Durability > 0)
				{					
					GCItemDurSend2(lpObj->m_Index,i,lpObj->pInventory[i].m_Durability,0);
				}else
				{				
					DestroyLuckyItem(lpObj->m_Index,i);
				}
			}
		}

		lpObj->m_TimeCount2 = 0;
	}
	
	if(lpObj->m_TimeCount % 10 != 0)
	{
		return;
	}

#if(ENABLE_CSHOP == 1)
	if(MuItemShop.DeleteItem(lpObj->m_Index) == true)
		reCalCharacter = 1;
#endif

	if(lpObj->pInventory[7].IsItem() == 1 )
	{
		ret = lpObj->pInventory[7].DurabilityDown(1,lpObj->m_Index);

		if(ret != 0)
		{
			GCItemDurSend2(lpObj->m_Index,7,lpObj->pInventory[7].m_Durability,0);
		}

		//Small wings dont show durability when not bough from NPC
		//Will create separate config for them later for NON-WZ Mushop creations
		/*if (lpObj->pInventory[7].m_Type >= ITEMGET(12,130) && lpObj->pInventory[7].m_Type <= ITEMGET(12,135) && lpObj->pInventory[7].m_Durability == 0)
		{
				GCServerMsgStringSend("Small wings Vanished!", lpObj->m_Index, 1);
				LogAddTD(lMsg.Get(MSGGET(2, 10)), lpObj->AccountID, lpObj->Name,
					lpObj->pInventory[7].GetName(), lpObj->pInventory[7].m_Level);
				gObjInventoryDeleteItem(lpObj->m_Index, 7);
				GCInventoryItemDeleteSend(lpObj->m_Index, 7, 0);
				gObjMakePreviewCharSet(lpObj->m_Index);
				GCEquipmentChange(lpObj->m_Index, 7);
		}*/

		if(lpObj->pInventory[7].m_Durability == 0)
		{
			if ((lpObj->pInventory[7].GetNumber() == 0xFFFFFFFF && ReadConfig.DeleteFFFFFFFFSerial == TRUE) ||
				(lpObj->pInventory[7].GetNumber() == 0xFFFFFFFE && ReadConfig.DeleteFFFFFFFESerial == TRUE) ||
				(lpObj->pInventory[7].GetNumber() == 0xFFFFFFFD && ReadConfig.DeleteFFFFFFFDSerial == TRUE) ||
				(lpObj->pInventory[7].GetNumber() == 0xFFFFFFFC && ReadConfig.DeleteFFFFFFFCSerial == TRUE) )
			{
				GCServerMsgStringSend("Promo item has Vanished!", lpObj->m_Index, 1);
				LogAddTD(lMsg.Get(MSGGET(2, 10)), lpObj->AccountID, lpObj->Name,
					lpObj->pInventory[7].GetName(), lpObj->pInventory[7].m_Level);
				gObjInventoryDeleteItem(lpObj->m_Index, 7);
				GCInventoryItemDeleteSend(lpObj->m_Index, 7, 0);
				gObjMakePreviewCharSet(lpObj->m_Index);
				GCEquipmentChange(lpObj->m_Index, 7);
			}
		}

		if(ret == 2)
		{
			reCalCharacter = 1;
		}
	}

	if((attr&1)==0)
	{
		if(lpObj->pInventory[10].IsItem() == 1)
		{
			if((lpObj->pInventory[10].m_Type == ITEMGET(13,20) && lpObj->pInventory[10].m_Level == 0) ||
				lpObj->pInventory[10].m_Type == ITEMGET(13,107))
				
			{
				ret = lpObj->pInventory[10].DurabilityDown(70,lpObj->m_Index,10);
				if(ret != 0)
				{
					GCItemDurSend2(lpObj->m_Index,10,lpObj->pInventory[10].m_Durability,0);
				}
				if(ret == 2)
				{
					reCalCharacter = 1;
				}
			}
			else if(lpObj->pInventory[10].m_Type == ITEMGET(13,38))
			{
				ret = lpObj->pInventory[10].DurabilityDown(63,lpObj->m_Index,10);
				if(ret != 0)
				{
					GCItemDurSend2(lpObj->m_Index,10,lpObj->pInventory[10].m_Durability,0);
				}
			}
			else if(lpObj->pInventory[10].m_Type != ITEMGET(13,10))
			{
				ret = lpObj->pInventory[10].DurabilityDown(1,lpObj->m_Index,10);
				if(ret != 0)
				{
					GCItemDurSend2(lpObj->m_Index,10,lpObj->pInventory[10].m_Durability,0);
				}

				if(ret == 2)
				{
					reCalCharacter = 1;
				}
			}

			if(lpObj->pInventory[10].m_Durability == 0)
			{
				if ((lpObj->pInventory[10].GetNumber() == 0xFFFFFFFF && ReadConfig.DeleteFFFFFFFFSerial == TRUE) ||
					(lpObj->pInventory[10].GetNumber() == 0xFFFFFFFE && ReadConfig.DeleteFFFFFFFESerial == TRUE) ||
					(lpObj->pInventory[10].GetNumber() == 0xFFFFFFFD && ReadConfig.DeleteFFFFFFFDSerial == TRUE) ||
					(lpObj->pInventory[10].GetNumber() == 0xFFFFFFFC && ReadConfig.DeleteFFFFFFFCSerial == TRUE) )
				{
					GCServerMsgStringSend("Promo item has Vanished!", lpObj->m_Index, 1);
					LogAddTD(lMsg.Get(MSGGET(2, 10)), lpObj->AccountID, lpObj->Name,
						lpObj->pInventory[10].GetName(), lpObj->pInventory[10].m_Level);
					gObjInventoryDeleteItem(lpObj->m_Index, 10);
					GCInventoryItemDeleteSend(lpObj->m_Index, 10, 0);
					gObjMakePreviewCharSet(lpObj->m_Index);
					GCEquipmentChange(lpObj->m_Index,10);
				}
			}
		}

		if(lpObj->pInventory[11].IsItem() == 1)
		{
			if((lpObj->pInventory[11].m_Type == ITEMGET(13,20) && lpObj->pInventory[11].m_Level == 0) ||
				lpObj->pInventory[11].m_Type == ITEMGET(13,107))
			{
				ret = lpObj->pInventory[11].DurabilityDown(70,lpObj->m_Index,11);
				if(ret != 0)
				{
					GCItemDurSend2(lpObj->m_Index,11,lpObj->pInventory[11].m_Durability,0);
				}

				if(ret == 2)
				{
					reCalCharacter = 1;
				}
			}
			else if(lpObj->pInventory[11].m_Type == ITEMGET(13,38))
			{
				ret = lpObj->pInventory[11].DurabilityDown(63,lpObj->m_Index,11);
				if(ret != 0)
				{
					GCItemDurSend2(lpObj->m_Index,11,lpObj->pInventory[11].m_Durability,0);
				}
			}
			else if(lpObj->pInventory[11].m_Type != ITEMGET(13,10))
			{
				ret = lpObj->pInventory[11].DurabilityDown(1,lpObj->m_Index,11);
				if(ret != 0)
				{
					GCItemDurSend2(lpObj->m_Index,11,lpObj->pInventory[11].m_Durability,0);
				}

				if(ret == 2)
				{
					reCalCharacter = 1;
				}
			}

			if(lpObj->pInventory[11].m_Durability == 0)
			{
				if ((lpObj->pInventory[11].GetNumber() == 0xFFFFFFFF && ReadConfig.DeleteFFFFFFFFSerial == TRUE) ||
					(lpObj->pInventory[11].GetNumber() == 0xFFFFFFFE && ReadConfig.DeleteFFFFFFFESerial == TRUE) ||
					(lpObj->pInventory[11].GetNumber() == 0xFFFFFFFD && ReadConfig.DeleteFFFFFFFDSerial == TRUE) ||
					(lpObj->pInventory[11].GetNumber() == 0xFFFFFFFC && ReadConfig.DeleteFFFFFFFCSerial == TRUE) )
				{
					GCServerMsgStringSend("Promo item has Vanished!", lpObj->m_Index, 1);
					LogAddTD(lMsg.Get(MSGGET(2, 10)), lpObj->AccountID, lpObj->Name,
						lpObj->pInventory[11].GetName(), lpObj->pInventory[11].m_Level);
					gObjInventoryDeleteItem(lpObj->m_Index, 11);
					GCInventoryItemDeleteSend(lpObj->m_Index, 11, 0);
					gObjMakePreviewCharSet(lpObj->m_Index);
					GCEquipmentChange(lpObj->m_Index,11);
				}
			}
		}

		if(lpObj->pInventory[9].IsItem() == 1)
		{
			if(lpObj->pInventory[9].m_Type != ITEMGET(13,10))
			{
				ret = lpObj->pInventory[9].DurabilityDown(1,lpObj->m_Index,9);

				if(ret != 0)
				{
					GCItemDurSend2(lpObj->m_Index,9,lpObj->pInventory[9].m_Durability,0);
				}

				if(ret == 2)
				{
					reCalCharacter = 1;
				}
			}

			if(lpObj->pInventory[9].m_Durability == 0)
			{
				if ((lpObj->pInventory[9].GetNumber() == 0xFFFFFFFF && ReadConfig.DeleteFFFFFFFFSerial == TRUE) ||
					(lpObj->pInventory[9].GetNumber() == 0xFFFFFFFE && ReadConfig.DeleteFFFFFFFESerial == TRUE) ||
					(lpObj->pInventory[9].GetNumber() == 0xFFFFFFFD && ReadConfig.DeleteFFFFFFFDSerial == TRUE) ||
					(lpObj->pInventory[9].GetNumber() == 0xFFFFFFFC && ReadConfig.DeleteFFFFFFFCSerial == TRUE) )
				{
					GCServerMsgStringSend("Promo item has Vanished!", lpObj->m_Index, 1);
					LogAddTD(lMsg.Get(MSGGET(2, 10)), lpObj->AccountID, lpObj->Name,
						lpObj->pInventory[9].GetName(), lpObj->pInventory[9].m_Level);
					gObjInventoryDeleteItem(lpObj->m_Index, 9);
					GCInventoryItemDeleteSend(lpObj->m_Index, 9, 0);
					gObjMakePreviewCharSet(lpObj->m_Index);
					GCEquipmentChange(lpObj->m_Index,9);
				}
			}
		}
	}

	if(reCalCharacter != 0)
	{
		gObjCalCharacter(lpObj->m_Index);
	}
}






void gObjChangeDurProc(LPOBJ lpObj)
{

	if(lpObj->pInventory[10].IsItem() == 1 && lpObj->pInventory[10].m_Type == ITEMGET(13,10))
	{
		int m_Durability = lpObj->pInventory[10].m_Durability;

		lpObj->pInventory[10].m_Durability -= (float)0.02;

		if(m_Durability > (int)lpObj->pInventory[10].m_Durability)
		{
			if(lpObj->pInventory[10].m_Durability < 1)
			{
				lpObj->pInventory[10].m_Durability = 0;
			}

			BYTE dur = lpObj->pInventory[10].m_Durability;

			GCItemDurSend(lpObj->m_Index,10,dur,0);

			if(dur == 0)
			{
				gObjUseSkill.SkillChangeUse(lpObj->m_Index);
				LogAddTD(lMsg.Get(544),lpObj->AccountID,lpObj->Name,lpObj->pInventory[10].GetName(),lpObj->pInventory[10].m_Level,dur);
				lpObj->pInventory[10].Clear();
				GCInventoryItemDeleteSend(lpObj->m_Index,10,0);
			}
		}
	}
	else if(lpObj->pInventory[11].IsItem() == 1 && lpObj->pInventory[11].m_Type == ITEMGET(13,10))
	{
		int m_Durability = lpObj->pInventory[11].m_Durability;

		lpObj->pInventory[11].m_Durability -= (float)0.02;

		if(m_Durability > (int)lpObj->pInventory[11].m_Durability)
		{
			if(lpObj->pInventory[11].m_Durability < 1)
			{
				lpObj->pInventory[11].m_Durability = 0;
			}

			BYTE dur = lpObj->pInventory[11].m_Durability;

			GCItemDurSend(lpObj->m_Index,11,dur,0);

			if(dur == 0)
			{
				gObjUseSkill.SkillChangeUse(lpObj->m_Index);
				LogAddTD(lMsg.Get(544),lpObj->AccountID,lpObj->Name,lpObj->pInventory[10].GetName(),lpObj->pInventory[11].m_Level,dur);
				lpObj->pInventory[11].Clear();
				GCInventoryItemDeleteSend(lpObj->m_Index,11,0);
			}
		}
	}
	else if(lpObj->pInventory[10].IsItem() == 1 && lpObj->pInventory[10].m_Type == ITEMGET(13,39))
	{
		int m_Durability = lpObj->pInventory[10].m_Durability;

		lpObj->pInventory[10].m_Durability -= (float)0.02;

		if(m_Durability > (int)lpObj->pInventory[10].m_Durability)
		{
			if(lpObj->pInventory[10].m_Durability < 1)
			{
				lpObj->pInventory[10].m_Durability = 0;
			}

			BYTE dur = lpObj->pInventory[10].m_Durability;

			GCItemDurSend(lpObj->m_Index,10,dur,0);

			if(dur == 0)
			{
				gObjUseSkill.SkillChangeUse(lpObj->m_Index);
				LogAddTD(lMsg.Get(544),lpObj->AccountID,lpObj->Name,lpObj->pInventory[10].GetName(),lpObj->pInventory[10].m_Level,dur);
				lpObj->pInventory[10].Clear();
				GCInventoryItemDeleteSend(lpObj->m_Index,10,0);
			}
		}
	}
	else if(lpObj->pInventory[11].IsItem() == 1 && lpObj->pInventory[11].m_Type == ITEMGET(13,39))
	{
		int m_Durability = lpObj->pInventory[11].m_Durability;

		lpObj->pInventory[11].m_Durability -= (float)0.02;

		if(m_Durability > (int)lpObj->pInventory[11].m_Durability)
		{
			if(lpObj->pInventory[11].m_Durability < 1)
			{
				lpObj->pInventory[11].m_Durability = 0;
			}

			BYTE dur = lpObj->pInventory[11].m_Durability;

			GCItemDurSend(lpObj->m_Index,11,dur,0);

			if(dur == 0)
			{
				gObjUseSkill.SkillChangeUse(lpObj->m_Index);
				LogAddTD(lMsg.Get(544),lpObj->AccountID,lpObj->Name,lpObj->pInventory[10].GetName(),lpObj->pInventory[11].m_Level,dur);
				lpObj->pInventory[11].Clear();
				GCInventoryItemDeleteSend(lpObj->m_Index,11,0);
			}
		}
	}
	else if(lpObj->pInventory[10].IsItem() == 1 && lpObj->pInventory[10].m_Type == ITEMGET(13,68))
	{
		int m_Durability = lpObj->pInventory[10].m_Durability;

		lpObj->pInventory[10].m_Durability -= (float)0.02;

		if(m_Durability > (int)lpObj->pInventory[10].m_Durability)
		{
			if(lpObj->pInventory[10].m_Durability < 1)
			{
				lpObj->pInventory[10].m_Durability = 0;
			}

			BYTE dur = lpObj->pInventory[10].m_Durability;

			GCItemDurSend(lpObj->m_Index,10,dur,0);

			if(dur == 0)
			{
				gObjUseSkill.SkillChangeUse(lpObj->m_Index);
				LogAddTD(lMsg.Get(544),lpObj->AccountID,lpObj->Name,lpObj->pInventory[10].GetName(),lpObj->pInventory[10].m_Level,dur);
				lpObj->pInventory[10].Clear();
				GCInventoryItemDeleteSend(lpObj->m_Index,10,0);
			}
		}
	}
	else if(lpObj->pInventory[11].IsItem() == 1 && lpObj->pInventory[11].m_Type == ITEMGET(13,68))
	{
		int m_Durability = lpObj->pInventory[11].m_Durability;

		lpObj->pInventory[11].m_Durability -= (float)0.02;

		if(m_Durability > (int)lpObj->pInventory[11].m_Durability)
		{
			if(lpObj->pInventory[11].m_Durability < 1)
			{
				lpObj->pInventory[11].m_Durability = 0;
			}

			BYTE dur = lpObj->pInventory[11].m_Durability;

			GCItemDurSend(lpObj->m_Index,11,dur,0);

			if(dur == 0)
			{
				gObjUseSkill.SkillChangeUse(lpObj->m_Index);
				LogAddTD(lMsg.Get(544),lpObj->AccountID,lpObj->Name,lpObj->pInventory[10].GetName(),lpObj->pInventory[11].m_Level,dur);
				lpObj->pInventory[11].Clear();
				GCInventoryItemDeleteSend(lpObj->m_Index,11,0);
			}
		}
	}
	else if(lpObj->pInventory[10].IsItem() == 1 && lpObj->pInventory[10].m_Type == ITEMGET(13,76))
	{
		int m_Durability = lpObj->pInventory[10].m_Durability;

		lpObj->pInventory[10].m_Durability -= (float)0.02;

		if(m_Durability > (int)lpObj->pInventory[10].m_Durability)
		{
			if(lpObj->pInventory[10].m_Durability < 1)
			{
				lpObj->pInventory[10].m_Durability = 0;
			}

			BYTE dur = lpObj->pInventory[10].m_Durability;

			GCItemDurSend(lpObj->m_Index,10,dur,0);

			if(dur == 0)
			{
				gObjUseSkill.SkillChangeUse(lpObj->m_Index);
				LogAddTD(lMsg.Get(544),lpObj->AccountID,lpObj->Name,lpObj->pInventory[10].GetName(),lpObj->pInventory[10].m_Level,dur);
				lpObj->pInventory[10].Clear();
				GCInventoryItemDeleteSend(lpObj->m_Index,10,0);
			}
		}
	}
	else if(lpObj->pInventory[10].IsItem() == 1 && lpObj->pInventory[10].m_Type == ITEMGET(13,122))
	{
		int m_Durability = lpObj->pInventory[10].m_Durability;

		lpObj->pInventory[10].m_Durability -= (float)0.02;

		if(m_Durability > (int)lpObj->pInventory[10].m_Durability)
		{
			if(lpObj->pInventory[10].m_Durability < 1)
			{
				lpObj->pInventory[10].m_Durability = 0;
			}

			BYTE dur = lpObj->pInventory[10].m_Durability;

			GCItemDurSend(lpObj->m_Index,10,dur,0);

			if(dur == 0)
			{
				gObjUseSkill.SkillChangeUse(lpObj->m_Index);
				LogAddTD(lMsg.Get(544),lpObj->AccountID,lpObj->Name,lpObj->pInventory[10].GetName(),lpObj->pInventory[10].m_Level,dur);
				lpObj->pInventory[10].Clear();
				GCInventoryItemDeleteSend(lpObj->m_Index,10,0);
			}
		}
	}
	else if(lpObj->pInventory[11].IsItem() == 1 && lpObj->pInventory[11].m_Type == ITEMGET(13,76))
	{
		int m_Durability = lpObj->pInventory[11].m_Durability;

		lpObj->pInventory[11].m_Durability -= (float)0.02;

		if(m_Durability > (int)lpObj->pInventory[11].m_Durability)
		{
			if(lpObj->pInventory[11].m_Durability < 1)
			{
				lpObj->pInventory[11].m_Durability = 0;
			}

			BYTE dur = lpObj->pInventory[11].m_Durability;

			GCItemDurSend(lpObj->m_Index,11,dur,0);

			if(dur == 0)
			{
				gObjUseSkill.SkillChangeUse(lpObj->m_Index);
				LogAddTD(lMsg.Get(544),lpObj->AccountID,lpObj->Name,lpObj->pInventory[10].GetName(),lpObj->pInventory[11].m_Level,dur);
				lpObj->pInventory[11].Clear();
				GCInventoryItemDeleteSend(lpObj->m_Index,11,0);
			}
		}
	}
	else if(lpObj->pInventory[11].IsItem() == 1 && lpObj->pInventory[11].m_Type == ITEMGET(13,122))
	{
		int m_Durability = lpObj->pInventory[11].m_Durability;

		lpObj->pInventory[11].m_Durability -= (float)0.02;

		if(m_Durability > (int)lpObj->pInventory[11].m_Durability)
		{
			if(lpObj->pInventory[11].m_Durability < 1)
			{
				lpObj->pInventory[11].m_Durability = 0;
			}

			BYTE dur = lpObj->pInventory[11].m_Durability;

			GCItemDurSend(lpObj->m_Index,11,dur,0);

			if(dur == 0)
			{
				gObjUseSkill.SkillChangeUse(lpObj->m_Index);
				LogAddTD(lMsg.Get(544),lpObj->AccountID,lpObj->Name,lpObj->pInventory[10].GetName(),lpObj->pInventory[11].m_Level,dur);
				lpObj->pInventory[11].Clear();
				GCInventoryItemDeleteSend(lpObj->m_Index,11,0);
			}
		}
	}
	else if(lpObj->pInventory[11].IsItem() == 1 && lpObj->pInventory[11].m_Type == ITEMGET(13,40))
	{
		int m_Durability = lpObj->pInventory[11].m_Durability;

		lpObj->pInventory[11].m_Durability -= (float)0.02;

		if(m_Durability > (int)lpObj->pInventory[11].m_Durability)
		{
			if(lpObj->pInventory[11].m_Durability < 1)
			{
				lpObj->pInventory[11].m_Durability = 0;
			}

			BYTE dur = lpObj->pInventory[11].m_Durability;

			GCItemDurSend(lpObj->m_Index,11,dur,0);

			if(dur == 0)
			{
				gObjUseSkill.SkillChangeUse(lpObj->m_Index);
				LogAddTD(lMsg.Get(544),lpObj->AccountID,lpObj->Name,lpObj->pInventory[10].GetName(),lpObj->pInventory[11].m_Level,dur);
				lpObj->pInventory[11].Clear();
				GCInventoryItemDeleteSend(lpObj->m_Index,11,0);
			}
		}
	}
	else if(lpObj->pInventory[10].IsItem() == 1 && lpObj->pInventory[10].m_Type == ITEMGET(13,40))
	{
		int m_Durability = lpObj->pInventory[10].m_Durability;

		lpObj->pInventory[10].m_Durability -= (float)0.02;

		if(m_Durability > (int)lpObj->pInventory[10].m_Durability)
		{
			if(lpObj->pInventory[10].m_Durability < 1)
			{
				lpObj->pInventory[10].m_Durability = 0;
			}

			BYTE dur = lpObj->pInventory[10].m_Durability;

			GCItemDurSend(lpObj->m_Index,10,dur,0);

			if(dur == 0)
			{
				gObjUseSkill.SkillChangeUse(lpObj->m_Index);
				LogAddTD(lMsg.Get(544),lpObj->AccountID,lpObj->Name,lpObj->pInventory[10].GetName(),lpObj->pInventory[10].m_Level,dur);
				lpObj->pInventory[10].Clear();
				GCInventoryItemDeleteSend(lpObj->m_Index,10,0);
			}
		}
	}
	else if(lpObj->pInventory[11].IsItem() == 1 && lpObj->pInventory[11].m_Type == ITEMGET(13,41))
	{
		int m_Durability = lpObj->pInventory[11].m_Durability;

		lpObj->pInventory[11].m_Durability -= (float)0.02;

		if(m_Durability > (int)lpObj->pInventory[11].m_Durability)
		{
			if(lpObj->pInventory[11].m_Durability < 1)
			{
				lpObj->pInventory[11].m_Durability = 0;
			}

			BYTE dur = lpObj->pInventory[11].m_Durability;

			GCItemDurSend(lpObj->m_Index,11,dur,0);

			if(dur == 0)
			{
				gObjUseSkill.SkillChangeUse(lpObj->m_Index);
				LogAddTD(lMsg.Get(544),lpObj->AccountID,lpObj->Name,lpObj->pInventory[10].GetName(),lpObj->pInventory[11].m_Level,dur);
				lpObj->pInventory[11].Clear();
				GCInventoryItemDeleteSend(lpObj->m_Index,11,0);
			}
		}
	}
	else if(lpObj->pInventory[10].IsItem() == 1 && lpObj->pInventory[10].m_Type == ITEMGET(13,41))
	{
		int m_Durability = lpObj->pInventory[10].m_Durability;

		lpObj->pInventory[10].m_Durability -= (float)0.02;

		if(m_Durability > (int)lpObj->pInventory[10].m_Durability)
		{
			if(lpObj->pInventory[10].m_Durability < 1)
			{
				lpObj->pInventory[10].m_Durability = 0;
			}

			BYTE dur = lpObj->pInventory[10].m_Durability;

			GCItemDurSend(lpObj->m_Index,10,dur,0);

			if(dur == 0)
			{
				gObjUseSkill.SkillChangeUse(lpObj->m_Index);
				LogAddTD(lMsg.Get(544),lpObj->AccountID,lpObj->Name,lpObj->pInventory[10].GetName(),lpObj->pInventory[10].m_Level,dur);
				lpObj->pInventory[10].Clear();
				GCInventoryItemDeleteSend(lpObj->m_Index,10,0);
			}
		}
	}
}




#pragma warning ( disable : 4101 )

void gObjWingDurProc(LPOBJ lpObj) 
{
	BYTE send_dur=0; // HERE GOES A MACRO
	{
		int dur;
	}
}

#pragma warning ( default : 4101 )




void gObjPenaltyDurDown(LPOBJ lpObj, LPOBJ lpTargetObj)	// Functio n Not USed
{
	int ret = 0;
	int reCalCharacter = 0;

	int irand = rand()%2;

	CItem * Weapon = &lpObj->pInventory[irand];
	int decdur = 1;

#if(ENABLE_CSHOP == 1)
	if(MuItemShop.IsCShopItem(lpObj->m_Index,irand) == true)
		return;
#endif

	switch(lpObj->Class)
	{
	case 0:
		decdur = lpObj->Level - (lpTargetObj->Level * 38) / 20;
		break;
	case 1:
		decdur = lpObj->Level - (lpTargetObj->Level * 37) / 20;
		break;
	case 2:
		decdur = lpObj->Level - (lpTargetObj->Level * 36) / 20;
		break;
	case 3:
	case 4:
		decdur = lpObj->Level - (lpTargetObj->Level * 37) / 20;
		break;
	case 5:
		decdur = lpObj->Level - (lpTargetObj->Level * 38) / 20;
		break;
	default : break;
	}

	decdur /= 10;

	if(Weapon->m_Type != ITEMGET(4,7) && Weapon->m_Type != ITEMGET(4,15))
	{
		if(Weapon->m_Type < ITEMGET(6,0) && Weapon->m_Type >= ITEMGET(7,0) )
		{
			if ( Weapon->IsItem() )
			{
				ret = Weapon->DurabilityDown2(decdur,lpObj->m_Index);
			}
		}
		else
		{
			CItem * Weapon = &lpObj->pInventory[irand];

			if(Weapon->IsItem())
			{
				ret = Weapon->DurabilityDown2(decdur,lpObj->m_Index);
			}
		}
	}

	if(ret != 0)
	{
		GCItemDurSend2(lpObj->m_Index,irand,lpObj->pInventory[irand].m_Durability,0);
	}

	if(lpObj->pInventory[irand].m_Durability == 0)
	{
		if ((lpObj->pInventory[irand].GetNumber() == 0xFFFFFFFF && ReadConfig.DeleteFFFFFFFFSerial == TRUE) ||
			(lpObj->pInventory[irand].GetNumber() == 0xFFFFFFFE && ReadConfig.DeleteFFFFFFFESerial == TRUE) ||
			(lpObj->pInventory[irand].GetNumber() == 0xFFFFFFFD && ReadConfig.DeleteFFFFFFFDSerial == TRUE) ||
			(lpObj->pInventory[irand].GetNumber() == 0xFFFFFFFC && ReadConfig.DeleteFFFFFFFCSerial == TRUE) )
		{
			GCServerMsgStringSend("Promo item has Vanished!", lpObj->m_Index, 1);
			LogAddTD(lMsg.Get(MSGGET(2, 10)), lpObj->AccountID, lpObj->Name,
				lpObj->pInventory[irand].GetName(), lpObj->pInventory[irand].m_Level);
			gObjInventoryDeleteItem(lpObj->m_Index, irand);
			GCInventoryItemDeleteSend(lpObj->m_Index, irand, 0);
			gObjMakePreviewCharSet(lpObj->m_Index);
			GCEquipmentChange(lpObj->m_Index,irand);
		}
	}
	if(IsLuckyItem(lpObj->pInventory[irand].m_Type) == true && lpObj->pInventory[irand].m_Durability == 0)
	{
		DestroyLuckyItem(lpObj->m_Index,irand);
	}

	if(ret == 2)
	{
		reCalCharacter = 1;
	}

	int item_num[7] = {0,1,2,3,4,5,6};
	int item_pos = rand()%7;

	CItem * DurItem = &lpObj->pInventory[item_num[item_pos]];

	if(DurItem->m_Type != ITEMGET(4,7) && DurItem->m_Type != ITEMGET(4,15))
	{
		if(item_pos < 2)
		{
			if(DurItem->m_Type >= ITEMGET(6,0) && DurItem->m_Type < ITEMGET(7,0))
			{
				ret = DurItem->DurabilityDown2(decdur,lpObj->m_Index);
	
				if(ret != 0)
				{
					GCItemDurSend2(lpObj->m_Index,item_num[item_pos],DurItem->m_Durability,0);
	
					if(ret == 2)
					{
						reCalCharacter = 1;
					}
				}
			}
		}
		else
		{
			ret = DurItem->DurabilityDown2(decdur,lpObj->m_Index);

			if(ret != 0)
			{
				GCItemDurSend2(lpObj->m_Index,item_num[item_pos],DurItem->m_Durability,0);

				if(ret == 2)
				{
					reCalCharacter = 1;
				}
			}
		}

		if(lpObj->pInventory[item_num[item_pos]].m_Durability == 0)
		{
			if ((lpObj->pInventory[item_num[item_pos]].GetNumber() == 0xFFFFFFFF && ReadConfig.DeleteFFFFFFFFSerial == TRUE) ||
				(lpObj->pInventory[item_num[item_pos]].GetNumber() == 0xFFFFFFFE && ReadConfig.DeleteFFFFFFFESerial == TRUE) ||
				(lpObj->pInventory[item_num[item_pos]].GetNumber() == 0xFFFFFFFD && ReadConfig.DeleteFFFFFFFDSerial == TRUE) ||
				(lpObj->pInventory[item_num[item_pos]].GetNumber() == 0xFFFFFFFC && ReadConfig.DeleteFFFFFFFCSerial == TRUE) )
			{
				GCServerMsgStringSend("Promo item has Vanished!", lpObj->m_Index, 1);
				LogAddTD(lMsg.Get(MSGGET(2, 10)), lpObj->AccountID, lpObj->Name,
					lpObj->pInventory[item_num[item_pos]].GetName(), lpObj->pInventory[item_num[item_pos]].m_Level);
				gObjInventoryDeleteItem(lpObj->m_Index, item_num[item_pos]);
				GCInventoryItemDeleteSend(lpObj->m_Index, item_num[item_pos], 0);
				gObjMakePreviewCharSet(lpObj->m_Index);
				GCEquipmentChange(lpObj->m_Index,item_num[item_pos]);
			}
		}
		if(IsLuckyItem(lpObj->pInventory[item_num[item_pos]].m_Type) == true && lpObj->pInventory[item_num[item_pos]].m_Durability == 0)
		{
			DestroyLuckyItem(lpObj->m_Index,item_num[item_pos]);
		}
	}

	CItem * Wing = &lpObj->pInventory[7];

	if(Wing->IsItem())
	{
		ret = DurItem->DurabilityDown2(decdur,lpObj->m_Index);

		if(ret != 0)
		{
			GCItemDurSend2(lpObj->m_Index,item_num[item_pos],DurItem->m_Durability,0);

			if(ret == 2)
			{
				reCalCharacter = 1;
			}
		}

		if(lpObj->pInventory[7].m_Durability == 0)
		{
			if ((lpObj->pInventory[7].GetNumber() == 0xFFFFFFFF && ReadConfig.DeleteFFFFFFFFSerial == TRUE) ||
				(lpObj->pInventory[7].GetNumber() == 0xFFFFFFFE && ReadConfig.DeleteFFFFFFFESerial == TRUE) ||
				(lpObj->pInventory[7].GetNumber() == 0xFFFFFFFD && ReadConfig.DeleteFFFFFFFDSerial == TRUE) ||
				(lpObj->pInventory[7].GetNumber() == 0xFFFFFFFC && ReadConfig.DeleteFFFFFFFCSerial == TRUE) )
			{
				GCServerMsgStringSend("Promo item has Vanished!", lpObj->m_Index, 1);
				LogAddTD(lMsg.Get(MSGGET(2, 10)), lpObj->AccountID, lpObj->Name,
					lpObj->pInventory[7].GetName(), lpObj->pInventory[7].m_Level);
				gObjInventoryDeleteItem(lpObj->m_Index, 7);
				GCInventoryItemDeleteSend(lpObj->m_Index, 7, 0);
				gObjMakePreviewCharSet(lpObj->m_Index);
				GCEquipmentChange(lpObj->m_Index,7);
			}
		}
	}

	if(reCalCharacter != 0)
	{
		gObjCalCharacter(lpObj->m_Index);
	}
}






void gObjWeaponDurDown(LPOBJ lpObj, LPOBJ lpTargetObj, int type)
{
	if(lpObj->Type == OBJ_MONSTER || lpObj->Type == OBJ_NPC)
	{
		return;
	}

	int itargetdefence = 0;
	CItem * Right = &lpObj->pInventory[0];
	CItem * Left = &lpObj->pInventory[1];
	int ret = 0;
				
#if(ENABLE_CSHOP == 1)
	if(MuItemShop.IsCShopItem(lpObj->m_Index,0) == true)
		return;
	if(MuItemShop.IsCShopItem(lpObj->m_Index,1) == true)
		return;
#endif

	if(type == 0)
	{
		itargetdefence = lpTargetObj->m_Defense;
		itargetdefence += lpTargetObj->m_iSkillNPCDefense;

		if((lpObj->Class == CLASS_KNIGHT || 
			lpObj->Class == CLASS_RAGEFIGHTER || 
			lpObj->Class == CLASS_MAGICGLADIATOR || 
			lpObj->Class == CLASS_DARKLORD)
			&& (Right->m_Type >= 0 && Right->m_Type < ITEMGET(4,0) 
				&& Left->m_Type >= 0 && Left->m_Type < ITEMGET(4,0)))
		{
			if(rand()%2==0)
			{
				ret = Left->NormalWeaponDurabilityDown(itargetdefence,lpObj->m_Index);
				
				if(ret != 0)
				{
					GCItemDurSend(lpObj->m_Index,1,Left->m_Durability,0);
				}

				if(lpObj->pInventory[1].m_Durability == 0)
				{
					if ((lpObj->pInventory[1].GetNumber() == 0xFFFFFFFF && ReadConfig.DeleteFFFFFFFFSerial == TRUE) ||
						(lpObj->pInventory[1].GetNumber() == 0xFFFFFFFE && ReadConfig.DeleteFFFFFFFESerial == TRUE) ||
						(lpObj->pInventory[1].GetNumber() == 0xFFFFFFFD && ReadConfig.DeleteFFFFFFFDSerial == TRUE) ||
						(lpObj->pInventory[1].GetNumber() == 0xFFFFFFFC && ReadConfig.DeleteFFFFFFFCSerial == TRUE) )
					{
						GCServerMsgStringSend("Promo item has Vanished!", lpObj->m_Index, 1);
						LogAddTD(lMsg.Get(MSGGET(2, 10)), lpObj->AccountID, lpObj->Name,
							lpObj->pInventory[1].GetName(), lpObj->pInventory[1].m_Level);
						gObjInventoryDeleteItem(lpObj->m_Index, 1);
						GCInventoryItemDeleteSend(lpObj->m_Index, 1, 0);
						gObjMakePreviewCharSet(lpObj->m_Index);
						GCEquipmentChange(lpObj->m_Index,1);
					}
				}

				if(ret == 2)
				{
					gObjCalCharacter(lpObj->m_Index);
				}
			}
			else
			{
				ret = Right->NormalWeaponDurabilityDown(itargetdefence,lpObj->m_Index);

				if(ret != 0)
				{
					GCItemDurSend(lpObj->m_Index,0,Right->m_Durability,0);
				}

				if(lpObj->pInventory[0].m_Durability == 0)
				{
					if ((lpObj->pInventory[0].GetNumber() == 0xFFFFFFFF && ReadConfig.DeleteFFFFFFFFSerial == TRUE) ||
						(lpObj->pInventory[0].GetNumber() == 0xFFFFFFFE && ReadConfig.DeleteFFFFFFFESerial == TRUE) ||
						(lpObj->pInventory[0].GetNumber() == 0xFFFFFFFD && ReadConfig.DeleteFFFFFFFDSerial == TRUE) ||
						(lpObj->pInventory[0].GetNumber() == 0xFFFFFFFC && ReadConfig.DeleteFFFFFFFCSerial == TRUE) )
					{
						GCServerMsgStringSend("Promo item has Vanished!", lpObj->m_Index, 1);
						LogAddTD(lMsg.Get(MSGGET(2, 10)), lpObj->AccountID, lpObj->Name,
							lpObj->pInventory[0].GetName(), lpObj->pInventory[0].m_Level);
						gObjInventoryDeleteItem(lpObj->m_Index, 0);
						GCInventoryItemDeleteSend(lpObj->m_Index, 0, 0);
						gObjMakePreviewCharSet(lpObj->m_Index);
						GCEquipmentChange(lpObj->m_Index,0);
					}
				}

				if(ret==2)
				{
					gObjCalCharacter(lpObj->m_Index);
				}
			}

			return;
		}
		else if(Left->m_Type >= ITEMGET(4,0) && Left->m_Type < ITEMGET(4,7) || Left->m_Type == ITEMGET(4,20) || Left->m_Type == ITEMGET(4,21) || Left->m_Type == ITEMGET(4,22) || Left->m_Type == ITEMGET(4,23))
		{
			ret = Left->BowWeaponDurabilityDown(itargetdefence,lpObj->m_Index);

			if(ret != 0)
			{
				GCItemDurSend(lpObj->m_Index,1,Left->m_Durability,0);
			}

			if(lpObj->pInventory[1].m_Durability == 0)
			{
				if ((lpObj->pInventory[1].GetNumber() == 0xFFFFFFFF && ReadConfig.DeleteFFFFFFFFSerial == TRUE) ||
					(lpObj->pInventory[1].GetNumber() == 0xFFFFFFFE && ReadConfig.DeleteFFFFFFFESerial == TRUE) ||
					(lpObj->pInventory[1].GetNumber() == 0xFFFFFFFD && ReadConfig.DeleteFFFFFFFDSerial == TRUE) ||
					(lpObj->pInventory[1].GetNumber() == 0xFFFFFFFC && ReadConfig.DeleteFFFFFFFCSerial == TRUE) )
				{
					GCServerMsgStringSend("Promo item has Vanished!", lpObj->m_Index, 1);
					LogAddTD(lMsg.Get(MSGGET(2, 10)), lpObj->AccountID, lpObj->Name,
						lpObj->pInventory[1].GetName(), lpObj->pInventory[1].m_Level);
					gObjInventoryDeleteItem(lpObj->m_Index, 1);
					GCInventoryItemDeleteSend(lpObj->m_Index, 1, 0);
					gObjMakePreviewCharSet(lpObj->m_Index);
					GCEquipmentChange(lpObj->m_Index,1);
				}
			}

			if(ret==2)
			{
				gObjCalCharacter(lpObj->m_Index);
			}
		}
		else if(Right->m_Type >= ITEMGET(4,8) && Right->m_Type < ITEMGET(4,15) || Right->m_Type >= ITEMGET(4,16) && Right->m_Type < ITEMGET(5,0))
		{
			ret = Right->BowWeaponDurabilityDown(itargetdefence,lpObj->m_Index);

			if(ret != 0)
			{
				GCItemDurSend(lpObj->m_Index,0,Right->m_Durability,0);
			}

			if(lpObj->pInventory[0].m_Durability == 0)
			{
				if ((lpObj->pInventory[0].GetNumber() == 0xFFFFFFFF && ReadConfig.DeleteFFFFFFFFSerial == TRUE) ||
					(lpObj->pInventory[0].GetNumber() == 0xFFFFFFFE && ReadConfig.DeleteFFFFFFFESerial == TRUE) ||
					(lpObj->pInventory[0].GetNumber() == 0xFFFFFFFD && ReadConfig.DeleteFFFFFFFDSerial == TRUE) ||
					(lpObj->pInventory[0].GetNumber() == 0xFFFFFFFC && ReadConfig.DeleteFFFFFFFCSerial == TRUE) )
				{
					GCServerMsgStringSend("Promo item has Vanished!", lpObj->m_Index, 1);
					LogAddTD(lMsg.Get(MSGGET(2, 10)), lpObj->AccountID, lpObj->Name,
						lpObj->pInventory[0].GetName(), lpObj->pInventory[0].m_Level);
					gObjInventoryDeleteItem(lpObj->m_Index, 0);
					GCInventoryItemDeleteSend(lpObj->m_Index, 0, 0);
					gObjMakePreviewCharSet(lpObj->m_Index);
					GCEquipmentChange(lpObj->m_Index,0);
				}
			}

			if(ret==2)
			{
				gObjCalCharacter(lpObj->m_Index);
			}
		}
		else if(Right->m_Type >= ITEMGET(0,0) && Right->m_Type < ITEMGET(4,0))
		{
			ret = Right->NormalWeaponDurabilityDown(itargetdefence,lpObj->m_Index);

			if(ret != 0)
			{
				GCItemDurSend(lpObj->m_Index,0,Right->m_Durability,0);
			}

			if(lpObj->pInventory[0].m_Durability == 0)
			{
				if ((lpObj->pInventory[0].GetNumber() == 0xFFFFFFFF && ReadConfig.DeleteFFFFFFFFSerial == TRUE) ||
					(lpObj->pInventory[0].GetNumber() == 0xFFFFFFFE && ReadConfig.DeleteFFFFFFFESerial == TRUE) ||
					(lpObj->pInventory[0].GetNumber() == 0xFFFFFFFD && ReadConfig.DeleteFFFFFFFDSerial == TRUE) ||
					(lpObj->pInventory[0].GetNumber() == 0xFFFFFFFC && ReadConfig.DeleteFFFFFFFCSerial == TRUE) )
				{
					GCServerMsgStringSend("Promo item has Vanished!", lpObj->m_Index, 1);
					LogAddTD(lMsg.Get(MSGGET(2, 10)), lpObj->AccountID, lpObj->Name,
						lpObj->pInventory[0].GetName(), lpObj->pInventory[0].m_Level);
					gObjInventoryDeleteItem(lpObj->m_Index, 0);
					GCInventoryItemDeleteSend(lpObj->m_Index, 0, 0);
					gObjMakePreviewCharSet(lpObj->m_Index);
					GCEquipmentChange(lpObj->m_Index,0);
				}
			}

			if(ret==2)
			{
				gObjCalCharacter(lpObj->m_Index);
			}
		}
		else if(Right->m_Type >= ITEMGET(5,0) && Right->m_Type < ITEMGET(6,0))
		{
			ret = Right->StaffWeaponDurabilityDown(itargetdefence,lpObj->m_Index);

			if(ret != 0)
			{
				GCItemDurSend(lpObj->m_Index,0,Right->m_Durability,0);
			}

			if(lpObj->pInventory[0].m_Durability == 0)
			{
				if ((lpObj->pInventory[0].GetNumber() == 0xFFFFFFFF && ReadConfig.DeleteFFFFFFFFSerial == TRUE) ||
					(lpObj->pInventory[0].GetNumber() == 0xFFFFFFFE && ReadConfig.DeleteFFFFFFFESerial == TRUE) ||
					(lpObj->pInventory[0].GetNumber() == 0xFFFFFFFD && ReadConfig.DeleteFFFFFFFDSerial == TRUE) ||
					(lpObj->pInventory[0].GetNumber() == 0xFFFFFFFC && ReadConfig.DeleteFFFFFFFCSerial == TRUE) )
				{
					GCServerMsgStringSend("Promo item has Vanished!", lpObj->m_Index, 1);
					LogAddTD(lMsg.Get(MSGGET(2, 10)), lpObj->AccountID, lpObj->Name,
						lpObj->pInventory[0].GetName(), lpObj->pInventory[0].m_Level);
					gObjInventoryDeleteItem(lpObj->m_Index, 0);
					GCInventoryItemDeleteSend(lpObj->m_Index, 0, 0);
					gObjMakePreviewCharSet(lpObj->m_Index);
					GCEquipmentChange(lpObj->m_Index,0);
				}
			}

			if(ret==2)
			{
				gObjCalCharacter(lpObj->m_Index);
			}
		}
	return;
	}


	if(lpTargetObj->Type == OBJ_MONSTER || lpTargetObj->Type == OBJ_NPC)
	{
		itargetdefence = lpTargetObj->m_MagicDefense;
	}
	else
	{
		itargetdefence = lpTargetObj->m_Defense;
	}

	itargetdefence += lpTargetObj->m_iSkillNPCDefense;

	if(Right->m_Type >= ITEMGET(5,0) && Right->m_Type < ITEMGET(6,0))
	{
		ret = Right->StaffWeaponDurabilityDown(itargetdefence,lpObj->m_Index);

		if(ret != 0)
		{
			GCItemDurSend(lpObj->m_Index,0,Right->m_Durability,0);
		}

		if(lpObj->pInventory[0].m_Durability == 0)
		{
			if ((lpObj->pInventory[0].GetNumber() == 0xFFFFFFFF && ReadConfig.DeleteFFFFFFFFSerial == TRUE) ||
				(lpObj->pInventory[0].GetNumber() == 0xFFFFFFFE && ReadConfig.DeleteFFFFFFFESerial == TRUE) ||
				(lpObj->pInventory[0].GetNumber() == 0xFFFFFFFD && ReadConfig.DeleteFFFFFFFDSerial == TRUE) ||
				(lpObj->pInventory[0].GetNumber() == 0xFFFFFFFC && ReadConfig.DeleteFFFFFFFCSerial == TRUE) )
			{
				GCServerMsgStringSend("Promo item has Vanished!", lpObj->m_Index, 1);
				LogAddTD(lMsg.Get(MSGGET(2, 10)), lpObj->AccountID, lpObj->Name,
					lpObj->pInventory[0].GetName(), lpObj->pInventory[0].m_Level);
				gObjInventoryDeleteItem(lpObj->m_Index, 0);
				GCInventoryItemDeleteSend(lpObj->m_Index, 0, 0);
				gObjMakePreviewCharSet(lpObj->m_Index);
				GCEquipmentChange(lpObj->m_Index,0);
			}
		}

		if(ret==2)
		{
			gObjCalCharacter(lpObj->m_Index);
		}
	}
}






void gObjArmorRandomDurDown(LPOBJ lpObj, LPOBJ lpAttackObj, bool Check)
{
	int item_num[7] = {0,1,2,3,4,5,6};
	int item_pos = rand()%7;

	if(lpObj->Type != OBJ_USER)
	{
		return;
	}

#if(ENABLE_CSHOP == 1)
	if(MuItemShop.IsCShopItem(lpObj->m_Index,item_pos) == true)
		return;
#endif

	CItem * DurItem = &lpObj->pInventory[item_num[item_pos]];
	int damagemin = lpAttackObj->m_AttackDamageMin;
	int ret;

	if(item_pos < 2)
	{
		if(DurItem->m_Type >= ITEMGET(6,0) && DurItem->m_Type < ITEMGET(7,0))
		{
			if(Check == true)
				ret = DurItem->ArmorDurabilityDown(damagemin,lpObj->m_Index);
			else
				ret = DurItem->ArmorDurabilityDownNoCheck(lpObj->m_Index);

			if(ret != 0)
			{
				GCItemDurSend(lpObj->m_Index,item_num[item_pos],DurItem->m_Durability,0);
			}

			if(lpObj->pInventory[item_num[item_pos]].m_Durability == 0)
			{
				if ((lpObj->pInventory[item_num[item_pos]].GetNumber() == 0xFFFFFFFF && ReadConfig.DeleteFFFFFFFFSerial == TRUE) ||
					(lpObj->pInventory[item_num[item_pos]].GetNumber() == 0xFFFFFFFE && ReadConfig.DeleteFFFFFFFESerial == TRUE) ||
					(lpObj->pInventory[item_num[item_pos]].GetNumber() == 0xFFFFFFFD && ReadConfig.DeleteFFFFFFFDSerial == TRUE) ||
					(lpObj->pInventory[item_num[item_pos]].GetNumber() == 0xFFFFFFFC && ReadConfig.DeleteFFFFFFFCSerial == TRUE) )
				{
					GCServerMsgStringSend("Promo item has Vanished!", lpObj->m_Index, 1);
					LogAddTD(lMsg.Get(MSGGET(2, 10)), lpObj->AccountID, lpObj->Name,
						lpObj->pInventory[item_num[item_pos]].GetName(), lpObj->pInventory[item_num[item_pos]].m_Level);
					gObjInventoryDeleteItem(lpObj->m_Index, item_num[item_pos]);
					GCInventoryItemDeleteSend(lpObj->m_Index, item_num[item_pos], 0);
					gObjMakePreviewCharSet(lpObj->m_Index);
					GCEquipmentChange(lpObj->m_Index,item_num[item_pos]);
				}
			}

			if(ret == 2)
			{
				gObjCalCharacter(lpObj->m_Index);
			}
			return;
		}
	}
	else
	{
		if(DurItem->m_Type != ITEMGET(4,7) && DurItem->m_Type != ITEMGET(4,15))
		{
			if(Check == true)
				ret = DurItem->ArmorDurabilityDown(damagemin,lpObj->m_Index);
			else
				ret = DurItem->ArmorDurabilityDownNoCheck(lpObj->m_Index);

			if(ret != 0)
			{
				GCItemDurSend(lpObj->m_Index,item_num[item_pos],DurItem->m_Durability,0);
			}

			if(lpObj->pInventory[item_num[item_pos]].m_Durability == 0)
			{
				if ((lpObj->pInventory[item_num[item_pos]].GetNumber() == 0xFFFFFFFF && ReadConfig.DeleteFFFFFFFFSerial == TRUE) ||
					(lpObj->pInventory[item_num[item_pos]].GetNumber() == 0xFFFFFFFE && ReadConfig.DeleteFFFFFFFESerial == TRUE) ||
					(lpObj->pInventory[item_num[item_pos]].GetNumber() == 0xFFFFFFFD && ReadConfig.DeleteFFFFFFFDSerial == TRUE) ||
					(lpObj->pInventory[item_num[item_pos]].GetNumber() == 0xFFFFFFFC && ReadConfig.DeleteFFFFFFFCSerial == TRUE) )
				{
					GCServerMsgStringSend("Promo item has Vanished!", lpObj->m_Index, 1);
					LogAddTD(lMsg.Get(MSGGET(2, 10)), lpObj->AccountID, lpObj->Name,
						lpObj->pInventory[item_num[item_pos]].GetName(), lpObj->pInventory[item_num[item_pos]].m_Level);
					gObjInventoryDeleteItem(lpObj->m_Index, item_num[item_pos]);
					GCInventoryItemDeleteSend(lpObj->m_Index, item_num[item_pos], 0);
					gObjMakePreviewCharSet(lpObj->m_Index);
					GCEquipmentChange(lpObj->m_Index,item_num[item_pos]);
				}
			}

			if(IsLuckyItem(lpObj->pInventory[item_num[item_pos]].m_Type) == true && lpObj->pInventory[item_num[item_pos]].m_Durability == 0)
			{
				DestroyLuckyItem(lpObj->m_Index,item_num[item_pos]);
			}

			if(ret == 2)
			{
				gObjCalCharacter(lpObj->m_Index);
			}
		}
	}
}






bool gObjIsSelfDefense(LPOBJ lpObj, int aTargetIndex)
{
	if ( !OBJMAX_RANGE(aTargetIndex) )
	{
		return false;
	}

	for ( int n=0;n<MAX_SELF_DEFENSE;n++)
	{
		if ( lpObj->SelfDefense[n] >= 0 )
		{
			if ( lpObj->SelfDefense[n] == aTargetIndex )
			{
				return true;
			}
		}
	}

	return false;
}



void gObjCheckSelfDefense(LPOBJ lpObj, int aTargetIndex)
{
	if(!OBJMAX_RANGE(aTargetIndex))
	{
		return;
	}

	if(gObj[aTargetIndex].m_PK_Level > 4)
	{
		return;
	}

	//FIX SELF DEFENCE FOR SIEGE
	if ( g_CastleSiegeSync.GetCastleState() == 7 )
	{
		if ( ( lpObj->MapNumber == MAP_INDEX_CASTLESIEGE ) && ( gObj[aTargetIndex].MapNumber == MAP_INDEX_CASTLESIEGE ) )
			return;
	}

	for(int n = 0; n < MAX_SELF_DEFENSE;n++)
	{
		if(gObj[aTargetIndex].SelfDefense[n] >= 0)
		{
			if(gObj[aTargetIndex].SelfDefense[n]==lpObj->m_Index)
			{
				return;
			}
		}
	}

	int blank = -1;

	for(int n = 0; n < MAX_SELF_DEFENSE;n++)
	{
		if(lpObj->SelfDefense[n] >= 0)
		{
			if(lpObj->SelfDefense[n] == aTargetIndex)
			{
				lpObj->SelfDefenseTime[n] = GetTickCount()+30000;
				return;
			}
		}
		else
		{
			blank = n;
		}
	}

	if(blank < 0)
	{
		LogAdd(lMsg.Get(523));
		return;
	}

	lpObj->MySelfDefenseTime = GetTickCount();
	lpObj->SelfDefense[blank] = aTargetIndex;
	lpObj->SelfDefenseTime[blank] = GetTickCount() + 30000;

	char szTemp[64];

	wsprintf(szTemp,lMsg.Get(1114),lpObj->Name,gObj[aTargetIndex].Name);

	GCServerMsgStringSend(szTemp,lpObj->m_Index,1);
	GCServerMsgStringSend(szTemp,aTargetIndex,1);

	LogAddTD("[%s][%s] Set SelfDefence [%s][%s]",lpObj->AccountID,lpObj->Name,gObj[aTargetIndex].AccountID,gObj[aTargetIndex].Name);
}






void gObjTimeCheckSelfDefense(LPOBJ lpObj)
{
	char szTemp[64];

	for ( int n=0;n<MAX_SELF_DEFENSE;n++)
	{
		if ( lpObj->SelfDefense[n] >= 0 )
		{
			if ( GetTickCount() > lpObj->SelfDefenseTime[n] )
			{
				wsprintf(szTemp, lMsg.Get(MSGGET(4, 91)), lpObj->Name);
				GCServerMsgStringSend(szTemp, lpObj->m_Index, 1);
				GCServerMsgStringSend(szTemp, lpObj->SelfDefense[n], 1);
				lpObj->SelfDefense[n] = -1;
				
				LogAddTD("[%s][%s] ReSet SelfDefence [%s][%s]",
					lpObj->AccountID, lpObj->Name,
					gObj[lpObj->SelfDefense[n]].AccountID, gObj[lpObj->SelfDefense[n]].Name);
			}
		}
	}
}


struct PMSG_KILLPLAYER
{
	PBMSG_HEAD h;	// C1:16
	BYTE NumberH;	// 3
	BYTE NumberL;	// 4
	BYTE ExpH;	// 5
	BYTE ExpL;	// 6
	BYTE DamageH;	// 7
	BYTE DamageL;	// 8
};

void gObjAddSingleExp(LPOBJ lpObjDead, LPOBJ lpObj)
{		
	if ( lpObj->Type != OBJ_USER || lpObjDead->Type != OBJ_USER )
	{
		return;
	}

	unsigned int exp;
	int maxexp = 0;
	int totalExpMultiplyer = 0;
	int level = ((lpObjDead->Level/3)+25)*lpObjDead->Level/9;

	if(((lpObjDead->Level/3)+10) < (lpObj->Level/3))
	{
		level = level*((lpObjDead->Level/3)+10)/(lpObj->Level/3);
	}

	if(lpObjDead->Level >= 65)
	{
		level = level + ((lpObjDead->Level/3)-64)*(lpObjDead->Level/12);
	}

	if(level > 0)
	{
		maxexp = level/2;
	}
	else
	{
		level = 0;
	}

	if(maxexp < 1)
	{
		exp = level;
	}
	else
	{
		exp = level + rand()%maxexp;
	}

	exp = lpObjDead->MaxLife * exp / lpObj->MaxLife;

	if(BC_MAP_RANGE(lpObj->MapNumber))
	{
		exp = exp * 50 / 100;
	}

	DWORD mymaxexp = gObjExpCal(lpObj, 0); //gLevelExperience[lpObj->Level];

	if((ReadConfig.ExpSys == 1) || (ReadConfig.ExpSys == 2))
	{
		if ( exp > mymaxexp )
		{
			exp = mymaxexp;
		}
	}

	//Get Exp Rate for the user
	totalExpMultiplyer = gObjGetExperienceRate(lpObj,0);
	totalExpMultiplyer = totalExpMultiplyer / 2;

	exp *= totalExpMultiplyer;

	if ( (lpObj->m_wExprienceRate + lpObj->MasterCharacterInfo->IncExperience) > 0 )
	{
		exp =  (float)exp * ((float)(lpObj->m_wExprienceRate + lpObj->MasterCharacterInfo->IncExperience)  / 100.0f);
	}

	if ( (lpObj->pInventory[8].IsItem() == TRUE && lpObj->pInventory[8].m_Type == ITEMGET(13,123)) ) //NEW SKELETON PET EFFECT
	{
		exp = (int)(exp * 1.3f);
	}

	if ( g_CrywolfSync.GetOccupationState() == CRYWOLF_OCCUPATION_FAILED && g_iCrywolfApplyMvpPenalty != FALSE)
	{
#if (PACK_EDITION>=2)
		if ((VipSystem.VipIsApplyCWExpPenalty == 0)&&(lpObj->Vip >= 1))
		{
		} else {
			exp =  (exp * g_CrywolfSync.GetGettingExpPenaltyRate()) / 100;
		}
#else
		exp =  (exp * g_CrywolfSync.GetGettingExpPenaltyRate()) / 100;
#endif
	}

	if(ReadConfig.ExpSys == 0)
	{
		if ( exp > mymaxexp )
		{
			exp = mymaxexp;
		}
	}

	if ( lpObj->m_btDisableExpGain == 1 )
	{
		exp = 0;
	}

#if (PACK_EDITION>=3)
	if(lpObj->BotNumOwner >= OBJ_MAXMONSTER && lpObj->HaveBot == 1)
	{
		int botExp = exp * (gObj[lpObj->BotNumOwner].BotLvlUpExp/100.0f);
		exp -= botExp;
		botPet.AddExp(lpObj->BotNumOwner,botExp);
	}
#endif

	if(exp > 0)
	{
		if (gObjIsMaxLevel(lpObj))
		{
			//Do nothing
		}
		else
		{
			PMSG_KILLPLAYER pkillMsg;
				
			PHeadSetBE((LPBYTE)&pkillMsg, 0x16, sizeof(pkillMsg));

			pkillMsg.NumberH = -1;
			pkillMsg.NumberL = -1;
			pkillMsg.ExpH = SET_NUMBERH(exp);
			pkillMsg.ExpL = SET_NUMBERL(exp);
			pkillMsg.DamageH = SET_NUMBERH(0x00);
			pkillMsg.DamageL = SET_NUMBERL(0x00);

			DataSend(lpObj->m_Index, (UCHAR*)&pkillMsg, pkillMsg.h.size);

			gObjSetExpPetItem(lpObj->m_Index, exp);
			int LevelUp = 0;
				
			gObjLevelUp(lpObj,exp,lpObj->Class,EVENT_TYPE_EXTENDED_PVPSYSTEM,LevelUp);
		}
	}
}

BOOL gObjAttack(LPOBJ lpObj, LPOBJ lpTargetObj, CMagicInf * lpMagic, BOOL magicsend, BYTE MSBFlag, int AttackDamage, BOOL bCombo)
{
	return gclassObjAttack.Attack(lpObj, lpTargetObj, lpMagic, magicsend, MSBFlag, AttackDamage, bCombo);
}


void gObjMonsterExpDivision(LPOBJ lpMonObj, LPOBJ lpObj, int AttackDamage, int MSBFlag)
{
	int tObjNum;
	int exp;
	LPOBJ lpTargetObj;

	gObjMonsterHitDamageUserDel(lpMonObj);
	
	for(int n = 0; n < MAX_ST_HIT_DAMAGE;n++)
	{
		if(lpMonObj->sHD[n].number >= 0)
		{
			tObjNum = lpMonObj->sHD[n].number;
			lpTargetObj = &gObj[tObjNum];

			if(lpMonObj->sHD[n].HitDamage > 0)
			{
				int LevelUp;
				exp = gObjMonsterExpSingle(lpTargetObj,lpMonObj,lpMonObj->sHD[n].HitDamage,lpMonObj->MaxLife,LevelUp);

				if(exp > 0)
				{
					if(lpObj->Type == OBJ_USER)
					{
						if(LevelUp == 0)
						{
							if (gObjIsMaxLevel(lpTargetObj))
							{
								//Do nothing
							}
							else if(lpTargetObj->m_Index == lpObj->m_Index)
							{
								if((lpObj->m_wExprienceRate + lpObj->MasterCharacterInfo->IncExperience) > 0 && lpObj->m_btDisableExpGain == 0)
								{
									GCKillPlayerExpSend(lpTargetObj->m_Index,lpMonObj->m_Index,exp,AttackDamage,MSBFlag);
								}
							}
							else if((lpTargetObj->m_wExprienceRate + lpTargetObj->MasterCharacterInfo->IncExperience) > 0 && lpTargetObj->m_btDisableExpGain == 0)
							{
								GCKillPlayerExpSend(lpTargetObj->m_Index,lpMonObj->m_Index,exp,0,MSBFlag);
							}
						}
					}
					lpMonObj->Money = exp * ReadConfig.ZenDropMult;
				}
			}
		}
	}
}

//VAR Type: reserved for future.
int gObjGetExperienceRate(LPOBJ lpObj, BYTE type)
{
	int totalExpMultiplyer = 0;

	totalExpMultiplyer = gAddExperience;

#if (PACK_EDITION>=1)
	if(HappyHour.Enabled == 1 && HappyHour.Start == 1)
	{
		totalExpMultiplyer += HappyHour.ExtraExp;
	}
#endif

#if (PACK_EDITION>=2)
	if(lpObj->Vip >= 1)
	{
		if (VipSystem.VipExperience > gAddExperience)
			totalExpMultiplyer = VipSystem.VipExperience;
		else
			totalExpMultiplyer = gAddExperience;
	}

	if ( XMasEvent.Enabled == 1 )
	{
		totalExpMultiplyer += XMasEvent.ExtraExperience;
	}

	int SkyPlusExp = SkyEvent.MonsterDie(lpObj);

	if(SkyPlusExp != 1)
	{
		totalExpMultiplyer += SkyPlusExp;
	}

	//VIP Extra Eperience System
	if (ReadConfig.IsVipExtraExp == 1)
	{
		if(lpObj->Vip >= 1)
		{
			if(lpObj->VipDays <= ReadConfig.VIPDaysExtraExp[0])
			{
				totalExpMultiplyer += ReadConfig.VIPExtraExp[0];
			}else if((lpObj->VipDays > ReadConfig.VIPDaysExtraExp[0]) && (lpObj->VipDays <= ReadConfig.VIPDaysExtraExp[1]))
			{
				totalExpMultiplyer += ReadConfig.VIPExtraExp[1];
			}else if((lpObj->VipDays > ReadConfig.VIPDaysExtraExp[1]) && (lpObj->VipDays <= ReadConfig.VIPDaysExtraExp[2]))
			{
				totalExpMultiplyer += ReadConfig.VIPExtraExp[2];
			}
		}
	}
#endif

	//Add Extra Map Experience
	if(lpObj->PartyNumber >= 0)
	{
		totalExpMultiplyer += ReadConfig.MapExtraExpParty[lpObj->MapNumber];
	} else {
		totalExpMultiplyer += ReadConfig.MapExtraExpSingle[lpObj->MapNumber];
	}

	//Add Extra Day experience
	totalExpMultiplyer += ReadConfig.ExpDay;

	//Reset Extra Eperience System
	if (ReadConfig.IsResetExtraExp == 1)
	{
		if(lpObj->Resets > 0)
		{
			if(lpObj->Resets <= ReadConfig.ResetNrExtraExp[0])
			{
				totalExpMultiplyer += ReadConfig.ResetExtraExp[0];
			}else if((lpObj->Resets > ReadConfig.ResetNrExtraExp[0]) && (lpObj->Resets <= ReadConfig.ResetNrExtraExp[1]))
			{
				totalExpMultiplyer += ReadConfig.ResetExtraExp[1];
			}else if((lpObj->Resets > ReadConfig.ResetNrExtraExp[1]) && (lpObj->Resets <= ReadConfig.ResetNrExtraExp[2]))
			{
				totalExpMultiplyer += ReadConfig.ResetExtraExp[2];
			}
		}
	}

	//Reset Penalty Eperience System
	if (ReadConfig.IsResetPenaltyExp == 1)
	{
#if (PACK_EDITION>=2)
		if ((VipSystem.VipIsApplyResetExpPenalty == 0)&&(lpObj->Vip >= 1))
		{
		} else {
			totalExpMultiplyer -= roundInt(ReadConfig.PenaltyExpPerReset*lpObj->Resets);
		}
#else
		totalExpMultiplyer -= roundInt(ReadConfig.PenaltyExpPerReset*lpObj->Resets);
#endif
	}

	if (totalExpMultiplyer < 1)
	{
		totalExpMultiplyer = 1;
	}

	return totalExpMultiplyer;
}

int gObjMonsterExpSingle(LPOBJ lpObj, LPOBJ lpTargetObj, int dmg, int tot_dmg, int & LevelUp)
{
	LevelUp = 0;
	if(DS_MAP_RANGE(lpObj->MapNumber))
	{
		return g_DevilSquare.gObjMonsterExpSingle(lpObj,lpTargetObj,dmg,tot_dmg);
	}
	if(lpObj->Type != OBJ_USER)
	{
		return 0;
	}

	unsigned int exp;
	int maxexp = 0;
	int totalExpMultiplyer = 0;
	int level = (lpTargetObj->Level+25)*lpTargetObj->Level/3;

	if((lpTargetObj->Level+10) < lpObj->Level)
	{
		level = level*(lpTargetObj->Level+10)/lpObj->Level;
	}

	if(lpTargetObj->Level >= 65)
	{
		level = level + (lpTargetObj->Level-64)*(lpTargetObj->Level/4);
	}

	if(level > 0)
	{
		maxexp = level/2;
	}
	else
	{
		level = 0;
	}

	if(maxexp < 1)
	{
		exp = level;
	}
	else
	{
		exp = level + rand()%maxexp;
	}

	exp = dmg * exp / tot_dmg;

	if(BC_MAP_RANGE(lpObj->MapNumber))
	{
		exp = exp * 50 / 100;
	}

	DWORD mymaxexp = gObjExpCal(lpObj, 0); //gLevelExperience[lpObj->Level];

	if((ReadConfig.ExpSys == 1) || (ReadConfig.ExpSys == 2))
	{
		if ( exp > mymaxexp )
		{
			exp = mymaxexp;
		}
	}

	//Get Exp Rate for the user
	totalExpMultiplyer = gObjGetExperienceRate(lpObj,0);

	exp *= totalExpMultiplyer;

	if ( (lpObj->m_wExprienceRate + lpObj->MasterCharacterInfo->IncExperience) > 0 )
	{
		exp =  (float)exp * ((float)(lpObj->m_wExprienceRate + lpObj->MasterCharacterInfo->IncExperience)  / 100.0f);
	}

	if ( (lpObj->pInventory[8].IsItem() == TRUE && lpObj->pInventory[8].m_Type == ITEMGET(13,123)) ) //NEW SKELETON PET EFFECT
	{
		exp = (int)(exp * 1.3f);
	}

	if ( g_CrywolfSync.GetOccupationState() == CRYWOLF_OCCUPATION_FAILED && g_iCrywolfApplyMvpPenalty != FALSE)
	{
#if (PACK_EDITION>=2)
		if ((VipSystem.VipIsApplyCWExpPenalty == 0)&&(lpObj->Vip >= 1))
		{
		} else {
			exp =  (exp * g_CrywolfSync.GetGettingExpPenaltyRate()) / 100;
		}
#else
		exp =  (exp * g_CrywolfSync.GetGettingExpPenaltyRate()) / 100;
#endif
	}

	if(ReadConfig.ExpSys == 0)
	{
		if ( exp > mymaxexp )
		{
			exp = mymaxexp;
		}
	}

#if (PACK_EDITION>=3)
	if(lpObj->BotNumOwner >= OBJ_MAXMONSTER && lpObj->HaveBot == 1)
	{
		int botExp = exp * (gObj[lpObj->BotNumOwner].BotLvlUpExp/100.0f);
		exp -= botExp;
		botPet.AddExp(lpObj->BotNumOwner,botExp);
	}
#endif

	if(exp > 0)
	{
		int iEventType = getCurrentEventType(lpObj,0);		

		if(ReadConfig.ExpSys == 2)
		{
			gObjSetExpPetItem(lpObj->m_Index, exp);

			if((lpObj->m_wExprienceRate + lpObj->MasterCharacterInfo->IncExperience) > 0 && lpObj->m_btDisableExpGain == 0)
			{
				lpObj->Experience += exp;
			}
			gObjLevelUp(lpObj, exp, 0, iEventType,LevelUp); //lpObj->Experience += exp;
		}else
		{
			if((lpObj->m_wExprienceRate + lpObj->MasterCharacterInfo->IncExperience) > 0 && lpObj->m_btDisableExpGain == 0)
			{
				gObjSetExpPetItem(lpObj->m_Index, exp);
				int iMAX_LEVCOUNT = 0, iCAL_EXP = exp;

				while ( iCAL_EXP > 0 )
				{
					if ( iCAL_EXP > 0 )
					{
						iCAL_EXP = gObjLevelUp(lpObj, iCAL_EXP, 0, iEventType,LevelUp);
					}

					if(iCAL_EXP == 0)
						break;

					iMAX_LEVCOUNT++;
					if (iMAX_LEVCOUNT > 5)
						break;
				}
			}
		}
	}

	return exp;
}



void gObjExpParty(LPOBJ lpObj , LPOBJ lpTargetObj, int AttackDamage, int MSBFlag)
{

	if(DS_MAP_RANGE(lpObj->MapNumber))
	{
		g_DevilSquare.gObjExpParty(lpObj,lpTargetObj,AttackDamage,MSBFlag);
		return;
	}

	int n;
	unsigned int exp = 0;
	unsigned int tmpExp = 0;
	unsigned int maxexp = 0;
	unsigned int totalexp;
	int level = (lpTargetObj->Level+25)*lpTargetObj->Level / 3;
	int number;
	int partynum = 0;
	int totallevel = 0;
	int partylevel;
	int partycount;
	int dis[5];
	int viewplayer = 0;
	int viewpercent = 100;
	int bApplaySetParty = 0;
	BYTE bCheckSetParty[MAX_TYPE_PLAYER];
	LPOBJ lpPartyObj;

	partynum = lpObj->PartyNumber;

	int toplevel = 0;
	int totalExpMultiplyer = 0;
	
	for(n = 0; n < MAX_USER_IN_PARTY; n++)
	{
		number = gParty.m_PartyS[partynum].Number[n];

		if(number >= 0)
		{
			lpPartyObj = &gObj[number];

			if(lpPartyObj->Level > toplevel)
			{
				toplevel = lpPartyObj->Level;
			}
		}
	}

	if(!OBJMAX_RANGE(partynum))
	{
		LogAdd("error : %s %d",__FILE__,__LINE__);
		return;
	}

	memset(bCheckSetParty,0,sizeof(bCheckSetParty));

	partycount = gParty.m_PartyS[partynum].Count;

	for(n = 0; n < MAX_USER_IN_PARTY; n++)
	{
		number = gParty.m_PartyS[partynum].Number[n];

		if(number >= 0)
		{
			lpPartyObj = &gObj[number];

			if(lpTargetObj->MapNumber == lpPartyObj->MapNumber)
			{
				dis[n] = gObjCalDistance(lpTargetObj,&gObj[number]);

				if(dis[n] < 10)
				{
					lpPartyObj = &gObj[number];

					if(toplevel >= (lpPartyObj->Level+200))
					{
						totallevel += lpPartyObj->Level+200;
					}
					else
					{
						totallevel += lpPartyObj->Level;
					}
					viewplayer += 1;
					bCheckSetParty[lpPartyObj->Class] = 1;
				}
			}
		}
	}

	if(bCheckSetParty[CLASS_WIZARD] != 0 && bCheckSetParty[CLASS_KNIGHT] != 0 && bCheckSetParty[CLASS_ELF]!= 0)
	{
		bApplaySetParty = 1;
	}

	if(viewplayer > 1)
	{
		if(bApplaySetParty != 0)
		{
			if(viewplayer == 3)
			{
				viewpercent = 230;
			}
			else if(viewplayer == 4)
			{
				viewpercent = 270;
			}
			else if(viewplayer >= 5)
			{
				viewpercent = 300;
			}
			else
			{
				viewpercent = 120;
			}
		}
		else
		{
			if(viewplayer == 2)
			{
				viewpercent = 160;
			}
			else if(viewplayer == 3)
			{
				viewpercent = 180;
			}
			else if(viewplayer == 4)
			{
				viewpercent = 200;
			}
			else if(viewplayer >= 5)
			{
				viewpercent = 220;
			}
			else
			{
				viewpercent = 150;
			}
		}

		partylevel = totallevel / viewplayer;
	}
	else
	{
		partylevel = totallevel;
	}

	if((lpTargetObj->Level+10) < partylevel)
	{
		if(partylevel > 0)
			level = level * (lpTargetObj->Level+10) / partylevel;
	}

	if(lpTargetObj->Level >= 65)
	{
		if(viewplayer == 1)
		{
			level = level + (lpTargetObj->Level-64)*(lpTargetObj->Level/4);
		}
		else
		{
			level += (200.0 - (lpObj->Level * 0.2));
		}
	}

	if(level > 0)
	{
		maxexp = level / 2;
	}
	else
	{
		level = 0;
	}

	if(maxexp < 1)
	{
		totalexp = level;
	}
	else
	{
		totalexp = level + rand()%maxexp;
	}

	for(n = 0; n < MAX_USER_IN_PARTY; n++)
	{
		number = gParty.m_PartyS[partynum].Number[n];

		if(number >= 0)
		{
			lpPartyObj = &gObj[number];

			if(lpPartyObj->Type != OBJ_USER)
			{
				continue;
			}

			if(lpTargetObj->MapNumber == lpPartyObj->MapNumber && dis[n] < 10)
			{
				DWORD mymaxexp = gObjExpCal(lpPartyObj, 0); //gLevelExperience[lpPartyObj->Level];

				exp = (totalexp * viewpercent * lpPartyObj->Level)/totallevel / 100;

				if(BC_MAP_RANGE(lpPartyObj->MapNumber))
				{
					exp = exp * 50 / 100;
				}

				if(lpPartyObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER)
				{
					exp = 0;
				}

				if((ReadConfig.ExpSys == 1) || (ReadConfig.ExpSys == 2))
				{
					if ( exp > mymaxexp )
					{
						exp = mymaxexp;
					}
				}

				//Get Exp Rate for the user
				totalExpMultiplyer = gObjGetExperienceRate(lpPartyObj,0);

				exp *= totalExpMultiplyer;
				
				if((lpPartyObj->m_wExprienceRate + lpPartyObj->MasterCharacterInfo->IncExperience) > 0)
				{
					exp = (float)exp * ((float)(lpPartyObj->m_wExprienceRate + lpPartyObj->MasterCharacterInfo->IncExperience)  / 100.0f);
				}

				if ( (lpPartyObj->pInventory[8].IsItem() == TRUE && lpPartyObj->pInventory[8].m_Type == ITEMGET(13,123)) ) //NEW SKELETON EXTRA EXP
				{
					exp = (int)(exp * 1.3f);
				}

				if ( g_CrywolfSync.GetOccupationState() == CRYWOLF_OCCUPATION_FAILED && g_iCrywolfApplyMvpPenalty != FALSE)
				{
#if (PACK_EDITION>=2)
					if ((VipSystem.VipIsApplyCWExpPenalty == 0)&&(lpPartyObj->Vip >= 1))
					{
					} else {
						exp =  (exp * g_CrywolfSync.GetGettingExpPenaltyRate()) / 100;
					}
#else
					exp =  (exp * g_CrywolfSync.GetGettingExpPenaltyRate()) / 100;
#endif
				}

				if(ReadConfig.ExpSys == 0)
				{
					if ( exp > mymaxexp )
					{
						exp = mymaxexp;
					}
				}
				
#if (PACK_EDITION>=3)
				if(lpPartyObj->BotNumOwner >= OBJ_MAXMONSTER && lpPartyObj->HaveBot == 1)
				{
					int botExp = exp * (gObj[lpPartyObj->BotNumOwner].BotLvlUpExp/100.0f);
					exp -= botExp;
					botPet.AddExp(lpPartyObj->BotNumOwner,botExp);
				}
#endif
				int LevelUp;

				if(exp > 0)
				{
					int iEventType = getCurrentEventType(lpPartyObj,1);

					if(ReadConfig.ExpSys == 2)
					{
						if( (lpPartyObj->m_wExprienceRate + lpPartyObj->MasterCharacterInfo->IncExperience) > 0 && lpPartyObj->m_btDisableExpGain == 0)
						{
							lpPartyObj->Experience += exp;
						}

						if(gObjLevelUp(lpPartyObj, exp, 0, iEventType,LevelUp) == 0)
						{
							continue;
						}
					}else
					{
						if((lpPartyObj->m_wExprienceRate + lpPartyObj->MasterCharacterInfo->IncExperience) > 0 && lpPartyObj->m_btDisableExpGain == 0)
						{
							gObjSetExpPetItem(lpPartyObj->m_Index, exp);
							int iMAX_LEVCOUNT = 0, iCAL_EXP = exp;

							while ( iCAL_EXP > 0 )
							{
								if ( iCAL_EXP > 0 )
								{
									iCAL_EXP = gObjLevelUp(lpPartyObj, iCAL_EXP, 0, iEventType,LevelUp);
								}

								if(iCAL_EXP == 0)
									break;

								iMAX_LEVCOUNT++;
								if (iMAX_LEVCOUNT > 5)
									break;
							}

							if ((iCAL_EXP < 0)&&(iMAX_LEVCOUNT < 2))
								continue;
						}
					}
				}

				if(lpTargetObj->Type == OBJ_MONSTER)
				{
					if (tmpExp < exp)
						tmpExp = exp;
				}

				if(LevelUp == 0)
				{
					if((lpPartyObj->m_wExprienceRate + lpPartyObj->MasterCharacterInfo->IncExperience) > 0 && lpPartyObj->m_btDisableExpGain == 0)
					{
						GCKillPlayerExpSend(lpPartyObj->m_Index,lpTargetObj->m_Index,exp,AttackDamage,MSBFlag);
					}
				}
			}
		}
	}

	if(lpTargetObj->Type == OBJ_MONSTER)
	{
		tmpExp = tmpExp * ReadConfig.ZenDropMult;
		//lpTargetObj->Money = totalexp;
		if (tmpExp > MAX_ZEN)
			lpTargetObj->Money = MAX_ZEN;
		else
			lpTargetObj->Money = tmpExp;
	}	

}






void gObjMonsterDieLifePlus(LPOBJ lpObj, LPOBJ lpTartObj)
{
	if(lpTartObj->Level < 0 || lpTartObj->Level > 255)
	{
		return;
	}

	lpObj->Life += lpTartObj->Level;

	if(lpObj->MonsterDieGetLife != 0)
	{
		lpObj->Life = lpObj->Life + ((lpObj->MaxLife + lpObj->AddLife)/8.0f) * lpObj->MonsterDieGetLife;
	}

	if (lpObj->Type == OBJ_USER)
	{
		if(lpObj->MasterCharacterInfo->MonsterDieIncRecoverSD != 0)
		{
			if(ReadConfig.S6E2 == 0)
				lpObj->iShield = lpObj->iShield + ((lpObj->iMaxShield + lpObj->iAddShield)/8.0f) * lpObj->MasterCharacterInfo->MonsterDieIncRecoverSD;
			else
				lpObj->iShield = lpObj->iShield + ((lpObj->iMaxShield + lpObj->iAddShield)/8.0f) + lpObj->MasterCharacterInfo->MonsterDieIncRecoverSD;
		}
	}

	if(lpObj->MonsterDieGetMana != 0)
	{
		lpObj->Mana = lpObj->Mana + ((lpObj->MaxMana + lpObj->AddMana)/8.0f) * lpObj->MonsterDieGetMana;
	}

	if (lpObj->Type == OBJ_USER)
	{
		if(ReadConfig.S6E2 == 1)
		{
			lpObj->Life += lpObj->MasterCharacterInfo->MonsterDieIncRecoverLife;
			lpObj->Mana += lpObj->MasterCharacterInfo->MonsterDieIncRecoverMana;
		}
	}

	if(lpObj->iShield > (lpObj->iMaxShield + lpObj->iAddShield))
	{
		lpObj->iShield = lpObj->iMaxShield + lpObj->iAddShield;
	}

	if(lpObj->Life > (lpObj->MaxLife + lpObj->AddLife))
	{
		lpObj->Life = lpObj->MaxLife + lpObj->AddLife;
	}

	GCReFillSend(lpObj->m_Index,lpObj->Life,255,0,lpObj->iShield);

	if(lpObj->MonsterDieGetMana != 0)
	{
		if(lpObj->Mana > (lpObj->MaxMana + lpObj->AddMana))
		{
			lpObj->Mana = lpObj->MaxMana + lpObj->AddMana;
		}

		GCManaSend(lpObj->m_Index,lpObj->Mana,255,0,lpObj->BP);
	}
}






void gObjLifeCheck(LPOBJ lpTargetObj, LPOBJ lpObj, int AttackDamage, int DamageSendType, int MSBFlag, int MSBDamage, WORD Skill, int iShieldDamage)
{
	if(lpObj->Connected < PLAYER_PLAYING)
	{
		return;
	}

	if(ATTRIBUTE_RANGE(lpObj->m_Attribute) || ATTRIBUTE_RANGE(lpTargetObj->m_Attribute))
	{
		return;
	}

	LPOBJ lpCallObj = lpObj;

	if(lpObj->Type == OBJ_MONSTER && lpObj->m_RecallMon >= 0)
	{
		lpCallObj = &gObj[lpObj->m_RecallMon];
	}

	if(AttackDamage != 0 && lpTargetObj->Type == OBJ_MONSTER)
	{
		if(lpTargetObj->m_RecallMon >= 0)
		{
			GCRecallMonLife(lpTargetObj->m_RecallMon,lpTargetObj->MaxLife,lpTargetObj->Life);
		}

		gObjMonsterSetHitDamage(lpTargetObj,lpCallObj->m_Index,AttackDamage);
	}

	if(lpTargetObj->Life <= 0 && lpTargetObj->Live != 0)
	{
		if(lpObj->Type == OBJ_MONSTER)
		{
			gObjAddMsgSend(lpObj,3,lpTargetObj->m_Index,0);
			CreateFrustrum(lpObj->X,lpObj->Y,lpObj->m_Index);
			lpTargetObj->KillerType = 1;
		}

		if(lpObj->Type == OBJ_USER)
		{
			gDarkSpirit[lpObj->m_Index].ReSetTarget(lpTargetObj->m_Index);
		}

		if(lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER)
		{
			gObjSetKillCount(lpObj->m_Index,1);
		}

		if(lpTargetObj->Type == OBJ_MONSTER)
		{
			if(lpTargetObj->Class == 338 || lpTargetObj->Class == 275 || lpTargetObj->Class == 295 || lpTargetObj->MapNumber == MAP_INDEX_CRYWOLF_FIRSTZONE)
			{
				gObjAddMsgSendDelayInSpecificQPos(lpTargetObj,1,lpObj->m_Index,800,0,0);
			}
			else
			{
				gObjAddMsgSendDelay(lpTargetObj,1,lpObj->m_Index,800,0);
			}

			#if (GS_CASTLE==1)
			g_Crywolf.CrywolfMonsterDieProc(lpTargetObj->m_Index, lpObj->m_Index);

			/*if( lpTargetObj->m_Attribute == 0x3E )
			{
				int iTargetClass = lpTargetObj->Class;

				if( iTargetClass != 295)
				{
					HuntZoneItemBoxOpen(lpObj, lpObj->MapNumber, lpObj->X, lpObj->Y);
				}
			}*/

			#else
			g_Kanturu.KanturuMonsterDieProc(lpTargetObj->m_Index,lpObj->m_Index);
			#endif

			if(BC_MAP_RANGE(lpTargetObj->MapNumber))
			{
				int BCRest = MAP_INDEX_BLOODCASTLE1;
				if(lpObj->MapNumber == MAP_INDEX_BLOODCASTLE8)
					BCRest = 45;

				if(lpTargetObj->Class == 131)
				{
					int TopHitUser = gObjMonsterTopHitDamageUser(lpTargetObj);

					if(TopHitUser != -1)
					{
						char szMsg[256];

						wsprintf(szMsg,lMsg.Get(1178),gObj[TopHitUser].Name);

						g_BloodCastle.SendNoticeMessage(lpObj->MapNumber - BCRest,szMsg);

						g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_iExtraEXP_Kill_Door_Party = gObj[TopHitUser].PartyNumber;
						g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_iExtraEXP_Kill_Door_Index = TopHitUser;

						memcpy(g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_szKill_Door_CharName,gObj[TopHitUser].Name,10);
						memcpy(g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_szKill_Door_AccountID,gObj[TopHitUser].AccountID,10);

						g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_szKill_Door_CharName[10] = 0;
						g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_szKill_Door_AccountID[10] = 0;

						LogAddTD("[Blood Castle] (%d) Door Terminated -> %d [%s][%s]",lpObj->MapNumber - (BCRest-1),g_BloodCastle.m_BridgeData[lpObj->MapNumber-BCRest].m_iBC_BOSS_MONSTER_KILL_COUNT,gObj[TopHitUser].AccountID,gObj[TopHitUser].Name);
					}
					else
					{
						g_BloodCastle.SendNoticeMessage(lpObj->MapNumber - BCRest,lMsg.Get(1169));
						LogAddTD("[Blood Castle] (%d) Door Terminated -> %d)",lpObj->MapNumber - (BCRest-1),g_BloodCastle.m_BridgeData[lpObj->MapNumber-BCRest].m_iBC_BOSS_MONSTER_KILL_COUNT);
					}
					if(g_BloodCastle.OpenDoorCheck[lpObj->MapNumber - BCRest] == false)
					{
						g_BloodCastle.OpenDoorCheck[lpObj->MapNumber - BCRest] = true;
						g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_bCASTLE_DOOR_LIVE = 0;
						g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_bBC_DOOR_TERMINATE_COMPLETE = 1;
						g_BloodCastle.m_BridgeData[lpObj->MapNumber - BCRest].m_iBC_BOSS_MONSTER_MAX_COUNT = g_BloodCastle.GetCurrentLiveUserCount(lpObj->MapNumber - BCRest) * 2;
	
						g_BloodCastle.m_BridgeData[lpObj->MapNumber - BCRest].m_iBC_BOSS_MONSTER_KILL_COUNT = 0;
	
						if(g_BloodCastle.m_BridgeData[lpObj->MapNumber - BCRest].m_iBC_BOSS_MONSTER_MAX_COUNT > 10)
						{
							g_BloodCastle.m_BridgeData[lpObj->MapNumber - BCRest].m_iBC_BOSS_MONSTER_MAX_COUNT = 10;
						}
	
						g_BloodCastle.SendCastleDoorBlockInfo(lpObj->m_cBloodCastleIndex,0);
						g_BloodCastle.ReleaseCastleDoor(lpObj->m_cBloodCastleIndex);
	
						if(g_BloodCastle.GetCurrentState(lpObj->m_cBloodCastleIndex) == 2)
						{
							g_BloodCastle.SetBossMonster(lpObj->m_cBloodCastleIndex);
						}
					}
				}


				if(BC_STATUE_RANGE(lpTargetObj->Class - 132))
				{
					int TopHitUser = gObjMonsterTopHitDamageUser(lpTargetObj);

					if(TopHitUser != -1)
					{
						int BCRest = MAP_INDEX_BLOODCASTLE1;
						if(lpObj->MapNumber == MAP_INDEX_BLOODCASTLE8)
							BCRest = 45;
						char szMsg[256];

						wsprintf(szMsg,lMsg.Get(1179),gObj[TopHitUser].Name);

						g_BloodCastle.SendNoticeMessage(lpObj->MapNumber - BCRest,szMsg);


						g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_iExtraEXP_Kill_Statue_Party = gObj[TopHitUser].PartyNumber;
						g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_iExtraEXP_Kill_Statue_Index = gObj[TopHitUser].m_Index;

						memcpy(g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_szKill_Status_CharName,gObj[TopHitUser].Name,10);
						memcpy(g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_szKill_Status_AccountID,gObj[TopHitUser].AccountID,10);

						g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_szKill_Status_CharName[10] = 0;
						g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_szKill_Status_AccountID[10] = 0;

						LogAddTD("[Blood Castle] (%d) a stone statue Of Saint Terminated -> %d [%s][%s]",lpObj->MapNumber - (BCRest - 1),g_BloodCastle.m_BridgeData[lpObj->MapNumber-BCRest].m_iBC_BOSS_MONSTER_KILL_COUNT,gObj[TopHitUser].AccountID,gObj[TopHitUser].Name);
					}
					else
					{
						g_BloodCastle.SendNoticeMessage(lpObj->MapNumber - BCRest,lMsg.Get(1170));
						LogAddTD("[Blood Castle] (%d) a stone statue Of Saint Terminated -> %d",lpObj->MapNumber - (BCRest-1),g_BloodCastle.m_BridgeData[lpObj->MapNumber-BCRest].m_iBC_BOSS_MONSTER_KILL_COUNT);
					}
				}

				for(int n = 0; n < MAX_BLOOD_CASTLE_SUB_BRIDGE; n++)
				{
					if(g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_UserData[n].m_iIndex != -1)
					{
						if(gObj[g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_UserData[n].m_iIndex].Connected > PLAYER_LOGGED)
						{
							g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_UserData[n].m_bLiveWhenDoorBreak = 1;
						}
					}
				}
			}

			if(CC_MAP_RANGE(lpTargetObj->MapNumber))
			{
				if(rand()%100 < 50)
				{
					g_ChaosCastle.SearchNBlowObjs(lpTargetObj->MapNumber,lpTargetObj->X,lpTargetObj->Y);
				}
				int CCRest = MAP_INDEX_CHAOSCASTLE1;
				if (lpTargetObj->MapNumber == MAP_INDEX_CHAOSCASTLE7)
					CCRest = 47;

				g_ChaosCastle.DelMonsterList(lpTargetObj->MapNumber - CCRest,lpTargetObj->m_Index);

				int TopHitUser = gObjMonsterTopHitDamageUser(lpTargetObj);

				if(TopHitUser != -1)
				{
					if(CC_MAP_RANGE(gObj[TopHitUser].MapNumber))
					{
						if(gObjIsConnected(TopHitUser))
						{
							gObj[TopHitUser].m_cKillMonsterCount += 1;
						}
					}
				}
			}

			gObjPkDownTimeCheck(lpObj,lpTargetObj->Level);

			if(lpObj->Type == OBJ_USER)
			{
				gObjAddMsgSendDelay(lpObj,3,lpTargetObj->m_Index,2000,0);
			}

			//lpTargetObj	= Monster
			//lpObj			= User
			IllusionTemple.MonsterDieKillPoint(lpObj->m_Index);
			Raklion.MonsterDiePoint(lpObj,lpTargetObj);
			g_ImperialGuardian.MonsterDiePoint(lpTargetObj);

			#if (PACK_EDITION>=2)
				customQuest.MonsterDie(lpObj,lpTargetObj->Class);
				PCShop.MonsterDie(lpObj,lpTargetObj);
			#endif

				qs5.MonsterDie(lpObj->m_Index,lpTargetObj->m_Index);
		}
		else if(lpTargetObj->Type == OBJ_USER)
		{
			if(BC_MAP_RANGE(lpTargetObj->MapNumber))
			{
				g_BloodCastle.SetUserState(lpTargetObj->m_Index,1);
				g_BloodCastle.SearchUserDropQuestItem(lpTargetObj->m_Index);
			}

			if(CC_MAP_RANGE(lpTargetObj->MapNumber))
			{
				int CCRest = MAP_INDEX_CHAOSCASTLE1;
				if (lpTargetObj->MapNumber == MAP_INDEX_CHAOSCASTLE7)
					CCRest = 47;
				g_ChaosCastle.SetUserState(lpTargetObj->m_Index,1);

				if(CC_MAP_RANGE(lpObj->MapNumber))
				{
					if(gObjIsConnected(lpObj->m_Index))
					{
						lpObj->m_cKillUserCount += 1;
					}
				}

				g_ChaosCastle.SendFailMessage(lpTargetObj->MapNumber - CCRest,lpTargetObj->m_Index);

				LogAddTD("[Chaos Castle] (%d) [%s][%s] User Dead In Chaos Castle : Killed by [%s][%s]",lpTargetObj->MapNumber - (CCRest - 1),lpTargetObj->AccountID,lpTargetObj->Name,lpObj->AccountID,lpObj->Name);
			}

			if(DG_MAP_RANGE(lpTargetObj->MapNumber))
			{
				g_DoppelGanger.DeadUserProc(lpTargetObj);

				LogAddTD("[DoppelGanger] [%s][%s] User Die on event -> Delete from event", lpObj->AccountID, lpObj->Name);
			}

			lpTargetObj->KillerType = 0;

			gObjUserDie(lpTargetObj,lpCallObj);
#if (PACK_EDITION>=3)
			botPet.DieClose(lpTargetObj->m_Index);
#endif
			//PET BUG FIX LALITABLUE
			if(lpTargetObj->Type == OBJ_USER)
				GCItemListSend(lpTargetObj->m_Index);
			//PET BUG FIX LALITABLUE

			int iGuildWar = gObjGuildWarCheck(lpCallObj,lpTargetObj);
			int iDuel = g_DuelManager.gObjDuelCheck(lpCallObj,lpTargetObj);

			if(iGuildWar == 0 && iDuel == 0)
			{
				if(lpTargetObj->m_bAttackerKilled == 0)
				{
					gObjPlayerKiller(lpCallObj,lpTargetObj);
				}
			}

			lpTargetObj->m_bAttackerKilled = 0;

			if(lpTargetObj->m_RecallMon >= 0)
			{
				gObjMonsterCallKill(lpTargetObj->m_Index);
			}
		}

		lpTargetObj->Live = 0;
		lpTargetObj->m_State = 4;
		lpTargetObj->RegenTime = GetTickCount();
		lpTargetObj->DieRegen = 1;
		lpTargetObj->PathCount = 0;
		if(gObjTargetGuildWarCheck(lpTargetObj,lpCallObj)==1)
		{
			lpTargetObj->KillerType = 2;
		}

		g_DuelManager.KillUserProc(lpObj,lpTargetObj);

		if(lpTargetObj->Teleport == 1)
		{
			lpTargetObj->Teleport = 0;
		}

		GCDiePlayerSend(lpTargetObj,lpTargetObj->m_Index,Skill,lpObj->m_Index);

		if(lpTargetObj->Class == 79)
		{
			gEledoradoEvent.m_BossGoldDerconMapNumber[lpTargetObj->m_BossGoldDerconMapNumber] = -1;
			gEledoradoEvent.CheckGoldDercon(lpObj->MapNumber);
		}

		if(lpTargetObj->m_RecallMon >= 0)
		{
			return;
		}

		if(lpTargetObj->Class == 340 || lpTargetObj->Class == 348 || lpTargetObj->Class == 349)
		{
			return;
		}

		if(lpTargetObj->Class == 287 || lpTargetObj->Class == 286)
		{
			return;
		}

		if(lpTargetObj->Class == 275 || lpTargetObj->Class == 338)
		{
			return;
		}

		if(lpTargetObj->Class == 288)
		{
			return;
		}

		if(lpTargetObj->Class == 295)
		{
			return;
		}

		if(lpTargetObj->Class == 362 || lpTargetObj->Class == 363 || lpTargetObj->Class == 361)
		{
			return;
		}

		if(lpCallObj->Type == OBJ_USER)
		{
			if(lpCallObj->PartyNumber >= 0)
			{
				gObjExpParty(lpCallObj,lpTargetObj,AttackDamage,MSBFlag);
			}
			else if(lpTargetObj->Type == OBJ_MONSTER)
			{
				gObjMonsterExpDivision(lpTargetObj,lpCallObj,AttackDamage,MSBFlag);
			}

			if(lpTargetObj->Type == OBJ_MONSTER && DS_MAP_RANGE(lpTargetObj->MapNumber))
			{
				g_DevilSquare.gObjMonsterScoreDivision(lpTargetObj,lpCallObj,AttackDamage,MSBFlag);
			}
		}
		if(lpTargetObj->Type == OBJ_MONSTER)
		{
			if(lpTargetObj->m_bIsInMonsterHerd != false && lpTargetObj->m_lpMonsterHerd != 0)
			{
				lpTargetObj->m_lpMonsterHerd->MonsterDieAction(lpTargetObj);
			}
		}
	}
	else
	{
		if(DamageSendType == DAMAGE_TYPE_REG)
		{
			GCDamageSend(lpCallObj->m_Index,lpTargetObj->m_Index,AttackDamage,MSBFlag,MSBDamage,iShieldDamage);
		}
		else if(DamageSendType == DAMAGE_TYPE_DELAY)
		{
			//Attack is called from gObjDelayLifeCheck function and acts as a life check function
			GCDamageSend(lpTargetObj->m_Index,AttackDamage,iShieldDamage);
		}
		else if(DamageSendType == DAMAGE_TYPE_BEAT)
		{
			//Attack is called in gObjSkillBeAttackProc and appears as a poison time attack type
			GCDamageSend(lpCallObj->m_Index, lpTargetObj->m_Index, AttackDamage, MSBFlag, MSG_DAMAGE_BEAT, iShieldDamage);
		} 
	}
}






BOOL gObjInventoryTrans(int aIndex)
{
	if ( ((aIndex< 0)?FALSE:(aIndex > OBJMAX-1)?FALSE:TRUE) == FALSE )
	{
		LogAdd("error : %s %d", __FILE__, __LINE__);
		return FALSE;
	}

	if ( gObj[aIndex].pTransaction == 1 )
	{
		LogAddTD("[%s][%s] error-L3 : pTransaction(%d) status error %s %d",
			gObj[aIndex].AccountID, gObj[aIndex].Name,
			gObj[aIndex].pTransaction, __FILE__, __LINE__);
		return FALSE;
	}

	if (aIndex >= OBJ_MAXMONSTER )
	{
		for ( int n=0;n<MAX_MAGIC;n++)
		{
			gObj[aIndex].m_lpMagicBack[n] = gObj[aIndex].Magic[n]; 
		}
	}

	for (int n=0;n<ReadConfig.MAIN_INVENTORY_SIZE(aIndex,false);n++)
	{
		gObj[aIndex].Inventory2[n] = gObj[aIndex].Inventory1[n];
	}

	memcpy(gObj[aIndex].InventoryMap2, gObj[aIndex].InventoryMap1, ReadConfig.INVENTORY_MAP_SIZE(aIndex,true));
	gObjSetInventory2Pointer(&gObj[aIndex]);
	gObj[aIndex].pTransaction = 1;
	
	//TRADE_LOG.Output("Trade Transaction (%s)", gObj[aIndex].Name);
	//LogAddL("Trade Transaction (%s)", gObj[aIndex].Name);

	return TRUE;
}

// CMagicInf::operator= 4c4d90






int gObjInventoryCommit(int aIndex)
{
	if(!OBJMAX_RANGE(aIndex))
	{
		LogAddTD("error : gObjInventoryCommit() - aIndex out of bound %s %d",__FILE__,__LINE__);
		return false;
	}

	if(gObj[aIndex].pTransaction != 1)
	{
		LogAddTD("[%s][%s] error-L3 : pTransaction(%d) status2",gObj[aIndex].AccountID,gObj[aIndex].Name,gObj[aIndex].pTransaction);
		return false;
	}

	for(int n = 0; n < ReadConfig.MAIN_INVENTORY_SIZE(aIndex,false); n++)
	{
		gObj[aIndex].Inventory1[n] = gObj[aIndex].Inventory2[n];
	}

	memcpy(gObj[aIndex].InventoryMap1,gObj[aIndex].InventoryMap2, ReadConfig.INVENTORY_MAP_SIZE(aIndex,true));
	gObjSetInventory1Pointer(&gObj[aIndex]);
	gObj[aIndex].pTransaction = 2;
	//TRADE_LOG.Output("Trade Commit (%s)",gObj[aIndex].Name);
	//LogAddL("Trade Commit (%s)",gObj[aIndex].Name);

	return true;
}






int gObjInventoryRollback(int aIndex)
{
	if(!OBJMAX_RANGE(aIndex))
	{
		LogAdd("error : %s %d",__FILE__,__LINE__);
		return false;
	}

	if(gObj[aIndex].pTransaction != 1)
	{
		LogAddTD("[%s][%s] error-L3 : pTransaction(%d) status2",gObj[aIndex].AccountID,gObj[aIndex].Name,gObj[aIndex].pTransaction);
		return false;
	}

	if (aIndex >= OBJ_MAXMONSTER )
	{
		for(int n = 0; n < MAX_MAGIC; n++)
		{
			gObj[aIndex].Magic[n] = gObj[aIndex].m_lpMagicBack[n];
		}
	}

	for(int n = 0; n < ReadConfig.INVENTORY_SIZE(aIndex,false); n++)
	{
		gObj[aIndex].Inventory2[n].Clear();
	}

	gObjSetInventory1Pointer(&gObj[aIndex]);

	gObj[aIndex].pTransaction = 3;

	//TRADE_LOG.Output("Trade Rollback (%s)",gObj[aIndex].Name);
	//LogAddL("Trade Rollback (%s)",gObj[aIndex].Name);

	return true;
}






void gObjInventoryItemSet(int aIndex, int itempos, BYTE set_byte)
{
	int width;
	int height;
	if(itempos < INVETORY_WEAR_SIZE)
	{
		return;
	}
	else if(itempos > (ReadConfig.INVENTORY_SIZE(aIndex,false)-1))
	{
		return;
	}

	if(gObj[aIndex].pInventory[itempos].GetSize((int&)width,(int &)height)==0)
	{
		LogAdd(lMsg.Get(527),__FILE__,__LINE__);
		return;
	}

	gObjInventoryItemBoxSet(aIndex,itempos,width,height,set_byte);
}



void gObjInventoryItemBoxSet(int aIndex, int itempos, int xl, int yl, BYTE set_byte)
{
	int InventoryRows = 12;
	if(ReadConfig.S6E2 == 1)
		InventoryRows = 28;
			
	int itemposx = (itempos - INVETORY_WEAR_SIZE)%8;
	int itemposy = (itempos - INVETORY_WEAR_SIZE)/8;

	int xx,yy;

	for(int y = 0; y < yl; y ++)
	{
		yy = itemposy + y;

		for(int x = 0; x < xl; x++)
		{
			xx = itemposx + x;

			if((ExtentCheck(xx,yy,8,InventoryRows)==1) 
#if (PACK_EDITION>=3)
				|| (gObj[aIndex].IsBot >= 1)
#endif
				)
			{
				*(BYTE*)(gObj[aIndex].pInventoryMap + (itemposy + y)*8+(itemposx + x)) = set_byte;
			}
			else
			{
				LogAdd("error : %s %d",__FILE__,__LINE__);
				return;
			}
		}
	}
}


bool gObjFixInventoryPointer(int aIndex)
{

	if(gObjIsConnected(aIndex)==0)
	{
		LogAdd("[Fix Inv.Ptr] [%s][%s] - disconnected",gObj[aIndex].AccountID,gObj[aIndex].Name);
		return false;
	}


	if(gObj[aIndex].pInventory == gObj[aIndex].Inventory1)
	{
		return true;
	}

	if(gObj[aIndex].pInventory == gObj[aIndex].Inventory2)
	{
		if(gObj[aIndex].pTransaction == 1)
		{
			LogAdd("[Fix Inv.Ptr] [%s][%s] - Transaction == 1, Do not change Pointer",gObj[aIndex].AccountID,gObj[aIndex].Name);
			return false;
		}
		else
		{
			LogAdd("[Fix Inv.Ptr] [%s][%s] - Inventory Pointer was 2",gObj[aIndex].AccountID,gObj[aIndex].Name);

			for(int n = 0; n < ReadConfig.INVENTORY_SIZE(aIndex,false); n++)
			{
				gObj[aIndex].Inventory2[n].Clear();
			}
		}
	}
	else
	{
		LogAdd("[Fix Inv.Ptr] [%s][%s] - Inventory Pointer was Wrong",gObj[aIndex].AccountID,gObj[aIndex].Name);
	}

	gObjSetInventory1Pointer(&gObj[aIndex]);

	return false;
}






void gObjInventoryItemSet_PShop(int aIndex, int itempos, BYTE set_byte)
{
	int width;
	int height;
	if(itempos < INVETORY_WEAR_SIZE)
	{
		return;
	}
	else if(itempos > (ReadConfig.INVENTORY_SIZE(aIndex,false)-1))
	{
		return;
	}

	if(gObj[aIndex].Inventory1[itempos].GetSize((int&)width,(int &)height)==0)
	{
		LogAdd(lMsg.Get(527),__FILE__,__LINE__);
		return;
	}

	gObjInventoryItemBoxSet_PShop(aIndex,itempos,width,height,set_byte);
}




void gObjInventoryItemBoxSet_PShop(int aIndex, int itempos, int xl, int yl, BYTE set_byte)
{
	int InventoryRows = 12;
	if(ReadConfig.S6E2 == 1)
		InventoryRows = 28;

	int itemposx = (itempos - INVETORY_WEAR_SIZE)%8;
	int itemposy = (itempos - INVETORY_WEAR_SIZE)/8;

	int xx,yy;

	for(int y = 0; y < yl; y ++)
	{
		yy = itemposy + y;

		for(int x = 0; x < xl; x++)
		{
			xx = itemposx + x;

			if(ExtentCheck(xx,yy,8,InventoryRows)==1)
			{
				*(BYTE*)(gObj[aIndex].InventoryMap1 + (itemposy + y)*8+(itemposx + x)) = set_byte;
			}
			else
			{
				LogAdd("error : %s %d",__FILE__,__LINE__);
				return;
			}
		}
	}
}






BYTE gObjInventoryDeleteItem(int aIndex ,int itempos)
{
	if(itempos < INVETORY_WEAR_SIZE)
	{
		int MagicDel = gObjMagicDel(&gObj[aIndex],gObj[aIndex].pInventory[itempos].m_Special[0],gObj[aIndex].pInventory[itempos].m_Level);

		if(MagicDel >= 0)
		{
			GCMagicListOneDelSend(aIndex,MagicDel,
				gObj[aIndex].pInventory[itempos].m_Special[0],
				gObj[aIndex].pInventory[itempos].m_Level,0,0);
		}

		gObj[aIndex].pInventory[itempos].Clear();
	}
	else
	{
		gObjInventoryItemSet(aIndex,itempos,255);
		gObj[aIndex].pInventory[itempos].Clear();
	}

	return true;
}






BYTE gObjWarehouseDeleteItem(int aIndex, int itempos)
{
	gObjWarehouseItemSet(aIndex, itempos, -1);
	gObj[aIndex].pWarehouse[itempos].Clear();

	return true;
}






BYTE gObjChaosBoxDeleteItem(int aIndex, int itempos)
{
	gObjChaosItemSet(aIndex, itempos, -1);
	gObj[aIndex].pChaosBox[itempos].Clear();

	return true;
}






BYTE gObjInventoryInsertItem(int aIndex, int item_num)
{
	int w,h,map_num,iwidth,iheight;
	BYTE blank;

	blank = 0;
	map_num = gObj[aIndex].MapNumber;

	if(MapC[map_num].m_cItem[item_num].live == 0)
	{
		return -1;
	}

	if(MapC[map_num].m_cItem[item_num].GetSize((int &)iwidth,(int &)iheight) == 0)
	{
		LogAdd(lMsg.Get(527),__FILE__,__LINE__);
		return -1;
	}

	for(h = 0; h < 8; h++)
	{
		for(w = 0; w < 8; w++)
		{
			if(*(BYTE*)(gObj[aIndex].pInventoryMap+h*8+w) == 255)
			{
				blank = gObjInventoryRectCheck(aIndex,w,h,iwidth,iheight);

				if(blank == 254)
				{
					goto NEXT1;
				}

				if(blank != 255)
				{
					gObj[aIndex].pInventory[blank] = MapC[map_num].m_cItem[item_num];
					gObjInventoryItemSet(aIndex,blank,gObj[aIndex].pInventory[blank].m_Type);
					return blank;
				}
			}
		}
	}
NEXT1:
	if(ReadConfig.IS_EXTENDED_INV(aIndex,1) == TRUE)
	{
		for(h = 8; h < 12; h++)
		{
			for(w = 0; w < 8; w++)
			{
				if(*(BYTE*)(gObj[aIndex].pInventoryMap+h*8+w) == 255)
				{
					blank = gObjInventoryRectCheck(aIndex,w,h,iwidth,iheight);

					if(blank == 254)
					{
						goto NEXT2;
					}

					if(blank != 255)
					{
						gObj[aIndex].pInventory[blank] = MapC[map_num].m_cItem[item_num];
						gObjInventoryItemSet(aIndex,blank,gObj[aIndex].pInventory[blank].m_Type);
						return blank;
					}
				}
			}
		}
	}
NEXT2:
	if(ReadConfig.IS_EXTENDED_INV(aIndex,2) == TRUE)
	{
		for(h = 12; h < 16; h++)
		{
			for(w = 0; w < 8; w++)
			{
				if(*(BYTE*)(gObj[aIndex].pInventoryMap+h*8+w) == 255)
				{
					blank = gObjInventoryRectCheck(aIndex,w,h,iwidth,iheight);

					if(blank == 254)
					{
						goto GOTO_EndFunc;
					}

					if(blank != 255)
					{
						gObj[aIndex].pInventory[blank] = MapC[map_num].m_cItem[item_num];
						gObjInventoryItemSet(aIndex,blank,gObj[aIndex].pInventory[blank].m_Type);
						return blank;
					}
				}
			}
		}
	}
GOTO_EndFunc:
	return -1;

}

BYTE gObjInventoryInsertItem(int aIndex, CItem item)
{
	int w,h,iwidth,iheight;
	BYTE blank = 0;

	if(item.GetSize((int&)iwidth,(int&)iheight)==0)
	{
		LogAdd(lMsg.Get(527),__FILE__,__LINE__);
		return -1;
	}

	for(h = 0; h < 8; h++)
	{
		for( w = 0; w < 8; w++)
		{
			if(*(BYTE*)(gObj[aIndex].pInventoryMap+h*8+w) == 255)
			{
				blank = gObjInventoryRectCheck(aIndex,w,h,iwidth,iheight);

				if(blank == 254)
				{
					goto NEXT1;
				}

				if(blank != 255)
				{
					if(gObjCheckSerial0ItemList(&item)!=0)
					{
						MsgOutput(aIndex,lMsg.Get(3354));
						return -1;
					}


					if(gObjInventorySearchSerialNumber(&gObj[aIndex],item.m_Number) == 0)
					{
						return -1;
					}

					gObj[aIndex].pInventory[blank] = item;

					gObjInventoryItemSet(aIndex,blank,gObj[aIndex].pInventory[blank].m_Type);
					return blank;
				}
			}
		}
	}
NEXT1:
	if(ReadConfig.IS_EXTENDED_INV(aIndex,1) == TRUE)
	{
		for(h = 8; h < 12; h++)
		{
			for( w = 0; w < 8; w++)
			{
				if(*(BYTE*)(gObj[aIndex].pInventoryMap+h*8+w) == 255)
				{
					blank = gObjInventoryRectCheck(aIndex,w,h,iwidth,iheight);

					if(blank == 254)
					{
						goto NEXT2;
					}

					if(blank != 255)
					{
						if(gObjCheckSerial0ItemList(&item)!=0)
						{
							MsgOutput(aIndex,lMsg.Get(3354));
							return -1;
						}


						if(gObjInventorySearchSerialNumber(&gObj[aIndex],item.m_Number) == 0)
						{
							return -1;
						}

						gObj[aIndex].pInventory[blank] = item;

						gObjInventoryItemSet(aIndex,blank,gObj[aIndex].pInventory[blank].m_Type);
						return blank;
					}
				}
			}
		}
	}
NEXT2:
	if(ReadConfig.IS_EXTENDED_INV(aIndex,2) == TRUE)
	{
		for(h = 12; h < 16; h++)
		{
			for( w = 0; w < 8; w++)
			{
				if(*(BYTE*)(gObj[aIndex].pInventoryMap+h*8+w) == 255)
				{
					blank = gObjInventoryRectCheck(aIndex,w,h,iwidth,iheight);

					if(blank == 254)
					{
						goto GOTO_EndFunc;
					}

					if(blank != 255)
					{
						if(gObjCheckSerial0ItemList(&item)!=0)
						{
							MsgOutput(aIndex,lMsg.Get(3354));
							return -1;
						}


						if(gObjInventorySearchSerialNumber(&gObj[aIndex],item.m_Number) == 0)
						{
							return -1;
						}

						gObj[aIndex].pInventory[blank] = item;

						gObjInventoryItemSet(aIndex,blank,gObj[aIndex].pInventory[blank].m_Type);
						return blank;
					}
				}
			}
		}
	}

GOTO_EndFunc:
	return -1;
}






BYTE gObjOnlyInventoryInsertItem(int aIndex, CItem item)
{
		int w,h,iwidth,iheight;
	BYTE blank = 0;

	if(item.GetSize((int&)iwidth,(int&)iheight)==0)
	{
		LogAdd(lMsg.Get(527),__FILE__,__LINE__);
		return -1;
	}

	for(h = 0; h < 8; h++)
	{
		for( w = 0; w < 8; w++)
		{
			if(*(BYTE*)(gObj[aIndex].pInventoryMap+h*8+w) == 255)
			{
				blank = gObjOnlyInventoryRectCheck(aIndex,w,h,iwidth,iheight);

				if(blank == 254)
				{
					goto NEXT1;
				}

				if(blank != 255)
				{
					if(gObjCheckSerial0ItemList(&item)!=0)
					{
						MsgOutput(aIndex,lMsg.Get(3354));
						return -1;
					}

					if(gObjInventorySearchSerialNumber(&gObj[aIndex],item.m_Number) == 0)
					{
						return -1;
					}

					gObj[aIndex].pInventory[blank] = item;

					gObjInventoryItemSet(aIndex,blank,gObj[aIndex].pInventory[blank].m_Type);
					return blank;
				}
			}
		}
	}
NEXT1:
	if(ReadConfig.IS_EXTENDED_INV(aIndex,1) == TRUE)
	{
		for(h = 8; h < 12; h++)
		{
			for( w = 0; w < 8; w++)
			{
				if(*(BYTE*)(gObj[aIndex].pInventoryMap+h*8+w) == 255)
				{
					blank = gObjOnlyInventoryRectCheck(aIndex,w,h,iwidth,iheight);

					if(blank == 254)
					{
						goto NEXT2;
					}

					if(blank != 255)
					{
						if(gObjCheckSerial0ItemList(&item)!=0)
						{
							MsgOutput(aIndex,lMsg.Get(3354));
							return -1;
						}

						if(gObjInventorySearchSerialNumber(&gObj[aIndex],item.m_Number) == 0)
						{
							return -1;
						}

						gObj[aIndex].pInventory[blank] = item;

						gObjInventoryItemSet(aIndex,blank,gObj[aIndex].pInventory[blank].m_Type);
						return blank;
					}
				}
			}
		}
	}
NEXT2:
	if(ReadConfig.IS_EXTENDED_INV(aIndex,2) == TRUE)
	{
		for(h = 12; h < 16; h++)
		{
			for( w = 0; w < 8; w++)
			{
				if(*(BYTE*)(gObj[aIndex].pInventoryMap+h*8+w) == 255)
				{
					blank = gObjOnlyInventoryRectCheck(aIndex,w,h,iwidth,iheight);

					if(blank == 254)
					{
						goto GOTO_EndFunc;
					}

					if(blank != 255)
					{
						if(gObjCheckSerial0ItemList(&item)!=0)
						{
							MsgOutput(aIndex,lMsg.Get(3354));
							return -1;
						}

						if(gObjInventorySearchSerialNumber(&gObj[aIndex],item.m_Number) == 0)
						{
							return -1;
						}

						gObj[aIndex].pInventory[blank] = item;

						gObjInventoryItemSet(aIndex,blank,gObj[aIndex].pInventory[blank].m_Type);
						return blank;
					}
				}
			}
		}
	}

GOTO_EndFunc:
	return -1;
}



BYTE gObjInventoryInsertItem(int aIndex, CMapItem * item)
{
	int w,h,iwidth,iheight;
	BYTE blank = 0;

	CItem copyitem;


	if(item->GetSize((int &)iwidth,(int &)iheight)==0)
	{
		LogAdd(lMsg.Get(527),__FILE__,__LINE__);
		return -1;
	}

	for(h = 0; h < 8; h++)
	{
		for(w = 0; w < 8; w++)
		{
			if(*(BYTE*)(gObj[aIndex].pInventoryMap+h*8+w) == 255)
			{
				blank = gObjOnlyInventoryRectCheck(aIndex,w,h,iwidth,iheight);
				if(blank == 254)
				{
					goto NEXT1;
				}
	
				if(blank != 255)
				{
					copyitem.m_Level = item->m_Level;
					copyitem.m_Durability = item->m_Durability;
	
					copyitem.Convert(item->m_Type,item->m_SkillOption,item->m_LuckOption,item->m_Z28Option,item->m_NewOption,item->m_SetOption,item->m_ItemOptionEx,CURRENT_DB_VERSION);
					copyitem.SetPetItemInfo(item->m_PetItem_Level,item->m_PetItem_Exp);
					copyitem.SetSocketItemInfo(item->m_ItemSlot1,item->m_ItemSlot2,item->m_ItemSlot3,item->m_ItemSlot4,item->m_ItemSlot5);
	
					copyitem.m_Number = item->m_Number;
	
					gObj[aIndex].pInventory[blank] = copyitem;
		
					gObjInventoryItemSet(aIndex,blank,gObj[aIndex].pInventory[blank].m_Type);
					return blank;
				}
			}
		}
	}
NEXT1:
	if(ReadConfig.IS_EXTENDED_INV(aIndex,1) == TRUE)
	{
		for(h = 8; h < 12; h++)
		{
			for(w = 0; w < 8; w++)
			{
				if(*(BYTE*)(gObj[aIndex].pInventoryMap+h*8+w) == 255)
				{
					blank = gObjOnlyInventoryRectCheck(aIndex,w,h,iwidth,iheight);
					if(blank == 254)
					{
						goto NEXT2;
					}
		
					if(blank != 255)
					{
						copyitem.m_Level = item->m_Level;
						copyitem.m_Durability = item->m_Durability;
		
						copyitem.Convert(item->m_Type,item->m_SkillOption,item->m_LuckOption,item->m_Z28Option,item->m_NewOption,item->m_SetOption,item->m_ItemOptionEx,CURRENT_DB_VERSION);
						copyitem.SetPetItemInfo(item->m_PetItem_Level,item->m_PetItem_Exp);
						copyitem.SetSocketItemInfo(item->m_ItemSlot1,item->m_ItemSlot2,item->m_ItemSlot3,item->m_ItemSlot4,item->m_ItemSlot5);
		
						copyitem.m_Number = item->m_Number;
		
						gObj[aIndex].pInventory[blank] = copyitem;
			
						gObjInventoryItemSet(aIndex,blank,gObj[aIndex].pInventory[blank].m_Type);
						return blank;
					}
				}
			}
		}
	}

NEXT2:
	if(ReadConfig.IS_EXTENDED_INV(aIndex,2) == TRUE)
	{
		for(h = 12; h < 16; h++)
		{
			for(w = 0; w < 8; w++)
			{
				if(*(BYTE*)(gObj[aIndex].pInventoryMap+h*8+w) == 255)
				{
					blank = gObjOnlyInventoryRectCheck(aIndex,w,h,iwidth,iheight);
					if(blank == 254)
					{
						goto GOTO_EndFunc;
					}
		
					if(blank != 255)
					{
						copyitem.m_Level = item->m_Level;
						copyitem.m_Durability = item->m_Durability;
		
						copyitem.Convert(item->m_Type,item->m_SkillOption,item->m_LuckOption,item->m_Z28Option,item->m_NewOption,item->m_SetOption,item->m_ItemOptionEx,CURRENT_DB_VERSION);
						copyitem.SetPetItemInfo(item->m_PetItem_Level,item->m_PetItem_Exp);
						copyitem.SetSocketItemInfo(item->m_ItemSlot1,item->m_ItemSlot2,item->m_ItemSlot3,item->m_ItemSlot4,item->m_ItemSlot5);
		
						copyitem.m_Number = item->m_Number;
		
						gObj[aIndex].pInventory[blank] = copyitem;
			
						gObjInventoryItemSet(aIndex,blank,gObj[aIndex].pInventory[blank].m_Type);
						return blank;
					}
				}
			}
		}
	}

GOTO_EndFunc:
	return -1;
}






BOOL gObjSearchItem(LPOBJ lpObj, int item_type, int add_dur, int nLevel)
{
	for(int n = INVETORY_WEAR_SIZE; n < ReadConfig.MAIN_INVENTORY_SIZE(lpObj->m_Index,false); n++)
	{
		if(lpObj->pInventory[n].IsItem() == 1)
		{
			if(lpObj->pInventory[n].m_Type == item_type)
			{
				if(lpObj->pInventory[n].m_Level == nLevel)
				{
					if((int)lpObj->pInventory[n].m_Durability + add_dur <= 3)
					{
						lpObj->pInventory[n].m_Durability += add_dur;
						GCItemDurSend(lpObj->m_Index,n,lpObj->pInventory[n].m_Durability,0);
						return true;
					}
				}
			}
		}
	}
	return false;
}






BOOL gObjSearchItemMinus(LPOBJ lpObj, int pos, int m_dur)
{
	int n = pos;
	if(n < 0 || n > (ReadConfig.MAIN_INVENTORY_SIZE(lpObj->m_Index,false)-1))
	{
		LogAdd("error-L4 : %s %d",__FILE__,__LINE__);
		return 1;
	}

	if(lpObj->pInventory[n].IsItem() == 1)
	{
		if((int)lpObj->pInventory[n].m_Durability - m_dur > 0)
		{
			lpObj->pInventory[n].m_Durability -= m_dur;
			GCItemDurSend(lpObj->m_Index,n,lpObj->pInventory[n].m_Durability,1);
			return 1;
		}
	}
	return 0;
}




BYTE gObjShopBuyInventoryInsertItem(int aIndex, CItem item)
{
	int w,h,iwidth,iheight;
	BYTE blank = 0;

	if(item.GetSize(iwidth,iheight)==0)
	{
		LogAdd(lMsg.Get(527),__FILE__,__LINE__);
		return -1;
	}

	for(h = 0; h < 8; h++)
	{
		for( w = 0; w < 8; w++)
		{
			if(*(BYTE*)(gObj[aIndex].pInventoryMap+h*8+w) == 255)
			{
				blank = gObjOnlyInventoryRectCheck(aIndex,w,h,iwidth,iheight);

				if(blank == 254)
				{
					goto NEXT1;
				}

				if(blank != 255)
				{
					gObj[aIndex].pInventory[blank] = item;

					gObj[aIndex].pInventory[blank].m_Number = 0;
					
					gObjInventoryItemSet(aIndex,blank,gObj[aIndex].pInventory[blank].m_Type);
					return blank;
				}
			}
		}
	}
NEXT1:
	if(ReadConfig.IS_EXTENDED_INV(aIndex,1) == TRUE)
	{	
		for(h = 8; h < 12; h++)
		{
			for( w = 0; w < 8; w++)
			{
				if(*(BYTE*)(gObj[aIndex].pInventoryMap+h*8+w) == 255)
				{
					blank = gObjOnlyInventoryRectCheck(aIndex,w,h,iwidth,iheight);

					if(blank == 254)
					{
						goto NEXT2;
					}

					if(blank != 255)
					{
						gObj[aIndex].pInventory[blank] = item;

						gObj[aIndex].pInventory[blank].m_Number = 0;
						
						gObjInventoryItemSet(aIndex,blank,gObj[aIndex].pInventory[blank].m_Type);
						return blank;
					}
				}
			}
		}
	}
NEXT2:
	if(ReadConfig.IS_EXTENDED_INV(aIndex,2) == TRUE)
	{	
		for(h = 12; h < 16; h++)
		{
			for( w = 0; w < 8; w++)
			{
				if(*(BYTE*)(gObj[aIndex].pInventoryMap+h*8+w) == 255)
				{
					blank = gObjOnlyInventoryRectCheck(aIndex,w,h,iwidth,iheight);

					if(blank == 254)
					{
						goto GOTO_EndFunc;
					}

					if(blank != 255)
					{
						gObj[aIndex].pInventory[blank] = item;

						gObj[aIndex].pInventory[blank].m_Number = 0;
						
						gObjInventoryItemSet(aIndex,blank,gObj[aIndex].pInventory[blank].m_Type);
						return blank;
					}
				}
			}
		}
	}
GOTO_EndFunc:
	return -1;
}


BYTE gObjShopBuyInventoryInsertItem(LPOBJ lpObj, int type, int index, int level, int iSerial, int iDur)
{
	int w,h,iwidth,iheight;
	BYTE blank = 0;
	CItem item;

	int item_type = ItemGetNumberMake(type,index);
	item.m_Level = level;
	item.m_Durability = iDur;

	item.Convert(item_type,0,0,0,0,0,0,CURRENT_DB_VERSION);

	if(item.GetSize((int &)iwidth,(int &)iheight)==0)
	{
		LogAdd(lMsg.Get(527),__FILE__,__LINE__);
		return -1;
	}

	for(h = 0; h < 8; h++)
	{
		for(w = 0; w < 8; w++)
		{
			if(*(BYTE*)(lpObj->pInventoryMap+h*8+w) == 255)
			{
				blank = gObjOnlyInventoryRectCheck(lpObj->m_Index,w,h,iwidth,iheight);

				if(blank == 254)
				{
					goto NEXT1;
				}

				if(blank != 255)
				{
					lpObj->pInventory[blank] = item;
					lpObj->pInventory[blank].m_Number = 0;

					gObjInventoryItemSet(lpObj->m_Index,blank,lpObj->pInventory[blank].m_Type);
					return blank;
				}
			}
		}
	}
NEXT1:
	if(ReadConfig.IS_EXTENDED_INV(lpObj->m_Index,1) == TRUE)
	{
		for(h = 8; h < 12; h++)
		{
			for(w = 0; w < 8; w++)
			{
				if(*(BYTE*)(lpObj->pInventoryMap+h*8+w) == 255)
				{
					blank = gObjOnlyInventoryRectCheck(lpObj->m_Index,w,h,iwidth,iheight);

					if(blank == 254)
					{
						goto NEXT2;
					}

					if(blank != 255)
					{
						lpObj->pInventory[blank] = item;
						lpObj->pInventory[blank].m_Number = 0;

						gObjInventoryItemSet(lpObj->m_Index,blank,lpObj->pInventory[blank].m_Type);
						return blank;
					}
				}
			}
		}
	}
NEXT2:
	if(ReadConfig.IS_EXTENDED_INV(lpObj->m_Index,2) == TRUE)
	{
		for(h = 12; h < 16; h++)
		{
			for(w = 0; w < 8; w++)
			{
				if(*(BYTE*)(lpObj->pInventoryMap+h*8+w) == 255)
				{
					blank = gObjOnlyInventoryRectCheck(lpObj->m_Index,w,h,iwidth,iheight);

					if(blank == 254)
					{
						goto GOTO_EndFunc;
					}

					if(blank != 255)
					{
						lpObj->pInventory[blank] = item;
						lpObj->pInventory[blank].m_Number = 0;

						gObjInventoryItemSet(lpObj->m_Index,blank,lpObj->pInventory[blank].m_Type);
						return blank;
					}
				}
			}
		}
	}
GOTO_EndFunc:
	return -1;
}



BYTE gObjInventoryInsertItemTemp(LPOBJ lpObj, CMapItem * Item)
{
	CItem item;
	int w,h,iwidth,iheight;
	BYTE blank = 0;

	if(Item->GetSize((int &)iwidth,(int &)iheight)==0)
	{
		LogAdd(lMsg.Get(527),__FILE__,__LINE__);
		return -1;
	}

	for(h = 0; h < 8; h++)
	{
		for(w = 0; w < 8; w++)
		{
			if(*(BYTE*)(lpObj->pInventoryMap+h*8+w) == 255)
			{
				blank = gObjInventoryRectCheck(lpObj->m_Index,w,h,iwidth,iheight);

				if(blank == 254)
				{
					goto NEXT1;
				}

				if(blank != 255)
				{
					return blank;
				}
			}
		}
	}
NEXT1:
	if(ReadConfig.IS_EXTENDED_INV(lpObj->m_Index,1) == TRUE)
	{
		for(h = 8; h < 12; h++)
		{
			for(w = 0; w < 8; w++)
			{
				if(*(BYTE*)(lpObj->pInventoryMap+h*8+w) == 255)
				{
					blank = gObjInventoryRectCheck(lpObj->m_Index,w,h,iwidth,iheight);

					if(blank == 254)
					{
						goto NEXT2;
					}

					if(blank != 255)
					{
						return blank;
					}
				}
			}
		}
	}
NEXT2:
	if(ReadConfig.IS_EXTENDED_INV(lpObj->m_Index,1) == TRUE)
	{
		for(h = 12; h < 16; h++)
		{
			for(w = 0; w < 8; w++)
			{
				if(*(BYTE*)(lpObj->pInventoryMap+h*8+w) == 255)
				{
					blank = gObjInventoryRectCheck(lpObj->m_Index,w,h,iwidth,iheight);

					if(blank == 254)
					{
						goto GOTO_EndFunc;
					}

					if(blank != 255)
					{
						return blank;
					}
				}
			}
		}
	}
GOTO_EndFunc:
	return -1;
}







BYTE gObjInventoryInsertItem(LPOBJ lpObj, int type, int index, int level)
{
	CItem item;
	int w,h,iwidth,iheight;
	BYTE blank = 0;

	int item_type = ItemGetNumberMake(type,index);

	item.m_Level = level;

	item.Convert(item_type,0,0,0,0,0,0,CURRENT_DB_VERSION);

	if(item.GetSize((int &)iwidth,(int &)iheight)==0)
	{
		LogAdd(lMsg.Get(527),__FILE__,__LINE__);
		return -1;
	}

	for(h = 0; h < 8; h++)
	{
		for( w = 0; w < 8; w++)
		{

			if(*(BYTE*)(lpObj->pInventoryMap+h*8+w) == 255)
			{
				blank = gObjOnlyInventoryRectCheck(lpObj->m_Index,w,h,iwidth,iheight);
	
				if(blank == 254)
				{
					goto NEXT1;
				}
	
				if(blank != 255)
				{
					lpObj->pInventory[blank] = item;
					lpObj->pInventory[blank].m_Number = 0;

					gObjInventoryItemSet(lpObj->m_Index,blank,lpObj->pInventory[blank].m_Type);
					return blank;
				}
			}
		}
	}
NEXT1:
	if(ReadConfig.IS_EXTENDED_INV(lpObj->m_Index,1) == TRUE)
	{
		for(h = 8; h < 12; h++)
		{
			for( w = 0; w < 8; w++)
			{

				if(*(BYTE*)(lpObj->pInventoryMap+h*8+w) == 255)
				{
					blank = gObjOnlyInventoryRectCheck(lpObj->m_Index,w,h,iwidth,iheight);
		
					if(blank == 254)
					{
						goto NEXT2;
					}
		
					if(blank != 255)
					{
						lpObj->pInventory[blank] = item;
						lpObj->pInventory[blank].m_Number = 0;

						gObjInventoryItemSet(lpObj->m_Index,blank,lpObj->pInventory[blank].m_Type);
						return blank;
					}
				}
			}
		}
	}
NEXT2:
	if(ReadConfig.IS_EXTENDED_INV(lpObj->m_Index,2) == TRUE)
	{
		for(h = 12; h < 16; h++)
		{
			for( w = 0; w < 8; w++)
			{

				if(*(BYTE*)(lpObj->pInventoryMap+h*8+w) == 255)
				{
					blank = gObjOnlyInventoryRectCheck(lpObj->m_Index,w,h,iwidth,iheight);
		
					if(blank == 254)
					{
						goto GOTO_EndFunc;
					}
		
					if(blank != 255)
					{
						lpObj->pInventory[blank] = item;
						lpObj->pInventory[blank].m_Number = 0;

						gObjInventoryItemSet(lpObj->m_Index,blank,lpObj->pInventory[blank].m_Type);
						return blank;
					}
				}
			}
		}
	}
GOTO_EndFunc:
	return -1;
}






BYTE gObjInventoryInsertItem(LPOBJ lpObj, int type, int index, int level, int iSerial, int iDur, int ItemSlot1, int ItemSlot2, int ItemSlot3, int ItemSlot4, int ItemSlot5)
{
	CItem item;
	int w,h,iwidth,iheight;
	BYTE blank = 0;

	int item_type = ItemGetNumberMake(type,index);
	item.m_Level = level;
	item.m_Durability = iDur;
	item.m_ItemSlot1 = ItemSlot1;
	item.m_ItemSlot2 = ItemSlot2;
	item.m_ItemSlot3 = ItemSlot3;
	item.m_ItemSlot4 = ItemSlot4;
	item.m_ItemSlot5 = ItemSlot5;

	item.Convert(item_type,0,0,0,0,0,0,CURRENT_DB_VERSION);

	if(item.GetSize((int &)iwidth,(int &)iheight)==0)
	{
		LogAdd(lMsg.Get(527),__FILE__,__LINE__);
		return -1;
	}

	for(h = 0; h < 8; h++)
	{
		for(w = 0; w < 8; w++)
		{
			if(*(BYTE*)(lpObj->pInventoryMap+h*8+w) == 255)
			{
				blank = gObjInventoryRectCheck(lpObj->m_Index,w,h,iwidth,iheight);

				if(blank == 254)
				{
					goto NEXT1;
				}

				if(blank != 255)
				{
					lpObj->pInventory[blank] = item;
					lpObj->pInventory[blank].m_Number = iSerial;

					gObjInventoryItemSet(lpObj->m_Index,blank,lpObj->pInventory[blank].m_Type);
					return blank;
				}
			}
		}
	}
NEXT1:
	if(ReadConfig.IS_EXTENDED_INV(lpObj->m_Index,1) == TRUE)
	{
		for(h = 8; h < 12; h++)
		{
			for(w = 0; w < 8; w++)
			{
				if(*(BYTE*)(lpObj->pInventoryMap+h*8+w) == 255)
				{
					blank = gObjInventoryRectCheck(lpObj->m_Index,w,h,iwidth,iheight);

					if(blank == 254)
					{
						goto NEXT2;
					}

					if(blank != 255)
					{
						lpObj->pInventory[blank] = item;
						lpObj->pInventory[blank].m_Number = iSerial;

						gObjInventoryItemSet(lpObj->m_Index,blank,lpObj->pInventory[blank].m_Type);
						return blank;
					}
				}
			}
		}
	}
NEXT2:
	if(ReadConfig.IS_EXTENDED_INV(lpObj->m_Index,2) == TRUE)
	{
		for(h = 12; h < 16; h++)
		{
			for(w = 0; w < 8; w++)
			{
				if(*(BYTE*)(lpObj->pInventoryMap+h*8+w) == 255)
				{
					blank = gObjInventoryRectCheck(lpObj->m_Index,w,h,iwidth,iheight);

					if(blank == 254)
					{
						goto GOTO_EndFunc;
					}

					if(blank != 255)
					{
						lpObj->pInventory[blank] = item;
						lpObj->pInventory[blank].m_Number = iSerial;

						gObjInventoryItemSet(lpObj->m_Index,blank,lpObj->pInventory[blank].m_Type);
						return blank;
					}
				}
			}
		}
	}
GOTO_EndFunc:
	return -1;
}






BYTE gObjMonsterInventoryInsertItem(LPOBJ lpObj, int type, int index, int level, int op1, int op2, int op3)
{
	CItem item;
	int w,h,iwidth,iheight;
	BYTE blank = 0;
	int item_type;

	item_type = ItemGetNumberMake(type,index);
	item.m_Level = level;

	if(type == 13 && index == 10)
	{
		item.m_Durability = rand()%100+100.0f;
	}

	item.Convert(item_type,op1,op2,op3,0,0,0,CURRENT_DB_VERSION);

	lpObj->pInventory[blank] = item;
	iwidth = 1;
	iheight = 1;

	for(h = 0; h < 8; h++)
	{
		for(w = 0; w <8; w++)
		{
			if(*(BYTE*)(lpObj->pInventoryMap+h*8+w) == 255)
			{
				blank = gObjInventoryRectCheck(lpObj->m_Index,w,h,iwidth,iheight);

				if(blank == 254)
				{
					goto NEXT1;
				}

				if(blank != 255)
				{
					lpObj->pInventory[blank] = item;
					//*lpObj->pInventoryCount += 1;

					gObjInventoryItemBoxSet(lpObj->m_Index,blank,iwidth,iwidth,lpObj->pInventory[blank].m_Type);
					return blank;
				}
			}
		}
	}
NEXT1:
	if(ReadConfig.IS_EXTENDED_INV(lpObj->m_Index,1) == TRUE)
	{
		for(h = 8; h < 12; h++)
		{
			for(w = 0; w <8; w++)
			{
				if(*(BYTE*)(lpObj->pInventoryMap+h*8+w) == 255)
				{
					blank = gObjInventoryRectCheck(lpObj->m_Index,w,h,iwidth,iheight);

					if(blank == 254)
					{
						goto NEXT2;
					}

					if(blank != 255)
					{
						lpObj->pInventory[blank] = item;
						//*lpObj->pInventoryCount += 1;

						gObjInventoryItemBoxSet(lpObj->m_Index,blank,iwidth,iwidth,lpObj->pInventory[blank].m_Type);
						return blank;
					}
				}
			}
		}
	}
NEXT2:
	if(ReadConfig.IS_EXTENDED_INV(lpObj->m_Index,2) == TRUE)
	{
		for(h = 12; h < 16; h++)
		{
			for(w = 0; w <8; w++)
			{
				if(*(BYTE*)(lpObj->pInventoryMap+h*8+w) == 255)
				{
					blank = gObjInventoryRectCheck(lpObj->m_Index,w,h,iwidth,iheight);

					if(blank == 254)
					{
						goto GOTO_EndFunc;
					}

					if(blank != 255)
					{
						lpObj->pInventory[blank] = item;
						//*lpObj->pInventoryCount += 1;

						gObjInventoryItemBoxSet(lpObj->m_Index,blank,iwidth,iwidth,lpObj->pInventory[blank].m_Type);
						return blank;
					}
				}
			}
		}
	}
GOTO_EndFunc:
	return -1;
}






BYTE gObjInventoryRectCheck(int aIndex, int sx, int sy, int width, int height)
{
	int x,y;
	int blank = 0;

	int InventoryRows = 12;
	if(ReadConfig.S6E2 == 1)
		InventoryRows = 28;

	if(sx + width > 8)
	{
		return -1;
	}

	if(sy + height > InventoryRows)
	{
		return -2;
	}

	if(ReadConfig.S6E2 == 1 && height > 1)
	{
		if(sy < 8 && sy+height > 8 )
		{
			return -1;
		}

		if(sy > 7 && sy < 12 && sy+height > 12)
		{
			return -1;
		}

		if(sy > 11 && sy < 16 && sy+height > 16)
		{
			return -1;
		}

		if(sy > 16 && sy < 20 && sy+height > 20)
		{
			return -1;
		}

		if(sy > 20 && sy < 24 && sy+height > 24)
		{
			return -1;
		}

		if(sy > 24 && sy < 28 && sy+height > 28)
		{
			return -1;
		}
	}

	int xx,yy;

	for(y = 0; y < height; y ++)
	{
		yy = sy+y;

		for(x = 0; x < width; x++)
		{
			xx = sx + x;

			if(ExtentCheck(xx,yy,8,InventoryRows)==1)
			{
				if(*(BYTE*)(gObj[aIndex].pInventoryMap+(sy+y)*8+(sx+x)) != 255)
				{
					blank += 1;
					return -1;
				}
			}
			else
			{
				LogAdd("error : %s %d",__FILE__,__LINE__);
				return -1;
			}
		}
	}

	if(blank == 0)
	{
		return sx+sy*8+12;
	}
	return  -1;
}






BYTE gObjOnlyInventoryRectCheck(int aIndex, int sx, int sy, int width, int height)
{
	int x,y;
	int blank = 0;
	int InventoryRows = 12;
	InventoryRows = ReadConfig.GETHEIGHT(aIndex);

	if(sx + width > 8)
	{
		return -1;
	}

	if(sy + height > InventoryRows)
	{
		return -2;
	}

	if(ReadConfig.S6E2 == 1 && height > 1)
	{
		if(sy < 8 && sy+height > 8 )
		{
			return -1;
		}

		if(sy > 7 && sy < 12 && sy+height > 12)
		{
			return -1;
		}

		if(sy > 11 && sy < 16 && sy+height > 16)
		{
			return -1;
		}

		if(sy > 16 && sy < 20 && sy+height > 20)
		{
			return -1;
		}

		if(sy > 20 && sy < 24 && sy+height > 24)
		{
			return -1;
		}
	}

	int xx,yy;
	for(y = 0; y < height; y ++)
	{

		yy = sy+y;

		for(x = 0; x < width; x++)
		{
			xx = sx + x;

			if(ExtentCheck(xx,yy,8,InventoryRows)==1)
			{
				if(*(BYTE*)(gObj[aIndex].pInventoryMap+(sy+y)*8+(sx+x)) != 255)
				{
					blank += 1;
					return -1;
				}
			}
			else
			{
				LogAdd("error : %s %d",__FILE__,__LINE__);
				return -1;
			}
		}
	}

	if(blank == 0)
	{
		return sx+sy*8+12;
	}
	return  -1;
}


BOOL CheckInventoryEmptySpace(LPOBJ lpObj, int iItemHeight, int iItemWidth)
{
	int h=0;
	int w=0;
	BYTE blank = 0;
	
	for(h=0;h<8;h++)
	{
		for(w=0;w<8;w++)
		{
			if(*(BYTE*)(lpObj->pInventoryMap+(h)*8+(w)) == 255)
			{
				blank = gObjOnlyInventoryRectCheck(lpObj->m_Index, w, h, iItemWidth, iItemHeight);

				if ( blank == 0xFE )
					return FALSE;

				if ( blank != 0xFF )
					return TRUE;
			}
		}
	}

	return FALSE;
}



BOOL gObjIsItemPut(LPOBJ lpObj, CItem * lpItem, int pos )
{
	if(lpItem->m_TwoHand == 1)
	{
		if(pos == 0 || pos == 1)
		{
			if(lpObj->pInventory[0].IsItem() == 1)
			{
				if(lpObj->pInventory[0].m_Type == ITEMGET(4,7) || lpObj->pInventory[0].m_Type == ITEMGET(4,15))
				{
					
				}
				else
				{
					return false;
				}
			}

			if(lpObj->pInventory[1].IsItem() == 1)
			{
				if(lpObj->pInventory[1].m_Type == ITEMGET(4,7) || lpObj->pInventory[1].m_Type == ITEMGET(4,15))
				{
					
				}
				else
				{
					return false;
				}
			}
		}
	}

	if(pos == 1)
	{
		if(lpObj->pInventory[0].IsItem() == 1)
		{
			if(lpItem->m_Type == ITEMGET(4,7) || lpItem->m_Type == ITEMGET(4,15))
			{
					
			}
			else
			{
				if(lpObj->pInventory[0].m_TwoHand)
				{
					return false;
				}
			}
		}
	}

	if(lpItem->m_Part != pos)
	{
		if(pos == 1 || pos == 0)
		{
			if(lpItem->m_Part == 1 || lpItem->m_Part == 0)
			{

			}
			else
			{
				return false;
			}
		}
		else if(pos == 11)
		{
			if(lpItem->m_Part == 10)
			{

			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}

	if ((lpItem->GetNumber() != 0xFFFFFFFF && ReadConfig.ItemTradeFFFFBlock == TRUE) &&
		(lpItem->GetNumber() != 0xFFFFFFFE && ReadConfig.ItemTradeFFFEBlock == TRUE) &&
		(lpItem->GetNumber() != 0xFFFFFFFD && ReadConfig.ItemTradeFFFDBlock == TRUE) &&
		(lpItem->GetNumber() != 0xFFFFFFFC && ReadConfig.ItemTradeFFFCBlock == TRUE) )
	{
		if((lpObj->Strength + lpObj->AddStrength) < (lpItem->m_RequireStrength - lpItem->m_HJOpStrength))
		{
			return false;
		}

		if((lpObj->Dexterity + lpObj->AddDexterity) < (lpItem->m_RequireDexterity - lpItem->m_HJOpDexterity))
		{
			return false;
		}

		if(lpObj->Level < lpItem->m_RequireLevel)
		{
			return false;
		}

		if(lpItem->m_Type >= 0 && lpItem->m_Type < ITEMGET(12,0))
		{
			if((lpObj->Vitality + lpObj->AddVitality) < lpItem->m_RequireVitality)
			{
				return false;
			}

			if((lpObj->Energy + lpObj->AddEnergy) < lpItem->m_RequireEnergy)
			{
				return false;
			}
		}

		if((lpObj->Leadership + lpObj->AddLeadership) < lpItem->m_RequireLeaderShip)
		{
			return false;
		}
	}

	if(lpItem->m_Type >= ITEMGET(12,0) && lpItem->m_Type <= ITEMGET(12,6))
	{
		if(lpObj->Level < lpItem->m_RequireLevel)
		{
			return false;
		}
	}
	else if(lpItem->m_Type >= ITEMGET(12,130) && lpItem->m_Type <= ITEMGET(12,135))
	{
		if(lpObj->Level < lpItem->m_RequireLevel)
		{
			return false;
		}
	}
	else if(lpItem->m_Type >= ITEMGET(12,36) && lpItem->m_Type <= ITEMGET(12,43))
	{
		if(lpObj->Level < lpItem->m_RequireLevel)
		{
			return false;
		}
	}
	else if(lpItem->m_Type >= ITEMGET(12,49) && lpItem->m_Type <= ITEMGET(12,50))
	{
		if(lpObj->Level < lpItem->m_RequireLevel)
		{
			return false;
		}
	}
	
	if(lpItem->m_Type >= ITEMGET(12,130) && lpItem->m_Type <= ITEMGET(12,135))
	{
		if(lpObj->Level < lpItem->m_RequireLevel)
		{
			return false;
		}
	}
#if (CRYSTAL_EDITION == 1)
	else if (lpItem->m_Type >= ITEMGET(12,200) && lpItem->m_Type <= ITEMGET(12,254))
	{
		if(lpObj->Level < lpItem->m_RequireLevel)
		{
			return false;
		}
	}
#endif
	else if(lpItem->m_Type >= ITEMGET(13,0) && lpItem->m_Type <= ITEMGET(13,15))
	{
		if(lpItem->m_Type == ITEMGET(13,10))
		{
			if(lpItem->m_Level <= 2)
			{
				if(lpObj->Level < 20)
				{
					return false;
				}
			}
			else
			{
				if(lpObj->Level < 50)
				{
					return false;
				}
			}
		}
		else if(lpItem->m_Type == ITEMGET(13,2))
		{
			if(lpObj->MapNumber == MAP_INDEX_ATLANS || lpObj->Level < lpItem->m_RequireLevel)
			{
				return false;
			}
		}
		else if(lpItem->m_Type == ITEMGET(13,3))
		{
			if(lpObj->MapNumber == MAP_INDEX_ATLANS || lpObj->Level < lpItem->m_RequireLevel)
			{
				return false;
			}
		}
		else
		{
			if(lpObj->Level < lpItem->m_RequireLevel)
			{
				return false;
			}
		}
	}

	if(lpItem->m_Type == ITEMGET(13,20) && lpItem->m_Level == 0)
	{
		int count = gObjGetItemCountInEquipment(lpObj->m_Index,13,20,0);

		if(count != 0)
		{
			return false;
		}
	}

	if(lpItem->m_Type == ITEMGET(13,107) && lpItem->m_Level == 0)
	{
		int count = gObjGetItemCountInEquipment(lpObj->m_Index,13,107,0);

		if(count != 0)
		{
			return false;
		}
	}

	if(lpItem->m_Type == ITEMGET(13,10))
	{
		if(CC_MAP_RANGE(lpObj->MapNumber))
		{
			return false;
		}
	}

	if(lpItem->m_Type == ITEMGET(13,37))
	{
		if(CC_MAP_RANGE(lpObj->MapNumber))
		{
			return false;
		}
	}

	if(lpItem->m_Type == ITEMGET(13,38))
	{
		int count = gObjGetItemCountInEquipment(lpObj->m_Index,13,38,0);

		if(count != 0)
		{
			return false;
		}
	}

	if(lpItem->m_Type == ITEMGET(13,39))
	{
		if(CC_MAP_RANGE(lpObj->MapNumber))
		{
			return false;
		}

		int count = 0;

		for(int n = 0; n < 7; n++)
		{
			count = gObjGetItemCountInEquipment(lpObj->m_Index,13,10,n);

			if(count != 0)
			{
				return false;
			}
		}

		count = gObjGetItemCountInEquipment(lpObj->m_Index,13,39,0);

		if(count != 0)
		{
			return false;
		}
	}

	if(lpItem->m_Type == ITEMGET(13,10))
	{
		int count = 0;

		for(int n = 0; n < 7; n ++)
		{
			count = gObjGetItemCountInEquipment(lpObj->m_Index,13,10,n);

			if(count != 0)
			{
				return false;
			}
		}

		count = gObjGetItemCountInEquipment(lpObj->m_Index,13,39,0);

		if(count != 0)
		{
			return false;
		}
	}
	else if(lpItem->m_Type >= ITEMGET(14,00))
	{
		return false;
	}

	return true;
}






BYTE gObjWerehouseRectCheck(int aIndex, int sx, int sy, int width, int height)
{
	int x,y;
	int blank = 0;
	int WarehouseRows = 15;
	if (ReadConfig.S6E2 == 1)
		WarehouseRows = 30;

	if(sx + width > 8)
	{
		return -1;
	}

	if(sx + width < 0)
	{
		return -1;
	}

	if(sy + height > WarehouseRows)
	{
		return -1;
	}

	if(sy + height < 0)
	{
		return -1;
	}

	int xx,yy;

	for(y = 0; y < height; y ++)
	{

		yy = sy+y;

		for(x = 0; x < width; x++)
		{
			xx = sx + x;

			if (ExtentCheck(xx,yy,8,WarehouseRows)==1)
			{
				if(*(BYTE*)(gObj[aIndex].pWarehouseMap+(sy+y)*8+(sx+x)) != 255)
				{
					blank += 1;
					return -1;
				}
			}
			else
			{
				LogAdd("error : %s %d",__FILE__,__LINE__);
				return -1;
			}
		}
	}

	if(blank == 0)
	{
		return sx+sy*8;
	}
	return  -1;
}






BYTE gObjMapRectCheck(BYTE * lpMapBuf, int sx, int sy, int ex, int ey, int width, int height)
{
	int x,y;
	int blank = 0;

	if(sx + width > ex || sx + width < 0)
	{
		return -1;
	}

	if(sy + height > ey || sy + height < 0)
	{
		return -1;
	}

	int xx,yy;

	for(y = 0; y < height; y++)
	{
		yy = sy + y;

		for(x = 0; x < width; x++)
		{
			xx = sx + x;

			//DaRKav removed, ey never becomes 15 in this function!!!
			//if(ReadConfig.S6E2 == 1 && ex == 8 && ey == 15)
			//{
			//	ey = ey*2;
			//}
			if(ExtentCheck(xx,yy,ex,ey)==1)
			{
				if(*(BYTE*)(lpMapBuf + (sy + y)*ex + (sx+x)) != 255)
				{
					blank += 1;
					return -1;
				}
			}
			else
			{
				LogAdd("error : %s %d",__FILE__,__LINE__);
				return -1;
			}
		}
	}

	if(blank == 0)
	{
		return sy*ex+sx;
	}

	return -1;
}






void gObjWarehouseItemBoxSet(int aIndex, int itempos, int xl, int yl, BYTE set_byte)
{
	int itemposx = (itempos)%8;
	int itemposy = (itempos)/8;

	int WarehouseRows = 15;
	if (ReadConfig.S6E2 == 1)
		WarehouseRows = 30;

	int xx,yy;

	for(int y = 0; y < yl; y ++)
	{
		yy = itemposy + y;

		for(int x = 0; x < xl; x++)
		{
			xx = itemposx + x;

			if (ExtentCheck(xx,yy,8,WarehouseRows)==1)
			{
				*(BYTE*)(gObj[aIndex].pWarehouseMap + (itemposy + y)*8+(itemposx + x)) = set_byte;
			}
			else
			{
				LogAdd("error : %d/%d  %s %d",xx,yy,__FILE__,__LINE__);
				return;
			}
		}
	}
}






void gObjWarehouseItemSet(int aIndex, int itempos, BYTE set_byte)
{
	int width;
	int height;
	if(itempos < 0)
	{
		return;
	}
	else if(itempos > (ReadConfig.WAREHOUSE_SIZE(aIndex,false)-1))
	{
		return;
	}

	if(gObj[aIndex].pWarehouse[itempos].GetSize((int&)width,(int &)height)==0)
	{
		LogAdd(lMsg.Get(527),__FILE__,__LINE__);
		return;
	}

	gObjWarehouseItemBoxSet(aIndex,itempos,width,height,set_byte);
}






void gObjChaosItemBoxSet(int aIndex, int itempos, int xl, int yl, BYTE set_byte)
{
	int itemposx = (itempos)%8;
	int itemposy = (itempos)/8;

	int ChaosBoxRows = 4;

	int xx,yy;

	for(int y = 0; y < yl; y ++)
	{
		yy = itemposy + y;

		for(int x = 0; x < xl; x++)
		{
			xx = itemposx + x;

			if(ExtentCheck(xx,yy,8,ChaosBoxRows)==1)
			{
				*(BYTE*)(gObj[aIndex].pChaosBoxMap + (itemposy + y)*8+(itemposx + x)) = set_byte;
			}
			else
			{
				LogAdd("error : %d/%d  %s %d",xx,yy,__FILE__,__LINE__);
				return;
			}
		}
	}
}






void gObjChaosItemSet(int aIndex, int itempos, BYTE set_byte)
{
	int width;
	int height;
	if(itempos < 0)
	{
		return;
	}
	else if(itempos > (CHAOS_BOX_SIZE-1))
	{
		return;
	}

	if(gObj[aIndex].pChaosBox[itempos].GetSize((int&)width,(int &)height)==0)
	{
		LogAdd(lMsg.Get(527),__FILE__,__LINE__);
		return;
	}

	gObjChaosItemBoxSet(aIndex,itempos,width,height,set_byte);
}






BYTE gObjChaosBoxInsertItemPos(int aIndex, CItem item, int pos, int source)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return -1;
	}

	LPOBJ lpObj;
	BYTE TempMap[CHAOS_BOX_MAP_SIZE];

	int w,h,iwidth,iheight;

	int blank,useClass;
	int ChaosBoxRows = 4;

	if(pos < 0 || pos > (CHAOS_BOX_SIZE-1))
	{
		return -1;
	}

	useClass = 0;

	lpObj = &gObj[aIndex];

	if(lpObj->pChaosBox[pos].IsItem() == 1)
	{
		return -1;
	}

	if(item.IsItem() == 0)
	{
		return -1;
	}

	w = pos%8;
	h = pos/8;

	if(ExtentCheck(w,h,8,ChaosBoxRows) == 0)
	{
		return -1;
	}

	item.GetSize((int &)iwidth,(int &)iheight);

	if(source >= 0)
	{
		memcpy(TempMap,lpObj->pChaosBoxMap,CHAOS_BOX_MAP_SIZE);
		gObjChaosItemBoxSet(lpObj->m_Index,source,iwidth,iheight,255);
	}

	if(*(BYTE*)(lpObj->pChaosBoxMap + h * 8 + w) != 255)
	{
		if(source >= 0)
		{
			memcpy(lpObj->pChaosBoxMap,TempMap,CHAOS_BOX_MAP_SIZE);
		}
		return -1;
	}

	blank = gObjMapRectCheck(lpObj->pChaosBoxMap,w,h,8,4,iwidth,iheight);

	if(blank == 255)
	{
		if(source >= 0)
		{
			memcpy(lpObj->pChaosBoxMap,TempMap,CHAOS_BOX_MAP_SIZE);
		}
		return -1;
	}

	lpObj->pChaosBox[pos] = item;
	return pos;
}






BYTE gObjWarehouseInsertItemPos(int aIndex, CItem item, int pos, int source)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return -1;
	}

	LPOBJ lpObj;

	BYTE TempMap[WAREHOUSE_S6E2_SIZE];

	int w,h,iwidth,iheight;

	int blank,useClass;

	if(pos < 0 || pos > (ReadConfig.WAREHOUSE_SIZE(aIndex,false)-1))
	{
		return -1;
	}

	useClass = 0;

	lpObj = &gObj[aIndex];

	if(lpObj->pWarehouse[pos].IsItem() == 1)
	{
		return -1;
	}

	if(item.IsItem() == 0)
	{
		return -1;
	}

	w = pos%8;
	h = pos/8;

	int WarehouseRows = 15;
	if (ReadConfig.S6E2 == 1)
		WarehouseRows = 30;

	if(ExtentCheck(w,h,8,WarehouseRows) == 0)
	{
		return -1;
	}

	item.GetSize((int &)iwidth,(int &)iheight);

	if(source >= 0)
	{
		memcpy(TempMap,lpObj->pWarehouseMap,ReadConfig.WAREHOUSE_SIZE(aIndex,false));
		gObjWarehouseItemBoxSet(lpObj->m_Index,source,iwidth,iheight,255);
	}

	if(*(BYTE*)(lpObj->pWarehouseMap + h * 8 + w) != 255)
	{
		if(source >= 0)
		{
			memcpy(lpObj->pWarehouseMap,TempMap,ReadConfig.WAREHOUSE_SIZE(aIndex,false));
		}
		return -1;
	}

	blank = gObjWerehouseRectCheck(lpObj->m_Index,w,h,iwidth,iheight);

	if(blank == 255)
	{
		if(source >= 0)
		{
			memcpy(lpObj->pWarehouseMap,TempMap,ReadConfig.WAREHOUSE_SIZE(aIndex,false));
		}
		return -1;
	}

	lpObj->pWarehouse[pos] = item;
	return pos;
}






BYTE gObjInventoryInsertItemPos(int aIndex, CItem item, int pos, BOOL RequestCheck)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return -1;
	}

	if(pos < 0 || pos > ReadConfig.INVENTORY_SIZE(aIndex,false))
	{
		return -1;
	}

	LPOBJ lpObj;
	BYTE TempInventoryMap[MAIN_INVENTORY_NORMAL_SIZE + (MAX_EXTENDED_INV*EXTENDED_INV_SIZE) + PSHOP_SIZE];

	int w,h,iwidth,iheight;

	int blank,useClass = 0;

	lpObj = &gObj[aIndex];

	if(lpObj->pInventory[pos].IsItem() == 1)
	{
		return -1;
	}

	if(item.IsItem() == 0)
	{
		return -1;
	}

	if(pos < 12)
	{
#if (PACK_EDITION>=3)
		if(lpObj->IsBot == 0)
		{
#endif
			useClass = item.IsClass(lpObj->Class,lpObj->ChangeUP);

			if(useClass == 0)
			{
				LogAdd(lMsg.Get(528));
				return -1;
			}
#if (PACK_EDITION>=3)
		}
#endif

		if(RequestCheck != 0)
		{
			if(gObjIsItemPut(lpObj,&item,pos) == 0)
			{
				return -1;
			}
		}
	}
	else
	{
		w = (pos - 12)%8;
		h = (pos - 12)/8;

		int InventoryRows = 12;
		if(ReadConfig.S6E2 == 1)
		{
			//DaRKav Fix PSHOP
			InventoryRows = 28;
		}

		if(ExtentCheck(w,h,8,InventoryRows)==0)
		{
			return -1;
		}

		memcpy(TempInventoryMap,lpObj->pInventoryMap,ReadConfig.INVENTORY_MAP_SIZE(lpObj->m_Index,false));
		item.GetSize((int &)iwidth,(int &)iheight);

		if(*(BYTE*)(lpObj->pInventoryMap + h * 8 + w) != 255 )
		{
			memcpy(lpObj->pInventoryMap,TempInventoryMap,ReadConfig.INVENTORY_MAP_SIZE(lpObj->m_Index,false));
			return -1;
		}

		blank = gObjInventoryRectCheck(lpObj->m_Index,w,h,iwidth,iheight);

		if(blank >= 254)
		{
			memcpy(lpObj->pInventoryMap,TempInventoryMap,ReadConfig.INVENTORY_MAP_SIZE(lpObj->m_Index,false));
			return false;
		}
	}

	lpObj->pInventory[pos] = item;

	if(pos > 11)
	{
		gObjInventoryItemSet(aIndex,pos,1);
	}

	if(pos < 12)
	{
		CItem * lpItem = &lpObj->pInventory[pos];

		if(lpItem->m_SkillOption)
		{
			int result = gObjWeaponMagicAdd(lpObj,lpItem->m_Special[0],lpItem->m_Level);

			if(result >= 0)
			{
				GCMagicListOneSend(lpObj->m_Index,result,lpItem->m_Special[0],lpItem->m_Level,0,0);
			}
		}
	}
	return pos;
}






BOOL gObjInventorySearchSerialNumber(LPOBJ lpObj, DWORD serial)
{
	int s_num;
	int count = 0;

	if(gItemSerialCheck == 0)
	{
		return true;
	}

	if(serial == 0)
	{
		return true;
	}

	if (serial >= 0xFFFFFFF0 && serial <= 0xFFFFFFFF)
	{
		return true;
	}

	for(int n = 0; n < ReadConfig.INVENTORY_SIZE(lpObj->m_Index,false); n++)
	{
		s_num = lpObj->pInventory[n].GetNumber();

#if(ENABLE_CSHOP == 1)
		if(MuItemShop.IsCShopItem(lpObj->m_Index,n) == true)
		{
			continue;
		}
#endif

		if(s_num != 0 && s_num == serial)
		{
			count++;
		}
	}

	if(count <= 1)
	{
		return true;
	}

	bool Enter = false;

	for(int n = 0; n < ReadConfig.INVENTORY_SIZE(lpObj->m_Index,false); n++)
	{
		s_num = lpObj->pInventory[n].GetNumber();

		if(s_num != 0 && s_num == serial)
		{
			if(ReadConfig.AHLog == TRUE)
			{
				ANTI_HACK_LOG.Output(lMsg.Get(529),lpObj->AccountID,lpObj->Name,lpObj->pInventory[n].GetName(),n,s_num);
			}
			GCServerMsgStringSend(lMsg.Get(1102),lpObj->m_Index,1);
#if (PACK_EDITION<3 || CRYSTAL_EDITION == 0)
			gObjUserKill(lpObj->m_Index);
			break;
#else
			Enter = true;
			if(CrystalWall.IsHaveSameSerialDeleteItems >= 2)
			{
				gObjInventoryDeleteItem(lpObj->m_Index, n);
				GCInventoryItemDeleteSend(lpObj->m_Index, n, 1);
			}
#endif
		}
	}
#if (PACK_EDITION>=3 && CRYSTAL_EDITION == 1)
	if(Enter == true)
	{
		if(CrystalWall.IsHaveSameSerialKickPlayer == 1)
			gObjUserKill(lpObj->m_Index);
	}
#endif

	return false;
}


BOOL gObjInventorySearchSerialNumber2(LPOBJ lpObj, DWORD serial)
{
	int s_num;
	int count = 0;

	if(gItemSerialCheck == 0)
	{
		return true;
	}

	if(serial == 0)
	{
		return true;
	}

	if (serial >= 0xFFFFFFF0 && serial <= 0xFFFFFFFF)
	{
		return true;
	}

	for(int n = 0; n < ReadConfig.INVENTORY_SIZE(lpObj->m_Index,false); n++)
	{
		s_num = lpObj->pInventory[n].GetNumber();

#if(ENABLE_CSHOP == 1)
		if(MuItemShop.IsCShopItem(lpObj->m_Index,n) == true)
		{
			continue;
		}
#endif

		if(s_num != 0 && s_num == serial)
		{
			count++;
		}
	}

	if(count < 1)
	{
		return true;
	}

	bool Enter = false;

	for(int n = 0; n < ReadConfig.INVENTORY_SIZE(lpObj->m_Index,false); n++)
	{
		s_num = lpObj->pInventory[n].GetNumber();

		if(s_num != 0 && s_num == serial)
		{
			if(ReadConfig.AHLog == TRUE)
			{
				ANTI_HACK_LOG.Output(lMsg.Get(529),lpObj->AccountID,lpObj->Name,lpObj->pInventory[n].GetName(),n,s_num);
			}
			GCServerMsgStringSend(lMsg.Get(1102),lpObj->m_Index,1);
#if (PACK_EDITION<3 || CRYSTAL_EDITION == 0)
			gObjUserKill(lpObj->m_Index);
			break;
#else
			Enter = true;
			if(CrystalWall.IsHaveSameSerialDeleteItems >= 2)
			{
				gObjInventoryDeleteItem(lpObj->m_Index, n);
				GCInventoryItemDeleteSend(lpObj->m_Index, n, 1);
			}
#endif
		}
	}
#if (PACK_EDITION>=3 && CRYSTAL_EDITION == 1)
	if(Enter == true)
	{
		if(CrystalWall.IsHaveSameSerialKickPlayer == 1)
			gObjUserKill(lpObj->m_Index);
	}
#endif

	return false;
}


BOOL gObjWarehouseSearchSerialNumber(LPOBJ lpObj, DWORD serial)
{
	int s_num;
	int count = 0;
	//return true;

	if(gItemSerialCheck == 0)
	{
		return true;
	}

	if(serial == 0)
	{
		return true;
	}

	if(serial >= 0xFFFFFFF0 && serial <= 0xFFFFFFFF)
	{
		return true;
	}

	for(int n = 0; n < ReadConfig.WAREHOUSE_SIZE(lpObj->m_Index,false); n++)
	{
		s_num = lpObj->pWarehouse[n].GetNumber();

#if(ENABLE_CSHOP == 1)
		if(MuItemShop.IsCShopItem(lpObj->m_Index,n) == true)
		{
			continue;
		}
#endif
		if(s_num != 0 && s_num == serial)
		{
			count++;
		}
	}

	if(count <= 1)
	{
		return true;
	}
	bool Enter = false;

	for(int n = 0; n < ReadConfig.WAREHOUSE_SIZE(lpObj->m_Index,false); n++)
	{
		s_num = lpObj->pWarehouse[n].GetNumber();

		if(s_num != 0 && s_num == serial)
		{
			if(ReadConfig.AHLog == TRUE)
			{
				ANTI_HACK_LOG.Output(lMsg.Get(530),lpObj->AccountID,lpObj->Name,lpObj->pWarehouse[n].GetName(),n,s_num);
			}
			GCServerMsgStringSend(lMsg.Get(1102),lpObj->m_Index,1);
#if (PACK_EDITION<3 || CRYSTAL_EDITION == 0)
			gObjUserKill(lpObj->m_Index);
			break;
#else
			Enter = true;
			if(CrystalWall.IsHaveSameSerialDeleteItems == 1 || CrystalWall.IsHaveSameSerialDeleteItems == 3)
			{
				gObjWarehouseDeleteItem(lpObj->m_Index, n);
			}
#endif
		}
	}

#if (PACK_EDITION>=3 && CRYSTAL_EDITION == 1)
	if(Enter == true)
	{
		if(CrystalWall.IsHaveSameSerialDeleteItems == 1 || CrystalWall.IsHaveSameSerialDeleteItems == 3)
		{
			CGWarehouseUseEnd(lpObj->m_Index);
		}
		if(CrystalWall.IsHaveSameSerialKickPlayer == 1)
			gObjUserKill(lpObj->m_Index);
	}
#endif

	return false;
}


BOOL gObjWarehouseSearchSerialNumber2(LPOBJ lpObj, DWORD serial, bool Disconnect)
{
	int s_num;
	int count = 0;
	//return true;

	if(gItemSerialCheck == 0)
	{
		return true;
	}

	if(serial == 0)
	{
		return true;
	}

	if(serial >= 0xFFFFFFF0 && serial <= 0xFFFFFFFF)
	{
		return true;
	}

	for(int n = 0; n < ReadConfig.WAREHOUSE_SIZE(lpObj->m_Index,false); n++)
	{
		s_num = lpObj->pWarehouse[n].GetNumber();

#if(ENABLE_CSHOP == 1)
		if(MuItemShop.IsCShopItem(lpObj->m_Index,n) == true)
		{
			continue;
		}
#endif
		if(s_num != 0 && s_num == serial)
		{
			count++;
		}
	}

	if(count < 1)
	{
		return true;
	}
	bool Enter = false;

	for(int n = 0; n < ReadConfig.WAREHOUSE_SIZE(lpObj->m_Index,false); n++)
	{
		s_num = lpObj->pWarehouse[n].GetNumber();

		if(s_num != 0 && s_num == serial)
		{
			if(ReadConfig.AHLog == TRUE)
			{
				ANTI_HACK_LOG.Output(lMsg.Get(530),lpObj->AccountID,lpObj->Name,lpObj->pWarehouse[n].GetName(),n,s_num);
			}
			GCServerMsgStringSend(lMsg.Get(1102),lpObj->m_Index,1);
#if (PACK_EDITION<3 || CRYSTAL_EDITION == 0)
			gObjUserKill(lpObj->m_Index);
			break;
#else
			Enter = true;
			if(CrystalWall.IsHaveSameSerialDeleteItems == 1 || CrystalWall.IsHaveSameSerialDeleteItems == 3)
			{
				gObjWarehouseDeleteItem(lpObj->m_Index, n);
			}
#endif
		}
	}

#if (PACK_EDITION>=3 && CRYSTAL_EDITION == 1)
	if(Enter == true)
	{
		if(CrystalWall.IsHaveSameSerialDeleteItems == 1 || CrystalWall.IsHaveSameSerialDeleteItems == 3)
		{
			CGWarehouseUseEnd(lpObj->m_Index);
		}
		if(Disconnect == true)
		{
			if(CrystalWall.IsHaveSameSerialKickPlayer == 1)
				gObjUserKill(lpObj->m_Index);
		}
	}
#endif

	return false;
}






BYTE gObjInventoryMoveItem(int aIndex, unsigned char source, unsigned char target, int& durSsend, int& durTsend, unsigned char sFlag, unsigned char tFlag, unsigned char* siteminfo)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return -1;
	}

	LPOBJ lpObj;
	BYTE TempInventoryMap[MAIN_INVENTORY_NORMAL_SIZE + (MAX_EXTENDED_INV*EXTENDED_INV_SIZE) + PSHOP_SIZE];
	int w,h;
	int iwidth,iheight;
	int blank;
	int s_num;

	CItem * sitem;
	CItem * titem;

	int bPersonalShopTrans = 0;
	int bSourceIsPShop = 0;

	durSsend = 0;
	durTsend = 0;

	lpObj = &gObj[aIndex];

	int useClass = 0;
	__try
	{
		if(sFlag == 2 || tFlag == 2)
		{
			if(lpObj->m_IfState.type != 6)
			{
				LogAdd(lMsg.Get(531),lpObj->AccountID,lpObj->Name);
				return -1;
			}

			if(lpObj->m_IfState.state == 0)
			{
				LogAdd(lMsg.Get(531),lpObj->AccountID,lpObj->Name);
				return -1;
			}
		}

		switch(sFlag)
		{
		case 0:
			if(source < 0 || source > (ReadConfig.MAIN_INVENTORY_SIZE(lpObj->m_Index,true /*false*/)-1))
			{
				LogAdd("error-L1 : %s %d",__FILE__,__LINE__);
				return -1;
			}

			if(lpObj->pInventory[source].IsItem() == 0)
			{
				LogAdd(lMsg.Get(532),lpObj->AccountID,lpObj->Name,__LINE__);
				return -1;
			}

			sitem = &lpObj->pInventory[source];

			if(lpObj->MapNumber == MAP_INDEX_ICARUS)
			{
				if(source == 8)
				{
					if(sitem->m_Type == ITEMGET(13,3))
					{
						if(lpObj->pInventory[7].IsItem() == 0)
						{
							return -1;
						}
					}
					else if(sitem->m_Type == ITEMGET(13,37))
					{
						if(lpObj->pInventory[7].IsItem() == 0)
						{
							return -1;
						}
					}
				}
				else if(source == 7)
				{
					if(lpObj->pInventory[8].m_Type != ITEMGET(13,3))
					{
						return -1;
					}
				}
			}

			break;
		case 2:
			if(source < 0 || source > (ReadConfig.WAREHOUSE_SIZE(aIndex,false)-1))
			{
				LogAdd("error-L1 : %s %d",__FILE__,__LINE__);
				return -1;
			}

			if(lpObj->pWarehouse[source].IsItem() == 0)
			{
				LogAdd(lMsg.Get(532),lpObj->AccountID,lpObj->Name,__LINE__);
				return -1;
			}

			sitem = &lpObj->pWarehouse[source];
			break;
		case 3:
		case 5:
		case 6:
		case 7:
		case 8:
		case 10:
		case 11:
		case 12:
		case 13:
		case 14:
		case 15:
		case 16:
		case 9:
			if(source < 0 || source > (CHAOS_BOX_SIZE -1))
			{
				LogAdd("error-L1 : %s %d",__FILE__,__LINE__);
				return -1;
			}

			if(lpObj->pChaosBox[source].IsItem() == 0)
			{
				LogAdd(lMsg.Get(532),lpObj->AccountID,lpObj->Name,__LINE__);
				return -1;
			}

			sitem = &lpObj->pChaosBox[source];
			break;
		case 4:
			if(gObj[aIndex].m_bPShopOpen == 1)
			{
				return -1;
			}

			EnterCriticalSection(&gObj[aIndex].m_critPShopTrade);

			if(gObj[aIndex].m_bPShopTransaction == 1)
			{
				LogAddTD("[PShop] [%s][%s] PShop Item Move Request Failed : Already Trade With Other",gObj[aIndex].AccountID,gObj[aIndex].Name);
				LeaveCriticalSection(&lpObj->m_critPShopTrade);
				return -1;
			}

			lpObj->m_bPShopTransaction = 1;
			bPersonalShopTrans = 1;
			bSourceIsPShop = 1;

			LeaveCriticalSection(&lpObj->m_critPShopTrade);

			if(source < ReadConfig.MAIN_INVENTORY_SIZE(lpObj->m_Index,false) || source > (ReadConfig.INVENTORY_SIZE(lpObj->m_Index,false) - 1))
			{
				LogAdd("error-L1 : %s %d",__FILE__,__LINE__);
				return -1;
			}

			if(lpObj->pInventory[source].IsItem() == 0)
			{
				LogAdd(lMsg.Get(532),lpObj->AccountID,lpObj->Name,__LINE__);
				return -1;
			}

			sitem = &lpObj->pInventory[source];

			break;
		default: return -1;
		}

		BYTE level;
		BYTE op1;
		BYTE op2;
		BYTE op3;
		BYTE dur;
		int type;

		BufferItemtoConvert3(siteminfo,type,level,op1,op2,op3,dur);

		if(sitem->m_Type != type || sitem->m_Level != level || sitem->m_SkillOption != op1 || sitem->m_LuckOption != op2 || sitem->m_Z28Option != op3)
		{
			LogAddC(2,lMsg.Get(533),type,level,op1,op2,op3,sitem->m_Type,sitem->m_Level,sitem->m_SkillOption,sitem->m_LuckOption,sitem->m_Z28Option);
			return -1;
		}

		switch(tFlag)
		{
		case 16:
			{				
				if(target < 0 || target > (CHAOS_BOX_SIZE -1))
				{
					LogAdd("error-L1 : %s %d",__FILE__,__LINE__);
					return -1;
				}

				titem = &lpObj->pChaosBox[target];

				if(sFlag == 0)
				{
					if( (sitem->m_Type >= ITEMGET(7,62) && sitem->m_Type == ITEMGET(7,72))
						|| (sitem->m_Type >= ITEMGET(8,62) && sitem->m_Type == ITEMGET(8,72))
						|| (sitem->m_Type >= ITEMGET(9,62) && sitem->m_Type == ITEMGET(9,72))
						|| (sitem->m_Type >= ITEMGET(10,62) && sitem->m_Type == ITEMGET(10,72))
						|| (sitem->m_Type >= ITEMGET(11,62) && sitem->m_Type == ITEMGET(11,72))
						)
					{

					}
				}else
				{
					return -1;
				}
			}break;
		case 15:
			{				
				if(target < 0 || target > (CHAOS_BOX_SIZE -1))
				{
					LogAdd("error-L1 : %s %d",__FILE__,__LINE__);
					return -1;
				}

				titem = &lpObj->pChaosBox[target];

				if(sFlag == 0)
				{
					if(sitem->m_Type >= ITEMGET(13,135)	&& sitem->m_Type <= ITEMGET(13,144))
					{

					}
				}else
				{
					return -1;
				}
			}break;
		case 10:
			{
				
				if(target < 0 || target > (CHAOS_BOX_SIZE -1))
				{
					LogAdd("error-L1 : %s %d",__FILE__,__LINE__);
					return -1;
				}

				titem = &lpObj->pChaosBox[target];

				if(sFlag == 0)
				{
					if(sitem->m_Type == ITEMGET(14,88)	||
							sitem->m_Type == ITEMGET(14,89) ||
							sitem->m_Type == ITEMGET(14,90)
							)
					{

					}
				}else
				{
					return -1;
				}
			}break;
		case 0:
			if(target < 0 || target > (ReadConfig.MAIN_INVENTORY_SIZE(lpObj->m_Index,false) - 1))
			{
				LogAdd("error-L1 : %s %d",__FILE__,__LINE__);
				return -1;
			}

			titem = &lpObj->pInventory[target];
			break;
		case 2:
			if(target < 0 || target > (ReadConfig.WAREHOUSE_SIZE(aIndex,false) - 1))
			{
				//LogAdd("error-L1 : %s %d",__FILE__,__LINE__);
				GCServerMsgStringSend ("You need to buy warehouse extension!",aIndex,0x01 );
				LogAdd("[%s][%s] Cant put item in that position on warehouse",gObj[aIndex].AccountID,gObj[aIndex].Name);
				return -1;
			}

			titem = &lpObj->pWarehouse[target];
			break;
		case 3:
		case 5:
		case 6:
		case 7:
		case 8:
			
		case 11:
		case 12:
		case 13:
		case 14:
		case 9:
			if(target < 0 || target > (CHAOS_BOX_SIZE -1))
			{
				LogAdd("error-L1 : %s %d",__FILE__,__LINE__);
				return -1;
			}

			titem = &lpObj->pChaosBox[target];

			if(sFlag == 0)
			{
				if(tFlag == 5)
				{
					if(sitem->m_Type == ITEMGET(12,15)	||
						sitem->m_Type == ITEMGET(14,13) ||
						sitem->m_Type == ITEMGET(14,14) ||
						sitem->m_Type == ITEMGET(14,22) ||
						sitem->m_Type == ITEMGET(13,31) ||
						sitem->m_Type == ITEMGET(12,31) ||
						sitem->m_Type == ITEMGET(12,32) ||
						sitem->m_Type == ITEMGET(13,52) ||
						sitem->m_Type == ITEMGET(13,53) ||
						sitem->m_Type == ITEMGET(14,53))
					{

					}
					else
					{
						return -1;
					}
				}
				else if(tFlag == 6)
				{
					if(g_kJewelOfHarmonySystem.IsJewelOfHarmonyOriginal(sitem->m_Type) == 0)
					{
						return -1;
					}
				}
				else if(tFlag == 7)
				{
					if(g_kJewelOfHarmonySystem.IsEnableToMakeSmeltingStoneItem(sitem) == 0)
					{
						GCServerMsgStringSend(lMsg.Get(3377),lpObj->m_Index,1);
						return -1;
					}
				}
				else if(tFlag == 8)
				{
					if(g_kJewelOfHarmonySystem.IsStrengthenByJewelOfHarmony(sitem) == 0)
					{
						return -1;
					}
				}
				else if(tFlag == 9)
				{
					if( sitem->m_Type != ITEMGET(14,54) &&	//Chaos Card
						sitem->m_Type != ITEMGET(14,92) &&	//Chaos Card Gold
						sitem->m_Type != ITEMGET(14,93) &&	//Chaos Card Rare
						sitem->m_Type != ITEMGET(14,95)		//Chaos Card Mini
						)
					{
						return -1;
					}
				}
				else if(tFlag == 11)
				{
					if ((sitem->m_NewOption > 0) && ( sitem->m_Level > 3))
					{

					}else if ((sitem->m_SetOption > 0) && ( sitem->m_Level > 3))
					{

					}else if( sitem->m_Type == ITEMGET(14,42) || sitem->m_Type == ITEMGET(12,15) || sitem->m_Type == ITEMGET(14,22))
					{

					}else
					{
						return -1;
					}
				}
				else if(tFlag == 12)
				{
					if (sitem->m_Type >= 0x183C && sitem->m_Type <= 0x1841) 
					{
						
					}else if (sitem->m_Type >= ITEMGET(12,70) && sitem->m_Type <= ITEMGET(12,74))
					{

					}else if (sitem->m_Type == ITEMGET(12,15) )
					{

					}else if (sitem->m_Type == ITEMGET(14,22) )
					{

					}else
					{
						return -1;
					}
				}
				else if(tFlag == 13)
				{
					if((IsSlotItem(sitem->m_Type)) && (sitem->m_ItemSlot1 == 0xFF || sitem->m_ItemSlot2 == 0xFF  || sitem->m_ItemSlot3 == 0xFF  || sitem->m_ItemSlot4 == 0xFF  || sitem->m_ItemSlot5 == 0xFF ))
					{

					}else if(sitem->m_Type >= 0x1864 && sitem->m_Type <= 0x1881)
					{

					}else if (sitem->m_Type == ITEMGET(12,15) )
					{

					}else if (sitem->m_Type == ITEMGET(14,22) )
					{

					}else
					{
						return -1;
					}
				}
				else if (tFlag == 14)
				{
					if((IsSlotItem(sitem->m_Type)) && (sitem->m_ItemSlot1 != 0xFF || sitem->m_ItemSlot2 != 0xFF  || sitem->m_ItemSlot3 != 0xFF  || sitem->m_ItemSlot4 != 0xFF  || sitem->m_ItemSlot5 != 0xFF ))
					{

					}else
					{
						return -1;
					}
				}
				else
				{
					if(sitem->m_Level < 4 && sitem->m_Z28Option*4 < 4)
					{
						if( sitem->m_Type == ITEMGET(12,15) ||	//Jewel of Chaos
							sitem->m_Type == ITEMGET(12,30)	||	//Bundle of Bless
							sitem->m_Type == ITEMGET(12,31)	||	//Bundle of Souls
							sitem->m_Type == ITEMGET(12,32) ||	//Red Ribbon Box
							sitem->m_Type == ITEMGET(13,14) ||	//Loch's Feather
							sitem->m_Type == ITEMGET(13,52) ||	//Crystal
							sitem->m_Type == ITEMGET(13,53) ||	//Condor Feather
							sitem->m_Type == ITEMGET(14,13) ||	//Jewel of Bless
							sitem->m_Type == ITEMGET(14,14) ||	//Jewel of Soul
							sitem->m_Type == ITEMGET(14,22) ||	//Jewel of Creation
							sitem->m_Type == ITEMGET(14,53) ||	//Charm Of Luck
							sitem->m_Type == ITEMGET(14,54) ||	//Chaos Cards
							sitem->m_Type == ITEMGET(14,92) ||	//Chaos Cards Gold
							sitem->m_Type == ITEMGET(14,93) ||	//Chaos Cards Rare
							sitem->m_Type == ITEMGET(14,95) ||	//Chaos Cards Mini
							sitem->m_Type == ITEMGET(14,96))	//Chaos machine rate
						{
							
						}
						else if(sitem->m_Type >= ITEMGET(12,0) && sitem->m_Type <= ITEMGET(12,2))
						{

						}
						else if(sitem->m_Type >= ITEMGET(12,130) && sitem->m_Type <= ITEMGET(12,135))
						{

						}
						else if(sitem->m_Type >= ITEMGET(14,103) && sitem->m_Type <= ITEMGET(14,108))
						{

						}
						else if(sitem->m_Type == ITEMGET(13,49) || sitem->m_Type == ITEMGET(13,50))
						{

						}
						else if(sitem->m_Type == ITEMGET(12,41))
						{

						}
						else if(sitem->m_Type == ITEMGET(14,17) || sitem->m_Type == ITEMGET(14,18))
						{

						}
						else if(sitem->m_Type == ITEMGET(13,2) && sitem->m_Durability == 255.0f)
						{

						}
						else if(sitem->m_Type == ITEMGET(13,16) || sitem->m_Type == ITEMGET(13,17))
						{

						}
						else if(sitem->m_Type == ITEMGET(14,31))
						{

						}
						else if(sitem->m_Type == ITEMGET(12,26))
						{

						}
						else if (sitem->m_Type == ITEMGET(14,112) )
						{

						}
						else if (sitem->m_Type == ITEMGET(14,113) )
						{

						}
						else if (sitem->m_Type == ITEMGET(14,121) )
						{

						}
						else if (sitem->m_Type == ITEMGET(14,122) )
						{

						}
						else if(
							sitem->m_Type == ITEMGET(13,32) ||
							sitem->m_Type == ITEMGET(13,33) ||
							sitem->m_Type == ITEMGET(13,34) ||
							sitem->m_Type == ITEMGET(13,35) ||
							sitem->m_Type == ITEMGET(13,36) ||
							sitem->m_Type == ITEMGET(13,37) ||
							sitem->m_Type == ITEMGET(14,16))
						{

						}
						else if(sitem->m_Type == ITEMGET(14,3) || sitem->m_Type == ITEMGET(14,38) || sitem->m_Type == ITEMGET(14,39))
						{

						}
						else if(sitem->m_Type == ITEMGET(14,70) || sitem->m_Type == ITEMGET(14,71) || sitem->m_Type == ITEMGET(14,94) || sitem->m_Type == ITEMGET(14,133))	//Elite Potions
						{

						}
						else if(g_kJewelOfHarmonySystem.IsJewelOfHarmonyPurity(sitem->m_Type) == 1)
						{

						}
						else if(sitem->m_Type == ITEMGET(14,31))
						{

						}
						else
						{
							return -1;
						}
					}
					else if(sitem->m_Type == ITEMGET(14,11))
					{
						return -1;
					}
				}
			}
			break;
		case 4:
			if(gDoPShopOpen == 0)
			{
				return -1;
			}

			if(gObj[aIndex].m_bPShopOpen == 1)
			{
				return -1;
			}

			if(gObjCheckSerialSpecialItemList(sitem) != 0)
			{
				//GCServerMsgStringSend("Can Not PShop Promo Items!", lpObj->m_Index, 1);
				return -1;
			}
		
			if(sitem->m_Type == ITEMGET(14,11) && sitem->m_Level == 13)
			{
				return -1;
			}

			if(g_kJewelOfHarmonySystem.IsStrengthenByJewelOfHarmony(sitem) == 1)
			{
				GCServerMsgStringSend(lMsg.Get(3367),lpObj->m_Index,1);
				return -1;
			}

			if(IsCashItem(sitem->m_Type) == 1)
			{
				return -1;
			}

			if(sitem->m_Type == ITEMGET(13,38))
			{
				GCServerMsgStringSend(lMsg.Get(3390),lpObj->m_Index,1);
				return -1;
			}

			if(sitem->m_Type == ITEMGET(13,39))
			{
				return -1;
			}

			if(sitem->m_Type == ITEMGET(13,20))
			{
				return -1;
			}

			if(sitem->m_Type == ITEMGET(13,107))
			{
				return -1;
			}

			if(sitem->m_Type == ITEMGET(14,64))//strange item IT
			{
				return -1;
			}

			EnterCriticalSection(&gObj[aIndex].m_critPShopTrade);

			if(gObj[aIndex].m_bPShopTransaction == 1)
			{
				if(bSourceIsPShop == 1)
				{
					lpObj->m_bPShopTransaction = 1;
					bPersonalShopTrans = 1;
				}
				else
				{
					LogAddTD("[PShop] [%s][%s] PShop Item Move Request Failed : Already Trade With Other",gObj[aIndex].AccountID,gObj[aIndex].Name);
					LeaveCriticalSection(&lpObj->m_critPShopTrade);
					return -1;
				}
			}
			else
			{
				lpObj->m_bPShopTransaction = 1;
				bPersonalShopTrans = 1;
			}

			LeaveCriticalSection(&lpObj->m_critPShopTrade);

			if(target < ReadConfig.MAIN_INVENTORY_SIZE(lpObj->m_Index,false) || target > (ReadConfig.INVENTORY_SIZE(lpObj->m_Index,false) - 1))
			{
				LogAdd("error-L1 : %s %d",__FILE__,__LINE__);
				return -1;
			}

			titem = &lpObj->pInventory[target];

			break;
		default: return -1;
		}

		s_num = sitem->GetNumber();

		if(gObjCheckSerial0ItemList(sitem))
		{
			MsgOutput(lpObj->m_Index,lMsg.Get(3354));
			if(ReadConfig.AHLog == TRUE)
			{
				ANTI_HACK_LOG.Output("[Anti-Hack][Serial 0 Item] [MoveItem] (%s)(%s) Item(%s) Pos(%d)",
					lpObj->AccountID,lpObj->Name,sitem->GetName(),source);
			}
			GCServerMsgStringSend("[Anti-Hack] Serial 0 Item!",lpObj->m_Index, 0x01);

			return -1;
		}
		
		if ( sFlag == 2 && tFlag == 0 )
		{
			if(gObjInventorySearchSerialNumber2(lpObj,s_num) == 0)
				return -1;
		}
		if ( sFlag == 0 && tFlag == 2 )
		{
			if(gObjWarehouseSearchSerialNumber2(lpObj,s_num,true) == 0)
				return -1;
		}

		if(gObjInventorySearchSerialNumber(lpObj,s_num) == 0)
		{
			return -1;
		}

		if(gObjWarehouseSearchSerialNumber(lpObj,s_num) == 0)
		{
			return -1;
		}

		if(titem->IsItem() == 1)
		{
			int max_count = 0;

			if(sitem->m_Type == ITEMGET(13,32) && titem->m_Durability < 20.0f)
			{
				max_count = 20;
			}

			if(sitem->m_Type == ITEMGET(13,33) && titem->m_Durability < 20.0f)
			{
				max_count = 20;
			}

			if(sitem->m_Type == ITEMGET(13,34) && titem->m_Durability < 10.0f)
			{
				max_count = 10;
			}

			if(sitem->m_Type >= ITEMGET(14,35) || sitem->m_Type >= ITEMGET(14,36) || sitem->m_Type >= ITEMGET(14,37))
			{
				if(titem->m_Durability < 1.0f)
				{
					max_count = 1;
				}
			}

			if(sitem->m_Type >= ITEMGET(14,46) && sitem->m_Type <= ITEMGET(14,50))
			{
				if(titem->m_Durability < 3.0f)
				{
					max_count = 3;
				}
			}

			if(sitem->m_Type == ITEMGET(14,29))		//Symbol of Kundun
			{
				if(sitem->m_Level == titem->m_Level)
				{
					max_count = 5;
				}
			}

			if(sitem->m_Type == ITEMGET(14,100))	//Lucky Peny
			{
				max_count = 255;
			}

			if(sitem->m_Type == ITEMGET(14,153))	//Quest Item
			{
				max_count = 255;
			}

			if(sitem->m_Type == ITEMGET(14,154))	//Quest Item
			{
				max_count = 255;
			}

			if(sitem->m_Type == ITEMGET(14,155))	//Quest Item
			{
				max_count = 255;
			}

			if(sitem->m_Type == ITEMGET(14,156))	//Quest Item
			{
				max_count = 255;
			}

			if(sitem->m_Type == ITEMGET(14,101))	//Paper Piece
			{
				max_count = 5;
			}

			if(sitem->m_Type == ITEMGET(14,110))	//Blue Ball Piece Goer Event
			{
				max_count = 5;
			}

			if(sitem->m_Type == ITEMGET(14,94))		//Medium Elite Healing Potion
			{
				max_count = 255;
			}
			if(sitem->m_Type == ITEMGET(14,70))		//Elite Healing Potion
			{
				max_count = 255;
			}
			if(sitem->m_Type == ITEMGET(14,71))		//Elite Mana Potion
			{
				max_count = 255;
			}
			if(sitem->m_Type == ITEMGET(14,133))	//Elite SD potion
			{
				max_count = 3;
			}

			if(sitem->m_Type == ITEMGET(14,7))
			{
				if(titem->m_Durability < 250.0f)
				{
					max_count = 250;
				}
			}
			else if(sitem->m_Type >= ITEMGET(14,0) && sitem->m_Type <= ITEMGET(14,8) || sitem->m_Type >= ITEMGET(14,38) && sitem->m_Type <= ITEMGET(14,40))
			{
				if(titem->m_Durability < 3.0f)
				{
					max_count = 3;
				}
			}
			else if(sitem->m_Type == ITEMGET(4,15) || sitem->m_Type == ITEMGET(4,7))
			{
				if(titem->m_Durability < 255.0f)
				{
					max_count = 255;
				}
			}

			if(max_count != 0)
			{
				if(sFlag != 0)
				{
					return -1;
				}

				if(titem->m_Type == sitem->m_Type && titem->m_Level == sitem->m_Level)
				{
					int t_dur = titem->m_Durability;

					if(t_dur < max_count)
					{
						int dif_dur = max_count - int(titem->m_Durability);

						if(dif_dur > sitem->m_Durability)
						{
							dif_dur = sitem->m_Durability;
						}

						titem->m_Durability += dif_dur;
						sitem->m_Durability -= dif_dur;

						if(titem->m_Type == ITEMGET(14,29) && max_count <= titem->m_Durability)	//Symbol of Kundun
						{
							titem->m_Durability -= max_count;

							if(titem->m_Durability == 0)
							{
								gObjInventoryDeleteItem(aIndex, target);
								GCInventoryItemDeleteSend(aIndex, target, 1);
								durTsend = 0;
							}
							else
							{
								durTsend = 1;
							}

							ItemSerialCreateSend(aIndex,235,gObj[aIndex].X,gObj[aIndex].Y,ItemGetNumberMake(14,28),sitem->m_Level,0,0,0,0,aIndex,0,0);
						}

						if(titem->m_Type == ITEMGET(14,101) && max_count <= titem->m_Durability)//Paper Piece
						{
							titem->m_Durability -= max_count;

							if(titem->m_Durability == 0)
							{
								gObjInventoryDeleteItem(aIndex, target);
								GCInventoryItemDeleteSend(aIndex, target, 1);
								durTsend = 0;
							}
							else
							{
								durTsend = 1;
							}
							ItemSerialCreateSend(aIndex,235,gObj[aIndex].X,gObj[aIndex].Y,ItemGetNumberMake(14,102),sitem->m_Level,0,0,0,0,aIndex,0,0);
						}

						if(titem->m_Type == ITEMGET(14,110) && max_count <= titem->m_Durability)//Blue Ball Piece
						{
							titem->m_Durability -= max_count;

							if(titem->m_Durability == 0)
							{
								gObjInventoryDeleteItem(aIndex, target);
								GCInventoryItemDeleteSend(aIndex, target, 1);
								durTsend = 0;
							}
							else
							{
								durTsend = 1;
							}
							ItemSerialCreateSend(aIndex,235,gObj[aIndex].X,gObj[aIndex].Y,ItemGetNumberMake(14,111),sitem->m_Level,0,0,0,0,aIndex,0,0);
						}

						if(sitem->m_Durability > 0)
						{
							durSsend = 1;
							durTsend = 1;
						}
						else
						{
							switch(sFlag)
							{
								case 0:
									gObjInventoryItemSet(lpObj->m_Index,source,255);
									sitem->Clear();
									GCInventoryItemDeleteSend(lpObj->m_Index,source,0);
									durTsend = 1;
								break;
								default:	return 0;
							}
						}
					}
				}
			}
			return -1;
		}

		switch(tFlag)
		{
		case 0:
		case 4:
			if(target < INVETORY_WEAR_SIZE)
			{
				if(lpObj->MapNumber == MAP_INDEX_ICARUS)
				{
					if(target == 10)
					{
						if(lpObj->pInventory[source].m_Type == ITEMGET(13,10) || lpObj->pInventory[source].m_Type == ITEMGET(13,68) || lpObj->pInventory[source].m_Type == ITEMGET(13,76) || lpObj->pInventory[source].m_Type == ITEMGET(13,122) || lpObj->pInventory[source].m_Type == ITEMGET(13,40) || lpObj->pInventory[source].m_Type == ITEMGET(13,41))
						{
							return -1;
						}
					}
					else if(target == 11)
					{
						if(lpObj->pInventory[source].m_Type == ITEMGET(13,10) || lpObj->pInventory[source].m_Type == ITEMGET(13,68) || lpObj->pInventory[source].m_Type == ITEMGET(13,76) || lpObj->pInventory[source].m_Type == ITEMGET(13,122) || lpObj->pInventory[source].m_Type == ITEMGET(13,40) || lpObj->pInventory[source].m_Type == ITEMGET(13,41))
						{
							return -1;
						}
					}
				}

				if(lpObj->MapNumber == MAP_INDEX_ICARUS || lpObj->MapNumber == MAP_INDEX_KANTURU_BOSS)
				{
					if(target == 10)
					{
						if(lpObj->pInventory[source].m_Type == ITEMGET(13,39) ||
							lpObj->pInventory[source].m_Type == ITEMGET(13,10) ||
							lpObj->pInventory[source].m_Type == ITEMGET(13,76) ||
							lpObj->pInventory[source].m_Type == ITEMGET(13,122) ||
							lpObj->pInventory[source].m_Type == ITEMGET(13,40) ||
							lpObj->pInventory[source].m_Type == ITEMGET(13,41) ||
							lpObj->pInventory[source].m_Type == ITEMGET(13,68))
						{
							return -1;
						}
					}
					else if(target == 11)
					{
						if(lpObj->pInventory[source].m_Type == ITEMGET(13,39) ||
							lpObj->pInventory[source].m_Type == ITEMGET(13,10) ||
							lpObj->pInventory[source].m_Type == ITEMGET(13,76) ||
							lpObj->pInventory[source].m_Type == ITEMGET(13,122) ||
							lpObj->pInventory[source].m_Type == ITEMGET(13,40) ||
							lpObj->pInventory[source].m_Type == ITEMGET(13,41) ||
							lpObj->pInventory[source].m_Type == ITEMGET(13,68))
						{
							return -1;
						}
					}
				}

				useClass = sitem->IsClass(lpObj->Class,lpObj->ChangeUP);

				if(useClass == 0)
				{
					LogAddTD("[IsClass] Not His Class! [%s][%s][%d vs %d]",
						lpObj->AccountID, lpObj->Name,
						lpObj->Class,lpObj->ChangeUP);
					return -1;
				}

				if(gObjIsItemPut(lpObj,sitem,target) == 0)
				{
					return -1;
				}
			}
			else 
			{
				if(target >= ReadConfig.MAIN_INVENTORY_SIZE(lpObj->m_Index,false))
				{
					if(lpObj->pInventory[source].m_Type == ITEMGET(12,26))
					{
						if(lpObj->pInventory[source].m_Level == 1 ||
							lpObj->pInventory[source].m_Level == 2 ||
							lpObj->pInventory[source].m_Level == 3 ||
							lpObj->pInventory[source].m_Level == 4 ||
							lpObj->pInventory[source].m_Level == 5)
						{
							return -1;
						}
					}	
				}

				w = (target - INVETORY_WEAR_SIZE)%8;
				h = (target - INVETORY_WEAR_SIZE)/8;

				int InventoryRows = 12;
				if(ReadConfig.S6E2 == 1)
				{
					InventoryRows = 28;
				}

				if(ExtentCheck(w,h,8,InventoryRows) == 0)
				{
					return -1;
				}

			sitem->GetSize((int &)iwidth,(int &)iheight);
			memcpy(TempInventoryMap,lpObj->pInventoryMap,ReadConfig.GETSLOTS(aIndex));

				switch(sFlag)
				{
				case 0:
				case 4:
					if(source > (INVETORY_WEAR_SIZE - 1))
					{
						gObjInventoryItemBoxSet(lpObj->m_Index,source,iwidth,iheight,255);
					}
					break;
				default: break;
				}
			
			if(*(BYTE*)(gObj[aIndex].pInventoryMap + h * 8 + w) != 255)
			{
				memcpy(lpObj->pInventoryMap,TempInventoryMap,ReadConfig.GETSLOTS(aIndex));
				return -1;
			}

			blank = gObjInventoryRectCheck(lpObj->m_Index,w,h,iwidth,iheight);

			if(blank >= 254)
			{
				memcpy(lpObj->pInventoryMap,TempInventoryMap,ReadConfig.GETSLOTS(aIndex));
				return -1;
			}
			}

			s_num = sitem->GetNumber();
			titem = sitem;

				switch(sFlag)
				{
				case 0:
				case 4:
					lpObj->pInventory[target] = lpObj->pInventory[source];
					gObjInventoryDeleteItem(aIndex,source);
					break;
				case 2:
					lpObj->pInventory[target] = lpObj->pWarehouse[source];
					gObjWarehouseDeleteItem(aIndex,source);
					break;
				case 3:
				case 5:
				case 6:
				case 7:
				case 8:
				case 10:
				case 11:
				case 12:
				case 13:
				case 14:
				case 15:
				case 16:
				case 9:
					lpObj->pInventory[target] = lpObj->pChaosBox[source];
					gObjChaosBoxDeleteItem(aIndex,source);
					break;
				default : break;
				}

				if(target > (INVETORY_WEAR_SIZE - 1))
				{
					gObjInventoryItemSet(aIndex,target,1);
				}

				if(target < INVETORY_WEAR_SIZE)
				{
					CItem * lpItem = &lpObj->pInventory[target];
					if(lpItem->m_SkillOption)
					{
						int s_pos = gObjWeaponMagicAdd(&gObj[aIndex],lpItem->m_Special[0],lpItem->m_Level);
						if(s_pos >= 0)
						{
							GCMagicListOneSend(aIndex,s_pos,lpItem->m_Special[0],lpItem->m_Level,0,0);
						}
					}
				}
			break;
		case 2:
			BYTE res_1;
				switch(sFlag)
				{
				case 0:
					if(lpObj->pInventory[source].m_Type == ITEMGET(13,20))
					{
						if(lpObj->pInventory[source].m_Level == 0 ||
							lpObj->pInventory[source].m_Level == 1)
						{
							return -1;
						}
					}

					if(lpObj->pInventory[source].m_Type == ITEMGET(13,107))
					{
						return -1;
					}

					if(lpObj->pInventory[source].m_Type == ITEMGET(14,11))
					{
						if(lpObj->pInventory[source].m_Level == 13)
						{
							return -1;
						}
					}

					if(lpObj->pInventory[source].m_Type == ITEMGET(12,26))
					{
						if(lpObj->pInventory[source].m_Level == 1 ||
							lpObj->pInventory[source].m_Level == 2 ||
							lpObj->pInventory[source].m_Level == 3 ||
							lpObj->pInventory[source].m_Level == 4 ||
							lpObj->pInventory[source].m_Level == 5)
						{
							return -1;
						}
					}

					if(lpObj->pInventory[source].m_Type == ITEMGET(13,39))
					{
						return -1;
					}

					res_1 = gObjWarehouseInsertItemPos(aIndex,lpObj->pInventory[source],target,-1);
					break;
				case 2:
					res_1 = gObjWarehouseInsertItemPos(aIndex,lpObj->pWarehouse[source],target,source);
					break;
				default : break;
				}

				if(res_1 == 255)
				{
					return -1;
				}

				switch(sFlag)
				{
				case 0:
					lpObj->pWarehouse[target] = lpObj->pInventory[source];
					gObjInventoryDeleteItem(aIndex,source);
					break;
				case 2:
					lpObj->pWarehouse[target] = lpObj->pWarehouse[source];
					gObjWarehouseDeleteItem(aIndex,source);
					break;
				default : break;
				}

				gObjWarehouseItemSet(aIndex,target,1);

				if(sFlag == 0)
				{
					if(source < INVETORY_WEAR_SIZE)
					{
						if(source == 10 || source == 11)
						{
							if(lpObj->pWarehouse[target].m_Type == ITEMGET(13,10) || lpObj->pWarehouse[target].m_Type == ITEMGET(13,68) || lpObj->pWarehouse[target].m_Type == ITEMGET(13,76) || lpObj->pWarehouse[target].m_Type == ITEMGET(13,122) || lpObj->pWarehouse[target].m_Type == ITEMGET(13,40) || lpObj->pWarehouse[target].m_Type == ITEMGET(13,41))
							{
								gObjUseSkill.SkillChangeUse(aIndex);
							}
						}

						gObjMakePreviewCharSet(aIndex);
						GCEquipmentChange(aIndex,source);
					}
				}
			return 2;
		case 3:
		case 5:
		case 6:
		case 7:
		case 8:

		case 10:
		case 11:
		case 12:
		case 13:
		case 14:
		case 15:
		case 16:
		case 9:
			BYTE res_2;
				switch(sFlag)
				{
				case 0:
					res_2 = gObjChaosBoxInsertItemPos(aIndex,lpObj->pInventory[source],target,-1);
					break;
				case 3:
				case 5:
				case 6:
				case 7:
				case 8:
				case 10:
				case 11:
				case 12:
				case 13:
				case 14:
				case 15:
				case 9:

					res_2 = gObjChaosBoxInsertItemPos(aIndex,lpObj->pChaosBox[source],target,source);
					break;
				default : break;
				}

				if(res_2 == 255)
				{
					return -1;
				}

				switch(sFlag)
				{
				case 0:
					lpObj->pChaosBox[target] = lpObj->pInventory[source];
					gObjInventoryDeleteItem(aIndex,source);
					break;
				case 3:
				case 5:
				case 6:
				case 7:
				case 8:
				case 10:
				case 11:
				case 12:
				case 13:
				case 14:
				case 15:
				case 16:
				case 9:

					lpObj->pChaosBox[target] = lpObj->pChaosBox[source];
					gObjChaosBoxDeleteItem(aIndex,source);
					break;
				default : break;
				}

				gObjChaosItemSet(aIndex,target,1);

				if(sFlag == 0)
				{
					if(source < INVETORY_WEAR_SIZE)
					{
						if(source == 10 || source == 11)
						{
							if(lpObj->pChaosBox[target].m_Type == ITEMGET(13,10))
							{
								gObjUseSkill.SkillChangeUse(aIndex);
							}

							if(lpObj->pChaosBox[target].m_Type == ITEMGET(13,39))
							{
								gObjUseSkill.SkillChangeUse(aIndex);
							}

							if(lpObj->pChaosBox[target].m_Type == ITEMGET(13,68))
							{
								gObjUseSkill.SkillChangeUse(aIndex);
							}

							if(lpObj->pChaosBox[target].m_Type == ITEMGET(13,76))
							{
								gObjUseSkill.SkillChangeUse(aIndex);
							}

							if(lpObj->pChaosBox[target].m_Type == ITEMGET(13,122))
							{
								gObjUseSkill.SkillChangeUse(aIndex);
							}

							if(lpObj->pChaosBox[target].m_Type == ITEMGET(13,40))
							{
								gObjUseSkill.SkillChangeUse(aIndex);
							}

							if(lpObj->pChaosBox[target].m_Type == ITEMGET(13,41))
							{
								gObjUseSkill.SkillChangeUse(aIndex);
							}
						}

						gObjMakePreviewCharSet(aIndex);
						GCEquipmentChange(aIndex,source);
					}
				}
			return tFlag;
		default: break;
		}

		if(sFlag == 0 && source < INVETORY_WEAR_SIZE)
		{
			if(lpObj->pInventory[source].IsItem() == 1)
			{
				if(lpObj->pInventory[source].m_Type == ITEMGET(13,10))
				{
					gObjUseSkill.SkillChangeUse(aIndex);
					LogAdd(lMsg.Get(534),gObj[aIndex].Name,lpObj->pInventory[source].m_Level);
				}

				if(lpObj->pInventory[source].m_Type == ITEMGET(13,39))
				{
					gObjUseSkill.SkillChangeUse(aIndex);
					LogAdd(lMsg.Get(534),gObj[aIndex].Name,lpObj->pInventory[source].m_Level);
				}

				if(lpObj->pInventory[source].m_Type == ITEMGET(13,68))
				{
					gObjUseSkill.SkillChangeUse(aIndex);
					LogAdd(lMsg.Get(534),gObj[aIndex].Name,lpObj->pInventory[source].m_Level);
				}

				if(lpObj->pInventory[source].m_Type == ITEMGET(13,76))
				{
					gObjUseSkill.SkillChangeUse(aIndex);
					LogAdd(lMsg.Get(534),gObj[aIndex].Name,lpObj->pInventory[source].m_Level);
				}

				if(lpObj->pInventory[source].m_Type == ITEMGET(13,122))
				{
					gObjUseSkill.SkillChangeUse(aIndex);
					LogAdd(lMsg.Get(534),gObj[aIndex].Name,lpObj->pInventory[source].m_Level);
				}

				if(lpObj->pInventory[source].m_Type == ITEMGET(13,40))
				{
					gObjUseSkill.SkillChangeUse(aIndex);
					LogAdd(lMsg.Get(534),gObj[aIndex].Name,lpObj->pInventory[source].m_Level);
				}

				if(lpObj->pInventory[source].m_Type == ITEMGET(13,41))
				{
					gObjUseSkill.SkillChangeUse(aIndex);
					LogAdd(lMsg.Get(534),gObj[aIndex].Name,lpObj->pInventory[source].m_Level);
				}
			}
			else
			{
				if(source == 10 || source == 11)
				{
					if(lpObj->pInventory[target].m_Type == ITEMGET(13,10))
					{
						gObjUseSkill.SkillChangeUse(aIndex);
					}

					if(lpObj->pInventory[target].m_Type == ITEMGET(13,39))
					{
						gObjUseSkill.SkillChangeUse(aIndex);
					}

					if(lpObj->pInventory[target].m_Type == ITEMGET(13,68))
					{
						gObjUseSkill.SkillChangeUse(aIndex);
					}

					if(lpObj->pInventory[target].m_Type == ITEMGET(13,76))
					{
						gObjUseSkill.SkillChangeUse(aIndex);
					}

					if(lpObj->pInventory[target].m_Type == ITEMGET(13,122))
					{
						gObjUseSkill.SkillChangeUse(aIndex);
					}

					if(lpObj->pInventory[target].m_Type == ITEMGET(13,40))
					{
						gObjUseSkill.SkillChangeUse(aIndex);
					}

					if(lpObj->pInventory[target].m_Type == ITEMGET(13,41))
					{
						gObjUseSkill.SkillChangeUse(aIndex);
					}
				}
			}

			gObjMakePreviewCharSet(aIndex);
			GCEquipmentChange(aIndex,source);
		}

		if(tFlag == 0 && target < INVETORY_WEAR_SIZE)
		{
			if(lpObj->pInventory[target].m_Type == ITEMGET(13,10))
			{
				gObjUseSkill.SkillChangeUse(aIndex);
				LogAdd(lMsg.Get(534),gObj[aIndex].Name,lpObj->pInventory[target].m_Level);
			}

			if(lpObj->pInventory[target].m_Type == ITEMGET(13,39))
			{
				gObjUseSkill.SkillChangeUse(aIndex);
				LogAdd(lMsg.Get(534),gObj[aIndex].Name,lpObj->pInventory[target].m_Level);
			}

			if(lpObj->pInventory[target].m_Type == ITEMGET(13,68))
			{
				gObjUseSkill.SkillChangeUse(aIndex);
				LogAdd(lMsg.Get(534),gObj[aIndex].Name,lpObj->pInventory[target].m_Level);
			}

			if(lpObj->pInventory[target].m_Type == ITEMGET(13,76))
			{
				gObjUseSkill.SkillChangeUse(aIndex);
				LogAdd(lMsg.Get(534),gObj[aIndex].Name,lpObj->pInventory[target].m_Level);
			}

			if(lpObj->pInventory[target].m_Type == ITEMGET(13,122))
			{
				gObjUseSkill.SkillChangeUse(aIndex);
				LogAdd(lMsg.Get(534),gObj[aIndex].Name,lpObj->pInventory[target].m_Level);
			}

			if(lpObj->pInventory[target].m_Type == ITEMGET(13,40))
			{
				gObjUseSkill.SkillChangeUse(aIndex);
				LogAdd(lMsg.Get(534),gObj[aIndex].Name,lpObj->pInventory[target].m_Level);
			}

			if(lpObj->pInventory[target].m_Type == ITEMGET(13,41))
			{
				gObjUseSkill.SkillChangeUse(aIndex);
				LogAdd(lMsg.Get(534),gObj[aIndex].Name,lpObj->pInventory[target].m_Level);
			}

			gObjMakePreviewCharSet(aIndex);
			GCEquipmentChange(aIndex,target);
		}
	}
	__finally
	{
		if(bPersonalShopTrans == 1)
		{
			gObj[aIndex].m_bPShopTransaction = 0;
		}
	}
	return false;
}






BYTE gObjTradeRectCheck(int aIndex, int sx, int sy, int width, int height )
{
	int x;
	int y;
	int blank = 0;
	int xx;
	int yy;

	int TradeBoxRows = 4;

	if ( ( sx + width) > 8 )
	{
		return -1;
	}

	if ( (sy + height) > TradeBoxRows )
	{
		return -1;
	}

	for ( y=0;y<height;y++)
	{
		yy = sy + y;
		
		for ( x=0;x<width;x++)
		{
			xx = sx + x;

			if ( ::ExtentCheck(xx, yy, 8, TradeBoxRows) == TRUE )
			{
				if ( *(BYTE*)(gObj[aIndex].TradeMap + yy * 8 + xx) != (BYTE)-1)
				{
					blank++;
					return -1;
				}
			}
			else
			{
				LogAdd("error : %s %d", __FILE__ ,__LINE__);
				return -1;
			}
		}
	}

	if ( blank == 0 )
	{
		return (sy * 8 + sx);
	}

	return -1;
}






BOOL gObjTradeItemBoxSet(int aIndex, int itempos, int xl, int yl, BYTE set_byte)
{
	int itemposx = (itempos)%8;
	int itemposy = (itempos)/8;

	int TradeBoxRows = 4;

	int xx,yy;

	for(int y = 0; y < yl; y ++)
	{
		yy = itemposy + y;

		for(int x = 0; x < xl; x++)
		{
			xx = itemposx + x;

			if(ExtentCheck(xx,yy,8,TradeBoxRows)==1)
			{
				*(BYTE*)(gObj[aIndex].TradeMap + yy*8+xx) = set_byte;
			}
			else
			{
				LogAdd("error : %s %d",__FILE__,__LINE__);
				return false;
			}
		}
	}
	return true;
}






BYTE gObjTradeInventoryMove(LPOBJ lpObj, BYTE source, BYTE target)
{
	int iwidth,iheight,s_num;
	BYTE itembuf[8+5];

	lpObj->m_InterfaceTime = GetTickCount();

	if(source > TRADE_BOX_SIZE)
	{
		LogAdd("error : %s %d",__FILE__,__LINE__);
		return -1;
	}

	if(target > ReadConfig.MAIN_INVENTORY_SIZE(lpObj->m_Index,false))
	{
		LogAdd("error : %s %d",__FILE__,__LINE__);
		return -1;
	}

	if(lpObj->TargetNumber < 0)
	{
		return -1;
	}

	if(lpObj->Trade[source].IsItem() == 0)
	{
		return -1;
	}

	if(lpObj->m_IfState.use == 0 || lpObj->m_IfState.type != 1)
	{
		return -1;
	}

	if(gObjInventoryInsertItemPos(lpObj->m_Index,lpObj->Trade[source],target,1) == 255)
	{
		return -1;
	}

	s_num = 0;
	s_num = lpObj->Trade[source].m_Number;

	lpObj->Trade[source].GetSize((int &)iwidth,(int &)iheight);

	gObjTradeItemBoxSet(lpObj->m_Index,source,iwidth,iheight,255);
	lpObj->Trade[source].Clear();

	ItemByteConvert(itembuf,lpObj->Trade[source]);

	GCTradeOtherDel(lpObj->TargetNumber,source);

	if(gObjCheckSerial0ItemList(&lpObj->Trade[source]) != 0)
	{
		MsgOutput(lpObj->m_Index,lMsg.Get(3354));
		if(ReadConfig.AHLog == TRUE)
		{
			ANTI_HACK_LOG.Output("[Anti-Hack][Serial 0 Item] [Trade] (%s)(%s) Item(%s) Pos(%d)",
				lpObj->AccountID,lpObj->Name,lpObj->Trade[source].GetName(),source);
		}
		GCServerMsgStringSend("[Anti-Hack] Serial 0 Item!",lpObj->m_Index, 0x01);

		return -1;
	}

	if(gObjInventorySearchSerialNumber(lpObj,s_num) == 0)
	{
		return -1;
	}

	if(target < INVETORY_WEAR_SIZE)
	{
		if(lpObj->pInventory[target].IsItem() == 1)
		{
			if(lpObj->pInventory[target].m_Type == ITEMGET(13,10))
			{
				gObjUseSkill.SkillChangeUse(lpObj->m_Index);
				LogAdd(lMsg.Get(534),lpObj->Name,lpObj->pInventory[target].m_Level);
			}
			if(lpObj->pInventory[target].m_Type == ITEMGET(13,39))
			{
				gObjUseSkill.SkillChangeUse(lpObj->m_Index);
				LogAdd(lMsg.Get(534),lpObj->Name,lpObj->pInventory[target].m_Level);
			}
			if(lpObj->pInventory[target].m_Type == ITEMGET(13,68))
			{
				gObjUseSkill.SkillChangeUse(lpObj->m_Index);
				LogAdd(lMsg.Get(534),lpObj->Name,lpObj->pInventory[target].m_Level);
			}
			if(lpObj->pInventory[target].m_Type == ITEMGET(13,76))
			{
				gObjUseSkill.SkillChangeUse(lpObj->m_Index);
				LogAdd(lMsg.Get(534),lpObj->Name,lpObj->pInventory[target].m_Level);
			}
			if(lpObj->pInventory[target].m_Type == ITEMGET(13,122))
			{
				gObjUseSkill.SkillChangeUse(lpObj->m_Index);
				LogAdd(lMsg.Get(534),lpObj->Name,lpObj->pInventory[target].m_Level);
			}
			if(lpObj->pInventory[target].m_Type == ITEMGET(13,40))
			{
				gObjUseSkill.SkillChangeUse(lpObj->m_Index);
				LogAdd(lMsg.Get(534),lpObj->Name,lpObj->pInventory[target].m_Level);
			}
			if(lpObj->pInventory[target].m_Type == ITEMGET(13,41))
			{
				gObjUseSkill.SkillChangeUse(lpObj->m_Index);
				LogAdd(lMsg.Get(534),lpObj->Name,lpObj->pInventory[target].m_Level);
			}
		}

		gObjMakePreviewCharSet(lpObj->m_Index);
		GCEquipmentChange(lpObj->m_Index,target);
	}

	return false;
}






BYTE gObjInventoryTradeMove(LPOBJ lpObj, BYTE source, BYTE target)
{
	int h,w,iwidth,iheight,s_num,blank;

	lpObj->m_InterfaceTime = GetTickCount();

	if(source > ReadConfig.MAIN_INVENTORY_SIZE(lpObj->m_Index,false))
	{
		LogAdd("error : %s %d",__FILE__,__LINE__);
		return -1;
	}

	if(lpObj->pInventory[source].IsItem() == 0)
	{
		return -1;
	}

	if(lpObj->TargetNumber < 0)
	{
		return -1;
	}

	if(lpObj->m_IfState.use == 0 || lpObj->m_IfState.type != 1)
	{
		return -1;
	}

	if(lpObj->pInventory[source].m_Type == ITEMGET(13,20))
	{
		return -1;
	}

	if(lpObj->pInventory[source].m_Type == ITEMGET(13,107))
	{
		return -1;
	}

	if(lpObj->pInventory[source].m_Type == ITEMGET(13,20))
	{
		if(lpObj->pInventory[source].m_Level == 2)
		{
			return -1;
		}
	}

	if(lpObj->pInventory[source].m_Type == ITEMGET(14,11))
	{
		if(lpObj->pInventory[source].m_Level == 13)
		{
			return -1;
		}
	}

	if(lpObj->pInventory[source].m_Type == ITEMGET(12,26))
	{
		if(lpObj->pInventory[source].m_Level == 1 ||
			lpObj->pInventory[source].m_Level == 2 ||
			lpObj->pInventory[source].m_Level == 3 ||
			lpObj->pInventory[source].m_Level == 4 ||
			lpObj->pInventory[source].m_Level == 5)
		{
			return -1;
		}
	}

	if(lpObj->pInventory[source].m_Type == ITEMGET(13,38))
	{
		return -1;
	}

	if(lpObj->pInventory[source].m_Type == ITEMGET(13,39))
	{
		return -1;
	}

	if(lpObj->pInventory[source].m_Type == ITEMGET(13,67))
	{
		return -1;
	}

	if(lpObj->pInventory[source].m_Type == ITEMGET(13,80))
	{
		return -1;
	}

	if(lpObj->pInventory[source].m_Type == ITEMGET(13,123))
	{
		return -1;
	}
	if(IsCashItem(lpObj->pInventory[source].m_Type) == 1)
	{
		return -1;
	}

	lpObj->pInventory[source].GetSize((int &)iwidth,(int &)iheight);
	s_num = lpObj->pInventory[source].GetNumber();

	if(gObjCheckSerial0ItemList(&lpObj->pInventory[source]) != 0)
	{
		MsgOutput(lpObj->m_Index,lMsg.Get(3354));
		if(ReadConfig.AHLog == TRUE)
		{
			ANTI_HACK_LOG.Output("[Anti-Hack][Serial 0 Item] [Trade] (%s)(%s) Item(%s) Pos(%d)",
				lpObj->AccountID,lpObj->Name,lpObj->pInventory[source].GetName(),source);
		}
		GCServerMsgStringSend("[Anti-Hack] Serial 0 Item!",lpObj->m_Index, 0x01);

		return -1;
	}

	if(gObjCheckSerialSpecialItemList(&lpObj->pInventory[source]) != 0)
	{
		GCServerMsgStringSend("Can Not trade Promo Items!", lpObj->m_Index, 1);
		return -1;
	}

	if(gObjInventorySearchSerialNumber(lpObj,s_num) == 0)
	{
		return -1;
	}

	w = target % 8;
	h = target / 8;

	int TradeBoxRows = 4;
	if(ExtentCheck(w,h,8,TradeBoxRows) == 0)
	{
		return -1;
	}

	if(*(BYTE*)(lpObj->TradeMap + h * 8 + w) == 255)
	{
		BYTE itembuf[8+5];
		blank = gObjTradeRectCheck(lpObj->m_Index,w,h,iwidth,iheight);

		if(blank == 255)
		{
			return -1;
		}

		lpObj->Trade[blank] = lpObj->pInventory[source];
		gObjInventoryDeleteItem(lpObj->m_Index,source);
		gObjTradeItemBoxSet(lpObj->m_Index,blank,iwidth,iheight,lpObj->Trade[blank].m_Type);
		ItemByteConvert(itembuf,lpObj->Trade[blank]);
		GCTradeOtherAdd(lpObj->TargetNumber,blank,itembuf);

		if(source < INVETORY_WEAR_SIZE)
		{
			if(source == 10 || source == 11)
			{
				if(lpObj->Trade[blank].m_Type == ITEMGET(13,10) || lpObj->Trade[blank].m_Type == ITEMGET(13,39) || lpObj->Trade[blank].m_Type == ITEMGET(13,68) || lpObj->Trade[blank].m_Type == ITEMGET(13,76) || lpObj->Trade[blank].m_Type == ITEMGET(13,122) || lpObj->Trade[blank].m_Type == ITEMGET(13,40) || lpObj->Trade[blank].m_Type == ITEMGET(13,41))
				{
					gObjUseSkill.SkillChangeUse(lpObj->m_Index);
				}
			}
			gObjMakePreviewCharSet(lpObj->m_Index);
			GCEquipmentChange(lpObj->m_Index,source);
		}
		return true;
	}
	return -1;
}






BYTE gObjTradeTradeMove(LPOBJ lpObj, BYTE source, BYTE target)
{
	int h,w;
	int iwidth,iheight;
	BYTE TempTradeMap[TRADE_BOX_SIZE];
	int blank;

	lpObj->m_InterfaceTime = GetTickCount();

	if(source > TRADE_BOX_SIZE)
	{
		LogAdd("error : %s %d",__FILE__,__LINE__);
		return -1;
	}
	if(source == target)
	{
		LogAdd("error : %s %d",__FILE__,__LINE__);
		return -1;
	}
	if(lpObj->Trade[source].IsItem() == 0)
	{
		return -1;
	}
	if(lpObj->Trade[target].IsItem() == 1)
	{
		return -1;
	}
	if(lpObj->TargetNumber < 0)
	{
		return -1;
	}
	if(lpObj->m_IfState.use == 0 || lpObj->m_IfState.type != 1)
	{
		return -1;
	}

	lpObj->Trade[source].GetSize((int &)iwidth,(int &)iheight);
	memcpy(TempTradeMap,lpObj->TradeMap,TRADE_BOX_SIZE);
	gObjTradeItemBoxSet(lpObj->m_Index,source,iwidth,iheight,255);

	w = target % 8;
	h = target / 8;

	int TradeBoxRows = 4;
	if(ExtentCheck(w,h,8,TradeBoxRows) == 0)
	{
		memcpy(lpObj->TradeMap,TempTradeMap,TRADE_BOX_SIZE);
		return -1;
	}

	if(*(BYTE*)(lpObj->TradeMap + h * 8 + w) == 255)
	{
		BYTE itembuf[8+5];
		blank = gObjTradeRectCheck(lpObj->m_Index,w,h,iwidth,iheight);

		if(blank == 255)
		{
			memcpy(lpObj->TradeMap,TempTradeMap,TRADE_BOX_SIZE);
			return -1;
		}

		lpObj->Trade[blank] = lpObj->Trade[source];
		lpObj->Trade[source].Clear();
		GCTradeOtherDel(lpObj->TargetNumber,source);
		gObjTradeItemBoxSet(lpObj->m_Index,blank,iwidth,iheight,lpObj->Trade[blank].m_Type);
		ItemByteConvert(itembuf,lpObj->Trade[blank]);
		GCTradeOtherAdd(lpObj->TargetNumber,blank,itembuf);
		return true;
	}

	memcpy(lpObj->TradeMap,TempTradeMap,TRADE_BOX_SIZE);
	return -1;
}






void gObjTradeCancel(int aIndex)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		if(ReadConfig.TradeLog == TRUE)
			TRADE_LOG.Output("Error : index[%d] error %s %d",aIndex,__FILE__,__LINE__);
		//LogAdd("Error : index error %s %d",__FILE__,__LINE__);
		return;
	}
	if(gObj[aIndex].Type != OBJ_USER || gObj[aIndex].Connected != PLAYER_PLAYING)
	{
		//if(ReadConfig.TradeLog == TRUE)
		//	TRADE_LOG.Output(lMsg.Get(535),gObj[aIndex].AccountID,gObj[aIndex].Name);
		//LogAdd(lMsg.Get(535),gObj[aIndex].AccountID,gObj[aIndex].Name);
		return;
	}
	if(gObj[aIndex].m_IfState.use != 1)
	{
		//if(ReadConfig.TradeLog == TRUE)
		//	TRADE_LOG.Output(lMsg.Get(536),gObj[aIndex].AccountID,gObj[aIndex].Name);
		//LogAdd(lMsg.Get(536),gObj[aIndex].AccountID,gObj[aIndex].Name);
		return;
	}
	if(gObj[aIndex].m_IfState.type != 1)
	{
		//if(ReadConfig.TradeLog == TRUE)
		//	TRADE_LOG.Output(lMsg.Get(537),gObj[aIndex].AccountID,gObj[aIndex].Name);
		//LogAdd(lMsg.Get(537),gObj[aIndex].AccountID,gObj[aIndex].Name);
		return;
	}
	gObjInventoryRollback(aIndex);
	gObj[aIndex].TargetNumber = -1;
	gObj[aIndex].m_IfState.use = 0;
	gObjCharTradeClear(&gObj[aIndex]);
	GCMoneySend(aIndex,gObj[aIndex].Money);
	GCItemListSend(aIndex);
	GCEquipmentSend(aIndex);
	GCMagicListMultiSend(&gObj[aIndex],0);
	gObjUseSkill.SkillChangeUse(aIndex);

	if(gObj[aIndex].GuildNumber > 0)
	{
		GCGuildViewportNowPaint(aIndex,gObj[aIndex].GuildName,0,0);
		gObjNotifyUpdateUnionV1(&gObj[aIndex]);
		gObjNotifyUpdateUnionV2(&gObj[aIndex]);
	}
	//LogAddTD(lMsg.Get(514),gObj[aIndex].AccountID,gObj[aIndex].Name);
	if(ReadConfig.TradeLog == TRUE)
		TRADE_LOG.Output(lMsg.Get(514),gObj[aIndex].AccountID,gObj[aIndex].Name);
}






void gObjTempInventoryItemBoxSet(BYTE * TempMap, int itempos, int xl, int yl, BYTE set_byte)
{
	int itemposx = (itempos - INVETORY_WEAR_SIZE) % 8;
	int itemposy = (itempos - INVETORY_WEAR_SIZE) / 8;

	for(int y = 0 ; y < yl ; y++)
	{
		for(int x = 0; x < xl; x++)
		{
			*(BYTE*)(TempMap + (itemposy +y) * 8 + (itemposx + x)) = set_byte;
		}
	}
}






BYTE gObjTempInventoryRectCheck(BYTE * TempMap, int sx, int sy, int width, int height)
{
	int x,y,blank = 0;

	if(sx + width > 8)
	{
		return -1;
	}
	if(sy + height > 8)
	{
		return -2;
	}

	if(ReadConfig.S6E2 == 1)
	{
		if(sy < 8 && sy+height > 8 )
		{
			return -1;
		}

		if(sy > 7 && sy < 12 && sy+height > 12)
		{
			return -1;
		}

		if(sy > 11 && sy < 16 && sy+height > 16)
		{
			return -1;
		}

		if(sy > 16 && sy < 20 && sy+height > 20)
		{
			return -1;
		}

		if(sy > 20 && sy < 24 && sy+height > 24)
		{
			return -1;
		}

		if(sy > 24 && sy < 28 && sy+height > 28)
		{
			return -1;
		}
	}

	for(y = 0; y < height; y++)
	{
		for(x = 0; x < width; x++)
		{
			if(*(BYTE*)(TempMap + (sy+y)*8 + (sx+x))!= 255)
			{
				blank += 1;
				return -1;
			}
		}
	}
	if(blank == 0)
	{
		return sx+sy*8+INVETORY_WEAR_SIZE;
	}
	return -1;
}






BYTE gObjTempInventoryInsertItem(LPOBJ lpObj, CItem item, BYTE * TempMap)
{
	int w,h,iwidth,iheight;
	BYTE blank = 0;

	if(item.GetSize(iwidth,iheight) == 0)
	{
		LogAdd(lMsg.Get(532),__FILE__,__LINE__,item.m_Type);
		return -1;
	}
	for(h = 0; h < 8; h++)
	{
		for(w = 0; w < 8; w++)
		{
			if(*(BYTE*)(TempMap + h * 8 + w) == 255)
			{
				blank = gObjTempInventoryRectCheck(TempMap,w,h,iwidth,iheight);

				if(blank == 254)
				{
					goto GOTO_EndFunc;
				}
				if(blank != 255)
				{
					gObjTempInventoryItemBoxSet(TempMap,blank,iwidth,iheight,item.m_Type);
					return blank;
				}
			}
		}
	}
GOTO_EndFunc:
	return -1;
}

BYTE gObjTempInventoryInsertItem(LPOBJ lpObj, int itemType, BYTE * TempMap)
{
	int w,h,iwidth,iheight;
	BYTE blank = 0;

	
	iwidth=ItemAttribute[itemType].Width;
	iheight=ItemAttribute[itemType].Height;

	for(h = 0; h < 8; h++)
	{
		for(w = 0; w < 8; w++)
		{
			if(*(BYTE*)(TempMap + h * 8 + w) == 255)
			{
				blank = gObjTempInventoryRectCheck(TempMap,w,h,iwidth,iheight);

				if(blank == 254)
				{
					goto GOTO_EndFunc;
				}
				if(blank != 255)
				{
					gObjTempInventoryItemBoxSet(TempMap,blank,iwidth,iheight,itemType);
					return blank;
				}
			}
		}
	}
GOTO_EndFunc:
	return -1;
}







BOOL TradeItemInventoryPutTest(int aIndex)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return false;
	}

	int number,result = 1;
	number = gObj[aIndex].TargetNumber;

	if(
#if (PACK_EDITION>=3)
		(gObj[number].IsBot == 0) && 
#endif
		gObjIsConnected(number) == 0)
	{
		return false;
	}

	//BYTE TempInventoryMap[BASIC_INVENTORY_SIZE + (MAX_EXTENDED_INV*EXTENDED_INV_SIZE)];
	BYTE TempInventoryMap[MAIN_INVENTORY_NORMAL_SIZE + (MAX_EXTENDED_INV*EXTENDED_INV_SIZE) + PSHOP_SIZE];
	memcpy(TempInventoryMap,gObj[aIndex].pInventoryMap,ReadConfig.GETSLOTS(aIndex));

	for(int n = 0; n < TRADE_BOX_SIZE; n++)
	{
		if(gObj[number].Trade[n].IsItem() == 1)
		{
			if(gObjTempInventoryInsertItem(&gObj[aIndex],gObj[number].Trade[n],TempInventoryMap) == 255)
			{
				return false;
			}
			result = 1;
		}
	}
	if(result == 1)
	{
		return true;
	}
	return false;
}






BOOL TradeitemInventoryPut(int aIndex)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return false;
	}

	int number;
	BYTE insert;
	char szItemName[50] = "Item";
	BYTE ExOption[8];

	number = gObj[aIndex].TargetNumber;

	if(gObjIsConnected(number) == 0)
	{
		return false;
	}

	for(int n = 0; n < TRADE_BOX_SIZE; n++)
	{
		if(gObj[number].Trade[n].IsItem() == 1)
		{
			insert = gObjInventoryInsertItem(aIndex,gObj[number].Trade[n]);
			if(insert != 255)
			{
				strcpy(szItemName,gObj[number].Trade[n].GetName());
				ItemIsBufExOption(ExOption,&gObj[number].Trade[n]);
				//LogAddTD(lMsg.Get(538),
				if(ReadConfig.TradeLog == TRUE)
				{
					TRADE_LOG.Output(lMsg.Get(MSGGET(2, 26)),
						gObj[number].AccountID,gObj[number].Name,gObj[number].MapNumber,gObj[number].X,gObj[number].Y,
						gObj[aIndex].AccountID,gObj[aIndex].Name,gObj[aIndex].MapNumber,gObj[aIndex].X,gObj[aIndex].Y,
						szItemName,gObj[number].Trade[n].m_Number,gObj[number].Trade[n].m_Level,gObj[number].Trade[n].m_SkillOption,gObj[number].Trade[n].m_LuckOption,gObj[number].Trade[n].m_Z28Option,
						ExOption[0],ExOption[1],ExOption[2],ExOption[3],ExOption[4],ExOption[5],ExOption[6],gObj[number].Trade[n].m_SetOption
					);
				}

				if(ReadConfig.GMLog == TRUE)
				{
					if (gObj[aIndex].AuthorityCode > 0 || gObj[number].AuthorityCode > 0)
					{
						GM_LOG.Output(lMsg.Get(MSGGET(2, 26)),
							gObj[number].AccountID,gObj[number].Name,gObj[number].MapNumber,gObj[number].X,gObj[number].Y,
							gObj[aIndex].AccountID,gObj[aIndex].Name,gObj[aIndex].MapNumber,gObj[aIndex].X,gObj[aIndex].Y,
							szItemName,gObj[number].Trade[n].m_Number,gObj[number].Trade[n].m_Level,gObj[number].Trade[n].m_SkillOption,gObj[number].Trade[n].m_LuckOption,gObj[number].Trade[n].m_Z28Option,
							ExOption[0],ExOption[1],ExOption[2],ExOption[3],ExOption[4],ExOption[5],ExOption[6],gObj[number].Trade[n].m_SetOption
						);
					}
				}
			}
			else
			{
				if(ReadConfig.TradeLog == TRUE)
					TRADE_LOG.Output("Error : [%s][%s] vs [%s][%s] TradeitemInventoryPut!",gObj[number].AccountID,gObj[number].Name,gObj[aIndex].AccountID,gObj[aIndex].Name);
				return false;
			}
		}
	}
	return true;
}






void gObjTradeOkButton(int aIndex)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	BYTE result = 1;
	int number = gObj[aIndex].TargetNumber;

	if(number < 0)
	{
		return;
	}

	gObj[aIndex].b_BlockMoveCommandSeconds = 60;
	gObj[number].b_BlockMoveCommandSeconds = 60;

	if(gObjIsConnected(number) == 0)
	{
		return;
	}

	if(gObj[number].TargetNumber != aIndex)
	{
		LogAdd(lMsg.Get(539),gObj[aIndex].AccountID,gObj[aIndex].Name,number,gObj[number].TargetNumber);
		return;
	}

	if(gObj[aIndex].TradeOk != 1 || gObj[number].TradeOk != 1)
	{
		return;
	}

	//gObj[aIndex].m_IfState.use = 0;
	//gObj[number].TargetNumber = -1;

	if(TradeItemInventoryPutTest(aIndex) == 1)
	{
		if(TradeItemInventoryPutTest(number) == 1)
		{
			result = 1;
		}
		else
		{
			result = 2;
		}
	}
	else
	{
		result = 2;
	}

	/*if(gObjCheckMaxZen(aIndex,gObj[number].TradeMoney) == 0)
	{
		result = 2;
	}

	if(gObjCheckMaxZen(number,gObj[aIndex].TradeMoney) == 0)
	{
		result = 2;
	}*/

	if(ReadConfig.ItemTradeJOHBlock == TRUE)
	{
		if(g_kJewelOfHarmonySystem.IsEnableToTrade(&gObj[number]) == 0 || g_kJewelOfHarmonySystem.IsEnableToTrade(&gObj[aIndex]) == 0)
		{
			result = 4;
			gObjTradeCancel(aIndex);
			gObjTradeCancel(number);
			CGTradeResult(aIndex,result);
			CGTradeResult(number,result);
			return;
		}
	}

	if(result == 1)
	{
		TradeitemInventoryPut(aIndex);
		TradeitemInventoryPut(number);
		gObjInventoryCommit(aIndex);
		gObjInventoryCommit(number);
		gObjMakePreviewCharSet(aIndex);
		gObjMakePreviewCharSet(number);

		gObj[aIndex].Money -= gObj[aIndex].TradeMoney;
		if(gObj[aIndex].Money < 0)
		{
			gObj[aIndex].Money = 0;
		}
		gObj[number].Money -= gObj[number].TradeMoney;
		if(gObj[number].Money < 0)
		{
			gObj[number].Money = 0;
		}

		__int64 tmpZen = (__int64)gObj[aIndex].Money + (__int64)gObj[number].TradeMoney;
		if (tmpZen > (__int64)MAX_ZEN)
		{
			gObj[aIndex].Money = MAX_ZEN;
		} else {
			gObj[aIndex].Money += gObj[number].TradeMoney;
		}

		tmpZen = (__int64)gObj[number].Money + (__int64)gObj[aIndex].TradeMoney;
		if ( tmpZen > MAX_ZEN)
		{
			gObj[number].Money = MAX_ZEN;
		} else {
			gObj[number].Money += gObj[aIndex].TradeMoney;
		}

		//LogAddTD(lMsg.Get(540),gObj[aIndex].AccountID,gObj[aIndex].Name,gObj[number].AccountID,gObj[number].Name,gObj[aIndex].TradeMoney,gObj[number].TradeMoney);
		if(ReadConfig.TradeLog == TRUE)
		{
			TRADE_LOG.Output(lMsg.Get(MSGGET(2, 28)),
				gObj[aIndex].AccountID,gObj[aIndex].Name,gObj[number].AccountID,gObj[number].Name,gObj[aIndex].TradeMoney,gObj[number].TradeMoney
			);
		}

		if(ReadConfig.GMLog == TRUE)
		{
			if (gObj[aIndex].AuthorityCode > 0 || gObj[number].AuthorityCode > 0)
			{
					GM_LOG.Output(lMsg.Get(MSGGET(2, 28)),
						gObj[aIndex].AccountID,gObj[aIndex].Name,gObj[number].AccountID,gObj[number].Name,gObj[aIndex].TradeMoney,gObj[number].TradeMoney
					);
			}
		}

		GJSetCharacterInfo(&gObj[aIndex],aIndex,0);
		gObjItemTextSave(&gObj[aIndex]);
		gObjStatTextSave(&gObj[aIndex]);
		gObjSavePetItemInfo(aIndex, 0);

		GJSetCharacterInfo(&gObj[number],number,0);
		gObjItemTextSave(&gObj[number]);
		gObjStatTextSave(&gObj[number]);
		gObjSavePetItemInfo(number, 0);
	}
	else
	{
		result = 2;
		gObjTradeCancel(aIndex);
		gObjTradeCancel(number);
		CGTradeResult(aIndex,result);
		CGTradeResult(number,result);
		return;
	}

	gObj[aIndex].TargetNumber = -1;
	gObj[aIndex].m_IfState.use = 0;
	gObj[number].TargetNumber = -1;
	gObj[number].m_IfState.use = 0;
	gObj[aIndex].TradeOk = 0;
	gObj[number].TradeOk = 0;
	gObj[aIndex].TradeMoney = 0;
	gObj[number].TradeMoney = 0;

	GCMoneySend(aIndex,gObj[aIndex].Money);
	GCMoneySend(number,gObj[number].Money);
	CGTradeResult(aIndex,result);
	CGTradeResult(number,result);

	GCItemListSend(aIndex);
	GCItemListSend(number);

	for(int n = 0; n < TRADE_BOX_SIZE; n++)
	{
		gObj[aIndex].Trade[n].Clear();
		gObj[number].Trade[n].Clear();
	}

	gObjNotifyUpdateUnionV1(&gObj[aIndex]);
	gObjNotifyUpdateUnionV2(&gObj[aIndex]);
}






BYTE LevelSmallConvert(int aIndex, int inventoryindex)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return 0;
	}

	int level = gObj[aIndex].pInventory[inventoryindex].m_Level;

	if(level >= 15)
	{
		return 7;
	}

	if(level >= 13 && level <= 14)
	{
		return 6;
	}

	if(level >= 11 && level <= 12)
	{
		return 5;
	}

	if(level >= 9 && level <= 10)
	{
		return 4;
	}

	if(level >= 7 && level <= 8)
	{
		return 3;
	}

	if(level >= 5 && level <= 6)
	{
		return 2;
	}

	if(level >= 3 && level <= 4)
	{
		return 1;
	}
	
	return 0;
}






BYTE LevelSmallConvert(int level)
{
	if(level >= 15)
	{
		return 7;
	}

	if(level >= 13 && level <=14)
	{
		return 6;
	}

	if(level >= 11 && level <=12)
	{
		return 5;
	}

	if(level >= 9 && level <= 10)
	{
		return 4;
	}

	if(level >= 7 && level <= 8)
	{
		return 3;
	}

	if(level >= 5 && level <= 6)
	{
		return 2;
	}

	if(level >= 3 && level <= 4)
	{
		return 1;
	}
	
	return 0;
}






void gObjAttackDamageCalc(int aIndex) 
{
	return;	// Here goes a Macro
}


// TEST FUNCTION
char gObjFixClassPacket(int DbClass)
{
 char Preview = 0;

	switch(DbClass)
	{
	case DB_DARK_WIZARD:
		Preview = PR_DARK_WIZARD;
		break;

	case DB_SOUL_MASTER:
		Preview = PR_SOUL_MASTER;
		break;

	case DB_GRAND_MASTER:
		Preview = PR_GRAND_MASTER;
		break;

	case DB_DARK_KNIGHT:
		Preview = PR_DARK_KNIGHT;
		break;

	case DB_BLADE_KNIGHT:
		Preview = PR_BLADE_KNIGHT;
		break;

	case DB_BLADE_MASTER:
		Preview = PR_BLADE_MASTER;
		break;

	case DB_FAIRY_ELF:
		Preview = PR_FAIRY_ELF;
		break;

	case DB_MUSE_ELF:
		Preview = PR_MUSE_ELF;
		break;

	case DB_HIGH_ELF:
		Preview = PR_HIGH_ELF;
		break;

	case DB_MAGIC_GLADIATOR:
		Preview = PR_MAGIC_GLADIATOR;
		break;

	case DB_DUEL_MASTER:
		Preview = PR_DUEL_MASTER;
		break;

	case DB_DUEL_MASTER+1:
		Preview = PR_DUEL_MASTER;
		break;
		
	case DB_DARK_LORD:
		Preview = PR_DARK_LORD;
		break;

	case DB_LORD_EMPEROR:
		Preview = PR_LORD_EMPEROR;
		break;

	case DB_LORD_EMPEROR+1:
		Preview = PR_LORD_EMPEROR;
		break;

	case DB_SUMMONER:
		Preview = PR_SUMMONER;
		break;

	case DB_BLOODY_SUMMONER:
		Preview = PR_BLOODY_SUMMONER;
		break;
	case DB_DIMENSION_MASTER:
		Preview = PR_DIMENSION_MASTER;
		break;

	case DB_RAGEFIGHER:
		Preview = PR_RAGEFIGHER;
		break;

	case DB_RAGEFIGHER_EVO:
	case DB_RAGEFIGHER_EVO+1:
		Preview = PR_RAGEFIGHER_EVO;
		break;
	}

	return Preview;
}

void gObjMakePreviewCharSet(int aIndex)
{
	if ( !OBJMAX_RANGE(aIndex ))
	{
		LogAdd("error : %s %d", __FILE__, __LINE__);
		return;
	}

	BYTE index;
	LPOBJ lpObj = &gObj[aIndex];
	memset(lpObj->CharSet, 0, sizeof(lpObj->CharSet));

	lpObj->CharSet[CS_CLASS] = gObjFixClassPacket(lpObj->DbClass);// CS_SET_CLASS(lpObj->Class);//gObjFixClassPacket(lpObj->DbClass);//CS_SET_CLASS(lpObj->Class);//Test(lpObj->Class);
	lpObj->CharSet[CS_CLASS] |= CS_SET_CHANGEUP(lpObj->ChangeUP);

	// Set Class
	if ( lpObj->m_ActionNumber == 0x80 )
	{
		lpObj->CharSet[CS_CLASS] |= 2; 
	}
	else if ( lpObj->m_ActionNumber == 0x81 )
	{
		lpObj->CharSet[CS_CLASS] |= 3;
	}
	else
	{
		lpObj->CharSet[CS_CLASS] = lpObj->CharSet[CS_CLASS];
	}

	// Set Righth HAnd Item
	if ( lpObj->pInventory[0].m_Type < ITEMGET(0,0) )
	{
		lpObj->CharSet[CS_WEAPON1_DATA] |= (BYTE)DBI_GET_TYPE(-1);
		lpObj->CharSet[CS_WEAPON1_TYPE] = (BYTE)DBI_GET_INDEX(-1);
	}
	else
	{
		lpObj->CharSet[CS_WEAPON1_DATA] |= DBI_GET_TYPE(lpObj->pInventory[0].m_Type);
		lpObj->CharSet[CS_WEAPON1_TYPE] = DBI_GET_INDEX(lpObj->pInventory[0].m_Type);
	}

	// Set Left Hand
	if ( lpObj->pInventory[1].m_Type < ITEMGET(0,0) )
	{
		lpObj->CharSet[CS_WEAPON2_DATA] |= (BYTE)DBI_GET_TYPE(-1);
		lpObj->CharSet[CS_WEAPON2_TYPE] = (BYTE)DBI_GET_INDEX(-1);
	}
	else
	{
		lpObj->CharSet[CS_WEAPON2_DATA] |= DBI_GET_TYPE(lpObj->pInventory[1].m_Type);
		lpObj->CharSet[CS_WEAPON2_TYPE] = DBI_GET_INDEX(lpObj->pInventory[1].m_Type);
	}

	// Set Helmet
	if ( lpObj->pInventory[2].m_Type < ITEMGET(0,0) )
	{
		lpObj->CharSet[13] |= CS_SET_HELMET1(-1);
		lpObj->CharSet[9]  |= CS_SET_HELMET2(-1);
		lpObj->CharSet[3]  |= CS_SET_HELMET3(-1);
	}
	else
	{
		lpObj->CharSet[13] |= CS_SET_HELMET1(lpObj->pInventory[2].m_Type);
		lpObj->CharSet[9]  |= CS_SET_HELMET2(lpObj->pInventory[2].m_Type);
		lpObj->CharSet[3]  |= CS_SET_HELMET3(lpObj->pInventory[2].m_Type);
	}

	// Set Armor
	if ( lpObj->pInventory[3].m_Type < ITEMGET(0,0) )
	{
		lpObj->CharSet[14] |= CS_SET_ARMOR1(-1);
		lpObj->CharSet[9]  |= CS_SET_ARMOR2(-1);
		lpObj->CharSet[3]  |= CS_SET_ARMOR3(-1);
	}
	else
	{
		lpObj->CharSet[14] |= CS_SET_ARMOR1(lpObj->pInventory[3].m_Type);
		lpObj->CharSet[9]  |= CS_SET_ARMOR2(lpObj->pInventory[3].m_Type);
		lpObj->CharSet[3]  |= CS_SET_ARMOR3(lpObj->pInventory[3].m_Type);
	}

	// Set Pants
	if ( lpObj->pInventory[4].m_Type < ITEMGET(0,0) )
	{
		lpObj->CharSet[14] |= CS_SET_PANTS1(-1);
		lpObj->CharSet[9]  |= CS_SET_PANTS2(-1);
		lpObj->CharSet[4]  |= CS_SET_PANTS3(-1);
	}
	else
	{
		lpObj->CharSet[14] |= CS_SET_PANTS1(lpObj->pInventory[4].m_Type);
		lpObj->CharSet[9]  |= CS_SET_PANTS2(lpObj->pInventory[4].m_Type);
		lpObj->CharSet[4]  |= CS_SET_PANTS3(lpObj->pInventory[4].m_Type);
	}

	// Set Gloves
	if ( lpObj->pInventory[5].m_Type < ITEMGET(0,0) )
	{
		lpObj->CharSet[15] |= CS_SET_GLOVES1(-1);
		lpObj->CharSet[9]  |= CS_SET_GLOVES2(-1);
		lpObj->CharSet[4]  |= CS_SET_GLOVES3(-1);
	}
	else
	{
		lpObj->CharSet[15] |= CS_SET_GLOVES1(lpObj->pInventory[5].m_Type);
		lpObj->CharSet[9]  |= CS_SET_GLOVES2(lpObj->pInventory[5].m_Type);
		lpObj->CharSet[4]  |= CS_SET_GLOVES3(lpObj->pInventory[5].m_Type);
	}

	// Set Boots
	if ( lpObj->pInventory[6].m_Type < ITEMGET(0,0) )
	{
		lpObj->CharSet[15] |= CS_SET_BOOTS1(-1);
		lpObj->CharSet[9]  |= CS_SET_BOOTS2(-1);
		lpObj->CharSet[5]  |= CS_SET_BOOTS3(-1);
	}
	else
	{
		lpObj->CharSet[15] |= CS_SET_BOOTS1(lpObj->pInventory[6].m_Type);
		lpObj->CharSet[9]  |= CS_SET_BOOTS2(lpObj->pInventory[6].m_Type);
		lpObj->CharSet[5]  |= CS_SET_BOOTS3(lpObj->pInventory[6].m_Type);
	}

#if (CRYSTAL_EDITION==1)
	BYTE NewWings = 0;
#endif
/*
Season 6 Episode 1:

Wings Fixed:

Lvl 1:
DW,DK,ELF,SU

Lvl 2:
DW,DK,MG,SU,DL,RF,Elf

Lvl3:
DW,DK,ELF,SU,MG,DL,RF

*/

	if(ReadConfig.S6E1 == 1)
	{
		switch(lpObj->pInventory[7].m_Type)
		{
			case ITEMGET(12,0):
			case ITEMGET(12,1):
			case ITEMGET(12,4):
			case ITEMGET(12,5):
			case ITEMGET(12,6):
			case ITEMGET(12,41):
			case ITEMGET(12,49):
			{
				lpObj->CharSet[5] |= 4;
				BYTE wID = (lpObj->pInventory[7].m_Type - ITEMGET(12,0))+1;
				if(lpObj->pInventory[7].m_Type == ITEMGET(12,41))
					lpObj->CharSet[9] |= 4;
				else if(lpObj->pInventory[7].m_Type == ITEMGET(12,49))
					lpObj->CharSet[9] |= 7;
				else
					lpObj->CharSet[9] |= wID;
			}break;
			case ITEMGET(12,3):
			{
				lpObj->CharSet[9] |= 1;
			}break;
			case ITEMGET(12,42):
			{
				lpObj->CharSet[9] |= 6;
			}break;
			case ITEMGET(13,30):
			{
				lpObj->CharSet[9] |= 5;
			}break;

			case ITEMGET(12,36):
			case ITEMGET(12,37):
			case ITEMGET(12,38):
			case ITEMGET(12,39):
			case ITEMGET(12,40):
			case ITEMGET(12,43):
			case ITEMGET(12,50):
			{
				lpObj->CharSet[5] |= CS_SET_WING1(-1);

				BYTE wID = (lpObj->pInventory[7].m_Type - ITEMGET(12,36))+1;
				if(lpObj->pInventory[7].m_Type == ITEMGET(12,43))
					lpObj->CharSet[9] |= 6;
				else if(lpObj->pInventory[7].m_Type == ITEMGET(12,50))
					lpObj->CharSet[9] |= 7;
				else
					lpObj->CharSet[9] |= wID;
			}break;
		};
#if (CRYSTAL_EDITION==1)
		if(lpObj->pInventory[7].m_Type >= ITEMGET(12,200) && lpObj->pInventory[7].m_Type <= ITEMGET(12,254))
		{
			for(int i=0;i<55;i++)
			{
				if ( lpObj->pInventory[7].m_Type == ITEMGET(12,(i+200)) )
				{
					lpObj->CharSet[9] |= 4;
					NewWings = ((i+1)*4);
					break;
				}
			}
		}
#endif
	}

	index = 0;

	// Set Part onf Wings
	if ( lpObj->pInventory[7].m_Type < ITEMGET(0,0) )
	{
		if(ReadConfig.S6E1 == 0)
			index |= CS_SET_WING1(-1);
	}
	else
	{
		index |= CS_SET_WING1(lpObj->pInventory[7].m_Type);
	}

	if ( lpObj->pInventory[8].m_Type != ITEMGET(13,4) )
	{
		if ( lpObj->pInventory[8].m_Type < ITEMGET(0,0) )
		{
			index |= CS_SET_HELPER(-1);
		}
		else
		{
			index |= CS_SET_HELPER(lpObj->pInventory[8].m_Type);
		}
	}
	else
	{
		index |= CS_SET_HELPER(-1);
	}
	
	if(ReadConfig.S6E1 == 0)
	{
		lpObj->CharSet[5] |= index;
	}else
	{
		switch(lpObj->pInventory[7].m_Type)
		{
			case ITEMGET(12,49):
			{
				lpObj->CharSet[5] += index;
				//lpObj->CharSet[5] |= 7;
			}break;
			case ITEMGET(12,3):
			{
				if ( lpObj->pInventory[8].m_Type != ITEMGET(13,4) )
				{
					if ( lpObj->pInventory[8].m_Type < ITEMGET(0,0) )
					{
						lpObj->CharSet[5] |= 0x0B;
					}
					else
					{
						lpObj->CharSet[5] |= 0x08;
						lpObj->CharSet[5] +=  CS_SET_HELPER(lpObj->pInventory[8].m_Type);
					}
				}
			}break;

			case ITEMGET(12,0):
			case ITEMGET(12,1):
			case ITEMGET(12,4):
			case ITEMGET(12,5):
			case ITEMGET(12,6):
			{
				if ( lpObj->pInventory[8].m_Type != ITEMGET(13,4) )
				{
					if ( lpObj->pInventory[8].m_Type < ITEMGET(0,0) )
					{
						lpObj->CharSet[5] |= 0x03;
					}
					else
					{
						lpObj->CharSet[5] +=  CS_SET_HELPER(lpObj->pInventory[8].m_Type);
					}
				}
			}break;
			default:
			{
				lpObj->CharSet[5] |= index;
			}break;
		};
	}

	int levelindex = CS_SET_SMALLLEVEL_RH(LevelSmallConvert(aIndex, 0));
	levelindex |= CS_SET_SMALLLEVEL_LH(LevelSmallConvert(aIndex, 1));
	levelindex |= CS_SET_SMALLLEVEL_HELMET(LevelSmallConvert(aIndex, 2));
	levelindex |= CS_SET_SMALLLEVEL_ARMOR(LevelSmallConvert(aIndex, 3));
	levelindex |= CS_SET_SMALLLEVEL_PANTS(LevelSmallConvert(aIndex, 4));
	levelindex |= CS_SET_SMALLLEVEL_GLOVES(LevelSmallConvert(aIndex, 5));
	levelindex |= CS_SET_SMALLLEVEL_BOOTS(LevelSmallConvert(aIndex, 6));

	lpObj->CharSet[6] = CS_SET_SMALLLEVEL1(levelindex);
	lpObj->CharSet[7] = CS_SET_SMALLLEVEL2(levelindex);
	lpObj->CharSet[8] = CS_SET_SMALLLEVEL3(levelindex);

	if(ReadConfig.S6E1 == 0)
	{
		if ( (lpObj->pInventory[7].m_Type >= ITEMGET(12,3) && lpObj->pInventory[7].m_Type <= ITEMGET(12,6)) || lpObj->pInventory[7].m_Type == ITEMGET(12,41) ||
#if (CRYSTAL_EDITION==1)
		(lpObj->pInventory[7].m_Type >= ITEMGET(12,200) && lpObj->pInventory[7].m_Type <= ITEMGET(12,254)) || 
#endif
			  lpObj->pInventory[7].m_Type == ITEMGET(13,30) || lpObj->pInventory[7].m_Type >= ITEMGET(12,36) && lpObj->pInventory[7].m_Type <= ITEMGET(12,40) || (lpObj->pInventory[7].m_Type >= ITEMGET(12,42) && lpObj->pInventory[7].m_Type <= ITEMGET(12,43)) )
		{
			lpObj->CharSet[5] |= CS_SET_WING1(-1);
			
			if ( lpObj->pInventory[7].m_Type == ITEMGET(13,30) )
			{
				lpObj->CharSet[9] |= 5;
			}
			
			else if ( lpObj->pInventory[7].m_Type == ITEMGET(12,41) )
			{
				lpObj->CharSet[9] |= 6;
			}
#if (CRYSTAL_EDITION==1)
			else if(lpObj->pInventory[7].m_Type >= ITEMGET(12,200) && lpObj->pInventory[7].m_Type <= ITEMGET(12,254) )
			{
				for(int i=0;i<55;i++)
				{
					if ( lpObj->pInventory[7].m_Type == ITEMGET(12,(i+200)) )
					{
						lpObj->CharSet[9] |= 4;
						NewWings = ((i+1)*4);
						break;
					}
				}
				//else if ( lpObj->pInventory[7].m_Type == ITEMGET(12,200) )
				//{
				//	lpObj->CharSet[9] |= 4;
				//	NewWings = 4;
				//}
				//else if ( lpObj->pInventory[7].m_Type == ITEMGET(12,201) )
				//{
				//	lpObj->CharSet[9] |= 4;
				//	NewWings = 8;
				//}
				//else if ( lpObj->pInventory[7].m_Type == ITEMGET(12,201) )
				//{
				//	lpObj->CharSet[9] |= 4;
				//	NewWings = 8;
				//}
			}
#endif
	
			else
			{
				lpObj->CharSet[9] |= (lpObj->pInventory[7].m_Type - 2) & 0x07;
			}
		}
	}

	// Set Excellent Items
	lpObj->CharSet[10] = 0;

	if ( lpObj->pInventory[2].IsExtItem() != FALSE )
		lpObj->CharSet[10] = (char)0x80;

	if ( lpObj->pInventory[3].IsExtItem() != FALSE )
		lpObj->CharSet[10] |= 0x40;

	if ( lpObj->pInventory[4].IsExtItem() != FALSE )
		lpObj->CharSet[10] |= 0x20;

	if ( lpObj->pInventory[5].IsExtItem() != FALSE )
		lpObj->CharSet[10] |= 0x10;

	if ( lpObj->pInventory[6].IsExtItem() != FALSE )
		lpObj->CharSet[10] |= 0x8;

	if ( lpObj->pInventory[0].IsExtItem() != FALSE )
		lpObj->CharSet[10] |= 0x04;

	if ( lpObj->pInventory[1].IsExtItem() != FALSE )
		lpObj->CharSet[10] |= 0x02;

	// Set Set Items
	lpObj->CharSet[11] = 0;

	if ( lpObj->pInventory[2].IsSetItem() != FALSE )
		lpObj->CharSet[11] = (char)0x80;

	if ( lpObj->pInventory[3].IsSetItem() != FALSE )
		lpObj->CharSet[11] |= 0x40;

	if ( lpObj->pInventory[4].IsSetItem() != FALSE )
		lpObj->CharSet[11] |= 0x20;

	if ( lpObj->pInventory[5].IsSetItem() != FALSE )
		lpObj->CharSet[11] |= 0x10;

	if ( lpObj->pInventory[6].IsSetItem() != FALSE )
		lpObj->CharSet[11] |= 0x8;

	if ( lpObj->pInventory[0].IsSetItem() != FALSE )
		lpObj->CharSet[11] |= 0x04;

	if ( lpObj->pInventory[1].IsSetItem() != FALSE )
		lpObj->CharSet[11] |= 0x02;

	gObjCalCharacter(aIndex);

	if ( lpObj->IsFullSetItem != false )
		lpObj->CharSet[11] |= 0x01;

/*
13
	0	"Guardian Angel"
	1	"Imp"
	2	"Horn of Uniria"
	3	"Horn of Dinorant"
	4	"Horn of Dark Horse"
*/
	if ( (lpObj->pInventory[8].m_Type & 0x03) != 0 )
	{
		if ( lpObj->pInventory[8].m_Type > 0 )
		{
			lpObj->CharSet[10] |= 0x01;
		}
	}

	if ( lpObj->pInventory[8].m_Type == ITEMGET(13,4) )	// Dark Horse
	{
		lpObj->CharSet[12] |= 0x01;
	}

	if ( lpObj->pInventory[8].m_Type == ITEMGET(13,37) )	// Fenrir
	{
		lpObj->CharSet[10] &= 0xFE;
		lpObj->CharSet[12] &= 0xFE;
		lpObj->CharSet[12] |= 0x04;
		lpObj->CharSet[16] = 0;
		lpObj->CharSet[17] = 0;

		if ( lpObj->pInventory[8].IsFenrirIncLastAttackDamage() != FALSE )
		{
			lpObj->CharSet[16] |= 0x01;
		}

		if ( lpObj->pInventory[8].IsFenrirDecLastAttackDamage() != FALSE )
		{
			lpObj->CharSet[16] |= 0x02;
		}

		if ( lpObj->pInventory[8].IsFenrirIllusion() != FALSE )
		{
			//lpObj->CharSet[16] |= 0x04;
			lpObj->CharSet[17] = 0x01;//GoldFenrir
		}
	}else if(lpObj->pInventory[8].m_Type == ITEMGET(13,64) )//NEW PET VISUAL BUG
	{
		lpObj->CharSet[5] &= 0xFC;
		lpObj->CharSet[16] |= 0x20;
	}else if(lpObj->pInventory[8].m_Type == ITEMGET(13,65) )
	{
		lpObj->CharSet[5] &= 0xFD;
		lpObj->CharSet[10] |= 0x01;
		lpObj->CharSet[16] |= 0x40;
	}else if(lpObj->pInventory[8].m_Type == ITEMGET(13,67) )
	{
		lpObj->CharSet[5] &= 0xFC;
		lpObj->CharSet[10] |= 0x01;
		lpObj->CharSet[16] |= 0x80;
	}
	else if(lpObj->pInventory[8].m_Type == ITEMGET(13,80) ) //Panda Visual Bug
	{
		lpObj->CharSet[5] &= 0xFC;
		lpObj->CharSet[10] |= 0x01;
		lpObj->CharSet[16] |= 0xE0;
	}
	else if(lpObj->pInventory[8].m_Type == ITEMGET(13,106) ) //Unicorn Visual Bug
	{
		lpObj->CharSet[5] &= 0xFC;
		lpObj->CharSet[10] |= 0x01;
		lpObj->CharSet[16] |= 0xA0;
	}
	else if(lpObj->pInventory[8].m_Type == ITEMGET(13,123) ) //Skeleton Visual Bug
	{
		lpObj->CharSet[5] &= 0xFC;
		lpObj->CharSet[10] |= 0x01;
		lpObj->CharSet[16] |= 0x60;
	}
	
	if (lpObj->pInventory[7].m_Type == ITEMGET(12,36))
	{
		lpObj->CharSet[16] |= 4;
	}

	else if (lpObj->pInventory[7].m_Type == ITEMGET(12,37))
	{
		lpObj->CharSet[16] |= 8;
	}

	else if (lpObj->pInventory[7].m_Type == ITEMGET(12,38))
	{
		lpObj->CharSet[16] |= 12;
	}

	else if (lpObj->pInventory[7].m_Type == ITEMGET(12,39))
	{
		lpObj->CharSet[16] |= 16;
	}

	else if (lpObj->pInventory[7].m_Type == ITEMGET(12,40))
	{
		lpObj->CharSet[16] |= 20;
	}

	else if (lpObj->pInventory[7].m_Type == ITEMGET(12,42))
	{
		lpObj->CharSet[16] |= 28;
	}

	else if (lpObj->pInventory[7].m_Type == ITEMGET(12,43))
	{
		lpObj->CharSet[16] |= 24;
	}

	else if (lpObj->pInventory[7].m_Type == ITEMGET(12,130))
	{
		lpObj->CharSet[5] += 0x04;
		lpObj->CharSet[17] |= 32;
	}

	else if (lpObj->pInventory[7].m_Type == ITEMGET(12,131))
	{
		lpObj->CharSet[17] |= 64;
	}

	else if (lpObj->pInventory[7].m_Type == ITEMGET(12,132))
	{
		lpObj->CharSet[5] += 0x0C;
		lpObj->CharSet[17] |= 96;
	}

	else if (lpObj->pInventory[7].m_Type == ITEMGET(12,133))
	{
		lpObj->CharSet[5] += 0x08;
		lpObj->CharSet[17] |= 128;
	}

	else if (lpObj->pInventory[7].m_Type == ITEMGET(12,134))
	{
		lpObj->CharSet[5] += 0x04;
		lpObj->CharSet[17] |= 160;
	}

	else if (lpObj->pInventory[7].m_Type == ITEMGET(12,135))
	{
		//lpObj->CharSet[5] += 0x04;
		lpObj->CharSet[17] |= 192;
	}

#if(CRYSTAL_EDITION ==1)
	if(NewWings > 0)
	{
		if (lpObj->CharSet[17] + NewWings <= 0xFF)
			lpObj->CharSet[17] += NewWings;
	}
#endif



}








void gObjViewportPaint(HWND hWnd, short aIndex)
{
	int n;
	HDC hdc;
	//char szTemp[256];
	int count = 0;
	int count2 = 0;
	int count3 = 0;
	int playerc = 0;
	int totalplayer = 0;

	if ( !OBJMAX_RANGE(aIndex))
		return;

	hdc = GetDC(hWnd);

	for ( n=0;n<OBJMAX;n++)
	{
		if ( gObj[n].Live != FALSE )
		{
			if ( gObj[n].Connected != PLAYER_EMPTY )
			{
				if ( gCurPaintMapNumber == gObj[n].MapNumber )
				{
					if ( gObj[n].Type == OBJ_USER )
					{
						playerc++;
					}
				}
			}
		}

		if ( gObj[n].Type == OBJ_USER && gObj[n].Connected != PLAYER_EMPTY )
		{
			totalplayer++;
		}
		else if ( gObj[n].Connected != PLAYER_EMPTY )
		{
			count++;
		}
	}				

	if ( gCurPaintType == 1 )
	{
		RECT rect;
		int iStartX = 100;
		int iStartY = 50;
		int iWidth	= 1;
		int iHeight	= 1;
		int iMagnifying = 3;
		int x;
		int y;

		hdc = GetDC(hWnd);

		HBRUSH hCharacterBrush = CreateSolidBrush(RGB(255, 0, 0));
		HBRUSH hMonsterBrush = CreateSolidBrush(RGB(128, 128, 128));
		HBRUSH hNpcBrush = CreateSolidBrush(RGB(0, 255, 255));
		HBRUSH hItemBrush = CreateSolidBrush(RGB(0, 0, 255));
		HBRUSH hCrywolfMovePath = CreateSolidBrush(RGB(0, 255, 0));
		int iOldBkMode = SetBkMode(hdc,TRANSPARENT);
		
		// Set a Gray backgraound on non-walkable areas
		if ( MapC[gCurPaintMapNumber].m_attrbuf != NULL )
		{
			for ( y=0;y<255;y++)
			{
				for (x=0;x<255;x++)
				{
					if ( (MapC[gCurPaintMapNumber].m_attrbuf[y*256+x] & 4) ==4 || (MapC[gCurPaintMapNumber].m_attrbuf[y*256+x]&8) == 8)
					{
						rect.left = iStartX+y*iWidth*iMagnifying;
						rect.right = (iStartX+y*iWidth*iMagnifying) + (iWidth*iMagnifying);
						rect.top = iStartY+x*iHeight*iMagnifying;
						rect.bottom = (iStartY+x*iHeight*iMagnifying) + (iHeight*iMagnifying);

						FillRect(hdc, &rect, (HBRUSH)GetStockObject(LTGRAY_BRUSH));
					}
				}
			}
		}
//#error message("put else if gObjViewportPaint")
		//27F1C
		if(gCurPaintMapNumber == MAP_INDEX_CRYWOLF_FIRSTZONE)
		{
			if ( TMonsterAIElement::s_MonsterAIMovePath[MAP_INDEX_CRYWOLF_FIRSTZONE].m_bDataLoad != FALSE )
			{
				TMonsterAIMovePath & MovePath = TMonsterAIElement::s_MonsterAIMovePath[MAP_INDEX_CRYWOLF_FIRSTZONE];

				for ( int i=0;i<MovePath.m_iMovePathSpotCount;i++)
				{
					rect.left = iStartX+MovePath.m_MovePathInfo[i].m_iPathY*iWidth*iMagnifying;
					rect.right = (iStartX+MovePath.m_MovePathInfo[i].m_iPathY*iWidth*iMagnifying) + (iWidth*iMagnifying);
					rect.top = iStartY+MovePath.m_MovePathInfo[i].m_iPathX*iHeight*iMagnifying;
					rect.bottom = (iStartY+MovePath.m_MovePathInfo[i].m_iPathX*iHeight*iMagnifying) + (iHeight*iMagnifying);

					FillRect(hdc, &rect, hCrywolfMovePath);

					CString szDesc;
					szDesc.Format("(%d,%d)", MovePath.m_MovePathInfo[i].m_iPathX, MovePath.m_MovePathInfo[i].m_iPathY);
				}
			}
		}

		for ( n=0;n<OBJMAX;n++)
		{
			if ( gObj[n].Live != FALSE )
			{
				if ( gObj[n].Connected != PLAYER_EMPTY )
				{
					if ( gCurPaintMapNumber == gObj[n].MapNumber )
					{
						if ( gObj[n].Type == OBJ_USER )
						{
							rect.left = iStartX+gObj[n].Y*iWidth*iMagnifying;
							rect.right = (iStartX+gObj[n].Y*iWidth*iMagnifying) + ((iWidth+2)*iMagnifying);
							rect.top = iStartY+gObj[n].X*iHeight*iMagnifying;
							rect.bottom = (iStartY+gObj[n].X*iHeight*iMagnifying) + ((iHeight+2)*iMagnifying);

							FillRect(hdc, &rect, hCharacterBrush);

							CString szName;
							szName.Format("%s (%d,%d)", gObj[n].Name, gObj[n].X, gObj[n].Y);
							TextOut(hdc, rect.left, rect.bottom, szName, szName.GetLength());
						}
						else if ( gObj[n].Type == OBJ_MONSTER )
						{
							rect.left = iStartX+gObj[n].Y*iWidth*iMagnifying;
							rect.right = (iStartX+gObj[n].Y*iWidth*iMagnifying) + ((iWidth)*iMagnifying);
							rect.top = iStartY+gObj[n].X*iHeight*iMagnifying;
							rect.bottom = (iStartY+gObj[n].X*iHeight*iMagnifying) + ((iHeight)*iMagnifying);

							FillRect(hdc, &rect, hMonsterBrush);

							if ( gObj[n].m_iCurrentAI != 0 )
							{
								if ( gObj[n].m_iGroupMemberGuid == 0 )
								{
									FillRect(hdc, &rect, hCharacterBrush);

									CString szDesc;
									szDesc.Format("G(%d,%d)", gObj[n].m_iGroupNumber, gObj[n].m_iGroupMemberGuid );
									TextOut(hdc, rect.left, rect.bottom, szDesc, szDesc.GetLength());
								}
							}
						}
						else if ( gObj[n].Type == OBJ_NPC )
						{
							rect.left = iStartX+gObj[n].Y*iWidth*iMagnifying;
							rect.right = (iStartX+gObj[n].Y*iWidth*iMagnifying) + ((iWidth)*iMagnifying);
							rect.top = iStartY+gObj[n].X*iHeight*iMagnifying;
							rect.bottom = (iStartY+gObj[n].X*iHeight*iMagnifying) + ((iHeight)*iMagnifying);

							FillRect(hdc, &rect, hNpcBrush);
						}
					}
				}
			}
		}

		for ( int n=0;n<MAX_MAPITEM;n++)
		{
			if ( MapC[gCurPaintMapNumber].m_cItem[n].IsItem() )
			{
				rect.left = iStartX+MapC[gCurPaintMapNumber].m_cItem[n].py*iWidth*iMagnifying;
				rect.right = (iStartX+MapC[gCurPaintMapNumber].m_cItem[n].py*iWidth*iMagnifying) + (iWidth*iMagnifying);
				rect.top = iStartY+MapC[gCurPaintMapNumber].m_cItem[n].px*iHeight*iMagnifying;
				rect.bottom = (iStartY+MapC[gCurPaintMapNumber].m_cItem[n].px*iHeight*iMagnifying) + (iHeight*iMagnifying);

				FillRect(hdc, &rect, hItemBrush);

				CString szDesc;
				szDesc.Format("%s (%d,%d)", MapC[gCurPaintMapNumber].m_cItem[n].GetName(),
					MapC[gCurPaintMapNumber].m_cItem[n].px, MapC[gCurPaintMapNumber].m_cItem[n].py);
			}
		}

		DeleteObject((HGDIOBJ)hCharacterBrush);
		DeleteObject((HGDIOBJ)hMonsterBrush);
		DeleteObject((HGDIOBJ)hNpcBrush);
		DeleteObject((HGDIOBJ)hItemBrush);
		DeleteObject((HGDIOBJ)hCrywolfMovePath);
		SetBkMode(hdc, iOldBkMode);
	}

	gObjTotalUser = totalplayer;

#if (GS_OLDSTYLE == 0)
	if(statusBar.hStatus != NULL)
	{
		char tmp[150]={0};
		wsprintf(tmp,"ACTIVE MOBS: %d - LOADED MOBS: %d/%d",count,gMSetBase.m_Count,OBJ_MAXMONSTER);
		statusBar.ModText(0,tmp);
		wsprintf(tmp,"PLAYERS: %d/%d",totalplayer,gServerMaxUser);
		statusBar.ModText(1,tmp);
	}

#if (WL_PROTECT==1)
	TextOut(hdc, 0, 0, GlTmp, strlen(GlTmp));
	TextOut(hdc, 0, 15, GlTmp2, strlen(GlTmp2));
#endif

#else
	wsprintf(szTemp, "ACTIVE MOBS:%d - LOADED MOBS:%d/%d  TotalPlayer: %d  Player(%d):%d VpCount:%d(%d/%d) : max user:%d",
		count,gMSetBase.m_Count,OBJ_MAXMONSTER, totalplayer, aIndex, playerc, gObj[aIndex].VPCount, count3, count2, gServerMaxUser);
	if ( XMasEvent.gXMasEvent )
		strcat(szTemp, ":StarOfXMas");

	if ( gFireCrackerEvent )
		strcat(szTemp, ":FireCracker");

	if ( gHeartOfLoveEvent )
		strcat(szTemp, ":HeartOfLove");

	if ( gMedalEvent )
		strcat(szTemp, ":MedalEvent");

	char szTemp2[256];
	wsprintf(szTemp2," - [%s] %s %s",VersionName,Creators,EMails);
	strcat(szTemp, szTemp2);
	TextOut(hdc, 0, 0, szTemp, strlen(szTemp));
#if (WL_PROTECT==1)
	TextOut(hdc, 0, 100, GlTmp, strlen(GlTmp));
	TextOut(hdc, 0, 115, GlTmp2, strlen(GlTmp2));
#endif

#endif
	ReleaseDC(hWnd, hdc);
}
		



static int FrustrumX[MAX_ARRAY_FRUSTRUM];
static int FrustrumY[MAX_ARRAY_FRUSTRUM];

void InitFrustrum()
{
	float CameraViewFar = 2400.0;
	float CameraViewNear = CameraViewFar * 0.19f;
	float CameraViewTarget = CameraViewFar * 0.53f;
	float WidthFar = 1190.0;
	float WidthNear =  550.0;

	float p[4][3];

	p[0][0] = -WidthFar;
	p[0][1] = CameraViewFar-CameraViewTarget;
	p[0][2] = 0;
	p[1][0] = WidthFar;
	p[1][1] = CameraViewFar-CameraViewTarget;
	p[1][2] = 0;
	p[2][0] = WidthNear;
	p[2][1] = CameraViewNear - CameraViewTarget;
	p[2][2] = 0;
	p[3][0]= -WidthNear;
	p[3][1] = CameraViewNear - CameraViewTarget;
	p[3][2] = 0;

	float Angle[3];
	Angle[0] = 0;
	Angle[1] = 0;
	Angle[2] = 45.0f;

	float Matrix[3][4];
	float Frustrum[4][3];

	AngleMatrix(Angle,Matrix);

	for(int i = 0; i < MAX_ARRAY_FRUSTRUM;i++)
	{
		VectorRotate((const float *)&p[i],(const float *)&Matrix[0],(float *)&Frustrum[i]);
		FrustrumX[i] = Frustrum[i][0]*0.01f;
		FrustrumY[i] = Frustrum[i][1]*0.01f;

	}
}








void CreateFrustrum(int x, int y, int aIndex)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	for ( int i=0;i<MAX_ARRAY_FRUSTRUM;i++)
	{
		lpObj->FrustrumX[i] = FrustrumX[i] + x;
		lpObj->FrustrumY[i] = FrustrumY[i] + y;
	}
}







bool TestFrustrum2(int x, int y, int aIndex)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return false;
	}

	int j = 3;
	LPOBJ lpObj = &gObj[aIndex];

	for(int i = 0; i < MAX_ARRAY_FRUSTRUM;j = i,i++)
	{
		if((lpObj->FrustrumX[i]- x) * (lpObj->FrustrumY[j]-y) - (lpObj->FrustrumX[j]-x) * (lpObj->FrustrumY[i]-y) < 0.0f)
		{
			return false;
		}
	}
	return true;
}






BOOL gObjCheckViewport(int aIndex, int x, int y)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return false;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if(x < lpObj->X - 15  || x > lpObj->X + 15 || y < lpObj->Y - 15 || y > lpObj->Y + 15)
	{
		return false;
	}

	int j = 3;

	for(int i = 0; i < MAX_ARRAY_FRUSTRUM; j = i, i++)
	{
		int frustrum = (lpObj->FrustrumX[i]- x) * (lpObj->FrustrumY[j]-y) - (lpObj->FrustrumX[j]-x) * (lpObj->FrustrumY[i]-y);
		if(frustrum < 0)
		{
			return false;
		}
	}
	return true;
}






void gObjViewportClose(LPOBJ lpObj)
{
	int tObjNum,i;
	int MVL = MAX_VIEWPORT;

	if(lpObj->Type == OBJ_MONSTER)
	{
		MVL = MAX_VIEWPORT_MONSTER;
	}

	if(lpObj->Connected == PLAYER_PLAYING)
	{
		for(int n = 0; n < MVL; n++)
		{
			if(lpObj->VpPlayer2[n].state != 0)
			{
				if(lpObj->VpPlayer2[n].type != 5)
				{
					tObjNum = lpObj->VpPlayer2[n].number;

					i = 0;

					while( true )
					{
						if(gObj[tObjNum].VpPlayer[i].number == lpObj->m_Index)
						{
							if(gObj[tObjNum].VpPlayer[i].type == lpObj->Type)
							{
								gObj[tObjNum].VpPlayer[i].state = 3;
								break;
							}
						}

						if(i < MVL-1)
						{
							i++;
						}
						else
						{
							break;
						}
					}

					i = 0;

					while( true )
					{
						if(gObj[tObjNum].VpPlayer2[i].number == lpObj->m_Index)
						{
							if(gObj[tObjNum].VpPlayer2[i].type == lpObj->Type)
							{
								gObj[tObjNum].VpPlayer2[i].state = 0;
								break;
							}
						}

						if(i < MVL-1)
						{
							i++;
						}
						else
						{
							break;
						}
					}
				}
			}
		}
	}
	gObjClearViewport(lpObj);
}






void gObjViewportListCreate(short aIndex)
{
	int result,n;
	LPOBJ lpObj;
	int mapnum;

	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	lpObj = &gObj[aIndex];

	if(lpObj->Connected < PLAYER_PLAYING)
	{
		return;
	}

	if(lpObj->RegenOk > 0)
	{
		return;
	}

	mapnum = lpObj->MapNumber;
	//gItemLoop2 = 0;

	if(lpObj->Type == OBJ_USER)
	{
		MapClass * lpMap = &MapC[mapnum];

		for(n = 0; n < MAX_MAPITEM; n++)
		{
			if(lpMap->m_cItem[n].live)
			{
				//gItemLoop2++;

				if(lpMap->m_cItem[n].m_State == 1 || lpMap->m_cItem[n].m_State == 2)
				{
					if(gObjCheckViewport(aIndex,lpMap->m_cItem[n].px,lpMap->m_cItem[n].py) == 1)
					{
						result = ViewportAdd(aIndex,n,5);
					}
				}
			}
		}
	}

	if(aIndex == 0)
	{
		//gItemLoopMax = gItemLoop2;
		//gItemLoop = gItemLoop2;
		gCurPaintPlayer = aIndex;
	}

	LPOBJ lpTempObj;

	if(lpObj->Type == OBJ_USER)
	{
		for(n = 0; n < OBJMAX; n++)
		{
			lpTempObj = &gObj[n];

			if(lpTempObj->Connected == PLAYER_PLAYING && aIndex != n)
			{
				if(lpTempObj->m_State == 1 || lpTempObj->m_State == 2)
				{
					if(mapnum == lpTempObj->MapNumber)
					{
						if(gObjCheckViewport(aIndex,lpTempObj->X,lpTempObj->Y) == 1)
						{
							result = ViewportAdd(aIndex,n,lpTempObj->Type);
							result = ViewportAdd2(n,aIndex,gObj[aIndex].Type);
						}
					}
				}
			}
		}
	}
	else if(lpObj->Type == OBJ_MONSTER)
	{
		for(n = OBJ_STARTUSERINDEX; n < OBJMAX; n++)
		{
			lpTempObj = &gObj[n];

			if(lpTempObj->Connected == PLAYER_PLAYING && aIndex != n)
			{
				if(lpTempObj->m_State == 1 || lpTempObj->m_State == 2)
				{
					if(mapnum == lpTempObj->MapNumber)
					{
						if(gObjCheckViewport(aIndex,lpTempObj->X,lpTempObj->Y) == 1)
						{
							result = ViewportAdd(aIndex,n,gObj[n].Type);
							result = ViewportAdd2(n,aIndex,gObj[aIndex].Type);
						}
					}
				}
			}
		}
	}
}






int  ViewportAdd(int aIndex, int aAddIndex, int aType)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return -1;
	}

	int blank = -1;
	LPOBJ lpObj = &gObj[aIndex];
	int MVL = MAX_VIEWPORT;

	if(lpObj->Type == OBJ_MONSTER)
	{
		MVL = MAX_VIEWPORT_MONSTER;
	}

	for(int n = 0; n < MVL; n++)
	{
		if(lpObj->VpPlayer[n].state != 0)
		{
			if(lpObj->VpPlayer[n].number == aAddIndex)
			{
				if(lpObj->VpPlayer[n].type == aType)
				{
					return -1;
				}
			}
		}
		else
		{
			if(blank < 0)
			{
				blank = n;
			}
		}
	}

	if(blank < 0)
	{
		return -1;
	}


	lpObj->VpPlayer[blank].state = 1;
	lpObj->VpPlayer[blank].number = aAddIndex;
	lpObj->VpPlayer[blank].type = aType;
	lpObj->VPCount ++;

	if(lpObj->Type == OBJ_MONSTER || lpObj->Type == OBJ_NPC)
	{
		if(aType == 1)
		{
			if(lpObj->m_iCurrentAI != 0)
			{
				lpObj->m_Agro.SetAgro(aAddIndex,1000);
			}
		}
	}
	return blank;
}






int  ViewportAdd2(int aIndex, int aAddIndex, int aType)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return -1;
	}

	int blank = -1;

	LPOBJ lpObj = &gObj[aIndex];
	int MVL = MAX_VIEWPORT;

	if(lpObj->Type == OBJ_MONSTER)
	{
		MVL = MAX_VIEWPORT_MONSTER;
	}

	for(int n = 0; n < MVL; n++)
	{
		if(lpObj->VpPlayer2[n].state != 0)
		{
			if(lpObj->VpPlayer2[n].number == aAddIndex)
			{
				if(lpObj->VpPlayer2[n].type == aType)
				{
					return -1;
				}
			}
		}
		else
		{
			if(blank < 0)
			{
				blank = n;
			}
		}
	}

	if(blank < 0)
	{
		return -1;
	}


	lpObj->VpPlayer2[blank].state = 1;
	lpObj->VpPlayer2[blank].number = aAddIndex;
	lpObj->VpPlayer2[blank].type = aType;
	lpObj->VPCount2 ++;

	return blank;
}






void ViewportDel(short aIndex, int aDelIndex)
{
	if(gObjIsConnected(aIndex) == 0)
	{
		return;
	}

	if(gObjIsConnected(aDelIndex) == 0)
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];
	int MVL = MAX_VIEWPORT;

	if(lpObj->Type == OBJ_MONSTER)
	{
		MVL = MAX_VIEWPORT_MONSTER;
	}

	for(int n = 0; n < MVL; n++)
	{
		if(lpObj->VpPlayer[n].state == 1 || lpObj->VpPlayer[n].state == 2)
		{
			if(lpObj->VpPlayer[n].number == aDelIndex)
			{
				lpObj->VpPlayer[n].state = 0;
				lpObj->VpPlayer[n].number = -1;
				lpObj->VPCount --;
			}
		}
	}
}






void Viewport2Del(short aIndex, int aDelIndex)
{
	if(gObjIsConnected(aIndex) == 0)
	{
		return;
	}

	if(gObjIsConnected(aDelIndex) == 0)
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];
	int MVL = MAX_VIEWPORT;

	if(lpObj->Type == OBJ_MONSTER)
	{
		MVL = MAX_VIEWPORT_MONSTER;
	}

	for(int n = 0; n < MVL; n++)
	{
		if(lpObj->VpPlayer2[n].state == 1 || lpObj->VpPlayer2[n].state == 2)
		{
			if(lpObj->VpPlayer2[n].number == aDelIndex)
			{
				lpObj->VpPlayer2[n].state = 0;
				lpObj->VpPlayer2[n].number = -1;
				lpObj->VPCount2 --;
			}
		}
	}
}






void gObjViewportAllDel(short aIndex)
{
	if(gObjIsConnected(aIndex) == 0)
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];
	int MVL = MAX_VIEWPORT;

	if(lpObj->Type == OBJ_MONSTER)
	{
		MVL = MAX_VIEWPORT_MONSTER;
	}

	for(int n = 0; n < MVL; n++)
	{
		if(lpObj->VpPlayer[n].state == 1 || lpObj->VpPlayer[n].state == 2)
		{
			Viewport2Del(lpObj->VpPlayer[n].number,aIndex);
		}

		if(lpObj->VpPlayer2[n].state == 1 || lpObj->VpPlayer2[n].state == 2)
		{
			ViewportDel(lpObj->VpPlayer2[n].number,aIndex);
		}
	}
}






void gObjViewportListDestroy(short aIndex)
{
	short tObjNum;
	int n;
	LPOBJ lpObj;
	int MVL;

	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	if(gObj[aIndex].Connected < PLAYER_PLAYING)
	{
		return;
	}

	lpObj = &gObj[aIndex];
	MVL = MAX_VIEWPORT;

	if(lpObj->Type == OBJ_MONSTER)
	{
		MVL = MAX_VIEWPORT_MONSTER;
	}

	for(n = 0; n < MVL; n++)
	{
		int mapnum;
		if(lpObj->VpPlayer[n].state == 1 || lpObj->VpPlayer[n].state == 2)
		{
			tObjNum = lpObj->VpPlayer[n].number;

			switch(lpObj->VpPlayer[n].type)
			{
			case 5:
				mapnum = lpObj->MapNumber;

				if(MapC[mapnum].m_cItem[tObjNum].IsItem() == 1)
				{
					if(MapC[mapnum].m_cItem[tObjNum].m_State == 8)
					{
						lpObj->VpPlayer[n].state = 3;
					}
					else
					{
						if(gObjCheckViewport(aIndex,MapC[mapnum].m_cItem[tObjNum].px,MapC[mapnum].m_cItem[tObjNum].py) == 0)
						{
							lpObj->VpPlayer[n].state = 3;
						}
					}
				}
				else
				{
					if(MapC[mapnum].m_cItem[tObjNum].live == 0 || MapC[mapnum].m_cItem[tObjNum].Give == 0)
					{
						lpObj->VpPlayer[n].state = 3;
					}
					else
					{
						lpObj->VpPlayer[n].state = 3;
					}
				}
				break;
			default : 
				if(gObj[tObjNum].m_State == 1)
				{
					if(gObj[tObjNum].Live == 0)
					{
						lpObj->VpPlayer[n].state = 3;
					}
				}

				if(gObj[tObjNum].Connected  == PLAYER_EMPTY ||
					gObj[tObjNum].m_State == 8 ||
					gObj[tObjNum].Teleport != 0 ||
					gObj[tObjNum].m_State == 32)
				{
					lpObj->VpPlayer[n].state = 3;
				}
				else
				{
					if(gObjCheckViewport(aIndex,gObj[tObjNum].X,gObj[tObjNum].Y) == 0)
					{
						lpObj->VpPlayer[n].state = 3;

						if(lpObj->Type == OBJ_MONSTER || lpObj->Type == OBJ_NPC)
						{
							if(gObj[tObjNum].Type == OBJ_USER)
							{
								if(lpObj->m_iCurrentAI != 0)
								{
									lpObj->m_Agro.DelAgro(tObjNum);
								}
							}
						}
					}
				}
				break;
			}
		}

		if(lpObj->VpPlayer2[n].state == 1 || lpObj->VpPlayer2[n].state == 2)
		{
			tObjNum = lpObj->VpPlayer2[n].number;

			if(gObj[tObjNum].Connected < PLAYER_PLAYING)
			{
				lpObj->VpPlayer2[n].state = 0;
				lpObj->VpPlayer2[n].number = -1;
				lpObj->VPCount2 --;
			}
			else
			{
				if(gObjCheckViewport(tObjNum,lpObj->X,lpObj->Y) == 0)
				{
					lpObj->VpPlayer2[n].state = 0;
					lpObj->VpPlayer2[n].number = -1;
					lpObj->VPCount2 --;
				}
			}
		}
	}
}

struct PMSG_ANS_PSHOP_VIEWPORT_NOTIFY {
	struct PWMSG_HEAD2 h;
	unsigned char btCount;
};

struct PMSG_PSHOP_VIEWPORT_NOTIFY {
  unsigned char btNumberH;
  unsigned char btNumberL;
  char szPShopText[36];
};

void PShop_ViewportListRegenarate(short aIndex)
{
	if(gObjIsConnected(aIndex) == 0)
	{
		return ;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->Type != OBJ_USER)
	{
		return;
	}

	if(lpObj->RegenOk > 0)
	{
		return;
	}

	if(lpObj->CloseCount > -1)
	{
		return;
	}

	BYTE cBUFFER[6000];
	int iVpOpenCount = 0;
	int iVpAddCount = 0;

	PMSG_ANS_PSHOP_VIEWPORT_NOTIFY * lpMsg = (PMSG_ANS_PSHOP_VIEWPORT_NOTIFY *)&cBUFFER;
	PMSG_PSHOP_VIEWPORT_NOTIFY * lpMsgBody = (PMSG_PSHOP_VIEWPORT_NOTIFY *)&cBUFFER[6];

	int iPShopOpenIndex[MAX_VIEWPORT];

	if(lpObj->m_bPShopRedrawAbs != 0)
	{
		memset(lpObj->m_iVpPShopPlayer,0x00,sizeof(lpObj->m_iVpPShopPlayer));
		lpObj->m_wVpPShopPlayerCount = 0;
		lpObj->m_bPShopRedrawAbs = 0;
	}

	for(int n = 0; n < MAX_VIEWPORT; n++)
	{
		if(lpObj->VpPlayer[n].state == 2 && 
			(lpObj->VpPlayer[n].type == OBJ_USER 
#if (PACK_EDITION>=3)
			|| gObj[lpObj->VpPlayer[n].number].IsBot == 4
#endif
			) && 
			gObj[lpObj->VpPlayer[n].number].m_bPShopOpen == 1)
		{
			iPShopOpenIndex[iVpOpenCount] = lpObj->VpPlayer[n].number;
			iVpOpenCount++;

			if(lpObj->m_wVpPShopPlayerCount > 0 && lpObj->m_wVpPShopPlayerCount <= MAX_VIEWPORT)
			{
				int found = 0;

				for(int i = 0; i < lpObj->m_wVpPShopPlayerCount; i++)
				{
					if(lpObj->m_iVpPShopPlayer[i] == lpObj->VpPlayer[n].number)
					{
						found = 1;
						break;
					}
				}

				if(found == 0)
				{
					lpMsgBody[iVpAddCount].btNumberL = SET_NUMBERL(WORD(lpObj->VpPlayer[n].number));
					lpMsgBody[iVpAddCount].btNumberH = SET_NUMBERH(WORD(lpObj->VpPlayer[n].number));
					memcpy(lpMsgBody[iVpAddCount].szPShopText,gObj[lpObj->VpPlayer[n].number].m_szPShopText,sizeof(gObj[lpObj->VpPlayer[n].number].m_szPShopText));
					iVpAddCount++;
				}
			}
			else if(lpObj->m_wVpPShopPlayerCount == 0)
			{
				lpMsgBody[iVpAddCount].btNumberL = SET_NUMBERL(WORD(lpObj->VpPlayer[n].number));
				lpMsgBody[iVpAddCount].btNumberH = SET_NUMBERH(WORD(lpObj->VpPlayer[n].number));
				memcpy(lpMsgBody[iVpAddCount].szPShopText,gObj[lpObj->VpPlayer[n].number].m_szPShopText,sizeof(gObj[lpObj->VpPlayer[n].number].m_szPShopText));
				iVpAddCount++;
			}
			else
			{
				LogAddTD("[PShop] ERROR : lpObj->m_wVpPShopPlayerCount is OUT of BOUND : %d",lpObj->m_wVpPShopPlayerCount);
			}
		}
	}

	memset(lpObj->m_iVpPShopPlayer,0x00,sizeof(lpObj->m_iVpPShopPlayer));
	lpObj->m_wVpPShopPlayerCount = 0;

	if(iVpOpenCount > 0 && iVpOpenCount <= MAX_VIEWPORT)
	{
		for( int n = 0; n < iVpOpenCount; n++)
		{
			lpObj->m_iVpPShopPlayer[n] = iPShopOpenIndex[n];
		}
		lpObj->m_wVpPShopPlayerCount = iVpOpenCount;
	}
	else if(iVpOpenCount != 0)
	{
		LogAddTD("[PShop] ERROR : iVpOpenCount is OUT of BOUND: %d",iVpOpenCount);
	}

	if(iVpAddCount > 0 && iVpAddCount <= MAX_VIEWPORT)
	{
		lpMsg->btCount = iVpAddCount;
		PHeadSubSetW((unsigned char *)lpMsg,0x3F,0,sizeof(PMSG_ANS_PSHOP_VIEWPORT_NOTIFY)+sizeof(PMSG_PSHOP_VIEWPORT_NOTIFY)*iVpAddCount);
		DataSend(aIndex,(unsigned char *)lpMsg,((lpMsg->h.sizeL & 0xFF)& 0xFF | ((lpMsg->h.sizeH & 0xFF) & 0xFF) << 8) & 0xFFFF);
	}
	else if(iVpAddCount != 0)
	{
		LogAddTD("[PShop] ERROR : iVpAddCount is OUT of BOUND: %d",iVpAddCount);
	}

	if(gObj[aIndex].m_bPShopWantDeal == 1)
	{
		if(gObjIsConnected(gObj[aIndex].m_iPShopDealerIndex) != 0)
		{
			if (gObj[gObj[aIndex].m_iPShopDealerIndex].Type == OBJ_USER
#if (PACK_EDITION>=3)
				|| gObj[gObj[aIndex].m_iPShopDealerIndex].IsBot == 4			//Personal Shop Bot
#endif
				)
			{
				if(gObj[gObj[aIndex].m_iPShopDealerIndex].m_bPShopOpen == 0)
				{
					gObj[aIndex].m_bPShopWantDeal = 0;
					gObj[aIndex].m_iPShopDealerIndex = -1;
					memset(gObj[aIndex].m_szPShopDealerName,0,sizeof(gObj[aIndex].m_szPShopDealerName));

					CGPShopAnsDealerClosedShop(aIndex,gObj[aIndex].m_iPShopDealerIndex);
					//GCMoneySend(aIndex, gObj[aIndex].Money);
				}
				else if(gObj[gObj[aIndex].m_iPShopDealerIndex].CloseCount >= 0 || gObj[aIndex].CloseCount >= 0)
				{
					gObj[aIndex].m_bPShopWantDeal = 0;
					gObj[aIndex].m_iPShopDealerIndex = -1;
					memset(gObj[aIndex].m_szPShopDealerName,0,sizeof(gObj[aIndex].m_szPShopDealerName));

					CGPShopAnsDealerClosedShop(aIndex,gObj[aIndex].m_iPShopDealerIndex);
					//GCMoneySend(aIndex, gObj[aIndex].Money);
				}
				else if(gObj[gObj[aIndex].m_iPShopDealerIndex].m_bMapAntiHackMove == true || gObj[aIndex].m_bMapAntiHackMove == true )
				{
					gObj[aIndex].m_bPShopWantDeal = 0;
					gObj[aIndex].m_iPShopDealerIndex = -1;
					memset(gObj[aIndex].m_szPShopDealerName,0,sizeof(gObj[aIndex].m_szPShopDealerName));

					CGPShopAnsDealerClosedShop(aIndex,gObj[aIndex].m_iPShopDealerIndex);
					//GCMoneySend(aIndex, gObj[aIndex].Money);
				}
				else if(gObj[gObj[aIndex].m_iPShopDealerIndex].m_bPShopItemChange == 1)
				{
					CGPShopAnsBuyList(aIndex,gObj[aIndex].m_iPShopDealerIndex,1,1);
				}
			}
			else
			{
				gObj[aIndex].m_bPShopWantDeal = 0;
				gObj[aIndex].m_iPShopDealerIndex = -1;
				memset(gObj[aIndex].m_szPShopDealerName,0,sizeof(gObj[aIndex].m_szPShopDealerName));

				CGPShopAnsDealerClosedShop(aIndex,gObj[aIndex].m_iPShopDealerIndex);
				//GCMoneySend(aIndex, gObj[aIndex].Money);
			}
		}
		else
		{
			gObj[aIndex].m_bPShopWantDeal = 0;
			gObj[aIndex].m_iPShopDealerIndex = -1;
			memset(gObj[aIndex].m_szPShopDealerName,0,sizeof(gObj[aIndex].m_szPShopDealerName));

			CGPShopAnsDealerClosedShop(aIndex,gObj[aIndex].m_iPShopDealerIndex);
			//GCMoneySend(aIndex, gObj[aIndex].Money);
		}
	}
}

bool PShop_CheckInventoryEmpty(short aIndex)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return false;
	}

	LPOBJ lpObj = &gObj[aIndex];

	for(int i = ReadConfig.MAIN_INVENTORY_SIZE(lpObj->m_Index,false); i < ReadConfig.INVENTORY_SIZE(lpObj->m_Index,false); i++)
	{
		if(lpObj->Inventory1[i].IsItem() == 1)
		{
			return false;
		}
	}
	return true;
}






void gObjStateSetCreate(int aIndex)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->Connected < PLAYER_PLAYING)
	{
		return;
	}

	DWORD dwNowTick = GetTickCount();
	
	if(lpObj->DieRegen == 1)
	{
		if(dwNowTick - lpObj->RegenTime > lpObj->MaxRegenTime + 1000)
		{
			lpObj->DieRegen = 2;
			lpObj->m_State = 8;
		}
	}

	if(lpObj->Type == OBJ_USER)
	{
		if(lpObj->m_State == 8)
		{
			gObjCalCharacter(lpObj->m_Index);		//Ready for Relife
		}
		else
		{
			gObjCheckTimeOutValue(lpObj,(DWORD &)dwNowTick);
		}
	}

	if(lpObj->Teleport == 1)
	{
		if(lpObj->DieRegen != 0)
		{
			lpObj->Teleport = 0;
		}
		else if(GetTickCount() - lpObj->TeleportTime > 100)
		{
			lpObj->Teleport = 2;
		}
	}
	else if(lpObj->Teleport == 2)
	{
		lpObj->Teleport = 3;

		if(lpObj->Type == OBJ_USER)
		{
			lpObj->TX = lpObj->X;
			lpObj->TY = lpObj->Y;

			CreateFrustrum(lpObj->X,lpObj->Y,aIndex);
			GCTeleportSend(&gObj[aIndex],0,lpObj->MapNumber,lpObj->X,lpObj->Y,lpObj->Dir);
			gObjViewportListProtocolCreate(lpObj);
			lpObj->Teleport = 0;
		}
		else if(lpObj->Type == OBJ_NPC && lpObj->Class == 229)	//Marlon Quest
		{
			int x = lpObj->X;
			int y = lpObj->Y;
			
			lpObj->X = x;
			lpObj->Y = y;

			lpObj->TX = lpObj->X;
			lpObj->TY = lpObj->Y;

			CreateFrustrum(lpObj->X,lpObj->Y,aIndex);

			lpObj->m_State = 1;
			lpObj->PathStartEnd = 0;
		}
		else if(lpObj->Type == OBJ_NPC && lpObj->Class == 568)	//Wandering Merchant Quest
		{
			int x = lpObj->X;
			int y = lpObj->Y;
			
			lpObj->X = x;
			lpObj->Y = y;

			lpObj->TX = lpObj->X;
			lpObj->TY = lpObj->Y;

			CreateFrustrum(lpObj->X,lpObj->Y,aIndex);

			lpObj->m_State = 1;
			lpObj->PathStartEnd = 0;
		}
		else if(lpObj->Type == OBJ_MONSTER)
		{
			lpObj->TX = lpObj->X;
			lpObj->TY = lpObj->Y;

			CreateFrustrum(lpObj->X,lpObj->Y,aIndex);

			gObjViewportListCreate(lpObj->m_Index);
			gObjViewportListProtocolCreate(lpObj);

			lpObj->Teleport = 0;
			lpObj->PathStartEnd = 0;
			lpObj->PathCur = 0;
			lpObj->PathCount = 0;
		}
		else 
		{
			int x,y;

			if(lpObj->m_RecallMon >= 0)
			{
				x = lpObj->X;
				y = lpObj->Y;
			}
			else
			{
				x = lpObj->X + rand()%6 - 3;
				y = lpObj->Y + rand()%6 - 3;
			}

			lpObj->X = x;
			lpObj->Y = y;

			lpObj->TX = lpObj->X;
			lpObj->TY = lpObj->Y;

			if(lpObj->m_RecallMon >= 0)
			{
				gObjViewportListProtocolCreate(lpObj);
				lpObj->PathStartEnd = 0;
			}
		}
	}

	if(lpObj->RegenOk == 2 && lpObj->Type == OBJ_USER)
	{
		lpObj->m_State = 1;
		lpObj->RegenOk = 3;
		lpObj->X = lpObj->RegenMapX;
		lpObj->Y = lpObj->RegenMapY;
		lpObj->MapNumber = lpObj->RegenMapNumber;

		lpObj->TX = lpObj->X;
		lpObj->TY = lpObj->Y;

		CreateFrustrum(lpObj->X,lpObj->Y,aIndex);

		#if (GS_CASTLE==1)
		//Castle Siege State Send Fix [4]
		if ( lpObj->MapNumber == MAP_INDEX_CASTLESIEGE )
		{
			if ( g_CastleSiege.GetCastleState() == 7 )
			{
				GCAnsCsNotifyStart(lpObj->m_Index, 1);
				g_CastleSiege.SetUserCsJoinSide(lpObj->m_Index);
				g_CastleSiege.NotifySelfCsJoinSide(lpObj->m_Index);
				g_CastleSiege.NotifyCsSelfLeftTime(lpObj->m_Index);
			} else {
				GCAnsCsNotifyStart(lpObj->m_Index, 0);
			}
		}
		#endif

		if ( lpObj->MapNumber == MAP_INDEX_CRYWOLF_FIRSTZONE )
		{
			g_CrywolfSync.NotifyCrywolfCurrentStateByUserId(lpObj->m_Index);
		}

		if(DragonEvent->GetState() > 0)
		{
			if(DragonEvent->GetMapNumber() == lpObj->MapNumber)
			{
				GCMapEventStateSend(lpObj->MapNumber,1,1);
			}
		}

		gEledoradoEvent.CheckGoldDercon(lpObj->MapNumber);

		if(gTamaJJangEvent != 0 && gAppearTamaJJang != 0)
		{
			if(lpObj->MapNumber == MAP_INDEX_LORENCIA)
			{
				GCMapEventStateSend(0,gAppearTamaJJang,2);
			}
		}
	}
}

struct PMSG_CHARREGEN {

  // static data ------------------------------------

  // non-static data --------------------------------
  /*<thisrel this+0x0>*/ /*|0x3|*/ struct PBMSG_HEAD h;
  /*<thisrel this+0x3>*/ /*|0x1|*/ unsigned char subcode;
  /*<thisrel this+0x4>*/ /*|0x1|*/ unsigned char MapX;
  /*<thisrel this+0x5>*/ /*|0x1|*/ unsigned char MapY;
  /*<thisrel this+0x6>*/ /*|0x1|*/ unsigned char MapNumber;
  /*<thisrel this+0x7>*/ /*|0x1|*/ unsigned char Dir;
  /*<thisrel this+0x8>*/ /*|0x2|*/ unsigned short Life;
  /*<thisrel this+0xa>*/ /*|0x2|*/ unsigned short Mana;
  /*<thisrel this+0xc>*/ /*|0x2|*/ unsigned short wShield;
  /*<thisrel this+0xe>*/ /*|0x2|*/ unsigned short BP;
  /*<thisrel this+0x10>*/ /*|0x4|*/ //unsigned long Exp;
  DWORD unk1;
  /*<thisrel this+0x10>*/ /*|0x4|*/ BYTE Exp[4];
  /*<thisrel this+0x14>*/ /*|0x4|*/ unsigned long Money[4];

  // base classes -----------------------------------

  // friends ----------------------------------------

  // static functions -------------------------------

  // non-virtual functions --------------------------

  // virtual functions ------------------------------
};

void gObjSetState()
{
	int n;
	LPOBJ lpObj;

	for(n = 0; n < OBJMAX; n++)
	{
		lpObj = &gObj[n];

		if(lpObj->Connected > PLAYER_LOGGED)
		{
			if(lpObj->m_State == 1)
			{
				lpObj->m_State = 2;
				if(lpObj->RegenOk == 3)
				{
					lpObj->RegenOk = 0;
				}
			}
	
			if(lpObj->Teleport == 3)
			{
				lpObj->Teleport = 0;
			}
	
			if(lpObj->DieRegen == 2)
			{
				int mapnum = lpObj->MapNumber;
				int x = lpObj->X;
				int y = lpObj->Y;
	
				lpObj->m_bAttackerKilled = 0;
	
				if(DS_MAP_RANGE(lpObj->MapNumber))
				{
					if(lpObj->Type >= OBJ_MONSTER)
					{
						g_DevilSquare.gDevilSquareMonsterRegen(lpObj);
						continue;
					}
				}
	
				if(lpObj->MapNumber == MAP_INDEX_KANTURU_BOSS && lpObj->Type == OBJ_MONSTER)
				{
					continue;
				}
	
				if(lpObj->m_iCurrentAI != 0 && lpObj->m_iRegenType != 0)
				{
					continue;
				}
	
				if(lpObj->m_bIsInMonsterHerd != false)
				{
					if(lpObj->m_lpMonsterHerd != 0)
					{
						lpObj->m_lpMonsterHerd->MonsterRegenAction(lpObj);
					}
					continue;
				}
	
				if(BC_MAP_RANGE(lpObj->MapNumber) && lpObj->Type >= OBJ_MONSTER)
				{
					int BCRest = MAP_INDEX_BLOODCASTLE1;
					if(lpObj->MapNumber == MAP_INDEX_BLOODCASTLE8)
						BCRest = 45;
					if(lpObj->Class == 131)
					{
						if(g_BloodCastle.OpenDoorCheck[lpObj->MapNumber - BCRest] == false)
						{
							g_BloodCastle.OpenDoorCheck[lpObj->MapNumber - BCRest] = true;
							g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_bCASTLE_DOOR_LIVE = 0;
							g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_bBC_DOOR_TERMINATE_COMPLETE = 1;
							g_BloodCastle.m_BridgeData[lpObj->MapNumber - BCRest].m_iBC_BOSS_MONSTER_MAX_COUNT = g_BloodCastle.GetCurrentLiveUserCount(lpObj->MapNumber - BCRest) * 2;
		
							g_BloodCastle.m_BridgeData[lpObj->MapNumber - BCRest].m_iBC_BOSS_MONSTER_KILL_COUNT = 0;
		
							if(g_BloodCastle.m_BridgeData[lpObj->MapNumber - BCRest].m_iBC_BOSS_MONSTER_MAX_COUNT > 10)
							{
								g_BloodCastle.m_BridgeData[lpObj->MapNumber - BCRest].m_iBC_BOSS_MONSTER_MAX_COUNT = 10;
							}
		
							g_BloodCastle.SendCastleDoorBlockInfo(lpObj->m_cBloodCastleIndex,0);
							g_BloodCastle.ReleaseCastleDoor(lpObj->m_cBloodCastleIndex);
		
							if(g_BloodCastle.GetCurrentState(lpObj->m_cBloodCastleIndex) == 2)
							{
								g_BloodCastle.SetBossMonster(lpObj->m_cBloodCastleIndex);
							}
						}
					}
	
					if(BC_STATUE_RANGE(lpObj->Class - 132) && BC_MAP_RANGE(lpObj->MapNumber))
					{
						int TopHitUser = gObjMonsterTopHitDamageUser(lpObj);
						int level = lpObj->Class - 132;
	
						int type = ItemGetNumberMake(13,19);
	
					
						if(lpObj->MapNumber != 52)
							ItemSerialCreateSend(lpObj->m_Index,lpObj->MapNumber + 236,lpObj->X,lpObj->Y,type,level,0,0,0,0,TopHitUser,0,0);
						else
							ItemSerialCreateSend(lpObj->m_Index,52,lpObj->X,lpObj->Y,type,level,0,0,0,0,TopHitUser,0,0);

						g_BloodCastle.m_BridgeData[lpObj->MapNumber - BCRest].m_btBC_QUEST_ITEM_NUMBER = level;
	
						LogAddTD("[Blood Castle] (%d) Ultimate Weapon Dropped -> (%s)",
							lpObj->MapNumber - BCRest+1, (level !=0)?((level==1)?"Sword":"Crossbow"):"Staff");
					}

					if(lpObj->m_PosNum == -1)
					{
						gObjDel(lpObj->m_Index);
						continue;
					}

					if(lpObj->Type == OBJ_MONSTER)
					{
						if(lpObj->Class != 131 && BC_STATUE_RANGE(lpObj->Class - 132)== 0)
						{
							g_BloodCastle.ChangeMonsterState(lpObj->MapNumber - BCRest,lpObj->m_Index);
						}
					}
				}

				if(lpObj->m_Attribute == 60)	//Delete Monster from the regeneration!
				{
					gObjDel(lpObj->m_Index);
					continue;
				}

				int State = 1;
				if(lpObj->Type >= OBJ_MONSTER)
				{
					#if (GS_CASTLE==1)
					//Life Stone
					if(lpObj->Class == 278)
					{
						g_CsNPC_LifeStone.DeleteLifeStone(lpObj->m_Index);
						gObjDel(lpObj->m_Index);
						continue;
					}
					//User Guardian
					if(lpObj->Class == 285 && lpObj->m_Attribute == 60)
					{
						g_CsNPC_Guardian.DeleteGuardian(lpObj->m_Index);
						gObjDel(lpObj->m_Index);
						continue;
					}
					//Spearman and Archer
					if(lpObj->Class == 287 || lpObj->Class == 286)
					{
						g_CsNPC_Mercenary.DeleteMercenary(lpObj->m_Index);
						gObjDel(lpObj->m_Index);
						continue;
					}
					#endif

					if(lpObj->Class == 44 && DragonEvent->GetState() == 0)
					{
						State = 0;
					}

					if(lpObj->Class == 53 || lpObj->Class == 54	|| lpObj->Class == 55 || lpObj->Class == 56)
					{
						State = 0;
						lpObj->Life = lpObj->MaxLife;
						lpObj->DieRegen = 0;
					}
	
					if(lpObj->Class >= 78 && lpObj->Class <= 83)
					{
						State = 0;
						lpObj->Life = lpObj->MaxLife;
						lpObj->DieRegen = 0;
					}
#if (PACK_EDITION>=3)	
					if(lpObj->Class >= 493 && lpObj->Class <= 502)
					{
						State = 0;
						lpObj->Life = lpObj->MaxLife;
						lpObj->DieRegen = 0;
					}
#endif	
					if(lpObj->Class == 77)
					{
						int i_shield = lpObj->m_SkyBossMonSheildLinkIndex;
	
						if(i_shield != -1)
						{	
							gObj[i_shield].Life = gObj[i_shield].MaxLife;
						}	
					}
				}

				PMSG_CHARREGEN pMsg;

				if(State != 0)
				{
					int skinId = -1;

					lpObj->Life = lpObj->MaxLife + lpObj->AddLife;
					lpObj->Mana = lpObj->MaxMana + lpObj->AddMana;
					lpObj->iShield = lpObj->iMaxShield + lpObj->iAddShield;

					lpObj->Live = 1;
					lpObj->m_PoisonBeattackCount = 0;
					lpObj->m_ColdBeattackCount = 0;
					//lpObj->m_ViewState = 0;
					lpObj->Teleport = 0;

					for(int i = 0; i < MAX_SELF_DEFENSE; i++)
					{
						lpObj->SelfDefenseTime[i] = 0;
					}

					gObjTimeCheckSelfDefense(lpObj);

					lpObj->m_ViewSkillState &= 0xFFFFFFFE;
					lpObj->m_ViewSkillState &= 0xFFFFFFFD;

					gObjClearViewport(lpObj);

					if(lpObj->Type == OBJ_USER)
					{
						if(g_iUseCharacterAutoRecuperationSystem != 0 && lpObj->Level <= g_iCharacterRecuperationMaxLevel)
						{
							lpObj->m_iAutoRecuperationTime = GetTickCount();
						}
						else
						{
							lpObj->m_iAutoRecuperationTime = 0;
						}

						lpObj->dwShieldAutoRefillTimer = GetTickCount();

						if(lpObj->m_iSkillNPCHelpTime > 0)
						{
							lpObj->m_iSkillNPCHelpTime = 0;
							lpObj->m_iSkillNPCDefense = 0;
							lpObj->m_iSkillNPCAttack = 0;

							lpObj->m_ViewSkillState &= 0xFDFFFFFF;

							GCSkillInfoSend(lpObj,0,0x03);
						}

						gDarkSpirit[lpObj->m_Index].SetMode(CDarkSpirit::ePetItem_Mode::PetItem_Mode_Normal,-1);

						if(lpObj->KillerType == 3)
						{
							MapC[lpObj->MapNumber].GetRandomLengthPos((short &)lpObj->X,(short &)lpObj->Y,18);
//#pragma message(" [TO FIX] !!! : If character is in duel teleport on regen")
//							GCMoneySend(lpObj->m_Index, lpObj->Money);		//Fix Zen when Die
//							gObjTeleport(lpObj->m_Index,lpObj->MapNumber,lpObj->X,lpObj->Y);
//							continue;
						}
						else if(lpObj->KillerType == 2)
						{
							int w_Type = 0;

							if(lpObj->lpGuild != 0)
							{
								if(lpObj->lpGuild->WarType == 1)
								{
									w_Type = 1;
								}
							}

							if(w_Type == 0)
							{
								MapC[lpObj->MapNumber].GetRandomLengthPos((short &)lpObj->X,(short &)lpObj->Y,18);
							}
							else if(lpObj->lpGuild->WarState)
							{
								int BattleGroundIndex = lpObj->lpGuild->BattleGroundIndex;
								gBattleGetTeamPosition(BattleGroundIndex,lpObj->lpGuild->BattleTeamCode,(short &)lpObj->X,(short &)lpObj->Y);
							}
						}
						else if(lpObj->MapNumber == MAP_INDEX_LORENCIA || lpObj->MapNumber == MAP_INDEX_DUNGEON)
						{
							lpObj->MapNumber = MAP_INDEX_LORENCIA;
							MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber,(short &)lpObj->X,(short &)lpObj->Y);
						}
						else if(IT_MAP_RANGE(lpObj->MapNumber))
						{
							//skinId = 
							IllusionTemple.PlayerDieRespawn(lpObj->m_Index,(short &)lpObj->X,(short &)lpObj->Y);
						}
						else if(DS_MAP_RANGE(lpObj->MapNumber))
						{
							g_DevilSquare.DieProcDevilSquare(lpObj);
							lpObj->MapNumber = MAP_INDEX_NORIA;
							MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber,(short &)lpObj->X,(short &)lpObj->Y);
						}
#if (PACK_EDITION>=3)
						else if(DG_MAP_RANGE(lpObj->MapNumber))
						{
							lpObj->MapNumber = MAP_INDEX_ELBELAND;
							MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber,(short &)lpObj->X,(short &)lpObj->Y);
						}
						else if(lpObj->MapNumber == MAP_INDEX_SWAMP)
						{
							//lpObj->MapNumber = MAP_INDEX_SWAMP;
							MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber,(short &)lpObj->X,(short &)lpObj->Y);
						}
#endif						
#if (PACK_EDITION>=2)
						else if(IMPERIALGUARDIAN_MAP_RANGE(lpObj->MapNumber))
						{
							lpObj->MapNumber = MAP_INDEX_DEVIAS;
							MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber,(short &)lpObj->X,(short &)lpObj->Y);
						}
#endif
						else if(lpObj->MapNumber == MAP_INDEX_ICARUS)
						{
							lpObj->MapNumber = MAP_INDEX_DEVIAS;
							MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber,(short &)lpObj->X,(short &)lpObj->Y);
						}
						else if(BC_MAP_RANGE(lpObj->MapNumber))
						{
							lpObj->MapNumber = MAP_INDEX_DEVIAS;
							MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber,(short &)lpObj->X,(short &)lpObj->Y);
						}
						else if(CC_MAP_RANGE(lpObj->MapNumber))
						{
							lpObj->MapNumber = MAP_INDEX_DEVIAS;
							MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber,(short &)lpObj->X,(short &)lpObj->Y);
						}
						else if(KALIMA_MAP_RANGE(lpObj->MapNumber))
						{
							lpObj->MapNumber = MAP_INDEX_DEVIAS;
							MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber,(short &)lpObj->X,(short &)lpObj->Y);
						}
						else if(lpObj->MapNumber == MAP_INDEX_AIDA)
						{
							MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber,(short &)lpObj->X,(short &)lpObj->Y);
						}
						else if(lpObj->MapNumber == MAP_INDEX_CRYWOLF_FIRSTZONE)
						{
							#if (GS_CASTLE==1)
							//if ( g_Crywolf.GetCrywolfState() == CRYWOLF_STATE_START )
							if ( g_Crywolf.GetOccupationState() == CRYWOLF_OCCUPATION_SECURE ||
								(g_Crywolf.GetCrywolfState() >= CRYWOLF_STATE_NOTIFY_1 && 
								 g_Crywolf.GetCrywolfState() < CRYWOLF_STATE_START))
							{
								MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber,(short &)lpObj->X,(short &)lpObj->Y);
							} else {
								int mgt = 118;
								short x,y,level;
								BYTE map = lpObj->MapNumber,dir;
								int result = gGateC.GetGate(mgt,(short &)x,(short &)y,(BYTE &)map,(BYTE &)dir,(short &)level);
								if(result >= 0)
								{
									lpObj->MapNumber = map;
									lpObj->X = x;
									lpObj->Y = y;
								}
							}
							#else
							if ( g_CrywolfSync.GetOccupationState() == CRYWOLF_OCCUPATION_SECURE ||
								(g_CrywolfSync.GetCrywolfState() >= CRYWOLF_STATE_NOTIFY_1 && 
								 g_CrywolfSync.GetCrywolfState() < CRYWOLF_STATE_START))
							{
								MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber,(short &)lpObj->X,(short &)lpObj->Y);
							} else {
								int mgt = 118;
								short x,y,level;
								BYTE map = lpObj->MapNumber,dir;
								int result = gGateC.GetGate(mgt,(short &)x,(short &)y,(BYTE &)map,(BYTE &)dir,(short &)level);
								if(result >= 0)
								{
									lpObj->MapNumber = map;
									lpObj->X = x;
									lpObj->Y = y;
								}
							}
							#endif
						}
						#if (GS_CASTLE==0)
						else if(lpObj->MapNumber == MAP_INDEX_KANTURU_BOSS)
						{
							int mgt = 137;
							short x,y,level;
							BYTE map = lpObj->MapNumber,dir;
							int result = gGateC.GetGate(mgt,(short &)x,(short &)y,(BYTE &)map,(BYTE &)dir,(short &)level);
							if(result >= 0)
							{
								lpObj->MapNumber = map;
								lpObj->X = x;
								lpObj->Y = y;
							}

							g_KanturuBattleUserMng.DeleteUserData(lpObj->m_Index);
						}
						#endif
						else if(lpObj->MapNumber == MAP_INDEX_KANTURU1)
						{
							int mgt = 138;
							short x,y,level;
							BYTE map = lpObj->MapNumber,dir;

							int result = gGateC.GetGate(mgt,(short &)x,(short &)y,(BYTE &)map,(BYTE &)dir,(short &)level);
							if(result >= 0)
							{
								lpObj->MapNumber = map;
								lpObj->X = x;
								lpObj->Y = y;
							}
						}
						else if(lpObj->MapNumber == MAP_INDEX_KANTURU2)
						{
							int mgt = 139;
							short x,y,level;
							BYTE map = lpObj->MapNumber,dir;

							int result = gGateC.GetGate(mgt,(short &)x,(short &)y,(BYTE &)map,(BYTE &)dir,(short &)level);
							if(result >= 0)
							{
								lpObj->MapNumber = map;
								lpObj->X = x;
								lpObj->Y = y;
							}
						}
						else if(lpObj->MapNumber == MAP_INDEX_REFUGE)
						{
							lpObj->MapNumber = MAP_INDEX_BARRACKS;
							MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber,(short &)lpObj->X,(short &)lpObj->Y);
						}
						else if(lpObj->MapNumber == MAP_INDEX_RAKLIONBOSS)
						{
							lpObj->MapNumber = MAP_INDEX_RAKLION;
							MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber,(short &)lpObj->X,(short &)lpObj->Y);
						}
						else if(lpObj->MapNumber == MAP_INDEX_DUELMAP)
						{
							lpObj->MapNumber = MAP_INDEX_VULCANUS;
							MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber,(short &)lpObj->X,(short &)lpObj->Y);
						}
						else if (lpObj->MapNumber == MAP_INDEX_CASTLESIEGE)
						{
							//Check if Siege running and lifestone exists
							#if (GS_CASTLE==1)

							//Reset Crown Time if Killed
							lpObj->m_iAccumulatedCrownAccessTime = 0;

							if ( !g_CsNPC_LifeStone.SetReSpawnUserXY(lpObj->m_Index) )
							{
								int mgt = 100;
								short x,y,level;
								BYTE map = lpObj->MapNumber,dir;

								if (lpObj->m_btCsJoinSide == 1)
									mgt = 105;

								int result = gGateC.GetGate(mgt,(short &)x,(short &)y,(BYTE &)map,(BYTE &)dir,(short &)level);
								if(result >= 0)
								{
									lpObj->MapNumber = map;
									lpObj->X = x;
									lpObj->Y = y;
								}
							}
							#else
								MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber,(short &)lpObj->X,(short &)lpObj->Y);
							#endif
						}
						else if (lpObj->MapNumber == MAP_INDEX_CASTLEHUNTZONE)
						{
							int mgt = 106;
							short x,y,level;
							BYTE map = lpObj->MapNumber,dir;

							int result = gGateC.GetGate(mgt,(short &)x,(short &)y,(BYTE &)map,(BYTE &)dir,(short &)level);
							if(result >= 0)
							{
								lpObj->MapNumber = map;
								lpObj->X = x;
								lpObj->Y = y;
							}						
						}
						else
						{
							MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber,(short &)lpObj->X,(short &)lpObj->Y);
						}

						short m_Result = g_MapServerManager.CheckMoveMapSvr(lpObj->m_Index,lpObj->MapNumber,lpObj->m_sPrevMapSvrCode);

						if(m_Result != gGameServerCode )
						{
							if(m_Result == -1)
							{
								LogAddC(2,"[MapServerMng] Map Server Move Fail : CheckMoveMapSvr() == -1 [%s][%s] (%d)",lpObj->AccountID,lpObj->Name,lpObj->m_Index);
								continue;
							}

							if(lpObj->b_BlockMoveCommandSeconds > 0)
							{
								//Server Move Anti-Dupe Check
								char moveErr[1024]={0};
								wsprintf(moveErr,"You have to wait %d seconds.!", lpObj->b_BlockMoveCommandSeconds);
								GCServerMsgStringSend(moveErr, lpObj->m_Index, 1);
								continue;
							}

							int _map = lpObj->MapNumber;
							int _x = lpObj->X;
							int _y = lpObj->Y;

							lpObj->MapNumber = mapnum;
							lpObj->X = x;
							lpObj->Y = y;

							GJReqMapSvrMove(lpObj->m_Index,m_Result,_map,_x,_y);
							LogAddTD("[MapServerMng] Request to Move Map Server : (%d) - [%s][%s] (%d)",m_Result,lpObj->AccountID,lpObj->Name,lpObj->m_Index);
							continue;
						}

						lpObj->TX = lpObj->X;
						lpObj->TY = lpObj->Y;

						CreateFrustrum(lpObj->X,lpObj->Y,n);

						pMsg.h.c = 0xC3;
						pMsg.h.headcode = 0xF3;
						pMsg.h.size = 0x18;
						pMsg.subcode = 4;
						pMsg.Dir = lpObj->Dir;
						pMsg.MapX = lpObj->X;
						pMsg.MapY = lpObj->Y;
						pMsg.Life = lpObj->Life;
						pMsg.Mana = lpObj->Mana;
						WORD Tmp1 = HIWORD(lpObj->Experience);
						WORD Tmp2 = LOWORD(lpObj->Experience);
						pMsg.unk1 = 0;
						pMsg.Exp[0] = HIBYTE(Tmp1);
						pMsg.Exp[1] = LOBYTE(Tmp1);
						pMsg.Exp[2] = HIBYTE(Tmp2);
						pMsg.Exp[3] = LOBYTE(Tmp2);
						//pMsg.Exp = lpObj->Experience;
						pMsg.MapNumber = lpObj->MapNumber;
						pMsg.wShield = lpObj->iShield;
						pMsg.BP = lpObj->BP;

						WORD hiWord = SET_NUMBERHW(lpObj->Money);
						WORD loWord = SET_NUMBERLW(lpObj->Money);
						pMsg.Money[0] = SET_NUMBERH(hiWord);
						pMsg.Money[1] = SET_NUMBERL(hiWord);
						pMsg.Money[2] = SET_NUMBERH(loWord);
						pMsg.Money[3] = SET_NUMBERL(loWord);

						lpObj->PathCount = 0;

						//Fix ReAppear Show to Others
						lpObj->Teleport = 3;
						DataSend(n,(unsigned char *)&pMsg,pMsg.h.size);
						GCMoneySend(lpObj->m_Index, lpObj->Money);		//Fix Zen when Die

						gObjViewportListProtocolCreate(lpObj);

						#if (GS_CASTLE==1)
						//Castle Siege State Send Fix [7]
						if ( lpObj->MapNumber == MAP_INDEX_CASTLESIEGE )
						{
							if ( g_CastleSiege.GetCastleState() == 7 )
							{
								GCAnsCsNotifyStart(lpObj->m_Index, 1);
								g_CastleSiege.SetUserCsJoinSide(lpObj->m_Index);
								g_CastleSiege.NotifySelfCsJoinSide(lpObj->m_Index);
								g_CastleSiege.NotifyCsSelfLeftTime(lpObj->m_Index);
							} else {
								GCAnsCsNotifyStart(lpObj->m_Index, 0);
							}
						}
						#endif

						if ( lpObj->MapNumber == MAP_INDEX_CRYWOLF_FIRSTZONE )
						{
							g_CrywolfSync.NotifyCrywolfCurrentStateByUserId(lpObj->m_Index);
						}

						if(DragonEvent->GetState())
						{
							if(DragonEvent->GetMapNumber() == lpObj->MapNumber)
							{
								GCMapEventStateSend(lpObj->MapNumber,1,1);
							}
						}

						gEledoradoEvent.CheckGoldDercon(lpObj->MapNumber);

						if(gTamaJJangEvent!=0 && gAppearTamaJJang!=0)
						{
							if(lpObj->MapNumber == MAP_INDEX_LORENCIA)
							{
								GCMapEventStateSend(0,gAppearTamaJJang,2);
							}
						}

						if( gObjIsNewClass(lpObj) == 1 )
							Mastering.InfoSend(lpObj->m_Index);

						//if(skinId> -1)
						//{
						//	lpObj->m_Change = skinId;
						//	gObjViewportListProtocolCreate(lpObj);
						//}

					}
					else
					{
						if(gObjMonsterRegen(lpObj)==0)
						{
							continue;
						}
						CreateFrustrum(lpObj->X,lpObj->Y,n);
					}

					lpObj->DieRegen = 0;
					lpObj->m_State = 1;
				}
			}
		}
	}

	for(n = 0; n < MAX_MAP_NUMBER; n++)
	{
		MapC[n].StateSetDestroy();
	}
}






void gObjSecondProc()
{
	//DebugLog("%s START",__FUNCTION__);
	if(gDisconnect == 1)
	{
		//DebugLog("%s END",__FUNCTION__);
		return;
	}

	int n;
	LPOBJ lpObj;

	for(n = 0; n < OBJMAX;n++)
	{
		try
		{
		lpObj = &gObj[n];

		if(lpObj->Connected > PLAYER_LOGGED)
		{
			if(lpObj->m_bMapSvrMoveQuit == true)
			{
				if(GetTickCount() - lpObj->m_dwMapSvrQuitTick > 30000)
				{
					gObjDel(lpObj->m_Index);
					continue;
				}
			}

			gObjSkillUseProc(lpObj);
			gObjSkillBeAttackProc(lpObj);

			if(lpObj->Type == OBJ_MONSTER)
			{
				if(lpObj->Class == 275 || lpObj->Class == 338)
				{
					gObjRefillMonsterHP(lpObj,giKundunRefillHPSec);
				}

				#if (GS_CASTLE==1)
				//Guardian Statue	sub_403C29(*(_DWORD *)v11);		[00562000]
				if (lpObj->Class == 283)
				{
					g_GuardianStatue.GuardianStatueAct(lpObj->m_Index);
					continue;
				}

				//Guardian
				if(lpObj->Class == 285)
				{
					g_CsNPC_Guardian.GuardianAct(lpObj->m_Index);
					continue;
				}

				//Life Stone		sub_401FFF(*(_DWORD *)v11);		[00562F60]
				if (lpObj->Class == 278)
				{
					g_CsNPC_LifeStone.LifeStoneAct(lpObj->m_Index);
					continue;
				}

				//Canon Tower		sub_402EC8(*(_DWORD *)v11);		[00560940]
				if (lpObj->Class == 288)
				{
					g_CsNPC_CannonTower.CannonTowerAct(lpObj->m_Index);
					continue;
				}
				#endif
			}

			if(lpObj->Type == OBJ_NPC)
			{
				#if (GS_CASTLE==1)
				//CryWolf Altars
				if ((lpObj->Class >= 204)&&(lpObj->Class <= 209))
				{
					g_Crywolf.CrywolfNpcAct(lpObj->m_Index);
				}

				//Castle Crown		sub_4019C4(*(DWORD *)v11);		[00560DA0]
				if (lpObj->Class == 216)
				{
					g_CastleCrown.CalcAccessCrownTime(lpObj->m_Index);
					continue;
				}

				//Crown Switches	sub_40491C(*(DWORD *)v11);		[00561490]
				if ((lpObj->Class >= 217)&&(lpObj->Class <= 218))
				{
					g_CastleCrownSwitch.CrownSwitchPushTime(lpObj->m_Index);
					continue;
				}
				#endif

				if(lpObj->Class == 221 || lpObj->Class == 222)
				{
					g_CsNPC_Weapon.WeaponAct(lpObj->m_Index);
				}

				if(lpObj->Class == 229)	//Marlon
				{
					gQuestNpcTeleport.Run(n);
				}

				if(lpObj->Class == 568)	//Wandering Merchant
				{
					gQuestNpcTeleport.Run(n);
				}
			}
#if (PACK_EDITION>=3)
			if(lpObj->Type == OBJ_MONSTER && lpObj->IsBot >= 1)
			{
				botPet.gProcessAutoRecuperation(lpObj);
			}
#endif
			if(lpObj->Type == OBJ_USER)
			{
				if(ReadConfig.UseMoveperSecs == 1)
				{
					if(lpObj->MovePerSecs >= ReadConfig.AllowedMoveperSecs)
					{
						LogAddC(2,"[%s][%s] Make %d/Sec Moviments,Speed Hack Detected",lpObj->AccountID,lpObj->Name,lpObj->MovePerSecs);
						CloseClient(lpObj->m_Index);
					}
				}
				lpObj->MovePerSecs = 0;
#if (PACK_EDITION>=3)
#if (CRYSTAL_EDITION == 1)
				CrystalWall.CheckPing(lpObj->m_Index);
#endif
				BotRacer.CheckRaceTime(lpObj->m_Index);
#endif
				gObjManaPotionFill(lpObj);
				gObjRestPotionFill(lpObj);

				gObjCheckXYMapTile(lpObj,3);

#if (PACK_EDITION>=2)
				if(lpObj->MarryCharacterInfo->MarrySeconds > 0)
					lpObj->MarryCharacterInfo->MarrySeconds--;
#endif

				if(lpObj->ChatLimitTime > 0)
				{
					lpObj->ChatLimitTimeSec++;

					if(lpObj->ChatLimitTimeSec > 60)
					{
						lpObj->ChatLimitTimeSec = 0;
						lpObj->ChatLimitTime--;

						if(lpObj->ChatLimitTime < 1)
						{
							lpObj->ChatLimitTime = 0;
							MsgOutput(lpObj->m_Index,lMsg.Get(1141));
						}
					}
				}

				if(g_iUseCharacterAutoRecuperationSystem != 0)
				{
					if(lpObj->Level <= g_iCharacterRecuperationMaxLevel)
					{
						gProcessAutoRecuperation(lpObj);
					}
				}
	
				if(lpObj->Type == OBJ_USER)
				{
					gObjShieldAutoRefill(lpObj);
				}
	
				/*if(lpObj->Type == OBJ_USER && lpObj->m_LastTeleportTime > 0)
				{
					lpObj->m_LastTeleportTime--;
				}*/
	
				gObjDelayLifeCheck(n);
				gObjSecondDurDown(lpObj);
				gObjOfficialHelperTimer(lpObj);
	
				if(lpObj->PartyNumber >= 0)
				{
					gParty.PartyMemberLifeSend(lpObj->PartyNumber);
				}
	
				if(lpObj->m_ShopTime >= 1 && lpObj->m_ShopTime < 255)
				{
					lpObj->m_ShopTime++;
				}
	
				int BattleGround = 1;
				int m_BattleTimer = gCheckBattleGroundTimer();
	
				if((lpObj->Authority & 2) == 2)
				{
					BattleGround = 0;
					GCTimeViewSend(lpObj->m_Index,m_BattleTimer);
				}
	
				if(lpObj->MapNumber == MAP_INDEX_BATTLESOCCER && BattleGround == 1)
				{
					BattleGround = 1;
	
					if(lpObj->lpGuild != 0)
					{
						if(lpObj->lpGuild->WarState != 0)
						{
							int CheckBattleGround = gCheckBattleGround(lpObj);
	
							if(CheckBattleGround != lpObj->lpGuild->BattleGroundIndex)
							{
								if(lpObj->lpGuild->WarType == 1)
								{
									BattleGround = 0;
								}
								else
								{
									BattleGround = 0;
								}
							}
							else
							{
								BattleGround = 0;
							}
						}
					}
					else
					{
						BattleGround = 1;
					}
	
					if(BattleGround != 0)
					{
						int CheckBattleGround = gCheckBattleGround(lpObj);
		
						if(CheckBattleGround >= 0)
						{
							gObjMoveGate(lpObj->m_Index,17);
						}	
					}
				}
			}
		}

		if(lpObj->Connected >= PLAYER_LOGGED && 
				lpObj->Type == OBJ_USER &&
				lpObj->CloseCount > 0)
		{
			if(lpObj->CloseCount == 1)
			{
				if(lpObj->CloseType == 1)
				{
					if(gObjGameClose(lpObj->m_Index) == 1)
					{
						GCCloseMsgSend(lpObj->m_Index,1);
						GJUpdateMatchDBUserCharacters(lpObj);
					}
				}
				else if(lpObj->CloseType == 0)
				{
					GCCloseMsgSend(lpObj->m_Index,0);
					CloseClient(lpObj->m_Index);
				}
				else if(lpObj->CloseType == 2)
				{
					GCCloseMsgSend(lpObj->m_Index,2);
				}
			}
			else
			{
				char Msg[128];
				wsprintf(Msg,lMsg.Get(1116),lpObj->CloseCount-1);
				GCServerMsgStringSend(Msg,lpObj->m_Index,1);
			}
			lpObj->CloseCount--;
		}

		if(lpObj->Connected > PLAYER_LOGGED &&
			lpObj->Type == OBJ_USER)
		{
			
			if(lpObj->WarehouseTimeCount > 0)
				lpObj->WarehouseTimeCount--;

			if(lpObj->b_BlockMoveCommandSeconds > 0)
				lpObj->b_BlockMoveCommandSeconds--;
			else
				lpObj->b_BlockMoveCommandSeconds = 0;

#if (PACK_EDITION>=2)
			if(lpObj->MarryCharacterInfo->aFloodMarryCmd > 0)
				lpObj->MarryCharacterInfo->aFloodMarryCmd--;
#endif

#if (PACK_EDITION>=3)
			if(lpObj->aFloodBotPetCmd > 0)
				lpObj->aFloodBotPetCmd--;
#endif

			if(lpObj->aFloodPostCmd > 0)
				lpObj->aFloodPostCmd--;

			if(GetTickCount() - lpObj->AutoSaveTime > 600000)
			{
				GJSetCharacterInfo(lpObj,n,0);
				lpObj->AutoSaveTime = GetTickCount();
			}

			if(lpObj->CheckSumTime > 0 && GetTickCount() - lpObj->CheckSumTime > 6000)
			{
				LogAddC(2,"[CheckSumTime] Index: %d Time: %d",n,lpObj->CheckSumTime);
				CloseClient(n);
			}

			gObjPkDownTimeCheck(lpObj,1);
			gObjInterfaceTimeCheck(lpObj);
			gObjTimeCheckSelfDefense(lpObj);
			gObjNProtectGGCheckSum(n);
		}

		if((lpObj->Connected == PLAYER_CONNECTED ||
			lpObj->Connected == PLAYER_LOGGED ||
			lpObj->Connected == PLAYER_PLAYING) 
			&& ReadConfig.ResponErrorCheck == 1)
		{
			if(lpObj->Type == OBJ_USER)
			{
				if(lpObj->Connected >= PLAYER_LOGGED)
				{
					if(GetTickCount() - lpObj->ConnectCheckTime > ReadConfig.ResponErrorTimePlaying)
					{
						LogAddTD(lMsg.Get(542),lpObj->m_Index,lpObj->AccountID,lpObj->Name,lpObj->Ip_addr);
						ResponErrorCloseClient(n);
					}
				}
				else
				{
					if(GetTickCount() - lpObj->ConnectCheckTime > ReadConfig.ResponErrorTimeConnected)
					{
						LogAddTD(lMsg.Get(543),lpObj->m_Index,lpObj->AccountID,lpObj->Name,lpObj->Ip_addr);
						ResponErrorCloseClient(n);
					}
				}
			}
		}
		}catch(...)
		{
			//DebugLog("%s ERROR\n",__FUNCTION__);
		}
	}

	if( gTamaJJangEvent != 0)
	{
		gTamaJJangTime--;

		if(gTamaJJangTime <= 0)
		{
			if(gAppearTamaJJang==0)
			{
				gAppearTamaJJang = 1;
				gTamaJJangTime = gTamaJJangKeepTime;
				LogAddTD("Appear TamaJJang : Keep Time (%d)",gTamaJJangTime);
			}
			else
			{
				gAppearTamaJJang = 0;

				gTamaJJangTime = gTamaJJangDisappearTime +rand()% gTamaJJangDisappearTimeRandomRange;
				LogAddTD("DisAppear TamaJJang : Keep Time (%d)",gTamaJJangTime);
			}

			GCMapEventStateSend(0,gAppearTamaJJang,2);
		}
	}
	
	//DebugLog("%s END",__FUNCTION__);
	//gGameServerAuth.SendInfo();
}






void gObjManaPotionFill(LPOBJ lpObj)
{
	lpObj->m_ManaFillCount++;

	float tmana;
	int ChangeMana = 0;

	if(lpObj->m_ManaFillCount > 2)
	{
		lpObj->m_ManaFillCount = 0;

		int BP,AddBP;
		BYTE MapAttr;
		int rBP,CurBP;

		if(lpObj->MaxMana + lpObj->AddMana > lpObj->Mana)
		{
			tmana = lpObj->Mana;
			if(ReadConfig.S6E2 == 0)
				tmana += (lpObj->MaxMana + lpObj->AddMana)*0.037f;
			else
			{
				if(gObjIsNewClass(lpObj) == 1 && lpObj->MasterCharacterInfo->RegenMana > 0)
				{					
					//tmana += (lpObj->MaxMana + lpObj->AddMana)*lpObj->MasterCharacterInfo->RegenMana;
					tmana += (lpObj->MaxMana + lpObj->AddMana)*lpObj->MasterCharacterInfo->RegenMana*0.037f;
				}else
				{
					tmana += (lpObj->MaxMana + lpObj->AddMana)*0.037f;
				}
			}

			tmana += lpObj->m_JewelOfHarmonyEffect.HJOpAddRefillMP;

			if(lpObj->MaxMana + lpObj->AddMana < tmana)
			{
				tmana = lpObj->MaxMana + lpObj->AddMana;
			}

			lpObj->Mana = tmana;
			ChangeMana = 1;
		}

		BP = lpObj->MaxBP + lpObj->AddBP;

		if(lpObj->BP < BP)
		{
			MapAttr = MapC[lpObj->MapNumber].GetAttr((short &)lpObj->X,(short &)lpObj->Y);
			rBP = 2;

			if((MapAttr & 1) != 0)
			{
				rBP = 5;
			}

			if(lpObj->Class == CLASS_KNIGHT)
			{
				AddBP = rBP +(BP * 5 / 100);
			}
			else
			{
				AddBP = rBP +(BP * 3 / 100);
			}

			if(lpObj->m_iPotionSoulTime > 0)
			{
				AddBP += 8;
			}

			AddBP += lpObj->SetOpIncAGValue;

			if(AddBP < 1)
			{
				AddBP = 1;
			}

			if(ReadConfig.S6E2 == 1)
			{
				if(gObjIsNewClass(lpObj) == 1 && lpObj->MasterCharacterInfo->RegenAG > 0)
				{					
					AddBP = AddBP * lpObj->MasterCharacterInfo->RegenAG;
				}
			}

			CurBP = lpObj->BP;
			CurBP += AddBP;

			if(CurBP > BP)
			{
				CurBP = BP;
			}

			lpObj->BP = CurBP;

			ChangeMana = 1;
		}

		if(ChangeMana != 0)
		{
			GCManaSend(lpObj->m_Index,lpObj->Mana,255,0,lpObj->BP);
		}
	}
}






void gObjRestPotionFill(LPOBJ lpObj)
{
	if(lpObj->m_Rest != 0)
	{
		float tlife;
		float tmana;
		int percent = 3;
		lpObj->m_LifeFillCount++;

		if(lpObj->m_LifeFillCount > 4)
		{
			lpObj->m_LifeFillCount = 0;

			tlife = lpObj->Life;

			if(lpObj->pInventory[10].IsItem() == 1)
			{
				if(lpObj->pInventory[10].m_Z28Option != 0 && lpObj->pInventory[10].m_Type != ITEMGET(13,24))
				{
					percent += lpObj->pInventory[10].m_Z28Option;
				}
			}
			else if(lpObj->pInventory[11].IsItem() == 1)
			{
				if(lpObj->pInventory[11].m_Z28Option != 0 && lpObj->pInventory[11].m_Type != ITEMGET(13,24))
				{
					percent += lpObj->pInventory[11].m_Z28Option;
				}
			}
			else if(lpObj->pInventory[9].IsItem() == 1)
			{
				if(lpObj->pInventory[9].m_Z28Option != 0 && lpObj->pInventory[9].m_Type != ITEMGET(13,28))
				{
					percent += lpObj->pInventory[9].m_Z28Option;
				}
			}

			if (lpObj->Type == OBJ_USER)
			{
				if(lpObj->MasterCharacterInfo->AutoRegen > 0)
				{
					percent += lpObj->MasterCharacterInfo->AutoRegen;
				}
			}

			tlife += (lpObj->MaxLife + lpObj->AddLife)*percent / 100.0f;
			tlife += lpObj->m_JewelOfHarmonyEffect.HJOpAddRefillHP;
			
			if((lpObj->MaxLife + lpObj->AddLife) < tlife)
			{
				tlife = lpObj->MaxLife + lpObj->AddLife;
			}

		lpObj->Life = tlife;

		GCReFillSend(lpObj->m_Index,lpObj->Life,255,0,lpObj->iShield);

		tmana = lpObj->Mana + lpObj->AddMana;
		tmana += (lpObj->MaxMana + lpObj->AddMana) * 3.0f / 100.0f;
		tmana += lpObj->m_JewelOfHarmonyEffect.HJOpAddRefillMP;

		if((lpObj->MaxMana + lpObj->AddMana) < tmana)
		{
			tmana = lpObj->MaxMana + lpObj->AddMana;
		}

		lpObj->Mana = tmana;

		GCManaSend(lpObj->m_Index,lpObj->Mana,255,0,lpObj->BP);
		}
	}
	else
	{
		float percent = 0;
		float tlife;
		if(lpObj->pInventory[10].IsItem() == 1)
		{
			if(lpObj->pInventory[10].m_Z28Option != 0 && lpObj->pInventory[10].m_Durability != 0.0f && lpObj->pInventory[10].m_Type != ITEMGET(13,24))
			{
				percent += lpObj->pInventory[10].m_Z28Option;
			}
		}

		if(lpObj->pInventory[11].IsItem() == 1)
		{
			if(lpObj->pInventory[11].m_Z28Option != 0 && lpObj->pInventory[11].m_Durability != 0.0f && lpObj->pInventory[11].m_Type != ITEMGET(13,24))
			{
				percent += lpObj->pInventory[11].m_Z28Option;
			}
		}

		if(lpObj->pInventory[9].IsItem() == 1)
		{
			if(lpObj->pInventory[9].m_Z28Option != 0 && lpObj->pInventory[9].m_Durability != 0.0f && lpObj->pInventory[9].m_Type != ITEMGET(13,28))
			{
				percent += lpObj->pInventory[9].m_Z28Option;
			}
		}

		if(lpObj->pInventory[7].IsItem() == 1)
		{
			if(lpObj->pInventory[7].m_Z28Option != 0 && lpObj->pInventory[7].m_Special[2] == 85 && lpObj->pInventory[7].m_Durability != 0.0f)
			{
				percent += lpObj->pInventory[7].m_Z28Option;
			}
		}

		if (lpObj->Type == OBJ_USER)
		{
			if(lpObj->MasterCharacterInfo->AutoRegen > 0)
			{
				percent += lpObj->MasterCharacterInfo->AutoRegen;
			}
		}

		if(percent != 0)
		{
			lpObj->m_LifeFillCount++;

			if(lpObj->m_LifeFillCount > 6)
			{
				lpObj->m_LifeFillCount = 0;
				tlife = lpObj->Life;

				tlife += (lpObj->MaxLife +lpObj->AddLife) * percent / 100.f;
				tlife += lpObj->m_JewelOfHarmonyEffect.HJOpAddRefillHP;

				if((lpObj->MaxLife + lpObj->AddLife) < tlife)
				{
					tlife = lpObj->MaxLife + lpObj->AddLife;
				}

				lpObj->Life = tlife;

				GCReFillSend(lpObj->m_Index,lpObj->Life,255,0,lpObj->iShield);
			}
		}
	}

	if(lpObj->iFillShield > 0)
	{
		int iFillSDPoint = 0;
		if(lpObj->iFillShieldCount <= 0)
		{
			lpObj->iFillShieldCount = 1;
		}

		iFillSDPoint = lpObj->iFillShieldMax / lpObj->iFillShieldCount;

		lpObj->iShield += iFillSDPoint;

		lpObj->iFillShield -= iFillSDPoint;

		if(lpObj->iFillShield < iFillSDPoint)
		{
			iFillSDPoint = lpObj->iFillShield;
			lpObj->iFillShield = 0;

			if(iFillSDPoint < 0)
			{
				iFillSDPoint = 0;
			}
		}

		if(lpObj->iShield > (lpObj->iMaxShield + lpObj->iAddShield))
		{
			lpObj->iShield = lpObj->iMaxShield + lpObj->iAddShield;

			lpObj->iFillShield = 0;
			lpObj->iFillShieldMax = 0;
		}

		GCReFillSend(lpObj->m_Index,lpObj->Life,255,0,lpObj->iShield);
	} else {
		if (lpObj->Type == OBJ_USER)
		{
			if(lpObj->MasterCharacterInfo->IncRecoverSDRate > 0)
			{
				lpObj->m_iShieldFillCount++;

				if(lpObj->m_iShieldFillCount > 6)
				{
					int tshield = lpObj->iShield;

					tshield += (int)(((lpObj->iMaxShield +lpObj->iAddShield)/lpObj->m_iShieldFillCount) * lpObj->MasterCharacterInfo->IncRecoverSDRate / 100);
					lpObj->m_iShieldFillCount = 0;

					if((lpObj->iMaxShield + lpObj->iAddShield) < tshield)
					{
						tshield = lpObj->iMaxShield + lpObj->iAddShield;
					}

					lpObj->iShield = tshield;
					lpObj->m_iShieldFillCount = 0;

					GCReFillSend(lpObj->m_Index,lpObj->Life,255,0,lpObj->iShield);
				}
			}
		}
	}

	if(lpObj->FillLife > 0)
	{
		float cur = lpObj->FillLifeMax * 34.0f / 100.0f;

		if(lpObj->FillLifeCount == 2)
		{
			cur = lpObj->FillLifeMax * 54.0f / 100.0f;
		}

		if(lpObj->FillLifeCount == 4)
		{
			cur = lpObj->FillLifeMax * 25.0f / 100.0f;
		}

		if(lpObj->FillLife < cur)
		{
			cur = lpObj->FillLife;
			lpObj->FillLife = 0;

			if(cur < 0)
			{
				cur = 0;
			}
		}
		else
		{
			lpObj->FillLife -= cur;
		}

		lpObj->Life += cur;

		if((lpObj->MaxLife + lpObj->AddLife) < lpObj->Life)
		{
			lpObj->Life = lpObj->MaxLife + lpObj->AddLife;
			lpObj->FillLife = 0;
		}

		GCReFillSend(lpObj->m_Index,lpObj->Life,255,0,lpObj->iShield);
	}
}






void gObjUseDrink(LPOBJ lpObj, int level)
{
	int time;

	if ( level == 2 )
	{
		time = 90;
		GCItemUseSpecialTimeSend(lpObj->m_Index, 1, time);
		lpObj->m_SkillAttack2 = 1;
		lpObj->m_SkillAttackTime2 = 90;
	}
	else if ( level == 1 )
	{
		time = 180;
		GCItemUseSpecialTimeSend(lpObj->m_Index, 0, time);
	}
	else if ( level == 0x4D )
	{
		time = 60;
		GCItemUseSpecialTimeSend(lpObj->m_Index, 2, time);
	}
	else
	{
		time = 80;
		GCItemUseSpecialTimeSend(lpObj->m_Index, 0, time);
	}
}


void gObjViewportListProtocolDestroy(LPOBJ lpObj)
{
	BYTE sendBuf[5000];
	int lOfs,count;
	count = 0;
	lOfs = 4;

	PBMSG_COUNT pCount;

	pViewportDestroy.NumberH = SET_NUMBERH(lpObj->m_Index);
	pViewportDestroy.NumberL = SET_NUMBERL(lpObj->m_Index);


	memcpy(&sendBuf[lOfs],&pViewportDestroy,2);

	lOfs += 2;
	count += 1;

	pCount.h.c = 0xC1;
	pCount.h.headcode = 20;
	pCount.h.size = lOfs;
	pCount.count = count;

	memcpy(sendBuf,&pCount,sizeof(pCount));

	if(count < 1)	{
		return;
	}

	MsgSendV2(lpObj,sendBuf,lOfs);
}


void gObjViewportListProtocolCreate(LPOBJ lpObj)
{
	int n,tObjNum;
	BYTE sendBuf[5000];
	BYTE sendBuf2[5000];
	int lOfs,count,moncount;
	PWMSG_COUNT pwCount;
	PWMSG_SUBCODE_COUNT pwCount2 = {0};

	count = 0;
	moncount = 0;
	lOfs = sizeof(pwCount);
	int lOfs2 = sizeof(pwCount2);
	int count2 = 0;

	int MVL = MAX_VIEWPORT;

	if(lpObj->Type == OBJ_MONSTER)
	{
		MVL = MAX_VIEWPORT_MONSTER;
	}


	if(lpObj->Type == OBJ_USER
#if (PACK_EDITION>=3)
		|| lpObj->IsBot >= 1
#endif
		)
	{
		if(lpObj->m_Change >= 0
#if (PACK_EDITION>=3)
			&& lpObj->IsBot == 0
#endif
			)
		{

			pViewportCreateChange.NumberH = SET_NUMBERH(lpObj->m_Index);
			pViewportCreateChange.NumberL = SET_NUMBERL(lpObj->m_Index);

			pViewportCreateChange.NumberH |= 0x80;

			pViewportCreateChange.X = lpObj->X;
			pViewportCreateChange.Y = lpObj->Y;

			pViewportCreateChange.TX = lpObj->TX;
			pViewportCreateChange.TY = lpObj->TY;

			pViewportCreateChange.SkinH = SET_NUMBERH((lpObj->m_Change & 0xFFFF) & 0xFFFF);
			pViewportCreateChange.SkinL = SET_NUMBERL((lpObj->m_Change & 0xFFFF) & 0xFFFF);

			//pViewportCreateChange.ViewSkillState = lpObj->m_ViewSkillState;
			pViewportCreateChange.DirAndPkLevel = lpObj->Dir << 4;
			pViewportCreateChange.DirAndPkLevel |= lpObj->m_PK_Level & 0x0F;

			//if(CC_MAP_RANGE(lpObj->MapNumber))
			//{
			//	pViewportCreateChange.ViewSkillState = 0;
			//}

			int SkillStateCount = ViewPortChangeSkillStates(lpObj,&pViewportCreateChange);
			pViewportCreateChange.SkillStateCount = SkillStateCount;

			//if ( IT_MAP_RANGE(lpObj->MapNumber) == TRUE )	//Illusion Temple Character View
			//{
				memcpy(pViewportCreateChange.CharSet,lpObj->CharSet,sizeof(pViewportCreateChange.CharSet));
			//} else {
			//	memset(pViewportCreateChange.CharSet, 0xFF, sizeof(pViewportCreateChange.CharSet));
			//}

			memcpy(pViewportCreateChange.Id,lpObj->Name,sizeof(pViewportCreateChange.Id));
			memcpy(&sendBuf[lOfs],&pViewportCreateChange,sizeof(pViewportCreateChange));

			lOfs += sizeof(pViewportCreateChange) - sizeof(pViewportCreateChange.ViewChangeSkillState) + pViewportCreateChange.SkillStateCount;
			//lOfs += sizeof(pViewportCreateChange);
			count += 1;

			if(count < 1)
			{
				return;
			}

			pwCount.h.headcode = 0x45;
		}
		else	//PROTOCOL SKILL STATE VISUAL BUG
		{
			pViewportCreate.NumberH = SET_NUMBERH(lpObj->m_Index);
			pViewportCreate.NumberL = SET_NUMBERL(lpObj->m_Index);

			if(lpObj->m_State == 1 && lpObj->Teleport==0)
			{
				pViewportCreate.NumberH |= 0x80;
			}

			int SkillStateCount = ViewPortSkillStates(lpObj,&pViewportCreate);
			pViewportCreate.SkillStateCount = SkillStateCount;

			pViewportCreate.X = lpObj->X;
			pViewportCreate.Y = lpObj->Y;
			pViewportCreate.TX = lpObj->TX;
			pViewportCreate.TY = lpObj->TY;

			pViewportCreate.DirAndPkLevel = lpObj->Dir << 4;
			pViewportCreate.DirAndPkLevel |= lpObj->m_PK_Level & 0x0F;

			memcpy(pViewportCreate.CharSet,lpObj->CharSet,sizeof(pViewportCreate.CharSet));
			memcpy(pViewportCreate.Id,lpObj->Name,sizeof(pViewportCreate.Id));
			memcpy(&sendBuf[lOfs],&pViewportCreate,sizeof(pViewportCreate));

			lOfs += sizeof(pViewportCreate) - sizeof(pViewportCreate.ViewSkillState) + pViewportCreate.SkillStateCount;
			count += 1;

			if(count < 1)
			{
				return;
			}

			pwCount.h.headcode = 0x12;
		}

		pwCount.h.c = 0xC2;
		pwCount.h.sizeH = SET_NUMBERH(lOfs);
		pwCount.h.sizeL = SET_NUMBERL(lOfs);
		pwCount.count = count;

		memcpy(sendBuf,&pwCount,sizeof(pwCount));

		DataSend(lpObj->m_Index,sendBuf,lOfs);

		if(Gens.Enabled == 1)
		{
			if(lpObj->Type == OBJ_USER
	#if (PACK_EDITION>=3)
				&& lpObj->IsBot == 0
	#endif
				)
			{
				if(lpObj->GensFam > 0)
				{
					Gens.Ranking(lpObj->m_Index);
					pViewportCreateGens.Family = lpObj->GensFam;
					pViewportCreateGens.NumberH = SET_NUMBERH(lpObj->m_Index);
					pViewportCreateGens.NumberL = SET_NUMBERL(lpObj->m_Index);
					pViewportCreateGens.Level = lpObj->GensLevel;
					memcpy(&sendBuf2[lOfs2],&pViewportCreateGens,sizeof(pViewportCreateGens));
					lOfs2 += sizeof(pViewportCreateGens);
					count2 += 1;

					pwCount2.h.c = 0xC2;
					pwCount2.h.sizeH = SET_NUMBERH(lOfs2);
					pwCount2.h.sizeL = SET_NUMBERL(lOfs2);
					pwCount2.h.headcode = 0xF8;
					pwCount2.count = count2;
					pwCount2.subcode = 5;
					memcpy(&sendBuf2[0],&pwCount2,sizeof(pwCount2));
				}
			}
		}

		if(lpObj->GuildNumber != 0)
		{
			GuildInfoOfs = 0;
			GuildInfoOfs = 5;

			PMSG_SIMPLE_GUILDVIEWPORT pGuild;

			pGuild.GuildNumber = lpObj->lpGuild->Number;
			pGuild.NumberH = SET_NUMBERH(lpObj->m_Index) & 0x7F;
			pGuild.NumberL = SET_NUMBERL(lpObj->m_Index);

			if(strcmp(lpObj->lpGuild->Names[0],lpObj->Name) == 0)
			{
				pGuild.NumberH |= 0x80;
			}

			pGuild.btGuildType = lpObj->lpGuild->btGuildType;
			pGuild.btGuildStatus = lpObj->GuildStatus;

			memcpy(&GuildInfoBuf[GuildInfoOfs],&pGuild,sizeof(pGuild));

			GuildInfoOfs += sizeof(pGuild);

			PMSG_SIMPLE_GUILDVIEWPORT_COUNT pwCount;

			pwCount.h.c = 0xC2;
			pwCount.h.headcode = 0x65;
			pwCount.h.sizeH = SET_NUMBERH(GuildInfoOfs);
			pwCount.h.sizeL = SET_NUMBERL(GuildInfoOfs);
			pwCount.Count = 1;

			memcpy(GuildInfoBuf,&pwCount,sizeof(pwCount));

			DataSend(lpObj->m_Index,GuildInfoBuf,GuildInfoOfs);
		}

		for(n = 0; n < MVL; n++)
		{
			if(lpObj->VpPlayer2[n].state != 0)
			{
				tObjNum = lpObj->VpPlayer2[n].number;

				if(tObjNum >= 0)
				{
					if(lpObj->VpPlayer2[n].type == OBJ_USER)
					{
						DataSend(tObjNum,sendBuf,lOfs);

						if(GuildInfoCount != 0)
						{
							DataSend(tObjNum,GuildInfoBuf,GuildInfoOfs);
						}

						if(GuildUserCount != 0)
						{
							DataSend(tObjNum,GuildUserBuf,GuildUserOfs);
						}

						if(count2 != 0)
						{
							DataSend(tObjNum,sendBuf2,lOfs2);
						}
					}
				}
			}
		}
	}
	else
	{
		count = 0;

		//if(lpObj->IsBot >= 1)
		//{			
		//	pViewportCreate.NumberH = SET_NUMBERH(lpObj->m_Index);
		//	pViewportCreate.NumberL = SET_NUMBERL(lpObj->m_Index);

		//	if(lpObj->m_State == 1 && lpObj->Teleport==0)
		//	{
		//		pViewportCreate.NumberH |= 0x80;
		//	}

		//	int SkillStateCount = ViewPortSkillStates(lpObj,&pViewportCreate);
		//	pViewportCreate.SkillStateCount = SkillStateCount;

		//	pViewportCreate.X = lpObj->X;
		//	pViewportCreate.Y = lpObj->Y;
		//	pViewportCreate.TX = lpObj->TX;
		//	pViewportCreate.TY = lpObj->TY;

		//	pViewportCreate.DirAndPkLevel = lpObj->Dir << 4;
		//	pViewportCreate.DirAndPkLevel |= lpObj->m_PK_Level & 0x0F;

		//	memcpy(pViewportCreate.CharSet,lpObj->CharSet,sizeof(pViewportCreate.CharSet));
		//	memcpy(pViewportCreate.Id,lpObj->Name,sizeof(pViewportCreate.Id));
		//	memcpy(&sendBuf[lOfs],&pViewportCreate,sizeof(pViewportCreate));

		//	lOfs += sizeof(pViewportCreate) - sizeof(pViewportCreate.ViewSkillState) + pViewportCreate.SkillStateCount;
		//	count += 1;

		//	if(count < 1)
		//	{
		//		return;
		//	}

		//	pwCount.h.headcode = 0x12;
		//	pwCount.h.c = 0xC2;
		//	pwCount.h.sizeH = SET_NUMBERH(lOfs);
		//	pwCount.h.sizeL = SET_NUMBERL(lOfs);
		//	pwCount.count = count;

		//	memcpy(sendBuf,&pwCount,sizeof(pwCount));

		//	DataSend(lpObj->m_Index,sendBuf,lOfs);

		//	return;
		//}
	
		if(lpObj->m_RecallMon >= 0)
		{
			pCallMonsterViewportCreate.NumberH = SET_NUMBERH(lpObj->m_Index);
			pCallMonsterViewportCreate.NumberL = SET_NUMBERL(lpObj->m_Index);
	
			if(lpObj->m_State == 1)
			{
				pCallMonsterViewportCreate.NumberH |= 0x80;
			}
	
			pCallMonsterViewportCreate.Type_HI = SET_NUMBERH(lpObj->Class);
			pCallMonsterViewportCreate.Type_LO = SET_NUMBERL(lpObj->Class);
			pCallMonsterViewportCreate.ViewState = lpObj->m_ViewSkillState;
	
			pCallMonsterViewportCreate.X = lpObj->X;
			pCallMonsterViewportCreate.Y = lpObj->Y;
	
			pCallMonsterViewportCreate.TX = lpObj->TX;
			pCallMonsterViewportCreate.TY = lpObj->TY;
	
			pCallMonsterViewportCreate.Path = lpObj->Dir << 4;
	
			if(lpObj->m_RecallMon >= 0 && lpObj->m_RecallMon < OBJMAX-1)
			{
				memcpy(pCallMonsterViewportCreate.Id,gObj[lpObj->m_RecallMon].Name,sizeof(pCallMonsterViewportCreate.Id));
			}
			else
			{
				memset(pCallMonsterViewportCreate.Id,0x00,sizeof(pCallMonsterViewportCreate.Id));
			}
	
			memcpy(&sendBuf[lOfs],&pCallMonsterViewportCreate,sizeof(pCallMonsterViewportCreate));
	
			lOfs += sizeof(pCallMonsterViewportCreate);
			count += 1;
		}
		else
		{
		
			pMonsterViewportCreate.NumberH = SET_NUMBERH(lpObj->m_Index);
			pMonsterViewportCreate.NumberL = SET_NUMBERL(lpObj->m_Index);
	
			if(lpObj->m_State == 1 && lpObj->Teleport == 0)
			{
				pMonsterViewportCreate.NumberH |= 0x80;
				pMonsterViewportCreate.NumberH |= 0x40;
			}
	
			pMonsterViewportCreate.Type_HI = SET_NUMBERH(lpObj->Class);
			pMonsterViewportCreate.Type_LO = SET_NUMBERL(lpObj->Class);
	
			int SkillStateCount = ViewPortMonsterSkillStates(lpObj,&pMonsterViewportCreate);
			pViewportCreate.SkillStateCount = SkillStateCount;
			//pMonsterViewportCreate.ViewState = lpObj->m_ViewSkillState;
			pMonsterViewportCreate.X = lpObj->X;
			pMonsterViewportCreate.Y = lpObj->Y;
			pMonsterViewportCreate.TX = lpObj->TX;
			pMonsterViewportCreate.TY = lpObj->TY;
	
			pMonsterViewportCreate.Path = lpObj->Dir << 4;
	
			memcpy(&sendBuf[lOfs],&pMonsterViewportCreate,sizeof(pMonsterViewportCreate));
			//lOfs += sizeof(pMonsterViewportCreate);
			lOfs += sizeof(pMonsterViewportCreate) - sizeof(pMonsterViewportCreate.ViewSkillState) + pMonsterViewportCreate.SkillStateCount;
			moncount += 1;
		}
	
		if(count > 0)
		{
			PWMSG_COUNT pwCount;
	
			pwCount.h.c = 0xC2;
			pwCount.h.headcode = 0x1F;
			pwCount.h.sizeH = SET_NUMBERH(lOfs);
			pwCount.h.sizeL = SET_NUMBERL(lOfs);
			pwCount.count = count;
	
			memcpy(sendBuf,&pwCount,sizeof(pwCount));
			MsgSendV2(lpObj,sendBuf,lOfs);
		}
	
		if(moncount > 0)
		{

			
			PWMSG_COUNT pwCount;
		
			pwCount.h.c = 0xC2;
			pwCount.h.headcode = 0x13;
			pwCount.h.sizeH = SET_NUMBERH(lOfs);
			pwCount.h.sizeL = SET_NUMBERL(lOfs);
			pwCount.count = moncount;

			memcpy(sendBuf,&pwCount,sizeof(pwCount));
			MsgSendV2(lpObj,sendBuf,lOfs);
		}
	}
}

void gObjViewportListProtocol(short aIndex)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	if(gObj[aIndex].Connected < PLAYER_PLAYING)
	{
		return;
	}

	int n;
	PBMSG_COUNT pCount;
	LPOBJ lpTargetObj,lpObj = &gObj[aIndex];
	int lOfs_Item,count_Item,ItemStructSize = sizeof(PMSG_ITEMVIEWPORTCREATE),lOfs,MonlOfs,callMonlOfs;
	int pMsgSize;
	short tObjNum;
	BYTE count,monstercount,callmonstercount;

	BYTE sendBuf[5000];
	BYTE sendBuf2[5000]={0};
	BYTE MonstersendBuf[5000];
	BYTE callMonstersendBuf[5000];
	BYTE ItemBuf[5000];

	int GensInfoOfs = 6;
	int count2 = 0;
	GuildInfoOfs = 0;
	GuildInfoCount = 0;
	GuildUserOfs = 0;
	GuildUserCount = 0;
	ChangeCount = 0;
	GuildInfoOfs = 5;
	GuildUserOfs = 5;
	lOfsChange = 5;

	lOfs = 4;
	lOfs_Item = 5;
	count = 0;
	count_Item = 0;

	if(lpObj->Type == OBJ_USER)
	{
		for(n = 0; n < MAX_VIEWPORT;n++)
		{
			if(lpObj->VpPlayer[n].state == 3)
			{
				tObjNum = lpObj->VpPlayer[n].number;

				if(tObjNum >= 0)
				{
					switch(lpObj->VpPlayer[n].type)
					{
					case 1:
					case 2:
					case 3:
							pViewportDestroy.NumberH = SET_NUMBERH(tObjNum);
							pViewportDestroy.NumberL = SET_NUMBERL(tObjNum);

							memcpy(&sendBuf[lOfs],&pViewportDestroy,sizeof(pViewportDestroy));
							lOfs += sizeof(pViewportDestroy);
							count += 1;
						break;
					case 5:
						pItemViewportDestroy.NumberH = SET_NUMBERH(tObjNum);
						pItemViewportDestroy.NumberL = SET_NUMBERL(tObjNum);

						memcpy(&ItemBuf[lOfs_Item],&pItemViewportDestroy,sizeof(pItemViewportDestroy));

						lOfs_Item += sizeof(pItemViewportDestroy);
						count_Item += 1;
						break;
					}
				}

				lpObj->VpPlayer[n].state = 0;
				lpObj->VpPlayer[n].number = -1;
				lpObj->VPCount -= 1;
			}
		}
	}
	else if(lpObj->Type == OBJ_MONSTER || lpObj->Type == OBJ_NPC)
	{
		for(n = 0; n < MAX_VIEWPORT_MONSTER;n++)
		{
			if(lpObj->VpPlayer[n].state == 3)
			{
				lpObj->VpPlayer[n].state = 0;
				lpObj->VpPlayer[n].number = -1;
				lpObj->VPCount -= 1;
			}
		}
	}

	if(lpObj->Type == OBJ_USER)
	{
		if(count > 0)
		{
			pCount.h.c = 0xC1;
			pCount.h.headcode = 0x14;
			pCount.h.size = lOfs;
			pCount.count = count;

			memcpy(sendBuf,&pCount,sizeof(pCount));
			DataSend(aIndex,sendBuf,lOfs);
		}

		if(count_Item > 0)
		{
			pItemCount.h.c = 0xC2;
			pItemCount.h.headcode = 0x21;
			pItemCount.h.sizeH = SET_NUMBERH(lOfs_Item);
			pItemCount.h.sizeL = SET_NUMBERL(lOfs_Item);
			pItemCount.count = count_Item;

			memcpy(ItemBuf,&pItemCount,sizeof(pItemCount));
			DataSend(aIndex,ItemBuf,lOfs_Item);
		}
	}

	if(lpObj->Type == OBJ_MONSTER || lpObj->Type == OBJ_NPC)
	{
		for(n = 0; n < MAX_VIEWPORT_MONSTER;n++)
		{
			if(lpObj->VpPlayer[n].state == 1)
			{
				lpObj->VpPlayer[n].state = 2;
			}
		}
	}
	else
	{

		//int SplitCount = 0;
		//BYTE Spliter = 5;

		if(lpObj->Type == OBJ_USER)
		{
			count = 0;
			monstercount = 0;
			count_Item = 0;
			callmonstercount = 0;
			lOfs = 5;
			MonlOfs = lOfs;
			callMonlOfs = MonlOfs;
			lOfs_Item = 5;
			pMsgSize = 44;
			for(n=0;n<MAX_VIEWPORT;n++)
			{
				if(lpObj->VpPlayer[n].state == 1)
				{
					tObjNum = lpObj->VpPlayer[n].number;
		
					if(tObjNum >= 0)
					{
						switch(lpObj->VpPlayer[n].type)
						{
						case 1:
							lpTargetObj = &gObj[tObjNum];
#if (PACK_EDITION>=2)
							if(HalloweenEvent.Enabled == 1 && HalloweenEvent.Start == true && HalloweenEvent.CheckMap(lpTargetObj->MapNumber) == true)
							{
								pViewportCreateChange.NumberH = SET_NUMBERH(tObjNum);
								pViewportCreateChange.NumberL = SET_NUMBERL(tObjNum);
	
								if(lpTargetObj->m_State == 1 && lpTargetObj->Teleport == 0)
								{
									pViewportCreateChange.NumberH |= 0x80;
								}

								pViewportCreateChange.X = lpTargetObj->X;
								pViewportCreateChange.Y = lpTargetObj->Y;
								pViewportCreateChange.TX = lpTargetObj->TX;
								pViewportCreateChange.TY = lpTargetObj->TY;
								pViewportCreateChange.SkinH = SET_NUMBERH((373 & 0xFFFF) & 0xFFFF);
								pViewportCreateChange.SkinL = SET_NUMBERL((373 & 0xFFFF) & 0xFFFF);
								pViewportCreateChange.DirAndPkLevel = lpTargetObj->Dir << 4;
								pViewportCreateChange.DirAndPkLevel |= 6 & 0x0F;
								int SkillStateCount = ViewPortChangeSkillStates(lpObj,&pViewportCreateChange);
								pViewportCreateChange.SkillStateCount = SkillStateCount;

								//if ( IT_MAP_RANGE(lpTargetObj->MapNumber) == TRUE )	//Illusion Temple Character View
								//{
									memcpy(pViewportCreateChange.CharSet,lpTargetObj->CharSet,sizeof(pViewportCreateChange.CharSet));
								//} else {
								//	memset(pViewportCreateChange.CharSet, 0xFF, sizeof(pViewportCreateChange.CharSet));
								//}

								memcpy(pViewportCreateChange.Id,lpTargetObj->Name,sizeof(pViewportCreateChange.Id));
								memcpy(&SendGBufChange[lOfsChange],&pViewportCreateChange,sizeof(pViewportCreateChange));
								
								lOfsChange += sizeof(pViewportCreateChange) - sizeof(pViewportCreateChange.ViewChangeSkillState) + pViewportCreateChange.SkillStateCount;
								ChangeCount +=1;
							}else
#endif
							{		
								if(lpTargetObj->m_Change >= 0)
								{
									pViewportCreateChange.NumberH = SET_NUMBERH(tObjNum);
									pViewportCreateChange.NumberL = SET_NUMBERL(tObjNum);
			
								//	lpTargetObj->CharSet[0] &= 0xF0;
		
									if(lpTargetObj->m_State == 1 && lpTargetObj->Teleport == 0)
									{
										pViewportCreateChange.NumberH |= 0x80;
									}
									pViewportCreateChange.X = lpTargetObj->X;
									pViewportCreateChange.Y = lpTargetObj->Y;
									pViewportCreateChange.TX = lpTargetObj->TX;
									pViewportCreateChange.TY = lpTargetObj->TY;
									pViewportCreateChange.SkinH = SET_NUMBERH((lpTargetObj->m_Change & 0xFFFF) & 0xFFFF);
									pViewportCreateChange.SkinL = SET_NUMBERL((lpTargetObj->m_Change & 0xFFFF) & 0xFFFF);
									//pViewportCreateChange.ViewSkillState = lpTargetObj->m_ViewSkillState;

									pViewportCreateChange.DirAndPkLevel = lpTargetObj->Dir << 4;
									if ( IT_MAP_RANGE(lpTargetObj->MapNumber) == TRUE && lpTargetObj->Type == OBJ_USER)	//No CTRL attack in Illusion Fix
									{
										if (lpObj->m_Change != lpTargetObj->m_Change)
										{
											pViewportCreateChange.DirAndPkLevel |= ILLUSIONTEMPLE_ENEMY_PKLEVEL & 0x0F;
										} else {
											pViewportCreateChange.DirAndPkLevel |= lpTargetObj->m_PK_Level & 0x0F;
										}
									} else {
										pViewportCreateChange.DirAndPkLevel |= lpTargetObj->m_PK_Level & 0x0F;
									}
									//pViewportCreateChange.DirAndPkLevel |= lpTargetObj->m_PK_Level & 0x0F;
									
									int SkillStateCount = ViewPortChangeSkillStates(lpObj,&pViewportCreateChange);
									pViewportCreateChange.SkillStateCount = SkillStateCount;

									//if ( IT_MAP_RANGE(lpTargetObj->MapNumber) == TRUE )	//Illusion Temple Character View
									//{
										memcpy(pViewportCreateChange.CharSet,lpTargetObj->CharSet,sizeof(pViewportCreateChange.CharSet));
									//} else {
									//	memset(pViewportCreateChange.CharSet, 0xFF, sizeof(pViewportCreateChange.CharSet));
									//}

									memcpy(pViewportCreateChange.Id,lpTargetObj->Name,sizeof(pViewportCreateChange.Id));
									memcpy(&SendGBufChange[lOfsChange],&pViewportCreateChange,sizeof(pViewportCreateChange));
									
									lOfsChange += sizeof(pViewportCreateChange) - sizeof(pViewportCreateChange.ViewChangeSkillState) + pViewportCreateChange.SkillStateCount;
									//lOfsChange += sizeof(pViewportCreateChange);
									ChangeCount +=1;
								}
								else
								{
									pViewportCreate.NumberH = SET_NUMBERH(tObjNum);
									pViewportCreate.NumberL = SET_NUMBERL(tObjNum);
			
									//lpTargetObj->CharSet[0] &= 0xF0;
			
									if(lpTargetObj->m_State == 1 && lpTargetObj->Teleport == 0)
									{
										pViewportCreate.NumberH |= 0x80;
									}

									int SkillStateCount = ViewPortSkillStates(lpTargetObj,&pViewportCreate);
									pViewportCreate.SkillStateCount = SkillStateCount ;
									//pViewportCreate.ViewSkillState = lpTargetObj->m_ViewSkillState;
									//lpTargetObj->CharSet[0] |= lpTargetObj->m_ViewState & 0x0F;
				
									pViewportCreate.X = lpTargetObj->X;
									pViewportCreate.Y = lpTargetObj->Y;
									pViewportCreate.TX = lpTargetObj->TX;
									pViewportCreate.TY = lpTargetObj->TY;
			
									pViewportCreate.DirAndPkLevel = lpTargetObj->Dir << 4;
									if ( IT_MAP_RANGE(lpTargetObj->MapNumber) == TRUE && lpTargetObj->Type == OBJ_USER)	//No CTRL attack in Illusion Fix
									{
										if (lpObj->m_Change != lpTargetObj->m_Change)
										{
											pViewportCreate.DirAndPkLevel |= ILLUSIONTEMPLE_ENEMY_PKLEVEL & 0x0F;
										} else {
											pViewportCreate.DirAndPkLevel |= lpTargetObj->m_PK_Level & 0x0F;
										}
									} else {
										pViewportCreate.DirAndPkLevel |= lpTargetObj->m_PK_Level & 0x0F;
									}
									//pViewportCreate.DirAndPkLevel |= lpTargetObj->m_PK_Level & 0x0F;

									memcpy(pViewportCreate.CharSet,lpTargetObj->CharSet,sizeof(pViewportCreate.CharSet));
									memcpy(pViewportCreate.Id,lpTargetObj->Name,sizeof(pViewportCreate.Id));
									memcpy(&sendBuf[lOfs],&pViewportCreate,sizeof(pViewportCreate));
			
									lOfs += sizeof(pViewportCreate) - sizeof(pViewportCreate.ViewSkillState) + pViewportCreate.SkillStateCount;
									//lOfs += sizeof(pViewportCreate);
									count += 1;
								}
							}
		
							if(lpObj->Type == OBJ_USER)
							{
								if(lpTargetObj->lpGuild != 0)
								{
									PMSG_SIMPLE_GUILDVIEWPORT pGuild;
		
									pGuild.GuildNumber = lpTargetObj->lpGuild->Number;
									pGuild.NumberH = SET_NUMBERH(lpTargetObj->m_Index) & 0x7F;
									pGuild.NumberL = SET_NUMBERL(lpTargetObj->m_Index);
		
									pGuild.btGuildStatus = lpTargetObj->GuildStatus;
									pGuild.btGuildType = lpTargetObj->lpGuild->btGuildType;
		
									if(lpObj->lpGuild != 0)
									{
										pGuild.btGuildRelationShip = gObjGetRelationShip(lpObj,lpTargetObj);
									}
									else
									{
										pGuild.btGuildRelationShip = 0;
									}
		
									if(strcmp(lpTargetObj->lpGuild->Names[0],lpTargetObj->Name)==0)
									{
										pGuild.NumberH |= 0x80;
									}
		
									memcpy(&GuildInfoBuf[GuildInfoOfs],&pGuild,sizeof(pGuild));
									GuildInfoOfs += sizeof(pGuild);
									GuildInfoCount += 1;
								}

								if(Gens.Enabled == 1)
								{
								#if (PACK_EDITION>=3)
									if(lpObj->IsBot == 0)
									{
								#endif
										pViewportCreateGens.Family = lpTargetObj->GensFam;
										pViewportCreateGens.NumberH = SET_NUMBERH(lpTargetObj->m_Index);
										pViewportCreateGens.NumberL = SET_NUMBERL(lpTargetObj->m_Index);
										pViewportCreateGens.Level = lpTargetObj->GensLevel;
										pViewportCreateGens.Unk1[1] = 0xA8;
										pViewportCreateGens.Unk1[2] = 0x08;
										memcpy(&sendBuf2[GensInfoOfs],&pViewportCreateGens,sizeof(pViewportCreateGens));
										GensInfoOfs += sizeof(pViewportCreateGens);
										count2 += 1;
								#if (PACK_EDITION>=3)
									}
								#endif
								}
								if(false)
								{

									PMSG_GUILDVIEWPORT_USER pGuildUserViewport;

									if(ViewGuildMng.Add(lpTargetObj->GuildNumber,lpTargetObj->m_Index)==1 && lpTargetObj->lpGuild != 0)
									{
										PMSG_GUILDVIEWPORT pGuildViewport;
		
										pGuildViewport.NumberH = SET_NUMBERH(lpTargetObj->lpGuild->Number);
										pGuildViewport.NumberL = SET_NUMBERL(lpTargetObj->lpGuild->Number);
		
										strcpy(pGuildViewport.GuildName,lpTargetObj->lpGuild->Name);
										memcpy(pGuildViewport.Mark,lpTargetObj->lpGuild->Mark,sizeof(pGuildViewport.Mark));
										memcpy(&GuildInfoBuf[GuildInfoOfs],&pGuildViewport,sizeof(pGuildViewport));
		
										GuildInfoOfs += sizeof(pGuildViewport);
										GuildInfoCount += 1;
									}
		
									pGuildUserViewport.NumberH = SET_NUMBERH(lpTargetObj->m_Index) & 0x7F;
									pGuildUserViewport.NumberL = SET_NUMBERL(lpTargetObj->m_Index);
		
									if(lpTargetObj->lpGuild != 0 && strcmp(lpTargetObj->lpGuild->Names[0],lpTargetObj->Name)==0)
									{
										pGuildUserViewport.NumberH |= 0x80;
									}
		
									if(lpTargetObj->lpGuild != 0)
									{
										pGuildUserViewport.GNumberH = SET_NUMBERH(lpTargetObj->lpGuild->Number);
										pGuildUserViewport.GNumberL = SET_NUMBERL(lpTargetObj->lpGuild->Number);
		
										memcpy(&GuildUserBuf[GuildUserOfs],&pGuildUserViewport,sizeof(pGuildUserViewport));
		
										GuildUserOfs += sizeof(pGuildUserViewport);
										GuildUserCount += 1;
									}
								}
							}
							break;
						case 2:
						case 3:
							if(lpObj->Type == OBJ_USER)
							{
								lpTargetObj = &gObj[tObjNum];

#if (PACK_EDITION>=3)
								if(lpTargetObj->IsBot >= 1)
								{									
									pViewportCreate.NumberH = SET_NUMBERH(tObjNum);
									pViewportCreate.NumberL = SET_NUMBERL(tObjNum);
			
									//lpTargetObj->CharSet[0] &= 0xF0;
			
									if(lpTargetObj->m_State == 1 && lpTargetObj->Teleport == 0)
									{
										pViewportCreate.NumberH |= 0x80;
									}

									int SkillStateCount = ViewPortSkillStates(lpTargetObj,&pViewportCreate);
									pViewportCreate.SkillStateCount = SkillStateCount ;
									//pViewportCreate.ViewSkillState = lpTargetObj->m_ViewSkillState;
									//lpTargetObj->CharSet[0] |= lpTargetObj->m_ViewState & 0x0F;
				
									pViewportCreate.X = lpTargetObj->X;
									pViewportCreate.Y = lpTargetObj->Y;
									pViewportCreate.TX = lpTargetObj->TX;
									pViewportCreate.TY = lpTargetObj->TY;
			
									pViewportCreate.DirAndPkLevel = lpTargetObj->Dir << 4;
									if ( IT_MAP_RANGE(lpTargetObj->MapNumber) == TRUE && lpTargetObj->Type == OBJ_USER)	//No CTRL attack in Illusion Fix
									{
										if (lpObj->m_Change != lpTargetObj->m_Change)
										{
											pViewportCreate.DirAndPkLevel |= ILLUSIONTEMPLE_ENEMY_PKLEVEL & 0x0F;
										} else {
											pViewportCreate.DirAndPkLevel |= lpTargetObj->m_PK_Level & 0x0F;
										}
									} else {
										pViewportCreate.DirAndPkLevel |= lpTargetObj->m_PK_Level & 0x0F;
									}

									memcpy(pViewportCreate.CharSet,lpTargetObj->CharSet,sizeof(pViewportCreate.CharSet));
									memcpy(pViewportCreate.Id,lpTargetObj->Name,sizeof(pViewportCreate.Id));
									memcpy(&sendBuf[lOfs],&pViewportCreate,sizeof(pViewportCreate));
			
									lOfs += sizeof(pViewportCreate) - sizeof(pViewportCreate.ViewSkillState) + pViewportCreate.SkillStateCount;
									//lOfs += sizeof(pViewportCreate);
									count += 1;
								}
								else if(lpTargetObj->m_RecallMon >= 0)
#else
								if(lpTargetObj->m_RecallMon >= 0)
#endif
								{
									pCallMonsterViewportCreate.NumberH = SET_NUMBERH(tObjNum);
									pCallMonsterViewportCreate.NumberL = SET_NUMBERL(tObjNum);
		
									if(lpTargetObj->m_State == 1)
									{
										pCallMonsterViewportCreate.NumberH |= 0x80;
									}
		
									pCallMonsterViewportCreate.Type_HI = SET_NUMBERH(lpTargetObj->Class);
									pCallMonsterViewportCreate.Type_LO = SET_NUMBERL(lpTargetObj->Class);
		
//									pCallMonsterViewportCreate.ViewState = lpTargetObj->m_ViewSkillState;
									pCallMonsterViewportCreate.X = lpTargetObj->X;
									pCallMonsterViewportCreate.Y = lpTargetObj->Y;
									pCallMonsterViewportCreate.TX = lpTargetObj->TX;
									pCallMonsterViewportCreate.TY = lpTargetObj->TY;
									pCallMonsterViewportCreate.Path = lpTargetObj->Dir << 4;
		
									if(lpTargetObj->m_RecallMon >= 0 && lpTargetObj->m_RecallMon < OBJMAX-1)
									{
										memcpy(pCallMonsterViewportCreate.Id,gObj[lpTargetObj->m_RecallMon].Name,sizeof(pCallMonsterViewportCreate.Id));
									}
									else
									{
										memset(pCallMonsterViewportCreate.Id,0x00,sizeof(pCallMonsterViewportCreate.Id));
									}
		
									memcpy(&callMonstersendBuf[callMonlOfs],&pCallMonsterViewportCreate,sizeof(pCallMonsterViewportCreate));
									callMonlOfs += sizeof(pCallMonsterViewportCreate);
									callmonstercount += 1;
								}
								else
								{
									pMonsterViewportCreate.NumberH = SET_NUMBERH(tObjNum);
									pMonsterViewportCreate.NumberL = SET_NUMBERL(tObjNum);
		
									if(lpTargetObj->m_State == 1)
									{
										pMonsterViewportCreate.NumberH |= 0x80;
		
										if(lpTargetObj->Teleport != 0)
										{
											pMonsterViewportCreate.NumberH |= 0x40;
										}
									}
		
									pMonsterViewportCreate.Type_HI = SET_NUMBERH(lpTargetObj->Class);
									pMonsterViewportCreate.Type_LO = SET_NUMBERL(lpTargetObj->Class);
		
									#if (GS_CASTLE==1)
									if( lpTargetObj->Class == 278 )	//LifeStone
									{
										if( g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE )
										{
											if( lpTargetObj->m_btCsJoinSide == lpObj->m_btCsJoinSide )
											{
												pItemViewportCreate.NumberH |= 0x80;
											}
										}

										if( lpTargetObj->lpGuild && lpObj->lpGuild )
										{
											if( lpTargetObj->lpGuild == lpObj->lpGuild )
											{
												pItemViewportCreate.NumberH |= 0x80;
											}
										}

										pItemViewportCreate.NumberH |= (lpTargetObj->m_btCreationState << 4) & 0x70;
									}
									if( lpTargetObj->Class == 285 )	//Guardian
									{
										if( g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE )
										{
											if( lpTargetObj->m_btCsJoinSide == lpObj->m_btCsJoinSide )
											{
												pItemViewportCreate.NumberH |= 0x80;
											}
										}

										if( lpTargetObj->lpGuild && lpObj->lpGuild )
										{
											if( lpTargetObj->lpGuild == lpObj->lpGuild )
											{
												pItemViewportCreate.NumberH |= 0x80;
											}
										}

										pItemViewportCreate.NumberH |= (lpTargetObj->m_btCreationState << 4) & 0x70;
									}
									#endif

									int SkillStateCount = ViewPortMonsterSkillStates(lpTargetObj,&pMonsterViewportCreate);
									pMonsterViewportCreate.SkillStateCount = SkillStateCount;
									
									pMonsterViewportCreate.X = lpTargetObj->X;
									pMonsterViewportCreate.Y = lpTargetObj->Y;
									pMonsterViewportCreate.TX = lpTargetObj->TX;
									pMonsterViewportCreate.TY = lpTargetObj->TY;
									pMonsterViewportCreate.Path = lpTargetObj->Dir << 4;

									memcpy(&MonstersendBuf[MonlOfs],&pMonsterViewportCreate,sizeof(pMonsterViewportCreate));
									MonlOfs += sizeof(pMonsterViewportCreate) - sizeof(pMonsterViewportCreate.ViewSkillState) + pMonsterViewportCreate.SkillStateCount;
									monstercount += 1;
								}
							}
							break;
						case 5:
							if(lpObj->Type == OBJ_USER)
							{
								pItemViewportCreate.NumberH = SET_NUMBERH(tObjNum);
								pItemViewportCreate.NumberL = SET_NUMBERL(tObjNum);
		
								if(MapC[lpObj->MapNumber].m_cItem[tObjNum].m_State == 1)
								{
									pItemViewportCreate.NumberH |= 0x80;
								}
		
								pItemViewportCreate.px = MapC[lpObj->MapNumber].m_cItem[tObjNum].px;
								pItemViewportCreate.py = MapC[lpObj->MapNumber].m_cItem[tObjNum].py;
		
								if(MapC[lpObj->MapNumber].m_cItem[tObjNum].m_Type == ITEMGET(14,15))
								{
									WORD MoneyHW = SET_NUMBERHW(MapC[lpObj->MapNumber].m_cItem[tObjNum].m_BuyMoney);
									WORD MoneyLW = SET_NUMBERLW(MapC[lpObj->MapNumber].m_cItem[tObjNum].m_BuyMoney);
		
									pItemViewportCreate.ItemInfo[0] = BYTE(MapC[lpObj->MapNumber].m_cItem[tObjNum].m_Type)%255;
									pItemViewportCreate.ItemInfo[1] = SET_NUMBERL(MoneyHW);
									pItemViewportCreate.ItemInfo[2] = SET_NUMBERH(MoneyLW);
									pItemViewportCreate.ItemInfo[4] = SET_NUMBERL(MoneyLW);
									pItemViewportCreate.ItemInfo[3] = 0;
									pItemViewportCreate.ItemInfo[5] = (MapC[lpObj->MapNumber].m_cItem[tObjNum].m_Type & 0x1E00) >> 5;
									pItemViewportCreate.ItemInfo[6] = 0;
		
									memcpy(&ItemBuf[lOfs_Item],&pItemViewportCreate,sizeof(pItemViewportCreate));
									lOfs_Item += ItemStructSize;
								}
								else
								{
									ItemByteConvert(pItemViewportCreate.ItemInfo, (MapC[lpObj->MapNumber].m_cItem[tObjNum]));
									memcpy(&ItemBuf[lOfs_Item],&pItemViewportCreate,sizeof(pItemViewportCreate));
									lOfs_Item += ItemStructSize;
								}

								count_Item += 1;
							}
							break;
						}
					}
					lpObj->VpPlayer[n].state = 2;
				}

				if(count > 0)
				{
					if(count == spliterVar || n == (MAX_VIEWPORT-1))
					{
						PWMSG_COUNT pCount;
			
						pCount.h.c = 0xC2;
						pCount.h.headcode = 0x12;
						pCount.h.sizeH = SET_NUMBERH(lOfs);
						pCount.h.sizeL = SET_NUMBERL(lOfs);
						pCount.count = count;
			
						memcpy(sendBuf,&pCount,sizeof(pCount));
						DataSend(aIndex,(unsigned char *)&sendBuf,lOfs);
						count = 0;
						lOfs = 5;
					}
				}
		//{
				if(count2 > 0)
				{
					if(count2 == spliterVar || n == (MAX_VIEWPORT-1))
					{
						PWMSG_SUBCODE_COUNT pCount2;
			
						pCount2.h.c = 0xC2;
						pCount2.h.headcode = 0xF8;
						pCount2.h.sizeH = SET_NUMBERH(GensInfoOfs);
						pCount2.h.sizeL = SET_NUMBERL(GensInfoOfs);
						pCount2.count = count2;
						pCount2.subcode = 5;
			
						memcpy(sendBuf2,&pCount2,sizeof(pCount2));
						DataSend(aIndex,(unsigned char *)&sendBuf2,GensInfoOfs);
						count2 = 0;
						GensInfoOfs = 6;
					}
				}
		
		
				if(ChangeCount > 0)
				{
					if(ChangeCount == spliterVar || n == (MAX_VIEWPORT-1))
					{
						PWMSG_COUNT pCount;
			
						pCount.h.c = 0xC2;
						pCount.h.headcode = 0x45;
						pCount.h.sizeH = SET_NUMBERH(lOfsChange);
						pCount.h.sizeL = SET_NUMBERL(lOfsChange);
						pCount.count = ChangeCount;
			
						memcpy(SendGBufChange,&pCount,sizeof(pCount));
						DataSend(aIndex,(unsigned char *)&SendGBufChange,lOfsChange);
						ChangeCount = 0;
						lOfsChange = 5;
					}
				}
		
				if(monstercount > 0)
				{
					if(monstercount == spliterVar || n == (MAX_VIEWPORT-1))
					{
						PWMSG_COUNT pCount;
			
						pCount.h.c = 0xC2;
						pCount.h.headcode = 0x13;
						pCount.h.sizeH = SET_NUMBERH(MonlOfs);
						pCount.h.sizeL = SET_NUMBERL(MonlOfs);
						pCount.count = monstercount;
			
						memcpy(MonstersendBuf,&pCount,sizeof(pCount));
						DataSend(aIndex,(unsigned char *)&MonstersendBuf,MonlOfs);

						monstercount = 0;
						MonlOfs = 5;
					}
				}	
		
				if(callmonstercount > 0)
				{
					if(callmonstercount == spliterVar || n == (MAX_VIEWPORT-1))
					{
						PWMSG_COUNT pCount;
			
						pCount.h.c = 0xC2;
						pCount.h.headcode = 0x1F;
						pCount.h.sizeH = SET_NUMBERH(callMonlOfs);
						pCount.h.sizeL = SET_NUMBERL(callMonlOfs);
						pCount.count = callmonstercount;
						
						memcpy(callMonstersendBuf,&pCount,sizeof(pCount));
						DataSend(aIndex,(unsigned char *)&callMonstersendBuf,callMonlOfs);
						callmonstercount = 0;
						callMonlOfs = 5;
					}
				}
		
				if(count_Item > 0)
				{
					if(count_Item == spliterVar || n == (MAX_VIEWPORT-1))
					{
						PWMSG_COUNT pCount;
			
						pCount.h.c = 0xC2;
						pCount.h.headcode = 0x20;
						pCount.count = count_Item;
						pCount.h.sizeH = SET_NUMBERH(lOfs_Item);
						pCount.h.sizeL = SET_NUMBERL(lOfs_Item);
						
						memcpy(ItemBuf,&pCount,sizeof(pCount));
						DataSend(aIndex,(unsigned char *)&ItemBuf,lOfs_Item);
						count_Item = 0;
						lOfs_Item = 5;
					}
				}
		
				if(GuildInfoCount != 0)
				{
					if(GuildInfoCount == spliterVar || n == (MAX_VIEWPORT-1))
					{
						PWMSG_COUNT pCount;
			
						pCount.h.headcode = 0x65;
						pCount.h.c = 0xC2;
						pCount.h.sizeH = SET_NUMBERH(GuildInfoOfs);
						pCount.h.sizeL = SET_NUMBERL(GuildInfoOfs);
						pCount.count = GuildInfoCount;
						
						memcpy(GuildInfoBuf,&pCount,sizeof(pCount));
						DataSend(aIndex,(unsigned char *)&GuildInfoBuf,GuildInfoOfs);
						GuildInfoCount = 0;
						GuildInfoOfs = 5;
					}
				}
			}
		}


		/*if(lpObj->Type == OBJ_USER)
		{
			while(SplitCount < MAX_VIEWPORT)
			{
				count = 0;
				monstercount = 0;
				count_Item = 0;
				callmonstercount = 0;
				lOfs = 5;
				MonlOfs = lOfs;
				callMonlOfs = MonlOfs;
				lOfs_Item = 5;
				pMsgSize = 44;

				for(n = SplitCount; n < (SplitCount+Spliter);n++)
				{
					if(lpObj->VpPlayer[n].state == 1)
					{
						tObjNum = lpObj->VpPlayer[n].number;
			
						if(tObjNum >= 0)
						{
							switch(lpObj->VpPlayer[n].type)
							{
							case 1:
								lpTargetObj = &gObj[tObjNum];
			
								if(lpTargetObj->m_Change >= 0)
								{
									pViewportCreateChange.NumberH = SET_NUMBERH(tObjNum);
									pViewportCreateChange.NumberL = SET_NUMBERL(tObjNum);
			
								//	lpTargetObj->CharSet[0] &= 0xF0;
		
									if(lpTargetObj->m_State == 1 && lpTargetObj->Teleport == 0)
									{
										pViewportCreateChange.NumberH |= 0x80;
									}
									pViewportCreateChange.X = lpTargetObj->X;
									pViewportCreateChange.Y = lpTargetObj->Y;
									pViewportCreateChange.TX = lpTargetObj->TX;
									pViewportCreateChange.TY = lpTargetObj->TY;
									pViewportCreateChange.SkinH = SET_NUMBERH((lpTargetObj->m_Change & 0xFFFF) & 0xFFFF);
									pViewportCreateChange.SkinL = SET_NUMBERL((lpTargetObj->m_Change & 0xFFFF) & 0xFFFF);
									//pViewportCreateChange.ViewSkillState = lpTargetObj->m_ViewSkillState;
									pViewportCreateChange.DirAndPkLevel = lpTargetObj->Dir << 4;
									pViewportCreateChange.DirAndPkLevel |= lpTargetObj->m_PK_Level & 0x0F;
									
									int SkillStateCount = ViewPortChangeSkillStates(lpObj,&pViewportCreateChange);
									pViewportCreateChange.SkillStateCount = SkillStateCount;

									memcpy(pViewportCreateChange.Id,lpTargetObj->Name,sizeof(pViewportCreateChange.Id));
									memcpy(&SendGBufChange[lOfsChange],&pViewportCreateChange,sizeof(pViewportCreateChange));
									
									lOfsChange += sizeof(pViewportCreateChange) - sizeof(pViewportCreateChange.ViewChangeSkillState) + pViewportCreateChange.SkillStateCount;
									//lOfsChange += sizeof(pViewportCreateChange);
									ChangeCount +=1;
								}
								else
								{
									pViewportCreate.NumberH = SET_NUMBERH(tObjNum);
									pViewportCreate.NumberL = SET_NUMBERL(tObjNum);
			
									//lpTargetObj->CharSet[0] &= 0xF0;
			
									if(lpTargetObj->m_State == 1 && lpTargetObj->Teleport == 0)
									{
										pViewportCreate.NumberH |= 0x80;
									}

									int SkillStateCount = ViewPortSkillStates(lpTargetObj,&pViewportCreate);
									pViewportCreate.SkillStateCount = SkillStateCount ;
									//pViewportCreate.ViewSkillState = lpTargetObj->m_ViewSkillState;
									//lpTargetObj->CharSet[0] |= lpTargetObj->m_ViewState & 0x0F;
				
									pViewportCreate.X = lpTargetObj->X;
									pViewportCreate.Y = lpTargetObj->Y;
									pViewportCreate.TX = lpTargetObj->TX;
									pViewportCreate.TY = lpTargetObj->TY;
			
									pViewportCreate.DirAndPkLevel = lpTargetObj->Dir << 4;
									pViewportCreate.DirAndPkLevel |= lpTargetObj->m_PK_Level & 0x0F;

									memcpy(pViewportCreate.CharSet,lpTargetObj->CharSet,sizeof(pViewportCreate.CharSet));
									memcpy(pViewportCreate.Id,lpTargetObj->Name,sizeof(pViewportCreate.Id));
									memcpy(&sendBuf[lOfs],&pViewportCreate,sizeof(pViewportCreate));
			
									lOfs += sizeof(pViewportCreate) - sizeof(pViewportCreate.ViewSkillState) + pViewportCreate.SkillStateCount;
									//lOfs += sizeof(pViewportCreate);
									count += 1;
								}
			
								if(lpObj->Type == OBJ_USER)
								{
									if(lpTargetObj->lpGuild != 0)
									{
										PMSG_SIMPLE_GUILDVIEWPORT pGuild;
			
										pGuild.GuildNumber = lpTargetObj->lpGuild->Number;
										pGuild.NumberH = SET_NUMBERH(lpTargetObj->m_Index) & 0x7F;
										pGuild.NumberL = SET_NUMBERL(lpTargetObj->m_Index);
			
										pGuild.btGuildStatus = lpTargetObj->GuildStatus;
										pGuild.btGuildType = lpTargetObj->lpGuild->btGuildType;
			
										if(lpObj->lpGuild != 0)
										{
											pGuild.btGuildRelationShip = gObjGetRelationShip(lpObj,lpTargetObj);
										}
										else
										{
											pGuild.btGuildRelationShip = 0;
										}
			
										if(strcmp(lpTargetObj->lpGuild->Names[0],lpTargetObj->Name)==0)
										{
											pGuild.NumberH |= 0x80;
										}
			
										memcpy(&GuildInfoBuf[GuildInfoOfs],&pGuild,sizeof(pGuild));
										GuildInfoOfs += sizeof(pGuild);
										GuildInfoCount += 1;
									}

									if(Gens.Enabled == 1)
									{
									#if (PACK_EDITION>=3)
										if(lpObj->IsBot == 0)
										{
									#endif
											pViewportCreateGens.Family = lpTargetObj->GensFam;
											pViewportCreateGens.NumberH = SET_NUMBERH(lpTargetObj->m_Index);
											pViewportCreateGens.NumberL = SET_NUMBERL(lpTargetObj->m_Index);
											pViewportCreateGens.Level = lpTargetObj->GensLevel;
											pViewportCreateGens.Unk1[1] = 0xA8;
											pViewportCreateGens.Unk1[2] = 0x08;
											memcpy(&sendBuf2[GensInfoOfs],&pViewportCreateGens,sizeof(pViewportCreateGens));
											GensInfoOfs += sizeof(pViewportCreateGens);
											count2 += 1;
									#if (PACK_EDITION>=3)
										}
									#endif
									}
									if(false)
									{

										PMSG_GUILDVIEWPORT_USER pGuildUserViewport;

										if(ViewGuildMng.Add(lpTargetObj->GuildNumber,lpTargetObj->m_Index)==1 && lpTargetObj->lpGuild != 0)
										{
											PMSG_GUILDVIEWPORT pGuildViewport;
			
											pGuildViewport.NumberH = SET_NUMBERH(lpTargetObj->lpGuild->Number);
											pGuildViewport.NumberL = SET_NUMBERL(lpTargetObj->lpGuild->Number);
			
											strcpy(pGuildViewport.GuildName,lpTargetObj->lpGuild->Name);
											memcpy(pGuildViewport.Mark,lpTargetObj->lpGuild->Mark,sizeof(pGuildViewport.Mark));
											memcpy(&GuildInfoBuf[GuildInfoOfs],&pGuildViewport,sizeof(pGuildViewport));
			
											GuildInfoOfs += sizeof(pGuildViewport);
											GuildInfoCount += 1;
										}
			
										pGuildUserViewport.NumberH = SET_NUMBERH(lpTargetObj->m_Index) & 0x7F;
										pGuildUserViewport.NumberL = SET_NUMBERL(lpTargetObj->m_Index);
			
										if(lpTargetObj->lpGuild != 0 && strcmp(lpTargetObj->lpGuild->Names[0],lpTargetObj->Name)==0)
										{
											pGuildUserViewport.NumberH |= 0x80;
										}
			
										if(lpTargetObj->lpGuild != 0)
										{
											pGuildUserViewport.GNumberH = SET_NUMBERH(lpTargetObj->lpGuild->Number);
											pGuildUserViewport.GNumberL = SET_NUMBERL(lpTargetObj->lpGuild->Number);
			
											memcpy(&GuildUserBuf[GuildUserOfs],&pGuildUserViewport,sizeof(pGuildUserViewport));
			
											GuildUserOfs += sizeof(pGuildUserViewport);
											GuildUserCount += 1;
										}
									}
								}
								break;
							case 2:
							case 3:
								if(lpObj->Type == OBJ_USER)
								{
									lpTargetObj = &gObj[tObjNum];

#if (PACK_EDITION>=3)
									if(lpTargetObj->IsBot >= 1)
									{									
										pViewportCreate.NumberH = SET_NUMBERH(tObjNum);
										pViewportCreate.NumberL = SET_NUMBERL(tObjNum);
				
										//lpTargetObj->CharSet[0] &= 0xF0;
				
										if(lpTargetObj->m_State == 1 && lpTargetObj->Teleport == 0)
										{
											pViewportCreate.NumberH |= 0x80;
										}

										int SkillStateCount = ViewPortSkillStates(lpTargetObj,&pViewportCreate);
										pViewportCreate.SkillStateCount = SkillStateCount ;
										//pViewportCreate.ViewSkillState = lpTargetObj->m_ViewSkillState;
										//lpTargetObj->CharSet[0] |= lpTargetObj->m_ViewState & 0x0F;
					
										pViewportCreate.X = lpTargetObj->X;
										pViewportCreate.Y = lpTargetObj->Y;
										pViewportCreate.TX = lpTargetObj->TX;
										pViewportCreate.TY = lpTargetObj->TY;
				
										pViewportCreate.DirAndPkLevel = lpTargetObj->Dir << 4;
										pViewportCreate.DirAndPkLevel |= lpTargetObj->m_PK_Level & 0x0F;

										memcpy(pViewportCreate.CharSet,lpTargetObj->CharSet,sizeof(pViewportCreate.CharSet));
										memcpy(pViewportCreate.Id,lpTargetObj->Name,sizeof(pViewportCreate.Id));
										memcpy(&sendBuf[lOfs],&pViewportCreate,sizeof(pViewportCreate));
				
										lOfs += sizeof(pViewportCreate) - sizeof(pViewportCreate.ViewSkillState) + pViewportCreate.SkillStateCount;
										//lOfs += sizeof(pViewportCreate);
										count += 1;
									}
									else if(lpTargetObj->m_RecallMon >= 0)
#else
									if(lpTargetObj->m_RecallMon >= 0)
#endif
									{
										pCallMonsterViewportCreate.NumberH = SET_NUMBERH(tObjNum);
										pCallMonsterViewportCreate.NumberL = SET_NUMBERL(tObjNum);
			
										if(lpTargetObj->m_State == 1)
										{
											pCallMonsterViewportCreate.NumberH |= 0x80;
										}
			
										pCallMonsterViewportCreate.Type_HI = SET_NUMBERH(lpTargetObj->Class);
										pCallMonsterViewportCreate.Type_LO = SET_NUMBERL(lpTargetObj->Class);
			
	//									pCallMonsterViewportCreate.ViewState = lpTargetObj->m_ViewSkillState;
										pCallMonsterViewportCreate.X = lpTargetObj->X;
										pCallMonsterViewportCreate.Y = lpTargetObj->Y;
										pCallMonsterViewportCreate.TX = lpTargetObj->TX;
										pCallMonsterViewportCreate.TY = lpTargetObj->TY;
										pCallMonsterViewportCreate.Path = lpTargetObj->Dir << 4;
			
										if(lpTargetObj->m_RecallMon >= 0 && lpTargetObj->m_RecallMon < OBJMAX-1)
										{
											memcpy(pCallMonsterViewportCreate.Id,gObj[lpTargetObj->m_RecallMon].Name,sizeof(pCallMonsterViewportCreate.Id));
										}
										else
										{
											memset(pCallMonsterViewportCreate.Id,0x00,sizeof(pCallMonsterViewportCreate.Id));
										}
			
										memcpy(&callMonstersendBuf[callMonlOfs],&pCallMonsterViewportCreate,sizeof(pCallMonsterViewportCreate));
										callMonlOfs += sizeof(pCallMonsterViewportCreate);
										callmonstercount += 1;
									}
									else
									{
										pMonsterViewportCreate.NumberH = SET_NUMBERH(tObjNum);
										pMonsterViewportCreate.NumberL = SET_NUMBERL(tObjNum);
			
										if(lpTargetObj->m_State == 1)
										{
											pMonsterViewportCreate.NumberH |= 0x80;
			
											if(lpTargetObj->Teleport != 0)
											{
												pMonsterViewportCreate.NumberH |= 0x40;
											}
										}
			
										pMonsterViewportCreate.Type_HI = SET_NUMBERH(lpTargetObj->Class);
										pMonsterViewportCreate.Type_LO = SET_NUMBERL(lpTargetObj->Class);
			
										#if (GS_CASTLE==1)
										if( lpTargetObj->Class == 278 )
										{
											if( g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE )
											{
												if( lpTargetObj->m_btCsJoinSide == lpObj->m_btCsJoinSide )
												{
													pItemViewportCreate.NumberH |= 0x80;
												}
											}

											if( lpTargetObj->lpGuild && lpObj->lpGuild )
											{
												if( lpTargetObj->lpGuild == lpObj->lpGuild )
												{
													pItemViewportCreate.NumberH |= 0x80;
												}
											}

											pItemViewportCreate.NumberH |= (lpTargetObj->m_btCreationState << 4) & 0x70;
										}
										#endif

										int SkillStateCount = ViewPortMonsterSkillStates(lpTargetObj,&pMonsterViewportCreate);
										pMonsterViewportCreate.SkillStateCount = SkillStateCount;
										
										pMonsterViewportCreate.X = lpTargetObj->X;
										pMonsterViewportCreate.Y = lpTargetObj->Y;
										pMonsterViewportCreate.TX = lpTargetObj->TX;
										pMonsterViewportCreate.TY = lpTargetObj->TY;
										pMonsterViewportCreate.Path = lpTargetObj->Dir << 4;

										memcpy(&MonstersendBuf[MonlOfs],&pMonsterViewportCreate,sizeof(pMonsterViewportCreate));
										MonlOfs += sizeof(pMonsterViewportCreate) - sizeof(pMonsterViewportCreate.ViewSkillState) + pMonsterViewportCreate.SkillStateCount;
										monstercount += 1;
									}
								}
								break;
							case 5:
								if(lpObj->Type == OBJ_USER)
								{
									pItemViewportCreate.NumberH = SET_NUMBERH(tObjNum);
									pItemViewportCreate.NumberL = SET_NUMBERL(tObjNum);
			
									if(MapC[lpObj->MapNumber].m_cItem[tObjNum].m_State == 1)
									{
										pItemViewportCreate.NumberH |= 0x80;
									}
			
									pItemViewportCreate.px = MapC[lpObj->MapNumber].m_cItem[tObjNum].px;
									pItemViewportCreate.py = MapC[lpObj->MapNumber].m_cItem[tObjNum].py;
			
									if(MapC[lpObj->MapNumber].m_cItem[tObjNum].m_Type == ITEMGET(14,15))
									{
										WORD MoneyHW = SET_NUMBERHW(MapC[lpObj->MapNumber].m_cItem[tObjNum].m_BuyMoney);
										WORD MoneyLW = SET_NUMBERLW(MapC[lpObj->MapNumber].m_cItem[tObjNum].m_BuyMoney);
			
										pItemViewportCreate.ItemInfo[0] = BYTE(MapC[lpObj->MapNumber].m_cItem[tObjNum].m_Type)%255;
										pItemViewportCreate.ItemInfo[1] = SET_NUMBERL(MoneyHW);
										pItemViewportCreate.ItemInfo[2] = SET_NUMBERH(MoneyLW);
										pItemViewportCreate.ItemInfo[4] = SET_NUMBERL(MoneyLW);
										pItemViewportCreate.ItemInfo[3] = 0;
										pItemViewportCreate.ItemInfo[5] = (MapC[lpObj->MapNumber].m_cItem[tObjNum].m_Type & 0x1E00) >> 5;
										pItemViewportCreate.ItemInfo[6] = 0;
			
										memcpy(&ItemBuf[lOfs_Item],&pItemViewportCreate,sizeof(pItemViewportCreate));
										lOfs_Item += ItemStructSize;
									}
									else
									{
										ItemByteConvert(pItemViewportCreate.ItemInfo, (MapC[lpObj->MapNumber].m_cItem[tObjNum]));
										memcpy(&ItemBuf[lOfs_Item],&pItemViewportCreate,sizeof(pItemViewportCreate));
										lOfs_Item += ItemStructSize;
									}

									count_Item += 1;
								}
								break;
							}
						}
						lpObj->VpPlayer[n].state = 2;
					}
				}
		//if(lpObj->Type == OBJ_USER)
		//{
				if(count > 0)
				{
					PWMSG_COUNT pCount;
		
					pCount.h.c = 0xC2;
					pCount.h.headcode = 0x12;
					pCount.h.sizeH = SET_NUMBERH(lOfs);
					pCount.h.sizeL = SET_NUMBERL(lOfs);
					pCount.count = count;
		
					memcpy(sendBuf,&pCount,sizeof(pCount));
					DataSend(aIndex,(unsigned char *)&sendBuf,lOfs);
				}
		//{
				if(count2 > 0)
				{
					PWMSG_SUBCODE_COUNT pCount2;
		
					pCount2.h.c = 0xC2;
					pCount2.h.headcode = 0xF8;
					pCount2.h.sizeH = SET_NUMBERH(GensInfoOfs);
					pCount2.h.sizeL = SET_NUMBERL(GensInfoOfs);
					pCount2.count = count2;
					pCount2.subcode = 5;
		
					memcpy(sendBuf2,&pCount2,sizeof(pCount2));
					DataSend(aIndex,(unsigned char *)&sendBuf2,GensInfoOfs);
				}
		
		
				if(ChangeCount > 0)
				{
					PWMSG_COUNT pCount;
		
					pCount.h.c = 0xC2;
					pCount.h.headcode = 0x45;
					pCount.h.sizeH = SET_NUMBERH(lOfsChange);
					pCount.h.sizeL = SET_NUMBERL(lOfsChange);
					pCount.count = ChangeCount;
		
					memcpy(SendGBufChange,&pCount,sizeof(pCount));
					DataSend(aIndex,(unsigned char *)&SendGBufChange,lOfsChange);
				}
		
				if(monstercount > 0)
				{
					PWMSG_COUNT pCount;
		
					pCount.h.c = 0xC2;
					pCount.h.headcode = 0x13;
					pCount.h.sizeH = SET_NUMBERH(MonlOfs);
					pCount.h.sizeL = SET_NUMBERL(MonlOfs);
					pCount.count = monstercount;
		
					memcpy(MonstersendBuf,&pCount,sizeof(pCount));
					DataSend(aIndex,(unsigned char *)&MonstersendBuf,MonlOfs);
				}	
		
				if(callmonstercount > 0)
				{
					PWMSG_COUNT pCount;
		
					pCount.h.c = 0xC2;
					pCount.h.headcode = 0x1F;
					pCount.h.sizeH = SET_NUMBERH(callMonlOfs);
					pCount.h.sizeL = SET_NUMBERL(callMonlOfs);
					pCount.count = callmonstercount;
					
					memcpy(callMonstersendBuf,&pCount,sizeof(pCount));
					DataSend(aIndex,(unsigned char *)&callMonstersendBuf,callMonlOfs);
				}
		
				if(count_Item > 0)
				{
					PWMSG_COUNT pCount;
		
					pCount.h.c = 0xC2;
					pCount.h.headcode = 0x20;
					pCount.count = count_Item;
					pCount.h.sizeH = SET_NUMBERH(lOfs_Item);
					pCount.h.sizeL = SET_NUMBERL(lOfs_Item);
					
					memcpy(ItemBuf,&pCount,sizeof(pCount));
					DataSend(aIndex,(unsigned char *)&ItemBuf,lOfs_Item);
				}
		
				if(GuildInfoCount != 0)
				{
					PWMSG_COUNT pCount;
		
					pCount.h.headcode = 0x65;
					pCount.h.c = 0xC2;
					pCount.h.sizeH = SET_NUMBERH(GuildInfoOfs);
					pCount.h.sizeL = SET_NUMBERL(GuildInfoOfs);
					pCount.count = GuildInfoCount;
					
					memcpy(GuildInfoBuf,&pCount,sizeof(pCount));
					DataSend(aIndex,(unsigned char *)&GuildInfoBuf,GuildInfoOfs);
				}
				
				SplitCount +=Spliter;
			}
		}*/
	}
}






void gObjSkillUseProc(LPOBJ lpObj)
{
	if(lpObj->m_SkillAttackTime > 0)
	{
		lpObj->m_SkillAttackTime--;

		if(lpObj->m_SkillAttackTime == 0)
		{
			lpObj->m_SkillAttack = 0;
			lpObj->m_ViewSkillState &= ~4;

			GCMagicCancelSend(lpObj,28);
		}
	}

	if(lpObj->m_SkillDragonSlayerTime > 0)
	{
		lpObj->m_SkillDragonSlayerTime--;

		if(lpObj->m_SkillDragonSlayerTime == 0)
		{		
			lpObj->m_SkillDragonSlayerDmg = 0;
		}
	}

	if(lpObj->m_SkillRedArmorIgnoreTime > 0)
	{
		lpObj->m_SkillRedArmorIgnoreTime--;

		if(lpObj->m_SkillRedArmorIgnoreTime == 0)
		{		
			lpObj->m_SkillRedArmorIgnoreNum = 0;
			GCSkillInfoSend(lpObj,0,129);
		}
	}

	if(lpObj->m_SkillFitnessTime > 0)
	{
		lpObj->m_SkillFitnessTime--;

		if(lpObj->m_SkillFitnessTime == 0)
		{
			lpObj->AddVitality = lpObj->AddVitality - lpObj->m_SkillFitnessVal;
			lpObj->m_SkillFitnessVal = 0;
			GCSkillInfoSend(lpObj,0,130);
			gObjCalCharacter(lpObj->m_Index);		//Fitness Skill Recalculate
		}
	}

	if(lpObj->m_SkillDefSuccessRateIncTime > 0)
	{
		lpObj->m_SkillDefSuccessRateIncTime--;

		if(lpObj->m_SkillDefSuccessRateIncTime == 0)
		{		
			lpObj->m_SkillDefSuccessRateIncNum = 0;
			GCSkillInfoSend(lpObj,0,131);
		}
	}

	if(lpObj->m_SkillBerserkerTime > 0)
	{
		lpObj->m_SkillBerserkerTime--;

		if(lpObj->m_SkillBerserkerTime == 0)
		{		
			lpObj->m_SkillBerserkerDef = 0;
			lpObj->m_SkillBerserkerAttack = 0;

			//lpObj->MaxLife = lpObj->MaxLife / 0.85;
			//lpObj->MaxMana = lpObj->MaxMana / 1.15;

			GCSkillInfoSend(lpObj,0,0x51);
			gObjCalCharacter(lpObj->m_Index);		//Berserker Skill Recalculate
		}
	}

	if(lpObj->m_SkillReflectTime > 0)				
	{
		lpObj->m_SkillReflectTime--;

		if(lpObj->m_SkillReflectTime == 0)
		{			
			lpObj->DamageReflect -= lpObj->m_SkillReflect;
			if(lpObj->DamageReflect < 0)
			{
				lpObj->DamageReflect = 0;
			}
			lpObj->m_SkillReflect = 0;
			GCSkillInfoSend(lpObj,0,0x47);
			gObjCalCharacter(lpObj->m_Index);		//Reflect Skill Recalculate
		}
	}

	if(lpObj->m_SkillMagicCircleTime > 0)
	{
		lpObj->m_SkillMagicCircleTime--;

		if(lpObj->m_SkillMagicCircleTime == 0)
		{
			lpObj->m_MagicDamageMin -= lpObj->m_SkillMagicCircle;

			if(lpObj->m_MagicDamageMin < 0)
				lpObj->m_MagicDamageMin = 0;

			lpObj->m_SkillMagicCircle = 0;
			lpObj->m_SkillMagicCircleCriticalDamage = 0;
			GCSkillInfoSend(lpObj,0,0x52);			//MagicCircle Skill Recalculate
		}
	}

	if(lpObj->m_SkillReduceDamageTime > 0)
	{
		lpObj->m_SkillReduceDamageTime--;

		if(lpObj->m_SkillReduceDamageTime == 0)
		{
			lpObj->m_SkillReduceDamage = 0;
			GCSkillInfoSend(lpObj,0,0x4C);
		}
	}

	if(lpObj->m_iSkillNPCSantaTime > 0)
	{
		lpObj->m_iSkillNPCSantaTime--;

		if(lpObj->m_iSkillNPCSantaTime == 0)
		{
			GCSantaStateInfoSend(lpObj,0);
			lpObj->m_iSkillNPCSantaType = 0;
			gObjCalCharacter(lpObj->m_Index);		//Recalculate Santas Buffs
		}
	}

	if(lpObj->m_SkillReduceDefenseTime > 0)
	{
		lpObj->m_SkillReduceDefenseTime--;

		if(lpObj->m_SkillReduceDefenseTime == 0)
		{
			lpObj->m_SkillReduceDefense = 0;
			GCSkillInfoSend(lpObj,0,0x4D);
		}
	}	

	if(lpObj->m_RFBufReduceDefenseTime > 0)
	{
		lpObj->m_RFBufReduceDefenseTime--;

		if(lpObj->m_RFBufReduceDefenseTime == 0)
		{
			lpObj->m_RFBufReduceDefense = 0;
			GCSkillInfoSend(lpObj,0,164);
		}
	}

	if(lpObj->m_SkillNightTime > 0)
	{
		lpObj->m_SkillNightTime--;

		if(lpObj->m_SkillNightTime == 0)
		{
			//lpObj->m_Defense += lpObj->m_SkillNight;
			gObjUseSkill.RemoveSkillNight(lpObj->m_Index);
		}
	}

	if(lpObj->m_SkillGladiatorsGloryTime > 0)
	{
		lpObj->m_SkillGladiatorsGloryTime--;

		if(lpObj->m_SkillGladiatorsGloryTime == 0)
		{
			gObjUseSkill.GladiatorsGlory(eVS_DUEL_MEDAL, lpObj->m_Index, FALSE);
			gObjCalCharacter(lpObj->m_Index);		//Recalculate Gladiators Glory
		}
	}
	
	if(lpObj->m_SkillSleep_Time > 0)
	{
		lpObj->m_SkillSleep_Time--;

		if(lpObj->Life <= 0.0f)
			lpObj->m_SkillSleep_Time = 0;

		if(lpObj->m_SkillSleep_Time == 0)
		{
			gObjUseSkill.SkillSleepRemove(lpObj);
		}
	}	

	if(lpObj->m_SkillExplotionTime > 0)				//Skill Does Nothing
	{
		lpObj->m_SkillExplotionTime--;

		if(lpObj->Life <= 0.0f)
			lpObj->m_SkillExplotionTime = 0;

		if(lpObj->m_SkillExplotionTime == 0)
		{
			GCSkillInfoSend(lpObj,0,0x56);
		}
	}

	if(lpObj->m_SkillIT_Time > 0)	//Skill Does Nothing
	{
		lpObj->m_SkillIT_Time--;

		if(lpObj->m_SkillIT_Time == 0)
		{			
			switch(lpObj->m_SkillIT_Number)
			{
				case IL_ORDER_OF_PROTECT:
				{
					GCSkillInfoSend(lpObj,0,34);
				}break;
				case IL_ORDER_OF_FREEZE:
				{
					GCSkillInfoSend(lpObj,0,65);
				}break;
			}

			IllusionTemple.SkillEffectEnd(lpObj->m_Index, lpObj->m_SkillIT_Number);

			lpObj->m_SkillIT_Time = 0;
			lpObj->m_SkillIT_Number = 0;
		}
	}
			
	if(lpObj->m_SkillAttackTime2 > 0)
	{
		lpObj->m_SkillAttackTime2--;

		if(lpObj->m_SkillAttackTime2 <= 0)
		{
			lpObj->m_SkillAttack2 = 0;
			lpObj->m_SkillAttackTime2 = 0;
		}
	}

	if(lpObj->m_SkillDefenseTime > 0)
	{
		lpObj->m_SkillDefenseTime--;

		if(lpObj->m_SkillDefenseTime == 0)
		{
			lpObj->m_SkillDefense = 0;
			lpObj->m_ViewSkillState &= ~8;

			GCMagicCancelSend(lpObj,27);
		}
	}

	if(lpObj->m_WizardSkillDefenseTime > 0)
	{
		lpObj->m_WizardSkillDefenseTime--;

		if(lpObj->m_WizardSkillDefenseTime == 0)
		{
			lpObj->m_WizardSkillDefense = 0;
			lpObj->m_ViewSkillState &= ~256;

			GCMagicCancelSend(lpObj,16);
		}
	}

	if(lpObj->m_SkillAddLifeTime > 0)
	{
		lpObj->m_SkillAddLifeTime--;

		if(lpObj->m_SkillAddLifeTime == 0)
		{
			lpObj->m_SkillAddLife = 0;
			lpObj->m_SkillAddLife_Mana = 0;
			lpObj->m_ViewSkillState &= ~16;
			GCMagicCancelSend(lpObj,48);
			gObjCalCharacter(lpObj->m_Index);		//Recalculate Add Life
		}
	}

	if(lpObj->m_SkillHardenTime > 0)
	{
		lpObj->m_SkillHardenTime--;

		if(lpObj->m_SkillHardenTime <= 0)
		{
			lpObj->m_SkillHarden = 0;
			lpObj->m_ViewSkillState &= ~32;

			GCMagicCancelSend(lpObj,51);
		}
	}

	if(lpObj->m_SkillMagumReduceDefenseTime > 0)
	{
		lpObj->m_SkillMagumReduceDefenseTime--;

		if(lpObj->m_SkillMagumReduceDefenseTime == 0)
		{
			lpObj->m_SkillMagumReduceDefense = 0;
			lpObj->m_ViewSkillState &= ~64;

			GCMagicCancelSend(lpObj,55);
		}
	}

	if(lpObj->Class == 77)
	{
		lpObj->m_SkyBossMonSheildTime++;

		if(lpObj->m_SkyBossMonSheildTime > 5)
		{
			lpObj->m_SkyBossMonSheildTime = 0;

			if(lpObj->m_SkyBossMonSheild != 0)
			{
				lpObj->m_SkyBossMonSheild = 0;
				lpObj->m_ViewSkillState &= ~128;

				GCMagicCancelSend(lpObj,16);
			}
			else
			{
				lpObj->m_SkyBossMonSheild = 1;
				lpObj->m_ViewSkillState |= 128;

				GCMagicAttackNumberSend(lpObj,16,lpObj->m_Index,1);
			}
		}
	}

	if(lpObj->m_iSkillNPCHelpTime > 0)
	{
		if(GetTickCount() > lpObj->m_iSkillNPCHelpTime + 1800000)
		{
			lpObj->m_iSkillNPCHelpTime = 0;
			lpObj->m_iSkillNPCDefense = 0;
			lpObj->m_iSkillNPCAttack = 0;

			lpObj->m_ViewSkillState &= ~131072;

			GCSkillInfoSend(lpObj,0,0x03);
		}
	}

	if(lpObj->SkillRecallParty_Time != 0 && lpObj->Type == OBJ_USER)
	{
		lpObj->SkillRecallParty_Time--;

		if(lpObj->SkillRecallParty_Time <= 0)
		{
			lpObj->SkillRecallParty_Time = 0;

			int m_check1 = gMoveCommand.CheckMainToMove(lpObj);
			int m_check2 = gMoveCommand.CheckEquipmentToMove(lpObj,lpObj->SkillRecallParty_MapNumber);
			int m_check3 = gMoveCommand.CheckInterfaceToMove(lpObj);

			if(m_check1 == false
				|| m_check2 == false
				|| m_check3 == false)
			{
				char szMsg[256];
				wsprintf(szMsg,lMsg.Get(1254));

				GCServerMsgStringSend(szMsg,lpObj->m_Index,1);
			}
			else
			{
#if (PACK_EDITION>=3)
				if(lpObj->RaceCheck == 1)
				{
					BotRacer.ResetTime(lpObj->m_Index);
					GCServerMsgStringSend(lMsg.Get(MSGGET(14, 158)) ,lpObj->m_Index, 0x00);
				}
#endif
				gObjRecall(lpObj->m_Index,lpObj->SkillRecallParty_MapNumber,lpObj->SkillRecallParty_X,lpObj->SkillRecallParty_Y);
			}
		}
		else if(lpObj->SkillRecallParty_Time <= 5)
		{
			char szMsg[256];
			wsprintf(szMsg,lMsg.Get(1243),lpObj->SkillRecallParty_Time);

			GCServerMsgStringSend(szMsg,lpObj->m_Index,1);
		}
	}

	if(lpObj->SkillAddCriticalDamageTime != 0 && lpObj->Type == OBJ_USER)
	{
		lpObj->SkillAddCriticalDamageTime--;

		if(lpObj->SkillAddCriticalDamageTime <= 0)
		{
			lpObj->SkillAddCriticalDamageTime = 0;
			lpObj->SkillAddCriticalDamage = 0;

			MsgOutput(lpObj->m_Index,lMsg.Get(1242));

			lpObj->m_ViewSkillState &= ~0x800;
			GCMagicCancelSend(lpObj,64);
		}
	}

	TMonsterSkillElementInfo::CheckSkillElementInfoProc(lpObj);

	if(lpObj->m_iSkillStunTime != 0)
	{
		lpObj->m_iSkillStunTime--;

		if(lpObj->m_iSkillStunTime <= 0)
		{
			lpObj->m_iSkillStunTime = 0;
			lpObj->m_ViewSkillState &= ~0x1000;

			if(lpObj->Type == OBJ_USER)
			{				
				MsgOutput(lpObj->m_Index,lMsg.Get(1636));
			}
			GCSkillInfoSend(lpObj,0,0x3D);
		}
	}

	if(lpObj->m_iSkillBrandOfSkillTime != 0 && lpObj->Type == OBJ_USER)
	{
		lpObj->m_iSkillBrandOfSkillTime--;

		if(lpObj->m_iSkillBrandOfSkillTime <= 0)
		{
			lpObj->m_iSkillBrandOfSkillTime = 0;
			MsgOutput(lpObj->m_Index,lMsg.Get(1637));

			lpObj->m_ViewSkillState &= ~0x200;
			
			GCSkillInfoSend(lpObj,0,0x13);		//Sacrifice Skill
		}
	}

	if(lpObj->m_iSkillInvisibleTime != 0 && lpObj->Type == OBJ_USER)
	{
		lpObj->m_iSkillInvisibleTime--;

		if(lpObj->m_iSkillInvisibleTime <= 0)
		{
			lpObj->m_iSkillInvisibleTime = 0;
			MsgOutput(lpObj->m_Index,lMsg.Get(1638));

			lpObj->m_ViewSkillState &= ~0x4000;
			GCSkillInfoSend(lpObj,0,0x12);
		}
	}

	if(lpObj->m_iSkillManaSwellTime != 0 && lpObj->Type == OBJ_USER)
	{
		lpObj->m_iSkillManaSwellTime--;

		if(lpObj->m_iSkillManaSwellTime <= 0)
		{
			lpObj->m_iSkillManaSwellTime = 0;
			lpObj->m_iSkillManaSwell = 0;
			MsgOutput(lpObj->m_Index,lMsg.Get(1639));

			lpObj->m_ViewSkillState &= ~0x2000;
			GCSkillInfoSend(lpObj,0,0x09);
			gObjCalCharacter(lpObj->m_Index);		//Recalculate Add Mana
		}
	}

	if(lpObj->m_iPotionBlessTime != 0 && lpObj->Type == OBJ_USER)
	{
		lpObj->m_iPotionBlessTime--;

		if(lpObj->m_iPotionBlessTime <= 0)
		{
			lpObj->m_iPotionBlessTime = 0;
			MsgOutput(lpObj->m_Index,lMsg.Get(1634));

			lpObj->m_ViewSkillState &= ~0x8000;
			GCSkillInfoSend(lpObj,0,0x0A);
		}
	}

	if(lpObj->m_iPotionSoulTime != 0 && lpObj->Type == OBJ_USER)
	{
		lpObj->m_iPotionSoulTime--;

		if(lpObj->m_iPotionSoulTime <= 0)
		{
			lpObj->m_iPotionSoulTime = 0;
			MsgOutput(lpObj->m_Index,lMsg.Get(1635));

			lpObj->m_ViewSkillState &= ~0x10000;
			GCSkillInfoSend(lpObj,0,0x0B);
		}
	}

	if(lpObj->Type == OBJ_USER)
	{
		for(int i=0;i<2;i++)
		{
			if(lpObj->m_iItemEffectValidTime[i] > 0)
			{
				lpObj->m_iItemEffectValidTime[i]--;

				if(lpObj->m_iItemEffectValidTime[i] <= 0)
				{
					lpObj->m_iItemEffectValidTime[i] = 0;

					g_ItemAddOption.ClearItemEffect(lpObj,1,i);
					gObjCalCharacter(lpObj->m_Index);		//Item Effect Recalculate
				}
			}
		}
	}

	if(lpObj->m_SkillNumber < 1)
	{
		return;
	}

	if(lpObj->m_SkillNumber == 18)
	{
		if(GetTickCount() > lpObj->m_SkillTime)
		{
			lpObj->m_SkillNumber = 0;
		}
	}

	if(lpObj->m_iMuseElfInfinityArrowSkillTime != 0 && lpObj->Type == OBJ_USER)
	{
		lpObj->m_iMuseElfInfinityArrowSkillTime--;

		if(lpObj->m_iMuseElfInfinityArrowSkillTime <= 0)
		{
			lpObj->m_iMuseElfInfinityArrowSkillTime = 0;
			lpObj->m_ViewSkillState &= ~1024;
			GCSkillInfoSend(lpObj,0,0x06);
		}
	}
}






void gObjSkillBeAttackProc(LPOBJ lpObj)
{
	int ad,iShieldDamage = 0;

	if(lpObj->m_SkillLightingStormTime > 0)
	{
		lpObj->m_SkillLightingStormTime--;

		if(lpObj->Life <= 0.0f)
			lpObj->m_SkillLightingStormTime = 0;

		if(lpObj->m_SkillLightingStormTime == 0)
		{
			GCSkillInfoSend(lpObj,0,0x54);
			lpObj->m_SkillLightingStorm = 0;
			lpObj->m_SkillLightingStormSender = 0;
		}else
		{
			int Dmg = (1-((lpObj->m_AddResistance[R_LIGHTNING] + lpObj->m_Resistance[R_LIGHTNING])/10.0f)) * lpObj->m_SkillLightingStorm;

			if(Dmg < 0)
				Dmg = 0;

			if(g_ShieldSystemOn == 1)
			{
				if(lpObj->Type == OBJ_USER && gObj[lpObj->m_SkillLightingStormSender].Type == OBJ_USER)
				{
					if(lpObj->iShield > Dmg)
					{
						lpObj->iShield -= Dmg;
						iShieldDamage = Dmg;
						Dmg = 0;
					}
					else
					{
						Dmg -= lpObj->iShield;
						iShieldDamage = lpObj->iShield;
						lpObj->iShield = 0;

						lpObj->Life -= Dmg;

						if(lpObj->Life < 0)
						{
							lpObj->Life = 0;
						}
					}
				}
				else
				{
					lpObj->Life -= Dmg;

					if(lpObj->Life < 0)
					{
						lpObj->Life = 0;
					}
				}
			}
			else
			{
				lpObj->Life -= Dmg;

				if(lpObj->Life < 0)
				{
					lpObj->Life = 0;
				}
			}

			gObjLifeCheck(lpObj,&gObj[lpObj->m_SkillLightingStormSender],Dmg,DAMAGE_TYPE_BEAT,0, 0, AT_SKILL_POISON,iShieldDamage);
		}
	}

	if(lpObj->m_SkillSummonerBookTime > 0)
	{
		lpObj->m_SkillSummonerBookTime--;

		if(lpObj->Life <= 0.0f)
			lpObj->m_SkillSummonerBookTime = 0;

		if (gObjCalDistance(lpObj,&gObj[lpObj->m_SkillSummonerBookSender]) > 6)
		{
			lpObj->m_SkillSummonerBookTime = 0;
		}

		if(lpObj->m_SkillSummonerBookTime == 0)
		{
			GCSkillInfoSend(lpObj,0,lpObj->m_SkillSummonerBookTypeDmg);
			lpObj->m_SkillSummonerBookTypeDmg = 0;	
			lpObj->m_SkillSummonerBookDmg=0;
			lpObj->m_SkillSummonerBookSender=0;
		}else
		{
			int Dmg = lpObj->m_SkillSummonerBookDmg;

			if(lpObj->m_SkillSummonerBookTypeDmg == 0x57)//Phantom
			{
				Dmg = (1-((lpObj->m_AddResistance[R_WIND] + lpObj->m_Resistance[R_WIND])/10.0f)) * lpObj->m_SkillSummonerBookDmg;
			}else if(lpObj->m_SkillSummonerBookTypeDmg == 0x4B)//Sahamut
			{
				Dmg = (1-((lpObj->m_AddResistance[R_FIRE] + lpObj->m_Resistance[R_FIRE])/10.0f)) * lpObj->m_SkillSummonerBookDmg;
			}else if(lpObj->m_SkillSummonerBookTypeDmg == 0x4A)//Nail
			{
				Dmg = (1-((lpObj->m_AddResistance[R_EARTH] + lpObj->m_Resistance[R_EARTH])/10.0f)) * lpObj->m_SkillSummonerBookDmg;
			}

			if(Dmg < 0)
				Dmg = 0;

			if(g_ShieldSystemOn == 1)
			{
				if(lpObj->Type == OBJ_USER && gObj[lpObj->m_SkillSummonerBookSender].Type == OBJ_USER)
				{
					if(lpObj->iShield > Dmg)
					{
						lpObj->iShield -= Dmg;
						iShieldDamage = Dmg;
						Dmg = 0;
					}
					else
					{
						Dmg -= lpObj->iShield;
						iShieldDamage = lpObj->iShield;
						lpObj->iShield = 0;

						lpObj->Life -= Dmg;

						if(lpObj->Life < 0)
						{
							lpObj->Life = 0;
						}
					}
				}
				else
				{
					lpObj->Life -= Dmg;

					if(lpObj->Life < 0)
					{
						lpObj->Life = 0;
					}
				}
			}
			else
			{
				lpObj->Life -= Dmg;

				if(lpObj->Life < 0)
				{
					lpObj->Life = 0;
				}
			}

			gObjLifeCheck(lpObj,&gObj[lpObj->m_SkillSummonerBookSender],Dmg,DAMAGE_TYPE_BEAT,0,0, AT_SKILL_POISON,iShieldDamage);
		}
	}

	if(lpObj->m_SkillSwordSlashTime > 0)
	{
		lpObj->m_SkillSwordSlashTime--;

		if(lpObj->Life <= 0.0f)
			lpObj->m_SkillSwordSlashTime = 0;

		if(lpObj->m_SkillSwordSlashTime == 0)
		{
			GCSkillInfoSend(lpObj,0,0x53);
			lpObj->m_SkillSwordSlashDmg=0;
			lpObj->m_SkillSwordSlashSender=0;
		}else
		{
			int Dmg = (1-((lpObj->m_AddResistance[R_WIND] + lpObj->m_Resistance[R_WIND])/10.0f)) * lpObj->m_SkillSwordSlashDmg;

			if(Dmg < 0)
				Dmg = 0;

			if(g_ShieldSystemOn == 1)
			{
				if(lpObj->Type == OBJ_USER && gObj[lpObj->m_SkillSwordSlashSender].Type == OBJ_USER)
				{
					if(lpObj->iShield > Dmg)
					{
						lpObj->iShield -= Dmg;
						iShieldDamage = Dmg;
						Dmg = 0;
					}
					else
					{
						Dmg -= lpObj->iShield;
						iShieldDamage = lpObj->iShield;
						lpObj->iShield = 0;

						lpObj->Life -= Dmg;

						if(lpObj->Life < 0)
						{
							lpObj->Life = 0;
						}
					}
				}
				else
				{
					lpObj->Life -= Dmg;

					if(lpObj->Life < 0)
					{
						lpObj->Life = 0;
					}
				}
			}
			else
			{
				lpObj->Life -= Dmg;

				if(lpObj->Life < 0)
				{
					lpObj->Life = 0;
				}
			}

			gObjLifeCheck(lpObj,&gObj[lpObj->m_SkillSwordSlashSender],Dmg,DAMAGE_TYPE_BEAT,0,0, AT_SKILL_POISON,iShieldDamage);
		}
	}

	if(lpObj->m_SkillRedStormTime > 0)
	{
		lpObj->m_SkillRedStormTime--;

		if(lpObj->Life <= 0.0f)
			lpObj->m_SkillRedStormTime = 0;

		if(lpObj->m_SkillRedStormTime == 0)
		{
			GCSkillInfoSend(lpObj,0,0x55);
			lpObj->m_SkillRedStormDmg=0;
			lpObj->m_SkillRedStormSender=0;
		}else
		{
			int Dmg = (1-((lpObj->m_AddResistance[R_LIGHTNING] + lpObj->m_Resistance[R_LIGHTNING])/10.0f)) * lpObj->m_SkillRedStormDmg;

			if(Dmg < 0)
				Dmg = 0;

			if(g_ShieldSystemOn == 1)
			{
				if(lpObj->Type == OBJ_USER && gObj[lpObj->m_SkillRedStormSender].Type == OBJ_USER)
				{
					if(lpObj->iShield > Dmg)
					{
						lpObj->iShield -= Dmg;
						iShieldDamage = Dmg;
						Dmg = 0;
					}
					else
					{
						Dmg -= lpObj->iShield;
						iShieldDamage = lpObj->iShield;
						lpObj->iShield = 0;

						lpObj->Life -= Dmg;

						if(lpObj->Life < 0)
						{
							lpObj->Life = 0;
						}
					}
				}
				else
				{
					lpObj->Life -= Dmg;

					if(lpObj->Life < 0)
					{
						lpObj->Life = 0;
					}
				}
			}
			else
			{
				lpObj->Life -= Dmg;

				if(lpObj->Life < 0)
				{
					lpObj->Life = 0;
				}
			}

			gObjLifeCheck(lpObj,&gObj[lpObj->m_SkillRedStormSender],Dmg,DAMAGE_TYPE_BEAT,0,0, AT_SKILL_POISON,iShieldDamage);
		}
	}

	if(lpObj->m_PoisonBeattackCount > 0)
	{
		lpObj->m_PoisonBeattackCount--;

		if(lpObj->m_PoisonBeattackCount % 3 == 0)
		{
			if(lpObj->Life > 0.0f)
			{
				if(lpObj->m_PoisonType == 38)
				{
					ad = lpObj->Life * 3 / 100;
				}
				else
				{
					ad = lpObj->Life * 3 / 100;
					if(lpObj->m_PoisonType == 1)
					{
						ad += lpObj->m_PoisonAdd;
					}
				}
			}
			else
			{
				ad = 1;
			}

			if(ad < 0)
			{
				ad = 0;
			}

			if(g_ShieldSystemOn == 1)
			{
				if(lpObj->Type == OBJ_USER && lpObj->lpAttackObj->Type == OBJ_USER)
				{
					if(lpObj->iShield > ad)
					{
						lpObj->iShield -= ad;
						iShieldDamage = ad;
						ad = 0;
					}
					else
					{
						ad -= lpObj->iShield;
						iShieldDamage = lpObj->iShield;
						lpObj->iShield = 0;

						lpObj->Life -= ad;

						if(lpObj->Life < 0)
						{
							lpObj->Life = 0;
						}
					}
				}
				else
				{
					lpObj->Life -= ad;

					if(lpObj->Life < 0)
					{
						lpObj->Life = 0;
					}
				}
			}
			else
			{
				lpObj->Life -= ad;

				if(lpObj->Life < 0)
				{
					lpObj->Life = 0;
				}
			}

			if(lpObj->lpAttackObj != 0)
			{
				gObjLifeCheck(lpObj,lpObj->lpAttackObj,ad,DAMAGE_TYPE_BEAT,0,0, AT_SKILL_POISON,iShieldDamage);
			}

			if(lpObj->m_PoisonBeattackCount <= 0)
			{
				lpObj->lpAttackObj = 0;
				lpObj->m_ViewSkillState &= ~1;
				//lpObj->m_ViewState = 0;

				if(lpObj->m_PoisonType == 38)
				{
					GCSkillInfoSend(lpObj,0,0x37);
				}
				else
				{
					GCMagicCancelSend(lpObj,1);
				}

				lpObj->m_PoisonType = 0;
				lpObj->m_PoisonAdd = 0;
			}
		}
	}
	else if(lpObj->m_ColdBeattackCount > 0)
	{
		lpObj->m_ColdBeattackCount--;

		if(lpObj->m_ColdBeattackCount <= 0)
		{
			lpObj->lpAttackObj = 0;
			lpObj->DelayActionTime = 0;
			lpObj->DelayLevel = 0;

			lpObj->m_ViewSkillState &= ~2;
			//lpObj->m_ViewState = 0;

			if(lpObj->m_IceType == 39)
			{
				GCSkillInfoSend(lpObj,0,0x38);
			}
			else
			{
				GCMagicCancelSend(lpObj,7);
			}

			lpObj->m_IceType = 0;
		}
	}
	else if(lpObj->m_ImmuneToMagicCount > 0)
	{
		lpObj->m_ImmuneToMagicCount--;

		if(lpObj->m_ImmuneToMagicCount <= 0)
		{
			lpObj->m_ViewSkillState &= ~0x80;
			GCSkillInfoSend(lpObj,1,0x3F);
		}
	}
	else if(lpObj->m_ImmuneToHarmCount > 0)
	{
		lpObj->m_ImmuneToHarmCount--;

		if(lpObj->m_ImmuneToHarmCount <= 0)
		{
			lpObj->m_ViewSkillState &= ~8;
			GCSkillInfoSend(lpObj,0,0x02);
		}
	}
}






void gObjTeleportMagicUse(int aIndex, BYTE x, BYTE y)	// Check if x and y are BYTE
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->Teleport != 0)
	{
		return;
	}

	lpObj->TeleportTime = GetTickCount();
	lpObj->PathCount = 0;
	lpObj->Teleport = 1;
	//lpObj->m_StateSub = 1;
	//lpObj->m_ViewState = 1;

	lpObj->X = x;
	lpObj->Y = y;
	lpObj->TX = x;
	lpObj->TY = y;

	MapC[lpObj->MapNumber].ClearStandAttr(lpObj->m_OldX,lpObj->m_OldY);
	MapC[lpObj->MapNumber].SetStandAttr(lpObj->TX,lpObj->TY);

	lpObj->m_OldX = lpObj->TX;
	lpObj->m_OldY = lpObj->TY;

	gObjViewportListProtocolDestroy(lpObj);
}


BOOL gObjMoveHackCheck(int aIndex)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return false;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->Type == OBJ_USER)
	{
		gDarkSpirit[lpObj->m_Index].SetMode(CDarkSpirit::ePetItem_Mode::PetItem_Mode_Normal,-1);

		if ( gObj[aIndex].CloseCount >= 0 )
			return false; 

		if ( gObj[aIndex].m_bMapSvrMoveQuit == true || gObj[aIndex].m_bMapAntiHackMove == true )
			return false;

		if(lpObj->m_bPShopOpen == true)
		{
			GCServerMsgStringSend("[Anti-Hack] Closing PShop...", lpObj->m_Index, 1);
			if(ReadConfig.AHLog == TRUE)
			{
				ANTI_HACK_LOG.Output("[Anti-Hack][Move][%s][%d,%d,%d] Open PShop-Dupe Attempt",
					lpObj->Name,lpObj->MapNumber,lpObj->X,lpObj->Y);
			}
			CGPShopReqClose(lpObj->m_Index);

			GCServerMsgStringSend("[Anti-Hack] Please try to move again!", lpObj->m_Index, 1);
			return false;
		}

		if(lpObj->m_IfState.use == 1 && lpObj->m_IfState.type == 1)
		{
			GCServerMsgStringSend("[Anti-Hack] Cancelling Trade...", lpObj->m_Index, 1);
			if(ReadConfig.AHLog == TRUE)
			{
				ANTI_HACK_LOG.Output("[Anti-Hack][Move][%s][%d,%d,%d] Trade-Dupe Attempt 1, Trade: %s[%d], Action: %d",
					lpObj->Name,lpObj->MapNumber,lpObj->X,lpObj->Y,gObj[lpObj->TargetNumber].Name,lpObj->TargetNumber,lpObj->pTransaction);
			}
			CGTradeCancelButtonRecv(lpObj->m_Index);

			GCServerMsgStringSend("[Anti-Hack] Please try to move again!", lpObj->m_Index, 1);
			return false;
		}

		if(lpObj->m_IfState.use == 1 && lpObj->m_IfState.type == 6)
		{
			GCServerMsgStringSend("[Anti-Hack] Closing Warehouse...", lpObj->m_Index, 1);
			if(ReadConfig.AHLog == TRUE)
			{
				ANTI_HACK_LOG.Output("[Anti-Hack][Move][%s][%d,%d,%d] Open Warehouse-Dupe Attempt",
					lpObj->Name,lpObj->MapNumber,lpObj->X,lpObj->Y);
			}
			CGWarehouseUseEnd(lpObj->m_Index);

			GCServerMsgStringSend("[Anti-Hack] Please try to move again!", lpObj->m_Index, 1);
			return false;
		}

		if(lpObj->DieRegen != 0)
		{
			GCServerMsgStringSend("[Anti-Hack] DieRegen ERROR!",lpObj->m_Index,1);
			if(ReadConfig.AHLog == TRUE)
			{
				ANTI_HACK_LOG.Output("[Anti-Hack][Move][%s][%d,%d,%d] Trying to Move during DIE",
					lpObj->Name,lpObj->MapNumber,lpObj->X,lpObj->Y);
			}

			GCServerMsgStringSend("[Anti-Hack] Please try to move again!", lpObj->m_Index, 1);
			return false;
		}

		if ((lpObj->TargetNumber!=-1)&&(lpObj->pTransaction==1))
		{
			GCServerMsgStringSend("[Anti-Hack][Move] DUPE ATTEMPT: In Trade!",aIndex, 0x01);
			if(ReadConfig.AHLog == TRUE)
			{
				ANTI_HACK_LOG.Output("[Anti-Hack][Move][%s][%d,%d,%d] Trade-Dupe Attempt 2, Trade: %s[%d], Action: %d",
					lpObj->Name,lpObj->MapNumber,lpObj->X,lpObj->Y,gObj[lpObj->TargetNumber].Name,lpObj->TargetNumber,lpObj->pTransaction);
			}
			return false;
		} 

		if (lpObj->m_IfState.use == 1 && (
			lpObj->m_IfState.type == 96 ||	//Illusion Temple
			lpObj->m_IfState.type == 97 ||	//Lucky Coins
			lpObj->m_IfState.type == 98 ||	//Imperial Guardian
			lpObj->m_IfState.type == 99)	//Double Goer
			)
		{
			lpObj->m_IfState.use = 0;
			lpObj->m_IfState.type = 0;
		}

		if ( lpObj->m_IfState.use != 0 ) 
		{
			GCServerMsgStringSend("[Anti-Hack][Move] Character in use state!",aIndex, 0x01);
			if(ReadConfig.AHLog == TRUE)
			{
				ANTI_HACK_LOG.Output("[Anti-Hack][Move][%s][%d,%d,%d] Character in use state, State: %d,%d,%d",
					lpObj->Name,lpObj->MapNumber,lpObj->X,lpObj->Y,lpObj->m_IfState.state,lpObj->m_IfState.use,lpObj->m_IfState.type);
			}
			return false;
		}
	}

	return TRUE;
}



BOOL gObjMoveGate(int aIndex, int gt)
{
	int movefail = 0;
	BYTE mapNumber,dir;
	short x,y,level;

	if(aIndex < 0 || aIndex > OBJMAX - 1)
	{
		return false;
	}

	if(!gObjMoveHackCheck(aIndex))
	{
		return false;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->SkillRecallParty_Time != 0)
	{
		char szMsg[255];
		lpObj->SkillRecallParty_Time = 0;
		wsprintf(szMsg,lMsg.Get(1254));
		GCServerMsgStringSend(szMsg,aIndex,1);
	}

	//MOVED HIGHER
//#if (PACK_EDITION>=2)
	movefail = !g_ImperialGuardian.gObjMoveGateCheck(lpObj,gt);
//#else
//	movefail = 0;
//#endif

#if (PACK_EDITION>=3)
	#if (GS_CASTLE==1)
		if(g_Swamp.GateMove(aIndex,gt) == false)
			movefail = 1;
	#endif
#endif

	if(gGateC.IsGate(gt) == false)
	{
		LogAdd(lMsg.Get(545),gt,__FILE__,__LINE__);
		movefail = 1;
	}

	if(gGateC.GetGate(gt,(short &)x,(short &)y,(BYTE &)mapNumber,(BYTE &)dir,(short &)level) < 0)
	{
		LogAdd("Error : GetGate: %d (%s %d)",gt,__FILE__,__LINE__);
		movefail = 1;
	}

	if(gGateC.CheckGateLevel(aIndex,gt) == 0)
	{
		GCServerMsgStringSend("[Move] Too low level to enter!",lpObj->m_Index,1);
		LogAddTD("[MapServerMng][%s][%s] Move Gate:%d Fail based on Level:%d",lpObj->AccountID,lpObj->Name,gt,lpObj->Level);
		movefail = 1;
	}

	if ((BC_MAP_RANGE(mapNumber) == 0) && 
		(CC_MAP_RANGE(mapNumber) == 0) && 
		(IT_MAP_RANGE(mapNumber) == 0) && 
		(KALIMA_MAP_RANGE(mapNumber) == 0) && 
#if (PACK_EDITION>=2)
		(IMPERIALGUARDIAN_MAP_RANGE(mapNumber) == 0) &&  
		(SKYEVENT_MAP_RANGE(mapNumber,x) == 0) && 
#endif 
#if (PACK_EDITION>=3)
		(DG_MAP_RANGE(mapNumber) == 0)	&&
#endif
		(DS_MAP_RANGE(mapNumber) == 0))
	{
#if (PACK_EDITION>=2)
		if(HalloweenEvent.Enabled == 1 && HalloweenEvent.Start == true)
		{
			if(HalloweenEvent.CheckMap(lpObj->MapNumber) == true && HalloweenEvent.CheckMap(mapNumber) == false)
			{
				gObjClearViewport(&gObj[aIndex]);
				GCTeleportSend(&gObj[aIndex], gt, 
				gObj[aIndex].MapNumber, gObj[aIndex].X, gObj[aIndex].Y, gObj[aIndex].Dir);
				GCServerMsgStringSend("You cant warp to other map until Halloween PK Event finish",lpObj->m_Index,1);
				return false;
			}
		}
#endif
		
#if (PACK_EDITION>=3)
		if(GensCloseMap.Enabled == 1)
		{
			if(GensCloseMap.CheckMap(lpObj->m_Index,mapNumber) == true)
			{
				gObjClearViewport(&gObj[aIndex]);
				GCTeleportSend(&gObj[aIndex], gt, 
				gObj[aIndex].MapNumber, gObj[aIndex].X, gObj[aIndex].Y, gObj[aIndex].Dir);
				return false;
			}
		}
#endif
	}

	//Check Maps
	if(gt >= 58 && gt <= 61)
	{
		if(lpObj->m_bDevilSquareAuth == 0)
		{
			LogAddTD("[DevilSquare] [%s][%s] Not Used DevilSquare Invitation In DevilSquare",lpObj->AccountID,lpObj->Name);
			return false;
		}
		else
		{
			lpObj->m_bDevilSquareAuth = 0;
		}
	}

	if(BC_MAP_RANGE(lpObj->MapNumber))
	{
		if(g_BloodCastle.GetCurrentState(lpObj->MapNumber-11) == BC_STATE_PLAYING)
		{
			g_BloodCastle.SearchUserDropQuestItem(lpObj->m_Index);
		}
		else
		{
			g_BloodCastle.SearchUserDeleteQuestItem(lpObj->m_Index);
		}
	}

	if( BC_MAP_RANGE(mapNumber) || 
		CC_MAP_RANGE(mapNumber) ||
		IT_MAP_RANGE(mapNumber) ||
#if (PACK_EDITION>=2)
		IMPERIALGUARDIAN_MAP_RANGE(mapNumber) ||
#endif
#if (PACK_EDITION>=3)
		DG_MAP_RANGE(mapNumber) ||
#endif
		DS_MAP_RANGE(mapNumber))
	{
#if (PACK_EDITION>=3)
		int botNum = lpObj->BotNumOwner;
		
		if((botNum >= 0) && (gObjIsConnected(botNum) >= 0))
		{
			if(lpObj->m_RecallMon >= 0)
			{			
				GCServerMsgStringSend("BotPet is Logged Out!", aIndex, 1);
				botPet.Close(aIndex);
			}
		}
#endif

		if(lpObj->m_iSkillNPCHelpTime > 0)
		{
			lpObj->m_iSkillNPCHelpTime = 0;
			lpObj->m_iSkillNPCDefense = 0;
			lpObj->m_iSkillNPCAttack = 0;

			lpObj->m_ViewSkillState &= ~131072;
			GCSkillInfoSend(lpObj,0,0x03);
		}
	}

	if(!KALIMA_MAP_RANGE(mapNumber) && KALIMA_MAP_RANGE(lpObj->MapNumber))
	{
		int iKalimaGateIndex = lpObj->m_iKalimaGateIndex;

		if(g_KalimaGate.DeleteKalimaGate(lpObj->m_Index))
		{
			LogAddTD("[Kalima] [%s][%s] Kalima Gate Vanished - User MoveGate (SummonIndex:%d, EnterCount:%d)",lpObj->AccountID,lpObj->Name,iKalimaGateIndex,gObj[iKalimaGateIndex].m_cKalimaGateEnterCount);
		}
	}

	if(mapNumber == MAP_INDEX_ATLANS)
	{
		if(lpObj->pInventory[8].IsItem())
		{
			if(lpObj->pInventory[8].m_Type == ITEMGET(13,2))
			{
				GCServerMsgStringSend(lMsg.Get(702),lpObj->m_Index,1);
				movefail = 1;
			}

			if(lpObj->pInventory[8].m_Type == ITEMGET(13,3))
			{
				GCServerMsgStringSend(lMsg.Get(1604),lpObj->m_Index,1);
				movefail = 1;
			}
		}
	}

	if(mapNumber == MAP_INDEX_ICARUS)
	{
		if(!(lpObj->pInventory[8].m_Type == ITEMGET(13,3)
			|| lpObj->pInventory[7].m_Type == ITEMGET(13,30)
			|| lpObj->pInventory[8].m_Type == ITEMGET(13,37)
			|| (lpObj->pInventory[7].m_Type >= ITEMGET(12,0)
				&& lpObj->pInventory[7].m_Type <=ITEMGET(12,6))
			|| (lpObj->pInventory[7].m_Type >= ITEMGET(12,36)
				&& lpObj->pInventory[7].m_Type <=ITEMGET(12,43))
			|| (lpObj->pInventory[7].m_Type >= ITEMGET(12,49)
				&& lpObj->pInventory[7].m_Type <=ITEMGET(12,50))
			|| (lpObj->pInventory[7].m_Type >= ITEMGET(12,130)
				&& lpObj->pInventory[7].m_Type <=ITEMGET(12,135)))
			|| lpObj->pInventory[8].m_Type == ITEMGET(13,2)
			|| lpObj->pInventory[11].m_Type == ITEMGET(13,10)
			|| lpObj->pInventory[10].m_Type == ITEMGET(13,10)
			|| lpObj->pInventory[10].m_Type == ITEMGET(13,39)
			|| lpObj->pInventory[11].m_Type == ITEMGET(13,39)
			|| lpObj->pInventory[10].m_Type == ITEMGET(13,68)
			|| lpObj->pInventory[11].m_Type == ITEMGET(13,68)
			|| lpObj->pInventory[10].m_Type == ITEMGET(13,76)
			|| lpObj->pInventory[11].m_Type == ITEMGET(13,76)
			|| lpObj->pInventory[10].m_Type == ITEMGET(13,122)
			|| lpObj->pInventory[11].m_Type == ITEMGET(13,122)
			|| lpObj->pInventory[10].m_Type == ITEMGET(13,40)
			|| lpObj->pInventory[11].m_Type == ITEMGET(13,40)
			|| lpObj->pInventory[10].m_Type == ITEMGET(13,41)
			|| lpObj->pInventory[11].m_Type == ITEMGET(13,41)
#if (CRYSTAL_EDITION == 1)
			|| (lpObj->pInventory[7].m_Type >= ITEMGET(12,200) && lpObj->pInventory[7].m_Type <= ITEMGET(12,254))
#endif
			)
		{
			GCServerMsgStringSend(lMsg.Get(1604),lpObj->m_Index,1);
			movefail = 1;
			return false;
		}

		if(lpObj->m_RecallMon >= 0)
		{
			GCRecallMonLife(aIndex,60,0);
			gObjMonsterCallKill(aIndex);
		}
	}

	if(mapNumber == MAP_INDEX_KANTURU_BOSS)
	{
		if(!(lpObj->pInventory[8].m_Type == ITEMGET(13,3)
			|| lpObj->pInventory[7].m_Type == ITEMGET(13,30)
			|| lpObj->pInventory[8].m_Type == ITEMGET(13,37)
			|| (lpObj->pInventory[7].m_Type >= ITEMGET(12,0)
				&& lpObj->pInventory[7].m_Type <=ITEMGET(12,6))
			|| (lpObj->pInventory[7].m_Type >= ITEMGET(12,36)
				&& lpObj->pInventory[7].m_Type <=ITEMGET(12,43))
			|| (lpObj->pInventory[7].m_Type >= ITEMGET(12,49)
				&& lpObj->pInventory[7].m_Type <=ITEMGET(12,50))
			|| (lpObj->pInventory[7].m_Type >= ITEMGET(12,130)
				&& lpObj->pInventory[7].m_Type <=ITEMGET(12,135)))
			|| lpObj->pInventory[11].m_Type == ITEMGET(13,10)
			|| lpObj->pInventory[10].m_Type == ITEMGET(13,10)
			|| lpObj->pInventory[11].m_Type == ITEMGET(13,39)
			|| lpObj->pInventory[10].m_Type == ITEMGET(13,39)
			|| lpObj->pInventory[11].m_Type == ITEMGET(13,68)
			|| lpObj->pInventory[10].m_Type == ITEMGET(13,68)
			|| lpObj->pInventory[11].m_Type == ITEMGET(13,76)
			|| lpObj->pInventory[10].m_Type == ITEMGET(13,76)
			|| lpObj->pInventory[11].m_Type == ITEMGET(13,122)
			|| lpObj->pInventory[10].m_Type == ITEMGET(13,122)
			|| lpObj->pInventory[11].m_Type == ITEMGET(13,40)
			|| lpObj->pInventory[10].m_Type == ITEMGET(13,40)
			|| lpObj->pInventory[11].m_Type == ITEMGET(13,41)
			|| lpObj->pInventory[10].m_Type == ITEMGET(13,41)
#if (CRYSTAL_EDITION == 1)
			|| (lpObj->pInventory[7].m_Type >= ITEMGET(12,200) && lpObj->pInventory[7].m_Type <= ITEMGET(12,254))
#endif
			)
		{
			GCServerMsgStringSend(lMsg.Get(1604),lpObj->m_Index,1);
			movefail = 1;
			return false;
		}
	}

	if(g_iBlockKanturuMapEnter == 1)
	{
		if((mapNumber >= MAP_INDEX_KANTURU1) && (mapNumber <= MAP_INDEX_KANTURU_BOSS))
		{
			GCServerMsgStringSend(lMsg.Get(1604),lpObj->m_Index,1);
			movefail = 1;
		}
	}

	if(g_iBlockCastleSiegeMapEnter == 1)
	{
		if(mapNumber == MAP_INDEX_CASTLESIEGE || mapNumber == MAP_INDEX_CASTLEHUNTZONE)
		{
			GCServerMsgStringSend(lMsg.Get(1604),lpObj->m_Index,1);
			movefail = 1;
		}
	}

	#if (GS_CASTLE==1)
	if(gt == 96)
	{
		if(g_CastleSiege.CheckCastleOwnerMember(lpObj->m_Index) == FALSE && g_CastleSiege.CheckCastleOwnerUnionMember(lpObj->m_Index) == FALSE)
		{
			if((g_CastleSiege.GetCastleState() == 7) && (g_CastleSiege.CheckGuardianStatueExist() == FALSE))
			{

			} else {
				GCServerMsgStringSend(lMsg.Get(MSGGET(6, 88)),lpObj->m_Index,1);
				movefail = 1;
			}
		}
	}
	#endif

	//#if (WL_PROTECT==1)  
	//	VM_START_WITHLEVEL(5)
	//	int MyCheckVar1;
	//	CHECK_PROTECTION(MyCheckVar1, 0x62175416)  
	//	if (MyCheckVar1 != 0x62175416)
	//	{	
	//		gObjTeleport(rand()%aIndex+20, rand()%64,rand()%255,rand()%255);
	//	}
	//	VM_END
	//#endif

	//Final MOVE
	if(movefail == 0)
	{
		lpObj->Dir = dir;
		//lpObj->m_MoveGateNumber = gt;

		//CryWolf Positioning Fix
		if ( mapNumber == MAP_INDEX_CRYWOLF_FIRSTZONE )
		{
			if ( g_CrywolfSync.GetOccupationState() == CRYWOLF_OCCUPATION_SECURE ||
				(g_CrywolfSync.GetCrywolfState() >= CRYWOLF_STATE_NOTIFY_1 && 
				 g_CrywolfSync.GetCrywolfState() < CRYWOLF_STATE_START))
			{
				MapC[lpObj->MapNumber].GetMapPos( mapNumber,(short &)x,(short &)y);
			}
		}

		//Swamp Event Fix
		#if (GS_CASTLE==1)
		#if (PACK_EDITION>=3)
		if ( mapNumber == MAP_INDEX_SWAMP )
		{
			if(g_Swamp.SwampMapEnterOnlyWhenStarted == 1 && g_Swamp.Enabled)
			{
				if(g_Swamp.Start == FALSE)
				{
					MapC[lpObj->MapNumber].GetMapPos( mapNumber,(short &)x,(short &)y);
				}
			}
		}
		#endif
		#endif
		LogAddTD("[MoveMap][%s][%s] : (Before:Map:%d X:%d Y:%d) -> (New Map:%d X:%d Y:%d)",lpObj->AccountID,lpObj->Name,lpObj->MapNumber,lpObj->X,lpObj->Y,mapNumber,x,y);

		gObjTeleport(lpObj->m_Index, mapNumber, x, y);

		if ( PvP.GetType(mapNumber) == 2)
			GCServerMsgStringSend("You are in PK Free Map!",aIndex, 1);

		if ( PvP.GetType(mapNumber) == 4)
			GCServerMsgStringSend("You are in GensPvP PK Free Map!",aIndex, 1);

		#if (GS_CASTLE==1)
		//Castle Siege State Send Fix [6]
		if ( lpObj->MapNumber == MAP_INDEX_CASTLESIEGE )
		{
			if ( g_CastleSiege.GetCastleState() == 7 )
			{
				GCAnsCsNotifyStart(lpObj->m_Index, 1);
				g_CastleSiege.SetUserCsJoinSide(lpObj->m_Index);
				g_CastleSiege.NotifySelfCsJoinSide(lpObj->m_Index);
				g_CastleSiege.NotifyCsSelfLeftTime(lpObj->m_Index);
			} else {
				GCAnsCsNotifyStart(lpObj->m_Index, 0);
			}
		}
		#endif

		if ( lpObj->MapNumber == MAP_INDEX_CRYWOLF_FIRSTZONE )
		{
			g_CrywolfSync.NotifyCrywolfCurrentStateByUserId(lpObj->m_Index);
		}
	} else {
		gObjTeleport(lpObj->m_Index, lpObj->MapNumber,lpObj->X,lpObj->Y);
		return false;
	}

	return true;
}

void gObjMassMove(int Map, int ToMap, int ToX, int ToY)
{
	for ( int n = OBJ_STARTUSERINDEX ; n < OBJMAX ; n++)
	{
		if ( gObj[n].Connected == PLAYER_PLAYING )
		{
			if ( gObj[n].Type  == OBJ_USER )
			{
				if (gObj[n].MapNumber == Map)
					gObjTeleport(n,ToMap,ToX,ToY);
			}
		}
	}
}

void gObjTeleport(int aIndex, int map, int x, int y)
{
	if(aIndex < 0 || aIndex > OBJMAX - 1)
	{
		return;
	}

	if(!gObjMoveHackCheck(aIndex))
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if(MapNumberCheck(map)==0)
	{
		LogAdd("error-L3 : %s %d",__FILE__,__LINE__);
		return;
	}

	if ((BC_MAP_RANGE(map) == 0) && 
		(CC_MAP_RANGE(map) == 0) && 
		(IT_MAP_RANGE(map) == 0) && 
		(KALIMA_MAP_RANGE(map) == 0) && 
#if (PACK_EDITION>=2)
		(IMPERIALGUARDIAN_MAP_RANGE(map) == 0) &&  
		(SKYEVENT_MAP_RANGE(map,x) == 0) && 
#endif 
#if (PACK_EDITION>=3)
		(DG_MAP_RANGE(map) == 0)	&&
#endif
		(DS_MAP_RANGE(map) == 0))
	{
#if (PACK_EDITION>=2)
		if(HalloweenEvent.Enabled == 1 && HalloweenEvent.Start == true)
		{
			if(HalloweenEvent.CheckMap(gObj[aIndex].MapNumber) == true && HalloweenEvent.CheckMap(map) == false)
			{
				GCServerMsgStringSend("You cant warp to other map until Halloween PK Event finish",gObj[aIndex].m_Index,1);
				return;
			}
		}
#endif
		
#if (PACK_EDITION>=3)
		if(GensCloseMap.Enabled == 1)
		{
			if(GensCloseMap.CheckMap(aIndex,map) == true)
			{
				return;
			}
		}
#endif
	}

	if(gObj[aIndex].MapNumber != map)
	{
		short sSvrCode = g_MapServerManager.CheckMoveMapSvr(lpObj->m_Index,map,lpObj->m_sPrevMapSvrCode);

		if(sSvrCode != gGameServerCode)
		{
			if(gObj[aIndex].b_BlockMoveCommandSeconds > 0)
			{
				//Server Move Anti-Dupe Check
				char moveErr[1024]={0};
				wsprintf(moveErr,"You can move after %d seconds.!", gObj[aIndex].b_BlockMoveCommandSeconds);
				GCServerMsgStringSend(moveErr, aIndex, 1);
				return;
			}

			if(sSvrCode == -1)
			{
				LogAddC(2,"[MapServerMng] Map Server Move Fail : CheckMoveMapSvr() == -1 [%s][%s] (%d)",lpObj->AccountID,lpObj->Name,lpObj->m_Index);
				return;
			}

			if(lpObj->m_bPShopOpen == 1)
			{
				LogAddC(2,"[MapServerMng] Map Server Move Fail : m_bPShopOpen == 1 [%s][%s] (%d)",lpObj->AccountID,lpObj->Name,lpObj->m_Index);
				return;
			}

			GJReqMapSvrMove(lpObj->m_Index,sSvrCode,map,x,y);
			LogAddTD("[MapServerMng] Request to Move Map Server : (%d) - [%s][%s] (%d)",sSvrCode,lpObj->AccountID,lpObj->Name,lpObj->m_Index);
			return;
		}
	}

	lpObj->m_State = 32;
	gObj[aIndex].X = x;
	gObj[aIndex].Y = y;

	gObj[aIndex].TX = x;
	gObj[aIndex].TY = y;	//  FIXED

	gObj[aIndex].MapNumber = map;
	gObj[aIndex].PathCount = 0;
	gObj[aIndex].Teleport = 0;

	gObjClearViewport(&gObj[aIndex]);

	GCTeleportSend(&gObj[aIndex],1,gObj[aIndex].MapNumber,gObj[aIndex].X,gObj[aIndex].Y,gObj[aIndex].Dir);

	//if(lpObj->m_Change >= 0) //-->> Commented IcaruS FIX For Show Pets on MOVE
	//{
		gObjViewportListProtocolCreate(&gObj[aIndex]);
	//}

	gObj[aIndex].RegenMapNumber = map;
	gObj[aIndex].RegenMapX = x;
	gObj[aIndex].RegenMapY = y;
	gObj[aIndex].RegenOk = 1;

	//Display EXP Info
	gObjGetExtraExperienceInfoMsg(aIndex);

	#if (GS_CASTLE==1)
	//Castle Siege State Send Fix [5]
	if ( gObj[aIndex].MapNumber == MAP_INDEX_CASTLESIEGE )
	{
		if ( g_CastleSiege.GetCastleState() == 7 )
		{
			GCAnsCsNotifyStart(aIndex, 1);
			g_CastleSiege.SetUserCsJoinSide(aIndex);
			g_CastleSiege.NotifySelfCsJoinSide(aIndex);
			g_CastleSiege.NotifyCsSelfLeftTime(aIndex);
		} else {
			GCAnsCsNotifyStart(aIndex, 0);
		}
	}
	#endif

	if ( gObj[aIndex].MapNumber == MAP_INDEX_CRYWOLF_FIRSTZONE )
	{
		g_CrywolfSync.NotifyCrywolfCurrentStateByUserId(aIndex);
	}
}

void gObjGetExtraExperienceInfoMsg(int aIndex)
{
	if ( gObjIsConnectedGP(aIndex) == FALSE )
	{
		LogAddC(2, "error-L3 [%s][%d]", __FILE__, __LINE__);
		return;
	}

	if(ReadConfig.ShowExp == 1)
	{
		char sBuf[1024]={0};
		char cwPenalty[255]={0};
		char resetPenalty[255]={0};

		int expExtra = 0;
		LPOBJ lpObj;

		lpObj = &gObj[aIndex];

		if (lpObj->m_btDisableExpGain == 1)
		{
			GCServerMsgStringSend ("Your experience gain is DISABLED!",lpObj->m_Index,0x01 );
			return;
		}

		wsprintf(cwPenalty, "/ NO CWP");
		wsprintf(resetPenalty, "/ NO RP");

		//Get Exp Rate for the user
		expExtra = gObjGetExperienceRate(lpObj,0);

		//Reset Penalty Eperience System
		if (ReadConfig.IsResetPenaltyExp == 1)
		{
	#if (PACK_EDITION>=2)
			if ((VipSystem.VipIsApplyResetExpPenalty == 0)&&(lpObj->Vip >= 1))
			{
			} else {
				expExtra += roundInt(ReadConfig.PenaltyExpPerReset*lpObj->Resets);
				wsprintf(resetPenalty, "/ RP: %dx", roundInt(ReadConfig.PenaltyExpPerReset*lpObj->Resets) );
			}
	#else
			expExtra += roundInt(ReadConfig.PenaltyExpPerReset*lpObj->Resets);
			wsprintf(resetPenalty, "/ Reset Penalty: %dx", roundInt(ReadConfig.PenaltyExpPerReset*lpObj->Resets) );
	#endif
		}

		if ( (lpObj->m_wExprienceRate + lpObj->MasterCharacterInfo->IncExperience) == 0 )
			expExtra = 0;
		else
			expExtra = roundInt(expExtra * ((float)(lpObj->m_wExprienceRate + lpObj->MasterCharacterInfo->IncExperience)  / 100.0f));

		if ( (lpObj->pInventory[8].IsItem() == TRUE && lpObj->pInventory[8].m_Type == ITEMGET(13,123)) ) //NEW SKELETON EFFECT
		{
			expExtra = (int)(expExtra * 1.3f);
		}

		if ( g_CrywolfSync.GetOccupationState() == CRYWOLF_OCCUPATION_FAILED && g_iCrywolfApplyMvpPenalty != FALSE)
		{
	#if (PACK_EDITION>=2)
			if ((VipSystem.VipIsApplyCWExpPenalty == 0)&&(lpObj->Vip >= 1))
			{
			} else {
				wsprintf(cwPenalty, "/ CWP: %dx", (expExtra - (expExtra * g_CrywolfSync.GetGettingExpPenaltyRate()/100)) );
				//expExtra = roundInt((expExtra * g_CrywolfSync.GetGettingExpPenaltyRate()) / 100);
			}
	#else
				wsprintf(cwPenalty, "/ CWP: %dx", (expExtra - (expExtra * g_CrywolfSync.GetGettingExpPenaltyRate()/100)) );
	#endif
		} else {
			if ( g_CrywolfSync.GetOccupationState() != CRYWOLF_OCCUPATION_FAILED && g_iCrywolfApplyMvpPenalty != FALSE)
			{
				wsprintf(cwPenalty, "/ CW Protected");
			}
		}

		wsprintf(sBuf, "Exp: %dx %s %s", expExtra, resetPenalty, cwPenalty);
		GCServerMsgStringSend (sBuf,lpObj->m_Index,0x01 );
	}
}

void gObjMoveDataLoadingOK(int aIndex)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	if(gObj[aIndex].RegenOk == 1)
	{
		gObj[aIndex].RegenOk = 2;
	}
}


class CMagicInf* gObjGetMagic(LPOBJ lpObj, int mIndex)
{
	if ( mIndex < 0 || mIndex > MAX_MAGIC-1 )
	{
		//LogAdd(lMsg.Get(MSGGET(2, 34)), __FILE__, __LINE__);//MAGIC EXCEED
		return NULL;
	}

	if ( lpObj->Magic[mIndex].IsMagic() == FALSE )
	{
		return NULL;
	}

	return &lpObj->Magic[mIndex];
}


CMagicInf * gObjGetMagicSearch(LPOBJ lpObj, WORD skillnumber)
{

	if(skillnumber == 58)
	{
		skillnumber = 40;
	}

	for(int n = 0; n < MAGIC_SIZE;n++)
	{
		if(lpObj->Magic[n].IsMagic() == 1)
		{
			if(lpObj->Magic[n].m_Skill == skillnumber)
			{
				if(gObjMagicEnergyCheck(lpObj,lpObj->Magic[n].m_Skill) == 0
#if (PACK_EDITION>=3)
					&& lpObj->IsBot == 0
#endif
					)
				{
					GCServerMsgStringSend(lMsg.Get(1255),lpObj->m_Index,1);
					return false;
				}
				else
				{
					return &lpObj->Magic[n];
				}
			}
		}
	}
	return false;
}


/*int	gObjMagicManaUse(LPOBJ lpObj, CMagicInf * lpMagic)
{
	float mana = lpObj->Mana;

	if(lpObj->Type != OBJ_USER)
	{
		return false;
	}

	if(lpMagic == 0)
	{
		return false;
	}
	mana -= MagicDamageC.SkillGetMana(lpMagic->m_Skill);

	if(mana < 0)
	{
		return -1;
	}

	return mana;
}*/


/*int  gObjMagicBPUse(LPOBJ lpObj, CMagicInf * lpMagic)
{
	int bp = lpObj->BP;

	if(lpObj->Type != OBJ_USER)
	{
		return false;
	}

	if(lpMagic == 0)
	{
		return false;
	}

	bp -= MagicDamageC.SkillGetBP(lpMagic->m_Skill);

	if(bp < 0)
	{
		return -1;
	}

	return bp;
}*/



struct PMSG_POSMAGIC_RESULT {

  // static data ------------------------------------

  // non-static data --------------------------------
  /*<thisrel this+0x0>*/ /*|0x1|*/ unsigned char NumberH;
  /*<thisrel this+0x1>*/ /*|0x1|*/ unsigned char NumberL;

  // base classes -----------------------------------

  // friends ----------------------------------------

  // static functions -------------------------------

  // non-virtual functions --------------------------

  // virtual functions ------------------------------
};
// <size 0x2>

struct PMSG_POSMAGIC_COUNT {

  // static data ------------------------------------

  // non-static data --------------------------------
  /*<thisrel this+0x0>*/ /*|0x3|*/ struct PBMSG_HEAD h;
  /*<thisrel this+0x3>*/ /*|0x1|*/ unsigned char NumberH;
  /*<thisrel this+0x4>*/ /*|0x1|*/ unsigned char NumberL;
  /*<thisrel this+0x5>*/ /*|0x1|*/ unsigned char MagicNumber;
  /*<thisrel this+0x6>*/ /*|0x1|*/ unsigned char MapX;
  /*<thisrel this+0x7>*/ /*|0x1|*/ unsigned char MapY;
  /*<thisrel this+0x8>*/ /*|0x1|*/ unsigned char Count;

  // base classes -----------------------------------

  // friends ----------------------------------------

  // static functions -------------------------------

  // non-virtual functions --------------------------

  // virtual functions ------------------------------
};
// <size 0x9>


int gObjPosMagicAttack(LPOBJ lpObj, CMagicInf * lpMagic, BYTE x, BYTE y)	// Not used
{
	int tObjNum;
	double dis;
	int tx;
	int ty;
	int MagicDistance;
	unsigned char sbuf[256];
	int lOfs;
	unsigned char MagicNumber;
	PMSG_POSMAGIC_COUNT pCount;
	PMSG_POSMAGIC_RESULT pResult;
	int MVL;
	int n;

	MagicDistance = 3;
	lOfs = 0;
	MagicNumber = lpMagic->m_Skill;

	switch(MagicNumber)
	{
	case 9: MagicDistance = 3;
		break;
	default: MagicDistance = 3;
		break;
	}

	lOfs = sizeof(pCount);
	pCount.h.c = 0xC1;
	pCount.h.headcode = 0x1A;
	pCount.h.size = 0;

	pCount.MapX = x;
	pCount.MapY = y;
	pCount.MagicNumber = MagicNumber;
	pCount.NumberH = SET_NUMBERH(lpObj->m_Index);
	pCount.NumberL = SET_NUMBERL(lpObj->m_Index);
	pCount.Count = 0;

	MVL = MAX_VIEWPORT;

	if(lpObj->Type == OBJ_MONSTER)
	{
		MVL = MAX_VIEWPORT_MONSTER;
	}

	for(n = 0; n < MVL; n++)
	{
		if(lpObj->VpPlayer[n].state)
		{
			tObjNum = lpObj->VpPlayer[n].number;

			if(tObjNum >= 0)
			{
				if(gObj[tObjNum].Live)
				{
					tx = lpObj->X - gObj[tObjNum].X;
					ty = lpObj->Y - gObj[tObjNum].Y;

					dis = (int)sqrt((float)(tx*tx + ty*ty));

					if(dis <= MagicDistance)
					{
						if(gObjAttack(lpObj,&gObj[tObjNum],lpMagic,0,0,0,0) == 1)
						{
							pResult.NumberH = SET_NUMBERH(tObjNum);
							pResult.NumberL = SET_NUMBERL(tObjNum);

							memcpy(&sbuf[lOfs],&pResult,sizeof(pResult));
							pCount.Count++;
							lOfs+=sizeof(pResult);

							if(lOfs > 250)
							{
								LogAdd(lMsg.Get(547),__FILE__,__LINE__);
								return false;
							}
						}
					}
				}
			}
		}
	}

	pCount.h.size = lOfs;

	memcpy(sbuf,&pCount,sizeof(pCount));

	if(lpObj->Type == OBJ_USER)
	{
		DataSend(lpObj->m_Index,sbuf,lOfs);
	}

	MsgSendV2(lpObj,sbuf,lOfs);
	return true;
}


//void gObjMagicAddEnergyCheckOnOff(int flag)
//{
//	g_EnergyCheckOff=flag;
//}



int  gObjMagicAdd(LPOBJ lpObj, BYTE Type, BYTE Index, BYTE Level, WORD & SkillNumber)
{

	int skill = -1,n,reqeng,reqleadership;

	skill = GetSkillNumberInex(Type,Index,Level);

	if(skill < 0)
	{
		GCServerMsgStringSend ("Skill not found!",lpObj->m_Index,0x01 );
		LogAdd(lMsg.Get(548));
		return -1;
	}

	//if(g_EnergyCheckOff == 1)
	//{
		reqeng = MagicDamageC.SkillGetRequireEnergy(skill);

		if(reqeng < 0)
		{
			GCServerMsgStringSend ("Lacking Energy!",lpObj->m_Index,0x01 );
			LogAdd(lMsg.Get(549));
			return -1;
		}

		if((lpObj->Energy + lpObj->AddEnergy) < reqeng)
		{
			GCServerMsgStringSend ("Lacking Energy!",lpObj->m_Index,0x01 );
			LogAdd(lMsg.Get(549));
			return -1;
		}
	//}

	reqleadership = MagicDamageC.GetskillRequireLeadership(skill);

	if(reqleadership < 0)
	{
		LogAdd("error-L2: Skill Leadership not enough");
		GCServerMsgStringSend ("Lacking Leadership!",lpObj->m_Index,0x01 );
		return -1;
	}

	if((lpObj->Leadership + lpObj->AddLeadership) < reqleadership)
	{
		LogAdd("error-L2: Skill Leadership not enough");
		return -1;
	}

	if(MagicDamageC.SkillGetRequireClass(lpObj->Class,lpObj->ChangeUP,skill) < 1)
	{
		GCServerMsgStringSend ("Wrong class!",lpObj->m_Index,0x01 );
		LogAdd(lMsg.Get(550));
		return -1;
	}

	for(n = 0; n < MAGIC_SIZE; n++)
	{
		if(lpObj->Magic[n].IsMagic() == 1)
		{
			//skill = GetSkillNumberInex(Type,Index,Level);

			//if(skill < 0)
			//{
			//	LogAdd(lMsg.Get(551),__FILE__,__LINE__);
			//	return -1;
			//}

			if(lpObj->Magic[n].m_Skill == skill)
			{
				GCServerMsgStringSend ("Skill Exists!",lpObj->m_Index,0x01 );
				LogAdd(lMsg.Get(552));
				return -1;
			}
			else if (ReadConfig.S6E2 == 0)
			{
				switch(skill)
				{
					case 0x0A:
					{
						if((lpObj->Magic[n].m_Skill>=0x1B8) && (lpObj->Magic[n].m_Skill<=0x1BC)) //New HellFire
						{
							goto Error;
						}
					}break;
					case 0x10:
					{
						if((lpObj->Magic[n].m_Skill>=0x1B3) && (lpObj->Magic[n].m_Skill<=0x1B7)) //New SoulBarrier
						{
							goto Error;
						}
					}break;
					case 0x09:
					{
						if((lpObj->Magic[n].m_Skill>=0x1BD) && (lpObj->Magic[n].m_Skill<=0x1C1)) //New EvilSpirit
						{
							goto Error;
						}else if((lpObj->Magic[n].m_Skill>=0x212) && (lpObj->Magic[n].m_Skill<=0x216)) //New EvilSpirit
						{
							goto Error;
						}
					}break;
					case 0x27:
					{
						if((lpObj->Magic[n].m_Skill>=0x1C2) && (lpObj->Magic[n].m_Skill<=0x1C6)) //New IceStorm
						{
							goto Error;
						}
					}break;
					case 0x29:
					{
						if((lpObj->Magic[n].m_Skill>=0x1EF) && (lpObj->Magic[n].m_Skill<=0x1F3)) //New TwistingSlash
						{
							goto Error;
						}else if((lpObj->Magic[n].m_Skill>=0x1C7) && (lpObj->Magic[n].m_Skill<=0x1CB)) //New TwistingSlash
						{
							goto Error;
						}
					}break;
					case 0x0D:
					{
						if((lpObj->Magic[n].m_Skill>=0x1FE) && (lpObj->Magic[n].m_Skill<=0x202)) //New CometFall
						{
							goto Error;
						}
					}break;
					case 0x37:
					{
						if((lpObj->Magic[n].m_Skill>=0x1F4) && (lpObj->Magic[n].m_Skill<=0x1F8)) //New FireSlash
						{
							goto Error;
						}
					}break;
					case 0x38:
					{
						if((lpObj->Magic[n].m_Skill>=0x1F9) && (lpObj->Magic[n].m_Skill<=0x1FD)) //New PowerSlash
						{
							goto Error;
						}
					}break;
					case 0x1A:
					{
						if((lpObj->Magic[n].m_Skill>=0x1DB) && (lpObj->Magic[n].m_Skill<=0x1DF)) //New Healing
						{
							goto Error;
						}
					}break;
					case 0x1B:
					{
						if((lpObj->Magic[n].m_Skill>=0x1E0) && (lpObj->Magic[n].m_Skill<=0x1E4)) //New Greater Defense
						{
							goto Error;
						}
					}break;
					case 0x1C:
					{
						if((lpObj->Magic[n].m_Skill>=0x1E5) && (lpObj->Magic[n].m_Skill<=0x1E9)) //New Greater Damage
						{
							goto Error;
						}
					}break;
					case 0x18:
					{
						if((lpObj->Magic[n].m_Skill>=0x1EA) && (lpObj->Magic[n].m_Skill<=0x1EE)) //New Triple Shot
						{
							goto Error;
						}
					}break;
					case 0x2B:
					{
						if((lpObj->Magic[n].m_Skill>=0x1CC) && (lpObj->Magic[n].m_Skill<=0x1D0)) //New Death Stab
						{
							goto Error;
						}
					}break;
					case 0x2A:
					{
						if((lpObj->Magic[n].m_Skill>=0x1D1) && (lpObj->Magic[n].m_Skill<=0x1D5)) //New Rageful Blow
						{
							goto Error;
						}
					}break;
					case 0x30:
					{
						if((lpObj->Magic[n].m_Skill>=0x1D6) && (lpObj->Magic[n].m_Skill<=0x1DA)) //Greater Fortitude
						{
							goto Error;
						}
					}break;
					case 0x3D:
					{
						if((lpObj->Magic[n].m_Skill>=0x208) && (lpObj->Magic[n].m_Skill<=0x20C)) //New FireBurst
						{
							goto Error;
						}
					}break;
					case 0x4E:
					{
						if((lpObj->Magic[n].m_Skill>=0x20D) && (lpObj->Magic[n].m_Skill<=0x211)) //New Fire Scream
						{
							goto Error;
						}
					}break;
					case 0x3E:
					{
						if((lpObj->Magic[n].m_Skill>=0x203) && (lpObj->Magic[n].m_Skill<=0x207)) //New Earthshake
						{
							goto Error;
						}
					}break;
					case 0xDB:
					{
						if((lpObj->Magic[n].m_Skill>=0x217) && (lpObj->Magic[n].m_Skill<=0x21B)) //New Sleep
						{
							goto Error;
						}
					}break;
					case 215:
					{
						if((lpObj->Magic[n].m_Skill>=540) && (lpObj->Magic[n].m_Skill<=544)) //New Chain Lighting
						{
							goto Error;
						}
					}break;
					case 230:
					{
						if((lpObj->Magic[n].m_Skill>=545) && (lpObj->Magic[n].m_Skill<=549)) //New Lighting Shock
						{
							goto Error;
						}
					}break;
					case 214:
					{
						if((lpObj->Magic[n].m_Skill>=550) && (lpObj->Magic[n].m_Skill<=554)) //New Drain Life
						{
							goto Error;
						}
					}break;
				}
			}
		}
	}

	skill = -1;

	for(n = 0; n < MAGIC_SIZE; n++)
	{
		if(lpObj->Magic[n].IsMagic() == 0)
		{
			skill = lpObj->Magic[n].Set(Type,Index,Level);
			if(skill < 0)
			{
				LogAdd(lMsg.Get(553),__FILE__,__LINE__);
				return -1;
			}
			else
			{
				lpObj->MagicCount++;
				SkillNumber = skill;
				return n;
			}
		}
	}

	LogAdd(lMsg.Get(554),__FILE__,__LINE__);
	return -1;

Error:
	LogAdd(lMsg.Get(552));
	return -1;
}



void gObjMagicDelAll(LPOBJ lpObj)
{

	for(int n = 0; n < MAGIC_SIZE; n++)
	{
		if(lpObj->Magic[n].IsMagic() == 1)
		{
			lpObj->Magic[n].Clear();
		}
	}
}



int  gObjMagicDel(LPOBJ lpObj, WORD aSkill, BYTE Level)
{

	for(int n = 0; n < MAGIC_SIZE; n++)
	{
		if(lpObj->Magic[n].IsMagic() == 1)
		{
			if(lpObj->Magic[n].m_Skill == aSkill)
			{
				lpObj->Magic[n].Clear();
				return n;
			}
		}
	}
	return -1;
}



BOOL gObjMagicExist(LPOBJ lpObj, WORD Skill, int & SkillPosition)
{
	for(int n = 0; n < MAGIC_SIZE; n++)
	{
		if(lpObj->Magic[n].IsMagic() == 1)
		{
			if(lpObj->Magic[n].m_Skill == Skill)
			{
				SkillPosition = n;
				return 1;
			}
		}
	}
	SkillPosition = -1;
	return 0;
}


int  gObjMagicAddResetCheck(LPOBJ lpObj, WORD aSkill, BYTE Level)
{
	int skill,n,reqeng;

	//skill = -1;
	//if(g_EnergyCheckOff == 1)
	//{
		reqeng = MagicDamageC.SkillGetRequireEnergy(aSkill);

		if(reqeng < 0)
		{
			LogAdd(lMsg.Get(549));
			return -1;
		}

		if((lpObj->Energy + lpObj->AddEnergy) < reqeng)
		{
#if (DSGN_RESET_SYSTEM==0)
#if (PACK_EDITION>=2)
			if((ResetChar.Normal.ClearSkills == 0 && ResetChar.Normal.Enabled == TRUE) || ((ResetChar.Masters.ClearSkills == 0 && ResetChar.Masters.Enabled == TRUE) ))
			{
			}else
#endif
			{
				LogAdd(lMsg.Get(549));
				return -1;
			}
#else
			LogAdd(lMsg.Get(549));
			return -1;
#endif
		}
	//}

	if((MagicDamageC.SkillGetRequireClass(lpObj->Class,lpObj->ChangeUP,aSkill) < 1) && (aSkill != 0xFFFF))
	{
		LogAdd(lMsg.Get(550));
		return -1;
	}

	for(n = 0; n < MAGIC_SIZE; n++)
	{
		if(lpObj->Magic[n].IsMagic() == 1)
		{
			if(lpObj->Magic[n].m_Skill == aSkill)
			{
				LogAdd(lMsg.Get(555),lpObj->Magic[n].m_Skill,aSkill);
				return -1;
			}
		}
	}

	for(n = 0; n < MAGIC_SIZE; n++)
	{
		if(lpObj->Magic[n].IsMagic() == 0)
		{
			skill = lpObj->Magic[n].Set(aSkill,Level);
			if(skill < 0)
			{
				LogAdd(lMsg.Get(553),__FILE__,__LINE__);
				return -1;
			}
			else
			{
				lpObj->MagicCount++;
				return n;
			}
		}
	}

	LogAdd(lMsg.Get(554),__FILE__,__LINE__);
	return -1;
}







int  gObjMagicAdd(LPOBJ lpObj, WORD aSkill, BYTE Level)
{
	int skill,n,reqeng;

	//skill = -1;
	//if(g_EnergyCheckOff == 1)
	//{
		reqeng = MagicDamageC.SkillGetRequireEnergy(aSkill);

		if(reqeng < 0)
		{
			LogAdd(lMsg.Get(549));
			return -1;
		}

		if((lpObj->Energy + lpObj->AddEnergy) < reqeng)
		{
			LogAdd(lMsg.Get(549));
			return -1;
		}
	//}

	if((MagicDamageC.SkillGetRequireClass(lpObj->Class,lpObj->ChangeUP,aSkill) < 1) && (aSkill != 0xFFFF))
	{
		LogAdd(lMsg.Get(550));
		return -1;
	}

	for(n = 0; n < MAGIC_SIZE; n++)
	{
		if(lpObj->Magic[n].IsMagic() == 1)
		{
			if(lpObj->Magic[n].m_Skill == aSkill)
			{
				LogAdd(lMsg.Get(555),lpObj->Magic[n].m_Skill,aSkill);
				return -1;
			}
		}
	}

	for(n = 0; n < MAGIC_SIZE; n++)
	{
		if(lpObj->Magic[n].IsMagic() == 0)
		{
			skill = lpObj->Magic[n].Set(aSkill,Level);
			if(skill < 0)
			{
				LogAdd(lMsg.Get(553),__FILE__,__LINE__);
				return -1;
			}
			else
			{
				lpObj->MagicCount++;
				return n;
			}
		}
	}

	LogAdd(lMsg.Get(554),__FILE__,__LINE__);
	return -1;
}






int gObjWeaponMagicAdd(LPOBJ lpObj, WORD aSkill, BYTE Level)
{
	int skill = -1,n;
	skill = -1;

	if(MagicDamageC.SkillGetRequireClass(lpObj->Class,lpObj->ChangeUP,aSkill) < 1)
	{
		return -1;
	}
	for(n = 0; n < MAGIC_SIZE; n++)
	{
		if(lpObj->Magic[n].IsMagic() == 0)
		{
			skill = lpObj->Magic[n].Set(aSkill,Level);
			if(skill < 0)
			{
				LogAdd(lMsg.Get(553),__FILE__,__LINE__);
				return -1;
			}
			else
			{
				lpObj->MagicCount++;
				return n;
			}
		}
	}
	LogAdd(lMsg.Get(554),__FILE__,__LINE__);
	return -1;
}






int gObjMonsterMagicAdd(LPOBJ lpObj, WORD aSkill, BYTE Level)
{
	int skill = -1;

	for (int n=0; n<MAGIC_SIZE ; n++)
	{
		if ( lpObj->Magic[n].IsMagic() == FALSE )
		{
			skill = lpObj->Magic[n].Set(aSkill, Level);

			if ( skill < 0 )
			{
				LogAdd(lMsg.Get( MSGGET(2, 41)),__FILE__, __LINE__);
				return -1;
			}

			lpObj->MagicCount++;
			return n;
		}
	}

	LogAdd(lMsg.Get( MSGGET(2, 42)), __FILE__, __LINE__);
	return -1;
}






BOOL gObjMagicEnergyCheck(LPOBJ lpObj , WORD aSkill)
{
	int reqeng,reqlevel;

	reqeng = MagicDamageC.SkillGetRequireEnergy(aSkill);

	if(reqeng < 0)
	{
		return false;
	}

	if((lpObj->Energy + lpObj->AddEnergy) < reqeng)
	{
		return false;
	}

	if(ReadConfig.DisableLevelReq_gObjMagicEnergyCheck == 0)
	{
		reqlevel = MagicDamageC.SkillGetRequireLevel(aSkill);

		if(lpObj->Level < reqlevel)
		{
			return false;
		}
	}
	return true;
}


BOOL gObjJewelUpHackCheck(LPOBJ lpObj,int target)
{
	//Restrict: rings, pendants, boxes, etc...
	if ( lpObj->pInventory[target].m_Type == ITEMGET(12,32) )	//Box of red libbon
		return  FALSE;
	if ( lpObj->pInventory[target].m_Type == ITEMGET(12,33) )	//Box of green libbon
		return  FALSE;
	if ( lpObj->pInventory[target].m_Type == ITEMGET(12,34) )	//Box of blue libbob
		return  FALSE;
	if ( lpObj->pInventory[target].m_Type == ITEMGET(13,8) )	//Ring of Ice
		return  FALSE;
	if ( lpObj->pInventory[target].m_Type == ITEMGET(13,9) )	//Ring of Poison
		return  FALSE;
	if ( lpObj->pInventory[target].m_Type == ITEMGET(13,10) )	//Transformation Ring
		return  FALSE;
	if ( lpObj->pInventory[target].m_Type == ITEMGET(13,12) )	//Pendant of Lighting
		return  FALSE;
	if ( lpObj->pInventory[target].m_Type == ITEMGET(13,13) )	//Pendant of Fire
		return  FALSE;
	if ( lpObj->pInventory[target].m_Type == ITEMGET(13,14) )	//Loch's Feather
		return  FALSE;
	if ( lpObj->pInventory[target].m_Type == ITEMGET(13,15) )	//Fruits
		return  FALSE;
	if ( lpObj->pInventory[target].m_Type == ITEMGET(13,16) )	//Scroll of Archangel
		return  FALSE;
	if ( lpObj->pInventory[target].m_Type == ITEMGET(13,17) )	//Blood Bone
		return  FALSE;
	if ( lpObj->pInventory[target].m_Type == ITEMGET(13,18) )	//Invisibility Cloak
		return  FALSE;
	if ( lpObj->pInventory[target].m_Type == ITEMGET(13,20) )	//Wizards Ring
		return  FALSE;
	if ( lpObj->pInventory[target].m_Type == ITEMGET(13,107))	//Lethal Wizards Ring
		return  FALSE;
	if ( lpObj->pInventory[target].m_Type == ITEMGET(13,21) )	//Ring of Fire
		return  FALSE;
	if ( lpObj->pInventory[target].m_Type == ITEMGET(13,22) )	//Ring of Earth
		return  FALSE;
	if ( lpObj->pInventory[target].m_Type == ITEMGET(13,23) )	//Ring of Wind
		return  FALSE;
	if ( lpObj->pInventory[target].m_Type == ITEMGET(13,24) )	//Ring of Magic
		return  FALSE;
	if ( lpObj->pInventory[target].m_Type == ITEMGET(13,25) )	//Pendant of Ice
		return  FALSE;
	if ( lpObj->pInventory[target].m_Type == ITEMGET(13,26) )	//Pendant of Wind�
		return  FALSE;
	if ( lpObj->pInventory[target].m_Type == ITEMGET(13,27) )	//Pendant of Water
		return  FALSE;
	if ( lpObj->pInventory[target].m_Type == ITEMGET(13,28) )	//Pendant of Ability
		return  FALSE;
	if ( lpObj->pInventory[target].m_Type == ITEMGET(13,38) )	//Moonstone Pendant
		return  FALSE;
	if ( lpObj->pInventory[target].m_Type == ITEMGET(13,39) )	//Ring 1
		return  FALSE;
	if ( lpObj->pInventory[target].m_Type == ITEMGET(13,40) )	//Ring 2
		return  FALSE;
	if ( lpObj->pInventory[target].m_Type == ITEMGET(13,41) )	//Ring 3
		return  FALSE;
	if ( lpObj->pInventory[target].m_Type == ITEMGET(13,42) )	//GM Ring
		return  FALSE;
	if ( lpObj->pInventory[target].m_Type == ITEMGET(13,43) )	//Green Symbol
		return  FALSE;
	if ( lpObj->pInventory[target].m_Type == ITEMGET(13,44) )	//Pink Symbol
		return  FALSE;
	if ( lpObj->pInventory[target].m_Type == ITEMGET(13,45) )	//Yellow Symbol
		return  FALSE;
	if ( lpObj->pInventory[target].m_Type == ITEMGET(13,49) )	//Scroll
		return  FALSE;
	if ( lpObj->pInventory[target].m_Type == ITEMGET(13,50) )	//Potion
		return  FALSE;
	if ( lpObj->pInventory[target].m_Type == ITEMGET(13,51) )	//Plataform,
		return  FALSE;
	if ( lpObj->pInventory[target].m_Type == ITEMGET(14,11) )	//Box of Luck
		return  FALSE;
	if ( lpObj->pInventory[target].m_Type == ITEMGET(14,17) )	//Devil's Eye
		return  FALSE;
	if ( lpObj->pInventory[target].m_Type == ITEMGET(14,18) )	//Devil's Key
		return  FALSE;
	if ( lpObj->pInventory[target].m_Type == ITEMGET(14,19) )	//Devil's Invitation
		return  FALSE;
	if ( lpObj->pInventory[target].m_Type == ITEMGET(14,28) )	//Lost Map
		return  FALSE;
	if ( lpObj->pInventory[target].m_Type == ITEMGET(14,29) )	//Symbol of Kundun
		return  FALSE;
	if ( lpObj->pInventory[target].m_Type == ITEMGET(14,32) )	//Pink Chocolate Box
		return  FALSE;
	if ( lpObj->pInventory[target].m_Type == ITEMGET(14,33) )	//Red Chocolate Box
		return  FALSE;
	if ( lpObj->pInventory[target].m_Type == ITEMGET(14,34) )	//Blue Chocolate Box
		return  FALSE;
	//Sacred Lucky Items
	if ( lpObj->pInventory[target].m_Type >= ITEMGET(7,62) && lpObj->pInventory[target].m_Type <= ITEMGET(7,72) )
		return  FALSE;
	if ( lpObj->pInventory[target].m_Type >= ITEMGET(8,62) && lpObj->pInventory[target].m_Type <= ITEMGET(8,72) )
		return  FALSE;
	if ( lpObj->pInventory[target].m_Type >= ITEMGET(9,62) && lpObj->pInventory[target].m_Type <= ITEMGET(9,72) )
		return  FALSE;
	if ( lpObj->pInventory[target].m_Type >= ITEMGET(10,62) && lpObj->pInventory[target].m_Type <= ITEMGET(10,72) )
		return  FALSE;
	if ( lpObj->pInventory[target].m_Type >= ITEMGET(11,62) && lpObj->pInventory[target].m_Type <= ITEMGET(11,72) )
		return  FALSE;

	return TRUE;
}


BOOL gObjItemLuckyDurablityUp(LPOBJ lpObj, int source, int target)
{
	if(source < 0 || source > ReadConfig.MAIN_INVENTORY_SIZE(lpObj->m_Index,false) -1)
	{
		return false;
	}

	if(target < 0 || target > ReadConfig.MAIN_INVENTORY_SIZE(lpObj->m_Index,false) -1)
	{
		return false;
	}

	if(lpObj->pInventory[source].IsItem() == 0)
	{
		return false;
	}

	if(lpObj->pInventory[target].IsItem() == 0)
	{
		return false;
	}

	if( (lpObj->pInventory[target].m_Type >= ITEMGET(7,62) && lpObj->pInventory[target].m_Type <= ITEMGET(7,72))
		|| (lpObj->pInventory[target].m_Type >= ITEMGET(8,62) && lpObj->pInventory[target].m_Type <= ITEMGET(8,72))
		|| (lpObj->pInventory[target].m_Type >= ITEMGET(9,62) && lpObj->pInventory[target].m_Type <= ITEMGET(9,72))
		|| (lpObj->pInventory[target].m_Type >= ITEMGET(10,62) && lpObj->pInventory[target].m_Type <= ITEMGET(10,72))
		|| (lpObj->pInventory[target].m_Type >= ITEMGET(11,62) && lpObj->pInventory[target].m_Type <= ITEMGET(11,72))
		)
	{
	}
	else
	{
		return false;
	}

	LogAddTD("[%s][%s]Before using Jewel of Extension : (%s)serial:(%d)(%d)",lpObj->AccountID,lpObj->Name,lpObj->pInventory[target].GetName(),lpObj->pInventory[target].m_Number,lpObj->pInventory[target].m_JewelOfHarmonyOption);

	lpObj->pInventory[target].m_Durability += lpObj->pInventory[source].m_Durability;

	if(lpObj->pInventory[target].m_Durability > 255.0f)
		lpObj->pInventory[target].m_Durability = 255.0f;
	
	GCItemDurSend(lpObj->m_Index, target, lpObj->pInventory[target].m_Durability, 0);
	
	LogAddTD("[%s][%s]After using Jewel of Extension(%d) : (%s) serial:(%d)(%d)",lpObj->AccountID,lpObj->Name,lpObj->pInventory[source].m_Number,
			lpObj->pInventory[target].GetName(),lpObj->pInventory[target].m_Number,lpObj->pInventory[target].m_JewelOfHarmonyOption);

	return true;
}

BOOL gObjItemLuckyRefineryUp(LPOBJ lpObj, int source, int target)
{
	if(source < 0 || source > ReadConfig.MAIN_INVENTORY_SIZE(lpObj->m_Index,false) -1)
	{
		return false;
	}

	if(target < 0 || target > ReadConfig.MAIN_INVENTORY_SIZE(lpObj->m_Index,false) -1)
	{
		return false;
	}

	if(lpObj->pInventory[source].IsItem() == 0)
	{
		return false;
	}

	if(lpObj->pInventory[target].IsItem() == 0)
	{
		return false;
	}

	if( (lpObj->pInventory[target].m_Type >= ITEMGET(7,62) && lpObj->pInventory[target].m_Type <= ITEMGET(7,72))
		|| (lpObj->pInventory[target].m_Type >= ITEMGET(8,62) && lpObj->pInventory[target].m_Type <= ITEMGET(8,72))
		|| (lpObj->pInventory[target].m_Type >= ITEMGET(9,62) && lpObj->pInventory[target].m_Type <= ITEMGET(9,72))
		|| (lpObj->pInventory[target].m_Type >= ITEMGET(10,62) && lpObj->pInventory[target].m_Type <= ITEMGET(10,72))
		|| (lpObj->pInventory[target].m_Type >= ITEMGET(11,62) && lpObj->pInventory[target].m_Type <= ITEMGET(11,72))
		)
	{
	}
	else
	{
		return false;
	}

	LogAddTD("[%s][%s]Before using Jewel of Elevation : (%s)serial:(%d)(%d)",lpObj->AccountID,lpObj->Name,lpObj->pInventory[target].GetName(),lpObj->pInventory[target].m_Number,lpObj->pInventory[target].m_JewelOfHarmonyOption);

	int HarmonyOpt = 0;
	if(lpObj->pInventory[target].m_JewelOfHarmonyOption == 0)
	{
		if(lpObj->pInventory[target].m_Level >= 0 && lpObj->pInventory[target].m_Level <= 2)
		{
			HarmonyOpt = 1;
		}else if(lpObj->pInventory[target].m_Level >= 3 && lpObj->pInventory[target].m_Level <= 5)
		{
			srand(static_cast<int>(time(NULL)));
			HarmonyOpt = rand()%3 + 1;
		}else if(lpObj->pInventory[target].m_Level >= 6 && lpObj->pInventory[target].m_Level <= 8)
		{
			srand(static_cast<int>(time(NULL)));
			HarmonyOpt = rand()%4 + 1;
		}else if(lpObj->pInventory[target].m_Level >= 9 && lpObj->pInventory[target].m_Level <= 12)
		{
			srand(static_cast<int>(time(NULL)));
			HarmonyOpt = rand()%7 + 1;
		}else
		{
			srand(static_cast<int>(time(NULL)));
			HarmonyOpt = rand()%8 + 1;
		}
	}else
	{
		return false;
	}

	lpObj->pInventory[target].m_JewelOfHarmonyOption = AT_JEWELOFHARMONY_NOT_STRENGTHEN_ITEM;
	lpObj->pInventory[target].m_JewelOfHarmonyOption |= HarmonyOpt << 4;
	lpObj->pInventory[target].m_JewelOfHarmonyOption |= lpObj->pInventory[target].m_Level & 0x0F;

	gObjMakePreviewCharSet(lpObj->m_Index);

	lpObj->pInventory[target].Convert(
			lpObj->pInventory[target].m_Type,
			lpObj->pInventory[target].m_SkillOption,
			lpObj->pInventory[target].m_LuckOption,
			lpObj->pInventory[target].m_Z28Option,
			lpObj->pInventory[target].m_NewOption,
			lpObj->pInventory[target].m_SetOption,
			lpObj->pInventory[target].m_ItemOptionEx,
			CURRENT_DB_VERSION);

	LogAddTD("[%s][%s]After using Jewel of Elevation(%d) : (%s) serial:(%d)(%d)",lpObj->AccountID,lpObj->Name,lpObj->pInventory[source].m_Number,
			lpObj->pInventory[target].GetName(),lpObj->pInventory[target].m_Number,lpObj->pInventory[target].m_JewelOfHarmonyOption);
	return true;
}

BOOL gObjItemLevelUp(LPOBJ lpObj, int source, int target)
{
	if(source < 0 || source > ReadConfig.MAIN_INVENTORY_SIZE(lpObj->m_Index,false) -1)
	{
		return false;
	}

	if(target < 0 || target > ReadConfig.MAIN_INVENTORY_SIZE(lpObj->m_Index,false) -1)
	{
		return false;
	}

	if(lpObj->pInventory[source].IsItem() == 0)
	{
		return false;
	}

	if(lpObj->pInventory[target].IsItem() == 0)
	{
		return false;
	}

	if (!gObjJewelUpHackCheck(lpObj,target))
	{
		GCServerMsgStringSend("[Anti-Hack] Shadow Bug Attempt!",lpObj->m_Index, 0x01);
		if(ReadConfig.AHLog == TRUE)
		{
			ANTI_HACK_LOG.Output("[Anti-Hack][Jewel of Bless][%s][%s] Shadow Bug Attempt, Item: %d",
				lpObj->AccountID,lpObj->Name,lpObj->pInventory[target].m_Type);
		}
		return false;
	}

	if(lpObj->pInventory[target].m_Type == ITEMGET(13,37))
	{
		CItem * ItemFenrir = &lpObj->pInventory[target];

		if(ItemFenrir->m_Durability >= 255)
		{
			return false;
		}

		if(rand()% 10000 < g_iFenrirRepairRate)
		{
			int iAddDur = rand()%150 + 50;

			if((ItemFenrir->m_Durability + iAddDur) > 255)
			{
				ItemFenrir->m_Durability = 255.0f;
			}
			else
			{
				ItemFenrir->m_Durability += iAddDur;
			}

			MsgOutput(lpObj->m_Index,lMsg.Get(3342),int(ItemFenrir->m_Durability));

			LogAddTD("[FENRIR REPAIR][SUCCESS] [%s][%s] - %d/255 (+%d)",lpObj->AccountID,lpObj->Name,ItemFenrir->m_Durability,iAddDur);
		}
		else
		{
			MsgOutput(lpObj->m_Index,lMsg.Get(3343));

			LogAddTD("[FENRIR REPAIR][FAILED] [%s][%s] - %d/255 (+%d)",lpObj->AccountID,lpObj->Name,ItemFenrir->m_Durability);
		}

		return true;
	}

	if(!(lpObj->pInventory[target].m_Type < ITEMGET(12,7)
		|| lpObj->pInventory[target].m_Type == ITEMGET(13,30)
		|| lpObj->pInventory[target].m_Type == ITEMGET(4,7)
		|| lpObj->pInventory[target].m_Type == ITEMGET(4,15)
		|| (lpObj->pInventory[target].m_Type >= ITEMGET(12,36) && lpObj->pInventory[target].m_Type <= ITEMGET(12,43)) 
		|| (lpObj->pInventory[target].m_Type >= ITEMGET(12,49) && lpObj->pInventory[target].m_Type <= ITEMGET(12,50)) 
#if (CRYSTAL_EDITION == 1)
			|| (lpObj->pInventory[target].m_Type >= ITEMGET(12,200) && lpObj->pInventory[target].m_Type <= ITEMGET(12,254))
#endif
			) )
	{
		return false;
	}

	LogAddTD(lMsg.Get(556),lpObj->AccountID,lpObj->Name,lpObj->pInventory[target].GetName(),lpObj->pInventory[target].m_Number,lpObj->pInventory[target].m_Level);

	lpObj->pInventory[target].m_Level++;

	if(lpObj->pInventory[target].m_Level > 6)
	{
		lpObj->pInventory[target].m_Level = 6;
	}

	gObjMakePreviewCharSet(lpObj->m_Index);

	float levelitemdur = ItemGetDurability(lpObj->pInventory[target].m_Type,lpObj->pInventory[target].m_Level,lpObj->pInventory[target].IsExtItem(),lpObj->pInventory[target].IsSetItem());

	lpObj->pInventory[target].m_Durability = levelitemdur * lpObj->pInventory[target].m_Durability / lpObj->pInventory[target].m_BaseDurability;

	if(lpObj->pInventory[target].m_Durability > 255.0f)
		lpObj->pInventory[target].m_Durability = 255.0f;

	lpObj->pInventory[target].Convert(
			lpObj->pInventory[target].m_Type,
			lpObj->pInventory[target].m_SkillOption,
			lpObj->pInventory[target].m_LuckOption,
			lpObj->pInventory[target].m_Z28Option,
			lpObj->pInventory[target].m_NewOption,
			lpObj->pInventory[target].m_SetOption,
			lpObj->pInventory[target].m_ItemOptionEx,
			CURRENT_DB_VERSION);

	LogAddTD(lMsg.Get(557),lpObj->AccountID,lpObj->Name,lpObj->pInventory[source].m_Number,
			lpObj->pInventory[target].GetName(),lpObj->pInventory[target].m_Number,lpObj->pInventory[target].m_Level);
	return true;
}


BOOL gObjItemRandomLevelUp(LPOBJ lpObj, int source, int target)
{
	if(source < 0 || source > ReadConfig.MAIN_INVENTORY_SIZE(lpObj->m_Index,false) -1)
	{
		return false;
	}

	if(target < 0 || target > ReadConfig.MAIN_INVENTORY_SIZE(lpObj->m_Index,false) -1)
	{
		return false;
	}

	if(lpObj->pInventory[source].IsItem() == 0)
	{
		return false;
	}

	if(lpObj->pInventory[target].IsItem() == 0)
	{
		return false;
	}

	if (!gObjJewelUpHackCheck(lpObj,target))
	{
		GCServerMsgStringSend("[Anti-Hack] Shadow Bug Attempt!",lpObj->m_Index, 0x01);
		if(ReadConfig.AHLog == TRUE)
		{
			ANTI_HACK_LOG.Output("[Anti-Hack][Jewel of Soul][%s][%s] Shadow Bug Attempt, Item: %d",
				lpObj->AccountID,lpObj->Name,lpObj->pInventory[target].m_Type);
		}
		return false;
	}

	if(!(lpObj->pInventory[target].m_Type < ITEMGET(12,7)
		|| lpObj->pInventory[target].m_Type == ITEMGET(13,30)
		|| lpObj->pInventory[target].m_Type == ITEMGET(4,7)
		|| lpObj->pInventory[target].m_Type == ITEMGET(4,15)
		|| lpObj->pInventory[target].m_Type >= ITEMGET(12,36) && lpObj->pInventory[target].m_Type <= ITEMGET(12,43)
		|| lpObj->pInventory[target].m_Type >= ITEMGET(12,49) && lpObj->pInventory[target].m_Type <= ITEMGET(12,50)
#if (CRYSTAL_EDITION == 1)
			|| (lpObj->pInventory[target].m_Type >= ITEMGET(12,200) && lpObj->pInventory[target].m_Type <= ITEMGET(12,254))
#endif
			))
	{
		return false;
	}


	if(lpObj->pInventory[target].m_Level >= 9)
	{
		return false;
	}

	LogAddTD(lMsg.Get(558),
		lpObj->AccountID,lpObj->Name,lpObj->pInventory[target].GetName(),
		lpObj->pInventory[target].m_Number,lpObj->pInventory[target].m_Level);

	int _r = rand()%100;

	if(lpObj->pInventory[target].m_LuckOption)
	{
		if(_r < ReadConfig.gObjItemRandomLevelUpSoulLuck)
		{
			lpObj->pInventory[target].m_Level++;
		}
		else
		{
			if(lpObj->pInventory[target].m_Level >= 7)
			{
				lpObj->pInventory[target].m_Level = 0;
			}
			else
			{
				lpObj->pInventory[target].m_Level--;

				if(lpObj->pInventory[target].m_Level < 0)
				{
					lpObj->pInventory[target].m_Level = 0;
				}
			}
		}
	}
	else
	{
		if(_r < ReadConfig.gObjItemRandomLevelUpSoulNoLuck)
		{
			if(lpObj->pInventory[target].m_Level >= 7)
			{
				lpObj->pInventory[target].m_Level = 0;
			}
			else
			{
				lpObj->pInventory[target].m_Level--;

				if(lpObj->pInventory[target].m_Level < 0)
				{
					lpObj->pInventory[target].m_Level = 0;
				}
			}
		}
		else
		{
			lpObj->pInventory[target].m_Level++;
		}
	}

	if(lpObj->pInventory[target].m_Level > 9)
	{
		lpObj->pInventory[target].m_Level = 9;
	}
	else
	{
		gObjMakePreviewCharSet(lpObj->m_Index);
	}


	//#if (WL_PROTECT==1)  
	//	VM_START_WITHLEVEL(19)
	//	int MyCheckVar1;
	//	CHECK_REGISTRATION(MyCheckVar1, 0x67183817)  
	//	if (MyCheckVar1 != 0x67183817)
	//	{	
	//		ReadConfig.Max_Normal_Level = 27;
	//	}
	//	VM_END
	//#endif

	float levelitemdur = ItemGetDurability(lpObj->pInventory[target].m_Type,lpObj->pInventory[target].m_Level,lpObj->pInventory[target].IsExtItem(),lpObj->pInventory[target].IsSetItem());

	lpObj->pInventory[target].m_Durability = levelitemdur * lpObj->pInventory[target].m_Durability / lpObj->pInventory[target].m_BaseDurability;

	if(lpObj->pInventory[target].m_Durability > 255.0f)
		lpObj->pInventory[target].m_Durability = 255.0f;

	lpObj->pInventory[target].Convert(
			lpObj->pInventory[target].m_Type,
			lpObj->pInventory[target].m_SkillOption,
			lpObj->pInventory[target].m_LuckOption,
			lpObj->pInventory[target].m_Z28Option,
			lpObj->pInventory[target].m_NewOption,
			lpObj->pInventory[target].m_SetOption,
			lpObj->pInventory[target].m_ItemOptionEx,
			CURRENT_DB_VERSION);

	LogAddTD(lMsg.Get(559),lpObj->AccountID,lpObj->Name,lpObj->pInventory[source].m_Number,
			lpObj->pInventory[target].GetName(),lpObj->pInventory[target].m_Number,lpObj->pInventory[target].m_Level);

	if(g_kJewelOfHarmonySystem.IsStrengthenByJewelOfHarmony(&lpObj->pInventory[target])== 1)
	{
		if(g_kJewelOfHarmonySystem.IsActive(&lpObj->pInventory[target]) == 0)
		{
			GCServerMsgStringSend(lMsg.Get(3370),lpObj->m_Index,1);
		}
	}

	return true;
}


BOOL gObjItemRandomOption3Up(LPOBJ lpObj, int source, int target)
{
	if(source < 0 || source > ReadConfig.MAIN_INVENTORY_SIZE(lpObj->m_Index,false) -1)
	{
		return false;
	}

	if(target < 0 || target > ReadConfig.MAIN_INVENTORY_SIZE(lpObj->m_Index,false) -1)
	{
		return false;
	}

	if(lpObj->pInventory[source].IsItem() == 0)
	{
		return false;
	}

	if(lpObj->pInventory[target].IsItem() == 0)
	{
		return false;
	}

	if(!(lpObj->pInventory[target].m_Type < ITEMGET(12,7)
		|| lpObj->pInventory[target].m_Type == ITEMGET(13,30)
		|| lpObj->pInventory[target].m_Type == ITEMGET(4,7)
		|| lpObj->pInventory[target].m_Type == ITEMGET(4,15)
		|| lpObj->pInventory[target].m_Type >= ITEMGET(12,36) && lpObj->pInventory[target].m_Type <= ITEMGET(12,43)
		|| lpObj->pInventory[target].m_Type >= ITEMGET(12,49) && lpObj->pInventory[target].m_Type <= ITEMGET(12,50)
#if (CRYSTAL_EDITION == 1)
		|| (lpObj->pInventory[target].m_Type >= ITEMGET(12,200) && lpObj->pInventory[target].m_Type <= ITEMGET(12,254))
#endif
			))
	{
		return false;
	}

	if (!gObjJewelUpHackCheck(lpObj,target))
	{
		GCServerMsgStringSend("[Anti-Hack] Shadow Bug Attempt!",lpObj->m_Index, 0x01);
		if(ReadConfig.AHLog == TRUE)
		{
			ANTI_HACK_LOG.Output("[Anti-Hack][Jewel of Life][%s][%s] Shadow Bug Attempt, Item: %d",
				lpObj->AccountID,lpObj->Name,lpObj->pInventory[target].m_Type);
		}
		return false;
	}

	LogAddTD(lMsg.Get(576),
		lpObj->AccountID,lpObj->Name,
		lpObj->pInventory[target].GetName(),
		lpObj->pInventory[target].m_Number,
		lpObj->pInventory[target].m_Z28Option);

	int _r = rand()%100;

	if(lpObj->pInventory[target].m_Z28Option == 0)
	{
		if((lpObj->pInventory[target].m_Type >= ITEMGET(12,3) && lpObj->pInventory[target].m_Type <= ITEMGET(12,6)) 
			|| (lpObj->pInventory[target].m_Type >= ITEMGET(12,36) && lpObj->pInventory[target].m_Type <= ITEMGET(12,40))
			|| (lpObj->pInventory[target].m_Type >= ITEMGET(12,42) && lpObj->pInventory[target].m_Type <= ITEMGET(12,43))
			|| (lpObj->pInventory[target].m_Type == ITEMGET(12,50))
#if (CRYSTAL_EDITION == 1)
			|| (lpObj->pInventory[target].m_Type >= ITEMGET(12,200) && lpObj->pInventory[target].m_Type <= ITEMGET(12,254))
#endif
			)
		{
			lpObj->pInventory[target].m_NewOption &= 0xDF;

			if(rand()%2)
			{
				lpObj->pInventory[target].m_NewOption |= 0x20;
			}
		}
	}

	if(lpObj->pInventory[target].m_Z28Option < 7)
	{
		if(_r < ReadConfig.gObjItemRandomOption3UpLife)
		{
			lpObj->pInventory[target].m_Z28Option++;
		}
		else
		{
			lpObj->pInventory[target].m_Z28Option = 0;
		}
	}
	else
	{
		return false;
	}

	gObjMakePreviewCharSet(lpObj->m_Index);

	float levelitemdur = ItemGetDurability(lpObj->pInventory[target].m_Type,lpObj->pInventory[target].m_Level,lpObj->pInventory[target].IsExtItem(),lpObj->pInventory[target].IsSetItem());

	lpObj->pInventory[target].m_Durability = levelitemdur * lpObj->pInventory[target].m_Durability / lpObj->pInventory[target].m_BaseDurability;

	if(lpObj->pInventory[target].m_Durability > 255.0f)
		lpObj->pInventory[target].m_Durability = 255.0f;

	lpObj->pInventory[target].Convert(
			lpObj->pInventory[target].m_Type,
			lpObj->pInventory[target].m_SkillOption,
			lpObj->pInventory[target].m_LuckOption,
			lpObj->pInventory[target].m_Z28Option,
			lpObj->pInventory[target].m_NewOption,
			lpObj->pInventory[target].m_SetOption,
			lpObj->pInventory[target].m_ItemOptionEx,
			CURRENT_DB_VERSION);

	LogAddTD(lMsg.Get(577),lpObj->AccountID,lpObj->Name,lpObj->pInventory[source].m_Number,
			lpObj->pInventory[target].GetName(),lpObj->pInventory[target].m_Number,lpObj->pInventory[target].m_Z28Option);

	unsigned char NewOption[8];

	ItemIsBufExOption(NewOption,&lpObj->pInventory[target]);

	LogAddTD("[%s][%s] Jewel of Life Result [%d,%s,%d,%d,%d,%d] serial:[%d] dur:[%d] Ex:[%d,%d,%d,%d,%d,%d,%d]",
			lpObj->AccountID,lpObj->Name,target,lpObj->pInventory[target].GetName(),
			lpObj->pInventory[target].m_Level,lpObj->pInventory[target].m_SkillOption,lpObj->pInventory[target].m_LuckOption,
			lpObj->pInventory[target].m_Z28Option,lpObj->pInventory[target].m_Number,
			BYTE(lpObj->pInventory[target].m_Durability),NewOption[0],NewOption[1],NewOption[2],NewOption[3],
			NewOption[4],NewOption[5],NewOption[6]);

	return true;
}






void gObjAbilityReSet(LPOBJ lpObj)
{
	lpObj->LevelUpPoint = (lpObj->Level - 1) * 5;

	lpObj->Strength = DCInfo.DefClass[lpObj->Class].Strength;
	lpObj->Dexterity = DCInfo.DefClass[lpObj->Class].Dexterity;
	lpObj->Vitality = DCInfo.DefClass[lpObj->Class].Vitality;
	lpObj->Energy = DCInfo.DefClass[lpObj->Class].Energy;

	lpObj->MaxLife = DCInfo.DefClass[lpObj->Class].MaxLife;
	lpObj->MaxMana = DCInfo.DefClass[lpObj->Class].MaxMana;

	if(lpObj->Class == CLASS_WIZARD || lpObj->Class == CLASS_SUMMONER)
	{
		lpObj->MaxLife += (lpObj->Level -1)*0.5f;
		lpObj->MaxMana += 2.0f * (lpObj->Level -1);

		lpObj->Life = lpObj->MaxLife;
		lpObj->Mana = lpObj->MaxMana;
	}
	else
	{
		lpObj->MaxLife += 2.0f * (lpObj->Level - 1);
		lpObj->MaxMana += (lpObj->Level - 1)*0.5f;

		lpObj->Life = lpObj->MaxLife;
		lpObj->Mana = lpObj->MaxMana;
	}

	LogAddC(2,"[gObjAbilityReSet] Index: %d Time: %d",lpObj->m_Index,lpObj->CheckSumTime);
	CloseClient(lpObj->m_Index);
}






BOOL gObjTargetGuildWarCheck(LPOBJ lpObj, LPOBJ lpTargetObj)
{
	if(lpObj->Type != OBJ_USER)
	{
		return false;
	}

	if(lpTargetObj->Type != OBJ_USER)
	{
		return false;
	}

	if(lpObj->GuildNumber < 1 || lpTargetObj->GuildNumber < 1)
	{
		return false;
	}

	if(lpObj->lpGuild->WarState != 1 || lpTargetObj->lpGuild->WarState != 1)
	{
		return false;
	}

	if(strcmp(lpObj->lpGuild->TargetGuildName,lpTargetObj->lpGuild->Name))
	{
		return false;
	}
	return true;
}






void gObjGuildWarEndSend(LPOBJ lpObj, BYTE Result1, BYTE Result2)
{
	__try
	{

	if(lpObj == 0)
	{
		LogAdd("error-L3 : %s %d",__FILE__,__LINE__);
		return;
	}

	if(lpObj->lpGuild == 0)
	{
		LogAdd("error-L3 : %s %d",__FILE__,__LINE__);
		return;
	}

	if(lpObj->lpGuild->WarState != 1)
	{
		LogAdd("error-L3 : %s %d",__FILE__,__LINE__);
		return;
	}

	if(lpObj->lpGuild->lpTargetGuildNode == 0)
	{
		LogAdd("error-L3 : %s %d",__FILE__,__LINE__);
		return;
	}

	if ( lpObj->Type != OBJ_USER || lpObj->Connected <= PLAYER_LOGGED )
	{
		LogAdd("error-L3 : %s %d",__FILE__,__LINE__);
		return;
	}

	if(Result1 == 1)
	{
		gObjGuildWarItemGive(lpObj->lpGuild,lpObj->lpGuild->lpTargetGuildNode);
	}
	else if(Result1 == 3)
	{
		gObjGuildWarItemGive(lpObj->lpGuild->lpTargetGuildNode,lpObj->lpGuild);
	}
	else if(Result1 == 1)
	{
		gObjGuildWarItemGive(lpObj->lpGuild->lpTargetGuildNode,lpObj->lpGuild);
	}

	for(int n = 0; n < MAX_USER_GUILD;n++)
	{
		if(lpObj->lpGuild->Use[n] && lpObj->lpGuild->Index[n] >= 0)
		{
			gObj[lpObj->lpGuild->Index[n]].IsInBattleGround = 0;
			GCGuildWarEnd(lpObj->lpGuild->Index[n],Result1,lpObj->lpGuild->lpTargetGuildNode->Name);
		}
	}

	for(int n = 0; n < MAX_USER_GUILD;n++)
	{
		if(lpObj->lpGuild->lpTargetGuildNode->Use[n] && lpObj->lpGuild->lpTargetGuildNode->Index[n] >= 0)
		{
			gObj[lpObj->lpGuild->lpTargetGuildNode->Index[n]].IsInBattleGround = 0;
			GCGuildWarEnd(lpObj->lpGuild->lpTargetGuildNode->Index[n],Result2,lpObj->lpGuild->Name);
		}
	}
	
	}__except( EXCEPTION_ACCESS_VIOLATION == GetExceptionCode() )
	{
	}
}

void gObjGuildWarEndSend(_GUILD_INFO_STRUCT * lpGuild1, _GUILD_INFO_STRUCT * lpGuild2, BYTE Result1, BYTE Result2)
{
	if(lpGuild1 == 0 || lpGuild2 == 0)
	{
		return;
	}

	if(lpGuild1->WarState != 1 || lpGuild2->WarState != 1)
	{
		return;
	}

	for(int n = 0; n < MAX_USER_GUILD;n++)
	{
		if(lpGuild1->Use[n] && lpGuild1->Index[n] >= 0)
		{
			gObj[lpGuild1->Index[n]].IsInBattleGround = 0;
			GCGuildWarEnd(lpGuild1->Index[n],Result1,lpGuild2->Name);
		}
	}

	for(int n = 0; n < MAX_USER_GUILD;n++)
	{
		if(lpGuild2->Use[n] && lpGuild2->Index[n] >= 0)
		{
			gObj[lpGuild2->Index[n]].IsInBattleGround = 0;
			GCGuildWarEnd(lpGuild2->Index[n],Result2,lpGuild1->Name);
		}
	}
}






void gObjGuildWarEnd(_GUILD_INFO_STRUCT * lpGuild, _GUILD_INFO_STRUCT * lpTargetGuild)
{
	if(lpGuild == 0 || lpTargetGuild == 0)
	{
		return;
	}

	lpGuild->WarDeclareState = 0;
	lpGuild->WarState = 0;

	if(lpTargetGuild != 0)
	{
		lpTargetGuild->WarDeclareState = 0;
		lpTargetGuild->WarState = 0;

		if(lpGuild->lpTargetGuildNode != 0)
		{
			_GUILD_INFO_STRUCT * lpTguild = lpGuild->lpTargetGuildNode;

			lpGuild->lpTargetGuildNode = 0;
			lpTguild->lpTargetGuildNode = 0;
		}
	}
	else
	{
		lpGuild->lpTargetGuildNode = 0;
	}
}






BOOL gObjGuildWarProc(_GUILD_INFO_STRUCT * lpGuild1, _GUILD_INFO_STRUCT * lpGuild2, int score)
{
	int totalscore;
	int n;
	int maxscore;
	char szTemp[0x100];

	if(lpGuild1 == 0 || lpGuild2 == 0)
	{
		return false;
	}

	totalscore = 1;
	maxscore = 20;

	lpGuild1->PlayScore += score;

	wsprintf(szTemp,"%s ( %d ) VS %s ( %d )",lpGuild1->Name,lpGuild1->PlayScore,lpGuild2->Name,lpGuild2->PlayScore);
	LogAddTD(szTemp);

	if(lpGuild1->WarType == 1)
	{
		SetBattleTeamScore(lpGuild1->BattleGroundIndex,lpGuild1->BattleTeamCode,lpGuild1->PlayScore);
		maxscore = 100;
	}
	else
	{
		maxscore = 20;
	}

	cManager.BattleInfoSend(GetBattleTeamName(0,0),GetBattleTeamScore(0,0),GetBattleTeamName(0,1),GetBattleTeamScore(0,1));

	if(lpGuild1->PlayScore >= maxscore)
	{
		if(lpGuild1->PlayScore > maxscore && lpGuild2->PlayScore == 0)
		{
			totalscore = 3;
		}
		else if(lpGuild1->PlayScore > maxscore && lpGuild2->PlayScore <= 10)
		{
			totalscore = 2;
		}

		lpGuild1->TotalScore += totalscore;
		DGGuildScoreUpdate(lpGuild1->Name,lpGuild1->TotalScore);
		DGGuildScoreUpdate(lpGuild2->Name,lpGuild2->TotalScore);

		return true;
	}

	for(n = 0; n < MAX_USER_GUILD; n++)
	{
		if(lpGuild1->Use[n] && lpGuild1->Index[n] >= 0)
		{
			GCGuildWarScore(lpGuild1->Index[n]);
		}
	}

	for(n = 0; n < MAX_USER_GUILD; n++)
	{
		if(lpGuild2->Use[n] && lpGuild2->Index[n] >= 0)
		{
			GCGuildWarScore(lpGuild2->Index[n]);
		}
	}
	return false;
}






BOOL gObjGuildWarCheck(LPOBJ lpObj, LPOBJ lpTargetObj)
{
	int score = 1;

	if(gObjTargetGuildWarCheck(lpObj,lpTargetObj)==0)
	{
		return false;
	}

	if(strcmp(lpTargetObj->Name,lpTargetObj->lpGuild->Names[0])==0)
	{
		score = 2;
	}

	_GUILD_INFO_STRUCT * lpGuild = lpObj->lpGuild;
	_GUILD_INFO_STRUCT * lpTargetGuild = lpTargetObj->lpGuild;

	if(gObjGuildWarProc(lpGuild,lpTargetGuild,score)==1)
	{
		if(lpGuild->WarType == 1)
		{
			gObjGuildWarEndSend(lpGuild,lpTargetGuild,1,0);
			gBattleGroundEnable(lpGuild->BattleGroundIndex,0);
			gObjGuildWarEnd(lpGuild,lpTargetGuild);
			cManager.BattleInfoSend(GetBattleTeamName(0,0),255,GetBattleTeamName(0,1),255);
		}
		else
		{
			gObjGuildWarEndSend(lpObj,1,0);
			gObjAddMsgSendDelay(lpObj,4,lpObj->m_Index,2000,0);
		}
	}
	return true;
}






BOOL gObjGuildWarMasterClose(LPOBJ lpObj)
{
	if(lpObj->GuildNumber < 1)
	{
		return false;
	}

	if(strcmp(lpObj->lpGuild->Names[0],lpObj->Name))
	{
		return false;
	}

	gObjGuildWarEndSend(lpObj,3,2);

	if(lpObj->lpGuild->WarType == 1 && lpObj->lpGuild->BattleGroundIndex < 1)
	{
		gBattleGroundEnable(lpObj->lpGuild->BattleGroundIndex,0);
		cManager.BattleInfoSend(GetBattleTeamName(0,0),255,GetBattleTeamName(0,1),255);
	}
	gObjGuildWarEnd(lpObj->lpGuild,lpObj->lpGuild->lpTargetGuildNode);
	return true;
}






int gObjGuildWarItemGive(_GUILD_INFO_STRUCT * lpWinGuild, _GUILD_INFO_STRUCT * lpLoseGuild)
{
	return true;
	int n;
	int r_userindex[MAX_USER_GUILD];
	int r_usercount;

	for(n = 0; n < MAX_USER_GUILD; n++)
	{
		if(lpLoseGuild->Use[n] > 0)
		{
			r_userindex[r_usercount] = lpLoseGuild->Index[n];
			r_usercount++;
		}
	}

	if(r_usercount < 1)
	{
		LogAdd(lMsg.Get(560),__FILE__,__LINE__);
		return false;
	}

	int lose_user = r_userindex[rand()%r_usercount];

	if(lose_user < 0)
	{
		LogAdd(lMsg.Get(560),__FILE__,__LINE__);
		return false;
	}

	if(gObj[lose_user].Connected < PLAYER_PLAYING)
	{
		LogAdd(lMsg.Get(560),__FILE__,__LINE__);
		return false;
	}

	r_usercount = 0;

	for(n = 0; n < MAX_USER_GUILD;n++)
	{
		if(lpWinGuild->Use[n] > 0)
		{
			r_userindex[r_usercount] = lpWinGuild->Index[n];
			r_usercount++;
		}
	}

	if(r_usercount < 1)
	{
		LogAdd(lMsg.Get(561));
		return false;
	}

	int win_user = r_userindex[rand()%r_usercount];

	if(win_user < 0)
	{
		LogAdd(lMsg.Get(561));
		return false;
	}

	if(gObj[win_user].Connected < PLAYER_PLAYING)
	{
		LogAdd(lMsg.Get(561));
		return false;
	}

	LPOBJ lpObj = &gObj[lose_user];

	int count = 24;

	int number;

	while(count--)
	{
		number = rand()%INVETORY_WEAR_SIZE;

		if(lpObj->pInventory[number].IsItem()==1)
		{
			if(gObjInventoryInsertItem(win_user,lpObj->pInventory[number]) != 0xFF)
			{
				return true;
			}
			break;
		}
	}

	count = 64;

	while(count--)
	{
		number = rand()%BASIC_INVENTORY_SIZE+INVETORY_WEAR_SIZE;

		if(lpObj->pInventory[number].IsItem()==1)
		{
			if(gObjInventoryInsertItem(win_user,lpObj->pInventory[number]) != 0xFF)
			{
				return true;
			}
			break;
		}
	}
	return true;
}


BOOL gObjMonsterCall(int aIndex, int MonsterType, int x, int y)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return false;
	}

	if(gObj[aIndex].MapNumber == MAP_INDEX_ICARUS)
	{
		return false;
	}

	if(gObj[aIndex].m_RecallMon >= 0)
	{

		GCRecallMonLife(aIndex,60,0);
		gObjMonsterCallKill(aIndex);
		return false;
	}

	int result = gObjAddCallMon();

	if(result >= 0)
	{
		gObj[result].X = x;
		gObj[result].Y = y;

		gObj[result].MTX = x;
		gObj[result].MTY = y;
		gObj[result].Dir = 2;

		gObj[result].MapNumber = gObj[aIndex].MapNumber;

		gObjSetMonster(result,MonsterType,"gObjMonsterCall");

		gObj[result].m_RecallMon = aIndex;
		gObj[result].m_Attribute = 100;

		gObj[result].TargetNumber = -1;
		gObj[result].m_ActState.Emotion = 0;
		gObj[result].m_ActState.Attack = 0;
		gObj[result].m_ActState.EmotionCount = 0;

		gObj[result].PathCount = 0;

		gObj[aIndex].m_RecallMon = result;
		gObj[result].m_MoveRange = 15;

		GCRecallMonLife(gObj[result].m_RecallMon,gObj[result].MaxLife,gObj[result].Life);
		LogAddTD("[Summon Monster] [%s][%s] Try to Summon Monster - Succeed (SummonIndex:%d)",gObj[aIndex].AccountID,gObj[aIndex].Name,result);
		return true;
	}
	return false;
}






void gObjMonsterCallKill(int aIndex)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	LogAddTD("[Summon Monster] [%s][%s] Try to Kill Summoned Monster",gObj[aIndex].AccountID,gObj[aIndex].Name);

	if(gObj[aIndex].m_RecallMon < 0)
	{
		return;
	}

	int callmon = gObj[aIndex].m_RecallMon;

	if(callmon < 0 || callmon > OBJMAX -1 )
	{
		return;
	}

	if(gObj[callmon].Type != OBJ_MONSTER)
	{
		return;
	}

	if(gObj[callmon].m_RecallMon != aIndex)
	{
		return;
	}

	if(gObj[aIndex].m_RecallMon >= 0)
	{
		if(gObjIsConnected(gObj[aIndex].m_RecallMon)==1)
		{
			LogAddTD("[Summon Monster] [%s][%s] Try to Kill Summoned Monster - Found Summoned Monster (SummonIndex:%d)",gObj[aIndex].AccountID,gObj[aIndex].Name,gObj[aIndex].m_RecallMon);
			gObj[gObj[aIndex].m_RecallMon].m_RecallMon = -1;
			gObj[callmon].DieRegen = FALSE;
		}
#if (PACK_EDITION>=3)
		if(gObj[callmon].IsBot == 1)
		{
			LogAddTD("[BotPet] [%s][%s] Remove BotPet (BotPet Index:%d)",gObj[aIndex].AccountID,gObj[aIndex].Name,gObj[aIndex].m_RecallMon);
			gObjCharZeroSet(callmon);
		}
#endif
	}
	gObjCallMonCount--; //IcaruS FIX Summon Shit
	gObj[aIndex].m_RecallMon = -1;
	gObjDel(callmon);

	LogAddTD("[Summon Monster] [%s][%s] Try to Kill Summoned Monster - Finished to Kill Summoned Monster (SummonIndex:%d)",gObj[aIndex].AccountID,gObj[aIndex].Name,callmon);
}






BOOL gObjCheckTeleportArea(int aIndex, BYTE x, BYTE y)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return false;
	}

	if(x < (gObj[aIndex].X - 8)
		|| x > (gObj[aIndex].X + 8)
		|| y < (gObj[aIndex].Y - 8)
		|| y > (gObj[aIndex].Y + 8))
	{
		return false;
	}

	int mapnumber = gObj[aIndex].MapNumber;

	if(mapnumber < 0 || mapnumber >= MAX_MAP_NUMBER)
	{
		return false;
	}

	unsigned char attr = MapC[mapnumber].GetAttr(x,y);

	if(attr)
	{
		return false;
	}

	attr = MapC[mapnumber].GetAttr(gObj[aIndex].X,gObj[aIndex].Y);

	if(attr & 1)
	{
		return false;
	}

	return true;
}

BOOL gObjCheckTeleportAreaByMap(BYTE mapnumber, BYTE x, BYTE y)
{
	if(mapnumber < 0 || mapnumber >= MAX_MAP_NUMBER)
	{
		return false;
	}

	unsigned char attr = MapC[mapnumber].GetAttr(x,y);

	if(attr)
	{
		return false;
	}

	attr = MapC[mapnumber].GetAttr(x,y);

	if(attr & 1)
	{
		return false;
	}

	return true;
}






BOOL gObjCheckAttackAreaUsedPath(int aIndex, int TarObjIndex)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return false;
	}

	int X,Y;
	if(gObj[aIndex].PathCount > 1)
	{
		 X = gObj[aIndex].PathX[gObj[aIndex].PathCount-1];
		 Y = gObj[aIndex].PathY[gObj[aIndex].PathCount-1];
	}
	else
	{
		return false;
	}

	if(gObj[TarObjIndex].X < (X - 10)
		|| gObj[TarObjIndex].X > (X + 10)
		|| gObj[TarObjIndex].Y < (Y - 10)
		|| gObj[TarObjIndex].Y > (Y + 10))
	{
		return false;
	}
	return true;
}






BOOL gObjCheckattackAreaUsedViewPort(int aIndex, int TarObjIndex)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return false;
	}

	LPOBJ lpObj = &gObj[aIndex];
	LPOBJ lpTargetObj = &gObj[TarObjIndex];

	if(lpTargetObj->m_Index == lpObj->m_Index)
	{
		return true;
	}

	int MVL = MAX_VIEWPORT;

	if(lpObj->Type == OBJ_MONSTER)
	{
		MVL = MAX_VIEWPORT_MONSTER;
	}

	for(int n = 0; n < MVL;n++)
	{
		if(lpObj->VpPlayer[n].state)
		{
			if(lpObj->VpPlayer[n].number == lpTargetObj->m_Index)
			{
				return true;
			}
		}

		if(lpObj->VpPlayer2[n].state)
		{
			if(lpObj->VpPlayer2[n].number == lpTargetObj->m_Index)
			{
				return true;
			}
		}
	}
	return false;
}






int  gObjCheckAttackArea(int aIndex, int TarObjIndex)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return false;
	}
	if(OBJMAX_RANGE(TarObjIndex) == 0)
	{
		return false;
	}

	if(gObjCheckattackAreaUsedViewPort(aIndex,TarObjIndex)==0)
	{
		return 1;
	}

	int mapnumber = gObj[aIndex].MapNumber;

	if(mapnumber < 0 || mapnumber >= MAX_MAP_NUMBER)
	{
		return 2;
	}

	int tarmapnumber = gObj[TarObjIndex].MapNumber;

	if(tarmapnumber < 0 || tarmapnumber >= MAX_MAP_NUMBER)
	{
		return 3;
	}

	if(tarmapnumber != mapnumber)
	{
		return 4;
	}

	unsigned char attr = MapC[mapnumber].GetAttr(gObj[TarObjIndex].X,gObj[TarObjIndex].Y);

	if(attr && !(attr&2))
	{
		return 5;
	}

	attr = MapC[mapnumber].GetAttr(gObj[aIndex].X,gObj[aIndex].Y);

	if(attr && !(attr&2))
	{
		return 6;
	}

	return false;
}






BOOL gUserFindDevilSquareInvitation(int aIndex)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return false;
	}

	LPOBJ lpObj = &gObj[aIndex];

	for(int n = 0; n < ReadConfig.MAIN_INVENTORY_SIZE(aIndex,false); n++)
	{
		if(lpObj->pInventory[n].IsItem() == 1)
		{
			if(lpObj->pInventory[n].m_Type == ITEMGET(14,19))
			{
				return true;
			}

			if(lpObj->pInventory[n].m_Type == ITEMGET(13,46))
			{
				return true;
			}
		}
	}
	return false;
}






BOOL gUserFindDevilSquareKeyEyes(int aIndex)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return false;
	}

	LPOBJ lpObj = &gObj[aIndex];
	int bFindKey = 0;
	int bFindEyes = 0;

	for(int n = 0; n < ReadConfig.MAIN_INVENTORY_SIZE(aIndex,false); n++)
	{
		if(lpObj->pInventory[n].IsItem() == 1)
		{
			if(lpObj->pInventory[n].m_Type == ITEMGET(14,17))
			{
				bFindEyes = 1;
			}

			if(lpObj->pInventory[n].m_Type == ITEMGET(14,18))
			{
				bFindKey = 1;
			}
		}
	}

	if(bFindKey != 0 && bFindEyes != 0)
	{
		return true;
	}
	return false;
}






void gObjSendUserStatistic(int aIndex, int startLevel, int endLevel)
{
	if(gDisconnect == 1)
	{
		return;
	}

	if(endLevel < startLevel)
	{
		endLevel = ReadConfig.Max_Master_Level+ReadConfig.Max_Normal_Level;
	}

	int n;
	LPOBJ lpObj;
	int sClassCount[10];

	sClassCount[CLASS_WIZARD] = 0;
	sClassCount[CLASS_KNIGHT] = 0;
	sClassCount[CLASS_ELF] = 0;
	sClassCount[CLASS_MAGICGLADIATOR] = 0;
	sClassCount[CLASS_SUMMONER] = 0;
	sClassCount[CLASS_DARKLORD] = 0;
	sClassCount[CLASS_RAGEFIGHTER] = 0;

	int ClassCounterTotal = 0;

	for(n = OBJ_STARTUSERINDEX; n < OBJMAX; n++)
	{
		lpObj = &gObj[n];

		if(lpObj->Connected > PLAYER_LOGGED)
		{
			if(lpObj->Level >= startLevel && lpObj->Level <= endLevel)
			{
				if (lpObj->Class < 10 )
				{
					sClassCount[lpObj->Class]++;
				}
			}

			ClassCounterTotal++;
		}
	}

	MsgOutput(gObj[aIndex].m_Index,"[%d]-[%d] Total %d [%s %d %s %d %s %d %s %d %s %d %s %d %s %d]",startLevel,endLevel,
		ClassCounterTotal,
		lMsg.Get(1900),sClassCount[CLASS_WIZARD],
		lMsg.Get(1901),sClassCount[CLASS_KNIGHT],
		lMsg.Get(1902),sClassCount[CLASS_ELF],
		lMsg.Get(1903),sClassCount[CLASS_MAGICGLADIATOR],
		"DarkLord"	  ,sClassCount[CLASS_DARKLORD],
		"Summoner"	  ,sClassCount[CLASS_SUMMONER],
		"RageFighter" ,sClassCount[CLASS_RAGEFIGHTER]
	);
}






LPOBJ gObjFind(char * targetcharname)
{
	if ( gDisconnect == TRUE )
	{
		return NULL;
	}

	int n;
	LPOBJ lpObj;

	for ( n = OBJ_STARTUSERINDEX ; n<OBJMAX;n++)
	{
		lpObj = &gObj[n];

		if ( lpObj->Connected > PLAYER_LOGGED )
		{
			if ( lpObj->Name[0] == targetcharname[0] )
			{
				if ( strcmp(lpObj->Name, targetcharname) == 0 )
				{
					return lpObj;
				}
			}
		}
	}

	return NULL;

}






BOOL gObjFind10EventChip(int aIndex)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return false;
	}

	LPOBJ lpObj = &gObj[aIndex];
	int count = 0;

	for(int n = 0; n < ReadConfig.MAIN_INVENTORY_SIZE(aIndex,false);n++)
	{
		if(lpObj->pInventory[n].IsItem() == 1)
		{
			if(lpObj->pInventory[n].m_Type == ITEMGET(14,21))
			{
				count++;

				if(count >= 10)
				{
					return true;
				}
			}
		}
	}
	return false;
}






BOOL gObjDelete10EventChip(int aIndex)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return false;
	}

	LPOBJ lpObj = &gObj[aIndex];
	int count = 0;

	for(int n = 0; n < ReadConfig.MAIN_INVENTORY_SIZE(aIndex,false);n++)
	{
		if(lpObj->pInventory[n].IsItem() == 1)
		{
			if(lpObj->pInventory[n].m_Type == ITEMGET(14,21))
			{
				gObjInventoryDeleteItem(aIndex,n);
				GCInventoryItemDeleteSend(aIndex,n,1);
				LogAddTD("[EventChip] [%s][%s] Delete Rena (%d)",lpObj->AccountID,lpObj->Name,n);
				count++;

				if(count >= 10)
				{
					return true;
				}
			}
		}
	}
	return false;
}

void gObjSetPosition(int aIndex, int x, int y)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];
	PMSG_POSISTION_SET pMove;

	pMove.h.c = 0xC1;
	pMove.h.headcode = 0xDF;
	pMove.h.size = sizeof(pMove);

	pMove.X = x;
	pMove.Y = y;

	lpObj->m_Rest = 0;


	MapC[lpObj->MapNumber].ClearStandAttr(lpObj->m_OldX,lpObj->m_OldY);
	MapC[lpObj->MapNumber].SetStandAttr(x,y);

	lpObj->m_OldX = x;
	lpObj->m_OldY = y;

	RecvPositionSetProc(&pMove,lpObj->m_Index);
}






int  gObjGetItemCountInEquipment(int aIndex, int itemtype, int itemindex, int itemlevel)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return 0;
	}

	LPOBJ lpObj = &gObj[aIndex];
	int count = 0;

	for ( int n =0;n<MAX_PLAYER_EQUIPMENT ; n++ )
	{
		if ( lpObj->pInventory[n].IsItem() == TRUE )
		{
			if ( lpObj->pInventory[n].m_Type == ((itemtype * MAX_SUBTYPE_ITEMS) + itemindex) )
			{
				if ( itemlevel != -1 )
				{
					if (  lpObj->pInventory[n].m_Level != itemlevel )
					{
						continue;
					}
				}
				
				count++;
			}
		}
	}

	return count;
}







int  gObjGetItemCountInIventory(int aIndex, int itemtype, int itemindex, int itemlevel)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return 0;
	}

	LPOBJ lpObj = &gObj[aIndex];
	int count = 0;

	for ( int n =0;n<ReadConfig.MAIN_INVENTORY_SIZE(aIndex,false) ; n++ )
	{
		if ( lpObj->pInventory[n].IsItem() == TRUE )
		{
			if ( lpObj->pInventory[n].m_Type == ((itemtype * MAX_SUBTYPE_ITEMS) + itemindex) )
			{
				if ( itemlevel != -1 )
				{
					if (  lpObj->pInventory[n].m_Level != itemlevel )
					{
						continue;
					}
				}
				
				count++;
			}
		}
	}

	return count;
}






int gObjGetItemCountInIventory(int aIndex, int itemnum)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return 0;
	}

	LPOBJ lpObj = &gObj[aIndex];
	int count = 0;

	for ( int n=0;n<ReadConfig.MAIN_INVENTORY_SIZE(aIndex,false);n++)
	{
		if ( lpObj->pInventory[n].IsItem() == TRUE )
		{
			if ( lpObj->pInventory[n].m_Type == itemnum )
			{
				count++;
			}
		}
	}

	return count;

}


int  gObjGetHPItemPos(int aIndex)	// [Failure : -1]
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return -1;
	}

	LPOBJ lpObj = &gObj[aIndex];
	
	for ( int n=0;n<ReadConfig.MAIN_INVENTORY_SIZE(aIndex,false);n++)
	{
		if ( lpObj->pInventory[n].IsItem() == TRUE )
		{
			if ( lpObj->pInventory[n].m_Type >= ITEMGET(14,0) && lpObj->pInventory[n].m_Type <= ITEMGET(14,3) )
			{
				return n;
			}
		}
	}

	return -1;
}


int  gObjGetManaItemPos(int aIndex)	// [Failure : -1]
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return -1;
	}

	LPOBJ lpObj = &gObj[aIndex];
	
	for ( int n=0;n<ReadConfig.MAIN_INVENTORY_SIZE(aIndex,false);n++)
	{
		if ( lpObj->pInventory[n].IsItem() == TRUE )
		{
			if ( lpObj->pInventory[n].m_Type == ITEMGET(14,4) || lpObj->pInventory[n].m_Type == ITEMGET(14,5) || lpObj->pInventory[n].m_Type == ITEMGET(14,6) )
			{
				return n;
			}
		}
	}

	return -1;
}


int  gObjGetItemPos(int aIndex,int section,int itemid)	// [Failure : -1]
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return -1;
	}

	LPOBJ lpObj = &gObj[aIndex];
	
	for ( int n=0;n<ReadConfig.MAIN_INVENTORY_SIZE(aIndex,false);n++)
	{
		if ( lpObj->pInventory[n].IsItem() == TRUE )
		{
			if ( lpObj->pInventory[n].m_Type == ITEMGET(section,itemid) )
			{
				return n;
			}
		}
	}

	return -1;
}



void gObjDelteItemCountInInventory(int aIndex, int itemtype, int itemindex, int count)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];
	int delcount = 0;

	for(int n = 0; n < ReadConfig.MAIN_INVENTORY_SIZE(aIndex,false); n++)
	{
		if(lpObj->pInventory[n].IsItem() == 1)
		{
			if(lpObj->pInventory[n].m_Type == itemtype * 512 + itemindex)
			{
				gObjInventoryDeleteItem(aIndex,n);
				GCInventoryItemDeleteSend(aIndex,n,1);
				LogAddTD("[DeleteItem] [%s][%s] Delete (%d,%d)(%d)",lpObj->AccountID,lpObj->Name,itemtype,itemindex,n);
				delcount++;

				if(delcount >= count)
				{
					return;
				}
			}
		}
	}
}






void gObjGetStatPointState(int aIndex, short& AddPoint, short& MaxAddPoint, short& MinusPoint, short& MaxMinusPoint)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	if(gObj[aIndex].Level <= 5)
	{
		AddPoint = 0;
		MaxAddPoint = 0;
	}

	LPOBJ lpObj = &gObj[aIndex];

	int total_point = lpObj->Strength + lpObj->Dexterity + lpObj->Vitality + lpObj->Energy + lpObj->Leadership + lpObj->LevelUpPoint;
	int ori_point;

	if(lpObj->Class == CLASS_DARKLORD || lpObj->Class == CLASS_MAGICGLADIATOR || lpObj->Class == CLASS_RAGEFIGHTER)
	{
		ori_point = DCInfo.GetDefPoint(lpObj->Class) + (lpObj->Level - 1) * 7;
	}
	else
	{
		ori_point = DCInfo.GetDefPoint(lpObj->Class) + (lpObj->Level - 1) * 5;
	}

	if(g_QuestInfo.GetQuestState(lpObj,0) == 2)
	{
		ori_point+= 10;
	}

	if(g_QuestInfo.GetQuestState(lpObj,1) == 2)
	{
		ori_point+= 10;
	}

	if(g_QuestInfo.GetQuestState(lpObj,2) == 2)
	{
		ori_point+= lpObj->Level - 220;
	}

	int addpoint = ori_point;

	addpoint += gStatMng.GetMaxStat(lpObj->Level,lpObj->Class);

	AddPoint = total_point - ori_point;
	MaxAddPoint = gStatMng.GetMaxStat(lpObj->Level,lpObj->Class);
	MinusPoint = lpObj->iFruitPoint;
	MaxMinusPoint = gStatMng.GetMaxMinusStat(lpObj->Level,lpObj->Class);
}






BOOL gObjCheckStatPointUp(int aIndex)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return false;
	}

	LPOBJ lpObj = &gObj[aIndex];

	int total_point = lpObj->Strength + lpObj->Dexterity + lpObj->Vitality + lpObj->Energy + lpObj->Leadership + lpObj->LevelUpPoint;
	int ori_point;

	if(lpObj->Class == CLASS_DARKLORD || lpObj->Class == CLASS_MAGICGLADIATOR || lpObj->Class == CLASS_RAGEFIGHTER)
	{
		ori_point = DCInfo.GetDefPoint(lpObj->Class) + (lpObj->Level - 1) * 7;
	}
	else
	{
		ori_point = DCInfo.GetDefPoint(lpObj->Class) + (lpObj->Level - 1) * 5;
	}

	if(g_QuestInfo.GetQuestState(lpObj,0) == 2)
	{
		ori_point+= 10;
	}

	if(g_QuestInfo.GetQuestState(lpObj,1) == 2)
	{
		ori_point+= 10;
	}

	if(g_QuestInfo.GetQuestState(lpObj,2) == 2)
	{
		ori_point+= lpObj->Level - 220;
	}

	ori_point += gStatMng.GetMaxStat(lpObj->Level,lpObj->Class);

	if(total_point < ori_point)
	{
		return true;
	}
	return false;
}

BOOL gObjIsNewClass(LPOBJ lpObj)
{
	int Class = lpObj->DbClass;

	if( Class == DB_GRAND_MASTER  || 
		Class == DB_BLADE_MASTER || 
		Class == DB_HIGH_ELF || 
		Class == DB_DUEL_MASTER ||			//MG
		Class == DB_DUEL_MASTER+1 ||		//MG
		Class == DB_LORD_EMPEROR ||			//DL
		Class == DB_LORD_EMPEROR+1 ||		//DL
		Class == DB_DIMENSION_MASTER ||		//SUM
		Class == DB_RAGEFIGHER_EVO ||		//RF
		Class == DB_RAGEFIGHER_EVO+1)		//RF
	{
		return 1;
	}else
	{
		return 0;
	}
}

BOOL gObjCheckStatPointDown(int aIndex)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return false;
	}

	LPOBJ lpObj = &gObj[aIndex];

	int total_point = lpObj->Strength + lpObj->Dexterity + lpObj->Vitality + lpObj->Energy + lpObj->LevelUpPoint;
	int ori_point;

	if(lpObj->Class == CLASS_DARKLORD || lpObj->Class == CLASS_MAGICGLADIATOR || lpObj->Class == CLASS_RAGEFIGHTER)
	{
		ori_point = DCInfo.GetDefPoint(lpObj->Class) + (lpObj->Level - 1) * 7;
	}
	else
	{
		ori_point = DCInfo.GetDefPoint(lpObj->Class) + (lpObj->Level - 1) * 5;
	}

	if(g_QuestInfo.GetQuestState(lpObj,0) == 2)
	{
		ori_point+= 10;
	}

	if(g_QuestInfo.GetQuestState(lpObj,1) == 2)
	{
		ori_point+= 10;
	}

	ori_point += gStatMng.GetMaxStat(lpObj->Level,lpObj->Class);

	if(total_point > ori_point)
	{
		return true;
	}
	return false;
}






void gObjUseCircle(int aIndex, int pos)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	int level = gObj[aIndex].pInventory[pos].m_Level;
	int iSerial = gObj[aIndex].pInventory[pos].m_Number;

	PMSG_DEFRESULT pResult;

	PHeadSetB((LPBYTE)&pResult,0x2C,sizeof(pResult));

	pResult.result = 0;

	if(gObj[aIndex].Level <= 10 || level > 3)
	{
		pResult.result |= 0xC0;
		DataSend(aIndex,(unsigned char*)&pResult,pResult.h.size);
		return;
	}

	if(gObjCheckStatPointUp(aIndex)==0)
	{
		pResult.result |= 0xC0;
		DataSend(aIndex,(unsigned char*)&pResult,pResult.h.size);
		return;
	}

	LogAddTD("[StatUp] [%s][%s] Use StatUpItem Level:%d Pos:%d serial:%d",gObj[aIndex].AccountID,gObj[aIndex].Name,level,pos,iSerial);

	gObjInventoryItemSet(aIndex,pos,0xFF);
	gObj[aIndex].pInventory[pos].Clear();
	GCInventoryItemDeleteSend(aIndex,pos,1);

	short AddPoint;
	short MaxAddPoint;
	short MinusPoint;
	short MaxMinusPoint;

	gObjGetStatPointState(aIndex,(short &)AddPoint,(short &)(MaxAddPoint),(short &)MinusPoint,(short &)MaxMinusPoint);

	int iSuccessRate = 0;

	if(AddPoint <= 10)
	{
		iSuccessRate = 100;
	}
	else if((AddPoint - 10) < MaxAddPoint * (double)0.1)
	{
		iSuccessRate = 90;
	}
	else if((AddPoint - 10) < MaxAddPoint * (double)0.3)
	{
		iSuccessRate = 80;
	}
	else if((AddPoint - 10) < MaxAddPoint * (double)0.5)
	{
		iSuccessRate = 70;
	}
	else if((AddPoint - 10) < MaxAddPoint * (double)0.8)
	{
		iSuccessRate = 60;
	}
	else
	{
		iSuccessRate = 50;
	}

	int incvalue = 0;

	if(rand()%100 < iSuccessRate)
	{
		iSuccessRate = rand()%100;

		int AddStat = 0;

		if(iSuccessRate < 70)
		{
			AddStat = 1;
		}
		else if(iSuccessRate < 95)
		{
			AddStat = 2;
		}
		else
		{
			AddStat = 3;
		}

		if((AddStat + AddPoint) > MaxAddPoint)
		{
			AddStat = 1;
		}

		pResult.result |= level * 16;
		pResult.result |= AddStat;

		DataSend(aIndex,(unsigned char *)&pResult,pResult.h.size);

		incvalue = AddStat;

		LogAddTD("[StatUp] [%s][%s] Success [%d][%d] %d",gObj[aIndex].AccountID,gObj[aIndex].Name,level,incvalue,iSerial);
	}
	else
	{
		pResult.result |= 0x40;
		pResult.result |= level * 16;
		pResult.result = pResult.result;

		DataSend(aIndex,(unsigned char *)&pResult,pResult.h.size);
		LogAddTD("[StatUp] [%s][%s] Fail [%d] %d",gObj[aIndex].AccountID,gObj[aIndex].Name,level,iSerial);
		return;
	}

	switch(level)
	{
		case 3:
			gObj[aIndex].Strength += incvalue;
		break;
		case 2:
			gObj[aIndex].Dexterity += incvalue;
		break;
		case 1:
			gObj[aIndex].Vitality += incvalue;
			gObj[aIndex].MaxLife += gObj[aIndex].VitalityToLife * incvalue;
			//GCReFillSend(gObj[aIndex].m_Index,gObj[aIndex].MaxLife + gObj[aIndex].AddLife,0xFE,0,gObj[aIndex].iMaxShield+gObj[aIndex].iAddShield);
		break;
		case 0:
			gObj[aIndex].Energy += incvalue;
			gObj[aIndex].MaxMana += gObj[aIndex].EnergyToMana * incvalue;
			//gObjSetBP(aIndex);
			//GCManaSend(gObj[aIndex].m_Index,gObj[aIndex].MaxMana + gObj[aIndex].AddMana,0xFE,0,gObj[aIndex].MaxBP);
		break;
		default: break;
	}

	gObjCalCharacter(aIndex);
}

struct PMSG_USE_STAT_FRUIT {

  // static data ------------------------------------

  // non-static data --------------------------------
  /*<thisrel this+0x0>*/ /*|0x3|*/ struct PBMSG_HEAD h;
  /*<thisrel this+0x3>*/ /*|0x1|*/ unsigned char result;
  /*<thisrel this+0x4>*/ /*|0x1|*/ WORD btStatValue;
  /*<thisrel this+0x5>*/ /*|0x1|*/ unsigned char btFruitType;

  // base classes -----------------------------------

  // friends ----------------------------------------

  // static functions -------------------------------

  // non-virtual functions --------------------------

  // virtual functions ------------------------------
};

void gObjUsePlusStatFruit(int aIndex,int pos)
{
	if(!OBJMAX_RANGE(aIndex))
	{
		return;
	}

	if(gObj[aIndex].Class >= 6 || gObj[aIndex].Class < 0)
	{
		return;
	}

	int iItemLevel = gObj[aIndex].pInventory[pos].m_Level;
	int iItemSerial = gObj[aIndex].pInventory[pos].m_Number;

	PMSG_USE_STAT_FRUIT pResult;

	PHeadSetB((LPBYTE)&pResult,0x2C,sizeof(pResult));

	pResult.result = 2;
	pResult.btFruitType = iItemLevel;
	pResult.btStatValue = 0;

	if(gObj[aIndex].Level <= 9 || iItemLevel > 4)
	{
		pResult.result = 2;
		DataSend(aIndex,(unsigned char *)&pResult,pResult.h.size);
		return;
	}

	if(iItemLevel == 4 && gObj[aIndex].Class != 4)
	{
		pResult.result = 2;
		DataSend(aIndex,(unsigned char *)&pResult,pResult.h.size);
		return;
	}

	for(int iItemIndex = 0; iItemIndex < INVETORY_WEAR_SIZE; iItemIndex++)
	{
		if(gObj[aIndex].pInventory[iItemIndex].IsItem())
		{
			pResult.result = 16;
			DataSend(aIndex,(unsigned char *)&pResult,pResult.h.size);
			return;
		}
	}

	if(gObjCheckStatPointUp(aIndex) == false)
	{
		pResult.result = 33;
		DataSend(aIndex,(unsigned char *)&pResult,pResult.h.size);
		return;
	}

	LogAddTD("[StatUp] [%s][%s] Use StatUpItem Level:%d Pos:%d serial:%d",
		gObj[aIndex].AccountID,gObj[aIndex].Name,iItemLevel,pos,iItemSerial);

	gObjInventoryItemSet(aIndex,pos,0xFF);
	gObj[aIndex].pInventory[pos].Clear();
	GCInventoryItemDeleteSend(aIndex,pos,1);

	short AddPoint;
	short MaxAddPoint;
	short MinusPoint;
	short MaxMinusPoint;

	gObjGetStatPointState(aIndex,(short &)AddPoint,(short &)MaxAddPoint,(short &)MinusPoint,(short &)MaxMinusPoint);

	int iSuccessRate = 0;

	if(AddPoint <= 10)
	{
		iSuccessRate = 100;
	}
	else if((AddPoint - 10) < (MaxAddPoint * (double)0.1))
	{
		iSuccessRate = 90;
	}
	else if((AddPoint - 10) < (MaxAddPoint * (double)0.3))
	{
		iSuccessRate = 80;
	}
	else if((AddPoint - 10) < (MaxAddPoint * (double)0.5))
	{
		iSuccessRate = 70;
	}
	else if((AddPoint - 10) < (MaxAddPoint * (double)0.8))
	{
		iSuccessRate = 60;
	}
	else
	{
		iSuccessRate = 50;
	}

	int iIncStat = 0;
	int bSuccess = 0;

	if(rand()%100 < iSuccessRate)
	{
		bSuccess = 1;
	}

	if(bSuccess != 0)
	{
		iSuccessRate = rand()%100;

		int AddStat = 0;

		if(iSuccessRate < 70)
		{
			AddStat = 1;
		}
		else if(iSuccessRate < 95)
		{
			AddStat = 2;
		}
		else
		{
			AddStat = 3;
		}

		if((AddStat + AddPoint) > MaxAddPoint)
		{
			AddStat = 1;
		}

		pResult.result = 0;
		pResult.btStatValue = AddStat;
		pResult.btFruitType = iItemLevel;

		DataSend(aIndex,(unsigned char *)&pResult,pResult.h.size);
		iIncStat = AddStat;

		LogAddTD("[StatUp] [%s][%s] Success [%d][%d] %d",
			gObj[aIndex].AccountID,gObj[aIndex].Name,iItemLevel,iIncStat,iItemSerial);
	}
	else
	{
		pResult.result = 1;
		pResult.btStatValue = 0;
		pResult.btFruitType = iItemLevel;

		DataSend(aIndex,(unsigned char *)&pResult,pResult.h.size);

		LogAddTD("[StatUp] [%s][%s] Fail [%d] %d",
			gObj[aIndex].AccountID,gObj[aIndex].Name,iItemLevel,iItemSerial);
		return;
	}

	switch(iItemLevel)
	{
		case 4: gObj[aIndex].Leadership += iIncStat;	break;
		case 3: gObj[aIndex].Strength += iIncStat;	break;
		case 2: gObj[aIndex].Dexterity += iIncStat;	break;
		case 1:
			gObj[aIndex].Vitality += iIncStat;
			gObj[aIndex].MaxLife += gObj[aIndex].VitalityToLife * iIncStat;
		break;
		case 0:
			gObj[aIndex].Energy += iIncStat;
			gObj[aIndex].MaxMana += gObj[aIndex].EnergyToMana * iIncStat;
			//gObjSetBP(aIndex);
		break;
		default : break;
	}

	gObjCalCharacter(aIndex);
	//gObjSetBP(aIndex);

	//GCReFillSend(gObj[aIndex].m_Index,gObj[aIndex].MaxLife + gObj[aIndex].AddLife,0xFE,0,gObj[aIndex].iMaxShield + gObj[aIndex].iAddShield);
	//GCManaSend(gObj[aIndex].m_Index,gObj[aIndex].MaxMana + gObj[aIndex].AddMana,0xFE,0,gObj[aIndex].MaxBP + gObj[aIndex].AddBP);
}



void gObjUseMinusStatFruit(int aIndex, int pos)
{
	if(!OBJMAX_RANGE(aIndex))
	{
		return;
	}

	if(gObj[aIndex].Class >= 6 || gObj[aIndex].Class < 0)
	{
		return;
	}

	int iItemLevel = gObj[aIndex].pInventory[pos].m_Level;
	int iItemSerial = gObj[aIndex].pInventory[pos].m_Number;
	int iCharacterClass = gObj[aIndex].Class;

	PMSG_USE_STAT_FRUIT pResult;

	PHeadSetB((LPBYTE)&pResult,0x2C,sizeof(pResult));

	pResult.result = 5;
	pResult.btFruitType = iItemLevel;
	pResult.btStatValue = 0;

	if(gObj[aIndex].Level <= 9 || iItemLevel > 4)
	{
		pResult.result = 5;
		DataSend(aIndex,(unsigned char *)&pResult,pResult.h.size);
		return;
	}

	if(iItemLevel == 4 && gObj[aIndex].Class != 4)
	{
		pResult.result = 5;
		DataSend(aIndex,(unsigned char *)&pResult,pResult.h.size);
		return;
	}

	for(int iItemIndex = 0; iItemIndex < INVETORY_WEAR_SIZE; iItemIndex++)
	{
		if(gObj[aIndex].pInventory[iItemIndex].IsItem())
		{
			pResult.result = 16;
			DataSend(aIndex,(unsigned char *)&pResult,pResult.h.size);
			return;
		}
	}

	int iDefaultStat = 0;
	int iPresentStat = 0;
	int bEnableUseFruit = 1;

	if(iItemLevel == 0)
	{
		iDefaultStat = DCInfo.DefClass[iCharacterClass].Energy;
		iPresentStat = gObj[aIndex].Energy;

		if(iPresentStat <= iDefaultStat)
		{
			bEnableUseFruit = 0;
		}
	}
	else if(iItemLevel == 1)
	{
		iDefaultStat = DCInfo.DefClass[iCharacterClass].Vitality;
		iPresentStat = gObj[aIndex].Vitality;

		if(iPresentStat <= iDefaultStat)
		{
			bEnableUseFruit = 0;
		}
	}
	else if(iItemLevel == 2)
	{
		iDefaultStat = DCInfo.DefClass[iCharacterClass].Dexterity;
		iPresentStat = gObj[aIndex].Dexterity;

		if(iPresentStat <= iDefaultStat)
		{
			bEnableUseFruit = 0;
		}
	}
	else if(iItemLevel == 3)
	{
		iDefaultStat = DCInfo.DefClass[iCharacterClass].Strength;
		iPresentStat = gObj[aIndex].Strength;

		if(iPresentStat <= iDefaultStat)
		{
			bEnableUseFruit = 0;
		}
	}
	else if(iItemLevel == 4)
	{
		iDefaultStat = DCInfo.DefClass[iCharacterClass].Leadership;
		iPresentStat = gObj[aIndex].Leadership;

		if(iPresentStat <= iDefaultStat)
		{
			bEnableUseFruit = 0;
		}
	}

	if(bEnableUseFruit == 0)
	{
		pResult.result = 38;
		DataSend(aIndex,(unsigned char *)&pResult,pResult.h.size);
		return;
	}

	short AddPoint;
	short MaxAddPoint;
	short MinusPoint;
	short MaxMinusPoint = 0;

	gObjGetStatPointState(aIndex,(short &)AddPoint,(short &)MaxAddPoint,(short &)MinusPoint,(short &)MaxMinusPoint);

	if(MinusPoint >= MaxMinusPoint || MinusPoint < 0)
	{
		pResult.result = 37;
		DataSend(aIndex,(unsigned char *)&pResult,pResult.h.size);
		return;
	}

	LogAddTD("[StatDown] [%s][%s] Use StatDownItem Level:%d Pos:%d serial:%d",
		gObj[aIndex].AccountID,gObj[aIndex].Name,iItemLevel,pos,iItemSerial);

	gObjInventoryItemSet(aIndex,pos,0xFF);
	gObj[aIndex].pInventory[pos].Clear();
	GCInventoryItemDeleteSend(aIndex,pos,1);

	int iDecStat = 0;
	int bSuccess = 0;

	int iSuccessRate = 100 - gObj[aIndex].Level / 6;

	if(iSuccessRate < 0)
	{
		iSuccessRate = 0;
	}

	if(rand()%100 < iSuccessRate)
	{
		bSuccess = 1;
	}

	if(MinusPoint <= 10)
	{
		bSuccess = 1;
	}

	if(bSuccess != 0)
	{
		iSuccessRate = rand()%100;

		if(iSuccessRate < 50)
		{
			iDecStat = 1;
		}
		else if(iSuccessRate < 75)
		{
			iDecStat = 3;
		}
		else if(iSuccessRate < 91)
		{
			iDecStat = 5;
		}
		else if(iSuccessRate < 98)
		{
			iDecStat = 7;
		}
		else
		{
			iDecStat = 9;
		}

		if((MinusPoint + iDecStat) >= MaxMinusPoint)
		{
			iDecStat = MaxMinusPoint - MinusPoint;
		}

		if((iPresentStat - iDecStat) < iDefaultStat)
		{
			iDecStat = iPresentStat - iDefaultStat;
		}

		pResult.result = 3;
		pResult.btFruitType = iItemLevel;
		pResult.btStatValue = iDecStat;

		DataSend(aIndex,(unsigned char *)&pResult,pResult.h.size);
		LogAddTD("[StatDown] [%s][%s] Success [%d][%d] %d",
			gObj[aIndex].AccountID,gObj[aIndex].Name,iItemLevel,iDecStat,iItemSerial);
	}
	else
	{
		pResult.result = 4;
		pResult.btFruitType = iItemLevel;
		pResult.btStatValue = 0;
		DataSend(aIndex,(unsigned char *)&pResult,pResult.h.size);
		LogAddTD("[StatDown] [%s][%s] Fail [%d] %d",
			gObj[aIndex].AccountID,gObj[aIndex].Name,iItemLevel,iItemSerial);
		return;
	}

	switch(iItemLevel)
	{
		case 4:	gObj[aIndex].Leadership -= iDecStat;	break;
		case 3: gObj[aIndex].Strength -= iDecStat;	break;
		case 2: gObj[aIndex].Dexterity -= iDecStat;	break;
		case 1:
			gObj[aIndex].Vitality -= iDecStat;
			gObj[aIndex].MaxLife -= gObj[aIndex].VitalityToLife *iDecStat;
		break;
		case 0:
			gObj[aIndex].Energy -= iDecStat;
			gObj[aIndex].MaxMana -= gObj[aIndex].EnergyToMana * iDecStat;
			//gObjSetBP(aIndex);
		break;

		default:
		break;
	}

	int iOldLevelUpPoint = gObj[aIndex].LevelUpPoint;
	gObj[aIndex].LevelUpPoint += iDecStat;

	int iOldFruitPoint = gObj[aIndex].LevelUpPoint;
	gObj[aIndex].iFruitPoint += iDecStat;

	LogAddTD("[StatDown] [%s][%s] MinusStat[%d] -> LevelUpPoint Old(%d)/New(%d)  FruitPoint Old(%d)/New(%d)",
		gObj[aIndex].AccountID,gObj[aIndex].Name,iDecStat,iOldLevelUpPoint,gObj[aIndex].LevelUpPoint,iOldFruitPoint,gObj[aIndex].iFruitPoint);

	gObjCalCharacter(aIndex);
	//gObjSetBP(aIndex);

	//GCReFillSend(gObj[aIndex].m_Index,gObj[aIndex].MaxLife + gObj[aIndex].AddLife,0xFE,0,gObj[aIndex].iMaxShield+gObj[aIndex].iAddShield);
	//GCManaSend(gObj[aIndex].m_Index,gObj[aIndex].MaxMana + gObj[aIndex].AddMana,0xFE,0,gObj[aIndex].MaxBP);
}


void gObjCalcMaxLifePower(int aIndex)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	switch(lpObj->Class)
	{
	case CLASS_WIZARD:
			lpObj->m_MaxLifePower = lpObj->Level + lpObj->Vitality + 110;
		break;
	case CLASS_KNIGHT:
			lpObj->m_MaxLifePower = lpObj->Level*2 + lpObj->Vitality + 60;
		break;
	case CLASS_ELF:
			lpObj->m_MaxLifePower = lpObj->Level + lpObj->Vitality + 80;
		break;
	case CLASS_RAGEFIGHTER:
			lpObj->m_MaxLifePower = lpObj->Level + lpObj->Vitality + 110;
		break;
	case CLASS_MAGICGLADIATOR:
			lpObj->m_MaxLifePower = lpObj->Level + lpObj->Vitality + 110;
		break;
	case CLASS_DARKLORD:
			lpObj->m_MaxLifePower = lpObj->Level + lpObj->Vitality + 110;
		break;
	case CLASS_SUMMONER:
			lpObj->m_MaxLifePower = lpObj->Level + lpObj->Vitality + 110;
		break;
	default :
		lpObj->m_MaxLifePower = 0;
		break;
	}
}






void gObjDelayLifeCheck(int aIndex)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->m_CheckLifeTime > 0)
	{
		lpObj->m_CheckLifeTime--;

		if(lpObj->m_CheckLifeTime <= 0)
		{
			lpObj->m_CheckLifeTime = 0;

			if(lpObj->Life < 0)
			{
				lpObj->Life = 0;
			}

			if(lpObj->lpAttackObj != 0)
			{
				gObjLifeCheck(lpObj,lpObj->lpAttackObj,0,DAMAGE_TYPE_DELAY,0,0,0,0);
			}
		}
	}
}


void gObjNProtectGGCheckSum(int aIndex)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	if(gUseNPGGChecksum==0)
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	//if(lpObj->m_InWebzen)
	//{
	//	return;
	//}

	if(lpObj->m_bSentGGAuth == 0)
	{
		if((GetTickCount() - lpObj->m_NPggCheckSumSendTime) > 300000)
		{
			lpObj->m_NPggCheckSumSendTime = GetTickCount();

			unsigned long dwGGErrCode = lpObj->NPggCSAuth.GetAuthQuery();

			if(dwGGErrCode != 0)
			{
				LogAddTD("[NPgg] Failed Send Checksum %x [%s][%s] errorcode: %d",dwGGErrCode,lpObj->AccountID,lpObj->Name,dwGGErrCode);
				lpObj->m_bSentGGAuth = 0;
				gObjCloseSet(aIndex,0);
			}
			else
			{
				LogAddTD("[NPgg] Send Checksum %x [%s][%s]",dwGGErrCode,lpObj->AccountID,lpObj->Name);
				GCNPggSendCheckSum(aIndex,&lpObj->NPggCSAuth.m_AuthQuery);
				lpObj->m_bSentGGAuth = 1;
			}
		}
	}
	else
	{
		if((GetTickCount() - lpObj->m_NPggCheckSumSendTime) > 30000)
		{
			LogAddTD("[NPgg] Checksum Time Out [%s][%s]",lpObj->AccountID,lpObj->Name);

			PMSG_NOTICE pNotice;
			pNotice.type = 0;	// 3
			pNotice.btCount = 0;	// 4
			pNotice.wDelay = 0;	// 6	
			pNotice.dwColor = 0;	// 8
			pNotice.btSpeed = 0;	// C

			TNotice::SetNoticeProperty(&pNotice,10,_ARGB(0xFF,0xFE,0x51,0x51),1,0,20);

			TNotice::MakeNoticeMsg(&pNotice,10,lMsg.Get(1201));
			TNotice::SendNoticeToUser(aIndex,&pNotice);

			TNotice::MakeNoticeMsg(&pNotice,10,lMsg.Get(1202));
			TNotice::SendNoticeToUser(aIndex,&pNotice);

			TNotice::MakeNoticeMsg(&pNotice,10,lMsg.Get(1203));
			TNotice::SendNoticeToUser(aIndex,&pNotice);

			gObjCloseSet(aIndex,0);
		}
	}
}








void SkillFrustrum(BYTE bangle, int aIndex)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	float p[MAX_ARRAY_FRUSTRUM][3];

	p[0][0] = -6;
	p[0][1] = 6;
	p[0][2] = 0;
	p[1][0] = 6;
	p[1][1] = 6;
	p[1][2] = 0;
	p[2][0] = 1;
	p[2][1] = 0;
	p[2][2] = 0;
	p[3][0] = -1;
	p[3][1] = 0;
	p[3][2] = 0;

	float Angle[0x3];

	Angle[0] = 0;
	Angle[1] = 0;
	Angle[2] = bangle * 360 / 255;

	float Matrix[3][MAX_ARRAY_FRUSTRUM];

	AngleMatrix(Angle,Matrix);

	float vFrustrum[MAX_ARRAY_FRUSTRUM][3];

	for(int i = 0; i < MAX_ARRAY_FRUSTRUM; i++)
	{
		VectorRotate((const float *)&p[i],(const float *)&Matrix[0],(float *)&vFrustrum[i]);

		lpObj->fSkillFrustrumX[i] = (int)vFrustrum[i][0] + lpObj->X;
		lpObj->fSkillFrustrumY[i] = (int)vFrustrum[i][1] + lpObj->Y;
	}
}






BOOL SkillTestFrustrum(int x, int y,int aIndex)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return false;
	}

	LPOBJ lpObj = &gObj[aIndex];

	int j = 3;

	for(int i = 0; i < MAX_ARRAY_FRUSTRUM; j = i,i++)
	{
		if((lpObj->fSkillFrustrumX[i]- x) * (lpObj->fSkillFrustrumY[j]-y) - (lpObj->fSkillFrustrumX[j]-x) * (lpObj->fSkillFrustrumY[i]-y) < 0.0f)
		{
			return false;
		}
	}
	return true;
}

/*BOOL gObjCheckMaxZen(int aIndex, int nAddZen)
{
	if ( !OBJMAX_RANGE(aIndex) )
		return FALSE;

	LPOBJ lpObj = &gObj[aIndex];
	
	__int64 _Money = 0;
	_Money = (__int64)lpObj->Money + (__int64)nAddZen;

	if ( _Money > (__int64)MAX_ZEN )
	{
#if (PACK_EDITION>=3)
		if (lpObj->IsBot == 0)
#endif
			GCServerMsgStringSend(lMsg.Get(MSGGET(4, 226)), aIndex, 1);
		return FALSE;
	}

	return TRUE;
}*/






void MakeRandomSetItem(int aIndex)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	int itemnum = gSetItemOption.GenRandomItemNum();

	int SetOption = gSetItemOption.GenSetOption(itemnum);

	int option1rand;
	int option2rand;
	int option3rand;
	int optionc;
	int Option1 = 0;
	int Option2 = 0;
	int Option3 = 0;

	option1rand = 6;
	option2rand = 4;

	option3rand = rand()%100;
	optionc = rand()%3;

	if(rand()%100 < option2rand)
	{
		Option2 = 1;
	}

	switch(optionc)
	{
	case 0: 
		if(option3rand < 4)
		{
			Option3 = 3;
		}
		break;
	case 1:
		if(option3rand < 8)
		{
			Option3 = 2;
		}
		break;
	case 2: 
		if(option3rand < 12)
		{
			Option3 = 1;
		}
		break;
	default: break;
	}

	Option1 = 1;

	ItemSerialCreateSend(aIndex,gObj[aIndex].MapNumber,gObj[aIndex].X,gObj[aIndex].Y,itemnum,0,0,Option1,Option2,Option3,aIndex,0,SetOption);
	LogAddTD("[Reward] [%s][%s] Winner Set Item itemnum:[%d] skill:[%d] luck:[%d] option:[%d] SetOption:[%d]",gObj[aIndex].AccountID,gObj[aIndex].Name,
		itemnum,Option1,Option2,Option3,SetOption);
}






void MakeRewardSetItem(int aIndex, BYTE cDropX, BYTE cDropY, int iRewardType, int iMapnumber)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	int itemnum = gSetItemOption.GenRandomItemNum();

	int SetOption = gSetItemOption.GenSetOption(itemnum);

	int option1rand;
	int option2rand;
	int option3rand;
	int optionc;
	int Option1 = 0;
	int Option2 = 0;
	int Option3 = 0;

	option1rand = 6;
	option2rand = 4;

	option3rand = rand()%100;
	optionc = rand()%3;

	if(rand()%100 < option2rand)
	{
		Option2 = 1;
	}

	switch(optionc)
	{
	case 0: 
		if(option3rand < 4)
		{
			Option3 = 3;
		}
		break;
	case 1:
		if(option3rand < 8)
		{
			Option3 = 2;
		}
		break;
	case 2: 
		if(option3rand < 12)
		{
			Option3 = 1;
		}
		break;
	default: break;
	}

	Option1 = 1;

	if(cDropX == 0 && cDropY == 0)
	{
		cDropX = gObj[aIndex].X;
		cDropY = gObj[aIndex].Y;
	}

	ItemSerialCreateSend(aIndex,iMapnumber,cDropX,cDropY,itemnum,0,0,Option1,Option2,Option3,aIndex,0,SetOption);

	if(iRewardType == 1)
	{
		LogAddTD("[Reward][KUNDUN] [%s][%s] Set Item itemnum:[%d] skill:[%d] luck:[%d] option:[%d] SetOption:[%d]",
			gObj[aIndex].AccountID,gObj[aIndex].Name,itemnum,Option1,Option2,Option3,SetOption);
	}
	else
	{
		LogAddTD("[Reward][Etc] [%s][%s] Set Item itemnum:[%d] skill:[%d] luck:[%d] option:[%d] SetOption:[%d]",
			gObj[aIndex].AccountID,gObj[aIndex].Name,itemnum,Option1,Option2,Option3,SetOption);
	}		
}






void gObjRecall(int aIndex, int mapnumber, int x, int y)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	if(gObj[aIndex].MapNumber != mapnumber)
	{
		gObj[aIndex].X = x;
		gObj[aIndex].Y = y;

		gObj[aIndex].TX = x;
		//gObj[aIndex].TX = y;	// #error Change to TY
		gObj[aIndex].TY = y;	// FIXED

		gObj[aIndex].MapNumber = mapnumber;

		gObj[aIndex].PathCount = 0;
		gObj[aIndex].Teleport = 0;

		gObjClearViewport(&gObj[aIndex]);
		GCTeleportSend(&gObj[aIndex],-1,mapnumber,gObj[aIndex].X,gObj[aIndex].Y,gObj[aIndex].Dir);

		if(gObj[aIndex].m_Change >= 0)
		{
			gObjViewportListProtocolCreate(&gObj[aIndex]);
		}

		gObj[aIndex].RegenMapNumber = mapnumber;
		gObj[aIndex].RegenMapX = x;
		gObj[aIndex].RegenMapY = y;
		gObj[aIndex].RegenOk = 1;
	}
	else
	{
		if(ReadConfig.S5E2 == TRUE)
		{
			PMSG_MAGICATTACK_RESULT_S5E2 pAttack;

			PHeadSetBE((unsigned char *)&pAttack,0x19,sizeof(pAttack));
			//pAttack.UNK1 = 0x00;
			//pAttack.MagicNumber = 6;
			pAttack.MagicNumberH = 0;
			pAttack.MagicNumberL = 6;
			pAttack.SourceNumberH = SET_NUMBERH(aIndex);
			pAttack.SourceNumberL = SET_NUMBERL(aIndex);
			pAttack.TargetNumberH = SET_NUMBERH(aIndex);
			pAttack.TargetNumberL = SET_NUMBERL(aIndex);
			pAttack.UnkS5E2 = 1;

			DataSend(aIndex,(unsigned char *)&pAttack,pAttack.h.size);
			MsgSendV2(&gObj[aIndex],(unsigned char*)&pAttack,pAttack.h.size);
			gObjTeleportMagicUse(aIndex,x,y);
		}else
		{
			PMSG_MAGICATTACK_RESULT pAttack;

			PHeadSetBE((unsigned char *)&pAttack,0x19,sizeof(pAttack));
			//pAttack.UNK1 = 0x00;
			//pAttack.MagicNumber = 6;
			pAttack.MagicNumberH = 0;
			pAttack.MagicNumberL = 6;
			pAttack.SourceNumberH = SET_NUMBERH(aIndex);
			pAttack.SourceNumberL = SET_NUMBERL(aIndex);
			pAttack.TargetNumberH = SET_NUMBERH(aIndex);
			pAttack.TargetNumberL = SET_NUMBERL(aIndex);

			DataSend(aIndex,(unsigned char *)&pAttack,pAttack.h.size);
			MsgSendV2(&gObj[aIndex],(unsigned char*)&pAttack,pAttack.h.size);
			gObjTeleportMagicUse(aIndex,x,y);
		}
	}
}






void gObjSetExpPetItem(int aIndex, int exp)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->Class != 4)	
	{
		return;
	}

	exp *= gDarkSpiritAddExperience;

	if(lpObj->pInventory[8].m_Type == ITEMGET(13,4) && lpObj->pInventory[1].m_Type == ITEMGET(13,5))
	{
		int addexp = exp * 10 / 100;

		if(lpObj->pInventory[1].AddPetItemExp(aIndex,addexp))
		{
			if(gObjIsItemPut(lpObj,&lpObj->pInventory[1],1) == 0)
			{
				lpObj->pInventory[1].PetItemLevelDown(addexp);
				MsgOutput(aIndex,lMsg.Get(1245));
			}
			else
			{
				LogAddTD("[%s][%s][PetItemLevelUp] [%s] Level:[%d]Exp:[%d]AddExp:[%d]",
					lpObj->AccountID,lpObj->Name,lpObj->pInventory[1].GetName(),lpObj->pInventory[1].m_PetItem_Level,lpObj->pInventory[1].m_PetItem_Exp,addexp);
				CDarkSpirit::SendLevelmsg(lpObj->m_Index,1,0,0xFE);
			}
		}

		if(lpObj->pInventory[8].AddPetItemExp(aIndex,addexp))
		{
			if(gObjIsItemPut(lpObj,&lpObj->pInventory[8],8) == 0)
			{
				lpObj->pInventory[8].PetItemLevelDown(addexp);
				MsgOutput(aIndex,lMsg.Get(1246));
			}
			else
			{
				LogAddTD("[%s][%s][PetItemLevelUp] [%s] Level:[%d]Exp:[%d]AddExp:[%d]",
					lpObj->AccountID,lpObj->Name,lpObj->pInventory[8].GetName(),lpObj->pInventory[8].m_PetItem_Level,lpObj->pInventory[8].m_PetItem_Exp,addexp);
				CDarkSpirit::SendLevelmsg(lpObj->m_Index,8,1,0xFE);
			}
		}
	}
	else if(lpObj->pInventory[8].m_Type == ITEMGET(13,4))
	{
		int addexp = exp * 20 / 100;

		if(lpObj->pInventory[8].AddPetItemExp(aIndex,addexp))
		{
			if(gObjIsItemPut(lpObj,&lpObj->pInventory[8],8) == 0)
			{
				lpObj->pInventory[8].PetItemLevelDown(addexp);
				MsgOutput(aIndex,lMsg.Get(1246));
			}
			else
			{
				LogAddTD("[%s][%s][PetItemLevelUp] [%s] Level:[%d]Exp:[%d]AddExp:[%d]",
					lpObj->AccountID,lpObj->Name,lpObj->pInventory[8].GetName(),lpObj->pInventory[8].m_PetItem_Level,lpObj->pInventory[8].m_PetItem_Exp,addexp);
				CDarkSpirit::SendLevelmsg(lpObj->m_Index,8,1,0xFE);
			}
		}
	}
	else if(lpObj->pInventory[1].m_Type == ITEMGET(13,5))
	{
		int addexp = exp * 20 / 100;

		if(lpObj->pInventory[1].AddPetItemExp(aIndex,addexp))
		{
			if(gObjIsItemPut(lpObj,&lpObj->pInventory[1],1) == 0)
			{
				lpObj->pInventory[1].PetItemLevelDown(addexp);
				MsgOutput(aIndex,lMsg.Get(1245));
			}
			else
			{
				LogAddTD("[%s][%s][PetItemLevelUp] [%s] Level:[%d]Exp:[%d]AddExp:[%d]",
					lpObj->AccountID,lpObj->Name,lpObj->pInventory[1].GetName(),lpObj->pInventory[1].m_PetItem_Level,lpObj->pInventory[1].m_PetItem_Exp,addexp);
				CDarkSpirit::SendLevelmsg(lpObj->m_Index,1,0,0xFE);
			}
		}
	}
}






BOOL gObjGetRandomItemDropLocation(int iMapNumber, BYTE& cX, BYTE& cY, int iRangeX, int iRangeY, int iLoopCount)
{
	int iUX = cX;
	int iUY = cY;

	if ( iRangeX <= 0 )
	{
		iRangeX = 1;
	}

	if ( iRangeY <= 0 )
	{
		iRangeY = 1;
	}

	if ( iLoopCount <= 0 )
	{
		iLoopCount = 1;
	}

	while ( iLoopCount-- > 0 )
	{
		cX = ( rand() % (iRangeX+1) ) * (((rand()%2==0)?-1:1)) + iUX;
		cY = ( rand() % (iRangeY+1) ) * (((rand()%2==0)?-1:1)) + iUY;

		BYTE attr = MapC[iMapNumber].GetAttr(cX, cY);

		if ( (attr&4) != 4 && (attr&8) != 8)
		{
			return TRUE;
		}
	}

	return FALSE;
}







BOOL gObjGetRandomFreeLocation(int iMapNumber, BYTE &cX, BYTE & cY, int iRangeX, int iRangeY, int iLoopCount)
{
	int iUX = cX;
	int iUY = cY;

	if ( iRangeX <= 0 )
	{
		iRangeX = 1;
	}

	if ( iRangeY <= 0 )
	{
		iRangeY = 1;
	}

	if ( iLoopCount <= 0 )
	{
		iLoopCount = 1;
	}

	while ( iLoopCount-- > 0 )
	{
		cX = ( rand() % (iRangeX+1) ) * (((rand()%2==0)?-1:1)) + iUX;
		cY = ( rand() % (iRangeY+1) ) * (((rand()%2==0)?-1:1)) + iUY;

		BYTE attr = MapC[iMapNumber].GetAttr(cX, cY);

		if ( attr == 0 )
		{
			return TRUE;
		}
	}

	return FALSE;
}







int gObjCheckAttackTypeMagic(int iClass, int iSkill)
{
	if ( iSkill == 0 )
	{
		return 0;
	}

	return MagicDamageC.GetSkillType(iSkill);
}







int gObjGetGuildUnionNumber(LPOBJ lpObj)
{
	int iUnion=0;

	if ( lpObj->lpGuild != NULL )
	{
		iUnion = (lpObj->lpGuild->iGuildUnion == 0)? lpObj->lpGuild->Number : lpObj->lpGuild->iGuildUnion;
	}

	return iUnion;

}






void gObjGetGuildUnionName(LPOBJ lpObj, char* szUnionName, int iUnionNameLen)
{
	szUnionName[0] = 0;
	
	if ( lpObj->lpGuild == NULL )
	{
		return;
	}

	if ( lpObj->lpGuild->iGuildUnion == 0 )
	{
		return;
	}

	TUnionInfo * pUnionInfo = UnionManager.SearchUnion(lpObj->lpGuild->iGuildUnion);

	if ( pUnionInfo != NULL )
	{
		memcpy(szUnionName, pUnionInfo->m_szMasterGuild, iUnionNameLen);
	}
}






BOOL gObjCheckRival(LPOBJ lpObj, LPOBJ lpTargetObj)
{
	if(lpTargetObj->Type != OBJ_USER)
	{
		return false;
	}

	if(lpObj->Type != OBJ_USER)
	{
		return false;
	}

	if(lpTargetObj->lpGuild == 0 || lpObj->lpGuild == 0)
	{
		return false;
	}

	if(lpTargetObj->lpGuild->iGuildRival == 0 || lpObj->lpGuild->iGuildRival == 0)
	{
		return false;
	}

	int iUnion = !lpObj->lpGuild->iGuildUnion ? lpObj->lpGuild->Number : lpObj->lpGuild->iGuildUnion;

	if(UnionManager.GetGuildRelationShip(iUnion,lpTargetObj->lpGuild->Number) == 2)
	{
		return true;
	}
	return false;
}






int gObjGetRelationShip(LPOBJ lpObj, LPOBJ lpTargetObj)
{
	if ( lpObj == NULL || lpTargetObj == NULL )
	{
		return false;
	}

	if ( lpObj->lpGuild == NULL || lpTargetObj->lpGuild == NULL )
	{
		return 0;
	}

	int iUnion = (!lpObj->lpGuild->iGuildUnion) ? lpObj->lpGuild->Number : lpObj->lpGuild->iGuildUnion;

	return UnionManager.GetGuildRelationShip(iUnion, lpTargetObj->lpGuild->Number);
}

struct PMSG_UNION_VIEWPORT_NOTIFY_COUNT {

  // static data ------------------------------------

  // non-static data --------------------------------
  /*<thisrel this+0x0>*/ /*|0x4|*/ struct PWMSG_HEAD h;
  /*<thisrel this+0x4>*/ /*|0x1|*/ unsigned char btCount;

  // base classes -----------------------------------

  // friends ----------------------------------------

  // static functions -------------------------------

  // non-virtual functions --------------------------

  // virtual functions ------------------------------
};
// <size 0x5>

struct PMSG_UNION_VIEWPORT_NOTIFY {

  // static data ------------------------------------

  // non-static data --------------------------------
  /*<thisrel this+0x0>*/ /*|0x1|*/ unsigned char btNumberH;
  /*<thisrel this+0x1>*/ /*|0x1|*/ unsigned char btNumberL;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int iGuildNumber;
  /*<thisrel this+0x8>*/ /*|0x1|*/ unsigned char btGuildRelationShip;
  /*<thisrel this+0x9>*/ /*|0x8|*/ char szUnionName[8];

  // base classes -----------------------------------

  // friends ----------------------------------------

  // static functions -------------------------------

  // non-virtual functions --------------------------

  // virtual functions ------------------------------
};
// <size 0x14>

void gObjNotifyUpdateUnionV1(LPOBJ lpObj)
{
	if(lpObj == 0)
	{
		return;
	}

	char cBUFFER_V1[6000];
	int iVp1Count = 0;

	memset(cBUFFER_V1,0x00,sizeof(cBUFFER_V1));

	PMSG_UNION_VIEWPORT_NOTIFY_COUNT * lpMsg = (PMSG_UNION_VIEWPORT_NOTIFY_COUNT *)&cBUFFER_V1;
	PMSG_UNION_VIEWPORT_NOTIFY * lpMsgBody = (PMSG_UNION_VIEWPORT_NOTIFY * )&cBUFFER_V1[sizeof(PMSG_UNION_VIEWPORT_NOTIFY_COUNT)];


	for(int n = 0; n < MAX_VIEWPORT; n++)
	{
		if(!OBJMAX_RANGE(lpObj->VpPlayer[n].number))
		{
			continue;
		}

		if(lpObj->VpPlayer[n].state == 2 && lpObj->VpPlayer[n].type == OBJ_USER)
		{
			LPOBJ lpTargetObj = &gObj[lpObj->VpPlayer[n].number];

			if(lpTargetObj == 0)
			{
				continue;
			}

			if(lpTargetObj->lpGuild == 0)
			{
				continue;
			}

			gObjGetGuildUnionName(lpTargetObj,lpMsgBody[iVp1Count].szUnionName,sizeof(lpMsgBody[iVp1Count].szUnionName));

			lpMsgBody[iVp1Count].btGuildRelationShip = gObjGetRelationShip(lpObj,lpTargetObj);
			lpMsgBody[iVp1Count].btNumberL = SET_NUMBERL(WORD(lpTargetObj->m_Index));
			lpMsgBody[iVp1Count].btNumberH = SET_NUMBERH(WORD(lpTargetObj->m_Index));
			lpMsgBody[iVp1Count].iGuildNumber = lpTargetObj->lpGuild->Number;
			iVp1Count++;
		}
	}

	if(iVp1Count > 0 && iVp1Count <= MAX_VIEWPORT)
	{
		lpMsg->btCount = iVp1Count;

		PHeadSetW((unsigned char *)lpMsg,0x67,iVp1Count * sizeof(PMSG_UNION_VIEWPORT_NOTIFY) + sizeof(PMSG_UNION_VIEWPORT_NOTIFY_COUNT));

		DataSend(lpObj->m_Index,(unsigned char *)lpMsg,((lpMsg->h.sizeL & 0xFF) & 0xFF | ((lpMsg->h.sizeH & 0xFF) & 0xFF) << 8) & 0xFFFF);
	}
	else
	{
		if(iVp1Count != 0)
		{
			LogAddTD("[Union ViewPort] ERROR : iVp1Count is OUT of BOUND: %d",iVp1Count);
		}
	}
}



void gObjNotifyUpdateUnionV2(LPOBJ lpObj)
{
	if(lpObj == 0)
	{
		return;
	}

	if(lpObj->lpGuild == 0)
	{
		return;
	}

	char cBUFFER_V2[100] = {0};

	PMSG_UNION_VIEWPORT_NOTIFY_COUNT * lpMsg2 = (PMSG_UNION_VIEWPORT_NOTIFY_COUNT *)&cBUFFER_V2;
	PMSG_UNION_VIEWPORT_NOTIFY * lpMsgBody2 = (PMSG_UNION_VIEWPORT_NOTIFY *)&cBUFFER_V2[sizeof(PMSG_UNION_VIEWPORT_NOTIFY_COUNT)];

	gObjGetGuildUnionName(lpObj,lpMsgBody2->szUnionName,sizeof(lpMsgBody2->szUnionName));
	lpMsgBody2->btNumberL = SET_NUMBERL(WORD(lpObj->m_Index));
	lpMsgBody2->btNumberH = SET_NUMBERH(WORD(lpObj->m_Index));

	lpMsgBody2->iGuildNumber = lpObj->lpGuild->Number;
	lpMsgBody2->btGuildRelationShip = 0;
	lpMsg2->btCount = 1;

	PHeadSetW((unsigned char *)lpMsg2,0x67,sizeof(lpMsg2) + sizeof(lpMsgBody2[0])+1);

	DataSend(lpObj->m_Index,(unsigned char *)lpMsg2,((lpMsg2->h.sizeL & 0xFF) & 0xFF | ((lpMsg2->h.sizeH & 0xFF) & 0xFF) << 8) & 0xFFFF);

	for(int n = 0; n < MAX_VIEWPORT; n++)
	{
		if(lpObj->VpPlayer2[n].type == OBJ_USER && lpObj->VpPlayer2[n].state != 0)
		{
			LPOBJ lpTargetObj = &gObj[lpObj->VpPlayer2[n].number];

			if(lpTargetObj->lpGuild != 0)
			{
				lpMsgBody2->btGuildRelationShip = gObjGetRelationShip(lpTargetObj,lpObj);
			}

			if(lpMsgBody2->btGuildRelationShip != 1)
			{
				DataSend(lpObj->VpPlayer2[n].number,(unsigned char *)lpMsg2,((lpMsg2->h.sizeL & 0xFF) & 0xFF | ((lpMsg2->h.sizeH & 0xFF) & 0xFF) << 8) & 0xFFFF);
			}
		}
	}
}







void gObjUnionUpdateProc(int iIndex)
{
	if ( gObjIsConnected(iIndex) == FALSE )
		return;

	LPOBJ lpObj = &gObj[iIndex];

	if ( lpObj->Type != OBJ_USER )
		return;

	if ( lpObj->RegenOk > 0 )
		return;

	if ( lpObj->CloseCount > -1 )
		return;

	if ( lpObj->lpGuild == NULL )
		return;

	if ( lpObj->m_bMapSvrMoveQuit == true || lpObj->m_bMapAntiHackMove == true )
		return;

	if ( lpObj->lpGuild->CheckTimeStamp(lpObj->iGuildUnionTimeStamp) != FALSE )
		return;

	lpObj->iGuildUnionTimeStamp = lpObj->lpGuild->GetTimeStamp();
	gObjNotifyUpdateUnionV1(lpObj);
	gObjNotifyUpdateUnionV2(lpObj);

}

// _GUILD_INFO_STRUCT 0x4e0a10







void gObjSetKillCount(int aIndex, int iOption)	// Option : [0 : SetToZero] [1 : Increase] [2 : Decrease]
{
	if ( gObjIsConnected(aIndex) == FALSE )
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if ( iOption== 1 )
	{
		if ( lpObj->m_btKillCount <= 254 )
		{
			lpObj->m_btKillCount++;
		}
	}
	else if ( iOption == 2 )
	{
		if ( lpObj->m_btKillCount > 0 )
		{
			lpObj->m_btKillCount--;
		}
	}
	else if ( iOption == 0 )
	{
		lpObj->m_btKillCount = 0;
	}

	PMSG_KILLCOUNT pMsg = {0};
	PHeadSubSetB((LPBYTE)&pMsg, 0xB8,0x01, sizeof(pMsg));
	pMsg.btKillCount = lpObj->m_btKillCount;
	
	DataSend( aIndex, (UCHAR*)&pMsg, sizeof(pMsg));
}

struct PMSG_NOTIFY_REGION_OF_WEAPON {

  // static data ------------------------------------

  // non-static data --------------------------------
  /*<thisrel this+0x0>*/ /*|0x4|*/ struct PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x1|*/ unsigned char btWeaponType;
  /*<thisrel this+0x5>*/ /*|0x1|*/ unsigned char btPointX;
  /*<thisrel this+0x6>*/ /*|0x1|*/ unsigned char btPointY;

  // base classes -----------------------------------

  // friends ----------------------------------------

  // static functions -------------------------------

  // non-virtual functions --------------------------

  // virtual functions ------------------------------
};
// <size 0x7>

struct PMSG_NOTIFY_TARGET_OF_WEAPON {

  // static data ------------------------------------

  // non-static data --------------------------------
  /*<thisrel this+0x0>*/ /*|0x4|*/ struct PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x1|*/ unsigned char NumberH;
  /*<thisrel this+0x5>*/ /*|0x1|*/ unsigned char NumberL;
  /*<thisrel this+0x6>*/ /*|0x1|*/ unsigned char btWeaponType;

  // base classes -----------------------------------

  // friends ----------------------------------------

  // static functions -------------------------------

  // non-virtual functions --------------------------

  // virtual functions ------------------------------
};
// <size 0x7>

struct PMSG_WEAPON_OWNER_VIEWPORT_NOTIFY_COUNT {

  // static data ------------------------------------

  // non-static data --------------------------------
  /*<thisrel this+0x0>*/ /*|0x4|*/ struct PWMSG_HEAD h;
  /*<thisrel this+0x4>*/ /*|0x1|*/ unsigned char btCount;

  // base classes -----------------------------------

  // friends ----------------------------------------

  // static functions -------------------------------

  // non-virtual functions --------------------------

  // virtual functions ------------------------------
};
// <size 0x5>

struct PMSG_WEAPON_OWNER_VIEWPORT_NOTIFY {

  // static data ------------------------------------

  // non-static data --------------------------------
  /*<thisrel this+0x0>*/ /*|0x1|*/ unsigned char btObjType;
  /*<thisrel this+0x1>*/ /*|0x1|*/ unsigned char btObjClassH;
  /*<thisrel this+0x2>*/ /*|0x1|*/ unsigned char btObjClassL;
  /*<thisrel this+0x3>*/ /*|0x1|*/ unsigned char btObjIndexH;
  /*<thisrel this+0x4>*/ /*|0x1|*/ unsigned char btObjIndexL;
  /*<thisrel this+0x5>*/ /*|0x1|*/ unsigned char btX;
  /*<thisrel this+0x6>*/ /*|0x1|*/ unsigned char btY;
  /*<thisrel this+0x7>*/ /*|0x11|*/ unsigned char CharSet[17];
  /*<thisrel this+0x18>*/ /*|0x4|*/ unsigned long dwViewSkillState;

  // base classes -----------------------------------

  // friends ----------------------------------------

  // static functions -------------------------------

  // non-virtual functions --------------------------

  // virtual functions ------------------------------
};
// <size 0x1c>

void gObjNotifyUseWeaponV1(LPOBJ lpOwnerObj, LPOBJ lpWeaponObj, int iTargetX, int iTargetY)
{
	if(lpOwnerObj == 0)
	{
		return;
	}

	if(lpWeaponObj == 0)
	{
		return;
	}

	PMSG_NOTIFY_REGION_OF_WEAPON pNotifyRegionMsg = {0};

	PHeadSubSetB((unsigned char *)&pNotifyRegionMsg,0xB7,2,sizeof(pNotifyRegionMsg));

	pNotifyRegionMsg.btPointX = iTargetX;
	pNotifyRegionMsg.btPointY = iTargetY;

	PMSG_NOTIFY_TARGET_OF_WEAPON pNotifyTargetMsg = {0};

	PHeadSubSetB((unsigned char *)&pNotifyTargetMsg,0xB7,3,sizeof(pNotifyTargetMsg));

	if(lpWeaponObj->Class == 0xDD)
	{
		pNotifyRegionMsg.btWeaponType = 1;
		pNotifyTargetMsg.btWeaponType = 1;
	}
	else if(lpWeaponObj->Class == 0xDE)
	{
		pNotifyRegionMsg.btWeaponType = 2;
		pNotifyTargetMsg.btWeaponType = 2;
	}

	char cBUFFER_V1[6000];
	int iVp1Count = 0;

	memset(cBUFFER_V1,0x00,sizeof(cBUFFER_V1));

	PMSG_WEAPON_OWNER_VIEWPORT_NOTIFY_COUNT * lpMsg = (PMSG_WEAPON_OWNER_VIEWPORT_NOTIFY_COUNT *)&cBUFFER_V1;
	PMSG_WEAPON_OWNER_VIEWPORT_NOTIFY * lpMsgBody = (PMSG_WEAPON_OWNER_VIEWPORT_NOTIFY *)&cBUFFER_V1[sizeof(PMSG_WEAPON_OWNER_VIEWPORT_NOTIFY_COUNT)];

	for(int n = 0; n < OBJMAX;n++)
	{
		LPOBJ lpTargetObj = &gObj[n];

		if(gObjIsConnected(n) == 0)
		{
			continue;
		}

		if(lpOwnerObj->MapNumber != lpTargetObj->MapNumber)
		{
			continue;
		}

		if(abs(lpTargetObj->X - iTargetX) > 6)
		{
			continue;
		}

		if(abs(lpTargetObj->Y - iTargetY) > 6)
		{
			continue;
		}

		lpMsgBody[iVp1Count].btObjClassH = SET_NUMBERH(lpTargetObj->Class);
		lpMsgBody[iVp1Count].btObjClassL = SET_NUMBERL(lpTargetObj->Class);
		lpMsgBody[iVp1Count].btObjIndexH = SET_NUMBERH(lpTargetObj->m_Index);
		lpMsgBody[iVp1Count].btObjIndexH = SET_NUMBERL(lpTargetObj->m_Index);
		// lpMsgBody[iVp1Count].btObjIndexL = SET_NUMBERL(lpTargetObj->m_Index); need some testing here 
		lpMsgBody[iVp1Count].btX = lpTargetObj->X;
		lpMsgBody[iVp1Count].btY = lpTargetObj->Y;
		lpMsgBody[iVp1Count].dwViewSkillState = lpTargetObj->m_ViewSkillState;

		if(lpTargetObj->Type == OBJ_USER)
		{
			lpMsgBody[iVp1Count].btObjType = OBJ_USER;
			lpMsgBody[iVp1Count].btObjClassH = CS_SET_CLASS(lpTargetObj->Class);
			lpMsgBody[iVp1Count].btObjClassL = 0;

			memcpy(lpMsgBody[iVp1Count].CharSet,&lpTargetObj->CharSet[1],sizeof(lpMsgBody[iVp1Count].CharSet));
		}
		else if(lpTargetObj->Type == OBJ_MONSTER)
		{
			lpMsgBody[iVp1Count].btObjType = OBJ_MONSTER;
			memset(lpMsgBody[iVp1Count].CharSet,0x00,sizeof(lpMsgBody[iVp1Count].CharSet));
		}
		else
		{
			lpMsgBody[iVp1Count].btObjType = OBJ_NPC;
			memset(lpMsgBody[iVp1Count].CharSet,0x00,sizeof(lpMsgBody[iVp1Count].CharSet));
		}

		iVp1Count++;

		if(lpTargetObj->Type == OBJ_USER)
		{
			DataSend(lpTargetObj->m_Index,(unsigned char *)&pNotifyRegionMsg,sizeof(pNotifyRegionMsg));

			if(abs(lpTargetObj->X - iTargetX) > 3)
			{
				continue;
			}

			if(abs(lpTargetObj->Y - iTargetY) > 3)
			{
				continue;
			}

			pNotifyTargetMsg.NumberH = SET_NUMBERH(lpTargetObj->m_Index);
			pNotifyTargetMsg.NumberL = SET_NUMBERL(lpTargetObj->m_Index);
		}
	}

	if(iVp1Count > 0 && iVp1Count <= 100)
	{
		lpMsg->btCount = iVp1Count;
		PHeadSetW((unsigned char *)lpMsg,0x68,iVp1Count * sizeof(PMSG_WEAPON_OWNER_VIEWPORT_NOTIFY) + sizeof(PMSG_WEAPON_OWNER_VIEWPORT_NOTIFY_COUNT));

		DataSend(lpOwnerObj->m_Index,(unsigned char *)lpMsg,((lpMsg->h.sizeL & 0xFF) & 0xFF | ((lpMsg->h.sizeH & 0xFF) & 0xFF) << 8) & 0xFFFF);
	}
}







void gObjNotifyUseWeaponDamage(LPOBJ lpWeaponObj, int iTargetX, int iTargetY)
{
	if ( lpWeaponObj->Class == 221 || lpWeaponObj->Class == 222)
	{
		for ( int i=0;i<OBJMAX;i++)
		{
			LPOBJ lpObj = &gObj[i];

			if ( lpWeaponObj->MapNumber != lpObj->MapNumber )
			{
				continue;
			}

			if ( abs(lpObj->X - iTargetX) > 3 )
			{
				continue;
			}

			if ( abs(lpObj->Y - iTargetY) > 3 )
			{
				continue;
			}

			g_CsNPC_Weapon.AddWeaponDamagedTargetInfo(lpWeaponObj->m_Index, i, 100);
		}
	}

}







void gObjUseBlessAndSoulPotion(int aIndex, int iItemLevel)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if ( iItemLevel == 0 )	// Bless Potion
	{
		lpObj->m_iPotionBlessTime = 120;
		lpObj->m_ViewSkillState |= 0x8000;
		GCSkillInfoSend(lpObj, 1, 0x0A);
	}
	else if ( iItemLevel == 1 ) // Soul Potion
	{
		lpObj->m_iPotionSoulTime = 60;
		lpObj->m_ViewSkillState |= 0x10000;
		GCSkillInfoSend(lpObj,  1, 0x0B);
		gObjUseDrink(lpObj, 0x4D);
	}
}








void gObjWeaponDurDownInCastle(LPOBJ lpObj, LPOBJ lpTargetObj, int iDecValue)
{
	if(lpObj->Type == OBJ_MONSTER || lpObj->Type == OBJ_NPC)
	{
		return;
	}

#if(ENABLE_CSHOP == 1)
	if(MuItemShop.IsCShopItem(lpObj->m_Index,0) == true)
		return;
	if(MuItemShop.IsCShopItem(lpObj->m_Index,1) == true)
		return;
#endif

	CItem * Right = &lpObj->pInventory[0];
	CItem * Left = &lpObj->pInventory[1];

	int bIsRightDurDown = 0;
	int bIsLeftDurDown = 0;


	if( lpObj->Class == CLASS_KNIGHT || 
		lpObj->Class == CLASS_MAGICGLADIATOR || 
		lpObj->Class == CLASS_DARKLORD || 
		lpObj->Class == CLASS_RAGEFIGHTER )
	{
		if(Right->m_Type >= ITEMGET(0,0) && Right->m_Type < ITEMGET(4,0)
			&& Left->m_Type >= ITEMGET(0,0) && Left->m_Type < ITEMGET(4,0))
		{
			bIsRightDurDown = 1;
			bIsLeftDurDown = 1;
		}
	}

	if(Left->m_Type >= ITEMGET(4,0) && Left->m_Type < ITEMGET(4,7)
		|| Left->m_Type == ITEMGET(4,20)
		|| Left->m_Type == ITEMGET(4,21)
		|| Left->m_Type == ITEMGET(4,22)
		|| Left->m_Type == ITEMGET(4,23))
	{
		bIsLeftDurDown = 1;
	}
	else if(Right->m_Type >= ITEMGET(4,8) && Right->m_Type < ITEMGET(4,15) || Right->m_Type >= ITEMGET(4,16) && Right->m_Type < ITEMGET(5,0))
	{
		bIsRightDurDown = 1;
	}
	else if(Right->m_Type >= 0 && Right->m_Type < ITEMGET(4,0))
	{
		bIsRightDurDown = 1;
	}
	else if(Right->m_Type >= ITEMGET(5,0) && Right->m_Type < ITEMGET(6,0))
	{
		bIsRightDurDown = 1;
	}

	if(bIsRightDurDown != 0)
	{
		int iRet = Right->SimpleDurabilityDown(iDecValue);

		if(iRet != 0)
		{
			GCItemDurSend(lpObj->m_Index,0,Right->m_Durability,0);
		}

		if(lpObj->pInventory[0].m_Durability == 0)
		{
			if ((lpObj->pInventory[0].GetNumber() == 0xFFFFFFFF && ReadConfig.DeleteFFFFFFFFSerial == TRUE) ||
				(lpObj->pInventory[0].GetNumber() == 0xFFFFFFFE && ReadConfig.DeleteFFFFFFFESerial == TRUE) ||
				(lpObj->pInventory[0].GetNumber() == 0xFFFFFFFD && ReadConfig.DeleteFFFFFFFDSerial == TRUE) ||
				(lpObj->pInventory[0].GetNumber() == 0xFFFFFFFC && ReadConfig.DeleteFFFFFFFCSerial == TRUE) )
			{
				GCServerMsgStringSend("Promo item has Vanished!", lpObj->m_Index, 1);
				LogAddTD(lMsg.Get(MSGGET(2, 10)), lpObj->AccountID, lpObj->Name,
					lpObj->pInventory[0].GetName(), lpObj->pInventory[0].m_Level);
				gObjInventoryDeleteItem(lpObj->m_Index, 0);
				GCInventoryItemDeleteSend(lpObj->m_Index, 0, 0);
				gObjMakePreviewCharSet(lpObj->m_Index);
				GCEquipmentChange(lpObj->m_Index,0);
			}
		}

		if(iRet == 2)
		{
			gObjCalCharacter(lpObj->m_Index);
		}
	}

	if(bIsLeftDurDown != 0)
	{
		int iRet = Left->SimpleDurabilityDown(iDecValue);

		if(iRet != 0)
		{
			GCItemDurSend(lpObj->m_Index,1,Left->m_Durability,0);
		}

		if(lpObj->pInventory[1].m_Durability == 0)
		{
			if ((lpObj->pInventory[1].GetNumber() == 0xFFFFFFFF && ReadConfig.DeleteFFFFFFFFSerial == TRUE) ||
				(lpObj->pInventory[1].GetNumber() == 0xFFFFFFFE && ReadConfig.DeleteFFFFFFFESerial == TRUE) ||
				(lpObj->pInventory[1].GetNumber() == 0xFFFFFFFD && ReadConfig.DeleteFFFFFFFDSerial == TRUE) ||
				(lpObj->pInventory[1].GetNumber() == 0xFFFFFFFC && ReadConfig.DeleteFFFFFFFCSerial == TRUE) )
			{
				GCServerMsgStringSend("Promo item has Vanished!", lpObj->m_Index, 1);
				LogAddTD(lMsg.Get(MSGGET(2, 10)), lpObj->AccountID, lpObj->Name,
					lpObj->pInventory[1].GetName(), lpObj->pInventory[1].m_Level);
				gObjInventoryDeleteItem(lpObj->m_Index, 1);
				GCInventoryItemDeleteSend(lpObj->m_Index, 1, 0);
				gObjMakePreviewCharSet(lpObj->m_Index);
				GCEquipmentChange(lpObj->m_Index,1);
			}
		}

		if(iRet == 2)
		{
			gObjCalCharacter(lpObj->m_Index);
		}
	}
}


void gObjCheckTimeOutValue(LPOBJ lpObj, DWORD& rNowTick)
{
	long lTick;
	long lSecond;

	lTick = rNowTick - lpObj->m_dwLastCheckTick;
	lSecond = lTick / 1000;

	if(lSecond == 0)
	{
		return;
	}

	lpObj->m_dwLastCheckTick = rNowTick;
}




void MsgOutput(int aIndex, char* msg, ...) 
{
	__try
	{
	char szBuffer[512]="";
	va_list pArguments;

	va_start(pArguments, msg );
	vsprintf(&szBuffer[0], msg, pArguments);
	va_end(pArguments);
	
	GCServerMsgStringSend(&szBuffer[0], aIndex, 1);
	}__except( EXCEPTION_ACCESS_VIOLATION == GetExceptionCode() )
	{
	}
}


void PlayerRepairOverflow2ndClass(LPOBJ lpObj)
{
	//Class Overflow Fix Section
	if ((lpObj->DbClass>=DB_GRAND_MASTER)&&	(lpObj->DbClass<DB_DARK_KNIGHT)) 
	{
		lpObj->DbClass = DB_SOUL_MASTER;
	} 
	else if ((lpObj->DbClass>=DB_BLADE_MASTER)&&(lpObj->DbClass<DB_FAIRY_ELF)) 
	{
		lpObj->DbClass = DB_BLADE_KNIGHT;
	} 
	else if ((lpObj->DbClass>=DB_HIGH_ELF)&&(lpObj->DbClass<DB_MAGIC_GLADIATOR)) 
	{
		lpObj->DbClass = DB_MUSE_ELF;
	} 
	else if ((lpObj->DbClass>=DB_DIMENSION_MASTER)&&(lpObj->DbClass<DB_RAGEFIGHER)) 
	{
		lpObj->DbClass = DB_BLOODY_SUMMONER;
	}
}

void PlayerRepairOverflow3rdClass(LPOBJ lpObj)
{
	//Class Overflow Fix Section
	if ((lpObj->DbClass>DB_GRAND_MASTER)&&(lpObj->DbClass<DB_DARK_KNIGHT)) 
	{
		lpObj->DbClass = DB_GRAND_MASTER;
	} 
	else if ((lpObj->DbClass>DB_BLADE_MASTER)&&(lpObj->DbClass<DB_FAIRY_ELF)) 
	{
		lpObj->DbClass = DB_BLADE_MASTER;
	} 
	else if ((lpObj->DbClass>DB_HIGH_ELF)&&(lpObj->DbClass<DB_MAGIC_GLADIATOR)) 
	{
		lpObj->DbClass = DB_HIGH_ELF;
	} 
	else if ((lpObj->DbClass>DB_DUEL_MASTER+1)&&(lpObj->DbClass<DB_DARK_LORD)) 
	{	//Magic Gladiator +1
		lpObj->DbClass = DB_DUEL_MASTER+1;
	} 
	else if ((lpObj->DbClass>DB_LORD_EMPEROR+1)&&(lpObj->DbClass<DB_SUMMONER)) 
	{	//Dark Lord +1
		lpObj->DbClass = DB_LORD_EMPEROR+1;
	} 
	else if ((lpObj->DbClass>DB_DIMENSION_MASTER)&&(lpObj->DbClass<DB_RAGEFIGHER)) 
	{	//Summoner
		lpObj->DbClass = DB_DIMENSION_MASTER;
	} 
	else if (lpObj->DbClass>DB_RAGEFIGHER_EVO) 
	{	//Rage Fighter
		lpObj->DbClass = DB_RAGEFIGHER_EVO;
	}
}



void gProcessAutoRecuperation(LPOBJ lpObj)
{
	if(lpObj->Life == (lpObj->MaxLife + lpObj->AddLife)
		&& lpObj->Mana == (lpObj->MaxMana + lpObj->AddMana)
		&& lpObj->BP == (lpObj->MaxBP + lpObj->AddBP) )
	{
		lpObj->m_iAutoRecuperationTime = GetTickCount();
		return;
	}

	if(( GetTickCount() - lpObj->m_iAutoRecuperationTime ) >= 10000 && ( GetTickCount() - lpObj->m_iAutoRecuperationTime ) < 15000)
	{
		if(lpObj->Life < ( lpObj->MaxLife + lpObj->AddLife ) )
		{
			lpObj->Life += 1.0f;

			if(lpObj->Life > (lpObj->MaxLife + lpObj->AddLife))
			{
				lpObj->Life = lpObj->MaxLife + lpObj->AddLife;
			}

			GCReFillSend(lpObj->m_Index,lpObj->Life,0xFF,0,lpObj->iShield);
		}

		if(lpObj->Mana < ( lpObj->MaxMana + lpObj->AddMana ) || lpObj->BP < ( lpObj->MaxBP + lpObj->AddBP) )
		{
			lpObj->Mana += 1.0f;

			if(lpObj->Mana > (lpObj->MaxMana + lpObj->AddMana))
			{
				lpObj->Mana = lpObj->MaxMana + lpObj->AddMana;
			}

			lpObj->BP += 1;

			if(lpObj->BP > (lpObj->MaxBP + lpObj->AddBP))
			{
				lpObj->BP = lpObj->MaxBP + lpObj->AddBP;
			}

			GCManaSend(lpObj->m_Index,lpObj->Mana,0xFF,0,lpObj->BP);
		}
	}
	else if(( GetTickCount() - lpObj->m_iAutoRecuperationTime ) >= 15000 && ( GetTickCount() - lpObj->m_iAutoRecuperationTime ) < 25000)
	{
		if(lpObj->Life < ( lpObj->MaxLife + lpObj->AddLife ) )
		{
			lpObj->Life += 5.0f;

			if(lpObj->Life > (lpObj->MaxLife + lpObj->AddLife))
			{
				lpObj->Life = lpObj->MaxLife + lpObj->AddLife;
			}

			GCReFillSend(lpObj->m_Index,lpObj->Life,0xFF,0,lpObj->iShield);
		}

		if(lpObj->Mana < ( lpObj->MaxMana + lpObj->AddMana ) || lpObj->BP < ( lpObj->MaxBP + lpObj->AddBP) )
		{
			lpObj->Mana += 5.0f;

			if(lpObj->Mana > (lpObj->MaxMana + lpObj->AddMana))
			{
				lpObj->Mana = lpObj->MaxMana + lpObj->AddMana;
			}

			lpObj->BP += 5;

			if(lpObj->BP > (lpObj->MaxBP + lpObj->AddBP))
			{
				lpObj->BP = lpObj->MaxBP + lpObj->AddBP;
			}

			GCManaSend(lpObj->m_Index,lpObj->Mana,0xFF,0,lpObj->BP);
		}
	}
	else if(( GetTickCount() - lpObj->m_iAutoRecuperationTime ) >= 25000)
	{
		if(lpObj->Life < ( lpObj->MaxLife + lpObj->AddLife ) )
		{
			lpObj->Life += 10.0f;

			if(lpObj->Life > (lpObj->MaxLife + lpObj->AddLife))
			{
				lpObj->Life = lpObj->MaxLife + lpObj->AddLife;
			}

			GCReFillSend(lpObj->m_Index,lpObj->Life,0xFF,0,lpObj->iShield);
		}

		if(lpObj->Mana < ( lpObj->MaxMana + lpObj->AddMana ) || lpObj->BP < ( lpObj->MaxBP + lpObj->AddBP) )
		{
			lpObj->Mana += 10.0f;

			if(lpObj->Mana > (lpObj->MaxMana + lpObj->AddMana))
			{
				lpObj->Mana = lpObj->MaxMana + lpObj->AddMana;
			}

			lpObj->BP += 10;

			if(lpObj->BP > (lpObj->MaxBP + lpObj->AddBP))
			{
				lpObj->BP = lpObj->MaxBP + lpObj->AddBP;
			}

			GCManaSend(lpObj->m_Index,lpObj->Mana,0xFF,0,lpObj->BP);
		}
	}
}


void gObjShieldAutoRefill(LPOBJ lpObj)
{
	int iRefillPoint = 0;
	int iShieldRefillOption = 0;

	if(g_ShieldSystemOn == 0)
	{
		return;
	}

	if(g_ShieldAutoRefillOn == 0)
	{
		return;
	}

	if(g_ShieldAutoRefillOnSafeZone == 1)
	{
		unsigned char btMapAttribute = MapC[lpObj->MapNumber].GetAttr(lpObj->X,lpObj->Y);

		if((btMapAttribute & 1) != 1 && lpObj->m_ItemOptionExFor380.OpRefillOn == 0)
		{
			lpObj->dwShieldAutoRefillTimer = GetTickCount();
			return;
		}
	}

	if(lpObj->iShield >= (lpObj->iMaxShield + lpObj->iAddShield))
	{
		lpObj->dwShieldAutoRefillTimer = GetTickCount();
		return;
	}

	iShieldRefillOption = lpObj->m_ItemOptionExFor380.OpAddRefillSD;

	int iRefillExpression = 0;

	float fRefillExpressionA = (lpObj->iMaxShield + lpObj->iAddShield) / 30;
	float fRefillExpressionB = iShieldRefillOption + 100;

	iRefillExpression = ((fRefillExpressionA * fRefillExpressionB) / 100.0f) / 25.0f;

	unsigned long dwTick = GetTickCount() - lpObj->dwShieldAutoRefillTimer;

	if(dwTick >= 25000)
	{
		iRefillPoint = iRefillExpression * 3; 
	}
	else if(dwTick >= 15000)
	{
		iRefillPoint = (iRefillExpression * 25) / 10;
	}
	else if(dwTick >= 10000)
	{
		iRefillPoint = iRefillExpression * 2;
	}
	else
	{
		return;
	}

	iRefillExpression++;

	if(iRefillPoint == 0)
	{
		return;
	}

	lpObj->iShield += iRefillPoint;

	if(lpObj->iShield > (lpObj->iMaxShield + lpObj->iAddShield))
	{
		lpObj->iShield = lpObj->iMaxShield + lpObj->iAddShield;
	}

	GCReFillSend(lpObj->m_Index,lpObj->Life,0xFF,0,lpObj->iShield);
}

int gObjCheckOverlapItemUsingDur(int iUserIndex, int iMaxOverlapped, int iItemType, int iItemLevel)
{
	if(OBJMAX_RANGE(iUserIndex) == 0)
	{
		return -1;
	}

	for(int x = INVETORY_WEAR_SIZE; x < ReadConfig.MAIN_INVENTORY_SIZE(iUserIndex,false); x++)
	{
		if(gObj[iUserIndex].pInventory[x].IsItem() == 1
			&& gObj[iUserIndex].pInventory[x].m_Type == (short)iItemType
			&& gObj[iUserIndex].pInventory[x].m_Level == (short)iItemLevel)
		{
			int iITEM_DUR = gObj[iUserIndex].pInventory[x].m_Durability;

			if((((iITEM_DUR)<0)?FALSE:((iITEM_DUR)>iMaxOverlapped-1)?FALSE:TRUE ))
			{
				return x;
			}
		}
	}
	return -1;
}

int gObjOverlapItemUsingDur(class CItem* lpItem, int iMapNumber, int iItemNumber, int iUserIndex, int iMaxOverlapped, int iItemType, int iItemLevel)
{
	if(OBJMAX_RANGE(iUserIndex) == 0)
	{
		return -1;
	}

	for(int iLoop = 0; iLoop < ReadConfig.MAIN_INVENTORY_SIZE(iUserIndex,false); iLoop ++)
	{
		int iInventoryIndex = gObjCheckOverlapItemUsingDur(iUserIndex,iMaxOverlapped,iItemType,iItemLevel);
		if(ReadConfig.MAIN_INVENTORY_RANGE(iUserIndex,iInventoryIndex,false))
		{
			int iItemDur = gObj[iUserIndex].pInventory[iInventoryIndex].m_Durability + lpItem->m_Durability;

			if(iItemDur <= iMaxOverlapped)
			{
				if(MapC[iMapNumber].ItemGive(iUserIndex,iItemNumber,1) == 1)
				{
					return iInventoryIndex;
				}
			}
			else
			{
				lpItem->m_Durability = iItemDur - iMaxOverlapped;
				gObj[iUserIndex].pInventory[iInventoryIndex].m_Durability = iMaxOverlapped;

				GCItemDurSend(iUserIndex,iInventoryIndex,gObj[iUserIndex].pInventory[iInventoryIndex].m_Durability,0);
			}
		}
		else
		{
			return -1;
		}
	}
	return -1;
}

int gObjCheckSerialSpecialItemList(class CItem* lpItem)
{
	if(lpItem->GetNumber() == 0xFFFFFFF0) 
	{
		return true;
	}
	if ((lpItem->GetNumber() == 0xFFFFFFFF && ReadConfig.ItemTradeFFFFBlock == TRUE) ||
		(lpItem->GetNumber() == 0xFFFFFFFE && ReadConfig.ItemTradeFFFEBlock == TRUE) ||
		(lpItem->GetNumber() == 0xFFFFFFFD && ReadConfig.ItemTradeFFFDBlock == TRUE) ||
		(lpItem->GetNumber() == 0xFFFFFFFC && ReadConfig.ItemTradeFFFCBlock == TRUE) )
	{
		return true;
	}
	if(MuItemShop.IsCShopItemByType(lpItem->m_Type,lpItem->m_ItemOptionEx) == true)
	{
		return true;
	}

	return false;
}

int gObjCheckSerial0ItemList(class CItem* lpItem)
{
	if(gItemSerialCheck == 0)
	{
		return false;
	}
	if(gItemZeroSerialCheck == 0)
	{
		return false;
	}

	if(lpItem->m_Type == ITEMGET(19,13)
		|| lpItem->m_Type == ITEMGET(19,14)
		|| lpItem->m_Type == ITEMGET(19,16)
		|| lpItem->m_Type == ITEMGET(19,22)
		|| lpItem->m_Type == ITEMGET(19,15)
		|| lpItem->m_Type == ITEMGET(19,30)
		|| lpItem->m_Type == ITEMGET(19,31)
		|| lpItem->m_Type == ITEMGET(19,31))
	{
		if(lpItem->GetNumber() == 0)
		{
			return true;
		}
	}
	return false;
}


int gObjCheckItemPos(int iIndex, short Type) 
{
	if(OBJMAX_RANGE(iIndex) == 0)
	{
		return -1;
	}

	for(int ItemPos = 0; ItemPos < MAIN_INVENTORY_NORMAL_SIZE /*76*/; ItemPos++)
	{
		if(gObj[iIndex].pInventory[ItemPos].m_Type == Type)
		{
			return ItemPos;
		}
	}
	return -1;
}

int gObjCheckItemPosByDur(int iIndex, short Type, BYTE Dur) 
{
	if(OBJMAX_RANGE(iIndex) == 0)
	{
		return -1;
	}

	for(int ItemPos = 0; ItemPos < MAIN_INVENTORY_NORMAL_SIZE /*76*/; ItemPos++)
	{
		if(gObj[iIndex].pInventory[ItemPos].m_Type == Type && gObj[iIndex].pInventory[ItemPos].m_Durability >= Dur)
		{
			return ItemPos;
		}
	}
	return -1;
}



int gObjCheckItemsCount(int iIndex, short Type, BYTE ItemLevel, BYTE ItemOpt, BYTE ItemLuck, BYTE ItemSkill) 
{
	if(OBJMAX_RANGE(iIndex) == 0)
	{
		return 0;
	}

	int count = 0;
	for(int ItemPos = 0; ItemPos < MAIN_INVENTORY_NORMAL_SIZE /*76*/; ItemPos++)
	{
		if(gObj[iIndex].pInventory[ItemPos].m_Type == Type && gObj[iIndex].pInventory[ItemPos].m_Level == ItemLevel && gObj[iIndex].pInventory[ItemPos].m_SkillOption == ItemSkill && gObj[iIndex].pInventory[ItemPos].m_LuckOption == ItemLuck && gObj[iIndex].pInventory[ItemPos].m_Z28Option == ItemOpt )
		{
			gObjInventoryDeleteItem(iIndex, ItemPos);
			GCInventoryItemDeleteSend(iIndex, ItemPos, 1);
			count++;
		}
	}
	return count;
}

int gObjCheckItemsCount(int iIndex, short Type, BYTE ItemLevel, BYTE ItemOpt, BYTE ItemLuck, BYTE ItemSkill, BYTE ItemExcellent, BYTE ItemAncient) 
{
	if(OBJMAX_RANGE(iIndex) == 0)
	{
		return 0;
	}

	int count = 0;
	for(int ItemPos = 0; ItemPos < MAIN_INVENTORY_NORMAL_SIZE /*76*/; ItemPos++)
	{
		if(gObj[iIndex].pInventory[ItemPos].m_Type == Type && gObj[iIndex].pInventory[ItemPos].m_Level == ItemLevel && gObj[iIndex].pInventory[ItemPos].m_SkillOption == ItemSkill && gObj[iIndex].pInventory[ItemPos].m_LuckOption == ItemLuck && gObj[iIndex].pInventory[ItemPos].m_Z28Option == ItemOpt && gObj[iIndex].pInventory[ItemPos].m_NewOption == ItemExcellent && gObj[iIndex].pInventory[ItemPos].m_SetOption == ItemAncient)
		{
			count++;
		}
	}
	return count;
}

int gObjDeleteItemsCount(int iIndex, short Type, BYTE ItemLevel, BYTE ItemOpt, BYTE ItemLuck, BYTE ItemSkill, BYTE ItemExcellent, BYTE ItemAncient,int HowMany) 
{
	if(OBJMAX_RANGE(iIndex) == 0)
	{
		return 0;
	}

	int count = 0;
	for(int ItemPos = 0; ItemPos < MAIN_INVENTORY_NORMAL_SIZE /*76*/; ItemPos++)
	{
		if(gObj[iIndex].pInventory[ItemPos].m_Type == Type && gObj[iIndex].pInventory[ItemPos].m_Level == ItemLevel && gObj[iIndex].pInventory[ItemPos].m_SkillOption == ItemSkill && gObj[iIndex].pInventory[ItemPos].m_LuckOption == ItemLuck && gObj[iIndex].pInventory[ItemPos].m_Z28Option == ItemOpt && gObj[iIndex].pInventory[ItemPos].m_NewOption == ItemExcellent && gObj[iIndex].pInventory[ItemPos].m_SetOption == ItemAncient)
		{
			if(count < HowMany)
			{
				gObjInventoryDeleteItem(iIndex, ItemPos);
				GCInventoryItemDeleteSend(iIndex, ItemPos, 1);
				count++;
			}else
			{
				return count;
			}
		}
	}
	return count;
}

BOOL DeleteItemByMultipleValues ( int iIndex, DWORD itemId, BYTE ItemLevel, BYTE ItemOpt, BYTE ItemLuck, BYTE ItemSkill )
{
	if(OBJMAX_RANGE(iIndex) == 0)
	{
		return false;
	}

	for ( BYTE ItemPos = 0 ; ItemPos < MAIN_INVENTORY_NORMAL_SIZE /*76*/ ; ItemPos ++ )
	{
		if ( gObj[iIndex].pInventory[ItemPos].m_Type == itemId )
		{
			if (  gObj[iIndex].pInventory[ItemPos].m_Level == ItemLevel && gObj[iIndex].pInventory[ItemPos].m_SkillOption == ItemSkill && gObj[iIndex].pInventory[ItemPos].m_LuckOption == ItemLuck && gObj[iIndex].pInventory[ItemPos].m_Z28Option == ItemOpt ) 
			{
				gObjInventoryDeleteItem ( iIndex , ItemPos );
				GCInventoryItemDeleteSend(iIndex, ItemPos, 1);
				return 1;
			}
		}
	}
	return 0;
}

void DeleteAllItems ( int iIndex )
{
	if(OBJMAX_RANGE(iIndex) == 0)
	{
		return;
	}

	for ( BYTE ItemPos = 0 ; ItemPos < MAIN_INVENTORY_NORMAL_SIZE /*76*/ ; ItemPos ++ )
	{
		if ( gObj[iIndex].pInventory[ItemPos].IsItem() == TRUE)
		{
			gObjInventoryDeleteItem ( iIndex , ItemPos );				
			GCInventoryItemDeleteSend(iIndex, ItemPos, 1);
		}
	}
}

void DeleteAllItemsLessInv ( int iIndex )
{
	if(OBJMAX_RANGE(iIndex) == 0)
	{
		return;
	}

	for ( BYTE ItemPos = INVETORY_WEAR_SIZE ; ItemPos < ReadConfig.MAIN_INVENTORY_SIZE(iIndex,false) ; ItemPos ++ )
	{
		if ( gObj[iIndex].pInventory[ItemPos].IsItem() == TRUE)
		{
			gObjInventoryDeleteItem ( iIndex , ItemPos );				
			GCInventoryItemDeleteSend(iIndex, ItemPos, 1);
		}
	}
}

BOOL DeleteItemByID ( int iIndex, DWORD itemId)
{
	if(OBJMAX_RANGE(iIndex) == 0)
	{
		return false;
	}

	for ( BYTE ItemPos = INVETORY_WEAR_SIZE ; ItemPos < MAIN_INVENTORY_NORMAL_SIZE /*76*/ ; ItemPos ++ )
	{
		if ( gObj[iIndex].pInventory[ItemPos].m_Type == itemId )
		{
			gObjInventoryDeleteItem ( iIndex , ItemPos );
			GCInventoryItemDeleteSend(iIndex, ItemPos, 1);
			return 1;
		}
	}
	return 0;
}

BOOL DeleteItemByLevel ( int iIndex, DWORD itemId, BYTE ItemLevel )
{
	if(OBJMAX_RANGE(iIndex) == 0)
	{
		return false;
	}

	for ( BYTE ItemPos = INVETORY_WEAR_SIZE ; ItemPos < MAIN_INVENTORY_NORMAL_SIZE /*76*/ ; ItemPos ++ )
	{
		if ( gObj[iIndex].pInventory[ItemPos].m_Type == itemId )
		{
			if (  gObj[iIndex].pInventory[ItemPos].m_Level == ItemLevel )
			{
				gObjInventoryDeleteItem ( iIndex , ItemPos );
				GCInventoryItemDeleteSend(iIndex, ItemPos, 1);
				return 1;
			}
		}
	}
	return 0;
}



BOOL gCheckSkillDistance(int aIndex, int aTargetIndex, int iSkillNum)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return false;
	}
	if(OBJMAX_RANGE(aTargetIndex) == 0)
	{
		return false;
	}

	if ( !g_iSkillDistanceCheck  )
		return TRUE;

	if ( iSkillNum == 40 )
		return TRUE;

	int iSkillDistance = MagicDamageC.GetSkillDistance(iSkillNum);

	if ( iSkillDistance == -1 )
		return FALSE;

	iSkillDistance += g_iSkillDistanceCheckTemp;
	LPOBJ lpObj = &gObj[aIndex];
	LPOBJ lpTargetObj = &gObj[aTargetIndex];

	if(lpObj->MapNumber != lpTargetObj->MapNumber)
	{
		if(ReadConfig.AHLog == TRUE)
		{
			ANTI_HACK_LOG.Output("[SKILL DISTANCE CHECK] [%s][%s] Invalid Skill attacker(%d, %d, %d), defender(%d, %d, %d), skillnum:%d",
				lpObj->AccountID, lpObj->Name,
				lpObj->MapNumber, lpObj->X, lpObj->Y, 
				lpTargetObj->MapNumber, lpTargetObj->X, lpTargetObj->Y,
				iSkillNum);
		}
		GCServerMsgStringSend("[Anti-Hack] Invalid Skill Map.",lpObj->m_Index, 0x01);
		return FALSE;
	}

	if ( abs(lpObj->X - lpTargetObj->X) > iSkillDistance ||
		 abs(lpObj->Y - lpTargetObj->Y) > iSkillDistance )
	{
		if(ReadConfig.AHLog == TRUE)
		{
			ANTI_HACK_LOG.Output("[SKILL DISTANCE CHECK] [%s][%s] Invalid Skill attacker(%d, %d), defender(%d, %d), skillnum:%d skilldistance:%d",
				lpObj->AccountID, lpObj->Name,
				lpObj->X, lpObj->Y, 
				lpTargetObj->X, lpTargetObj->Y,
				iSkillNum, iSkillDistance);
		}
		GCServerMsgStringSend("[Anti-Hack] Invalid Skill distance.",lpObj->m_Index, 0x01);

		return FALSE;
	}

	return TRUE;
}


void gObjSaveChaosBoxItemList(LPOBJ lpObj)
{
	unsigned char ExOption[0x8];
	for(int n = 0; n < CHAOS_BOX_SIZE;n++)
	{
		if(lpObj->pChaosBox[n].IsItem() == 1)
		{
			ItemIsBufExOption(ExOption,&lpObj->pChaosBox[n]);

			CHAOS_LOG.Output("[ChaosBoxItemList][Lost ItemList] [%s][%s] [%d,%s,%d,%d,%d,%d] Serial:[%d][%d] Ex:[%d,%d,%d,%d,%d,%d,%d] Set:[%d] 380:[%d] HO:[%d,%d]",
				lpObj->AccountID,lpObj->Name,n,lpObj->pChaosBox[n].GetName(),lpObj->pChaosBox[n].m_Level,lpObj->pChaosBox[n].m_SkillOption,lpObj->pChaosBox[n].m_LuckOption,lpObj->pChaosBox[n].m_Z28Option,lpObj->pChaosBox[n].m_Number,
				(int)lpObj->pChaosBox[n].m_Durability,ExOption[0],ExOption[1],ExOption[2],ExOption[3],ExOption[4],ExOption[5],ExOption[6],lpObj->pChaosBox[n].m_SetOption,
				lpObj->pChaosBox[n].m_ItemOptionEx >> 7,g_kJewelOfHarmonySystem.GetItemStrengthenOption(&lpObj->pChaosBox[n]),g_kJewelOfHarmonySystem.GetItemOptionLevel(&lpObj->pChaosBox[n]));
		}
	}
}

int gObjDeleteItemsCount(int aIndex, short Type, short Level, int Count) 
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return false;
	}

	int count = 0;
	for(int i = 0; i < MAIN_INVENTORY_NORMAL_SIZE /*76*/; i++)
	{
		if(gObj[aIndex].pInventory[i].m_Type == Type && gObj[aIndex].pInventory[i].m_Level == Level)
		{
			gObjInventoryDeleteItem(aIndex, i);
			GCInventoryItemDeleteSend(aIndex, i, 1);
			count++;

			if(Count == count)
				break;
		}
	}
	return count;
}

BOOL gObjGetRandomFreeArea(int iMapNumber, BYTE &cX, BYTE &cY, int iSX, int iSY, int iDX, int iDY, int iLoopCount)
{
	if( iSX <= 0 ) iSX = 1;
	if( iSY <= 0 ) iSY = 1;
	if( iDX <= 0 ) iDX = 1;
	if( iDY <= 0 ) iDY = 1;
	
	if( iSX >= 256 ) iSX = 255;
	if( iSY >= 256 ) iSY = 255;
	if( iDX >= 256 ) iDX = 255;
	if( iDY >= 256 ) iDY = 255;

	if( iLoopCount <= 0 ) iLoopCount = 1;

	while( (iLoopCount--) > 0 )
	{
		cX = iSX + (rand() % (iDX-iSX));
		cY = iSY + (rand() % (iDY-iSY));

		BYTE btMapAttr = MapC[iMapNumber].GetAttr(cX, cY);

		if( btMapAttr == 0 )
			return TRUE;
	}
	return FALSE;
}