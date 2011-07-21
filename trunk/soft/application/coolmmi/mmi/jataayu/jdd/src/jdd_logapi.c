#define MODULE_NAME "JDD_LOGAPI" 
#define FILE_NAME	"jdd_logapi.c"
/***************************************************************************
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
 ***************************************************************************/

 /** 
 * @file	jdd_logapi.c
 * @ingroup	jdd_logapi
 * @brief	Describes the log related API's.
 **/
#ifdef JATAAYU_SUPPORT 
/***************************************************************************
 * System Include Files
 **************************************************************************/
 /*---Platform specific implementation Start---*/
 /* For platforms supporting variable argument list, include platform's 
  * specific standard argument header file for e.g. as mentioned below */
 
 /* #include <stdarg.h> */
 /*---Platform specific implementation End---*/
 /***************************************************************************
 * User Include Files
 **************************************************************************/
#include <ddl.h>
#include <jcal.h>
#include <jdi_cutils.h>

 /***************************************************************************
 * Macros
 **************************************************************************/
#ifdef JDD_LOG_ENABLED
#define __MODULE_ID__					LOG_MODULE
#else
#define __MODULE_ID__					0
#endif

/*---Platform specific implementation Start---*/
/* Below mentioned macros to be configured based on platform */
#define	LOG_MAX_VAR_ARG		250
#define LOG_MIN_VAR_ARG		80
#define MAX_DISP_LEN		80
/* 
 * Macro to enable with maximum file size value, if platform have maximum
 * file size limit. Disable if platform don't have file size limitation.
 */
#define	LOG_MAX_FILE_SIZE	(1000 * 1024)
#define JDD_LOG_FILE_NAME	"logfile.log" 
#define	JDD_MODULE_NAME		0x00ff0000

#define FILE_LOG /*Macro to enable logs for perticular files*/
/*---Platform specific implementation End---*/

 /***************************************************************************
 * Global Variable declarations and definitions
 **************************************************************************/
 JDD_FILE g_pLogHandle ; /*Global handle used in jdd_LogInitialize. This can 
						  be commented in case platform does not support file 
						  based logging*/
#ifdef FILE_LOG
 static const JC_INT8 * const g_pLogFileName [] =
 {
/*	"jdi_transport.c",
	"trutils.c",
*/	 JC_NULL
 };
#endif //FILE_LOG

 
 /***************************************************************************
 * Local Function Prototypes
 **************************************************************************/
#if 0
 static JC_INT8 *logConvertVarArgToString (va_list *pvList, \
										  JC_INT8 cFmtChar, \
										  JC_INT8 cFmtNextChar) ;
static JC_CHAR* getLogFileName(void) ;
static void writeLogsToFile(JC_INT8 *pBuffer, \
								  JC_UINT32 uiBufferLen) ;
static void useModule(JC_INT32 moduleId,JC_INT8 * pcModuleName) ;
#endif
/***************************************************************************
 * All Local Function Definitions
 **************************************************************************/

/**
 * @brief 		This function writes logs to a file
 * @param[in]	ucBuffer Log Buffer to be written to File
 * @param[in]	uiBufLen Length of the Log Buffer
 *
 */
#if 0
static void writeLogsToFile (JC_INT8 *ucBuffer, \
								   JC_UINT32 uiBufLen )
{
	JC_UINT32 uiFileSize = 0;

	/* if platform had maximum file size limit */
#ifdef LOG_MAX_FILE_SIZE
	jdd_FSGetSize(g_pLogHandle, &uiFileSize) ;
	if(uiFileSize > LOG_MAX_FILE_SIZE)
	{
		jdd_FSClose (g_pLogHandle) ;		
		/* if you want to delete file (for platform needed) */
		jdd_FSDelete (JC_NULL, (const JC_CHAR *)JDD_LOG_FILE_NAME) ;
		
		g_pLogHandle = (JDD_FILE)jdd_FSOpen(JC_NULL, \
							(const JC_CHAR *)JDD_LOG_FILE_NAME, \
							E_CREATE_APPEND_WRITE_MODE) ;
	}
#endif /* LOG_MAX_FILE_SIZE */

	if(g_pLogHandle)
	{
		jdd_FSWrite ((void*)ucBuffer, sizeof( JC_UINT8 ), \
								uiBufLen, g_pLogHandle) ;
	}	
}

