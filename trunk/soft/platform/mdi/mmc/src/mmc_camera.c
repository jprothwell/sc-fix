
#if (CHIP_HAS_GOUDA == 1)

#ifdef __PRJ_WITH_SPICAM__

#include "cs_types.h"
#include "global.h"
#include "event.h"
#include "sxr_tls.h"
#include "mcip_debug.h"
#include "sys_ctrl.h"
#include "csw.h"
#include "hal_gouda.h"
#include "camera_m.h"
#include "mmc.h"
#include "mmc_camera.h"
#include "mmc_timer.h"

#include "lcdd_m.h"
#include "lcd_if.h"

#include "vpp_audiojpeg_enc.h"
#include "vpp_preview.h"

typedef enum {
    MMC_TASK_STOPPED,
    MMC_TASK_INITIALIZED,
    MMC_TASK_PREVIEWING,
    MMC_TASK_CAPTURING,
} MMC_TASK_STATE_E;

#define MSG_MMC_BAL_CAPTURE_REQ 0x1001
#define MMC_CAM_SRC_WIDTH 240
#define MMC_CAM_SRC_HEIGHT 320

volatile int32 countdown =3;
volatile uint16 srcwidth=0, srcheight=0; // the width and height of the image sent out by ISP
volatile uint16 cutwidth=0, cutheight=0; // the width and height of the im
volatile CAM_INFO_STRUCT CameraInfo = {0, };
volatile uint32 startPos = 0;   // the start pixel to be previewed
volatile uint16 bset=0; // 0 - special effect was not initiated, 1 - special effect was initiated  
volatile int32 Effect_Set_Valid;

LCDD_FBW_T displayFbw;
CAM_SIZE_T MY_NPIX = CAM_NPIX_VGA;
uint16* sensorbuf = NULL;
uint16* blendBuffer = NULL;
volatile int32 sensorready=0;

static MMC_TASK_STATE_E s_mmcTaskState = MMC_TASK_STOPPED;

#define   Q15      32767
#define   Q14      16384
#define   S15      15
#define   MASK     (Q15-1)

void  zoom_line_yuv(uint16* srcbuf,uint16* dec, uint16 dec_w,int scalex)
{
    //input format : y1 u y2 v 
    short wa,wb,i,w;
    int   wn;
    uint16 y,y1,u,u1,v,v1;
    short *pb=dec;
    short *pbu=dec+dec_w;
    uint16 *pa=srcbuf;

    wn = 0;
    for (i=0;i<(dec_w>>1);i++)
    {
        wb =( wn)&(MASK);
        wa = Q15 - wb;

        w  = (wn>>S15);
        /////////u v//////////
        pa = srcbuf + (w<<1);
        u = (*(pa))>>8;
        v = (*(pa+1))>>8;
        u1 = (*(pa+2))>>8;
        v1 = (*(pa+3))>>8;
        /////////y////////////
        pa = srcbuf + w ;
        y = ((*pa++))&0xff;
        y1 = ((*pa))&0xff;

        *pb++ =(y*wa + y1*wb)>>S15;
        *pbu++ = (u*wa+u1*wb)>>S15;
        *pbu++ = (v*wa+v1*wb)>>S15;

        wn+=scalex;
    }

    for (i=0;i<(dec_w>>1);i++)
    {
        wb =( wn)&(MASK);
        wa = Q15 - wb;

        w  = (wn>>S15);

        pa = srcbuf + w ;
        y = ((*pa++))&0xff;
        y1 = ((*pa))&0xff;

        *pb++ =(y*wa + y1*wb)>>S15;

        wn+=scalex;
    }
}

const BYTE rgb_sat[768] = 
{
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0x0,0x1,0x2,0x3,0x4,0x5,0x6,0x7,0x8,0x9,0xa,0xb,0xc,0xd,0xe,0xf,
0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f,
0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2a,0x2b,0x2c,0x2d,0x2e,0x2f,
0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3a,0x3b,0x3c,0x3d,0x3e,0x3f,
0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4a,0x4b,0x4c,0x4d,0x4e,0x4f,
0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5a,0x5b,0x5c,0x5d,0x5e,0x5f,
0x60,0x61,0x62,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6a,0x6b,0x6c,0x6d,0x6e,0x6f,
0x70,0x71,0x72,0x73,0x74,0x75,0x76,0x77,0x78,0x79,0x7a,0x7b,0x7c,0x7d,0x7e,0x7f,
0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f,
0x90,0x91,0x92,0x93,0x94,0x95,0x96,0x97,0x98,0x99,0x9a,0x9b,0x9c,0x9d,0x9e,0x9f,
0xa0,0xa1,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7,0xa8,0xa9,0xaa,0xab,0xac,0xad,0xae,0xaf,
0xb0,0xb1,0xb2,0xb3,0xb4,0xb5,0xb6,0xb7,0xb8,0xb9,0xba,0xbb,0xbc,0xbd,0xbe,0xbf,
0xc0,0xc1,0xc2,0xc3,0xc4,0xc5,0xc6,0xc7,0xc8,0xc9,0xca,0xcb,0xcc,0xcd,0xce,0xcf,
0xd0,0xd1,0xd2,0xd3,0xd4,0xd5,0xd6,0xd7,0xd8,0xd9,0xda,0xdb,0xdc,0xdd,0xde,0xdf,
0xe0,0xe1,0xe2,0xe3,0xe4,0xe5,0xe6,0xe7,0xe8,0xe9,0xea,0xeb,0xec,0xed,0xee,0xef,
0xf0,0xf1,0xf2,0xf3,0xf4,0xf5,0xf6,0xf7,0xf8,0xf9,0xfa,0xfb,0xfc,0xfd,0xfe,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
};
const short TableV2G[]  =
{
 0,   0,   1,   2,   2,   3,   4,   5,   5,   6,   7,   7,   8,   9,  10,  10, 
 11,  12,  12,  13,  14,  15,  15,  16,  17,  17,  18,  19,  20,  20,  21,  22, 
 22,  23,  24,  25,  25,  26,  27,  27,  28,  29,  30,  30,  31,  32,  32,  33, 
 34,  35,  35,  36,  37,  37,  38,  39,  40,  40,  41,  42,  42,  43,  44,  45, 
 45,  46,  47,  47,  48,  49,  50,  50,  51,  52,  52,  53,  54,  55,  55,  56, 
 57,  57,  58,  59,  60,  60,  61,  62,  62,  63,  64,  65,  65,  66,  67,  67, 
 68,  69,  70,  70,  71,  72,  72,  73,  74,  75,  75,  76,  77,  77,  78,  79, 
 80,  80,  81,  82,  82,  83,  84,  85,  85,  86,  87,  87,  88,  89,  90,  90, 
 91,  92,  92,  93,  94,  95,  95,  96,  97,  97,  98,  99,  100, 100, 101, 102, 
 102, 103, 104, 105, 105, 106, 107, 107, 108, 109, 110, 110, 111, 112, 112, 113, 
 114, 115, 115, 116, 117, 117, 118, 119, 120, 120, 121, 122, 122, 123, 124, 125, 
 125, 126, 127, 127, 128, 129, 130, 130, 131, 132, 132, 133, 134, 135, 135, 136, 
 137, 137, 138, 139, 140, 140, 141, 142, 142, 143, 144, 145, 145, 146, 147, 147, 
 148, 149, 150, 150, 151, 152, 152, 153, 154, 155, 155, 156, 157, 157, 158, 159, 
 160, 160, 161, 162, 162, 163, 164, 165, 165, 166, 167, 167, 168, 169, 170, 170, 
 171, 172, 172, 173, 174, 175, 175, 176, 177, 177, 178, 179, 180, 180, 181, 182
};
const short TabV2R[] =
{
-179 ,-178 ,-177 ,-175 ,-174 ,-172 ,-171 ,-170 ,
-168 ,-167 ,-165 ,-164 ,-163 ,-161 ,-160 ,-158 ,
-157 ,-156 ,-154 ,-153 ,-151 ,-150 ,-149 ,-147 ,
-146 ,-144 ,-143 ,-142 ,-140 ,-139 ,-137 ,-136 ,
-135 ,-133 ,-132 ,-130 ,-129 ,-128 ,-126 ,-125 ,
-123 ,-122 ,-121 ,-119 ,-118 ,-116 ,-115 ,-114 ,
-112 ,-111 ,-109 ,-108 ,-107 ,-105 ,-104 ,-102 ,
-101 ,-100 , -98 , -97 , -95 , -94 , -93 , -91 ,
 -90 , -88 , -87 , -86 , -84 , -83 , -81 , -80 ,
 -79 , -77 , -76 , -74 , -73 , -72 , -70 , -69 ,
 -67 , -66 , -65 , -63 , -62 , -60 , -59 , -57 ,
 -56 , -55 , -53 , -52 , -50 , -49 , -48 , -46 ,
 -45 , -43 , -42 , -41 , -39 , -38 , -36 , -35 ,
 -34 , -32 , -31 , -29 , -28 , -27 , -25 , -24 ,
 -22 , -21 , -20 , -18 , -17 , -15 , -14 , -13 ,
 -11 , -10 ,  -8 ,  -7 ,  -6 ,  -4 ,  -3 ,  -1 ,
   0 ,   1 ,   3 ,   4 ,   6 ,   7 ,   8 ,  10 ,
  11 ,  13 ,  14 ,  15 ,  17 ,  18 ,  20 ,  21 ,
  22 ,  24 ,  25 ,  27 ,  28 ,  29 ,  31 ,  32 ,
  34 ,  35 ,  36 ,  38 ,  39 ,  41 ,  42 ,  43 ,
  45 ,  46 ,  48 ,  49 ,  50 ,  52 ,  53 ,  55 ,
  56 ,  57 ,  59 ,  60 ,  62 ,  63 ,  65 ,  66 ,
  67 ,  69 ,  70 ,  72 ,  73 ,  74 ,  76 ,  77 ,
  79 ,  80 ,  81 ,  83 ,  84 ,  86 ,  87 ,  88 ,
  90 ,  91 ,  93 ,  94 ,  95 ,  97 ,  98 , 100 ,
 101 , 102 , 104 , 105 , 107 , 108 , 109 , 111 ,
 112 , 114 , 115 , 116 , 118 , 119 , 121 , 122 ,
 123 , 125 , 126 , 128 , 129 , 130 , 132 , 133 ,
 135 , 136 , 137 , 139 , 140 , 142 , 143 , 144 ,
 146 , 147 , 149 , 150 , 151 , 153 , 154 , 156 ,
 157 , 158 , 160 , 161 , 163 , 164 , 165 , 167 ,
 168 , 170 , 171 , 172 , 174 , 175 , 177 , 178 ,
};
const short TabU2G[] =
{
 135 , 135 , 135 , 134 , 134 , 134 , 133 , 133 ,
 133 , 132 , 132 , 132 , 131 , 131 , 131 , 130 ,
 130 , 130 , 129 , 129 , 129 , 128 , 128 , 128 ,
 127 , 127 , 127 , 126 , 126 , 126 , 125 , 125 ,
 124 , 124 , 124 , 123 , 123 , 123 , 122 , 122 ,
 122 , 121 , 121 , 121 , 120 , 120 , 120 , 119 ,
 119 , 119 , 118 , 118 , 118 , 117 , 117 , 117 ,
 116 , 116 , 116 , 115 , 115 , 115 , 114 , 114 ,
 113 , 113 , 113 , 112 , 112 , 112 , 111 , 111 ,
 111 , 110 , 110 , 110 , 109 , 109 , 109 , 108 ,
 108 , 108 , 107 , 107 , 107 , 106 , 106 , 106 ,
 105 , 105 , 105 , 104 , 104 , 104 , 103 , 103 ,
 102 , 102 , 102 , 101 , 101 , 101 , 100 , 100 ,
 100 ,  99 ,  99 ,  99 ,  98 ,  98 ,  98 ,  97 ,
  97 ,  97 ,  96 ,  96 ,  96 ,  95 ,  95 ,  95 ,
  94 ,  94 ,  94 ,  93 ,  93 ,  93 ,  92 ,  92 ,
  91 ,  91 ,  91 ,  90 ,  90 ,  90 ,  89 ,  89 ,
  89 ,  88 ,  88 ,  88 ,  87 ,  87 ,  87 ,  86 ,
  86 ,  86 ,  85 ,  85 ,  85 ,  84 ,  84 ,  84 ,
  83 ,  83 ,  83 ,  82 ,  82 ,  82 ,  81 ,  81 ,
  80 ,  80 ,  80 ,  79 ,  79 ,  79 ,  78 ,  78 ,
  78 ,  77 ,  77 ,  77 ,  76 ,  76 ,  76 ,  75 ,
  75 ,  75 ,  74 ,  74 ,  74 ,  73 ,  73 ,  73 ,
  72 ,  72 ,  72 ,  71 ,  71 ,  71 ,  70 ,  70 ,
  69 ,  69 ,  69 ,  68 ,  68 ,  68 ,  67 ,  67 ,
  67 ,  66 ,  66 ,  66 ,  65 ,  65 ,  65 ,  64 ,
  64 ,  64 ,  63 ,  63 ,  63 ,  62 ,  62 ,  62 ,
  61 ,  61 ,  61 ,  60 ,  60 ,  60 ,  59 ,  59 ,
  58 ,  58 ,  58 ,  57 ,  57 ,  57 ,  56 ,  56 ,
  56 ,  55 ,  55 ,  55 ,  54 ,  54 ,  54 ,  53 ,
  53 ,  53 ,  52 ,  52 ,  52 ,  51 ,  51 ,  51 ,
  50 ,  50 ,  50 ,  49 ,  49 ,  49 ,  48 ,  48 ,
};
const short TabU2B[] =
{
-227 ,-226 ,-224 ,-222 ,-220 ,-219 ,-217 ,-215 ,
-213 ,-212 ,-210 ,-208 ,-206 ,-204 ,-203 ,-201 ,
-199 ,-197 ,-196 ,-194 ,-192 ,-190 ,-188 ,-187 ,
-185 ,-183 ,-181 ,-180 ,-178 ,-176 ,-174 ,-173 ,
-171 ,-169 ,-167 ,-165 ,-164 ,-162 ,-160 ,-158 ,
-157 ,-155 ,-153 ,-151 ,-149 ,-148 ,-146 ,-144 ,
-142 ,-141 ,-139 ,-137 ,-135 ,-134 ,-132 ,-130 ,
-128 ,-126 ,-125 ,-123 ,-121 ,-119 ,-118 ,-116 ,
-114 ,-112 ,-110 ,-109 ,-107 ,-105 ,-103 ,-102 ,
-100 , -98 , -96 , -94 , -93 , -91 , -89 , -87 ,
 -86 , -84 , -82 , -80 , -79 , -77 , -75 , -73 ,
 -71 , -70 , -68 , -66 , -64 , -63 , -61 , -59 ,
 -57 , -55 , -54 , -52 , -50 , -48 , -47 , -45 ,
 -43 , -41 , -40 , -38 , -36 , -34 , -32 , -31 ,
 -29 , -27 , -25 , -24 , -22 , -20 , -18 , -16 ,
 -15 , -13 , -11 ,  -9 ,  -8 ,  -6 ,  -4 ,  -2 ,
   0 ,   1 ,   3 ,   5 ,   7 ,   8 ,  10 ,  12 ,
  14 ,  15 ,  17 ,  19 ,  21 ,  23 ,  24 ,  26 ,
  28 ,  30 ,  31 ,  33 ,  35 ,  37 ,  39 ,  40 ,
  42 ,  44 ,  46 ,  47 ,  49 ,  51 ,  53 ,  54 ,
  56 ,  58 ,  60 ,  62 ,  63 ,  65 ,  67 ,  69 ,
  70 ,  72 ,  74 ,  76 ,  78 ,  79 ,  81 ,  83 ,
  85 ,  86 ,  88 ,  90 ,  92 ,  93 ,  95 ,  97 ,
  99 , 101 , 102 , 104 , 106 , 108 , 109 , 111 ,
 113 , 115 , 117 , 118 , 120 , 122 , 124 , 125 ,
 127 , 129 , 131 , 133 , 134 , 136 , 138 , 140 ,
 141 , 143 , 145 , 147 , 148 , 150 , 152 , 154 ,
 156 , 157 , 159 , 161 , 163 , 164 , 166 , 168 ,
 170 , 172 , 173 , 175 , 177 , 179 , 180 , 182 ,
 184 , 186 , 187 , 189 , 191 , 193 , 195 , 196 ,
 198 , 200 , 202 , 203 , 205 , 207 , 209 , 211 ,
 212 , 214 , 216 , 218 , 219 , 221 , 223 , 225 ,
};

int  Coolsand_bilinerz_zoom_op_yuv2rgb(uint16* srcbuf,uint16 src_w,uint16 src_h,uint16 cut_w,uint16 cut_h,uint16* decbuf,uint16 dec_w,uint16 dec_h)
{
    short  offx,offy;
    short  wa, wb, i, j;
    int    w,scalex,scaley;
    uint16  *pb ,*pa,*p1,*p2,*p3,*p4,*p5,*p6;
    int y,u,v,y1,u1,v1,y2,r,g,b;
    short*  tmpbuf =(short*)mmc_MemMallocBack(dec_w*8) ;
    short*  ptmp2 = tmpbuf + dec_w*2;
    short  num1,num2,num3;
    BYTE *irgb_sat = (BYTE *)(rgb_sat+256);

    if(tmpbuf==NULL)
        return -1;
  
    offx = (src_w - cut_w)>>1;
    offx = (offx>>1)<<1;
    offy = (src_h - cut_h)>>1; 
    scalex=((cut_w-1)<<S15)/dec_w;
    scaley=((cut_h-1)<<S15)/dec_h;
    pa = srcbuf + offy*src_w + offx;
    pb = decbuf;
    w  = 0;
    num2 = -2;
    num3 = 1;
    //p3 = pa;
    //p4 = p3 + src_w;
    //zoom_line_yuv(p3,tmpbuf,dec_w,scalex);
    //zoom_line_yuv(p4,ptmp2,dec_w,scalex);
    p1 = tmpbuf;
    p2 = ptmp2;
    for (j=0;j<dec_h;j++)
    {
        num1 = (w>>S15);
        p3 = pa + (num1*src_w);
        p4 = p3 + src_w;
 
        if (num1-num2==1)
        {
            if (num3==1)
            {
                zoom_line_yuv(p4,tmpbuf,dec_w,scalex);
                p2 = tmpbuf;
                p1 = ptmp2;
                num3=0;
            }
            else
            {
                zoom_line_yuv(p4,ptmp2,dec_w,scalex);
                p1 = tmpbuf;
                p2 = ptmp2;
                num3 = 1;
            }
        }
        else if (num1!=num2)
        {
            zoom_line_yuv(p3,tmpbuf,dec_w,scalex);
            zoom_line_yuv(p4,ptmp2,dec_w,scalex);
            p1 = tmpbuf;
            p2 = ptmp2;
            num3 = 1;
        }
        wb = (w)&(MASK);
        wa = Q15 - wb;
   
        p3 = p1;
        p4 = p2;
        p5 = p1 + dec_w;
        p6 = p2 + dec_w;
        for (i=0;i<(dec_w>>1);i++)
        {
            y = *p3++;
            u = *p5++;
            v = *p5++;
            y1 = *p4++;
            u1 = *p6++;
            v1 = *p6++;
            y = ((y*wa + y1*wb)>>S15);
            u = ((u*wa + u1*wb)>>S15);
            v = ((v*wa + v1*wb)>>S15);
            ////////////////////////////
            y1 = *p3++;
            y2 = *p4++;
            y1 = ((y1*wa + y2*wb)>>S15);
            r = irgb_sat[y + TabV2R[v]];
            g = irgb_sat[y + TabU2G[u] - TableV2G[v]];
            b = irgb_sat[y + TabU2B[u]];
            *pb++ = DRV_RGB2PIXEL565(r,g,b);
            r = irgb_sat[y1 + TabV2R[v]];
            g = irgb_sat[y1 + TabU2G[u] - TableV2G[v]];
            b = irgb_sat[y1 + TabU2B[u]];
            *pb++ = DRV_RGB2PIXEL565(r,g,b);
        }
        w+=scaley;
        num2 = num1;
    }
    mmc_MemFreeLast((char*)tmpbuf);
    return 0;
}

int Coolsand_bilinerz_zoom_op_yuv(uint16* srcbuf,uint16 src_w,uint16 src_h,uint16 cut_w,uint16 cut_h,uint16* decbuf,uint16 dec_w,uint16 dec_h)
{
    short  offx,offy;
    short  wa,wb,i,j;
    int    w,scalex,scaley;
    uint16  *pb ,*pa,*p1,*p2,*p3,*p4,*p5,*p6;
    short y,u,v,y1,u1,v1;
    short*  tmpbuf =(short*)mmc_MemMallocBack(dec_w*8) ;
    short*  ptmp2 = tmpbuf + dec_w*2;
    short  num1,num2,num3;

    if(tmpbuf==NULL)
        return -1;
		
    offx = (src_w - cut_w)>>1;
    offx = (offx>>1)<<1;
    offy = (src_h - cut_h)>>1;
		
    scalex=((cut_w-1)<<S15)/dec_w;
    scaley=((cut_h-1)<<S15)/dec_h;
		
    pa = srcbuf + offy*src_w + offx;
    pb = decbuf;
		
    w  = 0;
    num2 = -2;
    num3 = 1;

    p1 = tmpbuf;
    p2 = ptmp2;

    if (dec_h >= 480)
    {
        short*  PTmp=(short*)mmc_MemMallocBack(320*61*2) ;
        if(PTmp==NULL)
            return -1;

        memcpy(PTmp,decbuf+320*179,320*61*2);//keep one more line
        for (j=0;j<dec_h;j++)
        {
            num1 = (w>>S15);

            if (num1>(178-offy))//for 640*480
            {
                pa = PTmp + offx;
                w = w & MASK;
                num1 = num1-179+offy;
                p3 = pa + (num1*src_w);
                p4 = p3 + src_w;
            }
            else
            {
                p3 = pa + (num1*src_w);
                p4 = p3 + src_w;
            }

            if (num1-num2==1)
            {
                if (num3==1)
                {
                    zoom_line_yuv(p4,tmpbuf,dec_w,scalex);
                    p2 = tmpbuf;
                    p1 = ptmp2;
                    num3=0;
                }
                else
                {
                    zoom_line_yuv(p4,ptmp2,dec_w,scalex);
                    p1 = tmpbuf;
                    p2 = ptmp2;
                    num3 = 1;
                }
            }
            else if (num1!=num2)
            {
                zoom_line_yuv(p3,tmpbuf,dec_w,scalex);
                zoom_line_yuv(p4,ptmp2,dec_w,scalex);
                p1 = tmpbuf;
                p2 = ptmp2;
                num3 = 1;
            }
				
            wb =( w)&(MASK);
            wa = Q15 - wb;
				
            p3 = p1;
            p4 = p2;
            p5 = p1 + dec_w;
            p6 = p2 + dec_w;
            for (i=0;i<(dec_w>>1);i++)
            {
                y = *p3++;
                u = *p5++;
                y1 = *p4++;
                u1 = *p6++;

                y = ((y*wa + y1*wb)>>S15);
                u = ((u*wa + u1*wb)>>S15);
					
                *pb++ = (u<<8)|(y);
                /////////////////////////////
                y = *p3++;
                v = *p5++;
                y1 = *p4++;
                v1 = *p6++;

                y = ((y*wa + y1*wb)>>S15);
                v = ((v*wa + v1*wb)>>S15);
                *pb++ = (v<<8)|(y);
            }

            w+=scaley;
            num2 = num1;
        }

        mmc_MemFreeLast((char*)PTmp);
    }
    else
    {
        for (j=0;j<dec_h;j++)
        {
            num1 = (w>>S15);
            p3 = pa + (num1*src_w);
            p4 = p3 + src_w;
            
            if (num1-num2==1)
            {
                if (num3==1)
                {
                    zoom_line_yuv(p4,tmpbuf,dec_w,scalex);
                    p2 = tmpbuf;
                    p1 = ptmp2;
                    num3=0;
                }
                else
                {
                    zoom_line_yuv(p4,ptmp2,dec_w,scalex);
                    p1 = tmpbuf;
                    p2 = ptmp2;
                    num3 = 1;
                }
            }
            else if (num1!=num2)
            {
                zoom_line_yuv(p3,tmpbuf,dec_w,scalex);
                zoom_line_yuv(p4,ptmp2,dec_w,scalex);
                p1 = tmpbuf;
                p2 = ptmp2;
                num3 = 1;
            }

            wb =( w)&(MASK);
            wa = Q15 - wb;
			
            p3 = p1;
            p4 = p2;
            p5 = p1 + dec_w;
            p6 = p2 + dec_w;

            for (i=0;i<(dec_w>>1);i++)
            {
                y = *p3++;
                u = *p5++;
                y1 = *p4++;
                u1 = *p6++;

                y = ((y*wa + y1*wb)>>S15);
                u = ((u*wa + u1*wb)>>S15);
					
                *pb++ = (u<<8)|(y);
                /////////////////////////////
                y = *p3++;
                v = *p5++;
                y1 = *p4++;
                v1 = *p6++;

                y = ((y*wa + y1*wb)>>S15);
                v = ((v*wa + v1*wb)>>S15);
                *pb++ = (v<<8)|(y);
            }
            w+=scaley;
            num2 = num1;
        }
    }
		
    mmc_MemFreeLast((char*)tmpbuf);

    return 0;
}

