/*
 * MODULE NAME:    host_callback.c
 * DESCRIPTION:    Bluestream control functions implementation
 * AUTHOR:         Martin Mellody
 *
 * SOURCE CONTROL: $Id: rdabt_callback.c 8609 2011-06-29 06:01:34Z cuilf $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2002 Parthus Technologies.
 *     All rights reserved.
 *
 */
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

#include "nvram_struct.h"           /* for nvram read/write */
#include "nvram_user_defs.h"
#include "bluetooth_bm_struct.h"

#include "bt.h"
#include "sdp_cli.h"

#include "bt_a2dp_struct.h"
#include "rda_bt_hfg.h"
#include "rda_bt_avrcp.h"
#include "bt_dm_struct.h"
#include "bt_hfg_struct.h"
#include "bt_avrcp_struct.h"
//#include "rdabtmmia2dp.h"

#include "rdabt.h"
#include "rdabt_msg.h"
//#include "manage.h"

#include "rdabt_config.h"
#include "rdabt_task.h"
#include "rdabt_profile.h"
#include "rdabt_link_prim.h"
#include "rdabt_callback.h"
#include "manager.h"
#include "papi.h"
#include "mmi_trace.h"
#include "rdabt_opp.h"

extern rdabt_opps_struct* rdabt_oppsp;
extern rdabt_ftps_struct* rdabt_ftpsp;

extern kal_uint16 rdabt_btNap;
extern kal_uint16 rdabt_btUap;
extern kal_uint32 rdabt_btLap;
extern u_int32 rdabt_put_message(u_int16 q, u_int16 mi, void *mv);
extern bt_bm_bonding_req_struct pin_code_sdap;
extern int MGR_GetDeviceNumFound(void);

//scheduler_identifier getremoteName_cb_timer;
scheduler_identifier wait_bonding_timer=0;
extern int g_PairingFinished; 		//add by tan , 7-12-2009

//for detect HCI_evDISCONNECTIONCOMPLETE
scheduler_identifier SDP_ReleaseACL_timer=0;

t_SDP_Results  * pResults;
t_SDP_service_Parse_Results uuid_finded; 
t_bdaddr  bonding_addr;

int 	responsePinCode = 0;
int 	bonding_role  = 0; 					//0 indicates we received bonding request from other device
int   pincode_sent=0;
char * deviceName = NULL;
u_int8 nameLen = 0;
//bt_host_cb_tbl bt_host_cb_cntx;

scheduler_identifier pin_code_ind_timer;

extern int bth_del_paired_device_info(btbm_bd_addr_t bd_addr,  int flag);
extern int bth_write_paired_device_info(bth_dev_struct * pDevInfo);
extern void bth_read_paired_dev_info(bth_dev_info_array *pDevInfo);
void bth_get_specified_device_class(btbm_bd_addr_t bd_addr, u_int32 * classOfdevice);
extern int  bth_is_the_specified_device_paired(btbm_bd_addr_t bd_addr);
extern u_int8 * print_link_key(u_int8 *pLinkKey );			//add by tan only for test
extern scheduler_identifier rdabt_timed_event_in(kal_uint32 duration, rdabt_timer_func fn, /*uint16_t mi,*/ void *mv);
void rdabt_wait_bonding_MoniterTimer_cb(void);

	
void rdabt_write_scanenable_cb(int status)
{
    LINK_WRITE_SCANENABLE_MODE_ST mv;
    mv.type = LINK_WRITE_SCANENABLE_MODE_CNF;
    mv.result = status;
    rdabt_put_message(EXTERNAL_TASK_MASK,LINK_Q,&mv);
}


void rdabt_write_localaddr_cb(int status)
{
    LINK_WRITE_LOCALADDR_ST mv;
    mv.type = LINK_WRITE_LOCALADDR_CNF;
    mv.result = status;
    mv.bd_addr.lap = rdabt_btLap;
    mv.bd_addr.uap = rdabt_btUap;
    mv.bd_addr.nap = rdabt_btNap;
    rdabt_put_message(EXTERNAL_TASK_MASK,LINK_Q,&mv);
}

