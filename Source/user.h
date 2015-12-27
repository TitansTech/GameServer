// ------------------------------
// Decompiled by Deathway
// Date : 2007-05-09
// ------------------------------
#ifndef USER_H__
#define USER_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SkillDelay.h"
#include "ComboAttack.h"
#include "giocp.h"
#include "..\ggsvr\ggsvr.h"
#include "MagicInf.h"
#include "..\common\zzzitem.h"
#include "..\common\classdef.h"
#include "GuildClass.h"
#include "TDurMagicKeyChecker.h"
#include "TMonsterAIAgro.h"
#include "TMonsterSkillElementInfo.h"
#include "JewelOfHarmonySystem.h"
#include "ItemSystemFor380.h"
#include "stdafx.h"
#include "AntiAFK.h"

#define CRYWOLF_MAP_RANGE(map) ( ( ((map)) == MAP_INDEX_CRYWOLF_FIRSTZONE  )?TRUE:FALSE  )

#if (WL_PROTECT==1)
	#if  (PACK_EDITION>=1)
		#if (GS_CASTLE==0)
			#define					OBJMAX 13000 //7400
		#else
			#define					OBJMAX 5000
		#endif
	#else
		#if (GS_CASTLE==0)
			#define					OBJMAX 9500
		#else
			#define					OBJMAX 4000
		#endif
	
	#endif
#else
	#if (GS_CASTLE==0)
		#define OBJMAX				15000 //7400
	#else
		#define OBJMAX				5000
	#endif
#endif

#if (PACK_EDITION>=2)
	#define OBJMAXUSER				1000
#endif
#if  (PACK_EDITION==1)
	#define OBJMAXUSER				300
#endif
#if  (PACK_EDITION==0)
	#define OBJMAXUSER				100
#endif

#define MAX_SKILLTREE				60
#define OBJ_STARTUSERINDEX			(OBJMAX - OBJMAXUSER)
#define OBJ_MAXMONSTER				(OBJ_STARTUSERINDEX - 500)		//11500 //5800
#define OBJ_CALLMONCOUNT			(OBJ_STARTUSERINDEX - OBJ_MAXMONSTER)
#define MAX_MONSTER_SEND_MSG		10
#define MAX_MONSTER_SEND_ATTACK_MSG	50
#define MAX_MONSTER_TYPE			1024
#define MAX_GUILD_LEN				8
#define MAX_ACCOUNT_LEN				10
#define WAREHOUSE_NORMAL_SIZE		120
#define TRADE_BOX_SIZE				32

#if (TOEXDB == 0)
	#define MAGIC_SIZE				45 //before 60 but now i think must be 45
#else
	#define MAGIC_SIZE				80 //before 60 but now i think must be 45
#endif

//===================================================================================================

#define WAREHOUSE_NORMAL_SIZE		120
#define WAREHOUSE_S6E2_SIZE			240

#define INVETORY_WEAR_SIZE			12
#define INVENTORY_NORMAL_SIZE		108
#define MAIN_INVENTORY_NORMAL_SIZE	(INVENTORY_NORMAL_SIZE - PSHOP_SIZE)	// Not included PersonalSh
#define BASIC_INVENTORY_SIZE		(INVENTORY_NORMAL_SIZE - PSHOP_SIZE - INVETORY_WEAR_SIZE)
#define INVENTORY_BAG_START			INVETORY_WEAR_SIZE
#define TRADE_BOX_SIZE				32
#define PSHOP_SIZE					32

#define MAX_EXTENDED_INV			4
#define EXTENDED_INV_SIZE			32

#define BASIC_INV_HEIGHT			8
#define EXTENDED_INV_HEIGHT			4

//===================================================================================================

#define MAX_CHAR_LEVEL				1000
#define MAX_SELF_DEFENSE			5
#define MAX_ST_HIT_DAMAGE			40
#define MAX_ZEN						2000000000
#define MAX_WAREHOUSE_ZEN			2000000000
#define ACTIVE_ITEM_SEARCH			0
#define CHAOS_BOX_SIZE				32
#define CHAOS_BOX_MAP_SIZE			32

#define MAX_SD_VALUE				65535
#define MAX_BP_VALUE				65535
#define MAX_LIFE_VALUE				65535
#define MAX_MANA_VALUE				65535

#define MAX_DAMAGE_VALUE			65535

enum DB_CLASS_CODES {
	DB_DARK_WIZARD		= 0,
	DB_SOUL_MASTER		= 1,
	DB_GRAND_MASTER		= 2,

	DB_DARK_KNIGHT		= 16,
	DB_BLADE_KNIGHT		= 17,
	DB_BLADE_MASTER		= 18,

	DB_FAIRY_ELF		= 32,
	DB_MUSE_ELF			= 33,
	DB_HIGH_ELF			= 34,

	DB_MAGIC_GLADIATOR	= 48,
	DB_DUEL_MASTER		= 49,

	DB_DARK_LORD		= 64,
	DB_LORD_EMPEROR		= 65,

	DB_SUMMONER			= 80,
	DB_BLOODY_SUMMONER	= 81,
	DB_DIMENSION_MASTER = 82,

	DB_RAGEFIGHER		= 96,
	DB_RAGEFIGHER_EVO	= 97,
};

enum PROTO_CLASS_CODES {
	PR_DARK_WIZARD		= 0x0,
	PR_SOUL_MASTER		= 0x10,
	PR_GRAND_MASTER		= 0x18,

	PR_DARK_KNIGHT		= 0x20,
	PR_BLADE_KNIGHT		= 0x30,
	PR_BLADE_MASTER		= 0x38,

	PR_FAIRY_ELF		= 0x40,
	PR_MUSE_ELF			= 0x50,
	PR_HIGH_ELF			= 0x58,

	PR_MAGIC_GLADIATOR	= 0x60,
	PR_DUEL_MASTER		= 0x78,

	PR_DARK_LORD		= 0x80,
	PR_LORD_EMPEROR		= 0x98,

	PR_SUMMONER			= 0x0A0,
	PR_BLOODY_SUMMONER	= 0x0B0,
	PR_DIMENSION_MASTER = 0x0B8,

	PR_RAGEFIGHER		= 0xC0,
	PR_RAGEFIGHER_EVO	= 0xD8,
};

enum MAP_INDEX
{
	MAP_INDEX_LORENCIA = 0x0,
	MAP_INDEX_DUNGEON = 0x1,
	MAP_INDEX_DEVIAS = 0x2,
	MAP_INDEX_NORIA = 0x3,
	MAP_INDEX_LOSTTOWER = 0x4,
	MAP_INDEX_RESERVED = 0x5,
	MAP_INDEX_BATTLESOCCER = 0x6,
	MAP_INDEX_ATLANS = 0x7,
	MAP_INDEX_TARKAN = 0x8,
	MAP_INDEX_DEVILSQUARE = 0x9,
	MAP_INDEX_ICARUS = 0xa,
	MAP_INDEX_BLOODCASTLE1 = 0xb,
	MAP_INDEX_BLOODCASTLE2 = 0xc,
	MAP_INDEX_BLOODCASTLE3 = 0xd,
	MAP_INDEX_BLOODCASTLE4 = 0xe,
	MAP_INDEX_BLOODCASTLE5 = 0xf,
	MAP_INDEX_BLOODCASTLE6 = 0x10,
	MAP_INDEX_BLOODCASTLE7 = 0x11,
	MAP_INDEX_CHAOSCASTLE1 = 0x12,
	MAP_INDEX_CHAOSCASTLE2 = 0x13,
	MAP_INDEX_CHAOSCASTLE3 = 0x14,
	MAP_INDEX_CHAOSCASTLE4 = 0x15,
	MAP_INDEX_CHAOSCASTLE5 = 0x16,
	MAP_INDEX_CHAOSCASTLE6 = 0x17,
	MAP_INDEX_KALIMA1 = 0x18,
	MAP_INDEX_KALIMA2 = 0x19,
	MAP_INDEX_KALIMA3 = 0x1a,
	MAP_INDEX_KALIMA4 = 0x1b,
	MAP_INDEX_KALIMA5 = 0x1c,
	MAP_INDEX_KALIMA6 = 0x1d,
	MAP_INDEX_CASTLESIEGE = 0x1e,
	MAP_INDEX_CASTLEHUNTZONE = 0x1f,
	MAP_INDEX_DEVILSQUARE2 = 0x20,
	MAP_INDEX_AIDA = 0x21,
	MAP_INDEX_CRYWOLF_FIRSTZONE = 0x22,
	MAP_INDEX_CRYWOLF_SECONDZONE = 0x23,
	MAP_INDEX_KALIMA7 = 0x24,
	MAP_INDEX_KANTURU1 = 0x25,
	MAP_INDEX_KANTURU2 = 0x26,
	MAP_INDEX_KANTURU_BOSS = 0x27,
	MAP_INDEX_BLOODCASTLE8 = 0x34,
	MAP_INDEX_CHAOSCASTLE7 = 0x35,
	MAP_INDEX_BARRACKS = 41,
	MAP_INDEX_REFUGE = 42,
	MAP_INDEX_ILLUSIONTEMPLE1 = 45,
	MAP_INDEX_ILLUSIONTEMPLE2 = 46,
	MAP_INDEX_ILLUSIONTEMPLE3 = 47,
	MAP_INDEX_ILLUSIONTEMPLE4 = 48,
	MAP_INDEX_ILLUSIONTEMPLE5 = 49,
	MAP_INDEX_ILLUSIONTEMPLE6 = 50,
	MAP_INDEX_ELBELAND = 51,
	MAP_INDEX_SWAMP = 56,
	MAP_INDEX_RAKLION = 57,
	MAP_INDEX_RAKLIONBOSS = 58,
	MAP_INDEX_XMAS = 62,
	MAP_INDEX_VULCANUS = 63,
	MAP_INDEX_DUELMAP = 64,
	MAP_INDEX_DOUBLE_GOER1 = 65,
	MAP_INDEX_DOUBLE_GOER2 = 66,
	MAP_INDEX_DOUBLE_GOER3 = 67,
	MAP_INDEX_DOUBLE_GOER4 = 68,
	MAP_INDEX_EMPIREGUARDIAN1 = 69,
	MAP_INDEX_EMPIREGUARDIAN2 = 70,
	MAP_INDEX_EMPIREGUARDIAN3 = 71,
	MAP_INDEX_EMPIREGUARDIAN4 = 72,

	MAP_INDEX_EMPIREGUARDIAN5 = 73,
	MAP_INDEX_EMPIREGUARDIAN6 = 74,
	MAP_INDEX_EMPIREGUARDIAN7 = 75,

	MAP_INDEX_LORRENMARKET = 79,
	MAP_INDEX_KARLUTAN1 = 80,
	MAP_INDEX_KARLUTAN2 = 81,
};

enum VIEW_STATE_INDEX
{
	eVS_INVISIBLE = 27,
	eVS_TRANSPARENCY = 18,
	eVS_DUEL_INTERFACE = 98,
	eVS_DUEL_MEDAL = 103,
};



enum EXP_GETTING_EVENT_TYPE {
	EVENT_TYPE_NONE = 0,
	EVENT_TYPE_PARTY = 1,
	EVENT_TYPE_DEVILSQUARE = 2,
	EVENT_TYPE_DEVILSQUARE_PARTY = 3,
	EVENT_TYPE_CHAOSCASTLE = 4,
	EVENT_TYPE_BLOODCASTLE = 5,
	EVENT_TYPE_CRYWOLF = 6,
	EVENT_TYPE_AUTOEXP = 10,
	EVENT_TYPE_EXTENDED_PVPSYSTEM = 11,
	EVENT_TYPE_SEASON5_QUEST = 12
};








#define MAX_RESISTENCE_TYPE 7

#define R_ICE			0
#define R_POISON		1
#define R_LIGHTNING		2
#define R_FIRE			3
#define R_EARTH			4
#define R_WIND			5
#define R_WATER			6



