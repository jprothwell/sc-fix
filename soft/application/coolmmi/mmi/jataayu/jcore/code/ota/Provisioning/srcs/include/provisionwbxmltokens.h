/***************************************************************************
 * $Id: ProvisionWbxmlTokens.h,v 1.3 2007/01/31 05:39:24 lumitdevpclient Exp $
 * $Revision: 1.3 $
 * $DateTime: $
 * 
 * IMPORTANT NOTICE
 *
 * Please note that any and all title and/or intellectual property rights 
 * in and to this Software or any part of this (including without limitation 
 * any images, photographs, animations, video, audio, music, text and/or 
 * "applets," incorporated into the Software), herein mentioned to as 
 * "Software", the accompanying printed materials, and any copies of the 
 * Software, are owned by Jataayu Software (P) Ltd., Bangalore ("Jataayu") 
 * or Jataayu's suppliers as the case may be. The Software is protected by 
 * copyright, including without limitation by applicable copyright laws, 
 * international treaty provisions, other intellectual property laws and 
 * applicable laws in the country in which the Software is being used. 
 * You shall not modify, adapt or translate the Software, without prior 
 * express written consent from Jataayu. You shall not reverse engineer, 
 * decompile, disassemble or otherwise alter the Software, except and 
 * only to the extent that such activity is expressly permitted by 
 * applicable law notwithstanding this limitation. Unauthorized reproduction 
 * or redistribution of this program or any portion of it may result in severe 
 * civil and criminal penalties and will be prosecuted to the maximum extent 
 * possible under the law. Jataayu reserves all rights not expressly granted. 
 * 
 * THIS SOFTWARE IS PROVIDED TO YOU "AS IS" WITHOUT WARRANTY OF ANY 
 * KIND AND ANY AND ALL REPRESENTATION AND WARRANTIES, EITHER EXPRESS 
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY ACCURACY OF INFORMATIONAL CONTENT, AND/OR FITNESS 
 * FOR A PARTICULAR PURPOSE OR USE, TITLE OR INFRINGEMENT ARE EXPRESSLY 
 * DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. YOU ASSUME THE 
 * ENTIRE RISK AS TO THE ACCURACY AND THE USE OF THIS SOFTWARE. JATAAYU 
 * SHALL NOT BE LIABLE FOR ANY CONSEQUENTIAL, INCIDENTAL, INDIRECT, 
 * EXEMPLARY, SPECIAL OR PUNITIVE DAMAGES INCLUDING WITHOUT LIMITATION 
 * ANY LOSS OF DATA, OR; LOSS OF PROFIT, SAVINGS BUSINESS OR GOODWILL 
 * OR OTHER SIMILAR LOSS RESULTING FROM OR OUT OF THE USE OR INABILITY 
 * TO USE THIS SOFTWARE, EVEN IF JATAAYU HAS BEEN ADVISED OF THE 
 * POSSIBILITY OF SUCH DAMAGE, OR FOR ANY CLAIM BY ANY THIRD PARTY.
 *
 ***************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: ProvisionWbxmlTokens.h,v $
 * Revision 1.3  2007/01/31 05:39:24  lumitdevpclient
 * Compiler warning and Lint warning fixed as reported by TTPCom
 *
 * Revision 1.2  2006/01/04 06:50:14  lumitdevpclient
 * committ after fixing some continuous provisioning related discripancies
 *
 * Revision 1.1  2005/09/28 12:51:29  arunpclient
 * Initial Version
 *
 *
 *  
 ***************************************************************************/

#ifndef PROVISION_WBXML_TAGS_H
#define PROVISION_WBXML_TAGS_H


/**< Macros				*/

#define WAP_PROV_SWITCH_PAGE					0x00		
#define OMA_PROV_SWITCH_PAGE					0x01		

/**< Tag tokens */

#define PROVT_WAP_PROVISIONING_DOC				0x05
#define PROVT_CHARACTERISTIC		    		0x06
#define PROVT_PARM					    		0x07

/**< Attribute start tokens */