int  Coolsand_VGA_zoom_op_yuv(uint16* srcbuf,uint16 src_w,uint16 src_h,uint16 cut_w,uint16 cut_h,uint16* decbuf,uint16 dec_w,uint16 dec_h)
{
    short  offx,offy;
    short  wa,wb,i,j,scalex,scaley;
    int    w;
    uint16  *pb ,*pa,*p1,*p2,*p3,*p4,*p5,*p6;
    short y,u,v,y1,u1,v1;
    short*  tmpbuf =(short*)mmc_MemMalloc(dec_w*8) ;
    short*  ptmp2 = tmpbuf + dec_w*2;
    short  num1,num2,num3;
    int    flag=0;

    if(tmpbuf==NULL)
        return -1;

    offx = (src_w - cut_w)>>1;
    offx = (offx>>1)<<1;
    offy = (src_h - cut_h)>>1;

    scalex=((cut_w-1)<<S15)/dec_w;
    scaley=((cut_h-1)<<S15)/dec_h;
	
    pa = srcbuf + offy*src_w + offx;
    pb = decbuf;

    w  = 0;
    num2 = -2;
    num3 = 1;

    p3 = pa;
    p4 = p3 + src_w;
    p1 = NULL;
    p2 = NULL;
    //zoom_line_yuv_normal(p3,tmpbuf,dec_w,scalex);
    //zoom_line_yuv_normal(p4,ptmp2,dec_w,scalex);
    //p1 = tmpbuf;
    //p2 = ptmp2;

    for (j=0;j<dec_h;j++)
    {
        num1 = (w>>S15);
        if ( !flag &&j>=offy + num1 )
        {
            //break;
            flag = 1;//break;
            memcpy(blendBuffer,srcbuf+ offx+j*src_w,j*src_w*2);
            w = w&MASK;
            num1 = 0;
            num2 -= num1;
            pa = blendBuffer;
        }

        p3 = pa + (num1*src_w);
        p4 = p3 + src_w;

        if (num1-num2==1)
        {
            if (num3==1)
            {
                zoom_line_yuv(p4,tmpbuf,dec_w,scalex);
                p2 = tmpbuf;
                p1 = ptmp2;
                num3=0;
            }
            else
            {
                zoom_line_yuv(p4,ptmp2,dec_w,scalex);
                p1 = tmpbuf;
                p2 = ptmp2;
                num3 = 1;
            }
        }
        else if (num1!=num2)
        {
            zoom_line_yuv(p3,tmpbuf,dec_w,scalex);
            zoom_line_yuv(p4,ptmp2,dec_w,scalex);
            p1 = tmpbuf;
            p2 = ptmp2;
            num3 = 1;
        }				
        wb = w&MASK;
        wa = Q15 - wb;
				
        p3 = p1;
        p4 = p2;
        p5 = p1 + dec_w;
        p6 = p2 + dec_w;

        for (i=0;i<(dec_w>>1);i++)
        {
            //*pb++ = (*p1++)*wa + (*p2++)*wb;
            y = *p3++;
            u = *p5++;
            y1 = *p4++;
            u1 = *p6++;

            y = ((y*wa + y1*wb)>>S15);
            u = ((u*wa + u1*wb)>>S15);

            *pb++ = (u<<8)|(y);
            /////////////////////////////
            y = *p3++;
            v = *p5++;
            y1 = *p4++;
            v1 = *p6++;

            y = ((y*wa + y1*wb)>>S15);
            v = ((v*wa + v1*wb)>>S15);
            *pb++ = (v<<8)|(y);
        }

        w+=scaley;
        num2 = num1;
    }

    mmc_MemFreeLast((char*)tmpbuf);

    return 0;
}

VOID lcd_BlockWriteBypass(UINT16* buffer, UINT16 startx,UINT16 starty,UINT16 endx,UINT16 endy)
{

}

//UINT16 capture_buffer[240*320];
int Lily_Camera_PowerOn(INT32  videomode, void(*cb)(int32))
{
    UINT16* InvalideAddress = (UINT16*)convertaddr(NULL);
    UINT8 camId = 0xff;

    if (s_mmcTaskState != MMC_TASK_STOPPED)
        return MDI_RES_CAMERA_ERR_FAILED;

    mmc_SetCurrMode(MMC_MODE_CAMERA);
    camera_Reserve(NULL);
    // note here: just power on the sensor but don't start the preview
    camera_PowerOn(CAM_NPIX_QVGA, 128, 160, CAM_FORMAT_RGB565);
    camId = camera_GetId();
    if (camId == 0)
    {
        camera_PowerOff();
        return MDI_RES_CAMERA_ERR_HW_NOT_READY;
    }

    srcwidth = MMC_CAM_SRC_WIDTH;
    srcheight = MMC_CAM_SRC_HEIGHT;
    sensorbuf = (UINT16*)convertaddr(mmc_MemMalloc(srcwidth*srcheight*4));

    if (sensorbuf == InvalideAddress)
    {
        camera_PowerOff();
        return MDI_RES_CAMERA_ERR_MEMORY_NOT_ENOUGH;
    }
    blendBuffer = sensorbuf + srcwidth*srcheight;

    bset = 0;
    s_mmcTaskState = MMC_TASK_INITIALIZED;
    return MDI_RES_CAMERA_SUCCEED;
}

int Lily_Camera_PowerOff()
{
    if (s_mmcTaskState == MMC_TASK_CAPTURING)
        while (sensorready == 0)
            sxr_Sleep(16);

    Lily_Camera_PreviewOff();
    bset = 2;
    mmc_SetCurrMode(MMC_MODE_IDLE);
    camera_PowerOff();
    mmc_MemFreeAll();

    s_mmcTaskState = MMC_TASK_STOPPED;
    return MDI_RES_CAMERA_SUCCEED;
}
extern VOID pas6175_preview(UINT16 Width, UINT16 Height);

int Lily_Camera_Preview(CAM_PREVIEW_STRUCT* data)
{
    if (s_mmcTaskState != MMC_TASK_INITIALIZED)
        return MDI_RES_CAMERA_ERR_FAILED;

    mci_LcdStartBypass(lcd_BlockWriteBypass);

    CameraInfo.start_x = data->start_x;
    CameraInfo.start_y = data->start_y;
    CameraInfo.end_x = data->end_x;
    CameraInfo.end_y = data->end_y;
    CameraInfo.preview_width = data->end_x - data->start_x + 1;
    CameraInfo.preview_height = data->end_y - data->start_y + 1;
    CameraInfo.image_height = data->image_height;
    CameraInfo.image_width = data->image_width;
    CameraInfo.preview_rotate=0;
    CameraInfo.nightmode = data->nightmode;

    cutwidth = srcwidth;
    cutheight = srcwidth * CameraInfo.preview_height/ CameraInfo.preview_width;
    if(cutheight > srcheight)
    {
        cutheight = srcheight;
        cutwidth = srcheight * CameraInfo.preview_width/CameraInfo.preview_height;
    }
    startPos = srcwidth*((srcheight-cutheight)>>1) + ((srcwidth-cutwidth)>>1);

    displayFbw.fb.buffer = NULL;
    displayFbw.fb.width = MMC_LcdWidth;
    displayFbw.fb.height = MMC_LcdHeight;
    displayFbw.fb.colorFormat = LCDD_COLOR_FORMAT_RGB_565;
    displayFbw.roi.width = MMC_LcdWidth;
    displayFbw.roi.height = MMC_LcdHeight;
    displayFbw.roi.x = 0;
    displayFbw.roi.y = 0;

    pas6175_preview(128, 160);

    if(bset==0)
    {
        COS_SetTimerEX(GetMCITaskHandle(MBOX_ID_MMC), MMC_CAMERA_TIMER_ID, COS_TIMER_MODE_SINGLE, 200);        
        bset = 1;
    }

    s_mmcTaskState = MMC_TASK_PREVIEWING;
    return MDI_RES_CAMERA_SUCCEED;
}

int Lily_Camera_PreviewOff()
{
    if (s_mmcTaskState != MMC_TASK_PREVIEWING)
        return MDI_RES_CAMERA_ERR_FAILED;

    mci_LcdStopBypass();

    s_mmcTaskState = MMC_TASK_INITIALIZED;
    return MDI_RES_CAMERA_SUCCEED;
}

#define CAPTURE_IMAGE_8M 240*320

int Lily_Camera_Capture(CAM_CAPTURE_STRUCT *data) 
{
    COS_EVENT ev = {0, };

    if (s_mmcTaskState == MMC_TASK_STOPPED)
        return MDI_RES_CAMERA_ERR_FAILED;

    Lily_Camera_PreviewOff();

    sensorready = 0;
    ev.nEventId = MSG_MMC_BAL_CAPTURE_REQ;
    ev.nParam1 = (UINT32)sensorbuf;
    ev.nParam2 = CAPTURE_IMAGE_8M;
    COS_SendEvent(BAL_TH(COS_BK_TASK_PRI), &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);

    //camera_SetFlashL(CAM_FLASH_FORCED);
    while (sensorready == 0)
    {
        //sxr_Sleep(500*16384/1000);
        sxr_Sleep(8192);
    }
    
    return MDI_RES_CAMERA_SUCCEED;
}


int Lily_Capture_Preview()
{
    HAL_GOUDA_VID_LAYER_DEF_T gouda_vid_def = {0, };  
    LCDD_ERR_T err = LCDD_ERR_NO;
    UINT16 tmpw = CameraInfo.preview_width;
    UINT16 tmph = CameraInfo.preview_height;

    gouda_vid_def.fmt = HAL_GOUDA_IMG_FORMAT_YUYV;
    gouda_vid_def.addrY = (UINT32*)(sensorbuf+startPos);
    gouda_vid_def.width =  cutwidth;
    gouda_vid_def.height = cutheight;
    gouda_vid_def.stride = srcwidth*2;

    gouda_vid_def.alpha = 0xFF;
    gouda_vid_def.cKeyColor = 0;
    gouda_vid_def.cKeyEn = FALSE;
    gouda_vid_def.cKeyMask = HAL_GOUDA_CKEY_MASK_OFF;
    gouda_vid_def.depth = HAL_GOUDA_VID_LAYER_BETWEEN_2_1;

    gouda_vid_def.pos.tlPX = CameraInfo.start_x;
    gouda_vid_def.pos.tlPY = CameraInfo.start_y;
    gouda_vid_def.pos.brPX  = CameraInfo.end_x;
    gouda_vid_def.pos.brPY  = CameraInfo.end_y;

    while(hal_GoudaIsActive());
    hal_GoudaVidLayerClose();
    hal_GoudaVidLayerOpen(&gouda_vid_def);

    memset((UINT8*)blendBuffer, 0, srcwidth*srcheight*2);
    displayFbw.fb.buffer = blendBuffer; 
    err = lcdd_Blit16(&displayFbw,0 ,0);
    while (LCDD_ERR_NO != err)
    {
        sxr_Sleep(500);
        err = lcdd_Blit16(&displayFbw,0 ,0);
    };
    hal_GoudaVidLayerClose();

    return MDI_RES_CAMERA_SUCCEED;
}

int Lily_camera_savePhoto(int  file_handle)
{
    int len = 0;
    int zoomRes = 1;
    UINT16 dec_w = 0, dec_h = 0;

    if (file_handle < 0)
    {
        MCI_TRACE(MCI_CAMERA_TRC|TSTDOUT, 0, "[MMC_CAMERA_ERROR]FILE HANDLE IS INVALIBLE!");
        return MDI_RES_CAMERA_ERR_FAILED;
    }

    initVocJpegEncode();
    if ((CameraInfo.image_width < srcwidth) && (CameraInfo.image_height < srcheight))
    {
        dec_w = CameraInfo.image_width;
        dec_h = CameraInfo.image_height;
    }
    else
    {
        dec_w = 320;
        dec_h = 240;
    }

    zoomRes = Coolsand_bilinerz_zoom_op_yuv(sensorbuf, srcwidth, srcheight, cutwidth, cutheight, 
                                        blendBuffer, dec_w, dec_h);
    if (zoomRes)
    {
        quitVocJpegEncode();
        return MDI_RES_CAMERA_ERR_DISK_FULL;
    }
    MMC_jpgEn(CameraInfo.image_width, CameraInfo.image_height, CameraInfo.image_quality, 
                                    (char*)convertaddr(blendBuffer), (char*)sensorbuf, 1);    

#if 0    
    if ((CameraInfo.image_width < srcwidth) && (CameraInfo.image_height < srcheight))
    {
        zoomRes = Coolsand_bilinerz_zoom_op_yuv(sensorbuf, srcwidth, srcheight, cutwidth, cutheight, 
                                        blendBuffer, CameraInfo.image_width, CameraInfo.image_height);
        if (zoomRes)
        {
            quitVocJpegEncode();
            return MDI_RES_CAMERA_ERR_DISK_FULL;
        }
        MMC_jpgEn(CameraInfo.image_width, CameraInfo.image_height, CameraInfo.image_quality, 
                                        (char*)convertaddr(blendBuffer), (char*)sensorbuf, 1);    
    }
    else
    {
        if ((cutwidth != srcwidth) || (cutheight != srcheight))
        {
            zoomRes = Coolsand_VGA_zoom_op_yuv(sensorbuf, srcwidth, srcheight,
                                        cutwidth, cutheight, blendBuffer, 320, 240);
            if (zoomRes)
            {
                quitVocJpegEncode();
                return MDI_RES_CAMERA_ERR_DISK_FULL;
            }
        }

        MMC_jpgEn(CameraInfo.image_width, CameraInfo.image_height, CameraInfo.image_quality,
                                        (char*)convertaddr(blendBuffer), (char*)sensorbuf, 1);
    }
#endif

    len= FS_Write(file_handle, (uint8*)sensorbuf + 1, getJpegEncodeLen());
    quitVocJpegEncode();

    if (len <= 0)
    {
        return MDI_RES_CAMERA_ERR_DISK_FULL;
    }
    return MDI_RES_CAMERA_SUCCEED;
}

int Lily_Camera_SetEffect(CAM_SET_PARA effectCode,uint32 value)
{
    uint32 localEffectCode=0;
    uint32 localValue=0;    
    UINT16 tmpw = 0;
    UINT16 tmph = 0;

    switch (effectCode)
    {
        case CAM_PARAM_CONTRAST:
            localEffectCode = CameraSetContrast;
            localValue =5- value;
            if(localValue < 0)
            {
                localValue = 0;
            }
            camera_SetContrastL(localValue);
            break;
            
        case CAM_PARAM_BRIGHTNESS:
            localEffectCode = CameraSetBrightness;
            localValue = value+1;
            if(localValue > 5)
            {
                localValue = 5;
            }
            camera_SetBrightnessL(localValue);
            break;
            
        case CAM_PARAM_EFFECT:
            switch(value)
            {
                case MDI_CAMERA_EFFECT_NORMAL:
                    localEffectCode = CAM_SPE_NORMAL;
                    break;
                case MDI_CAMERA_EFFECT_ANTIQUE:
                    localEffectCode = CAM_SPE_ANTIQUE;
                    break;
                case MDI_CAMERA_EFFECT_REDRISH:
                    localEffectCode = CAM_SPE_REDISH;
                    break;
                case MDI_CAMERA_EFFECT_GREENRISH:
                    localEffectCode = CAM_SPE_GREENISH;
                    break;
                case MDI_CAMERA_EFFECT_BLUERISH:
                    localEffectCode = CAM_SPE_BLUEISH;
                    break;
                case MDI_CAMERA_EFFECT_BLACKWHITE:
                    localEffectCode = CAM_SPE_BLACKWHITE;
                    break;
                case MDI_CAMERA_EFFECT_NEGATIVENESS:
                    localEffectCode = CAM_SPE_NEGATIVE;
                    break;
                default:
                    MCI_TRACE(MCI_CAMERA_TRC|TSTDOUT, 0, "[MMC_CAMERA_ERROR]ERROR CAM_PARAM_EFFECT");
                    localEffectCode = MDI_CAMERA_EFFECT_NORMAL;
                    break;
            }
            MCI_TRACE(MCI_CAMERA_TRC, 0, "[MMC_CAMERA]localEffectCode=%d",localEffectCode);
            camera_SetSpecialEffect(localEffectCode);
            break;
            
        case CAM_PARAM_WB:
            MCI_TRACE(MCI_CAMERA_TRC, 0, "[MMC_CAMERA]CAM_PARAM_WB=%d",value);
            switch(value)
            {
                case 0:
                    camera_SetWhiteBalanceL(CAM_WB_AUTO); /*Auto */
                    break;
                case 1:
                    camera_SetWhiteBalanceL(CAM_WB_DAYLIGHT);
                    break;
                case 2:
                    camera_SetWhiteBalanceL(CAM_WB_TUNGSTEN);
                    break;
                case 3:
                    camera_SetWhiteBalanceL(CAM_WB_OFFICE);
                    break;
                case 4:
                    camera_SetWhiteBalanceL(CAM_WB_CLOUDY);
                    break;				
                default:
                    camera_SetWhiteBalanceL(CAM_WB_AUTO); /*Auto */
                    break;
            }
            break;
            
        case CAM_PARAM_IMAGESIZE:
            if(MY_NPIX != CAM_NPIX_QQVGA)//camera
            {
                switch(value)
                {
                    case 1:
                        hal_HstSendEvent(0xbaba0001);
                        CameraInfo.image_width =  MMC_LcdWidth;
                        CameraInfo.image_height = MMC_LcdHeight;
                        CameraInfo.preview_width = MMC_LcdWidth;
                        CameraInfo.preview_height= MMC_LcdHeight;
                        CameraInfo.start_x = 0;
                        CameraInfo.start_y = 0;
                        break;
                    case 2:
                        hal_HstSendEvent(0xbaba0002);
                        CameraInfo.image_width =  176;
                        CameraInfo.image_height = 220;
                        CameraInfo.preview_width = MMC_LcdWidth;
                        CameraInfo.preview_height= MMC_LcdHeight;
                        CameraInfo.start_x = 0;
                        CameraInfo.start_y = 0;
                        break;
                    case 3:
                        hal_HstSendEvent(0xbaba0003);
                        CameraInfo.image_width =  320;
                        CameraInfo.image_height = 240;
                        CameraInfo.preview_width = MMC_LcdWidth;
                        CameraInfo.preview_height= 
                                    (MMC_LcdWidth*CameraInfo.image_height /CameraInfo.image_width)&0xfffffffe;
                        CameraInfo.start_x = 0;
                        CameraInfo.start_y = (MMC_LcdHeight - CameraInfo.preview_height) >> 1;
                        break;
                    default:
                        hal_HstSendEvent(0xbaba0004);                        
                        hal_HstSendEvent(value);                        
                        CameraInfo.image_width =  MMC_LcdWidth;
                        CameraInfo.image_height = MMC_LcdHeight;
                        break;
                }
                
                tmpw = CameraInfo.preview_width;
                tmph = CameraInfo.preview_height;
                cutwidth = srcwidth;
                cutheight = srcwidth * tmph / tmpw;
                if(cutheight > srcheight)
                {
                    cutheight = srcheight;
                    cutwidth = srcheight * tmpw/tmph;
                }
                startPos = srcwidth*((srcheight-cutheight)>>1) + ((srcwidth-cutwidth)>>1);
            }
            break;
            
        default:
            break;
    }
	//return 0;
}