/**
 * @brief 		This function converts the variable argument to string
 * @param[in]	vList Specifies variable argument list
 * @param[in]	cFmtChar Character from format string which decides the	
 *				arguments
 * @param[in]	cFmtNextChar Next character to cFmtChar in the format string
 * @retval		The string form of variable argument
 *
 * It retrives the argument from variable argument list which is identified
 * by the character from format string.
 */

static JC_INT8* logConvertVarArgToString (va_list *vList, \
										  JC_INT8 cFmtChar, \
										  JC_INT8 cFmtNextChar)
{
	

	return JC_NULL ;

#if 0

	JC_INT8 *pcVarArg = JC_NULL ;

	
	pcVarArg = (JC_INT8 *) jdd_MemAlloc (LOG_MIN_VAR_ARG, \
										sizeof (JC_INT8)) ;
	if (JC_NULL != pcVarArg)
	{
		switch (cFmtChar)
		{
		case 'c' :
			jc_snprintf (pcVarArg, (LOG_MIN_VAR_ARG - 1), \
							(const JC_INT8*)"%c", va_arg (*vList, JC_INT8)) ;
			break ;
		case 'd' :
			jc_snprintf (pcVarArg, (LOG_MIN_VAR_ARG - 1), \
							(const JC_INT8*)"%d", va_arg (*vList, JC_INT32)) ;
			break ;
		case 'e' :
			jc_snprintf (pcVarArg, (LOG_MIN_VAR_ARG - 1), \
							(const JC_INT8*)"%f", va_arg (*vList, JC_DOUBLE)) ;
			break ;
		case 'f' :
			jc_snprintf (pcVarArg, (LOG_MIN_VAR_ARG - 1), \
							(const JC_INT8*)"%f", va_arg (*vList, JC_DOUBLE)) ;
			break ;
		case '%' :
			jc_snprintf (pcVarArg, (LOG_MIN_VAR_ARG - 1), \
							(const JC_INT8*)"%c", cFmtChar) ;
			break ;
		case 'l' :
			{
				if ('d' == cFmtNextChar)
				{
					jc_snprintf (pcVarArg, (LOG_MIN_VAR_ARG - 1), \
								(const JC_INT8*)"%ld", \
								va_arg (*vList, JC_INT32)) ;
				}
				else if ('u' == cFmtNextChar)
				{
					jc_snprintf (pcVarArg, (LOG_MIN_VAR_ARG - 1), \
								(const JC_INT8*)"%lu", \
								va_arg (*vList, JC_UINT32)) ;
				}
			}
			break ;
		case 'p' :
			{
				if(pcVarArg)
				{
					jdd_MemFree (pcVarArg) ;
				}
				pcVarArg = (JC_INT8 *) jdd_MemAlloc (LOG_MAX_VAR_ARG, \
													sizeof (JC_INT8)) ;
				if (JC_NULL != pcVarArg)
				{
					jc_snprintf (pcVarArg, (LOG_MAX_VAR_ARG - 1), \
									(const JC_INT8*)"%p", \
									va_arg (*vList, void *) ) ;
					pcVarArg [LOG_MAX_VAR_ARG - 1] = '\0' ;
				}
			}
			break ;
		case 's' :
			{
				if(pcVarArg)
				{
					jdd_MemFree (pcVarArg) ;
				}
				pcVarArg = (JC_INT8 *) jdd_MemAlloc (LOG_MAX_VAR_ARG, \
													sizeof (JC_INT8)) ;
				if (JC_NULL != pcVarArg)
				{
					jc_snprintf (pcVarArg, (LOG_MAX_VAR_ARG - 1), \
								(const JC_INT8*)"%s", \
								va_arg (*vList, JC_INT8 *)) ;
					pcVarArg [LOG_MAX_VAR_ARG - 1] = '\0';
				}
			}
			break ;
		case 'u' :
			jc_snprintf (pcVarArg, (LOG_MIN_VAR_ARG - 1), \
									(const JC_INT8*)"%u", \
									va_arg (*vList, JC_UINT32)) ;
			break ;
		case 'x' :
			jc_snprintf (pcVarArg, (LOG_MIN_VAR_ARG - 1), \
									(const JC_INT8*)"%x", \
									va_arg (*vList, JC_INT32)) ;
			break ;
		default :
			break ;
		}
	}
	return  pcVarArg ;
#endif
}


