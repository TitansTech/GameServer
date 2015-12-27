#include "stdafx.h"

#if (ISEX700 == 1)
#include "ComplexModulus.h"

CComplexModulus g_ComplexModulusCS;

DWORD CComplexModulus::KEYS[] = {
0xE2CB2C47,0x676CCC60,0x59C06192,0x8AB3B66D,0x23A82990,0x4DA877B9,0x254A0788,0x83EFE892,
0xF0D21807,0x527CED5D,0x9A165576,0xB51ED51A,0x65298BF3,0x5EACAF1A,0x004F0813,0xB30F3FF9,
0x7751A486,0x83A69183,0x94B9397C,0x0D3F9C51,0x3A3395F4,0xEBF511C5,0x954C2EB4,0x082E330B,
0x94F0A80A,0xC661B594,0xE9744643,0x2EB9425B,0x630A3121,0x0F8758D8,0x5424534C,0xCB251A98,
0xD422BA68,0x4A4487F9,0x6D124452,0x5A7E6FAF,0x9ADB16B7,0xFF1583DA,0x8B232CEC,0x1FC6BC3C,
0x01FC75E5,0x39036FF6,0xDC4B0869,0xC7808981,
};

DWORD CComplexModulus::Rol(DWORD x,BYTE shift)
{
	return (x << shift) | (x >> (sizeof(x)*CHAR_BIT - shift));
}

DWORD CComplexModulus::Ror(DWORD x,BYTE shift)
{
	return (x >> shift) | (x << (sizeof(x)*CHAR_BIT - shift));
}

void CComplexModulus::FixPacket_F3(unsigned char* lpBody,int iSize)
{
	BYTE btmp1;
	BYTE btmp2;

	if( iSize < 4 )
		return;

	if( iSize < 8 )
	{
		lpBody[0] = (lpBody[0]-((lpBody[0]>>4)<<4))<<4 | (lpBody[0]>>4);

		btmp1 = lpBody[1];
		lpBody[1] = lpBody[0];
		lpBody[0] = btmp1;

		lpBody[3] ^= 95;
		lpBody[3] ^= 192;
		lpBody[3] ^= 1;
		lpBody[3] ^= 75;

		btmp1 = lpBody[2];
		lpBody[2] = lpBody[0];
		lpBody[0] = btmp1;

		btmp1 = (lpBody[3]>>4)&1;
		btmp2 = (lpBody[3]>>7)&1;

		(btmp1) ? lpBody[3] |= 128 : lpBody[3] &=~128;
		(btmp2) ? lpBody[3] |= 16 : lpBody[3] &=~16;

		lpBody[2] ^= 46;
	}
	else if( iSize < 16 )
	{
		btmp1 = lpBody[7];
		lpBody[7] = lpBody[0];
		lpBody[0] = btmp1;

		lpBody[6] ^= 75;

		btmp1 = (lpBody[1]>>6)&1;
		btmp2 = (lpBody[1]>>4)&1;

		(btmp1) ? lpBody[1] |= 4 : lpBody[1] &=~4;
		(btmp2) ? lpBody[1] |= 64 : lpBody[1] &=~64;

		btmp1 = lpBody[1];
		lpBody[1] = lpBody[4];
		lpBody[4] = btmp1;

		btmp1 = (lpBody[3]>>1)&1;
		btmp2 = (lpBody[3]>>7)&1;

		(btmp1) ? lpBody[3] |= 128 : lpBody[3] &= ~128;
		(btmp2) ? lpBody[3] |= 2 : lpBody[3] &= ~2;

		btmp1 = (lpBody[0]>>3)&1;
		btmp2 = (lpBody[0]>>1)&1;

		(btmp1) ? lpBody[0] |= 2 : lpBody[0] &= ~2;
		(btmp2) ? lpBody[0] |= 8 : lpBody[0] &= ~8;

		btmp1 = lpBody[6];
		lpBody[6] = lpBody[3];
		lpBody[3] = btmp1;
	}
	else if( iSize < 32 )
	{
		btmp1 = lpBody[6];
		lpBody[6] = lpBody[2];
		lpBody[2] = btmp1;

		lpBody[4] = (lpBody[4]-((lpBody[4]>>5)<<5))<<3 | (lpBody[4]>>5);
		lpBody[6] = (lpBody[6]-((lpBody[6]>>7)<<7))<<1 | (lpBody[6]>>7);
		lpBody[14] ^= 147;

		lpBody[13] = (lpBody[13]-((lpBody[13]>>1)<<1))<<7 | (lpBody[13]>>1);
		lpBody[8] = (lpBody[8]-((lpBody[8]>>7)<<7))<<1 | (lpBody[8]>>7);
	}
	else
	{
		lpBody[30] = (lpBody[30]-((lpBody[30]>>5)<<5))<<3 | (lpBody[30]>>5);
		lpBody[31] ^= 130;
		lpBody[18] ^= 244;
		lpBody[8] = (lpBody[8]-((lpBody[8]>>5)<<5))<<3 | (lpBody[8]>>5);
	}
}

