/***************************************************************************** 
* Include
*****************************************************************************/
/* basic system service headers */
#include "stdlib.h"
#include "kal_release.h"      	/* basic data type */
#include "stack_common.h"          /* message and module ids */
#include "stack_msgs.h"            /* enum for message ids */
#include "app_ltlcom.h"       	/* task message communiction */
//#include "syscomp_config.h"         /* type of system module components */
//#include "task_config.h"      	/* task creation */
#include "stacklib.h"        	   /* basic type for dll, evshed, stacktimer */
#include "event_shed.h"       	/* event scheduler */
#include "stack_timer.h"      	/* stack timer */
//#include "stdio.h"                  /* basic c i/o functions */
#include "gpio_sw.h"               /* for gpio */
#include "uart_sw.h" 
#include "nvram_struct.h"           /* for nvram read/write */
#include "nvram_user_defs.h"
#include "nvramtype.h"
#include "nvramprot.h"
#include "bluetooth_bm_struct.h"
#include "bluetooth_struct.h"

#include "bt_a2dp_struct.h"
#include "rda_bt_hfg.h"
#include "rda_bt_avrcp.h"
#include "bt_dm_struct.h"
#include "bt_hfg_struct.h"
#include "bt_avrcp_struct.h"

#include "bt.h"
#include "rdabt.h"
#include "rdabt_msg.h"
#include "rdabt_opp.h"


//#include "rda5868.h"
#include "rdabt_config.h"
#include "rdabt_task.h"
#include "rdabt_profile.h"
#include "rdabt_link_prim.h"
#include "rdabt_callback.h"

#include "mmi_trace.h"

#include "bt.h"
//#include "host_types.h"
#include "papi.h"
#include "sxr_mem.h"
#include "sxr_tim.h"
#include "sxr_tls.h"
#include "adp_events.h"
#include "device_drv.h"
//extern BOOL BT_INIT_OK;
//extern VOID BT_TransRecv_init(VOID);
#ifdef __BT_HFG_PROFILE__
#include "rdabt_hfp.h"
#include "rdabt_hsp.h"
#endif
#ifdef  __BT_OPP_PROFILE__
#include "rdabt_opp.h"
#endif
#ifdef  __BT_FTS_PROFILE__
#endif
#ifdef  __BT_FTC_PROFILE__
#endif
#ifdef  __BT_A2DP_PROFILE__
#include "rdabt_a2dp.h"
#endif
#ifdef  __BT_AVRCP_PROFILE__
#include "rdabt_avrcp.h"
#endif
#include "rdabt_uart.h"
#include "bt_ctrl.h"
#include "hal_sys.h"
#include "hal_timers.h"
typedef UINT32 MsgBody_t;
#include "itf_msg.h"



rdabt_context_struct rdabt_cntx;
rdabt_context_struct* rdabt_p = &rdabt_cntx;
extern const unsigned char bt_gpio_setting[16]; 
extern kal_uint16 rdabt_btNap;
extern kal_uint16 rdabt_btUap;
extern kal_uint32 rdabt_btLap;
const kal_uint8 rdabt_wakeup_data[] = {0x01,0xc0,0xfc,0x00};
static const kal_uint8 rdabt_change_baudrate_921600[] = {0x01,0x34,0xfc,0x04,0x00,0x10,0x0e,0x00};//921600
static const kal_uint8 rdabt_change_baudrate_460800[] = {0x01,0x34,0xfc,0x04,0x00,0x08,0x07,0x00};//460800
static const kal_uint8 rdabt_change_baudrate_230400[] = {0x01,0x34,0xfc,0x04,0x00,0x84,0x03,0x00};//230400
 u_int8 Bt_init_Finished =0;
 

//static t_bdaddr temp_Addr;
extern scheduler_identifier pin_code_ind_timer;
extern kal_uint32 rand_num_seed;
static const kal_uint8 rdabt_nable_sleep[][14] =
{
     {0x01,0x02,0xFD,0x0A,0x00,0x01,0x38,0x00,0x00,0x10,0x00,0x40,0x05,0x0F},//pskey rf delay
     
     {0x01,0x02,0xFD,0x0A,0x00,0x01,0x3C,0x00,0x00,0x10,0x30,0xBF,0xA0,0xA0},//pskey rf timing
     {0x01,0x02,0xFD,0x0A,0x00,0x01,0x34,0x00,0x00,0x10,0x20,0x50,0x02,0xa0},//pskey sleep enable
     
     {0x01,0x02,0xFD,0x0A,0x00,0x01,0x00,0x00,0x00,0x10,0x00,0xe0,0x00,0x00}//pskey flag

};
static const kal_uint8 rdabt_nable_sleep_FF[1]={0xff};
extern int 	bonding_role; 	
extern char *  deviceName;
extern t_bdaddr bonding_addr; 		//7-12-2009 , add by tan
 int 	g_PairingFinished; 	//7-12-2009 , add by tan
 extern int 	responsePinCode;	//7-13-2009, add by tan 
 
 extern void rdabt_accept_conn_cb( t_bdaddr address);

extern u_int8  nameLen;
u_int8  rdabt_send_notify_pending = 0;
volatile int8  rdabt_recive_msg_count=0;
bt_bm_bonding_req_struct pin_code_sdap;
bth_dev_info_array  g_paired_devInfo;

void rdabt_send_ready_ind(void);
void papiSched(unsigned int dummy);
void rdabt_SDP_serviceSearch(bt_bm_service_search_req_struct * msg_p);
extern void rdabt_GetRemoteName_cb(t_bdaddr address, int status);
extern APIDECL1 int APIDECL2 MGR_GetDeviceNumFound(void);

extern kal_bool rdabt_cancel_timed_event(scheduler_identifier event_id/*, kal_uint16 *pmi, void **pmv*/);
extern scheduler_identifier rdabt_timed_event_in(kal_uint32 duration, rdabt_timer_func fn, /*uint16_t mi,*/ void *mv);
#ifdef RDA_BT_DEBUG
extern void Debug_BT_Sap(BOOL dir,module_type src_mod,UINT32 msgid);
#endif

extern int rdabt_poweron_init(void);
extern void rdabt_baudrate_ctrl(void);
void rdabt_wait_bonding_MoniterTimer_cb(void);
extern int MGR_WriteLocalDeviceAddress(u_int8 *address, void (*callbakFunc)(int status));
extern int MGR_ChangeLocalName(u_int8 *name);
extern int MGR_RegisterAcceptCnnHandle(void (*AcceptCnnCallBack)(t_bdaddr address));
extern int MGR_InquiryCancel(void (*InquiryCancelCallBackfun)(int num, int  status));
extern int MGR_AcceptConnRequest(t_bdaddr address, int result);
extern u_int32 get_class_of_device(t_bdaddr address);


extern scheduler_identifier wait_bonding_timer;


u_int8 linkKey[0x11];

u_int8 * print_link_key(u_int8 *pLinkKey )			//add by tan only for test
{	
	int i = 0;

	memset( linkKey, 0, 0x11);
	
	if( pLinkKey)
	{
		for(; i<0x10; i++)
		{
			linkKey[i] = pLinkKey[i] %10+48;
		}
	}
	return  linkKey;
	
}

//03-19-2009,  add by tan
void bth_read_paired_dev_info(bth_dev_info_array *pDevInfo)
{
	if(pDevInfo)
		memcpy(pDevInfo, &g_paired_devInfo, sizeof(bth_dev_info_array));
	
}

//03-19-2009 , add by tan
int bth_write_paired_device_info(bth_dev_struct * pDevInfo)
{
	int i = 0;
	if(!pDevInfo)
		return 1;
	//traverse through the device array
	for( ; i<g_paired_devInfo.devNum; i++)
	{
		if( g_paired_devInfo.devInfo_Array[i].bd_addr.lap == pDevInfo->bd_addr.lap
			&& g_paired_devInfo.devInfo_Array[i].bd_addr.nap == pDevInfo->bd_addr.nap
			&& g_paired_devInfo.devInfo_Array[i].bd_addr.uap == pDevInfo->bd_addr.uap)
			break;
	}

	//if the device's info exgist, replace it, else add a new
	if(i<10)
	{
		memcpy(&g_paired_devInfo.devInfo_Array[i],  pDevInfo, sizeof(bth_dev_struct));

		if( i==g_paired_devInfo.devNum)
			g_paired_devInfo.devNum++;

		//bth_write_nvram();
	}
	return 0;
}

