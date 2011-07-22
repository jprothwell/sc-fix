/***************************************************************************
 *
 *  FILE NAME:  vpp_encoder.c
 *
 *              Vpp EFR Encoder Function
 *
 ***************************************************************************/


#include "string.h"

#include "vpp_efr_sig_proc.h"
#include "vpp_efr_codec.h"
#include "vpp_efr_cnst.h"
#include "vpp_efr_e_homing.h"

#include "vpp_efr_dtx.h"

#include "vpp_efr_debug.h"
#include "vpp_efr_profile_codes.h"


INT16 dtx_mode;
extern INT16 txdtx_ctrl;
extern INT16 *new_speech;  /* Pointer to new speech data            */


void vpp_efr_encode (HAL_SPEECH_PCM_HALF_BUF_T EncIn, HAL_SPEECH_ENC_OUT_T* EncOut)
{
    INT16 prm[PRM_SIZE];       /* Analysis parameters.                  */
    INT16 syn[L_FRAME];        /* Buffer for synthesis speech           */
    INT16 serial [SERIAL_SIZE -1];

    INT16 reset_flag;
    INT16 i;

    VPP_EFR_PROFILE_FUNCTION_ENTER (vpp_efr_encode);

    memcpy((INT8*) new_speech, (INT8*)EncIn, L_FRAME*2);

    /* Check whether this frame is an encoder homing frame */
    reset_flag = encoder_homing_frame_test (new_speech);


    for (i = 0; i < L_FRAME; i++)   /* Delete the 3 LSBs (13-bit input) */
    {
        new_speech[i] = new_speech[i] & 0xfff8;
    }

     Pre_Process (new_speech, L_FRAME);           /* filter + downscaling */

     Coder_12k2 (prm, syn);  /* Find speech parameters   */

     if ((txdtx_ctrl & TX_SP_FLAG) == 0)
     {
         /* Write comfort noise parameters into the parameter frame.
         Use old parameters in case SID frame is not to be updated */
         CN_encoding (prm, txdtx_ctrl);
     }
     Prm2bits_12k2 (prm, &serial[0]); /* Parameters to serial bits */

    if ((txdtx_ctrl & TX_SP_FLAG) == 0)
    {
        /* Insert SID codeword into the serial parameter frame */
        sid_codeword_encoding (&serial[0]);
    }

    for(i=0; i<(SERIAL_SIZE -1); i++)
    {
          EncOut->encOutBuf [i/16] |= (serial[i] << (i%16));
    }

    /* Write the VAD- and SP-flags to file after the speech parameter bit stream */
    EncOut->echoSkipEncFrame = (txdtx_ctrl & TX_VAD_FLAG)? 1 : 0;
    EncOut->sp = (txdtx_ctrl & TX_SP_FLAG)? 1 : 0;

    if (reset_flag)
    {
          reset_enc (); /*Bring the encoder, VAD and DTX to the home state */
    }

    VPP_EFR_PROFILE_FUNCTION_EXIT (vpp_efr_encode);
}
