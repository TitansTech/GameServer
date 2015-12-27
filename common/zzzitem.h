// ------------------------------
// Decompiled by Deathway
// Date : 2007-03-09
// ------------------------------
#ifndef ZZZITEM_H
#define ZZZITEM_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAX_TYPE_PLAYER 7
#define MAX_ITEM_LEVEL	15
// ItemAttribute Begin:9439368 END 9446B68 Array[512]

// **************************
// Item Limits
// **************************
#define MAX_TYPE_ITEMS 16
#define MAX_SUBTYPE_ITEMS 512

#define MAX_EXOPTION_SIZE 8

// ********************
// MACRO for get Item
// ********************
#define ITEMGET(x,y) ( (x)*MAX_SUBTYPE_ITEMS + (y))

// **************
// Pet Level
// **************
#define MAX_PET_LEVEL 50

#define MAX_ITEM_DURABILITY	255

#define MAX_ITEMS (MAX_TYPE_ITEMS*MAX_SUBTYPE_ITEMS)

#define MAX_ITEM_SPECIAL_ATTRIBUTE 7

#define DBI_GET_TYPE(x) ( ((x)&((MAX_TYPE_ITEMS-1)<<8))>>4 )
#define DBI_GET_INDEX(x) ( (x) & 0xFF )
#define DBI_GET_LEVEL(x)  ( ((x)>>3)& MAX_ITEM_LEVEL )
#define DBI_GET_SKILL(x)  ( ((x) >> 7) & 0x01)
#define DBI_GET_LUCK(x)  ( ((x) >> 2) & 0x01)
#define DBI_GET_OPTION(x)  ( ( (x) ) & 0x03 )
#define DBI_GET_OPTION16(x) ( (x) & 0x40  )
#define DBI_GET_DINO_OPTION(x)  ( DBI_GET_OPTION16((x)) >> 4 )
#define DBI_GET_NOPTION(x)  ( (x) & 0x3F  )
#define DBI_GET_380OPTION(x) ( ((x) & 0x08) << 4  )
#define ITEM_GET_TYPE(x)  ( (x)/MAX_SUBTYPE_ITEMS  )
#define ITEM_GET_INDEX(x) ( (x)%MAX_SUBTYPE_ITEMS  )

#define DBI_TYPE			0
#define DBI_OPTION_DATA		1
#define DBI_DUR				2
#define DBI_SERIAL1			3
#define DBI_SERIAL2			4
#define DBI_SERIAL3			5
#define DBI_SERIAL4			6
#define DBI_NOPTION_DATA	7
#define DBI_SOPTION_DATA	8
#define DBI_OPTION380_DATA	9
#define DBI_JOH_DATA		10

#define MAX_ITEM_INFO	7+5

#define I_TYPE		0
#define I_OPTION	1
#define I_DUR		2
#define I_NOPTION	3
#define I_SOPTION	4
#define I_380OPTION	5
#define I_JOHOPTION	6

#define ITEM_DUR_RANGE(x) (((x)<0)?FALSE:((x)>MAX_ITEM_DURABILITY-1)?FALSE:true )

typedef struct
{
	char Name[32]; // 0
	BYTE HaveItemInfo; // 20
	BYTE TwoHand;	// 21
	BYTE Level; // 22
	BYTE Width;	// 23
	BYTE Height;	// 24
	char Serial; // 25
	BYTE OptionFlag; // 26
	BYTE MondownFlag; // 27
	BYTE AttackSpeed; // 28
	BYTE WalkSpeed; // 29
	BYTE DamageMin; // 2A
	BYTE DamageMax; // 2B
	BYTE SuccessfulBlocking; // 2C
	BYTE Defense; // 2D
	BYTE MagicDefense; // 2E
	BYTE Speed;	// 2F
	BYTE Durability; // 30
	BYTE MagicDurability; // 31
	BYTE AttackDur;	// 32
	BYTE DefenceDur; // 33
	WORD RequireStrength; // 34
	WORD RequireDexterity; // 36
	WORD RequireEnergy; // 38
	WORD RequireLevel; // 3A
	WORD Value; // 3C
	BYTE RequireClass[MAX_TYPE_PLAYER]; // 3E
	BYTE Resistance[MAX_ITEM_SPECIAL_ATTRIBUTE]; // unk43 - Ice poisonous lightning non ground wind water
	WORD RequireVitality;	// 4A
	WORD RequireLeadership;	// 68
	int BuyMoney; // 4C
	int MagicPW; // 50
	float RepaireMoneyRate;	// 54
	float AllRepaireMoneyRate; // 58
	bool QuestItem;	// 5C
	BYTE SetAttr; // 5D
	BYTE ResistanceType;	// 5E
	BYTE ItemSlot; // 60
	int SkillType; // 64
	BOOL isSocketItem;
}  ITEM_ATTRIBUTE, * LPITEM_ATTRIBUTE;

