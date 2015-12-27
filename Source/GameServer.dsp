# Microsoft Developer Studio Project File - Name="GameServer" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=GameServer - Win32 DebugRelease
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "GameServer.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "GameServer.mak" CFG="GameServer - Win32 DebugRelease"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "GameServer - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "GameServer - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "GameServer - Win32 DebugRelease" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "GameServer - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x280a /d "NDEBUG"
# ADD RSC /l 0x280a /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x280a /d "_DEBUG"
# ADD RSC /l 0x280a /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "GameServer___Win32_DebugRelease"
# PROP BASE Intermediate_Dir "GameServer___Win32_DebugRelease"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "GameServer___Win32_DebugRelease"
# PROP Intermediate_Dir "GameServer___Win32_DebugRelease"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /GX /ZI /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /FAcs /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x280a /d "_DEBUG"
# ADD RSC /l 0x280a /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /map /debug /machine:I386 /out:"D:\MuServer\GameServer99.60\GameServer.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "GameServer - Win32 Release"
# Name "GameServer - Win32 Debug"
# Name "GameServer - Win32 DebugRelease"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AcceptIp.cpp
# End Source File
# Begin Source File

SOURCE=.\AttackEvent.cpp
# End Source File
# Begin Source File

SOURCE=.\BattleGround.cpp
# End Source File
# Begin Source File

SOURCE=.\BattleSoccer.cpp
# End Source File
# Begin Source File

SOURCE=.\BattleSoccerManager.cpp
# End Source File
# Begin Source File

SOURCE=.\BloodCastle.cpp
# End Source File
# Begin Source File

SOURCE=.\CallStackTrace.cpp
# End Source File
# Begin Source File

SOURCE=.\CannonTower.cpp
# End Source File
# Begin Source File

SOURCE=.\CashItemPeriodSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\CashLotterySystem.cpp
# End Source File
# Begin Source File

SOURCE=.\CashShop.cpp
# End Source File
# Begin Source File

SOURCE=.\CastleCrown.cpp
# End Source File
# Begin Source File

SOURCE=.\CastleCrownSwitch.cpp
# End Source File
# Begin Source File

SOURCE=.\CastleDeepEvent.cpp
# End Source File
# Begin Source File

SOURCE=.\CastleSiege.cpp
# End Source File
# Begin Source File

SOURCE=.\CastleSiegeSync.cpp
# End Source File
# Begin Source File

SOURCE=.\CCastleEventItemList.cpp
# End Source File
# Begin Source File

SOURCE=.\ChaosBox.cpp
# End Source File
# Begin Source File

SOURCE=.\ChaosCastle.cpp
# End Source File
# Begin Source File

SOURCE=.\ChaosCastleSetItemDrop.cpp
# End Source File
# Begin Source File

SOURCE=.\ChinaHackUserKick.cpp
# End Source File
# Begin Source File

SOURCE=..\common\classdef.cpp
# End Source File
# Begin Source File

SOURCE=.\CLoginCount.cpp
# End Source File
# Begin Source File

SOURCE=.\ClosePlayer.cpp
# End Source File
# Begin Source File

SOURCE=.\ComboAttack.cpp
# End Source File
# Begin Source File

SOURCE=.\ConMember.cpp
# End Source File
# Begin Source File

SOURCE=.\Crywolf.cpp
# End Source File
# Begin Source File

SOURCE=.\CrywolfAltar.cpp
# End Source File
# Begin Source File

SOURCE=.\CrywolfDarkElf.cpp
# End Source File
# Begin Source File

SOURCE=.\CrywolfMonster.cpp
# End Source File
# Begin Source File

SOURCE=.\CrywolfStateTimeInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\CrywolfStatue.cpp
# End Source File
# Begin Source File

SOURCE=.\CrywolfSync.cpp
# End Source File
# Begin Source File

SOURCE=.\CrywolfTanker.cpp
# End Source File
# Begin Source File

SOURCE=.\CrywolfUtil.cpp
# End Source File
# Begin Source File