void bt_write_authentication_mode_cb(int status)
{
    LINK_WRITE_LOCALADDR_ST mv;
    mv.type = LINK_WRITE_AUTHENTICATION_MODE_CNF;
    mv.result = status;
    rdabt_put_message(EXTERNAL_TASK_MASK,LINK_Q,&mv);
}

void bt_discovery_cnf_cb(u_int8 status ,u_int8 total_num)
{
    LINK_DISCOVERY_CNF_ST mv;
    mv.type = LINK_DISCOVERY_CNF;
    mv.result = status;
    mv.total_number = total_num;
    rdabt_put_message(EXTERNAL_TASK_MASK,LINK_Q,&mv);
} 

void bt_discovery_result_ind_cb(t_MGR_DeviceEntry *newDevice, int status)
{

  kal_prompt_trace(1,"bt_discovery_result_ind_cb find device ? newDevice =%x,status =%d ", newDevice,status);
	if(!newDevice)
	{
	    LINK_DISCOVERY_CNF_ST mv;
	    mv.type = LINK_DISCOVERY_CNF;
	    mv.result = status;
	    mv.total_number = MGR_GetDeviceNumFound();
	    rdabt_put_message(EXTERNAL_TASK_MASK,LINK_Q,&mv);

	}
	else
	{
		if(status == 0)
		{
			    LINK_DISCOVERY_RESULT_ST mv;
			    mv.type = LINK_DISCOVERY_RESULT_IND;
			    mv.result = status;
			    //pMemcpy(mv.device.address.bytes, (u_int8 *)&newDevice, sizeof(t_MGR_DeviceEntry));

			    mv.device = (t_MGR_DeviceEntry *)newDevice;

			    rdabt_put_message(EXTERNAL_TASK_MASK,LINK_Q,&mv);
		}
	}

} 

void bt_discovery_cancel_cb(u_int8 num,u_int8 result)
{
    LINK_CANCEL_DISCOVERY_ST mv;
    mv.type = LINK_DISCOVERY_CANCEL_CNF;
    mv.result = result;
    mv.total_number = num;
    rdabt_put_message(EXTERNAL_TASK_MASK,LINK_Q,&mv);
}



void rdabt_InputPin_MoniterTimer_cb(void  *addr )
{
	//t_bdaddr tempaddr;
	//memcpy(tempaddr.bytes,(u_int8 *)addr,sizeof(t_bdaddr));	//commented by tan, 7-12-2009
	
	
	mmi_trace(1,"rdabt_InputPin_MoniterTimer_cb is called responsePinCode=%d,bonding_role=%d",responsePinCode,bonding_role);
	if( responsePinCode ==0)
	{

		MGR_PINResponse(bonding_addr, 0, NULL, 0);			//modified by tan 7-12-2009

		if(bonding_role == 0)
		{
			bt_bm_bonding_result_ind_struct * param_ptr = NULL;
			bt_bm_bonding_cnf_struct * bonding_cnf=NULL;   
			param_ptr = (bt_bm_bonding_result_ind_struct*)construct_local_para((kal_uint16)sizeof(bt_bm_bonding_result_ind_struct), TD_CTRL);

			param_ptr->bd_addr.lap= BDADDR_Get_LAP(&bonding_addr);
			param_ptr->bd_addr.uap = BDADDR_Get_UAP(&bonding_addr);
			param_ptr->bd_addr.nap = BDADDR_Get_NAP(&bonding_addr);
			param_ptr->result = 1;
			param_ptr->cod = 0;

			bonding_cnf = (bt_bm_bonding_cnf_struct *)construct_local_para((kal_uint16)sizeof(bt_bm_bonding_cnf_struct), TD_CTRL);
                    bonding_cnf->result = 1;
                    bonding_cnf->bd_addr = param_ptr->bd_addr;
			rdabt_send_msg_up(MOD_MMI, MSG_ID_BT_BM_BONDING_RESULT_IND, (void *)param_ptr, NULL);
		       rdabt_send_msg_up(MOD_MMI, MSG_ID_BT_BM_BONDING_CNF, (void*)bonding_cnf, NULL);//zhou siyou add 200724	
			
		}
	}
	/*
	if (getremoteName_cb_timer)
	{
		pTimerCancel(getremoteName_cb_timer);
		getremoteName_cb_timer=0;
	}
	*/
	if (pin_code_ind_timer)//zhou siyou open
	{
		pTimerCancel(pin_code_ind_timer);
		pin_code_ind_timer=0;
	}
	
}