#define OBJ_EMPTY -1
#define OBJ_MONSTER 2
#define OBJ_USER 1
#define OBJ_NPC	3
#define MAX_MAGIC 45					//#define MAX_MAGIC 60 //--> Original // I think now must be 45
#define MAX_VIEWPORT 75					//BEFORE 75	=> 50
#define MAX_VIEWPORT_MONSTER 20			//BEFORE 20 => 10
#define MAX_ROAD_PATH_TABLE 16
#define MAX_ARRAY_FRUSTRUM 4
#define PLAYER_EMPTY  0
#define PLAYER_CONNECTED 1
#define PLAYER_LOGGED 2
#define PLAYER_PLAYING 3
#define MAX_CHECKSUM_KEY 1024
#define OBJMAX_RANGE(aIndex) ( ((aIndex) < 0 )?FALSE:( (aIndex) > OBJMAX-1 )?FALSE:TRUE   )
#define FRIEND_SERVER_STATE_LOGIN_FAIL	0
#define FRIEND_SERVER_STATE_OFFLINE		1
#define FRIEND_SERVER_STATE_ONLINE		2

//#define MAIN_INVENTORY_NORMAL_RANGE(x) (((x)<0)?FALSE:((x)>MAIN_INVENTORY_NORMAL_SIZE-1)?FALSE:TRUE )
//#define INVENTORY_RANGE(x) (((x)<0)?FALSE:((x)>INVENTORY_SIZE-1)?FALSE:TRUE )
#define WAREHOUSE_NORMAL_RANGE(x) (((x)<0)?FALSE:((x)>WAREHOUSE_NORMAL_SIZE-1)?FALSE:TRUE )
#define WAREHOUSE_S6E2_RANGE(x) (((x)<0)?FALSE:((x)>WAREHOUSE_S6E2_SIZE-1)?FALSE:TRUE )
#define TRADE_BOX_RANGE(x) (((x)<0)?FALSE:((x)>TRADE_BOX_SIZE-1)?FALSE:TRUE )
#define OBJMON_RANGE(x) ( ((x)<0)?FALSE:((x)>OBJ_MAXMONSTER-1)?FALSE:TRUE)
//#define PSHOP_RANGE(x) ( (((x)-MAIN_INVENTORY_NORMAL_SIZE)<0)?FALSE:(((x)-MAIN_INVENTORY_NORMAL_SIZE)>(INVENTORY_SIZE-MAIN_INVENTORY_NORMAL_SIZE-1))?FALSE:TRUE )
#define ATTRIBUTE_RANGE(x) (((x)<51)?FALSE:((x)>58)?FALSE:TRUE )

#define CURRENT_DB_VERSION	3
/*
 *	CHAR SET
 *	
 *	It Contains the Character Settings
 *	CS_CLASS(0)	Contains the Class and the ChangeUP
 *				You can use CS_GET_CLASS and CS_GET_CHANGEUP
 *	MAP of CHAR SET
 *
 *	    0         1			2		  3			4		  5			6		  7
 *	0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000
 *	AAAB   CC EEEE EEEE GGGG GGGG JJJJ MMMM PPPP SSSS VVVV WWWW XXXX XXXX YYYY YYYY
 *
 *	    8         9			10		  11		12		  13		14		  15
 *	0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000
 *	ZZZZ ZZZZ ILOR U    aaaa aaad bbbb bbbc DDDD    e FFFF HHHH KKKK NNNN QQQQ TTTT
 *
 *		16		  17
 *	0000 0000 0000 0000
 *	       gf         
 *
 *	AAA Contain Class Character Definition (DK:1 DW:0 ELF:2 MG:3 DL:4)
 *	B Contains the Change UP (0:Normal 1:Evolved)
 *	CC Contains a value Acording m_ActionNumber fron gObj
 *
 *	DDDD Contains the 8..11 bits of Rigth Hand Item Index
 *	EEEE EEEE Contains the 0..7 bits Rigth Hand Item Index
 *
 *	FFFF Contains the 8..11 bits of Left Hand Item Index
 *	GGGG GGGG Contains the 0..7 bits of Left Hand Item Index
 *
 *	HHHH Contains the 5..8 Bits from Helmet Item Index
 *	I Contains the 4 bit from Helmet Item Index
 *	JJJJ Contains the 0..3 bits from Helmet Item Index
 *
 *	KKKK Contains the 5..8 Bits from Armor Item Index
 *	L Contains the 4 bit from Armor Item Index
 *	MMMM Contains the 0..3 bits from Armor Item Index
 *
 *	NNNN Contains the 5..8 Bits from Pants Item Index
 *	O Contains the 4 bit from Pants Item Index
 *	PPPP Contains the 0..3 bits from Pants Item Index
 *
 *	QQQQ Contains the 5..8 Bits from Gloves Item Index
 *	R Contains the 4 bit from Gloves Item Index
 *	SSSS Contains the 0..3 bits from Gloves Item Index
 *
 *	TTTT Contains the 5..8 Bits from Boots Item Index
 *	U Contains the 4 bit from Boots Item Index
 *	VVVV Contains the 0..3 bits from Boots Item Index
 *
 *	WWWW Contains Wings and Helper
 *
 *	XXXX XXXX Contains Levels (Shine Levels) from Wings and Boots
 *	YYYY YYYY Contains Levels (Shine Levels) from Gloves and Pants and Armor
 *	ZZZZ ZZZZ Contains Levels (Shine Levels) from Helmet , Right and Left HAnd
 *
 *	aaaa aaa Contains the If the item is Excellent
 *
 *	bbbb bbb Contains the If the item is SetItem
 *	c	1 if the Char have the Full Set Item
 *
 *	d Unkown
 *
 *	e Have Dark Horse?
 *
 *	f	Fenrir Inc Attack Damage
 *	g	Fenrir Dec Attack Damage
 */
#define CHAR_SET_SIZE		18

	#define CS_CLASS		0

	#define CS_WEAPON1_TYPE	1
	#define CS_WEAPON2_TYPE	2

	#define CS_WEAPON1_DATA	12
	#define CS_WEAPON2_DATA	13

	#define CS_HELMET1		13
	#define CS_HELMET2		9
	#define CS_HELMET3		3

	#define CS_ARMOR1		14
	#define CS_ARMOR2		9
	#define CS_ARMOR3		3

	#define CS_PANTS1		14
	#define CS_PANTS2		9
	#define CS_PANTS3		4

	#define CS_GLOVES1		15
	#define CS_GLOVES2		9
	#define CS_GLOVES3		4

	#define CS_BOOTS1		15
	#define CS_BOOTS2		9
	#define CS_BOOTS3		5



#define CHECK_LIMIT(value, limit) ( ((value)<0)?FALSE:((value)>((limit)-1))?FALSE:TRUE  )
#define CHECK_CLASS(value, type) ( ((value)!=(type))?FALSE:TRUE )


#define CS_GET_CLASS(x) ( (((x)>>4)<<5)&(0xE0) )
#define CS_GET_CHANGEUP(x) ( ((x)&0x07) )
#define CS_SET_CLASS(x) ( ((x)<<5)& 0xE0 )
#define CS_SET_CHANGEUP(x) ( ((x) << 4) & 0x10 )

#define CS_SET_HELMET1(x) ( ((x) & 0x1E0) >> 5 )
#define CS_SET_HELMET2(x) ( ((x) & 0x10 ) << 3 )
#define CS_SET_HELMET3(x) ( ((x) & 0x0F ) << 4 )

#define CS_SET_ARMOR1(x) ( ((x) & 0x1E0) >> 1 )
#define CS_SET_ARMOR2(x) ( ((x) & 0x10 ) << 2 )
#define CS_SET_ARMOR3(x) ( ((x) & 0x0F )      )

#define CS_SET_PANTS1(x) ( ((x) & 0x1E0) >> 5 )
#define CS_SET_PANTS2(x) ( ((x) & 0x10 ) << 1 )
#define CS_SET_PANTS3(x) ( ((x) & 0x0F ) << 4 )

#define CS_SET_GLOVES1(x) ( ((x) & 0x1E0) >> 1 )
#define CS_SET_GLOVES2(x) ( ((x) & 0x10 )      )
#define CS_SET_GLOVES3(x) ( ((x) & 0x0F )      )

#define CS_SET_BOOTS1(x) ( ((x) & 0x1E0) >> 5 )
#define CS_SET_BOOTS2(x) ( ((x) & 0x10 ) >> 1 )
#define CS_SET_BOOTS3(x) ( ((x) & 0x0F ) << 4 )

#define CS_SET_WING1(x)  ( ((x) & 0x03 ) << 2 )
#define CS_SET_HELPER(x) ( ((x) & 0x03 )      )

#define CS_SET_SMALLLEVEL_RH(x)		( (x)       )
#define CS_SET_SMALLLEVEL_LH(x)		( (x) << 3  ) 
#define CS_SET_SMALLLEVEL_HELMET(x)	( (x) << 6  )
#define CS_SET_SMALLLEVEL_ARMOR(x)	( (x) << 9  )
#define CS_SET_SMALLLEVEL_PANTS(x)	( (x) << 12 )
#define CS_SET_SMALLLEVEL_GLOVES(x)	( (x) << 15 )
#define CS_SET_SMALLLEVEL_BOOTS(x)	( (x) << 18 )

#define CS_SET_SMALLLEVEL1(x)		( ((x) >> 16) & 0xFF )
#define CS_SET_SMALLLEVEL2(x)		( ((x) >> 8 ) & 0xFF )
#define CS_SET_SMALLLEVEL3(x)		((x) & 0xFF )





#include "ViewPortGuild.h"





typedef union tagPATHTABLE
{
	short sPathTable[16];
	char cPathTable[32];
} PATHTABLE, * LPPATHTABLE;



typedef struct tagActionState
{
	unsigned long Rest:1;	// 0
	unsigned long Attack:1;	// 1
	unsigned long Move:1;	// 2
	unsigned long Escape:1;	// 3
	unsigned long Emotion:4;	// 4
	unsigned long EmotionCount:8;	// 8

} ACTION_STATE, * LPACTION_STATE;



typedef struct tagInterfaceState
{
	DWORD use	: 2;
	DWORD state : 4;
	DWORD type	: 10;

} INTERFACE_STATE, * LPINTERFACE_STATE;



typedef struct
{
	DWORD unk0 : 1;
	DWORD unk1 : 1;
	DWORD unk2 : 1;
	DWORD unk3 : 1;
	DWORD unk4 : 1;
	DWORD unk5 : 1;
	DWORD unk6 : 1;
	DWORD unk7 : 1;
	DWORD unk8 : 1;
	DWORD unk9 : 1;
	DWORD unkA : 1;
	DWORD unkB : 1;
	DWORD unkC : 1;
	DWORD unkD : 1;
	DWORD unkE : 1;
	DWORD unkF : 1;
	DWORD unk10: 1;

}  ST_SKILL_STATE;


union STU_SKILL_STATE
{
	ST_SKILL_STATE SkillStates;
	DWORD AllData;
};


extern short RoadPathTable[MAX_ROAD_PATH_TABLE];
extern int  gServerMaxUser;	
//extern BOOL g_EnergyCheckOff;

struct HITDAMAGE_STRUCT
{
	short number;	// 0
	int HitDamage;	// 4
	DWORD LastHitTime;	// 8
};



struct botReward_Data_Struct
{
	WORD num;
	BYTE Level;
	BYTE Opt;
	BYTE Luck;
	BYTE Skill;
	BYTE Dur;
	BYTE Exc;
	BYTE Anc;

	BYTE Sock1;
	BYTE Sock2;
	BYTE Sock3;
	BYTE Sock4;
	BYTE Sock5;

	BYTE Days;
	BYTE JOH;
	BYTE IsForeverFFFE;
	
	int Zen;
	int VipMoney;
};


