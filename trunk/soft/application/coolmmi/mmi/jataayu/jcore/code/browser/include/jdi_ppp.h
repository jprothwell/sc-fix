/***************************************************************************
 * $Id: jdi_ppp.h,v 1.45 2009/03/04 13:04:13 kavithadevhtmlbrow Exp $
 * $Revision: 1.45 $
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
 * $Log: jdi_ppp.h,v $
 * Revision 1.45  2009/03/04 13:04:13  kavithadevhtmlbrow
 * Doxygen Comments Updated
 *
 * Revision 1.44  2009/02/05 11:13:54  arpitadevhtmlbrow
 
 *
 * Revision 1.43  2008/07/23 04:32:01  kumardevhtmlbrow
 * Compilation Warning update
 *
 *  
 ***************************************************************************/

/**
 * @page savepagecode Sample Save Page Code
 * <table align="center" bgcolor="lightblue"> <tr> <td>
 * <PRE>
 * JC_RETCODE appSaveFetchtedPage (JC_HANDLE vBrowserHandle)
 * {
 *    JC_RETCODE                  rCode ;
 *    #ST_PPP_SAVEPAGE             stSavePage ;
 * 
 *    jc_memset (&stSavePage, 0x00, sizeof (#ST_PPP_SAVEPAGE)) ;
 *    stSavePage.pmFileName = pmFileName ;
 *    stSavePage.bUseSubFolders = E_TRUE ;
 *    stSavePage.eSaveEvent = #E_PPP_SAVEPAGE_NEW ;
 *    stSavePage.eSaveType = #E_SAVEPAGE_MULTIPART ;
 
 *    rCode = #jdi_DEExecuteReadPPP (vHandle,
 *                                  E_PPP_SAVEPAGE,
 *                                  &stSavePage) ;
 * @else
 *    rCode = #jdi_BEExecuteReadPPP (vBrowserHandle,
 *                                  E_PPP_SAVEPAGE,
 *                                  &stSavePage) ;
 * @endif
 *	
 *    return rCode ;
 * }
 * </PRE>
 * </td> </tr> </table>
 * @page findcode Sample Find Code
 * <table align="center" bgcolor="lightblue"> <tr> <td>
 * <PRE>
 * JC_RETCODE appFind (JC_HANDLE vBrowserHandle,
 *                     JC_CHAR   *pmKeyword)
 * {
 *    JC_RETCODE               rCode ;
 *    #ST_PPP_FIND              stFind ;
 *
 *    jc_memset (&stFind, 0x00, sizeof (#ST_PPP_FIND)) ;
 *    stFind.eType = #E_FIND_KEYWORD ;
 *    stFind.eOption = #E_FIND_CURRENT ;
 *
 *    stFind.uFindData.stFindInfo.pmWord = pmKeyword ;
 *    stFind.uFindData.stFindInfo.bMatchCase = E_TRUE ;
 *    stFind.uFindData.stFindInfo.bMatchWholeWord = E_TRUE ;
 
 *    rCode = #jdi_DEExecuteReadPPP (vHandle, 
 *                                  E_PPP_FIND,
 *                                  &stFind) ;
 * @else
 *    rCode = #jdi_BEExecuteReadPPP (vBrowserHandle, 
 *                                  E_PPP_FIND,
 *                                  &stFind) ;
 *	@endif
 *
 *    return rCode ;
 * }
 * </PRE>
 * </td> </tr> </table>
 */

/**
 * @file jdi_ppp.h
 * @ingroup Plugins
 * @brief Defines the exposed enumerations, structures and Callback prototypes
 *		  that can be used to use a PPP function.
 * 
 */

#ifndef JDI_PPP_H
#define JDI_PPP_H

/**
 * @brief The application needs to use this enum to specify the type of
 *		  read plugin operation to be executed by the browser.
 */
typedef enum pppReadPlugin
{
	E_PPP_FIND, /**< Find operation */
	E_PPP_SAVEPAGE, /**< Save page operation */ 
	E_PPP_GETITEMS /**< Get inline objects info Operation */
} E_PPP_READ_PLUGIN ;


/**
 * @brief The application needs to use this enum to specify the type of
 *		  write plugin operation to be executed by the browser.
 */
typedef enum pppReadWritePlugin
{
	E_PPP_FORM_DATA,			/**< Form Data operation */
	E_PPP_PASSWORD_MANAGER,		/**< Password Manager operation */	
	E_PPP_CUSTOMIZE,			/**< Customized page options */
	E_PPP_WIDGET				/**< Widget options */
} E_PPP_READWRITE_PLUGIN ;


