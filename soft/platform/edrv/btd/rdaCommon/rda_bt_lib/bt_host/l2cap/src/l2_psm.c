/**************************************************************************
 * MODULE NAME:    l2_psm.c
 * PROJECT CODE:
 * DESCRIPTION:    l2cap psm handler.
 * AUTHOR:         Gary Fleming
 * DATE:           02-07-2000
 *
 *
 * LICENSE
 *     This source code is copyright (c) 2000-2002 Parthus Technologies Plc.
 *     All rights reserved.
 *
 * DESCRIPTION
 * This module is responsible for the support of PSMs and the (de)registration
 * of the PSM specific event handlers.
 *************************************************************************/

#include "host_config.h"
#include "host_types.h"
#include "host_error.h"
#include "host_buf.h"

#include "l2_types.h"
#include "l2_const.h"
#include "l2cap_primitives.h"
#include "papi.h"

#include "l2_psm.h"
#include "l2_channel.h"
#include "l2_config_helper.h"

#if COMBINED_HOST==1
#include <stdarg.h>
#include <assert.h>
#include <string.h>
#include "bt_codec.h"
#endif
/***************************************************************** 
 * Two local functions used to enable/disable all connectionless traffic
 *****************************************************************/ 
#if (L2CAP_CLS_TRAFFIC_SUPPORTED==1)
static void prh_l2_psm_enable_all(void);
static void prh_l2_psm_disable_all(void);
#endif

u_int8 custom_copy(u_int8* dest, u_int8 s1_len, u_int8* s1, u_int8 s2_len,...);


struct l2cap_psm prh_l2_psm_cb_list[PRH_L2_MAX_NUM_PSM]; /* Array of PSM callbacks                   */
u_int16 prh_l2_psm_id_array[PRH_L2_MAX_NUM_PSM];  /* Friendly mapping of PSM Values to PSM Ids */
void psm_custom_debug_print(u_int8* temp,u_int8 event, u_int16 psmNumber,t_L2_ControlCallbackArgs*  p_eventinfo, u_int8 index);
/*******************************************************************
 *
 * Function :- L2_RegisterPSM
 *
 * Parameters
 *      u_int16 psm                 The identifier for the higher layer protocol.
 *      l2_callbacks *callbacks     A pointer to a structure containing the callback
 *                                  functions to be used for delivery of events to the
 *                                  higher layers.
 * Returns
 *      t_api                       outcome of the function.
 *                                      BT_NOERROR
 *                                      BT_INVALIDPARAM
 * Description 
 * Registers the callbacks for events occuring on a specific PSM
 *
 *******************************************************************/

APIDECL1 t_api APIDECL2  L2_RegisterPSM(u_int16 psm,  l2_callbacks *callbacks) 
{
#if ((pDEBUG==0) && (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED))
u_int8 temp[2];
#endif
    u_int16 psm_ident;
    t_api status = BT_INVALIDPARAM;
    struct l2cap_cbs* p_psm_cbs;
#if pDEBUG
    pDebugPrintfEX((pLOGNOTICE,pLOGL2CAP,"L2CAP: L2_RegisterPSM called, psm=0x%02X\n",psm));
#elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
    BT_WRITE_LE_UINT16(temp,psm);
    SYSDebug_PrintEx(pLOGDEBUG,pLOGL2CAP, (LOG_L2CAP_L2_PSM_INDEX),temp,2,BT_CODE_WORD);
#endif

    if (psm & 0x0001) /* Check is the PSM value odd :- BT Spec requirement */
    {

        psm_ident= prh_l2_psm_get_psm_ident(psm);
        if (psm_ident==0) 
            psm_ident = prh_l2_psm_store_psm(psm);
#if pDEBUG
        else
            pDebugPrintfEX((pLOGDEBUG,pLOGL2CAP,"PSM Already exists -- Re-Registering \n"));
#elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
        else
             SYSDebug_PrintEx(pLOGDEBUG,pLOGL2CAP, (LOG_L2CAP_L2_PSM_INDEX+1),NULL,0,0);
#endif

        if(psm_ident)
        {   
            prh_l2_psm_cb_list[psm_ident].psm=psm;
            p_psm_cbs = &prh_l2_psm_cb_list[psm_ident].psm_cbs;
        
            p_psm_cbs->connectionIndication=callbacks->connectionIndication;
            p_psm_cbs->configureIndication=callbacks->configureIndication;
            p_psm_cbs->disconnectionIndication=callbacks->disconnectionIndication;
            p_psm_cbs->qosviolationIndication=callbacks->qosviolationIndication;
            p_psm_cbs->connectionConfirmation=callbacks->connectionConfirmation;
            p_psm_cbs->configureConfirmation=callbacks->configureConfirmation;
            p_psm_cbs->disconnectionConfirmation=callbacks->disconnectionConfirmation;
            p_psm_cbs->dataIndication=callbacks->dataIndication;
            p_psm_cbs->timeoutIndication=callbacks->timeoutIndication;
			p_psm_cbs->flushOccurred =callbacks->flushOccurred;
            p_psm_cbs->bufferOverflow =callbacks->bufferOverflow;

#if PRH_L2CAP_AUTO_CONFIG==1
	        p_psm_cbs->connectionConfiguredConfirmation=callbacks->connectionConfiguredConfirmation;
#endif
            /* Disable Connectionless Traffic */
            prh_l2_psm_cb_list[psm_ident].state = PRH_L2_CLT_DISABLED;

#if PRH_L2CAP_AUTO_CONFIG==1
	        if ((p_psm_cbs->configureConfirmation==NULL) && (p_psm_cbs->configureIndication==NULL) && (p_psm_cbs->connectionConfiguredConfirmation))
			{
		        p_psm_cbs->connectionConfirmation=prh_l2_configure_helper_conn_confirm;
		        p_psm_cbs->configureConfirmation=prh_l2_configure_helper_confirmation;
		        p_psm_cbs->configureIndication=prh_l2_configure_helper_indication;
			}
#endif
            status = BT_NOERROR;
        }
    }
    return status;
}

