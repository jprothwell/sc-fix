#ifndef _APP_MMI_INPUTBOX_H_
#define _APP_MMI_INPUTBOX_H_

#include "wingtech_def.h"


#ifdef __cplusplus
    extern "C"
    {
#endif

#define     SINGLE_MODE     0
#define     MULTI_MODE      1
#define     MASK_MODE       2

#define INPUT_TRANSFORM_NONE			0
#define INPUT_TRANSFORM_CAPITALIZE		1  // not supported
#define INPUT_TRANSFORM_UPPERCASE		2
#define INPUT_TRANSFORM_LOWERCASE		3

//set the default MAX edit value
#define DEFAULT_EDIT_MAX_LEN       256
#define DEFAULT_MASK_EDIT_MAX_LEN  32
typedef struct _MMI_EDITBOX_PARAM
{    
    Int16           edit_mode;          // single line or multiline, mask
    Int16           max_len;            // indicates the max characters
    Int16*          default_str;        // default string
    Int16*          pm_format;           //indicate the format should be followed when user inupt 
    Boolean         is_editable;       // specify whether it is readonly.
    Boolean         is_notEmpty;       // indicate whether can be Empty or not
    Int16           input_transform;   // upper case, lower case
    void*           reserved;          // for extention, say input format, iput method
}MMI_EDITBOX_PARAM;

/** 
 * @brief This function is used to create input box in browser 
 * It is called by function jdd MMI, say function jddex_MMICreateInputBox.
 * @param[in] edit_param specify the edit parameters. 
 * Normally it is converted from JC_INPUT_BOX to BRW_EDITBOX_PARAM 
 * 
 * The function will return -1 in case of any error
 */
PUBLIC Int32 MMICreateEditBox(MMI_EDITBOX_PARAM* edit_param);
#ifdef __cplusplus
    }
#endif
#endif

