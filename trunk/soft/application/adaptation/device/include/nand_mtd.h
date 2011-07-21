/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of COOLSAND Inc. (C) 2005
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("COOLSAND SOFTWARE")
*  RECEIVED FROM COOLSAND AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. COOLSAND EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES COOLSAND PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE COOLSAND SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. COOLSAND SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY COOLSAND SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND COOLSAND'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE COOLSAND SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT COOLSAND'S OPTION, TO REVISE OR REPLACE THE COOLSAND SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  COOLSAND FOR SUCH COOLSAND SOFTWARE AT ISSUE.
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
*****************************************************************************/

/*******************************************************************************
 *
 * Filename:
 * ---------
 *	NandMTD.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This is a NAND Flash MTD driver header file for Samsung and Toshiba flash devices.
 *
 * Author:
 * -------
 * -------
 * -------
 *
 *==============================================================================
 * 				HISTORY
 
 *------------------------------------------------------------------------------
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/

#ifndef NAND_MTD_H
#define NAND_MTD_H


/*******************************************************************************
 * Include Header Files
 *******************************************************************************/
//#include "drvpdn.h"
#include "cswtype.h"

/*******************************************************************************
 * define
 *******************************************************************************/

#ifndef NODEBUG
   #define DEBUG
#endif

#ifdef _MSC_VER
   #define INTERN _fastcall
   #define WATCOM_VOLATILE
#elif defined __BORLANDC__
   #define INTERN __fastcall
   #define WATCOM_VOLATILE
#else
   #define INTERN
   #define WATCOM_VOLATILE volatile   // Watcom compiler bug workaround
#endif


/*******************************************************************************
 * NFI address definition
 *******************************************************************************/
#define DMA_NFI  0xf

#if ((defined(MT6228)) || (defined(MT6229)) )
#define NFI_GPIO_CON    ((volatile UINT16P)(0x801201D0))		// gpio control pin
#define NFI_GPIO_DATA1  ((volatile UINT16P)(0x80120200))		// data bit 8-9
#define NFI_GPIO_DATA2  ((volatile UINT16P)(0x80120210))		// data bit 10-15
#elif ((defined(MT6219))|| (defined(MT6226))|| (defined(MT6226M)) || (defined(MT6227)))
#define NFI_GPIO_CON    ((volatile UINT16P)(0x80120180))
#else    
#define NFI_GPIO_CON    ((volatile UINT16P)(0x80120130))
#endif