/**
 * @brief The application needs to use this enum to specify the type
 *		  of save page operation to be performed.
 */
typedef enum pppSavepageType
{
	E_SAVEPAGE_MULTIPART, /**< Save page in multipart format */
	E_SAVEPAGE_COMPLETE, /**< Save the complete page */
	E_SAVEPAGE_PAGE_ONLY /**< Save the page only */
} E_PPP_SAVEPAGE_TYPE ;


/**
 * @brief The application needs to use this enum to specify the type of 
 *		  save page event
 */
typedef enum pppSavePageEvent
{
	E_PPP_SAVEPAGE_NEW, /**< Request for a New Save page operation */
	E_PPP_SAVEPAGE_CONTINUE, /**< Request for the continuation of Save page operation */
	E_PPP_SAVEPAGE_CANCEL, /**< Request for the cancellation of Save page operation */
	E_PPP_SAVEPAGE_SIZE /**< Request to know the total size of the memory taken by savepage operation. This can be used with all the savepage types.This size is approximate only and it may vary upto + or - 10 to 20% */
} E_PPP_SAVEPAGE_EVENT ;


/**
 * @brief The application needs to use this enum to specify the type of Search to be performed.
 */
typedef enum pppFindType
{
	E_FIND_KEYWORD, /**< Highlights the specified keyword */
	E_FIND_NUMBERS, /**< Highlights the numbers present in the page */
	E_FIND_TELNUMBERS, /**< Highlights the telephone numbers, allows the presence of + sign in the beginning or a - sign within. Search is based upon the minimum number of digits allowed in a telephone number, which is a configurable parameter */
	E_FIND_CURRENCY, /**< Highlights the numbers containing a . sign within */
	E_FIND_EMAILIDS, /**< Highlights the Email IDs present in the page */
	E_FIND_URLS, /**< Highlights the URLS present in the page */
	E_FIND_HIGHLIGHTSCROLL, /**< Highlights the occurances corresponding to the shift in the visible area */
	E_FIND_CANCEL, /**< To specify that further search has been cancelled by the user and to highlight the last searched result */
	E_FIND_CLEAR, /**< Finds and clears all the highlighted items */
	E_FIND_GETVAL, /**< Used to retrieve the value of the current highlighted value */
	E_FIND_CONFIG /**< Used to update the configurable find application related data in the find context */
} E_PPP_FIND_TYPE ;

/**
 * @brief The application needs to use this enum to specify the position
 *		  from which the find operation should start. These options are available for the find keyword
 *		  type alone.
 */
typedef enum pppFindOption
{
	E_FIND_CURRENT, /**< From the current cursor position */
	E_FIND_START, /**< From the start of the page */
	E_FIND_HIGHLIGHT /**< Highlights all the occurances */
} E_PPP_FIND_OPTION ;


/**
 * @brief Specifies the events processed by customize PPP
 */
typedef enum pppCutomizeEvent
{
	E_PPP_CUSTOMIZE_EVENT_SETOPTION, /**< Sets the customize option(enable/disable) - Called by applicaiton */
	E_PPP_CUSTOMIZE_EVENT_APPLY /**< Apply the enabled customization - called by DE **/
} E_PPP_CUSTOMIZE_EVENT ;

/**
 * @brief Specifies the PPP Widgets soft keys 
 */
typedef enum pppWidgetSoftkey 
{
	E_PPP_WIDGET_LEFT,		/** Specifies the left soft key */
	E_PPP_WIDGET_RIGHT,		/** Specifies the right soft key */
	E_PPP_WIDGET_CENTER		/** Specifies the center soft key */
} E_PPP_WIDGET_SOFTKEY ;

/**
 *  @brief Specifies the PPP Widget event
 */
typedef enum pppWidgetEvent
{
	E_PPP_WIDGET_INIT,			/** Application calls to initialize the widget with Filename of widget information */
	E_PPP_WIDGET_ACTIVE,		/** Application calls to get the current active widget name */	
	E_PPP_WIDGET_MENU_LIST,		/** Application calls to get the list of menu to displayed for the initialized widget */
	E_PPP_WIDGET_MENU_ACTION,   /** Application calls to execute the action associated with the specific menu item */
	E_PPP_WIDGET_APPLY,			/** DE Calls to apply the script/style of the initialized widget for the page URI passed */
	E_PPP_WIDGET_REQUEST,		/** DE Calls when any bwp: scheme is occured in fetch request */
	E_PPP_WIDGET_INLINE_REQUEST,/** DE calls when bwp: scheme is occured for an inline fetch request */
	E_PPP_WIDGET_TIMER,			/** DE Calls when timer expired or to be suspened or stopped */
	E_PPP_WIDGET_MENU_OBJECT,	/** DE Calls when there is an change for the menu & menu item from the script. */
	E_PPP_WIDGET_DEINIT			/** Application calls to de-initialize the widget */
} E_PPP_WIDGET_EVENT ;

