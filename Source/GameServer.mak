# Microsoft Developer Studio Generated NMAKE File, Based on GameServer.dsp
!IF "$(CFG)" == ""
CFG=GameServer - Win32 DebugRelease
!MESSAGE No configuration specified. Defaulting to GameServer - Win32 DebugRelease.
!ENDIF 

!IF "$(CFG)" != "GameServer - Win32 Release" && "$(CFG)" != "GameServer - Win32 Debug" && "$(CFG)" != "GameServer - Win32 DebugRelease"
!MESSAGE Invalid configuration "$(CFG)" specified.
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
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "GameServer - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\GameServer.exe"


CLEAN :
	-@erase "$(INTDIR)\AcceptIp.obj"
	-@erase "$(INTDIR)\AttackEvent.obj"
	-@erase "$(INTDIR)\BattleGround.obj"
	-@erase "$(INTDIR)\BattleSoccer.obj"
	-@erase "$(INTDIR)\BattleSoccerManager.obj"
	-@erase "$(INTDIR)\BloodCastle.obj"
	-@erase "$(INTDIR)\CallStackTrace.obj"
	-@erase "$(INTDIR)\CannonTower.obj"
	-@erase "$(INTDIR)\CashItemPeriodSystem.obj"
	-@erase "$(INTDIR)\CashLotterySystem.obj"
	-@erase "$(INTDIR)\CashShop.obj"
	-@erase "$(INTDIR)\CastleCrown.obj"
	-@erase "$(INTDIR)\CastleCrownSwitch.obj"
	-@erase "$(INTDIR)\CastleDeepEvent.obj"
	-@erase "$(INTDIR)\CastleSiege.obj"
	-@erase "$(INTDIR)\CastleSiegeSync.obj"
	-@erase "$(INTDIR)\CCastleEventItemList.obj"
	-@erase "$(INTDIR)\ChaosBox.obj"
	-@erase "$(INTDIR)\ChaosCastle.obj"
	-@erase "$(INTDIR)\ChaosCastleSetItemDrop.obj"
	-@erase "$(INTDIR)\ChinaHackUserKick.obj"
	-@erase "$(INTDIR)\classdef.obj"
	-@erase "$(INTDIR)\CLoginCount.obj"
	-@erase "$(INTDIR)\ClosePlayer.obj"
	-@erase "$(INTDIR)\ComboAttack.obj"
	-@erase "$(INTDIR)\ConMember.obj"
	-@erase "$(INTDIR)\Crywolf.obj"
	-@erase "$(INTDIR)\CrywolfAltar.obj"
	-@erase "$(INTDIR)\CrywolfDarkElf.obj"
	-@erase "$(INTDIR)\CrywolfMonster.obj"
	-@erase "$(INTDIR)\CrywolfStateTimeInfo.obj"
	-@erase "$(INTDIR)\CrywolfStatue.obj"
	-@erase "$(INTDIR)\CrywolfSync.obj"
	-@erase "$(INTDIR)\CrywolfTanker.obj"
	-@erase "$(INTDIR)\CrywolfUtil.obj"
	-@erase "$(INTDIR)\CWhatsUpDummyServer.obj"
	-@erase "$(INTDIR)\DarkSpirit.obj"
	-@erase "$(INTDIR)\DbSave.obj"
	-@erase "$(INTDIR)\DBSockMng.obj"
	-@erase "$(INTDIR)\DevilSquare.obj"
	-@erase "$(INTDIR)\DevilSquareGround.obj"
	-@erase "$(INTDIR)\DirPath.obj"
	-@erase "$(INTDIR)\DragonEvent.obj"
	-@erase "$(INTDIR)\DSProtocol.obj"
	-@erase "$(INTDIR)\EDSProtocol.obj"
	-@erase "$(INTDIR)\EledoradoEvent.obj"
	-@erase "$(INTDIR)\Event.obj"
	-@erase "$(INTDIR)\EventManagement.obj"
	-@erase "$(INTDIR)\GameEvent.obj"
	-@erase "$(INTDIR)\GameMain.obj"
	-@erase "$(INTDIR)\GameServer.obj"
	-@erase "$(INTDIR)\GameServer.pch"
	-@erase "$(INTDIR)\GameServer.res"
	-@erase "$(INTDIR)\GameServerAuth.obj"
	-@erase "$(INTDIR)\Gate.obj"
	-@erase "$(INTDIR)\giocp.obj"
	-@erase "$(INTDIR)\GMMng.obj"
	-@erase "$(INTDIR)\gObjMonster.obj"
	-@erase "$(INTDIR)\Guardian.obj"
	-@erase "$(INTDIR)\GuardianStatue.obj"
	-@erase "$(INTDIR)\GuildClass.obj"
	-@erase "$(INTDIR)\IpCache.obj"
	-@erase "$(INTDIR)\ItemAddOption.obj"
	-@erase "$(INTDIR)\ItemBag.obj"
	-@erase "$(INTDIR)\ItemBagEx.obj"
	-@erase "$(INTDIR)\ItemSystemFor380.obj"
	-@erase "$(INTDIR)\JewelMixSystem.obj"
	-@erase "$(INTDIR)\JewelOfHarmonySystem.obj"
	-@erase "$(INTDIR)\KalimaGate.obj"
	-@erase "$(INTDIR)\Kanturu.obj"
	-@erase "$(INTDIR)\KanturuBattleOfMaya.obj"
	-@erase "$(INTDIR)\KanturuBattleOfNightmare.obj"
	-@erase "$(INTDIR)\KanturuBattleStanby.obj"
	-@erase "$(INTDIR)\KanturuBattleUserMng.obj"
	-@erase "$(INTDIR)\KanturuEntranceNPC.obj"
	-@erase "$(INTDIR)\KanturuMaya.obj"
	-@erase "$(INTDIR)\KanturuMonsterMng.obj"
	-@erase "$(INTDIR)\KanturuNightmare.obj"
	-@erase "$(INTDIR)\KanturuStateInfo.obj"
	-@erase "$(INTDIR)\KanturuTowerOfRefinement.obj"
	-@erase "$(INTDIR)\KanturuUtil.obj"
	-@erase "$(INTDIR)\LargeRand.obj"
	-@erase "$(INTDIR)\LifeStone.obj"
	-@erase "$(INTDIR)\LogProc.obj"
	-@erase "$(INTDIR)\LogToFile.obj"
	-@erase "$(INTDIR)\MagicDamage.obj"
	-@erase "$(INTDIR)\MagicInf.obj"
	-@erase "$(INTDIR)\MapClass.obj"
	-@erase "$(INTDIR)\MapItem.obj"
	-@erase "$(INTDIR)\MapServerManager.obj"
	-@erase "$(INTDIR)\Mercenary.obj"
	-@erase "$(INTDIR)\MonsterAttr.obj"
	-@erase "$(INTDIR)\MonsterHerd.obj"
	-@erase "$(INTDIR)\MonsterItemMng.obj"
	-@erase "$(INTDIR)\MonsterSetBase.obj"
	-@erase "$(INTDIR)\MoveCheck.obj"
	-@erase "$(INTDIR)\MoveCommand.obj"
	-@erase "$(INTDIR)\MultiAttackHackCheck.obj"
	-@erase "$(INTDIR)\MultiCheckSum.obj"
	-@erase "$(INTDIR)\MyWinsockBase.obj"
	-@erase "$(INTDIR)\NpcTalk.obj"
	-@erase "$(INTDIR)\NSerialCheck.obj"
	-@erase "$(INTDIR)\ObjAttack.obj"
	-@erase "$(INTDIR)\ObjBaseAttack.obj"
	-@erase "$(INTDIR)\ObjCalCharacter.obj"
	-@erase "$(INTDIR)\ObjUseSkill.obj"
	-@erase "$(INTDIR)\PacketCheckSum.obj"
	-@erase "$(INTDIR)\PartyClass.obj"
	-@erase "$(INTDIR)\protocol.obj"
	-@erase "$(INTDIR)\QuestInfo.obj"
	-@erase "$(INTDIR)\RingAttackEvent.obj"
	-@erase "$(INTDIR)\SendHackLog.obj"
	-@erase "$(INTDIR)\SetItemMacro.obj"
	-@erase "$(INTDIR)\SetItemOption.obj"
	-@erase "$(INTDIR)\Shop.obj"
	-@erase "$(INTDIR)\SkillAdditionInfo.obj"
	-@erase "$(INTDIR)\SkillDelay.obj"
	-@erase "$(INTDIR)\SkillHitBox.obj"
	-@erase "$(INTDIR)\SProtocol.obj"
	-@erase "$(INTDIR)\StatMng.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\TDurMagicKeyChecker.obj"
	-@erase "$(INTDIR)\TMonsterAI.obj"
	-@erase "$(INTDIR)\TMonsterAIAgro.obj"
	-@erase "$(INTDIR)\TMonsterAIAutomata.obj"
	-@erase "$(INTDIR)\TMonsterAIElement.obj"
	-@erase "$(INTDIR)\TMonsterAIGroup.obj"
	-@erase "$(INTDIR)\TMonsterAIGroupMember.obj"
	-@erase "$(INTDIR)\TMonsterAIMovePath.obj"
	-@erase "$(INTDIR)\TMonsterAIRule.obj"
	-@erase "$(INTDIR)\TMonsterAIRuleInfo.obj"
	-@erase "$(INTDIR)\TMonsterAIUnit.obj"
	-@erase "$(INTDIR)\TMonsterAIUtil.obj"
	-@erase "$(INTDIR)\TMonsterSkillElement.obj"
	-@erase "$(INTDIR)\TMonsterSkillElementInfo.obj"
	-@erase "$(INTDIR)\TMonsterSkillManager.obj"
	-@erase "$(INTDIR)\TMonsterSkillUnit.obj"
	-@erase "$(INTDIR)\TNotice.obj"
	-@erase "$(INTDIR)\TRandomPoolMgr.obj"
	-@erase "$(INTDIR)\TServerAlertManager.obj"
	-@erase "$(INTDIR)\TServerInfoDisplayer.obj"
	-@erase "$(INTDIR)\TStatistics.obj"
	-@erase "$(INTDIR)\TSync.obj"
	-@erase "$(INTDIR)\TUnion.obj"
	-@erase "$(INTDIR)\TUnionInfo.obj"
	-@erase "$(INTDIR)\user.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\ViewportGuild.obj"
	-@erase "$(INTDIR)\Weapon.obj"
	-@erase "$(INTDIR)\WhisperCash.obj"
	-@erase "$(INTDIR)\winutil.obj"
	-@erase "$(INTDIR)\wsGameServer.obj"
	-@erase "$(INTDIR)\wsJoinServerCli.obj"
	-@erase "$(INTDIR)\wsShopServerCli.obj"
	-@erase "$(INTDIR)\WTEventItemList.obj"
	-@erase "$(INTDIR)\WzMemScript.obj"
	-@erase "$(INTDIR)\WzMultiCastSock.obj"
	-@erase "$(INTDIR)\WzQueue.obj"
	-@erase "$(INTDIR)\WZScriptEncode.obj"
	-@erase "$(INTDIR)\WzUdp.obj"
	-@erase "$(INTDIR)\zzzitem.obj"
	-@erase "$(INTDIR)\zzzmathlib.obj"
	-@erase "$(OUTDIR)\GameServer.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)\GameServer.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x280a /fo"$(INTDIR)\GameServer.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\GameServer.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\GameServer.pdb" /machine:I386 /out:"$(OUTDIR)\GameServer.exe" 
