/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of CoolSand Inc. (C) 2005
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("COOLSAND SOFTWARE")
*  RECEIVED FROM COOLSAND AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. COOLSAND EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES COOLSAND PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE COOLSAND SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. COOLSAND SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY COOLSAND SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND COOLSAND'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE COOLSAND SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT COOLSAND'S OPTION, TO REVISE OR REPLACE THE COOLSAND SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  COOLSAND FOR SUCH COOLSAND SOFTWARE AT ISSUE. 
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
*****************************************************************************/

/*****************************************************************************
 *
 * Filename:
 * ---------
 * JProvCCAHandler.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   header file for JProvCCAHandler.c file
 *
 * Author:
 * -------
 * -------
 *
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------

 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/
#ifndef _JPROVCCAHANDLER_H
#define _JPROVCCAHANDLER_H

#include "mmi_features.h"
#include "ddl.h"
#ifdef __MMI_JATAAYU_CCA_SUPPORT__

#if defined (JATAAYU_SUPPORT) && (defined (WAP_SUPPORT) || (defined (MMS_SUPPORT)))

#include "mmi_data_types.h"
#include "jbrowsermmigprots.h"
#include "jprovwap.h"
#include "jprovoma.h"

#define MAX_TONAPID_SUPPORT 10
#define MAX_NODE_LEVEL  5
//#define MAX_PHYSICAL_PROXY_NODE  10
//#define MAX_EMPTY_PROXY_NODE  10

#define PROV_ROOT_NODE_VAL          0x01
#define PROV_PXPHYSICAL_NODE_VAL    0x02
#define PROV_PXLOGICAL_NODE_VAL        0x04
#define PROV_PORT_NODE_VAL          0x08
#define PROV_PXAUTHINFO_NODE_VAL    0x10
#define PROV_APPLICATION_NODE_VAL      0x20
#define PROV_RESOURCE_NODE_VAL         0x40
#define PROV_APPADDR_NODE_VAL       0x80
#define PROV_APPAUTH_NODE_VAL       0x100
#define PROV_BOOTSTRAP_NODE_VAL        0x200
#define PROV_NAPDEF_NODE_VAL        0x400
#define PROV_NAPAUTHINFO_NODE_VAL      0x800
#define PROV_VALIDITY_NODE_VAL         0x1000
#define PROV_ACCESS_NODE_VAL        0x2000

/* TK start on 08/02/07 */
#define PROV_AACCEPT             "AACCEPT"
#define PROV_AAUTHDATA              "AAUTHDATA"
#define PROV_AAUTHLEVEL          "AAUTHLEVEL"
#define PROV_AAUTHNAME              "AAUTHNAME"
#define PROV_AAUTHSECRET            "AAUTHSECRET"
#define PROV_AAUTHTYPE              "AAUTHTYPE"
#define PROV_ADDR                "ADDR"
#define PROV_ADDRTYPE               "ADDRTYPE"
#define PROV_APPID                  "APPID"
#define PROV_APROTOCOL              "APROTOCOL"
#define PROV_CIDPREFIX              "CIDPREFIX"
#define PROV_FROM                "FROM"
#define PROV_NAME                "NAME"
#define PROV_PROVIDER_ID            "PROVIDER-ID"
#define PROV_RT_ADDR             "RT-ADDR"
#define PROV_STARTPAGE              "STARTPAGE"
#define PROV_TO_PROXY               "TO-PROXY"
#define PROV_URI                 "URI"
#define PROV_AAUTHDATA              "AAUTHDATA"
#define PROV_AAUTHNAME              "AAUTHNAME"
#define PROV_AAUTHSECRET            "AAUTHSECRET"
#define PROV_AUTH_ENTITY            "AUTH-ENTITY"
#define PROV_AUTHNAME               "AUTHNAME"
#define PROV_AUTHSECRET             "AUTHSECRET"
#define PROV_AUTHTYPE               "AUTHTYPE"
#define PROV_BASAUTH_ID             "BASAUTH-ID"
#define PROV_BASAUTH_PW             "BASAUTH-PW"
#define PROV_BEARER                 "BEARER"
#define PROV_CALLTYPE               "CALLTYPE"
#define PROV_COUNTRY             "COUNTRY"
#define PROV_DELIVERY_ERR_SDU       "DELIVERY-ERR-SDU"
#define PROV_DELIVERY_ORDER            "DELIVERY-ORDER"
#define PROV_DNLINKSPEED            "DNLINKSPEED"
#define PROV_DNS_ADDR               "DNS-ADDR"
#define PROV_FIRST_RETRY_TIMEOUT    "FIRST-RETRY-TIMEOUT"
#define PROV_GUARANTEED_BITRATE_DNLINK "GUARANTEED-BITRATE-DNLINK"
#define PROV_GUARANTEED_BITRATE_UPLINK "GUARANTEED-BITRATE-UPLINK"
#define PROV_INTERNET               "INTERNET"
#define PROV_LINGER                 "LINGER"
#define PROV_LINKSPEED              "LINKSPEED"
#define PROV_LOCAL_ADDR             "LOCAL-ADDR"
#define PROV_LOCAL_ADDRTYPE            "LOCAL-ADDRTYPE"
#define PROV_MASTER              "MASTER"
#define PROV_MAX_BITRATE_DNLINK        "MAX-BITRATE-DNLINK"
#define PROV_MAX_BITRATE_UPLINK        "MAX-BITRATE-UPLINK"
#define PROV_MAX_NUM_RETRY          "MAX-NUM-RETRY"
#define PROV_MAX_SDU_SIZE           "MAX-SDU-SIZE"
#define PROV_NAP_ADDRESS            "NAP-ADDRESS"
#define PROV_NAP_ADDRTYPE           "NAP-ADDRTYPE"
#define PROV_NAPID                  "NAPID"
#define PROV_NETWORK             "NETWORK"
#define PROV_PHYSICAL_PROXY_ID         "PHYSICAL-PROXY-ID"
#define PROV_PORTNBR             "PORTNBR"
#define PROV_PPGAUTH_TYPE           "PPGAUTH-TYPE"
#define PROV_PROXY_ID               "PROXY-ID"
#define PROV_PROXY_PROVIDER_ID         "PROXY-PROVIDER-ID"
#define PROV_PROXY_PW               "PROXY-PW"
#define PROV_PULLENABLED            "PULLENABLED"
#define PROV_PUSHENABLED            "PUSHENABLED"
#define PROV_PXADDR                 "PXADDR"
#define PROV_PXADDR_FQDN            "PXADDR-FQDN"
#define PROV_PXADDRTYPE             "PXADDRTYPE"
#define PROV_PXAUTH_ID              "PXAUTH-ID"
#define PROV_PXAUTH_PW              "PXAUTH-PW"
#define PROV_PXAUTH_TYPE            "PXAUTH-TYPE"
#define PROV_REREG_THRESHOLD        "REREG-THRESHOLD"
#define PROV_RESIDUAL_BER           "RESIDUAL-BER"
#define PROV_SDU_ERROR_RATIO        "SDU-ERROR-RATIO"
#define PROV_SERVICE             "SERVICE"
#define PROV_SID                 "SID"
#define PROV_SOC                 "SOC"
#define PROV_SPI                 "SPI"
#define PROV_T_BIT                  "T-BIT"
#define PROV_TO_NAPID               "TO-NAPID"
#define PROV_TRAFFIC_CLASS          "TRAFFIC-CLASS"
#define PROV_TRAFFIC_HANDL_PRIO        "TRAFFIC-HANDL-PRIO"
#define PROV_TRANSFER_DELAY            "TRANSFER-DELAY"
#define PROV_TRUST                  "TRUST"
#define PROV_VALIDUNTIL             "VALIDUNTIL"
#define PROV_WSP_VERSION            "WSP-VERSION"
/* ENUM VALUE START */
#define  PROV_IPV4                  "IPV4"
#define  PROV_IPV6                  "IPV6"
#define  PROV_E164                  "E164"
#define  PROV_ALPHA                 "ALPHA"
#define  PROV_APN                "APN"
#define  PROV_SCODE                 "SCODE"
#define  PROV_TETRA_ITSI               "TETRA-ITSI"
#define  PROV_MAN                "MAN"

