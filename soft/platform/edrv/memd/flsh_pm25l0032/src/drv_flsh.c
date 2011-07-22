////////////////////////////////////////////////////////////////////////////////
//                                                                            //
/// @file drv_flsh.c                                                          //
/// That file provides the driver implementation for the SPI      //
/// flash.                                                                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include "cs_types.h"
#include "hal_ebc.h"
#include "hal_sys.h"
//#include "sxs_io.h"
#include "string.h"
#include "memd_m.h"
#include "memdp_debug.h"
#include "memdp.h"
#include "hal_host.h"
#define KBYTES 1024
#define MBYTES (1024*KBYTES)
#define FLASH_SIZE  4*MBYTES // 4MB

#define SECOND        * HAL_TICK1S
#define MILLI_SECOND  SECOND / 1000
#define MILLI_SECONDS MILLI_SECOND

// Get the page number from the _physical_ flash address
#define GET_PAGE_PHYS(PhysAddr)  (UINT8)(((UINT32)PhysAddr >> 22) & 0x7)

// =============================================================================
//  GLOBAL VARIABLES
// =============================================================================
/// Base address
UINT32 g_memdFlashBaseAddress = 0;

BOOL g_memdLockOverride = TRUE;
extern VOID mon_Event(UINT32 ch);

EXPORT PUBLIC BOOL spi_flash_ini(BOOL quard_flag, UINT8 clk_offset_val, UINT8 clkdiv_val, UINT8 dual_mode);

#define MREAD_8(addr) *((volatile UINT8 *)(addr))
#define MWRITE_8(addr,data) *((volatile UINT8 *)(addr)) = data

#define MREAD_16(addr) *((volatile UINT16 *)(addr))
#define MWRITE_16(addr,data) *((volatile UINT16 *)(addr)) = data

#define MREAD_32(addr) *((volatile UINT32 *)(addr))
//#define MWRITE_32(addr,data)  { *((volatile UINT32 *)(addr)) = data; hal_HstSendEvent(addr);  hal_HstSendEvent(data);}

#define MWRITE_32(addr,data)  { *((volatile UINT32 *)(addr)) = data; }

//#define MREAD_32(addr) (0)
//#define MWRITE_32(addr,data) 


// SPI FLASH INTERFACE
#define SPI_FLASH_REG_BASE 0xa1a25000
#define CMD_ADDR_OFFSET 0x0
#define MODE_BLOCKSIZE_OFFSET 0x4
#define DATA_FIFO_OFFSET 0x8
#define STATUS_OFFSET 0xc
#define READBACK_REG_OFFSET 0x10
#define FLASH_CONFIG_OFFSET 0x14
#define FIFO_CTRL_OFFSET 0x18
#define DUAL_SPI_OFFSET 0x1c
#define READ_CMD_OFFSET 0x20

#define SPI_CMD_ADDR (SPI_FLASH_REG_BASE+CMD_ADDR_OFFSET)
#define SPI_BLOCK_SIZE (SPI_FLASH_REG_BASE+MODE_BLOCKSIZE_OFFSET)
#define SPI_DATA_FIFO (SPI_FLASH_REG_BASE+DATA_FIFO_OFFSET)
#define SPI_STATUS (SPI_FLASH_REG_BASE+STATUS_OFFSET)
#define SPI_READ_BACK (SPI_FLASH_REG_BASE+READBACK_REG_OFFSET)
#define SPI_CONFIG (SPI_FLASH_REG_BASE+FLASH_CONFIG_OFFSET)
#define SPI_FIFO_CTRL (SPI_FLASH_REG_BASE+FIFO_CTRL_OFFSET)
#define SPI_CS_SIZE (SPI_FLASH_REG_BASE+DUAL_SPI_OFFSET)
#define SPI_READ_CMD (SPI_FLASH_REG_BASE+READ_CMD_OFFSET)

#define OPCODE_WRITE_ENABLE 0x06
#define OPCODE_READ_STATUS 0x05
#define OPCODE_WRITE_STATUS 0x01
#define OPCODE_READ_STATUS_1 0x35
#define OPCODE_QUAD_PAGE_PROGRAM 0x32
#define OPCODE_BLOCK_ERASE_64K 0xd8
#define OPCODE_SECTOR_ERASE 0x20
#define OPCODE_CHIP_ERASE 0x60
#define OPCODE_READ_DATA 0x03
#define OPCODE_QUAD_FAST_READ 0xeb


#define OPCODE_WRITE_ENABLE 0x06
#define OPCODE_RDSR 0x05
#define OPCODE_WRR 0x01
#define OPCODE_QPP 0x32
#define OPCODE_SE 0xd8
#define OPCODE_P4E 0x20
#define OPCODE_BE 0x60
#define OPCODE_READ 0x03
#define OPCODE_QIQR 0xeb
//for test OPCODE_HIGH_P_MODE
//#define OPCODE_HIGH_P_MODE 0xff
#define OPCODE_RDI 0xab
#define OPCODE_DP 0xb9
#define OPCODE_HIGH_P_MODE 0xa3
#define OPCODE_ReadReset 0xff
#define OPCODE_PROGRAM_ERASE_SUSPEND 0x75
#define OPCODE_PER 0x7a

//#define OPCODE_BLOCK_ERASE_64K      0x20    // sector erase

#define OPCODE_REMS    0x90    // read manufacture/device ID 
#define OPCODE_RDID    0x9f    // read identification
#define OPCODE_BE_1    0x52    // 32KB block erase
#define OPCODE_BE_2    0xd8    // 64kB block erase
#define SECTOR_SIZE 0x10000

/*****************************************************************/
//	FLASH_TYPE_ID = 0 : normal flash
//	FLASH_TYPE_ID = 1 : x25u3235e
//	FLASH_TYPE_ID = 2 : en25q32
/*****************************************************************/
#define FLASH_TYPE_ID   0
#define TOTAL_FLASH_ID  3

#define PAGE_SIZE (FLASH_TYPE_ID?128:256)


UINT32        RUN_Status4=0;
UINT32        RUN_Status=0;
UINT32        RUN_Status2=0;
UINT32        RUN_Status3=0;

PUBLIC void wait_flash_busy(void)
{
    UINT32 data_tmp_32;
    UINT32 addr_tmp;
    addr_tmp = SPI_STATUS;
    data_tmp_32 = MREAD_32(addr_tmp);   
    while (data_tmp_32&0x1)
    {
        data_tmp_32 = MREAD_32(addr_tmp);        
    }

    return ;
}


PUBLIC void wait_flash_read(void)
{
    UINT32 data_tmp_32;
    UINT32 addr_tmp;
    addr_tmp = SPI_STATUS;
    data_tmp_32 = MREAD_32(addr_tmp);   
    while ((data_tmp_32&0x8)==0)
    {
        data_tmp_32 = MREAD_32(addr_tmp);        
    }

    return ;
}


PRIVATE void SPI_Flash_Disable_Read(void)
{
#ifdef SPI_FLASH_CONTINUOUS_READ
    MWRITE_32(SPI_CS_SIZE_REG, (0x10));
#endif
    return ;
}


PRIVATE void SPI_Flash_Enable_Read(void)
{
#ifdef SPI_FLASH_CONTINUOUS_READ   
    MWRITE_32(SPI_CS_SIZE_REG, (0x0));
    wait_flash_busy();    
#endif
    return ;
}


PUBLIC BOOL flash_spi_busy(void)
{
    UINT32 data_tmp_32;
    UINT32 addr_tmp;
    addr_tmp = SPI_STATUS;
    data_tmp_32 = (UINT32)MREAD_32(addr_tmp);   
    return (((data_tmp_32&0x1)==1)||((data_tmp_32&0x8)==0));
}



/****************************************************************************/
/*	data_array[]: bytes array to be push									*/
/*	data_cnt:			push data count										*/
/*	quard_flag:		true for quard operation, false for serial operation;   */
/*								 only valid for extended operation			*/
/*	clr_flag:			true for clear fifo before push; false for nothing	*/												
/****************************************************************************/
PROTECTED BOOL push_fifo_data(UINT8 data_array[], UINT32 data_cnt, BOOL quard_flag, BOOL clr_flag)
{
    UINT32 data_tmp_32 = 0;
    UINT32 addr_tmp;
    UINT32 i=0;

    if(clr_flag)
    {
        addr_tmp = SPI_FIFO_CTRL;
        data_tmp_32 = 0x2;
        MWRITE_32(addr_tmp,data_tmp_32);
        
        addr_tmp = SPI_STATUS;
        data_tmp_32 = MREAD_32(addr_tmp);		
        while(((data_tmp_32&0x2)==0))
        {
            data_tmp_32 = MREAD_32(addr_tmp);	
        }
    }

    addr_tmp = SPI_STATUS;
    data_tmp_32 = MREAD_32(addr_tmp);		
    while (data_tmp_32&0x4)
        data_tmp_32 = MREAD_32(addr_tmp);	

    addr_tmp = SPI_DATA_FIFO;
    for(i = 0; i < data_cnt; i++)
    {
        data_tmp_32 = (UINT32)data_array[i];
        if(quard_flag)
            data_tmp_32 = data_tmp_32 | 0x100;
//wait_flash_busy();        
        MWRITE_32(addr_tmp, data_tmp_32);
    }
    return TRUE;
}