/**
 * @brief The application needs to use this enum to specify the type of action 
 *		  they need to do for getting the object
 */
typedef enum pppGetitenEvent
{
	E_GETITEM_FIRST, /**< To get the first object from the fetched page, this restarts the process of getting the info*/
	E_GETITEM_NEXT, /**< To get the next object */
	E_GETITEM_CANCEL /**< Cancels the process */
} E_PPP_GETITEM_EVENT ;

/**
 * @brief The application needs to use this enum to specify the type of action
 *		  need to be taken on the form data.
 */
typedef enum pppFormDataEvent
{
	E_INTIALIZE_FORM_DATA, /**< Sets the File name for the Form Data Storage*/
	E_GET_FORM_DATA,	/**< To get the form data values for an input name*/
	E_STORE_FORM_DATA,	/**< To store the form data value for an input name*/
	E_CONFIG_FORM_DATA,	/**< To enable or disable form data storage */
	E_CLEAR_ALL_FORM_DATA,  /**< Deletes a value or all values for an input Name*/
	E_FREE_FORM_DATA /**< To free the specific form data value, which is retrieved using E_GET_FORM_DATA. */
} E_PPP_FORM_DATA_EVENT ;

/**
 * @brief Specifies the various event in handling password manager
 */
typedef enum pppPwdMgrEvent
{
	E_PWD_MGR_INIT,
	E_PWD_MGR_CONFIG,
	E_PWD_MGR_SET,
	E_PWD_MGR_CONFIRM,
	E_PWD_MGR_GET,
	E_PWD_MGR_LIST,
	E_PWD_MGR_REMOVE,
	E_PWD_MGR_REMOVE_ALL	
} E_PPP_PWD_MGR_EVENT ;

/**
 * brief Specifies the variouse confirm option 
 */
typedef enum pppPwdMgrConfirm
{
	E_PWD_MGR_CONFIRM_OK,
	E_PWD_MGR_CONFIRM_CANCEL,
	E_PWN_MGR_CONFIRM_NEVER
} E_PPP_PWD_MGR_CONFIRM ;

/**
 * @brief The application needs to use this structure to specify the information 
 *		  required for a save page operation.
 */
typedef struct pppSavePage
{
	E_PPP_SAVEPAGE_TYPE		eSaveType ; /**< Specifies the type of save page operation */
	E_PPP_SAVEPAGE_EVENT	eSaveEvent ; /**< Specifies the type of save page event */
	JC_CHAR					*pmFileName ; /**< Specifies the file in which the page has to be saved */
	JC_BOOLEAN				bUseSubFolders ; /**< Specifies whether sub folders can be created while save page operation is performed */
	JC_UINT32				uiSavepageSize ; /**< Holds the size of the memory taken by savepage operation in bytes, this is an output parameter, used with the savepage event E_PPP_SAVEPAGE_SIZE. This size is approximate only and it may vary upto + or - 10 to 20% */
} ST_PPP_SAVEPAGE ;


/**
 * @brief The application needs to use this structure to specify the information 
 *		  required for a find operation.
 */
typedef struct pppFindInfo
{
	JC_CHAR				*pmWord ; /**< Specifies the word to be found. */
	JC_BOOLEAN			bMatchCase ; /**< Specifies whether the case has to be matched */
	JC_BOOLEAN			bMatchWholeWord ; /**< Specifies whether the whole word has to be matched */
	JC_BOOLEAN			bFindUp ; /**< Specifies the direction in which the find operation has to be done */
	JC_BOOLEAN			bStartSearch ; /**< Specifies whether the search is the initial search or consecutive search requests */
} ST_PPP_FIND_INFO ;

/**
 * @brief Specifies the configuration data for find.
 */
typedef struct pppFindConfig
{
	JC_UINT32		uiMinNumLen ; /**< Specifies the mimimum number of digits to be searched for while finding numbers */
	JC_UINT32		uiMinTelNumLen ; /**< Specifies the minimum number of telephone digits */
	JC_INT8			**ppcTelSpecChars ; /**< Specifies the special characters allowed in a telephone number */
	JC_INT8			**ppcWebIdentifiers ; /**< Specifies the web identifiers to be searched for in a string, while searching for URLs */
} ST_PPP_FIND_CONFIG ;

/**
 * @brief Specifies the union of the search related information and the find
 *		  configuration data.
 */