extern  ITEM_ATTRIBUTE ItemAttribute[MAX_ITEMS];



class CItem
{

public:

	CItem();

	void Convert(int type, BYTE Option1, BYTE Option2, BYTE Option3,BYTE Attribute2, BYTE SetOption, BYTE ItemEffectEx, BYTE DbVersion);
	void Value();
	void OldValue();
	int GetSize(int& w, int& h);
	void Clear();
	int IsItem();
	int IsSetItem();
	int GetAddStatType();
	void SetSocketItemInfo(BYTE ItemSocket1, BYTE ItemSocket2, BYTE ItemSocket3, BYTE ItemSocket4, BYTE ItemSocket5);
	void SetPetItemInfo(int petlevel, int petexp);
	int AddPetItemExp(int aIndex,int petexp);
	int DecPetItemExp(int percent);
	void PetValue();
	int PetItemLevelDown(int exp);
	int ItemDamageMin();
	int ItemDefense();
	int IsClass(char aClass, int ChangeUP);
	LPSTR GetName();
	//int GetLevel();
	void PlusSpecial(int* Value, int Special);
	void PlusSpecialPercent(int* Value, int Special, WORD Percent);
	void PlusSpecialPercentEx(int* Value, int SourceValue, int Special);
	void SetItemPlusSpecialStat(short* Value, int Special);
	int GetWeaponType();
	void PlusSpecialSetRing(unsigned char* Value);
	DWORD GetNumber();
	int IsExtItem();
	int IsExtLifeAdd();
	int IsExtManaAdd();
	int IsExtDamageMinus();
	int IsExtDamageReflect();
	int IsExtDefenseSuccessfull();
	int IsExtMonsterMoney();
	int IsExtExcellentDamage();
	int IsExtAttackRate();
	int IsExtAttackRate2();
	int IsExtAttackSpeed();
	int IsExtMonsterDieLife();
	int IsExtMonsterDieMana();
	int IsWingOpGetOnePercentDamage();
	//int __thiscall IsWingOpGetLifeToMonster();
	int IsWingOpGetManaToMoster();
	int IsDinorantReduceAttackDamaege();
	int IsFenrirIncLastAttackDamage();
	int IsFenrirDecLastAttackDamage();
	int IsFenrirIllusion();
	int SimpleDurabilityDown(int iDur);
	int DurabilityDown(int dur, int aIndex);
	int DurabilityDown(int dur, int aIndex, int Pos);
	int DurabilityDown2(int dur, int aIndex);
	int NormalWeaponDurabilityDown(int defence, int aIndex);
	int BowWeaponDurabilityDown(int defence, int aIndex);
	int StaffWeaponDurabilityDown(int defence, int aIndex);
	int ArmorDurabilityDown(int damagemin, int aIndex);
	int ArmorDurabilityDownNoCheck(int aIndex);
	int CheckDurabilityState();
	BOOL CheckThirdWingsSuccessfulBlocking();
	BOOL CheckFullManaRecoveryWing();
	BOOL CheckFullLifeRecoveryWing();
	BOOL CheckDamageReturnWing();


	