/****************************************************************************/
/*	data_array[]: bytes array to be push									*/
/*	data_cnt:			push data count										*/
/*	quard_flag:		true for quard operation, false for serial operation;   */
/*								 only valid for extended operation			*/
/*	clr_flag:			true for clear fifo before push; false for nothing	*/												
/****************************************************************************/
PROTECTED BOOL clear_fifo_data(BOOL clr_read_flag,BOOL clr_write_flag)
{
    UINT32 data_tmp_32 = 0;
    UINT32 addr_tmp;


    if(clr_write_flag)
    {
        addr_tmp = SPI_FIFO_CTRL;
        data_tmp_32 = 2;
        MWRITE_32(addr_tmp,data_tmp_32);
    }
    if(clr_read_flag)
    {
        addr_tmp = SPI_FIFO_CTRL;
        data_tmp_32 = 1;
        MWRITE_32(addr_tmp,data_tmp_32);
    }

    return TRUE;
}



/****************************************************************************/
/*	data_array[]: bytes array to be push									*/
/*	data_cnt:			push data count										*/
/*	quard_flag:		true for quard operation, false for serial operation;   */
/*								 only valid for extended operation			*/
/*	clr_flag:			true for clear fifo before push; false for nothing	*/												
/****************************************************************************/
PROTECTED BOOL enter_quad_mode(BOOL quad_mode)
{
    UINT32 data_tmp_32 = 0;
    UINT32 addr_tmp;
//    UINT32 i;

    addr_tmp = SPI_CONFIG;
    data_tmp_32 = MREAD_32(addr_tmp);
    data_tmp_32 = (data_tmp_32&(~1))|quad_mode;
    MWRITE_32(addr_tmp,data_tmp_32);

    return TRUE;
}

/********************************************************************/
/*	opmode = 0:	normal operation																		*/
/*	opmode = 1:	extended serial read																*/
/*	opmode = 2:	extended quard read																	*/
/*	opmode = 3:	extended write			(OPCODE_WRITE_ENABLE, flash_addr, opmode, 0);																*/
/********************************************************************/
PROTECTED void start_flash_operation(UINT8 cmd, UINT32 addr, UINT8 opmode, UINT8 blocksize)
{
    UINT32 data_tmp_32;
    UINT32 addr_tmp;
    //UINT32 addr_upper;
   if (OPCODE_HIGH_P_MODE == cmd ) return ;
   // addr_upper = (addr&0xfff000)<<8;

    if(blocksize!=0)
    {
        addr_tmp = SPI_BLOCK_SIZE;
        data_tmp_32 = MREAD_32(addr_tmp);
        data_tmp_32 = (data_tmp_32&(~0x1ff00))|(blocksize<<8);

        MWRITE_32(addr_tmp,data_tmp_32);
    }

    addr_tmp = SPI_CMD_ADDR;
    if(opmode==0)
    {	
        data_tmp_32 = ((addr<<8)&0xffffff00)|cmd;
        MWRITE_32(addr_tmp,data_tmp_32);
    }
    else if(opmode==1)
    {
        data_tmp_32 = (((cmd<<8)|0x10000)&0x000fff00);
        MWRITE_32(addr_tmp,data_tmp_32);
    }
    else if(opmode==2)
    {
        data_tmp_32 = (((cmd<<8)|0x30000)&0x000fff00);
        MWRITE_32(addr_tmp,data_tmp_32);
    }
    else if(opmode==3)
    {
        data_tmp_32 = ((cmd<<8)&0x000fff00);
        MWRITE_32(addr_tmp,data_tmp_32);
    }
    else
    {
        data_tmp_32 = ((cmd<<8)&0x000fff00);
        MWRITE_32(addr_tmp,data_tmp_32);
    }
    wait_flash_busy();   
    return;
}

PROTECTED UINT8 get_flash_data(void)
{
    UINT32 data_tmp_32;
    UINT32 addr_tmp;
    addr_tmp =SPI_STATUS;
    data_tmp_32 = MREAD_32(addr_tmp);
    while (data_tmp_32&0x8)
        data_tmp_32 = MREAD_32(addr_tmp);
 
    addr_tmp = SPI_READ_BACK;
    data_tmp_32 = MREAD_32(addr_tmp)&0xff;		
    return (UINT8)data_tmp_32;
}

PUBLIC UINT32 get_flash_status(UINT32 flash_addr)
{
    UINT32 data_tmp_32;
    UINT32 addr_tmp;
    UINT8 opmode;

    addr_tmp = SPI_CONFIG;
    data_tmp_32 = MREAD_32(addr_tmp);
    if(data_tmp_32&0x10000)
        opmode = 2;
    else if(FLASH_TYPE_ID)
        opmode = 1;
    else
        opmode = 0;
    
    wait_flash_busy();
    addr_tmp = SPI_FIFO_CTRL;
    data_tmp_32 = 0x1;		
    MWRITE_32(addr_tmp,data_tmp_32);

    wait_flash_busy();


    start_flash_operation(OPCODE_READ_STATUS, flash_addr, opmode, 1);
    wait_flash_busy();

    addr_tmp = SPI_READ_BACK;
    data_tmp_32 = MREAD_32(addr_tmp);
    return data_tmp_32;
}


PUBLIC UINT32 get_flash_status_h(UINT32 flash_addr)
{
    UINT32 data_tmp_32;
    UINT32 addr_tmp;
    UINT8 opmode;

    addr_tmp = SPI_CONFIG;
    data_tmp_32 = MREAD_32(addr_tmp);
    if(data_tmp_32&0x10000)
        opmode = 2;
    else if(FLASH_TYPE_ID)
        opmode = 1;
    else
        opmode = 0;
wait_flash_busy();
    addr_tmp = SPI_FIFO_CTRL;
    data_tmp_32 = 0x1;		
    MWRITE_32(addr_tmp,data_tmp_32);

wait_flash_busy();

    start_flash_operation(OPCODE_READ_STATUS_1, flash_addr, opmode, 1);
wait_flash_busy();

    addr_tmp = SPI_READ_BACK;
    data_tmp_32 = MREAD_32(addr_tmp);
    return data_tmp_32;
}



PUBLIC UINT32 get_flash_ID(UINT32 flash_addr)
{
    UINT32 data_tmp_32;
    UINT32 addr_tmp;
    UINT8 opmode;
    BOOL quard_flag;
    UINT8 data_array[2];
    UINT8 data_size;

    addr_tmp = SPI_CONFIG;
    data_tmp_32 = MREAD_32(addr_tmp);
    if(FLASH_TYPE_ID==0)
    {
        opmode = 0;
        data_array[1] = 0;
        data_array[0] = 0x0;
        data_size = 0;
    }
    else if(FLASH_TYPE_ID>=TOTAL_FLASH_ID)
    {
        opmode = 3;
        data_array[1] = 0;
        data_array[0] = 0x0;		
        data_size = 2;
    }
    else
    {
        opmode = 3;
        data_array[1] = 0x0;
        data_array[0] = 0;		
        data_size = 1;
    }

    if(data_tmp_32&0x10000)
        quard_flag = TRUE;
    else
        quard_flag = FALSE;
    wait_flash_busy();
    addr_tmp = SPI_FIFO_CTRL;
    data_tmp_32 = 0x3;		
    MWRITE_32(addr_tmp,data_tmp_32);

    wait_flash_busy();

    start_flash_operation(OPCODE_WRITE_ENABLE, flash_addr, opmode, 0);
    
    push_fifo_data(data_array,data_size,quard_flag,TRUE);

    start_flash_operation(OPCODE_RDID, flash_addr, opmode, 3);

    wait_flash_busy();

    data_tmp_32 = get_flash_status(flash_addr);
    while (data_tmp_32&0x1)
        data_tmp_32 = get_flash_status(flash_addr);

   wait_flash_busy();


    addr_tmp = SPI_READ_BACK;

    
    data_tmp_32 = MREAD_32(addr_tmp);
    data_tmp_32 = MREAD_32(addr_tmp);
    data_tmp_32 = MREAD_32(addr_tmp);
    
    return data_tmp_32;
}

PUBLIC void write_flash_status(UINT32 flash_addr, UINT8 status_val)
{
    UINT32 data_tmp_32;
    UINT32 addr_tmp;
    UINT8 opmode;
    BOOL quard_flag;
    UINT8 data_array[2];
    UINT8 data_size;

    addr_tmp = SPI_CONFIG;
    data_tmp_32 = MREAD_32(addr_tmp);
    if(FLASH_TYPE_ID==0)
    {
        opmode = 0;
        data_array[1] = 0;
        data_array[0] = status_val;
        data_size = 1;
    }
    else if(FLASH_TYPE_ID>=TOTAL_FLASH_ID)
    {
        opmode = 3;
        data_array[1] = status_val;
        data_array[0] = 0x0;		
        data_size = 2;
    }
    else
    {
        opmode = 3;
        data_array[1] = 0x0;
        data_array[0] = status_val;		
        data_size = 1;
    }

    if(data_tmp_32&0x10000)
        quard_flag = TRUE;
    else
        quard_flag = FALSE;
    wait_flash_busy();
    addr_tmp = SPI_FIFO_CTRL;
    data_tmp_32 = 0x3;		
    MWRITE_32(addr_tmp,data_tmp_32);

    wait_flash_busy();

    start_flash_operation(OPCODE_WRITE_ENABLE, flash_addr, opmode, 0);
    push_fifo_data(data_array,data_size,quard_flag,TRUE);
    start_flash_operation(OPCODE_WRITE_STATUS, flash_addr, opmode, 0);

    data_tmp_32 = get_flash_status(flash_addr);
    while (data_tmp_32&0x1)
        data_tmp_32 = get_flash_status(flash_addr);

    return ;
}

