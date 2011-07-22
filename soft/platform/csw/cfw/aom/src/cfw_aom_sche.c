#include "base.h"
#include "cfw.h"
#include "stdkey.h"
#include "sul.h" // only for my test
#include "api_msg.h" // only for my test
#include "ts.h"
#include "cfw_prv.h"
#include "event.h"
#include "csw_debug.h"
  

#include "csw_mem_prv.h"
HANDLE g_aom_sem = 0x0;
#define AOM_Down()   COS_WaitForSemaphore(g_aom_sem, COS_WAIT_FOREVER)
#define AOM_Up()     COS_ReleaseSemaphore( g_aom_sem ) 
#define YJX_SIM_VER       0
#define WCF_SIM_VER       1 


#define IS_DM_EVENT(evId)       (((evId)>=EV_DM_BASE) && ((evId)<EV_DM_END_))

#define IS_SCL_EVENT(evId)        (evId > HVY_API && evId < HVY_API + 0xA90)

#define SERVICE_ID_MAX                  CFW_MAX_SRV_ID
#define SERVICE_ID_UNKNOW               255

#define GET_0TO7_BITS(x)        (x & 0x000000FF)        // TI from Stack or other device
#define GET_8TO23_BITS(x)       ((x & 0x00FFFF00)>>8)   // For Internal CFW or APP

#define GET_STACK_UTI(x)    GET_0TO7_BITS(x)
#define GET_APP_CFW_UTI(x)  GET_8TO23_BITS(x)

#define QUERY_AO_ILLEGAL(PAOC)\
    if(PAOC)\
    {    if(PAOC->nServiceID > CFW_MAX_SRV_ID)\
            return ERR_AOM_HAO_ILLEGAL;\
    }\
    else\
        return ERR_AOM_HAO_ILLEGAL;

#define QUERY_AO_ILLEGAL_BREAK(PAOC)\
    if(PAOC)\
    {    if(PAOC->nServiceID > CFW_MAX_SRV_ID)\
            break;\
    }\
    else\
        break;

#define MAX_CC_NUM     7

typedef struct _AO_CONTENT
{
    UINT32                UTI;
    UINT32                nProcCode;
    void                  *UserData;
    PFN_AO_PORC           pAO_Proc;
    UINT32                AO_State;
    UINT32                AO_NextState;
    
    PFN_CFW_EV_PORC    pfnEvProc;
    void                  *pEventUserData;
        
    struct _AO_CONTENT    *NextAO;
    UINT32                nServiceID;
#ifdef CFW_MULTI_SIM
    CFW_SIM_ID        nSimID;
#endif
} AO_CONTENT;


typedef struct  _AO_CB_DOING
{
    AO_CONTENT                   *DoingAO;

    struct  _AO_CB_DOING         *NextDoing;

}AO_CB_DOING;

/*AO Control Block*/
typedef struct  _AO_CB
{
    AO_CONTENT         *AcitveAO;
    AO_CONTENT         *ReadyAO;
   
    AO_CONTENT         *SpecialAO;      //for something marked...
    
    AO_CB_DOING        *DoingHead;
    AO_CB_DOING        *DoingTail;
    
    UINT8              ActiveNum;
    UINT8              ReadyNum;
    UINT8              pading2;   
    UINT8              pading1;   

}AO_CB;


static UINT32   AO_GoOnStatus[2];
static AO_CB    AO_ControlBlock[SERVICE_ID_MAX];

#define EVENT_IND_CM        0
#define EVENT_IND_SMS_MT    1
#define EVENT_IND_GPRS      2
#define EVENT_IND_SIM       3
#define EVENT_IND_TIMER     4
#define EVENT_IND_USSD      5

#define EVENT_IND_MODULE_NUM 6

static UINT32   UTI_Status[8];

static PFN_EV_IND_TRIGGER_PORC Event_Ind_Fun[EVENT_IND_MODULE_NUM+1];

static UINT32 gCurrentServId = 0;

PFN_CFW_DEFAULT_PROC CFW_Sys_Default_Proc = NULL;
#define    NOTIFY_TO_ATL          0xFF
#define    NOTIFY_TO_SYS_DEFAULT  0xFE 

//#if DEFINE_ALLOC_UTI
static UINT32 ReleaseUsingUTI (  UINT32 nServiceId,  UINT8 UTI );
//#endif

//add for QQ, 2010-05-10
#ifdef CFW_TCPIP_SUPPORT
extern VOID CFW_SmarchCCStatus(UINT32 Status);
#endif
//add end

static UINT32 CFW_bitSet(UINT8 bset, UINT8 bindex,UINT32 *bdes,UINT8 bcount);
static UINT8 CFW_bitGet(UINT8 bindex,UINT32 *bdes,UINT8 bcount);
#define SET_AO_GOONSTATUS(a,b)  CFW_bitSet(a,b,AO_GoOnStatus,2)
#define GET_AO_GOONSTATUS  CFW_bitGet(0xff, AO_GoOnStatus, 2)
#if 0
PRIVATE void AOM_init()
{             
    UINT32 n = SIZEOF(AO_ControlBlock);
    SUL_ZeroMemory32(AO_ControlBlock, n);

    n = SIZEOF(Event_Ind_Fun);
    SUL_ZeroMemory32(Event_Ind_Fun, n);
    
    n = SIZEOF(UTI_Status);
    SUL_ZeroMemory32(UTI_Status, n);
}
#endif

//=====================================================================================
//
//
//=====================================================================================    
static UINT8 WhickKindOfNotifyEvent(UINT32 ID)
{
    UINT32 APP_CFW_UTI   = GET_APP_CFW_UTI(ID);
    CSW_TRACE(CFW_AOM_TS_ID, "WhickKindOfNotifyEvent,  APP_CFW_UTIn = 0x%x\n", APP_CFW_UTI);
    if((APP_CFW_UTI >= CFW_UTI_MIN) && (APP_CFW_UTI <= CFW_UTI_MAX))
    {
        if( APP_CFW_UTI >= SMS_MO_UTI_MIN && APP_CFW_UTI <= SMS_MO_UTI_MAX )
        {
            return CFW_SMS_MO_SRV_ID;
        }
        else if( APP_CFW_UTI >= SMS_MT_UTI_MIN && APP_CFW_UTI <= SMS_MT_UTI_MAX )
        {
            return CFW_SMS_MT_SRV_ID;
        }
        else if( APP_CFW_UTI >= CC_UTI_MIN && APP_CFW_UTI <= CC_UTI_MAX)
        {
            return CFW_CC_SRV_ID;
        }
        else if ( APP_CFW_UTI >= NW_UTI_MIN && APP_CFW_UTI <= NW_UTI_MAX)
        {
            return CFW_NW_SRV_ID;
        }
        else if( APP_CFW_UTI >= SIM_UTI_MIN && APP_CFW_UTI <= SIM_UTI_MAX )
        {
            return CFW_SIM_SRV_ID;            
        }
        else if( APP_CFW_UTI >= SS_UTI_MIN && APP_CFW_UTI <= SS_UTI_MAX )
        {
            return CFW_SS_SRV_ID;
        }
        else if( APP_CFW_UTI >= SMS_CB_UTI_MIN && APP_CFW_UTI <= SMS_CB_UTI_MAX )
        {
            return CFW_SMS_CB_ID;
        }
        else if( APP_CFW_UTI >= PDP_UTI_MIN && APP_CFW_UTI <= PDP_UTI_MAX )
        {
            return CFW_PDP_SRV_ID;
        }
        else if( APP_CFW_UTI >= PSD_UTI_MIN && APP_CFW_UTI <= PSD_UTI_MAX )
        {
            return CFW_PSD_SRV_ID;
        }
        else if( APP_CFW_UTI >= CSD_UTI_MIN && APP_CFW_UTI <= CSD_UTI_MAX )
        {
            return CFW_CSD_SRV_ID;
        }
        else if( APP_CFW_UTI >= CFW_APP_UTI_MIN && APP_CFW_UTI <= CFW_APP_UTI_MAX )
        {
            return CFW_APP_SRV_ID;
        }
    }

    if( APP_CFW_UTI == APP_UTI_SHELL )
        return NOTIFY_TO_SYS_DEFAULT;
    
    return NOTIFY_TO_ATL;
       
}


//=====================================================================================
// 
//
//=====================================================================================
static UINT32 WhichKindOfService(UINT32 ID)
{
    CSW_TRACE(CFW_AOM_TS_ID, "WhichKindOfService,  ID = 0x%x\n", ID);
    if(ID >= API_IMEI_IND && ID <= API_STOP_CNF)
    {
        //General
        //return CFW_GEN_SRV_ID;
        return CFW_NW_SRV_ID;
    }
    else if (( ID >= API_NW_CSREGSTATUS_IND && ID <= API_NW_STORELIST_IND) || (ID == API_NW_LISTABORT_IND) )
    {
        //Network
        return CFW_NW_SRV_ID;
    }
    else if ( ID >= API_CC_TI_IND && ID <= API_CC_CHANMODE_IND)
    {
        // CC     
        return CFW_CC_SRV_ID;
    
    }
    else if ( ID >= API_SS_ACTION_CNF && ID <= API_SS_ACTION_IND)
    {
        // SS 
        return CFW_SS_SRV_ID;
    }
    else if ( ID >= API_PDP_CTXACTIVATE_CNF && ID <= API_PDP_CTXDEACTIVATE_CNF)
    {
        // PDP 
        return CFW_PDP_SRV_ID;    
    }
    else if ((ID >= API_PSDATA_IND && ID <= API_PS_ULCONGEST_IND)||(ID == API_SYS_CTR_IND))
    {
        //PSD
        return CFW_PSD_SRV_ID;
    }
    else if ( (ID >= API_SIM_RESET_CNF && ID <= API_SIM_FETCH_IND) )
    {
        // SIM 
        return CFW_SIM_SRV_ID;
    
    }
    else if ( ID == API_SMSPP_RECEIVE_IND)
    {
        //SMS_MT
        return CFW_SMS_MT_SRV_ID;

    }
    else if (ID == API_SMSPP_ACK_IND || ID == API_SMSPP_ERROR_IND)
    {
        //SMS_MO
        return CFW_SMS_MO_SRV_ID;
    
    }
    else if (ID == API_SMSCB_RECEIVE_IND || ID == API_SMSCB_PAGE_IND)
    {
        //SMS_CB
        return CFW_SMS_CB_ID;
    }
    else if (ID == API_CSD_DATA_IND)
    {
        //FAX, DATA
        return CFW_CSD_SRV_ID;
    }
    else if( (ID >= API_DBGINFO_IND && ID <= API_ADJCELL_INFO_IND) )
    {
        //Engineering MODE
        return CFW_EMOD_SRV_ID;
    }
    else if( ID == EV_TIMER)
    {
        return CFW_TIMER_SRV_ID;
    }
    else
    {
        return SERVICE_ID_UNKNOW;
    }


}
extern UINT32 CSW_SAT_InitEX(CFW_EV* pEV);