struct VIEWPORT_STRUCT
{
	char state;	// 0
	short number;	// 2
	unsigned char type;	// 4
	short index;	// 6
	int dis;	// 8
};

struct VIEWPORT_PLAYER_STRUCT
{
	char state;	// 0
	short number;	// 2
	unsigned char type;	// 4
	short index;	// 6
	int dis;	// 8

};


struct MessageStateMachine
{
	int MsgCode;	// 0
	int SendUser;	// 4
	DWORD MsgTime;	// 8
	int SubCode;	// C

};


struct ExMessageStateMachine
{
	int MsgCode;	// 0
	int SendUser;	// 4
	DWORD MsgTime;	// 8
	int SubCode;	// C
	int SubCode2;	// 10
};



typedef struct tagSEND_MONSTER_ATTACK_MENSSAGE
{
	int iMsgCode;	// unk0
	int iIndex;	// unk4
	DWORD dwDelay;	// unk8
	int iSubCode;	// unkC
	int iSubCode2;	// unk10
}SEND_MONSTER_ATTACK_MENSSAGE;

extern ExMessageStateMachine gSMAttackProcMsg[OBJMAX][MAX_MONSTER_SEND_ATTACK_MSG];
extern MessageStateMachine gSMMsg[OBJMAX][MAX_MONSTER_SEND_MSG];

class MARRYINFO
{
public:
	MARRYINFO()
	{
		this->Clear();
	}

	void Clear()
	{
		this->Married = 0;
		this->MarrySeconds = 0;
		this->aFloodMarryCmd = 0;
		memset( this->MarriedName, 0, MAX_ACCOUNT_LEN);
		memset( this->MarryTarget, 0, MAX_ACCOUNT_LEN);
	}

	BOOL Married;
	char MarriedName[11];
	char MarryTarget[11];
	BYTE MarrySeconds;
	WORD aFloodMarryCmd;
};

class S5QUEST
{
public:
	S5QUEST()
	{
		this->Clear();
	}

	void Clear()
	{
		this->Active = 0;
		this->NumID = 0;
		for(int i=0;i<MAX_QUEST_REQ;i++)
				this->State[i] = 0;
	}

	BYTE Active;
	WORD NumID;
	BYTE State[MAX_QUEST_REQ];
};

class CUSTOMQUEST
{
public:
	CUSTOMQUEST()
	{
		this->Clear();
	}

	void Clear()
	{
		this->State = 0;
		this->Status = 0;
	}

	BYTE State;
	BYTE Status;
};

#if(ENABLE_CSHOP == 1)
class MUITEMSHOP
{
public:
	MUITEMSHOP()
	{
		this->Clear();
	}

	void Clear()
	{
		this->m_Serial1 = 0;
		this->m_Serial2 = 0;
		this->ItemID = 0;
		this->ID1 = 0;
		this->ID2 = 0;
	}

	DWORD m_Serial1;
	DWORD m_Serial2;
	WORD ItemID;
	WORD ID1;
	WORD ID2;
};

struct SKILLKEY
{
	BYTE SKillKey[20];	// 4
	BYTE GameOption;	// E
	BYTE QkeyDefine;	// F
	BYTE WkeyDefine;	// 10
	BYTE EkeyDefine;	// 11
	BYTE ChatWindow;	// 12
#if (BORDS == 1)
	BYTE RkeyDefine;	// 11
#endif
};

class MUITEMSHOPGIFT
{
public:
	MUITEMSHOPGIFT()
	{
		this->Clear();
	}

	void Clear()
	{
		memset(this->Name,0,sizeof(this->Name));
		memset(this->Msg,0,sizeof(this->Msg));
	}

	char Name[11];
	char Msg[100];
};
#endif

struct MASTERSKILLS
{
	WORD Type;
	BYTE Level;
};

class MASTERCHARINFO
{
public:
	MASTERCHARINFO()
	{
		this->Clear();
		this->ClearSkills();
		this->ClearLevels();
	}

	void Clear()
	{
		this->AttackSucRate = 0;
		this->AttackSucRatePVP = 0;
		this->DefenseSucRatePVP = 0;
		this->Repair[0] = 0;
		this->Repair[1] = 0;
		this->Repair[2] = 0;
		this->Resistance[0] = 0;
		this->Resistance[1] = 0;
		this->Resistance[2] = 0;
		this->Resistance2[0] = 0;
		this->Resistance2[1] = 0;
		this->Resistance2[2] = 0;
		this->AutoRegen = 0;
		this->IncZen = 0;
		this->IncDefense = 0;
		this->IncMaxHP = 0;
		this->IncMaxAG = 0;
		this->MonsterDieIncRecoverMana = 0;
		this->MonsterDieIncRecoverLife = 0;
		this->MonsterDieIncRecoverSD = 0;
		this->IncExperience = 0;
		this->IncMaxSD = 0;
		this->IncRecoverSDRate = 0;
		this->IncMaxDmg = 0;
		this->IncMinDmg = 0;
		this->DecManaReduction = 0;
		this->IncMaxSkillPower = 0;
		this->IncMinSkillPower = 0;
		this->DecPetConsumeRate = 0;
		this->IncMaxDmgSkillPower = 0;
		this->IncMinDmgSkillPower = 0;
		this->RegenMana = 0;
		this->RegenAG = 0;
		this->IncMaxMana = 0;
		this->IncPhysicalAttackPower = 0;
		this->IncPhysicalAttackPowerTwoHand = 0;
		this->IncPhysicalAttackPowerOneHand = 0;
		this->IncPhysicalAttackPowerMace = 0;		//BOW//Fist Weapon
		this->IncPhysicalAttackPowerSpear = 0;		//CROSSBOW
		this->IncPhysicalAttackPowerTwoHandPvP = 0; //CROSSBOW
		this->IncAttackSpeedOneHand = 0;			//BOW //STICK
		this->MaceStun = 0;
		this->SpearDoubleDmg = 0;
		this->SwellLifeStamina = 0;
		this->SwellLifeMana = 0;
		this->IncCyclone = 0;
		this->IncSlash = 0;
		this->IncFallingSlash = 0;
		this->IncLunge = 0;
		this->IncTwistingSlash = 0;
		this->IncRagefulBlow = 0;
		this->IncDeathStab = 0;
		this->IncStrikeofDestruction = 0;
		this->PullTwistingSlash = 0;
		this->BrokenArmorRagefulBlow = 0;
		this->IncFlame = 0;
		this->IncLighting = 0;
		this->IncInferno = 0;
		this->IncCometFall = 0;
		this->IncEvilSpirits = 0;
		this->IncDarkSide = 0;
		this->IncDragonLore = 0;
		this->IncChainDrive = 0;
		this->IncLargerRingBlower = 0;
		this->IncUpperBeast = 0;
		this->IncLargerRingBlowerDebuf = 0;
		this->IncUpperBeastDebuf = 0;
		this->IncDecay = 0;
		this->IncHellFire = 0;
		this->IncIce = 0;
		this->IncExpansionWizardy1 = 0;
		this->IncExpansionWizardy2 = 0;
		this->IncPoison = 0;
		this->IncWizardyOneHand = 0;
		this->IncWizardyTwoHand = 0;			//BOOKS
		this->DefenseSucRate = 0;
		this->DefSucRateSkillMULT = 0;
		this->DefSucRateSkillSUM = 0;
		this->FitnessSkillSUM = 0;
		this->IncDefenseShield = 0;
		this->IncStaffSpeedOneHand = 0;
		this->IncStaffPvPAttackTwoHand = 0;
		this->IncSoulBarrier = 0;
		this->IncSoulBarrierTime = 0;
		this->IncHealing = 0;
		this->IncTripleShot = 0;
		this->IncMonsterCallHP = 0;
		this->IncMonsterCallDefense = 0;
		this->IncPenetration = 0;
		this->IncGreaterDefense = 0;
		this->IncTripleShotOneArrow = 0;
		this->IncGreaterAttack = 0;
		this->IncIceArrow = 0;
		this->IncGreaterAttackTime = 0;
		this->IncGreaterDefenseTime = 0;
		this->IncInfiniteArrow = 0;
		this->IncPowerSlash = 0;
		this->IncFireSlash = 0;
		this->IncFireBook1 = 0;
		this->IncLightingBook1 = 0;
		this->IncWindBook1 = 0;
		this->IncFireBook2 = 0;
		this->IncLightingBook2 = 0;
		this->IncWindBook2 = 0;
		this->IncLightingShock = 0;
		this->IncChainLighting = 0;
		this->IncDrainLife = 0;
		this->IncSleep = 0;
		this->IncBerserker1 = 0;
		this->IncBerserker2 = 0;
		this->IncFireBurst1 = 0;
		this->IncForceWave = 0;
		this->IncDefenseWDarkHorse = 0;
		this->IncCriticalDamage1 = 0;
		this->IncEarthShake1 = 0;
		this->IncFireBurst2 = 0;
		this->IncCriticalDamage2 = 0;
		this->IncEarthShake2 = 0;
		this->IncCriticalDamage3 = 0;
		this->IncFireScream = 0;
		this->IncDarkSpirit1 = 0;
		this->IncScepterAdds1 = 0;
		this->IncDarkSpirit2 = 0;
		this->IncScepterAdds2 = 0;
		this->IncDarkSpirit3 = 0;
		this->DecPetLifeReduction = 0;
	}

	void ClearLevels()
	{
		this->MasterLevel = 0;
		this->MasterPoints = 0;
		this->MasterPointsTmp = 0;
	}

	void ClearSkills()
	{
		memset(&this->Skill[0],0,sizeof(this->Skill));
	}

	WORD MasterLevel;
	WORD MasterPoints;
	WORD MasterPointsTmp;
	MASTERSKILLS Skill[MAX_SKILLTREE];

	float AttackSucRate;
	float AttackSucRatePVP;
	float DefenseSucRatePVP;
	float Repair[3];
	BYTE Resistance[3];
	float Resistance2[3];
	float AutoRegen;
	float IncZen;
	float IncDefense;
	float IncMaxHP;
	float IncMaxAG;
	float MonsterDieIncRecoverMana;
	float MonsterDieIncRecoverLife;
	float MonsterDieIncRecoverSD;
	float IncExperience;
	float IncMaxSD;
	float IncRecoverSDRate;
	float IncMaxDmg;
	float IncMinDmg;
	float DecManaReduction;
	float IncMaxSkillPower;
	float IncMinSkillPower;
	float DecPetConsumeRate;
	float IncMaxDmgSkillPower;
	float IncMinDmgSkillPower;