#define NFI_base        (0x80090000)
#define NFI_ACCCON      ((volatile UINT16P)(NFI_base+0x0000))
#define NFI_PAGEFMT     ((volatile UINT16P)(NFI_base+0x0004))
#define NFI_OPCON       ((volatile UINT16P)(NFI_base+0x0008))
#define NFI_CMD         ((volatile UINT16P)(NFI_base+0x000C))
#define NFI_ADDNOB      ((volatile UINT16P)(NFI_base+0x0010))
#define NFI_ADDRL       ((volatile UINT32P)(NFI_base+0x0014))
#define NFI_ADDRM       ((volatile UINT16P)(NFI_base+0x0018))
#define NFI_DATAW       ((volatile UINT32P)(NFI_base+0x001C))
#define NFI_DATAWB      ((volatile UINT16P)(NFI_base+0x0020))
#define NFI_DATAR       ((volatile UINT32P)(NFI_base+0x0024))
#define NFI_DATARB		((volatile UINT16P)(NFI_base+0x0028))
#define NFI_PSTA        ((volatile UINT16P)(NFI_base+0x002C))
#define NFI_FIFOCON     ((volatile UINT16P)(NFI_base+0x0030))
#define NFI_CON         ((volatile UINT16P)(NFI_base+0x0034))
#define NFI_INTR        ((volatile UINT16P)(NFI_base+0x0038))
#define NFI_INTR_EN     ((volatile UINT16P)(NFI_base+0x003C))
#define NFI_PAGECNTR		((volatile UINT16P)(NFI_base+0x0040))
#define NFI_ADDRCNTR		((volatile UINT16P)(NFI_base+0x0044))
#define NFI_SYM0_ADDR   ((volatile UINT16P)(NFI_base+0x0050))
#define NFI_SYM1_ADDR   ((volatile UINT16P)(NFI_base+0x0054))
#define NFI_SYM2_ADDR   ((volatile UINT16P)(NFI_base+0x0058))
#define NFI_SYM3_ADDR   ((volatile UINT16P)(NFI_base+0x005C))
#define NFI_SYM0_DAT    ((volatile UINT32P)(NFI_base+0x0060))
#define NFI_SYM1_DAT    ((volatile UINT32P)(NFI_base+0x0064))
#define NFI_SYM2_DAT    ((volatile UINT32P)(NFI_base+0x0068))
#define NFI_SYM3_DAT    ((volatile UINT32P)(NFI_base+0x006C))
#define NFI_ERRDET      ((volatile UINT16P)(NFI_base+0x0070))
#define NFI_PAR0        ((volatile UINT16P)(NFI_base+0x0080))
#define NFI_PAR1        ((volatile UINT16P)(NFI_base+0x0084))
#define NFI_PAR2        ((volatile UINT16P)(NFI_base+0x0088))
#define NFI_PAR3        ((volatile UINT16P)(NFI_base+0x008C))
#define NFI_PAR4        ((volatile UINT16P)(NFI_base+0x0090))
#define NFI_PAR5        ((volatile UINT16P)(NFI_base+0x0094))
#define NFI_PAR6        ((volatile UINT16P)(NFI_base+0x0098))
#define NFI_PAR7        ((volatile UINT16P)(NFI_base+0x009C))
#define NFI_CSEL			((volatile UINT16P)(NFI_base+0x0100))


/*******************************************************************************
 * NFI register definition
 *******************************************************************************/
/* NFI_ACCCON - example
	*NFI_ACCCON = ACCCON | (((NFI_W2R<<6)|(NFI_WH<<4)) | ((NFI_WST<<2)|NFI_RLC));

	*NFI_ACCCON = 0x07C6	// C2R=111, W2R=11, WH=00, WST=00, RLT=10

*/
#define ACCCON						0x0700	// C2R=111
#define NFI_W2R					3			// 0:2T, 1:4T, 2:6T, 3:8T ; recommanded value=3
#define NFI_WH						3			// 0:0T, 1:1T, 2:2T, 3:3T ; recommanded value=1
#define NFI_WST					3			// 0:0T, 1:1T, 2:2T, 3:3T ; recommanded value=1
#define NFI_RLC					3			// 0:0T, 1:1T, 2:2T, 3:3T ; recommanded value=2


/* NFI_PAGEFMT; support for PAGEFMT_PageSize_BusWidth */
#define PAGEFMT_512_8BIT   	0x0020
#define PAGEFMT_512_16BIT  	0x0120
#define PAGEFMT_2K_8BIT			0x0025
#define PAGEFMT_2K_16BIT		0x0125

#define	IO_ACCESS_8BIT			8
#define	IO_ACCESS_16BIT		16


/* NFI_OPCON */
#define BURST_RD           	0x0001
#define BURST_WR           	0x0002
#define ECC_RD_INIT        	0x0010
#define ECC_WR_INIT        	0x0020
#define SINGLE_RD          	0x0100
#define NOB_BYTE           	0x1000
#define NOB_WORD           	0x2000
#define NOB_DWORD          	0x0000


/* Nand flash command */
#define RD_1ST_CMD         	0x00
#define RD_2ND_HALF_CMD     	0x01	// only for 512B page-size
#define RD_SPARE_CMD       	0x50	// only for 512B page-size
#define RD_2ND_CYCLE_CMD		0x30	// only for 2KB  page-size
#define INPUT_DATA_CMD     	0x80
#define PROG_DATA_CMD      	0x10
#define BLOCK_ERASE1_CMD   	0x60
#define BLOCK_ERASE2_CMD   	0xD0
#define RD_ID_CMD          	0x90
#define RD_STATUS_CMD      	0x70
#define RESET_CMD          	0xFF



