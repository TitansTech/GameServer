/****************************************************************************** 
  Header: WinLicenseSDK.h
  Description: SDK header definition for the C/C++ language

  Author/s: Oreans Technologies  
  (c) 2010 Oreans Technologies
*****************************************************************************/ 

#pragma once

#ifdef _WIN64
 #pragma comment(lib, "Extra\\WinLicenseSDK64.lib")
#else
 #pragma comment(lib, "Extra\\WinLicenseSDK32.lib")
#endif

#include "SecureEngineSDK.h"


// ***********************************************
// WinLicense constants definition
// ***********************************************

// Trial extension constants 

const int wlNoTrialExt                  = 0;
const int wlAppExtended                 = 1;
const int wlInvalidTrialExt             = 2;
const int wlNoMoreExt                   = 3;

// Trial status constants

const int wlTrialOk                     = 0;
const int wlTrialDaysExpired            = 1;
const int wlTrialExecExpired            = 2;
const int wlTrialDateExpired            = 3;
const int wlTrialRuntimExpired          = 4;
const int wlTrialGlobalExpired          = 5;
const int wlTrialInvalidCountry         = 6;
const int wlTrialManipulated            = 7;

// Registration status constants

const int wlIsTrial                     = 0;
const int wlIsRegistered                = 1;
const int wlInvalidLicense              = 2;
const int wlInvalidHardwareLicense      = 3;
const int wlNoMoreHwdChanges            = 4;
const int wlLicenseExpired              = 5;
const int wlInvalidCountryLicense       = 6;
const int wlLicenseStolen               = 7;
const int wlWrongLicenseExp             = 8;
const int wlWrongLicenseHardware        = 9;
const int wlIsRegisteredNotActivared    = 10;
const int wlIsRegisteredAndActivated    = wlIsRegistered;
const int wlNoMoreInstancesAllowed      = 12;
const int wlNetworkNoServerRunning      = 13;
const int wlInstallLicenseDateExpired   = 14;
const int wlLicenseDisabledInstance     = 15;
const int wlTrialModeNotSupported       = 16;

// Registration status (extended) constants

const int wlLicenseDaysExpired          = 1;
const int wlLicenseExecExpired          = 2;
const int wlLicenseDateExpired          = 3;
const int wlLicenseGlobalExpired        = 4;
const int wlLicenseRuntimeExpired       = 5;
const int wlLicenseActivationExpired    = 6;

// Invalid key constants

const int wlMarkStolenKey               = 0;
const int wlMarkInvalidKey              = 1;

// License restrictions constants

const int wlRegRestrictionDays          = 1 << 0;
const int wlRegRestrictionExec          = 1 << 1;
const int wlRegRestrictionDate          = 1 << 2;
const int wlRegRestrictionRuntime       = 1 << 3;
const int wlRegRestrictionGlobalTime    = 1 << 4;
const int wlRegRestrictionCountry       = 1 << 5;
const int wlRegRestrictionHardwareId    = 1 << 6;
const int wlRegRestrictionNetwork       = 1 << 7;
const int wlRegRestrictionInstallDate   = 1 << 8;
const int wlRegRestrictionCreationDate  = 1 << 9;
const int wlRegRestrictionEmbedUserInfo = 1 << 10;

// Trial restrictions constants

const int wlTrialRestrictionUnlimited   = 0;
const int wlTrialRestrictionDays        = 1 << 0;
const int wlTrialRestrictionExec        = 1 << 1;
const int wlTrialRestrictionDate        = 1 << 2;
const int wlTrialRestrictionRuntime     = 1 << 3;
const int wlTrialRestrictionGlobalTime  = 1 << 4;
const int wlTrialRestrictionCountry     = 1 << 5;

// Miscellaneous constants 

const int wlPermKey                     = -1;
const int wlNoKey                       = -2;
const int wlNoTrialDate                 = -1;
const int wlInvalidCounter              = -1;

// Hardware Machine types

const int wlHardwareTypePC              = 0;
const int wlHardwareTypeU3              = 1;
const int wlHardwareTypeCustom          = 2;


// ***********************************************
// WinLicense typedef definition
// ***********************************************

typedef struct _sLicenseFeatures 
{ 
    unsigned    cb;                     // size of struct
    unsigned    NumDays;                // expiration days
    unsigned    NumExec;                // expiration executions
    SYSTEMTIME  ExpDate;                // expiration date 
    unsigned    CountryId;              // country ID
    unsigned    Runtime;                // expiration runtime
    unsigned    GlobalMinutes;          // global time expiration
    SYSTEMTIME  InstallDate;            // Date to install the license since it was created
    unsigned    NetInstances;           // Network instances via shared file
    unsigned    EmbedLicenseInfoInKey;  // for Dynamic SmartKeys, it embeds Name+Company+Custom inside generated SmartKey
    unsigned    EmbedCreationDate;      // Embed the date that the key was created
} sLicenseFeatures;


