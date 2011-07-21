/***************************************************************************
 * $Id: jdi_se.h,v 1.17 2009/05/04 06:09:07 browserdevjtbase Exp $
 * $Revision: 1.17 $
 * $Date: 2009/05/04 06:09:07 $
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
 * $Log: jdi_se.h,v $
 * Revision 1.17  2009/05/04 06:09:07  browserdevjtbase
 * Core Updates
 *
 * Revision 1.66  2009/04/20 07:49:12  shajudevhtmlbrow
 * OBO changes.
 *
 * Revision 1.65  2008/12/17 04:48:31  shajudevhtmlbrow
 * ScriptEngine_V_1_1_4_11_16_12_2008
 *
 * Revision 1.64  2008/12/09 10:49:24  shajudevhtmlbrow
 * ScriptEngine_V_1_1_4_10_09_12_2008
 *
 * Revision 1.63  2008/11/28 09:42:00  shajudevhtmlbrow
 * GFO optimization changes.
 *
 * Revision 1.62  2008/11/11 11:09:24  shajudevhtmlbrow
 * BOM & DOM peak memory optimization changes
 *
 * Revision 1.61  2008/10/22 07:12:08  shajudevhtmlbrow
 * changes for new assignment overload attribute.
 *
 * Revision 1.60  2008/06/19 11:01:23  shajudevhtmlbrow
 * changes in custome object implementation.
 *
 * Revision 1.59  2008/06/19 06:55:42  shajudevhtmlbrow
 * changes for modication in sphinx interface.
 *
 * Revision 1.58  2008/05/27 06:24:58  shajudevhtmlbrow
 * WRT custom object implementation
 *
 * Revision 1.57  2008/05/14 07:35:11  shajudevhtmlbrow
 * Comment added
 *
 * Revision 1.56  2008/04/28 15:21:14  kavithadevhtmlbrow
 * jdi_SEPermRegisterCustomMethod
 *
 * Revision 1.55  2008/03/25 06:35:14  kavithadevhtmlbrow
 * jdi_SEGetSessionWindowObject
 *
 * Revision 1.54  2008/03/07 13:21:16  shajudevhtmlbrow
 * Host object alias and navigator language support
 *
 * Revision 1.53  2008/03/07 06:54:05  shajudevhtmlbrow
 * Errorcode is handled for Reset Session.
 *
 * Revision 1.52  2008/02/25 05:40:38  shajudevhtmlbrow
 * Changes for SE new interface
 *
 * Revision 1.51  2007/12/07 08:00:29  kavithadevhtmlbrow
 * Interface for Parent/Child Session Init/Deinit & Reset added
 *
 * Revision 1.50  2007/09/19 05:46:51  kavithadevhtmlbrow
 * ScriptEngine_V_0_8_0_45_18_09_2007
 *
 * Revision 1.49  2007/09/17 11:58:18  kavithadevhtmlbrow
 * SE - Interfaces Added
 *
 * Revision 1.48  2007/09/06 13:19:38  kavithadevhtmlbrow
 * Ajax Integration
 *
 * Revision 1.47  2007/09/05 05:12:10  kavithadevhtmlbrow
 * DOM Attribute, Content type in EvaluateScript, Script Cont Status in DE
 *
 * Revision 1.46  2007/09/03 13:51:18  kavithadevhtmlbrow
 * Bom Location Put Override
 *
 * Revision 1.45  2007/08/27 08:38:50  kavithadevhtmlbrow
 * SetTimeOut Handled
 *
 * Revision 1.44  2007/08/17 07:01:14  babithapkdevhtmlbrow
 * Added comments
 *
 * Revision 1.43  2007/08/13 06:13:23  babithapkdevhtmlbrow
 * ScriptEngine_V_0_8_0_27_10_08_2007 related changes
 *
 * Revision 1.42  2007/08/08 12:49:24  babithapkdevhtmlbrow
 * Added jdi_SEDestroyResultContent
 *
 * Revision 1.41  2007/08/08 09:22:53  babithapkdevhtmlbrow
 * Added jdi_SEExecuteFunctionObject
 *
 * Revision 1.40  2007/07/18 05:51:47  babithapkdevhtmlbrow
 * Removed SE_HOST_OBJECT.
 * Sphinx has provision to store Host environment specific data for an Object.
 *
 * Revision 1.39  2007/07/03 12:38:14  babithapkdevhtmlbrow
 * Modified for watchdog timer
 *
 * Revision 1.38  2007/07/03 08:10:47  babithapkdevhtmlbrow
 * Removed ddl.h & jcal.h from interface files
 *
 * Revision 1.37  2007/06/01 05:02:36  babithapkdevhtmlbrow
 * Registered Host Object comparison callback with script engine
 *
 * Revision 1.36  2007/05/15 09:37:37  babithapkdevhtmlbrow
 * ScriptEngine_V_0_8_0_06_15/05/2007 changes
 *
 * Revision 1.35  2007/05/03 12:58:38  babithapkdevhtmlbrow
 * Added jdi_SESetGlobalObjectPutBehaviour
 *
 * Revision 1.34  2007/05/03 05:04:44  babithapkdevhtmlbrow
 * Added charset of script content in jdi_SEEvaluateScript & jdi_SEEvaluateMoreScript
 *
 * Revision 1.33  2007/05/02 05:47:37  babithapkdevhtmlbrow
 * Removed #defines for property attributes
 *
 * Revision 1.32  2007/04/27 13:01:36  babithapkdevhtmlbrow
 * Modified jdi_SEGetFunctionName
 *
 * Revision 1.31  2007/04/20 09:34:31  babithapkdevhtmlbrow
 * Added jdi_SESetGlobalObjectGetBehaviour
 *
 * Revision 1.30  2007/04/19 08:51:49  babithapkdevhtmlbrow
 * Added jdi_SEFreeString
 *
 * Revision 1.29  2007/04/18 11:51:45  babithapkdevhtmlbrow
 * Added jdi_SEFreeObject
 *
 * Revision 1.28  2007/04/17 08:59:42  babithapkdevhtmlbrow
 * Supported multiple script engine sessions
 *
 * Revision 1.27  2007/04/11 11:56:31  babithapkdevhtmlbrow
 * Modified jdi_SEValueToTChar
 *
 * Revision 1.26  2007/03/30 11:23:30  babithapkdevhtmlbrow
 * Added jdi_SECreateHostObjectSansData to handle dummy style object
 *
 * Revision 1.25  2007/03/29 10:27:18  babithapkdevhtmlbrow
 * Add interfaces to register string, number, boolean properties to global object
 *
 * Revision 1.24  2007/03/29 06:21:17  babithapkdevhtmlbrow
 * Added SE_OBJECT_SET_CALL
 *
 * Revision 1.23  2007/03/27 07:16:42  babithapkdevhtmlbrow
 * Added jdi_SEConstructArrayObject
 *
 * Revision 1.22  2007/03/23 13:11:36  babithapkdevhtmlbrow
 * Added jdi_SEAddObjectReference,jdi_SEIsValueNULL,jdi_SEIsValueUndefined
 *
 * Revision 1.21  2007/03/21 07:26:14  babithapkdevhtmlbrow
 * Modified type conversion interfaces & jdi_SEFreeValue
 *
 * Revision 1.20  2007/03/08 11:49:42  babithapkdevhtmlbrow
 * Incorporate code review comments
 *
 * Revision 1.19  2007/03/02 11:47:21  shreyasdevhtmlbrow
 * RVCT Compilation fix
 *
 * Revision 1.18  2007/02/27 10:51:01  babithapkdevhtmlbrow
 * Added jdi_SEThrowError
 *
 * Revision 1.17  2007/02/24 10:06:12  babithapkdevhtmlbrow
 * Added jdi_SECopyValue
 *
 * Revision 1.16  2007/02/24 06:09:21  babithapkdevhtmlbrow
 * Removed jdi_SEFreeObject
 *
 * Revision 1.15  2007/02/23 11:28:21  babithapkdevhtmlbrow
 * Added jdi_SEFreeResult
 *
 * Revision 1.14  2007/02/23 06:13:09  babithapkdevhtmlbrow
 * Final result provided as output to interfaces
 *
 * Revision 1.13  2007/02/22 03:49:08  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.12  2007/02/20 09:39:49  babithapkdevhtmlbrow
 * Modified jdi_SESetObjectType & jdi_SEGetObjectType
 *
 * Revision 1.11  2007/02/20 08:49:41  babithapkdevhtmlbrow
 * Modified jdi_SERegisterSingletonHostObject to add attributes
 *
 * Revision 1.10  2007/02/09 09:40:39  babithapkdevhtmlbrow
 * Rename jdi_SECreateErrorObj as jdi_SEConstructObject
 *
 * Revision 1.9  2007/02/07 11:19:46  babithapkdevhtmlbrow
 * Added Object destructor callback & jdi_SESetThisValueObj.
 *
 * Revision 1.8  2007/02/06 06:49:09  babithapkdevhtmlbrow
 * Changes for Sphinx M6 Build3 Patch
 *
 * Revision 1.7  2007/02/02 10:31:54  babithapkdevhtmlbrow
 * Removed jdi_SECreateHostObjectArray
 *
 * Revision 1.6  2007/01/25 11:25:58  babithapkdevhtmlbrow
 * Added jdi_SESetPrototypeParent
 *
 * Revision 1.5  2007/01/24 07:53:46  babithapkdevhtmlbrow
 * Added SE_GET_STRING_VALUE
 *
 * Revision 1.4  2007/01/23 04:20:19  babithapkdevhtmlbrow
 * Added jdi_SEInitSession, jdi_SEDeInitSession. Changed string interning strategy.
 *
 * Revision 1.3  2007/01/22 07:04:37  babithapkdevhtmlbrow
 * Added ESMP Event handling
 *
 * Revision 1.2  2007/01/19 12:04:15  babithapkdevhtmlbrow
 * jdi_SEValueToNumber interface changed
 *
 * Revision 1.1  2007/01/10 11:23:55  kumardevhtmlbrow
 * *** empty log message ***
 *
 *  
 ***************************************************************************/