//03-19-2009 , add by tan
//flag equals to 0, delete the specified device info, or else delete all..
int bth_del_paired_device_info(btbm_bd_addr_t bd_addr,  int flag)
{
	int i = 0;
	if(flag != 0)
	{
		g_paired_devInfo.devNum = 0;
		memset(&g_paired_devInfo, 0, sizeof(bth_dev_info_array));
	}
	else
	{
		for(; i<g_paired_devInfo.devNum; i++)
		{
			if( 	g_paired_devInfo.devInfo_Array[i].bd_addr.lap == bd_addr.lap
		   		&& g_paired_devInfo.devInfo_Array[i].bd_addr.uap== bd_addr.uap
		   		&& g_paired_devInfo.devInfo_Array[i].bd_addr.nap== bd_addr.nap)
			{
			 	memmove(&g_paired_devInfo.devInfo_Array[i], &g_paired_devInfo.devInfo_Array[i+1], sizeof(bth_dev_struct)*(g_paired_devInfo.devNum-i-1));
				g_paired_devInfo.devNum--;
				memset(&g_paired_devInfo.devInfo_Array[g_paired_devInfo.devNum], 0, sizeof(bth_dev_struct));
				break;
			}
		}
	}
	//bth_write_nvram();
	
	return 0;
}

//03-22-2009 , add by tan
//will be called when local device deliver the service search request...
void bth_get_specified_device_class(btbm_bd_addr_t bd_addr, u_int32 * classOfdevice)
{
	int i = 0; 
	
	*classOfdevice = 0;
	
	for(; i<g_paired_devInfo.devNum; i++)
	{
		if( 	g_paired_devInfo.devInfo_Array[i].bd_addr.lap== bd_addr.lap
		   	&& g_paired_devInfo.devInfo_Array[i].bd_addr.uap== bd_addr.uap
		   	&&g_paired_devInfo.devInfo_Array[i].bd_addr.nap== bd_addr.nap)
		{
		   	*classOfdevice = g_paired_devInfo.devInfo_Array[i].cod;
			break;
		}
	}
}

//03-25-2009, add by tan
//
int bth_is_the_specified_device_paired(btbm_bd_addr_t bd_addr)
{
	int i = 0;
	for(; i<g_paired_devInfo.devNum; i++)
	{
		if( 	g_paired_devInfo.devInfo_Array[i].bd_addr.lap == bd_addr.lap
		   	&& g_paired_devInfo.devInfo_Array[i].bd_addr.uap== bd_addr.uap
		   	&& g_paired_devInfo.devInfo_Array[i].bd_addr.nap== bd_addr.nap)
		{
			return 1;
		}
	}
	return 0;
}


int bth_get_paired_dev_num()
{
	return g_paired_devInfo.devNum;
}



// add by tan, 
// this function used to load all the paired devices' info, 
void load_paired_device_info()
{
	int j= 0;//, status = 0;

	t_bdaddr 	 	address;

//	btbm_bd_addr_t 	bd_addr;

	bth_dev_info_array 	devArray;

	memset(&devArray, 0, sizeof(bth_dev_info_array));
	
	bth_read_paired_dev_info(&devArray);

	kal_prompt_trace(1,"devArray.devNum:%d\n", devArray.devNum);

   	for( j= 0; j<devArray.devNum; j++)
   	{

            BDADDR_Set_LAP(&address,devArray.devInfo_Array[j].bd_addr.lap);
            BDADDR_Set_UAP(&address,devArray.devInfo_Array[j].bd_addr.uap);
            BDADDR_Set_NAP(&address,devArray.devInfo_Array[j].bd_addr.nap);
	                
		kal_prompt_trace(1,"address: %x %x %x, link_key:%s\n", devArray.devInfo_Array[j].bd_addr.nap,
                                                                                                        devArray.devInfo_Array[j].bd_addr.uap,                                                                                                     
                                                                                                    devArray.devInfo_Array[j].bd_addr.lap,
                                                                                                    print_link_key(devArray.devInfo_Array[j].linkKey));
            
            MGR_SetDeviceInfo(address, devArray.devInfo_Array[j].linkKey, devArray.devInfo_Array[j].KeyType);
	}
			
	kal_prompt_trace(1,"write remote device's info succeed %d\n",devArray.devNum);
	
}

static INT32 check_count;
extern void CheckIsNeedToSleepBT(void);
extern void Uart1_GetStatus();
extern         pBOOL isInCall(void);
static BOOL IsInLpMode;
BOOL  IsFlowCtrlStart;/*added by laixf for Lps to remember flow control*/



kal_uint8 rdabt_get_rxcount(void)
{
	UINT32  rda5868_rxcountaddrs = 0;
       UINT32 rxcountaddrs = 0x50000008;

    I2C_Open();

	rdabt_iic_core_read_data(rxcountaddrs,&rda5868_rxcountaddrs,1);
	//mmi_trace(1,"0x50000008 =0x%x ",rda5868_rxcountaddrs);

       rxcountaddrs = 0x50000010;

	rdabt_iic_core_read_data(rxcountaddrs,&rda5868_rxcountaddrs,1);
	//mmi_trace(1,"0x50000010 =0x%x ",rda5868_rxcountaddrs);

       rxcountaddrs = 0x50000024;

	rdabt_iic_core_read_data(rxcountaddrs,&rda5868_rxcountaddrs,1);
	//mmi_trace(1,"0x50000024 =0x%x ",rda5868_rxcountaddrs);
			
       rxcountaddrs = 0x50000020;

	rdabt_iic_core_read_data(rxcountaddrs,&rda5868_rxcountaddrs,1);
	//mmi_trace(1,"rda5868_rxcountaddrs 0x50000020 =0x%x ",rda5868_rxcountaddrs);
       rda5868_rxcountaddrs = ((rda5868_rxcountaddrs & 0x2fe00) >>9);

    I2C_Close();
    
	return rda5868_rxcountaddrs;
}


VOID rdabt_EnterLpMode(VOID)
{
	if (!IsInLpMode)
	{
		mmi_trace(1,"rdabt_EnterLpMode-----------at %d----------cts=%d--------------uart_rx_nm =%d----\n",hal_TimGetUpTime(),hal_UartGetCts(HAL_UART_1),rdabt_get_rxcount());
		Uart1_GetStatus();
    		sxr_StopFunctionTimer(CheckIsNeedToSleepBT);
		//hal_GpioClr(g_btdConfig->pinWakeUp);
		hal_UartClose(HAL_UART_1);
		//bt_drv_status.chip_uart=FALSE;
		IsInLpMode = 1;
		mmi_trace(1,"rdabt_EnterLpMode-----------at %d----------cts=%d--------------uart_rx_nm =%d----\n",hal_TimGetUpTime(),hal_UartGetCts(HAL_UART_1),rdabt_get_rxcount());
	}
}
#if 1
extern volatile u_int8 bt_execute_in_process;
extern u_int8 need_send_fc;
void rdabt_bt_restart();

void CheckIsNeedToSleepBT(void)
{
	TASK_HANDLE * hTask = (TASK_HANDLE *)GetMmiTaskHandle(MOD_BT);
	//mmi_trace(1,"CheckIsNeedToSleepBT----bt_execute_in_process=%d---need_send_fc=%d----at %d----------cts=%d---------------uart_rx_nm =%d----\n",bt_execute_in_process,need_send_fc,hal_TimGetUpTime(),hal_UartGetCts(HAL_UART_1),rdabt_get_rxcount());
	Uart1_GetStatus();
	if ((check_count==15)/*&& (!isInCall())*/)
	{
		if (!bt_execute_in_process)
		{
			rdabt_EnterLpMode();
			return ;
		}
		else
			check_count=10;
	}	
	if(!sxr_SbxHot(hTask->nMailBoxId))
	{
		check_count++;
	}
	
	if (hal_UartGetCts(HAL_UART_1) )
	{
	
	  mmi_trace(1,"CheckIsNeedToSleepBT----uart_rx_nm =%d---\n", rdabt_get_rxcount());	

 #if DEBUG
	   sxr_Sleep(100);  
           asm("break 1");   
#endif		   
	
	}
	
    	sxr_StartFunctionTimer(16384, CheckIsNeedToSleepBT, 0, 0);
}
#else
extern void mmi_bt_power_off(void);
void CheckIsNeedToSleepBT(void)
{
	TASK_HANDLE * hTask = (TASK_HANDLE *)GetMmiTaskHandle(MOD_BT);
	mmi_trace(1,"CheckIsNeedToSleepBT-----------at %d----------cts=%d---\n",hal_TimGetUpTime(),hal_UartGetCts(HAL_UART_1));
	if ((check_count==15)/*&& (!isInCall())*/)
	{
		rdabt_EnterLpMode();
//		return ;
	}	
	if ((check_count==120)/*&& (!isInCall())*/)
	{
		mmi_trace(1,"CheckIsNeedToSleepBT-----------entry power off---\n");
		      rdabt_send_msg_up(MOD_MMI, MSG_ID_BT_POWEROFF_IND, NULL, NULL);

//		 mmi_bt_power_off();
		return;
	}	
	
	if(!sxr_SbxHot(hTask->nMailBoxId))
	{
		check_count++;
	}
    		sxr_StartFunctionTimer(16384, CheckIsNeedToSleepBT, 0, 0);
}