PUBLIC BOOL spi_flash_quard_ctrl(BOOL quard_en_flag, UINT8 dual_mode)
{
    UINT32 data_tmp_32;
    UINT32 addr_tmp;
    UINT8 qmode_cmd;

    UINT8 opmode;
    BOOL quard_flag;
    UINT8 status_val;
    UINT32 flash_addr;

    addr_tmp = SPI_CONFIG;
    data_tmp_32 = MREAD_32(addr_tmp);
    if(data_tmp_32&0x10000)
        quard_flag = TRUE;
    else
        quard_flag = FALSE;

    if(FLASH_TYPE_ID)
        opmode = 3;
    else
        opmode = 0;

    if(quard_en_flag&(FLASH_TYPE_ID ==1))
        status_val = 0x40;
    else if(FLASH_TYPE_ID ==2)
        status_val = 0x0;
    else if(quard_en_flag)
        status_val = 0x40;//0x02;
    else
        status_val = 0x0;

    if(dual_mode == 1)
        flash_addr = 0x400000;
    else if(dual_mode == 3)
        flash_addr = 0x800000;
    else
        flash_addr = 0x0;		

    write_flash_status(0,status_val);

    if(dual_mode)
        write_flash_status(flash_addr,status_val);	

    if(FLASH_TYPE_ID == 0x2)
    {
        if(quard_en_flag)
            qmode_cmd = 0x38;
        else
            qmode_cmd = 0xff;

        start_flash_operation(OPCODE_WRITE_ENABLE, flash_addr, opmode, 0);
        start_flash_operation(qmode_cmd,flash_addr,opmode,0);

        if(dual_mode)
        {
            start_flash_operation(OPCODE_WRITE_ENABLE, flash_addr, opmode, 0);
            start_flash_operation(qmode_cmd,flash_addr,opmode,0);
        }
    }	
         start_flash_operation(OPCODE_HIGH_P_MODE, flash_addr, opmode, 0);

    addr_tmp =SPI_CONFIG;
    data_tmp_32 = MREAD_32(addr_tmp);			
    if(quard_en_flag)
        data_tmp_32 = data_tmp_32|0x1;
    else
        data_tmp_32 = data_tmp_32&0xfffffffe;

    if((FLASH_TYPE_ID == 2)&quard_en_flag)
        data_tmp_32 = data_tmp_32 | 0x10000;
    else
        data_tmp_32 = data_tmp_32&0xfffeffff;
    wait_flash_busy();
    MWRITE_32(addr_tmp,data_tmp_32);

    return TRUE;
}
	
/* **************************************************************************** */
//	quard_setH_resetL:	1 for quard pin operation, 0 for serial operation								//
//	clk_offset_val: stands for SPI clock delay cycles when reading data, 0~7		//
//	spi_clk = 104MHz/clkdiv_val																									//
//	dual_mode : dual spi flash mode selection.																	//
//							1 							-- 4MB + 4MB																		//
//							3 							-- 8MB + 8MB																		//
//							dual_mode&1 =0	-- single SPI flash															//
//							ohter						-- don't set this value													//
/* **************************************************************************** */
PUBLIC BOOL spi_flash_ini(BOOL quard_flag, UINT8 clk_offset_val, UINT8 clkdiv_val, UINT8 dual_mode)
{
    UINT32 data_tmp_32;
    UINT32 addr_tmp;
    wait_flash_busy();
    addr_tmp = SPI_CS_SIZE;
    data_tmp_32 = dual_mode&0x3;
    MWRITE_32(addr_tmp, data_tmp_32);
    wait_flash_busy();
    addr_tmp = SPI_CONFIG;
    data_tmp_32 = ((clk_offset_val<<4)|(clkdiv_val<<8))&0xff70;
    MWRITE_32(addr_tmp,data_tmp_32);

    get_flash_ID(0);   
    spi_flash_quard_ctrl(quard_flag,dual_mode);
    wait_flash_busy();

#ifdef SPI_FLASH_CONTINUOUS_READ
    addr_tmp = SPI_BLOCK_SIZE;
    data_tmp_32 = (0xaf)&0xff;
    MWRITE_32(addr_tmp,data_tmp_32);
#endif

    return TRUE;
}	


#ifdef SPI_FLASH_CONTINUOUS_READ
PUBLIC VOID spi_flash_open_continuous_read(VOID)
{
    MWRITE_32(SPI_BLOCK_SIZE_REG, (0xaf << SPI_FLASH_MODEBIT_BIT));
}

PUBLIC VOID spi_flash_close_continuous_read(VOID)
{
    MWRITE_32(SPI_BLOCK_SIZE_REG, (0x00 << SPI_FLASH_MODEBIT_BIT));
}

#endif

PUBLIC BOOL spi_flash_block_erase(UINT32 flash_addr)
{
    UINT32 data_tmp_32;
    UINT32 addr_tmp;
    UINT8 data_array[3];
    UINT8 opmode;
    BOOL quard_flag;

    addr_tmp = SPI_CONFIG;
    data_tmp_32 = MREAD_32(addr_tmp);
    if(FLASH_TYPE_ID)
        opmode = 3;
    else
        opmode = 0;

    if(data_tmp_32&0x10000)
        quard_flag = TRUE;
    else
        quard_flag = FALSE;
					
    start_flash_operation(OPCODE_WRITE_ENABLE, flash_addr, opmode, 0);
    addr_tmp = flash_addr&0xffffff;
    data_array[2] = (UINT8)(addr_tmp&0xff);
    data_array[1] = (UINT8)((addr_tmp>>8)&0xff);
    data_array[0] = (UINT8)((addr_tmp>>16)&0xff);

    if(opmode)
    {
        push_fifo_data(data_array,3,quard_flag,TRUE);	
        start_flash_operation(OPCODE_BLOCK_ERASE_64K,addr_tmp,opmode,0);	
    }
    else
        start_flash_operation(OPCODE_BLOCK_ERASE_64K,addr_tmp,opmode,0);

    data_tmp_32 = get_flash_status(flash_addr);

    while (1)
    {		
        if(data_tmp_32&0x20&(FLASH_TYPE_ID!=0))
            return FALSE;
        else if (!(data_tmp_32&0x1))
            break;
        else
            data_tmp_32 = get_flash_status(flash_addr);
    }
    return TRUE;
}

PUBLIC BOOL spi_flash_sector_erase2(UINT32 flash_addr)
{
    UINT32 data_tmp_32;
    UINT32 addr_tmp;

    UINT8 opmode;
    BOOL quard_flag;

    UINT32 erase_time=0;
    volatile UINT32 spi_men_read=0;

    spi_men_read = *(UINT32*)(0xa2000000);

    addr_tmp =SPI_CONFIG;
    data_tmp_32 = MREAD_32(addr_tmp);
    if(FLASH_TYPE_ID)
        opmode = 3;
    else
        opmode = 0;
//  status = hal_SysEnterCriticalSection();
    if(data_tmp_32&0x10000)
        quard_flag = TRUE;
    else
        quard_flag = FALSE;

        data_tmp_32 = get_flash_status(flash_addr);
    while (1)
    {		
        if (!(data_tmp_32&0x1))
            break;
        else
            data_tmp_32 = get_flash_status(flash_addr);
    }


    wait_flash_busy();
//    clear_fifo_data(1,1);

    start_flash_operation(OPCODE_ReadReset, flash_addr, 0, 0);

    start_flash_operation(OPCODE_WRITE_ENABLE, flash_addr, opmode, 0);

    addr_tmp = flash_addr&0xffffff;

    start_flash_operation(OPCODE_SECTOR_ERASE,addr_tmp,opmode,0);

//        hal_SysExitCriticalSection(status);
//    hal_DbgPageProtectEnable (HAL_DBG_PAGE_4);

    data_tmp_32 = get_flash_status(addr_tmp);

    while (1)
    {		
        if (!(data_tmp_32&0x1))
            break;
        else
            data_tmp_32 = get_flash_status(addr_tmp);
    }
        
    start_flash_operation(OPCODE_HIGH_P_MODE, flash_addr, opmode, 0);

    erase_time = hal_TimGetUpTime();
//    hal_DbgPageProtectDisable (HAL_DBG_PAGE_4);
//    if(spi_men_read != *(UINT32*)(0xa2000000))
    {
//        start_flash_operation(OPCODE_WRITE_ENABLE, addr_tmp, opmode, 0);
//        start_flash_operation(OPCODE_QUAD_FAST_READ, addr_tmp, opmode, 0);       
    }
    clear_fifo_data(1,1);
    if(spi_men_read != *(UINT32*)(0xa2000000))
    {
        enter_quad_mode(0);
        data_tmp_32 = *(UINT32*)(0xa2000000);
        enter_quad_mode(1);
   }
    if(spi_men_read != *(UINT32*)(0xa2000000))
    {
			asm("break 7");
   }

    return TRUE;
}