	//S6E2
	float DefSucRateSkillMULT;
	float DefSucRateSkillSUM;
	float FitnessSkillSUM;
	float DefenseSucRate;
	float RegenMana;
	float RegenAG;
	float IncMaxMana;
	float IncPhysicalAttackPower;
	float IncPhysicalAttackPowerTwoHand;
	float IncPhysicalAttackPowerOneHand;
	float IncPhysicalAttackPowerMace;
	float IncPhysicalAttackPowerSpear;
	float IncPhysicalAttackPowerTwoHandPvP;
	float IncAttackSpeedOneHand;
	float MaceStun;
	float SpearDoubleDmg;//RF Double DMG with fist weapons
	float SwellLifeStamina;
	float SwellLifeMana;
	float IncCyclone;
	float IncSlash;
	float IncFallingSlash;
	float IncLunge;
	float IncTwistingSlash;
	float IncRagefulBlow;
	float IncDeathStab;
	float IncStrikeofDestruction;
	float PullTwistingSlash;
	float BrokenArmorRagefulBlow;
	float IncFlame;
	float IncLighting;
	float IncInferno;
	float IncCometFall;
	float IncEvilSpirits;
	float IncDarkSide;
	float IncChainDrive;
	float IncDragonLore;
	float IncLargerRingBlower;
	float IncUpperBeast;
	float IncLargerRingBlowerDebuf;
	float IncUpperBeastDebuf;
	float IncDecay;
	float IncHellFire;
	float IncIce;
	float IncExpansionWizardy1;	//IncCurse
	float IncExpansionWizardy2;
	float IncPoison;
	float IncWizardyOneHand;	
	float IncWizardyTwoHand;	//SCEPTER
	float IncDefenseShield;
	float IncStaffSpeedOneHand;
	float IncStaffPvPAttackTwoHand;	//SCEPTER
	float IncSoulBarrier;
	float IncSoulBarrierTime;
	float IncHealing;
	float IncTripleShot;
	float IncMonsterCallHP;
	float IncMonsterCallDefense;
	float IncPenetration;
	float IncGreaterDefense;
	float IncTripleShotOneArrow;
	float IncGreaterAttack;
	float IncIceArrow;
	float IncGreaterAttackTime;
	float IncGreaterDefenseTime;
	float IncInfiniteArrow;
	float IncPowerSlash;
	float IncFireSlash;
	float IncFireBook1;
	float IncLightingBook1;
	float IncWindBook1;
	float IncFireBook2;
	float IncLightingBook2;
	float IncWindBook2;
	float IncLightingShock;
	float IncChainLighting;
	float IncDrainLife;
	float IncSleep;
	float IncBerserker1;
	float IncBerserker2;
	float IncFireBurst1;
	float IncForceWave;
	float IncDefenseWDarkHorse;
	float IncCriticalDamage1;
	float IncEarthShake1;
	float IncFireBurst2;
	float IncCriticalDamage2;
	float IncEarthShake2;
	float IncCriticalDamage3;
	float IncFireScream;
	float IncDarkSpirit1;
	float IncScepterAdds1;
	float IncDarkSpirit2;
	float IncScepterAdds2;
	float IncDarkSpirit3;
	float DecPetLifeReduction;
};

struct OBJECTSTRUCT
{
	short m_Index;		// 0
	short Connected;	// 4

	char LoginMsgSnd;	// 8
	char LoginMsgCount;	// 9
	char CloseCount;	// A
	char CloseType;	// B
	bool bEnableDelCharacter;	// C

	_PER_SOCKET_CONTEXT* PerSocketContext;	// 10
	unsigned int m_socket;	// 14
	char Ip_addr[16];	// 18
	int UserNumber;	// 28
	short DBNumber;	// 2C
	unsigned char Magumsa;	// 20
	unsigned char AllowSummoner;
	unsigned char AllowRageFighter;
	DWORD AutoSaveTime;	// 34
	DWORD ConnectCheckTime;	// 38
	DWORD CheckTick;	// 3C
	unsigned char CheckSpeedHack;	// 40
	DWORD CheckTick2;	// 44
	unsigned char CheckTickCount;	// 48
	//DWORD SaveTimeForStatics;	// 4C
	
	int iPingTime;	// 50
	BYTE bPacketsReceivedCount;	// 50

	unsigned char m_TimeCount;	// 54

	short CheckSumTableNum;	// 5C
	DWORD CheckSumTime;	// 60
	WORD Type;	// 64
	unsigned char Live;	// 66
	//char  CharacterPos;	// 67
	char  AccountID[11];	// 68
	char  Name[11];	// 73
	char  LastJoominNumber[14];	// 7E
	bool  PlusStatQuestClear; // 8C
	bool  ComboSkillquestClear;	// 8D
	ComboSkillData comboSkill; // 90
	WORD  Class; // 9C
	BYTE  DbClass;	// 9E
	BYTE  ChangeUP; // 9F
	short Level; // A0

#if (DSGN_COMPILE == 1)
	unsigned short   LevelUpPoint;	// A4
#else
	int LevelUpPoint;	// A4
#endif
	unsigned short   iFruitPoint;	// A8
	
	unsigned long Experience;		// AC
	unsigned long NextExp;			// B0
	int   Money;					// B4

/*
	short   Strength;				// B8
	short   Dexterity;				// BA
	short   Vitality;				// BC
	short	Energy;					// BE
*/

	unsigned short   Strength;		// B8
	unsigned short   Dexterity;		// BA
	unsigned short   Vitality;		// BC
	unsigned short	 Energy;		// BE
	unsigned short	 Leadership;	// DC
	
	short AddStrength;	// E4
	short AddDexterity;	// E6
	short AddVitality;	// E8
	short AddEnergy;	// EA
	short AddLeadership;	// DE

	float Life; // C0
	float MaxLife; // C4
	int   m_iScriptMaxLife;	// C8
	float FillLife;	// CC
	float FillLifeMax;	// D0
	float Mana;	// D4
	float MaxMana;	// D8
	WORD  ChatLimitTime;	// E0
	BYTE  ChatLimitTimeSec;	// E2
	BYTE  FillLifeCount;	// E3

	int   BP;	// EC
	int   MaxBP;	// F0
	int	  AddBP;	// F4
	float VitalityToLife;	// F8
	float EnergyToMana;	// FC
	int   m_PK_Count;	// 100	[char]
	//int   m_HERO_Count;	// 100	[char] add later for hero count
	char  m_PK_Level;	// 101
	int   m_PK_Time;	// 104

	BYTE  Dir; // 10C
	BYTE  MapNumber; // 10D
	int   AddLife;	// 110
	int   AddMana;	// 114

	int   iShield;	// 118
	int   iMaxShield;	// 11C
	int   iAddShield;	// 120
	int   iFillShieldMax;	// 124
	int   iFillShield;	// 128
	BYTE  iFillShieldCount;	// 12C
	DWORD dwShieldAutoRefillTimer;	// 130

	BYTE  DamageMinus;	// 134
	BYTE  DamageReflect;	// 135
	short MonsterDieGetMoney;	// 136
	BYTE  MonsterDieGetLife;	// 138
	BYTE  MonsterDieGetMana;	// 139

	short X; // 108
	short Y; // 10A
	BYTE  StartX;	// 13A
	BYTE  StartY;	// 13B
	short m_OldX;	// 13C
	short m_OldY;	// 13E
	short TX;		// 140
	short TY;		// 142
	short MTX;	// 144
	short MTY;	// 146

	short PathCount;	// 148
	BYTE  PathCur;	// 14C
	char  PathStartEnd;	// 150
	//short PathOri[15];	// 152
	short PathX[15];	// 170
	short PathY[15];	// 18E
	char  PathDir[15];	// 1AC
	DWORD PathTime; // 1BC
	//WORD  m_MoveGateNumber;	// 1C0

	//DWORD Authority;		// 1C4
	//DWORD AuthorityCode;	// 1C8
	//DWORD Penalty;		// 1CC
	BYTE Authority;			// 1C4
	BYTE AuthorityCode;		// 1C8
	BYTE Penalty;			// 1CC

	BYTE  m_cAccountItemBlock; // 1D0
	ACTION_STATE m_ActState;	// 1D4
	BYTE  m_ActionNumber;	// 1D8
	DWORD m_State;	// 1DC
	//char m_StateSub;	// 1E0
	unsigned char m_Rest;	// 1E1
	//char m_ViewState;	// 1E2
	int m_ViewSkillState;	// 1E4
	int m_LastViewSkillState;	// 1E4
	DWORD m_LastMoveTime;	// 1E8
	DWORD m_LastAttackTime;	// 1EC
	BYTE m_FriendServerOnline;	// 1F0
	int m_DetectSpeedHackTime;	// 1F4
	unsigned long m_ZeroTimeAttackCount;	// 1F8
	unsigned long m_DetectCount;	// 1FC
	short m_DetectedHackKickCount;	// 200
	//int m_SpeedHackPenalty;	// 204
	unsigned char m_AttackSpeedHackDetectedCount;	// 208
	unsigned long m_PacketCheckTime;	// 20C
	unsigned char m_ShopTime;	// 210
	//unsigned long m_TotalAttackTime;	// 214
	//int m_TotalAttackCount;	// 218
	unsigned long TeleportTime;	// 21C
	char Teleport;	// 220
	char KillerType;	// 221
	char DieRegen;	// 222
	char RegenOk;	// 223
	BYTE  RegenMapNumber;	// 224
	BYTE  RegenMapX;	// 225
	BYTE  RegenMapY;	// 226
	DWORD RegenTime;	// 228
	DWORD MaxRegenTime;	// 22C
	short m_PosNum;	// 230
	//DWORD LifeRefillTimer;	// 234
	DWORD CurActionTime;	// 238
	DWORD NextActionTime;	// 23C
	DWORD DelayActionTime;	// 240
	char DelayLevel;	// 244
	char m_PoisonType;	// 245
	char m_IceType;	// 246
	char m_PoisonBeattackCount; // 247
	char m_ColdBeattackCount;	// 248
	char m_ImmuneToMagicCount;	// 249
	char m_ImmuneToHarmCount;	// 24A
	char m_iMonsterBattleDelay;	// 24B
	char m_cKalimaGateExist; // 24C
	int m_iKalimaGateIndex; // 250
	char m_cKalimaGateEnterCount;	// 254
	OBJECTSTRUCT * lpAttackObj;	// 258
	short m_SkillNumber;	// 25C
	DWORD m_SkillTime;	// 260
	bool m_bAttackerKilled;	// 264

	BYTE m_ManaFillCount;	// 265
	BYTE m_LifeFillCount;	// 266
	BYTE m_iShieldFillCount;

	short SelfDefense[MAX_SELF_DEFENSE];	// 268
	DWORD SelfDefenseTime[MAX_SELF_DEFENSE];	// 27C
	DWORD MySelfDefenseTime;	// 290
	//char m_Drink;	// 294
	int m_SkillDefense;	// 298
	int m_SkillDefenseTime;	// 29C

	int m_SkillAttack;	// 2A0
	int m_SkillAttackTime;	// 2A4

	BYTE m_SkillAttack2;	// 2A8
	char m_SkillAttackTime2;	// 2AC
	
	short m_SkillHarden;	// 2B8
	int m_SkillHardenTime;	// 2BC
	
	short m_SkillMagumReduceDefense;	// 2C0
	int m_SkillMagumReduceDefenseTime;	// 2C4

	short PartyNumber; // 2C8
	short PartyTargetUser;	// 2CC
	
	int GuildNumber;	// 2D0
	_GUILD_INFO_STRUCT * lpGuild;	// 2D4
	char GuildName[11]; // 2D8
	short GuildStatus;	// 2E4
	int iGuildUnionTimeStamp;	// 2E8

	short m_RecallMon;	// 2EC
	
	short m_Change;	// 2F0
	short TargetNumber;	// 2F4
	short TargetShopNumber;	// 2F6
	short ShopNumber;	// 2F8
	short LastAttackerID;	// 2FA
	int m_AttackDamageMin;	// 2FC
	int m_AttackDamageMax;	// 300
	int m_MagicDamageMin;	// 304
	int m_MagicDamageMax;	// 308
	int m_AttackDamageLeft;	// 30C
	int m_AttackDamageRight;	// 310
	int m_AttackDamageMaxLeft;	// 314
	int m_AttackDamageMinLeft;	// 318
	int m_AttackDamageMaxRight;	// 31C
	int m_AttackDamageMinRight;	// 320
	int m_AttackRating;	// 324
	int m_AttackSpeed;	// 328
	int m_MagicSpeed;	// 32C
	int m_Defense;	// 330
	int m_MagicDefense;	// 334
	int m_SuccessfulBlocking;	// 338
	short m_MoveSpeed;	// 33C
	BYTE m_MoveRange;	// 33E
	short m_AttackRange;	// 340
	short m_AttackType;	// 342
	BYTE m_ViewRange;	// 344
	short m_Attribute;	// 346
	short m_ItemRate;	// 348
	short m_MoneyRate;	// 34A
	int m_CriticalDamage;	// 34C
	int m_ExcelentDamage;	// 350

	CMagicInf * m_lpMagicBack;	// 354	USER ONLY
	unsigned long UseMagicTime;	// 360	USER ONLY
	char UseMagicCount;	// 364	USER ONLY

