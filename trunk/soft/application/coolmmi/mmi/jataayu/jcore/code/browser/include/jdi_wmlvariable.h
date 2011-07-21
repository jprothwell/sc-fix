/****************************************************************
 * $Id: jdi_wmlvariable.h,v 1.8 2006/04/22 06:53:15 kumardevhtmlbrow Exp $
 * $Revision: 1.8 $
 * $DateTime: $
 * 
 * IMPORTANT NOTICE
 * Please note that any and all title and/or intellectual property 
 * rights in and to this Software or any part of this	(including 
 * without limitation any images, photographs, animations,	video, 
 * audio, music, text and/or "applets," incorporated into	   the 
 * Software), herein mentioned to as "Software", the  accompanying 
 * printed materials, and any copies of the Software, are owned by 
 * Jataayu Software (P) Ltd., Bangalore ("Jataayu") or	 Jataayu's 
 * suppliers as the case may be. The Software is protected		by 
 * copyright, including without limitation by applicable copyright 
 * laws, international treaty provisions,	other	  intellectual 
 * property laws and applicable laws in the country in	which  the 
 * Software is being used. You shall not modify,adapt or translate 
 * the Software,without prior express written consent from Jataayu.
 * You shall not reverse engineer, decompile,	disassemble		or 
 * otherwise alter the Software,except and only to the extent that 
 * such activity is  expressly  permitted   by   applicable	   law 
 * notwithstanding this limitation. Unauthorized  reproduction  or 
 * redistribution of this program or any  portion of it may result 
 * in severe civil and criminal penalties and will  be  prosecuted 
 * to the maximum extent possible under the law.  Jataayu reserves 
 * all rights not expressly granted.
 *
 * THIS SOFTWARE IS PROVIDED TO YOU "AS IS" WITHOUT WARRANTY OF ANY 
 * KIND AND ANY AND ALL  REPRESENTATION  AND   WARRANTIES,   EITHER 
 * EXPRESS OR IMPLIED, INCLUDING  BUT  NOT  LIMITED  TO THE IMPLIED 
 * WARRANTIES OF MERCHANTABILITY ACCURACY OF INFORMATIONAL CONTENT, 
 * AND/OR FITNESS FOR A  PARTICULAR  PURPOSE  OR  USE,  TITLE    OR 
 * INFRINGEMENT ARE  EXPRESSLY  DISCLAIMED  TO  THE  FULLEST EXTENT 
 * PERMITTED BY LAW. YOU ASSUME THE ENTIRE RISK AS TO THE  ACCURACY 
 * AND THE USE OF THIS SOFTWARE.JATAAYU SHALL NOT BE LIABLE FOR ANY 
 * CONSEQUENTIAL,  INCIDENTAL,  INDIRECT,  EXEMPLARY,   SPECIAL  OR 
 * PUNITIVE DAMAGES INCLUDING  WITHOUT LIMITATION ANY LOSS OF DATA, 
 * OR; LOSS OF PROFIT,SAVINGS BUSINESS OR GOODWILL OR OTHER SIMILAR 
 * LOSS RESULTING FROM  OR OUT OF  THE USE OR INABILITY TO USE THIS 
 * SOFTWARE, EVEN IF JATAAYU HAS BEEN ADVISED OF THE POSSIBILITY OF 
 * SUCH DAMAGE, OR FOR ANY CLAIM BY ANY THIRD PARTY.
 ******************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: jdi_wmlvariable.h,v $
 * Revision 1.8  2006/04/22 06:53:15  kumardevhtmlbrow
 * LINT warning ....
 *
 * Revision 1.7  2005/09/12 12:20:43  kumardevhtmlbrow
 * no message
 *
 * Revision 1.6  2005/09/03 10:38:43  kumardevhtmlbrow
 * no message
 *
 * Revision 1.5  2005/08/17 06:23:56  kumardevhtmlbrow
 * Updated the jdd_MemAlloc to jdi_MemAlloc function.
 *
 * Revision 1.4  2005/07/15 15:40:23  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.3  2005/06/24 14:23:09  kumardevhtmlbrow
 * no message
 *
 * Revision 1.2  2005/06/24 10:40:50  kumardevhtmlbrow
 * no message
 *
 * Revision 1.1  2005/06/17 05:09:46  kumardevhtmlbrow
 * Initial version of WML Variable
 *
 *  
 *****************************************************************/
#ifndef _JDI_WML_VARIABLE
#define _JDI_WML_VARIABLE

#define	DEFAULT_MAX_NODE		50
#define DEFAULT_ALLOCATED_LEN	1024
#define DOLLAR_SYMBOL			'$'

#ifdef __cplusplus
extern "C" {
#endif

typedef enum wml_var_escape_seq
{
	E_WML_VAR_ESCAPE,
	E_WML_VAR_UNESCAPE,
	E_WML_VAR_DONOTHING
} E_WML_VAR_ESCAPE_SEQ ;

typedef struct _wml_var_list
{
	JC_CHAR					*pmVariableName ;
	JC_CHAR					*pmVariableValue ;
	JC_UINT64				ulHashKey ;
	struct _wml_var_list	*pstNext ;
} ST_WML_VAR_LIST ;

typedef struct _variable_handler_context
{
	JC_HANDLE				vCMemHandle ;
	ST_WML_VAR_LIST			*pstNodeList ;
	ST_WML_VAR_LIST			*pstLastNode ;
	JC_HANDLE				vMemHandle ;
} ST_WML_VAR_CONTEXT ;

JC_RETCODE jdi_WMLVarInitialize (JC_HANDLE vMemHandle, JC_HANDLE *pvHandle) ;

JC_RETCODE jdi_WMLVarAdd (JC_HANDLE vHandle, 
						  const JC_CHAR *pmVariableName, 
						  const JC_CHAR *pmVariableValue, 
						  JC_BOOLEAN bIsAppend) ;

JC_RETCODE jdi_WMLVarGet (JC_HANDLE vHandle, 
						 const JC_CHAR *pmVariableName, 
						 JC_BOOLEAN bIsWMLS, 
						 JC_CHAR **ppmValue) ;

void jdi_WMLVarClearList (JC_HANDLE vHandle) ;

JC_RETCODE jdi_WMLVarSubstitute (JC_HANDLE vHandle, 
								const JC_CHAR *pmVariable,
								E_WML_VAR_ESCAPE_SEQ eEscapeSeq,
								JC_CHAR	 **ppmSubstitutedVar) ;

void jdi_WMLVarDeinitialize (JC_HANDLE vHandle) ;

#ifdef __cplusplus
}
#endif

#endif

/* End of File */