UINT32 NOT_SUSPEND;

PUBLIC MEMD_ERR_T spi_flash_sector_erase_time(UINT32 flash_addr,UINT32 time)
{
    volatile UINT32 data_tmp_32;
    UINT32 addr_tmp;
    UINT8 opmode;
    BOOL quard_flag;


    UINT32 suspend_time = 0,max_time = 0;
    max_time = time MILLI_SECONDS;
    UINT32 now = hal_TimGetUpTime();
    RUN_Status=2;
    wait_flash_busy();
    SPI_Flash_Disable_Read();
    wait_flash_read();
    wait_flash_busy();

    start_flash_operation(OPCODE_ReadReset, flash_addr, 0, 0);

    addr_tmp = SPI_CONFIG;
    data_tmp_32 = MREAD_32(addr_tmp);
    if(data_tmp_32&0x10000)
        quard_flag = TRUE;
    else
        quard_flag = FALSE;
    opmode = 0;
    

    NOT_SUSPEND=0; 


    addr_tmp = flash_addr&0xffffff;
    wait_flash_busy();
    wait_flash_read();
    start_flash_operation(OPCODE_WRITE_ENABLE, addr_tmp, opmode, 0);

    wait_flash_read();    
    start_flash_operation(OPCODE_SECTOR_ERASE,addr_tmp,opmode,0);
    RUN_Status3 = MREAD_32(SPI_CMD_ADDR);   
      
    data_tmp_32 = get_flash_status(addr_tmp);
    while (1)
    {		
        suspend_time = hal_TimGetUpTime();
         if (suspend_time - now > max_time) 
        {
//            hal_HstSendEvent(0x81000000|(addr_tmp&0xffffff));            
            start_flash_operation(OPCODE_PROGRAM_ERASE_SUSPEND,addr_tmp,opmode,0);                  
            data_tmp_32 = get_flash_status(addr_tmp);
            while (1)
            {		
                  if (!(data_tmp_32&0x1))
                       break;
                   else
                   data_tmp_32 = get_flash_status(addr_tmp);
            }
            start_flash_operation(OPCODE_HIGH_P_MODE, flash_addr, opmode, 0);
            SPI_Flash_Enable_Read();            
            return MEMD_ERR_SUSPEND;
        }
        if (!(data_tmp_32&0x1))
            break;
        else
            data_tmp_32 = get_flash_status(addr_tmp);
    }

    if(get_flash_status(addr_tmp))
    {
    start_flash_operation(OPCODE_WRITE_ENABLE, addr_tmp, opmode, 0);
    start_flash_operation(OPCODE_SECTOR_ERASE,addr_tmp,opmode,0);
      
    data_tmp_32 = get_flash_status(addr_tmp);
    while (1)
    {		
        suspend_time = hal_TimGetUpTime();
         if (suspend_time - now > max_time) 
        {
//            hal_HstSendEvent(0x81000000|(addr_tmp&0xffffff));            
            start_flash_operation(OPCODE_PROGRAM_ERASE_SUSPEND,addr_tmp,opmode,0);                  
            data_tmp_32 = get_flash_status(addr_tmp);
            while (1)
            {		
                  if (!(data_tmp_32&0x1))
                       break;
                   else
                   data_tmp_32 = get_flash_status(addr_tmp);
            }
            start_flash_operation(OPCODE_HIGH_P_MODE, flash_addr, opmode, 0);
            SPI_Flash_Enable_Read();            
            return MEMD_ERR_SUSPEND;
        }
        if (!(data_tmp_32&0x1))
            break;
        else
            data_tmp_32 = get_flash_status(addr_tmp);
    }
    }

    
    RUN_Status4 = suspend_time - now;
    start_flash_operation(OPCODE_HIGH_P_MODE, flash_addr, opmode, 0);
    SPI_Flash_Enable_Read();

    return MEMD_ERR_NO;
}

PUBLIC BOOL spi_flash_sector_erase(UINT32 flash_addr)
{
    spi_flash_sector_erase_time(flash_addr,500);
    return TRUE;
}

PUBLIC MEMD_ERR_T spi_flash_sector_erase_resume(UINT32 flash_addr,UINT32 time)
{
    UINT32 data_tmp_32;
    UINT32 addr_tmp;
    UINT8 opmode;
    BOOL quard_flag;


    UINT32 suspend_time = 0,max_time = 0;
    max_time = time MILLI_SECONDS;
    UINT32 now = hal_TimGetUpTime();

    addr_tmp = SPI_CONFIG;
    data_tmp_32 = MREAD_32(addr_tmp);
    if(data_tmp_32&0x10000)
        quard_flag = TRUE;
    else
        quard_flag = FALSE;
    opmode = 0;

//    hal_HstSendEvent(0x8C000000|(flash_addr&0xffffff));
    SPI_Flash_Disable_Read();

    wait_flash_read();
    wait_flash_busy();

    start_flash_operation(OPCODE_ReadReset, flash_addr, 0, 0);

    NOT_SUSPEND=0;    


    start_flash_operation(OPCODE_WRITE_ENABLE, flash_addr, opmode, 0);
    addr_tmp = flash_addr&0xffffff;
    start_flash_operation(OPCODE_PER,addr_tmp,opmode,0);

    data_tmp_32 = get_flash_status(flash_addr);
    while (1)
    {		
     suspend_time = hal_TimGetUpTime();
     if (suspend_time - now > max_time) 
        {
            start_flash_operation(OPCODE_PROGRAM_ERASE_SUSPEND,addr_tmp,opmode,0);                  
            data_tmp_32 = get_flash_status(flash_addr);
            while (1)
            {		
                  if (!(data_tmp_32&0x1))
                       break;
                   else
                   data_tmp_32 = get_flash_status(flash_addr);
            }
            start_flash_operation(OPCODE_HIGH_P_MODE, flash_addr, opmode, 0);
            SPI_Flash_Enable_Read();
            return MEMD_ERR_SUSPEND;
        }
        if (!(data_tmp_32&0x1))
            break;
        else
            data_tmp_32 = get_flash_status(flash_addr);
    }
        
    start_flash_operation(OPCODE_HIGH_P_MODE, flash_addr, 0, 0);
    SPI_Flash_Enable_Read();
    return MEMD_ERR_NO;
}

PUBLIC BOOL spi_flash_chip_erase_dual()
{
    UINT32 flash_addr = 0;
    UINT32 data_tmp_32;
    
    while(1);

    if(!spi_flash_chip_erase(flash_addr))
        return FALSE;

    data_tmp_32 = MREAD_32(SPI_CS_SIZE);
    if((data_tmp_32&0x3) == 0x1)
        flash_addr = 0x400000;
    else if((data_tmp_32&0x3) == 0x3)
        flash_addr = 0x800000;
    else
        flash_addr = 0;

    if((data_tmp_32&0x1) == 0x1)	
    {
        if(!spi_flash_chip_erase(flash_addr))
            return FALSE;
    }

    return TRUE;
}

PUBLIC BOOL spi_flash_chip_erase(UINT32 flash_addr)
{
    UINT32 data_tmp_32;
    UINT32 addr_tmp;
    UINT8 opmode;

    addr_tmp = SPI_CONFIG;
    data_tmp_32 = MREAD_32(addr_tmp);
    if(FLASH_TYPE_ID)
        opmode = 3;
    else
        opmode = 0;
    while(1);

    flash_addr = 0;
    start_flash_operation(OPCODE_WRITE_ENABLE, flash_addr, opmode, 0);
    start_flash_operation(OPCODE_CHIP_ERASE, flash_addr,opmode,0);	

    data_tmp_32 = get_flash_status(flash_addr);
    while (1)
    {
        if(data_tmp_32&0x20&(FLASH_TYPE_ID!=0))
            return FALSE;
        else if (!(data_tmp_32&0x1))
            break;
        else
            data_tmp_32 = get_flash_status(flash_addr);
    }

    return TRUE;
}

