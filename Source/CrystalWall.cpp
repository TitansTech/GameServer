#include "stdafx.h"
#include "GameServer.h"
#include "LogProc.h"
#include "GameMain.h"
#include "User.h"
#include "TNotice.h"
#include "LogToFile.h"
#include "CrystalWall.h"
#include "SCFExDBProtocol.h"
#include "..\common\winutil.h"
#include "..\include\readscript.h"
#include "AntiFloodSystem.h"

#if (CRYSTAL_EDITION == 1)
#if (PACK_EDITION>=3)

CLogToFile CW_HACK_LOG("CRYSTALWALL_ANTI_HACK_LOG", ".\\CRYSTALWALL_ANTI_HACK_LOG", 1);	// line : 68

cCrystalWall CrystalWall;

void cCrystalWall::Read(char * FilePath)
{		
	this->UseTTMain				 = GetPrivateProfileInt("Common", "SCFUseTTMain",0, FilePath);
	this->MsgShowToClient		 = GetPrivateProfileInt("Common", "SCFShowMsgToClient",0, FilePath);
	this->UseCrystalShield		 = GetPrivateProfileInt("Common", "SCFUseCrystalShield",0, FilePath);
	this->EncryptKey			 = GetPrivateProfileInt("Common", "SCFEncryptKey",0, FilePath);

	this->CheckItemSerialOnOpenVault	= GetPrivateProfileInt("ItemSerials", "SCFCheckItemSerialOnOpenVault",0, FilePath);
	this->IsHaveSameSerialDeleteItems	= GetPrivateProfileInt("ItemSerials", "SCFIsHaveSameSerialDeleteItems",0, FilePath);
	this->IsHaveSameSerialKickPlayer	= GetPrivateProfileInt("ItemSerials", "SCFIsHaveSameSerialKickPlayer",0, FilePath);

	this->SpeedMax_UseMaxLimit	 = GetPrivateProfileInt("MaxSpeed", "SCFUseMaxLimit",0, FilePath);
	this->SpeedMax_Physic		 = GetPrivateProfileInt("MaxSpeed", "SCFMaxPhysic",850, FilePath);
	this->SpeedMax_Magic		 = GetPrivateProfileInt("MaxSpeed", "SCFMaxMagic",850, FilePath);

	this->Crystal_IsAntiSpeedHack		 = GetPrivateProfileInt("Cheats", "SCFUseCrystalAntiHackSpeed",0, FilePath);
	this->Crystal_IsAntiSpeedHack_MaxDif = GetPrivateProfileInt("Cheats", "SCFAntiHackSpeedMaxDif",0, FilePath);	
	this->Crystal_IsAntiSpeedHack_MinDet = GetPrivateProfileInt("Cheats", "SCFAntiHackSpeedMinDetect",0, FilePath);	
	this->Crystal_PingCheckSeconds		 = GetPrivateProfileInt("Cheats", "SCFPingCheckSeconds",55, FilePath);	

	this->IsSendCRC = GetPrivateProfileInt("CRC32", "SCFSendCRC",0, FilePath);

	this->CRC[0] = GetPrivateProfileInt("CRC32", "SCF_CWS01",0, FilePath);
	this->CRC[1] = GetPrivateProfileInt("CRC32", "SCF_CWS02",0, FilePath);
	this->CRC[2] = GetPrivateProfileInt("CRC32", "SCF_CWS03",0, FilePath);
	this->CRC[3] = GetPrivateProfileInt("CRC32", "SCF_CWS04",0, FilePath);
	this->CRC[4] = GetPrivateProfileInt("CRC32", "SCF_CWS05",0, FilePath);
	this->CRC[5] = GetPrivateProfileInt("CRC32", "SCF_CWS06",0, FilePath);
	this->CRC[6] = GetPrivateProfileInt("CRC32", "SCF_Config",0, FilePath);

	this->CRC[7] = GetPrivateProfileInt("CRC32", "SCF_TitanAddOn",0, FilePath);

#if (CRYSTAL_EDITION == 1)
	this->CRC[8] = GetPrivateProfileInt("CRC32", "SCF_TitanGuiEngine",0, FilePath);
	this->CRC[9] = GetPrivateProfileInt("CRC32", "SCF_TitanGuiAddOn",0, FilePath);
#else
	this->CRC[8] = 0;
	this->CRC[9] = 0;
#endif

	this->OnCheatAction[0][0] = GetPrivateProfileInt("OnNoPingDetection", "SCF_BanCharacter",0, FilePath);
	this->OnCheatAction[0][1] = GetPrivateProfileInt("OnNoPingDetection", "SCF_BanAccount",0, FilePath);
	this->OnCheatAction[0][2] = GetPrivateProfileInt("OnNoPingDetection", "SCF_BanIP",0, FilePath);
	this->OnCheatAction[0][3] = GetPrivateProfileInt("OnNoPingDetection", "SCF_Disconnect",0, FilePath);

	this->OnCheatAction[1][0] = GetPrivateProfileInt("OnCRCDetection", "SCF_BanCharacter",0, FilePath);
	this->OnCheatAction[1][1] = GetPrivateProfileInt("OnCRCDetection", "SCF_BanAccount",0, FilePath);
	this->OnCheatAction[1][2] = GetPrivateProfileInt("OnCRCDetection", "SCF_BanIP",0, FilePath);
	this->OnCheatAction[1][3] = GetPrivateProfileInt("OnCRCDetection", "SCF_Disconnect",0, FilePath);

	this->OnCheatAction[2][0] = GetPrivateProfileInt("OnWndTitleDetection", "SCF_BanCharacter",0, FilePath);
	this->OnCheatAction[2][1] = GetPrivateProfileInt("OnWndTitleDetection", "SCF_BanAccount",0, FilePath);
	this->OnCheatAction[2][2] = GetPrivateProfileInt("OnWndTitleDetection", "SCF_BanIP",0, FilePath);
	this->OnCheatAction[2][3] = GetPrivateProfileInt("OnWndTitleDetection", "SCF_Disconnect",0, FilePath);

	this->OnCheatAction[3][0] = GetPrivateProfileInt("OnAPIDetection", "SCF_BanCharacter",0, FilePath);
	this->OnCheatAction[3][1] = GetPrivateProfileInt("OnAPIDetection", "SCF_BanAccount",0, FilePath);
	this->OnCheatAction[3][2] = GetPrivateProfileInt("OnAPIDetection", "SCF_BanIP",0, FilePath);
	this->OnCheatAction[3][3] = GetPrivateProfileInt("OnAPIDetection", "SCF_Disconnect",0, FilePath);

	this->OnCheatAction[4][0] = GetPrivateProfileInt("OnSignatureDetection", "SCF_BanCharacter",0, FilePath);
	this->OnCheatAction[4][1] = GetPrivateProfileInt("OnSignatureDetection", "SCF_BanAccount",0, FilePath);
	this->OnCheatAction[4][2] = GetPrivateProfileInt("OnSignatureDetection", "SCF_BanIP",0, FilePath);
	this->OnCheatAction[4][3] = GetPrivateProfileInt("OnSignatureDetection", "SCF_Disconnect",0, FilePath);

	this->OnCheatAction[5][0] = GetPrivateProfileInt("OnInjectedModuleDetection", "SCF_BanCharacter",0, FilePath);
	this->OnCheatAction[5][1] = GetPrivateProfileInt("OnInjectedModuleDetection", "SCF_BanAccount",0, FilePath);
	this->OnCheatAction[5][2] = GetPrivateProfileInt("OnInjectedModuleDetection", "SCF_BanIP",0, FilePath);
	this->OnCheatAction[5][3] = GetPrivateProfileInt("OnInjectedModuleDetection", "SCF_Disconnect",0, FilePath);

	this->OnCheatAction[6][0] = GetPrivateProfileInt("OnSpeedHackDetection", "SCF_BanCharacter",0, FilePath);
	this->OnCheatAction[6][1] = GetPrivateProfileInt("OnSpeedHackDetection", "SCF_BanAccount",0, FilePath);
	this->OnCheatAction[6][2] = GetPrivateProfileInt("OnSpeedHackDetection", "SCF_BanIP",0, FilePath);
	this->OnCheatAction[6][3] = GetPrivateProfileInt("OnSpeedHackDetection", "SCF_Disconnect",0, FilePath);

	this->OnCheatAction[7][0] = GetPrivateProfileInt("OnThreadClosedDetection", "SCF_BanCharacter",0, FilePath);
	this->OnCheatAction[7][1] = GetPrivateProfileInt("OnThreadClosedDetection", "SCF_BanAccount",0, FilePath);
	this->OnCheatAction[7][2] = GetPrivateProfileInt("OnThreadClosedDetection", "SCF_BanIP",0, FilePath);
	this->OnCheatAction[7][3] = GetPrivateProfileInt("OnThreadClosedDetection", "SCF_Disconnect",0, FilePath);
	
	LogAddC(3, "[CrystalWall] Loaded Successfull!");
}

