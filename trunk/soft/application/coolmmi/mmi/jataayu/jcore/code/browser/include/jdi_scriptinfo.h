/***************************************************************************
 * $Id: jdi_scriptinfo.h,v 1.9 2009/03/04 13:04:13 kavithadevhtmlbrow Exp $
 * $Revision: 1.9 $
 * $Date: 2009/03/04 13:04:13 $
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
 * $Log: jdi_scriptinfo.h,v $
 * Revision 1.9  2009/03/04 13:04:13  kavithadevhtmlbrow
 * Doxygen Comments Updated
 *
 * Revision 1.8  2008/12/17 09:28:33  kavithadevhtmlbrow
 * UserAgent in Application Info
 *
 * Revision 1.7  2008/12/10 14:46:16  kavithadevhtmlbrow
 * window.open Support - DE_WINDOW_OPEN_ENABLE
 *
 * Revision 1.6  2008/06/19 11:01:23  shajudevhtmlbrow
 * changes in custome object implementation.
 *
 * Revision 1.5  2008/04/28 15:20:24  kavithadevhtmlbrow
 * Custom Object & DOM Parser Support
 *
 * Revision 1.4  2008/04/22 10:24:22  kavithadevhtmlbrow
 * Comments updated
 *
 * Revision 1.3  2008/04/22 06:11:49  shajudevhtmlbrow
 * Support for getting  offset values and screen info through script.
 *
 * Revision 1.2  2008/03/06 05:52:35  shajudevhtmlbrow
 * Applications info callback implemented.
 *
 * Revision 1.1  2007/12/05 13:54:22  kumardevhtmlbrow
 * Script info newly added, which is shared by application, DE and BOM.
 *
 *  
 ***************************************************************************/

/**
 * @file jdi_scriptinfo.h
 * @ingroup BrEngine
 * @brief Defines the exposed enumerations, structures of the script related that
 *		  can be used by the application and display engine modules.
 * 
 * This file defines the various structures and enumnerations that are used for the interface 
 * between the browser, display engine and script related modules.
 */

#ifndef JDI_SCRIPTINFO_H
#define JDI_SCRIPTINFO_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Specify a alert function call
 *
 * The application can implement alert as blocking or non-blocking window.
 */
typedef struct st_bom_alert
{
	JC_CHAR				*pmMessage ; /**< Specifies the alert message.JC_NULL is valid. */
	void				*pvCbArg ; /**< Specifies BOM argument that application has to pass in ST_BOM_ALERT_RESP.*/
} ST_BOM_ALERT ;

/**
 * @brief Specify a alert function call response
 *
 * The application can implement alert as blocking or non-blocking window.
 * If application chooses to implement alert as non-blocking window, it has
 * to send alert response to BOM layer.Application has to call jdi_BOMSetEvent
 * with E_BOM_ALERT_RESP and ST_BOM_ALERT_RESP as second and third parameters.
 */
typedef struct st_bom_alert_resp
{
	void				*pvCbArg ; /**< Specifies BOM argument passed in ST_BOM_ALERT.*/
} ST_BOM_ALERT_RESP ;

/**
 * @brief Specify a confirm function call
 *
 * The application can implement confirm as blocking or non-blocking window.
 */
typedef struct st_bom_confirm
{
	JC_CHAR				*pmMessage ; /**< Specifies the confirmation message.JC_NULL is valid */
    JC_CHAR             *pmOKLabel ; /**< Specifies optional OK label.JC_NULL is valid */
    JC_CHAR             *pmCancelLabel ; /**< Specifies optional cancel label.JC_NULL is valid */
	JC_BOOLEAN			bIsOkStatus ; /**< Specifies user response. */
    void				*pvCbArg ; /**< Specifies BOM argument that application has to pass in ST_BOM_CONFIRM_RESP. */
} ST_BOM_CONFIRM ;