PUBLIC BOOL spi_flash_write(UINT32 flash_addr, UINT8 data_array[], UINT32 data_size)
{
    volatile UINT32 data_tmp_32;
    UINT32 addr_tmp;

    UINT8 opmode;
    BOOL quard_flag;
    BOOL addr_quard_flag;
    UINT8 write_cmd;
    BOOL fifo_clr_flag = TRUE;

    RUN_Status=1;

   if(!data_size)
        return TRUE;

    wait_flash_busy();
    SPI_Flash_Disable_Read();
    wait_flash_read();
    wait_flash_busy();

    start_flash_operation(OPCODE_ReadReset, flash_addr, 0, 0);


    addr_tmp = SPI_CONFIG;
    data_tmp_32 = MREAD_32(addr_tmp);
    if(FLASH_TYPE_ID)
        opmode = 3;
    else
        opmode = 0;

    if((data_tmp_32&0x1))
        quard_flag = TRUE;
    else
        quard_flag = FALSE;

    if((FLASH_TYPE_ID>0)&(FLASH_TYPE_ID<TOTAL_FLASH_ID))
        addr_quard_flag = TRUE;
    else
        addr_quard_flag = FALSE;

    if(FLASH_TYPE_ID==1)
        write_cmd = 0x38;
    else if(FLASH_TYPE_ID==2)
        write_cmd = 0x02;
    else
        write_cmd = OPCODE_QUAD_PAGE_PROGRAM;

     if( quard_flag == FALSE)
        write_cmd = 0x02;       


    addr_tmp = flash_addr&0xffffff;

    push_fifo_data(data_array,data_size,quard_flag,fifo_clr_flag);
    wait_flash_busy();
    wait_flash_read();
    
    data_tmp_32 = MREAD_32(SPI_STATUS);
    while((data_tmp_32&0x2)==0)
    {
        addr_tmp = flash_addr&0xffffff;        
        start_flash_operation(OPCODE_WRITE_ENABLE, addr_tmp, opmode, 0);        
        start_flash_operation(write_cmd, addr_tmp,opmode,0);
        data_tmp_32 = get_flash_status(addr_tmp);
         while (1)
        {
            if (!(data_tmp_32&0x1))
                break;
            else
                data_tmp_32 = get_flash_status(addr_tmp);
        }
        data_tmp_32 = MREAD_32(SPI_STATUS);
    }

    start_flash_operation(OPCODE_HIGH_P_MODE, flash_addr, opmode, 0);
    SPI_Flash_Enable_Read();

    return TRUE;
}

#if 0
// just used for FPGA ,read the function memd_FlashRead()
PUBLIC void flash_read(UINT32 flash_addr, UINT8 data_array[], UINT32 data_size)
{
    UINT32 i;
    UINT8 addr_array[6];
    UINT32 addr_tmp;
    UINT32 data_tmp_32;
    UINT8 opmode;
    BOOL quard_flag;
    BOOL addr_quard_flag;

    addr_tmp = SPI_CONFIG;
    data_tmp_32 = MREAD_32(addr_tmp);
    if(FLASH_TYPE_ID)
        opmode = 2;
    else
        opmode = 0;

    if((data_tmp_32&0x1))
        quard_flag = TRUE;
    else
        quard_flag = FALSE;

    if((FLASH_TYPE_ID>0)&(FLASH_TYPE_ID<TOTAL_FLASH_ID)&(data_tmp_32&0x10000))
        addr_quard_flag = TRUE;
    else
        addr_quard_flag = FALSE;
    wait_flash_busy();
    addr_tmp = SPI_FIFO_CTRL;
    data_tmp_32 = 0x1;		
    MWRITE_32(addr_tmp,data_tmp_32);

    addr_array[2] = (UINT8)(flash_addr&0xff);
    addr_array[1] = (UINT8)((flash_addr>>8)&0xff);
    addr_array[0] = (UINT8)((flash_addr>>16)&0xff);	
    if(opmode)
    {
        addr_array[3] = 0x0;
        addr_array[4] = 0x0;
        addr_array[5] = 0x0;
        push_fifo_data(addr_array,6,addr_quard_flag,TRUE);	
        start_flash_operation(OPCODE_QUAD_FAST_READ, flash_addr,opmode,data_size);	
    }
    else
    {
        push_fifo_data(addr_array,3,addr_quard_flag,TRUE);	
        start_flash_operation(OPCODE_READ_DATA, flash_addr,opmode,data_size);	
    }

    for(i=0;i<data_size;i++)
        data_array[i] = get_flash_data();		
    return;
}	

#endif

// =============================================================================

// =============================================================================
PUBLIC MEMD_FLASH_LAYOUT_T* memd_FlashOpen(CONST MEMD_FLASH_CONFIG_T* cfg)
{        
    UINT8 clk_div;
//    UINT32 data_tmp_32;
//    UINT32 addr_tmp;


    MEMD_ASSERT(cfg != NULL, "Opening flash with a NULL configuration");	

    if(FLASH_TYPE_ID == 0x1) clk_div = 0x8;
    else clk_div = 0x2;
    
    
#ifdef SPI_FLSH_52M
    (*(UINT32*)(0xa1a000ac)) = 0x4;//624/(n+2)/2=52
    (*(UINT32*)(0xa1a000ac)) = 0x14;
#else
#ifdef SPI_FLSH_78M
    (*(UINT32*)(0xa1a000ac)) = 0x2;//624/(2+2)/2=78
    (*(UINT32*)(0xa1a000ac)) = 0x12;
#else
#ifdef SPI_FLSH_104M
    (*(UINT32*)(0xa1a000ac)) = 0x1;//624/(1+2)/2=104
    (*(UINT32*)(0xa1a000ac)) = 0x11;
#else
    #error spi clk is out of 104MHz. 
#endif //SPI_FLSH_104M
#endif //SPI_FLSH_78M
#endif //SPI_FLSH_52M

#ifdef dual_spi_flash
    spi_flash_ini(TRUE,0x2,clk_div,0x1);
#else
    spi_flash_ini(TRUE,0x2,clk_div,0x0);
#endif    
    g_memdFlashBaseAddress = (UINT32) hal_EbcFlashOpen(HAL_SYS_FREQ_39M, &cfg->csConfig, NULL);
if(!g_memdFlashBaseAddress)
    g_memdFlashBaseAddress=0xa2000000;    
/*
> w 0xa1a25000,6
> w 0xa1a25008,0x12
> w 0xa1a25008,0x12
> w 0xa1a25008,0x34
> w 0xa1a25008,0x56
> w 0xa1a25008,0x78
> w 0xa1a25000,0x10002
*/    

    return ((MEMD_FLASH_LAYOUT_T*) &g_memdFlashLayout);
}

PUBLIC VOID memd_FlashClose(VOID)
{
#ifdef SPI_FLASH_CONTINUOUS_READ
    spi_flash_close_continuous_read();
#endif
	return;
}

PUBLIC void SPI_Flash_Open()
{
    UINT8 clk_div;
    if(FLASH_TYPE_ID == 0x1) clk_div = 0x8;
    else clk_div = 0x2;
    clk_div = 0x8;
    
    spi_flash_ini(FALSE,0x2,clk_div,0x0);
    return;
}

#ifdef MEMD_RAM_IS_BURST
#ifdef RAM_CLK_IS_52M
#define PSRAM_BCR 0x08590f
#else
#ifdef RAM_CLK_IS_78M
#define PSRAM_BCR 0x08710f 
#else
#ifdef RAM_CLK_IS_104M
#define PSRAM_BCR 0x08710f 
#else
#error sram clk is out of 133MHz. 
#endif //RAM_CLK_IS_104M
#endif //RAM_CLK_IS_78M
#endif//RAM_CLK_IS_52M

PRIVATE VOID HAL_FUNC_INTERNAL 
memd_RamConfigureBurstMode(REG16* csBase, REG32* pCRE)
{
    UINT16 read;

    // Using the CR controlled method 
    // Configure BCR
    // PSRAM_BCR[15] : 0 = synch / 1 = asynch
    // PSRAM_BCR[14] : 0 = variable Latency / 1 = Fixed latency
    // PSRAM_BCR[13:11] : Latency count 011(3)-111(7) clock cycle could be used 
    // PSRAM_BCR[10] : Wait Polarity 0 = Active Low / 1 = Active High
    // PSRAM_BCR[9] : MUST be set to 0
    // PSRAM_BCR[8] : Wait Config 0 = Asserted during wait / 1 = Asserted 1 cycle before wait
    // PSRAM_BCR[7:6] : MUST be set to 0   
    // PSRAM_BCR[5:4] : driver strength bits 0 = Full Drive / 1 = 1/2 Drive / 2 = 1/4 Drive
    // PSRAM_BCR[3] : 0 = Wrap / 1 = No wrap
    // PSRAM_BCR[2:0] : Burst len 001 = 4 words / 010 = 8 words / 011 = 16 words / 100 = 32 words / 111 Continuous
    read=*(csBase+PSRAM_BCR); 
    
    
    *pCRE=1; // MEM_FIFO_CRE
    *(csBase+PSRAM_BCR)=read; // read burst with latency = 3 fixed (4 clocks)
    
    {
        UINT32 flush __attribute__((unused)) = *pCRE;
    }
    
    *pCRE=0;

}
#endif //!MEMD_RAM_IS_BURST


// =============================================================================
// memd_RamOpen
// -----------------------------------------------------------------------------
/// That function configures the RAM.
/// It must be called to configure the best settings for the RAM, thus enabling
/// the fatest access.
/// @param cfg Ram configuration used to open the RAM.
///
// =============================================================================
PUBLIC VOID memd_RamOpen(CONST MEMD_RAM_CONFIG_T* cfg)
{
    MEMD_ASSERT(cfg != NULL, "Opening RAM with a NULL configuration");
    // Open External SRAM
#ifdef MEMD_RAM_IS_BURST
    hal_EbcSramOpen(&cfg->csConfig, memd_RamConfigureBurstMode);
#else //!MEMD_RAM_IS_BURST
    hal_EbcCsOpen(HAL_EBC_SRAM, &cfg->csConfig);
#endif //!MEMD_RAM_IS_BURST
}

// =============================================================================