#ifndef _JDI_SE_H_
#define _JDI_SE_H_

#include <jdi_sec.h>
#include <jdi_scobehaviour.h>
#include <jdi_sco.h>
#include <jdi_sym.h>
#include <jdi_stb.h>

typedef JC_HANDLE   SE_CONTEXT ;
typedef JC_HANDLE	SE_SESSION ;
typedef JC_HANDLE   SE_OBJECT ;
typedef JC_HANDLE   SE_STRING ;

#ifdef BROWSER_OBO_ENABLE
typedef ST_SCO_CALLBACK_INFO	ST_SE_CALLBACK_INFO ;
#endif /* BROWSER_OBO_ENABLE */
typedef CB_SCO_GET_METHOD			CB_SE_GET_METHOD ;
typedef CB_SCO_PUT_METHOD			CB_SE_PUT_METHOD ;
typedef CB_SCO_DELETE_METHOD		CB_SE_DELETE_METHOD ;
typedef CB_SCO_CALL_METHOD			CB_SE_METHOD_CALL ;
typedef CB_SCO_DEFAULTVAL_METHOD	CB_SE_DEFAULTVAL_METHOD ;
typedef CB_SCO_GET_ALL_PROPERTIES	CB_SE_GET_ALL_PROPERTIES ;

/**
 * @brief Abstracts any ECMAScript data type
 */
typedef JC_HANDLE SE_VALUE ;

/**
 * @brief Abstracts internal methods for an ECMAScript object
 */
typedef JC_HANDLE SE_OBJECT_BEHAVIOUR; 

/**
 * @brief Defines Initialize parameters
 */
typedef struct st_se_init_params
{
	JC_CHAR									*pmFilePath ;
	CB_SEC_OBJECT_CONSUMER_PERM_DESTROY		cbObjectPermDestructor ;
	CB_SEC_HOST_OBJECT_COMPARE				cbObjectCompare ;
	JC_UINT32								uiOWRThresholdVal ;
#ifdef BROWSER_OBO_ENABLE					
	ST_SE_CALLBACK_INFO						*pstCallbackInfo ;
#endif /* BROWSER_OBO_ENABLE */
} ST_SE_INIT_PARAMS ;

/**
 * @brief Defines Frame information
 */

typedef struct st_se_frame_info
{
	JC_HANDLE		vParentSession ;
	JC_CHAR			*pmFrameName ;
	JC_UINT32		uiFrameIndex ;
} ST_SE_FRAME_INFO ;

/**
 * @brief Specifies script buffer.
 */
typedef U_SCRIPT_SRC	U_SE_SCRIPT_SRC ;

/**
 * @brief Specifies script source.
 */
typedef E_SOURCE_INFO	E_SE_SCRIPT_INFO ;

/**
 * @brief Specifies evalute script parameters.
 */
typedef ST_SE_SCRIPT	ST_SE_EVALUATE_SCRIPT ;

#define SE_OBJECT_GET_CLASS(vObj) (SCO_OBJECT_GET_NAME(vObj))

#define SE_OBJECT_SET_GET(vObj, cbGet) SCO_OBJECT_SET_GET((ST_SCO_OBJECT *) vObj, cbGet)
#define SE_OBJECT_SET_PUT(vObj, cbPut) SCO_OBJECT_SET_PUT((ST_SCO_OBJECT *) vObj, cbPut)
#define SE_OBJECT_SET_HASPROPERTY(vObj, cbHasProperty) SCO_OBJECT_SET_HASPROPERTY((ST_SCO_OBJECT *) vObj, cbHasProperty)
#define SE_OBJECT_SET_CLASS(vObj, pmClass) SCO_OBJECT_SET_CLASS((ST_SCO_OBJECT *) vObj, pmClass)
#define SE_OBJECT_SET_CONSTRUCT(vObj, cbConstruct) SCO_OBJECT_SET_CONSTRUCT ((ST_SCO_OBJECT *) vObj, cbConstruct)
#define SE_OBJECT_SET_CALL(vObj, cbCall) SCO_OBJECT_SET_CALL ((ST_SCO_OBJECT *) vObj, cbCall)
#define SE_GET_STRING_VALUE(vStrRef) STB_GET_STRING_VALUE (vStrRef)
#define jdi_SESetObjectType(vObj, eObjTypeArg) jdi_SECHostSetHostType((ST_SCO_OBJECT *) vObj, (JC_UINT8) eObjTypeArg)
#define jdi_SEGetObjectType(vObj) jdi_SECHostGetHostType((ST_SCO_OBJECT *) vObj) 
#define SE_OBJECT_SET_DELETE(vObj, cbDeleteArg) SCO_OBJECT_SET_DELETE ((ST_SCO_OBJECT *) vObj, cbDeleteArg)
#define SE_OBJECT_SET_DEFAULT_VALUE(vObj, cbDefaultValueArg) SCO_OBJECT_SET_DEFAULTVALUE ((ST_SCO_OBJECT *) vObj, cbDefaultValueArg)

/** 
 * @brief Add Host Object's reference
 * @param[in]  vObject		            Given Host Object
 * @retval 	Nil
 *
 * If a host object needs to be shared, the object reference has to be incremented.
 * For eg: location object is added to Global object.If location object needs to be added as a 
 * property to another object, update the reference of location object before adding as property.
 */

#define jdi_SEAddObjectReference(vObject) jdi_SECHostObjectUpdateShare ((ST_SCO_OBJECT *) vObject)

/** 
 * @brief Get prototype object of a host object
 * @param[in]  vObject		            Host object
 * @retval 	non JC-NULL                 Indicates prototype object present
 * @retval 	JC-NULL                     Indicates no prototype object present
 */

#define jdi_SEGetPrototype(vObject) jdi_SECHostGetPrototypeParent ((ST_SCO_OBJECT *) vObject)

/** 
 * @brief Set prototype of a host object
 * @param[in]  vObject		            Object whose prototype to be set
 * @param[in]  vPrototype				Prototype object
 * @retval 	Nil
 */

#define jdi_SESetPrototype(vObject, vPrototype) \
		jdi_SECHostSetPrototypeParent ((ST_SCO_OBJECT *) vObject, (ST_SCO_OBJECT *) vPrototype)

/** 
 * @brief Get data of a host object
 * @param[in]  vObject		            Host object
 * @retval 	Host object data (JC_HANDLE)
 */

#define jdi_SEGetHostObjectData(vObject) jdi_SECHostGetObjectData ((ST_SCO_OBJECT *) vObject)

/** 
 * @brief Set data of a host object
 * @param[in]  vObject		            Host object
 * @param[in]  vData		            Host object data
 * @retval 	Nil
 */

#define jdi_SESetHostObjectData(vObject, vData) jdi_SECHostSetObjectData ((ST_SCO_OBJECT *) vObject, vData)

/**
 * @brief Overload Object Assignment Property for Host Objects
 * @param[in]  vHostObj		           Given ST_SCO_OBJECT
 */

#define jdi_SEHostSetObjectAssignmentOverload(vHostObj) \
        jdi_SECHostSetAssignmentOverload(vHostObj)

/**
 * @brief Convert a JC_BOOLEAN to an SE_VALUE
 * @param[in]  bBool			       JC_BOOLEAN value
 * @param[out] vValue			       New SE_VALUE
 * @retval Nil
 *
 * Application shall allocate memory for vValue.
 */

#define jdi_SEBooleanToValue(bBool, vValue) SE_SET_BOOLEAN ((ST_SE_VALUE *) vValue, bBool)

/**
 * @brief Convert an SE_OBJECT to an SE_VALUE
 * @param[in]  vObject			       SE_OBJECT value
 * @param[out] vValue			       New SE_VALUE
 * @retval Nil
 *
 * Application shall allocate memory for vValue.
 */

#define jdi_SEObjectToValue(vObject,vValue) SE_SET_OBJECT ((ST_SE_VALUE *) vValue, (ST_SCO_OBJECT *) vObject)

/**
 * @brief Sets SE_VALUE as undefined
 * @param[in]  vValue		           Given SE VALUE
 * @retval Nil
 */

#define jdi_SEValueAsUndefined(vValue) SE_SET_UNDEFINED ((ST_SE_VALUE *) vValue)

/**
 * @brief Sets SE_VALUE as NULL
 * @param[in]  vValue		           Given SE VALUE
 * @retval Nil
 */

#define jdi_SEValueAsNULL(vValue) SE_SET_NULL ((ST_SE_VALUE *) vValue)

/**
 * @brief Checks whether given SE_VALUE is null
 * @param[in]  vValue		           Given SE VALUE
 * @retval JC_BOOLEAN
 */

#define jdi_SEIsValueNULL(vValue) SE_IS_NULL ((ST_SE_VALUE *) vValue)

/**
 * @brief Checks whether given SE_VALUE is undefined
 * @param[in]  vValue		           Given SE VALUE
 * @retval JC_BOOLEAN
 */

#define jdi_SEIsValueUndefined(vValue) SE_IS_UNDEFINED ((ST_SE_VALUE *) vValue)

/* Note : Enum values shall match with values given in jdi_sec.h */
typedef enum e_se_native_error
{
    E_SE_EVAL_ERROR = 2, 
    E_SE_SYNTAX_ERROR, 
    E_SE_RANGE_ERROR, 
    E_SE_REF_ERROR, 
    E_SE_TYPE_ERROR, 
    E_SE_URI_ERROR
} E_SE_NATIVE_ERROR ;