#define  PROV_CL_WSP                "CL-WSP"
#define  PROV_CO_WSP                "CO-WSP"
#define  PROV_CL_SEC_WSP               "CL-SEC-WSP"
#define  PROV_CO_SEC_WSP               "CO-SEC-WSP"
#define  PROV_CO_SEC_WTA               "CO-SEC-WTA"
#define  PROV_CL_SEC_WTA               "CL-SEC-WTA"
#define  PROV_OTA_HTTP_TO           "OTA-HTTP-TO"
#define  PROV_OTA_HTTP_TLS_TO       "OTA-HTTP-TLS-TO"
#define  PROV_OTA_HTTP_PO           "OTA-HTTP-PO"
#define  PROV_OTA_HTTP_TLS_PO       "OTA-HTTP-TLS-PO"
#define  PROV_STARTTLS              "STARTTLS"

#define  PROV_GSM_CSD               "GSM-CSD"
#define  PROV_GSM_GPRS              "GSM-GPRS"
#define  PROV_GSM_USSD              "GSM-USSD"
#define  PROV_GSM_SMS               "GSM-SMS"
#define  PROV_CDMA_SMS              "IS-95-CDMA-SMS"
#define  PROV_CDMA_CSD              "IS-95-CDMA-CSD"
#define  PROV_CDMA_PACKET           "IS-95-CDMA-PACKET"
#define  PROV_ANSI_136_GUTS            "ANSI-136-GUTS"
#define  PROV_ANSI_136_CSD          "ANSI-136-CSD"
#define  PROV_ANSI_136_GPRS            "ANSI-136_GPRS"
#define  PROV_ANSI_136_GHOST           "ANSI-136_GHOST"
#define  PROV_AMPS_CDPD             "AMPS-CDPD"
#define  PROV_PDC_CSD               "PDC-CSD"
#define  PROV_PDC_PACKET               "PDC-PACKET"
#define  PROV_IDEN_SMS              "IDEN-SMS"
#define  PROV_IDEN_CSD              "IDEN-CSD"
#define  PROV_IDEN_PACKET           "IDEN-PACKET"
#define  PROV_FLEX_REFLEX           "FLEX/REFLEX"
#define  PROV_PHS_SMS               "PHS-SMS"
#define  PROV_PHS_CSD               "PHS-CSD"
#define  PROV_TETRA_SDS             "TETRA-SDS"
#define  PROV_TETRA_PACKET          "TETRA-PACKET"
#define  PROV_MOBITEX_MPAK          "MOBITEX MPAK"
#define  PROV_CDMA2000_1X_SIMPLE_IP    "CDMA2000-1X-SIMPLE-IP"
#define  PROV_CDMA2000_1X_MOBILE_IP    "CDMA2000-1X_MOBILE_IP"