	DWORD m_Number; // 0
	char m_serial;	// 4
	short m_Type; // 6
	short m_Level; // 8
	BYTE m_Part;	// A
	BYTE m_Class;	// B
	BYTE m_TwoHand;	// C
	BYTE m_AttackSpeed;	// D
	BYTE m_WalkSpeed;	// E
	WORD m_DamageMin;	// 10
	WORD m_DamageMax;	// 12
	BYTE m_SuccessfulBlocking;	// 14
	WORD m_Defense;	// 16
	WORD m_MagicDefense;	// 18
	BYTE m_Speed;	// 1A
	WORD m_DamageMinOrigin;	// 1C
	WORD m_DefenseOrigin;	// 1E
	WORD m_Magic;	// 20
	float m_Durability; // 24
	WORD m_DurabilitySmall; // 28
	float m_BaseDurability;	// 2C
	BYTE m_SpecialNum;	// 30
	WORD m_Special[8];	// 31
	BYTE m_SpecialValue[8];	// 39
	WORD m_RequireStrength;	// 42
	WORD m_RequireDexterity;	// 44
	WORD m_RequireEnergy;	// 46
	WORD m_RequireLevel;	// 48
	WORD m_RequireVitality;	// 4A
	WORD m_RequireLeaderShip;	// 4C
	WORD m_Leadership;	// 4E
	BYTE m_RequireClass[MAX_TYPE_PLAYER];	// 50
	BYTE m_Resistance[MAX_ITEM_SPECIAL_ATTRIBUTE];	// 55
	int m_Value;	// 5C
	DWORD m_SellMoney;	// 60
	DWORD m_BuyMoney;	// 64
	int m_iPShopValue;	// 68
	bool m_bItemExist;	// 6C
	int m_OldSellMoney;	// 70
	int m_OldBuyMoney;	// 74
	BYTE m_SkillOption;	// 78  Skill
	BYTE m_LuckOption;	// 79  Luck
	BYTE m_Z28Option;	// 7A  Option
	BYTE m_NewOption;	// 7B ExcellentOption
	float m_DurabilityState[4];	// 7C
	float m_CurrentDurabilityState;	// 8C
	bool m_SkillChange;	// 90
	bool m_QuestItem;	// 91
	BYTE m_SetOption;	// 92
	BYTE m_SetAddStat;	// 93
	bool m_IsValidItem;	// 94
	BYTE m_SkillResistance[MAX_ITEM_SPECIAL_ATTRIBUTE];	// 95
	BOOL m_IsLoadPetItemInfo;	// 9C
	int  m_PetItem_Level;	// A0
	int  m_PetItem_Exp; // A4
	BYTE m_JewelOfHarmonyOption;	// A8
	WORD m_HJOpStrength;	// AA
	WORD m_HJOpDexterity;	// AC
	BYTE m_ItemOptionEx;	// AE
	BYTE m_ItemSlot1;	// AE
	BYTE m_ItemSlot2;	// AE
	BYTE m_ItemSlot3;	// AE
	BYTE m_ItemSlot4;	// AE
	BYTE m_ItemSlot5;	// AE
};

class CPetItemExp
{

public:

	int m_DarkSpiritExpTable[MAX_PET_LEVEL+2];
	int m_DarkHorseExpTable[MAX_PET_LEVEL+2];

public:

	CPetItemExp()
	{
		this->m_DarkSpiritExpTable[0] = 0;
		this->m_DarkSpiritExpTable[1] = 0;

		for ( int i = 2 ; i < MAX_PET_LEVEL+2 ; i++ )
		{
			this->m_DarkSpiritExpTable[i] = (i+10) * i * i * i * 100;
		}

		this->m_DarkHorseExpTable[0] = 0;
		this->m_DarkHorseExpTable[1] = 0;

		for ( int i = 2 ; i < MAX_PET_LEVEL+2 ; i++ )
		{
			this->m_DarkHorseExpTable[i] = (i+10) * i * i * i * 100;
		}
	};
};


extern int g_MaxItemIndexOfEachItemType[MAX_TYPE_ITEMS];

void BufferItemtoConvert3(unsigned char* buf, int& type, BYTE& level, BYTE& op1, BYTE& op2, BYTE& op3, BYTE& dur);
void ItemByteConvert7(unsigned char * buf, CItem * const item , int maxitem);
void ItemByteConvert10(unsigned char * buf, CItem * const item , int maxitem);
void ItemByteConvert16(LPBYTE buf, CItem * const item , int maxitem);
void ItemByteConvert(unsigned char* buf, CItem item);
void ItemByteConvert(unsigned char* buf, int type, BYTE Option1, BYTE Option2, BYTE Option3, BYTE level, BYTE dur, BYTE Noption, BYTE SetOption, BYTE JewelOfHarmonyOption, BYTE ItemEffectEx, BYTE SocketSlot1, BYTE SocketSlot2, BYTE SocketSlot3, BYTE SocketSlot4, BYTE SocketSlot5);
int ItemGetNumberMake(int type, int index);
void ItemGetSize(int index, int & width, int & height);
BOOL HasItemDurability(int index);
int ItemGetDurability(int index, int itemLevel, int ExcellentItem, int SetItem);
int ItemGetAttackDurability(int index);
int ItemGetDefenseDurability(int index);
float GetRepairItemRate(int index);
float GetAllRepairItemRate(int index);
void CalRepairRate( int itemtype, int itemsubtype, LPITEM_ATTRIBUTE p);
BOOL OpenItemScript(char* FileName);
//BOOL OpenItemScript(char* Buffer, int iSize);
int zzzItemLevel(int type, int index, int level);
int GetLevelItem(int type, int index, int level);
int GetSerialItem(int type);
int IsItem(int item_num);
bool IsLuckyItem(int item_num);
void DestroyLuckyItem(int aIndex, int target);

LPITEM_ATTRIBUTE GetItemAttr(int item_num);

void InitSocketItems(char *FilePathSockets);
int IsSlotItem(WORD ItemID);

#endif