/*=====================================================================================


=====================================================================================*/
static UINT32 Dispatch(CFW_EV* pEvent)
{
	AO_CONTENT  *pAOC = 0;
	UINT8  i = 0;
	UINT8  nActiveAONumber = 0;

	CFW_EV *Msg  = pEvent;
	UINT32    MsgID = 0;
	UINT32    nServiceId = 0xFF;
       UINT32    nServiceIdO = 0xFF;
	CSW_PROFILE_FUNCTION_ENTER(Dispatch);
	MsgID = Msg->nEventId;

	if( !Msg )
	{
		CSW_TRACE(CFW_AOM_TS_ID, "ERROR ! Dispatch Msg data is NULL \n");

		CSW_PROFILE_FUNCTION_EXIT(Dispatch);
		return ERR_CFW_NOT_EXIST_AO;
	}
	nServiceId = WhichKindOfService(MsgID);
	nServiceIdO = nServiceId;
#ifdef CFW_MULTI_SIM
	if(CFW_TIMER_SRV_ID!= nServiceId)
		nServiceId = (nServiceId*CFW_SIM_COUNT+Msg->nFlag);
#endif
       CSW_TRACE(CFW_AOM_TS_ID, "UINT32 Dispatch(),  nServiceId = 0x%x\n", nServiceId);
	switch( nServiceIdO )
	{
		    //************************************************************************
		    //                    CALL MANAGER
		    //************************************************************************
		    case CFW_CC_SRV_ID:

				nActiveAONumber = AO_ControlBlock[nServiceId].ActiveNum;

		        	// If incoming call, need to register one, . 
				if (MsgID == API_CC_SETUP_IND )
			    {
					AO_CONTENT  *pTmp = AO_ControlBlock[nServiceId].AcitveAO;                    
					nActiveAONumber = AO_ControlBlock[nServiceId].ActiveNum;

					i = 0;
					while( (i < nActiveAONumber - 1) && ( pTmp ))
					{
						i++;
						pTmp = pTmp->NextAO;
					}

					//Call the register AO function.
					if(Event_Ind_Fun[EVENT_IND_CM])
					    pAOC = (AO_CONTENT *)Event_Ind_Fun[EVENT_IND_CM](Msg);
					else
					{
						CSW_TRACE(CFW_AOM_TS_ID, "UINT32 Dispatch(), 1 error\n");
						CSW_PROFILE_FUNCTION_EXIT(Dispatch);
						return ERR_AOM_EMPTY_REGISTER_FUNCTION;
					}

					if(pTmp)
					    	pTmp->NextAO = pAOC;
					else
					    	AO_ControlBlock[nServiceId].AcitveAO = pAOC; 

					AO_ControlBlock[nServiceId].ActiveNum++;
			            
					//Update the AO of incoming call position.       
					if( AO_ControlBlock[nServiceId].SpecialAO == pAOC)
					{
					    	AO_ControlBlock[nServiceId].SpecialAO = 0;
					}
					else if( AO_ControlBlock[nServiceId].ReadyAO )
					{
						AO_CONTENT  *pReadyAOC = AO_ControlBlock[nServiceId].ReadyAO;                                        
						UINT8 nReadyAONumber = AO_ControlBlock[nServiceId].ReadyNum;

						if (pReadyAOC == pAOC)
						{
							AO_ControlBlock[nServiceId].ReadyAO = 0;
							AO_ControlBlock[nServiceId].ReadyNum = 0;
						}
						else
						{
							i = 0;
							while(( i < nReadyAONumber - 1 ) && ( pReadyAOC->NextAO != pAOC))
							{
								i++;
								pReadyAOC = pReadyAOC->NextAO;
							}                                    

							if ( pReadyAOC->NextAO == pAOC )
							{
								pReadyAOC->NextAO = 0;                               
								AO_ControlBlock[nServiceId].ReadyNum--;
							}
							else
							{
								CSW_TRACE(CFW_AOM_TS_ID, "UINT32 Dispatch(), 2 error\n");
								CSW_PROFILE_FUNCTION_EXIT(Dispatch);
								return ERR_CFW_NOT_EXIST_AO;
							}
						}
					}
					else
					{
						CSW_TRACE(CFW_AOM_TS_ID, "UINT32 Dispatch(), 3 error\n");
						CSW_PROFILE_FUNCTION_EXIT(Dispatch);
						return ERR_AOM_READY_QU_EMPTY;
					}
			           
			            pAOC->AO_State = CFW_SM_STATE_IDLE;
			            
			            if (pAOC->pAO_Proc != NULL)
			                pAOC->pAO_Proc((HAO)pAOC, Msg);
			            
			            break;
			     	}


				//Dispatch message to special AO for initiating
				if(AO_ControlBlock[nServiceId].SpecialAO && (MsgID == API_CC_TI_IND ))                    
				{
					pAOC = AO_ControlBlock[nServiceId].SpecialAO;

					if(!pAOC)
					{
						CSW_TRACE(CFW_AOM_TS_ID, "UINT32 Dispatch(), 4 error\n");
						CSW_PROFILE_FUNCTION_EXIT(Dispatch);
						return ERR_AOM_INIT_QU_FAILURE;
					}

					if (pAOC->pAO_Proc != NULL)
						pAOC->pAO_Proc((HAO)pAOC, Msg);  /*So, this AO must obtain a TI.*/

					break;
				}

				//Dispatch message to init_active AO.
				pAOC = AO_ControlBlock[nServiceId].SpecialAO;
				if( pAOC )
				{
					// Not having parameter Message...
					if( ( MsgID == API_CC_AUDIOON_IND ) ||
					( MsgID == API_CC_AOC_CCM_IND ) ||
					( MsgID == API_CC_CHANMODE_IND ) )
					{
						if ( pAOC )
						{
							if (pAOC->pAO_Proc != NULL)
							pAOC->pAO_Proc((HAO)pAOC, Msg);

							break;
						}
						else 
						{
							CSW_TRACE(CFW_AOM_TS_ID, "UINT32 Dispatch(), 5 error\n");
							CSW_PROFILE_FUNCTION_EXIT(Dispatch);
							return ERR_AOM_MISMATCH_MSG;
						}

					}
					// Other messages... 
					if( (UINT8)GET_0TO7_BITS(pAOC->UTI) == GET_0TO7_BITS(*(UINT8*)(Msg->nParam1)) )
					{
						if (pAOC->pAO_Proc != NULL)
							pAOC->pAO_Proc((HAO)pAOC, Msg);


						//The Call have been connected 
						if (MsgID == API_CC_CONNECT_IND)
						{
							//Move the init_active AO--->Actice Queue, if it is possible. 
							if(nActiveAONumber == 0)
							{
								AO_ControlBlock[nServiceId].AcitveAO = pAOC;
							}
							else if( nActiveAONumber < MAX_CC_NUM)
							{
								AO_CONTENT  *pTmp = AO_ControlBlock[nServiceId].AcitveAO;                    
								i = 0;
								while( (i < nActiveAONumber - 1) && ( pTmp ))
								{
									i++;
									pTmp = pTmp->NextAO;
								}
								pTmp->NextAO = pAOC;
							}
							AO_ControlBlock[nServiceId].ActiveNum++;    

							//Move the first ready AO to init_active, if it is possible. 
							nActiveAONumber = AO_ControlBlock[nServiceId].ActiveNum;

							if( nActiveAONumber < MAX_CC_NUM && AO_ControlBlock[nServiceId].ReadyAO ) 
							{
								AO_ControlBlock[nServiceId].SpecialAO = AO_ControlBlock[nServiceId].ReadyAO;
								pAOC = AO_ControlBlock[nServiceId].SpecialAO;

								AO_ControlBlock[nServiceId].ReadyAO = AO_ControlBlock[nServiceId].ReadyAO->NextAO;

								AO_ControlBlock[nServiceId].SpecialAO->NextAO = 0;

								AO_ControlBlock[nServiceId].ReadyNum--;

								pAOC->AO_State = CFW_SM_STATE_IDLE;

								if (pAOC->pAO_Proc != NULL)
									pAOC->pAO_Proc((HAO)pAOC, SPECIAL_MESSAGE);


							}
							else
							{    
								AO_ControlBlock[nServiceId].SpecialAO = 0;
							}
						} 
						break;    
					}
				                           
				}

				//Dispatch message to active AO.
				if ( nActiveAONumber  > 0 )
				{
					UINT8 m = (UINT8)(*(UINT8 *)(Msg->nParam1));
					pAOC = AO_ControlBlock[nServiceId].AcitveAO;

					// Not having parameter Message...
					if( ( MsgID == API_CC_AUDIOON_IND ) ||
					( MsgID == API_CC_AOC_CCM_IND ) ||
					( MsgID == API_CC_CHANMODE_IND ) )
					{
						if ( pAOC )
						{
							if (pAOC->pAO_Proc != NULL)
							pAOC->pAO_Proc((HAO)pAOC, Msg);

							break;
						}
						else 
						{
							CSW_TRACE(CFW_AOM_TS_ID, "UINT32 Dispatch(), 6 error 0x%x \n", MsgID);
							CSW_PROFILE_FUNCTION_EXIT(Dispatch);
							return ERR_AOM_MISMATCH_MSG;
						}

					}
					i = 0;
					while( ((UINT8)(pAOC->UTI) != m ) && (i < nActiveAONumber) )
					{
						pAOC = pAOC->NextAO;
						i++;

						if( !pAOC)
							break;
					}

					if ( pAOC )
					{
						if (pAOC->pAO_Proc != NULL)
						pAOC->pAO_Proc((HAO)pAOC, Msg);
					}
					else 
					{
						CSW_TRACE(CFW_AOM_TS_ID, "UINT32 Dispatch(), 7 error 0x%x \n", MsgID);
						CSW_PROFILE_FUNCTION_EXIT(Dispatch);
						return ERR_AOM_MISMATCH_MSG;
					}

				}
				else
				{
					CSW_TRACE(CFW_AOM_TS_ID, "UINT32 Dispatch(), 8 error 0x%x \n", MsgID);
					CSW_PROFILE_FUNCTION_EXIT(Dispatch);
					return ERR_AOM_MISMATCH_MSG;
				}

				break;    

		    //************************************************************************
		    //                    NET WORK
		    //************************************************************************
		    case CFW_NW_SRV_ID:
				nActiveAONumber = AO_ControlBlock[nServiceId].ActiveNum;
				// Give the message to static AO OR Dynamic AO. 
				if ( nActiveAONumber >= 1 )    
				{    
					pAOC = AO_ControlBlock[nServiceId].AcitveAO;
					if (pAOC->pAO_Proc != NULL)
						pAOC->pAO_Proc((HAO)pAOC, Msg); 

					//Check if this AO has been unregistered.
					QUERY_AO_ILLEGAL(pAOC);

					if( ( (pAOC->nProcCode) & CFW_AO_PROC_CODE_NOT_CONSUMED) == 
											CFW_AO_PROC_CODE_NOT_CONSUMED )
					{
						pAOC->nProcCode =( pAOC->nProcCode & (CFW_AO_PROC_CODE_NOT_CONSUMED ^ 0xFFFFFFFF) );

						if ( nActiveAONumber == 2 )
						{
							pAOC = AO_ControlBlock[nServiceId].AcitveAO->NextAO;

							if (pAOC->pAO_Proc != NULL) 
								pAOC->pAO_Proc((HAO)pAOC, Msg);
						}
						else
						{
							CSW_TRACE(CFW_AOM_TS_ID, "UINT32 Dispatch(), 9 error  \n");
							CSW_PROFILE_FUNCTION_EXIT(Dispatch);
							return ERR_AOM_NW_FAILURE;
						}
					}
				}
				else
				{
					CSW_TRACE(CFW_AOM_TS_ID, "UINT32 Dispatch(), 10 error  \n");
					CSW_PROFILE_FUNCTION_EXIT(Dispatch);
					return ERR_AOM_NW_FAILURE;                    
				}

				break;
		        
		    //************************************************************************
		    //                   SIM
		    //************************************************************************
		    case CFW_SIM_SRV_ID:
#if YJX_SIM_VER
				nActiveAONumber = AO_ControlBlock[nServiceId].ActiveNum;
				// Give the message to static AO OR Dynamic AO. 
				if ( nActiveAONumber >= 1 )    
				{    
					pAOC = AO_ControlBlock[nServiceId].AcitveAO;
					if (pAOC->pAO_Proc != NULL)
					pAOC->pAO_Proc((HAO)pAOC, Msg);

					//Check if this AO has been unregistered.
					QUERY_AO_ILLEGAL(pAOC);

					if( ( (pAOC->nProcCode) & CFW_AO_PROC_CODE_NOT_CONSUMED) == 
					CFW_AO_PROC_CODE_NOT_CONSUMED )
					{
						pAOC->nProcCode =( pAOC->nProcCode & (CFW_AO_PROC_CODE_NOT_CONSUMED ^ 0xFFFFFFFF) );

						if ( nActiveAONumber == 2 )
						{
							pAOC = AO_ControlBlock[nServiceId].AcitveAO->NextAO;

							if (pAOC->pAO_Proc != NULL)
								pAOC->pAO_Proc((HAO)pAOC, Msg);
						}
						else
						{
							CSW_TRACE(CFW_AOM_TS_ID, "UINT32 Dispatch(), 11 error  \n");
							CSW_PROFILE_FUNCTION_EXIT(Dispatch);
							return ERR_AOM_SIM_FAILURE;
						}
					}
				}
				else
				{
					CSW_TRACE(CFW_AOM_TS_ID, "UINT32 Dispatch(), 12 error  \n");
					CSW_PROFILE_FUNCTION_EXIT(Dispatch);
					return ERR_AOM_SIM_FAILURE;   
				}

				break;                
#endif

#if WCF_SIM_VER
				nActiveAONumber = AO_ControlBlock[nServiceId].ActiveNum;

				if( MsgID == API_SIM_STATUSERROR_IND )    
				{    
					if(Event_Ind_Fun[EVENT_IND_SIM])
					pAOC = (AO_CONTENT *)Event_Ind_Fun[EVENT_IND_SIM](Msg);
					else
					{
						CSW_TRACE(CFW_AOM_TS_ID, "UINT32 Dispatch(), 13 error  \n");
						CSW_PROFILE_FUNCTION_EXIT(Dispatch);
						return ERR_AOM_SIM_FAILURE;
					}

					pAOC = AO_ControlBlock[nServiceId].AcitveAO;

					if (pAOC->pAO_Proc != NULL)
					pAOC->pAO_Proc((HAO)pAOC, Msg);

				}
				else if (nActiveAONumber == 1)    
				{    
					CSW_TRACE(CFW_AOM_TS_ID, "ActiveAONum[1] nServiceID[%d]\n",nServiceId);
          
					pAOC = AO_ControlBlock[nServiceId].AcitveAO;
					if (pAOC->pAO_Proc != NULL)
					{
	           			pAOC->pAO_Proc((HAO)pAOC, Msg);
   						CSW_TRACE(CFW_AOM_TS_ID, "Process nEventId[%x]\n",Msg->nEventId);

          			}
					
					if( ( (pAOC->nProcCode) & CFW_AO_PROC_CODE_NOT_CONSUMED) == 
						CFW_AO_PROC_CODE_NOT_CONSUMED )
					{
						CSW_SAT_InitEX(Msg);          

					}
				}
				else if(  API_SIM_FETCH_IND == Msg->nEventId )//Add by lixp at 20080821 for fetch IND.
				{

					int j = 0x00;
					UINT8 *pDate = (UINT8 *)Msg->nParam1;
					for( ; j < 10; j++ )
					{
						CSW_TRACE(CFW_AOM_TS_ID, "Dispatch(), pDate[%x]= 0x%x \r\n",i, pDate[i]); 			   	
					}

					CSW_SAT_InitEX(Msg);          
					return 0;
				}
				else
				{
					CSW_TRACE(CFW_AOM_TS_ID, "Dispatch(), 14 error  \n");
					CSW_PROFILE_FUNCTION_EXIT(Dispatch);
					return ERR_AOM_SIM_FAILURE;
				}
				break;
#endif

		    //************************************************************************
		    //                    SMS MO
		    //************************************************************************
		    case CFW_SMS_MO_SRV_ID:
				nActiveAONumber = AO_ControlBlock[nServiceId].ActiveNum;
				if (nActiveAONumber == 1)    
				{    
					pAOC = AO_ControlBlock[nServiceId].AcitveAO;
					if (pAOC->pAO_Proc != NULL)
						pAOC->pAO_Proc((HAO)pAOC, Msg);

				}
				else
				{
					CSW_TRACE(CFW_AOM_TS_ID, "Dispatch(), 15 error  \n");
					CSW_PROFILE_FUNCTION_EXIT(Dispatch);
					return ERR_AOM_SMS_MO_FAILURE;
				}

				break;
		        
		    //************************************************************************
		    //                    SMS MT
		    //************************************************************************
		    case CFW_SMS_MT_SRV_ID:
				nActiveAONumber = AO_ControlBlock[nServiceId].ActiveNum;
				if (nActiveAONumber == 0)    
				{    
					if(Event_Ind_Fun[EVENT_IND_SMS_MT])
						pAOC = (AO_CONTENT *)Event_Ind_Fun[EVENT_IND_SMS_MT](Msg);
					else
					{
	  					CSW_TRACE(CFW_AOM_TS_ID, "Dispatch(), 16 error  \n");
						CSW_PROFILE_FUNCTION_EXIT(Dispatch);
						return ERR_AOM_SMS_MT_FAILURE;
					}

					pAOC = AO_ControlBlock[nServiceId].AcitveAO;

					if (pAOC->pAO_Proc != NULL)
						pAOC->pAO_Proc((HAO)pAOC, Msg);

				}
				else 
				{
  					CSW_TRACE(CFW_AOM_TS_ID, "Dispatch(), 17 error  \n");
					CSW_PROFILE_FUNCTION_EXIT(Dispatch);
					return ERR_AOM_SMS_MT_FAILURE;
				}

				break;
		        
		    //************************************************************************
		    //                    SS
		    //************************************************************************
		    case CFW_SS_SRV_ID:
				nActiveAONumber = AO_ControlBlock[nServiceId].ActiveNum;

				if (MsgID == API_SS_ACTION_IND )
				{
					i = 0;
					//Call the register AO function.
					if(Event_Ind_Fun[EVENT_IND_USSD])////
						pAOC = (AO_CONTENT *)Event_Ind_Fun[EVENT_IND_USSD](Msg);
					else
					{
	  					CSW_TRACE(CFW_AOM_TS_ID, "Dispatch 18 error  \n");
						CSW_PROFILE_FUNCTION_EXIT(Dispatch);
						return ERR_AOM_EMPTY_REGISTER_FUNCTION;
					}

					pAOC->AO_State = CFW_SM_STATE_IDLE;

					if (pAOC->pAO_Proc != NULL)
						pAOC->pAO_Proc((HAO)pAOC, Msg);

					break;
				}

				pAOC = AO_ControlBlock[nServiceId].AcitveAO;
				i = 0;				

				while(pAOC && ( i < nActiveAONumber )) 
				{
					if(pAOC->pAO_Proc)
					{
						pAOC->pAO_Proc((HAO)pAOC, Msg);    	

						//Check if this AO has been unregistered.
						QUERY_AO_ILLEGAL_BREAK(pAOC);

						//This notify event has been consumed.
						if( ( (pAOC->nProcCode) & CFW_AO_PROC_CODE_CONSUMED) == 
												CFW_AO_PROC_CODE_CONSUMED )
						{
							pAOC->nProcCode = ( pAOC->nProcCode & (CFW_AO_PROC_CODE_CONSUMED ^ 0xFFFFFFFF) );

							break;
						}
					}

					pAOC = pAOC->NextAO;
					i++; 

				} 
				break;
		    //************************************************************************
		    //                    Engineering MOde
		    //************************************************************************
		    case CFW_EMOD_SRV_ID:
				nActiveAONumber = AO_ControlBlock[nServiceId].ActiveNum;
				if (nActiveAONumber == 1)    
				{    
					pAOC = AO_ControlBlock[nServiceId].AcitveAO;
					if (pAOC->pAO_Proc != NULL)
						pAOC->pAO_Proc((HAO)pAOC, Msg);

				}
				else
				{
  					CSW_TRACE(CFW_AOM_TS_ID, "Dispatch 19 error  \n");
					CSW_PROFILE_FUNCTION_EXIT(Dispatch);
					return ERR_CFW_NOT_EXIST_AO;//ERR_AOM_SS_FAILURE
				}

				break;                
		    //************************************************************************
		    //                    GPRS 
		    //************************************************************************
		    case CFW_GPRS_SRV_ID:
			nActiveAONumber = AO_ControlBlock[nServiceId].ActiveNum;

		        // If incoming data call, need to register one
		        if ( MsgID == API_PDP_CTXACTIVATE_IND )
		        {
		            AO_CONTENT  *pTmp = AO_ControlBlock[nServiceId].AcitveAO;                    
		        
		            i = 0;
		            while( (i < nActiveAONumber - 1) && ( pTmp ))
		            {
		                i++;
		                pTmp = pTmp->NextAO;
		            }

		            //Call the register AO function.
		            if(Event_Ind_Fun[EVENT_IND_GPRS])
		                pAOC = (AO_CONTENT *)Event_Ind_Fun[EVENT_IND_GPRS](Msg);
		            else
		            {
					CSW_TRACE(CFW_AOM_TS_ID, "UINT32 Dispatch(), 20 error  \n");
					CSW_PROFILE_FUNCTION_EXIT(Dispatch);
					return ERR_AOM_EMPTY_REGISTER_FUNCTION;
		            }

		            if(pTmp)
		                pTmp->NextAO = pAOC;
		            else
		                AO_ControlBlock[nServiceId].AcitveAO = pAOC; 

		            AO_ControlBlock[nServiceId].ActiveNum++;
		            
		            //Update the AO of incoming data call position.       
		            if( AO_ControlBlock[nServiceId].SpecialAO == pAOC)
		            {
		                AO_ControlBlock[nServiceId].SpecialAO = 0;
		            }
		            else if( AO_ControlBlock[nServiceId].ReadyAO )
		            {
		                AO_CONTENT  *pReadyAOC = AO_ControlBlock[nServiceId].ReadyAO;                                        
		                UINT8 nReadyAONumber = AO_ControlBlock[nServiceId].ReadyNum;

		                if (pReadyAOC == pAOC)
		                {
		                    AO_ControlBlock[nServiceId].ReadyAO = 0;
		                    AO_ControlBlock[nServiceId].ReadyNum = 0;
		                }
		                else
		                {
		                    i = 0;
		                    while(( i < nReadyAONumber - 1 ) && ( pReadyAOC->NextAO != pAOC))
		                    {
		                        i++;
		                        pReadyAOC = pReadyAOC->NextAO;
		                    }                                    

		                    if ( pReadyAOC->NextAO == pAOC )
		                    {
		                        pReadyAOC->NextAO = 0;                               
		                        AO_ControlBlock[nServiceId].ReadyNum--;
		                    }
		                    else
		                    {
						CSW_TRACE(CFW_AOM_TS_ID, "UINT32 Dispatch(), 21 error  \n");
						CSW_PROFILE_FUNCTION_EXIT(Dispatch);
						return ERR_CFW_NOT_EXIST_AO;
		                    }

		                }
		            }
		            else
		            {
					CSW_TRACE(CFW_AOM_TS_ID, "UINT32 Dispatch(), 22 error  \n");
					CSW_PROFILE_FUNCTION_EXIT(Dispatch);
					return ERR_AOM_READY_QU_EMPTY;
		            }
		           
		            pAOC->AO_State = CFW_SM_STATE_IDLE;
		            
		            if (pAOC->pAO_Proc != NULL)
		                pAOC->pAO_Proc((HAO)pAOC, Msg);
		            
		            break;
		        }

		        //Dispatch message to init_active AO.
		        pAOC = AO_ControlBlock[nServiceId].SpecialAO;
		        if( pAOC )
		        {
		            //The Call have been connected 
		            if (MsgID == API_PDP_CTXACTIVATE_CNF)
		            {
		                if (pAOC->pAO_Proc != NULL)
		                    pAOC->pAO_Proc((HAO)pAOC, Msg);
		                
		                //Move the init_active AO--->Actice Queue, if it is possible. 
		                if(nActiveAONumber == 0)
		                {
		                     AO_ControlBlock[nServiceId].AcitveAO = pAOC;
		                }
		                else if( nActiveAONumber < MAX_CC_NUM)
		                {
		                    AO_CONTENT  *pTmp = AO_ControlBlock[nServiceId].AcitveAO;                    
		                    i = 0;
		                    while( (i < nActiveAONumber - 1) && ( pTmp ))
		                    {
		                        i++;
		                        pTmp = pTmp->NextAO;
		                    }
		                    pTmp->NextAO = pAOC;
		                }
		                AO_ControlBlock[nServiceId].ActiveNum++;    

		                //Move the first ready AO to init_active, if it is possible. 
		                nActiveAONumber = AO_ControlBlock[nServiceId].ActiveNum;
		                
		                if( nActiveAONumber < MAX_CC_NUM && AO_ControlBlock[nServiceId].ReadyAO ) 
		                {
		                    AO_ControlBlock[nServiceId].SpecialAO = AO_ControlBlock[nServiceId].ReadyAO;
		                    pAOC = AO_ControlBlock[nServiceId].SpecialAO;

		                    AO_ControlBlock[nServiceId].ReadyAO = AO_ControlBlock[nServiceId].ReadyAO->NextAO;
		                    
		                    AO_ControlBlock[nServiceId].SpecialAO->NextAO = 0;
		                    
		                    AO_ControlBlock[nServiceId].ReadyNum--;

		                    pAOC->AO_State = CFW_SM_STATE_IDLE;

		                    if (pAOC->pAO_Proc != NULL)
		                        pAOC->pAO_Proc((HAO)pAOC, SPECIAL_MESSAGE);
		                 
		                }
		                else
		                {    
		                    AO_ControlBlock[nServiceId].SpecialAO = 0;
		                }
		                
		                break;
		            } 

		            //Other msgs for init AO
		            if ( (MsgID == API_PDP_CTXACTIVATE_REJ) || (MsgID == API_PDP_CTXSMINDEX_IND) ||(API_PDP_CTXDEACTIVATE_IND ==MsgID))
		            {
		                if (pAOC->pAO_Proc != NULL)
		                    pAOC->pAO_Proc((HAO)pAOC, Msg);
		                
		                break;
		            }

		        }

		        //Dispatch message to active AO.
		        if ( nActiveAONumber > 0 )
		        {
		            pAOC = AO_ControlBlock[nServiceId].AcitveAO;
		            
		            do 
		            {
		                if(pAOC->pAO_Proc)
		                {
		                    pAOC->pAO_Proc((HAO)pAOC, Msg);    	
		                    
		                    //Check if this AO has been unregistered.
		                    QUERY_AO_ILLEGAL_BREAK(pAOC);
		                                                                                
		                    //This notify event has been consumed.
		                    if( ( (pAOC->nProcCode) & CFW_AO_PROC_CODE_CONSUMED) == 
		                                            CFW_AO_PROC_CODE_CONSUMED )
		                    {
		                        pAOC->nProcCode = ( pAOC->nProcCode & (CFW_AO_PROC_CODE_CONSUMED ^ 0xFFFFFFFF) );
		                        
		                        break;
		                    }
		                }

		                pAOC = pAOC->NextAO;
		                i++; 

		            } while(pAOC && ( i < nActiveAONumber ));
		        }
		        else
		        {
				CSW_TRACE(CFW_AOM_TS_ID, "UINT32 Dispatch(), 22 error  \n");
				CSW_PROFILE_FUNCTION_EXIT(Dispatch);
				return ERR_AOM_MISMATCH_MSG;
		        }

		        break;    
		        
		    //************************************************************************
		    //                    TIMER 
		    //************************************************************************            
		    case CFW_TIMER_SRV_ID:
				CSW_TRACE(CFW_AOM_TS_ID, "Timer coming  \n");
				if(Event_Ind_Fun[EVENT_IND_TIMER])
					Event_Ind_Fun[EVENT_IND_TIMER](Msg);

				break;
		    //                    Default option
		    //************************************************************************            
		    default:
			 CSW_TRACE(CFW_AOM_TS_ID, "UINT32 Dispatch(), 23 error  \n");
		        break;

	}//SWITCH
	//CSW_TRACE(CFW_AOM_TS_ID, "UINT32 Dispatch(), 24 error  \n");
	CSW_PROFILE_FUNCTION_EXIT(Dispatch);
	return ERR_SUCCESS;
}


