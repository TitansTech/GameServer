#include "Compile.h"

#if (DSGN_COMPILE == 1)
//----------------------------------------------------
// Read connection string from SCF_Connect.ini
//----------------------------------------------------
// 0 : Dissabled
// 1 : Enabled
#define CONNECT_PARAMS_IN_INI					0

//----------------------------------------------------
// Dark Side of MU item shift (Do not send to others)
//----------------------------------------------------
#define DSGN_ITEM_DBMOVE						1	//Enable (0 - off/1 - on)

//----------------------------------------------------
// Dark Side of MU item drop rate for party
//----------------------------------------------------
#define DSGN_DROP_PARTYEXTRA					1	//Enable (0 - off/1 - on)

//----------------------------------------------------
// Dark Side of MU item cleanup (Do not send to others)
// Enable removal of Full Option Rings,Pendants,Wings
//----------------------------------------------------
#define DSGN_ITEM_MONITOR						0	//Enable Monitor (0 - off/1 - on)

#define DSGN_ITEM_MON_RING_LVL					9	//Monitor Rings Level
#define DSGN_ITEM_MON_RING_EXC					4	//Monitor # of exc options on rings
#define DSGN_ITEM_MON_RING_Z28					5	//Monitor Z28 max number on rings
#define DSGN_ITEM_FIX_RING						0	//Fix detected rings

#define DSGN_ITEM_MON_WING_EXC					3	//Monitor # of exc options on wings
#define DSGN_ITEM_FIX_WING						0	//Fix detected wings

//----------------------------------------------------
// Dark Side of MU ingame reset system
// (Disables usage of SCF Reset system)
//----------------------------------------------------
#define DSGN_RESET_SYSTEM						1	//Reset System (0 - off/1 - on)

//----------------------------------------------------
// Dark Side of MU test tools
//----------------------------------------------------
#define DSGN_EFFECT_CMD							1

//----------------------------------------------------------
// SHOW PROXY WINDOW
#define PROXY_SHOW								1
//----------------------------------------------------------
//Comands:
// /index 14000		(14000 = PLAYER INDEX)
// /showrecv 1		(1 Show DataRecv, 0 Dont Show DataRecv)
// /showsend 1		(1 Show DataSend, 0 Dont Show DataSend)
//----------------------------------------------------------
// for packets only write the packet like:
// c1 03 04			(With Spaces between each byte)
//----------------------------------------------------------
#else
//----------------------------------------------------
// Read connection string from SCF_Connect.ini
//----------------------------------------------------
// 0 : Dissabled
// 1 : Enabled
#define CONNECT_PARAMS_IN_INI					1

#define DSGN_ITEM_DBMOVE						0	//Enable (0 - off/1 - on)
#define DSGN_DROP_PARTYEXTRA					0	//Enable (0 - off/1 - on)
#define DSGN_ITEM_MONITOR						0	//Enable Monitor (0 - off/1 - on)
#define DSGN_ITEM_MON_RING_LVL					0	//Monitor Rings Level
#define DSGN_ITEM_MON_RING_EXC					0	//Monitor # of exc options on rings
#define DSGN_ITEM_MON_RING_Z28					0	//Monitor Z28 max number on rings
#define DSGN_ITEM_FIX_RING						0	//Fix detected rings
#define DSGN_ITEM_MON_WING_EXC					0	//Monitor # of exc options on wings
#define DSGN_ITEM_FIX_WING						0	//Fix detected wings
#define DSGN_RESET_SYSTEM						0	//Reset System (0 - off/1 - on)
#define DSGN_EFFECT_CMD							0
#define PROXY_SHOW								0
#endif