	CMagicInf * Magic;	// 358
	char MagicCount;	// 35C
	unsigned char UseMagicNumber;	// 35D

	short OSAttackSerial;	// 366	USER ONLY
	//unsigned char SASCount;	// 368
	DWORD SkillAttackTime;	// 36C
	char CharSet[18]; 	// 370
	char m_Resistance[MAX_RESISTENCE_TYPE];	// 382
	char m_AddResistance[MAX_RESISTENCE_TYPE];	// 389

	int FrustrumX[MAX_ARRAY_FRUSTRUM];	// 390
	int FrustrumY[MAX_ARRAY_FRUSTRUM]; // 3A4

	VIEWPORT_STRUCT VpPlayer[MAX_VIEWPORT];	// 3B0
	VIEWPORT_PLAYER_STRUCT VpPlayer2[MAX_VIEWPORT];	// 734
	int VPCount; // AB8
	int VPCount2;	// ABC
	HITDAMAGE_STRUCT sHD[MAX_ST_HIT_DAMAGE];	// AC0
	//short sHDCount;	// CA0
	tagInterfaceState m_IfState;	// CA4
	DWORD m_InterfaceTime;	// CA8

	CItem * pInventory; // CAC
	LPBYTE  pInventoryMap; // CB0
	//char * pInventoryCount;	// CB4
	char pTransaction;	// CB8
	CItem * Inventory1;	//CBC
	LPBYTE  InventoryMap1;	// CC0
	//char InventoryCount1;	// CC4
	CItem * Inventory2;	// CC8
	LPBYTE InventoryMap2;	// CCC
	//char InventoryCount2;	// CD0
	
	CItem * Trade; // CD4
	LPBYTE TradeMap;	// unkCD8
	int TradeMoney;				// CDC	BOTPET + USER
	bool TradeOk;	// CE0
	
	CItem * pWarehouse; // CE4
	LPBYTE pWarehouseMap; // CE8
	//char WarehouseCount;	// CEC
	short WarehousePW;			// CEE	USER ONLY
	short WarehouseExNum;		// USER ONLY
	short WarehouseExNumLast;	// USER ONLY
	short WarehouseVipCount;	// USER ONLY
	int WarehouseTimeCount;		// USER ONLY
	BYTE WarehouseLock;			// USER ONLY
	//BYTE WarehouseUnfailLock;	// CF1
	int WarehouseMoney;			// USER ONLY
	bool WarehouseSave;			// USER ONLY

	CItem * pChaosBox;			// CFC	USER ONLY
	LPBYTE pChaosBoxMap;		// D00	USER ONLY
	int ChaosMoney;				// D04	USER ONLY
	BYTE ChaosSuccessRate;		// D08	USER ONLY
	BOOL ChaosLock;				// D0C	USER ONLY
	
	//DWORD m_Option;			// D10
	BYTE m_Option;				// D10

	int m_nEventScore;			// D14
	int m_nEventExp;			// D18
	int m_nEventMoney;			// D1C

	BYTE m_bDevilSquareIndex;	// D20	ALL
	bool m_bDevilSquareAuth;	// D21	USER ONLY

	char m_cBloodCastleIndex;	// D22	ALL
	char m_cBloodCastleSubIndex;// D23	USER ONLY
	int m_iBloodCastleEXP;		// D24	USER ONLY
	bool m_bBloodCastleComplete;// D28	USER ONLY

	//BYTE m_cIllusionTempleIndex;
	//BYTE m_cIllusionTempleTeam;
	BYTE m_bEventTicketType;

	char m_cChaosCastleIndex;	// D29	ALL
	int m_iChaosCastleBlowTime;	// D2C	ALL
	char m_cChaosCastleSubIndex;// D2A	USER ONLY
	char m_cKillUserCount;		// D30	USER ONLY
	char m_cKillMonsterCount;	// D31	USER ONLY

	short m_iDuelUserReserved;	//	USER ONLY
	short m_iDuelUserRequested;	//	USER ONLY
	short m_iDuelUser;			//	USER ONLY
	BYTE m_btDuelScore;			//	USER ONLY
	int m_iDuelTickCount;		//	USER ONLY
	char m_iDuelRoom;			//	USER ONLY
	BYTE m_isDuelSpectator;		//	USER ONLY

	bool m_bPShopOpen;	// D48
	bool m_bPShopTransaction;	// D49	
	bool m_bPShopItemChange;	// D4A
	bool m_bPShopRedrawAbs;	// D4B
	char m_szPShopText[36];		// D4C
	bool m_bPShopWantDeal;	// D70
	short m_iPShopDealerIndex;	// D74
	char m_szPShopDealerName[10];	// D78
	CRITICAL_SECTION m_critPShopTrade;	// D84
	short m_iVpPShopPlayer[MAX_VIEWPORT];	// D9C
	WORD m_wVpPShopPlayerCount;	// EC8

	bool IsInBattleGround;	// ECA
	bool HaveWeaponInHand;	// ECB
	short EventChipCount;	// ECC
	int MutoNumber;	// ED0
	bool UseEventServer;	// ED4
	bool LoadWareHouseInfo;	// ED8
	short iStoneCount;	// EDC

	BYTE m_Quest[50];	// EE0
	bool m_SendQuestInfo;	// F12

	short m_SkyBossMonSheildLinkIndex;	// F14
	int m_SkyBossMonSheild;	// F18
	int m_SkyBossMonSheildTime;	// F1C
	
	int m_MaxLifePower;	// F20
	int m_WizardSkillDefense;	// F24
	int m_WizardSkillDefenseTime;	// F28

	//int m_PacketChecksumTime;	// F2C
	int m_CheckLifeTime;	// F30
	unsigned char m_MoveOtherServer;	// F34
	char BackName[11];	// F35
	char m_BossGoldDerconMapNumber;	// F40
	//bool m_InWebzen;	// F41
	//char m_LastTeleportTime;	// F42
	//BYTE m_ClientHackLogCount;	// F43
	
	bool m_bIsInMonsterHerd;	// F44
	BYTE m_bIsMonsterAttackFirst;	// F48
	class MonsterHerd * m_lpMonsterHerd;	// F4C

	CCSAuth2 NPggCSAuth;	// F50
	bool m_bSentGGAuth;	// F7C
	DWORD m_NPggCheckSumSendTime;	// F80
	
	int fSkillFrustrumX[MAX_ARRAY_FRUSTRUM];	// F84
	int fSkillFrustrumY[MAX_ARRAY_FRUSTRUM];	// F94

	BYTE SkillHellFire2State;	// FA4
	BYTE SkillHellFire2Count;	// FA5
	DWORD SkillHellFire2Time;	// FA8
	unsigned char m_ReqWarehouseOpen;	// FAC
	//int m_NotAttackAreaCount;	// FB0
	short SetOpAddSkillAttack;	// FB4
	short SetOpAddExDamage;	// FB6
	short SetOpAddExDamageSuccessRate;	// FB8
	short SetOpAddCriticalDamage;	// FBA
	short SetOpAddCriticalDamageSuccessRate;	// FBC
	short SetOpIncAGValue;	// FBE
	short SetOpAddDamage;	// FC0
	short SetOpAddMinAttackDamage;	// FC2
	short SetOpAddMaxAttackDamage;	// FC4
	short SetOpAddAttackDamage;	// FC6
	short SetOpAddDefence;	// FC8
	short SetOpAddDefenceRate;	// FCA
	short SetOpAddMagicPower;	// FCC
	BYTE SetOpIgnoreDefense;	// FCE
	BYTE SetOpDoubleDamage;	// FCF
	BYTE SetOpTwoHandSwordImproveDamage;	// FD0
	BYTE SetOpImproveSuccessAttackRate;	// FD1
	BYTE SetOpReflectionDamage;	// FD2
	BYTE SetOpImproveSheldDefence;	// FD3
	BYTE SetOpDecreaseAG;	// FD4
	BYTE SetOpImproveItemDropRate;	// FD5
	bool IsFullSetItem;	// FD6
	TDurMagicKeyChecker DurMagicKeyChecker;	// FD8
	WORD SkillRecallParty_Time;	// 1104
	BYTE SkillRecallParty_MapNumber;	// 1106
	BYTE SkillRecallParty_X;	// 1107
	BYTE SkillRecallParty_Y;	// 1108
	WORD SkillAddCriticalDamageTime;	// 110A
	WORD SkillAddCriticalDamage;	// 110C
	bool bIsChaosMixCompleted;	// 110E
	bool SkillLongSpearChange;	// 110F
	CSkillDelay SkillDelay;	// 1110
	int iObjectSecTimer;	// 1514

	bool m_bMapAntiHackMove;
	BYTE b_BlockMoveCommandSeconds;	//Anti-Dupe solution for server move.
	bool m_bMapSvrMoveQuit;	// 1518
	bool m_bMapSvrMoveReq;	// 1519
	DWORD m_dwMapSvrQuitTick;	// 151C
	short m_sPrevMapSvrCode;	// 1520
	short m_sDestMapNumber;	// 1522
	BYTE m_btDestX;	// 1524
	BYTE m_btDestY;	// 1525

#if (GS_CASTLE==1)
	//BYTE m_btCsNpcExistVal1; // 1528 [GS_CS]
	//BYTE m_btCsNpcExistVal2; // 1529 [GS_CS]
    //BYTE m_btCsNpcExistVal3; // 152A [GS_CS]
    //BYTE m_btCsNpcExistVal4; // 152B [GS_CS]
	//int m_iCsNpcExistVal; // 1528 [GS_CS]
    BYTE m_btCsNpcType; // 152C [GS_CS]
    BYTE m_btCsGateOpen; // 152D [GS_CS]
    short m_iCsGateLeverLinkIndex; // 1530 [GS_CS]
    BYTE m_btCsNpcDfLevel; // 1534 [GS_CS]
    BYTE m_btCsNpcRgLevel; // 1535 [GS_CS]
    BYTE m_btCsJoinSide; // 1536 [GS_CS]
    //bool m_bCsGuildInvolved; // 1537 [GS_CS]
	//bool m_bIsCastleNPCUpgradeCompleted;	// 1526
#endif

	BYTE m_btWeaponState;	// 1527
	int m_iWeaponUser;	// 1528
	BYTE m_btKillCount;	// 152C
	int m_iSkillStunTime;	// 1530
	int m_iSkillBrandOfSkillTime;	// 1534
	int m_iSkillInvisibleTime;	// 1538
	int m_iSkillManaSwellTime;	// 153C
	int m_iSkillManaSwell;	// 1540
	int m_iAccumulatedDamage;	// 1544

	int m_iPotionBlessTime;	// 1548
	int m_iPotionSoulTime;	// 154C
	BYTE m_btLifeStoneCount; // 1564 [GS_CS]
    BYTE m_btCreationState; // 1565 [GS_CS]

	int m_iCreatedActivationTime; // 1568 [GS_CS]
	int m_iAccumulatedCrownAccessTime;	// 1550

	TMonsterSkillElementInfo m_MonsterSkillElementInfo;	// 1554
	int m_iBasicAI;	// 15A4
	int m_iCurrentAI;	// 15A8
	int m_iCurrentAIState;	// 15AC
	int m_iLastAIRunTime;	// 15B0
	int m_iGroupNumber;	// 15B4
	//int m_iSubGroupNumber;	// 15B8
	int m_iGroupMemberGuid;	// 15BC
	int m_iRegenType;	// 15C0
	TMonsterAIAgro m_Agro;	// 15C4
	int m_iLastAutomataRuntime;	// 18E8
	int m_iLastAutomataDelay;	// 18EC
	int m_iCrywolfMVPScore;	// 18F0
	DWORD m_dwLastCheckTick;	// 18F4
	
	int m_iAutoRecuperationTime;	// 18F8
	
	int m_iSkillNPCHelpTime;	// 18FC
	short m_iSkillNPCDefense;	// 1900
	short m_iSkillNPCAttack;	// 1904