/**
 * @brief 		The Log layer shall use this API to provide fileName to the 
 *				JDD LOG Module.
 *  
 * This function should be invoked by the application layer before 
 * initializing JDD_LOG
 */ 

static JC_CHAR* getLogFileName(void)
{
	return (JC_CHAR*)JDD_LOG_FILE_NAME ;
}

/**
 * @brief 		Converts Module number to module name
 * @param[in]	moduleId Module ID.
 * @param[in]	pcModuleName Module Number.
 *				
 * This function should be invoked by the JDD_LOG to Convert Module 
 * number to module name
 */ 

static void useModule(JC_INT32 moduleId, JC_INT8 * pcModuleName)
{
    switch (moduleId & JDD_MODULE_NAME)
    {
	    case COMMON_APPLICATION:
	        jc_memcpy(pcModuleName,"COMMON_APPLICATION", \
							jc_strlen((JC_INT8*)"COMMON_APPLICATION")+ 1) ;		
	        break ;		
		case BROWSER_APPLICATION:
	        jc_memcpy(pcModuleName,"BROWSER_APPLICATION", \
							jc_strlen((JC_INT8*)"BROWSER_APPLICATION")+ 1) ;		
	        break ; 		
		case MMS_APPLICATION :
	        jc_memcpy(pcModuleName,"MMS_APPLICATION", \
							jc_strlen((JC_INT8*)"MMS_APPLICATION")+ 1) ;		
	        break ;	     	
		case IM_APPLICATION	 :
	        jc_memcpy(pcModuleName,"IM_APPLICATION", \
							jc_strlen((JC_INT8*)"IM_APPLICATION")+ 1) ;		
	        break ;    		
		case DM_APPLICATION	 :
	        jc_memcpy(pcModuleName,"DM_APPLICATION", \
							jc_strlen((JC_INT8*)"DM_APPLICATION")+ 1) ;		
	        break ;    		
		case EMAIL_APPLICATION  :
	        jc_memcpy(pcModuleName,"EMAIL_APPLICATION", \
							jc_strlen((JC_INT8*)"EMAIL_APPLICATION")+ 1) ;		
	        break ;   		
		case EMS_APPLICATION:
	        jc_memcpy(pcModuleName,"EMS_APPLICATION", \
							jc_strlen((JC_INT8*)"EMS_APPLICATION")+ 1) ;		
	        break ;       		
		case PROVISIONING_APPLICATION:
	        jc_memcpy(pcModuleName,"PROVISIONING_APPLICATION", \
						jc_strlen((JC_INT8*)"PROVISIONING_APPLICATION")+ 1) ;		
	        break ;	
		case TRANSPORT_APPLICATION:
	        jc_memcpy(pcModuleName,"TRANSPORT_APPLICATION", \
						jc_strlen((JC_INT8*)"TRANSPORT_APPLICATION")+ 1) ;		
	        break ;		
		case ABSTRACT_APPLICATION	:
	        jc_memcpy(pcModuleName,"ABSTRACT_APPLICATION", \
						jc_strlen((JC_INT8*)"ABSTRACT_APPLICATION")+ 1) ;		
	        break ;	
		case DOWNLOAD_CLIENT_APPLICATION:
	        jc_memcpy(pcModuleName,"COMMON_APPLICATION", \
						jc_strlen((JC_INT8*)"COMMON_APPLICATION")+ 1) ;		
	        break ;	
		case SCRIPT_ENGINE_APPLICATION:
	        jc_memcpy(pcModuleName,"SCRIPT_ENGINE_APPLICATION", \
						jc_strlen((JC_INT8*)"SCRIPT_ENGINE_APPLICATION")+ 1) ;
	        break ;	
		/*	case RSS_CLIENT_APPLICATION	:
	        jc_memcpy(pcModuleName,"RSS_CLIENT_APPLICATION", \
						jc_strlen((JC_INT8*)"RSS_CLIENT_APPLICATION")+ 1) ;		
	        break; */
		default:
			jc_memcpy(pcModuleName,"UNKNOWN_APPLICATION", \
							jc_strlen((JC_INT8*)"UNKNOWN_APPLICATION")+ 1) ;	
	        break ;		
    }
}

