/******************************************************************************/
/*              Copyright (C) 2005, CII Technologies, Inc.                    */
/*                            All Rights Reserved                             */
/******************************************************************************/
/* Filename:    cfw_prv.h                                                     */
/*                                                                            */
/* Description:                                                               */
/*   cfw SIM service private procedure declarations,                          */
/*   constant definitions and macros.                                         */
/*                                                                            */
/******************************************************************************/

#if !defined(__CFW_SIM_PRV_H__)
#define __CFW_SIM_PRV_H__

#include <cfw_prv.h> 



 






typedef struct _CFW_SIM_SMS_PARAMETERS{
    UINT8  nSMSCInfo[12];
    UINT8  nPID;
    UINT8  nDCS;
    UINT8  nVP;
	UINT8 nFlag;
} CFW_SIM_SMS_PARAMETERS;





#if 1
UINT32 SRVAPI CFW_ComposePDU(
    UINT8   Concat,
    UINT8   SMSStatus,
    UINT8*  pData,
    UINT16  nDataSize,
    CFW_DIALNUMBER* pNumber,
    CFW_EV* pEvent
    );

UINT32 SRVAPI CFW_DecomposePDU(
						PVOID pInData,    //176 length PDU date
						UINT8 nType,			
						UINT32* pOutData,
						UINT8* pSMSStatus, //SMS Status just as below
						CFW_SMS_MULTIPART_INFO* pLongerMsg
						);
#endif

UINT32 SRVAPI CFW_CfgGet_Default_SCA_Index(
                   UINT8 *pIndex
);        


UINT32 SRVAPI CFW_CfgSet_Default_SCA_Index(
                   UINT8 nIndex
);

UINT32 SRVAPI CFW_SIMSmsInit(VOID);

typedef struct _SIM_SAT_PARAM{
	CFW_SAT_MENU_RSP_LIST* pSetupMenu;
	VOID* pSatComData;
    UINT8 nCurCmd;
    UINT8 nCmdNum;
    UINT8 nPending[2];
}SIM_SAT_PARAM;

typedef struct _SIM_SMS_PARAM{
	UINT8 nPending[4];
}SIM_SMS_PARAM;

//For Sim initialization,SIM����ʼ�����SIM��״̬ͨ����Ϣ���͸�SHELL,ͬʱ��״̬�洢��G_Mic_Param�С�
//used for nCHVAuthen variable

#define CFW_SIM_CARD_REMOVED        CFW_STY_AUTH_NOSIM     //No sim card is inserted.That just only is sent after API_SIM_RESET_CNF received.
#define CFW_SIM_CARD_INSERTED       CFW_STY_AUTH_SIM_PRESENT  
#define CFW_SIM_CARD_BAD            (CFW_STY_AUTH_SIM_END+1)    //The sim card cannot be recognised.
#define CFW_SIM_CARD_ABNORMITY      (CFW_STY_AUTH_SIM_END+2)    //The sim card cannot be used(IMSI,LOCI).
#define CFW_SIM_CARD_TEST           (CFW_STY_AUTH_SIM_END+3)    //The sim card is for test mode.
#define CFW_SIM_CARD_NORMAL         (CFW_STY_AUTH_SIM_END+4)    //The sim card is for normal mode.

#define CFW_SIM_CARD_PIN2BLOCK      CFW_STY_AUTH_PIN2BLOCK  //The sim card has blocked by pin2.
#define CFW_SIM_CARD_PUK2           CFW_STY_AUTH_SIMPUK2    //The sim card has been lock by puk2.
#define CFW_SIM_CARD_PIN2           CFW_STY_AUTH_SIMPIN2    //The sim card has been lock by pin2.
#define CFW_SIM_CARD_PIN2READY      CFW_STY_AUTH_READY      //The pin1 have been presented.

#define CFW_SIM_CARD_PIN1BLOCK      CFW_STY_AUTH_PIN1BLOCK   //The sim card has blocked by pin1.
#define CFW_SIM_CARD_PUK1           CFW_STY_AUTH_SIMPUK  //The sim card has been lock by puk1.
#define CFW_SIM_CARD_PIN1           CFW_STY_AUTH_SIMPIN  //The sim card has been lock by pin1.
#define CFW_SIM_CARD_PIN1DISABLE    CFW_STY_AUTH_PIN1_DISABLE//The status of the pin1 is disable.
#define CFW_SIM_CARD_PIN1READY      CFW_STY_AUTH_PIN1_READY  //The pin1 have been presented.
#define CFW_SIM_CARD_PHASE          CFW_STY_AUTH_SIM_PHASE   