	int m_iMuseElfInfinityArrowSkillTime;	// 1908
	short m_iSkillDistanceErrorCount;	// 190C

	DWORD m_dwSkillDistanceErrorTick;	// 1910
	JEWELOFHARMONY_ITEM_EFFECT m_JewelOfHarmonyEffect;	// 1914
	ITEMOPTION_FOR380ITEM_EFFECT m_ItemOptionExFor380;	// 193A
	BYTE m_bKanturuEntranceByNPC;	// 194C
	WORD m_wItemEffectType[2];	// 1950
	int m_iItemEffectValidTime[2];	// 1954
	WORD m_wCashPoint;	// 1958
	short m_iPeriodItemEffectIndex;	// 195C

	BYTE m_btDisableExpGain;
	WORD m_wExprienceRate;	// 1960
	WORD m_wItemDropRate;	// 1962

	//BYTE m_btMoveMapBound;	// 1964

	int m_SkillReflectTime;
	BYTE m_SkillReflect;

	int m_SkillMagicCircleTime;
	BYTE m_SkillMagicCircle;

	int m_SkillReduceDamageTime;
	BYTE m_SkillReduceDamage;

	int m_SkillReduceDefenseTime;
	BYTE m_SkillReduceDefense;

	int m_RFBufReduceDefenseTime;
	BYTE m_RFBufReduceDefense;

	BYTE m_SkillSleep;
	int m_SkillSleep_Time;
	short m_SkillSleep_MoveRange;
	short m_SkillSleep_AttackRange;
	short m_SkillSleep_ViewRange;

	int m_SkillNightTime;
	BYTE m_SkillNight;

	int m_SkillGladiatorsGloryTime;
	//BYTE m_SkillGladiatorsGlory;
	//int m_SkillGladiatorsGlory_Exp;
	//int m_SkillGladiatorsGlory_Drop;

	int m_SkillSummonerBookTime;
	int m_SkillSummonerBookTypeDmg;
	int m_SkillSummonerBookDmg;
	short m_SkillSummonerBookSender;
	
	int m_SkillExplotionTime;

	int m_SkillLightingStormTime;
	BYTE m_SkillLightingStorm;
	short m_SkillLightingStormSender;

	int m_SkillRedStormTime;
	int m_SkillRedStormDmg;
	short m_SkillRedStormSender;

	int m_SkillSwordSlashTime;
	int m_SkillSwordSlashDmg;
	short m_SkillSwordSlashSender;

	BYTE m_SkillIT_Number;
	BYTE m_SkillIT_Time;

	int m_iSkillNPCSantaTime;
	int m_iSkillNPCSantaType;

	int m_SkillBerserkerTime;
	float m_SkillBerserkerDef;
	int m_SkillBerserkerAttack;

	//==============================================
	BYTE PlayerExtraInfoReceived;	//Resets, Mastering, Custom Quest, Marry, CShop
	BYTE PlayerQ5InfoReceived;		//Season 5 Quest System
	BYTE PlayerCQInfoReceived;		//Custom Quest System
	BYTE AccountExtraInfoReceived;
	BYTE PlayerSkillTreeInfoReceived;

	BYTE PlayerExtraInfoModified;
	BYTE PlayerOfficialHelperInfoModified;
	BYTE PlayerSkillTreeInfoModified;
	BYTE PlayerQ5InfoModified;
	BYTE PlayerCustomQuestInfoModified;
	BYTE AccountExtraInfoModified;

#if (PACK_EDITION>=2)
	CUSTOMQUEST * CustomQuest;
	MARRYINFO * MarryCharacterInfo;
#endif

#if (PACK_EDITION>=3)
	ANTIAFKCHARINFO * AFKCharClass;
#endif

	S5QUEST * S5Quest;
	MASTERCHARINFO * MasterCharacterInfo;

	int PCPoints;
	int Resets;

	BYTE Vip;
	int VipMoney;
	WORD VipDays;

#if (CRYSTAL_EDITION == 1)
#if (PACK_EDITION>=3)
	BYTE VipShopOpen;
#endif
	BYTE MuTabOpen;
#endif

#if (PACK_EDITION>=3)
	BYTE IsBot;
	int BotNumOwner;
	int BotPower;
	int BotDefense;
	int BotLife;
	int BotMaxLife;

	int BotLvlUpDefense;
	int BotLvlUpPower;
	int BotLvlUpLife;
	int BotLvlUpMana;
	int BotLvlUpExp;
	short BotLvlUpMaxLevel;
	BYTE BotVersion;

	BYTE BotFollowMe;
	BYTE BotSkillAttack;

	BYTE HaveBot;
	WORD aFloodBotPetCmd;
#endif

	WORD aFloodPostCmd;

	BYTE GensFam;
	BYTE GensRank;
	BYTE GensLevel;
	BYTE GensNotAllow;
	BYTE RecvRewItem;
	int GensCont;

	short LuckyCoins;

	int m_SkillDragonSlayerTime;
	float m_SkillDragonSlayerDmg;

	int m_SkillRedArmorIgnoreTime;
	BYTE m_SkillRedArmorIgnoreNum;

	int m_SkillFitnessTime;
	int m_SkillFitnessVal;

	int m_SkillDefSuccessRateIncTime;
	int m_SkillDefSuccessRateIncNum;


	int m_SkillAddLife_Mana;
	int m_SkillAddLife;	// 2B0
	int m_SkillAddLifeTime;	// 2B4
	
	int m_SkillMagicCircleCriticalDamage;
	int m_PoisonAdd;
	BYTE ExWarehouse;
	BYTE ExWarehouseTmp;
	BYTE ExInventory;
	BYTE ExInventoryTmp;
	unsigned short m_TimeCount2;	// 54

	BYTE OfficialHelper[257];
	bool OfficialHelper_Active;
	unsigned short OfficialHelper_TimeCount;	// 54	

#if(ENABLE_CSHOP == 1)
	MUITEMSHOP * CShop;
	MUITEMSHOP * CShopGift;
	MUITEMSHOPGIFT * CShopGiftMsg;

	int WCoin;
	int WCoinP;
	int GoblinCoin;

	BYTE CShopInfoRequested;
	BYTE CShopInfoModified_Normal;
	BYTE CShopInfoModified_Gift;
	BYTE CShopInfoModified_Points;
#endif

	WORD m_wItem_MuShop_Seal_EffectType;
	int m_iItem_MuShop_Seal_EffectValidTime;

	WORD m_wItem_MuShop_Scroll_EffectType;
	int m_iItem_MuShop_Scroll_EffectValidTime;

	BYTE DieMap;
	BYTE DieX;
	BYTE DieY;

	int MovePerSecs;

	SKILLKEY sSkillKey;

#if (CRYSTAL_EDITION == 1)
	BYTE CrystalPing;
	BYTE CrystalPingReply;
#endif

#if (PACK_EDITION>=3)
	BYTE RaceCheck;
	int RaceTime;
#endif

	BYTE m_Invisible;
	BYTE LotteryCnt;
};


typedef OBJECTSTRUCT * LPOBJ;


// sizeof ( BILL_CLASS ) == 0x18
class BILL_CLASS
{

private:

	char cCertifyType;	// 0
	BYTE PayCode;	// 1
	int EndTime;	// 4
	char EndsDays[13];	// 8

public:

	BILL_CLASS()	// line : 253
	{
		this->Init();
	}


	void Init()	// line : 219
	{
		this->cCertifyType=-1;
		this->PayCode=0;
		this->EndTime=0;
		this->EndsDays[0]=0;
	}	// line : 224


	BOOL SetBill(BYTE certify_type, BYTE pay, DWORD end_time, char* ends_days)	// line : 228
	{
		BOOL bRet=0;

		if ( this->cCertifyType == -1 )
		{
			bRet = 1;
		}

		this->cCertifyType = certify_type;
		this->PayCode = pay;
		this->EndTime = end_time;
		strcpy(this->EndsDays, ends_days);
		return bRet;
	}	// line : 237

	//int __thiscall IsEndTime();
	BYTE GetPayCode() {return this->PayCode;}	// line : 252
	char* GetEndsDays() {return &this->EndsDays[0];}	// line : 253
	int GetEndTime() { return this->EndTime;}	// line : 254
	int GetCertify() {return this->cCertifyType;}	// line : 255
};



#include "protocol.h"



//extern CViewportGuild ViewGuildMng;
extern OBJECTSTRUCT gObj[OBJMAX];
extern BILL_CLASS m_ObjBill[OBJMAX];


//extern int gObjCSFlag;
//extern int gItemLoop;
//extern int gItemLoop2;
//extern int gItemLoopMax;
extern int gObjTotalUser;
extern int gDisconnect;
//extern int gCurConnectUser; 
//extern int skillSuccess;
extern int GuildInfoOfs;
extern int GuildInfoCount;
extern int GuilUserOfs;
extern int GuildUserCount;
extern int lOfsChange;
extern int ChangeCount; // 8bf8b44

extern int gObjCallMonCount;
extern int gObjMonCount;
extern int gObjCount;