#endif
 /***************************************************************************
 * All Global Function Definitions
 **************************************************************************/

/**
 * @ingroup		jdd_logapi
 * @brief 		This function initializes the Log layer
 * @param [in]	pvLogReference Specifies the reference which shall be used
 *				by the log module
 * 
 * This function should be invoked by the application layer before invoking
 * the Jataayu terminal components.
 */

void jdd_LogInitialize (void *pvLogReference)
{
#if 0
	JC_CHAR* pLogFileName = JC_NULL ;
	/*--- Platform Specific Variables ---*/
	/*-----------------------------------*/


	/*--- Platform Specific Implementation Start ---*/

	PARAM_INTENTIONALLY_NOT_USED(pvLogReference) ;


	/*--- Platform Specific Implementation End ---*/
	
	if(JC_NULL != (pLogFileName = getLogFileName()))	
	{
		g_pLogHandle = jdd_FSOpen (JC_NULL, pLogFileName, \
								E_CREATE_WRITE_READ_MODE) ;			
	}
#endif //0
}


/**
 * @ingroup		jdd_logapi
 * @brief 		This function De-Initializes  Log layer
 * @param[in]	pvLogReference Specifies the reference which shall be used
 *				by the log module
 * 
 * This function should be invoked by the application layer after 
 * deinitializing all the Jataayu terminal components.
 */

void jdd_LogDeInitialize (void *pvLogReference)
{
#if 0
	/*--- Platform Specific Variables ---*/
	/*-----------------------------------*/

	PARAM_INTENTIONALLY_NOT_USED(pvLogReference) ;


	/*--- Platform Specific Implementation Start ---*/

	/*--- Platform Specific Implementation End ---*/


	jdd_FSClose (g_pLogHandle) ;
	g_pLogHandle = JC_NULL ;
#endif
}

 
/**
 * @ingroup		jdd_logapi
 * @brief 		This function is used for logging the message
 * @param[in]	eLogInfo The log level 
 *				(e.g. LOG_LEVEL_INFORMATION/LOG_LEVEL_WARNING/LOG_LEVEL_FATAL)
 * @param[in]	iModuleNum The ID of the module where the log is placed
 * @param[in]	pcFileName The full path of the source file where the log is
 *              placed, or module name or JC_NULL if LOG_FILE_NAMES is 
 *				turned off.
 * @param[in]	iLineNumber The line number in the source file where the 
 *				log is placed
 * @param[in]	pcFormat The printf-like format string of the log statement.
 */