#define CFW_SIM_ME_PS               0x10  //The ME lock by PS.
#define CFW_SIM_ME_PF               0x20  //The ME lock by PF.

//#define CFW_SIM_ADN               (0x0F)        //adn is effective in the current sim
//#define CFW_SIM_FDN                     (0xF0)        //fdn is effective in the current sim
//#define CFW_SIM_BDN               (0xFF)        //bdn is effective in the current sim

//The following lines define the phase of the sim.
#define SIM_PHASE1              0x00
#define SIM_PHASE2              0x02
#define SIM_PHASE2Plus       0x03

#if 0
//��CFW_SimGetAuthenticationStatus�У�CFW_SIM_AUTH_NOSIM��Ӧ�÷��أ�ͬʱ��Ҫ���һ��״̬
#define CFW_STY_AUTH_PIN1BLOCK          CFW_SIM_CARD_PIN1BLOCK
#define CFW_STY_AUTH_SIMPUK             CFW_SIM_CARD_PUK1
#define CFW_STY_AUTH_SIMPIN             CFW_SIM_CARD_PIN1
#define CFW_STY_AUTH_PIN1DISABLE        CFW_SIM_CARD_PIN1DISABLE
#define CFW_STY_AUTH_PIN1_READY         CFW_SIM_CARD_PIN1READY
      
#define CFW_STY_AUTH_PIN2BLOCK          CFW_SIM_CARD_PIN2BLOCK
#define CFW_STY_AUTH_SIMPUK2            CFW_SIM_CARD_PUK2
#define CFW_STY_AUTH_SIMPIN2            CFW_SIM_CARD_PIN2
#define CFW_STY_AUTH_READY              CFW_SIM_CARD_PIN2READY
#endif

//��CFW_SimGetAuthenticationStatus�У�CFW_SIM_AUTH_NOSIM��Ӧ�÷��أ�ͬʱ��Ҫ���һ��״̬
#define CFW_SIM_AUTH_PIN1BLOCK          CFW_STY_AUTH_PIN1BLOCK
#define CFW_SIM_AUTH_PUK1               CFW_STY_AUTH_SIMPUK
#define CFW_SIM_AUTH_PIN1               CFW_STY_AUTH_SIMPIN
#define CFW_SIM_AUTH_PIN1DISABLE        CFW_STY_AUTH_PIN1_DISABLE
#define CFW_SIM_AUTH_PIN1READY          CFW_STY_AUTH_PIN1_READY
      
#define CFW_SIM_AUTH_PIN2BLOCK          CFW_STY_AUTH_PIN2BLOCK
#define CFW_SIM_AUTH_PUK2               CFW_STY_AUTH_SIMPUK2
#define CFW_SIM_AUTH_PIN2               CFW_STY_AUTH_SIMPIN2
#define CFW_SIM_AUTH_PIN2READY          CFW_STY_AUTH_READY

//That Macro define three stages to SIM starting.
#define SIM_INIT_EV_STAGE1                  1
#define SIM_INIT_EV_STAGE2                  2
#define SIM_INIT_EV_STAGE3                  3
#define SIM_INIT_EV_GSM                     4


//The following lines define CHV's status.
#define CHV_UNINIT        0         //PIN1,PUK1,PIN2,PUK2
#define CHV_NOVERIFY      1         //PIN1,PIN2
#define CHV_VERIFY        2         //PIN1,PIN2
#define CHV_DISABLE       3         //PIN1
//#define CHV_BLOCKED       4         //PIN1,PIN2
#define CHV_INIT          5         //PUK1,PUK2

//#define CHV_BLOCK         4         //PUK1,PUK2
//#define CHV_UNBLOCK       5         //PUK1,PUK2
#define CHV_UNKNOW        7         //ALL


