 /**************************************************************************
 * MODULE NAME:    l2_config_helper.c
 * PROJECT CODE:
 * DESCRIPTION:    l2cap channel configuration state machine
 * AUTHOR:         Dave Airlie
 * DATE:           02-08-2001
 *
 * SOURCE CONTROL: $Id: l2_config_helper.c 2720 2010-10-11 07:15:34Z jiancui $
 *
 * LICENSE
 *     This source code is copyright (c) 2000-2002 Parthus Technologies Plc.
 *     All rights reserved.
 *
 *************************************************************************/
#include "host_config.h"

#if (PRH_L2CAP_AUTO_CONFIG==1)
#include "host_types.h"
#include "host_error.h"
#include "host_ll.h"

#include "l2_types.h"
#include "l2_const.h"
#include "l2_enc_dec.h"
#include "l2cap_primitives.h"
#include "l2_psm.h"
#include "l2_channel.h"
#include "l2_config_helper.h"

t_api L2_Connect_Ex(t_bdaddr bdaddr, u_int16 psm, u_int16 *cid_p);

typedef enum {
	PRH_CONFIG_FINISH,
	PRH_CONN_INIT,
	PRH_CONN_INITFINISHIND,
	PRH_CONN_INITFINISHCFM,
	PRH_CONN_INITFINISHBOTH,
	PRH_CONN_NONINIT,
	PRH_CONN_NONINITFINISHIND,
	PRH_CONN_NONINITFINISHCFM,
	PRH_CONN_NONINITFINISHBOTH,
	PRH_CONN_INCONNECT
} PRH_CONFIG_STATE;

struct prh_l2cap_config_helper_connection {
	u_int16 cid;
	t_bdaddr bdaddr;
	u_int16 psm;
	
	/* Last agreed parameters */
	u_int16 inMTU;
	u_int16 outMTU;
	u_int16 flushTO;
	
	/* parameters requested by this side */
	t_L2_ConfigReqParams req_params;
	
	PRH_CONFIG_STATE config_state;
#if COMBINED_HOST==1
} new_conn;
#else
};
#endif

int prh_l2cap_config_helper_add_connection(struct prh_l2cap_config_helper_connection *new_conn);

int prh_l2cap_config_helper_lookup_connection(u_int16 cid, struct prh_l2cap_config_helper_connection **new_conn);
int prh_l2cap_config_helper_remove_connection(u_int16 cid, struct prh_l2cap_config_helper_connection **new_conn);

APIDECL1 t_api APIDECL2 L2_Connect_internal(t_bdaddr bdaddr, u_int16 psm, u_int16 *cid_p);
int prh_l2cap_config_helper_add_connection(struct prh_l2cap_config_helper_connection *new_conn);
int prh_l2cap_config_helper_lookup_connection(u_int16 cid, struct prh_l2cap_config_helper_connection **new_conn);

struct prh_host_gen_ll *prh_l2cap_config_helper_list;
t_pMutex prh_l2cap_config_helper_list_lock;


/* L2_ConnectionConfigured(u_int16 inMTU, u_int16 outMTU, u_int16 FlushTO); */
int prh_l2_config_helper_startup(u_int32 flags)
{
#if COMBINED_HOST==0
	prh_l2cap_config_helper_list_lock=pMutexCreate(0);
#endif
	return BT_NOERROR;
}

int prh_l2_config_helper_shutdown(u_int32 flags)
{
	prh_l2cap_config_helper_remove_all_connections();
#if COMBINED_HOST==0
	pMutexFree(prh_l2cap_config_helper_list_lock);
#endif
	return BT_NOERROR;
}

/*****************************************************************************
 * Function :- L2_ConnectandConfig
 *
 * Description 
 * This allows the upper layer to use one call to initiate an outgoing
 * connection and initiate configuration.
 *****************************************************************************/