/**
 * @brief Initializes the Script Engine
 * @param[in]  vMemHandle		        Memory handle (can be JC_NULL)
 * @param[in]  cbObjectPermDestructor	Permanent Host Object's destructor
 * @param[in]  cbObjectCompare			Host Object compare callback
 * @param[in]  uiOWRThresholdVal        Script Engine's OWR threshold value 
 * @param[out] pvContext		        Initialized script engine context
 * @retval 	JC_OK                       Indicates success
 * @retval 	JC_ERR_MEMORY_ALLOCATION    Indicates memory allocation error
 * @retval Script Engine related error
 *
 * Application shall call this API initially before using any other script
 * engine APIs.Application shall pass pvContext obtained as output parameter 
 * to other APIs.
 */

JC_RETCODE jdi_SEInitialize (JC_HANDLE				vMemHandle, 
							 ST_SE_INIT_PARAMS		*pstInitParams, 
#ifdef BROWSER_OBO_ENABLE
							 ST_SE_CALLBACK_INFO	*pstCallbackInfo, 
#endif
							 SE_CONTEXT				*pvContext
							) ;

/**
 * @brief Perform cleanup of the Script Engine
 * @param[in] vContext		            Script engine context
 * @retval 	Nil
 *
 * Application shall call this API in the end when it finishes using script 
 * engine APIs.
 */

void jdi_SECleanup (SE_CONTEXT vContext) ;

/**
 * @brief Delete  a function object in Script Engine context
 * @param[in] vContext		            Script engine context
 * @param[in] vFunctionObj		        Function Object
 * @retval 	Nil
 *
 */

void jdi_SEDeleteStaticMethod (SE_CONTEXT        vContext,
                               SE_OBJECT		 vFunctionObj) ;

/**
 * @brief Initializes a Script Engine session for a parent frame
 * @param[in] vContext		            Script engine context
 * @param[in] cbObjectTempDestructor	Temporary Host Object's destructor
 * @param[in] vHostAppData		        Host application data (if any)
 * @param[out] pvSession		        Initialized script engine session
 * @retval 	JC_OK                       Indicates success
 * @retval 	JC_ERR_MEMORY_ALLOCATION    Indicates memory allocation error
 * @retval Script Engine related error
 *
 * Application shall call this API after jdi_SEInitialize to initialize 
 * a script engine session for a parent frame. The interface should be called only once.
 */

JC_RETCODE  jdi_SEInitParentSession (
					   SE_CONTEXT							vContext, 
					   CB_SEC_OBJECT_CONSUMER_TEMP_DESTROY  cbObjectTempDestructor, 
					   JC_HANDLE							vHostAppData, 
					   SE_SESSION							*pvSession
                     ) ;

/**
 * @brief DeInitializes a Script Engine session for a parent frame
 * @param[in] vSession		            Script engine session
 * @retval 	Nil
 *
 * This interface would be called only once for the parent frame.
 */

void jdi_SEDeInitParentSession (SE_SESSION     vSession) ;

/**
 * @brief Initializes a Script Engine session for a Child frame
 * @param[in] vContext		            Script engine context
 * @param[in] cbObjectTempDestructor	Temporary Host Object's destructor
 * @param[in] pstFrameInfo				Frame Related information
 * @param[in] vHostAppData		        Host application data (if any)
 * @param[in] bIsAliasChild		        Set to E_TRUE if the aliases should be set like
 *										a parent session.
 * @param[out] pvSession		        Initialized script engine session
 * @retval 	JC_OK                       Indicates success
 * @retval 	JC_ERR_MEMORY_ALLOCATION    Indicates memory allocation error
 * @retval Script Engine related error
 *
 * Application shall call this API after jdi_SEInitialize to initialize 
 * a script engine session for a child frame. The interface should be called 
 * repeatedly for all the child frames.
 */

JC_RETCODE  jdi_SEInitChildSession (
					   SE_CONTEXT							vContext, 
					   CB_SEC_OBJECT_CONSUMER_TEMP_DESTROY  cbObjectTempDestructor, 
					   ST_SE_FRAME_INFO						*pstFrameInfo, 
					   JC_HANDLE							vHostAppData, 
					   JC_BOOLEAN							bIsAliasChild, 
					   SE_SESSION							*pvSession
                     ) ;

/**
 * @brief DeInitializes a Script Engine session for a child frame
 * @param[in] vSession		            Script engine session
 * @param[in] bIsAliasChild		        Set to E_TRUE if the aliases should be set like
 *										a parent session.
 * @retval 	Nil
 *
 * This interface would be called to deinitialize the child frame.
 */

void jdi_SEDeInitChildSession (SE_SESSION		vSession, 
							   JC_BOOLEAN		bIsAliasChild) ;

/**
 * @brief	    Resets a Script Engine session for a parent frame
 * @param[in]	vSession                Script engine session
 * @retval 	JC_OK on success
 *			Script Engine related errors 
 */

JC_RETCODE	jdi_SEResetParentSession (SE_SESSION vSession) ;

/**
 * @brief	    Resets a Script Engine session for a child frame
 * @param[in]	vSession                Script engine session
 * @param[in]	bIsAliasChild			Need to pass the same flag which 
 *										has passed at the time of init session.
 * @retval 	JC_OK on success
 *			Script Engine related errors 
 */

JC_RETCODE	jdi_SEResetChildSession (SE_SESSION		vSession, 
									 JC_BOOLEAN		bIsAliasChild) ;
/**
 * @brief Parses and evaluates given script
 * @param[in]  vSession		            Script engine session
 * @param[in]  pcScript                 Script content
 * @param[in]  uiScriptLen              Script content length
 * @param[in]  pucCharset               Charset of script content.Any BOM 
 
 *									    in script is handled by Script Engine.
 *									    Defaults to UTF-8 if JC_NULL is passed.
 * @param[in]  pcContentType            Reference to the content type
 * @param[in]  bHasMoreScript		    E_TRUE if there is more script to evaluate
 * @param[in]  bEventScript				E_TRUE indicates script belongs to event handler.
 *										E_FALSE indicates script belongs to inline or linked 
 *										scripts.
 * @param[out] pvResult					Final Result
 * @retval 	JC_OK                       Indicates success
 * @retval 	JC_ERR_MEMORY_ALLOCATION    Indicates memory allocation error
 * @retval  JC_ERR_SE_SUSPENSION		Indicates suspension of script execution since 
 *									    user interaction is required,
 * @retval  JC_ERR_SE_OWR_SUSPENSION	Indicates suspension of script execution 
 *										since maximum operations threshold is exceeded
 * @retval  JC_ERR_SEC_COMPILATION		Indicates script source has syntax errors,
 * @retval  JC_ERR_SEC_EVALUATION		Indicates script source has run-time errors.
 *
 * Caller shall free pvResult using jdi_SEFreeValue()
 */

JC_RETCODE jdi_SEEvaluateScript (SE_SESSION				vSession, 
								 ST_SE_EVALUATE_SCRIPT	*pstEvalScript, 
								 SE_VALUE				*pvResult
                                ) ;

/**
 * @brief Parses and evaluates more script
 * @param[in]  vSession		            Script engine session
 * @param[in]  pcScript                 Script content
 * @param[in]  uiScriptLen              Script content length
 * @param[in]  pucCharset               Charset of script content.Any BOM 
 
 *									    in script is handled by Script Engine.
 *									    Defaults to UTF-8 if JC_NULL is passed.
 * @param[in]  pcContentType            Reference to the content type
 * @param[in]  bHasMoreScript		    E_TRUE if there is more script to evaluate
 * @param[out] pvResult					Final Result
 * @retval 	JC_OK                       Indicates success
 * @retval 	JC_ERR_MEMORY_ALLOCATION    Indicates memory allocation error
 * @retval  JC_ERR_SE_SUSPENSION		Indicates suspension of script execution since 
 *									    user interaction is required,
 * @retval  JC_ERR_SE_OWR_SUSPENSION	Indicates suspension of script execution 
 *										since maximum operations threshold is exceeded
 * @retval  JC_ERR_SEC_COMPILATION		Indicates script source has syntax errors,
 * @retval  JC_ERR_SEC_EVALUATION		Indicates script source has run-time errors.
 *
 * Caller shall free pvResult using jdi_SEFreeValue()
 */

JC_RETCODE jdi_SEEvaluateMoreScript (SE_SESSION				vSession, 
									 ST_SE_EVALUATE_SCRIPT	*pstEvalScript, 
                                     SE_VALUE				*pvResult
									) ;

/** 
 * @brief Registers a host object of the given name
 * @param[in]  vSession		            Script engine session
 * @param[in]  vObjNameRef		        Host object name reference
 * @param[in]  vParentProto             Parent prototype of host.prototype 
 *                                      object.If JC_NULL, 
 *                                      Object.prototype is assumed.
 * @param[out] pvProtoObj		        New Prototype object
 * @param[out] pvCtorObj                New constructor object
 * @retval 	JC_OK                       Indicates success
 * @retval 	JC_ERR_MEMORY_ALLOCATION    Indicates memory allocation error
 * @retval Script Engine related error
 *
 * The function creates a constructor object and a prototype object for a host 
 * object.The required prototype chain is established using vParentProto parameter. 
 * vParentProto indicates prototype for host.prototype object. If vParentProto 
 * is JC_NULL, Object.prototype is set as prototype for host.prototype 
 * object. The constructor object is added to Global object.
 * 
 * If underlying script engine provides garbage collection, caller shall not 
 * free memory allocated for pvProtoObj & pvCtorObj.Otherwise caller has to free
 * memory allocated for pvProtoObj & pvCtorObj.
 */