/**
 * @brief Specify a confirm function call response
 *
 * The application can implement confirm as blocking or non-blocking window.
 * If application chooses to implement confirm as non-blocking window, it has
 * to send confirm response to BOM layer.Application has to call jdi_BOMSetEvent
 * with E_BOM_CONFIRM_RESP and ST_BOM_CONFIRM_RESP as second and third parameters.
 */
typedef struct st_bom_confirm_rsp
{
	void				*pvCbArg ; /**< Specifies BOM argument passed in ST_BOM_CONFIRM. */
	JC_BOOLEAN			bIsOkStatus ; /**< Specifies user response */
} ST_BOM_CONFIRM_RESP ;

/**
 * @brief Specify a prompt function call
 *
 * The application can implement prompt as blocking or non-blocking window.
 */
typedef struct st_bom_prompt
{
	JC_CHAR				*pmMessage ; /**< Specifies the prompt message.JC_NULL is valid */
	JC_CHAR				*pmValue ; /**< [IN] Specifies the default value.[OUT] Specifies value entered by user (if any). JC_NULL is valid. */
	JC_BOOLEAN			bIsOkStatus ; /**< Specifies user response */
	void				*pvCbArg ; /**< Specifies BOM argument that application has to pass in ST_BOM_PROMPT_RESP. */
} ST_BOM_PROMPT ;

/**
 * @brief Specify a prompt function call response
 *
 * The application can implement prompt as blocking or non-blocking window.
 * If application chooses to implement prompt as non-blocking window, it has
 * to send prompt response to BOM layer.Application has to call jdi_BOMSetEvent
 * with E_BOM_PROMPT_RESP and ST_BOM_PROMPT_RESP as second and third parameters.
 *
 */
typedef struct st_bom_prompt_rsp
{
	JC_CHAR				*pmValue ; /**< Specifies value entered by the user or default value. JC_NULL is valid. */
	JC_BOOLEAN			bIsOkStatus ; /**< Specifies user response */
	void				*pvCbArg ; /**< Specifies BOM argument passed in ST_BOM_PROMPT. */
} ST_BOM_PROMPT_RESP ;

/**
 * @brief Specify the informations related to application
 *
 * The application can provide these information and will be used in navigator object in BOM.
 *
 */
typedef struct st_bom_application_info
{
	JC_INT8		*pcAppName ; /**< Specifies the application name */
	JC_INT8		*pcAppVersion ; /**< Specifies the application version */
	JC_INT8		*pcPlatform ; /**< Specifies the browser platform */
	JC_INT8		*pcLanguage ; /**< Specifies the browser language in two character format */
	JC_INT8		*pcUserAgent ; /** Specifies the browser user agent */
} ST_BOM_APPLICATION_INFO ;


/**
 * @brief Specify screen's details
 */
typedef struct st_bom_screen_info
{
    JC_INT32        iAvailHeight ; /**< Specifies size in pixels of the visible vertical writable screen area for the window. */ 
    JC_INT32        iAvailwidth ;  /**< Specifies size in pixels of the visible horizontal writable screen area for the window. */ 
	JC_INT32		iColorDepth ;  /**< Specifies number of bits assigned to the management of color for the screen. */ 
	JC_INT32		iHeight ;      /**< Specifies physical height of the screen in pixels. */ 
	JC_INT32		iWidth ;       /**< Specifies physical width of the screen in pixels. */  
} ST_BOM_SCREEN_INFO ;

/**
 * @brief Specify a window.open call
 */