void jdd_LogMsg (JC_INT32 eLogInfo, \
				JC_INT32 iModuleNum,\
				const JC_INT8 *pcFileName, \
				JC_INT32 iLineNumber, \
				const JC_INT8 *pcFormat, ...)
{
#if 0
#ifdef _VA_LIST_DEFINED
	
	va_list			vList ;
	const JC_INT8	*pcFmtPtr ;
	JC_INT8			*pcFmtdMsg = JC_NULL ;
	JC_INT8			*pcVarArg ;
	JC_UINT32		uiMsgLen = LOG_MAX_VAR_ARG ;
	JC_UINT32		uiOrgLen = 0 ;
	JC_INT8		    aWriteBuffer[LOG_MAX_VAR_ARG] = {0,} ;
	JC_UINT32       uiCount ;
	JC_INT8			aModuleName[25];
#ifdef FILE_LOG
	JC_INT32		i=0 ;
	JC_BOOLEAN		bLogFile = E_FALSE ;

#endif //FILE_LOG
	jdd_AssertOnFail ((JC_NULL != pcFileName), \
						"jdd_LogMsg: pcFileName is NULL") ;
	jdd_AssertOnFail ((JC_NULL != pcFormat), \
						"jdd_LogMsg: pcFormat is NULL") ;	
#ifdef FILE_LOG
	/*Check to enable logs only for Some files (present in g_pLogFileName) */
	while (g_pLogFileName[i] != JC_NULL)
	{
		if (jc_strstr(g_pLogFileName[i], pcFileName))
		{
			bLogFile = E_TRUE ;
			break ;
		}
		i++ ;
	}
if (bLogFile)
{
#endif //FILE_LOG
	va_start (vList, pcFormat) ;
	PARAM_INTENTIONALLY_NOT_USED(eLogInfo) ;
	
	if(g_pLogHandle)
	{
		pcFmtdMsg = (JC_INT8 *) jdd_MemAlloc (LOG_MAX_VAR_ARG, \
											sizeof(JC_INT8)) ;
		
		if (JC_NULL != pcFmtdMsg)
		{
			for (pcFmtPtr = pcFormat; pcFmtPtr && *pcFmtPtr; pcFmtPtr++)
			{
				if ('%' != *pcFmtPtr)
				{
					if ((jc_strlen (pcFmtdMsg) + 1) >= uiMsgLen )
					{
						uiOrgLen = uiMsgLen ;
						uiMsgLen += LOG_MAX_VAR_ARG ;
						jc_memset ((pcFmtdMsg + LOG_MAX_VAR_ARG), 0, \
												LOG_MAX_VAR_ARG) ;
					}
					
					*(pcFmtdMsg + jc_strlen (pcFmtdMsg)) = *pcFmtPtr ;
				}
				else if ('%' == *pcFmtPtr)
				{
					pcFmtPtr++ ;
					while (*pcFmtPtr >= '0' && *pcFmtPtr <= '9')
					{
						pcFmtPtr++ ;
					}
					
					pcVarArg = logConvertVarArgToString ( &vList, \
						*pcFmtPtr, *(pcFmtPtr + 1) ) ;
					if (JC_NULL != pcVarArg)
					{
						if ( uiMsgLen <= (jc_strlen (pcVarArg) + \
											jc_strlen (pcFmtdMsg)))
						{
							uiOrgLen = uiMsgLen ;
							uiMsgLen += LOG_MAX_VAR_ARG ;
							jc_memset ((pcFmtdMsg + LOG_MAX_VAR_ARG), 0, \
													LOG_MAX_VAR_ARG) ;
						}
						jc_strcat (pcFmtdMsg, pcVarArg) ;
						jdd_MemFree (pcVarArg) ;
					}
				}
			}
			va_end (vList) ;

			useModule(iModuleNum,aModuleName) ;
			/* Application should decide the parameters requirement */

			uiCount = jc_sprintf(aWriteBuffer,
					"\n MODULENAME[%s]  FILENAME[%s]  LINENUMBER[%d] ",
					aModuleName, pcFileName, iLineNumber) ;
			jc_memcpy((aWriteBuffer + uiCount),pcFmtdMsg,  \
						jc_strlen(pcFmtdMsg)) ;
			jdd_MemFree (pcFmtdMsg) ;
			writeLogsToFile(aWriteBuffer, (jc_strlen(aWriteBuffer))) ;
			jdd_FSFlush(g_pLogHandle) ;
		}
	}
	else
	{
		JDD_FSHANDLE vFileSysHandle ;
		if(!g_pLogHandle)
		{			 
			if(JC_OK == jdd_FSInitialize (&vFileSysHandle))
				g_pLogHandle = (JDD_FILE)jdd_FSOpen(JC_NULL, \
								(JC_CHAR*)JDD_LOG_FILE_NAME, \
								E_CREATE_APPEND_WRITE_MODE) ;
		}
	}
#ifdef FILE_LOG
}
#endif //FILE_LOG

#else /*End of _VA_LIST_DEFINED*/
	{
		/*--- Platform Specific Variables ---*/
		/*-----------------------------------*/

		/*--- Platform Specific Implementation Start ---*/
		/*--- Platform Specific Implementation End ---*/

	}
#endif
#endif //0
}



/**
 * @ingroup		jdd_logapi
 * @brief 		This function is used for logging message in JC_CHAR format
 * @param[in]	eLogInfo The log level 
 *				(e.g. LOG_LEVEL_INFORMATION/LOG_LEVEL_WARNING/LOG_LEVEL_FATAL)
 * @param[in]	iModuleNum The ID of the module where the log is placed
 * @param[in]	pcFileName The full path of the source file where the log is
 *              placed, or module name or JC_NULL if LOG_FILE_NAMES is 
 *				turned off.
 * @param[in]	iLineNumber The line number in the source file where the 
 *				log is placed
 * @param[in]	pcMsg The Message associated to the JC_CHAR string.
 * @param[in]	pmValue The Value which nees to be logged
 */

