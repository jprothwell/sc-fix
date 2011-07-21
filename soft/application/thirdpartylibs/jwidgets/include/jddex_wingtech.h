/***************************************************************************
*
* File Name : jddex_wingetch.h
*
***************************************************************************
*
*
* File Description
* ----------------
*
* Purpose			:	declarations for wingtech specifc handling
*						
* Created By		:
* Created Date		:
*
*
*
* Current Revision	:
*
***************************************************************************
*
*
* Revision Details
* ----------------
*
* 1. Modified By	:
*    Modified Date	:
*	 Purpose		:
*
*
*
*
***************************************************************************/
#ifndef _JDDEX_WINGTECH_
#define _JDDEX_WINGTECH_

#ifdef __cplusplus
    extern   "C"
    {
#endif

#include <ddl.h>
#include <jcal.h>
/* wingtech secondary - primary interaction events */
/* TBD replace 100 with E_JDDEX_KEY_END*/
typedef enum e_wingtech_SPIEvents
{
	E_WINGTECH_INPUT_DONE = 0x100 ,//E_JDDEX_KEY_END, /* event data JDDEX_INPUTDATA */
    E_WINGTECH_INPUT_CANCEL,
    E_WINGTECH_SECMMI_START,/* no event data */
    E_WINGTECH_SECMMI_END,
    E_WINGTECH_DRAW_FRAME
}EwingtechSPIEvents, E_WINGTECH_SPI_EVENTS ;


/**
 * @brief This structure contains information about the event that needs to be sent.
 * 
 */
typedef struct st_jc_inputdata
{
	JC_CHAR						*pszValue ;/* to be allocated by the sender  
								and to be freed by sender after function is returned */
} JDDEX_INPUTDATA ;






// API to create list box

/* memory is allocated from heap and passed to this function. wingtech can choose to resuse
same memory( instead of allocating a new copy )and upadte the same memory.
*/
//JC_RETCODE jddex_PlatformCreateListBox(JDDEX_LIST_BOX*  pstListBox);



/*
call back functions to be called beforing returning to primary MMI . refer to input box implimentation

JDDEX_LIST_ITEM	  **pstListItems;

jddex_WingtechHandleEvent(JC_NULL, E_WINGTECH_LISTBOX_DONE, (void*)pstListItems);
jddex_WingtechHandleEvent(JC_NULL, E_WINGTECH_LISTBOX_CANCEL, NULL);

*/

JC_RETCODE jddex_MapWingetchEvents(JC_UINT32 uiInputEventType,JC_UINT32 *puiMappedEventType);

JC_RETCODE jddex_WingtechHandleEvent(JC_HANDLE	vHandle,
								   JC_UINT32 uiEventType, void	*pvEventInfo);


JC_INT32 jddex_GetDeviceRenderWidth();
JC_INT32 jddex_GetDeviceRenderHeight();

/**
 * @brief 		gives the status of secondary MMI. 
 * @param[out]	TRUE/FALSE
 * @retval		JC_OK
 * checks if the secondar is in focus or out of focus
 */
JC_RETCODE jddex_IsSecondaryMMIUp(JC_BOOLEAN *isSecMMIUp);

#ifdef __cplusplus
    }
#endif


#endif // _JDDEX_WINGTECH_