//=======================================================================
//
//=======================================================================
static void AddNewAOToReadyQu( UINT32 nServiceId, AO_CONTENT *NewAO)
{
    UINT8  nReadyAONumber = 0;
    AO_CONTENT *pAOC = 0;
    UINT8 i = 0;
    
    nReadyAONumber = AO_ControlBlock[nServiceId].ReadyNum;
    if( nReadyAONumber == 0)
    {
        AO_ControlBlock[nServiceId].ReadyAO = NewAO;
    }
    else
    {
        pAOC = AO_ControlBlock[nServiceId].ReadyAO;                    
        i = 0;
        while( i < nReadyAONumber - 1 )
        {
            i++;
            pAOC = pAOC->NextAO;
        }
        pAOC->NextAO = NewAO;
 
    }
    NewAO->AO_State = CFW_SM_STATE_READY;
    AO_ControlBlock[nServiceId].ReadyNum++;

}

//=======================================================================
//
//=======================================================================
#ifdef CFW_MULTI_SIM
#if 0
UINT32 _GetAOCount()
{
  UINT32 nCount =   CFW_GetAoCount(   CFW_NW_SRV_ID    ,CFW_SIM_COUNT);
  nCount += CFW_GetAoCount(   CFW_SIM_SRV_ID   ,CFW_SIM_COUNT);
  nCount += CFW_GetAoCount(   CFW_CC_SRV_ID    ,CFW_SIM_COUNT);
  nCount += CFW_GetAoCount(   CFW_SS_SRV_ID    ,CFW_SIM_COUNT);
  nCount += CFW_GetAoCount(   CFW_SMS_MO_SRV_ID,CFW_SIM_COUNT);
  nCount += CFW_GetAoCount(   CFW_SMS_MT_SRV_ID,CFW_SIM_COUNT);
  nCount += CFW_GetAoCount(   CFW_SMS_CB_ID    ,CFW_SIM_COUNT);
  nCount += CFW_GetAoCount(   CFW_GPRS_SRV_ID  ,CFW_SIM_COUNT);
  nCount += CFW_GetAoCount(   CFW_CSD_SRV_ID   ,CFW_SIM_COUNT);
  nCount += CFW_GetAoCount(   CFW_EMOD_SRV_ID  ,CFW_SIM_COUNT);
  nCount += CFW_GetAoCount(   CFW_APP_SRV_ID   ,CFW_SIM_COUNT);

  return nCount;
}
#endif
#endif