void CComplexModulus::DecryptBlock(unsigned long* lpdwFrom,unsigned long* lpdwTo)
{
	LPDWORD lpdwKey = &this->KEYS[42];

	int DEC2 = lpdwFrom[1];
	int DEC4 = lpdwFrom[3];
	int DEC1 = lpdwFrom[0]-lpdwKey[0];
	int DEC3 = lpdwFrom[2]-lpdwKey[1];
	int tmpDEC1;
	int tmpDEC4;

	lpdwKey -=2;
	int loopcount = 20;

	do
	{
		tmpDEC1 = DEC1;
		DEC1 = DEC4-lpdwKey[0];
		DEC4 = DEC3;
		DEC3 = DEC2-lpdwKey[1];
		lpdwKey-=2;

		DEC2 = (tmpDEC1+tmpDEC1+1)*tmpDEC1;
		tmpDEC4 = (DEC4+DEC4+1)*DEC4;

		tmpDEC4 = this->Rol(tmpDEC4,5);
		DEC2 = this->Rol(DEC2,5);

		DEC3 = this->Ror(DEC3,DEC2&0xFF);
		DEC1 = this->Ror(DEC1,tmpDEC4&0xFF);

		DEC3 ^= tmpDEC4;
		DEC1 ^= DEC2;

		DEC2 = tmpDEC1;
		loopcount--;
	}while(loopcount != 0 );

	DEC4 -= lpdwKey[1];
	DEC2 -= lpdwKey[0];

	lpdwTo[0] = DEC1;
	lpdwTo[1] = DEC2;
	lpdwTo[2] = DEC3;
	lpdwTo[3] = DEC4;
}

void CComplexModulus::EncryptBlock(unsigned long* lpdwFrom,unsigned long* lpdwTo)
{
	LPDWORD lpdwKey = &this->KEYS[0];

	int tmpENC3 = lpdwFrom[0];
	int ENC3 = lpdwFrom[2];
	int ENC2 = lpdwKey[0]+lpdwFrom[1];
	int ENC4 = lpdwKey[1]+lpdwFrom[3];
	int tmpENC2;
	int tmpENC4;
	int ENC1;

	lpdwKey+= 2;
	int loopcount = 20;
	int esp1c = ENC3;

	do
	{
		tmpENC2 = (ENC2+ENC2+1)*ENC2;
		tmpENC4 = (ENC4+ENC4+1)*ENC4;

		tmpENC2 = this->Rol(tmpENC2,5);
		tmpENC4 = this->Rol(tmpENC4,5);

		ENC3 = tmpENC2;
		ENC3 ^= tmpENC3;
		ENC3 = this->Rol(ENC3,tmpENC4&0xFF);

		tmpENC4 ^= esp1c;
		tmpENC4 = this->Rol(tmpENC4,tmpENC2&0xFF);

		esp1c = ENC4;

		ENC4 = lpdwKey[0];

		tmpENC4 += lpdwKey[1];
		tmpENC3 = ENC2;

		ENC4 += ENC3;

		lpdwKey+=2;
		loopcount--;
		ENC2 = tmpENC4;
	}while(loopcount != 0);

	ENC3 = esp1c;
	ENC3 += lpdwKey[1];

	ENC1 = lpdwKey[0];
	ENC1 += tmpENC3;

	lpdwTo[0] = ENC1;
	lpdwTo[1] = ENC2;
	lpdwTo[2] = ENC3;
	lpdwTo[3] = ENC4;
}

int CComplexModulus::Decrypt(void* lpDest,void* lpSource,int iSize)
{
	if ( lpDest == NULL)
		return iSize;

	LPBYTE lpTempDest = (LPBYTE)lpDest;
	LPBYTE lpTempSrc = (LPBYTE)lpSource;

	int iResult = 0;
	int iDecLen = 0;

	BYTE btSizeDiff = lpTempSrc[iSize-1];

	if ( iSize > 0 )
	{
		int lOfs = 0;
		do
		{
			this->DecryptBlock((LPDWORD)&lpTempSrc[lOfs],(LPDWORD)&lpTempDest[lOfs]);
			lOfs += 16;
		}while( lOfs < iSize );
	}
	return (iSize-btSizeDiff);
}

int CComplexModulus::Encrypt(void* lpDest,void* lpSource,int iSize)
{
	if ( lpDest == NULL)
		return iSize;

	LPBYTE lpTempDest = (LPBYTE)lpDest;
	LPBYTE lpTempSrc = (LPBYTE)lpSource;

	int iResult = 0;
	int iDecLen = 0;
	int lOfs = 0;

	if ( iSize > 0 )
	{
		do
		{
			this->EncryptBlock((LPDWORD)&lpTempSrc[lOfs],(LPDWORD)&lpTempDest[lOfs]);
			lOfs += 16;
		}while( lOfs < iSize );
		lpTempDest[lOfs] = lOfs-iSize;
		lOfs++;
	}
	return lOfs;
}

#endif