
// all size and laddr defines are given in word size
#define SECTION_1_START            (((RAM_I_MIN)+0x300)/2)
#define SECTION_2_START            (((RAM_I_MIN)+0xd00)/2)

// start of different image sections
#define SECTION_JPEG_DEC_START      SECTION_1_START
#define SECTION_ZOOM_DEC_START      SECTION_1_START
#define SECTION_MP12_DEC_START      SECTION_1_START
#define SECTION_AAC_DEC_START       SECTION_2_START
#define SECTION_MP3_DEC_START       SECTION_2_START
#define SECTION_AMR_DEC_START       SECTION_2_START

// size of different image sections
#define SECTION_JPEG_DEC_SIZE       ((((RAM_I_MIN)+0x0c00)/2) - SECTION_JPEG_DEC_START)
#define SECTION_ZOOM_DEC_SIZE       ((((RAM_I_MIN)+0x0c00)/2) - SECTION_JPEG_DEC_START)
#define SECTION_MP12_DEC_SIZE       ((((RAM_I_MIN)+0x0700)/2) - SECTION_MP12_DEC_START)
#define SECTION_AAC_DEC_SIZE        ((((RAM_I_MIN)+0x2400)/2) - SECTION_AAC_DEC_START)
#define SECTION_MP3_DEC_SIZE        ((((RAM_I_MIN)+0x2400)/2) - SECTION_MP3_DEC_START)
#define SECTION_AMR_DEC_SIZE        ((((RAM_I_MIN)+0x1900)/2) - SECTION_AMR_DEC_START)