#ifdef CFW_MULTI_SIM
HAO CFW_RegisterAo( UINT32 nServiceId, PVOID pUserData, PFN_AO_PORC pfnAoProc,CFW_SIM_ID nSimID)
#else
HAO CFW_RegisterAo( UINT32 nServiceId, PVOID pUserData, PFN_AO_PORC pfnAoProc)
#endif
{
	UINT8  nActiveAONumber = 0; 
	UINT8  nInitAONumber = 0;

	AO_CONTENT *NewAO = 0;
	AO_CONTENT *pAOC = 0;
       UINT32 nServiceIdO =nServiceId;


#ifdef CFW_MULTI_SIM
	//CSW_TRACE(CFW_AOM_TS_ID, "===================== _GetAOCount %d\n",_GetAOCount());
#endif 
	CSW_PROFILE_FUNCTION_ENTER(CFW_RegisterAo);
	NewAO = (AO_CONTENT*)CSW_AOM_MALLOC( sizeof(AO_CONTENT) );
	CSW_TRACE(CFW_AOM_TS_ID, " CFW_RegisterAo NewAO: 0x%x\n", NewAO);

	if(NewAO == 0)
	{
		CSW_TRACE(CFW_AOM_TS_ID, " CFW_RegisterAo melloc error \n");
		CSW_PROFILE_FUNCTION_EXIT(CFW_RegisterAo);
		return (HAO)NULL;
	}


#ifdef CFW_MULTI_SIM
 	nServiceId =(nServiceId*CFW_SIM_COUNT+nSimID);
#endif

	SUL_ZeroMemory32(NewAO, SIZEOF(AO_CONTENT));

	NewAO->NextAO = NULL;
	NewAO->nProcCode = 0;
	NewAO->pAO_Proc = pfnAoProc;
	NewAO->UserData = pUserData;
	NewAO->UTI = 0;
	NewAO->nServiceID = nServiceId;
#ifdef CFW_MULTI_SIM
 	NewAO->nSimID =nSimID;
#endif


	nActiveAONumber = AO_ControlBlock[nServiceId].ActiveNum;

	QUERY_AO_ILLEGAL(NewAO);
       CSW_TRACE(CFW_AOM_TS_ID, "CFW_RegisterAo nActiveAONumber %d nServiceId = 0x%x\n", nActiveAONumber,nServiceId);
    	switch(nServiceIdO)
	{
		case CFW_CC_SRV_ID:
			//comment add by wys 2007-05-24
			//"specialAO" present a initial call, if there is a initial call, put the new one into "ReadyAO"
			if(AO_ControlBlock[nServiceId].SpecialAO)
			{
				nInitAONumber = 1;
			}

			if ( nActiveAONumber + nInitAONumber < MAX_CC_NUM )
			{
				//Init Active has had ONE AO to wait.
				if(AO_ControlBlock[nServiceId].SpecialAO)
				{  
					AddNewAOToReadyQu(nServiceId, NewAO);
				} 
				else
				{
					AO_ControlBlock[nServiceId].SpecialAO = NewAO;
					NewAO->AO_State = CFW_SM_STATE_IDLE;
				}
			}
			else
			{
				AddNewAOToReadyQu(nServiceId, NewAO);
			}
			//add for QQ, 2010-05-10
			#ifdef CFW_TCPIP_SUPPORT
			if((nActiveAONumber>0)||(AO_ControlBlock[nServiceId].SpecialAO))
			{
			   CFW_SmarchCCStatus(0x01);
			}
			#endif
			//add end
			break;

		case CFW_SMS_MO_SRV_ID:            
		case CFW_EMOD_SRV_ID:   
		case CFW_SMS_MT_SRV_ID:
#if WCF_SIM_VER            
		case CFW_SIM_SRV_ID:
#endif

			if ( nActiveAONumber == 0 )
			{
				AO_ControlBlock[nServiceId].AcitveAO = NewAO;
				NewAO->AO_State = CFW_SM_STATE_IDLE;
				AO_ControlBlock[nServiceId].ActiveNum++;
			}
			else
			{
        //CSW_TRACE(CFW_AOM_TS_ID, "ActAO 0x%x\n",(UINT32)(AO_ControlBlock[nServiceId].AcitveAO->pAO_Proc));
				AddNewAOToReadyQu(nServiceId, NewAO);
			}

			break;

		case CFW_SS_SRV_ID://add wuys 2007-06-19
			if ( nActiveAONumber == 0 )
			{
				AO_ControlBlock[nServiceId].AcitveAO = NewAO;
				NewAO->AO_State = CFW_SM_STATE_IDLE;
				AO_ControlBlock[nServiceId].ActiveNum++;
			}
      #if 0
			else if(nActiveAONumber == 1) 
			{
				AO_ControlBlock[nServiceId].AcitveAO->NextAO = NewAO;
				NewAO->AO_State = CFW_SM_STATE_IDLE;
				AO_ControlBlock[nServiceId].ActiveNum++;
			}
      #endif
			else
			{
				AddNewAOToReadyQu(nServiceId, NewAO);
			}

			break;

		case CFW_NW_SRV_ID:
#if YJX_SIM_VER            
		case CFW_SIM_SRV_ID:
#endif
			if ( nActiveAONumber == 0 )//This must be the static AO. 
			{
				AO_ControlBlock[nServiceId].AcitveAO = NewAO;
				NewAO->AO_State = CFW_SM_STATE_IDLE;
				AO_ControlBlock[nServiceId].ActiveNum++;
			}
			else if(nActiveAONumber == 1)//This must be the dynamic AO. 
			{
				AO_ControlBlock[nServiceId].AcitveAO->NextAO = NewAO;
				NewAO->AO_State = CFW_SM_STATE_IDLE;
				AO_ControlBlock[nServiceId].ActiveNum++;
			}
			else
			{
				AddNewAOToReadyQu(nServiceId, NewAO);
			}

			break;

		case CFW_APP_SRV_ID:
			pAOC = AO_ControlBlock[nServiceId].AcitveAO;

			NewAO->AO_State = CFW_SM_STATE_IDLE;

			if( !pAOC )
			{    
				AO_ControlBlock[nServiceId].AcitveAO = NewAO;
				AO_ControlBlock[nServiceId].ActiveNum++;
				break;
			}

			while( pAOC->NextAO )
			{
				pAOC = pAOC->NextAO;
			}
			pAOC->NextAO = NewAO;
			AO_ControlBlock[nServiceId].ActiveNum++;
			break;
			
		case CFW_GPRS_SRV_ID:
			if(AO_ControlBlock[nServiceId].SpecialAO)
			{
				nInitAONumber = 1;
			}

			if ( nActiveAONumber + nInitAONumber < MAX_CC_NUM )
			{
				/*Init Active has had ONE AO to wait.*/
				if(AO_ControlBlock[nServiceId].SpecialAO)
				{  
					AddNewAOToReadyQu(nServiceId, NewAO);
				} 
				else
				{
					AO_ControlBlock[nServiceId].SpecialAO = NewAO;
					NewAO->AO_State = CFW_SM_STATE_IDLE;
				}
			}
			else
			{
				AddNewAOToReadyQu(nServiceId, NewAO);
			}

			break;    

		default:
			CSW_TRACE(CFW_AOM_TS_ID, "CFW_RegisterAo(), default error  \n");
			break;


	}
	CSW_PROFILE_FUNCTION_EXIT(CFW_RegisterAo);
				CSW_TRACE(CFW_AOM_TS_ID, "CFW_RegisterAo() end, NewAO: 0x%x  \n",NewAO);

	return (HAO)NewAO;
}

#define ACTIVE_AO   1
#define READY_AO    0
/*=============================================================================

=============================================================================*/
static UINT32 DeleteReadyAO(UINT32 nServiceId, AO_CONTENT *pCurrentAO, UINT8 type )
{
    AO_CONTENT *pAOC = 0;
    AO_CONTENT *pAOC_pre = 0;

    UINT8  nAONumber = 0; 
    
    UINT8 i = 0;
    CSW_PROFILE_FUNCTION_ENTER(DeleteReadyAO);
    if(type == READY_AO)
    {
        nAONumber = AO_ControlBlock[nServiceId].ReadyNum;
        pAOC = AO_ControlBlock[nServiceId].ReadyAO;
    }
    else if(type == ACTIVE_AO)
    {
        nAONumber = AO_ControlBlock[nServiceId].ActiveNum;
        pAOC = AO_ControlBlock[nServiceId].AcitveAO;
    }
		CSW_TRACE(CFW_AOM_TS_ID, "DeleteReadyAO 3\n");
		CSW_TRACE(CFW_AOM_TS_ID, "pAOC 0x%x pCurrentAO 0x%x \n",pAOC,pCurrentAO);

    if( pAOC == pCurrentAO )
    {
		CSW_TRACE(CFW_AOM_TS_ID, "DeleteReadyAO 8\n");
        if(type == READY_AO)
        {
            AO_ControlBlock[nServiceId].ReadyAO = pAOC->NextAO;
            AO_ControlBlock[nServiceId].ReadyNum--;
        }
        else if(type == ACTIVE_AO)
        {
            AO_ControlBlock[nServiceId].AcitveAO = pAOC->NextAO;
            AO_ControlBlock[nServiceId].ActiveNum--;
        }
		CSW_TRACE(CFW_AOM_TS_ID, "DeleteReadyAO 1\n");

        CSW_AOM_FREE(pCurrentAO->UserData); // Or not!

        CSW_AOM_FREE(pCurrentAO);
		CSW_TRACE(CFW_AOM_TS_ID, "DeleteReadyAO 2\n");

        CSW_TRACE(CFW_AOM_TS_ID, TSTXT("FREE For AO_UNREG %x \n"), (UINT32)pCurrentAO);

        CSW_PROFILE_FUNCTION_EXIT(DeleteReadyAO);
        return ERR_SUCCESS;

    }
    pAOC_pre = pAOC;
        		CSW_TRACE(CFW_AOM_TS_ID, "DeleteReadyAO 6 pAOC 0x%x\n",pAOC);
    pAOC = pAOC->NextAO;
        		CSW_TRACE(CFW_AOM_TS_ID, "DeleteReadyAO 4\n");

    while( i < nAONumber -1 )
    {
        if( pAOC == pCurrentAO )
        {
            pAOC_pre->NextAO = pAOC->NextAO;

        		CSW_TRACE(CFW_AOM_TS_ID, "DeleteReadyAO 10\n");
            CSW_AOM_FREE(pCurrentAO->UserData); // Or not!
        		CSW_TRACE(CFW_AOM_TS_ID, "DeleteReadyAO 11\n");
            CSW_AOM_FREE(pCurrentAO);
        		CSW_TRACE(CFW_AOM_TS_ID, "DeleteReadyAO 12\n");

            CSW_TRACE(CFW_AOM_TS_ID, TSTXT("FREE For AO_UNREG %x\n"), (UINT32)pCurrentAO);

            if(type == READY_AO)
            {
                AO_ControlBlock[nServiceId].ReadyNum--;
            }
            else if(type == ACTIVE_AO)
            {
                AO_ControlBlock[nServiceId].ActiveNum--;
            }

           CSW_PROFILE_FUNCTION_EXIT(DeleteReadyAO);
            return ERR_SUCCESS;
            
        }
        else
        {
            pAOC_pre = pAOC;
            pAOC = pAOC->NextAO;
            i++;
        }

    }
    
    CSW_PROFILE_FUNCTION_EXIT(DeleteReadyAO);
    return ERR_CFW_NOT_EXIST_AO;

}


