/***********************************************************************
 *
 * MODULE NAME:    tcs_gm_decode.c
 * PROJECT CODE:   host side stack
 * DESCRIPTION:    TCS Group Management
 * MAINTAINER:     Dave Airlie
 * CREATION DATE:  15 March 2000
 *
 * SOURCE CONTROL: $Id: tcs_gm_decode.c 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2001 Parthus Technologies Inc.
 *     All rights reserved.
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
#include "host_config.h"

#if (PRH_TCS_TERMINAL || PRH_TCS_GATEWAY)

#include "tcs_lower_layer.h"
#include "tcs_core.h"
#include "tcs_gm_types.h"
#include "tcs_wug.h"
#include "tcs_proto.h"

static int prh_tcs_gm_process_listen_accept(struct prh_st_tcs_l2conn *device_info, struct host_buf *p_buf);
static int prh_tcs_gm_process_listen_reject(struct prh_st_tcs_l2conn *device_info, struct host_buf *p_buf);

#if PRH_TCS_TERMINAL
static int prh_tcs_gm_process_access_rights_accept(struct prh_st_tcs_l2conn *device_info, struct host_buf *p_buf);
static int prh_tcs_gm_process_access_rights_reject(struct prh_st_tcs_l2conn *device_info, struct host_buf *p_buf);
static int prh_tcs_gm_process_info_suggest(struct prh_st_tcs_l2conn *device_info, struct host_buf *p_buf);
static int prh_tcs_gm_process_listen_suggest(struct prh_st_tcs_l2conn *device_info, struct host_buf *p_buf);
#endif

#if PRH_TCS_GATEWAY
static int prh_tcs_gm_process_access_rights_req(struct prh_st_tcs_l2conn *device_info, struct host_buf *p_buf);
static int prh_tcs_gm_process_info_accept(struct prh_st_tcs_l2conn *device_info, struct host_buf *p_buf);
static int prh_tcs_gm_process_listen_req(struct prh_st_tcs_l2conn *device_info, struct host_buf *p_buf);
#endif

#if (PRH_TCS_GATEWAY && PRH_TCS_TERMINAL)
static int (*prh_tcs_gm_decode_event[])(struct prh_st_tcs_l2conn *device_info, struct host_buf *p_buf) = 
{
  prh_tcs_gm_process_info_suggest,
  prh_tcs_gm_process_info_accept,
  prh_tcs_gm_process_listen_req,
  prh_tcs_gm_process_listen_accept,
  prh_tcs_gm_process_listen_suggest,
  prh_tcs_gm_process_listen_reject,
  prh_tcs_gm_process_access_rights_req,
  prh_tcs_gm_process_access_rights_accept,
  prh_tcs_gm_process_access_rights_reject,
};
#elif PRH_TCS_GATEWAY
static int (*prh_tcs_gm_decode_event[])(struct prh_st_tcs_l2conn *device_info, struct host_buf *p_buf) = 
{
  NULL,
  prh_tcs_gm_process_info_accept,
  prh_tcs_gm_process_listen_req,
  prh_tcs_gm_process_listen_accept,
  NULL,
  prh_tcs_gm_process_listen_reject,
  prh_tcs_gm_process_access_rights_req,
  NULL,
  NULL
};
#elif PRH_TCS_TERMINAL
static int (*prh_tcs_gm_decode_event[])(struct prh_st_tcs_l2conn *device_info, struct host_buf *p_buf) = 
{
  prh_tcs_gm_process_info_suggest,
  NULL, 
  NULL,
  prh_tcs_gm_process_listen_accept,
  prh_tcs_gm_process_listen_suggest,
  prh_tcs_gm_process_listen_reject,
  NULL,
  prh_tcs_gm_process_access_rights_accept,
  prh_tcs_gm_process_access_rights_reject,
};
#endif


#define TCS_GM_CHECK_FOR_MORE_IE(ch_ptr, p_buf) (((unsigned)(ch_ptr-p_buf->data)>p_buf->len))

int prh_tcs_gm_process_event(struct prh_st_tcs_l2conn *device_info, struct host_buf *p_buf)
{
  u_int8 opcode=p_buf->data[0] & ~PRH_TCS_PROTO_DISC_GM;

  if (opcode > PRH_TCS_MSG_ACCESS_RIGHTS_REJECT)
    pDebugPrintfEX((pLOGERROR,pLOGTCS,"TCS: Illegal Msg in gm_process_event %d\n", opcode));
  
  if (prh_tcs_gm_decode_event[opcode])
    return (*prh_tcs_gm_decode_event[opcode])(device_info, p_buf);
  else
    return BT_INVALIDPARAM;
}

static pINLINEFUNC int  prh_tcs_gm_decode_ie_company_specific(u_int8 **data_dptr, u_int16 *company_id)
{
  u_int8 len;
  u_int8 *data_ptr=*data_dptr;
  
  data_ptr++;
  len=*data_ptr++;
  
  pDebugPrintfEX((pLOGDEBUG,pLOGTCS, "TCS GM: CS IE, %d bytes\n", len));
  *company_id=BT_READ_LE_UINT16(data_ptr);
  data_ptr+=len;
  *data_dptr=data_ptr;
  return BT_NOERROR;
}

static pINLINEFUNC int prh_tcs_gm_decode_cause(u_int8 **data_dptr, u_int8 *cause)
{
  (*data_dptr)++;
  *cause=**data_dptr;
  (*data_dptr)++;
  return BT_NOERROR;
}

static pINLINEFUNC int prh_tcs_gm_decode_clock_offset(u_int8 **data_dptr, u_int16 *clock_offset)
{
  u_int8 len;
  (*data_dptr)++;
  len=**data_dptr;
  if (len!=2)
    {
      *data_dptr+=len;
      return BT_INVALIDPARAM;
    }
  *clock_offset=BT_READ_LE_UINT16(*data_dptr);
  *data_dptr+=2;
  return BT_NOERROR;
}

static int prh_tcs_gm_decode_called_party_number(u_int8 **data_dptr, struct prh_st_tcs_gm_called_party_number *tcs_cpn_ret)
{
  u_int8 len;
  u_int8 *data_ptr=*data_dptr;
  
  /* Skip over IE byte */
  ++data_ptr;

  len=*data_ptr;
  ++data_ptr;
  if (len==0)
    return BT_INVALIDPARAM;

  tcs_cpn_ret->type=(*data_ptr & 0x70)>>4;
  tcs_cpn_ret->plan_id=(*data_ptr) & 0x0F;
  ++data_ptr;
  
  tcs_cpn_ret->num_len=len-1;

  tcs_cpn_ret->num=(u_int8 *)pMalloc(len-1);
  pMemcpy(tcs_cpn_ret->num, data_ptr, len-1);

  data_ptr+=len-1;
  *data_dptr=data_ptr;
  return BT_NOERROR;
}