LINK32_OBJS= \
	"$(INTDIR)\AcceptIp.obj" \
	"$(INTDIR)\AttackEvent.obj" \
	"$(INTDIR)\BattleGround.obj" \
	"$(INTDIR)\BattleSoccer.obj" \
	"$(INTDIR)\BattleSoccerManager.obj" \
	"$(INTDIR)\BloodCastle.obj" \
	"$(INTDIR)\CallStackTrace.obj" \
	"$(INTDIR)\CannonTower.obj" \
	"$(INTDIR)\CashItemPeriodSystem.obj" \
	"$(INTDIR)\CashLotterySystem.obj" \
	"$(INTDIR)\CashShop.obj" \
	"$(INTDIR)\CastleCrown.obj" \
	"$(INTDIR)\CastleCrownSwitch.obj" \
	"$(INTDIR)\CastleDeepEvent.obj" \
	"$(INTDIR)\CastleSiege.obj" \
	"$(INTDIR)\CastleSiegeSync.obj" \
	"$(INTDIR)\CCastleEventItemList.obj" \
	"$(INTDIR)\ChaosBox.obj" \
	"$(INTDIR)\ChaosCastle.obj" \
	"$(INTDIR)\ChaosCastleSetItemDrop.obj" \
	"$(INTDIR)\ChinaHackUserKick.obj" \
	"$(INTDIR)\classdef.obj" \
	"$(INTDIR)\CLoginCount.obj" \
	"$(INTDIR)\ClosePlayer.obj" \
	"$(INTDIR)\ComboAttack.obj" \
	"$(INTDIR)\ConMember.obj" \
	"$(INTDIR)\Crywolf.obj" \
	"$(INTDIR)\CrywolfAltar.obj" \
	"$(INTDIR)\CrywolfDarkElf.obj" \
	"$(INTDIR)\CrywolfMonster.obj" \
	"$(INTDIR)\CrywolfStateTimeInfo.obj" \
	"$(INTDIR)\CrywolfStatue.obj" \
	"$(INTDIR)\CrywolfSync.obj" \
	"$(INTDIR)\CrywolfTanker.obj" \
	"$(INTDIR)\CrywolfUtil.obj" \
	"$(INTDIR)\CWhatsUpDummyServer.obj" \
	"$(INTDIR)\DarkSpirit.obj" \
	"$(INTDIR)\DbSave.obj" \
	"$(INTDIR)\DBSockMng.obj" \
	"$(INTDIR)\DevilSquare.obj" \
	"$(INTDIR)\DevilSquareGround.obj" \
	"$(INTDIR)\DirPath.obj" \
	"$(INTDIR)\DragonEvent.obj" \
	"$(INTDIR)\DSProtocol.obj" \
	"$(INTDIR)\EDSProtocol.obj" \
	"$(INTDIR)\EledoradoEvent.obj" \
	"$(INTDIR)\Event.obj" \
	"$(INTDIR)\EventManagement.obj" \
	"$(INTDIR)\GameEvent.obj" \
	"$(INTDIR)\GameMain.obj" \
	"$(INTDIR)\GameServer.obj" \
	"$(INTDIR)\GameServerAuth.obj" \
	"$(INTDIR)\Gate.obj" \
	"$(INTDIR)\giocp.obj" \
	"$(INTDIR)\GMMng.obj" \
	"$(INTDIR)\gObjMonster.obj" \
	"$(INTDIR)\Guardian.obj" \
	"$(INTDIR)\GuardianStatue.obj" \
	"$(INTDIR)\GuildClass.obj" \
	"$(INTDIR)\IpCache.obj" \
	"$(INTDIR)\ItemAddOption.obj" \
	"$(INTDIR)\ItemBag.obj" \
	"$(INTDIR)\ItemBagEx.obj" \
	"$(INTDIR)\ItemSystemFor380.obj" \
	"$(INTDIR)\JewelMixSystem.obj" \
	"$(INTDIR)\JewelOfHarmonySystem.obj" \
	"$(INTDIR)\KalimaGate.obj" \
	"$(INTDIR)\Kanturu.obj" \
	"$(INTDIR)\KanturuBattleOfMaya.obj" \
	"$(INTDIR)\KanturuBattleOfNightmare.obj" \
	"$(INTDIR)\KanturuBattleStanby.obj" \
	"$(INTDIR)\KanturuBattleUserMng.obj" \
	"$(INTDIR)\KanturuEntranceNPC.obj" \
	"$(INTDIR)\KanturuMaya.obj" \
	"$(INTDIR)\KanturuMonsterMng.obj" \
	"$(INTDIR)\KanturuNightmare.obj" \
	"$(INTDIR)\KanturuStateInfo.obj" \
	"$(INTDIR)\KanturuTowerOfRefinement.obj" \
	"$(INTDIR)\KanturuUtil.obj" \
	"$(INTDIR)\LargeRand.obj" \
	"$(INTDIR)\LifeStone.obj" \
	"$(INTDIR)\LogProc.obj" \
	"$(INTDIR)\LogToFile.obj" \
	"$(INTDIR)\MagicDamage.obj" \
	"$(INTDIR)\MagicInf.obj" \
	"$(INTDIR)\MapClass.obj" \
	"$(INTDIR)\MapItem.obj" \
	"$(INTDIR)\MapServerManager.obj" \
	"$(INTDIR)\Mercenary.obj" \
	"$(INTDIR)\MonsterAttr.obj" \
	"$(INTDIR)\MonsterHerd.obj" \
	"$(INTDIR)\MonsterItemMng.obj" \
	"$(INTDIR)\MonsterSetBase.obj" \
	"$(INTDIR)\MoveCheck.obj" \
	"$(INTDIR)\MoveCommand.obj" \
	"$(INTDIR)\MultiAttackHackCheck.obj" \
	"$(INTDIR)\MultiCheckSum.obj" \
	"$(INTDIR)\MyWinsockBase.obj" \
	"$(INTDIR)\NpcTalk.obj" \
	"$(INTDIR)\NSerialCheck.obj" \
	"$(INTDIR)\ObjAttack.obj" \
	"$(INTDIR)\ObjBaseAttack.obj" \
	"$(INTDIR)\ObjCalCharacter.obj" \
	"$(INTDIR)\ObjUseSkill.obj" \
	"$(INTDIR)\PacketCheckSum.obj" \
	"$(INTDIR)\PartyClass.obj" \
	"$(INTDIR)\protocol.obj" \
	"$(INTDIR)\QuestInfo.obj" \
	"$(INTDIR)\RingAttackEvent.obj" \
	"$(INTDIR)\SendHackLog.obj" \
	"$(INTDIR)\SetItemMacro.obj" \
	"$(INTDIR)\SetItemOption.obj" \
	"$(INTDIR)\Shop.obj" \
	"$(INTDIR)\SkillAdditionInfo.obj" \
	"$(INTDIR)\SkillDelay.obj" \
	"$(INTDIR)\SkillHitBox.obj" \
	"$(INTDIR)\SProtocol.obj" \
	"$(INTDIR)\StatMng.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\TDurMagicKeyChecker.obj" \
	"$(INTDIR)\TMonsterAI.obj" \
	"$(INTDIR)\TMonsterAIAgro.obj" \
	"$(INTDIR)\TMonsterAIAutomata.obj" \
	"$(INTDIR)\TMonsterAIElement.obj" \
	"$(INTDIR)\TMonsterAIGroup.obj" \
	"$(INTDIR)\TMonsterAIGroupMember.obj" \
	"$(INTDIR)\TMonsterAIMovePath.obj" \
	"$(INTDIR)\TMonsterAIRule.obj" \
	"$(INTDIR)\TMonsterAIRuleInfo.obj" \
	"$(INTDIR)\TMonsterAIUnit.obj" \
	"$(INTDIR)\TMonsterAIUtil.obj" \
	"$(INTDIR)\TMonsterSkillElement.obj" \
	"$(INTDIR)\TMonsterSkillElementInfo.obj" \
	"$(INTDIR)\TMonsterSkillManager.obj" \
	"$(INTDIR)\TMonsterSkillUnit.obj" \
	"$(INTDIR)\TNotice.obj" \
	"$(INTDIR)\TRandomPoolMgr.obj" \
	"$(INTDIR)\TServerAlertManager.obj" \
	"$(INTDIR)\TServerInfoDisplayer.obj" \
	"$(INTDIR)\TStatistics.obj" \
	"$(INTDIR)\TSync.obj" \
	"$(INTDIR)\TUnion.obj" \
	"$(INTDIR)\TUnionInfo.obj" \
	"$(INTDIR)\user.obj" \
	"$(INTDIR)\ViewportGuild.obj" \
	"$(INTDIR)\Weapon.obj" \
	"$(INTDIR)\WhisperCash.obj" \
	"$(INTDIR)\winutil.obj" \
	"$(INTDIR)\wsGameServer.obj" \
	"$(INTDIR)\wsJoinServerCli.obj" \
	"$(INTDIR)\wsShopServerCli.obj" \
	"$(INTDIR)\WTEventItemList.obj" \
	"$(INTDIR)\WzMemScript.obj" \
	"$(INTDIR)\WzMultiCastSock.obj" \
	"$(INTDIR)\WzQueue.obj" \
	"$(INTDIR)\WZScriptEncode.obj" \
	"$(INTDIR)\WzUdp.obj" \
	"$(INTDIR)\zzzitem.obj" \
	"$(INTDIR)\zzzmathlib.obj" \
	"$(INTDIR)\GameServer.res" \
	"..\..\MuMsg\Release\MuMsg.lib" \
	"..\SimpleModulus\Release\SimpleModulus.lib" \
	"..\GGSvr\Release\GGSvr.lib"

"$(OUTDIR)\GameServer.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\GameServer.exe"


