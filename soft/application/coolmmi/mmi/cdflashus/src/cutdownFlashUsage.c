/*******************coolsand,chenhe,20100819 create for cut down flash usage****/

#ifdef COMPRESS_OPTIMIZE_FLASH_SIZE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cs_types.h"
#include "mmi_trace.h"
#include "cos.h"
#include "oslmemory.h"
#include "cutdownflashusage.h"
#include "huffman.h"
#include "debuginitdef.h"
//#include "sxs_io.h"
extern  UINT32 csw_TMGetTick(VOID);

typedef void(*CDFU_CB)(void);

typedef struct
{
	CDFU_CB callback;
	BOOL	isDeCompressed;
}CDFU_APP_CB_T;

static CDFU_APP_CB_T cdfu_app_cb[CDFU_MAX_APP_CNT];

static void cdfu_register_cb(CDFU_APP_IDX idx,CDFU_CB cb)
{
	cdfu_app_cb[idx].callback = cb;
}
extern int decompress(unsigned char *buf, int in_len,
			      unsigned char *output);

void cdfu_app_init(const unsigned char *bufin,  unsigned int bufinlen, unsigned char *bufDes)
{
	int result = 0;
#ifdef COMPRESS_USE_LZMA	
	result = decompress((unsigned char *)bufin,(int)bufinlen,bufDes);
#else
	unsigned char  *bufout = NULL;
	U32 hfOutlen = 0;
	result=huffman_decode_memory(bufin,bufinlen,&bufout,&hfOutlen);
	//if (hfOutlen != bufinlen)
	//	asm("break 1");
	memcpy(bufDes,bufout,hfOutlen);
	OslMfree(bufout);
#endif	
}

void cdfu_do_init(CDFU_APP_IDX id)
{
	UINT32 compressTime=0;
	compressTime = csw_TMGetTick();
	if (id >= CDFU_MAX_APP_CNT)
		asm("break 1");
#ifdef __MMI_COMPRESS_CODE_SECTION__
	if (id < CDFU_JMETOC)
	{
		if (cdfu_app_cb[id].isDeCompressed == 1)
			return;
		else
		{
			int i=0;
			for (i=0;i<CDFU_JMETOC;i++)
			{
				cdfu_app_cb[i].isDeCompressed = 0;
			}
			cdfu_app_cb[id].callback();
			cdfu_app_cb[id].isDeCompressed = 1;			
		}
	}
	else if (id < CDFU_OVERLAY_END)
	{
		if (cdfu_app_cb[id].isDeCompressed == 1)
			return;
		else
		{
			int i=0;
			for (i=0;i<CDFU_OVERLAY_END;i++)
			{
				cdfu_app_cb[i].isDeCompressed = 0;
			}
			cdfu_app_cb[id].callback();
			cdfu_app_cb[id].isDeCompressed = 1;
		}
	}
	else
#endif		
	{
		if ((id == CDFU_QQ && cdfu_app_cb[CDFU_QQ].isDeCompressed)|| (id == CDFU_GEO && cdfu_app_cb[CDFU_GEO].isDeCompressed) )
			return;
		else if (id == CDFU_QQ)
			cdfu_app_cb[CDFU_GEO].isDeCompressed = 0;
		else if (id == CDFU_GEO)
			cdfu_app_cb[CDFU_QQ].isDeCompressed = 0;
		if (id != CDFU_RES)
		{
			if (cdfu_app_cb[id].isDeCompressed)
				return;
			else
				cdfu_app_cb[id].isDeCompressed = 1;
		}
		cdfu_app_cb[id].callback();
		cdfu_app_cb[id].isDeCompressed = 1;
	}
	compressTime = csw_TMGetTick() - compressTime;
	PRINT_INFORMATION(("cdfu_do_init %d %d ticks\n", id,compressTime));
}

void cdfu_do_deinit(CDFU_APP_IDX id,char *buff)
{
	if (cdfu_app_cb[id].isDeCompressed == 1)
		OslMfree(buff);
	cdfu_app_cb[id].isDeCompressed =0;
}

extern void ch_strdec(void);
//extern void ch_fontInit(void);
extern void cdfu_qq_init(void);
extern void cdfu_geo_init(void);
extern void cdfu_msn_init(void);
#ifdef __MMI_COMPRESS_CODE_SECTION__

unsigned char *const xcpu_overlay_space = (unsigned char *)0xa03a8000;