#endif


VOID rdabt_ExitLpMode(VOID)
{
	UINT16 bytesWritten;
	if (IsInLpMode)
	{
	/*
		HAL_UART_CFG_T uartCfg;
		HAL_UART_IRQ_STATUS_T mask = 
		{        
			.txModemStatus   = 0,        
			.rxDataAvailable = 0,        
			.txDataNeeded    = 0,        
			.rxTimeout       = 0,        
			.rxLineErr       = 0,        
			.txDmaDone       = 0,        
			.rxDmaDone       = 1,        
			.rxDmaTimeout    = 0    
		};	
		uartCfg.afc = HAL_UART_AFC_MODE_RX_TRIG_16;	
		uartCfg.data = HAL_UART_8_DATA_BITS;	
		uartCfg.irda = HAL_UART_IRDA_MODE_DISABLE;	
		uartCfg.parity = HAL_UART_NO_PARITY;	
		uartCfg.rate = HAL_UART_BAUD_RATE_921600;	
		uartCfg.rx_mode = HAL_UART_TRANSFERT_MODE_DMA_IRQ;	 
		uartCfg.rx_trigger = HAL_UART_RX_TRIG_QUARTER; 	
		uartCfg.stop = HAL_UART_1_STOP_BIT;	
		uartCfg.tx_mode = HAL_UART_TRANSFERT_MODE_DMA_POLLING;	
		uartCfg.tx_trigger = HAL_UART_TX_TRIG_EMPTY;	
		//hal_GpioSet(g_btdConfig->pinWakeUp);
		hal_UartOpen(HAL_UART_1, &uartCfg);	
		hal_UartFifoFlush(HAL_UART_1);	
		hal_UartClearErrorStatus(HAL_UART_1);    	 
		hal_UartIrqSetMask(HAL_UART_1,mask);    	
		hal_UartIrqSetHandler(HAL_UART_1,UartIrqHandler);	
		hal_UartGetData(HAL_UART_1,rx_one,UART_RECBUF_SIZE);	
		hal_SysInvalidateCache(rx_one,sizeof(rx_one));	
		hal_UartSetRts(HAL_UART_1,1);	
		buf_first = TRUE;	
		*/
		mmi_trace(1,"rdabt_ExitLpMode------------at %d------cts=%d-----uart_rx_nm =%d----\n",hal_TimGetUpTime(),hal_UartGetCts(HAL_UART_1),rdabt_get_rxcount());
		sxr_StopFunctionTimer(CheckIsNeedToSleepBT);
		rda5868_uart_configure(921600/*460800*//*230400*/,IsFlowCtrlStart);
		sxr_Sleep(210);
		rda5868_uart_tx((u_int8 *)rdabt_wakeup_data,sizeof(rdabt_wakeup_data),&bytesWritten);
		sxr_Sleep(210);
    		sxr_StartFunctionTimer(16384, CheckIsNeedToSleepBT, 0, 0);
		mmi_trace(1,"rdabt_ExitLpMode 2------------at %d------cts=%d-----uart_rx_nm =%d----\n",hal_TimGetUpTime(),hal_UartGetCts(HAL_UART_1),rdabt_get_rxcount());
		//bt_drv_status.chip_uart=TRUE;
		IsInLpMode=0;
	}
}




scheduler_identifier tmp_timer_id;
extern DEVICE Uart_Dev[4];
#include "btd_config.h"
#include "tgt_btd_cfg.h"
#include "pmd_m.h"
#include "hal_gpio.h"
CONST TGT_BTD_CONFIG_T* g_btdConfig;
void rdabt_antenna_off(void);
VOID pal_EnableAuxClkCtrl(BOOL on);
extern t_pTimer uart_timer ;

void   rdabt_antenna_on_retry()
{
	mmi_trace(g_sw_BT," rdabt_antenna_on fail !!!!! \n");
	mmi_trace(g_sw_BT,"  rdabt_antenna_on fail !!!!! \n");

	   hal_GpioClr(g_btdConfig->pinReset);
 //  hal_GpioClr(g_btdConfig->pinSleep);
   pmd_EnablePower(PMD_POWER_BT, FALSE);

   rda5868_uart_stop();


   /* reset global context */
   rdabt_p->seq_num = 0;
   rdabt_p->num_int_msg = 0;
   rdabt_p->rx_cursor = 0;
   rdabt_p->rx_length = 0;
   rdabt_p->rx_length = 0;
   //rdabt_p->curr_queue = 0;
   rdabt_p->sent_rx_ind = KAL_FALSE;
   rdabt_p->sent_tx_ind = KAL_FALSE;

   rdabt_p->host_wake_up = KAL_FALSE;
   rdabt_p->chip_knocked = KAL_FALSE;

  // rdabt_mem_deinit();

   rdabt_p->state = RDABT_STATE_POWER_OFF;
   memset(rdabt_p,0,sizeof(rdabt_cntx));
   	rdabt_send_notify_pending=0;//zhou siyou add it. 20090701
   Bt_init_Finished=0;
   if(uart_timer)
   	{	pTimerCancel(uart_timer);
  		 uart_timer=0;
   	}
   if(tmp_timer_id)
   	{
   	rdabt_cancel_timed_event(tmp_timer_id);
   	tmp_timer_id=0;
   	}
	sxr_Sleep(300);/*10 ms*/
	rdabt_antenna_on();

}
int  rdabt_antenna_on()
{
    kal_uint16 num_send;

    mmi_trace(g_sw_BT,"Enter function  rdabt_antenna_on \n");

    pal_EnableAuxClkCtrl(TRUE);
    // Enable 26M AUX clock
    hal_SysAuxClkOut(TRUE);

    hal_GpioClr(g_btdConfig->pinReset);
    pmd_EnablePower(PMD_POWER_BT, TRUE);
    sxr_Sleep(168*50);/*10 ms*/
    hal_GpioSet(g_btdConfig->pinReset);

//    sxr_Sleep(700);/*10 ms*/
//every time power on , try stop uart first
    rda5868_uart_stop();
    rda5868_uart_start();
    I2C_Open();
    rda5868_uart_configure(115200,FALSE);
    GPIO_INT_Registration(g_btdConfig->pinSleep,0,rdabt_uart_eint_hdlr);
    //rdabt_uart_register_eint();
    RDA5868_uart_register_cb(); 

    if( rdabt_poweron_init() == -1 )//for pskey   zhou siyou add it .20090701 
    {
        hal_GpioClr(g_btdConfig->pinReset);
        hal_GpioClr(g_btdConfig->pinSleep);
        pmd_EnablePower(PMD_POWER_BT, FALSE);
        rda5868_uart_stop();
        I2C_Close();
        // Disable 26M AUX clock
        hal_SysAuxClkOut(FALSE);
        pal_EnableAuxClkCtrl(FALSE);
        return -1;
    }

    //sxr_Sleep(1000); 
    BT_Init(NULL,NULL); 
    sxr_Sleep(100);  
    rda5868_uart_tx(rdabt_nable_sleep_FF,sizeof(rdabt_nable_sleep_FF),&num_send);
    rdabt_send_notify_pending=0;//zhou siyou add it. 20090701
    I2C_Close();
    // Disable 26M AUX clock
    hal_SysAuxClkOut(FALSE);

    rdabt_baudrate_ctrl();

    tmp_timer_id = rdabt_timed_event_in((1000)*1000,(rdabt_timer_func) rdabt_antenna_on_retry, /*0,*/ NULL);
    CheckIsNeedToSleepBT();
    return 0;
}


void rdabt_antenna_off(void)
{
//   ilm_struct msg;
//   kal_uint32 msg_count;
    mmi_trace(g_sw_BT,"Enter function  rdabt_antenna_off \n");

    BT_Terminate(0);
    sxr_Sleep(160);

    hal_GpioClr(g_btdConfig->pinReset);
    //  hal_GpioClr(g_btdConfig->pinSleep);
    pmd_EnablePower(PMD_POWER_BT, FALSE);
    rda5868_uart_stop();
    pal_EnableAuxClkCtrl(FALSE);

    /* reset global context */
    rdabt_p->seq_num = 0;
    rdabt_p->num_int_msg = 0;
    rdabt_p->rx_cursor = 0;
    rdabt_p->rx_length = 0;
    rdabt_p->rx_length = 0;
    //rdabt_p->curr_queue = 0;
    rdabt_p->sent_rx_ind = KAL_FALSE;
    rdabt_p->sent_tx_ind = KAL_FALSE;

    rdabt_p->host_wake_up = KAL_FALSE;
    rdabt_p->chip_knocked = KAL_FALSE;

    // rdabt_mem_deinit();

    rdabt_p->state = RDABT_STATE_POWER_OFF;
    memset(rdabt_p,0,sizeof(rdabt_cntx));
    rdabt_send_notify_pending=0;//zhou siyou add it. 20090701
    Bt_init_Finished=0;
    if(uart_timer)
   	{
        pTimerCancel(uart_timer);
        uart_timer=0;
   	}
}