void bt_pin_code_ind_cb(t_bdaddr address )
{
    LINK_PIN_CODE_IND_ST mv;
    t_bdaddr  *addr = (t_bdaddr *)&address;
    btbm_bd_addr_t	bd_addr;
    bd_addr.lap= BDADDR_Get_LAP(&address);
    bd_addr.uap = BDADDR_Get_UAP(&address);
    bd_addr.nap = BDADDR_Get_NAP(&address);	
mmi_trace(1,"bt_pin_code_ind_cb is called!! bonding_role=%d,pincode_sent=%d",bonding_role,pincode_sent);
if(pincode_sent)//zhou siyou 20110628
{
	pincode_sent=0;
	mmi_trace(1,"!!!!!!!!!!!!!!!!!!!!!!!!!!bt_pin_code_ind_cb is called pin %s!!","0000");
	
	mmi_trace(1,"!!!!!!!!!!!!!!!!!!!!!!!!!!bt_pin_code_ind_cb is called pin %s!!","0000");

	mmi_trace(1,"!!!!!!!!!!!!!!!!!!!!!!!!!!bt_pin_code_ind_cb is called pin %s!!","0000");
	MGR_PINResponse(address, 1, "0000", 4);	
	return;

}
     responsePinCode = 0;
    if(bth_is_the_specified_device_paired(bd_addr))
    {		
    		mmi_trace(1,"bt_pin_code_ind_cb is called!!if(bth_is_the_specified_device_paired(bd_addr))");
		mv.type = LINK_PIN_CODE_IND;
		memcpy(mv.bd_addr.bytes,address.bytes,sizeof(t_bdaddr));
		MGR_GetRemoteName(address, (char*)mv.name, 47, &mv.name_len, NULL);

		rdabt_put_message(EXTERNAL_TASK_MASK,LINK_Q,&mv);
		pin_code_ind_timer = rdabt_timed_event_in(15*1000*1000,rdabt_InputPin_MoniterTimer_cb,(void *)addr);
    }
    else
    {
    if( bonding_role != 0)
    {		mmi_trace(1,"bt_pin_code_ind_cb is called!! if( bonding_role != 0)");

		mv.type = LINK_PIN_CODE_IND;
		memcpy(mv.bd_addr.bytes,address.bytes,sizeof(t_bdaddr));
		MGR_GetRemoteName(address, (char*)mv.name, 47, &mv.name_len, NULL);

		rdabt_put_message(EXTERNAL_TASK_MASK,LINK_Q,&mv);
		bonding_addr = address;		//add by tan, 7-12-2009
		pin_code_ind_timer = rdabt_timed_event_in(15*1000*1000,rdabt_InputPin_MoniterTimer_cb,NULL);
    	}		
	else 
	{ 
		//may be the other device has lost the security info..
		//anyway, we do not need to bond...

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

			/*
			bt_bm_link_connect_accept_ind_struct	  * accept_ind;

			accept_ind = (bt_bm_link_connect_accept_ind_struct*)construct_local_para((kal_uint16)sizeof(bt_bm_link_connect_accept_ind_struct), TD_CTRL);
			accept_ind->bd_addr.lap= BDADDR_Get_LAP(&address);
			accept_ind->bd_addr.uap = BDADDR_Get_UAP(&address);
			accept_ind->bd_addr.nap = BDADDR_Get_NAP(&address);	

			rdabt_send_msg_up(MOD_MMI, MSG_ID_BT_BM_LINK_CONNECT_ACCEPT_IND, (void *)accept_ind, NULL);		
	              MGR_SetBondingCallBack(address, rdabt_being_bonding_cb);	
	              */
		}

	}	
}
kal_uint8 sdp_result = 0;
kal_uint8 sdp_searched_number = 0;

