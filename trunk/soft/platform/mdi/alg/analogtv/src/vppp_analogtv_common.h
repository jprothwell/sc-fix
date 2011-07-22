//#define   Q15      8192//16384//32768
//#define   Q14      4096//8192//16384
//#define   S15      13//14// 15
//#define   MASK     0x00001fff//Q15-1

#define   Q15      32767
#define   Q14      16384
#define   S15      15
#define   MASK     ((Q15-1)&0X0000FFFF)


//#define   Q15      16384
//#define   Q14      8192
//#define   S15      14
//#define   MASK     (Q15-1)

#define   NUMLINE      16
#define   YUV2RGBLINE  16
//#define   NUMLINE_1  23
#define   OUTPUT_COUNT    9000
#define   OUTLINE       32

#define READ_BURST_SIZE  8
#define WRITE_BURST_SIZE 1
void Coolsand_MAIN(void);
void CoolSand_SignBits2(void);
void CoolSand_Div2(void);

void Coolsand_zoomline_yuv(void);
void Coolsand_yuv2rgb565_oneline(void);
void Coolsand_AverageTwoLine(void);
void Coolsand_zoomin_rgbline(void);
void Coolsand_yuv2rgb_oneline(void);

void Coolsand_CameraPreview_Entrance(void);
void Coolsand_bilinear_preview_yuv_zoomout(void);
void Coolsand_YUV2RGB565_Directly(void);
void Coolsand_bilinear_preview_yuv_zoomin(void);

void COOLSAND_DMA_READ(void);
void COOLSAND_DMA_WRITE(void);
void COOLSAND_DMA_READ_BlendBuf(void);
void  Coolsand_WriteLast(void);

void COOLSAND_DMA_READ_Slowly(void);
void COOLSAND_DMA_READ_BlendBuf_Slowly(void);
void Coolsand_DMAWRITE_sb33bf(void);
void Coolsand_WriteLast_sb33bf_blitLCD(void);
void Coolsand_InitGlobal_Normal(void);
void Coolsand_InitGlobal_withZoom(void);
void Coolsand_yuv2rgb565_oneline_with_rotateDMA(void);
void Coolsand_YUV2RGB565_Rotate(void);
void Coolsand_yuv2rgb565_oneline_whenRotate(void);

void Coolsand_AverageTwoLine_withoutblend(void);
void Coolsand_bilinear_preview_yuv_zoomin_withoutblend(void);
void Coolsand_bilinear_preview_yuv_zoomout_withoutblend(void);