typedef struct st_bom_window_open
{
    JC_CHAR             *pmURL ;        /**< If JC_NULL, open existing window */
    JC_CHAR             *pmWindowName ; /**< Specifies the name of the window to be opened */
    JC_BOOLEAN          bAlwaysLowered ;/**< If yes, creates a new window that floats below other windows, whether it is active or not. This is a secure feature and must be set in signed scripts. */
    JC_BOOLEAN          bAlwaysRaised ; /**< If yes, creates a new window that floats on top of other windows, whether it is active or not. This is a secure feature and must be set in signed scripts.*/
    JC_BOOLEAN          bDependent ;    /**< If yes, creates a new window as a child of the current window. A dependent window closes when its parent window closes.*/
    JC_BOOLEAN          bDirectories ;  /**< If yes, creates the standard browser directory buttons, such as What's New and What's Cool.*/
    JC_BOOLEAN          bHotkeys ;      /**< If no (or 0), disables most hot keys in a new window that has no menu bar. The security and quit hotkeys remain enabled. */
    JC_BOOLEAN          bLocation ;     /**< If yes, creates a Location entry field.*/
    JC_BOOLEAN          bMenubar ;      /**< If yes, creates the menu at the top of the window. */
    JC_BOOLEAN          bPersonalbar ;  /**< If yes, creates the Personal Toolbar */
    JC_BOOLEAN          bResizable ;    /**< If yes, allows a user to resize the window */
    JC_BOOLEAN          bScrollbars ;   /**< If yes, creates horizontal and vertical scrollbars when the Document grows */
    JC_BOOLEAN          bStatus ;       /**< If yes, creates the status bar at the bottom of the window */
    JC_BOOLEAN          bTitlebar ;     /**< If yes, creates a window with a title bar */
    JC_BOOLEAN          bToolbar ;      /**< If yes, creates the standard browser toolbar */
    JC_BOOLEAN          bZlock ;        /**< If yes, creates a new window that does not rise above other windows */
    JC_INT32            iHeight ;       /**< Specifies the height of the window in pixels.-1 if unspecified */
    JC_INT32            iInnerHeight ;  /**< Specifies the height, in pixels, of the window's content area.-1 if unspecified */
    JC_INT32            iInnerWidth ;   /**< Specifies the width, in pixels, of the window's content area.-1 if unspecified  */
    JC_INT32            iOuterHeight ;  /**< Specifies the vertical dimension, in pixels.-1 if unspecified. */
    JC_INT32            iScreenX ;      /**< Specifies the distance the new window is placed from the left side of the screen.-1 if unspecified  */
    JC_INT32            iScreenY ;      /**< Specifies the distance the new window is placed from the top of the screen.-1 if unspecified  */
    JC_INT32            iWidth ;        /**< Specifies the width of the window in pixels. -1 if unspecified */
    JC_HANDLE           vWndHandle ;   /**< Window handle of the opened window */
} ST_BOM_WINDOW_OPEN ;


#ifdef BROWSER_CUSTOM_SCRIPT

/**
 * @brief Specify the callback type for custom objects
 */
typedef enum e_bom_custom_callback
{
	E_BOM_CUSTOM_OBJ_GET,	/**< Intimates application to [[GET]] the property value associated with an custom object (Argument passed #ST_BOM_CUSTOM_GET *) */ 
	E_BOM_CUSTOM_OBJ_PUT,	/**< Intimates application to [[PUT]] the property value associated with an custom object (Argument passed #ST_BOM_CUSTOM_PUT *) */ 
	E_BOM_CUSTOM_OBJ_CTOR,	/**< Intimates application about creating an instance of the custom object (Argument passed #ST_BOM_CUSTOM_CTOR *) */ 
	E_BOM_CUSTOM_OBJ_DELETE /**< Intimates application about the deletion of the custom object (Argument passed #ST_BOM_CUSTOM_DELETE *) */ 
} E_BOM_CUSTOM_CALLBACK ;

/**
 * @brief Specify the type of data contained inside JC_HANDLE data pointer 
 */
typedef enum e_bom_custom_data
{
	E_BOM_CUSTOM_UNDEFINED,	/**< Specify undefined type   **/
	E_BOM_CUSTOM_NUMBER,    /**< Specify number type   **/
	E_BOM_CUSTOM_STRING,    /**< Specify string type   **/
	E_BOM_CUSTOM_BOOLEAN,	/**< Specify boolean type   **/
	E_BOM_CUSTOM_OBJECT,	/**< Specify object type   **/
	E_BOM_CUSTOM_SOURCE	/**< Specify Pointer type   **/

} E_BOM_CUSTOM_DATA ;

/**
 * @brief Specify the data to be used while passing the value to BOM. 
 */