void cCrystalWall::SendCRC(int aIndex)
{
	if(this->UseCrystalShield == 1)
	{
		if(this->IsSendCRC == 1)
		{
			PMSG_SENDCRC pMsg = {0};
			PHeadSetB((LPBYTE)&pMsg, 0xFC, sizeof(pMsg));
			pMsg.SubCode = 0x9A;
			pMsg.SubSubCode = 0;

			if(gObj[aIndex].Connected == true)
			{
				for(int i=0;i<CRYSTALWALL_MAXCRC;i++)
				{
					pMsg.CRC[i] = this->CRC[i];
				}
			}
			DataSend(aIndex, (UCHAR*)&pMsg, pMsg.h.size);
		}
	}
}

void cCrystalWall::RecvPing(int aIndex)
{
	//if(this->UseCrystalShield == 1)
	//{
		if(this->UseTTMain == 1)
			gObj[aIndex].CrystalPingReply++;
	//}
}

void cCrystalWall::CheckPing(int aIndex)
{
	//if(this->UseCrystalShield == 1)
	//{
		if(this->UseTTMain == 1)
		{
			int val = gObj[aIndex].CrystalPing % 17;
			if ( val == 0 )
			{
				PMSG_SET_DEVILSQUARE pMsg={0};
				PHeadSetB((LPBYTE)&pMsg, 0xE0, sizeof(pMsg));
				pMsg.Type=0x99;

				DataSend(aIndex, (UCHAR*)&pMsg, pMsg.h.size);
			}
			if ( gObj[aIndex].CrystalPing > this->Crystal_PingCheckSeconds )
			{
				gObj[aIndex].CrystalPing = 0;
				if(gObj[aIndex].CrystalPingReply == 0 && this->UseCrystalShield == 1)
				{
					CW_HACK_LOG.Output("[%s][%s][%s] ClientHack Detected : Doesnt Recv Any Ping...",
							gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].Ip_addr);
					this->Action(aIndex,0);
				}else			
					gObj[aIndex].CrystalPingReply = 0;
			}
			gObj[aIndex].CrystalPing++;
		}
	//}
}