SOURCE=.\CWhatsUpDummyServer.cpp
# End Source File
# Begin Source File

SOURCE=.\DarkSpirit.cpp
# End Source File
# Begin Source File

SOURCE=.\DbSave.cpp
# End Source File
# Begin Source File

SOURCE=.\DBSockMng.cpp
# End Source File
# Begin Source File

SOURCE=.\DevilSquare.cpp
# End Source File
# Begin Source File

SOURCE=.\DevilSquareGround.cpp
# End Source File
# Begin Source File

SOURCE=.\DirPath.cpp
# End Source File
# Begin Source File

SOURCE=.\DragonEvent.cpp
# End Source File
# Begin Source File

SOURCE=.\DSProtocol.cpp
# End Source File
# Begin Source File

SOURCE=.\EDSProtocol.cpp
# End Source File
# Begin Source File

SOURCE=.\EledoradoEvent.cpp
# End Source File
# Begin Source File

SOURCE=.\Event.cpp
# End Source File
# Begin Source File

SOURCE=.\EventManagement.cpp
# End Source File
# Begin Source File

SOURCE=.\GameEvent.cpp
# End Source File
# Begin Source File

SOURCE=.\GameMain.cpp
# End Source File
# Begin Source File

SOURCE=.\GameServer.cpp
# End Source File
# Begin Source File

SOURCE=.\GameServer.rc
# End Source File
# Begin Source File

SOURCE=.\GameServerAuth.cpp
# End Source File
# Begin Source File

SOURCE=.\Gate.cpp
# End Source File
# Begin Source File

SOURCE=.\giocp.cpp
# End Source File
# Begin Source File

SOURCE=.\GMMng.cpp
# End Source File
# Begin Source File

SOURCE=.\gObjMonster.cpp
# End Source File
# Begin Source File

SOURCE=.\Guardian.cpp
# End Source File
# Begin Source File

SOURCE=.\GuardianStatue.cpp
# End Source File
# Begin Source File

SOURCE=.\GuildClass.cpp
# End Source File
# Begin Source File

SOURCE=.\IpCache.cpp
# End Source File
# Begin Source File

SOURCE=.\ItemAddOption.cpp
# End Source File
# Begin Source File

SOURCE=.\ItemBag.cpp
# End Source File
# Begin Source File

SOURCE=.\ItemBagEx.cpp
# End Source File
# Begin Source File

SOURCE=.\ItemSystemFor380.cpp
# End Source File
# Begin Source File

SOURCE=.\JewelMixSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\JewelOfHarmonySystem.cpp
# End Source File
# Begin Source File

SOURCE=.\KalimaGate.cpp
# End Source File
# Begin Source File

SOURCE=.\Kanturu.cpp
# End Source File
# Begin Source File

SOURCE=.\KanturuBattleOfMaya.cpp
# End Source File
# Begin Source File

SOURCE=.\KanturuBattleOfNightmare.cpp
# End Source File
# Begin Source File

SOURCE=.\KanturuBattleStanby.cpp
# End Source File
# Begin Source File

SOURCE=.\KanturuBattleUserMng.cpp
# End Source File
# Begin Source File

SOURCE=.\KanturuEntranceNPC.cpp
# End Source File
# Begin Source File

SOURCE=.\KanturuMaya.cpp
# End Source File
# Begin Source File

SOURCE=.\KanturuMonsterMng.cpp
# End Source File
# Begin Source File

SOURCE=.\KanturuNightmare.cpp
# End Source File
# Begin Source File

SOURCE=.\KanturuStateInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\KanturuTowerOfRefinement.cpp
# End Source File
# Begin Source File

SOURCE=.\KanturuUtil.cpp
# End Source File
# Begin Source File

SOURCE=.\LargeRand.cpp
# End Source File
# Begin Source File

SOURCE=.\LifeStone.cpp
# End Source File
# Begin Source File

SOURCE=.\LogProc.cpp
# End Source File
# Begin Source File

SOURCE=.\LogToFile.cpp
# End Source File
# Begin Source File

