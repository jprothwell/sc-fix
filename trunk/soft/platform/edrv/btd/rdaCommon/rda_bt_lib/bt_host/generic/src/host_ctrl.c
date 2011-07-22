/*
 * MODULE NAME:    host_ctrl.c
 * DESCRIPTION:    Bluestream control functions implementation
 * AUTHOR:         Martin Mellody
 *
 * SOURCE CONTROL: $Id: host_ctrl.c 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2002 Parthus Technologies.
 *     All rights reserved.
 *
 */
 
#include "host_config.h"

#include "host_types.h"
#include "host_ctrl.h"
#include "papi.h"

#if (PRH_HOST_UNIT_TEST==1)
#include "stubs/unit_stub_hcit.h"
#else
#include "hci_transport.h"
#endif

#include "hci_flow_ctrl.h"
#include "hci_primitives.h"
#include "host_error.h"
#include "mgr_core.h"



t_pMutex prh_bt_execute_lock;
t_pEvent prh_scheduler_sleep_eventvariable;
t_pEvent bootupWaitForHC;
t_pEvent prh_scheduler_terminated;
volatile u_int8 prh_scheduler_terminate_flag;
volatile u_int8 resetCompleted;
static t_api host_layer_ctrl(u_int8 layerFlags);
static t_api send_raw_reset(void);
static t_api prh_bt_execute(u_int32 maxIterations);
extern volatile u_int8 prh_host_reset_required;
volatile u_int8 prh_hci_expect_reset;

volatile u_int8 bt_execute_in_process=0;
#if pDEBUG
void print_build_options(void);
#endif


volatile extern t_api prh_host_startup_complete;

#if (SINGLE_THREAD_FLAG==1)  //Zhu Jianguo added it. 2008.2.1
/**
tianwq: add host schedular for signle thread
*/
#if (PRH_BT_COMBINED==1)
void  BThost_Sched(u_int32 num_interations);
void  BTapp_Sched(u_int32 num_interations);
#endif
#endif


/*************************************************************************************************
 * BT_Execute
 *
 * This function performs system flow control (for split stack configuration). This 
 * function must be called periodically, either from a host scheduler 'task' or from an application 
 * event loop (not both). A number of iterations is provided which controls (on a system-specific 
 * scale) the amount of processing performed in this function call.
 * This function returns BT_NOERROR if no more data can currently be processed (none is queued or 
 * queued data cannot be processed due to flow control. This function returns BT_RETRY if there 
 * is data queued which would have been sent now, but cannot due to insufficient iterations 
 * specified (i.e. call this function again as soon as possible). 
 *   See Host Stack Architecture Document re: BT_Execute
 ************************************************************************************************/
APIDECL1 t_api APIDECL2 BT_Execute(u_int32 maxIterations) {
#if SCHEDULER_PRESENT
    return BT_UNSUPPORTED;
#else
    return prh_bt_execute(maxIterations);
#endif
}

/* This inlined because it is called from only one of two places through wrappers */
pINLINEFUNC t_api prh_bt_execute(u_int32 maxIterations) { 

    u_int32 iterations;
    t_api status;

    //pDebugPrintfEX((pLOGDEBUG,pLOGALWAYS,"Host_Execute: called, maxIterations=0x%02X\n",maxIterations)); 

    if (prh_host_reset_required) /* BT_Reset clears this flag */
        BT_Reset(0);

#if SCHEDULER_PRESENT || COMBINED_HOST==1
    pMutexLock(prh_bt_execute_lock);
#endif

	if (prh_scheduler_terminate_flag) 
        {
#if SCHEDULER_PRESENT || COMBINED_HOST==1
		pMutexUnlock(prh_bt_execute_lock);
#endif
		return BT_DISALLOWED;
	}



    iterations=maxIterations;

    do {
	 bt_execute_in_process = 1;	
        status = prh_hci_fc_process_outgoing(&iterations); /* hci flow control is handled here */

        if (status && status!=BT_RETRY) {
            //pDebugPrintfEX((pLOGNOTICE,pLOGALWAYS,"Host_Execute: got status return from HCI_ProcessOutgoingFlowControl, iterations=0x%02X\n",iterations)); 
            break;
        }

        if (status!=BT_RETRY) {            
            /* nothing was sent last time, so no point looping. Flow control is not blocking data throughput */
           // pDebugPrintfEX((pLOGDEBUG,pLOGALWAYS,"Host_Execute: nothing was sent last time, so no point looping, iterations=0x%02X\n", iterations));
            break;
        }

        //pDebugPrintfEX((pLOGDEBUG,pLOGALWAYS,"Host_Execute: iterations=0x%02X\n",iterations)); 
    }   while (status==BT_RETRY && iterations);
    bt_execute_in_process = 0;
#if SCHEDULER_PRESENT || COMBINED_HOST==1
    pMutexUnlock(prh_bt_execute_lock);
#endif

    return status;
}