JC_RETCODE jdi_SERegisterHostObject (
         SE_SESSION				vSession, 
         SE_STRING              vObjNameRef, 
         SE_OBJECT              vParentProto, 
         SE_OBJECT              *pvProtoObj, 
         SE_OBJECT              *pvCtorObj
        ) ;

/** 
 * @brief Registers a permanent host object of the given name
 * @param[in]  vContext		            Script engine Context
 * @param[in]  vObjNameRef		        Host object name reference
 * @param[in]  vParentProto             Parent prototype of host.prototype 
 *                                      object.If JC_NULL, 
 *                                      Object.prototype is assumed.
 * @param[out] pvProtoObj		        New Prototype object
 * @param[out] pvCtorObj                New constructor object
 * @retval 	JC_OK                       Indicates success
 * @retval 	JC_ERR_MEMORY_ALLOCATION    Indicates memory allocation error
 * @retval Script Engine related error
 *
 * The function creates a constructor object and a prototype object for a host 
 * object.The required prototype chain is established using vParentProto parameter. 
 * vParentProto indicates prototype for host.prototype object. If vParentProto 
 * is JC_NULL, Object.prototype is set as prototype for host.prototype 
 * object. The constructor object is added to Global object.
 * 
 * If underlying script engine provides garbage collection, caller shall not 
 * free memory allocated for pvProtoObj & pvCtorObj.Otherwise caller has to free
 * memory allocated for pvProtoObj & pvCtorObj.
 */

JC_RETCODE jdi_SEPermRegisterHostObject (SE_CONTEXT				vContext, 
                                         SE_STRING              vObjNameRef, 
                                         SE_OBJECT              vParentProto, 
                                         SE_OBJECT              *pvProtoObj, 
                                         SE_OBJECT              *pvCtorObj
                                        ) ;

/** 
 * @brief Registers a singleton host object of given name
 * @param[in]  vSession		            Script engine session
 * @param[in]  vObjNameRef		        Host object name reference
 * @param[in]  uiAttributes		        Attributes for host object
 * @param[in]  vParentProto		        Parent prototype of host object.
 *                                      If JC_NULL, Object.prototype is assumed.
 * @param[out] pvHostObject             New host object
 * @retval 	JC_OK                       Indicates success
 * @retval 	JC_ERR_MEMORY_ALLOCATION    Indicates memory allocation error
 * @retval Script Engine related error
 *
 * If underlying script engine provides garbage collection, caller shall not 
 * free memory allocated for pvHostObject.Otherwise caller has to free
 * memory allocated for pvHostObject.
 *
 */

JC_RETCODE jdi_SERegisterSingletonHostObject (
         SE_SESSION				vSession, 
         SE_STRING              vObjNameRef, 
         SE_OBJECT              vParentProto, 
         JC_UINT32              uiAttributes, 
         SE_OBJECT              *pvHostObject 
        ) ;

/** 
 * @brief Registers a permanent singleton host object of given name
 * @param[in]  vContext		            Script engine context
 * @param[in]  vObjNameRef		        Host object name reference
 * @param[in]  uiAttributes		        Attributes for host object
 * @param[in]  vParentProto		        Parent prototype of host object.
 *                                      If JC_NULL, Object.prototype is assumed.
 * @param[out] pvHostObject             New host object
 * @retval 	JC_OK                       Indicates success
 * @retval 	JC_ERR_MEMORY_ALLOCATION    Indicates memory allocation error
 * @retval Script Engine related error
 *
 * If underlying script engine provides garbage collection, caller shall not 
 * free memory allocated for pvHostObject.Otherwise caller has to free
 * memory allocated for pvHostObject.
 *
 */

JC_RETCODE jdi_SEPermRegisterSingletonHostObject (
         SE_CONTEXT				vContext, 
         SE_STRING              vObjNameRef, 
         SE_OBJECT              vParentProto, 
         JC_UINT32              uiAttributes, 
         SE_OBJECT              *pvHostObject 
        ) ;

/** 
 * @brief Creates a host object and sets its data
 * @param[in]  vSession		            Script engine session
 * @param[in]  vParentProto		        Parent prototype object
 * @param[in]  vData		            Data for host object.
 * @param[out] pvNewObject		        New instance
 * @retval 	JC_OK                       Indicates success
 * @retval 	JC_ERR_MEMORY_ALLOCATION    Indicates memory allocation error
 * @retval Script Engine related error
 *
 * If underlying script engine provides garbage collection, caller shall not 
 * free memory allocated for pvNewObject.Otherwise caller has to free
 * memory allocated for pvNewObject.
 */

JC_RETCODE jdi_SECreateHostObject (
         SE_SESSION				vSession, 
         SE_OBJECT              vParentProto, 
         JC_HANDLE              vData, 
         SE_OBJECT				*pvNewObject 
        ) ;	

/** 
 * @brief Creates a host object without data
 * @param[in]  vSession		            Script engine session
 * @param[in]  vParentProto		        Parent prototype object
 * @param[out] pvNewObject		        New instance
 * @retval 	JC_OK                       Indicates success
 * @retval 	JC_ERR_MEMORY_ALLOCATION    Indicates memory allocation error
 * @retval Script Engine related error
 *
 * TODO : Added to handle dummy implementation of style object;Remove later
 */

JC_RETCODE jdi_SECreateHostObjectSansData (
         SE_SESSION				vSession, 
         SE_OBJECT              vParentProto, 
         SE_OBJECT				*pvNewObject 
        ) ;

/** 
 * @brief Set given object as "this" value for current active execution context
 * @param[in]  vSession		            Script engine session
 * @param[in]  vObject		            Host Object
 * @retval 	Nil
 */

void jdi_SESetThisValueObj (SE_SESSION			vSession, 
							SE_OBJECT			vObject
						   ) ;

/** 
 * @brief Get Object.prototype object
 * @param[in]  vSession		            Script engine session
 * @retval 	Object.prototype object
 */

SE_OBJECT jdi_SEGetObjectPrototype (SE_SESSION		vSession) ;

/** 
 * @brief Get function name of an SE_VALUE
 * @param[in]  vSession		            Script engine session
 * @param[in]  vValue		            Given SE_VALUE
 * @param[out] ppmName		            Function name (Can be JC_NULL
 *										for anonymous functions)
 * @retval 	JC_OK                       Indicates success
 * @retval Script Engine related error
 *
 * Caller shall not free memory allocated for ppmName
 */

JC_RETCODE jdi_SEGetFunctionName (SE_SESSION	 vSession, 
                                  SE_VALUE       vValue, 
								  JC_CHAR		 **ppmName) ;

/** 
 * @brief Registers a method on a host object
 * @param[in]  vSession		            Script engine session
 * @param[in]  vHostObject		        Host object
 * @param[in]  vMethodNameRef		    Method name reference
 * @param[in]  cbMethod		            Callback registered for the method
 * @param[in]  uiArgCnt                 Number of arguments for the method
 * @retval 	JC_OK                       Indicates success
 * @retval 	JC_ERR_MEMORY_ALLOCATION    Indicates memory allocation error
 * @retval Script Engine related error
 */

JC_RETCODE jdi_SERegisterMethod (SE_SESSION			vSession, 
                                 SE_OBJECT          vHostObject, 
                                 SE_STRING          vMethodNameRef, 
                                 CB_SCO_CALL_METHOD cbMethod, 
                                 JC_UINT32          uiArgCnt
                                ) ;

/** 
 * @brief Registers a method on a permanent host object
 * @param[in]  vContext		            Script engine Context
 * @param[in]  vHostObject		        Host object
 * @param[in]  vMethodNameRef		    Method name reference
 * @param[in]  cbMethod		            Callback registered for the method
 * @param[in]  uiArgCnt                 Number of arguments for the method
 * @retval 	JC_OK                       Indicates success
 * @retval 	JC_ERR_MEMORY_ALLOCATION    Indicates memory allocation error
 * @retval Script Engine related error
 */

JC_RETCODE jdi_SEPermRegisterMethod (SE_CONTEXT         vContext, 
                                     SE_OBJECT          vHostObject, 
                                     SE_STRING          vMethodNameRef, 
                                     CB_SCO_CALL_METHOD cbMethod, 
                                     JC_UINT32          uiArgCnt
                                    ) ;

/** 
 * @brief Registers a method on global object
 * @param[in]  vSession		            Script engine session
 * @param[in]  vMethodNameRef		    Method name reference
 * @param[in]  cbMethod		            Callback registered for the method
 * @param[in]  uiArgCnt                 Number of arguments for the method
 * @retval 	JC_OK                       Indicates success
 * @retval 	JC_ERR_MEMORY_ALLOCATION    Indicates memory allocation error
 * @retval Script Engine related error
 */

JC_RETCODE jdi_SERegisterGlobalMethod (SE_SESSION		  vSession, 
                                       SE_STRING          vMethodNameRef, 
                                       CB_SCO_CALL_METHOD cbMethod, 
                                       JC_UINT32          uiArgCnt
                                      ) ;

/** 
 * @brief Registers a property for a host object
 * @param[in]  vSession		            Script engine session
 * @param[in]  vHostObject		        Host object
 * @param[in]  vPropNameRef		        Property name reference
 * @param[in]  vValue		            Value of the property
 * @param[in]  uiAttributes		        Attributes for the property
 * @retval 	JC_OK                       Indicates success
 * @retval 	JC_ERR_MEMORY_ALLOCATION    Indicates memory allocation error
 * @retval Script Engine related error
 */

JC_RETCODE jdi_SERegisterProperty (SE_SESSION	  vSession, 
                                   SE_OBJECT      vHostObject, 
                                   SE_STRING      vPropNameRef, 
                                   SE_VALUE       vValue, 
                                   JC_UINT32      uiAttributes
                                  ) ;