APIDECL1 t_api APIDECL2 L2_ConnectandConfig(t_bdaddr bdaddr, u_int16 psm, t_L2_ConfigReqParams *req_params)
{
	/* need linked list of connections for this bdaddr,psm */
#if COMBINED_HOST==0
	
	struct prh_l2cap_config_helper_connection *new_conn;
	int ret;
	new_conn=(struct prh_l2cap_config_helper_connection *)pMalloc(sizeof(struct prh_l2cap_config_helper_connection));
    	pDebugPrintfEX((pLOGNOTICE,pLOGL2CAP,"L2_ConnectandConfig --- new_conn 1 address = %p\n",new_conn));
	if (new_conn==NULL)
		return BT_NORESOURCES;
	new_conn->psm=psm;
	new_conn->bdaddr=bdaddr;
	new_conn->req_params=*req_params;
	new_conn->config_state=PRH_CONN_INIT;
	
	ret=prh_l2cap_config_helper_add_connection(new_conn);
	pDebugPrintfEX((pLOGNOTICE,pLOGL2CAP,"L2_ConnectandConfig --- new_conn 2 address = %p; ret = %d\n",new_conn,ret));
	if (ret!=BT_NOERROR)
    {
            if(new_conn)
            {
                pFree(new_conn);
                new_conn=NULL;
            }
		return BT_NORESOURCES;
    }
	
	ret=L2_Connect_Ex(bdaddr, psm, &new_conn->cid);
	pDebugPrintfEX((pLOGNOTICE,pLOGL2CAP,"L2_ConnectandConfig --- new_conn 3 address = %p; ret = %d\n",new_conn,ret));
	if ((ret == BT_NOERROR) || (ret == BT_PENDING))
	{
		return ret;
	}
	else
	{ /* Error on the L2_Connect - release connection */
	    	ret = prh_l2cap_config_helper_remove_connection(new_conn->cid,&new_conn);
		pDebugPrintfEX((pLOGNOTICE,pLOGL2CAP,"L2_ConnectandConfig --- new_conn 4 address = %p; ret = %d\n",new_conn,ret));
		if(!ret && new_conn)
              {		
                  pFree(new_conn);
                  new_conn=NULL;
             }
		return BT_NORESOURCES;
	}
#else
	
	int ret;
	new_conn.psm=psm;
	new_conn.bdaddr=bdaddr;
	new_conn.req_params=*req_params;
	new_conn.config_state=PRH_CONN_INIT;
	
	ret=prh_l2cap_config_helper_add_connection(&new_conn);
	if (ret!=BT_NOERROR)
    {
		return BT_NORESOURCES;
    }
	
	ret=L2_Connect_Ex(bdaddr, psm, &new_conn.cid);

	return ret;
#endif

}

/*****************************************************************************
 * Function :- L2_ConnectResponseandConfig
 *
 * Description 
 * This allows the upper layer to use one call to accept an incoming
 * connection and initiate configuration.
 *****************************************************************************/

APIDECL1 t_api APIDECL2 L2_ConnectResponseandConfig(t_bdaddr bdaddr, u_int16 cid, u_int16 response, u_int16 status, t_L2_ConfigReqParams *req_params)
{
#if COMBINED_HOST==0
	struct prh_l2cap_config_helper_connection *new_conn;
	int ret;
	prh_t_l2_channel *p_channel;
	
	new_conn=(struct prh_l2cap_config_helper_connection *)pMalloc(sizeof(struct prh_l2cap_config_helper_connection));
	if (new_conn==NULL)
		return BT_NORESOURCES;
	
	p_channel=prh_l2_chn_get_p_channel(cid);
       if(p_channel)
	new_conn->psm=p_channel->psm;
       else
   		return BT_NORESOURCES;
	new_conn->cid=cid;
	new_conn->req_params=*req_params;
	new_conn->config_state=PRH_CONN_NONINIT;
	new_conn->bdaddr=bdaddr;
	
	ret=prh_l2cap_config_helper_add_connection(new_conn);
#else
	int ret;
	prh_t_l2_channel *p_channel;
	
	p_channel=prh_l2_chn_get_p_channel(cid);
       if(p_channel)
	new_conn.psm=p_channel->psm;
       else
     		return BT_NORESOURCES;
	new_conn.cid=cid;
	new_conn.req_params=*req_params;
	new_conn.config_state=PRH_CONN_NONINIT;
	new_conn.bdaddr=bdaddr;
	
	ret=prh_l2cap_config_helper_add_connection(&new_conn);
#endif
	if (ret!=BT_NOERROR)
    {
#if COMBINED_HOST==0
                if(new_conn)
                {
                    pFree(new_conn);
                    new_conn=NULL;
                }
#endif
		return BT_NORESOURCES;
    }
	
	ret=L2_ConnectResponse(bdaddr, cid, response, status);
	if (ret==BT_NOERROR)
    {
		if (response==0)
		{
			ret=L2_Configure(cid, req_params->inMTUmax, req_params->flushTO, req_params->linkTO, NULL);
		}
    }
	return ret;
}