typedef union pppFindData
{
	ST_PPP_FIND_INFO	stFindInfo ; /**< Specifies the information required for the find operation */
	ST_PPP_FIND_CONFIG	stFindConfig ; /**< Specifies the configuration related information for find */
} U_PPP_FIND_DATA ;

/**
 * @brief The application needs to use this structure to specify the type of find
 *		  operation and the information related to the find operation.
 */
typedef struct pppFind
{
	E_PPP_FIND_TYPE			eType ; /**< Specifies the type of find operation */
	E_PPP_FIND_OPTION		eOption ; /**< Specifies one of the find options giving the search criterion */ 
	U_PPP_FIND_DATA			uFindData ; /**< Specifies either the search related information, or, specifies the configuration information for find */
} ST_PPP_FIND ;

/**
 * Specifies the customize set option data
 */
typedef struct pppCustomizeSetOption
{
	JC_BOOLEAN				bEnable ;    /**< Specifies whether the selected option needs to be enabled or disabled */
	JC_INT8					*pcPattern ; /**< Specifies pattern for domain match */
	JC_INT8					*pcStyle ;   /**< Specifies the style to be used for this pattern matching */
	JC_INT8					*pcScript ;	 /**< Specifies the script to be added for this pattern matching */
} ST_PPP_CUSTOMIZE_SET_OPTION ;

typedef union pppCustomizeEventData
{
	ST_PPP_CUSTOMIZE_SET_OPTION		stOption ;
	JC_CHAR							*pmPageURI ;
} U_PPP_CUSTOMIZE_EVENT_DATA ;

/**
 * @brief The application needs to use this structure to specify the option for which the customization has been
 *		  selected by the user.
 */
typedef struct pppCustomize
{
	E_PPP_CUSTOMIZE_EVENT		eEvent ;  /**< Specifies the event type for customize */
	U_PPP_CUSTOMIZE_EVENT_DATA	uData ;   /**< Specifies the data associated with the event */
} ST_PPP_CUSTOMIZE ;

/** 
 * @brief Specifies the source of the menu. 
 */
typedef enum ePPPWidgetMenuSource
{
	E_PPP_WIDGET_STATIC_MENU, /**< Specifies the menu is from the XML format. */
	E_PPP_WIDGET_DYNAMIC_MENU /**< Specifies the menu is from the JavaScript. */
} E_PPP_WIDGET_MENU_SOURCE ;

/**
 *  @brief Specifies the menu item data for PPP Widget
 */
typedef struct pppWidgetMenuItem
{
	JC_CHAR						*pmCaption ;	/**< Specifies the caption of the menu item */
	JC_UINT32					uiMenuID ;		/**< Specifies the menu id. */
	JC_BOOLEAN					bHide ;			/**< Specifies whether the menu needs to be hidden. */
	E_PPP_WIDGET_MENU_SOURCE	eMenuSource ;	/**< Specifies the source of the menu. 
													This is just a reference to the application and used by the browser core. */
	JC_HANDLE					vMenuHandle ;   /**< Specifies the menu handle */
	struct pppWidgetMenuItem	*pstNext ;      /**< Reference to the next menu item */
	struct pppWidgetMenuItem	*pstChild ;      /**< Reference to the child menu item */
} ST_PPP_WIDGET_MENUITEM ;

/**
 * @brief Specifies the menu data for softkeys used by PPP Widget 
 */
typedef struct pppWidgetMenu
{
	E_PPP_WIDGET_SOFTKEY		eSoftKey ;		/**< Specifies the soft key of menu */
	JC_CHAR						*pmCaption ;	/**< Specifies the caption of the menu */
	E_PPP_WIDGET_MENU_SOURCE	eMenuSource ;	/**< Specifies the source of the menu. 
													This is just a reference to the application and used by the browser core. */
	JC_HANDLE					vMenuHandle ;	/**< Specifies the menu handle */
	ST_PPP_WIDGET_MENUITEM		*pstMenuItem ;  /**< Specifies the menu item list */
	struct pppWidgetMenu		*pstNext ;		/**< Reference to the next menu */
} ST_PPP_WIDGET_MENU ;

/**
 * @brief Specifies the different action for the menu object.
 */