SOURCE=.\MagicDamage.cpp
# End Source File
# Begin Source File

SOURCE=.\MagicInf.cpp
# End Source File
# Begin Source File

SOURCE=.\MapClass.cpp
# End Source File
# Begin Source File

SOURCE=.\MapItem.cpp
# End Source File
# Begin Source File

SOURCE=.\MapServerManager.cpp
# End Source File
# Begin Source File

SOURCE=.\Mercenary.cpp
# End Source File
# Begin Source File

SOURCE=.\MonsterAttr.cpp
# End Source File
# Begin Source File

SOURCE=.\MonsterHerd.cpp
# End Source File
# Begin Source File

SOURCE=.\MonsterItemMng.cpp
# End Source File
# Begin Source File

SOURCE=.\MonsterSetBase.cpp
# End Source File
# Begin Source File

SOURCE=.\MoveCheck.cpp
# End Source File
# Begin Source File

SOURCE=.\MoveCommand.cpp
# End Source File
# Begin Source File

SOURCE=.\MultiAttackHackCheck.cpp
# End Source File
# Begin Source File

SOURCE=.\MultiCheckSum.cpp
# End Source File
# Begin Source File

SOURCE=..\common\MyWinsockBase.cpp
# End Source File
# Begin Source File

SOURCE=.\NpcTalk.cpp
# End Source File
# Begin Source File

SOURCE=.\NSerialCheck.cpp
# End Source File
# Begin Source File

SOURCE=.\ObjAttack.cpp
# End Source File
# Begin Source File

SOURCE=.\ObjBaseAttack.cpp
# End Source File
# Begin Source File

SOURCE=.\ObjCalCharacter.cpp
# End Source File
# Begin Source File

SOURCE=.\ObjUseSkill.cpp
# End Source File
# Begin Source File

SOURCE=.\PacketCheckSum.cpp
# End Source File
# Begin Source File

SOURCE=.\PartyClass.cpp
# End Source File
# Begin Source File

SOURCE=.\protocol.cpp
# End Source File
# Begin Source File

SOURCE=.\QuestInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\RingAttackEvent.cpp
# End Source File
# Begin Source File

SOURCE=.\SendHackLog.cpp
# End Source File
# Begin Source File

SOURCE=.\SetItemMacro.cpp
# End Source File
# Begin Source File

SOURCE=..\common\SetItemOption.cpp
# End Source File
# Begin Source File

SOURCE=.\Shop.cpp
# End Source File
# Begin Source File

SOURCE=.\SkillAdditionInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\SkillDelay.cpp
# End Source File
# Begin Source File

SOURCE=.\SkillHitBox.cpp
# End Source File
# Begin Source File

SOURCE=.\SProtocol.cpp
# End Source File
# Begin Source File

SOURCE=.\StatMng.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TDurMagicKeyChecker.cpp
# End Source File
# Begin Source File

SOURCE=.\TMonsterAI.cpp
# End Source File
# Begin Source File

SOURCE=.\TMonsterAIAgro.cpp
# End Source File
# Begin Source File

SOURCE=.\TMonsterAIAutomata.cpp
# End Source File
# Begin Source File

SOURCE=.\TMonsterAIElement.cpp
# End Source File
# Begin Source File

SOURCE=.\TMonsterAIGroup.cpp
# End Source File
# Begin Source File

SOURCE=.\TMonsterAIGroupMember.cpp
# End Source File
# Begin Source File

SOURCE=.\TMonsterAIMovePath.cpp
# End Source File
# Begin Source File

SOURCE=.\TMonsterAIRule.cpp
# End Source File
# Begin Source File

SOURCE=.\TMonsterAIRuleInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\TMonsterAIUnit.cpp
# End Source File
# Begin Source File

SOURCE=.\TMonsterAIUtil.cpp
# End Source File
# Begin Source File

SOURCE=.\TMonsterSkillElement.cpp
# End Source File
# Begin Source File

SOURCE=.\TMonsterSkillElementInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\TMonsterSkillManager.cpp
# End Source File
# Begin Source File