//=====================================================================
//Unregister the dynamic AO. This action may lead to AO scheduling.
//
//=====================================================================
UINT32 CFW_UnRegisterAO(UINT32 nServiceId, HANDLE hAo)
{
	AO_CONTENT *pAOC = 0;

	UINT8  nActiveAONumber = 0; 
       UINT8 nUTI = 0;
	AO_CONTENT *pCurrentAO = (AO_CONTENT *)hAo;
       UINT32 nServiceIdO =nServiceId;

	CSW_PROFILE_FUNCTION_ENTER(CFW_UnRegisterAO);
	if(!hAo)
	{
		CSW_PROFILE_FUNCTION_EXIT(CFW_UnRegisterAO);
		return ERR_AOM_UNREG_FAILURE;
	}

	QUERY_AO_ILLEGAL(pCurrentAO);

#ifdef CFW_MULTI_SIM
	nServiceId =(nServiceId*CFW_SIM_COUNT+pCurrentAO->nSimID);
#endif


	//    Free using UTI for Extended APP 
	//now, we don't use service ID,if want use more UTI,we can use service ID.
	if(!LOUINT8((pCurrentAO->UTI>>16)&0x000000FF))
	{
		nUTI = LOUINT8((pCurrentAO->UTI>>8)&0x000000FF);
		ReleaseUsingUTI(0,nUTI);
	}
	//    To do...

       CSW_TRACE(CFW_AOM_TS_ID, "CFW_UnRegisterAO(),  nServiceId = 0x%x\n", nServiceId);
	switch( nServiceIdO )
	{
		case CFW_CC_SRV_ID:
			// pCurrentAO is Init Active AO 
			if ( AO_ControlBlock[nServiceId].SpecialAO == pCurrentAO)
			{
				CSW_AOM_FREE(pCurrentAO->UserData); 

				CSW_AOM_FREE(pCurrentAO);

				CSW_TRACE(CFW_AOM_TS_ID, TSTXT("FREE For AO_UNREG %x\n"), (UINT32)pCurrentAO);

				AO_ControlBlock[nServiceId].SpecialAO = 0;

				if(AO_ControlBlock[nServiceId].ReadyAO)
				{
					nActiveAONumber = AO_ControlBlock[nServiceId].ActiveNum;

					if( nActiveAONumber < MAX_CC_NUM && AO_ControlBlock[nServiceId].ReadyAO ) 
					{
						AO_ControlBlock[nServiceId].SpecialAO = AO_ControlBlock[nServiceId].ReadyAO;
						pAOC = AO_ControlBlock[nServiceId].SpecialAO;

						AO_ControlBlock[nServiceId].ReadyAO = AO_ControlBlock[nServiceId].ReadyAO->NextAO;
						AO_ControlBlock[nServiceId].SpecialAO->NextAO = 0;
						AO_ControlBlock[nServiceId].ReadyNum--;

						pAOC->AO_State = CFW_SM_STATE_IDLE;
						CFW_SetAoProcCode((HAO)pAOC, CFW_AO_PROC_CODE_CONTINUE); 
					}
				}
				CSW_TRACE(CFW_AOM_TS_ID, "CFW_UnRegisterAO(), error\n");
				CSW_PROFILE_FUNCTION_EXIT(CFW_UnRegisterAO);
				//add for QQ, 2010-05-10
			#ifdef CFW_TCPIP_SUPPORT
			if(( AO_ControlBlock[nServiceId].ActiveNum==0x00)&&(AO_ControlBlock[nServiceId].SpecialAO==0x00))
			{
			   CFW_SmarchCCStatus(0x00);
			}
			//add end
			#endif
				return ERR_SUCCESS;
			}

			// pCurrentAO is READY AO
			if ( pCurrentAO->AO_State == CFW_SM_STATE_READY )
			{
				DeleteReadyAO( nServiceId, pCurrentAO, READY_AO );
			}
			// pCurrentAO is real Active AO
			else
			{
				DeleteReadyAO(nServiceId, pCurrentAO, ACTIVE_AO );

				nActiveAONumber = AO_ControlBlock[nServiceId].ActiveNum;

				if( nActiveAONumber < MAX_CC_NUM && 
					AO_ControlBlock[nServiceId].ReadyAO &&
					!AO_ControlBlock[nServiceId].SpecialAO) 
				{
					AO_ControlBlock[nServiceId].SpecialAO = AO_ControlBlock[nServiceId].ReadyAO;
					pAOC = AO_ControlBlock[nServiceId].SpecialAO;

					AO_ControlBlock[nServiceId].ReadyAO = AO_ControlBlock[nServiceId].ReadyAO->NextAO;
					AO_ControlBlock[nServiceId].SpecialAO->NextAO = 0;
					AO_ControlBlock[nServiceId].ReadyNum--;

					pAOC->AO_State = CFW_SM_STATE_IDLE;
					CFW_SetAoProcCode((HAO)pAOC, CFW_AO_PROC_CODE_CONTINUE); 

				}
			}
			//add for QQ, 2010-05-10
			#ifdef CFW_TCPIP_SUPPORT
			if(( AO_ControlBlock[nServiceId].ActiveNum==0x00)&&(AO_ControlBlock[nServiceId].SpecialAO==0x00))
			{
			   CFW_SmarchCCStatus(0x00);
			}
			//add end
			#endif
			break;

		case CFW_SMS_MO_SRV_ID:            
		case CFW_SMS_MT_SRV_ID:
		case CFW_EMOD_SRV_ID:
#if WCF_SIM_VER                        
		case CFW_SIM_SRV_ID:
#endif               
			//CSW_TRACE(CFW_AOM_TS_ID, "CFW_UnRegisterAO 1,\n");

			nActiveAONumber = AO_ControlBlock[nServiceId].ActiveNum;
			pAOC = AO_ControlBlock[nServiceId].AcitveAO;
			if( pAOC == pCurrentAO )
			{
				//CSW_TRACE(CFW_AOM_TS_ID, "CFW_UnRegisterAO 2,\n");
				void *pT = pCurrentAO->UserData;
				AO_ControlBlock[nServiceId].AcitveAO = 0;

				CSW_AOM_FREE(pT); // Or not!
				CSW_AOM_FREE(pCurrentAO);
				pT               = NULL;
				pCurrentAO = NULL;
				//CSW_TRACE(CFW_AOM_TS_ID, "CFW_UnRegisterAO 3\n");
				if(AO_ControlBlock[nServiceId].ReadyAO)
				{
					//CSW_TRACE(CFW_AOM_TS_ID, "CFW_UnRegisterAO 4\n");
					AO_ControlBlock[nServiceId].AcitveAO = AO_ControlBlock[nServiceId].ReadyAO;
					pAOC = AO_ControlBlock[nServiceId].AcitveAO;

					AO_ControlBlock[nServiceId].ReadyAO = AO_ControlBlock[nServiceId].ReadyAO->NextAO;;
					AO_ControlBlock[nServiceId].ReadyNum--;

					pAOC->AO_State = CFW_SM_STATE_IDLE;
					CFW_SetAoProcCode((HAO)pAOC, CFW_AO_PROC_CODE_CONTINUE); 

				}
				else
				{    
					AO_ControlBlock[nServiceId].ActiveNum--;
				}
				CSW_TRACE(CFW_AOM_TS_ID, "CFW_UnRegisterAO 5\n");
				CSW_PROFILE_FUNCTION_EXIT(CFW_UnRegisterAO);
				return ERR_SUCCESS;
			}
			else
			{
				CSW_TRACE(CFW_AOM_TS_ID, "CFW_UnRegisterAO 6\n");
				//Delete Ready AO
				DeleteReadyAO( nServiceId, pCurrentAO, READY_AO );
			}

			break;

		case CFW_SS_SRV_ID:  //add wys 2007-06-19
			pAOC=AO_ControlBlock[nServiceId].AcitveAO;

			UINT8 i=0;
			while((pAOC!=pCurrentAO) && (pAOC!=NULL))
			{
				i++;
				pAOC=pAOC->NextAO;

			}
			if(pAOC==pCurrentAO)
			{
				void *pT = pCurrentAO->UserData;
				if(0==i)
				{
					if(pAOC->NextAO)
					{
						AO_ControlBlock[nServiceId].AcitveAO = pAOC->NextAO;
					}
					else
					{
						AO_ControlBlock[nServiceId].AcitveAO = NULL;
					}
				}
				else
				{
					AO_ControlBlock[nServiceId].AcitveAO->NextAO= NULL;
				}
				CSW_AOM_FREE(pT); // Or not!
				CSW_AOM_FREE(pCurrentAO);

				if(AO_ControlBlock[nServiceId].ReadyAO)
				{
					AO_ControlBlock[nServiceId].AcitveAO = AO_ControlBlock[nServiceId].ReadyAO;
					pAOC = AO_ControlBlock[nServiceId].AcitveAO;

					AO_ControlBlock[nServiceId].ReadyAO = AO_ControlBlock[nServiceId].ReadyAO->NextAO;;
					AO_ControlBlock[nServiceId].ReadyNum--;

					pAOC->AO_State = CFW_SM_STATE_IDLE;
					CFW_SetAoProcCode((HAO)pAOC, CFW_AO_PROC_CODE_CONTINUE); 

				}
				else
				{    
					AO_ControlBlock[nServiceId].ActiveNum--;
				}

			}
			else
			{
				DeleteReadyAO( nServiceId, pCurrentAO, READY_AO );
			}
			break;

		case CFW_NW_SRV_ID:

#if YJX_SIM_VER                        
		case CFW_SIM_SRV_ID:
#endif            
			nActiveAONumber = AO_ControlBlock[nServiceId].ActiveNum;

			pAOC = AO_ControlBlock[nServiceId].AcitveAO;

			if( pAOC == pCurrentAO )
			{
				//The static AO maybe not be unregistered, but the system also support you to do that... 
				CSW_PROFILE_FUNCTION_EXIT(CFW_UnRegisterAO);
				return ERR_SUCCESS;
			}
			else if( pCurrentAO == pAOC->NextAO )
			{
				AO_ControlBlock[nServiceId].AcitveAO->NextAO = 0;

				CSW_AOM_FREE(pCurrentAO->UserData); // Or not!

				CSW_AOM_FREE(pCurrentAO);

				CSW_TRACE(CFW_AOM_TS_ID, TSTXT("FREE For AO_UNREG %x\n"), (UINT32)pCurrentAO);

				if(AO_ControlBlock[nServiceId].ReadyAO)
				{

					AO_ControlBlock[nServiceId].AcitveAO->NextAO = AO_ControlBlock[nServiceId].ReadyAO;
					pAOC = AO_ControlBlock[nServiceId].ReadyAO;

					AO_ControlBlock[nServiceId].ReadyAO = AO_ControlBlock[nServiceId].ReadyAO->NextAO;;
					AO_ControlBlock[nServiceId].ReadyNum--;

					pAOC->AO_State = CFW_SM_STATE_IDLE;
					CFW_SetAoProcCode((HAO)pAOC, CFW_AO_PROC_CODE_CONTINUE); 

				}
				else
				{    
					AO_ControlBlock[nServiceId].ActiveNum--;
				}
				CSW_PROFILE_FUNCTION_EXIT(CFW_UnRegisterAO);
				return ERR_SUCCESS;
			}
			else
			{
				DeleteReadyAO( nServiceId, pCurrentAO, READY_AO );
			}

			break;

		case CFW_APP_SRV_ID:
			DeleteReadyAO( nServiceId, pCurrentAO, ACTIVE_AO );//wug changed from APP_SVR_ID to this value

			break;

		case CFW_GPRS_SRV_ID:
			// pCurrentAO is Init Active AO 
			if ( AO_ControlBlock[nServiceId].SpecialAO == pCurrentAO)
			{
				CSW_AOM_FREE(pCurrentAO->UserData); 

				CSW_AOM_FREE(pCurrentAO);

				CSW_TRACE(CFW_AOM_TS_ID, TSTXT("FREE For AO_UNREG %x\n"), (UINT32)pCurrentAO);

				AO_ControlBlock[nServiceId].SpecialAO = 0;

				if(AO_ControlBlock[nServiceId].ReadyAO)
				{
					nActiveAONumber = AO_ControlBlock[nServiceId].ActiveNum;

					if( nActiveAONumber < MAX_CC_NUM && AO_ControlBlock[nServiceId].ReadyAO ) 
					{
						AO_ControlBlock[nServiceId].SpecialAO = AO_ControlBlock[nServiceId].ReadyAO;
						pAOC = AO_ControlBlock[nServiceId].SpecialAO;

						AO_ControlBlock[nServiceId].ReadyAO = AO_ControlBlock[nServiceId].ReadyAO->NextAO;
						AO_ControlBlock[nServiceId].SpecialAO->NextAO = 0;
						AO_ControlBlock[nServiceId].ReadyNum--;

						pAOC->AO_State = CFW_SM_STATE_IDLE;
						CFW_SetAoProcCode((HAO)pAOC, CFW_AO_PROC_CODE_CONTINUE); 

					}
				}
				CSW_PROFILE_FUNCTION_EXIT(CFW_UnRegisterAO);
				return ERR_SUCCESS;
			}

			// pCurrentAO is READY AO
			if ( pCurrentAO->AO_State == CFW_SM_STATE_READY )
			{
				DeleteReadyAO( nServiceId, pCurrentAO, READY_AO );
			}
			// pCurrentAO is real Active AO
			else
			{
				DeleteReadyAO( nServiceId, pCurrentAO, ACTIVE_AO );

				nActiveAONumber = AO_ControlBlock[nServiceId].ActiveNum;

				if( nActiveAONumber < MAX_CC_NUM && 
					AO_ControlBlock[nServiceId].ReadyAO &&
					!AO_ControlBlock[nServiceId].SpecialAO) 
				{
					AO_ControlBlock[nServiceId].SpecialAO = AO_ControlBlock[nServiceId].ReadyAO;
					pAOC = AO_ControlBlock[nServiceId].SpecialAO;

					AO_ControlBlock[nServiceId].ReadyAO = AO_ControlBlock[nServiceId].ReadyAO->NextAO;
					AO_ControlBlock[nServiceId].SpecialAO->NextAO = 0;
					AO_ControlBlock[nServiceId].ReadyNum--;

					pAOC->AO_State = CFW_SM_STATE_IDLE;
					CFW_SetAoProcCode((HAO)pAOC, CFW_AO_PROC_CODE_CONTINUE); 

				}
			}
			break;
			default:
				CSW_TRACE(CFW_AOM_TS_ID, "CFW_UnRegisterAO() default error\n");
				break;
		

	}
	CSW_PROFILE_FUNCTION_EXIT(CFW_UnRegisterAO);
	return ERR_SUCCESS;
}
#define _GetRA(store) asm volatile("move $2, $31\n\tsw $2, 0(%0)"::"r"((store)):"$2")

//#define _GetRA(store) asm volatile("sw $31, 0(%0)"::"r"((store)))
//================================================================
//
//Set current AO process result code.
//================================================================
UINT32 g_CallerAdd;
UINT32 SRVAPI CFW_SetAoProcCode(HAO hAO, UINT32 nProcCode) 
{
    AO_CONTENT  *pAOC = (AO_CONTENT *)hAO;
    UINT32  CurrentState = 0;
    UINT32  nServID = 0;
    AO_CB_DOING *pNewOne = 0;       
    UINT32 CallerAdd=0;
    SXS_GET_RA(&CallerAdd);

	g_CallerAdd =CallerAdd;
	
    CSW_TRACE(CFW_AOM_TS_ID, TSTXT("CFW_SetAoProcCode %x\n"), (UINT32)CallerAdd);


    AO_CB_DOING *QuHead = 0;
    AO_CB_DOING *QuTail = 0;
    CSW_PROFILE_FUNCTION_ENTER(CFW_SetAoProcCode);
    
    QUERY_AO_ILLEGAL(pAOC);

    CurrentState = pAOC->AO_State;
    nServID = pAOC->nServiceID;

    pAOC->nProcCode = nProcCode;
    
    QuHead = AO_ControlBlock[nServID].DoingHead;
    QuTail = AO_ControlBlock[nServID].DoingTail;
    
    if(( CurrentState != CFW_SM_STATE_READY ) && (nProcCode == CFW_AO_PROC_CODE_CONTINUE))
    {
        
        pNewOne = (AO_CB_DOING*)CSW_AOM_MALLOC( sizeof(AO_CB_DOING) );
        if( !pNewOne )
            return ERR_AOM_DOING_TAIL_OVER;

        CSW_TRACE(CFW_AOM_TS_ID, TSTXT("Malloc Doing Qu Node %x nServID %x\n"), (UINT32)pNewOne,nServID);
        
        pNewOne->DoingAO = pAOC;
        pNewOne->NextDoing = 0;
        
        CSW_TRACE(CFW_AOM_TS_ID, "CFW_SetAoProcCode nServID %x\n",nServID);
        
        if( QuTail )
        {
            QuTail->NextDoing = pNewOne;
            AO_ControlBlock[nServID].DoingTail = pNewOne;
        }
        else
        {
            AO_ControlBlock[nServID].DoingTail = pNewOne;
            AO_ControlBlock[nServID].DoingHead = pNewOne;
        }

        /*Set the continuing flag*/
	SET_AO_GOONSTATUS(1, nServID) ;
#ifdef CFW_MULTI_SIM
        CFW_AoTriggerTaskSch(pAOC->nSimID);
#else
	CFW_AoTriggerTaskSch();
#endif
    }
    else
    {
      CSW_TRACE(CFW_AOM_TS_ID, TSTXT("CFW_SetAoProcCode ,state CurrentState: 0x%x; nProcCode: 0x%x\n"), CurrentState,nProcCode);
    }
	
    CSW_PROFILE_FUNCTION_EXIT(CFW_SetAoProcCode);
        
    return ERR_SUCCESS;
}


//---------------------------------------------------