typedef enum eMenuObjectAction 
{
	E_WIDGET_MENU_RIGHT_UPDATE, /**< Specifies the right softkey label update. */
	E_WIDGET_MENU_APPEND,		/**< Specifies the menu addition in the left sokt key. */
	E_WIDGET_MENU_REMOVE,		/**< Specifies the menu removal in the left soft key. */
	E_WIDGET_MENU_REMOVE_ALL,		/**< Specifies the menu removal in the left soft key. */
	E_WIDGET_MENU_DIMMED,		/**< Specifies the menu visiblity. */
	E_WIDGET_MENU_GET_BY_ID,	/**< Specifies the menu get item by ID. */
	E_WIDGET_MENU_GET_BY_NAME	/**< Specifies the menu get item by name. */
} E_WIDGET_MENU_OBJECT_ACTION ;

/**
 * @brief Specifies the menu object information for updating from the JavaScript.
 */
typedef struct pppWidgetMenuObject
{
	E_WIDGET_MENU_OBJECT_ACTION		eAction ;				/**< Specifies the menu object action. */
	JC_UINT32						uiMenuID ;				/**< Specifies the menu identifier. */
	JC_CHAR							*pmLabel ;				/**< Specifies the label for the menu. This can be NULL if the action is remove or dimmed. */
	void							*pvMenuHandle ;			/**< Specifies the handle for the menu. */
	void							*pvParentMenuHandle ;	/**< Specifies the parent handle, if any. */
} ST_PPP_WIDGET_MENU_OBJECT ;

/**
 * @brief Specifies the Widget request info
 */
typedef enum pppWidgetTimerEvent
{
	E_PPP_WIDGET_TIMER_EXPIRED,		/**< Indicates timer expired status */
	E_PPP_WIDGET_TIMER_SUSPEND,     /**< Indicates to suspend the timer */ 
	E_PPP_WIDGET_TIMER_RESUME,		/**< Indicates to resume the timer */
	E_PPP_WIDGET_TIMER_STOP			/**< Indicates to stop the timer */
} E_PPP_WIDGET_TIMER_EVENT ;

typedef struct pppWidgetTimerData
{
	E_PPP_WIDGET_TIMER_EVENT	eTimerEvent ;  /**< Specifies the timer event */
	JC_UINT32					uiTimerID ;	   /**< Specifies the timer ID - Expired Event */
} ST_PPP_WIDGET_TIMER_DATA ;


typedef union u_ppp_page_buffer
{
	JC_UINT8		*pucBuffer ; /**< Specifies the buffer content */
	JC_CHAR			*pmFileName ; /**< Specifies the file where the buffer is stored */
} U_PPP_PAGE_BUFFER ;

/**
 * @brief Specifies information about the content received from the lower layers
 *		  for the request given from the display engine.
 */
typedef struct pppWidgetInlineReqCont
{
	JC_INT8				*pcContentType ;/**< Specifies the content type of the content received form the fetch request */
	void				*pvHeaders ;	/**< Specifies the headers of the content received form the fetch request */
	JC_UINT32			uiBufLen ;		/**< Specifies the total buffer's (content) length received (stored only if known) */
	JC_BOOLEAN			bUseMemory ;	/**< Specifies whether memory has to be allocated or use the previously used memory */
	JC_BOOLEAN			bFileContent ; /**< Specifies whether the source of the content. */
	U_PPP_PAGE_BUFFER	uContent ; /**< Specifies the widget content. */
} ST_PPP_WIDGET_INLINE_REQ_CONT ;

/**
 * @brief Specifies the union of widget event data
 */
typedef union pppWidgetEventData
{
	JC_CHAR							*pmFileName ;	/**< Specifies the file name of the Widget - In parameter for Init event */
	JC_CHAR							*pmPageURI ;	/**< Specifies the Page URI - In parameter for Apply Event Event */
	JC_CHAR							*pmActiveWidget ; /**< Specifies the active widget name - Out parameter for Active event */
	JC_HANDLE						vMenuHandle ;	/**< Specifies the menu handle - In parameter for Menu Action Event*/
	ST_PPP_WIDGET_TIMER_DATA		stTimerData ;	/**< Specifies the timer data - In parameter for Timer event */
	ST_PPP_WIDGET_MENU				*pstMenu ;		/**< Specifies the menu list - out parameter for MENU_LIST event */
	ST_PPP_WIDGET_MENU_OBJECT		stMenuObject ; /**< Specifies the menu object. */
	ST_PPP_WIDGET_INLINE_REQ_CONT	stResponse ;	/**< Specifies the reponse for inline request - out parameter for event E_PPP_WIDGET_INLINE_REQUEST */
} U_PPP_WIDGET_EVENT_DATA ;

/**
 * @brief Specifies the argument for Widget PPP
 */
typedef struct pppWidget
{
	E_PPP_WIDGET_EVENT			eEvent ;
	U_PPP_WIDGET_EVENT_DATA		uData ;
} ST_PPP_WIDGET ;

