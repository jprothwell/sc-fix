/***************************************************************************
 * $Id: jdi_wmls.h,v 1.6 2008/01/30 14:17:02 kumardevhtmlbrow Exp $
 * $Revision: 1.6 $
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
 * $Log: jdi_wmls.h,v $
 * Revision 1.6  2008/01/30 14:17:02  kumardevhtmlbrow
 * Added the asynchronous way of send dtmf execution.
 *
 * Revision 1.5  2007/10/10 08:10:41  sriramdevhtmlbrow
 * Proper handling of variable substitution and memory error incase of wml script functions
 *
 * Revision 1.4  2005/12/12 15:29:01  kumardevhtmlbrow
 * Providing the logging framework for all the modules with the new JDD log abstraction
 *
 * Revision 1.3  2005/09/28 07:54:12  kumardevhtmlbrow
 * removed single line .. comment
 *
 * Revision 1.2  2005/09/12 12:20:43  kumardevhtmlbrow
 * no message
 *
 * Revision 1.1  2005/08/27 07:25:16  kumardevhtmlbrow
 * no message
 *
 *  
 ***************************************************************************/

#ifndef _JDI_WMLS_H
#define _JDI_WMLS_H

#include "jdi_browsermodule.h"

#define JC_ERR_WMLS_POP							TERMINAL_ERR_CODE(WMLSCRIPT_MODULE, 0x01)
#define JC_ERR_WMLS_PUSH						TERMINAL_ERR_CODE(WMLSCRIPT_MODULE, 0x02)
#define JC_ERR_WMLS_PARSING						TERMINAL_ERR_CODE(WMLSCRIPT_MODULE, 0x03)
#define JC_ERR_WMLS_INVALID_BYTE_CODE			TERMINAL_ERR_CODE(WMLSCRIPT_MODULE, 0x04)
#define JC_ERR_WMLS_ADD							TERMINAL_ERR_CODE(WMLSCRIPT_MODULE, 0x05)
#define JC_ERR_WMLS_STORE_VAR					TERMINAL_ERR_CODE(WMLSCRIPT_MODULE, 0x06)
#define JC_ERR_WMLS_ADD_VAR						TERMINAL_ERR_CODE(WMLSCRIPT_MODULE, 0x07)
#define JC_ERR_WMLS_DATA						TERMINAL_ERR_CODE(WMLSCRIPT_MODULE, 0x08)
#define JC_ERR_WMLS_URL							TERMINAL_ERR_CODE(WMLSCRIPT_MODULE, 0x09)
#define JC_ERR_WMLS_WTA							TERMINAL_ERR_CODE(WMLSCRIPT_MODULE, 0x0A)
#define JC_ERR_WMLS_VARNAME						TERMINAL_ERR_CODE(WMLSCRIPT_MODULE, 0x0B)
#define JC_ERR_WMLS_VARIABLE					TERMINAL_ERR_CODE(WMLSCRIPT_MODULE, 0x0C)
#define JC_ERR_WMLS_EXIT						TERMINAL_ERR_CODE(WMLSCRIPT_MODULE, 0x0D)
#define JC_ERR_WMLS_LOAD_CONST					TERMINAL_ERR_CODE(WMLSCRIPT_MODULE, 0x0E)
#define JC_ERR_WMLS_BROWSER						TERMINAL_ERR_CODE(WMLSCRIPT_MODULE, 0x0F)
#define JC_ERR_WMLS_INVALID_OFFSET				TERMINAL_ERR_CODE(WMLSCRIPT_MODULE, 0x10)
#define JC_ERR_WMLS_INVALID_VERSION				TERMINAL_ERR_CODE(WMLSCRIPT_MODULE, 0x11)
#define JC_ERR_WMLS_INVALID_CONSTANT_DATA		TERMINAL_ERR_CODE(WMLSCRIPT_MODULE, 0x12)
#define JC_ERR_WMLS_INVALID_PRAGMA_DATA			TERMINAL_ERR_CODE(WMLSCRIPT_MODULE, 0x13)
#define JC_ERR_WMLS_INVALID_EXT_FUNCTION_DATA	TERMINAL_ERR_CODE(WMLSCRIPT_MODULE, 0x14)
#define JC_ERR_WMLS_INVALID_FUNCTION_DATA		TERMINAL_ERR_CODE(WMLSCRIPT_MODULE, 0x15)
#define JC_ERR_WMLS_ACCESS_DENIED				TERMINAL_ERR_CODE(WMLSCRIPT_MODULE, 0x16)
#define JC_ERR_WMLS_BLOCKING_CALL				TERMINAL_ERR_CODE(WMLSCRIPT_MODULE, 0x17)
#define JC_ERR_WMLS_FUNCTION_NOT_AVAILABLE		TERMINAL_ERR_CODE(WMLSCRIPT_MODULE, 0x18)
#define JC_ERR_WMLS_WAIT_FOR_INPUT				TERMINAL_ERR_CODE(WMLSCRIPT_MODULE, 0x19)
#define JC_ERR_WMLS_NETWORK_NOT_AVAILABLE		TERMINAL_ERR_CODE(WMLSCRIPT_MODULE, 0x1A)
#define JC_ERR_WMLS_CALLED_PARTY_NOT_ANSWER		TERMINAL_ERR_CODE(WMLSCRIPT_MODULE, 0x1B)
#define JC_ERR_WMLS_CALLED_PARTY_BUSY			TERMINAL_ERR_CODE(WMLSCRIPT_MODULE, 0x1C)
#define JC_ERR_WMLS_INVALID_PB_NAME				TERMINAL_ERR_CODE(WMLSCRIPT_MODULE, 0x1D)
#define JC_ERR_WMLS_INVALID_PB_NUMBER			TERMINAL_ERR_CODE(WMLSCRIPT_MODULE, 0x1E)
#define JC_ERR_WMLS_INVALID_PB_ENTRY			TERMINAL_ERR_CODE(WMLSCRIPT_MODULE, 0x1F)
#define JC_ERR_WMLS_INVALID_PB_FULL				TERMINAL_ERR_CODE(WMLSCRIPT_MODULE, 0x20)
#define JC_ERR_WMLS_SETVAR						TERMINAL_ERR_CODE(WMLSCRIPT_MODULE, 0x21)
#define JC_ERR_WMLS_URL_INVALID_PORT_NUMBER		TERMINAL_ERR_CODE(WMLSCRIPT_MODULE, 0x22)
#define JC_ERR_WMLS								TERMINAL_ERR_CODE(WMLSCRIPT_MODULE, 0x23)