/** 
 * @brief Registers a property for a permanent host object
 * @param[in]  vContext		            Script engine Context
 * @param[in]  vHostObject		        Host object
 * @param[in]  vPropNameRef		        Property name reference
 * @param[in]  vValue		            Value of the property
 * @param[in]  uiAttributes		        Attributes for the property
 * @retval 	JC_OK                       Indicates success
 * @retval 	JC_ERR_MEMORY_ALLOCATION    Indicates memory allocation error
 * @retval Script Engine related error
 */

JC_RETCODE jdi_SEPermRegisterProperty (SE_CONTEXT	  vContext, 
                                       SE_OBJECT      vHostObject, 
                                       SE_STRING      vPropNameRef, 
                                       SE_VALUE       vValue, 
                                       JC_UINT32      uiAttributes
                                      ) ;

/** 
 * @brief Registers a Number property for a host object
 * @param[in]  vSession		            Script engine session
 * @param[in]  vHostObject		        Host object
 * @param[in]  vPropNameRef		        Property name reference
 * @param[in]  dValue		            Value of the property
 * @param[in]  uiAttributes		        Attributes for the property
 * @retval 	JC_OK                       Indicates success
 * @retval 	JC_ERR_MEMORY_ALLOCATION    Indicates memory allocation error
 * @retval Script Engine related error
 */

JC_RETCODE jdi_SERegisterNumberProperty (SE_SESSION	    vSession, 
                                         SE_OBJECT      vHostObject, 
                                         SE_STRING      vPropNameRef,  
                                         JC_DOUBLE      dValue, 
                                         JC_UINT32      uiAttributes
                                        ) ;

/** 
 * @brief Registers a Number property for a permanent host object
 * @param[in]  vContext		            Script engine context
 * @param[in]  vHostObject		        Host object
 * @param[in]  vPropNameRef		        Property name reference
 * @param[in]  dValue		            Value of the property
 * @param[in]  uiAttributes		        Attributes for the property
 * @retval 	JC_OK                       Indicates success
 * @retval 	JC_ERR_MEMORY_ALLOCATION    Indicates memory allocation error
 * @retval Script Engine related error
 */

JC_RETCODE jdi_SEPermRegisterNumberProperty (SE_CONTEXT	    vContext, 
                                             SE_OBJECT      vHostObject, 
                                             SE_STRING      vPropNameRef,  
                                             JC_DOUBLE      dValue, 
                                             JC_UINT32      uiAttributes
                                            ) ;

/** 
 * @brief Registers a Number property for Global object
 * @param[in]  vSession		            Script engine session
 * @param[in]  vPropNameRef		        Property name reference
 * @param[in]  dValue		            Value of the property
 * @param[in]  uiAttributes		        Attributes for the property
 * @retval 	JC_OK                       Indicates success
 * @retval 	JC_ERR_MEMORY_ALLOCATION    Indicates memory allocation error
 * @retval Script Engine related error
 */

JC_RETCODE jdi_SERegisterGlobalNumberProperty (SE_SESSION	vSession, 
											   SE_STRING	vPropNameRef, 
											   JC_DOUBLE	dValue, 
											   JC_UINT32	uiAttributes
											  ) ;
/**
 * @brief Registers a Boolean property for a host object
 * @param[in]  vSession		            Script engine session
 * @param[in]  vHostObject		        Host object
 * @param[in]  vPropNameRef		        Property name reference
 * @param[in]  bValue		            Boolean value of property
 * @param[in]  uiAttributes		        Attributes for the property
 * @retval 	JC_OK                       Indicates success
 * @retval 	JC_ERR_MEMORY_ALLOCATION    Indicates memory allocation error
 * @retval Script Engine related error
 */

JC_RETCODE jdi_SERegisterBooleanProperty (SE_SESSION	vSession, 
                                          SE_OBJECT     vHostObject, 
                                          SE_STRING     vPropNameRef, 
                                          JC_BOOLEAN    bValue, 
                                          JC_UINT32     uiAttributes
                                         ) ;

/**
 * @brief Registers a Boolean property for a permanent host object
 * @param[in]  vContext		            Script engine context
 * @param[in]  vHostObject		        Host object
 * @param[in]  vPropNameRef		        Property name reference
 * @param[in]  bValue		            Boolean value of property
 * @param[in]  uiAttributes		        Attributes for the property
 * @retval 	JC_OK                       Indicates success
 * @retval 	JC_ERR_MEMORY_ALLOCATION    Indicates memory allocation error
 * @retval Script Engine related error
 */

JC_RETCODE jdi_SEPermRegisterBooleanProperty (SE_CONTEXT	vContext, 
                                              SE_OBJECT     vHostObject, 
                                              SE_STRING     vPropNameRef, 
                                              JC_BOOLEAN    bValue, 
                                              JC_UINT32     uiAttributes
                                             ) ;

/** 
 * @brief Registers a Boolean property for Global object
 * @param[in]  vSession		            Script engine session
 * @param[in]  vPropNameRef		        Property name reference
 * @param[in]  bValue		            Boolean value of property
 * @param[in]  uiAttributes		        Attributes for the property
 * @retval 	JC_OK                       Indicates success
 * @retval 	JC_ERR_MEMORY_ALLOCATION    Indicates memory allocation error
 * @retval Script Engine related error
 */

JC_RETCODE jdi_SERegisterGlobalBooleanProperty (SE_SESSION	  vSession, 
                                                SE_STRING     vPropNameRef, 
                                                JC_BOOLEAN    bValue, 
                                                JC_UINT32     uiAttributes
                                               ) ;
/** 
 * @brief Registers an Object property for a host object
 * @param[in]  vSession		            Script engine session
 * @param[in]  vHostObject		        Host object
 * @param[in]  vPropNameRef		        Property name reference
 * @param[in]  vObject		            Value of the property (can be JC_NULL)
 * @param[in]  uiAttributes		        Attributes for the property
 * @retval 	JC_OK                       Indicates success
 * @retval 	JC_ERR_MEMORY_ALLOCATION    Indicates memory allocation error
 * @retval Script Engine related error
 *
 * The API sets the value of object property as JC_NULL.
 */

JC_RETCODE jdi_SERegisterObjectProperty (SE_SESSION	    vSession, 
                                         SE_OBJECT      vHostObject, 
                                         SE_STRING      vPropNameRef, 
                                         SE_OBJECT      vObject, 
                                         JC_UINT32      uiAttributes
                                        ) ;

/** 
 * @brief Registers an Object property for a permanent host object
 * @param[in]  vContext		            Script engine context
 * @param[in]  vHostObject		        Host object
 * @param[in]  vPropNameRef		        Property name reference
 * @param[in]  vObject		            Value of the property (can be JC_NULL)
 * @param[in]  uiAttributes		        Attributes for the property
 * @retval 	JC_OK                       Indicates success
 * @retval 	JC_ERR_MEMORY_ALLOCATION    Indicates memory allocation error
 * @retval Script Engine related error
 *
 * The API sets the value of object property as JC_NULL.
 */

JC_RETCODE jdi_SEPermRegisterObjectProperty (SE_CONTEXT	    vContext, 
                                             SE_OBJECT      vHostObject, 
                                             SE_STRING      vPropNameRef, 
                                             SE_OBJECT      vObject, 
                                             JC_UINT32      uiAttributes
                                            ) ;

/** 
 * @brief Registers an Object property for global object
 * @param[in]  vSession		            Script engine session
 * @param[in]  vPropNameRef		        Property name reference
 * @param[in]  vObject		            Object (can't be JC_NULL)
 * @param[in]  uiAttributes		        Attributes for the property
 * @retval 	JC_OK                       Indicates success
 * @retval 	JC_ERR_MEMORY_ALLOCATION    Indicates memory allocation error
 * @retval Script Engine related error
 */

JC_RETCODE jdi_SERegisterGlobalObjectProperty (SE_SESSION	  vSession,  
                                               SE_STRING      vPropNameRef, 
                                               SE_OBJECT      vObject, 
                                               JC_UINT32      uiAttributes
                                              ) ;

/** 
 * @brief Registers an Object property for global object with value set to undefined
 * @param[in]  vSession					Script engine session
 * @param[in]  vPropNameRef				Property name reference
  * @param[in]  uiAttributes			Attributes for the property
 * @retval 	JC_OK						Indicates success
 * @retval 	JC_ERR_MEMORY_ALLOCATION	Indicates memory allocation error
 * @retval Script Engine related error
 */
JC_RETCODE jdi_SERegisterGlobalUndefinedProperty (SE_SESSION	vSession, 
												  SE_STRING		vPropNameRef, 
												  JC_UINT32		uiAttributes
												  ) ;

/** 
 * @brief Registers a String property for a host object
 * @param[in]  vSession		            Script engine session
 * @param[in]  vHostObject		        Host object
 * @param[in]  vPropNameRef		        Property name reference
 * @param[in]  pmValue		            Value of the property (can be JC_NULL)
 * @param[in]  uiAttributes		        Attributes for the property
 * @retval 	JC_OK                       Indicates success
 * @retval 	JC_ERR_MEMORY_ALLOCATION    Indicates memory allocation error
 * @retval Script Engine related error
 */

JC_RETCODE jdi_SERegisterStringProperty (SE_SESSION	    vSession, 
                                         SE_OBJECT      vHostObject, 
                                         SE_STRING      vPropNameRef, 
                                         JC_CHAR        *pmValue, 
                                         JC_UINT32      uiAttributes
                                        ) ;