/**
 * @brief The application needs to use this structure to give the event type 
 *		  to get the items.
 */
typedef struct pppGetItems
{
	E_PPP_GETITEM_EVENT		eEvent ;	/**< Type of event given from the application*/
	JC_CHAR					*pmURI ;	/**< The image/object url given back to the application */
	JC_OBJECT_INFO			stObjectInfo ; /**< The image/object info given back to the application based on the event */
} ST_PPP_GETITEMS ;


/**
 * @brief Application needs to fill this structure with the input name and value and 
 *        pass it as argument to the pppExecuteFormData function in order to store the 
 *	      value corresponding to the input name or to delete the value corresponding to the
 *        name from the form data.The two cases can be distinguished by the 
 *        E_PPP_FORM_DATA_EVENT enum value which also needs to be passed to the execute
 *        function.
 */
typedef struct pppFormDataPair
{
	JC_CHAR		*pmInputName ;		/*Input name for which the operation is to be performed*/
	JC_CHAR		*pmInputValue ;		/*Value corresponding to the input name which needs to be stored or deleted based on operation*/
} ST_PPP_FORM_DATA_PAIR ;			

/**
 * @brief	The application needs to pass this structure to the form data module for a 
 *			get data operation on the form data available.The form data module will fill
 *			the linked list of ST_FORM_DATA_LIST structure for all the value available
 *			for the input name.If the input name is given as NULL then the value for the 
 *			current focused control will be returned.The caller should not try to free
 *			the value link list that is returned for this call.That will be done automatically
 *			for the next call to a get data function or on de-initialisation.
 */
typedef struct pppFormDataGet
{
	JC_CHAR				*pmInputName ;	/*Input name for which application wants the values*/
	JC_INPUT_BOX_VALUE	*pstValueList ; /*Linked list of associated values.*/	
} ST_PPP_FORM_DATA_GET ;

typedef struct pppFormDataClear
{
	JC_BOOLEAN	bDeleteAllValues ;	/* Specifies whether all the values pertaining to the input control need to be cleared or not */
	JC_CHAR		*pmInputName ;		/*Input name for which the operation is to be performed*/
	JC_CHAR		*pmInputValue ;		/*Value corresponding to the input name which need to be deleted. NULL in case of Clearing all the values*/
} ST_PPP_FORM_DATA_CLEAR ;

/**
 * @brief	Union of different arguments to the execute function for the form data PPP.
 */
typedef union pppFormData
{
	JC_BOOLEAN				bFormDataEnabled ;	/*Boolean to configure the form data to ENABLE/DISABLE*/
	JC_CHAR					*pmFilename ;		/*Name of the file which is used for the form data storage*/
	ST_PPP_FORM_DATA_PAIR	stFormDataPair ;	/*Structure used for storing and deleting operations*/
	ST_PPP_FORM_DATA_GET	stFormDataGet ;		/*Structure used for a get operation*/
	ST_PPP_FORM_DATA_CLEAR	stFormDataClear ;	/*Structure used for specifying the control values that need to be cleared. In parameter for E_CLEAR_ALL_FORM_DATA event */
	JC_INPUT_BOX_VALUE		*pstValueList ;		/* In parameter for E_FREE_FORM_DATA event. Specifies the value list that needs to be freed */
} U_PPP_FORM_DATA ;

/**
 * @brief	Argument to the execute function
 */
typedef struct pppFormDataArg
{
	E_PPP_FORM_DATA_EVENT	eFormDataEvent ;	/*Operation that is to be performed*/
	U_PPP_FORM_DATA			uFormData ;			/*Union of possible arguments*/
} ST_FORM_DATA_ARG ;

/**
 * @brief Uses as input/output event info for processing SET/GET/REMOVE events
 */
typedef struct pppPwdMgrData
{
	JC_CHAR			*pmURI ;		 /** Used as input for GET/REMOVE events */
	JC_CHAR			*pmUserField ;	 /** Used as input for SET and output for GET */
	JC_CHAR			*pmUserValue ;   /** Used as input for SET and output for GET */
	JC_CHAR			*pmPasswdField ; /** Used as input for SET and output for GET */
	JC_CHAR			*pmPasswdValue ; /** Used as input for SET and output for GET */
	JC_BOOLEAN		bAllow ;		 /** Used as input for SET and output for GET */
} ST_PPP_PWD_MGR_DATA ;

/**
 * @brief Used as the output event info for processing E_PPP_PWD_MGR_LIST event
 */
typedef struct pppPwdMgrList
{
	ST_PPP_PWD_MGR_DATA		stData ;
	struct pppPwdMgrList	*pstNext ;
} ST_PPP_PWD_MGR_LIST ;