//add by cuijian 20100115 for discaooect acl
void rdabt_ServiceSearch_cb_ReleaseACL(void *dbEntry, int status)			
{
	    bt_bm_service_search_cnf_struct	* service_cnf=NULL;
           if(SDP_ReleaseACL_timer)
		{
			pTimerCancel(SDP_ReleaseACL_timer);
			SDP_ReleaseACL_timer = 0;
		}
	    service_cnf = (bt_bm_service_search_cnf_struct*)construct_local_para( (kal_uint16)sizeof(bt_bm_service_search_cnf_struct), TD_CTRL);
	    service_cnf->result = sdp_result;
	    service_cnf->searched_number = sdp_searched_number;//uuid_finded.total_searched;

	    service_cnf->bd_addr.lap = pin_code_sdap.bd_addr.lap;
	    service_cnf->bd_addr.uap = pin_code_sdap.bd_addr.uap;
	    service_cnf->bd_addr.nap = pin_code_sdap.bd_addr.nap;	
	    kal_prompt_trace(1,"MSG_ID_BT_BM_SERVICE_SEARCH_CNF");
	    rdabt_send_msg_up(MOD_MMI, MSG_ID_BT_BM_SERVICE_SEARCH_CNF, (void *)service_cnf, NULL);
}

//add by cuijian 20100115 for discaooect acl
extern  int  MGR_GetACLState(t_bdaddr addr) ;
extern void  prh_l2_acl_shutdown_all(u_int32 flags);
void rdabt_ServiceSearch_cb_timeout(void)			
{
   int result_aclsatus=0;
   result_aclsatus=MGR_GetACLState(pResults->element.deviceAddress);
       kal_prompt_trace(0, "rdabt_ServiceSearch_cb_timeout,bdaddrs=%s,result_aclsatus=0x%x\n",prh_mgr_printBDAddress(pResults->element.deviceAddress,result_aclsatus));	
    if( (result_aclsatus== 2)||(result_aclsatus== 0))//CONN_CLOSED  zhou siyou modified 20100722
	{
				rdabt_ServiceSearch_cb_ReleaseACL(NULL,0);
	}
	else
	{
		    kal_prompt_trace(0, "rdabt_ServiceSearch_cb_timeout2\n");	
		    if(OBEX_L2_ACL_Expired(&(pResults->element.deviceAddress)) )
		    	{
		    		prh_l2_acl_shutdown_all(0);
				rdabt_ServiceSearch_cb_ReleaseACL(NULL,0);
		    	}
	}
	SDP_ReleaseACL_timer = 0;
	return;
}