CLEAN :
	-@erase "$(INTDIR)\AcceptIp.obj"
	-@erase "$(INTDIR)\AttackEvent.obj"
	-@erase "$(INTDIR)\BattleGround.obj"
	-@erase "$(INTDIR)\BattleSoccer.obj"
	-@erase "$(INTDIR)\BattleSoccerManager.obj"
	-@erase "$(INTDIR)\BloodCastle.obj"
	-@erase "$(INTDIR)\CallStackTrace.obj"
	-@erase "$(INTDIR)\CannonTower.obj"
	-@erase "$(INTDIR)\CashItemPeriodSystem.obj"
	-@erase "$(INTDIR)\CashLotterySystem.obj"
	-@erase "$(INTDIR)\CashShop.obj"
	-@erase "$(INTDIR)\CastleCrown.obj"
	-@erase "$(INTDIR)\CastleCrownSwitch.obj"
	-@erase "$(INTDIR)\CastleDeepEvent.obj"
	-@erase "$(INTDIR)\CastleSiege.obj"
	-@erase "$(INTDIR)\CastleSiegeSync.obj"
	-@erase "$(INTDIR)\CCastleEventItemList.obj"
	-@erase "$(INTDIR)\ChaosBox.obj"
	-@erase "$(INTDIR)\ChaosCastle.obj"
	-@erase "$(INTDIR)\ChaosCastleSetItemDrop.obj"
	-@erase "$(INTDIR)\ChinaHackUserKick.obj"
	-@erase "$(INTDIR)\classdef.obj"
	-@erase "$(INTDIR)\CLoginCount.obj"
	-@erase "$(INTDIR)\ClosePlayer.obj"
	-@erase "$(INTDIR)\ComboAttack.obj"
	-@erase "$(INTDIR)\ConMember.obj"
	-@erase "$(INTDIR)\Crywolf.obj"
	-@erase "$(INTDIR)\CrywolfAltar.obj"
	-@erase "$(INTDIR)\CrywolfDarkElf.obj"
	-@erase "$(INTDIR)\CrywolfMonster.obj"
	-@erase "$(INTDIR)\CrywolfStateTimeInfo.obj"
	-@erase "$(INTDIR)\CrywolfStatue.obj"
	-@erase "$(INTDIR)\CrywolfSync.obj"
	-@erase "$(INTDIR)\CrywolfTanker.obj"
	-@erase "$(INTDIR)\CrywolfUtil.obj"
	-@erase "$(INTDIR)\CWhatsUpDummyServer.obj"
	-@erase "$(INTDIR)\DarkSpirit.obj"
	-@erase "$(INTDIR)\DbSave.obj"
	-@erase "$(INTDIR)\DBSockMng.obj"
	-@erase "$(INTDIR)\DevilSquare.obj"
	-@erase "$(INTDIR)\DevilSquareGround.obj"
	-@erase "$(INTDIR)\DirPath.obj"
	-@erase "$(INTDIR)\DragonEvent.obj"
	-@erase "$(INTDIR)\DSProtocol.obj"
	-@erase "$(INTDIR)\EDSProtocol.obj"
	-@erase "$(INTDIR)\EledoradoEvent.obj"
	-@erase "$(INTDIR)\Event.obj"
	-@erase "$(INTDIR)\EventManagement.obj"
	-@erase "$(INTDIR)\GameEvent.obj"
	-@erase "$(INTDIR)\GameMain.obj"
	-@erase "$(INTDIR)\GameServer.obj"
	-@erase "$(INTDIR)\GameServer.pch"
	-@erase "$(INTDIR)\GameServer.res"
	-@erase "$(INTDIR)\GameServerAuth.obj"
	-@erase "$(INTDIR)\Gate.obj"
	-@erase "$(INTDIR)\giocp.obj"
	-@erase "$(INTDIR)\GMMng.obj"
	-@erase "$(INTDIR)\gObjMonster.obj"
	-@erase "$(INTDIR)\Guardian.obj"
	-@erase "$(INTDIR)\GuardianStatue.obj"
	-@erase "$(INTDIR)\GuildClass.obj"
	-@erase "$(INTDIR)\IpCache.obj"
	-@erase "$(INTDIR)\ItemAddOption.obj"
	-@erase "$(INTDIR)\ItemBag.obj"
	-@erase "$(INTDIR)\ItemBagEx.obj"
	-@erase "$(INTDIR)\ItemSystemFor380.obj"
	-@erase "$(INTDIR)\JewelMixSystem.obj"
	-@erase "$(INTDIR)\JewelOfHarmonySystem.obj"
	-@erase "$(INTDIR)\KalimaGate.obj"
	-@erase "$(INTDIR)\Kanturu.obj"
	-@erase "$(INTDIR)\KanturuBattleOfMaya.obj"
	-@erase "$(INTDIR)\KanturuBattleOfNightmare.obj"
	-@erase "$(INTDIR)\KanturuBattleStanby.obj"
	-@erase "$(INTDIR)\KanturuBattleUserMng.obj"
	-@erase "$(INTDIR)\KanturuEntranceNPC.obj"
	-@erase "$(INTDIR)\KanturuMaya.obj"
	-@erase "$(INTDIR)\KanturuMonsterMng.obj"
	-@erase "$(INTDIR)\KanturuNightmare.obj"
	-@erase "$(INTDIR)\KanturuStateInfo.obj"
	-@erase "$(INTDIR)\KanturuTowerOfRefinement.obj"
	-@erase "$(INTDIR)\KanturuUtil.obj"
	-@erase "$(INTDIR)\LargeRand.obj"
	-@erase "$(INTDIR)\LifeStone.obj"
	-@erase "$(INTDIR)\LogProc.obj"
	-@erase "$(INTDIR)\LogToFile.obj"
	-@erase "$(INTDIR)\MagicDamage.obj"
	-@erase "$(INTDIR)\MagicInf.obj"
	-@erase "$(INTDIR)\MapClass.obj"
	-@erase "$(INTDIR)\MapItem.obj"
	-@erase "$(INTDIR)\MapServerManager.obj"
	-@erase "$(INTDIR)\Mercenary.obj"
	-@erase "$(INTDIR)\MonsterAttr.obj"
	-@erase "$(INTDIR)\MonsterHerd.obj"
	-@erase "$(INTDIR)\MonsterItemMng.obj"
	-@erase "$(INTDIR)\MonsterSetBase.obj"
	-@erase "$(INTDIR)\MoveCheck.obj"
	-@erase "$(INTDIR)\MoveCommand.obj"
	-@erase "$(INTDIR)\MultiAttackHackCheck.obj"
	-@erase "$(INTDIR)\MultiCheckSum.obj"
	-@erase "$(INTDIR)\MyWinsockBase.obj"
	-@erase "$(INTDIR)\NpcTalk.obj"
	-@erase "$(INTDIR)\NSerialCheck.obj"
	-@erase "$(INTDIR)\ObjAttack.obj"
	-@erase "$(INTDIR)\ObjBaseAttack.obj"
	-@erase "$(INTDIR)\ObjCalCharacter.obj"
	-@erase "$(INTDIR)\ObjUseSkill.obj"
	-@erase "$(INTDIR)\PacketCheckSum.obj"
	-@erase "$(INTDIR)\PartyClass.obj"
	-@erase "$(INTDIR)\protocol.obj"
	-@erase "$(INTDIR)\QuestInfo.obj"
	-@erase "$(INTDIR)\RingAttackEvent.obj"
	-@erase "$(INTDIR)\SendHackLog.obj"
	-@erase "$(INTDIR)\SetItemMacro.obj"
	-@erase "$(INTDIR)\SetItemOption.obj"
	-@erase "$(INTDIR)\Shop.obj"
	-@erase "$(INTDIR)\SkillAdditionInfo.obj"
	-@erase "$(INTDIR)\SkillDelay.obj"
	-@erase "$(INTDIR)\SkillHitBox.obj"
	-@erase "$(INTDIR)\SProtocol.obj"
	-@erase "$(INTDIR)\StatMng.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\TDurMagicKeyChecker.obj"
	-@erase "$(INTDIR)\TMonsterAI.obj"
	-@erase "$(INTDIR)\TMonsterAIAgro.obj"
	-@erase "$(INTDIR)\TMonsterAIAutomata.obj"
	-@erase "$(INTDIR)\TMonsterAIElement.obj"
	-@erase "$(INTDIR)\TMonsterAIGroup.obj"
	-@erase "$(INTDIR)\TMonsterAIGroupMember.obj"
	-@erase "$(INTDIR)\TMonsterAIMovePath.obj"
	-@erase "$(INTDIR)\TMonsterAIRule.obj"
	-@erase "$(INTDIR)\TMonsterAIRuleInfo.obj"
	-@erase "$(INTDIR)\TMonsterAIUnit.obj"
	-@erase "$(INTDIR)\TMonsterAIUtil.obj"
	-@erase "$(INTDIR)\TMonsterSkillElement.obj"
	-@erase "$(INTDIR)\TMonsterSkillElementInfo.obj"
	-@erase "$(INTDIR)\TMonsterSkillManager.obj"
	-@erase "$(INTDIR)\TMonsterSkillUnit.obj"
	-@erase "$(INTDIR)\TNotice.obj"
	-@erase "$(INTDIR)\TRandomPoolMgr.obj"
	-@erase "$(INTDIR)\TServerAlertManager.obj"
	-@erase "$(INTDIR)\TServerInfoDisplayer.obj"
	-@erase "$(INTDIR)\TStatistics.obj"
	-@erase "$(INTDIR)\TSync.obj"
	-@erase "$(INTDIR)\TUnion.obj"
	-@erase "$(INTDIR)\TUnionInfo.obj"
	-@erase "$(INTDIR)\user.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\ViewportGuild.obj"
	-@erase "$(INTDIR)\Weapon.obj"
	-@erase "$(INTDIR)\WhisperCash.obj"
	-@erase "$(INTDIR)\winutil.obj"
	-@erase "$(INTDIR)\wsGameServer.obj"
	-@erase "$(INTDIR)\wsJoinServerCli.obj"
	-@erase "$(INTDIR)\wsShopServerCli.obj"
	-@erase "$(INTDIR)\WTEventItemList.obj"
	-@erase "$(INTDIR)\WzMemScript.obj"
	-@erase "$(INTDIR)\WzMultiCastSock.obj"
	-@erase "$(INTDIR)\WzQueue.obj"
	-@erase "$(INTDIR)\WZScriptEncode.obj"
	-@erase "$(INTDIR)\WzUdp.obj"
	-@erase "$(INTDIR)\zzzitem.obj"
	-@erase "$(INTDIR)\zzzmathlib.obj"
	-@erase "$(OUTDIR)\GameServer.exe"
	-@erase "$(OUTDIR)\GameServer.ilk"
	-@erase "$(OUTDIR)\GameServer.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MLd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)\GameServer.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x280a /fo"$(INTDIR)\GameServer.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\GameServer.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\GameServer.pdb" /debug /machine:I386 /out:"$(OUTDIR)\GameServer.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\AcceptIp.obj" \
	"$(INTDIR)\AttackEvent.obj" \
	"$(INTDIR)\BattleGround.obj" \
	"$(INTDIR)\BattleSoccer.obj" \
	"$(INTDIR)\BattleSoccerManager.obj" \
	"$(INTDIR)\BloodCastle.obj" \
	"$(INTDIR)\CallStackTrace.obj" \
	"$(INTDIR)\CannonTower.obj" \
	"$(INTDIR)\CashItemPeriodSystem.obj" \
	"$(INTDIR)\CashLotterySystem.obj" \
	"$(INTDIR)\CashShop.obj" \
	"$(INTDIR)\CastleCrown.obj" \
	"$(INTDIR)\CastleCrownSwitch.obj" \
	"$(INTDIR)\CastleDeepEvent.obj" \
	"$(INTDIR)\CastleSiege.obj" \
	"$(INTDIR)\CastleSiegeSync.obj" \
	"$(INTDIR)\CCastleEventItemList.obj" \
	"$(INTDIR)\ChaosBox.obj" \
	"$(INTDIR)\ChaosCastle.obj" \
	"$(INTDIR)\ChaosCastleSetItemDrop.obj" \
	"$(INTDIR)\ChinaHackUserKick.obj" \
	"$(INTDIR)\classdef.obj" \
	"$(INTDIR)\CLoginCount.obj" \
	"$(INTDIR)\ClosePlayer.obj" \
	"$(INTDIR)\ComboAttack.obj" \
	"$(INTDIR)\ConMember.obj" \
	"$(INTDIR)\Crywolf.obj" \
	"$(INTDIR)\CrywolfAltar.obj" \
	"$(INTDIR)\CrywolfDarkElf.obj" \
	"$(INTDIR)\CrywolfMonster.obj" \
	"$(INTDIR)\CrywolfStateTimeInfo.obj" \
	"$(INTDIR)\CrywolfStatue.obj" \
	"$(INTDIR)\CrywolfSync.obj" \
	"$(INTDIR)\CrywolfTanker.obj" \
	"$(INTDIR)\CrywolfUtil.obj" \
	"$(INTDIR)\CWhatsUpDummyServer.obj" \
	"$(INTDIR)\DarkSpirit.obj" \
	"$(INTDIR)\DbSave.obj" \
	"$(INTDIR)\DBSockMng.obj" \
	"$(INTDIR)\DevilSquare.obj" \
	"$(INTDIR)\DevilSquareGround.obj" \
	"$(INTDIR)\DirPath.obj" \
	"$(INTDIR)\DragonEvent.obj" \
	"$(INTDIR)\DSProtocol.obj" \
	"$(INTDIR)\EDSProtocol.obj" \
	"$(INTDIR)\EledoradoEvent.obj" \
	"$(INTDIR)\Event.obj" \
	"$(INTDIR)\EventManagement.obj" \
	"$(INTDIR)\GameEvent.obj" \
	"$(INTDIR)\GameMain.obj" \
	"$(INTDIR)\GameServer.obj" \
	"$(INTDIR)\GameServerAuth.obj" \
	"$(INTDIR)\Gate.obj" \
	"$(INTDIR)\giocp.obj" \
	"$(INTDIR)\GMMng.obj" \
	"$(INTDIR)\gObjMonster.obj" \
	"$(INTDIR)\Guardian.obj" \
	"$(INTDIR)\GuardianStatue.obj" \
	"$(INTDIR)\GuildClass.obj" \
	"$(INTDIR)\IpCache.obj" \
	"$(INTDIR)\ItemAddOption.obj" \
	"$(INTDIR)\ItemBag.obj" \
	"$(INTDIR)\ItemBagEx.obj" \
	"$(INTDIR)\ItemSystemFor380.obj" \
	"$(INTDIR)\JewelMixSystem.obj" \
	"$(INTDIR)\JewelOfHarmonySystem.obj" \
	"$(INTDIR)\KalimaGate.obj" \
	"$(INTDIR)\Kanturu.obj" \
	"$(INTDIR)\KanturuBattleOfMaya.obj" \
	"$(INTDIR)\KanturuBattleOfNightmare.obj" \
	"$(INTDIR)\KanturuBattleStanby.obj" \
	"$(INTDIR)\KanturuBattleUserMng.obj" \
	"$(INTDIR)\KanturuEntranceNPC.obj" \
	"$(INTDIR)\KanturuMaya.obj" \
	"$(INTDIR)\KanturuMonsterMng.obj" \
	"$(INTDIR)\KanturuNightmare.obj" \
	"$(INTDIR)\KanturuStateInfo.obj" \
	"$(INTDIR)\KanturuTowerOfRefinement.obj" \
	"$(INTDIR)\KanturuUtil.obj" \
	"$(INTDIR)\LargeRand.obj" \
	"$(INTDIR)\LifeStone.obj" \
	"$(INTDIR)\LogProc.obj" \
	"$(INTDIR)\LogToFile.obj" \
	"$(INTDIR)\MagicDamage.obj" \
	"$(INTDIR)\MagicInf.obj" \
	"$(INTDIR)\MapClass.obj" \
	"$(INTDIR)\MapItem.obj" \
	"$(INTDIR)\MapServerManager.obj" \
	"$(INTDIR)\Mercenary.obj" \
	"$(INTDIR)\MonsterAttr.obj" \
	"$(INTDIR)\MonsterHerd.obj" \
	"$(INTDIR)\MonsterItemMng.obj" \
	"$(INTDIR)\MonsterSetBase.obj" \
	"$(INTDIR)\MoveCheck.obj" \
	"$(INTDIR)\MoveCommand.obj" \
	"$(INTDIR)\MultiAttackHackCheck.obj" \
	"$(INTDIR)\MultiCheckSum.obj" \
	"$(INTDIR)\MyWinsockBase.obj" \
	"$(INTDIR)\NpcTalk.obj" \
	"$(INTDIR)\NSerialCheck.obj" \
	"$(INTDIR)\ObjAttack.obj" \
	"$(INTDIR)\ObjBaseAttack.obj" \
	"$(INTDIR)\ObjCalCharacter.obj" \
	"$(INTDIR)\ObjUseSkill.obj" \
	"$(INTDIR)\PacketCheckSum.obj" \
	"$(INTDIR)\PartyClass.obj" \
	"$(INTDIR)\protocol.obj" \
	"$(INTDIR)\QuestInfo.obj" \
	"$(INTDIR)\RingAttackEvent.obj" \
	"$(INTDIR)\SendHackLog.obj" \
	"$(INTDIR)\SetItemMacro.obj" \
	"$(INTDIR)\SetItemOption.obj" \
	"$(INTDIR)\Shop.obj" \
	"$(INTDIR)\SkillAdditionInfo.obj" \
	"$(INTDIR)\SkillDelay.obj" \
	"$(INTDIR)\SkillHitBox.obj" \
	"$(INTDIR)\SProtocol.obj" \
	"$(INTDIR)\StatMng.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\TDurMagicKeyChecker.obj" \
	"$(INTDIR)\TMonsterAI.obj" \
	"$(INTDIR)\TMonsterAIAgro.obj" \
	"$(INTDIR)\TMonsterAIAutomata.obj" \
	"$(INTDIR)\TMonsterAIElement.obj" \
	"$(INTDIR)\TMonsterAIGroup.obj" \
	"$(INTDIR)\TMonsterAIGroupMember.obj" \
	"$(INTDIR)\TMonsterAIMovePath.obj" \
	"$(INTDIR)\TMonsterAIRule.obj" \
	"$(INTDIR)\TMonsterAIRuleInfo.obj" \
	"$(INTDIR)\TMonsterAIUnit.obj" \
	"$(INTDIR)\TMonsterAIUtil.obj" \
	"$(INTDIR)\TMonsterSkillElement.obj" \
	"$(INTDIR)\TMonsterSkillElementInfo.obj" \
	"$(INTDIR)\TMonsterSkillManager.obj" \
	"$(INTDIR)\TMonsterSkillUnit.obj" \
	"$(INTDIR)\TNotice.obj" \
	"$(INTDIR)\TRandomPoolMgr.obj" \
	"$(INTDIR)\TServerAlertManager.obj" \
	"$(INTDIR)\TServerInfoDisplayer.obj" \
	"$(INTDIR)\TStatistics.obj" \
	"$(INTDIR)\TSync.obj" \
	"$(INTDIR)\TUnion.obj" \
	"$(INTDIR)\TUnionInfo.obj" \
	"$(INTDIR)\user.obj" \
	"$(INTDIR)\ViewportGuild.obj" \
	"$(INTDIR)\Weapon.obj" \
	"$(INTDIR)\WhisperCash.obj" \
	"$(INTDIR)\winutil.obj" \
	"$(INTDIR)\wsGameServer.obj" \
	"$(INTDIR)\wsJoinServerCli.obj" \
	"$(INTDIR)\wsShopServerCli.obj" \
	"$(INTDIR)\WTEventItemList.obj" \
	"$(INTDIR)\WzMemScript.obj" \
	"$(INTDIR)\WzMultiCastSock.obj" \
	"$(INTDIR)\WzQueue.obj" \
	"$(INTDIR)\WZScriptEncode.obj" \
	"$(INTDIR)\WzUdp.obj" \
	"$(INTDIR)\zzzitem.obj" \
	"$(INTDIR)\zzzmathlib.obj" \
	"$(INTDIR)\GameServer.res" \
	"..\..\MuMsg\Release\MuMsg.lib" \
	"..\SimpleModulus\Release\SimpleModulus.lib" \
	"..\GGSvr\Release\GGSvr.lib"