#ifdef CFW_MULTI_SIM
UINT8  CFW_GetCcCount( CFW_SIM_ID nSimID)
{
         UINT32 nSrvId =0x00;
	   nSrvId = CFW_CC_SRV_ID*CFW_SIM_COUNT+ nSimID;
	   return AO_ControlBlock[nSrvId ].ActiveNum;
}
#else
UINT8  CFW_GetCcCount(VOID)
{
         return AO_ControlBlock[CFW_CC_SRV_ID ].ActiveNum;
}
#endif
//========================================================
//Return AO count existing in this module.
//
//========================================================
#ifdef CFW_MULTI_SIM
UINT8 SRVAPI CFW_GetAoCount( UINT32 nServiceId,CFW_SIM_ID nSimID)
#else
UINT8 SRVAPI CFW_GetAoCount(VOID)
#endif
{
	UINT8  RetNum  = 0;
	UINT32 nSrvId   = 0;
	UINT8 nActiveAONumber = 0; 
	UINT8 nReadyAONumber = 0;
	UINT8 nSpecialAONumber = 0;

#ifdef CFW_MULTI_SIM
	UINT8   SimIndex = 0;
       CSW_TRACE(CFW_AOM_TS_ID, "CFW_GetAoCount(),  nServiceId = 0x%x, nSimID=0x%x \n", nServiceId,nSimID);

#if 1
     if(nSimID ==CFW_SIM_COUNT)
	{
             for(SimIndex =0; SimIndex < CFW_SIM_COUNT;SimIndex++)
		{	
			nSrvId = nServiceId *CFW_SIM_COUNT+ SimIndex;
		
			nActiveAONumber += AO_ControlBlock[nSrvId ].ActiveNum;
			nReadyAONumber += AO_ControlBlock[nSrvId ].ReadyNum;

			if( (nServiceId == CFW_CC_SRV_ID)||(nServiceId == CFW_GPRS_SRV_ID))
			{
				if(AO_ControlBlock[nSrvId].SpecialAO)
				{
					nSpecialAONumber += 1;
				}
			}
		}
     	}else if(nSimID <CFW_SIM_COUNT && nSimID >=0)
     	{
		
		nSrvId = nServiceId*CFW_SIM_COUNT+ nSimID;
		nActiveAONumber = AO_ControlBlock[nSrvId].ActiveNum;
		nReadyAONumber = AO_ControlBlock[nSrvId].ReadyNum;

		if( (nServiceId == CFW_CC_SRV_ID)||(nServiceId == CFW_GPRS_SRV_ID))
		{
			if(AO_ControlBlock[nSrvId].SpecialAO)
			{
				nSpecialAONumber = 1;
			}
		}
     	}else{

		CSW_TRACE(CFW_AOM_TS_ID, "CFW_GetAoCount() default error\n");
     	}
#else

	switch(nSimID)
	{
		case CFW_SIM_0:
		case CFW_SIM_1:
			nSrvId = nServiceId*CFW_SIM_COUNT+ nSimID;
			nActiveAONumber = AO_ControlBlock[nSrvId].ActiveNum;
			nReadyAONumber = AO_ControlBlock[nSrvId].ReadyNum;

			if( (nSrvId == CFW_CC_SRV_ID)||(nSrvId == CFW_GPRS_SRV_ID)
			||(nSrvId == CFW_CC_SRV_ID_SIM1)||(nSrvId == CFW_GPRS_SRV_ID_SIM1) )
			{
				if(AO_ControlBlock[nSrvId].SpecialAO)
				{
					nSpecialAONumber = 1;
				}
			}
			break;

		case CFW_SIM_COUNT:			
			for(SimIndex =0; SimIndex < CFW_SIM_COUNT;SimIndex++)
			{	
				nSrvId = nServiceId *CFW_SIM_COUNT+ SimIndex;
			
				nActiveAONumber += AO_ControlBlock[nSrvId ].ActiveNum;
				nReadyAONumber += AO_ControlBlock[nSrvId ].ReadyNum;

				if( (nSrvId == CFW_CC_SRV_ID)||(nSrvId == CFW_GPRS_SRV_ID)
				||(nSrvId == CFW_CC_SRV_ID_SIM1)||(nSrvId == CFW_GPRS_SRV_ID_SIM1) )
				{
					if(AO_ControlBlock[nSrvId].SpecialAO)
					{
						nSpecialAONumber += 1;
					}
				}
			}
			break;
		default:
			CSW_TRACE(CFW_AOM_TS_ID, "CFW_GetAoCount() default error\n");
			break;
	}
	#endif
#else   
	nSrvId= gCurrentServId;
	nActiveAONumber = AO_ControlBlock[nSrvId].ActiveNum;
	nReadyAONumber = AO_ControlBlock[nSrvId].ReadyNum;
    
	if( (nSrvId == CFW_CC_SRV_ID)||(nSrvId == CFW_GPRS_SRV_ID))
	{
		if(AO_ControlBlock[nSrvId].SpecialAO)
		{
			nSpecialAONumber = 1;
		}
	}
#endif

	RetNum = nActiveAONumber + nReadyAONumber + nSpecialAONumber;
	return RetNum;

}

//========================================================
//if hAo is NULL, retrieve the current AO state.
//========================================================
UINT32 SRVAPI CFW_GetAoState(HAO hAO)
{
    
    QUERY_AO_ILLEGAL(((AO_CONTENT *)hAO));

    return (((AO_CONTENT *)hAO)->AO_State);

}

//========================================================
//
//
//========================================================
UINT32 SRVAPI CFW_SetAoState(HAO hAO, UINT32 nState)
{
    
    QUERY_AO_ILLEGAL(((AO_CONTENT *)hAO));
    
    ((AO_CONTENT *)hAO)->AO_State = nState;

    return ERR_SUCCESS;

}


//========================================================
//
//
//========================================================
UINT32 SRVAPI CFW_SetAoNextState(HAO hAO, UINT32 nState)
{
    
    QUERY_AO_ILLEGAL(((AO_CONTENT *)hAO));
    
    ((AO_CONTENT *)hAO)->AO_NextState = nState;

    return ERR_SUCCESS;; 


}


//========================================================
//
//Set current AO state to next state.
//========================================================
UINT32 SRVAPI CFW_ChangeAoState( HAO hAO )
{
    AO_CONTENT  *pAOC = (AO_CONTENT *)hAO;
    UINT32  CurrentState = 0;
    UINT32  nServID = 0;
    AO_CB_DOING *pNewOne = 0;       
 
    AO_CB_DOING *QuHead = 0;
    AO_CB_DOING *QuTail = 0;
    
    QUERY_AO_ILLEGAL(pAOC);
    
    CurrentState = pAOC->AO_State;
    nServID = pAOC->nServiceID;

    QuHead = AO_ControlBlock[nServID].DoingHead;
    QuTail = AO_ControlBlock[nServID].DoingTail;
    
    if( CurrentState != CFW_SM_STATE_READY )
    {
        
        pNewOne = (AO_CB_DOING*)CSW_AOM_MALLOC( sizeof(AO_CB_DOING) );
        if( !pNewOne )
            return ERR_AOM_DOING_TAIL_OVER;
        
        CSW_TRACE(CFW_AOM_TS_ID, TSTXT("Malloc Doing Qu Node %x\n"), (UINT32)pNewOne);

        pNewOne->DoingAO = pAOC;
        pNewOne->NextDoing = 0;

        if( QuTail )
        {
            QuTail->NextDoing = pNewOne;
            AO_ControlBlock[nServID].DoingTail = pNewOne;
        }
        else
        {
            AO_ControlBlock[nServID].DoingTail = pNewOne;
            AO_ControlBlock[nServID].DoingHead = pNewOne;
        }

        //Set AO continuing flag.
	SET_AO_GOONSTATUS(1, nServID) ;

#ifdef CFW_MULTI_SIM
        CFW_AoTriggerTaskSch(pAOC->nSimID);
#else
	CFW_AoTriggerTaskSch();
#endif
    }
        
    return ERR_SUCCESS;


}


//========================================================
//
//if hAO is NULL, retrieve the current AO's user data registered.
//========================================================
PVOID SRVAPI CFW_GetAoUserData(HAO hAO)
{
    PVOID p = 0;
    AO_CONTENT  *pAOC = (AO_CONTENT  *)hAO;
    
    //QUERY_AO_ILLEGAL(pAOC);

    if(pAOC)
        p = (PVOID)(pAOC->UserData);

    return p;


}


//========================================================
//
//
//========================================================
static AO_CONTENT *GetAOByIndex ( UINT8 nBaseIndex, UINT8 nIndex, UINT8 MaxNumber,  AO_CONTENT  *Hao)
{
    AO_CONTENT  *pAOC = Hao;
    
    
    if( nIndex > MaxNumber)
        return (AO_CONTENT *)NULL;
    
    for(; nBaseIndex < nIndex; nBaseIndex++)
    {
        if( !pAOC )
            return (AO_CONTENT *)NULL;
        
        pAOC = pAOC->NextAO;
    }

    return pAOC;
    
}

//=============================================================
//
//=============================================================
UINT32 SRVAPI CFW_SetServiceId (UINT32 nServiceId)
{
    
    gCurrentServId = nServiceId;
    return ERR_SUCCESS;
}



//========================================================
// if nState = CFW_SM_STATE_UNKONW, this function ignore this parameter.
// if nIndex the default value is zero if support single instance application.
//========================================================
#ifdef CFW_MULTI_SIM
HAO SRVAPI CFW_GetAoHandle( UINT8 nIndex, UINT32 nState,UINT32 nServiceId,CFW_SIM_ID nSimID)
#else
HAO SRVAPI CFW_GetAoHandle( UINT8 nIndex, UINT32 nState)
#endif
{
    AO_CONTENT  *pAOC = 0;
    AO_CONTENT  *pAORet = 0;

    UINT8 nActiveAONumber = 0; 
    UINT8 nReadyAONumber = 0;
    UINT8 nSpecialAONumber = 0;
	UINT32 nSrvIdO = 0xFF;
#ifdef CFW_MULTI_SIM
    UINT32 nSrvId = (nServiceId*CFW_SIM_COUNT+ nSimID);
    nSrvIdO = nServiceId;
#else
    UINT32 nSrvId = gCurrentServId;
nSrvIdO = gCurrentServId;
#endif

    nActiveAONumber = AO_ControlBlock[nSrvId].ActiveNum;
    nReadyAONumber = AO_ControlBlock[nSrvId].ReadyNum;
    CSW_TRACE(CFW_AOM_TS_ID, "CFW_GetAoHandle(),  nSrvId=0x%x \n", nSrvId);
    switch(nSrvIdO)
    {
        case CFW_CC_SRV_ID:
            if(AO_ControlBlock[nSrvId].SpecialAO)
                nSpecialAONumber = 1;
            
            pAOC = AO_ControlBlock[nSrvId].AcitveAO;                    

            pAORet = GetAOByIndex( 0, nIndex, nActiveAONumber, pAOC);

            if( (pAORet == NULL) && (nActiveAONumber == nIndex) )
            {
                pAORet = AO_ControlBlock[nSrvId].SpecialAO;
            }

            if(pAORet == NULL)
            {
                pAOC = AO_ControlBlock[nSrvId].ReadyAO;

                pAORet = GetAOByIndex((UINT8)(nActiveAONumber+nSpecialAONumber), 
                                      nIndex, 
                                      (UINT8)(nActiveAONumber+nSpecialAONumber+nReadyAONumber),
                                      pAOC);

            }

            if( pAORet == NULL)
                return (HAO)NULL;
            
            if( nState == CFW_SM_STATE_UNKONW )
                return (HAO)pAORet;
            else if (pAORet->AO_State == nState)
            {
                return (HAO)pAORet;

            }
            else
            {
                return (HAO)NULL;
            }
            break;

        case CFW_SMS_MO_SRV_ID:            
        case CFW_SMS_MT_SRV_ID:
        case CFW_NW_SRV_ID:
        case CFW_SIM_SRV_ID:
        case CFW_SS_SRV_ID:
        case CFW_EMOD_SRV_ID:
        case CFW_APP_SRV_ID:
	     pAOC = AO_ControlBlock[nSrvId].AcitveAO;                    

            pAORet = GetAOByIndex( 0, nIndex, nActiveAONumber, pAOC);
#ifdef CFW_MULTI_SIM
            if( (pAORet == NULL) && ((nSrvIdO != CFW_APP_SRV_ID)) )//wug add the last condition
#else
            if( (pAORet == NULL) && (nSrvIdO != CFW_APP_SRV_ID) )
#endif
			{
                pAOC = AO_ControlBlock[nSrvId].ReadyAO;

                pAORet = GetAOByIndex((UINT8)(nActiveAONumber), 
                                      nIndex, 
                                      (UINT8)(nActiveAONumber+nReadyAONumber), 
                                      pAOC);

            }

            if( pAORet == NULL)
                return (HAO)NULL;
            
            if( nState == CFW_SM_STATE_UNKONW )
                return (HAO)pAORet;
            else if (pAORet->AO_State == nState)
            {
                return (HAO)pAORet;

            }
            else
            {
                return (HAO)NULL;
            }
            break;
        case CFW_GPRS_SRV_ID:
            if(AO_ControlBlock[nSrvId].SpecialAO)
                nSpecialAONumber = 1;
            
            pAOC = AO_ControlBlock[nSrvId].AcitveAO;                    

            pAORet = GetAOByIndex( 0, nIndex, nActiveAONumber, pAOC);

            if( (pAORet == NULL) && (nActiveAONumber == nIndex) )
            {
                pAORet = AO_ControlBlock[nSrvId].SpecialAO;
            }

            if(pAORet == NULL)
            {
                pAOC = AO_ControlBlock[nSrvId].ReadyAO;

                pAORet = GetAOByIndex((UINT8)(nActiveAONumber+nSpecialAONumber), 
                                      nIndex, 
                                      (UINT8)(nActiveAONumber+nSpecialAONumber+nReadyAONumber),
                                      pAOC);

            }

            if( pAORet == NULL)
                return (HAO)NULL;
            
            if( nState == CFW_SM_STATE_UNKONW )
                return (HAO)pAORet;
            else if (pAORet->AO_State == nState)
            {
                return (HAO)pAORet;

            }
            else
            {
                return (HAO)NULL;
            }

            break;
	default:
	 	CSW_TRACE(CFW_AOM_TS_ID, "CFW_GetAoHandle() default error\n");
	 	break;
    }//switch
    return (HAO)NULL;
        
}