/**
 * @brief Used to pass the initialization parameters
 */
typedef struct pppPwdMgrInit
{
	JC_CHAR					*pmFilename ;
	JC_UINT32				uiMaxEntries ;
} ST_PPP_PWD_MGR_INIT ;

/**
 * @brief Specifies the Password Manager event information
 */
typedef union pppPwdMgrEventInfo
{
	JC_BOOLEAN				bEnable ;
	ST_PPP_PWD_MGR_INIT		*pstInit ;
	ST_PPP_PWD_MGR_DATA		stData ;
	ST_PPP_PWD_MGR_LIST		*pstList ;
	E_PPP_PWD_MGR_CONFIRM	eConfirm ;
} U_PPP_PWD_MGR_EVENT_INFO ;

/**
 * @brief Argument to execute Password manager PPP 
 */
typedef struct pppPwdMgrArg
{
	E_PPP_PWD_MGR_EVENT			eEvent ;
	U_PPP_PWD_MGR_EVENT_INFO	uEventInfo ;
} ST_PPP_PWD_MGR_ARG ;

/**
 * @brief Specifies the PPP Pwd mgr - application request type
 */
typedef struct pppPwdMgrRequest
{
	JC_BOOLEAN				bIsOverride ;
	E_PPP_PWD_MGR_CONFIRM	eConfirm ;
} ST_PPP_PWD_MGR_REQUEST ;

/**
 * @brief Specifies the PPP - Application request data
 */
typedef union pppReadWriteRequestData
{
	ST_PPP_PWD_MGR_REQUEST	stPwdMgrReq ;
} U_PPP_READWRITE_REQUEST_DATA ;


/**
 * @brief Specifies the PPP Read Write Request info
 */
typedef struct pppReadWriteRequest
{
	E_PPP_READWRITE_PLUGIN			ePluginType ;
	U_PPP_READWRITE_REQUEST_DATA	uData ;
} ST_PPP_READWRITE_REQUEST ;


/**
 * @brief	    Prototype of PPP Plugin initialize callback.
 *
 * @param[in]	vHandle Specifies the handle to the plug in function.
 * @param[in]	pvPPPContext Once the plug in function is intiailized the callback
 *				function needs to return its context in this parameter.
 * @retval		JC_OK if the plug in is initialized successfully.
 *
 * The browser will call this callback to initialize a PPP function. The callback
 * needs to initialize the PPP and return the context of PPP in pvPPPContext. The 
 * browser will use this contenxt while calling the PPP.
 *
 * For each PPP it has to follow the steps of initialization the plugin, Use of 
 * plugin and the deinitialize the plugin. This callback will be called to initialize
 * the various plugins (read or read write plugin).
 */
typedef JC_RETCODE (* CB_PPP_INITIALIZE) (JC_HANDLE			vHandle,
										  JC_HANDLE			*pvPPPContext) ;

/**
 * @brief	    Prototype of PPP Read Plugin callback.
 *
 * @param[in]	vHandle Specifies the handle to the plug in function.
 * @param[in]	pvPPPContext Specifies the PPP context which was returned when the
 *				plug in was initialized.
 * @param[in]	vPPPArg Specifies the arguement to the read plug in callback. 
 * @retval 		JC_OK if the read plug in is executed successfully.
 *
 * The browser will call this callback to execute the read plugin function which 
 * was initialized. The information needed for the plug in is specified
 * in the vPPPArg.
 */
typedef JC_RETCODE (* CB_PPP_READ_PLUGIN) (JC_HANDLE			vHandle,
										   JC_HANDLE			vPPPContext,
										   JC_HANDLE			vPPPArg) ;

/**
 * @brief	    Prototype of PPP Read write Plug-in callback.
 *
 * @param[in]	vHandle Specifies the handle to the plug in function.
 * @param[in]	pvPPPContext Specifies the PPP context which was returned when the
 *				plug in was initialized.
 * @param[in]	vPPPArg Specifies the argument to the read write plug in callback. 
 * @retval 		JC_OK if the read write plug in is executed successfully.
 *
 * The browser will call this callback to execute the read write plug-in function which 
 * was initialized. The information needed for the plug in is specified
 * in the vPPPArg.
 */
typedef JC_RETCODE (* CB_PPP_READWRITE_PLUGIN) (JC_HANDLE			vHandle,
												JC_HANDLE			vPPPContext,
												JC_HANDLE			vPPPArg) ;