"$(OUTDIR)\GameServer.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"

OUTDIR=.\GameServer___Win32_DebugRelease
INTDIR=.\GameServer___Win32_DebugRelease
# Begin Custom Macros
OutDir=.\GameServer___Win32_DebugRelease
# End Custom Macros

ALL : "$(OUTDIR)\GameServer.exe" "$(OUTDIR)\GameServer.bsc"


CLEAN :
	-@erase "$(INTDIR)\AcceptIp.obj"
	-@erase "$(INTDIR)\AcceptIp.sbr"
	-@erase "$(INTDIR)\AttackEvent.obj"
	-@erase "$(INTDIR)\AttackEvent.sbr"
	-@erase "$(INTDIR)\BattleGround.obj"
	-@erase "$(INTDIR)\BattleGround.sbr"
	-@erase "$(INTDIR)\BattleSoccer.obj"
	-@erase "$(INTDIR)\BattleSoccer.sbr"
	-@erase "$(INTDIR)\BattleSoccerManager.obj"
	-@erase "$(INTDIR)\BattleSoccerManager.sbr"
	-@erase "$(INTDIR)\BloodCastle.obj"
	-@erase "$(INTDIR)\BloodCastle.sbr"
	-@erase "$(INTDIR)\CallStackTrace.obj"
	-@erase "$(INTDIR)\CallStackTrace.sbr"
	-@erase "$(INTDIR)\CannonTower.obj"
	-@erase "$(INTDIR)\CannonTower.sbr"
	-@erase "$(INTDIR)\CashItemPeriodSystem.obj"
	-@erase "$(INTDIR)\CashItemPeriodSystem.sbr"
	-@erase "$(INTDIR)\CashLotterySystem.obj"
	-@erase "$(INTDIR)\CashLotterySystem.sbr"
	-@erase "$(INTDIR)\CashShop.obj"
	-@erase "$(INTDIR)\CashShop.sbr"
	-@erase "$(INTDIR)\CastleCrown.obj"
	-@erase "$(INTDIR)\CastleCrown.sbr"
	-@erase "$(INTDIR)\CastleCrownSwitch.obj"
	-@erase "$(INTDIR)\CastleCrownSwitch.sbr"
	-@erase "$(INTDIR)\CastleDeepEvent.obj"
	-@erase "$(INTDIR)\CastleDeepEvent.sbr"
	-@erase "$(INTDIR)\CastleSiege.obj"
	-@erase "$(INTDIR)\CastleSiege.sbr"
	-@erase "$(INTDIR)\CastleSiegeSync.obj"
	-@erase "$(INTDIR)\CastleSiegeSync.sbr"
	-@erase "$(INTDIR)\CCastleEventItemList.obj"
	-@erase "$(INTDIR)\CCastleEventItemList.sbr"
	-@erase "$(INTDIR)\ChaosBox.obj"
	-@erase "$(INTDIR)\ChaosBox.sbr"
	-@erase "$(INTDIR)\ChaosCastle.obj"
	-@erase "$(INTDIR)\ChaosCastle.sbr"
	-@erase "$(INTDIR)\ChaosCastleSetItemDrop.obj"
	-@erase "$(INTDIR)\ChaosCastleSetItemDrop.sbr"
	-@erase "$(INTDIR)\ChinaHackUserKick.obj"
	-@erase "$(INTDIR)\ChinaHackUserKick.sbr"
	-@erase "$(INTDIR)\classdef.obj"
	-@erase "$(INTDIR)\classdef.sbr"
	-@erase "$(INTDIR)\CLoginCount.obj"
	-@erase "$(INTDIR)\CLoginCount.sbr"
	-@erase "$(INTDIR)\ClosePlayer.obj"
	-@erase "$(INTDIR)\ClosePlayer.sbr"
	-@erase "$(INTDIR)\ComboAttack.obj"
	-@erase "$(INTDIR)\ComboAttack.sbr"
	-@erase "$(INTDIR)\ConMember.obj"
	-@erase "$(INTDIR)\ConMember.sbr"
	-@erase "$(INTDIR)\Crywolf.obj"
	-@erase "$(INTDIR)\Crywolf.sbr"
	-@erase "$(INTDIR)\CrywolfAltar.obj"
	-@erase "$(INTDIR)\CrywolfAltar.sbr"
	-@erase "$(INTDIR)\CrywolfDarkElf.obj"
	-@erase "$(INTDIR)\CrywolfDarkElf.sbr"
	-@erase "$(INTDIR)\CrywolfMonster.obj"
	-@erase "$(INTDIR)\CrywolfMonster.sbr"
	-@erase "$(INTDIR)\CrywolfStateTimeInfo.obj"
	-@erase "$(INTDIR)\CrywolfStateTimeInfo.sbr"
	-@erase "$(INTDIR)\CrywolfStatue.obj"
	-@erase "$(INTDIR)\CrywolfStatue.sbr"
	-@erase "$(INTDIR)\CrywolfSync.obj"
	-@erase "$(INTDIR)\CrywolfSync.sbr"
	-@erase "$(INTDIR)\CrywolfTanker.obj"
	-@erase "$(INTDIR)\CrywolfTanker.sbr"
	-@erase "$(INTDIR)\CrywolfUtil.obj"
	-@erase "$(INTDIR)\CrywolfUtil.sbr"
	-@erase "$(INTDIR)\CWhatsUpDummyServer.obj"
	-@erase "$(INTDIR)\CWhatsUpDummyServer.sbr"
	-@erase "$(INTDIR)\DarkSpirit.obj"
	-@erase "$(INTDIR)\DarkSpirit.sbr"
	-@erase "$(INTDIR)\DbSave.obj"
	-@erase "$(INTDIR)\DbSave.sbr"
	-@erase "$(INTDIR)\DBSockMng.obj"
	-@erase "$(INTDIR)\DBSockMng.sbr"
	-@erase "$(INTDIR)\DevilSquare.obj"
	-@erase "$(INTDIR)\DevilSquare.sbr"
	-@erase "$(INTDIR)\DevilSquareGround.obj"
	-@erase "$(INTDIR)\DevilSquareGround.sbr"
	-@erase "$(INTDIR)\DirPath.obj"
	-@erase "$(INTDIR)\DirPath.sbr"
	-@erase "$(INTDIR)\DragonEvent.obj"
	-@erase "$(INTDIR)\DragonEvent.sbr"
	-@erase "$(INTDIR)\DSProtocol.obj"
	-@erase "$(INTDIR)\DSProtocol.sbr"
	-@erase "$(INTDIR)\EDSProtocol.obj"
	-@erase "$(INTDIR)\EDSProtocol.sbr"
	-@erase "$(INTDIR)\EledoradoEvent.obj"
	-@erase "$(INTDIR)\EledoradoEvent.sbr"
	-@erase "$(INTDIR)\Event.obj"
	-@erase "$(INTDIR)\Event.sbr"
	-@erase "$(INTDIR)\EventManagement.obj"
	-@erase "$(INTDIR)\EventManagement.sbr"
	-@erase "$(INTDIR)\GameEvent.obj"
	-@erase "$(INTDIR)\GameEvent.sbr"
	-@erase "$(INTDIR)\GameMain.obj"
	-@erase "$(INTDIR)\GameMain.sbr"
	-@erase "$(INTDIR)\GameServer.obj"
	-@erase "$(INTDIR)\GameServer.pch"
	-@erase "$(INTDIR)\GameServer.res"
	-@erase "$(INTDIR)\GameServer.sbr"
	-@erase "$(INTDIR)\GameServerAuth.obj"
	-@erase "$(INTDIR)\GameServerAuth.sbr"
	-@erase "$(INTDIR)\Gate.obj"
	-@erase "$(INTDIR)\Gate.sbr"
	-@erase "$(INTDIR)\giocp.obj"
	-@erase "$(INTDIR)\giocp.sbr"
	-@erase "$(INTDIR)\GMMng.obj"
	-@erase "$(INTDIR)\GMMng.sbr"
	-@erase "$(INTDIR)\gObjMonster.obj"
	-@erase "$(INTDIR)\gObjMonster.sbr"
	-@erase "$(INTDIR)\Guardian.obj"
	-@erase "$(INTDIR)\Guardian.sbr"
	-@erase "$(INTDIR)\GuardianStatue.obj"
	-@erase "$(INTDIR)\GuardianStatue.sbr"
	-@erase "$(INTDIR)\GuildClass.obj"
	-@erase "$(INTDIR)\GuildClass.sbr"
	-@erase "$(INTDIR)\IpCache.obj"
	-@erase "$(INTDIR)\IpCache.sbr"
	-@erase "$(INTDIR)\ItemAddOption.obj"
	-@erase "$(INTDIR)\ItemAddOption.sbr"
	-@erase "$(INTDIR)\ItemBag.obj"
	-@erase "$(INTDIR)\ItemBag.sbr"
	-@erase "$(INTDIR)\ItemBagEx.obj"
	-@erase "$(INTDIR)\ItemBagEx.sbr"
	-@erase "$(INTDIR)\ItemSystemFor380.obj"
	-@erase "$(INTDIR)\ItemSystemFor380.sbr"
	-@erase "$(INTDIR)\JewelMixSystem.obj"
	-@erase "$(INTDIR)\JewelMixSystem.sbr"
	-@erase "$(INTDIR)\JewelOfHarmonySystem.obj"
	-@erase "$(INTDIR)\JewelOfHarmonySystem.sbr"
	-@erase "$(INTDIR)\KalimaGate.obj"
	-@erase "$(INTDIR)\KalimaGate.sbr"
	-@erase "$(INTDIR)\Kanturu.obj"
	-@erase "$(INTDIR)\Kanturu.sbr"
	-@erase "$(INTDIR)\KanturuBattleOfMaya.obj"
	-@erase "$(INTDIR)\KanturuBattleOfMaya.sbr"
	-@erase "$(INTDIR)\KanturuBattleOfNightmare.obj"
	-@erase "$(INTDIR)\KanturuBattleOfNightmare.sbr"
	-@erase "$(INTDIR)\KanturuBattleStanby.obj"
	-@erase "$(INTDIR)\KanturuBattleStanby.sbr"
	-@erase "$(INTDIR)\KanturuBattleUserMng.obj"
	-@erase "$(INTDIR)\KanturuBattleUserMng.sbr"
	-@erase "$(INTDIR)\KanturuEntranceNPC.obj"
	-@erase "$(INTDIR)\KanturuEntranceNPC.sbr"
	-@erase "$(INTDIR)\KanturuMaya.obj"
	-@erase "$(INTDIR)\KanturuMaya.sbr"
	-@erase "$(INTDIR)\KanturuMonsterMng.obj"
	-@erase "$(INTDIR)\KanturuMonsterMng.sbr"
	-@erase "$(INTDIR)\KanturuNightmare.obj"
	-@erase "$(INTDIR)\KanturuNightmare.sbr"
	-@erase "$(INTDIR)\KanturuStateInfo.obj"
	-@erase "$(INTDIR)\KanturuStateInfo.sbr"
	-@erase "$(INTDIR)\KanturuTowerOfRefinement.obj"
	-@erase "$(INTDIR)\KanturuTowerOfRefinement.sbr"
	-@erase "$(INTDIR)\KanturuUtil.obj"
	-@erase "$(INTDIR)\KanturuUtil.sbr"
	-@erase "$(INTDIR)\LargeRand.obj"
	-@erase "$(INTDIR)\LargeRand.sbr"
	-@erase "$(INTDIR)\LifeStone.obj"
	-@erase "$(INTDIR)\LifeStone.sbr"
	-@erase "$(INTDIR)\LogProc.obj"
	-@erase "$(INTDIR)\LogProc.sbr"
	-@erase "$(INTDIR)\LogToFile.obj"
	-@erase "$(INTDIR)\LogToFile.sbr"
	-@erase "$(INTDIR)\MagicDamage.obj"
	-@erase "$(INTDIR)\MagicDamage.sbr"
	-@erase "$(INTDIR)\MagicInf.obj"
	-@erase "$(INTDIR)\MagicInf.sbr"
	-@erase "$(INTDIR)\MapClass.obj"
	-@erase "$(INTDIR)\MapClass.sbr"
	-@erase "$(INTDIR)\MapItem.obj"
	-@erase "$(INTDIR)\MapItem.sbr"
	-@erase "$(INTDIR)\MapServerManager.obj"
	-@erase "$(INTDIR)\MapServerManager.sbr"
	-@erase "$(INTDIR)\Mercenary.obj"
	-@erase "$(INTDIR)\Mercenary.sbr"
	-@erase "$(INTDIR)\MonsterAttr.obj"
	-@erase "$(INTDIR)\MonsterAttr.sbr"
	-@erase "$(INTDIR)\MonsterHerd.obj"
	-@erase "$(INTDIR)\MonsterHerd.sbr"
	-@erase "$(INTDIR)\MonsterItemMng.obj"
	-@erase "$(INTDIR)\MonsterItemMng.sbr"
	-@erase "$(INTDIR)\MonsterSetBase.obj"
	-@erase "$(INTDIR)\MonsterSetBase.sbr"
	-@erase "$(INTDIR)\MoveCheck.obj"
	-@erase "$(INTDIR)\MoveCheck.sbr"
	-@erase "$(INTDIR)\MoveCommand.obj"
	-@erase "$(INTDIR)\MoveCommand.sbr"
	-@erase "$(INTDIR)\MultiAttackHackCheck.obj"
	-@erase "$(INTDIR)\MultiAttackHackCheck.sbr"
	-@erase "$(INTDIR)\MultiCheckSum.obj"
	-@erase "$(INTDIR)\MultiCheckSum.sbr"
	-@erase "$(INTDIR)\MyWinsockBase.obj"
	-@erase "$(INTDIR)\MyWinsockBase.sbr"
	-@erase "$(INTDIR)\NpcTalk.obj"
	-@erase "$(INTDIR)\NpcTalk.sbr"
	-@erase "$(INTDIR)\NSerialCheck.obj"
	-@erase "$(INTDIR)\NSerialCheck.sbr"
	-@erase "$(INTDIR)\ObjAttack.obj"
	-@erase "$(INTDIR)\ObjAttack.sbr"
	-@erase "$(INTDIR)\ObjBaseAttack.obj"
	-@erase "$(INTDIR)\ObjBaseAttack.sbr"
	-@erase "$(INTDIR)\ObjCalCharacter.obj"
	-@erase "$(INTDIR)\ObjCalCharacter.sbr"
	-@erase "$(INTDIR)\ObjUseSkill.obj"
	-@erase "$(INTDIR)\ObjUseSkill.sbr"
	-@erase "$(INTDIR)\PacketCheckSum.obj"
	-@erase "$(INTDIR)\PacketCheckSum.sbr"
	-@erase "$(INTDIR)\PartyClass.obj"
	-@erase "$(INTDIR)\PartyClass.sbr"
	-@erase "$(INTDIR)\protocol.obj"
	-@erase "$(INTDIR)\protocol.sbr"
	-@erase "$(INTDIR)\QuestInfo.obj"
	-@erase "$(INTDIR)\QuestInfo.sbr"
	-@erase "$(INTDIR)\RingAttackEvent.obj"
	-@erase "$(INTDIR)\RingAttackEvent.sbr"
	-@erase "$(INTDIR)\SendHackLog.obj"
	-@erase "$(INTDIR)\SendHackLog.sbr"
	-@erase "$(INTDIR)\SetItemMacro.obj"
	-@erase "$(INTDIR)\SetItemMacro.sbr"
	-@erase "$(INTDIR)\SetItemOption.obj"
	-@erase "$(INTDIR)\SetItemOption.sbr"
	-@erase "$(INTDIR)\Shop.obj"
	-@erase "$(INTDIR)\Shop.sbr"
	-@erase "$(INTDIR)\SkillAdditionInfo.obj"
	-@erase "$(INTDIR)\SkillAdditionInfo.sbr"
	-@erase "$(INTDIR)\SkillDelay.obj"
	-@erase "$(INTDIR)\SkillDelay.sbr"
	-@erase "$(INTDIR)\SkillHitBox.obj"
	-@erase "$(INTDIR)\SkillHitBox.sbr"
	-@erase "$(INTDIR)\SProtocol.obj"
	-@erase "$(INTDIR)\SProtocol.sbr"
	-@erase "$(INTDIR)\StatMng.obj"
	-@erase "$(INTDIR)\StatMng.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\TDurMagicKeyChecker.obj"
	-@erase "$(INTDIR)\TDurMagicKeyChecker.sbr"
	-@erase "$(INTDIR)\TMonsterAI.obj"
	-@erase "$(INTDIR)\TMonsterAI.sbr"
	-@erase "$(INTDIR)\TMonsterAIAgro.obj"
	-@erase "$(INTDIR)\TMonsterAIAgro.sbr"
	-@erase "$(INTDIR)\TMonsterAIAutomata.obj"
	-@erase "$(INTDIR)\TMonsterAIAutomata.sbr"
	-@erase "$(INTDIR)\TMonsterAIElement.obj"
	-@erase "$(INTDIR)\TMonsterAIElement.sbr"
	-@erase "$(INTDIR)\TMonsterAIGroup.obj"
	-@erase "$(INTDIR)\TMonsterAIGroup.sbr"
	-@erase "$(INTDIR)\TMonsterAIGroupMember.obj"
	-@erase "$(INTDIR)\TMonsterAIGroupMember.sbr"
	-@erase "$(INTDIR)\TMonsterAIMovePath.obj"
	-@erase "$(INTDIR)\TMonsterAIMovePath.sbr"
	-@erase "$(INTDIR)\TMonsterAIRule.obj"
	-@erase "$(INTDIR)\TMonsterAIRule.sbr"
	-@erase "$(INTDIR)\TMonsterAIRuleInfo.obj"
	-@erase "$(INTDIR)\TMonsterAIRuleInfo.sbr"
	-@erase "$(INTDIR)\TMonsterAIUnit.obj"
	-@erase "$(INTDIR)\TMonsterAIUnit.sbr"
	-@erase "$(INTDIR)\TMonsterAIUtil.obj"
	-@erase "$(INTDIR)\TMonsterAIUtil.sbr"
	-@erase "$(INTDIR)\TMonsterSkillElement.obj"
	-@erase "$(INTDIR)\TMonsterSkillElement.sbr"
	-@erase "$(INTDIR)\TMonsterSkillElementInfo.obj"
	-@erase "$(INTDIR)\TMonsterSkillElementInfo.sbr"
	-@erase "$(INTDIR)\TMonsterSkillManager.obj"
	-@erase "$(INTDIR)\TMonsterSkillManager.sbr"
	-@erase "$(INTDIR)\TMonsterSkillUnit.obj"
	-@erase "$(INTDIR)\TMonsterSkillUnit.sbr"
	-@erase "$(INTDIR)\TNotice.obj"
	-@erase "$(INTDIR)\TNotice.sbr"
	-@erase "$(INTDIR)\TRandomPoolMgr.obj"
	-@erase "$(INTDIR)\TRandomPoolMgr.sbr"
	-@erase "$(INTDIR)\TServerAlertManager.obj"
	-@erase "$(INTDIR)\TServerAlertManager.sbr"
	-@erase "$(INTDIR)\TServerInfoDisplayer.obj"
	-@erase "$(INTDIR)\TServerInfoDisplayer.sbr"
	-@erase "$(INTDIR)\TStatistics.obj"
	-@erase "$(INTDIR)\TStatistics.sbr"
	-@erase "$(INTDIR)\TSync.obj"
	-@erase "$(INTDIR)\TSync.sbr"
	-@erase "$(INTDIR)\TUnion.obj"
	-@erase "$(INTDIR)\TUnion.sbr"
	-@erase "$(INTDIR)\TUnionInfo.obj"
	-@erase "$(INTDIR)\TUnionInfo.sbr"
	-@erase "$(INTDIR)\user.obj"
	-@erase "$(INTDIR)\user.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\ViewportGuild.obj"
	-@erase "$(INTDIR)\ViewportGuild.sbr"
	-@erase "$(INTDIR)\Weapon.obj"
	-@erase "$(INTDIR)\Weapon.sbr"
	-@erase "$(INTDIR)\WhisperCash.obj"
	-@erase "$(INTDIR)\WhisperCash.sbr"
	-@erase "$(INTDIR)\winutil.obj"
	-@erase "$(INTDIR)\winutil.sbr"
	-@erase "$(INTDIR)\wsGameServer.obj"
	-@erase "$(INTDIR)\wsGameServer.sbr"
	-@erase "$(INTDIR)\wsJoinServerCli.obj"
	-@erase "$(INTDIR)\wsJoinServerCli.sbr"
	-@erase "$(INTDIR)\wsShopServerCli.obj"
	-@erase "$(INTDIR)\wsShopServerCli.sbr"
	-@erase "$(INTDIR)\WTEventItemList.obj"
	-@erase "$(INTDIR)\WTEventItemList.sbr"
	-@erase "$(INTDIR)\WzMemScript.obj"
	-@erase "$(INTDIR)\WzMemScript.sbr"
	-@erase "$(INTDIR)\WzMultiCastSock.obj"
	-@erase "$(INTDIR)\WzMultiCastSock.sbr"
	-@erase "$(INTDIR)\WzQueue.obj"
	-@erase "$(INTDIR)\WzQueue.sbr"
	-@erase "$(INTDIR)\WZScriptEncode.obj"
	-@erase "$(INTDIR)\WZScriptEncode.sbr"
	-@erase "$(INTDIR)\WzUdp.obj"
	-@erase "$(INTDIR)\WzUdp.sbr"
	-@erase "$(INTDIR)\zzzitem.obj"
	-@erase "$(INTDIR)\zzzitem.sbr"
	-@erase "$(INTDIR)\zzzmathlib.obj"
	-@erase "$(INTDIR)\zzzmathlib.sbr"
	-@erase "$(OUTDIR)\GameServer.bsc"
	-@erase "$(OUTDIR)\GameServer.exe"
	-@erase "$(OUTDIR)\GameServer.ilk"
	-@erase "$(OUTDIR)\GameServer.map"
	-@erase "$(OUTDIR)\GameServer.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MT /W3 /GX /ZI /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FAcs /Fa"$(INTDIR)\\" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\GameServer.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x280a /fo"$(INTDIR)\GameServer.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\GameServer.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\AcceptIp.sbr" \
	"$(INTDIR)\AttackEvent.sbr" \
	"$(INTDIR)\BattleGround.sbr" \
	"$(INTDIR)\BattleSoccer.sbr" \
	"$(INTDIR)\BattleSoccerManager.sbr" \
	"$(INTDIR)\BloodCastle.sbr" \
	"$(INTDIR)\CallStackTrace.sbr" \
	"$(INTDIR)\CannonTower.sbr" \
	"$(INTDIR)\CashItemPeriodSystem.sbr" \
	"$(INTDIR)\CashLotterySystem.sbr" \
	"$(INTDIR)\CashShop.sbr" \
	"$(INTDIR)\CastleCrown.sbr" \
	"$(INTDIR)\CastleCrownSwitch.sbr" \
	"$(INTDIR)\CastleDeepEvent.sbr" \
	"$(INTDIR)\CastleSiege.sbr" \
	"$(INTDIR)\CastleSiegeSync.sbr" \
	"$(INTDIR)\CCastleEventItemList.sbr" \
	"$(INTDIR)\ChaosBox.sbr" \
	"$(INTDIR)\ChaosCastle.sbr" \
	"$(INTDIR)\ChaosCastleSetItemDrop.sbr" \
	"$(INTDIR)\ChinaHackUserKick.sbr" \
	"$(INTDIR)\classdef.sbr" \
	"$(INTDIR)\CLoginCount.sbr" \
	"$(INTDIR)\ClosePlayer.sbr" \
	"$(INTDIR)\ComboAttack.sbr" \
	"$(INTDIR)\ConMember.sbr" \
	"$(INTDIR)\Crywolf.sbr" \
	"$(INTDIR)\CrywolfAltar.sbr" \
	"$(INTDIR)\CrywolfDarkElf.sbr" \
	"$(INTDIR)\CrywolfMonster.sbr" \
	"$(INTDIR)\CrywolfStateTimeInfo.sbr" \
	"$(INTDIR)\CrywolfStatue.sbr" \
	"$(INTDIR)\CrywolfSync.sbr" \
	"$(INTDIR)\CrywolfTanker.sbr" \
	"$(INTDIR)\CrywolfUtil.sbr" \
	"$(INTDIR)\CWhatsUpDummyServer.sbr" \
	"$(INTDIR)\DarkSpirit.sbr" \
	"$(INTDIR)\DbSave.sbr" \
	"$(INTDIR)\DBSockMng.sbr" \
	"$(INTDIR)\DevilSquare.sbr" \
	"$(INTDIR)\DevilSquareGround.sbr" \
	"$(INTDIR)\DirPath.sbr" \
	"$(INTDIR)\DragonEvent.sbr" \
	"$(INTDIR)\DSProtocol.sbr" \
	"$(INTDIR)\EDSProtocol.sbr" \
	"$(INTDIR)\EledoradoEvent.sbr" \
	"$(INTDIR)\Event.sbr" \
	"$(INTDIR)\EventManagement.sbr" \
	"$(INTDIR)\GameEvent.sbr" \
	"$(INTDIR)\GameMain.sbr" \
	"$(INTDIR)\GameServer.sbr" \
	"$(INTDIR)\GameServerAuth.sbr" \
	"$(INTDIR)\Gate.sbr" \
	"$(INTDIR)\giocp.sbr" \
	"$(INTDIR)\GMMng.sbr" \
	"$(INTDIR)\gObjMonster.sbr" \
	"$(INTDIR)\Guardian.sbr" \
	"$(INTDIR)\GuardianStatue.sbr" \
	"$(INTDIR)\GuildClass.sbr" \
	"$(INTDIR)\IpCache.sbr" \
	"$(INTDIR)\ItemAddOption.sbr" \
	"$(INTDIR)\ItemBag.sbr" \
	"$(INTDIR)\ItemBagEx.sbr" \
	"$(INTDIR)\ItemSystemFor380.sbr" \
	"$(INTDIR)\JewelMixSystem.sbr" \
	"$(INTDIR)\JewelOfHarmonySystem.sbr" \
	"$(INTDIR)\KalimaGate.sbr" \
	"$(INTDIR)\Kanturu.sbr" \
	"$(INTDIR)\KanturuBattleOfMaya.sbr" \
	"$(INTDIR)\KanturuBattleOfNightmare.sbr" \
	"$(INTDIR)\KanturuBattleStanby.sbr" \
	"$(INTDIR)\KanturuBattleUserMng.sbr" \
	"$(INTDIR)\KanturuEntranceNPC.sbr" \
	"$(INTDIR)\KanturuMaya.sbr" \
	"$(INTDIR)\KanturuMonsterMng.sbr" \
	"$(INTDIR)\KanturuNightmare.sbr" \
	"$(INTDIR)\KanturuStateInfo.sbr" \
	"$(INTDIR)\KanturuTowerOfRefinement.sbr" \
	"$(INTDIR)\KanturuUtil.sbr" \
	"$(INTDIR)\LargeRand.sbr" \
	"$(INTDIR)\LifeStone.sbr" \
	"$(INTDIR)\LogProc.sbr" \
	"$(INTDIR)\LogToFile.sbr" \
	"$(INTDIR)\MagicDamage.sbr" \
	"$(INTDIR)\MagicInf.sbr" \
	"$(INTDIR)\MapClass.sbr" \
	"$(INTDIR)\MapItem.sbr" \
	"$(INTDIR)\MapServerManager.sbr" \
	"$(INTDIR)\Mercenary.sbr" \
	"$(INTDIR)\MonsterAttr.sbr" \
	"$(INTDIR)\MonsterHerd.sbr" \
	"$(INTDIR)\MonsterItemMng.sbr" \
	"$(INTDIR)\MonsterSetBase.sbr" \
	"$(INTDIR)\MoveCheck.sbr" \
	"$(INTDIR)\MoveCommand.sbr" \
	"$(INTDIR)\MultiAttackHackCheck.sbr" \
	"$(INTDIR)\MultiCheckSum.sbr" \
	"$(INTDIR)\MyWinsockBase.sbr" \
	"$(INTDIR)\NpcTalk.sbr" \
	"$(INTDIR)\NSerialCheck.sbr" \
	"$(INTDIR)\ObjAttack.sbr" \
	"$(INTDIR)\ObjBaseAttack.sbr" \
	"$(INTDIR)\ObjCalCharacter.sbr" \
	"$(INTDIR)\ObjUseSkill.sbr" \
	"$(INTDIR)\PacketCheckSum.sbr" \
	"$(INTDIR)\PartyClass.sbr" \
	"$(INTDIR)\protocol.sbr" \
	"$(INTDIR)\QuestInfo.sbr" \
	"$(INTDIR)\RingAttackEvent.sbr" \
	"$(INTDIR)\SendHackLog.sbr" \
	"$(INTDIR)\SetItemMacro.sbr" \
	"$(INTDIR)\SetItemOption.sbr" \
	"$(INTDIR)\Shop.sbr" \
	"$(INTDIR)\SkillAdditionInfo.sbr" \
	"$(INTDIR)\SkillDelay.sbr" \
	"$(INTDIR)\SkillHitBox.sbr" \
	"$(INTDIR)\SProtocol.sbr" \
	"$(INTDIR)\StatMng.sbr" \
	"$(INTDIR)\StdAfx.sbr" \
	"$(INTDIR)\TDurMagicKeyChecker.sbr" \
	"$(INTDIR)\TMonsterAI.sbr" \
	"$(INTDIR)\TMonsterAIAgro.sbr" \
	"$(INTDIR)\TMonsterAIAutomata.sbr" \
	"$(INTDIR)\TMonsterAIElement.sbr" \
	"$(INTDIR)\TMonsterAIGroup.sbr" \
	"$(INTDIR)\TMonsterAIGroupMember.sbr" \
	"$(INTDIR)\TMonsterAIMovePath.sbr" \
	"$(INTDIR)\TMonsterAIRule.sbr" \
	"$(INTDIR)\TMonsterAIRuleInfo.sbr" \
	"$(INTDIR)\TMonsterAIUnit.sbr" \
	"$(INTDIR)\TMonsterAIUtil.sbr" \
	"$(INTDIR)\TMonsterSkillElement.sbr" \
	"$(INTDIR)\TMonsterSkillElementInfo.sbr" \
	"$(INTDIR)\TMonsterSkillManager.sbr" \
	"$(INTDIR)\TMonsterSkillUnit.sbr" \
	"$(INTDIR)\TNotice.sbr" \
	"$(INTDIR)\TRandomPoolMgr.sbr" \
	"$(INTDIR)\TServerAlertManager.sbr" \
	"$(INTDIR)\TServerInfoDisplayer.sbr" \
	"$(INTDIR)\TStatistics.sbr" \
	"$(INTDIR)\TSync.sbr" \
	"$(INTDIR)\TUnion.sbr" \
	"$(INTDIR)\TUnionInfo.sbr" \
	"$(INTDIR)\user.sbr" \
	"$(INTDIR)\ViewportGuild.sbr" \
	"$(INTDIR)\Weapon.sbr" \
	"$(INTDIR)\WhisperCash.sbr" \
	"$(INTDIR)\winutil.sbr" \
	"$(INTDIR)\wsGameServer.sbr" \
	"$(INTDIR)\wsJoinServerCli.sbr" \
	"$(INTDIR)\wsShopServerCli.sbr" \
	"$(INTDIR)\WTEventItemList.sbr" \
	"$(INTDIR)\WzMemScript.sbr" \
	"$(INTDIR)\WzMultiCastSock.sbr" \
	"$(INTDIR)\WzQueue.sbr" \
	"$(INTDIR)\WZScriptEncode.sbr" \
	"$(INTDIR)\WzUdp.sbr" \
	"$(INTDIR)\zzzitem.sbr" \
	"$(INTDIR)\zzzmathlib.sbr"