// ***********************************************
// WinLicense functions prototype
// ***********************************************

#ifdef __cplusplus
extern "C" {
#endif

int __stdcall GenerateTrialExtensionKey(char* TrialHash, int Level, int NumDays, int NumExec,\
                                        SYSTEMTIME* NewDate, int NumMinutes, int TimeRuntime,\
                                        char* BufferOut);

int __stdcall WLGenTrialExtensionFileKey(char* TrialHash, int Level,\
                                         int NumDays, int NumExec, SYSTEMTIME* NewDate, int NumMinutes,\
                                         int TimeRuntime, char* BufferOut);

int __stdcall WLGenTrialExtensionRegistryKey(char* TrialHash, int Level,\
                                             int NumDays, int NumExec, SYSTEMTIME* NewDate, int NumMinutes,\
                                             int TimeRuntime, char* pKeyName, char* pKeyValueName, char* BufferOut);

int __stdcall WLGenPassword(char* TrialHash, char* Name, char* BufferOut);

int __stdcall WLGenLicenseFileKey(char* LicenseHash, char* UserName, char* Organization,\
                                  char* CustomData, char* MachineID, int NumDays, int NumExec, SYSTEMTIME* NewDate, \
                                  int CountryId, int Runtime, int GlobalMinutes, char* BufferOut);

int __stdcall WLGenLicenseFileKeyW(const wchar_t* LicenseHash, const wchar_t* UserName, const wchar_t* Organization,\
                                   const wchar_t* CustomData, const wchar_t* MachineID, int NumDays, int NumExec,\
                                   SYSTEMTIME* NewDate, int CountryId, int Runtime, int GlobalMinutes, char* BufferOut);

int __stdcall WLGenLicenseFileKeyEx(const char* LicenseHash, const char* UserName, const char* Organization,\
                                    const char* CustomData, const char* MachineID, sLicenseFeatures* LicenseFeatures,\
                                    char* BufferOut);

int __stdcall WLGenLicenseFileKeyExW(const wchar_t* LicenseHash, const wchar_t* UserName, const wchar_t* Organization,\
                                     const wchar_t* CustomData, const wchar_t* MachineID, sLicenseFeatures* LicenseFeatures,\
                                     char* BufferOut);

int __stdcall WLGenLicenseRegistryKey(char* LicenseHash, char* UserName, char* Organization,\
                                      char* CustomData, char* MachineID, int NumDays, int NumExec, SYSTEMTIME* NewDate,\
                                      int CountryId, int Runtime, int GlobalMinutes, char* KeyName, char* KeyValueName,\
                                      char* BufferOut);

int __stdcall WLGenLicenseRegistryKeyW(const wchar_t* LicenseHash, const wchar_t* UserName, const wchar_t* Organization,\
                                       const wchar_t* CustomData, const wchar_t* MachineID, int NumDays, int NumExec,\
                                       SYSTEMTIME* NewDate, int CountryId, int Runtime, int GlobalMinutes,\
                                       const wchar_t* KeyName, const wchar_t* KeyValueName, char* BufferOut);

int __stdcall WLGenLicenseRegistryKeyEx(const char* LicenseHash, const char* UserName, const char* Organization,\
                                        const char* CustomData, const char* MachineID, sLicenseFeatures* LicenseFeatures,\
                                        char* KeyName, char* KeyValueName, char* BufferOut);

int __stdcall WLGenLicenseRegistryKeyExW(const wchar_t* LicenseHash, const wchar_t* UserName, const wchar_t* Organization,\
                                         const wchar_t* CustomData, const wchar_t* MachineID, sLicenseFeatures* LicenseFeatures,\
                                         const wchar_t* KeyName, const wchar_t* KeyValueName, wchar_t* BufferOut);

int __stdcall WLGenLicenseTextKey(char* LicenseHash, char* UserName, char* Organization, char* CustomData,\
                                  char* MachineID, int NumDays, int NumExec, SYSTEMTIME* NewDate,\
                                  int CountryId, int Runtime, int GlobalMinutes, char* BufferOut);

int __stdcall WLGenLicenseTextKeyW(const wchar_t* LicenseHash, const wchar_t* UserName, const wchar_t* Organization,\
                                   const wchar_t* CustomData, const wchar_t* MachineID, int NumDays, int NumExec,\
                                   SYSTEMTIME* NewDate, int CountryId, int Runtime, int GlobalMinutes, wchar_t* BufferOut);

int __stdcall WLGenLicenseTextKeyEx(const char* LicenseHash, const char* UserName, const char* Organization,\
                                    const char* CustomData, const char* MachineID, sLicenseFeatures* LicenseFeatures,\
                                    char* BufferOut);

int __stdcall WLGenLicenseTextKeyExW(const wchar_t* LicenseHash, const wchar_t* UserName, const wchar_t* Organization,\
                                     const wchar_t* CustomData, const wchar_t* MachineID, sLicenseFeatures* LicenseFeatures,\
                                     wchar_t* BufferOut);

int __stdcall WLGenLicenseSmartKey(char* LicenseHash, char* UserName, char* Organization, char* CustomData,\
                                   char* MachineID, int NumDays, int NumExec, SYSTEMTIME* NewDate, char* BufferOut);

int __stdcall WLGenLicenseSmartKeyW(wchar_t* LicenseHash, wchar_t* UserName, wchar_t* Organization, wchar_t* CustomData,\
                                    wchar_t* MachineID, int NumDays, int NumExec, SYSTEMTIME* NewDate, wchar_t* BufferOut);

int __stdcall WLGenLicenseDynSmartKey(const char* LicenseHash, const char* UserName, const char* Organization,\
                                      const char* CustomData, const char* MachineID, sLicenseFeatures* LicenseFeatures,\
                                      char* BufferOut);

int __stdcall WLGenLicenseDynSmartKeyW(const wchar_t* LicenseHash, const wchar_t* UserName, const wchar_t* Organization,\
                                       const wchar_t* CustomData, const wchar_t* MachineID, sLicenseFeatures* LicenseFeatures,\
                                       wchar_t* BufferOut);

int __stdcall WLRegGetStatus(int* pExtendedInfo);

int __stdcall WLTrialGetStatus(int* pExtendedInfo);

int __stdcall WLTrialExtGetStatus(void);

BOOL __stdcall WLRegGetLicenseInfo(char* pName, char* pCompanyName, char* pCustomData);

BOOL __stdcall WLRegGetLicenseInfoW(wchar_t* pName, wchar_t* pCompanyName, wchar_t* pCustomData);

int __stdcall WLTrialTotalDays(void);

int __stdcall WLTrialTotalExecutions(void);

int __stdcall WLTrialDaysLeft(void);

int __stdcall WLTrialExecutionsLeft(void);

int __stdcall WLTrialExpirationDate(SYSTEMTIME* pExpDate);

int __stdcall WLTrialGlobalTimeLeft(void);

int __stdcall WLTrialRuntimeLeft(void);

int __stdcall WLTrialLockedCountry(void);

int __stdcall WLRegDaysLeft(void);

int __stdcall WLRegExecutionsLeft(void);

int __stdcall WLRegExpirationDate(SYSTEMTIME* pExpDate);

BOOL __stdcall WLRegLicenseCreationDate(SYSTEMTIME* pCreationDate);

int __stdcall WLRegTotalExecutions(void);

int __stdcall WLRegTotalDays(void);

BOOL __stdcall WLHardwareGetID(char* pHardwareId);

BOOL __stdcall WLHardwareCheckID(char* pHardwareId);

BOOL __stdcall WLRegSmartKeyCheck(const char* UserName, const char* Organization, const char* Custom, const char* AsciiKey);

BOOL __stdcall WLRegSmartKeyCheckW(const wchar_t* UserName, const wchar_t* Organization, const wchar_t* Custom,\
                                   const wchar_t* AsciiKey);

BOOL __stdcall WLRegNormalKeyCheck(const char* AsciiKey);

BOOL __stdcall WLRegNormalKeyCheckW(const wchar_t* AsciiKey);

BOOL __stdcall WLRegNormalKeyInstallToFile(const char* AsciiKey);

BOOL __stdcall WLRegNormalKeyInstallToFileW(const wchar_t* AsciiKey);

BOOL __stdcall WLRegNormalKeyInstallToRegistry(const char* AsciiKey);

BOOL __stdcall WLRegNormalKeyInstallToRegistryW(const wchar_t* AsciiKey);

BOOL __stdcall WLRegSmartKeyInstallToFile(const char* UserName, const char* Organization, const char* Custom,\
                                          const char* AsciiKey);

BOOL __stdcall WLRegSmartKeyInstallToRegistry(const char* UserName, const char* Organization, const char* Custom,\
                                              const char* AsciiKey);

BOOL __stdcall WLRegSmartKeyInstallToFileW(const wchar_t* UserName, const wchar_t* Organization, const wchar_t* Custom,\
                                            const wchar_t* AsciiKey);

BOOL __stdcall WLRegSmartKeyInstallToRegistryW(const wchar_t* UserName, const wchar_t* Organization, const wchar_t* Custom,\
                                               const wchar_t* AsciiKey);

int __stdcall WLTrialCustomCounterInc(int Value, int CounterId);

int __stdcall WLTrialCustomCounterDec(int Value, int CounterId);

int __stdcall WLTrialCustomCounter(int CounterId);

int __stdcall WLTrialCustomCounterSet(int Value, int CounterId);

BOOL __stdcall WLRestartApplication(void);

int __stdcall WLRegLockedCountry(void);

int __stdcall WLRegRuntimeLeft(void);

int __stdcall WLRegGlobalTimeLeft(void);

BOOL __stdcall WLRegDisableCurrentKey(int DisableFlags);

BOOL __stdcall WLRegRemoveCurrentKey(void);

BOOL __stdcall WLHardwareGetFormattedID(int BlockCharSize, int Uppercase, char* Buffer);

BOOL __stdcall WLPasswordCheck(char* UserName, char* Password);

BOOL __stdcall WLTrialExpireTrial(void);

char* __stdcall WLStringDecrypt(char* pString);

wchar_t* __stdcall WLStringDecryptW(wchar_t* pString);

void __stdcall WLRegLicenseName(char* FileKeyName, char* RegKeyName, char* RegKeyValueName);

BOOL __stdcall WLRestartApplicationArgs(char* pArgs);

BOOL __stdcall WLActGetInfo(int* Custom1, int* Custom2, int* Custom3);

BOOL __stdcall WLActCheck(char* ActivationCode);

BOOL __stdcall WLActInstall(char* ActivationCode);

BOOL __stdcall WLActExpirationDate(SYSTEMTIME* pExpDate);

int __stdcall WLActDaysToActivate(void);

BOOL __stdcall WLActUninstall(void);

BOOL __stdcall WLRegGetLicenseHardwareID(char* pHardwareId);

int __stdcall WLGetCurrentCountry(void);

int __stdcall WLTrialExtGetLevel(void);

BOOL __stdcall WLProtectCheckDebugger(void);

BOOL __stdcall  WLTrialExtendExpiration(int NumDays, int NumExec, SYSTEMTIME* NewDate, int Runtime, int GlobalMinutes);

BOOL __stdcall WLTrialFirstRun(void);

BOOL __stdcall WLRegFirstRun(void);

BOOL __stdcall WLRegCheckMachineLocked(void);

void __stdcall WLSplashHide(void);

void __stdcall WLBufferCrypt(void* Buffer, int BufferLength, char* Password);

void __stdcall WLBufferDecrypt(void* Buffer, int BufferLength, char* Password);

BOOL __stdcall WLRegSmartKeyInstallToFileInFolder(const char* UserName, const char* Organization, const char* Custom,\
                                                  const char* AsciiKey, const char* FilePath);
int __stdcall WLTrialDateDaysLeft(void);

int __stdcall WLRegDateDaysLeft(void);

void __stdcall WLResetLicenseFeatures(sLicenseFeatures *LicenseFeatures, int SizeStructure);

BOOL __stdcall WLRegGetDynSmartKey(char* SmartKey);

BOOL __stdcall WLRegDisableKeyCurrentInstance(void);

BOOL __stdcall WLHardwareRuntimeCheckU3(void);

void __stdcall WLGetVersion(char* Buffer);

int __stdcall WLRegNetInstancesGet(void);

int __stdcall WLRegNetInstancesMax(void);

void __stdcall WLGetProtectionDate(SYSTEMTIME* pProtectionDate);

BOOL __stdcall WLProtectCheckCodeIntegrity(void);

int __stdcall WLHardwareGetIdType(void);

BOOL __stdcall WLTrialStringRead(const char *StringName, char *StringValue);

BOOL __stdcall WLTrialStringReadW(const wchar_t *StringName, wchar_t *StringValue);

BOOL __stdcall WLTrialStringWrite(const char *StringName, const char *StringValue);

BOOL __stdcall WLTrialStringWriteW(const wchar_t *StringName, const wchar_t *StringValue);

BOOL __stdcall WLTrialDebugCheck(void);

BOOL __stdcall WLRegExpirationTimestamp(LPFILETIME lpFileTime);

BOOL __stdcall WLTrialExpirationTimestamp(LPFILETIME lpFileTime);

int __stdcall WLRegGetLicenseRestrictions(void);

int __stdcall WLRegGetLicenseType(void);

BOOL __stdcall WLCheckVirtualPC(void);

BOOL __stdcall WLHardwareGetIDW(wchar_t * pHardwareId);

int __stdcall WLTrialGetTrialRestrictions(void);

#ifdef __cplusplus
}
#endif