/*****************************************************************************
 * Function :- prh_l2_configure_helper_conn_confirm
 *
 * Description 
 * This handles a Connect_Confirm from the l2cap layer. This event is used to trigger configuration
 * of the l2cap channel.
 *****************************************************************************/

void prh_l2_configure_helper_conn_confirm(u_int8 eventType,t_L2_ControlCallbackArgs *args)
{
	struct prh_l2cap_config_helper_connection *conn;
	int ret;
	t_L2_ControlCallbackArgs  eventinfo;
	u_int16 psm_index;
	/* lookup connection structure somewhere */
	pDebugPrintfEX((pLOGDEBUG,pLOGL2CAP,"prh_l2_configure_helper_conn_confirm"));
	ret=prh_l2cap_config_helper_lookup_connection(args->cid, &conn);
    pDebugPrintfEX((pLOGDEBUG,pLOGL2CAP,"prh_l2_configure_helper_conn_confirm ret=%d,args->cid=0x%x,args->result=%d",ret,args->cid,args->result));
	if (ret!=BT_NOERROR)
		return;
	
	if (args->result==L2CAP_CONNECTION_SUCCESSFUL)
    {
		/* need to do the configuration process from here now */
		
		ret=L2_Configure(args->cid, conn->req_params.inMTUmax, conn->req_params.flushTO, conn->req_params.linkTO, NULL);
		if (ret!=BT_NOERROR)
			return;
    }
	else
    {
		/* finished callback application */
		eventinfo.psm=conn->psm;
		eventinfo.BDAddress = conn->bdaddr;
		eventinfo.cid=conn->cid;
		eventinfo.result=args->result;
		eventinfo.status=args->status;
		psm_index = prh_l2_psm_get_psm_ident(conn->psm);
		if(args->result != L2CAP_CONNECTION_PENDING)
		    prh_l2_config_helper_remove_cid(conn->cid);
		prh_l2_psm_event(psm_index, L2CAP_evCONNCONFIGUREDCONF, &eventinfo);
    }
	
} 

/*****************************************************************************
 * Function :- prh_l2_configure_helper_indication
 *
 * Description 
 * This handles a Configuration_Indication from the l2cap layer. Seperate configuration of
 * both the incoming and outgoing data paths has to be performed before configuration is complete.
 * 
 * Each peer on the link is responsible for successfully configuring their own incoming 
 * data path. Thus to reach completion of configuration each side had to successfull complete
 * a configuration initiated by itself and a configuration configured by the peer.
 *****************************************************************************/

void prh_l2_configure_helper_indication(u_int8 eventType,t_L2_ControlCallbackArgs *args)
{
	t_L2_ControlCallbackArgs  eventinfo;
	PRH_CONFIG_STATE old_state;
	struct prh_l2cap_config_helper_connection *conn;
	int ret;
	u_int16 response;
	u_int16 psm_index;
	u_int16 out_mtu;
	/* lookup up the configuration */
    pDebugPrintfEX((pLOGDEBUG,pLOGL2CAP,"prh_l2_configure_helper_indication"));
	ret=prh_l2cap_config_helper_lookup_connection(args->cid, &conn);
	if (ret!=BT_NOERROR)
		return;
	
	old_state = conn->config_state;
pDebugPrintfEX((pLOGDEBUG,pLOGL2CAP,"prh_l2_configure_helper_indication ret=%d,cid=0x%x,result=%d, config_state:%d",ret,args->cid,args->result,conn->config_state));
	/* setup the next state */
	switch(conn->config_state)
    {
    case PRH_CONN_INIT:
		conn->config_state=PRH_CONN_INITFINISHIND;
		break;
    case PRH_CONN_INITFINISHCFM:
		conn->config_state=PRH_CONN_INITFINISHBOTH;
		break;
    case PRH_CONN_NONINIT:
		conn->config_state=PRH_CONN_NONINITFINISHIND;
		break;
    case PRH_CONN_NONINITFINISHCFM:
		conn->config_state=PRH_CONN_NONINITFINISHBOTH;
		break;
    case PRH_CONN_INITFINISHBOTH:
    case PRH_CONN_NONINITFINISHBOTH:
		conn->config_state=PRH_CONN_INCONNECT;	
		ret=L2_Configure(conn->cid, conn->req_params.inMTUmax, conn->req_params.flushTO, conn->req_params.linkTO, NULL);
		break;
    default:
		
		break;
    }
	
	/* check peers requested MTU vs our storage */
	out_mtu=args->mtu;
	response=L2CAP_CONFIG_SUCCESSFUL;

	if ((conn->req_params.outMTUmax != 0) && (out_mtu > conn->req_params.outMTUmax))
    {
		out_mtu=conn->req_params.outMTUmax;
		/* I don't believe this is UNACCEPTABLE_PARAMS 
		 * according to specification - D.A. upf7 */
/*	response=L2CAP_CONFIG_FAILURE_UNACCEPTABLE_PARAMS;*/
    }
	else if ((conn->req_params.outMTUmin != 0) && (conn->req_params.outMTUmin > out_mtu))
	{
		if( out_mtu !=0)
		{
#if COMBINED_HOST==0
		response=L2CAP_CONFIG_FAILURE_UNACCEPTABLE_PARAMS;
		conn->config_state = old_state; /* move the state back */
#endif
		}
		out_mtu = conn->req_params.outMTUmin;
	}
	
	conn->outMTU=out_mtu;
	
	L2_ConfigureResponse(conn->cid, out_mtu, response, NULL);
	
	if (conn->config_state==PRH_CONN_INITFINISHBOTH || conn->config_state==PRH_CONN_NONINITFINISHBOTH)
	{
		/* finished callback application */
		eventinfo.psm=conn->psm;
		eventinfo.BDAddress = conn->bdaddr;
		eventinfo.mtu = conn->outMTU;
		eventinfo.mtu_in = conn->inMTU;
		eventinfo.cid=conn->cid;
		eventinfo.flushTimeout=conn->flushTO;
		eventinfo.flow=NULL;
		eventinfo.result=0;
		psm_index = prh_l2_psm_get_psm_ident(conn->psm);
		prh_l2_psm_event(psm_index, L2CAP_evCONNCONFIGUREDCONF, &eventinfo);
	}
	
}