t_api transport_callback_func(struct host_buf *pdu, u_int8 type) {
    if (!resetCompleted && type==HCI_pduEVENT && pdu->data[0]==HCI_evCOMMANDCOMPLETE && BT_READ_LE_UINT16(pdu->data+3)==HCI_cmdRESET) {
        pDebugPrintfEX((pLOGDEBUG,pLOGALWAYS,"Got CommandComplete for HCI_Reset - continuing with BT_Init bootup sequence\n"));
        resetCompleted=1;
        pEventSet(bootupWaitForHC);
    }
#if pDEBUG
    else {
        pDebugPrintfEX((pLOGNOTICE,pLOGALWAYS,"Discarding incoming data packet of type %d, len %d suring host bootup sequence\n", type, pdu->len));
    }
#endif
	host_buf_free(pdu);
    return BT_NOERROR;
}

void bootupResetFailureTimerCallback(void *unused) {
        pDebugPrintfEX((pLOGNOTICE,pLOGALWAYS,"Bootup Reset Failure Timer Callback fired\n"));
#if HCI_PRESENT
        HCI_Transport_Shutdown(); /* close this here as in some transports we may be blocked (e.g. on RTS/CTS) trying to send the reset */
#endif
        pEventSet(bootupWaitForHC);
}

/*************************************************************************************************
 * BT_Init
 *
 * This function initialises the stack (host or combined). This initialises the PAPI and initiates
 * communication with the host controller (for split stack configuration). 
 * This function performs stack initialisation and returns immediately.
 *   See Host Stack Architecture Document re: BT_Execute
 * The stackConfig parameter is reserved for future stack configuration options.
 * The platformConfig parameter is passed to all platform-specific code and may be 
 * used by individual ports in a platform-specific manner.
 *
 ************************************************************************************************/
 #if (SINGLE_THREAD_FLAG==1)  //Zhu Jianguo added it. 2008.2.1
 int  BT_Init_Step2(void *dummy);
static t_pTimer bootupResetFailureTimerHandle;
static u_int8 BT_Init_Step = 0;
#endif

