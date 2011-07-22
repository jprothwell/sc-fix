
#ifndef CFW_SIM_H
#define CFW_SIM_H

extern UINT8 nECC_Code[5][3];
extern BOOL g_SAT_INIT_COMPLETE;

#define ERR_CFW_UTI_EXIST                       ERR_CFW_UTI_IS_BUSY //The UTI is exist.

UINT16 CFW_SimSetMeProfile(CFW_PROFILE* pMeProfile);
UINT32 CFW_SimTest(UINT8 Index);

VOID cfw_PLMNtoBCD(UINT8 *pIn, UINT8* pOut, UINT8 *nInLength);
VOID cfw_BCDtoPLMN(UINT8 *pIn, UINT8* pOut, UINT8 nInLength);
VOID cfw_IMSItoASC(UINT8 *InPut,UINT8 *OutPut, UINT8 *OutLen);


#define  CHV1                   0x01
#define  CHV2                   0x02

//#define SIM_RESET_REQ                   0x00
#define SIM_READBINARY              0x02
#define SIM_UPDATEBINARY            0x03

#define SIM_READRECORD              0x04
#define SIM_UPDATERECORD            0x05

//#define SIM_ELEMFILESTATUS          0x06
//#define SIM_DEDICFILESTATUS         0x07
#define SIM_SEEK                    0x08

#define SIM_VERIFYCHV               0x09
#define SIM_CHANGECHV               0x0A
#define SIM_DISABLECHV              0x0B
#define SIM_ENABLECHV               0x0C
#define SIM_UNBLOCKCHV              0x0D

#define SIM_INVALIDATE              0x0E
#define SIM_REHABILITATE            0x0F

//Add for SIM sat command set by lixp at 20100806
#define SIM_SAT_FRESH_IND               0x01
#define SIM_SAT_MORE_TIME_IND           0x02
#define SIM_SAT_POLL_INTERVAL_IND       0x03
#define SIM_SAT_POLLING_OFF_IND         0x04
#define SIM_SAT_SETUP_EVENT_LIST_IND    0x05
#define SIM_SAT_CALL_SETUP_IND          0x10
#define SIM_SAT_SEND_SS_IND             0x11
#define SIM_SAT_SEND_USSD_IND           0x12
#define SIM_SAT_SEND_SMS_IND            0x13
#define SIM_SAT_SEND_DTMF_IND           0x14
#define SIM_SAT_LAUNCH_BROWSER_IND      0x15
#define SIM_SAT_PLAY_TONE_IND           0x20
#define SIM_SAT_DISPLAY_TEXT_IND        0x21
#define SIM_SAT_GET_INKEY_IND           0x22
#define SIM_SAT_GET_INPUT_IND           0x23
#define SIM_SAT_SELECT_ITEM_IND         0x24
#define SIM_SAT_SETUP_MENU_IND          0x25
#define SIM_SAT_PROVIDE_LOCAL_INFO_IND  0x26
#define SIM_SAT_TIMER_MANAGEMENT_IND    0x27
#define SIM_SAT_SETUP_IDLE_DISPLAY_IND  0x28
#define SIM_SAT_CARD_APDU_IND           0x30
#define SIM_SAT_POWER_ON_CARD_IND       0x31
#define SIM_SAT_POWER_OFF_CARD_IND      0x32
#define SIM_SAT_GET_READER_STATUS_IND   0x33
#define SIM_SAT_RUN_AT_COMMAND_IND      0x34
#define SIM_SAT_LANG_NOTIFY_IND         0x35
#define SIM_SAT_OPEN_CHANNEL_IND        0x40
#define SIM_SAT_CLOSE_CHANNEL_IND       0x41 
#define SIM_SAT_RECEIVE_DATA_IND        0x42 
#define SIM_SAT_SEND_DATA_IND           0x43 
#define SIM_SAT_GET_CHANNEL_STATUS_IND  0x44 