/*****************************************************************************
* FUNCTION
*   rdabt_is_bd_addr_valid
* DESCRIPTION
*   Check if BD Address is valid
* PARAMETERS
*   bd_addr    IN    address to be checked
* RETURNS
*   TRUE if the address is valid; otherwise, FALSE.
* GLOBALS AFFECTED
*   None
*****************************************************************************/
kal_bool rdabt_is_bd_addr_valid(rdabt_bd_addr_struct *bd_addr)
{
   if(bd_addr->LAP == 0 && bd_addr->UAP == 0 && bd_addr->UAP == 0)
      return FALSE;
   else if(bd_addr->LAP>=0x9E8B00 && bd_addr->LAP<= 0x9E8B3F)
      return FALSE;
   else if(bd_addr->LAP>0xFFFFFF)
      return FALSE;
   else
      return TRUE;
}

#ifndef __COOLSAND_TARGET__
U32 rdabt_get_random_number(void)
{
   U32 temp; 
   temp = hal_TimGetUpTime() & 0x003f;
   temp ^= ( (hal_TimGetUpTime() & 0x003f) << ((hal_TimGetUpTime())&0x001f));
   return temp;
}
#endif
/*****************************************************************************
* FUNCTION
*   rdabt_set_default_bd_addr
* DESCRIPTION
*   Write a random BD Address to NVRAM
* PARAMETERS
*   None.
* RETURNS
*   None.
* GLOBALS AFFECTED
*   external_global
*****************************************************************************/
void rdabt_set_default_bd_addr(void)
{
   rdabt_bd_addr_struct bd_addr;
   bd_addr.NAP = rdabt_btNap;
   bd_addr.UAP = rdabt_btUap;
  	UINT16 error = 0;
//   int error=0;
   /* loop until we find one valid LAP by random*/
   #ifdef __COOLSAND_TARGET__
   srand(rand_num_seed);
   #else
       srand(rdabt_get_random_number());  
   #endif
   while(1)
   {
      bd_addr.LAP= rand()&0xFFFFFF;

      /* we use NAP and UAP from CSR as default, so only LAP is required */
      if(rdabt_is_bd_addr_valid(&bd_addr))
         break;
   }
   rdabt_btLap = (kal_uint32 )bd_addr.LAP;

   /* write to NVRAM */
   WriteRecord(NVRAM_EF_RDA_SETTING_LID,1,(void *)&bd_addr,sizeof(bd_addr),&error);

   mmi_trace(1,"rdabt_set_default_bd_addr error= %d\n",error);
}

/*****************************************************************************
* FUNCTION
*   rdabt_get_db_address
* DESCRIPTION
*   Get previous-saved BD address to NVRAM
* PARAMETERS
*   None
* RETURNS
*   None.
* GLOBALS AFFECTED
*   external_global
*****************************************************************************/
void rdabt_get_local_addr_req(kal_msgqid queue_id)
{
    rdabt_bd_addr_struct result_buff;
  	UINT16 error = 0;
    ReadRecord(NVRAM_EF_RDA_SETTING_LID,1,(void *)&result_buff,sizeof(result_buff),&error);
    /* wait until BD address received */
    mmi_trace(1,"rdabt_get_local_addr_req error = %d\n",error);
    if(error != NVRAM_READ_SUCCESS) /* Failed to read in NVRAM */
    {
        /* write RAMDOM value to NVRAM */
        rdabt_set_default_bd_addr();
        //rdabt_write_localaddr_cb(0);
        result_buff.NAP = rdabt_btNap;
        result_buff.UAP = rdabt_btUap;
        result_buff.LAP  =rdabt_btLap;
    }

    {
       rdabt_bd_addr_struct *bd_addr;
 //       kal_uint16 len;
        bd_addr = (rdabt_bd_addr_struct*)&result_buff;//get_pdu_ptr(ext_msg.peer_buff_ptr, &len);

        /* check if BD address is valid */
	      	mmi_trace(g_sw_BT, "bd_addr=%x  %x %x", bd_addr->LAP, bd_addr->UAP, bd_addr->NAP);
        if(rdabt_is_bd_addr_valid(bd_addr))
        {
        	mmi_trace(g_sw_BT, "rdabt_is_bd_addr_valid");
        	 t_bdaddr tempbd;
		rdabt_btNap= (kal_uint16)bd_addr->NAP;
		rdabt_btUap = (kal_uint16)bd_addr->UAP;
		rdabt_btLap = (kal_uint32)bd_addr->LAP;
           
		BDADDR_Set_LAP(&tempbd, bd_addr->LAP);
		BDADDR_Set_UAP(&tempbd, bd_addr->UAP);
		BDADDR_Set_NAP(&tempbd, bd_addr->NAP);
		MGR_WriteLocalDeviceAddress(tempbd.bytes,rdabt_write_localaddr_cb);
        }
        else
        {
           rdabt_set_default_bd_addr();
           rdabt_write_localaddr_cb(0);
        }
    }
	HCI_ReadBDAddress();
}

void rdabt_send_notify(void)
{
   	TASK_HANDLE * hTask = (TASK_HANDLE *)GetMmiTaskHandle(MOD_BT);
	//papiSched(0);
      /* proecess external queue */
	//BT_Execute(1);
	if(!sxr_SbxHot(hTask->nMailBoxId))
	{
      	   //rdabt_send_msg_up(MOD_BT, MSG_ID_BT_NOTIFY_EVM_IND, NULL, NULL);
		   ilm_struct *msg = allocate_ilm(MOD_BT);
#ifdef RDA_BT_DEBUG		   
		   Debug_BT_Sap(1,MOD_BT,MSG_ID_BT_NOTIFY_EVM_IND);
#endif		   
		   msg->src_mod_id = MOD_BT;
		   msg->dest_mod_id = (module_type)MOD_BT;
		   msg->sap_id = BT_APP_SAP;
		   msg->msg_id = MSG_ID_BT_NOTIFY_EVM_IND;
		   msg->local_para_ptr = NULL;
		   msg->peer_buff_ptr = NULL;
		   msg_send_ext_queue(msg);
   //   rdabt_timed_event_in((100)*1000, rdabt_send_notify,NULL);
	  rdabt_send_notify_pending++;
	}
}
/*****************************************************************************
* FUNCTION
*   bchs_send_ready_ind
* DESCRIPTION
*   Send ready indication to MMI and L4C
* PARAMETERS
*   None
* RETURNS
*   None
* GLOBALS AFFECTED
*   external_global
*****************************************************************************/

void rdabt_send_ready_ind(void)
{
	sxr_Sleep(100);/*10 ms*/

   if(rdabt_p->original_boot_state)
   {
      /* send to L4 */
   //bchs_send_msg_up(MOD_L4C, MSG_ID_BT_READY_IND, NULL, NULL);

      /* send to MMI */
      rdabt_send_msg_up(MOD_MMI, MSG_ID_BT_READY_IND, NULL, NULL);
   }
   pTimerCancel(tmp_timer_id);
   tmp_timer_id=0;
  // rdabt_timed_event_in((20)*1000, rdabt_send_notify,NULL);
}




/*****************************************************************************
* FUNCTION
*   rdabt_dummy_handler
* DESCRIPTION
*   Dummy handler of a internal task
* PARAMETERS
*   param   IN    message passed into the internal task
* RETURNS
*   None
* GLOBALS AFFECTED
*   external_global
*****************************************************************************/
void rdabt_dummy_handler(void** param)
{
   /* presently, do nothing */
//	uint16_t eventClass;
//	void *msg;
	
}

/*****************************************************************************
* FUNCTION
*   rdabt_init_internal_task
* DESCRIPTION
*   Initialize all internal tasks
* PARAMETERS
*   None.
* RETURNS
*   None
* GLOBALS AFFECTED
*   external_global
*****************************************************************************/
void rdabt_init_internal_task(void)
{
   kal_int32 i;

   for (i=0; i<RDABT_NUM_TASK; i++)
      rdabt_p->task_list[i].handler_func = rdabt_dummy_handler;


   
   rdabt_p->task_list[LINK_QUEUE].init_func = LINK_INIT;
   rdabt_p->task_list[LINK_QUEUE].handler_func = LINK_TASK;
   rdabt_p->task_list[LINK_QUEUE].deinit_func= LINK_DEINIT;

}