// =============================================================================
PUBLIC MEMD_ERR_T memd_FlashErase2(
    UINT8 *startFlashAddress,
    UINT8 *endFlashAddress)
{
    BOOL ret_flag;

    while(endFlashAddress >startFlashAddress)
    {
        ret_flag = spi_flash_sector_erase((UINT32)startFlashAddress);
        if(!ret_flag) return MEMD_ERR_ERASE;
        startFlashAddress+=0x1000;
    }
//    if(ret_flag) return MEMD_ERR_NO;
    return MEMD_ERR_NO;
}

PRIVATE UINT32 memd_FlashGetSectorLimits(UINT32 sectorId, UINT32 *start, UINT32 *end);

// =============================================================================
// memd_FlashErase
// -----------------------------------------------------------------------------
/// This function erases contiguous flash sectors. 
/// Addresses <B> must be aligned on sectors</B>:
/// - The \c startFlashAddress is the address of the first sector to erase.
/// - The \c endFlashAddress is the address of the first sector NOT to erase.
///   If \c endFlashAddress is \c NULL, only one sector will be erased.
/// .
/// Care must be taken not to erase the code present at the beginning of the flash.
///
/// @param start_flashAddress The address of the first sector to erase
/// @param end_flashAddress The address of the first sector NOT to erase.
///   If \c NULL, only one sector will be erased 
/// @return #MEMD_ERR_NO, #MEMD_ERR_ERASE, #MEMD_ERR_ALIGN or #MEMD_ERR_PROTECT
/// whether the operation succeeded or failed
///
// =============================================================================
PUBLIC  MEMD_ERR_T memd_FlashErase_First(UINT8 *startFlashAddress, UINT8 *endFlashAddress,UINT32 time)
{
    UINT32 status;
    VOLATILE UINT8 * ptr;
//    VOLATILE UINT16 *BankAddr;
    UINT32 Start, End;
    UINT32 phys_end_add, phys_start_add;
    
    #ifdef FLASH_DEBUG
    hal_HstSendEvent(0x98765432);
    #endif
    phys_start_add = (UINT32)startFlashAddress;
    MEMD_ASSERT((phys_start_add & 0xe0000000) == 0, 
        "flash_address is expected to be a byte offset within the flash chip, not an absolute adresse");

    // Check that start & end addresses are aligned
    if (endFlashAddress == NULL)
    {
        memd_FlashGetSectorLimits(phys_start_add, &Start, &End);
        phys_end_add = End;
    } else
    {
        phys_end_add = (UINT32)endFlashAddress;
        MEMD_ASSERT((phys_end_add & 0xe0000000) == 0, 
            "flash_address is expected to be a byte offset within the flash chip, not an absolute adresse")
        if (phys_end_add != FLASH_SIZE)
        {
            memd_FlashGetSectorLimits(phys_end_add , &Start, &End);
            if (phys_end_add != Start)
            {
                MEMD_ASSERT(0, "flash erase error 1,phys_end_add = 0x%x,start = 0x%x.",phys_end_add,Start);
                return MEMD_ERR_ALIGN;
            }
        }
    }

    memd_FlashGetSectorLimits(phys_start_add, &Start, &End);
    if (phys_start_add != Start)
    {
        MEMD_ASSERT(0, "flash erase error 2,phys_end_add = 0x%x,start = 0x%x.",phys_end_add,Start);
        return MEMD_ERR_ALIGN;
    }


    while (Start != phys_end_add)
    {
        // Start should already be aligned to sector boundary, so shouldn't need any more masking
        ptr = (VOLATILE UINT8 *)(g_memdFlashBaseAddress + Start);

        // Sector Erase command
        status =spi_flash_sector_erase_time((UINT32)ptr,time);
        if(status!=MEMD_ERR_NO)
        {
            return status;
        }

        // note the pooling could be done on data == 0xff also
    
        if (End != FLASH_SIZE)
        {
            memd_FlashGetSectorLimits(End, &Start, &End);
        } else
        {
            Start = End;
        }
    }
    

    // erase done
    return MEMD_ERR_NO;
}

// =============================================================================
// memd_FlashErase
// -----------------------------------------------------------------------------
/// This function erases contiguous flash sectors. 
/// Addresses <B> must be aligned on sectors</B>:
/// - The \c startFlashAddress is the address of the first sector to erase.
/// - The \c endFlashAddress is the address of the first sector NOT to erase.
///   If \c endFlashAddress is \c NULL, only one sector will be erased.
/// .
/// Care must be taken not to erase the code present at the beginning of the flash.
///
/// @param start_flashAddress The address of the first sector to erase
/// @param end_flashAddress The address of the first sector NOT to erase.
///   If \c NULL, only one sector will be erased 
/// @return #MEMD_ERR_NO, #MEMD_ERR_ERASE, #MEMD_ERR_ALIGN or #MEMD_ERR_PROTECT
/// whether the operation succeeded or failed
///
// =============================================================================
PUBLIC  MEMD_ERR_T memd_FlashErase_Continue(UINT8 *startFlashAddress, UINT8 *endFlashAddress,UINT32 time)
{
    UINT32 status;
    VOLATILE UINT8 * ptr;

    UINT32 Start, End;
    UINT32 phys_end_add, phys_start_add;
    
    #ifdef FLASH_DEBUG
    hal_HstSendEvent(0x98765432);
    #endif
    phys_start_add = (UINT32)startFlashAddress;
    MEMD_ASSERT((phys_start_add & 0xe0000000) == 0, 
        "flash_address is expected to be a byte offset within the flash chip, not an absolute adresse");

    // Check that start & end addresses are aligned
    if (endFlashAddress == NULL)
    {
        memd_FlashGetSectorLimits(phys_start_add, &Start, &End);
        phys_end_add = End;
    } else
    {
        phys_end_add = (UINT32)endFlashAddress;
        MEMD_ASSERT((phys_end_add & 0xe0000000) == 0, 
            "flash_address is expected to be a byte offset within the flash chip, not an absolute adresse")
        if (phys_end_add != FLASH_SIZE)
        {
            memd_FlashGetSectorLimits(phys_end_add , &Start, &End);
            if (phys_end_add != Start)
            {
                MEMD_ASSERT(0, "flash erase error 1,phys_end_add = 0x%x,start = 0x%x.",phys_end_add,Start);
                return MEMD_ERR_ALIGN;
            }
        }
    }

    memd_FlashGetSectorLimits(phys_start_add, &Start, &End);
    if (phys_start_add != Start)
    {
        MEMD_ASSERT(0, "flash erase error 2,phys_end_add = 0x%x,start = 0x%x.",phys_end_add,Start);
        return MEMD_ERR_ALIGN;
    }


    while (Start != phys_end_add)
    {
        // Start should already be aligned to sector boundary, so shouldn't need any more masking
        ptr = (VOLATILE UINT8 *)(g_memdFlashBaseAddress + Start);

        // Sector Erase command
        if(NOT_SUSPEND)
        {
            status =spi_flash_sector_erase_time((UINT32)ptr,time);
        }
        else
        {
            status =spi_flash_sector_erase_resume((UINT32)ptr,time);
        }
        if(status != MEMD_ERR_ERASE)
        {
            return status;
        }

        // note the pooling could be done on data == 0xff also
    
        if (End != FLASH_SIZE)
        {
            memd_FlashGetSectorLimits(End, &Start, &End);
        } else
        {
            Start = End;
        }
    }
    
//    hal_HstSendEvent(0x89000000|((UINT32)endFlashAddress&0xffffff));

    if(status) return MEMD_ERR_ERASE;

    // erase done
    return MEMD_ERR_NO;
}