#define PROVS_NAME								0x05
#define PROVS_VALUE								0x06
#define PROVS_NAME_NAME							0x07
#define PROVS_NAME_NAP_ADDRESS					0x08
#define PROVS_NAME_NAP_ADDRTYPE					0x09
#define PROVS_NAME_CALLTYPE			   			0x0A
#define PROVS_NAME_VALIDUNTIL       			0x0B
#define PROVS_NAME_AUTHTYPE            			0x0C	
#define PROVS_NAME_AUTHNAME            			0x0D
#define PROVS_NAME_AUTHSECRET          			0x0E	
#define PROVS_NAME_LINGER		           		0x0F
#define PROVS_NAME_BEARER						0x10
#define PROVS_NAME_NAPID						0x11
#define PROVS_NAME_COUNTRY						0x12
#define PROVS_NAME_NETWORK						0x13
#define PROVS_NAME_INTERNET						0x14
#define PROVS_NAME_PROXY_ID						0x15
#define PROVS_NAME_PROXY_PROVIDER_ID			0x16
#define PROVS_NAME_DOMAIN						0x17
#define PROVS_NAME_PROVURL						0x18
#define PROVS_NAME_PXAUTH_TYPE					0x19
#define PROVS_NAME_PXAUTH_ID					0x1A
#define PROVS_NAME_PXAUTH_PW					0x1B
#define PROVS_NAME_STARTPAGE					0x1C
#define PROVS_NAME_BASAUTH_ID					0x1D
#define PROVS_NAME_BASAUTH_PW					0x1E
#define	PROVS_NAME_PUSHENABLED					0x1F
#define PROVS_NAME_PXADDR						0x20
#define PROVS_NAME_PXADDRTYPE					0x21
#define PROVS_NAME_TO_NAPID						0x22
#define PROVS_NAME_PORTNBR						0x23
#define PROVS_NAME_SERVICE						0x24
#define PROVS_NAME_LINKSPEED					0x25
#define PROVS_NAME_DNLINKSPEED					0x26
#define PROVS_NAME_LOCAL_ADDR					0x27
#define PROVS_NAME_LOCAL_ADDR_TYPE				0x28
#define PROVS_NAME_CONTEXT_ALLOW				0x29
#define PROVS_NAME_TRUST						0x2A
#define PROVS_NAME_MASTER						0x2B
#define PROVS_NAME_SID							0x2C
#define PROVS_NAME_SOC							0x2D
#define PROVS_NAME_WSP_VERSION					0x2E
#define PROVS_NAME_PHYSICAL_PROXY_ID			0x2F
#define PROVS_NAME_CLIENT_ID					0x30
#define PROVS_NAME_DELIVERY_ERR_SDU				0x31
#define PROVS_NAME_DELIVERY_ORDER				0x32
#define PROVS_NAME_TRAFFIC_CLASS				0x33
#define PROVS_NAME_MAX_SDU_SIZE					0x34
#define PROVS_NAME_MAX_BITRATE_UPLINK			0x35
#define PROVS_NAME_MAX_BITRATE_DNLINK			0x36
#define PROVS_NAME_RESIDUAL_BER					0x37
#define PROVS_NAME_SDU_ERROR_RATIO				0x38
#define PROVS_NAME_TRAFFIC_HANDL_PRIO			0x39
#define PROVS_NAME_TRANSFER_DELAY				0x3A
#define PROVS_NAME_GUARANTEED_BITRATE_UPLINK	0x3B
#define PROVS_NAME_GUARANTEED_BITRATE_DNLINK	0x3C

/**< Srart - Arun added on 02/12/2005 */

#define PROVS_NAME_PXADDR_FQDN					0x3D
#define PROVS_NAME_PROXY_PW						0x3E
#define PROVS_NAME_PPGAUTH_TYPE					0x3F
#define PROVS_NAME_PULLENABLED					0x47
#define PROVS_NAME_DNS_ADDR						0x48
#define PROVS_NAME_MAX_NUM_RETRY				0x49
#define PROVS_NAME_FIRST_RETRY_TIMEOUT			0x4A
#define PROVS_NAME_REREG_THRESHOLD				0x4B
#define PROVS_NAME_T_BIT						0x4C
#define PROVS_NAME_AUTH_ENTITY					0x4E
#define PROVS_NAME_SPI							0x4F

/**< End - Arun added on 02/12/2005 */



#define PROVS_VERSION							0x45
#define PROVS_VERSION_1_0						0x46

#define PROVS_TYPE								0x50
#define PROVS_TYPE_PXLOGICAL					0x51
#define PROVS_TYPE_PXPHYSICAL					0x52
#define PROVS_TYPE_PORT							0x53
#define PROVS_TYPE_VALIDITY						0x54
#define PROVS_TYPE_NAPDEF						0x55
#define PROVS_TYPE_BOOTSTRAP					0x56
#define PROVS_TYPE_VENDORCONFIG					0x57
#define PROVS_TYPE_CLIENTIDENTITY				0x58
#define PROVS_TYPE_PXAUTHINFO					0x59
#define PROVS_TYPE_NAPAUTHINFO					0x5A
#define PROVS_TYPE_ACCESS						0x5B

/**< Provisiong Parameter Token Values **************/