/** 
 * @brief Registers a String property for a host object
 * @param[in]  vContext		            Script engine context
 * @param[in]  vHostObject		        Host object
 * @param[in]  vPropNameRef		        Property name reference
 * @param[in]  pmValue		            Value of the property (can be JC_NULL)
 * @param[in]  uiAttributes		        Attributes for the property
 * @retval 	JC_OK                       Indicates success
 * @retval 	JC_ERR_MEMORY_ALLOCATION    Indicates memory allocation error
 * @retval Script Engine related error
 */

JC_RETCODE jdi_SEPermRegisterStringProperty (SE_CONTEXT	    vContext, 
                                             SE_OBJECT      vHostObject, 
                                             SE_STRING      vPropNameRef, 
                                             JC_CHAR		*pmValue, 
                                             JC_UINT32      uiAttributes
                                            ) ;

/** 
 * @brief Registers a String property for global object
 * @param[in]  vSession		            Script engine session
 * @param[in]  vPropNameRef		        Property name reference
 * @param[in]  pcValue		            Value of the property (can be JC_NULL)
 * @param[in]  uiAttributes		        Attributes for the property
 * @retval 	JC_OK                       Indicates success
 * @retval 	JC_ERR_MEMORY_ALLOCATION    Indicates memory allocation error
 * @retval Script Engine related error
 */

JC_RETCODE jdi_SERegisterGlobalStringProperty (SE_SESSION	  vSession, 
                                               SE_STRING      vPropNameRef, 
                                               JC_INT8        *pcValue, 
                                               JC_UINT32      uiAttributes
                                              ) ;

/** 
 * @brief Converts any given SE_VALUE to JC_CHAR
 * @param[in]  vSession		            Script engine session
 * @param[in]  vValue		            Given SE_VALUE
 * @param[in]  ppmString		        JC_CHAR string
 * @retval 	JC_OK                       Indicates success
 * @retval 	JC_ERR_MEMORY_ALLOCATION    Indicates memory allocation error
 * @retval Script Engine related error
 *
 * Caller shall free memory allocated for ppmString using jdi_SEFreeString().
 */

JC_RETCODE  jdi_SEValueToTChar (SE_SESSION		  vSession, 
                                SE_VALUE          vValue,
                                JC_CHAR           **ppmString
                               ) ;

/**
 * @brief Convert SE_VALUE to a NULL terminated JC_INT8 string
 * @param[in]  vSession		           Script engine session
 * @param[in]  vValue			       Given SE_VALUE
 * @param[out] ppcString			   NULL terminated JC_INT8 string.
 *                                     JC_NULL for an empty string.
 * @retval JC_OK     				   Success
 * @retval JC_ERR_MEMORY_ALLOCATION    Indicates memory allocation error
 * @retval Script Engine related error
 *
 * Caller shall free the memory allocated for ppcString using jdi_MemFree.
 * The vMemHandle used for jdi_SEInitialize shall be used for jdi_MemFree.
 * ppcString is NULL for an empty string.
 */

JC_RETCODE jdi_SEValueToChar (SE_SESSION	  vSession, 
                              SE_VALUE		  vValue, 
                              JC_INT8		  **ppcString
                             ) ;

/**
 * @brief Convert a NULL terminated JC_INT8 string to an SE_VALUE
 * @param[in]  vSession		           Script engine session
 * @param[in]  pcString			       NULL terminated JC_INT8 string.
 *                                     JC_NULL for an empty string.
 * @param[out] vValue			       New SE_VALUE
 * @retval JC_OK     				   Success
 * @retval JC_ERR_MEMORY_ALLOCATION    Indicates memory allocation error
 * 
 * Application shall allocate memory for vValue.
 */

JC_RETCODE jdi_SECharToValue (SE_SESSION	  vSession, 
                              JC_INT8		  *pcString, 
                              SE_VALUE		  vValue 
                             ) ;

/**
 * @brief Convert a NULL terminated JC_CHAR string to an SE_VALUE
 * @param[in]  vSession		           Script engine session
 * @param[in]  pmString			       NULL terminated JC_CHAR string.
 *                                     JC_NULL for an empty string.
 * @param[out] vValue			       New SE_VALUE
 * @retval JC_OK     				   Success
 * @retval JC_ERR_MEMORY_ALLOCATION    Indicates memory allocation error
 * 
 * Application shall allocate memory for vValue.
 */

JC_RETCODE jdi_SETCharToValue (SE_SESSION	  vSession, 
                               JC_CHAR		  *pmString, 
                               SE_VALUE		  vValue 
                              ) ;

/**
 * @brief Convert an SE_VALUE to a JC_BOOLEAN
 * @param[in]  vSession		           Script engine session
 * @param[in]  vValue			       Given SE_VALUE
 * @param[out] pbBool			       JC_BOOLEAN value
 * @retval JC_OK     				   Success
 * @retval Script Engine related error
 */

JC_RETCODE jdi_SEValueToBoolean (SE_SESSION	    vSession, 
                                 SE_VALUE       vValue, 
                                 JC_BOOLEAN     *pbBool) ;

/**
 * @brief Convert an SE_VALUE to a JC_INT32
 * @param[in]  vSession		           Script engine session
 * @param[in]  vValue			       Given SE_VALUE
 * @param[out] piNumber			       Number value
 * @retval JC_OK     				   Success
 * @retval Script Engine related error
 */

JC_RETCODE jdi_SEValueToInt32 (SE_SESSION	vSession, 
                               SE_VALUE     vValue, 
                               JC_INT32     *piNumber) ;

/**
 * @brief Convert an SE_VALUE to a JC_UINT32
 * @param[in]  vSession		           Script engine session
 * @param[in]  vValue			       Given SE_VALUE
 * @param[out] puiNumber			   Number value
 * @retval JC_OK     				   Success
 * @retval Script Engine related error
 */

JC_RETCODE jdi_SEValueToUInt32 (SE_SESSION	vSession, 
                                SE_VALUE    vValue, 
                                JC_UINT32   *puiNumber) ;

/**
 * @brief Convert a JC_DOUBLE to an SE_VALUE
 * @param[in]  vSession		           Script engine session
 * @param[in]  dNumber			       Number value
 * @param[out] vValue			       New SE_VALUE
 * @retval JC_OK     				   Success
 * @retval JC_ERR_MEMORY_ALLOCATION    Indicates memory allocation error
 * @retval Script Engine related error
 *
 * Application shall allocate memory for vValue.
 */

JC_RETCODE jdi_SENumberToValue (SE_SESSION	  vSession, 
                                JC_DOUBLE     dNumber, 
                                SE_VALUE      vValue
                               ) ;

/**
 * @brief Convert an SE_VALUE to a Host Object
 * @param[in]  vSession		           Script engine session
 * @param[in]  vValue			       Given SE_VALUE
 * @param[out] pvObject			       Host Object
 * @retval JC_OK     				   Success
 * @retval Script Engine related error
 *
 * Caller shall free pvObject using jdi_SEFreeObject
 */

JC_RETCODE jdi_SEValueToObject (SE_SESSION	    vSession, 
                                SE_VALUE        vValue, 
                                SE_OBJECT		*pvObject) ;


/**
 * @brief Convert an SE_STRING to a NULL terminated JC_CHAR string
 * @param[in]  vSession		           Script engine session
 * @param[in]  vString			       Given SE_STRING
 * @param[out] ppmString			   New JC_CHAR string.
 *                                     JC_NULL for an empty string.
 * @retval JC_OK     				   Success
 *
 * Caller shall not free ppmString.ppmString is NULL for an empty string.
 */

JC_RETCODE  jdi_SEStringToTChar (SE_SESSION			vSession, 
                                 SE_STRING          vString, 
                                 JC_CHAR            **ppmString
                                ) ;

/**
 * @brief Convert an SE_STRING to a NULL terminated JC_INT8 string
 * @param[in]  vSession		           Script engine session
 * @param[in]  vString			       Given SE_STRING
 * @param[out] ppcString			   New JC_INT8 string.
 *                                     JC_NULL for an empty string.
 * @retval JC_OK     				   Success
 * @retval JC_ERR_MEMORY_ALLOCATION    Indicates memory allocation error
 *
 * The API is needed since SEE_string's data member is not NULL terminated.
 * Caller shall free memory allocated for ppcString using jdi_MemFree.
 * The vMemHandle used for jdi_SEInitialize shall be used for jdi_MemFree.
 * ppcString is NULL for an empty string.
 */

JC_RETCODE  jdi_SEStringToChar (SE_SESSION		   vSession, 
                                SE_STRING          vString, 
                                JC_INT8            **ppcString
                               ) ;

/** 
 * @brief Performs string interning for a string
 * @param[in]  vContext		            Script engine context
 * @param[in]  pmString		            NULL terminated string
 * @param[out] pvString		            Interned string
 * @retval 	JC_OK                       Indicates success
 * @retval 	JC_ERR_MEMORY_ALLOCATION    Indicates memory allocation error
 * @retval Script Engine related error
 *
 */

JC_RETCODE jdi_SEInternString (SE_CONTEXT   vContext, 
                               JC_CHAR      *pmString, 
                               SE_STRING    *pvString
                              ) ;

/** 
 * @brief Inserts the string to the sphinx string table in the session
 * @param[in]  vSession		            Script engine session
 * @param[in]  pcString		            NULL terminated string
 * @param[out] pvString		            Interned string
 * @retval 	JC_OK                       Indicates success
 * @retval 	JC_ERR_MEMORY_ALLOCATION    Indicates memory allocation error
 * @retval Script Engine related error
 */

JC_RETCODE jdi_SETempInternString (SE_SESSION		vSession, 
								   JC_INT8			*pcString, 
								   SE_STRING		*pvString
								   ) ;

/** 
 * @brief Registers an error object
 * @param[in]  vSession		            Script engine session
 * @param[in]  vObjNameRef		        Error object name reference
 * @param[out] pvProtoObj		        Error object's prototype
 * @param[out] pvCtorObj		        Error object's constructor
 * @retval 	JC_OK                       Indicates success
 * @retval 	JC_ERR_MEMORY_ALLOCATION    Indicates memory allocation error
 * @retval Script Engine related error
 */