/*****************************************************************************
* FUNCTION
*   rdabt_start_base_timer
* DESCRIPTION
*   Function to stop stack timer
* PARAMETERS
*   base_timer IN stack timer of bt
*  time_out    IN duration of timer
* RETURNS
*   None
* GLOBALS AFFECTED
*   external_global
*****************************************************************************/
void rdabt_start_base_timer(void *base_timer,unsigned int time_out)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
   stack_start_timer((stack_timer_struct *)base_timer, 0, time_out);
}


/*****************************************************************************
* FUNCTION
*   rdabt_stop_base_timer
* DESCRIPTION
*   Function to stop stack timer
* PARAMETERS
*   base_timer IN stack timer of bt
* RETURNS
*   None
* GLOBALS AFFECTED
*   external_global
*****************************************************************************/
void rdabt_stop_base_timer(void *base_timer)
{
	stack_stop_timer((stack_timer_struct *)base_timer);
}


/*****************************************************************************
* FUNCTION
*   rdabt_reset
* DESCRIPTION
*   Reset function if bt task
* PARAMETERS
*   task_index  IN   index of the taks
* RETURNS
*   TRUE if reset successfully
* GLOBALS AFFECTED
*   external_global
*****************************************************************************/
kal_bool rdabt_reset(task_indx_type task_index)
{
   return KAL_TRUE;
}

/*****************************************************************************
* FUNCTION
*   rdabt_write_scan_enable_cb
* DESCRIPTION
*   rdabt_write_scan_enable_cb
* PARAMETERS
*   task_index  IN   index of the taks
* RETURNS
*   TRUE if reset successfully
* GLOBALS AFFECTED
*   external_global
*****************************************************************************/

void rdabt_set_discoverable(kal_uint8 flag)
{
	kal_uint8 mode;
	mode = MGR_NONDISCOVERABLE;
	switch(flag)
	{
		case BTBM_ADP_P_OFF_I_OFF:
			mode = MGR_NONCONNECTABLE | MGR_NONDISCOVERABLE;
			break;
	       case BTBM_ADP_P_OFF_I_ON:
		   	mode = MGR_DISCOVERABLE_BIT;
			break;
	       case BTBM_ADP_P_ON_I_OFF:
		   	mode = MGR_CONNECTABLE_BIT;
			break;
	       case BTBM_ADP_P_ON_I_ON:
		   	mode = MGR_CONNECTABLE_BIT | MGR_DISCOVERABLE_BIT;
			break;
	       case BTBM_ADP_P_ON_I_ON_L:
		   	mode = MGR_LIMDISCOVERABLE_BIT | MGR_DISCOVERABLE_BIT;
			break;			
	}
	kal_prompt_trace(1,"rdabt_set_discoverable ,mode = %d",mode);
	MGR_SetMode(mode,rdabt_write_scanenable_cb);
}



void rdabt_set_SecurityMode(kal_uint8 mode)
{
    int temp = mode?MGR_LINKLEVEL:MGR_NOSECURITY;
    MGR_SetSecurityMode(temp,bt_write_authentication_mode_cb);
}


/*****************************************************************************
* FUNCTION
*   rdabt_init
* DESCRIPTION
*   Init function if bt task
* PARAMETERS
*   task_index  IN   index of the taks
* RETURNS
*   TRUE if reset successfully
* GLOBALS AFFECTED
*   external_global
*****************************************************************************/
kal_bool rdabt_init(task_indx_type task_index)
{
    kal_prompt_trace(1,"rdabt_init");
    /* create semaphore for ADM memory */

    rdabt_p->mem_mutex = kal_create_mutex("RDABT MEM Mutex");

    /* initalize internal tasks */
   rdabt_init_internal_task();

    stack_init_timer (&rdabt_p->base_timer, "RDABT_Timer", MOD_BT);

    rdabt_p->evsh_sched= new_evshed(&rdabt_p->base_timer,
                                                                rdabt_start_base_timer, rdabt_stop_base_timer,
                                                                0 , kal_evshed_get_mem, kal_evshed_free_mem, 0);



   return KAL_TRUE;
}


void forBtSchedule(void)
{
	rdabt_send_notify();
    	sxr_StartFunctionTimer(16384, forBtSchedule, 0, 0);
}

BOOL  reset_in_process=0;
extern volatile u_int8 prh_host_reset_required;
extern int   pincode_sent;

/*****************************************************************************
* FUNCTION
*   rdabt_power_off_sched
* DESCRIPTION
*   Main loop when in power off state
* PARAMETERS
*  queue_id               IN     queue index of bt
* RETURNS
*   None.
* GLOBALS AFFECTED
*   external_global
*****************************************************************************/
void rdabt_power_off_sched(kal_msgqid queue_id)
{
	ilm_struct *msg=NULL;
	COS_EVENT ev;
	TASK_HANDLE * hTask=NULL;
	Msg_t *BtMsg=NULL;
    	hTask = (TASK_HANDLE *)GetMmiTaskHandle(MOD_BT);
//	unsigned int err = 0;
	while(1) /* main root for message processing */
	{
		SUL_ZeroMemory32(&ev, sizeof(COS_EVENT) );
		BtMsg = sxr_Wait((u32 *)&ev, hTask->nMailBoxId);
   		if (BtMsg!= NULL )
   		{
   		
			asm("break 1");
   		}
		else
		{
			if (ev.nEventId == BT_TIMER_PAPI)
			{
	
				asm("break 1");
			}
			else
			{
				msg = (ilm_struct*)ev.nParam1;
				mmi_trace(1,"rdabt_power_off_sched Get a Message ,msg id = %d\n",msg->msg_id);
#ifdef RDA_BT_DEBUG			
			Debug_BT_Sap(0,msg->src_mod_id,msg->msg_id);
#endif			

				switch(msg->msg_id)
				{
					case MSG_ID_BT_POWERON_REQ:
					{
						reset_in_process = 0;
						prh_host_reset_required = 0;
						 bt_bm_PowerON_cnf_struct *cnf_param_ptr;
    						cnf_param_ptr = (bt_bm_PowerON_cnf_struct *)construct_local_para( (kal_uint16)sizeof(bt_bm_PowerON_cnf_struct), TD_CTRL);

						if(rdabt_antenna_on())
						{
							cnf_param_ptr->result = FALSE;
						}
						else 
						{
							cnf_param_ptr->result = TRUE;
						}

						/* ====================== state change ====================== */
						/* send response back to MMI */
						
						rdabt_send_msg_up(MOD_MMI, MSG_ID_BT_POWERON_CNF, cnf_param_ptr, NULL);

						MMI_Free_msg_buf(msg);

						if(cnf_param_ptr->result)
						{
							rdabt_p->state = RDABT_STATE_POWER_ON;
							rdabt_p->original_boot_state = RDABT_STATE_POWER_ON;
						}
						//sxr_StartFunctionTimer(16384, forBtSchedule, 0, 0);
						//rdabt_send_ready_ind();
						return;
					}
					break;
					case MSG_ID_BT_NOTIFY_EVM_IND://zhou siyou added 20090701
					{	//mmi_trace(1,"MSG_ID_BT_NOTIFY_EVM_IND is be processed poweroffshced\n");
						if(rdabt_send_notify_pending)
						rdabt_send_notify_pending--;
    			             	}
					break;
					default :
						mmi_trace(g_sw_BT,"rdabt_power_off_sched recived msg id = %d\n",msg->msg_id);
					break;
				} /* end of switch */
				MMI_Free_msg_buf(msg);
			}
		}
	}  /* end of while(1) */
}

extern void rdabt_goep_init(void) ;
void rdabt_bt_restart()
{
	prh_host_reset_required = 1;
	reset_in_process = 1;
	//rdabt_send_msg_to_head(MOD_BT, MSG_ID_BT_POWEROFF_REQ, NULL, NULL);	
	rdabt_send_notify();
	rdabt_goep_init();
       //rdabt_antenna_off();
       sxr_StopFunctionTimer(CheckIsNeedToSleepBT);
     //sxr_StopFunctionTimer(forBtSchedule);		
        rdabt_send_msg_up(MOD_MMI, MSG_ID_BT_RESET_REQ_IND, NULL, NULL);
	 //COS_Sleep(100);
	//rdabt_send_msg_up(MOD_MMI, MSG_ID_BT_MMI_RESET_CNF, NULL, NULL);
}