SOURCE=.\TMonsterSkillUnit.cpp
# End Source File
# Begin Source File

SOURCE=.\TNotice.cpp
# End Source File
# Begin Source File

SOURCE=.\TRandomPoolMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\TServerAlertManager.cpp
# End Source File
# Begin Source File

SOURCE=.\TServerInfoDisplayer.cpp
# End Source File
# Begin Source File

SOURCE=.\TStatistics.cpp
# End Source File
# Begin Source File

SOURCE=..\common\TSync.cpp
# End Source File
# Begin Source File

SOURCE=.\TUnion.cpp
# End Source File
# Begin Source File

SOURCE=.\TUnionInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\user.cpp
# End Source File
# Begin Source File

SOURCE=.\ViewportGuild.cpp
# End Source File
# Begin Source File

SOURCE=.\Weapon.cpp
# End Source File
# Begin Source File

SOURCE=.\WhisperCash.cpp
# End Source File
# Begin Source File

SOURCE=..\common\winutil.cpp
# End Source File
# Begin Source File

SOURCE=.\wsGameServer.cpp
# End Source File
# Begin Source File

SOURCE=.\wsJoinServerCli.cpp
# End Source File
# Begin Source File

SOURCE=.\wsShopServerCli.cpp
# End Source File
# Begin Source File

SOURCE=.\WTEventItemList.cpp
# End Source File
# Begin Source File

SOURCE=..\common\WzMemScript.cpp
# End Source File
# Begin Source File

SOURCE=.\WzMultiCastSock.cpp
# End Source File
# Begin Source File

SOURCE=.\WzQueue.cpp
# End Source File
# Begin Source File

SOURCE=..\common\WZScriptEncode.cpp
# End Source File
# Begin Source File

SOURCE=.\WzUdp.cpp
# End Source File
# Begin Source File

SOURCE=..\common\zzzitem.cpp
# End Source File
# Begin Source File

SOURCE=.\zzzmathlib.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AcceptIp.h
# End Source File
# Begin Source File

SOURCE=.\AttackEvent.h
# End Source File
# Begin Source File

SOURCE=.\BattleGround.h
# End Source File
# Begin Source File

SOURCE=.\BattleSoccer.h
# End Source File
# Begin Source File

SOURCE=.\BattleSoccerManager.h
# End Source File
# Begin Source File

SOURCE=.\BloodCastle.h
# End Source File
# Begin Source File

SOURCE=.\CallStackTrace.h
# End Source File
# Begin Source File

SOURCE=.\CannonTower.h
# End Source File
# Begin Source File

SOURCE=.\CashItemPeriodSystem.h
# End Source File
# Begin Source File

SOURCE=.\CashLotterySystem.h
# End Source File
# Begin Source File

SOURCE=.\CashShop.h
# End Source File
# Begin Source File

SOURCE=.\CastleCrown.h
# End Source File
# Begin Source File

SOURCE=.\CastleCrownSwitch.h
# End Source File
# Begin Source File

SOURCE=.\CastleDeepEvent.h
# End Source File
# Begin Source File

SOURCE=.\CastleSiege.h
# End Source File
# Begin Source File

SOURCE=.\CastleSiegeSync.h
# End Source File
# Begin Source File

SOURCE=.\CCastleEventItemList.h
# End Source File
# Begin Source File

SOURCE=.\ChaosBox.h
# End Source File
# Begin Source File

SOURCE=.\ChaosCastle.h
# End Source File
# Begin Source File

SOURCE=.\ChaosCastleSetItemDrop.h
# End Source File
# Begin Source File

SOURCE=.\ChinaHackUserKick.h
# End Source File
# Begin Source File

SOURCE=..\common\classdef.h
# End Source File
# Begin Source File

SOURCE=.\CLoginCount.h
# End Source File
# Begin Source File

SOURCE=.\ClosePlayer.h
# End Source File
# Begin Source File

SOURCE=.\ComboAttack.h
# End Source File
# Begin Source File