static int prh_tcs_gm_decode_ie_config_data(u_int8 **data_dptr, struct prh_st_tcs_gm_config_data **config_data, u_int8 *num_members)
{
  u_int8 *data_ptr=*data_dptr;
  u_int8 info_len;
  u_int8 num_wugmembers;
  struct prh_st_tcs_gm_config_data *config_array;
  int i;

  data_ptr++;
  info_len=*data_ptr++;
  if (info_len==0)
    return BT_INVALIDPARAM;
  
  pDebugPrintfEX((pLOGDEBUG,pLOGTCS, "Info length is %d\n", info_len));
  num_wugmembers=info_len / 24; 

  if (num_wugmembers>0)
    {
      config_array=(struct prh_st_tcs_gm_config_data *)pMalloc(sizeof(struct prh_st_tcs_gm_config_data)*num_wugmembers);

      for (i=0; i<num_wugmembers; i++)
	{
	  config_array[i].internal_num[0]=*data_ptr++;
	  config_array[i].internal_num[1]=*data_ptr++;

	  pMemcpy(config_array[i].bd_addr.bytes, data_ptr, 6);
	  data_ptr+=6;
	  pMemcpy(config_array[i].link_key, data_ptr, 16);
	  data_ptr+=16;
	}

      *num_members=num_wugmembers;
      *config_data=config_array;
      *data_dptr=data_ptr;
      /*      prh_tcs_wug_put_info_into_db(config_array, *num_members);*/
      return BT_NOERROR;
    }
  else
    return BT_INVALIDPARAM;
}

