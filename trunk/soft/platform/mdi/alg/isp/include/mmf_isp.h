/*author: mayx*/
/*Date: 2007/08/07 */
#ifndef _MMF_ISP_H_
#define _MMF_ISP_H_
#include <cs_types.h>


enum  MMF_effect
{ 
	SPECIA, 
	GRAY, 
	NEGATIVE,
	RED_STANDOUT 
};

enum  MMF_ZoomOutWay 
{
	INTERPOLATION1,
	INTERPOLATION2 
};

enum  MMF_ZoomInWay 
{
	TWENTY_FIVE_PERCENT,
	FIFTY_PERCENT,
	SEVENTY_FIVE_PERCENT,
	EIGHTY_PERCENTY 
};


//error code
#define ISP_DATA_ERROR  -1
#define ISP_STATE_ERROR -2
#define ISP_FUN_OK       1
#define int_abs(a)    	(a)>=0?(a):(-a)
#define fun_max(a,b)	(a)>(b)?(a):(b)



int 	MMF_Isp_Specialeffect1 (unsigned short H,  unsigned short W,  unsigned short* pInBuf, enum MMF_effect  Effect, unsigned short* pOutBuf );
int	MMF_Isp_ZoomOut(unsigned short* pH,  unsigned short* pW, unsigned short* rgb_buf,  enum MMF_ZoomOutWay ZoomOutWay, unsigned short* output_buf);
int 	MMF_ISP_ZoomIn(unsigned short* pH,unsigned short* pW,unsigned short* pInBuf,unsigned short* pOutBuf, enum MMF_ZoomInWay ZoomInWay, int display_mode);
int	MMF_Isp_Blend1(unsigned short H1,unsigned short W1,unsigned short* pBuf1,unsigned short H2,unsigned short W2,unsigned short* pBuf2,unsigned short OffsetH,unsigned short OffsetW,unsigned short TransColor,unsigned short* pBufRtn);
int     MMF_ISP_VGAto1280_1024( unsigned short hei, unsigned short wid, unsigned short* rgb_buf, unsigned short* output_buf );
int     MMF_ISP_Rotate( int* pH, int* pW, unsigned short* rgb_buf, int angle, unsigned short* output_buf );
int 	MMF_ISP_VGAto160_128( int* pH, int* pW, unsigned short* rgb_buf, unsigned short* output_buf );
int	MMF_Isp_Blend_Rotate(unsigned short* pH,unsigned short* pW,unsigned short* pBuf1,unsigned short H2,unsigned short W2,unsigned short* pBuf2,unsigned short OffsetH,unsigned short OffsetW,unsigned short TransColor,unsigned short* pBufRtn, unsigned short Mode );

//11yue2ri
int MMF_ISP_ZoomIn_Blend_Rotate(unsigned short hei_b, unsigned short wid_b, unsigned short* rgb_buf_b, 
				unsigned short hei_a_ori, unsigned short wid_a_ori, unsigned short* rgb_buf_a,
				unsigned short offsetH, unsigned short offsetW, enum MMF_ZoomInWay ZoomInWay );


#endif