void gObjSkillUseProcTime500(LPOBJ lpObj);
void MonsterAndMsgProc();
void MoveMonsterProc();
void gObjRefillProcess(int aIndex, LPOBJ lpObj);
void gObjInit();
void gObjEnd();
void gObjClearViewportOfMine(LPOBJ lpObj);
void gObjClearViewport(LPOBJ lpObj);
void gObjCloseSet(int aIndex, int Flag);
void gObjCharTradeClear(LPOBJ lpObj);
void gObjCharZeroSet(int aIndex);
int gObjGetSocket(SOCKET socket);
void gObjSetTradeOption(int aIndex, int option);
void gObjSetDuelOption(int aIndex, int option);
int GetMapMoveLevel(LPOBJ lpObj, int mapnumber, int max_over);
void DbItemSetInByte(LPOBJ lpObj, struct SDHP_DBCHAR_INFORESULT* lpMsg, int ItemDbByte, bool* bAllItemExist);
void gObjSetBP(int aIndex);
int gObjSetCharacter(unsigned char* lpdata, int aIndex);
int gObjCanItemTouch(LPOBJ lpObj, int type);
void gObjMagicTextSave(LPOBJ lpObj);
void ItemIsBufExOption(unsigned char* buf, class CItem* lpItem);
void gObjStatTextSave(LPOBJ lpObj);
void gObjItemTextSave(LPOBJ lpObj);
void gObjWarehouseTextSave(LPOBJ lpObj);
void gObjAuthorityCodeSet(LPOBJ lpObj);
int gObjSetPosMonster(int aIndex, int PosTableNum);
int gObjMonsterAdd(WORD Type, BYTE Map, BYTE X, BYTE Y);
int gObjSetMonster(int aIndex, int MonsterClass, LPSTR FunctionName);
void gObjDestroy(SOCKET aSocket, int client);
#ifdef _WIN64
short gObjAddSearch(SOCKET aSocket, char* ip);
#else
short gObjAddSearch(unsigned int aSocket, char* ip);
#endif
short gObjAdd(SOCKET aSocket, char* ip, int aIndex);
short gObjMonsterRecall(int iMapNumber);
short gObjAddMonster(int iMapNumber);
short gObjAddCallMon();
void gObjUserKill(int aIndex);
void gObjAllLogOut();
void gObjAllDisconnect();
void gObjTradeSave(LPOBJ lpObj, int index);
void gObjBillRequest(LPOBJ lpObj);
short gObjMemFree(int index);
int gObjGameClose(int aIndex);
short gObjDel(int index);
short gObjSometimeClose(int index);
int gObjIsGamePlaing(LPOBJ lpObj);
int gObjIsConnectedGP(int aIndex, char* CharName);
int gObjIsConnectedGP(int aIndex);
int gObjIsConnectedAccount(int aIndex);
int gObjIsConnectedGPBOT(int aIndex);
int gObjIsConnected(LPOBJ lpObj, int dbnumber);
int gObjIsConnected(int aIndex);
int gObjIsConnected(LPOBJ lpObj);
int gObjIsAccontConnect(int aIndex, char* accountid);
int gObjIsAccontConnectProxy(int aIndex);
int gObjJoominCheck(int aIndex, char* szInJN);
//int gObjTaiwanJoominCheck(int aIndex, char* szInJN);
int gObjCheckXYMapTile(LPOBJ lpObj, int iDbgName);
int gObjSetAccountLogin(int aIndex, char* szId, int aUserNumber, int aDBNumber, char* JoominNumber);
int gObjGetUserNumber(int aIndex);
int gObjGetNumber(int aIndex, int& UN, int& DBN);
LPSTR gObjGetAccountId(int aIndex);
int gObjGetIndex(char* szId);
int gObjGetIndexbyAccount(char* szId);
int gObjUserIdConnectCheck(char* szId, int index);
unsigned char GetPathPacketDirPos(int px, int py);
int gObjCalDistance(LPOBJ lpObj1, LPOBJ lpObj2);
int gObjPositionCheck(LPOBJ lpObj);
int gObjCheckTileArea(int aIndex, int x, int y, int dis);
int ExtentCheck(int x, int y, int w, int h);
void gObjSetInventory1Pointer(LPOBJ lpObj);
void gObjSetInventory2Pointer(LPOBJ lpObj);
void gObjAddMsgSend(LPOBJ lpObj, int aMsgCode, int aIndex, int SubCode);
void gObjAddMsgSendDelay(LPOBJ lpObj, int aMsgCode, int aIndex, int delay, int SubCode);
void gObjAddMsgSendDelayInSpecificQPos(LPOBJ lpObj, int aMsgCode, int aIndex, int delay, int SubCode, int iQPosition);
void gObjAddAttackProcMsgSendDelay(LPOBJ lpObj, int aMsgCode, int aIndex, int delay, int SubCode, int SubCode2);
void gObjMsgProc(LPOBJ lpObj);
void gObjStateProc(LPOBJ lpObj, int aMsgCode, int aIndex, int SubCode);
void gObjStateAttackProc(LPOBJ lpObj, int aMsgCode, int aIndex, int SubCode, int SubCode2);
int gObjBackSpring(LPOBJ lpObj, LPOBJ lpTargetObj);
int BackSpringCheck(int& x, int& y, int& dir, unsigned char map);
int gObjBackSpring2(LPOBJ lpObj, LPOBJ lpTargetObj, int count);
BOOL gObjIsMaxLevel(LPOBJ lpObj);
int gObjLevelUp(LPOBJ lpObj, int addexp, int iMonsterType, int iEventType, int & LevelUp);
int gObjLevelUpPointAdd(unsigned char type, LPOBJ lpObj);
void gObjMonsterItemLoadScript(LPOBJ lpObj, char* script_file);
void gObjGiveItemSearch(LPOBJ lpObj, int maxlevel);
unsigned char gObjWarehouseInsertItem(LPOBJ lpObj, int type, int index, int level, int pos);
void gObjGiveItemWarehouseSearch(LPOBJ lpObj, int maxlevel);
BYTE gObjGuildMasterCapacityTest(LPOBJ lpObj);
void gObjNextExpCal(LPOBJ lpObj);
int gObjExpCal(LPOBJ lpObj, int LevelFix);
BYTE retResistance(LPOBJ lpObj, int Resistance_Type);
int retCalcSkillResistance(LPOBJ lpTargetObj, int isDouble);
BYTE gObjAttackType(LPOBJ lpTargetObj,LPOBJ lpObjAttacker);
void gObjPlayerKiller(LPOBJ lpObj, LPOBJ lpTargetObj);
void gObjUserDie(LPOBJ lpObj, LPOBJ lpTargetObj);
void gObjInterfaceTimeCheck(LPOBJ lpObj);
void gObjPkDownTimeCheck(LPOBJ lpObj, int TargetLevel);
int gObjAngelSprite(LPOBJ lpObj);
int gObjSpiritOfGuardianSprite(LPOBJ lpObj);
int gObjSatanSprite(LPOBJ lpObj);
int gObjWingSprite(LPOBJ lpObj);
int gObjUniriaSprite(LPOBJ lpObj);
void gObjSpriteDamage(LPOBJ lpObj, int damage);
void gObjSecondDurDown(LPOBJ lpObj);
void gObjOfficialHelperStatus(LPOBJ lpObj, BYTE Status);
void gObjOfficialHelperTimer(LPOBJ lpObj);
void gObjChangeDurProc(LPOBJ lpObj);
void gObjWingDurProc(LPOBJ lpObj);
void gObjPenaltyDurDown(LPOBJ lpObj, LPOBJ lpTargetObj);
void gObjWeaponDurDown(LPOBJ lpObj, LPOBJ lpTargetObj, int type);
void gObjArmorRandomDurDown(LPOBJ lpObj, LPOBJ lpAttackObj, bool Check);
bool gObjIsSelfDefense(LPOBJ lpObj, int aTargetIndex);
void gObjCheckSelfDefense(LPOBJ lpObj, int aTargetIndex);
void gObjTimeCheckSelfDefense(LPOBJ lpObj);
void gObjAddSingleExp(LPOBJ lpObjDead, LPOBJ lpObj);
int gObjAttack(LPOBJ lpObj, LPOBJ lpTargetObj, class CMagicInf* lpMagic, int magicsend, unsigned char MSBFlag, int AttackDamage, BOOL bCombo);
void gObjMonsterExpDivision(LPOBJ lpMonObj, LPOBJ lpObj, int AttackDamage, int MSBFlag);
int gObjGetExperienceRate(LPOBJ lpObj, BYTE type);
int gObjMonsterExpSingle(LPOBJ lpObj, LPOBJ lpTargetObj, int dmg, int tot_dmg, int & LevelUp);
void gObjExpParty(LPOBJ lpObj, LPOBJ lpTargetObj, int AttackDamage, int MSBFlag);
void gObjMonsterDieLifePlus(LPOBJ lpObj, LPOBJ lpTartObj);
void gObjLifeCheck(LPOBJ lpTargetObj, LPOBJ lpObj, int AttackDamage, int DamageSendType, int MSBFlag, int MSBDamage, WORD Skill, int iShieldDamage);
int gObjInventoryTrans(int aIndex);
int gObjInventoryCommit(int aIndex);
int gObjInventoryRollback(int aIndex);
void gObjInventoryItemSet(int aIndex, int itempos, unsigned char set_byte);
void gObjInventoryItemBoxSet(int aIndex, int itempos, int xl, int yl, unsigned char set_byte);
bool gObjFixInventoryPointer(int aIndex);
void gObjInventoryItemSet_PShop(int aIndex, int itempos, unsigned char set_byte);
void gObjInventoryItemBoxSet_PShop(int aIndex, int itempos, int xl, int yl, unsigned char set_byte);
BYTE gObjInventoryDeleteItem(int aIndex, int itempos);
BYTE gObjWarehouseDeleteItem(int aIndex, int itempos);
BYTE gObjChaosBoxDeleteItem(int aIndex, int itempos);
unsigned char gObjInventoryInsertItem(LPOBJ lpObj, int type, int index, int level, int iSerial, int iDur, int ItemSlot1, int ItemSlot2, int ItemSlot3, int ItemSlot4, int ItemSlot5);
unsigned char gObjInventoryInsertItem(LPOBJ lpObj, int type, int index, int level, int iSerial, int iDur, int ItemSlot1, int ItemSlot2, int ItemSlot3, int ItemSlot4, int ItemSlot5);
unsigned char gObjOnlyInventoryInsertItem(int aIndex, class CItem item);
unsigned char gObjInventoryInsertItem(LPOBJ lpObj, int type, int index, int level);
int gObjSearchItem(LPOBJ lpObj, int item_type, int add_dur, int nLevel);
int gObjSearchItemMinus(LPOBJ lpObj, int pos, int m_dur);
unsigned char gObjShopBuyInventoryInsertItem(LPOBJ lpObj, int type, int index, int level, int iSerial, int iDur);
unsigned char gObjShopBuyInventoryInsertItem(int aIndex, class CItem item);
unsigned char gObjInventoryInsertItemTemp(LPOBJ lpObj, class CMapItem* Item);
unsigned char gObjInventoryInsertItem(int aIndex, class CMapItem* item);
unsigned char gObjInventoryInsertItem(int aIndex, class CItem item);
unsigned char gObjMonsterInventoryInsertItem(LPOBJ lpObj, int type, int index, int level, int op1, int op2, int op3);
unsigned char gObjInventoryRectCheck(int aIndex, int sx, int sy, int width, int height);
unsigned char gObjOnlyInventoryRectCheck(int aIndex, int sx, int sy, int width, int height);
int CheckInventoryEmptySpace(LPOBJ lpObj, int iItemHeight, int iItemWidth);
int gObjIsItemPut(LPOBJ lpObj, class CItem* lpItem, int pos);
unsigned char gObjWerehouseRectCheck(int aIndex, int sx, int sy, int width, int height);
unsigned char gObjMapRectCheck(unsigned char* lpMapBuf, int sx, int sy, int ex, int ey, int width, int height);
void gObjWarehouseItemBoxSet(int aIndex, int itempos, int xl, int yl, unsigned char set_byte);
void gObjWarehouseItemSet(int aIndex, int itempos, unsigned char set_byte);
void gObjChaosItemBoxSet(int aIndex, int itempos, int xl, int yl, unsigned char set_byte);
void gObjChaosItemSet(int aIndex, int itempos, unsigned char set_byte);
unsigned char gObjChaosBoxInsertItemPos(int aIndex, class CItem item, int pos, int source);
unsigned char gObjWarehouseInsertItemPos(int aIndex, class CItem item, int pos, int source);
unsigned char gObjInventoryInsertItemPos(int aIndex, class CItem item, int pos, int RequestCheck);
int gObjInventorySearchSerialNumber(LPOBJ lpObj, unsigned long serial);
int gObjInventorySearchSerialNumber2(LPOBJ lpObj, unsigned long serial);
int gObjWarehouseSearchSerialNumber(LPOBJ lpObj, unsigned long serial);
int gObjWarehouseSearchSerialNumber2(LPOBJ lpObj, unsigned long serial,bool Disconnect);
unsigned char gObjInventoryMoveItem(int aIndex, unsigned char source, unsigned char target, int& durSsend, int& durTsend, unsigned char sFlag, unsigned char tFlag, unsigned char* siteminfo);
unsigned char gObjTradeRectCheck(int aIndex, int sx, int sy, int width, int height);
int gObjTradeItemBoxSet(int aIndex, int itempos, int xl, int yl, unsigned char set_byte);
unsigned char gObjTradeInventoryMove(LPOBJ lpObj, unsigned char source, unsigned char target);
unsigned char gObjInventoryTradeMove(LPOBJ lpObj, unsigned char source, unsigned char target);
unsigned char gObjTradeTradeMove(LPOBJ lpObj, unsigned char source, unsigned char target);
void gObjTradeCancel(int aIndex);
void gObjTempInventoryItemBoxSet(unsigned char* TempMap, int itempos, int xl, int yl, unsigned char set_byte);
unsigned char gObjTempInventoryRectCheck(unsigned char* TempMap, int sx, int sy, int width, int height);
unsigned char gObjTempInventoryInsertItem(LPOBJ lpObj, class CItem item, unsigned char* TempMap);
unsigned char gObjTempInventoryInsertItem(LPOBJ lpObj, int itemType, BYTE * TempMap);
int TradeItemInventoryPutTest(int aIndex);
int TradeitemInventoryPut(int aIndex);
void gObjTradeOkButton(int aIndex);
unsigned char LevelSmallConvert(int level);
unsigned char LevelSmallConvert(int aIndex, int inventoryindex);
void gObjAttackDamageCalc(int aIndex);
void gObjMakePreviewCharSet(int aIndex);
void gObjViewportPaint(HWND hWnd, /*<regrel ebp+0xc>*/ /*|0x2|*/ short aIndex);
void InitFrustrum();
void CreateFrustrum(int x, int y, int aIndex);
bool TestFrustrum2(int x, int y, int aIndex);
int gObjCheckViewport(int aIndex, int x, int y);
void gObjViewportClose(LPOBJ lpObj);
void gObjViewportListCreate(short aIndex);
int ViewportAdd(int aIndex, int aAddIndex, int aType);
int ViewportAdd2(int aIndex, int aAddIndex, int aType);
void ViewportDel(short aIndex, int aDelIndex);
void Viewport2Del(short aIndex, int aDelIndex);
void gObjViewportAllDel(short aIndex);
void gObjViewportListDestroy(short aIndex);
void PShop_ViewportListRegenarate(short aIndex);
bool PShop_CheckInventoryEmpty(short aIndex);
void gObjStateSetCreate(int aIndex);
void gObjSetState();
void gObjSecondProc();
void gObjManaPotionFill(LPOBJ lpObj);
void gObjRestPotionFill(LPOBJ lpObj);
void gObjUseDrink(LPOBJ lpObj, int level);
void gObjViewportListProtocolDestroy(LPOBJ lpObj);
void gObjViewportListProtocolCreate(LPOBJ lpObj);
void gObjViewportListProtocol(short aIndex);
void gObjSkillUseProc(LPOBJ lpObj);
void gObjSkillBeAttackProc(LPOBJ lpObj);
void gObjTeleportMagicUse(int aIndex, unsigned char x, unsigned char y);
int gObjMoveGate(int aIndex, int gt);
BOOL gObjMoveHackCheck(int aIndex);
void gObjTeleport(int aIndex, int map, int x, int y);
void gObjMassMove(int Map, int ToMap, int ToX, int ToY);
void gObjGetExtraExperienceInfoMsg(int aIndex);
void gObjMoveDataLoadingOK(int aIndex);
class CMagicInf* gObjGetMagic(LPOBJ lpObj, int mIndex);
class CMagicInf* gObjGetMagicSearch(LPOBJ lpObj, WORD skillnumber);
//int gObjMagicManaUse(LPOBJ lpObj, class CMagicInf* lpMagic);
//int gObjMagicBPUse(LPOBJ lpObj, class CMagicInf* lpMagic);
int gObjPosMagicAttack(LPOBJ lpObj, class CMagicInf* lpMagic, unsigned char x, unsigned char y);
int gObjMagicAdd(LPOBJ lpObj, WORD aSkill, unsigned char Level);
int gObjMagicAddResetCheck(LPOBJ lpObj, WORD aSkill, unsigned char Level);
BOOL gObjMagicExist(LPOBJ lpObj, WORD Skill, int & SkillPosition);
int gObjMagicDel(LPOBJ lpObj, WORD aSkill, unsigned char Level);
void gObjMagicDelAll(LPOBJ lpObj);
int gObjMagicAdd(LPOBJ lpObj, unsigned char Type, unsigned char Index, unsigned char Level, WORD& SkillNumber);
int gObjWeaponMagicAdd(LPOBJ lpObj, WORD aSkill, BYTE Level);
int gObjMonsterMagicAdd(LPOBJ lpObj, WORD aSkill, BYTE Level);
int gObjMagicEnergyCheck(LPOBJ lpObj, WORD aSkill);