"$(OUTDIR)\GameServer.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\GameServer.pdb" /map:"$(INTDIR)\GameServer.map" /debug /machine:I386 /out:"$(OUTDIR)\GameServer.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\AcceptIp.obj" \
	"$(INTDIR)\AttackEvent.obj" \
	"$(INTDIR)\BattleGround.obj" \
	"$(INTDIR)\BattleSoccer.obj" \
	"$(INTDIR)\BattleSoccerManager.obj" \
	"$(INTDIR)\BloodCastle.obj" \
	"$(INTDIR)\CallStackTrace.obj" \
	"$(INTDIR)\CannonTower.obj" \
	"$(INTDIR)\CashItemPeriodSystem.obj" \
	"$(INTDIR)\CashLotterySystem.obj" \
	"$(INTDIR)\CashShop.obj" \
	"$(INTDIR)\CastleCrown.obj" \
	"$(INTDIR)\CastleCrownSwitch.obj" \
	"$(INTDIR)\CastleDeepEvent.obj" \
	"$(INTDIR)\CastleSiege.obj" \
	"$(INTDIR)\CastleSiegeSync.obj" \
	"$(INTDIR)\CCastleEventItemList.obj" \
	"$(INTDIR)\ChaosBox.obj" \
	"$(INTDIR)\ChaosCastle.obj" \
	"$(INTDIR)\ChaosCastleSetItemDrop.obj" \
	"$(INTDIR)\ChinaHackUserKick.obj" \
	"$(INTDIR)\classdef.obj" \
	"$(INTDIR)\CLoginCount.obj" \
	"$(INTDIR)\ClosePlayer.obj" \
	"$(INTDIR)\ComboAttack.obj" \
	"$(INTDIR)\ConMember.obj" \
	"$(INTDIR)\Crywolf.obj" \
	"$(INTDIR)\CrywolfAltar.obj" \
	"$(INTDIR)\CrywolfDarkElf.obj" \
	"$(INTDIR)\CrywolfMonster.obj" \
	"$(INTDIR)\CrywolfStateTimeInfo.obj" \
	"$(INTDIR)\CrywolfStatue.obj" \
	"$(INTDIR)\CrywolfSync.obj" \
	"$(INTDIR)\CrywolfTanker.obj" \
	"$(INTDIR)\CrywolfUtil.obj" \
	"$(INTDIR)\CWhatsUpDummyServer.obj" \
	"$(INTDIR)\DarkSpirit.obj" \
	"$(INTDIR)\DbSave.obj" \
	"$(INTDIR)\DBSockMng.obj" \
	"$(INTDIR)\DevilSquare.obj" \
	"$(INTDIR)\DevilSquareGround.obj" \
	"$(INTDIR)\DirPath.obj" \
	"$(INTDIR)\DragonEvent.obj" \
	"$(INTDIR)\DSProtocol.obj" \
	"$(INTDIR)\EDSProtocol.obj" \
	"$(INTDIR)\EledoradoEvent.obj" \
	"$(INTDIR)\Event.obj" \
	"$(INTDIR)\EventManagement.obj" \
	"$(INTDIR)\GameEvent.obj" \
	"$(INTDIR)\GameMain.obj" \
	"$(INTDIR)\GameServer.obj" \
	"$(INTDIR)\GameServerAuth.obj" \
	"$(INTDIR)\Gate.obj" \
	"$(INTDIR)\giocp.obj" \
	"$(INTDIR)\GMMng.obj" \
	"$(INTDIR)\gObjMonster.obj" \
	"$(INTDIR)\Guardian.obj" \
	"$(INTDIR)\GuardianStatue.obj" \
	"$(INTDIR)\GuildClass.obj" \
	"$(INTDIR)\IpCache.obj" \
	"$(INTDIR)\ItemAddOption.obj" \
	"$(INTDIR)\ItemBag.obj" \
	"$(INTDIR)\ItemBagEx.obj" \
	"$(INTDIR)\ItemSystemFor380.obj" \
	"$(INTDIR)\JewelMixSystem.obj" \
	"$(INTDIR)\JewelOfHarmonySystem.obj" \
	"$(INTDIR)\KalimaGate.obj" \
	"$(INTDIR)\Kanturu.obj" \
	"$(INTDIR)\KanturuBattleOfMaya.obj" \
	"$(INTDIR)\KanturuBattleOfNightmare.obj" \
	"$(INTDIR)\KanturuBattleStanby.obj" \
	"$(INTDIR)\KanturuBattleUserMng.obj" \
	"$(INTDIR)\KanturuEntranceNPC.obj" \
	"$(INTDIR)\KanturuMaya.obj" \
	"$(INTDIR)\KanturuMonsterMng.obj" \
	"$(INTDIR)\KanturuNightmare.obj" \
	"$(INTDIR)\KanturuStateInfo.obj" \
	"$(INTDIR)\KanturuTowerOfRefinement.obj" \
	"$(INTDIR)\KanturuUtil.obj" \
	"$(INTDIR)\LargeRand.obj" \
	"$(INTDIR)\LifeStone.obj" \
	"$(INTDIR)\LogProc.obj" \
	"$(INTDIR)\LogToFile.obj" \
	"$(INTDIR)\MagicDamage.obj" \
	"$(INTDIR)\MagicInf.obj" \
	"$(INTDIR)\MapClass.obj" \
	"$(INTDIR)\MapItem.obj" \
	"$(INTDIR)\MapServerManager.obj" \
	"$(INTDIR)\Mercenary.obj" \
	"$(INTDIR)\MonsterAttr.obj" \
	"$(INTDIR)\MonsterHerd.obj" \
	"$(INTDIR)\MonsterItemMng.obj" \
	"$(INTDIR)\MonsterSetBase.obj" \
	"$(INTDIR)\MoveCheck.obj" \
	"$(INTDIR)\MoveCommand.obj" \
	"$(INTDIR)\MultiAttackHackCheck.obj" \
	"$(INTDIR)\MultiCheckSum.obj" \
	"$(INTDIR)\MyWinsockBase.obj" \
	"$(INTDIR)\NpcTalk.obj" \
	"$(INTDIR)\NSerialCheck.obj" \
	"$(INTDIR)\ObjAttack.obj" \
	"$(INTDIR)\ObjBaseAttack.obj" \
	"$(INTDIR)\ObjCalCharacter.obj" \
	"$(INTDIR)\ObjUseSkill.obj" \
	"$(INTDIR)\PacketCheckSum.obj" \
	"$(INTDIR)\PartyClass.obj" \
	"$(INTDIR)\protocol.obj" \
	"$(INTDIR)\QuestInfo.obj" \
	"$(INTDIR)\RingAttackEvent.obj" \
	"$(INTDIR)\SendHackLog.obj" \
	"$(INTDIR)\SetItemMacro.obj" \
	"$(INTDIR)\SetItemOption.obj" \
	"$(INTDIR)\Shop.obj" \
	"$(INTDIR)\SkillAdditionInfo.obj" \
	"$(INTDIR)\SkillDelay.obj" \
	"$(INTDIR)\SkillHitBox.obj" \
	"$(INTDIR)\SProtocol.obj" \
	"$(INTDIR)\StatMng.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\TDurMagicKeyChecker.obj" \
	"$(INTDIR)\TMonsterAI.obj" \
	"$(INTDIR)\TMonsterAIAgro.obj" \
	"$(INTDIR)\TMonsterAIAutomata.obj" \
	"$(INTDIR)\TMonsterAIElement.obj" \
	"$(INTDIR)\TMonsterAIGroup.obj" \
	"$(INTDIR)\TMonsterAIGroupMember.obj" \
	"$(INTDIR)\TMonsterAIMovePath.obj" \
	"$(INTDIR)\TMonsterAIRule.obj" \
	"$(INTDIR)\TMonsterAIRuleInfo.obj" \
	"$(INTDIR)\TMonsterAIUnit.obj" \
	"$(INTDIR)\TMonsterAIUtil.obj" \
	"$(INTDIR)\TMonsterSkillElement.obj" \
	"$(INTDIR)\TMonsterSkillElementInfo.obj" \
	"$(INTDIR)\TMonsterSkillManager.obj" \
	"$(INTDIR)\TMonsterSkillUnit.obj" \
	"$(INTDIR)\TNotice.obj" \
	"$(INTDIR)\TRandomPoolMgr.obj" \
	"$(INTDIR)\TServerAlertManager.obj" \
	"$(INTDIR)\TServerInfoDisplayer.obj" \
	"$(INTDIR)\TStatistics.obj" \
	"$(INTDIR)\TSync.obj" \
	"$(INTDIR)\TUnion.obj" \
	"$(INTDIR)\TUnionInfo.obj" \
	"$(INTDIR)\user.obj" \
	"$(INTDIR)\ViewportGuild.obj" \
	"$(INTDIR)\Weapon.obj" \
	"$(INTDIR)\WhisperCash.obj" \
	"$(INTDIR)\winutil.obj" \
	"$(INTDIR)\wsGameServer.obj" \
	"$(INTDIR)\wsJoinServerCli.obj" \
	"$(INTDIR)\wsShopServerCli.obj" \
	"$(INTDIR)\WTEventItemList.obj" \
	"$(INTDIR)\WzMemScript.obj" \
	"$(INTDIR)\WzMultiCastSock.obj" \
	"$(INTDIR)\WzQueue.obj" \
	"$(INTDIR)\WZScriptEncode.obj" \
	"$(INTDIR)\WzUdp.obj" \
	"$(INTDIR)\zzzitem.obj" \
	"$(INTDIR)\zzzmathlib.obj" \
	"$(INTDIR)\GameServer.res" \
	"..\..\MuMsg\Release\MuMsg.lib" \
	"..\SimpleModulus\Release\SimpleModulus.lib" \
	"..\GGSvr\Release\GGSvr.lib"