static int prh_tcs_gm_process_info_suggest(struct prh_st_tcs_l2conn *device_info, struct host_buf *p_buf)
{
  struct prh_st_tcs_gm_config_data *config_data;
  u_int8 num_members;
  int ret;
  u_int8 *ch_ptr=p_buf->data;
  u_int16 comp_id;
  struct prh_st_tcs_gm_encode_info accept_params;

  pDebugPrintfEX((pLOGDEBUG,pLOGTCS, "TCS: got info suggest\n"));
  if (p_buf->len<2)
    {
      pDebugPrintfEX((pLOGERROR,pLOGTCS, "TCS: GM: Info Suggest received bad, %d\n", p_buf->len));
      return BT_INVALIDPARAM;
    }
  
  ch_ptr++;
  ret=prh_tcs_gm_decode_ie_config_data(&ch_ptr, &config_data, &num_members);
  if (ret==BT_NOERROR)
    {
      if (TCS_GM_CHECK_FOR_MORE_IE(ch_ptr, p_buf))
	{
	  ret=prh_tcs_gm_decode_ie_company_specific(&ch_ptr, &comp_id);
	  if (ret==BT_NOERROR)
	    {
	      pDebugPrintfEX((pLOGNOTICE, pLOGTCS,"TCS: GM: Got Company Specific, company ID=%d\n", comp_id));
	    }
	}
      
      /* send up to application the info stuff?? */
      prh_tcs_wug_put_info_into_db(config_data, num_members);
      pMemset(&accept_params, 0, sizeof(struct prh_st_tcs_gm_encode_info));
      prh_tcs_gm_encode_info_accept(&accept_params, device_info);      
      
    }
  else
    return BT_INVALIDPARAM;
  return BT_NOERROR;
}

static int prh_tcs_gm_process_info_accept(struct prh_st_tcs_l2conn *device_info, struct host_buf *p_buf)
{
  u_int8 *ch_ptr=p_buf->data;
  u_int16 comp_id;

  if (p_buf->len>1)
    {
      ch_ptr++;
      
      /* Decode the Company spec IE */
      if (*ch_ptr!=PRH_TCS_IE_VL_COMPANY_SPECIFIC)
	{
	  pDebugPrintfEX((pLOGERROR,pLOGTCS,"TCS: Illegal IE in GM Access Rights Req %d\n", *ch_ptr));
	}
      
      prh_tcs_gm_decode_ie_company_specific(&ch_ptr, &comp_id);
      
    }
  /* Do an INFO ACCEPT */
  return prh_tcs_wug_handle_info_accept(device_info);   

}

static int prh_tcs_gm_process_listen_req(struct prh_st_tcs_l2conn *device_info, struct host_buf *p_buf)
{
  struct prh_st_tcs_gm_called_party_number cpn_num;
  u_int8 *ch_ptr=p_buf->data;
  u_int16 comp_id;
  u_int8 msg_good=0;

  ch_ptr++;
  /* get called party number */
  if (*ch_ptr==PRH_TCS_IE_VL_CALLED_PARTY_NUM)
    {
      msg_good=1;
      prh_tcs_gm_decode_called_party_number(&ch_ptr, &cpn_num);
    }
  
  if (TCS_GM_CHECK_FOR_MORE_IE(ch_ptr, p_buf))
    {
      /* Get company specific */
      if (*ch_ptr==PRH_TCS_IE_VL_COMPANY_SPECIFIC)
	prh_tcs_gm_decode_ie_company_specific(&ch_ptr, &comp_id);
    }

  if (msg_good)
    prh_tcs_wug_handle_listen_request(device_info, &cpn_num);
  return BT_NOERROR;
}

static int prh_tcs_gm_process_listen_accept(struct prh_st_tcs_l2conn *device_info, struct host_buf *p_buf)
{
  u_int8 *ch_ptr=p_buf->data;
  u_int16 clock_offset;
  u_int16 comp_id;
  ch_ptr++;

  if (p_buf->len>1)
    {
      if (*ch_ptr==PRH_TCS_IE_VL_CLOCK_OFFSET)
	prh_tcs_gm_decode_clock_offset(&ch_ptr, &clock_offset);
      
      if (TCS_GM_CHECK_FOR_MORE_IE(ch_ptr, p_buf))
	{
	    /* Get company specific */
	  if (*ch_ptr==PRH_TCS_IE_VL_COMPANY_SPECIFIC)
	    prh_tcs_gm_decode_ie_company_specific(&ch_ptr, &comp_id);
	}
    }

  /* Do a Listen Accept */
  prh_tcs_wug_handle_listen_accept(device_info);  
  return BT_NOERROR;
}