BOOL gObjJewelUpHackCheck(LPOBJ lpObj,int target);
int gObjItemLuckyRefineryUp(LPOBJ lpObj, int source, int target);
int gObjItemLuckyDurablityUp(LPOBJ lpObj, int source, int target);
int gObjItemLevelUp(LPOBJ lpObj, int source, int target);
int gObjItemRandomLevelUp(LPOBJ lpObj, int source, int target);
int gObjItemRandomOption3Up(LPOBJ lpObj, int source, int target);

void gObjAbilityReSet(LPOBJ lpObj);
int gObjTargetGuildWarCheck(LPOBJ lpObj, LPOBJ lpTargetObj);
void gObjGuildWarEndSend(struct _GUILD_INFO_STRUCT* lpGuild1, struct _GUILD_INFO_STRUCT* lpGuild2, unsigned char Result1, unsigned char Result2);
void gObjGuildWarEndSend(LPOBJ lpObj, unsigned char Result1, unsigned char Result2);
void gObjGuildWarEnd(struct _GUILD_INFO_STRUCT* lpGuild, struct _GUILD_INFO_STRUCT* lpTargetGuild);
int gObjGuildWarProc(struct _GUILD_INFO_STRUCT* lpGuild1, struct _GUILD_INFO_STRUCT* lpGuild2, int score);
int gObjGuildWarCheck(LPOBJ lpObj, LPOBJ lpTargetObj);
int gObjGuildWarMasterClose(LPOBJ lpObj);
int gObjGuildWarItemGive(struct _GUILD_INFO_STRUCT* lpWinGuild, struct _GUILD_INFO_STRUCT* lpLoseGuild);
int gObjMonsterCall(int aIndex, int MonsterType, int x, int y);
void gObjMonsterCallKill(int aIndex);
int gObjCheckTeleportArea(int aIndex, unsigned char x, unsigned char y);
int gObjCheckTeleportAreaByMap(BYTE mapnumber, BYTE x, BYTE y);
int gObjCheckAttackAreaUsedPath(int aIndex, int TarObjIndex);
int gObjCheckattackAreaUsedViewPort(int aIndex, int TarObjIndex);
int gObjCheckAttackArea(int aIndex, int TarObjIndex);
int gUserFindDevilSquareInvitation(int aIndex);
int gUserFindDevilSquareKeyEyes(int aIndex);
void gObjSendUserStatistic(int aIndex, int startLevel, int endLevel);
LPOBJ gObjFind(char* targetcharname);
int gObjFind10EventChip(int aIndex);
int gObjDelete10EventChip(int aIndex);
void gObjSetPosition(int aIndex, int x, int y);
int gObjGetItemCountInEquipment(int aIndex, int itemtype, int itemindex, int itemlevel);
int gObjGetItemCountInIventory(int aIndex, int itemnum);
int gObjGetItemCountInIventory(int aIndex, int itemtype, int itemindex, int itemlevel);
int gObjGetHPItemPos(int aIndex);
int gObjGetItemPos(int aIndex,int section,int itemid);
int gObjGetManaItemPos(int aIndex);
void gObjDelteItemCountInInventory(int aIndex, int itemtype, int itemindex, int count);
void gObjGetStatPointState(int aIndex, short& AddPoint, short& MaxAddPoint, short& MinusPoint, short& MaxMinusPoint);
int gObjCheckStatPointUp(int aIndex);
int gObjCheckStatPointDown(int aIndex);
BOOL gObjIsNewClass(LPOBJ lpObj);
int getCurrentEventType(LPOBJ lpObj,BYTE isParty);
int gObjGetLevelUpPointsByEventID(LPOBJ lpObj,int EventID);
void gObjUseCircle(int aIndex, int pos);
void gObjUsePlusStatFruit(int aIndex, int pos);
void gObjUseMinusStatFruit(int aIndex, int pos);
void gObjCalcMaxLifePower(int aIndex);
void gObjDelayLifeCheck(int aIndex);
void gObjNProtectGGCheckSum(int aIndex);
void SkillFrustrum(unsigned char bangle, int aIndex);
int SkillTestFrustrum(int x, int y, int aIndex);
//int gObjCheckMaxZen(int aIndex, int nAddZen);
void MakeRandomSetItem(int aIndex);
void MakeRewardSetItem(int aIndex, BYTE cDropX, BYTE cDropY, int iRewardType, int iMapnumber);
void gObjRecall(int aIndex, int mapnumber, int x, int y);
void gObjSetExpPetItem(int aIndex, int exp);
int gObjGetRandomItemDropLocation(int iMapNumber, BYTE& cX, BYTE& cY, int iRangeX, int iRangeY, int iLoopCount);
int gObjGetRandomFreeLocation(int iMapNumber, BYTE& cX, BYTE& cY, int iRangeX, int iRangeY, int iLoopCount);
int gObjCheckAttackTypeMagic(int iClass, int iSkill);
int gObjGetGuildUnionNumber(LPOBJ lpObj);
void gObjGetGuildUnionName(LPOBJ lpObj, char* szUnionName, int iUnionNameLen);
int gObjCheckRival(LPOBJ lpObj, LPOBJ lpTargetObj);
int gObjGetRelationShip(LPOBJ lpObj, LPOBJ lpTargetObj);
void gObjNotifyUpdateUnionV1(LPOBJ lpObj);
void gObjNotifyUpdateUnionV2(LPOBJ lpObj);
void gObjUnionUpdateProc(int aIndex);
void gObjSetKillCount(int aIndex, int iOption);
void gObjNotifyUseWeaponV1(LPOBJ lpOwnerObj, LPOBJ lpWeaponObj, int iTargetX, int iTargetY);
void gObjNotifyUseWeaponDamage(LPOBJ lpWeaponObj, int iTargetX, int iTargetY);
void gObjUseBlessAndSoulPotion(int aIndex, int iItemLevel);
void gObjWeaponDurDownInCastle(LPOBJ lpObj, LPOBJ lpTargetObj, int iDecValue);
void gObjCheckTimeOutValue(LPOBJ lpObj, DWORD& rNowTick);
void MsgOutput(int aIndex, char* msg, ...);
void PlayerRepairOverflow2ndClass(LPOBJ lpObj);
void PlayerRepairOverflow3rdClass(LPOBJ lpObj);
void gProcessAutoRecuperation(LPOBJ lpObj);
void gObjShieldAutoRefill(LPOBJ lpObj);
int gObjCheckOverlapItemUsingDur(int iUserIndex, int iMaxOverlapped, int iItemType, int iItemLevel);
int gObjOverlapItemUsingDur(class CItem* lpItem, int iMapNumber, int iItemNumber, int iUserIndex, int iMaxOverlapped, int iItemType, int iItemLevel);
int gObjCheckSerial0ItemList(class CItem* lpItem);
int gObjCheckSerialSpecialItemList(class CItem* lpItem);
BOOL gCheckSkillDistance(int aIndex, int aTargetIndex, int iSkillNum);
BOOL DeleteItemByLevel ( int iIndex, DWORD itemId, BYTE ItemLevel );
BOOL DeleteItemByID ( int iIndex, DWORD itemId);
void DeleteAllItems ( int iIndex );
void DeleteAllItemsLessInv ( int iIndex );
BOOL DeleteItemByMultipleValues ( int iIndex, DWORD itemId, BYTE ItemLevel, BYTE ItemOpt, BYTE ItemLuck, BYTE ItemSkill );
int gObjCheckItemsCount(int iIndex, short Type, BYTE ItemLevel, BYTE ItemOpt, BYTE ItemLuck, BYTE ItemSkill);
void gObjSaveChaosBoxItemList(LPOBJ lpObj);
int gObjDeleteItemsCount(int gObjId, short Type, short Level, int Count) ;
char gObjFixClassPacket(int DbClass);
BOOL gObjGetRandomFreeArea(int iMapNumber, BYTE &cX, BYTE &cY, int iSX, int iSY, int iDX, int iDY, int iLoopCount);

int gObjCheckItemPos(int iIndex, short Type) ;
int gObjCheckItemPosByDur(int iIndex, short Type, BYTE Dur) ;
int gObjCheckItemsCount(int iIndex, short Type, BYTE ItemLevel, BYTE ItemOpt, BYTE ItemLuck, BYTE ItemSkill, BYTE ItemExcellent, BYTE ItemAncient);
int gObjDeleteItemsCount(int iIndex, short Type, BYTE ItemLevel, BYTE ItemOpt, BYTE ItemLuck, BYTE ItemSkill, BYTE ItemExcellent, BYTE ItemAncient,int HowMany);

#endif