#define WMLS_ALERT_PAUSED					0
#define WMLS_PROMPT_PAUSED					1
#define WMLS_CONFIRM_PAUSED					2
#define WMLS_EXTERNAL_URL_PAUSED			3
#define WMLS_LOAD_STRING_PAUSED				4

struct InterpreterReturns
{
	JC_INT32 exit_flag ;
	JC_INT32 extern_url ;
	JC_INT8 *returnURL ;
	void *appArg ;
	void *last_node ;
	JC_UINT8 prevFlag ;
	JC_UINT8 goFlag ;
	struct wmls_wait_state *wmls_wait_info ;
} ;

struct dialog_box_state
{
	JC_INT8 * message ;
	struct input_tag
	{
		JC_INT8 * prompt ;
		struct confirm_tag
		{
			JC_INT8 * ok ;
			JC_INT8 * cancel ;
		} confirm ;
	} input ;

	struct result_tag
	{
		JC_INT8 * string ;
		JC_INT32	 status ;
	} result ;
} ;

struct external_url_state 
{
	JC_INT8 * parent_url ;
	JC_INT8 * script_url ;
	struct InterpreterReturns * iReturn ;
} ;

struct load_string
{
	JC_INT8 * url ;
	JC_INT8 * content_type ;
	JC_INT8 * load_content_type ;
	JC_INT32 status ;
	JC_INT8 * content ;
} ;

typedef enum _wta_event
{
	WTA_MAKE_CALL,
	WTA_ADD_PHONEBOOK,
	WTA_SEND_DTMF
} WTA_EVENT ;

struct wta_state
{
	WTA_EVENT wta_event ;
	JC_INT8 *name ;
	JC_INT8 *number ;
	JC_INT8 *dtmf ;
	JC_RETCODE wta_ret ;
} ;

/* Would be exposed to the application using WMLScriptInterpreter */
struct block_state
{
	JC_INT32	block_type ;
	struct dialog_box_state * dialog_box ;
	struct external_url_state *ext_url_info ;
	struct load_string	 *load_string_info ;
	struct wta_state *wta_info ;
	struct ret_tag
	{
		JC_INT8 * string ;
		JC_INT32	 status ;
	} ret ;
} ;

struct wmls_wait_state
{
	void * continue_info ; 
	struct block_state * block_info ;
	struct InterpreterReturns *intr_ptr ;
	JC_INT32 status ;
	struct wmls_wait_state *next ;
} ;

JC_RETCODE jdi_WMLSInterpreter (JC_INT32 length, JC_UINT8 *buff, const JC_INT8 *callingURL, 
							   const JC_INT8 *calledURL, struct InterpreterReturns *iReturn) ;

JC_RETCODE jdi_WMLSContinueInterpreter (struct wmls_wait_state * wmls_wait_info, 
									  struct InterpreterReturns ** iReturn) ;

JC_RETCODE wmls_parsevar (JC_INT8 *varName, void *appArg, JC_INT8 **varValue) ;

JC_RETCODE wmls_load_string (struct wmls_wait_state *wmls_wait_info) ;
JC_RETCODE wmls_call_external_url (struct wmls_wait_state *wmls_wait_info) ;
JC_RETCODE wta_confirmation (JC_INT8 * message, JC_INT32 function) ;

JC_INT8 *wmls_resolve_url (JC_INT8 *base_url, JC_INT8 *embedded_url) ;
JC_INT8 *wmls_getvar (JC_INT8 *varName, void *appArg) ;
JC_INT8 *wmls_new_context (void *appArg) ;

JC_RETCODE wmls_dialog_prompt (struct wmls_wait_state *wmls_wait_info) ;
JC_RETCODE wmls_dialog_alert (struct wmls_wait_state *wmls_wait_info) ;
JC_RETCODE wmls_dialog_confirm (struct wmls_wait_state *wmls_wait_info) ;
JC_RETCODE wmls_refresh (struct wmls_wait_state *wmls_wait_info) ;
JC_RETCODE wmls_make_call (struct wmls_wait_state *wmls_wait_info) ;
JC_RETCODE wmls_add_pb (struct wmls_wait_state *wmls_wait_info) ;
JC_RETCODE wmls_send_dtmf (struct wmls_wait_state *wmls_wait_info) ;
JC_RETCODE wmls_setvar (JC_INT8 *varName, JC_INT8 *varValue, void *appArg) ;

#endif	

/* End of File */