void remove_unwanted_msg()
{
   	COS_EVENT evt; 
   	TASK_HANDLE * hTask = (TASK_HANDLE *)GetMmiTaskHandle(MOD_BT);
	evt.nEventId = MSG_ID_UART_READY_TO_READ_IND; 
	evt.nParam1 = (UINT32)0x1f1f1f1f; 
       if (sxr_RmvMsgFromMbx(&evt,1,hTask->nMailBoxId,2))
	   mmi_trace(1,"remove_unwanted_msg is called and some message is removed.....\n");//RDA5868_uart_rx_cb(2);	
}
void rdabt_main_msg_hdler(kal_msgqid queue_id, ilm_struct *message)
{
UINT32 ret;
    switch(message->msg_id)
    {
     case MSG_ID_TIMER_EXPIRY:   /* timer routine */
        {
	   /*
            //bchs_p->curr_queue = 0x1000 | 1; // added by YS
            rdabt_timer_expiry_hdlr((void*)message->local_para_ptr);
          */
          asm("break 1");
            break;
        }
         case MSG_ID_BT_NOTIFY_EVM_IND:
		if(rdabt_send_notify_pending)
			rdabt_send_notify_pending--;
        //papiSched(0);
        //BT_Execute(1);
        break;
        
     case MSG_ID_UART_READY_TO_READ_IND:
           ret = rda5868_uart_rx();
	    rdabt_recive_msg_count--;
          // if (rdabt_recive_msg_count <0)
	   //    mmi_trace(1,"rdabt_recive_msg_count = %d, recived %d bytes\n",rdabt_recive_msg_count,ret);

        break;
        
      case MSG_ID_BT_BM_WRITE_SCANENABLE_MODE_REQ:
        {
            bt_bm_write_scanenable_mode_req_struct *msg_p = (bt_bm_write_scanenable_mode_req_struct *)message->local_para_ptr;
            rdabt_set_discoverable(msg_p->modeconnected);
            break;
        }
        
    case MSG_ID_BT_HOST_WAKE_UP_IND:
        {
            kal_uint16 num_send;
			kal_prompt_trace(0, "MSG_ID_BT_HOST_WAKE_UP_IND\n");
            rda5868_uart_tx(rdabt_wakeup_data,sizeof(rdabt_wakeup_data),&num_send);
            break;
        }
        
   case MSG_ID_BT_BM_READ_LOCAL_ADDR_REQ:
        {
            rdabt_get_local_addr_req(queue_id);	 
            break;		 	
        }    
        
   case MSG_ID_BT_BM_WRITE_LOCAL_NAME_REQ:
        {
            bt_bm_write_local_name_cnf_struct *param_ptr;

            bt_bm_write_local_name_req_struct *msg_p = (bt_bm_write_local_name_req_struct *)message->local_para_ptr;
            param_ptr = (bt_bm_write_local_name_cnf_struct *)construct_local_para(
            (kal_uint16)sizeof(bt_bm_write_local_name_cnf_struct), TD_CTRL);	
            param_ptr->result = 0;
            MGR_ChangeLocalName(msg_p->name);				  
          rdabt_send_msg_up(MOD_MMI, MSG_ID_BT_BM_WRITE_LOCAL_NAME_CNF, (void *)param_ptr, NULL);		 

            //add here for test .......
            load_paired_device_info();			//add by tan

	    MGR_SetLinkFlags(MGR_CONNREQ_ACL_ACCEPT|MGR_KEYREQ_HANDLE|MGR_PINREQ_HANDLE|MGR_CONNREQ_SCO_ACCEPT);		

            MGR_RegisterPINHandler(bt_pin_code_ind_cb);	//it should always handle the request since it is scannable 
	     MGR_RegisterAcceptCnnHandle(rdabt_accept_conn_cb);

            break;		 	
        }   	
        
    case MSG_ID_BT_BM_WRITE_AUTHENTICATION_MODE_REQ:
        {
            bt_bm_write_authentication_mode_req_struct *msg_p = (bt_bm_write_authentication_mode_req_struct *)message->local_para_ptr;
            rdabt_set_SecurityMode(msg_p->mode);
            break;		 	
        }   	
        
      case MSG_ID_BT_BM_LINK_ALLOW_REQ:
        {
        rdabt_send_msg_up(MOD_MMI, MSG_ID_BT_BM_LINK_ALLOW_CNF, NULL, NULL);		 
            break;		 	
        }   	
        
      case MSG_ID_BT_BM_LINK_DISALLOW_REQ:
	  	
	       kal_prompt_trace(1, "Get MSG_ID_BT_BM_LINK_DISALLOW_REQ msg!!!");
	     rdabt_goep_link_disallow();
         rdabt_send_msg_up(MOD_MMI, MSG_ID_BT_BM_LINK_DISALLOW_CNF, NULL, NULL);		 
        break;	
        
      case MSG_ID_BT_POWEROFF_REQ:
	
            rdabt_antenna_off();
	     sxr_StopFunctionTimer(CheckIsNeedToSleepBT);
	     //sxr_StopFunctionTimer(forBtSchedule);		
            rdabt_send_msg_up(MOD_MMI, MSG_ID_BT_POWEROFF_CNF, NULL, NULL);	
        break;
        
     case MSG_ID_BT_BM_DISCOVERY_REQ:
        {
            bt_bm_discovery_req_struct *msg_p = (bt_bm_discovery_req_struct *)message->local_para_ptr;
	     int  status = 0;

           pincode_sent = 0;
            kal_prompt_trace(1, "cod =%x, discovery_mode=%d",msg_p->cod, msg_p->discovery_mode);
           status = MGR_InitDeviceDiscovery(!msg_p->discovery_mode,msg_p->cod,msg_p ->inquiry_length, 0x9E8B33, msg_p->inquiry_number, bt_discovery_result_ind_cb);	// msg_p ->inquiry_length
              kal_prompt_trace(1, "MSG_ID_BT_BM_DISCOVERY_REQ::status=%d",status);
	     if( status != BT_PENDING)
	     {
			bt_discovery_result_ind_cb(NULL, status);
	     }
            break;
        }
        
     case MSG_ID_BT_BM_LINK_CONNECT_ACCEPT_NOT_AUTO_REQ:
	 {/*	
	     bt_bm_link_connect_accept_ind_struct *param_ptr;
            param_ptr = (bt_bm_link_connect_accept_ind_struct *)construct_local_para(
            (kal_uint16)sizeof(bt_bm_link_connect_accept_ind_struct), TD_CTRL);	
	     param_ptr->bd_addr.lap = rdabt_btLap;	// TODO: bd address need to check!!!
	     param_ptr->bd_addr.nap = rdabt_btNap;
	     param_ptr->bd_addr.uap = rdabt_btUap;
            rdabt_send_msg_up(MOD_MMI, MSG_ID_BT_BM_LINK_CONNECT_ACCEPT_IND, (void *)param_ptr, NULL);//the confirm message may not right,check it
*/
        break;
     	}
     case MSG_ID_BT_BM_DISCOVERY_CANCEL_REQ:
        {
            MGR_InquiryCancel((void *)bt_discovery_cancel_cb);	
            break;
        }
        
    case MSG_ID_BT_BM_BONDING_REQ:
        {
            bt_bm_bonding_req_struct *msg_p = (bt_bm_bonding_req_struct *)message->local_para_ptr;
            t_bdaddr tempbd;
            int pairedNum = 0;
            kal_uint8 i;
            pairedNum =bth_get_paired_dev_num();
            if( pairedNum <10 || bth_is_the_specified_device_paired(msg_p->bd_addr))
            {
                bonding_role = 1;		//we sent the bonding request....
                BDADDR_Set_LAP(&tempbd,msg_p ->bd_addr.lap);
                BDADDR_Set_UAP(&tempbd,msg_p ->bd_addr.uap);
                BDADDR_Set_NAP(&tempbd,msg_p ->bd_addr.nap);	
                pin_code_sdap.bd_addr.lap= msg_p->bd_addr.lap;
                pin_code_sdap.bd_addr.uap= msg_p->bd_addr.uap;
                pin_code_sdap.bd_addr.nap= msg_p->bd_addr.nap;
                pin_code_sdap.sdap_len= msg_p->sdap_len;
                for(i= 0;i<BTBM_ADP_MAX_SDAP_UUID_NO;i++)
                    pin_code_sdap.sdap_uuid[i] = msg_p->sdap_uuid[i];
                
                //memcpy(&pin_code_sdap,&msg_p->sdap_len,sizeof(pin_code_sdap_struct));
                i = MGR_InitBonding(tempbd, rdabt_reply_rsp_cb);

                kal_prompt_trace(1,"MGR_InitBonding return %d", i);
            }
            else
            {
                bt_bm_bonding_cnf_struct * bonding_cnf ;
                bonding_cnf = (bt_bm_bonding_cnf_struct *)construct_local_para((kal_uint16)sizeof(bt_bm_bonding_cnf_struct), TD_CTRL);
                bonding_cnf->result = 1;
                bonding_cnf->bd_addr = msg_p->bd_addr;
          	  rdabt_send_msg_up(MOD_MMI, MSG_ID_BT_BM_BONDING_CNF, (void*)bonding_cnf, NULL);	
            }
            break;
        }
        
       case MSG_ID_BT_BM_PIN_CODE_RSP:
        {
            bt_bm_pin_code_rsp_struct *msg_p = (bt_bm_pin_code_rsp_struct *)message->local_para_ptr;
            bt_bm_bonding_cnf_struct * bonding_cnf=NULL;   
            bt_bm_bonding_result_ind_struct * result_ind=NULL;
            t_bdaddr tempbd;
//	    t_bdaddr  *addr = (t_bdaddr *)&tempbd;
            BDADDR_Set_LAP(&tempbd,msg_p ->bd_addr.lap);
            BDADDR_Set_UAP(&tempbd,msg_p ->bd_addr.uap);
            BDADDR_Set_NAP(&tempbd,msg_p ->bd_addr.nap);	

	     responsePinCode = 1;		//07-13-2009
	     if(pin_code_ind_timer)
	     	{
           		 rdabt_cancel_timed_event(pin_code_ind_timer);	
		 	pin_code_ind_timer=0;
	     	}
            if(  bonding_role == 0)
            {	
                kal_prompt_trace(1,"other device request pair....................................................");
                if( msg_p->pin_len > 0)
                {
                    MGR_SetBondingCallBack(tempbd, rdabt_being_bonding_cb);	
			bonding_addr  = tempbd;
			g_PairingFinished = 0;
			if((get_class_of_device(tempbd)&0x200000)==0x200000)
			{
				wait_bonding_timer = rdabt_timed_event_in(3*1000*1000, (rdabt_timer_func)rdabt_wait_bonding_MoniterTimer_cb, NULL);//zhou siyou 20090711,after putin pincode
				mmi_trace(1,"wait_bonding_timer is create 2.52.52.5wait_bonding_timer=%d",wait_bonding_timer);

			}
			else
			{
				wait_bonding_timer = rdabt_timed_event_in(20*1000*1000, (rdabt_timer_func)rdabt_wait_bonding_MoniterTimer_cb, NULL);//zhou siyou 20090711,after putin pincode
				mmi_trace(1,"wait_bonding_timer is create20202020 wait_bonding_timer=%d",wait_bonding_timer);
			}
                }
                else
                {
                    bonding_cnf = (bt_bm_bonding_cnf_struct *)construct_local_para((kal_uint16)sizeof(bt_bm_bonding_cnf_struct), TD_CTRL);
                    result_ind = (bt_bm_bonding_result_ind_struct*)construct_local_para((kal_uint16)sizeof(bt_bm_bonding_result_ind_struct),TD_CTRL);
                    bonding_cnf->result = result_ind->result = 1;
                    bonding_cnf->bd_addr = result_ind->bd_addr = msg_p->bd_addr;
                    result_ind->cod = 0;
              rdabt_send_msg_up(MOD_MMI, MSG_ID_BT_BM_BONDING_RESULT_IND, (void*)result_ind, NULL);	
              rdabt_send_msg_up(MOD_MMI, MSG_ID_BT_BM_BONDING_CNF, (void*)bonding_cnf, NULL);	

                }
            }
            if(msg_p->pin_len==0)
                {
                  pincode_sent = 0;
                MGR_PINResponse(tempbd,0, NULL,  0);///PIN CODE Negative
                }
            else
                MGR_PINResponse(tempbd,1, msg_p ->pin_code,  msg_p ->pin_len);///PIN CODE Negative
            break;
        }
        
     case MSG_ID_BT_BM_SERVICE_SEARCH_REQ:
        {
            int i;
            bt_bm_service_search_req_struct *msg_p = (bt_bm_service_search_req_struct *)message->local_para_ptr;
            pin_code_sdap.bd_addr.lap= msg_p->bd_addr.lap;
            pin_code_sdap.bd_addr.uap= msg_p->bd_addr.uap;
            pin_code_sdap.bd_addr.nap= msg_p->bd_addr.nap;
            pin_code_sdap.sdap_len= msg_p->sdap_len;
            for(i= 0;i<BTBM_ADP_MAX_SDAP_UUID_NO;i++)
            pin_code_sdap.sdap_uuid[i] = msg_p->sdap_uuid[i];          
            rdabt_SDP_serviceSearch(msg_p);
            break;
        }
        
       case MSG_ID_BT_BM_DELETE_TRUST_REQ:
        {
            bt_bm_delete_trust_req_struct * msg_p = (bt_bm_delete_trust_req_struct*)message->local_para_ptr;
            bt_bm_delete_trust_cnf_struct *delete_cnf;
            int status = 0;
            t_bdaddr 	address;

            struct st_t_BT_ConnectionEntry * cur = NULL, *next= NULL, *theEntry = NULL;
            delete_cnf = (bt_bm_delete_trust_cnf_struct*) construct_local_para(sizeof(bt_bm_delete_trust_cnf_struct), TD_CTRL);

            BDADDR_Set_LAP(&address,msg_p ->bd_addr.lap);
            BDADDR_Set_UAP(&address,msg_p ->bd_addr.uap);
            BDADDR_Set_NAP(&address,msg_p ->bd_addr.nap);	


            MGR_GetNextConnectionRecord(NULL, &next);
            while(next)
            {
                cur = next;
           if( BD_ADDR_CMP(next->bdAddress, address))
		  {
                    theEntry = next;
                }
                MGR_GetNextConnectionRecord(cur, &next);

            }

            if( theEntry)
            {
                status = MGR_RemoveConnectionRecord(theEntry, 0);
                if( status == 0)
                {
                    kal_prompt_trace(1, "It is about to delete the device info that stored in file........");
                    status = bth_del_paired_device_info(msg_p->bd_addr, 0);
                }
            }

            delete_cnf->bd_addr = msg_p->bd_addr;
            delete_cnf->result = status;
           rdabt_send_msg_up( MOD_MMI,  MSG_ID_BT_BM_DELETE_TRUST_CNF, (void *)delete_cnf,  NULL);

            break;
        }
        	
	case MSG_ID_BT_BM_LINK_CONNECT_ACCEPT_RSP: 
	{
		bt_bm_link_connect_accept_rsp_struct* msg_p = (bt_bm_link_connect_accept_rsp_struct*)message->local_para_ptr;
		t_bdaddr address;
		BDADDR_Set_LAP(&address,msg_p ->bd_addr.lap);
		BDADDR_Set_UAP(&address,msg_p ->bd_addr.uap);
		BDADDR_Set_NAP(&address,msg_p ->bd_addr.nap);	
		
		MGR_AcceptConnRequest(address, msg_p->result);

		/*
		if(msg_p->result == 0)
		{
			deviceName = (char*)pMalloc(48);
			if( deviceName == NULL)
			{
				MGR_PINResponse(address,  0, NULL, 0);	
			}
			else
			{
				int status = 0;
				memset(deviceName, 0, 48);
				status = MGR_GetRemoteName( address, deviceName, 47,  &nameLen, rdabt_GetRemoteName_cb);
				if( status == 0)
				{
					rdabt_GetRemoteName_cb(address, 0);	
				}
			}
		}
		else
		{
			MGR_PINResponse(address,  0, NULL, 0);	
		}
		*/
		break;
	}

   	case MSG_ID_BT_SDPDB_REGISTER_REQ:
	{
	        bt_sdpdb_register_req_struct *msg_p = (bt_sdpdb_register_req_struct *)message->local_para_ptr;
		 rdabt_bt_sdpdb_register_req(msg_p);

		break;
	}

       case   	 MSG_ID_BT_SDPDB_DEREGISTER_REQ:
	{
	        bt_sdpdb_deregister_req_struct *msg_p = (bt_sdpdb_deregister_req_struct *)message->local_para_ptr;
		kal_prompt_trace(1,"MSG_ID_BT_SDPDB_DEREGISTER_REQ");
		 rdabt_sdpdb_deregister_req(msg_p);

		break;
	}

	 case MSG_ID_BT_RESTART_REQ:
	 {
	 	rdabt_bt_restart();
	 	break;
	 }
	 case MSG_ID_BT_MMI_RESET_REQ:
	 {
	 	rdabt_antenna_off();
	 	rdabt_send_msg_up(MOD_MMI, MSG_ID_BT_MMI_RESET_CNF, NULL, NULL);
	 }
        default:
            break;				
    }



	
}