bool cCrystalWall::Action(int aIndex, BYTE Cheat)
{
	if(this->UseCrystalShield == 1)
	{
		char pId[11]={0};
		
		if(this->MsgShowToClient == 1)
			GCServerMsgStringSend("[Anti-Hack] Cheat Detected!!", aIndex, 0x01);

		if(this->OnCheatAction[Cheat][0] == 1)//Ban Char
		{
			strcpy(pId,gObj[aIndex].Name);
			if(pId[0] != 0)
			{
				GJSetStatusBan(pId,0,1);
				return true;
			}
		}
		if(this->OnCheatAction[Cheat][1] == 1)//Ban Account
		{
			strcpy(pId,gObj[aIndex].AccountID);
			if(pId[0] != 0)
			{
				GJSetStatusBan(pId,1,1);
				return true;
			}
		}
		if(this->OnCheatAction[Cheat][2] == 1)//Ban IP
		{
			AntiFlood.AddToBlackList(gObj[aIndex].Ip_addr);
			return true;
		}
		if(this->OnCheatAction[Cheat][3] == 1)//Disconnect
		{
			CloseClient(aIndex);
			return true;
		}
	}

	return false;
}

void cCrystalWall::ProtocolCore(int aIndex, BYTE * aRecv)
{
	if(this->UseCrystalShield == 1)
	{
		char szAccount[11]={0};
		char szName[11]={0};
		char szIP[16]={0};

		if(gObj[aIndex].AccountID[0] != 0)
			strcpy(szAccount,gObj[aIndex].AccountID);

		if(gObj[aIndex].Name[0] != 0)
			strcpy(szName,gObj[aIndex].Name);

		if(szIP[0] != 0)
			strcpy(szIP,gObj[aIndex].Ip_addr);

		if(aRecv[4] == 0)
		{
			PMSG_SENDCRC * msg = (PMSG_SENDCRC *)aRecv;
			for(int i=0;i<CRYSTALWALL_MAXCRC;i++)
			{
				if(msg->CRC[i] == 1)
				{
					if(this->Action(aIndex,1))
						CW_HACK_LOG.Output("[%s][%s][%s] ClientHack Detected : BAD CRC32 In File [%d] - Server => Client",
							szAccount, szName, szIP,
							i);
				}
			}
		}else
		{
			PMSG_SENDCHEATDATA * msg = (PMSG_SENDCHEATDATA *)aRecv;
			switch(msg->SubSubCode)
			{
				case 1:
				{
					if(this->Action(aIndex,msg->SubSubCode))
						CW_HACK_LOG.Output("[%s][%s][%s] ClientHack Detected : BAD CRC32 In File [%d]",
							szAccount, szName, szIP,
							msg->Extra);
				}break;
				case 2:
				{
					if(this->Action(aIndex,msg->SubSubCode))
						CW_HACK_LOG.Output("[%s][%s][%s] ClientHack Detected : Hack Window Detected [%d]",
							szAccount, szName, szIP,
							msg->Extra);
				}break;
				case 3:
				{
					if(this->Action(aIndex,msg->SubSubCode))
						CW_HACK_LOG.Output("[%s][%s][%s] ClientHack Detected : API Hooked [%d]",
							szAccount, szName, szIP,
							msg->Extra);
				}break;
				case 4:
				{
					if(this->Action(aIndex,msg->SubSubCode))
						CW_HACK_LOG.Output("[%s][%s][%s] ClientHack Detected : Hack Process Detected by Signature [%d]",
							szAccount, szName, szIP,
							msg->Extra);
				}break;
				case 5:
				{
					if(this->Action(aIndex,msg->SubSubCode))
						CW_HACK_LOG.Output("[%s][%s][%s] ClientHack Detected : Injected Hack Module Detected [%d]",
							szAccount, szName, szIP,
							msg->Extra);
				}break;
				case 7:
				{
					if(this->Action(aIndex,msg->SubSubCode))
						CW_HACK_LOG.Output("[%s][%s][%s] ClientHack Detected : Closed Thread",
							szAccount, szName, szIP);
				}break;
			};
		}
	}
}

#endif
#endif