/*******************************************************************
 *
 * Function :- L2_DeRegisterPSM
 *
 * Parameters
 *      u_int16 psm     The identifier for the higher layer protocol.
 *
 * Returns
 *      t_api           outcome of the function.
 *                          BT_NOERROR
 *                          BT_INVALIDPARAM
 * Description 
 * De-registers the callbacks for a specific PSM.
 *
 *******************************************************************/

APIDECL1 t_api APIDECL2  L2_DeregisterPSM(u_int16 psm) 
{
#if ((pDEBUG==0) && (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED))
u_int8 temp[2];
#endif
    u_int16  psm_ident;
    struct l2cap_cbs* p_psm_cbs;
    t_api status = BT_INVALIDPARAM;
#if pDEBUG
    pDebugPrintfEX((pLOGNOTICE,pLOGL2CAP,"L2CAP: L2_DeregisterPSM called, psm=0x%02X\n",psm));
#elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
    BT_WRITE_LE_UINT16(temp,psm);
    SYSDebug_PrintEx(pLOGDEBUG,pLOGL2CAP, (LOG_L2CAP_L2_PSM_INDEX+2),temp,2,BT_CODE_WORD);

#endif

    psm_ident= prh_l2_psm_get_psm_ident(psm);
    if (psm_ident) 
    {
        p_psm_cbs = &prh_l2_psm_cb_list[psm_ident].psm_cbs;
    
        p_psm_cbs->connectionIndication=NULL;
        p_psm_cbs->configureIndication=NULL;
        p_psm_cbs->disconnectionIndication=NULL;
        p_psm_cbs->qosviolationIndication=NULL;
        p_psm_cbs->connectionConfirmation=NULL;
        p_psm_cbs->configureConfirmation=NULL;
        p_psm_cbs->disconnectionConfirmation=NULL;
        p_psm_cbs->dataIndication=NULL;

        prh_l2_chn_free_channels_on_psm(psm);
        prh_l2_psm_free_psm(psm);


        status = BT_NOERROR;
    }
    return status;
}

#if (L2CAP_CLS_TRAFFIC_SUPPORTED==1)
/********************************************************************
 *
 * Function :- L2_DisableConnectionlessTraffic
 *
 * Parameters :-
 *      u_int16 psm     The PSM for which connectionless traffic is to be 
 *                      disabled. A PSM of 0x0000 will disable connectionless
 *                      traffic for all PSMs and groups.
 * Returns
 *      t_api           The outcome of the operation
 *                          BT_NOERROR 
 *                          BT_INVALIDPARAM
 * Description
 * This function disables connection less traffic for a group identified
 * by a PSM.
 *
 *********************************************************************/

APIDECL1 t_api APIDECL2 L2_DisableConnectionlessTraffic(u_int16 psm)
{
#if ((pDEBUG==0) && (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED))
u_int8 temp[2];
#endif
    u_int16 psm_ident;
#if pDEBUG
    pDebugPrintfEX((pLOGNOTICE,pLOGL2CAP,"L2CAP: L2_DisableConnectionlessTraffic called, psm=0x%02X\n",psm));
#elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
    BT_WRITE_LE_UINT16(temp,psm);
    SYSDebug_PrintEx(pLOGDEBUG,pLOGL2CAP, (LOG_L2CAP_L2_PSM_INDEX+3),temp,2,BT_CODE_WORD);
#endif

    if (psm != 0x0000)
    {
        psm_ident=prh_l2_psm_get_psm_ident(psm);
        if(psm_ident)
            prh_l2_psm_cb_list[psm_ident].state = PRH_L2_CLT_DISABLED;
        else
            return BT_INVALIDPARAM;
    }
    else
        prh_l2_psm_disable_all();

    return BT_NOERROR;  
}

/********************************************************************
 *
 * Function :- L2_EnableConnectionlessTraffic
 *
 * Parameters :-
 *      u_int16 psm     The PSM for which connectionless traffic is to be 
 *                      disabled. A PSM of 0x0000 will enable connectionless
 *                      traffic for all PSMs and groups.
 * Returns
 *      t_api           The outcome of the operation
 *                          BT_NOERROR 
 *                          BT_INVALIDPARAM
 * Description
 * This function enables connection less traffic for a group identified
 * by PSMs.
 *
 *********************************************************************/