/*****************************************************************************
* FUNCTION
*   rdabt_power_on_sched
* DESCRIPTION
*   Main loop when in power on state
* PARAMETERS
*  queue_id               IN     queue index of bt
*   taks_entry_ptr      IN    task entry
* RETURNS
*   None.
* GLOBALS AFFECTED
*   external_global
*****************************************************************************/
void rdabt_power_on_sched(kal_msgqid queue_id)
{
   ilm_struct *ext_message=NULL;
//   kal_uint32 msg_count;
 //  kal_int32 task_index = 0;
   int status=0;
   kal_prompt_trace(1,"rdabt_power_on_sched");
   COS_EVENT ev;
   TASK_HANDLE * hTask;
   Msg_t *BtMsg;
   hTask = (TASK_HANDLE *)GetMmiTaskHandle(MOD_BT);
 //  unsigned int err = 0;
    static UINT32 count; 
    UINT32 checkLoopTime=0;
    while (1)
    {
      /*
         Message within rdabt tasks can be separated into external and internal messages,
         and we take 1:1 ratio between them. Because there are more than 1 internal modules,
         rounb-robin machanism is applied to all the internal modules.
      */

       SUL_ZeroMemory32(&ev, sizeof(COS_EVENT) );
	do {
		if (count ==0)
		{
			checkLoopTime = hal_TimGetUpTime();
		}
		check_count = 0;
		rdabt_ExitLpMode();
		papiSched(0);
		if (!reset_in_process)
		status = BT_Execute(2);
		rda5868_uart_rx();
		count++;
		if (count >1)
			mmi_trace(1,"loop in rabt_main,count=%d  loops use %d ticks\n",count,hal_TimGetUpTime()-checkLoopTime);
		if(sxr_SbxHot(hTask->nMailBoxId))
			break;
	}while (status ==BT_RETRY);
	count = 0;
	BtMsg = sxr_Wait((u32 *)&ev, hTask->nMailBoxId);
	check_count = 0;
	rdabt_ExitLpMode();		
	if (BtMsg!= NULL )
	{
		UINT32 *msg= (UINT32*)BtMsg;
		if (*msg  == 0x12345678)
		{
			rdabt_bt_restart();
	   		mmi_trace(1,"error occur $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
		}
		else
	   	mmi_trace(1,"rdabt_power_on_sched Get a envet for BT set event\n");
		sxr_Free(BtMsg);
	}
	else
	{
		if ( (ev.nEventId >= BT_TIMER_PAPI) && (ev.nEventId < BT_TIMER_PAPI+MAX_BT_TIMER))
		{
			BT_TIMER_EVT *timer;
			timer = (BT_TIMER_EVT *)ev.nParam1;
		//	mmi_trace(1,"rdabt_power_on_sched The callback timer handler is 0x%x\n",timer->callbackfun);

			if(timer->callbackfun)
			   timer->callbackfun(timer->argv);
		//	else
              //          asm("break 1");
			
			pTimerCancel(timer->timerHandle);
			timer->timerHandle=0;
		}
		else
		{
		  	ext_message = (ilm_struct*)ev.nParam1;
			//mmi_trace(1,"rdabt_power_on_sched Get a Message id = %d,count = %d",ext_message->msg_id,count);
#ifdef RDA_BT_DEBUG			
			Debug_BT_Sap(0,ext_message->src_mod_id,ext_message->msg_id);
#endif			
		      	// receive_msg_ext_q (queue_id, &ext_message);
		      	if (ext_message==NULL)
					asm("break 1");
				
			  #ifdef  __BT_A2DP_PROFILE__
		         if( (ext_message->msg_id >=MSG_ID_BT_A2DP_ACTIVATE_REQ) && (ext_message->msg_id<= MSG_ID_BT_A2DP_STREAM_ABORT_RES))
		         {
		            rdabt_a2dp_msg_hdlr( ext_message );
		         }
			 #endif	
			 
			 #ifdef  __BT_AVRCP_PROFILE__	 
		        else if ( (ext_message->msg_id >=MSG_ID_BT_AVRCP_ACTIVATE_REQ) && (ext_message->msg_id<= MSG_ID_BT_AVRCP_CMD_FRAME_IND_RES))
		         {
 		            rdabt_avrcp_msg_hdlr( ext_message );
		  
		         }
			  #endif		
		      else if( (ext_message->msg_id ==MSG_ID_MEDIA_BT_AUDIO_OPEN_REQ) || (ext_message->msg_id ==MSG_ID_MEDIA_BT_AUDIO_CLOSE_REQ)
		                    || ((ext_message->msg_id >=MSG_TAG_BT_HFG_FIRST_REQ_MSG) && (ext_message->msg_id<= MSG_TAG_BT_HFG_LAST_REQ_MSG)))
		         {
                          rdabt_hfg_msg_hdler(ext_message);
		         }
		       else if( ext_message->msg_id<=MSG_TAG_BT_HSG_LAST_REQ_MSG && ext_message->msg_id>=MSG_TAG_BT_HSG_FIRST_REQ_MSG)
		        {
		            rdabt_hsg_msg_hdler(ext_message);
		        }
		        else if( (ext_message->msg_id >=MSG_ID_GOEP_REGISTER_SERVER_REQ) && (ext_message->msg_id<= MSG_ID_BT_RECORD_UNREQ_CNF))
		         {
		            rdabt_opp_msg_hdler(ext_message);
		         }
		         else
		        {
		            rdabt_main_msg_hdler(queue_id, ext_message);
		        }   

		        //  rdabt_p->curr_queue = 0xffff; // added by YS
		        if(ext_message)
		        {
					MMI_Free_msg_buf(ext_message);
		              	ext_message  = NULL;
		        }
			else
			{
				asm("break 1");
			}
		        if(rdabt_p->state == RDABT_STATE_POWER_OFF)
		            break;
		    //rdabt_send_msg_up(MOD_BT, MSG_ID_BT_NOTIFY_EVM_IND, NULL, NULL);
		}
	}
	rda5868_uart_rx();
    }
}

/*****************************************************************************
* FUNCTION
*   rdabt_main
* DESCRIPTION
*   This function is the main function of bt task
* PARAMETERS
*   task_entry_ptr  IN   taks entry of bt
* RETURNS
*   None.
* GLOBALS AFFECTED
*   external_global
*****************************************************************************/
void rdabt_main(void)
{
    kal_msgqid queue_id=0;//not used

    /* main loop of bchs_main for all kinds of states */
    mmi_trace(g_sw_BT,"entry rdabt_main--------------------------------------------\n");
    memset(rdabt_p,0,sizeof(rdabt_cntx));

    g_btdConfig = tgt_GetBtdConfig();
	if (g_btdConfig->pinReset.type == HAL_GPIO_TYPE_IO)
	{
		hal_GpioSetOut(g_btdConfig->pinReset.gpioId);
	}
	if (g_btdConfig->pinWakeUp.type == HAL_GPIO_TYPE_IO)
	{
		hal_GpioSetOut(g_btdConfig->pinWakeUp.gpioId);
	}
	if (g_btdConfig->pinSleep.type == HAL_GPIO_TYPE_IO)
	{
		hal_GpioSetIn(g_btdConfig->pinSleep.gpioId);
	}

 	hal_GpioClr(g_btdConfig->pinReset);
    pmd_EnablePower(PMD_POWER_BT, FALSE);

   while(1)
   {
      switch(rdabt_p->state)
      {
         case RDABT_STATE_POWER_OFF:
            rdabt_power_off_sched(queue_id);
            break;
         case RDABT_STATE_POWER_ON:
            rdabt_power_on_sched(queue_id);
            break;
         default:
            ASSERT(0);
      }
   }
}
/*****************************************************************************
* FUNCTION
*   bt_create
* DESCRIPTION
*   This function creats bt task
* PARAMETERS
*   handle  OUT   pointer to the table of bt task structure.
* RETURNS
*   Always TRUE since the creation shall not be failed.
* GLOBALS AFFECTED
*   external_global
*****************************************************************************/
#if 0
kal_bool bt_create(comptask_handler_struct **handle)
{
	static const comptask_handler_struct rda_handler_info =
	{
		rdabt_main,       /* task entry function */
		rdabt_init,       /* task initialization function */
		NULL,            /* task configuration function */
		rdabt_reset,      /* task reset handler */
		NULL,            /* task termination handler */
	};

	*handle = (comptask_handler_struct *)&rda_handler_info;
	return KAL_TRUE;
}

#endif

/*****************************************************************************
* FUNCTION
*   rdabt_get_seq_num
* DESCRIPTION
*   This function get a global sequence number within rdabt
* PARAMETERS
*   None
* RETURNS
*   Global sequence number
* GLOBALS AFFECTED
*   None.
*****************************************************************************/
kal_uint32 rdabt_get_seq_num(void)
{
   if(++rdabt_p->seq_num == RDABT_MAX_SEQ_NUM)
      rdabt_p->seq_num = 0;

   return rdabt_p->seq_num;
}

kal_uint8 rdabt_get_LMP_VersioID(void)
{
	 t_MGR_HCVersion versionInfo;

	MGR_GetDeviceVersion(&versionInfo);
	return versionInfo.LMPversion;

}