/*****************************************************************************
 * Function :- prh_l2_configure_helper_confirmation
 *
 * Description 
 * This handles a Configuration_Confirmation from the l2cap layer. Seperate configuration of
 * both the incoming and outgoing data paths has to be performed before configuration is complete.
 * 
 * Each peer on the link is responsible for successfully configuring their own incoming 
 * data path. Thus to reach completion of configuration each side had to successfull complete
 * a configuration initiated by itself and a configuration configured by the peer.
 *****************************************************************************/

void prh_l2_configure_helper_confirmation(u_int8 eventType,t_L2_ControlCallbackArgs *args)
{
	t_L2_ControlCallbackArgs  eventinfo;
	struct prh_l2cap_config_helper_connection *conn;
	u_int16 psm_index;
	int ret;
	/* lookup up the configuration */
	ret=prh_l2cap_config_helper_lookup_connection(args->cid, &conn);
	if (ret!=BT_NOERROR)
		return;
	
	/* if we got an unacceptable parameters cool deal with it */
    pDebugPrintfEX((pLOGDEBUG,pLOGL2CAP,"prh_l2_configure_helper_confirmation ret=%d,cid=0x%x,result=%d, config_state:%d",ret,args->cid,args->result,conn->config_state));
	switch(args->result)
    {
    case L2CAP_CONFIG_SUCCESSFUL :
		/* setup the next state */
		switch(conn->config_state)
		{
		case PRH_CONN_INIT:
			conn->config_state=PRH_CONN_INITFINISHCFM;
			break;
		case PRH_CONN_INITFINISHIND:
			conn->config_state=PRH_CONN_INITFINISHBOTH;
			break;
		case PRH_CONN_NONINIT:
			conn->config_state=PRH_CONN_NONINITFINISHCFM;
			break;
		case PRH_CONN_NONINITFINISHIND:
			conn->config_state=PRH_CONN_NONINITFINISHBOTH;
			break;
		case PRH_CONN_INCONNECT:
			
		default:
			break;
		} /* End of switch(conn->config_state) */
		
		conn->flushTO=args->flushTimeout;
		conn->inMTU=args->mtu;
		
		if (conn->config_state==PRH_CONN_INITFINISHBOTH || conn->config_state==PRH_CONN_NONINITFINISHBOTH)
		{
			/* finished callback application */
			eventinfo.psm=conn->psm;
			eventinfo.BDAddress = conn->bdaddr;
			eventinfo.mtu = conn->outMTU;
			eventinfo.mtu_in = conn->inMTU;
			eventinfo.cid=conn->cid;
			eventinfo.flushTimeout=conn->flushTO;
			eventinfo.flow=NULL;
			eventinfo.result=0;
			psm_index = prh_l2_psm_get_psm_ident(conn->psm);
			prh_l2_psm_event(psm_index, L2CAP_evCONNCONFIGUREDCONF, &eventinfo); 
		}
		break; /* end of case L2CAP_CONFIG_SUCCESSFUL  */

	case L2CAP_CONFIG_FAILURE_UNACCEPTABLE_PARAMS :
		ret=L2_Configure(args->cid, args->mtu, args->flushTimeout, conn->req_params.linkTO, NULL);
		break;

	case L2CAP_CONFIG_FAILURE_REJECTED :
	case L2CAP_CONFIG_FAILURE_UNKNOWN_OPTIONS :
		/* do what? */
		break;
    } /* End of switch(args->result) */
}

