/******************************************************************************/
/*                                                                            */
/*              Copyright (C) 2005, Coolsand Technologies, Inc.               */
/*                            All Rights Reserved                             */
/*                                                                            */
/******************************************************************************/
/*                                                                            */
/* This source code is property of Coolsand. The information contained in     */
/* this file is confidential. Distribution, reproduction, as well as          */
/* exploitation,or transmisison of any content of this file is not            */
/* allowed except if expressly permitted.                                     */
/* Infringements result in damage claims!                                     */
/*                                                                            */
/* FILE NAME                                                                  */
/*      btd_win32.c                                                           */
/*                                                                            */
/* DESCRIPTION                                                                */
/*    BT service declarations, constant definitions and macros                */
/*    See CSW developement Manual for more.                                   */
/*                                                                            */
/******************************************************************************/

#ifdef MMI_ON_WIN32

#include "cswtype.h"
#include "mmi_data_types.h"
#include "bt_drv.h"
#include "med_context.h"
#include "bt_avrcp_struct.h"
#include "bluetooth_bm_struct.h"
#include "fs_type.h"
#include "xml_def.h"
#include "bt_a2dp_struct.h"
#include "app_mine.h"
A2DP_codec_struct *codec;
med_context_struct med_context;
bth_dev_info_array g_paired_devInfo;
const bt_a2dp_sbc_codec_cap_struct sbc_cap[1];
const bt_a2dp_mp3_codec_cap_struct mp3_cap[1];
kal_uint8 SBC_buf[16*1024];

VOID bt_SendMessage(UINT8 msgid,BT_CMDSTRUCT_T* cmd)
{
	
}

VOID btd_free(VOID* msg)
{

}

void rda_bt_hfg_send_BldnFail_ind(void)
{

}

void rda_bt_hfg_send_reject_cfm(void)
{

}
void rda_bt_hfg_send_CallSetup_1_ind(void)
{
}

void aud_send_bt_audio_open_req(module_type src_mod_id, kal_uint8 profile, kal_uint8 mode, kal_uint16 connect_id)
{

}

void media_aud_bt_audio_close(kal_uint8 profile)
{

}

void aud_send_bt_audio_turn_on_req(module_type src_mod_id, kal_uint8 profile)
{

}

void media_aud_bt_turn_off_audio(kal_uint8 profile)
{

}

void rda_bt_poweroff_for_test(void)
{

}

void RDA5868_send_vco_test(void)
{

}

kal_uint8 rda_bt_poweron_for_test(void)
{
  return 0;
}

void RDA5868_send_bbtest_singlehop_inquiry(void)
{

}

void RDA_bt_send_cmu200_enable_dut(void)
{
  
}

void RDA_bt_send_tc3000_enable_dut(void)
{

}

void RDA_bt_send_n4010_enable_dut(void)
{

}

kal_uint8 rdabt_get_chipids(void)
{
  return 0;
}

kal_uint8 rdabt_get_lmpversion(void)
{
  return 0;
}
kal_uint8 rdabt_get_chipid(void)
{
return 0;
}
void bt_a2dp_send_activate_req( module_type src_mod_id, kal_uint8 local_role )
{

}

void bt_avrcp_send_activate_req( module_type src_mod_id, kal_uint8 chnl_num, kal_uint8 local_role )
{

}

void bt_avrcp_send_disconnect_req( module_type src_mod_id, kal_uint8 chnl_num)
{

}

void bt_avrcp_send_deactivate_req( module_type src_mod_id, kal_uint8 chnl_num )
{

}

void bt_avrcp_send_connect_req( module_type src_mod_id, kal_uint8 chnl_num, bt_avrcp_dev_addr_struct* device_addr, kal_uint8 local_role )
{

}

void bt_avrcp_send_cmd_frame_req( module_type src_mod_id, kal_uint8 chnl_num, kal_uint8 seq_id, kal_uint8 c_type, kal_uint8 subunit_type, kal_uint8 subunit_id, kal_uint16 data_len, kal_uint8* frame_data)
{

}

void bt_avrcp_send_cmd_frame_ind_res( module_type src_mod_id, kal_uint8 chnl_num, kal_uint8 seq_id, kal_uint16 profile_id,kal_uint8 c_type, kal_uint8 subunit_type, kal_uint8 subunit_id, kal_uint16 data_len, kal_uint8* frame_data)
{

}

void xml_close_parser(XML_PARSER_STRUCT *parser)
{
  
}

kal_int32 xml_resume_parse(XML_PARSER_STRUCT *parser)
{
  return 0;
}

kal_int32 xml_parse(XML_PARSER_STRUCT *parser, kal_wchar *file_name)
{
  return 0;
}

void xml_register_element_handler(
        XML_PARSER_STRUCT *parser,
        XML_start_elem_hdlr start_hdlr,
        XML_end_elem_hdlr end_hdlr)
{
}

kal_int32 xml_new_parser(XML_PARSER_STRUCT *parser)
{
  return 0;
}

void xml_pause_parse(XML_PARSER_STRUCT *parser)
{
}

void xml_stop_parse(XML_PARSER_STRUCT *parser)
{

}


void rda_SetRoamStatusCB(void * func)
{

}

void rda_SetSignalStrengthCB(void * func)
{

}

void rda_setCallHoldCountCB(void * func)
{

}