typedef struct _SIM_CHV_PARAM
{
    UINT8 nFunID;                   //SIM function ID.

    UINT8 bCHV2VerifyReq:1;         //This variable is used to mark where the verification of CHV2 is needed or not.
    UINT16 nCHVAuthen;              //
   
    UINT32 nPin1Status:3;           //0:initial; 
                                    //1:enable and not verify;
                                    //2:enable and verify;
                                    //3:disable;
                                    //4:blocked,that cannot be unblocked.

    UINT32 nPin2Status:3;           //0:not initial;
                                    //1:initial(not verify);
                                    //2:verify

    UINT32 nPuk1Status:3;           //CHV_BLOCK,CHV_UNBLOCK or CHV_UNINIT
    UINT32 nPuk2Status:3;           //CHV_BLOCK,CHV_UNBLOCK or CHV_UNINIT

//    UINT32 nPin1Remain:4;           //the number of attempts still available for entering a 
                                    //required password(PIN, PUK, PH-SIM PUK etc).
                                    //for Pin1
//    UINT32 nPuk1Remain:4;           //See above for puk1
//    UINT32 nPin2Remain:4;           //See above for pin2
//    UINT32 nPuk2Remain:4;           //See above for puk2
    
}SIM_CHV_PARAM;


#define EF_ENABLE            1
#define EF_DISABLE          0

typedef struct _SIM_PBK_PARAM{
    UINT8  nADNTotalNum;            //total Number of ADN
    UINT8  nADNRecordSize;          //
    
    UINT8  nFDNTotalNum;            //total Number of FDN
    UINT8  nFDNRecordSize;          //

    UINT8  nLNDTotalNum;            //total Number of LND
    UINT8  nLNDRecordSize;          //
    
    UINT8  nMSISDNTotalNum;         //total Number of MSISDN
    UINT8  nMSISDNRecordSize;       //

    UINT8  nECCTotalNum;            //total Number of ECC
    UINT8  nECCRecordSize;          //

    UINT8  nBDNTotalNum;            //
    UINT8  nBDNRecordSize;          //

	UINT8  nSDNTotalNum;            //
    UINT8  nSDNRecordSize;          //
    
#if 0 
    UINT8 bADNStatus:1;            //This byte is used to show the ADN whether to invalidate or not.
    UINT8 bBDNStatus:1;
#else
    UINT8 bADNStatus;
    UINT8 bBDNStatus;
#endif
} SIM_PBK_PARAM;

#define SERVICES_NUM        0x13    //The number of max services is 50.
//#define SERVICES_STATUS     ((SERVICES_NUM+1)>>1)
typedef struct _SIM_MISC_PARAM{
    BOOL   bImageDone;
    BOOL bStartPeriod;              // true:have been transfered; false:not be transfered 
    UINT16 nSimMode;             //CFW_SIM_CARD_TEST,CFW_SIM_CARD_NOMRAL...
    //UINT8 nPad[2];
    UINT8 nPhase;
    UINT8 nServices[SERVICES_NUM];
}SIM_MISC_PARAM;                                                 

#ifdef CFW_MULTI_SIM
UINT32 SRVAPI CFW_SimSmsInit(CFW_SIM_ID nSimID);
UINT32 SRVAPI CFW_SimGetSmsStorageInfo ( 
										UINT8 nStatus,
										UINT16 nUTI,
										CFW_SIM_ID nSimID
										);
UINT32 SRVAPI CFW_SimSetSmsParameters (
									   CFW_SIM_SMS_PARAMETERS pSmsParamArray[],
									   UINT8 nArrayCount,
									   UINT8 nIndex,
									   UINT16 nUTI,
									   CFW_SIM_ID nSimID
									   );

UINT32 SRVAPI CFW_SimGetMR(
                    UINT16  nUTI,
                    CFW_SIM_ID nSimID
                    );

UINT32 SRVAPI CFW_SimSetMR(
                    UINT8   nMR,
                    UINT16   nUTI,
                    CFW_SIM_ID nSimID
                    );
UINT32 SRVAPI CFW_SimReadBinary ( 
								 UINT8 nFileId,
								 UINT8 nOffset,
								 UINT8 nBytesToRead,
								 UINT16 nUTI,
								 CFW_SIM_ID nSimID
								 ); 


UINT32 SRVAPI CFW_SimUpdateBinary ( 
								   UINT8 nFileId,
								   UINT8 nOffset,
								   UINT8* pData,
								   UINT8 nBytesToWrite,
								   UINT16 nUTI,
								   CFW_SIM_ID nSimID
								   );
UINT32 SRVAPI CFW_SimDeleteMessage ( 
									UINT16 nLocation,
									UINT16 nIndex,
									UINT8 nStatus,
									UINT16 nUTI,
									CFW_SIM_ID nSimID
									);

UINT32 SRVAPI CFW_SimWriteMessage ( 
								   UINT16 nLocation,
								   UINT32 nIndex,
								   UINT8* pData,
								   UINT8 nDataSize,
								   UINT16 nUTI,
								   CFW_SIM_ID nSimID
								   );



