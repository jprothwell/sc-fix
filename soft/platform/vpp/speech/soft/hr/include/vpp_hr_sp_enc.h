#ifndef __SP_ENC
#define __SP_ENC

#include "vpp_hr_typedefs.h"

/*_________________________________________________________________________
 |                                                                         |
 |                           Function Prototypes                           |
 |_________________________________________________________________________|
*/

void   vpp_hr_encode(HAL_SPEECH_PCM_HALF_BUF_T EncInput, HAL_SPEECH_ENC_OUT_T* EncOutput);
//void   speechEncoder(INT16 pswSpeechIn[], INT16 pswFrmCodes[]);

//INT32 error_free (UINT16* EncoderIn, UINT16* EncoderOut);
//INT32 error_free_initial(HAL_SPEECH_ENC_OUT_T EdIfaceIn, HAL_SPEECH_DEC_IN_T* EdIfaceOutput);
//INT32 error_free_initial (UINT16* EncoderIn, UINT16* EncoderOut);
//INT32 error_free_interface (UINT16* EncoderIn, UINT16* EncoderOut);
#endif