#if (SINGLE_THREAD_FLAG==0)  //Zhu Jianguo added it. 2008.2.1
APIDECL1 t_api APIDECL2 BT_Init(void *stackConfig, void *platformConfig) {

    t_api status;
	
    t_pTimer bootupResetFailureTimerHandle;

    status=pInit(platformConfig); 
    if (status!=BT_NOERROR) {
        pDebugPrintfEX((pLOGERROR,pLOGALWAYS, "PAPI failed to initialise - error code %d\n",status));
        prh_host_startup_complete=status;
        return status;
    } 
    bt_host_cb_init();

#if pDEBUG
    print_build_options();
#endif

#if HCI_PRESENT
    status=HCI_Transport_Init(platformConfig); 
    if (status!=BT_NOERROR) {
        pDebugPrintfEX((pLOGERROR,pLOGALWAYS,"HCI Transport failed to initialise - error code %d\n",status));
        prh_host_startup_complete=status;
        return status;
    } 
#endif

#if HCI_PRESENT
    bootupWaitForHC=pEventCreate(0);
    HCI_Transport_Set_Rx_CB(transport_callback_func);
    resetCompleted=0;
    status=send_raw_reset();
    if (status) {
        pDebugPrintfEX((pLOGERROR,pLOGALWAYS,"Cannot send HCI reset PDU - error code %d\n",status));
        HCI_Transport_Shutdown();
        prh_host_startup_complete=status;
       return status;
    }
    bootupResetFailureTimerHandle=pTimerCreate(BOOTUP_HC_RESPONSE_TIMEOUT,bootupResetFailureTimerCallback,NULL,pTIMER_ONESHOT);
    pEventWait(bootupWaitForHC,NULL,NULL);    
	if(bootupResetFailureTimerHandle)
	{
    	pTimerCancel(bootupResetFailureTimerHandle);
		bootupResetFailureTimerHandle = 0;
	}
    HCI_Transport_Set_Rx_CB(NULL);
    pEventFree(bootupWaitForHC);
    if (!resetCompleted) {
        pDebugPrintfEX((pLOGERROR,pLOGALWAYS,"Did not receive command complete for HCI_Reset - aborting bootup\n"));
        pShutdown();
        prh_host_startup_complete=BT_HCITERROR;
        return BT_HCITERROR;
    }
#endif

    status=host_layer_ctrl(PRH_HOST_STARTUP);
    if (status) {        
#if pDEBUG
        t_api status2;
        pDebugPrintfEX((pLOGNOTICE,pLOGALWAYS,"Got error %d from layer startup.\n",status));
        status2=host_layer_ctrl(PRH_HOST_SHUTDOWN);
        if (status2) {
            pDebugPrintfEX((pLOGNOTICE,pLOGALWAYS,"Got error %d from layer shutdown during failed startup cleanup.\n",status2));
        }
#else
        host_layer_ctrl(PRH_HOST_SHUTDOWN);
#endif        
#if HCI_PRESENT
        HCI_Transport_Shutdown();
#endif
        pShutdown();
        prh_host_startup_complete=status;
        return status;
    }

#if SCHEDULER_PRESENT
    prh_scheduler_terminate_flag=0;
    prh_scheduler_sleep_eventvariable=pEventCreate(0); /* this may be used by the HCI when it starts */
#endif

	status=0;
	status |= HCI_ReadBDAddress(); /* event trapped by the manager */

	status |= HCI_ReadLocalVersionInformation(); /* event trapped by the HCI & manager */

	status |= HCI_ReadLocalSupportedFeatures(); /* event trapped by the manager */

	status |= HCI_ReadScanEnable(); /* event trapped by the manager */
	
#if PRH_HOST_HC_TO_HOST_FLOW_CONTROL_SUPPORTED    
	/* This command does not actually turn on hc->host flow control - this is done in hci_callbacks.c */
	status |= HCI_HostBufferSize(HCI_HC_TO_HOST_FLOWCTRL_ACL_SIZE,HCI_HC_TO_HOST_FLOWCTRL_SCO_SIZE,HCI_HC_TO_HOST_FLOWCTRL_ACL_NUM,HCI_HC_TO_HOST_FLOWCTRL_SCO_NUM);  
#endif
	status |= HCI_ReadBufferSize(); /* event trapped by the hci - signals init complete */

        status |= HCI_SetHostControllerToHostFlowControl(TRUE);

        status |= HCI_WriteClassOfDevice(0x5a0204);

        status |= HCI_WritePageTimeout(0x2000);

    if (status) {
        pDebugPrintfEX((pLOGERROR,pLOGALWAYS,"BT_Init: failure during stack init\n",status));
#if SCHEDULER_PRESENT
        pEventFree(prh_scheduler_sleep_eventvariable);
#endif
        host_layer_ctrl(PRH_HOST_SHUTDOWN);
#if HCI_PRESENT
        HCI_Transport_Shutdown();
#endif
        pShutdown();
        prh_host_startup_complete=BT_UNKNOWNERROR;
        return BT_UNKNOWNERROR;
    }

#if SCHEDULER_PRESENT
    prh_scheduler_terminated=pEventCreate(0);
    prh_bt_execute_lock=pMutexCreate(0);
#endif

    pDebugPrintfEX((pLOGNOTICE,pLOGALWAYS,"BT_Init: stack initialisation complete.\n"));
	pDebugCheck();
    return BT_NOERROR;
}
#else
APIDECL1 t_api APIDECL2 BT_Init(void *stackConfig, void *platformConfig) {

    t_api status;

    status=pInit(platformConfig); 
    if (status!=BT_NOERROR) {
        pDebugPrintfEX((pLOGERROR,pLOGALWAYS, "PAPI failed to initialise - error code %d\n",status));
        prh_host_startup_complete=status;
        return status;
    } 

#if pDEBUG
    print_build_options();
#endif

#if HCI_PRESENT
    status=HCI_Transport_Init(platformConfig); 
    if (status!=BT_NOERROR) {
        pDebugPrintfEX((pLOGERROR,pLOGALWAYS,"HCI Transport failed to initialise - error code %d\n",status));
        prh_host_startup_complete=status;
       return status;
    } 
#endif


#if HCI_PRESENT
    bootupWaitForHC=pEventCreate(0);
    HCI_Transport_Set_Rx_CB(transport_callback_func);
    resetCompleted=0;
    status=send_raw_reset();
    if (status) {
        pDebugPrintfEX((pLOGERROR,pLOGALWAYS,"Cannot send HCI reset PDU - error code %d\n",status));
        HCI_Transport_Shutdown();
        prh_host_startup_complete=status;
       return status;
    }

	
    bootupResetFailureTimerHandle=pTimerCreate(BOOTUP_HC_RESPONSE_TIMEOUT,bootupResetFailureTimerCallback,NULL,pTIMER_ONESHOT);
    pEventWait(bootupWaitForHC, BT_Init_Step2, 0);    

	BT_Init_Step = 1;
	return BT_NOERROR;
}

