// VideoCapture.h: interface for the VideoCapture class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VIDEOCAPTURE_H__5C825E61_611A_11D6_889B_000B2B0F84B6__INCLUDED_)
#define AFX_VIDEOCAPTURE_H__5C825E61_611A_11D6_889B_000B2B0F84B6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



#include<vfw.h>
#include<afxmt.h>
#include<afxole.h>

//#include "..\h263\encoder\def.h"

/****************************   NOTE **************************/
//  If the parameters below are changed then change following
//  > cdata & cbuffer_size in VideoNetDlg.cpp
//  > rgbdata & buffersize in VideoNetDlg.cpp
//  > update CPARAMS structure before compression
//  > computation of local_wnd_x para in OnInitDialog()
//
////////////////////////////////////////////////////////////////

#if 0 //bruce
#define QCIF_WIDTH  176
#define QCIF_HEIGHT 144

#define IMAGE_WIDTH       QCIF_WIDTH
#define IMAGE_HEIGHT      QCIF_HEIGHT
#endif  

//bruce
#define OTHER_YWIDTH	96
#define OTHER_YHEIGHT	96

//camera supported optimize pixel format
#define CAMERA_SUPPORT_YWIDTH		176
#define CAMERA_SUPPORT_YHEIGHT		144

//bruce
//#define CPARAM_DEFAULT_FORMAT CPARAM_OTHER	//CPARAM_OTHER	//bruce	CPARAM_QCIF
#if (CPARAM_DEFAULT_FORMAT==CPARAM_SQCIF)
#define IMAGE_WIDTH       SQCIF_YWIDTH
#define IMAGE_HEIGHT      SQCIF_YHEIGHT
#elif (CPARAM_DEFAULT_FORMAT==CPARAM_QCIF)
#define IMAGE_WIDTH       QCIF_YWIDTH
#define IMAGE_HEIGHT      QCIF_YHEIGHT
#elif (CPARAM_DEFAULT_FORMAT==CPARAM_CIF)
#define IMAGE_WIDTH       CIF_YWIDTH
#define IMAGE_HEIGHT      CIF_YHEIGHT
#elif (CPARAM_DEFAULT_FORMAT==CPARAM_4CIF)
#define IMAGE_WIDTH       CIF4_YWIDTH
#define IMAGE_HEIGHT      CIF4_YHEIGHT
#elif (CPARAM_DEFAULT_FORMAT==CPARAM_16CIF)
#define IMAGE_WIDTH       CIF16_YWIDTH
#define IMAGE_HEIGHT      CIF16_YHEIGHT
//OTHER
#elif (CPARAM_DEFAULT_FORMAT==CPARAM_OTHER)
#define IMAGE_WIDTH       OTHER_YWIDTH
#define IMAGE_HEIGHT      OTHER_YHEIGHT
#endif

#define IMAGE_WIDTH	96
#define IMAGE_HEIGHT	96

#if 0
class VideoCapture  
{
public:
	
	HWND m_capwnd;
	CStdioFile log;
	CAPDRIVERCAPS caps;
	CMutex protectframe;
	int avgaudiosize;
	BOOL isOver;
	CDialog *dlg;
	BITMAPINFOHEADER m_bmpheader;
	BITMAPINFO m_bmpinfo;

	BOOL isCamDefatulFmt; //bruce

	VideoCapture();
	virtual ~VideoCapture();
	void SetDialog(CDialog *dlg);
	BOOL Initialize();
	BOOL  SetCapturePara();
	void Destroy();
	void GetDriverCaps();
	void SetAudioFormat();
	int AllocateMemory(PBITMAPINFO &bmpinfo);
	int getFormatSize(BITMAPINFO bmp);
    int getImageSize(BITMAPINFO bmp);

	BOOL StartCapture();
	BOOL StopCapture();

	//Callback functions
    //friend LRESULT CALLBACK OnCaptureError(HWND hwnd, int errid, LPSTR errtext);
    friend LRESULT CALLBACK OnCaptureVideo(HWND hWnd, LPVIDEOHDR lpheader) ;
    //friend LRESULT CALLBACK OnCaptureFrame(HWND hWnd, LPVIDEOHDR lpheader);
    //friend LRESULT CALLBACK OnCaptureWave(HWND hWnd, LPWAVEHDR lpheader);

};

#else
	HWND m_capwnd;
//	CStdioFile log;
	CAPDRIVERCAPS caps;
	CMutex protectframe;
	int avgaudiosize;
	BOOL isOver;
//	CDialog *dlg;
	BITMAPINFOHEADER m_bmpheader;
	BITMAPINFO m_bmpinfo;

	BOOL isCamDefatulFmt; //bruce

//	VideoCapture();
//	virtual ~VideoCapture();
//	void SetDialog(CDialog *dlg);
	BOOL VideoCapture_Initialize();
	BOOL  VideoCapture_SetCapturePara();
	void VideoCapture_Destroy();
	void VideoCapture_GetDriverCaps();
//	void VideoCapture_SetAudioFormat();
	int VideoCapture_AllocateMemory(PBITMAPINFO &bmpinfo);
	int VideoCapture_getFormatSize(BITMAPINFO bmp);
    int VideoCapture_getImageSize(BITMAPINFO bmp);

	BOOL VideoCapture_StartCapture();
	BOOL VideoCapture_StopCapture();

	//Callback functions
    //friend LRESULT CALLBACK OnCaptureError(HWND hwnd, int errid, LPSTR errtext);
    LRESULT CALLBACK OnCaptureVideo(HWND hWnd, LPVIDEOHDR lpheader) ;
    //friend LRESULT CALLBACK OnCaptureFrame(HWND hWnd, LPVIDEOHDR lpheader);
    //friend LRESULT CALLBACK OnCaptureWave(HWND hWnd, LPWAVEHDR lpheader);


	extern "C" void GetWebCamPreviewFrame(unsigned char *p_Data,unsigned int dataLen);


#endif

#endif // !defined(AFX_VIDEOCAPTURE_H__5C825E61_611A_11D6_889B_000B2B0F84B6__INCLUDED_)