typedef struct st_bom_value
{
	E_BOM_CUSTOM_DATA		eType ;
	union u_bom_value
	{
		JC_UINT32			uiNumber ;
		JC_CHAR				*pmString ;
		JC_BOOLEAN			bBoolean ;
		JC_HANDLE			vObject ;
		JC_HANDLE			vDataPtr ;
	} uv ;
} ST_BOM_VALUE ;

/**
 * @brief	    Prototype of Application callback to be registered with each custom object.
 *
 * @param[in]	pvCallbackArg	Application can pass a context argument while initializing the custom object
 *								BOM layer pass the same argument to application when callback is invoked.
 * @param[in]	eCallbackType	Specifies type of callback for custom object 
 * @param[in]	pvCallbackInfo	Specifies the callback arguement(Args can be provided for the constructor 
 *								in the script).
 * @return		The callback function can return any application related error.
 *
 * Application shall implement a callback with this prototype and pass its address
 * in (#ST_BOM_CUSTOM_OBJECT *)for each object while initializing it which will handle al the callback 
 * types specified in E_BOM_CUSTOM_CALLBACK.
 * 
 */
typedef JC_RETCODE (* CB_BOM_CUSTOM_CALLBACK) (void						*pvCallbackArg,
											   E_BOM_CUSTOM_CALLBACK	eCallbackType,
											   void						*pvCallbackInfo) ;

/**
 * @brief	    Prototype of Application method to be provided with each method registration.
 *
 * @param[in]	pvCallbackArg	Application can pass a context argument while initializing the object.
 *								BOM layer pass the same argument to application when method is invoked.
 * @param[in]	vobjHandle		Specifies the object handle supplied from BOM. BOM will use this handle to 
 *								set the result value of the method execution.
 * @param[in]	vArgsPtr		Specifies list of arguements to the callback function 
 * @param[out]	uhArgCnt		Specifies the number of arguements.
 * @param[out]	vResultVal		Specifies the result pointer need to be passed in jdi_BOMSetCustomResult.
 * @return		The callback function can return any application related error.
 *
 * Application shall implement a callback with this prototype and pass its address
 * in (#ST_BOM_CUSTOM_METHOD *)for each method while registering it.
 * 
 */
typedef JC_RETCODE (* CB_BOM_CUSTOM_METHOD) (void				*pvCallbackArg,
											 JC_HANDLE			vobjHandle,
											 JC_HANDLE			vCustomDataPtr,
										     JC_HANDLE			vArgsPtr,
										     JC_UINT16			uhArgCnt,
										     ST_BOM_VALUE		*pstResultVal) ;


/**
 * @brief Handle E_BOM_CUSTOM_OBJ_GET custom callback.
 */
typedef struct st_bom_custom_get
{
	JC_HANDLE				vObjHandle ;	/**< Object handle supplied from BOM. This handle will be used by BOM to set the result value >**/
	JC_HANDLE				vPropertyID ;	/**< Specifies the ID value of the property i.e. returned from jdi_BOMInitCustomObject() with each property. >**/
	JC_HANDLE				vCustomDataPtr ;/**< Specifies the pointer to the data provided from custom application from the E_BOM_CUSTOM_OBJ_CTOR callback >**/
	ST_BOM_VALUE			*pstGetValue ;
} ST_BOM_CUSTOM_GET ;

/**
 * @brief Handle E_BOM_CUSTOM_OBJ_PUT custom callback.
 */
typedef struct st_bom_custom_put
{
	JC_HANDLE				vObjHandle ;	/**< Specifies the handle in BOM for the custom object (i.e. returned from jdi_BOMInitCustomObject) >**/
	JC_HANDLE				vNewObjHandle ;	/**< Specifies the Object handle on which the put is invoked >**/
	JC_HANDLE				vPropertyID ;	/**< Specifies the ID value of the property i.e. returned from jdi_BOMInitCustomObject() with each property. >**/
	JC_HANDLE				vCustomDataPtr ;/**< Specifies the pointer to the data provided from custom application from the E_BOM_CUSTOM_OBJ_CTOR callback >**/
	ST_BOM_VALUE			*pstPutValue ;
} ST_BOM_CUSTOM_PUT ;

