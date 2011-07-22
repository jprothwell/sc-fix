/***********************************************************************
 *
 * MODULE NAME:    tcs_gm_encode.c
 * PROJECT CODE:   host side stack
 * DESCRIPTION:    TCS Group Management
 * MAINTAINER:     Dave Airlie
 * CREATION DATE:  15 March 2000
 *
 * SOURCE CONTROL: $Id: tcs_gm_encode.c 1532 2010-08-23 05:47:34Z huazeng $
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

struct host_buf *prh_tcs_gm_get_l2cap_buf(u_int16 cid, u_int32 length)
{

  int ret;
  struct host_buf *p_buf;
  
  ret=prh_l2_GetWriteBuffer(cid, (u_int16)length, 0, &p_buf);
  if (ret==BT_NOERROR)
    return p_buf;
  return NULL;

}

static int prh_tcs_gm_encode_comp_spec_only(struct prh_st_tcs_gm_encode_info *info, struct prh_st_tcs_l2conn *l2_conn, u_int8 msg_type)
{
  u_int16 buf_len=1;
  struct host_buf *p_buf;
  u_int8 *ch_ptr;

  if (info->compspec)
    {
      buf_len+=PRH_TCS_IE_COMP_SPEC_LEN;
    }
  
  p_buf=prh_tcs_gm_get_l2cap_buf(l2_conn->cid, buf_len);
  
  if (!p_buf)
    return BT_NORESOURCES;

  ch_ptr=p_buf->data;
  *ch_ptr=msg_type | PRH_TCS_PROTO_DISC_GM;
  
  ch_ptr++;
  if (info->compspec)
    {
      *ch_ptr++=PRH_TCS_IE_VL_COMPANY_SPECIFIC;
      *ch_ptr++=PRH_TCS_IE_COMP_SPEC_LEN;
      BT_WRITE_LE_UINT16(ch_ptr, info->compspec->comp_id);
    }
  p_buf->len = buf_len;
  L2_Write(l2_conn->cid, (t_DataBuf *)p_buf);
  return BT_NOERROR;
}

#if PRH_TCS_TERMINAL
int prh_tcs_gm_encode_info_accept(struct prh_st_tcs_gm_encode_info *info, struct prh_st_tcs_l2conn *l2_conn)
{
  return prh_tcs_gm_encode_comp_spec_only(info, l2_conn, PRH_TCS_MSG_INFO_ACCEPT);
}
#endif

#if PRH_TCS_GATEWAY
int prh_tcs_gm_encode_listen_suggest(struct prh_st_tcs_gm_encode_info *info, struct prh_st_tcs_l2conn *l2_conn)
{
  return prh_tcs_gm_encode_comp_spec_only(info, l2_conn, PRH_TCS_MSG_LISTEN_SUGGEST);
}
#endif

#if PRH_TCS_TERMINAL
int prh_tcs_gm_encode_access_rights_req(struct prh_st_tcs_gm_encode_info *info, struct prh_st_tcs_l2conn *l2_conn)
{
  return prh_tcs_gm_encode_comp_spec_only(info, l2_conn, PRH_TCS_MSG_ACCESS_RIGHTS_REQ);
}
#endif

#if PRH_TCS_GATEWAY
int prh_tcs_gm_encode_access_rights_accept(struct prh_st_tcs_gm_encode_info *info, struct prh_st_tcs_l2conn *l2_conn)
{
  return prh_tcs_gm_encode_comp_spec_only(info, l2_conn, PRH_TCS_MSG_ACCESS_RIGHTS_ACCEPT);
}
#endif

#if PRH_TCS_GATEWAY
int prh_tcs_gm_encode_access_rights_reject(struct prh_st_tcs_gm_encode_info *info, struct prh_st_tcs_l2conn *l2_conn)
{
  return prh_tcs_gm_encode_comp_spec_only(info, l2_conn, PRH_TCS_MSG_ACCESS_RIGHTS_REJECT);
}
#endif

#if PRH_TCS_TERMINAL
int prh_tcs_gm_encode_listen_request(struct prh_st_tcs_gm_encode_info *info, struct prh_st_tcs_l2conn *l2_conn)
{
  u_int16 buf_len=1, cpn_len;
  struct host_buf *p_buf;
  int ret;
  u_int8 *ch_ptr;

  if (!info->calledpn)
    return BT_INVALIDPARAM;

  cpn_len=info->calledpn->num_len;

  buf_len+=PRH_TCS_IE_CALLED_PN_BASE_LEN+cpn_len;

  
  if (info->compspec)
    {
      buf_len+=PRH_TCS_IE_COMP_SPEC_LEN;
    }

  p_buf=prh_tcs_gm_get_l2cap_buf(l2_conn->cid, buf_len);
  
  if (!p_buf)
    return BT_NORESOURCES;

  ch_ptr=p_buf->data;

  *ch_ptr=PRH_TCS_MSG_LISTEN_REQ | PRH_TCS_PROTO_DISC_GM;
  
  ch_ptr++;

  *ch_ptr++=PRH_TCS_IE_VL_CALLED_PARTY_NUM;
  *ch_ptr++=cpn_len+1;
  *ch_ptr++=((info->calledpn->type&0x7)<<4) + (info->calledpn->plan_id & 0xF);
  pMemcpy(ch_ptr, info->calledpn->num, info->calledpn->num_len);
  ch_ptr+=cpn_len;

  if (info->compspec)
    {
      *ch_ptr++=PRH_TCS_IE_VL_COMPANY_SPECIFIC;
      *ch_ptr++=PRH_TCS_IE_COMP_SPEC_LEN;
      BT_WRITE_LE_UINT16(ch_ptr, info->compspec->comp_id);
    }
  p_buf->len=buf_len;
  ret=L2_Write(l2_conn->cid, (t_DataBuf *)p_buf);
  return ret;
}
#endif

int prh_tcs_gm_encode_listen_accept(struct prh_st_tcs_gm_encode_info *info, struct prh_st_tcs_l2conn *l2_conn)
{
  u_int16 buf_len=1;
  struct host_buf *p_buf;
  int ret;
  u_int8 *ch_ptr;

  if (info->clock_offset)
    buf_len+=4;
  
  if (info->compspec)
    buf_len+=PRH_TCS_IE_COMP_SPEC_LEN;

  p_buf=prh_tcs_gm_get_l2cap_buf(l2_conn->cid, buf_len);
  
  if (!p_buf)
    return BT_NORESOURCES;
  
  ch_ptr=p_buf->data;

  *ch_ptr=PRH_TCS_MSG_LISTEN_ACCEPT | PRH_TCS_PROTO_DISC_GM;
  
  ch_ptr++;

  if (info->clock_offset)
    {
      *ch_ptr++=PRH_TCS_IE_VL_CLOCK_OFFSET;
      *ch_ptr++=2;
      BT_WRITE_LE_UINT16(ch_ptr, *(info->clock_offset));
      ch_ptr+=2;
    }

  if (info->compspec)
    {
      *ch_ptr++=PRH_TCS_IE_VL_COMPANY_SPECIFIC;
      *ch_ptr++=PRH_TCS_IE_COMP_SPEC_LEN;
      BT_WRITE_LE_UINT16(ch_ptr, info->compspec->comp_id);
    }
  p_buf->len=buf_len;
  ret=L2_Write(l2_conn->cid, (t_DataBuf *)p_buf);
  return ret;
}

int prh_tcs_gm_encode_listen_reject(struct prh_st_tcs_gm_encode_info *info, struct prh_st_tcs_l2conn *l2_conn)
{
  u_int16 buf_len=1;
  struct host_buf *p_buf;
  int ret;
  u_int8 *ch_ptr;

  if (info->cause)
    buf_len+=2;
  
  if (info->compspec)
    buf_len+=PRH_TCS_IE_COMP_SPEC_LEN;

  p_buf=prh_tcs_gm_get_l2cap_buf(l2_conn->cid, buf_len);
  
  if (!p_buf)
    return BT_NORESOURCES;
  
  ch_ptr=p_buf->data;

  *ch_ptr=PRH_TCS_MSG_LISTEN_REJECT | PRH_TCS_PROTO_DISC_GM;
  
  ch_ptr++;

  if (info->cause)
    {
      *ch_ptr++=PRH_TCS_IE_DO_CAUSE;
      *ch_ptr++=*(info->cause);
    }

  if (info->compspec)
    {
      *ch_ptr++=PRH_TCS_IE_VL_COMPANY_SPECIFIC;
      *ch_ptr++=PRH_TCS_IE_COMP_SPEC_LEN;
      BT_WRITE_LE_UINT16(ch_ptr, info->compspec->comp_id);
    }
  p_buf->len=buf_len;
  ret=L2_Write(l2_conn->cid, (t_DataBuf *)p_buf);
  return ret;
}

#if PRH_TCS_GATEWAY
int prh_tcs_gm_encode_info_suggest(struct prh_st_tcs_gm_encode_info *info, struct prh_st_tcs_l2conn *l2_conn)
{
 
  u_int16 buf_len=3;
  struct host_buf *p_buf;
  int i, ret;
  u_int8 *ch_ptr;

  if (!info->config || info->num_conf_ents==0)
    return BT_INVALIDPARAM;

  buf_len+=info->num_conf_ents*24;
  
  if (info->compspec)
    buf_len+=PRH_TCS_IE_COMP_SPEC_LEN;

  p_buf=prh_tcs_gm_get_l2cap_buf(l2_conn->cid, buf_len);
  
  if (!p_buf)
    return BT_NORESOURCES;

  ch_ptr=p_buf->data;

  *ch_ptr=PRH_TCS_MSG_INFO_SUGGEST | PRH_TCS_PROTO_DISC_GM;
  
  ch_ptr++;

  *ch_ptr++=PRH_TCS_IE_VL_CONFIG_DATA;
  *ch_ptr++=info->num_conf_ents*24;

  for (i=0; i<info->num_conf_ents; i++)
    {
      *ch_ptr++=info->config[i].internal_num[0];
      *ch_ptr++=info->config[i].internal_num[1];
      pMemcpy(ch_ptr, info->config[i].bd_addr.bytes, 6);
      ch_ptr+=6;
      pMemcpy(ch_ptr, info->config[i].link_key, 16);
      ch_ptr+=16;
    }

  if (info->compspec)
    {
      *ch_ptr++=PRH_TCS_IE_VL_COMPANY_SPECIFIC;
      *ch_ptr++=PRH_TCS_IE_COMP_SPEC_LEN;
      BT_WRITE_LE_UINT16(ch_ptr, info->compspec->comp_id);
    }
  p_buf->len=buf_len;
  ret=L2_Write(l2_conn->cid, (t_DataBuf *)p_buf);
  return ret;
}
#endif


#endif /* (PRH_TCS_TERMINAL || PRH_TCS_GATEWAY)*/