JC_RETCODE  jdi_SERegisterErrorObject (SE_SESSION		    vSession, 
                                       SE_STRING            vObjNameRef, 
                                       SE_OBJECT            *pvProtoObj, 
                                       SE_OBJECT            *pvCtorObj
                                    ) ;

/** 
 * @brief Registers a permanent error object
 * @param[in]  vContext		            Script engine context
 * @param[in]  vObjNameRef		        Error object name reference
 * @param[out] pvProtoObj		        Error object's prototype
 * @param[out] pvCtorObj		        Error object's constructor
 * @retval 	JC_OK                       Indicates success
 * @retval 	JC_ERR_MEMORY_ALLOCATION    Indicates memory allocation error
 * @retval Script Engine related error
 */

JC_RETCODE  jdi_SEPermRegisterErrorObject (SE_CONTEXT		    vContext, 
                                           SE_STRING            vObjNameRef, 
                                           SE_OBJECT            *pvProtoObj, 
                                           SE_OBJECT            *pvCtorObj
                                          ) ;

/** 
 * @brief Creates an object using given constructor object
 * @param[in]  vSession		            Script engine session
 * @param[in]  vCtorObj		            Constructor object
 * @param[in]  uhArgsCnt		        Number of arguments for constructor
 * @param[in]  pvArgList		        Argument list for constructor
 *                                      (can be JC_NULL)
 * @param[out] pvNewObj		            Newly created object
 * @retval 	JC_OK                       Indicates success
 * @retval 	JC_ERR_MEMORY_ALLOCATION    Indicates memory allocation error
 * @retval Script Engine related error
 */

JC_RETCODE jdi_SEConstructObject (SE_SESSION		 vSession, 
                                  SE_OBJECT          vCtorObj, 
							      JC_UINT16          uhArgsCnt,
							      SE_VALUE           *pvArgList, 
                                  SE_OBJECT          *pvNewObj       
                                 ) ;

/** 
 * @brief Resumes execution of script
 * @param[in]  vSession		            Script engine session
 * @param[in]  bIsExcep		            E_TRUE in case of Exception
 * @param[in]  vValue		            Result of blocking call
 * @param[out] pvResult		            Final Result
 * @retval 	JC_OK                       Indicates success
 * @retval 	JC_ERR_MEMORY_ALLOCATION    Indicates memory allocation error
 * @retval  JC_ERR_SE_SUSPENSION		Indicates suspension of script execution since 
 *									    user interaction is required,
 * @retval  JC_ERR_SE_OWR_SUSPENSION	Indicates suspension of script execution 
 *										since maximum operations threshold is exceeded
 * @retval  JC_ERR_SEC_COMPILATION		Indicates script source has syntax errors,
 * @retval  JC_ERR_SEC_EVALUATION		Indicates script source has run-time errors.
 * 
 * The function is used to continue execution of script after a blocking call.
 * (eg: alert,prompt,confirm calls).
 * Caller shall free pvResult using jdi_SEFreeValue()
 */

JC_RETCODE jdi_SEResumeExecution (SE_SESSION		vSession,
								  JC_BOOLEAN		bIsExcep,
                                  SE_VALUE			vValue, 
                                  SE_VALUE			*pvResult
                                 ) ;

/** 
 * @brief Execute function with a given function name
 * @param[in]  vSession		            Script engine session
 * @param[in]  pmFuncName		        Function Name
 * @param[in]  uhArgsCnt				No of arguments
 * @param[in]  pvArgList				Argument List for function 
 *                                      (can be JC_NULL)
 * @param[out] pvResult					Final Result
 * @retval 	JC_OK                       Indicates success
 * @retval 	JC_ERR_MEMORY_ALLOCATION    Indicates memory allocation error
 * @retval  JC_ERR_SE_SUSPENSION		Indicates suspension of script execution since 
 *									    user interaction is required,
 * @retval  JC_ERR_SE_OWR_SUSPENSION	Indicates suspension of script execution 
 *										since maximum operations threshold is exceeded
 * @retval  JC_ERR_SEC_EVALUATION		Indicates script source has run-time errors.
 *
 * Caller shall free pvResult using jdi_SEFreeValue()
 */

JC_RETCODE jdi_SEExecuteFunction (SE_SESSION	  vSession, 
								  JC_CHAR		  *pmFuncName,
 						          JC_UINT16       uhArgsCnt,
							      SE_VALUE        *pvArgList, 
								  SE_VALUE		  *pvResult
								 ) ;

/** 
 * @brief Frees an SE_VALUE content & SE_VALUE
 * @param[in]  vSession		            Script engine session
 * @param[in]  vValue		            SE_VALUE to be freed
 * @retval 	Nil
 *
 * SE_VALUE is freed using memory handle given in jdi_SEInitialize().
 */

void jdi_SEFreeValue (SE_SESSION	  vSession, 
                      SE_VALUE        vValue
                     ) ;

/** 
 * @brief Copy SE_VALUE
 * @param[in]  vSession		            Script engine session
 * @param[in]  vSrcVal		            SE_VALUE to be copied
 * @param[out] vDestVal		            Destination SE_VALUE
 * @retval 	JC_OK                       Indicates success
 * @retval 	JC_ERR_MEMORY_ALLOCATION    Indicates memory allocation error
 *
 * Caller shall allocate for vDestVal.
 */

JC_RETCODE jdi_SECopyValue (SE_SESSION		   vSession, 
                            const SE_VALUE     vSrcVal, 
                            SE_VALUE           vDestVal
                           ) ;

/** 
 * @brief Throw a native error
 * @param[in]  vSession		            Script engine session
 * @param[in]  eType		            Type of native error
 * @param[out] vResultVal	            Value containing error object
 * @retval 	JC_ERR_SE_EXCEPTION         Indicates Exception to be thrown
 * @retval 	JC_ERR_MEMORY_ALLOCATION    Indicates memory allocation error
 * @retval Script Engine related error
 *
 * Caller shall allocate for vResultVal.
 */

JC_RETCODE jdi_SEThrowError (SE_SESSION			vSession, 
                             E_SE_NATIVE_ERROR  eType, 
							 SE_VALUE           vResultVal
							) ;

/** 
 * @brief Creates an Array object
 * @param[in]  vSession		            Script engine session
 * @param[out] pvArrayObj		        New Array Object
 * @retval 	JC_OK                       Indicates success
 * @retval Script Engine related error
 * 
 * The function creates an empty array.Caller shall not free pvArrayObj.
 */

JC_RETCODE jdi_SEConstructArrayObject (SE_SESSION		  vSession, 
									   SE_OBJECT          *pvArrayObj
									  ) ;

/** 
 * @brief Frees a host object
 * @param[in]  vSession		            Script engine session
 * @param[in]  vObject		            Object to be freed
 * @retval 	Nil
 */

void jdi_SEFreeObject (SE_SESSION		vSession, 
					   SE_OBJECT        vObject
					  ) ;

/** 
 * @brief Frees a string
 * @param[in]  vSession		            Script engine session
 * @param[in]  pmString		            String to be freed
 * @retval 	Nil
 */

void jdi_SEFreeString (SE_SESSION		vSession, 
					   JC_CHAR			*pmString
					  ) ;

/** 
 * @brief Set [[Get]] behaviour of global object
 * @param[in]  vSession		            Script engine session
 * @param[in]  cbGetArg		            [[Get]] behaviour for global object
 * @retval 	Nil
 */

void jdi_SESetGlobalObjectGetBehaviour (SE_SESSION			vSession, 
										CB_SCO_GET_METHOD	cbGetArg
									   ) ;

/** 
 * @brief Set [[Put]] behaviour of global object
 * @param[in]  vSession		            Script engine session
 * @param[in]  cbPutArg		            [[Put]] behaviour for global object
 * @retval 	Nil
 */

void jdi_SESetGlobalObjectPutBehaviour (SE_SESSION			vSession, 
										CB_SCO_PUT_METHOD	cbPutArg
									   ) ;

/** 
 * @brief Creates a permanent host method.
 * @param[in]  vContext		            Script engine context
 * @param[in]  vMethodNameRef		    Method name reference
 * @param[in]  cbMethod		            Callback registered for the method
 * @param[in]  uiArgCnt                 Number of arguments for the method
 * @param[out] pvNewObject              New funtion object
 * @retval 	JC_OK                       Indicates success
 * @retval 	JC_ERR_MEMORY_ALLOCATION    Indicates memory allocation error
 * @retval Script Engine related error
 */

JC_RETCODE jdi_SEPermCreateHostFunctionObject (
                         SE_CONTEXT				vContext, 
                         SE_STRING				vMethodNameRef, 
                         CB_SCO_CALL_METHOD		cbMethod, 
                         JC_UINT8				uiArgCnt, 
                         SE_OBJECT				*pvNewObject 
                        ) ;

/** 
 * @brief Executes the given function object 
 * @param[in]  vSession					Script engine session
 * @param[in]  vFunObjVal				Function Object to be executed
 * @param[in]  pvArgList				List of arguments if any
 * @param[in]  uhNoOfArgs				No of arguments to the function
 * @param[out] pvResult					Result value
 * @retval 	JC_OK                       Indicates success
 * @retval 	JC_ERR_MEMORY_ALLOCATION    Indicates memory allocation error
 * @retval  JC_ERR_SE_SUSPENSION		Indicates suspension of script execution since 
 *									    user interaction is required,
 * @retval  JC_ERR_SE_OWR_SUSPENSION	Indicates suspension of script execution 
 *										since maximum operations threshold is exceeded
 * @retval  JC_ERR_SEC_EVALUATION		Indicates script source has run-time errors.
 *
 * Caller shall free pvResult using jdi_SEFreeValue()
 */

