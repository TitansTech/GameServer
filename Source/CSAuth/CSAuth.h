#ifndef CCSAUTH_H___
#define CCSAUTH_H___


#ifdef CSAUTH_EXPORTS
#define CSAUTH_API __declspec(dllexport)
#else
#define CSAUTH_API __declspec(dllimport)
#endif


class CCSAuth 
{

public:

	CCSAuth(void);
	~CCSAuth();

	void Init();
	BOOL IsAuth();
	void np_srandom(int iSeed);
	int np_random();
	DWORD GetAuthDword();
	//BOOL TransAuthDword(DWORD dwAuth);
	//GetAuthResults
	//EncryptPacket
	//DecryptPacket
	// CheckLastPacket
	DWORD PPGetLastError();
	
private:

	DWORD m_dwRandomSeed;	// unk0 : Confirmed
	DWORD m_dwAuthDword;	// unk4 : Confirmed
	BOOL m_bIsAuth;	// unk8 : Confirmed
	int unkC;
	int unk10[10];
	// Cover until 0x38
	int unk38;
	int unk3C;
	int unk40[12];
	// Cover until 0x70
	DWORD m_dwLastError;	// unk70 : Confirmed


};

int _LoadAuthTable(char* filename);
int _LoadAuthIndex(char * filename);

#endif