
#define   Q15      8192//16384//32768
#define   Q14      4096//8192//16384
#define   S15      13//14// 15
#define   MASK     0x00001fff//Q15-1

//#define   NUMIN_LINE       22  //x
//#define   NUMIN_LINE_1     21  //x - 1
//#define   NUMIN_LINE_2     20  //x - 2
//#define   NUMIN_LINE_UV    11  //x/2
//#define   NUMIN_LINE_UV_1  10  //x/2 -1

#define  INTPUT_COUNT      4000//X*4/3
#define  OUTPUT_COUNT      2560
#define  ZOOM_CONST_Y_SIZE   69

void vpp_AmjpZoom(void);
void CoolSand_SignBits2(void);
void CoolSand_Div2(void);
void Coolsand_UpdateP3_addr(void);
void CoolSand_Updatep3p4(void);
void Coolsand_UpdateP4_addr(void);
void Coolsand_zoomline_yuv411(void);
void COOLSAND_DMA_READ(void);
void Coolsand_bilinear_yuv4112rgb(void);

void vpp_AmjpDecompressVssAdpcm16Table(void);
void vpp_AmjpZoomGenerateYuv2RgbTable(void);