//====================================================================
//
//
//AO <--> AO communicaton, normally used to notify other AO to change state or process event.
//====================================================================
UINT32 SRVAPI CFW_PostAoEvent ( HAO hAo,
                         UINT32 nEventId, // event
                         UINT32 nParam1, // event parameter
                         UINT32 nParam2 // AO source you can access when event process.
                         )
{

    return ERR_SUCCESS;

}



//=========================================================================================
//
//=========================================================================================
UINT32 SRVAPI CFW_RegisterCreateAoProc(UINT32 nEventId, PFN_EV_IND_TRIGGER_PORC pfnProc)
{
    CSW_TRACE(CFW_AOM_TS_ID, "CFW_RegisterCreateAoProc,  nEventId = 0x%x\n", nEventId);
    if( nEventId == API_CC_SETUP_IND )
    {
        Event_Ind_Fun[EVENT_IND_CM] = pfnProc;
    }
    else if( nEventId == API_SMSPP_RECEIVE_IND )
    {
        Event_Ind_Fun[EVENT_IND_SMS_MT] = pfnProc;
    }
    else if( nEventId == API_PDP_CTXACTIVATE_IND )
    {
        Event_Ind_Fun[EVENT_IND_GPRS] = pfnProc;
    }
    else if( nEventId == API_SIM_STATUSERROR_IND )
    {
        Event_Ind_Fun[EVENT_IND_SIM] = pfnProc;
    }
	else if( nEventId == API_SS_ACTION_IND)
    {
        Event_Ind_Fun[EVENT_IND_USSD] = pfnProc;
    }
    else if( nEventId == EV_TIMER )
    {
        Event_Ind_Fun[EVENT_IND_TIMER] = pfnProc;
    }
    else
    {
        return ERR_CFW_NOT_EXIST_AO;
    }

    return ERR_SUCCESS;

}



//=========================================================================================
//mode: 0 表示APP 的 UTI, 1 表示是CFW的UTI, 2: 表示是Stack的UTI
//
//(1)	MMI / ATL纵向调用： 0x00 ~ 0xFF
//(2)	CFW内部横向调用：   0x1000 ~ 0xCFFF
//(3)	动态加载的APP调用： 0xE000 ~ 0xFFFF
//=========================================================================================
UINT32 SRVAPI CFW_SetUTI(HAO hAO, UINT16 nUTI, UINT8 mode)
{
    AO_CONTENT  *pAOC = (AO_CONTENT *)hAO;

    if( mode == 2 )
    {
        (pAOC->UTI) = ((pAOC->UTI) & 0xFFFFFF00) | nUTI;
    }
    else
        pAOC->UTI = ((pAOC->UTI) & 0xFF0000FF) | (nUTI<<8);

    
    return ERR_SUCCESS;
} 



//=======================================================================================
//Get entitle UTI(necessary for CFW_PostNotifyEvent).
//
//=======================================================================================
UINT32 SRVAPI CFW_GetUTI(HAO hAO, UINT32* pUTI)
{

    AO_CONTENT  *pAOC = (AO_CONTENT *)hAO;    

    QUERY_AO_ILLEGAL(pAOC);

    *pUTI = pAOC->UTI;

    return ERR_SUCCESS;

}
UINT32  CFW_GetUsingUTI(HAO hAO, UINT8* pUTI)
{

    AO_CONTENT  *pAOC = (AO_CONTENT *)hAO;    

    QUERY_AO_ILLEGAL(pAOC);

    *pUTI =LOUINT8((pAOC->UTI>>8)&0x000000FF);

    return ERR_SUCCESS;

}
#ifdef CFW_MULTI_SIM
UINT32 SRVAPI CFW_GetSimID(HAO hAO, CFW_SIM_ID *pSimID)
{

    AO_CONTENT  *pAOC = (AO_CONTENT *)hAO;    

    QUERY_AO_ILLEGAL(pAOC);

    *pSimID = pAOC->nSimID;

    return ERR_SUCCESS;

}
#endif


//==================================================================================
//
//
//==================================================================================
static UINT32 GetFreeUTI(UINT32 *Uti_Status, UINT8 uti_min, UINT8 uti_max, UINT8 *pUTI)
{
    UINT32  TmpStatus = 0;
    UINT8   i = 0;

    TmpStatus = *Uti_Status;
    
    while( (TmpStatus & 0x01) && ((i+uti_min) <= uti_max) )
    {
        TmpStatus = TmpStatus >> 1 ;
        i++;
    }
    
    if( (i+uti_min) > uti_max )
    {
        return ERR_AOM_UTI_EMPTY;
    }
    else
    {
        UINT32 nOne = 0x01;
        *pUTI = i + uti_min;
        
        nOne = nOne << i;
        
        *Uti_Status = *Uti_Status | nOne;
        
    }

    return ERR_SUCCESS;

}


//#if DEFINE_ALLOC_UTI
//=======================================================================
//QueryUsingUTI
//If return 1 means that this UTI is busy;
//Otherwise, 0 means that this UTI is free.
//=======================================================================
UINT32 QueryUsingUTI (UINT32 nServiceId,  UINT8 UTI )
{
    UINT8   m = 0;
    UINT8   n = 0;
    UINT32  nOne = 0x01;
    AOM_Down();   
    m = UTI / 32;
    n = UTI % 32;

    nOne = nOne << n;

    if (UTI_Status[m] & nOne) 
    {
    	AOM_Up();
        return 1;
    }
    else
    {
    	AOM_Up();
        return 0;
    }
    
}


//=======================================================================
//AllocUsingUTI
//=======================================================================
UINT32 AllocUsingUTI (  UINT32 nServiceId,  UINT8 UTI )
{
    UINT32  ret = ERR_SUCCESS;
    
    UINT8   m = 0;
    UINT8   n = 0;
    UINT32  nOne = 0x01;
   AOM_Down();   
    m = UTI / 32;
    n = UTI % 32;

    nOne = nOne << n;

    UTI_Status[m] = UTI_Status[m] | ( nOne );
   AOM_Up();   
    return ret;
}


//===================================================================================
//ReleaseUsingUTI
//===================================================================================
static UINT32 ReleaseUsingUTI (  UINT32 nServiceId,  UINT8 UTI )
{
    UINT32  ret = ERR_SUCCESS;
    
    UINT8   m = 0;
    UINT8   n = 0;
    UINT32  nOne = 0x01;
    CSW_TRACE(CFW_AOM_TS_ID, "FREE UTI = %d\n", UTI);
    AOM_Down();
    m = UTI / 32;
    n = UTI % 32;

    nOne = nOne << n;

    UTI_Status[m] = (UTI_Status[m] & (nOne ^ 0XFFFFFFFF));
    AOM_Up();
    return ret;
}

//#endif

UINT32 CFW_RelaseUsingUTI( UINT32 nServiceId, UINT8 UTI )
{
  return ReleaseUsingUTI(nServiceId, UTI);
}

//===============================================================================
//if return value == ERR_SUCCESS, then it have been allocated one free uti. 
//===============================================================================
UINT32 CFW_GetFreeUTI ( UINT32 nServiceId, UINT8* pUTI )
{
	UINT32  ret = ERR_AOM_UTI_EMPTY;  
	UINT8  nScale = 32;
	UINT8  i = 0;
	UINT32 *pUTI_Status = &UTI_Status[1];
       AOM_Down();
	while(ret && i< 7 ) 
	{
		ret = GetFreeUTI(pUTI_Status, nScale, (UINT8)(nScale+31), pUTI);	
		i++;    
		nScale += 32;
		pUTI_Status++;
	}
       AOM_Up();
	if( ERR_AOM_UTI_EMPTY == ret )
	{
		AO_CONTENT *pAO = NULL;
		UINT32     n = SIZEOF(UTI_Status);
    		SUL_ZeroMemory32(UTI_Status, n);
		CSW_TRACE(0, "ERR_AOM_UTI_EMPTY == ret\n");
		
		for(i = 0; i < CFW_MAX_SRV_ID; i++ )
		{
			pAO = AO_ControlBlock[i].SpecialAO;
			if(pAO != NULL)
			{
				AllocUsingUTI(0,LOUINT8((pAO->UTI>>8)&0x000000FF));
			}
			pAO =AO_ControlBlock[i].AcitveAO;
			while(pAO != NULL)
			{
				AllocUsingUTI(0,LOUINT8((pAO->UTI>>8)&0x000000FF));
				pAO = pAO->NextAO;
			}
			pAO = AO_ControlBlock[i].ReadyAO;
			while(pAO != NULL)
			{
				AllocUsingUTI(0,LOUINT8((pAO->UTI>>8)&0x000000FF));
				pAO = pAO->NextAO;
			}
		}
		
    i            = 0;
    nScale       = 32;
    pUTI_Status = &UTI_Status[1];
    AOM_Down();
    while (ret && i < 7)
    {
      ret = GetFreeUTI(pUTI_Status, nScale, (UINT8)(nScale + 31), pUTI);
      i++;
      nScale += 32;
      pUTI_Status++;
    }
    AOM_Up();

}


CSW_TRACE(CFW_AOM_TS_ID, "GET UTI = %d\n", *pUTI);
 
	return ret;
}

//=======================================================================
//
//
//=======================================================================
UINT32 CFW_IsFreeUTI ( UINT16 nUTI, UINT32 nServiceId )
{
    HAO     hAO;
    UINT8   i = 0;
    UINT32  AOCount = 0;
    UINT32  nAO_UTI = 0;
    AO_CONTENT  *pAOC;
    CSW_TRACE(CFW_AOM_TS_ID, "CFW_RegisterCreateAoProc,  nUTI = 0x%x, nServiceId = 0x%x\n", nUTI,nServiceId);
    if( (nUTI >= NW_UTI_MIN) && (nUTI <= CSD_UTI_MAX) )
        return ERR_SUCCESS;

    if( (nUTI == APP_UTI_SHELL) || (nUTI == APP_UTI_URC) )
        return ERR_SUCCESS;
#ifdef CFW_MULTI_SIM
 	UINT8 sim_index = 0;

	for(sim_index = 0;sim_index < CFW_SIM_COUNT;sim_index++)
	{	
		AOCount = CFW_GetAoCount(nServiceId,(CFW_SIM_ID)sim_index);

		while( i < AOCount )
		{ 
			hAO = CFW_GetAoHandle(i, CFW_SM_STATE_UNKONW,nServiceId,(CFW_SIM_ID)sim_index);

			pAOC = (AO_CONTENT *)hAO;

			if(pAOC)
			{    
				if(pAOC->nServiceID > CFW_MAX_SRV_ID)
				{
					i++;
					continue;
				}

			}

			if(hAO)
			{
				CFW_GetUTI(hAO, &nAO_UTI);

				nAO_UTI = nAO_UTI >> 8;

				if( nUTI == (UINT16)nAO_UTI )
				return ERR_CFW_UTI_IS_BUSY;

			}
			i++;
		}
	}
#else
    CFW_SetServiceId(nServiceId);
    AOCount = CFW_GetAoCount();

    while( i < AOCount )
    { 
        hAO = CFW_GetAoHandle(i, CFW_SM_STATE_UNKONW);
        
        pAOC = (AO_CONTENT *)hAO;

        if(pAOC)
        {    
            if(pAOC->nServiceID > CFW_MAX_SRV_ID)
            {
                i++;
                continue;
            }

        }
        
        if(hAO)
        {
            CFW_GetUTI(hAO, &nAO_UTI);
           
            nAO_UTI = nAO_UTI >> 8;

            if( nUTI == (UINT16)nAO_UTI )
                return ERR_CFW_UTI_IS_BUSY;
                    
        }
        i++;
    }
#endif

    return ERR_SUCCESS;
}


//=======================================================================
//
//Check UTI is busy(occupied by AO)
//=======================================================================
UINT32 SRVAPI CFW_UTIIsBusy(HAO hAo, UINT16 nUTI)
{

    return ERR_SUCCESS;
}



//=================================================================
//
//
//=================================================================
UINT32 SRVAPI CFW_SetAoEventNotifyProc (
                                HAO hAO,
                                PVOID pUserData,
                                PFN_CFW_EV_PORC pfnEvProc)
{
    AO_CONTENT  *pAOC = (AO_CONTENT *)hAO;
    
    QUERY_AO_ILLEGAL(pAOC);
    
    pAOC->pfnEvProc = pfnEvProc;
    pAOC->pEventUserData = pUserData;  

    return ERR_SUCCESS;

}

 

//=================================================================
//
//=================================================================
#ifdef CFW_MULTI_SIM
UINT32 SRVAPI CFW_AoTriggerTaskSch(CFW_SIM_ID nSimID)
#else
UINT32 SRVAPI CFW_AoTriggerTaskSch(VOID)
#endif
{
    #ifdef MF32_CFW_MMI_TWO_TASKS
#ifdef CFW_MULTI_SIM
	CFW_BalSendMessage(11, HVY_API_RESERVED, NULL, 0, nSimID);
#else
	CFW_BalSendMessage(11, HVY_API_RESERVED, NULL, 0 );
#endif
    
	
    #endif
    
    return ERR_SUCCESS;
}


//=================================================================
//
//=================================================================
UINT32 SRVAPI CFW_RegSysDefaultProc(PFN_CFW_DEFAULT_PROC pfnProc)
{
    
    CFW_Sys_Default_Proc = pfnProc;
    
    return ERR_SUCCESS;
}

//=================================================================
//
//=================================================================
UINT32 SRVAPI CFW_AoInit(VOID)
{
    UINT32 n = SIZEOF(AO_ControlBlock);
    SUL_ZeroMemory32(AO_ControlBlock, n);

    n = SIZEOF(Event_Ind_Fun);
    SUL_ZeroMemory32(Event_Ind_Fun, n);
    
    n = SIZEOF(UTI_Status);
    SUL_ZeroMemory32(UTI_Status, n);

     SET_AO_GOONSTATUS(0, 0xff) ;
	//modify by wys 2007-05-23
    //AOM_init();
    if (!g_aom_sem) 
    { 
        g_aom_sem = COS_CreateSemaphore(1);
    }

    return ERR_SUCCESS;
}


//=================================================================
//
//=================================================================
UINT32 SRVAPI CFW_AoExit(VOID)
{
    return ERR_SUCCESS;
}