"$(OUTDIR)\GameServer.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("GameServer.dep")
!INCLUDE "GameServer.dep"
!ELSE 
!MESSAGE Warning: cannot find "GameServer.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "GameServer - Win32 Release" || "$(CFG)" == "GameServer - Win32 Debug" || "$(CFG)" == "GameServer - Win32 DebugRelease"
SOURCE=.\AcceptIp.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\AcceptIp.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\AcceptIp.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\AcceptIp.obj"	"$(INTDIR)\AcceptIp.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\AttackEvent.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\AttackEvent.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\AttackEvent.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\AttackEvent.obj"	"$(INTDIR)\AttackEvent.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\BattleGround.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\BattleGround.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\BattleGround.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\BattleGround.obj"	"$(INTDIR)\BattleGround.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\BattleSoccer.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\BattleSoccer.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\BattleSoccer.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\BattleSoccer.obj"	"$(INTDIR)\BattleSoccer.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\BattleSoccerManager.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\BattleSoccerManager.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\BattleSoccerManager.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\BattleSoccerManager.obj"	"$(INTDIR)\BattleSoccerManager.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\BloodCastle.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\BloodCastle.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\BloodCastle.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\BloodCastle.obj"	"$(INTDIR)\BloodCastle.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\CallStackTrace.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\CallStackTrace.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\CallStackTrace.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\CallStackTrace.obj"	"$(INTDIR)\CallStackTrace.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\CannonTower.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\CannonTower.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\CannonTower.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\CannonTower.obj"	"$(INTDIR)\CannonTower.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\CashItemPeriodSystem.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\CashItemPeriodSystem.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\CashItemPeriodSystem.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\CashItemPeriodSystem.obj"	"$(INTDIR)\CashItemPeriodSystem.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\CashLotterySystem.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\CashLotterySystem.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\CashLotterySystem.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\CashLotterySystem.obj"	"$(INTDIR)\CashLotterySystem.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\CashShop.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\CashShop.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\CashShop.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\CashShop.obj"	"$(INTDIR)\CashShop.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\CastleCrown.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\CastleCrown.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\CastleCrown.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\CastleCrown.obj"	"$(INTDIR)\CastleCrown.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\CastleCrownSwitch.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\CastleCrownSwitch.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\CastleCrownSwitch.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\CastleCrownSwitch.obj"	"$(INTDIR)\CastleCrownSwitch.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\CastleDeepEvent.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\CastleDeepEvent.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\CastleDeepEvent.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\CastleDeepEvent.obj"	"$(INTDIR)\CastleDeepEvent.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\CastleSiege.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\CastleSiege.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\CastleSiege.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\CastleSiege.obj"	"$(INTDIR)\CastleSiege.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\CastleSiegeSync.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\CastleSiegeSync.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\CastleSiegeSync.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\CastleSiegeSync.obj"	"$(INTDIR)\CastleSiegeSync.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\CCastleEventItemList.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\CCastleEventItemList.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\CCastleEventItemList.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\CCastleEventItemList.obj"	"$(INTDIR)\CCastleEventItemList.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\ChaosBox.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\ChaosBox.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\ChaosBox.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\ChaosBox.obj"	"$(INTDIR)\ChaosBox.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\ChaosCastle.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\ChaosCastle.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\ChaosCastle.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\ChaosCastle.obj"	"$(INTDIR)\ChaosCastle.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\ChaosCastleSetItemDrop.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\ChaosCastleSetItemDrop.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\ChaosCastleSetItemDrop.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\ChaosCastleSetItemDrop.obj"	"$(INTDIR)\ChaosCastleSetItemDrop.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\ChinaHackUserKick.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\ChinaHackUserKick.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\ChinaHackUserKick.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\ChinaHackUserKick.obj"	"$(INTDIR)\ChinaHackUserKick.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=..\common\classdef.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\classdef.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\classdef.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\classdef.obj"	"$(INTDIR)\classdef.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\CLoginCount.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\CLoginCount.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\CLoginCount.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\CLoginCount.obj"	"$(INTDIR)\CLoginCount.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\ClosePlayer.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\ClosePlayer.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\ClosePlayer.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\ClosePlayer.obj"	"$(INTDIR)\ClosePlayer.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\ComboAttack.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\ComboAttack.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\ComboAttack.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\ComboAttack.obj"	"$(INTDIR)\ComboAttack.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\ConMember.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\ConMember.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\ConMember.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\ConMember.obj"	"$(INTDIR)\ConMember.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\Crywolf.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\Crywolf.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\Crywolf.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\Crywolf.obj"	"$(INTDIR)\Crywolf.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\CrywolfAltar.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\CrywolfAltar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\CrywolfAltar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\CrywolfAltar.obj"	"$(INTDIR)\CrywolfAltar.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\CrywolfDarkElf.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\CrywolfDarkElf.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\CrywolfDarkElf.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\CrywolfDarkElf.obj"	"$(INTDIR)\CrywolfDarkElf.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\CrywolfMonster.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\CrywolfMonster.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\CrywolfMonster.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\CrywolfMonster.obj"	"$(INTDIR)\CrywolfMonster.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\CrywolfStateTimeInfo.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\CrywolfStateTimeInfo.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\CrywolfStateTimeInfo.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\CrywolfStateTimeInfo.obj"	"$(INTDIR)\CrywolfStateTimeInfo.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\CrywolfStatue.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\CrywolfStatue.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\CrywolfStatue.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\CrywolfStatue.obj"	"$(INTDIR)\CrywolfStatue.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\CrywolfSync.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\CrywolfSync.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\CrywolfSync.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\CrywolfSync.obj"	"$(INTDIR)\CrywolfSync.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\CrywolfTanker.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\CrywolfTanker.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\CrywolfTanker.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\CrywolfTanker.obj"	"$(INTDIR)\CrywolfTanker.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\CrywolfUtil.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\CrywolfUtil.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\CrywolfUtil.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\CrywolfUtil.obj"	"$(INTDIR)\CrywolfUtil.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\CWhatsUpDummyServer.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\CWhatsUpDummyServer.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\CWhatsUpDummyServer.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\CWhatsUpDummyServer.obj"	"$(INTDIR)\CWhatsUpDummyServer.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\DarkSpirit.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\DarkSpirit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\DarkSpirit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\DarkSpirit.obj"	"$(INTDIR)\DarkSpirit.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\DbSave.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\DbSave.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\DbSave.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\DbSave.obj"	"$(INTDIR)\DbSave.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\DBSockMng.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\DBSockMng.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\DBSockMng.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\DBSockMng.obj"	"$(INTDIR)\DBSockMng.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\DevilSquare.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\DevilSquare.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\DevilSquare.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\DevilSquare.obj"	"$(INTDIR)\DevilSquare.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\DevilSquareGround.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\DevilSquareGround.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\DevilSquareGround.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\DevilSquareGround.obj"	"$(INTDIR)\DevilSquareGround.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\DirPath.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\DirPath.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\DirPath.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\DirPath.obj"	"$(INTDIR)\DirPath.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\DragonEvent.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\DragonEvent.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\DragonEvent.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\DragonEvent.obj"	"$(INTDIR)\DragonEvent.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\DSProtocol.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\DSProtocol.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\DSProtocol.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\DSProtocol.obj"	"$(INTDIR)\DSProtocol.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\EDSProtocol.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\EDSProtocol.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\EDSProtocol.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\EDSProtocol.obj"	"$(INTDIR)\EDSProtocol.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\EledoradoEvent.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\EledoradoEvent.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\EledoradoEvent.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\EledoradoEvent.obj"	"$(INTDIR)\EledoradoEvent.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\Event.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\Event.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\Event.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\Event.obj"	"$(INTDIR)\Event.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\EventManagement.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\EventManagement.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\EventManagement.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\EventManagement.obj"	"$(INTDIR)\EventManagement.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\GameEvent.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\GameEvent.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\GameEvent.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\GameEvent.obj"	"$(INTDIR)\GameEvent.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\GameMain.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\GameMain.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\GameMain.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\GameMain.obj"	"$(INTDIR)\GameMain.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\GameServer.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\GameServer.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\GameServer.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\GameServer.obj"	"$(INTDIR)\GameServer.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\GameServer.rc