void Camera_UserMsg(COS_EVENT *pnMsg)
{
    MCI_TRACE(MCI_CAMERA_TRC, 0, "[MMC_CAMERA]Camera_UserMsg pnMsg->nEventId:%d ",pnMsg->nEventId);

    switch (pnMsg->nEventId)
    {
        case EV_TIMER:
            switch(pnMsg->nParam1)
            {
                case MMC_CAMERA_TIMER_ID:
                    if (bset < 2) //bset==2 means the camera is power off
                        Lily_Camera_SetEffect(CAM_PARAM_WB, CameraInfo.nightmode);
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
}

void set_multicaputure(int i)
{
    // TODO: reserved for future use
}


void set_mmc_camera_preview()
{
    // TODO: reserved for future use
}

#else

#include "cs_types.h"
#include "global.h"
#include "mcip_debug.h"
#include "lcd_if.h"
#include "hal_timers.h"
#include "hal_gouda.h"
#include "event.h"
#include "sxr_tls.h"
#include "global_macros.h"
#include "sys_ctrl.h"

#include "mci_lcd.h"
#include "lcdd_m.h"
#include "mmc.h"
#include "mci.h"
#include "mmc_timer.h"
#include "fs.h"

#include "vpp_preview.h"
#include "camera_m.h"
#include "pmd_m.h"
#include "mmc_camera.h"
#include "mmc_previewvoc.h"
#include "mmc_jpegencvoc.h"
#include "vpp_audiojpeg_enc.h"

#include "mmc_videorecord.h"

#ifdef _ATV_SPLIT_
#undef _ATV_SPLIT_  // this functionality is disabled now
#endif

extern void mmc_atvIntraFrameAveAsm(UINT32* imgBuf, UINT32 srcwidth, UINT32 cutwidth, UINT32 cutheight);

// pointer to the memory which stores the image sent out by ISP
LCDD_FBW_T displayFbw;

uint16* sensorbuf = NULL;
uint16* blendBuffer = NULL;
// recors the width and the height of the image sent out by ISP
volatile uint16 srcwidth=0, srcheight=0; // the width and height of the image sent out by ISP
volatile uint16 cutwidth=0, cutheight=0; // the width and height of the image to be zoomed
volatile uint32 startPos = 0;   // the start pixel to be previewed
volatile uint16 bset=0; // 0 - special effect was not initiated, 1 - special effect was initiated  
volatile uint16 flash_enable=0; // 1 - flash available; 0 - flash not available
volatile int32 previewstop=0; // 0 - in preview; 1 - preview stopped
volatile int32 sensorready=0;
volatile int32 countdown =3;
volatile CAM_INFO_STRUCT CameraInfo = {0, };

CAM_SIZE_T MY_NPIX = CAM_NPIX_VGA;

static short yuvmode=0;
#ifdef _CAM_SMALL_MEM_
static UINT16 turn = 0;
static UINT16* bufPointer[2] = {NULL, NULL};
static BOOL s_vocinited = FALSE;
#endif
static int multicapture=0;
static int previewstatus=0;
static uint32 cam_wbValue = 0;

volatile int32 Effect_Set_Valid;

// this variable is not used now for gallite, and kept here just for the compatibility with greenstone
MMC_CAMERA_CB mmc_camCB; 

#define   Q15      32767
#define   Q14      16384
#define   S15      15
#define   MASK     (Q15-1)

VOID lcd_BlockWriteBypass(UINT16* buffer, UINT16 startx,UINT16 starty,UINT16 endx,UINT16 endy)
{
}

#define AVERAGE_FRAME_NUM 3
#if 0
#ifdef _CAM_SMALL_MEM_
#define YUV_MASK 0xFEFEFEFE
PRIVATE void frameAverageYuv422 (UINT16* pBufTobeAdded, UINT16* pBufToAdd)
{
    UINT8* pBuf1 = (UINT32*)pBufTobeAdded;
    UINT8* pBuf2 = (UINT32*)pBufToAdd;
    UINT32 size = (srcwidth*srcheight)>>1;

    while (size--)
    {
        pBuf1[0] = (pBuf1[0]>>1)+(pBuf2[0]>>1);
        pBuf1[1] = (pBuf1[1]>>1)+(pBuf2[1]>>1);
        pBuf1[2] = (pBuf1[2]>>1)+(pBuf2[2]>>1);
        pBuf1[3] = (pBuf1[3]>>1)+(pBuf2[3]>>1);
        pBuf1 += 4;
        pBuf2 += 4;
    }
}
#endif
#endif

#ifdef __MMI_ANALOGTV__ 
#include "atvd_m.h"
extern void atvif_ISPSettingInit(void);
extern void atvif_PreviewSettingInit(void);
extern ATV_ISP_SETTING_T* atvif_GetISPSetting(void);
static ATV_ISP_SETTING_T* Atv_Isp_Settint=NULL;
#ifdef _ATV_SPLIT_
static INT16 s_atvBlendCtrl = -1;
#endif
#endif

int Lily_Camera_PowerOn(INT32  videomode, void(*cb)(int32))
{
    UINT16* InvalideAddress = (UINT16*)convertaddr(NULL);
    UINT8 camId = 0xff;

    MCI_TRACE(MCI_CAMERA_TRC|TSTDOUT, 0,  "[MMC_CAMERA]Lily_Camera_PowerOn");

#ifdef __MMI_ANALOGTV__
	if (atvif_GetCurModule() == 1)
    {
        mmc_SetCurrMode(MMC_MODE_CAMERA);
        atvif_ISPSettingInit();
		Atv_Isp_Settint = atvif_GetISPSetting();
        MY_NPIX = CAM_NPIX_QVGA;

        srcwidth = Atv_Isp_Settint->SrcWidth;
        srcheight = Atv_Isp_Settint->Srcheight;
        camera_Reserve(Cam_CallbackFun);
    }
    else
#endif
    {
        // Camera mode
        if (videomode == 0)
        {
            MY_NPIX = CAM_NPIX_VGA;
            mmc_SetCurrMode(MMC_MODE_CAMERA);
#ifdef _CAM_SMALL_MEM_
            srcwidth = 320;
            srcheight = 240;
#else
            srcwidth = 640;
            srcheight = 480;
#endif
            camera_Reserve(Cam_CallbackFun);
	     camera_GetImageInfo(&MY_NPIX,&srcwidth,&srcheight);
        }
        // Video Recoder mode
        else
        {
#ifdef VIDEO_RECORDER_SUPPORT
            MY_NPIX = CAM_NPIX_QQVGA;
            srcwidth = 160;
            srcheight = 120;
            camera_Reserve(VidRec_CallbackFun);
#else
            return MDI_RES_CAMERA_ERR_HW_NOT_READY;
#endif
        }
    }
    camera_PowerOn(MY_NPIX, srcwidth, srcheight, CAM_FORMAT_RGB565);

    sensorbuf = NULL;
#ifdef __MMI_ANALOGTV__ 
    if(atvif_GetCurModule() == 1)
    {
        sensorbuf = (UINT16*)convertaddr(mmc_MemMalloc(360*288*2));
        //blendBuffer = (UINT16*)convertaddr(mmc_MemMalloc(320*240*4));
    }
    else if(videomode == 0)
#else
    if (videomode == 0)
#endif
    {
        blendBuffer = NULL;
#ifdef _CAM_SMALL_MEM_
        sensorbuf = (UINT16*)convertaddr(mmc_MemMalloc(srcwidth*srcheight*4));
        blendBuffer= sensorbuf + srcwidth*srcheight;
        bufPointer[0] = sensorbuf;
        bufPointer[1] = blendBuffer;
#else
        sensorbuf = (UINT16*)convertaddr(mmc_MemMalloc(srcwidth*srcheight*2));
        blendBuffer = (UINT16*)convertaddr(mmc_MemMalloc(320*240*4));
#endif        
    }
#ifdef VIDEO_RECORDER_SUPPORT
    else
    {
        sensorbuf = (UINT16*)convertaddr(mmc_MemMalloc(srcwidth*srcheight*2));
    }
#endif

    if (sensorbuf == InvalideAddress)
    {
        camera_PowerOff();
        MCI_TRACE(MCI_CAMERA_TRC|TSTDOUT, 0, "[MMC_CAMERA]Lily_Camera_Preview lack of memory,sensorbuf=%d",sensorbuf);
        return  MDI_RES_CAMERA_ERR_MEMORY_NOT_ENOUGH;
    }

    multicapture = 0;
    bset = 0;

    // Initialize Frame Buffer Window
    // Buffer pointer will be set at the time of display
    // Other parameters are constant
    displayFbw.fb.buffer = NULL;
    displayFbw.fb.width = MMC_LcdWidth;
    displayFbw.fb.height = MMC_LcdHeight;
    displayFbw.fb.colorFormat = LCDD_COLOR_FORMAT_RGB_565;
    displayFbw.roi.width = MMC_LcdWidth;
    displayFbw.roi.height = MMC_LcdHeight;
    displayFbw.roi.x = 0;
    displayFbw.roi.y = 0;

    camId = camera_GetId();
    hal_HstSendEvent(camId);
    if (camId==0)
    {
        MCI_TRACE(MCI_CAMERA_TRC, 0, "[MMC_CAMERA]Invalide sensor id: %d", camId);
        Lily_Camera_PowerOff();
        return MDI_RES_CAMERA_ERR_HW_NOT_READY;
    }
    mmc_camCB = cb;

#ifdef _CAM_SMALL_MEM_
#ifdef __MMI_ANALOGTV__ 
    if(atvif_GetCurModule() != 1)
#endif
    {
        hwp_sysCtrl->Cfg_Clk_Voc = SYS_CTRL_VOC_FREQ_78M;
        if (videomode == 0)
        {
            initVocPreviewYUV();  
            s_vocinited = TRUE;
        }
    }
#endif
    
    return MDI_RES_CAMERA_SUCCEED;
}

int Lily_Camera_PowerOff()
{
    MCI_TRACE(MCI_CAMERA_TRC|TSTDOUT, 0, "[MMC_CAMERA]Lily_Camera_PowerOff");

    countdown = 0;
    previewstop = 1;
    bset = 2;

#ifdef __MMI_ANALOGTV__
    if(atvif_GetCurModule() != 1)
#endif
    {
        if (CameraInfo.nightmode == 1 && flash_enable == 1)
        {
            camera_SetFlashL(CAM_FLASH_NONE);
        }
    }

#ifdef _CAM_SMALL_MEM_
#ifdef __MMI_ANALOGTV__ 
    if(atvif_GetCurModule() != 1)
#endif
    {
        hwp_sysCtrl->Cfg_Clk_Voc = SYS_CTRL_VOC_FREQ_52M;
        if (s_vocinited == TRUE)
        {
            quitPreview();
            s_vocinited = FALSE;
        }
    }
#endif
    mmc_SetCurrMode(MMC_MODE_IDLE);
    Lily_Camera_PreviewOff();
    camera_PowerOff();
    mmc_MemFreeAll();

    sensorbuf = NULL;
    blendBuffer = NULL;
#ifdef _CAM_SMALL_MEM_
    bufPointer[0] = bufPointer[1] = NULL;
#endif
    
    return MDI_RES_CAMERA_SUCCEED;
}

int Lily_Camera_Preview(CAM_PREVIEW_STRUCT* data)
{
    MCI_TRACE(MCI_CAMERA_TRC|TSTDOUT, 0, "[MMC_CAMERA]Lily_Camera_Preview");

#ifdef __MMI_ANALOGTV__ 
    if(atvif_GetCurModule() == 1)
    {
        atvif_PreviewSettingInit();
        Atv_Isp_Settint = atvif_GetISPSetting();
        srcwidth = Atv_Isp_Settint->SrcWidth;
        srcheight = Atv_Isp_Settint->Srcheight;
#ifdef _ATV_SPLIT_
        s_atvBlendCtrl = -1;
#endif		
        countdown = 0;
    }	
    else
#endif
        countdown = 2;

    CameraInfo.start_x = data->start_x;
    CameraInfo.start_y = data->start_y;
    CameraInfo.end_x = data->end_x;
    CameraInfo.end_y = data->end_y;
    CameraInfo.preview_width = data->end_x - data->start_x + 1;
    CameraInfo.preview_height = data->end_y - data->start_y + 1;
    CameraInfo.image_height = data->image_height;
    CameraInfo.image_width = data->image_width;
    CameraInfo.preview_rotate=0;
    CameraInfo.nightmode = data->nightmode;

    cutwidth = srcwidth;
    cutheight = srcwidth * CameraInfo.preview_height/ CameraInfo.preview_width;
    if(cutheight > srcheight)
    {
        cutheight = srcheight;
        cutwidth = srcheight * CameraInfo.preview_width/CameraInfo.preview_height;
    }
    cutheight = cutheight*10/(10+2*(CameraInfo.previewZoom));
    cutwidth = cutwidth*10/(10+2*(CameraInfo.previewZoom));   

#ifndef _CAM_SMALL_MEM_
#ifdef __MMI_ANALOGTV__ 
    if(atvif_GetCurModule() != 1)
#endif        
    {    
        if ((MY_NPIX != CAM_NPIX_QQVGA)
                && ((MMC_LcdWidth == 128) || (MMC_LcdWidth == 160))
                &&((srcwidth > srcheight)==(CameraInfo.preview_width > CameraInfo.preview_height)))
        {
            cutwidth = (cutwidth>>1) + (cutwidth>>2);
            cutheight = (cutheight>>1) + (cutheight>>2);
        }
    }
#endif
    startPos = ((srcheight - cutheight)/2)*srcwidth + (srcwidth - cutwidth)/2;

#ifdef __MMI_ANALOGTV__ 
    if(atvif_GetCurModule() == 1)
    {    
        CameraInfo.preview_rotate=data->preview_rotate;	//xiaoyifeng add  10/11/1
    }
#endif

    flash_enable = data->flashenable;
    previewstop = 0;
    previewstatus = 1;
    sensorready = 0;

    if (CameraInfo.preview_rotate)
    {
        MCI_TRACE(MCI_CAMERA_TRC, 0, "[MMC_CAMERA]Lily_Camera_Preview_SetDirRotation ");
        lcdd_SetDirRotation();
    }

#ifdef __MMI_ANALOGTV__ 
    if (CameraInfo.preview_rotate)
    {
        MCI_TRACE(MCI_CAMERA_TRC, 0, "[MMC_CAMERA]Lily_Camera_Preview_SetDirRotation ");
        displayFbw.fb.buffer = NULL;
        displayFbw.fb.width =  srcwidth;
        displayFbw.fb.height = srcheight;
        displayFbw.fb.colorFormat = LCDD_COLOR_FORMAT_RGB_565;
        displayFbw.roi.width =	MMC_LcdHeight;
        displayFbw.roi.height = MMC_LcdWidth;
        displayFbw.roi.x = 0;
        displayFbw.roi.y = 0;
        lcdd_SetDirRotation();
    }
#endif

    if(bset==0)
    {
        Lily_Camera_SetEffect(CAM_PARAM_ADD_FRAME,data->addFrame);
        Lily_Camera_SetEffect(CAM_PARAM_QUALITY,data->imageQuality);
        Lily_Camera_SetEffect(CAM_PARAM_ZOOM_FACTOR,data->factor);
	
        cam_wbValue = data->whiteBlance;
        COS_SetTimerEX(GetMCITaskHandle(MBOX_ID_MMC), MMC_CAMERA_TIMER_ID, COS_TIMER_MODE_SINGLE, 200);

        Lily_Camera_SetEffect(CAM_PARAM_EFFECT,data->specialEffect);
        Lily_Camera_SetEffect(CAM_PARAM_NIGHT_MODE,data->nightmode);
        Lily_Camera_SetEffect(CAM_PARAM_CONTRAST,data->contrast);
        Lily_Camera_SetEffect(CAM_PARAM_BRIGHTNESS,data->brightNess);
        bset = 1;
    }

    MCI_TRACE(MCI_CAMERA_TRC, 0, "[MMC_CAMERA]Lily_Camera_Preview and before camera_PrepareImageCaptureL");

    mci_LcdStartBypass(lcd_BlockWriteBypass);

#ifdef __MMI_ANALOGTV__
    if(atvif_GetCurModule() != 1)
#endif
    {
        if (CameraInfo.nightmode == 1 && flash_enable == 1)
        {
            camera_SetFlashL(CAM_FLASH_FORCED);
        }
    }

#ifdef _CAM_SMALL_MEM_    
    turn = 0;    
#endif
    if (srcwidth != 160)
        yuvmode = 1;
    camera_PrepareImageCaptureL(CAM_FORMAT_RGB565, (srcwidth*srcheight), sensorbuf);
#ifdef __MMI_ANALOGTV__ 
    if(atvif_GetCurModule() == 1)
    {
        camera_CaptureImage();
    }
#endif

    return MDI_RES_CAMERA_SUCCEED;
}

int Lily_Camera_PreviewOff(void)
{
    int res;

    MCI_TRACE(MCI_CAMERA_TRC|TSTDOUT, 0, "[MMC_CAMERA]Lily_Camera_PreviewOff");

    //xiaoyifeng add  10/11/1
#ifdef __MMI_ANALOGTV__ 
    if (CameraInfo.preview_rotate)
    {
        MCI_TRACE(MCI_CAMERA_TRC, 0, "[MMC_CAMERA]Lily_Camera_Preview_SetDirDefault ");
        lcdd_SetDirDefault();
    }	
#endif
    //xiaoyifeng end

#ifdef __MMI_ANALOGTV__
    if(atvif_GetCurModule() != 1)
#endif
    {
        if (CameraInfo.nightmode == 1 && flash_enable == 1)
        {
            camera_SetFlashL(CAM_FLASH_NONE);
        }
    }

    previewstop = 1;
    previewstatus = 0;

    res = COS_KillTimerEX(GetMCITaskHandle(MBOX_ID_MMC), MMC_CAMERA_TIMER_ID);
    MCI_TRACE(MCI_VIDEO_TRC, 0, "[MMC_CAMERA]KILL TIMER res=%d", res);

    mci_LcdStopBypass();

    while(hal_GoudaIsActive());
    hal_GoudaVidLayerClose();
    return MDI_RES_CAMERA_SUCCEED;
}

void Cam_CallbackFun(UINT8 id,  UINT8 ActiveBufNum)
{
    MCI_TRACE(MCI_CAMERA_TRC, 0, "[MMC_CAMERA]Cam_CallbackFun id:%d",id);

    if (id == 1)
        hal_HstSendEvent(0xe0);

    if (previewstop == 0) // in preview
    {
        if (countdown > 0 || id != 0)
        {
            if (countdown > 0) countdown--;

            {
#ifdef __MMI_ANALOGTV__
                if(atvif_GetCurModule() == 1)
                {
#ifdef _ATV_SPLIT_				
                    s_atvBlendCtrl = -1;
#endif					
                    camera_PrepareImageCaptureL(CAM_FORMAT_RGB565, (srcwidth*srcheight), sensorbuf);
                    camera_CaptureImage();
                }
                else
#endif
                {
#ifdef _CAM_SMALL_MEM_
                    turn = 1 - turn;
                    camera_PrepareImageCaptureL(CAM_FORMAT_RGB565, (srcwidth*srcheight), bufPointer[turn]);
#else
                    camera_PrepareImageCaptureL(CAM_FORMAT_RGB565, (srcwidth*srcheight), sensorbuf);
#endif
                }
            } 
            return;
        }
        
        mmc_SendEvent(GetMCITaskHandle(MBOX_ID_MMC), MSG_CAM_CALLBACK);
#ifdef _CAM_SMALL_MEM_
#ifdef __MMI_ANALOGTV__
        if(atvif_GetCurModule() != 1)
#endif
        {
            turn = 1 - turn;
            camera_PrepareImageCaptureL(CAM_FORMAT_RGB565, (srcwidth*srcheight), bufPointer[turn]);
        } 
#endif
    }
    else
    {
        if (id == 1)
        {
            camera_CaptureImage();
            return;
        }
//#ifndef _CAM_SMALL_MEM_	
        if (--countdown > 0)
        {
#ifdef __MMI_ANALOGTV__
            if(atvif_GetCurModule() != 1)
#endif
            {
                camera_CaptureImage();
            } 
            return;
        }
    
        Lily_Camera_PreviewOff();//priew off????
        sensorready = 1;
//#else
//        mmc_SendEvent(GetMCITaskHandle(MBOX_ID_MMC), MSG_CAM_CAPTURE);
//#endif
    }
}

void mmc_CamBlendDsp()
{
    HAL_GOUDA_VID_LAYER_DEF_T gouda_vid_def = {0, };
    LCDD_ERR_T err = LCDD_ERR_NO;

    MCI_TRACE(MCI_CAMERA_TRC, 0, "[MMC_CAMERA]mmc_CamBlendDsp tick: %d",hal_TimGetUpTime());

#ifdef __MMI_ANALOGTV__ 
    if(atvif_GetCurModule() == 1)
    {
#ifdef _ATV_SPLIT_	
        if (s_atvBlendCtrl == -1) 
        {
            s_atvBlendCtrl = 0;
            return;
        }
#endif            
        cutwidth  = Atv_Isp_Settint->ispOutputWidth;
        cutheight = Atv_Isp_Settint->ispOutputheight;  
        startPos  = Atv_Isp_Settint->ispOutputStartPos;	
#ifdef _ATV_SPLIT_
        if (s_atvBlendCtrl == 1)
            startPos += (srcwidth * (cutheight>>1));
#endif
        gouda_vid_def.fmt = HAL_GOUDA_IMG_FORMAT_YUYV;
        gouda_vid_def.addrY = (UINT32*)(sensorbuf+startPos);
        gouda_vid_def.width =  cutwidth;    
#ifdef _ATV_SPLIT_
        gouda_vid_def.height = (cutheight >> 1);
#else
		gouda_vid_def.height = cutheight;
#endif		
        gouda_vid_def.stride = srcwidth*2;
        
#if defined(RDAATV_CHIPVER_5888HE) || defined(RDAATV_CHIPVER_5888H)
       mmc_atvIntraFrameAveAsm((UINT32*)(sensorbuf+startPos),srcwidth,cutwidth,cutheight);
#endif
    }
    else
#endif
    {
#ifdef _CAM_SMALL_MEM_
        CutZoomInStruct   cutzoom = {0, };      
        UINT16 tmpw = CameraInfo.preview_width; 
        UINT16 tmph = CameraInfo.preview_height;

        cutzoom.inStreamBufAddr = (INT32*)bufPointer[1-turn];
        cutzoom.outStreamBufAddr = (INT32*)bufPointer[1-turn];
        cutzoom.blendBufAddr = NULL;
        cutzoom.src_w = srcwidth;
        cutzoom.src_h = srcheight;
        cutzoom.cut_w = cutwidth;
        cutzoom.cut_h = cutheight;
        cutzoom.dec_w = tmpw;
        cutzoom.dec_h = tmph;
        cutzoom.reset = 1;

        MMC_Preview_Wait(&cutzoom);        

        gouda_vid_def.fmt = HAL_GOUDA_IMG_FORMAT_RGB565;
        gouda_vid_def.addrY = (UINT32*)bufPointer[1-turn];
        gouda_vid_def.width =  tmpw;    
        gouda_vid_def.height = tmph;
        gouda_vid_def.stride = tmpw*2;
#else
        gouda_vid_def.fmt = HAL_GOUDA_IMG_FORMAT_YUYV;
        gouda_vid_def.addrY = (UINT32*)(sensorbuf+startPos);
        gouda_vid_def.width =  cutwidth;    
        gouda_vid_def.height = cutheight;
        gouda_vid_def.stride = srcwidth*2;
#endif
    }

    gouda_vid_def.alpha = 0xFF;
    gouda_vid_def.cKeyColor = 0;
    gouda_vid_def.cKeyEn = FALSE;
    gouda_vid_def.cKeyMask = HAL_GOUDA_CKEY_MASK_OFF;
#ifdef __MMI_ANALOGTV__ 
    if(atvif_GetCurModule() == 1)
    {
        gouda_vid_def.depth = HAL_GOUDA_VID_LAYER_OVER_ALL;
    }
    else
#endif
    {
        gouda_vid_def.depth = HAL_GOUDA_VID_LAYER_BETWEEN_2_1;
    }

#ifdef __MMI_ANALOGTV__     
    if (CameraInfo.preview_rotate)
    {
        gouda_vid_def.pos.tlPX = CameraInfo.start_y;
        gouda_vid_def.pos.tlPY = CameraInfo.start_x;
        gouda_vid_def.pos.brPX = CameraInfo.end_y;
        gouda_vid_def.pos.brPY = CameraInfo.end_x;
    }
    else 
#endif        
    {
        gouda_vid_def.pos.tlPX = CameraInfo.start_x;
        gouda_vid_def.pos.tlPY = CameraInfo.start_y; 
        gouda_vid_def.pos.brPX = CameraInfo.end_x;
        gouda_vid_def.pos.brPY = CameraInfo.end_y;
    }

#ifdef _ATV_SPLIT_
#ifdef __MMI_ANALOGTV__ 
    if(atvif_GetCurModule() == 1)
    {
        if (s_atvBlendCtrl == 1)
        {
            gouda_vid_def.pos.tlPY = (gouda_vid_def.pos.tlPY + gouda_vid_def.pos.brPY + 1) >> 1;
        }
        else if (s_atvBlendCtrl == 0)
        {
            gouda_vid_def.pos.brPY = ((gouda_vid_def.pos.tlPY + gouda_vid_def.pos.brPY + 1) >> 1) - 1;
        }
    }
#endif
#endif

    while(hal_GoudaIsActive());
    hal_GoudaVidLayerClose();
    hal_GoudaVidLayerOpen(&gouda_vid_def);

    displayFbw.fb.buffer = NULL;
#ifdef _ATV_SPLIT_	
#ifdef __MMI_ANALOGTV__ 
    if(atvif_GetCurModule() == 1)
    {
        displayFbw.roi.x = gouda_vid_def.pos.tlPX;
        displayFbw.roi.y = gouda_vid_def.pos.tlPY;
        displayFbw.roi.width = gouda_vid_def.pos.brPX - gouda_vid_def.pos.tlPX + 1;
        displayFbw.roi.height = gouda_vid_def.pos.brPY - gouda_vid_def.pos.tlPY + 1;

        if (s_atvBlendCtrl == 1)
        {
            if (CameraInfo.preview_rotate)
                displayFbw.roi.height = MMC_LcdWidth - displayFbw.roi.height;                
            else
                displayFbw.roi.height = MMC_LcdHeight - displayFbw.roi.height;                
        }
    }
#endif
#endif
    lcdd_Blit16(&displayFbw, displayFbw.roi.x , displayFbw.roi.y);
    while (LCDD_ERR_NO != err)
    {
        sxr_Sleep(10);
        err = lcdd_Blit16(&displayFbw,0 ,0);
    };
    
    while(hal_GoudaIsActive());

#ifdef __MMI_ANALOGTV__    
    if(atvif_GetCurModule() == 1)
    {
#ifdef _ATV_SPLIT_	
        if (s_atvBlendCtrl == 0)
        {
#endif		
            camera_PrepareImageCaptureL(CAM_FORMAT_RGB565, (srcwidth*srcheight), sensorbuf);            
            camera_CaptureImage();
#ifdef _ATV_SPLIT_			
        }
        s_atvBlendCtrl = ((s_atvBlendCtrl + 1) & 0x1);
#endif		
    }
    else
#endif        
    {
#ifndef _CAM_SMALL_MEM_
        camera_PrepareImageCaptureL(CAM_FORMAT_RGB565, (srcwidth*srcheight), sensorbuf);
#endif    
    }
}

int Lily_Camera_Capture(CAM_CAPTURE_STRUCT *data) 
{
    MCI_TRACE(MCI_CAMERA_TRC, 0, "[MMC_CAMERA]Lily_Camera_Capture time:%d",hal_TimGetUpTime());
    MCI_TRACE(MCI_CAMERA_TRC, 0, 
            "[MMC_CAMERA]Lily_Camera_Capture data->image_width:%d, data->image_height:%d",
            data->image_width,data->image_height);

    CameraInfo.image_width = data->image_width;
    CameraInfo.image_height = data->image_height; 

    previewstop = 1;	

    if (flash_enable == 1 && CameraInfo.nightmode == 0)
        camera_SetFlashL(CAM_FLASH_FORCED);

    countdown = AVERAGE_FRAME_NUM;
#ifdef _CAM_SMALL_MEM_    
#if 0
    turn = 0;
    camera_PrepareImageCaptureL(CAM_FORMAT_RGB565, (srcwidth*srcheight), blendBuffer);    
#else
    turn = 0;
    camera_PrepareImageCaptureL(CAM_FORMAT_RGB565, (srcwidth*srcheight), sensorbuf);    
#endif
#endif    
    camera_CaptureImage();

    while(sensorready==0)
    {
        sxr_Sleep(10);
    }

    if (flash_enable == 1 && CameraInfo.nightmode == 0)
        camera_SetFlashL(CAM_FLASH_NONE);

    if (multicapture)
    {
        MCI_TRACE(MCI_CAMERA_TRC, 0, "[MMC_CAMERA]cont save photo");
        Lily_Capture_Preview();
    }

    return MDI_RES_CAMERA_SUCCEED;
}

int Lily_Capture_Preview()
{
    HAL_GOUDA_VID_LAYER_DEF_T gouda_vid_def = {0, };  
    LCDD_ERR_T err = LCDD_ERR_NO;
    UINT16 tmpw = CameraInfo.preview_width;
    UINT16 tmph = CameraInfo.preview_height;

    MCI_TRACE(MCI_CAMERA_TRC, 0, "[MMC_CAMERA]Lily_Capture_Preview,multicaputure=%d srcwidth=%d", multicapture, srcwidth);

    previewstatus = 0;

#ifdef _CAM_SMALL_MEM_
    CutZoomInStruct   cutzoom = {0, };      
    
    cutzoom.inStreamBufAddr = (INT32*)sensorbuf;
    cutzoom.outStreamBufAddr = (INT32*)blendBuffer;
    cutzoom.blendBufAddr = NULL;
    cutzoom.src_w = srcwidth;
    cutzoom.src_h = srcheight;
    cutzoom.cut_w = cutwidth;
    cutzoom.cut_h = cutheight;
    cutzoom.dec_w = tmpw;
    cutzoom.dec_h = tmph;
    cutzoom.reset = 1;

    MMC_Preview_Wait(&cutzoom);        

    gouda_vid_def.fmt = HAL_GOUDA_IMG_FORMAT_RGB565;
    gouda_vid_def.addrY = (UINT32*)blendBuffer;
    gouda_vid_def.width =  tmpw;    
    gouda_vid_def.height = tmph;
    gouda_vid_def.stride = tmpw*2;
#else
    gouda_vid_def.fmt = HAL_GOUDA_IMG_FORMAT_YUYV;
    gouda_vid_def.addrY = (UINT32*)(sensorbuf+startPos);
    gouda_vid_def.width =  cutwidth;    
    gouda_vid_def.height = cutheight;
    gouda_vid_def.stride = srcwidth*2;
#endif

    gouda_vid_def.alpha = 0xFF;
    gouda_vid_def.cKeyColor = 0;
    gouda_vid_def.cKeyEn = FALSE;
    gouda_vid_def.cKeyMask = HAL_GOUDA_CKEY_MASK_OFF;
    gouda_vid_def.depth = HAL_GOUDA_VID_LAYER_BETWEEN_2_1;

    gouda_vid_def.pos.tlPX = CameraInfo.start_x;
    gouda_vid_def.pos.tlPY = CameraInfo.start_y;
    gouda_vid_def.pos.brPX  = CameraInfo.end_x;
    gouda_vid_def.pos.brPY  = CameraInfo.end_y;

    while(hal_GoudaIsActive());
    hal_GoudaVidLayerClose();
    displayFbw.fb.buffer =(uint16*)NULL;
    hal_GoudaVidLayerOpen(&gouda_vid_def);

    err = lcdd_Blit16(&displayFbw,0 ,0);
    while (LCDD_ERR_NO != err)
    {
        sxr_Sleep(10);
        err = lcdd_Blit16(&displayFbw,0 ,0);
    };

    while(hal_GoudaIsActive());
    
    return MDI_RES_CAMERA_SUCCEED;
}

void  zoom_line_yuv(uint16* srcbuf,uint16* dec, uint16 dec_w,int scalex)
{
    //input format : y1 u y2 v 
    short wa,wb,i,w;
    int   wn;
    uint16 y,y1,u,u1,v,v1;
    short *pb=dec;
    short *pbu=dec+dec_w;
    uint16 *pa=srcbuf;

    wn = 0;
    for (i=0;i<(dec_w>>1);i++)
    {
        wb =( wn)&(MASK);
        wa = Q15 - wb;

        w  = (wn>>S15);
        /////////u v//////////
        pa = srcbuf + (w<<1);
        u = (*(pa))>>8;
        v = (*(pa+1))>>8;
        u1 = (*(pa+2))>>8;
        v1 = (*(pa+3))>>8;
        /////////y////////////
        pa = srcbuf + w ;
        y = ((*pa++))&0xff;
        y1 = ((*pa))&0xff;

        *pb++ =(y*wa + y1*wb)>>S15;
        *pbu++ = (u*wa+u1*wb)>>S15;
        *pbu++ = (v*wa+v1*wb)>>S15;

        wn+=scalex;
    }

    for (i=0;i<(dec_w>>1);i++)
    {
        wb =( wn)&(MASK);
        wa = Q15 - wb;

        w  = (wn>>S15);

        pa = srcbuf + w ;
        y = ((*pa++))&0xff;
        y1 = ((*pa))&0xff;

        *pb++ =(y*wa + y1*wb)>>S15;

        wn+=scalex;
    }
}

extern void * memcpy(void *,const void *,size_t);
int Coolsand_bilinerz_zoom_op_yuv(uint16* srcbuf,uint16 src_w,uint16 src_h,uint16 cut_w,uint16 cut_h,uint16* decbuf,uint16 dec_w,uint16 dec_h)
{
    short  offx,offy;
    short  wa,wb,i,j;
    int    w,scalex,scaley;
    uint16  *pb ,*pa,*p1,*p2,*p3,*p4,*p5,*p6;
    short y,u,v,y1,u1,v1;
    short*  tmpbuf =(short*)mmc_MemMallocBack(dec_w*8) ;
    short*  ptmp2 = tmpbuf + dec_w*2;
    short  num1,num2,num3;

    if(tmpbuf==NULL)
        return -1;
		
    offx = (src_w - cut_w)>>1;
    offx = (offx>>1)<<1;
    offy = (src_h - cut_h)>>1;
		
    scalex=((cut_w-1)<<S15)/dec_w;
    scaley=((cut_h-1)<<S15)/dec_h;
		
    pa = srcbuf + offy*src_w + offx;
    pb = decbuf;
		
    w  = 0;
    num2 = -2;
    num3 = 1;

    p1 = tmpbuf;
    p2 = ptmp2;

    if (dec_h >= 480)
    {
        short*  PTmp=(short*)mmc_MemMallocBack(320*61*2) ;
        if(PTmp==NULL)
            return -1;

        memcpy(PTmp,decbuf+320*179,320*61*2);//keep one more line
        for (j=0;j<dec_h;j++)
        {
            num1 = (w>>S15);

            if (num1>(178-offy))//for 640*480
            {
                pa = PTmp + offx;
                w = w & MASK;
                num1 = num1-179+offy;
                p3 = pa + (num1*src_w);
                p4 = p3 + src_w;
            }
            else
            {
                p3 = pa + (num1*src_w);
                p4 = p3 + src_w;
            }

            if (num1-num2==1)
            {
                if (num3==1)
                {
                    zoom_line_yuv(p4,tmpbuf,dec_w,scalex);
                    p2 = tmpbuf;
                    p1 = ptmp2;
                    num3=0;
                }
                else
                {
                    zoom_line_yuv(p4,ptmp2,dec_w,scalex);
                    p1 = tmpbuf;
                    p2 = ptmp2;
                    num3 = 1;
                }
            }
            else if (num1!=num2)
            {
                zoom_line_yuv(p3,tmpbuf,dec_w,scalex);
                zoom_line_yuv(p4,ptmp2,dec_w,scalex);
                p1 = tmpbuf;
                p2 = ptmp2;
                num3 = 1;
            }
				
            wb =( w)&(MASK);
            wa = Q15 - wb;
				
            p3 = p1;
            p4 = p2;
            p5 = p1 + dec_w;
            p6 = p2 + dec_w;
            for (i=0;i<(dec_w>>1);i++)
            {
                y = *p3++;
                u = *p5++;
                y1 = *p4++;
                u1 = *p6++;

                y = ((y*wa + y1*wb)>>S15);
                u = ((u*wa + u1*wb)>>S15);
					
                *pb++ = (u<<8)|(y);
                /////////////////////////////
                y = *p3++;
                v = *p5++;
                y1 = *p4++;
                v1 = *p6++;

                y = ((y*wa + y1*wb)>>S15);
                v = ((v*wa + v1*wb)>>S15);
                *pb++ = (v<<8)|(y);
            }

            w+=scaley;
            num2 = num1;
        }

        mmc_MemFreeLast((char*)PTmp);
    }
    else
    {
        for (j=0;j<dec_h;j++)
        {
            num1 = (w>>S15);
            p3 = pa + (num1*src_w);
            p4 = p3 + src_w;
            
            if (num1-num2==1)
            {
                if (num3==1)
                {
                    zoom_line_yuv(p4,tmpbuf,dec_w,scalex);
                    p2 = tmpbuf;
                    p1 = ptmp2;
                    num3=0;
                }
                else
                {
                    zoom_line_yuv(p4,ptmp2,dec_w,scalex);
                    p1 = tmpbuf;
                    p2 = ptmp2;
                    num3 = 1;
                }
            }
            else if (num1!=num2)
            {
                zoom_line_yuv(p3,tmpbuf,dec_w,scalex);
                zoom_line_yuv(p4,ptmp2,dec_w,scalex);
                p1 = tmpbuf;
                p2 = ptmp2;
                num3 = 1;
            }

            wb =( w)&(MASK);
            wa = Q15 - wb;
			
            p3 = p1;
            p4 = p2;
            p5 = p1 + dec_w;
            p6 = p2 + dec_w;

            for (i=0;i<(dec_w>>1);i++)
            {
                y = *p3++;
                u = *p5++;
                y1 = *p4++;
                u1 = *p6++;

                y = ((y*wa + y1*wb)>>S15);
                u = ((u*wa + u1*wb)>>S15);
					
                *pb++ = (u<<8)|(y);
                /////////////////////////////
                y = *p3++;
                v = *p5++;
                y1 = *p4++;
                v1 = *p6++;

                y = ((y*wa + y1*wb)>>S15);
                v = ((v*wa + v1*wb)>>S15);
                *pb++ = (v<<8)|(y);
            }
            w+=scaley;
            num2 = num1;
        }
    }
		
    mmc_MemFreeLast((char*)tmpbuf);

    return 0;
}

#ifndef _CAM_SMALL_MEM_
//extern void mmi_camera_get_next_filename(char* file_buf_p);
//extern U16 UnicodeToAnsii(S8 *pOutBuffer, S8 *pInBuffer );
//extern U16 AnsiiToUnicodeString(S8 *pOutBuffer, S8 *pInBuffer );
int  Coolsand_VGA_zoom_op_yuv(uint16* srcbuf,uint16 src_w,uint16 src_h,uint16 cut_w,uint16 cut_h,uint16* decbuf,uint16 dec_w,uint16 dec_h)
{
    short  offx,offy;
    short  wa,wb,i,j,scalex,scaley;
    int    w;
    uint16  *pb ,*pa,*p1,*p2,*p3,*p4,*p5,*p6;
    short y,u,v,y1,u1,v1;
    short*  tmpbuf =(short*)mmc_MemMalloc(dec_w*8) ;
    short*  ptmp2 = tmpbuf + dec_w*2;
    short  num1,num2,num3;
    int    flag=0;

    if(tmpbuf==NULL)
        return -1;

    offx = (src_w - cut_w)>>1;
    offx = (offx>>1)<<1;
    offy = (src_h - cut_h)>>1;

    scalex=((cut_w-1)<<S15)/dec_w;
    scaley=((cut_h-1)<<S15)/dec_h;
	
    pa = srcbuf + offy*src_w + offx;
    pb = decbuf;

    w  = 0;
    num2 = -2;
    num3 = 1;

    p3 = pa;
    p4 = p3 + src_w;
    p1 = NULL;
    p2 = NULL;
    //zoom_line_yuv_normal(p3,tmpbuf,dec_w,scalex);
    //zoom_line_yuv_normal(p4,ptmp2,dec_w,scalex);
    //p1 = tmpbuf;
    //p2 = ptmp2;

    for (j=0;j<dec_h;j++)
    {
        num1 = (w>>S15);
        if ( !flag &&j>=offy + num1 )
        {
            //break;
            flag = 1;//break;
            memcpy(blendBuffer,srcbuf+ offx+j*src_w,j*src_w*2);
            w = w&MASK;
            num1 = 0;
            num2 -= num1;
            pa = blendBuffer;
        }

        p3 = pa + (num1*src_w);
        p4 = p3 + src_w;

        if (num1-num2==1)
        {
            if (num3==1)
            {
                zoom_line_yuv(p4,tmpbuf,dec_w,scalex);
                p2 = tmpbuf;
                p1 = ptmp2;
                num3=0;
            }
            else
            {
                zoom_line_yuv(p4,ptmp2,dec_w,scalex);
                p1 = tmpbuf;
                p2 = ptmp2;
                num3 = 1;
            }
        }
        else if (num1!=num2)
        {
            zoom_line_yuv(p3,tmpbuf,dec_w,scalex);
            zoom_line_yuv(p4,ptmp2,dec_w,scalex);
            p1 = tmpbuf;
            p2 = ptmp2;
            num3 = 1;
        }				
        wb = w&MASK;
        wa = Q15 - wb;
				
        p3 = p1;
        p4 = p2;
        p5 = p1 + dec_w;
        p6 = p2 + dec_w;

        for (i=0;i<(dec_w>>1);i++)
        {
            //*pb++ = (*p1++)*wa + (*p2++)*wb;
            y = *p3++;
            u = *p5++;
            y1 = *p4++;
            u1 = *p6++;

            y = ((y*wa + y1*wb)>>S15);
            u = ((u*wa + u1*wb)>>S15);

            *pb++ = (u<<8)|(y);
            /////////////////////////////
            y = *p3++;
            v = *p5++;
            y1 = *p4++;
            v1 = *p6++;

            y = ((y*wa + y1*wb)>>S15);
            v = ((v*wa + v1*wb)>>S15);
            *pb++ = (v<<8)|(y);
        }

        w+=scaley;
        num2 = num1;
    }

    mmc_MemFreeLast((char*)tmpbuf);

    return 0;
}
#endif

int Lily_camera_savePhoto(int  file_handle)
{
    int len = 0;
    int zoomRes = 1;

    MCI_TRACE(MCI_CAMERA_TRC, 0, "[MMC_CAMERA]Lily_camera_savePhoto");

    if (file_handle < 0)
    {
        MCI_TRACE(MCI_CAMERA_TRC|TSTDOUT, 0, "[MMC_CAMERA_ERROR]FILE HANDLE IS INVALIBLE!");
        return MDI_RES_CAMERA_ERR_FAILED;
    }

#ifdef _CAM_SMALL_MEM_
    if (s_vocinited == TRUE)
    {
        quitPreview();
        s_vocinited = FALSE;
    }
#endif
    while(hal_GoudaIsActive());
    hal_GoudaVidLayerClose();
    initVocJpegEncode();

#ifndef _CAM_SMALL_MEM_
    if ((CameraInfo.image_width < srcwidth) && (CameraInfo.image_height < srcheight))
    {
        zoomRes = Coolsand_bilinerz_zoom_op_yuv(sensorbuf, srcwidth, srcheight, cutwidth, cutheight, 
                                        blendBuffer, CameraInfo.image_width, CameraInfo.image_height);
        if (zoomRes)
        {
            quitVocJpegEncode();
            return MDI_RES_CAMERA_ERR_DISK_FULL;
        }
        MMC_jpgEn(CameraInfo.image_width, CameraInfo.image_height, CameraInfo.image_quality, 
                                        (char*)convertaddr(blendBuffer), (char*)sensorbuf, yuvmode);    
    }
    else
    {
        if ((cutwidth != srcwidth) || (cutheight != srcheight))
        {
            zoomRes = Coolsand_VGA_zoom_op_yuv(sensorbuf, srcwidth, srcheight,
                                        cutwidth, cutheight, sensorbuf, 640, 480);
            if (zoomRes)
            {
                quitVocJpegEncode();
                return MDI_RES_CAMERA_ERR_DISK_FULL;
            }
        }

        MMC_jpgEn(CameraInfo.image_width, CameraInfo.image_height, CameraInfo.image_quality,
                                        (char*)sensorbuf, (char*)sensorbuf, yuvmode);
    }
#else
    memset((UINT8*)blendBuffer, 0, (srcwidth*srcheight*2));

    if ((CameraInfo.image_width <= srcwidth) && (CameraInfo.image_height <= srcheight))
        zoomRes = Coolsand_bilinerz_zoom_op_yuv(sensorbuf, srcwidth, srcheight, cutwidth, cutheight, blendBuffer, CameraInfo.image_width, CameraInfo.image_height);
    else
        zoomRes = Coolsand_bilinerz_zoom_op_yuv(sensorbuf, srcwidth, srcheight, cutwidth, cutheight, blendBuffer, srcwidth, srcheight);

    if (zoomRes)
    {
        quitVocJpegEncode();
        if (s_vocinited == FALSE)
        {
            initVocPreviewYUV();
            s_vocinited = TRUE;
        }
        return MDI_RES_CAMERA_ERR_DISK_FULL;
    }

    MMC_jpgEn(CameraInfo.image_width, CameraInfo.image_height, CameraInfo.image_quality, (char*)convertaddr(blendBuffer), (char*)sensorbuf, yuvmode);
#endif

    len= FS_Write(file_handle, (uint8*)sensorbuf + 1, getJpegEncodeLen());
    MCI_TRACE(MCI_CAMERA_TRC, 0, "[MMC_CAMERA]SAVE PHOTO LEN=%d, actual len=%d", getJpegEncodeLen(),len);

    quitVocJpegEncode();
#ifdef _CAM_SMALL_MEM_
    if (s_vocinited == FALSE)
    {
        initVocPreviewYUV();
        s_vocinited = TRUE;
    }
#endif
    if (len <= 0)
    {
        return MDI_RES_CAMERA_ERR_DISK_FULL;
    }

    return MDI_RES_CAMERA_SUCCEED;
}

void Camera_UserMsg(COS_EVENT *pnMsg)
{
    MCI_TRACE(MCI_CAMERA_TRC, 0, "[MMC_CAMERA]Camera_UserMsg pnMsg->nEventId:%d ",pnMsg->nEventId);

    switch (pnMsg->nEventId)
    {
        case  (MSG_CAM_CALLBACK):
            mmc_CamBlendDsp();
            break;
#if 0            
#ifdef _CAM_SMALL_MEM_
        case (MSG_CAM_CAPTURE):
            if (countdown-- == AVERAGE_FRAME_NUM)
            {
                memcpy((UINT8*)sensorbuf, (UINT8*)blendBuffer, (srcwidth*srcheight*2));
            }
            else
            {
                frameAverageYuv422(sensorbuf, blendBuffer);
            }
            if (!countdown)
            {
                Lily_Camera_PreviewOff();//priew off????
                sensorready = 1;
            }
            else
            {
                camera_CaptureImage();
            }
            break;
#endif
#endif
        case EV_TIMER:
            switch(pnMsg->nParam1)
            {
                case MMC_CAMERA_TIMER_ID:
                    MCI_TRACE(MCI_CAMERA_TRC, 0,"[MMC_CAMERA]Camera_UserMsg  EV_TIMER");
                    if (bset < 2)//bset==2 means the camera is power off
                        Lily_Camera_SetEffect(CAM_PARAM_WB, cam_wbValue);
                    break;
                default:
                    MCI_TRACE(MCI_CAMERA_TRC, 0,"[MMC_CAMERA]Invalid timer ID in  camera");
                    break;
            }
            break;
        default:
            MCI_TRACE(MCI_CAMERA_TRC, 0,"[MMC_CAMERA]Camera_UserMsg:%d",pnMsg->nEventId);
            break;
    }
}

int Lily_Camera_SetEffect(CAM_SET_PARA effectCode,uint32 value)
{
    uint32 localEffectCode=0;
    uint32 localValue=0;
    UINT16 tmpw = 0, tmph = 0;  

#ifdef _ATV_SPLIT_
#ifdef __MMI_ANALOGTV__
    if(atvif_GetCurModule() != 1)
#endif
#endif	
        countdown = 1;
		
    Effect_Set_Valid=1;
    switch(effectCode)
    {
        case CAM_PARAM_CONTRAST:
            localEffectCode = CameraSetContrast;
            localValue =5- value;
            if(localValue == 0)
            {
                localValue = 0;
            }
            //cameffect.contrast = value;
            camera_SetContrastL(localValue);
            break;
        case CAM_PARAM_BRIGHTNESS:
            localEffectCode = CameraSetBrightness;
            localValue = value+1;
            if(localValue > 5)
            {
                localValue = 5;
            }
            //cameffect.bright = value;
            camera_SetBrightnessL(localValue);
            break;
        case CAM_PARAM_IMAGESIZE:
            if(MY_NPIX != CAM_NPIX_QQVGA)//camera
            {
                switch(value)
                {
                    case 1:
                        CameraInfo.image_width =  MMC_LcdWidth;
                        CameraInfo.image_height = MMC_LcdHeight;
                        CameraInfo.preview_width = MMC_LcdWidth;
                        CameraInfo.preview_height= MMC_LcdHeight;
                        CameraInfo.start_x = 0;
                        CameraInfo.start_y = 0;
                        break;
                    case 2:
                        CameraInfo.image_width =  176;
                        CameraInfo.image_height = 220;
                        CameraInfo.preview_width = MMC_LcdWidth;
                        CameraInfo.preview_height= MMC_LcdHeight;
                        CameraInfo.start_x = 0;
                        CameraInfo.start_y = 0;
                        break;
                    case 3:
                        CameraInfo.image_width =  320;
                        CameraInfo.image_height = 240;
                        CameraInfo.preview_width = MMC_LcdWidth;
                        CameraInfo.preview_height= 
                                    (MMC_LcdWidth*CameraInfo.image_height /CameraInfo.image_width)&0xfffffffe;
                        CameraInfo.start_x = 0;
                        CameraInfo.start_y = (MMC_LcdHeight - CameraInfo.preview_height) >> 1;
                        break;
                    case 4:
                        CameraInfo.image_width =  640;
                        CameraInfo.image_height = 480;
                        CameraInfo.preview_width = MMC_LcdWidth;
                        CameraInfo.preview_height = 
                                    (MMC_LcdWidth*CameraInfo.image_height /CameraInfo.image_width)&0xfffffffe;
                        CameraInfo.start_x = 0;
                        CameraInfo.start_y = (MMC_LcdHeight - CameraInfo.preview_height) >> 1;
                        break;
                    case 5:
                        CameraInfo.image_width =  1280;
                        CameraInfo.image_height = 1024;
                        CameraInfo.preview_width = MMC_LcdWidth;
                        CameraInfo.preview_height = 
                                    MMC_LcdWidth*3>>2;//(MMC_LcdWidth*CameraInfo.image_height /CameraInfo.image_width)&0xfffffffe;
                        CameraInfo.start_x = 0;
                        CameraInfo.start_y = (MMC_LcdHeight - CameraInfo.preview_height) >> 1;
                        break;
                    default:
                        CameraInfo.image_width =  MMC_LcdWidth;
                        CameraInfo.image_height = MMC_LcdHeight;
                        break;
                }

                tmpw = CameraInfo.preview_width;
                tmph = CameraInfo.preview_height;
                
                CameraInfo.end_x = CameraInfo.start_x + tmpw - 1;
                CameraInfo.end_y = CameraInfo.start_y + tmph - 1;

                cutwidth = srcwidth;
                cutheight = srcwidth * tmph / tmpw;
                if(cutheight > srcheight)
                {
                    cutheight = srcheight;
                    cutwidth = srcheight * tmpw/tmph;
                }
                cutheight = cutheight*10/(10+2*(CameraInfo.previewZoom));
                cutwidth = cutwidth*10/(10+2*(CameraInfo.previewZoom));   
#ifndef _CAM_SMALL_MEM_
#ifdef __MMI_ANALOGTV__ 
                if(atvif_GetCurModule() != 1)
#endif        
                {
                    if (((MMC_LcdWidth == 128) || (MMC_LcdWidth == 160))
                        &&((srcwidth > srcheight)==(CameraInfo.preview_width > CameraInfo.preview_height)))
                    {
                        cutwidth = (cutwidth>>1) + (cutwidth>>2);
                        cutheight = (cutheight>>1) + (cutheight>>2);
                    }   
                }
#endif                
                startPos = ((srcheight - cutheight)/2)*srcwidth + (srcwidth - cutwidth)/2;
            }
            else //video
            {
                switch(value)
                {
                    case 1:
                        CameraInfo.image_width =  MMC_LcdWidth;
                        CameraInfo.image_height = MMC_LcdWidth*3>>2;
                        break;

                    default:
                        CameraInfo.image_width =  MMC_LcdWidth;
                        CameraInfo.image_height = MMC_LcdWidth*3>>2;
                        break;
                }
                CameraInfo.preview_width = MMC_LcdWidth;
                CameraInfo.preview_height= MMC_LcdWidth*3>>2;
            }			
            break;		
        case CAM_PARAM_ADD_FRAME:
            CameraInfo.addFrame = value;
            break;	
        /*******the followings is ought to return each case****/		
        case CAM_PARAM_ZOOM_FACTOR:
            CameraInfo.previewZoom = value;
            
            tmpw = CameraInfo.preview_width;
            tmph = CameraInfo.preview_height;
                
            CameraInfo.end_x = CameraInfo.start_x + tmpw - 1;
            CameraInfo.end_y = CameraInfo.start_y + tmph - 1;

            cutwidth = srcwidth;
            cutheight = srcwidth * tmph / tmpw;
            if(cutheight > srcheight)
            {
                cutheight = srcheight;
                cutwidth = srcheight * tmpw/tmph;
            }
            cutheight = cutheight*10/(10+2*(CameraInfo.previewZoom));
            cutwidth = cutwidth*10/(10+2*(CameraInfo.previewZoom));   
#ifndef _CAM_SMALL_MEM_
#ifdef __MMI_ANALOGTV__ 
            if(atvif_GetCurModule() != 1)
#endif        
            {
                if ((MY_NPIX != CAM_NPIX_QQVGA)
                        &&((MMC_LcdWidth == 128) || (MMC_LcdWidth == 160))
                        &&((srcwidth > srcheight)==(CameraInfo.preview_width > CameraInfo.preview_height)))
                {
                    cutwidth = (cutwidth>>1) + (cutwidth>>2);
                    cutheight = (cutheight>>1) + (cutheight>>2);
                }
            }
#endif
            startPos = ((srcheight - cutheight)/2)*srcwidth + (srcwidth - cutwidth)/2;
            break;
        case CAM_PARAM_QUALITY:
            if(value == 0)
            {
                CameraInfo.image_quality =0;
            }
            else
            {
                CameraInfo.image_quality = 1;
            }
            MCI_TRACE(MCI_CAMERA_TRC, 0, "[MMC_CAMERA]CAM_PARAM_QUALITY=%d",value);
            break;
        case CAM_PARAM_EFFECT:
            switch(value)
            {
                case MDI_CAMERA_EFFECT_NORMAL:
                    localEffectCode = CAM_SPE_NORMAL;
                    break;
                case MDI_CAMERA_EFFECT_ANTIQUE:
                    localEffectCode = CAM_SPE_ANTIQUE;
                    break;
                case MDI_CAMERA_EFFECT_REDRISH:
                    localEffectCode = CAM_SPE_REDISH;
                    break;
                case MDI_CAMERA_EFFECT_GREENRISH:
                    localEffectCode = CAM_SPE_GREENISH;
                    break;
                case MDI_CAMERA_EFFECT_BLUERISH:
                    localEffectCode = CAM_SPE_BLUEISH;
                    break;
                case MDI_CAMERA_EFFECT_BLACKWHITE:
                    localEffectCode = CAM_SPE_BLACKWHITE;
                    break;
                case MDI_CAMERA_EFFECT_NEGATIVENESS:
                    localEffectCode = CAM_SPE_NEGATIVE;
                    break;
                default:
                    MCI_TRACE(MCI_CAMERA_TRC|TSTDOUT, 0, "[MMC_CAMERA_ERROR]ERROR CAM_PARAM_EFFECT");
                    localEffectCode = MDI_CAMERA_EFFECT_NORMAL;
                    break;
            }
            //cameffect.effect = value;
            MCI_TRACE(MCI_CAMERA_TRC, 0, "[MMC_CAMERA]localEffectCode=%d",localEffectCode);
            camera_SetSpecialEffect(localEffectCode);
            break;
        case CAM_PARAM_NIGHT_MODE:
            localEffectCode = CameraSetBrightness;
            localValue = value;
            //cameffect.nightmode = value;
            if(TRUE== localValue)
            {
                camera_SetNightMode(1);//CameraIocontrol(CameraNightModeEn,NULL,NULL);
                CameraInfo.nightmode = 1;
                if (flash_enable == 1)
                    camera_SetFlashL(CAM_FLASH_FORCED);
            }
            else
            {
                camera_SetNightMode(0);//CameraIocontrol(CameraNightModeDis,NULL,NULL);
                CameraInfo.nightmode = 0;
                if (flash_enable == 1)
                    camera_SetFlashL(CAM_FLASH_NONE);
                
            }	
            break;	
        case CAM_PARAM_WB:
            MCI_TRACE(MCI_CAMERA_TRC, 0, "[MMC_CAMERA]CAM_PARAM_WB=%d",value);
            switch(value)
            {
                case 0:
                    camera_SetWhiteBalanceL(CAM_WB_AUTO); /*Auto */
                    break;
                case 1:
                    camera_SetWhiteBalanceL(CAM_WB_DAYLIGHT);
                    break;
                case 2:
                    camera_SetWhiteBalanceL(CAM_WB_TUNGSTEN);
                    break;
                case 3:
                    camera_SetWhiteBalanceL(CAM_WB_OFFICE);
                    break;
                case 4:
                    camera_SetWhiteBalanceL(CAM_WB_CLOUDY);
                    break;				
                default:
                    camera_SetWhiteBalanceL(CAM_WB_AUTO); /*Auto */
                    break;
            }
            break;
        case CAM_PARAM_FLASH:
            MCI_TRACE(MCI_CAMERA_TRC, 0, "[MMC_CAMERA]CAM_PARAM_FLASHlocalEffectCode=%d,value is %d",localEffectCode,value);
            flash_enable=value;
            if (flash_enable == 0 && CameraInfo.nightmode == 1)
                camera_SetFlashL(CAM_FLASH_NONE);
            break;
        default:
            break;			
    }	
	
    return 0;	
}	

void set_multicaputure(int i)
{
    multicapture = i;
}

void set_mmc_camera_preview()
{
    previewstatus = 1;
}

#endif

#else // (CHIP_HAS_GOUDA != 1)


#include "cs_types.h"
#include "mcip_debug.h"
#include "hal_timers.h"
#include "lcd_if.h"

#include "mmc.h"
#include "mci.h"

#include "camera_m.h"
#include "pmd_m.h"

#include "mmc_camera.h"
#include "mmc_videorecord.h"


#include "event.h"
#include "fs.h"
#include "mmc_timer.h"


#include "mmc_previewvoc.h"
#include "mmc_jpegencvoc.h"
#include "vpp_audiojpeg_enc.h"

#include "global.h"

#if (CSW_EXTENDED_API_LCD == 1)
#include "sxr_tls.h"
#include "mci_lcd.h"
#include "lcdd_m.h"

PRIVATE VOID lcd_BlockWriteBypass(UINT16* buffer, UINT16 startx,UINT16 starty,UINT16 endx,UINT16 endy);
#endif // (CSW_EXTENDED_API_LCD == 1)

#define   USE_YUV_FORMAT  1
#define DRV_RGB2PIXEL565(r,g,b)	\
	((((r) & 0xf8) << 8) | (((g) & 0xfc) << 3) | (((b) & 0xf8) >> 3))
	
extern void * memset(void *,int,size_t);
extern void * memcpy(void *,const void *,size_t);
//extern kal_uint32 get_lcd_frame_buffer_address(void);

//#define   GUGOGI 1
//#define   USE_VIDEO  
//#define USE_SRM

//extern volatile  int32 Voc_JpegEnc_Busy;
//extern volatile int32 Voc_AmrEnc_Busy;
//extern volatile int32 Voc_Jpg_Need;
//extern volatile int32 Voc_Amr_Need;
//extern volatile int32 Voc_Interrupt_From;//0: VOC interrupt from jpgenc  and 1: VOC interrupt from amrenc  
//#define   USE_VGA   1
uint16  *sensorbuf=NULL;//[640*480];
uint16  *sensorbuf1=NULL;
uint16  *sensorbuf2=NULL;
uint16  *blendBuffer=NULL;//[MMC_LcdWidth*MMC_LcdHeight];
uint16  *blendBuffer2=NULL;
uint16  *backbuffer=NULL;

static UINT32 mmcCameraErrordatatcnt = 0;

MMC_CAMERA_CB mmc_camCB;

#if (CSW_EXTENDED_API_LCD == 1)
    LCDD_FBW_T displayFbw;
#endif
//static   CAM_INFO_STRUCT  CameraInfo;
volatile   CAM_INFO_STRUCT  CameraInfo;
volatile int32   previewstop=0;
volatile int32   sensorready=0;
volatile int32   sensorCaptureReady=0;

volatile int32   countdown =3;
static   uint32  cam_wbValue = 0;
//static   HANDLE  ptimer;
static   int     previewstatus=0;
static   int     multicaputure=0;
//static   int     countcaputure=0;
//static   uint16  srcwidth=0,srcheight=0;
//static   uint16  cutwidth=0,cutheight=0;
volatile   uint16  srcwidth=0,srcheight=0;
volatile  uint16  cutwidth=0,cutheight=0;
extern INT32 b_vga;
volatile uint16  bset=0;
volatile uint16  flash_enable=0;
static   short   yuvmode=0;
static  int32  turn =0;
CAM_SIZE_T   MY_NPIX   =CAM_NPIX_QVGA;
#ifdef USE_SRM
const BYTE rgb_sat[768]  
__attribute__((section (".sramdata")))	=
#else
const BYTE rgb_sat[768] = 
#endif
{
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0x0,0x1,0x2,0x3,0x4,0x5,0x6,0x7,0x8,0x9,0xa,0xb,0xc,0xd,0xe,0xf,
0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f,
0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2a,0x2b,0x2c,0x2d,0x2e,0x2f,
0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3a,0x3b,0x3c,0x3d,0x3e,0x3f,
0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4a,0x4b,0x4c,0x4d,0x4e,0x4f,
0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5a,0x5b,0x5c,0x5d,0x5e,0x5f,
0x60,0x61,0x62,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6a,0x6b,0x6c,0x6d,0x6e,0x6f,
0x70,0x71,0x72,0x73,0x74,0x75,0x76,0x77,0x78,0x79,0x7a,0x7b,0x7c,0x7d,0x7e,0x7f,
0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f,
0x90,0x91,0x92,0x93,0x94,0x95,0x96,0x97,0x98,0x99,0x9a,0x9b,0x9c,0x9d,0x9e,0x9f,
0xa0,0xa1,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7,0xa8,0xa9,0xaa,0xab,0xac,0xad,0xae,0xaf,
0xb0,0xb1,0xb2,0xb3,0xb4,0xb5,0xb6,0xb7,0xb8,0xb9,0xba,0xbb,0xbc,0xbd,0xbe,0xbf,
0xc0,0xc1,0xc2,0xc3,0xc4,0xc5,0xc6,0xc7,0xc8,0xc9,0xca,0xcb,0xcc,0xcd,0xce,0xcf,
0xd0,0xd1,0xd2,0xd3,0xd4,0xd5,0xd6,0xd7,0xd8,0xd9,0xda,0xdb,0xdc,0xdd,0xde,0xdf,
0xe0,0xe1,0xe2,0xe3,0xe4,0xe5,0xe6,0xe7,0xe8,0xe9,0xea,0xeb,0xec,0xed,0xee,0xef,
0xf0,0xf1,0xf2,0xf3,0xf4,0xf5,0xf6,0xf7,0xf8,0xf9,0xfa,0xfb,0xfc,0xfd,0xfe,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
};

#ifdef USE_SRM
const short TableV2G[]  
__attribute__((section (".sramdata")))	=
#else
const short TableV2G[]  =
#endif
{
	0,   0,   1,   2,   2,   3,   4,   5,   5,   6,   7,   7,   8,   9,  10,  10, 
	11,  12,  12,  13,  14,  15,  15,  16,  17,  17,  18,  19,  20,  20,  21,  22, 
	22,  23,  24,  25,  25,  26,  27,  27,  28,  29,  30,  30,  31,  32,  32,  33, 
	34,  35,  35,  36,  37,  37,  38,  39,  40,  40,  41,  42,  42,  43,  44,  45, 
	45,  46,  47,  47,  48,  49,  50,  50,  51,  52,  52,  53,  54,  55,  55,  56, 
	57,  57,  58,  59,  60,  60,  61,  62,  62,  63,  64,  65,  65,  66,  67,  67, 
	68,  69,  70,  70,  71,  72,  72,  73,  74,  75,  75,  76,  77,  77,  78,  79, 
	80,  80,  81,  82,  82,  83,  84,  85,  85,  86,  87,  87,  88,  89,  90,  90, 
	91,  92,  92,  93,  94,  95,  95,  96,  97,  97,  98,  99,  100, 100, 101, 102, 
	102, 103, 104, 105, 105, 106, 107, 107, 108, 109, 110, 110, 111, 112, 112, 113, 
	114, 115, 115, 116, 117, 117, 118, 119, 120, 120, 121, 122, 122, 123, 124, 125, 
	125, 126, 127, 127, 128, 129, 130, 130, 131, 132, 132, 133, 134, 135, 135, 136, 
	137, 137, 138, 139, 140, 140, 141, 142, 142, 143, 144, 145, 145, 146, 147, 147, 
	148, 149, 150, 150, 151, 152, 152, 153, 154, 155, 155, 156, 157, 157, 158, 159, 
	160, 160, 161, 162, 162, 163, 164, 165, 165, 166, 167, 167, 168, 169, 170, 170, 
	171, 172, 172, 173, 174, 175, 175, 176, 177, 177, 178, 179, 180, 180, 181, 182
};
#ifdef USE_SRM
const short TabV2R[]
__attribute__((section (".sramdata")))	=
#else
const short TabV2R[]	=
#endif
{
-179 ,-178 ,-177 ,-175 ,-174 ,-172 ,-171 ,-170 ,
-168 ,-167 ,-165 ,-164 ,-163 ,-161 ,-160 ,-158 ,
-157 ,-156 ,-154 ,-153 ,-151 ,-150 ,-149 ,-147 ,
-146 ,-144 ,-143 ,-142 ,-140 ,-139 ,-137 ,-136 ,
-135 ,-133 ,-132 ,-130 ,-129 ,-128 ,-126 ,-125 ,
-123 ,-122 ,-121 ,-119 ,-118 ,-116 ,-115 ,-114 ,
-112 ,-111 ,-109 ,-108 ,-107 ,-105 ,-104 ,-102 ,
-101 ,-100 , -98 , -97 , -95 , -94 , -93 , -91 ,
 -90 , -88 , -87 , -86 , -84 , -83 , -81 , -80 ,
 -79 , -77 , -76 , -74 , -73 , -72 , -70 , -69 ,
 -67 , -66 , -65 , -63 , -62 , -60 , -59 , -57 ,
 -56 , -55 , -53 , -52 , -50 , -49 , -48 , -46 ,
 -45 , -43 , -42 , -41 , -39 , -38 , -36 , -35 ,
 -34 , -32 , -31 , -29 , -28 , -27 , -25 , -24 ,
 -22 , -21 , -20 , -18 , -17 , -15 , -14 , -13 ,
 -11 , -10 ,  -8 ,  -7 ,  -6 ,  -4 ,  -3 ,  -1 ,
   0 ,   1 ,   3 ,   4 ,   6 ,   7 ,   8 ,  10 ,
  11 ,  13 ,  14 ,  15 ,  17 ,  18 ,  20 ,  21 ,
  22 ,  24 ,  25 ,  27 ,  28 ,  29 ,  31 ,  32 ,
  34 ,  35 ,  36 ,  38 ,  39 ,  41 ,  42 ,  43 ,
  45 ,  46 ,  48 ,  49 ,  50 ,  52 ,  53 ,  55 ,
  56 ,  57 ,  59 ,  60 ,  62 ,  63 ,  65 ,  66 ,
  67 ,  69 ,  70 ,  72 ,  73 ,  74 ,  76 ,  77 ,
  79 ,  80 ,  81 ,  83 ,  84 ,  86 ,  87 ,  88 ,
  90 ,  91 ,  93 ,  94 ,  95 ,  97 ,  98 , 100 ,
 101 , 102 , 104 , 105 , 107 , 108 , 109 , 111 ,
 112 , 114 , 115 , 116 , 118 , 119 , 121 , 122 ,
 123 , 125 , 126 , 128 , 129 , 130 , 132 , 133 ,
 135 , 136 , 137 , 139 , 140 , 142 , 143 , 144 ,
 146 , 147 , 149 , 150 , 151 , 153 , 154 , 156 ,
 157 , 158 , 160 , 161 , 163 , 164 , 165 , 167 ,
 168 , 170 , 171 , 172 , 174 , 175 , 177 , 178 ,
};
#ifdef USE_SRM
const short TabU2G[]
__attribute__((section (".sramdata")))	=
#else
const short TabU2G[]	=
#endif
{
 135 , 135 , 135 , 134 , 134 , 134 , 133 , 133 ,
 133 , 132 , 132 , 132 , 131 , 131 , 131 , 130 ,
 130 , 130 , 129 , 129 , 129 , 128 , 128 , 128 ,
 127 , 127 , 127 , 126 , 126 , 126 , 125 , 125 ,
 124 , 124 , 124 , 123 , 123 , 123 , 122 , 122 ,
 122 , 121 , 121 , 121 , 120 , 120 , 120 , 119 ,
 119 , 119 , 118 , 118 , 118 , 117 , 117 , 117 ,
 116 , 116 , 116 , 115 , 115 , 115 , 114 , 114 ,
 113 , 113 , 113 , 112 , 112 , 112 , 111 , 111 ,
 111 , 110 , 110 , 110 , 109 , 109 , 109 , 108 ,
 108 , 108 , 107 , 107 , 107 , 106 , 106 , 106 ,
 105 , 105 , 105 , 104 , 104 , 104 , 103 , 103 ,
 102 , 102 , 102 , 101 , 101 , 101 , 100 , 100 ,
 100 ,  99 ,  99 ,  99 ,  98 ,  98 ,  98 ,  97 ,
  97 ,  97 ,  96 ,  96 ,  96 ,  95 ,  95 ,  95 ,
  94 ,  94 ,  94 ,  93 ,  93 ,  93 ,  92 ,  92 ,
  91 ,  91 ,  91 ,  90 ,  90 ,  90 ,  89 ,  89 ,
  89 ,  88 ,  88 ,  88 ,  87 ,  87 ,  87 ,  86 ,
  86 ,  86 ,  85 ,  85 ,  85 ,  84 ,  84 ,  84 ,
  83 ,  83 ,  83 ,  82 ,  82 ,  82 ,  81 ,  81 ,
  80 ,  80 ,  80 ,  79 ,  79 ,  79 ,  78 ,  78 ,
  78 ,  77 ,  77 ,  77 ,  76 ,  76 ,  76 ,  75 ,
  75 ,  75 ,  74 ,  74 ,  74 ,  73 ,  73 ,  73 ,
  72 ,  72 ,  72 ,  71 ,  71 ,  71 ,  70 ,  70 ,
  69 ,  69 ,  69 ,  68 ,  68 ,  68 ,  67 ,  67 ,
  67 ,  66 ,  66 ,  66 ,  65 ,  65 ,  65 ,  64 ,
  64 ,  64 ,  63 ,  63 ,  63 ,  62 ,  62 ,  62 ,
  61 ,  61 ,  61 ,  60 ,  60 ,  60 ,  59 ,  59 ,
  58 ,  58 ,  58 ,  57 ,  57 ,  57 ,  56 ,  56 ,
  56 ,  55 ,  55 ,  55 ,  54 ,  54 ,  54 ,  53 ,
  53 ,  53 ,  52 ,  52 ,  52 ,  51 ,  51 ,  51 ,
  50 ,  50 ,  50 ,  49 ,  49 ,  49 ,  48 ,  48 ,
};
#ifdef USE_SRM
const short TabU2B[]
__attribute__((section (".sramdata")))	=
#else
const short TabU2B[]	=
#endif
{
-227 ,-226 ,-224 ,-222 ,-220 ,-219 ,-217 ,-215 ,
-213 ,-212 ,-210 ,-208 ,-206 ,-204 ,-203 ,-201 ,
-199 ,-197 ,-196 ,-194 ,-192 ,-190 ,-188 ,-187 ,
-185 ,-183 ,-181 ,-180 ,-178 ,-176 ,-174 ,-173 ,
-171 ,-169 ,-167 ,-165 ,-164 ,-162 ,-160 ,-158 ,
-157 ,-155 ,-153 ,-151 ,-149 ,-148 ,-146 ,-144 ,
-142 ,-141 ,-139 ,-137 ,-135 ,-134 ,-132 ,-130 ,
-128 ,-126 ,-125 ,-123 ,-121 ,-119 ,-118 ,-116 ,
-114 ,-112 ,-110 ,-109 ,-107 ,-105 ,-103 ,-102 ,
-100 , -98 , -96 , -94 , -93 , -91 , -89 , -87 ,
 -86 , -84 , -82 , -80 , -79 , -77 , -75 , -73 ,
 -71 , -70 , -68 , -66 , -64 , -63 , -61 , -59 ,
 -57 , -55 , -54 , -52 , -50 , -48 , -47 , -45 ,
 -43 , -41 , -40 , -38 , -36 , -34 , -32 , -31 ,
 -29 , -27 , -25 , -24 , -22 , -20 , -18 , -16 ,
 -15 , -13 , -11 ,  -9 ,  -8 ,  -6 ,  -4 ,  -2 ,
   0 ,   1 ,   3 ,   5 ,   7 ,   8 ,  10 ,  12 ,
  14 ,  15 ,  17 ,  19 ,  21 ,  23 ,  24 ,  26 ,
  28 ,  30 ,  31 ,  33 ,  35 ,  37 ,  39 ,  40 ,
  42 ,  44 ,  46 ,  47 ,  49 ,  51 ,  53 ,  54 ,
  56 ,  58 ,  60 ,  62 ,  63 ,  65 ,  67 ,  69 ,
  70 ,  72 ,  74 ,  76 ,  78 ,  79 ,  81 ,  83 ,
  85 ,  86 ,  88 ,  90 ,  92 ,  93 ,  95 ,  97 ,
  99 , 101 , 102 , 104 , 106 , 108 , 109 , 111 ,
 113 , 115 , 117 , 118 , 120 , 122 , 124 , 125 ,
 127 , 129 , 131 , 133 , 134 , 136 , 138 , 140 ,
 141 , 143 , 145 , 147 , 148 , 150 , 152 , 154 ,
 156 , 157 , 159 , 161 , 163 , 164 , 166 , 168 ,
 170 , 172 , 173 , 175 , 177 , 179 , 180 , 182 ,
 184 , 186 , 187 , 189 , 191 , 193 , 195 , 196 ,
 198 , 200 , 202 , 203 , 205 , 207 , 209 , 211 ,
 212 , 214 , 216 , 218 , 219 , 221 , 223 , 225 ,
};
//////////process begin//////////////////////////////////
//#define   Q15      8192//16384//32768
//#define   Q14      4096//8192//16384
//#define   S15      13//14// 15
//#define   MASK     (Q15-1)

#define   Q15      32767
#define   Q14      16384
#define   S15      15
#define   MASK     (Q15-1)

#define CAMERA_RGB2PIXEL565(r,g,b) \
  ((( (r) & 0xf8) << 8) | (( (g) & 0xfc) << 3) | (( (b) & 0xf8) >> 3))

void set_multicaputure(int i)
{
     multicaputure = i;
}

int mmc_camera_getstatus()
{
  
   return previewstatus;
}

void set_mmc_camera_preview()
{
#if (CSW_EXTENDED_API_LCD == 1)
      mci_LcdStartBypass(lcd_BlockWriteBypass);
#endif
      previewstatus = 1;
}
#if 0
void setwb_isr_process(void)
{
  if(bset<2)//bset==2 means the camera is power off
     Lily_Camera_SetEffect(CAM_PARAM_WB,cam_wbValue);
  
  if(ptimer)
     StopMyTimer(EV_VIDEO_TIMER_ID);  
}
#endif
#if (CSW_EXTENDED_API_LCD == 1)
VOID lcd_BlockWriteBypass(UINT16* buffer, UINT16 startx,UINT16 starty,UINT16 endx,UINT16 endy)
{
	if(backbuffer)
   	{
		int startPos;
		int i,step;
		UINT16 *plinesrc,*plinedes;

		startPos =MMC_LcdWidth*starty+startx;
		plinesrc = buffer+startPos;
		plinedes = backbuffer+startPos;
		step=endx-startx+1;

		if(step==MMC_LcdWidth){
			//memcpy((void*)backbuffer,(void*)buffer,MMC_LcdWidth*MMC_LcdHeight*2);
			memcpy((void*)plinedes,(void*)plinesrc,MMC_LcdWidth*(endy-starty+1)*2);
		}
		else{
			step<<=1;
			for(i=starty;i<=endy;i++)
			{
				memcpy((void*)plinedes,(void*)plinesrc, step);
				plinesrc+=MMC_LcdWidth;
				plinedes+=MMC_LcdWidth;
			}
		}
	}
}
#endif

int lily_transfer_data()
{
   if(backbuffer)
   	{
        memcpy((void*)backbuffer,(void*)get_lcd_frame_buffer_address(),MMC_LcdWidth*MMC_LcdHeight*2);
	  return 0;
   	}
   else
   	  return -1;

}


void  zoom_line(uint16* srcbuf,uint16* dec, uint16 dec_w,uint16 scalex)
{
	short wa,wb,i,w1,w2,w;
	int   wn;
	short rr,gg,bb,rr1,gg1,bb1;
	short *pb=dec;
	uint16*  pa;

		
	wn = 0;
	for (i=0;i<dec_w;i++)
	{
		wb = (wn)&(MASK);
		wa = Q15 - wb;

		w  = (wn>>S15);
		w1 = w&1;
		w2 = 1-w1;

		pa = srcbuf + w - w1+w2;
		rr = ((*pa)>>11) & 0x1f;
		rr = rr<<3;
		gg = ((*pa)>>5)  & 0x3f;
		gg = gg<<2;
		bb = ((*pa))  & 0x1f;
		bb = bb<<3;

		pa = srcbuf + w +(w1<<1);
		rr1 = ((*pa)>>11) & 0x1f;
		rr1 = rr1<<3;
		gg1 = ((*pa)>>5)  & 0x3f;
		gg1 = gg1<<2;
		bb1 = ((*pa))  & 0x1f;
		bb1 = bb1<<3;
				
		*pb++ =(rr*wa + rr1*wb)>>S15;
		*pb++ =(gg*wa + gg1*wb)>>S15;
		*pb++ =(bb*wa + bb1*wb)>>S15;
		
		wn+=scalex;
		}
}
#if 0
void Coolsand_bilinerz_zoom_op(uint16* srcbuf,uint16 src_w,uint16 src_h,uint16 cut_w,uint16 cut_h,uint16* decbuf,uint16 dec_w,uint16 dec_h)
{
		short  offx,offy;
		short  wa,wb,i,j;
		int    w,scalex,scaley;
		uint16  *pb ,*pa,*p1,*p2,*p3,*p4;
		short rr,gg,bb,rr1,gg1,bb1;
		short*  tmpbuf =(short*)mmc_MemMallocBack(dec_w*12) ;
		short*  ptmp2 = tmpbuf + dec_w*3;
		short  num1,num2,num3;

		cut_w= (cut_w>>1)<<1;
		offx = (src_w - cut_w)>>1;
		offx = (offx>>1)<<1;
		offy = (src_h - cut_h)>>1;
	
		
		scalex=((cut_w-1)<<S15)/dec_w;
		scaley=((cut_h-1)<<S15)/dec_h;
		
	       pa = srcbuf + offy*src_w + offx;
              pb = decbuf;
		
		w  = 0;
		num2 = 0;
		num3 = 1;

		p3 = pa;
		p4 = p3 + src_w;
		zoom_line(p3,tmpbuf,dec_w,scalex);
		zoom_line(p4,ptmp2,dec_w,scalex);
		p1 = tmpbuf;
		p2 = ptmp2;

		if (dec_h >= 480)
		{
		      short*  PTmp=(short*)mmc_MemMallocBack(320*61*2) ;
		      memcpy(PTmp,decbuf+320*179,320*61*2);////keep one more line
			for (j=0;j<dec_h;j++)
			{
				num1 = (w>>S15);
				
				
				if (num1>(178-offy))//for 640*480
				{
					pa = PTmp+ offx;
					//w  = (num1-179+offy)<<S15;
					//num1 = (w>>S15);
					w = w & MASK;
					num1 = num1-179+offy;
					p3 = pa + (num1*src_w);
					p4 = p3 + src_w;
					
				}
				else
				{
					p3 = pa + (num1*src_w);
					p4 = p3 + src_w;
				}
				
				
				
				if (num1-num2==1)
				{
					if (num3==1)
					{
						zoom_line(p4,tmpbuf,dec_w,scalex);
						p2 = tmpbuf;
						p1 = ptmp2;
						num3=0;
					}
					else
					{
						zoom_line(p4,ptmp2,dec_w,scalex);
						p1 = tmpbuf;
						p2 = ptmp2;
						num3 = 1;
					}
					
				}
				else if (num1!=num2)
				{
					zoom_line(p3,tmpbuf,dec_w,scalex);
					zoom_line(p4,ptmp2,dec_w,scalex);
					p1 = tmpbuf;
					p2 = ptmp2;
					num3=1;
					
				}
				
				
				wb = (w)&(MASK);
				wa = Q15 - wb;
				
				p3 = p1;
				p4 = p2;
				for (i=0;i<dec_w;i++)
				{
					//*pb++ = (*p1++)*wa + (*p2++)*wb;
					rr = *p3++;
					gg = *p3++;
					bb = *p3++;
					//////////////////////////
					rr1 = *p4++;
					gg1 = *p4++;
					bb1 = *p4++;
					//////////////////////////
					rr = ((rr*wa + rr1*wb)>>S15);
					gg = ((gg*wa + gg1*wb)>>S15);
					bb = ((bb*wa + bb1*wb)>>S15);
					
					*pb++ = CAMERA_RGB2PIXEL565(rr,gg,bb);
				}
				w+=scaley;
				num2 = num1;
			}

			mmc_MemFreeLast((char*)PTmp);
			
		}
		else
		{
		for (j=0;j<dec_h;j++)
		{
                     num1 = (w>>S15);


			 p3 = pa + (num1*src_w);
			 p4 = p3 + src_w;

			
			if (num1-num2==1)
			{
				if (num3==1)
				{
					zoom_line(p4,tmpbuf,dec_w,scalex);
					p2 = tmpbuf;
					p1 = ptmp2;
					num3=0;
				}
				else
				{
                                   zoom_line(p4,ptmp2,dec_w,scalex);
					p1 = tmpbuf;
					p2 = ptmp2;
					num3 = 1;
				}

			}
			else if (num1!=num2)
			{
				zoom_line(p3,tmpbuf,dec_w,scalex);
				zoom_line(p4,ptmp2,dec_w,scalex);
				p1 = tmpbuf;
			       p2 = ptmp2;
				num3=1;
				
			}

			
			wb = (w)&(MASK);
			wa = Q15 - wb;
			
			p3 = p1;
			p4 = p2;
			for (i=0;i<dec_w;i++)
			{
                            //*pb++ = (*p1++)*wa + (*p2++)*wb;
				rr = *p3++;
				gg = *p3++;
				bb = *p3++;
				//////////////////////////
				rr1 = *p4++;
				gg1 = *p4++;
				bb1 = *p4++;
				//////////////////////////
				rr = ((rr*wa + rr1*wb)>>S15);
				gg = ((gg*wa + gg1*wb)>>S15);
				bb = ((bb*wa + bb1*wb)>>S15);
			
				*pb++ = CAMERA_RGB2PIXEL565(rr,gg,bb);
			}
			w+=scaley;
			num2 = num1;
		}

		}
		 
    mmc_MemFreeLast((char*)tmpbuf);
}
#endif

void  zoom_line_yuv(uint16* srcbuf,uint16* dec, uint16 dec_w,int scalex)
{
      //input format : y1 u y2 v 
	short wa,wb,i,w;
	int   wn;
	uint16 y,y1,u,u1,v,v1;
	short *pb=dec;
	short *pbu=dec+dec_w;
	uint16 *pa=srcbuf;

		
	wn = 0;
	for (i=0;i<(dec_w>>1);i++)
	{
		wb =( wn)&(MASK);
		wa = Q15 - wb;

		w  = (wn>>S15);
		/////////u v//////////
              pa = srcbuf + (w<<1);
		u = (*(pa))>>8;
		v = (*(pa+1))>>8;
		u1 = (*(pa+2))>>8;
		v1 = (*(pa+3))>>8;
             /////////y////////////
		pa = srcbuf + w ;
		y = ((*pa++))&0xff;
		y1 = ((*pa))&0xff;
				
		*pb++ =(y*wa + y1*wb)>>S15;
		*pbu++ = (u*wa+u1*wb)>>S15;
		*pbu++ = (v*wa+v1*wb)>>S15;

		wn+=scalex;
	}

	for (i=0;i<(dec_w>>1);i++)
	{
		wb =( wn)&(MASK);
		wa = Q15 - wb;

		w  = (wn>>S15);
        
		pa = srcbuf + w ;
		y = ((*pa++))&0xff;
		y1 = ((*pa))&0xff;
				
		*pb++ =(y*wa + y1*wb)>>S15;
		
		wn+=scalex;
	}
}
int Coolsand_bilinerz_zoom_op_yuv(uint16* srcbuf,uint16 src_w,uint16 src_h,uint16 cut_w,uint16 cut_h,uint16* decbuf,uint16 dec_w,uint16 dec_h)
{
		short  offx,offy;
		short  wa,wb,i,j;
		int    w,scalex,scaley;
		uint16  *pb ,*pa,*p1,*p2,*p3,*p4,*p5,*p6;
		short y,u,v,y1,u1,v1;
		short*  tmpbuf =(short*)mmc_MemMallocBack(dec_w*8) ;
		short*  ptmp2 = tmpbuf + dec_w*2;
		short  num1,num2,num3;

		if(tmpbuf==NULL)
			return -1;
		
		offx = (src_w - cut_w)>>1;
		offx = (offx>>1)<<1;
		offy = (src_h - cut_h)>>1;
	
		
		scalex=((cut_w-1)<<S15)/dec_w;
		scaley=((cut_h-1)<<S15)/dec_h;
		
	      pa = srcbuf + offy*src_w + offx;
             pb = decbuf;
		
		w  = 0;
		num2 = -2;
		num3 = 1;

		//p3 = pa;
		//p4 = p3 + src_w;
		//zoom_line_yuv(p3,tmpbuf,dec_w,scalex);
		//zoom_line_yuv(p4,ptmp2,dec_w,scalex);
		p1 = tmpbuf;
		p2 = ptmp2;

		if (dec_h >= 480)
		{
		      short*  PTmp=(short*)mmc_MemMallocBack(320*61*2) ;
			  if(PTmp==NULL)
			  	return -1;
		      memcpy(PTmp,decbuf+320*179,320*61*2);//keep one more line
			for (j=0;j<dec_h;j++)
			{
				num1 = (w>>S15);
				
				
				if (num1>(178-offy))//for 640*480
				{
					pa = PTmp + offx;
					//w  = (num1-179+offy)<<S15;
					//num1 = (w>>S15);
					w = w & MASK;
					num1 = num1-179+offy;
					p3 = pa + (num1*src_w);
					p4 = p3 + src_w;
					
				}
				else
				{
					p3 = pa + (num1*src_w);
					p4 = p3 + src_w;
				}
				
				
				
				if (num1-num2==1)
				{
					if (num3==1)
					{
						zoom_line_yuv(p4,tmpbuf,dec_w,scalex);
						p2 = tmpbuf;
						p1 = ptmp2;
						num3=0;
					}
					else
					{
						zoom_line_yuv(p4,ptmp2,dec_w,scalex);
						p1 = tmpbuf;
						p2 = ptmp2;
						num3 = 1;
					}
					
				}
				else if (num1!=num2)
				{
					zoom_line_yuv(p3,tmpbuf,dec_w,scalex);
					zoom_line_yuv(p4,ptmp2,dec_w,scalex);
					p1 = tmpbuf;
					p2 = ptmp2;
					num3 = 1;
					
				}
				
				
				wb =( w)&(MASK);
				wa = Q15 - wb;
				
				p3 = p1;
				p4 = p2;
				p5 = p1 + dec_w;
			    p6 = p2 + dec_w;
				for (i=0;i<(dec_w>>1);i++)
				{
					//*pb++ = (*p1++)*wa + (*p2++)*wb;
					y = *p3++;
					u = *p5++;
					y1 = *p4++;
					u1 = *p6++;

					y = ((y*wa + y1*wb)>>S15);
					u = ((u*wa + u1*wb)>>S15);
					
					*pb++ = (u<<8)|(y);
					/////////////////////////////
					y = *p3++;
					v = *p5++;
					y1 = *p4++;
					v1 = *p6++;

					y = ((y*wa + y1*wb)>>S15);
					v = ((v*wa + v1*wb)>>S15);
					*pb++ = (v<<8)|(y);
					
				}
				w+=scaley;
				num2 = num1;
			}

			mmc_MemFreeLast((char*)PTmp);
			
		}
		else
		{
		for (j=0;j<dec_h;j++)
		{
             num1 = (w>>S15);
			 p3 = pa + (num1*src_w);
			 p4 = p3 + src_w;

			
			if (num1-num2==1)
			{
				if (num3==1)
				{
					zoom_line_yuv(p4,tmpbuf,dec_w,scalex);
					p2 = tmpbuf;
					p1 = ptmp2;
					num3=0;
				}
				else
				{
                                   zoom_line_yuv(p4,ptmp2,dec_w,scalex);
					p1 = tmpbuf;
					p2 = ptmp2;
					num3 = 1;
				}

			}
			else if (num1!=num2)
			{
				zoom_line_yuv(p3,tmpbuf,dec_w,scalex);
				zoom_line_yuv(p4,ptmp2,dec_w,scalex);
				p1 = tmpbuf;
			    p2 = ptmp2;
				num3 = 1;
				
			}

			
			wb =( w)&(MASK);
			wa = Q15 - wb;
			
			p3 = p1;
			p4 = p2;
			p5 = p1 + dec_w;
			p6 = p2 + dec_w;

			for (i=0;i<(dec_w>>1);i++)
				{
					//*pb++ = (*p1++)*wa + (*p2++)*wb;
					y = *p3++;
					u = *p5++;
					y1 = *p4++;
					u1 = *p6++;

					y = ((y*wa + y1*wb)>>S15);
					u = ((u*wa + u1*wb)>>S15);
					
					*pb++ = (u<<8)|(y);
					/////////////////////////////
					y = *p3++;
					v = *p5++;
					y1 = *p4++;
					v1 = *p6++;

					y = ((y*wa + y1*wb)>>S15);
					v = ((v*wa + v1*wb)>>S15);
					*pb++ = (v<<8)|(y);
					
				}
			w+=scaley;
			num2 = num1;
		}

		}
		
    mmc_MemFreeLast((char*)tmpbuf);

    return 0;
}

int  Coolsand_bilinerz_zoom_op_yuv2rgb(uint16* srcbuf,uint16 src_w,uint16 src_h,uint16 cut_w,uint16 cut_h,uint16* decbuf,uint16 dec_w,uint16 dec_h)
{
		short  offx,offy;
		short  wa,wb,i,j;
		int    w,scalex,scaley;
		uint16  *pb ,*pa,*p1,*p2,*p3,*p4,*p5,*p6;
		int y,u,v,y1,u1,v1,y2,r,g,b;
		short*  tmpbuf =(short*)mmc_MemMallocBack(dec_w*8) ;
		short*  ptmp2 = tmpbuf + dec_w*2;
		short  num1,num2,num3;
              BYTE *irgb_sat = (BYTE *)(rgb_sat+256);


		if(tmpbuf==NULL)
			return -1;
		
		offx = (src_w - cut_w)>>1;
		offx = (offx>>1)<<1;
		offy = (src_h - cut_h)>>1;
	
		
		scalex=((cut_w-1)<<S15)/dec_w;
		scaley=((cut_h-1)<<S15)/dec_h;
		
	      pa = srcbuf + offy*src_w + offx;
             pb = decbuf;
		
		w  = 0;
		num2 = -2;
		num3 = 1;

		//p3 = pa;
		//p4 = p3 + src_w;
		//zoom_line_yuv(p3,tmpbuf,dec_w,scalex);
		//zoom_line_yuv(p4,ptmp2,dec_w,scalex);
		p1 = tmpbuf;
		p2 = ptmp2;


		for (j=0;j<dec_h;j++)
		{
                      num1 = (w>>S15);
			 p3 = pa + (num1*src_w);
			 p4 = p3 + src_w;

			
			if (num1-num2==1)
			{
				if (num3==1)
				{
					zoom_line_yuv(p4,tmpbuf,dec_w,scalex);
					p2 = tmpbuf;
					p1 = ptmp2;
					num3=0;
				}
				else
				{
                                  zoom_line_yuv(p4,ptmp2,dec_w,scalex);
					p1 = tmpbuf;
					p2 = ptmp2;
					num3 = 1;
				}

			}
			else if (num1!=num2)
			{
				zoom_line_yuv(p3,tmpbuf,dec_w,scalex);
				zoom_line_yuv(p4,ptmp2,dec_w,scalex);
				p1 = tmpbuf;
			       p2 = ptmp2;
				num3 = 1;
				
			}

			
			wb = (w)&(MASK);
			wa = Q15 - wb;
			
			p3 = p1;
			p4 = p2;
			p5 = p1 + dec_w;
			p6 = p2 + dec_w;

			for (i=0;i<(dec_w>>1);i++)
				{
					y = *p3++;
					u = *p5++;
					v = *p5++;

					y1 = *p4++;
					u1 = *p6++;
					v1 = *p6++;

					y = ((y*wa + y1*wb)>>S15);
					u = ((u*wa + u1*wb)>>S15);
					v = ((v*wa + v1*wb)>>S15);
					////////////////////////////
					y1 = *p3++;
					y2 = *p4++;
					y1 = ((y1*wa + y2*wb)>>S15);
#if 0
					  r=irgb_sat[((y<<8)+18*u+367*v-49280)>>8];
				         g=irgb_sat[((y<<8)-159*u-220*v+48512)>>8];
				         b=irgb_sat[((y<<8)+411*u-29*v-48896)>>8];
				        *pb++ = CAMERA_RGB2PIXEL565(r,g,b);
				        r=irgb_sat[((y1<<8)+18*u+367*v-49280)>>8];
				         g=irgb_sat[((y1<<8)-159*u-220*v+48512)>>8];
				         b=irgb_sat[((y1<<8)+411*u-29*v-48896)>>8];
				       *pb++ = CAMERA_RGB2PIXEL565(r,g,b);
#else
					r = irgb_sat[y + TabV2R[v]];
					g = irgb_sat[y + TabU2G[u] - TableV2G[v]];
					b = irgb_sat[y + TabU2B[u]];
					*pb++ = DRV_RGB2PIXEL565(r,g,b);
					r = irgb_sat[y1 + TabV2R[v]];
					g = irgb_sat[y1 + TabU2G[u] - TableV2G[v]];
					b = irgb_sat[y1 + TabU2B[u]];
					*pb++ = DRV_RGB2PIXEL565(r,g,b);
 #endif
					   
					
				}
			w+=scaley;
			num2 = num1;
		}


		
    mmc_MemFreeLast((char*)tmpbuf);

    return 0;
}
#if 0
void  SimCutZoom(uint16* srcbuf,uint16 src_w,uint16 src_h,uint16 cut_w,uint16 cut_h,uint16* decbuf,uint16 dec_w,uint16 dec_h)
{
    uint16 offx,offy;
    int  scalex,scaley;
    uint16  arrayx[640], arrayy[480];
    uint16  *px, *py;
	int  tmp,i;


	offx = (src_w - cut_w)>>1;
	offy = (src_h - cut_h)>>1;

	offx = (offx>>1)<<1;

	scalex = (int)((cut_w-1)<<16)/(dec_w);
	scaley = (int)((cut_h-1)<<16)/(dec_h);

	px = arrayx;
	py = arrayy;

	
	for (tmp=0;tmp<(dec_w);tmp++)
	{
		 i = ((tmp * scalex)>>16);
		 i = i - (i&1) + ((i+1)&1);
		 *px++ = i+offx;
	}

	for (tmp=0;tmp<(dec_h);tmp++)
	{
		*py++ = ((tmp * scaley)>>16)+offy;
	}
     //////////////////////////////////
     px = decbuf;
	for (tmp=0;tmp<(dec_h);tmp++)
	{
		for (i=0;i<(dec_w);i++)
		{
              *px++ = srcbuf[(arrayy[tmp])*src_w + (arrayx[i])];
		}
	}
}

//for video preview
void zoomyuv422_SIM(uint16* yuv_buf,uint16 src_w,uint16 src_h,uint16 cut_w,uint16 cut_h, uint16*rgb_buf,uint16 dec_w,uint16 dec_h)
{
       #define  PARAM   8
       uint16  arrayx[320];
	uint16  arrayy[240];
	uint16  *px=arrayx,*py=arrayy,*pt;
	//unsigned char *irgb_sat = rgb_sat+256;
	//FILE*  f1 = fopen("arrayx.dat","wb");
	//FILE*  f2 = fopen("arrayy.dat","wb");  


	int scalex ,scaley;
	int tmp , i ,offx, offy;
	uint16 y,u,v;


	offx = (src_w - cut_w)>>1;
	offy = (src_h - cut_h)>>1;

	scalex = (int)((cut_w-1)<<PARAM)/dec_w;
	scaley = (int)((cut_h-1)<<PARAM)/dec_h;


 	for (tmp=0;tmp<dec_w;tmp++)
	{
		 *px++ = ((tmp * scalex)>>PARAM)+offx;
	}

	for (tmp=0;tmp<dec_h;tmp++)
	{
		*py++ = ((tmp * scaley)>>PARAM)+offy;
	}
	
    //scale and zoom
	px = rgb_buf;
	for (tmp=0;tmp<dec_h;tmp++)
	{
		pt = yuv_buf+(arrayy[tmp]*src_w) ;
		for (i=0;i<dec_w;i+=2)
		{
			  py = pt + arrayx[i];
              	  if ((arrayx[i]&1)==0)//even point  [y u]
			  {
				  y = (*py)&0xff;
			         u = (*py)>>8;
			  }
			  else//odd point [y v]
			  {
				   y = (*py)&0xff;
				   u = (*(py-1))>>8;
			  }
			  *px++ = (u<<8)|y;
			  /////////////////////////
			  py = pt + arrayx[i+1];
              	  if ((arrayx[i+1]&1)==0)//even point  [y u]
			  {
				  y = (*py)&0xff;
			         v = (*(py+1))>>8;
			  }
			  else//odd point [y v]
			  {
				   y = (*py)&0xff;
				   v = (*(py))>>8;
			  }

			 *px++ = (v<<8)|y;
		}
	}

}

void yuv4112rgb565(uint16* yuv_buf,uint16 src_w,uint16 src_h, uint16*rgb_buf)
{


	unsigned char *irgb_sat = rgb_sat+256;
	int tmp , i ;
	uint16 y,y1,u,v,r,g,b;
	uint16 *px,*pt,*py;

   	
    //scale and zoom
	px = rgb_buf;
	for (tmp=0;tmp<src_h;tmp++)
	{
		pt = yuv_buf+(tmp*src_w) ;
		for (i=0;i<src_w;i+=2)
		{
			  //py = pt + i;
              
                       y = (*py)&0xff;
			  u = (*py++)>>8;
			  y1 = (*(py))&0xff;
			  v = (*(py++))>>8;

			  r = irgb_sat[y + TabV2R[v]];
			  g = irgb_sat[y + TabU2G[u] - TableV2G[v]];
			  b = irgb_sat[y + TabU2B[u]];

			  *px++ = DRV_RGB2PIXEL565(r,g,b);
			  /////////////////////////////////////
			   //py = pt + i +1;
                       // y = (*py)&0xff;
			   //v = (*py)>>8; 
			   //u = (*(py-1))>>8;
			  

			  //r = irgb_sat[y + TabV2R[v]];
			  //g = irgb_sat[y + TabU2G[u] - TableV2G[v]];
			  //b = irgb_sat[y + TabU2B[u]];
			  r = irgb_sat[r+y1-y];
			  g =irgb_sat[g+y1-y];
			  b =irgb_sat[b+y1-y];

			  *px++ = DRV_RGB2PIXEL565(r,g,b);
			  
		}
	}

}
//#else
void yuv4112rgb565(uint16* yuv_buf,uint16 src_w,uint16 src_h, uint16*rgb_buf)
{


	unsigned char *irgb_sat = (unsigned char *)(rgb_sat+256);
	int tmp , i ;
	uint16 y,y1,u,v,r,g,b;
	uint16 *px=NULL,*pt=NULL,*py=NULL;

   	
    //scale and zoom
	px = rgb_buf;
	for (tmp=0;tmp<src_h;tmp++)
	{
		pt = yuv_buf+(tmp*src_w) ;
		for (i=0;i<src_w;i+=2)
		{
			  py = pt + i;
              
                       y = (*py)&0xff;
			  u = (*py++)>>8;
			  y1 = (*(py))&0xff;
			  v = (*(py))>>8;

			  r = irgb_sat[y + TabV2R[v]];
			  g = irgb_sat[y + TabU2G[u] - TableV2G[v]];
			  b = irgb_sat[y + TabU2B[u]];

			  *px++ = DRV_RGB2PIXEL565(r,g,b);
			  /////////////////////////////////////
			  r = irgb_sat[y1 + TabV2R[v]];
			  g = irgb_sat[y1 + TabU2G[u] - TableV2G[v]];
			  b = irgb_sat[y1 + TabU2B[u]];

			  *px++ = DRV_RGB2PIXEL565(r,g,b);
			  
		}
	}

}

void display_240by240(uint16* yuv_buf, uint16*rgb_buf)
{
	 int i,j;
     uint16* pline = yuv_buf;
	 uint16* px = rgb_buf,*py;
	 uint16 y,y1,u,v,r,g,b;
	BYTE *irgb_sat = (BYTE *)(rgb_sat+256);

	 for (j=0;j<320;j++)
	 {
		 for (i=0;i<240;i+=2)
		 {
			 py = pline + i;
              
                       y = (*py)&0xff;
			  u = ((*py++)>>8)&0xff;
			  y1 = (*(py))&0xff;
			  v = ((*(py))>>8)&0xff;
#if 0
			  r = irgb_sat[((short)y) + TabV2R[v]];
			  g = irgb_sat[((short)y)  + TabU2G[u] - TableV2G[v]];
			  b = irgb_sat[((short)y)  + TabU2B[u]];
#else
                       r=irgb_sat[((y<<8)+18*u+367*v-49280)>>8];
			  g=irgb_sat[((y<<8)-159*u-220*v+48512)>>8];
		         b=irgb_sat[((y<<8)+411*u-29*v-48896)>>8];

			 //r=( 298 * y + 409 * v - 56992)>> 8;
	               //if(r<0) r=0; else if(r>255) r=255;
	               //g=( 298 * y -100 * u -208 * v + 34784)>> 8;
	               //if(g<0) g=0; else if(g>255) g=255;
	               //b=( 298 * y + 516 * u - 70688)>> 8;
#endif
			  *px++ = DRV_RGB2PIXEL565(r,g,b);
			  /////////////////////////////////////
#if 0			  
			  r = irgb_sat[((short)y1)  + TabV2R[v]];
			  g = irgb_sat[((short)y1)  + TabU2G[u] - TableV2G[v]];
			  b = irgb_sat[((short)y1)  + TabU2B[u]];
#else
                       r=irgb_sat[((y1<<8)+18*u+367*v-49280)>>8];
			  g=irgb_sat[((y1<<8)-159*u-220*v+48512)>>8];
			  b=irgb_sat[((y1<<8)+411*u-29*v-48896)>>8];
#endif
			  *px++ = DRV_RGB2PIXEL565(r,g,b);
		 }
		 pline+=240;
	 }
};
#endif

int  CoolSand_Cut_Zoom(uint16* srcbuf,uint16 src_w,uint16 src_h,uint16 cut_w,uint16 cut_h,uint16* decbuf,uint16 dec_w,uint16 dec_h)//__attribute__((section(".sramtext")))
{
      return Coolsand_bilinerz_zoom_op_yuv(srcbuf,src_w,src_h,cut_w,cut_h,decbuf,dec_w,dec_h);
}




int  CoolSand_Preview(uint16* srcbuf,uint16 src_w,uint16 src_h,uint16 cut_w,uint16 cut_h,uint16* decbuf,uint16 dec_w,uint16 dec_h)//__attribute__((section(".sramtext")))
{

     return Coolsand_bilinerz_zoom_op_yuv2rgb(srcbuf,src_w,src_h,cut_w,cut_h,decbuf,dec_w,dec_h);
}

int Mmc_blendPartPure(uint16 *baseBuffer,uint16 baseBufW,uint16 baseBufH,uint16 *relBuffer,uint16 relBufW,uint16 relBufH,uint16 transfCol)
{
	ass(NULL==baseBuffer ||NULL==relBuffer);
	
	//uint16 *tmpBuffer = baseBuffer +  xOffsetOfBase*yOffsetOfBase;
	uint16* pt = relBuffer;
	uint16* ps = baseBuffer;
	uint32 baseVW=0,baseVH=0;
	//uint32  countnum = 0;
	//int temp;

    if(relBufW == MMC_LcdWidth && relBufH==MMC_LcdHeight)
    	{

			for(baseVH=0;baseVH<baseBufH;baseVH++)
			{

				for(baseVW=0;baseVW<baseBufW;baseVW++)
				{
				
		              if(*ps==transfCol)
		              	{
					  	 pt++;
						 ps++;
		              	}
		              else
		              	{
					  	 *pt++ =*ps++;
						 //countnum++;
		              	}
			                 
				}
			}
    	}
	else
		{
		    int offy=(baseBufH-relBufH)>>1;

		    memcpy(relBuffer,baseBuffer,(MMC_LcdHeight-relBufH)*MMC_LcdWidth);
	           memcpy(relBuffer+(relBufH+((MMC_LcdHeight-relBufH)/2))*MMC_LcdWidth,baseBuffer+(relBufH+((MMC_LcdHeight-relBufH)/2))*MMC_LcdWidth,(MMC_LcdHeight-relBufH)*MMC_LcdWidth);

               pt = relBuffer+offy*baseBufW;
	        ps = baseBuffer+offy*baseBufW;
			
            for(baseVH=offy;baseVH<baseBufH-offy;baseVH++)
			{
               
				for(baseVW=0;baseVW<baseBufW;baseVW++)
				{
				
		              if(*ps!=transfCol)//if(baseBuffer[temp+baseVW]!=transfCol)
		              	{
                                          *pt = *ps;//relBuffer[temp+baseVW]=baseBuffer[temp+baseVW];
						//	 countnum++;
		              	}
					  pt++;
					  ps++;

			                 
				}
			}
		
		}


		return 0;//countnum;//

}

void EmbedBmpDataIntoLcdShadowBuffer(
	uint16 LcdFrameBuffer_x, uint16 LcdFrameBuffer_y, 
	uint16 bmp_width, uint16 bmp_height, 
	uint16 *pbmp, uint16 *pLcdShadowBuffer)
{
	uint16 ystart, yend, xstart, xend;
	uint16 *p1 = pbmp;
	uint16 *p2 = NULL;
	uint16 i, j;

	
	ystart = LcdFrameBuffer_y;
	yend = LcdFrameBuffer_y + bmp_height - 1;
	xstart = LcdFrameBuffer_x;
	xend = LcdFrameBuffer_x + bmp_width - 1;

	for (j = ystart; j <= yend; j++)
	{	
		p2 = pLcdShadowBuffer + j * MMC_LcdWidth + xstart;
		
		for (i = xstart; i <= xend; i++)
		{
			*p2++ = *p1++;
		}
	}
}

void mmc_camSwitchDataEnds(uint16 *buffer,uint32 bufferSize)
{
	int32 vi=0;
	uint32 pexCnt=bufferSize;
	uint16 tmp;
	for(vi=0;vi<pexCnt;vi+=2)
	{
		tmp = *(buffer+vi);
		*(buffer+vi)=*(buffer+vi+1);
		*(buffer+vi+1) = tmp;
	}
}
/*
void SwitchDatayuv(uint16* yuv_buf, uint16 width, uint16 height)
{
   int    i, j;
   int*   pt = (int*)yuv_buf;
   uint16 *pout = yuv_buf;
   uint16 y1,u,y2,v;
   unsigned int    temp;

   

   for (i=0;i<height;i++)
   {
	   for (j=0;j<width;j+=2)
	   {
          temp = *pt++;

		  y1 = temp >> 24;
		  v  = (temp >> 16)&0xff;
		  y2 = (temp>>8)&0xff;
		  u  = temp & 0xff;


//	     *pout++ = ((u<<8)&0xff00 )|(y1&0xff);
//           *pout++ = ((v<<8)&0xff00 )|(y2&0xff);
	     *pout++ = ((v<<8)&0xff00 )|(y1&0xff);
            *pout++ = ((u<<8)&0xff00 )|(y2&0xff);

	   }
   }
}
*/
//siv100b
/*
void convert_rgb5652yuv422(uint16 *src, uint16 width, uint16 height, uint16 *dec)
{
   uint16 *pin = src;
   unsigned char *pout= (unsigned char*)dec;
   uint16 i,j,tmp;
   int r, g ,b,y1,u,v,y2;

   for (i=0;i<height;i++)
   {
	   for (j=0;j<width;j+=2)
	   {
		    tmp = *pin++;
		   	r = (int)((tmp&0xf800)>>8);
			g = (int)((tmp&0x07e0)>>3);
			b = (int)((tmp&0x001f)<<3);

			y1=(((r * 76) + (g * 151) + (b * 29)) >> 8);
			u =(((r * -43) + (g * -85) + (b << 7)) >> 8)+128;
			v =(((r << 7) + (g * -107) + (b * -21)) >> 8)+128;

			tmp = *pin++;
		   	r = (int)((tmp&0xf800)>>8);
			g = (int)((tmp&0x07e0)>>3);
			b = (int)((tmp&0x001f)<<3);

			y2=(((r * 76) + (g * 151) + (b * 29)) >> 8);

			*pout++ = (unsigned char)y1;
			*pout++ = (unsigned char)u;
			*pout++ = (unsigned char)y2;
			*pout++ = (unsigned char)v;

	   }
   }
}
*/
//basic funcitons of camera
extern void VidRec_CallbackFun(UINT8 id,  UINT8 ActiveBufNum);
int Lily_Camera_PowerOn(INT32  videomode,void(*cb)(int32))
{

	//initialize for the pointers for safety reason
	UINT16*  InvalideAddress = (UINT16*)convertaddr(NULL);

	MCI_TRACE(MCI_CAMERA_TRC|TSTDOUT, 0,  "[MMC_CAMERA]Lily_Camera_PowerOn");
	
	sensorbuf = NULL;
	backbuffer = NULL;

	/* SET SEN18.V and SEN2.8V out */
	pmd_EnablePower(PMD_POWER_CAMERA, TRUE);
	

	//#ifndef USE_VIDEO
    if(videomode==0)
	{
		mmc_SetCurrMode(MMC_MODE_CAMERA);
		sensorbuf =(UINT16*)convertaddr(mmc_MemMalloc(640*480*2));
		sensorbuf2 = sensorbuf+320*240;
		blendBuffer2 = sensorbuf2+320*240;
		blendBuffer   = blendBuffer2+320*240;


		if(b_vga)	
		{
			blendBuffer=(UINT16*)(mmc_MemMalloc(320*240*4));
			backbuffer = blendBuffer + 320*240;
		}
		else
		{
			backbuffer =(UINT16*)(mmc_MemMalloc(MMC_LcdWidth*MMC_LcdHeight*2));
		}

		multicaputure = 0;
		bset = 0;
		//#else	
	}
#ifndef TARGET_VIDEO_RECORDER_SUPPORT_OFF
	else
	{
		mmc_SetCurrMode(MMC_MODE_VIDREC);
		sensorbuf =(UINT16*)convertaddr(mmc_MemMalloc(320*240*4));	
		sensorbuf1=sensorbuf+160*120;
		sensorbuf2 = sensorbuf1+160*120;
		blendBuffer = sensorbuf2+160*120;

		backbuffer =(UINT16*)convertaddr(mmc_MemMalloc(MMC_LcdWidth*MMC_LcdHeight*2));

		multicaputure = 0;
		bset = 0; 
		//#endif		
	}
#endif



#if (CSW_EXTENDED_API_LCD == 1)
	// Initialize Frame Buffer Window
	// Buffer pointer will be set at the time of display
	// Other parameters are constant
	displayFbw.fb.buffer = NULL;
	displayFbw.fb.width = MMC_LcdWidth;
	displayFbw.fb.height = MMC_LcdHeight;
	displayFbw.fb.colorFormat = LCDD_COLOR_FORMAT_RGB_565;
	displayFbw.roi.width = MMC_LcdWidth;
	displayFbw.roi.height = MMC_LcdHeight;
	displayFbw.roi.x = 0;
	displayFbw.roi.y = 0;

	// Start Bypass every time previewstatus is set to 1
	// Stop it when it is set to 0
	mci_LcdStartBypass(lcd_BlockWriteBypass);
#endif
	previewstatus = 1;
    if(videomode==0)
	{
		if(b_vga)
		{
			MY_NPIX   =CAM_NPIX_VGA;//camera
			camera_Reserve(Cam_CallbackFun_VGA);//ATV_CallbackFun//Cam_CallbackFun
		}
		else
		{
			MY_NPIX   =CAM_NPIX_QVGA;//camera
			camera_Reserve(Cam_CallbackFun);//ATV_CallbackFun//Cam_CallbackFun
		}

	}
#ifndef TARGET_VIDEO_RECORDER_SUPPORT_OFF
	else
	{
		MY_NPIX   =CAM_NPIX_QQVGA;//vedio
		camera_Reserve(VidRec_CallbackFun);
		// camera_Reserve(Cam_CallbackFun);
	}
#endif
           
       camera_PowerOn(MY_NPIX, CAM_FORMAT_RGB565);
	 
	if(sensorbuf==InvalideAddress || backbuffer==InvalideAddress||blendBuffer==InvalideAddress ||sensorbuf==NULL || backbuffer==NULL|| blendBuffer==NULL)
	{
		Lily_Camera_PowerOff();
		MCI_TRACE(MCI_CAMERA_TRC|TSTDOUT, 0, "[MMC_CAMERA]Lily_Camera_Preview lack of memory,sensorbuf=%d,blendBuffer=%d",sensorbuf,blendBuffer);
		return  MDI_RES_CAMERA_ERR_MEMORY_NOT_ENOUGH;
	}

    hal_TimDelay(25 * 16384 / 1000);          // delay 25ms

// 0xff means no camera found!
    if(camera_GetId()== 0xff)
	{
		MCI_TRACE(MCI_CAMERA_TRC|TSTDOUT, 0,  "[MMC_CAMERA]ID ERROR!");
		Lily_Camera_PowerOff();
		return MDI_RES_CAMERA_ERR_HW_NOT_READY;
	}
	mmc_camCB = cb;
	MCI_TRACE(MCI_CAMERA_TRC, 0, "[MMC_CAMERA]the sensor number is %d  b_vga=%d",camera_GetId(),b_vga);

	return MDI_RES_CAMERA_SUCCEED;

}

int Lily_Camera_PowerOff(void)
{
	int res;
	MCI_TRACE(MCI_CAMERA_TRC|TSTDOUT, 0, "[MMC_CAMERA]Lily_Camera_PowerOff");

	countdown = 0;
	previewstop = 1;
	bset = 2;

	res=COS_KillTimerEX(GetMCITaskHandle(MBOX_ID_MMC), MMC_CAMERA_TIMER_ID);
	MCI_TRACE(MCI_VIDEO_TRC,0,"[MMC_CAMERA]KILL TIMER res=%d", res);
	mmc_SetCurrMode(MMC_MODE_IDLE);
	Lily_Camera_PreviewOff();
	camera_PowerOff();
	mmc_MemFreeAll();

	return MDI_RES_CAMERA_SUCCEED;
}



int Lily_Camera_Preview(CAM_PREVIEW_STRUCT* data)
{
   

    //int i;
     
   MCI_TRACE(MCI_CAMERA_TRC|TSTDOUT, 0, "[MMC_CAMERA]Lily_Camera_Preview");
 
    
    CameraInfo.start_x = data->start_x;
    CameraInfo.start_y = data->start_y;
    CameraInfo.end_x   = data->end_x;
    CameraInfo.end_y   = data->end_y;
    CameraInfo.preview_width = data->end_x - data->start_x + 1;
    CameraInfo.preview_height= data->end_y - data->start_y + 1;
    CameraInfo.image_height = data->image_height;
    CameraInfo.image_width= data->image_width;
	
    flash_enable=data->flashenable;
	   
   previewstop = 0;
   sensorready = 0;
   countdown   = 0;
   turn =0;
  
   
   if(bset==0)//
   	{

	Lily_Camera_SetEffect(CAM_PARAM_ADD_FRAME,data->addFrame);
	Lily_Camera_SetEffect(CAM_PARAM_QUALITY,data->imageQuality);
	Lily_Camera_SetEffect(CAM_PARAM_ZOOM_FACTOR,data->factor);
	
	#if 1
	cam_wbValue = data->whiteBlance;
       //ptimer = StartMyTimer(EV_VIDEO_TIMER_ID,200,(oslTimerFuncPtr)setwb_isr_process);
       COS_SetTimerEX(GetMCITaskHandle(MBOX_ID_MMC), MMC_CAMERA_TIMER_ID, COS_TIMER_MODE_SINGLE, 200);

	#else
	  Lily_Camera_SetEffect(CAM_PARAM_WB,data->whiteBlance);
	#endif
	Lily_Camera_SetEffect(CAM_PARAM_EFFECT,data->specialEffect);
	Lily_Camera_SetEffect(CAM_PARAM_NIGHT_MODE,data->nightmode);
	Lily_Camera_SetEffect(CAM_PARAM_CONTRAST,data->contrast);
	Lily_Camera_SetEffect(CAM_PARAM_BRIGHTNESS,data->brightNess);
      bset = 1;

   }
   
	 if(MY_NPIX == CAM_NPIX_VGA)
    	{
            srcwidth = 640;
	     srcheight = 480;
	     //init voc preview 
	    #ifdef  USE_YUV_FORMAT
		initVocPreviewYUV();
	    #else	
	       initVocPreview();
	    #endif
           memset(blendBuffer, 0, MMC_LcdWidth*MMC_LcdHeight<<1);// previewstatus = 1;

    	}
	 else  if(MY_NPIX == CAM_NPIX_QVGA)
    	{
            srcwidth = 320;
	     srcheight = 240;
	     //init voc preview 
	    #ifdef  USE_YUV_FORMAT
		initVocPreviewYUV();
	    #else	
	       initVocPreview();
	    #endif
           memset(blendBuffer2, 0, MMC_LcdWidth*MMC_LcdHeight<<1);// previewstatus = 1;

    	}
       else if(MY_NPIX == CAM_NPIX_QQVGA)
       {
           srcwidth = 160;
	    srcheight = 120;
	    memset(blendBuffer, 0, MMC_LcdWidth*MMC_LcdHeight<<1);// previewstatus = 1;

       }
	else
	{
           srcwidth = 320;
	    srcheight = 240;
	}

#ifdef  USE_YUV_FORMAT
          yuvmode = 1;
#else
	    yuvmode = 0;
#endif

	mmcCameraErrordatatcnt = 0;

   //asm("break 1");
    MCI_TRACE(MCI_CAMERA_TRC, 0, "[MMC_CAMERA]Lily_Camera_Preview and before camera_PrepareImageCaptureL");
   camera_PrepareImageCaptureL(CAM_FORMAT_RGB565, MY_NPIX, sensorbuf);
   camera_CaptureImage();
	while(sensorready == 0)
	{
	/***wait forever here,to avoid mmi capture at once***/
	}
   return MDI_RES_CAMERA_SUCCEED;
}


int Lily_Camera_PreviewOff(void)
{
	MCI_TRACE(MCI_CAMERA_TRC|TSTDOUT, 0, "[MMC_CAMERA]Lily_Camera_PreviewOff");
	
	previewstop=1;
#if (CSW_EXTENDED_API_LCD == 1)
      mci_LcdStopBypass();
#endif
	previewstatus = 0;
      if(MY_NPIX != CAM_NPIX_QQVGA)
            quitPreview();
	return MDI_RES_CAMERA_SUCCEED;
	
}

void Cam_CallbackFun_VGA(UINT8 id,  UINT8 ActiveBufNum)
{

	       //mmi_trace(GUGOGI,"Cam_CallbackFun = %d ,srcwidth=%d,srcheight=%d",hal_TimGetUpTime(),srcwidth,srcheight);
              MCI_TRACE(MCI_CAMERA_TRC, 0, "[MMC_CAMERA]Cam_CallbackFun_VGA id:%d",id);
 
		   
		if(previewstop==0)//preview
			{


			   	if(countdown>0 || id!=0 )//countdown 
				{
				#if 0
					if(mmcCameraErrordatatcnt++>=25)
					{
						mmc_camCB( -1 );//-1 is not offical
						return;
					}
					#endif
					  countdown--;
                                     camera_PrepareImageCaptureL(CAM_FORMAT_RGB565, MY_NPIX, sensorbuf);
					  camera_CaptureImage();

					  return ;
				}

	  			    sensorready = 1;
	
			    //mmc_SetCurrMode(MMC_MODE_CAMERA);
                         mmc_SendEvent(GetMCITaskHandle(MBOX_ID_MMC), MSG_CAM_CALLBACK_VGA);
			}
		else  //capture
			{
			    if(countdown>0 || id!=0 )//countdown
				{
					countdown--;
		                     camera_PrepareImageCaptureL(CAM_FORMAT_RGB565, MY_NPIX, sensorbuf);
		                     camera_CaptureImage();
		                    return ;
				}
			
			    Lily_Camera_PreviewOff();//priew off????
			    sensorready = 1;
				sensorCaptureReady = 1;

                        // mmi_trace(1,"privew poweroff and data is ready");

			}


}


void mmc_CamBlendDsp_VGA()
{
	MCI_TRACE(MCI_CAMERA_TRC|TSTDOUT, 0, "[MMC_CAMERA]mmc_CamBlendDsp_VGA = %d",hal_TimGetUpTime());

	mmcCameraErrordatatcnt = 0;
	if(CameraInfo.preview_width==MMC_LcdWidth&&CameraInfo.preview_height==MMC_LcdHeight)
	{
		cutheight = srcheight*10/(10+2*(CameraInfo.previewZoom));
		cutwidth   =   cutheight*3>>2;//cutwidth  = (srcheight<<3)/(10+2*CameraInfo.previewZoom);//


		if(MMC_LcdWidth>MMC_LcdHeight)
		{
			if(((srcwidth*100)/srcheight)>((MMC_LcdWidth*100)/MMC_LcdHeight))
			{		
				cutwidth  = ((srcheight*CameraInfo.preview_width/CameraInfo.preview_height)*10/(10+2*(CameraInfo.previewZoom)))&0xfffffffe;
				cutheight = srcheight*10/(10+2*(CameraInfo.previewZoom));			
			}
			else
			{
				cutwidth  = srcwidth*10/(10+2*(CameraInfo.previewZoom));
				cutheight = ((srcwidth*CameraInfo.preview_height/CameraInfo.preview_width)*10/(10+2*(CameraInfo.previewZoom)))&0xfffffffe;				
			}
		}
		else
		{
			cutheight = srcheight*10/(10+2*(CameraInfo.previewZoom));
			cutwidth   =   cutheight*3>>2;//cutwidth  = (srcheight<<3)/(10+2*CameraInfo.previewZoom);//		}
		}


	}
	else
	{
		cutwidth  = srcwidth*10/(10+2*(CameraInfo.previewZoom));
		cutheight = srcheight*10/(10+2*(CameraInfo.previewZoom));
	}

#if (CSW_EXTENDED_API_LCD == 0)
	uint32  local;
#endif
	uint16 tmpw,tmph;
	uint32 count=0;
	uint32  c1;
	CutZoomInStruct   cutzoom;
	tmpw = CameraInfo.preview_width;
	tmph = CameraInfo.preview_height;
	count = ((MMC_LcdHeight-tmph)>>1)*MMC_LcdWidth;




	//this part is to generate preview image<1>
	cutzoom.inStreamBufAddr=(INT32*)sensorbuf;
	cutzoom.outStreamBufAddr=(INT32*)(blendBuffer+count);
	cutzoom.blendBufAddr=(INT32*)(backbuffer+count);
	cutzoom.src_w=srcwidth;
	cutzoom.src_h=srcheight;
	cutzoom.cut_w=cutwidth;
	cutzoom.cut_h=cutheight;
	cutzoom.dec_w=tmpw;
	cutzoom.dec_h=tmph;
	cutzoom.reset = 1;
	MMC_Preview_Wait(&cutzoom);


	c1=hal_TimGetUpTime();

	if(count!=0)
	{
		memcpy(blendBuffer,backbuffer,count<<1);
		memcpy(blendBuffer+(tmph*MMC_LcdWidth+count),backbuffer+(tmph*MMC_LcdWidth+count),count<<1);
	}

	MCI_TRACE(MCI_CAMERA_TRC|TSTDOUT, 0, "[MMC_CAMERA]COPY PIXELS.TOTAL TIME:%d ",hal_TimGetUpTime()-c1);

	//c1 = hal_TimGetUpTime();
	//Mmc_blendPartPure(backbuffer,MMC_LcdWidth,MMC_LcdHeight,blendBuffer,tmpw,tmph,0);
	//MCI_TRACE(MCI_CAMERA_TRC, 0, "cpu blend time=%d",hal_TimGetUpTime()-c1);

	//this part is to display the image to  the lcd screen<2>
//	c1 = hal_TimGetUpTime();
#if (CSW_EXTENDED_API_LCD == 1)
	displayFbw.fb.buffer = (UINT16*) blendBuffer;

	// Display the blended image
	LCDD_ERR_T err = lcdd_Blit16(&displayFbw,0 ,0);
	// In case the LCD was busy (maybe it is already doing a dma)
	// Retry until it is available
	// We could add a timeout here
	while (LCDD_ERR_NO != err)
	{
		sxr_Sleep(10);
		err = lcdd_Blit16(&displayFbw,0 ,0);
	}; 
#else
	local=get_lcd_frame_buffer_address();
	set_lcd_frame_buffer_address((kal_uint32)blendBuffer);
	previewstatus = 0;
	MainLCD->BlockWrite(0,0,MMC_LcdWidth-1,MMC_LcdHeight-1);
	previewstatus = 1;
	set_lcd_frame_buffer_address(local);
#endif

	//MCI_TRACE(MCI_CAMERA_TRC, 0, "[MMC_CAMERA]LCD Update time=%d",hal_TimGetUpTime()-c1);

	camera_PrepareImageCaptureL(CAM_FORMAT_RGB565, MY_NPIX, sensorbuf);
	camera_CaptureImage();


	//mmi_trace(GUGOGI,"mmc_CamBlendDsp end = %d",hal_TimGetUpTime());
}


void Cam_CallbackFun(UINT8 id,  UINT8 ActiveBufNum)
{

	       //mmi_trace(GUGOGI,"Cam_CallbackFun = %d ,srcwidth=%d,srcheight=%d",hal_TimGetUpTime(),srcwidth,srcheight);
              MCI_TRACE(MCI_CAMERA_TRC, 0, "[MMC_CAMERA]Cam_CallbackFun id:%d",id);

		   
		if(previewstop==0)//preview
			{

			   	if(countdown>0 || id!=0 )//countdown 
				{
				#if 0
					if(mmcCameraErrordatatcnt++>=25)
					{
						mmc_camCB( -1 );//-1 is not offical
						return;

					}
					#endif
					  countdown--;
					  camera_PrepareImageCaptureL(CAM_FORMAT_RGB565, CAM_NPIX_VGA, sensorbuf);
					  if(turn==0)
                                            camera_PrepareImageCaptureL(CAM_FORMAT_RGB565, MY_NPIX, sensorbuf);
					  else
					        camera_PrepareImageCaptureL(CAM_FORMAT_RGB565, MY_NPIX, sensorbuf2);

					  camera_CaptureImage();

					  return ;
				}

	  			    sensorready = 1;
	
			    //mmc_SetCurrMode(MMC_MODE_CAMERA);
                         mmc_SendEvent(GetMCITaskHandle(MBOX_ID_MMC), MSG_CAM_CALLBACK);
			}
		else  //capture
			{
			    if(countdown>0)//countdown
				{
					countdown--;
		                     camera_PrepareImageCaptureL(CAM_FORMAT_RGB565, MY_NPIX, sensorbuf);
		                     camera_CaptureImage();
		                    return ;
				}
			
			    Lily_Camera_PreviewOff();//priew off????
			    sensorready = 1;
				sensorCaptureReady = 1;
                        // mmi_trace(1,"privew poweroff and data is ready");

			}


}

void mmc_CamBlendDsp()
{
#if (CSW_EXTENDED_API_LCD == 0)
	uint32  local;
#endif
	uint16 tmpw,tmph;
	uint32 count=0;
	uint32  c1;
	CutZoomInStruct   cutzoom;
	mmcCameraErrordatatcnt = 0;
	MCI_TRACE(MCI_CAMERA_TRC|TSTDOUT, 0, "[MMC_CAMERA]mmc_CamBlendDsp tick: %d",hal_TimGetUpTime());
#if 0
         display_240by240(sensorbuf,blendBuffer2);
#if (CSW_EXTENDED_API_LCD == 1)
	         displayFbw.fb.buffer = (UINT16*) blendBuffer2;
	        // Display the blended image
	         LCDD_ERR_T err = lcdd_Blit16(&displayFbw,0 ,0);
	         // In case the LCD was busy (maybe it is already doing a dma)
	         // Retry until it is available
	         // We could add a timeout here
	        while (LCDD_ERR_NO != err)
	        {
	            sxr_Sleep(10);
	            err = lcdd_Blit16(&displayFbw,0 ,0);
	        }; 
#else
			  local=get_lcd_frame_buffer_address();
			  set_lcd_frame_buffer_address((kal_uint32)blendBuffer2);
			  previewstatus = 0;
			  MainLCD->BlockWrite(0,0,MMC_LcdWidth-1,MMC_LcdHeight-1);
			  previewstatus = 1;
			  set_lcd_frame_buffer_address(local);
#endif

              camera_PrepareImageCaptureL(CAM_FORMAT_RGB565, MY_NPIX, sensorbuf);
              camera_CaptureImage();

		return;
#else

      if(CameraInfo.preview_width==MMC_LcdWidth&&CameraInfo.preview_height==MMC_LcdHeight)
      	{
             cutheight = srcheight*10/(10+2*(CameraInfo.previewZoom));
             cutwidth   =   cutheight*3>>2;//cutwidth  = (srcheight<<3)/(10+2*CameraInfo.previewZoom);


		if(MMC_LcdWidth>MMC_LcdHeight)
		{
			if(((srcwidth*100)/srcheight)>((MMC_LcdWidth*100)/MMC_LcdHeight))
			{		
			 	cutwidth  = ((srcheight*CameraInfo.preview_width/CameraInfo.preview_height)*10/(10+2*(CameraInfo.previewZoom)))&0xfffffffe;
				cutheight = srcheight*10/(10+2*(CameraInfo.previewZoom));			
			}
			else
			{
			 	cutwidth  = srcwidth*10/(10+2*(CameraInfo.previewZoom));
			 	cutheight = ((srcwidth*CameraInfo.preview_height/CameraInfo.preview_width)*10/(10+2*(CameraInfo.previewZoom)))&0xfffffffe;				
			}
		}
		else
		{
			cutheight = srcheight*10/(10+2*(CameraInfo.previewZoom));
			cutwidth   =   cutheight*3>>2;//cutwidth  = (srcheight<<3)/(10+2*CameraInfo.previewZoom);//		}
		}

	      
      	}
	else
      {
	     cutwidth  = srcwidth*10/(10+2*(CameraInfo.previewZoom));
	     cutheight = srcheight*10/(10+2*(CameraInfo.previewZoom));
      }
	  
	  tmpw = CameraInfo.preview_width;
	  tmph = CameraInfo.preview_height;
         count = ((MMC_LcdHeight-tmph)>>1)*MMC_LcdWidth;
	   
	if(turn==0)
	{
		 //this part is to generate preview image<1>
		   cutzoom.inStreamBufAddr=(INT32*)sensorbuf;
		   cutzoom.outStreamBufAddr=(INT32*)(blendBuffer+count);
		   cutzoom.blendBufAddr=(INT32*)(backbuffer+count);
		   cutzoom.src_w=srcwidth;
		   cutzoom.src_h=srcheight;
		   cutzoom.cut_w=cutwidth;
		   cutzoom.cut_h=cutheight;
		   cutzoom.dec_w=tmpw;
		   cutzoom.dec_h=tmph;
		   cutzoom.reset = 1;
	 	 MMC_Preview(&cutzoom);
#if 0
		if(count!=0)
	      {
	        memcpy(blendBuffer,backbuffer,count<<1);
		 memcpy(blendBuffer+(tmph*MMC_LcdWidth+count),backbuffer+(tmph*MMC_LcdWidth+count),count<<1);
	        }
#else	
	       c1 = hal_TimGetUpTime();
		Mmc_blendPartPure(backbuffer,MMC_LcdWidth,MMC_LcdHeight,blendBuffer2,tmpw,tmph,0);
		MCI_TRACE(MCI_CAMERA_TRC, 0, "[MMC_CAMERA]CPU BLEND TIME:%d",hal_TimGetUpTime()-c1);
              
#endif	  
		 //this part is to display the image to  the lcd screen<2>
		 c1 = hal_TimGetUpTime();

#if (CSW_EXTENDED_API_LCD == 1)
	         displayFbw.fb.buffer = (UINT16*) blendBuffer2;
	        // Display the blended image
	         LCDD_ERR_T err = lcdd_Blit16(&displayFbw,0 ,0);
	         // In case the LCD was busy (maybe it is already doing a dma)
	         // Retry until it is available
	         // We could add a timeout here
	        while (LCDD_ERR_NO != err)
	        {
	            sxr_Sleep(10);
	            err = lcdd_Blit16(&displayFbw,0 ,0);
	        }; 
#else
			  local=get_lcd_frame_buffer_address();
			  set_lcd_frame_buffer_address((kal_uint32)blendBuffer2);
			  previewstatus = 0;
			  MainLCD->BlockWrite(0,0,MMC_LcdWidth-1,MMC_LcdHeight-1);
			  previewstatus = 1;
			  set_lcd_frame_buffer_address(local);
#endif

         //    MCI_TRACE(MCI_CAMERA_TRC, 0, "[MMC_CAMERA]LCD Update time=%d",hal_TimGetUpTime()-c1);
              

              camera_PrepareImageCaptureL(CAM_FORMAT_RGB565, MY_NPIX, sensorbuf2);
              camera_CaptureImage();

		turn = 1;	  
	}
	else
	{
		 //this part is to generate preview image<1>
		   cutzoom.inStreamBufAddr=(INT32*)sensorbuf2;
		   cutzoom.outStreamBufAddr=(INT32*)(blendBuffer2+count);
		   cutzoom.blendBufAddr=(INT32*)(backbuffer+count);
		   cutzoom.src_w=srcwidth;
		   cutzoom.src_h=srcheight;
		   cutzoom.cut_w=cutwidth;
		   cutzoom.cut_h=cutheight;
		   cutzoom.dec_w=tmpw;
		   cutzoom.dec_h=tmph;
		   cutzoom.reset = 1;
	 	   MMC_Preview(&cutzoom);

#if 0		   
	        if(count!=0)
	         {
	         memcpy(blendBuffer2,backbuffer,count<<1);
		  memcpy(blendBuffer2+(tmph*MMC_LcdWidth+count),backbuffer+(tmph*MMC_LcdWidth+count),count<<1);
	         }
#else	        
		 c1 = hal_TimGetUpTime();
		 Mmc_blendPartPure(backbuffer,MMC_LcdWidth,MMC_LcdHeight,blendBuffer,tmpw,tmph,0);
               MCI_TRACE(MCI_CAMERA_TRC, 0, "[MMC_CAMERA]CPU BLEND TIME:%d",hal_TimGetUpTime()-c1);
#endif			  

		 //this part is to display the image to  the lcd screen<2>
	//	 c1 = hal_TimGetUpTime();
#if (CSW_EXTENDED_API_LCD == 1)
	         displayFbw.fb.buffer = (UINT16*) blendBuffer;
	        // Display the blended image
	         LCDD_ERR_T err = lcdd_Blit16(&displayFbw,0 ,0);
	         // In case the LCD was busy (maybe it is already doing a dma)
	         // Retry until it is available
	         // We could add a timeout here
	        while (LCDD_ERR_NO != err)
	        {
	            sxr_Sleep(10);
	            err = lcdd_Blit16(&displayFbw,0 ,0);
	        }; 
#else
			  local=get_lcd_frame_buffer_address();
			  set_lcd_frame_buffer_address((kal_uint32)blendBuffer);
			  previewstatus = 0;
			  MainLCD->BlockWrite(0,0,MMC_LcdWidth-1,MMC_LcdHeight-1);
			  previewstatus = 1;
			  set_lcd_frame_buffer_address(local);
#endif

       //        MCI_TRACE(MCI_CAMERA_TRC, 0, "[MMC_CAMERA]LCD Update time=%d",hal_TimGetUpTime()-c1);

               camera_PrepareImageCaptureL(CAM_FORMAT_RGB565, MY_NPIX, sensorbuf);
               camera_CaptureImage();

		turn = 0;
	}
 

#endif
      


//mmi_trace(GUGOGI,"mmc_CamBlendDsp end = %d",hal_TimGetUpTime());
}




int Lily_Camera_Capture(CAM_CAPTURE_STRUCT *data) 
{

	INT32 result = MDI_RES_CAMERA_SUCCEED;



      MCI_TRACE(MCI_CAMERA_TRC, 0, "[MMC_CAMERA]Lily_Camera_Capture time:%d",hal_TimGetUpTime());

      MCI_TRACE(MCI_CAMERA_TRC, 0, "[MMC_CAMERA]Lily_Camera_Capture data->image_width:%d,data->image_height:%d",data->image_width,data->image_height);

	  
      CameraInfo.image_width = data->image_width;
      CameraInfo.image_height= data->image_height;

     

#if 0
	if(0!=flash_enable)
	{
		camera_SetFlashL(CAM_FLASH_FORCED);
	}
	else
	{
		camera_SetFlashL(CAM_FLASH_NONE);
	}
#endif


      previewstop = 1;	
      countdown   = 1;
sensorCaptureReady = 0;
		
	if((CameraInfo.addFrame!=0 && CameraInfo.image_width == MMC_LcdWidth && CameraInfo.image_height == MMC_LcdHeight))
	{
		MCI_TRACE(MCI_CAMERA_TRC, 0, "[MMC_CAMERA]image_width == MMC_LcdWidth");
	      memcpy((void*)blendBuffer,(void*)get_lcd_frame_buffer_address(),MMC_LcdWidth*MMC_LcdHeight*2);
	}

        while(sensorready==0)
	{
	     ;
	}
	while(sensorCaptureReady == 0)
	{
	}

    if (multicaputure)
	{
	       MCI_TRACE(MCI_CAMERA_TRC, 0, "[MMC_CAMERA]cont save photo");
		if((CameraInfo.addFrame!=0 && CameraInfo.image_width == MMC_LcdWidth && CameraInfo.image_height == MMC_LcdHeight))
        		Lily_Capture_Preview();
		//result=Lily_camera_savePhoto();
	}
	
	//camera_SetFlashL(CAM_FLASH_NONE);
	
        return result;

}


int Lily_Capture_Preview()
{

    uint16 tmpwidth = 0,tmpheight = 0;
#if (CSW_EXTENDED_API_LCD == 0)
    kal_uint32 local;
#endif
   

	MCI_TRACE(MCI_CAMERA_TRC, 0, "[MMC_CAMERA]Lily_Capture_Preview,multicaputure=%d srcwidth=%d",multicaputure,srcwidth);

	tmpwidth = CameraInfo.preview_width;
	tmpheight = CameraInfo.preview_height;
	previewstatus = 0;
	#if (CSW_EXTENDED_API_LCD == 1)	
	mci_LcdStopBypass();
      #endif

	if((CameraInfo.addFrame!=0 && CameraInfo.image_width == MMC_LcdWidth && CameraInfo.image_height == MMC_LcdHeight))
		{
			MCI_TRACE(MCI_CAMERA_TRC, 0, "[MMC_CAMERA]Lily_Capture_Preview image_width == MMC_LcdWidth");
			 if(CoolSand_Preview(sensorbuf,srcwidth,srcheight,cutwidth,cutheight,backbuffer,tmpwidth,tmpheight))
			 	return MDI_RES_CAMERA_ERR_DISK_FULL;
		        Mmc_blendPartPure(blendBuffer,MMC_LcdWidth,MMC_LcdHeight,backbuffer,MMC_LcdWidth,MMC_LcdHeight,0);
			 memcpy((void*)sensorbuf,(void*)backbuffer,MMC_LcdWidth*MMC_LcdHeight*2);
		        Mmc_blendPartPure((uint16*)get_lcd_frame_buffer_address(),MMC_LcdWidth,MMC_LcdHeight,backbuffer,MMC_LcdWidth,MMC_LcdHeight,0);
         displayFbw.fb.buffer =(uint16*)backbuffer;

        // Display the blended image
         LCDD_ERR_T err = lcdd_Blit16(&displayFbw,0 ,0);
         // In case the LCD was busy (maybe it is already doing a dma)
         // Retry until it is available
         // We could add a timeout here
        while (LCDD_ERR_NO != err)
        {
            sxr_Sleep(10);
            err = lcdd_Blit16(&displayFbw,0 ,0);
        }; 

    	}
    else 
	{
		MCI_TRACE(MCI_CAMERA_TRC, 0, "[MMC_CAMERA]Lily_Capture_Preview else");
		
		if(CoolSand_Preview(sensorbuf,srcwidth,srcheight,cutwidth,cutheight,blendBuffer+(((MMC_LcdHeight-tmpheight)>>1)*MMC_LcdWidth),tmpwidth,tmpheight))
		return MDI_RES_CAMERA_ERR_DISK_FULL;
		
		if(CameraInfo.preview_width == MMC_LcdWidth && CameraInfo.preview_height == MMC_LcdHeight)
		{
			Mmc_blendPartPure((uint16*)get_lcd_frame_buffer_address(),MMC_LcdWidth,MMC_LcdHeight,blendBuffer,MMC_LcdWidth,MMC_LcdHeight,0);

#if (CSW_EXTENDED_API_LCD == 1)
			displayFbw.fb.buffer = (UINT16*) blendBuffer;
			// Display the blended image
			LCDD_ERR_T err = lcdd_Blit16(&displayFbw,0 ,0);
			// In case the LCD was busy (maybe it is already doing a dma)
			// Retry until it is available
			// We could add a timeout here
			while (LCDD_ERR_NO != err)
			{
				sxr_Sleep(10);
				err = lcdd_Blit16(&displayFbw,0 ,0);
			}; 
#else
			local=get_lcd_frame_buffer_address();
			set_lcd_frame_buffer_address((kal_uint32)blendBuffer);
			MainLCD->BlockWrite(0,0,MMC_LcdWidth-1,MMC_LcdHeight-1);
			set_lcd_frame_buffer_address(local);
#endif
		}
		else
		{
			//EmbedBmpDataIntoLcdShadowBuffer((MMC_LcdWidth-tmpwidth)>>1,(MMC_LcdHeight-tmpheight)>>1, tmpwidth,tmpheight, blendBuffer, (uint16*)get_lcd_frame_buffer_address());
			Mmc_blendPartPure((uint16*)get_lcd_frame_buffer_address(),MMC_LcdWidth,MMC_LcdHeight,blendBuffer,tmpwidth,tmpheight,0);

#if (CSW_EXTENDED_API_LCD == 1)
			displayFbw.fb.buffer = (UINT16*) blendBuffer;
			// Display the blended image
			LCDD_ERR_T err = lcdd_Blit16(&displayFbw,0 ,0);
			// In case the LCD was busy (maybe it is already doing a dma)
			// Retry until it is available
			// We could add a timeout here
			while (LCDD_ERR_NO != err)
			{
				sxr_Sleep(10);
				err = lcdd_Blit16(&displayFbw,0 ,0);
			}; 
#else
			MainLCD->BlockWrite(0,0,MMC_LcdWidth-1,MMC_LcdHeight-1);
#endif

		}
	}


	return MDI_RES_CAMERA_SUCCEED;

}

//extern void mmi_camera_get_next_filename(char* file_buf_p);
//extern U16 UnicodeToAnsii(S8 *pOutBuffer, S8 *pInBuffer );
//extern U16 AnsiiToUnicodeString(S8 *pOutBuffer, S8 *pInBuffer );
int  Coolsand_VGA_zoom_op_yuv(uint16* srcbuf,uint16 src_w,uint16 src_h,uint16 cut_w,uint16 cut_h,uint16* decbuf,uint16 dec_w,uint16 dec_h)
{
		short  offx,offy;
		short  wa,wb,i,j,scalex,scaley;
		int    w;
		uint16  *pb ,*pa,*p1,*p2,*p3,*p4,*p5,*p6;
		short y,u,v,y1,u1,v1;
		short*  tmpbuf =(short*)mmc_MemMalloc(dec_w*8) ;
		short*  ptmp2 = tmpbuf + dec_w*2;
		short  num1,num2,num3;
		int    flag=0;

              if(tmpbuf==NULL)
			  	return -1;

		offx = (src_w - cut_w)>>1;
		offx = (offx>>1)<<1;
		offy = (src_h - cut_h)>>1;
	
		
		scalex=((cut_w-1)<<S15)/dec_w;
		scaley=((cut_h-1)<<S15)/dec_h;
		
	    pa = srcbuf + offy*src_w + offx;
           pb = decbuf;
		
		w  = 0;
		num2 = -2;
		num3 = 1;

		p3 = pa;
		p4 = p3 + src_w;
		p1 = NULL;
		p2 = NULL;
		//zoom_line_yuv_normal(p3,tmpbuf,dec_w,scalex);
		//zoom_line_yuv_normal(p4,ptmp2,dec_w,scalex);
		//p1 = tmpbuf;
		//p2 = ptmp2;


		for (j=0;j<dec_h;j++)
			{

				num1 = (w>>S15);
				if ( !flag &&j>=offy + num1 )
				{
					//break;
					flag = 1;//break;
					memcpy(blendBuffer,srcbuf+ offx+j*src_w,j*src_w*2);
					w = w&MASK;
					num1 = 0;
					num2 -= num1;
					pa = blendBuffer;
				}
				
			    p3 = pa + (num1*src_w);
				p4 = p3 + src_w;

				if (num1-num2==1)
				{
					if (num3==1)
					{
						zoom_line_yuv(p4,tmpbuf,dec_w,scalex);
						p2 = tmpbuf;
						p1 = ptmp2;
						num3=0;
					}
					else
					{
						zoom_line_yuv(p4,ptmp2,dec_w,scalex);
						p1 = tmpbuf;
						p2 = ptmp2;
						num3 = 1;
					}
					
				}
				else if (num1!=num2)
				{
					zoom_line_yuv(p3,tmpbuf,dec_w,scalex);
					zoom_line_yuv(p4,ptmp2,dec_w,scalex);
					p1 = tmpbuf;
					p2 = ptmp2;
					num3 = 1;
					
				}
				
				
				wb = w&MASK;
				wa = Q15 - wb;
				
				p3 = p1;
				p4 = p2;
				p5 = p1 + dec_w;
			    p6 = p2 + dec_w;
				for (i=0;i<(dec_w>>1);i++)
				{
					//*pb++ = (*p1++)*wa + (*p2++)*wb;
					y = *p3++;
					u = *p5++;
					y1 = *p4++;
					u1 = *p6++;

					y = ((y*wa + y1*wb)>>S15);
					u = ((u*wa + u1*wb)>>S15);
					
					*pb++ = (u<<8)|(y);
					/////////////////////////////
					y = *p3++;
					v = *p5++;
					y1 = *p4++;
					v1 = *p6++;

					y = ((y*wa + y1*wb)>>S15);
					v = ((v*wa + v1*wb)>>S15);
					*pb++ = (v<<8)|(y);
					
				}
				w+=scaley;
				num2 = num1;
			}

	mmc_MemFreeLast((char*)tmpbuf);

	return 0;
}


int Lily_camera_savePhoto(int  file_handle)
{
	
	//int 	file_handle;
	int len=0;
	//char buf_filename[ 520 ];
	//char filename_xuml[520];
	//uint8 tmp[128]="/t";
	//FS_INFO  fsinfo;
	//uint16 tmpwidth = 0,tmpheight = 0;


	MCI_TRACE(MCI_CAMERA_TRC, 0,  "[MMC_CAMERA]Lily_camera_savePhoto");

	//FS_GetFSInfo("MMC0", &fsinfo);
	// if( fsinfo.iTotalSize-fsinfo.iUsedSize<=0)
	//	return  MDI_RES_CAMERA_ERR_DISK_FULL;

	if(file_handle<0)
	{
	   MCI_TRACE(MCI_CAMERA_TRC|TSTDOUT, 0,  "[MMC_CAMERA_ERROR]FILE HANDLE IS INVALIBLE!");
	   return MDI_RES_CAMERA_ERR_FAILED;
	}
#if 0
	FS_Write(file_handle, (uint8 *)sensorbuf,320*240*2);
	return MDI_RES_CAMERA_SUCCEED;	
#endif


	initVocJpegEncode();//init encode jpeg voc

	//get a new filename and creat it
	//mmi_camera_get_next_filename(buf_filename);
#if 0
	UnicodeToAnsii(filename_xuml, buf_filename);
	strcat(tmp,filename_xuml+2);
	AnsiiToUnicodeString(buf_filename,tmp);
#endif
	// file_handle = MMI_FS_Open(buf_filename, FS_CREATE_ALWAYS);



	if(b_vga)
	{
		if((CameraInfo.addFrame!=0 && CameraInfo.image_width == MMC_LcdWidth && CameraInfo.image_height == MMC_LcdHeight))
		{
			MMC_jpgEn(CameraInfo.image_width,CameraInfo.image_height,CameraInfo.image_quality,(char*)sensorbuf,(char*)sensorbuf,0);
		}
		else if(CameraInfo.image_width<640)
		{
			if(CoolSand_Cut_Zoom(sensorbuf,srcwidth,srcheight,cutwidth,cutheight,blendBuffer,CameraInfo.image_width,CameraInfo.image_height))
			{
				quitVocJpegEncode();
				return MDI_RES_CAMERA_ERR_DISK_FULL;
			}
			MMC_jpgEn(CameraInfo.image_width,CameraInfo.image_height,CameraInfo.image_quality,(char*)convertaddr(blendBuffer),(char*)sensorbuf,yuvmode);
		}
		else
		{
			if(cutheight!=480 || cutwidth!=640)
			{
				if(Coolsand_VGA_zoom_op_yuv(sensorbuf,srcwidth,srcheight,cutwidth,cutheight,sensorbuf,640,480)) 
				{
					quitVocJpegEncode();
					return MDI_RES_CAMERA_ERR_DISK_FULL;
				}
			}

			MMC_jpgEn(CameraInfo.image_width,CameraInfo.image_height,CameraInfo.image_quality,(char*)sensorbuf,(char*)sensorbuf,yuvmode);
			if (multicaputure && cutwidth!=640)
			{
				memcpy(backbuffer,(void*)get_lcd_frame_buffer_address(),MMC_LcdWidth*MMC_LcdHeight*2);
			}

		}

              UINT32 time1=hal_TimGetUpTime();
		len= FS_Write(file_handle, (uint8 *)sensorbuf+1,getJpegEncodeLen());
		MCI_TRACE(MCI_CAMERA_TRC|TSTDOUT, 0, "[MMC_CAMERA]FS_Write TIME: %d",(hal_TimGetUpTime()-time1));
		MCI_TRACE(MCI_CAMERA_TRC, 0, "[MMC_CAMERA]SAVE PHOTO LEN=%d,actual len=%d",getJpegEncodeLen(),len);
		
		//MMI_FS_Close(file_handle);
		quitVocJpegEncode();
		if(len<=0)
		{
			//len = MMI_FS_Delete(buf_filename);
			//mmi_trace(GUGOGI,"DELETE RESULT=%d",len);
			return MDI_RES_CAMERA_ERR_DISK_FULL;
		}
	
	}
	else
	{/////////////////////////////////////////////////////////////////////////////////////////

		if((CameraInfo.addFrame!=0 && CameraInfo.image_width == MMC_LcdWidth && CameraInfo.image_height == MMC_LcdHeight))
		{
			MMC_jpgEn(CameraInfo.image_width,CameraInfo.image_height,CameraInfo.image_quality,(char*)sensorbuf,(char*)sensorbuf,0);
		}
		else if(CameraInfo.image_width<640)
		{
			if(cutwidth==320 && CameraInfo.image_width==320)
			{
				MMC_jpgEn(CameraInfo.image_width,CameraInfo.image_height,CameraInfo.image_quality,(char*)sensorbuf,(char*)sensorbuf,yuvmode);
			}
			else 
			{
				CoolSand_Cut_Zoom(sensorbuf,srcwidth,srcheight,cutwidth,cutheight,sensorbuf+320*240,CameraInfo.image_width,CameraInfo.image_height);
				MMC_jpgEn(CameraInfo.image_width,CameraInfo.image_height,CameraInfo.image_quality,(char*)(sensorbuf+320*240),(char*)sensorbuf,yuvmode);

			}
		}
		else
		{

			//memcpy(blendBuffer,sensorbuf+320*179,320*61*2);
			memcpy(sensorbuf+320*780 , sensorbuf , 320*180*2);

			CoolSand_Cut_Zoom(sensorbuf+320*780,320,240,cutwidth,cutheight,sensorbuf,640,480);
			MMC_jpgEn(CameraInfo.image_width,CameraInfo.image_height,CameraInfo.image_quality,(char*)sensorbuf,(char*)sensorbuf,yuvmode);
		}


		len= FS_Write(file_handle, (uint8 *)sensorbuf+1,getJpegEncodeLen());
		MCI_TRACE(MCI_CAMERA_TRC, 0, "[MMC_CAMERA]SAVE PHOTO LEN=%d,actual len=%d",getJpegEncodeLen(),len);
		//MMI_FS_Close(file_handle);
		quitVocJpegEncode();
		if(len<=0)
		{
			//len = MMI_FS_Delete(buf_filename);
			//mmi_trace(GUGOGI,"DELETE RESULT=%d",len);
			return MDI_RES_CAMERA_ERR_DISK_FULL;
		}
	
	}



	return MDI_RES_CAMERA_SUCCEED;	
	
}


void Camera_UserMsg(COS_EVENT *pnMsg)
{
	MCI_TRACE(MCI_CAMERA_TRC, 0,"[MMC_CAMERA]Camera_UserMsg pnMsg->nEventId:%d ",pnMsg->nEventId);
#if 1
	switch (pnMsg->nEventId)
	{
		case  (MSG_CAM_CALLBACK):	
			mmc_CamBlendDsp();					
			break;
			
		case (MSG_CAM_CALLBACK_VGA):
			mmc_CamBlendDsp_VGA();					
			break;
			
		case EV_TIMER:
			switch(pnMsg->nParam1)
			{
				case MMC_CAMERA_TIMER_ID:
					MCI_TRACE(MCI_CAMERA_TRC, 0,"[MMC_CAMERA]Camera_UserMsg  EV_TIMER");

					if(bset<2)//bset==2 means the camera is power off
					Lily_Camera_SetEffect(CAM_PARAM_WB,cam_wbValue);

				//if(ptimer)
				//    StopMyTimer(EV_VIDEO_TIMER_ID);  
					break;
				default:
					MCI_TRACE(MCI_CAMERA_TRC, 0,"[MMC_CAMERA]Invalid timer ID in  camera");
					break;

			}
			break;
			
		default:
			MCI_TRACE(MCI_CAMERA_TRC, 0,"[MMC_CAMERA]Camera_UserMsg:%d",pnMsg->nEventId);
			break;
	}


#else
	switch (nMsg)
	{

	case  (MCI_CAM_PREVIEW_CALLBACK-MCI_CAM_POWER_UP_REQ):	
	mmi_trace(1,"mmc_CamBlendDsp");
	mmc_CamBlendDsp();					
	break;
	default:
	mmi_trace(1,"Camera_UserMsg,default,nMsg is %ld",nMsg);
	break;
	}
#endif	
}

#ifndef TARGET_VIDEO_RECORDER_SUPPORT_OFF
extern volatile int32 Effect_Set_Valid;
#endif

int Lily_Camera_SetEffect(CAM_SET_PARA effectCode,uint32 value)
{
	uint32 localEffectCode=0;
	uint32 localValue=0;

	countdown = 1;
#ifndef TARGET_VIDEO_RECORDER_SUPPORT_OFF
	Effect_Set_Valid=1;
#endif
	switch(effectCode)
	{
		case CAM_PARAM_CONTRAST:
			localEffectCode = CameraSetContrast;
			localValue =5- value;
			if(localValue == 0)
			{
				localValue = 0;
			}
			//cameffect.contrast = value;
			camera_SetContrastL(localValue);
			break;
			
			
		case CAM_PARAM_BRIGHTNESS:
			localEffectCode = CameraSetBrightness;
			localValue = value+1;
			if(localValue > 5)
			{
				localValue = 5;
			}
			//cameffect.bright = value;
			camera_SetBrightnessL(localValue);
			break;

			
		case CAM_PARAM_IMAGESIZE:

			if(MY_NPIX != CAM_NPIX_QQVGA)//camera
			{
				switch(value)
				{
					case 1:
						CameraInfo.image_width =  MMC_LcdWidth;
						CameraInfo.image_height = MMC_LcdHeight;
						CameraInfo.preview_width = MMC_LcdWidth;
						CameraInfo.preview_height= MMC_LcdHeight;
						break;
					case 2:
						CameraInfo.image_width =  176;
						CameraInfo.image_height = 220;
						CameraInfo.preview_width = MMC_LcdWidth;
						CameraInfo.preview_height= MMC_LcdHeight;
						break;
					case 3:
						CameraInfo.image_width =  320;
						CameraInfo.image_height = 240;

						CameraInfo.preview_width = MMC_LcdWidth;
						CameraInfo.preview_height= (MMC_LcdWidth*CameraInfo.image_height /CameraInfo.image_width)&0xfffffffe;
						break;
					case 4:
						CameraInfo.image_width =  640;
						CameraInfo.image_height = 480;
						CameraInfo.preview_width = MMC_LcdWidth;
						CameraInfo.preview_height= (MMC_LcdWidth*CameraInfo.image_height /CameraInfo.image_width)&0xfffffffe;
						break;
					case 5:
						CameraInfo.image_width =  1280;
						CameraInfo.image_height = 1024;
						CameraInfo.preview_width = MMC_LcdWidth;
						CameraInfo.preview_height= MMC_LcdWidth*3>>2;//(MMC_LcdWidth*CameraInfo.image_height /CameraInfo.image_width)&0xfffffffe;
						break;
					default:
						CameraInfo.image_width =  MMC_LcdWidth;
						CameraInfo.image_height = MMC_LcdHeight;
						break;
				}
			}
			else //video
			{
				switch(value)
				{
					case 1:
						CameraInfo.image_width =  MMC_LcdWidth;
						CameraInfo.image_height = MMC_LcdWidth*3>>2;
						break;
					default:
						CameraInfo.image_width =  MMC_LcdWidth;
						CameraInfo.image_height = MMC_LcdWidth*3>>2;
						break;
				}

				CameraInfo.preview_width = MMC_LcdWidth;
				CameraInfo.preview_height= MMC_LcdWidth*3>>2;
			}			
			break;		

			
		case CAM_PARAM_ADD_FRAME:
			CameraInfo.addFrame = value;
			break;	

						
		/*******the followings is ought to return each case****/		
		case CAM_PARAM_ZOOM_FACTOR:
			CameraInfo.previewZoom = value;
			break;

					
		case CAM_PARAM_QUALITY:
			if(value == 0)
			{
				CameraInfo.image_quality =0;
			}
			else
			{
				CameraInfo.image_quality = 1;
			}
			
			MCI_TRACE(MCI_CAMERA_TRC, 0, "[MMC_CAMERA]CAM_PARAM_QUALITY=%d",value);
			break;

			
		case CAM_PARAM_EFFECT:
			switch(value)
			{
				
				case MDI_CAMERA_EFFECT_NORMAL:
					localEffectCode = CAM_SPE_NORMAL;
					break;
					
				case MDI_CAMERA_EFFECT_ANTIQUE:
					localEffectCode = CAM_SPE_ANTIQUE;
					break;
					
				case MDI_CAMERA_EFFECT_REDRISH:
					localEffectCode = CAM_SPE_REDISH;
					break;

				case MDI_CAMERA_EFFECT_GREENRISH:
					localEffectCode = CAM_SPE_GREENISH;
					break;
					
				case MDI_CAMERA_EFFECT_BLUERISH:
					localEffectCode = CAM_SPE_BLUEISH;
					break;

				case MDI_CAMERA_EFFECT_BLACKWHITE:
					localEffectCode = CAM_SPE_BLACKWHITE;
					break;
					
				case MDI_CAMERA_EFFECT_NEGATIVENESS:
					localEffectCode = CAM_SPE_NEGATIVE;
					break;
					
				default:
					MCI_TRACE(MCI_CAMERA_TRC|TSTDOUT, 0, "[MMC_CAMERA_ERROR]ERROR CAM_PARAM_EFFECT");
					localEffectCode = MDI_CAMERA_EFFECT_NORMAL;
					break;
					
			}
			//cameffect.effect = value;
			MCI_TRACE(MCI_CAMERA_TRC, 0, "[MMC_CAMERA]localEffectCode=%d",localEffectCode);
			camera_SetSpecialEffect(localEffectCode);
			break;
			

		case CAM_PARAM_NIGHT_MODE:
			
			localEffectCode = CameraSetBrightness;
			localValue = value;
			//cameffect.nightmode = value;
			
			if(TRUE== localValue)
			{
				camera_SetNightMode(1);//CameraIocontrol(CameraNightModeEn,NULL,NULL);
			}
			else
			{
				camera_SetNightMode(0);//CameraIocontrol(CameraNightModeDis,NULL,NULL);
			}
			
			break;	

			
		case CAM_PARAM_WB:
			
			MCI_TRACE(MCI_CAMERA_TRC, 0, "[MMC_CAMERA]CAM_PARAM_WB=%d",value);
			
			switch(value)
			{
				case 0:
					camera_SetWhiteBalanceL(CAM_WB_AUTO); /*Auto */
					break;
				case 1:
					camera_SetWhiteBalanceL(CAM_WB_DAYLIGHT);/*̫*/
					break;
				case 2:
					camera_SetWhiteBalanceL(CAM_WB_TUNGSTEN);/*˿*/
					break;
				case 3:
					camera_SetWhiteBalanceL(CAM_WB_OFFICE);/*չ*/
					break;
				case 4:
					camera_SetWhiteBalanceL(CAM_WB_CLOUDY);/**/
					break;				
				default:
					camera_SetWhiteBalanceL(CAM_WB_AUTO); /*Auto */
					break;
			}
			break;
			
		case CAM_PARAM_FLASH:
			
			MCI_TRACE(MCI_CAMERA_TRC, 0, "[MMC_CAMERA]CAM_PARAM_FLASHlocalEffectCode=%d,value is %d",localEffectCode,value);
			flash_enable=value;
			break;
			
		default:
			break;

			
	}	
	
	return 0;	
}	

#endif // (CHIP_HAS_GOUDA != 1)