void jdd_LogMsgTcs (JC_INT32 eLogInfo, \
					JC_INT32 iModuleNum, \
					const JC_INT8 *pcFileName, \
					JC_INT32 iLineNumber, \
					const JC_INT8 *pcMsg, \
					const JC_CHAR *pmValue)
{
#if 0
	JC_UINT32   uiCount = 0 ;
	JC_INT8	    aWriteBuffer[LOG_MAX_VAR_ARG] = {0,} ;
	JC_INT8     aModuleName[25] ; /* Assuming JCF module name not to exceed 
								  * 25 bytes */

	jdd_AssertOnFail ((JC_NULL != pcFileName), \
						"jdd_LogMsg: pcFileName is NULL") ;
	jdd_AssertOnFail ((JC_NULL != pcMsg), \
						"jdd_LogMsg: pcMsg is NULL") ;
	jdd_AssertOnFail ((JC_NULL != pmValue), \
						"jdd_LogMsg: pmValue is NULL") ;

	PARAM_INTENTIONALLY_NOT_USED(eLogInfo) ;

	if(JC_NULL == g_pLogHandle)
	{
		JDD_FSHANDLE vFileSysHandle ;
		if(JC_OK == jdd_FSInitialize (&vFileSysHandle))
			g_pLogHandle = (JDD_FILE)jdd_FSOpen(JC_NULL, \
			(JC_CHAR*)JDD_LOG_FILE_NAME, \
			E_CREATE_APPEND_WRITE_MODE) ;
	}

	useModule(iModuleNum,aModuleName) ;

	uiCount = jc_sprintf(aWriteBuffer,(const JC_INT8*)"%s - %s - %d", \
				aModuleName, pcFileName, iLineNumber) ;	
	jc_memcpy((aWriteBuffer + uiCount), pcMsg, jc_strlen(pcMsg) + 1) ;
	
	jdd_FSWrite(aWriteBuffer, jc_strlen(aWriteBuffer), 1, \
							g_pLogHandle) ;
	
	//jdd_FSWrite(pmValue, jc_wstrlen(pmValue), 1, g_pLogHandle) ;	
	jdd_FSFlush(g_pLogHandle) ;
#endif//0
	JC_INT8 *pcValue = JC_NULL ;
	JC_RETCODE rCode ;
	JC_INT8	*pcFormattedMsg = (JC_INT8 *)jdd_MemAlloc (	jc_strlen (pcMsg) + 12, sizeof (JC_INT8)) ;
	rCode = jdi_CUtilsTcharToChar (JC_NULL, pmValue, &pcValue) ;
	if(rCode == JC_OK)
	{
		jc_strcpy (pcFormattedMsg, "JAT_LOG: ") ;
		jc_strcat (pcFormattedMsg, pcMsg) ;
		jc_strcat (pcFormattedMsg, "%s") ;
		mmi_trace(12, pcFormattedMsg, pcValue) ;
		jdd_MemFree (pcFormattedMsg) ;
		jdd_MemFree(pcValue) ;
	}
}


/**
 * @ingroup		jdd_logapi
 * @brief 		This function is used for dumping the data in the hex format
 * @param[in]	eLogInfo The log level 
 *				(e.g. LOG_LEVEL_INFORMATION/LOG_LEVEL_WARNING/LOG_LEVEL_FATAL)
 * @param[in]	iModuleNum The ID of the module where the log is placed
 * @param[in]	pcFileName The full path of the source file where the log is
 *              placed, or module name or JC_NULL if LOG_FILE_NAMES is 
 *				turned off.
 * @param[in]	iLineNumber The line number in the source file where the 
 *				log is placed
 * @param[in]	pcMsgInfo The Message associated to the data.
 * @param[in]	pucDumpInfo The content which nees to be dumped as the hex 
 *				format in the log
 * @param[in]	iLength The Size of the content
 */