#define SIM_SAT_PROACTIVE_SIM_COMMAND   0xD0
#define SIM_SAT_SMSPP_DOWNLOAD          0xD1
#define SIM_SAT_CB_DOWNLOAD             0xD2
#define SIM_SAT_MENU_SELECTION          0xD3
#define SIM_SAT_CALL_CONTROL            0xD4
#define SIM_SAT_MO_SMS_CONTROL          0xD5
#define SIM_SAT_EVENT_DOWNLOAD          0xD6
#define SIM_SAT_TIMER_EXPIRATION        0xD7
#ifdef CFW_MULTI_SIM
UINT32 SimParseDedicatedStatus(UINT8* pDedicatedData,CFW_SIM_ID nSimID);
UINT32 Sim_ParseSW1SW2(UINT8 SW1,UINT8 SW2,CFW_SIM_ID nSimID);
UINT32 SimResetReq(CFW_SIM_ID nSimID);
UINT32 SimReadBinaryReq(UINT8 nFile, UINT8 nOffset, UINT8 nLen,CFW_SIM_ID nSimID);
UINT32 SimUpdateBinaryReq(UINT8 nFile, UINT8 nOffset, UINT8 nLen,UINT8 *pData,CFW_SIM_ID nSimID);
UINT32 SimElemFileStatusReq(UINT8 nFile,CFW_SIM_ID nSimID);
UINT32 SimDedicFileStatusReq(UINT8 nFile,CFW_SIM_ID nSimID);
UINT32 SimReadRecordReq(UINT8 nFile, UINT8 nRecordNb,UINT8 nMode,UINT8 nLen,CFW_SIM_ID nSimID);
UINT32 SimUpdateRecordReq(UINT8 nFile, UINT8 nRecordNb,UINT8 nMode,UINT8 nLen,UINT8 *pData,CFW_SIM_ID nSimID);
UINT32 SimSeekReq(UINT8 nFile,UINT8 nTypeMode,UINT8 nLen,UINT8 *pData,CFW_SIM_ID nSimID);
UINT32 SimVerifyCHVReq(UINT8 *pCHVValue,UINT8 nCHVNumber,CFW_SIM_ID nSimID);
UINT32 SimChangeCHVReq(UINT8 *pOldCHVValue,UINT8 *pNewCHVValue,UINT8 nCHVNumber,CFW_SIM_ID nSimID);
UINT32 SimDisableCHVReq(UINT8 *pCHVValue,CFW_SIM_ID nSimID);
UINT32 SimEnableCHVReq(UINT8 *pCHVValue,CFW_SIM_ID nSimID);
UINT32 SimUnblockCHVReq(UINT8 *pUnblockCHVValue,UINT8 *pNewCHVValue,UINT8 nCHVNumber,CFW_SIM_ID nSimID);
UINT32 SimInvalidateReq(UINT8 nFile,CFW_SIM_ID nSimID);
UINT32 SimRehabilitateReq(UINT8 nFile,CFW_SIM_ID nSimID);
UINT32 CFW_SimGetMeProfile(CFW_PROFILE** pMeProfile, CFW_SIM_ID nSimID);

#else

UINT32 SimParseDedicatedStatus(UINT8* pDedicatedData);
UINT32 Sim_ParseSW1SW2(UINT8 SW1,UINT8 SW2);
UINT32 SimResetReq(VOID);
UINT32 SimReadBinaryReq(UINT8 nFile, UINT8 nOffset, UINT8 nLen);
UINT32 SimUpdateBinaryReq(UINT8 nFile, UINT8 nOffset, UINT8 nLen,UINT8 *pData);
UINT32 SimElemFileStatusReq(UINT8 nFile);
UINT32 SimDedicFileStatusReq(UINT8 nFile);
UINT32 SimReadRecordReq(UINT8 nFile, UINT8 nRecordNb,UINT8 nMode,UINT8 nLen);
UINT32 SimUpdateRecordReq(UINT8 nFile, UINT8 nRecordNb,UINT8 nMode,UINT8 nLen,UINT8 *pData);
UINT32 SimSeekReq(UINT8 nFile,UINT8 nTypeMode,UINT8 nLen,UINT8 *pData);
UINT32 SimVerifyCHVReq(UINT8 *pCHVValue,UINT8 nCHVNumber);
UINT32 SimChangeCHVReq(UINT8 *pOldCHVValue,UINT8 *pNewCHVValue,UINT8 nCHVNumber);
UINT32 SimDisableCHVReq(UINT8 *pCHVValue);
UINT32 SimEnableCHVReq(UINT8 *pCHVValue);
UINT32 SimUnblockCHVReq(UINT8 *pUnblockCHVValue,UINT8 *pNewCHVValue,UINT8 nCHVNumber);
UINT32 SimInvalidateReq(UINT8 nFile);
UINT32 SimRehabilitateReq(UINT8 nFile);
UINT32 CFW_SimGetMeProfile(CFW_PROFILE** pMeProfile);

#endif

VOID CFW_ValidatePDU(UINT8* pTPDU);
UINT32 cfw_SatProCmdProccess(HAO hAo, UINT8 nCmdType, UINT8* pData, UINT8 nLen);
UINT32 cfw_SatProCmdSetupMenu(HAO hAo, UINT8* pData, UINT8 nLength);
UINT32 cfw_SatProCmdLocal(HAO hAo, UINT8* pData);
#endif //CFW_SIM_H