// =============================================================================
// memd_FlashErase
// -----------------------------------------------------------------------------
/// This function erases contiguous flash sectors. 
/// Addresses <B> must be aligned on sectors</B>:
/// - The \c startFlashAddress is the address of the first sector to erase.
/// - The \c endFlashAddress is the address of the first sector NOT to erase.
///   If \c endFlashAddress is \c NULL, only one sector will be erased.
/// .
/// Care must be taken not to erase the code present at the beginning of the flash.
///
/// @param start_flashAddress The address of the first sector to erase
/// @param end_flashAddress The address of the first sector NOT to erase.
///   If \c NULL, only one sector will be erased 
/// @return #MEMD_ERR_NO, #MEMD_ERR_ERASE, #MEMD_ERR_ALIGN or #MEMD_ERR_PROTECT
/// whether the operation succeeded or failed
///
// =============================================================================
PUBLIC MEMD_ERR_T memd_FlashErase(UINT8 *startFlashAddress, UINT8 *endFlashAddress)
{
    VOLATILE UINT8 * ptr;
//    VOLATILE UINT16 *BankAddr;
    UINT32 Start, End;
    UINT32 phys_end_add, phys_start_add;
    BOOL ret_flag=TRUE;
    #ifdef FLASH_DEBUG
    hal_HstSendEvent(0x98765432);
    #endif
    phys_start_add = (UINT32)startFlashAddress;
    MEMD_ASSERT((phys_start_add & 0xe0000000) == 0, 
        "flash_address is expected to be a byte offset within the flash chip, not an absolute adresse");

    // Check that start & end addresses are aligned
    if (endFlashAddress == NULL)
    {
        memd_FlashGetSectorLimits(phys_start_add, &Start, &End);
        phys_end_add = End;
    } else
    {
        phys_end_add = (UINT32)endFlashAddress;
        MEMD_ASSERT((phys_end_add & 0xe0000000) == 0, 
            "flash_address is expected to be a byte offset within the flash chip, not an absolute adresse")
        if (phys_end_add != FLASH_SIZE)
        {
            memd_FlashGetSectorLimits(phys_end_add , &Start, &End);
            if (phys_end_add != Start)
            {
                MEMD_ASSERT(0, "flash erase error 1,phys_end_add = 0x%x,start = 0x%x.",phys_end_add,Start);
                return MEMD_ERR_ALIGN;
            }
        }
    }

    memd_FlashGetSectorLimits(phys_start_add, &Start, &End);
    if (phys_start_add != Start)
    {
        MEMD_ASSERT(0, "flash erase error 2,phys_end_add = 0x%x,start = 0x%x.",phys_end_add,Start);
        return MEMD_ERR_ALIGN;
    }


    while (Start != phys_end_add)
    {
        {
//            BankAddr = (VOLATILE UINT16 *)(g_memdFlashBaseAddress + (Start&FLASHBANK_MASK));
            // Start should already be aligned to sector boundary, so shouldn't need any more masking
            ptr = (VOLATILE UINT8 *)(g_memdFlashBaseAddress + Start);
        }

//        status = hal_SysEnterCriticalSection();
        // Sector Erase command
        ret_flag |=spi_flash_sector_erase((UINT32)ptr);
//        hal_SysExitCriticalSection(status);

        // note the pooling could be done on data == 0xff also
    
        if (End != FLASH_SIZE)
        {
            memd_FlashGetSectorLimits(End, &Start, &End);
        } else
        {
            Start = End;
        }
    }
    

    if(!ret_flag) return MEMD_ERR_ERASE;

    // erase done
    return MEMD_ERR_NO;
}

#ifdef FLASH_DEBUG   
#define MCP_DLEN 512
UINT8 buffer_psram[MCP_DLEN];
UINT8 buffer_flash[MCP_DLEN];
#endif
// =============================================================================
// memd_FlashWrite
// =============================================================================
PUBLIC MEMD_ERR_T memd_FlashWrite(UINT8 *flashAddress,
        UINT32 byteSize, 
        UINT32 * pWrittenByteSize, 
        CONST UINT8* buffer)
{
    BOOL ret_flag=TRUE;
    UINT32 i;
    UINT32 times;
    UINT16 remain_bytes;
    UINT32 one_page_bytes;
    UINT32 addr_tmp;
    UINT8* buf_ptr;
#ifdef FLASH_DEBUG   
    UINT8* buf_pds;
    UINT32 byteSize_bak=byteSize;
	
    hal_HstSendEvent(0x68000000 | ((int)flashAddress&0xffffff));
    flashAddress = g_memdFlashBaseAddress + flashAddress;
    memcpy(buffer_psram, (UINT8 *)buffer, ((byteSize<MCP_DLEN)?byteSize:MCP_DLEN));
    memcpy(buffer_flash, (UINT8 *)flashAddress, ((byteSize<MCP_DLEN)?byteSize:MCP_DLEN));    
#else
    flashAddress = g_memdFlashBaseAddress + flashAddress;
#endif


    one_page_bytes = PAGE_SIZE;
//    one_page_bytes = PAGE_SIZE2;



    addr_tmp = (UINT32)flashAddress;
    buf_ptr = (UINT8*)buffer;
    *pWrittenByteSize = 0;
//    status = hal_SysEnterCriticalSection();



if( ( ((UINT32)flashAddress&~0xff)!=0)&&(((UINT32)flashAddress+byteSize)>=(((UINT32)flashAddress&~0xff)+PAGE_SIZE)))
{
        remain_bytes = (((UINT32)flashAddress&~0xff)+PAGE_SIZE) - ((UINT32)flashAddress);
        byteSize-= remain_bytes;
        if(remain_bytes<=one_page_bytes)
        {
            ret_flag =  spi_flash_write(addr_tmp, buf_ptr, remain_bytes);
        }
        else
        {
            ret_flag =  spi_flash_write(addr_tmp, buf_ptr, one_page_bytes);
            addr_tmp = addr_tmp + one_page_bytes;
            buf_ptr = buf_ptr + one_page_bytes;       
            *pWrittenByteSize += one_page_bytes;
            
            remain_bytes=remain_bytes-one_page_bytes;
            ret_flag =  spi_flash_write(addr_tmp, buf_ptr, remain_bytes);
        }
        addr_tmp = addr_tmp + remain_bytes;
        buf_ptr = buf_ptr + remain_bytes;
        *pWrittenByteSize += remain_bytes;

}
    if(!ret_flag)
    {
//        hal_SysExitCriticalSection(status);
        return MEMD_ERR_WRITE;
    }
    if(!byteSize)
    {
//        hal_SysExitCriticalSection(status);
        return MEMD_ERR_NO;
    }

    times = byteSize/one_page_bytes;
    remain_bytes = byteSize%one_page_bytes;


    for(i=0;(i<times)&&ret_flag;i++)
    {
        ret_flag =  spi_flash_write(addr_tmp, buf_ptr, one_page_bytes);
        addr_tmp = addr_tmp + one_page_bytes;
        buf_ptr = buf_ptr + one_page_bytes;
        *pWrittenByteSize += one_page_bytes;
        
    }
    if(!ret_flag)
    {
//        hal_SysExitCriticalSection(status);
        return MEMD_ERR_WRITE;
    }

    if(remain_bytes!=0)
    {
        ret_flag =  spi_flash_write(addr_tmp, buf_ptr, remain_bytes);
        *pWrittenByteSize += remain_bytes;
    }
    if(!ret_flag)
    {
        return MEMD_ERR_WRITE;
    }
    else
    {
#ifdef FLASH_DEBUG
        {
            buf_pds = (UINT8*)flashAddress;
            buf_ptr = (UINT8*)buffer;
            
            for(i=0;(i<byteSize_bak);i++)
            {
                if(buf_pds[i] != buf_ptr[i])
                {
                    hal_HstSendEvent( 0x12345678);
                    hal_HstSendEvent( buf_pds[i]);
                    hal_HstSendEvent( buf_ptr[i]|(byteSize_bak<<16));
                    hal_HstSendEvent( (UINT32)(buf_pds)+i);
                    hal_HstSendEvent( (UINT32)buf_ptr+i);
                    ret_flag =0;                
                }
                if(i<MCP_DLEN)
                {
                    if(buffer_psram[i] != buf_ptr[i])
                    {
                    hal_HstSendEvent( 0x87654321);
                    hal_HstSendEvent( buffer_psram[i]);
                    hal_HstSendEvent( buf_ptr[i]);
                    hal_HstSendEvent( (UINT32)(buffer_psram)+i);
                    hal_HstSendEvent( (UINT32)buf_ptr+i);
                    ret_flag =0;                
                    }              
                    if(!ret_flag)
                    {
                        if(buf_pds[i] != buffer_flash[i])
                        {
                        hal_HstSendEvent( 0x12345678);
                        hal_HstSendEvent( buf_pds[i]);
                        hal_HstSendEvent( buffer_flash[i]);
                        hal_HstSendEvent( (UINT32)(buf_pds)+i);
                        hal_HstSendEvent( (UINT32)buffer_flash+i);
                        }
                    }
                }
            }
            if(!ret_flag)
            {
                asm("break 7");
            }
        }
#endif
        return MEMD_ERR_NO;
    }
}



// =============================================================================
// memd_FlashRead
// =============================================================================
PUBLIC MEMD_ERR_T memd_FlashRead(UINT8 *flashAddress,UINT32  byteSize,UINT32* pReadByteSize,UINT8*  buffer)
{
    VOLATILE UINT8 * ptr;
    UINT32 phys_add;

    phys_add = (UINT32)flashAddress;

    MEMD_ASSERT((phys_add & 0xe0000000) == 0,
        "flash_address is expected to be a byte offset within the flash chip, not an absolute adresse")

    if (GET_PAGE_PHYS(phys_add) <= 2)
    {
        ptr = (VOLATILE UINT8 *)(g_memdFlashBaseAddress + phys_add);
    }
    else
    {
        ptr = (VOLATILE UINT8 *)((g_memdFlashBaseAddress + phys_add) - ((GET_PAGE_PHYS(phys_add)-3)*0x400000));
        // Configure Page Register
        hal_EbcConfigRemap((HAL_EBC_FLSH_PHYSADD_T)phys_add);
    }


    /* could do aligned read from flash to improve bus accesses as it is uncached */
    memcpy(buffer, (UINT8 *)ptr, byteSize);
    *pReadByteSize=byteSize;
    return MEMD_ERR_NO;
}

PUBLIC VOLATILE CONST UINT8* memd_FlashGetGlobalAddress(UINT8 *flashAddress)
{
    VOLATILE CONST UINT8* ptr;
    UINT32 phys_add;

    phys_add = (UINT32)flashAddress;
    MEMD_ASSERT((phys_add & 0xe0000000) == 0,
        "flash_address is expected to be a byte offset within the flash chip, not an absolute adresse");

    if (GET_PAGE_PHYS(phys_add) <= 2)
    {
        ptr = (VOLATILE CONST UINT8 *)(g_memdFlashBaseAddress + phys_add);
    }
    else  // Page > 2, so remap
    {
        ptr = (VOLATILE CONST UINT8 *)((g_memdFlashBaseAddress + phys_add) - ((GET_PAGE_PHYS(phys_add)-3)*0x400000));
        // Configure Page Register
        hal_EbcConfigRemap((HAL_EBC_FLSH_PHYSADD_T)phys_add);
    }

    return ptr;
}