#define PROVA_IPV4								0x85
#define PROVA_IPV6								0x86
#define PROVA_E164								0x87
#define PROVA_ALPHA								0x88
#define PROVA_APN								0x89
#define PROVA_SCODE								0x8A
#define PROVA_TETRA_ITSI						0x8B
#define PROVA_MAN								0x8C

#define PROVA_ANALOG_MODEM						0x90
#define PROVA_V_120								0x91
#define PROVA_V_110								0x92
#define PROVA_X_31								0x93
#define PROVA_BIT_TRANSPARENT					0x94
#define PROVA_DIRECT_ASYNCHRONOUS_DATA_SERVICE	0x95

#define PROVA_PAP								0x9A
#define PROVA_CHAP								0x9B
#define PROVA_HTTP_BASIC						0x9C
#define PROVA_HTTP_DIGEST						0x9D
#define PROVA_WTLS_SS							0x9E

#define PROVA_GSM_USSD							0xA2
#define PROVA_GSM_SMS							0xA3
#define PROVA_ANSI_136_GUTS						0xA4
#define PROVA_IS_95_CDMA_SMS					0xA5
#define PROVA_IS_95_CDMA_CSD					0xA6
#define PROVA_IS_95_CDMA_PACKET					0xA7
#define PROVA_ANSI_136_CSD						0xA8
#define PROVA_ANSI_136_GPRS						0xA9
#define PROVA_GSM_CSD							0xAA
#define PROVA_GSM_GPRS							0xAB
#define PROVA_AMPS_CDPD							0xAC
#define PROVA_PDC_CSD							0xAD
#define PROVA_PDC_PACKET						0xAE
#define PROVA_IDEN_SMS							0xAF
#define PROVA_IDEN_CSD							0xB0
#define PROVA_IDEN_PACKET						0xB1
#define PROVA_FLEX_REFLEX						0xB2
#define PROVA_PHS_SMS							0xB3
#define PROVA_PHS_CSD							0xB4
#define PROVA_TETRA_SDS							0xB5
#define PROVA_TETRA_PACKET						0xB6
#define PROVA_ANSI_136_GHOST					0xB7
#define PROVA_MOBITEX_MPAK						0xB8

#define PROVA_AUTOBAUDING						0xC5

#define PROVA_CL_WSP							0xCA
#define PROVA_CO_WSP							0xCB
#define PROVA_CL_SEC_WSP						0xCC
#define PROVA_CO_SEC_WSP						0xCD
#define PROVA_CL_SEC_WTA						0xCE
#define PROVA_CO_SEC_WTA						0xCF

#define PROV_TAGS_MIN							PROVT_WAP_PROVISIONING_DOC
#define PROV_TAGS_MAX							PROVT_PARM

#define PROV_TAG_DESC(a) (((a>=PROV_TAGS_MIN)&&(a<=PROV_TAGS_MAX))?pacProvTags[(a-PROV_TAGS_MIN)]:JC_NULL)

#define PROV_SATTR_MIN PROVS_NAME								
#define PROV_SATTR_MAX PROVS_TYPE_ACCESS

#define PROV_SATTR_DESC(a) (((a>=PROV_SATTR_MIN)&&(a<=PROV_SATTR_MAX))?pacProvStartAttributes[(a-PROV_SATTR_MIN)]:JC_NULL)

#define PROV_VATTR_MIN PROVA_IPV4
#define PROV_VATTR_MAX PROVA_CO_SEC_WTA

#define PROV_VATTR_DESC(a) (((a>=PROV_VATTR_MIN)&&(a<=PROV_VATTR_MAX))?pacProvValueAttributes[(a-PROV_VATTR_MIN)]:JC_NULL)




/* Add for OMA Provisioning */
/******* OMA Provisioning Parameter Token Values **************/

/* Tag tokens */

#define OMA_PROVT_CHARACTERISTIC	    		0x06
#define OMA_PROVT_PARM				    		0x07

/* Attribute start tokens */ 

#define OMA_PROVS_NAME							0x05
#define OMA_PROVS_VALUE							0x06
#define OMA_PROVS_NAME_VALUE					0x07

#define OMA_PROVS_NAME_INTERNET					0x14

#define OMA_PROVS_NAME_STARTPAGE				0x1C

#define OMA_PROVS_NAME_TO_NAPID					0x22
#define OMA_PROVS_NAME_PORTNBR					0x23
#define OMA_PROVS_NAME_SERVICE					0x24