SOURCE=.\ConMember.h
# End Source File
# Begin Source File

SOURCE=.\Crywolf.h
# End Source File
# Begin Source File

SOURCE=.\CrywolfAltar.h
# End Source File
# Begin Source File

SOURCE=.\CrywolfDarkElf.h
# End Source File
# Begin Source File

SOURCE=.\CrywolfMonster.h
# End Source File
# Begin Source File

SOURCE=.\CrywolfObjInfo.h
# End Source File
# Begin Source File

SOURCE=.\CrywolfStateTimeInfo.h
# End Source File
# Begin Source File

SOURCE=.\CrywolfStatue.h
# End Source File
# Begin Source File

SOURCE=.\CrywolfSync.h
# End Source File
# Begin Source File

SOURCE=.\CrywolfTanker.h
# End Source File
# Begin Source File

SOURCE=.\CrywolfUtil.h
# End Source File
# Begin Source File

SOURCE=.\CWhatsUpDummyServer.h
# End Source File
# Begin Source File

SOURCE=.\DarkSpirit.h
# End Source File
# Begin Source File

SOURCE=.\DbSave.h
# End Source File
# Begin Source File

SOURCE=.\DBSockMng.h
# End Source File
# Begin Source File

SOURCE=.\DevilSquare.h
# End Source File
# Begin Source File

SOURCE=.\DevilSquareGround.h
# End Source File
# Begin Source File

SOURCE=.\dirpath.h
# End Source File
# Begin Source File

SOURCE=.\DragonEvent.h
# End Source File
# Begin Source File

SOURCE=.\DSProtocol.h
# End Source File
# Begin Source File

SOURCE=.\EDSProtocol.h
# End Source File
# Begin Source File

SOURCE=.\EledoradoEvent.h
# End Source File
# Begin Source File

SOURCE=.\Event.h
# End Source File
# Begin Source File

SOURCE=.\EventManagement.h
# End Source File
# Begin Source File

SOURCE=.\GameEvent.h
# End Source File
# Begin Source File

SOURCE=.\GameMain.h
# End Source File
# Begin Source File

SOURCE=.\GameServer.h
# End Source File
# Begin Source File

SOURCE=.\GameServerAuth.h
# End Source File
# Begin Source File

SOURCE=.\Gate.h
# End Source File
# Begin Source File

SOURCE=.\giocp.h
# End Source File
# Begin Source File

SOURCE=.\GMMng.h
# End Source File
# Begin Source File

SOURCE=.\gObjMonster.h
# End Source File
# Begin Source File

SOURCE=.\Guardian.h
# End Source File
# Begin Source File

SOURCE=.\GuardianStatue.h
# End Source File
# Begin Source File

SOURCE=.\GuildClass.h
# End Source File
# Begin Source File

SOURCE=.\IpCache.h
# End Source File
# Begin Source File

SOURCE=.\ItemAddOption.h
# End Source File
# Begin Source File

SOURCE=.\ItemBag.h
# End Source File
# Begin Source File

SOURCE=.\ItemBagEx.h
# End Source File
# Begin Source File

SOURCE=.\ItemSystemFor380.h
# End Source File
# Begin Source File

SOURCE=.\JewelMixSystem.h
# End Source File
# Begin Source File

SOURCE=.\JewelOfHarmonySystem.h
# End Source File
# Begin Source File

SOURCE=.\KalimaGate.h
# End Source File
# Begin Source File

SOURCE=.\Kanturu.h
# End Source File
# Begin Source File

SOURCE=.\KanturuBattleOfMaya.h
# End Source File
# Begin Source File

SOURCE=.\KanturuBattleOfNightmare.h
# End Source File
# Begin Source File

SOURCE=.\KanturuBattleStanby.h
# End Source File
# Begin Source File

SOURCE=.\KanturuBattleUser.h
# End Source File
# Begin Source File

SOURCE=.\KanturuBattleUserMng.h
# End Source File
# Begin Source File

SOURCE=.\KanturuEntranceNPC.h
# End Source File
# Begin Source File