int BT_Init_Step2(void *dummy)
{
     t_api status;

if(bootupResetFailureTimerHandle)
{
    pTimerCancel(bootupResetFailureTimerHandle);
	bootupResetFailureTimerHandle = 0;
}
    HCI_Transport_Set_Rx_CB(NULL);
    pEventFree(bootupWaitForHC);
    if (!resetCompleted) {
        pDebugPrintfEX((pLOGERROR,pLOGALWAYS,"Did not receive command complete for HCI_Reset - aborting bootup\n"));
        pShutdown();
        prh_host_startup_complete=BT_HCITERROR;
        return BT_HCITERROR;
    }
#endif

    status=host_layer_ctrl(PRH_HOST_STARTUP);
    if (status) {        
#if pDEBUG
        t_api status2;
        pDebugPrintfEX((pLOGNOTICE,pLOGALWAYS,"Got error %d from layer startup.\n",status));
        status2=host_layer_ctrl(PRH_HOST_SHUTDOWN);
        if (status2) {
            pDebugPrintfEX((pLOGNOTICE,pLOGALWAYS,"Got error %d from layer shutdown during failed startup cleanup.\n",status2));
        }
#else
        host_layer_ctrl(PRH_HOST_SHUTDOWN);
#endif        
#if HCI_PRESENT
        HCI_Transport_Shutdown();
#endif
        pShutdown();
        prh_host_startup_complete=status;
        return status;
    }

#if SCHEDULER_PRESENT
    prh_scheduler_terminate_flag=0;
    prh_scheduler_sleep_eventvariable=pEventCreate(0); /* this may be used by the HCI when it starts */
#endif


    status=0;
    status |= HCI_ReadBDAddress(); /* event trapped by the manager */
    status |= HCI_ReadLocalVersionInformation(); /* event trapped by the HCI & manager */
    status |= HCI_ReadLocalSupportedFeatures(); /* event trapped by the manager */
	status |= HCI_ReadScanEnable(); /* event trapped by the manager */
#if PRH_HOST_HC_TO_HOST_FLOW_CONTROL_SUPPORTED    
    /* This command does not actually turn on hc->host flow control - this is done in hci_callbacks.c */
    status |= HCI_HostBufferSize(HCI_HC_TO_HOST_FLOWCTRL_ACL_SIZE,HCI_HC_TO_HOST_FLOWCTRL_SCO_SIZE,HCI_HC_TO_HOST_FLOWCTRL_ACL_NUM,HCI_HC_TO_HOST_FLOWCTRL_SCO_NUM);  
#endif
    status |= HCI_ReadBufferSize(); /* event trapped by the hci - signals init complete */

        //status |= HCI_SetHostControllerToHostFlowControl(TRUE);
#if COMBINED_HOST == 0
        status |= HCI_WriteClassOfDevice(0x5a0204);
#endif

        status |= HCI_WritePageTimeout(0x2000);

    if (status) 
    {
        pDebugPrintfEX((pLOGERROR,pLOGALWAYS,"BT_Init: failure during stack init\n",status));
#if SCHEDULER_PRESENT
        pEventFree(prh_scheduler_sleep_eventvariable);
#endif
        host_layer_ctrl(PRH_HOST_SHUTDOWN);
#if HCI_PRESENT
        HCI_Transport_Shutdown();
#endif
        pShutdown();
        prh_host_startup_complete=BT_UNKNOWNERROR;
        return BT_UNKNOWNERROR;
    }

#if SCHEDULER_PRESENT
    prh_scheduler_terminated=pEventCreate(0);
    prh_bt_execute_lock=pMutexCreate(0);
#endif

    pDebugPrintfEX((pLOGNOTICE,pLOGALWAYS,"BT_Init: stack initialisation complete.\n"));
	pDebugCheck();
#if SCHEDULER_PRESENT || COMBINED_HOST==1
	// start host scheduler
	pEventWait(prh_scheduler_sleep_eventvariable, BT_Main, NULL);
#endif
	BT_Init_Step = 2;
    return BT_NOERROR;
}
#endif