APIDECL1 t_api APIDECL2 L2_EnableConnectionlessTraffic(u_int16 psm) 
{
#if ((pDEBUG==0) && (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED))
u_int8 temp[2];
#endif
    u_int16 psm_ident;
#if pDEBUG
    pDebugPrintfEX((pLOGNOTICE,pLOGL2CAP,"L2CAP: L2_EnableConnectionlessTraffic called, psm=0x%02X\n",psm));

#elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
    BT_WRITE_LE_UINT16(temp,psm);
    SYSDebug_PrintEx(pLOGDEBUG,pLOGL2CAP, (LOG_L2CAP_L2_PSM_INDEX+4),temp,2,BT_CODE_WORD);

    #endif

    if (psm != 0x0000)
    {
        psm_ident=prh_l2_psm_get_psm_ident(psm);
        if(psm_ident)
            prh_l2_psm_cb_list[psm_ident].state = PRH_L2_CLT_ENABLED;
        else
            return BT_INVALIDPARAM;
    }
    else
        prh_l2_psm_enable_all();

    return BT_NOERROR;  
}


/********************************************************************
 *
 * Function :- prh_l2_psm_enable_all
 *
 * Parameters :
 *      u_int16 psm    The Protocol/Service Multiplexer Identifer
 *
 * Returns :
 *      u_int8         PRH_L2_CLT_NONE    00
 *                     PRH_L2_CLT_ENABLED 01
 *                     PRH_L2_CLT_DISABLED 02
 *
 * Description
 * This function checks Connection less traffic on a connectionless
 * PSMs.
 *
 *********************************************************************/

u_int8 prh_l2_psm_get_clt_state(u_int16 psm)
{
    u_int8 psm_ident;

    psm_ident=prh_l2_psm_get_psm_ident(psm);
    if(psm_ident)
        return prh_l2_psm_cb_list[psm_ident].state;
    else
        return 0;
}


/********************************************************************
 *
 * Function :- prh_l2_psm_enable_all
 *
 * Parameters :
 *      NONE
 *
 * Returns :
 *      NONE
 *
 * Description
 * This function enables Connection less traffic on all the connectionless
 * PSMs.
 *
 *********************************************************************/

void prh_l2_psm_enable_all()
{
    int psm_index;

    for (psm_index=0;psm_index<PRH_L2_MAX_NUM_PSM;psm_index++)
    {
        if(prh_l2_psm_cb_list[psm_index].psm != 0x0000)
            prh_l2_psm_cb_list[psm_index].state = PRH_L2_CLT_ENABLED;
    }
}


/********************************************************************
 * Function :- prh_l2_psm_disable_all
 *
 * Parameters :
 *      NONE
 *
 * Returns :
 *      NONE
 *
 * Description
 * This function dis-ables Connection less traffic on all the connectionless
 * PSMs.
 *********************************************************************/

void prh_l2_psm_disable_all()
{
    int psm_index;

    for (psm_index=0;psm_index< PRH_L2_MAX_NUM_PSM;psm_index++)
    {
        if(prh_l2_psm_cb_list[psm_index].psm != 0x0000)
            prh_l2_psm_cb_list[psm_index].state = PRH_L2_CLT_DISABLED;
    }
}

#endif

/*******************************************************************
 *
 * Function :- prh_l2_psm_store_psm
 *
 * Description 
 * Stores a PSM value and returns an index value for that PSM.
 *
 *******************************************************************/

u_int16 prh_l2_psm_store_psm(t_psm psm)
{
    u_int16 psm_index;

    for(psm_index=1;psm_index<PRH_L2_MAX_NUM_PSM; psm_index++)
    {   
        if(prh_l2_psm_id_array[psm_index] == 0x0000) /* Free Entry in the Array */
        { 
            prh_l2_psm_id_array[psm_index] = psm;
            return psm_index;
        }
    }
    return 0;
}

/*******************************************************************
 *
 * Function :- prh_l2_psm_free_psm
 *
 * Description 
 * Frees a PSM id, returning its PSM index.
 *
 *******************************************************************/

u_int16 prh_l2_psm_free_psm(t_psm psm)
{
    u_int16 psm_index;

    for(psm_index=1;psm_index<PRH_L2_MAX_NUM_PSM; psm_index++)
    {   
        if(prh_l2_psm_id_array[psm_index] == psm) /* Free Entry in the Array */
        { 
            prh_l2_psm_id_array[psm_index] = 0x0000;
            prh_l2_psm_cb_list[psm_index].psm = 0x00;
            return psm_index;
        }
    }
    return 0;
}

/*******************************************************************
 *
 * Function :- prh_l2_psm_get_psm_ident
 *
 * Parameter
 *      t_psm psm_value     A psm value.
 *      
 * Returns 
 *      u_int8  The index of the entry for the PSM value in the Connection 
 *              oriented PSM array.
 *                  0x01 -- 0xFF  PSM index
 *                  0x00 -- Invalid PSM.
 * Description 
 * Retrieves the PSM index for a given PSM value
 *
 *******************************************************************/
    
u_int8 prh_l2_psm_get_psm_ident(t_psm psm_value)
{
    u_int8 psm_index;

    for(psm_index=1;psm_index<PRH_L2_MAX_NUM_PSM; psm_index++)
    {   
        if(prh_l2_psm_id_array[psm_index] == psm_value) 
            return psm_index;
    }
    return 0;
}

#if pDEBUG
static char *eventNames[] =
{ 
    "L2CAP_evCONNECTIND","L2CAP_evCONNECTCONF","L2CAP_evCONFIGUREIND","L2CAP_evCONFIGURECONF",
    "L2CAP_evDISCONNECTIND","L2CAP_evDISCONNECTCONF","L2CAP_evQOSVIOLATIONIND","L2CAP_evDATAIND",
    "L2CAP_evBUFFEROVERFLOW","L2CAP_evFLUSHOCCURRED","L2CAP_evTIMEOUTIND","L2CAP_evCONNCONFIGUREDCONF" 
};
#endif