/* NFI_PSTA */
#define STATUS_CMD         	0x1
#define STATUS_ADDR        	0x2
#define STATUS_DATAR       	0x4
#define STATUS_DATAW       	0x8
#define STATUS_BUSY        	0x100


/* NFI_FIFOCON */
#define RD_EMPTY_MASK      	0x001
#define RD_FULL_MASK       	0x002
#define WR_EMPTY_MASK      	0x004
#define WR_FULL_MASK      		0x008
#define FIFO_FLUSH         	0x010
#define RESET              	0x020


/* NFI_CON */
#define BYTE_RW_EN				0x8000
#define READ_CON_EN				0x0400
#define PROGRAM_CON_EN			0X0200
#define ERASE_CON_EN				0X0100
#define SW_PROGSPARE_EN    	0x0020
#define MULTI_PAGE_RD_EN   	0x0010
#define AUTOECC_ENC_EN     	0x0008
#define AUTOECC_DEC_EN     	0x0004
#define DMA_WR_EN          	0x0002
#define DMA_RD_EN          	0x0001


/* Status register */
#define RDY_BUSY           	0x40
#define PASS_FAIL          	0x01


#define IS_NFI_DMA_RUN 	 *(volatile kal_uint32 *)DMA_GLBSTA & (kal_uint32)DMA_GLBSTA_RUN(nfi_dmaport)

/* NFI_INTR_EN */
#define RD_COMPLETE_EN     	0x01
#define WR_COMPLETE_EN     	0x02
#define RESET_COMPLETE_EN  	0x04
#define ERASE_COMPLETE_EN  	0x08
#define ERR_DET_EN         	0x10
#define ERR_COR_EN         	0x20
#define BUSY_RETURN_EN     	0x40
#define ALL_EN             	0x7F


/* NFI_INTR */
#define RD_COMPLETE        0x0001
#define WR_COMPLETE        0x0002
#define RESET_COMPLETE     0x0004
#define ERASE_COMPLETE     0x0008
#define ERR_DET0           0x0010
#define ERR_DET1           0x0020
#define ERR_DET2           0x0040
#define ERR_DET3           0x0080
#define ERR_COR0           0x0100
#define ERR_COR1           0x0200
#define ERR_COR2           0x0400
#define ERR_COR3           0x0800
#define BUSY_RETURN        0x1000



/*******************************************************************************
 * Dual NAND declaration
 *******************************************************************************/
typedef struct
{
	int     (* ProgramPage)    (void * DriveData, DWORD PageIndex, BYTE * Data, BYTE * Spare, BYTE SpareFlag);
	int     (* ReadPage)       (void * DriveData, DWORD PageIndex, BYTE * Data, BYTE * Spare, BYTE SpareFlag);
	int     (* ProgramSpare)   (void * DriveData, DWORD PageIndex, BYTE * Data);
	int     (* ReadSpare)      (void * DriveData, DWORD PageIndex, BYTE * Data);
} NAND_MTD_SUB_DRIVER;


typedef struct drive_data_tag
{
	DWORD		base_page;
   BYTE     address_cycle;	 	// 3, 4, or 5
   BYTE		io_width;			// 8 or 16 bits
   NAND_MTD_SUB_DRIVER * sub_driver;
}  NAND_MTD_SUB_DRIVER_DATA;



// define NFI CS0/1 DIR, DOUT, CON
#if ( (defined(MT6218B)) )
#define NFI_CS_DIR		((volatile UINT16P)(0x80120010))		// GPIO 30 as NFI CS0
#define NFI_CS_DOUT		((volatile UINT16P)(0x801200A0))
#elif ( (defined(MT6219)) )
#define NFI_CS_DIR		((volatile UINT16P)(0x80120010))		// GPIO 30 as NFI CS0
#define NFI_CS_DOUT		((volatile UINT16P)(0x801200D0))
#endif