void rdabt_ServiceSearch_cb(u_int8 status)			
{
 //   t_SDP_Element *pCurrentResult = NULL;
    bt_bm_service_search_result_ind_struct * result_ind = NULL;
  //  bt_bm_service_search_cnf_struct	* service_cnf=NULL;

  //  u_int32 	numRecords = 0;
  //  u_int8 	*pData = NULL;

    u_int8  i=0, j=0, k= 0, bfinded=0;

    kal_prompt_trace(1, "rdabt_ServiceSearch_cb pResults->element.dataLen=%d  pResults->element.pData=%x", pResults->element.dataLen,pResults->element.pData);
        pincode_sent=0;
    status = pResults->status;
    if(status == BT_NOERROR)
    {
        memset(&uuid_finded, 0, sizeof(t_SDP_service_Parse_Results));
        if(pResults->element.dataLen!=0)
            status = sdp_service_parse(pResults, &uuid_finded);

        for(i=0;i<BTBM_ADP_MAX_SDAP_UUID_NO;i++)
        {
            if(uuid_finded.service[i].SC_uuid==0)
                break;
            kal_prompt_trace(1, "SC_uuid[%d]=%x, PDL_uuid=%x, chn_num=%d total_search=%d\n", i, uuid_finded.service[i].SC_uuid, uuid_finded.service[i].PDL_uuid,uuid_finded.service[i].chn_num, uuid_finded.total_searched);
        }
        kal_prompt_trace(1,"find %d profiles", i);

    }

    i=0;
    for(j=0;j<uuid_finded.total_searched;j++)
    {
        bfinded=0;
        for(k=0;k<pin_code_sdap.sdap_len;k++)
        {
            if(pin_code_sdap.sdap_uuid[k]==uuid_finded.service[j].SC_uuid)
            bfinded=1;
            i++;
            
        }
        
        if(bfinded)
        {
            result_ind = (bt_bm_service_search_result_ind_struct*)construct_local_para((kal_uint16)sizeof(bt_bm_service_search_result_ind_struct), TD_CTRL);
            result_ind->uuid = uuid_finded.service[j].SC_uuid;
   
            result_ind->bd_addr.lap = pin_code_sdap.bd_addr.lap;
            result_ind->bd_addr.uap = pin_code_sdap.bd_addr.uap;
            result_ind->bd_addr.nap = pin_code_sdap.bd_addr.nap;
	     kal_prompt_trace(1,"MSG_ID_BT_BM_SERVICE_SEARCH_RESULT_IND");
            rdabt_send_msg_up(MOD_MMI, MSG_ID_BT_BM_SERVICE_SEARCH_RESULT_IND, (void *)result_ind, NULL);
        }

    }

	  sdp_result = status;
	  sdp_searched_number = i;

    //if(MGR_GetACLState(pResults->element.deviceAddress) != 2)//CONN_CLOSED
    if(!(MGR_GetACLState(pResults->element.deviceAddress)%4))//CONN_RELEASEPENDING CONN_NONE zhou siyou 20100722
	{
		if ( (rdabt_oppsp->OPPSstatus == GOEP_Client_Connect) || (rdabt_ftpsp->FTPSstatus == GOEP_Client_Connect))
		{
			rdabt_ServiceSearch_cb_ReleaseACL(NULL,0);
		}
		else
		{
			if(!MGR_SetACLCallBack(pResults->element.deviceAddress,0,rdabt_ServiceSearch_cb_ReleaseACL))
				kal_prompt_trace(0, "MGR_SetACLCallBack!@!!!!!!");	
			else
			{
				kal_prompt_trace(0, "MGR_SetACLCallBack fail");
			}
			if(SDP_ReleaseACL_timer)
				pTimerCancel(SDP_ReleaseACL_timer);
			SDP_ReleaseACL_timer = pTimerCreate(30,(rdabt_timer_func)rdabt_ServiceSearch_cb_timeout,NULL,pTIMER_ONESHOT);//60 is 6 secend		
		}	
	}
	else
	{
		rdabt_ServiceSearch_cb_ReleaseACL(NULL,0);
	}

}





