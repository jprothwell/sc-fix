/*************************************************************************/
/*                                                                       */
/*              Copyright (C) 2006, Coolsand Technologies, Inc.          */
/*                            All Rights Reserved                        */
/*                                                                       */
/*************************************************************************/
/// 
///   @file vpp_AudioJpegEnc_enc.h
///   This file defines VPP AUDIO_ENC API structures and functions.
/// 

#ifndef vpp_PREVIEW_H
#define vpp_PREVIEW_H

#include "cs_types.h"
#include "hal_voc.h"
#include "hal_error.h"

typedef struct
{
	INT32*    inStreamBufAddr;         //Input stream buffer address
	INT32*    outStreamBufAddr;        //Output stream buffer address
	INT32*    blendBufAddr;
       INT16  src_w;
	INT16  src_h;
	INT16  cut_w;
	INT16  cut_h;
	INT16  dec_w;
	INT16  dec_h;
	INT16  reset;
	INT16  rotate;
}CutZoomInStruct;

HAL_ERR_T vpp_PreviewOpen(HAL_VOC_IRQ_HANDLER_T vocIrqHandler) ;
HAL_ERR_T vpp_PreviewYUVOpen(HAL_VOC_IRQ_HANDLER_T vocIrqHandler) ;
void vpp_PreviewClose(void);
HAL_ERR_T vpp_PreviewScheduleOneFrame(CutZoomInStruct *pEncIn);

#endif  // vpp_PREVIEW_H









