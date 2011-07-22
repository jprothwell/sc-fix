////////////////////////////////////////////////////////////////////////////////
//        Copyright (C) 2002-2010, Coolsand Technologies, Inc.
//                       All Rights Reserved
// 
// This source code is property of Coolsand. The information contained in this
// file is confidential. Distribution, reproduction, as well as exploitation,
// or transmission of any content of this file is not allowed except if
// expressly permitted.Infringements result in damage claims!
//
// FILENAME: cfw_sim_chv.h
// 
// DESCRIPTION:
// TODO: ...
// 
// REVISION HISTORY:
//    NAME              DATE                REMAKS
// Administrator      2010-8-28       Created initial version 1.0
//
////////////////////////////////////////////////////////////////////////////////
 

#ifndef CFW_SIM_CHV_H
#define CFW_SIM_CHV_H

// The following lines define status of state machines for get authentication function
#define SM_GETAUTH_INIT         0xFF
#define SM_GETAUTH_IDLE         0x01
#define SM_GETAUTH_MMI          0x02
#define SM_GETAUTH_STACK        0x03
#define SM_GETAUTH_DEDICATED    0x04
#define SM_GETAUTH_REHABILITATE 0x05

typedef struct _CFW_GETAUTH
{
  UINT8 nSm_GetAuth_prevStatus;
  UINT8 nSm_GetAuth_currStatus;
} CFW_GETAUTH;

// The following lines define status of state machines for enter authentication function
#define SM_ENTAUTH_INIT         0xFF
#define SM_ENTAUTH_IDLE         0x01
#define SM_ENTAUTH_MMI          0x02
#define SM_ENTAUTH_STACK        0x03
#define SM_ENTAUTH_DEDICATED    0x04
#define SM_ENTAUTH_UNBLOCKCHV   0x05
#define SM_ENTAUTH_VERIFYCHV    0x06
#define SM_ENTAUTH_REHABILITATE 0x07

typedef struct _CFW_ENTERAUTH
{
  UINT8 pPinPwd[8];
  UINT8 pNewPwd[8];
  UINT8 nOption;

  UINT8 nSm_EntAuth_prevStatus;
  UINT8 nSm_EntAuth_currStatus;
} CFW_ENTERAUTH;

// The following lines define status of state machines for change password function
#define SM_CHGPW_INIT           0xFF
#define SM_CHGPW_IDLE           0x01
#define SM_CHGPW_MMI            0x02
#define SM_CHGPW_STACK          0x03
#define SM_CHGPW_CHANGECHV      0x04

typedef struct _CFW_CHANGEPW
{
  UINT16 nFac;
  UINT8 nCHVNumber;
  UINT8 pOldPwd[8];
  UINT8 pNewPwd[8];

  UINT8 nSm_ChgPw_prevStatus;
  UINT8 nSm_ChgPw_currStatus;

} CFW_CHANGEPW;

// The following lines define status of state machines for get facility function
#define SM_GETFAC_INIT          0xFF
#define SM_GETFAC_IDLE          0x01
#define SM_GETFAC_MMI           0x02
#define SM_GETFAC_STACK         0x03
#define SM_GETFAC_DEDICATED     0x04
#define SM_GETFAC_ELEMENTARY    0x05

typedef struct _CFW_GETFACILITY
{
  UINT16 nFac;

  UINT8 nSm_GetFac_prevStatus;
  UINT8 nSm_GetFac_currStatus;
} CFW_GETFACILITY;

// The following lines define status of state machines for set facility function
#define SM_SETFAC_INIT          0xFF
#define SM_SETFAC_IDLE          0x01
#define SM_SETFAC_MMI           0x02
#define SM_SETFAC_STACK         0x03
#define SM_SETFAC_ENABLECHV     0x04
#define SM_SETFAC_DISABLECHV    0x05
#define SM_SETFAC_REHABILITATE  0x06
#define SM_SETFAC_INVALIDATE    0x07
#define SM_SETFAC_VERIFYCHV     0X08

typedef struct _CFW_SETFACILITY
{
  UINT16 nFac;
  UINT8 nMode;
  UINT8 pBufPwd[8];
  UINT8 nPwdSize;

  UINT8 nSm_SetFac_prevStatus;
  UINT8 nSm_SetFac_currStatus;
} CFW_SETFACILITY;

UINT32 CFW_SimGetFacilityProc(HAO hAo, CFW_EV* pEvent);
UINT32 CFW_SimSetFacilityProc(HAO hAo, CFW_EV* pEvent);
UINT32 CFW_SimChangePasswordProc(HAO hAo, CFW_EV* pEvent);
UINT32 CFW_SimEnterAuthenticationProc(HAO hAo, CFW_EV* pEvent);
UINT32 CFW_SimGetAuthenStatusProc(HAO hAo, CFW_EV* pEvent);

#endif // CFW_SIM_CHV_H
 
