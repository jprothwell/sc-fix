
// all size and laddr defines are given in word size
#define SECTION_1_START            (((RAM_I_MIN)+0x0200)/2)
#define SECTION_2_START            (((RAM_I_MIN)+0x1200)/2)

// start of different image sections
#define SECTION_JPEG_ENC_START      SECTION_1_START
#define SECTION_AMR_ENC_START       SECTION_2_START
#define SECTION_MP3_ENC_START       SECTION_2_START

// size of different image sections
#define SECTION_JPEG_ENC_SIZE       ((((RAM_I_MIN)+0x1200)/2) - SECTION_JPEG_ENC_START)
#define SECTION_AMR_ENC_SIZE        ((((RAM_I_MIN)+0x2200)/2) - SECTION_AMR_ENC_START)
#define SECTION_MP3_ENC_SIZE        ((((RAM_I_MIN)+0x2000)/2) - SECTION_MP3_ENC_START)
