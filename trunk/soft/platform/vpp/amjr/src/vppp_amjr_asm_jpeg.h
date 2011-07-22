#include "vppp_amjr_asm_map.h"
#include "vppp_amjr_asm_sections.h"
#include "voc2_library.h"
#include "voc2_define.h"

#define JPEG_ConstY_size 378

#define MAX_32		 		 		 		 		 		 0x7fffffffL
#define MIN_32		 		 		 		 		 		 0x80000000L
#define MAX_16		 		 		 		 		 		 0x7fff
#define MIN_16		 		 		 		 		 		 0x8000
#define INC_PRECISION                                        11
#define ZOOM_PRECISION                                       1024

// VoC_directive: PARSER_OFF

/*****************************
 **    Top functions     **
 *****************************/

void vpp_AmjrPreviewAndJpegEncode(void);
void vpp_AmjrPreview(void);
void vpp_AmjrPreviewZoom(void);
void vpp_AmjrJpegEncode(void);
void vpp_AmjrDecompressVssAdpcmTable(void);
void vpp_AmjrGenerateJpegEncHuffTable(void);
void vpp_AmjrJpegRead(void);

/*****************************
 **    Common functions     **
 *****************************/

void COOLSAND_ENCODE_JPEG(void);
void DMAE_READDATA(void);
void DMAI_WRITE(void);
void COOLSAND_encode_block(void);
void apply_fdct(void);
void format_number(void);
void COOLSAND_put_bits(void);
void COOLSNAD_flush_bits(void);
void COOLSNAD_DIV(void);
void DMAI_WRITE(void);
void COOLSAND_encode_block(void);
void apply_fdct(void);
void format_number(void);
void COOLSAND_put_bits(void);
void COOLSNAD_flush_bits(void);
void COOLSAND_ENCODE_AMRRING(void);
void MCU_QUA_RGB2YUV411(void);
void MCU_QUA_RGB2YUV422(void);
void MCU_QUA_RGB2YUV444(void);
void COOLSAND_YUVJPEG422(void);
void Convert_YUV422_Block(void);
void Convert_YUV411_Block(void);
void COOLSAND_Double2SingleLastRow_YUV(void);
void COOLSAND_Double2Single_YUV(void);
void COOLSAND_Double2SingleLastRow_YUV(void);
void COOLSAND_Double2Row_YUV(void);
void COOLSAND_1280BY1024_YUV422_JPEG(void);
void COOLSAND_MEMCopy_YUV(void);
void COOLSAND_YUV2JPEG411(void);
void COOLSAND_RGB2JPEG411(void);
void COOLSAND_RGB2JPEG422(void);
void COOLSAND_RGB2JPEG444(void);
void DEMAI_READ_RGBDATA(void);
void COOLSAND_1280BY1024_JPEG(void);
void COOLSAND_DMAIRead_1280BY1024Odd(void);
void COOLSAND_DMAIRead_1280BY1024Even(void);
void COOLSAN_Encode422MCUBlock(void);
void COOLSAND_Double2Row(void);
void COOLSAND_Double2SingleLastRow(void);
void COOLSAND_Double2Single(void);
void COOLSAND_MEMCopy(void);

// VoC_directive: PARSER_ON