"$(INTDIR)\GameServer.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\GameServerAuth.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\GameServerAuth.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\GameServerAuth.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\GameServerAuth.obj"	"$(INTDIR)\GameServerAuth.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\Gate.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\Gate.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\Gate.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\Gate.obj"	"$(INTDIR)\Gate.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\giocp.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\giocp.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\giocp.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\giocp.obj"	"$(INTDIR)\giocp.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\GMMng.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\GMMng.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\GMMng.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\GMMng.obj"	"$(INTDIR)\GMMng.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\gObjMonster.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\gObjMonster.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\gObjMonster.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\gObjMonster.obj"	"$(INTDIR)\gObjMonster.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\Guardian.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\Guardian.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\Guardian.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\Guardian.obj"	"$(INTDIR)\Guardian.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\GuardianStatue.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\GuardianStatue.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\GuardianStatue.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\GuardianStatue.obj"	"$(INTDIR)\GuardianStatue.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\GuildClass.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\GuildClass.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\GuildClass.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\GuildClass.obj"	"$(INTDIR)\GuildClass.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\IpCache.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\IpCache.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\IpCache.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\IpCache.obj"	"$(INTDIR)\IpCache.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\ItemAddOption.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\ItemAddOption.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\ItemAddOption.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\ItemAddOption.obj"	"$(INTDIR)\ItemAddOption.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\ItemBag.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\ItemBag.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\ItemBag.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\ItemBag.obj"	"$(INTDIR)\ItemBag.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\ItemBagEx.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\ItemBagEx.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\ItemBagEx.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\ItemBagEx.obj"	"$(INTDIR)\ItemBagEx.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\ItemSystemFor380.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\ItemSystemFor380.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\ItemSystemFor380.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\ItemSystemFor380.obj"	"$(INTDIR)\ItemSystemFor380.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\JewelMixSystem.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\JewelMixSystem.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\JewelMixSystem.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\JewelMixSystem.obj"	"$(INTDIR)\JewelMixSystem.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\JewelOfHarmonySystem.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\JewelOfHarmonySystem.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\JewelOfHarmonySystem.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\JewelOfHarmonySystem.obj"	"$(INTDIR)\JewelOfHarmonySystem.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\KalimaGate.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\KalimaGate.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\KalimaGate.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\KalimaGate.obj"	"$(INTDIR)\KalimaGate.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\Kanturu.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\Kanturu.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\Kanturu.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\Kanturu.obj"	"$(INTDIR)\Kanturu.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\KanturuBattleOfMaya.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\KanturuBattleOfMaya.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\KanturuBattleOfMaya.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\KanturuBattleOfMaya.obj"	"$(INTDIR)\KanturuBattleOfMaya.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\KanturuBattleOfNightmare.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\KanturuBattleOfNightmare.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\KanturuBattleOfNightmare.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\KanturuBattleOfNightmare.obj"	"$(INTDIR)\KanturuBattleOfNightmare.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\KanturuBattleStanby.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\KanturuBattleStanby.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\KanturuBattleStanby.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\KanturuBattleStanby.obj"	"$(INTDIR)\KanturuBattleStanby.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\KanturuBattleUserMng.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\KanturuBattleUserMng.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\KanturuBattleUserMng.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\KanturuBattleUserMng.obj"	"$(INTDIR)\KanturuBattleUserMng.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\KanturuEntranceNPC.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\KanturuEntranceNPC.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\KanturuEntranceNPC.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\KanturuEntranceNPC.obj"	"$(INTDIR)\KanturuEntranceNPC.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\KanturuMaya.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\KanturuMaya.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\KanturuMaya.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\KanturuMaya.obj"	"$(INTDIR)\KanturuMaya.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\KanturuMonsterMng.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\KanturuMonsterMng.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\KanturuMonsterMng.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\KanturuMonsterMng.obj"	"$(INTDIR)\KanturuMonsterMng.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\KanturuNightmare.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\KanturuNightmare.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\KanturuNightmare.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\KanturuNightmare.obj"	"$(INTDIR)\KanturuNightmare.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\KanturuStateInfo.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\KanturuStateInfo.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\KanturuStateInfo.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\KanturuStateInfo.obj"	"$(INTDIR)\KanturuStateInfo.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\KanturuTowerOfRefinement.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\KanturuTowerOfRefinement.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\KanturuTowerOfRefinement.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\KanturuTowerOfRefinement.obj"	"$(INTDIR)\KanturuTowerOfRefinement.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\KanturuUtil.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\KanturuUtil.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\KanturuUtil.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\KanturuUtil.obj"	"$(INTDIR)\KanturuUtil.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\LargeRand.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\LargeRand.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\LargeRand.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\LargeRand.obj"	"$(INTDIR)\LargeRand.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\LifeStone.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\LifeStone.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\LifeStone.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\LifeStone.obj"	"$(INTDIR)\LifeStone.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\LogProc.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\LogProc.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\LogProc.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\LogProc.obj"	"$(INTDIR)\LogProc.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\LogToFile.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\LogToFile.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\LogToFile.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\LogToFile.obj"	"$(INTDIR)\LogToFile.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\MagicDamage.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\MagicDamage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\MagicDamage.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\MagicDamage.obj"	"$(INTDIR)\MagicDamage.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\MagicInf.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\MagicInf.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\MagicInf.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\MagicInf.obj"	"$(INTDIR)\MagicInf.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\MapClass.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\MapClass.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\MapClass.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\MapClass.obj"	"$(INTDIR)\MapClass.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\MapItem.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\MapItem.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\MapItem.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\MapItem.obj"	"$(INTDIR)\MapItem.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\MapServerManager.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\MapServerManager.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\MapServerManager.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\MapServerManager.obj"	"$(INTDIR)\MapServerManager.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\Mercenary.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\Mercenary.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\Mercenary.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\Mercenary.obj"	"$(INTDIR)\Mercenary.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\MonsterAttr.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\MonsterAttr.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\MonsterAttr.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\MonsterAttr.obj"	"$(INTDIR)\MonsterAttr.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\MonsterHerd.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\MonsterHerd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\MonsterHerd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\MonsterHerd.obj"	"$(INTDIR)\MonsterHerd.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\MonsterItemMng.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\MonsterItemMng.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\MonsterItemMng.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\MonsterItemMng.obj"	"$(INTDIR)\MonsterItemMng.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\MonsterSetBase.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\MonsterSetBase.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\MonsterSetBase.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\MonsterSetBase.obj"	"$(INTDIR)\MonsterSetBase.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\MoveCheck.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\MoveCheck.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\MoveCheck.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\MoveCheck.obj"	"$(INTDIR)\MoveCheck.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\MoveCommand.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\MoveCommand.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\MoveCommand.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\MoveCommand.obj"	"$(INTDIR)\MoveCommand.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\MultiAttackHackCheck.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\MultiAttackHackCheck.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\MultiAttackHackCheck.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\MultiAttackHackCheck.obj"	"$(INTDIR)\MultiAttackHackCheck.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\MultiCheckSum.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\MultiCheckSum.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\MultiCheckSum.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\MultiCheckSum.obj"	"$(INTDIR)\MultiCheckSum.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=..\common\MyWinsockBase.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\MyWinsockBase.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\MyWinsockBase.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\MyWinsockBase.obj"	"$(INTDIR)\MyWinsockBase.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\NpcTalk.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\NpcTalk.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\NpcTalk.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\NpcTalk.obj"	"$(INTDIR)\NpcTalk.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\NSerialCheck.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\NSerialCheck.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\NSerialCheck.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\NSerialCheck.obj"	"$(INTDIR)\NSerialCheck.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\ObjAttack.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\ObjAttack.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\ObjAttack.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\ObjAttack.obj"	"$(INTDIR)\ObjAttack.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\ObjBaseAttack.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\ObjBaseAttack.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\ObjBaseAttack.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\ObjBaseAttack.obj"	"$(INTDIR)\ObjBaseAttack.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\ObjCalCharacter.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\ObjCalCharacter.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\ObjCalCharacter.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\ObjCalCharacter.obj"	"$(INTDIR)\ObjCalCharacter.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\ObjUseSkill.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\ObjUseSkill.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\ObjUseSkill.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\ObjUseSkill.obj"	"$(INTDIR)\ObjUseSkill.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\PacketCheckSum.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\PacketCheckSum.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\PacketCheckSum.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\PacketCheckSum.obj"	"$(INTDIR)\PacketCheckSum.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\PartyClass.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\PartyClass.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\PartyClass.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\PartyClass.obj"	"$(INTDIR)\PartyClass.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\protocol.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\protocol.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\protocol.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\protocol.obj"	"$(INTDIR)\protocol.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\QuestInfo.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\QuestInfo.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\QuestInfo.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\QuestInfo.obj"	"$(INTDIR)\QuestInfo.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\RingAttackEvent.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\RingAttackEvent.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\RingAttackEvent.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\RingAttackEvent.obj"	"$(INTDIR)\RingAttackEvent.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\SendHackLog.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\SendHackLog.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\SendHackLog.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\SendHackLog.obj"	"$(INTDIR)\SendHackLog.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\SetItemMacro.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\SetItemMacro.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\SetItemMacro.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\SetItemMacro.obj"	"$(INTDIR)\SetItemMacro.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=..\common\SetItemOption.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\SetItemOption.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\SetItemOption.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\SetItemOption.obj"	"$(INTDIR)\SetItemOption.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\Shop.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\Shop.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\Shop.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\Shop.obj"	"$(INTDIR)\Shop.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\SkillAdditionInfo.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\SkillAdditionInfo.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\SkillAdditionInfo.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\SkillAdditionInfo.obj"	"$(INTDIR)\SkillAdditionInfo.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\SkillDelay.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\SkillDelay.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\SkillDelay.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\SkillDelay.obj"	"$(INTDIR)\SkillDelay.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\SkillHitBox.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\SkillHitBox.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\SkillHitBox.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\SkillHitBox.obj"	"$(INTDIR)\SkillHitBox.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\SProtocol.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\SProtocol.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\SProtocol.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\SProtocol.obj"	"$(INTDIR)\SProtocol.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\StatMng.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\StatMng.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\StatMng.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\StatMng.obj"	"$(INTDIR)\StatMng.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"