int prh_l2_config_helper_remove_cid(u_int16 cid)
{
	struct prh_l2cap_config_helper_connection *rem_conn;
	int ret;
	ret=prh_l2cap_config_helper_remove_connection(cid, &rem_conn);
#if COMBINED_HOST==0
	if (ret==BT_NOERROR)
        {
            if(rem_conn)
            {
                pFree(rem_conn);
                rem_conn=NULL;
            }
       }
#endif
	return ret;
}


int prh_l2cap_config_helper_add_connection(struct prh_l2cap_config_helper_connection *new_conn)
{
	int ret;
	pMutexLock(prh_l2cap_config_helper_list_lock);
	ret=prh_host_ll_add(&prh_l2cap_config_helper_list, (void *)new_conn);
	pMutexUnlock(prh_l2cap_config_helper_list_lock);
	return ret;
}

static int prh_l2cap_config_helper_cid_cmp(void *cmpcid, void *compstruct)
{
	struct prh_l2cap_config_helper_connection *conn=(struct prh_l2cap_config_helper_connection *)compstruct;
	u_int16 cid = (u_int16)(u_int32)cmpcid;
	
	return (conn->cid==cid);
}

/*****************************************************************************************
 * Function :- prh_l2cap_config_helper_lookup_connection
 *
 * Description
 * retrieves a pointer to a structure describing an l2cap connection identified by its CID
 *****************************************************************************************/
int  prh_l2cap_config_helper_lookup_connection(u_int16 cid, struct prh_l2cap_config_helper_connection **new_conn)
{
	int ret;
	pMutexLock(prh_l2cap_config_helper_list_lock);
	ret=prh_host_ll_search(&prh_l2cap_config_helper_list, (void **)new_conn, (void *)(u_int32)cid, &prh_l2cap_config_helper_cid_cmp);
	pMutexUnlock(prh_l2cap_config_helper_list_lock);
	return ret;
}


/*****************************************************************************************
 * Function :- prh_l2cap_config_helper_remove_connection
 *
 * Description
 * removes the structure describing an l2cap connection identified by its CID
 *****************************************************************************************/

int prh_l2cap_config_helper_remove_connection(u_int16 cid, struct prh_l2cap_config_helper_connection **new_conn)
{
	int ret;
	pMutexLock(prh_l2cap_config_helper_list_lock);
	ret=prh_host_ll_remove(&prh_l2cap_config_helper_list, (void **)new_conn, (void *)(u_int32)cid, &prh_l2cap_config_helper_cid_cmp);
	pMutexUnlock(prh_l2cap_config_helper_list_lock);
	return ret;
}

static int prh_l2cap_config_remove_all_inst_helper(void *data, void *func_info)
{
#if COMBINED_HOST==0
	struct prh_l2cap_config_helper_connection *conn=(struct prh_l2cap_config_helper_connection *)data;
        if(conn)
        {
            pFree(conn);
            conn=NULL;
        }
#endif
	return BT_NOERROR;
}

/*****************************************************************************************
 * Function :- prh_l2cap_config_helper_remove_all_connections
 *
 * Description
 * 
 *****************************************************************************************/

int prh_l2cap_config_helper_remove_all_connections(void)
{
	int ret;
	pMutexLock(prh_l2cap_config_helper_list_lock);
	ret=prh_host_ll_remove_all(&(prh_l2cap_config_helper_list), &prh_l2cap_config_remove_all_inst_helper, NULL);
	pMutexUnlock(prh_l2cap_config_helper_list_lock);
	return ret;
}
#endif