#if PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED

void psm_custom_debug_print(u_int8* temp,u_int8 event, u_int16 psmNumber,t_L2_ControlCallbackArgs*  p_eventinfo, u_int8 index)
{
    u_int8 size=0;
    u_int32 type; 
    u_int8 para=0;

    if(event>12)
        para=0;
    else
        para=event;
    
    if(index!=(LOG_L2CAP_L2_PSM_INDEX+7))
    {
         type=(BT_CODE_EVENT_STRING | (BT_CODE_WORD << 4) | (BT_CODE_WORD << 8) | (BT_CODE_WORD << 12) |(BT_CODE_WORD << 16));
         size=9;
         EncodeData(temp,type,para,psmNumber,p_eventinfo->cid,p_eventinfo->result,p_eventinfo->status);   
            
    }
    else
    {
        type=(BT_CODE_EVENT_STRING | (BT_CODE_WORD << 4) | (BT_CODE_WORD << 8) | (BT_CODE_WORD << 12) |(BT_CODE_WORD << 16) | (BT_CODE_WORD<<20));
        size=11;
        EncodeData(temp,type,para,psmNumber,p_eventinfo->cid,p_eventinfo->result,p_eventinfo->status,p_eventinfo->timerType);
  
    }
    SYSDebug_PrintEx(pLOGNOTICE,pLOGL2CAP, index,temp,size,type);     
}


#endif

  
/*******************************************************************
 *
 * Function :- prh_l2_psm_event
 *
 * Parameters
 *      u_int16 psm_index   The index of the PSM structure in the PSM array.
 *      u_int8 event        Identifier for the event to be sent to the upper layer
 *      struct l2_controlCallbackArgs*  A pointer to a structure containing the parameters
 *                          to the event (i.e. callback function) to be sent to the 
 *                          higher layers.
 *
 * Returns
 *      NONE            
 *      
 * Description 
 * This function is the router for callback events on specific PSMs.
 * Calling this function with the Event Id & PSM index calls back the
 * higher layer function associated with the event,
 *
 *******************************************************************/