UINT32 SRVAPI CFW_SimListMessage ( 
								  UINT16 nLocation, 
								  UINT8 nStatus, 
								  UINT16 nCount, 
								  UINT16 nTimes, 
								  UINT16 nUTI,
								  CFW_SIM_ID nSimID
								  );
UINT32 CFW_SimReadMessage ( 
                           UINT16 nLocation,
                           UINT32 nIndex,
                           UINT16 nUTI, 
                           CFW_SIM_ID nSimID
                           );
UINT32 CFW_CfgSimGetSmsParam(SIM_SMS_PARAM** pSimSmsParam,
  CFW_SIM_ID nSimID
);

UINT32 CFW_CfgSimGetChvParam(SIM_CHV_PARAM** pSimChvParam,
  CFW_SIM_ID nSimID
);

UINT32 CFW_CfgSimGetPbkParam(SIM_PBK_PARAM** pSimPbkParam,
  CFW_SIM_ID nSimID
);

UINT32 CFW_CfgSimGetMicParam(SIM_MISC_PARAM** pSimMicParam,
  CFW_SIM_ID nSimID
);

UINT32 CFW_CfgSimGetSatParam(SIM_SAT_PARAM** pSimSatParam,
  CFW_SIM_ID nSimID
);

#else
UINT32 SRVAPI CFW_SimSmsInit(VOID);
UINT32 SRVAPI CFW_SimGetSmsStorageInfo ( 
										UINT8 nStatus,
										UINT16 nUTI
										);
UINT32 SRVAPI CFW_SimSetSmsParameters (
									   CFW_SIM_SMS_PARAMETERS pSmsParamArray[],
									   UINT8 nArrayCount,
									   UINT8 nIndex,
									   UINT16 nUTI
									   );

UINT32 SRVAPI CFW_SimGetMR(
                    UINT16  nUTI
                    );

UINT32 SRVAPI CFW_SimSetMR(
                    UINT8   nMR,
                    UINT16   nUTI
                    );
UINT32 SRVAPI CFW_SimReadBinary ( 
								 UINT8 nFileId,
								 UINT8 nOffset,
								 UINT8 nBytesToRead,
								 UINT16 nUTI
								 ); 


UINT32 SRVAPI CFW_SimUpdateBinary ( 
								   UINT8 nFileId,
								   UINT8 nOffset,
								   UINT8* pData,
								   UINT8 nBytesToWrite,
								   UINT16 nUTI
								   );
UINT32 SRVAPI CFW_SimDeleteMessage ( 
									UINT16 nLocation,
									UINT16 nIndex,
									UINT8 nStatus,
									UINT16 nUTI
									);

UINT32 SRVAPI CFW_SimWriteMessage ( 
								   UINT16 nLocation,
								   UINT32 nIndex,
								   UINT8* pData,
								   UINT8 nDataSize,
								   UINT16 nUTI
								   );



UINT32 SRVAPI CFW_SimListMessage ( 
								  UINT16 nLocation, 
								  UINT8 nStatus, 
								  UINT16 nCount, 
								  UINT16 nTimes, 
								  UINT16 nUTI
								  );
UINT32 SRVAPI CFW_SimReadMessage ( 
								  UINT16 nLocation,
								  UINT32 nIndex,
								  UINT16 nUTI
								  );
UINT32 CFW_CfgSimGetSmsParam(SIM_SMS_PARAM** pSim_SmsParam);
//UINT32 CFW_CfgSimSetSmsParam(SIM_SMS_PARAM* pSim_SmsParam);

UINT32 CFW_CfgSimGetChvParam(SIM_CHV_PARAM** pSim_ChvParam);
//UINT32 CFW_CfgSimSetChvParam(SIM_CHV_PARAM* pSim_ChvParam);

UINT32 CFW_CfgSimGetPbkParam(SIM_PBK_PARAM** pSim_PbkParam);
//UINT32 CFW_CfgSimSetPbkParam(SIM_PBK_PARAM* pSim_PbkParam);

UINT32 CFW_CfgSimGetMicParam(SIM_MISC_PARAM** pSim_MicParam);
//UINT32 CFW_CfgSimSetMicParam(SIM_MISC_PARAM* pSim_MicParam);
UINT32 CFW_CfgSimGetSatParam(SIM_SAT_PARAM** pSim_SatParam);
//UINT32 CFW_CfgSimSetSatParam(SIM_SAT_PARAM* pSim_SatParam);

#endif



#endif // _H_