#define  PROV_ANALOG_MODEM          "ANALOG-MODEM"
#define  PROV_V120                  "V.120"
#define  PROV_V110                  "V.110"
#define  PROV_X31                "X.31"
#define  PROV_BIT_TRANSPARENT       "BIT-TRANSPARENT"
#define  PROV_DIRECT_ASYNC_DATA_SERVICE   "DIRECT-ASYNC-DATA-SERVICE"

#if defined(__MMI_VOIP_OTAP_DMP__) || defined(__MMI_WLAN_OTAP_DMP__) || defined(__MMI_FOTA_OTAP_DMP__)
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif /*#if defined(__MMI_VOIP_OTAP_DMP__) || defined(__MMI_WLAN_OTAP_DMP__) || defined(__MMI_FOTA_OTAP_DMP__)*/


typedef enum
{
    PROV_CHARACTERISTIC_START,
    PROV_CHARACTERISTIC_END
} prov_characteristic_status_enum;

typedef enum
{
    PROV_ERROR_DUP_NODE_CREATED = 10,
    PROV_ERR_UNKNOWN_TAG
} prov_error_enum;

typedef enum
{
    PROV_CCA_CONV_STR = 1,
    PROV_CCA_CONV_ENUM_FROM_STR,
    PROV_CCA_CONV_INT,
    PROV_CCA_CONV_TOTAL /* Add element before it */
} prov_cca_conv_type_enum;