void rdabt_SDP_serviceSearch(bt_bm_service_search_req_struct * msg_p)
{

	
	t_SDP_Addresses 		devices;
	t_SDP_SearchPattern  	pattern;
	t_SDP_AttributeIDs  	attributes;
	t_SDP_StopRule 		rule;
	 u_int16 criteria=0;
	t_bdaddr bdAddr;

	u_int32 	cod = 0;

	btbm_bd_addr_t	bd_addr;
	
	kal_prompt_trace(1,"--> rdabt_SDP_serviceSearch");

	BDADDR_Set_LAP(&bdAddr,msg_p ->bd_addr.lap);
	BDADDR_Set_UAP(&bdAddr,msg_p ->bd_addr.uap);
	BDADDR_Set_NAP(&bdAddr,msg_p ->bd_addr.nap);	




	bd_addr = msg_p->bd_addr;

	bth_get_specified_device_class(bd_addr, &cod);

	kal_prompt_trace(1,"--> rdabt_SDP_serviceSearch, cod == %d", cod);
	
	devices.numItems = 1;
	devices.addresses = ( t_bdaddr *)pMalloc(sizeof(t_bdaddr));
	if(devices.addresses)
		devices.addresses[0] = bdAddr;

	pattern.numItems = 1;
	pattern.patternUUIDs = (u_int32*)pMalloc(pattern.numItems*sizeof(u_int32));
	if(pattern.patternUUIDs)
		pattern.patternUUIDs[0] = SDP_PROTOCOL_UUID_L2CAP;

	if(cod & 0x200)
		attributes.numItems = 1;
	else
		attributes.numItems = 4;
	attributes.attributeIDs =(u_int32*)pMalloc(attributes.numItems*sizeof(u_int32));
	if(attributes.attributeIDs)
	{
		attributes.attributeIDs[0] = SDP_AID_SERVICE_CLASS;
		if(!(cod&0x200))
	  	{
	  		attributes.attributeIDs[1] = SDP_AID_PROFILE_LIST;
	 		attributes.attributeIDs[2] = SDP_AID_PROTOCOL;
	 		attributes.attributeIDs[3] = SDP_AID_SUPPORTED_FEATURES;
		}
	}

	rule.maxBytes = 512;
	rule.maxDuration = 50;
	rule.maxItems = 10;

	SDP_ServiceBrowse(&pResults,&devices,&pattern,&attributes,&rule,criteria,(sdp_api_callback)rdabt_ServiceSearch_cb);    

	pFree(devices.addresses);
	pFree(pattern.patternUUIDs);
	pFree(attributes.attributeIDs);


}


void rdabt_reply_rsp_cb(t_bdaddr address, int status)
{


    LINK_PIN_CODE_RESULT_IND_ST mv1;
    LINK_PIN_CODE_CNF_ST mv2;
    bt_bm_service_search_req_struct  service_msg;	
    bt_bm_service_search_req_struct *service_msgp=&service_msg;
    kal_uint8 i;

	u_int8 flag_preset=0;
	struct st_t_BT_ConnectionEntry * cur=NULL, *next=NULL;
	bth_dev_struct 	dev_Info;

	bonding_role = 0;
	pincode_sent=1;
	if (pin_code_ind_timer)//zhou siyou add 20070725
	{
		pTimerCancel(pin_code_ind_timer);
		pin_code_ind_timer=0;
	}
	
	memset(&dev_Info, 0, sizeof(bth_dev_struct));
	
	kal_prompt_trace(1,"rdabt_reply_rsp_cb, found linkkey, status:%d\n", status);
    if( status == 0)
    {
        MGR_GetNextConnectionRecord(NULL, &next);
        while(next)
        {				
            cur = next;
          if(BD_ADDR_CMP(next->bdAddress, address))
            {
               if( next->linkKey)
                {
					dev_Info.bd_addr.lap= BDADDR_Get_LAP(&address);
				    	dev_Info.bd_addr.uap = BDADDR_Get_UAP(&address);
				    	dev_Info.bd_addr.nap = BDADDR_Get_NAP(&address);
					dev_Info.KeyType = next->linkKeyType;
					dev_Info.cod	= next->cod;
					memcpy( dev_Info.linkKey, next->linkKey, 0x10);

					kal_prompt_trace(1,"rdabt_reply_rsp_cb, found linkkey, cod:%d\n", next->cod);
					flag_preset=1;

                }
              //  break;
				
            }
           //else
            {
                MGR_GetNextConnectionRecord(cur, &next);
            }
        }

      //if(next && next->linkKey)
	  	if(flag_preset)
        {
			mv2.result  = bth_write_paired_device_info(&dev_Info);
        }

		
    }	
 
   mv1.type = LINK_BONDING_RESULT_IND;
    mv1.result = status;
    mv1.bd_addr.lap= BDADDR_Get_LAP(&address);
    mv1.bd_addr.uap = BDADDR_Get_UAP(&address);
    mv1.bd_addr.nap = BDADDR_Get_NAP(&address);	
    mv1.cod =  next?next->cod:0;
     pMemcpy(mv1.linkKey, dev_Info.linkKey, 16);
    mv1.KeyType = dev_Info.KeyType;

//    mmi_trace(g_sw_BT,"rdabt_reply_rsp_cb::status=%d", status);
//    mmi_trace_hex(g_sw_BT,16, dev_Info.linkKey);

    rdabt_put_message(EXTERNAL_TASK_MASK,LINK_Q,&mv1);

    mv2.type = LINK_BONDING_CNF;
    mv2.result = status;
    mv2.bd_addr = mv1.bd_addr;
  rdabt_put_message(EXTERNAL_TASK_MASK,LINK_Q,&mv2);

    if( mv2.result == 0)	//modified by tan
    {
        service_msgp->bd_addr=mv1.bd_addr;
        service_msgp->sdap_len = pin_code_sdap.sdap_len;
        for (i = 0; i < BTBM_ADP_MAX_SDAP_UUID_NO; i++)
        {
            service_msgp->sdap_uuid[i] =  pin_code_sdap.sdap_uuid[i];
        }
        rdabt_SDP_serviceSearch(service_msgp);		
    }

}