void jdd_LogDumpHex (JC_INT32 eLogInfo, \
					JC_INT32 iModuleNum, \
					const JC_INT8 *pcFileName, \
					JC_INT32 iLineNumber, \
					const JC_INT8 *pcMsgInfo, \
					JC_UINT8 *pucDumpInfo, \
					JC_INT32 iLength)
{
#if 0
	JC_UINT32   uiCount = 0 ;
	JC_INT8		aWriteBuffer[LOG_MAX_VAR_ARG] = {0,} ;
	JC_INT8	*pcHexBuffer = JC_NULL ;
	JC_INT8     aModuleName[25] ;


	jdd_AssertOnFail ((JC_NULL != pcFileName), \
						"jdd_LogMsg: pcFileName is NULL") ;
	jdd_AssertOnFail ((JC_NULL != pcMsgInfo), \
						"jdd_LogMsg: pcMsgInfo is NULL") ;
	jdd_AssertOnFail ((JC_NULL != pucDumpInfo), \
						"jdd_LogMsg: pucDumpInfo is NULL") ;
	jdd_AssertOnFail ((0 != iLength), \
						"jdd_LogMsg: iLength is 0") ;


	PARAM_INTENTIONALLY_NOT_USED(eLogInfo) ;

	useModule(iModuleNum,aModuleName) ;

	uiCount = jc_sprintf(aWriteBuffer,(const JC_INT8*)"%s - %s - %d", \
						aModuleName, pcFileName, iLineNumber) ;
	pcHexBuffer = (JC_INT8 *)jdd_MemAlloc(iLength + 1,1) ;
	if(pcHexBuffer)
	{
		//jc_sprintf(pcHexBuffer,(const JC_INT8*)"%x",pucDumpInfo) ;
	}

	jc_memcpy((aWriteBuffer + uiCount), pcMsgInfo, \
					jc_strlen(pcMsgInfo) + 1) ;

	jc_memcpy((aWriteBuffer + uiCount + jc_strlen(pcMsgInfo)), \
							pcHexBuffer, (iLength + 1)) ;
	
	if(pcHexBuffer)
	{
		jdd_MemFree(pcHexBuffer) ;
	}
	
	jdd_FSWrite(aWriteBuffer, jc_strlen(aWriteBuffer), 1, \
								g_pLogHandle) ;
	jdd_FSFlush(g_pLogHandle) ;
#endif
	mmi_trace(1, (PCSTR)pcMsgInfo) ;
	mmi_trace_hex(1, iLength, pucDumpInfo) ;

}

/**
 * @ingroup		jdd_logapi
 * @brief 		This function is used for logging the data in the same content
 * @param[in]	eLogInfo The log level 
 *				(e.g. LOG_LEVEL_INFORMATION/LOG_LEVEL_WARNING/LOG_LEVEL_FATAL)
 * @param[in]	iModuleNum The ID of the module where the log is placed
 * @param[in]	pcFileName The full path of the source file where the log is
 *              placed, or module name or JC_NULL if LOG_FILE_NAMES is 
 *				turned off.
 * @param[in]	iLineNumber The line number in the source file where the 
 *				log is placed
 * @param[in]	pcMsgInfo The Message associated to the data.
 * @param[in]	pucDumpInfo The content which nees to be logged
 * @param[in]	iLength The Size of the content
 */

void jdd_LogDumpMem (JC_INT32 eLogInfo, \
					JC_INT32 iModuleNum, \
					const JC_INT8 *pcFileName, \
					JC_INT32 iLineNumber, \
					const JC_INT8 *pcMsgInfo, \
					JC_UINT8 *pucDumpInfo, \
					JC_INT32 iLength)
{
	jdd_AssertOnFail ((JC_NULL != pcFileName), \
						"jdd_LogMsg: pcFileName is NULL") ;
	jdd_AssertOnFail ((JC_NULL != pcMsgInfo), \
						"jdd_LogMsg: pcMsgInfo is NULL") ;
	jdd_AssertOnFail ((JC_NULL != pucDumpInfo), \
						"jdd_LogMsg: pucDumpInfo is NULL") ;
	jdd_AssertOnFail ((0 != iLength), \
						"jdd_LogMsg: iLength is 0") ;

	jdd_LogMsg(eLogInfo, iModuleNum, pcFileName, \
				iLineNumber, pcMsgInfo, pucDumpInfo, iLength) ;
	/*TBD:This Function is not used in core */
}

/*End of File*/
#endif

