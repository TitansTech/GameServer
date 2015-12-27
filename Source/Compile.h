//----------------------------------------------------
// Windows 2003 SP2 Fix TRY
//----------------------------------------------------
// 0 : Dissabled
// 1 : Enabled
#define W2K3SP2_FIX								0

// GameServer Build
//----------------------------------------------------
// 0 : As Normal Game Server
// 1 : As Castle Siege Game Server
#define GS_CASTLE								1
#define DSGN_COMPILE							0
#define GS_CASTLE_NOEVENTS						0
#define GS_CASTLE_NOLOGSAVE						0

//----------------------------------------------------
// WinLicense Protection
//----------------------------------------------------
// 0 : Dissabled
// 1 : Enabled
#define WL_PROTECT	0
#define MAC_PROTECT	0
#define MEM_PROTECT	0
#define MEM_PROTECT_DEBUG						0

//----------------------------------------------------
// MAC Protection
//----------------------------------------------------
// 0 : Dissabled
// 1 : Enabled
#define MAC_PROTECT_OLD							0

//----------------------------------------------------
// WinDbg Debug info
//----------------------------------------------------
// 0 : Dissabled
// 1 : Enabled
#define ShowWinDbgDebugInfo						0

//----------------------------------------------------
// S6E2 And Global Protocol
//----------------------------------------------------
// 0 : Dissabled
// 1 : Enabled
#define IsS6E2_And_EngProtocol_Enabled			1
//----------------------------------------------------
// Pack Edition
//----------------------------------------------------
// 0 : Basic
// 1 : Lite
// 2 : Medium
// 3 : Advanced
#define PACK_EDITION							3

//----------------------------------------------------
// Crystal Edition (New Wings + New Jewels)
//----------------------------------------------------
// 0 : Dissabled
// 1 : Enabled
#define CRYSTAL_EDITION							1
#if (CRYSTAL_EDITION==1)
#define CRYSTAL_PING							0
#define CRYSTAL_MOB_BAR							0
#else
#define CRYSTAL_PING							0
#define CRYSTAL_MOB_BAR							0
#endif


//----------------------------------------------------
// To SCFExDB
//----------------------------------------------------
// 0 : Dissabled
// 1 : Enabled
#define TOEXDB									0

//----------------------------------------------------
// To BORDS
//----------------------------------------------------
// 0 : Dissabled
// 1 : Enabled
#define BORDS									1

#define GS_OLDSTYLE								0
#define ENABLE_CSHOP							1
#define spliterVar								10
#define ISEX700									0