t_api prh_l2_psm_event(u_int16 psm_index, u_int8 event,t_L2_ControlCallbackArgs*  p_eventinfo)
{

#if ((pDEBUG==0) && (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED))
    u_int8 temp[50];    
    u_int8 index=0;
#endif

    t_api status=BT_NOERROR;

    struct l2cap_cbs* p_cbs;

#if pDEBUG || PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED
//#if PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED
    //static u_int32 psmNumber;
    static u_int16 psmNumber;
/*
    static char *eventNames[] = { 
        "L2CAP_evCONNECTIND","L2CAP_evCONNECTCONF","L2CAP_evCONFIGUREIND","L2CAP_evCONFIGURECONF",
        "L2CAP_evDISCONNECTIND","L2CAP_evDISCONNECTCONF","L2CAP_evQOSVIOLATIONIND","L2CAP_evDATAIND",
        "L2CAP_evBUFFEROVERFLOW","L2CAP_evFLUSHOCCURRED","L2CAP_evTIMEOUTIND","L2CAP_evCONNCONFIGUREDCONF" 
    };
    */
    psmNumber=prh_l2_psm_id_array[psm_index];
#endif

    p_cbs = &prh_l2_psm_cb_list[psm_index].psm_cbs;

    

    switch (event)
    {
#if pDEBUG
    case L2CAP_evCONNECTIND :
        if (p_cbs->connectionIndication) {
            pDebugPrintfEX((pLOGNOTICE,pLOGL2CAP,"L2CAP: About to deliver event %s on psm 0x%02X, cid=0x%02X, result=0x%02X, status=0x%02X\n",event>12?"<UNKNOWN>":eventNames[event-1],psmNumber,p_eventinfo->cid,p_eventinfo->result,p_eventinfo->status));
            p_cbs->connectionIndication(L2CAP_evCONNECTIND,p_eventinfo);
        } else {
            pDebugPrintfEX((pLOGNOTICE,pLOGL2CAP,"L2CAP: Cannot deliver event %s (no callback set) on psm 0x%02X, cid=0x%02X, result=0x%02X, status=0x%02X\n",event>12?"<UNKNOWN>":eventNames[event-1],psmNumber,p_eventinfo->cid,p_eventinfo->result,p_eventinfo->status));
            status = BT_INVALIDPARAM;
        }
        break;

    case L2CAP_evCONNECTCONF :
        if (p_cbs->connectionConfirmation) {
            pDebugPrintfEX((pLOGNOTICE,pLOGL2CAP,"L2CAP: About to deliver event %s on psm 0x%02X, cid=0x%02X, result=0x%02X, status=0x%02X\n",event>12?"<UNKNOWN>":eventNames[event-1],psmNumber,p_eventinfo->cid,p_eventinfo->result,p_eventinfo->status));
            p_cbs->connectionConfirmation(L2CAP_evCONNECTCONF,p_eventinfo);
        } else {
            pDebugPrintfEX((pLOGNOTICE,pLOGL2CAP,"L2CAP: Cannot deliver event %s (no callback set) on psm 0x%02X, cid=0x%02X, result=0x%02X, status=0x%02X\n",event>12?"<UNKNOWN>":eventNames[event-1],psmNumber,p_eventinfo->cid,p_eventinfo->result,p_eventinfo->status));
            status = BT_INVALIDPARAM;
        }
        break;

    case L2CAP_evCONFIGUREIND  :
        if (p_cbs->configureIndication) {
            pDebugPrintfEX((pLOGNOTICE,pLOGL2CAP,"L2CAP: About to deliver event %s on psm 0x%02X, cid=0x%02X, result=0x%02X, status=0x%02X\n",event>12?"<UNKNOWN>":eventNames[event-1],psmNumber,p_eventinfo->cid,p_eventinfo->result,p_eventinfo->status));
            p_cbs->configureIndication(L2CAP_evCONFIGUREIND,p_eventinfo);
        } else {
            pDebugPrintfEX((pLOGNOTICE,pLOGL2CAP,"L2CAP: Cannot deliver event %s (no callback set) on psm 0x%02X, cid=0x%02X, result=0x%02X, status=0x%02X\n",event>12?"<UNKNOWN>":eventNames[event-1],psmNumber,p_eventinfo->cid,p_eventinfo->result,p_eventinfo->status));
            status = BT_INVALIDPARAM;
        }
        break;

    case L2CAP_evCONFIGURECONF :
        if (p_cbs->configureConfirmation) {
            pDebugPrintfEX((pLOGNOTICE,pLOGL2CAP,"L2CAP: About to deliver event %s on psm 0x%02X, cid=0x%02X, result=0x%02X, status=0x%02X\n",event>12?"<UNKNOWN>":eventNames[event-1],psmNumber,p_eventinfo->cid,p_eventinfo->result,p_eventinfo->status));
            p_cbs->configureConfirmation(L2CAP_evCONFIGURECONF,p_eventinfo);
        } else {
            pDebugPrintfEX((pLOGNOTICE,pLOGL2CAP,"L2CAP: Cannot deliver event %s (no callback set) on psm 0x%02X, cid=0x%02X, result=0x%02X, status=0x%02X\n",event>12?"<UNKNOWN>":eventNames[event-1],psmNumber,p_eventinfo->cid,p_eventinfo->result,p_eventinfo->status));
            status = BT_INVALIDPARAM;
        }
        break;

    case L2CAP_evDISCONNECTIND :
        if (p_cbs->disconnectionIndication) {
            pDebugPrintfEX((pLOGNOTICE,pLOGL2CAP,"L2CAP: About to deliver event %s on psm 0x%02X, cid=0x%02X, result=0x%02X, status=0x%02X\n",event>12?"<UNKNOWN>":eventNames[event-1],psmNumber,p_eventinfo->cid,p_eventinfo->result,p_eventinfo->status));
            p_cbs->disconnectionIndication(L2CAP_evDISCONNECTIND,p_eventinfo);
        } else {
            pDebugPrintfEX((pLOGNOTICE,pLOGL2CAP,"L2CAP: Cannot deliver event %s (no callback set) on psm 0x%02X, cid=0x%02X, result=0x%02X, status=0x%02X\n",event>12?"<UNKNOWN>":eventNames[event-1],psmNumber,p_eventinfo->cid,p_eventinfo->result,p_eventinfo->status));
            status = BT_INVALIDPARAM;
        }
        break;

    case L2CAP_evDISCONNECTCONF :
        if(p_cbs->disconnectionConfirmation) {
            pDebugPrintfEX((pLOGNOTICE,pLOGL2CAP,"L2CAP: About to deliver event %s on psm 0x%02X, cid=0x%02X, result=0x%02X, status=0x%02X\n",event>12?"<UNKNOWN>":eventNames[event-1],psmNumber,p_eventinfo->cid,p_eventinfo->result,p_eventinfo->status));
            p_cbs->disconnectionConfirmation(L2CAP_evDISCONNECTCONF,p_eventinfo);
        } else {
            pDebugPrintfEX((pLOGNOTICE,pLOGL2CAP,"L2CAP: Cannot deliver event %s (no callback set) on psm 0x%02X, cid=0x%02X, result=0x%02X, status=0x%02X\n",event>12?"<UNKNOWN>":eventNames[event-1],psmNumber,p_eventinfo->cid,p_eventinfo->result,p_eventinfo->status));
            status = BT_INVALIDPARAM;
        }
        break;

    case L2CAP_evQOSVIOLATIONIND :
        if(p_cbs->qosviolationIndication) {
            pDebugPrintfEX((pLOGNOTICE,pLOGL2CAP,"L2CAP: About to deliver event %s on psm 0x%02X, cid=0x%02X, result=0x%02X, status=0x%02X\n",event>12?"<UNKNOWN>":eventNames[event-1],psmNumber,p_eventinfo->cid,p_eventinfo->result,p_eventinfo->status));
            p_cbs->qosviolationIndication(L2CAP_evQOSVIOLATIONIND,p_eventinfo);
        } else {
            pDebugPrintfEX((pLOGNOTICE,pLOGL2CAP,"L2CAP: Cannot deliver event %s (no callback set) on psm 0x%02X, cid=0x%02X, result=0x%02X, status=0x%02X\n",event>12?"<UNKNOWN>":eventNames[event-1],psmNumber,p_eventinfo->cid,p_eventinfo->result,p_eventinfo->status));
            status = BT_INVALIDPARAM;
        }
        break;

    case L2CAP_evDATAIND :
        if (p_cbs->dataIndication) {
            pDebugPrintfEX((pLOGNOTICE,pLOGL2CAP,"L2CAP: About to deliver event %s on psm 0x%02X, cid=0x%02X, result=0x%02X, status=0x%02X\n",event>12?"<UNKNOWN>":eventNames[event-1],psmNumber,p_eventinfo->cid,p_eventinfo->result,p_eventinfo->status));
            p_cbs->dataIndication(p_eventinfo->cid, p_eventinfo->p_buf);
        } else {
            pDebugPrintfEX((pLOGNOTICE,pLOGL2CAP,"L2CAP: Cannot deliver event %s (no callback set) on psm 0x%02X, cid=0x%02X, result=0x%02X, status=0x%02X\n",event>12?"<UNKNOWN>":eventNames[event-1],psmNumber,p_eventinfo->cid,p_eventinfo->result,p_eventinfo->status));
            status = BT_INVALIDPARAM;
        }
        break;

    case L2CAP_evTIMEOUTIND:
        if (p_cbs->timeoutIndication) {
            pDebugPrintfEX((pLOGNOTICE,pLOGL2CAP,"L2CAP: About to deliver event %s on psm 0x%02X, cid=0x%02X, result=0x%02X, status=0x%02X, timerType=0x%02X\n",event>12?"<UNKNOWN>":eventNames[event-1],psmNumber,p_eventinfo->cid,p_eventinfo->result,p_eventinfo->status,p_eventinfo->timerType));
            p_cbs->timeoutIndication(L2CAP_evTIMEOUTIND,p_eventinfo);
        } else {
            pDebugPrintfEX((pLOGNOTICE,pLOGL2CAP,"L2CAP: Cannot deliver event %s (no callback set) on psm 0x%02X, cid=0x%02X, result=0x%02X, status=0x%02X\n",event>12?"<UNKNOWN>":eventNames[event-1],psmNumber,p_eventinfo->cid,p_eventinfo->result,p_eventinfo->status));
            status = BT_INVALIDPARAM;
        }
        break;


	case L2CAP_evBUFFEROVERFLOW :
		if (p_cbs->bufferOverflow) {
            pDebugPrintfEX((pLOGNOTICE,pLOGL2CAP,"L2CAP: About to deliver event %s on psm 0x%02X, cid=0x%02X, result=0x%02X, status=0x%02X\n",event>12?"<UNKNOWN>":eventNames[event-1],psmNumber,p_eventinfo->cid,p_eventinfo->result,p_eventinfo->status));
			p_cbs->bufferOverflow(L2CAP_evBUFFEROVERFLOW,p_eventinfo);
        } else {
            pDebugPrintfEX((pLOGNOTICE,pLOGL2CAP,"L2CAP: Cannot deliver event %s (no callback set) on psm 0x%02X, cid=0x%02X, result=0x%02X, status=0x%02X\n",event>12?"<UNKNOWN>":eventNames[event-1],psmNumber,p_eventinfo->cid,p_eventinfo->result,p_eventinfo->status));
            status = BT_INVALIDPARAM;
        }
        break;

	case L2CAP_evFLUSHOCCURRED :
		if (p_cbs->flushOccurred) {
            pDebugPrintfEX((pLOGNOTICE,pLOGL2CAP,"L2CAP: About to deliver event %s on psm 0x%02X, cid=0x%02X, result=0x%02X, status=0x%02X\n",event>12?"<UNKNOWN>":eventNames[event-1],psmNumber,p_eventinfo->cid,p_eventinfo->result,p_eventinfo->status));
			p_cbs->flushOccurred(L2CAP_evFLUSHOCCURRED,p_eventinfo);
        } else {
            pDebugPrintfEX((pLOGNOTICE,pLOGL2CAP,"L2CAP: Cannot deliver event %s (no callback set) on psm 0x%02X, cid=0x%02X, result=0x%02X, status=0x%02X\n",event>12?"<UNKNOWN>":eventNames[event-1],psmNumber,p_eventinfo->cid,p_eventinfo->result,p_eventinfo->status));
            status = BT_INVALIDPARAM;
        }
        break;

#if (PRH_L2CAP_AUTO_CONFIG==1)
    case L2CAP_evCONNCONFIGUREDCONF:
        if (p_cbs->connectionConfiguredConfirmation) {
            pDebugPrintfEX((pLOGNOTICE,pLOGL2CAP,"L2CAP: About to deliver event %s on psm 0x%02X, cid=0x%02X, result=0x%02X, status=0x%02X\n",event>12?"<UNKNOWN>":eventNames[event-1],psmNumber,p_eventinfo->cid,p_eventinfo->result,p_eventinfo->status));
			p_cbs->connectionConfiguredConfirmation(L2CAP_evCONNCONFIGUREDCONF,p_eventinfo);
        } else {
            pDebugPrintfEX((pLOGNOTICE,pLOGL2CAP,"L2CAP: Cannot deliver event %s (no callback set) on psm 0x%02X, cid=0x%02X, result=0x%02X, status=0x%02X\n",event>12?"<UNKNOWN>":eventNames[event-1],psmNumber,p_eventinfo->cid,p_eventinfo->result,p_eventinfo->status));
            status = BT_INVALIDPARAM;
        }
        break;
#endif


#elif(PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)

 case L2CAP_evCONNECTIND :
        if (p_cbs->connectionIndication) 
        {
            index=LOG_L2CAP_L2_SAR_INDEX+5;
            psm_custom_debug_print(temp,event,psmNumber,p_eventinfo,index);
            p_cbs->connectionIndication(L2CAP_evCONNECTIND,p_eventinfo);
        } 
        else 
        {
            index=LOG_L2CAP_L2_SAR_INDEX+6;
            psm_custom_debug_print(temp,event,psmNumber,p_eventinfo,index);  
            status = BT_INVALIDPARAM;
        }        
        break;

    case L2CAP_evCONNECTCONF :
        if (p_cbs->connectionConfirmation) 
        {
            index=LOG_L2CAP_L2_SAR_INDEX+5;
            psm_custom_debug_print(temp,event,psmNumber,p_eventinfo,index);
            p_cbs->connectionConfirmation(L2CAP_evCONNECTCONF,p_eventinfo);
        } 
        else 
        {
            index=LOG_L2CAP_L2_SAR_INDEX+6;
            psm_custom_debug_print(temp,event,psmNumber,p_eventinfo,index);  
            status = BT_INVALIDPARAM;
        }
        break;

    case L2CAP_evCONFIGUREIND  :
        if (p_cbs->configureIndication) 
        {
            index=LOG_L2CAP_L2_SAR_INDEX+5;
            psm_custom_debug_print(temp,event,psmNumber,p_eventinfo,index);
            p_cbs->configureIndication(L2CAP_evCONFIGUREIND,p_eventinfo);
        } 
        else 
        {
            index=LOG_L2CAP_L2_SAR_INDEX+6;
            psm_custom_debug_print(temp,event,psmNumber,p_eventinfo,index);  
            status = BT_INVALIDPARAM;            
        }
        break;

    case L2CAP_evCONFIGURECONF :
        if (p_cbs->configureConfirmation) 
        {
            index=LOG_L2CAP_L2_SAR_INDEX+5;
            psm_custom_debug_print(temp,event,psmNumber,p_eventinfo,index);
            p_cbs->configureConfirmation(L2CAP_evCONFIGURECONF,p_eventinfo);
        } 
        else 
        {
            index=LOG_L2CAP_L2_SAR_INDEX+6;
            psm_custom_debug_print(temp,event,psmNumber,p_eventinfo,index);  
            status = BT_INVALIDPARAM;
        }
	   break;
    case L2CAP_evDISCONNECTIND :
        if (p_cbs->disconnectionIndication) 
        {
            index=LOG_L2CAP_L2_SAR_INDEX+5;
            psm_custom_debug_print(temp,event,psmNumber,p_eventinfo,index);
            p_cbs->disconnectionIndication(L2CAP_evDISCONNECTIND,p_eventinfo);
        } 
        else 
        {
            index=LOG_L2CAP_L2_SAR_INDEX+6;
            psm_custom_debug_print(temp,event,psmNumber,p_eventinfo,index);  
            status = BT_INVALIDPARAM;
        }
        break;

    case L2CAP_evDISCONNECTCONF :
        if(p_cbs->disconnectionConfirmation) 
        {
            index=LOG_L2CAP_L2_SAR_INDEX+5;
            psm_custom_debug_print(temp,event,psmNumber,p_eventinfo,index);
            p_cbs->disconnectionConfirmation(L2CAP_evDISCONNECTCONF,p_eventinfo);
            
        } 
        else 
        {
            index=LOG_L2CAP_L2_SAR_INDEX+6;
            psm_custom_debug_print(temp,event,psmNumber,p_eventinfo,index);  
            status = BT_INVALIDPARAM;
        }
        break;

    case L2CAP_evQOSVIOLATIONIND :
        if(p_cbs->qosviolationIndication) 
        {
            index=LOG_L2CAP_L2_SAR_INDEX+5;
            psm_custom_debug_print(temp,event,psmNumber,p_eventinfo,index);
            p_cbs->qosviolationIndication(L2CAP_evQOSVIOLATIONIND,p_eventinfo);
        } 
        else 
        {
            index=LOG_L2CAP_L2_SAR_INDEX+6;
            psm_custom_debug_print(temp,event,psmNumber,p_eventinfo,index);  
            status = BT_INVALIDPARAM;
        }
        break;

    case L2CAP_evDATAIND :
        if (p_cbs->dataIndication) 
        {
            index=LOG_L2CAP_L2_SAR_INDEX+5;
            //psm_custom_debug_print(temp,event,psmNumber,p_eventinfo,index);
            p_cbs->dataIndication(p_eventinfo->cid, p_eventinfo->p_buf);
        }
        else 
        {
            index=LOG_L2CAP_L2_SAR_INDEX+6;
            psm_custom_debug_print(temp,event,psmNumber,p_eventinfo,index);  
            status = BT_INVALIDPARAM;
        }
        break;

    case L2CAP_evTIMEOUTIND:
        if (p_cbs->timeoutIndication) 
        {
            index=LOG_L2CAP_L2_SAR_INDEX+7;
            psm_custom_debug_print(temp,event,psmNumber,p_eventinfo,index);  
            
            p_cbs->timeoutIndication(L2CAP_evTIMEOUTIND,p_eventinfo);
        } 
        else 
        {
            index=LOG_L2CAP_L2_SAR_INDEX+6;
            psm_custom_debug_print(temp,event,psmNumber,p_eventinfo,index);  
            status = BT_INVALIDPARAM;
        }
        break;


    case L2CAP_evBUFFEROVERFLOW :
        if (p_cbs->bufferOverflow) 
        {
            index=LOG_L2CAP_L2_SAR_INDEX+5;
            psm_custom_debug_print(temp,event,psmNumber,p_eventinfo,index);
            p_cbs->bufferOverflow(L2CAP_evBUFFEROVERFLOW,p_eventinfo);
        } 
        else 
        {
            index=LOG_L2CAP_L2_SAR_INDEX+6;
            psm_custom_debug_print(temp,event,psmNumber,p_eventinfo,index);  
            status = BT_INVALIDPARAM;
        }
        break;

    case L2CAP_evFLUSHOCCURRED :
        if (p_cbs->flushOccurred) 
        {
            index=LOG_L2CAP_L2_SAR_INDEX+5;
            psm_custom_debug_print(temp,event,psmNumber,p_eventinfo,index);
            p_cbs->flushOccurred(L2CAP_evFLUSHOCCURRED,p_eventinfo);
        } 
        else 
        {
            index=LOG_L2CAP_L2_SAR_INDEX+6;
            psm_custom_debug_print(temp,event,psmNumber,p_eventinfo,index);  
            status = BT_INVALIDPARAM;
        }
        break;

#if (PRH_L2CAP_AUTO_CONFIG==1)
    case L2CAP_evCONNCONFIGUREDCONF:
        if (p_cbs->connectionConfiguredConfirmation) 
        {
            index=LOG_L2CAP_L2_SAR_INDEX+5;
            psm_custom_debug_print(temp,event,psmNumber,p_eventinfo,index);
            p_cbs->connectionConfiguredConfirmation(L2CAP_evCONNCONFIGUREDCONF,p_eventinfo);
        } 
        else 
        {
            index=LOG_L2CAP_L2_SAR_INDEX+6;
            psm_custom_debug_print(temp,event,psmNumber,p_eventinfo,index);  
            status = BT_INVALIDPARAM;
        }
        break;
#endif

#else
    case L2CAP_evCONNECTIND :
        if (p_cbs->connectionIndication) 
            p_cbs->connectionIndication(L2CAP_evCONNECTIND,p_eventinfo);
        else 
            status = BT_INVALIDPARAM;
        break;

    case L2CAP_evCONNECTCONF :
        if (p_cbs->connectionConfirmation) 
            p_cbs->connectionConfirmation(L2CAP_evCONNECTCONF,p_eventinfo);
        else
            status = BT_INVALIDPARAM;
        break;

    case L2CAP_evCONFIGUREIND  :
        if (p_cbs->configureIndication) 
            p_cbs->configureIndication(L2CAP_evCONFIGUREIND,p_eventinfo);
        else 
            status = BT_INVALIDPARAM;
        break;

    case L2CAP_evCONFIGURECONF :
        if (p_cbs->configureConfirmation)  
            p_cbs->configureConfirmation(L2CAP_evCONFIGURECONF,p_eventinfo); 
        else 
            status = BT_INVALIDPARAM;
        break;

    case L2CAP_evDISCONNECTIND :
        if (p_cbs->disconnectionIndication) 
            p_cbs->disconnectionIndication(L2CAP_evDISCONNECTIND,p_eventinfo);
        else
            status = BT_INVALIDPARAM;
        break;

    case L2CAP_evDISCONNECTCONF :
        if(p_cbs->disconnectionConfirmation) 
            p_cbs->disconnectionConfirmation(L2CAP_evDISCONNECTCONF,p_eventinfo); 
        else 
            status = BT_INVALIDPARAM;
        break;

    case L2CAP_evQOSVIOLATIONIND :
        if(p_cbs->qosviolationIndication) 
            p_cbs->qosviolationIndication(L2CAP_evQOSVIOLATIONIND,p_eventinfo); 
        else 
            status = BT_INVALIDPARAM;
        break;

    case L2CAP_evDATAIND :
        if (p_cbs->dataIndication)  
            p_cbs->dataIndication(p_eventinfo->cid, p_eventinfo->p_buf); 
        else  
            status = BT_INVALIDPARAM;
        break;

    case L2CAP_evTIMEOUTIND:
        if (p_cbs->timeoutIndication) 
            p_cbs->timeoutIndication(L2CAP_evTIMEOUTIND,p_eventinfo); 
        else 
            status = BT_INVALIDPARAM;
        break;


	case L2CAP_evBUFFEROVERFLOW :
        if (p_cbs->bufferOverflow) 
            p_cbs->bufferOverflow(L2CAP_evBUFFEROVERFLOW,p_eventinfo); 
        else 
            status = BT_INVALIDPARAM;
        break;

	case L2CAP_evFLUSHOCCURRED :
		if (p_cbs->flushOccurred) 
            p_cbs->flushOccurred(L2CAP_evFLUSHOCCURRED,p_eventinfo); 
        else 
            status = BT_INVALIDPARAM;
        break;

#if (PRH_L2CAP_AUTO_CONFIG==1)
    case L2CAP_evCONNCONFIGUREDCONF:
        if (p_cbs->connectionConfiguredConfirmation) 
            p_cbs->connectionConfiguredConfirmation(L2CAP_evCONNCONFIGUREDCONF,p_eventinfo); 
        else 
            status = BT_INVALIDPARAM;
        break;
#endif

#endif
    default :
            status = BT_INVALIDPARAM;
        break;

    }

   return BT_NOERROR;
}