t_api send_raw_reset(void) {
    /* scheduler must not be running before this is called as HCI_Transport_PDU_Tx() may not be reentrant */
    struct host_buf *reset_command;
    t_api status;
    reset_command=host_buf_alloc(10);
    host_buf_reserve_header(reset_command,HEADERS_FOR_HCITRANSPORT);
    pMemcpy(reset_command->data,"\x03\x0C\x00",3); 
    reset_command->len=3;
    status=HCI_Transport_PDU_Tx(reset_command, HCI_pduCOMMAND);
	
#if (SINGLE_THREAD_FLAG==0)  //Zhu Jianguo added it. 2008.2.1
    if(status) 
        HCI_Transport_Free_Buffer(reset_command,HCI_pduCOMMAND);
#endif
    return status;
}


/*************************************************************************************************
 * BT_Main
 *
 * This function contains the system (host or combined stack) scheduler which should be run from
 * the bluetooth 'task' in the system. This function does not return until the stack is shut
 * down via BT_Terminate. On systems where no system task is present (i.e. BT_Execute is polled
 * from an application) this function is unused. This scheduler is sleeping until awoken by 
 * prh_system_scheduler_execute or terminated by prh_system_scheduler_terminate (which is called 
 * from BT_Terminate). On systems where no system task is present (i.e. BT_Execute is polled from 
 * an application) this function is unused.
 ************************************************************************************************/
u_int8 max_round_item=2;


#if (SINGLE_THREAD_FLAG==0)  //Zhu Jianguo added it. 2008.2.1
APIDECL1 t_api APIDECL2 BT_Main(void * reservedFlags) {

#if SCHEDULER_PRESENT
    t_api status;

    pDebugPrintfEX((pLOGDEBUG,pLOGALWAYS,"BT_Main called\n"));

    for (;;) {
        pEventWait(prh_scheduler_sleep_eventvariable,NULL,NULL);
        if (prh_scheduler_terminate_flag)
            break;
        do {
            //status=prh_bt_execute(2);  /* this value is not critical */  
            status=prh_bt_execute(max_round_item); 
            pDebugPrintfEX((pLOGDEBUG,pLOGALWAYS,"scheduler called BT_Execute, status = %s\n",status==BT_RETRY?"BT_RETRY":"BT_NOERROR"));    
        } while (status==BT_RETRY);
    };

    pEventSet(prh_scheduler_terminated); 

    return BT_NOERROR;
#else
    return BT_UNSUPPORTED;
#endif
}
#else
APIDECL1 t_api APIDECL2 BT_Main(void *reservedFlags) {

#if SCHEDULER_PRESENT
    t_api status;

    pDebugPrintfEX((pLOGDEBUG,pLOGALWAYS,"BT_Main called\n"));

     do {
          //status=prh_bt_execute(2);  /* this value is not critical */  
          status=prh_bt_execute(max_round_item); 
          pDebugPrintfEX((pLOGDEBUG,pLOGALWAYS,"scheduler called BT_Execute, status = %s\n",status==BT_RETRY?"BT_RETRY":"BT_NOERROR"));    
    } while (status==BT_RETRY);

    if (prh_scheduler_terminate_flag)
       pEventSet(prh_scheduler_terminated); 
   else
       pEventWait(prh_scheduler_sleep_eventvariable, BT_Main, reservedFlags);

    return BT_NOERROR;
#else
    return BT_UNSUPPORTED;
#endif
}

#endif