extern unsigned char  __load_start_xcpu_overlay_jmetoc[];
extern unsigned char  __load_stop_xcpu_overlay_jmetoc[];
unsigned char *xcpu_overlay_qq_space = (unsigned char *)0xa03a8000;

extern unsigned char _compressed_code_start;
extern unsigned char _xcpu_overlay_flash_end;

extern unsigned char jmetoc_start[];
extern unsigned char jmetoc_end[];
extern unsigned char other_start[];
extern unsigned char other_end[];
extern unsigned char csdapp_start[];
extern unsigned char csdapp_end[];
extern unsigned char init_func_start[];
extern unsigned char init_func_end[];
extern unsigned char wap_start[];
extern unsigned char wap_end[];


void cdfu_init_func_init(void)
{
	unsigned int compressed_code_size = init_func_end -  init_func_start;
	unsigned char *compressed_code_address = &_compressed_code_start + (init_func_start - &_xcpu_overlay_flash_end);
	cdfu_app_init( compressed_code_address, compressed_code_size ,xcpu_overlay_space );
}

void cdfu_csdapp_init(void)
{
	unsigned int compressed_code_size = csdapp_end -  csdapp_start;
	unsigned char *compressed_code_address = &_compressed_code_start + (csdapp_start - &_xcpu_overlay_flash_end);
	cdfu_app_init( compressed_code_address, compressed_code_size ,xcpu_overlay_space );
}

void cdfu_wap_init(void)
{
	unsigned int compressed_code_size = wap_end -  wap_start;
	unsigned char *compressed_code_address = &_compressed_code_start + (wap_start - &_xcpu_overlay_flash_end);
	cdfu_app_init( compressed_code_address, compressed_code_size ,xcpu_overlay_space );
}

void cdfu_jmetoc_init(void)
{
	unsigned int compressed_code_size = jmetoc_end -  jmetoc_start;
	unsigned char *compressed_code_address = &_compressed_code_start + (jmetoc_start - &_xcpu_overlay_flash_end);
	cdfu_app_init( compressed_code_address, compressed_code_size ,xcpu_overlay_space );
	xcpu_overlay_qq_space = xcpu_overlay_space+(__load_stop_xcpu_overlay_jmetoc - __load_start_xcpu_overlay_jmetoc);
}


void cdfu_other_init(void)
{
	unsigned int compressed_code_size = other_end -  other_start;
	unsigned char *compressed_code_address = &_compressed_code_start + (other_start - &_xcpu_overlay_flash_end);
	PRINT_INFORMATION( ( "compressed_code_size = %d\n", compressed_code_size) );
	PRINT_INFORMATION( ( "compressed_code_address = 0x%x\n", compressed_code_address) );
	cdfu_app_init( compressed_code_address, compressed_code_size ,xcpu_overlay_space );
}


 void cdfu_other(void)
{

	cdfu_register_cb(CDFU_OHTER,cdfu_other_init);
 	cdfu_do_init(CDFU_OHTER);
 }

void cdfu_boot()
{
	cdfu_register_cb(CDFU_INIT,cdfu_init_func_init);
 	cdfu_do_init(CDFU_INIT);
}

void cdfu_csdapp()
{
	cdfu_register_cb(CDFU_CSDAPP,cdfu_csdapp_init);
 	cdfu_do_init(CDFU_CSDAPP);
}

void cdfu_wap()
{
	cdfu_register_cb(CDFU_WAP,cdfu_wap_init);
 	cdfu_do_init(CDFU_WAP);
}

void cdfu_jmetoc()
{
	cdfu_register_cb(CDFU_JMETOC,cdfu_jmetoc_init);
 	cdfu_do_init(CDFU_JMETOC);
}
#endif
void cdfu_init(void)
{
#ifdef __MMI_SUPPORT_JME__
#ifdef _MMI_SUPPORT_JME_QQ_	
	cdfu_register_cb(CDFU_QQ,cdfu_qq_init);
#endif
#ifdef _MMI_SUPPORT_JME_GEO_	
	cdfu_register_cb(CDFU_GEO,cdfu_geo_init);
#endif
#ifdef _MMI_SUPPORT_JME_MSN_	
	cdfu_register_cb(CDFU_MSN,cdfu_msn_init);
#endif
#endif	
	//cdfu_register_cb(CDFU_FONT,ch_fontInit);
	cdfu_register_cb(CDFU_RES,ch_strdec);
	//we should call cdfu_do_init as later as possible
	cdfu_do_init(CDFU_RES);
	//cdfu_do_init(CDFU_FONT);
}

#endif