//=================================================================
//If match a notify call function, so it returns ERR_SUCCESS. Or not
//the return value will be -1.
//=================================================================
UINT32 CallNotifyFun( CFW_EV *pEvent )
{
    UINT8 nNotifyID = WhickKindOfNotifyEvent( pEvent->nTransId );
    AO_CONTENT  *pAOC = 0;
    UINT8  i = 0;
    UINT8  nActiveAONumber = 0;
    UINT32 nProcCode = 0;
UINT8 nNotifyIDX = 0xFF;
    CFW_EVENT   ExterEv;
	
#ifdef CFW_MULTI_SIM
	nNotifyIDX =nNotifyID*CFW_SIM_COUNT + pEvent->nFlag;
#endif	
	ExterEv.nEventId = pEvent->nEventId;
	ExterEv.nParam1  = pEvent->nParam1;
	ExterEv.nParam2  = pEvent->nParam2;
	ExterEv.nType    = pEvent->nType;
	ExterEv.nUTI     = (UINT16)pEvent->nTransId;
	ExterEv.nFlag    = pEvent->nFlag;
    if((nNotifyID == CFW_SMS_MO_SRV_ID)||
       (nNotifyID == CFW_SMS_MT_SRV_ID)||
       (nNotifyID == CFW_NW_SRV_ID)||
       (nNotifyID == CFW_CC_SRV_ID)||
       (nNotifyID == CFW_SS_SRV_ID)||
       (nNotifyID == CFW_EMOD_SRV_ID)||
       (nNotifyID == CFW_APP_SRV_ID)||
       (nNotifyID == CFW_GPRS_SRV_ID)
       )
	{
		nActiveAONumber = AO_ControlBlock[nNotifyIDX].ActiveNum;
		if ( nActiveAONumber > 0 )
		{
			pAOC = AO_ControlBlock[nNotifyIDX].AcitveAO;

			do 
			{
				if(pAOC->pfnEvProc)
				{
					pEvent->nTransId = (pEvent->nTransId & 0x00FFFF00) >> 8;
					nProcCode = pAOC->pfnEvProc(&ExterEv, pAOC->pEventUserData);    	

					if( ( (nProcCode) & CFW_AO_PROC_CODE_CONSUMED) == CFW_AO_PROC_CODE_CONSUMED )
					{
					    pAOC->nProcCode =( pAOC->nProcCode & (CFW_AO_PROC_CODE_CONSUMED ^ 0xFFFFFFFF) );
					    
					    return ERR_SUCCESS;
					}
				}

				pAOC = pAOC->NextAO;
				i++; 
			    
			} while(pAOC && ( i < nActiveAONumber ));
		}
	}
    
    return -1;

}

//=================================================================
//
//
//=================================================================
#if 0
static void TriggerStateMachine( VOID )
{
    UINT32   nSevID = 0;

    for( nSevID = 0; nSevID <= CFW_MAX_SRV_ID; nSevID++)
    {
        AO_CB_DOING *QuHead = AO_ControlBlock[nSevID].DoingHead;
        
        while( QuHead )
        {
            if( !(QuHead->DoingAO) )
                break;
            
            if( !((QuHead->DoingAO)->pAO_Proc) )
                break;

            //Implementing continue...
            (QuHead->DoingAO)->pAO_Proc( (HAO)(QuHead->DoingAO), SPECIAL_MESSAGE );
            
            //Get new one 
            AO_ControlBlock[nSevID].DoingHead = QuHead->NextDoing;
            CSW_AOM_FREE(QuHead);
            
            CSW_TRACE(CFW_AOM_TS_ID, TSTXT("FREE Doing Qu Node %x\n"), (UINT32)QuHead);

            QuHead = AO_ControlBlock[nSevID].DoingHead;
            
            if(!QuHead)
                AO_ControlBlock[nSevID].DoingTail = QuHead;

        }//while()

        //Clear the AO continuing flag.
        AO_GoOnStatus = AO_GoOnStatus & ( (0x01 << nSevID) ^ 0xFFFFFFFF ) ;

    }//for()

}

#else

static void TriggerStateMachine( VOID )
{
    UINT32   nSevID = 0;

    CSW_PROFILE_FUNCTION_ENTER(TriggerStateMachine);
    //Modify by lixp for Memory Issue
    //for( nSevID = 0; nSevID <= CFW_MAX_SRV_ID; nSevID++)
    for( nSevID = 0; nSevID < CFW_MAX_SRV_ID; nSevID++)
    {
        AO_CB_DOING *QuHead = AO_ControlBlock[nSevID].DoingHead;
        
        if( QuHead )
        {
            CSW_TRACE(CFW_AOM_TS_ID, TSTXT("TriggerStateMachine Begin nSevID %x\n"), (UINT32)nSevID);
            if( !(QuHead->DoingAO) )
                continue;
            
            if( !((QuHead->DoingAO)->pAO_Proc) )
                continue;

            //Implementing continue...
            (QuHead->DoingAO)->pAO_Proc( (HAO)(QuHead->DoingAO), SPECIAL_MESSAGE );
            
            //Get new one 
            AO_ControlBlock[nSevID].DoingHead = QuHead->NextDoing;
            CSW_AOM_FREE(QuHead);
            
            CSW_TRACE(CFW_AOM_TS_ID, TSTXT("TriggerStateMachine Used nSevID %x\n"), (UINT32)nSevID);

            QuHead = AO_ControlBlock[nSevID].DoingHead;
            
            if(!QuHead)
                AO_ControlBlock[nSevID].DoingTail = QuHead;

        }//if()

        //Clear the AO continuing flag.
        if(!QuHead)
            SET_AO_GOONSTATUS(0, nSevID) ;
        else
            SET_AO_GOONSTATUS(1, nSevID) ;

    }//for()

    CSW_PROFILE_FUNCTION_EXIT(TriggerStateMachine);
}

#endif


//=============================================================================
//
//=============================================================================
static void CFW_AOM_EVENT_FREE(CFW_EV Event)
{
    if( 1 ) // Now, all internal events will be freed...
    {

        CSW_AOM_FREE(Event.nParam1);
        CSW_TRACE(CFW_AOM_TS_ID, TSTXT("FREE the Param1 of EVENT  %x\n"), (UINT32)Event.nParam1);
    }
}
#ifdef __221_UNIT_TEST__
extern CFW_SIM_ID gSimID;
#endif
//=============================================================================
//
//=============================================================================
UINT32 SRVAPI CFW_AoScheduler(PVOID pMsg)
{
    UINT32 ret      = ERR_SUCCESS;
    UINT8 bHasEvent = 0;
    UINT32   retBAL_Run = 0;    
    CFW_EV   Event;
    
    UINT32 *p   = (UINT32*)pMsg;

    UINT32 EventId = *p++;   
    
    CSW_PROFILE_FUNCTION_ENTER(CFW_AoScheduler);
    Event.nEventId = EventId;
    
    //CSW_TRACE(CFW_AOM_TS_ID, TSTXT("CFW_AoScheduler:0x%x \n"), EventId);

    /* If there is something to wait for continue */
    
	if( IS_SCL_EVENT(EventId) || EventId == EV_TIMER)
	{
#ifdef CFW_MULTI_SIM
		if( EV_TIMER != EventId)
		{
#ifndef __221_UNIT_TEST__
			Event.nFlag = (UINT8)*p++;
#else
    //CSW_TRACE(CFW_AOM_TS_ID, TSTXT("CFW_AoScheduler:Event.nFlag=0x%x \n"), Event.nFlag);
			Event.nFlag = gSimID;
#endif
		}
#endif
		Event.nParam1 = (UINT32)p;
		ret = Dispatch( &Event );
	}

    SUL_ZeroMemory32(&Event, SIZEOF(CFW_EV) );

    do
    {
        //
        // The first thing: Trigger all STATE MACHINE after 
        // marking them by function call.
        //
        
        TriggerStateMachine();
        
        //
        // The second thing: Process the notify event.
        //
        while(CFW_GetEventQueue(&Event))
        {
          UINT8 nNotifyID = WhickKindOfNotifyEvent(Event.nTransId);
	 UINT8 nNotifyIDO = nNotifyID;
          AO_CONTENT* pAOC = 0;
          UINT8 i               = 0;
          UINT8 nActiveAONumber = 0;
          CFW_EVENT ExterEv;

    #ifdef CFW_MULTI_SIM
          if (nNotifyID < CFW_MAX_SRV_ID)
          {
            nNotifyID =(nNotifyID * CFW_SIM_COUNT +Event.nFlag);
          }
    #endif
          Event.nTransId = (Event.nTransId & 0x00FFFF00) >> 8;

          ExterEv.nEventId = Event.nEventId;
          ExterEv.nParam1  = Event.nParam1;
          ExterEv.nParam2  = Event.nParam2;
          ExterEv.nType    = Event.nType;
          ExterEv.nUTI     = (UINT16)Event.nTransId;
          ExterEv.nFlag    = Event.nFlag;
          CSW_TRACE(CFW_AOM_TS_ID, "CFW_AoScheduler(),  nNotifyID=0x%x \n", nNotifyID);
          switch (nNotifyIDO)
          {
          case NOTIFY_TO_ATL:
            {
              retBAL_Run = BAL_Run(&ExterEv);
            }
            break;

          case NOTIFY_TO_SYS_DEFAULT:
            retBAL_Run = CFW_Sys_Default_Proc(&Event);

            if (retBAL_Run & BAL_PROC_NOT_CONSUME)
            {
              retBAL_Run = BAL_Run(&ExterEv);
            }

            // Free NotifyEvent 
            if (!(retBAL_Run & BAL_PROC_DONT_FREE_MEM))
            {
              CFW_AOM_EVENT_FREE(Event);
              SUL_ZeroMemory32(&Event, SIZEOF(CFW_EVENT));

            }
            break;

          case CFW_SMS_MO_SRV_ID:
          case CFW_SMS_MT_SRV_ID:
          case CFW_EMOD_SRV_ID:
            nActiveAONumber = AO_ControlBlock[nNotifyID].ActiveNum;
            if (nActiveAONumber > 0)
            {
              pAOC = AO_ControlBlock[nNotifyID].AcitveAO;
              if (pAOC)
              {
                if (pAOC->pAO_Proc != NULL)
                  pAOC->pAO_Proc((HAO) pAOC, &Event);

                CFW_AOM_EVENT_FREE(Event);

                QUERY_AO_ILLEGAL_BREAK(pAOC);

              }
            }

            break;

          case CFW_NW_SRV_ID:
          case CFW_SS_SRV_ID:
          case CFW_CC_SRV_ID:
          case CFW_APP_SRV_ID:
          case CFW_GPRS_SRV_ID:
            i               = 0;
            nActiveAONumber = AO_ControlBlock[nNotifyID].ActiveNum;
            if (nActiveAONumber > 0)
            {
              pAOC = AO_ControlBlock[nNotifyID].AcitveAO;

              do
              {
                if (pAOC->pAO_Proc)
                {
                  pAOC->pAO_Proc((HAO) pAOC, &Event);

                  // Check if this AO has been unregistered.
                  // QUERY_AO_ILLEGAL_BREAK(pAOC);
                  if (pAOC)
                  {
                    if (pAOC->nServiceID > CFW_MAX_SRV_ID)
                    {
                      CFW_AOM_EVENT_FREE(Event);

                      break;
                    }

                  }
                  else
                    break;

                  // This notify event has been consumed.
                  if (((pAOC->nProcCode) & CFW_AO_PROC_CODE_CONSUMED) == CFW_AO_PROC_CODE_CONSUMED)
                  {
                    pAOC->nProcCode = (pAOC->nProcCode & (CFW_AO_PROC_CODE_CONSUMED ^ 0xFFFFFFFF));

                    CFW_AOM_EVENT_FREE(Event);

                    break;
                  }

                }

                pAOC = pAOC->NextAO;
                i++;

              } while (pAOC && (i < nActiveAONumber));

            }
            break;

          default:
            CSW_TRACE(CFW_AOM_TS_ID, "CFW_AoScheduler() default error\n");
            CFW_AOM_EVENT_FREE(Event);
            break;
          }
        } // while(CFW_GetEventQueue(&Event)
        
        //The third thing: Query if there is any external event.
        bHasEvent = FALSE; //IsCFWEventAvail();
        
        //The last thing: Have it run again.
        if( (retBAL_Run & BAL_PROC_CONTINUE) && !bHasEvent )
            retBAL_Run = BAL_Run( (CFW_EVENT*)SPECIAL_MESSAGE );
        
    	}while( ((retBAL_Run&BAL_PROC_CONTINUE) && !bHasEvent ) || (GET_AO_GOONSTATUS) ); 
    CSW_PROFILE_FUNCTION_EXIT(CFW_AoScheduler);
    return ret;
}

//if bindex is 0xff,is means that we will set all bit to bset value.
UINT32 CFW_bitSet(UINT8 bset, UINT8 bindex,UINT32 *bdes,UINT8 bcount)
{
	UINT8   i  = 0;
	UINT8   n = 0;
	UINT32 *pdata = bdes;

	if (0xff == bindex)
	{
		if(0 == bset)
		{
			for(i = 0 ; i < bcount; i++)
			{
				pdata[i] = pdata[i]&0x00000000;
			}
		}
		else
		{
			for(i = 0 ; i < bcount; i++)
			{
				pdata[i] = pdata[i]|0xffffffff;
			}
		}
		return 1;
	}
#if 0
	if(bindex > bcount*32)
	{
		//
	}
#endif
	i = bindex/32;
	n = bindex - i * 32;
	if(1==bset)
	{
		pdata[i]=pdata[i]|(bset<<(n-1));
	}
	else
	{
		pdata[i]=pdata[i]&(bset<<(n-1));
	}
	
	return 1;
}
//if bindex is 0xff,we will check all bit and return 1 .
UINT8 CFW_bitGet(UINT8 bindex,UINT32 *bdes,UINT8 bcount)
{
	UINT8   i  = 0;
	UINT8   n = 0;
	UINT32 *pdata = bdes;


	if (0xff == bindex)
	{  
		for( i = 0; i < bcount ; i++)
		{
			if(pdata[i] > 0)
			{
				return 1;
			}
		}
		return 0;
	}
	i = bindex/32;
	n = bindex - i * 32;
	 
	return pdata[i]&(0x01<<(n-1));
}


