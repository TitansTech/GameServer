
#if (PACK_EDITION>=3)
#if (CRYSTAL_EDITION == 1)

#define CRYSTALWALL_MAXCRC			10
#define CHEATDETECTIONMETHOD_COUNT	8

struct PMSG_SENDCRC
{
	PBMSG_HEAD h;
	BYTE SubCode;
	BYTE SubSubCode;
	ULONG CRC[CRYSTALWALL_MAXCRC];
};

struct PMSG_SENDCHEATDATA
{
	PBMSG_HEAD h;
	BYTE SubCode;
	BYTE SubSubCode;
	int Extra;
};

class cCrystalWall
{
public:
	void Read(char * FilePath);
	void SendCRC(int aIndex);
	void ProtocolCore(int aIndex, BYTE * aRecv);	
	bool Action(int aIndex, BYTE Cheat);
	void RecvPing(int aIndex);
	void CheckPing(int aIndex);
	bool ItemSerialAction(int aIndex);

	BYTE UseTTMain;
	BYTE MsgShowToClient;
	BYTE UseCrystalShield;
	BYTE EncryptKey;
	BYTE CheckItemSerialOnOpenVault;
	BYTE IsHaveSameSerialDeleteItems;
	BYTE IsHaveSameSerialKickPlayer;
//Speed Max var:
	BYTE SpeedMax_UseMaxLimit;
	WORD SpeedMax_Physic;
	WORD SpeedMax_Magic;
	BYTE Crystal_IsAntiSpeedHack;
	int Crystal_IsAntiSpeedHack_MaxDif;
	BYTE Crystal_IsAntiSpeedHack_MinDet;
	WORD Crystal_PingCheckSeconds;

private:
	BYTE IsSendCRC;
	ULONG CRC[CRYSTALWALL_MAXCRC];
	int OnCheatAction[CHEATDETECTIONMETHOD_COUNT][4];
};
extern cCrystalWall CrystalWall;

#endif
#endif