//03-28-2009, add by tan 
void rdabt_being_bonding_cb(t_bdaddr address, int status)
{

	bth_dev_struct 	dev_Info;
	struct st_t_BT_ConnectionEntry * cur=NULL, *next=NULL;
	bt_bm_bonding_result_ind_struct  *param_ptr=NULL;
	bt_bm_bonding_cnf_struct * bonding_cnf=NULL;   
	u_int8 flag_present=0;

	kal_prompt_trace(1,"rdabt_being_bonding_cb status = %d\n",status);
	//status = 1;
	
	if( g_PairingFinished == 0)
		g_PairingFinished = 1;  		//7-12-2009, add by tan
	else
		return;

	memset(&dev_Info, 0, sizeof(bth_dev_struct));

	param_ptr = (bt_bm_bonding_result_ind_struct*)construct_local_para((kal_uint16)sizeof(bt_bm_bonding_result_ind_struct), TD_CTRL);

	 param_ptr->bd_addr.lap= BDADDR_Get_LAP(&address);
	 param_ptr->bd_addr.uap = BDADDR_Get_UAP(&address);
	 param_ptr->bd_addr.nap = BDADDR_Get_NAP(&address);

	kal_prompt_trace(1,"rdabt_being_bonding_cb\n");

	if (wait_bonding_timer)
	{	

		kal_prompt_trace(1,"wait_bonding_timer be canceled wait_bonding_timer=%d\n",wait_bonding_timer);
		
		pTimerCancel(wait_bonding_timer);
		wait_bonding_timer=0;
	}
	MGR_GetNextConnectionRecord(NULL, &next);
	while(next)
	{				
		cur = next;
		if( BD_ADDR_CMP(next->bdAddress, address))
		{
			if( next->linkKey)
			{
				kal_prompt_trace(1,"get the device' s info, link_key:%s and cod:%d\n", print_link_key(next->linkKey),next->cod);
				dev_Info.bd_addr = param_ptr->bd_addr;
				dev_Info.KeyType = next->linkKeyType;
				memcpy( dev_Info.linkKey, next->linkKey, 0x10);
				dev_Info.cod =param_ptr->cod = next->cod;
				flag_present=1;//zhou siyou add 20070725
			}
			//break;
		}
		//else
		{
			MGR_GetNextConnectionRecord(cur, &next);
		}
	}

	
	//if(next && next->linkKey)
	if(flag_present)
	{
		bth_write_paired_device_info(&dev_Info);
		status = 0;
	//	kal_prompt_trace(1,"write  the device info into a file, status:%d (0 indicates succeed)\n", status_tem);
	}
	
	 param_ptr->result = status;
        pMemcpy(param_ptr->linkKey, dev_Info.linkKey, 16);
	param_ptr->KeyType = dev_Info.KeyType;
	

	bonding_cnf = (bt_bm_bonding_cnf_struct *)construct_local_para((kal_uint16)sizeof(bt_bm_bonding_cnf_struct), TD_CTRL); 
        bonding_cnf->result = status; 
        bonding_cnf->bd_addr = param_ptr->bd_addr; 

	rdabt_send_msg_up(MOD_MMI, MSG_ID_BT_BM_BONDING_RESULT_IND, (void *)param_ptr, NULL);
       rdabt_send_msg_up(MOD_MMI, MSG_ID_BT_BM_BE_BONDING_CNF, (void*)bonding_cnf, NULL);//zhou siyou add 200728       

	if(deviceName != NULL)
	{
		pFree(deviceName);
		deviceName = NULL;
		nameLen = 0;
	}

  	 return ;
  
}