/**
 * @brief Handle E_BOM_CUSTOM_OBJ_CTOR custom callback.
 */
typedef struct st_bom_custom_ctor
{
	JC_HANDLE		vObjHandle ;	/**< Specifies the handle in BOM for the custom object (i.e. returned from jdi_BOMInitCustomObject) >**/
	JC_HANDLE		vNewObjHandle ; /**< Object handle supplied from BOM i.e. the new instance of the constructor >**/
	JC_HANDLE		vArgsPtr ;		/**< Specifies the pointer to the arguements array specified in the constructor function >**/
	JC_INT16		uhArgCnt ;		/**< Specifies the no of arguements in the constructor function>**/
	JC_HANDLE		vCustomDataPtr ;/**< Specifies the pointer to the data provided from custom application from the E_BOM_CUSTOM_OBJ_CTOR callback >**/
} ST_BOM_CUSTOM_CTOR ;

/**
 * @brief Handle E_BOM_CUSTOM_OBJ_DELETE custom callback.
 */
typedef struct st_bom_custom_delete
{
	JC_HANDLE		vObjHandle ; /**< Specifies the handle in BOM for the custom object (i.e. returned from jdi_BOMInitCustomObject) >**/
	JC_HANDLE		vCustomDataPtr ; /**< Specifies the pointer to the data provided from custom application from the E_BOM_CUSTOM_OBJ_CTOR callback >**/
} ST_BOM_CUSTOM_DELETE ;

typedef struct st_bom_custom_prop_list
{
	JC_INT8							*pcPropName ; /**< Specifies the name of the property that will be mentioned in the script >**/
	JC_HANDLE						vPropertyID ; /**< Specifies the handle to the property returned from BOM (from jdi_BOMInitCustomObject method)these ID values will be used in registering them >**/
} ST_BOM_CUSTOM_PROP_LIST ;
/**
 * @brief Specify object informatin required while initializing custom object.
 */
typedef struct st_bom_custom_object
{
	void					*pvCallbackArg ; /**< Specifies the Call back data supplied from the custom application. This data will be returned from BOM when callback will be invoked.>**/
	JC_INT8					*pcObjName ; /**< Specifies the name of the object by which the constructor function will be invoked (e.g. 'DOMParser' for new DOMParser() call). >**/
	JC_UINT32				uiPropCnt ; /**< Specifies the number of properties need to be registered with the object>**/
	ST_BOM_CUSTOM_PROP_LIST	*pstPropList ; /**< Specifies the list of properties need to be registered with the object. >**/
	CB_BOM_CUSTOM_CALLBACK	cbAppCallback ; /**< Specifies the pointer to the function which will handle all the callback types E_BOM_CUSTOM_CALLBACK >**/
} ST_BOM_CUSTOM_OBJECT ;

/**
 * @brief Specify the information required to register a property. 
 */
typedef struct st_bom_custom_property
{
	JC_HANDLE			vPropertyID ; /**< Specifies the ID value of the property i.e. returned from jdi_BOMInitCustomObject() with each property. >**/
	ST_BOM_VALUE		*pstPropVal ;
} ST_BOM_CUSTOM_PROPERTY ;

/**
 * @brief Specify list of methods to be registered with a custom object
 */
typedef struct st_bom_custom_method
{
	JC_INT8					*pcFunName ; /**< Specifies the name by which the method will be called from the script >**/
	JC_UINT16				uhArgCnt ; /**< Specifies the number of arguments that is possible in the function calls >**/
	CB_BOM_CUSTOM_METHOD	cbCustomMethod ; /**< Specifies the pointer to the functin that need to be called when this method will be invoked from the script >**/
} ST_BOM_CUSTOM_METHOD ;

#endif  /* BROWSER_CUSTOM_SCRIPT */

#ifdef __cplusplus
}
#endif

#endif
/* End of File */