void rda_setCurrentCallStatusCB(void * func)
{

}

kal_uint8 rda_bt_a2dp_match_audio_capabilities( bt_a2dp_audio_cap_struct *audio_cap, bt_a2dp_audio_cap_struct *audio_cfg, kal_uint16 *result )
{
  return 0;
}

void aud_send_bt_open_codec_req(module_type src_mod_id, bt_a2dp_audio_cap_struct *cfg, 
    kal_uint8 stream_handle, kal_bool immediate, kal_uint32 seq_no)
{

}

void aud_send_bt_close_codec_req(module_type src_mod_id)
{

}

void bt_a2dp_send_sep_discover_req( module_type src_mod_id, kal_uint16 connect_id )
{

}

void bt_a2dp_send_capabilities_get_req( module_type src_mod_id, kal_uint16 connect_id, kal_uint8 acp_seid )
{
  
}

void bt_a2dp_send_stream_config_req( module_type src_mod_id, kal_uint16 connect_id, kal_uint8 acp_seid, kal_uint8 int_seid, bt_a2dp_audio_cap_struct* audio_cap )
{

}

void bt_a2dp_send_stream_open_req( module_type src_mod_id, kal_uint8 stream_handle )
{

}

void bt_a2dp_send_stream_start_req( module_type src_mod_id, kal_uint8 stream_handle )
{
  
}

void bt_a2dp_send_stream_pause_req( module_type src_mod_id, kal_uint8 stream_handle,A2DP_codec_struct* codec )
{

}

void bt_a2dp_send_stream_close_req( module_type src_mod_id, kal_uint8 stream_handle )
{

}

void bt_a2dp_send_stream_abort_req( module_type src_mod_id, kal_uint8 stream_handle )
{

}

void bt_a2dp_send_sep_discover_res( module_type src_mod_id, kal_uint16 connect_id, kal_uint16 result, kal_uint8 sep_num, bt_sep_info_struct* sep_list )
{

}

void bt_a2dp_send_capabilities_get_res( module_type src_mod_id, kal_uint16 connect_id, kal_uint16 result, kal_uint8 audio_cap_num, bt_a2dp_audio_cap_struct* audio_cap_list )
{

}

void bt_a2dp_send_stream_config_res( module_type src_mod_id, kal_uint16 result, kal_uint8 stream_handle )
{

}

void bt_a2dp_send_stream_open_res( module_type src_mod_id, kal_uint16 result, kal_uint8 stream_handle )
{

}

void bt_a2dp_send_stream_start_res( module_type src_mod_id, kal_uint16 result, kal_uint8 stream_handle )
{

}

void bt_a2dp_send_stream_pause_res( module_type src_mod_id, kal_uint16 result, kal_uint8 stream_handle )
{

}

void bt_a2dp_send_stream_close_res( module_type src_mod_id, kal_uint16 result, kal_uint8 stream_handle )
{

}

void bt_a2dp_send_stream_abort_res( module_type src_mod_id, kal_uint8 stream_handle )
{

}

void rda_bt_hfg_send_ring(void)
{

}

void bt_a2dp_send_signal_disconnect_req( module_type src_mod_id, kal_uint16 connect_id )
{

}

void bt_a2dp_send_deactivate_req( module_type src_mod_id )
{

}

void bt_a2dp_send_signal_connect_req( module_type src_mod_id, bt_device_addr_struct* device_addr, kal_uint8 local_role )
{

}

kal_int16 med_get_media_type(kal_wchar *file_name)
{
  return 0;
}

kal_int32 med_util_utf8_to_ucs2( kal_uint8 *dest, kal_uint32 dest_size, kal_uint8* src, kal_uint32 src_size )
{
  return 0;
}

#if 0
applib_mime_type_struct *mime_get_file_type(kal_wchar *file_path)
{
  return NULL;
}
#endif
void rda_bt_a2dp_send_stream_start_res( kal_uint8 src_mod_id, kal_uint16 result, kal_uint8 stream_handle )
{

}

void rda_bt_a2dp_send_stream_open_res( kal_uint8 src_mod_id, kal_uint16 result, kal_uint8 stream_handle )
{

}

void rda_bt_a2dp_send_stream_config_res( kal_uint8 src_mod_id, kal_uint16 result, kal_uint8 stream_handle)
{
  
}

void rda_bt_a2dp_send_capabilities_get_res( kal_uint8 src_mod_id, kal_uint16 connect_id, kal_uint16 result, kal_uint8 codec_type, bt_a2dp_audio_codec_cap_struct* audio_cap )
{

}

void rda_bt_a2dp_send_sep_discover_res( kal_uint8 src_mod_id, kal_uint16 connect_id, kal_uint16 result, kal_uint8 sep_num, bt_sep_info_struct* sep_list)
{

}

void rda_bt_avrcp_send_cmd_frame_ind_res(kal_uint8 chnl_num, kal_uint8 seq_id, kal_uint16 profile_id,kal_uint8 result, kal_uint8 subunit_type, kal_uint8 subunit_id, kal_uint16 data_len, kal_uint8* frame_data )
{

}

void bt_a2dp_send_stream_reconfig_req( module_type src_mod_id,kal_uint16 connect_id, kal_uint8 acp_seid, bt_a2dp_audio_cap_struct* audio_cap )
{

}

#endif /* MMI_ON_WIN32 */