SOURCE=.\KanturuMaya.h
# End Source File
# Begin Source File

SOURCE=.\KanturuMonsterMng.h
# End Source File
# Begin Source File

SOURCE=.\KanturuNightmare.h
# End Source File
# Begin Source File

SOURCE=.\KanturuObjInfo.h
# End Source File
# Begin Source File

SOURCE=.\KanturuStateInfo.h
# End Source File
# Begin Source File

SOURCE=.\KanturuTowerOfRefinement.h
# End Source File
# Begin Source File

SOURCE=.\KanturuUtil.h
# End Source File
# Begin Source File

SOURCE=.\LargeRand.h
# End Source File
# Begin Source File

SOURCE=.\LifeStone.h
# End Source File
# Begin Source File

SOURCE=.\logproc.h
# End Source File
# Begin Source File

SOURCE=.\LogToFile.h
# End Source File
# Begin Source File

SOURCE=.\MagicDamage.h
# End Source File
# Begin Source File

SOURCE=.\MagicInf.h
# End Source File
# Begin Source File

SOURCE=.\MapClass.h
# End Source File
# Begin Source File

SOURCE=.\MapItem.h
# End Source File
# Begin Source File

SOURCE=.\MapServerManager.h
# End Source File
# Begin Source File

SOURCE=.\Mercenary.h
# End Source File
# Begin Source File

SOURCE=.\MonsterAttr.h
# End Source File
# Begin Source File

SOURCE=.\MonsterHerd.h
# End Source File
# Begin Source File

SOURCE=.\MonsterItemMng.h
# End Source File
# Begin Source File

SOURCE=.\MonsterSetBase.h
# End Source File
# Begin Source File

SOURCE=.\MoveCheck.h
# End Source File
# Begin Source File

SOURCE=.\MoveCommand.h
# End Source File
# Begin Source File

SOURCE=.\MultiAttackHackCheck.h
# End Source File
# Begin Source File

SOURCE=.\MultiCheckSum.h
# End Source File
# Begin Source File

SOURCE=..\common\MyWinsockBase.h
# End Source File
# Begin Source File

SOURCE=.\NpcTalk.h
# End Source File
# Begin Source File

SOURCE=.\NSerialCheck.h
# End Source File
# Begin Source File

SOURCE=.\ObjAttack.h
# End Source File
# Begin Source File

SOURCE=.\ObjBaseAttack.h
# End Source File
# Begin Source File

SOURCE=.\ObjCalCharacter.h
# End Source File
# Begin Source File

SOURCE=.\ObjUseSkill.h
# End Source File
# Begin Source File

SOURCE=.\PacketCheckSum.h
# End Source File
# Begin Source File

SOURCE=.\PartyClass.h
# End Source File
# Begin Source File

SOURCE=.\PCSProtocol.h
# End Source File
# Begin Source File

SOURCE=..\include\prodef.h
# End Source File
# Begin Source File

SOURCE=.\protocol.h
# End Source File
# Begin Source File

SOURCE=.\QuestInfo.h
# End Source File
# Begin Source File

SOURCE=..\include\Readscript.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\RingAttackEvent.h
# End Source File
# Begin Source File

SOURCE=.\SendHackLog.h
# End Source File
# Begin Source File

SOURCE=.\SetItemMacro.h
# End Source File
# Begin Source File

SOURCE=..\common\SetItemOption.h
# End Source File
# Begin Source File

SOURCE=.\Shop.h
# End Source File
# Begin Source File

SOURCE=.\SkillAdditionInfo.h
# End Source File
# Begin Source File

SOURCE=.\SkillDelay.h
# End Source File
# Begin Source File

SOURCE=.\SkillHitBox.h
# End Source File
# Begin Source File

SOURCE=.\spe.h
# End Source File
# Begin Source File

SOURCE=.\SProtocol.h
# End Source File
# Begin Source File

SOURCE=.\StatMng.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TDurMagicKeyChecker.h
# End Source File
# Begin Source File

