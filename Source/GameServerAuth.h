// ------------------------------
// Decompiled by Deathway
// Date : 2007-03-09
// ------------------------------
#ifndef GAMESERVERAUTH_H
#define GAMESERVERAUTH_H


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum DATA_FILE_INDEX {
  FILE_ITEM = 0x0,
  FILE_ITEMLOCAL = 0x1,
  FILE_SKILL = 0x2,
  FILE_SKILLLOCAL = 0x3,
  FILE_QUEST = 0x4,
  FILE_QUESTLOCAL = 0x5,
  FILE_CHECKSUM = 0x6,
  FILE_MONSTER = 0x7,
  FILE_GATE = 0x8,
  FILE_MONSTERSETBASE = 0x9,
  FILE_NPGGAUTH = 0xa,
  FILE_SHOP0 = 0xb,
  FILE_SHOP1 = 0xc,
  FILE_SHOP2 = 0xd,
  FILE_SHOP3 = 0xe,
  FILE_SHOP4 = 0xf,
  FILE_SHOP5 = 0x10,
  FILE_SHOP6 = 0x11,
  FILE_SHOP7 = 0x12,
  FILE_SHOP8 = 0x13,
  FILE_SHOP9 = 0x14,
  FILE_SHOP10 = 0x15,
  FILE_SHOP11 = 0x16,
  FILE_SHOP12 = 0x17,
  FILE_MOVEREQ = 0x18,
  FILE_MOVEREQLOCAL = 0x19,
  FILE_SETITEMOPTION = 0x1a,
  FILE_SETITEMOPTIONLOCAL = 0x1b,
  FILE_SETITEMTYPE = 0x1c,
  FILE_SETITEMTYPELOCAL = 0x1d,
};



class CGameServerAuth
{
public:
	CGameServerAuth();
	virtual ~CGameServerAuth();

	void SendInfo();
	void Init();
	void GetKey(char* key,int startindex,int len);
	void RequestData(int);
	void SetHandle(DWORD bHandle){this->m_bDllHandle = bHandle;};
	DWORD GetHandle() {return this->m_bDllHandle;};
	int GetDataBufferSize();
	char* GetDataBuffer();
	DWORD SetInfo(short iServerCode, char* szServerVersion,  char* szServerName, int ServerType,  void  (*callbackfunc)(int, int));
	void GetClientVersion(char* szClientVersion,  char* szClientSerial);

private:

  HINSTANCE m_bHandle;
  int m_iTimer;
  unsigned long m_bDllHandle;
  unsigned long  (*WZAGSerInfo)(int, unsigned char, short, char*, char*, int, void  (*)(int, int));
  void  (*WZAGGetClientVersion)(unsigned long, char*, char*);
  void  (*WZAGSetSubInfo)(unsigned long, int, int, int, int, float, int);
  void  (*WZAGGetKey)(unsigned long, char*, int, int);
  void  (*WzAGRequestData)(unsigned long, int);
  int  (*WzAGGetDataBufferSize)(unsigned long);
  char*  (*WzAGGetDataBuffer)(unsigned long);
  int  (*WZAGConnectSetFS)(unsigned long);
  void  (*WzAGReleaseHandle)(unsigned long);

};

void GameServerAuthCallBackFunc(int Type, int pData);



extern CGameServerAuth gGameServerAuth;


#endif