//03-28-2009, add by tan 
void rdabt_GetRemoteName_cb(t_bdaddr address, int status)
{

//       t_bdaddr  *addr = (t_bdaddr *)&address;
	   
	if( status == 0 && nameLen > 0)
	{
		bt_bm_pin_code_ind_struct *param_ptr;
       	param_ptr = (bt_bm_pin_code_ind_struct*)construct_local_para((kal_uint16)sizeof(bt_bm_pin_code_ind_struct), TD_CTRL);
		param_ptr->bd_addr.lap= BDADDR_Get_LAP(&address);
		param_ptr->bd_addr.uap = BDADDR_Get_UAP(&address);
		param_ptr->bd_addr.nap = BDADDR_Get_NAP(&address);
		param_ptr->name_len = nameLen;
		memset(param_ptr->name, 0, 47);
		memcpy(param_ptr->name, deviceName, nameLen);
 		mmi_trace(1,"rdabt_GetRemoteName_cb name:%s,nameLen=%d\n", deviceName,nameLen);		
		rdabt_send_msg_up(MOD_MMI, MSG_ID_BT_BM_PIN_CODE_IND, (void *)param_ptr, NULL);

		responsePinCode = 0;
		bonding_addr = address;
		bonding_role = 0;
		pin_code_ind_timer = rdabt_timed_event_in(12*1000*1000, rdabt_InputPin_MoniterTimer_cb, NULL);


	}
	else
	{
		MGR_PINResponse( address, 0 , NULL,  0);
	}
	
}


void rdabt_wait_bonding_MoniterTimer_cb(void)
{
	  
//	bt_bm_bonding_result_ind_struct * result_ind=NULL;
	
	rdabt_being_bonding_cb(bonding_addr, 1);

	if (wait_bonding_timer)
	{
		pTimerCancel(wait_bonding_timer);
		wait_bonding_timer=0;
	}
	
}

void rdabt_accept_conn_cb( t_bdaddr address)
{
	bt_bm_link_connect_accept_ind_struct	  * accept_ind;

	accept_ind = (bt_bm_link_connect_accept_ind_struct*)construct_local_para((kal_uint16)sizeof(bt_bm_link_connect_accept_ind_struct), TD_CTRL);
	accept_ind->bd_addr.lap= BDADDR_Get_LAP(&address);
	accept_ind->bd_addr.uap = BDADDR_Get_UAP(&address);
	accept_ind->bd_addr.nap = BDADDR_Get_NAP(&address);	

	rdabt_send_msg_up(MOD_MMI, MSG_ID_BT_BM_LINK_CONNECT_ACCEPT_IND, (void *)accept_ind, NULL);		
}

struct st_t_BT_ConnectionEntry *prh_mgr_find_deviceEntryByAddress(t_bdaddr addr) ;
u_int32 get_class_of_device(t_bdaddr address)
{	
	u_int32  cod_get=0;
	struct st_t_BT_ConnectionEntry * cur= prh_mgr_find_deviceEntryByAddress(address);
	if(cur)
		cod_get=cur->cod;
	return cod_get;//return 1 current device is hs,else is phone
}