SOURCE=.\TMonsterAI.h
# End Source File
# Begin Source File

SOURCE=.\TMonsterAIAgro.h
# End Source File
# Begin Source File

SOURCE=.\TMonsterAIAutomata.h
# End Source File
# Begin Source File

SOURCE=.\TMonsterAIElement.h
# End Source File
# Begin Source File

SOURCE=.\TMonsterAIGroup.h
# End Source File
# Begin Source File

SOURCE=.\TMonsterAIGroupMember.h
# End Source File
# Begin Source File

SOURCE=.\TMonsterAIMovePath.h
# End Source File
# Begin Source File

SOURCE=.\TMonsterAIRule.h
# End Source File
# Begin Source File

SOURCE=.\TMonsterAIRuleInfo.h
# End Source File
# Begin Source File

SOURCE=.\TMonsterAIState.h
# End Source File
# Begin Source File

SOURCE=.\TMonsterAIUnit.h
# End Source File
# Begin Source File

SOURCE=.\TMonsterAIUtil.h
# End Source File
# Begin Source File

SOURCE=.\TMonsterSkillElement.h
# End Source File
# Begin Source File

SOURCE=.\TMonsterSkillElementInfo.h
# End Source File
# Begin Source File

SOURCE=.\TMonsterSkillInfo.h
# End Source File
# Begin Source File

SOURCE=.\TMonsterSkillManager.h
# End Source File
# Begin Source File

SOURCE=.\TMonsterSkillUnit.h
# End Source File
# Begin Source File

SOURCE=.\TNotice.h
# End Source File
# Begin Source File

SOURCE=.\TRandomPoolMgr.h
# End Source File
# Begin Source File

SOURCE=.\TServerAlertManager.h
# End Source File
# Begin Source File

SOURCE=.\TServerInfoDisplayer.h
# End Source File
# Begin Source File

SOURCE=.\TSkillElement.h
# End Source File
# Begin Source File

SOURCE=.\TStatistics.h
# End Source File
# Begin Source File

SOURCE=..\common\TSync.h
# End Source File
# Begin Source File

SOURCE=.\TUnion.h
# End Source File
# Begin Source File

SOURCE=.\TUnionInfo.h
# End Source File
# Begin Source File

SOURCE=.\user.h
# End Source File
# Begin Source File

SOURCE=.\ViewportGuild.h
# End Source File
# Begin Source File

SOURCE=.\Weapon.h
# End Source File
# Begin Source File

SOURCE=.\WhisperCash.h
# End Source File
# Begin Source File

SOURCE=..\common\winutil.h
# End Source File
# Begin Source File

SOURCE=.\wsGameServer.h
# End Source File
# Begin Source File

SOURCE=.\wsJoinServerCli.h
# End Source File
# Begin Source File

SOURCE=.\wsShopServerCli.h
# End Source File
# Begin Source File

SOURCE=.\WTEventItemList.h
# End Source File
# Begin Source File

SOURCE=..\common\WzMemScript.h
# End Source File
# Begin Source File

SOURCE=.\WzMultiCastSock.h
# End Source File
# Begin Source File

SOURCE=.\WzQueue.h
# End Source File
# Begin Source File

SOURCE=..\common\WZScriptEncode.h
# End Source File
# Begin Source File

SOURCE=.\WzUdp.h
# End Source File
# Begin Source File

SOURCE=..\common\zzzitem.h
# End Source File
# Begin Source File

SOURCE=.\zzzmathlib.h
# End Source File
# Begin Source File

SOURCE=..\common\zzzpath.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\bitmap.bmp
# End Source File
# Begin Source File

SOURCE=.\GameServer.ico
# End Source File
# Begin Source File

SOURCE=.\small.ico
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\MuMsg\Release\MuMsg.lib
# End Source File
# Begin Source File

SOURCE=..\SimpleModulus\Release\SimpleModulus.lib
# End Source File
# Begin Source File

SOURCE=..\GGSvr\Release\GGSvr.lib
# End Source File
# End Target
# End Project
