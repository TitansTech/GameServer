#if (ISEX700 == 1)

#ifndef COMPLEXMODULUS_H
#define COMPLEXMODULUS_H

class CComplexModulus
{
protected:

	static DWORD KEYS[];

public:

	int Encrypt(void * lpDest, void * lpSource, int iSize);
	int Decrypt(void * lpDest, void * lpSource, int iSize);
	void FixPacket_F3(unsigned char* lpBody,int iSize);

protected:

	void EncryptBlock(unsigned long* lpdwFrom,unsigned long* lpdwTo);
	void DecryptBlock(unsigned long* lpdwFrom,unsigned long* lpdwTo);
	int CComplexModulus::AddBits(void*lpDest,int iDestBitPos,void*lpSource,int iBitSourcePos,int iBitLen);
	DWORD Rol(DWORD x,BYTE shift);
	DWORD Ror(DWORD x,BYTE shift);
};

extern CComplexModulus g_ComplexModulusCS;

#endif

#endif