static int prh_tcs_gm_process_listen_suggest(struct prh_st_tcs_l2conn *device_info, struct host_buf *p_buf)
{
  u_int8 *ch_ptr=p_buf->data;
  u_int16 comp_id;
  
  ch_ptr++;
  if (p_buf->len>1)
    {
      
      /* Decode the Company spec IE */
      if (*ch_ptr!=PRH_TCS_IE_VL_COMPANY_SPECIFIC)
	{
	  pDebugPrintfEX((pLOGDEBUG,pLOGTCS, "TCS: Illegal IE in GM Access Rights Req %d\n", *ch_ptr));
	}
	
      prh_tcs_gm_decode_ie_company_specific(&ch_ptr, &comp_id);

    }
  /* Do an LISTEN SUGGEST */
  prh_tcs_wug_handle_listen_suggest(device_info);
  return BT_NOERROR;
}

static int prh_tcs_gm_process_listen_reject(struct prh_st_tcs_l2conn *device_info, struct host_buf *p_buf)
{
  u_int8 *ch_ptr=p_buf->data;

  u_int16 comp_id;
  u_int8 cause;

  ch_ptr++;
  if (p_buf->len>1)
    {
      /* Check for a Cause */
      if (*ch_ptr==PRH_TCS_IE_DO_CAUSE)
	prh_tcs_gm_decode_cause(&ch_ptr, &cause);
      
      if (TCS_GM_CHECK_FOR_MORE_IE(ch_ptr, p_buf))
	{
	  if (*ch_ptr==PRH_TCS_IE_VL_COMPANY_SPECIFIC)
	    prh_tcs_gm_decode_ie_company_specific(&ch_ptr, &comp_id);
	}
      
    }
  prh_tcs_wug_handle_listen_reject(device_info, cause);
  return BT_NOERROR;
}

/*
 * prh_tcs_gm_process_access_rights_req
 * 
 * Access Rights request make contain an optional company speific IE
 */
#if PRH_TCS_GATEWAY
static int prh_tcs_gm_process_access_rights_req(struct prh_st_tcs_l2conn *device_info, struct host_buf *p_buf)
{
  u_int8 *ch_ptr=p_buf->data;
  u_int16 comp_id;  
  int ret;
  ch_ptr++;
  if (p_buf->len>1)
    {
      /* Decode the Company spec IE */
      if (*ch_ptr!=PRH_TCS_IE_VL_COMPANY_SPECIFIC)
	{
	  pDebugPrintfEX((pLOGERROR,pLOGTCS, "TCS: Illegal IE in GM Access Rights Req %d\n", *ch_ptr));
	}
	
      prh_tcs_gm_decode_ie_company_specific(&ch_ptr, &comp_id);

    }
  /* Do an ACCESS RIGHTS REQ */
 ret=prh_tcs_wug_handle_access_rights_request(device_info);
  
  return BT_NOERROR;
}
#endif

#if PRH_TCS_TERMINAL
static int prh_tcs_gm_process_access_rights_accept(struct prh_st_tcs_l2conn *device_info, struct host_buf *p_buf)
{
  u_int8 *ch_ptr=p_buf->data;
  u_int16 comp_id;
  int ret;

  ch_ptr++;
  if (p_buf->len>1)
    {
      /* Decode the Company spec IE */
      if (*ch_ptr!=PRH_TCS_IE_VL_COMPANY_SPECIFIC)
	{
	  pDebugPrintfEX((pLOGERROR,pLOGTCS, "TCS: Illegal IE in GM Access Rights Req %d\n", *ch_ptr));
	}
	
      prh_tcs_gm_decode_ie_company_specific(&ch_ptr, &comp_id);

    }
  /* Do an ACCESS RIGHTS ACCEPT */
  ret=prh_tcs_wug_handle_access_rights_accept(device_info);
  return BT_NOERROR;
}
#endif

#if PRH_TCS_TERMINAL
static int prh_tcs_gm_process_access_rights_reject(struct prh_st_tcs_l2conn *device_info, struct host_buf *p_buf)
{
  u_int8 *ch_ptr=p_buf->data;
  u_int16 comp_id;
  int ret;

  ch_ptr++;
  if (p_buf->len>1)
    {
      /* Decode the Company spec IE */
      if (*ch_ptr!=PRH_TCS_IE_VL_COMPANY_SPECIFIC)
	{
	  pDebugPrintfEX((pLOGERROR,pLOGTCS, "TCS: Illegal IE in GM Access Rights Req %d\n", *ch_ptr));
	}
	
      prh_tcs_gm_decode_ie_company_specific(&ch_ptr, &comp_id);

    }

  /* Do an ACCESS RIGHTS REJECT*/
  ret=prh_tcs_wug_handle_access_rights_reject(device_info);
  return BT_NOERROR;
}
#endif
#endif /*(PRH_TCS_TERMINAL || PRH_TCS_GATEWAY)*/
