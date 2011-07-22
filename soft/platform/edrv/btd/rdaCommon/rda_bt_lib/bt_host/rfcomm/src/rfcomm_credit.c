/***********************************************************************
 *
 * MODULE NAME:    rfcomm_credit.c
 * PROJECT CODE:   host side stack
 * DESCRIPTION:    RFCOMM Credit Flow Handler
 * MAINTAINER:     Dave Airlie
 * CREATION DATE:  02 February 2000
 *
 * SOURCE CONTROL: $Id: rfcomm_credit.c 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2002 Parthus Technologies Inc.
 *     All rights reserved.
 *
 ***********************************************************************/
#include "rfcomm_lower_layer.h"
#include "rfcomm_debug.h"
#include "rfcomm_ue_api.h"
#include "rfcomm_core.h"
#include "rfcomm_frame.h"

#include "rfcomm_proto.h"

/* 
 * prh_rfcomm_credit_peer_nocredit
 * -- Remote Peer has used up all his credits
 */
t_api prh_rfcomm_credit_peer_nocredit(struct prh_rfcomm_dlc_inst *self)
{
    if (self->auto_credit_issue==0) 
    { /* 0 is the default value for backward compatability */
          /* send PRH_RFCOMM_CREDIT_ISSUE credits */
          struct host_buf *frame;
          int ret;

          self->cur_local_credits+=PRH_RFCOMM_CREDIT_ISSUE;

          ret=PRH_RFCOMM_GET_L2_FRAME(self->rfcomm_l2cap_ptr->cid, PRH_RFCOMM_BUF_UIH_LONG, &frame);
          if (ret!=BT_NOERROR)
            return ret;
          host_buf_reserve_header(frame, PRH_RFCOMM_BUF_UIH_LONG);

          frame->len=0;
          prh_rfcomm_create_uih_credit(frame, self->dlci, self->rfcomm_l2cap_ptr->initiating, PRH_RFCOMM_CREDIT_ISSUE);
          pMutexLock(self->rfcomm_l2cap_ptr->lock);
          ret=L2_Write(self->rfcomm_l2cap_ptr->cid, (t_DataBuf *)frame);
          pMutexUnlock(self->rfcomm_l2cap_ptr->lock);
          return BT_NOERROR;
    } else {
      /* 
        Do not send the peer more credits. When space becomes available, the PE
        will call prh_rfcomm_credit_peer_issue to send more credits to the peer.
      */
    }
    return BT_NOERROR;
}

/* 
 * PRH_RFCOMM_UE_Set_Credit_Mode
 * -- Set credit mode for issuing credits
 */
t_api PRH_RFCOMM_UE_Set_Credit_Mode(prh_t_rfcomm_dlci dlci, u_int8 auto_credit_issue, void *multi_inst)
{
    struct prh_rfcomm_dlc_inst *dlc_inst;
    struct prh_rfcomm_l2cap_inst *l2cap_inst=(struct prh_rfcomm_l2cap_inst *)multi_inst;
    t_api ret;

    ret=prh_rfcomm_validate_and_lock_l2cap_inst_by_ptr(l2cap_inst);
    if (ret==BT_NOERROR)
    {
        ret=prh_rfcomm_lookup_dlci_instance_and_lock(multi_inst, dlci, &dlc_inst);
        pMutexUnlock(l2cap_inst->lock);   
        
        if (ret==BT_NOERROR) 
        {
            /* 
                auto_credit_issue:
                  0 == automatically issue credits to peer
                  1 == PE will call PRH_RFCOMM_UE_Issue_Credits to issue credits
                  >1   reserved
            */
            if (auto_credit_issue>1)
                return BT_INVALIDPARAM;
        
            dlc_inst->auto_credit_issue=auto_credit_issue;
            ret=BT_NOERROR;
            pMutexUnlock(dlc_inst->lock);
        }
    } 
    return ret;
}

/* 
 * PRH_RFCOMM_UE_Issue_Credits
 * -- Issue credits to remote peer
 */
t_api PRH_RFCOMM_UE_Issue_Credits(prh_t_rfcomm_dlci dlci, u_int8 num_credits, void *multi_inst)
{
    struct prh_rfcomm_dlc_inst *dlc_inst;
    struct prh_rfcomm_l2cap_inst *l2cap_inst=(struct prh_rfcomm_l2cap_inst *)multi_inst;
    t_api ret;

    ret=prh_rfcomm_validate_and_lock_l2cap_inst_by_ptr(l2cap_inst);
    if (ret==BT_NOERROR)
    {
        ret=prh_rfcomm_lookup_dlci_instance_and_lock(multi_inst, dlci, &dlc_inst);
        pMutexUnlock(l2cap_inst->lock);   
        
        if (ret==BT_NOERROR) 
        {
              /* send credits */
              struct host_buf *frame;
              int ret_b;

              dlc_inst->cur_local_credits+=num_credits;

              ret_b=PRH_RFCOMM_GET_L2_FRAME(dlc_inst->rfcomm_l2cap_ptr->cid, PRH_RFCOMM_BUF_UIH_LONG, &frame);
              if (ret_b!=BT_NOERROR)
                return ret_b;
              host_buf_reserve_header(frame, PRH_RFCOMM_BUF_UIH_LONG);

              frame->len=0;
              prh_rfcomm_create_uih_credit(frame, dlc_inst->dlci, dlc_inst->rfcomm_l2cap_ptr->initiating, num_credits);
              pMutexLock(dlc_inst->rfcomm_l2cap_ptr->lock);
              ret=L2_Write(dlc_inst->rfcomm_l2cap_ptr->cid, (t_DataBuf *)frame);
              pMutexUnlock(dlc_inst->rfcomm_l2cap_ptr->lock);
           
            
            //ret=BT_NOERROR;
            pMutexUnlock(dlc_inst->lock);
        }
    } 
    return ret;
}