#define OMA_PROVS_NAME_AACCEPT					0x2E
#define OMA_PROVS_NAME_AAUTHDATA				0x2F
#define OMA_PROVS_NAME_AAUTHLEVEL				0x30
#define OMA_PROVS_NAME_AAUTHNAME				0x31
#define OMA_PROVS_NAME_AAUTHSECRET				0x32
#define OMA_PROVS_NAME_AAUTHTYPE				0x33
#define OMA_PROVS_NAME_ADDR						0x34
#define OMA_PROVS_NAME_ADDRTYPE					0x35
#define OMA_PROVS_NAME_APPID					0x36
#define OMA_PROVS_NAME_APROTOCOL				0x37
#define OMA_PROVS_NAME_PROVIDER_ID				0x38
#define OMA_PROVS_NAME_TO_PROXY					0x39
#define OMA_PROVS_NAME_URI						0x3A
#define OMA_PROVS_NAME_RULE						0x3B


#define OMA_PROVS_TYPE							0x50

#define OMA_PROVS_TYPE_PORT						0x53

#define OMA_PROVS_TYPE_APPLICATION				0x55
#define OMA_PROVS_TYPE_APPADDR					0x56
#define OMA_PROVS_TYPE_APPAUTH					0x57
#define OMA_PROVS_TYPE_CLIENTIDENTITY			0x58
#define OMA_PROVS_TYPE_RESOURCE					0x59

/* OMA Provisioning Parameter Token Values  */

#define OMA_PROVS_COMMA							0x80
#define OMA_PROVS_HTTP_							0x81
#define OMA_PROVS_BASIC							0x82
#define OMA_PROVS_DIGEST						0x83

#define OMA_PROVS_IPV6							0x86
#define OMA_PROVS_E164							0x87
#define OMA_PROVS_ALPHA							0x88

#define OMA_PROVS_APPSRV						0x8D
#define OMA_PROVS_OBEX							0x8E

#define OMA_PROVS_CL_WSP						0xCA
#define OMA_PROVS_CO_WSP						0xCB
#define OMA_PROVS_CL_SEC_WSP					0xCC
#define OMA_PROVS_CO_SEC_WSP					0xCD
#define OMA_PROVS_CL_SEC_WTA					0xCE
#define OMA_PROVS_CO_SEC_WTA					0xCF

#define OMA_PROV_TAGS_MIN						OMA_PROVT_CHARACTERISTIC
#define OMA_PROV_TAGS_MAX						OMA_PROVT_PARM
#define OMA_PROV_SATTR_MIN						OMA_PROVS_NAME								
#define OMA_PROV_SATTR_MAX						OMA_PROVS_TYPE_RESOURCE				
#define OMA_PROV_VATTR_MIN						OMA_PROVS_COMMA
#define OMA_PROV_VATTR_MAX						OMA_PROVS_CO_SEC_WTA


#define OMA_PROV_TAG_DESC(a) (((a>=OMA_PROV_TAGS_MIN)&&(a<=OMA_PROV_TAGS_MAX))?pacOmaProvTags[(a-OMA_PROV_TAGS_MIN)]:JC_NULL)


#define OMA_PROV_SATTR_DESC(a) (((a>=OMA_PROV_SATTR_MIN)&&(a<=OMA_PROV_SATTR_MAX))?pacOmaProvStartAttributes[(a-OMA_PROV_SATTR_MIN)]:JC_NULL)


#define OMA_PROV_VATTR_DESC(a) (((a>=OMA_PROV_VATTR_MIN)&&(a<=OMA_PROV_VATTR_MAX))?pacOmaProvValueAttributes[(a-OMA_PROV_VATTR_MIN)]:JC_NULL)


/**< Arun added some generic TAGs here */

#define PROV_WBXML_END_TAG				0x01
#define PROV_WBXML_START_INLINE_STRING	0x03
#define PROV_WBXML_END_INLINE_STRING	0x00
#define PROV_WBXML_SWITCH				0x00


/**< 00xx xxxx - Here No attrubut and no content*/
#define ENCODE_TAG_NO_ATTR_NO_CONT(ucEncodeValue) (((ucEncodeValue) & 0x3F) & 0xFF)

/**< 01xx xxxx - Here No attrubut will be there but content*/
#define ENCODE_TAG_NO_ATTR_WITH_CONT(ucEncodeValue) (( 0x40 | ((ucEncodeValue) & 0x3F)) & 0xFF)


/**< 10xx xxxx - Here Attrubut is there but no content*/
#define ENCODE_TAG_WITH_ATTR_NO_CONT(ucEncodeValue) (( 0x80 | ((ucEncodeValue) & 0x3F)) & 0xFF)

/**< 11xx xxxx - Here both Attrubut and content are there*/
#define ENCODE_TAG_WITH_ATTR_WITH_CONT(ucEncodeValue) (( 0xC0 | (ucEncodeValue)) & 0xFF)

/**< Function Prototypes */



#endif /**< PROVISION_WBXML_TAGS_H */

/*** END OF FILE ***/