typedef struct
{
    U8 *param;
    U16 cca_symbol;
    prov_cca_conv_type_enum convType;
      PROV_RETCODE(*param_tag_hdlr) (U16 *attr_value, S32 *root, U16 *ret_value);
} prov_characteristic_param_tag_hdlr_struct;

typedef struct
{
    U8 *src_str;
    U16 cca_enum_value;
} prov_convert_str_to_enum_struct;

typedef struct
{
    U8 *characterestic;
    U16 cca_node_id;
    S32 src_parent_node_id;
    S32 src_node_id;
      PROV_RETCODE(*characteristic_tag_hdlr) (S32 *root, prov_characteristic_status_enum charac_status);
    const prov_characteristic_param_tag_hdlr_struct *char_param_tag_map_hdlr;
} prov_characteristic_tag_hdlr_struct;

typedef struct
{
    S32 xmlNodeId;  /* to be removed later */
    S32 par_cca_node_h;
    S32 crt_cca_node_h;
      PROV_RETCODE(*characteristic_tag_hdlr) (S32 *root, prov_characteristic_status_enum charac_status);
    const prov_characteristic_param_tag_hdlr_struct *char_param_tag_map_hdlr;
} prov_curr_ccanode_characteristic_tag_hdlr_st;

typedef struct
{
    S32 sibHCCANode;
    S32 curHCCANode;
    struct sibling_node_struct *sib_node_next;
} sibling_node_struct;

typedef struct
{
    S32 node_parent_id;
    S32 parent_parent_id;
    U8 cur_node_level;
    MMI_BOOL valid_prov_tag;            /* Handled if all the characteristic are unknown */
    U8 *tonapid[MAX_TONAPID_SUPPORT];   /* Contains to napid param of an application */
    sibling_node_struct *sibling_node_list_p;

} prov_cca_handler_context_st;

extern prov_cca_handler_context_st g_prov_cca_handler_ctx;

extern PROV_RETCODE mmi_prov_appauth_param_authlevel_tag_hdlr(U16 *attr_value, S32 *root, U16 *ret_value);
extern PROV_RETCODE mmi_prov_application_param_appid_tag_hdlr(U16 *attr_value, S32 *root, U16 *ret_value);
extern PROV_RETCODE mmi_prov_napauthinfo_param_authentity_tag_hdlr(U16 *attr_value, S32 *root, U16 *ret_value);

/* extern PROV_RETCODE mmi_prov_napdef_param_addrtype_tag_hdlr(U16 *attr_value, S32 *root, U16 *ret_value); */
extern PROV_RETCODE mmi_prov_napdef_param_bearer_tag_hdlr(U16 *attr_value, S32 *root, U16 *ret_value);
extern PROV_RETCODE mmi_prov_napdef_param_calltype_tag_hdlr(U16 *attr_value, S32 *root, U16 *ret_value);

extern PROV_RETCODE mmi_prov_port_param_service_tag_hdlr(U16 *attr_value, S32 *root, U16 *ret_value);
extern PROV_RETCODE mmi_prov_proxy_param_ppgauth_tag_hdlr(U16 *attr_value, S32 *root, U16 *ret_value);
extern PROV_RETCODE mmi_prov_proxy_param_pushpullenabled_tag_hdlr(U16 *attr_value, S32 *root, U16 *ret_value);

extern PROV_RETCODE mmi_prov_proxy_param_pxaddrtype_tag_hdlr(U16 *attr_value, S32 *root, U16 *ret_value);