#if ( (defined(MT6218B)) || (defined(MT6219)))
#define INITIALIZE_NFI_DIR_DOUT \
{\
	*NFI_CS_DIR |= 0x4100;\
	*NFI_CS_DOUT |= 0x4100;\
}

// set GPIO 30 as NFI CS0
#define SET_NFI_CS0 \


// set GPIO 30 as GPIO and GPIO 24 DOUT=0
#define SET_NFI_CS1 \
{\
	*NFI_GPIO_CON &= 0xCFFF;\
	*NFI_CS_DOUT &= 0xFEFF;\
}

// set GPIO 24 DOUT=1
#define SET_NFI_CS1_END \
{\
   *NFI_GPIO_CON |= 0x1000;\
	*NFI_CS_DOUT |= 0x0100;\
}
#else    
#define INITIALIZE_NFI_DIR_DOUT 

#define SET_NFI_CS0 \
	*NFI_CSEL = 0;

#define SET_NFI_CS1 \
	*NFI_CSEL = 1;

#define SET_NFI_CS1_END 

#endif



/*******************************************************************************
 * structure definition for flash supply list and MTD
 *******************************************************************************/
typedef struct flash_list_tag
{
   WORD     id;
   WORD     plane_size;      	// in MB
   WORD     block_size;      	// in KB
   WORD     page_size;       	// in B
   BYTE     address_cycle;	 	// 3, 4, or 5
   BYTE		io_width;			 // 8 or 16 bits
   NAND_MTD_SUB_DRIVER * sub_driver;
}  flash_list;


/*******************************************************************************/
typedef struct
{
	int     (* MountDevice)    (void * DriveData);
	int     (* ShutDown)       (void * DriveData);
	int     (* EraseBlock)     (void * DriveData, DWORD BlockIndex);
	int     (* ProgramPage)    (void * DriveData, DWORD PageIndex, BYTE * Data, BYTE * Spare, BYTE SpareFlag);
	int     (* ReadPage)       (void * DriveData, DWORD PageIndex, BYTE * Data, BYTE * Spare, BYTE SpareFlag);
	int     (* ProgramSpare)   (void * DriveData, DWORD PageIndex, BYTE * Data);
	int     (* ReadSpare)      (void * DriveData, DWORD PageIndex, BYTE * Data);
} NAND_MTD_DRIVER;



/*******************************************************************************
 *  Macro definition
 *******************************************************************************/
#define NOT_WAIT_NFI_DONE(a)  \
{\
   kal_uint16 dida = 0;\
   while(a)\
      if(dida++ > 0xFFF0) return 0;\
}

/*******************************************************************************
 *  MTD function declaration
 *******************************************************************************/
int MTD_MountDevice	(void * DriveData);
int MTD_ShutDown	(void * DriveData);
int MTD_EraseBlock	(void * DriveData, DWORD BlockIndex);


/*******************************************************************************
 *  Sub-function declaration
 *******************************************************************************/
int ProgramPage_512(void * DriveData, DWORD PageIndex, BYTE * Data, BYTE * Spare, BYTE SpareFlag);
int ReadPage_512(void * DriveData, DWORD PageIndex, BYTE * Data, BYTE * Spare, BYTE SpareFlag);
int ProgramSpare_512(void * DriveData, DWORD PagesIndex, BYTE * Data);
int ReadSpare_512(void * DriveData, DWORD PagesIndex, BYTE * Data);
int ProgramPage_2K(void * DriveData, DWORD PageIndex, BYTE * Data, BYTE * Spare, BYTE SpareFlag);
int ReadPage_2K(void * DriveData, DWORD PageIndex, BYTE * Data, BYTE * Spare, BYTE SpareFlag);
int ProgramSpare_2K(void * DriveData, DWORD PagesIndex, BYTE * Data);
int ReadSpare_2K(void * DriveData, DWORD PagesIndex, BYTE * Data);