/*************************************************************************************************
 * prh_system_scheduler_execute
 *
 * This function is called from queuing code in the flowcontrol handler and other functionality
 * which requires the scheduler to run. This function causes the scheduler loop 
 * (prh_system_scheduler_entry), to execute one iteration. On systems where no system task is 
 * present (i.e. BT_Execute is polled from an application) this function is unused.
 ************************************************************************************************/
extern void rdabt_send_notify(void);
void prh_system_scheduler_execute(void) {
       
    pDebugPrintfEX((pLOGDEBUG,pLOGALWAYS,"prh_system_scheduler_execute called - waking up scheduler...\n"));    
#if SCHEDULER_PRESENT || COMBINED_HOST==1
    pEventSet(prh_scheduler_sleep_eventvariable);
#else
	rdabt_send_notify();   
#endif
}


/*************************************************************************************************
 * BT_Terminate
 *
 * This function causes the system (host or combined stack) scheduler to terminate, causing a 
 * return from a previous BT_Main function. On systems where no system task is present 
 * (i.e. BT_Execute is polled from an application) this function is still used to allow
 * the release of stack resources before termination.
 * The flags parameter is reserved.
 ************************************************************************************************/
APIDECL1 t_api APIDECL2 BT_Terminate(u_int32 flags) {
        
    pDebugPrintfEX((pLOGNOTICE,pLOGALWAYS,"BT_Terminate called\n"));
	pDebugCheck();

    if (!(flags&PRH_HOST_NOHCRESET)) {
		prh_hci_expect_reset=1;
        HCI_Reset(); /* command flow control might delay this, but nothing we can do at this stage... */
        prh_system_scheduler_execute(); 
    }

#if SCHEDULER_PRESENT || COMBINED_HOST==1
	pMutexLock(prh_bt_execute_lock); /* stop the scheduler */
#endif

    HCI_Transport_Set_Rx_CB(NULL);

#if SCHEDULER_PRESENT
    prh_scheduler_terminate_flag=1;
    prh_system_scheduler_execute();
#endif

    host_layer_ctrl(PRH_HOST_SHUTDOWN);     

#if HCI_PRESENT
    HCI_Transport_Shutdown(); 
#endif

    pDebugPrintfEX((pLOGNOTICE,pLOGALWAYS,"BT_Terminate: Host stack shutdown complete.\n"));
    
#if SCHEDULER_PRESENT || COMBINED_HOST==1
	pMutexUnlock(prh_bt_execute_lock); /* restart the scheduler to allow the main thread to exit */
#endif

#if SCHEDULER_PRESENT

#if (SINGLE_THREAD_FLAG==0)  //Zhu Jianguo added it. 2008.2.1
// tianwq: no need to wait for single thread
    pEventWait(prh_scheduler_terminated,NULL,NULL);  /* wait for the main thread to exit */
#endif

    pEventFree(prh_scheduler_terminated);  
	pEventFree(prh_scheduler_sleep_eventvariable);
#endif

#if SCHEDULER_PRESENT || COMBINED_HOST==1
	pMutexFree(prh_bt_execute_lock); 
#endif

	pShutdown(); /* shutdown PAPI */

    return BT_NOERROR;
}

/*************************************************************************************************
 * BT_SetLocalDeviceAddress
 *
 * This function sets the bluetooth device address, if possible. This is generally only performed 
 * immediately after device bootup by firmaware and may not be supported on all systems.
 ************************************************************************************************/
t_api BT_SetLocalDeviceAddress(t_bdaddr deviceAddress) {

    /* Modifying the local device address is currently unsupported - combined stack only */

    return BT_UNSUPPORTED;
}

/*************************************************************************************************
 * BT_SetLocalCountryCode
 *
 * This function sets the bluetooth device country code, if possible. This is generally only 
 * performed immediately after device bootup by firmware and may not be supported on all systems.
 ************************************************************************************************/
t_api BT_SetLocalCountryCode(u_int8 countryCode) {

    /* Modifying the device country code is currently unsupported - combined stack only */

    return BT_UNSUPPORTED;
}


/*************************************************************************************************
 * BT_Reset
 *
 * This function resets the host stack and the host controller 
 * HC reset can be overridden by the PRH_RESET_NOHCRESET flag, and this should be used in 
 * configurations where BT_Execute is polled from the application main loop.
 * The bottom 8 bits of the flags parameter may be used for platform-specfic config.
 *   See Host Stack Architecture Document re: BT_Execute
 * If this function fails to reset the stack, the stack will call BT_Terminate.
 ************************************************************************************************/
