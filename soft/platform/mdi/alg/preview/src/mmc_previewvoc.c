#include "vpp_preview.h"
#include "cs_types.h"

#include "mcip_debug.h"

volatile int Previewframecount=0;
volatile int PreviewFirstIsr=0;
#define  uint16  unsigned short
static   int  flagpreview=0;
extern PUBLIC UINT32 hal_TimGetUpTime(VOID);
void PreviewIsr(void)
{

	if (PreviewFirstIsr)
	  Previewframecount=1;
	else
	  PreviewFirstIsr=1;

	MCI_TRACE(MCI_CAMERA_TRC|TSTDOUT, 0, "[MMC_PREVIEW]PreviewIsr TIME=%d",hal_TimGetUpTime());

}

void initVocPreview(void)
{
	MCI_TRACE(MCI_CAMERA_TRC|TSTDOUT, 0, "[MMC_PREVIEW]initVocPreview");

	//diag_printf("************init voc for initVocPreview");
	flagpreview = 1;
	Previewframecount=0;
	PreviewFirstIsr=0;
	vpp_PreviewOpen((HAL_VOC_IRQ_HANDLER_T)PreviewIsr);

}

void initVocPreviewYUV(void)
{
	MCI_TRACE(MCI_CAMERA_TRC|TSTDOUT, 0, "[MMC_PREVIEW]initVocPreviewYUV");

	flagpreview = 1;
	Previewframecount=0;
	PreviewFirstIsr=0;
	vpp_PreviewYUVOpen((HAL_VOC_IRQ_HANDLER_T)PreviewIsr);

}

void quitPreview(void)
{
      if(flagpreview==1)
      	{
	MCI_TRACE(MCI_CAMERA_TRC|TSTDOUT, 0, "[MMC_PREVIEW]quitPreview");

	vpp_PreviewClose();
	flagpreview=0;
      	}
}


void  MMC_Preview_Wait(CutZoomInStruct*  pIn)
{
	
	if(flagpreview==1)
	{
		UINT32 time=hal_TimGetUpTime();
		vpp_PreviewScheduleOneFrame(pIn);

#if 1
		while(Previewframecount==0)
		{
		;
		}
		Previewframecount = 0;
#endif	

		MCI_TRACE(MCI_CAMERA_TRC|TSTDOUT, 0, "[MMC_PREVIEW]MMC_Preview_Wait.VoC PROCESS TOTAL TIME=%d",hal_TimGetUpTime()-time);
	}
	else
	{
		MCI_ASSERT(1, "[MMC_PREVIEW_WARN]VOC IS NOT OPENED!");
	}

}

void  MMC_Preview(CutZoomInStruct*  pIn)
{
	MCI_TRACE(MCI_CAMERA_TRC|TSTDOUT, 0, "[MMC_PREVIEW]MMC_Preview TIME=%d",hal_TimGetUpTime());

	vpp_PreviewScheduleOneFrame(pIn);
#if 0
	while(Previewframecount==0)
	{
		;
	}
	Previewframecount = 0;
#endif	
	//diag_printf("*****************MMC_Preview  finish");

}