/**
 * @brief	    Prototype of PPP Plugin deinitialize callback.
 *
 * @param[in]	pvPPPContext Specifies the context of the PPP function which 
 *				is to be deinitialized.
 * @retval		JC_OK if the plug in is deinitialized successfully.
 *
 * The browser will call this callback to deinitialize a PPP function. 
 */
typedef JC_RETCODE (* CB_PPP_DEINITIALIZE) (JC_HANDLE		vPPPContext) ;

/**
 * @brief This error is returned when a keyword, based uopn the specified search criteria,
 *		  is not found in the given page. Also, this error code would be returned when no
 *		  further occurance of the searched keyword is found, that is, when end of file
 *		  is reached while searching for the next occurance.
 */
#define JC_ERR_PPP_FIND_NOT_FOUND			TERMINAL_ERR_CODE(DE_MODULE, 0xFE)

/**
 * @brief The error is returned when the the save page operation exceeds the configured 
 *		  timeout value or when frame content has to be saved. In this case application 
 *		  should call the save page api again after some time.
 */
#define JC_ERR_PPP_SAVEPAGE_CONTINUE		TERMINAL_ERR_CODE(DE_MODULE, 0xFF)

/**
 * @brief This error is returned when there are no inline image/object to be listed 
 *		  for the first option given by the application at the start.
 */
#define JC_ERR_PPP_OBJECT_NOT_FOUND			TERMINAL_ERR_CODE(DE_MODULE, 0xFD)

/**
 * @brief This error is returned when the last image/object is reached and there 
 *		  are no more inline object to be listed for the next get request given by the application.
 */
#define JC_ERR_PPP_NEXT_OBJECT_NOT_FOUND		TERMINAL_ERR_CODE(DE_MODULE, 0xFC)

/**
 * @brief This error is returned when the configuration data has not been set by the
 *		  application before carrying out a search operation that requires the config
 *		  values.
 */
#define JC_ERR_PPP_FIND_CONFIG_NOT_SET			TERMINAL_ERR_CODE(DE_MODULE, 0xFB)

/**
 * @brief This error code is returned if the application tries to access any form data
 *		  functionality without initializing it, i.e., setting the form-data file.
 */
#define JC_ERR_PPP_FORM_DATA_NOT_INITIALIZED	TERMINAL_ERR_CODE(DE_MODULE, 0xFA)

/**
 * @brief This error code is returned if the application tries to access any form data
 *		  functionality without enabling it.
 */
#define JC_ERR_PPP_FORM_DATA_NOT_ENABLED		TERMINAL_ERR_CODE(DE_MODULE, 0xF9)

/**
 * @brief This error code is returned if the application tries to access any Password 
 *		  manager functionality after initializing it.
 */
#define JC_ERR_PPP_PWD_MGR_ALREADY_INITIALIZED	TERMINAL_ERR_CODE(DE_MODULE, 0xF8)

/**
 * @brief This error code is returned if the application tries to access any Password 
 *		  manager functionality without initializing it.
 */
#define JC_ERR_PPP_PWD_MGR_NOT_INITIALIZED	TERMINAL_ERR_CODE(DE_MODULE, 0xF7)

/**
 * @brief This error code is returned on any get/set/remove/list event and 
 *        password manager is not enabled
 */
#define JC_ERR_PPP_PWD_MGR_NOT_ENABLED			TERMINAL_ERR_CODE(DE_MODULE, 0xF6)
	
/**
 * @brief This error code is returned when data is corrupted while accessing password manager file
 */
#define JC_ERR_PPP_PWD_MGR_DATA_CORRUPTED		TERMINAL_ERR_CODE(DE_MODULE, 0xF5)

/**
 * @brief This error code is returned when one widget is already active and init is called for the same
 *        or another widget
 */
#define JC_ERR_PPP_WIDGET_ALREADY_ACTIVE		TERMINAL_ERR_CODE(DE_MODULE, 0xF4)
/**
 * @brief This error code is returned when the widget XML format is not valid.
 */
#define JC_ERR_PPP_WIDGET_INVALID_FORMAT		TERMINAL_ERR_CODE(DE_MODULE, 0xF3)
/**
 * @brief This error code is returned when widget has to be exited, application should call de-init 
 *        of widget on receiving this error code. This error code will be returned when processing 
 *		  the special scheme bwp:exit 
 */
#define JC_ERR_PPP_WIDGET_EXIT_ACTION			TERMINAL_ERR_CODE(DE_MODULE, 0xF2)

/**
 * @brief This error code is returned on when the count of the existing Password Manager entries exceeds
 *		  the maximum count for entries.
 */
#define JC_ERR_PPP_PWD_MGR_MAX_ENTRIES			TERMINAL_ERR_CODE(DE_MODULE, 0xF1)

#endif


