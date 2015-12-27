#ifndef NPCTALK_H
#define NPCTALK_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#define MAX_NPC_PHANTOM_BUFFER	180



struct GC_ANS_NPC_TALK_V1
{
	PHEADB	PHeader;	// C3:30
	BYTE NpcNumber;
	BYTE Param1;
	BYTE Param2;
	BYTE Param3;
	BYTE Param4;
};

struct GC_ANS_NPC_TALK_V2
{
	PHEADB	PHeader;	// C3:30
	BYTE NpcNumber;
	BYTE Param1;
	BYTE Param2;
	BYTE Param3;
	BYTE Param4;
	BYTE Param5;
	BYTE Param6;
};

BOOL NpcTalk(LPOBJ lpNpc, LPOBJ lpObj);

int NpcBattleAnnouncer(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcFasi(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcGuildMasterTalk(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcLorenciaGuard(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcChaosGoblin(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcRusipher(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcPotionsGirl(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcDeviasWizard(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcDeviasGuard(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcDeviasWareHousemen(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcWarehouse(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcNoriaRara(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcDeviasMadam(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcEventChipNPC(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcLorenciaSmith(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcNoriJangIn(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcQuestCheck(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcServerDivision(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcRoadMerchant(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcAngelKing(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcAngelMessanger(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcAlchemist(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcLorenciaMadam(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcDeviasWeapon(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcDarkSpiritTrainer(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcJewelMixDealer(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcSiegeMachine_Attack(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcSiegeMachine_Defense(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcElderCircle(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcCastleGuard(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcCastleGateLever(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcCastleCrown(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcCastleTrialsGuard(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcCastleSwitch(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcShadowPhantom(LPOBJ lpObj);
#if (PACK_EDITION>=2)
BOOL NpcLittleSanta(LPOBJ lpNpc, LPOBJ lpObj);
#endif
BOOL NpcMainatenceMachine(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcReira(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcChaosCardMaster(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcPriestDevin(LPOBJ lpNpc, LPOBJ lpObj);		//406
BOOL NpcWerewolfQuarrel(LPOBJ lpNpc, LPOBJ lpObj);	//407
BOOL NpcKeepergate(LPOBJ lpNpc, LPOBJ lpObj);		//408
BOOL NpcLuckyCoins(LPOBJ lpNpc, LPOBJ lpObj);
//BOOL NpcLuckyItems(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcImperialGuardian(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcDoubleGoer(LPOBJ lpNpc, LPOBJ lpObj);

void QuestTeleport(int aIndex, int Type);
#endif