PRIVATE UINT32 memd_FlashGetSectorLimits(UINT32 sectorId, UINT32 *start, UINT32 *end)
{
    UINT32 aStart, aEnd;
    int bankNum = 0;
    int majSecIdx, minSecIdx;
    UINT32 sectSize;

    if (sectorId == g_memdFlashLayout.fullSize)
    {
        *start = g_memdFlashLayout.fullSize;
        *end = g_memdFlashLayout.fullSize;
        return 0;
    }
    else if (sectorId > g_memdFlashLayout.fullSize)
    {
        *start = 0;
        *end = 0;
        MEMD_ASSERT(FALSE, "Out of accessible flash space !!!!");
        return 0;
    }
    else
    {
        while ( (sectorId >= g_memdFlashLayout.bankLayout[bankNum+1].bankStartAddr) &&
                (bankNum < (g_memdFlashLayout.numberOfBank-1)) )
        {
            ++bankNum;
        }
        
        aStart = g_memdFlashLayout.bankLayout[bankNum].bankStartAddr;
        majSecIdx = 0;
        aEnd = aStart +
                (g_memdFlashLayout.bankLayout[bankNum].sectorLayoutTable[majSecIdx][0] *
                g_memdFlashLayout.bankLayout[bankNum].sectorLayoutTable[majSecIdx][1]);

        while (sectorId >= aEnd)
        {
            if (majSecIdx == 2)
            {
                *start = 0;
                *end = 0;
                MEMD_ASSERT(FALSE, "Out of accessible flash space !!!!");
                return 0;
            } 
            else
            {
                ++majSecIdx;
                aStart = aEnd;
                aEnd = aStart +
                        (g_memdFlashLayout.bankLayout[bankNum].sectorLayoutTable[majSecIdx][0] *
                        g_memdFlashLayout.bankLayout[bankNum].sectorLayoutTable[majSecIdx][1]);
            }
        }

        minSecIdx = 0;
        sectSize = g_memdFlashLayout.bankLayout[bankNum].sectorLayoutTable[majSecIdx][1];
        aEnd = aStart + sectSize;

        while (sectorId >= aEnd)
        {
            aStart = aEnd;
            aEnd = aStart + sectSize;
            ++minSecIdx;
        }

        *start = aStart;
        *end = aEnd;
        return sectSize;
    }
}

PRIVATE MEMD_ERR_T memd_SetLock(UINT8 *startFlashAddress, UINT8 *endFlashAddress, BOOL Enable)
{  
    return MEMD_ERR_NO;
}

PRIVATE UINT16 memd_LockStatus(UINT8 * SectorAddr, UINT16 *Lock)
{
    *Lock = 0;
    return MEMD_ERR_NO;
}


// =============================================================================
// =============================================================================
PUBLIC MEMD_ERR_T memd_FlashLock(
    UINT8 *startFlashAddress,
    UINT8 *endFlashAddress)
{
    MEMD_ASSERT((((UINT32)startFlashAddress) & 0xe0000000) == 0,
        "startFlashAddress is expected to be a byte offset within the flash chip, not an absolute adresse");
    MEMD_ASSERT((((UINT32)endFlashAddress) & 0xe0000000) == 0,
        "endFlashAddress is expected to be a byte offset within the flash chip, not an absolute adresse");

    return memd_SetLock(startFlashAddress, endFlashAddress, TRUE);
}


// =============================================================================

// ============================================================================
PUBLIC MEMD_ERR_T memd_FlashUnlock(
    UINT8 *startFlashAddress,
    UINT8 *endFlashAddress)
{
    MEMD_ASSERT((((UINT32)startFlashAddress) & 0xe0000000) == 0,
        "startFlashAddress is expected to be a byte offset within the flash chip, not an absolute adresse");
    MEMD_ASSERT((((UINT32)endFlashAddress) & 0xe0000000) == 0,
        "endFlashAddress is expected to be a byte offset within the flash chip, not an absolute adresse");

    return memd_SetLock(startFlashAddress, endFlashAddress, FALSE);
}

// =============================================================================

// =============================================================================
PUBLIC MEMD_ERR_T memd_FlashGetLockStatus(UINT8* startFlashAddress, BOOL* lockStatus)
{
    UINT16 lock;

    MEMD_ASSERT((((UINT32)startFlashAddress) & 0xe0000000) == 0,
            "startFlashAddress is expected to be a byte offset within the flash chip, not an absolute adresse");

    MEMD_ERR_T status = memd_LockStatus(startFlashAddress, &lock);

    if (lock == 1)
    {
        *lockStatus = TRUE;
    }
    else
    {
        *lockStatus = FALSE;
    }

    return status;
}


// =============================================================================
// memd_FlashGetUserFieldInfo
// -----------------------------------------------------------------------------
/// That function get the user data info from memd.def.
///
/// @param userFieldBase The USER_DATA_BASE defined in memd.def.
/// @param userFieldSize The USER_DATA_SIZE defined in memd.def.
/// @param sectorSize The sector size of user data field, it is the large sector size.
///        Some small boot sectors can be regarded as one large sector.
/// @return VOID.
// =============================================================================
PUBLIC VOID memd_FlashGetUserFieldInfo(UINT32 *userFieldBase, UINT32 *userFieldSize, UINT32 *sectorSize, UINT32 *blockSize)
{
    UINT32 Start, End;
    
    *userFieldBase = g_memdFlashLayout.userDataBase;
    *userFieldSize = g_memdFlashLayout.userDataSize;
    *sectorSize = memd_FlashGetSectorLimits(*userFieldBase, &Start, &End);
    *blockSize = g_memdFlashLayout.userDataSectorLogicalBlockSize;
}

UINT32 FLASH_IN_DP=0;

// =============================================================================
// memd_FlashGetUserFieldInfo
// -----------------------------------------------------------------------------
/// That function get the user data info from memd.def.
///
/// @param userFieldBase The USER_DATA_BASE defined in memd.def.
/// @param userFieldSize The USER_DATA_SIZE defined in memd.def.
/// @param sectorSize The sector size of user data field, it is the large sector size.
///        Some small boot sectors can be regarded as one large sector.
/// @return VOID.
// =============================================================================
PUBLIC VOID memd_FlashSleep(VOID)
{
    wait_flash_read();
    wait_flash_busy();
#ifdef dual_spi_flash
    start_flash_operation(OPCODE_ReadReset, 0, 0, 0);
    start_flash_operation(OPCODE_RDI, 0, 0, 0);
#ifdef SPI_FLASH_DEEP_POWER_DOWN   
    start_flash_operation(OPCODE_DP, 0, 0, 0);
#endif

    start_flash_operation(OPCODE_ReadReset, 0x400000, 0, 0);
    start_flash_operation(OPCODE_RDI, 0x400000, 0, 0);
#ifdef SPI_FLASH_DEEP_POWER_DOWN   
    start_flash_operation(OPCODE_DP, 0x400000, 0, 0);
#endif
#else
    start_flash_operation(OPCODE_ReadReset, 0, 0, 0);
    start_flash_operation(OPCODE_RDI, 0, 0, 0);
#ifdef SPI_FLASH_DEEP_POWER_DOWN   
    start_flash_operation(OPCODE_DP, 0, 0, 0);
#endif
#endif

    FLASH_IN_DP=1;
    wait_flash_busy();    
    {
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
    }  
}


// =============================================================================
// memd_FlashGetUserFieldInfo
// -----------------------------------------------------------------------------
/// That function get the user data info from memd.def.
///
/// @param userFieldBase The USER_DATA_BASE defined in memd.def.
/// @param userFieldSize The USER_DATA_SIZE defined in memd.def.
/// @param sectorSize The sector size of user data field, it is the large sector size.
///        Some small boot sectors can be regarded as one large sector.
/// @return VOID.
// =============================================================================
PUBLIC VOID memd_FlashWakeup(VOID)
{
    wait_flash_read();
    wait_flash_busy();
#ifdef dual_spi_flash
    start_flash_operation(OPCODE_RDI, 0, 0, 0);
    start_flash_operation(OPCODE_ReadReset, 0, 0, 0);
    start_flash_operation(OPCODE_HIGH_P_MODE, 0, 0, 0);
    start_flash_operation(OPCODE_RDI, 0x400000, 0, 0);
    start_flash_operation(OPCODE_ReadReset, 0x400000, 0, 0);
    start_flash_operation(OPCODE_HIGH_P_MODE, 0x400000, 0, 0);

#else

    start_flash_operation(OPCODE_RDI, 0, 0, 0);
    start_flash_operation(OPCODE_ReadReset, 0, 0, 0);
    start_flash_operation(OPCODE_HIGH_P_MODE, 0, 0, 0);
#endif
    FLASH_IN_DP=0;    

    wait_flash_busy();
    {
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
    }  
}