APIDECL1 t_api APIDECL2 BT_Reset(u_int32 flags) {

    t_api status;
    
    prh_host_reset_required=0;

    HCI_Transport_Set_Rx_CB(NULL); /* disable incoming data path from the HC - HCI layer reset will re-enable this */   

    if (!(flags&PRH_HOST_NOLINKDISCONNECT)) {
       /* prh_l2cap_local_close_all() */ /* should send L2CAP_evDISCONNECTIND to all apps to let them clean up gracefully....*/
    }
        
#if SCHEDULER_PRESENT || COMBINED_HOST==1
    pMutexLock(prh_bt_execute_lock); /* stop the scheduler */
#endif
    
    status=host_layer_ctrl(PRH_HOST_RESET);    
    if (status) {
        pDebugPrintfEX((pLOGNOTICE,pLOGALWAYS,"BT_Reset: got error %d from layer reset\n",status));
        return BT_UNKNOWNERROR;
    }

    if (!(flags&PRH_HOST_NOHCRESET)) {
		prh_hci_expect_reset=1;
        HCI_Reset(); 
    }

    prh_system_scheduler_execute();
#if SCHEDULER_PRESENT || COMBINED_HOST==1
    pMutexUnlock(prh_bt_execute_lock); /* reenable the scheduler */
#endif

#if 0 //PRH_HOST_HC_TO_HOST_FLOW_CONTROL_SUPPORTED==1    
    /* workaround for reverse flow control bug in Parthus HC revision 0x701 */
    if (prh_mgr_ManufacturerId!=PARTHUS_MANUFACTURERID || prh_mgr_HCIrevision!=0x701) {
        extern volatile u_int8 wait_for_flow_control_enable;
        pDebugPrintfEX((pLOGDEBUG,pLOGHCI,"BT_Reset: Enabling HC->Host flow control\n"));                            
        HCI_HostBufferSize(HCI_HC_TO_HOST_FLOWCTRL_ACL_SIZE,HCI_HC_TO_HOST_FLOWCTRL_SCO_SIZE,HCI_HC_TO_HOST_FLOWCTRL_ACL_NUM,HCI_HC_TO_HOST_FLOWCTRL_SCO_NUM);  
        wait_for_flow_control_enable=1;
        HCI_SetHostControllerToHostFlowControl(0x01 /* ACL only */); 
    }
#endif /* PRH_HOST_HC_TO_HOST_FLOW_CONTROL_SUPPORTED */

    return BT_NOERROR;    
}

#if (SINGLE_THREAD_FLAG==1)  //Zhu Jianguo added it. 2008.2.1
#if (PRH_BT_COMBINED==1)
// hci rx event and task
extern t_pEvent hci_transport_combined_event;
int hci_transport_rx_task(void *dummy);

/* host schedule function */
void  BThost_Sched(u_int32 num_interations)
{
	// process timer and event
	papiTimerSched(0);

	// initialize bt host if not
	if(BT_Init_Step == 0)
	{
		BT_Init(NULL, NULL);
		// start hci receive event
		pEventWait(hci_transport_combined_event, hci_transport_rx_task, NULL);
	} else if(BT_Init_Step == 2)
	{
              // app scheduler
		BTapp_Sched(0x01);
	}
}
#endif
#endif


/*************************************************************************************************
 * host_layer_ctrl
 *
 * Passes startup/shutdown commands to the stack layers. 
 * NB: starting/stopping individual layers in isolation is not supported.
 ************************************************************************************************/
extern t_api Avdtp_Layer_Ctrl(u_int8 flags) ;
extern t_api Avctp_Layer_Ctrl(u_int8 flags) ;

t_api host_layer_ctrl(u_int8 layerFlags) {
    t_api status=0;

#if HCI_PRESENT 
   status|=prh_hci_layer_ctrl(layerFlags);
#endif

#if MANAGER_PRESENT
    status|=prh_manager_layer_ctrl(layerFlags);
#endif

#if L2CAP_PRESENT
    status|=prh_l2cap_layer_ctrl(layerFlags);
#endif

#if SDP_CLIENT
    status|=prh_sdp_cli_layer_ctrl(layerFlags);
#endif

#if SDP_SERVER
    status|=prh_sdp_srv_layer_ctrl(layerFlags);
#endif

#if RFCOMM_PRESENT
    status|=prh_rfcomm_layer_ctrl(layerFlags);
#endif

#if A2DP_PRESENT
	status|=Avdtp_Layer_Ctrl(layerFlags);
#endif

#if AVRCP_PRESENT
//#if (AVRCP_PRESENT==1) 
	status|=Avctp_Layer_Ctrl(layerFlags);
#endif

#if OBEX_PRESENT
    status|=prh_obex_layer_ctrl(layerFlags);
#endif
    
    if (status)
        return BT_UNKNOWNERROR;
    else
        return BT_NOERROR;
}