int ProgramPage_512_default(void * DriveData, DWORD PageIndex, BYTE * Data, BYTE * Spare, BYTE SpareFlag);
int ReadPage_512_default(void * DriveData, DWORD PageIndex, BYTE * Data, BYTE * Spare, BYTE SpareFlag);
int ProgramSpare_512_default(void * DriveData, DWORD PagesIndex, BYTE * Data);
int ReadSpare_512_default(void * DriveData, DWORD PagesIndex, BYTE * Data);
int ProgramPage_2K_default(void * DriveData, DWORD PageIndex, BYTE * Data, BYTE * Spare, BYTE SpareFlag);
int ReadPage_2K_default(void * DriveData, DWORD PageIndex, BYTE * Data, BYTE * Spare, BYTE SpareFlag);
int ProgramSpare_2K_default(void * DriveData, DWORD PagesIndex, BYTE * Data);
int ReadSpare_2K_default(void * DriveData, DWORD PagesIndex, BYTE * Data);

int ProgramPage_512_specific(void * DriveData, DWORD PageIndex, BYTE * Data, BYTE * Spare, BYTE SpareFlag);
int ReadPage_512_specific(void * DriveData, DWORD PageIndex, BYTE * Data, BYTE * Spare, BYTE SpareFlag);
int ProgramSpare_512_specific(void * DriveData, DWORD PagesIndex, BYTE * Data);
int ReadSpare_512_specific(void * DriveData, DWORD PagesIndex, BYTE * Data);
int ProgramPage_2K_specific(void * DriveData, DWORD PageIndex, BYTE * Data, BYTE * Spare, BYTE SpareFlag);
int ReadPage_2K_specific(void * DriveData, DWORD PageIndex, BYTE * Data, BYTE * Spare, BYTE SpareFlag);
int ProgramSpare_2K_specific(void * DriveData, DWORD PagesIndex, BYTE * Data);
int ReadSpare_2K_specific(void * DriveData, DWORD PagesIndex, BYTE * Data);


void writefifo_512(kal_uint32* data, kal_uint32 * spare, BYTE ECC_flag, kal_uint32 length);
void writefifo_2K(kal_uint32* data, kal_uint32 * spare, BYTE ECC_flag, kal_uint32 length);
void readfifo_512(kal_uint32* data, kal_uint32* spare, BYTE ECC_flag, kal_uint32 length);
void readfifo_2K(kal_uint32* data, kal_uint32* spare, BYTE ECC_flag, kal_uint32 length);
void cmd_resetpointer(void);
void cmd_sparepointer(void);
kal_uint8 read_status(void);
kal_bool InitializeMTD(void * DriveData);
kal_bool InitializeMTDData(void * DriveData);
void nfi_lisr(void);
void nfi_hisr(void);
void NFI_DMA_Callback(void);
void pdn_nfi(kal_bool ON);




/******************************************************************************
 * SuperAND flash device definition                                           *
 ******************************************************************************/
#define SUPERAND_PAGE_READ_CMD  0x00    /* Can be recognized by NFI */
#define SUPERAND_SEQ_READ_CMD   0x0F    /* Can't be recognized by NFI */
#define SUPERAND_READ_END_CMD   0xF0    /* Can't be recognized by NFI */
#define SUPERAND_REWRITE_CMD    0x1F    /* Can't be recognized by NFI */
#define SUPERAND_PROG_CMD       0x10    /* Can be recognized by NFI */