/* extern PROV_RETCODE mmi_prov_pxauthinfo_param_pxauthtype_tag_hdlr(U16 *attr_value, S32 *root, U16 *ret_value); */
extern PROV_RETCODE mmi_prov_param_addrtype_tag_hdlr(U16 *attr_value, S32 *root, U16 *ret_value);
extern PROV_RETCODE mmi_prov_param_authtype_tag_hdlr(U16 *attr_value, S32 *root, U16 *ret_value);

extern PROV_RETCODE mmi_prov_proxy_param_proxyid_tag_hdlr(U16 *attr_value, S32 *root, U16 *ret_value);

extern PROV_RETCODE mmi_prov_proxy_param_name_tag_hdlr(U16 *attr_value, S32 *root, U16 *ret_value);
extern PROV_RETCODE mmi_prov_proxy_param_phyproxyid_tag_hdlr(U16 *attr_value, S32 *root, U16 *ret_value);
extern PROV_RETCODE mmi_prov_proxy_param_pxaddr_tag_hdlr(U16 *attr_value, S32 *root, U16 *ret_value);

extern PROV_RETCODE mmi_prov_proxy_param_tonapid_tag_hdlr(U16 *attr_value, S32 *root, U16 *ret_value);
extern PROV_RETCODE mmi_prov_port_param_port_tag_hdlr(U16 *attr_value, S32 *root, U16 *ret_value);
extern PROV_RETCODE mmi_prov_pxauthinfo_param_uri_tag_hdlr(U16 *attr_value, S32 *root, U16 *ret_value);
extern PROV_RETCODE mmi_prov_appaddr_param_addr_tag_hdlr(U16 *attr_value, S32 *root, U16 *ret_value);

extern PROV_RETCODE mmi_prov_oma_handle_characteristic_tag(TAG **nextroot_p);
extern PROV_RETCODE mmi_prov_oma_handle_param_tag(TAG **nextroot_p);
extern PROV_RETCODE mmi_prov_oma_search_and_fill(ATTRIBUTE *attr_p);
extern PROV_RETCODE mmi_prov_characteristic_root_tag_hdlr(S32 *root, prov_characteristic_status_enum charac_status);

extern PROV_RETCODE mmi_prov_characteristic_pxphysical_tag_hdlr(
                        S32 *root,
                        prov_characteristic_status_enum charac_status);
extern PROV_RETCODE mmi_prov_characteristic_pxlogical_tag_hdlr(
                        S32 *root,
                        prov_characteristic_status_enum charac_status);
extern PROV_RETCODE mmi_prov_characteristic_port_tag_hdlr(S32 *root, prov_characteristic_status_enum charac_status);
extern PROV_RETCODE mmi_prov_characteristic_pxauthinfo_tag_hdlr(
                        S32 *root,
                        prov_characteristic_status_enum charac_status);
extern PROV_RETCODE mmi_prov_characteristic_application_tag_hdlr(
                        S32 *root,
                        prov_characteristic_status_enum charac_status);
extern PROV_RETCODE mmi_prov_characteristic_resource_tag_hdlr(
                        S32 *root,
                        prov_characteristic_status_enum charac_status);
extern PROV_RETCODE mmi_prov_characteristic_appaddr_tag_hdlr(S32 *root, prov_characteristic_status_enum charac_status);
extern PROV_RETCODE mmi_prov_characteristic_appauth_tag_hdlr(S32 *root, prov_characteristic_status_enum charac_status);
extern PROV_RETCODE mmi_prov_characteristic_bootstrap_tag_hdlr(
                        S32 *root,
                        prov_characteristic_status_enum charac_status);
extern PROV_RETCODE mmi_prov_characteristic_napdef_tag_hdlr(S32 *root, prov_characteristic_status_enum charac_status);
extern PROV_RETCODE mmi_prov_characteristic_napauthinfo_tag_hdlr(
                        S32 *root,
                        prov_characteristic_status_enum charac_status);
extern PROV_RETCODE mmi_prov_characteristic_validity_tag_hdlr(
                        S32 *root,
                        prov_characteristic_status_enum charac_status);