JC_RETCODE jdi_SEExecuteFunctionObject (SE_SESSION		vSession,
										SE_VALUE		vFunObjVal,
										SE_VALUE        *pvArgList, 
										JC_UINT16		uhNoOfArgs,
										SE_VALUE		*pvResult
										) ;
/** 
 * @brief Frees an SE_VALUE content
 * @param[in]  vSession		            Script engine session
 * @param[in]  vValue		            SE_VALUE whose content has to be freed
 * @retval 	Nil
 *
 */

void jdi_SEDestroyResultContent (SE_SESSION	  vSession, 
                                 SE_VALUE     vValue
                                ) ;
/** 
 * @brief Gets value of a Property 
 * @param[in]  vSession				Script engine session
 * @param[in]  vObject		        Object with which property is associated
 * @param[in]  vPropNameRef		    Property name reference
 * @param[out] vValue				Value of the property
 * @retval 	JC_OK					Indicates success
 * @retval Script Engine related error
 */

JC_RETCODE jdi_SEGetPropertyValue (SE_SESSION		vSession, 
								   SE_OBJECT		vObject, 
								   SE_STRING		vPropNameRef,
								   SE_VALUE			vValue
								   ) ;

/** 
 * @brief Puts value of a Property forcefully
 * @param[in]  vSession				Script engine session
 * @param[in]  vObject		        Object with which property is associated
 * @param[in]  vPropNameRef		    Property name reference
 * @param[in]  vValue				Value of the property
 * @param[out] vErrorObj			Error value if any 
 * @retval 	JC_OK					Indicates success
 * @retval Script Engine related error
 */

JC_RETCODE jdi_SEForcePutProperty (SE_SESSION		vSession, 
								   SE_OBJECT		vObject,
								   SE_STRING		vPropNameRef,
								   SE_VALUE			vValue, 
								   SE_VALUE			vErrorObj
								   ) ;

/** 
 * @brief resume after timeout response from application
 * @param[in]  vSession		            Script engine session
 * @param[in]  ppstArgList		        Arguments list before timeout
 * @retval 	JC_OK                       Indicates success
 * @retval 	JC_ERR_MEMORY_ALLOCATION    Indicates memory allocation error
 * @retval  JC_ERR_SE_SUSPENSION		Indicates suspension of script execution since 
 *									    user interaction is required,
 * @retval  JC_ERR_SE_OWR_SUSPENSION	Indicates suspension of script execution 
 *										since maximum operations threshold is exceeded
 * @retval  JC_ERR_SEC_COMPILATION		Indicates script source has syntax errors,
 * @retval  JC_ERR_SEC_EVALUATION		Indicates script source has run-time errors.
 *
 */

JC_RETCODE jdi_SEExecuteSetTimeOut (SE_SESSION			vSession, 
                                    ST_SE_VALUE         **ppstArgList, 
                                    JC_UINT32           uiArgCnt
                                   ) ;

/** 
 * @brief Terminates the Execution of a Script 
 * @param[in]  vSession		            Script engine session
 * @retval 	Nil
 *
 * Note : Call this method while the Script execution is in suspension and the 
 *		  resume execution is not needed.
 *
 */

void jdi_SETerminateScriptEval (SE_SESSION	vSession) ;

/** 
 * @brief Executes the given function object while the script engine is in suspension
 * @param[in]  vSession					Script engine session
 * @param[in]  vFunObjVal				Function Object to be executed
 * @param[in]  pvArgList				List of arguments if any
 * @param[in]  uhNoOfArgs				No of arguments to the function
 * @param[out] pvResult					Result value
 * @retval 	JC_OK                       Indicates success
 * @retval 	JC_ERR_MEMORY_ALLOCATION    Indicates memory allocation error
 * @retval  JC_ERR_SE_SUSPENSION		Indicates suspension of script execution since 
 *									    user interaction is required,
 * @retval  JC_ERR_SE_OWR_SUSPENSION	Indicates suspension of script execution 
 *										since maximum operations threshold is exceeded
 * @retval  JC_ERR_SEC_EVALUATION		Indicates script source has run-time errors.
 *
 * Caller shall free pvResult using jdi_SEFreeValue()
 */

JC_RETCODE jdi_SEExecFunctionObjectOnSusp (SE_SESSION		vSession,
										   SE_VALUE			vFunObjVal,
										   SE_VALUE			*pvArgList, 
										   JC_UINT16		uhNoOfArgs,
										   SE_VALUE			*pvResult) ;

/** 
 * @brief Get the Global window object for a given session. 
 * @param[in]  vSession		            Script engine session
 * @param[out] pvWindowObj	            Window object for the given session
 * @retval 	JC_OK					Indicates success
 * @retval Script Engine related error
 */

void jdi_SEGetSessionWindowObject (SE_SESSION	vSession,
								   SE_OBJECT	*pvWindowObj) ;

/** 
 * @brief Creates name alias for a host object
 * @param[in]  vSession					Script engine session
 * @param[in]  vPropNameRef				Property name reference
 * @param[in]  vAliasStrNameRef			Name reference of the string to be alliased
 * @retval 	JC_OK                       Indicates success
 * @retval 	JC_ERR_MEMORY_ALLOCATION    Indicates memory allocation error
 * @retval  Other Script Engine related errors
 */

JC_RETCODE jdi_SESetHostObjectAlias (SE_SESSION		vSession, 
									 SE_STRING		vPropNameRef, 
									 SE_STRING		vAliasStrNameRef) ;


/** 
 * @brief Registers a method on a custom object also set the data required
 * @param[in]  vSession		            Script engine session
 * @param[in]  vHostObject		        Host object
 * @param[in]  vMethodNameRef		    Method name reference
 * @param[in]  cbMethod		            Dummy callback registered for the method
 * @param[in]  vCallbackInfo			Handle contain (#ST_BOM_CUSTOM_METHOD *)required
 *										to invoke the applicatin callback for the method
 * @param[in]  uiArgCnt                 Number of arguments for the method
 * @retval 	JC_OK                       Indicates success
 * @retval 	JC_ERR_MEMORY_ALLOCATION    Indicates memory allocation error
 * @retval Script Engine related error
 */

JC_RETCODE jdi_SEPermRegisterCustomMethod (SE_CONTEXT			vContext,
										   SE_OBJECT			vHostObject, 
										   SE_STRING			vMethodNameRef,
										   CB_SCO_CALL_METHOD	cbMethod,
										   JC_HANDLE			vCallbackInfo,
										   JC_UINT32			 uiArgCnt
										   ) ;

/** 
 * @brief Registers a custom object method on Session, also set the data required
 * @param[in]  vSession		            Script engine session
 * @param[in]  vHostObject		        Host object
 * @param[in]  vMethodNameRef		    Method name reference
 * @param[in]  cbMethod		            Dummy callback registered for the method
 * @param[in]  vCallbackInfo			Handle contain (#ST_BOM_CUSTOM_METHOD *)required
 *										to invoke the applicatin callback for the method
 * @param[in]  uiArgCnt                 Number of arguments for the method
 * @retval 	JC_OK                       Indicates success
 * @retval 	JC_ERR_MEMORY_ALLOCATION    Indicates memory allocation error
 * @retval Script Engine related error
 */

JC_RETCODE jdi_SERegisterCustomMethod (SE_SESSION			vSession,
									   SE_OBJECT			vHostObject, 
									   SE_STRING			vMethodNameRef,
									   CB_SCO_CALL_METHOD	cbMethod,
									   JC_HANDLE			vCallbackInfo,
									   JC_UINT32			 uiArgCnt
									   ) ;
/** 
 * @brief Gives the prototype in which the given property is registered
 * @param[in]  vSession		            Script engine session
 * @param[in]  vHostObject		        Starting Host object
 * @param[in]  vPropNameRef				Property name reference
 * @param[in]  pvResultObj	            Reference to the result value
 * @retval 	JC_OK                       Indicates success
 * @retval 	JC_ERR_MEMORY_ALLOCATION    Indicates memory allocation error
 * @retval Script Engine related error
 * This function returns the prototype object in which the given property is registered
 * following the parent proto heirarchy.
 */

JC_RETCODE jdi_SEGetPrototypeOfProperty (SE_SESSION			vSession,
										 SE_OBJECT			vHostObject, 
										 SE_STRING			vPropNameRef,
										 SE_OBJECT			*pvResultObj
										 ) ;

/** 
 * @brief Creates a dummy factory function object.
 * @param[in]  vContext		            Script engine context
 * @param[in]  cbMethod		            Callback registered for the method
 * @param[in]  vMethodNameRef		    Method name reference
 * @param[out] pvResultObj              New funtion object
 * @retval 	JC_OK                       Indicates success
 * @retval 	JC_ERR_MEMORY_ALLOCATION    Indicates memory allocation error
 * @retval Script Engine related error
 */


JC_RETCODE	jdi_SECreateDummyFactoryFunction (SE_CONTEXT			vSEContext, 
#ifndef BROWSER_OBO_ENABLE
											  CB_SCO_CALL_METHOD	cbMethod,
#endif /* BROWSER_OBO_ENABLE */
											  SE_STRING				vMethodNameRef, 
											  SE_OBJECT				*pvResultObj) ;

/** 
 * @brief Frees the dummy factory function object.
 * @param[in]  vContext		            Script engine context
 * @param[out] vObject		            Dummy function object
 * @retval 	
 */

void	jdi_SEDeleteDummyFactoryFunction (SE_CONTEXT			vSEContext,
										  SE_OBJECT				vObject) ;

#endif /* _JDI_SE_H_ */