CPP_SWITCHES=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)\GameServer.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\GameServer.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"

CPP_SWITCHES=/nologo /MLd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)\GameServer.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\GameServer.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"

CPP_SWITCHES=/nologo /MT /W3 /GX /ZI /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FAcs /Fa"$(INTDIR)\\" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\GameServer.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\GameServer.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\TDurMagicKeyChecker.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\TDurMagicKeyChecker.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\TDurMagicKeyChecker.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\TDurMagicKeyChecker.obj"	"$(INTDIR)\TDurMagicKeyChecker.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\TMonsterAI.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\TMonsterAI.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\TMonsterAI.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\TMonsterAI.obj"	"$(INTDIR)\TMonsterAI.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\TMonsterAIAgro.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\TMonsterAIAgro.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\TMonsterAIAgro.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\TMonsterAIAgro.obj"	"$(INTDIR)\TMonsterAIAgro.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\TMonsterAIAutomata.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\TMonsterAIAutomata.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\TMonsterAIAutomata.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\TMonsterAIAutomata.obj"	"$(INTDIR)\TMonsterAIAutomata.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\TMonsterAIElement.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\TMonsterAIElement.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\TMonsterAIElement.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\TMonsterAIElement.obj"	"$(INTDIR)\TMonsterAIElement.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\TMonsterAIGroup.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\TMonsterAIGroup.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\TMonsterAIGroup.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\TMonsterAIGroup.obj"	"$(INTDIR)\TMonsterAIGroup.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\TMonsterAIGroupMember.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\TMonsterAIGroupMember.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\TMonsterAIGroupMember.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\TMonsterAIGroupMember.obj"	"$(INTDIR)\TMonsterAIGroupMember.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\TMonsterAIMovePath.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\TMonsterAIMovePath.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\TMonsterAIMovePath.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\TMonsterAIMovePath.obj"	"$(INTDIR)\TMonsterAIMovePath.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\TMonsterAIRule.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\TMonsterAIRule.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\TMonsterAIRule.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\TMonsterAIRule.obj"	"$(INTDIR)\TMonsterAIRule.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\TMonsterAIRuleInfo.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\TMonsterAIRuleInfo.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\TMonsterAIRuleInfo.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\TMonsterAIRuleInfo.obj"	"$(INTDIR)\TMonsterAIRuleInfo.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\TMonsterAIUnit.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\TMonsterAIUnit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\TMonsterAIUnit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\TMonsterAIUnit.obj"	"$(INTDIR)\TMonsterAIUnit.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\TMonsterAIUtil.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\TMonsterAIUtil.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\TMonsterAIUtil.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\TMonsterAIUtil.obj"	"$(INTDIR)\TMonsterAIUtil.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\TMonsterSkillElement.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\TMonsterSkillElement.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\TMonsterSkillElement.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\TMonsterSkillElement.obj"	"$(INTDIR)\TMonsterSkillElement.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\TMonsterSkillElementInfo.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\TMonsterSkillElementInfo.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\TMonsterSkillElementInfo.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\TMonsterSkillElementInfo.obj"	"$(INTDIR)\TMonsterSkillElementInfo.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\TMonsterSkillManager.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\TMonsterSkillManager.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\TMonsterSkillManager.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\TMonsterSkillManager.obj"	"$(INTDIR)\TMonsterSkillManager.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\TMonsterSkillUnit.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\TMonsterSkillUnit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\TMonsterSkillUnit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\TMonsterSkillUnit.obj"	"$(INTDIR)\TMonsterSkillUnit.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\TNotice.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\TNotice.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\TNotice.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\TNotice.obj"	"$(INTDIR)\TNotice.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\TRandomPoolMgr.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\TRandomPoolMgr.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\TRandomPoolMgr.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\TRandomPoolMgr.obj"	"$(INTDIR)\TRandomPoolMgr.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\TServerAlertManager.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\TServerAlertManager.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\TServerAlertManager.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\TServerAlertManager.obj"	"$(INTDIR)\TServerAlertManager.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\TServerInfoDisplayer.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\TServerInfoDisplayer.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\TServerInfoDisplayer.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\TServerInfoDisplayer.obj"	"$(INTDIR)\TServerInfoDisplayer.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\TStatistics.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\TStatistics.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\TStatistics.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\TStatistics.obj"	"$(INTDIR)\TStatistics.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=..\common\TSync.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\TSync.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\TSync.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\TSync.obj"	"$(INTDIR)\TSync.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\TUnion.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\TUnion.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\TUnion.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\TUnion.obj"	"$(INTDIR)\TUnion.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\TUnionInfo.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\TUnionInfo.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\TUnionInfo.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\TUnionInfo.obj"	"$(INTDIR)\TUnionInfo.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\user.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\user.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\user.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\user.obj"	"$(INTDIR)\user.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\ViewportGuild.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\ViewportGuild.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\ViewportGuild.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\ViewportGuild.obj"	"$(INTDIR)\ViewportGuild.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\Weapon.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\Weapon.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\Weapon.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\Weapon.obj"	"$(INTDIR)\Weapon.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\WhisperCash.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\WhisperCash.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\WhisperCash.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\WhisperCash.obj"	"$(INTDIR)\WhisperCash.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=..\common\winutil.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\winutil.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\winutil.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\winutil.obj"	"$(INTDIR)\winutil.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\wsGameServer.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\wsGameServer.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\wsGameServer.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\wsGameServer.obj"	"$(INTDIR)\wsGameServer.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\wsJoinServerCli.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\wsJoinServerCli.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\wsJoinServerCli.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\wsJoinServerCli.obj"	"$(INTDIR)\wsJoinServerCli.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\wsShopServerCli.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\wsShopServerCli.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\wsShopServerCli.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\wsShopServerCli.obj"	"$(INTDIR)\wsShopServerCli.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\WTEventItemList.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\WTEventItemList.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\WTEventItemList.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\WTEventItemList.obj"	"$(INTDIR)\WTEventItemList.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=..\common\WzMemScript.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\WzMemScript.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\WzMemScript.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\WzMemScript.obj"	"$(INTDIR)\WzMemScript.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\WzMultiCastSock.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\WzMultiCastSock.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\WzMultiCastSock.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\WzMultiCastSock.obj"	"$(INTDIR)\WzMultiCastSock.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=.\WzQueue.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\WzQueue.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\WzQueue.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\WzQueue.obj"	"$(INTDIR)\WzQueue.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=..\common\WZScriptEncode.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\WZScriptEncode.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\WZScriptEncode.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\WZScriptEncode.obj"	"$(INTDIR)\WZScriptEncode.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\WzUdp.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\WzUdp.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\WzUdp.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\WzUdp.obj"	"$(INTDIR)\WzUdp.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 

SOURCE=..\common\zzzitem.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\zzzitem.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\zzzitem.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\zzzitem.obj"	"$(INTDIR)\zzzitem.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\zzzmathlib.cpp

!IF  "$(CFG)" == "GameServer - Win32 Release"


"$(INTDIR)\zzzmathlib.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 Debug"


"$(INTDIR)\zzzmathlib.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ELSEIF  "$(CFG)" == "GameServer - Win32 DebugRelease"


"$(INTDIR)\zzzmathlib.obj"	"$(INTDIR)\zzzmathlib.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\GameServer.pch"


!ENDIF 


!ENDIF 