#define SUPERAND_ADDR_CYCLE     0x04
#define SUPERAND_BYTES_PER_SECTOR   (SUPERAND_PAGES_PER_SECTOR << 9)
#define SUPERAND_PAGES_PER_SECTOR   0x04
#define SUPERAND_SECTOR_MASK    (SUPERAND_PAGES_PER_SECTOR - 1)
#define SUPERAND_PAGES_PER_EXT_SECTOR   0x08
#define SUPERAND_EXT_SSECTOR_MASK    (SUPERAND_PAGES_PER_EXT_SECTOR - 1)


#define SUPERAND_PAGEFMT_8BIT   0x0005
#define SUPERAND_PAGEFMT_16BIT  0x0105

#define SUPERAND_ACCCON         0x07FF

#define PAGES2BYTES(pages)      ((pages) << 9)
#define PAGES2DWORDS(pages)     ((pages) << 7)

#define MAX_DEVICE_NUM          2

typedef struct
{
   WORD     id;
   WORD     plane_size;      // in MBytes
   WORD     block_size;      // in KBytes
   BYTE		io_width;		 // 8 or 16 bits
}  superand_flash_list;
#define SUPERAND_FLASH_LIST_END {0xFFFF, 0, 0, 0}

typedef struct
{
	int     (* MountDevice)    (void * DriveData);
	int     (* ShutDown)       (void * DriveData);
	int     (* ReadPages)      (void * DriveData, DWORD PageIndex, DWORD Pages, BYTE * Data);
	int     (* RewritePages)   (void * DriveData, DWORD PageIndex, DWORD Pages, BYTE * Data);
	int     (* ProgramData)    (void * DriveData, DWORD PageIndex, DWORD Bytes, BYTE * Data);
	int     (* EraseBlock)     (void * DriveData, DWORD PageIndex);
	int     (* ReadGoodBlkCnt) (void * DriveData);
} SuperAND_MTD_DRIVER;

#if (defined(MT6218B) || defined(MT6219))
#define COMPUTE_ADDRESS_AND_SET_CSEL \
{\
    DWORD page_idx = PageIndex;\
    WORD *paddr = (WORD*)&addr;\
    paddr[0] = (D->io_width == 8) ? (WORD)((page_idx&0x3) << 9) : (WORD)((page_idx&0x3) << 8);\
    paddr[1] = page_idx >> 2;\
}
#else
#define COMPUTE_ADDRESS_AND_SET_CSEL \
{\
    DWORD page_idx = PageIndex;\
    WORD *paddr = (WORD*)&addr;\
    if(page_idx >= D->pages_of_first_device)\
    {\
        *NFI_CSEL = 0x1;\
        page_idx -= D->pages_of_first_device;\
    }else{\
        *NFI_CSEL = 0x0;\
    }\
    paddr[0] = (D->io_width == 8) ? (WORD)((page_idx&0x3) << 9) : (WORD)((page_idx&0x3) << 8);\
    paddr[1] = page_idx >> 2;\
}
#endif

void SuperAND_MTD_Init(void);
int SuperAND_MTD_MountDevice(void * DriveData);
int SuperAND_MTD_ShutDown(void * DriveData);
int SuperAND_MTD_ReadPages(void * DriveData, DWORD PageIndex, DWORD Pages, BYTE * Data);
int SuperAND_MTD_RewritePages(void * DriveData, DWORD PageIndex, DWORD Pages, BYTE * Data);
int SuperAND_MTD_ProgramData(void * DriveData, DWORD PageIndex, DWORD Bytes, BYTE * Data);
int SuperAND_MTD_EraseBlock(void * DriveData, DWORD PageIndex);
int SuperAND_MTD_ReadGoodBlkCnt(void * DriveData);

kal_uint16 superand_read_device_id(kal_uint16 csel);
kal_bool superand_checkdevice(void * DriveData);
kal_int32 superand_writefifo(void * DriveData, DWORD PageIndex, BYTE * Data, DWORD Bytes, kal_bool rewrite);
void superand_readfifo(BYTE * Data, DWORD Pages);
kal_uint8 superand_read_status(void);


extern DWORD custom_part_secs;

#endif