extern PROV_RETCODE mmi_prov_characteristic_access_tag_hdlr(S32 *root, prov_characteristic_status_enum charac_status);
extern PROV_RETCODE mmi_prov_characteristic_end_tag_hdlr(U32 parentNodeId);
extern PROV_RETCODE mmi_prov_attach_node_to_sibling_node_list(S32 curHCCANode, S32 par_cca_node_h);
extern PROV_RETCODE mmi_prov_add_nodedata_str_to_sibling_node_list(S32 CurHCCANode, U16 cca_symbol, S8 *valuPtr);
extern PROV_RETCODE mmi_prov_add_nodedata_int_to_sibling_node_list(S32 CurHCCANode, U16 cca_symbol, S32 value);
extern PROV_RETCODE mmi_prov_release_sibling_node_from_node_list(S32 *CurHCCANode, PROV_RETCODE retCode);
extern PROV_RETCODE mmi_prov_add_sibling_node_to_node_list(S32 CurHCCANode, S32 SibHCCANode);
extern PROV_RETCODE mmi_prov_application_param_toproxy_tag_hdlr(U16 *attr_value, S32 *root, U16 *ret_value);
extern PROV_RETCODE mmi_prov_application_param_tonapid_tag_hdlr(U16 *attr_value, S32 *root, U16 *ret_value);
extern PROV_RETCODE mmi_prov_napdef_param_napid_tag_hdlr(U16 *attr_value, S32 *root, U16 *ret_value);
extern const prov_characteristic_param_tag_hdlr_struct *mmi_prov_oma_search_characteristic_tag_hdlr(U16 *attrValue);
extern PROV_RETCODE mmi_prov_create_dependency_relationship(S32 hroot, PROV_RETCODE retcode);
extern U16 mmi_prov_get_enum_from_str_enum_table(U16 *attrValue, prov_convert_str_to_enum_struct strEnumTable[]);
extern S32 mmi_prov_compare_ansi_unicode_str(S8 *firstStr, U16 *secondStr);
extern PROV_RETCODE mmi_prov_root_end_tag_hdlr(PROV_RETCODE retcode);
extern PROV_RETCODE mmi_prov_oma_construct_cca_doc(TAG *root_p);
extern PROV_RETCODE mmi_prov_provisioning_data_handler(S8 *prov_buffer_p, U32 prov_length, S8 *content_type_p);
extern PROV_RETCODE mmi_prov_add_cca_nodeid(S32 crtHNode, U16 *charTagName);
extern PROV_RETCODE mmi_prov_add_nodedata_enum_to_cca_node(S32 curccanode_h, U16 cca_symbol, S32 value);
extern PROV_RETCODE mmi_prov_add_nodedata_int_to_cca_node(S32 curccanode_h, U16 cca_symbol, U16 *valuPtr);
extern PROV_RETCODE mmi_prov_add_nodedata_str_to_cca_node(S32 curccanode_h, U16 cca_symbol, U16 *valuPtr);
extern PROV_RETCODE mmi_prov_handle_unknown_characteristic_tag(TAG **nextroot_p, PROV_RETCODE retcode);
extern void mmi_prov_cca_new_doc_res_hdlr(void *msg);
extern void mmi_prov_free_application_param_tonapid_value(void);
extern PROV_RETCODE mmi_prov_check_dup_application_param_tonapid_value(U8 *out_buffer);
extern PROV_RETCODE mmi_prov_fix_cca_doc(S32 hroot, PROV_RETCODE retcode);

#if defined(__MMI_VOIP_OTAP_DMP__) || defined(__MMI_WLAN_OTAP_DMP__) || defined(__MMI_FOTA_OTAP_DMP__)
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif /*#if defined(__MMI_VOIP_OTAP_DMP__) || defined(__MMI_WLAN_OTAP_DMP__) || defined(__MMI_FOTA_OTAP_DMP__)*/



#endif /* defined (JATAAYU_SUPPORT) && (defined (WAP_SUPPORT) || (defined (MMS_SUPPORT))) */ 
#endif /* __MMI_JATAAYU_CCA_SUPPORT__ */ 
#endif /* _JPROVCCAHANDLER_H */ /* _JPROVOMA_C */

