#include "stdafx.h"
#include "GameServer.h"
#include "LogProc.h"
#include "GameMain.h"
#include "User.h"
#include "TNotice.h"
#include "AutoExp.h"
#include "..\common\winutil.h"
#include "..\include\readscript.h"
//#include "GameServerAuth.h"


#if (PACK_EDITION>=3)

AutoExpClass AutoExp;

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


void AutoExpClass::Init(char * FilePath)
{	
	this->Enabled = 0;
	this->Time = 0;
	this->BaseExp = 0;
	this->LevelUpPoints = 0;

	this->Enabled		= GetPrivateProfileInt("AutoExp", "SCFAutoExpEnabled", 0, FilePath);
	this->BaseExp		= GetPrivateProfileInt("AutoExp", "SCFAutoExpBaseExp", 8000, FilePath);
	int Secs			= GetPrivateProfileInt("AutoExp", "SCFAutoExpTimeCheck", 180, FilePath);
	this->LevelUpPoints = GetPrivateProfileInt("AutoExp", "SCFAutoExpLevelUpPoint", 1, FilePath);
	
	if(this->Enabled == 1)
	{
		this->Time		= (1000 * Secs);
		_beginthread( AutoExpClass__AddExp, 0, NULL  );
	}
}


void AutoExpClass__AddExp(void * lpParam)
{
	PMSG_KILLPLAYER pkillMsg;
	PHeadSetBE((LPBYTE)&pkillMsg, 0x16, sizeof(pkillMsg));

	while(AutoExp.Enabled)
	{
		Sleep(AutoExp.Time);
		for ( int n = OBJ_STARTUSERINDEX ; n < OBJMAX ; n++)
		{
			if ( gObj[n].Connected == PLAYER_PLAYING )
			{
				if ( gObj[n].Type == OBJ_USER )
				{
					if((gObj[n].Level < ReadConfig.Max_Normal_Level)||(gObj[n].MasterCharacterInfo->MasterLevel < ReadConfig.Max_Master_Level))
					{
						if(gObjItsInSafeZone(gObj[n].m_Index) == 1)//If safezone
						{
							int AddExp = 0;
							AddExp = (int)(AutoExp.BaseExp * gObj[n].Level) ;
						
							if(AddExp > 0x0FFFF)
								AddExp = 65534;

							if(AddExp > 0)
							{
								pkillMsg.NumberH = 0xFF;
								pkillMsg.NumberL = 0xFF;
								pkillMsg.ExpH = SET_NUMBERH(AddExp);
								pkillMsg.ExpL = SET_NUMBERL(AddExp);
								pkillMsg.DamageH = SET_NUMBERH(0x00);
								pkillMsg.DamageL = SET_NUMBERL(0x00);

								DataSend(gObj[n].m_Index, (UCHAR*)&pkillMsg, pkillMsg.h.size);

								gObjSetExpPetItem(n, AddExp);
								int LevelUp = 0;
								gObjLevelUp(&gObj[n],AddExp,gObj[n].Class,EVENT_TYPE_AUTOEXP,LevelUp);
							}
						}
					}
				}
			}
		}
	}
}

#endif