#if pDEBUG
void print_build_options(void) {

    pDebugPrintfEX((pLOGNOTICE,pLOGALWAYS, "\n"));
    pDebugPrintfEX((pLOGNOTICE,pLOGALWAYS, "\n"));
    pDebugPrintfEX((pLOGNOTICE,pLOGALWAYS, "RDA Microellectronics Bluetooth Host Stack, Copyright (c) 2008-2009 RDA inc.\n"));
    pDebugPrintfEX((pLOGNOTICE,pLOGALWAYS, "\n"));
    pDebugPrintfEX((pLOGNOTICE,pLOGALWAYS, "Platform: "PLATFORMSTRING",  "__FILE__" compiled "__DATE__" "__TIME__"\n"));
    pDebugPrintfEX((pLOGNOTICE,pLOGALWAYS, "\n"));
    pDebugPrintfEX((pLOGNOTICE,pLOGALWAYS, "Build Options - \n"));

    /* this line may cause a preprocessor warning if APIDECL1 or APIDECL2 are defined to be empty */
    pDebugPrintfEX((pLOGNOTICE,pLOGALWAYS, "%s\n","\t\t" \
        "APIDECL1: " BT_VALUEOF(APIDECL1) ", APIDECL2: " BT_VALUEOF(APIDECL2)));

    pDebugPrintfEX((pLOGNOTICE,pLOGALWAYS, "%s\n","\t\t" \
	    "pDEBUG: " BT_VALUEOF(pDEBUG) ", BTHOST_BIGENDIAN: " BT_VALUEOF(BTHOST_BIGENDIAN) \
	    ", BT_SPEC_VER: " BT_VALUEOF(BT_SPEC_VER)));

    pDebugPrintfEX((pLOGNOTICE,pLOGALWAYS, "%s\n","\t\t" \
	    "L2CAP_PRESENT: " BT_VALUEOF(L2CAP_PRESENT)  \
	    "  , pDEBUGLEVEL: " BT_VALUEOF(pDEBUGLEVEL) ));

    pDebugPrintfEX((pLOGNOTICE,pLOGALWAYS, "%s\n","\t\t" \
	    "HOST_SECURITY: " BT_VALUEOF(HOST_SECURITY) ", MAX_OUTSTANDING_CALLBACKS: " BT_VALUEOF(MAX_OUTSTANDING_CALLBACKS)));

    pDebugPrintfEX((pLOGNOTICE,pLOGALWAYS, "%s\n","\t\t" \
	    "HCI_OUTGOING_SCO_FLOWCONTROL_SUPPORTED: " BT_VALUEOF(HCI_OUTGOING_SCO_FLOWCONTROL_SUPPORTED) ", " \
	    "STACK_MAX_OUTGOING_QUEUED_HCI_COMMAND_BYTES: " BT_VALUEOF(STACK_MAX_OUTGOING_QUEUED_HCI_COMMAND_BYTES)));

  pDebugPrintfEX((pLOGNOTICE,pLOGALWAYS, "%s\n","\t\t" \
	    "STACK_MAX_OUTGOING_QUEUED_ACL_BYTES_PER_HANDLE: " BT_VALUEOF(STACK_MAX_OUTGOING_QUEUED_ACL_BYTES_PER_HANDLE) ", " \
	    "STACK_MAX_OUTGOING_QUEUED_SCO_BYTES_PER_HANDLE: " BT_VALUEOF(STACK_MAX_OUTGOING_QUEUED_SCO_BYTES_PER_HANDLE)));	     

    pDebugPrintfEX((pLOGNOTICE,pLOGALWAYS, "\n"));
    pDebugPrintfEX((pLOGNOTICE,pLOGALWAYS, "\n"));

}
#endif /* pDEBUG */
